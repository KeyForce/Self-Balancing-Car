#include "Include_Def.h"

void PIT_Signal(void)
{   
  DisableInterrupts;
  
  //--------------------------------------------------------------------------
  g_nSpeedControlPeriod++;
  SpeedControlOutput();  
  g_nDirectionControlPeriod ++;
  DirectionControlOutput();
  //---------------------------------------------------------------------------  
  g_nCarCount++;//中断服务片选
  switch(g_nCarCount)
  {
    //---------------------------计数：0   
  case 1:
   {   
      asm("nop"); 
      
   }; break;
    
     //---------------------------计数：1   AD采集                   第1个1ms  
  case 2:          
   {
      asm("nop");
      AngleCalculate(); 
      g_nDirectionGyro = (int16)LPLD_ADC_Get(ADC0, AD11); //方向陀螺仪采集
   }; break;
    
    //----------------------------计数：2   直立控制 电机PWM输出     第2个1ms
  case 3:
    { 
       asm("nop");
       AngleControl();
       MotorOutput();
    };  break;
    
    //----------------------------计数：3    车模速度控制            第3个1ms
  case 4:
    {  
       asm("nop");
       g_nSpeedControlCount++;  
      if(g_nSpeedControlCount>=20)        //控制是否进入速度更新环节
      { 
        SpeedControl();
        g_nSpeedControlCount = 0;
        g_nSpeedControlPeriod = 0; 
      }
    };break;
    
    //----------------------------计数：4    车模方向控制           第4个1ms
  case 5:   
    {  
      asm("nop");
      g_nCarCount=0; 
      
      g_nDirectionControlCount ++;
      DirectionVoltageSigma();           //电感采集、滤波                                             
      
      if( g_nDirectionControlCount >= 2)//控制是否进入方向更新环节
       {
          DirectionControl();
          g_nDirectionControlCount = 0;
          g_nDirectionControlPeriod = 0;
       }
    };break;
  }
  
  
  EnableInterrupts;
}