/*
 * mpu6050.c
 *
 *  Created on: Apr 28, 2024
 *      Author: helsis
 */

#include "mpu6050.h"

MPU6050 devices[MAX_MPU6050_COUNT];

static int devices_count;

static HAL_StatusTypeDef MPU6050_Ready(MPU6050 *device) {
	return HAL_I2C_IsDeviceReady(device -> I2C, MPU6050_ADDRESS, 1, 1000);
}

static HAL_StatusTypeDef MPU6050_Write(MPU6050 *device, uint8_t reg_addr, uint8_t val) {
	return HAL_I2C_Mem_Write(device -> I2C, MPU6050_ADDRESS, reg_addr, I2C_MEMADD_SIZE_8BIT, &val, 1, 0xFF);
}

static HAL_StatusTypeDef MPU6050_Read(MPU6050 *device, uint8_t reg_addr) {
	return HAL_I2C_Mem_Read(device -> I2C, MPU6050_ADDRESS, reg_addr, I2C_MEMADD_SIZE_8BIT, &device->status, 1, 0xFF);
}

MPU6050* MPU6050_GetState(char name) {
	for(int i = 0; i < devices_count; i++)
		if (devices[i].name == name) return &devices[i];

	return (MPU6050*)NULL;
}

MPU6050_Error MPU6050_Init(char name, I2C_HandleTypeDef *hi2c) {
	MPU6050 *device = MPU6050_GetState(name);

    if (device != NULL)
		return MERR_STATE_ALLREADY_EXIST;

    if (devices_count == MAX_MPU6050_COUNT)
    	return MERR_STATE_MAX_COUNT;

    device 						= &devices[devices_count++];
    device -> name				= name;
    device -> status			= MPU_STOPPED;

    device -> I2C				= hi2c;

    MPU6050_Read(device, MPU6050_WHO_AM_I);

	if (MPU6050_Ready(device) == HAL_OK && device -> status == MPU6050_RA_SIGNAL_PATH_RESET) {
		MPU6050_Write(device, MPU6050_RA_PWR_MGMT_1, 1 << 7);

		HAL_Delay(50);

		MPU6050_Write(device, MPU6050_RA_PWR_MGMT_1, MPU6050_CLOCK_PLL_ZGYRO);	//PLL with Z axis gyroscope reference
		MPU6050_Write(device, MPU6050_RA_CONFIG, 0x01); 						//DLPF_CFG = 1: Fs=1khz; bandwidth=42hz
		MPU6050_Write(device, MPU6050_RA_SMPLRT_DIV, 0x00);						//500Hz sample rate ~ 2ms
		MPU6050_Write(device, MPU6050_RA_GYRO_CONFIG, MPU6050_GYRO_FS_2000);	//Gyro full scale setting
		MPU6050_Write(device, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACCEL_FS_16);	//Accel full scale setting
//		MPU6050_Write(device, MPU6050_RA_INT_CONFIG, 1 << 4);					//interrupt status bits are cleared on any read operation
//		MPU6050_Write(device, MPU6050_RA_INT_ENABLE, 1 << 0);					//interupt occurs when data is ready. The interupt routine is in the receiver.c file.
		MPU6050_Write(device, MPU6050_RA_SIGNAL_PATH_RESET, 0x07);				//reset gyro and accel sensor

	}

	printf ("============== Stepper: %c ==============\r\n", device -> name);
	printf ("  Status: 0x%x \r\n", device -> status);
	printf ("=========================================\r\n\r\n");

    HAL_I2C_Mem_Read_DMA(
		device -> I2C, MPU6050_ADDRESS,
		MPU6050_RA_ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT,
		device -> buffer, MPU6050_DMA_BUFFER_SIZE
	);

	return MERR_OK;
}

MPU6050_Error MPU6050_Update(char name) {
	MPU6050 *device = MPU6050_GetState(name);

    if (device == NULL)
		return MERR_STATE_NOT_FOUND;

    HAL_I2C_Mem_Read_DMA(
		device -> I2C, MPU6050_ADDRESS,
		MPU6050_RA_ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT,
		device -> buffer, MPU6050_DMA_BUFFER_SIZE
	);

	device -> Ax = device -> buffer[0] << 8 | device -> buffer[1];
	device -> Ay = device -> buffer[2] << 8 | device -> buffer[3];
	device -> Az = device -> buffer[4] << 8 | device -> buffer[5];

//	device -> Gx = (int16_t) (device -> buffer[8] << 8 | device -> buffer[9])
//			/ 131.072f;
//	device -> Gy = (int16_t) (device -> buffer[10] << 8 | device -> buffer[11])
//			/ 131.072f;
//	device -> Gz = (int16_t) (device -> buffer[12] << 8 | device -> buffer[13])
//			/ 131.072f;

//	device->pitch = 180
//			* atan(device->Ax
//					/ sqrt(device->Ay * device->Ay
//							+ device->Az * device->Az)) / M_PI;
//	device->roll = 180
//			* atan(device->Ay
//					/ sqrt(device->Ax * device->Ax
//							+ device->Az * device->Az)) / M_PI;
//	device->yaw = 180
//			* atan(device->Az
//					/ sqrt(device->Ax * device->Ax
//							+ device->Az * device->Az)) / M_PI;


	printf("Ax:%hu, Ay:%hu, Az:%hu\r\n", device -> Ax, device -> Ay, device -> Az);
//	printf("P:%lf, R:%lf, Y:%lf\r\n", device -> pitch, device -> roll, device -> yaw);

	return MERR_OK;
}


void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	for(int i = 0; i < devices_count; i++)
		if(hi2c -> Instance == devices[i].I2C -> Instance)
			MPU6050_Update('G');
}
