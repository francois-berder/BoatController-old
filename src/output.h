/*
 * File:   output.h
 * Author: francois
 *
 * Created on 5 juin 2016, 14:53
 */

#ifndef OUTPUT_H
#define OUTPUT_H

#ifdef    __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define LEFT_RUDDER_CHANNEL     (0)
#define RIGHT_RUDDER_CHANNEL    (1)
#define LEFT_MOTOR_CHANNEL      (2)
#define RIGHT_MOTOR_CHANNEL     (3)
#define OUTPUT_CHANNEL_CNT      (4)

#define MIN_POS                 (2000)
#define NEUTRAL_POS             (3000)
#define MAX_POS                 (4000)

/**
 * @brief Change and execute next state.
 *
 * This function must only be called by TMR3 callback.
 */
void OUTPUT_change_state(void);

/**
 * @brief Update buffer.
 *
 * It does not use the new data immediately. Data will be fetched during the
 * FETCH_DATA state.
 *
 * @param[in] data Array of length 4. All elements must be in range 2000..4000.
 */
void OUTPUT_set_data(uint16_t *data);

/**
 * @brief Set current pin to low.
 *
 * This function must only be called by TMR4 callback.
 */
void OUTPUT_set_pin_low(void);

#ifdef    __cplusplus
}
#endif

#endif    /* OUTPUT_H */
