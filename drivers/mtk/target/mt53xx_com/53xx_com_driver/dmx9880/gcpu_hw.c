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
 * $RCSfile: dmx_api.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file gcpu_hw.c
 *  gcpu driver - hw access
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "gcpu.h"
#include "gcpu_dram.h"
#include "x_bim.h"
#include "x_hal_926.h"

#if defined(CC_MT5396)
#include "gcpu_iram0_96.h"
#elif defined(CC_MT5368)
#include "gcpu_iram0_68.h"
#elif defined(CC_MT5389)
#include "gcpu_iram0_89.h"
#else
#error "Please define a supported IC.\n"
#endif

#ifndef CC_MTK_LOADER
#ifdef __KERNEL__
#include <linux/semaphore.h>
#endif
#endif
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER
static HANDLE_T _hGcpuHwSema = NULL_HANDLE;
static HANDLE_T _hGcpuIrqSema = NULL_HANDLE;
#endif
static BOOL fgPageFault = FALSE;

//-----------------------------------------------------------------------------
// Extern variables
//-----------------------------------------------------------------------------
#ifdef __KERNEL__
extern struct semaphore aes_api;
extern struct semaphore aes_sema;
extern BOOL fgNativeAESISR;
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static void GCPU_HW_LOCK(void)
{
#ifndef CC_MTK_LOADER
    #ifdef __KERNEL__
    if(fgNativeAESISR)
    {
        down(&aes_api);
    }
    #endif
    VERIFY(x_sema_lock(_hGcpuHwSema, X_SEMA_OPTION_WAIT) == OSR_OK);
#endif
}


static void GCPU_HW_UNLOCK(void)
{
#ifndef CC_MTK_LOADER
    #ifdef __KERNEL__
    if(fgNativeAESISR)
    {
        up(&aes_api);
    }
    #endif
    VERIFY(x_sema_unlock(_hGcpuHwSema) == OSR_OK);
#endif
}

static void GCPU_IRQ_LOCK(void)
{
#ifndef CC_MTK_LOADER
    #ifdef __KERNEL__
    if(!fgNativeAESISR)
    {
        VERIFY(x_sema_lock(_hGcpuIrqSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    }
    else
    {
        down(&aes_sema);
    }
    #else
    VERIFY(x_sema_lock(_hGcpuIrqSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    #endif
#else
    while((GCPUCMD_READ32(GCPU_REG_INT_SET) & 0x1) == 0x0);
    GCPUCMD_WRITE32(GCPU_REG_INT_CLR, GCPU_INT_MASK);
    GCPUCMD_WRITE32(GCPU_REG_INT_EN, 0);
#endif
}

#ifndef CC_MTK_LOADER
static void GCPU_IRQ_UNLOCK(void)
{
    VERIFY(x_sema_unlock(_hGcpuIrqSema) == OSR_OK);
}
#endif

//-----------------------------------------------------------------------------
/** _GcpuHWReset
 *  Reset GCPU HW
 */
//-----------------------------------------------------------------------------
static INT32 _Gcpu_Hw_Reset(void)
{
    UINT32 u4Value = 0;

    u4Value = GCPUCMD_READ32(GCPU_REG_CTL);
    u4Value &= ~(0x7);
    GCPUCMD_WRITE32(GCPU_REG_CTL, u4Value);
    u4Value &= ~(0x8);
    GCPUCMD_WRITE32(GCPU_REG_CTL, u4Value);

    u4Value |= 0xF;
    GCPUCMD_WRITE32(GCPU_REG_CTL, u4Value);

    return S_GCPU_OK;
}


static void _Gcpu_InitRamCode(void)
{
    UINT32 i, u4Len = sizeof(GCPU_IRAM0_CODE) / 4;
    UINT32 *pu4RamCode = (UINT32*) GCPU_IRAM0_CODE;

    //Register Mode
    GCPUCMD_WRITE32(GCPU_REG_MEM_ADDR, 0x80002000);
    for (i = 0; i < u4Len; i++)
    {
        GCPUCMD_WRITE32(GCPU_REG_MEM_DATA, *(pu4RamCode + i));
    }
}


static void _Gcpu_InitDramCode(void)
{
#ifndef CC_MTK_LOADER
    static BYTE *pbAddr = 0;
    static UINT32 u4PhyAddr = 0;

    if(pbAddr == 0)
    {
        u4PhyAddr = BSP_AllocAlignedDmaMemory(GCPU_DRAM_LEN, GCPU_FIFO_ALIGNMENT);
        pbAddr = (BYTE *)VIRTUAL(u4PhyAddr);
    }
    x_memcpy((void *)pbAddr, (void *)GCPU_DRAM_CODE, GCPU_DRAM_LEN);
    HalFlushInvalidateDCacheMultipleLine((UINT32)pbAddr, GCPU_DRAM_LEN);
    GCPUCMD_WRITE32(GCPU_REG_MSC, GCPUCMD_READ32(GCPU_REG_MSC) | (1 << 12));
    GCPUCMD_WRITE32(GCPU_REG_DRAM_INST_BASE, u4PhyAddr);
#else
    UNUSED(GCPU_DRAM_CODE);
#endif
}

static void _Gcpu_SetupTrapping(void)
{
    UINT32 i, u4Len = sizeof(GCPU_IRAM0_TRAPPING) / 4;
    UINT32 *pu4Trapping = (UINT32*) GCPU_IRAM0_TRAPPING;

    //Register Mode
    for (i = 0; i < u4Len; i++)
    {
        GCPUCMD_WRITE32((GCPU_REG_TRAP_START + i), *(pu4Trapping + i));
    }
}

//-----------------------------------------------------------------------------
/** _GCPU_IrqHandle()
 *  @param u2Vector: The IRQ vector, must be VECTOR_UP0
 */
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER
static VOID _Gcpu_IrqHandle(UINT16 u2Vector)
{
    ASSERT(u2Vector == VECTOR_UP0);
    if (!BIM_IsIrqPending(VECTOR_UP0))
    {
        LOG(3, "Spurious gcpu global interrupt!\n");
        return;
    }

#if !defined(__linux__)
    // Check if it's aes interrupt
    while (BIM_IsIrqPending(VECTOR_UP0))
#endif
    {
        // Clear interrupt
        GCPUCMD_WRITE32(GCPU_REG_INT_CLR, GCPU_INT_MASK);
        GCPUCMD_WRITE32(GCPU_REG_INT_EN, 0);
        VERIFY(BIM_ClearIrq(VECTOR_UP0));

        GCPU_IRQ_UNLOCK();
    }
}
#endif

#ifndef CC_MTK_LOADER
static void _Gcpu_IOMMU_IrqHandle(UINT16 u2Vector)
{
    ASSERT(u2Vector == VECTOR_MMU_GCPU);
    if (!BIM_IsIrqPending(VECTOR_MMU_GCPU))
    {
        LOG(3, "Spurious gcpu iommu global interrupt!\n");
        return;
    }

#if !defined(__linux__)
    // Check if it's aes interrupt
    while (BIM_IsIrqPending(VECTOR_MMU_GCPU))
#endif
    {
        fgPageFault = TRUE;
        _Gcpu_Hw_Reset();
        IOMMU_GCPU_WRITE32(REG_IOMMU_CFG4, IOMMU_GCPU_READ32(REG_IOMMU_CFG4) | 0x06000005);
        IOMMU_GCPU_WRITE32(REG_IOMMU_CFG4, IOMMU_GCPU_READ32(REG_IOMMU_CFG4) & ~(0x0610000F));
        VERIFY(BIM_ClearIrq(u2Vector));

        GCPU_IRQ_UNLOCK();
    }
}
#endif

//-----------------------------------------------------------------------------
/** _GCPU_InitIsr
*/
//-----------------------------------------------------------------------------
static void _Gcpu_InitIsr(void)
{
#ifndef CC_MTK_LOADER
    static BOOL _fgInit = FALSE;
    x_os_isr_fct pfnOldIsr;

    if(!_fgInit)
    {
        #ifdef __KERNEL__
        if(!fgNativeAESISR) // only initial ISR when native AES did not do it
        #endif
        {
            VERIFY(OSR_OK == x_sema_create(&_hGcpuIrqSema,
                X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK));

            // GCPU interrupt
            GCPUCMD_WRITE32(GCPU_REG_INT_CLR, GCPU_INT_MASK);
            GCPUCMD_WRITE32(GCPU_REG_INT_EN, 0);

            BIM_ClearIrq(VECTOR_UP0);
            if (x_reg_isr(VECTOR_UP0, _Gcpu_IrqHandle, &pfnOldIsr) != OSR_OK)
            {
                LOG(0, "Error: fail to register GCPU ISR!\n");
            }

            // GCPU IOMMU interrupt
            BIM_ClearIrq(VECTOR_MMU_GCPU);
            if (x_reg_isr(VECTOR_MMU_GCPU, _Gcpu_IOMMU_IrqHandle, &pfnOldIsr) != OSR_OK)
            {
                LOG(0, "Error: fail to register GCPU IOMMU ISR!\n");
            }
        }

        UNUSED(pfnOldIsr);

        _fgInit = TRUE;
    }
    else
    {
        #ifdef __KERNEL__
        if(!fgNativeAESISR) // only initial ISR when native AES did not do it
        #endif
        {
            INT32 i4Ret;
            // Make sure the semaphore is reset to the locked state.
            i4Ret = x_sema_lock(_hGcpuIrqSema, X_SEMA_OPTION_NOWAIT);
            if (i4Ret == OSR_OK)
            {
                LOG(6, "Reset GCPU semaphore to the Locked state!\n");
            }
            else if (i4Ret == OSR_WOULD_BLOCK)
            {
                LOG(6, "GCPU semaphore is in the Locked state!\n");
            }
            else
            {
                LOG(6, "%d: Semaphore API failed!\n", __LINE__);
            }
        }
    }
#endif
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
INT32 _GCPU_EnableIOMMU(UINT32 u4Src1Start, UINT32 u4Src1End, UINT32 u4Src2Start, UINT32 u4Src2End)
{
#ifndef CC_MTK_LOADER
    // setup IOMMU
    IOMMU_GCPU_WRITE32(REG_IOMMU_CFG4, 0x0603000A);
    IOMMU_GCPU_WRITE32(REG_IOMMU_CFG4, 0x0010000A);
    IOMMU_GCPU_WRITE32(REG_IOMMU_CFG0, 0xFE | (0x01 << 0));
    IOMMU_GCPU_WRITE32(REG_IOMMU_CFG1, HalGetMMUTableAddress());
    IOMMU_GCPU_WRITE32(REG_IOMMU_CFG2, 0x00130011);

    IOMMU_GCPU_WRITE32(REG_IOMMU_CFG9, (u4Src1Start & 0xfffff000) | 0x3);
    IOMMU_GCPU_WRITE32(REG_IOMMU_CFGA, (u4Src1End - 1) & 0xfffff000);
    if(u4Src2Start == 0 || u4Src2End == 0)
    {
        IOMMU_GCPU_WRITE32(REG_IOMMU_CFGB, (u4Src1Start & 0xfffff000) | 0x1);
        IOMMU_GCPU_WRITE32(REG_IOMMU_CFGC, (u4Src1End - 1) & 0xfffff000);
    }
    else
    {
        IOMMU_GCPU_WRITE32(REG_IOMMU_CFGB, (u4Src2Start & 0xfffff000) | 0x1);
        IOMMU_GCPU_WRITE32(REG_IOMMU_CFGC, (u4Src2End - 1) & 0xfffff000);
    }

    //fire IOMMU cache
    IOMMU_GCPU_WRITE32(REG_IOMMU_CFG4, IOMMU_GCPU_READ32(REG_IOMMU_CFG4) | (1U << 31));
    while((IOMMU_GCPU_READ32(REG_IOMMU_CFG8) & (1 << 29)) != 0);
#endif
    return S_GCPU_OK;
}

INT32 _GCPU_GetRomCodeChecksum(void)
{
    UINT32 i, res, tmp, d0, d1, d2, mask;

    GCPU_HW_LOCK();

    _Gcpu_Hw_Reset();

    _GCPU_Lock();

    mask = 255; //0xff
    res = 0;

    GCPUCMD_WRITE32(GCPU_REG_MEM_ADDR, 0x00000000);

    for (i = 1; i <= GCPU_IMEM_ROM_SIZE; i++)
    {
        tmp = GCPUCMD_READ32(GCPU_REG_MEM_DATA);
        //Printf("%04d:%08x\n", i, tmp);

        d0 = tmp & mask;
        d1 = (tmp >> 8) & mask;
        d2 = (tmp >> 16) & mask;
        if((i % 4) == 0)
        {
            tmp = (d2 << 16) + (d1 << 8) + d0;
        }
        if((i % 4) == 1)
        {
            tmp = (d1 << 16) + (d0 << 8) + d2;
        }
        if((i % 4) == 2)
        {
            tmp = (d0 << 16) + (d2 << 8) + d1;
        }
        if((i % 4) == 3)
        {
            tmp = (d0 << 16) + (d1 << 8) + d2;
        }

        res = res ^ tmp;
    }

    _GCPU_Unlock();

    LOG(5, "%x\n", res);

    GCPU_HW_UNLOCK();

    return res;
}


//-----------------------------------------------------------------------------
/** _GCPU_Hw_Init
 *  Init GCPU HW
 */
//-----------------------------------------------------------------------------
INT32 _GCPU_Hw_Init(UINT32 u4Mode)
{
    UINT32 u4Value = 0;

    #ifndef CC_MTK_LOADER
    // create a semaphore
    if (0 == _hGcpuHwSema)
    {
        if(x_sema_create(&_hGcpuHwSema, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) != OSR_OK)
        {
            LOG(1, "Fail to create HW semaphore\n");
            return E_GCPU_OS_OPERA_FAIL;
        }
    }
    #endif

    GCPU_HW_LOCK();

    CKGEN_WRITE32(0x278, CKGEN_READ32(0x278) | 0x20000);
    CKGEN_WRITE32(0x318, 0x1);  // GCPU Max clock: 240MHz

    _Gcpu_Hw_Reset();

    // Enable GCPU
    u4Value = GCPUCMD_READ32(GCPU_REG_MSC);
    GCPUCMD_WRITE32(GCPU_REG_MSC, u4Value | (1 << 6) | (1 << 11) | (1 << 17));

    if(u4Mode == 0)
    {
        //Init Ram Code
        _Gcpu_InitRamCode();
    }
    else
    {
        //Init Dram Code
        _Gcpu_InitDramCode();
    }

    _Gcpu_SetupTrapping();

    _Gcpu_InitIsr();

    GCPU_HW_UNLOCK();

    return S_GCPU_OK;
}


INT32 _GCPU_Hw_SetSlot(UINT8 u1Idx, UINT32 data)
{
    GCPU_HW_LOCK();

    GCPUCMD_WRITE32(GCPU_REG_MEM_CMD + u1Idx, data);

    GCPU_HW_UNLOCK();

    return S_GCPU_OK;
}


INT32 _GCPU_Hw_CmdExe(GCPU_HW_CMD_T *prHwCmd)
{
    UINT32 i;

    GCPU_HW_LOCK();

    // Setup Parameter
    for(i = 1; i < GCPU_PARAM_NUM; i++)
    {
        GCPUCMD_WRITE32(GCPU_REG_MEM_CMD + i, prHwCmd->u4Param[i]);
    }

    // Clear/Enable GCPU Interrupt
    GCPUCMD_WRITE32(GCPU_REG_INT_CLR, GCPU_INT_MASK);
    GCPUCMD_WRITE32(GCPU_REG_INT_EN, GCPU_INT_MASK);

    // GCPU Decryption Mode
    GCPUCMD_WRITE32(GCPU_REG_MEM_CMD, prHwCmd->u4Param[0]);

    // GCPU PC
    GCPUCMD_WRITE32(GCPU_REG_PC_CTL, 0x0);

    GCPU_IRQ_LOCK();

    if(fgPageFault == TRUE)
    {
        LOG(1, "GCPU IOMMU has page fault!\n");
        fgPageFault = FALSE;
        GCPU_HW_UNLOCK();
        return E_GCPU_PAGE_FAULT;
    }

    // read back param 0 - 31
    for(i = 0; i < 32; i++)
    {
        prHwCmd->u4Param[i] = GCPUCMD_READ32(GCPU_REG_MEM_CMD + i);
    }

    // read back param 32 - 47
    for(i = GCPU_PARAM_RET_PTR; i < GCPU_PARAM_RET_PTR + GCPU_PARAM_RET_NUM; i++)
    {
        prHwCmd->u4Param[i] = GCPUCMD_READ32(GCPU_REG_MEM_CMD + i);
    }

    GCPU_HW_UNLOCK();

    return S_GCPU_OK;
}

