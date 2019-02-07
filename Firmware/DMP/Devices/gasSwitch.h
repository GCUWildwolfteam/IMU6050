#ifndef __GASSWITCH_H
#define __GASSWITCH_H

#include "user_common.h"


#define GAS_COUNT 8

extern GPIOPins_t Gass_GPIO[GAS_COUNT];


#define GAS1_GPIO_CLK             	RCC_APB2Periph_GPIOA
#define GAS1_GPIO_PORT           	GPIOA
#define GAS1_PIN                  	GPIO_Pin_15

#define GAS2_GPIO_CLK             	RCC_APB2Periph_GPIOB
#define GAS2_GPIO_PORT           	GPIOB
#define GAS2_PIN                  	GPIO_Pin_3

#define GAS3_GPIO_CLK             	RCC_APB2Periph_GPIOB
#define GAS3_GPIO_PORT           	GPIOB
#define GAS3_PIN                  	GPIO_Pin_4

#define GAS4_GPIO_CLK             	RCC_APB2Periph_GPIOB
#define GAS4_GPIO_PORT           	GPIOB
#define GAS4_PIN                  	GPIO_Pin_5

#define GAS5_GPIO_CLK             	RCC_APB2Periph_GPIOB
#define GAS5_GPIO_PORT           	GPIOB
#define GAS5_PIN                  	GPIO_Pin_6

#define GAS6_GPIO_CLK             	RCC_APB2Periph_GPIOB
#define GAS6_GPIO_PORT           	GPIOB
#define GAS6_PIN                  	GPIO_Pin_7

#define GAS7_GPIO_CLK             	RCC_APB2Periph_GPIOB
#define GAS7_GPIO_PORT           	GPIOB
#define GAS7_PIN                  	GPIO_Pin_8

#define GAS8_GPIO_CLK             	RCC_APB2Periph_GPIOB
#define GAS8_GPIO_PORT           	GPIOB
#define GAS8_PIN                  	GPIO_Pin_9

void gasSwitch_Init(void);
void gasSwitch_AllOFF(void);

void gasSwitch_writeData(uint8_t data);

#endif /* __GASSWITCH_H */
