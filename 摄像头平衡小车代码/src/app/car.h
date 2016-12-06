#ifndef _CAR_H_
#define _CAR_H_

#include "include.h"


#define Angle_Out_Max  (12800)//上限值
#define Angle_Out_Min   (-12800)//下限值

#define Time_PartCntMax  5   //5个中断片


#define CAR_SPEED_CONSTANT  0.164//速度的比例的调节（1000/(100*157))

#define  SPEED_CONTROL_COUNT  20   //速度分布输出次数                       
#define  SPEED_CONTROL_PERIOD  100//电机的控制周期是100ms

#define  MOTOR_OUT_DEAD_VAL  0//电机死区控制
#define  MOTOR_OUT_MAX 0//输出的最大占空比
#define  MOTOR_OUT_MIN 0//输出的最小占空比

#define  DIRECTION_CONTROL_PERIOD 17	//方向控制周期20ms
#define  DIRECTION_CONTROL_COUNT 1	//方向分布输出次数10ms
#define  DIRECTION_OFFSET  0
/************摄像头参数*****************/
#define ROW 220               //行数
#define COL 260              //列数  为消除消隐区  可以改变车体的位置
#define PicSize  ROW*COL      //图像大小


#define rowStart 160       //图像处理起始行数100
#define rowEnd   200      //图像处理终止行数140
#define rowNum   rowEnd-rowStart  //有效处理行数，原始采集行数为ROW(在Include.h中定义)
#define proPix   6        //边缘跟踪搜索边界有效像素  
#define CENTURE  154      //中心值

void Get_AD_angle(void);
void Angle_Control(void);//计算倾角的大小

void SpeedControl(void);//计算车的速度
void SpeedControlOutput(void);
void GetMotorspeed(void);
void SetMotorPWM(float L_Motor_PWMOUT,float R_Motor_PWMOUT);
void MotorSpeedOut(void);
void MotorOutput(void) ;
void DirectionControlOutput(void);
void DirectionControl(void);
void DataReset(void);
void ImageProc(void);
#endif