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
#define POWER_CHANNEL           (1)
#define CHANNEL_CNT             (2)
    
void radio_update(const uint8_t channel);
bool radio_is_buffer_empty(const uint8_t channel);
uint16_t radio_buffer_read(const uint8_t channel);

#ifdef	__cplusplus
}
#endif

#endif	/* RADIO_H */
