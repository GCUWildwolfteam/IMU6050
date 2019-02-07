#ifndef _TIM_H
#define _TIM_H

#include "user_common.h"

void TIM2_PWM_Config(u16 prescaler, u16 period, u16 Pulse);
void TIM2_Encoder_Config(u16 prescaler, u16 period);
void TIM4_Counter_Config(u16 prescaler, u16 period);

//void TIM_Init(void);

#endif
