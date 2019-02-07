/**
  ******************************************************************************
  * @file    mpu6050.c
  * @author  Hare
  * @version V1.0
  * @date    
  * @brief   MPU6050Ӧ�ú����ӿ�
  ******************************************************************************
  */

#include "mpu6050.h"
#include "delay.h"
#include "usart.h"   
#include "I2C_SW.h"

/**
  * @brief  ��I2C�豸д�������
  * @param      addr 	      �豸��ַ
  * @param			reg	        �豸�Ĵ�����ַ
  * @param			len 		    ���ݳ���
  * @param[in]	*p_buf 	    ��������
  * @retval !!! �ɹ�����0��ʧ�ܷ���1 !!!
  */
uint8_t MPU_writeBuff(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *p_buf)
{
  return !I2C_writeBuff(addr, reg, len, p_buf);
}

/**
  * @brief  ��I2C�豸���������
  * @param      addr 	      �豸��ַ
  * @param			reg	        �豸�Ĵ�����ַ
  * @param			len 		    ���ݳ���
  * @param[out]	*p_buf 	    ��������
  * @retval !!! �ɹ�����0��ʧ�ܷ���1 !!!
  */
uint8_t MPU_readBuff(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *p_buf)
{
  return !I2C_readBuff(addr, reg, len, p_buf);
}

/**
  * @brief  ��I2C�豸���������
  * @param      addr 	      �豸��ַ
  * @param			reg	        �豸�Ĵ�����ַ
  * @param			len 		    ���ݳ���
  * @param[out]	*p_buf 	    ��������
  * @retval !!! �ɹ�����0��ʧ�ܷ���1 !!!
  */
uint8_t MPU_Read_Byte(uint8_t reg)
{
  uint8_t data = 0;
  MPU_readBuff(MPU_ADDR, reg, 1, &data);
  return data;
}

/**
  * @brief  ��I2C�豸д�������
  * @param      addr 	      �豸��ַ
  * @param			reg	        �豸�Ĵ�����ַ
  * @param			len 		    ���ݳ���
  * @param[in]	*p_buf 	    ��������
  * @retval !!! �ɹ�����0��ʧ�ܷ���1 !!!
  */
uint8_t MPU_Write_Byte(uint8_t reg, uint8_t data)
{
  return MPU_readBuff(MPU_ADDR, reg, 1, &data);
}
 
/**
  * @brief MPU6050��ʼ��
  * @param None
  * @retval !!! �ɹ�����0��ʧ�ܷ���1 !!!
  */
uint8_t MPU_Init(void)
{
  uint8_t res;
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80);	//��λMPU6050
  delay_ms(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);	//����MPU6050 
	MPU_Set_Gyro_Fsr(3);					//�����Ǵ�����,��2000dps
	MPU_Set_Accel_Fsr(0);					//���ٶȴ�����,��2g
	MPU_Set_Rate(50);						//���ò�����50Hz
	MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//�ر������ж�
	MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);	//I2C��ģʽ�ر�
	MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);	//�ر�FIFO
	MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80);	//INT���ŵ͵�ƽ��Ч
	res=MPU_Read_Byte(MPU_DEVICE_ID_REG);
	if(res==MPU_ADDR)//����ID��ȷ
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);	//����CLKSEL,PLL X��Ϊ�ο�
		MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);	//���ٶ��������Ƕ�����
		MPU_Set_Rate(50);						//���ò�����Ϊ50Hz
 	}
  else 
    return 1;
  
	return 0;
}
//����MPU6050�����Ǵ����������̷�Χ
//fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3);//���������������̷�Χ  
}
//����MPU6050���ٶȴ����������̷�Χ
//fsr:0,��2g;1,��4g;2,��8g;3,��16g
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3);//���ü��ٶȴ����������̷�Χ  
}
//����MPU6050�����ֵ�ͨ�˲���
//lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
uint8_t MPU_Set_LPF(u16 lpf)
{
	uint8_t data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU_CFG_REG,data);//�������ֵ�ͨ�˲���  
}
//����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
//rate:4~1000(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
uint8_t MPU_Set_Rate(u16 rate)
{
	uint8_t data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	//�������ֵ�ͨ�˲���
 	return MPU_Set_LPF(rate/2);	//�Զ�����LPFΪ�����ʵ�һ��
}

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��)
short MPU_Get_Temperature(void)
{
    uint8_t buf[2]; 
    short raw;
	float temp;
	MPU_readBuff(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
    raw=((u16)buf[0]<<8)|buf[1];  
    temp=36.53+((double)raw)/340;  
    return temp*100;
}
//�õ�������ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
uint8_t MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
  uint8_t buf[6],res;  
	res=MPU_readBuff(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];  
		*gy=((u16)buf[2]<<8)|buf[3];  
		*gz=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;
}
//�õ����ٶ�ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
uint8_t MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    uint8_t buf[6],res;  
	res=MPU_readBuff(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;
}


