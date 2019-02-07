/**
  ******************************************************************************
  * @file    I2C_SW.c
  * @author  Hare
  * @version V1.1
  * @date    
  * @brief   软件I2C应用函数接口
  *			 2018-01-27 修复I2C读函数返回值错误导致DMP初始化失败的问题
  ******************************************************************************
  */

#include "I2C_SW.h"


static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);
void I2C_Stop(void);


/**
  * @brief  用作模拟I2C产生时钟信号的延时
  *	@details 	可用逻辑分析仪测量I2C通讯时的频率
  *				工作条件：CPU主频180MHz ，MDK编译环境，1级优化
  *				经测试，循环次数为20~250时都能通讯正常
  * @param  None
  * @retval None
  */
//static void I2C_Delay(void)
//{
//	uint8_t i;
//	for (i = 0; i < 30; i++);
//}

/**
  * @brief  用作模拟I2C产生时钟信号的延时
  *	@details 	可用逻辑分析仪测量I2C通讯时的频率
  *				工作条件：CPU主频72MHz ，MDK编译环境，0级优化
  *       循环次数    SCL频率
  *           5       421Khz
  * @param  None
  * @retval None
  */
static void I2C_Delay(void)
{
	uint8_t i;
	for (i = 0; i < 4; i++);
}




/**
  * @brief  模拟I2C初始化
  * @param  None
  * @retval None
  */
void I2C_SW_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 使能GPIO时钟 */
	I2C_SW_GPIO_CLK_CMD(I2C_SW_GPIO_CLK, ENABLE);
	
	/* 配置GPIO ( SCL,SDA 均为复用开漏输出 ) */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
	GPIO_InitStructure.GPIO_Pin = I2C_SW_SCL_Pin;
  GPIO_Init(I2C_SW_SCL_PORT, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin = I2C_SW_SDA_Pin;  
	GPIO_Init(I2C_SW_SDA_PORT, &GPIO_InitStructure);
	
	I2C_Stop();
	
}


/**
  * @brief  发送I2C起始信号
  * @param  None
  * @retval None
  */
void I2C_Start(void)
{
	SDA_H;
	SCL_H;
	I2C_Delay();
	SDA_L;
	I2C_Delay();
	SCL_L;
	I2C_Delay();
}


/**
  * @brief 发送I2C停止信号
  * @param  None
  * @retval None
  */
void I2C_Stop(void)
{
	SDA_L;
	SCL_H;
	I2C_Delay();
	SDA_H;
}


/**
  * @brief  向I2C设备写8bit数据
  * @param  data 数据
  * @retval None
  */
void I2C_SendByte(uint8_t data)
{
	uint8_t i;
	//发送的第一个位为7bit
	for (i = 0; i < 8; i++)
	{		
		if (data & 0x80)
		{
			SDA_H;
		}
		else
		{
			SDA_L;
		}
		I2C_Delay();
		SCL_H;
		I2C_Delay();	
		SCL_L;
		if (i == 7)
		{
			 SDA_H; // 释放总线
		}
		data <<= 1;	/* 左移一位 */
		I2C_Delay();
	}
}



/**
  * @brief  从I2C设备读数据
  * @param  None
  * @retval 读取到的数据
  */
uint8_t I2C_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* 读到第一个位为数据的7bit */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		SCL_H;
		I2C_Delay();
		if (SDA_READ)
		{
			value++;
		}
		SCL_L;
		I2C_Delay();
	}
	return value;
}



/**
  * @brief  等待i2c设备响应
  * @param  None
  * @retval 返回0表示设备已应答，返回1表示设备无响应
  */
uint8_t I2C_WaitAck(void)
{
	uint8_t re;

	SDA_H;	/* 释放SDA总线 */
	I2C_Delay();
	SCL_H;	/* 拉高SCL，设备应答 */
	I2C_Delay();
	if (SDA_READ)	/* 读SDA */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	SCL_L;
	I2C_Delay();
	return re;
}


/**
  * @brief  产生一个ACK信号
  * @param  None
  * @retval None
  */
void I2C_Ack(void)
{
	SDA_L;	/* 拉低SDA */
	I2C_Delay();
	SCL_H;	/* 产生时钟信号 */
	I2C_Delay();
	SCL_L;
	I2C_Delay();
	SDA_H;	/* 释放SDA总线 */
}


/**
  * @brief  产生一个非ACK信号
  * @param  None
  * @retval None
  */
void I2C_NAck(void)
{
	SDA_H;	/* 拉高SDA */
	I2C_Delay();
	SCL_H;	/* 产生时钟信号 */
	I2C_Delay();
	SCL_L;
	I2C_Delay();	
}




/**
  * @brief  检查设备是否存在
  * @param  D_Address 	设备地址
  * @retval 返回1表示设备正常，返回0表示找不到设备
  */
uint8_t I2C_checkDevice(uint8_t D_Address)
{
	uint8_t ucAck;

	I2C_Stop();	/*发送停止信号，复位所有I2C设备*/
	
	I2C_Start();		/* 发送起始信号 */

	I2C_SendByte(D_Address | I2C_WR_ADDR);/* 发送写地址 */
	ucAck = I2C_WaitAck();	/* 等待设备响应 */

	I2C_Stop();			/* 发送结束信号  */

	return !ucAck;
}


/**
  * @brief  向I2C设备写多个数据
  * @param      D_Address 	设备地址
  * @param			R_Address	  设备寄存器地址
  * @param			len 		    数据长度
  * @param[in]	data_buf 	  数据数组
  * @retval 成功返回1，失败返回0
  */
uint8_t I2C_writeBuff(uint8_t D_Address, uint8_t R_Address, uint16_t len, uint8_t *data_buf)
{
	
	//1,发送起始信号
	I2C_Start();
	
	//2,发送7位设备地址+写方向
	I2C_SendByte((D_Address<<1) | I2C_WR_ADDR);
	
	//3,检查设备是否响应，1为无响应
	if (I2C_WaitAck() == 1){
		return I2C_TIMEOUT_UserCallback(1);
	}
	
	//4，发送寄存器地址
	I2C_SendByte(R_Address);

	//5,检测EV8_2事件，检查设备是否响应，1为无响应
	if (I2C_WaitAck() == 1){
		return I2C_TIMEOUT_UserCallback(2);
	}
	for (uint8_t i=0;i<len;i++){
		//6,发送一个数据
		I2C_SendByte(data_buf[i]);
	
		//7,检测EV8_2事件
		if (I2C_WaitAck() == 1){
			return I2C_TIMEOUT_UserCallback(3);
		}
	 
	}
	/* 8,发送结束信号  */
	I2C_Stop();
	
	return 1;
}


/**
  * @brief  向I2C设备读多个数据
  * @param  D_Address 	设备地址
  *			R_Address	设备寄存器地址
  *			len 		  数据长度
  *			data_buf 	数据数组
  * @retval 成功返回1，失败返回0
  */
uint8_t I2C_readBuff(uint8_t D_Address, uint8_t R_Address, uint16_t len, uint8_t *data_buf)
{
	uint8_t i = 0;

	//发送起始信号
	I2C_Start();

	//发送7位设备地址+写方向
	I2C_SendByte((D_Address<<1) | I2C_WR_ADDR);
	
	//检测EV6事件
	if (I2C_WaitAck() == 1){
			return I2C_TIMEOUT_UserCallback(4);
	}

	//发送寄存器地址
	I2C_SendByte(R_Address);

	//检测EV8_2事件，检查设备是否响应，1为无响应
	if (I2C_WaitAck() == 1){
			return I2C_TIMEOUT_UserCallback(5);
	}
	
	//发送启动信号，重新启动I2C总线，开始接收数据
	I2C_Start();
	
	//发送7位设备地址+读方向
	I2C_SendByte((D_Address<<1) | I2C_RD_ADDR);
	
	//检测EV6事件
	if (I2C_WaitAck() == 1){
			return I2C_TIMEOUT_UserCallback(6);
	}
	
	//开始接收len个数据
	while(len){
		
		data_buf[i] = I2C_ReadByte();
		if(len == 1){
			//当为最后一个数据时产生非应答信号并发送停止信号
			I2C_NAck();
		}
		else{
			I2C_Ack();
		}
		
		i++;
		len--;
	}
	
	I2C_Stop();
	
	return 1;
}


/**
  * @brief  向I2C设备写入一个8位的数据，通常用于写8位寄存器
  * @param  D_Address 	设备地址
  * @param  R_Address	  设备寄存器地址
  * @param	data 	  数据
  * @retval 成功返回1，失败返回0
  */
uint8_t I2C_writeRegister8(uint8_t D_Address, uint8_t R_Address, uint8_t data)
{
    return I2C_writeBuff(D_Address, R_Address, 1, &data);
}


/**
  * @brief  向I2C设备读一个8位的数据，通常用于读8位寄存器
  * @param          D_Address 	设备地址
  * @param  			  R_Address	  设备寄存器地址
  * @param  		    len 		    数据长度
  * @param[out]  		*p_data 	  数据指针
  * @retval 成功返回1，失败返回0
  */
uint8_t I2C_readRegister8(uint8_t D_Address, uint8_t R_Address, uint8_t *p_data)
{
    return I2C_readBuff(D_Address, R_Address, 1, p_data);
}


/**
  * @brief  向I2C设备读一个16位的数据，通常用于读16位寄存器(数据通常会是int16类型)
  * @param          D_Address 	设备地址
  * @param  			  R_Address	  设备寄存器地址
  * @param  		    len 		    数据长度
  * @param[out]  		*p_value 	  数据指针
  * @retval 成功返回1，失败返回0
  */
uint8_t I2C_readRegister16(uint8_t D_Address, uint8_t R_Address, int16_t *p_value){
	uint8_t data[2];
	if(I2C_readBuff(D_Address, R_Address, 2, data)){
		*p_value = data[0] << 8 | data[1];
		return 1;
	}
	else 
		return 0;
}


/**
  * @brief  向I2C设备写入一个16位的数据，通常用于写16位寄存器
  * @param  D_Address 	设备地址
  * @param  R_Address	  设备寄存器地址
  * @param	value 	  数据
  * @retval 成功返回1，失败返回0
  */
uint8_t I2C_writeRegister16(uint8_t D_Address, uint8_t R_Address, int16_t value){
	uint8_t data[2];
	data[0] = (uint8_t)(value >> 8);
	data[1] = (uint8_t)value;
	return I2C_writeBuff(D_Address, R_Address, 2, data);
}

/**
  * @brief  向I2C设备读一个位的数据，通常用于读寄存器某一位
  * @param  D_Address 	设备地址
  * @param  R_Address	  设备寄存器地址
  * @param	pos 	      位，范围0-7
  * @param	*p_state 	  读回来的位状态，0或1
  * @retval 成功返回1，失败返回0
  */
uint8_t I2C_readRegisterBit(uint8_t D_Address, uint8_t R_Address, uint8_t pos, uint8_t *p_state){
    uint8_t value = 0;
		if(I2C_readRegister8(D_Address, R_Address, &value))
    {
      *p_state = ((value >> pos) & 1);
      return 1;
    }
    return 0;
}

// Write register bit
/**
  * @brief  向I2C设备写一个位的数据，通常用于写寄存器某一位
  * @param  D_Address 	设备地址
  * @param  R_Address	  设备寄存器地址
  * @param	pos 	      位，范围0-7
  * @param	state 	    位状态，0或1
  * @retval 成功返回1，失败返回0
  */
uint8_t I2C_writeRegisterBit(uint8_t D_Address, uint8_t R_Address, uint8_t pos, uint8_t state){
    uint8_t value = 0;
    if(!I2C_readRegister8(D_Address, R_Address, &value))
      return 0;

    if (state){
        value |= (1 << pos);
    }
		else {
        value &= ~(1 << pos);
    }

	return I2C_writeRegister8(D_Address, R_Address, value);
}


/**
  * @brief  Basic management of the timeout situation.
  * @param  errorCode:打印错误代码.
  * @retval 返回0, 表示IIC读取失败.
  */
static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
	I2C_Stop();	/*发送停止信号，复位所有I2C设备*/
  /* Block communication and all processes */
	printf("I2C Timeout ErrorCode = %d\n",errorCode);
	return 0;
}



