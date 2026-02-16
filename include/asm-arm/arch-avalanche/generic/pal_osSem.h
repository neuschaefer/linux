/*
 *
 * pal_osSem.h
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

/** \file   pal_osSem.h
    \brief  OsSEM Services Header File

    This file declares OS abstraction services for counting semaphores.
    All services run in the context of the calling thread or program.
    OsSEM does not spawn a thread of its own to implement the APIs
    declared here.

 
    \author     PSP Architecture Team
    \version    1.0
 */

#ifndef __PAL_OSSEM_H__
#define __PAL_OSSEM_H__

#include "pal_defs.h"
#include "pal_os.h"

/**
 * \defgroup PalOSSem PAL OS Semaphore Interface
 * 
 * PAL OS Semaphore Interface
 * @{
 */

/** \name PAL OS Semaphore Interface
 *  PAL OS Semaphore Interface
 * @{
 */

/* Blocking call without timeout */
#define PAL_OSSEM_NO_TIMEOUT    (-1)

/**
 * \brief   PAL OS Semaphore Init
 * 
 *      This is an idempotent function that must be called ahead of
 *      calling any other OsSEM services. It initializes OsSEM internal
 *      data structures and does any book-keep necessary to implement
 *      the published services of OsSEM.
 * \param   param [IN] is an arbitrary void* data type used to pass platform
 *      specific initialization information for OsSEM. This can be used
 *      to extend OsSEM configurability to decisions made at run-time.
 *      This is added for future extensions only. 
 * \return  PAL_SOK if successful else a suitable error code.
 */
PAL_INLINE PAL_Result PAL_osSemInit(Ptr param);

typedef enum 
{
  PAL_OSSEM_TYPE_FIFO = 0,
  PAL_OSSEM_TYPE_PRIORITY = 1
} PAL_OsSemType;

typedef Ptr PAL_OsSemHandle;

typedef struct 
{
  PAL_OsSemType  type;
  Uint32 memSegId;
} PAL_OsSemAttrs;

/**
 * \brief   PAL OS Semaphore Create
 * 
 *      This function creates a counting semaphore with specified
 *      attributes and initial value.
 * \param   name [IN] is char string name of the semaphore
 * \param   initVal [IN] is initial value for semaphore
 * \param   attrs [IN] is the semaphore attributes ex: Fifo type
 * \param   hSem [OUT] is location to recieve the handle to just created
 *      semaphore
 * \return  PAL_SOK if succesful, else a suitable error code.
 */
PAL_INLINE PAL_Result PAL_osSemCreate(const char* name,
                           Int32 initVal,
                           PAL_OsSemAttrs *attrs, 
                           PAL_OsSemHandle *hSem);

/**
 * \brief   PAL OS Semaphore Delete
 * 
 *      This function deletes or removes the specified semaphore
 *      from the system. Associated dynamically allocated memory
 *      if any is also freed up.
 * \warning OsSEM services run in client context and not in a thread
 *      of their own. If there exist threads pended on a semaphore
 *      that is being deleted, results are undefined.
 * \param   hSem [IN] handle to the semaphore to be deleted
 * \return  PAL_SOK if succesful else a suitable error code
 */
PAL_INLINE PAL_Result PAL_osSemDelete(PAL_OsSemHandle hSem);

/**
 * \brief   PAL OS Semaphore Take
 * 
 *      This function takes a semaphore token if available.
 *      If a semaphore is unavailable, it blocks currently
 *      running thread in wait (for specified duration) for
 *      a free semaphore.
 * \param   hSem [IN] is the handle of the specified semaphore
 * \param   mSecTimeout [IN] is wait time in milliseconds
 * \return  PAL_SOK if successful else a suitable error code
 */
PAL_INLINE PAL_Result PAL_osSemTake(PAL_OsSemHandle hSem, Int32 mSecTimeout);

/**
 * \brief   PAL OS Semaphore Give
 * 
 *      This function gives or relinquishes an already
 *      acquired semaphore token
 * \param   hSem [IN] is the handle of the specified semaphore
 * \return  PAL_SOK if successful else a suitable error code
 */
PAL_INLINE PAL_Result PAL_osSemGive(PAL_OsSemHandle hSem);

/**
 * \brief Semaphore Report Data Structure
 *
 * Data structure for Semaphore Report service
 */
typedef struct 
{
    char* name;
    Int32 initVal;
    Int32 currentVal;
    PAL_OsSemAttrs attrs;
} PAL_OsSemReport;

/**
 * \brief   PAL OS Semaphore Report
 * 
 *      This function reports assorted usage statistics information
 *      regarding the specified semaphore
 * \param   hSem [IN] handle of semaphore to be reported on.
 * \param   report [IN/OUT] location where information must be reported
 *      If NULL, structure is not filled.
 * \param   buf [IN/OUT] string buffer where a text formatted report will
 *      be printed. If NULL, no text style reporting is done.
 * \return  PAL_SOK if successful, else a suitable error code.
 */
PAL_INLINE PAL_Result PAL_osSemReport(PAL_OsSemHandle hSem, PAL_OsSemReport *report, Char* buf);

/*@}*/
/*@}*/

#endif /* _PAL_OSSEM_H_ */
