/*


*/

#include "TIM.h"


void TIM2_PWM_Config(u16 prescaler, u16 period, u16 Pulse){
	//使能定时器时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  
	/* 配置时基结构体 */
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//配置定时器时钟分频，72分频后定时器时钟为1M
	TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
  //配置自动重装载寄存器的值,1M的时钟，计数器计100次，即产生PWM的频率为(72M/prescaler)/period = ???K
	TIM_TimeBaseStructure.TIM_Period = period;
	//配置死区时间与采样滤波分频，不分频
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//配置计数模式，向上计数
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
  /*配置输出比较结构体*/
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	//配置输出模式，PWM1模式
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //PWM模式1
  //使能输出
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  //设置初始占空比
	TIM_OCInitStructure.TIM_Pulse = Pulse;
	//设置PWM极性，初始电平为高
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  //初始化为对应通道的输出，通道1
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	
  //使能对应通道重装载
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	//使能定时器
	TIM_Cmd(TIM2, ENABLE);
	
}

void TIM2_Encoder_Config(u16 prescaler, u16 period){
	//使能定时器时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
	/* 配置时基结构体 */
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//配置定时器时钟分频，72分频后定时器时钟为1M
	TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
  //配置自动重装载寄存器的值,1M的时钟，计数器计100次，即产生PWM的频率为(72M/prescaler)/period = ???K
	TIM_TimeBaseStructure.TIM_Period = period;
	//配置死区时间与采样滤波分频，不分频
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//配置计数模式，向上计数
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	//启用编码器模式
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	//配置输入捕获寄存器
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);

	//TIM_ARRPreloadConfig(TIM2, ENABLE);
	//使能定时器
	TIM_Cmd(TIM2, ENABLE);
	
}

void TIM4_Counter_Config(u16 prescaler, u16 period){
	//使能定时器时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	/* 配置时基结构体 */
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//配置定时器时钟分频，72分频后定时器时钟为1M
	TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
	//配置自动重装载寄存器的值,1M的时钟，计数器计100次，即产生PWM的频率为(72M/prescaler)/period = ???K
	TIM_TimeBaseStructure.TIM_Period = period;
	//配置死区时间与采样滤波分频，不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//配置计数模式，向上计数
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	//启用编码器模式
	
	
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	//使能定时器
	TIM_Cmd(TIM4, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	NVIC_Config(TIM4_IRQn, 2, 0);
	
}

