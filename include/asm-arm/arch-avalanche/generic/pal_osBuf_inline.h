/*
 *
 * pal_osBuf_inline.h
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


/** \file   pal_osBuf_inline.h
    \brief  OsBUF Services Header File

    This file defines OS abstraction services for fixed length buffer
    memory management operations. The PAL module implementing these APIs
    is called OsBUF.

    In this implementaion of PAL OS for Linux, the fixed size buffer pool
    is implemented using the linux slab allocator. The slab allocator 
    allows creation of "slab caches" of fixed size. There is however no
    restriction on the number of buffers that can be allocated from a 
    particular cache. The slab allocator will attempt to fulfill a request
    for a buffer till the time the system has memory available.

 
    \author     PSP Architecture Team
    \version    1.0
 */

#ifndef __PAL_OSBUF_INLINE_H__
#define __PAL_OSBUF_INLINE_H__

#include "pal_os.h"
#include "pal_defs.h"
#include "pal_osCfg.h"
#include <asm/page.h>
#include <linux/slab.h>
#include <linux/mm.h>

typedef struct kmem_cache kmem_cache_t;

/**
 * \defgroup PalOSBuf PAL OS Buffer Interface
 * 
 * PAL OS Buffer Interface
 * @{
 */

/**
 * \brief   PAL OS Buffer Init.
 * Linux implementation leaves this as an empty function. The slab allocator
 * is initialized by the kernel itself. Nothing to do here.
 */
PAL_INLINE PAL_Result PAL_osBufInit(Ptr param)
{
   return PAL_SOK;
}


/**
 * \brief   PAL OS Buffer Pool Define
 * 
 * Only segment Id 0 is recognized. Please refer to documentaion in OsMEM 
 * module.
 * The parameter maxBufs (maximum number of buffers that can be allocated) is 
 * not honored. The allocate function will keep returning memory till it is
 * available. This constraint is because of the way slab allocator is
 * implemented in linux.
 * Alignment to Hardware cache line is only supported. No other alignment
 * is guaranteed. If the user asks for any alignment 
 * (by passing a non-NULL attrs) hardware cache alignment is assumed.
 */
PAL_INLINE PAL_Result PAL_osBufPoolDefine(   const char* name,
                                         Uint32 segId,
                                         Uint maxBufs,
                                         Uint32 bytesPerBuf,
                                         PAL_OsBufAttrs *attrs,
                                         Uint32 *pBufPoolId)
{
    Uint flags = 0;
        
    if(segId != 0)
	{
        return PAL_OS_ERROR_INVALID_PARAM;
	}

    /* if user asks for some alignment, give him Hardware cache alignmen.
     * This is the only alignment supported by Linux kernel 
     */
    if(attrs)
	{
        flags = SLAB_HWCACHE_ALIGN;
	}
		
    *pBufPoolId = (Uint32) kmem_cache_create(name, bytesPerBuf, 0, flags, 
                                             NULL);
            
    if(!(*pBufPoolId)) 
	{
        return PAL_OS_ERROR_NO_RESOURCES;
	}
		
    return PAL_SOK;
}

/**
 * \brief   PAL OS Buffer Pool Undefine
 */
PAL_INLINE PAL_Result PAL_osBufPoolUndefine (Uint32 bufPoolId)
{
    if(!bufPoolId)
	{
		return PAL_OS_ERROR_INVALID_PARAM;
	}
	
   kmem_cache_destroy((kmem_cache_t*)bufPoolId);	
   return PAL_SOK;
}

/**
 * \brief   PAL OS Buffer Alloc
 */
PAL_INLINE PAL_Result PAL_osBufAlloc(Uint32 bufPoolId, Ptr *bufAddr)
{
    *bufAddr = kmem_cache_alloc((kmem_cache_t*)bufPoolId, GFP_KERNEL);

    if(*bufAddr == NULL)
	{
        return PAL_OS_ERROR_NO_RESOURCES;
	}

    return PAL_SOK;
}

/**
 * \brief   PAL OS Buffer Free
 */
PAL_INLINE PAL_Result PAL_osBufFree(Uint32 bufPoolId, Ptr bufAddr)
{
    kmem_cache_free((kmem_cache_t*)bufPoolId, bufAddr);
   
    return PAL_SOK;
}


/**
 * \brief   PAL OS Buffer Report
 */
PAL_INLINE PAL_Result PAL_osBufReport(Uint32 bufPoolId, PAL_OsBufReport *report, Char* buf)
{
   return PAL_OS_ERROR_NOT_SUPPORTED;
}
/*@}*/
/*@}*/
#endif

