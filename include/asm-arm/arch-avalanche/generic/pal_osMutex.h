/*
 *
 * pal_osMutex.h
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


/** \file   pal_osMutex.h
    \brief  OsMUTEX Services Header File

    This file declares OS abstraction services for mutually exclusive
    locks or binary semaphores. All services run in the context of the
    calling thread or program. OsMUTEX does not spawn a thread of its own 
    to implement the APIs declared here.


    \author     PSP Architecture Team
    \version    1.0
 */

#ifndef __PAL_OSMUTEX_H__
#define __PAL_OSMUTEX_H__

#include "pal_defs.h"
#include "pal_os.h"

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

/* Blocking call without timeout */
#define PAL_OSMUTEX_NO_TIMEOUT    (-1)

/**
 * \brief   PAL OS Mutex Init
 *
 *      This is an idempotent function that must be called ahead of
 *      calling any other OsMUTEX services. It initializes OsMUTEX internal
 *      data structures and does any book-keep necessary to implement
 *      the published services of OsMUTEX.
 * \param   param [IN] is an arbitrary void* data type used to pass platform
 *      specific initialization information for OsMUTEX. This can be used
 *      to extend OsMUTEX configurability to decisions made at run-time.
 *      This is added for future extensions only. 
 * \return  PAL_SOK if successful else a suitable error code.
 */
PAL_INLINE PAL_Result PAL_osMutexInit(Ptr param);

/**
 * \brief Mutex Attributes
 *
 * Attributes specified during Mutex creation
 */

typedef enum 
{
  PAL_OSMUTEX_TYPE_FIFO = 0,
  PAL_OSMUTEX_TYPE_PRIORITY = 1
} PAL_OsMutexType;

typedef Ptr PAL_OsMutexHandle;

typedef struct 
{
  PAL_OsMutexType  type;
  Uint32 memSegId; /**< identifier of memory segment for mutex variables */
} PAL_OsMutexAttrs;

/**
 * \brief   PAL OS Mutex Create
 *
 *      This function creates a mutex with specified attributes
 * \param   name [IN] is char string name of the mutex
 * \param   attrs [IN] is the mutex attributes ex: memory segment from
 *      where this particular mutex object must be allocated from.
 * \param   hMutex [OUT] is location to recieve the handle to just created
 *      mutex object.
 * \return  PAL_SOK if succesful, else a suitable error code.
 */
PAL_INLINE PAL_Result PAL_osMutexCreate(const Char* name,
                           PAL_OsMutexAttrs *attrs, 
                           PAL_OsMutexHandle *hMutex);

/**
 * \brief   PAL OS Mutex Delete
 *
 *      This function deletes or removes the specified mutex
 *      from the system. Associated dynamically allocated memory
 *      if any is also freed up.
 * \param   hMutex [IN] handle to the mutex to be deleted
 * \return  PAL_SOK if succesful else a suitable error code
 */
PAL_INLINE PAL_Result PAL_osMutexDelete(PAL_OsMutexHandle hMutex);

/**
 * \brief   PAL OS Mutex Lock
 * 
 *      This function acquires a lock on the given Mutex. If the
 *      specified mutex is already locked by someone else, the calling
 *      thread will block on this function for a maximum duration of
 *      mSecTimeout. If the mutex continues to be unavailable even
 *      after waiting for specified duration, the function will return
 *      with a suitable error code.
 * \param   hMutex [IN] is the handle of the specified mutex
 * \param   mSecTimeout [IN] is wait time in milliseconds
 * \return  PAL_SOK if successful else a suitable error code
 */
PAL_INLINE PAL_Result PAL_osMutexLock(PAL_OsMutexHandle hMutex, Int32 mSecTimeout);

/**
 * \brief   PAL OS Mutex Unlock
 *
 *      This function releases an already acquired mutex lock
 * \param   hMutex [IN] is the handle of the specified mutex
 * \return  PAL_SOK if successful else a suitable error code
 */
PAL_INLINE PAL_Result PAL_osMutexUnlock(PAL_OsMutexHandle hMutex);

/**
 * \brief Mutex Report Data Structure
 *
 * Data structure for Mutex Report service
 */
typedef struct 
{
    Char* name;             /**< Name of mutex lock */
    Bool  free;             /**< =FALSE if locked state */
    PAL_OsMutexAttrs attrs; /**< Mutex Attribues */
} PAL_OsMutexReport;

/**
 * \brief   PAL OS Mutex Report
 *
 *      This function reports assorted usage statistics information
 *      regarding the specified mutex
 * \param   hMutex [IN] handle of mutex to be reported on.
 * \param   report [IN/OUT] location where mutex information must be reported
 *      If NULL, structure is not filled.
 * \param   buf [IN/OUT] string buffer where a text formatted report will
 *      be printed for the mutex. If NULL, no text style reporting is done.
 * \return  PAL_SOK if successful, else a suitable error code.
 */
PAL_INLINE PAL_Result PAL_osMutexReport(PAL_OsMutexHandle hMutex, PAL_OsMutexReport *report, Char* buf);

/*\}*/
/*\}*/

#endif /* _PAL_OSMUTEX_H_ */
