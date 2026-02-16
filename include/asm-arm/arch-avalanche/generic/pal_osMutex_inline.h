/*
 *
 * pal_osMutex_inline.h
 * Description:
 * see below
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


/** \file   pal_osMutex_inline.h
    \brief  OsMUTEX Services Source File

    This file declares OS abstraction services for mutually exclusive
    locks or binary semaphores. All services run in the context of the
    calling thread or program. OsMUTEX does not spawn a thread of its own 
    to implement the APIs declared here.

 
    \author     PSP Architecture Team
    \version    1.0
 */

#ifndef __PAL_OSMUTEX_INLINE_H__
#define __PAL_OSMUTEX_INLINE_H__

#include "pal_os.h"
#include "pal_defs.h"
#include "pal_osCfg.h"
#include <mach/semaphore.h>
#include <linux/slab.h>

/**
 * \defgroup PalOSMutex PAL OS Mutex Interface
 * 
 * PAL OS Mutex Interface
 * \{
 */

/** \name PAL OS Mutex Interface
 *  PAL OS Mutex Interface
 * \{
 */

/**
 * \brief   PAL OS Mutex Init
 * Nothing to initialize for Linux.
 */
PAL_INLINE PAL_Result PAL_osMutexInit(Ptr param)
{
	return PAL_SOK;
}

/**
 * \brief   PAL OS Mutex Create
 */
PAL_INLINE PAL_Result PAL_osMutexCreate(	const Char* name,
				PAL_OsMutexAttrs *attrs, 
				PAL_OsMutexHandle* hMutex)
{
    *hMutex = kmalloc(sizeof(struct semaphore), GFP_KERNEL);
    if(*hMutex == NULL) 
	{
        return PAL_OS_ERROR_NO_RESOURCES;
	}

    sema_init((struct semaphore *)*hMutex,1);
    
    return PAL_SOK;
}

/**
 * \brief   PAL OS Mutex Delete
 * This does not care about any pending threads. The onus is 
 * on the user.
 */
PAL_INLINE PAL_Result PAL_osMutexDelete(PAL_OsMutexHandle hMutex)
{
    kfree(hMutex);
    return PAL_SOK;
}

/**
 * \brief   PAL OS Mutex Lock
 * The wait should always be infinite for Linux else error is returned.
 */
PAL_INLINE PAL_Result PAL_osMutexLock(PAL_OsMutexHandle hMutex, Int32 mSecTimeout)
{
	if(mSecTimeout != PAL_OSMUTEX_NO_TIMEOUT)
		return PAL_OS_ERROR_NOT_SUPPORTED;
	
    down_interruptible((struct semaphore*)hMutex);
    return PAL_SOK;
}

/**
 * \brief   PAL OS Mutex Unlock
 */
PAL_INLINE PAL_Result PAL_osMutexUnlock(PAL_OsMutexHandle hMutex)
{
    up((struct semaphore*)hMutex);
    return PAL_SOK;
}

/**
 * \brief   PAL OS Mutex Report
 * No reporting supported by the kernel.
 */
PAL_INLINE PAL_Result PAL_osMutexReport(PAL_OsMutexHandle hMutex, PAL_OsMutexReport *report, Char* buf)
{
    return PAL_OS_ERROR_NOT_SUPPORTED;
}
		
/*\}*/
/*\}*/
#endif

