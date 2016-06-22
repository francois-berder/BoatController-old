#define    FCY    32000000UL
#include <libpic30.h>
#include <xc.h>
#include "mcc_generated_files/i2c1.h"
#include "mpu6050.h"
#include "log.h"

#define MPU6050_ADDRESS     (0x68)

/* Register address*/
#define WHO_AM_I            (0x75)
#define CONFIG              (0x1A)
#define ACCEL_CONFIG        (0x1C)
#define FIFO_CONFIG         (0x23)
#define USER_CTRL           (0x6A)
#define PWR_MGMT_1          (0x6B)
#define FIFO_COUNT_HIGH     (0x72)
#define FIFO_COUNT_LOW      (0x73)
#define FIFO_DATA           (0x74)
#define ACCEL_X_HIGH        (0x3B)
#define ACCEL_X_LOW         (0x3C)

/* Register values*/
#define MPU6050_DEVICE_ID   (0x68)
#define RESET               (0x80)
#define SLEEP               (0x40)
#define ACCEL_RANGE_2G      (0x00)
#define ACCEL_RANGE_4G      (0x08)
#define ACCEL_RANGE_8G      (0x10)
#define ACCEL_RANGE_16G     (0x18)
#define ACCEL_FIFO_EN       (0x08)
#define FIFO_EN             (0x40)
#define FIFO_RESET          (0x04)

static uint8_t read_8bit_reg(uint8_t reg_address)
{
    I2C1_MESSAGE_STATUS status;
    uint8_t value;
    I2C1_MasterWrite(&reg_address, 1, MPU6050_ADDRESS, &status);
    while(status == I2C1_MESSAGE_PENDING);
    
    I2C1_MasterRead(&value, 1, MPU6050_ADDRESS, &status);
    while(status == I2C1_MESSAGE_PENDING);
    
    return value;
}

static void write_8bit_reg(uint8_t reg_address, uint8_t value)
{
    I2C1_MESSAGE_STATUS status;
    uint8_t buffer[2] = { reg_address, value };
    I2C1_MasterWrite(buffer, sizeof(buffer), MPU6050_ADDRESS, &status);
    while(status == I2C1_MESSAGE_PENDING);
}

void MPU6050_init(void)
{
    uint8_t device_ID;
    __delay_ms(100);
    
    device_ID = read_8bit_reg(WHO_AM_I);
    if (device_ID != MPU6050_DEVICE_ID)
        LOG_ERR("Error invalid device ID of MPU6050.");

    /* Reset device - clears most registers to 0 */
    write_8bit_reg(PWR_MGMT_1, RESET);
    __delay_ms(50);

    /* Wake up from sleep*/
    write_8bit_reg(PWR_MGMT_1, 0);
    __delay_ms(10);

    write_8bit_reg(CONFIG, 0);
    
    /* Configure FIFO */
    write_8bit_reg(USER_CTRL, FIFO_RESET);
    __delay_ms(10);
    write_8bit_reg(USER_CTRL, FIFO_EN);
    write_8bit_reg(FIFO_CONFIG, ACCEL_FIFO_EN);
    
    /* Configure accelerometer*/
    write_8bit_reg(ACCEL_CONFIG, ACCEL_RANGE_4G);
}

uint16_t MPU6050_get_samples_cnt(void)
{
    uint16_t cnt = 0;
    I2C1_MESSAGE_STATUS status;
    uint8_t buffer[2];

    buffer[0] = FIFO_COUNT_HIGH;
    I2C1_MasterWrite(buffer, 1, MPU6050_ADDRESS, &status);
    while(status == I2C1_MESSAGE_PENDING);
    
    I2C1_MasterRead(buffer, sizeof(buffer), MPU6050_ADDRESS, &status);
    while(status == I2C1_MESSAGE_PENDING);

    cnt = buffer[0];
    cnt <<= 8;
    cnt |= buffer[1];
    
    return cnt;
}

void MPU6050_read_fifo(int16_t *samples, uint16_t samples_cnt)
{
    I2C1_MESSAGE_STATUS status;
    uint16_t i;
    
    for (i = 0; i < samples_cnt; ++i) {
        uint8_t buffer[6];
        uint8_t reg = FIFO_DATA;
        I2C1_MasterWrite(&reg, 1, MPU6050_ADDRESS, &status);
        while(status == I2C1_MESSAGE_PENDING);

        I2C1_MasterRead(buffer, sizeof(buffer), MPU6050_ADDRESS, &status);
        while(status == I2C1_MESSAGE_PENDING);

        samples[i*3] = buffer[0];
        samples[i*3] <<= 8;
        samples[i*3] |= buffer[1];

        samples[i*3+1] = buffer[2];
        samples[i*3+1] <<= 8;
        samples[i*3+1] |= buffer[3];

        samples[i*3+2] = buffer[4];
        samples[i*3+2] <<= 8;
        samples[i*3+2] |= buffer[5];    
    }
}

void MPU6050_clear_fifo(void)
{
    write_8bit_reg(USER_CTRL, FIFO_RESET);
    write_8bit_reg(USER_CTRL, FIFO_EN);
}