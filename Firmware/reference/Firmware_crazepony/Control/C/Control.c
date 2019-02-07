/*    
      ____                      _____                  +---+
     / ___\                     / __ \                 | R |
    / /                        / /_/ /                 +---+
   / /   ________  ____  ___  / ____/___  ____  __   __
  / /  / ___/ __ `/_  / / _ \/ /   / __ \/ _  \/ /  / /
 / /__/ /  / /_/ / / /_/  __/ /   / /_/ / / / / /__/ /
 \___/_/   \__,_/ /___/\___/_/    \___ /_/ /_/____  /
                                                 / /
                                            ____/ /
                                           /_____/
*/
/* Control.c file
编写者：小马  (Camel) 、祥、Nieyong
作者E-mail：375836945@qq.com
编译环境：MDK-Lite  Version: 4.23
初版时间: 2014-01-28
功能：
1.PID参数初始化
2.控制函数

------------------------------------
*/
#include <stm32f10x.h>
#include "control.h"
#include "moto.h"
#include "math.h"
#include "sys_fun.h"
#include "mpu6050.h"
#include "imu.h"
#include "extern_variable.h"
#include "led.h"
#include "stmflash.h"
#include "ReceiveData.h"
#include "DMP.h"
#include "Battery.h"
#include "stdio.h"
#include "BT.h"
#include "Altitude.h"
#include "SysConfig.h"

extern uint32_t micros(void);

uint8_t offLandFlag=0;


volatile unsigned char motorLock=1;

int16_t Motor[4]={0};   //定义电机PWM数组，分别对应M1-M4
float rollSp =0,pitchSp =0;		//根据动力分配重新计算得到的期望roll pitch
float Thro=0,Roll=0,Pitch=0,Yaw=0;


//----PID结构体实例化----
PID_Typedef pitch_angle_PID;	//pitch角度环的PID
PID_Typedef pitch_rate_PID;		//pitch角速率环的PID

PID_Typedef roll_angle_PID;   //roll角度环的PID
PID_Typedef roll_rate_PID;    //roll角速率环的PID

PID_Typedef yaw_angle_PID;    //yaw角度环的PID 
PID_Typedef yaw_rate_PID;     //yaw角速率环的PID

PID_Typedef	alt_PID;
PID_Typedef alt_vel_PID;

float gyroxGloble = 0;
float gyroyGloble = 0;


S_FLOAT_XYZ DIF_ACC;		//实际去期望相差的加速度
S_FLOAT_XYZ EXP_ANGLE;	//期望角度	
S_FLOAT_XYZ DIF_ANGLE;	//实际与期望相差的角度	

uint32_t ctrlPrd=0;
uint8_t headFreeMode=0;
float headHold=0;


//函数名：PID_Postion_Cal()
//描述：位置式PID
static void PID_Postion_Cal(PID_Typedef * PID,float target,float measure,int32_t dertT)
{
	float termI=0;
	float dt= dertT/1000000.0;

	//误差=期望值-测量值
	PID->Error=target-measure;

	PID->Deriv= (PID->Error-PID->PreError)/dt;

	PID->Output=(PID->P * PID->Error) + (PID->I * PID->Integ) + (PID->D * PID->Deriv);    //PID:比例环节+积分环节+微分环节

	PID->PreError=PID->Error;
	//仅用于角度环和角速度环的

	if(FLY_ENABLE && offLandFlag){
		if(fabs(PID->Output) < Thro )		              //比油门还大时不积分
		{
			termI=(PID->Integ) + (PID->Error) * dt;     //积分环节
			if(termI > - PID->iLimit && termI < PID->iLimit && PID->Output > - PID->iLimit && PID->Output < PID->iLimit)       //在-300~300时才进行积分环节
					PID->Integ=termI;
		}
	}else{
		PID->Integ= 0;
	}
}


void SetHeadFree(uint8_t on)
{
	if(on==1){
		headHold=imu.yaw;
		headFreeMode=1;
	}else{
		headFreeMode=0;
	}
}


//函数名：CtrlAttiAng(void)
//描述：对飞行器姿态控制（pitch，roll，yaw）控制中，串级PID中的角度环控制
void CtrlAttiAng(void)
{
		static uint32_t tPrev=0;
		float angTarget[3]={0};
		float dt=0,t=0;
		t=micros();
		dt=(tPrev>0)?(t-tPrev):0;
		tPrev=t;
		
		if(altCtrlMode==MANUAL){
			angTarget[ROLL]=(float)(RC_DATA.ROOL);
			angTarget[PITCH]=(float)(RC_DATA.PITCH);
		}else{
			angTarget[ROLL]=rollSp;
			angTarget[PITCH]=pitchSp;
		}

		if(headFreeMode){
			#ifdef YAW_CORRECT
        float radDiff = -(imu.yaw - headHold) * M_PI_F / 180.0f; 
			#else
				float radDiff = (imu.yaw - headHold) * M_PI_F / 180.0f; 
			#endif
        float cosDiff = cosf(radDiff);
        float sinDiff = sinf(radDiff);
        float tarPitFree = angTarget[PITCH] * cosDiff + angTarget[ROLL] * sinDiff;
        angTarget[ROLL] = angTarget[ROLL] * cosDiff - angTarget[PITCH] * sinDiff;
        angTarget[PITCH] = tarPitFree;
		}
 
		PID_Postion_Cal(&pitch_angle_PID,angTarget[PITCH],imu.pitch,dt);
		PID_Postion_Cal(&roll_angle_PID,angTarget[ROLL],imu.roll,dt);	 
}




//函数名：CtrlAttiRate(void)
//描述：对飞行器姿态控制（pitch，roll，yaw）控制中，串级PID中的角速度环控制
void CtrlAttiRate(void)
{
 	float yawRateTarget=0;
	static uint32_t tPrev=0; 

	float dt=0,t=0;
	t=micros();
	dt=(tPrev>0)?(t-tPrev):0;
	tPrev=t;
		
	yawRateTarget=-(float)RC_DATA.YAW;
	
	//注意，原来的pid参数，对应的是 ad值,故转之
	#ifdef IMU_SW
	PID_Postion_Cal(&pitch_rate_PID,pitch_angle_PID.Output,imu.gyro[PITCH]*180.0f/M_PI_F,dt);	
	PID_Postion_Cal(&roll_rate_PID,roll_angle_PID.Output,imu.gyro[ROLL]*180.0f/M_PI_F,dt);//gyroxGloble
	PID_Postion_Cal(&yaw_rate_PID,yawRateTarget,imu.gyro[YAW]*180.0f/M_PI_F,dt);//DMP_DATA.GYROz
	#else
	
	//原参数对应于 DMP的直接输出gyro , 是deg.  且原DMP之后的处理运算是错误的
	PID_Postion_Cal(&pitch_rate_PID,pitch_angle_PID.Output,imu.gyro[PITCH]*DMP_GYRO_SCALE,0);	
	PID_Postion_Cal(&roll_rate_PID,roll_angle_PID.Output,imu.gyro[ROLL]*DMP_GYRO_SCALE,0);//gyroxGloble
	PID_Postion_Cal(&yaw_rate_PID,yawRateTarget,imu.gyro[YAW]*DMP_GYRO_SCALE,0);          //DMP_DATA.GYROz
	#endif

	Pitch = pitch_rate_PID.Output;
	Roll  = roll_rate_PID.Output;
	Yaw   = yaw_rate_PID.Output; 
}

//cut deadband, move linear
float dbScaleLinear(float x, float x_end, float deadband)
{
	if (x > deadband) {
		return (x - deadband) / (x_end - deadband);

	} else if (x < -deadband) {
		return (x + deadband) / (x_end - deadband);

	} else {
		return 0.0f;
	}
}


float thrInit;

#define ALT_FEED_FORWARD  		0.5f
#define THR_MAX								1.0f		//max thrust
#define TILT_MAX 					(Angle_Max * M_PI_F / 180.0 )
const float ALT_CTRL_Z_DB = 0.1f;	//
float spZMoveRate;

uint8_t altCtrlMode;					//normal=0  CLIMB rate, normal .  tobe tested
float hoverThrust=0;
uint8_t zIntReset=1;	//integral reset at first . when change manual mode to climb rate mode
float thrustZInt=0, thrustZSp=0;
float thrustXYSp[2]={0,0};	//roll pitch
uint8_t recAltFlag=0;
float holdAlt=0;
uint8_t satZ=0,satXY=0;	//是否过饱和


#define ALT_LIMIT							2.0f		//限高 3.5
uint8_t isAltLimit=0;
float altLand;



//函数名：estimateHoverThru()
//输入：无
//输出: 预估得到的悬停油门基准值
//描述：预估悬停油门基准值，直接影响到该飞行器的z轴悬停
//悬停油门值相关因素有：电池电压
//Get a estimated value for hold throttle.It will have a direct affection on hover
//Battery voltage
float estimateHoverThru(void){
	float hoverHru = -0.55f;
	
	//电池电压检测
	Battery.BatteryAD  = GetBatteryAD();
	Battery.BatteryVal = Battery.Bat_K * (Battery.BatteryAD/4096.0) * Battery.ADRef;//实际电压 值计算
	
	if(Battery.BatteryVal > 4.05){
		hoverHru = -0.25f;
	}else if(Battery.BatteryVal > 3.90){
		hoverHru = -0.40f;
	}else if(Battery.BatteryVal > 3.80){
		hoverHru = -0.45f;
	}else if(Battery.BatteryVal > 3.70){
		hoverHru = -0.50f;
	}else{
		hoverHru = -0.55f;
	}
	
	
//	if(Battery.BatteryVal > 4.05){
//		hoverHru = -0.05f;
//	}else if(Battery.BatteryVal > 3.90){
//		hoverHru = -0.10f;
//	}else if(Battery.BatteryVal > 3.80){
//		hoverHru = -0.15f;
//	}else if(Battery.BatteryVal > 3.70){
//		hoverHru = -0.20f;
//	}else{
//		hoverHru = -0.25f;
//	}
	
	return hoverHru;
}


//函数名：estimateMinThru()
//输入：无
//输出: 预估得到的最小油门值
//描述：预估最小油门值，根据机重、电池电量而定
//油门过小，下降速度过大时，导致失衡，例如快速下降时机身晃动厉害。再增加fuzzy control ，在油门小时用更大的姿态参数
//相关因素有：电池电压
float estimateMinThru(void){
	float minThru = -0.55f;
	
	//电池电压检测
	Battery.BatteryAD  = GetBatteryAD();
	Battery.BatteryVal = Battery.Bat_K * (Battery.BatteryAD/4096.0) * Battery.ADRef;//实际电压 值计算
	
	if(Battery.BatteryVal > 4.05){
		minThru = -0.30f;
	}else if(Battery.BatteryVal > 3.90){
		minThru = -0.40f;
	}else{
		minThru = -0.55f;
	}
	
	return minThru;
}

//函数名：CtrlAlti()
//输入：无
//输出: 最终结果输出到全局变量thrustZSp
//描述：控制高度，也就是高度悬停控制函数
//only in climb rate mode and landind mode. now we don't work on manual mode
void CtrlAlti(void)
{
	float manThr=0,alt=0,velZ=0;
	float altSp=0;
	float posZVelSp=0;
	float altSpOffset,altSpOffsetMax=0;
	float dt=0,t=0;
	static float tPrev=0,velZPrev=0;
	float posZErr=0,velZErr=0,valZErrD=0;
	float thrustXYSpLen=0,thrustSpLen=0;
	float thrustXYMax=0;
	float minThrust;
	
	//get dt		
	//保证dt运算不能被打断，保持更新，否则dt过大，积分爆满。
	if(tPrev==0){
			tPrev=micros();
			return;
	}else{
			t=micros();
			dt=(t-tPrev) /1000000.0f;
			tPrev=t;
	}
	
	//only in climb rate mode and landind mode. now we don't work on manual mode
	//手动模式不使用该高度控制算法
	if(MANUAL == altCtrlMode || !FLY_ENABLE){
		return;
	}
	
	//--------------pos z ctrol---------------//
	//get current alt 
	alt=-nav.z;
	//get desired move rate from stick
	manThr=RC_DATA.THROTTLE / 1000.0f;
	spZMoveRate= -dbScaleLinear(manThr-0.5f,0.5f,ALT_CTRL_Z_DB);	// scale to -1~1 . NED frame
	spZMoveRate = spZMoveRate * ALT_VEL_MAX;	// scale to vel min max

	//get alt setpoint in CLIMB rate mode
	altSp 	=-nav.z;						//only alt is not in ned frame.
	altSp  -= spZMoveRate * dt;	 
	//limit alt setpoint
	altSpOffsetMax=ALT_VEL_MAX / alt_PID.P * 2.0f;
	altSpOffset = altSp-alt; 
	if( altSpOffset > altSpOffsetMax){
		altSp=alt +  altSpOffsetMax;
	}else if( altSpOffset < -altSpOffsetMax){
		altSp=alt - altSpOffsetMax;
	}

	//限高
	if(isAltLimit)
	{
		if(altSp - altLand > ALT_LIMIT)
		{
				altSp=altLand+ALT_LIMIT;
				spZMoveRate=0;
		}
	}
	
	// pid and feedforward control . in ned frame
	posZErr= -(altSp - alt);
	posZVelSp = posZErr * alt_PID.P + spZMoveRate * ALT_FEED_FORWARD;
	//consider landing mode
	if(altCtrlMode==LANDING)
		posZVelSp = LAND_SPEED;
	
	//获取一个预估的Z轴悬停基准值，相关因素有电池电压
	//get hold throttle. give it a estimated value
	if(zIntReset){
		thrustZInt = estimateHoverThru();
		zIntReset = 0;
	}
	
	velZ=nav.vz;	
	velZErr = posZVelSp - velZ;
	valZErrD = (spZMoveRate - velZ) * alt_PID.P - (velZ - velZPrev) / dt;	//spZMoveRate is from manual stick vel control
	velZPrev=velZ;
	
	thrustZSp= velZErr * alt_vel_PID.P + valZErrD * alt_vel_PID.D + thrustZInt;	//in ned frame. thrustZInt contains hover thrust
	
	//限制最小下降油门
	minThrust = estimateMinThru();
	if(altCtrlMode!=LANDING){
		if (-thrustZSp < minThrust){
			thrustZSp = -minThrust; 
		} 
	}
	
	//与动力分配相关	testing
	satXY=0;
	satZ=0;
	thrustXYSp[0]= sinf(RC_DATA.ROOL * M_PI_F /180.0f) ;//目标角度转加速度
	thrustXYSp[1]= sinf(RC_DATA.PITCH * M_PI_F /180.0f) ; 	//归一化
	thrustXYSpLen= sqrtf(thrustXYSp[0] * thrustXYSp[0] + thrustXYSp[1] * thrustXYSp[1]);
	//limit tilt max
	if(thrustXYSpLen >0.01f )
	{
		thrustXYMax=-thrustZSp * tanf(TILT_MAX);
		if(thrustXYSpLen > thrustXYMax)
		{
				float k=thrustXYMax / thrustXYSpLen;
				thrustXYSp[1] *= k;
				thrustXYSp[0] *= k;
				satXY=1;
				thrustXYSpLen= sqrtf(thrustXYSp[0] * thrustXYSp[0] + thrustXYSp[1] * thrustXYSp[1]);
		}
		
	}
	//limit max thrust!! 
	thrustSpLen=sqrtf(thrustXYSpLen * thrustXYSpLen + thrustZSp * thrustZSp);
	if(thrustSpLen > THR_MAX)
	{
			if(thrustZSp < 0.0f)	//going up
			{
						if (-thrustZSp > THR_MAX) 
						{
								/* thrust Z component is too large, limit it */
								thrustXYSp[0] = 0.0f;
								thrustXYSp[1] = 0.0f;
								thrustZSp = -THR_MAX;
								satXY = 1;
								satZ = 1;

							} 
							else {
								float k = 0;
								/* preserve thrust Z component and lower XY, keeping altitude is more important than position */
								thrustXYMax = sqrtf(THR_MAX * THR_MAX- thrustZSp * thrustZSp);
								k=thrustXYMax / thrustXYSpLen;
								thrustXYSp[1] *=k;
								thrustXYSp[0] *= k;
								satXY=1;
							}
			}
			else {		//going down
							/* Z component is negative, going down, simply limit thrust vector */
							float k = THR_MAX / thrustSpLen;
							thrustZSp *= k;
							thrustXYSp[1] *=k;
							thrustXYSp[0] *= k;
							satXY = 1;
							satZ = 1;
						}
		
	} 
	rollSp= asinf(thrustXYSp[0]) * 180.0f /M_PI_F;
	pitchSp = asinf(thrustXYSp[1]) * 180.0f /M_PI_F;				
	
	
	// if saturation ,don't integral
	if(!satZ )//&& fabs(thrustZSp)<THR_MAX
	{
			thrustZInt += velZErr * alt_vel_PID.I * dt;
			if (thrustZInt > 0.0f)
							thrustZInt = 0.0f;
	}
}
 

//函数名：CtrlMotor()
//输入：无
//输出: 4个电机的PWM输出
//描述：输出PWM，控制电机，本函数会被主循环中100Hz循环调用
void CtrlMotor(void)
{
		float  cosTilt = imu.accb[2] / ONE_G;
	
		if(altCtrlMode==MANUAL)
		{
			DIF_ACC.Z =  imu.accb[2] - ONE_G;
			Thro = RC_DATA.THROTTLE;
			cosTilt=imu.DCMgb[2][2];
			Thro=Thro/cosTilt;
		}else{
			Thro=(-thrustZSp) * 1000;// /imu.DCMgb[2][2];  //倾角补偿后效果不错，有时过猛
			if(Thro>1000)
				Thro=1000;
		}
		
		//将输出值融合到四个电机 
		Motor[2] = (int16_t)(Thro - Pitch - Roll - Yaw );    //M3  
		Motor[0] = (int16_t)(Thro + Pitch + Roll - Yaw );    //M1
		Motor[3] = (int16_t)(Thro - Pitch + Roll + Yaw );    //M4 
		Motor[1] = (int16_t)(Thro + Pitch - Roll + Yaw );    //M2
	
   	if((FLY_ENABLE!=0)) 
			MotorPwmFlash(Motor[0],Motor[1],Motor[2],Motor[3]);   
		else                  
			MotorPwmFlash(0,0,0,0); 
}
