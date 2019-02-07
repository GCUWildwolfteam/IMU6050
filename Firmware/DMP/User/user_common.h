#ifndef __USER_COMMON_H
#define __USER_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

///***---FreeRTOS_BEGIN---***/
//#include "FreeRTOS.h"
//#include "task.h"
//#include "queue.h"
//#include "croutine.h"
///***---FreeRTOS_END---***/


#include "stm32f10x.h"


#include "binary.h"
#include "UserMath.h"
#include "typedefs.h"

/***---Drivers_Begin---***/
#include "delay.h"
#include "NVIC.h"
#include "GPIO.h"
#include "USART.h"
//#include "I2C_SW.h"
//#include "SPI_HW.h"
#include "CAN.h"
#include "Flash.h"
#include "TIM.h"
//#include "bsp_dwt.h"
/***---Drivers_End---***/

/***---Devices_Begin---***/
//#include "LED.h"
#include "key.h"
//#include "Laser.h"
#include "M3508_Motor.h"
#include "IDData.h"
//#include "M6623_Motor.h"
//#include "M2006_Motor.h"
//#include "frictionMotor.h"
//#include "DR16_Receiver.h"
//#include "UWB.h"
//#include "MC01_JudgeSystem.h"
//#include "imu.h"
//#include "MahonyAHRS.h"
#include "gasSwitch.h"
//#include "Ultrasonic.h"
//#include "GY_IMU.h"
//#include "DJI_BSP.h"
//#include "MPU6050.h"
//#include "ak8963.h"

/***---Devices_End---***/

/***---Apps_Begin---***/
#include "Filter.h"
//#include "PID.h"
//#include "Control_param.h"
//#include "Rocker.h"
//#include "Chassis_control.h"
//#include "Cloud_control.h"
//#include "BigSymbol.h"
//#include "DevicesMonitor.h"
//#include "Shoot.h"
/***---Apps_End---***/

/***---Tasks_Begin---***/
//#include "SystemInit_Tasks.h"
//#include "LED_Task.h"
/***---Tasks_End---***/


#endif /*__USER_COMMON_H */
