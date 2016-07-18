/*
**==============================================================================
** CARSUB.C:             -- by Dr. ZhuoQing, 2012-2-3
**
**==============================================================================
*/
#include <intrinsics_56800e.h>
#define CARSUB_GLOBALS        1                       // Define the global variables
#include "CARSUB.H"
//------------------------------------------------------------------------------
#include "F8013.H"

//------------------------------------------------------------------------------
#include "Cpu.h"
#include "Events.h"
#include "LED.h"
#include "AS1.h"
#include "AD1.h"
#include "PWMC1.h"
#include "COUNTER1.h"
#include "COUNTER2.h"
#include "TI1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

//------------------------------------------------------------------------------
#include "config.h"
#include "debug.h"

//==============================================================================
//				CAR CONTROL SUBROUTINES
//------------------------------------------------------------------------------
void CarSubInit(void) {
	//--------------------------------------------------------------------------
	// Initialize the Global Variable
	g_n1MSEventCount = 0;
	AD_INT_START;	
	TIME_TEST_DIS;
	
	CarControlStop();		
	g_nWaitCarStandCount = 0;
	
	//--------------------------------------------------------------------------
	
	g_nInputVoltageCount = 0;
	g_lnInputVoltageSigma[0] = 0;
	g_lnInputVoltageSigma[1] = 0;
	g_lnInputVoltageSigma[2] = 0;
	g_lnInputVoltageSigma[3] = 0;
	g_lnInputVoltageSigma[4] = 0;
	g_lnInputVoltageSigma[5] = 0;

	g_nLeftMotorPulse = g_nRightMotorPulse = 0;	
	g_nLeftMotorPulseSigma = g_nRightMotorPulseSigma = 0;
	g_fCarSpeed = 0;
	MOTOR_STOP;
	
	g_fLeftMotorOut = g_fRightMotorOut = 0;
	g_fAngleControlOut = g_fSpeedControlOut = g_fDirectionControlOut = 0;
	
	g_fSpeedControlOutOld = g_fSpeedControlOutNew = 0;
	g_nSpeedControlCount = g_nSpeedControlPeriod = 0;
	g_fSpeedControlIntegral = 0;

	g_fCarAngle = 0;
	g_fGyroscopeAngleSpeed = 0;
	g_fGravityAngle = 0;
	g_fGyroscopeAngleIntegral = 0;
	
#if SPEED_CONTROL_METHOD == 1
	g_fAngleControlSpeedFeedback = 0;
	g_fAngleControlSpeedFeedbackNew = 0;
	g_fAngleControlSpeedFeedbackOld = 0;
#endif // SPEED_CONTROL_METHOD
	//--------------------------------------------------------------------------
	g_fLeftVoltageSigma = g_fRightVoltageSigma = 0;
	g_fDirectionControlOutNew = g_fDirectionControlOutOld = 0;
	g_nDirectionControlCount = g_nDirectionControlPeriod = 0;
	
	//--------------------------------------------------------------------------
	g_fCarSpeedSet = 0;
	
	//--------------------------------------------------------------------------
	g_fTestSpeedValue = 0;
}

//------------------------------------------------------------------------------
//						GET INPUT VOLTAGE
void GetInputVoltage(void) {
	int i;
	
	AD1_GetValue16((unsigned int *)g_nInputVoltage);
	for(i = 0; i < 6; i ++)
		g_nInputVoltage[i] = (int)(((unsigned int)g_nInputVoltage[i]) >> 4);
}

void SampleInputVoltage(void) {
	unsigned int nInputVoltage[6];
	
	AD1_Measure(0);
	Delay10US(2);
	AD1_GetValue16(nInputVoltage);
	g_lnInputVoltageSigma[0] += nInputVoltage[0];
	g_lnInputVoltageSigma[1] += nInputVoltage[1];
	g_lnInputVoltageSigma[2] += nInputVoltage[2];
	g_lnInputVoltageSigma[3] += nInputVoltage[3];
	g_lnInputVoltageSigma[4] += nInputVoltage[4];
	g_lnInputVoltageSigma[5] += nInputVoltage[5];
	g_nInputVoltageCount ++;	
}

void GetInputVoltageAverage(void) {
	int i;
	
	if(g_nInputVoltageCount == 0) {
		for(i = 0; i < 6; i ++) 
			g_lnInputVoltageSigma[i] = 0;
		return;
	}
	
	g_nInputVoltageCount <<= 4;
	for(i = 0; i < 6; i ++) {
		g_nInputVoltage[i] = (int)(g_lnInputVoltageSigma[i] / g_nInputVoltageCount);
		g_lnInputVoltageSigma[i] = 0;
	}
	
	g_nInputVoltageCount = 0;
}

//------------------------------------------------------------------------------
//						MOTOR SPEED CONTROL
//
// 
void GetMotorPulse(void) {
	unsigned int nLeftPulse, nRightPulse;
	
	COUNTER1_GetNumEvents(&nLeftPulse);
	COUNTER2_GetNumEvents(&nRightPulse);
	COUNTER1_Reset();
	COUNTER2_Reset();
	
	g_nLeftMotorPulse = (int)nLeftPulse;
	g_nRightMotorPulse = (int)nRightPulse;
	if(!MOTOR_LEFT_SPEED_POSITIVE)		g_nLeftMotorPulse = -g_nLeftMotorPulse;
	if(!MOTOR_RIGHT_SPEED_POSITIVE)		g_nRightMotorPulse = -g_nRightMotorPulse;
		
	g_nLeftMotorPulseSigma += g_nLeftMotorPulse;
	g_nRightMotorPulseSigma += g_nRightMotorPulse;
}

//------------------------------------------------------------------------------
void SetMotorVoltage(float fLeftVoltage, float fRightVoltage) {
                                                // Voltage : > 0 : Move forward;
                                                //           < 0 : Move backward
	short nPeriod;
	int nOut;
	
	nPeriod = (short)getReg(PWM_PWMCM);
	
	//--------------------------------------------------------------------------
	if(fLeftVoltage > 1.0) 			fLeftVoltage = 1.0;
	else if(fLeftVoltage < -1.0) 	fLeftVoltage = -1.0;
	
	if(fRightVoltage > 1.0) 		fRightVoltage = 1.0;
	else if(fRightVoltage < -1.0)	fRightVoltage = -1.0;
                                              
	//--------------------------------------------------------------------------	                                            	                                              
	if(fLeftVoltage > 0) {
		setReg(PWM_PWMVAL1, 0);
		nOut = (int)(fLeftVoltage * nPeriod);
		setReg(PWM_PWMVAL0, nOut);
	} else {
		setReg(PWM_PWMVAL0, 0);
		fLeftVoltage = -fLeftVoltage;
		nOut = (int)(fLeftVoltage * nPeriod);
		setReg(PWM_PWMVAL1, nOut);
	}                                     

	//--------------------------------------------------------------------------
	if(fRightVoltage > 0) {
		setReg(PWM_PWMVAL2, 0);
		nOut = (int)(fRightVoltage * nPeriod);
		setReg(PWM_PWMVAL3, nOut);
	} else {
		setReg(PWM_PWMVAL3, 0);
		fRightVoltage = -fRightVoltage;
		nOut = (int)(fRightVoltage * nPeriod);
		setReg(PWM_PWMVAL2, nOut);
	}

	MOTOR_SETLOAD;                              // Reload the PWM value
}                                            


//------------------------------------------------------------------------------
//						MOTOR SPEED CONTROL OUTPUT
// 
void MotorSpeedOut(void) {
	float fLeftVal, fRightVal;
	
	fLeftVal = g_fLeftMotorOut;
	fRightVal = g_fRightMotorOut;
	if(fLeftVal > 0) 			fLeftVal += MOTOR_OUT_DEAD_VAL;
	else if(fLeftVal < 0) 		fLeftVal -= MOTOR_OUT_DEAD_VAL;
	
	if(fRightVal > 0)			fRightVal += MOTOR_OUT_DEAD_VAL;
	else if(fRightVal < 0)		fRightVal -= MOTOR_OUT_DEAD_VAL;
		
	if(fLeftVal > MOTOR_OUT_MAX)	fLeftVal = MOTOR_OUT_MAX;
	if(fLeftVal < MOTOR_OUT_MIN)	fLeftVal = MOTOR_OUT_MIN;
	if(fRightVal > MOTOR_OUT_MAX)	fRightVal = MOTOR_OUT_MAX;
	if(fRightVal < MOTOR_OUT_MIN)	fRightVal = MOTOR_OUT_MIN;
			
	SetMotorVoltage(fLeftVal, fRightVal);
}
		
//------------------------------------------------------------------------------
void MotorOutput(void) {
	float fLeft, fRight;

	fLeft = g_fAngleControlOut - g_fSpeedControlOut - g_fDirectionControlOut;
	fRight = g_fAngleControlOut - g_fSpeedControlOut + g_fDirectionControlOut;
	
	if(fLeft > MOTOR_OUT_MAX)		fLeft = MOTOR_OUT_MAX;
	if(fLeft < MOTOR_OUT_MIN)		fLeft = MOTOR_OUT_MIN;
	if(fRight > MOTOR_OUT_MAX)		fRight = MOTOR_OUT_MAX;
	if(fRight < MOTOR_OUT_MIN)		fRight = MOTOR_OUT_MIN;
		
	g_fLeftMotorOut = fLeft;
	g_fRightMotorOut = fRight;
	MotorSpeedOut();
}

//------------------------------------------------------------------------------
void SpeedControl(void) {
	float fP, fDelta;
	float fI;

	
	//--------------------------------------------------------------------------
	g_fCarSpeed = (g_nLeftMotorPulseSigma + g_nRightMotorPulseSigma) / 2;
	g_nLeftMotorPulseSigma = g_nRightMotorPulseSigma = 0;
	g_fCarSpeed *= CAR_SPEED_CONSTANT;

	//--------------------------------------------------------------------------	
	if(g_nSpeedControlFlag == 0) {
		g_fSpeedControlOutOld = g_fSpeedControlOutNew = g_fSpeedControlOut = 0;
		g_fSpeedControlIntegral = 0;
		return;
	}
		
	//--------------------------------------------------------------------------
	fDelta = CAR_SPEED_SET;
	fDelta -= g_fCarSpeed;
	
	fP = fDelta * SPEED_CONTROL_P;
	fI = fDelta * SPEED_CONTROL_I;
	g_fSpeedControlIntegral += fI;
	
	
		
	//--------------------------------------------------------------------------
	
#if SPEED_CONTROL_METHOD == 0
	if(g_fSpeedControlIntegral > SPEED_CONTROL_OUT_MAX)	
		g_fSpeedControlIntegral = SPEED_CONTROL_OUT_MAX;
	if(g_fSpeedControlIntegral < SPEED_CONTROL_OUT_MIN)  	
		g_fSpeedControlIntegral = SPEED_CONTROL_OUT_MIN;
	g_fSpeedControlOutOld = g_fSpeedControlOutNew;

	g_fSpeedControlOutNew = fP + g_fSpeedControlIntegral;
	
#else if SPEED_CONTROL_METHOD == 1
//	g_fSpeedControlOutOld = g_fSpeedControlOutNew;
	
//	g_fAngleControlSpeedFeedbackOld = g_fAngleControlSpeedFeedbackNew;
//	g_fAngleControlSpeedFeedbackNew = g_fSpeedControlIntegral + fP;
#endif // SPEED_CONTROL_METHOD
	
}

//------------------------------------------------------------------------------
//   
void SpeedControlOutput(void) {
	float fValue;
	fValue = g_fSpeedControlOutNew - g_fSpeedControlOutOld;
	g_fSpeedControlOut = fValue * (g_nSpeedControlPeriod + 1) / SPEED_CONTROL_PERIOD + g_fSpeedControlOutOld;
	
#if SPEED_CONTROL_METHOD == 1
//	fValue = g_fAngleControlSpeedFeedbackNew - g_fAngleControlSpeedFeedbackOld;
//	g_fAngleControlSpeedFeedback = fValue * (g_nSpeedControlPeriod + 1) / SPEED_CONTROL_PERIOD + 
//					g_fAngleControlSpeedFeedbackOld;
#endif // SPEED_CONTROL_METHOD
}



//==============================================================================
//				INPUT GRAVITY ACCELERATION AND GYROSCOPE 
//------------------------------------------------------------------------------
void AngleCalculate(void) {
	float fDeltaValue;
	
	//--------------------------------------------------------------------------
	// Define Angle 
	g_fGravityAngle = (VOLTAGE_GRAVITY - GRAVITY_OFFSET) * GRAVITY_ANGLE_RATIO;
	g_fGyroscopeAngleSpeed = (VOLTAGE_GYRO - GYROSCOPE_OFFSET) * GYROSCOPE_ANGLE_RATIO;
	
	g_fCarAngle = g_fGyroscopeAngleIntegral;
	fDeltaValue = (g_fGravityAngle - g_fCarAngle) / GRAVITY_ADJUST_TIME_CONSTANT;
	
#if SPEED_CONTROL_METHOD == 0
	g_fGyroscopeAngleIntegral += (g_fGyroscopeAngleSpeed + fDeltaValue) / GYROSCOPE_ANGLE_SIGMA_FREQUENCY;
#else if SPEED_CONTROL_METHOD == 1
//	g_fGyroscopeAngleIntegral += (g_fGyroscopeAngleSpeed + g_fAngleControlSpeedFeedback) / GYROSCOPE_ANGLE_SIGMA_FREQUENCY;
#endif // SPEED_CONTROL_METHOD
}
	
//------------------------------------------------------------------------------
void AngleControl(void) {
	float fValue;

	if(g_nAngleControlFlag == 0) {
		g_fAngleControlOut = 0;
		return;
	}
	
	//--------------------------------------------------------------------------
	fValue = (CAR_ANGLE_SET - g_fCarAngle) * ANGLE_CONTROL_P +
	         (CAR_ANGLE_SPEED_SET - g_fGyroscopeAngleSpeed) * ANGLE_CONTROL_D;
	         
	if(fValue > ANGLE_CONTROL_OUT_MAX)			fValue = ANGLE_CONTROL_OUT_MAX;
	else if(fValue < ANGLE_CONTROL_OUT_MIN) 	fValue = ANGLE_CONTROL_OUT_MIN;
	g_fAngleControlOut = fValue;
	
}

//------------------------------------------------------------------------------
void DirectionVoltageSigma(void) {
	int nLeft, nRight;
	if(VOLTAGE_LEFT > DIR_LEFT_OFFSET) 		nLeft = VOLTAGE_LEFT - DIR_LEFT_OFFSET;
	else nLeft = 0;
	if(VOLTAGE_RIGHT > DIR_RIGHT_OFFSET) 	nRight = VOLTAGE_RIGHT - DIR_RIGHT_OFFSET;
	else nRight = 0;

	g_fLeftVoltageSigma += nLeft;
	g_fRightVoltageSigma += nRight;
}

//------------------------------------------------------------------------------	
void DirectionControl(void) {
	float fLeftRightAdd, fLeftRightSub, fValue;
	int nLeft, nRight;

	if(g_nDirectionControlFlag == 0) {
		g_fDirectionControlOut = 0;
		g_fDirectionControlOutOld = g_fDirectionControlOutNew = 0;
		return;
	}

	//--------------------------------------------------------------------------
	nLeft = (int)(g_fLeftVoltageSigma /= DIRECTION_CONTROL_COUNT);
	nRight = (int)(g_fRightVoltageSigma /= DIRECTION_CONTROL_COUNT);
	g_fLeftVoltageSigma = 0;
	g_fRightVoltageSigma = 0;
	//--------------------------------------------------------------------------	
/*	if(VOLTAGE_LEFT > DIR_LEFT_OFFSET) 		nLeft = VOLTAGE_LEFT - DIR_LEFT_OFFSET;
	else nLeft = 0;
	if(VOLTAGE_RIGHT > DIR_RIGHT_OFFSET) 	nRight = VOLTAGE_RIGHT - DIR_RIGHT_OFFSET;
	else nRight = 0;
*/	
	fLeftRightAdd = nLeft + nRight;
	fLeftRightSub = nLeft - nRight;
	
	g_fDirectionControlOutOld = g_fDirectionControlOutNew;
	
	if(fLeftRightAdd < LEFT_RIGHT_MINIMUM) {
		g_fDirectionControlOutNew = 0;
	} else {
		fValue = fLeftRightSub * DIR_CONTROL_P / fLeftRightAdd;
		
		if(fValue > 0) fValue += DIRECTION_CONTROL_DEADVALUE;
		if(fValue < 0) fValue -= DIRECTION_CONTROL_DEADVALUE;
		
		if(fValue > DIRECTION_CONTROL_OUT_MAX) fValue = DIRECTION_CONTROL_OUT_MAX;
		if(fValue < DIRECTION_CONTROL_OUT_MIN) fValue = DIRECTION_CONTROL_OUT_MIN;
			g_fDirectionControlOutNew = fValue;
	}
}

//------------------------------------------------------------------------------
void DirectionControlOutput(void) {
	float fValue;
	fValue = g_fDirectionControlOutNew - g_fDirectionControlOutOld;
	g_fDirectionControlOut = fValue * (g_nDirectionControlPeriod + 1) / DIRECTION_CONTROL_PERIOD + g_fDirectionControlOutOld;
	
}


//------------------------------------------------------------------------------
void CarControlStart(void) {
	CAR_CONTROL_SET;
	ANGLE_CONTROL_START;
	SPEED_CONTROL_START;
	DIRECTION_CONTROL_START;
	TIME_TEST_ENA;
}

//------------------------------------------------------------------------------
void CarControlStop(void) {
	SetMotorVoltage(0, 0);
	
	CAR_CONTROL_CLEAR;
	ANGLE_CONTROL_STOP;
	SPEED_CONTROL_STOP;
	DIRECTION_CONTROL_STOP;
	TIME_TEST_DIS;

	g_fLeftMotorOut = g_fRightMotorOut = 0;
	g_fAngleControlOut = 0;
	g_fSpeedControlOut = 0;
	g_fDirectionControlOut = 0;

	g_nWaitCarStandCount = 0;
}

//------------------------------------------------------------------------------
void WaitCarStand(void) {
	if(g_nCarControlFlag == 1) return;
	if(g_nWaitCarStandCount < WAIT_CAR_STAND_COUNT) {
		g_nWaitCarStandCount ++;
		return;
	}
	
	
	if(g_fCarAngle > CAR_STAND_ANGLE_MIN &&
	   g_fCarAngle < CAR_STAND_ANGLE_MAX &&
	   g_fGravityAngle > CAR_STAND_ANGLE_MIN &&
	   g_fGravityAngle < CAR_STAND_ANGLE_MAX) {
	   	CarControlStart();
	} 	
}

//------------------------------------------------------------------------------	
void CheckCarStand(void) {
	if(g_nCarControlFlag == 0) return;
		
	if(g_fCarAngle >= CAR_FAILURE_ANGLE_MAX ||
	   g_fCarAngle <= CAR_FAILURE_ANGLE_MIN) {
		CarControlStop();
		return;
	}
}


//==============================================================================
//                END OF THE FILE : CARSUB.C
//------------------------------------------------------------------------------
