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
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

//============================================================================
// NOTE:
//      In this file, all function cannot call sub-function!!!
//============================================================================

#include "rs232.h"

#include "x_ckgen.h"
#include "x_assert.h"
#include "x_timer.h"
#include "x_bim.h"
#include "x_pdwnc.h"
#include "drvcust_if.h"
#include "c_model.h"


extern void CHIP_DisplayChar(UINT32 u4Char);
extern void CHIP_DisplayString(const INT8 *szString);

void CHIP_ClockSetting(void);
void  Chip_TimerInit(void);
void CHIP_BimInit(void);
void CHIP_1stBootRegInit(void);


// Serial flash
#define SFLASH_CFG1_REG         0x0760
#define SFLASH_CKGEN_REG        0x07B0
#define SFLASH_SAMPLE_EDGE_REG  0x07B4

void CHIP_Delay_us(UINT32 u4Micros)
{
    UINT32 u4Xtal;
    UINT64 u8Current;
    UINT64 u8TimeStop;

    switch(XTAL_STRAP_MODE) {
    case SET_XTAL_27MHZ: 
    case SET_XTAL_27MHZ_A: 
        u4Xtal = 27*u4Micros; break;
    case SET_XTAL_54MHZ:
        u4Xtal = 54*u4Micros; break;
    default:
        while (1) { }
    }
    do {
        u8Current = BIM_READ32(REG_RW_TIMER2_HIGH);
        u8Current = u8Current << 32;
        u8Current |= BIM_READ32(REG_RW_TIMER2_LOW);
        u8TimeStop = u8Current - u4Xtal;
    } while (u8TimeStop > u8Current);
    do {
        u8Current = BIM_READ32(REG_RW_TIMER2_HIGH);
        u8Current = u8Current << 32;
        u8Current |= BIM_READ32(REG_RW_TIMER2_LOW);
    } while (u8TimeStop < u8Current);
}

void Chip_TimerInit(void)
{
#ifdef CC_MTK_PRELOADER
        // Start timer 2 for bootup time counter enable.
        BIM_WRITE32(REG_RW_TIMER2_LLMT, 0xffffffff);
        BIM_WRITE32(REG_RW_TIMER2_LOW , 0xffffffff);
        BIM_WRITE32(REG_RW_TIMER2_HLMT, 0xffffffff);
        BIM_WRITE32(REG_RW_TIMER2_HIGH, 0xffffffff);
        BIM_SetTimeLog(0);
        BIM_SetTimeLog(1);
        BIM_SetTimeLog(2);
        BIM_SetTimeLog(3);
        BIM_SetTimeLog(4);
        BIM_SetTimeLog(5);
        BIM_SetTimeLog(6);
        BIM_SetTimeLog(7);
        BIM_WRITE32(REG_RW_TIMER_CTRL, (TMR2_CNTDWN_EN | TMR2_AUTOLD_EN));
#endif
}

void CHIP_ClockSetting()
{
    //it's workaround in 5391, should not need in 5387
    //BIM_WRITE32(0x24, 0x10); // delay register read path 1T.

#ifdef CC_MTK_LOADER
    if ((!(BIM_READ32(REG_RW_REMAP) & REMAP_ENABLE)) && (!(BIM_READ32(REG_RW_TIMER_CTRL) & TMR2_CNTDWN_EN)))
    {
        // Start timer 2 for bootup time counter enable.
        BIM_WRITE32(REG_RW_TIMER2_LLMT, 0xffffffff);
        BIM_WRITE32(REG_RW_TIMER2_LOW , 0xffffffff);
        BIM_WRITE32(REG_RW_TIMER2_HLMT, 0xffffffff);
        BIM_WRITE32(REG_RW_TIMER2_HIGH, 0xffffffff);
        BIM_SetTimeLog(0);
        BIM_SetTimeLog(1);
        BIM_SetTimeLog(2);
        BIM_SetTimeLog(3);
        BIM_SetTimeLog(4);
        BIM_SetTimeLog(5);
        BIM_SetTimeLog(6);
        BIM_SetTimeLog(7);
        BIM_WRITE32(REG_RW_TIMER_CTRL, (TMR2_CNTDWN_EN | TMR2_AUTOLD_EN));
    }
#endif /* CC_MTK_LOADER */

#ifndef CC_RUN_ON_NOR_EMPTY
    CHIP_Delay_us(500);
    // 3. set SYSPLL for  324 MHz
    if (IS_XTAL_27MHZ())
    {
        CKGEN_WRITE32(0x10C, 0x198C0200); //(27*12 = 324MHz)
    }
    else if (IS_XTAL_54MHZ())
    {
        CKGEN_WRITE32(0x10C, 0xD98C0200); //(27*12 = 324MHz)
    }
    CKGEN_WRITE32(0x110, CKGEN_READ32(0x110) |0xC1050004); //Sel 324MHz
    CKGEN_WRITE32(0x114, 0x00003001);

    // 4. Set CPUPLL

    // 5. Select CPU clock to CPUPLL and Bus clock
    // Begin                                                                     
    // Power up Bandgap                                                          
    IO_WRITE32(0x2002b000, 0x104, 0x00000000);                                   
    IO_WRITE32(0x2002b000, 0x108, 0x09a89500);                                   
    // Re-init DEMOD PLL                                                         
    IO_WRITE32(0x2000d000, 0x1e8, 0x00000100);                                   
    if (IS_XTAL_54MHZ())
    {
        IO_WRITE32(0x2000d000, 0x1e0, 0x0021b527);
    }
    else if (IS_XTAL_27MHZ())
    {
        IO_WRITE32(0x2000d000, 0x1e8, 0x000009ff);                                   
    }

    // init USB PLL
    IO_WRITE32(MUSB_BASE, 0x600, 0); 
    CHIP_Delay_us(500);
    IO_WRITE32(MUSB_BASE, 0x600, 3); 
    IO_WRITE32(MUSB_BASE,0x40C, (IO_READ32(MUSB_BASE,0x40C)& 0xFFFFFF00) |0x26); 
    IO_WRITE32(MUSB_BASE,0x404, IO_READ32(MUSB_BASE,0x404) |0x01);   // enable USB PLL

   #if  1//def CC_MT8223H
    CKGEN_WRITE32(REG_CPU_CKCFG, 0x0002000C); // cpu: 240mhz, bus: 240/2mhz  Select   usb PLL
    #else //default use Sys PLL
    CKGEN_WRITE32(REG_CPU_CKCFG, 0x00020006); // cpu: 162mhz, bus: 324/2mhz
    #endif

    //enable all video in clk
    CKGEN_WRITE32(REG_VPCLK_STOP, 0x0);

#endif /* CC_RUN_ON_NOR_EMPTY */

#ifndef CC_RUN_ON_NOR_EMPTY
    // 4mA
    //IO_WRITE32MSK(CKGEN_BASE, 0x32C, 0x01, 0x01);//flash driving current
    //IO_WRITE32MSK(CKGEN_BASE, 0x320, 0x01, 0x01);//flash driving current

    BIM_WRITE32(SFLASH_SAMPLE_EDGE_REG, 1);
    BIM_WRITE32(SFLASH_CFG1_REG, 1); // fast read

    // BIM_WRITE32(SFLASH_CKGEN_REG, 0); // 13.5
    BIM_WRITE32(SFLASH_CKGEN_REG, 1); // 27
    // BIM_WRITE32(SFLASH_CKGEN_REG, 4); // 54
    // BIM_WRITE32(SFLASH_CKGEN_REG, 3); // 72
    //BIM_WRITE32(SFLASH_CKGEN_REG, 7); // 40.5 from syspll
#endif /* CC_RUN_ON_NOR_EMPTY */

    // C27M
    if (IS_XTAL_27MHZ())
    {
        CKGEN_WRITE32(0x204, 0x000); // xtal
    }
    else if (IS_XTAL_54MHZ())
    {
        CKGEN_WRITE32(0x204, 0x100); // xtal/2
    }

#ifndef CC_MTK_LOADER
    // set watchdog timer as max value.
    vIO32Write4B(PDWNC_WDT0, 0);//PDWNC_WRITE32(REG_RW_WATCHDOG_TMR, 0);
#endif /* CC_MTK_LOADER */
}

void CHIP_BimInit()
{
    // Disable irq & fiq
    BIM_WRITE32(REG_RW_IRQEN, 0);
    BIM_WRITE32(REG_RW_FIQEN, 0);
    BIM_WRITE32(REG_RW_IRQCL, 0xffffffff);
    BIM_WRITE32(REG_RW_FIQCL, 0xffffffff);
}

void CHIP_1stBootRegInit()
{
//    UINT32 u4Val;

    // 5. Setup clock source
//    u4Val = CKGEN_READ32(REG_XTAL_CKCFG);
//    u4Val &= ~(RG_C27M_CK | RG_VCXO27_CK);
//    u4Val |= (DEFAULT_C27M | DEFAULT_VCXO);
//    CKGEN_WRITE32(REG_XTAL_CKCFG, u4Val);

#ifndef CC_MTK_PRELOADER
    // Initial timer, then can use HAL_Delay_us()
    VERIFY(HAL_InitTimer());
#endif /* CC_MTK_PRELOADER */

//#ifdef CC_MTK_PRELOADER
// no need for 5387
#if 0
    #ifdef CC_NAND_BOOT
        #ifdef CC_ROM_BOOT
            CHIP_NANDCopyToDRAM((UINT32 *)0x2B001400, 1024*9, 1024*2);      //Support preloader size is over 9KB in ROM BOOT case
        #else /* CC_ROM_BOOT */
            CHIP_NANDCopyToDRAM((UINT32 *)0x1000, 0x1000, 0x1800 - 0x40);
        #endif /* CC_ROM_BOOT */
        ARM_InvalidateICache();
    #endif /* CC_NAND_BOOT */
#endif /* CC_MTK_PRELOADER */ 
}

#if defined(CC_MTK_PRELOADER) && defined(__KERNEL__)
/* Replacement (=dummy) for GNU/Linux undefined reference on gcc preloader */
void raise(void)
{
    return;
}

void __aeabi_unwind_cpp_pr0(void)
{
    return;
}

extern void raise(void);
extern void __aeabi_unwind_cpp_pr0(void);

#endif

