/**
  ******************************************************************************
  * @file     delay.c
  * @author   Hare
  * @version  V1.3
  * @date     2019-01-14
  * @brief    延时应用函数接口，OS可用
  * @Note     1.如果启用OS，则Systick中断服务函数建议写成如下形式（以FreeRTOS为例）
  *             在任务开始调度后再调用系统的SystickHandle
  *             否则如果在系统开始调度前如果使用了delay会使程序卡在中断里
  *             void SysTick_Handler(void)
  *             {
	*               if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)
  *               {
  *                 xPortSysTickHandler();	
  *               }
  *             }
  *           2.记得在main中初始化Systick才可正常使用delay
  *             SysTick_Config(SystemCoreClock/1000);
  ******************************************************************************
  */
  
  
#include "delay.h"

//uint32_t sysTickCnt = 0;

/**
  * @brief  微秒级延时,参考安富莱DWT，延时数值较小时会有0.25us左右误差
  * @Note 	两个32位无符号数相减，获取的结果再赋值给32位无符号数依然可以正确的获取差值。
  *			假如A,B,C都是32位无符号数。
  *              如果A > B  那么A - B = C，这个很好理解，完全没有问题
  *              如果A < B  那么A - B = C， C的数值就是0xFFFFFFFF - B + A + 1。
  * @param  _us 延时微秒数，32位计数器计满是2^32/SystemCoreClock秒
  * @retval None
  */
void delay_us(uint32_t _us)
{
	uint32_t tCnt, tDelayCnt;
	uint32_t tStart;
	
	tStart = SysTick->VAL;
	tCnt = 0;
	tDelayCnt = _us * (SystemCoreClock / 1000000);	 /* 需要的节拍数 */ 
	
	while(tCnt < tDelayCnt)
	{
		tCnt = tStart - SysTick->VAL; /* 求减过程中，如果发生第一次32位计数器重新计数，依然可以正确计算 */	
	}
	
}

/**
  * @brief  毫秒级延时
  * @param  _ms 延时毫秒数
  * @retval None
  */
void delay_ms(uint32_t _ms)
{
	uint32_t i;
	for(i=0;i<_ms;i++){
		delay_us(1000);
	}
}


