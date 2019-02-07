#ifndef __MPU6050_H
#define __MPU6050_H

#include "stm32f10x.h"
#include "delay.h"

typedef struct		// MPU6050数据
{
	short Accel_X;	//X轴加速度
	short Accel_Y; //Y轴加速度
	short Accel_Z; //Z轴加速度

	short Gyro_X;  //X轴陀螺仪
	short Gyro_Y;  //Y轴陀螺仪
	short Gyro_Z;  //Z轴陀螺仪

	float Temp;		//温度数据
}MPU_Struct;

extern MPU_Struct  MPU_Data;	//添加定义声明

/*----------------------------------- MPU6050相关定义 ----------------------------------*/

#define  MPU6050_ADDRESS      	0x68	//器件地址

#define	MPU6050_SMPLRT_DIV		0x19	//采样频率分频寄存器
#define	MPU6050_CONFIG				0x1A	//芯片配置寄存器
#define	MPU6050_GYRO_CONFIG		0x1B	//陀螺仪配置
#define	MPU6050_ACCEL_CONFIG		0x1C	//加速度计配置

#define	MPU6050_ACCEL_XOUT_H		0x3B	//加速度数据寄存器
#define	MPU6050_ACCEL_XOUT_L		0x3C
#define	MPU6050_ACCEL_YOUT_H		0x3D
#define	MPU6050_ACCEL_YOUT_L		0x3E
#define	MPU6050_ACCEL_ZOUT_H		0x3F
#define	MPU6050_ACCEL_ZOUT_L		0x40

#define	MPU6050_GYRO_XOUT_H		0x43	//陀螺仪数据寄存器
#define	MPU6050_GYRO_XOUT_L		0x44
#define	MPU6050_GYRO_YOUT_H		0x45
#define	MPU6050_GYRO_YOUT_L		0x46
#define	MPU6050_GYRO_ZOUT_H		0x47
#define	MPU6050_GYRO_ZOUT_L		0x48

#define	MPU6050_TEMP_OUT_H		0x41	//温度数据寄存器
#define	MPU6050_TEMP_OUT_L		0x42

#define	MPU6050_PWR_MGMT_1		0x6B	//电源管理1
#define	MPU6050_WHO_AM_I			0x75	//器件地址寄存器

/*------------------------------------ 函数声明 -----------------------------------*/  	

u8 MPU6050_Init(void);				//MPU6050初始化		
u8	MPU6050_ReadData(void);			//读取传感器数据


#endif //__MPU6050_H
