#include <xc.h>
#include "mcc_generated_files/ic1.h"
#include "mcc_generated_files/ic2.h"
#include "log.h"
#include "radio.h"

#define BUFFER_SIZE     (16)

static uint16_t start[CHANNEL_CNT] = { 0, 0 };

static struct 
{
    uint16_t entries[BUFFER_SIZE];
    uint8_t entries_cnt;
    uint8_t head;                   /* position of oldest entry */
} buffer[CHANNEL_CNT];

static bool RADIO_is_buffer_full(const uint8_t channel)
{
    return buffer[channel].entries_cnt == BUFFER_SIZE;
}

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

static uint16_t RADIO_read_ic(const uint8_t channel)
{
    if (channel == DIRECTION_CHANNEL)
        return IC1_CaptureDataRead();
    else if (channel == SPEED_CHANNEL)
        return IC2_CaptureDataRead();
    else {
        LOG_ERR("radio: Cannot read from IC with invalid channel");
        return 0;
    }
}

static void RADIO_add_entry(const uint8_t channel, const uint16_t data)
{
    uint8_t index;
    
    if (RADIO_is_buffer_full(channel)) {
        LOG_ERR("radio: Dropping data on channel %d", channel);
        return;
    }
    
    index = (buffer[channel].head + buffer[channel].entries_cnt) % BUFFER_SIZE;
    buffer[channel].entries[index] = data;
    ++buffer[channel].entries_cnt;
}

static uint8_t RADIO_read_input_pin(const uint8_t channel)
{
    if (channel == DIRECTION_CHANNEL)
        return PORTBbits.RB0;
    else if (channel == SPEED_CHANNEL)
        return PORTBbits.RB1;
    else {
        LOG_ERR("radio: Cannot check input pin state with invalid channel");
        return 1;
    }
}

void RADIO_update(const uint8_t channel)
{
    if (RADIO_read_input_pin(channel) == 1) {
            start[channel] = RADIO_read_ic(channel);
    } else {
        uint16_t entry = RADIO_read_ic(channel) - start[channel];
        entry >>= 3;
        RADIO_add_entry(channel, entry);
    }
}

bool RADIO_is_buffer_empty(const uint8_t channel)
{
    return buffer[channel].entries_cnt == 0;
}

uint16_t RADIO_buffer_read(const uint8_t channel)
{
    uint16_t data = 0;
    uint8_t index = 0;
    
    RADIO_disable_interrupts(channel);
    
    if (RADIO_is_buffer_empty(channel)) {
        LOG_WARN("radio: read from empty buffer on channel %d", channel);
        RADIO_enable_interrupts(channel);
        return 0;
    }
    index = buffer[channel].head;
    buffer[channel].head = (buffer[channel].head + 1) % BUFFER_SIZE;
    --buffer[channel].entries_cnt;
    data = buffer[channel].entries[index];
    
    RADIO_enable_interrupts(channel);
    
    return data;
}