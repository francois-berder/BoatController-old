#include <string.h>
#include "hal_sd.h"
#include "log.h"
#include "sd.h"

/**
 * Sector 0 is reserved for MBR. Hence, no partition
 * should ever request sector 0.
 */
#define INVALID_SECTOR_INDEX            (0)
#define CACHE_ENTRY_COUNT               (4)

static uint32_t current_address = 0; /* In byte */

/*
 * First sector of the FAT16 partition, This value is found
 * when reading the Master Boot Record partition table.
 */
static uint32_t start_sector = INVALID_SECTOR_INDEX;

struct sector {
    uint8_t data[BLOCK_LENGTH];
    uint32_t index;
    uint16_t last_usage; /**
                             * This variable is used to determine which entry
                             * is the oldest. A low value means the sector was
                             * used not long ago.
                             */
};

static struct sector sectors[CACHE_ENTRY_COUNT];

/* Assume that each sector is 512 bytes long */
static inline uint32_t get_sector_from_address(uint32_t address)
{
    return start_sector + (address >> 9);
}

static inline uint32_t get_offset_from_address(uint32_t address)
{
    return current_address & (BLOCK_LENGTH - 1);
}

static void update_last_usage(uint8_t index)
{
    uint8_t i;

    for (i = 0; i < CACHE_ENTRY_COUNT; ++i) {
        if (i == index)
            sectors[i].last_usage = 0;
        else if (sectors[i].index != INVALID_SECTOR_INDEX)
            ++sectors[i].last_usage;
    }
}

static uint8_t get_entry_index(uint32_t sector_index)
{
    uint8_t i;

    /* Check if the sector is already loaded*/
    for (i = 0; i < CACHE_ENTRY_COUNT; ++i) {
        if (sectors[i].index == sector_index)
            return i;
    }

    /* Try to find empty slot in cache */
    for (i = 0; i < CACHE_ENTRY_COUNT; ++i) {
        if (sectors[i].index == INVALID_SECTOR_INDEX)
            break;
    }

    /* If the cache is full, evict one entry */
    if (i == CACHE_ENTRY_COUNT) {
        uint16_t lu = 0;
        uint8_t j;
        for (j = 0; j < CACHE_ENTRY_COUNT; ++j) {
            if (sectors[j].last_usage >= lu) {
                lu = sectors[j].last_usage;
                i = j;
            }
        }
    }

    /* Load sector from SD card */
    LOG_DBG("hal_sd: Loading sector 0x%08lX in entry %u", sector_index, i);
    if (sd_read_block(sectors[i].data, sector_index)) {
        sectors[i].index = INVALID_SECTOR_INDEX;
        return 1;
    }
    sectors[i].index = sector_index;
    sectors[i].last_usage = 0;

    return i;
}

void hal_init(uint32_t _start_sector)
{
    uint8_t i;

    start_sector = _start_sector;

    for (i = 0; i < CACHE_ENTRY_COUNT; ++i)
        sectors[i].index = INVALID_SECTOR_INDEX;
}

int hal_read(uint8_t *buffer, uint32_t length)
{
    uint32_t count = 0;
    LOG_DBG("hal_sd: Reading %lu bytes at address %08lX.", length, current_address);
    while (count < length) {
        uint32_t chunk = 0, bytes_remaining_sector = 0;
        uint32_t offset = get_offset_from_address(current_address);
        uint32_t current_sector_index = get_sector_from_address(current_address);

        uint8_t index = get_entry_index(current_sector_index);

        /* Copy as many bytes from sector to buffer */
        chunk = length;
        bytes_remaining_sector = BLOCK_LENGTH - offset;
        if (chunk > bytes_remaining_sector)
            chunk = bytes_remaining_sector;

        memcpy(buffer, &sectors[index].data[offset], chunk);
        current_address += chunk;
        count += chunk;
        buffer += chunk;

        update_last_usage(index);
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

        uint8_t index = get_entry_index(current_sector_index);

        /* Copy as many bytes from buffer to sector */
        chunk = length;
        bytes_remaining_sector = BLOCK_LENGTH - offset;
        if (chunk > bytes_remaining_sector)
            chunk = bytes_remaining_sector;

        memcpy(&sectors[index].data[offset], buffer, chunk);
        current_address += chunk;
        count += chunk;
        buffer += chunk;

        if (sd_write_block(sectors[index].data, current_sector_index))
            return -1;

        update_last_usage(index);
    }

    return 0;
}