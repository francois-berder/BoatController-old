#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "fat16.h"
#include "hal_sd.h"
#include "log.h"

#define NDEBUG          (1)

#define INVALID_HANDLE  (255)
#define HANDLE_COUNT    (16)        /* Must not be greater than 254 */
#define READ_MODE       (true)
#define WRITE_MODE      (false)

#define FIRST_CLUSTER_INDEX_IN_FAT     (3)
#define MAX_BYTES_PER_CLUSTER           (((unsigned long)32) * (unsigned long)1024)
#define ROOT_DIR_VFAT_ENTRY             (0x0F)
#define ROOT_DIR_AVAILABLE_ENTRY        (0xE5)

/* cluster is a 16bit integer stored 26 bytes after the start of a
 * root directory entry.
 */
#define CLUSTER_OFFSET_ROOT_DIR_ENTRY   (26)

static struct fat16_bpb {
    char        oem_name[8];
    uint16_t    bytes_per_sector;
    uint8_t     sectors_per_cluster;
    uint16_t    reversed_sector_count;
    uint8_t     num_fats;
    uint16_t    root_entry_count;
    uint32_t    sector_count;
    uint16_t    fat_size; /* in sectors */
    uint32_t    volume_id;
    char        label[11];
    char        fs_type[8];
} bpb;

struct dir_entry {
    char        filename[11];
    uint8_t     attribute;
    uint8_t     reserved[10];
    uint8_t     time[2];
    uint8_t     date[2];
    uint16_t    starting_cluster;
    uint32_t    size;
};

enum FILE_ATTRIBUTE {
    READ_ONLY = 0x01,
    HIDDEN = 0x02,
    SYSTEM = 0x04,
    VOLUME = 0x08,
    SUBDIR = 0x10,
    ARCHIVE = 0x20
};

static struct {
    char        filename[11];       /* If handle is not used, filename[0] == 0 */
    bool        read_mode;          /* True if reading from file, false if writing to file */
    uint16_t    entry_index;        /* Index of the entry in the root directory */
    uint16_t    cluster;            /* Current cluster reading/writing */
    uint16_t    offset;             /* Offset in bytes in cluster */
    uint32_t    remaining_bytes;    /* Remaining bytes to be read in bytes in the file, only used in read mode */
} handles[HANDLE_COUNT];

static uint32_t start_fat_region = 0;               /* offset in bytes of first FAT */
static uint32_t start_root_directory_region = 0;    /* offset in bytes of root directory */
static uint32_t start_data_region = 0;              /* offset in bytes of data region */
static uint32_t data_cluster_count = 0;             /* Number of clusters in data region */

static int fat16_read_bpb(void)
{
    uint8_t data;

    memset(&bpb, 0, sizeof(struct fat16_bpb));

    /* Parse boot sector */
    LOG_DBG("#######   BPB   #######");
    /* jump instruction on 3 bytes.
     * Either: 0xEB,0x??, 0x90
     * or: 0xE9,0x??,0x??
     */
    hal_read_byte(&data);
    if (data == 0xEB) {
        hal_read_byte(&data);
        hal_read_byte(&data);
        if (data != 0x90)
            return -INVALID_JUMP_INSTRUCTION;
    } else if (data == 0xE9) {
        hal_read_byte(&data);
        hal_read_byte(&data);
    } else {
        return -INVALID_JUMP_INSTRUCTION;
    }

    hal_read((uint8_t *)&bpb.oem_name, 8);
    LOG_DBG("OEM NAME: %s", bpb.oem_name);
    hal_read((uint8_t *)&bpb.bytes_per_sector, 2);
    LOG_DBG("bytes per sector: %u", bpb.bytes_per_sector);
    if (bpb.bytes_per_sector != 512
        && bpb.bytes_per_sector != 1024
        && bpb.bytes_per_sector != 2048
        && bpb.bytes_per_sector != 4096)
        return -INVALID_BYTES_PER_SECTOR;

    hal_read(&bpb.sectors_per_cluster, 1);
    LOG_DBG("sectors per cluster: %u", bpb.sectors_per_cluster);
    if (bpb.sectors_per_cluster != 1
        && bpb.sectors_per_cluster != 2
        && bpb.sectors_per_cluster != 4
        && bpb.sectors_per_cluster != 8
        && bpb.sectors_per_cluster != 16
        && bpb.sectors_per_cluster != 32
        && bpb.sectors_per_cluster != 64
        && bpb.sectors_per_cluster != 128)
        return -INVALID_SECTOR_PER_CLUSTER;

    if (bpb.bytes_per_sector * bpb.sectors_per_cluster > MAX_BYTES_PER_CLUSTER)
        return -INVALID_BYTES_PER_CLUSTER;

    hal_read((uint8_t *)&bpb.reversed_sector_count, 2);
    LOG_DBG("reserved sector count: %u", bpb.reversed_sector_count);
    if (bpb.reversed_sector_count != 1)
        return -INVALID_RESERVED_SECTOR_COUNT;

    hal_read(&bpb.num_fats, 1);
    LOG_DBG("num fats: %u", bpb.num_fats);

    hal_read((uint8_t *)&bpb.root_entry_count, 2);
    LOG_DBG("root entry count: %u", bpb.root_entry_count);
    if ((((32 * bpb.root_entry_count) / bpb.bytes_per_sector) & 0x1) != 0)
        return -INVALID_ROOT_ENTRY_COUNT;

    hal_read((uint8_t *)&bpb.sector_count, 2);


    /* Skip media */
    hal_read_byte(&data);

    hal_read((uint8_t *)&bpb.fat_size, 2);
    LOG_DBG("fat size: %u", bpb.fat_size);

    /* Skip sector per track for int 0x13 */
    hal_read_byte(&data);
    hal_read_byte(&data);

    /* Skip number of heads for int 0x13 */
    hal_read_byte(&data);
    hal_read_byte(&data);

    /* Skip hidden sectors */
    hal_read_byte(&data);
    hal_read_byte(&data);
    hal_read_byte(&data);
    hal_read_byte(&data);

    uint32_t sector_count_32b;
    hal_read((uint8_t *)&sector_count_32b, 4);
    if ((bpb.sector_count != 0 && sector_count_32b != 0)
        || (bpb.sector_count == 0 && sector_count_32b == 0))
        return -INVALID_SECTOR_COUNT;

    if (bpb.sector_count == 0)
        bpb.sector_count = sector_count_32b;
    LOG_DBG("sector count: %lu", (unsigned long)bpb.sector_count);

    /* Skip drive number */
    hal_read_byte(&data);

    /* Skip reserved byte */
    hal_read_byte(&data);

    hal_read_byte(&data);
    if (data == 0x29) {
        hal_read((uint8_t *)&bpb.volume_id, 4);
        LOG_DBG("volume ID: %lu", bpb.volume_id);

        hal_read((uint8_t *)&bpb.label, 11);
        LOG_DBG("label: %s", bpb.label);

        hal_read((uint8_t *)bpb.fs_type, 8);
        LOG_DBG("fs type: %s", bpb.fs_type);
    }

    return 0;
}

static bool is_character_valid(char c)
{
    return ('a' <= c && c <= 'z')
           || ('A' <= c && c <= 'Z')
           || ('0' <= c && c <= '9')
           || c == '#'
           || c == '$'
           || c == '%'
           || c == '&'
           || c == '\''
           || c == '('
           || c == ')'
           || c == '-'
           || c == '@';
}

/**
 * @brief Convert filename to 8.3 short FAT name.
 *
 * Example "hello.txt" to "HELLO   TXT"
 *
 * @param[out] fat_filename 11 long char array
 * @param[in] filename arbitrary long string
 * @return 0 if successful, -1 otherwise
 */
static int make_fat_filename(char *fat_filename, char *filename)
{
    uint8_t i = 0;
    uint8_t sep;

    /* Find position of . (marker between name and extension) */
    for (i = 0; i < 9; ++i) {
        if (filename[i] == '\0')
            return -1;
        if (filename[i] == '.') {
            sep = i;
            break;
        }

        if (!is_character_valid(filename[i])) {
            LOG_ERR("Invalid character in filename: %s", filename);
            return -1;
        }
    }

    /* If it cannot find . in the first 9 characters then the name is more
     * than 8 characters long which is forbidden. */
    if (i == 9)
        return -1;

    /* Copy name */
    for (i = 0; i < sep; ++i)
        fat_filename[i] = toupper(filename[i]);

    memset(&fat_filename[sep], ' ', 8 - sep);

    /* Copy extension */
    for (i = 0; i < 3; ++i) {
        if (filename[i] == '\0')
            return -1;

        if (!is_character_valid(filename[sep + 1 + i])) {
            LOG_ERR("Invalid character in filename: %s\n", filename);
            return -1;
        }

        fat_filename[8 + i] = toupper(filename[sep + 1 + i]);
    }

#ifndef NDEBUG
    for (i = 0; i < 11; ++i)
        printf("%c", fat_filename[i]);
    printf("\n");
#endif
    return 0;
}

#ifndef NDEBUG
static void dump_root_entry(struct dir_entry e)
{
    LOG_INFO("filename: %s", e.filename);
    LOG_INFO("attribute: ");
    if (e.attribute & READ_ONLY)
        LOG_INFO("read-only ");
    if (e.attribute & HIDDEN)
        LOG_INFO("hidden ");
    if (e.attribute & SYSTEM)
        LOG_INFO("system ");
    if (e.attribute & VOLUME)
        LOG_INFO("volume ");
    if (e.attribute & SUBDIR)
        LOG_INFO("subdir ");
    if (e.attribute & ARCHIVE)
        LOG_INFO("archive ");
    LOG_INFO("starting cluster: %u", e.starting_cluster);
    LOG_INFO("size: %lu", (unsigned long)e.size);
}
#endif

static bool is_file_opened(char *filename, bool mode)
{
    uint8_t i = 0;

    for (; i < HANDLE_COUNT; ++i) {
        if (handles[i].filename[0] == 0)
            continue;

        if (memcmp(filename, handles[i].filename, sizeof(handles[i].filename)) == 0)
            if (handles[i].read_mode == mode)
                return true;
    }

    return false;
}

static uint8_t find_available_handle(void)
{
    uint8_t i = 0;

    for (; i < HANDLE_COUNT; ++i)
        if (handles[i].filename[0] == 0)
            return i;

    return INVALID_HANDLE;
}

/**
 * @brief Move cursor to a specific byte in data region.
 *
 * @param[in] cluster Index of the cluster
 * @param[in] offset Offset in bytes from the start of the cluster.
 */
static void move_to_data_region(uint16_t cluster, uint16_t offset)
{
    uint32_t tmp = cluster - 2;

    tmp *= bpb.sectors_per_cluster;
    tmp *= bpb.bytes_per_sector;
    uint32_t pos = start_data_region;
    pos += tmp;
    pos += offset;
    LOG_DBG("Moving to %08lX", pos);
    hal_seek(pos);
}

/**
 * @brief Move cursor to an entry in the root directory.
 *
 * @param[in] entry_index Index of the entry, must not be greater than bpb.root_entry_count
 */
static void move_to_root_directory_region(uint16_t entry_index)
{
    uint32_t pos = entry_index * 32;

    pos += start_root_directory_region;
    LOG_DBG("Moving to %08lX\n", pos);
    hal_seek(pos);
}

/**
 * @brief Move cursor to a location in the first FAT.
 *
 * @param[in] cluster Index of the cluster.
 */
static void move_to_fat_region(uint16_t cluster)
{
    uint32_t pos = start_fat_region;

    pos += cluster * 2;
    LOG_DBG("Moving to %08lX\n", pos);
    hal_seek(pos);
}

/**
 * @brief Find the index of an entry based on its name.
 *
 * @param[in] filename name of the file in 8.3 format
 * @return -1 if it cannot find the entry, otherwise give the index (positive number)
 */
static long find_root_directory_entry(char *filename)
{
    uint16_t i = 0;
    long ret = -1;

    move_to_root_directory_region(0);
    for (i = 0; i < bpb.root_entry_count; ++i) {
        struct dir_entry e;
        hal_read((uint8_t *)&e, sizeof(struct dir_entry));
#ifndef NDEBUG
        dump_root_entry(e);
#endif

        /* Skip available entry */
        if ((uint8_t)(e.filename[0]) == ROOT_DIR_AVAILABLE_ENTRY)
            continue;

        /* Check if we reach end of list of root directory entries */
        if (e.filename[0] == 0)
            break;

        /* Ignore any VFAT entry */
        if ((e.attribute & ROOT_DIR_VFAT_ENTRY) == ROOT_DIR_VFAT_ENTRY)
            continue;

        if (memcmp(filename, e.filename, sizeof(e.filename)) == 0) {
            ret = i;
            break;
        }
    }

    return ret;
}

/**
 * @brief Create a handle for reading a file.
 *
 * @param[in] handle Index to an available handle
 * @param[in] filename Name of the file in 8.3 format
 * @return handle if successful, -1 otherwise
 */
static int fat16_open_read(uint8_t handle, char *filename)
{
    long entry_index = 0;
    struct dir_entry entry;

    /* Check that it is not opened for writing operations. */
    if (is_file_opened(filename, WRITE_MODE)) {
        LOG_ERR("Cannot read from file while writing to it.");
        return -1;
    }

    /* Find the file in the root directory */
    if ((entry_index = find_root_directory_entry(filename)) < 0) {
        LOG_ERR("File %s not found.", filename);
        return -1;
    }

    move_to_root_directory_region(entry_index);
    hal_read((uint8_t *)&entry, sizeof(struct dir_entry));

    /* Create handle */
    memcpy(handles[handle].filename, filename, sizeof(handles[handle].filename));
    handles[handle].read_mode = READ_MODE;
    handles[handle].entry_index = entry_index;
    handles[handle].cluster = entry.starting_cluster;
    handles[handle].offset = 0;
    handles[handle].remaining_bytes = entry.size;

    return handle;
}

/**
 * @brief Find an unused entry in the root directory.
 *
 * @return -1 if there is no available entry in the root directory, a positive number otherwise.
 */
static long find_available_entry_in_root_directory(void)
{
    uint16_t i = 0;

    do {
        uint8_t tmp;
        move_to_root_directory_region(i);
        hal_read(&tmp, sizeof(tmp));

        if (tmp == 0 || tmp == ROOT_DIR_AVAILABLE_ENTRY)
            return i;

        ++i;
    } while (i < bpb.root_entry_count);

    return -1;
}

/**
 * @brief Check if the entry is the last entry in the root directory.
 *
 * @param[in] entry_index
 * @return True if the entry is the last one.
 */
static bool last_entry_in_root_directory(uint16_t entry_index)
{
    uint8_t tmp = 0;

    if (entry_index == (bpb.root_entry_count - 1))
        return true;

    /* Check if the next entry is marked as being the end of the
     * root directory list.
     */
    move_to_root_directory_region(entry_index + 1);
    hal_read((uint8_t *)&tmp, sizeof(tmp));
    return tmp == 0;
}

/* @brief Indicate that a root entry is now available.
 *
 * The first byte of the entry must be 0xE5 if it is not the last
 * entry in the root directory. Otherwise, a value of 0 must be
 * written.
 *
 * @param[in] entry_index
 */
static void mark_root_entry_as_available(uint16_t entry_index)
{
    uint8_t entry_marker = 0;

    if (!last_entry_in_root_directory(entry_index))
        entry_marker = ROOT_DIR_AVAILABLE_ENTRY;
    move_to_root_directory_region(entry_index);
    hal_write(&entry_marker, sizeof(entry_marker));
}

static void free_cluster_chain(uint16_t cluster)
{
    /* If the file is empty, the starting cluster variable is equal to 0.
     * No need to iterate through the FAT. */
    if (cluster == 0)
        return;

    /* Mark all clusters in the FAT as available */
    do {
        uint16_t free_cluster = 0;
        uint16_t next_cluster;
        move_to_fat_region(cluster);
        hal_read((uint8_t *)&next_cluster, sizeof(next_cluster));

        move_to_fat_region(cluster);

        hal_write((uint8_t *)&free_cluster, sizeof(free_cluster));

        if (next_cluster >= 0xFFF8)
            break;
        cluster = next_cluster;
    } while (1);
}

/**
 * @brief Delete a file.
 *
 * Remove the entry from the and mark all clusters used by this file as
 * available. It does not clear the data region.
 *
 * @param[in] fat_filename Name of the filename in 8.3 format
 * @return 0 if successful, -1 otherwise
 */
static int delete_file(char *fat_filename)
{
    long entry_index = 0;
    uint32_t pos = 0;
    uint16_t starting_cluster = 0;

    /* Find the file in the root directory */
    if ((entry_index = find_root_directory_entry(fat_filename)) < 0)
        return -1;

    mark_root_entry_as_available(entry_index);

    /* Find the first cluster used by the file */
    pos = start_root_directory_region;
    pos += entry_index * 32;
    pos += CLUSTER_OFFSET_ROOT_DIR_ENTRY;
    LOG_INFO("Moving to %08lX\n", pos);
    hal_seek(pos);
    hal_read((uint8_t *)&starting_cluster, sizeof(starting_cluster));
    free_cluster_chain(starting_cluster);

    return 0;
}

/**
 * @brief Create a handle and an entry in the FAT.
 *
 * @param[in] handle Index to an available handle
 * @param[in] filename Name of the file in 8.3 format
 * @return handle if successful, -1 otherwise
 */
static int fat16_open_write(uint8_t handle, char *filename)
{
    struct dir_entry entry;
    long entry_index = 0;

    /* Check that it is not opened for reading operations. */
    if (is_file_opened(filename, READ_MODE)) {
        LOG_INFO("Cannot write to file while reading from it.");
        return -1;
    }

    /* Check that it is not opened for writing operations.
     * For simplicity, a file can be written by only one
     * handle.
     */
    if (is_file_opened(filename, WRITE_MODE)) {
        LOG_INFO("Cannot write to file already opened in write mode.");
        return -1;
    }

    /* Discard any previous content.
     * Do not check return value because the file may not exist.
     */
    delete_file(filename);

    /* Find a location in the root directory region */
    if ((entry_index = find_available_entry_in_root_directory()) < 0)
        return -1;

    /* Create an entry in the root directory */
    memcpy(entry.filename, filename, sizeof(entry.filename));
    entry.attribute = 0;
    memset(entry.reserved, 0, sizeof(entry.reserved));
    memset(entry.time, 0, sizeof(entry.time));
    memset(entry.date, 0, sizeof(entry.date));
    entry.starting_cluster = 0;
    entry.size = 0;

    move_to_root_directory_region(entry_index);
    hal_write((uint8_t *)&entry, sizeof(struct dir_entry));

    /* Create a handle */
    memcpy(handles[handle].filename, filename, sizeof(handles[handle].filename));
    handles[handle].read_mode = WRITE_MODE;
    handles[handle].entry_index = entry_index;
    handles[handle].cluster = 0;
    handles[handle].offset = 0;
    handles[handle].remaining_bytes = 0; /* Not used in write mode */

    return handle;
}

/* Return true if handle is valid, false otherwise */
static bool check_handle(uint8_t handle)
{
    if (handle >= HANDLE_COUNT)
        return false;

    if (handles[handle].filename[0] == 0)
        return false;

    return true;
}

/**
 * @brief Read the cluster status in the FAT.
 */
static uint16_t read_fat_entry(uint16_t cluster)
{
    uint16_t fat_entry = 0;

    move_to_fat_region(cluster);
    hal_read((uint8_t *)&fat_entry, sizeof(fat_entry));

    return fat_entry;
}

static long allocate_cluster(uint16_t cluster)
{
    uint16_t next_cluster = FIRST_CLUSTER_INDEX_IN_FAT;

    /* Find an empty location in the FAT, skip first 3 entries in the FAT,
     * because they are reserved.
     */
    move_to_fat_region(next_cluster);
    for (; next_cluster < data_cluster_count - FIRST_CLUSTER_INDEX_IN_FAT; ++next_cluster) {
        uint16_t fat_entry;
        hal_read((uint8_t *)&fat_entry, sizeof(fat_entry));

        /* Mark it as end of file */
        if (fat_entry == 0) {
            fat_entry = 0xFFFF;
            move_to_fat_region(next_cluster);
            hal_write((uint8_t *)&fat_entry, sizeof(fat_entry));
            break;
        }
    }

    if (next_cluster == data_cluster_count) {
        LOG_INFO("Could not find an available cluster.");
        return -1;
    }

    /* Update current cluster to point to next one */
    if (cluster != 0) {
        move_to_fat_region(cluster);
        hal_write((uint8_t *)&next_cluster, sizeof(next_cluster));
    }

    return next_cluster;
}

/**
 * @brief Update the size of file in the root entry.
 *
 * @param[in] entry_index Index of the entry in the root directory
 * @param[in] bytes_written_count Number of bytes appended to the file. Hence
 * the new size of the file is bytes_written_count + old_size
 */
static void update_size_file(uint16_t entry_index, uint32_t bytes_written_count)
{
    uint32_t file_size = 0;
    uint32_t pos = start_root_directory_region;

    pos += entry_index * 32;
    pos += 28; /* Offset in bytes of the file size in the entry */

    hal_seek(pos);
    hal_read((uint8_t *)&file_size, sizeof(file_size));

    file_size += bytes_written_count;

    hal_seek(pos);
    hal_write((uint8_t *)&file_size, sizeof(file_size));
}

int fat16_init(void)
{
    uint32_t data_sector_count, root_directory_sector_count;
    int ret = fat16_read_bpb();

    if (ret < 0)
        return ret;

    root_directory_sector_count = (bpb.root_entry_count * 32) / bpb.bytes_per_sector;
    LOG_DBG("root directory sector count: %lu", root_directory_sector_count);

    /* Find number of sectors in data region */
    data_sector_count = bpb.sector_count - (bpb.reversed_sector_count + (bpb.num_fats * bpb.fat_size) + root_directory_sector_count);
    data_cluster_count = data_sector_count / bpb.sectors_per_cluster;
    LOG_DBG("data cluster count: %lu", data_cluster_count);

    if (data_cluster_count < 4085
        || data_cluster_count >= 65525)
        return -INVALID_FAT_TYPE;

    start_fat_region = bpb.reversed_sector_count * bpb.bytes_per_sector;
    start_root_directory_region = bpb.num_fats * bpb.fat_size;
    start_root_directory_region *= bpb.bytes_per_sector;
    start_root_directory_region += start_fat_region;
    start_data_region = start_root_directory_region + (root_directory_sector_count * bpb.bytes_per_sector);

    LOG_DBG("start_fat_region=%08lX", start_fat_region);
    LOG_DBG("start_root_directory_region=%08lX", start_root_directory_region);
    LOG_DBG("start_data_region=%08lX", start_data_region);

    /* Make sure that all handles are available */
    memset(handles, 0, sizeof(handles));

    return 0;
}

int fat16_open(char *filename, char mode)
{
    char fat_filename[11];
    uint8_t handle = INVALID_HANDLE;

    if (mode != 'r' && mode != 'w') {
        LOG_ERR("Invalid mode.");
        return -1;
    }

    if (filename == NULL) {
        LOG_ERR("Cannot open a file with a null path string.");
        return -1;
    }

    handle = find_available_handle();
    if (handle == INVALID_HANDLE) {
        LOG_ERR("No available handle found.");
        return -1;
    }

    if (make_fat_filename(fat_filename, filename) < 0)
        return -1;

    if (mode == 'r')
        return fat16_open_read(handle, fat_filename);
    else
        return fat16_open_write(handle, fat_filename);
}

int fat16_read(uint8_t handle, char *buffer, uint32_t count)
{
    uint32_t bytes_read_count = 0;

    if (check_handle(handle) == false) {
        LOG_ERR("fat16_read: Invalid handle.");
        return -1;
    }

    if (handles[handle].read_mode != READ_MODE) {
        LOG_ERR("fat16_read: Cannot read with handle in write mode.");
        return -1;
    }

    /* Check if we reach end of file */
    if (handles[handle].remaining_bytes == 0)
        return -END_OF_FILE_REACHED;

    move_to_data_region(handles[handle].cluster, handles[handle].offset);

    /* Read in chunk until count is 0 or end of file is reached */
    while (count > 0) {
        uint32_t chunk_length = count, bytes_remaining_in_cluster = 0;

        /* Check if we reach end of file */
        if (handles[handle].remaining_bytes == 0)
            return bytes_read_count;

        /* Check that we read within the boundary of the current cluster */
        bytes_remaining_in_cluster = bpb.sectors_per_cluster * bpb.bytes_per_sector - handles[handle].offset;
        if (chunk_length > bytes_remaining_in_cluster)
            chunk_length = bytes_remaining_in_cluster;

        /* Check that we do not read past the end of file */
        if (chunk_length > handles[handle].remaining_bytes)
            chunk_length = handles[handle].remaining_bytes;

        hal_read((uint8_t *)buffer, chunk_length);

        handles[handle].remaining_bytes -= chunk_length;
        handles[handle].offset += chunk_length;
        if (handles[handle].offset == bpb.sectors_per_cluster * bpb.bytes_per_sector) {
            handles[handle].offset = 0;

            /* Look for the next cluster in the FAT, unless we are already reading the last one */
            if (handles[handle].remaining_bytes != 0) {
                uint16_t fat_entry = read_fat_entry(handles[handle].cluster);
                /* TODO: check fat entry */

                handles[handle].cluster = fat_entry;

                move_to_data_region(handles[handle].cluster, handles[handle].offset);
            }
        }
        count -= chunk_length;
        buffer += chunk_length;
        bytes_read_count += chunk_length;
    }

    return bytes_read_count;
}

int fat16_write(uint8_t handle, char *buffer, uint32_t count)
{
    uint32_t bytes_written_count = 0;

    if (check_handle(handle) == false) {
        LOG_ERR("fat16_write: Invalid handle.");
        return -1;
    }

    if (handles[handle].read_mode != WRITE_MODE) {
        LOG_ERR("fat16_write: Cannot write with handle in read mode.");
        return -1;
    }

    if (buffer == NULL) {
        LOG_ERR("fat16_write: Cannot write using null buffer.");
        return -1;
    }

    if (count == 0)
        return 0;

    /* If the file is not empty, move position to the end of file */
    if (handles[handle].cluster != 0)
        move_to_data_region(handles[handle].cluster, handles[handle].offset);

    /* Write in chunk until count is 0 or no clusters can be allocated */
    while (count > 0) {
        uint32_t chunk_length = count;
        uint32_t bytes_remaining_in_cluster = bpb.sectors_per_cluster * bpb.bytes_per_sector - handles[handle].offset;

        /* Check if we need to allocate a new cluster */
        if (handles[handle].cluster == 0
            || bytes_remaining_in_cluster == 0) {
            long new_cluster = allocate_cluster(handles[handle].cluster);
            if (new_cluster < 0)
                return -1;

            /* If the file was empty, update cluster in root directory entry */
            if (handles[handle].cluster == 0) {
                uint32_t pos = start_root_directory_region;
                pos += handles[handle].entry_index * 32;
                pos += CLUSTER_OFFSET_ROOT_DIR_ENTRY;
                hal_seek(pos);
                hal_write((uint8_t *)&new_cluster, sizeof(new_cluster));
            }

            handles[handle].cluster = new_cluster;
            handles[handle].offset = 0;

            move_to_data_region(new_cluster, 0);
            bytes_remaining_in_cluster = bpb.sectors_per_cluster * bpb.bytes_per_sector;
        }

        /* Check that we write within the boundary of the current cluster */
        if (chunk_length > bytes_remaining_in_cluster)
            chunk_length = bytes_remaining_in_cluster;

        hal_write((uint8_t *)buffer, chunk_length);

        count -= chunk_length;
        buffer += chunk_length;
        bytes_written_count += chunk_length;
        handles[handle].offset += chunk_length;
    }

    /* Update size of file in root directory entry */
    update_size_file(handles[handle].entry_index, bytes_written_count);

    return bytes_written_count;
}

int fat16_close(uint8_t handle)
{
    if (check_handle(handle) == false) {
        LOG_ERR("fat16_write: Invalid handle.");
        return -1;
    }
    handles[handle].filename[0] = 0;
    return 0;
}

int fat16_delete(char *filename)
{
    char fat_filename[11];

    if (filename == NULL) {
        LOG_ERR("Cannot open a file with a null path string.");
        return -1;
    }

    if (make_fat_filename(fat_filename, filename) < 0)
        return -1;

    if (is_file_opened(fat_filename, READ_MODE)
        || is_file_opened(fat_filename, WRITE_MODE)) {
        LOG_ERR("Cannot delete a file currently opened.");
        return -1;
    }

    return delete_file(fat_filename);
}

long fat16_ls(long index, char *filename)
{
    uint8_t name_length = 0, ext_length = 0;
    char fat_filename[11];

    if (index < 0 || index >= bpb.root_entry_count)
        return -1;

    if (filename == NULL)
        return -1;

    fat_filename[0] = 0;
    while (fat_filename[0] == 0
           || ((uint8_t)fat_filename[0]) == ROOT_DIR_AVAILABLE_ENTRY) {
        uint8_t attribute = 0;

        move_to_root_directory_region(index);
        hal_read((uint8_t *)fat_filename, 11);
        ++index;

        /* If this condition is true, the end of the root directory is reached.
         * and there are no more files to be found.
         */
        if (index == bpb.root_entry_count)
            return -2;

        /* Also reading attribute to skip any vfat entry. */
        hal_read(&attribute, 1);
        if ((attribute & ROOT_DIR_VFAT_ENTRY) == ROOT_DIR_VFAT_ENTRY) {
            fat_filename[0] = 0; /* Make sure that the condition of the loop
                                  * remains true.
                                  */
            continue;
        }
    }

    /* Reformat filename:
     *   - Trim name
     *   - Add '.' to separate name and extension
     *   - Trim extension
     *   - Add null terminated
     */
    for (name_length = 0; name_length < 8; ++name_length) {
        char c = fat_filename[name_length];
        if (c == ' ')
            break;

        filename[name_length] = c;
    }

    filename[name_length] = '.';

    for (ext_length = 0; ext_length < 3; ++ext_length) {
        char c = fat_filename[8 + ext_length];
        if (c == ' ')
            break;
        filename[name_length + 1 + ext_length] = c;
    }

    filename[name_length + 1 + ext_length] = '\0';

    return index;
}

void fat16_flush(void)
{
    hal_flush();
}
