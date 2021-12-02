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
#include "drvcust_if.h"
#include "c_model.h"

// #define CC_ACD_TEST

extern void CHIP_DisplayChar(UINT32 u4Char);
extern void CHIP_DisplayString(const INT8 *szString);

void CHIP_ClockSetting(void);
void CHIP_BimInit(void);
void CHIP_1stBootRegInit(void);

#define BIM_READ32(off)         IO_READ32(BIM_BASE, (off))
#define BIM_WRITE32(off, val)   IO_WRITE32(BIM_BASE, (off), (val))

#define REG_RW_REMAP            (0x001c)        // Remap Register
    #define REMAP_ENABLE        (1)
#define REG_RW_BUSTIME          (0x0024)        // BUS Access Timing Register
    #define BUS_RT_EN           (1U << 21)      // RISC timeout counter enable bit
#define REG_RW_IRQEN            (0x0034)        // RISC IRQ Enable Register
#define REG_RW_IRQCLR           (0x0038)        // RISC IRQ Clear Register
#define REG_RW_FIQEN            (0x0040)        // RISC IRQ Enable Register
#define REG_RW_FIQCLR           (0x0044)        // RISC IRQ Clear Register
#define REG_RW_TIMER2_LLMT      (0x0070)        // RISC Timer 2 Low Limit Register
#define REG_RW_TIMER2_LOW       (0x0074)        // RISC Timer 2 Low Register
#define REG_RW_TIMER2_HLMT      (0x0190)        // RISC Timer 2 High Limit Register
#define REG_RW_TIMER2_HIGH      (0x0194)        // RISC Timer 2 High Register

#define REG_RW_TIMER_CTRL   0x0078      // RISC Timer Control Register
    #define TMR0_CNTDWN_EN  (1U << 0)   // Timer 0 enable to count down
    #define TMR0_AUTOLD_EN  (1U << 1)   // Timer 0 auto-load enable
    #define TMR1_CNTDWN_EN  (1U << 8)   // Timer 1 enable to count down
    #define TMR1_CNTDWN_EN  (1U << 8)   // Timer 1 enable to count down
    #define TMR1_AUTOLD_EN  (1U << 9)   // Timer 1 auto-load enable
    #define TMR2_CNTDWN_EN  (1U << 16)  // Timer 2 enable to count down
    #define TMR2_AUTOLD_EN  (1U << 17)  // Timer 2 auto-load enable

#define REG_RW_GPRB0        0x00e0      // RISC Byte General Purpose Register 0

#define BIM_SetTimeLog(x)       BIM_WRITE32((REG_RW_GPRB0 + (x << 2)), BIM_READ32(REG_RW_TIMER2_LOW))

#define PDWNC_READ32(off)       IO_READ32(PDWNC_BASE, (off))
#define PDWNC_WRITE32(off, val) IO_WRITE32(PDWNC_BASE, (off), (val))

#define REG_RW_IOPADEN          (0x0080)        // IO Pad Output Enable Register
    #define PAD_UR0_TX_EN       (1U << 15)      // UART 0 TX PAD output enable
#define REG_RW_WATCHDOG_EN      0x0100          // Watchdog Timer Control Register
#define REG_RW_WATCHDOG_TMR     0x0104          // Watchdog Timer Register
#define REG_RW_WAKE_RSTCNT      0x0108          // Wakeup Reset Counter Register

// Serial flash
#define SFLASH_CFG1_REG			0x0760
#define SFLASH_CKGEN_REG        0x07B0
#define SFLASH_SAMPLE_EDGE_REG  0x07B4

#define XTAL_STRAP_MODE     (CKGEN_READ32(0) & 0x60)
    #define SET_XTAL_27MHZ  (0U << 5)
    #define SET_XTAL_60MHZ  (1U << 5)
    #define SET_XTAL_48MHZ  (2U << 5)
    #define SET_XTAL_54MHZ  (3U << 5)

void CHIP_Delay_us(UINT32 u4Micros)
{
    UINT32 u4Xtal;
    UINT64 u8Current;
    UINT64 u8TimeStop;

    switch(XTAL_STRAP_MODE) {
    case SET_XTAL_27MHZ: u4Xtal = 27*u4Micros; break;
    case SET_XTAL_60MHZ: u4Xtal = 60*u4Micros; break;
    case SET_XTAL_48MHZ: u4Xtal = 48*u4Micros; break;
    case SET_XTAL_54MHZ: u4Xtal = 54*u4Micros; break;
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

void CHIP_ClockSetting()
{
    BIM_WRITE32(0x24, 0x10); // delay register read path 1T.
#ifdef CC_5391_LOADER
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
#endif /* CC_5391_LOADER */

#ifndef CC_RUN_ON_NOR_EMPTY
    // Add Bandgap toggle protection. 0->1->0 then wait 500us.
    CKGEN_WRITE32(REG_PLL_TOPCFG0, CKGEN_READ32(REG_PLL_TOPCFG0) & (~(RG_PLL_BG_PWD)));
    CKGEN_WRITE32(REG_PLL_TOPCFG0, CKGEN_READ32(REG_PLL_TOPCFG0) | (RG_PLL_BG_PWD));
    CKGEN_WRITE32(REG_PLL_TOPCFG0, CKGEN_READ32(REG_PLL_TOPCFG0) & (~(RG_PLL_BG_PWD)));
    CHIP_Delay_us(500);

    if (XTAL_STRAP_MODE == SET_XTAL_27MHZ)
    {
        // 1. set TVDPLL for 540MHz from Xtal 27MHz*20
#ifdef CC_ACD_TEST
        CKGEN_WRITE32(0x170, 0xF0039EEC);       // acd test code.
#else /* CC_ACD_TEST */
        CKGEN_WRITE32(0x170, 0xF00312E0);       // original
#endif /* CC_ACD_TEST */
        CKGEN_WRITE32(0x174, 0x000000C0);
        CKGEN_WRITE32(0x174, 0x00000040);
        CHIP_Delay_us(1);
        CKGEN_WRITE32(0x174, 0x00000041);
#ifndef CC_DTDPLL_432MHZ
        // 2.1 set DTDPLL for 600MHz from TVDPLL/9 60MHz*10
        CKGEN_WRITE32(0x180, 0xF00308E1);
        CKGEN_WRITE32(0x184, 0x000000C0);
        CKGEN_WRITE32(0x184, 0x00040040);
        CHIP_Delay_us(1);
        CKGEN_WRITE32(0x184, 0x00040041);
#else /* CC_DTDPLL_432MHZ */
        // 2.2 set DTDPLL for 432MHz from Xtal 27MHz*16
        CKGEN_WRITE32(0x180, 0xF0030EE0);
        CKGEN_WRITE32(0x184, 0x000000C0);
        CKGEN_WRITE32(0x184, 0x00000040);
        CHIP_Delay_us(1);
        CKGEN_WRITE32(0x184, 0x00000041);
#endif /* CC_DTDPLL_432MHZ */
    }
    else if (XTAL_STRAP_MODE == SET_XTAL_60MHZ)
    {
        // 1. set TVDPLL for 540MHz from Xtal 60MHz*9
        CKGEN_WRITE32(0x170, 0xF00307E0);
        CKGEN_WRITE32(0x174, 0x000000C0);
        CKGEN_WRITE32(0x174, 0x00000040);
        CHIP_Delay_us(1);
        CKGEN_WRITE32(0x174, 0x00000041);
        // 2. set DTDPLL for 600MHz from Xtal 60MHz*10
        CKGEN_WRITE32(0x180, 0xF00308E0);
        CKGEN_WRITE32(0x184, 0x000000C0);
        CKGEN_WRITE32(0x184, 0x00000040);
        CHIP_Delay_us(1);
        CKGEN_WRITE32(0x184, 0x00000041);
    }
    else if (XTAL_STRAP_MODE == SET_XTAL_48MHZ)
    {
        // 1. set DTDPLL for 432MHz from Xtal 48MHz*9
        CKGEN_WRITE32(0x180, 0xF00307E0);
        CKGEN_WRITE32(0x184, 0x000000C0);
        CKGEN_WRITE32(0x184, 0x00000040);
        CHIP_Delay_us(1);
        CKGEN_WRITE32(0x184, 0x00000041);
        // 2. set TVDPLL for 540MHz from DTDPLL/8 54MHz*10
        CKGEN_WRITE32(0x170, 0xF00308E1);
        CKGEN_WRITE32(0x174, 0x000400C0);
        CKGEN_WRITE32(0x174, 0x00040040);
        CHIP_Delay_us(1);
        CKGEN_WRITE32(0x174, 0x00040041);
    }
    else if (XTAL_STRAP_MODE == SET_XTAL_54MHZ)
    {
        // 1. set TVDPLL for 540MHz from Xtal 54MHz*10
        CKGEN_WRITE32(0x170, 0xF00308E0);
        CKGEN_WRITE32(0x174, 0x000000C0);
        CKGEN_WRITE32(0x174, 0x00000040);
        CHIP_Delay_us(1);
        CKGEN_WRITE32(0x174, 0x00000041);
#ifndef CC_DTDPLL_432MHZ
        // 2.1 set DTDPLL for 600MHz from TVDPLL/9 60MHz*10
        CKGEN_WRITE32(0x180, 0xF00308E1);
        CKGEN_WRITE32(0x184, 0x000000C0);
        CKGEN_WRITE32(0x184, 0x00040040);
        CHIP_Delay_us(1);
        CKGEN_WRITE32(0x184, 0x00040041);
#else /* CC_DTDPLL_432MHZ */
        // 2.2 set DTDPLL for 432MHz from Xtal 54MHz*8
        CKGEN_WRITE32(0x180, 0xF00306E0);
        CKGEN_WRITE32(0x184, 0x000000C0);
        CKGEN_WRITE32(0x184, 0x00000040);
        CHIP_Delay_us(1);
        CKGEN_WRITE32(0x184, 0x00000041);
#endif /* CC_DTDPLL_432MHZ */
    }
    else
    {
        while (1) { }
    }

    // 3. set SYSPLL for 648MHz
#ifdef CC_ACD_TEST
    CKGEN_WRITE32(0x160, CKGEN_READ32(0x160) | (7U << 29));
    CKGEN_WRITE32(0x164, 0x0CBE4403);
#endif /* CC_ACD_TEST */
    CKGEN_WRITE32(0x168, 0x00000000);
    CHIP_Delay_us(1);
    CKGEN_WRITE32(0x168, 0x01000000);

    CKGEN_WRITE32(0x204, 0x00002100);
#ifdef CC_VCXO_ON
    if ((CKGEN_READ32(0x204) & (7U << 16)) == (7U << 16))
    {
        CKGEN_WRITE32(0x204, 0x00001100);
    }
    CKGEN_WRITE32(0x160, (CKGEN_READ32(0x160) & ~(3U << 16)) | (1U << 16));   // From VCXO
#endif /* CC_VCXO_ON */

    // 4. Set CPUPLL
//    CKGEN_WRITE32(0x100, 0xF0032464);   // 0x24=>(36+2)*27/2 = 513MHz
    CKGEN_WRITE32(0x100, 0xF0032A64);   // 0x2A=>(42+2)*27/2 = 594MHz
//    CKGEN_WRITE32(0x100, 0xF0033064);   // 0x30=>(48+2)*27/2 = 675MHz
    CKGEN_WRITE32(0x104, 0x000000C0);
    CKGEN_WRITE32(0x104, 0x00000040);
    CHIP_Delay_us(1);
    CKGEN_WRITE32(0x104, 0x00000041);

    // 5. Select CPU clock to CPUPLL and Bus clock
    // CKGEN_WRITE32(0x208, 0x00000101); // 180MHz
    CKGEN_WRITE32(0x208, 0x00000301); // 162MHz

#endif /* CC_RUN_ON_NOR_EMPTY */

#ifndef CC_RUN_ON_NOR_EMPTY
#if 0
    if (GET_XTAL_CLK() == 27000000)
    {
        BIM_WRITE32(SFLASH_CKGEN_REG, 4);   // 13.5MHz
    }
    else if(GET_XTAL_CLK() == 54000000)
    {
        BIM_WRITE32(SFLASH_CKGEN_REG, 3);   // 18MHz
    }
#endif
    BIM_WRITE32(SFLASH_CKGEN_REG, 1);   // 20MHz
#endif /* CC_RUN_ON_NOR_EMPTY */
#ifndef CC_SET_DEFAULT_PBIC_DRIVING
    CKGEN_WRITE32(0x320, CKGEN_READ32(0x320)|8);
#endif
#ifndef CC_5391_LOADER
    // set watchdog timer as max value.
    PDWNC_WRITE32(REG_RW_WATCHDOG_TMR, 0);
#endif /* CC_5391_LOADER */
}

void CHIP_BimInit()
{
    // Disable irq & fiq
    BIM_WRITE32(REG_RW_IRQEN, 0);
    BIM_WRITE32(REG_RW_FIQEN, 0);
    BIM_WRITE32(REG_RW_IRQCLR, 0xffffffff);
    BIM_WRITE32(REG_RW_FIQCLR, 0xffffffff);

    // Enable reigster bus access timeout, to avoid h/w hang forever.
    BIM_WRITE32(REG_RW_BUSTIME, (BIM_READ32(REG_RW_BUSTIME) | BUS_RT_EN));
}

void CHIP_1stBootRegInit()
{
//    UINT32 u4Val;

    // 5. Setup clock source
//    u4Val = CKGEN_READ32(REG_XTAL_CKCFG);
//    u4Val &= ~(RG_C27M_CK | RG_VCXO27_CK);
//    u4Val |= (DEFAULT_C27M | DEFAULT_VCXO);
//    CKGEN_WRITE32(REG_XTAL_CKCFG, u4Val);

#ifndef CC_5391_PRELOADER
    // Initial timer, then can use HAL_Delay_us()
    VERIFY(HAL_InitTimer());
#endif /* CC_5391_PRELOADER */

#ifdef CC_5392B_PRELOADER
#ifdef CC_NAND_BOOT
#ifdef CC_ROM_BOOT
    CHIP_NANDCopyToDRAM((UINT32 *)0x2B001400, 1024*9, 1024*2);      //Support preloader size is over 9KB in ROM BOOT case
#else /* CC_ROM_BOOT */
    CHIP_NANDCopyToDRAM((UINT32 *)0x1000, 0x1000, 0x1800 - 0x40);
#endif /* CC_ROM_BOOT */
    ARM_InvalidateICache();
#endif /* CC_NAND_BOOT */
#endif /* CC_5392B_PRELOADER */ 
}

#if defined(CC_5391_PRELOADER) && defined(__KERNEL__)
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

