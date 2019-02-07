
#ifndef __ADC_H
#define __ADC_H

#include "user_common.h"

//要转换的通道数
#define 		 ADC_NunOfCH									2

extern __IO uint16_t ADC_Value[ADC_NunOfCH];

void ADC1_Init(void);
float ADCtoVol(float ADC_Value);

#endif	/*__ADC_H*/
