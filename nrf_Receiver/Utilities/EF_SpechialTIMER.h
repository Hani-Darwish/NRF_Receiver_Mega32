 /***************************************************************
 *  Source File: EF_SpechialTIMER.h
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
#ifndef _SPECHIAL_TIMER__
#define _SPECHIAL_TIMER__

#include "EF_SpechialTIMER_cfg.h"


/*************************************************
 * 	 API functions
 *************************************************/

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
extern void EF_void_TimerInit(void);
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
extern void EF_void_TimerCreate(U8_t timerNumber , U32_t timerTimeout);
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
extern void EF_void_TimerDelete(U8_t timerNumber);

/****************************************************************************
* Function    : EF_BOOLEAN_Timer_IsTimedOut(U8_t timerNumber)
*
* DESCRIPTION : return the status of timer if it finished or not, not change Timer status
*
* PARAMETERS  : timerNumber
*
* Return Value: True if time out.
******************************************************************************/
BOOLEAN EF_BOOLEAN_Timer_IsTimedOut(U8_t timerNumber);

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
extern void EF_void_TimersUpdate(void);
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
extern BOOLEAN EF_BOOLEAN_TimerCheck(U8_t timerNumber);
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
extern void EF_void_TimerStart(U8_t timerNumber);

/****************************************************************************
* Function    : EF_u8_TimerIsEnabled(U8_t timerNumber)
*
* DESCRIPTION : return the status of Counter is Enabled or not
*
* PARAMETERS  : timerNumber: Number of Timer
*
* Return Value: True if Enableand False if disable.
******************************************************************************/
BOOLEAN EF_u8_TimerIsEnabled(U8_t timerNumber);

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
void EF_void_TimerChangeTimeOut(U8_t timerNumber, U32_t NewTimeOut);

/****************************************************************************
* Function    : EF_u32_TimerGetCounterValue (U8_t timerNumber )
*
* DESCRIPTION : return the current Counter Value of the wanted Counter.
*
* PARAMETERS  : timerNumber: Number of Timer
*
* Return Value: Counter Value.
******************************************************************************/
U32_t EF_u32_TimerGetCounterValue ( U8_t timerNumber );



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

extern void EF_void_TimerStop(U8_t timerNumber);
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
void EF_void_TimerReset(U8_t timerNumber);


/****************************************************************************
* Function    : EF_void_Timer_TurnOff
*
* DESCRIPTION : stop and reset and release the timeout flag
*
* PARAMETERS  : timerNumber
*
* Return Value: None.
******************************************************************************/
void EF_void_Timer_TurnOff(U8_t timerNumber);
#endif
