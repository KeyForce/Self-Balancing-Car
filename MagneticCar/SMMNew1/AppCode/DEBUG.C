/*
**==============================================================================
** DEBUG.C:             -- by Dr. ZhuoQing, 2012-2-6
**
**==============================================================================
*/

#define DEBUG_GLOBALS        1                       // Define the global variables
#include "DEBUG.H"
#include "F8013.H"
#include "LED.h"
#include "AS1.h"
#include "AD1.h"
#include "config.h"
#include "carsub.h"

//==============================================================================
void DebugInit(void) {
	DEBUG_START_CONTROL;
}

//------------------------------------------------------------------------------
void DebugFirstStart(void) {
	int i;
	unsigned char c;
	
	TIME1MS_INT_STOP;
	
	//--------------------------------------------------------------------------
	SendChar(DEBUG_START);
	SendChar(DEBUG_START1);
	
	for(i = 0; i < DEBUG_WAIT_TIME; i ++) {
		if(i & 1) LED_ON;
		else LED_OFF;
		if(ReceChar(&c) == 0) break;
	}
	if(i >= DEBUG_WAIT_TIME) {
		TIME1MS_INT_START;
		return;
	}
	if(c != DEBUG_START) {
		TIME1MS_INT_START;
		return;
	}
		
	if(DebugReceiveArgument() != 0) {
		TIME1MS_INT_START;
		return;
	}
	SendChar(DEBUG_START1);
	ArgumentSave();
	TIME1MS_INT_START;
}

//------------------------------------------------------------------------------
void DebugSendArgument(void) {
	int i, nSize;
	unsigned int nCheckNumber;
	
	unsigned char * p;
	unsigned char c;
	
	nSize = sizeof(g_Argument);
	p = (unsigned char *)&g_Argument;
	
	Delay1MS(100);
	nCheckNumber = 0;
	for(i = 0; i < nSize; i ++) {
		c = *(p + i);
		nCheckNumber += c;
		SendChar(c);
	}
	
	c = (unsigned char)(nCheckNumber >> 8);
	SendChar(c);
	c = (unsigned char)nCheckNumber;
	SendChar(c);
}

#define DEBUG_RECEIVE_LOOP			10
int DebugReceiveArgument(void) {
	int i, nSize, j;
	unsigned char * p, *p1;
	unsigned char c;
	unsigned int nCheckNumber, nReceiveCheckNumber;
	
	tagArgument argument;
	
	nSize = sizeof(g_Argument);
	p = (unsigned char *)&argument; //&g_Argument;
	
	nCheckNumber = 0;
	
	for(i = 0; i < nSize; i ++) {
		for(j = 0; j < DEBUG_RECEIVE_LOOP; j ++) {
			if(ReceChar(&c) == 0) break;
		}
		
		if(j >= DEBUG_RECEIVE_LOOP) return i + 1;
		*(p + i) = c;
		nCheckNumber += c;
	}
	
	for(j = 0; j < DEBUG_RECEIVE_LOOP; j ++) {
		if(ReceChar(&c) == 0) break;
	}
	if(j >= DEBUG_RECEIVE_LOOP) return 10;
	nReceiveCheckNumber = c;
	
	for(j = 0; j < DEBUG_RECEIVE_LOOP; j ++) {
		if(ReceChar(&c) == 0) break;
	}
	if(j >= DEBUG_RECEIVE_LOOP) return 11;
	nReceiveCheckNumber = (nReceiveCheckNumber << 8) + c;
	
	if(nReceiveCheckNumber != nCheckNumber) return 12;
		
	p1 = (unsigned char  *)&g_Argument;
	
	for(i = 0; i < nSize; i ++) 
		*(p1 + i) = *(p + i);
	
	
	return 0;
}

//------------------------------------------------------------------------------
void DebugProcess(void) {
	unsigned char c;
	int nReturn;
	unsigned char ucCheckNumber;
	float fSpeed;
	unsigned char * p;
	int i;
	
	//--------------------------------------------------------------------------
	if(ReceChar(&c)) return;
		
	//--------------------------------------------------------------------------
	TIME1MS_INT_STOP;
	
	if(c		== DEBUG_HELLO) 	SendChar(DEBUG_OK);
	else if(c	== DEBUG_SENDARG)	DebugSendArgument();
	else if(c	== DEBUG_RECEARG ||
		    c   == DEBUG_RECEANDSAVEARG) {
		nReturn = DebugReceiveArgument();
		if(nReturn == 0) {
			if(c == DEBUG_RECEANDSAVEARG) 
				ArgumentSave();
		}
		SendChar((unsigned char)nReturn);
	} else if(c == DEBUG_SAVEARG) {
		ArgumentSave();
		SendChar(DEBUG_OK);
	} else if(c == DEBUG_INITARG) {
		ArgumentInit();
		ArgumentSave();
		SendChar(DEBUG_OK);
	} else if(c	== DEBUG_ADVALUE) {
		SendChar(0x55);
		GetADAverage(1024);
		SendDebugWord();					
	} else if(c == DEBUG_ENTERBOOTLOADER) {
		SendChar(0x55);
		JUMP_BOOTLOADER;
	} else if(c == DEBUG_SETSPEED) {
		ucCheckNumber = 0;
		p = (unsigned char *)&fSpeed;
		for(i = 0; i < 4; i ++) {
			if(ReceChar(&c)) return;
			ucCheckNumber += c;
			*(p + i) = c;
		}
		
		if(ReceChar(&c)) return;
		if(c != ucCheckNumber) return;
		g_Argument.fCarSpeedSet = fSpeed;
		SendChar(0x55);
	} else if(c == DEBUG_STOPCONTROL) {
		DEBUG_STOP_CONTROL;
		CarControlStop();
		SendChar(0x55);
	} else if(c == DEBUG_STARTCONTROL) {
		DebugStartControl();
		SendChar(0x55);
	}

	
	TIME1MS_INT_START;

}

	
//==============================================================================
//				SEND DEBUG WORD
//------------------------------------------------------------------------------
void SendDebugWord(void) {
	int i;
	
	SendChar(0x55);
	for(i = 0; i < DEBUG_WORD; i ++) 
		SendWord((unsigned int)g_nDebugWord[i]);
}
void SendDebugWordFloat(void) {
	int i;
	
	for(i = 0; i < DEBUG_WORD; i ++) {
		if(g_fDebugWord[i] >= 0 && g_fDebugWord[i] < 0x7fff ||
		   g_fDebugWord[i] < 0 && g_fDebugWord[i] >= -0x7fff) 
	   		g_nDebugWord[i] = (int)g_fDebugWord[i];
		else if(g_fDebugWord[i] > 0x7fff) 		g_nDebugWord[i] = 0x7fff;
		else if(g_fDebugWord[i] < -0x7fff)		g_nDebugWord[i] = -0x7fff;
	}
	SendDebugWord();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// AVERAGE Number is set intp g_nDebugWord
void GetADAverage(int nAverageNumber) {
	long lnNumber[6];
	int i, j;
	
	
	AD_INT_STOP;                                // Stop AD Convert in 1ms interrupt subrouinte
	for(i = 0; i < 6; i ++) lnNumber[i] = 0;
	for(i = 0; i < nAverageNumber; i ++) {
		AD1_Measure(0);
		Delay100US(1);
  		GetInputVoltage();
		for(j = 0; j < 6; j ++) 
			lnNumber[j] += g_nInputVoltage[j];
			
	}
	AD_INT_START;                               // Restart AD convert in 1 ms interrupt.
	
	if(nAverageNumber <= 0) return;
	for(i = 0; i < 6; i ++) {
		g_nDebugWord[i] = (int)(lnNumber[i] / nAverageNumber);
	}
}

//==============================================================================
//				MAIN BOARD LED FLASH
//------------------------------------------------------------------------------
int g_nLEDFlag, g_nLEDCount;
#define LED_FLASH_RUN		80
#define LED_FLASH_STOP		160
#define LED_FLASH_STAND		20
void LEDFlashInit(void) {
	g_nLEDFlag = 0;
	g_nLEDCount = 0;
}

void LEDFlash(void) {
	int nCount;
	
	if(g_nTimeTestFlag) return;
	
	g_nLEDCount ++;
	if(g_nCarControlFlag) {
		if(g_nLEDCount >= LED_FLASH_RUN) {
			g_nLEDCount = 0;
			if(g_nLEDFlag == 0) 	g_nLEDFlag = 1;
			else 					g_nLEDFlag = 0;
		}
	} else {
		nCount = LED_FLASH_STOP;	
		if(g_fGravityAngle > CAR_STAND_ANGLE_MIN &&
	       g_fGravityAngle < CAR_STAND_ANGLE_MAX) nCount = LED_FLASH_STAND;

		if(g_nLEDCount >= nCount) {
			g_nLEDCount = 0;
			if(g_nLEDFlag == 0) 	g_nLEDFlag = 1;
			else 					g_nLEDFlag = 0;
		}
	}
	
	if(g_nLEDFlag) LED_ON;
	else LED_OFF;
}

//------------------------------------------------------------------------------	

void DebugStartControl(void) {
	DEBUG_START_CONTROL;
	g_nWaitCarStandCount = WAIT_CAR_STAND_COUNT;
	g_fGyroscopeAngleIntegral = 0;
	g_fSpeedControlIntegral = 0;
}


//==============================================================================
//                END OF THE FILE : DEBUG.C
//------------------------------------------------------------------------------
