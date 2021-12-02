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

#include "x_hal_arm.h"
#include "x_hal_5381.h"
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
#include "chip_txt.h"
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

#if defined(CC_MTK_PRELOADER) && defined(NDEBUG)
#define Printf(fmt...)
#endif /* defined(CC_MTK_PRELOADER) && defined(NDEBUG) */

//-----------------------------------------------------------------------------
// Constant definition
//-----------------------------------------------------------------------------

#define ICACHE_ENABLE               (1 << 12)
#define SDR_32_TURBO                0x00000570

#define CLK_1600MHZ    1600000000
#define CLK_1333MHZ    1333000000
#define CLK_1242MHZ    1242000000
#define CLK_1188MHZ    1188000000
#define CLK_1080MHZ    1080000000
#define CLK_1066MHZ    1066000000
#define CLK_1026MHZ    1026000000
#define CLK_972MHZ      972000000
#define CLK_810MHZ      810000000
#define CLK_800MHZ      800000000
#define CLK_700MHZ      700000000
#define CLK_650MHZ      650000000
#define CLK_600MHZ      600000000
#define CLK_550MHZ      550000000
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

#define KEY_ESCAPE                      0x1b

//-----------------------------------------------------------------------------
// Public functions declaration
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER
EXTERN UINT32 g_u4DramSize;
EXTERN UINT32 g_u4DramChBSize;
#endif /* CC_MTK_LOADER */

EXTERN void RemapJumpToDram(void);

//-----------------------------------------------------------------------------
// This file
//-----------------------------------------------------------------------------

EXTERN UINT32 IsInitBootVerbose(void);
EXTERN UINT32 IsBootVerbose(void);
EXTERN void BootInit(void);
EXTERN void BootBimInitial(void);
EXTERN void HalSetInstTCMReg(UINT32 u4BaseAddr, UINT32 u4Size, BOOL fgEnable);

//-----------------------------------------------------------------------------
// Private functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** IsInitBootVerbose() return assembly verbose flag, only call from assembly.
 *  @retval 1 to verbose message, otherwise quite.
 */
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER

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
  //  PDWNC_WRITE32(REG_RW_PDWNC_INTEN, 0);

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
//    PDWNC_WRITE32(REG_RW_IOPADEN, PDWNC_READ32(REG_RW_IOPADEN) | PAD_UR0_TX_EN);//vIO32Write4B(PDWNC_GPIOEN, u4IO32Read4B(PDWNC_GPIOEN) | PAD_UR0_TX_EN);
    HAL_WRITE32(PDWNC_GPIOEN1, HAL_READ32(PDWNC_GPIOEN1) | PAD_UR0_TX_EN);

    // set bus access timeout to avoid register access halting CPU.
    BIM_WRITE32(REG_RW_TIMEOUT_CTRL, TO_FLASH_CNT | TO_DRAM_CNT | TO_IOBUS_CNT | TO_FLASH_EN | TO_DRAM_EN | TO_IOBUS_EN);

    // Disable and clear interrupt.
    BIM_WRITE32(REG_IRQEN, 0);
    BIM_WRITE32(REG_FIQEN, 0);
    BIM_WRITE32(REG_IRQCL, 0xffffffff);
    BIM_WRITE32(REG_FIQCL, 0xffffffff);
}
#endif /* CC_MTK_LOADER */

#ifdef CC_MTK_PRELOADER
static void CLK_Spectrum(UINT32 u4Clk)
{
    /* Check if DDS. */
    UINT32 u4Val;
    UINT32 u4Divider;
    UINT32 u4Permillage = 0;
    UINT32 u4Frequency = 0;
    UINT32 u4DMSS,u4CW_TEST;
    UINT32 u4SysPLL = 648;          // Unit in Mhz.

    /* Get Spectrum Permillage. */
    u4Permillage = DRVCUST_InitGet(eDmpllSpectrumPermillage);
    if (u4Permillage == 0)
    {
        return;
    }
    if (u4Permillage > 200)
    {
        LOG(1, "DRAM DELTA overflow.\n");
        return;
    }
    
    u4Frequency = DRVCUST_InitGet(eDmpllSpectrumFrequency);
    if (u4Frequency == 0)
    {
        return;
    }
    
    if (!IS_DDR_DMSSON())    
    {
        LOG(1, "DRAM SSC off.\n");
    }
    else
    {   
        LOG(1, "DRAM SSC Step=%d, Freq=%d Khz.\n", u4Permillage, u4Frequency);
    }

    // DMSS power down = 1 : normal, = 0: power down..    
    u4Val = IO_READ32(ANA_PLL_GROUP_BASE, 0x20);
    IO_WRITE32(ANA_PLL_GROUP_BASE, 0x20,u4Val|0x00004000);

    //DMSS_CKTX_PWDB power down, current mode set to '1'    
	u4Val = IO_READ32(ANA_PLL_GROUP_BASE, 0x20);
    if(!IS_DDR_CTSMODE())
    {
        IO_WRITE32(ANA_PLL_GROUP_BASE, 0x20,u4Val|0x00000040);
    }

    u4Val = IO_READ32(ANA_PLL_GROUP_BASE, 0x20);
    u4Val &= ~0x1800;      //DMSS output clock DSS clock/2
    IO_WRITE32(ANA_PLL_GROUP_BASE, 0x20, u4Val);

    u4Val = IO_READ32(ANA_PLL_GROUP_BASE, 0x20);
    IO_WRITE32(ANA_PLL_GROUP_BASE, 0x20, u4Val|0x000002000); //DMSS_MONEN=1

    /* Disable Spectrum */
    u4Val = CKGEN_READ32(0x194);
    u4Val &= ~0x00004000;
    CKGEN_WRITE32(0x194, u4Val);

    /*back to  operation status */
    u4Val = CKGEN_READ32(0x194);
    u4Val |= 0x00008002;
    CKGEN_WRITE32(0x194, u4Val);

#ifdef CC_MT5396
    /* Set Direct Mempll reference clock(Bypass Dmpll) from Xtal to DMSS */
    u4Val = CKGEN_READ32(0x338);
    CKGEN_WRITE32(0x338, u4Val | 0x00000001);
#endif    
   
    /* Set DMSS CW word and test.*/
    u4Clk = u4Clk>>1;//1188 =>594  DRAM Clock frequency
    u4Divider = u4Clk/CLK_27MHZ;   // MEMPLL_Divider * 27Mhz = DRAM clock.   MEMPLL_Divider=594/27=22     
    u4DMSS = u4Clk/u4Divider;// DMSS output frequency=DRAM clock/MEMPLL_Divider  594000000/22=27000000 
    /*  syspll x CW_test=DMSS output frequency
        syspll(Mhz) x CW_test=(DMSS output frequency /1000 000)(MHz)  =>syspll(Mhz)x1000 000 x CW_test=(DMSS output frequency )(MHz)
        CW_test=CW_test' x2^16 => syspll(Mhz)x1000 000 x CW_test' x2^16=(DMSS output frequency )(MHz)
    */
    u4CW_TEST = (u4SysPLL*1000*1000*4)/(u4DMSS/(4*256));
    u4CW_TEST = u4CW_TEST*(1024*4)/2; // Fractional part is 24 bits.
    u4CW_TEST = u4CW_TEST -0x1000000;//0x1200 0000 - 0x100 0000=0x1100 0000
    CKGEN_WRITE32(0x190, u4CW_TEST);// set target frequency

    u4Val =(u4SysPLL/54)*u4Permillage*2*2;
    u4Val =(u4Val*16 *1024)/(54000/(u4Frequency*16));
    u4Val =u4Val /10000;
    u4Val= ((u4Val/2)<<16)|u4Val;
    CKGEN_WRITE32(0x198, u4Val);//set DELTA & center spread

    u4Val = CKGEN_READ32(0x190);
    CKGEN_WRITE32(0x190,u4Val|0x80000000);//after change DMSS setting, DE suggest to trggle 0xD230[31]=1

    /* Setup Period. For 27.0mhz dmss output (sysplll:648)*/
    //turn on NCPO and FIFO state machine & DMSS_PRD & SSC triangle wave
    u4Val = 27000 / u4Frequency;    
    CKGEN_WRITE32(0x194, (CKGEN_READ32(0x194) & 0xFFFF) | (u4Val<<16)|0x0000A003);

}

static void CLK_SetMempll(UINT32 u4Clk)
{
    UINT32 u4MemClk;
    UINT32 u4Val;
    UINT32 u4MempllDevider;
    UINT32 u4BandControl;

    /*
    MemPLL setting.
    */
    u4Val = DRAM_DDRPHY_READ32(0x344);

    if (!IS_DDR_ASYNCON())
    {
        DRAM_DDRPHY_WRITE32(0x344, u4Val | 0x78);
    }
    else
    {
        DRAM_DDRPHY_WRITE32(0x344, u4Val | 0x58);
    }

    u4MemClk = u4Clk/2; 
    u4MempllDevider= (u4MemClk/ CLK_27MHZ) - 1;

    u4BandControl = (u4MemClk <= CLK_700MHZ) ? (1 << 9) : 0;

    if (!IS_DDR_CTSMODE())
    {
        DRAM_DDRPHY_WRITE32(0x294, DRAM_DDRPHY_READ32(0x294) & (~0x00900000));
    }
    else //CTS mode
    {
        DRAM_DDRPHY_WRITE32(0x294, DRAM_DDRPHY_READ32(0x294) | 0x00900000);
    }

    //enable mempll watchdog
    u4Val = DRAM_DDRPHY_READ32(0x294) & (~0xFC00);
    DRAM_DDRPHY_WRITE32(0x294, u4Val | 0x400);
    
    u4Val= 0x000c010F;
    u4Val |= (u4BandControl) | (u4MempllDevider << 24);

    /*
    MemPLL power on sequence.
    */
    DRAM_DDRPHY_WRITE32(0x290, u4Val | 0x80000000);
    u4Val = DRAM_DDRPHY_READ32(0x290);
    DRAM_DDRPHY_WRITE32(0x290, u4Val & ~0x00000010);
    u4Val = DRAM_DDRPHY_READ32(0x290);
    DRAM_DDRPHY_WRITE32(0x290, u4Val | 0x00000020);
    CHIP_Delay_us(10);			
    u4Val = DRAM_DDRPHY_READ32(0x290);
    DRAM_DDRPHY_WRITE32(0x290, u4Val & ~0x80000000);
    u4Val = DRAM_DDRPHY_READ32(0x290);
    DRAM_DDRPHY_WRITE32(0x290, u4Val & ~0x00000020);
    CHIP_Delay_us(10);

#if 0
    /*
    MemPLL clk output, TP/TN.
    */
    u4Val = DRAM_DDRPHY_READ32(0x290);
    u4Val &= ~0x00000004;
    u4Val |= 0x00000003;    
    DRAM_DDRPHY_WRITE32(0x290, u4Val);

    u4Val = DRAM_DDRPHY_READ32(0x294);
    u4Val |= 0x01000000;    
    DRAM_DDRPHY_WRITE32(0x294, u4Val);
#endif    

#if 0
    /*
    MemPLL clk output, Reference clk output
    */
    u4Val = DRAM_DDRPHY_READ32(0x290);
    u4Val &= ~0x00000004;
    u4Val |= 0x00000003;    
    DRAM_DDRPHY_WRITE32(0x290, u4Val);

    u4Val = DRAM_DDRPHY_READ32(0x294);
    u4Val |= 0x10000200;    
    DRAM_DDRPHY_WRITE32(0x294, u4Val);
#endif    
}
#endif /* #ifdef CC_MTK_PRELOADER */

#if defined(CC_MTK_PRELOADER) && (defined(DRAM_DEBUG) || defined(DRAM_SLT_DEBUG))
#ifdef DRAM_DEBUG
CHAR * DDR_DramTypeString(UINT32 u4Type)
{
    switch(u4Type)
    {
    case DDR_II_x1:
        return "DDR-IIx1";
    case DDR_II_x2:
        return "DDR-IIx2";
    case DDR_II_x3:
        return "DDR-IIx3";
	case DDR_II_x4:
		return "DDR-IIx4";
    case DDR_III_x1:
        return "DDR-IIIx1";
    case DDR_III_x2:
        return "DDR-IIIx2";
    case DDR_III_x3:
        return "DDR-IIIx3";
	case DDR_III_x4:
		return "DDR-IIIx4";
    default:
        return "unknown";
    }
}
#endif /* DRAM_DEBUG */

static void _CLK_DisplayInfo(UINT32 u4Stage)
{
    if (u4Stage == DRAM_CHANGE_CLK_SETTING)
    {
        /*Stage 1.*/
        CHIP_DisplayString("CLK: ");
        CHIP_DisplayInteger((TCMGET_DDR_CLK()/BASE_DDR_CLK));
        CHIP_DisplayString("MHz ('[' or ']' to change)\n");

        if (IS_DDR_DMSSON())
        {
            CHIP_DisplayString("DMSS: On ('\\' to change)\n");
        }
        else
        {
            CHIP_DisplayString("DMSS: Off ('\\' to change)\n");
        }      

        if (IS_DDR_ASYNCON())
        {
            CHIP_DisplayString("ASYNC: On ('N' to change)\n");
        }
        else
        {
            CHIP_DisplayString("ASYNC: Off ('N' to change)\n");
        }      

        if (IS_DDR_CTSMODE())
        {
            CHIP_DisplayString("CTS mode ('M' to change to Current mode)\n");
        }
        else
        {
            CHIP_DisplayString("Current mode ('M' to change to CTS mode)\n");
        }   

        CHIP_DisplayString("CAS Latency: ");
        CHIP_DisplayInteger(TCMGET_DDR_CL());
        CHIP_DisplayString("('T' to change)\n");
#ifdef DRAM_DEBUG
        Printf("1 DDR Clock:%d Hz (''-' to manual setting)\n", TCMGET_DDR_CLK());
    }
    else
    {
        /*Stage 2, 3.*/    
        Printf("2 DQSDRVP: 0x%01X ('Q': to change)\n", (DRAM_READ32(0x310) >> 28) & 0xF);
        Printf("2 DQSDRVN: 0x%01X ('W': to change)\n", (DRAM_READ32(0x310) >> 24) & 0xF);
        Printf("2 DQDRVP: 0x%01X ('E': to change)\n", (DRAM_READ32(0x318) >> 28) & 0xF);
        Printf("2 DQDRVN: 0x%01X ('R': to change)\n", (DRAM_READ32(0x318) >> 24) & 0xF);

        Printf("2 CLKDRVP: 0x%01X ('A': to change)\n", (DRAM_DDRPHY_READ32(0x300) >> 28) & 0xF);
        Printf("2 CLKDRVN: 0x%01X ('S': to change)\n", (DRAM_DDRPHY_READ32(0x300) >> 24) & 0xF);
        Printf("2 CMDRVP: 0x%01X ('D': to change)\n", (DRAM_DDRPHY_READ32(0x308) >> 28) & 0xF);
        Printf("2 CMDRVN: 0x%01X ('F': to change)\n", (DRAM_DDRPHY_READ32(0x308) >> 24) & 0xF);

        Printf("2 DQSODTP: 0x%01X ('G': to change)\n", (DRAM_READ32(0x310) >> 20) & 0x7);
        Printf("2 DQSODTN: 0x%01X ('H': to change)\n", (DRAM_READ32(0x310) >> 16) & 0x7);
        Printf("2 DQODTP: 0x%01X ('J': to change)\n", (DRAM_READ32(0x318) >> 20) & 0x7);
        Printf("2 DQODTN: 0x%01X ('K': to change)\n", (DRAM_READ32(0x318) >> 16) & 0x7);

        if (u4Stage == DRAM_CHANGE_DDR_BEFORE_AUTOINIT)
        {
            // MRS, EMRS only can be do before DRAM auto init.
            Printf("2 EMRS_H: 0x%01X ('U': to change)\n", (DRAM_READ32(0x10) >> 20) & 0xF);
            Printf("2 EMRS_L: 0x%01X ('I': to change)\n", (DRAM_READ32(0x10) >> 16) & 0xF);
            Printf("2 MRS_H: 0x%01X ('O': to change)\n", (DRAM_READ32(0x10) >> 4) & 0xF);
            Printf("2 MRS_L: 0x%01X ('P': to change)\n", (DRAM_READ32(0x10) >> 0) & 0xF);
            
            Printf("2 E3MRS_H: 0x%01X ('Z': to change)\n", (DRAM_READ32(0x14) >> 20) & 0xF);
            Printf("2 E3MRS_L: 0x%01X ('X': to change)\n", (DRAM_READ32(0x14) >> 16) & 0xF);
            Printf("2 E2MRS_H: 0x%01X ('C': to change)\n", (DRAM_READ32(0x14) >> 4) & 0xF);
            Printf("2 E2MRS_L: 0x%01X ('V': to change)\n", (DRAM_READ32(0x14) >> 0) & 0xF);
        }

        Printf("2 CLK0DLY: 0x%01X ('Y': to change)\n", (DRAM_DDRPHY_READ32(0x304) >> 0) & 0x0F);
        Printf("2 CLK1DLY: 0x%01X ('L': to change)\n", (DRAM_DDRPHY_READ32(0x304) >> 4) & 0x0F);
#endif /* DRAM_DEBUG */        
    }
}

static void _CLK_DisplayMenu(UINT32 u4Stage)
{
    CHIP_DisplayString("\n--\n");
    _CLK_DisplayInfo(u4Stage);
    CHIP_DisplayString("Press 'b' to download\n");
    CHIP_DisplayString("Press '0' to exit:\n\n");
}
#endif /* #if defined(CC_MTK_PRELOADER) && (defined(DRAM_DEBUG) || defined(DRAM_SLT_DEBUG)) */

#ifdef CC_MTK_PRELOADER
static void _CLKResetTCM(UINT32 u4DefClk)
{
    UINT32 u4Ret;
    UINT32 u4DramType = 0;
   
    TCM_DRAM_SIZE = 0;
    TCM_DRAM_FLAGS = 0;
    TCM_DRAM_FLAGS1 = 0;
    TCM_DRAM_FLAGS2 = 0;
    TCM_DRAM_FLAGS3 = 0;
    TCM_DRAM_FLAGS4 = 0;
    TCM_DRAM_REGBASE = DRAM_BASE;
    TCM_DRAM_DDRPHY_REGBASE = DRAM_DDRPHY_BASE;

    // Set DRAM type.           
    u4Ret = DRVCUST_InitGet(eDramType);
    TCM_DRAM_FLAGS |= ((u4Ret & 0xF) << TYPE_SHIFT);
    u4DramType = u4Ret;

    // Set DRAM clock.
    TCM_DRAM_FLAGS |= (((u4DefClk) / BASE_DDR_CLK) & DRAM_CLOCK_MASK);

    // Set DRAM DMSS on/off.
    if ((DRVCUST_InitGet(eDmpllSpectrumPermillage) != 0) && 
         (DRVCUST_InitGet(eDmpllSpectrumFrequency) != 0))
    {
        TCM_DRAM_FLAGS |= DRAM_DMSSON;
    }


    // Default turn on DRAM clock async mode.
    TCM_DRAM_FLAGS |= DRAM_ASYNCON;

    //Default input mempll clk source is CTS mode
    TCM_DRAM_FLAGS |= DRAM_CTSMODE;

    // Set if channel B is support.
    if ((u4DramType == DDR_II_x3) || (u4DramType == DDR_III_x3))
    {
        TCM_DRAM_FLAGS |= DRAM_CHANNELB_SUPPORT;
        TCM_DRAM_FLAGS |= DRAM_CHB_FORCE32;
    }
    else if((u4DramType == DDR_II_x4) || (u4DramType == DDR_III_x4))
    {
    	TCM_DRAM_FLAGS |= DRAM_CHANNELB_SUPPORT;
    }
    
    // Set 16 bits swap: DQS0 <-> DQS2, DQS1 <-> DQS3 exchange function.
    if (DRVCUST_InitGet(eFlagDDR16BitSwap))
    {
        TCM_DRAM_FLAGS |= DRAM_SWAP16BIT;
        LOG(1, "\nDRAM 16BitSwap Enable.\n");
    }
   
    // Set 16 bit mode or 32 bit mode data bus.    
    if ((u4DramType == DDR_II_x1) ||(u4DramType == DDR_III_x1))
    {
        TCM_DRAM_FLAGS |= DRAM_CHA_FORCE32;
    }

    // Set QFP or BGA package.
    u4Ret = DRVCUST_InitGet(eFlagDDRQfp);
    TCM_DRAM_FLAGS |= ((u4Ret & 0x1) << QFP_SHIFT);

    // Set bus is x8 or x16.
    TCM_DRAM_FLAGS |=  DRVCUST_InitGet(eDdrBusX8) ? DRAM_BUSX8 : 0;

    // Set CAS latency by DRAM clock.
    if ((u4DramType == DDR_II_x1) ||(u4DramType == DDR_II_x2) || (u4DramType == DDR_II_x3) || (u4DramType == DDR_II_x4))
    {
        if (u4DefClk <= CLK_800MHZ)
        {
            TCM_DRAM_FLAGS |= ((6) << CL_SHIFT);    
        }        
        else
        {
            TCM_DRAM_FLAGS |= ((7) << CL_SHIFT);    
        }
    }
    else
    {
        // DDR3.
        if (u4DefClk <= CLK_800MHZ)
        {
            TCM_DRAM_FLAGS |= ((6) << CL_SHIFT);    
        }        
        else if (u4DefClk <= CLK_1066MHZ)
        {
            TCM_DRAM_FLAGS |= ((7) << CL_SHIFT);
        }
        else if (u4DefClk <= CLK_1188MHZ)
        {
            TCM_DRAM_FLAGS |= ((9) << CL_SHIFT);    
        }
        else if(u4DefClk <= CLK_1333MHZ)
        {
            TCM_DRAM_FLAGS |= ((10) << CL_SHIFT);
        }
	    else 
	    {
	        TCM_DRAM_FLAGS |= ((11) << CL_SHIFT);
	    }
    }    
}
#endif /* #ifdef CC_MTK_PRELOADER */

//-----------------------------------------------------------------------------
// Public functions.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/* CLK_QueryDramSetting() for user to modify TCM_DRAM_SETUP value of the DRAM
 *      controller parameters.
 */
//-----------------------------------------------------------------------------
#if (!defined(__MODEL_slt__)) && defined(CC_MTK_PRELOADER) && (defined(DRAM_DEBUG) || defined(DRAM_SLT_DEBUG))
void CLK_QueryDramSetting(UINT8 *szString, UINT32 u4Stage)
{
    UINT32 u4Val;
    UINT32 i, u4CpuClk;
    UCHAR uc;
    
    u4CpuClk = CLK_200MHZ;
    // Wait a moment for ESC to enter setup menu
    CHIP_DisplayString((const char *)szString);
    SerTransparent();
    uc = 0;
    CHIP_DisplayString("\n");
    for (i=0; i<(u4CpuClk >> 15); i++)
    {
        CHIP_DisplayString("\r");
        switch(i & 0x3) {
        case 0:
            CHIP_DisplayString("|"); break;
        case 1:
            CHIP_DisplayString("/"); break;
        case 2:
            CHIP_DisplayString("-"); break;
        case 3:
            CHIP_DisplayString("\\"); break;
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
        CHIP_DisplayString("\n");
        return;
    }

    // Otherwise, enter menu loop.    
    while (1)
    {
        _CLK_DisplayMenu(u4Stage);

        // wait a key.
        while (!SerInByte(&uc)) { }

        if (u4Stage == DRAM_CHANGE_CLK_SETTING)
        {
            switch(uc)
            {        
            case ']':
                ADD_DDR_CLK();            
                break;
            case '[':
                SUB_DDR_CLK();
                break;
                
            case '\\':
                // Toggle DMSS on/off status.
                if (IS_DDR_DMSSON())
                {
                    TCM_DRAM_FLAGS &= ~DRAM_DMSSON;
                }
                else
                {
                    TCM_DRAM_FLAGS |= DRAM_DMSSON;
                }
                break;

            case 'N': case 'n':
                // Toggle ASYNC on/off status.
                if (IS_DDR_ASYNCON())
                {
                    TCM_DRAM_FLAGS &= ~DRAM_ASYNCON;
                }
                else
                {
                    TCM_DRAM_FLAGS |= DRAM_ASYNCON;
                }
                break;

            case 'M': case 'm':
                // Toggle ASYNC on/off status.
                if (IS_DDR_CTSMODE())
                {
                    TCM_DRAM_FLAGS &= ~DRAM_CTSMODE;
                }
                else
                {
                    TCM_DRAM_FLAGS |= DRAM_CTSMODE;
                }
                break;

            case 'T': case 't':
                u4Val = TCMGET_DDR_CL();
                u4Val++;
                if (u4Val > 11)
                {
                    u4Val = 6;
                }
                TCM_DRAM_FLAGS &= ~(DRAM_DDR_CL_MASK);
                TCM_DRAM_FLAGS |= ((u4Val & 0xF) << CL_SHIFT);
                break;
#ifdef DRAM_DEBUG          
            case '_': case '-':
                CHIP_DisplayString("Enter DRAM clock:\n");
                while (!SerInByte(&uc)) { }
                // Ascii to integer.
                u4Val = (uc-0x30)*1000;
                while (!SerInByte(&uc)) { }
                u4Val += (uc-0x30)*100;
                while (!SerInByte(&uc)) { }
                u4Val += (uc-0x30)*10;
                while (!SerInByte(&uc)) { }
                u4Val += (uc-0x30);

                CHIP_DisplayString("Set DRAM clock = ");
                CHIP_DisplayInteger(u4Val);
                CHIP_DisplayString(" MHz.\n");

                TCM_DRAM_FLAGS &= ~(DRAM_CLOCK_MASK);
                TCM_DRAM_FLAGS |= (u4Val & DRAM_CLOCK_MASK);
                // Clear uc to aviod exit.
                uc = 0;
                break;
#endif /* DRAM_DEBUG */                
            default:
                break;
            }
        }

#ifdef DRAM_DEBUG
        if (u4Stage == DRAM_CHANGE_DDR_BEFORE_AUTOINIT)
        {            
            switch(uc)
            {        
                // MRS, EMRS setting start.
            case 'U': case 'u':
                u4Val = (DRAM_READ32(0x10) >> 20) & 0xF;
                u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
                u4Val = u4Val << 20;
                u4Val = (DRAM_READ32(0x10) & 0xFF0FFFFF) | u4Val;
                DRAM_WRITE32(0x10, u4Val);
                break;
            case 'I': case 'i':
                u4Val = (DRAM_READ32(0x10) >> 16) & 0xF;
                u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
                u4Val = u4Val << 16;
                u4Val = (DRAM_READ32(0x10) & 0xFFF0FFFF) | u4Val;
                DRAM_WRITE32(0x10, u4Val);
                break;
            case 'O': case 'o':
                u4Val = (DRAM_READ32(0x10) >> 4) & 0xF;
                u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
                u4Val = u4Val << 4;
                u4Val = (DRAM_READ32(0x10) & 0xFFFFFF0F) | u4Val;
                DRAM_WRITE32(0x10, u4Val);
                break;
            case 'P': case 'p':
                u4Val = (DRAM_READ32(0x10) >> 0) & 0xF;
                u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
                u4Val = u4Val << 0;
                u4Val = (DRAM_READ32(0x10) & 0xFFFFFFF0) | u4Val;
                DRAM_WRITE32(0x10, u4Val);
                break;
                
                // E2MRS, E3MRS setting start.
            case 'Z': case 'z':
                u4Val = (DRAM_READ32(0x14) >> 20) & 0xF;
                u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
                u4Val = u4Val << 20;
                u4Val = (DRAM_READ32(0x14) & 0xFF0FFFFF) | u4Val;
                DRAM_WRITE32(0x14, u4Val);
                break;
            case 'X': case 'x':
                u4Val = (DRAM_READ32(0x14) >> 16) & 0xF;
                u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
                u4Val = u4Val << 16;
                u4Val = (DRAM_READ32(0x14) & 0xFFF0FFFF) | u4Val;
                DRAM_WRITE32(0x14, u4Val);
                break;
            case 'C': case 'c':
                u4Val = (DRAM_READ32(0x14) >> 4) & 0xF;
                u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
                u4Val = u4Val << 4;
                u4Val = (DRAM_READ32(0x14) & 0xFFFFFF0F) | u4Val;
                DRAM_WRITE32(0x14, u4Val);
                break;
            case 'V': case 'v':
                u4Val = (DRAM_READ32(0x14) >> 0) & 0xF;
                u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
                u4Val = u4Val << 0;
                u4Val = (DRAM_READ32(0x14) & 0xFFFFFFF0) | u4Val;
                DRAM_WRITE32(0x14, u4Val);
                break;
                
            default:
                break;
            }
        }
        
        if ((u4Stage == DRAM_CHANGE_DDR_BEFORE_AUTOINIT) ||
            (u4Stage == DRAM_CHANGE_DDR_AFTER_AUTOINIT))            
        {            
            switch(uc)
            {        
            // DQSDRVP, DQSDRVN, DQDRVP, DQDRVN setting start.
            case 'Q': case 'q':
                u4Val = (DRAM_READ32(0x310) >> 28) & 0xF;
                u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
                u4Val = u4Val << 28;
                u4Val = (DRAM_READ32(0x310) & 0x0FFFFFFF) | u4Val;
                DRAM_WRITE32(0x310, u4Val);
                break;
            case 'W': case 'w':
                u4Val = (DRAM_READ32(0x310) >> 24) & 0xF;
                u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
                u4Val = u4Val << 24;
                u4Val = (DRAM_READ32(0x310) & 0xF0FFFFFF) | u4Val;
                DRAM_WRITE32(0x310, u4Val);
                break;
            case 'E': case 'e':
                u4Val = (DRAM_READ32(0x318) >> 28) & 0xF;
                u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
                u4Val = u4Val << 28;
                u4Val = (DRAM_READ32(0x318) & 0x0FFFFFFF) | u4Val;
                DRAM_WRITE32(0x318, u4Val);
                break;
            case 'R': case 'r':
                u4Val = (DRAM_READ32(0x318) >> 24) & 0xF;
                u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
                u4Val = u4Val << 24;
                u4Val = (DRAM_READ32(0x318) & 0xF0FFFFFF) | u4Val;
                DRAM_WRITE32(0x318, u4Val);
                break;

            // CLKDRVP, CLKDRVN, CMDDRVP, CMDDRVN setting start.
            case 'A': case 'a':
                u4Val = (DRAM_DDRPHY_READ32(0x300) >> 28) & 0xF;
                u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
                u4Val = u4Val << 28;
                u4Val = (DRAM_DDRPHY_READ32(0x300) & 0x0FFFFFFF) | u4Val;
                DRAM_DDRPHY_WRITE32(0x300, u4Val);
                break;
            case 'S': case 's':
                u4Val = (DRAM_DDRPHY_READ32(0x300) >> 24) & 0xF;
                u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
                u4Val = u4Val << 24;
                u4Val = (DRAM_DDRPHY_READ32(0x300) & 0xF0FFFFFF) | u4Val;
                DRAM_DDRPHY_WRITE32(0x300, u4Val);
                break;
            case 'D': case 'd':
                u4Val = (DRAM_DDRPHY_READ32(0x308) >> 28) & 0xF;
                u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
                u4Val = u4Val << 28;
                u4Val = (DRAM_DDRPHY_READ32(0x308) & 0x0FFFFFFF) | u4Val;
                DRAM_DDRPHY_WRITE32(0x308, u4Val);
                break;
            case 'F': case 'f':
                u4Val = (DRAM_DDRPHY_READ32(0x308) >> 24) & 0xF;
                u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
                u4Val = u4Val << 24;
                u4Val = (DRAM_DDRPHY_READ32(0x308) & 0xF0FFFFFF) | u4Val;
                DRAM_DDRPHY_WRITE32(0x308, u4Val);
                break;

                // DQSODTP, DQSODTN, DQODTP, DQODTN setting start.
            case 'G': case 'g':
                u4Val = (DRAM_READ32(0x310) >> 20) & 0x7;
                u4Val = (u4Val == 0x7) ? 0 : (u4Val + 1);
                u4Val = u4Val << 20;
                u4Val = (DRAM_READ32(0x310) & 0xFF8FFFFF) | u4Val;
                DRAM_WRITE32(0x310, u4Val);
                break;
            case 'H': case 'h':
                u4Val = (DRAM_READ32(0x310) >> 16) & 0x7;
                u4Val = (u4Val == 0x7) ? 0 : (u4Val + 1);
                u4Val = u4Val << 16;
                u4Val = (DRAM_READ32(0x310) & 0xFFF8FFFF) | u4Val;
                DRAM_WRITE32(0x310, u4Val);
                break;
            case 'J': case 'j':
                u4Val = (DRAM_READ32(0x318) >> 20) & 0x7;
                u4Val = (u4Val == 0x7) ? 0 : (u4Val + 1);
                u4Val = u4Val << 20;
                u4Val = (DRAM_READ32(0x318) & 0xFF8FFFFF) | u4Val;
                DRAM_WRITE32(0x318, u4Val);
                break;
            case 'K': case 'k':
                u4Val = (DRAM_READ32(0x318) >> 16) & 0x7;
                u4Val = (u4Val == 0x7) ? 0 : (u4Val + 1);
                u4Val = u4Val << 16;
                u4Val = (DRAM_READ32(0x318) & 0xFFF8FFFF) | u4Val;
                DRAM_WRITE32(0x318, u4Val);
                break;

                // CLK0, CLK1 delay start.
            case 'Y': case 'y':
                u4Val = (DRAM_DDRPHY_READ32(0x304) >> 0) & 0x0F;
                u4Val = (u4Val == 0x0F) ? 0 : (u4Val + 1);
                u4Val = u4Val << 0;
                u4Val = (DRAM_DDRPHY_READ32(0x304) & 0xFFFFFFF0) | u4Val;
                DRAM_DDRPHY_WRITE32(0x304, u4Val);
                break;
            case 'L': case 'l':
                u4Val = (DRAM_DDRPHY_READ32(0x304) >> 4) & 0x0F;
                u4Val = (u4Val == 0x0F) ? 0 : (u4Val + 1);
                u4Val = u4Val << 4;
                u4Val = (DRAM_DDRPHY_READ32(0x304) & 0xFFFFFF0F) | u4Val;
                DRAM_DDRPHY_WRITE32(0x304, u4Val);
                break;
                
            default:
                break;
            }
        }
#endif /* DRAM_DEBUG */                

        switch(uc)
        {        
        case 'B': case 'b':
            SerNormalMode();
            CHIP_DisplayString("RS232 leave transparent mode! Console stop!\n");
            while (1) { }
        case '0': // to Exit.
            break;

        default:
            continue;
        }
        
        if (uc == '0')
        {
            break;
        }
    }

    SerNormalMode();
    return;
}
#endif /* #if defined(CC_MTK_PRELOADER) && (defined(DRAM_DEBUG) || defined(DRAM_SLT_DEBUG)) */

#ifdef CC_MTK_PRELOADER

#if defined(CC_FPGA)
//-----------------------------------------------------------------------------
/* CLK_SdrCalibrate() for SDR calibration
 *
 */
//-----------------------------------------------------------------------------
UINT32 CLK_SdrCalibrate(void)
{
    //The following code is suggested by Maoauo Lin, 2009/09/09.
    *((volatile UINT32*)(IO_VIRT + 0x07000)) = 0x338B1165;
    *((volatile UINT32*)(IO_VIRT + 0x07004)) = 0x03015007;
    *((volatile UINT32*)(IO_VIRT + 0x07010)) = 0x00000022;

    *((volatile UINT32*)(IO_VIRT + 0x0700C)) = 0x00000200; // Trigger auto initialization

#ifdef __GCC__
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
#else
    __asm
    {
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
    }
#endif
    *((volatile UINT32*)(IO_VIRT + 0x0700C)) = 0x00000000;

    *((volatile UINT32*)(IO_VIRT + 0x0701C)) = 0x03300080;

    *((volatile UINT32*)(IO_VIRT + 0x07008)) = 0x0000020A;

    #ifndef CC_FPGA_SDR64B    
    // 128 bits DRAM setting => 2 DRAM modules on fpga.
    *((volatile UINT32*)(IO_VIRT + 0x07018)) = 0xC0021410;
    #else
    // 64 bits DRAM setting => 1 DRAM module on fpga.
    *((volatile UINT32*)(IO_VIRT + 0x07018)) = 0xC0021010;
    #endif

//#ifdef CC_FPGA
#if 0
    TCMSET_CHANNELA_SIZE(FPGA_DRAM_SIZE_MB);
#else
    TCMSET_CHANNELA_SIZE(_DDR_CheckSize(FALSE)/0x100000);
#endif
    CHIP_DisplayString("DRAM Size = ");       
    CHIP_DisplayInteger(TCMGET_CHANNELA_SIZE());
    CHIP_DisplayString(" Mbytes.\n");            

    return 0;
}
#endif /* #if defined(CC_FPGA) */

UINT32 CLK_SetDdrPll(void)
{
    UINT32 u4Clock;
	UINT32 u4Val;
    
    HAL_WRITE32(PDWNC_CLKPDN, HAL_READ32(PDWNC_CLKPDN)|PWRDWN_CLOCK_LOCK);

    _CLKResetTCM(DRVCUST_InitGet(eDdrClock));

    CHIP_DisplayString("\n");

#if defined(DRAM_DEBUG) || defined(DRAM_SLT_DEBUG)
    // User's chance to modify the default settings at TCM_DRAM_SETUP.
    CLK_QueryDramSetting((UINT8 *)"First setup dram menu:", DRAM_CHANGE_CLK_SETTING);
#endif /* #if defined(DRAM_DEBUG) || defined(DRAM_SLT_DEBUG) */

    u4Clock = TCMGET_DDR_CLK();

    CLK_Spectrum(u4Clock);
    
    CLK_SetMempll(u4Clock);

    /*
    Caution: This DRAM controller clock source. Need to be very careful.
    Set MCK to normal clcok
    */
    CKGEN_WRITE32(0x20C, 0x30); 

    u4Val = DRAM_DDRPHY_READ32(0x344) & (~0x18);
    DRAM_DDRPHY_WRITE32(0x344, u4Val);

    CHIP_Delay_us(10);
    
    if (IS_DDR_ASYNCON())
    {
        u4Val &= ~(0x1 << 6);
        DRAM_DDRPHY_WRITE32(0x344, u4Val);
    }

    // u4Clock is the final clock setting.
    Printf("\nDRAM data rate = %d Mbps.\n", u4Clock / 1000000);

    return 0;
}
#endif /* #ifdef CC_MTK_PRELOADER */

UINT32 CLK_SetDdr()
{
#ifdef CC_MTK_PRELOADER

#if defined(CC_FPGA)
    return CLK_SdrCalibrate();
#else
    return CLK_SetDdrPll();
#endif // #if defined(CC_FPGA)

#else
    return 0;
#endif /* #ifdef CC_MTK_PRELOADER */
}

#ifndef CC_MTK_PRELOADER
UINT32 BSP_IsSystemAcOn()
{
    //return IS_SYS_ACON();
    return 1;
}
#endif /* CC_MTK_PRELOADER */

//-----------------------------------------------------------------------------
// Below is cut from board.c
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** BSP_IsFPGA() Check if current platform if FPGA or not
 *  @reval TRUE: FPGA
 *  @reval FALSE: IC
 */
//-----------------------------------------------------------------------------
BOOL BSP_IsFPGA(void)
{
    UINT32 u4Val;

    /* If there is FPGA ID, it must be FPGA, too. */
    u4Val = BIM_READ32(REG_RO_FPGA_ID);
    if (u4Val != 0) { return 1; }

    /* otherwise, it's not FPGA. */
    return 0;
}

//-----------------------------------------------------------------------------
/** BSP_GetIcVersion() Get IC version
 *  @return The IC version, or IC_VER_FPGA if running in a FPGA platform, or
 *      IC_VER_UNKOWN for unknown IC version
 */
//-----------------------------------------------------------------------------
IC_VERSION_T BSP_GetIcVersion(void)
{
    IC_VERSION_T eVer = IC_VER_FPGA;

    if (!BSP_IsFPGA())
    {
        UINT32 u4Version;

        u4Version = BIM_READ32(REG_RO_SW_ID);

        return (IC_VERSION_T)u4Version;
    }

    return eVer;
}


#ifndef CC_MTK_LOADER
//-----------------------------------------------------------------------------
/** BSP_GetChBMemSize()
 *  @return the channel B dram size.
 */
//-----------------------------------------------------------------------------
UINT32 BSP_GetChBMemSize()
{
    return g_u4DramChBSize;
}

//-----------------------------------------------------------------------------
/** BSP_GetMemSize()
 *  @return the channel A dram size.
 */
//-----------------------------------------------------------------------------
UINT32 BSP_GetMemSize()
{
#ifdef FAKE_MEM_SIZE
    return (g_u4DramSize > FAKE_MEM_SIZE) ? FAKE_MEM_SIZE : g_u4DramSize;
#else
    return g_u4DramSize;
#endif
}

//-----------------------------------------------------------------------------
/** BSP_SetMemSize()
 *  @return the total dram size.
 */
//-----------------------------------------------------------------------------
void BSP_SetMemSize(void)
{
    g_u4DramSize = TCMGET_CHANNELA_SIZE() * 0x100000; 
}

//-----------------------------------------------------------------------------
/** BSP_SetChBMemSize()
 *  @return the total dram size.
 */
//-----------------------------------------------------------------------------
void BSP_SetChBMemSize(void)
{
    g_u4DramChBSize = TCMGET_CHANNELB_SIZE() * 0x100000; 
}

#endif /* !CC_MTK_LOADER */

//lint -save
//lint --e{550} Symbol not accessed
//lint --e{529} Symbol not subsequently referenced

//-----------------------------------------------------------------------------
// Below is cut from hal_926.c
//-----------------------------------------------------------------------------

#ifndef __GCC__
#pragma arm
#endif

//-----------------------------------------------------------------------------
/** HalInvalidateICache() Invalidate entire I-cache
 */
//-----------------------------------------------------------------------------
void HalInvalidateICache(void)
{
    register UINT32 r = 0;
#ifdef __GCC__
    __asm__ ("MCR     p15, 0, %0, c7, c5, 0" : : "r" (r));
#else
    __asm
    {
        MCR     p15, 0, r, c7, c5, 0
    }
#endif
}


//-----------------------------------------------------------------------------
/** HalEnableICache() Enable I-cache
 */
//-----------------------------------------------------------------------------
void HalEnableICache(void)
{
    register UINT32 r = 0;

    HalInvalidateICache();

#ifdef __GCC__
    __asm__ ("MRC     p15, 0, %0, c1, c0, 0" : "=r" (r));
    __asm__ ("ORR     %0, %1, %2" : "=r" (r) : "r" (r), "r" (ICACHE_ENABLE));
    __asm__ ("MCR     p15, 0, %0, c1, c0, 0" : : "r" (r));
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
#else
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
        NOP
        NOP
        NOP
    }
#endif
}
