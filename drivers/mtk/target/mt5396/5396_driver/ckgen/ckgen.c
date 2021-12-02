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

/*----------------------------------------------------------------------------*
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: ckgen.c $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*
 * Include files
 *---------------------------------------------------------------------------*/
#include "x_lint.h"
#include "x_ckgen.h"
#include "x_hal_5381.h"
#include "x_hal_arm.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_dram.h"
#include "x_timer.h"
#include "drvcust_if.h"
#include "aud_if.h"
#include "hw_vdoin.h"
#include "hw_ckgen.h"
#include "hw_ifpll.h"
#include "hw_pdwnc.h"

#ifndef CC_MTK_LOADER
#include "hw_vfe.h"
#include "nim_drvif.h"
#include "x_drv_cli.h"
#include "hw_pll_group.h"
#endif

//-----------------------------------------------------------------------------
// Configuration
//-----------------------------------------------------------------------------
extern void AudPll2Setting(SAMPLE_FREQ_T eFs);

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
    case CAL_SRC_CPUPLL_D2:
        return "CPUPLL_D2";
    case CAL_SRC_SYSPLL_D2:
        return "SYSPLL_D2";
    case CAL_SRC_APLL1:
        return "APLL1";
    case CAL_SRC_APLL_D3:
        return "APLL_D3";
    case CAL_SRC_APLL2:
        return "APLL2";
    case CAL_SRC_ADCPLL:
        return "ADCPLL";
    case CAL_SRC_M_CK:
        return "M_CK";
    case CAL_SRC_VDECPLL:
        return "VDECPLL";
    case CAL_SRC_FBOUT:
        return "FBOUT";
    case CAL_SRC_HDMI_PLL340M:
        return "HDMI_PLL340M";
    case CAL_SRC_XTAL:
        return "XTAL";
    case CAL_SRC_PIXPLL:
        return "PPLL";
    case CAL_SRC_AD_HSYNC_OUT:
        return "AD_HSYNC_OUT";
    case CAL_SRC_AD_VSYNC_OUT:
        return "AD_VSYNC_OUT";
    case CAL_SRC_AD_SOGY_OUT:
        return "AD_SOGY_OUT";
    case CAL_SRC_AD_SOGY_ADC_CKOUT:
        return "AD_SOGY_ADC_CKOUT";
    case CAL_SRC_CVBSADC_CKOUT:
        return "AD_CVBSADC_CKOUT";
    case CAL_SRC_AD_GPIO_PWM_0:
        return "AD_GPIO_PWM_0";
    case CAL_SRC_AD_GPIO_PWM_1:
        return "AD_GPIO_PWM_1";
    case CAL_SRC_AD_GPIO_PWM_2:
        return "AD_GPIO_PWM_2";
    case CAL_SRC_AD_GPIO_PWM_3:
        return "AD_GPIO_PWM_3";
    case CAL_SRC_AD_GPIO_PWM_4:
        return "AD_GPIO_PWM_4";
    case CAL_SRC_AD_GPIO_PWM_5:
        return "AD_GPIO_PWM_5";
    case CAL_SRC_AD_GPIO_PWM_6:
        return "AD_GPIO_PWM_6";
    case CAL_SRC_AD_GPIO_PWM_7:
        return "AD_GPIO_PWM_7";
    case SRC_CPU_CLK:
        return "SRC_CPU_CLK";
    case SRC_MEM_CLK:
        return "SRC_MEM_CLK";
    case SRC_BUS_CLK:
        return "SRC_BUS_CLK";
    case SRC_TIMER_CLK:
        return "SRC_TIMER_CLK";
    case SRC_VDOIN_MPCLK:
        return "SRC_VDOIN_MPCLK";

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

    // Select PLL source at REG_PLL_CALIB
    vIO32WriteFldAlign(CKGEN_PLLCALIB, CAL_SRC_SAWLESSPLL, FLD_DBGCKSEL);

    // Select mode, 0: 2048
    vIO32WriteFldAlign(CKGEN_PLLCALIB, 0, FLD_CAL_MODE);

    // Set CAL_SEL, 0 is clk/1, 1 is clk/2, 2 is clk/4
    vIO32WriteFldAlign(CKGEN_PLLCALIB, 0, FLD_CAL_SEL);

    // Select PLL source at REG_PLL_CALIB
    vIO32WriteFldAlign(CKGEN_PLLCALIB, u4CalSel, FLD_DBGCKSEL);

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
    //UINT32 u4Crit;

    // Before Calibration, enable the PLL.
    _CalEnablePLL(eSource);

    _CalSetMN(eSource, u4Clock);

    switch(eSource)
    {
        case CAL_SRC_PIXPLL:
        case CAL_SRC_ADCPLL:
            fgRet = TRUE;
            if (DRVCUST_InitGet(eFlagBootVerbose))
            {
                // Delay a 1 ms to make close loop clock stable
                HAL_Delay_us(1000);

                u4CalClk = _CalGetPLLCounter((UINT32)(eSource));
                Printf("%s now is %d kHz\n", _CalString(eSource), u4CalClk);
            }
            break;
        #if 0
        case SRC_FRAC_DIV_CLK:
            // TODO: support generic frequencies
            if (u4Clock != 25000000)
            {
                return TRUE;
            }

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

            break;
        #endif
        default:
            return fgRet;
    }

    // Do the post settings.
    _CalSetPLL(eSource);

    return fgRet;
}


UINT32 BSP_GetClock(CAL_SRC_T eSource, UINT8* pu1Band, UINT16* pu2MS, UINT16* pu2NS, UINT16* pu2Counter)
{
    UINT32 u4Clock = 0;

    UNUSED(pu2MS);
    UNUSED(pu2NS);
    UNUSED(pu1Band);

    switch(eSource)
    {
    case CAL_SRC_SAWLESSPLL:
    case CAL_SRC_CPUPLL_D2:
    case CAL_SRC_SYSPLL_D2:
    case CAL_SRC_APLL1:
    case CAL_SRC_APLL_D3:
    case CAL_SRC_APLL2:
    case CAL_SRC_APLL2_D3:
    case CAL_SRC_ADCPLL:
    case CAL_SRC_M_CK:
    case CAL_SRC_VDECPLL:
    case CAL_SRC_FBOUT:
    case CAL_SRC_USBPLL:
    case CAL_SRC_ETHPLL_CLK675M:
    case CAL_SRC_XTAL:
    case CAL_SRC_ADC_CKTOUT:
    case CAL_SRC_CVBSADC_CKIND:
    case CAL_SRC_HDMI_PLL340M:
    case CAL_SRC_HDMI_PIX340M:
    case CAL_SRC_HDMI_DEEP170M:
    case CAL_SRC_PIXPLL:
//    case CAL_SRC_VOPLL:
    case CAL_SRC_AD_HSYNC_OUT:
    case CAL_SRC_AD_VSYNC_OUT:
    case CAL_SRC_AD_SOGY_OUT:
    case CAL_SRC_AD_FB_OUT:
    case CAL_SRC_AD_SOGY_ADC_CKOUT:
    case CAL_SRC_AD_GPIO_PWM_0:
    case CAL_SRC_AD_GPIO_PWM_1:
    case CAL_SRC_AD_GPIO_PWM_2:
    case CAL_SRC_AD_GPIO_PWM_3:
    case CAL_SRC_AD_GPIO_PWM_4:
    case CAL_SRC_AD_GPIO_PWM_5:
    case CAL_SRC_AD_GPIO_PWM_6:
    case CAL_SRC_AD_GPIO_PWM_7:
    case CAL_SRC_CVBSADC_CKOUT:
    case CAL_SRC_MEMPLL_D8:
        u4Clock = _CalGetPLLCounter((UINT32)eSource);
        u4Clock = u4Clock * 1000;
        break;
    case SRC_SYSPLL_CLK:
        u4Clock = _CalGetPLLCounter((UINT32)CAL_SRC_SYSPLL_D2);
        u4Clock = u4Clock * 1000 * 2;
        break;
    case SRC_VDECPLL_CLK:
        u4Clock = _CalGetPLLCounter((UINT32)CAL_SRC_VDECPLL);
        u4Clock = u4Clock * 1000;
        break;
    case SRC_CPU_CLK:
        switch (IO32ReadFldAlign(CKGEN_CPU_CKCFG, FLD_CPU_CK_SEL))
        {
            case  0: u4Clock = GET_XTAL_CLK(); break;
            case  1: u4Clock = BSP_GetDomainClock(CAL_SRC_CPUPLL_D2)*2; break;
            case  2: u4Clock = BSP_GetDomainClock(CAL_SRC_ETHPLL_CLK675M); break;
            case  3: u4Clock = BSP_GetDomainClock(CAL_SRC_SYSPLL_D2)/2; break;
            case  4: u4Clock = BSP_GetDomainClock(CAL_SRC_USBPLL); break;
            case  5: u4Clock = BSP_GetDomainClock(CAL_SRC_SAWLESSPLL); break;
            case  6: u4Clock = BSP_GetDomainClock(CAL_SRC_CPUPLL_D2); break;
            case  7: u4Clock = BSP_GetDomainClock(CAL_SRC_USBPLL)/2; break;
            case  8: u4Clock = BSP_GetDomainClock(CAL_SRC_SAWLESSPLL)/2; break;
            case  9: u4Clock = BSP_GetDomainClock(CAL_SRC_SAWLESSPLL)/3; break;
            case 10: u4Clock = BSP_GetDomainClock(CAL_SRC_USBPLL)/4; break;
            case 11: u4Clock = BSP_GetDomainClock(CAL_SRC_SAWLESSPLL)/4; break;
            case 12: u4Clock = BSP_GetDomainClock(CAL_SRC_ETHPLL_CLK675M)/4; break;
            case 13: u4Clock = BSP_GetDomainClock(SRC_MEM_CLK); break;
            case 14: u4Clock = (GET_XTAL_CLK() / (1U << (4+IO32ReadFldAlign(CKGEN_MISC_CKCFG, FLD_SLOW_CK)))); break;
            case 15: u4Clock = (IO32ReadFldAlign(CKGEN_MISC_CKCFG, FLD_BIST_CK_PD) ?  0 :
                                (GET_XTAL_CLK() / (1 + IO32ReadFldAlign(CKGEN_MISC_CKCFG, FLD_BIST_CK)))); break;
            default: u4Clock = 270000000; break;
        }
        break;
    case SRC_MEM_CLK:
        u4Clock = _CalGetPLLCounter((UINT32)CAL_SRC_MEMPLL_D8);
        u4Clock = u4Clock * 1000 * 4; // return digital design dram domain clock.
        break;
    case SRC_BUS_CLK:
        switch (IO32ReadFldAlign(CKGEN_CPU_CKCFG, FLD_BUS_CK_SEL))
        {
            case 0: u4Clock = GET_XTAL_CLK(); break;
            case 1: u4Clock = BSP_GetDomainClock(CAL_SRC_SAWLESSPLL)/2; break;
            case 2: u4Clock = BSP_GetDomainClock(CAL_SRC_USBPLL)/2*5; break;
            case 3: u4Clock = BSP_GetDomainClock(CAL_SRC_SYSPLL_D2)/2; break;
            case 4: u4Clock = BSP_GetDomainClock(CAL_SRC_SAWLESSPLL)/3; break;
            case 5: u4Clock = BSP_GetDomainClock(CAL_SRC_USBPLL)/4; break;
            case 6: u4Clock = BSP_GetDomainClock(CAL_SRC_SYSPLL_D2)*2/3; break;
            case 7: u4Clock = (GET_XTAL_CLK() / (1U << (4+IO32ReadFldAlign(CKGEN_MISC_CKCFG, FLD_SLOW_CK)))); break;
            default: u4Clock = 27000000; break;
        }
        break;
    case SRC_TIMER_CLK:
        u4Clock = GET_XTAL_CLK();
        break;
    case SRC_POCLK:
        switch (IO32ReadFldAlign(CKGEN_DISP_CKCFG, FLD_POCLK_SEL))
        {
            case 0: u4Clock = BSP_GetDomainClock(CAL_SRC_OCLK); break;
            case 1: u4Clock = BSP_GetDomainClock(CAL_SRC_OCLK)/2; break;
            case 2: u4Clock = BSP_GetDomainClock(SRC_POCLK_DPLL); break;
            case 3: u4Clock = BSP_GetDomainClock(SRC_MIB_OCLK); break;
            default: u4Clock = 0; break;
        }
        break;
    case SRC_VDOIN_MPCLK:
        if (RegReadFldAlign(OMUX_00, VDOIN_MPCLK_SEL) == MPCLK_337MHZ)
        {
            if (IO32ReadFldAlign(CKGEN_PMUX_MISC, FLD_CLK180M_SEL))
            {
                u4Clock = BSP_GetDomainClock(CAL_SRC_ETHPLL_CLK675M);
            }
            else
            {
                u4Clock = BSP_GetDomainClock(CAL_SRC_SAWLESSPLL)/2;
            }
        }
        else
        {
            u4Clock = 0 ;
        }
        break;
    case SRC_MIB_OCLK:
        switch (IO32ReadFldAlign(CKGEN_MIB_OCLK_CKCFG, FLD_MIB_OCLK_CK_SEL))
        {
            case 3:
            case 0: u4Clock = BSP_GetDomainClock(CAL_SRC_XTAL); break;
            case 1: u4Clock = BSP_GetDomainClock(CAL_SRC_SYSPLL_D2); break;
            default:
            case 2: u4Clock = BSP_GetDomainClock(CAL_SRC_ETHPLL_CLK675M); break;
        }
        break;
    case SRC_POCLK_DPLL:
        if (IO32ReadFldAlign(CKGEN_VPCLK_CFG, FLD_POCLK_DPLL_SEL))
        {
            u4Clock = (BSP_GetDomainClock(CAL_SRC_ETHPLL_CLK675M) >> 15) * IO32ReadFldAlign(CKGEN_POCLK_DPLL_DIVCFG, FLD_POCLK_DPLL_IDEAL_PLL_M);
        }
        else
        {
            u4Clock = (BSP_GetDomainClock(CAL_SRC_SYSPLL_D2) >> 15) * IO32ReadFldAlign(CKGEN_POCLK_DPLL_DIVCFG, FLD_POCLK_DPLL_IDEAL_PLL_M);
        }
        break;
    case SRC_MJC_CLK:
        switch (IO32ReadFldAlign(CKGEN_MJC_CKCFG, FLD_MJC_CK_SEL))
        {
            case 0: u4Clock = BSP_GetDomainClock(CAL_SRC_XTAL); break;
            case 1: u4Clock = BSP_GetDomainClock(CAL_SRC_SYSPLL_D2); break;
            case 2: u4Clock = BSP_GetDomainClock(CAL_SRC_ETHPLL_CLK675M); break;
            case 3: u4Clock = (BSP_GetDomainClock(CAL_SRC_SAWLESSPLL) * 3) >> 1; break;
            case 4: u4Clock = (BSP_GetDomainClock(CAL_SRC_SYSPLL_D2) * 2) / 3; break;
            default:  u4Clock = 0; break;
        }
        break;
    case SRC_ETHPLL_CLK:
        u4Clock = _CalGetPLLCounter((UINT32)CAL_SRC_ETHPLL_CLK675M);
        u4Clock = u4Clock * 1000 * 2;
        break;
    default:
		u4Clock = _CalGetPLLCounter((UINT32)eSource);
		u4Clock = u4Clock * 1000;
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
#if 1
    UINT32 u4Rtn = 0;
    switch(ePll)
    {
        #ifndef CC_MTK_LOADER
        case SRC_CPU_CLK:
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG2, 0x1, FLD_RG_MJCPLL_MONEN);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG2, 0x1, FLD_RG_MJCPLL_DIVEN);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG9, 0x1, FLD_RG_PLLGP_TSTEN);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x1, FLD_RG_PLLGP_TSTCKENB);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x0, FLD_RG_PLLGP_TSTCK_FMEN);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x0, FLD_RG_PLLGP_TSTCKAMP);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG9, 0x2, FLD_RG_PLLGP_TSTSEL);
            break;
        case SRC_SYSPLL_CLK:
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG3, 0x1, FLD_RG_SYSPLL_MONEN);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG9, 0x1, FLD_RG_PLLGP_TSTEN);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x1, FLD_RG_PLLGP_TSTCKENB);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x0, FLD_RG_PLLGP_TSTCK_FMEN);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x0, FLD_RG_PLLGP_TSTCKAMP);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG9, 0x3, FLD_RG_PLLGP_TSTSEL);
            break;
        case SRC_VDECPLL_CLK:
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG6, 0x1, FLD_RG_VDECPLL_MONEN);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG6, 0x1, FLD_RG_VDECPLL_DIVEN);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG9, 0x1, FLD_RG_PLLGP_TSTEN);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x1, FLD_RG_PLLGP_TSTCKENB);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x0, FLD_RG_PLLGP_TSTCK_FMEN);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x0, FLD_RG_PLLGP_TSTCKAMP);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG9, 0x5, FLD_RG_PLLGP_TSTSEL);
            break;
        case SRC_XPLL_CLK:
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG0, 0x0, FLD_RG_XPLL_PWD);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG0, 0x1, FLD_RG_XPLL_MONEN);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG9, 0x1, FLD_RG_PLLGP_TSTEN);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x1, FLD_RG_PLLGP_TSTCKENB);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x0, FLD_RG_PLLGP_TSTCK_FMEN);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x0, FLD_RG_PLLGP_TSTCKAMP);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG9, 0x4, FLD_RG_PLLGP_TSTSEL);
            break;
           
        case CAL_SRC_APLL1:
            AudPll2Setting(FS_48K); // Set HADDS2 = 294.912Mhz
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 0x1, FLD_RG_HADDS2_DDS_MONEN);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x1, FLD_RG_HADDS_TSTEN);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x1, FLD_RG_HADDS_TSTCKENB);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x0, FLD_RG_HADDS_TSTCK_FMEN);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x6, FLD_RG_HADDS_TSTSEL);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG9, 0x1, FLD_RG_PLLGP_TSTEN);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x1, FLD_RG_PLLGP_TSTCKENB);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x0, FLD_RG_PLLGP_TSTCK_FMEN);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x0, FLD_RG_PLLGP_TSTCKAMP);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG9, 0x0, FLD_RG_PLLGP_TSTSEL);
            break;
        case CAL_SRC_APLL2:
            AudPll2Setting(FS_44K); // Set HADDS2 = 270.9504Mhz
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 0x1, FLD_RG_HADDS2_DDS_MONEN);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x1, FLD_RG_HADDS_TSTEN);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x1, FLD_RG_HADDS_TSTCKENB);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x0, FLD_RG_HADDS_TSTCK_FMEN);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x6, FLD_RG_HADDS_TSTSEL);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG9, 0x1, FLD_RG_PLLGP_TSTEN);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x1, FLD_RG_PLLGP_TSTCKENB);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x0, FLD_RG_PLLGP_TSTCK_FMEN);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x0, FLD_RG_PLLGP_TSTCKAMP);
            vIO32WriteFldAlign(REG_PLL_GROUP_CFG9, 0x0, FLD_RG_PLLGP_TSTSEL);
            break;
        case SRC_MEM_CLK:
            vIO32WriteFldAlign(REG_MEMPLL0, 0x0, FLD_RG_MEMPLL_BRCKMON_PD);
            vIO32WriteFldAlign(REG_MEMPLL0, 0x0, FLD_RG_MEMPLL_SEL_CK);
            vIO32WriteFldAlign(REG_MEMPLL0, 0x1, FLD_RG_MEMPLL_MONEN);
            break;
        case SRC_SAWLESSPLL_CLK:
            vIO32WriteFldAlign(REG_IFPLL_CFG1, 0x1, FLD_RG_ULJPLL_MONEN);
            vIO32WriteFldAlign(REG_IFPGA_CFG1, 0x4, FLD_RG_IFTQI_MON);
            vIO32WriteFldAlign(REG_IFPGA_CFG1, 0x1, FLD_RG_IFTESTN_EN);
            vIO32WriteFldAlign(REG_IFPGA_CFG1, 0x1, FLD_RG_IFTESTP_EN);
            vIO32WriteFldAlign(REG_IFPGA_CFG0, 0x1, FLD_RG_IFPGA_TESTE);
            
            //vIO32Write4B(PDWNC_SRVCFG0, 0x00000051);
            vIO32WriteFldMulti(PDWNC_SRVCFG0, P_Fld(1,FLD_HWEN)|P_Fld(0,FLD_SWEN)|P_Fld(0,FLD_SRVWAK_HD)|P_Fld(0,FLD_ABIST_MODE)|P_Fld(5,FLD_SRVCH_SEL));
            vIO32WriteFldAlign(PDWNC_SRVCFG1, 0x20, FLD_SRVCH_EN);
            vIO32WriteFldMulti(PDWNC_SADCCFG1, P_Fld(1,FLD_RG_SRVADC_TESTE)|P_Fld(1,FLD_RG_TPBUFF_SEL)|P_Fld(1,FLD_RG_TPBUFF_EN)|P_Fld(1,FLD_RG_TNPASS_EN));
            break;
#if 0            
        case SRC_ADCPLL_CLK:
            IO_WRITE32MSK(IO_ADDR(0x2d000), 0x634, 0, 0xf);
            IO_WRITE32MSK(IO_ADDR(0x2d000), 0x634, 0x3f0000, 0x3f0000);
            vIO32WriteFldMulti(CKGEN_AFEPLLCFG20, P_Fld(1,FLD_RG_PLLGP_TEST_EN)|P_Fld(0,FLD_RG_PLLGP_SEL_CK)|P_Fld(5,FLD_RG_PLLGP_MONSEL));
//            vIO32WriteFldAlign(CKGEN_AFEPLLCFG1, 1, FLD_RG_VGAPLL_MONAIOEN);
      vIO32WriteFldAlign(REG_PLL_GROUP_CFG1, 1, FLD_RG_VGAPLL_MONCKEN);
 //           vIO32WriteFldAlign(CKGEN_AFEPLLCFG3, 1, FLD_RG_TVDPLL_MONAIOEN);
            break;
#endif            
        case SRC_VOPLL_CLK:
            vIO32WriteFldAlign(REG_VOPLL_CFG2, 0x1, RG_VPLL_MONEN);
            vIO32WriteFldAlign(REG_VOPLL_CFG2, 0x1, RG_VPLL_TST_CKENB);
            vIO32WriteFldAlign(REG_VOPLL_CFG3, 0x7, RG_VPLL_TST_SEL);
            vIO32WriteFldAlign(REG_VOPLL_CFG4, 0x1, RG_PPLL_TSTEN);
            vIO32WriteFldMulti(PDWNC_SRVCFG0, P_Fld(1,FLD_HWEN)|P_Fld(0,FLD_SWEN)|P_Fld(0,FLD_SRVWAK_HD)|P_Fld(0,FLD_ABIST_MODE)|P_Fld(0,FLD_SRVCH_SEL));
            vIO32Write4B(PDWNC_SADCCFG0, 0x350);
            vIO32WriteFldMulti(PDWNC_SADCCFG1, P_Fld(1,FLD_RG_SRVADC_TESTE)|P_Fld(1,FLD_RG_TPBUFF_EN)|P_Fld(1,FLD_RG_TNPASS_EN));
            vIO32Write4B(PDWNC_ADOUT7, (u4IO32Read4B(PDWNC_ADOUT7)|0x10000000));
            break;
        #endif
        default:
            u4Rtn = 1;
            break;
    }

    return u4Rtn;
#else
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

