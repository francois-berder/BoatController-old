/**
 * SD card cache module of the Boat Controller.
 *
 * This module is in charge of managing a small cache to reduce the number of
 * read and write to the SD card because these operations take a lot time to
 * complete.
 *
 * This is a write-back cache, it does not write blocks to the SD card unless:
 *   - the block is dirty and hal_flush is called
 *   - the block is dirty and is evicted from the cache
 *
 * A block becomes dirty if it has been written at least once. This prevents
 * unnecessary writes to the SD card.
 *
 * This module also stores the current address, enabling users to read/write
 * data without specifying the address. The address gets incremented after any
 * of these operations. Also, the address is relative to the start of a FAT16
 * partition. In other words, address 0 is the first byte of FAT16 partition.
 * There are no checks if the address is not within the bounds of the partition.
 */

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
#define DIRTY_FLAG                      (0x01)

static uint32_t current_address = 0; /* In byte */

/*
 * First sector of the FAT16 partition, This value is found
 * when reading the Master Boot Record partition table.
 */
static uint32_t start_sector = INVALID_SECTOR_INDEX;

struct sector {
    uint8_t     data[BLOCK_LENGTH];
    uint32_t    index;
    uint16_t    last_usage; /**
                             * This variable is used to determine which entry
                             * is the oldest. A low value means the sector was
                             * used not long ago.
                             */
    uint8_t     flags;
};

static struct sector sectors[CACHE_ENTRY_COUNT];

/* Assume that each sector is 512 bytes long */
static inline uint32_t get_sector_from_address(uint32_t address)
{
    address >>= 9;
    return start_sector + address;
}

static inline uint32_t get_offset_from_address(uint32_t address)
{
    uint32_t mask = BLOCK_LENGTH - 1;

    return address & mask;
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
    for (i = 0; i < CACHE_ENTRY_COUNT; ++i)
        if (sectors[i].index == sector_index)
            return i;

    /* Try to find empty slot in cache */
    for (i = 0; i < CACHE_ENTRY_COUNT; ++i)
        if (sectors[i].index == INVALID_SECTOR_INDEX)
            break;

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

    /* If sector is dirty, let's write data to SD card */
    if (sectors[i].index != INVALID_SECTOR_INDEX
        && sectors[i].flags & DIRTY_FLAG)
        sd_write_block(sectors[i].data, sectors[i].index);

    /* Load sector from SD card */
    LOG_DBG("hal_sd: Loading sector 0x%08lX in entry %u", sector_index, i);
    if (sd_read_block(sectors[i].data, sector_index)) {
        sectors[i].index = INVALID_SECTOR_INDEX;
        return 1;
    }
    sectors[i].index = sector_index;
    sectors[i].last_usage = 0;
    sectors[i].flags = 0;

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

        sectors[index].flags |= DIRTY_FLAG;

        update_last_usage(index);
    }

    return 0;
}

void hal_flush(void)
{
    uint8_t i = 0;

    for (i = 0; i < CACHE_ENTRY_COUNT; ++i) {
        if (sectors[i].index == INVALID_SECTOR_INDEX)
            continue;

        if (sectors[i].flags & DIRTY_FLAG) {
            sd_write_block(sectors[i].data, sectors[i].index);
            sectors[i].flags &= ~DIRTY_FLAG;
        }
    }
}
