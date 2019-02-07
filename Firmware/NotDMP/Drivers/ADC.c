/*


*/

#include "ADC.h"


__IO uint16_t ADC_Value[ADC_NunOfCH];

static void ADC1_GPIO_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//使能GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//配置GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}

static void ADC1_Mode_Config(void){
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	//配置ADC初始化结构体
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = ADC_NunOfCH;
	
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//配置ADC时钟分频	RCC_PCLK2_Div2/4/6/8分频
	RCC_ADCCLKConfig(RCC_PCLK2_Div2);
	//配置规则序列ADC通道，转换顺序和采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 2, ADC_SampleTime_55Cycles5);

	//使能ADC
	ADC_Cmd(ADC1, ENABLE);
	
	//复位校准ACD
	ADC_ResetCalibration(ADC1);
	//等待复位完成
	while(ADC_GetResetCalibrationStatus(ADC1));
	//校准ACD
	ADC_StartCalibration(ADC1);
	//等待校准完成
	while(ADC_GetCalibrationStatus(ADC1));
	
	//启用软件触发ADC转换
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
}



static void ADC1_DMAConfig(void){
	DMA_InitTypeDef DMA_InitStructure;
	//开启DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	//复位ADC通道的DMA
	DMA_DeInit(DMA1_Channel1);
	//设定外设（源）的地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));
	//设定目标变量的地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_Value[0];
	//设置传输方向，外设做源
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	//设置要传输的数据个数
	DMA_InitStructure.DMA_BufferSize = ADC_NunOfCH;
	//设置外设地址是否自增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//设置变量地址是否自增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	//设置外设数据宽度
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	//设置变量数据宽度
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	//配置DMA传输模式，单次或循环
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
	//配置通道优先级
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	//配置是否开启存储器到存储器模式
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	//初始化结构体
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	//使能DMA
	DMA_Cmd(DMA1_Channel1 , ENABLE);
	
	// 使能DMA 请求
	ADC_DMACmd(ADC1, ENABLE);
	
}

void ADC1_Init(void){
	//配置GPIO
	ADC1_GPIO_Config();
	//配置ADC模式
	ADC1_Mode_Config();
	//配置ADC	DMA
	ADC1_DMAConfig();

}

float ADCtoVol(float ADC_Value){
	return ((float)ADC_Value*3.30/0xfff);
}

