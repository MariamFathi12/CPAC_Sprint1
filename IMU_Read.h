/*
 * IMU_Read.h
 *
 *  Created on: Feb 2, 2024
 *      Author: PC J
 */

#ifndef IMU_READ_H_
#define IMU_READ_H_
#include <stdint.h>
void InitI2C(void);
void MPU6050_Init(void);
void MPU6050_WriteRegister(uint8_t reg, uint8_t data);
void MPU6050_SetGyroFullScaleRange(uint8_t range);
void MPU6050_SetAccelFullScaleRange(uint8_t range);
void MPU6050_ReadData(int16_t *accelX, int16_t *accelY, int16_t *accelZ, int16_t *gyroX, int16_t *gyroY, int16_t *gyroZ);


#endif /* IMU_READ_H_ */
