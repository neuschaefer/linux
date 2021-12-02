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
#include "x_chip_id.h"
#include "drvcust_if.h"
#include "c_model.h"
#include "hw_ifpll.h"


extern void CHIP_DisplayChar(UINT32 u4Char);
extern void CHIP_DisplayString(const CHAR *szString);

void CHIP_ClockSetting(void);
void CHIP_BimInit(void);
void CHIP_1stBootRegInit(void);


// Serial flash
//#define SFLASH_CFG1_REG         0x0760
//#define SFLASH_CKGEN_REG        0x07B0
//#define SFLASH_SAMPLE_EDGE_REG  0x07B4

void CHIP_Delay_us(UINT32 u4Micros)
{
	#ifdef CC_FPGA
    volatile UINT32 i;

    for (i = 0; i < (27*u4Micros)*100; i++)
    {
        ;
    }
    #else
    UINT32 u4Xtal;
    UINT64 u8Current;
    UINT64 u8TimeStop;

    switch(XTAL_STRAP_MODE) {
    case SET_XTAL_27MHZ:
        u4Xtal = 27*u4Micros;
        break;
    case SET_XTAL_54MHZ:
        u4Xtal = 54*u4Micros;
        break;
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
    #endif
}

void CHIP_ClockSetting()
{
#ifndef CC_FPGA

    BIM_WRITE32(REG_RW_BTIME, 0x11); // delay register read path 1T

    #if 0
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
    #endif

    CHIP_Delay_us(500);
    
    vIO32WriteFldAlign(REG_MEMPLL0, 0x1, FLD_RG_MEMPLL_DIVEN);
    
    // Set for MT5396 DVDD12=1.2V
    vIO32WriteFldAlign(REG_MEMPLL1, 0x0, FLD_RG_MEMPLL_V11EN);
    vIO32WriteFldMulti(REG_PLL_GROUP_CFG0, P_Fld(0,FLD_RG_XPLL_V11EN)|P_Fld(0,FLD_RG_VGAPLL_V11EN));
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG1, 0x0, FLD_RG_MJCPLL_V11EN);
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG2, 0x0, FLD_RG_SYSPLL_V11EN);
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG3, 0x0, FLD_RG_HADDS2_V11EN);
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG5, 0x0, FLD_RG_VDECPLL_V11EN);

    // Enable PLL watch dog
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG3, 0x01, FLD_RG_SYSPLL_RESERVE);
    vIO32WriteFldAlign(REG_IFPLL_CFG2, 0x01, FLD_RG_ULJPLL_RESERVE);

    // PLL 4-stage Phase VCO
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG2, 0x0, FLD_RG_SYSPLL_LF);
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG3, 0x0, FLD_RG_HADDS2_LF);

    // Set PI cap
    vIO32WriteFldMulti(REG_PLL_GROUP_CFG8, P_Fld(7,FLD_RG_DDDS1_PI_C)|P_Fld(7,FLD_RG_DDDS2_PI_C)|P_Fld(7,FLD_RG_DMSS_PI_C));
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG6, 0x07, FLD_RG_XDDS_PI_C);
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG9, 0x07, FLD_RG_HADDS1_PI_C);
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, IO32ReadFldAlign(REG_PLL_GROUP_CFG4, FLD_RG_HADDS2_DDS_DMY) | 0x07, FLD_RG_HADDS2_DDS_DMY);
    
    // 1. Init XPLL, SYSPLL, MJCPLL, VDECPLL
    // 27MHz xtal clock to IFPLL/PLLGP/CVBS/VGA enable
    // w 0xF0028900 0x038E0000
    vIO32WriteFldMulti(PDWNC_CONCFG0,P_Fld(1,FLD_RG_CON_XTAL2PLLGP_EN)|P_Fld(1,FLD_RG_XTAL_AR_EN)|P_Fld(1,FLD_RG_XTAL_DMSEL)|P_Fld(0,FLD_RG_CON_RESERVE));

    // set CPU/BUS/SRAM to xtal before PLL setting
    vIO32WriteFldMulti(CKGEN_CPU_CKCFG, P_Fld(0,FLD_BUS_CK_SEL)|P_Fld(0,FLD_CPU_CK_SEL));
    vIO32WriteFldAlign(CKGEN_ABIST_SRAM_CKCFG, 0x0, FLD_ABIST_SRAM_CK_SEL);

    // Set PLLGP_BIAS_PWD = 1
    // w 0xF0061024 0x8000C800
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG9, 0x1, FLD_RG_PLLGP_BIAS_PWD);

    // Set XPLL for 675 Mhz
    // XPLL = Xtal(27Mhz) *  XPLL_FBDIV(25)
    // Default XPLL = 27Mhz * 25

    // Select the source of SYSPLL, MJCPLL and VDECPLL from Xtal
    // w 0xF0061028 0x00440000
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x0, FLD_RG_XTAL_SEL);

    // Set SYSPLL for 648 MHz
    // SYSPLL = Xtal * SPLL_FBDIV2 * (SPLL_FBDIV + 1) / SPLL_PREDIV / SPLL_POSDIV
    // Default SYSPLL = 27Mhz * (1) * (23 + 1) / 1 / 1
    // w 0xF0061008 0x0000D017
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG2, 0x17, FLD_RG_SYSPLL_FBDIV);
    
    // Set MJCPLL for 810 MHz
    // MJCPLL = Xtal * MJCPLL_FBDIV2 * (MJCPLL_FBDIV + 1) / MJCPLL_PREDIV / MJCPLL_POSDIV
    // Default MJCPLL = 27Mhz * 2 * (29 + 1) / 2 / 1
    // 0x20 = 891mhz (for IC_VER_5396_AA)
    // 0x21 = 918mhz (for IC_VER_5396_AB later)
    // w 0xF0061004 0x0180D4A0
    if (BSP_GetIcVersion() == IC_VER_5396_AA)
    {
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG1, 0x20, FLD_RG_MJCPLL_FBDIV);
    }
    else
    {
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG1, 0x21, FLD_RG_MJCPLL_FBDIV);
    }
    // w 0xF0061008 0x0100D017
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG2, 0x02, FLD_RG_MJCPLL_DIVEN);

    // Set VDECPLL for 378MHz (for IC_VER_5396_AA)
    //                 405MHz (for IC_VER_5396_AB later)
    // MJCPLL = Xtal * VDECPLL_FBDIV2 * (VDECPLL_FBDIV + 1) / VDECPLL_PREDIV / VDECPLL_POSDIV
    // Default MJCPLL = 27Mhz * 2 * (27 + 1) / 2 / 2
    if (BSP_GetIcVersion() == IC_VER_5396_AA)
    {
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG5, 0x1B, FLD_RG_VDECPLL_FBDIV);
    }
    else
    {
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG5, 0x1D, FLD_RG_VDECPLL_FBDIV);
    }
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG6, 0x01, FLD_RG_VDECPLL_DIVEN);

    // Power on PLL
    // Set PLLGP_BIAS_PWD = 0
    // w 0xF0061024 0x80004800
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG9, 0x0, FLD_RG_PLLGP_BIAS_PWD);
    CHIP_Delay_us(10);

    // Power on XPLL, MJCPLL, SYSPLL, VDECPLL
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG0, 0x0, FLD_RG_XPLL_PWD);
    // w 0xF0061004 0x018054A0
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG1, 0x0, FLD_RG_MJCPLL_PWD);
    // w 0xF0061008 0x01005017
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG2, 0x0, FLD_RG_SYSPLL_PWD);
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG5, 0x0, FLD_RG_VDECPLL_PWD);

    // Enable DDS setting
    vIO32WriteFldMulti(REG_PLL_GROUP_CFG8,P_Fld(1,FLD_RG_DDDS1_PWDB)|P_Fld(1,FLD_RG_DDDS2_PWDB)|P_Fld(1,FLD_RG_DMSS_PWDB)|P_Fld(1,FLD_RG_HADDS1_PWDB));
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG6, 0x1, FLD_RG_XDDS_PWDB);
            
    // Set FLD_RG_XTAL_DIV_RSTB = 1
    // w 0xF0061028 0x004C0000
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x1, FLD_RG_XTAL_DIV_RSTB);

    // Set PLLGP_BIAS_RST = 0
    CHIP_Delay_us(20);
    // w 0xF0061024 0x80000800
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG9, 0x0, FLD_RG_PLLGP_BIAS_RST);

    // 2. Set IFPLL
    // Load BAIS current
    vIO32WriteFldAlign(REG_IFPGA_CFG1, 0x0, FLD_RG_IFBG_PWD);   // IFBG_PWD=0b
    vIO32WriteFldAlign(REG_IFPGA_CFG0, 0x1, FLD_RG_IFIBIAS);    // IFIBIAS[1:0]=01b

    // Load PLL 3456MHz default
    vIO32Write4B(REG_IFPLL_CFG0, 0x9F741900);
    vIO32Write4B(REG_IFPLL_CFG1, 0x2E160300);
    vIO32Write4B(REG_IFPLL_CFG2, 0xF0010000);

    // Calibration
    vIO32WriteFldAlign(REG_IFPLL_CFG1, 0x1, FLD_RG_ULJPLL_MONEN);       // ULJPLL_MONEN=1b
    vIO32WriteFldAlign(REG_IFPLL_CFG1, 0x1, FLD_RG_ULJPLL_AUTOK_VCO);   // ULJPLL_AUT0K_VCO=1b
    vIO32WriteFldAlign(REG_IFPLL_CFG1, 0x1, FLD_RG_ULJPLL_AUTOK_LOAD);  // ULJPLL_AUT0K_LOAD=1b
    vIO32WriteFldAlign(REG_IFPLL_CFG1, 0x1, FLD_RG_ULJPLL_LOAD_RSTB);   // ULJPLL_LOAD_RSTB=1b
    vIO32WriteFldAlign(REG_IFPLL_CFG0, 0x0, FLD_RG_ULJPLL_PWD);         // ULJPLL_PWD=0b
    CHIP_Delay_us(100);
    vIO32WriteFldAlign(REG_IFPLL_CFG1, 0x0, FLD_RG_ULJPLL_AUTOK_VCO);   // ULJPLL_AUT0K_VCO=0b
    vIO32WriteFldAlign(REG_IFPLL_CFG1, 0x0, FLD_RG_ULJPLL_DIV3_RSTB);   // ULJPLL_DIV3_RSTB=0b
    vIO32WriteFldAlign(REG_IFPLL_CFG1, 0x0, FLD_RG_ULJPLL_BYPASS);      // ULJPLL_BYPASS=0b

    // 3. init USB PLL
    // already do this in step0
    CHIP_Delay_us(500);
    // w 0xf000d25c 0x00010001
    // w 0xF000D284 0x1
    // w 0xf0059800 0x0009486E
    // w 0xf0059a00 0x0009486E
    // w 0xf0059868 0x02000000
    vIO32Write4B(0xf000d25c,0x00010001);
    vIO32Write4B(0xf000d284,0x00000001);
    vIO32Write4B(0xf0059800,(u4IO32Read4B(0xf0059800)|0x00004000)); //RG_USB20_INTR_EN=1b
    vIO32Write4B(0xf0059a00,(u4IO32Read4B(0xf0059a00)|0x00004000)); //RG_USB20_INTR_EN=1b
    vIO32Write4B(0xf0059868,(u4IO32Read4B(0xf0059868)&(~0x00040000))); 
    
#ifdef NEED_TO_FIX
    // init Ethernet PLL
    vIO32Write4B(CKGEN_AFEPLLCFG18, 0x1705031F);
    vIO32Write4B(CKGEN_AFEPLLCFG19, 0x00742000);
    CHIP_Delay_us(1);
    vIO32Write4B(CKGEN_AFEPLLCFG19, 0x00F60000);
    CHIP_Delay_us(100);
#endif

    // 4. Select CPU clock to MJCPLL and Bus clock
    // cpu clock = cpupll (810Mhz), bus clock = 216Mhz
    /*
    Clock source selection[3:0]
        0: xtal_ck
        1: cpupll_ck
        2: enetpll_ck (675MHZ)
        3: syspll_ck (648MHz)
        4: usbpll_ck (480MHz)
        5: sawlesspll_ck (432MHz)
        6: cpupll_d2_ck
        7: usbpll_d2_ck (240MHz )
        8: sawlesspll_d2_ck (216MHz )
        9: sawlesspll_d3_ck (144MHz )
        10: usbpll_d4_ck (120MHz )
        11: sawlesspll_d4_ck (108MHZ)
        12: enetpll_d4_ck (168MHZ)
        13: MEM_CK
        14: slow_ck
        15: bist_ck
    BUS_CK selection[10:8]
        0: bus_ck = xtal_ck
        1: bus_ck = sawlesspll_d2_ck
        2: bus_ck = usbpll_d2p5_ck
        3: bus_ck = syspll_d4_ck
        4: bus_ck = sawlesspll_d3_ck
        5: bus_ck = usbpll_d4_ck
        6: bus_ck = syspll_d3_ck
        7: bus_ck = slow_ck
    */
    // w 0xF000D208 0x00008601
    vIO32WriteFldMulti(CKGEN_CPU_CKCFG, P_Fld(1,FLD_BUS_CK_SEL)|P_Fld(1,FLD_CPU_CK_SEL));

    // 5. set MCK to normal clcok (mempll)
    vIO32WriteFldAlign(CKGEN_MEM_CKCFG, 0x3, FLD_M_CK_TST);

    // 6. set common clk
    // C27M = sawless / 16
    vIO32WriteFldAlign(CKGEN_XTAL_CKCFG, 0x1, FLD_C27M_CK);

    // 7. setup vopll
    // vopll source xtal    
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 2, FLD_RG_DDDS1_CKSEL);

    // vopll setting
    vIO32Write4B(REG_VOPLL_CFG0, 0x22879204);
    vIO32Write4B(REG_VOPLL_CFG1, 0x84020020);
    vIO32Write4B(REG_VOPLL_CFG2, 0x00332E84);
    vIO32Write4B(REG_VOPLL_CFG3, 0x00008754);
    vIO32Write4B(REG_VOPLL_CFG4, 0x00800000);
	
    // Power on
    CHIP_Delay_us(10);
    vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_PWD); 
    vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_BG_LVDS_PWD);
    vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_BG_VB1_TX_PWD);
    vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_BIAS_BYP);

    // OCLK and TCLK Common
    vIO32WriteFldAlign(CKGEN_MBIST_CK_SEL, 0, FLD_BIST_VPCLK_SEL);
	 
    // OCLK Path
    vIO32WriteFldAlign(CKGEN_VPCLK_CFG, FLD_OCLK_SEL_LVDS, FLD_OCLK_SEL); 	
    vIO32WriteFldAlign(CKGEN_OCLK_TEST, 0, FLD_OCLK_TSTSEL);

    // TCLK Path
    vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 1, FLD_TCLK_SEL);
    vIO32WriteFldAlign(CKGEN_OCLK_TEST, 0, FLD_TCLK_TSTSEL);

    // 8. setup serial flash pll
    //   0: 3.375 MHz   1: 5.4 MHz      2: 6.75 MHz     3: 9 MHz
    //   4: 13.5 MHz    5: 27 MHz       6: 30 MHz       7: 48 MHz
    //   8: 54 MHz      9: 72 MHz       10: 108 MHz     11: 0.84375 MHz
    // w 0xF00087B0 5
    IO_WRITE32(0, 0xF00087B0, 5);

    // 9. setup sram clock
    // w 0xF000D2D4 0x1
    vIO32WriteFldAlign(CKGEN_ABIST_SRAM_CKCFG, 0x1, FLD_ABIST_SRAM_CK_SEL);

    // 10. setup nand pll (0: XTAL 27Mhz, 8: 216MHz, 9: 144MHz)
    // w 0xF000D260 6
    vIO32WriteFldAlign(CKGEN_NFI_CKCFG, 0x6, FLD_NFI_CK_SEL);

    // set XDDS source to syspll
//    vIO32WriteFldAlign(CKGEN_XDDS2_CKCFG, 1, FLD_XDDS_CLK2_SEL);

    // enable TCLK/SS_OCLK/OCLK
//    vIO32WriteFldAlign(CKGEN_MBIST_CK_SEL, 0, FLD_BIST_VPCLK_SEL);

    // 11. setup PCI-Express clock
    /*
    0: xtal_ck = 27Mhz
    01: enetpll_d5_ck = 135Mhz
    02: sawlesspll_d3_ck = 144 Mhz.
    03: usbpll_d3_ck = 160Mhz.
    04: usbpll_d4_ck = 120Mhz
    05: sawlesspll_d2p5_ck = 172.8 Mhz
    06: slow_ck
    07: usbpll_d5_ck = 96 Mhz    
    */
    vIO32WriteFldAlign(CKGEN_PCIE11_MAC_CKCFG, 0x4, FLD_PCIE11_MAC_SEL);

    if(!IS_Support3DGPU())
    {
        IO_WRITE32(0xF0040000, 0xE010, 0x1);
    }
    else
    {
        CKGEN_WRITE32(0x314, 0xD);
    }

#endif
}

void CHIP_BimInit()
{
    // set bus access timeout to avoid register access halting CPU.
    BIM_WRITE32(REG_RW_TIMEOUT_CTRL, TO_FLASH_CNT | TO_DRAM_CNT | TO_IOBUS_CNT | TO_FLASH_EN | TO_DRAM_EN | TO_IOBUS_EN);
    //BIM_WRITE32(REG_RW_BTIME, TO_IORW | TO_IOWW);

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
}

