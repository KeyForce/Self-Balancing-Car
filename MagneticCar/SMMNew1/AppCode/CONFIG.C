/*
**==============================================================================
** CONFIG.C:             -- by Dr. ZhuoQing, 2012-2-6
**
**==============================================================================
*/

/* Including needed modules to compile this module/procedure */
#include <intrinsics_56800e.h>
#include "Cpu.h"
#include "IFsh1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

//------------------------------------------------------------------------------
#include "F8013.h"

//------------------------------------------------------------------------------
#define CONFIG_GLOBALS        1                       // Define the global variables
#include "CONFIG.H"
//==============================================================================

//------------------------------------------------------------------------------
//==============================================================================
//						CONFIGRATION 
//------------------------------------------------------------------------------
void ArgumentInit(void) {
	g_Argument.nGravityOffset 		= GRAVITY_OFFSET_DEFAULT;
	g_Argument.nGyroscopeOffset		= GYROSCOPE_OFFSET_DEFAULT;
	g_Argument.nGravityMax			= GRAVITY_MAX_DEFAULT;
	g_Argument.nGravityMin			= GRAVITY_MIN_DEFAULT;
	g_Argument.fAngleControlP		= ANGLE_CONTROL_P_DEFAULT;
	g_Argument.fAngleControlD		= ANGLE_CONTROL_D_DEFAULT;
	g_Argument.fSpeedControlP		= SPEED_CONTROL_P_DEFAULT;
	g_Argument.fSpeedControlI		= SPEED_CONTROL_I_DEFAULT;
	g_Argument.fDirectionControlP   = DIRECTION_CONTROL_P_DEFAULT;
	g_Argument.nDirLeftOffset		= DIR_LEFT_OFFSET_DEFAULT;
	g_Argument.nDirRightOffset		= DIR_RIGHT_OFFSET_DEFAULT;
	g_Argument.fDeadVoltage			= DEAD_VOLTAGE_DEFAULT;
	g_Argument.fGyroscopeAngleRatio = GYROSCOPE_ANGLE_RATIO_DEFAULT;
	g_Argument.fGravityTimeConstant = GRAVITY_TIME_CONSTANT_DEFAULT;
	g_Argument.fCarSpeedSet		 	= CAR_SPEED_SET_DEFAULT;
	g_Argument.nPad					= 0;
	g_Argument.nInitFlag			= INIT_FLAG_DEFAULT;
}
	
void ArgumentLoad(void) {
	pmemReadDim(ARGUMENT_ADDRESS, sizeof(g_Argument) / 2, (unsigned int *)&g_Argument);
}
			
void ArgumentSave(void) {
	unsigned int * pArgument;
	int nSize, i;
	
	nSize = sizeof(g_Argument);
	pArgument = (unsigned int *)&g_Argument;
	for(i = 0; i < (nSize + 1) / 2; i ++) {
		IFsh1_SetWordFlash(i + ARGUMENT_ADDRESS, *(pArgument + i));
	}
}

//==============================================================================
//                END OF THE FILE : CONFIG.C
//------------------------------------------------------------------------------
