/**
 * SD card driver of the Boat Controller.
 *
 * This driver can only read and write entire sectors to an SD card connected
 * over SPI. In this case, SPI 1 module is used and configured as mode 0, 4MHz.
 *
 * The initialization phase takes about 50ms to complete and configures the
 * SD card to use SPI, its block length (512 bytes) and CRC is disabled.
 * Any call to read or write blocks are blocking. It takes several milliseconds
 * to read or write blocks from/to the SD card as this driver waits until the
 * SD card is ready.
 * However, this driver will not wait forever. While the driver is waiting for
 * some specific bytes, a counter gets incremented and if it equals the maximum
 * iteration allowed, the driver exits the loop and returns an error code.
 */



#include "mcc_generated_files/spi1.h"
#include "mcc_generated_files/pin_manager.h"
#include "sd.h"
#define    FCY    32000000UL
#include <libpic30.h>

#define SD_IO_OK                        (0)
#define SD_IO_ERROR                     (1)
#define DUMMY_CRC                       (0xFF)
#define CMD1_ATTEMPT_COUNT_MAX          (2000)
#define RESPONSE_ATTEMPT_COUNT_MAX      (128)
#define DATA_START_TOKEN                (0xFE)
#define CMD_TOKEN                       (0x40)
#define DATA_ACCEPTED                   (0x05)

static inline void flush_spi_buffer(void)
{
    SPI1_Exchange8bit(0xFF);
}

static uint8_t wait_for(uint8_t byte)
{
    uint8_t attempt_count = RESPONSE_ATTEMPT_COUNT_MAX;

    while (attempt_count && SPI1_Exchange8bit(0xFF) != byte)
        --attempt_count;

    if (attempt_count == 0)
        return SD_IO_ERROR;

    return SD_IO_OK;
}

enum {
    GO_IDLE_STATE = 0,
    SEND_OP_COND = 1,
    SET_BLOCKLEN = 16,
    READ_SINGLE_BLOCK = 17,
    WRITE_SINGLE_BLOCK = 24,
    CRC_ON_OFF = 59
};

/*
 * @return 0 if successful, 1 otherwise
 */
static uint8_t send_cmd(uint8_t index, uint32_t arg, uint8_t crc)
{
    uint8_t ret = SD_IO_OK;
    uint8_t cmd[] = {
        CMD_TOKEN | (index & 0x3F),
        arg >> 24,
        arg >> 16,
        arg >> 8,
        arg,
        crc
    };

    flush_spi_buffer();

    SPI_CS_SetLow();
    SPI1_Exchange8bitBuffer(cmd, sizeof(cmd), NULL);

    switch (index) {
    case GO_IDLE_STATE:
        ret = wait_for(1);

        break;

    case SEND_OP_COND:
    case SET_BLOCKLEN:
    case CRC_ON_OFF:
    {
        uint8_t response_attempt_count = 0;
        while ((ret = SPI1_Exchange8bit(0xFF)) == 0xFF) {
            ++response_attempt_count;
            if (response_attempt_count > RESPONSE_ATTEMPT_COUNT_MAX) {
                ret = SD_IO_ERROR;
                break;
            }
        }
    }
    break;

    default:
        ret = SD_IO_ERROR;
        break;
    }

    SPI_CS_SetHigh();

    return ret;
}

uint8_t sd_init(void)
{
    uint8_t i;
    uint16_t cmd1_attempt_count = 0;

    __delay_ms(10);

    /* Reduce SPI clock to ~400kHz */
    SPI1BRGL = 20;

    SPI_CS_SetHigh();

    for (i = 0; i < 20; ++i)
        SPI1_Exchange8bit(0xFF);

    if (send_cmd(GO_IDLE_STATE, 0, 0x95) != 0)
        return SD_IO_ERROR;

    while (send_cmd(SEND_OP_COND, 0, DUMMY_CRC) != 0) {
        cmd1_attempt_count++;
        if (cmd1_attempt_count > CMD1_ATTEMPT_COUNT_MAX)
            return SD_IO_ERROR;
    }

    if (send_cmd(CRC_ON_OFF, 0, DUMMY_CRC)
        || send_cmd(SET_BLOCKLEN, BLOCK_LENGTH, DUMMY_CRC))
        return SD_IO_ERROR;

    /* Restore SPI clock to ~4MHz */
    SPI1BRGL = 1;

    return SD_IO_OK;
}

uint8_t sd_read_block(uint8_t *buffer, uint32_t sector)
{
    return sd_read_subblock(buffer, sector, 0, BLOCK_LENGTH);
}

uint8_t sd_read_subblock(uint8_t *buffer, uint32_t sector, uint16_t offset, uint16_t length)
{
    uint8_t cmd[] = {
        CMD_TOKEN | READ_SINGLE_BLOCK,
        sector >> 15,
        sector >> 7,
        sector << 1,
        0x00,
        DUMMY_CRC
    };

    if (offset + length > BLOCK_LENGTH)
        return SD_IO_ERROR;

    flush_spi_buffer();

    SPI_CS_SetLow();

    SPI1_Exchange8bitBuffer(cmd, sizeof(cmd), NULL);

    if (wait_for(0) /* Wait for SD card to be ready */
        || wait_for(DATA_START_TOKEN)) {
        SPI_CS_SetHigh();
        return SD_IO_ERROR;
    }

    SPI1_Exchange8bitBuffer(NULL, offset, NULL);
    SPI1_Exchange8bitBuffer(NULL, length, buffer);
    SPI1_Exchange8bitBuffer(NULL, BLOCK_LENGTH - offset - length, NULL);

    /* Discard CRC */
    SPI1_Exchange8bit(0xFF);
    SPI1_Exchange8bit(0xFF);

    SPI_CS_SetHigh();

    return SD_IO_OK;
}

uint8_t sd_write_block(uint8_t *buffer, uint32_t sector)
{
    uint8_t data_response;
    uint8_t cmd[] = {
        CMD_TOKEN | WRITE_SINGLE_BLOCK,
        sector >> 15,
        sector >> 7,
        sector << 1,
        0x00,
        DUMMY_CRC
    };

    flush_spi_buffer();

    SPI_CS_SetLow();

    SPI1_Exchange8bitBuffer(cmd, sizeof(cmd), NULL);

    /* Wait for SD card to be ready */
    if (wait_for(0)) {
        SPI_CS_SetHigh();
        return SD_IO_ERROR;
    }

    SPI1_Exchange8bit(DATA_START_TOKEN);

    SPI1_Exchange8bitBuffer(buffer, BLOCK_LENGTH, NULL);

    /* Send dummy CRC */
    SPI1_Exchange8bit(DUMMY_CRC);
    SPI1_Exchange8bit(DUMMY_CRC);

    data_response = SPI1_Exchange8bit(0xFF);
    if ((data_response & 0x1F) != DATA_ACCEPTED) {
        SPI_CS_SetHigh();
        return SD_IO_ERROR;
    }

    /* Wait while the SD card is busy */
    while (SPI1_Exchange8bit(0xFF) == 0)
        ;

    SPI_CS_SetHigh();

    return SD_IO_OK;
}
