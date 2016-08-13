/************************************************************************************
* keyOLED.c
* 描述:人机交互界面设计,按键与OLED结合使用。显示一些重要参数如：大体便是陀螺仪以及ccd的采集数据情况
*************************************************************************************/
#include  "Include_def.h"

#define FLASH_LED_SECTOR0   (27)//此处不同扇区取不同值
#define FLASH_LED_ADDR0     ((FLASH_LED_SECTOR0)*2048)

#define FLASH_LED_SECTOR1   (28)
#define FLASH_LED_ADDR1     ((FLASH_LED_SECTOR1)*2048)

#define FLASH_LED_SECTOR2   (29)
#define FLASH_LED_ADDR2     ((FLASH_LED_SECTOR2)*2048)

#define FLASH_LED_SECTOR3   (30)
#define FLASH_LED_ADDR3     ((FLASH_LED_SECTOR3)*2048)

#define FLASH_LED_SECTOR4   (31)
#define FLASH_LED_ADDR4     ((FLASH_LED_SECTOR4)*2048)

#define SHIFT   1  //移位
#define INC     2  //增加
#define DEC     3  //减少
#define ENTER   4  //确认
#define	FLASH_READ(sectorNo,offset,type)	(*(type *)((uint32)(((sectorNo)<<11) + (offset))))
int16 PARA1,PARA2,PARA3,PARA4,PARA5,PARA6,PARA7,PARA8,PARA9,PARA10,PARA11,PARA12,PARA13,PARA14,PARA15,PARA16,PARA17,PARA18,PARA19,PARA20,PARA21,PARA22,PARA23,PARA24; 
uint8  Dials;
int8  buf[4];
uint8 Xianbuf0[24];//显示的字符串为0到9
typedef union {
	uint8 Byte;
	struct {
        uint8 B0         :1;                                       // Flag Bit 0
        uint8 B1         :1;                                       // Flag Bit 1
        uint8 B2         :1;                                       // Flag Bit 2
        uint8 B3         :1;                                       // Flag Bit 3
        uint8 B4         :1;                                       // Flag Bit 4
        uint8 B5         :1;                                       // Flag Bit 5
        uint8 B6         :1;                                       // Flag Bit 6
        uint8 B7         :1;                                       // Flag Bit 7
	} Bits;
} BYTE;


//单文件所使用变量 
uint8 San_X,San_Y;   //闪烁光标的位置
uint8 LEDPage = 1;   //按键改变页面值（此值），然后根据值显示页面（该变量位于两个函数中）
uint8 SetOK = 0, Set_b = 0;    //Set_b KEY4键按下时，此值为1程序进入调试状态，调试状态时再按此键便退出调试状态
                               //即此值可让程序在更换页面以及调试参数键互相切换，SetOK起辅助实现此功能的作用
                               //
uint32 FlashWrite_flag=0;
//内部函数调用
uint8 ScanKey(void);
/*
 *函数名:void jiao0(int8 *p,uint8 beg,uint8 num) 
 *功能  :校零，使数据字符串显示位数相同
 *参数  :*p 待校准字符串
 *BY    :zf
 */
void jiao0(int8 *p,uint8 beg,uint8 num) 
{
 uint8 i,j;
 for (i=0;i<num-beg;i++)
 {
   for (j=beg+i+1;j>0;j--)
   {
    *(p+j)=*(p+j-1);
   }
    *p='0';
 }
}
/*
 *函数名:void ShowParameter(int16* para, uint8 LCD_Row, uint8* ParaName) 
 *功能  :将待改变的全局变量的值显示到oled上，为Xianbuf0附初值
 *参数  :para为添加的数字;LCD_Row显示的行数;参数显示的名字(最多8字符)
 *BY    :zf
 */
void ShowParameter(int16* para, uint8 LCD_Row, uint8* ParaName)
{
  uint8 i;
  LED_P6x8Str(4, LCD_Row, ParaName);  //x=1显示变量名
  i=Uint_to_S((uint16)*para,&buf[0]); 
  jiao0(&buf[0],i,4);
  LED_P6x8Str(90,LCD_Row,(uint8*)&buf[0]);//x=90显示数值
  Xianbuf0[LCD_Row+0]=buf[0];
  Xianbuf0[LCD_Row+6]=buf[1];
  Xianbuf0[LCD_Row+12]=buf[2];
  Xianbuf0[LCD_Row+18]=buf[3];        //四个参数设定
}
/*
 *函数名:void SetParameter(int16 *para, uint8 LCD_Row)
 *功能  :将设定的字符串转换为全局变量的值（int16）
 *参数  :*para 全局变量的地址，LCD_Row用于寻找设定的字符串
 *BY    :zf
 */
void SetParameter(int16 *para, uint8 LCD_Row) 
{
  *para = Xianbuf0[LCD_Row+0]-0x30; 	
  *para = *para * 10+(Xianbuf0[LCD_Row+6]-0x30);
  *para = *para * 10+(Xianbuf0[LCD_Row+12]-0x30);
  *para = *para * 10+(Xianbuf0[LCD_Row+18]-0x30);
}
/*
 *函数名:void LED_Bei(uint8 x,uint8 y) 
 *功能  :光点覆盖点一个字符
 *参数  :x为显示的横坐标0~122，y为页范围0～7
 *BY    :zf
 */
void LED_Bei(uint8 x,uint8 y) 
{		//显示光标
  uint8 buf[2];
  buf[0]=Xianbuf0[x-90+y];buf[1]='\0';
  LED_Light(x,y,&buf[0]);
}
/* *函数名:void LED_Clr(uint8 x,uint8 y) 
 *功能  :清除光标
 *参数  :x为显示的横坐标0~122，y为页范围0～7，ch要显示的字符
 *BY    :zf
 */
void LED_Clr(uint8 x,uint8 y) 
{		//清除光标
  uint8 buf[2];
  buf[0]=Xianbuf0[x-90+y];buf[1]='\0';
  LED_P6x8Str(x,y,&buf[0]);
}
/******************************************************************************/
/****************************4张页面设计***************************************/
/******************************************************************************/
/*
 *函数名:void SetPage1(void)
 *功能  :第一张页面，用作反应出系统的状态
 *参数  :无
 *BY    :zf
 */
void HOME(void) 
{
  if (Set_b==0)
  {
    LED_CLS(); 
    LED_P6x8Str(5,3,"       SetNeutral?");
  }
  else
  {
    SetOK = 1;
    LED_CLS();
  }
}
/*
 *函数名:void Angle(void)
 *功能  :第二张页面，安键改变直立时的变量；以及直立时的变量显示
 */
void Angle_ONE(void)
{    
  uint8 i;
  if (Set_b==0)
  {
    LED_CLS();
    for (i=0;i<24;i++)
      Xianbuf0[i]='\0';
    LED_P8x16Str(25,0,"<<Angle>>");
    ShowParameter(&PARA1, 2, "A_P:");
    ShowParameter(&PARA2, 3, "A_D*0.1:");
    ShowParameter(&PARA3, 4, "CAR_ANGLE_SET:");
    ShowParameter(&PARA4, 5, ":");
    ShowParameter(&PARA5, 6, ":");
    ShowParameter(&PARA6, 7, ":");
    
  }
  else          
  {   
    San_X=90;		
    San_Y=2;  
    LED_Bei(San_X,San_Y);  
    for (;;)
    {
      i = ScanKey();
      switch(i){
      case SHIFT:                            //移动光斑对应所要更该的变量
        LED_Clr(San_X,San_Y);
        San_X+=6;
        if (San_X>108) 
        {
          San_X=90; 
          San_Y++;
        }
        if (San_Y>7) //所能到达的行数
          San_Y=2;   //跳回
        LED_Bei(San_X,San_Y); 
        break;
      case INC:                              //改变光标所指变量对用参数的值0~9
        Xianbuf0[San_X-90+San_Y]++;                                             //光斑所对应的值
        if (Xianbuf0[San_X-90+San_Y]>0x39)                                      //San_X与LCD_low等价在一起
          Xianbuf0[San_X-90+San_Y]=0x30;
        buf[0]= Xianbuf0[San_X-90+San_Y];
        buf[1]='\0';
        LED_Bei(San_X,San_Y);
        break;
      case DEC: //减1
        Xianbuf0[San_X-90+San_Y]--;
        if (Xianbuf0[San_X-90+San_Y]<0x30)                
          Xianbuf0[San_X-90+San_Y]=0x39;                
        buf[0]= Xianbuf0[San_X-90+San_Y];
        buf[1]='\0';
        LED_Bei(San_X,San_Y);
        break;
      case ENTER: //确定
        SetParameter(&PARA1, 2);      //改变对应全局变量的值
        SetParameter(&PARA2, 3);      //第二个参数为所在行
        SetParameter(&PARA3, 4);
        SetParameter(&PARA4, 5);
        SetParameter(&PARA5, 6);
        SetParameter(&PARA6, 7);                  
        
        LPLD_Flash_SectorErase(FLASH_LED_ADDR0);//清除扇区
        
        LPLD_Flash_ByteProgram(FLASH_LED_ADDR0,    (uint32*)&PARA1, 4);//掉电后保存
        LPLD_Flash_ByteProgram(FLASH_LED_ADDR0+32, (uint32*)&PARA2, 4);
        LPLD_Flash_ByteProgram(FLASH_LED_ADDR0+64, (uint32*)&PARA3, 4);
        LPLD_Flash_ByteProgram(FLASH_LED_ADDR0+96, (uint32*)&PARA4, 4);  
        LPLD_Flash_ByteProgram(FLASH_LED_ADDR0+128,(uint32*)&PARA5, 4);
        LPLD_Flash_ByteProgram(FLASH_LED_ADDR0+160,(uint32*)&PARA6, 4); 
        LED_Clr(San_X, San_Y); 
        Set_b=0;   //退出设置状态
        return;
        break;
        
      default :  break;
      }
    }
  }
}
/*
 *函数名:void Speed_ONE(void)
 *功能  :页面3，安键改变跑动时的变量；以及跑动时的变量显示
 */
void Speed_ONE(void)     
{
  uint8 i;
  if (Set_b==0)
  {
    LED_CLS();
    for (i=0;i<24;i++)
      Xianbuf0[i]='\0';
    LED_P8x16Str(20,0,"<<Speed_ONE>>");
    ShowParameter(&PARA7, 2, "S_P/100:");
    ShowParameter(&PARA8, 3, "S_I/100:");
    ShowParameter(&PARA9, 4, "SpeedTarget:");
    ShowParameter(&PARA10,5, "p1:");
    ShowParameter(&PARA11,6, "P2:");
    ShowParameter(&PARA12,7, ":");
  }
  else          
  {   
    San_X=90;		
    San_Y=2;  
    LED_Bei(San_X,San_Y);  
    for (;;)
    {
      i = ScanKey();
      switch(i){
      case SHIFT:                            //移动光斑对应所要更该的变量
        LED_Clr(San_X,San_Y);
        San_X+=6;
        if (San_X>108) 
        {
          San_X=90; 
          San_Y++;
        }
        if (San_Y>7) 
          San_X=2;
        LED_Bei(San_X,San_Y); 
        break;
      case INC:                              //改变光标所指变量对用参数的值0~9
        Xianbuf0[San_X-90+San_Y]++;    //光斑所对应的值
        if (Xianbuf0[San_X-90+San_Y]>0x39)      //San_X与LCD_low等价在一起
          Xianbuf0[San_X-90+San_Y]=0x30;
        buf[0]= Xianbuf0[San_X-90+San_Y];
        buf[1]='\0';
        LED_Bei(San_X,San_Y);
        break;
      case DEC: //减1
        Xianbuf0[San_X-90+San_Y]--;
        if (Xianbuf0[San_X-90+San_Y]<0x30)
          Xianbuf0[San_X-90+San_Y]=0x39;
        buf[0]= Xianbuf0[San_X-90+San_Y];
        buf[1]='\0';
        LED_Bei(San_X,San_Y);
        break;
      case ENTER: //确定
        SetParameter(&PARA7, 2);      //改变对应全局变量的值
        SetParameter(&PARA8, 3);
        SetParameter(&PARA9, 4);
        SetParameter(&PARA10, 5);
        SetParameter(&PARA11, 6);
        SetParameter(&PARA12, 7);                  
        
        LPLD_Flash_SectorErase(FLASH_LED_ADDR1);//清除扇区
        
        LPLD_Flash_ByteProgram(FLASH_LED_ADDR1,    (uint32*)&PARA7, 4);
        LPLD_Flash_ByteProgram(FLASH_LED_ADDR1+32, (uint32*)&PARA8, 4);
        LPLD_Flash_ByteProgram(FLASH_LED_ADDR1+64, (uint32*)&PARA9, 4);
        LPLD_Flash_ByteProgram(FLASH_LED_ADDR1+96, (uint32*)&PARA10, 4);  
        LPLD_Flash_ByteProgram(FLASH_LED_ADDR1+128,(uint32*)&PARA11, 4);
        LPLD_Flash_ByteProgram(FLASH_LED_ADDR1+160,(uint32*)&PARA12, 4);  
        LED_Clr(San_X, San_Y); 
        Set_b=0;   //退出设置状态
        return;
        break;
        
      default :  break;
      }
    }
  }
}


/*
 *函数名:void Speed_TWO(void)
 *功能  :安键改变跑动时的变量；以及跑动时的变量显示
 */
void Speed_TWO(void)     
{
  uint8 i;
  if (Set_b==0)
  {
    LED_CLS();
    for (i=0;i<24;i++)
      Xianbuf0[i]='\0';
    LED_P8x16Str(20,0,"<<Speed_TWO>>");
    ShowParameter(&PARA19, 2, ":");
    ShowParameter(&PARA20, 3, ":");
    ShowParameter(&PARA21, 4, ":");
    ShowParameter(&PARA22, 5, ":");
    ShowParameter(&PARA23, 6, ":");
    ShowParameter(&PARA24, 7, ":");
  }
  else          
  {   
    San_X=90;		
    San_Y=2;  
    LED_Bei(San_X,San_Y);  
    for (;;)
    {
      i = ScanKey();
      switch(i){
      case SHIFT:                            //移动光斑对应所要更该的变量
        LED_Clr(San_X,San_Y);
        San_X+=6;
        if (San_X>108) 
        {
          San_X=90; 
          San_Y++;
        }
        if (San_Y>7) 
          San_X=2;
        LED_Bei(San_X,San_Y); 
        break;
      case INC:                              //改变光标所指变量对用参数的值0~9
        Xianbuf0[San_X-90+San_Y]++;    //光斑所对应的值
        if (Xianbuf0[San_X-90+San_Y]>0x39)      //San_X与LCD_low等价在一起
          Xianbuf0[San_X-90+San_Y]=0x30;
        buf[0]= Xianbuf0[San_X-90+San_Y];
        buf[1]='\0';
        LED_Bei(San_X,San_Y);
        break;
      case DEC: //减1
        Xianbuf0[San_X-90+San_Y]--;
        if (Xianbuf0[San_X-90+San_Y]<0x30)
          Xianbuf0[San_X-90+San_Y]=0x39;
        buf[0]= Xianbuf0[San_X-90+San_Y];
        buf[1]='\0';
        LED_Bei(San_X,San_Y);
        break;
      case ENTER: //确定
        SetParameter(&PARA19, 2);      //改变对应全局变量的值
        SetParameter(&PARA20, 3);
        SetParameter(&PARA21, 4);
        SetParameter(&PARA22, 5);
        SetParameter(&PARA23, 6);
        SetParameter(&PARA24, 7);                  
        
        LPLD_Flash_SectorErase(FLASH_LED_ADDR4);//清除扇区
        
        LPLD_Flash_ByteProgram(FLASH_LED_ADDR4,    (uint32*)&PARA19, 4);
        LPLD_Flash_ByteProgram(FLASH_LED_ADDR4+32, (uint32*)&PARA20, 4);
        LPLD_Flash_ByteProgram(FLASH_LED_ADDR4+64, (uint32*)&PARA21, 4);
        LPLD_Flash_ByteProgram(FLASH_LED_ADDR4+96, (uint32*)&PARA22, 4);  
        LPLD_Flash_ByteProgram(FLASH_LED_ADDR4+128,(uint32*)&PARA23, 4);
        LPLD_Flash_ByteProgram(FLASH_LED_ADDR4+160,(uint32*)&PARA24, 4);  
        LED_Clr(San_X, San_Y); 
        Set_b=0;   //退出设置状态
        return;
        break;
        
      default :  break;
      }
    }
  }
}






/*
 *函数名:void Direction(void) 
 *功能  :手动清除扇区页，清除后存在flash里的数据将全部删除
 */

void Direction(void)     
{
  uint8 i;
  if (Set_b==0)
  {
    LED_CLS();
    for (i=0;i<24;i++)
      Xianbuf0[i]='\0';
    LED_P8x16Str(15,0,"<<Direction>>");
    ShowParameter(&PARA13, 2, "D_P:");
    ShowParameter(&PARA14, 3, "D_D/100:");
    ShowParameter(&PARA15, 4, "L_R_OFF:");
    ShowParameter(&PARA16, 5, ":");
    ShowParameter(&PARA17, 6, ":");
    ShowParameter(&PARA18, 7, ":");
    
  }
  else          
  {   
    San_X=90;		
    San_Y=2;  
    LED_Bei(San_X,San_Y);  
    for (;;)
    {
      i = ScanKey();
      switch(i){
      case SHIFT:                            //移动光斑对应所要更该的变量
        LED_Clr(San_X,San_Y);
        San_X+=6;
        if (San_X>108) 
        {
          San_X=90; 
          San_Y++;
        }
        if (San_Y>7) 
          San_Y=2;
        LED_Bei(San_X,San_Y); 
        break;
      case INC:                              //改变光标所指变量对用参数的值0~9
        Xianbuf0[San_X-90+San_Y]++;    //光斑所对应的值
        if (Xianbuf0[San_X-90+San_Y]>0x39)      //San_X与LCD_low等价在一起
          Xianbuf0[San_X-90+San_Y]=0x30;
        buf[0]= Xianbuf0[San_X-90+San_Y];
        buf[1]='\0';
        LED_Bei(San_X,San_Y);
        break;
      case DEC: //减1
        Xianbuf0[San_X-90+San_Y]--;
        if (Xianbuf0[San_X-90+San_Y]<0x30)
          Xianbuf0[San_X-90+San_Y]=0x39;
        buf[0]= Xianbuf0[San_X-90+San_Y];
        buf[1]='\0';
        LED_Bei(San_X,San_Y);
        break;
      case ENTER: //确定
        SetParameter(&PARA13, 2);      //改变对应全局变量的值
        SetParameter(&PARA14, 3);
        SetParameter(&PARA15, 4);
        SetParameter(&PARA16, 5);
        SetParameter(&PARA17, 6);
        SetParameter(&PARA18, 7);                  
        
        LPLD_Flash_SectorErase(FLASH_LED_ADDR3);//清除扇区
        
        LPLD_Flash_ByteProgram(FLASH_LED_ADDR3,    (uint32*)&PARA13, 4);
        LPLD_Flash_ByteProgram(FLASH_LED_ADDR3+32, (uint32*)&PARA14, 4);
        LPLD_Flash_ByteProgram(FLASH_LED_ADDR3+64, (uint32*)&PARA15, 4);
        LPLD_Flash_ByteProgram(FLASH_LED_ADDR3+96, (uint32*)&PARA16, 4);  
        LPLD_Flash_ByteProgram(FLASH_LED_ADDR3+128,(uint32*)&PARA17, 4);
        LPLD_Flash_ByteProgram(FLASH_LED_ADDR3+160,(uint32*)&PARA18, 4);  
        LED_Clr(San_X, San_Y); 
        Set_b=0;   //退出设置状态
        return;
        break;
        
      default :  break;
      }
    }
  }
}
/******************************************************************************/
/****************************4张页面设计完毕！！！*****************************/
/******************************************************************************/
/*
 *函数名:uint8 ScanKey(void)
 *功能  :扫描安键,并且返回安键值对应值为1~4,0为没有键按下
 *参数  :无
 *BY    :zf
 */
uint8 ScanKey(void)
{
  uint8 Key;    
  BYTE key;          //结构体引用
  key.Byte=0xff;     //通过按不同的键可以改变此值（八位），从而做出判断
  key.Bits.B0=KEY1;
  key.Bits.B1=KEY2;
  key.Bits.B2=KEY3;
  key.Bits.B3=KEY4;
  Dials=key.Byte;
  if((key.Byte & 0x0f)!=0x0f)//1到4按键有一个按下 
  {
    for(int32 j=0;j<=10000;j++)
        {
          volatile uint8 i ;
          for(i=0;i<1;i++) {
           asm("nop");
           asm("nop");}
        }
    if (KEY4==0) 
      Key=ENTER;
    else if (KEY3==0)
      Key=DEC;
    else if (KEY2==0)
      Key=INC;
    else 
      Key=SHIFT;
    while((key.Byte & 0x0f)!=0x0f)//支持连续按键功能
    {
      key.Bits.B0=KEY1;
      key.Bits.B1=KEY2;
      key.Bits.B2=KEY3;
      key.Bits.B3=KEY4;
    }
    for(int32 j=0;j<=10000;j++)
        {
          volatile uint8 i ;
          for(i=0;i<1;i++) {
           asm("nop");
           asm("nop");}
        }
  }
  else 
    Key=0;
  return(Key);
}
/*
 *函数名:void ShowPage(void) 
 *功能  :读取页面值，并且显示相应的页面
 */
void ShowPage(void)
 {
    switch (LEDPage)
     {
       case 1: HOME();          break;
       case 2: Angle_ONE();     break;
       case 3: Speed_ONE();     break;
       case 4: Direction();     break;
       //case 5: Speed_TWO();     break;
       default :                break;
     } 
 }
/*
 *函数名:void SetLCD() 
 *功能  :扫描键值，并且读取页面值，通过oled显示页面
 *参数  :无
 *BY    :zf
 */
void SetLED(void)
{
  uint8 k;
  while(SetOK == 0)
  {
    k = ScanKey();
    switch(k)
    {
    case SHIFT:
      ShowPage();         break;
    case INC:
      if (LEDPage < 5)   //向前翻
        LEDPage++;
      else
        LEDPage = 1;
      ShowPage();         break;
    case DEC:
      if (LEDPage > 1)   //向后翻
        LEDPage--;
      else
        LEDPage = 4;
      ShowPage();         break;
    case ENTER:
      Set_b=1;           //进入画面操作
      ShowPage();         break;
    default :             break;
    }
  }
}
/*
 *函数名:void InitFlashLcd(void) 
 *功能  :初始化flash功能
 *参数  :无
 *BY    :zf
 */
void InitFlashLed(void) 
{
  LPLD_Flash_Init();
  FlashWrite_flag=FLASH_READ(FLASH_LED_SECTOR0,0,uint32);
  if(FlashWrite_flag!=0xffffffff)//flash若全部清除，则不读取flash，变量使用程序本身的变量值
   { 
      PARA1=FLASH_READ(FLASH_LED_SECTOR0,0,uint32);
      PARA2=FLASH_READ(FLASH_LED_SECTOR0,32,uint32);
      PARA3=FLASH_READ(FLASH_LED_SECTOR0,64,uint32);
      PARA4=FLASH_READ(FLASH_LED_SECTOR0,96,uint32);
      PARA5=FLASH_READ(FLASH_LED_SECTOR0,128,uint32);
      PARA6=FLASH_READ(FLASH_LED_SECTOR0,160,uint32);
      
      PARA7=FLASH_READ(FLASH_LED_SECTOR1,0,uint32);
      PARA8=FLASH_READ(FLASH_LED_SECTOR1,32,uint32);
      PARA9=FLASH_READ(FLASH_LED_SECTOR1,64,uint32);
      PARA10=FLASH_READ(FLASH_LED_SECTOR1,96,uint32);
      PARA11=FLASH_READ(FLASH_LED_SECTOR1,128,uint32);
      PARA12=FLASH_READ(FLASH_LED_SECTOR1,160,uint32);   
      
      PARA13=FLASH_READ(FLASH_LED_SECTOR3,0,uint32);
      PARA14=FLASH_READ(FLASH_LED_SECTOR3,32,uint32);
      PARA15=FLASH_READ(FLASH_LED_SECTOR3,64,uint32);
      PARA16=FLASH_READ(FLASH_LED_SECTOR3,96,uint32);
      PARA17=FLASH_READ(FLASH_LED_SECTOR3,128,uint32);
      PARA18=FLASH_READ(FLASH_LED_SECTOR3,160,uint32);
      
      PARA19=FLASH_READ(FLASH_LED_SECTOR4,0,uint32);
      PARA20=FLASH_READ(FLASH_LED_SECTOR4,32,uint32);
      PARA21=FLASH_READ(FLASH_LED_SECTOR4,64,uint32);
      PARA22=FLASH_READ(FLASH_LED_SECTOR4,96,uint32);
      PARA23=FLASH_READ(FLASH_LED_SECTOR4,128,uint32);
      PARA24=FLASH_READ(FLASH_LED_SECTOR4,160,uint32);
   }
}

