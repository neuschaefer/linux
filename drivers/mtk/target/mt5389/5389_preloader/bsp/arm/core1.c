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




//lint --e{950} No ANSI reserved word (__asm)

//lint --e{529} Symbol not subsequently referenced
//lint --e{530} Symbol not initialized
//lint --e{715} Symbol not referenced
//lint --e{506} Constant value Boolean

//lint --e{641} Converting enum to int

#define XOR_CACHE                   (1 << 7)
#define FOZ                         (1 << 3)
#define L1C_PREFETCH                (1 << 2)
#define L2C_PREFETCH_HINT           (1 << 1)

//-----------------------------------------------------------------------------

// Include files
//-----------------------------------------------------------------------------



#include "x_typedef.h"
#include "x_hal_arm.h"

#include "x_hal_5381.h"

#include "x_bim.h"
#include "x_assert.h"
#include "drvcust_if.h"
#include "c_model.h"

//#define CC_SIMPLE_TZ


typedef VOID (*KERNEL_START)(VOID);



void BIM_Core1Stub(void)

{
#if defined(CC_MT5396) || defined(CC_MT5398)
#ifdef __GCC__
    register unsigned int reg = LINUX_KERNEL_MEM_SIZE + 0x4000;
    __asm__ ("isb");
    __asm__ ("bl     inv_cache");

    __asm__ ("wfi");
    __asm__ ("LOOP:");
//    __asm__ ("B       LOOP");
    __asm__ ("ldr     r0, =0xf0008068");
    __asm__ ("ldr     r1, =0xffffb007");
    __asm__ ("ldr     r2, [r0]");
    __asm__ ("cmp     r2, r1");
    __asm__ ("bne     LOOP");

#ifdef CC_TRUSTZONE_SUPPORT
    __asm__ ("MRC     p15, 0, r0, c1, c0, 2       @ Read CP15 control register");
    __asm__ ("ORR     r0, r0, #0xf00000           @ Enable CP10 ACCESS");
    __asm__ ("MCR     p15, 0, r0, c1, c0, 2       @ Write back control register");
    __asm__ ("MOV     r0, #0");
    __asm__ ("MCR     p15, 0, r0, c7, c5, 4       @ISB");
    __asm__ ("MOV     r0,#0x40000000              @ VFP enable bit");
    __asm__ ("FMXR    FPEXC,r0");
    //__asm__ ("ldr     r1, =0x10404000");
    __asm__ ("MOV    r1, %0" : : "r" (reg));
    __asm__ ("mov     r3, pc");
    __asm__ ("blx     r1");
    __asm__ ("bl     inv_cache");
#endif //CC_TRUSTZONE_SUPPORT

    __asm__ ("ldr     r0, =0xf0008188");
    __asm__ ("ldr     r1, [r0]");
    __asm__ ("blx     r1");

    __asm__ volatile (
             "inv_cache:                              \n"
             "       mcr  p15, 0, r0, c8, c7, 0       \n"
             "       dmb                              \n"
             "       mov  r0, #255       @ Sets to go \n"
             "1:                                      \n"
             "       mov  r1, #3         @ 4 Ways     \n"
             "2:                                      \n"
             "       mov  r2, r1, lsl #30             \n"
             "       orr  r2, r2, r0, lsl #5          \n"
             "       mcr  p15, 0, r2, c7, c6, 2  @ Invalidate\n"
             "       subs r1, r1, #1                  \n" 
             "       bge  2b                          \n"
             "       subs r0, r0, #1                  \n"
             "       bge  1b                          \n"
             "       dsb                              \n"
             "       mov  pc, lr                      \n");

#else

    UINT32 u4JumpAddress;
    KERNEL_START pfnStart;
    register UINT32 r = 0;

    __asm
    {
        MCR     p15, 0, r, c7, c5, 4
    }

    // write 0x8068=0x5396c2 to make sure core2 is alive
    BIM_WRITE32(REG_RW_TIMER1_LLMT, 0x005396c2);

    while (BIM_READ32(REG_RW_TIMER1_LLMT)!=0xffffb007)
    {
        u4JumpAddress = BIM_READ32(REG_RW_TIMER1_LOW);
        BIM_WRITE32(REG_RW_TIMER1_LOW, u4JumpAddress+1);

    }

    u4JumpAddress = BIM_READ32(REG_RW_TIMER1_HLMT);
    if (u4JumpAddress)
    {
        pfnStart = (KERNEL_START)(u4JumpAddress);
        pfnStart();
    }

#endif //__GCC__

#endif //CC_MT5396 || CC_MT5398
}
