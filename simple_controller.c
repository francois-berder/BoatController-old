#include <stdbool.h>
#include <string.h>
#include "radio.h"
#include "output.h"
#include "log.h"
#define QUEUE_SIZE      (1)
#define INPUT_VALID_BIT_MASK    (0xFFF0)

static struct {
    uint16_t last[CHANNEL_CNT][QUEUE_SIZE];
    uint16_t value[CHANNEL_CNT];
} input_data;

static bool get_input(uint8_t channel)
{
    bool change = false;
   
    while (!RADIO_is_buffer_empty(channel)) {
        uint8_t i;
        uint16_t raw, filtered;
        change = true;
        
        raw = RADIO_buffer_read(channel);
        
        /* Do the average of the new value and the last three values. */
        filtered = raw;
        for (i = 0; i < QUEUE_SIZE; ++i)
            filtered += input_data.last[channel][i];

        /* Assume that QUEUE_SIZE = 1 */
        filtered >>= 1;
        
        /* Remove noise by ignoring some bits */
        filtered &= INPUT_VALID_BIT_MASK;
        //LOG_DBG("channel %u: %u (raw=%u)", channel, filtered, raw);
        
        for (i = QUEUE_SIZE - 1; i > 0; --i)
            input_data.last[channel][i] = input_data.last[channel][i-1];

        input_data.last[channel][0] = raw;
        input_data.value[channel] = filtered;
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
    
    /* Invert left/right for direction channel */
	uint16_t inverted_dir = MAX_POS + MIN_POS - input_data.value[DIRECTION_CHANNEL];

    //LOG_DBG("output: %u\t%u", inverted_dir, input_data.value[SPEED_CHANNEL]);
    output_data[LEFT_RUDDER_CHANNEL] = inverted_dir;
    output_data[RIGHT_RUDDER_CHANNEL] = inverted_dir;
    output_data[LEFT_MOTOR_CHANNEL] = input_data.value[SPEED_CHANNEL];
    output_data[RIGHT_MOTOR_CHANNEL] = input_data.value[SPEED_CHANNEL];

    OUTPUT_set_data(output_data);
}

void simple_controller_init(void)
{
	uint8_t i = 0;
	
	input_data.value[DIRECTION_CHANNEL] = NEUTRAL_POS;
	input_data.value[SPEED_CHANNEL] = NEUTRAL_POS;

	for (i = 0; i < QUEUE_SIZE; ++i) {
		input_data.last[DIRECTION_CHANNEL][i] = NEUTRAL_POS;
		input_data.last[SPEED_CHANNEL][i] = NEUTRAL_POS;
	}
	
    compute_output();
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