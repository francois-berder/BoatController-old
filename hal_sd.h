/*
 * File:   hal_sd.h
 * Author: francois
 *
 * Created on 20 April 2017, 20:01
 */

#ifndef HAL_SD_H
#define	HAL_SD_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

    void hal_set_start_sector(uint32_t _start_sector);
    int hal_read(uint8_t *buffer, uint32_t length);
    int hal_read_byte(uint8_t *data);
    int hal_seek(uint32_t address);
    int hal_write(uint8_t *buffer, uint32_t length);

#ifdef	__cplusplus
}
#endif

#endif	/* HAL_SD_H */

