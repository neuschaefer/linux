/*
 * pal_osTimer.h
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

/** \file   pal_osTimer.h
    \brief  OsTIMER Services Header File

    This file declares OS abstraction services for OS Timers.
    All services run in the context of the calling thread or program.
    OsTIMER does not spawn a thread of its own to implement the APIs
    declared here.

 
    \author     PSP Architecture Team
    \version    1.0
 */

#ifndef __PAL_OSTIMER_H__
#define __PAL_OSTIMER_H__

#include "pal_defs.h"
#include "pal_os.h"

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

typedef Ptr PAL_OsTimerHandle;
typedef void (*PAL_OsTimerFunc) (unsigned long);


/**
 * \brief   PAL OS Timer Create
 * 
 *      This function creates and initializes a timer with user provided timer
 *      function. 
 * \warn    Note that the timer function is called in (software) interrupt
 *          context and thus all the constraints related to interrupt handlers
 *          are applicable.
 * \param   pfn [IN] pointer to timer function 
 * \param   arg [IN] parameter to be passed to timer function
 * \param   phTimer [OUT] location to recieve the handle to timer just created
 * \return  PAL_SOK if succesful, else a suitable error code.
 */
PAL_INLINE PAL_Result PAL_osTimerCreate(PAL_OsTimerFunc pfn, 
                                Uint32 arg, 
                                PAL_OsTimerHandle* phTimer);

/**
 * \brief   PAL OS Timer Destroy
 * 
 *      This function destroys the specified timer. The timer is stopped if
 *      active and data associated is cleaned up. 
 * \param   hTimer [IN] handle to the timer
 * \return  PAL_SOK if succesful, else a suitable error code.
 */
PAL_INLINE PAL_Result PAL_osTimerDestroy(PAL_OsTimerHandle hTimer);

/**
 * \brief   PAL OS Timer Start
 * 
 *      This function activates the specified timer. The timer is areloaded
 *      with new timeout if already started.
 * \param   hTimer [IN] handle to the timer
 * \param   msec [IN] timeout in milli seconds before the timer expires
 * \return  PAL_SOK if succesful, else a suitable error code.
 */
PAL_INLINE PAL_Result PAL_osTimerStart(PAL_OsTimerHandle hTimer, Uint32 msec);

/**
 * \brief   PAL OS Timer Stop
 * 
 *      This function deactivates the specified timer. 
 * \param   hTimer [IN] handle to the timer
 * \return  PAL_SOK if succesful, else a suitable error code.
 */
PAL_INLINE PAL_Result PAL_osTimerStop(PAL_OsTimerHandle hTimer);

/*@}*/
/*@}*/

#endif /* _PAL_OSSTIMER_H_ */
