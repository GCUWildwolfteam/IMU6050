/***
	***************************************************************************************
	*	@file  	mpu6050.c
	*	@brief   MPU6050接口相关函数，使用软件IIC接口
   ***************************************************************************************
   *  @description
	*
	*	功能说明：
	*  1.提供MPU6050初始化函数
	* 	2.提供读取原始传感器数据函数
	*	3.IIC读写字节函数
	*
	***************************************************************************************
***/

#include "mpu6050.h"  
#include "I2C_SW.h"  

MPU_Struct  MPU_Data;	//全局结构体变量，用于存储MPU6050的传感器数据

/*****************************************************************************************
*	函 数 名: MPU6050_Init
*	入口参数: 无
*	返 回 值: SUCCESS - 读取成功
*				 ERROR - 读取失败
*	函数功能: 初始化MPU6050
*	说    明: 无
******************************************************************************************/

u8 MPU6050_Init(void)
{
	u8	MPU6050_ID = 0;	//器件ID
	u8 status = ERROR;	//错误标志位
	
	delay_ms(10);	//在初始化配置MPU6050之前需要进行简短延时，否则重新断电再上电会导致数据不对
  I2C_readRegister8(MPU6050_ADDRESS, MPU6050_WHO_AM_I, &MPU6050_ID);//读取器件ID
	if(MPU6050_ID == MPU6050_ADDRESS)	
	{
		I2C_writeRegister8(MPU6050_ADDRESS, MPU6050_PWR_MGMT_1, 0x00);		
		I2C_writeRegister8(MPU6050_ADDRESS, MPU6050_SMPLRT_DIV, 0x07);		
		I2C_writeRegister8(MPU6050_ADDRESS, MPU6050_CONFIG, 0x06);
		I2C_writeRegister8(MPU6050_ADDRESS, MPU6050_GYRO_CONFIG, 0xE8);
		I2C_writeRegister8(MPU6050_ADDRESS, MPU6050_ACCEL_CONFIG, 0x01);			
		
		status = SUCCESS;
	}			
	return status;	
}

/*****************************************************************************************
*	函 数 名: MPU6050_ReadData
*	入口参数: 无
*	返 回 值: SUCCESS - 读取成功
*				 ERROR - 读取失败
*	函数功能: 读取MPU6050的加速度、陀螺仪、温度数据
*	说    明: 无
******************************************************************************************/

u8	MPU6050_ReadData(void)
{
	u8	ReadBuffer[14];	//数据存储区
	u8	status = 0;	//错误标志
	short Temp_RegVaule = 0;	//温度值
  
  status = I2C_readBuff(MPU6050_ADDRESS, MPU6050_ACCEL_XOUT_H, 14, ReadBuffer);

	MPU_Data.Accel_X = (ReadBuffer[0] << 8) + ReadBuffer[1];		//加速度数据
	MPU_Data.Accel_Y = (ReadBuffer[2] << 8) + ReadBuffer[3];
	MPU_Data.Accel_Z = (ReadBuffer[4] << 8) + ReadBuffer[5];

	MPU_Data.Gyro_X = (ReadBuffer[8] << 8) + ReadBuffer[9];		//陀螺仪数据
	MPU_Data.Gyro_Y = (ReadBuffer[10] << 8) + ReadBuffer[11];
	MPU_Data.Gyro_Z = (ReadBuffer[12] << 8) + ReadBuffer[13]-3847;

	Temp_RegVaule = (int16_t)((ReadBuffer[6] << 8) + ReadBuffer[7]);	//温度数据
	MPU_Data.Temp = ( (double)Temp_RegVaule/340 )+ 36.53;		//计算实际的温度值
	return status;
}






