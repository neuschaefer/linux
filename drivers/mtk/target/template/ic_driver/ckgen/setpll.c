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
#include "x_typedef.h"
#include "x_ckgen.h"
#include "x_timer.h"

//=====================================================================
// Configuration
#define PSCANPLL_REF_NCOCLK     1       // default should be 1

#define VPLL_PREV_DIVIDER_ON    0       // vpll pre-divider on/off flag.


//=====================================================================
// Constant definitions
// VIDEO_IN0 register
#define VPLL_MULTIPLIER 2
//#define VPLL_VCOG_B_THRESHOLD 100000000
#define VPLL_BAND_THRESHOLD 120000000
#define VPLL_BAND_THRESHOLD_L 30000000	

#define REG_RW_ADCPLL_DCLK_FREQ_CTRL    (0x04d4)
    #define ADCPLL_FREQ_CW              (0x0fffffff)
    #define ADCPLL_FREQ_INT_SHIFT       (24)
#define REG_RW_VPLL_DDS_PERIOD          (0x0ca0) //#define REG_RW_VPLL_DDS_PERIOD          (0x04a0)
    #define VPLL_DDS_INIT               (1U << 31)
#define REG_RW_VPLL_SPREAD_FREQ_CTRL    (0x0ca4)//#define REG_RW_VPLL_SPREAD_FREQ_CTRL    (0x04a4)
    #define VPLL_SPREAD_INIT            (1U << 31)
    #define VPLL_SPREAD_CW              (0x0fffffff)
    #define VPLL_SPREAD_INT_SHIFT       (24)
#define REG_RW_PSPLL_NCO_FREQ_CTRL      (0x05c8)
    #define PS_NCO_INIT                 (1U << 31)
    #define PS_NCO_CW                   (0x0fffffff)
    #define PS_NCO_INT_SHIFT            (24)
#define REG_RW_B2RPLL_NCO_FREQ_CTRL     (0x05d0)
    #define B2R_NCO_INIT                (1U << 31)
    #define B2R_NCO_CW                  (0x0fffffff)
    #define B2R_NCO_INT_SHIFT           (24)

static BOOL _fgVopllUseDDDS = TRUE;
static UINT32 _u4CurrentVClk = 0;
static BOOL _fgDualChannel = FALSE;

/**
 * @brief set VOPLL clock
 *
 * Set video output PLL frequency
 * @param bCLK is the target VOPLL clock rate (MHz)
 * @retval void
 */
void vDrvVOPLLSet(UINT32 u4Freq, BOOL fgLVDSDiv2)
{
    static BOOL fgInit = FALSE;
    static UINT32 u4VpllCfg0 = 0;
    UINT32 u4Val, u4Dval = 1;
    UINT32 u4Syspll;
    UINT64 u8Cw;

    if (u4Freq == 0)
    {
        return;
    }

    // save current value
    _u4CurrentVClk = u4Freq;
    _fgDualChannel = fgLVDSDiv2;

    if (!fgInit)
    {
        // 0. power on vpll...this should move to ckgen.c later
//        u4Val = CKGEN_READ32(REG_VOPLL_CFG0);
//        u4Val &= ~(VPLL_PD);
//        CKGEN_WRITE32(REG_VOPLL_CFG0, u4Val);

        u4Val = CKGEN_READ32(0x0144);
        u4Val &= ~(1<<7);
        CKGEN_WRITE32(0x0144, u4Val);


        // luis@2007/7/3, to make vopll more stable
//        u4Val = CKGEN_READ32(REG_VOPLL_CFG1);
//        u4Val &= ~(VPLL_LPFR | VPLL_ICP);
//        u4Val |= (VPLL_LPFR_4K | VPLL_ICP_20UA);
//        CKGEN_WRITE32(REG_VOPLL_CFG1, u4Val);

        fgInit = TRUE;
    }

    if (_fgVopllUseDDDS)
    {
        // use ddds
        //IO_WRITE32MSK(CKGEN_BASE, 0x164, 1 << 7, 1 << 7);
        IO_WRITE32MSK(CKGEN_BASE, 0x160, 1 <<22, 1 << 22);
    }
    else
    {
        // use xtal
        //IO_WRITE32MSK(CKGEN_BASE, 0x164, 0, 1 << 7);
	 IO_WRITE32MSK(CKGEN_BASE, 0x160, 0, 1 << 22);
    }

    if (_fgVopllUseDDDS)
    {
        // 1. setup ddds start -------------------------------------------------
        // rise ddds spread init, bit[31]
        u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_VPLL_DDS_PERIOD);
        u4Val |= VPLL_DDS_INIT;
        IO_WRITE32(VIDEO_IN0_BASE, REG_RW_VPLL_DDS_PERIOD, u4Val);

        // calculate control word
        u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_VPLL_SPREAD_FREQ_CTRL);
        u4Syspll = BSP_GetPLLClock(CAL_SRC_SYSPLL);
        u8Cw = (UINT64)u4Syspll;

        if (u4Freq < CLK_50MHZ)
        {
            u4Dval = 0;  /* CW from 6 ~ 12 */
        }
        else if (u4Freq < CLK_81MHZ)
        {
            u4Dval = 1;  /* CW from 6 ~ 12 */
        }
        else
        {
            u4Dval = 4;  /* CW from 6 ~ 12 */
        }

        // becuz we will set vpll divide by 2
        u8Cw = ((u8Cw * ((UINT64)u4Dval + 2)) >> 1);
        u8Cw = (u8Cw << VPLL_SPREAD_INT_SHIFT) / u4Freq;
        u4Val &= ~(VPLL_SPREAD_CW);
        u4Val |= (((UINT32)u8Cw) & VPLL_SPREAD_CW);
        IO_WRITE32(VIDEO_IN0_BASE, REG_RW_VPLL_SPREAD_FREQ_CTRL, u4Val);

        // enable spread spectrum
        u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_VPLL_DDS_PERIOD);
        u4Val &= ~(VPLL_DDS_INIT);
        IO_WRITE32(VIDEO_IN0_BASE, REG_RW_VPLL_DDS_PERIOD, u4Val);

        // setup ddds done -----------------------------------------------------
    }
    else
    {
        u4Dval = (((((u4Freq) * 2) + 27000000) - 1) / 27000000) - 2;
    }

    // 2. setup vpll start -----------------------------------------------------
    // adjust VPLL
    u4Val = CKGEN_READ32(REG_VOPLL_CFG0);
    u4Val &= ~(VPLL_DIV | VPLL_DIV12 | VPLL_BAND | VPLL_LVDS_CLK_DIV2);

    u4Val |= (1 << VPLL_DIV12_SHIFT) | (u4Dval << VPLL_DIV_SHIFT);

    // when dual channel, lvds clk need to divide 2
    if (fgLVDSDiv2)
    {
    #if 0 
        u4Val |= VPLL_LVDS_CLK_DIV2;
    #else	
	 u4Val = CKGEN_READ32(0x0144);
	 u4Val |= (1<<22);
        CKGEN_WRITE32(0x0144, u4Val);
     #endif		
    }
#if 0    
    // check band
    if (u4Freq > VPLL_BAND_THRESHOLD)
    {
        u4Val |= VPLL_BAND_LARGE; // clk > 120MHz
    }
    else if(u4Freq<VPLL_BAND_THRESHOLD_L)
    {
	u4Val |= VPLL_BAND_SMALL; // clk < 30MHz
    }
    else
    {
        u4Val |= VPLL_BAND_MEDIUM;
    }
#else
    // check band
    u4Val = CKGEN_READ32(0x0140);
    if (u4Freq > VPLL_BAND_THRESHOLD)
    {
        u4Val |= (4<<28); // clk > 120MHz
    }
    else if(u4Freq<VPLL_BAND_THRESHOLD_L)
    {
	u4Val |= (5<<28); // clk < 30MHz
    }
    else
    {
        u4Val |= (6<<28);
    }
	CKGEN_WRITE32(0x0140, u4Val);

#endif
    // reset phase only when vpll change to avoid display defect
#if 0    
    if (u4VpllCfg0 != u4Val)
    {
        // toggle REG_VOPLL_CFG0

        u4Val |= (VPLL_PHASE_RST);
        CKGEN_WRITE32(REG_VOPLL_CFG0, u4Val);
        // delay enough time for HW reset
        HAL_Delay_us(100);
        u4Val &= ~(VPLL_PHASE_RST);
        CKGEN_WRITE32(REG_VOPLL_CFG0, u4Val);

        u4VpllCfg0 = u4Val;
    }		
#else
    // reset phase only when vpll change to avoid display defect
    u4Val = CKGEN_READ32(0x0144);
    if (u4VpllCfg0 != u4Val)
    {
        // toggle REG_VOPLL_CFG0
        u4Val |= (1<<23);
        CKGEN_WRITE32(0x0144, u4Val);
        // delay enough time for HW reset
        HAL_Delay_us(100);
        u4Val &= ~(1<<23);
        CKGEN_WRITE32(0x0144, u4Val);

        u4VpllCfg0 = u4Val;
    }
#endif
    // setup vpll done ---------------------------------------------------------
}

/**
 * @brief set ADCPLL frequency
 *
 * Set ADCPLL rough frequency
 * @param bADCCLK bADCCLK is the target frequency (MHz)
 * @retval void
 */
void vDrvADCPLLSetFreq(UINT8 bADCCLK)
{
    UINT32 u4Val;

    u4Val = bADCCLK;
    u4Val = u4Val * 1000000;
    CKGEN_SetADCPLLFreq(u4Val);
}

void CKGEN_SetVOPLLInputAsDDDS(BOOL fgDDDS)
{
    _fgVopllUseDDDS = fgDDDS;
    if (_u4CurrentVClk != 0)
    {
        vDrvVOPLLSet(_u4CurrentVClk, _fgDualChannel);
    }

    if (BSP_GetIcVersion() < IC_VER_AC)
    {
        if (fgDDDS)
        {
            // power down sdds
            //IO_WRITE32MSK(VIDEO_IN0_BASE, 0x490, 0, 1 << 30);
            IO_WRITE32MSK(VIDEO_IN0_BASE, 0xc90, 0, 1 << 30);
        }
        else
        {
            // power on sdds
            //IO_WRITE32MSK(VIDEO_IN0_BASE, 0x490, 1 << 30, 1 << 30);
            IO_WRITE32MSK(VIDEO_IN0_BASE, 0xc90, 1 << 30, 1 << 30);
        }
    }
}

void CKGEN_SetVOPLLFreq(UINT32 u4Freq)
{
    vDrvVOPLLSet(u4Freq, FALSE);
}

void CKGEN_SetADCPLLFreq(UINT32 u4Freq)
{
    UINT32 u4Val, u4Ns, u4Rev, u4Syspll;
    UINT64 u8Cw;
    IC_VERSION_T eVer;

    eVer = BSP_GetIcVersion();

    if (eVer <= IC_VER_AB) {
        u4Ns = 2;   // 5 is recommend for 162MHz, 2 for 81MHz. Xtal.
        u4Rev = 1;
    } else {
        u4Rev = 1;
        if (u4Freq < CLK_50MHZ) {
            u4Ns = 1;   /* CW from 6 ~ 12 */
        } else if (u4Freq < CLK_81MHZ) {
            u4Ns = 2;   /* CW from 6 ~ 12 */
        } else {
            u4Ns = 5;   /* CW from 6 ~ 12 */
        }
    }
    u4Syspll = BSP_GetPLLClock(CAL_SRC_SYSPLL);
    u8Cw = (UINT64)u4Syspll;
    u8Cw = ((u8Cw * ((UINT64)u4Ns + 1)) / ((UINT64)u4Rev + 1));
    u4Val = ((UINT32)((u8Cw << ADCPLL_FREQ_INT_SHIFT) / u4Freq)) & ADCPLL_FREQ_CW;
    IO_WRITE32(VIDEO_IN0_BASE, REG_RW_ADCPLL_DCLK_FREQ_CTRL, u4Val);

    u4Val = CKGEN_READ32(REG_ADCPLL_CFG1);
    u4Val &= ~(ADC_NS | ADC_REV | VCO_BAND_ADC);
    u4Val |= ((u4Ns << ADC_NS_SHIFT) | (u4Rev << ADC_REV_SHIFT)
                | ADC_EN_SEFBIA);
    CKGEN_WRITE32(REG_ADCPLL_CFG1, u4Val);

    u4Val = CKGEN_READ32(REG_ADCPLL_CFG0);
    u4Val &= ~(VCO_GAIN_ADC | V2I_RB_ADC | DIV_SEL_ADC);
    u4Val |= ((2 << V2I_RB_ADC_SHIFT) | (1 << DIV_SEL_ADC_SHIFT));

    if (u4Freq < CLK_30MHZ)   // 10MHz~50MHz
    {
        u4Val |= (1 << VCO_GAIN_ADC_SHIFT);
    }
    else if (u4Freq < CLK_100MHZ) // 20MHz~140MHz
    {
        u4Val |= (3 << VCO_GAIN_ADC_SHIFT);
    }
    else    // 50MHz~200MHz
    {
        u4Val |= (5 << VCO_GAIN_ADC_SHIFT);
    }
    CKGEN_WRITE32(REG_ADCPLL_CFG0, u4Val);
}

void CKGEN_SetPSPLLFreq(UINT32 u4Freq)
{
    UINT32 u4Val, u4Nco, u4Syspll;
    UINT64 u8Cw;
    IC_VERSION_T eVer;

    eVer = BSP_GetIcVersion();
    // To enable/initialize PScan NCO clock
    u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_PSPLL_NCO_FREQ_CTRL);
    u4Val &= ~(PS_NCO_CW);
    u4Val |= PS_NCO_INIT;
    IO_WRITE32(VIDEO_IN0_BASE, REG_RW_PSPLL_NCO_FREQ_CTRL, u4Val);

    // PSPLL_CLK = ((NCO_CLK x 4)/(DIV_SEL_PS)) * (1/2)
    u4Val = CKGEN_READ32(REG_PSPLL_CFG0);
    u4Val &= ~(V2I_RB_PS | VCO_GAIN_PS | CKSEL_0_PS |
                CKSEL_1_PS | PD_PSPLL | DIV_SEL_PS);
    u4Val |= ((1 << V2I_RB_PS_SHIFT) | (5 << VCO_GAIN_PS_SHIFT));
    u4Val |= (CKSEL_0_PS);

    // Must the same as u4Freq shifter
    // Set DIV_SEL_PS is 1, 2, or 4.
    u4Val |= ((u4Freq > CLK_54MHZ) ? (PSPLL_DIV_1 << DIV_SEL_PS_SHIFT) :
                ((u4Freq > CLK_27MHZ) ? (PSPLL_DIV_2 << DIV_SEL_PS_SHIFT) :
                                        (PSPLL_DIV_4 << DIV_SEL_PS_SHIFT)));
    CKGEN_WRITE32(REG_PSPLL_CFG0, u4Val);

    u4Val = CKGEN_READ32(REG_PSPLL_CFG1);
    u4Val &= ~(PS_REV | VCO_BAND_PS | CHP_PS);

    if (eVer <= IC_VER_AB) {
        // 2U to open NCO
        u4Val |= ((2U << PS_REV_SHIFT) | (1U << CHP_PS_SHIFT));
    } else {
        // 0x62 to open NCO, to set All PLLs related to NCO works.
        u4Val |= ((0x62U << PS_REV_SHIFT) | (1U << CHP_PS_SHIFT));
    }
    CKGEN_WRITE32(REG_PSPLL_CFG1, u4Val);

    // Must the same as PSPLL_DIV_#
    // u4Nco = u4Freq * DIV_SEL_PS / 2;
    u4Nco = ((u4Freq > CLK_54MHZ) ? (u4Freq >> 1) :
            ((u4Freq > CLK_27MHZ) ? (u4Freq) :
                                    (u4Freq << 1)));

    u4Syspll = BSP_GetPLLClock(CAL_SRC_SYSPLL);
    // (NCO clock)= syspll/[27:24].[23:0], for example, 148.5MHz => 0x45d1745
    u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_PSPLL_NCO_FREQ_CTRL);
    u4Val &= ~(PS_NCO_CW);
    u8Cw = u4Syspll;
    u8Cw = (u8Cw << PS_NCO_INT_SHIFT);
    u8Cw = (u8Cw / u4Nco);
    u4Val |= ((UINT32)u8Cw & PS_NCO_CW);
    IO_WRITE32(VIDEO_IN0_BASE, REG_RW_PSPLL_NCO_FREQ_CTRL, u4Val);
}

void CKGEN_SetB2RPLLFreq(UINT32 u4Freq)
{
#if 0
    UINT32 u4Val, u4Nco, u4Syspll;
    UINT64 u8Cw;

    // To enable/initialize B2R NCO clock
    u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_B2RPLL_NCO_FREQ_CTRL);
    u4Val &= ~(B2R_NCO_CW);
    u4Val |= B2R_NCO_INIT;
    IO_WRITE32(VIDEO_IN0_BASE, REG_RW_B2RPLL_NCO_FREQ_CTRL, u4Val);

    // B2RPLL = NCO_CLK x 4
    u4Val = CKGEN_READ32(REG_B2RPLL_CFG0);
    u4Val &= ~(PD_B2RPLL);
    u4Val |= (CKSEL_B2R | (1U << 25));  // XXX why bit 25?
    CKGEN_WRITE32(REG_B2RPLL_CFG0, u4Val);

    // u4Nco = u4Freq / 4;
    u4Nco = (u4Freq >> 2);
    u4Syspll = BSP_GetPLLClock(CAL_SRC_SYSPLL);
    // (NCO clock)= syspll/[27:24].[23:0], for example, 148.352 MHz => 0x8bc6ace
    u4Val = IO_READ32(VIDEO_IN0_BASE, REG_RW_B2RPLL_NCO_FREQ_CTRL);
    u4Val &= ~(B2R_NCO_CW);
    u8Cw = u4Syspll;
    u8Cw = (u8Cw << B2R_NCO_INT_SHIFT);
    u8Cw = ((u8Cw + (u4Nco >> 1)) / u4Nco);

    if (u4Freq == B2RPLL_DEFAULT_CLOCK)
    {
        u4Val |= 0x8bc6ace; // 148.352 MHz, why? 0x8bc6924 is more close, XXX
    }
    else
    {
        u4Val |= ((UINT32)u8Cw & B2R_NCO_CW);
    }
    IO_WRITE32(VIDEO_IN0_BASE, REG_RW_B2RPLL_NCO_FREQ_CTRL, u4Val);
#else
    UNUSED(u4Freq);
#endif
}

