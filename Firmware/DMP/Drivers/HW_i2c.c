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
	
	//使能I2C时钟与GPIO时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//配置GPIO ( PB6-SCL,PB7-SDA 均为复用开漏输出 )
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//配置I2C工作模式
	I2C_DeInit(I2C1);
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; 
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C1_ClockSpeed;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1 = I2C1_OwnAddress1;
	I2C_Init(I2C1, &I2C_InitStructure);
	
	//使能I2C1
	I2C_Cmd(I2C1, ENABLE);
	
	#endif
	
	
	
	#if defined (Open_I2C2)
	
	//使能I2C时钟与GPIO时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//配置GPIO ( PB10-SCL,PB11-SDA 均为复用开漏输出 )
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//配置I2C工作模式
	I2C_DeInit(I2C2);
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; 
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C2_ClockSpeed;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1 = I2C2_OwnAddress1;
	I2C_Init(I2C2, &I2C_InitStructure);
	
	//使能I2C2
	I2C_Cmd(I2C2, ENABLE);
	
	#endif
	
	
}




uint32_t I2C_Write(I2C_TypeDef* I2Cx, uint8_t D_Address, uint8_t R_Address, uint8_t data){
	
	//等待总线空闲
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)){
		//超时处理
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(9);
	}
	
	//发送起始信号
	I2C_GenerateSTART(I2Cx, ENABLE);
	
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//检测EV5事件
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)){
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(0);
  } 
	
	//发送7位设备地址+写方向
	I2C_Send7bitAddress(I2Cx, D_Address, I2C_Direction_Transmitter);
	
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//检测EV6事件
	 while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(1);
  }  
	
	//发送寄存器地址
	I2C_SendData(I2Cx, R_Address);
	
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//检测EV8_2事件
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(2);
  }
	
	//发送一个数据
	I2C_SendData(I2Cx, data);
	
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//检测EV8_2事件
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
		//超时处理
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(9);
	}

	//发送起始信号
	I2C_GenerateSTART(I2Cx, ENABLE);
	
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//检测EV5事件
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)){
		//超时处理
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(10);
	}

		//发送7位设备地址+写方向
	I2C_Send7bitAddress(I2Cx, D_Address, I2C_Direction_Transmitter);
	
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//检测EV6事件
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
		//超时处理
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(11);
	}
	
	//清除EV6
	I2C_Cmd(I2Cx, ENABLE);

	//发送寄存器地址
	I2C_SendData(I2Cx, R_Address);
	
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//检测EV8_2事件
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
		//超时处理
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(12);
	}
	
	//I2C_GenerateSTOP(I2Cx, ENABLE);

	I2C_GenerateSTART(I2Cx, ENABLE);
	
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//检测EV5事件
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)){
		//超时处理
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(13);
	}
	
	//发送7位设备地址+读方向
	I2C_Send7bitAddress(I2Cx, D_Address, I2C_Direction_Receiver);
	
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//检测EV6事件
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){
		//超时处理
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(14);
	}
	
	//开始接收quantity个数据
	while(quantity){
		
		if(quantity == 1){
			//当为最后一个数据时产生非应答信号并发送停止信号
			I2C_AcknowledgeConfig(I2Cx, DISABLE);
			I2C_GenerateSTOP(I2Cx, ENABLE);
		}
		   
    I2CTimeout = I2CT_LONG_TIMEOUT;
    /* 检测EV7事件 */ 
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
	//接收完后重新开启应答准备下一次通讯
	I2C_AcknowledgeConfig(I2Cx, ENABLE);
	
	return 1;
}


uint8_t I2C1_read(void){
	
	//返回读取到的数值
	uint8_t value;
	value = I2C1_rxBuffer[I2C1_readPoint];
	I2C1_readPoint++;
	return value;
}

uint8_t I2C2_read(void){
	
	//返回读取到的数值
	uint8_t value;
	value = I2C2_rxBuffer[I2C2_readPoint];
	I2C2_readPoint++;
	return value;
}


/**
  * @brief  Basic management of the timeout situation.
  * @param  errorCode:打印错误代码.
  * @retval 返回0,表示IIC读取失败.
  */
static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  printf("I2C超时 ErrorCode = %d\n",errorCode);
  return 0;
}


