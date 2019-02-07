#ifndef __IDDATA_H
#define __IDDATA_H

#include "user_common.h"

#define GasData_READID_START	0x500
#define GasData_READID_END		0x500
#define GasData_SENDID			0x501

extern uint8_t id;

//void PowerData_send(void);
void GasData_getInfo(CanRxMsg RxMessage);

void IDData_setID(uint8_t id);
uint8_t IDData_getID(void);

#endif /* __IDDATA_H */
