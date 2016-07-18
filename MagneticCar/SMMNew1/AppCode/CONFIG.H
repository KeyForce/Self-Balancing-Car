/*
**==============================================================================
** CONFIG.H:             -- by Dr. ZhuoQing, 2012-2-6
**
**  Description:
**
**==============================================================================
*/
#ifndef __CONFIG__
#define __CONFIG__
//------------------------------------------------------------------------------
#ifdef CONFIG_GLOBALS
    #define CONFIG_EXT
#else
    #define CONFIG_EXT extern
#endif // CONFIG_GLOBALS
//------------------------------------------------------------------------------
//==============================================================================
//						CONFIGRATION 
//------------------------------------------------------------------------------
typedef struct __tagArgument {
	int nGravityOffset;
	int nGyroscopeOffset;
	int nGravityMax;
	int nGravityMin;
	
	float fAngleControlP, fAngleControlD;
	float fSpeedControlP, fSpeedControlI;
	float fDirectionControlP;

	float fGyroscopeAngleRatio;
	float fGravityTimeConstant;
	float fDeadVoltage;
	float fCarSpeedSet;
	
	int nDirLeftOffset;
	int nDirRightOffset;
	
	unsigned int nPad;
	unsigned int nInitFlag;
	
} tagArgument;

CONFIG_EXT tagArgument g_Argument;

//------------------------------------------------------------------------------
#define GRAVITY_OFFSET_DEFAULT			4000
#define GYROSCOPE_OFFSET_DEFAULT		2000
#define GRAVITY_MAX_DEFAULT				3000
#define GRAVITY_MIN_DEFAULT				1000
#define ANGLE_CONTROL_P_DEFAULT			50
#define ANGLE_CONTROL_D_DEFAULT			1.5
#define SPEED_CONTROL_P_DEFAULT			2
#define SPEED_CONTROL_I_DEFAULT			0.2
#define DIRECTION_CONTROL_P_DEFAULT		1
#define DIR_LEFT_OFFSET_DEFAULT			0
#define DIR_RIGHT_OFFSET_DEFAULT		0
#define DEAD_VOLTAGE_DEFAULT			0x40
#define INIT_FLAG_DEFAULT				0x55aa
#define GYROSCOPE_ANGLE_RATIO_DEFAULT	2.40
#define GRAVITY_TIME_CONSTANT_DEFAULT	2

#define CAR_SPEED_SET_DEFAULT			0
#define CAR_PAD_DEFAULT					0

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#define ARGUMENT_ADDRESS				0x1A00

void ArgumentInit(void);
void ArgumentLoad(void);
void ArgumentSave(void);

//------------------------------------------------------------------------------


//==============================================================================
//             END OF THE FILE : CONFIG.H
//------------------------------------------------------------------------------
#endif // __CONFIG__
