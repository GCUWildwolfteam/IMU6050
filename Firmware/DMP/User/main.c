/*

*/

#include "user_common.h"
#include "I2C_SW.h"
#include "LED.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

RCC_ClocksTypeDef RCC_Clocks;

typedef struct{
  eular_t eular;
  vector16_t gyro;
  vector16_t accel;
  float temperature;
  
}MPU_DMP_Data_t;

MPU_DMP_Data_t MPU_DMP_Data;


int main(void){
	
	/* 优先级分组4 （抢占式16）使用os不要分组 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  /* SYSTICK初始化 */
  SysTick_Config(SystemCoreClock/1000);
	
	//RCC_GetClocksFreq(&RCC_Clocks);
  
	USART1_QuickInit(115200);
	CAN1_QuickInit();
  I2C_SW_Init();
  LED_Init();
  
  LED_RED(1);

  while(MPU_Init()){
    /* MPU硬件错误/ID错误,LED快闪 */
    LED_RED_TOGGLE();
    delay_ms(100);
  }
  
  while(mpu_dmp_init()){
    /* MPU DMP启动错误，lED慢闪 */
    LED_RED_TOGGLE();
    delay_ms(300);
  }
	LED_RED(0);
	
	
	//TIM4_Counter_Config(71, 9999);	//10ms定时
	
	while(1) {
    if(mpu_dmp_get_data(&MPU_DMP_Data.eular.pitch,&MPU_DMP_Data.eular.roll,&MPU_DMP_Data.eular.yaw)==0)
		{ 
			MPU_DMP_Data.temperature=MPU_Get_Temperature();	//得到温度值
			MPU_Get_Accelerometer(&MPU_DMP_Data.accel.x,&MPU_DMP_Data.accel.y,&MPU_DMP_Data.accel.z);	//得到加速度传感器数据
			MPU_Get_Gyroscope(&MPU_DMP_Data.gyro.x,&MPU_DMP_Data.gyro.y,&MPU_DMP_Data.gyro.z);	//得到陀螺仪数据
      LED_GREEN_TOGGLE();
    }
		
//		delay_ms(100);
//		M3508_setCurrent(0,0,0,400);
    
	}
	
	return 0;
}
