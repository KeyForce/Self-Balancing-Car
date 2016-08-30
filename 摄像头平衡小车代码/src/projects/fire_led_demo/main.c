//sysinit()有printf()对UART的重定向  利用printf可以直接输出任意长度的十进制的数值
//当提高BUS__CLK的频率的时候图像上的噪点全部消失了---k60_fire.h中修改时钟频率
//    PTB0-PTB7   Y0-Y7
//    PTD12       PCLK
//    PTA24       VSYN
//    PTD13       HREF
//    PTE10       SCL
//    PTE12       SDA
//    PTD15       FODD
#include "common.h"
#include "include.h"
#include "car.h"
#include "dma.h"
extern s16  Gyro_Angle0;//陀螺仪0偏值
extern s16  J_Angle0;//加速度0偏值
extern s16  ANGLE_SET;//初始设定的角度值
extern float g_fAngleControlOut;//角度输出直接用来控制电机
extern float KP_Angle;
extern float KD_Angle;//参数
extern u16 count;
extern float g_fSpeedControlOutNew;
extern float g_fLeftMotorOut,g_fRightMotorOut; 
extern float g_fSpeedControlOut;//速度的输出
extern float SPEED_CONTROL_P;//
extern float SPEED_CONTROL_I;//速度控制I参数 
extern float SPEED_CONTROL_D;

extern s16   g_fLeftDirectionSigma;
extern s16   g_fRightDirectionSigma;
/***********摄像头参数**************/
extern u8  VSYN_Flag;//场完成标志
extern u8  VSYN1_FLAG;//二值化处理标志位
u8 Buffer1[ROW][COL]={0};//图像存储
extern u8 SampleFlag;
volatile u32 rowCnt=0;//行计数
u16  speed1=0;
extern u16 count;
extern u16 middle_num;
extern u16 row_qian;
extern u16 turn_left;
extern u16 turn_right;
extern float DIR_CONTROL_P;
extern float DIR_CONTROL_D;
extern u8    shiziflag;
extern u16   shizinum;
float          PreDirection=0;
float          PreDirection1=0;
/*******************寻路径******************/
/************··*************/
void EXTI_Init();//10ns;
void set_speed(void);
void  main(void)
{  
    DisableInterrupts;//关中断初始化
    FTM_PWM_init(FTM0, CH4, 20000,0);//电机1正  PTD4  
    FTM_PWM_init(FTM0, CH5, 20000,0);//电机1反  PTD5
    FTM_PWM_init(FTM0, CH6, 20000,0);//电机2正  PTD6
    FTM_PWM_init(FTM0, CH7, 20000,0);//电机2反  PTD7  20khz

    gpio_init(PORTA,14,GPO,1);//工作状态灯
    gpio_init(PORTA,16,GPO,1);
    gpio_init(PORTA,15,GPO,1);
    gpio_init(PORTA,17,GPO,1);
    gpio_init(PORTC,0,GPO,1);
    gpio_init(PORTC,1,GPO,1);//速度快前瞻给远一点  速度慢前瞻给近一点
    
    //速度设置初始化
    gpio_init(PORTC,13,GPI_DOWN,0);
    gpio_init(PORTC,12,GPI_DOWN,0);
    gpio_init(PORTC,11,GPI_DOWN,0);
    gpio_init(PORTC,10,GPI_DOWN,0);
    gpio_init(PORTC,9,GPI_DOWN,0);
    gpio_init(PORTC,8,GPI_DOWN,0);
    gpio_init(PORTC,7,GPI_DOWN,0);
    gpio_init(PORTC,6,GPI_DOWN,0);
    /**********变量初始化**************/
    KP_Angle=11.5;//11.89不要大于
    KD_Angle=1.25;//1.5  直立积分小的同时一定要调节减小速度的比例相
    SPEED_CONTROL_P=1.5;//4.2比例越小下坡越稳但是速度太快越大下坡容易抖动
    SPEED_CONTROL_I=0.95;//0.25越积分小上坡越容易
    ANGLE_SET=0;//初始值一般设定为0
    uart_init(UART1,9600);
    EXTI_Init();
    pit_init_ms(PIT0,1);//1ms的定时周期
    set_irq_priority(PIT0+68,14);//PIT中断
    adc_init(ADC1,SE4a);//PE0 4a  Gyro角速度  
    adc_init(ADC1,SE5a);//PE1 5a  J角度       
    adc_init(ADC1,SE6a);//PE2   保留
    adc_init(ADC1,SE7a);//PE3   保留
    /***设置速度*********/
   
   /*******************正交编码***********************/
    FTM_QUAD_Init1(FTM1, CH0);//PA8   左
    FTM_QUAD_Init1(FTM2, CH0);//PA10  右
    
    Gyro_Angle0=ad_ave(ADC1,SE4a,ADC_12bit,200); //获得0偏值 角速度0偏转
    J_Angle0=ad_ave(ADC1,SE5a,ADC_12bit,200); //获得0偏值    直接读取角度
    shizinum=0;
    set_speed();
    VSYN_Flag=0;
    rowCnt=0;
    count=0;
    EnableInterrupts;//开中断进入工作
    while(1)
    { 
        if(VSYN_Flag==1)//一场16.64ms
         {   
            gpio_set(PORTA,14,0);//工作状态参
            VSYN1_FLAG = 0;
            VSYN_Flag=0;
            ImageProc();
            gpio_set(PORTA,16,0);
            rowCnt=0;
            SampleFlag=0;
            enable_irq(PORTA + 87);
            enable_irq(PORTD + 87);
            
         }
    }
}

void EXTI_Init()
{
    //----初始化外部中断---//
    exti_init(PORTD, 13, rising_down);      //HREF----PORTD13 行端口外部中断初始化 ，上升沿触发中断，内部下拉
    exti_init(PORTA, 24, rising_down);      //VSYN----PORTA24 场端口外部中断初始化 ，上升沿触发中断，内部下拉
    set_irq_priority(PORTD+87,10);//中断向量,优先级行中断
    set_irq_priority(PORTA+87,2);//场中断
}
void set_speed(void)
{
  
     if(gpio_get(PORTC,9)==1)//速度5档   直立加硬过坡道
    {
         row_qian=65;//250一下76的前瞻   260,270以上速度需加前瞻
         speed1=320;
         DIR_CONTROL_D=1.98;
         DIR_CONTROL_P=1.53;
         PreDirection=DIR_CONTROL_P;
         PreDirection1=4.5;
         KP_Angle=11.5;//11.89不要大于
         KD_Angle=1.43;//1.5  直立积分小的同时一定要调节减小速度的比例相
    } 
    
    
    
    if(gpio_get(PORTC,13)==1)//速度1档
    {
         row_qian=65;//250一下76的前瞻   260,270以上速度需加前瞻
         speed1=250;
         DIR_CONTROL_D=1.84;
         DIR_CONTROL_P=1.4;
         PreDirection=DIR_CONTROL_P;
         PreDirection1=1.5;
    }
    if(gpio_get(PORTC,12)==1)//速度2档
    {
         row_qian=65;//250一下76的前瞻   260,270以上速度需加前瞻
         speed1=280;
         DIR_CONTROL_D=1.84;
         DIR_CONTROL_P=1.4;
         PreDirection=DIR_CONTROL_P;
         PreDirection1=1.8;
    }
    if(gpio_get(PORTC,11)==1)//速度3档
    {
         row_qian=60;//
         speed1=300;
         DIR_CONTROL_D=1.96;
         DIR_CONTROL_P=1.85;
         PreDirection=DIR_CONTROL_P;
         PreDirection1=1.8;
       
    }
    if(gpio_get(PORTC,10)==1)//速度4档
    {
         row_qian=60;//250一下76的前瞻   260,270以上速度需加前瞻
         speed1=320;
         DIR_CONTROL_D=1.98;
         DIR_CONTROL_P=1.53;
         PreDirection=DIR_CONTROL_P;
         PreDirection1=4.5;
    }    
    if(gpio_get(PORTC,8)==1)//起跑线
    {
       shiziflag=1;
    } 
    if(gpio_get(PORTC,7)==1)//人字左转
    {
       shizinum=40;
    }
    if(gpio_get(PORTC,6)==1)//人字右转
    {
       
       shizinum=220;
    }
     if((gpio_get(PORTC,12)==1)&&(gpio_get(PORTC,13)==1))//1.2
    {    
         row_qian=55;//250一下76的前瞻   260,270以上速度需加前瞻
         speed1=330;
         DIR_CONTROL_D=1.98;
         DIR_CONTROL_P=1.53;
         PreDirection=DIR_CONTROL_P;
         PreDirection1=4.0;
    }
     if((gpio_get(PORTC,11)==1)&&(gpio_get(PORTC,12)==1))//2.3
    {    
         row_qian=50;//250一下76的前瞻   260,270以上速度需加前瞻
         speed1=340;
         DIR_CONTROL_D=1.98;
         DIR_CONTROL_P=1.53;
         PreDirection=DIR_CONTROL_P;
         PreDirection1=4.0;
    }
     if((gpio_get(PORTC,11)==1)&&(gpio_get(PORTC,10)==1))//3.4
    {    
         row_qian=45;//250一下76的前瞻   260,270以上速度需加前瞻
         speed1=360;
         DIR_CONTROL_D=2.18;
         DIR_CONTROL_P=1.63;
         PreDirection=DIR_CONTROL_P;
         PreDirection1=4.0;
    }
     if((gpio_get(PORTC,10)==1)&&(gpio_get(PORTC,9)==1))//4.5
    {    
         row_qian=40;//250一下76的前瞻   260,270以上速度需加前瞻
         speed1=380;
         DIR_CONTROL_D=2.38;
         DIR_CONTROL_P=1.63;
         PreDirection=DIR_CONTROL_P;
         PreDirection1=4.0;
    }
}

