#ifndef __RTC_H
#define __RTC_H

#include "include.h"

#define HH 23
#define MM 59
#define SS 50

#define RTC_CLK 		LSE
//#define RTC_CLK 		LSI

#define RTC_Prescaler  (32768-1)
//#define RTC_Prescaler  (40000-1)

void RTC_Config(void);
void RTC_ToTime(u8 *hour, u8 *minute, u8 *second);












#endif /*__RTC_H*/

