/*
 * File:   mbr.h
 * Author: francois
 *
 * Created on 20 April 2017, 18:24
 */

#ifndef MBR_H
#define    MBR_H

#ifdef    __cplusplus
extern "C" {
#endif

#include <stdint.h>

    /**
     * @brief Read the MBR at sector 0 present on the
     * SD card. It assumes that the sector size is 512.
     *
     * @return 0 if an error occurred. Otherwise, it
     * returns the start sector of the first FAT16
     * partition found.
     */
    uint32_t mbr_read(void);

#ifdef    __cplusplus
}
#endif

#endif    /* MBR_H */

