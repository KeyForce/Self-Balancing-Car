/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：isr.c
 * 描述         ：中断处理例程
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：野火嵌入式开发工作室
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/
#include "common.h"
#include "include.h"
#include "isr.h"
#include "car.h"
#include "dma.h"
u16  Time_PartCnt;
extern u16 g_nSpeedControlPeriod;
extern u16 g_SpeedControlCount;
extern float   g_fAngleControlOut;
extern s16 g_nRightMotorPulse;
extern float g_fSpeedControlOutNew;
extern float g_fSpeedControlOut;

extern s16 g_RightMotorPulseSigma;
extern s16 g_LeftMotorPulseSigma;
extern float CAR_SPEED_SET;
extern u16 g_nDirectionControlPeriod;
extern u16 g_nDirectionControlCount;
u16 count=0;
u16 count1=0;
/***************摄像头参数*************/
extern u32 rowCnt ;   //行计数
extern u8 Buffer1[ROW][COL];
u8 SampleFlag = 0;
u8 VSYN_Flag=0;
u8 VSYN1_FLAG;
u8 Run_Flag=0;
float flag_count=0;
extern u16 speed1;
void delays(u16 t)
{
     while(t--);
}
void PIT0_IRQHandler(void)//每隔5ms秒执行一次 MOTOROUT    
{
      Time_PartCnt++;
      
      g_nSpeedControlPeriod++;
      SpeedControlOutput();
      
      g_nDirectionControlPeriod++;
      DirectionControlOutput();
      count1++;
      count++;
      if(count1==2500)
      {  
         Run_Flag=1;
         
      }
      if(Run_Flag==1)
      {
         flag_count++;
         CAR_SPEED_SET=(flag_count/1000.0)*speed1;
         if(CAR_SPEED_SET>=speed1)
         {
            Run_Flag=0;
            CAR_SPEED_SET=speed1;
            flag_count=0;
         }
      }
      if(count1>2600)
      {
        count1=2700;
      }
      if(count>8000)
      {
        count=8000;
      }
     if((PIT_TFLG(PIT0)&PIT_TFLG_TIF_MASK)!=0)
       {  
       
        if(Time_PartCnt>=Time_PartCntMax)//5ms的时间获取速度脉冲  片段5
        {
            Time_PartCnt=0;
            GetMotorspeed();
            
        }else  if(Time_PartCnt==1)//片段1  
              {
            
           
              }else if(Time_PartCnt==2)//角度控制
                     {
                       Angle_Control();
                       
                     }else if(Time_PartCnt==3)//速度控制
                           {
                              g_SpeedControlCount++;
                              if(g_SpeedControlCount>=SPEED_CONTROL_COUNT)
                               {
                                 SpeedControl();
                                 g_SpeedControlCount=0;
                                 g_nSpeedControlPeriod=0;
                               }
                           }else if(Time_PartCnt==4)//方向控制
                                    {
                                      g_nDirectionControlCount++;
                                      if(g_nDirectionControlCount >= DIRECTION_CONTROL_COUNT)
    	                                  { 
                                              DirectionControl();
                                              g_nDirectionControlCount = 0; 
                                              g_nDirectionControlPeriod = 0;  
                                          }
         
                                    }     
        MotorOutput();
	PIT_TFLG(PIT0)|=PIT_TFLG_TIF_MASK;       //清标志
       }
      PIT_TCTRL(PIT0)|=PIT_TCTRL_TIE_MASK;//使能定时器中断
}
void PORTB_IRQHandler(void)
{
  
}
/***********************行场中断*****************/
void PORTA_IRQHandler(void)
{
      //---VSYN场中断处理
    if(PORTA_ISFR & (1 << 24))         //PTA24触发中断  场中断
    { 
        PORTA_ISFR  |= (1 << 24);       //写1清中断标志位
        rowCnt = 0;
        SampleFlag = 1; 
    }
}
void PORTD_IRQHandler(void)
{
   //---HREF行中断处理
    if( PORTD_ISFR & (1 << 13))        
    {
        PORTD_ISFR  |= (1 << 13);  
        
        if ( SampleFlag == 0 )         //不足一场时返回   其实这个是丢弃第一个不完整的场
        { 
          return;
        }  
        DMA_PORTx2BUFF_Init (DMA_CH4, (void *)&PTB_BYTE0_IN, Buffer1[rowCnt], PTD12, DMA_BYTE1, COL, DMA_rising_keepon);
        DMA_EN(DMA_CH4);//重新使能DMA的硬件请求 
        //一行数据包括数据和消隐区:当采集够点数时(DMA中断发生禁止DMA),其实一行并没有结束,此时后面的点都不管了当进入消隐区后此时触发行中断然后使能DMA进行下一次采集
        rowCnt++;
        if(rowCnt == ROW)
        {
          rowCnt = 0;
          DMA_DIS(DMA_CH4);
          VSYN_Flag = 1;//完成数据的采集
          disable_irq(PORTD + 87);
         
        }
       
    }
}
void DMA_CH0_Handler(void)
{
}