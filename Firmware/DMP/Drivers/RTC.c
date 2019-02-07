/*


*/

#include "RTC.h" 

void RTC_Config(void){
	//ʹ�ܵ�Դ���ƺͱ�����ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	//�����������
	PWR_BackupAccessCmd(ENABLE);
	//��λ������Ĵ���
	BKP_DeInit();
	
	/* �趨RTCʱ�� */
	#if (RTC_CLK == LSE)
	//�����ⲿ����ʱ��LSE
	RCC_LSEConfig(RCC_LSE_ON);
	//�ȴ�LSEʱ���ȶ�
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	//��LSEʱ������ΪRTC��ʱ��
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	#endif
	
	/* �趨RTCʱ�� */
	#if (RTC_CLK == LSI)
	//ʹ���ڲ�����ʱ��LSI
	RCC_LSICmd(ENABLE);
	//�ȴ�LSI�ȶ�
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	//��LSI����ΪRTC��ʱ��
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	#endif
	
	//ʹ��RTCʱ��
	RCC_RTCCLKCmd(ENABLE);
	
	//�ȴ��ڻ�ʱ����RTCʱ��ͬ��
	RTC_WaitForSynchro();
	//�ȴ��������
	RTC_WaitForLastTask();
	
	// ʹ��RTC���ж�
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	//�ȴ��������
	RTC_WaitForLastTask();
	
	//����RTCʱ�ӷ�Ƶ
	RTC_SetPrescaler(RTC_Prescaler); /* ÿ���ж�һ�Σ���ʱ����Ϊ��RTCʱ��Ƶ��-1 */
	//�ȴ��������
	RTC_WaitForLastTask();
	
	//����RTCʱ��
	RTC_SetCounter(HH*3600+MM*60+SS);
	//�ȴ��������
	RTC_WaitForLastTask();
	
	//�����ж����ȼ�
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

