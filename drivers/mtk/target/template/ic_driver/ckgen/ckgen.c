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
#include "x_assert.h"
#include "x_printf.h"
#include "x_dram.h"
#include "x_timer.h"
#include "drvcust_if.h"

//-----------------------------------------------------------------------------
// Configuration
//-----------------------------------------------------------------------------

#define INIT_PSPLL_WITH_OUTPATH     1   // default should be 1


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define LOG(level, fmt...)  // Printf(fmt)

#if INIT_PSPLL_WITH_OUTPATH
#define REG_RW_MAINPIP_OUTPATH_SEL      (0x06bc)
    #define OMUX_MAIN_SEL               (0xfU << 4)     // Select video channel to main output path
    #define OMUX_MAIN_TVD3D             (0U << 4)
    #define OMUX_MAIN_NO_VIDEO          (1U << 4)
    #define OMUX_MAIN_HDTV              (2U << 4)
    #define OMUX_MAIN_VGA               (3U << 4)
    #define OMUX_MAIN_CCD               (4U << 4)
    #define OMUX_MAIN_CCH               (5U << 4)
    #define OMUX_MAIN_DVI               (6U << 4)
    #define OMUX_MAIN_MPEG1             (8U << 4)
    #define OMUX_MAIN_MPEG2             (9U << 4)
#endif /* INIT_PSPLL_WITH_OUTPATH */

#define APLL1_CLOCK             CLK_APLL294MHZ   // 294.92M
#define APLL2_CLOCK             CLK_APLL270MHZ   // 270.95M

#define IS_SYSPLL_432()         (CKGEN_READ32(REG_SYSPLL_CFG0) & RG_SYSPLL_432MHZ_EN)

//-----------------------------------------------------------------------------
// Static routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _CalDelta() to get the postive delta value between two clocks.
 *  @retval     Return the postive delta value between two clocks.
 */
//-----------------------------------------------------------------------------
static UINT32 _CalDelta(UINT32 u4Clock, UINT32 u4CalClk)
{
    if (u4Clock > u4CalClk)
    {
        return (u4Clock - u4CalClk);
    }

    return (u4CalClk - u4Clock);
}

//-----------------------------------------------------------------------------
/** _CalString() to get the divider of the calibrated PLL.
 *  @retval     Return the selective count of the PLL.
 */
//-----------------------------------------------------------------------------
static CHAR * _CalString(CAL_SRC_T eSource)
{
    switch(eSource)
    {
    case CAL_SRC_APLL1:
        return "APLL1";
    case CAL_SRC_APLL2:
        return "APLL2";
    case CAL_SRC_DMPLL:
        return "DMPLL";
    case CAL_SRC_PSPLL:
        return "PSPLL";
    case CAL_SRC_VOPLL:
        return "VPLL";
    case CAL_SRC_ADCPLL:
        return "ADCPLL";
    case CAL_SRC_SYSPLL:
        return "SYSPLL";
    default:
        return "unknown PLL";
    }
}

//-----------------------------------------------------------------------------
/** _CalSelect() to get the divider of the calibrated PLL.
 *  @retval     Return the selective count of the PLL.
 */
//-----------------------------------------------------------------------------
static UINT32 _CalSelect(CAL_SRC_T eSource)
{
    switch(eSource)
    {
    case CAL_SRC_APLL1:
        return (UINT32)APLL1_D1_CLK;
    case CAL_SRC_APLL2:
        return (UINT32)APLL2_D1_CLK;
    case CAL_SRC_DMPLL:
        return (UINT32)M_CLK_D1_CLK;
    case CAL_SRC_PSPLL:
        return (UINT32)PSPLL_D1_CLK;
    case CAL_SRC_VOPLL:
        return (UINT32)VPLL_D1_CLK;
    case CAL_SRC_ADCPLL:
        return (UINT32)ADCPLL_D1_CLK;
    case CAL_SRC_SYSPLL:
        return (UINT32)SYSPLL_D1_CLK;
    default:
        ASSERT(0);
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return 0xffU;
    }
}

static void _SetMaskOnOff(UINT32 u4Reg, UINT32 u4Mask, BOOL fgSet)
{   
    UINT32 u4Val;

    u4Val = CKGEN_READ32(u4Reg);
    if (fgSet)
    {
        u4Val |= u4Mask;
    }
    else
    {
        u4Val &= ~u4Mask;
    }
    CKGEN_WRITE32(u4Reg, u4Val);
}

static void _CalSetCalibrate(CAL_SRC_T eSource, BOOL fgSet)
{
    switch(eSource)
    {
    case CAL_SRC_APLL1:
        _SetMaskOnOff(REG_APLL1_CFG0, RG_APLL1_CAL_EN, fgSet);
        _SetMaskOnOff(REG_APLL1_CFG1, RG_APLL1_TEST_EN, fgSet);
        break;
    case CAL_SRC_APLL2:
        _SetMaskOnOff(REG_APLL2_CFG0, RG_APLL2_CAL_EN, fgSet);
        _SetMaskOnOff(REG_APLL2_CFG1, RG_APLL2_TEST_EN, fgSet);
        break;
    default:
        ASSERT(0);
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return;
    }
}

static void _CalSetBand(CAL_SRC_T eSource, UINT32 u4Band)
{
    UINT32 u4Val, u4Reg, u4Mask, u4Shift;

    switch(eSource)
    {
    case CAL_SRC_APLL1:
        u4Reg = REG_APLL1_CFG1;
        u4Mask = RG_APLL1_VCO_BAND;
        u4Shift = APLL1_VCO_BAND_SHIFT;
        break;
    case CAL_SRC_APLL2:
        u4Reg = REG_APLL2_CFG1;
        u4Mask = RG_APLL2_VCO_BAND;
        u4Shift = APLL2_VCO_BAND_SHIFT;
        break;
    default:
        ASSERT(0);
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return;
    }
    u4Val = CKGEN_READ32(u4Reg);
    u4Val &= ~(u4Mask);
    u4Val |= (u4Band << u4Shift);
    CKGEN_WRITE32(u4Reg, u4Val);

    // Delay a 1 ms to make band clock stable
    HAL_Delay_us(1000);
}

static void _CalEnablePLL(CAL_SRC_T eSource)
{
    UINT32 u4Val, u4Reg = 0, u4Bit = 0;
    BOOL fgSet = TRUE;

    switch(eSource)
    {
    case CAL_SRC_APLL1:
        // Disable APLL1
        _SetMaskOnOff(REG_APLL1_CFG1, RG_APLL1_ENABLE, 0);
        // Reset APLL1 divider
        _SetMaskOnOff(REG_APLL1_CFG3, RG_APLL1_RESET_B_CLKDIV, 0);
        // Release APLL1 divider
        _SetMaskOnOff(REG_APLL1_CFG3, RG_APLL1_RESET_B_CLKDIV, 1);
        // Enable APLL1
        _SetMaskOnOff(REG_APLL1_CFG1, RG_APLL1_ENABLE, 1);
        break;
    case CAL_SRC_APLL2:
        // Disable APLL2
        _SetMaskOnOff(REG_APLL2_CFG1, RG_APLL2_ENABLE, 0);
        // Reset APLL2 divider
        _SetMaskOnOff(REG_APLL2_CFG3, RG_APLL2_RESET_B_CLKDIV, 0);
        // Release APLL2 divider
        _SetMaskOnOff(REG_APLL2_CFG3, RG_APLL2_RESET_B_CLKDIV, 1);
        // Enable APLL2
        _SetMaskOnOff(REG_APLL2_CFG1, RG_APLL2_ENABLE, 1);
        break;
    case CAL_SRC_PSPLL:
        // This two PLL have no enable/power_down bit.
        u4Reg = REG_PSPLL_CFG0;
        u4Bit = PD_PSPLL;
        fgSet = FALSE;
        break;
    case CAL_SRC_ADCPLL:
        u4Reg = REG_ADCPLL_CFG0;
        u4Bit = PD_ADCPLL;
        fgSet = FALSE;
        break;
    case CAL_SRC_VOPLL:
        u4Reg = REG_VOPLL_CFG0;
        //u4Bit = VPLL_ENABLE;
        // write 0 to powen on vpll
        u4Bit = VPLL_PD;
        break;
    default:
        return;
    }

    u4Val = CKGEN_READ32(u4Reg);
    if (fgSet)
    {
        u4Val |= u4Bit;
    }
    else
    {
        u4Val &= ~u4Bit;
    }
    CKGEN_WRITE32(u4Reg, u4Val);
}

static void _CalSetPLL(CAL_SRC_T eSource)
{
    UNUSED(eSource);
}

static void _CalSetMN(CAL_SRC_T eSource, UINT32 u4Clock)
{
    UINT32 u4Val;
    UINT32 u4M = 0, u4N = 0;

    switch(eSource)
    {
    case CAL_SRC_PSPLL:
//        CKGEN_SetPSPLLFreq(u4Clock);
        break;
    case CAL_SRC_ADCPLL:
//        vDrvADCPLLSetFreq((u4Clock / 1000000));
        break;
    case CAL_SRC_APLL1:
        if (IS_SYSPLL_432())
        {
            if (u4Clock == CLK_APLL294MHZ)
            {
               u4M = 0x189374;
               u4N = 6;
            }
            else    // u4Clock == CLK_APLL270MHZ
            {
                u4M = 0xc36114;
                u4N = 6;
            }
        }
        else
        {
            if (u4Clock == CLK_APLL294MHZ)
            {
                u4M = 0x8d159e;
                u4N = 5;
            }
            else    // u4Clock == CLK_APLL270MHZ
            {
                u4M = 0x2e6bdc;
                u4N = 5;
            }         
        }

        // Set N value & VCOG_B vco gain control
        u4Val = CKGEN_READ32(REG_APLL1_CFG1);
        u4Val &= ~(RG_APLL1_PREDIV | RG_APLL1_VCOG_B);
        u4Val |= ((u4N << APLL1_PREDIV_SHIFT) | (DEFAULT_VCOG_B << APLL1_VCOG_B_SHIFT));
        CKGEN_WRITE32(REG_APLL1_CFG1, u4Val);
        // Set M value
        u4Val = CKGEN_READ32(REG_APLL1_CFG0);
        u4Val &= ~(RG_APLL1_MODIN);
        u4Val |= (u4M | RG_APLL1_BIASOPTEN);
        CKGEN_WRITE32(REG_APLL1_CFG0, u4Val);
        break;
    case CAL_SRC_APLL2:
        if (IS_SYSPLL_432())
        {
            if (u4Clock == CLK_APLL294MHZ)
            {
               u4M = 0x189374;
               u4N = 6;
            }
            else if (u4Clock == CLK_APLL270MHZ)
            {
                u4M = 0xc36114;
                u4N = 6;
            }
            else    // Set the true clock
            {
                u4Val = CKGEN_READ32(REG_APLL2_CFG3);
                u4Val &= ~(RG_APLL2_MODDIV);
                u4Val |= (3);
                CKGEN_WRITE32(REG_APLL2_CFG3, u4Val);
                u4N = 7;
                u4M = 0;
            }
        }
        else // SYSPLL 324MHz
        {
            if (u4Clock == CLK_APLL294MHZ)
            {
                u4M = 0x8d159e;
                u4N = 5;
            }
            else if (u4Clock == CLK_APLL270MHZ)
            {
                u4M = 0x2e6bdc;
                u4N = 5;
            }
            else if (u4Clock == CLK_231_43MHZ)
            {
                u4Val = CKGEN_READ32(REG_APLL2_CFG3);
                u4Val &= ~(RG_APLL2_MODDIV);
                u4Val |= (4);
                CKGEN_WRITE32(REG_APLL2_CFG3, u4Val);
                u4N = 7;
                u4M = 0;
            }
            else if (u4Clock == CLK_243MHZ)
            {
                u4Val = CKGEN_READ32(REG_APLL2_CFG3);
                u4Val &= ~(RG_APLL2_MODDIV);
                u4Val |= (2);
                CKGEN_WRITE32(REG_APLL2_CFG3, u4Val);
                u4N = 4;
                u4M = 0;
            }
            else if (u4Clock == CLK_259_2MHZ)
            {
                u4Val = CKGEN_READ32(REG_APLL2_CFG3);
                u4Val &= ~(RG_APLL2_MODDIV);
                u4Val |= (3);
                CKGEN_WRITE32(REG_APLL2_CFG3, u4Val);
                u4N = 5;
                u4M = 0;
            }
            else    // Set the true clock.
            {
            }

            // Select post divider mode.
            if (u4M)    // fractional-N mode.
            {
                u4Val = CKGEN_READ32(REG_APLL2_CFG1);
                u4Val &= ~(RG_APLL2_POSDIVSEL);
                CKGEN_WRITE32(REG_APLL2_CFG1, u4Val);
            }
            else        // integer-N mode.
            {
                u4Val = CKGEN_READ32(REG_APLL2_CFG1);
                u4Val |= (RG_APLL2_POSDIVSEL);
                CKGEN_WRITE32(REG_APLL2_CFG1, u4Val);
            }
        }

        // Set N value & VCOG_B vco gain control
        u4Val = CKGEN_READ32(REG_APLL2_CFG1);
        u4Val &= ~(RG_APLL2_PREDIV | RG_APLL2_VCOG_B);
        u4Val |= ((u4N << APLL2_PREDIV_SHIFT) | (DEFAULT_VCOG_B << APLL2_VCOG_B_SHIFT));
        CKGEN_WRITE32(REG_APLL2_CFG1, u4Val);
        // Set M value
        u4Val = CKGEN_READ32(REG_APLL2_CFG0);
        u4Val &= ~(RG_APLL2_MODIN);
        u4Val |= (u4M | RG_APLL2_BIASOPTEN);
        CKGEN_WRITE32(REG_APLL2_CFG0, u4Val);
        break;
    case CAL_SRC_VOPLL:
        // vDrvVOPLLSet(u4Clock, (u4Clock & 1));
        break;
    default:
        break;
    }
}

//-----------------------------------------------------------------------------
/** _CalGetPLLCounter() to get the PLL clock
 *  @retval the return value is the kHz NOT Hz.
 */
//-----------------------------------------------------------------------------
static UINT32 _CalGetPLLCounter(UINT32 u4CalSel)
{
    UINT32 u4Val;

    // Select PLL source at REG_PLL_CALIB
    u4Val = CKGEN_READ32(REG_PLL_CALIB);
    u4Val &= ~(RG_PLL_CAL_SELECT);
    u4Val |= PLL_CAL_SEL(u4CalSel);
    CKGEN_WRITE32(REG_PLL_CALIB, u4Val);

    // Set PLL_CAL_TRIGGER = 1
    u4Val = CKGEN_READ32(REG_PLL_CALIB);
    u4Val |= RG_PLL_CAL_TRIGGER;
    CKGEN_WRITE32(REG_PLL_CALIB, u4Val);

    // Wait calibration finishing.
    while (CKGEN_READ32(REG_PLL_CALIB) & RG_PLL_CAL_TRIGGER) { }

    // Calculate clock.
    u4Val = CKGEN_READ32(REG_PLL_CALIB);
    return ((60000 * (u4Val & RG_PLL_CAL_CNT)) / 256);
}

static BOOL _CalPLL(CAL_SRC_T eSource, UINT32 u4Clock)
{
    UINT32 u4Val, i, u4Band;
    UINT32 u4CalClk, u4PrevCalClk; // (u4CalClk / 1000) is x MHz;
    UINT32 u4Delta = 0xFFFFFFFF;
    BOOL fgRet = FALSE;

    u4Clock = u4Clock / 1000;
    // Select PLL source at REG_PLL_CALIB
    u4Val = CKGEN_READ32(REG_PLL_CALIB);
    u4Val &= ~(RG_PLL_CAL_SELECT);
    u4Val |= PLL_CAL_SEL(_CalSelect(eSource));
    CKGEN_WRITE32(REG_PLL_CALIB, u4Val);
    // Set CAL_EN_XXX on for the specific PLL
    _CalSetCalibrate(eSource, TRUE);

    u4PrevCalClk = 0;
    u4Band = 0;
    for (i=0; i<PLL_BAND_NUM; i++)
    {
        // Set band number to each PLLs.
        _CalSetBand(eSource, i);
        u4Val = _CalGetPLLCounter(_CalSelect(eSource));
        u4CalClk = _CalGetPLLCounter(_CalSelect(eSource));

        if (DRVCUST_InitGet(eFlagBootVerbose))
        {
            LOG(7, "Band %d calibrate %d kHz [first %d kHz]\n", i, u4CalClk, u4Val);
        }

        if ((u4CalClk > u4PrevCalClk) &&
            (u4Delta > _CalDelta(u4Clock, u4CalClk)))
        {
            u4Delta = _CalDelta(u4Clock, u4CalClk);
            u4Band = i;
            fgRet = TRUE;
        }

        if (u4PrevCalClk < u4CalClk)
        {
            u4PrevCalClk = u4CalClk;
        }
    }

    _CalSetBand(eSource, u4Band);
    _CalSetCalibrate(eSource, FALSE);

    // Delay a 1 ms to make close loop clock stable
    HAL_Delay_us(1000);

    if (DRVCUST_InitGet(eFlagBootVerbose))
    {
        u4CalClk = _CalGetPLLCounter(_CalSelect(eSource));
        LOG(5, "%s use band %d now is %d kHz\n", _CalString(eSource), u4Band, u4CalClk);
    }

    return fgRet;
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
}

BOOL BSP_Calibrate(CAL_SRC_T eSource, UINT32 u4Clock)
{
    BOOL fgRet = FALSE;
    UINT32 u4CalClk;

    // Before Calibration, enable the PLL.
    _CalEnablePLL(eSource);

    _CalSetMN(eSource, u4Clock);
    switch(eSource)
    {
    case CAL_SRC_APLL1:
        fgRet = _CalPLL(eSource, u4Clock);
        break;
    case CAL_SRC_APLL2:
        fgRet = _CalPLL(eSource, u4Clock);
        break;
    case CAL_SRC_DMPLL:
    case CAL_SRC_PSPLL:
    case CAL_SRC_VOPLL:
    case CAL_SRC_ADCPLL:
        fgRet = TRUE;
        if (DRVCUST_InitGet(eFlagBootVerbose))
        {
            // Delay a 1 ms to make close loop clock stable
            HAL_Delay_us(1000);

            u4CalClk = _CalGetPLLCounter(_CalSelect(eSource));
            Printf("%s now is %d kHz\n", _CalString(eSource), u4CalClk);
        }
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
    UINT32 u4Val, u4Clock = 0;

    UNUSED(pu2MS);
    UNUSED(pu2NS);
    UNUSED(pu1Band);

    switch(eSource)
    {
    case CAL_SRC_APLL1:
        u4Clock = _CalGetPLLCounter((UINT32)APLL1_D1_CLK);
        u4Clock = u4Clock * 1000;
        break;
    case CAL_SRC_APLL2:
        u4Clock = _CalGetPLLCounter((UINT32)APLL2_D1_CLK);
        u4Clock = u4Clock * 1000;
        break;
    case CAL_SRC_DMPLL:
        u4Clock = _CalGetPLLCounter((UINT32)M_CLK_D1_CLK);
        u4Clock = u4Clock * 1000;   // DMPLL measure is M_CLK.
        if (pu2Counter)
        {
            u4Val = CKGEN_READ32(REG_PLL_CALIB);
            *pu2Counter = u4Val & RG_PLL_CAL_CNT;
        }
        // DMPLL is 2*M_CLK
        u4Clock = u4Clock * 2;
        break;
    case CAL_SRC_TVDPLL:
        u4Clock = _CalGetPLLCounter((UINT32)TVDPLL_D3_CLK);
        u4Clock = (u4Clock * 1000) * 3;
        break;
    case CAL_SRC_DTDPLL:
        u4Clock = _CalGetPLLCounter((UINT32)DTDPLL_D3_CLK);
        u4Clock = (u4Clock * 1000) * 3;
        break;
    case CAL_SRC_PSPLL:
        u4Clock = _CalGetPLLCounter((UINT32)PSPLL_D1_CLK);
        u4Clock = u4Clock * 1000;
        break;
    case CAL_SRC_VOPLL:
        u4Clock = _CalGetPLLCounter((UINT32)VPLL_D1_CLK);
        u4Clock = u4Clock * 1000;
        break;
    case CAL_SRC_ADCPLL:
        u4Clock = _CalGetPLLCounter((UINT32)ADCPLL_D1_CLK);
        u4Clock = u4Clock * 1000;
        break;
    case CAL_SRC_SYSPLL:
        u4Clock = _CalGetPLLCounter((UINT32)SYSPLL_D1_CLK);
        u4Clock = u4Clock * 1000;
        break;
    case CAL_SRC_HDMIPLL:
        u4Clock = _CalGetPLLCounter((UINT32)HDMIPLL_D3_CLK);
        u4Clock = u4Clock * 1000 * 3;
        break;
    case SRC_CPU_CLK:
        switch(CKGEN_READ32(REG_CPU_CKCFG) & CPU_CK_SEL_MASK) {
        case 0: u4Clock = 27000000; break;
        case 1: u4Clock = 270000000; break;
        case 2: u4Clock = 180000000; break;
        case 3: u4Clock = 135000000; break;
        case 4: u4Clock = 108000000; break;
        case 5: u4Clock = 300000000; break;
        case 6: u4Clock = 200000000; break;
        case 7: u4Clock = 150000000; break;
        case 8: u4Clock = 120000000; break;
        case 9: u4Clock = (IS_SYSPLL_432()) ? 216000000 : 162000000; break;
        case 10: u4Clock = (IS_SYSPLL_432()) ? 144000000 : 108000000; break;
        case 11: u4Clock = BSP_GetDomainClock(CAL_SRC_APLL1); break;
        case 12: u4Clock = BSP_GetDomainClock(CAL_SRC_APLL2); break;
        case 13: u4Clock = BSP_GetDomainClock(SRC_MEM_CLK); break;
        default: u4Clock = 200000000; break;
        }
        break;
    case SRC_MEM_CLK:
        u4Clock = _CalGetPLLCounter((UINT32)M_CLK_D1_CLK);
        u4Clock = u4Clock * 1000;   // DMPLL measure is M_CLK.
        break;
    case SRC_BUS_CLK:
        // XXX fix me.
        u4Clock = BSP_GetDomainClock(SRC_CPU_CLK) >> 1;
        break;
    default:
        break;
    }

    return u4Clock;
}

