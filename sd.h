/*
 * File:   sd.h
 * Author: francois
 *
 */

#ifndef SD_H
#define	SD_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define BLOCK_LENGTH    (512)

    uint8_t sd_init(void);

    uint8_t sd_read_block(uint8_t *buffer, uint32_t sector);

#ifdef	__cplusplus
}
#endif

#endif	/* SD_H */
