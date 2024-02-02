
#include "IMU_Read.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/i2c.h"

#define MPU6050_I2C_BASE    I2C0_BASE
#define MPU6050_I2C_SPEED   400000  // 400 kHz

void InitI2C(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

    I2CMasterInitExpClk(MPU6050_I2C_BASE, SysCtlClockGet(), true);
    I2CMasterEnable(MPU6050_I2C_BASE);
}

void MPU6050_Init(void)
{
    // Power Management Register (PWR_MGMT_1)
    MPU6050_WriteRegister(0x6B, 0x00);  // Clear sleep mode, CLKSEL = 0 (Internal oscillator)

    // Configuration Register (CONFIG)
    MPU6050_WriteRegister(0x1A, 0x03);  // DLPF_CFG = 3 (Low-pass filter bandwidth = 42 Hz)

    // Gyroscope Configuration Register (GYRO_CONFIG)
    MPU6050_SetGyroFullScaleRange(0x00);  // Full-scale range = +/- 250 degrees/s

    // Accelerometer Configuration Register (ACCEL_CONFIG)
    MPU6050_SetAccelFullScaleRange(0x00);  // Full-scale range = +/- 2g
}

void MPU6050_WriteRegister(uint8_t reg, uint8_t data)
{
    I2CMasterSlaveAddrSet(MPU6050_I2C_BASE, 0x69, false);  // 0x69 is the I2C address of MPU6050
    I2CMasterDataPut(MPU6050_I2C_BASE, reg);  // Set the register to write to
    I2CMasterControl(MPU6050_I2C_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while (I2CMasterBusy(MPU6050_I2C_BASE)) {}

    I2CMasterDataPut(MPU6050_I2C_BASE, data);
    I2CMasterControl(MPU6050_I2C_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while (I2CMasterBusy(MPU6050_I2C_BASE)) {}
}

void MPU6050_SetGyroFullScaleRange(uint8_t range)
{
    MPU6050_WriteRegister(0x1B, range << 3);  // GYRO_CONFIG register
}

void MPU6050_SetAccelFullScaleRange(uint8_t range)
{
    MPU6050_WriteRegister(0x1C, range << 3);  // ACCEL_CONFIG register
}

void MPU6050_ReadData(int16_t *accelX, int16_t *accelY, int16_t *accelZ, int16_t *gyroX, int16_t *gyroY, int16_t *gyroZ)
{
    uint8_t rawData[14];


    I2CMasterSlaveAddrSet(MPU6050_I2C_BASE, 0x69, false);  // 0x69 is the I2C address of MPU6050
    I2CMasterDataPut(MPU6050_I2C_BASE, 0x3B);  // Start reading from register 0x3B
    I2CMasterControl(MPU6050_I2C_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    while (I2CMasterBusy(MPU6050_I2C_BASE)) {}
    if (I2CMasterErr(MPU6050_I2C_BASE) != I2C_MASTER_ERR_NONE) {
        printf("Failed %s\n");
    }

    I2CMasterSlaveAddrSet(MPU6050_I2C_BASE, 0x69, true);
    I2CMasterControl(MPU6050_I2C_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    while (I2CMasterBusy(MPU6050_I2C_BASE)) {}
    rawData[0] = I2CMasterDataGet(MPU6050_I2C_BASE);
    if (I2CMasterErr(MPU6050_I2C_BASE) != I2C_MASTER_ERR_NONE) {
        printf("Failed %s\n");
    }
    int i;
    for (i = 1; i < 14; i++)
    {
        I2CMasterControl(MPU6050_I2C_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
        while (I2CMasterBusy(MPU6050_I2C_BASE)) {}
        rawData[i] = I2CMasterDataGet(MPU6050_I2C_BASE);
//        printf("Read register 0x3B: %d\n", rawData[i]);
    }
    I2CMasterControl(MPU6050_I2C_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    while (I2CMasterBusy(MPU6050_I2C_BASE)) {}

    *accelX = (rawData[0] << 8) | rawData[1];
    *accelY = (rawData[2] << 8) | rawData[3];
    *accelZ = (rawData[4] << 8) | rawData[5];
    *gyroX = (rawData[8] << 8) | rawData[9];
    *gyroY = (rawData[10] << 8) | rawData[11];
    *gyroZ = (rawData[12] << 8) | rawData[13];
}

