#ifndef _Include_Def_H_
#define _Include_Def_H_

/* 头文件包含 */
#include <common.h>
#include <math.h>

/*底层模块初始化*/
#include "HW_GPIO.h"
#include "HW_UART.h"
#include "HW_ADC.h"
#include "HW_FTM.h"

/*用户函数初始化*/
#include "Globals.h"
#include "Com_Def.h"

#include "Speed_PID.h"
#include "Angle_PID.h"
#include "PIT_ISR.h"
#include "SendData.h"
#include "OLED.h"

#include "Kalman_Filter.h"
#include "DataScope_DP.h"

#include "MOTOR.h"

#include "HW_FLASH.h"
#include "KeyOLED.h"
#include "Direction_PID.h"
#include "System_Init.h"
#endif