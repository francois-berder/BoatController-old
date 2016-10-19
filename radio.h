/*
 * File:   radio.h
 * Author: francois
 *
 * Created on 1 juin 2016, 20:09
 */

#ifndef RADIO_H
#define	RADIO_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define DIRECTION_CHANNEL       (0)
#define SPEED_CHANNEL           (1)
#define CHANNEL_CNT             (2)

/**
 * @brief Initialise variables to zero.
 *
 * Note that this function must be called before SYSTEM_Initialize()
 */
void RADIO_init(void);

/**
 * @brief Process an edge.
 *
 * This function should always be called from interrupt handler of Interrupt
 * On Change gpios connected to the radio.
 *
 * If this is a rising edge, the value of Timer5 is recorded. Otherwise,
 * an entry is added in the buffer.
 *
 * @param[in] channel Index of the channel must be 0 or 1
 */
void RADIO_update(const uint8_t channel);

/**
 * @brief Check if a buffer is empty.
 *
 * @param[in] channel Index of the channel must be 0 or 1
 */
bool RADIO_is_buffer_empty(const uint8_t channel);

/**
 * @brief Retrieve one value from a buffer.
 *
 * Before calling this function, the buffer should always be checked if it is
 * not empty. If that is the case, 0 is returned and a warning is printed.
 *
 * @param[in] channel Index of the channel must be 0 or 1
 * @return Value in range 2000..4000, 0 if buffer is empty.
 */
uint16_t RADIO_buffer_read(const uint8_t channel);

#ifdef	__cplusplus
}
#endif

#endif	/* RADIO_H */

