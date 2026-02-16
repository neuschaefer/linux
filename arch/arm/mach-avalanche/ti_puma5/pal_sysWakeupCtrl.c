/*
 *
 * pal_sysWakeupCtrl.c
 * Description:
 * see below
 *
 *
 * Copyright (C) 2008, Texas Instruments, Incorporated
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */

/** \file   pal_sys_wakeup_ctrl.c
    \brief  PAL wakeup control code file
	

    \author     Ajay Singh
    \version    0.1
 */
#include <mach/generic/pal.h>

/***********************************************************************
 *    Wakeup Control Module for TNETV1050 Communication Processor
 ***********************************************************************/

#define AVALANCHE_WAKEUP_POLARITY_BIT   16

void PAL_sysWakeupCtrl(PAL_SYS_WAKEUP_INTERRUPT_T wakeup_int,
                         PAL_SYS_WAKEUP_CTRL_T      wakeup_ctrl,
                         PAL_SYS_WAKEUP_POLARITY_T  wakeup_polarity)
{
#if 0 /* This is obsolete, please use PSC APIs */
    volatile unsigned int *wakeup_status_reg = (unsigned int*) AVALANCHE_WAKEUP_CTRL_WKCR;

    /* enable/disable */
    if (wakeup_ctrl == WAKEUP_ENABLED)
	{
        /* enable wakeup */
        *wakeup_status_reg |= wakeup_int;
	}
    else
	{
        /* disable wakeup */
        *wakeup_status_reg &= (~wakeup_int);
	}

    /* set polarity */
    if (wakeup_polarity == WAKEUP_ACTIVE_LOW)
	{
        *wakeup_status_reg |=  (wakeup_int << AVALANCHE_WAKEUP_POLARITY_BIT);
	}
    else
	{
        *wakeup_status_reg &= ~(wakeup_int << AVALANCHE_WAKEUP_POLARITY_BIT);
	}
#endif
}

