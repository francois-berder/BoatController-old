#include <xc.h>
#include "mcc_generated_files/ic1.h"
#include "mcc_generated_files/ic2.h"
#include "log.h"
#include "radio.h"

#define BUFFER_SIZE     (16)

uint16_t start[CHANNEL_CNT] = { 0, 0 };

struct 
{
    uint16_t entries[BUFFER_SIZE];
    uint8_t entries_cnt;
    uint8_t head;                   /* position of oldest entry */
} buffer[CHANNEL_CNT];

static bool radio_is_buffer_full(const uint8_t channel)
{
    return buffer[channel].entries_cnt == BUFFER_SIZE;
}

static void radio_enable_interrupts(const uint8_t channel)
{
    if (channel == DIRECTION_CHANNEL)
        IPC0bits.IC1IP = 1;
    else if (channel == POWER_CHANNEL)
        IPC1bits.IC2IP = 1;
    else
        LOG_ERR("radio: cannot enable interrupts with invalid channel.");
}

static void radio_disable_interrupts(const uint8_t channel)
{
    if (channel == DIRECTION_CHANNEL)
        IPC0bits.IC1IP = 0;
    else if (channel == POWER_CHANNEL)
        IPC1bits.IC2IP = 0;
    else
        LOG_ERR("radio: cannot disable interrupts with invalid channel.");
}

static uint16_t radio_read_ic(const uint8_t channel)
{
    if (channel == DIRECTION_CHANNEL)
        return IC1_CaptureDataRead();
    else if (channel == POWER_CHANNEL)
        return IC2_CaptureDataRead();
    else {
        LOG_ERR("radio: Cannot read from IC with invalid channel");
        return 0;
    }
}

static void radio_add_entry(const uint8_t channel, const uint16_t data)
{
    uint8_t index;
    
    if (radio_is_buffer_full(channel)) {
        LOG_ERR("radio: Dropping data on channel %d", channel);
        return;
    }
    
    index = (buffer[channel].head + buffer[channel].entries_cnt) % BUFFER_SIZE;
    buffer[channel].entries[index] = data;
    ++buffer[channel].entries_cnt;
}

static uint8_t radio_read_input_pin(const uint8_t channel)
{
    if (channel == DIRECTION_CHANNEL)
        return PORTBbits.RB0;
    else if (channel == POWER_CHANNEL)
        return PORTBbits.RB1;
    else {
        LOG_ERR("radio: Cannot check input pin state with invalid channel");
        return 1;
    }
}

void radio_update(const uint8_t channel)
{
    if (radio_read_input_pin(channel) == 1) {
            start[channel] = radio_read_ic(channel);
    } else {
        uint16_t data;
        uint16_t end = radio_read_ic(channel);

        if (end >= start[channel])
            data = end - start[channel];
        else
            data = start[channel] - end;

        radio_add_entry(channel, data);
    }
}

bool radio_is_buffer_empty(const uint8_t channel)
{
    return buffer[channel].entries_cnt == 0;
}

uint16_t radio_buffer_read(const uint8_t channel)
{
    uint16_t data = 0;
    uint8_t index = 0;
    
    radio_disable_interrupts(channel);
    
    if (radio_is_buffer_empty(channel)) {
        LOG_WARN("radio: read from empty buffer on channel %d", channel);
        radio_enable_interrupts(channel);
        return 0;
    }
    index = buffer[channel].head;
    buffer[channel].head = (buffer[channel].head + 1) % BUFFER_SIZE;
    --buffer[channel].entries_cnt;
    data = buffer[channel].entries[index];
    
    radio_enable_interrupts(channel);
    
    return data;
}