/*
 *
 * pal_osMem_inline.h
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


/** \file   pal_osMem_inline.h
    \brief  OsMEM Services Source File

    This file implements the OsMEM services for Linux.


    \author     PSP Architecture Team
    \version   0.1
*/

#ifndef __PAL_OSMEM_INLINE_H__
#define __PAL_OSMEM_INLINE_H__

#ifdef CONFIG_ARM_AVALANCHE_SOC
#include "pal_os.h"
#include "pal_defs.h"
#include <asm/page.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <asm/memory.h>
#else /*AP-CPU*/
#include "linux/avalanche/generic/pal_os.h"
#include "linux/avalanche/generic/pal_defs.h"
#include <asm/page.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <asm/io.h>
#endif


/**
 * \brief PAL OS MEM Init
 */
PAL_INLINE PAL_Result PAL_osMemInit (Ptr param)
{
    return PAL_SOK;
}

/**
 * \brief PAL OS Memory Segment Define
 * For Linux, the whole available memory is considered into one segment.
 * No further segments are made in the memory. This function designates that
 * segment with id 0.
 */
PAL_INLINE PAL_Result PAL_osMemSegDefine (    const char * name,
                                Uint32 startAddr,
                                Uint32 numBytes,
                                PAL_OsMemAttrs * attrs,
                                Uint32 *segId)
{
    /* always return the segment id 0.
     * Only one segment is defined and used
     */
    *segId = 0;
    return PAL_SOK;
}

/**
 * \brief PAL OS Memory Segment Undefine
 * Only segment 0 is recognized.
 */
PAL_INLINE PAL_Result PAL_osMemSegUndefine (Uint32 segId)
{
    return PAL_SOK;
}

/**
 * \brief PAL OS Memory Alloc
 * Only segment 0 is recognized.
 * This function allocates only contiguous memory.
 * specify alignment as 0 if all you
 */
PAL_INLINE PAL_Result PAL_osMemAlloc (
                Uint32 segId,
                Uint32 numBytes,
                Uint16 alignment,
                Ptr* memAddr)
{
    *memAddr = kmalloc(numBytes, GFP_KERNEL);

    if(*memAddr == NULL)
    {
        return PAL_OS_ERROR_NO_RESOURCES;
    }

     return PAL_SOK;
}

/**
 * \brief PAL OS Memory Free
 * Only segment 0 is recognized.
 */
PAL_INLINE PAL_Result PAL_osMemFree (Uint32 segId, Ptr memAddr, Uint32 numBytes)
{
    kfree(memAddr);
    return PAL_SOK;
}

/**
 * \brief PAL OS Memory Copy
 * \note This will misbehave if presented with invalid arguments.
 */
PAL_INLINE PAL_Result PAL_osMemCopy (Ptr dest, const Ptr src, Uint32 numBytes)
{
    memcpy(dest, src, numBytes);
    return PAL_SOK;
}

/**
 * \brief PAL OS Memory Set
 * This will crash if presented with invalid arguments.
 */
PAL_INLINE PAL_Result PAL_osMemSet (Ptr memAddr, Char fillVal, Uint32 numBytes)
{
    memset(memAddr, fillVal, numBytes);
    return PAL_SOK;
}

/**
 * \brief PAL OS Memory Lock
 * In linux, memory locking/Unlocking is supported only at page granularity.
 * This implementation, locks all the pages from memAddr to memAddr + byteLen
 * pages containing both addresses inclusive.
 */
PAL_INLINE PAL_Result PAL_osMemLock (Ptr memAddr, Uint32 byteLen, Uint32 *cookie)
{
    Uint32 temp;

    /*
     * Get the page associated with the memory address
     * and set the reserved bit for that page
     */
    for (temp = (Uint32)memAddr; temp < PAGE_ALIGN((Uint32)memAddr+byteLen); temp += PAGE_SIZE)
	{
        SetPageReserved(virt_to_page(temp));
    }
    return PAL_SOK;
}

/**
 * \brief PAL OS Memory Un-Lock
 * In linux, memory locking/Unlocking is supported only at page granularity.
 * This implementation, locks all the pages from memAddr to memAddr + byteLen
 * pages containing both addresses inclusive.
 */
PAL_INLINE PAL_Result PAL_osMemUnlock (Ptr memAddr, Uint32 byteLen, Uint32 *cookie)
{
    Uint32 temp;

    /*
     * Get the page associated with the memory address
     * and unset the reserved bit for that page
     */
    for (temp = (Uint32)memAddr; temp < PAGE_ALIGN((Uint32)memAddr+byteLen); temp += PAGE_SIZE)
	{
        ClearPageReserved(virt_to_page(temp));
    }
    return PAL_SOK;
}

/**
 * \brief PAL OS Memory Virtual To Physical
 */
#ifdef CONFIG_ARM_AVALANCHE_SOC
PAL_INLINE Uint32 PAL_osMemVirt2Phy (Ptr virtAddress)
{
    return (Uint32) __virt_to_phys((Uint32)virtAddress);
}

/**
 * \brief PAL OS Memory Physical To Virtual
 */
PAL_INLINE Ptr PAL_osMemPhy2Virt (Uint32 phyAddress)
{

    return (Ptr) __phys_to_virt(phyAddress);
}
#endif

/**
 * \brief PAL OS Memory Report
 * No reporting is supported inherently by the Linux kernel.
 */
PAL_INLINE PAL_Result PAL_osMemReport (Uint32 segId, PAL_OsMemReport * report, Char *buf)
{
    return PAL_OS_ERROR_NOT_SUPPORTED;
}

/**
 *  \brief PAL os Memory Allocation API. Allocated memory
 *  memory will be aligned to requested size. For smaller
 *  memory chunks (less than page size) use PAL_osMemAlloc
 */

PAL_INLINE void* PAL_osMemAllocSizeAligned(Uint32 segId, Uint32 numBytes)
{
    Uint32 order;
    Uint32 ret;

    /* find number of pages */
    numBytes = (numBytes/PAGE_SIZE) + ((numBytes % PAGE_SIZE)?1:0);

    /* find allocation order */
    for(order = 0; (1 << order) < numBytes; order++);

    ret = __get_free_pages(GFP_KERNEL, order);

    /* really defensive stuff: just to make sure we are good */
    if((ret % (1 << order)) != 0) {
        free_pages(ret, order);
        ret = 0;
    }

    return (void*) ret;
}

/**
 *  \brief PAL os Memory Free API. This API can only free memory
 *  allocated using PAL_osMemAllocSizeAligned.
 */
PAL_INLINE void PAL_osMemFreeSizeAligned(Uint32 segId, void* addr, Uint32 numBytes)
{
    Uint32 order;

    /* find number of pages */
    numBytes = (numBytes/PAGE_SIZE) + ((numBytes % PAGE_SIZE)?1:0);

    /* find allocation order */
    for(order = 0; (1 << order) < numBytes; order++);

    free_pages((unsigned long)addr, order);
}

#endif

