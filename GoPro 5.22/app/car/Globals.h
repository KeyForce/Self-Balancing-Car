#ifndef _Globals_H_
#define _Globals_H_

#ifdef CAR_GLOBALS
    #define CAR_EXT
#else
    #define CAR_EXT extern
#endif 


/**************************/
CAR_EXT	float DirOld;
CAR_EXT	float DirNew;
CAR_EXT	float DifferSub;
CAR_EXT	float DifferOutNew;
CAR_EXT	float DifferOutOld;
//-------------------------发车控制标识----------------------------------
CAR_EXT	int g_nCarControlFlag;
CAR_EXT	int g_nAngleControlFlag;
CAR_EXT	int g_nSpeedControlFlag;
CAR_EXT	int g_nDirectionControlFlag;


//------------------------循环计数----------------------------------
CAR_EXT int g_nCarCount;//1~5事件片选

CAR_EXT int g_nSpeedControlCount;//速度PID更新计数  5ms * 20 =100 ms  控制一次 均分在20个角度控制周期内
CAR_EXT int g_nSpeedControlPeriod;//平滑输出速度

CAR_EXT int g_nDirectionControlCount;//方向PID更新计数 5ms * 2 =10ms 控制一次
CAR_EXT int g_nDirectionControlPeriod;//平滑方向速度

CAR_EXT int g_nDirectionPrior;//方向控制优先控制
CAR_EXT int StraightRoadCount;
//------------------------直立环--------------------------------
CAR_EXT float g_fAngleControlOut;

CAR_EXT float ENC_03_Value;//陀螺仪ADC0 A7 加速度计ADC0 BO
CAR_EXT float MMA8451_Value;
CAR_EXT int16 ACC_Value  ;//加速度AD
CAR_EXT int16 GYRO_Value ;//陀螺仪AD


//------------------------速度环-------------------------------
CAR_EXT int16 g_nLeftMotorPulse,g_nRightMotorPulse;//int16输出正负

CAR_EXT float g_fCarSpeed;//编码器取读的速度

CAR_EXT float g_fCarSpeedSet,g_fCarSpeedTarget;

CAR_EXT float g_fSpeedControlOutNew;
CAR_EXT float g_fSpeedControlOutOld;

CAR_EXT float g_fSpeedControlIntegral;

CAR_EXT float g_fSpeedControlOut;

//-------------------------方向环------------------------------

CAR_EXT int16 VOLTAGE_RIGHT,VOLTAGE_LEFT;      //左右电感AD
CAR_EXT int16 DIR_LEFT_OFFSET,DIR_RIGHT_OFFSET;//丢线AD

CAR_EXT int16 g_nLeftVoltageSigma,g_nRightVoltageSigma;//电感2次累加值

CAR_EXT float g_fDirectionControlOutP;
CAR_EXT float g_fDirectionControlOutD;

CAR_EXT float g_fDirectionControlOutOld;
CAR_EXT float g_fDirectionControlOutNew;

CAR_EXT float g_fDirection;   //电感归一化输出值

CAR_EXT int16 g_nDirectionGyro;//方向控制变化量

CAR_EXT float g_fDirectionControlOut;

CAR_EXT int16 CurrentDirError;
CAR_EXT float DirDFeedBack,LastDirErr;


CAR_EXT int16 Encoder_Left;
CAR_EXT int16 Encoder_Right; 
CAR_EXT float speed_straight;
//--------------------------------------------------------------
CAR_EXT float fAngleAndSpeed;

CAR_EXT float g_fLeftMotorOut,g_fRightMotorOut;

CAR_EXT int16 GYRO_OFFSET;              //x轴陀螺仪零偏值
CAR_EXT int16 ACC_OFFSET ;               //加速度计零偏值
CAR_EXT int16 DIRECTION_OFFSET;           //y轴陀螺仪零偏值


#endif