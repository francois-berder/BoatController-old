/*
 * File:   accel.h
 * Author: francois
 *
 * Created on 12 juin 2016, 14:29
 */

#ifndef MPU6050_H
#define    MPU6050_H

#ifdef    __cplusplus
extern "C" {
#endif

/**
 * @brief Initialise the device.
 *
 *   - Both accelerometer and gyroscope are enabled
 *   - FIFO is enabled
 *
 * Notice that this function should take up to 300 ms to complete.
 */
void MPU6050_init(void);

/**
 * @brief Retrieves the number of samples available in the FIFO of the device.
 *
 * @return Number of samples in the FIFO.
 */
uint16_t MPU6050_get_samples_cnt(void);

/**
 * @brief Reads an array of samples from the
 *
 * @param[out] samples Array to store samples from device.
 * @param[in] sample_cnt Number of samples to read from the device.
 */
void MPU6050_read_fifo(int16_t *samples, uint16_t samples_cnt);

/**
 * @brief Clears the FIFO on the device.
 *
 * It has been found that reading the FIFO does not decrease the number of
 * samples until the FIFO gets full and discards new samples. Hence, it is
 * advised to periodically read every available samples from the device and
 * then clear the FIFO.
 */
void MPU6050_clear_fifo(void);

#ifdef    __cplusplus
}
#endif

#endif    /* MPU6050_H */

