/**
 * Output module of the Boat Controller.
 *
 * This module is in charge of converting numbers to pulse-length signals. It
 * performs the inverse of the radio module.
 *
 * This module can be in 5 different states. Every 5 ms, the output module
 * changes to another state in a circular fashion. Hence, the module is in a
 * given state every 25 ms. Also, each state must take less than 5 ms to
 * complete. The timer 3 is configured to call OUTPUT_change_state every 5 ms to
 * perform this task.
 *
 * Four states corresponds to 4 outputs: it expects a number in range
 * 2000..4000 and sets high one output during a period of 1 to 2 ms. To do so,
 * the timer 4 is configured using the new data to trigger an interrupt and
 * call OUTPUT_set_pin_low. Once the pin is low, timer 4 is stopped.
 *
 * The fifth state is in charge of fetching new data. The module uses
 * double-buffering technique to allow the user to update data at any time.
 * Notice that data are fetched only every 25 ms. Also, if it ensures that
 * data are in the correct range.
 */

#include <xc.h>
#include "output.h"
#include "mcc_generated_files/tmr4.h"
#include "mcc_generated_files/pin_manager.h"

#define MAX_TMR             (8000)

enum state {
    LEFT_RUDDER,
    RIGHT_RUDDER,
    LEFT_MOTOR,
    RIGHT_MOTOR,
    FETCH_DATA,
    STATE_CNT
};

static uint8_t state = FETCH_DATA;
static uint16_t data[OUTPUT_CHANNEL_CNT] = {
    NEUTRAL_POS,
    NEUTRAL_POS,
    NEUTRAL_POS,
    NEUTRAL_POS
};
static uint16_t buffer[OUTPUT_CHANNEL_CNT] = {
    NEUTRAL_POS,
    NEUTRAL_POS,
    NEUTRAL_POS,
    NEUTRAL_POS
};

static void OUTPUT_enable_interrupts(void)
{
    IEC0bits.T3IE = true;
}

static void OUTPUT_disable_interrupts(void)
{
    IEC0bits.T3IE = false;
}

void OUTPUT_set_pin_low(void)
{
    switch (state) {
    case LEFT_RUDDER:
        LEFT_RUDDER_SetLow();
        break;
    case RIGHT_RUDDER:
        RIGHT_RUDDER_SetLow();
        break;
    case LEFT_MOTOR:
        LEFT_MOTOR_SetLow();
        break;
    case RIGHT_MOTOR:
        RIGHT_MOTOR_SetLow();
        break;
    default:
        break;
    }
    TMR4_Stop();
}

static void OUTPUT_set_pin_high(void)
{
    switch (state) {
    case LEFT_RUDDER:
        LEFT_RUDDER_SetHigh();
        break;
    case RIGHT_RUDDER:
        RIGHT_RUDDER_SetHigh();
        break;
    case LEFT_MOTOR:
        LEFT_MOTOR_SetHigh();
        break;
    case RIGHT_MOTOR:
        RIGHT_MOTOR_SetHigh();
        break;
    default:
        break;
    }
}

void OUTPUT_change_state(void)
{
    ++state;
    if (state == STATE_CNT)
        state = 0;

    if (state == FETCH_DATA) {
        uint8_t i;
        for (i = 0; i < OUTPUT_CHANNEL_CNT; ++i)
            data[i] = buffer[i];
    } else {
        /*
         * The timer 4 raises an interrupt only when it overflows. Hence, we
         * need to ensure that it increments its counter data[state] times.
         */
        TMR4_Counter16BitSet(MAX_TMR - data[state]);
        OUTPUT_set_pin_high();
        TMR4_Start();
    }
}

void OUTPUT_set_data(uint16_t *new_data)
{
    uint8_t i;

    /*
     * Disabling interrupts to ensure that OUTPUT_update is not called in
     * FETCH_DATA state while updating buffer.
     */
    OUTPUT_disable_interrupts();

    for (i = 0; i < OUTPUT_CHANNEL_CNT; ++i) {
        uint16_t value = new_data[i];
        if (value < MIN_POS)
            value = MIN_POS;
        else if (value > MAX_POS)
            value = MAX_POS;
        buffer[i] = value;
    }

    OUTPUT_enable_interrupts();
}
