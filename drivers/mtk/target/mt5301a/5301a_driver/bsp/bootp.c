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

#if defined(CC_MTK_PRELOADER) && defined(NDEBUG)
#define Printf(fmt...)
#endif /* defined(CC_MTK_PRELOADER) && defined(NDEBUG) */

//-----------------------------------------------------------------------------
// Constant definition
//-----------------------------------------------------------------------------

#define ICACHE_ENABLE               (1 << 12)
#define SDR_32_TURBO                0x00000570

#define CLK_810MHZ      810000000
#define CLK_800MHZ      800000000
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
extern UINT32 g_u4DramSize;
#endif

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
    HAL_WRITE32(PDWNC_GPIOEN, HAL_READ32(PDWNC_GPIOEN) | PAD_UR0_TX_EN);

    // set bus access timeout to avoid register access halting CPU.
    BIM_WRITE32(REG_RW_BTIME, TO_RTO_CNT | TO_RTEN);


    // Disable and clear interrupt.
    BIM_WRITE32(REG_IRQEN, 0);
    BIM_WRITE32(REG_FIQEN, 0);
    BIM_WRITE32(REG_IRQCL, 0xffffffff);
    BIM_WRITE32(REG_FIQCL, 0xffffffff);
}
#endif /* CC_MTK_LOADER */

#ifdef CC_MTK_PRELOADER
static void CLK_DmpllSpectrum(UINT32 u4Clk)
{
    /* Check if DDS. */
    UINT32 u4Val;
    UINT32 u4Divider;
    UINT32 u4CW;
    UINT32 u4TEST;
    UINT32 u4Permillage;
    UINT32 u4Frequency;

    /* Get Spectrum Permillage. */
    u4Permillage = DRVCUST_InitGet(eDmpllSpectrumPermillage);
    if (u4Permillage == 0)
    {
        return;
    }
    
    u4Frequency = DRVCUST_InitGet(eDmpllSpectrumFrequency);
    if (u4Frequency == 0)
    {
        return;
    }
    
    LOG(1, "DRAM SSC Step=%d, Freq=%d Khz.\n", u4Permillage, u4Frequency);

    /* Disable Spectrum */
    CKGEN_WRITE32(0x158, CKGEN_READ32(0x158) | 0x40);

    /* Reset dmpll spread spectrum digital circuit */	        
    CKGEN_WRITE32(0x158, CKGEN_READ32(0x158) & ~0x80);
    CHIP_Delay_us(10);
    CKGEN_WRITE32(0x158, CKGEN_READ32(0x158) | 0x80);

    /* Set DMPLL CW word. Only for SYSPLL 324MHz only.*/
    //u4Divider is around 26Mhz ~ 28Mhz.    
    u4Divider = (u4Clk >> 1)/CLK_13_5MHZ;
    u4Divider = u4Clk/u4Divider;
    u4Val = ((324*32*1000)/(u4Divider/1000));
    u4CW = (u4Val >> 5) & 0xF;
    u4TEST = u4Val & 0x1F;

    u4Val = CKGEN_READ32(0x158);
    u4Val &= ~0x00001F1F;
    u4Val |= u4CW;
    u4Val |= u4TEST << 8;    
    CKGEN_WRITE32(0x158, u4Val);

    /* Setup Step. */	       
    //an example for SYSPLL 324MHz only.
    // 4*(2^16)*(324/27)*(RANGE/10000)*modulation/(27000)
    // => ((2^18)*(12)*(modulation*RANGE))/(27000*10000)	       
    //for 27mhz dmss output (sysplll:324) 
    u4Val = (u4Frequency*u4Permillage* 116/10000) & 0xFFFF;
    u4Val |= (u4CW -1) << 16;
    CKGEN_WRITE32(0x15C, (CKGEN_READ32(0x15C) & ~0xFFFF) | u4Val);

    /* Setup Period. For 27.0mhz dmss output (sysplll:324)*/
    u4Val = (27000 /4) / u4Frequency;
    CKGEN_WRITE32(0x158, (CKGEN_READ32(0x158) & 0xFFFF) | (u4Val<<16));

    /* Setup direction. 0-center;1-down;2-up */
    CKGEN_WRITE32(0x158, (CKGEN_READ32(0x158) & 0xFFFF3FFF) | (0));

    /* Set Dmpll reference clock from Xtal to DMSS */
    CKGEN_WRITE32(0x110, CKGEN_READ32(0x110) | 0x00020000); 
    CHIP_Delay_us(200);
}

static void CLK_SetDmpll(UINT32 u4Clk, BOOL fgDMPLL27M)
{
    UINT32 u4MemClk;
    UINT32 u4Val;
    UINT32 u4Type = TCMGET_DRAMTYPE();

    u4MemClk = ((u4Type == SDR_x1) ? (u4Clk): (u4Clk/2));

    // Disable dram clock test mode.
    CKGEN_WRITE32(0x20C, 0);

    /*
        DMPLL setting.
    */
    if (fgDMPLL27M)
    {
        // Set mux to 27Mhz.
        u4Val = CKGEN_READ32(0x124);
        u4Val |= 0x00400000;
        CKGEN_WRITE32(0x124, u4Val);
    }
    else
    {
        u4Val = CKGEN_READ32(0x120);
        u4Val = u4Val & 0x00F0DFFF;
        u4Val |= 0x40010000; //prediv=2; charge pump = 10u
        
        if (IS_FULLFREQ())
        {
           // MEMPLL = 1 * DMPLL.
            u4Val |= ((2*u4MemClk/CLK_13_5MHZ) << 24);
        }
        else
        {
           // MEMPLL = 2 * DMPLL.
            u4Val |= ((u4MemClk/CLK_13_5MHZ) << 24);
        }
        CKGEN_WRITE32(0x120, u4Val);

        u4Val = CKGEN_READ32(0x124);
        u4Val = u4Val & 0xFF00F0C0;
        u4Val |= 0x00120019; //posdiv=2; r=4.5k; 

        if (u4Type == SDR_x1)
        {
            u4Val |= 0x0300; // VCO gain = 166Mhz/189Mhz.
        }
        else
        {
            if (u4MemClk <= CLK_300MHZ)
            {
                u4Val |= 0x0200; // VCO gain = 508 Mhz.
            }
            else if ((u4MemClk > CLK_300MHZ) && (u4MemClk <= CLK_400MHZ))
            {
                u4Val |= 0x0300; // VCO gain = 671 Mhz.
            }
            else if ((u4MemClk > CLK_400MHZ) && (u4MemClk <= CLK_550MHZ))
            {
                u4Val |= 0x0400; // VCO gain = 808 Mhz.
            }
            else 
            {
                u4Val |= 0x0500; // VCO gain = 930 Mhz.
            }
        }

        CKGEN_WRITE32(0x124, u4Val);
        CKGEN_WRITE32(0x120, CKGEN_READ32(0x120) |0x80000000);
    }           

    // Wait DMPLL stable.
    CHIP_Delay_us(1000);
}

static void CLK_SetMempll(UINT32 u4Clk, BOOL fgDMPLL27M)
{
    UINT32 u4MemClk;
    UINT32 u4Val;
    UINT32 u4MempllDevider;
    UINT32 u4Type = TCMGET_DRAMTYPE();

    /*
        MemPLL setting.
    */
    if (u4Type == SDR_x1)
    {
        // Set MEMPLL power down.
        CKGEN_WRITE32(0x160, (CKGEN_READ32(0x160) |0x00C00000));
    }
    else
    {
        // Support DDR.
        u4MemClk = u4Clk/2;

        if (fgDMPLL27M)
        {
            // Set Mempll divider.        
            u4MempllDevider = (((u4MemClk*2) / CLK_27MHZ) -2) & 0x1F;
            CKGEN_WRITE32(0x164, CKGEN_READ32(0x164) | (u4MempllDevider << 9));
        
            if (u4MemClk <= CLK_200MHZ)
            {
                // Use ICP=110, LPFR=11, BAND=11, INVERT=0.
                u4Val = 0x400288F7;
            }
            else if ((u4MemClk > CLK_200MHZ) && (u4MemClk <= CLK_300MHZ))
            {
                // Use ICP=110, LPFR=11, BAND=10, INVERT=0.
                u4Val = 0x400288F6;
            }
            else if ((u4MemClk > CLK_300MHZ) && (u4MemClk <= CLK_400MHZ))
            {
                // Use ICP=110, LPFR=11, BAND=01, INVERT=0.
                u4Val = 0x400288F5;
            }
            else 
            {
                // Set high frequency use = 1.
                // Use ICP=110, LPFR=11, BAND=10, INVERT=0.
                u4Val = 0x400688E4;
            }
            
            CKGEN_WRITE32(0x160, u4Val); 
        }
        else
        {
            if (IS_FULLFREQ())
            {
                // Set MEMPLL FBDIV1.
                CKGEN_WRITE32(0x164, CKGEN_READ32(0x164) | 0x100);
            }
        
            if (u4MemClk <= CLK_200MHZ)
            {
                u4Val = 0x40228863;
            }
            else if ((u4MemClk > CLK_200MHZ) && (u4MemClk <= CLK_300MHZ))
            {
                u4Val = 0x40228862;
            }
            else if ((u4MemClk > CLK_300MHZ) && (u4MemClk <= CLK_400MHZ))
            {
                u4Val = 0x40228861;
            }
            else 
            {
                // Set high frequency use = 1.
                u4Val = 0x40268860;
            }        
            
            CKGEN_WRITE32(0x160, u4Val); 
        }    

        // Wait PLL stable.
        CHIP_Delay_us(1000);
    }
}
#endif /* #ifdef CC_MTK_PRELOADER */

#if defined(CC_MTK_PRELOADER) && defined(DRAM_DEBUG)
CHAR * DDR_DramTypeString(UINT32 u4Type)
{
    switch(u4Type)
    {
    case SDR_x1:
        return "SDRx1";
    case DDR_I_x1:
        return "DDR-Ix1";
    case DDR_I_x2:
        return "DDR-Ix2";
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

static void _CLK_DisplayInfo(UINT32 u4Stage)
{
    if (u4Stage == DRAM_CHANGE_CLK_SETTING)
    {
        /*Stage 1.*/
        CHIP_DisplayString((const INT8 *)DDR_DramTypeString(TCMGET_DRAMTYPE()));        
        CHIP_DisplayString(", CLK: ");
        CHIP_DisplayInteger((TCMGET_DDR_CLK()/BASE_DDR_CLK));
        CHIP_DisplayString("MHz ('[' or ']' to change)\n");

        Printf("1 CAS Latency: %d ('T' to change)\n", (TCMGET_DDR_CL()));
        Printf("1 Column address bit is %d ('C' to change)\n", (TCMGET_COLADDR()));   
        Printf("1 DDR Clock:%d Hz (''-' to manual setting)\n", TCMGET_DDR_CLK());
    }
    else
    {
        /*Stage 2, 3.*/    
        Printf("2 DQSDRVP: 0x%01X ('Q': to change)\n", (DRAM_READ32(0x310) >> 28) & 0xF);
        Printf("2 DQSDRVN: 0x%01X ('W': to change)\n", (DRAM_READ32(0x310) >> 24) & 0xF);
        Printf("2 DQDRVP: 0x%01X ('E': to change)\n", (DRAM_READ32(0x318) >> 28) & 0xF);
        Printf("2 DQDRVN: 0x%01X ('R': to change)\n", (DRAM_READ32(0x318) >> 24) & 0xF);

        Printf("2 CLKDRVP: 0x%01X ('A': to change)\n", (DRAM_READ32(0x300) >> 28) & 0xF);
        Printf("2 CLKDRVN: 0x%01X ('S': to change)\n", (DRAM_READ32(0x300) >> 24) & 0xF);
        Printf("2 CMDRVP: 0x%01X ('Z': to change)\n", (DRAM_READ32(0x308) >> 28) & 0xF);
        Printf("2 CMDRVN: 0x%01X ('X': to change)\n", (DRAM_READ32(0x308) >> 24) & 0xF);

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
        }

        Printf("2 CLK0DLY: 0x%01X ('D': to change)\n", (DRAM_READ32(0x304) >> 0) & 0x0F);
        Printf("2 CLK1DLY: 0x%01X ('L': to change)\n", (DRAM_READ32(0x304) >> 4) & 0x0F);
    }
}

static void _CLK_DisplayMenu(UINT32 u4Stage)
{
    CHIP_DisplayString("\n--\n");
    _CLK_DisplayInfo(u4Stage);
    CHIP_DisplayString("Press 'b' to download\n");
    CHIP_DisplayString("Press '0' to exit:\n\n");
}
#endif /* #if defined(CC_MTK_PRELOADER) && defined(DRAM_DEBUG) */

#ifdef CC_MTK_PRELOADER
static void _CLKResetTCM(UINT32 u4DefClk)
{
    UINT32 u4Ret;
    UINT32 u4DramType = 0;

    // if TCM is not configured, reset TCM. Here is AC-on.
    TCM_DRAM_FLAGS = 0;
    TCM_DRAM_TYPE = 0;
       
    u4Ret = DRVCUST_InitGet(eDramType);
    TCM_DRAM_TYPE |= (u4Ret);
    u4DramType = u4Ret;

    TCM_DRAM_FLAGS |= (((u4DefClk) / BASE_DDR_CLK) & DRAM_CLOCK_MASK);
    
    if ((u4DramType == DDR_I_x1) ||(u4DramType == DDR_II_x1) ||
        (u4DramType == DDR_III_x1) ||(u4DramType == SDR_x1))
    {
        TCM_DRAM_FLAGS |= (1 << FORCE32_SHIFT);
    }

    u4Ret = DRVCUST_InitGet(eFlagFullFreq);
    TCM_DRAM_FLAGS |= ((u4Ret & 0x1) << FULLFREQ_SHIFT);    
    if (u4DefClk <= CLK_400MHZ)
    {
        TCM_DRAM_FLAGS &= ~(DRAM_FULLFREQ);
        TCM_DRAM_FLAGS |= (0x1 << FULLFREQ_SHIFT);
    }    

    u4Ret = DRVCUST_InitGet(eFlagDDRQfp);
    TCM_DRAM_FLAGS |= ((u4Ret & 0x1) << QFP_SHIFT);

    u4Ret = DRVCUST_InitGet(eDdrCL);
    TCM_DRAM_FLAGS |= ((u4Ret & 0xF) << CL_SHIFT);    
    if (u4DramType == DDR_I_x1)
    {
        TCM_DRAM_FLAGS &= ~(DRAM_DDR_CL_MASK);
        TCM_DRAM_FLAGS |= (UINT32)((UINT32)8 << (UINT32)CL_SHIFT);
    }    

    u4Ret = DRVCUST_InitGet(eFlagSingleDQS);
    TCM_DRAM_FLAGS |= ((u4Ret & 0x1) << SINGLEDQS_SHIFT);
    
    u4Ret = DRVCUST_InitGet(eDramColAddr);
    u4Ret = (u4Ret >= COL_ADDR_MAX) ? COL_ADDR_BIT_10 : u4Ret;
    TCM_DRAM_FLAGS |= (((u4Ret ) << COLADDR_SHIFT) & DRAM_COL_ADDR);
    if (u4DramType == SDR_x1)
    {
        TCM_DRAM_FLAGS &= ~(DRAM_COL_ADDR);
        TCM_DRAM_FLAGS |= (UINT32)((UINT32)8 << (UINT32)COLADDR_SHIFT);
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
#if defined(CC_MTK_PRELOADER) && defined(DRAM_DEBUG)
void CLK_QueryDramSetting(UINT8 *szString, UINT32 u4Stage)
{
    UINT32 u4Val;
    UINT32 i, u4CpuClk;
    UCHAR uc;
    
    u4CpuClk = CLK_200MHZ;
    // Wait a moment for ESC to enter setup menu
    CHIP_DisplayString((const INT8 *)szString);
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
            case 'C': case 'c':
                u4Val = TCMGET_COLADDR()+1;
                if (u4Val >= COL_ADDR_MAX)
                {
                    u4Val = (COL_ADDR_MIN+1);
                }
                TCM_DRAM_FLAGS &= ~(DRAM_COL_ADDR);
                TCM_DRAM_FLAGS |= (((u4Val) << COLADDR_SHIFT) & DRAM_COL_ADDR);
                break;
            case 'T': case 't':
                u4Val = TCMGET_DDR_CL();
                u4Val++;
                if (u4Val > 9)
                {
                    u4Val = 2;
                }
                TCM_DRAM_FLAGS &= ~(DRAM_DDR_CL_MASK);
                TCM_DRAM_FLAGS |= ((u4Val & 0xF) << CL_SHIFT);
                break;
            case '_': case '-':
                CHIP_DisplayString("Enter DRAM clock:\n");
                while (!SerInByte(&uc)) { }
                // Ascii to integer.
                u4Val = (uc-0x30)*100;
                while (!SerInByte(&uc)) { }
                u4Val += (uc-0x30)*10;
                while (!SerInByte(&uc)) { }
                u4Val += (uc-0x30);

                CHIP_DisplayString("Set DRAM clock = ");
                CHIP_DisplayInteger(u4Val);
                CHIP_DisplayString(" MHz.\n");

                TCM_DRAM_FLAGS &= ~(DRAM_CLOCK_MASK);
                TCM_DRAM_FLAGS |= (u4Val & DRAM_CLOCK_MASK);
                break;
                
            default:
                break;
            }
        }

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
                u4Val = (DRAM_READ32(0x300) >> 28) & 0xF;
                u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
                u4Val = u4Val << 28;
                u4Val = (DRAM_READ32(0x300) & 0x0FFFFFFF) | u4Val;
                DRAM_WRITE32(0x300, u4Val);
                break;
            case 'S': case 's':
                u4Val = (DRAM_READ32(0x300) >> 24) & 0xF;
                u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
                u4Val = u4Val << 24;
                u4Val = (DRAM_READ32(0x300) & 0xF0FFFFFF) | u4Val;
                DRAM_WRITE32(0x300, u4Val);
                break;
            case 'Z': case 'z':
                u4Val = (DRAM_READ32(0x308) >> 28) & 0xF;
                u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
                u4Val = u4Val << 28;
                u4Val = (DRAM_READ32(0x308) & 0x0FFFFFFF) | u4Val;
                DRAM_WRITE32(0x308, u4Val);
                break;
            case 'X': case 'x':
                u4Val = (DRAM_READ32(0x308) >> 24) & 0xF;
                u4Val = (u4Val == 0xF) ? 0 : (u4Val + 1);
                u4Val = u4Val << 24;
                u4Val = (DRAM_READ32(0x308) & 0xF0FFFFFF) | u4Val;
                DRAM_WRITE32(0x308, u4Val);
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
            case 'D': case 'd':
                u4Val = (DRAM_READ32(0x304) >> 0) & 0x0F;
                u4Val = (u4Val == 0x0F) ? 0 : (u4Val + 1);
                u4Val = u4Val << 0;
                u4Val = (DRAM_READ32(0x304) & 0xFFFFFFF0) | u4Val;
                DRAM_WRITE32(0x304, u4Val);
                break;
            case 'L': case 'l':
                u4Val = (DRAM_READ32(0x304) >> 4) & 0x0F;
                u4Val = (u4Val == 0x0F) ? 0 : (u4Val + 1);
                u4Val = u4Val << 4;
                u4Val = (DRAM_READ32(0x304) & 0xFFFFFF0F) | u4Val;
                DRAM_WRITE32(0x304, u4Val);
                break;
                
            default:
                break;
            }
        }

        switch(uc)
        {        
        case 'B': case 'b':
            CHIP_DisplayString("RS232 leave transparent mode! Console stop!\n");
            //SerNormalMode();
            *((volatile UINT32*)(IO_VIRT + 0x8E04)) = 0;                        
            while (1) { }
        case '0': // to Exit.
            break;

        default:
            Printf("\r%c\n", uc);
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
#endif /* #if defined(CC_MTK_PRELOADER) && defined(DRAM_DEBUG) */

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

#ifdef CC_MTK_PRELOADER
UINT32 CLK_SetDdrOnNorFlash(UINT32 u4DefClk)
{
    UINT32 u4Clock = u4DefClk;
    
//    PDWNC_WRITE32(REG_RW_CLKPWD, PDWNC_READ32(REG_RW_CLKPWD)|PWRDWN_CLOCK_LOCK);//vIO32WriteFldAlign(PDWNC_CLKPDN, 0x1, FLD_LOCK);
    HAL_WRITE32(PDWNC_CLKPDN, HAL_READ32(PDWNC_CLKPDN)|PWRDWN_CLOCK_LOCK);

    // TCMSET_SYSACON(0);
    if (TCM_DRAM_FLAGS & SYSTEM_AC_ON)
    {
        // CHIP_DisplayString("\nAC ON flag disable\n");
        TCM_DRAM_FLAGS &= ~(SYSTEM_AC_ON);  // no call TCMSET_SYSACON, because it will set checksum.
    }

    _CLKResetTCM(u4DefClk);

    CHIP_DisplayString("\n");

#ifdef DRAM_DEBUG
    // User's chance to modify the default settings at TCM_DRAM_SETUP.
    CLK_QueryDramSetting((UINT8 *)"First setup dram menu:", DRAM_CHANGE_CLK_SETTING);
#endif /* DRAM_DEBUG */

    u4Clock = TCMGET_DDR_CLK();

    CLK_DmpllSpectrum(u4Clock);

#ifdef CC_MT8223L
    // Get the DRAM settings.
    CLK_SetDmpll(u4Clock, FALSE);
    CLK_SetMempll(u4Clock, FALSE);
#else
    CLK_SetDmpll(u4Clock, TRUE);
    CLK_SetMempll(u4Clock, TRUE);
#endif    

    // u4Clock is the final clock setting.
    Printf("\nDRAM data rate = %d Mbps.\n", u4Clock / 1000000);

    return 0;
}
#endif /* #ifdef CC_MTK_PRELOADER */

UINT32 CLK_SetDdr()
{
#ifdef CC_MTK_PRELOADER

#if (!defined(CC_NAND_BOOT) && !defined(CC_ROM_BOOT)) || defined(CC_FPGA)
    if( BSP_IsFPGA() )
        return CLK_SdrCalibrate();
    else
#endif /* !CC_NAND_BOOT || !CC_ROM_BOOT */

    return CLK_SetDdrOnNorFlash(DRVCUST_InitGet(eDdrClock));
#else
    return 0;
#endif /* #ifdef CC_MTK_PRELOADER */
}

#ifndef CC_MTK_PRELOADER
UINT32 BSP_IsSystemAcOn()
{
    return IS_SYS_ACON();
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

#if 1
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


#if !defined (CC_NAND_BOOT) || !defined(CC_MTK_PRELOADER)

#ifndef CC_MTK_LOADER
//-----------------------------------------------------------------------------
/** BSP_GetMemSize()
 *  @return the total dram size.
 */
//-----------------------------------------------------------------------------
UINT32 BSP_GetMemSize()
{
#ifdef CC_MTK_LOADER
    UINT32 g_u4DramSize;
#endif
#ifdef FAKE_MEM_SIZE
    return (g_u4DramSize > FAKE_MEM_SIZE) ? FAKE_MEM_SIZE : g_u4DramSize;
#else
    return g_u4DramSize;
#endif
}
#endif /* !CC_MTK_LOADER */

#ifndef __KERNEL__
//lint -save
//lint --e{550} Symbol not accessed
//lint --e{529} Symbol not subsequently referenced

//-----------------------------------------------------------------------------
// Below is cut from hal_926.c
//-----------------------------------------------------------------------------

#pragma arm

void HalDisableITCM(void)
{
    UINT32 r = 0;

    __asm
    {
        MCR     p15, 0, r, c9, c1, 1       // Write back ITCM control register
    }
}

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


#if defined(CC_MTK_PRELOADER) && defined(NDEBUG)
#else /* defined(CC_MTK_PRELOADER) && defined(NDEBUG) */
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
#endif /* defined(CC_MTK_PRELOADER) && defined(NDEBUG) */

#endif /* __KERNEL__ */
#endif /* CC_NAND_BOOT */
