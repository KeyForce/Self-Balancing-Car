#include "Include_Def.h"
/*加速度计 陀螺仪 采集 计算 滤波*/
void AngleCalculate(void)
{

  //陀螺仪AD采集
  GYRO_Value = (int16)LPLD_ADC_Get(ADC0, AD10);                                             //ADC0  A7 X轴

  //模拟加速度计AD采集  归一化
  ACC_Value = (int16)LPLD_ADC_Get(ADC0, AD8);                                               //ADC0  A8  ZOUT

  //计算加速度计倾角
  MMA8451_Value = ( ACC_Value - ACC_OFFSET ) * 0.098;

  //陀螺仪减零点
  ENC_03_Value = ( GYRO_Value - GYRO_OFFSET ) * 0.26;

  //卡尔曼
  Kalman_Filter(MMA8451_Value,ENC_03_Value);

}



/*直立控制 电机PWM输出*/
void AngleControl(void)
{
  if(g_nAngleControlFlag == 0) 
  {
    g_fAngleControlOut = 0;
    return;//提前结束本函数
  }
  
  g_fAngleControlOut = CAR_ANGLE_NOW * ANGLE_CONTROL_P + Angle_dot* ANGLE_CONTROL_D*0.1;
  
  if(g_fAngleControlOut>ANGLE_CONTROL_OUT_MAX) 
    g_fAngleControlOut=ANGLE_CONTROL_OUT_MAX;
  else if(g_fAngleControlOut<ANGLE_CONTROL_OUT_MIN)
    g_fAngleControlOut=ANGLE_CONTROL_OUT_MIN;

}


