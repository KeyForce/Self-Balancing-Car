/*
**==============================================================================
** DEBUG.H:             -- by Dr. ZhuoQing, 2012-2-6
**
**  Description:
**
**==============================================================================
*/
#ifndef __DEBUG__
#define __DEBUG__
//------------------------------------------------------------------------------
#ifdef DEBUG_GLOBALS
    #define DEBUG_EXT
#else
    #define DEBUG_EXT extern
#endif // DEBUG_GLOBALS
//------------------------------------------------------------------------------
//==============================================================================
void DebugInit(void);


//------------------------------------------------------------------------------
#define DEBUG_START                 0xaa
#define DEBUG_START1                0xab
#define DEBUG_SENDARG               0xac        // From F8013 to PC
#define DEBUG_RECEARG               0xad        // From PC to F8013
#define DEBUG_SAVEARG               0xae
#define DEBUG_INITARG               0xaf
#define DEBUG_ADVALUE				0xb0
#define DEBUG_ENTERBOOTLOADER		0xb1
#define DEBUG_SETSPEED				0xb2		
#define DEBUG_STOPCONTROL			0xb3
#define DEBUG_STARTCONTROL			0xb4
#define DEBUG_RECEANDSAVEARG		0xb5

#define DEBUG_OK                    0xa5
#define DEBUG_ERROR                 0x5a
#define DEBUG_HELLO					0x55

#define DEBUG_WAIT_TIME				10
void DebugFirstStart(void);

void DebugSendArgument(void);
int DebugReceiveArgument(void);

DEBUG_EXT unsigned char g_nTime1MSStopFlag;
#define TIME1MS_INT_STOP 			g_nTime1MSStopFlag = 1
#define TIME1MS_INT_START			g_nTime1MSStopFlag = 0;
#define TIME1MS_INT_FLAG			g_nTime1MSStopFlag


//------------------------------------------------------------------------------
void DebugProcess(void);

//------------------------------------------------------------------------------
void SendDebugWord(void);
void SendDebugWordFloat(void);

#define DEBUG_WORD			11
DEBUG_EXT int g_nDebugWord[DEBUG_WORD];
DEBUG_EXT float g_fDebugWord[DEBUG_WORD];

//------------------------------------------------------------------------------
// AVERAGE Number is set intp g_nDebugWord
void GetADAverage(int nAverageNumber);

//------------------------------------------------------------------------------
DEBUG_EXT int g_nDebugStopFlag;
#define DEBUG_START_CONTROL			g_nDebugStopFlag = 0
#define DEBUG_STOP_CONTROL			g_nDebugStopFlag = 1
#define IF_DEBUG_STOP				g_nDebugStopFlag == 0
void DebugStartControl(void);

//------------------------------------------------------------------------------
void LEDFlash(void);
void LEDFlashInit(void);


//==============================================================================
//             END OF THE FILE : DEBUG.H
//------------------------------------------------------------------------------
#endif // __DEBUG__
