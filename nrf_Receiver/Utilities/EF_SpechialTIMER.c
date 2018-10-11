 /***************************************************************
 *  Source File: EF_SpechialTIMER.c
 *
 *  Description: using Timer driver to make many features with time
 *				 such as delay , make unstuck functions ,..
 *
 *  History:     Version 1.0  - INIT Version
 *  Date   :     25/08/2014
 *  -------------------------------------------------------
 *  Author :     EmbeddedFab.
 *
 *  Copyright (C) <2014>  <EmbeddedFab>

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>

 **************************************************************/

#include "EF_SpechialTIMER.h"

/**************************************************
 * 	global variables
 *************************************************/
static TIMER_ID_str timers_list[MAX_NUMBER_OF_TIMERS];

/****************************************************************************
* Function    : void_SCHEDULER_timerISRCallback
*
* DESCRIPTION : Function to set the systemTik Flag, "Called in the TIMER ISR"
*
* PARAMETERS  : None
*
* Return Value: None
*
*  NOTE		  : "Don't forget putting Timer_ISR function in the startup code"
******************************************************************************/
static void EF_void_TimerISRCallback(void)
{
	//count the enabled timer and if it reached the timeOut ,counter=0 and set timeOut Flag
	EF_void_TimersUpdate();
}

/****************************************************************************
* Function    : EF_void_TimerDelete
*
* DESCRIPTION : make all timer_list_parameters = 0
*
* PARAMETERS  : timerNumber
*
* Return Value: None
*
******************************************************************************/
void EF_void_TimerDelete(U8_t timerNumber)
{
	timers_list[timerNumber].counter    = 0;
	timers_list[timerNumber].timeOut    = 0;
	timers_list[timerNumber].isTimedout =  FALSE;
	timers_list[timerNumber].isEnables  =  FALSE;
}

/****************************************************************************
* Function    : EF_void_TimerInit
*
* DESCRIPTION : initialize the timer ,define the callback function and
* 				call void_timerDelete().
*
* PARAMETERS  : None.
*
* Return Value: None.
*
******************************************************************************/
void EF_void_TimerInit(void)
{
	U16_t timersCounter;
	//don't forget put ISR function in startup code
	EF_void_Timer_PORTinit();
	// set the timer interrupt callback
	EF_void_Timer_SetISRCallBack(EF_void_TimerISRCallback);
	// set all Timers to the initial state
	for( timersCounter=0 ; timersCounter < MAX_NUMBER_OF_TIMERS ; timersCounter++ )
	{
		EF_void_TimerDelete(timersCounter);
	}
}



/****************************************************************************
* Function    : EF_BOOLEAN_Timer_IsTimedOut(U8_t timerNumber)
*
* DESCRIPTION : return the status of timer if it finished or not, not change Timer status
*
* PARAMETERS  : timerNumber
*
* Return Value: True if time out.
******************************************************************************/
BOOLEAN EF_BOOLEAN_Timer_IsTimedOut(U8_t timerNumber)
{
    return (timers_list[timerNumber].isTimedout);
}


/****************************************************************************
* Function    : EF_void_TimerCreate
*
* DESCRIPTION : set the time out for defined timer.
*
* PARAMETERS  : timerNumber : Timer ID
* 				timerTimeout: depend on Timer clock cycle
*
* Return Value: None.
*
******************************************************************************/
void EF_void_TimerCreate(U8_t timerNumber , U32_t timerTimeout) /* Timer timeout in ticks */
{
	if(timers_list[timerNumber].timeOut == 0)
	{
		timers_list[timerNumber].timeOut = timerTimeout;
		timers_list[timerNumber].counter = 0 ;
		timers_list[timerNumber].isTimedout = FALSE ;
		timers_list[timerNumber].isEnables = FALSE;
	}
}
/****************************************************************************
* Function    : EF_void_TimersUpdate
*
* DESCRIPTION : count the enabled timer and if it reached the timeOut ,counter=0 and set timeOut Flag
* 				this function called in the ISR by call back function.
*
* PARAMETERS  : None.
*
* Return Value: None.
*
******************************************************************************/
void EF_void_TimersUpdate(void)
{
	U16_t timersCounter = 0;

	for( timersCounter = 0 ; timersCounter < MAX_NUMBER_OF_TIMERS ; timersCounter++ )
	{
		if(timers_list[timersCounter].isEnables == TRUE)
		{
			timers_list[timersCounter].counter++ ;

			if(timers_list[timersCounter].counter == timers_list[timersCounter].timeOut)
			{
				timers_list[timersCounter].counter = 0;
				timers_list[timersCounter].isTimedout = TRUE;
			}
		}
	}
}
/****************************************************************************
* Function    : EF_BOOLEAN_TimerCheck
*
* DESCRIPTION : check for time out
*
* PARAMETERS  : timerNumber
*
* Return Value: BOOLEAN : return True when timer timeout
*
******************************************************************************/
BOOLEAN EF_BOOLEAN_TimerCheck(U8_t timerNumber)
{
	BOOLEAN timerTimeout = FALSE;
	if(timers_list[timerNumber].isTimedout)
	{
		timers_list[timerNumber].isTimedout = FALSE;
		timerTimeout = TRUE;
	}
	return timerTimeout;
}
/****************************************************************************
* Function    : EF_void_TimerStart
*
* DESCRIPTION : enable this timer by making isEnables = True
*
* PARAMETERS  : timerNumber
*
* Return Value: None.
*
******************************************************************************/
void EF_void_TimerStart(U8_t timerNumber)
{
	if(timers_list[timerNumber].timeOut != 0)
	{
		timers_list[timerNumber].isEnables = TRUE;
	}
}


/****************************************************************************
* Function    : EF_u8_TimerIsEnabled(U8_t timerNumber)
*
* DESCRIPTION : return the status of Counter is Enabled or not
*
* PARAMETERS  : timerNumber: Number of Timer
*
* Return Value: True if Enableand False if disable.
******************************************************************************/
BOOLEAN EF_u8_TimerIsEnabled(U8_t timerNumber)
{
   return ( timers_list[timerNumber].isEnables);
}



/****************************************************************************
* Function    : EF_void_TimerChangeTimeOut(U8_t timerNumber ,U32_t NewTimeOut)
*
* DESCRIPTION : change the time out of the Given timer
*
* PARAMETERS  : timerNumber: Number of Timer
*               NewTimeOut : New Time Out Value
*
* Return Value: Void.
******************************************************************************/
void EF_void_TimerChangeTimeOut(U8_t timerNumber ,U32_t NewTimeOut)
{
    timers_list[timerNumber].timeOut = NewTimeOut;
}


/****************************************************************************
* Function    : EF_u32_TimerGetCounterValue (U8_t timerNumber )
*
* DESCRIPTION : return the current Counter Value of the wanted Counter.
*
* PARAMETERS  : timerNumber: Number of Timer
*
* Return Value: Counter Value.
******************************************************************************/
U32_t EF_u32_TimerGetCounterValue (U8_t timerNumber )
{
    return ( timers_list[timerNumber].counter);
}

/****************************************************************************
* Function    : EF_void_TimerStop
*
* DESCRIPTION : stop this timer by making isEnables = False
*
* PARAMETERS  : timerNumber
*
* Return Value: None.
*
******************************************************************************/
void EF_void_TimerStop(U8_t timerNumber)
{
	timers_list[timerNumber].isEnables = FALSE;
}
/****************************************************************************
* Function    : EF_void_TimerReset
*
* DESCRIPTION : make counter = 0
*
* PARAMETERS  : timerNumber
*
* Return Value: None.
*
******************************************************************************/
void EF_void_TimerReset(U8_t timerNumber)
{
	timers_list[timerNumber].counter = 0;
}

/****************************************************************************
* Function    : EF_void_Timer_TurnOff
*
* DESCRIPTION : stop and reset and release the timeout flag
*
* PARAMETERS  : timerNumber
*
* Return Value: None.
******************************************************************************/
void EF_void_Timer_TurnOff(U8_t timerNumber)
{
    timers_list[timerNumber].counter = 0;
    timers_list[timerNumber].isEnables = FALSE;
    // Note: that I added this Line
    timers_list[timerNumber].isTimedout = FALSE;
}
