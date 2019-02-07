/*


*/

#include "RTC.h" 

void RTC_Config(void){
	//使能电源控制和备份域时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	//允许备份域访问
	PWR_BackupAccessCmd(ENABLE);
	//复位备份域寄存器
	BKP_DeInit();
	
	/* 设定RTC时钟 */
	#if (RTC_CLK == LSE)
	//开启外部低速时钟LSE
	RCC_LSEConfig(RCC_LSE_ON);
	//等待LSE时钟稳定
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	//将LSE时钟配置为RTC的时钟
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	#endif
	
	/* 设定RTC时钟 */
	#if (RTC_CLK == LSI)
	//使能内部低速时钟LSI
	RCC_LSICmd(ENABLE);
	//等待LSI稳定
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	//将LSI配置为RTC的时钟
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	#endif
	
	//使能RTC时钟
	RCC_RTCCLKCmd(ENABLE);
	
	//等待内环时钟与RTC时钟同步
	RTC_WaitForSynchro();
	//等待操作完成
	RTC_WaitForLastTask();
	
	// 使能RTC秒中断
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	//等待操作完成
	RTC_WaitForLastTask();
	
	//设置RTC时钟分频
	RTC_SetPrescaler(RTC_Prescaler); /* 每秒中断一次，即时设置为，RTC时钟频率-1 */
	//等待操作完成
	RTC_WaitForLastTask();
	
	//设置RTC时间
	RTC_SetCounter(HH*3600+MM*60+SS);
	//等待操作完成
	RTC_WaitForLastTask();
	
	//配置中断优先级
	NVIC_Config(RTC_IRQn, 2, 2);
	
}

u8 Time_Update = 1;

void RTC_ToTime(u8 *hour, u8 *minute, u8 *second){
	if(Time_Update){
		u32 tm;
		tm = RTC_GetCounter();
		*hour = tm / 3600;
		*minute = (tm %3600)/60;
		*second = (tm%3600)%60;
		RTC_WaitForLastTask();
		Time_Update = 0;
	}
	
}

