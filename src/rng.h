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

    /**
     * @brief Generate one random byte.
     *
     * The crypto module is configured to generate random values and one byte
     * is read. Then, the module is immediately turned off.
     *
     * @return A random byte
     */
    uint8_t rng_get_byte(void);

    /**
     * @brief Fill buffer with random data.
     *
     * This is convenient wrapper around rng_get_byte to fill a buffer with
     * random values.
     *
     * @param buffer Buffer of size @p length
     * @param length Number of random bytes to generate
     */
    void rng_get_bytes(uint8_t *buffer, uint32_t length);

#ifdef	__cplusplus
}
#endif

#endif	/* RNG_H */

