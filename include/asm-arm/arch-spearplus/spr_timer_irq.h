/*
 * linux/include/asm/arch/spr_timer_irq.h
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA*/
#ifndef _TMR_IRQ_H_
#define _TMR_IRQ_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*--------------------------------------------------------------------------*
 * Includes																	*
 *--------------------------------------------------------------------------*/

#include <asm/arch/hcl_defs.h>  

/*------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------*/
/*------------------------------------------------------------------------
 * New types
 *----------------------------------------------------------------------*/
typedef enum
{
    TMR_DEVICE_ID_INVALID = -1,
    TMR_DEVICE_ID_0       = 0,
    TMR_DEVICE_ID_1       = 1,
    TMR_DEVICE_ID_2       = 2,
    TMR_DEVICE_ID_3       = 3
} t_tmr_device_id;    /* SPEAR See the use of it */


typedef enum
{
    TMR_ID_0  = 0x00000001,
    TMR_ID_1  = 0x00000008,
    TMR_ID_2  = 0x00000040,
    TMR_ID_3  = 0x00000200,
    TMR_ID_4  = 0x00001000,
    TMR_ID_5  = 0x00008000,
    TMR_ID_6  = 0x00040000,
    TMR_ID_7  = 0x00200000
} t_tmr_id;

typedef enum
{
    TMR_IRQ_SRC_ID_0 = TMR_ID_0,
    TMR_IRQ_SRC_ID_1 = TMR_ID_1,
    TMR_IRQ_SRC_ID_2 = TMR_ID_2,
    TMR_IRQ_SRC_ID_3 = TMR_ID_3,
    TMR_IRQ_SRC_ID_4 = TMR_ID_4,
    TMR_IRQ_SRC_ID_5 = TMR_ID_5,
    TMR_IRQ_SRC_ID_6 = TMR_ID_6,
    TMR_IRQ_SRC_ID_7 = TMR_ID_7
} t_tmr_irq_src_id;

typedef t_uint32    t_tmr_irq_src;

/*------------------------------------------------------------------------
 * Local Functions
 *----------------------------------------------------------------------*/
void         TMR_SetBaseAddress(t_tmr_device_id tmr_device_id, t_logical_address tmr_base_address);
t_tmr_irq_src       TMR_GetIRQSrc(t_tmr_device_id tmr_device_id);
void                TMR_EnableIRQSrc(t_tmr_irq_src tmr_irq_src);
void                TMR_DisableIRQSrc(t_tmr_irq_src tmr_irq_src);
void                TMR_ClearIRQSrc(t_tmr_irq_src tmr_irq_src);
t_tmr_device_id     TMR_GetDeviceID(t_tmr_irq_src tmr_irq_src);
t_bool              TMR_IsPendingIRQSrc(t_tmr_irq_src tmr_irq_src);

#ifdef __cplusplus
}   /* allow C++ to use these head  ers */
#endif /* __cplusplus */
#endif /* #ifndef _TMR_IRQ_H_ */

/* End of file - tmr_irq.h*/

