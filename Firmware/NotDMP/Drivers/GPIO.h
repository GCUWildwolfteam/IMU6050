#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f10x.h"


void GPIO_QuickInit(uint32_t RCC_APB2Periph, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode);

void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif /*__GPIO_H*/
