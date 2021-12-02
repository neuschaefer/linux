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
 * $RCSfile: lzhs.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file lzhs_dec.c
 *  Brief of file lzhs_dec.c.
 *  Details of file lzhs_dec.c (optional).
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_typedef.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_bim.h"
#include "x_hal_arm.h"
#include "x_timer.h"
#include "x_ckgen.h"
#include "x_serial.h"

#ifndef CC_MTK_LOADER
#include "x_os.h"
#endif /* CC_MTK_LOADER */

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
//#define CHANGE_CLK

#define LZHS_HW_ALIGNMENT   8
#define LZHS_TIMEOUT        5000

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
#ifdef CC_MTK_LOADER
typedef void (*PFN_IMAGE)(void);
#endif /* CC_MTK_LOADER */

//#define CHANGE_DECOMPRESS_CLK

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#if defined(CC_MTK_PRELOADER) && defined(NDEBUG)
#define Printf(fmt...)
#endif /* defined(CC_MTK_PRELOADER) && defined(NDEBUG) */

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER

static HANDLE_T _hLZHSSema;
static BOOL _fgIsrEn = FALSE;
static BOOL _fgLZHSRegIsr = FALSE;



//-----------------------------------------------------------------------------
/** _LZHS_Isr()
 */
//-----------------------------------------------------------------------------
static void _LZHS_Isr(UINT16 u2VctId)
{
    UINT32 u4Val;

    UNUSED(u2VctId);
    u4Val = BIM_READ32(REG_LZHS_CTRL);

    if (u4Val & 0x100)
    {
        VERIFY(x_sema_unlock(_hLZHSSema) == OSR_OK);
    }

    VERIFY(BIM_ClearIrq(VECTOR_LZHS));
}


//-----------------------------------------------------------------------------
/** _LZHS_Isr()
 */
//-----------------------------------------------------------------------------
static void _LZHS_SetISR()
{
    void (* pfnOldIsr)(UINT16);

    if(!_fgLZHSRegIsr)
    {
        VERIFY(BIM_ClearIrq(VECTOR_LZHS));
        VERIFY(x_sema_create(&_hLZHSSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY(x_reg_isr(VECTOR_LZHS, _LZHS_Isr, &pfnOldIsr) == OSR_OK);

        VERIFY(BIM_DisableIrq(VECTOR_LZHS));        
        _fgLZHSRegIsr = TRUE;
    }
}

//-----------------------------------------------------------------------------
/** Brief of LZHS_SetIsrEnable.
 *  Details of LZHS_SetIsrEnable
 */
//-----------------------------------------------------------------------------
BOOL LZHS_SetIsrEnable(BOOL fgSet)
{
    BOOL fgOrg;

    fgOrg = _fgIsrEn;
    _fgIsrEn = fgSet;

    if (fgSet)
    {
        _LZHS_SetISR();
        
        VERIFY(BIM_ClearIrq(VECTOR_LZHS));
        VERIFY(BIM_EnableIrq(VECTOR_LZHS));        
    }
    else
    {
        VERIFY(BIM_DisableIrq(VECTOR_LZHS));
        VERIFY(BIM_ClearIrq(VECTOR_LZHS));    
    }

    return fgOrg;
}

#endif /* CC_MTK_LOADER */

//-----------------------------------------------------------------------------
/** Brief of LZHS_Dec.
 *  Details of LZHS_Dec
 *  @retval 0   Success
 *  @retval 1   Fail
 */
//-----------------------------------------------------------------------------
INT32 LZHS_Dec(UINT32 u4SrcAddr, UINT32 u4DestAddr, UINT32 u4Len, UINT8 *pu1CheckSum)
{
#ifdef CC_MTK_LOADER
    UINT32 u4Reg, u4Addr, i;
    UINT8 u1OrgChecksum;
    PFN_IMAGE pfnImage;
#ifdef CHANGE_DECOMPRESS_CLK    
    UINT32 u4OrgClkReg;
#endif
    BOOL fgFlash;

    // disable LED blinker timer
    u4Addr = BIM_READ32(REG_RW_TIMER_CTRL);
    u4Addr &= ~((TMR1_CNTDWN_EN) | (TMR1_AUTOLD_EN));
    BIM_WRITE32(REG_RW_TIMER_CTRL, u4Addr);       

    Printf("LZHS hardware decode\n");

    u1OrgChecksum = *pu1CheckSum;

    //------------------------------------------------------------
    // Speed up flash
    //BIM_WRITE32(0x7B4, 1);  // Falling edge
    //BIM_WRITE32(0x760, 1);  // Fast read
    //BIM_WRITE32(0x7B0, 6);  // 40 MHz

#ifndef CC_MTK_PRELOADER
    BIM_SetTimeLog(2);
#ifndef CC_UBOOT
    //------------------------------------------------------------
    // Sync instruction and data caches
    HalInstructionMemoryBarrier();

    // Disable MMU and caches
    HalDisableCaches();
    HalDisableMMU();

    // Wait until com data flush.
    SerWaitTxBufClear();
    // Set to normal mode back.
    SerNormalMode();
    //------------------------------------------------------------
#endif /* CC_UBOOT */
#else /* CC_MTK_PRELOADER */
    /* Here is preloader. */
    BIM_SetTimeLog(0);
#endif /* CC_MTK_PRELOADER */

    if(((u4DestAddr % LZHS_HW_ALIGNMENT)!=0) || ((u4Len % LZHS_HW_ALIGNMENT)!=0))
    {
        Printf("LZHS decompress address and len must 16-bytes alignment!!\n");
        return 1;
    }

    //----------------------------------------
    // Change clock
#ifdef CHANGE_DECOMPRESS_CLK
    u4OrgClkReg = CKGEN_READ32(REG_CPU_CKCFG);
    u4Reg = u4OrgClkReg;
    u4Reg &= ~0xF;
    u4Reg |= 0xD;   // mem clk
    CKGEN_WRITE32(REG_CPU_CKCFG, u4Reg);
#endif

    //--------------------------------------
    // Enable LZHS and switch clock
    BIM_WRITE32(REG_LZHS_RAMCK, SRAM_CLK_SEL | LZHS_SRAM_EN);
    do {
        u4Reg = BIM_READ32(REG_LZHS_RAMCK);
    } while (u4Reg & LZHS_CHGOK);

    //--------------------------------------
    // Prepare to decode
    BIM_WRITE32(REG_LZHS_CTRL, 0x600);  // reset
    BIM_WRITE32(REG_LZHS_CTRL, 0x400);

    if(u4SrcAddr > PBI_A_BASE)
    {
        // src is flash
        fgFlash = TRUE;
        u4Addr = u4SrcAddr;
        
        BIM_WRITE32(REG_LZHS_CTRL, 0x1400);
    }
    else
    {
        // src is dram
        fgFlash = FALSE;
        u4Addr = u4SrcAddr >> 3;

        BIM_WRITE32(REG_LZHS_CTRL, 0x400);
    }

    BIM_WRITE32(REG_LZHS_R_ADDR, u4Addr);
    BIM_WRITE32(REG_LZHS_W_ADDR, u4DestAddr>>3);
    BIM_WRITE32(REG_LZHS_W_END_ADDR, (u4DestAddr + u4Len)>>3);

    // Start doecode
    if(fgFlash)
    {
        BIM_WRITE32(REG_LZHS_CTRL, 0x1C00);
    }
    else
    {        
        BIM_WRITE32(REG_LZHS_CTRL, 0xC00);
    }

    i = 0;
    while((BIM_READ32(REG_LZHS_CTRL)&0x100)==0x0)
    {
        i++;
        if (i > 0x40000000)
        {
            Printf("decode is not finished within 5 seconds\n");
            return 1;
        }
    } // XXX forever loop.

    //--------------------------------------
    // Disable LZHS and switch clock
    BIM_WRITE32(REG_LZHS_RAMCK, 0x0);
    do {
        u4Reg = BIM_READ32(REG_LZHS_RAMCK);
    } while (u4Reg & LZHS_CHGOK);

    if(((BIM_READ32(REG_LZHS_CTRL))&0xFF) != u1OrgChecksum)
    {
        Printf("decode checksum is not the same.\n");
        return 1;
        // while(1) { } // XXX forever loop
    }

    //--------------------------------------
    // Restore clock
#ifdef CHANGE_DECOMPRESS_CLK    
    CKGEN_WRITE32(REG_CPU_CKCFG, u4OrgClkReg);
#endif

#ifdef CC_MTK_PRELOADER
    /* Here is preloader */
    BIM_SetTimeLog(1);
#else /* CC_PRELOADER */
    /* Here is uboot loader. */
    BIM_SetTimeLog(3);
#endif /* CC_PRELOADER */

    // Disable IRQ only when decoding successfully
    BIM_WRITE32(REG_IRQEN, 0);
    BIM_WRITE32(REG_IRQCL, 0xffffffff);

    // Jump to start...
#ifdef CC_MTK_PRELOADER
    pfnImage = (PFN_IMAGE)u4DestAddr;
    pfnImage();
#else /* CC_MTK_PRELOADER */
    UNUSED(pfnImage);
#endif /* CC_MTK_PRELOADER */

#else /* CC_MTK_LOADER */
    UINT32 u4Reg, u4Addr;
    INT32 i4Ret;
    HAL_TIME_T time1, time2, timediff;
    BOOL fgFlash;
    UINT8 u1CheckSum;
#ifdef CHANGE_DECOMPRESS_CLK    
    UINT32 u4OrgClkReg;
#endif

    Printf("LZHS hardware decode\n");
    if(((u4DestAddr % LZHS_HW_ALIGNMENT)!=0) || ((u4Len % LZHS_HW_ALIGNMENT)!=0))
    {
        Printf("LZHS decompress address and len must 16-bytes alignment!!\n");
        return 1;
    }

    // Flush DCache
    HalFlushInvalidateDCache();

    //----------------------------------------
    // Change clock
#ifdef CHANGE_DECOMPRESS_CLK
    u4OrgClkReg = CKGEN_READ32(REG_CPU_CKCFG);
    u4Reg = u4OrgClkReg;
    u4Reg &= ~0xF;
    u4Reg |= 0xD;   // mem clk
    CKGEN_WRITE32(REG_CPU_CKCFG, u4Reg);
#endif
    

    //--------------------------------------
    // Enable LZHS and switch clock
    BIM_WRITE32(REG_LZHS_RAMCK, SRAM_CLK_SEL | LZHS_SRAM_EN);
    do {
        u4Reg = BIM_READ32(REG_LZHS_RAMCK);
    } while (u4Reg & LZHS_CHGOK);

    //--------------------------------------
    // Decode
    BIM_WRITE32(REG_LZHS_CTRL, 0x600);  // reset
    BIM_WRITE32(REG_LZHS_CTRL, 0x400);

    fgFlash = FALSE;
    if(u4SrcAddr > PBI_A_BASE)
    {
        // src is flash
        fgFlash = TRUE;     
        u4Addr = u4SrcAddr - PBI_A_BASE;
        BIM_WRITE32(REG_LZHS_CTRL, 0x1400); // Must before set address register
    }
    else
    {
        // src is dram
        fgFlash = FALSE;
        u4Addr = u4SrcAddr >> 3;
        BIM_WRITE32(REG_LZHS_CTRL, 0x400);  // Must before set address register
    }
        
    BIM_WRITE32(REG_LZHS_R_ADDR, u4Addr);
    BIM_WRITE32(REG_LZHS_W_ADDR, u4DestAddr>>3);
    BIM_WRITE32(REG_LZHS_W_END_ADDR, (u4DestAddr + u4Len)>>3);

    HAL_GetTime(&time1);

    if(fgFlash)
    {
        BIM_WRITE32(REG_LZHS_CTRL, 0x1C00);
    }
    else
    {        
        BIM_WRITE32(REG_LZHS_CTRL, 0xC00);
    }

    i4Ret = 0;
    if(_fgIsrEn)
    {
        i4Ret = x_sema_lock_timeout(_hLZHSSema, LZHS_TIMEOUT);
    }
    else
    {
        while((BIM_READ32(REG_LZHS_CTRL)&0x100)==0x0)
        {
            ;
        }
    }

    //----------------------------------------
#ifdef CHANGE_DECOMPRESS_CLK    
    CKGEN_WRITE32(REG_CPU_CKCFG, u4OrgClkReg);
#endif

    HAL_GetTime(&time2);

    u1CheckSum = (BIM_READ32(REG_LZHS_CTRL)) & 0xFF;

    //--------------------------------------
    // Disable LZHS and switch clock
    BIM_WRITE32(REG_LZHS_RAMCK, 0);
    do {
        u4Reg = BIM_READ32(REG_LZHS_RAMCK);
    } while (u4Reg & LZHS_CHGOK);

    if(_fgIsrEn && (i4Ret == OSR_TIMEOUT))
    {
        Printf("LZHS hardware decode fail !\n");
        return 1;
    }
    else
    {
        Printf("LZHS hardware decode done, CheckSum: 0x%X\n", u1CheckSum);
        HAL_GetDeltaTime(&timediff, &time1, &time2);
        Printf(" time: %d sec, %d usec\n", timediff.u4Seconds, timediff.u4Micros);
    }

    if(pu1CheckSum != NULL)
    {
        *pu1CheckSum = u1CheckSum;
    }
    else
    {
        Printf("LZHS return checksum pointer is NULL\n");
    }

#endif /* CC_MTK_LOADER */    
    return 0;
}


