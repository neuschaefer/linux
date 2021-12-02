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

#include "x_typedef.h"
#include "x_pdwnc.h"
#include "x_ckgen.h"
#include "x_bim.h"
#include "x_lint.h"
#include "x_printf.h"
#include "x_hal_5381.h"
#include "x_dram.h"
#include <string.h>


//-----------------------------------------------------------------------------
// Extern functions declaration
//-----------------------------------------------------------------------------
EXTERN INT32 Printf(const CHAR *format, ...);
EXTERN void RemapJumpToDram(void);
EXTERN void CHIP_DisplayChar(UINT32 u4Char);
EXTERN void CHIP_DisplayDigit(UINT32 u4Char);
#ifdef CC_UBOOT
EXTERN void *x_memcpy(void *, const void *, UINT32);
#else
EXTERN void *x_memcpy(void *, const void *, SIZE_T);
#endif    

//lint --e{961} Violates MISRA Advisory Rule 28, 'register' class
//lint --e{950} No ANSI reserved word (__asm)
//lint --e{529} Symbol not subsequently referenced
//lint --e{506} Constant value Boolean
//lint --e{774} Boolean within 'if' always evaluates to constant
//lint --e{775} non-negative quantity cannot be less than zero
LINT_SUPPRESS_SYMBOL(18, Printf)

#if defined(CC_MTK_PRELOADER) && defined(NDEBUG)
#define Printf(fmt...)
#endif /* defined(CC_MTK_PRELOADER) && defined(NDEBUG) */

#if !defined(CC_MTK_PRELOADER) && !defined(CC_MTK_LOADER)
#ifndef __KERNEL__
//lint --e{77}
LINT_SAVE_AND_DISABLE
EXTERN UINT32 Image$$ENDCLI$$Limit;
LINT_RESTORE
#endif /* __KERNEL__ */
#else /* !defined(CC_MTK_PRELOADER) && !defined(CC_MTK_LOADER) */
#ifndef __KERNEL__
//lint --e{77}
LINT_SAVE_AND_DISABLE
EXTERN UINT32 Image$$BSS$$Limit;
LINT_RESTORE
#else /* __KERNEL__ */
//EXTERN UINT32 _end;
#ifdef CC_NAND_BOOT
#define CHUNK_SIZE      0x1000      //This size is for NAND chunk that includes nand3_preloader and zboot.img
EXTERN UINT32 _end;
#else /* CC_NAND_BOOT */
EXTERN UINT32 _end;
#endif /* CC_NAND_BOOT */
#endif /* __KERNEL__ */
#endif /* !defined(CC_MTK_PRELOADER) && !defined(CC_MTK_LOADER) */

extern void RemapJumpToDram(void);

#if defined(CC_MTK_PRELOADER) && defined(NDEBUG)

void CHIP_DisplayHex(UINT32 u4Val)
{
    CHIP_DisplayChar('0');
    CHIP_DisplayChar('x');
    CHIP_DisplayDigit(((u4Val >> 28) & 0x0f));
    CHIP_DisplayDigit(((u4Val >> 24) & 0x0f));
    CHIP_DisplayDigit(((u4Val >> 20) & 0x0f));
    CHIP_DisplayDigit(((u4Val >> 16) & 0x0f));
    CHIP_DisplayDigit(((u4Val >> 12) & 0x0f));
    CHIP_DisplayDigit(((u4Val >> 8) & 0x0f));
    CHIP_DisplayDigit(((u4Val >> 4) & 0x0f));
    CHIP_DisplayDigit(((u4Val >> 0) & 0x0f));
}

#endif /* defined(CC_MTK_PRELOADER) && defined(NDEBUG) */

void CHIP_Reboot()
{
    HAL_WRITE32(PDWNC_WDT0, 0xffffff00);//PDWNC_WRITE32(REG_RW_WATCHDOG_TMR, 0xffffff00);//vIO32Write4B(PDWNC_WDT0, 0xffffff00);
    HAL_WRITE32(PDWNC_WDTCTL, 0x1);//PDWNC_WRITE32(REG_RW_WATCHDOG_EN, 1);//vIO32WriteFldAlign(PDWNC_WDTCTL, 0x1, FLD_WDT0E);
    while (1) { }
}

void CLK_Shadow()
{

    UINT32 *pu4Nor, *pu4Mem;
    UINT32 fgNotEqual;

#if !defined(CC_MTK_PRELOADER) && !defined(CC_MTK_LOADER)
    LINT_SAVE_AND_DISABLE
        x_memcpy((void *)INIT_DRAM_B_BASE, (void *)0, (UINT32)&Image$$ENDCLI$$Limit);
    LINT_RESTORE
#else /* !defined(CC_MTK_PRELOADER) && !defined(CC_MTK_LOADER) */
    #ifndef __KERNEL__
        LINT_SAVE_AND_DISABLE
        #ifdef CC_ROM_BOOT
            x_memcpy((void *)INIT_DRAM_B_BASE, (void *)SRAM_ITCM_BASE, ((UINT32)&Image$$BSS$$Limit) - SRAM_ITCM_BASE );
        #else /* CC_ROM_BOOT */
            x_memcpy((void *)INIT_DRAM_B_BASE, (void *)0, (UINT32)&Image$$BSS$$Limit);
        #endif /* CC_ROM_BOOT */
        LINT_RESTORE
    #else /* __KERNEL__ */
        #ifdef CC_NAND_BOOT
            #ifndef CC_MTK_PRELOADER
                x_memcpy((void *)INIT_DRAM_B_BASE, (void *)0, (UINT32)(CHUNK_SIZE) );
            #else /* CC_MTK_PRELOADER */
                #ifdef CC_ROM_BOOT
                    x_memcpy((void *)INIT_DRAM_B_BASE, (void *)SRAM_ITCM_BASE, (UINT32) ((UINT32)(&_end) - SRAM_ITCM_BASE + 280 ) );
                #else
                    x_memcpy((void *)INIT_DRAM_B_BASE, (void *)0, (UINT32) ((&_end) + 280) );
                #endif
            #endif  /* CC_MTK_PRELOADER */
        #else /* CC_NAND_BOOT */
            #ifdef CC_ROM_BOOT
                x_memcpy((void *)INIT_DRAM_B_BASE, (void *)SRAM_ITCM_BASE, (UINT32) ((UINT32)(&_end) - SRAM_ITCM_BASE + 280 ) );
            #else /* CC_ROM_BOOT */
                x_memcpy((void *)INIT_DRAM_B_BASE, (void *)0, (UINT32) ((&_end) + 280) );
            #endif /* CC_ROM_BOOT */
        #endif /* CC_NAND_BOOT */
    #endif /* __KERNEL__ */
#endif /* !defined(CC_MTK_PRELOADER) && !defined(CC_MTK_LOADER) */

#ifdef CC_ROM_BOOT
    pu4Nor = (UINT32 *)SRAM_ITCM_BASE;
#else /* CC_ROM_BOOT */
    pu4Nor = (UINT32 *)0;
#endif /* CC_ROM_BOOT */
    pu4Mem = (UINT32 *)INIT_DRAM_B_BASE;

    fgNotEqual = 0;
#if !defined(CC_MTK_PRELOADER) && !defined(CC_MTK_LOADER)
    LINT_SAVE_AND_DISABLE
    while ((fgNotEqual < 20) && (pu4Nor < (&Image$$ENDCLI$$Limit)))
    LINT_RESTORE
#else /* #if !defined(CC_MTK_PRELOADER) && !defined(CC_MTK_LOADER) */
    #ifndef __KERNEL__
    LINT_SAVE_AND_DISABLE
        while ((fgNotEqual < 20) && (pu4Nor < (&Image$$BSS$$Limit)))
    LINT_RESTORE
    #else /* __KERNEL__ */
        #ifdef CC_NAND_BOOT
            #ifndef CC_MTK_PRELOADER
                while ((fgNotEqual < 20) && (pu4Nor < (CHUNK_SIZE)))
            #else /* CC_MTK_PRELOADER */
                while ((fgNotEqual < 20) && (pu4Nor < ((UINT32)(&_end))+280))
            #endif  /* CC_MTK_PRELOADER */
        #else /* CC_NAND_BOOT */
            while ((fgNotEqual < 20) && (pu4Nor < ((UINT32)(&_end))+280))
        #endif /* CC_NAND_BOOT */
    #endif /* __KERNEL__ */
#endif /* #if !defined(CC_MTK_PRELOADER) && !defined(CC_MTK_LOADER) */
    {
        if ((*pu4Nor) != (*pu4Mem))
        {
            fgNotEqual++;
            Printf("0x%08x: Nor-0x%08x Mem-0x%08x\n", (UINT32)pu4Nor, *pu4Nor, *pu4Mem);
        }
        pu4Nor++;
        pu4Mem++;
    }

    if (!fgNotEqual)
    {
#ifdef CC_ROM_BOOT
        BIM_WRITE32(4, 0x80000000);
#endif /* CC_ROM_BOOT */
        RemapJumpToDram();
    }

    // TODO:fill the default monitor value
    //BIM_WRITE32(REG_RW_DBG_MONSEL, 0x6005e);

#ifdef NDEBUG
    // reboot in release mode if dram initialized failed.
    CHIP_Reboot();
#endif /* NDEBUG */


}

