/*
 * linux/include/asm/arch/spr_timer.h
 *
 *  Copyright (C) 2006 ST MICROELECTRONICS
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA*/
#ifndef _TMR_H_
#define _TMR_H_

#include "debug.h"
#include "spr_timer_irq.h"


/*------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------*/
typedef t_uint32    t_tmr_event;
typedef t_uint32    t_tmr_filter_mode;

#define TMR_NO_FILTER_MODE  0

/*------------------------------------------------------------------------
 * Data types
 *----------------------------------------------------------------------*/
typedef enum
{
    TMR_INTERNAL_EVENT = HCL_INTERNAL_EVENT ,
    TMR_REMAINING_PENDING_EVENTS = HCL_REMAINING_PENDING_EVENTS ,
    TMR_NO_MORE_PENDING_EVENT = HCL_NO_MORE_PENDING_EVENT ,
    TMR_NO_MORE_FILTER_PENDING_EVENT = HCL_NO_MORE_FILTER_PENDING_EVENT ,
    TMR_NO_PENDING_EVENT_ERROR = HCL_NO_PENDING_EVENT_ERROR,
    TMR_OK = 0,
	TMR_INVALID_PARAMETER= -1,
    TMR_ERROR_NO_MORE_TIMER_AVAILABLE = -2,
    TMR_ERROR_INVALID_TIMER_ID = -3,
    TMR_ERROR_TIMER_ALREADY_STOPPED = -4,
    TMR_ERROR_TIMER_ALREADY_STARTED = -5,
    TMR_ERROR_MAX_RUNNING_TIMER_REACHED = -6,
    TMR_ERROR_PERIPHERAL_IDENTIFICATION_FAILED = -7,
    TMR_ERROR_INVALID_STATE  = -8 ,
    TMR_ERROR_INVALID_VALUE  = -9 ,
    TMR_ERROR_NOT_ONE_SHOT   = -10,
}t_tmr_error;

typedef enum
{
    TMR_MODE_AUTO_RELOAD          ,
	TMR_MODE_ONE_SHOT
} t_tmr_mode;



typedef enum
{
    TMR_MODE_NO_CAPTURE,
	TMR_MODE_CAPT_REDGE ,
    TMR_MODE_CAPT_FEDGE,
	TMR_MODE_CAPT_RFEDGE,
} t_tmr_capt_mode;

typedef enum
{
    TMR_CLK_PRESCALER_1,
	TMR_CLK_PRESCALER_2,
    TMR_CLK_PRESCALER_4,
	TMR_CLK_PRESCALER_8,
    TMR_CLK_PRESCALER_16,
	TMR_CLK_PRESCALER_32,

	TMR_CLK_PRESCALER_64,
	TMR_CLK_PRESCALER_128,
    TMR_CLK_PRESCALER_256
} t_tmr_clk_prescaler;

typedef enum
{
    TMR_IRQ_STATE_NEW,
    TMR_IRQ_STATE_OLD
} t_tmr_irq_state;

typedef struct
{
    t_tmr_irq_state interrupt_state;
    t_uint8         padding[3];
    t_tmr_irq_src   initial_irq;
    t_tmr_irq_src   pending_irq;
} t_tmr_irq_status;

/*------------------------------------------------------------------------
 * Public Functions
 *----------------------------------------------------------------------*/

t_tmr_error TMR_Init(t_tmr_device_id tmr_device_id, t_logical_address tmr_base_address);

t_tmr_error TMR_Reset(t_tmr_device_id tmr_device_id);
t_tmr_error TMR_AllocTimer(t_tmr_id *p_tmr_id);

t_tmr_error TMR_Configure
            (
                t_tmr_id            tmr_id,
                t_tmr_mode          tmr_mode,
                t_tmr_capt_mode     tmr_capt_mode,
                t_uint16            load_value,
                t_tmr_clk_prescaler tmr_clk_prescaler
            );
t_tmr_error TMR_StartTimer(t_tmr_id tmr_id);
t_tmr_error TMR_StopTimer(t_tmr_id tmr_id);
t_tmr_error TMR_ReStartTimer(t_tmr_id tmr_id, t_uint16 load_value, t_tmr_clk_prescaler tmr_clk_prescaler);
t_tmr_error TMR_GetCounterValue(t_tmr_id tmr_id, t_uint32 *p_current_value);

/*SPEAR: added for Fedge Redge Coutner values */
t_tmr_error TMR_GetRedgeCounterValue(t_tmr_id tmr_id, t_uint16 *p_current_value);
t_tmr_error TMR_GetFedgeCounterValue(t_tmr_id tmr_id, t_uint16 *p_current_value);

t_tmr_error TMR_FreeTimer(t_tmr_id tmr_id);

t_tmr_error TMR_SetDbgLevel(t_dbg_level dbg_level);
t_tmr_error TMR_GetDbgLevel(t_dbg_level *dbg_level);

void        TMR_SaveDeviceContext(void);
void        TMR_RestoreDeviceContext(void);

t_tmr_error TMR_GetVersion(t_version *p_version);

void        TMR_GetIRQSrcStatus(t_tmr_irq_src tmr_irq_src, t_tmr_irq_status *p_status);
t_tmr_error TMR_FilterProcessIRQSrc
            (
                t_tmr_irq_status    *p_tmr_irq_status,
                t_tmr_event         *p_tmr_event,
                t_tmr_filter_mode   filter_mode
            );
t_bool      TMR_IsEventActive(const t_tmr_irq_src *p_event);
void        TMR_AcknowledgeEvent(const t_tmr_event *p_event);
#endif /* #ifndef _HCL_TMR_H_ */

/* End of file - tmr.h*/

