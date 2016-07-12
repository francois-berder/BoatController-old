#include <stdbool.h>
#include "radio.h"
#include "output.h"
#include "log.h"
#define QUEUE_SIZE      (3)
#define INPUT_VALID_BIT_MASK    (0xFFF0)

static struct {
    uint16_t last[CHANNEL_CNT][QUEUE_SIZE];
    uint16_t value[CHANNEL_CNT];
} input_data;

static bool get_input(uint8_t channel)
{
    bool change = false;
   
    while (!RADIO_is_buffer_empty(channel)) {
        change = true;
        
        uint16_t raw = RADIO_buffer_read(channel);
        
        /* Do the average of the new value and the last three values. */
        uint16_t filtered = raw;
        filtered += input_data.last[channel][0];
        filtered += input_data.last[channel][1];
        filtered += input_data.last[channel][2];
        filtered >>= 2;
        
        /* Remove noise by ignoring some bits */
        filtered &= INPUT_VALID_BIT_MASK;
        LOG_DBG("channel %u: %u (raw=)", channel, filtered, raw);
        
        input_data.last[channel][2] = input_data.last[channel][1];
        input_data.last[channel][1] = input_data.last[channel][0];
        input_data.last[channel][0] = raw;
    }
    
    return change;
}
/**
 * Do the simplest thing possible, just give values directly to output module
 * without using data from accelerometer/gyroscope.
 */
static void compute_output(void)
{
    uint16_t output_data[OUTPUT_CHANNEL_CNT];
    
    LOG_DBG("output: %u\t%u", input_data.value[DIRECTION_CHANNEL],
             input_data.value[SPEED_CHANNEL]);
    output_data[LEFT_RUDDER_CHANNEL] = input_data.value[DIRECTION_CHANNEL];
    output_data[RIGHT_RUDDER_CHANNEL] = input_data.value[DIRECTION_CHANNEL];
    output_data[LEFT_MOTOR_CHANNEL] = input_data.value[SPEED_CHANNEL];
    output_data[RIGHT_MOTOR_CHANNEL] = input_data.value[SPEED_CHANNEL];

    OUTPUT_set_data(output_data);
}

void simple_controller_update(void)
{
    bool change = false;
    if (get_input(DIRECTION_CHANNEL))
        change = true;
    if (get_input(SPEED_CHANNEL))
        change = true;
    
    if (change)
        compute_output();
}