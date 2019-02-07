#ifndef __LED_H
#define __LED_H

#include "user_common.h"


#define LED_RED_GPIO_CLK             	RCC_APB2Periph_GPIOA
#define LED_RED_GPIO_PORT           	GPIOA
#define LED_RED_PIN                  	GPIO_Pin_6

#define LED_GREEN_GPIO_CLK            RCC_APB2Periph_GPIOA
#define LED_GREEN_GPIO_PORT           GPIOA
#define LED_GREEN_PIN                 GPIO_Pin_7

#define LED_RED(isOn) 				  GPIO_WriteBit(LED_RED_GPIO_PORT,LED_RED_PIN, (BitAction)!isOn)
#define LED_RED_TOGGLE() 				GPIO_ToggleBits(LED_RED_GPIO_PORT, LED_RED_PIN)


#define LED_GREEN(isOn) 				GPIO_WriteBit(LED_GREEN_GPIO_PORT,LED_GREEN_PIN, (BitAction)!isOn)
#define LED_GREEN_TOGGLE() 			GPIO_ToggleBits(LED_GREEN_GPIO_PORT, LED_GREEN_PIN)


void LED_Init(void);

#endif /* __LED_H */
