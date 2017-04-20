#include "mcc_generated_files/spi1.h"
#include "mcc_generated_files/pin_manager.h"
#include "sd.h"
#define    FCY    32000000UL
#include <libpic30.h>

#define DUMMY_CRC                       (0xFF)
#define CMD1_ATTEMPT_COUNT_MAX          (2000)
#define RESPONSE_ATTEMPT_COUNT_MAX      (16)

static inline void flush_spi_buffer(void)
{
    SPI1_Exchange8bit(0xFF);
}

enum {
    GO_IDLE_STATE = 0,
    SEND_OP_COND = 1,
    SET_BLOCKLEN = 16,
    READ_SINGLE_BLOCK = 17,
    CRC_ON_OFF = 59
};

/*
 * @return 0 if successful, 1 otherwise
 */
static uint8_t send_cmd(uint8_t index, uint32_t arg, uint8_t crc)
{
    uint8_t ret = 0, i = 0;
    uint8_t response_attempt_count = 0;

    flush_spi_buffer();

    SPI_CS_SetLow();

    SPI1_Exchange8bit(0x40 | (index & 0x3F));
    for (i = 0; i < 4; ++i)
        SPI1_Exchange8bit(arg >> ((3 - i) * 8));
    SPI1_Exchange8bit(crc);

    switch (index) {
    case GO_IDLE_STATE:
        while (SPI1_Exchange8bit(0xFF) != 1) {
            ++response_attempt_count;
            if (response_attempt_count > RESPONSE_ATTEMPT_COUNT_MAX) {
                ret = 1;
                break;
            }
        }
        break;

    case SEND_OP_COND:
    case SET_BLOCKLEN:
    case CRC_ON_OFF:
        while ((ret = SPI1_Exchange8bit(0xFF)) == 0xFF) {
            ++response_attempt_count;
            if (response_attempt_count > RESPONSE_ATTEMPT_COUNT_MAX) {
                ret = 1;
                break;
            }
        }
        break;

    default:
        ret = 1;
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
        return 1;

    while (send_cmd(SEND_OP_COND, 0, DUMMY_CRC) != 0) {
        cmd1_attempt_count++;
        if (cmd1_attempt_count > CMD1_ATTEMPT_COUNT_MAX)
            return 1;
    }

    if (send_cmd(CRC_ON_OFF, 0, DUMMY_CRC)
        || send_cmd(SET_BLOCKLEN, BLOCK_LENGTH, DUMMY_CRC))
        return 1;

    /* Restore SPI clock to ~4MHz */
    SPI1BRGL = 1;

    return 0;
}

uint8_t sd_read_block(uint8_t *buffer, uint32_t sector)
{
    uint16_t i;
    flush_spi_buffer();

    SPI_CS_SetLow();

    SPI1_Exchange8bit(0x40 | READ_SINGLE_BLOCK);
    SPI1_Exchange8bit(sector >> 15);
    SPI1_Exchange8bit(sector >> 7);
    SPI1_Exchange8bit(sector << 1);
    SPI1_Exchange8bit(0x00);
    SPI1_Exchange8bit(DUMMY_CRC);

    /* Wait for SD card to be ready */
    while (SPI1_Exchange8bit(0xFF) != 0)
        ;

    /* Wait for data start token */
    while (SPI1_Exchange8bit(0xFF) != 0xFE)
        ;

    for (i = 0; i < BLOCK_LENGTH; ++i)
        buffer[i] = SPI1_Exchange8bit(0xFF);

    /* Discard CRC */
    SPI1_Exchange8bit(0xFF);
    SPI1_Exchange8bit(0xFF);

    SPI_CS_SetHigh();

    return 0;
}
