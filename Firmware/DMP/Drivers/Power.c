 /*



*/

#include "Power.h"

int times = 0;


/* ��ֹͣģʽ���Ѻ�ָ�HSE��Ϊϵͳʱ�� */
static void HSE_Rec(void){
  /* ʹ�� HSE ʱ�� */
  RCC_HSEConfig(RCC_HSE_ON);
  /* �ȴ� HSE ʱ��׼������ */
  while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
  /* ʹ�� PLL */ 
  RCC_PLLCmd(ENABLE);
  /* �ȴ� PLL ׼������ */
  while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
  /* ѡ��PLL��Ϊϵͳʱ�� */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  /* �ȴ�ϵͳʱ��׼������ */
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

/* ֹͣģʽ */
void Power_StopMode(void){
	//����ֹͣģʽ
	PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI); 
	//���Ѻ�����￪ʼ����ִ��
	//�ָ�HSEʱ��
	HSE_Rec();
}

/* ����ģʽ */
void Power_StandbyMode(FunctionalState PA0_config){
	//�������ģʽ���뿪��PWRʱ�ӣ������޷��������ģʽ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	//�Ƿ���PA0Ϊ���Ѷ˿�
	PWR_WakeUpPinCmd(PA0_config);
	//�������ģʽ
	PWR_EnterSTANDBYMode();
}



