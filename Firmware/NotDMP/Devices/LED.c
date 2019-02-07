/**
  ******************************************************************************
  * @file    LED.c
  * @author  Hare
  * @version V1.0
  * @date    
  * @brief   LED灯应用函数接口
  ******************************************************************************
  */
  
  
#include "LED.h"


/**
  * @brief  LED灯初始化
  * @param  None
  * @retval None
  */
void LED_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(LED_GREEN_GPIO_CLK | LED_RED_GPIO_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = LED_RED_PIN;
	GPIO_Init(LED_RED_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = LED_GREEN_PIN;
	GPIO_Init(LED_GREEN_GPIO_PORT, &GPIO_InitStructure);
	
	LED_GREEN(0);
  LED_RED(0);
}


