/*
 *
 * pal_osProtect_inline.h
 * Description:
 * see below
 *
 *

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2008-2017 Intel Corporation.

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

  Copyright(c) 2008-2017 Intel Corporation. All rights reserved.

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

/** \file   pal_osProtect_inline.h
    \brief  OsPROTECT Services Source File


    \author     PSP Architecture Team
    \version    1.0
 */

#ifndef __PAL_OSPROTECT_INLINE_H__
#define __PAL_OSPROTECT_INLINE_H__

#include "pal_os.h"
#include "pal_defs.h"
#include "pal_osCfg.h"
#include <linux/spinlock.h>
#include <linux/slab.h>

/**
 * \defgroup PalOSProtect PAL OS Protect Interface
 *
 * PAL OS Protect Interface
 * \{
 */

/** \name PAL OS Protect Interface
 *  PAL OS Protect Interface
 * \{
 */


/**
 * \brief   PAL OS Protect Entry
 *
 *      This function saves the current state of protection in cookie
 *      variable passed by caller. It then applies the requested level
 *      of protection
 * \param   level is bitmask identifier of the desired type of
 *  		protection.
 * \param   cookie is memory location where current state of protection is
 *      saved for future use while restoring it via PAL_osProtectExit()
 * \note    user is not expected to interpret the cookie in any manner. It
 *      is intended for use in terminating the presently enforced
 *      protection via a matching PAL_osProtectExit() call discssed
 *      later in this file.
 * \return  None
 */
PAL_INLINE void PAL_osProtectEntry(Uint32 level, Uint32* cookie)
{
    Ulong flags;

    if(level & PAL_OSPROTECT_SCHEDULER)
    {
        preempt_disable();
    }

    if(level & PAL_OSPROTECT_INTERRUPT)
    {
        local_irq_save(flags);
           *cookie = flags;
    }
}

/**
 * \brief   PAL OS Protect Exit
 *
 *      This function undoes the protection enforced to original state
 *      as is specified by the cookie passed.
 * \param   level is numeric identifier of the desired degree of protection.
 * \param   cookie is original state of protection at time when the
 *      corresponding PAL_osProtectEnter() was called.
 * \return  None
 */
PAL_INLINE void PAL_osProtectExit(Uint32 level, Uint32 cookie)
{
    if(level & PAL_OSPROTECT_INTERRUPT)
    {
        local_irq_restore((Ulong)cookie);
    }

    if(level & PAL_OSPROTECT_SCHEDULER)
    {
        preempt_enable();
    }
}

#endif

