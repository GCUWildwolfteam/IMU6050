/**
  ******************************************************************************
  * @file    M3508_Motor.c
  * @author  Hare
  * @version V1.0
  * @date    
  * @brief   M3508无刷电机，配套C620电调驱动应用函数接口
  ******************************************************************************
  */
  
  
#include "M3508_Motor.h"

M3508s_t M3508s[4];


/**
  * @brief  设置M3508电机电流值（id号为1~4）
  * @param  iqx (x:1~4) 对应id号电机的电流值，范围-16384~0~16384
  * @retval None
  */
void M3508_setCurrent(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4){

	uint8_t data[8];
	
	//数据格式详见说明书P32
	data[0] = iq1 >> 8;
	data[1] = iq1;
	data[2] = iq2 >> 8;
	data[3] = iq2;
	data[4] = iq3 >> 8;
	data[5] = iq3;
	data[6] = iq4 >> 8;
	data[7] = iq4;
	
	CAN_SendData(CAN1, CAN_ID_STD, M3508_SENDID, data);
	
}	


/**
  * @brief  从CAN报文中获取M3508电机信息
  * @param  *RxMessage 	CAN报文接收结构体指针
  *			**info		M3508s_t电机数组指针
  * @retval None
  */
void M3508_getInfo(CanRxMsg RxMessage){
	//报文id确认
	if((RxMessage.StdId < M3508_READID_START) || (RxMessage.StdId > M3508_READID_END))
		return;
	uint32_t StdId;
	StdId = RxMessage.StdId - M3508_READID_START;
	
	//解包数据，数据格式详见C620电调说明书P33
	M3508s[StdId].realAngle = (uint16_t)(RxMessage.Data[0]<<8 | RxMessage.Data[1]);
	M3508s[StdId].realSpeed = (int16_t)(RxMessage.Data[2]<<8 | RxMessage.Data[3]);
	M3508s[StdId].realCurrent = (int16_t)(RxMessage.Data[4]<<8 | RxMessage.Data[5]);
	M3508s[StdId].temperture = RxMessage.Data[6];
	
	M3508s[StdId].infoUpdateFrame++;
	M3508s[StdId].infoUpdateFlag = 1;
}	


///**
//  * @brief  设定M3508电机的目标电流值
//  * @param  motorName 	电机名字 @ref M3508Name_e
//  *			current		电流值，范围 -16384~0~16384
//  * @retval None
//  */
//inline void M3508_setTargetCurrent(M3508Name_e motorName, int16_t current){
//	M3508s[motorName].targetCurrent = current;
//}

///**
//  * @brief  设定M3508电机的目标角度
//  * @param  motorName 	电机名字 @ref M3508Name_e
//  *			angle		电流值，范围 0~8191
//  * @retval None
//  */
//inline void M3508_setTargetAngle(M3508Name_e motorName, uint16_t angle){
//	M3508s[motorName].targetAngel = angle;
//}

/**
  * @brief  设定M3508电机的目标速度
  * @param  motorName 	电机名字 @ref M3508Name_e
  *			speed		电流值，范围 -16384~0~16384
  * @retval None
  */
void M3508_setTargetSpeed(M3508Name_e motorName, int16_t speed){
	M3508s[motorName].targetSpeed = speed;
}



