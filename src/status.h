/*
 * File:   status.h
 * Author: francois
 *
 * Created on 30 mai 2016, 13:21
 */

#ifndef STATUS_H
#define	STATUS_H

#ifdef	__cplusplus
extern "C" {
#endif

enum
{
    STATUS_OFF,
    STATUS_ON,
    STATUS_SLOW_BLINK,      /* Switch on LED for 100 ms every 2 seconds */
    STATUS_FAST_BLINK       /* Blink LED at 5 Hz */
};

/**
 * @brief Change the mode of the status LED.
 *
 * At startup, the LED is off.
 *
 * @param[in] new_mode Must be one of the four modes.
 */
void STATUS_set_mode(char new_mode);

/**
 * @brief Update the state of the status LED depending on the current mode.
 *
 * This function should only be called by the timer 2 interrupt handler.
 */
void STATUS_update(void);


#ifdef	__cplusplus
}
#endif

#endif	/* STATUS_H */

