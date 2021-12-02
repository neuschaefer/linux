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

/** @file ckgen.c
 *  ckgen.c contains functions to control/query ckgen PLLs clock.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
#include "x_ckgen.h"
#include "x_hal_5381.h"
#include "x_hal_arm.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_dram.h"
#include "x_timer.h"
#include "drvcust_if.h"
#include "hw_vdoin.h"
#include "hw_ckgen.h"
#include "hw_ifpll.h"

#ifndef CC_MTK_LOADER
//#include "tve_hw.h"
#include "hw_vfe.h"
#include "nim_drvif.h"
#include "x_drv_cli.h"
#endif

//-----------------------------------------------------------------------------
// Configuration
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define LOG(level, fmt...)  // Printf(fmt)

//-----------------------------------------------------------------------------
// Static routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _CalString() to get the divider of the calibrated PLL.
 *  @retval     Return the selective count of the PLL.
 */
//-----------------------------------------------------------------------------
static CHAR * _CalString(CAL_SRC_T eSource)
{
    switch(eSource)
    {
        case CAL_SRC_SAWLESSPLL:
            return "SAWLESSPLL";
        case CAL_SRC_CPUPLL:
            return "CPUPLL";
        case CAL_SRC_SIF_CK:
            return "SIF_CK";
        case CAL_SRC_AD_APLL1_CKOUT300M_CK:
            return "AD_APLL1_CKOUT300M_CK";
        case CAL_SRC_AD_APLL1_CKOUT108M_CK:
            return "AD_APLL1_CKOUT108M_CK";
        case CAL_SRC_AD_APLL2_CKOUT300M_CK:
            return "AD_APLL2_CKOUT300M_CK";
        case CAL_SRC_AD_APLL2_CKOUT108M_CK:
            return "AD_APLL2_CKOUT108M_CK";
        case CAL_SRC_AD_PCLK:
            return "AD_PCLK";
        case CAL_SRC_M_CK:
            return "M_CK";
        case CAL_SRC_TCLK:
            return "TCLK";
        case CAL_SRC_AD_APLL2_FBOUT27M_CK:
            return "AD_APLL2_FBOUT27M_CK";
        case CAL_SRC_AD_USBPLL_480M:
            return "AD_USBPLL_480M";
        case CAL_SRC_USBPLL:
            return "SRC_USBPLL";        
        case CAL_SRC_AD_USBPLL_480M_2P:
            return "AD_USBPLL_480M_2P";
        case CAL_SRC_AD_ETHPLL_CLK675M:
            return "AD_ETHPLL_CLK675M";
        case CAL_SRC_XTAL:
            return "XTAL";
        case CAL_SRC_AD_ADC_CKOUT:
            return "AD_ADC_CKOUT";
        case CAL_SRC_AD_CVBSADC_CKIND:
            return "AD_CVBSADC_CKIND";
        case CAL_SRC_AD_HDMI_1_PLL340M_CK:
            return "AD_HDMI_1_PLL340M_CK";
        case CAL_SRC_AD_HDMI_1_PIX340M_CK:
            return "AD_HDMI_1_PIX340M_CK";
        case CAL_SRC_AD_HDMI_1_DEEP170M_CK:
            return "AD_HDMI_1_DEEP170M_CK";
        case CAL_SRC_AD_DDDS2_VSP:
            return "AD_DDDS2_VSP";
        case CAL_SRC_AD_DDDS1_VSP:
            return "AD_DDDS1_VSP";
        case CAL_SRC_TCLK_DIV:
            return "TCLK_DIV";
        case CAL_SRC_OCLK:
            return "OCLK";
        case CAL_SRC_OCLK_DIV:
            return "OCLK_DIV";
        case CAL_SRC_LVDS_CLK_CTS:
            return "LVDS_CLK_CTS";
        case CAL_SRC_CPUPLL_D2_CLK:
            return "CPUPLL_D2_CLK";
        case CAL_SRC_AD_SOGY_ADC_CKOUT:
            return "AD_SOGY_ADC_CKOUT";
        case CAL_SRC_AD_CKT_B:
            return "AD_CKT_B";
        case CAL_SRC_AD_FB_OUT:
            return "AD_FB_OUT";
        case CAL_SRC_AD_SOGY_OUT_MON:
            return "AD_SOGY_OUT_MON";
        case CAL_SRC_AD_SOGY_OUT:
            return "AD_SOGY_OUT";
        case CAL_SRC_AD_VSYNC_OUT:
            return "AD_VSYNC_OUT";
        case CAL_SRC_AD_HSYNC_OUT:
            return "AD_HSYNC_OUT";
        case CAL_SRC_AD_GPIO_PWM_0_0:
            return "AD_GPIO_PWM_0_0";
        case CAL_SRC_AD_GPIO_PWM_0_1:
            return "AD_GPIO_PWM_0_1";
        case CAL_SRC_AD_GPIO_PWM_0_2:
            return "AD_GPIO_PWM_0_2";
        case CAL_SRC_AD_GPIO_PWM_0_3:
            return "AD_GPIO_PWM_0_3";
        case CAL_SRC_AD_GPIO_PWM_0_4:
            return "AD_GPIO_PWM_0_4";
        case CAL_SRC_AD_GPIO_PWM_0_5:
            return "AD_GPIO_PWM_0_5";
        case CAL_SRC_AD_GPIO_PWM_0_6:
            return "AD_GPIO_PWM_0_6";
        case CAL_SRC_AD_GPIO_PWM_0_7:
            return "AD_GPIO_PWM_0_7";
        case CAL_SRC_AD_VOPLL_TEST_CLK:
            return "AD_VOPLL_TEST_CLK";
        case CAL_SRC_AD_EVEN_CLK_DIV_125M_D11:
            return "AD_EVEN_CLK_DIV_125M_D11";
        case CAL_SRC_AD_ODD_CLK_DIV_125M_D11:
            return "AD_ODD_CLK_DIV_125M_D11";
        case CAL_SRC_AD_CLKDIG_D11:
            return "AD_CLKDIG_D11";
        case CAL_SRC_AD_CKOUT_DEMOD:
            return "AD_CKOUT_DEMOD";
        case CAL_SRC_AD_SRV_SPL:
            return "AD_SRV_SPL";
        case CAL_SRC_AD_CKT_A:
            return "AD_CKT_A";
        case CAL_SRC_AD_HDMI_2_PLL340M_CK:
            return "AD_HDMI_2_PLL340M_CK";
        case CAL_SRC_AD_HDMI_2_DEEP170M_CK:
            return "AD_HDMI_2_DEEP170M_CK";
        case CAL_SRC_AD_HDMI_2_PIX340M_CK:
            return "AD_HDMI_2_PIX340M_CK";
        case CAL_SRC_AD_HDMI_3_PLL340M_CK:
            return "AD_HDMI_3_PLL340M_CK";
        case CAL_SRC_AD_HDMI_4_DEEP170M_CK:
            return "AD_HDMI_4_DEEP170M_CK";
        case CAL_SRC_AD_HDMI_5_PIX340M_CK:
            return "AD_HDMI_5_PIX340M_CK";
        case CAL_SRC_AD_CLKDIG_CTS_D11:
            return "AD_CLKDIG_CTS_D11";
        case CAL_SRC_AD_CVBSADC_CKOUT:
            return "AD_CVBSADC_CKOUT";
        case CAL_SRC_AD_VOPLL_DPIX_CK:
            return "AD_VOPLL_DPIX_CK";
        case CAL_SRC_AD_VOPLL_TCON_CK:
            return "AD_VOPLL_TCON_CK";
        case CAL_SRC_AD_PPLL_DPIX_CK:
            return "AD_PPLL_DPIX_CK";
        case CAL_SRC_AD_XDDS_CLK:
            return "AD_XDDS_CLK";
        case CAL_SRC_AD_MJCPLL_REFCLK_BUF:
            return "AD_MJCPLL_REFCLK_BUF";
        case CAL_SRC_AD_MJCPLL_FBCLK_BUF:
            return "AD_MJCPLL_FBCLK_BUF";
        case CAL_SRC_AD_PLLGP_TSTCK:
            return "AD_PLLGP_TSTCK";
        case CAL_SRC_AD_HADDS2_FBOUT27M_CK:
            return "AD_HADDS2_FBOUT27M_CK";
        case CAL_SRC_AD_VDECPLL_FBCLK_BUF:
            return "AD_VDECPLL_FBCLK_BUF";
        case CAL_SRC_AD_VDECPLL_REFCLK_BUF:
            return "AD_VDECPLL_REFCLK_BUF";
        case CAL_SRC_AD_PL_RCLK250:
            return "AD_PL_RCLK250";
        case CAL_SRC_AD_MEMPLL_MONCLK_PRE:
            return "AD_MEMPLL_MONCLK_PRE";
        case CAL_SRC_AD_MEMPLL_FBCLK_BUF_D16:
            return "AD_MEMPLL_FBCLK_BUF_D16";
        case CAL_SRC_AD_MEMPLL_REFCLK_BUF_D16:
            return "AD_MEMPLL_REFCLK_BUF_D16";
        case CAL_SRC_MIO_CK_P0_DIV8:
            return "MIO_CK_P0_DIV8";
        case CAL_SRC_MIO_CK_P0_DIV4:
            return "MIO_CK_P0_DIV4";
        case CAL_SRC_AD_SYNC_R_CK:
            return "AD_SYNC_R_CK";
        case CAL_SRC_AD_SYNC_L_CK:
            return "AD_SYNC_L_CK";
        case CAL_SRC_AD_CVBSADC_CKIND_2:
            return "AD_CVBSADC_CKIND_2";
        case CAL_SRC_AD_USB20_MONCLK_3P:
            return "AD_USB20_MONCLK_3P";
        case CAL_SRC_AD_USB20_SESSEND_2P:
            return "AD_USB20_SESSEND_2P";
        case CAL_SRC_AD_USB20_MONCLK:
            return "AD_USB20_MONCLK";
        case CAL_SRC_AD_USB20_MONCLK_1P:
            return "AD_USB20_MONCLK_1P";
        case CAL_SRC_AD_USB20_MONCLK_2P:
            return "AD_USB20_MONCLK_2P";
        case CAL_SRC_SPL2CORE:
            return "SPL2CORE";
        case CAL_SRC_MON_CLK:
            return "MON_CLK";
        default:
            return "unknown PLL";
    }
}

static void _CalEnablePLL(CAL_SRC_T eSource)
{
    switch(eSource)
    {
        default:
            return;
    }
}

static void _CalSetPLL(CAL_SRC_T eSource)
{
    UNUSED(eSource);
}

static void _CalSetMN(CAL_SRC_T eSource, UINT32 u4Clock)
{
    switch(eSource)
    {
    default:
        break;
    }
}

//-----------------------------------------------------------------------------
/** _CalGetPLLCounter() to get the PLL clock
 *  @retval the return value is the kHz NOT Hz.
 */
//-----------------------------------------------------------------------------
UINT32 _CalGetPLLCounter(UINT32 u4CalSel)
{
    UINT32 u4Val, u4Mode, u4RefCount;
    
    // Reset CAL_CNT
    vIO32WriteFldAlign(CKGEN_PLLCALIB, 1, FLD_SOFT_RST_CAL);

    // Select mode, 0: 2048
    vIO32WriteFldAlign(CKGEN_PLLCALIB, 0, FLD_CAL_MODE);

    // Set CAL_SEL, 0 is clk/1, 1 is clk/2, 2 is clk/4
    vIO32WriteFldAlign(CKGEN_PLLCALIB, 0, FLD_CAL_SEL);

    // Select PLL source at REG_PLL_CALIB
/*    if ((u4CalSel >= CAL_SRC_AD_LVDS_AO0_TTLI0) &&
        (u4CalSel <= CAL_SRC_AD_LVDS_AECK_TTLI1)) {
        vIO32WriteFldAlign(CKGEN_MISC_CKCFG, u4CalSel - CAL_SRC_AD_LVDS_AO0_TTLI0, FLD_LVDS_TTLI_SEL);
        vIO32WriteFldAlign(CKGEN_PLLCALIB, CAL_SRC_LVDS_TTLI_DBGOUT, FLD_DBGCKSEL);
    }
    else {
    */
        vIO32WriteFldAlign(CKGEN_PLLCALIB, u4CalSel, FLD_DBGCKSEL);
    //}

    // Set PLL_CAL_TRIGGER = 1
    vIO32WriteFldAlign(CKGEN_PLLCALIB, 1, FLD_CAL_TRI);  

    // Wait calibration finishing.
    while (IO32ReadFldAlign(CKGEN_PLLCALIB, FLD_CAL_TRI)) { }

    // Calculate clock.
    HAL_Delay_us(10);
    u4Val = IO32ReadFldAlign(CKGEN_PLLCALIBCNT, FLD_CALI_CNT);
    
    u4Mode = IO32ReadFldAlign(CKGEN_PLLCALIB, FLD_CAL_MODE);

    switch(u4Mode) {
    case 0: u4RefCount = 11; break;
    case 1: u4RefCount = 16; break;
    case 2: u4RefCount = 20; break;
    case 3: u4RefCount = 24; break;
    default: u4RefCount = 11; break;
    }

    return (((GET_XTAL_CLK()/1000) * u4Val ) >> u4RefCount);
}

//=====================================================================
// Public

#if CKGENHW_DEBUG

void CKGENDB_IoWr(UINT32 u4Offset, UINT32 u4Value, INT8 *szFile, INT32 Line)
{
    LOG(_W, "%s(%d) write 0x%08x with val 0x%08x\n", szFile, Line, u4Offset, u4Value);
    IO_WRITE32(CKGEN_BASE, u4Offset, u4Value);
}

UINT32 CKGENDB_IoRd(UINT32 u4Offset, INT8 *szFile, INT32 Line)
{
    UINT32 u4Value;

    u4Value = IO_READ32(CKGEN_BASE, u4Offset);
    LOG(_R, "%s(%d) read 0x%08x getting val 0x%08x\n", szFile, Line, u4Offset, u4Value);
    return u4Value;
}

#endif /* CKGENHW_DEBUG */


//=====================================================================
// Interface

void BSP_CkgenInit()
{
    vIO32WriteFldAlign(CKGEN_PLLCALIB, 0, FLD_CAL_MODE);
}

BOOL BSP_Calibrate(CAL_SRC_T eSource, UINT32 u4Clock)
{
    BOOL fgRet = FALSE;
    UINT32 u4CalClk;
    UINT32 u4Crit;

    // Before Calibration, enable the PLL.
    _CalEnablePLL(eSource);

    _CalSetMN(eSource, u4Clock);

    switch(eSource)
    {
        case CAL_SRC_DMPLL: // 5368 ?
        case CAL_SRC_VOPLL: // 5368 ?
        case CAL_SRC_ADCPLL: // 5368 ?
            fgRet = TRUE;
            if (DRVCUST_InitGet(eFlagBootVerbose))
            {
                // Delay a 1 ms to make close loop clock stable
                HAL_Delay_us(1000);

                u4CalClk = _CalGetPLLCounter((UINT32)(eSource));
                Printf("%s now is %d kHz\n", _CalString(eSource), u4CalClk);
            }
            break;

        case SRC_FRAC_DIV_CLK:
            // TODO: support generic frequencies
            if (u4Clock != 25000000)
            {
                return TRUE;
            }

#if 0 // new_ckgen
            u4Crit = HalCriticalStart();
            // Set fractional divider
            //u4Reg = CKGEN_READ32(0x714);
            //u4Reg |= 0x20000000;
            //CKGEN_WRITE32(0x714, u4Reg);
            //u4Reg &= ~0x20000000;
            //CKGEN_WRITE32(0x714, u4Reg);
 //           vIO32WriteFldAlign(CKGEN_FRACT06, 1, FLD_NCPO_RST); // run
            vIO32WriteFldAlign(CKGEN_FRACT06, 0, FLD_NCPO_RST); // reset 
            
            //u4Reg &= ~0xC0008000;
            //u4Reg |= 0xA0000000;
            //CKGEN_WRITE32(0x714, 0x30004300);
            //CKGEN_WRITE32(0x714, u4Reg);
            vIO32WriteFldMulti(CKGEN_FRACT06, P_Fld(0, FLD_FRACT_2X_SEL) | P_Fld(2, FLD_SRC_SEL) | P_Fld(1, FLD_NCPO_RST));

            //CKGEN_WRITE32(0x700, 0x125C28F5);
            //CKGEN_WRITE32(0x704, 0xbfe10001);
            //CKGEN_WRITE32(0x708, 0xbfe10002);
            //CKGEN_WRITE32(0x70c, 0xbfe10001);
            //CKGEN_WRITE32(0x710, 0xbfe10002);
            vIO32Write4B(CKGEN_FRACT01, 0x125C28F5);
            vIO32Write4B(CKGEN_FRACT02, 0xbfe10001);
            vIO32Write4B(CKGEN_FRACT03, 0xbfe10002);
            vIO32Write4B(CKGEN_FRACT04, 0xbfe10001);
            vIO32Write4B(CKGEN_FRACT05, 0xbfe10002);
            HalCriticalEnd(u4Crit);
#else
            UNUSED(u4Crit);
#endif            
            break;

        default:
            return fgRet;
    }

    // Do the post settings.
    _CalSetPLL(eSource);

    return fgRet;
}


UINT32 BSP_GetClock(CAL_SRC_T eSource, UINT8* pu1Band, UINT16* pu2MS,
    UINT16* pu2NS, UINT16* pu2Counter)
{
    UINT32 u4Clock = 0;

    UNUSED(pu2MS);
    UNUSED(pu2NS);
    UNUSED(pu1Band);

    switch(eSource)
    {
        case CAL_SRC_SAWLESSPLL               : //= 0  ,
        case CAL_SRC_CPUPLL                   : //= 1  ,
        case CAL_SRC_SIF_CK                   : //= 2  ,
        case CAL_SRC_AD_APLL1_CKOUT300M_CK    : //= 3  ,
        case CAL_SRC_AD_APLL1_CKOUT108M_CK    : //= 4  ,
        case CAL_SRC_AD_APLL2_CKOUT300M_CK    : //= 5  ,
        case CAL_SRC_AD_APLL2_CKOUT108M_CK    : //= 6  ,
        case CAL_SRC_AD_PCLK                  : //= 7  ,
        case CAL_SRC_M_CK                     : //= 8  ,
        case CAL_SRC_TCLK                     : //= 9  ,
        case CAL_SRC_AD_APLL2_FBOUT27M_CK     : //= 10 ,
        case CAL_SRC_AD_USBPLL_480M           : //= 11 ,
        case CAL_SRC_AD_USBPLL_480M_2P        : //= 12 ,
        case CAL_SRC_AD_ETHPLL_CLK675M        : //= 13 ,
        case CAL_SRC_XTAL                     : //= 14 ,
        case CAL_SRC_AD_ADC_CKOUT             : //= 15 ,
        case CAL_SRC_AD_CVBSADC_CKIND         : //= 16 ,
        case CAL_SRC_AD_HDMI_1_PLL340M_CK     : //= 17 ,
        case CAL_SRC_AD_HDMI_1_PIX340M_CK     : //= 18 ,
        case CAL_SRC_AD_HDMI_1_DEEP170M_CK    : //= 19 ,
        case CAL_SRC_AD_DDDS2_VSP             : //= 20 ,
        case CAL_SRC_AD_DDDS1_VSP             : //= 21 ,
        case CAL_SRC_TCLK_DIV                 : //= 22 ,
        case CAL_SRC_OCLK                     : //= 23 ,
        case CAL_SRC_OCLK_DIV                 : //= 24 ,
        case CAL_SRC_LVDS_CLK_CTS             : //= 25 ,
        case CAL_SRC_CPUPLL_D2_CLK            : //= 26 ,
        case CAL_SRC_AD_SOGY_ADC_CKOUT        : //= 30 ,
        case CAL_SRC_AD_CKT_B                 : //= 31 ,
        case CAL_SRC_AD_FB_OUT                : //= 32 ,
        case CAL_SRC_AD_SOGY_OUT_MON          : //= 33 ,
        case CAL_SRC_AD_SOGY_OUT              : //= 34 ,
        case CAL_SRC_AD_VSYNC_OUT             : //= 35 ,
        case CAL_SRC_AD_HSYNC_OUT             : //= 36 ,
        case CAL_SRC_AD_GPIO_PWM_0_0          : //= 37 ,
        case CAL_SRC_AD_GPIO_PWM_0_1          : //= 38 ,
        case CAL_SRC_AD_GPIO_PWM_0_2          : //= 39 ,
        case CAL_SRC_AD_GPIO_PWM_0_3          : //= 40 ,
        case CAL_SRC_AD_GPIO_PWM_0_4          : //= 41 ,
        case CAL_SRC_AD_GPIO_PWM_0_5          : //= 42 ,
        case CAL_SRC_AD_GPIO_PWM_0_6          : //= 43 ,
        case CAL_SRC_AD_GPIO_PWM_0_7          : //= 44 ,
        case CAL_SRC_AD_VOPLL_TEST_CLK        : //= 45 ,
        case CAL_SRC_AD_EVEN_CLK_DIV_125M_D11 : //= 46 ,
        case CAL_SRC_AD_ODD_CLK_DIV_125M_D11  : //= 47 ,
        case CAL_SRC_AD_CLKDIG_D11            : //= 48 ,
        case CAL_SRC_AD_CKOUT_DEMOD           : //= 49 ,
        case CAL_SRC_AD_SRV_SPL               : //= 50 ,
        case CAL_SRC_AD_CKT_A                 : //= 51 ,
        case CAL_SRC_AD_HDMI_2_PLL340M_CK     : //= 52 ,
        case CAL_SRC_AD_HDMI_2_DEEP170M_CK    : //= 53 ,
        case CAL_SRC_AD_HDMI_2_PIX340M_CK     : //= 54 ,
        case CAL_SRC_AD_HDMI_3_PLL340M_CK     : //= 55 ,
        case CAL_SRC_AD_HDMI_4_DEEP170M_CK    : //= 56 ,
        case CAL_SRC_AD_HDMI_5_PIX340M_CK     : //= 57 ,
        case CAL_SRC_AD_CLKDIG_CTS_D11        : //= 58 ,
        case CAL_SRC_AD_CVBSADC_CKOUT         : //= 60 ,
        case CAL_SRC_AD_VOPLL_DPIX_CK         : //= 64 ,
        case CAL_SRC_AD_VOPLL_TCON_CK         : //= 65 ,
        case CAL_SRC_AD_PPLL_DPIX_CK          : //= 66 ,
        case CAL_SRC_AD_XDDS_CLK              : //= 67 ,
        case CAL_SRC_AD_MJCPLL_REFCLK_BUF     : //= 68 ,
        case CAL_SRC_AD_MJCPLL_FBCLK_BUF      : //= 69 ,
        case CAL_SRC_AD_PLLGP_TSTCK           : //= 70 ,
        case CAL_SRC_AD_HADDS2_FBOUT27M_CK    : //= 71 ,
        case CAL_SRC_AD_VDECPLL_FBCLK_BUF     : //= 72 ,
        case CAL_SRC_AD_VDECPLL_REFCLK_BUF    : //= 73 ,
        case CAL_SRC_AD_PL_RCLK250            : //= 74 ,
        case CAL_SRC_AD_MEMPLL_MONCLK_PRE     : //= 75 ,
        case CAL_SRC_AD_MEMPLL_FBCLK_BUF_D16  : //= 76 ,
        case CAL_SRC_AD_MEMPLL_REFCLK_BUF_D16 : //= 77 ,
        case CAL_SRC_MIO_CK_P0_DIV8           : //= 78 ,
        case CAL_SRC_MIO_CK_P0_DIV4           : //= 79 ,
        case CAL_SRC_AD_SYNC_R_CK             : //= 80 ,
        case CAL_SRC_AD_SYNC_L_CK             : //= 81 ,
        case CAL_SRC_AD_CVBSADC_CKIND_2       : //= 82 ,
        case CAL_SRC_AD_USB20_MONCLK_3P       : //= 83 ,
        case CAL_SRC_AD_USB20_SESSEND_2P      : //= 84 ,
        case CAL_SRC_AD_USB20_MONCLK          : //= 85 ,
        case CAL_SRC_AD_USB20_MONCLK_1P       : //= 86 ,
        case CAL_SRC_AD_USB20_MONCLK_2P       : //= 87 ,
        case CAL_SRC_SPL2CORE                 : //= 88 ,
        case CAL_SRC_MON_CLK                  : //= 110,
            u4Clock = _CalGetPLLCounter((UINT32)eSource);
            u4Clock = u4Clock * 1000;
            break;

        case CAL_SRC_VOPLL:
            u4Clock = _CalGetPLLCounter((UINT32)CAL_SRC_AD_VOPLL_DPIX_CK);
            u4Clock = u4Clock * 1000; 
            break;


        case SRC_SYSPLL_CLK:
            vIO32WriteFldAlign(CKGEN_DBGCFG, 0x38, FLD_CKMON_SEL);
            u4Clock = _CalGetPLLCounter((UINT32)CAL_SRC_MON_CLK);
            u4Clock = u4Clock * 1000; 
            break;
        case SRC_CPU_CLK:
            switch (IO32ReadFldAlign(CKGEN_CPU_CKCFG, FLD_CPU_CK_SEL)) {
                case 0: u4Clock = GET_XTAL_CLK(); break;
                case 1: u4Clock = BSP_GetDomainClock(CAL_SRC_CPUPLL); break;
                case 2: u4Clock = BSP_GetDomainClock(CAL_SRC_AD_ETHPLL_CLK675M); break; // enetpll                
                case 3: u4Clock = BSP_GetDomainClock(CAL_SRC_SAWLESSPLL); break;
                case 4: u4Clock = BSP_GetDomainClock(CAL_SRC_SAWLESSPLL)/2; break;
                case 5: u4Clock = BSP_GetDomainClock(CAL_SRC_SAWLESSPLL)/4; break;
                case 6: u4Clock = BSP_GetDomainClock(CAL_SRC_SAWLESSPLL)/6; break;
                case 7: u4Clock = BSP_GetDomainClock(CAL_SRC_USBPLL)/2; break;
                case 8: u4Clock = BSP_GetDomainClock(CAL_SRC_USBPLL)/2*5; break;
                case 9: u4Clock = BSP_GetDomainClock(CAL_SRC_USBPLL)/4; break;
                case 10: u4Clock = BSP_GetDomainClock(CAL_SRC_USBPLL); break;
                case 11: u4Clock = BSP_GetDomainClock(CAL_SRC_DMPLL); break;
                case 12: u4Clock = 0; break; // TODO
                case 13: u4Clock = BSP_GetDomainClock(SRC_MEM_CLK); break;
                case 14: u4Clock = (GET_XTAL_CLK() / (1U << (4+IO32ReadFldAlign(CKGEN_MISC_CKCFG, FLD_SLOW_CK)))); break;
                case 15: u4Clock = (IO32ReadFldAlign(CKGEN_MISC_CKCFG, FLD_BIST_CK_PD) ?  0 :
                                    (GET_XTAL_CLK() / (1 + IO32ReadFldAlign(CKGEN_MISC_CKCFG, FLD_BIST_CK)))); break;
                default: u4Clock = 270000000; break;
            }
            break;
        case SRC_MEM_CLK:
            vIO32WriteFldAlign(CKGEN_DBGCFG, 0xd4, FLD_CKMON_SEL);
            u4Clock = _CalGetPLLCounter((UINT32)CAL_SRC_MON_CLK);
            u4Clock = u4Clock * 1000; // return digital design dram domain clock.
            break;
        case SRC_BUS_CLK:
            switch (IO32ReadFldAlign(CKGEN_CPU_CKCFG, FLD_BUS_CK_SEL)) {
                case 0: u4Clock = GET_XTAL_CLK(); break;
                case 1: u4Clock = BSP_GetDomainClock(SRC_SYSPLL_CLK)*2/5; break;
                case 2: u4Clock = BSP_GetDomainClock(CAL_SRC_USBPLL)*2/5; break;
                case 3: u4Clock = BSP_GetDomainClock(SRC_SYSPLL_CLK)/4; break;
                case 4: u4Clock = BSP_GetDomainClock(CAL_SRC_USBPLL)/4; break;
                case 5: u4Clock = BSP_GetDomainClock(CAL_SRC_AD_ETHPLL_CLK675M); break;
                case 6: u4Clock = BSP_GetDomainClock(CAL_SRC_SAWLESSPLL)/6; break;
                case 7: u4Clock = BSP_GetDomainClock(CAL_SRC_SAWLESSPLL)/2; break;
                default: u4Clock = 27000000; break;
            }
            break;
        case SRC_TIMER_CLK:
            u4Clock = GET_XTAL_CLK();
            break;
        case SRC_VDOIN_MPCLK:
            if (RegReadFldAlign(OMUX_00, VDOIN_MPCLK_SEL) == MPCLK_337MHZ)
            {
                if (IO32ReadFldAlign(CKGEN_PMUX_MISC, FLD_CLK180M_SEL))
                {
                    u4Clock = BSP_GetDomainClock(CAL_SRC_AD_ETHPLL_CLK675M);
                }
                else
                {
                    u4Clock = BSP_GetDomainClock(CAL_SRC_SAWLESSPLL)/2;
                }
            }
            else
            {
                u4Clock = 0;
            }
            break;
        case SRC_MIB_OCLK:
            switch (IO32ReadFldAlign(CKGEN_MIB_OCLK_CKCFG, FLD_MIB_OCLK_CK_SEL))
            {
                case 3:
                case 0: u4Clock = BSP_GetDomainClock(CAL_SRC_XTAL); break;
                case 1: u4Clock = BSP_GetDomainClock(SRC_SYSPLL_CLK); break;
                default:
                case 2: u4Clock = BSP_GetDomainClock(CAL_SRC_AD_ETHPLL_CLK675M); break;
            }
            break;
        case SRC_ETHPLL_CLK:
            u4Clock = _CalGetPLLCounter((UINT32)CAL_SRC_AD_ETHPLL_CLK675M);
            u4Clock = u4Clock * 1000;
            break;

        case CAL_SRC_APLL1:
            vIO32WriteFldAlign(CKGEN_DBGCFG, 0x31, FLD_CKMON_SEL);
            u4Clock = _CalGetPLLCounter((UINT32)CAL_SRC_MON_CLK);
            u4Clock = u4Clock * 1000; 
            break;

        case CAL_SRC_APLL2:
            vIO32WriteFldAlign(CKGEN_DBGCFG, 0x29, FLD_CKMON_SEL);
            u4Clock = _CalGetPLLCounter((UINT32)CAL_SRC_MON_CLK);
            u4Clock = u4Clock * 1000; 
            break;

        case CAL_SRC_USBPLL:
            u4Clock = BSP_GetDomainClock(CAL_SRC_AD_USBPLL_480M);
            break;

        default:
            break;
    }

    return u4Clock;
}

UINT32 BSP_GetBand(CAL_SRC_T ePll)
{
    UINT32 u4Band = 0xff;

    switch(ePll)
    {
        case SRC_SAWLESSPLL_CLK:
            u4Band = IO32ReadFldAlign(REG_IFPLL_CFG0, FLD_RG_ULJPLL_BAND);
            break;
        default:
            break;
    }

    return u4Band;
}

UINT32 BSP_SetBand(CAL_SRC_T ePll, UINT32 u4CalEn, UINT32 u4Band)
{
    switch(ePll)
    {
        case SRC_SAWLESSPLL_CLK:
            vIO32WriteFldAlign(REG_IFPLL_CFG0, u4Band, FLD_RG_ULJPLL_BAND);
            break;
        default:
            break;
    }
    return TRUE;
}


UINT32 BSP_SetVConDebugOut(CAL_SRC_T ePll)
{
#if 0 // new_ckgen
    UINT32 u4Rtn = 0;
    switch(ePll)
    {
        #ifndef CC_MTK_LOADER    
        case SRC_CPU_CLK:
            IO_WRITE32MSK(IO_ADDR(0x2d000), 0x634, 0, 0xf);    
            IO_WRITE32MSK(IO_ADDR(0x2d000), 0x634, 0x3f0000, 0x3f0000);    
            vIO32WriteFldMulti(CKGEN_AFEPLLCFG20, P_Fld(1,FLD_RG_PLLGP_TEST_EN)|P_Fld(0,FLD_RG_PLLGP_SEL_CK)|P_Fld(7,FLD_RG_PLLGP_MONSEL)); 
            vIO32WriteFldAlign(CKGEN_AFEPLLCFG17, 1, FLD_RG_CPUPLL_MONAIO_EN);             
            break;
        case SRC_SYSPLL_CLK:
            IO_WRITE32MSK(IO_ADDR(0x2d000), 0x634, 0, 0xf);    
            IO_WRITE32MSK(IO_ADDR(0x2d000), 0x634, 0x3f0000, 0x3f0000);    
            vIO32WriteFldMulti(CKGEN_AFEPLLCFG20, P_Fld(1,FLD_RG_PLLGP_TEST_EN)|P_Fld(0,FLD_RG_PLLGP_SEL_CK)|P_Fld(1,FLD_RG_PLLGP_MONSEL)); 
 //           vIO32WriteFldAlign(CKGEN_AFEPLLCFG1, 1, FLD_RG_VGAPLL_MONAIOEN);             
            vIO32WriteFldAlign(CKGEN_AFEPLLCFG3, 1, FLD_RG_TVDPLL_MONAIOEN);             
            break;
        case SRC_APLL1_CLK:
            IO_WRITE32MSK(IO_ADDR(0x2d000), 0x634, 0, 0xf);    
            IO_WRITE32MSK(IO_ADDR(0x2d000), 0x634, 0x3f0000, 0x3f0000);    
            vIO32WriteFldMulti(CKGEN_AFEPLLCFG20, P_Fld(1,FLD_RG_PLLGP_TEST_EN)|P_Fld(0,FLD_RG_PLLGP_SEL_CK)|P_Fld(4,FLD_RG_PLLGP_MONSEL)); 
            vIO32WriteFldAlign(CKGEN_AFEPLLCFG10, 1, FLD_RG_HA1_MONAIOEN);             
            break;
        case SRC_APLL2_CLK:
            IO_WRITE32MSK(IO_ADDR(0x2d000), 0x634, 0, 0xf);    
            IO_WRITE32MSK(IO_ADDR(0x2d000), 0x634, 0x3f0000, 0x3f0000);    
            vIO32WriteFldMulti(CKGEN_AFEPLLCFG20, P_Fld(1,FLD_RG_PLLGP_TEST_EN)|P_Fld(0,FLD_RG_PLLGP_SEL_CK)|P_Fld(3,FLD_RG_PLLGP_MONSEL)); 
            vIO32WriteFldAlign(CKGEN_AFEPLLCFG13, 1, FLD_RG_HA2_MONAIOEN);             
            break;
        case SRC_SAWLESSPLL_CLK:
            vIO32WriteFldMulti(CKGEN_DEMODPLLCFG, P_Fld(1,FLD_RG_DCTESTEN)|P_Fld(1,FLD_RG_DCLPFEN)); 

           // Analog FE setting
            IO_WRITE32(0xF002B000, 0x100, 0x005aa984); 
            IO_WRITE32(0xF002B000, 0x104, 0x0001000c);
            IO_WRITE32(0xF002B000, 0x108, 0x81a89500);
            IO_WRITE32(0xF002B000, 0x10c, 0x00000003);

            vRegWrite4B(VFE_E6, 0x20000ac1);              
            vRegWrite4B(VFE_E7, 0x600);         

            // nim command
            NIM_SetTunerID(0);
            CLI_Parser("nim.atd w 0x100 0x84");
            CLI_Parser("nim.atd w 0x101 0xa9");
            CLI_Parser("nim.atd w 0x102 0x5a");
            CLI_Parser("nim.atd w 0x103 0x0");
            CLI_Parser("nim.atd w 0x104 0xc");
            CLI_Parser("nim.atd w 0x105 0x0");
            CLI_Parser("nim.atd w 0x106 0x1");
            CLI_Parser("nim.atd w 0x107 0x0");
            CLI_Parser("nim.atd w 0x108 0x0");
            CLI_Parser("nim.atd w 0x109 0x95");
            CLI_Parser("nim.atd w 0x10a 0xa8");
            CLI_Parser("nim.atd w 0x10b 0x81");
            CLI_Parser("nim.atd w 0x10c 0x3");
            CLI_Parser("nim.atd w 0x10d 0x0");
            CLI_Parser("nim.atd w 0x10e 0x0");
            CLI_Parser("nim.atd w 0x10f 0x0");            
            break;
        case SRC_ADCPLL_CLK:
            IO_WRITE32MSK(IO_ADDR(0x2d000), 0x634, 0, 0xf);    
            IO_WRITE32MSK(IO_ADDR(0x2d000), 0x634, 0x3f0000, 0x3f0000);    
            vIO32WriteFldMulti(CKGEN_AFEPLLCFG20, P_Fld(1,FLD_RG_PLLGP_TEST_EN)|P_Fld(0,FLD_RG_PLLGP_SEL_CK)|P_Fld(5,FLD_RG_PLLGP_MONSEL)); 
            vIO32WriteFldAlign(CKGEN_AFEPLLCFG1, 1, FLD_RG_VGAPLL_MONAIOEN);             
 //           vIO32WriteFldAlign(CKGEN_AFEPLLCFG3, 1, FLD_RG_TVDPLL_MONAIOEN);             
            break;
        case SRC_ETHPLL_CLK:
            IO_WRITE32MSK(IO_ADDR(0x2d000), 0x634, 0, 0xf);    
            IO_WRITE32MSK(IO_ADDR(0x2d000), 0x634, 0x3f0000, 0x3f0000);    
            vIO32WriteFldMulti(CKGEN_AFEPLLCFG20, P_Fld(1,FLD_RG_PLLGP_TEST_EN)|P_Fld(0,FLD_RG_PLLGP_SEL_CK)|P_Fld(0,FLD_RG_PLLGP_MONSEL)); 
            vIO32WriteFldAlign(CKGEN_AFEPLLCFG19, 1, FLD_RG_ETHPLL_MONAIOEN);             
            break;
        case SRC_ETHNETPLL_CLK:
            // todo
            {
                UINT32 u4Value;

                // switch to test page
                IO_WRITE32(ETHERNET_BASE, 0x0, 0x8000);
                IO_WRITE32(ETHERNET_BASE, 0x0, 0x2a303f00);

                do
                {
                    u4Value = IO_READ32MSK(ETHERNET_BASE, 0x0, 0x8000);
                    HAL_Delay_us(100000);
                } while(u4Value == 0);
                
                // RG_TST_MODE[5:3]=010
                IO_WRITE32(ETHERNET_BASE, 0x0, 0x8000);
                IO_WRITE32(ETHERNET_BASE, 0x0, 0x10002000);

                do
                {
                    u4Value = IO_READ32MSK(ETHERNET_BASE, 0x0, 0x8000);
                    HAL_Delay_us(100000);
                } while(u4Value == 0);

                // RG_PL_MON=1
                IO_WRITE32(ETHERNET_BASE, 0x0, 0x8000);
                IO_WRITE32(ETHERNET_BASE, 0x0, 0x903200);

                do
                {
                    u4Value = IO_READ32MSK(ETHERNET_BASE, 0x0, 0x8000);
                    HAL_Delay_us(100000);
                } while(u4Value == 0);
            }
            break;
        #endif    
        default:   
            u4Rtn = 1;
            break;
    }

    return u4Rtn;
#else
    UNUSED(ePll);
    return 0;
#endif
}
    
#if 0
static UINT32 _aru4CpuFreq[] = {
    27000000,
    702000000,
    459000000,
    432000000,
    216000000,
    108000000,
    72000000,
    240000000,
    160000000,
    120000000,
    480000000,
    0
};

// Returns duty in %
// Returns 0xFFFFFFFF if target clock out of range
UINT32 BSP_GetClockDuty(CAL_SRC_T eSrc, UINT32 u4TargetClk)
{
    UINT32 u4Duty = 0;
    UINT32 u4RegMisc, u4RegAbist;
    UINT32 u4High, u4All;
    UINT32 u4SavedReg, i, u4TargetCpuClock;

    u4SavedReg = CKGEN_READ32(0x208);

    // Change CPU freq if necessary
    i = 0;
    while (1)
    {
        u4TargetCpuClock = _aru4CpuFreq[i];
        if (u4TargetCpuClock == 0)
        {
            return 0xFFFFFFFF;
        }
        
        if ((u4TargetClk * 20 < u4TargetCpuClock) &&
            (u4TargetCpuClock < u4TargetClk * 256))
        {
            CKGEN_WRITE32(0x208, ((u4SavedReg & 0xFFFFFFF0) | i));
            //Printf("Use CPU clock %d\n", u4TargetCpuClock);
            break;
        }

        i++;
    }

    // Reset duty meter
    u4RegAbist = CKGEN_READ32(0x2D0);
    u4RegAbist &= ~0x100;
    CKGEN_WRITE32(0x2D0, u4RegAbist);

    // Clear target source
    u4RegMisc = CKGEN_READ32(0x200);
    u4RegMisc &= ~0xC0000000;
    u4RegAbist &= ~0xC00;

    // Set target source
    switch (eSrc)
    {
        case CAL_SRC_FBOUT:
            break;
        case CAL_SRC_AD_HSYNC_OUT:
            u4RegAbist |= 0x400;
            break;
        case CAL_SRC_AD_VSYNC_OUT:
            u4RegAbist |= 0x800;
            break;
        case CAL_SRC_AD_SOGY_OUT:
            u4RegAbist |= 0xC00;
            break;
        case CAL_SRC_AD_GPIO_PWM_0:
            u4RegMisc |= 0x80000000;
            u4RegAbist |= 0;
            break;
        case CAL_SRC_AD_GPIO_PWM_1:
            u4RegMisc |= 0x80000000;
            u4RegAbist |= 0x400;
            break;
        case CAL_SRC_AD_GPIO_PWM_2:
            u4RegMisc |= 0x80000000;
            u4RegAbist |= 0x800;
            break;
        case CAL_SRC_AD_GPIO_PWM_3:
            u4RegMisc |= 0x80000000;
            u4RegAbist |= 0xC00;
            break;
        case CAL_SRC_AD_GPIO_PWM_4:
            u4RegMisc |= 0xC0000000;
            u4RegAbist |= 0;
            break;
        case CAL_SRC_AD_GPIO_PWM_5:
            u4RegMisc |= 0xC0000000;
            u4RegAbist |= 0x400;
            break;
        case CAL_SRC_AD_GPIO_PWM_6:
            u4RegMisc |= 0xC0000000;
            u4RegAbist |= 0x800;
            break;
        case CAL_SRC_AD_GPIO_PWM_7:
            u4RegMisc |= 0xC0000000;
            u4RegAbist |= 0xC00;
            break;
        default:
            break;
    }

    CKGEN_WRITE32(0x200, u4RegMisc);

    // Start measure
    u4RegAbist |= 0x100;
    CKGEN_WRITE32(0x2D0, u4RegAbist);

    // Delay 1 ms
    // HAL_Delay_us(1000);

    // Check done
    while ((CKGEN_READ32(0x2D0) & 0x200) == 0)
    {
    }

    // Calculate duty
    u4High = (CKGEN_READ32(0x2D0) & 0xFF000000) >> 24;
    u4All = (CKGEN_READ32(0x2D0) & 0xFF0000) >> 16;

    if (u4All != 0)
    {
        u4Duty = (u4High * 100) / u4All;
    }

    CKGEN_WRITE32(0x208, u4SavedReg); 

    return u4Duty;
}
#endif

