/*    
      ____                      _____                  +---+
     / ___\                     / __ \                 | R |
    / /                        / /_/ /                 +---+
   / /   ________  ____  ___  / ____/___  ____  __   __
  / /  / ___/ __ `/_  / / _ \/ /   / __ \/ _  \/ /  / /
 / /__/ /  / /_/ / / /_/  __/ /   / /_/ / / / / /__/ /
 \___/_/   \__,_/ /___/\___/_/    \___ /_/ /_/____  /
                                                 / /
                                            ____/ /
                                           /_____/
*/
#include "UART1.h"
#include "stdio.h"


//uart reicer flag
#define b_uart_head  0x80
#define b_rx_over    0x40


u8 U1TxBuffer[256];
u8 U1TxPackage[TX_BUFFER_SIZE];
u8 U1TxCounter=0;
u8 U1RxCounter=0;
u8 U1count=0; 
char TxPackFlag;//����Ԥ����ʽ���ݰ���־λ

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 




/**************************ʵ�ֺ���********************************************
*����ԭ��:		void U1NVIC_Configuration(void)
*��������:		����1�ж�����
�����������
���������û��	
*******************************************************************************/
void UART1NVIC_Configuration(void)
{
        NVIC_InitTypeDef NVIC_InitStructure; 
        /* Enable the USART1 Interrupt */
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
}



/**************************ʵ�ֺ���********************************************
*����ԭ��:		void Initial_UART1(u32 baudrate)
*��������:		��ʼ��UART1
���������u32 baudrate   ����RS232���ڵĲ�����
���������û��	
*******************************************************************************/
void UART1_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
  mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  
	RCC->APB2ENR|=1<<14;  //ʹ�ܴ���ʱ�� 
	GPIOA->CRH&=0XFFFFF00F;//IO״̬����
	GPIOA->CRH|=0X000008B0;//IO״̬����
	RCC->APB2RSTR|=1<<14;   //��λ����1
	RCC->APB2RSTR&=~(1<<14);//ֹͣ��λ	   	   
	//����������
 	USART1->BRR=mantissa; // ����������	 
	USART1->CR1|=0X200C;  //1λֹͣ,��У��λ.
  USART1->CR1|=1<<8;    //PE�ж�ʹ��
	USART1->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
 
  UART1NVIC_Configuration();//�ж�����
  printf("ϵͳʱ��Ƶ�ʣ�%dMHz \r\n",pclk2);
  printf("����1��ʼ�������ʣ�%d \r\n",bound);
 
  
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:		void UART1_Put_Char(unsigned char DataToSend)
*��������:		RS232����һ���ֽ�
���������
		unsigned char DataToSend   Ҫ���͵��ֽ�����
���������û��	
*******************************************************************************/
void UART1_Put_Char(unsigned char DataToSend)
{
	U1TxBuffer[U1count++] = DataToSend;  
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);  
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 UART1_Get_Char(void)
*��������:		RS232����һ���ֽ�  һֱ�ȴ���ֱ��UART1���յ�һ���ֽڵ����ݡ�
���������		 û��
���������     UART1���յ�������	
*******************************************************************************/
u8 UART1_Get_Char(void)
{
	while (!(USART1->SR & USART_FLAG_RXNE));
	return(USART_ReceiveData(USART1));
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:		void UART2_Put_String(unsigned char *Str)
*��������:		RS232�����ַ���
���������
		unsigned char *Str   Ҫ���͵��ַ���
���������û��	
*******************************************************************************/
void UART1_Put_String(unsigned char *Str)
{
	//�ж�Strָ��������Ƿ���Ч.
	while(*Str){
	//�Ƿ��ǻس��ַ� �����,������Ӧ�Ļس� 0x0d 0x0a
	if(*Str=='\r')UART1_Put_Char(0x0d);
		else if(*Str=='\n')UART1_Put_Char(0x0a);
			else UART1_Put_Char(*Str);
	//�ȴ��������.
  	//while (!(USART1->SR & USART_FLAG_TXE));
	//ָ��++ ָ����һ���ֽ�.
	Str++;
	}
/*
	//�ж�Strָ��������Ƿ���Ч.
	while(*Str){
	//�Ƿ��ǻس��ַ� �����,������Ӧ�Ļس� 0x0d 0x0a
	if(*Str=='\r')USART_SendData(USART1, 0x0d);
		else if(*Str=='\n')USART_SendData(USART1, 0x0a);
			else USART_SendData(USART1, *Str);
	//�ȴ��������.
  	while (!(USART1->SR & USART_FLAG_TXE));
	//ָ��++ ָ����һ���ֽ�.
	Str++;
	}		 */
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void UART2_Putc_Hex(uint8_t b)
*��������:		RS232��ʮ������ASCII��ķ�ʽ����һ���ֽ�����
				�Ƚ�Ŀ���ֽ����ݸ�4λת��ASCCII �����ͣ��ٽ���4λת��ASCII����
				��:0xF2 ������ " F2 "
���������
		uint8_t b   Ҫ���͵��ֽ�
���������û��	
*******************************************************************************/
void UART1_Putc_Hex(uint8_t b)
{
      /* �ж�Ŀ���ֽڵĸ�4λ�Ƿ�С��10 */
    if((b >> 4) < 0x0a)
        UART1_Put_Char((b >> 4) + '0'); //С��10  ,����Ӧ����0-9��ASCII
    else
        UART1_Put_Char((b >> 4) - 0x0a + 'A'); //���ڵ���10 ����Ӧ���� A-F

    /* �ж�Ŀ���ֽڵĵ�4λ �Ƿ�С��10*/
    if((b & 0x0f) < 0x0a)
        UART1_Put_Char((b & 0x0f) + '0');//С��10  ,����Ӧ����0-9��ASCII
    else
        UART1_Put_Char((b & 0x0f) - 0x0a + 'A');//���ڵ���10 ����Ӧ���� A-F
   UART1_Put_Char(' '); //����һ���ո�,�����ֿ������ֽ�
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void UART2_Putw_Hex(uint16_t w)
*��������:		RS232��ʮ������ASCII��ķ�ʽ����һ���ֵ�����.���Ƿ���һ��int
				��:0x3456 ������ " 3456 "
���������
		uint16_t w   Ҫ���͵���
���������û��	
*******************************************************************************/
void UART1_Putw_Hex(uint16_t w)
{
	//���͸�8λ����,����һ���ֽڷ���
    UART1_Putc_Hex((uint8_t) (w >> 8));
	//���͵�8λ����,����һ���ֽڷ���
    UART1_Putc_Hex((uint8_t) (w & 0xff));
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void UART2_Putdw_Hex(uint32_t dw)
*��������:		RS232��ʮ������ASCII��ķ�ʽ����32λ������.
				��:0xF0123456 ������ " F0123456 "
���������
		uint32_t dw   Ҫ���͵�32λ����ֵ
���������û��	
*******************************************************************************/
void UART1_Putdw_Hex(uint32_t dw)
{
    UART1_Putw_Hex((uint16_t) (dw >> 16));
    UART1_Putw_Hex((uint16_t) (dw & 0xffff));
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void UART2_Putw_Dec(uint16_t w)
*��������:		RS232��ʮ����ASCII��ķ�ʽ����16λ������.
				��:0x123 ����������ʮ�������� " 291 "
���������
		uint16_t w   Ҫ���͵�16λ����ֵ
���������û��	
*******************************************************************************/
void UART1_Putw_Dec(uint32_t w)
{
    uint32_t num = 100000;
    uint8_t started = 0;

    while(num > 0)
    {
        uint8_t b = w / num;
        if(b > 0 || started || num == 1)
        {
            UART1_Put_Char('0' + b);
            started = 1;
        }
        w -= b * num;

        num /= 10;
    }
}




unsigned char rx_buffer[RX_BUFFER_SIZE];

//------------------------------------------------------
void USART1_IRQHandler(void)
{
  
  if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
  {   
    USART_SendData(USART1, U1TxBuffer[U1TxCounter++]);
    USART_ClearITPendingBit(USART1, USART_IT_TXE);  
    if(U1TxCounter == U1count){USART_ITConfig(USART1, USART_IT_TXE, DISABLE);}
  }

  else if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
  rx_buffer[U1RxCounter++]=USART_ReceiveData(USART1);
  if(U1RxCounter==RX_BUFFER_SIZE)
  {
  U1RxCounter=0;
  USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  }
  }
}


void DEBUG_PRINTLN(unsigned char *Str)
 {
	  UART1_Put_String(Str);  //ͨ��USART1 ���͵�����Ϣ
 }




