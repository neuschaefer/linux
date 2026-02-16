/*
 *
 * pal_os.h
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


/** \file   pal_os.h
    \brief  OS Abstraction Header File

    This file provides visibility to OS abstraction APIs by including
    only the configured service modules interface files.


    \author     PSP Architecture Team
    \version    1.0
 */

#ifndef __PAL_OS_H__
#define __PAL_OS_H__


#ifdef CONFIG_ARM_AVALANCHE_SOC
#include "pal_osCfg.h"
#else /*AP-CPU*/
#include "linux/avalanche/generic/pal_osCfg.h"
#endif


/* Added:
 * PAL OS module type to be placed in section D of the final "ERROR CODE"
 * as described in pal_defs.h
 */
#define PAL_OS_COMMON_ERR   (0)
#define PAL_OSMEM_ERR       (1)
#define PAL_OSBUF_ERR       (2)
#define PAL_OSSEM_ERR       (3)
#define PAL_OSMUTEX_ERR     (4)
#define PAL_OSWAIT_ERR      (5)
#define PAL_OSLIST_ERR      (6)
#define PAL_OSPROTECT_ERR   (7)
#define PAL_OSTIMER_ERR     (8)

/* Common error codes for ALL PAL OS modules */
#define PAL_OS_COMMON_ERROR_CREATE(x)   (PAL_ERROR(PAL_CRITICAL_ERROR, PAL_OS_COMMON_ERR, 0, (x)))

/* Invalid parameter passed to the function error */
#define PAL_OS_ERROR_INVALID_PARAM      (PAL_OS_COMMON_ERROR_CREATE(1))

/* Feature not supported error */
#define PAL_OS_ERROR_NOT_SUPPORTED      (PAL_OS_COMMON_ERROR_CREATE(2))

/* No resources available error */
#define PAL_OS_ERROR_NO_RESOURCES       (PAL_OS_COMMON_ERROR_CREATE(3))

/* OS specific error */
#define PAL_OS_ERROR_OS_SPECIFIC        (PAL_OS_COMMON_ERROR_CREATE(4))


/* Default (memory) segment Id - Many of the modules (like OSSEM, OSBUF),
 * depend upon a segment id to be passed in the API's. The macro below
 * defines a default segment Id that can be used in these API's
 */
#define PAL_OSMEM_DEFAULT_SEGID         0


#ifdef INLINE
#define PAL_INLINE static inline
#else
#define PAL_INLINE
#endif

#ifdef PAL_INCLUDE_OSMEM
#ifdef CONFIG_ARM_AVALANCHE_SOC
#include "pal_osMem.h"          /* OsMEM Services */
#else /*AP-CPU*/
#include "linux/avalanche/generic/pal_osMem.h"          /* OsMEM Services */
#endif
#endif /* PAL_INCLUDE_OSMEM */

#ifdef PAL_INCLUDE_OSBUF
#include "pal_osBuf.h"          /* OsBUF Services */
#endif /* PAL_INCLUDE_OSBUF */

#ifdef PAL_INCLUDE_OSSEM
#include "pal_osSem.h"          /* OsSEM Services */
#endif /* PAL_INCLUDE_OSSEM */

#ifdef PAL_INCLUDE_OSMUTEX
#include "pal_osMutex.h"        /* OsMUTEX Services */
#endif /* PAL_INCLUDE_OSMUTEX */

#ifdef PAL_INCLUDE_OSWAIT
#include "pal_osWait.h"        /* OsWAIT Services */
#endif /* PAL_INCLUDE_OSWAIT */

#ifdef PAL_INCLUDE_OSLIST
#include "pal_osList.h"         /* OsLIST Services */
#endif /* PAL_INCLUDE_OSLIST */

#ifdef PAL_INCLUDE_OSPROTECT
#include "pal_osProtect.h"      /* OsPROTECT Services */
#endif /* PAL_INCLUDE_OSPROTECT */

#ifdef PAL_INCLUDE_OSCACHE
#include "pal_osCache.h"      /* OsCACHE Services */
#endif /* PAL_INCLUDE_OSCACHE */

#ifdef PAL_INCLUDE_OSTIMER
#include "pal_osTimer.h"        /* OsSEM Services */
#endif /* PAL_INCLUDE_OSTIMER */

#ifdef INLINE

#ifdef PAL_INCLUDE_OSBUF
#include "pal_osBuf_inline.h"
#endif /* PAL_INCLUDE_OSBUF */

#ifdef PAL_INCLUDE_OSMEM
#include "pal_osMem_inline.h"
#endif /* PAL_INCLUDE_OSMEM */

#ifdef PAL_INCLUDE_OSMUTEX
#include "pal_osMutex_inline.h"
#endif /* PAL_INCLUDE_OSMUTEX */

#ifdef PAL_INCLUDE_OSSEM
#include "pal_osSem_inline.h"
#endif /* PAL_INCLUDE_OSSEM */

#ifdef PAL_INCLUDE_OSWAIT
#include "pal_osWait_inline.h"
#endif /* PAL_INCLUDE_OSWAIT */

#ifdef PAL_INCLUDE_OSPROTECT
#include "pal_osProtect_inline.h"
#endif /* PAL_INCLUDE_OSPROTECT */

#ifdef PAL_INCLUDE_OSLIST
#include "pal_osList_inline.h"
#endif /* PAL_INCLUDE_OSLIST */

#ifdef PAL_INCLUDE_OSCACHE
#include "pal_osCache_inline.h"      /* OsCACHE Services */
#endif /* PAL_INCLUDE_OSCACHE */

#ifdef PAL_INCLUDE_OSTIMER
#include "pal_osTimer_inline.h"
#endif /* PAL_INCLUDE_OSTIMER */

#endif /* INLINE */

#endif /* _PAL_OS_H_ */

