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

#ifdef	__cplusplus
}
#endif

#endif	/* SD_H */
