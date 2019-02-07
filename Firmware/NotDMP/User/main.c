/*

*/

#include "user_common.h"
#include "I2C_SW.h"
#include "LED.h"
#include "sensors.h"
//#include "inv_mpu.h"
//#include "inv_mpu_dmp_motion_driver.h" 

RCC_ClocksTypeDef RCC_Clocks;


void systemInit(void)
{
  USART1_QuickInit(115200);
	CAN1_QuickInit();
  I2C_SW_Init();
  LED_Init();
  
}

TaskHandle_t startTaskHandle;
static void startTask(void *arg);


void startTask(void *arg)
{
	taskENTER_CRITICAL();	/*进入临界区*/
	
	xTaskCreate(sensorsTask, "SENSORS", 450, NULL, 4, NULL);			/*创建传感器处理任务*/
	
	//xTaskCreate(stabilizerTask, "STABILIZER", 450, NULL, 5, NULL);		/*创建姿态任务*/
	
	vTaskDelete(startTaskHandle);										/*删除开始任务*/
		
	taskEXIT_CRITICAL();	/*退出临界区*/
} 

int main(void){
	
	/* 优先级分组4 （抢占式16）使用os不要分组 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  /* SYSTICK初始化 */
  SysTick_Config(SystemCoreClock/1000);
	
	//RCC_GetClocksFreq(&RCC_Clocks);
  
	
  
  systemInit();			/*底层硬件初始化*/	

	xTaskCreate(startTask, "START_TASK", 300, NULL, 2, &startTaskHandle);	/*创建起始任务*/

	vTaskStartScheduler();	/*开启任务调度*/
	
	while(1) 
  {
	}
	
	return 0;
}
