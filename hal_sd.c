#include <string.h>
#include "hal_sd.h"
#include "log.h"
#include "sd.h"

/**
 * Sector 0 is reserved for MBR. Hence, no partition
 * should ever request sector 0.
 */
#define INVALID_SECTOR_INDEX            (0)

static uint8_t sector[BLOCK_LENGTH];
static uint32_t sector_index = INVALID_SECTOR_INDEX;
static uint32_t current_address = 0; /* In byte */
static uint32_t start_sector = INVALID_SECTOR_INDEX;

/* Assume that each sector is 512 bytes long */
static inline uint32_t get_sector_from_address(uint32_t address)
{
    return start_sector + (address >> 9);
}

static inline uint32_t get_offset_from_address(uint32_t address)
{
    return current_address & (BLOCK_LENGTH - 1);
}

/**
 * @brief Load a sector from the SD card.
 *
 * @param[in] index Sector index to load
 * @return 0 if successful, 1 otherwise
 */
static uint8_t load_sector(uint32_t index)
{
    LOG_DBG("Loading sector %lu...", index);
    if (sd_read_block(sector, index)) {
        sector_index = INVALID_SECTOR_INDEX;
        return 1;
    }

    sector_index = index;
    return 0;
}

void hal_set_start_sector(uint32_t _start_sector)
{
    start_sector = _start_sector;
}

int hal_read(uint8_t *buffer, uint32_t length)
{
    uint32_t count = 0;
    LOG_DBG("hal_sd: Reading %lu bytes at address %08lX.", length, current_address);
    while (count < length) {
        uint32_t chunk = 0, bytes_remaining_sector = 0;
        uint32_t offset = get_offset_from_address(current_address);
        uint32_t current_sector_index = get_sector_from_address(current_address);

        /* Check if sector is already loaded */
        if (current_sector_index != sector_index) {
            if (load_sector(current_sector_index))
                return -1;
        }

        /* Copy as many bytes from sector to buffer */
        chunk = length;
        bytes_remaining_sector = BLOCK_LENGTH - offset;
        if (chunk > bytes_remaining_sector)
            chunk = bytes_remaining_sector;

        memcpy(buffer, &sector[offset], chunk);
        current_address += chunk;
        count += chunk;
        buffer += chunk;
    }

    return 0;
}

int hal_read_byte(uint8_t *data)
{
    return hal_read(data, 1);
}

int hal_seek(uint32_t address)
{
    current_address = address;
    return 0;
}

int hal_write(uint8_t *buffer, uint32_t length)
{
    uint32_t count = 0;
    LOG_DBG("hal_sd: Writing %lu bytes at address %08lX.", length, current_address);
    while (count < length) {
        uint32_t chunk = 0, bytes_remaining_sector = 0;
        uint32_t offset = get_offset_from_address(current_address);
        uint32_t current_sector_index = get_sector_from_address(current_address);

        /* Check if sector is already loaded */
        if (current_sector_index != sector_index) {
            if (load_sector(current_sector_index))
                return -1;
        }

        /* Copy as many bytes from sector to buffer */
        chunk = length;
        bytes_remaining_sector = BLOCK_LENGTH - offset;
        if (chunk > bytes_remaining_sector)
            chunk = bytes_remaining_sector;

        memcpy(&sector[offset], buffer, chunk);
        current_address += chunk;
        count += chunk;
        buffer += chunk;

        if (sd_write_block(sector, current_sector_index))
            return -1;
    }

    return 0;
}