/** ###################################################################
**     Filename  : Events.C
**     Project   : SMMNew1
**     Processor : 56F8013VFAE
**     Component : Events
**     Version   : Driver 01.03
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2012-2-2, 11:06
**     Abstract  :
**         This is user's event module.
**         Put your event handler code here.
**     Settings  :
**     Contents  :
**         No public methods
**
** ###################################################################*/
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"
#include "F8013.H"
#include "CarSub.H"
#include "debug.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

/*
** ===================================================================
**     Event       :  AD1_OnEnd (module Events)
**
**     Component   :  AD1 [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#pragma interrupt called /* Comment this line if the appropriate 'Interrupt preserve registers' property */
/*
** ===================================================================
**     Event       :  TI1_OnInterrupt (module Events)
**
**     Component   :  TI1 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/ 
#pragma interrupt called /* Comment this line if the appropriate 'Interrupt preserve registers' property */
                         /* is set to 'yes' (#pragma interrupt saveall is generated before the ISR)      */
void TI1_OnInterrupt(void)
{
	int i;
  /* Write your code here ... */
  	//--------------------------------------------------------------------------
  	WAITTIME_INC;                               // Increase WAITTIME function counter;
  	if(TIME1MS_INT_FLAG) 	return;
  	
  	//--------------------------------------------------------------------------
  	g_n1MSEventCount ++;
	if(g_nTimeTestFlag)		TIMETEST_ON;
		  	
  	//--------------------------------------------------------------------------
  	g_nSpeedControlPeriod ++;
  	SpeedControlOutput();
  	
  	g_nDirectionControlPeriod ++;
  	DirectionControlOutput();
 
 	//--------------------------------------------------------------------------
  	if(g_n1MSEventCount >= CONTROL_PERIOD) {    // Motor speed adjust
  		g_n1MSEventCount = 0;                   // Clear the event counter;
  		
  		GetMotorPulse();
  	} else if(g_n1MSEventCount == 1) {          // Start ADC convert and Car erect adjust
  		if(AD_FLAG) {
			for(i = 0; i < INPUT_VOLTAGE_AVERAGE; i ++) 
				SampleInputVoltage();
	  	}
  	} else if(g_n1MSEventCount == 2) {          // Get the voltage and start calculation
		if(AD_FLAG) GetInputVoltageAverage();

		//----------------------------------------------------------------------
		AngleCalculate();
		AngleControl();		

		//----------------------------------------------------------------------
		MotorOutput();                          // Output motor control voltage;  		
  	} else if(g_n1MSEventCount == 3) {          // Car speed adjust
  		g_nSpeedControlCount ++;
  		if(g_nSpeedControlCount >= SPEED_CONTROL_COUNT) {
  			SpeedControl();
  			g_nSpeedControlCount = 0;
  			g_nSpeedControlPeriod = 0;  			
  		}
  	} else if(g_n1MSEventCount == 4) {          // Car direction control
		g_nDirectionControlCount ++;
		DirectionVoltageSigma();
		if(g_nDirectionControlCount >= DIRECTION_CONTROL_COUNT) {
	  		DirectionControl();
	  		g_nDirectionControlCount = 0;
	  		g_nDirectionControlPeriod = 0;
	  	}
  	}
  	
 	//--------------------------------------------------------------------------
 	if(g_nTimeTestFlag)		TIMETEST_OFF;
}

/* END Events */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.00 [04.35]
**     for the Freescale 56800 series of microcontrollers.
**
** ###################################################################
*/
