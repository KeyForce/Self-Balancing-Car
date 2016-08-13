#include "Include_Def.h"

//***********************************速度PI***********************************************
void SpeedControl(void) 
 {
    float fP,fI,fDelta;
    int8 index;
    
    if(g_nSpeedControlFlag == 0) 
     {
        g_fSpeedControlOutOld =0; 
        g_fSpeedControlOutNew =0;
        g_fSpeedControlOut = 0;
        g_fSpeedControlIntegral = 0;
        return;
     }
    //-----------------------------------------------------------------------
    g_nLeftMotorPulse  =  LPLD_FTM_GetCounter(FTM2);
    g_nRightMotorPulse =  LPLD_FTM_GetCounter(FTM1);
    LPLD_FTM_ClearCounter(FTM2);
    LPLD_FTM_ClearCounter(FTM1);
    //if(g_nLeftMotorPulse<0)		g_nLeftMotorPulse = -g_nLeftMotorPulse;
    //if(g_nRightMotorPulse<0)		g_nRightMotorPulse = -g_nRightMotorPulse;
    
    g_fCarSpeed = (g_nLeftMotorPulse + g_nRightMotorPulse) / 2;
    //------------------------------------------------------------------------
    
    fDelta =CarSpeedTarget-g_fCarSpeed;
   
    //@@@@@@@@@@@@@@@@@@@@===16.5.17 night====@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    if( fabs(fDelta)<180 )
    {
      index=2;
    }else index=1;
     //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    
    fP = fDelta * SPEED_CONTROL_P*0.01;
    fI = fDelta * SPEED_CONTROL_I*index*0.01;
    
    g_fSpeedControlIntegral += fI;
    
    //==========================================================================
#define INTEGRAL_MAX        3000
#define INTEGRAL_MIN       -3000   
    
#define P_MAX         3000    
#define P_MIN        -3000
    
    if (g_fSpeedControlIntegral >= INTEGRAL_MAX)     g_fSpeedControlIntegral=INTEGRAL_MAX;
    if (g_fSpeedControlIntegral <= INTEGRAL_MIN)    g_fSpeedControlIntegral=-INTEGRAL_MIN;
    
    if (fP >= P_MAX )     fP=P_MAX;
    if (fP <= P_MIN )     fP=P_MIN;
    
    //===========================================================================    
    g_fSpeedControlOutOld = g_fSpeedControlOutNew;
    g_fSpeedControlOutNew = fP + g_fSpeedControlIntegral;
    
 }

//*************************************速度平滑输出**************************************************
void SpeedControlOutput(void) 
{
  
   float fValue;
  fValue = g_fSpeedControlOutNew - g_fSpeedControlOutOld;
  
  g_fSpeedControlOut = fValue * (g_nSpeedControlPeriod + 1) / SPEED_CONTROL_PERIOD + g_fSpeedControlOutOld;
}

//*************************************目标速度调节***************************************************
void SpeedTargetAdjust(void)
 {
#define SPEED_INC			100
#define SPEED_DEC			300
    if(g_nCarControlFlag) 
     {
        if( g_fCarSpeedSet != CarSpeedTarget) 
         {
            if(CarSpeedTarget > g_fCarSpeedSet) 
             {
                g_fCarSpeedSet += SPEED_INC;
                if(CarSpeedTarget < g_fCarSpeedSet)
                   g_fCarSpeedSet = CarSpeedTarget;
             } 
            else 
             {
                g_fCarSpeedSet -= SPEED_DEC;
                if(g_fCarSpeedTarget > g_fCarSpeedSet)
                   g_fCarSpeedSet = g_fCarSpeedTarget;
             }
         }
     }
 }