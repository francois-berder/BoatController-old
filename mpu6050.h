/* 
 * File:   accel.h
 * Author: francois
 *
 * Created on 12 juin 2016, 14:29
 */

#ifndef MPU6050_H
#define	MPU6050_H

#ifdef	__cplusplus
extern "C" {
#endif

void MPU6050_init(void);
uint16_t MPU6050_get_samples_cnt(void);
void MPU6050_read_fifo(int16_t *samples, uint16_t samples_cnt);
void MPU6050_clear_fifo(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MPU6050_H */

