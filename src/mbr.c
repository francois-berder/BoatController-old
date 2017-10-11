/**
 * Master Boot Record driver of the Boat Controller.
 *
 * Once the initialization of the SD card is finished, this module gets used
 * to read the first sector of the SD card and find a FAT16 partition. To
 * accomplish this task, it reads only the partition table of the MBR and
 * returns the start sector of the first FAT16 partition it found.
 *
 * Note that FAT16 LBA are not supported (default format when formatting SD card
 * on Windows 10).
 */

#include "log.h"
#include "mbr.h"
#include "sd.h"

#define BOOTSTRAP_CODE_SIZE     (446)
#define PARTITION_ENTRY_COUNT   (4)
#define PARTITION_ENTRY_SIZE    (16)
#define PARTITION_TABLE_SIZE    (PARTITION_ENTRY_COUNT * PARTITION_ENTRY_SIZE)
#define BOOTABLE_PARTITION      (0x80)
#define INACTIVE_PARTITION      (0x00)
#define FAT16_PARTITION         (0x06)

static uint32_t read_32bit_le(uint8_t *buffer)
{
    uint32_t res;

    res = buffer[3];
    res <<= 8;
    res |= buffer[2];
    res <<= 8;
    res |= buffer[1];
    res <<= 8;
    res |= buffer[0];
    return res;
}

uint32_t mbr_read(void)
{
    uint8_t i = 0;
    uint8_t partition_table[PARTITION_TABLE_SIZE];

    LOG_INFO("Reading MBR at sector 0...");

    if (sd_read_subblock(partition_table, 0, BOOTSTRAP_CODE_SIZE, PARTITION_TABLE_SIZE) != 0)
        return 0;

    /*
     * There are 4 entries in the partition table.
     * Iterate over each entry and return the start
     * sector of the first FAT16 partition.
     *
     * If no FAT16 partition is found, return 0.
     */
    for (i = 0; i < PARTITION_ENTRY_COUNT; ++i) {
        uint8_t *entry = &partition_table[i * PARTITION_ENTRY_SIZE];
        uint32_t start_sector, partition_size;

        /* Check status*/
        if (entry[0] != BOOTABLE_PARTITION && entry[0] != INACTIVE_PARTITION)
            continue;

        /* Check type */
        if (entry[4] != FAT16_PARTITION)
            continue;

        start_sector = read_32bit_le(&entry[8]);
        partition_size = read_32bit_le(&entry[12]);
        partition_size *= BLOCK_LENGTH;
        LOG_INFO("Found FAT16 partition in MBR (entry %u)", i);
        LOG_INFO("start (sector): %lu", start_sector);
        LOG_INFO("size (bytes): %lu", partition_size);

        return start_sector;
    }

    LOG_ERR("No FAT16 partition found in MBR\n");
    return 0;
}
