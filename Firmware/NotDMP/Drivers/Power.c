 /*



*/

#include "Power.h"

int times = 0;


/* 从停止模式唤醒后恢复HSE作为系统时钟 */
static void HSE_Rec(void){
  /* 使能 HSE 时钟 */
  RCC_HSEConfig(RCC_HSE_ON);
  /* 等待 HSE 时钟准备就绪 */
  while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
  /* 使能 PLL */ 
  RCC_PLLCmd(ENABLE);
  /* 等待 PLL 准备就绪 */
  while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
  /* 选择PLL作为系统时钟 */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  /* 等待系统时钟准备就绪 */
  while (RCC_GetSYSCLKSource() != 0x08);
}

void Power_SleepMode(wakeUpForm_m Form){
	if(Form == Interrupt){
		__WFI();
	}
	else{
		__WFE();
	}
	
}

/* 停止模式 */
void Power_StopMode(void){
	//进入停止模式
	PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI); 
	//唤醒后从这里开始继续执行
	//恢复HSE时钟
	HSE_Rec();
}

/* 待机模式 */
void Power_StandbyMode(FunctionalState PA0_config){
	//进入待机模式必须开启PWR时钟，否则无法进入待机模式
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	//是否开启PA0为唤醒端口
	PWR_WakeUpPinCmd(PA0_config);
	//进入待机模式
	PWR_EnterSTANDBYMode();
}



