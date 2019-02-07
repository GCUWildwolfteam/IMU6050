/*


*/

#include "HW_SPI.h"

static __IO uint32_t  SPITimeout = SPI_FLAG_TIMEOUT; 
static uint8_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);



/*
********************************************************************************************************************
		�������ã�Ӳ��SPI��ʼ��
		��������
		����ֵ����
********************************************************************************************************************
*/
void SPIx_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	//ʹ��SPIʱ��
  SPI_APBxClock_Cmd(SPI_CLK, ENABLE);
	
	//ʹ��GPIOʱ��
	SPI_GPIO_APBxClock_Cmd(SPI_SCK_CLK | SPI_MISO_CLK | SPI_MOSI_CLK, ENABLE);
	
  /* ����SCK����*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(SPI_SCK_PORT, &GPIO_InitStructure);

  /* ����MISO����*/
  GPIO_InitStructure.GPIO_Pin = SPI_MISO_PIN;
  GPIO_Init(SPI_MISO_PORT, &GPIO_InitStructure);

  /* ����MOSI����*/
  GPIO_InitStructure.GPIO_Pin = SPI_MOSI_PIN;
  GPIO_Init(SPI_MOSI_PORT, &GPIO_InitStructure);
	
	
	/* SPI ģʽ���� */
	/*
	����SPIģʽ0��ģʽ3
	SPIģʽ		����ʱCLK��ƽ		������Ե
		0							��				������Ե
		1							��				ż����Ե
		2							��				������Ե
		3							��				ż����Ե
	*/
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//˫��ȫ˫��ģʽ
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//SPI����ģʽ
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//8λ����֡����
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//ʱ�Ӽ���CPOL��SPI����ʱCLK��ƽ�����ߵ�ƽ
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//ʱ����λCPHA����/ż����Ե��������ż����Ե
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//�������CS��
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;	//SPIʱ�ӷ�Ƶ 4.5M
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//��λ����
  //SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCУ����ʽ��ͨ�����ã�����
  SPI_Init(Using_SPI , &SPI_InitStructure);

  /* ʹ�� SPI  */
  SPI_Cmd(Using_SPI , ENABLE);
	
}

/*
********************************************************************************************************************
		�������ã�SPI��������
		������Ҫ���͵�����
		����ֵ���ӻ����ص�����
********************************************************************************************************************
*/
u8 SPI_SendByte(u8 byte)
{
	 SPITimeout = SPI_FLAG_TIMEOUT;
  /* ��ⷢ�����ݻ������ǿ�,TXE��־λ */
  while (SPI_I2S_GetFlagStatus(Using_SPI , SPI_I2S_FLAG_TXE) == RESET)
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
   }

  /* ��Ҫ���͵�����д�뷢�����ݻ����� */
  SPI_I2S_SendData(Using_SPI , byte);

	SPITimeout = SPI_FLAG_TIMEOUT;
  /* �����ջ������ǿ�,�����������ѷ�����ɣ������յ����ݣ�RXNE��־λ */
  while (SPI_I2S_GetFlagStatus(Using_SPI , SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
   }

  /* ���ؽ��յ������� */
  return SPI_I2S_ReceiveData(Using_SPI);
}


/*
********************************************************************************************************************
		�������ã�SPI����2�ֽ�����
		������Ҫ���͵�����
		����ֵ���ӻ����ص�����
********************************************************************************************************************
*/
u16 SPI_FLASH_SendHalfWord(u16 HalfWord)
{
	  SPITimeout = SPI_FLAG_TIMEOUT;
  /* ��ⷢ�����ݻ������ǿ�,TXE��־λ */
  while (SPI_I2S_GetFlagStatus(Using_SPI , SPI_I2S_FLAG_TXE) == RESET)
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(2);
   }
	
   /* ��Ҫ���͵�����д�뷢�����ݻ����� */
  SPI_I2S_SendData(Using_SPI , HalfWord);

	 SPITimeout = SPI_FLAG_TIMEOUT;
   /* �����ջ������ǿ�,�����������ѷ�����ɣ������յ����ݣ�RXNE��־λ */
  while (SPI_I2S_GetFlagStatus(Using_SPI , SPI_I2S_FLAG_RXNE) == RESET)
	 {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(3);
   }
  /* ���ؽ��յ������� */
  return SPI_I2S_ReceiveData(Using_SPI);
}

/*
********************************************************************************************************************
		�������ã�SPI���Ϳ��ֽ�������ʱ�ӽ��մӻ�������
		��������
		����ֵ���ӻ����ص�����
********************************************************************************************************************
*/
u8 SPI_ReadByte(void)
{
  return (SPI_SendByte(Dummy_Byte));
}


/*
********************************************************************************************************************
		�������ã�SPIͨ�Ŵ���ص�
		��������
		����ֵ����
********************************************************************************************************************
*/
static uint8_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  printf("SPI��ʱ ErrorCode = %d",errorCode);
  return 0;
}

