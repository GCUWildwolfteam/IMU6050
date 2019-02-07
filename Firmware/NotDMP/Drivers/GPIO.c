/**
  ******************************************************************************
  * @file    GPIO.c
  * @author  Hare
  * @version V1.0
  * @date    
  * @brief   GPIO配置函数接口
  ******************************************************************************
  */
  
  
#include "GPIO.h"
/*
	GPIO_Mode_AIN
  GPIO_Mode_IN_FLOATING
  GPIO_Mode_IPD
  GPIO_Mode_IPU
  GPIO_Mode_Out_OD
  GPIO_Mode_Out_PP
  GPIO_Mode_AF_OD
  GPIO_Mode_AF_PP
*/

/**
  * @brief  GPIO快速初始化
  * @param  RCC_APB2Periph 	GPIO时钟 	RCC_APB2Periph_GPIOA~K
  *			GPIOx			GPIO端口	GPIOA~K
  *			GPIO_Pin		GPIO引脚	GPIO_Pin_0~15
  *			GPIO_Mode		配置GPIO的模式 @ref GPIOMode_TypeDef
  * @retval None
  */
void GPIO_QuickInit(uint32_t RCC_APB2Periph, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
	
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}

/**
  * @brief  Toggles the specified GPIO pins..
  * @param  GPIOx: where x can be (A..K) to select the GPIO peripheral for STM32F405xx/407xx and STM32F415xx/417xx devices
  *                      x can be (A..I) to select the GPIO peripheral for STM32F42xxx/43xxx devices.
  *                      x can be (A, B, C, D and H) to select the GPIO peripheral for STM32F401xx devices. 
  * @param  GPIO_Pin: Specifies the pins to be toggled.
  * @retval None
  */
void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

  GPIOx->ODR ^= GPIO_Pin;
}

