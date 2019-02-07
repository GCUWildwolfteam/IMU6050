#ifndef __HW_SPI_H
#define __HW_SPI_H

#include "include.h"

#define Open_SPI1
//#define Open_SPI2

#ifdef Open_SPI1

#define Using_SPI						SPI1

/*SPI Clock*/
#define      SPI_APBxClock_Cmd      		RCC_APB2PeriphClockCmd
#define      SPI_CLK                		RCC_APB2Periph_SPI1
#define      SPI_GPIO_APBxClock_Cmd   	RCC_APB2PeriphClockCmd

//CS	PA4
//#define      SPI_CS_CLK             		RCC_APB2Periph_GPIOA    
//#define      SPI_CS_PORT           		 	GPIOA
//#define      SPI_CS_PIN         	  		GPIO_Pin_4

//SCK	PA5
#define      SPI_SCK_CLK            		RCC_APB2Periph_GPIOA   
#define      SPI_SCK_PORT          			GPIOA   
#define      SPI_SCK_PIN           			GPIO_Pin_5

//MISO	PA6
#define      SPI_MISO_CLK                RCC_APB2Periph_GPIOA    
#define      SPI_MISO_PORT               GPIOA 
#define      SPI_MISO_PIN                GPIO_Pin_6

//MOSI	PA7
#define      SPI_MOSI_CLK                RCC_APB2Periph_GPIOA    
#define      SPI_MOSI_PORT               GPIOA 
#define      SPI_MOSI_PIN                GPIO_Pin_7

#endif

/*************************************************************************************/

#ifdef Open_SPI2

#define Using_SPI						SPI2

/*SPI Clock*/
#define      SPI_APBxClock_Cmd      		RCC_APB1PeriphClockCmd
#define      SPI_CLK                		RCC_APB1Periph_SPI2
#define      SPI_GPIO_APBxClock_Cmd   	RCC_APB2PeriphClockCmd

//CS	PB12
//#define      SPI_CS_CLK             		RCC_APB2Periph_GPIOB    
//#define      SPI_CS_PORT           		 	GPIOB
//#define      SPI_CS_PIN         	  		GPIO_Pin_12

//SCK	PB13
#define      SPI_SCK_CLK            		RCC_APB2Periph_GPIOB   
#define      SPI_SCK_PORT          			GPIOB   
#define      SPI_SCK_PIN           			GPIO_Pin_13

//MISO	PB14
#define      SPI_MISO_CLK                RCC_APB2Periph_GPIOB    
#define      SPI_MISO_PORT               GPIOB 
#define      SPI_MISO_PIN                GPIO_Pin_14

//MOSI	PB15
#define      SPI_MOSI_CLK                RCC_APB2Periph_GPIOB    
#define      SPI_MOSI_PORT               GPIOB 
#define      SPI_MOSI_PIN                GPIO_Pin_15

#endif


#define Dummy_Byte                0xFF

#define SPI_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPI_LONG_TIMEOUT         ((uint32_t)(10 * SPI_FLAG_TIMEOUT))


void SPIx_Init(void);
u8 SPI_SendByte(u8 byte);
u16 SPI_FLASH_SendHalfWord(u16 HalfWord);
u8 SPI_ReadByte(void);


#endif  /*__HW_SPI_H*/
