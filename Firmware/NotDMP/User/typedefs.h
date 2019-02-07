#ifndef __TYPEDEFS_H
#define __TYPEDEFS_H

#include <stdbool.h>
#include <stdint.h>
#include "stm32f10x.h"

//允许匿名共用体
#pragma anon_unions

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;


typedef struct{
	float Target; 					//设定目标值
	float Measured; 				//测量值
	float err; 						//本次偏差值
	float err_last; 				//上一次偏差
	float err_beforeLast; 			//上上次偏差
	float Kp, Ki, Kd;				//Kp, Ki, Kd控制系数
	float pwm; 						//pwm输出
	uint32_t MaxOutput;				//输出限幅
    uint32_t IntegralLimit;			//积分限幅 
}incrementalpid_t;

typedef struct{
	float Target; 					//设定目标值
	float Measured; 				//测量值
	float err; 						//本次偏差值
	float err_last; 				//上一次偏差
	float integral_err; 			//所有偏差之和
	float Kp, Ki, Kd;				//Kp, Ki, Kd控制系数
	float pwm; 						//pwm输出
	uint32_t MaxOutput;				//输出限幅
    uint32_t IntegralLimit;			//积分限幅 
}positionpid_t;

typedef struct{
	uint32_t GPIO_CLK;
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
}GPIOPins_t;


typedef struct{
	uint16_t realAngle;			//读回来的机械角度
	int16_t realSpeed;			//读回来的速度
	int16_t realCurrent;		//读回来的实际电流
	uint8_t temperture;			//读回来的电机温度
//	int16_t targetCurrent;			//目标电流
	int16_t targetSpeed;			//目标速度
	uint16_t targetAngle;			//目标角度
	int16_t outCurrent;				//输出电流
	incrementalpid_t pid;		//电机pid
	uint8_t infoUpdateFlag;		//信息读取更新标志
	uint16_t infoUpdateFrame;	//帧率
	uint8_t offLineFlag;		//设备离线标志
	
}M3508s_t;

typedef struct{
//	uint16_t motorInfoID;		//该电机读数据的CAN ID
	uint16_t realAngle;			//读回来的机械角度
	int32_t totalAngle;			//累积总共角度
//	int16_t realSpeed;			//读回来的速度
	int16_t realCurrent;		//读回来的实际电流
//	uint8_t temperture;			//读回来的电机温度
//	int16_t targetCurrent;			//目标电流
//	int16_t targetSpeed;			//目标速度
	int32_t targetAngle;			//目标角度
	int16_t outCurrent;				//输出电流
	uint16_t lastAngle;			//上次的角度
	int16_t turnCount;			//转过的圈数
	incrementalpid_t pid_speed;		//电机速度pid
	positionpid_t pid_angle;		//角度电机pid
	uint8_t infoUpdateFlag;		//信息读取更新标志
	uint16_t infoUpdateFrame;	//帧率
	uint8_t offLineFlag;		//设备离线标志
}M6623s_t;

typedef struct{
//	uint16_t motorInfoID;		//该电机读数据的CAN ID
	uint16_t realAngle;			//读回来的机械角度
	int16_t realSpeed;			//读回来的速度
	int16_t realTorque;			//读回来的实际转矩
	uint16_t lastAngle;			//上次的角度
	int32_t totalAngle;			//累积总共角度
	int16_t turnCount;			//转过的圈数
//	uint8_t temperture;			//读回来的电机温度
//	int16_t targetCurrent;			//目标电流
	int16_t targetSpeed;			//目标速度
	int32_t targetAngle;			//目标角度
	int16_t outCurrent;				//输出电流
	incrementalpid_t pid_speed;		//电机速度pid
	positionpid_t pid_angle;		//电机角度pid
	uint8_t infoUpdateFlag;		//信息读取更新标志
	uint8_t loadFinish;			//子弹加载完毕
	uint16_t infoUpdateFrame;	//帧率
	uint8_t offLineFlag;		//设备离线标志
}M2006s_t;

typedef struct{
	uint8_t shootFlag;			//射击标志
	uint8_t fricReady;			//摩擦轮准备标志
	uint8_t shootReady;			//摩擦轮准备标志
	uint16_t fricTargetSpeedRaw;	//摩擦轮目标速度
	float fricLpfAttFactor;		//摩擦轮滤波系数
	float fricspeedLPF;			//摩擦轮速度滤波后数据
	uint8_t waitTime;			//射击间隔
	uint8_t timeCount;			//时间计数
}shootGun_t;

typedef struct{
	float targetYawRaw;			//云台目标yaw轴原始数据
	float targetPitchRaw;		//云台目标pitch轴原始数据
	float LpfAttFactor;			//云台滤波系数
	float targetYawLPF;			//云台yaw轴滤波后数据
	float targetPitchLPF;		//云台pitch轴滤波后数据
	float yawOverflow;			//云台yaw轴转向溢出值
	float pitchOverflow;		//云台pitch轴转向溢出值
	float IMUtargetYawRaw;		//云台目标yaw轴原始数据
	float IMUtargetPitchRaw;	//云台目标pitch轴原始数据
	float IMUtargetYawLPF;		//云台yaw轴滤波后数据
	float IMUtargetPitchLPF;	//云台pitch轴滤波后数据
	positionpid_t YawAttitude_pid;			//云台yaw轴姿态pid
	positionpid_t RollAttitude_pid;			//云台roll轴姿态pid
	uint8_t Mode;						//云台控制模式
}Cloud_t;

typedef struct{
	float targetXRaw;		//底盘x轴原始数据
	float targetYRaw;		//底盘y轴原始数据
	float targetZRaw;		//底盘z轴原始数据
	float LpfAttFactor;		//底盘滤波系数
	float targetXLPF;		//底盘x轴滤波后数据
	float targetYLPF;		//底盘y轴滤波后数据
	float targetZLPF;		//底盘z轴滤波后数据
	float speedLimit;		//当前最大速度
	float IMUtargetYawRaw;		//底盘目标yaw轴原始数据
	float FollowtargetYawRaw;	//底盘目标yaw轴跟随云台原始数据
	float IMUtargetYawLPF;		//底盘yaw轴滤波后数据
	float FollowtargetYawLPF;	//底盘yaw轴跟随云台滤波后数据
	positionpid_t YawAttitude_pid;				//底盘yaw轴姿态pid
	positionpid_t FollowYawAttitude_pid;		//底盘yaw轴跟随云台pid
	uint8_t mode;								//底盘控制模式
	uint8_t PowerOverflowFlag;					//超功率标志位
}Chassis_t;

typedef struct{
	int32_t calibrationPosArray[2][3];
	int32_t BigSymbol_positionArray[2][9];
	uint8_t calibrationStep;
	uint8_t calibrationFinish;
	uint8_t targetPosition;
	uint8_t targetInfoUpdate;
}BigSymbol_t;


typedef enum{
	M3508_UL = 0,
	M3508_UR,
	M3508_DL,
	M3508_DR
}M3508Name_e;

typedef enum{
	M6623_YAW = 0,
	M6623_PITCH,
}M6623Name_e;

typedef enum{
	M2006_BulletLoad = 0,
}M2006Name_e;

typedef enum{
	ControlMode_ByRC = 3,
	ControlMode_ByPC = 2,
	ControlMode_OFF = 1,
}ControlMode_e;


typedef enum{
	SpecialMode_Normal = 0,
	//SpecialMode_BigSymbolPreFix = 1,
	SpecialMode_BigSymbol = 2,	//大符模式
	//SpecialMode_Swing = 3,
	SpecialMode_UPBridge = 3,	//上桥模式
	SpecialMode_Supply = 4, 	//补给站模式
}SpecialMode_t;

typedef struct{
	uint8_t ControlMode;			//控制方式
	uint8_t SpecialMode;			//特殊模式
	uint8_t isCombineKey;			//使用组合键
}ControlStatus_t;

typedef struct{
	uint16_t KeyStatus;
	uint16_t lastKeyStatus;
	uint16_t keyPressedJump;
	uint16_t keyReleaseJump;
	uint16_t longPressed;
	uint16_t timer;
}key_t;

typedef struct {
	struct{
		int16_t ch0;
		int16_t ch1;
		int16_t ch2;
		int16_t ch3;

		uint8_t s_left;
		uint8_t s_right;
	}rc;
	
	struct{
		int16_t x;
		int16_t y;
		int16_t z;
	
		uint8_t keyLeft;
		uint8_t keyRight;
		
	}mouse;
	
	union {
		uint16_t key_code;
		struct{
			bool press_W:1;
			bool press_S:1;
			bool press_A:1;
			bool press_D:1;
			bool press_Shift:1;
			bool press_Ctrl:1;
			bool press_Q:1;
			bool press_E:1;
			
			bool press_R:1;
			bool press_F:1;
			bool press_G:1;
			bool press_Z:1;
			bool press_X:1;
			bool press_C:1;
			bool press_V:1;
			bool press_B:1;
		};
	}keyBoard;
	
	uint16_t infoUpdateFrame;	//帧率
	uint8_t offLineFlag;		//设备离线标志
}DR16_DBUS_DATA_t;


typedef struct {
	union {
		uint8_t dataBuff[2][8];
		struct{
			int16_t posX;		/* 定位坐标x （厘米） */
			int16_t posY;		/* 定位坐标y （厘米） */
			uint16_t angleYaw;		/* 航向角 */
			int16_t distance[6];		/* 标签到各个基站的距离 */
			uint16_t errANDsignel;		/* 错误类型与信号强度 */
			uint16_t reserved;			/* 保留 */
		};
	}data;
	uint8_t infoUpdateFlag;
}UWB_DATA_t;



typedef struct {
	float x;
	float y;
	float radian;
	float degrees;
	float distance;
}rocker_t;


typedef struct {
		float w;
		float x;
		float y;
		float z;
}quaternion_t;

typedef struct {
		float x;
		float y;
		float z;
}vector_t;

typedef struct {
		int16_t x;
		int16_t y;
		int16_t z;
}vector16_t;

typedef struct {
	float roll;
	float pitch;
	float yaw;
}eular_t;

typedef struct{
	vector_t gyro;
	eular_t eular;
	float lastYaw;
	float totalYaw;
	int16_t turnCount;
	uint8_t devStatus;
	uint8_t infoUpdateFlag;
	uint16_t infoUpdateFrame;
	uint8_t offLineFlag;
}GY_IMU_t;


//比赛机器人状态，频率 10Hz 推送 CmdID 0x0001
typedef struct{
	union {
		uint8_t dataBuff[8];
		struct{
			uint16_t gameRemianTime;		//当前阶段剩余时间，单位 s
			uint8_t gameStatus;				//当前比赛状态
			uint8_t robotLevel;				//机器人当前等级
			uint16_t remainHP;				//机器人当前血量
			uint16_t maxHP;					//机器人满血量
		};
	}data;
	uint8_t infoUpdateFlag;
}Judge_RobotStatus_t;


//伤害数据，受到攻击伤害实时推送 CmdID 0x0002
typedef struct{
	union {
		uint8_t dataBuff[1];
		struct{
			uint8_t armorType : 4;		//受到伤害的装甲板ID号
			uint8_t hurtType : 4;		//伤害类型
		};
	}data;
	uint8_t infoUpdateFlag;
}Judge_RobotHurt_t;


//实时射击数据 CmdID 0x0003
typedef struct{
	union {
		uint8_t dataBuff[6];
		struct{
			uint8_t bulletType;		//弹丸类型
			uint8_t bulletFreq;		//弹丸射频，单位：发每秒
			float bulletSpeed;		//弹丸射速，单位：米每秒
		};
	}data;
	uint8_t infoUpdateFlag;
}Judge_ShootData_t;

//实时功率和热量数据，50Hz 频率周期发送 CmdID 0x0004
typedef struct{
	union {
		uint8_t dataBuff[20];
		struct{
			float chassisVolt;
			float chassisCurrent;
			float chassisPower;
			float chassisPowerBuffer;
			uint16_t shooter17mm_Heat;
			uint16_t shooter42mm_Heat;
		};
	}data;
	uint8_t infoUpdateFlag;
}Judge_PowerHeatData_t;


//实时场地交互数据，检测到 IC 卡时，10Hz 频率推送 CmdID 0x0005
typedef struct{
	union {
		uint8_t dataBuff[2];
		struct{
			uint8_t cardType;		//卡类型
			uint8_t cardIndex;		//卡索引号，可用于区分不同区域
		};
	}data;
	uint8_t infoUpdateFlag;
}Judge_RFIDCard_t;


//比赛结果数据，比赛结束时推送 CmdID 0x0006
typedef struct{
	uint8_t winner;			//比赛结果
}Judge_GameResult_t;


//获得 buff CmdID 0x0007
typedef struct{
	union {
		uint8_t dataBuff[2];
		struct{
			uint8_t buffType;		//Buff 类型
			uint8_t buffAddition;	//加成百分比（比如 10 代表加成 10%）
		};
	}data;
	uint8_t infoUpdateFlag;
}Judge_GetBuff_t;

//机器人位置和枪口朝向信息，50Hz 频率周期发送 CmdID 0x0008
typedef struct{
	union {
		uint8_t dataBuff[16];
		struct{
			float x;			//x位置坐标
			float y;			//y位置坐标
			float z;			//z位置坐标
			float gunYaw;		//枪口朝向角度
		};
	}data;
	uint8_t infoUpdateFlag;
}Judge_RobotPos_t;

//参赛队自定义数据，用于显示在操作界面，限频 10Hz CmdID 0x0100
typedef struct{
	union {
		uint8_t dataBuff[13];
		struct{
			float data1;
			float data2;
			float data3;
			uint8_t mask;
		};
	}data;
	uint8_t infoUpdateFlag;
	
}Judge_SendData_t;

#endif /* __TYPEDEFS_H */
