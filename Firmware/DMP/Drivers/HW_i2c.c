/*



*/

#include "HW_i2c.h"


static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);
static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;

int I2C1_rxBuffer[BUFFER_LENGTH];
int I2C2_rxBuffer[BUFFER_LENGTH];
uint8_t I2C1_readPoint = 0;
uint8_t I2C2_readPoint = 0;




void I2Cx_Init(void){
	I2C_InitTypeDef I2C_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	#if defined (Open_I2C1)
	
	//ʹ��I2Cʱ����GPIOʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//����GPIO ( PB6-SCL,PB7-SDA ��Ϊ���ÿ�©��� )
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//����I2C����ģʽ
	I2C_DeInit(I2C1);
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; 
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C1_ClockSpeed;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1 = I2C1_OwnAddress1;
	I2C_Init(I2C1, &I2C_InitStructure);
	
	//ʹ��I2C1
	I2C_Cmd(I2C1, ENABLE);
	
	#endif
	
	
	
	#if defined (Open_I2C2)
	
	//ʹ��I2Cʱ����GPIOʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//����GPIO ( PB10-SCL,PB11-SDA ��Ϊ���ÿ�©��� )
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//����I2C����ģʽ
	I2C_DeInit(I2C2);
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; 
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C2_ClockSpeed;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1 = I2C2_OwnAddress1;
	I2C_Init(I2C2, &I2C_InitStructure);
	
	//ʹ��I2C2
	I2C_Cmd(I2C2, ENABLE);
	
	#endif
	
	
}




uint32_t I2C_Write(I2C_TypeDef* I2Cx, uint8_t D_Address, uint8_t R_Address, uint8_t data){
	
	//�ȴ����߿���
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)){
		//��ʱ����
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(9);
	}
	
	//������ʼ�ź�
	I2C_GenerateSTART(I2Cx, ENABLE);
	
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//���EV5�¼�
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)){
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(0);
  } 
	
	//����7λ�豸��ַ+д����
	I2C_Send7bitAddress(I2Cx, D_Address, I2C_Direction_Transmitter);
	
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//���EV6�¼�
	 while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(1);
  }  
	
	//���ͼĴ�����ַ
	I2C_SendData(I2Cx, R_Address);
	
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//���EV8_2�¼�
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(2);
  }
	
	//����һ������
	I2C_SendData(I2Cx, data);
	
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//���EV8_2�¼�
	 while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
  }
	
	I2C_GenerateSTOP(I2Cx, ENABLE);
	
	return 1;
}


uint32_t I2C_requestFrom(I2C_TypeDef* I2Cx, uint8_t D_Address, uint8_t R_Address, uint8_t quantity){
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	uint8_t i = 0;
	
	if(I2Cx == I2C1)
		I2C1_readPoint = 0;
	else
		I2C2_readPoint = 0;
	
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)){
		//��ʱ����
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(9);
	}

	//������ʼ�ź�
	I2C_GenerateSTART(I2Cx, ENABLE);
	
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//���EV5�¼�
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)){
		//��ʱ����
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(10);
	}

		//����7λ�豸��ַ+д����
	I2C_Send7bitAddress(I2Cx, D_Address, I2C_Direction_Transmitter);
	
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//���EV6�¼�
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
		//��ʱ����
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(11);
	}
	
	//���EV6
	I2C_Cmd(I2Cx, ENABLE);

	//���ͼĴ�����ַ
	I2C_SendData(I2Cx, R_Address);
	
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//���EV8_2�¼�
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
		//��ʱ����
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(12);
	}
	
	//I2C_GenerateSTOP(I2Cx, ENABLE);

	I2C_GenerateSTART(I2Cx, ENABLE);
	
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//���EV5�¼�
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)){
		//��ʱ����
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(13);
	}
	
	//����7λ�豸��ַ+������
	I2C_Send7bitAddress(I2Cx, D_Address, I2C_Direction_Receiver);
	
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//���EV6�¼�
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){
		//��ʱ����
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(14);
	}
	
	//��ʼ����quantity������
	while(quantity){
		
		if(quantity == 1){
			//��Ϊ���һ������ʱ������Ӧ���źŲ�����ֹͣ�ź�
			I2C_AcknowledgeConfig(I2Cx, DISABLE);
			I2C_GenerateSTOP(I2Cx, ENABLE);
		}
		   
    I2CTimeout = I2CT_LONG_TIMEOUT;
    /* ���EV7�¼� */ 
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)){
			if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
		} 
		
    {
			if(I2Cx == I2C1)
				I2C1_rxBuffer[i] = I2C_ReceiveData(I2Cx);
			else
				I2C2_rxBuffer[i] = I2C_ReceiveData(I2Cx);
			i++;
			quantity--;       
    }   
  	
	}
	//����������¿���Ӧ��׼����һ��ͨѶ
	I2C_AcknowledgeConfig(I2Cx, ENABLE);
	
	return 1;
}


uint8_t I2C1_read(void){
	
	//���ض�ȡ������ֵ
	uint8_t value;
	value = I2C1_rxBuffer[I2C1_readPoint];
	I2C1_readPoint++;
	return value;
}

uint8_t I2C2_read(void){
	
	//���ض�ȡ������ֵ
	uint8_t value;
	value = I2C2_rxBuffer[I2C2_readPoint];
	I2C2_readPoint++;
	return value;
}


/**
  * @brief  Basic management of the timeout situation.
  * @param  errorCode:��ӡ�������.
  * @retval ����0,��ʾIIC��ȡʧ��.
  */
static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  printf("I2C��ʱ ErrorCode = %d\n",errorCode);
  return 0;
}


