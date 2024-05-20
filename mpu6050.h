/*
 * mpu6050.h
 *
 *  Created on: Apr 28, 2024
 *      Author: helsis
 */

#include "stdio.h"
#include "math.h"
#include "i2c.h"

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#define MAX_MPU6050_COUNT				3
#define MPU6050_DMA_BUFFER_SIZE     	14

#define MPU6050_ADDRESS 				0xD0	// AD0 low
//#define MPU6050_ADDRESS 				0xD1	// AD0 high
#define MPU6050_WHO_AM_I         		0x75

#define MPU6050_RA_CONFIG           	0x1A
#define MPU6050_RA_INT_CONFIG       	0x37
#define MPU6050_RA_INT_ENABLE       	0x38
#define MPU6050_RA_SMPLRT_DIV       	0x19
#define MPU6050_RA_PWR_MGMT_1       	0x6B
#define MPU6050_RA_GYRO_CONFIG      	0x1B
#define MPU6050_RA_ACCEL_CONFIG     	0x1C
#define MPU6050_RA_ACCEL_XOUT_H     	0x3B
#define MPU6050_RA_SIGNAL_PATH_RESET    0x68

#define MPU6050_PWR1_DEVICE_RESET_BIT   7
#define MPU6050_CLOCK_PLL_ZGYRO         0x03
#define MPU6050_GYRO_FS_2000        	0x03
#define MPU6050_ACCEL_FS_16         	0x03

typedef enum {
	MPU_RUNING,
	MPU_STOPPED,
	MPU_SIGNAL_PATH_RESET	= MPU6050_RA_SIGNAL_PATH_RESET
} MPU6050_Status;

typedef enum {
	MERR_OK,
	MERR_STATE_NOT_FOUND,
	MERR_STATE_MAX_COUNT,
	MERR_STATE_ALLREADY_EXIST,
} MPU6050_Error;

typedef struct {
	char				name;
	MPU6050_Status		status;

	I2C_HandleTypeDef	*I2C;

	uint8_t 			buffer[MPU6050_DMA_BUFFER_SIZE];

	short 				Ax;
	short 				Ay;
	short 				Az;

	double 				yaw;
	double 				roll;
	double 				pitch;
} MPU6050;

MPU6050* GetState(char name);

MPU6050_Error MPU6050_Init(char name, I2C_HandleTypeDef	*hi2c);
MPU6050_Error MPU6050_Update(char name);

#endif /* INC_MPU6050_H_ */
