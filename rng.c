#include <xc.h>
#include "rng.h"

#define ON          (1 << 15)
#define RNG_MODE    (0b1010 << 4)
#define GO          (1 << 8)

uint8_t rng_get_byte(void)
{
    uint8_t data;

    CRYCONL = ON;

    CRYCONL |= RNG_MODE;
    CRYCONL |= GO;
    while (CRYCONL & GO)
        ;

    data = CRYTXTA;

    CRYCONL &= ~ON;

    return data;
}

void rng_get_bytes(uint8_t *buffer, uint32_t length)
{
    while (length) {
        *buffer = rng_get_byte();

        --length;
        ++buffer;
    }
}