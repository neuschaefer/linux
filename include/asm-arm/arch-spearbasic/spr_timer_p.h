/*
 * linux/include/asm/arch/spr_timer_p.h
 *
 *  Copyright(C)2006 ST MICROELECTRONICS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*---------------------------------------------------------------------------------------------
* Private Header file for TIMER (DTU)
*---------------------------------------------------------------------------------------------
*/
#ifndef _TMR_P_H_
#define _TMR_P_H_

/*------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------*/
#include "spr_timer_irqp.h"


/*------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------*/
#define TMR_HCL_VERSION_ID          2
#define TMR_MAJOR_VERSION           0
#define TMR_MINOR_VERSION           0

#define TMR_TOTAL_NUMBER_OF_TIMERS  (TMR_INSTANCES_OF_TMR_UNITS * TMR_NUMBER_OF_TIMERS_IN_ONE_UNIT)
#define TMR_SPACE_FOR_SAVING_DEVICE_CONTEXT 24 /*SPEAR: one DTU contains 24 registers */

/*------------------------------------------------------------------------
 * New types
 *----------------------------------------------------------------------*/
/*definition relative to the use of DTU*/
typedef enum
{
    TMR_STATE_BUSY          = 0,
    TMR_STATE_FREE          = 1,
    TMR_STATE_INITIALIZED   = 2,
    TMR_STATE_RUNNING       = 3
} t_tmr_state;

typedef struct
{
    t_tmr_register          *p_tmr_register[TMR_INSTANCES_OF_TMR_UNITS];
    t_tmr_register_subset   *tmr_temp_ptr[TMR_TOTAL_NUMBER_OF_TIMERS];  /*timer ptrs */

    t_uint8                 tmr_initialized[TMR_INSTANCES_OF_TMR_UNITS];

    t_tmr_state             tmr_state[TMR_TOTAL_NUMBER_OF_TIMERS];      /*state of the timer BUSY/FREE*/
    t_tmr_id                tmr_mask[TMR_TOTAL_NUMBER_OF_TIMERS];       /*mask for a specific timer*/
    t_tmr_mode              tmr_mode[TMR_TOTAL_NUMBER_OF_TIMERS];       /*mode for a specific timer*/
    t_tmr_capt_mode         tmr_capt_mode[TMR_TOTAL_NUMBER_OF_TIMERS];   /*Capt mode of a timer*/

    t_uint32                number_of_total_timers;                     /*number of timers for the TU in use*/
    t_tmr_event             tmr_event;

    t_uint16                tmr_device_context[TMR_SPACE_FOR_SAVING_DEVICE_CONTEXT]; /*SPEAR: all the registers are 16 bit wide */
} t_tmr_system_context;

/*------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------*/

/* HW access macros to value register*/
/*spear #define TMR_SET_LOAD_REG(m)     (p_tmr_register_subset->tmr_load = (m)) */

#define TMR_SET_COMPARE_REG(m)     (p_tmr_register_subset->tmr_compare = (m))
/*#define TMR_GET_VALUE_REG(m)    ((m) = p_tmr_register->tmr_value) */
#define TMR_GET_VALUE_REG(m)    ((m) = p_tmr_register_subset->tmr_count)

/*SPEAR*/
#define TMR_GET_VALUE_FEDGE_REG(m)    ((m) = p_tmr_register_subset->tmr_fedge_capt)
#define TMR_GET_VALUE_REDGE_REG(m)    ((m) = p_tmr_register_subset->tmr_redge_capt)


/*********************control register  access macros********************/
#define TMR_TIMER_ENABLE    TMR_HCL_FIELDSET(p_tmr_register_subset->tmr_control, TIMER_ENABLE, 1)

/*
 * timer enable on control register
 */
#define TIMER_ENABLEwidth   1
#define TIMER_ENABLEshift   5

/*
 * timer disaable on control register
 */
#define TMR_TIMER_DISABLE   TMR_HCL_FIELDSET(p_tmr_register_subset->tmr_control, TIMER_DISABLE, 0)

/*
 * timer DISABLE on control register
 */
#define TIMER_DISABLEwidth  1
#define TIMER_DISABLEshift  5

/*
 * periodic timer enable on control register
 */
#define TMR_PERIODIC_TIMER_ENABLE   TMR_HCL_FIELDSET(p_tmr_register_subset->tmr_control, PERIODIC_TIMER_ENABLE, 0)

/*
 *  periodic PERIODIC_TIMER enable on control register
 */
#define PERIODIC_TIMER_ENABLEwidth  1
#define PERIODIC_TIMER_ENABLEshift  4

/*
 * periodic timer DISABLE on control register
 * SPEAR as Periodic/ One Shot are the same bits
*/
/*#define TMR_PERIODIC_TIMER_DISABLE  TMR_HCL_FIELDSET(p_tmr_register_subset->tmr_control, PERIODIC_TIMER_DISABLE, 1) */

/*
 *  periodic PERIODIC_TIMER DISABLE on control register
 */
/*
#define PERIODIC_TIMER_DISABLEwidth 1
#define PERIODIC_TIMER_DISABLEshift 4 */

/*
 * prescaler set on control register
 */
#define TMR_PRESCALER_SET(x)    TMR_HCL_FIELDSET(p_tmr_register_subset->tmr_control, PRESCALER_SET, x)
#define PRESCALER_SETwidth      4
#define PRESCALER_SETshift      0


#define TMR_CAPTURE_SET(x)   TMR_HCL_FIELDSET(p_tmr_register_subset->tmr_control, CAPTURE_SET, x)
#define CAPTURE_SETwidth   		2
#define CAPTURE_SETshift   		5


/*
 * timer one shot on control register
 */
#define TMR_TIMER_ONE_SHOT_SET(x)   TMR_HCL_FIELDSET(p_tmr_register_subset->tmr_control, TIMER_ONE_SHOT_SET, x)

/*
 *  timer one shot on control register
 */
#define TIMER_ONE_SHOT_SETwidth 1
#define TIMER_ONE_SHOT_SETshift 4

#define TIMER_AUTO_RELOAD       0
#define TIMER_ONE_SHOT          1

/*------------------------------------------------------------------------
 * Local Functions
 *----------------------------------------------------------------------*/
#ifdef __DEBUG
t_uint8         GetnAlarmsInserted(void);
void            PrintAlarmList(void);
#endif
PRIVATE void    tmr_EnterCriticalSection(t_uint32 *);
PRIVATE void    tmr_ExitCriticalSection(t_uint32);
#endif /* _HCL_TMRP_H_ */

/* End of file - tmr_p.h*/

