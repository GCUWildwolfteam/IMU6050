#ifndef __POWER_H
#define __POWER_H

#include "include.h"

#define Reference_voltage 3300

typedef enum
{
	Interrupt,
	Envent
} wakeUpForm_m;



extern int times;

void Power_SleepMode(wakeUpForm_m Form);
void Power_StopMode(void);
void Power_StandbyMode(FunctionalState PA0_config);

void Power_Manage(void);

int BatteryUpdate(void);





#endif

