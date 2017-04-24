/* 
 * File:   rng.h
 * Author: francois
 *
 * Created on 24 April 2017, 17:44
 */

#ifndef RNG_H
#define	RNG_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdint.h>

    uint8_t rng_get_byte(void);
    void rng_get_bytes(uint8_t *buffer, uint32_t length);

#ifdef	__cplusplus
}
#endif

#endif	/* RNG_H */

