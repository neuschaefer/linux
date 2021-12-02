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

#include "x_typedef.h"
#ifndef CC_UBOOT
#include "x_os.h"
#endif

#include "x_hal_926.h"
#include "x_ckgen.h"
#include "x_bim.h"
#include "x_pdwnc.h"
#include "x_printf.h"
#include "x_serial.h"
#include "x_dram.h"
#include "x_assert.h"
#include "x_lint.h"
#include "x_timer.h"

#include "drvcust_if.h"
#include "c_model.h"
#include "ddr.h"

#include <string.h>

//lint --e{961} Violates MISRA Advisory Rule 28, 'register' class
//lint --e{950} No ANSI reserved word (__asm)
//lint --e{529} Symbol not subsequently referenced
//lint --e{506} Constant value Boolean
//lint --e{774} Boolean within 'if' always evaluates to constant
//lint --e{775} non-negative quantity cannot be less than zero
LINT_SUPPRESS_SYMBOL(18, Printf)


//-----------------------------------------------------------------------------
// Config definition
//-----------------------------------------------------------------------------

#ifndef CC_INIT_BOOT_VERBOSE
#define CC_INIT_BOOT_VERBOSE        1
#endif /* CC_INIT_BOOT_VERBOSE */

#if defined(CC_5391_PRELOADER) && defined(NDEBUG)
#define Printf(fmt...)
#endif /* defined(CC_5391_PRELOADER) && defined(NDEBUG) */

//-----------------------------------------------------------------------------
// Constant definition
//-----------------------------------------------------------------------------

#define ICACHE_ENABLE               (1 << 12)
#define SDR_32_TURBO                0x00000570
//-----------------------------------------------------------------------------
// Public functions declaration
//-----------------------------------------------------------------------------

#ifndef CC_5391_LOADER
EXTERN UINT32 g_u4DramSize;
#endif /* CC_5391_LOADER */

EXTERN void RemapJumpToDram(void);

//-----------------------------------------------------------------------------
// This file
//-----------------------------------------------------------------------------

EXTERN UINT32 IsInitBootVerbose(void);
EXTERN UINT32 IsBootVerbose(void);
EXTERN void BootInit(void);
EXTERN void BootBimInitial(void);
EXTERN void CHIP_Delay_us(UINT32 u4Micros);
EXTERN UINT32 CLK_DmpllSpectrum(UINT32 u4Clk);
EXTERN UINT32 CLK_SetDmpll(UINT32 u4Clk, UINT32 fgFullFreq);
EXTERN UINT32 CLK_RingOscillatorTick(void);
EXTERN void HalSetInstTCMReg(UINT32 u4BaseAddr, UINT32 u4Size, BOOL fgEnable);

EXTERN UINT32 CLK_SetDdrOnNorFlash(UINT32 u4DefClk);
EXTERN UINT32 CLK_SetDdr(void);

EXTERN void CLK_QueryDramSetting(UINT8 *szString);
EXTERN void CLK_SetDdrPriority(void);
EXTERN void CHIP_DisplayString(const INT8 *szString);
EXTERN void CHIP_DisplayInteger(INT32 i);

//-----------------------------------------------------------------------------
// Private functions
//-----------------------------------------------------------------------------

#ifndef TCMSET_CHECKSUM
static void TCMSET_CHECKSUM()
{
    TCM_CHECKSUM = (((TCM_DRAM_FLAGS + TCM_DRAM_TYPE) + TCM_WAKEUP_STATUS) + 0x53815381);
}
#endif /* TCMSET_CHECKSUM */

#if defined(CC_5391_PRELOADER) && defined(NDEBUG)
#else /* defined(CC_5391_PRELOADER) && defined(NDEBUG) */

#ifndef TCMSET_DRAMTYPE
static void TCMSET_DRAMTYPE(UINT32 x)
{
    TCM_DRAM_TYPE &= ~(DRAM_TYPE_MASK);
    TCM_DRAM_TYPE |= (x);
    TCMSET_CHECKSUM();
}
#endif /* TCMSET_DRAMTYPE */

#ifndef TCMSET_COLADDR
static void TCMSET_COLADDR(UINT32 x)
{
    TCM_DRAM_FLAGS &= ~(DRAM_COL_ADDR);
    TCM_DRAM_FLAGS |= (((x) << COLADDR_SHIFT) & DRAM_COL_ADDR);
    TCMSET_CHECKSUM();
}
#endif /* TCMSET_COLADDR */

#if 0 // ndef TCMSET_SINGLEDQS
static void TCMSET_SINGLEDQS(UINT32 x)
{
    if (x) {
        TCM_DRAM_FLAGS |= DRAM_SINGLEDQS;
    } else {
        TCM_DRAM_FLAGS &= ~(DRAM_SINGLEDQS);
    }
    TCMSET_CHECKSUM();
}
#endif /* TCMSET_SINGLEDQS */

#if 0 // ndef TCMSET_DDR_QFP
static void TCMSET_DDR_QFP(UINT32 x)
{
    if (x) {
        TCM_DRAM_FLAGS |= DRAM_DDR_QFP;
    } else {
        TCM_DRAM_FLAGS &= ~(DRAM_DDR_QFP);
    }
    TCMSET_CHECKSUM();
}
#endif /* TCMSET_DDR_QFP */

#ifndef TCMSET_FULLFREQ
static void TCMSET_FULLFREQ(UINT32 x)
{
    if (x) {
        TCM_DRAM_FLAGS |= DRAM_FULLFREQ;
    } else {
        TCM_DRAM_FLAGS &= ~(DRAM_FULLFREQ);
    }
    TCMSET_CHECKSUM();
}
#endif /* TCMSET_FULLFREQ */

#ifndef TCMSET_FORCE32
static void TCMSET_FORCE32(UINT32 x)
{
    if (x) {
        TCM_DRAM_FLAGS |= DRAM_FORCE32;
    } else {
        TCM_DRAM_FLAGS &= ~(DRAM_FORCE32);
    }
    TCMSET_CHECKSUM();
}
#endif /* TCMSET_FORE32 */

#ifndef TCMSET_DDR_CLK
static void TCMSET_DDR_CLK(UINT32 x)
{
    if ((x) > CLK_405MHZ) {
        TCM_DRAM_FLAGS &= ~(DRAM_DDR_13_5MHZ);
    } else {
        TCM_DRAM_FLAGS |= DRAM_DDR_13_5MHZ;
    }
    TCM_DRAM_FLAGS &= ~(DRAM_CLOCK_MASK);
    TCM_DRAM_FLAGS |= (((x) / BASE_DDR_CLK) & DRAM_CLOCK_MASK);
    TCMSET_CHECKSUM();
}
#endif /* TCMSET_DDR_CLK */

#endif /* defined(CC_5391_PRELOADER) && defined(NDEBUG) */

#ifndef TCMSET_SYSACON
static void TCMSET_SYSACON(UINT32 x)
{
    if (x) {
        TCM_DRAM_FLAGS |= SYSTEM_AC_ON;
    } else {
        TCM_DRAM_FLAGS &= ~(SYSTEM_AC_ON);
    }
    TCMSET_CHECKSUM();
}
#endif /* TCMSET_SYSACON */

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** IsInitBootVerbose() return assembly verbose flag, only call from assembly.
 *  @retval 1 to verbose message, otherwise quite.
 */
//-----------------------------------------------------------------------------
#ifndef CC_5391_LOADER

UINT32 IsInitBootVerbose()
{
    return CC_INIT_BOOT_VERBOSE;
}

UINT32 IsBootVerbose()
{
    return 1; // DRVCUST_InitGet(eFlagBootVerbose);
}

void BootInit()
{
#if 0
    register UINT32 r, zero;

    r = ((UINT32)g_pu4TIME_Array_Head) | 5; // size 8KB & enable.
    __asm       // ITCM
    {
        MCR p15, 0, r, c9, c1, 1
    }

    g_pu4TIME_Array_Head[0] = 15;

    if (BIM_READ32(REG_RW_REMAP) == 0)
    {
        g_pu4TIME_Array_Head[0] = 1;
        zero = 0;
        for (r=1; r<32; r++)        // 128 bytes data needed.
        {
            g_pu4TIME_Array_Head[r] = zero;
        }
    }
#endif
    BIM_WRITE32(REG_MISC_IRQEN, 0);
    BIM_WRITE32(REG_RW_IRQEN, 0);
    PDWNC_WRITE32(REG_RW_PDWNC_INTEN, 0);

    if (((BIM_READ32(REG_RW_TIMER_CTRL) & (TMR2_CNTDWN_EN)) == 0) ||
        (BIM_READ32(REG_RW_REMAP) == 0))
    {
        BIM_WRITE32(REG_RW_TIMER2_LLMT, 0xffffffff);
        BIM_WRITE32(REG_RW_TIMER2_LOW , 0xffffffff);
        BIM_WRITE32(REG_RW_TIMER2_HLMT, 0xffffffff);
        BIM_WRITE32(REG_RW_TIMER2_HIGH, 0xffffffff);
        BIM_WRITE32(REG_RW_TIMER_CTRL, (TMR2_CNTDWN_EN | TMR2_AUTOLD_EN));
    }
}

void BootBimInitial()
{
    // unlock UART0 output pad to display.
    PDWNC_WRITE32(REG_RW_IOPADEN, PDWNC_READ32(REG_RW_IOPADEN) | PAD_UR0_TX_EN);

    // set bus access timeout to avoid register access halting CPU.
    BIM_WRITE32(REG_RW_TIMEOUT_CTRL, TO_FLASH_CNT | TO_DRAM_CNT | TO_IOBUS_CNT | TO_FLASH_EN | TO_DRAM_EN | TO_IOBUS_EN);

    // Disable and clear interrupt.
    BIM_WRITE32(REG_IRQEN, 0);
    BIM_WRITE32(REG_FIQEN, 0);
    BIM_WRITE32(REG_IRQCL, 0xffffffff);
    BIM_WRITE32(REG_FIQCL, 0xffffffff);
}
#endif /* CC_5391_LOADER */

#define CLK_810MHZ      810000000
#define CLK_650MHZ      650000000
#define CLK_600MHZ      600000000
#define CLK_450MHZ      450000000
#define CLK_400MHZ      400000000
#define CLK_360MHZ      360000000
#define CLK_320MHZ      320000000
#define CLK_300MHZ      300000000
#define CLK_270MHZ      270000000
#define CLK_260MHZ      260000000
#define CLK_200MHZ      200000000
#define CLK_175MHZ      175000000
#define CLK_54MHZ       54000000
#define CLK_27MHZ       27000000
#define CLK_13_5MHZ     13500000

// MT5391
#define REG_RW_DMPLL_SPECTRUM_CFG0  0x04a0
    #define DMPLL_CLKIN_SEL         (1U << 30)  // XXX no this register?
    #define RESET_DMPLL_SPECTRUM    (1U << 29)
#define REG_RW_DMPLL_SPECTRUM_CFG1  0x04a4
    #define DMPLL_SPECTRUM_DIR      (3U << 29)
    #define DMPLL_SPECTRUM_INIT     (1U << 31)
#define REG_RW_DMPLL_SPECTRUM_CFG2  0x04a8
    #define DMPLL_SPECTRUM_CW       (0xfU << 28)
    #define DMPLL_SPECTRUM_PERIOD   (0xfffU << 16)
    #define DMPLL_SPECTRUM_STEP     (0xffffU)

#if defined(CC_5391_PRELOADER) && (defined(NDEBUG) || defined(CC_NAND_BOOT))
#define CLK_DmpllSpectrum(u4Clk)    (u4Clk)
#else /* defined(CC_5391_PRELOADER) && (defined(NDEBUG) || defined(CC_NAND_BOOT)) */

#if (DRAM_SPREAD_SPECTRUM_RANGE==0)
UINT32 CLK_DmpllSpectrum(UINT32 u4Clk)
{
    /* Check if DDS. */
    if (DRVCUST_InitGet(eFlagDmpllSpectrum))
    {
        UINT32 u4Val, u4Permillage;

        /* Set DMSS is 27MHz */
        u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG0);
        u4Val &= ~(DMPLL_CLKIN_SEL); // 0 to spread spectrum clock to DMPLL.
        IO_WRITE32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG0, u4Val);

        /* Reset dmpll spread spectrum digital circuit */
        u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG0);
        u4Val |= (RESET_DMPLL_SPECTRUM);
        IO_WRITE32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG0, u4Val);

        /* Set DMPLL CW word. Only for SYSPLL 324MHz only. XXX fix me if 432 used. */
        u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG2);
        u4Val &= ~(DMPLL_SPECTRUM_CW);
        u4Val |= ((324UL/27) << 28);
        IO_WRITE32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG2, u4Val);

        /* Start doing DMPLL Spectrum. */
        u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG1);
        u4Val |= DMPLL_SPECTRUM_INIT;
        IO_WRITE32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG1, u4Val);

        /* Get Spectrum Permillage. */
        u4Permillage = DRVCUST_InitGet(eDmpllSpectrumPermillage);

        /* Setup Step. */
        u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG2);
        u4Val &= ~(DMPLL_SPECTRUM_STEP);
        // 4*(2^16)*(324/27)*(u4Permillage/1000)*30/(27000)
        // => ((2^18)*(12)*(30*u4Permillage))/(27000*1000)
        // => ((2^20)*(3^2)*10*u4Permillage)/(27000000)
        // => ((2^20)*u4Permillage)/(300000)
        u4Val |= (((u4Permillage << 20)/3000000) & DMPLL_SPECTRUM_STEP);
        IO_WRITE32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG2, u4Val);

        /* Setup Period. */
        u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG2);
        u4Val &= ~(DMPLL_SPECTRUM_PERIOD);
        u4Val |= (((27000/4) / 30) << 16);
        IO_WRITE32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG2, u4Val);

        /* Setup direction. */
        u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG1);
        u4Val &= ~(DMPLL_SPECTRUM_DIR); // direction set to 0 as center.
        IO_WRITE32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG1, u4Val);

        /* Finish Setup Spectrum. */
        u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG1);
        u4Val &= ~(DMPLL_SPECTRUM_INIT);
        IO_WRITE32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG1, u4Val);

        /* Set Dmpll reference clock from Xtal to DMSS, fix me by const. */
        u4Val = CKGEN_READ32(0x164);
        u4Val |= (1U << 6); // enable DMPLL input reference from DMSS.
        CKGEN_WRITE32(0x164, u4Val);
    }
    return u4Clk;
}


#else


UINT32 CLK_DmpllSpectrum(UINT32 u4Clk)
	{
	    /* Check if DDS. */
	        UINT32 u4Val;
//		UINT32 u4Permillage;

//		u4Permillage = 1; //SS ratio: 1/10000 
	        /* Set DMSS is 27.0MHz */
	        u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG0);
	        u4Val &= ~(DMPLL_CLKIN_SEL); // 0 to spread spectrum clock to DMPLL.
	        IO_WRITE32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG0, u4Val);
	
	        /* Reset dmpll spread spectrum digital circuit */
	        u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG0);
	        u4Val |= (RESET_DMPLL_SPECTRUM);
	        /* Set I-gain/P-gain as Max. value */
	        u4Val |= 0x00fff000;
	        IO_WRITE32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG0, u4Val);
	
	        /* Set DMPLL CW word. Only for SYSPLL 432MHz only.*/
	        u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG2);
	        u4Val &= ~(DMPLL_SPECTRUM_CW);
	        //for 27.5mhz dmss output (sysplll:432)
	        //(432/27.5) * 32 = 0x1f6 ==> 0xf for 0x700024a8[31:28], 0xb0 for 0x7000249C[31:27]
	        //(324/27)*32 = 0xc.0
	        u4Val |= 0xcU << 28;	//324?
	        IO_WRITE32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG2, u4Val);
					/* Set DMPLL control word(fractional part)*/
					IO_WRITE32(VIDEO_IN0_BASE, 0x049C, 0x00000000);//for 27.0mhz dmss output (sysplll:324)
					
					
	        /* Start doing DMPLL Spectrum. */
	        u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG1);
	        u4Val |= DMPLL_SPECTRUM_INIT;
	        IO_WRITE32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG1, u4Val);

	        /* Get Spectrum Permillage. */
	        //u4Permillage = DRVCUST_InitGet(eDmpllSpectrumPermillage);
//	    		Printf("Enable Dmpll spectrum (permillage=>%d)\n", DRAM_SPREAD_SPECTRUM_RANGE );
		CHIP_DisplayString("Enable Dmpll spectrum=");
	    	CHIP_DisplayInteger(DRAM_SPREAD_SPECTRUM_RANGE);
		CHIP_DisplayString("\n");

	        /* Setup Step. */
	        u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG2);
	        u4Val &= ~(DMPLL_SPECTRUM_STEP);
	        //an example for SYSPLL 324MHz only.
	        // 4*(2^16)*(324/27)*(u4Permillage/1000)*30/(27000)
	        // => ((2^18)*(12)*(30*u4Permillage))/(27000*1000)
	        // => ((2^20)*(3^2)*10*u4Permillage)/(27000000)
	        // => ((2^20)*u4Permillage)/(300000)
	        //u4Val |= (((u4Permillage << 20)/3000000) & DMPLL_SPECTRUM_STEP);
	        
//the case for syspll 324Mhz
// 4*(2^16)*(324/27.0)*(u4Permillage/10000)*20/(27000)
	        u4Val |= (DRAM_SPREAD_SPECTRUM_RANGE * 233/1000) & DMPLL_SPECTRUM_STEP;//for 27mhz dmss output (sysplll:324) 
	        IO_WRITE32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG2, u4Val);
	//Printf("Set Addr(0x200224A8) as : 0x%x\n", HAL_READ32(0x200224A8) );	        
	        /* Setup Period. */
	        u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG2);
	        u4Val &= ~(DMPLL_SPECTRUM_PERIOD);

	       if(DRAM_SPREAD_SPECTRUM_RANGE)
	        	u4Val |= (((27000/4) / 20) << 16);//for 27.0mhz dmss output (sysplll:324) = 20k
	        		
		IO_WRITE32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG2, u4Val);
		HAL_Delay_us(100);
	        /* Setup direction. */
	        u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG1);
	        u4Val &= ~(DMPLL_SPECTRUM_DIR); // direction set to 0 as center.
	        IO_WRITE32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG1, u4Val);
	
	        /* Finish Setup Spectrum. */
	        u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG1);
	        u4Val &= ~(DMPLL_SPECTRUM_INIT);
	        IO_WRITE32(VIDEO_IN0_BASE, REG_RW_DMPLL_SPECTRUM_CFG1, u4Val);
	
	
	        /* Set Dmpll reference clock from Xtal to DMSS, fix me by const. */
	        u4Val = CKGEN_READ32(0x160);
	        u4Val |= (1U << 20); // enable DMPLL input reference from DMSS.
	        CKGEN_WRITE32(0x160, u4Val);
	        HAL_Delay_us(200);

		return u4Clk;
	}


#endif








#endif /* defined(CC_5391_PRELOADER) && (defined(NDEBUG) || defined(CC_NAND_BOOT)) */

UINT32 CLK_SetDmpll(UINT32 u4Clk, UINT32 fgFullFreq)
{
#if 0
    UINT32 u4Clock, u4Val, u4Cnt;

    // 1. set DMPLL
    u4Clock = (fgFullFreq) ? (u4Clk) : (u4Clk >> 1);
    u4Val = CKGEN_READ32(REG_DMPLLCFG0);    // 0x00650e12
    u4Val &= ~(RG_DMPLL_D_MASK | RG_DMPLL_VCOG_SEL | BANDGAP_BIAS_MASK | RG_DMPLL_DIVI12);
    u4Val |= (RG_DMPLL_ENABLE | RG_DMPLL_ENABLE_FAK | DEF_BANDGAP_BIAS);
#if 0
    u4Val |= (u4Clock > CLK_405MHZ) ? 0 : RG_DMPLL_DIVI12;
    u4Val |= (u4Clock > CLK_405MHZ) ? RG_DMPLL_D((((u4Clock + CLK_27MHZ) - 1)/CLK_27MHZ)) :
                                    RG_DMPLL_D((((u4Clock + CLK_13_5MHZ) - 1)/CLK_13_5MHZ));
#else
    u4Cnt = (((u4Clock + CLK_13_5MHZ) - 1)/CLK_13_5MHZ);
    if (u4Cnt & 1)
    {
        u4Val |= RG_DMPLL_DIVI12;
        u4Val |= RG_DMPLL_D(u4Cnt);
    }
    else
    {
        u4Val |= RG_DMPLL_D((u4Cnt >> 1));
    }
#endif
    u4Val |= (u4Clock > CLK_450MHZ) ? VCOG_300_600MHZ :
            ((u4Clock > CLK_360MHZ) ? VCOG_250_550MHZ :
            ((u4Clock > CLK_260MHZ) ? VCOG_200_400MHZ : VCOG_200_300MHZ));
    CKGEN_WRITE32(REG_DMPLLCFG0, u4Val);

    // 2. set MEMPLL
    u4Clock = (fgFullFreq) ? (u4Clock >> 1) : (u4Clock);
    u4Val = CKGEN_READ32(REG_MEMPLLCFG0);   // 0x2a5069f0
    u4Val &= ~(RG_MEMPLL_DIV2 | RG_MEMPLL_BAND_MASK | RG_MEMPLL_PD | RG_MEMPLL_DIV2);
    u4Val |= (fgFullFreq) ? 0 : RG_MEMPLL_DIV2;
    u4Val |= (u4Clock > CLK_300MHZ) ? RG_MEMPLL_BAND_LARGE :
            ((u4Clock > CLK_175MHZ) ? RG_MEMPLL_BAND_MEDIUM : RG_MEMPLL_BAND_SMALL);
    CKGEN_WRITE32(REG_MEMPLLCFG0, u4Val);
    CKGEN_WRITE32(REG_MEMPLLCFG1, RG_MEMPLL_LDO_12V);

    // 3. Disable test mode
    CKGEN_WRITE32(REG_MEM_CKCFG, NORMAL_MEM_CFG);
    CKGEN_WRITE32(REG_XTALCFG, XTAL_NORMAL_MODE);

    u4Clk = CLK_DmpllSpectrum(u4Clk);

    return u4Clk;
#else
	#ifdef CC_MT5392B
    UINT32 i;
	#endif /* CC_MT5392B */
    UINT32 u4Val;

    // Disable dram clock test mode.
    CKGEN_WRITE32(0x20C, 0);

    //Set DMPLL
    u4Val = 0xF2030000;
    u4Val |= ((((((u4Clk >> 1) + CLK_27MHZ) - 1)/CLK_27MHZ) - 2) << 8);
    CKGEN_WRITE32(0x110, u4Val);
    //CKGEN_WRITE32(0x110, 0xF2030A00); //324mhz (x2=648mhz)
    //CKGEN_WRITE32(0x110, 0xF2031100); //513mhz (x2=1026mhz)
    //CKGEN_WRITE32(0x110, 0xF2031400); //594mhz (x2=1188mhz)
    CKGEN_WRITE32(0x114, 0xC0);
    CKGEN_WRITE32(0x114, 0x40);
    CHIP_Delay_us(1);
    CKGEN_WRITE32(0x114, 0x41);

    // Add MEMPLL Bandgap toggle protection
    CKGEN_WRITE32(0x124, 0x00013000 | (1U << 19));
    CKGEN_WRITE32(0x124, 0x00013000);

    // Set MEMPLL
    CKGEN_WRITE32(0x120, 0xF0030060);
    CKGEN_WRITE32(0x124, 0x00011300);//0x00011000

    CKGEN_WRITE32(0x128, 0x00000000);
    CHIP_Delay_us(500);
    CKGEN_WRITE32(0x128, 0x00000001);
    CHIP_Delay_us(500);//50

	#ifdef CC_MT5392B

		#if 0
    // Wait MEMPLL stable, then check Zero phase calibration
    i = 0;
    do {
        u4Val = CKGEN_READ32(0x124);
        u4Val = u4Val & (~(0xf0000000U));
        u4Val |= (i << 28);
        CKGEN_WRITE32(0x124, u4Val);
        CHIP_Delay_us(50);
    
        CKGEN_WRITE32(0x128, 0x00000000);
        CHIP_Delay_us(50);
        CKGEN_WRITE32(0x128, 0x00000001);
        CHIP_Delay_us(50);
    
        if ((CKGEN_READ32(0x1bc) & (1U << 28)) != 0)
        {
            Printf("1: Zero Phase:%d 0x%08x\n", i, (CKGEN_READ32(0x1bc) & (1U << 28)));
            break;
        }
        else
        {
            Printf("0: Zero Phase:%d 0x%08x\n", i, (CKGEN_READ32(0x1bc) & (1U << 28)));
        }
        i++;
    } while (i <= 0x7);

		#else
	
    u4Val = CKGEN_READ32(0x124);
    u4Val = u4Val & (~(0xf0000000U));
    CKGEN_WRITE32(0x124, u4Val);
    CHIP_Delay_us(50);

    if ((CKGEN_READ32(0x1bc) & (1U << 28)) != 0)
	{
	
    	i = 8;
	    do {
    	    u4Val = CKGEN_READ32(0x124);
        	u4Val = u4Val & (~(0xf0000000U));
	        u4Val |= (i << 28);
    	    CKGEN_WRITE32(0x124, u4Val);
        	CHIP_Delay_us(50);
    
    	    if ((CKGEN_READ32(0x1bc) & (1U << 28)) != 0)
        	{
            	Printf("1: Zero Phase:%d 0x%08x\n", i, (CKGEN_READ32(0x1bc) & (1U << 28)));
    	    }
        	else
	        {
    	        Printf("0: Zero Phase:%d 0x%08x\n", i, (CKGEN_READ32(0x1bc) & (1U << 28)));
    	        break;

        	}
	        i++;
    	} while (i <= 0xf);
	}
	else
	{
    	i = 0;
	    do {
    	    u4Val = CKGEN_READ32(0x124);
        	u4Val = u4Val & (~(0xf0000000U));
	        u4Val |= (i << 28);
    	    CKGEN_WRITE32(0x124, u4Val);
        	CHIP_Delay_us(50);
    
    	    if ((CKGEN_READ32(0x1bc) & (1U << 28)) != 0)
        	{
            	Printf("1: Zero Phase:%d 0x%08x\n", i, (CKGEN_READ32(0x1bc) & (1U << 28)));
    	        break;
    	    }
        	else
	        {
    	        Printf("0: Zero Phase:%d 0x%08x\n", i, (CKGEN_READ32(0x1bc) & (1U << 28)));
        	}
	        i++;
    	} while (i <= 0x7);
	}

		#endif
    
	#else/* CC_MT5392B */
    u4Val = CKGEN_READ32(0x124);
    u4Val = u4Val & (~(0xf0000000U));

		#if (DEFAULT_DRAM_TYPE == DDR_III_x1)
    u4Val = u4Val | 0xf0000000U;
		#else
    u4Val = u4Val | 0x70000000U;
		#endif

    Printf("0xd124 = %x",u4Val);
    CKGEN_WRITE32(0x124, u4Val);
    CHIP_Delay_us(50);
    CKGEN_WRITE32(0x128, 0x00000000);
    CHIP_Delay_us(50);
    CKGEN_WRITE32(0x128, 0x00000001);
    CHIP_Delay_us(50);
	#endif /* CC_MT5392B */

    u4Clk = CLK_DmpllSpectrum(u4Clk);

#if CC_USE_ANALOG_DLL
    DDR_UseAnalogDll();
#endif

    return u4Clk;
#endif
}

#if defined(CC_5391_PRELOADER) && defined(NDEBUG)
#else /* defined(CC_5391_PRELOADER) && defined(NDEBUG) */
UINT32 CLK_RingOscillatorTick()
{
    // Ring oscillator clock
    BIM_WRITE32(REG_RW_ROSCCTL, 0);
    BIM_WRITE32(REG_RW_ROSCCTL, ROSC_CLR);
    BIM_WRITE32(REG_RW_ROSCCTL, ROSC_SEL);
    BIM_WRITE32(REG_RW_ROSCCTL, (ROSC_SEL | ROSC_TRIGGER));
    while (BIM_READ32(REG_RW_ROSCCTL) & ROSC_TRIGGER) { }
    return BIM_READ32(REG_RO_ROSCCNT);
}

#ifndef NDEBUG
CHAR * DDR_DramTypeString(UINT32 u4Type)
{
    switch(u4Type)
    {
    case DDR_II_x1:
        return "DDR-IIx1";
    case DDR_II_x2:
        return "DDR-IIx2";
    case DDR_III_x1:
        return "DDR-IIIx1";
    case DDR_III_x2:
        return "DDR-IIIx2";
    default:
        return "unknown";
    }
}

static void _CLK_DisplayInfo()
{
    Printf("1 DDR Clock:%d Hz ('[' or ']' to change)\n", TCMGET_DDR_CLK());
    Printf("1 DramType: %s ('T' to change)\n", DDR_DramTypeString(TCMGET_DRAMTYPE()));
#ifndef CC_MT5391
    Printf("1 Full Freq: %s ('F' to change)\n", (IS_FULLFREQ() ? "TRUE" : "FALSE"));
#endif
    Printf("1 Force 32 bit: %s ('3' to change)\n", (IS_FORCE32() ? "TRUE" : "FALSE"));
    Printf("1 Column address bit is %d ('C' to change)\n", (TCMGET_COLADDR()));
#ifdef CC_DRAM_DEBUG
    Printf("2 DQSDRVP: 0x%01X ('Q': to change)\n", (DRAM_READ32(0xB8) >> 28) & 0xF);
    Printf("2 DQSDRVN: 0x%01X ('W': to change)\n", (DRAM_READ32(0xB8) >> 24) & 0xF);
    Printf("2 DQDRVP: 0x%01X ('E': to change)\n", (DRAM_READ32(0xB8) >> 12) & 0xF);
    Printf("2 DQDRVN: 0x%01X ('R': to change)\n", (DRAM_READ32(0xB8) >> 8) & 0xF);

    Printf("2 CLKDRVP: 0x%01X ('A': to change)\n", (DRAM_READ32(0xBC) >> 28) & 0xF);
    Printf("2 CLKDRVN: 0x%01X ('S': to change)\n", (DRAM_READ32(0xBC) >> 24) & 0xF);
    Printf("2 CMDRVP: 0x%01X ('Z': to change)\n", (DRAM_READ32(0xBC) >> 12) & 0xF);
    Printf("2 CMDRVN: 0x%01X ('X': to change)\n", (DRAM_READ32(0xBC) >> 8) & 0xF);

    Printf("2 DSODTP: 0x%01X ('G': to change)\n", (DRAM_READ32(0xB8) >> 20) & 0x7);
    Printf("2 DSODTN: 0x%01X ('H': to change)\n", (DRAM_READ32(0xB8) >> 16) & 0x7);
    Printf("2 DQODTP: 0x%01X ('J': to change)\n", (DRAM_READ32(0xB8) >> 4) & 0x7);
    Printf("2 DQODTN: 0x%01X ('K': to change)\n", (DRAM_READ32(0xB8) >> 0) & 0x7);

    Printf("2 AUTOKMODE: 0x%01X ('Y': to change)\n", (DRAM_READ32(0xC0) >> 20) & 0x3);

    Printf("2 EMRS_H: 0x%01X ('U': to change)\n", (DRAM_READ32(0x08) >> 20) & 0xF);
    Printf("2 EMRS_L: 0x%01X ('I': to change)\n", (DRAM_READ32(0x08) >> 16) & 0xF);
    Printf("2 MRS_H: 0x%01X ('O': to change)\n", (DRAM_READ32(0x08) >> 12) & 0xF);
    Printf("2 MRS_L: 0x%01X ('P': to change)\n", (DRAM_READ32(0x08) >> 8) & 0xF);

    Printf("2 Rtt: 0x%01X ('V': to change)\n", (DRAM_READ32(0x08) >> 18) & 0x3);

    Printf("2 CMDDLY: 0x%01X ('D': to change)\n", (DRAM_READ32(0x0C) >> 28) & 0x0F);
    Printf("2 CLKDLY: 0x%01X ('L': to change)\n", (DRAM_READ32(0x0C) >> 24) & 0x0F);
    Printf("2 DQDLY: 0x%01X ('M': to change)\n", DRAM_READ32(0x10) & 0x0F );
    Printf("2 DQSDLY: 0x%01X ('N': to change)\n", DRAM_READ32(0x14) & 0x0F);

    Printf("2 RKMODE:0x7110[6:0]= 0x%02X ('4': to change)\n", DRAM_READ32(0x110) & 0x7F);
    Printf("2 dual-rank EMRS[8] = 0x%02X ('5': to change, 0:SS 1:qimonda)\n", (DRAM_READ32(0x80)>>24) & 0x01);
    Printf("2 OCD EMRS2[11:9] = 0x%02X ('6': to change)\n", (DRAM_READ32(0x88)>>9) & 0x07);
#endif //CC_DRAM_DEBUG
}

static void _CLK_DisplayMenu()
{
    Printf("\n-------------------------------------------------------\n");
    _CLK_DisplayInfo();
    Printf("Press 'b' to download\n");
    Printf("Press '0' to exit:\n\n");
}
#endif /* NDEBUG */
#endif /* defined(CC_5391_PRELOADER) && defined(NDEBUG) */

static void _CLKResetTCM(UINT32 u4DefClk)
{
#if defined(CC_5391_PRELOADER) && defined(NDEBUG)
#else /* defined(CC_5391_PRELOADER) && defined(NDEBUG) */
    UINT32 u4Ret;

    // if TCM is not configured, reset TCM. Here is AC-on.
    TCM_DRAM_FLAGS = 0;
    TCM_DRAM_TYPE = 0;
    TCM_WAKEUP_STATUS = 0;
    TCM_CHECKSUM = 0;

    TCMSET_DDR_CLK(u4DefClk);

    u4Ret = DRVCUST_InitGet(eDramType);
    TCMSET_DRAMTYPE(u4Ret);

    u4Ret = DRVCUST_InitGet(eFlagForce32bit);
    TCMSET_FORCE32(u4Ret);

    u4Ret = DRVCUST_InitGet(eFlagFullFreq);
    TCMSET_FULLFREQ(u4Ret);

    u4Ret = DRVCUST_InitGet(eDramColAddr);
    u4Ret = (u4Ret >= COL_ADDR_MAX) ? COL_ADDR_BIT_10 : u4Ret;
    TCMSET_COLADDR(u4Ret);
#endif /* defined(CC_5391_PRELOADER) && defined(NDEBUG) */

    TCMSET_SYSACON(1);
}


//-----------------------------------------------------------------------------
// Public functions.
//-----------------------------------------------------------------------------

#define KEY_ESCAPE                      0x1b


#if defined(CC_5391_PRELOADER) && defined(NDEBUG)
#define CLK_QueryDramSetting(x)
#else /* defined(CC_5391_PRELOADER) && defined(NDEBUG) */
#ifdef NDEBUG
#define CLK_QueryDramSetting(x)
#else /* NDEBUG */
#ifdef __MODEL_slt__
#define CLK_QueryDramSetting(x)
#else /* __MODEL_slt__ */
//-----------------------------------------------------------------------------
/* CLK_QueryDramSetting() for user to modify TCM_DRAM_SETUP value of the DRAM
 *      controller parameters.
 */
//-----------------------------------------------------------------------------
void CLK_QueryDramSetting(UINT8 *szString)
{
    UINT32 i, u4CpuClk, u4Val;
    UCHAR uc;

#if 0
    // no verbose to return
    if (!IsBootVerbose())
    {
        return;
    }
#endif

    u4CpuClk = CLK_200MHZ;
    // Wait a moment for ESC to enter setup menu
    Printf("\n%s\n", szString);
    SerTransparent();
    uc = 0;
    for (i=0; i<(u4CpuClk >> 15); i++)
    {
        Printf("\r");
        switch(i & 0x3) {
        case 0:
            Printf("|"); break;
        case 1:
            Printf("/"); break;
        case 2:
            Printf("-"); break;
        case 3:
            Printf("\\"); break;
        default:
            continue;
        }

        if (SerInByte(&uc))
        {
            if (uc == KEY_ESCAPE)
            {
                break;
            }
        }
    }

    // Without ESC, just return.
    if (uc != KEY_ESCAPE)
    {
        SerNormalMode();
        Printf("\n");
        return;
    }

    _CLK_DisplayMenu();

    // Otherwise, enter menu loop.
    while (1)
    {
        // wait a key.
        while (!SerInByte(&uc)) { }

        switch(uc)
        {
        case '+':
        case ']':
            ADD_DDR_CLK();
            break;
        case '-':
        case '[':
            SUB_DDR_CLK();
            break;
        case 'B': case 'b':
            SerNormalMode();
            Printf("RS232 leave transparent mode! Console stop!\n");
            while (1) { }
        case 'C': case 'c':
            u4Val = TCMGET_COLADDR()+1;
            if (u4Val >= COL_ADDR_MAX)
            {
                u4Val = (COL_ADDR_MIN+1);
            }
            TCMSET_COLADDR(u4Val);
            break;
        case 'T': case 't':
            u4Val = TCMGET_DRAMTYPE();
            u4Val++;
            if (u4Val >= DDR_MAX_VAL)
            {
                u4Val = (DDR_MIN_VAL)+1;
            }
            TCMSET_DRAMTYPE(u4Val);
            break;
        case '0': // to Exit.
            break;
        case 'F': case 'f':
            TCMSET_FULLFREQ(!IS_FULLFREQ());
            break;
        case '3':
            TCMSET_FORCE32(!IS_FORCE32());
            break;
#ifdef CC_DRAM_DEBUG
        case 'Q': case 'q':
            u4Val = (DRAM_READ32(0xB8) >> 28) & 0xF;
            u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
            u4Val = u4Val << 28;
            u4Val = (DRAM_READ32(0xB8) & 0x0FFFFFFF) | u4Val;
            DRAM_WRITE32(0xB8, u4Val);
            break;
        case 'W': case 'w':
            u4Val = (DRAM_READ32(0xB8) >> 24) & 0xF;
            u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
            u4Val = u4Val << 24;
            u4Val = (DRAM_READ32(0xB8) & 0xF0FFFFFF) | u4Val;
            DRAM_WRITE32(0xB8, u4Val);
            break;
        case 'E': case 'e':
            u4Val = (DRAM_READ32(0xB8) >> 12) & 0xF;
            u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
            u4Val = u4Val << 12;
            u4Val = (DRAM_READ32(0xB8) & 0xFFFF0FFF) | u4Val;
            DRAM_WRITE32(0xB8, u4Val);
            break;
        case 'R': case 'r':
            u4Val = (DRAM_READ32(0xB8) >> 8) & 0xF;
            u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
            u4Val = u4Val << 8;
            u4Val = (DRAM_READ32(0xB8) & 0xFFFFF0FF) | u4Val;
            DRAM_WRITE32(0xB8, u4Val);
            break;

        case 'A': case 'a':
            u4Val = (DRAM_READ32(0xBC) >> 28) & 0xF;
            u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
            u4Val = u4Val << 28;
            u4Val = (DRAM_READ32(0xBC) & 0x0FFFFFFF) | u4Val;
            DRAM_WRITE32(0xBC, u4Val);
            break;
        case 'S': case 's':
            u4Val = (DRAM_READ32(0xBC) >> 24) & 0xF;
            u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
            u4Val = u4Val << 24;
            u4Val = (DRAM_READ32(0xBC) & 0xF0FFFFFF) | u4Val;
            DRAM_WRITE32(0xBC, u4Val);
            break;
        case 'Z': case 'z':
            u4Val = (DRAM_READ32(0xBC) >> 12) & 0xF;
            u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
            u4Val = u4Val << 12;
            u4Val = (DRAM_READ32(0xBC) & 0xFFFF0FFF) | u4Val;
            DRAM_WRITE32(0xBC, u4Val);
            break;
        case 'X': case 'x':
            u4Val = (DRAM_READ32(0xBC) >> 8) & 0xF;
            u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
            u4Val = u4Val << 8;
            u4Val = (DRAM_READ32(0xBC) & 0xFFFFF0FF) | u4Val;
            DRAM_WRITE32(0xBC, u4Val);
            break;

        case 'G': case 'g':
            u4Val = (DRAM_READ32(0xB8) >> 20) & 0x7;
            u4Val = (u4Val == 0x7) ? 0 : (u4Val + 1);
            u4Val = u4Val << 20;
            u4Val = (DRAM_READ32(0xB8) & 0xFF8FFFFF) | u4Val;
            DRAM_WRITE32(0xB8, u4Val);
            break;
        case 'H': case 'h':
            u4Val = (DRAM_READ32(0xB8) >> 16) & 0x7;
            u4Val = (u4Val == 0x7) ? 0 : (u4Val + 1);
            u4Val = u4Val << 16;
            u4Val = (DRAM_READ32(0xB8) & 0xFFF8FFFF) | u4Val;
            DRAM_WRITE32(0xB8, u4Val);
            break;
        case 'J': case 'j':
            u4Val = (DRAM_READ32(0xB8) >> 4) & 0x7;
            u4Val = (u4Val == 0x7) ? 0 : (u4Val + 1);
            u4Val = u4Val << 4;
            u4Val = (DRAM_READ32(0xB8) & 0xFFFFFF8F) | u4Val;
            DRAM_WRITE32(0xB8, u4Val);
            break;
        case 'K': case 'k':
            u4Val = (DRAM_READ32(0xB8) >> 0) & 0x7;
            u4Val = (u4Val == 0x7) ? 0 : (u4Val + 1);
            u4Val = u4Val << 0;
            u4Val = (DRAM_READ32(0xB8) & 0xFFFFFFF8) | u4Val;
            DRAM_WRITE32(0xB8, u4Val);
            break;

        case 'Y': case 'y':
            u4Val = (DRAM_READ32(0xC0) >> 20) & 0x3;
            u4Val = (u4Val == 0x3) ? 0 : (u4Val + 1);
            u4Val = u4Val << 20;
            u4Val = (DRAM_READ32(0xC0) & 0xFFCFFFFF) | u4Val;
            DRAM_WRITE32(0xC0, u4Val);
            break;

        case 'U': case 'u':
            u4Val = (DRAM_READ32(0x08) >> 20) & 0xF;
            u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
            u4Val = u4Val << 20;
            u4Val = (DRAM_READ32(0x08) & 0xFF0FFFFF) | u4Val;
            DRAM_WRITE32(0x08, u4Val);
            break;
        case 'I': case 'i':
            u4Val = (DRAM_READ32(0x08) >> 16) & 0xF;
            u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
            u4Val = u4Val << 16;
            u4Val = (DRAM_READ32(0x08) & 0xFFF0FFFF) | u4Val;
            DRAM_WRITE32(0x08, u4Val);
            break;
        case 'O': case 'o':
            u4Val = (DRAM_READ32(0x08) >> 12) & 0xF;
            u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
            u4Val = u4Val << 12;
            u4Val = (DRAM_READ32(0x08) & 0xFFFF0FFF) | u4Val;
            DRAM_WRITE32(0x08, u4Val);
            break;
        case 'P': case 'p':
            u4Val = (DRAM_READ32(0x08) >> 8) & 0xF;
            u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
            u4Val = u4Val << 8;
            u4Val = (DRAM_READ32(0x08) & 0xFFFFF0FF) | u4Val;
            DRAM_WRITE32(0x08, u4Val);
            break;

case 'V': case 'v':
            u4Val = (DRAM_READ32(0x08) >> 18) & 0x3;
            u4Val = (u4Val == 0x3) ? 0 : (u4Val + 1);
            u4Val = u4Val << 18;
            u4Val = (DRAM_READ32(0x08) & 0xFFF3FFFF) | u4Val;
            DRAM_WRITE32(0x08, u4Val);
            break;


case 'D': case 'd':
            u4Val = (DRAM_READ32(0x0C) >> 28) & 0x0F;
            u4Val = (u4Val == 0x0F) ? 0 : (u4Val + 1);
            u4Val = u4Val << 28;
            u4Val = (DRAM_READ32(0x0C) & 0x0FFFFFFF) | u4Val;
            DRAM_WRITE32(0x0C, u4Val);
            break;
case 'L': case 'l':
            u4Val = (DRAM_READ32(0x0C) >> 24) & 0x0F;
            u4Val = (u4Val == 0x0F) ? 0 : (u4Val + 1);
            u4Val = u4Val << 24;
            u4Val = (DRAM_READ32(0x0C) & 0xF0FFFFFF) | u4Val;
            DRAM_WRITE32(0x0C, u4Val);
            break;
case 'M': case 'm':
            u4Val = DRAM_READ32(0x10);
            u4Val = (u4Val == 0xFFFFFFFF) ? 0 : (u4Val + 0x11111111);
            DRAM_WRITE32(0x10, u4Val);
            break;
case 'N': case 'n':
            u4Val = DRAM_READ32(0x14);
            u4Val = (u4Val == 0xFFFFFFFF) ? 0 : (u4Val + 0x11111111);
            DRAM_WRITE32(0x14, u4Val);
            break;
case '4':
            u4Val = DRAM_READ32(0x110) & 0x7f;
            u4Val = (u4Val == 0x7f) ? 0 : (u4Val + 1);
            u4Val = (DRAM_READ32(0x110) & 0xFFFFFF80) | u4Val;
            DRAM_WRITE32(0x110, u4Val);
            break;
case '5':
            u4Val = DRAM_READ32(0x80) ^ 0x01000000;           
            DRAM_WRITE32(0x80, u4Val);
            break;
case '6':
	     u4Val = (DRAM_READ32(0x88) >> 9) & 0x07;
            u4Val = (u4Val == 0x07) ? 0 : (u4Val + 1);
            u4Val = u4Val << 9;
            u4Val = (DRAM_READ32(0x88) & 0xFFFFF1FF) | u4Val;
            DRAM_WRITE32(0x88, u4Val);
            break;
            
#endif //CC_DRAM_DEBUG
        default:
            Printf("\r%c\n", uc);
            continue;
        }

        if (uc == '0')
        {
            break;
        }

        _CLK_DisplayMenu();
    }

    SerNormalMode();
    return;
}
#endif /* __MODEL_slt__ */
#endif /* NDEBUG */
#endif /* defined(CC_5391_PRELOADER) && defined(NDEBUG) */

#if (defined(CC_NAND_BOOT) || defined(CC_ROM_BOOT)) && !defined(CC_FPGA)
//#define CLK_SetDdrPriority()
#define CLK_SdrCalibrate()
#else /* CC_NAND_BOOT || CC_ROM_BOOT */

//-----------------------------------------------------------------------------
/* CLK_SdrCalibrate() for SDR calibration
 *
 */
//-----------------------------------------------------------------------------
UINT32 CLK_SdrCalibrate()
{

    int i = 0;

    //The following code is suggested by HI
    DRAM_WRITE32(0x0, 0x22661154);
    DRAM_WRITE32(0x8, 0x00002100);  // REG2[15:8] = 0x21, for shorter burst
    DRAM_WRITE32(0xC, 0x0);
    DRAM_WRITE32(0x4, 0x00010000);  // Trigger auto initialization

    i = 5;
    while(i == 0)                   // How long should we wait?
        i--;

    DRAM_WRITE32(0x4, SDR_32_TURBO);
    DRAM_WRITE32(0x8, 0x00002105);// REG2[15:8] = 0x21, for shorter burst
    DRAM_WRITE32(0x10, 0x00001000);

    return 0;
}
#endif /* CC_NAND_BOOT || CC_ROM_BOOT */

UINT32 CLK_SetDdrOnNorFlash(UINT32 u4DefClk)
{
    UINT32 u4Clock, u4Ret;
    UINT32 u4Val;
    
    u4Val = PDWNC_READ32(REG_RW_UP_CFG) & ~ENGINE_EN;
    PDWNC_WRITE32(REG_RW_UP_CFG, u4Val);
    
    //CLK_SetDdrPriority(); // move priority settings to loader

    // TCMSET_SYSACON(0);
    if (TCM_DRAM_FLAGS & SYSTEM_AC_ON)
    {
        // CHIP_DisplayString("\nAC ON flag disable\n");
        TCM_DRAM_FLAGS &= ~(SYSTEM_AC_ON);  // no call TCMSET_SYSACON, because it will set checksum.
        TCM_CHECKSUM -= SYSTEM_AC_ON;
    }

    // Check TCM magic string.
    if (!IS_TCM_CHECKSUM_OK())
    {
        // CHIP_DisplayString("TCM Checksum bad\n");
        // CHIP_DisplayString("AC ON flag enable\n");
        _CLKResetTCM(u4DefClk);
    }
    else
    {
        // CHIP_DisplayString("TCM Checksum ok\n");
    }

    // Check TCM DMPLL configuration.
    u4Clock = TCMGET_DDR_CLK();
    if ((u4Clock > CLK_810MHZ) || (u4Clock < CLK_54MHZ))
    {
        _CLKResetTCM(u4DefClk);
    }

    // Check TCM Dram type configuration.
    u4Ret = TCMGET_DRAMTYPE();
    if ((u4Ret >= DDR_MAX_VAL) || (u4Ret <= DDR_MIN_VAL))
    {
        _CLKResetTCM(u4DefClk);
    }

    // Check TCM Dram column addr
    u4Ret = TCMGET_COLADDR();
    if ((u4Ret >= COL_ADDR_MAX) || (u4Ret <= COL_ADDR_MIN))
    {
        _CLKResetTCM(u4DefClk);
    }

    CHIP_DisplayString("\n");

    // User's chance to modify the default settings at TCM_DRAM_SETUP.
    CLK_QueryDramSetting((UINT8 *)"First setup dram menu:");

    // Get the DRAM settings.
    u4Clock = CLK_SetDmpll(TCMGET_DDR_CLK(), IS_FULLFREQ());
    // u4Clock is the final clock setting.
    Printf("\nDMPLL is set as %d.%d MHz\n", u4Clock / 1000000,
                (u4Clock / 100000) % 10);

#if 0
    fgDDR = TCMGET_DRAMTYPE();
    switch(fgDDR) {
    case DDR_III_x1:
        fgDDR2 = FALSE;
        u4Bits = (1 << (DDR_III_x1 - fgDDR));
        Printf("The memory is DDR-III %d Chips\n", u4Bits);
        break;
    case DDR_II_x2:
    case DDR_II_x1:
        fgDDR2 = TRUE;
        u4Bits = (1 << (DDR_II_x1 - fgDDR));
        Printf("The memory is DDR-II %d Chips\n", u4Bits);
        break;
    case DDR_I_x2:
    case DDR_I_x1:
        fgDDR2 = FALSE;
        u4Bits = (1 << (DDR_I_x1 - fgDDR));
        Printf("The memory is DDR-I %d Chips\n", u4Bits);
        break;
    default:
        Printf("The memory type is wrong.\n");
        return 1;
    }

    fgFullFreq = IS_FULLFREQ();
    fg32Bit = IS_FORCE32();
    DDR_SetDramController((u4Clock >> 1), fgFullFreq, fgDDR2, fg32Bit, 1,
                          u4Bits);

    // Do DQS calibration to get DQS window average value.
    u4Ret = DDR_CalibrateDqs(u4Bits, (!fgDDR2) && (IS_SINGLEDQS()));

    if (u4Ret) {
        CHIP_DisplayString("DDR_DQSCalibrate failed. get result:");
        CHIP_DisplayInteger((INT32)u4Ret);
        CHIP_DisplayString("\n");
    }

    // Detect 64/32/16 bits and set turbo mode.
    u4Bits = DDR_ResetBits((u4Clock >> 1), fgDDR, fgFullFreq, fg32Bit);

    TCMSET_CHECKSUM();
    if (u4Bits) {
        CHIP_DisplayString("DRAM is set as ");
        CHIP_DisplayInteger((INT32)u4Bits);
        CHIP_DisplayString(" bits\n");
    } else {
        CHIP_DisplayString("DRAM Set failed\n");
        return 1;
    }
#endif

    return 0;
}

UINT32 CLK_SetDdr()
{
#if (!defined(CC_NAND_BOOT) && !defined(CC_ROM_BOOT)) || defined(CC_FPGA)
    if( BSP_IsFPGA() )
        return CLK_SdrCalibrate();
    else
#endif /* !CC_NAND_BOOT || !CC_ROM_BOOT */
        return CLK_SetDdrOnNorFlash(DRVCUST_InitGet(eDdrClock));
}

#ifndef CC_5391_PRELOADER
UINT32 BSP_IsSystemAcOn()
{
    return IS_SYS_ACON();
}
#endif /* CC_5391_PRELOADER */

//-----------------------------------------------------------------------------
// Below is cut from board.c
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** BSP_IsFPGA() Check if current platform if FPGA or not
 *  @reval TRUE: FPGA
 *  @reval FALSE: IC
 */
//-----------------------------------------------------------------------------
#if !defined(CC_NAND_BOOT) && !defined(CC_ROM_BOOT)
BOOL BSP_IsFPGA(void)
{
    UINT32 u4Val;

    /* If there is FPGA ID, it must be FPGA, too. */
    u4Val = BIM_READ32(REG_RO_FPGA_ID);
    if (u4Val != 0) { return 1; }

    /* otherwise, it's not FPGA. */
    return 0;
}
#else
BOOL BSP_IsFPGA(void)
{
    return 0;
}
#endif /* !CC_NAND_BOOT && !CC_ROM_BOOT */

//-----------------------------------------------------------------------------
/** BSP_GetIcVersion() Get IC version
 *  @return The IC version, or IC_VER_FPGA if running in a FPGA platform, or
 *      IC_VER_UNKOWN for unknown IC version
 */
//-----------------------------------------------------------------------------
IC_VERSION_T BSP_GetIcVersion(void)
{
#if !defined(CC_NAND_BOOT) && !defined(CC_ROM_BOOT)
    IC_VERSION_T eVer = IC_VER_FPGA;

    if (!BSP_IsFPGA())
    {
#endif /* !CC_NAND_BOOT && !CC_ROM_BOOT */
        UINT32 u4Version;

#ifdef CC_MT5392B
        u4Version = BIM_READ32(REG_RO_SW_ID);
#else /* CC_MT5392B */
        // u4Version = *(UINT32*)REG_IC_VERSION;
        u4Version = BIM_READ32(REG_RO_CHIP_ID);
        u4Version = ((u4Version & 0x0fffff00U) << 4) |
                    ((u4Version & 0x000000ffU) << 0) |
                    ((u4Version & 0xf0000000U) >> 20);
#endif /* CC_MT5392B */

        return (IC_VERSION_T)u4Version;
#if !defined(CC_NAND_BOOT) && !defined(CC_ROM_BOOT)
    }

    return eVer;
#endif /* !CC_NAND_BOOT && !CC_ROM_BOOT */
}


#if !defined (CC_NAND_BOOT) || !defined(CC_5391_PRELOADER)

#ifndef CC_5391_LOADER
//-----------------------------------------------------------------------------
/** BSP_GetMemSize()
 *  @return the total dram size.
 */
//-----------------------------------------------------------------------------
UINT32 BSP_GetMemSize()
{
#ifdef CC_5391_LOADER
    UINT32 g_u4DramSize;
#endif
#ifdef FAKE_MEM_SIZE
    return (g_u4DramSize > FAKE_MEM_SIZE) ? FAKE_MEM_SIZE : g_u4DramSize;
#else
    return g_u4DramSize;
#endif
}
#endif /* !CC_5391_LOADER */

#ifndef __KERNEL__
//lint -save
//lint --e{550} Symbol not accessed
//lint --e{529} Symbol not subsequently referenced

//-----------------------------------------------------------------------------
// Below is cut from hal_926.c
//-----------------------------------------------------------------------------

#pragma arm

//-----------------------------------------------------------------------------
/** HalInvalidateICache() Invalidate entire I-cache
 */
//-----------------------------------------------------------------------------
void HalInvalidateICache(void)
{
    UINT32 r = 0;

    __asm
    {
        MCR     p15, 0, r, c7, c5, 0
    }
}


//-----------------------------------------------------------------------------
/** HalEnableICache() Enable I-cache
 */
//-----------------------------------------------------------------------------
void HalEnableICache(void)
{
    UINT32 r;

    HalInvalidateICache();

    __asm
    {
        MRC     p15, 0, r, c1, c0, 0
        ORR     r, r, #ICACHE_ENABLE
        MCR     p15, 0, r, c1, c0, 0
        NOP
        NOP
        NOP
        NOP
        NOP
    }
}


#if defined(CC_5391_PRELOADER) && defined(NDEBUG)
#else /* defined(CC_5391_PRELOADER) && defined(NDEBUG) */
//-----------------------------------------------------------------------------
// Below is cut from tcm.c at nor_cfg
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** HalSetInstTCMReg() Setup ITCM into some addres
 *  @param u4BaseAddr The address to map ITCM. align 4KB.
 *  @param u4Size This ITCM size setup, 4 is 8KB.
 *  @param fgEnable enable/disable ITCM mapping.
 */
//-----------------------------------------------------------------------------
void HalSetInstTCMReg(UINT32 u4BaseAddr, UINT32 u4Size, BOOL fgEnable)
{
    UINT32 r;

    r = (u4BaseAddr & 0xfffff000) | (u4Size & 0x3c) | (fgEnable ? 1 : 0);

    __asm
    {
        MCR p15, 0, r, c9, c1, 1
    }
}

//-----------------------------------------------------------------------------
/** HalSetDataTCMReg() Setup DTCM into some addres
 *  @param u4BaseAddr The address to map DTCM. align 4KB.
 *  @param u4Size This DTCM size setup, 4 is 8KB.
 *  @param fgEnable enable/disable DTCM mapping.
 */
//-----------------------------------------------------------------------------
void HalSetDataTCMReg(UINT32 u4BaseAddr, UINT32 u4Size, BOOL fgEnable)
{
    UINT32 r;

    r = (u4BaseAddr & 0xfffff000) | (u4Size & 0x3c) | (fgEnable ? 1 : 0);

    __asm
    {
        MCR p15, 0, r, c9, c1, 0
    }
}

//lint -restore
#endif /* defined(CC_5391_PRELOADER) && defined(NDEBUG) */

#endif /* __KERNEL__ */
#endif /* CC_NAND_BOOT */
