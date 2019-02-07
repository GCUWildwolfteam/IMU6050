/**
  ******************************************************************************
  * @file    IDData.c
  * @author  Hare
  * @version V1.0
  * @date    
  * @brief   ID信息函数接口
  ******************************************************************************
  */
  
  
#include "IDData.h"


uint8_t id = 0;

/**
  * @brief  从CAN报文中获取开关信息
  * @param  *RxMessage 	CAN报文接收结构体指针
  * @retval None
  */
void GasData_getInfo(CanRxMsg RxMessage){
	//报文id确认
	if((RxMessage.StdId < GasData_READID_START) || (RxMessage.StdId > GasData_READID_END))
		return;
	gasSwitch_writeData(RxMessage.Data[id]);
//	uint32_t StdId;
//	StdId = RxMessage.StdId - PowerData_READID_START;
	
}

void IDData_setID(uint8_t id){
	FLASH_Erase(62);
	Flash_Write16(62, 0, id);
}

uint8_t IDData_getID(void){
	uint16_t data;
	Flash_Read16(62, 0, &data);
	return data;
}




