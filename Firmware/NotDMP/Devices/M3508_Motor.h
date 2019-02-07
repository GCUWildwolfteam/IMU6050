#ifndef __M3508_MOTOR_H
#define __M3508_MOTOR_H

#include "user_common.h"

#define M3508_READID_START	0x201
#define M3508_READID_END	0x204
#define M3508_SENDID		0x200


extern M3508s_t M3508s[4];


void M3508_setCurrent(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4);
void M3508_getInfo(CanRxMsg RxMessage);
void M3508_setTargetSpeed(M3508Name_e motorName, int16_t speed);


#endif /* __M3508_MOTOR_H */
