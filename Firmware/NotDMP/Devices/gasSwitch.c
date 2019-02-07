/**
  ******************************************************************************
  * @file    gasSwitch.c
  * @author  Hare
  * @version V1.0
  * @date    
  * @brief   气阀应用函数接口
  ******************************************************************************
  */
  
  
#include "gasSwitch.h"


GPIOPins_t Gass_GPIO[GAS_COUNT] = {
	{GAS1_GPIO_CLK, GAS1_GPIO_PORT, GAS1_PIN},
	{GAS2_GPIO_CLK, GAS2_GPIO_PORT, GAS2_PIN},
	{GAS3_GPIO_CLK, GAS3_GPIO_PORT, GAS3_PIN},
	{GAS4_GPIO_CLK, GAS4_GPIO_PORT, GAS4_PIN},
	{GAS5_GPIO_CLK, GAS5_GPIO_PORT, GAS5_PIN},
	{GAS6_GPIO_CLK, GAS6_GPIO_PORT, GAS6_PIN},
	{GAS7_GPIO_CLK, GAS7_GPIO_PORT, GAS7_PIN},
	{GAS8_GPIO_CLK, GAS8_GPIO_PORT, GAS8_PIN},
};

/**
  * @brief  气阀控制初始化
  * @param  None
  * @retval None
  */
void gasSwitch_Init(void){
	for(int i = 0; i < GAS_COUNT; i++){
		GPIO_QuickInit(Gass_GPIO[i].GPIO_CLK, Gass_GPIO[i].GPIOx, Gass_GPIO[i].GPIO_Pin, GPIO_Mode_Out_PP);
	}
	gasSwitch_AllOFF();
}

void gasSwitch_AllOFF(void){
	for(int i = 0; i < GAS_COUNT; i++){
		GPIO_ResetBits(Gass_GPIO[i].GPIOx, Gass_GPIO[i].GPIO_Pin);
	}
}

void gasSwitch_writeData(uint8_t data){
	for(int i = 0; i < GAS_COUNT; i++){
		GPIO_WriteBit(Gass_GPIO[i].GPIOx, Gass_GPIO[i].GPIO_Pin, (BitAction)((data>>i)&0x01));
	}
	
}

