/*
 * pal_osTimer_inline.h
 * Description:
 * See below.
 *
 *

  This file is provided under a dual BSD/GPLv2 license.  When using or 
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2008-2014 Intel Corporation.

  This program is free software; you can redistribute it and/or modify 
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but 
  WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
  General Public License for more details.

  You should have received a copy of the GNU General Public License 
  along with this program; if not, write to the Free Software 
  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
  The full GNU General Public License is included in this distribution 
  in the file called LICENSE.GPL.


  Contact Information:
  Intel Corporation
  2200 Mission College Blvd.
  Santa Clara, CA  97052

  BSD LICENSE 

  Copyright(c) 2008-2014 Intel Corporation. All rights reserved.

  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions 
  are met:

    * Redistributions of source code must retain the above copyright 
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright 
      notice, this list of conditions and the following disclaimer in 
      the documentation and/or other materials provided with the 
      distribution.

    * Neither the name of Intel Corporation nor the names of its 
      contributors may be used to endorse or promote products derived 
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/** \file   pal_osTimer_inline.h
    \brief  OsTIMER Services Source File

    This file implements the OsTIMER services for Linux.


    \author     PSP Architecture Team
    \version    0.1
*/

#ifndef __PAL_OSTIMER_INLINE_H__
#define __PAL_OSTIMER_INLINE_H__

#include "pal_os.h"
#include "pal_defs.h"
#include <linux/timer.h>

/**
 * \defgroup PalOSTimer PAL OS Timer Interface
 * 
 * PAL OS Timer Interface
 * @{
 */

/** \name PAL OS Timer Interface
 *  PAL OS Timer Interface
 * @{
 */

/**
 * \brief   PAL OS Timer Create 
 */
PAL_INLINE PAL_Result PAL_osTimerCreate(PAL_OsTimerFunc pfn, 
                                Uint32 arg, 
                                PAL_OsTimerHandle* phTimer)
{
    if ((*phTimer =  kmalloc(sizeof(struct timer_list),GFP_KERNEL)) == NULL)
	{
        return PAL_OS_ERROR_NO_RESOURCES;
	}

    setup_timer((struct timer_list *) *phTimer, pfn, arg);
    return PAL_SOK;
}

/**
 * \brief   PAL OS Timer Destroy
 */
PAL_INLINE PAL_Result PAL_osTimerDestroy(PAL_OsTimerHandle hTimer)
{
    del_timer((struct timer_list*) hTimer); 
    kfree(hTimer);
    return PAL_SOK;
}

/**
 * \brief   PAL OS Timer Activate
 */
PAL_INLINE PAL_Result PAL_osTimerStart(PAL_OsTimerHandle hTimer, Uint32 msec)
{
    mod_timer((struct timer_list*) hTimer, msecs_to_jiffies(msec) + jiffies); 
    return 0;
}

/**
 * \brief   PAL OS Timer Deactivate
 */
PAL_INLINE PAL_Result PAL_osTimerStop(PAL_OsTimerHandle hTimer)
{
    del_timer((struct timer_list*) hTimer); 
    return 0;
}

/*@}*/
/*@}*/
#endif /* !__PAL_OSTIMER_INLINE_H__ */ 

