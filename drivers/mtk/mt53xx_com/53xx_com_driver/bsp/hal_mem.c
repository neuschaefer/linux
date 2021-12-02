/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile:  $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file hal_mem.c
 *  Hardware abstraction rountines - mem alloc related functions.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "hal.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_hal_arm.h"
#include "x_hal_5381.h"
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/dma-mapping.h>
#include "x_linux.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define ALLOC_BY_KMALLOC            0x12
#define ALLOC_BY_GETPAGE            0x87
#define ALLOC_BY_PAGES_EXACT        0x39
#define ALLOC_BY_VMALLOC            0xCD
#define ALLOC_BY_DMA_COHERENT       0x2D

#define ALLOC_MAX_SIZE                   (0xffffff << 3)
#define ALLOC_MINALIGN                   32
#define ALLOC_SIZE_MASK                  7
#define ALLOC_TAG_SET_SIZE(type, size)   (((type) << 24) | ((size)>>3))
#define ALLOC_TAG_SET_ORDER(type, order) (((type) << 24) | (order))
#define ALLOC_TAG_GET_TYPE(tag)          ((tag) >> 24)
#define ALLOC_TAG_GET_SIZE(tag)          (((tag) & 0xffffff) << 3)
#define ALLOC_TAG_GET_ORDER(tag)         (((tag) & 0xffffff))

#ifndef NDEBUG
#define DEBUG_MEM_ALLOC_WARNING_THRESHOLD   (512*1024-64)
#endif

//-----------------------------------------------------------------------------
// Internal Debug Functions.
//-----------------------------------------------------------------------------

#ifdef DEBUG_MEM_ALLOC_WARNING_THRESHOLD
#define x_mem_alloc_size_check(size)                 \
    if (size >= DEBUG_MEM_ALLOC_WARNING_THRESHOLD)   \
    {                                                \
        printk("DEBUG INFO: %s request for %d bytes, allocation callstack:\n", __FUNCTION__, size); \
        dump_stack();                                \
    }
#else
#define x_mem_alloc_size_check(size)
#endif

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

#define MAX_MEM_ALLOC_RETRY 20 
#define MIN_MEM_ALLOC_VIRTUAL   (64*1024)
#define MEM_ALLOC_PAGES_EXACTS_THRESHOLD (16*1024)
static void* x_mem_alloc_generic (SIZE_T z_size, gfp_t flags)
{
    void *pvPtr = NULL, *pvRet;
    UINT32 ui4_order, tag;
    gfp_t gfp_flags=flags | __GFP_NOWARN;
    UINT8 retrycnt=MAX_MEM_ALLOC_RETRY;
    int use_exacts = 0;

    x_mem_alloc_size_check(z_size);

    // Always allocate 1 cache line more for tag.
    // We already waste it, just make return address align.
    z_size += ALLOC_MINALIGN;
    z_size = (z_size + ALLOC_SIZE_MASK) & (~ALLOC_SIZE_MASK);
    if (z_size > ALLOC_MAX_SIZE)
    {
        ASSERT(0 && "Requested x_mem_alloc size larger then ALLOC_MAX_SIZE");
        return 0;
    }
    
    // Use alloc_pages_exact if we could save >= 16KB.
    if (z_size > MEM_ALLOC_PAGES_EXACTS_THRESHOLD*2)
    {
        ui4_order = get_order(z_size);
        use_exacts = (((1 << ui4_order) - z_size) >= MEM_ALLOC_PAGES_EXACTS_THRESHOLD);
    }

retry: 
    if (use_exacts)
        pvPtr = alloc_pages_exact(z_size, gfp_flags);
    else
        pvPtr = kmalloc(z_size, gfp_flags);

    if (pvPtr == NULL)
    {
        ui4_order = get_order(z_size);
        pvPtr = (void *)__get_free_pages(gfp_flags, ui4_order);
        if (pvPtr == NULL)
        {
		if(retrycnt--)
		{
			if(!retrycnt) //final-try
				gfp_flags=flags;
			goto retry;
		}
		else
		{
			return NULL;
		}
        }
        tag = ALLOC_TAG_SET_ORDER(ALLOC_BY_GETPAGE, ui4_order);
    }
    else
    {
        tag = ALLOC_TAG_SET_SIZE((use_exacts) ? ALLOC_BY_PAGES_EXACT : ALLOC_BY_KMALLOC, z_size);
    }

    pvRet = (void *)((UINT32)pvPtr + ALLOC_MINALIGN);
    *((UINT32*)pvRet-1) = tag;
    *((UINT32*)pvRet-2) = (UINT32)pvPtr;
    return pvRet;
}

VOID* x_mem_alloc (SIZE_T z_size)
{
    return x_mem_alloc_generic(z_size, GFP_KERNEL|__GFP_REPEAT);
}

VOID* x_mem_alloc_virtual (SIZE_T z_size)
{
    void *pvPtr = NULL, *pvRet;

    if (z_size < MIN_MEM_ALLOC_VIRTUAL - ALLOC_MINALIGN)
        return x_mem_alloc(z_size);

    x_mem_alloc_size_check(z_size);

    // Always allocate 1 cache line more for tag.
    // We already waste it, just make return address align.
    z_size += ALLOC_MINALIGN;
    z_size = (z_size + ALLOC_SIZE_MASK) & (~ALLOC_SIZE_MASK);
    if (z_size > ALLOC_MAX_SIZE)
    {
        ASSERT(0 && "Requested x_mem_alloc size larger then ALLOC_MAX_SIZE");
        return 0;
    }

    pvPtr = vmalloc(z_size);
    if (!pvPtr)
    {
        return NULL;
    }

    pvRet = (void *)((UINT32)pvPtr + ALLOC_MINALIGN);
    *((UINT32*)pvRet-1) = ALLOC_TAG_SET_SIZE(ALLOC_BY_VMALLOC, z_size);
    *((UINT32*)pvRet-2) = (UINT32)pvPtr;
    return pvRet;
}

static inline VOID* x_mem_dma_alloc (SIZE_T z_size)
{
    return x_mem_alloc_generic(z_size, GFP_KERNEL|GFP_DMA|__GFP_REPEAT);
}

VOID* x_mem_alloc_nocache (SIZE_T z_size, UINT32* hwaddr)
{
    void *pvPtr, *pvRet;

    x_mem_alloc_size_check(z_size);

    z_size += ALLOC_MINALIGN;    // for tag & handle
    pvPtr = dma_alloc_coherent(NULL, z_size, (dma_addr_t*)hwaddr, GFP_KERNEL|GFP_DMA|__GFP_REPEAT);
    if (!pvPtr)
        return 0;

    pvRet = (void *)((UINT32)pvPtr + ALLOC_MINALIGN);
    *((UINT32*)pvRet-1) = ALLOC_TAG_SET_SIZE(ALLOC_BY_DMA_COHERENT, z_size);
    *((UINT32*)pvRet-2) = *hwaddr;
    *hwaddr += ALLOC_MINALIGN;
    return pvRet;
}

VOID x_mem_free(VOID* pv_mem_block)
{
    UINT32 *pui4_real = ((UINT32 *)pv_mem_block) - 1;
    UINT32 type, order;
    void *addr;

    if (pv_mem_block == NULL)
    {
        return;
    }

    type = ALLOC_TAG_GET_TYPE(*pui4_real);
    addr = *((void**)pv_mem_block-2);
    if (type == ALLOC_BY_KMALLOC)
    {
        kfree(addr);
    }
    else if (type == ALLOC_BY_VMALLOC)
    {
        vfree(addr);
    }
    else if (type == ALLOC_BY_GETPAGE)
    {
        order = ALLOC_TAG_GET_ORDER(*pui4_real);
        free_pages((UINT32)addr, order);
    }
    else if (type == ALLOC_BY_PAGES_EXACT)
    {
        free_pages_exact(addr, ALLOC_TAG_GET_SIZE(*pui4_real));
    }
    else if (type == ALLOC_BY_DMA_COHERENT)
    {
        dma_free_coherent(NULL, ALLOC_TAG_GET_SIZE(*pui4_real), 
                          (void*)((UINT32)pv_mem_block-ALLOC_MINALIGN), (UINT32)addr);
    }
    else
    {
        ASSERT(0 && "Unknown Allocation");
    }
}

VOID* x_mem_realloc(VOID* pv_mem_block, SIZE_T z_new_size)
{
    UINT32 *pui4_real = ((UINT32 *)pv_mem_block) - 1;
    UINT32 type, size;
    VOID *pvNew;

    if (!pv_mem_block)
        return x_mem_alloc(z_new_size);

    if (!z_new_size)
    {
        x_mem_free(pv_mem_block);
        return 0;
    }

    type = ALLOC_TAG_GET_TYPE(*pui4_real);
    size = ALLOC_TAG_GET_SIZE(*pui4_real);
    if (type == ALLOC_BY_GETPAGE)
    {
        size = ALLOC_TAG_GET_ORDER(*pui4_real);
        size = (1<<size) * PAGE_SIZE;
    }
    else if (type == ALLOC_BY_DMA_COHERENT)
    {
        ASSERT(0 && "Realloc for x_mem_alloc_nocache memory not supported.");
        return 0;
    }

    size-=ALLOC_MINALIGN;    // space for tag.
    if (size >= z_new_size)
        return pv_mem_block;

    pvNew = x_mem_alloc(z_new_size);
    if (pvNew)
    {
        memcpy(pvNew, pv_mem_block, size);
        x_mem_free(pv_mem_block);
    }
    return pvNew;
}

VOID* x_mem_calloc(UINT32 ui4_num_unit, SIZE_T z_unit_size)
{
    VOID *pv_mem = NULL;
    UINT32  *pui4_mem;
    SIZE_T z_size;

    if (ui4_num_unit == 0)
    {
        ui4_num_unit = 1;   /* allocate 1 unit of size z_unit_size */
    }

    z_size = ((ui4_num_unit * z_unit_size) + 3) & ~0x3;
    pv_mem = x_mem_alloc(z_size);

    if (pv_mem != NULL)
    {
        pui4_mem = (UINT32 *) pv_mem;
        memset(pui4_mem, 0, z_size);
    }

    return(pv_mem);
}

UINT32 BSP_AllocDmaMemory(UINT32 u4Size)
{
    return BSP_AllocAlignedDmaMemory(u4Size, 0);
}

BOOL BSP_FreeDmaMemory(UINT32 uUser)
{
    return BSP_FreeAlignedDmaMemory(uUser);
}

UINT32 BSP_AllocAlignedDmaMemory(UINT32 u4Size, UINT32 u4Align)
{
    UINT32 u4CacheLineSize, u4AllocSize;
    void* pMem;
    UINT32* p;

    u4CacheLineSize = HalGetDCacheLineSize();

    // Adjust alignment
    if (u4Align < u4CacheLineSize)
    {
        u4Align = u4CacheLineSize;
    }

    // Allocate 1 cache_line bytes more, to guarantee that the entire DMA
    // buffer will not share cache lines with others
    u4AllocSize = u4Size + u4CacheLineSize + u4Align + sizeof(void*)*2;

    pMem = x_mem_dma_alloc(u4AllocSize);
    if (pMem == NULL)
    {
        return 0;
    }

    p = (UINT32*)(((UINT32)pMem + sizeof(void*)*2 + u4Align - 1) & ~(u4Align - 1));
    *(p - 1) = (UINT32)pMem;
    *(UINT32*)pMem = (UINT32)p;

    return PHYSICAL((UINT32)p);
}

BOOL BSP_FreeAlignedDmaMemory(UINT32 uUserAddr)
{
    UINT32* p;
    void* pMem, *pUser;

    // Check if NULL
    if (uUserAddr == 0)
    {
        return TRUE;
    }

    pUser = (void*)VIRTUAL(uUserAddr);

    // Check if the memory is allocated from BSP_AllocDmaMemory()
    p = (UINT32*)pUser;
    pMem = (void*)*(p - 1);
    if (*(UINT32*)pMem != (UINT32)p)
    {
    	return FALSE;
    }

    x_mem_free(pMem);

    return TRUE;
}
