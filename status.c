/**
 * Status module of the Boat Controller.
 *
 * This module is in charge of the status LED which can be configured to be on,
 * off or blinking.
 * This module relies on the timer 2, configured to call STATUS_update every
 * 100 ms, to achieve blinking the LED.
 */

#include "mcc_generated_files/pin_manager.h"
#include "status.h"
#include "log.h"

#define SLOW_BLINK_PERIOD       20

static char mode = STATUS_OFF;
static char period = 1;

void STATUS_set_mode(char new_mode)
{
    LOG_DBG("Status LED changing to mode %d", new_mode);

    mode = new_mode;

    if (mode == STATUS_SLOW_BLINK) {
        period = 0;
        STATUS_LED_SetHigh();
    }
}

void STATUS_update(void)
{
    switch (mode) {
        case STATUS_OFF:
            STATUS_LED_SetLow();
            break;
        case STATUS_ON:
            STATUS_LED_SetHigh();
            break;
        case STATUS_SLOW_BLINK:
            ++period;
            if ((period == 1 && _LATB5)
             || (period == SLOW_BLINK_PERIOD && _LATB5 == 0)) {
                STATUS_LED_Toggle();
                period = 0;
            }
            break;
        case STATUS_FAST_BLINK:
            STATUS_LED_Toggle();
            break;
    }
}
