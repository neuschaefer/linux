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
 * $RCSfile: appinit.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file appinit.c
 *  BSP functions for initialization.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_linux.h"
#include "board.h"
#include "hal.h"
#include "x_assert.h"
#include "x_start_mini.h"
#include "x_mid.h"

#include <asm/mach/map.h>
#include <asm/cacheflush.h>
#include <linux/version.h>
#ifdef LINUX_TURNKEY_SOLUTION
#include <linux/module.h>
#endif

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define MAX_HANDLE_NUM                  2048


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

UINT32 _ui4ResVirtStart;
UINT32 _ui4ResVirtEnd;
UINT32 _u4FbmPhyStart;         // FBM Physical Start address
UINT32 _u4FbmPhyEnd;           // FBM Physical End addr
UINT32 _u4MaxLowPhy;           // Max Direct mapped addr

extern VOID x_sys_start(VOID* pv_mem_addr, SIZE_T z_mem_size, VOID* pv_arg);


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

void* BSP_AllocateReservedAlign(UINT32 u4Size, UINT32 u4Align)
{
    UINT32 u4AlignSize, u4Start, u4AlignStart;

    u4AlignSize = (u4Size % u4Align) == 0 ?
                  u4Size :
                  ((u4Size / u4Align) + 1) * u4Align;

    u4Start = (UINT32)FBM_START_PHY_ADDR - u4AlignSize;

    u4AlignStart = (u4Start % u4Align) == 0 ?
                   u4Start :
                   ((u4Start / u4Align) + 1) * u4Align;

    printk(KERN_DEBUG "BSP_AllocateReservedAlign :\n"
           "u4Size=%d, u4Align=%d, u4AlignSize=%d,\n"
           "u4Start=0x%x, u4AlignStart=0x%x\n",
           (int)u4Size, (int)u4Align, (int)u4AlignSize,
           (unsigned int)u4Start, (unsigned int)u4AlignStart);

    return (void *)u4AlignStart;
}

//-----------------------------------------------------------------------------
/** Map a physical memory of reserved region to virtual address.
 *  @param  pv_Phys     Physical memory to map.
 *  @param  z_Size      Size of the physical memory (bytes).
 *  @return If success, the mapped virtual address. \n
 *          Otherwise, NULL.
 */
//-----------------------------------------------------------------------------
void* BSP_MapReservedMem(void *pv_Phys, UINT32 z_Size)
{
    void *pvVirt;
    unsigned long start, size;
    extern void mt53xx_get_reserved_area(unsigned long *start, unsigned long *size);

    // Check reserved area.
    mt53xx_get_reserved_area(&start, &size);
    if (start && (unsigned long)pv_Phys >= start && ((unsigned long)pv_Phys + z_Size) <= (start + size))
    {
        // Great!! It is in reserved area and kernel already have a mapping,
        // just return va.
        return phys_to_virt((unsigned long)pv_Phys);
    }

#ifdef EXTRA_VMALLOC_START
    if (z_Size >= 0x1000000)
    {
        static UINT32 vmalloc_use = EXTRA_VMALLOC_START;

        if (z_Size <= EXTRA_VMALLC_END - vmalloc_use)
        {
            unsigned long va, pa, i;
            int err;
            const struct mem_type *mtype;

            mtype = get_mem_type(MT_DEVICE_CACHED);
            if (!mtype)
                goto err_out;

            pa = (unsigned long)pv_Phys & (~(PAGE_SIZE-1));
            pvVirt = (void*)(vmalloc_use + ((unsigned long)pv_Phys & (PAGE_SIZE-1)));
            va = vmalloc_use;
            vmalloc_use += (z_Size + PAGE_SIZE*2 - 1) & (PAGE_MASK);
            for (i=0; i<z_Size; i+= PAGE_SIZE)
            {
                err = ioremap_page(va, pa, mtype);
                if (err)
                    goto err_out;

                va += PAGE_SIZE;
                pa += PAGE_SIZE;
            }

            return pvVirt;
        }
    }
    err_out:
#endif /* EXTRA_VMALLOC_START */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
    pvVirt = ioremap_cached((int)pv_Phys, z_Size);
#else
    pvVirt = __ioremap((int)pv_Phys, z_Size, L_PTE_BUFFERABLE | L_PTE_CACHEABLE);
#endif
    
    ASSERT(pvVirt != NULL);

    return pvVirt;
}

static void GetResVirtStart(void)
{
    //_u4FbmPhyStart = (num_physpages >> (20 - PAGE_SHIFT)) << 20;
    //_u4FbmPhyEnd = TOTAL_DRAM_SIZE + FBM_CHB_SIZE;
    _u4MaxLowPhy = _u4FbmPhyStart = TOTAL_DRAM_SIZE - FBM_SIZE;
    _u4FbmPhyEnd = TOTAL_DRAM_SIZE;
    _ui4ResVirtStart = (UINT32) BSP_MapReservedMem((void*) _u4FbmPhyStart, _u4FbmPhyEnd - _u4FbmPhyStart);
    _ui4ResVirtEnd = _ui4ResVirtStart + (_u4FbmPhyEnd - _u4FbmPhyStart);
#ifdef CONFIG_HIGHMEM
    _u4MaxLowPhy = __pa(high_memory-1);
#endif
    printk("Mapped physical address 0x%x - 0x%x to virtual begins 0x%x\n", 
           (unsigned int)_u4FbmPhyStart, (unsigned int)_u4FbmPhyEnd, (unsigned int)_ui4ResVirtStart);
}

// remap usage only
UINT32 PHYSICAL(UINT32 addr)
{
    ASSERT(_ui4ResVirtStart != 0);
    return __PHYSICAL(addr);
}

UINT32 VIRTUAL(UINT32 addr)
{
    ASSERT(_ui4ResVirtStart != 0);
    return __VIRTUAL(addr);

}
#ifdef LINUX_TURNKEY_SOLUTION
EXPORT_SYMBOL(VIRTUAL);
#endif

extern void HalInitBenchmark(void);
extern BOOL HAL_InitTimer(void);
extern void BSP_InitBoard(void);

VOID __init BspMainTask(void)
{
#ifdef LINUX_MID_PROTECT_AGENT
    UINT32 _u4MidRegion, _u4TopAddr;
#endif

    GetResVirtStart();
    HalInitBenchmark();
    VERIFY(HAL_InitTimer());

    // Initialize OSAI and handle libraries
    VERIFY(x_start_mini(NULL, MAX_HANDLE_NUM, NULL, 0) == INITR_OK);
#ifndef STANDALONE_CLI
    // Init cli if diag is builtin with driver.
    x_sys_start(NULL, 0, NULL);
#endif
    BSP_InitBoard();

#ifdef LINUX_MID_PROTECT_AGENT
    // Init MID protect for all kernel DRAM for said agent.
    // LINUX_MID_PROTECT_AGENT is set in IC board.h, ex. mt5396/5396_driver/bsp/board.h
    if (!MID_Init())
    {
        return;
    }

    // Allocate a read-write region
    if (!MID_AllocateFreeRegion(FALSE, &_u4MidRegion))
    {
        return;
    }

    // Set all kernel to read-write
    _u4TopAddr = _u4FbmPhyStart - DRVCUST_OptGet(eDirectFBMemSize) - DRVCUST_OptGet(eFBMemSize);
    if (!MID_SetRegionEx2(_u4MidRegion, MID_AGENT_CPU, 0, _u4TopAddr,
               MID_FLAG_CPUACCESS, LINUX_MID_PROTECT_AGENT))
    {
        return;
    }
    
    if (!MID_EnableRegionProtect(_u4MidRegion))
    {
        return;
    }
    printk("Enable MID on %08x to %08x\n", 0, _u4TopAddr);
#endif /* LINUX_MID_PROTECT_AGENT */
}


//-----------------------------------------------------------------------------
/** Dump system task state.
 */
//-----------------------------------------------------------------------------
extern void dump_system_info(void);
void BspDumpSystemTasksState(void)
{
    dump_system_info();
}
