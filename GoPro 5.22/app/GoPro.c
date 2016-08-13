#define CAR_GLOBALS 1
#include "Include_Def.h"
/*
 *  Data  2016.5.22  Night
 *  Log   取消速度加减控制
 *        速度环变积分控制
 *        方向换取消不完全微分控制 加入分段控制
 *        直立环限幅了
 *        测试fabs（）会改变内部的值吗
 */
void main (void)
 {
   DisableInterrupts;
    
    System_Init();
    //----------蜂鸣器-----------------------------------------------------------------------------------------------
    //  PTA9_O = 1;
    //  delayMs(10);
    //  PTA9_O = 0;     
    
    delayMs(30);
    SetNeutral();
    
    
    EnableInterrupts;
    
    //---------------------------等待车子到达直立---------------------------------------------------------------------
    while( g_nCarControlFlag==0 )
     {
        WaitCarStand();
        LED_FLOAT_PRINT(0,  0,"ANGLE: ",CAR_ANGLE_NOW, 4);
     }
    g_nDirectionPrior=0;// 1为方向控制优先 0为速度控制优先
    
    /*
    *  1：启动 
    *  0：关闭
    */
    g_nAngleControlFlag = 1;	/*****ANGLE_CONTROL_START*******/
    g_nSpeedControlFlag = 1;	/*****SPEED_CONTROL_START*******/
    g_nDirectionControlFlag = 1;  /*****DIRECTION_CONTROL_START***/
    
    while(1)
     { 
        //-----------------------数据发送选择区-----------------------------------
        //    switch(4)
        //    {
        //    case 1: SendData1();break;//1.MMA8451_Value   2.Angle_dot   3.Angle
        //    case 2: SendData2();break;//1.g_fAngleControlOut  2.g_fSpeedControlOut 3.g_fDirectionControlOut;
        //    case 3: SendData3();break;//1.g_nLeftMotorPulse 2.g_nRightMotorPulse
        //    case 4: SendData4();break;//1.fAngleAndSpeed  2.g_fCarSpeed
        //    case 5: SendData5();break;//1.VOLTAGE_LEFT    2.VOLTAGE_RIGHT
        //    case 6: SendData6();break;
        //    default:break;
        //    }
        //    LPLD_LPTMR_DelayMs(15);
        
        //-----------------------OLED显示区----------------------------------
        LED_FLOAT_PRINT(0,  0,"ANGLE: ",CAR_ANGLE_NOW, 4);
        LED_FLOAT_PRINT(0,  2,"L: ", VOLTAGE_LEFT, 4);
        LED_FLOAT_PRINT(0,  3,"R: ", VOLTAGE_RIGHT, 4);
        LED_FLOAT_PRINT(0,  5,"D:", g_fDirection, 4);
        
        
        
     }
 }
