/*
 * File:   hal_sd.h
 * Author: francois
 *
 * Created on 20 April 2017, 20:01
 */

#ifndef HAL_SD_H
#define    HAL_SD_H

#ifdef    __cplusplus
extern "C" {
#endif

#include <stdint.h>

    /**
     * @brief Initialize the cache, the current address and the start sector
     *
     * @param _start_sector Index of the first sector of a FAT16 partition
     */
    void hal_init(uint32_t _start_sector);

    /**
     * @brief Read some bytes.
     *
     * @param buffer Buffer of size @p length
     * @param length Number of bytes to read
     */
    int hal_read(uint8_t *buffer, uint32_t length);

    /**
     * @brief Read one byte.
     *
     * @param data Pointer to store the byte read at current address
     */
    int hal_read_byte(uint8_t *data);

    /**
     * @brief Change the current address.
     *
     * No checks are performed to ensure that the address is still within the
     * bounds of the FAT16 partition.
     *
     * @param address Address relative to the start of the FAT16 partition
     */
    int hal_seek(uint32_t address);

    /**
     * @brief Write some bytes.
     *
     * Note that it does not write the data to the SD card if all blocks can
     * be stored in the cache.
     *
     * @param buffer Buffer of size @p length
     * @param length Number of bytes to write
     */
    int hal_write(uint8_t *buffer, uint32_t length);

    /**
     * @brief Flush the cache.
     *
     * Any dirty blocks in the cache is written to the SD card. This might take
     * a long time to complete.
     */
    void hal_flush(void);

#ifdef    __cplusplus
}
#endif

#endif    /* HAL_SD_H */

