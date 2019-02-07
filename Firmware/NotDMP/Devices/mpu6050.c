/***
	***************************************************************************************
	*	@file  	mpu6050.c
	*	@brief   MPU6050�ӿ���غ�����ʹ�����IIC�ӿ�
   ***************************************************************************************
   *  @description
	*
	*	����˵����
	*  1.�ṩMPU6050��ʼ������
	* 	2.�ṩ��ȡԭʼ���������ݺ���
	*	3.IIC��д�ֽں���
	*
	***************************************************************************************
***/

#include "mpu6050.h"  
#include "I2C_SW.h"  

MPU_Struct  MPU_Data;	//ȫ�ֽṹ����������ڴ洢MPU6050�Ĵ���������

/*****************************************************************************************
*	�� �� ��: MPU6050_Init
*	��ڲ���: ��
*	�� �� ֵ: SUCCESS - ��ȡ�ɹ�
*				 ERROR - ��ȡʧ��
*	��������: ��ʼ��MPU6050
*	˵    ��: ��
******************************************************************************************/

u8 MPU6050_Init(void)
{
	u8	MPU6050_ID = 0;	//����ID
	u8 status = ERROR;	//�����־λ
	
	delay_ms(10);	//�ڳ�ʼ������MPU6050֮ǰ��Ҫ���м����ʱ���������¶ϵ����ϵ�ᵼ�����ݲ���
  I2C_readRegister8(MPU6050_ADDRESS, MPU6050_WHO_AM_I, &MPU6050_ID);//��ȡ����ID
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
*	�� �� ��: MPU6050_ReadData
*	��ڲ���: ��
*	�� �� ֵ: SUCCESS - ��ȡ�ɹ�
*				 ERROR - ��ȡʧ��
*	��������: ��ȡMPU6050�ļ��ٶȡ������ǡ��¶�����
*	˵    ��: ��
******************************************************************************************/

u8	MPU6050_ReadData(void)
{
	u8	ReadBuffer[14];	//���ݴ洢��
	u8	status = 0;	//�����־
	short Temp_RegVaule = 0;	//�¶�ֵ
  
  status = I2C_readBuff(MPU6050_ADDRESS, MPU6050_ACCEL_XOUT_H, 14, ReadBuffer);

	MPU_Data.Accel_X = (ReadBuffer[0] << 8) + ReadBuffer[1];		//���ٶ�����
	MPU_Data.Accel_Y = (ReadBuffer[2] << 8) + ReadBuffer[3];
	MPU_Data.Accel_Z = (ReadBuffer[4] << 8) + ReadBuffer[5];

	MPU_Data.Gyro_X = (ReadBuffer[8] << 8) + ReadBuffer[9];		//����������
	MPU_Data.Gyro_Y = (ReadBuffer[10] << 8) + ReadBuffer[11];
	MPU_Data.Gyro_Z = (ReadBuffer[12] << 8) + ReadBuffer[13]-3847;

	Temp_RegVaule = (int16_t)((ReadBuffer[6] << 8) + ReadBuffer[7]);	//�¶�����
	MPU_Data.Temp = ( (double)Temp_RegVaule/340 )+ 36.53;		//����ʵ�ʵ��¶�ֵ
	return status;
}






