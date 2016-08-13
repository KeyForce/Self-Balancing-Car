#include "Include_Def.h"

void SetMotorVoltage(float fLeftVoltage, float fRightVoltage)
{
  int16 nOut;                         

  if(fLeftVoltage > 0)
  {
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch5, 0);
    nOut = (int)(fLeftVoltage);
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch4, nOut);
  }
  else
  {
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch4, 0);
    fLeftVoltage = -fLeftVoltage;
    nOut = (int)(fLeftVoltage);
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch5, nOut);
  }
  //--------------------------------------------//
  if(fRightVoltage > 0)
  {
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch7, 0);
    nOut = (int)(fRightVoltage);
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch6, nOut);
  }
  else
  {
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch6, 0);
    fRightVoltage = -fRightVoltage;//将负值变成正值
    nOut = (int)(fRightVoltage);
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch7, nOut);  
  }
}

//--------------------------------------------------------------------------//
void MotorSpeedOut()
{
  float fLeftVal, fRightVal;
  
  fLeftVal  = g_fLeftMotorOut;
  fRightVal = g_fRightMotorOut;
  
  /*增加死区常数*/
  if(fLeftVal > 0) 			
    fLeftVal += MOTOR_OUT_L_DEAD_VAL;
  else if(fLeftVal < 0) 		
    fLeftVal -= MOTOR_OUT_L_DEAD_VAL;
  
  if(fRightVal > 0)			
    fRightVal += MOTOR_OUT_R_DEAD_VAL;
  else if(fRightVal < 0)		
    fRightVal -= MOTOR_OUT_R_DEAD_VAL;
  
  /*输出饱和处理，防止超出PWM范围*/	
  if(fLeftVal > MOTOR_OUT_MAX)	fLeftVal = MOTOR_OUT_MAX;
  if(fLeftVal < MOTOR_OUT_MIN)	fLeftVal = MOTOR_OUT_MIN;
  
  if(fRightVal > MOTOR_OUT_MAX)	fRightVal = MOTOR_OUT_MAX;
  if(fRightVal < MOTOR_OUT_MIN)	fRightVal = MOTOR_OUT_MIN;
  
   SetMotorVoltage(fLeftVal, fRightVal);
}

/***************************************************************
** 函数名称: MotorOutput
** 功能描述: 电机输出函数            
** 输　  入:   
** 输　  出:   
** 备    注: 将直立控制、速度控制、方向控制的输出量进行叠加,并加
	     入死区常量，对输出饱和作出处理。
***************************************************************/
void MotorOutput(void)
{
  //  float fLeft, fRight;
  
  //  fLeft  = g_fAngleControlOut*6-g_fSpeedControlOut - g_fDirectionControlOut;
  //  fRight = g_fAngleControlOut*6-g_fSpeedControlOut + g_fDirectionControlOut;
  
  //--------------------------------------角度环输出---------------------------  
  //  fLeft  = g_fAngleControlOut*6- g_fDirectionControlOut;
  //  fRight = g_fAngleControlOut*6+ g_fDirectionControlOut;
  //--------------------------------------角度环速度环叠加输出------------------ 
  //  fLeft  = g_fAngleControlOut*6-g_fSpeedControlOut;
  //  fRight = g_fAngleControlOut*6-g_fSpeedControlOut;
  //  
  float fLeft, fRight;//fAngleAndSpeed
  fAngleAndSpeed = g_fAngleControlOut-g_fSpeedControlOut;
  
 
  if(g_nDirectionPrior==0)// 速度、角度控制优先
  {
    fLeft  = fAngleAndSpeed - g_fDirectionControlOut;
    fRight = fAngleAndSpeed + g_fDirectionControlOut;
  }
  else if(g_nDirectionPrior==1)// 方向控制优先
  {
    
    if((fAngleAndSpeed)>MOTOR_OUT_MAX)
    {
      if(g_fDirectionControlOut>=0)
      {
        fLeft  = MOTOR_OUT_MAX-g_fDirectionControlOut
          +(fAngleAndSpeed-MOTOR_OUT_MAX)/2;
        fRight = MOTOR_OUT_MAX;
      }
      else
      {
        fLeft  = MOTOR_OUT_MAX;
        fRight = MOTOR_OUT_MAX+g_fDirectionControlOut
          +(fAngleAndSpeed-MOTOR_OUT_MAX)/2;
      }
    }
    else
    {
      fLeft  = fAngleAndSpeed	- g_fDirectionControlOut;
      fRight = fAngleAndSpeed	+ g_fDirectionControlOut;
    }
  }

  if(fLeft > MOTOR_OUT_MAX)		fLeft = MOTOR_OUT_MAX;
  if(fLeft < MOTOR_OUT_MIN)		fLeft = MOTOR_OUT_MIN;
  
  if(fRight > MOTOR_OUT_MAX)		fRight = MOTOR_OUT_MAX;
  if(fRight < MOTOR_OUT_MIN)		fRight = MOTOR_OUT_MIN;
  
  g_fLeftMotorOut  = fLeft;
  g_fRightMotorOut = fRight;
  MotorSpeedOut();
}