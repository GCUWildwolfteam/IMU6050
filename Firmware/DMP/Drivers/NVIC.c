/**
  ******************************************************************************
  * @file    NVIC.c
  * @author  Hare
  * @version V1.0
  * @date    
  * @brief   NVIC中断优先级应用函数接口
  ******************************************************************************
  */

#include "NVIC.h"


/**
  * @brief  NVIC中断优先级配置
  * @param  IRQChannel 	  		中断源，参考"stm32f4xx.h", 196行、（中断服务函数名在.s文件里）
  *			MainPriority		主优先级
  *			SubPriority			抢占式优先级
  * @retval None
  */
void NVIC_Config(uint16_t IRQChannel, uint16_t MainPriority, uint16_t SubPriority){
	NVIC_InitTypeDef NVIC_InitStructure;

	//设置中断来源
	NVIC_InitStructure.NVIC_IRQChannel = IRQChannel;
	//设置主优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = MainPriority;
	//设置抢占式优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
