#include "car.h"
#include  "math.h"
/**************直立相关参数****************/
s16  Gyro_Angle0=0;//陀螺仪0偏值
s16  J_Angle0=0;//加速度0偏值在初始的时候测定
float  KP_Angle=0;//角度控制P参数
float  KD_Angle=0;//角度控制D参数
s16  ANGLE_SET=0;//初始设定的角度值
s16  J_Angle_Num;
s16  Gyro_Angle_Num;
float   g_fAngleControlOut;//直立输出
/**************直立相关参数***************/

/************速度相关参数***************/
float CAR_SPEED_SET=0;//速度设定的初值
s16 g_fCarSpeed;//车的速度
s16 g_fSpeedControlIntegral=0;//积分部分
float g_fSpeedControlOutOld=0;//上一次的速度
float g_fSpeedControlOutNew=0;//当前的速度

s16 g_nLeftMotorPulse=0;
s16 g_nRightMotorPulse=0;
s16 g_LeftMotorPulseSigma=0;//左轮的脉冲计数
s16 g_RightMotorPulseSigma=0;//右轮的脉冲计数

float  g_fLeftMotorOut=0;
float  g_fRightMotorOut=0;
float g_fSpeedControlOut=0;//速度的输出
float SPEED_CONTROL_P=0;//
float SPEED_CONTROL_I=0;//速度控制I参数 
float SPEED_CONTROL_D=0;
u16   g_nSpeedControlPeriod=0,g_SpeedControlCount=0;
/***********速度相关参数**************/
/***********方向控制参数*************/
float g_DirectionControlOut;//方向输出
float g_fDirectionControlOutNew,g_fDirectionControlOutOld;//用于缓冲方向的输出
u8 g_nDirectionControlCount,g_nDirectionControlPeriod;
float DIR_CONTROL_P=1.55;//控制外径轮
float DIR_CONTROL_D=1.83;//0.35方向的D越大有利于拐S弯而且有利于贴内  控制内径轮
float DIR_CONTROL_I=0;   //180的速度对应  0.85  0.80
s16   g_fLeftDirectionSigma;
s16   g_fRightDirectionSigma;
extern u8 Buffer1[ROW][COL];
extern u32 rowCnt;
s32 zuoPos = 0;          //左边界位置[有符号数]
s32 youPos = 0;          //右边界位置[有符号数]
u8 zuoEdgeFlag = 0;      //右边界确认标志，找到后边界置位
u8 time;   //左边界确认标志，找到后边界置位
u8 youEdgeFlag = 0;
u8  FLAG_YOU=0;
u8  FLAG_ZUO=0;
u16  capture[10]={0};
u16  zuo=0;
u16  you=0;
u8  first_row=0;
float   flag_left=0;
float   flag_right=0;
u8   FLAG1=0;
u8   FLAG2=0;
u8   hinder_left=0;
u8   hinder_right=0;
u16   posleft=0;
u16   posright=0;
u8   hinder_left1=0;
u8   hinder_right1=0;
u8   hinder_right2=0;
u8   middle=0;
u8   flag_you=0;


u16  shizileft=0;
u16  shiziright=0;
u8   zuoflag=0;
u8   youflag=0;
u8   shiziflag=0;
u8   row_qian=0;
u16  shizinum=0;
s16   turn_left=0;
s16   turn_right=0;
extern u16 speed1;

u8  start_run=0;
u16 middle_num=0;

u8  wanflag1=0;
u8  wanflag2=0;
u16 poszuo=0;
u16 posyou=0;

int Gyro_numcha=0;
extern u16 count;
extern float  PreDirection;
extern float  PreDirection1;
float abs(float __x);
/*******获取角度的模拟值*****/
void Get_AD_angle()
{
       u16 Gyro_Angle,J_Angle;
       Gyro_Angle=ad_ave(ADC1,SE4a,ADC_12bit,70); //50均值滤波;之前用的是10次滤波。现在改为了100次的滤波（摄像头的问题好像是出现在这里)
       J_Angle=ad_ave(ADC1,SE5a,ADC_12bit,70); //均值滤波50;
       Gyro_Angle_Num =Gyro_Angle0 - Gyro_Angle;//陀螺仪角速度
       J_Angle_Num=J_Angle0-J_Angle;//角度
       Gyro_numcha=Gyro_Angle_Num;
}
//*************角度控制函数*******//
void Angle_Control()//角度PD控制算法
{
       float fValue;
       Get_AD_angle();
       fValue=(ANGLE_SET-J_Angle_Num)*KP_Angle+(Gyro_Angle_Num)*KD_Angle; //PD控制        
       if(fValue>Angle_Out_Max)  fValue = Angle_Out_Max;//上下限可以不要
       if(fValue<Angle_Out_Min)   fValue =Angle_Out_Min;	         
       g_fAngleControlOut=fValue;
      
   
}
/****************速度控制**********************/


void SpeedControl(void)
{
    float fDelta;
    float fP,fI,fD;
    static float Error_Error;
    g_fCarSpeed=(g_LeftMotorPulseSigma+g_RightMotorPulseSigma)/2;//车的速度
    g_LeftMotorPulseSigma=g_RightMotorPulseSigma=0;
    
    fDelta = CAR_SPEED_SET - g_fCarSpeed;//误差
   
    fP = fDelta * SPEED_CONTROL_P;//比例部分 
    fI = fDelta * SPEED_CONTROL_I;//积分部分
    g_fSpeedControlIntegral += fI;//积分求和
    fD=( fDelta-Error_Error)* SPEED_CONTROL_D;
    fD=0;
    if(g_fSpeedControlIntegral>=100){g_fSpeedControlIntegral=100;}
    if(g_fSpeedControlIntegral<=(-100)){g_fSpeedControlIntegral=-100;}//积分饱和处理
    g_fSpeedControlOutOld = g_fSpeedControlOutNew;
    g_fSpeedControlOutNew = fP + g_fSpeedControlIntegral + fD;//设置这个的目的就是为了输出平滑的速度
    if(g_fSpeedControlOutNew>CAR_SPEED_SET)
    {
       g_fSpeedControlOutNew=CAR_SPEED_SET;
    }
    Error_Error=fDelta;

}   
//因为MOTOROUT是5ms执行一次 所以100ms的速度变化被分割成20个5ms的调节过程
void SpeedControlOutput(void)//速度平滑函数  作用就是使速度慢慢的增加到调定的值以致中间过程无抖动控制比较平滑 
{
    float fValue;
    fValue = g_fSpeedControlOutNew - g_fSpeedControlOutOld;//NEW和OLD都是100ms才更新一次 
    g_fSpeedControlOut = fValue * (g_nSpeedControlPeriod + 1) / SPEED_CONTROL_PERIOD + g_fSpeedControlOutOld;
}
void GetMotorspeed(void)//获取脉冲计数 5ms采样一次采样100ms
//因为AB相只用了一相,只起计数的作用并不能判断方向
//判断方向是根据g_fLeftMotorOut最后的输出来决定的
{
g_nRightMotorPulse=FTM1_CNT;//右
g_nLeftMotorPulse =FTM2_CNT;//左
FTM2_CNT=0;//右轮前进编码器的值为负后退都为负
FTM1_CNT=0;//左轮前进编码器的值为正后退为正                                                                   
       if(g_fLeftMotorOut<0)  g_nLeftMotorPulse = -g_nLeftMotorPulse;
       if(g_fRightMotorOut<0)	g_nRightMotorPulse = -g_nRightMotorPulse;//为什么改成小于0就可以了呢		
g_LeftMotorPulseSigma+=g_nLeftMotorPulse;
g_RightMotorPulseSigma+=g_nRightMotorPulse;

}
/*************方向控制***************************/
void DirectionControl(void)
{
      float  fValue,D_Error=0; 
      float RL_Value,fDValue,fPValue,fIValue=0; 
      RL_Value=115-capture[9];
      g_fDirectionControlOutOld = g_fDirectionControlOutNew; 
      fDValue = DIR_CONTROL_D*(RL_Value-D_Error);//(RL_Value-D_Error);//加了补偿
      fPValue=RL_Value*DIR_CONTROL_P;//比例部分
      fIValue+=RL_Value;
      fIValue=fIValue * DIR_CONTROL_I;
      D_Error=RL_Value;
      fValue   = fPValue+fDValue;//-fD_g*Gyro_Angle_Num;//-(CAR_SPEED_SET*RL_Value)/1000;
      g_fDirectionControlOutNew = fValue;
       
}
void DirectionControlOutput(void)
{
  float fValue;
  fValue = g_fDirectionControlOutNew - g_fDirectionControlOutOld;
  g_DirectionControlOut =fValue * (g_nDirectionControlPeriod + 1)/DIRECTION_CONTROL_PERIOD+ g_fDirectionControlOutOld;
}

//************设置PWM****************************//
void SetMotorPWM(float L_Motor_PWMOUT,float R_Motor_PWMOUT) //4前5后 6后7前
{          
       if (L_Motor_PWMOUT<0)
  	  {
     	     FTM_PWM_Duty(FTM0,CH4,(int)(-L_Motor_PWMOUT));	//4,5
             FTM_PWM_Duty(FTM0,CH5,0);
          }
	else
     	{
	     FTM_PWM_Duty(FTM0,CH5,(int)(L_Motor_PWMOUT));	//5,4
             FTM_PWM_Duty(FTM0,CH4,0);
     	}
	if (R_Motor_PWMOUT<0)
     	{
	     FTM_PWM_Duty(FTM0,CH7,(int)(-R_Motor_PWMOUT));//7,6	
             FTM_PWM_Duty(FTM0,CH6,0);
     	}
  	else
      	{
	      FTM_PWM_Duty(FTM0,CH6,(int)(R_Motor_PWMOUT));//6,7	
              FTM_PWM_Duty(FTM0,CH7,0);
      	}	
  
  
}
/********************设置死区电压控制***********************/
void MotorSpeedOut(void) 
{  
    float L_Motor_PWM,R_Motor_PWM;
    L_Motor_PWM=g_fLeftMotorOut;
    R_Motor_PWM=g_fRightMotorOut;
    
     if(L_Motor_PWM>0) 	 	//
	  L_Motor_PWM+=MOTOR_OUT_DEAD_VAL;
  	else 	
	  L_Motor_PWM-=MOTOR_OUT_DEAD_VAL;//电机死区的控制   一般设置为0
    
	if(R_Motor_PWM>0)	 
	  R_Motor_PWM+=MOTOR_OUT_DEAD_VAL;
	else   
	  R_Motor_PWM-=MOTOR_OUT_DEAD_VAL;	
	if( L_Motor_PWM>1200)   L_Motor_PWM=1200;//饱和处理
	if( L_Motor_PWM<-1200)	 L_Motor_PWM=-1200;
	if( R_Motor_PWM>1200)   R_Motor_PWM=1200;
	if( R_Motor_PWM<-1200) 	 R_Motor_PWM=-1200; //判断是否超出最大占空比	
	SetMotorPWM(L_Motor_PWM,R_Motor_PWM);	//设置电机的输出PWM
        
	
  }
/*******************最后的输出**************/
void MotorOutput(void) 
{ 
  float fLeft, fRight;

  fLeft  = g_fAngleControlOut - g_fSpeedControlOut + g_DirectionControlOut; //速度角度
  fRight = g_fAngleControlOut - g_fSpeedControlOut - g_DirectionControlOut; //速度角度之和
  g_fLeftMotorOut = fLeft; 
  g_fRightMotorOut= fRight; 
  MotorSpeedOut();
}
 void ImageProc()//赛道宽度150   中心值125  閥值140
{
   u32 i=0,j=0,m=0;
   for(i=39;i>=30;i--)   //90-99  
     {
          for(j=130;j>5;j--)  
          {
            if(Buffer1[i+row_qian][j]<140) 
            {
              zuoPos = j;
              FLAG_ZUO=1;
              break;
            }
            else
            {
               zuoPos = j;//未搜索到边界
               continue;//继续搜索下一次
            }
          } 
         for(j=130;j<COL-1;j++)          
          {
                if(Buffer1[i+row_qian][j]<140)   
                {
                  youPos = j;//偏离中间位置的值
                  FLAG_YOU=1;
                  break;
                }
                else
                {
                  youPos = j;  //未搜索到边界
                  continue;//继续下一次搜索
                }
           }
          if((FLAG_ZUO==1)&&(FLAG_YOU==1))
          {
          
               FLAG_ZUO=0;
               FLAG_YOU=0;
               capture[39-i]=(zuoPos+youPos)/2;
              if(capture[39-i]!=130)
              {
                middle++;
              }
               if(39-i==9)
               {
                  zuo=zuoPos;
                  you=youPos;
               }//取出被捕线位置
          }
          else
          {
             
               if((FLAG_ZUO==1)&&(FLAG_YOU==0))//左边搜索到 即右转弯 偏差大于0   
               {
                    if((zuoPos>50)&&(zuoPos<60))
                        {
                         
                           capture[39-i]=(zuoPos+280)/2;//280  需要进行调节 考虑这个切内
                        }
                        if((zuoPos>60)&&(zuoPos<70))
                        {
                          
                           capture[39-i]=(zuoPos+275)/2;//280  需要进行调节 考虑这个切内
                        }
                        if((zuoPos>70)&&(zuoPos<80))
                        {
                         
                         capture[39-i]=(zuoPos+270)/2;
                        }
                        if((zuoPos>80)&&(zuoPos<90))
                        {
                          
                           capture[39-i]=(zuoPos+265)/2;//上面几句不起作用
                        }
                        if((zuoPos>90)&&(zuoPos<100))
                        {
                           capture[39-i]=(zuoPos+270)/2;
                        }
                        else
                        {
                            capture[39-i]=(zuoPos+265)/2;
                        }
               }  
               if((FLAG_ZUO==0)&&(FLAG_YOU==1))//右边搜索到了 即左转弯  且偏差小于0
               {
               
                  if((youPos>130)&&(youPos<140))
                  {
                     capture[39-i]=(youPos+40)/2;
                  }
                 if((youPos>140)&&(youPos<150))
                  {
                     capture[39-i]=(youPos+38)/2;
                  }
                 if((youPos>150)&&(youPos<160))
                  {
                     capture[39-i]=(youPos+28)/2;
                  }
                 if((youPos>170)&&(youPos<180))
                  {
                     
                     capture[39-i]=(youPos+27)/2;
                  }
                 if((youPos>180)&&(youPos<190))
                  {
                    
                     capture[39-i]=(youPos+15)/2;
                  }
                 if((youPos>200)&&(youPos<210))
                  {
                  
                     capture[39-i]=(youPos-10)/2;//上面几句不起作用
                  }
                 if((youPos>210)&&(youPos<220))
                  {
                   
                     capture[39-i]=(youPos-30)/2;
                  }
                 else
                 {
                     capture[39-i]=(youPos-40)/2;
                 }
                 
               }
               if((FLAG_ZUO==0)&&(FLAG_YOU==0))//十字弯
               {
                    
                     for(m=130;m>1;m--)
                       {
                          if(Buffer1[60][m]<140) 
                            {
                               shizileft = m;
                               zuoflag=1;
                               break;
                            }
                          else
                            {
                                shizileft = m;  //未搜索到边界
                                continue;//继续搜索下一次
                            }
                       }
          
          
          
          
                    for(m=130;m<COL-5;m++)
                    {
                      if(Buffer1[60][m]<140) 
                      {
                        shiziright = m;
                        youflag=1;
                        break;
                      }
                      else
                      {
                         shiziright = m;  //未搜索到边界
                         continue;//继续搜索下一次
                      }
                    }
                    if((zuoflag==1)&&(youflag==1)&&(shizileft==130)&&(shiziright==130))
                    {
                      capture[9]=130;
                    }
                    if((zuoflag==1)&&(youflag==1)&&(shizileft!=130)&&(shiziright!=130))
                    {
                         capture[9]=(shizileft+shiziright)/2;
                    }
                    else
                    {
                      capture[9]=115;
                    }
                    shiziright=0;
                    shizileft=0;
                    zuoflag=0;
                    youflag=0;
                    m=0;
                     
               FLAG_ZUO=0;
               FLAG_YOU=0;   
          }     
      }
      FLAG_ZUO=0;
      FLAG_YOU=0;
     }
/***************************************************************************/
/************************************************************/
     DIR_CONTROL_P=PreDirection;
/*******************************下面是越界处理************************/
      if(capture[9]==130)
      {
           for(i=130;i>110;i--)
          {
            if(Buffer1[30+row_qian][i]>140)//左搜最多全黑允许超过20个点进行越界处理
            {
              posleft = i;
              hinder_left=1;
              break;
            }
            else
            {
               posleft = i;  //未搜索到边界最多搜10个点越界处理
               continue;//继续搜索下一次
            }
          }
          
          
          
          
          for(i=130;i<150;i++)//右搜最多搜20个点
          {
            if(Buffer1[30+row_qian][i]>140) 
            {
              posright = i;
              hinder_right=1;
              break;
            }
            else
            {
               posright = i;  //未搜索到边界
               continue;//继续搜索下一次
            }
          }
          if((hinder_left==1)&&(hinder_right==0))//左边搜索到白边  
          {
              
              if(130-posleft<20)
              {
               capture[9]=40;
            
              }
                
          }else  if((hinder_left==0)&&(hinder_right==1))
          {
              if(posright-130<20)
              {
               capture[9]=220;
               
              }
          }else if((hinder_left==0)&&(hinder_right==0))
          {
               
               
                for(i=130;i>115;i--)
                          {
                           if(Buffer1[60][i]>140)//100左搜最多全黑允许超过10个点进行越界处理
                            {
                              posleft = i;
                              hinder_left=1;
                              break;
                            }
                            else
                            {
                               posleft = i;  //未搜索到边界最多搜10个点越界处理
                               continue;//继续搜索下一次
                            }
                          }
             
                            for(i=130;i<145;i++)//右搜最多搜十个点
                            {
                              if(Buffer1[60][i]>140)//100 
                              {
                                posright = i;
                                hinder_right=1;
                                break;
                              }
                              else
                              {
                                 posright = i;  //未搜索到边界
                                 continue;//继续搜索下一次
                              }
                            }
                         if((hinder_left==0)&&(hinder_right==0))
                         {
                             capture[9]=shizinum;
                             DIR_CONTROL_P=PreDirection1;
                         }
               
          }
      }
  /*************************人字***************************/
   
/*****************************起跑线的识别***************************/
       if(middle==10&&abs(capture[9]-124)<5)
       {
         for(j=80;j<170;j+=3)//每隔3列搜索一次
         {
              for(i=150;i<160;i++)
              {
                if(Buffer1[i][j]<140)
                {
                   FLAG1=1;
                   if(FLAG2==1)
                   {
                     start_run++;
                     FLAG2=0;
                   }
                }
                else
                {
                  FLAG2=1;
                  if(FLAG1==1)
                  {
                     FLAG1=0;
                     start_run+=1;
                  }
                }
              }
         }
       }
       if(start_run>=2&&count==8000&&shiziflag==1)//停车
       {
          CAR_SPEED_SET=0;
          delayms(50);
          FTM_PWM_init(FTM0, CH4, 20000,0);//电机1正  PTD4  
          FTM_PWM_init(FTM0, CH5, 20000,0);//电机1反  PTD5
          FTM_PWM_init(FTM0, CH6, 20000,0);//电机2正  PTD6
          FTM_PWM_init(FTM0, CH7, 20000,0);//电机2反  PTD7  20khz
          DisableInterrupts;
       }
    
/**********************************************************************/
          FLAG1=0;
          FLAG2=0;
          flag_right=0;
          start_run=0;
          middle=0;
          flag_you=0;
          posleft=0;
          posright=0;
          hinder_right=0;
          hinder_right1=0;
          hinder_right2=0;
          hinder_left=0;
          hinder_left1=0;
}