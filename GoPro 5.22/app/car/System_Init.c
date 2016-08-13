#include "Include_Def.h"

void SetNeutral()
{
  uint16 i;
  
  int32 sum1=0;
  int32 sum2=0;
  int32 sum3=0;

  delayMs(500);
  
  for(i = 0; i<1000; i++)
  {
    sum1 = sum1 +(int32)LPLD_ADC_Get(ADC0, AD15);
    sum2 = sum2 +(int32)LPLD_ADC_Get(ADC0, AD8);
    sum3 = sum3 +(int32)LPLD_ADC_Get(ADC0, AD11);
    delayMs(1);
  }
  
  
  GYRO_OFFSET = (int16)(sum1/1000);
  ACC_OFFSET  = (int16)(sum2/1000);
  DIRECTION_OFFSET = (int16)(sum3/1000);
  
  delayMs(1);
}

//------------------------------------结构体变量-----------------------------------------------
GPIO_InitTypeDef gpio_init_struct0,gpio_init_struct1,gpio_init_struct2;

ADC_InitTypeDef  adc_init_struct0,adc_init_struct1;

UART_InitTypeDef uart_init_struct;

FTM_InitTypeDef  ftm_init_struct0,ftm_init_struct1,ftm_init_struct2;

PIT_InitTypeDef  pit_init_struct;
//------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
void GPIO_Init(void)
{
  /*蜂鸣器*/
  gpio_init_struct0.GPIO_PTx = PTA;
  gpio_init_struct0.GPIO_Pins = GPIO_Pin9;
  gpio_init_struct0.GPIO_Dir = DIR_OUTPUT;
  gpio_init_struct0.GPIO_Output = OUTPUT_L;
  gpio_init_struct0.GPIO_PinControl = IRQC_DIS|OUTPUT_DSH;
  /*0LED初始化*/
  gpio_init_struct1.GPIO_PTx = PTC;
  gpio_init_struct1.GPIO_Pins = GPIO_Pin16|GPIO_Pin17|GPIO_Pin18|GPIO_Pin19;
  gpio_init_struct1.GPIO_Dir = DIR_OUTPUT;
  gpio_init_struct1.GPIO_Output = OUTPUT_L;
  gpio_init_struct1.GPIO_PinControl = IRQC_DIS;
  /*按键触发接口E4到E7*/
  gpio_init_struct2.GPIO_PTx = PTE;
  gpio_init_struct2.GPIO_Pins = GPIO_Pin4|GPIO_Pin5|GPIO_Pin6|GPIO_Pin7;
  gpio_init_struct2.GPIO_Dir = DIR_INPUT;
  gpio_init_struct2.GPIO_PinControl = INPUT_PULL_UP; 
  
  LPLD_GPIO_Init(gpio_init_struct0);
  LPLD_GPIO_Init(gpio_init_struct1);
  LPLD_GPIO_Init(gpio_init_struct2);
}
//--------------------------------------------------------------------------------------------
void ADC_Init(void)
{
  adc_init_struct0.ADC_Adcx = ADC0;
  adc_init_struct0.ADC_DiffMode = ADC_SE;
  adc_init_struct0.ADC_BitMode = SE_12BIT;                    //单端12位精度
  adc_init_struct0.ADC_SampleTimeCfg = SAMTIME_SHORT;           //短采样时间
  adc_init_struct0.ADC_HwAvgSel = HW_16AVG;                     //硬件平均
  
  LPLD_ADC_Init(adc_init_struct0);
  LPLD_ADC_Chn_Enable(ADC0, AD15); //陀螺仪  ADC0  A7 X轴
  LPLD_ADC_Chn_Enable(ADC0, AD8);  //模拟加速度计 ADC0  A8  ZOUT
  LPLD_ADC_Chn_Enable(ADC0, AD11); //陀螺仪  ADC0  A8 Y轴

  adc_init_struct1.ADC_Adcx = ADC1;
  adc_init_struct1.ADC_DiffMode = ADC_SE;
  adc_init_struct1.ADC_BitMode = SE_8BIT;                       //单端8位精度
  adc_init_struct1.ADC_SampleTimeCfg = SAMTIME_SHORT;           //短采样时间 
  adc_init_struct1.ADC_HwAvgSel = HW_32AVG;
  LPLD_ADC_Init(adc_init_struct1);
  LPLD_ADC_Chn_Enable(ADC1, AD14);//  白线 左电感 B10  
  LPLD_ADC_Chn_Enable(ADC1, AD15);//  蓝线 右电感 B11  
}
//----------------------------------------------------------------------------------------------------
void PWM_Init(void)
{
  //电机
  ftm_init_struct0.FTM_Ftmx = FTM0;	                                        //使能FTM0通道
  ftm_init_struct0.FTM_Mode = FTM_MODE_PWM;	                                //输出PWM模式
  ftm_init_struct0.FTM_PwmFreq = 20000;	                                        //PWM频率18000Hz
  ftm_init_struct0.FTM_PwmDeadtimeCfg=DEADTIME_CH45|DEADTIME_CH67;
  
    //编码器 FTM1:A12 A13    
  ftm_init_struct1.FTM_Ftmx = FTM1;              //只有FTM1和FTM2有正交解码功能
  ftm_init_struct1.FTM_Mode = FTM_MODE_QD;       //正交解码功能
  ftm_init_struct1.FTM_QdMode = QD_MODE_PHAB;    //AB相输入模式
  //编码器 FTM2:A10 A11
  ftm_init_struct2.FTM_Ftmx = FTM2;              //只有FTM1和FTM2有正交解码功能
  ftm_init_struct2.FTM_Mode = FTM_MODE_QD;       //正交解码功能
  ftm_init_struct2.FTM_QdMode = QD_MODE_PHAB;    //AB相输入模式
  
  LPLD_FTM_Init(ftm_init_struct0);
  LPLD_FTM_Init(ftm_init_struct1);
  LPLD_FTM_Init(ftm_init_struct2);

  LPLD_FTM_PWM_Enable(FTM0, FTM_Ch4, 0, PTD4, ALIGN_LEFT);
  LPLD_FTM_PWM_Enable(FTM0, FTM_Ch5, 0, PTD5, ALIGN_LEFT);
  LPLD_FTM_PWM_Enable(FTM0, FTM_Ch6, 0, PTD6, ALIGN_LEFT);
  LPLD_FTM_PWM_Enable(FTM0, FTM_Ch7, 0, PTD7, ALIGN_LEFT);
  
  LPLD_FTM_QD_Enable(FTM1, PTA12, PTA13);//右电机
  LPLD_FTM_QD_Enable(FTM2, PTA10, PTA11);//左电机

}
//----------------------------------------------------------------------------------------------------
void UART_Init(void)
{
  uart_init_struct.UART_Uartx = UART4;
  uart_init_struct.UART_BaudRate = 115200;
  uart_init_struct.UART_RxPin =PTE25; //接收引脚
  uart_init_struct.UART_TxPin =PTE24; //发送引脚

  LPLD_UART_Init(uart_init_struct);
}
//--------------------------------------------------------------------------------------------------------
void PIT_Init(void)
{
  pit_init_struct.PIT_Pitx = PIT0;
  pit_init_struct.PIT_PeriodMs = 1;      //定时周期1毫秒
  pit_init_struct.PIT_Isr = PIT_Signal;  //设置中断函数名

  LPLD_PIT_Init(pit_init_struct);
  LPLD_PIT_EnableIrq(pit_init_struct);
}
//--------------------------------------------------------------------------------------------------------
void System_Init(void)
{
  GPIO_Init();
  UART_Init();
  ADC_Init();
  PIT_Init();
  PWM_Init();
  LED_Init();
  InitFlashLed();
  //----------进入按键调参--------
  SetLED();
  
  //---------初始化全局变量-------
  CarControlStop();
  

  
  
  g_nCarCount=0;
  g_nSpeedControlCount=0;
  g_nSpeedControlPeriod=0;
  g_nDirectionControlCount=0;
  g_nDirectionControlPeriod=0;
  
  g_fAngleControlOut=0;
  
  g_nLeftMotorPulse=0;
  g_nRightMotorPulse=0;
  g_fCarSpeed=0;
  g_fSpeedControlOutNew=0;
  g_fSpeedControlOutOld=0;
  g_fSpeedControlIntegral=0;
  g_fSpeedControlOut=0;
  
  VOLTAGE_RIGHT=0;
  VOLTAGE_LEFT=0; 
  DIR_LEFT_OFFSET=0;
  DIR_RIGHT_OFFSET=0;
  g_fDirectionControlOutP=0;
  g_fDirectionControlOutD=0;
  g_fDirectionControlOutOld=0;
  g_fDirectionControlOutNew=0;
  g_fDirection=0;
  g_nDirectionGyro=0;
  g_fDirectionControlOut=0;
  
  g_fLeftMotorOut=0;
  g_fRightMotorOut=0;
}

//---------------------------------控制三环开始---------------------------------------------
void CarControlStart(void) 
 {
    g_nCarControlFlag = 1;//CAR_CONTROL_SET;
    g_nAngleControlFlag = 1;//	ANGLE_CONTROL_START;

    g_nSpeedControlFlag = 1;//	SPEED_CONTROL_START;
    g_nDirectionControlFlag = 1;//	DIRECTION_CONTROL_START;
    
    //  PTA9_O = 1;//蜂鸣器
    //  delayMs(15);
    //  PTA9_O = 0;    
 }

//---------------------------------控制三环结束---------------------------------------------
void CarControlStop(void) 
{
  SetMotorVoltage(0, 0);
  
  g_nCarControlFlag = 0;//	CAR_CONTROL_CLEAR;
  g_nAngleControlFlag = 0;//	ANGLE_CONTROL_STOP;
  g_nSpeedControlFlag = 0;//	SPEED_CONTROL_STOP;
  g_nDirectionControlFlag = 0;//	DIRECTION_CONTROL_STOP;
  
//  PTA9_O = 1;//蜂鸣器
//  delayMs(15);
//  PTA9_O = 0;  
  
  g_fLeftMotorOut = g_fRightMotorOut = 0;
  g_fAngleControlOut = 0;
  g_fSpeedControlOut = 0;
  g_fDirectionControlOut = 0;
  
  //g_nWaitCarStandCount = 0;
}
//------------------------------------------------------------------------------
void WaitCarStand(void) 
{
  if(CAR_ANGLE_NOW > CAR_STAND_ANGLE_MIN &&
     CAR_ANGLE_NOW < CAR_STAND_ANGLE_MAX &&
       Angle_dot > CAR_STAND_ANGLE_MIN &&
         Angle_dot < CAR_STAND_ANGLE_MAX) 
  {

    CarControlStart();//开始控制三环
  } 	
}

//------------------------------------------------------------------------------	
void CheckCarStand(void) 
{
  if(g_nCarControlFlag == 0) return;
  
  if(CAR_ANGLE_NOW >= CAR_FAILURE_ANGLE_MAX ||
     CAR_ANGLE_NOW <= CAR_FAILURE_ANGLE_MIN) {
       CarControlStop();
       return;
     }
}


