#ifndef __KEY_H
#define __KEY_H

#include "user_common.h"

#define	KEY_ON	 1		//按键按下
#define	KEY_OFF	 0		//按键放开

#define KEY_COUNT 8

extern uint8_t keyCode;


#define KEY1_GPIO_CLK             	RCC_APB2Periph_GPIOA
#define KEY1_GPIO_PORT           	GPIOA
#define KEY1_PIN                  	GPIO_Pin_3

#define KEY2_GPIO_CLK             	RCC_APB2Periph_GPIOA
#define KEY2_GPIO_PORT           	GPIOA
#define KEY2_PIN                  	GPIO_Pin_4

#define KEY3_GPIO_CLK             	RCC_APB2Periph_GPIOA
#define KEY3_GPIO_PORT           	GPIOA
#define KEY3_PIN                  	GPIO_Pin_5

#define KEY4_GPIO_CLK             	RCC_APB2Periph_GPIOA
#define KEY4_GPIO_PORT           	GPIOA
#define KEY4_PIN                  	GPIO_Pin_6

#define KEY5_GPIO_CLK             	RCC_APB2Periph_GPIOA
#define KEY5_GPIO_PORT           	GPIOA
#define KEY5_PIN                  	GPIO_Pin_7

#define KEY6_GPIO_CLK             	RCC_APB2Periph_GPIOB
#define KEY6_GPIO_PORT           	GPIOB
#define KEY6_PIN                  	GPIO_Pin_0

#define KEY7_GPIO_CLK             	RCC_APB2Periph_GPIOB
#define KEY7_GPIO_PORT           	GPIOB
#define KEY7_PIN                  	GPIO_Pin_1

#define KEY8_GPIO_CLK             	RCC_APB2Periph_GPIOB
#define KEY8_GPIO_PORT           	GPIOB
#define KEY8_PIN                  	GPIO_Pin_2




extern key_t userKeyList[KEY_COUNT];

void KEY_Init(void);
void Key_Process(void);

//extern TaskHandle_t xHandleTaskKey;
//void vTaskKey(void *pvParameters);

#endif /* __KEY_H */


