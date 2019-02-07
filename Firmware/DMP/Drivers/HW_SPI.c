/*


*/

#include "HW_SPI.h"

static __IO uint32_t  SPITimeout = SPI_FLAG_TIMEOUT; 
static uint8_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);



/*
********************************************************************************************************************
		函数作用：硬件SPI初始化
		参数：无
		返回值：无
********************************************************************************************************************
*/
void SPIx_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	//使能SPI时钟
  SPI_APBxClock_Cmd(SPI_CLK, ENABLE);
	
	//使能GPIO时钟
	SPI_GPIO_APBxClock_Cmd(SPI_SCK_CLK | SPI_MISO_CLK | SPI_MOSI_CLK, ENABLE);
	
  /* 配置SCK引脚*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(SPI_SCK_PORT, &GPIO_InitStructure);

  /* 配置MISO引脚*/
  GPIO_InitStructure.GPIO_Pin = SPI_MISO_PIN;
  GPIO_Init(SPI_MISO_PORT, &GPIO_InitStructure);

  /* 配置MOSI引脚*/
  GPIO_InitStructure.GPIO_Pin = SPI_MOSI_PIN;
  GPIO_Init(SPI_MOSI_PORT, &GPIO_InitStructure);
	
	
	/* SPI 模式配置 */
	/*
	多用SPI模式0，模式3
	SPI模式		空闲时CLK电平		采样边缘
		0							低				奇数边缘
		1							低				偶数边缘
		2							高				奇数边缘
		3							高				偶数边缘
	*/
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//双线全双工模式
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//SPI主机模式
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//8位数据帧长度
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//时钟极性CPOL（SPI空闲时CLK电平），高电平
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//时钟相位CPHA（奇/偶数边缘采样），偶数边缘
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//软件控制CS线
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;	//SPI时钟分频 4.5M
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//高位先行
  //SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC校验表达式，通常不用，乱填
  SPI_Init(Using_SPI , &SPI_InitStructure);

  /* 使能 SPI  */
  SPI_Cmd(Using_SPI , ENABLE);
	
}

/*
********************************************************************************************************************
		函数作用：SPI传输数据
		参数：要发送的数据
		返回值：从机返回的数据
********************************************************************************************************************
*/
u8 SPI_SendByte(u8 byte)
{
	 SPITimeout = SPI_FLAG_TIMEOUT;
  /* 检测发送数据缓冲区非空,TXE标志位 */
  while (SPI_I2S_GetFlagStatus(Using_SPI , SPI_I2S_FLAG_TXE) == RESET)
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
   }

  /* 将要发送的数据写入发送数据缓冲区 */
  SPI_I2S_SendData(Using_SPI , byte);

	SPITimeout = SPI_FLAG_TIMEOUT;
  /* 检测接收缓冲区非空,（表明数据已发送完成，并接收到数据）RXNE标志位 */
  while (SPI_I2S_GetFlagStatus(Using_SPI , SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
   }

  /* 返回接收到的数据 */
  return SPI_I2S_ReceiveData(Using_SPI);
}


/*
********************************************************************************************************************
		函数作用：SPI传输2字节数据
		参数：要发送的数据
		返回值：从机返回的数据
********************************************************************************************************************
*/
u16 SPI_FLASH_SendHalfWord(u16 HalfWord)
{
	  SPITimeout = SPI_FLAG_TIMEOUT;
  /* 检测发送数据缓冲区非空,TXE标志位 */
  while (SPI_I2S_GetFlagStatus(Using_SPI , SPI_I2S_FLAG_TXE) == RESET)
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(2);
   }
	
   /* 将要发送的数据写入发送数据缓冲区 */
  SPI_I2S_SendData(Using_SPI , HalfWord);

	 SPITimeout = SPI_FLAG_TIMEOUT;
   /* 检测接收缓冲区非空,（表明数据已发送完成，并接收到数据）RXNE标志位 */
  while (SPI_I2S_GetFlagStatus(Using_SPI , SPI_I2S_FLAG_RXNE) == RESET)
	 {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(3);
   }
  /* 返回接收到的数据 */
  return SPI_I2S_ReceiveData(Using_SPI);
}

/*
********************************************************************************************************************
		函数作用：SPI发送空字节来驱动时钟接收从机的数据
		参数：无
		返回值：从机返回的数据
********************************************************************************************************************
*/
u8 SPI_ReadByte(void)
{
  return (SPI_SendByte(Dummy_Byte));
}


/*
********************************************************************************************************************
		函数作用：SPI通信错误回调
		参数：无
		返回值：无
********************************************************************************************************************
*/
static uint8_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  printf("SPI超时 ErrorCode = %d",errorCode);
  return 0;
}

