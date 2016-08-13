#ifndef _OLED_H_
#define _OLED_H_

#include "common.h"
#include "HW_GPIO.h"
#include "HW_LPTMR.h"

#define LED_SCLH  PTC16_O = 1// 时钟定义 
#define LED_SCLL  PTC16_O = 0

#define LED_SDAH  PTC17_O = 1//数据口D0
#define LED_SDAL  PTC17_O = 0

#define LED_RSTH  PTC18_O = 1//复位低能电平
#define LED_RSTL  PTC18_O = 0

#define LED_DCH   PTC19_O = 1
#define LED_DCL   PTC19_O = 0//偏置常低


void LED_Init(void);                                                            //初始化
void LED_CLS(void);                                                             //清屏程序
void LED_Set_Pos(uint8 x, uint8 y);                                             //设置坐标函数
void LED_WrDat(uint8 data);                                                     //写数据函数
void LED_P6x8Char(uint8 x,uint8 y,uint8 ch);                                    //显示一个6x8标准ASCII字符（即由48个亮点组成）
void LED_P6x8Str(uint8 x,uint8 y,uint8 ch[]);                                   //写入一组6x8标准ASCII字符串
void LED_Light(uint8 x,uint8 y,uint8 ch[]);                                     //覆盖一组6x8标准ASCII字符串
void LED_P8x16Str(uint8 x,uint8 y,uint8 ch[]);
void LED_P14x16Str(uint8 x,uint8 y,uint8 ch[]);                                  //
void LED_PXx16MixStr(uint8 x, uint8 y, uint8 ch[]);
void LED_PrintBMP(uint8 x0,uint8 y0,uint8 x1,uint8 y1,uint8 bmp[]);
void LED_Fill(uint8 dat);                                                       //整个屏幕写满
void LED_PrintValueC(uint8 x, uint8 y,int8 data);                               //将一个char型数转换成3位数进行显示
void LED_PrintValueI(uint8 x, uint8 y, int32 data);
void LED_PrintValueF(uint8 x, uint8 y, float32 data, uint8 num);                //将一个float型数转换成整数部分5位带小数和符号的数据并进行显示
void LED_PrintEdge(void);
void LED_Cursor(uint8 cursor_column, uint8 cursor_row);
void LED_PrintLine(void);
uint8 Uint_to_S(uint16 data, int8 *p);

void LED_INT_PRINT(uint8 x, uint8 y, uint8 ch[], int32 data);
void LED_FLOAT_PRINT(uint8 x, uint8 y, uint8 ch[], float32 data, uint8 num);
#endif