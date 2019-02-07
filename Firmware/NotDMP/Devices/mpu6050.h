#ifndef __MPU6050_H
#define __MPU6050_H

#include "stm32f10x.h"
#include "delay.h"

typedef struct		// MPU6050����
{
	short Accel_X;	//X����ٶ�
	short Accel_Y; //Y����ٶ�
	short Accel_Z; //Z����ٶ�

	short Gyro_X;  //X��������
	short Gyro_Y;  //Y��������
	short Gyro_Z;  //Z��������

	float Temp;		//�¶�����
}MPU_Struct;

extern MPU_Struct  MPU_Data;	//��Ӷ�������

/*----------------------------------- MPU6050��ض��� ----------------------------------*/

#define  MPU6050_ADDRESS      	0x68	//������ַ

#define	MPU6050_SMPLRT_DIV		0x19	//����Ƶ�ʷ�Ƶ�Ĵ���
#define	MPU6050_CONFIG				0x1A	//оƬ���üĴ���
#define	MPU6050_GYRO_CONFIG		0x1B	//����������
#define	MPU6050_ACCEL_CONFIG		0x1C	//���ٶȼ�����

#define	MPU6050_ACCEL_XOUT_H		0x3B	//���ٶ����ݼĴ���
#define	MPU6050_ACCEL_XOUT_L		0x3C
#define	MPU6050_ACCEL_YOUT_H		0x3D
#define	MPU6050_ACCEL_YOUT_L		0x3E
#define	MPU6050_ACCEL_ZOUT_H		0x3F
#define	MPU6050_ACCEL_ZOUT_L		0x40

#define	MPU6050_GYRO_XOUT_H		0x43	//���������ݼĴ���
#define	MPU6050_GYRO_XOUT_L		0x44
#define	MPU6050_GYRO_YOUT_H		0x45
#define	MPU6050_GYRO_YOUT_L		0x46
#define	MPU6050_GYRO_ZOUT_H		0x47
#define	MPU6050_GYRO_ZOUT_L		0x48

#define	MPU6050_TEMP_OUT_H		0x41	//�¶����ݼĴ���
#define	MPU6050_TEMP_OUT_L		0x42

#define	MPU6050_PWR_MGMT_1		0x6B	//��Դ����1
#define	MPU6050_WHO_AM_I			0x75	//������ַ�Ĵ���

/*------------------------------------ �������� -----------------------------------*/  	

u8 MPU6050_Init(void);				//MPU6050��ʼ��		
u8	MPU6050_ReadData(void);			//��ȡ����������


#endif //__MPU6050_H
