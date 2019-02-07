/**
  ******************************************************************************
  * @file    Filter.c
  * @author  Hare
  * @version V1.0
  * @date    
  * @brief   滤波器函数接口
  ******************************************************************************
  */
  
  
#include "Filter.h"


#define SENSOR9_UPDATE_RATE   	50

/**
 * Set ACC_WANTED_LPF1_CUTOFF_HZ to the wanted cut-off freq in Hz.
 * The highest cut-off freq that will have any affect is fs /(2*pi).
 * E.g. fs = 350 Hz -> highest cut-off = 350/(2*pi) = 55.7 Hz -> 55 Hz
 */
#define SENSORS_ACC_WANTED_LPF_CUTOFF_HZ  4
/**
 * Attenuation should be between 1 to 256.
 *
 * f0 = fs / 2*pi*attenuation ->
 * attenuation = fs / 2*pi*f0
 */
#define SENSORS_ACC_IIR_LPF_ATTENUATION (SENSOR9_UPDATE_RATE / (2 * 3.1415 * SENSORS_ACC_WANTED_LPF_CUTOFF_HZ))
#define SENSORS_ACC_IIR_LPF_ATT_FACTOR  (int)(((1<<IIR_SHIFT) / SENSORS_ACC_IIR_LPF_ATTENUATION) + 0.5)
#define SENSORS_ACC_IIR_FACTOR		(float)(1/(1+SENSORS_ACC_IIR_LPF_ATTENUATION))
	

void sensorsAccIIRLPFilter(vector_t *in,vector_t *out)
{
	out->x = out->x + SENSORS_ACC_IIR_FACTOR*(in->x - out->x); 
	out->y = out->y + SENSORS_ACC_IIR_FACTOR*(in->y - out->y); 
	out->z = out->z + SENSORS_ACC_IIR_FACTOR*(in->z - out->z); 
}

/**
  * @brief  IIR低通滤波，输入与输出不能是同一个变量
  * @param[in]  *in 输入数据
  *				 LpfAttFactor 低通滤波衰减因子 Attenuation should be between 1 to 256.
  * @param[out]	*out 数据输出
  * @retval None
  */
void Filter_IIRLPF(float *in,float *out, float LpfAttFactor)
{
	*out = *out + LpfAttFactor*(*in - *out); 
}
