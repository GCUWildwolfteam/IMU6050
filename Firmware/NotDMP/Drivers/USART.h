#ifndef __USART_H
#define __USART_H

#include "user_common.h"

#define USART1_BUFFLEN	255
#define USART2_BUFFLEN	255

/********USART1_Pin_define********/
#define USART1_TX_GPIO_CLK          RCC_APB2Periph_GPIOA
#define USART1_TX_GPIO_PORT         GPIOA
#define USART1_TX_Pin          	 	GPIO_Pin_9
#define USART1_TX_PINSOURCE			GPIO_PinSource9

#define USART1_RX_GPIO_CLK          RCC_APB2Periph_GPIOA
#define USART1_RX_GPIO_PORT         GPIOA
#define USART1_RX_Pin           	GPIO_Pin_10
#define USART1_RX_PINSOURCE			GPIO_PinSource10
/********USART1_Pin_define_END********/

/********USART1_NVIC_Config********/
#define USART1_NVIC_MainPriority 	6
#define USART1_NVIC_SubPriority 	0
/********USART1_NVIC_Config********/

/********USART1_DMA_define********/
#define USART1_RX_ADDR						(uint32_t)(&USART1->DR)	//串口1数据寄存器地址
#define USART1_RX_DMA_CHANNEL           	DMA1_Channel5		//DMA通道号
//#define USART1_RX_DMA_STREAM           		DMA2_Stream2		//DMA数据流
/********USART1_DMA_define_END********/


/********USART2_Pin_define********/
#define USART2_TX_GPIO_CLK          RCC_APB2Periph_GPIOA
#define USART2_TX_GPIO_PORT         GPIOA
#define USART2_TX_Pin          	 	GPIO_Pin_2
#define USART2_TX_PINSOURCE			GPIO_PinSource2

#define USART2_RX_GPIO_CLK          RCC_APB2Periph_GPIOA
#define USART2_RX_GPIO_PORT         GPIOA
#define USART2_RX_Pin           	GPIO_Pin_3
#define USART2_RX_PINSOURCE			GPIO_PinSource3
/********USART2_Pin_define_END********/

/********USART2_NVIC_Config********/
#define USART2_NVIC_MainPriority 	6
#define USART2_NVIC_SubPriority 	0
/********USART2_NVIC_Config********/

/********USART2_DMA_define********/
#define USART2_RX_ADDR						(uint32_t)(&USART2->DR)	//串口1数据寄存器地址
#define USART2_RX_DMA_CHANNEL           	DMA1_Channel6		//DMA通道号
//#define USART2_RX_DMA_STREAM           		DMA1_Stream5		//DMA数据流
/********USART2_DMA_define_END********/

/********USART3_Pin_define********/
#define USART3_TX_GPIO_CLK          RCC_APB2Periph_GPIOC
#define USART3_TX_GPIO_PORT         GPIOC
#define USART3_TX_Pin          	 	GPIO_Pin_10
#define USART3_TX_PINSOURCE			GPIO_PinSource10

#define USART3_RX_GPIO_CLK          RCC_APB2Periph_GPIOC
#define USART3_RX_GPIO_PORT         GPIOC
#define USART3_RX_Pin           	GPIO_Pin_11
#define USART3_RX_PINSOURCE			GPIO_PinSource11
/********USART3_Pin_define_END********/

/********USART3_NVIC_Config********/
#define USART3_NVIC_MainPriority 	6
#define USART3_NVIC_SubPriority 	0
/********USART1_NVIC_Config********/

/********USART3_DMA_define********/
#define USART3_RX_ADDR						(uint32_t)(&USART3->DR)	//串口1数据寄存器地址
#define USART3_RX_DMA_CHANNEL           	DMA1_Channel3		//DMA通道号
//#define USART3_RX_DMA_STREAM           		DMA1_Stream1		//DMA数据流
/********USART3_DMA_define_END********/


extern char USART1_buff[USART2_BUFFLEN];
extern uint8_t USART1_Pointer;

void USART1_QuickInit(uint32_t USART_BaudRate);
//void USART1_RXDMA_Config(uint32_t USART1_DMABuff_addr, uint32_t buffsize);


void USART2_QuickInit(uint32_t USART_BaudRate);
//void USART2_RXDMA_Config(uint32_t USART2_DMABuff_addr, uint32_t buffsize);

void USART3_QuickInit(uint32_t USART_BaudRate);
//void USART3_RXDMA_Config(uint32_t USART3_DMABuff_addr, uint32_t buffsize);


void USART_setPort(USART_TypeDef* USARTx);

void USART_sendChar(USART_TypeDef* USARTx, char ch);
	

#endif /* __USART_H */
