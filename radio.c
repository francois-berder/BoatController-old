/**
 * Radio module of the Boat Controller.
 *
 * The radio module is in charge of converting signals from the
 * radio receiver to numbers. It computes the length of the pulse
 * on two channels.
 *
 * The pulse length is expected to be in range 1-2ms (neutral: 1.5ms).
 * Using Interrupt on Change to detect edges and by recording the
 * value of Timer 5 at both edges. It is possible to infer the length
 * of the pulse.
 * The Timer 5 is configured such that:
 *
 *      Joystick: Left                Neutral              Right
 *  Pulse Length: 1 ms  -------------- 1.5ms -------------- 2 ms
 *  Timer5 value: 2000                  3000                4000
 *
 * It has been found that the radio receiver skips pulses on channel 3. Hence,
 * it must not be connected to any of the inputs of the radio module. It also
 * discard any invalid inputs.
 *
 * The radio receiver sends a pulse on each channel every 25ms. Considering
 * that the radio overwrites existing data if the buffer is full, it is
 * expected that the user will read fast enough from the buffer. Hence,
 * it has up to (BUFFER_SIZE * 25) ms to read the data.
 */


#include <xc.h>
#include <string.h>
#include "mcc_generated_files/ic1.h"
#include "mcc_generated_files/ic2.h"
#include "log.h"
#include "radio.h"

#define BUFFER_SIZE     (16)        /* Must be a power of 2 */

static uint16_t start[CHANNEL_CNT] = { 0, 0 };
static bool pin_high[CHANNEL_CNT] = { false, false };

static struct
{
    uint8_t entries_cnt;
    uint8_t first_entry_index;
    uint16_t entries[BUFFER_SIZE];
} buffer[CHANNEL_CNT] = {
    { 0, 0 },
    { 0, 0 }
};

static void RADIO_enable_interrupts(const uint8_t channel)
{
    if (channel == DIRECTION_CHANNEL)
        IEC0bits.IC1IE = true;
    else if (channel == SPEED_CHANNEL)
        IEC0bits.IC2IE = true;
    else
        LOG_ERR("radio: cannot enable interrupts with invalid channel.");
}

static void RADIO_disable_interrupts(const uint8_t channel)
{
    if (channel == DIRECTION_CHANNEL)
        IEC0bits.IC1IE = false;
    else if (channel == SPEED_CHANNEL)
        IEC0bits.IC2IE = false;
    else
        LOG_ERR("radio: cannot disable interrupts with invalid channel.");
}

/**
 * @brief Returns value recorded by IoC module using Timer 5.
 *
 * @param[in] channel
 * @return 16bit value from Timer 5
 */
static uint16_t RADIO_read_ic(const uint8_t channel)
{
    if (channel == DIRECTION_CHANNEL) {
        return IC1_CaptureDataRead();
    }
    else if (channel == SPEED_CHANNEL)
        return IC2_CaptureDataRead();
    else {
        LOG_ERR("radio: Cannot read from IC with invalid channel");
        return 0;
    }
}

/**
 * @brief Add an entry to the buffer.
 *
 * Notice that if the buffer is full, it overwrites existing data.
 *
 * @param[in] channel
 * @param[in] data
 */
static void RADIO_add_entry(const uint8_t channel, const uint16_t data)
{
    uint8_t index = (buffer[channel].first_entry_index + buffer[channel].entries_cnt) % BUFFER_SIZE;
    buffer[channel].entries[index] = data;
    ++buffer[channel].entries_cnt;
}

void RADIO_update(const uint8_t channel)
{
    if (pin_high[channel] == false) {
		if(channel == DIRECTION_CHANNEL) {
			while (!IC1_IsCaptureBufferEmpty())
				start[channel] = RADIO_read_ic(channel);
		} else {
			while (!IC2_IsCaptureBufferEmpty())
				start[channel] = RADIO_read_ic(channel);
		}
        pin_high[channel] = true;

        /* interrupt on falling edge */
        if(channel == DIRECTION_CHANNEL)
            IC1CON1bits.ICM = 0x2;
        else
            IC2CON1bits.ICM = 0x2;
    } else {
        uint16_t end, entry;
		if(channel == DIRECTION_CHANNEL) {
			while (!IC1_IsCaptureBufferEmpty())
				end = RADIO_read_ic(channel);
		} else {
			while (!IC2_IsCaptureBufferEmpty())
				end = RADIO_read_ic(channel);
		}
        entry = end - start[channel];
        //LOG_DBG("entry%u= %u, start= %u, end= %u\n", channel, entry, start[channel], end);

        pin_high[channel] = false;

        /* interrupt on rising edge */
        if(channel == DIRECTION_CHANNEL)
            IC1CON1bits.ICM = 0x3;
        else
            IC2CON1bits.ICM = 0x3;

        /* Ignore any invalid inputs */
        if (!(entry < 2000 || entry > 4000))
			RADIO_add_entry(channel, entry);
    }
}

bool RADIO_is_buffer_empty(const uint8_t channel)
{
    return buffer[channel].entries_cnt == 0;
}

uint16_t RADIO_buffer_read(const uint8_t channel)
{
    uint8_t index = 0;

    if (RADIO_is_buffer_empty(channel)) {
        LOG_WARN("radio: read from empty buffer on channel %d", channel);
        return 0;
    }
    index = buffer[channel].first_entry_index;

	/* first_entry_index = (first_entry_index + 1) % BUFFER_SIZE */
    buffer[channel].first_entry_index = (buffer[channel].first_entry_index + 1) & (BUFFER_SIZE - 1);

    /* Prevent an interrupt to add an entry and increment entries_cnt */
    RADIO_disable_interrupts(channel);
    --buffer[channel].entries_cnt;
    RADIO_enable_interrupts(channel);

    return buffer[channel].entries[index];
}
