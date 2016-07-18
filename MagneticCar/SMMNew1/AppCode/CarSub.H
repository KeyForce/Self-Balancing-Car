/*
**==============================================================================
** CARSUB.H:             -- by Dr. ZhuoQing, 2012-2-3
**
**  Description:
**
**==============================================================================
*/
#ifndef __CARSUB__
#define __CARSUB__
//------------------------------------------------------------------------------
#ifdef CARSUB_GLOBALS
    #define CARSUB_EXT
#else
    #define CARSUB_EXT extern
#endif // CARSUB_GLOBALS
//------------------------------------------------------------------------------


//==============================================================================
//			CAR CONTROL SUBROUTIMES
//------------------------------------------------------------------------------
void CarSubInit(void);                          // Total Car Control Initialize functions

//------------------------------------------------------------------------------
//						1 millisecond event counter
CARSUB_EXT unsigned int g_n1MSEventCount;

//------------------------------------------------------------------------------
#define TIMETEST_EN						1
#if TIMETEST_EN
	#define TIMETEST_ON					LED_ON
	#define TIMETEST_OFF				LED_OFF
#else
	#define TIMETEST_ON
	#define TIMETEST_OFF
#endif // TIMETEST_EN

CARSUB_EXT int g_nTimeTestFlag;
#define TIME_TEST_ENA					g_nTimeTestFlag = 1
#define TIME_TEST_DIS					g_nTimeTestFlag = 0

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// INPUT VOLTAGE:
// Voltage 0 : Direction Left
//         1 : Direction Right;
//         2 : Debug voltage set 1
//         3 : Gyrograph 
//         4 : Gravity Z
//         5 : Debug voltage set 2
CARSUB_EXT int g_nInputVoltage[6];		// 
#define VOLTAGE_LEFT_C					0
#define VOLTAGE_RIGHT_C					1
#define VOLTAGE_SET1_C					2
#define VOLTAGE_GYRO_C					4
#define VOLTAGE_GRAVITY_C				3
#define VOLTAGE_SET2_C					5

#define VOLTAGE_LEFT					(g_nInputVoltage[VOLTAGE_LEFT_C])
#define VOLTAGE_RIGHT					(g_nInputVoltage[VOLTAGE_RIGHT_C])
#define VOLTAGE_GYRO					(g_nInputVoltage[VOLTAGE_GYRO_C])
#define VOLTAGE_GRAVITY					(g_nInputVoltage[VOLTAGE_GRAVITY_C])
#define VOLTAGE_SET1					(g_nInputVoltage[VOLTAGE_SET1_C])
#define VOLTAGE_SET2					(g_nInputVoltage[VOLTAGE_SET2_C])

//------------------------------------------------------------------------------
CARSUB_EXT int g_nADFlag;                       // 1: AD Convert in 1ms interrupt subroutine
#define AD_FLAG							g_nADFlag
#define AD_INT_START					g_nADFlag = 1
#define AD_INT_STOP						g_nADFlag = 0

//------------------------------------------------------------------------------
#define INPUT_VOLTAGE_AVERAGE			20
void SampleInputVoltage(void);
void GetInputVoltage(void);
void GetInputVoltageAverage(void);
CARSUB_EXT int g_nInputVoltageCount;
CARSUB_EXT long g_lnInputVoltageSigma[6];

//------------------------------------------------------------------------------
#define CONTROL_PERIOD					5		// Every 5 ms adjust all control

//------------------------------------------------------------------------------
//						MOTOR SPEED VOLTAGE OUTPUT FUNCTIONS
#define OPTICAL_ENCODE_CONSTANT			100     //
#define SPEED_CONTROL_COUNT				20  	// 20 ms * 5 ms

CARSUB_EXT int g_nLeftMotorPulse, g_nRightMotorPulse;	// unit : round/second
CARSUB_EXT int g_nLeftMotorPulseSigma, g_nRightMotorPulseSigma;
void GetMotorPulse(void);

//------------------------------------------------------------------------------
void SetMotorVoltage(float fLeftVoltage, float fRightVoltage);
                                                // Voltage : > 0 : Move forward;
                                                //           < 0 : Move backward
#define MOTOR_STOP						SetMotorVoltage(0, 0)
#define MOTOR_SETLOAD					PWMC1_Load()


//------------------------------------------------------------------------------
//						MOTOR SPEED CONTROL OUTPUT
// 
CARSUB_EXT float g_fLeftMotorOut, g_fRightMotorOut;
#define MOTOR_OUT_MAX					1.0
#define MOTOR_OUT_MIN					-1.0

#define MOTOR_OUT_DEAD_VAL				g_Argument.fDeadVoltage	//Unit : Full output
void MotorSpeedOut(void);						// 

#define MOTOR_LEFT_SPEED_POSITIVE		(g_fLeftMotorOut > 0)
#define MOTOR_RIGHT_SPEED_POSITIVE		(g_fRightMotorOut > 0)

CARSUB_EXT float g_fAngleControlOut, g_fSpeedControlOut, g_fDirectionControlOut;

void MotorOutput(void);

//------------------------------------------------------------------------------
//						MOTOR SPEED CONTROL
//
// Input : g_nCarSpeedSet: Speed setting value
//       : g_nCarSpeed: Motor Speed feedback value.
//
// Output :  g_nSpeedControlOut;
//
// Algrithm : PI adjust
#define SPEED_CONTROL_PERIOD			(SPEED_CONTROL_COUNT * CONTROL_PERIOD) //unit: ms
#define CAR_SPEED_CONSTANT				1000.0 / (SPEED_CONTROL_COUNT * CONTROL_PERIOD) / (float)OPTICAL_ENCODE_CONSTANT

CARSUB_EXT float g_fCarSpeed;			// Car Speed = (LeftMotorSpeedCount + nRightMotorSpeedCount) / 2;


CARSUB_EXT float g_fCarSpeedSet;
#define CAR_SPEED_SET					g_fCarSpeedSet; //g_Argument.fCarSpeedSet;


//------------------------------------------------------------------------------
#define SPEED_CONTROL_METHOD			0       // 0 : Speed standlone control
                                                // 1 : Angle combined control algrithm.

//------------------------------------------------------------------------------
#define CAR_SPEED_SET_MAX				50.0 		//
CARSUB_EXT float g_fSpeedControlIntegral;			// Keep the Speed Control Integral value
#define SPEED_CONTROL_OUT_MAX			MOTOR_OUT_MAX * 10
#define SPEED_CONTROL_OUT_MIN			MOTOR_OUT_MIN * 10

CARSUB_EXT float g_fSpeedControlOutOld, g_fSpeedControlOutNew;

//------------------------------------------------------------------------------
//					SPEED CONTROL
#define SPEED_CONTROL_STOP				g_nSpeedControlFlag = 0
#define SPEED_CONTROL_START				g_nSpeedControlFlag = 1
CARSUB_EXT int g_nSpeedControlFlag;


//------------------------------------------------------------------------------

CARSUB_EXT int g_nSpeedControlCount, g_nSpeedControlPeriod;


#define SPEED_CONTROL_P					g_Argument.fSpeedControlP
#define SPEED_CONTROL_I					g_Argument.fSpeedControlI

void SpeedControl(void);
void SpeedControlOutput(void);



//==============================================================================
//				INPUT GRAVITY ACCELERATION AND GYROSCOPE 
//------------------------------------------------------------------------------
#define GRAVITY_OFFSET					g_Argument.nGravityOffset 	// 2010
#define GRAVITY_MAX						g_Argument.nGravityMax		// 3030
#define GRAVITY_MIN						g_Argument.nGravityMin		// 1030

#define GYROSCOPE_OFFSET				g_Argument.nGyroscopeOffset	


CARSUB_EXT float g_fCarAngle;
CARSUB_EXT float g_fGravityAngle, g_fGyroscopeAngleSpeed;
CARSUB_EXT float g_fGyroscopeAngleIntegral;

#define CAR_ANGLE_RANGE					90.0
#define GRAVITY_ANGLE_RATIO				((float)CAR_ANGLE_RANGE / (float)(GRAVITY_MAX - GRAVITY_MIN) * 2.0)

#define GYROSCOPE_ANGLE_RATIO			g_Argument.fGyroscopeAngleRatio		// 2.40    //
#define GYROSCOPE_ANGLE_SIGMA_FREQUENCY	(1000 / CONTROL_PERIOD)

#define GRAVITY_ADJUST_TIME_CONSTANT	g_Argument.fGravityTimeConstant  //2.0     // unit : second

//------------------------------------------------------------------------------
void AngleCalculate(void);

//------------------------------------------------------------------------------
//  CONTROL CAR ANGLE 
//	Input : CAR_ANGLE_SET 0
//          g_fCarAngle, g_fGyroscopeAngleSpeed
//  Output : g_fAngleControlOut;
#define CAR_ANGLE_SET					0

//------------------------------------------------------------------------------
#define CAR_ANGLE_SPEED_SET				0
#define ANGLE_CONTROL_P					g_Argument.fAngleControlP		//50
#define ANGLE_CONTROL_D					g_Argument.fAngleControlD		//1.5

#define ANGLE_CONTROL_STOP				g_nAngleControlFlag = 0
#define ANGLE_CONTROL_START				g_nAngleControlFlag = 1;
CARSUB_EXT int g_nAngleControlFlag;

void AngleControl(void);
#define ANGLE_CONTROL_OUT_MAX			MOTOR_OUT_MAX * 10
#define ANGLE_CONTROL_OUT_MIN			MOTOR_OUT_MIN * 10

//------------------------------------------------------------------------------
#if SPEED_CONTROL_METHOD == 1
//CARSUB_EXT float g_fAngleControlSpeedFeedback;
//CARSUB_EXT float g_fAngleControlSpeedFeedbackOld, g_fAngleControlSpeedFeedbackNew;
#endif // SPEED_CONTROL_METHOD



//------------------------------------------------------------------------------
// 					DIRECTION CONTROL
#define DIRECTION_CONTROL_STOP			g_nDirectionControlFlag = 0
#define DIRECTION_CONTROL_START			g_nDirectionControlFlag = 1
CARSUB_EXT int g_nDirectionControlFlag;

void DirectionControl(void);
void DirectionControlOutput(void);

#define DIRECTION_CONTROL_OUT_MAX		MOTOR_OUT_MAX
#define DIRECTION_CONTROL_OUT_MIN		MOTOR_OUT_MIN

#define DIRECTION_CONTROL_DEADVALUE		0.0

#define DIR_LEFT_OFFSET					g_Argument.nDirLeftOffset
#define DIR_RIGHT_OFFSET				g_Argument.nDirRightOffset
#define DIR_CONTROL_P					g_Argument.fDirectionControlP

#define LEFT_RIGHT_MINIMUM				2

CARSUB_EXT float g_fDirectionControlOutOld, g_fDirectionControlOutNew;
CARSUB_EXT float g_fLeftVoltageSigma, g_fRightVoltageSigma;
CARSUB_EXT int g_nDirectionControlCount, g_nDirectionControlPeriod;

void DirectionVoltageSigma(void);

#define DIRECTION_CONTROL_COUNT			2
#define DIRECTION_CONTROL_PERIOD		(DIRECTION_CONTROL_COUNT * CONTROL_PERIOD)


//------------------------------------------------------------------------------
CARSUB_EXT	int g_nCarControlFlag;
#define CAR_CONTROL_SET					g_nCarControlFlag = 1
#define CAR_CONTROL_CLEAR				g_nCarControlFlag = 0
#define IF_CAR_CONTROL					(g_nCarControlFlag)
void WaitCarStand(void);
void CheckCarStand(void);

CARSUB_EXT int g_nWaitCarStandCount;
#define WAIT_CAR_STAND_COUNT			1


#define CAR_FAILURE_ANGLE_MAX			45.0
#define CAR_FAILURE_ANGLE_MIN			-45.0

#define CAR_STAND_ANGLE_MAX				10
#define CAR_STAND_ANGLE_MIN				-10.0



//------------------------------------------------------------------------------
void CarControlStart(void);
void CarControlStop(void);


//------------------------------------------------------------------------------
// 		TEST SPEED VALUE
// Notes: This global variable is only used for test purpos
CARSUB_EXT float g_fTestSpeedValue;

//==============================================================================
//             END OF THE FILE : CARSUB.H
//------------------------------------------------------------------------------
#endif // __CARSUB__
