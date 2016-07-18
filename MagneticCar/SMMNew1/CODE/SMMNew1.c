/** ###################################################################
**     Filename  : SMMNew1.C
**     Project   : SMMNew1
**     Processor : 56F8013VFAE
**     Version   : Driver 01.14
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2012-2-2, 11:06
**     Abstract  :
**         Main module.
**         This module contains user's application code.
**     Settings  :
**     Contents  :
**         No public methods
**
** ###################################################################*/
/* MODULE SMMNew1 */


/* Including needed modules to compile this module/procedure */
#include <intrinsics_56800e.h>
#include "Cpu.h"
#include "Events.h"
#include "LED.h"
#include "AS1.h"
#include "AD1.h"
#include "PWMC1.h"
#include "COUNTER1.h"
#include "COUNTER2.h"
#include "TI1.h"
#include "IFsh1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

//------------------------------------------------------------------------------
#include "F8013.h"
#include "CarSub.h"
#include "config.h"
#include "debug.h"



//------------------------------------------------------------------------------
#define LOOP_TIME			250
void main(void)
{
	unsigned int nLoop;
 /* Write your local variable definition here */
	//--------------------------------------------------------------------------
	
	//--------------------------------------------------------------------------
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

	//--------------------------------------------------------------------------
	//	Initialize MCU and control variables
	F8013Init();                                // Initialize the F8013
	CarSubInit();                               // Initialize the control algrithm
	LEDFlashInit();                             // Initialize LED Flash
	DebugInit();                                // Initialize debug global variable
		
	//--------------------------------------------------------------------------
	ArgumentLoad();                             // Load Argument from FLASH
	if(g_Argument.nInitFlag != INIT_FLAG_DEFAULT) {
		ArgumentInit();
		ArgumentSave();
		DebugFirstStart();
	}
	
	//--------------------------------------------------------------------------
	nLoop = 0;
	
	//--------------------------------------------------------------------------
//	g_fTestSpeedValue = MOTOR_OUT_MAX / 2;

		
	//--------------------------------------------------------------------------
	for(;;) {
		//----------------------------------------------------------------------
		WaitTime(1);		
		nLoop ++;
		
		//----------------------------------------------------------------------
		if(CAN_RECE) {
			DebugProcess();
		}

		//----------------------------------------------------------------------
		if(VOLTAGE_SET1 > 3000) 		DEBUG_STOP_CONTROL;
		if(VOLTAGE_SET2 > 3000) 		DebugStartControl();
		
		//----------------------------------------------------------------------
		LEDFlash();
		
		//----------------------------------------------------------------------
		if(nLoop >= LOOP_TIME) 
			nLoop = 0;
		else continue;

		//----------------------------------------------------------------------
		if(g_nDebugStopFlag == 0) {
			WaitCarStand();
			CheckCarStand();
		} else {
			CarControlStop();
		}
	

		//----------------------------------------------------------------------
		// Show the AD Result;
		g_fDebugWord[0] = g_fCarAngle * 10.0;
		g_fDebugWord[1] = g_fCarSpeed * 10.0;		
		g_fDebugWord[2] = g_fDirectionControlOut * 100.0;
		g_fDebugWord[3] = g_fSpeedControlOut * 100; 
		g_fDebugWord[4] = VOLTAGE_LEFT;
		g_fDebugWord[5] = VOLTAGE_RIGHT;
		g_fDebugWord[6] = VOLTAGE_GYRO;
		g_fDebugWord[7] = VOLTAGE_GRAVITY;//g_fSpeedControlIntegral * 10000; //VOLTAGE_GRAVITY;
		g_fDebugWord[8] = g_fCarSpeedSet * 10.0;
		
		if(g_nDebugStopFlag == 0) {
			g_fDebugWord[9] = g_fAngleControlOut * 100.0;
			g_fDebugWord[10] = g_fSpeedControlIntegral * 100.0; //VOLTAGE_SET2;
		} else {
			g_fDebugWord[9] = 9999;
			g_fDebugWord[10] = 9999;
		}

		SendDebugWordFloat();
//		GetADAverage(1024);
//		SendDebugWord();				
	
		//----------------------------------------------------------------------
#define SPEED_INC			1
#define SPEED_DEC			2
		if(IF_CAR_CONTROL) {
			if(g_fCarSpeedSet != g_Argument.fCarSpeedSet) {
				if(g_Argument.fCarSpeedSet > g_fCarSpeedSet) {
					g_fCarSpeedSet += SPEED_INC;
					if(g_Argument.fCarSpeedSet < g_fCarSpeedSet)
						g_fCarSpeedSet = g_Argument.fCarSpeedSet;
				} else {
					g_fCarSpeedSet -= SPEED_DEC;
					if(g_Argument.fCarSpeedSet > g_fCarSpeedSet)
						g_fCarSpeedSet = g_Argument.fCarSpeedSet;
				}
			}				
		} else {
			g_fCarSpeedSet = 0;
		}
		//----------------------------------------------------------------------
		
	}

	//--------------------------------------------------------------------------
  /* Write your code here */

  for(;;) {}
}



//------------------------------------------------------------------------------
	
/* END SMMNew1 */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.00 [04.35]
**     for the Freescale 56800 series of microcontrollers.
**
** ###################################################################
*/
