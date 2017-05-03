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

    /**
     * @brief Initialize the SD card.
     *
     * Configure SD card in SPI mode, discard CRC and set block length
     * to BLOCK_LENGTH.
     *
     * @return 0 if successful, 1 if it failed
     */
    uint8_t sd_init(void);

    /**
     * @brief Read one block from the SD card.
     *
     * @param buffer Buffer of size BLOCK_LENGTH
     * @param sector Index of the sector
     *
     * @return 0 if successful, 1 otherwise
     */
    uint8_t sd_read_block(uint8_t *buffer, uint32_t sector);

    /**
     * @brief Read one block from the SD card and stores a subset in buffer.
     *
     * While this returns only a subset of a sector, the entire sector must still
     * be read. Hence, this function is not faster than sd_read_block.
     *
     * @param buffer Buffer of size @p length
     * @param sector Index of the sector
     * @param offset Offset in bytes in the sector
     * @param length Number of bytes to store in buffer
     * @return 0 if succesful, 1 otherwise
     */
    uint8_t sd_read_subblock(uint8_t *buffer, uint32_t sector, uint16_t offset, uint16_t length);

    /**
     * @brief Writes one block to the SD card.
     *
     * This call is blocking and will wait until the SD card has finished writing
     * a sector. This might take a few milliseconds to complete.
     *
     * @param buffer Buffer of size BLOCK_LENGTH
     * @param sector Index of the sector
     * @return 0 if successful, 1 otherwise
     */
    uint8_t sd_write_block(uint8_t *buffer, uint32_t sector);

#ifdef	__cplusplus
}
#endif

#endif	/* SD_H */
