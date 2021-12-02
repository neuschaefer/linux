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
/*-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: aud_cfg.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_cfg.c
 *  Brief of file aud_cfg.c.
 *  Details of file aud_cfg.c (optional).
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
// KERNEL


LINT_EXT_HEADER_BEGIN

//#include "adc_if.h"
#include "x_util.h"
#include "x_bim.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_ckgen.h"
#include "x_pinmux.h"
#include "aud_drvif.h"
#include "x_os.h"

#include "sif_if.h"
#include "aud_debug.h"
#include "aud_cfg.h"
#include "adac_if.h"
#include "spdif_drvif.h"
#include "aud_if.h"
#include "aud_hw.h"
#include "aud_drv.h"
#include "psr_drvif.h"
#include "drv_adsp.h"
#include "drvcust_if.h"
#include "dsp_intf.h"
#ifdef CC_MT5391_AUD_SUPPORT
#include "../periph/codec_mt8297.h"
#else
#include "../periph/codec_mt536xinternal.h"
#endif

#include "dsp_shm.h"
#ifdef CC_MT5387
#include "hw_vdoin.h"
#include "hw_ckgen.h"
#include "../periph/codec_mt5387.h"
#endif

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//Laihui: Remove it for APLL setting
//#define APLL_WORKAROUND
//#define SUPPORT_DUAL_UART1

#define USE_16BIT_ALIGNMENT

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define APLL_ID_MAIN            0
#define APLL_ID_AUX             1

#define K2_VALID_VALUE_MIN      2
#define K2_VALID_VALUE_MAX      48

#define APLL_K2_MASK            (0x3f << 24)
#define APLL_K1_MASK            ((UINT32)1 << 31)

#define AUD_GPIO_SWITCH_0       (17)
#define AUD_GPIO_SWITCH_1       (15)
#define SW_NUMBER               (8)

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

typedef enum
{
    I2S_IN_LINEIN = 0,  // Line in
    I2S_IN_SPDIF,       // SPDIF
    I2S_IN_HDMI         // HDMI
}   I2S_IN_SEL;

//---------------------------------------------------------------------------
// Macro
//---------------------------------------------------------------------------

#define    REGISTER_ACCESS_TIME_BARRIER(u4Tick)      {  \
        volatile UINT32 i, j; \
        for (i=0; i<u4Tick; i++)  \
        {                     \
            j = i;            \
            UNUSED(j);  \
        }                     \
    }



#define SAMP_CHG_LOCK()      VERIFY(x_sema_lock(_hSmpChgSema, X_SEMA_OPTION_WAIT) == OSR_OK)
#define SAMP_CHG_UNLOCK()    VERIFY(x_sema_unlock(_hSmpChgSema) == OSR_OK)

#define AUD_DEC_ID_VALIDATE(u1DecId)    VERIFY(u1DecId < AUD_DEC_NUM)
#if defined(CC_MT8223)
#define RO_BIM_CHIP_ID        0x01FC      // 32'ha61223a0
#define IC_VER_MT8223_AB 0xB61223A0
#define Is_Chip_MT8223_E1()    ( BIM_READ32(RO_BIM_CHIP_ID) == IC_VER_MT8223_AB)
#endif


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
extern void vSetSpdifOutPort(UINT8 u1DecId, BOOL fgEnable);
extern void AUD_AoutPADMux(AUD_CH_T ePAD_SDATA0, AUD_CH_T ePAD_SDATA1, AUD_CH_T ePAD_SDATA2,
                               AUD_CH_T ePAD_SDATA3, AUD_CH_T ePAD_SDATA4, AUD_CH_T ePAD_SDATA5,
                               UINT8 PAD_MCLK0);
extern UINT32 u4GetDspReadPtr(UINT8 u1DecId);
extern void vDspSetFifoReadPtr(UCHAR ucDecId, UINT32 u4ReadPtr);
extern BOOL fgDspReadPtrSet (UCHAR ucDecId, UINT32 u4Address);
extern BOOL fgHDMIAudioClkOk(void);

extern void AUD_DspDvdEnable(void);
extern void AUD_DspDvdDisable(void);
extern void vHDMIHandleAudFmtChange(void);
extern void bHDMIClockDetect(void);
#ifdef CC_MT5391_AUD_SUPPORT
extern void _MT8297_DacChlSel(UINT8 u1DacId, AUD_CHL_DEF_T eChl);
#else
extern void _MT536xInternal_DacChlSel(UINT8 u1DacId, AUD_CHL_DEF_T eChl);
#endif

#ifdef CC_MT5387
//extern void _MT5387_ADCDOWN(BOOL fgEnalbe);
#endif

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------
UINT32 AUD_DspGetPc(void);
UINT32 AUD_DspProbePause(void);
UINT32 AUD_DspProbeGo(void);
UINT32 AUD_DspProbeStep(UINT32 u4Count);
UINT32 fgAUD_DspProbeHalt(void);
UINT32 u4AUD_DspProbeRead(UINT32 u4Addr);
UINT32 AUD_DspProbeWrite(UINT32 u4Addr, UINT32 u4Data);
UINT32 u4AUD_DspProbePBCfg(BOOL fgSet, UINT32 u4Value);
UINT32 u4AUD_DspProbePB(BOOL fgSet, UINT32 u4BPNum, UINT32 u4Addr);
UINT32 u4AUD_DspProbeStopOnWrite(UINT32 u4Addr);
UINT32 u4AUD_DspProbeChkStopOnWrite(BOOL *fgEnable);
BOOL AUD_IsHdmiClkExist(void);
static void AUD_LineinLRDetect(BOOL fgEnable);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static AOUT_CFG_T    _arAudOutCfg[2];
static AIN_CFG_T     _rLineInCfg;
static AIN_CFG_T     _rHDMIInCfg;
static AIN_CFG_T     _rSPDIFInCfg;
static HANDLE_T     _hSmpChgSema;

//static AIN_CFG_T     _rHwI2SInCfg;
//static AUD_DEC_STREAM_FROM_T    _eHwI2SInSel = AUD_STREAM_FROM_LINE_IN;

#ifdef CC_MT5391_AUD_SUPPORT
static UINT32        _au4ApllClk[2]   = {CLK_APLL294MHZ, CLK_APLL270MHZ};
static UINT32        _au4Apll1ClkBand[2]   = {1, 1};
static UINT32        _au4Apll2ClkBand[2]   = {1, 1};
#else
static UINT32        _au4ApllClk[2]   = {CLK_APLL294MHZ, CLK_APLL270MHZ};
static UINT32        _au4Apll1ClkBand[2]   = {2, 2};
static UINT32        _au4Apll2ClkBand[2]   = {2, 2};
#endif
static UINT8         _au1MckDrvCur[2] = {2, 2};
static UINT8         _au1BckDrvCur[2] = {2, 2};
static UINT8         _au1LRckDrvCur[2] = {2, 2};
static UINT8         _au1AOSDATADrvCur[2] = {2, 2};

static BOOL _fgInit = FALSE;
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
static AUD_DEC_STREAM_FROM_T _aeStrSrcConf[AUD_DEC_NUM] = {AUD_STREAM_FROM_DIGITAL_TUNER, AUD_STREAM_FROM_DIGITAL_TUNER};
static BOOL _fgChangeSource[AUD_DEC_NUM] = {TRUE, TRUE};
static SAMPLE_FREQ_T _aeSampleRate[2] = {FS_48K, FS_48K};
#else
static AUD_DEC_STREAM_FROM_T _aeStrSrcConf[AUD_DEC_NUM] = {AUD_STREAM_FROM_DIGITAL_TUNER, AUD_STREAM_FROM_DIGITAL_TUNER, AUD_STREAM_FROM_DIGITAL_TUNER};
static BOOL _fgChangeSource[AUD_DEC_NUM] = {TRUE, TRUE, TRUE};
static SAMPLE_FREQ_T _aeSampleRate[3] = {FS_48K, FS_48K, FS_48K};
#endif
static AUD_INPUT_ID_T _eInputIdToDvdGpioConfig;
static BOOL _fgDualDecMode = FALSE;
static AUD_INTER_DAC_CFG_T _rInterDacCfg;

#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
static SAMPLE_FREQ_T _aePcmSmpFreq[AUD_DEC_NUM] = {FS_44K, FS_44K};
static AUD_FMT_T     _aeDecFmt[AUD_DEC_NUM] = {AUD_FMT_AC3, AUD_FMT_AC3};
#else
static SAMPLE_FREQ_T _aePcmSmpFreq[AUD_DEC_NUM] = {FS_44K, FS_44K, FS_44K};
static AUD_FMT_T     _aeDecFmt[AUD_DEC_NUM] = {AUD_FMT_AC3, AUD_FMT_AC3, AUD_FMT_MPEG};
#endif



//---------------------------------------------------------------------------
// Global variables
//---------------------------------------------------------------------------
#ifdef CC_AUD_EU_INTERNALDAC_SAME_APLL
AUD_AOUT_PAD_CFG_T _grAOutPadCfg = {AUD_CH_AUX_FRONT_LEFT, AUD_CH_REAR_LEFT,
                                    AUD_CH_CENTER, AUD_CH_BYPASS_LEFT,
                                    AUD_CH_FRONT_LEFT, AUD_CH_AUX_FRONT_LEFT
                                   };

#else
AUD_AOUT_PAD_CFG_T _grAOutPadCfg = {AUD_CH_FRONT_LEFT, AUD_CH_REAR_LEFT,
                                    AUD_CH_CENTER, AUD_CH_BYPASS_LEFT,
                                    AUD_CH_FRONT_LEFT, AUD_CH_AUX_FRONT_LEFT
                                   };

#endif

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

#if 0 //SHINK_8223_CODE
static BOOL _IsInterDacRefMainDec(UINT8 u1DacId)
{
    if (u1DacId == INTERNAL_DAC_ID_MAIN)
    {
        return (_rInterDacCfg.eDac0ChlSel != AUD_CHL_AUX);
    }
    else if (u1DacId == INTERNAL_DAC_ID_AUX)
    {
        return (_rInterDacCfg.eDac1ChlSel != AUD_CHL_AUX);
    }
    else
    {
        return (_rInterDacCfg.eDac2ChlSel != AUD_CHL_AUX);
    }
#if 0
    return  (u1DacId == INTERNAL_DAC_ID_MAIN) ?
            (_rInterDacCfg.eDac0ChlSel != AUD_CHL_AUX) :
            (_rInterDacCfg.eDac1ChlSel != AUD_CHL_AUX);
#endif
}
static BOOL _IsInterDacRefAuxDec(UINT8 u1DacId)
{

    if (u1DacId == INTERNAL_DAC_ID_MAIN)
    {
        return (_rInterDacCfg.eDac0ChlSel == AUD_CHL_AUX);
    }
    else if (u1DacId == INTERNAL_DAC_ID_AUX)
    {
        return (_rInterDacCfg.eDac1ChlSel == AUD_CHL_AUX);
    }
    else
    {
        return (_rInterDacCfg.eDac2ChlSel == AUD_CHL_AUX);
    }
#if 0
    return  (u1DacId == INTERNAL_DAC_ID_MAIN) ?
            (_rInterDacCfg.eDac0ChlSel == AUD_CHL_AUX) :
            (_rInterDacCfg.eDac1ChlSel == AUD_CHL_AUX);
#endif
}
#endif

#if 0 //8223_CODE_SHINK 
static BOOL _Is294DivClk(SAMPLE_FREQ_T eFs)
{
    if ((eFs == FS_16K) || (eFs == FS_24K) || (eFs == FS_32K) || (eFs == FS_48K) ||
            (eFs == FS_64K) || (eFs == FS_96K) || (eFs == FS_192K))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif
#if 0
BOOL _AudBSPCalibrate(CAL_SRC_T eSource, UINT32 u4Clock)
{
    BOOL fgRet = FALSE;

    UINT32 u4CalClk;

    // Before Calibration, enable the PLL.
    _CalEnablePLL(eSource);

    _CalSetMN(eSource, u4Clock);
    switch (eSource)
    {
        case CAL_SRC_APLL1:
            fgRet = _CalPLL(eSource, u4Clock);
            break;
        case CAL_SRC_APLL2:
            fgRet = _CalPLL(eSource, u4Clock);
            break;
        default:
            return fgRet;
    }

    // Do the post settings.
    _CalSetPLL(eSource);

    return fgRet;
}
#endif

#ifdef CC_MT5391_AUD_SUPPORT
static void _AudDemodSetting(void)
{
    _MT8297_DirectWRITE(0xB6, 0x6B, 0x7B);
    _MT8297_DirectWRITE(0xB8, 0x0c, 0x0f);
    _MT8297_DirectWRITE(0x14, 0x03, 0x03);
    //demod frontEnd
    _MT8297_InDirectWRITE(0x73, 0x00, 0xff);
    _MT8297_InDirectWRITE(0x74, 0x04, 0xff);//SIF signal to CH_A
    _MT8297_InDirectWRITE(0x77, 0xe0, 0xff);//for CH_A, SIF enable + PGA GAIN=0bB, need to Calibration
    _MT8297_InDirectWRITE(0x78, 0x06, 0xff);//BW of SIF signal(CH_A)
    _MT8297_InDirectWRITE(0x7e, 0x00, 0xff);//PGA offset for CH_A, -30mV, need to Calibration
    _MT8297_InDirectWRITE(0x80, 0x04, 0xff);//setting ref VOLT for CH_A

    //demod CLK setting
    _MT8297_InDirectWRITE(0x0d, 0x40, 0xff);
    _MT8297_InDirectWRITE(0x0e, 0x02, 0xff);

    //DADC
    _MT8297_DirectWRITE(0x9c, 0xA2, 0xE4);

    //demod AD channel selection
    _MT8297_DirectWRITE(0x9e, 0x07, 0x07); //set CH_A & CH_B

    //demod
    _MT8297_DirectWRITE(0x8a, 0x80, 0x80); //demode sharing for CVBS/SIF
    _MT8297_DirectWRITE(0x8d, 0x00, 0x10);

    _MT8297_DirectWRITE(0x81, 0x60, 0xff); //080502 Change common setting for CVBS/SIF through DEMOD ADC
    _MT8297_DirectWRITE(0x83, 0x0a, 0xff);
    _MT8297_DirectWRITE(0x80, 0x03, 0xff);
    _MT8297_DirectWRITE(0x80, 0x07, 0xff);

#ifdef CC_MT5391_AUD_SUPPORT
    //reset FEIF for SIF sigal
    IO_WRITE32(IO_VIRT + 0x2B000, 0x4b4, 0x000f0000);
    IO_WRITE32(IO_VIRT + 0x2B000, 0x4a4, 0x09c87800);
    IO_WRITE32(IO_VIRT + 0x2B000, 0x110, 0x00000001);
    IO_WRITE32(IO_VIRT + 0x2B000, 0x110, 0x00000003);
    ///////////////////////////////////////
#endif

}
#endif

#ifdef CC_MT5391_AUD_SUPPORT

#elif defined (CC_MT5387)

#else // 536x
//-----------------------------------------------------------------------------
/** _AudSetBI2SPinmux
 *  Set BI2S pinmux
 *
 *  @param  void
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _AudSetBI2SPinmux(void)
{
    BOOL fgBI2SOutSet = FALSE;
    UINT32 u4RegVal = 0;
    INT32 i4Val = 1;

    // Check if BI2S output from GPIO 2~5
    fgBI2SOutSet = (BOOL)DRVCUST_OptGet(eAudioBI2SfromGpio25);

    if (fgBI2SOutSet)
    {
        GPIO_Enable(2, &i4Val);
        GPIO_Enable(3, &i4Val);
        GPIO_Enable(4, &i4Val);
        GPIO_Enable(5, &i4Val);
        u4RegVal = (CKGEN_READ32(REG_PINMUX_SEL1) & ~(0x0000ff00)) | 0x00000000;
        CKGEN_WRITE32(REG_PINMUX_SEL1, u4RegVal);
        u4RegVal = (CKGEN_READ32(REG_PINMUX_MISC) | 0x04000000);
        CKGEN_WRITE32(REG_PINMUX_MISC, u4RegVal);
    }

    // Check if BI2S output from AI2S
    fgBI2SOutSet = (BOOL)DRVCUST_OptGet(eAudioBI2SfromAI2S);

    if (fgBI2SOutSet)
    {
        GPIO_Enable(37, &i4Val);
        GPIO_Enable(38, &i4Val);
        GPIO_Enable(39, &i4Val);
        GPIO_Enable(40, &i4Val);
        u4RegVal = (CKGEN_READ32(REG_PINMUX_SEL2) & ~(0x0000000f)) | 0x0000000f;
        CKGEN_WRITE32(REG_PINMUX_SEL2, u4RegVal);
        u4RegVal = (CKGEN_READ32(REG_PINMUX_MISC) | 0x01000000);
        CKGEN_WRITE32(REG_PINMUX_MISC, u4RegVal);
    }
}
#endif

static void _AudPinmuxInit(void)
{
#if !defined(CC_MT8223) //mark by xiaoyi temp.
    UINT32 u4Reg0 = 0;
    UINT32 u4Reg1 = 0;
    UINT32 u4Reg2 = 0;
    UINT32 u4Reg3 = 0;
    UINT32 u4Reg4 = 0;
    CRIT_STATE_T rCritState;

    // Enter critical section
    rCritState = x_crit_start();

#ifdef SUPPORT_DUAL_UART1
    {
        UINT32 u4Reg0 = 0;
        u4Reg0 = (CKGEN_READ32(REG_PINMUX_SEL_0) & ~(0xf << 16)) | (0x9 << 16);
        CKGEN_WRITE32(REG_PINMUX_SEL_0, u4Reg0);
        LOG(0, "REG_PINMUX_SEL_0 = 0x%08x\n", CKGEN_READ32(REG_PINMUX_SEL_0));
    }
#endif

#ifdef CC_MT5391_AUD_SUPPORT
    // I2S Line-in selection
    //u4Reg1 = (CKGEN_READ32(REG_PINMUX_SEL_3) & ~(0x00020003)) | 0x00020001;

#ifndef CC_AUD_NO_I2S_OUT
    // Switch on MCLK/BCLK/LRCK/SDATA0
    u4Reg2 = (CKGEN_READ32(REG_PINMUX_SEL_2) & ~(0x08000000)) | 0x08000000;
    CKGEN_WRITE32(REG_PINMUX_SEL_2, u4Reg2);
#else
    u4Reg2 = (CKGEN_READ32(REG_PINMUX_SEL_2) & ~(0x08000000)) | 0x00000000;
    CKGEN_WRITE32(REG_PINMUX_SEL_2, u4Reg2);
#endif

    // Internal DAC output AL1/AR1/AL2/AR2
    //u4Reg2 = u4Reg2 & ~(0x07777000);

    // Digital I2S output MCLK/LRCK/BCLK/DATA0/DATA1/DATA2/DATA3/DATA4/DATA5

    // SPDIF select
    u4Reg3 = (CKGEN_READ32(REG_PINMUX_SEL_3) & ~(0x00020000)) | 0x00020000;

    CKGEN_WRITE32(REG_PINMUX_SEL_3, u4Reg3);

    //SIF select
    u4Reg3 = (CKGEN_READ32(REG_PINMUX_SEL_3) & ~(0x04000000)) | 0x04000000;
    CKGEN_WRITE32(REG_PINMUX_SEL_3, u4Reg3);

#elif defined (CC_MT5387)
    // MT5387
    AUD_DIGITAL_DATA_T* prDigitalDataCfg;

    //_AudSetBI2SPinmux();

    u4Reg0 = IO32ReadFldAlign(CKGEN_PMUX0, FLD_PMUX0);
    u4Reg1 = IO32ReadFldAlign(CKGEN_PMUX1, FLD_PMUX1);
    u4Reg2 = IO32ReadFldAlign(CKGEN_PMUX2, FLD_PMUX2);
#if !defined(CC_MT8223)
    u4Reg3 = IO32ReadFldAlign(CKGEN_PMUX3, FLD_PMUX3);
    u4Reg4 = IO32ReadFldAlign(CKGEN_PMUX4, FLD_PMUX4);
#endif

    // I2S Line-in selection
    //u4Reg2 = (CKGEN_READ32(REG_PINMUX_SEL2) & ~(0x00700000)) | 0x00000000;

    // SPDIF select
    //u4Reg2 = (u4Reg2 & ~(0x03000000)) | 0x00000000;
#if !defined(CC_MT8223)
    u4Reg4 = IO32ReadFldAlign(CKGEN_PMUX4, FLD_PMUX4);
    u4Reg4 = (u4Reg4 & ~(0x0000000C)) | 0x00000004;


    // Internal DAC output AL0/AR0/AL1/AR1/
    u4Reg3 = u4Reg3 & ~(0x77770000);
#endif

    // Digital I2S output MCLK/LRCK/BCLK/DATA0/DATA1/DATA2/DATA3/DATA4/DATA5
    UNUSED(DRVCUST_OptQuery(eAudioDigitalDataConfig, (UINT32 *)(void *)&prDigitalDataCfg));

    if (prDigitalDataCfg->fgAOSDATA0)
    {
        u4Reg0 = (u4Reg0 & ~(0x70000000)) | 0x00000000;
    }

    if (DRVCUST_InitGet(eFlagDDRQfp))
    {
        if (prDigitalDataCfg->fgAOSDATA1)
        {
            u4Reg3 = (u4Reg3 & ~(0x00070000)) | 0x00010000;
        }
        if (prDigitalDataCfg->fgAOSDATA2)
        {
            u4Reg3 = (u4Reg3 & ~(0x07000000)) | 0x01000000;
        }
        if (prDigitalDataCfg->fgAOSDATA3)
        {
            u4Reg3 = (u4Reg3 & ~(0x00700000)) | 0x00100000;
        }
    }
    else  //MT5388 BGA
    {

        if (prDigitalDataCfg->fgAOSDATA1)
        {
            u4Reg0 = (u4Reg0 & ~(0x00c00000)) | 0x00400000;
        }
        if (prDigitalDataCfg->fgAOSDATA2)
        {
            u4Reg0 = (u4Reg0 & ~(0x03000000)) | 0x01000000;
        }
        if (prDigitalDataCfg->fgAOSDATA3)
        {
            u4Reg0 = (u4Reg0 & ~(0x0c000000)) | 0x04000000;
        }
    }

    if (prDigitalDataCfg->fgCLK)
    {
        u4Reg0 = (u4Reg0 & ~(0x70000000)) | 0x00000000;
        u4Reg1 = (u4Reg1 & ~(0x70000000)) | 0x00000000;
    }

    vIO32WriteFldAlign(CKGEN_PMUX0, u4Reg0, FLD_PMUX0);
    vIO32WriteFldAlign(CKGEN_PMUX1, u4Reg1, FLD_PMUX1);
    vIO32WriteFldAlign(CKGEN_PMUX2, u4Reg2, FLD_PMUX2);
#if !defined(CC_MT8223)
    vIO32WriteFldAlign(CKGEN_PMUX3, u4Reg3, FLD_PMUX3);
    vIO32WriteFldAlign(CKGEN_PMUX4, u4Reg4, FLD_PMUX4);
#endif

#else
    // MT536x
    AUD_DIGITAL_DATA_T* prDigitalDataCfg;

    _AudSetBI2SPinmux();

    // I2S Line-in selection
    u4Reg2 = (CKGEN_READ32(REG_PINMUX_SEL2) & ~(0x00700000)) | 0x00000000;

    // SPDIF select
    u4Reg2 = (u4Reg2 & ~(0x03000000)) | 0x00000000;

    u4Reg3 = CKGEN_READ32(REG_PINMUX_SEL3);

    // Internal DAC output AL0/AR0/AL1/AR1/AL2/AR2
    u4Reg3 = u4Reg3 & ~(0x0007777f);

    // Digital I2S output MCLK/LRCK/BCLK/DATA0/DATA1/DATA2/DATA3/DATA4/DATA5
    UNUSED(DRVCUST_OptQuery(eAudioDigitalDataConfig, (UINT32 *)(void *)&prDigitalDataCfg));

    if (prDigitalDataCfg->fgAOSDATA0)
    {
        u4Reg3 = (u4Reg3 & ~(0x0000000c)) | 0x00000000;
    }
    if (prDigitalDataCfg->fgAOSDATA1)
    {
        u4Reg3 = (u4Reg3 & ~(0x00000030)) | 0x00000010;
    }
    if (prDigitalDataCfg->fgAOSDATA2)
    {
        u4Reg3 = (u4Reg3 & ~(0x000000c0)) | 0x00000040;
    }
    if (prDigitalDataCfg->fgAOSDATA3)
    {
        u4Reg3 = (u4Reg3 & ~(0x00000300)) | 0x00000100;
    }
    if (prDigitalDataCfg->fgAOSDATA4)
    {
        u4Reg3 = (u4Reg3 & ~(0x00000c00)) | 0x00000400;
    }
    if (prDigitalDataCfg->fgCLK)
    {
        u4Reg3 = (u4Reg3 & ~(0x00000003)) | 0x00000000;
    }

    CKGEN_WRITE32(REG_PINMUX_SEL2, u4Reg2);
    CKGEN_WRITE32(REG_PINMUX_SEL3, u4Reg3);
#endif

    // End of critical section
    x_crit_end(rCritState);
#endif //zhaoyuan for audio pll change ..TODO 
}

static void _AudGpioAvSwitch(UINT8 u1Sel)
{
    static BOOL fgInit = FALSE;
    INT32 i4RegAoutSwitch0 = 0;
    INT32 i4RegAoutSwitch1 = 0;
    INT32 i4Out = 1;

    // Temporary for romoving warning message
    UNUSED(i4RegAoutSwitch0);
    UNUSED(i4RegAoutSwitch1);
    UNUSED(i4Out);

    if (!fgInit)
    {
        // FIXME !! Temporary remove for testing
        //UNUSED(BSP_PinGpioSel(AUD_GPIO_SWITCH_0));
        //UNUSED(BSP_PinGpioSel(AUD_GPIO_SWITCH_1));
        UNUSED(GPIO_Enable(AUD_GPIO_SWITCH_0, (INT32 *)&i4Out));
        UNUSED(GPIO_Enable(AUD_GPIO_SWITCH_1, (INT32 *)&i4Out));
        fgInit = TRUE;
    }

    switch (u1Sel)
    {
        case 0:
            i4RegAoutSwitch0 = 1;
            i4RegAoutSwitch1 = 1;
            break;
        case 1:
            i4RegAoutSwitch0 = 0;
            i4RegAoutSwitch1 = 1;
            break;
        case 2:
            i4RegAoutSwitch0 = 1;
            i4RegAoutSwitch1 = 0;
            break;
        case 3:
            i4RegAoutSwitch0 = 0;
            i4RegAoutSwitch1 = 0;
            break;
        default:
            i4RegAoutSwitch0 = 0;
            i4RegAoutSwitch1 = 0;
            break;
    }

    UNUSED(GPIO_Output(AUD_GPIO_SWITCH_0, (INT32 *)&i4RegAoutSwitch0));
    UNUSED(GPIO_Output(AUD_GPIO_SWITCH_1, (INT32 *)&i4RegAoutSwitch1));
}

/*
static UINT8 _AudStrSrcToAudInId(AUD_DEC_STREAM_FROM_T eStrSrc)
{
    UINT8 u1Ret;

    switch (eStrSrc)
    {
    case AUD_STREAM_FROM_LINE_IN:
        u1Ret = 0;
        break;
    case AUD_STREAM_FROM_HDMI:
        u1Ret = 1;
        break;
    case AUD_STREAM_FROM_SPDIF:
        u1Ret = 2;
        break;
    default:
        LOG(3, "ERROR _AudStrSrcToAudInId\n");
        break;
    }

    return u1Ret;
}
*/

static void _AudInOutCfgInit(void)
{
    UINT8 u1DecId;
    AOUT_CFG_T *prAudOutFmt;
    AOUT_CFG_T *prAudOutFmt2;
    AOUT_CFG_T *prAudInFmt;

    // Query ADAC config
    UNUSED(DRVCUST_OptQuery(eAudioOutFmtTable, (UINT32 *)(void *) &prAudOutFmt));
    _arAudOutCfg[AUD_DEC_MAIN].eFormat = prAudOutFmt->eFormat;
    _arAudOutCfg[AUD_DEC_MAIN].eSampleFreq = prAudOutFmt->eSampleFreq;
    _arAudOutCfg[AUD_DEC_MAIN].eBits = prAudOutFmt->eBits;
    _arAudOutCfg[AUD_DEC_MAIN].eCycle = prAudOutFmt->eCycle;
    _arAudOutCfg[AUD_DEC_MAIN].fgDataInvert = prAudOutFmt->fgDataInvert;
    _arAudOutCfg[AUD_DEC_MAIN].fgLRInvert = prAudOutFmt->fgLRInvert;

#ifdef CC_MT5391_AUD_SUPPORT
    UNUSED(DRVCUST_OptQuery(eAudioOutFmtTable, (UINT32 *)(void *) &prAudOutFmt2));
#else
    UNUSED(DRVCUST_OptQuery(eAudioOutFmtTable2, (UINT32 *)(void *) &prAudOutFmt2));
#endif
    _arAudOutCfg[AUD_DEC_AUX].eFormat = prAudOutFmt2->eFormat;
    _arAudOutCfg[AUD_DEC_AUX].eSampleFreq = prAudOutFmt2->eSampleFreq;
    _arAudOutCfg[AUD_DEC_AUX].eBits = prAudOutFmt2->eBits;
    _arAudOutCfg[AUD_DEC_AUX].eCycle = prAudOutFmt2->eCycle;
    _arAudOutCfg[AUD_DEC_AUX].fgDataInvert = prAudOutFmt2->fgDataInvert;
    _arAudOutCfg[AUD_DEC_AUX].fgLRInvert = prAudOutFmt2->fgLRInvert;

    UNUSED(DRVCUST_OptQuery(eAudioInFmtTable, (UINT32 *)(void *) &prAudInFmt));

    // Audio output format init
#ifndef CC_MT5391_AUD_3_DECODER
    for (u1DecId = AUD_DEC_MAIN; u1DecId < AUD_DEC_NUM ; u1DecId++)
#else
    for (u1DecId = AUD_DEC_MAIN; u1DecId < 2 ; u1DecId++)
#endif
    {
        _au1MckDrvCur[u1DecId] = DRVCUST_OptGet(eAudioOutPadDrvCur);
        _au1BckDrvCur[u1DecId] = DRVCUST_OptGet(eAudioOutPadDrvCur);
        _au1LRckDrvCur[u1DecId] = DRVCUST_OptGet(eAudioOutPadDrvCur);
        _au1AOSDATADrvCur[u1DecId] = DRVCUST_OptGet(eAudioOutPadDrvCur);
    }

    // Line-in format init
    _rLineInCfg.eStrSrc       = AUD_STREAM_FROM_LINE_IN;
    _rLineInCfg.eFormat       = prAudInFmt->eFormat;
    _rLineInCfg.eBits         = prAudInFmt->eBits;
    _rLineInCfg.eCycle        = prAudInFmt->eCycle;
    _rLineInCfg.eSampleFreq   = prAudInFmt->eSampleFreq;
    _rLineInCfg.fgDataInvert  = prAudInFmt->fgDataInvert;
    _rLineInCfg.fgLRInvert    = prAudInFmt->fgLRInvert;

    // Spdif-in format init
    _rSPDIFInCfg.eStrSrc      = AUD_STREAM_FROM_SPDIF;
    _rSPDIFInCfg.eFormat      = FORMAT_RJ;
    _rSPDIFInCfg.eBits        = DAC_24_BIT;
    _rSPDIFInCfg.eCycle       = LRCK_CYC_32;
    _rSPDIFInCfg.eSampleFreq  = MCLK_256FS;
    _rSPDIFInCfg.fgDataInvert = FALSE;
    _rSPDIFInCfg.fgLRInvert   = FALSE;

    // HDMI-in format init
    _rHDMIInCfg.eStrSrc       = AUD_STREAM_FROM_HDMI;
    _rHDMIInCfg.eFormat       = FORMAT_I2S;
    _rHDMIInCfg.eBits         = DAC_24_BIT;
    _rHDMIInCfg.eCycle        = LRCK_CYC_32;
    _rHDMIInCfg.eSampleFreq   = MCLK_256FS;
    _rHDMIInCfg.fgDataInvert  = FALSE;
    _rHDMIInCfg.fgLRInvert    = FALSE;
}

static void _AudPllClkRefInternal(UINT8 u1ApllId, BOOL fgSysPll)
{
    if (u1ApllId == APLL_ID_MAIN)
    {
        if (fgSysPll) // Apll reference clock select,Master
        {
            vIO32WriteFldAlign(CKGEN_AFECFG1, 0, FLD_RG_APLL1_POSDIVSEL);
            //vIO32WriteFldAlign(CKGEN_AFECFG0, 1, FLD_RG_APLL1_PREDIV_EN);
            //vIO32WriteFldAlign(CKGEN_AFECFG0, 1, FLD_RG_APLL1_FREFSEL);
        }
        else // Apll reference ACLK (used when ACLK is from external pin),Slave
        {
            vIO32WriteFldAlign(CKGEN_AFECFG1, 1, FLD_RG_APLL1_POSDIVSEL);
            //vIO32WriteFldAlign(CKGEN_AFECFG0, 0, FLD_RG_APLL1_PREDIV_EN);
            //vIO32WriteFldAlign(CKGEN_AFECFG0, 0, FLD_RG_APLL1_FREFSEL);
        }
    }
    else
    {
        if (fgSysPll) // Apll reference clock select
        {
            vIO32WriteFldAlign(CKGEN_AFECFG15, 0, FLD_RG_APLL2_POSDIVSEL);
            //vIO32WriteFldAlign(CKGEN_AFECFG14, 1, FLD_RG_APLL2_PREDIV_EN);
            //vIO32WriteFldAlign(CKGEN_AFECFG14, 1, FLD_RG_APLL2_FREFSEL);
        }
        else // Apll reference ACLK (used when ACLK is from external pin)
        {
            vIO32WriteFldAlign(CKGEN_AFECFG15, 1, FLD_RG_APLL2_POSDIVSEL);
            //vIO32WriteFldAlign(CKGEN_AFECFG14, 0, FLD_RG_APLL2_PREDIV_EN);
            //vIO32WriteFldAlign(CKGEN_AFECFG14, 0, FLD_RG_APLL2_FREFSEL);
        }
    }
}

#if 0//SHINK_8223_CODE
static void _AudInterDacClkSel(UINT8 u1DacId, UINT8 u1ApllId)
{

#ifdef CC_MT5391_AUD_SUPPORT

    _MT8297_SemaLock();

    if (u1DacId == INTERNAL_DAC_ID_MAIN)
    {
        if (u1ApllId == APLL_ID_MAIN)
        {
            _MT8297_DirectWRITE(REG_BANK, 0x00, REG_BANK_MSK);
            _MT8297_DirectWRITE(REG_DAC_18, APLL_DAC0_CLK_SEL_APLL0, APLL_DAC0_CLK_SEL_MSK);
            _MT8297_DirectWRITE(REG_BANK, 0x01, REG_BANK_MSK);
            _MT8297_DirectWRITE(REG_ADAC_2C, CLKDIV24SEL0_SEL_APLL0, CLKDIV24SEL0_MSK);
        }
        else
        {
            _MT8297_DirectWRITE(REG_BANK, 0x00, REG_BANK_MSK);
            _MT8297_DirectWRITE(REG_DAC_18, APLL_DAC0_CLK_SEL_APLL1, APLL_DAC0_CLK_SEL_MSK);
            _MT8297_DirectWRITE(REG_BANK, 0x01, REG_BANK_MSK);
            _MT8297_DirectWRITE(REG_ADAC_2C, CLKDIV24SEL0_SEL_APLL1, CLKDIV24SEL0_MSK);
        }
    }
    else if (u1DacId == INTERNAL_DAC_ID_AUX)
    {
        if (u1ApllId == APLL_ID_MAIN)
        {
            _MT8297_DirectWRITE(REG_BANK, 0x00, REG_BANK_MSK);
            _MT8297_DirectWRITE(REG_DAC_18, APLL_DAC1_CLK_SEL_APLL0, APLL_DAC1_CLK_SEL_MSK);
            _MT8297_DirectWRITE(REG_BANK, 0x01, REG_BANK_MSK);
            _MT8297_DirectWRITE(REG_ADAC_2C, CLKDIV24SEL1_SEL_APLL0, CLKDIV24SEL1_MSK);
        }
        else
        {
            _MT8297_DirectWRITE(REG_BANK, 0x00, REG_BANK_MSK);
            _MT8297_DirectWRITE(REG_DAC_18, APLL_DAC1_CLK_SEL_APLL1, APLL_DAC1_CLK_SEL_MSK);
            _MT8297_DirectWRITE(REG_BANK, 0x01, REG_BANK_MSK);
            _MT8297_DirectWRITE(REG_ADAC_2C, CLKDIV24SEL1_SEL_APLL1, CLKDIV24SEL1_MSK);
        }
    }
    else
    {
        if (u1ApllId == APLL_ID_MAIN)
        {
            _MT8297_DirectWRITE(REG_BANK, 0x00, REG_BANK_MSK);
            _MT8297_DirectWRITE(REG_DAC_18, APLL_DAC2_CLK_SEL_APLL0, APLL_DAC2_CLK_SEL_MSK);
            _MT8297_DirectWRITE(REG_BANK, 0x01, REG_BANK_MSK);
            _MT8297_DirectWRITE(REG_ADAC_2C, CLKDIV24SEL2_SEL_APLL0, CLKDIV24SEL2_MSK);
        }
        else
        {
            _MT8297_DirectWRITE(REG_BANK, 0x00, REG_BANK_MSK);
            _MT8297_DirectWRITE(REG_DAC_18, APLL_DAC2_CLK_SEL_APLL1, APLL_DAC2_CLK_SEL_MSK);
            _MT8297_DirectWRITE(REG_BANK, 0x01, REG_BANK_MSK);
            _MT8297_DirectWRITE(REG_ADAC_2C, CLKDIV24SEL2_SEL_APLL1, CLKDIV24SEL2_MSK);
        }
    }

    _MT8297_SemaUnLock();
#elif defined (CC_MT5387)
    //5387 no need set PWM clock

#else
    if (u1DacId == INTERNAL_DAC_ID_MAIN)
    {
        if (u1ApllId == APLL_ID_MAIN)
        {
            AUD_CLR_BIT(REG_INT_DACCTL6, APLL_CLK_SEL_0);
            //AUD_SET_BIT(REG_INT_DACCTL6, DECODER_SEL_0); // gallen. Laihui: move to codec_mt536xinternal.c
            //AUD_CLR_BIT(REG_INT_DACCTL6, DECODER_SEL_0);
            AUD_CLR_BIT(REG_INT_DACCTL10, DAC_CLK_SEL_0);
        }
        else
        {
            AUD_SET_BIT(REG_INT_DACCTL6, APLL_CLK_SEL_0);
            //AUD_SET_BIT(REG_INT_DACCTL6, DECODER_SEL_0);//Laihui: move to codec_mt536xinternal.c
            AUD_SET_BIT(REG_INT_DACCTL10, DAC_CLK_SEL_0);
        }
    }
    else if (u1DacId == INTERNAL_DAC_ID_AUX)
    {
        if (u1ApllId == APLL_ID_MAIN)
        {
            AUD_CLR_BIT(REG_INT_DACCTL6, APLL_CLK_SEL_1);
            //AUD_CLR_BIT(REG_INT_DACCTL6, DECODER_SEL_1);//Laihui: move to codec_mt536xinternal.c
#ifdef CC_MT5360
#ifdef CC_MT5360B
            AUD_CLR_BIT(REG_INT_DACCTL10, DAC_CLK_SEL_1);
#else
            AUD_SET_BIT(REG_INT_DACCTL10, DAC_CLK_SEL_1);
#endif
#else
            AUD_CLR_BIT(REG_INT_DACCTL10, DAC_CLK_SEL_1);
#endif
        }
        else
        {
            AUD_SET_BIT(REG_INT_DACCTL6, APLL_CLK_SEL_1);
            //AUD_SET_BIT(REG_INT_DACCTL6, DECODER_SEL_1);
            //AUD_CLR_BIT(REG_INT_DACCTL6, DECODER_SEL_1); // gallen.Laihui: move to codec_mt536xinternal.c
#ifdef CC_MT5360
#ifdef CC_MT5360B
            AUD_SET_BIT(REG_INT_DACCTL10, DAC_CLK_SEL_1);
#else
            AUD_CLR_BIT(REG_INT_DACCTL10, DAC_CLK_SEL_1);
#endif
#else
            AUD_SET_BIT(REG_INT_DACCTL10, DAC_CLK_SEL_1);
#endif
        }
    }
    else
    {
        if (u1ApllId == APLL_ID_MAIN)
        {
            AUD_CLR_BIT(REG_INT_DACCTL6, APLL_CLK_SEL_2);
            //AUD_CLR_BIT(REG_INT_DACCTL6, DECODER_SEL_2);//Laihui: move to codec_mt536xinternal.c

#ifdef CC_MT5360
#ifdef CC_MT5360B
            AUD_CLR_BIT(REG_INT_DACCTL10, DAC_CLK_SEL_2);
#else
            AUD_SET_BIT(REG_INT_DACCTL10, DAC_CLK_SEL_2);
#endif
#else
            AUD_CLR_BIT(REG_INT_DACCTL10, DAC_CLK_SEL_2);
#endif
        }
        else
        {
            AUD_SET_BIT(REG_INT_DACCTL6, APLL_CLK_SEL_2);
            //AUD_SET_BIT(REG_INT_DACCTL6, DECODER_SEL_2);
            //AUD_CLR_BIT(REG_INT_DACCTL6, DECODER_SEL_2); // gallen.Laihui: move to codec_mt536xinternal.c
#ifdef CC_MT5360
#ifdef CC_MT5360B
            AUD_SET_BIT(REG_INT_DACCTL10, DAC_CLK_SEL_2);
#else
            AUD_CLR_BIT(REG_INT_DACCTL10, DAC_CLK_SEL_2);
#endif
#else
            AUD_SET_BIT(REG_INT_DACCTL10, DAC_CLK_SEL_2);
#endif
        }
    }
#endif
}
#endif

#if 1//SHINK_8223_CODE
// This function is used to select clock source
// a. apll  b. hdmi  c. spdif (external pin)
// MT8223_NOTES_Zhaoyuan : this funtion becomse simple since MT8223
// for 8223 this funciton only need to switch lrck/bck clk now..no more  config for CKGEN,PLL,INTERNAL DAC CLK,.
// and we make decoder always ref apll1.
static void _AudClkSrcSel(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eFmt)
{
    static AUD_DEC_STREAM_FROM_T ePreviouStream = AUD_STREAM_FROM_OTHERS;
    UNUSED(ePreviouStream);

    AUD_DEC_ID_VALIDATE(u1DecId);


    // MT8223 Select decoder always refer to APLL0, and we allready config CKGEN&PLL (init) to make aud_clk be 12.28M
    AUD_CLR_BIT(REG_ACK_CFG, CLK_SEL_APLL1);
    AUD_CLR_BIT(REG_ACK_CFG, DEC2_CLK_SEL_APLL1);

    AUD_WRITE32(REG_ACK_CFG, (AUD_READ32(REG_ACK_CFG) & (~BCK_DIV_MASK)) | BCK_DIV_4);
    AUD_WRITE32(REG_ACK_CFG, (AUD_READ32(REG_ACK_CFG) & (~DEC2_BCK_DIV_MASK)) | DEC2_BCK_DIV_4);

    if (_fgDualDecMode)
    {
        // Apll setting ..
        //MT8223_NOTES_Zhaoyuan: TODO,, _AudPllClkRefInternal need to remove or not
        //_AudPllClkRefInternal(APLL_ID_AUX, TRUE);
        // Master mode
        AUD_CLR_BIT(REG_ACK_CFG, LIN_MS);

    }
    else // Individual decoder mode
    {
        // Select SPDIF/HDMI/LINE-IN and clock source
        if (eStreamFrom == AUD_STREAM_FROM_SPDIF)
        {
            // Slave mode
            AUD_SET_BIT(REG_ACK_CFG, LIN_MS);
            // Line in from External MCLK
            AUD_WRITE32(REG_LIN_CFG0, (AUD_READ32(REG_LIN_CFG0) & (~LIN_CLK_SEL_MASK)) | AOUT_HDMI);
            // Apll setting ..
            //_AudPllClkRefInternal(APLL_ID_MAIN, FALSE);
        }
        else if (eStreamFrom == AUD_STREAM_FROM_HDMI)
        {
            // Slave mode
            AUD_SET_BIT(REG_ACK_CFG, LIN_MS);
            // Line in from HDMI/SPDIF
            AUD_WRITE32(REG_LIN_CFG0, (AUD_READ32(REG_LIN_CFG0) & (~LIN_CLK_SEL_MASK)) | AOUT_DEC0);

            // Apll setting ..
            //_AudPllClkRefInternal(APLL_ID_MAIN, FALSE);
        }
        else if (eStreamFrom == AUD_STREAM_FROM_LINE_IN)
        {
            // Apll setting ..
            //_AudPllClkRefInternal(APLL_ID_MAIN, TRUE);

            // Master mode
            AUD_CLR_BIT(REG_ACK_CFG, LIN_MS);
            // Line in feeding to MAIN/AUX decoder
#if 0 //FPGA_ONLY
            AUD_WRITE32(REG_LIN_CFG0, (AUD_READ32(REG_LIN_CFG0) & (~LIN_CLK_SEL_MASK)) | AOUT_DEC0);
#else
#ifndef CC_MT5391_AUD_3_DECODER
            AUD_WRITE32(REG_LIN_CFG0, (AUD_READ32(REG_LIN_CFG0) & (~LIN_CLK_SEL_MASK)) |
                        ((u1DecId == AUD_DEC_MAIN) ? AOUT_DEC0 : AOUT_DEC2));
#else
            // dec1/dec3 both share the same clock and AFIFO1
            AUD_WRITE32(REG_LIN_CFG0, (AUD_READ32(REG_LIN_CFG0) & (~LIN_CLK_SEL_MASK)) |
                        ((u1DecId == AUD_DEC_AUX) ? AOUT_DEC2 : AOUT_DEC0));
#endif
#endif

        }
        else if ((eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) ||
                 (eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) ||
                 (eStreamFrom == AUD_STREAM_FROM_MEMORY) ||
                 (eStreamFrom == AUD_STREAM_FROM_BUF_AGT) ||
                 (eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) ||
                 (eStreamFrom == AUD_STREAM_FROM_FEEDER))
        {
            // Master mode
            AUD_CLR_BIT(REG_ACK_CFG, LIN_MS);

            /*
            // Select decoder refer to APLL0, APLL1
            if (u1DecId == AUD_DEC_MAIN)
            {
                // Apll setting ..
                _AudPllClkRefInternal(APLL_ID_MAIN, TRUE);

            }
            else
            {
                // Apll setting ..
                _AudPllClkRefInternal(APLL_ID_AUX, TRUE);

            }
            */

            if (((eStreamFrom == AUD_STREAM_FROM_MEMORY) ||
                    (eStreamFrom == AUD_STREAM_FROM_BUF_AGT) ||
                    (eStreamFrom == AUD_STREAM_FROM_FEEDER)) &&
                    (eFmt == AUD_FMT_PCM)
               )
            {
                AUD_SetSampleFreq(u1DecId, _aeSampleRate[u1DecId]);
            }
        }
    }

    ePreviouStream = eStreamFrom;

}
#else
// This function is used to select clock source
// a. apll  b. hdmi  c. spdif (external pin)
// MT8223_NOTES_Zhaoyuan : this funtion becomse simple since MT8223
// for 8223 this funciton only need to switch lrck/bck clk now..no more  config for CKGEN,PLL,INTERNAL DAC CLK,.
// and we make decoder always ref apll1.
static void _AudClkSrcSel(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eFmt)
{
    static AUD_DEC_STREAM_FROM_T ePreviouStream = AUD_STREAM_FROM_OTHERS;

    AUD_DEC_ID_VALIDATE(u1DecId);

#if 0 ////MT8223_NOTES_Zhaoyuan no need to config CKGEN&PLL after init, so don't be afraid of popo nosie 

#ifdef CC_MT8223
    //vAUD_RequestCKGEN();
#endif


    if (AUD_GetTargetCountry() == COUNTRY_EU)
    {
        //#ifdef CC_AUD_DVBT_SUPPORT
        ADAC_ClkChangeCodecMute(TRUE);   // to avoid internal DAC change APLL "po" noise
        //#endif
    }

    if ((eStreamFrom == AUD_STREAM_FROM_HDMI) || (ePreviouStream == AUD_STREAM_FROM_HDMI) ||
            (eStreamFrom == AUD_STREAM_FROM_SPDIF) || (ePreviouStream == AUD_STREAM_FROM_SPDIF))
    {
        if (AUD_GetTargetCountry() != COUNTRY_EU)
        {
            //#ifndef CC_AUD_DVBT_SUPPORT
            ADAC_ClkChangeCodecMute(TRUE);
            //#endif
        }
#ifndef CC_MT8223
        bHDMIClockDetect();
#endif
    }

#if 1 //def CC_AUD_FIX_PASS

#else
    // fix a bug in MT538x (phase difference between L/R channels)
    // 1. Reset ADC module
    // 2. Motior LRCK   ( detect  transition from "1" to "0",
    //    note that  if we set LRCK pin inverse on , we should detect  transition from "0" to "1")
    // 3. If detect LRCK transition , release ADC reset immedialtely)
    {
        UINT16 u2TimeOutCount = 0;
        BOOL fgTemp = TRUE;

        if (eStreamFrom == AUD_STREAM_FROM_LINE_IN)
        {
            AUD_SET_BIT(REG_SDADC0_DRV, RST_FILTER);
            while (fgTemp)
            {
                u2TimeOutCount = u2TimeOutCount + 1;
                if ((AUD_READ32(REG_EXTADC_CFG) & SPLIN_LR_INVT) == 0)
                {
#ifdef CC_MT5387
                    if ((IO32ReadFldAlign(CKGEN_GPIOIN1, FLD_GPIO_IN1) & (0x00010000)) == 0x00010000 )
#else
                        if ((CKGEN_READ32(REG_GPIO_IN1) & (0x00010000)) == 0x00010000)
#endif
                        {
#ifdef CC_MT5387
                            while ((IO32ReadFldAlign(CKGEN_GPIOIN1, FLD_GPIO_IN1) & (0x00010000)) == 0x00000000 )
#else
                                while ((CKGEN_READ32(REG_GPIO_IN1) & (0x00010000)) == 0x00000000)
#endif
                                {
                                    AUD_CLR_BIT(REG_SDADC0_DRV, RST_FILTER);
                                    LOG(5, "TIMEOUT_COUNT = %x\n", u2TimeOutCount);
                                    fgTemp = FALSE;
                                    break;
                                }
                        }
                }
                else
                {
#ifdef CC_MT5387
                    if ((IO32ReadFldAlign(CKGEN_GPIOIN1, FLD_GPIO_IN1) & (0x00010000)) == 0x00000000 )
#else
                        if ((CKGEN_READ32(REG_GPIO_IN1) & (0x00010000)) == 0x00000000)
#endif
                        {
#ifdef CC_MT5387
                            while ((IO32ReadFldAlign(CKGEN_GPIOIN1, FLD_GPIO_IN1) & (0x00010000)) == 0x00010000 )
#else
                                while ((CKGEN_READ32(REG_GPIO_IN1) & (0x00010000)) == 0x00010000)
#endif
                                {
                                    AUD_CLR_BIT(REG_SDADC0_DRV, RST_FILTER);
                                    LOG(5, "TIMEOUT_COUNT = %x\n", u2TimeOutCount);
                                    fgTemp = FALSE;
                                    break;
                                }
                        }
                }
            }
        }
    }   // end of bug fix for MT538x
#endif

#endif

    // MT8223 Select decoder always refer to APLL0, and we allready config CKGEN&PLL (init) to make aud_clk be 12.28M
    AUD_CLR_BIT(REG_ACK_CFG, CLK_SEL_APLL1);
    AUD_CLR_BIT(REG_ACK_CFG, DEC2_CLK_SEL_APLL1);

    if (_fgDualDecMode)
    {   // AUX decoder with ATV 32K, MAIN decoder with pcm 32K, all refer to apll2
#ifdef CC_AUD_5387_APLL_FIX
        // Apll setting ..
        //MT8223_NOTES_Zhaoyuan: TODO _AudPllClkRefInternal need to be review..maybe can be removed..
        _AudPllClkRefInternal(APLL_ID_MAIN, TRUE);
        //vIO32WriteFldAlign(CKGEN_AUD_CKCFG, 1, FLD_AUD_CK_SEL);  //MT8223 MCLK From APLL1 K1&K2
        // Master mode
        AUD_CLR_BIT(REG_ACK_CFG, LIN_MS);
        // Select decoder refer to APLL1
        //AUD_SET_BIT(REG_ACK_CFG, CLK_SEL_APLL0);
        //AUD_SET_BIT(REG_ACK_CFG, DEC2_CLK_SEL_APLL0);
        //MT8223 mark all decoder clk config
        //AUD_CLR_BIT(REG_ACK_CFG, CLK_SEL_APLL1);
        //AUD_CLR_BIT(REG_ACK_CFG, DEC2_CLK_SEL_APLL1);
#else
        // Apll setting ..
        _AudPllClkRefInternal(APLL_ID_AUX, TRUE);
#ifdef CC_MT5391_AUD_SUPPORT
        CKGEN_WRITE32(REG_AUD_CKCFG_AUX, (CKGEN_READ32(REG_AUD_CKCFG_AUX) & (~AUD_CLK_SEL_MASK) ) | AUD_CLK_SEL_LDMCLK1);
#elif defined (CC_MT5387)
        //vIO32WriteFldAlign(CKGEN_AUD2_CKCFG, 1, FLD_AUD2_CK_SEL);  //MT8223 MCLK From APLL1 K1&K2
#else
        CKGEN_WRITE32(REG_AUD_CKCFG_AUX, (CKGEN_READ32(REG_AUD_CKCFG_AUX) & (~AUD_CLK_SEL_MASK) ) | AUD_CLK_SEL_APLL);
#endif //CC_MT5391_AUD_SUPPORT
        // Master mode
        AUD_CLR_BIT(REG_ACK_CFG, LIN_MS);
        // Select decoder refer to APLL0
        //MT8223 mark all decoder clk config
        //AUD_CLR_BIT(REG_ACK_CFG, CLK_SEL_APLL1);
        //AUD_CLR_BIT(REG_ACK_CFG, DEC2_CLK_SEL_APLL1);
#ifdef CC_MT5391_AUD_SUPPORT
        //Change MCLK selection: APLL1
        _MT8297_InDirectWRITE(REG_MCLK0_CKCFG0, MCLK_SEL_APLL1, REG_CLK_SRC_SEL_MSK);
#endif //CC_MT5391_AUD_SUPPORT
        // Select internal dac refer to APLL1
#if 0//SHINK_8223_CODE  //8223 no need to set internal pwm dac clk   
        _AudInterDacClkSel(INTERNAL_DAC_ID_MAIN,    APLL_ID_AUX);
        _AudInterDacClkSel(INTERNAL_DAC_ID_AUX,     APLL_ID_AUX);
        _AudInterDacClkSel(INTERNAL_DAC_ID_THIRD,   APLL_ID_AUX);
#endif
#endif  //CC_AUD_5387_APLL_FIX
    }
    else // Individual decoder mode
    {

        // Select SPDIF/HDMI/LINE-IN and clock source
        if (eStreamFrom == AUD_STREAM_FROM_SPDIF)
        {
            //Notice: MT5391 send SPDIF clock by itself!
#ifdef CC_MT5391_AUD_SUPPORT
            CKGEN_WRITE32(REG_AUD_CKCFG_MAIN, (CKGEN_READ32(REG_AUD_CKCFG_MAIN) & (~AUD_CLK_SEL_MASK)) | AUD_CLK_SEL_LDMCLK0);
#elif defined (CC_MT5387)
            //vIO32WriteFldAlign(CKGEN_AUD_CKCFG, 4, FLD_AUD_CK_SEL);  //MT8223 MCLK From APLL1 K1&K2
#else
            CKGEN_WRITE32(REG_AUD_CKCFG_MAIN, (CKGEN_READ32(REG_AUD_CKCFG_MAIN) & (~AUD_CLK_SEL_MASK)) | AUD_CLK_SEL_SPDIF);
#endif
            // Slave mode
            AUD_SET_BIT(REG_ACK_CFG, LIN_MS);
            // Line in from HDMI/SPDIF
            AUD_WRITE32(REG_LIN_CFG0, (AUD_READ32(REG_LIN_CFG0) & (~LIN_CLK_SEL_MASK)) | AOUT_HDMI);
            // Select decoder refer to APLL0
            //MT8223 mark all decoder clk config
            //AUD_CLR_BIT(REG_ACK_CFG, (u1DecId == AUD_DEC_MAIN) ? CLK_SEL_APLL1 : DEC2_CLK_SEL_APLL1);
#ifdef CC_MT5391_AUD_SUPPORT
            //Change MCLK selection: APLL0
            _MT8297_InDirectWRITE(REG_MCLK0_CKCFG0, MCLK_SEL_APLL0, REG_CLK_SRC_SEL_MSK);
            //_MT8297_InDirectWRITE(REG_MCLK1_CKCFG0, MCLK_SEL_APLL0, REG_CLK_SRC_SEL_MSK);
#endif

#if 0 //SHINK_8223_CODE
            // Select internal dac refer to APLL1
            if ((u1DecId == AUD_DEC_MAIN) && _IsInterDacRefMainDec(INTERNAL_DAC_ID_MAIN))
            {
                _AudInterDacClkSel(INTERNAL_DAC_ID_MAIN, APLL_ID_MAIN);
            }
            if ((u1DecId == AUD_DEC_MAIN) && _IsInterDacRefMainDec(INTERNAL_DAC_ID_AUX))
            {
                _AudInterDacClkSel(INTERNAL_DAC_ID_AUX, APLL_ID_MAIN);
            }
            if ((u1DecId == AUD_DEC_MAIN) && _IsInterDacRefMainDec(INTERNAL_DAC_ID_THIRD))
            {
                _AudInterDacClkSel(INTERNAL_DAC_ID_THIRD, APLL_ID_MAIN);
            }
#endif

            // Apll setting ..
            _AudPllClkRefInternal(APLL_ID_MAIN, FALSE);
        }
        else if (eStreamFrom == AUD_STREAM_FROM_HDMI)
        {
#ifdef CC_MT5387
            //vIO32WriteFldAlign(CKGEN_AUD_CKCFG, 3, FLD_AUD_CK_SEL);  //MT8223 MCLK From APLL1 K1&K2
#else
            CKGEN_WRITE32(REG_AUD_CKCFG_MAIN, (CKGEN_READ32(REG_AUD_CKCFG_MAIN) & (~AUD_CLK_SEL_MASK)) | AUD_CLK_SEL_HDMI);
#endif
            // Slave mode
            AUD_SET_BIT(REG_ACK_CFG, LIN_MS);
            // Line in from HDMI/SPDIF
            AUD_WRITE32(REG_LIN_CFG0, (AUD_READ32(REG_LIN_CFG0) & (~LIN_CLK_SEL_MASK)) | AOUT_HDMI);
            // Select decoder refer to APLL0
            //MT8223 mark all decoder clk config
            //AUD_CLR_BIT(REG_ACK_CFG, (u1DecId == AUD_DEC_MAIN) ? CLK_SEL_APLL1 : DEC2_CLK_SEL_APLL1);
#ifdef CC_MT5391_AUD_SUPPORT
            //Change MCLK selection: APLL0
            _MT8297_InDirectWRITE(REG_MCLK0_CKCFG0, MCLK_SEL_APLL0, REG_CLK_SRC_SEL_MSK);
            //_MT8297_InDirectWRITE(REG_MCLK1_CKCFG0, MCLK_SEL_APLL0, REG_CLK_SRC_SEL_MSK);
#endif
#if 0  //SHINK_8223_CODE
            // Select internal dac refer to APLL0
            if ((u1DecId == AUD_DEC_MAIN) && _IsInterDacRefMainDec(INTERNAL_DAC_ID_MAIN))
            {
                _AudInterDacClkSel(INTERNAL_DAC_ID_MAIN, APLL_ID_MAIN);
            }
            if ((u1DecId == AUD_DEC_MAIN) && _IsInterDacRefMainDec(INTERNAL_DAC_ID_AUX))
            {
                _AudInterDacClkSel(INTERNAL_DAC_ID_AUX, APLL_ID_MAIN);
            }
            if ((u1DecId == AUD_DEC_MAIN) && _IsInterDacRefMainDec(INTERNAL_DAC_ID_THIRD))
            {
                _AudInterDacClkSel(INTERNAL_DAC_ID_THIRD, APLL_ID_MAIN);
            }
#endif
            // Apll setting ..
            _AudPllClkRefInternal(APLL_ID_MAIN, FALSE);
        }
        else if (eStreamFrom == AUD_STREAM_FROM_LINE_IN)
        {
            // Apll setting ..
            _AudPllClkRefInternal(APLL_ID_MAIN, TRUE);

#ifdef CC_MT5391_AUD_SUPPORT
            CKGEN_WRITE32(REG_AUD_CKCFG_MAIN, (CKGEN_READ32(REG_AUD_CKCFG_MAIN) & (~AUD_CLK_SEL_MASK)) | AUD_CLK_SEL_LDMCLK0);
#elif defined (CC_MT5387)
            //vIO32WriteFldAlign(CKGEN_AUD_CKCFG, 1, FLD_AUD_CK_SEL);  //MT8223 MCLK From APLL1 K1&K2
#else
            CKGEN_WRITE32(REG_AUD_CKCFG_MAIN, (CKGEN_READ32(REG_AUD_CKCFG_MAIN) & (~AUD_CLK_SEL_MASK)) | AUD_CLK_SEL_APLL);
#endif
            // Master mode
            AUD_CLR_BIT(REG_ACK_CFG, LIN_MS);
            // Line in feeding to MAIN/AUX decoder
#if 0 //FPGA_ONLY
            AUD_WRITE32(REG_LIN_CFG0, (AUD_READ32(REG_LIN_CFG0) & (~LIN_CLK_SEL_MASK)) | AOUT_DEC0);
#else
#ifndef CC_MT5391_AUD_3_DECODER
            AUD_WRITE32(REG_LIN_CFG0, (AUD_READ32(REG_LIN_CFG0) & (~LIN_CLK_SEL_MASK)) |
                        ((u1DecId == AUD_DEC_MAIN) ? AOUT_DEC0 : AOUT_DEC2));
#else
            // dec1/dec3 both share the same clock and AFIFO1
            AUD_WRITE32(REG_LIN_CFG0, (AUD_READ32(REG_LIN_CFG0) & (~LIN_CLK_SEL_MASK)) |
                        ((u1DecId == AUD_DEC_AUX) ? AOUT_DEC2 : AOUT_DEC0));
#endif
#endif
            // Select decoder refer to APLL0 (44.1K)
            //MT8223 mark all decoder clk config
            //AUD_CLR_BIT(REG_ACK_CFG, (u1DecId == AUD_DEC_MAIN) ? CLK_SEL_APLL1 : DEC2_CLK_SEL_APLL1);
#ifdef CC_MT5391_AUD_SUPPORT
            //Change MCLK selection: APLL0
            _MT8297_InDirectWRITE(REG_MCLK0_CKCFG0, MCLK_SEL_APLL0, REG_CLK_SRC_SEL_MSK);
            //_MT8297_InDirectWRITE(REG_MCLK1_CKCFG0, MCLK_SEL_APLL0, REG_CLK_SRC_SEL_MSK);
#endif
#if 0  //SHINK_8223_CODE
            // Select internal dac refer to APLL0
            if ((u1DecId == AUD_DEC_MAIN) && _IsInterDacRefMainDec(INTERNAL_DAC_ID_MAIN))
            {
                _AudInterDacClkSel(INTERNAL_DAC_ID_MAIN, APLL_ID_MAIN);
            }
            if ((u1DecId == AUD_DEC_MAIN) && _IsInterDacRefMainDec(INTERNAL_DAC_ID_AUX))
            {
                _AudInterDacClkSel(INTERNAL_DAC_ID_AUX, APLL_ID_MAIN);
            }
            if ((u1DecId == AUD_DEC_MAIN) && _IsInterDacRefMainDec(INTERNAL_DAC_ID_THIRD))
            {
                _AudInterDacClkSel(INTERNAL_DAC_ID_THIRD, APLL_ID_MAIN);
            }
#endif

        }
        else if ((eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) ||
                 (eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) ||
                 (eStreamFrom == AUD_STREAM_FROM_MEMORY) ||
                 (eStreamFrom == AUD_STREAM_FROM_BUF_AGT) ||
                 (eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) ||
                 (eStreamFrom == AUD_STREAM_FROM_FEEDER))
        {
            // Master mode
            AUD_CLR_BIT(REG_ACK_CFG, LIN_MS);

            // Select decoder refer to APLL0, APLL1
            if (u1DecId == AUD_DEC_MAIN)
            {
                // Apll setting ..
                _AudPllClkRefInternal(APLL_ID_MAIN, TRUE);

#ifdef CC_MT5391_AUD_SUPPORT
                CKGEN_WRITE32(REG_AUD_CKCFG_MAIN, (CKGEN_READ32(REG_AUD_CKCFG_MAIN) & (~AUD_CLK_SEL_MASK)) | AUD_CLK_SEL_LDMCLK0);
                //Change MCLK selection: APLL0
                _MT8297_InDirectWRITE(REG_MCLK0_CKCFG0, MCLK_SEL_APLL0, REG_CLK_SRC_SEL_MSK);
                //_MT8297_InDirectWRITE(REG_MCLK1_CKCFG0, MCLK_SEL_APLL0, REG_CLK_SRC_SEL_MSK);
#elif defined (CC_MT5387)
                //vIO32WriteFldAlign(CKGEN_AUD_CKCFG, 1, FLD_AUD_CK_SEL);  //MT8223 MCLK From APLL1 K1&K2
#else
                CKGEN_WRITE32(REG_AUD_CKCFG_MAIN, (CKGEN_READ32(REG_AUD_CKCFG_MAIN) & (~AUD_CLK_SEL_MASK)) | AUD_CLK_SEL_APLL);
#endif
                // Select decoder refer to APLL0
                //MT8223 mark all decoder clk config
                //AUD_CLR_BIT(REG_ACK_CFG, CLK_SEL_APLL1);
            }
            else
            {
                // Apll setting ..
                _AudPllClkRefInternal(APLL_ID_AUX, TRUE);

#ifdef CC_MT5391_AUD_SUPPORT
                CKGEN_WRITE32(REG_AUD_CKCFG_AUX, (CKGEN_READ32(REG_AUD_CKCFG_AUX) & (~AUD_CLK_SEL_MASK)) | AUD_CLK_SEL_LDMCLK1);
                //Change MCLK1 selection: APLL1
                _MT8297_InDirectWRITE(REG_MCLK1_CKCFG0, MCLK_SEL_APLL1, REG_CLK_SRC_SEL_MSK);
#elif defined (CC_MT5387)
                //vIO32WriteFldAlign(CKGEN_AUD2_CKCFG, 1, FLD_AUD2_CK_SEL);  //MT8223 MCLK From APLL1 K1&K2
#else
                CKGEN_WRITE32(REG_AUD_CKCFG_AUX, (CKGEN_READ32(REG_AUD_CKCFG_AUX) & (~AUD_CLK_SEL_MASK)) | AUD_CLK_SEL_APLL);
#endif
                // Select decoder refer to APLL1
                //MT8223 mark all decoder clk config
                //AUD_SET_BIT(REG_ACK_CFG, DEC2_CLK_SEL_APLL1);
            }

#if 0 //SHINK_8223_CODE
            // Select internal dac reference clock
#ifndef CC_MT5391_AUD_3_DECODER
            if (eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER || eStreamFrom == AUD_STREAM_FROM_MEMORY || eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
#else
                if (eStreamFrom == AUD_STREAM_FROM_MEMORY || eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
#endif
                {
                    _AudInterDacClkSel(INTERNAL_DAC_ID_MAIN,  APLL_ID_MAIN);
                    _AudInterDacClkSel(INTERNAL_DAC_ID_AUX,   APLL_ID_MAIN);
                    _AudInterDacClkSel(INTERNAL_DAC_ID_THIRD, APLL_ID_MAIN);
                }
                else
                {
                    //IDTV case: check internal dac reference main decoder.
                    if ((u1DecId == AUD_DEC_MAIN) && _IsInterDacRefMainDec(INTERNAL_DAC_ID_MAIN))
                    {
                        _AudInterDacClkSel(INTERNAL_DAC_ID_MAIN, APLL_ID_MAIN);
                    }

                    if ((u1DecId == AUD_DEC_MAIN) && _IsInterDacRefMainDec(INTERNAL_DAC_ID_AUX))
                    {
                        _AudInterDacClkSel(INTERNAL_DAC_ID_AUX, APLL_ID_MAIN);
                    }

                    if ((u1DecId == AUD_DEC_MAIN) && _IsInterDacRefMainDec(INTERNAL_DAC_ID_THIRD))
                    {
                        _AudInterDacClkSel(INTERNAL_DAC_ID_THIRD, APLL_ID_MAIN);
                    }

                    //DVBT case: check internal dac reference main or aux decoder.
                    if ((u1DecId == AUD_DEC_AUX) && _IsInterDacRefAuxDec(INTERNAL_DAC_ID_MAIN))
                    {
                        _AudInterDacClkSel(INTERNAL_DAC_ID_MAIN, APLL_ID_AUX);
                    }

                    if ((u1DecId == AUD_DEC_AUX) && _IsInterDacRefAuxDec(INTERNAL_DAC_ID_AUX))
                    {
                        _AudInterDacClkSel(INTERNAL_DAC_ID_AUX, APLL_ID_AUX);
                    }

                    if ((u1DecId == AUD_DEC_AUX) && _IsInterDacRefAuxDec(INTERNAL_DAC_ID_THIRD))
                    {
                        _AudInterDacClkSel(INTERNAL_DAC_ID_THIRD, APLL_ID_AUX);
                    }
                }
#endif

            if (((eStreamFrom == AUD_STREAM_FROM_MEMORY) ||
                    (eStreamFrom == AUD_STREAM_FROM_BUF_AGT) ||
                    (eStreamFrom == AUD_STREAM_FROM_FEEDER)) &&
                    (eFmt == AUD_FMT_PCM)
               )
            {
                AUD_SetSampleFreq(u1DecId, _aeSampleRate[u1DecId]);
            }
        }
    }

#if 0//SHINK_8223_CODE
    if ((eStreamFrom == AUD_STREAM_FROM_HDMI) || (ePreviouStream == AUD_STREAM_FROM_HDMI) ||
            (eStreamFrom == AUD_STREAM_FROM_SPDIF) || (ePreviouStream == AUD_STREAM_FROM_SPDIF))
    {
#ifndef CC_MT8223
        bHDMIClockDetect();
#endif

        if (AUD_GetTargetCountry() != COUNTRY_EU)
        {
            //#ifndef CC_AUD_DVBT_SUPPORT
            ADAC_ClkChangeCodecMute(FALSE);
            //#endif
        }
    }

    if (AUD_GetTargetCountry() == COUNTRY_EU)
    {
        //#ifdef CC_AUD_DVBT_SUPPORT
        ADAC_ClkChangeCodecMute(FALSE);   // to avoid internal DAC change APLL "po" noise
        //#endif
    }
#ifdef CC_MT8223
    vAUD_ReturnCKGEN();
#endif
#endif

    ePreviouStream = eStreamFrom;



}
#endif

static void _AUD_SetCustSpecGpioEnable(BOOL fgEnable)
{
    UINT32 u4CustSpecGpio;
    INT32 i4RegGpioSwitch;
    AUD_GPIO_POLARITY_T eGpioPolariy;

    u4CustSpecGpio = DRVCUST_OptGet(eAudioCustSpecGpioCtrl);
    eGpioPolariy =  (AUD_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioSpecGpioPolarity);

    if (u4CustSpecGpio != 0xffffffff)
    {
        if (eGpioPolariy == AUD_GPIO_HIGH_ENALBE)
        {
            i4RegGpioSwitch = fgEnable ? 1 : 0;
        }
        else
        {
            i4RegGpioSwitch = fgEnable ? 0 : 1;
        }
        LOG(8, "Aud Cust Spec Gpio Switch : %d\n", i4RegGpioSwitch);
        UNUSED(GPIO_Output(u4CustSpecGpio, (INT32 *)&i4RegGpioSwitch));
    }
    else
    {
        LOG(8, "Not support cust specGPIO\n");
    }
}

static void _AUD_SetCustSpec2GpioEnable(BOOL fgEnable)
{
    UINT32 u4CustSpecGpio;
    INT32 i4RegGpioSwitch;
    AUD_GPIO_POLARITY_T eGpioPolariy;

    u4CustSpecGpio = DRVCUST_OptGet(eAudioCustSpec2GpioCtrl);
    eGpioPolariy =  (AUD_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioSpec2GpioPolarity);

    if (u4CustSpecGpio != 0xffffffff)
    {
        if (eGpioPolariy == AUD_GPIO_HIGH_ENALBE)
        {
            i4RegGpioSwitch = fgEnable ? 1 : 0;
        }
        else
        {
            i4RegGpioSwitch = fgEnable ? 0 : 1;
        }
        LOG(8, "Aud Cust Spec Gpio Switch : %d\n", i4RegGpioSwitch);
        UNUSED(GPIO_Output(u4CustSpecGpio, (INT32 *)&i4RegGpioSwitch));
    }
    else
    {
        LOG(8, "Not support cust specGPIO\n");
    }
}


static void _AUD_SetCustHpGpioEnable(BOOL fgEnable)
{
    UINT32 u4CustHpGpio;
    INT32 i4RegGpioSwitch;
    AUD_GPIO_POLARITY_T eGpioPolariy;

    u4CustHpGpio = DRVCUST_OptGet(eAudioCustHpGpioCtrl);
    eGpioPolariy =  (AUD_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioHpGpioPolarity);

    if (u4CustHpGpio != 0xffffffff)
    {
        if (eGpioPolariy == AUD_GPIO_HIGH_ENALBE)
        {
            i4RegGpioSwitch = fgEnable ? 1 : 0;
        }
        else
        {
            i4RegGpioSwitch = fgEnable ? 0 : 1;
        }
        LOG(8, "Aud Cust Spec Gpio Switch : %d\n", i4RegGpioSwitch);
        UNUSED(GPIO_Output(u4CustHpGpio, (INT32 *)&i4RegGpioSwitch));
    }
    else
    {
        LOG(8, "Not support cust HP GPIO\n");
    }
}

//-----------------------------------------------------------------------------
/** _AUD_ResetAmp
 *  Reset Amp
 *
 *  @param  fgEnable   TRUE (shut down), FALSE (not shut down)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _AUD_ResetAmp(BOOL fgEnable)
{
    UINT32 u4AmpSdGpio;
    INT32 i4RegGpioSwitch;
    AUD_GPIO_POLARITY_T eGpioPolariy;

    u4AmpSdGpio = DRVCUST_OptGet(eAudioAmpResetGpioNum);
    eGpioPolariy =  (AUD_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioAmpResetGpioPolarity);

    if (u4AmpSdGpio != 0xffffffffU)
    {
        if (eGpioPolariy == AUD_GPIO_HIGH_ENALBE)
        {
            i4RegGpioSwitch = fgEnable ? 1 : 0;
        }
        else
        {
            i4RegGpioSwitch = fgEnable ? 0 : 1;
        }
        LOG(8, "Aud Amp Reset Gpio Switch : %d\n", i4RegGpioSwitch);
        UNUSED(GPIO_Output((INT32)u4AmpSdGpio, (INT32 *)&i4RegGpioSwitch));
    }
}


//-----------------------------------------------------------------------------
/** _ConfigAPLLK1K2
 *  Config APLL K1 K2 value
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  K1        (2,3)
 *  @param  K2        (2 ~ 48)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
BOOL  _AUD_PLL_ConfigDivider(UINT8 u1DecId, UINT8 K1, UINT8 K2)
{
    UINT16  u2K1;
    UINT16  u2K2;

    AUD_DEC_ID_VALIDATE(u1DecId);
    

    if (K1 == 2)
    {
        u2K1 = 0;
    } 
    else if (K1 == 3)
    {
        u2K1 = 1;
    }
    else
    {
        return FALSE; 
    }
  
    if ((K2 >= K2_VALID_VALUE_MIN) && (K2 <= K2_VALID_VALUE_MAX))
    {
        u2K2 = K2; 
    } 
    else
    {
        return FALSE;
    }

  
    if (u1DecId == AUD_DEC_MAIN)
    {
        vIO32WriteFldAlign(CKGEN_APLLCFG, u2K2, FLD_APLL_K2);          
        vIO32WriteFldAlign(CKGEN_APLLCFG, u2K1, FLD_APLL_K1);
    }
    else // APLL1
    {
        vIO32WriteFldAlign(CKGEN_APLL2CFG, u2K2, FLD_APLL2_K2);          
        vIO32WriteFldAlign(CKGEN_APLL2CFG, u2K1, FLD_APLL2_K1);
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _AudPllInit
 *  Audio PLL initial
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _AudPllInit(void)
{
#if 0
    UINT8  ucBand;
    UINT16 u2MS;
    UINT16 u2NS;
    UINT16 u2Counter;
#endif
    UINT32 u4Apll0Result;
    UINT32 u4Apll1Result;

    //INPUT SEL,XTAL
    vIO32WriteFldAlign(CKGEN_AFECFG19, 0, FLD_RG_APLL1_IN_SEL1);
    //CHP
    vIO32WriteFldAlign(CKGEN_AFECFG19, 0x0, FLD_RG_APLL1_CHP); // 2.5uA
    //LPF_R,Non-HDMI
    vIO32WriteFldAlign(CKGEN_AFECFG19, 0x3, FLD_RG_APLL1_LPF_R_SEL); //6.6kohm.
    //PRE DIV?????????
    vIO32WriteFldAlign(CKGEN_APLL11, 0, FLD_APLL1_PRE_DIV);//kevin_pll
    //Feedback DIV
    vIO32WriteFldAlign(CKGEN_APLL1, 0x867C3ECE, FLD_SDIN_APLL1);
    vIO32WriteFldAlign(CKGEN_APLL11, 0x3, FLD_SDREF);

    //APLL1 Power down
    vIO32WriteFldAlign(CKGEN_AFECFG19, 1, FLD_RG_APLL1_PWD);
    //APLL1 Power on
    vIO32WriteFldAlign(CKGEN_AFECFG19, 0, FLD_RG_APLL1_PWD);
	
#if defined(CC_MT8223)
	//APLL1 segmadelta dither_on
	vIO32WriteFldAlign(CKGEN_APLL11, 1, FLD_DITHER_EN);
#endif

#if 0
    vIO32WriteFldAlign(CKGEN_AFECFG0, 1, FLD_RG_APLL1_BIASOPT_EN);
    vIO32WriteFldAlign(CKGEN_AFECFG14, 1, FLD_RG_APLL2_BIASOPT_EN);
    vIO32WriteFldAlign(CKGEN_AFECFG3, 1, FLD_RG_APLL1_BANDGAP_PD);
    vIO32WriteFldAlign(CKGEN_AFECFG3, 0, FLD_RG_APLL1_BANDGAP_PD);

    LOG(3, "Aud HW Init: Calibrate APLL1 to %d Hz...\n", _au4ApllClk[0]);
    VERIFY(BSP_Calibrate(CAL_SRC_APLL1, _au4ApllClk[0]));
    _au4Apll1ClkBand[0] = IO32ReadFldAlign(CKGEN_AFECFG0, FLD_RG_APLL1_VCO_BAND);

    LOG(3, "Aud HW Init: Calibrate APLL1 to %d Hz...\n", _au4ApllClk[1]);
    VERIFY(BSP_Calibrate(CAL_SRC_APLL1, _au4ApllClk[1]));
    _au4Apll1ClkBand[1] = IO32ReadFldAlign(CKGEN_AFECFG0, FLD_RG_APLL1_VCO_BAND);

    LOG(3, "Aud HW Init: Calibrate APLL2 to %d Hz...\n", _au4ApllClk[0]);
    VERIFY(BSP_Calibrate(CAL_SRC_APLL2, _au4ApllClk[0]));
    _au4Apll2ClkBand[0] = IO32ReadFldAlign(CKGEN_AFECFG14, FLD_RG_APLL2_VCO_BAND);

    LOG(3, "Aud HW Init: Calibrate APLL2 to %d Hz...\n", _au4ApllClk[1]);
    VERIFY(BSP_Calibrate(CAL_SRC_APLL2, _au4ApllClk[1]));
    _au4Apll2ClkBand[1] = IO32ReadFldAlign(CKGEN_AFECFG14, FLD_RG_APLL2_VCO_BAND);


    u4Apll0Result = BSP_GetClock(CAL_SRC_APLL1, &ucBand, &u2MS, &u2NS, &u2Counter);
    u4Apll1Result = BSP_GetClock(CAL_SRC_APLL2, &ucBand, &u2MS, &u2NS, &u2Counter);
#endif
    // To suppress warning messages for release version
    UNUSED(u4Apll0Result);
    UNUSED(u4Apll1Result);

    //LOG(3, "APLL1, APLL2 = (%d, %d)\n",u4Apll0Result, u4Apll1Result);

    // Initial set sampling freqnecy MAIN: 44.1K, AUX: 48K
    UNUSED(_AUD_PLL_ConfigDivider(AUD_DEC_MAIN, 3, 8));
    UNUSED(_AUD_PLL_ConfigDivider(AUD_DEC_AUX, 3, 8));
#if 0
    if (IS_SYSPLL_432())
    {
        vIO32WriteFldAlign(CKGEN_AFECFG0, 7, FLD_RG_APLL1_PREDIV);
        vIO32WriteFldAlign(CKGEN_AFECFG14, 7, FLD_RG_APLL2_PREDIV);
    }
    else
    {
        //for CID spur issue
        //vIO32WriteFldAlign(CKGEN_AFECFG0, 5, FLD_RG_APLL1_PREDIV);
        //vIO32WriteFldAlign(CKGEN_AFECFG14, 5, FLD_RG_APLL2_PREDIV);
        vIO32WriteFldAlign(CKGEN_AFECFG0, 4, FLD_RG_APLL1_PREDIV);
        vIO32WriteFldAlign(CKGEN_AFECFG14, 4, FLD_RG_APLL2_PREDIV);
    }

    vIO32WriteFldAlign(CKGEN_AFECFG0, 0, FLD_RG_APLL1_VCOINIT_ENB);
    vIO32WriteFldAlign(CKGEN_AFECFG0, 0, FLD_RG_APLL1_TEST_B_0);
    vIO32WriteFldAlign(CKGEN_AFECFG0, 2, FLD_RG_APLL1_VCO_BAND);
    vIO32WriteFldAlign(CKGEN_AFECFG0, 2, FLD_RG_APLL1_VCOG_B);
    vIO32WriteFldAlign(CKGEN_AFECFG0, 2, FLD_RG_APLL1_V2I_RB);
    vIO32WriteFldAlign(CKGEN_AFECFG0, 0, FLD_RG_APLL1_TEST_EN);
    vIO32WriteFldAlign(CKGEN_AFECFG0, 1, FLD_RG_APLL1_LPF_R_B);

    vIO32WriteFldAlign(CKGEN_AFECFG14, 0, FLD_RG_APLL2_VCOINIT_ENB);
    vIO32WriteFldAlign(CKGEN_AFECFG14, 2, FLD_RG_APLL2_VCO_BAND);
    vIO32WriteFldAlign(CKGEN_AFECFG14, 2, FLD_RG_APLL2_VCOG_B);
    vIO32WriteFldAlign(CKGEN_AFECFG14, 2, FLD_RG_APLL2_V2I_RB);
    vIO32WriteFldAlign(CKGEN_AFECFG14, 0, FLD_RG_APLL2_TEST_EN);

    vIO32WriteFldAlign(CKGEN_AFECFG14, 1, FLD_RG_APLL2_LPF_R_B);

    vIO32WriteFldAlign(CKGEN_AFECFG2, 2, FLD_RG_APLL1_SCFDIV);
    vIO32WriteFldAlign(CKGEN_AFECFG16, 2, FLD_RG_APLL2_SCFDIV);

    // Setup apll LPF dividor
    vIO32WriteFldAlign(CKGEN_AFECFG2, 0x2f, FLD_RG_APLL1_LPFDIV);
    vIO32WriteFldAlign(CKGEN_AFECFG16, 0x2f, FLD_RG_APLL2_LPFDIV);

    // Setup apll mod dividor
    vIO32WriteFldAlign(CKGEN_AFECFG2, 0x17, FLD_RG_APLL1_MODDIV);
    vIO32WriteFldAlign(CKGEN_AFECFG16, 0x17, FLD_RG_APLL2_MODDIV);
#endif
    // Use sys pll as apll reference clock as default
    _AudPllClkRefInternal(APLL_ID_MAIN, TRUE);
    _AudPllClkRefInternal(APLL_ID_AUX, TRUE);

    // Setup clock source as apll
    //MT8223_NOTES_Zhaoyuan: CKGEN input allways from APLL0
    vIO32WriteFldAlign(CKGEN_AUD_CKCFG, 1, FLD_AUD_CK_SEL);
    vIO32WriteFldAlign(CKGEN_AUD2_CKCFG, 1, FLD_AUD2_CK_SEL);


}



static void _AudPllFsSetting(UINT8 u1ApllId, SAMPLE_FREQ_T eFs)
{
    //Internal DAC PLL divider setting
    //UINT32 u4ApllLPFReg;
#if 0 //SHINK_8223_CODE
    if (u1ApllId == APLL_ID_MAIN)
    {
        u4ApllLPFReg = CKGEN_AFECFG2;
    }
    else
    {
        u4ApllLPFReg = CKGEN_AFECFG16;
    }
#endif
    switch (eFs)
    {
#if 0 //SHINK_8223_CODE
        case FS_24K:
            vIO32WriteFldAlign(u4ApllLPFReg, 0x5F, FLD_RG_APLL1_LPFDIV);
            vIO32WriteFldAlign(u4ApllLPFReg, 0x23, FLD_RG_APLL1_MODDIV);
            break;
        case FS_32K://8.192
            vIO32WriteFldAlign(u4ApllLPFReg, 0x47, FLD_RG_APLL1_LPFDIV);
            vIO32WriteFldAlign(u4ApllLPFReg, 0x23, FLD_RG_APLL1_MODDIV);
            break;
        case FS_44K:
        case FS_48K://12.288
            vIO32WriteFldAlign(u4ApllLPFReg, 0x2F, FLD_RG_APLL1_LPFDIV);
            vIO32WriteFldAlign(u4ApllLPFReg, 0x17, FLD_RG_APLL1_MODDIV);
            break;
        case FS_88K:
        case FS_96K://
            vIO32WriteFldAlign(u4ApllLPFReg, 0x17, FLD_RG_APLL1_LPFDIV);
            vIO32WriteFldAlign(u4ApllLPFReg, 0xb, FLD_RG_APLL1_MODDIV);
            break;
        case FS_176K:
        case FS_192K:
            vIO32WriteFldAlign(u4ApllLPFReg, 0xb, FLD_RG_APLL1_LPFDIV);
            vIO32WriteFldAlign(u4ApllLPFReg, 0x5, FLD_RG_APLL1_MODDIV);
            break;
#endif
        default:   //48k setting


            vIO32WriteFldAlign(CKGEN_APLL11, 0, FLD_APLL1_PRE_DIV);//kevin_pll
            vIO32WriteFldAlign(CKGEN_APLL1, 0x867C3ECE, FLD_SDIN_APLL1);
            vIO32WriteFldAlign(CKGEN_APLL11, 0x3, FLD_SDREF);
#if 0
            vIO32WriteFldAlign(u4ApllLPFReg, 0x2F, FLD_RG_APLL1_LPFDIV);
            vIO32WriteFldAlign(u4ApllLPFReg, 0x17, FLD_RG_APLL1_MODDIV);
#endif
            break;
    }
}



MCLK_FREQUENCY_T AUD_GetMclkFs(UINT8 u1ApllId)
{    
    return  _arAudOutCfg[u1ApllId].eSampleFreq;
}


static void _AudPllFsDivSetting(UINT8 u1ApllId, SAMPLE_FREQ_T eFs)
{
    MCLK_FREQUENCY_T eMclk = AUD_GetMclkFs(u1ApllId);

    // Setup APLL divider
    switch (eFs)
    {
#if 0 //SHINK_8223_CODE
        case FS_16K: //256fs
            UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 24)); // Set K1 = 3, K2 = 24
            LOG(5, "Set APLL[%d] K1&K2 = FS_16K\n", u1ApllId);
            break;
        case FS_22K: //256fs
            UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 16)); // Set K1 = 3, K2 = 16
            LOG(5, "Set APLL[%d] K1&K2 = FS_22K\n", u1ApllId);
            break;
        case FS_24K: //256fs
            UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 16)); // Set K1 = 3, K2 = 16
            LOG(5, "Set APLL[%d] K1&K2 = FS_24K\n", u1ApllId);
            break;
        case FS_32K:
            switch (eMclk)
            {
                case MCLK_128FS:
                    UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 24)); // Set K1 = 3, K2 = 24, MCLK = 4.096 MHz
                    break;
                case MCLK_256FS:
                    UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 12)); // Set K1 = 3, K2 = 12, MCLK = 8.192 MHz
                    break;
                case MCLK_384FS:
                    UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 2, 12)); // Set K1 = 2, K2 = 12, MCLK = 12.288 MHz
                    break;
                case MCLK_512FS:
                    UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 6)); // Set K1 = 3, K2 = 6, MCLK = 16.384 MHz
                    break;
                case MCLK_768FS:
                    //to do
                    break;
                default:
                    break;
            }
            LOG(5, "Set APLL[%d] K1&K2 = FS_32K\n", u1ApllId);
            break;
        case FS_44K:
            switch (eMclk)
            {
                case MCLK_128FS:
                    UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 16)); // Set K1 = 3, K2 = 16, MCLK = 5.6448 MHz
                    break;
                case MCLK_256FS:
                    UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 8)); // Set K1 = 3, K2 = 8, MCLK = 11.2896 MHz
                    break;
                case MCLK_384FS:
                    UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 2, 8)); // Set K1 = 2, K2 = 8, MCLK = 16.9340 MHz
                    break;
                case MCLK_512FS:
                    UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 4)); // Set K1 = 3, K2 = 8, MCLK = 22.5792 MHz
                    break;
                case MCLK_768FS:
                    //to do
                    break;
                default:
                    break;
            }
            LOG(5, "Set APLL[%d] K1&K2 = FS_44K\n", u1ApllId);
            break;
#endif
        case FS_48K:
            switch (eMclk)
            {
                case MCLK_128FS:
                    UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 16)); // Set K1 = 3, K2 = 16, MCLK = 6.144 MHz
                    break;
                case MCLK_256FS:
                    UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 8)); // Set K1 = 3, K2 = 8, MCLK = 12.288 MHz
                    break;
                case MCLK_384FS:
                    UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 2, 8)); // Set K1 = 2, K2 = 8, MCLK = 18.432 MHz
                    break;
                case MCLK_512FS:
                    UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 4)); // Set K1 = 3, K2 = 4, MCLK = 24.576 MHz
                    break;
                case MCLK_768FS:
                    //to do
                    break;
                default:
                    //to do
                    break;
            }
            LOG(5, "Set APLL[%d] K1&K2 = FS_48K\n", u1ApllId);
            break;
#if 0 //SHINK_8223_CODE    
        case FS_64K: //256fs
            UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 6)); // Set K1 = 3, K2 = 6
            LOG(5, "Set APLL[%d] K1&K2 = FS_64K\n", u1ApllId);
            break;
        case FS_88K: //256fs
            UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 4)); // Set K1 = 3, K2 = 4
            LOG(5, "Set APLL[%d] K1&K2 = FS_88K\n", u1ApllId);
            break;
        case FS_96K: //256fs
            UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 4)); // Set K1 = 3, K2 = 4
            LOG(5, "Set APLL[%d] K1&K2 = FS_96K\n", u1ApllId);
            break;
        case FS_176K:
            UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 2)); // Set K1 = 3, K2 = 2
            LOG(5, "Set APLL[%d] K1&K2 = FS_176K\n", u1ApllId);
            break;
        case FS_192K:
            UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 2)); // Set K1 = 3, K2 = 4, MCLK = 49.152MHz
            LOG(5, "Set APLL[%d] K1&K2 = FS_192K\n", u1ApllId);
            break;
        default:
            UNUSED(_AUD_PLL_ConfigDivider(u1ApllId, 3, 8)); // Set K1 = 3, K2 = 8, MCLK = 11.2896 MHz
            LOG(5, "Set APLL[%d] K1&K2 not normal, set FS_44K\n", u1ApllId);
            break;
#endif
    }


}

static BOOL _Is294DivClk(SAMPLE_FREQ_T eFs)
{
    if ((eFs==FS_16K) || (eFs==FS_24K) || (eFs==FS_32K) || (eFs==FS_48K) ||
        (eFs==FS_64K) || (eFs==FS_96K) || (eFs==FS_192K))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
/** _AClkSetting
 *  Audio clock setting \n
 *  Config Apll according to sampling freqency and MCLK.
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  u1Fs      A name string.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _AudPllSetting(UINT8 u1DecId, SAMPLE_FREQ_T eFs)
{
    UINT8  u1ApllNum = 0;
    SAMPLE_FREQ_T  eSmpFrq;
    //AUD_DEC_STREAM_FROM_T eStreamFrom;
    //AUD_FMT_T eFmt;
    //	  BOOL eDualDecMode;

    AUD_DEC_ID_VALIDATE(u1DecId);


    eSmpFrq = eFs;

    //MT8223_NOTES_Zhaoyuan: Hard code here to make CKGEN ouput 12.28M clk
    eSmpFrq = FS_48K;

#if 0 //SHINK_8223_CODE

    eStreamFrom = AUD_GetSrc(u1DecId);
    eFmt = AUD_GetFmt(u1DecId);
    eDualDecMode = AUD_GetDualDecMode();

#endif

    //Internal DAC digital mute.
    //AUD_SET_BIT(REG_DACCTL_CFG0, MUTE_0);
    //AUD_SET_BIT(REG_DACCTL_CFG2, MUTE_1);

#if 0
#ifdef CC_AUD_EU_INTERNALDAC_SAME_APLL
    if (0)
    {
    }
#else
    if ((eDualDecMode) && (u1DecId == AUD_DEC_MAIN))
    {
        // In dual decoder mode, only reference AUX decoder
        return ;
    }
#endif
#endif

    /*
    #ifndef SRC_SELF_TEST
    	if ((eDualDecMode) && (u1DecId == AUD_DEC_MAIN))
    #else
    	if (0)
    #endif
    	{
    		// In dual decoder mode, only reference AUX decoder
    		return ;
    	}
    	else // Normal case ..
      */
    {
#if 0 //SHINK_8223_CODE
        // Check if PCM format from memory playback
        if (((eStreamFrom == AUD_STREAM_FROM_MEMORY) ||
        (eStreamFrom == AUD_STREAM_FROM_BUF_AGT) ||
        (eStreamFrom == AUD_STREAM_FROM_FEEDER)) &&
        (eFmt == AUD_FMT_PCM))
        {
            eSmpFrq = AUD_GetPcmSmpFreq(u1DecId);
        }
#endif

        if (u1DecId == AUD_DEC_MAIN)
        {
            u1ApllNum = APLL_ID_MAIN;

            if (_Is294DivClk(eSmpFrq))
            {
                //change divider and change band
                if (IS_SYSPLL_432())
                {
                    vIO32WriteFldAlign(CKGEN_AFECFG1, APLL_MODIN_432MHZ_SYSCLK_294MHZ_APLL, FLD_RG_APLL1_MODIN);
                }
                else
                {
                    vIO32WriteFldAlign(CKGEN_AFECFG1, APLL_MODIN_324MHZ_SYSCLK_294MHZ_APLL, FLD_RG_APLL1_MODIN);
                }

                vIO32WriteFldAlign(CKGEN_AFECFG0, _au4Apll1ClkBand[0], FLD_RG_APLL1_VCO_BAND);
                _au4ApllClk[0] = CLK_APLL294MHZ;
            }
#if 0 //  SHINK_8223_CODE
            else
            {
                if (IS_SYSPLL_432())
                {
                    vIO32WriteFldAlign(CKGEN_AFECFG1, APLL_MODIN_432MHZ_SYSCLK_270MHZ_APLL, FLD_RG_APLL1_MODIN);
                }
                else
                {
                    vIO32WriteFldAlign(CKGEN_AFECFG1, APLL_MODIN_324MHZ_SYSCLK_270MHZ_APLL, FLD_RG_APLL1_MODIN);
                }

                vIO32WriteFldAlign(CKGEN_AFECFG0, _au4Apll1ClkBand[1], FLD_RG_APLL1_VCO_BAND);
                _au4ApllClk[0] = CLK_APLL270MHZ;
            }
#endif
            _AudPllFsSetting(u1ApllNum, eSmpFrq);
            _AudPllFsDivSetting(u1ApllNum, eSmpFrq);

            //Set DAC OSR
            //_MT5363Internal_DacOSRSel(u1ApllNum,eSmpFrq); //SHINK_8223_CODE

        }
        else
        {
            u1ApllNum = APLL_ID_AUX;

            if (_Is294DivClk(eSmpFrq))
            {
                if (IS_SYSPLL_432())
                {
                    vIO32WriteFldAlign(CKGEN_AFECFG15, APLL_MODIN_432MHZ_SYSCLK_294MHZ_APLL, FLD_RG_APLL2_MODIN);
                }
                else
                {
                    vIO32WriteFldAlign(CKGEN_AFECFG15, APLL_MODIN_324MHZ_SYSCLK_294MHZ_APLL, FLD_RG_APLL2_MODIN);
                }

                vIO32WriteFldAlign(CKGEN_AFECFG14, _au4Apll2ClkBand[0], FLD_RG_APLL2_VCO_BAND);
                _au4ApllClk[1] = CLK_APLL294MHZ;
            }
#if 0 //SHINK_8223_CODE
            else
            {
                if (IS_SYSPLL_432())
                {
                    vIO32WriteFldAlign(CKGEN_AFECFG15, APLL_MODIN_432MHZ_SYSCLK_270MHZ_APLL, FLD_RG_APLL2_MODIN);
                }
                else
                {
                    vIO32WriteFldAlign(CKGEN_AFECFG15, APLL_MODIN_324MHZ_SYSCLK_270MHZ_APLL, FLD_RG_APLL2_MODIN);
                }

                vIO32WriteFldAlign(CKGEN_AFECFG14, _au4Apll2ClkBand[1], FLD_RG_APLL2_VCO_BAND);
                _au4ApllClk[1] = CLK_APLL270MHZ;
            }
#endif

            _AudPllFsSetting(u1ApllNum, eSmpFrq);
            _AudPllFsDivSetting(u1ApllNum, eSmpFrq);

            //Set DAC OSR
            //_MT5363Internal_DacOSRSel(u1ApllNum,eSmpFrq);  //SHINK_8223_CODE

        }
        //Internal DAC digital unmute.
        //AUD_CLR_BIT(REG_DACCTL_CFG0, MUTE_0);
        //AUD_CLR_BIT(REG_DACCTL_CFG2, MUTE_1);
    }
}


//-----------------------------------------------------------------------------
/** _AudPllInit
 *  Audio PLL2  power down
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
#ifdef CC_MT5391_AUD_SUPPORT
static void _AudPll2OFF(void)
{

    _MT8297_DirectWRITE(0x2A, 0xA8, 0xff);//turn off bit 0

}
#endif

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _AUD_DMX_SetAudFifo
 *
 *  @param  u1DecId     Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  u4FifoStr
 *  @param  u4FifoEnd
 *
 *  @retval TRUE
 *  @retval FALSE
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DMX_SetAudFifo(UINT8 u1DecId, UINT32 u4FifoStr, UINT32 u4FifoEnd)
{

    AUD_DEC_ID_VALIDATE(u1DecId);

    /*
        // Check if FIFO start/end addresses are valid aligned
        if (!IsAligned(u4FifoStr, DMX_FIFO_ALIGNMENT) ||
            !IsAligned(u4FifoEnd, DMX_FIFO_ALIGNMENT))
        {
            return FALSE;
        }
    */
#ifndef CC_MT5391_AUD_3_DECODER
    if (u1DecId == AUD_DEC_MAIN) //Audio Description - decoder1 use parser pointer
#else
    if (u1DecId == AUD_DEC_AUX)
#endif
    {
        // Fill in the registers in Unit of 4 bytes
        AUD_WRITE32(REG_DMX_STR, DSP_INTERNAL_ADDR(u4FifoStr >> 2));
        AUD_WRITE32(REG_DMX_END, DSP_INTERNAL_ADDR(u4FifoEnd >> 2));
    }
    else
    {
        PARSER_WRITE32(REG_A0STA, DSP_INTERNAL_ADDR(u4FifoStr >> 8));
        PARSER_WRITE32(REG_A0END, DSP_INTERNAL_ADDR(u4FifoEnd >> 8));
    }

#ifdef CC_AUDIO_DSP_JPG_SUPPORT ///ADSP_JPEG_DEC2
    if((u1DecId == AUD_DEC_AUX)&&(_AudGetStrFormat(AUD_DEC_AUX) == AUD_FMT_JPEG))
    {
        PARSER_WRITE32(REG_VSTA, DSP_INTERNAL_ADDR(u4FifoStr >> 8));
        PARSER_WRITE32(REG_VEND, DSP_INTERNAL_ADDR(u4FifoEnd >> 8));
    }
#endif

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _AUD_DMX_GetAudFifo
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DMX_GetAudFifo(UINT8 u1DecId, UINT32 *pu4FifoStr, UINT32 *pu4FifoEnd)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Registers in Unit of 4 bytes
    if ((pu4FifoStr != NULL) && (pu4FifoEnd != NULL))
    {
        *pu4FifoStr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_DMX_STR) << 2);
        *pu4FifoEnd = DSP_PHYSICAL_ADDR(AUD_READ32(REG_DMX_END) << 2);
    }
}

//-----------------------------------------------------------------------------
/** _AUD_DMX_GetAudPtr
 *    Return the physical DRAM address
 *  @param  u1DecId     Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  pu4ReadPtr
 *  @param  pu4WritePtr
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DMX_GetAudPtr(UINT8 u1DecId, UINT32 *pu4ReadPtr, UINT32 *pu4WritePtr)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    //*pu4ReadPtr = AUD_READ32(REG_DMX_GTB_PNT);
    if ((pu4ReadPtr != NULL) && (pu4WritePtr != NULL))
    {
        *pu4ReadPtr = DSP_GetDspReadPtrPhysicalAddr(u1DecId);

#ifndef CC_MT5391_AUD_3_DECODER
        if (u1DecId == AUD_DEC_AUX) //Audio Description - decoder1 use parser pointer
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(PSR_SoftGetAudioWp());
        }
        else
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_DMX_WRITE_PNT));
        }
#else
        if (u1DecId != AUD_DEC_AUX) //3//main and THIRD use parser, 2nd use dmx
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(PSR_SoftGetAudioWp());
        }
        else
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_DMX_WRITE_PNT));
        }
#endif
    }
}

void _AUD_DMX_GetAudWrtPtr(UINT8 u1DecId, UINT32 *pu4WritePtr)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    //*pu4ReadPtr = AUD_READ32(REG_DMX_GTB_PNT);
    if ((pu4WritePtr != NULL))
    {

#ifndef CC_MT5391_AUD_3_DECODER
        if (u1DecId == AUD_DEC_AUX) //Audio Description - decoder1 use parser pointer
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(PSR_SoftGetAudioWp());
        }
        else
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_DMX_WRITE_PNT));
        }
#else
        if (u1DecId != AUD_DEC_AUX) //3//main and THIRD use parser, 2nd use dmx
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(PSR_SoftGetAudioWp());
        }
        else
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_DMX_WRITE_PNT));
        }
#endif
    }
}
//-----------------------------------------------------------------------------
/** _AUD_DMX_UpdateWritePtr
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DMX_UpdateWritePtr(UINT8 u1DecId, UINT32 u4WritePtr)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Check if write pointer address is within FIFO range
    if ((DSP_INTERNAL_ADDR(u4WritePtr) >= DSP_INTERNAL_ADDR(u4GetAFIFOEnd(u1DecId))) ||
            (DSP_INTERNAL_ADDR(u4WritePtr) < DSP_INTERNAL_ADDR(u4GetAFIFOStart(u1DecId))))
    {
        Printf("FIFO (%x, %x), Wp = %x\n", DSP_INTERNAL_ADDR(u4GetAFIFOStart(u1DecId)),  DSP_INTERNAL_ADDR(u4GetAFIFOEnd(u1DecId)), \
               DSP_INTERNAL_ADDR(u4WritePtr) );

        return FALSE;
    }

#ifndef CC_MT5391_AUD_3_DECODER
    if (u1DecId == AUD_DEC_MAIN)
    {
        AUD_WRITE32(REG_DMX_WRITE_PNT, DSP_INTERNAL_ADDR(u4WritePtr));	//laihui:REG_DMX_WRITE_PNT
    }
    else //if (u1DecId == AUD_DEC_AUX)
    {
        _AUD_BitStrPtrSel(AUD_DEC_AUX, AUD_STREAM_FROM_MEMORY);  //Important!!
        PSR_RiscSetAudioWp(u1DecId, DSP_INTERNAL_ADDR(u4WritePtr));
    }
#else
    if (u1DecId == AUD_DEC_AUX) //Audio Description - decoder3 use parser pointer
    {
        AUD_WRITE32(REG_DMX_WRITE_PNT, DSP_INTERNAL_ADDR(u4WritePtr));	//laihui:REG_DMX_WRITE_PNT
    }
    else //if (u1DecId != AUD_DEC_AUX)
    {
        //_AUD_BitStrPtrSel(u1DecId, AUD_STREAM_FROM_DIGITAL_TUNER);  //Important!!
        PSR_RiscSetAudioWp(u1DecId, DSP_INTERNAL_ADDR(u4WritePtr));
    }
#endif

    return TRUE;
}

BOOL _AUD_DEC2_UpdateWritePtr(UINT8 u1DecId, UINT32 u4WritePtr)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Check if write pointer address is within FIFO range
    if ((DSP_INTERNAL_ADDR(u4WritePtr) >= DSP_INTERNAL_ADDR(u4GetAFIFOEnd(u1DecId))) ||
            (DSP_INTERNAL_ADDR(u4WritePtr) < DSP_INTERNAL_ADDR(u4GetAFIFOStart(u1DecId))))
    {
        Printf("FIFO (%x, %x), Wp = %x\n", DSP_INTERNAL_ADDR(u4WritePtr),  DSP_INTERNAL_ADDR(u4GetAFIFOStart(u1DecId)), \
               DSP_INTERNAL_ADDR(u4GetAFIFOEnd(u1DecId)));

        return FALSE;
    }
    AUD_WRITE32(REG_DMX_WRITE_PNT, DSP_INTERNAL_ADDR(u4WritePtr));
    return TRUE;
}

//-----------------------------------------------------------------------------
/** _AUD_DMX_UpdateWPtr
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
VOID _AUD_DMX_UpdateWPtr(UINT8 u1DecId, UINT32 u4Wp)
{
    VERIFY(_AUD_DMX_UpdateWritePtr(u1DecId, u4Wp));
}

//-----------------------------------------------------------------------------
/** _AUD_BitStrPtrSel
 *
 *  @param  u1DecId       Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  eStreamFrom
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_BitStrPtrSel(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    UINT32 u4Format = 0;

    AUD_DEC_ID_VALIDATE(u1DecId);

    if (eStreamFrom != AUD_STREAM_FROM_ANALOG_TUNER)
    {
#ifndef CC_MT5391_AUD_3_DECODER
        if (u1DecId == AUD_DEC_MAIN)
        {
            u4Format = AUD_READ32_SPECIAL(REG_BS_CFG) & (~PTR_SEL_BANK0_MASK);
        }
        else  // AUX Decoder
        {
            u4Format = AUD_READ32_SPECIAL(REG_BS_CFG) & (~PTR_SEL_BANK1_MASK);
        }
#else
        if (u1DecId == AUD_DEC_AUX)
        {
            u4Format = AUD_READ32_SPECIAL(REG_BS_CFG) & (~PTR_SEL_BANK1_MASK);
        }
        else  // THIRD Decoder, Audio Description
        {
            u4Format = AUD_READ32_SPECIAL(REG_BS_CFG) & (~PTR_SEL_BANK0_MASK);
        }
#endif
    }

    switch (eStreamFrom)
    {
            //Audio Description decoder1 - use soft parser pointer
        case AUD_STREAM_FROM_DIGITAL_TUNER:
        case AUD_STREAM_FROM_MULTI_MEDIA:
            //AUD_WRITE32(REG_BS_CFG, u4Format |
            //	         ((u1DecId == AUD_DEC_MAIN) ? PTR_SEL_BANK0_DMX : PTR_SEL_BANK1_DMX));
#ifndef CC_MT5391_AUD_3_DECODER
            AUD_WRITE32(REG_BS_CFG, u4Format |
                        ((u1DecId == AUD_DEC_MAIN) ? PTR_SEL_BANK0_DMX : PTR_SEL_BANK1_PSR));
#else
            //AUD_WRITE32(REG_BS_CFG, u4Format |
            //		         ((u1DecId == AUD_DEC_THIRD) ? PTR_SEL_BANK1_PSR : PTR_SEL_BANK0_DMX ));
            AUD_WRITE32(REG_BS_CFG, u4Format |
                        ((u1DecId == AUD_DEC_AUX) ? PTR_SEL_BANK1_DMX : PTR_SEL_BANK0_PSR ));
#endif
           
#ifdef CC_AUDIO_DSP_JPG_SUPPORT ///ADSP_JPEG_DEC2
           if((u1DecId == AUD_DEC_AUX) && (_AudGetStrFormat(AUD_DEC_AUX) == AUD_FMT_JPEG))
           {
                //[Joel] Clean PAR_SEL2 setting
                u4Format = AUD_READ32_SPECIAL(REG_BS_CFG) & (~(PAR_SEL2_MASK|PTR_SEL_BANK1_MASK)); 
                AUD_WRITE32(REG_BS_CFG, u4Format | PAR_SEL2 | PTR_SEL_BANK1_VFIFO);   /// (Dec2 select Parser Bank1 | Bank1 select VFIFO be Getbits point)
           }        
#endif
            break;

        case AUD_STREAM_FROM_MEMORY:
        case AUD_STREAM_FROM_OTHERS:
        case AUD_STREAM_FROM_FEEDER:
        case AUD_STREAM_FROM_BUF_AGT:
            AUD_WRITE32(REG_BS_CFG, u4Format |
                        ((u1DecId == AUD_DEC_MAIN) ? PTR_SEL_BANK0_PSR : PTR_SEL_BANK1_PSR));
            break;

        case AUD_STREAM_FROM_SPDIF:
        case AUD_STREAM_FROM_HDMI:
        case AUD_STREAM_FROM_LINE_IN:
            AUD_WRITE32(REG_BS_CFG, u4Format |
                        ((u1DecId == AUD_DEC_MAIN) ?
                         PTR_SEL_BANK0_SPDIF_LINE_IN : PTR_SEL_BANK1_SPDIF_LINE_IN));
            break;

        default:
            break;
    }
}

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** AUD_HwInit
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_HwInit(void)
{
    UINT8 u1DecId;
    AUD_AOUT_PAD_CFG_T* prAOutPadCfg;
    AUD_INTER_DAC_CFG_T* prInterDacCfg;
    static BOOL fgInit = FALSE;
    UINT32 u4Reg4 = 0;
    UNUSED(_rInterDacCfg);

    if (!fgInit)
    {
        VERIFY(x_sema_create(&_hSmpChgSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        fgInit = TRUE;
    }

    ADAC_Mute(TRUE);
    ADAC_GpioCodecMute(TRUE);
    ADAC_GpioAmpEnable(FALSE);//xiaoyi for power on popo noise 100312

    // Load internal dac channel setup for apll clock reference,
    // Please refer to _IsInterDacRefMainDec() function
#ifdef CC_AUD_DAC_SPECIAL
    if (AUD_GetTargetCountry() == COUNTRY_EU)
    {
        UNUSED(DRVCUST_OptQuery(eAudioInterDacChlSpecialConfig, (UINT32 *)(void *)&prInterDacCfg));
    }
    else
    {
        UNUSED(DRVCUST_OptQuery(eAudioInterDacChlConfig, (UINT32 *)(void *)&prInterDacCfg));
    }
#else
    UNUSED(DRVCUST_OptQuery(eAudioInterDacChlConfig, (UINT32 *)(void *)&prInterDacCfg));
#endif

    _rInterDacCfg.eDac0ChlSel = prInterDacCfg->eDac0ChlSel;
    _rInterDacCfg.eDac1ChlSel = prInterDacCfg->eDac1ChlSel;
    _rInterDacCfg.eDac2ChlSel = prInterDacCfg->eDac2ChlSel;

    // Setup apll, default set sampling rate = 48 KHz
    if (!BSP_IsFPGA())
    {
        _AudPllInit();
#ifdef CC_MT5391_AUD_SUPPORT
        if (AUD_GetTargetCountry() == COUNTRY_US)
        {
            //power down APLL2
            _AudPll2OFF();
        }
#endif

    }

    // Select audio output pinmux
    _AudPinmuxInit();
   // Select audio output pinmux
   u4Reg4 = IO32ReadFldAlign(CKGEN_PMUX0, FLD_PMUX0);
   u4Reg4 = (u4Reg4 & ~(0x01f80000)) | 0x00900000;   
    vIO32WriteFldAlign(CKGEN_PMUX0, u4Reg4, FLD_PMUX0);  

    UNUSED(DRVCUST_OptQuery(eAudioAoutPadConfig, (UINT32 *)(void *)&prAOutPadCfg));
    if (prAOutPadCfg != NULL)
    {
        x_memcpy(&_grAOutPadCfg, prAOutPadCfg, sizeof(AUD_AOUT_PAD_CFG_T));
    }

    AUD_AoutPADMux(_grAOutPadCfg.ePadSdata0, _grAOutPadCfg.ePadSdata1,
                   _grAOutPadCfg.ePadSdata2, _grAOutPadCfg.ePadSdata3,
                   _grAOutPadCfg.ePadSdata4, _grAOutPadCfg.ePadSdata5,
                   MCLK0);

    // Turn off spdif/line-in
    AUD_CLR_BIT(REG_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
    VERIFY(BIM_DisableIrq(VECTOR_SPDIF));

    // Load audio in/out default setting
    _AudInOutCfgInit();

    // Setup audio output format
    for (u1DecId = AUD_DEC_MAIN; u1DecId < AUD_AOUT_NO ; u1DecId++)
    {
        // Audio output format config
        AUD_AoutCfg(u1DecId, &_arAudOutCfg[u1DecId]);

        // Setup audio clock: FS_32K, FS_44K, FS_48K
        _AudPllSetting(u1DecId, _aeSampleRate[u1DecId]);

        // Setup audio output(MCLK, BCLK , LRCK, AOSDATA) driving current
        AUD_AoutDrvCur(u1DecId, _au1MckDrvCur[u1DecId], _au1BckDrvCur[u1DecId] , _au1LRckDrvCur[u1DecId] , _au1AOSDATADrvCur[u1DecId]);

    }

    // Turn off audio output 128 bit mode
    AUD_CLR_BIT(REG_AOUT_DRAM_CFG, DRAM_ACCESS_128_BITS);

    AUD_AinCfg(AUD_STREAM_FROM_LINE_IN, &_rLineInCfg); // FIXME

    // Open MCLK ,
    AUD_OutPadEnable(AUD_DEC_MAIN, TRUE);
    AUD_OutPadEnable(AUD_DEC_AUX, TRUE);

    // Audio DAC init
    ADAC_Init();
    #if 0 //SHINK_8223_CODE
    _AudInterDacClkSel(INTERNAL_DAC_ID_MAIN, APLL_ID_MAIN);
    _AudInterDacClkSel(INTERNAL_DAC_ID_AUX, APLL_ID_MAIN);
    _AudInterDacClkSel(INTERNAL_DAC_ID_THIRD, APLL_ID_MAIN);
    #endif

    // FIXME
    // clock source config
    //AUD_WRITE32(REG_ACK_CFG, 0x05220122);
    // Line in config
    AUD_WRITE32(REG_LIN_CFG0, 0x00000802);

    //MCLK direction
    AUD_WRITE32(0x03fc, 0x01e60000);


    //SPDIF enable
    //UNUSED(AUD_DspIECConfig(AUD_DEC_MAIN,AUD_IEC_CFG_PCM,1));

    _AUD_DrvApllMonThreadGo(TRUE);

    _AUD_SetCustSpecGpioEnable(TRUE);
    _AUD_SetCustSpec2GpioEnable(TRUE);
    _AUD_SetCustHpGpioEnable(TRUE);

    _AUD_ResetAmp(FALSE);

#if 0//MT5387Merge
    // FIXME !! dsp rom access timing issue
    if (((BSP_GetIcVersion() >= IC_VER_5391_AA) && (BSP_GetIcVersion() <= IC_VER_5391_CC)) ||
            ((BSP_GetIcVersion() >= IC_VER_5392B_AA) && (BSP_GetIcVersion() <= IC_VER_5392B_E7)))
    {
        // For MT539x
        AUD_WRITE32(REG_DSPROM_CFG, 0xAC);
        AUD_WRITE32(REG_AINROM_CFG, 0x3);
    }
    else if ((BSP_GetIcVersion() >= IC_VER_5360_AA) && (BSP_GetIcVersion() <= IC_VER_5360_AF))
    {
        // For MT5360A
        AUD_WRITE32(REG_DSPROM_CFG, 0x54);
        AUD_WRITE32(REG_AINROM_CFG, 0x5);
    }
    else
#endif
    {
#ifdef CC_MT5387
        //AUD_WRITE32(REG_DSPROM_CFG, 0x23);
        //AUD_WRITE32(REG_AINROM_CFG, 0x2);
        //  MIT's new ROM DELSEL based on DSP timing margin.
    	AUD_WRITE32(REG_DSPROM_CFG, 0x23);
    	AUD_WRITE32(REG_AINROM_CFG, 0x2);

#else
        // for MT5360B
        AUD_WRITE32(REG_DSPROM_CFG, 0x134);
        AUD_WRITE32(REG_AINROM_CFG, 0x3);
#endif
    }

#ifndef CC_MT5387
    ADAC_GpioCodecMute(FALSE);  // For PWM first power on pop noise, 5388 let other module do GPIO unmute. MW: setoutport, DEMO : setSampFreq.
#endif

    // Laihui: Get bit hold issue
    AUD_SET_BIT(REG_BS_CFG, CLR_BS);

#ifdef CC_MT5387 //sunman power down Line in AD
   // _MT5387_ADCDOWN(TRUE);
#endif

    _fgInit = TRUE;
}

void AUD_CfgPcmConfig(UINT8 u1DecId, SAMPLE_FREQ_T eSamplingRate)
{
    UNUSED(_aePcmSmpFreq);
    switch (eSamplingRate)
    {
        case FS_8K:
            _aePcmSmpFreq[u1DecId] = FS_32K;
            break;
        case FS_11K:
            _aePcmSmpFreq[u1DecId] = FS_44K;
            break;
        case FS_12K:
            _aePcmSmpFreq[u1DecId] = FS_48K;
            break;
        case FS_16K:
            _aePcmSmpFreq[u1DecId] = FS_32K;
            break;
        case FS_22K:
            _aePcmSmpFreq[u1DecId] = FS_44K;
            break;
        case FS_24K:
            _aePcmSmpFreq[u1DecId] = FS_48K;
            break;
        default:
            _aePcmSmpFreq[u1DecId] = eSamplingRate;
    }
}

void AUD_ClkCfgDualDecMode(BOOL fgEnable)
{
    _fgDualDecMode = fgEnable;
}


void AUD_ClkSetToInternal(UINT8 u1DecId, AUD_FMT_T eFmt)
{
    _AudClkSrcSel(u1DecId, AUD_STREAM_FROM_LINE_IN, eFmt);
}

void AUD_StrInputEnable(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eFmt, BOOL fgEnable, UINT32 u4ReceivePesCnt)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Select audio getbs source
    _AUD_BitStrPtrSel(u1DecId, eStreamFrom);

    // Select clk source form apll ? spdif ? hdmi ?
#ifndef CC_MT5391_AUD_3_DECODER
    if (fgEnable)
#else
    if ((u1DecId !=  AUD_DEC_THIRD) && (fgEnable))
        // gallen, for dec3, we should not change any clock setting
#endif
    {
        _AudClkSrcSel(u1DecId, eStreamFrom, eFmt);
    }

    if (fgEnable)
    {
        if ((u1DecId == AUD_DEC_MAIN) && (eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER))
        {
            // If tnner is DTV.
            // close all analog input sources for "WORKING AROUND" a bug.
            // The bug is that: demod is disturbed if Line-in L channel is with data at DTV.
            // Shawn 2008/12/29
            ADAC_CodecAdChlSel(8);
        }

        switch (eStreamFrom)
        {
            case AUD_STREAM_FROM_BUF_AGT:
            case AUD_STREAM_FROM_MEMORY:
            case AUD_STREAM_FROM_FEEDER:
                VERIFY(PSR_SoftInit());
                VERIFY(PSR_SoftSetAudioFIFO(u1DecId, u4GetAFIFOStart(u1DecId), u4GetAFIFOEnd(u1DecId)));

                vDspSetFifoReadPtr(u1DecId, u4GetAFIFOStart(u1DecId));
                if (!fgDspReadPtrSet(u1DecId, u4GetAFIFOStart(u1DecId)))
                {
                    LOG(5, "DspReadPtrSet time out\n");
                }
                break;

            case AUD_STREAM_FROM_LINE_IN:
                // Setup audio fifo
                vSetSPDIFInFIFO(u4GetAFIFOStart(u1DecId), u4GetAFIFOEnd(u1DecId), u1DecId);
#ifndef CC_MT5391_AUD_3_DECODER
                VERIFY(PSR_SoftSetAudioFIFO(u1DecId, u4GetAFIFOStart(u1DecId), u4GetAFIFOEnd(u1DecId)));
#else
                VERIFY(PSR_SoftSetAudioFIFO( ((u1DecId == AUD_DEC_AUX) ? AUD_DEC_AUX : AUD_DEC_MAIN), u4GetAFIFOStart(u1DecId), u4GetAFIFOEnd(u1DecId)));
                // The 1st and 3rd decoders both use 1st AFIFO
#endif
                // Turn off input when config format
                AUD_AinEnable(u1DecId, eStreamFrom, FALSE);
                // Format config
                AUD_AinSrcSel(u1DecId, eStreamFrom, eFmt);
                // Turn on input
                AUD_AinEnable(u1DecId, eStreamFrom, TRUE);

#ifndef AUD_HW_LR_INVERSE_DECT
                AUD_LineInCtrlEnable(u1DecId, FALSE);
                AUD_LineInCtrlEnable(u1DecId, TRUE);
#else
                AUD_LineinLRDetect(TRUE);
#endif //AUD_HW_LR_INVERSE_DECT
                AUD_SET_BIT(REG_SDADC0_DRV, RST_FILTER);
                AUD_CLR_BIT(REG_SDADC0_DRV, RST_FILTER);
		  vWriteShmUINT16(W_PCM_PREBUF_DELAY_BANK, 0x1);    	
#ifdef CC_MT5387
               // _MT5387_ADCDOWN(FALSE);
#endif
                break;

            case AUD_STREAM_FROM_SPDIF:
            case AUD_STREAM_FROM_HDMI:
                // Setup audio fifo
                vSetSPDIFInFIFO(u4GetAFIFOStart(u1DecId), u4GetAFIFOEnd(u1DecId), u1DecId);
#ifndef CC_MT5391_AUD_3_DECODER
                VERIFY(PSR_SoftSetAudioFIFO(u1DecId, u4GetAFIFOStart(u1DecId), u4GetAFIFOEnd(u1DecId)));
#else
                VERIFY(PSR_SoftSetAudioFIFO( ((u1DecId == AUD_DEC_AUX) ? AUD_DEC_AUX : AUD_DEC_MAIN), u4GetAFIFOStart(u1DecId), u4GetAFIFOEnd(u1DecId)));
                // The 1st and 3rd decoders both use 1st AFIFO
#endif
                // Turn off input when config format
                AUD_AinEnable(u1DecId, eStreamFrom, FALSE);
                // Format config
                AUD_AinSrcSel(u1DecId, eStreamFrom, eFmt);
                // Turn on input
                AUD_AinEnable(u1DecId, eStreamFrom, TRUE);

                // Prevent L/R reverse
#ifndef AUD_HW_LR_INVERSE_DECT
                x_thread_delay(10);
                AUD_LineInCtrlEnable(u1DecId, FALSE);
                AUD_LineInCtrlEnable(u1DecId, TRUE);
#else
                AUD_LineinLRDetect(TRUE);
#endif //AUD_HW_LR_INVERSE_DECT
		  if(Is_Chip_MT8223_E1()) //tracking mode parameter is different for 8223A & 8223B
		  {
		  	vWriteShmUINT16(W_PCM_PREBUF_DELAY_BANK, 0x1);
		  }
		  else
		  {
		  	vWriteShmUINT16(W_PCM_PREBUF_DELAY_BANK, 0x3);
		  }
                break;

            case AUD_STREAM_FROM_DIGITAL_TUNER:
            case AUD_STREAM_FROM_MULTI_MEDIA:
//#ifdef CC_MT5391_AUD_SUPPOR
#if 1
#ifndef CC_MT5391_AUD_3_DECODER
                if (u1DecId == AUD_DEC_MAIN)
#else
                if (u1DecId == AUD_DEC_AUX)
#endif
                {
                    PSR_SoftInit();
                    VERIFY(_AUD_DMX_SetAudFifo(u1DecId, u4GetAFIFOStart(u1DecId), u4GetAFIFOEnd(u1DecId)));
                    if (u4ReceivePesCnt == 0)    //write pointer is not updated in SendAudPes
                    {
                        VERIFY(_AUD_DMX_UpdateWritePtr(u1DecId, u4GetAFIFOStart(u1DecId)));
                    }
                }
                else
                {
                    //Audio Description - decoder1 use parser pointer
                    PSR_SoftInit();
                    VERIFY(_AUD_DMX_SetAudFifo(u1DecId, u4GetAFIFOStart(u1DecId), u4GetAFIFOEnd(u1DecId)));
                    if (u4ReceivePesCnt == 0)    //write pointer is not updated in SendAudPes
                    {
                        VERIFY(_AUD_DMX_UpdateWritePtr(u1DecId, u4GetAFIFOStart(u1DecId)));
                    }
                }
#else
                if (u1DecId == AUD_DEC_THIRD) // Audio Description
                {
                    //Audio Description - decoder3 use parser pointer
                    PSR_SoftInit();
                    VERIFY(_AUD_DMX_SetAudFifo(u1DecId, u4GetAFIFOStart(u1DecId), u4GetAFIFOEnd(u1DecId)));
                    VERIFY(_AUD_DMX_UpdateWritePtr(u1DecId, u4GetAFIFOStart(u1DecId)));
                }
                else
                {
                    VERIFY(_AUD_DMX_SetAudFifo(u1DecId, u4GetAFIFOStart(u1DecId), u4GetAFIFOEnd(u1DecId)));
                    VERIFY(_AUD_DMX_UpdateWritePtr(u1DecId, u4GetAFIFOStart(u1DecId)));
                }
#endif
                break;

            case AUD_STREAM_FROM_ANALOG_TUNER:
#ifdef CC_MT5391_AUD_SUPPORT
                if (AUD_GetTargetCountry() == COUNTRY_EU)
                {
                    if (u1DecId == AUD_DEC_AUX)
                    {
                        _AudDemodSetting();
                    }
                    else if (_AudGetStrFormat(u1DecId) == AUD_FMT_DETECTOR)
                    {
                        _AudDemodSetting();
                    }
                }
                else
                {
                    _AudDemodSetting();
                }
#endif
#ifndef CC_MT5387
                DSP_SetAIFMux(SIF_PATH);
#endif
                break;

            case AUD_STREAM_FROM_OTHERS:
                break;

            default:
                break;
        }
    }
    else // Turn off stream input
    {
        switch (eStreamFrom)
        {
            case AUD_STREAM_FROM_MEMORY:
                break;
            case AUD_STREAM_FROM_BUF_AGT:
                break;
            case AUD_STREAM_FROM_LINE_IN:
                AUD_AinEnable(u1DecId, eStreamFrom, FALSE);
#ifdef CC_MT5387
               // _MT5387_ADCDOWN(TRUE);
#endif
#ifdef AUD_HW_LR_INVERSE_DECT
                AUD_LineinLRDetect(FALSE);
#endif
                break;
            case AUD_STREAM_FROM_SPDIF:
            case AUD_STREAM_FROM_HDMI:
                AUD_AinEnable(u1DecId, eStreamFrom, FALSE);
                // Switch clock source to apll since if there is no external clock,
                // dsp may fail to stop (flush done)
                _AudClkSrcSel(u1DecId, AUD_STREAM_FROM_LINE_IN, eFmt);
#ifdef AUD_HW_LR_INVERSE_DECT
                AUD_LineinLRDetect(FALSE);
#endif
		  vWriteShmUINT16(W_PCM_PREBUF_DELAY_BANK, 0x1);    	
                break;

            case AUD_STREAM_FROM_DIGITAL_TUNER:
            case AUD_STREAM_FROM_MULTI_MEDIA:
                // middleware will turn off dmx
                break;

            case AUD_STREAM_FROM_ANALOG_TUNER:
                // middleware will turn off tuner
                break;

            case AUD_STREAM_FROM_OTHERS:
                break;

            default:
                break;
        }
    }

}

//-----------------------------------------------------------------------------
/** AUD_SetSampleFreq
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  eSmpFreq  Samping frequency
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void  AUD_SetSampleFreq(UINT8 u1DecId, SAMPLE_FREQ_T eSmpFreq)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    SAMP_CHG_LOCK();
    if ((_aeSampleRate[u1DecId] != eSmpFreq) || _fgChangeSource[u1DecId])
    {
        _fgChangeSource[u1DecId] = FALSE;// DTV00126551 kai wang for reset apll when change source
        //Light: Mute external DAC or amplifier
        ADAC_Mute(TRUE);
        ADAC_GpioCodecMute(TRUE); //Call GPIO mute for PWM setting pop noise

#ifdef CC_AUD_5387_APLL_FIX   //5388 can't select APLL2 until ECO
        if (u1DecId == AUD_DEC_AUX)
        {
            u1DecId = AUD_DEC_MAIN;
        }
#endif

        _AudPllSetting(u1DecId, eSmpFreq);
#ifdef CC_MT5387
        //_MT5387_PWM_SetSamplingRate(eSmpFreq); //Reset PWN Ytam according to sampling rate.
#endif

        //Light: Unmute external DAC or amplifier
        ADAC_Mute(FALSE);
        ADAC_GpioCodecMute(FALSE);
    }
    else // _aeSampleRate[u1DecId] == eSmpFreq
    {
        //Laihui: The line in reset will cause spdif detect fail.
#if 0   //Laihui: Remove the line in reset flow in same freq.    
        if ((_aeStrSrcConf[u1DecId] == AUD_STREAM_FROM_HDMI) ||
                (_aeStrSrcConf[u1DecId] == AUD_STREAM_FROM_LINE_IN))
        {
            //Light: Mute external DAC or amplifier
            ADAC_Mute(TRUE);
#if 0
            //Light: Mute internal DAC (have pop noise)
#ifndef CC_MT5391_AUD_SUPPORT
            _MT536xInternal_Mute(3, TRUE);
#else
            _MT8297_Mute(3, TRUE); // 3: ALL DAC mute
#endif
#endif

            //vHDMIHandleAudFmtChange();
            // Light: Reset line-in interface after reseting HDMI audio clock to prevent from inverting L/R channels.
            AUD_LineInCtrlEnable(u1DecId, FALSE);
            AUD_LineInCtrlEnable(u1DecId, TRUE);

#if 0
            //Light: Unmute internal DAC
#ifndef CC_MT5391_AUD_SUPPORT
            _MT536xInternal_Mute(3, FALSE);
#else
            _MT8297_Mute(3, FALSE);
#endif
#endif
            //Light: Unmute external DAC or amplifier
            ADAC_Mute(FALSE);
        }
#endif
    }

    _aeSampleRate[u1DecId] = eSmpFreq;

    SAMP_CHG_UNLOCK();
}

//-----------------------------------------------------------------------------
/** AUD_GetSampleFreq
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval Samping frequency
 */
//-----------------------------------------------------------------------------
SAMPLE_FREQ_T AUD_GetSampleFreq(UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    return _aeSampleRate[u1DecId];
}

//-----------------------------------------------------------------------------
/** AUD_GetAinCfg
 *
 *  @param  eStreamFrom
 *  @param  prAinCfg
 *
 *  @retval TRUE
 *  @retval FALSE
 */
//-----------------------------------------------------------------------------
BOOL AUD_GetAinCfg(AUD_DEC_STREAM_FROM_T eStreamFrom, AIN_CFG_T* prAinCfg)
{
    if (!_fgInit)
    {
        return FALSE;
    }

    if (eStreamFrom == AUD_STREAM_FROM_LINE_IN)
    {
        x_memcpy((VOID *)prAinCfg, (const VOID *)&_rLineInCfg, sizeof(AIN_CFG_T));
    }
    else if (eStreamFrom == AUD_STREAM_FROM_SPDIF)
    {
        x_memcpy((VOID *)prAinCfg, (const VOID *)&_rSPDIFInCfg, sizeof(AIN_CFG_T));
    }
    else if (eStreamFrom == AUD_STREAM_FROM_HDMI)
    {
        x_memcpy((VOID *)prAinCfg, (const VOID *)&_rHDMIInCfg, sizeof(AIN_CFG_T));
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** AUD_AinInit
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AinInit(void)
{

}

//-----------------------------------------------------------------------------
/** AUD_AinGetMode
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AinGetMode(BOOL *pfgDeteced, AUD_FMT_T *peMode)
{
    UINT32 u4Reg = 0;
    //UINT8 bHDMIReg;

    if ((pfgDeteced == NULL) || (peMode == NULL))
    {
        return;
    }

    u4Reg = AUD_READ32(REG_SPDFTYP);
    //bHDMIReg = bHDMIReadDevH(0x2A); // HDMIH 0x2a [1] : SPDIF RAW(1) or PCM(0)
    *pfgDeteced = ((u4Reg & TYPE_DETECT) == 0) ? FALSE : TRUE;

    if (*pfgDeteced)
    {
        if ((u4Reg & TYPE_ROUGH) == TYPE_PCM)
        {
            *peMode = AUD_FMT_PCM;
            /*
            if ((bHDMIReg & 0x2) != 0)
            {
                // audio type from HDMI Rx is RAW
                // set as not detected
                *pfgDeteced = FALSE;
            }
            */
        }
        else if ((u4Reg & TYPE_DETAIL) == TYPE_AC3)
        {
            *peMode = AUD_FMT_AC3;
        }
        else if ((u4Reg & TYPE_DETAIL) == TYPE_MPEG1_L1)
        {
            *peMode = AUD_FMT_MPEG;
        }
        // Note: MP2/MP3 has the same type in IEC61937!
        else if ((u4Reg & TYPE_DETAIL) == TYPE_MPEG1_L2L3)
        {
            *peMode = AUD_FMT_MPEG;
        }
        else if ((u4Reg & TYPE_DETAIL) == TYPE_MPEG2_L1LSF)
        {
            *peMode = AUD_FMT_MPEG;
        }
        else if ((u4Reg & TYPE_DETAIL) == TYPE_MPEG2_L2LSF)
        {
            *peMode = AUD_FMT_MPEG;
        }
        else
        {
            *peMode = AUD_FMT_UNKNOWN;
            LOG(3, "SPDIF format not support\n");
        }
    }
}

void AUD_AinSetMode(UINT8 u1Mode)
{
    if (u1Mode == 0) // PCM
    {
#ifdef USE_16BIT_ALIGNMENT
        AUD_CLR_BIT_SPECIAL(REG_LIN_CFG0, DATA_PCM_24_BITS);
#else
        AUD_SET_BIT_SPECIAL(REG_LIN_CFG0, DATA_PCM_24_BITS);
#endif

    }
    else if (u1Mode == 1) // RAW
    {
        AUD_CLR_BIT_SPECIAL(REG_LIN_CFG0, DATA_PCM_24_BITS);
    }
}

//-----------------------------------------------------------------------------
/** AUD_AinSrcSel
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AinSrcSel(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eAudFmt)
{
    UINT32 u4Format1 = 0;
    UINT32 u4Format2 = 0;

    AUD_DEC_ID_VALIDATE(u1DecId);

    // Setup format
    u4Format1 = AUD_READ32_SPECIAL(REG_BS_CFG) & (~PTR_SEL_BANK0_MASK);
    u4Format2 = AUD_READ32_SPECIAL(REG_LIN_CFG0) & (~SPDIF_FMT_MASK);

    if ((eStreamFrom == AUD_STREAM_FROM_SPDIF) ||
            (eStreamFrom == AUD_STREAM_FROM_HDMI))
    {
        if (eAudFmt == AUD_FMT_PCM)
        {
            u4Format1 |= ( PTR_SEL_BANK0_SPDIF_LINE_IN);
#ifdef  USE_16BIT_ALIGNMENT
            u4Format2 |= (DATA_RAW_16_BITS |
                          DATA_NO_SWAP |
                          RISC_INT_PERIOD_256 |
                          INT_ADC_EXTLIN |
                          PCM_CH_NUM_2_CH);
#else
            u4Format2 |= (DATA_RAW_16_BITS |
                          DATA_NO_SWAP |
                          RISC_INT_PERIOD_256 |
                          INT_ADC_EXTLIN |
                          PCM_CH_NUM_2_CH);
#endif
        }
        else
        {
            u4Format1 |= ( PTR_SEL_BANK0_SPDIF_LINE_IN);
            u4Format2 |= (DATA_RAW_16_BITS |
                          DATA_NO_SWAP |
                          RISC_INT_PERIOD_256 |
                          INT_ADC_EXTLIN |
                          PCM_CH_NUM_2_CH);
        }

#ifndef CC_MT5387    //No SPDIF in in MT5387
        // Init SPDIF-in receiver
        SPDIF_InEnable(TRUE);
#endif

        AUD_WRITE32(REG_BS_CFG, u4Format1);
        AUD_WRITE32(REG_LIN_CFG0, u4Format2);
    }
    else if (eStreamFrom == AUD_STREAM_FROM_LINE_IN)
    {
        u4Format1 |= ( PTR_SEL_BANK0_SPDIF_LINE_IN);

#ifdef USE_16BIT_ALIGNMENT

        u4Format2 |= (DATA_RAW_16_BITS |
                      DATA_NO_SWAP |
                      RISC_INT_PERIOD_256 |
                      INT_ADC_INTLIN |
                      PCM_CH_NUM_2_CH/*FIXME*/);
#else
        u4Format2 |= (DATA_PCM_24_BITS |
                      DATA_NO_SWAP |
                      RISC_INT_PERIOD_256 |
                      INT_ADC_INTLIN |
                      PCM_CH_NUM_2_CH/*FIXME*/);
#endif

        AUD_WRITE32(REG_BS_CFG, u4Format1);
        AUD_WRITE32(REG_LIN_CFG0, u4Format2);
    }

    switch (eStreamFrom)
    {
        case AUD_STREAM_FROM_SPDIF:
            AUD_AinCfg(AUD_STREAM_FROM_SPDIF, (const AIN_CFG_T * )&_rSPDIFInCfg);
            //AUD_WRITE32(REG_SPLIN_CTL, DATA_RAW_16_BITS);
            break;
        case AUD_STREAM_FROM_LINE_IN:
            AUD_AinCfg(AUD_STREAM_FROM_LINE_IN, (const AIN_CFG_T * )&_rLineInCfg);
            break;
        case AUD_STREAM_FROM_HDMI:
            AUD_AinCfg(AUD_STREAM_FROM_HDMI, (const AIN_CFG_T * )&_rHDMIInCfg);
            break;
        default:
            ASSERT(0);
    }

}

//-----------------------------------------------------------------------------
/** AUD_AinEnable
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AinEnable(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, BOOL fgEnable)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Enable input source
    switch (eStreamFrom)
    {
        case AUD_STREAM_FROM_DIGITAL_TUNER:
        case AUD_STREAM_FROM_MULTI_MEDIA:
        case AUD_STREAM_FROM_MEMORY:
        case AUD_STREAM_FROM_OTHERS:
            break;

        case AUD_STREAM_FROM_SPDIF:
        case AUD_STREAM_FROM_HDMI:
        case AUD_STREAM_FROM_LINE_IN:
            if (fgEnable)
            {
                AUD_SET_BIT_SPECIAL(REG_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
            }
            else
            {
                AUD_CLR_BIT_SPECIAL(REG_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
            }
            break;

        default:
            break;
    }

    /*
        if (fgEnable)
        {
            AUD_SET_BIT_SPECIAL(REG_SPLIN_CTL, SPDIF_LINE_IN_ENABLE);
        }
        else
        {
            AUD_CLR_BIT_SPECIAL(REG_SPLIN_CTL, SPDIF_LINE_IN_ENABLE);
        }
    */

}

//-----------------------------------------------------------------------------
/** AUD_AinCfg
 *  Set value for audio input related configuration
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AinCfg(AUD_DEC_STREAM_FROM_T eStrSrc, const AIN_CFG_T* prAinCfg)
{
    UINT32 u4AUDCFG = 0;

    // Update local variables
    switch (eStrSrc)
    {
        case AUD_STREAM_FROM_LINE_IN:
            x_memcpy((VOID *)&_rLineInCfg, (const VOID *) prAinCfg, sizeof(AIN_CFG_T));
            break;
        case AUD_STREAM_FROM_HDMI:
            x_memcpy((VOID *)&_rHDMIInCfg, (const VOID *) prAinCfg, sizeof(AIN_CFG_T));
            break;
        case AUD_STREAM_FROM_SPDIF:
            x_memcpy((VOID *)&_rSPDIFInCfg, (const VOID *) prAinCfg, sizeof(AIN_CFG_T));
            break;
        default:
            LOG(1, "ERROR AUD_AinCfg\n");
            return;
    }

    // Save original audio output setting
    u4AUDCFG = AUD_READ32(REG_EXTADC_CFG) & (~AUD_IN_CFG_MASK);

    if (prAinCfg != NULL)
    {
        switch (prAinCfg->eFormat)
        {
            case FORMAT_I2S:
                u4AUDCFG |= SPLIN_I2S;
                break;
            case FORMAT_LJ:
                u4AUDCFG |= SPLIN_LJ;
                break;
            case FORMAT_RJ:
                u4AUDCFG |= SPLIN_RJ;
                break;
            default:
                break;
        }

        switch (prAinCfg->eBits)
        {
            case DAC_16_BIT:
                u4AUDCFG |= ADC_16BIT;
                break;
            case DAC_18_BIT:
                u4AUDCFG |= ADC_18BIT;
                break;
            case DAC_20_BIT:
                u4AUDCFG |= ADC_20BIT;
                break;
            case DAC_24_BIT:
                u4AUDCFG |= ADC_24BIT;
                break;
            default:
                break;
        }

        switch (prAinCfg->eCycle)
        {
            case LRCK_CYC_16:
                u4AUDCFG |= SPDIF_CYC16;
                break;
            case LRCK_CYC_24:
                u4AUDCFG |= SPDIF_CYC24;
                break;
            case LRCK_CYC_32:
                u4AUDCFG |= SPDIF_CYC32;
                break;
            default:
                break;
        }

        if (prAinCfg->fgLRInvert)
        {
            u4AUDCFG |= SPLIN_LR_INVT;
        }

        AUD_WRITE32(REG_EXTADC_CFG, u4AUDCFG);
        AUD_CLR_BIT(REG_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
        AUD_SET_BIT(REG_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
    }
    else
    {
        ASSERT(0);
    }
}

void AUD_AinFormat(AUD_DEC_STREAM_FROM_T eStrSrc, DATA_FORMAT_T eDataFormat)
{
    AIN_CFG_T * prAinCfg = NULL;

    switch (eStrSrc)
    {
        case AUD_STREAM_FROM_LINE_IN:
            prAinCfg = &_rLineInCfg;
            break;
        case AUD_STREAM_FROM_HDMI:
            prAinCfg = &_rHDMIInCfg;
            break;
        case AUD_STREAM_FROM_SPDIF:
            prAinCfg = &_rSPDIFInCfg;
            break;
        default:
            LOG(1, "ERROR AUD_AinCfg\n");
            prAinCfg = NULL;
    }

    if (prAinCfg != NULL)
    {
        if (prAinCfg->eFormat != eDataFormat)
        {
            prAinCfg->eFormat = eDataFormat;
            AUD_AinCfg(eStrSrc, prAinCfg);
        }
    }
}

//-----------------------------------------------------------------------------
/** AUD_AoutDrvCur
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutDrvCur(UINT8 u1DecId, UINT8 u1MckDrvCur, UINT8 u1BckDrvCur , UINT8 u1LRckDrvCur, UINT8 u1AOSDATADrvCur)
{
    UINT32 u4DrvCur = 0;
    UINT32 u4PADDRVCFG0 = 0;

    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));

    switch (u1MckDrvCur)
    {
        case 0:
        case 1:
        case 2:
            u4DrvCur = PADDRV_2MA;
            _au1MckDrvCur[u1DecId] = 2;
            break;
        case 3:
        case 4:
            u4DrvCur = PADDRV_4MA;
            _au1MckDrvCur[u1DecId] = 4;
            break;
        case 5:
        case 6:
            u4DrvCur = PADDRV_6MA;
            _au1MckDrvCur[u1DecId] = 6;
            break;
        case 7:
        case 8:
            u4DrvCur = PADDRV_8MA;
            _au1MckDrvCur[u1DecId] = 8;
            break;
        default:
            u4DrvCur = PADDRV_8MA;
            _au1MckDrvCur[u1DecId] = 8;
    }
    u4PADDRVCFG0 |= (u4DrvCur << AOMCLK_SHFT);

    switch (u1BckDrvCur)
    {
        case 0:
        case 1:
        case 2:
            u4DrvCur = PADDRV_2MA;
            _au1BckDrvCur[u1DecId] = 2;
            break;
        case 3:
        case 4:
            u4DrvCur = PADDRV_4MA;
            _au1BckDrvCur[u1DecId] = 4;
            break;
        case 5:
        case 6:
            u4DrvCur = PADDRV_6MA;
            _au1BckDrvCur[u1DecId] = 6;
            break;
        case 7:
        case 8:
            u4DrvCur = PADDRV_8MA;
            _au1BckDrvCur[u1DecId] = 8;
            break;
        default:
            u4DrvCur = PADDRV_8MA;
            _au1BckDrvCur[u1DecId] = 8;
    }
    u4PADDRVCFG0 |= (u4DrvCur << AOBCK_SHFT);

    switch (u1LRckDrvCur)
    {
        case 0:
        case 1:
        case 2:
            u4DrvCur = PADDRV_2MA;
            _au1LRckDrvCur[u1DecId] = 2;
            break;
        case 3:
        case 4:
            u4DrvCur = PADDRV_4MA;
            _au1LRckDrvCur[u1DecId] = 4;
            break;
        case 5:
        case 6:
            u4DrvCur = PADDRV_6MA;
            _au1LRckDrvCur[u1DecId] = 6;
            break;
        case 7:
        case 8:
            u4DrvCur = PADDRV_8MA;
            _au1LRckDrvCur[u1DecId] = 8;
            break;
        default:
            u4DrvCur = PADDRV_8MA;
            _au1LRckDrvCur[u1DecId] = 8;
    }
    u4PADDRVCFG0 |= (u4DrvCur << AOLRCK_SHFT);

    switch (u1AOSDATADrvCur)
    {
        case 0:
        case 1:
        case 2:
            u4DrvCur = PADDRV_2MA;
            _au1AOSDATADrvCur[u1DecId] = 2;
            break;
        case 3:
        case 4:
            u4DrvCur = PADDRV_4MA;
            _au1AOSDATADrvCur[u1DecId] = 4;
            break;
        case 5:
        case 6:
            u4DrvCur = PADDRV_6MA;
            _au1AOSDATADrvCur[u1DecId] = 6;
            break;
        case 7:
        case 8:
            u4DrvCur = PADDRV_8MA;
            _au1AOSDATADrvCur[u1DecId] = 8;
            break;
        default:
            u4DrvCur = PADDRV_8MA;
            _au1AOSDATADrvCur[u1DecId] = 8;
    }
    u4PADDRVCFG0 |= (u4DrvCur << AOSD_SHFT);


    // Light: Set SPDIF output PAD driving current to 8mA
    u4DrvCur = PADDRV_8MA;
    u4PADDRVCFG0 |= (u4DrvCur << ASPDF_SHFT);

#if defined(CC_MT8223)
#elif defined(CC_MT5387)
    vIO32WriteFldAlign(CKGEN_PADDRVCFG, u4PADDRVCFG0, FLD_PADDRV);
#else
    CKGEN_WRITE32(REG_PADDRV_CFG0, ((CKGEN_READ32(REG_PADDRV_CFG0) & (0x003fffff)) | u4PADDRVCFG0));
#endif
}

//-----------------------------------------------------------------------------
/** AUD_AoutCfg
 *  Audio output configuration
 *  1. Setup Aout registers
 *  2. Setup ADAC
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutCfg(UINT8 u1DecId, const AOUT_CFG_T* prAoutCfg)
{
    UINT32 u4AUDCFG = 0;
    UINT32 u4AUDCFG2 = 0;
    UINT32 u4AUDCFG3 = 0;

    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));

    // Update configuration to local variable
    if (prAoutCfg != NULL)
    {
        x_memcpy((VOID *)&_arAudOutCfg[u1DecId], (const VOID *) prAoutCfg, sizeof(AOUT_CFG_T));
    }

    if (prAoutCfg != NULL)
    {
        switch (prAoutCfg->eFormat)
        {
            case FORMAT_RJ:
                u4AUDCFG |= AOFMT_RJ;
                break;
            case FORMAT_LJ:
                u4AUDCFG |= AOFMT_LJ;
                break;
            case FORMAT_I2S:
                u4AUDCFG |= AOFMT_I2S;
                break;
            default:
                u4AUDCFG |= AOFMT_I2S;
        }

        switch (prAoutCfg->eSampleFreq)
        {
            case MCLK_128FS:
                u4AUDCFG2 |= (BCK_DIV_2 );
                u4AUDCFG3 |= (DEC2_BCK_DIV_2 );
                break;
            case MCLK_256FS:
                u4AUDCFG2 |= (BCK_DIV_4 );
                u4AUDCFG3 |= (DEC2_BCK_DIV_4 );
                break;
            case MCLK_384FS:
                u4AUDCFG2 |= (BCK_DIV_6 );
                u4AUDCFG3 |= (DEC2_BCK_DIV_6);
                break;
            case MCLK_512FS:
                u4AUDCFG2 |= (BCK_DIV_8 );
                u4AUDCFG3 |= (DEC2_BCK_DIV_8);
                break;
            default:
                u4AUDCFG2 |= (BCK_DIV_4);
                u4AUDCFG3 |= (DEC2_BCK_DIV_4 );
                LOG( 3, "Sampling Freq not support\n");
        }

        switch (prAoutCfg->eCycle)
        {
            case LRCK_CYC_16:
                u4AUDCFG |= LRCK_CYC16;
                break;
            case LRCK_CYC_24:
                u4AUDCFG |= LRCK_CYC24;
                break;
            case LRCK_CYC_32:
                u4AUDCFG |= LRCK_CYC32;
                break;
            default:
                u4AUDCFG |= LRCK_CYC24;
        }

        switch (prAoutCfg->eBits)
        {
            case DAC_16_BIT:
                u4AUDCFG |= DAC_16BIT;
                break;
            case DAC_18_BIT:
                u4AUDCFG |= DAC_18BIT;
                break;
            case DAC_20_BIT:
                u4AUDCFG |= DAC_20BIT;
                break;
            case DAC_24_BIT:
                u4AUDCFG |= DAC_24BIT;
                break;
            default:
                u4AUDCFG |= DAC_24BIT;
        }

        // If invert the Sdata output
        if (prAoutCfg->fgDataInvert)
        {
            u4AUDCFG |= INV_SDATA;
        }

        // Light: For I2S format the INV_LRCK is not set in normal mode.
        // Light: For I2S format the INV_LRCK is set in swap mode.
        // Light: For LJ and RJ formats the INV_LRCK is set in normal mode.
        // Light: For LJ and RJ formats the INV_LRCK is not set in swap mode.
        if ((prAoutCfg->eFormat == FORMAT_RJ) || (prAoutCfg->eFormat == FORMAT_LJ))
        {
            if (!prAoutCfg->fgLRInvert)
            {
                u4AUDCFG |= INV_LRCK;
            }
        }
        else // For I2S format and default
        {
            // If invert L/R output
            if (prAoutCfg->fgLRInvert)
            {
                u4AUDCFG |= INV_LRCK;
            }
        }

        // setting driving current
        //u4AUDCFG |= IEC_CUR_4;  ???
        u4AUDCFG |= INV_BCK;
        AUD_AoutDrvCur(u1DecId, _au1MckDrvCur[u1DecId], _au1BckDrvCur[u1DecId], _au1LRckDrvCur[u1DecId] , _au1AOSDATADrvCur[u1DecId]);

    }

    if (u1DecId == AUD_DEC_MAIN)
    {
        AUD_WRITE32(REG_EXTDAC_CFG0, (AUD_READ32(REG_EXTDAC_CFG0) & (~AUD_OUT_CFG_MASK)) | u4AUDCFG);
        AUD_WRITE32(REG_ACK_CFG, (AUD_READ32(REG_ACK_CFG) & ~(BCK_DIV_MASK)) | u4AUDCFG2);
        AUD_WRITE32(REG_ACK_CFG, (AUD_READ32(REG_ACK_CFG) & ~(IEC_DIV_MASK)) | IEC_DIV_2);
    }
    else // AUD_DEC_AUX
    {
        AUD_WRITE32(REG_EXTDAC_CFG1, (AUD_READ32(REG_EXTDAC_CFG1) & (~AUD_OUT_CFG_MASK)) | u4AUDCFG);
        AUD_WRITE32(REG_ACK_CFG, (AUD_READ32(REG_ACK_CFG) & ~(DEC2_BCK_DIV_MASK)) | u4AUDCFG3);
        AUD_WRITE32(REG_ACK_CFG, (AUD_READ32(REG_ACK_CFG) & ~(DEC2_IEC_DIV_MASK)) | DEC2_IEC_DIV_2);
    }

    // Update ADAC
    if (prAoutCfg != NULL)
    {
        ADAC_DacFormatCfg(u1DecId, prAoutCfg->eFormat, prAoutCfg->eSampleFreq);
    }
}

//-----------------------------------------------------------------------------
/** AUD_AoutFormat
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutFormat(UINT8 u1DecId, DATA_FORMAT_T eDataFormat)
{
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));

    if (_arAudOutCfg[u1DecId].eFormat != eDataFormat)
    {
        _arAudOutCfg[u1DecId].eFormat = eDataFormat;
        AUD_AoutCfg(u1DecId, &_arAudOutCfg[u1DecId]);
    }
}

//-----------------------------------------------------------------------------
/** AUD_AoutFormat
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutMclk(UINT8 u1DecId, MCLK_FREQUENCY_T eMclk)
{
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));

    if (_arAudOutCfg[u1DecId].eSampleFreq != eMclk)
    {
        _arAudOutCfg[u1DecId].eSampleFreq = eMclk;
        AUD_AoutCfg(u1DecId, &_arAudOutCfg[u1DecId]);
    }
}

//-----------------------------------------------------------------------------
/** AUD_AoutDacFs
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutDacFs(UINT8 u1DecId, SAMPLE_FREQ_T eFs)
{
    UNUSED(u1DecId);
    UNUSED(eFs);
}

//-----------------------------------------------------------------------------
/** AUD_AoutInvertData
 *  Invert the Sdata output for OP phase
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutInvertData(UINT8 u1DecId, BOOL fgInvert)
{
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));

    if (_arAudOutCfg[u1DecId].fgDataInvert != fgInvert)
    {
        _arAudOutCfg[u1DecId].fgDataInvert = fgInvert;
        AUD_AoutCfg(u1DecId, &_arAudOutCfg[u1DecId]);
    }
}

//-----------------------------------------------------------------------------
/** AUD_AoutDataNum
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutDataNum(UINT8 u1DecId, DAC_DATA_NUMBER_T eDataNum)
{
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));

    if (_arAudOutCfg[u1DecId].eBits != eDataNum)
    {
        _arAudOutCfg[u1DecId].eBits = eDataNum;
        AUD_AoutCfg(u1DecId, &_arAudOutCfg[u1DecId]);
    }
}

//-----------------------------------------------------------------------------
/** AUD_AoutLRCycle
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutLRCycle(UINT8 u1DecId, LRCK_CYC_T eCycle)
{
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));

    if (_arAudOutCfg[u1DecId].eCycle != eCycle)
    {
        _arAudOutCfg[u1DecId].eCycle = eCycle;
        AUD_AoutCfg(u1DecId, &_arAudOutCfg[u1DecId]);
    }
}

//-----------------------------------------------------------------------------
/** AUD_AoutPADMux
 *
 *  @param  PAD_SDATA0 (AOUTDATA0_MAIN ~ AOUTDATA4_MAIN, AOUTDATA0_AUX),
 *          PAD_SDATA1, ..., PAD_SDATA5,
 *          PAD_MCLK0 (MCLK0 / MCLK1)
 *          Notice that PAD_MCLK1 (MCLK0 /MCLK1) is decided by PAD_SDATA5
 *
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutPADMux(AUD_CH_T ePAD_SDATA0, AUD_CH_T ePAD_SDATA1, AUD_CH_T ePAD_SDATA2,
                    AUD_CH_T ePAD_SDATA3, AUD_CH_T ePAD_SDATA4, AUD_CH_T ePAD_SDATA5,
                    UINT8 PAD_MCLK0)
{
    UINT32 u4ADSP_ENV_BAK, u4AENV_CFG;
    BOOL fgAi2suseforspeaker = FALSE;

    UNUSED(fgAi2suseforspeaker);

    u4ADSP_ENV_BAK = AUD_READ32(REG_CH_CFG);		// laihui
    u4AENV_CFG = AUD_READ32(REG_CH_CFG);

    u4ADSP_ENV_BAK = u4ADSP_ENV_BAK & (~SDATA_MCLK_SYNC) & (~CH_SEL);
    u4AENV_CFG = u4AENV_CFG & (~SDATA_MCLK_SYNC) & (~CH_SEL);

    switch (((UINT32)ePAD_SDATA0) / 2)
    {
        case AOUTDATA0_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH0_AOUTDATA0;
            break;
        case AOUTDATA1_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH0_AOUTDATA1 ;
            break;
        case AOUTDATA2_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH0_AOUTDATA2 ;
            break;
        case AOUTDATA3_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH0_AOUTDATA3;
            break;
        case AOUTDATA4_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH0_AOUTDATA4;
            break;
        case AOUTDATA0_AUX:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | DEC2_CH0_AOUTDATA0;
            u4AENV_CFG = u4AENV_CFG | AOUTDATA0_AUX_TO_SDATA0;
            break;
        default:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH0_AOUTDATA0 ;
            break;
    }
    switch (((UINT32)ePAD_SDATA1) / 2)
    {
        case AOUTDATA0_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH1_AOUTDATA0;
            break;
        case AOUTDATA1_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH1_AOUTDATA1;
            break;
        case AOUTDATA2_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH1_AOUTDATA2;
            break;
        case AOUTDATA3_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH1_AOUTDATA3;
            break;
        case AOUTDATA4_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH1_AOUTDATA4;
            break;
        case AOUTDATA0_AUX:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | DEC2_CH1_AOUTDATA0;
            u4AENV_CFG = u4AENV_CFG | AOUTDATA0_AUX_TO_SDATA1;
            break;
        default:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH1_AOUTDATA1;
            break;
    }
    switch (((UINT32)ePAD_SDATA2) / 2)
    {
        case AOUTDATA0_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH2_AOUTDATA0;
            break;
        case AOUTDATA1_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH2_AOUTDATA1;
            break;
        case AOUTDATA2_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH2_AOUTDATA2;
            break;
        case AOUTDATA3_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH2_AOUTDATA3;
            break;
        case AOUTDATA4_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH2_AOUTDATA4;
            break;
        case AOUTDATA0_AUX:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH2_AOUTDATA0;
            u4AENV_CFG = u4AENV_CFG | AOUTDATA0_AUX_TO_SDATA2;
            break;
        default:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH2_AOUTDATA2;
            break;
    }
    switch (((UINT32)ePAD_SDATA3) / 2)
    {
        case AOUTDATA0_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH3_AOUTDATA0;
            break;
        case AOUTDATA1_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH3_AOUTDATA1;
            break;
        case AOUTDATA2_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH3_AOUTDATA2;
            break;
        case AOUTDATA3_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH3_AOUTDATA3;
            break;
        case AOUTDATA4_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH3_AOUTDATA4;
            break;
        case AOUTDATA0_AUX:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH3_AOUTDATA0;
            u4AENV_CFG = u4AENV_CFG | AOUTDATA0_AUX_TO_SDATA3;
            break;
        default:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH3_AOUTDATA3;
            break;
    }
    switch (((UINT32)ePAD_SDATA4) / 2)
    {
        case AOUTDATA0_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH4_AOUTDATA0;
            break;
        case AOUTDATA1_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH4_AOUTDATA1;
            break;
        case AOUTDATA2_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH4_AOUTDATA2;
            break;
        case AOUTDATA3_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH4_AOUTDATA3;
            break;
        case AOUTDATA4_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH4_AOUTDATA4;
            break;
        case AOUTDATA0_AUX:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH4_AOUTDATA0;
            u4AENV_CFG = u4AENV_CFG | AOUTDATA0_AUX_TO_SDATA4;
            break;
        default:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | CH4_AOUTDATA4;
            break;
    }
    switch (((UINT32)ePAD_SDATA5) / 2)
    {
        case AOUTDATA0_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | DEC2_CH0_AOUTDATA0;
            u4AENV_CFG = u4AENV_CFG | AOUTDATA_MCLK_MAIN_TO_SDATA5;
            break;
        case AOUTDATA1_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | DEC2_CH0_AOUTDATA1;
            u4AENV_CFG = u4AENV_CFG | AOUTDATA_MCLK_MAIN_TO_SDATA5;
            break;
        case AOUTDATA2_MAIN:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | DEC2_CH0_AOUTDATA2;
            u4AENV_CFG = u4AENV_CFG | AOUTDATA_MCLK_MAIN_TO_SDATA5;
            break;
            /*      case AOUTDATA3_MAIN:
                        u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | SDATA5_AOUTDATA3;
                        u4AENV_CFG = u4AENV_CFG | AOUTDATA_MCLK_MAIN_TO_SDATA5;
               	        break;
                    case AOUTDATA4_MAIN:
                        u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | SDATA5_AOUTDATA4;
                        u4AENV_CFG = u4AENV_CFG | AOUTDATA_MCLK_MAIN_TO_SDATA5;
               	        break;
            */
        case AOUTDATA0_AUX:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | DEC2_CH0_AOUTDATA0;
            break;
        default:
            u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | DEC2_CH0_AOUTDATA0;
            break;
    }
    //  laihui  ????????
    /*
        if (PAD_MCLK0==MCLK1)
        {
            u4AENV_CFG = u4AENV_CFG | MCLK_AUX_TO_MCLK_MAIN;
        }
    */

#ifdef CC_AUD_EU_INTERNALDAC_SAME_APLL
    u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | BCK_LRCK_SEL;
#endif
    u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | u4AENV_CFG;

    AUD_WRITE32(REG_CH_CFG, u4ADSP_ENV_BAK);

    //AUD_WRITE32(REG_CH_CFG, u4AENV_CFG);     // (Jessica)
    UNUSED (PAD_MCLK0);
}

//-----------------------------------------------------------------------------
/** AUD_DspClkSel
 *  ADSP Clk cource selection
 *
 *  @param  u1ADSPclkSel    0   xtal_clk,     \n
 *                          1   syspll_d2   432/2 (162 MHz/216 MHz)   \n
 *                          2   tvdpll_d3             (180 MHz)   \n
 *                          3   dtdpll_d3             (200 MHz)   \n
 *                          4   dtdpll_d4             (150 MHz)   \n
 *                          5   pspll_d1              (148.5 MHz) \n
 *                          6   mem_ck
 *                          7~13  syspll_d2_ck
 *				14  slow_ck
 *				15  bist_ck
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspClkSel(UINT8 u1ADSPclkSel)
{

    vIO32WriteFldAlign(CKGEN_DSP_CKCFG,  (UINT32)u1ADSPclkSel, FLD_DSP_CK_SEL);
}

//-----------------------------------------------------------------------------
/** AUD_SetApllModin
 *  Set APLL MODIN
 *
 *  @param  u4Level    video calculated level (0 ~ 255. 128 as center)
 *                     bit 8: 0 (slower), 1 (faster)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_SetApllModin(UINT32 u4Level)
{
    UINT32 u4ApllRef, u4ApllDiff, u4ApllLevel;
    UINT8 u1Byte;

    if (AUD_GetTargetCountry() == COUNTRY_EU)
    {
        if ((_aeStrSrcConf[AUD_DEC_AUX] != AUD_STREAM_FROM_DIGITAL_TUNER) && (_aeStrSrcConf[AUD_DEC_AUX] != AUD_STREAM_FROM_MULTI_MEDIA) )
        {
            return;
        }
    }
    else
    {
        if ((_aeStrSrcConf[AUD_DEC_MAIN] != AUD_STREAM_FROM_DIGITAL_TUNER) && (_aeStrSrcConf[AUD_DEC_AUX] != AUD_STREAM_FROM_MULTI_MEDIA))
        {
            return;
        }
    }

    u1Byte = (UINT8)(u4Level & 0x0ff);
    // Need to add 294/270 group check
#ifndef CC_MT5391_AUD_3_DECODER
    if (_au4ApllClk[0] == CLK_APLL294MHZ)
    {
        // Use 294.912MHz
#ifdef CC_MT5391_AUD_SUPPORT
        u4ApllRef = (UINT32)APLL_MODIN_REF1;
        u4ApllDiff = (UINT32)APLL_294_STEP_FACTOR * u1Byte;
#else
        if (IS_SYSPLL_432())
        {
            u4ApllRef = (UINT32)APLL_MODIN_432MHZ_SYSCLK_294MHZ_APLL;
            u4ApllDiff = (UINT32)APLL_432MHZ_SYSCLK_294_STEP * u1Byte;
        }
        else
        {
            u4ApllRef = (UINT32)APLL_MODIN_324MHZ_SYSCLK_294MHZ_APLL;
            u4ApllDiff = (UINT32)APLL_324MHZ_SYSCLK_294_STEP * u1Byte;
        }
#endif
        //LOG(9, "PLL check VCXO in APLL0: 294MHz\n");
    }
    else
    {
        // Use 270.9504MHz
#ifdef CC_MT5391_AUD_SUPPORT
        u4ApllRef = (UINT32)APLL_MODIN_REF2;
        u4ApllDiff = (UINT32)APLL_270_STEP_FACTOR * u1Byte;
#else
        if (IS_SYSPLL_432())
        {
            u4ApllRef = (UINT32)APLL_MODIN_432MHZ_SYSCLK_270MHZ_APLL;
            u4ApllDiff = (UINT32)APLL_432MHZ_SYSCLK_270_STEP * u1Byte;
        }
        else
        {
            u4ApllRef = (UINT32)APLL_MODIN_324MHZ_SYSCLK_270MHZ_APLL;
            u4ApllDiff = (UINT32)APLL_324MHZ_SYSCLK_270_STEP * u1Byte;
        }
#endif
        //LOG(9, "PLL check VCXO in APLL0: 270MHz\n");
    }
#else
    if (AUD_GetTargetCountry() != COUNTRY_EU)
    {
        if (_au4ApllClk[0] == CLK_APLL294MHZ)
        {
            // Use 294.912MHz
#ifdef CC_MT5391_AUD_SUPPORT
            u4ApllRef = (UINT32)APLL_MODIN_REF1;
            u4ApllDiff = (UINT32)APLL_294_STEP_FACTOR * u1Byte;
#else
            if (IS_SYSPLL_432())
            {
                u4ApllRef = (UINT32)APLL_MODIN_432MHZ_SYSCLK_294MHZ_APLL;
                u4ApllDiff = (UINT32)APLL_432MHZ_SYSCLK_294_STEP * u1Byte;
            }
            else
            {
                u4ApllRef = (UINT32)APLL_MODIN_324MHZ_SYSCLK_294MHZ_APLL;
                u4ApllDiff = (UINT32)APLL_324MHZ_SYSCLK_294_STEP * u1Byte;
            }
#endif
            //LOG(9, "PLL check VCXO in APLL0: 294MHz in 3 decoder\n");
        }
        else
        {
            // Use 270.9504MHz
#ifdef CC_MT5391_AUD_SUPPORT
            u4ApllRef = (UINT32)APLL_MODIN_REF2;
            u4ApllDiff = (UINT32)APLL_270_STEP_FACTOR * u1Byte;
#else
            if (IS_SYSPLL_432())
            {
                u4ApllRef = (UINT32)APLL_MODIN_432MHZ_SYSCLK_270MHZ_APLL;
                u4ApllDiff = (UINT32)APLL_432MHZ_SYSCLK_270_STEP * u1Byte;
            }
            else
            {
                u4ApllRef = (UINT32)APLL_MODIN_324MHZ_SYSCLK_270MHZ_APLL;
                u4ApllDiff = (UINT32)APLL_324MHZ_SYSCLK_270_STEP * u1Byte;
            }
#endif
            //LOG(9, "PLL check VCXO in APLL0: 270MHz in 3 decoder\n");
        }
    }
    else
    {
        if (_au4ApllClk[1] == CLK_APLL294MHZ)
        {
#ifdef CC_MT5391_AUD_SUPPORT
            u4ApllRef = (UINT32)APLL_MODIN_REF1;
            u4ApllDiff = (UINT32)APLL_294_STEP_FACTOR * u1Byte;
#else
            // Use 294.912MHz
            if (IS_SYSPLL_432())
            {
                u4ApllRef = (UINT32)APLL_MODIN_432MHZ_SYSCLK_294MHZ_APLL;
                u4ApllDiff = (UINT32)APLL_432MHZ_SYSCLK_294_STEP * u1Byte;
            }
            else
            {
                u4ApllRef = (UINT32)APLL_MODIN_324MHZ_SYSCLK_294MHZ_APLL;
                u4ApllDiff = (UINT32)APLL_324MHZ_SYSCLK_294_STEP * u1Byte;
            }
#endif
            //LOG(9, "PLL check VCXO in APLL1: 294MHz in 3 decoder\n");
        }
        else
        {
            // Use 270.9504MHz
#ifdef CC_MT5391_AUD_SUPPORT
            u4ApllRef = (UINT32)APLL_MODIN_REF2;
            u4ApllDiff = (UINT32)APLL_270_STEP_FACTOR * u1Byte;
#else
            if (IS_SYSPLL_432())
            {
                u4ApllRef = (UINT32)APLL_MODIN_432MHZ_SYSCLK_270MHZ_APLL;
                u4ApllDiff = (UINT32)APLL_432MHZ_SYSCLK_270_STEP * u1Byte;
            }
            else
            {
                u4ApllRef = (UINT32)APLL_MODIN_324MHZ_SYSCLK_270MHZ_APLL;
                u4ApllDiff = (UINT32)APLL_324MHZ_SYSCLK_270_STEP * u1Byte;
            }
#endif
            //LOG(9, "PLL check VCXO in APLL1: 270MHz in 3 decoder\n");
        }
    }
#endif

    u1Byte = (UINT8)((u4Level >> 8) & 0x01);
    if (u1Byte == 1)
    {
        // Adjust APLL faster
        u4ApllLevel = u4ApllRef + u4ApllDiff;
    }
    else
    {
        // Adjust APLL slower
        u4ApllLevel = u4ApllRef - u4ApllDiff;
    }

#ifdef CC_MT5391_AUD_SUPPORT
    if (AUD_GetTargetCountry() != COUNTRY_EU)
    {
        // Use APLL1
        u1Byte = (UINT8)(u4ApllLevel & 0x0ff);
        _MT8297_DirectWRITE(REG_APLL0_CFG4, u1Byte, REG_APLL_MOD_MSK);
        u1Byte = (UINT8)((u4ApllLevel >> 8) & 0x0ff);
        _MT8297_DirectWRITE(REG_APLL0_CFG5, u1Byte, REG_APLL_MOD_MSK);
        u1Byte = (UINT8)((u4ApllLevel >> 16) & 0x0ff);
        _MT8297_DirectWRITE(REG_APLL0_CFG6, u1Byte, REG_APLL_MOD_MSK);
    }
    else
    {
#ifndef CC_MT5391_AUD_3_DECODER
        // Use APLL1
        u1Byte = (UINT8)(u4ApllLevel & 0x0ff);
        _MT8297_DirectWRITE(REG_APLL0_CFG4, u1Byte, REG_APLL_MOD_MSK);
        u1Byte = (UINT8)((u4ApllLevel >> 8) & 0x0ff);
        _MT8297_DirectWRITE(REG_APLL0_CFG5, u1Byte, REG_APLL_MOD_MSK);
        u1Byte = (UINT8)((u4ApllLevel >> 16) & 0x0ff);
        _MT8297_DirectWRITE(REG_APLL0_CFG6, u1Byte, REG_APLL_MOD_MSK);
        //#endif
#else
        // Use APLL2
        u1Byte = (UINT8)(u4ApllLevel & 0x0ff);
        _MT8297_DirectWRITE(REG_APLL1_CFG4, u1Byte, REG_APLL_MOD_MSK);
        u1Byte = (UINT8)((u4ApllLevel >> 8) & 0x0ff);
        _MT8297_DirectWRITE(REG_APLL1_CFG5, u1Byte, REG_APLL_MOD_MSK);
        u1Byte = (UINT8)((u4ApllLevel >> 16) & 0x0ff);
        _MT8297_DirectWRITE(REG_APLL1_CFG6, u1Byte, REG_APLL_MOD_MSK);
        //#endif
#endif
    }
#elif defined (CC_MT5387)
    // MT5387
    u4ApllLevel = u4ApllLevel & RG_APLL1_MODIN;

    if (AUD_GetTargetCountry() != COUNTRY_EU)
    {
        // Use APLL1
        vIO32WriteFldAlign(CKGEN_AFECFG1, u4ApllLevel, FLD_RG_APLL1_MODIN);
    }
    else
    {
        // Use APLL2
        vIO32WriteFldAlign(CKGEN_AFECFG2, u4ApllLevel, FLD_RG_APLL2_MODIN);
    }
#else
    // MT5360
    u4ApllLevel = u4ApllLevel & RG_APLL1_MODIN;

    if (AUD_GetTargetCountry() != COUNTRY_EU)
    {
        // Use APLL1
        CKGEN_WRITE32(REG_APLL1_CFG0, (CKGEN_READ32(REG_APLL1_CFG0) & ~RG_APLL1_MODIN) | u4ApllLevel);
    }
    else
    {
        // Use APLL2
        CKGEN_WRITE32(REG_APLL2_CFG0, (CKGEN_READ32(REG_APLL2_CFG0) & ~RG_APLL2_MODIN) | u4ApllLevel);
    }
#endif
}

//-----------------------------------------------------------------------------
/** AUD_DspClkEnable
 *  ADSP Power Down
 *
 *  @param  fgEnable   Enable.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspClkEnable(BOOL fgEnable)
{
    if (fgEnable)
    {
        CKGEN_CLR_BIT(REG_DSP_CLK_SEL, PDN_DSP);
    }
    else
    {
        CKGEN_SET_BIT(REG_DSP_CLK_SEL, PDN_DSP);
    }
}

//-----------------------------------------------------------------------------
/** AUD_SpdifInPinSelect
 *  Set SPDIF (spdif-in 1 and spdif-in 2 use the same config)
 *
 *  @param  fgSet   TRUE: set as input, FALSE: set as GPIO
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_SpdifInPinSelect(BOOL fgSet)
{

    UNUSED(fgSet);
    /*
        UINT32 u4PadCfg0;


        u4PadCfg0 = CKGEN_READ32(REG_PAD_CFG_0) & (~((UINT32)(PAD_SPDIF_SEL)));
        if (fgSet)
        {
            u4PadCfg0 |= PAD_SPDIF;
        }
        CKGEN_WRITE32(REG_PAD_CFG_0, u4PadCfg0);
    */
    // TODO
    // for line-in pad config
}


//-----------------------------------------------------------------------------
/** AUD_LineInCtrlEnable
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
//#include "x_timer.h"
//static HAL_TIME_T _rTime;
void AUD_LineInCtrlEnable(UINT8 u1DecId, BOOL fgEnable)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    if (fgEnable)
    {   // Turn on line-in buffer control
        AUD_SET_BIT_SPECIAL(REG_LIN_CFG0, SPDIF_LINE_IN_ENABLE);			//laihui
    }
    else
    {   // Turn off line-in buffer control
        AUD_CLR_BIT_SPECIAL(REG_LIN_CFG0, SPDIF_LINE_IN_ENABLE);		//laihui
    }

    if (fgEnable)
    {
        LOG(5, "LineInCtrl start capturing\n");
        // HAL_GetTime(&_rTime);
        // Printf("LineInCtrl start capturing (%d.%06d)\n", _rTime.u4Seconds, _rTime.u4Micros);
    }
    else
    {
        LOG(5, "LineInCtrl stop capturing\n");
        // HAL_GetTime(&_rTime);
        // Printf("LineInCtrl stop capturing (%d.%06d)\n", _rTime.u4Seconds, _rTime.u4Micros);
    }
}

//-----------------------------------------------------------------------------
/** AUD_OutPadEnable
 *  Enable/disable audio output pad
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  fgEnable  1: enable (Set MCLK/LRCK/BCLK/DATA pad as output)   \n
 *                    0: diable (Set MCLK/LRCK/BCLK/DATA pad as tiastate)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
#ifndef CC_MT5391_AUD_3_DECODER
void AUD_OutPadEnable(UINT8 u1DecId, BOOL fgEnable)
{
    if (fgEnable)
    {
        AUD_SET_BIT(REG_ACK_CFG, (u1DecId == AUD_DEC_MAIN) ? DEC0_MTRI : DEC1_MTRI);
    }
    else
    {
        AUD_CLR_BIT(REG_ACK_CFG, (u1DecId == AUD_DEC_MAIN) ? DEC0_MTRI : DEC1_MTRI);
    }
}
#else
void AUD_OutPadEnable(UINT8 u1DecId, BOOL fgEnable)
{
    if (fgEnable)
    {
        if (u1DecId == AUD_DEC_MAIN) AUD_SET_BIT(REG_ACK_CFG, DEC0_MTRI);
        if (u1DecId == AUD_DEC_AUX) AUD_SET_BIT(REG_ACK_CFG, DEC1_MTRI);
        LOG(5, "AUD_OutPadEnable u1DecId = %d\n", u1DecId);
    }
    else
    {
        if (u1DecId == AUD_DEC_MAIN) AUD_CLR_BIT(REG_ACK_CFG, DEC0_MTRI);
        if (u1DecId == AUD_DEC_AUX) AUD_CLR_BIT(REG_ACK_CFG, DEC1_MTRI);
        LOG(5, "AUD_OutPadDisable u1DecId = %d\n", u1DecId);
    }
}
#endif

//-----------------------------------------------------------------------------
/** AUD_SpdifOutPadEnable
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_SpdifOutPadEnable(UINT8 u1DecId, BOOL fgEnable)
{

    // TODO in MT5371 ; FIXME
    UNUSED(fgEnable);
    UNUSED(u1DecId);

    /*
        if (BSP_GetIcVersion() >= IC_VER_B) // IC can set pad to tri-state
        {
            if (fgEnable)
            {
                CKGEN_CLR_BIT((u1DecId == AUD_DEC_MAIN) ? REG_AOUT_CFG : REG_AOUT_CFG2, SPDIF_DIR_INPUT);
            }
            else
            {
                CKGEN_SET_BIT((u1DecId == AUD_DEC_MAIN) ? REG_AOUT_CFG : REG_AOUT_CFG2, SPDIF_DIR_INPUT);
            }
        }
        if (fgEnable)
        {
            CKGEN_CLR_BIT(AUD_ACFG, (u1DecId == AUD_DEC_MAIN) ? (0x1 << 13) : (0x1 << 21));
        }
        else
        {
            CKGEN_SET_BIT(AUD_ACFG, (u1DecId == AUD_DEC_MAIN) ? (0x1 << 13) : (0x1 << 21));
        }

        vSetSpdifOutPort(u1DecId, fgEnable);
    */

}

//-----------------------------------------------------------------------------
/** AUD_AdcEnable
 *
 *  @param  fgEnable  1: enable (Set ADC un-mute)
 *                    0: diable (Set ADC mute)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AdcEnable(BOOL fgEnable)
{
#ifdef CC_MT5391_AUD_SUPPORT
#else
    if (fgEnable)
    {
        AUD_CLR_BIT(REG_INT_DACCTL9, ADAC_MUTE);
    }
    else
    {
        AUD_SET_BIT(REG_INT_DACCTL9, ADAC_MUTE);
    }
#endif
}

void AUD_AVMuxSel(UINT8 u1Sel)
{
#ifdef CC_AUD_WM8776_AD_SW_SEL_1
    switch (u1Sel)
    {
        case 0:
            ADAC_CodecAdChlSel(0);
            _AudGpioAvSwitch(0);
            break;
        case 1:
            ADAC_CodecAdChlSel(0);
            _AudGpioAvSwitch(1);
            break;
        case 2:
            ADAC_CodecAdChlSel(0);
            _AudGpioAvSwitch(2);
            break;
        case 3:
            ADAC_CodecAdChlSel(0);
            _AudGpioAvSwitch(3);
            break;
        case 4:
            ADAC_CodecAdChlSel(1);
            break;
        case 5:
            ADAC_CodecAdChlSel(2);
            break;
        case 6:
            ADAC_CodecAdChlSel(3);
            break;
        case 7:
            ADAC_CodecAdChlSel(4);
            break;
        default:
            ADAC_CodecAdChlSel(0);
    }

#else
#ifdef CC_AUD_WM8776_AD_SW_SEL_2
//#elif CC_AUD_WM8776_AD_SW_SEL_2
    switch (u1Sel)
    {
        case 0:
            ADAC_CodecAdChlSel(0);
            break;
        case 1:
            ADAC_CodecAdChlSel(1);
            _AudGpioAvSwitch(0);
            break;
        case 2:
            ADAC_CodecAdChlSel(1);
            _AudGpioAvSwitch(1);
            break;
        case 3:
            ADAC_CodecAdChlSel(1);
            _AudGpioAvSwitch(2);
            break;
        case 4:
            ADAC_CodecAdChlSel(1);
            _AudGpioAvSwitch(3);
            break;
        case 5:
            ADAC_CodecAdChlSel(2);
            break;
        case 6:
            ADAC_CodecAdChlSel(3);
            break;
        case 7:
            ADAC_CodecAdChlSel(4);
            break;
        default:
            ADAC_CodecAdChlSel(0);
    }
#else
    switch (u1Sel)
    {
        case 0:
            ADAC_CodecAdChlSel(0);
            break;
        case 1:
            ADAC_CodecAdChlSel(1);
            break;
        case 2:
            ADAC_CodecAdChlSel(2);
            break;
        case 3:
            ADAC_CodecAdChlSel(3);
            break;
        case 4:
            ADAC_CodecAdChlSel(4);
            _AudGpioAvSwitch(0);
            break;
        case 5:
            ADAC_CodecAdChlSel(4);
            _AudGpioAvSwitch(1);
            break;
        case 6:
            ADAC_CodecAdChlSel(4);
            _AudGpioAvSwitch(2);
            break;
        case 7:
            ADAC_CodecAdChlSel(4);
            _AudGpioAvSwitch(3);
            break;
        default:
            ADAC_CodecAdChlSel(0);
    }
#endif
#endif
}

void AUD_AVComponentSel(AV_COMPONENT_T eAVComp)
{
    AUD_INPUT_ID_T eInputId;

    // Get mux index
    switch (eAVComp)
    {
        case AVC_COMP_VIDEO_0:
            eInputId = AUD_INPUT_ID_COMP_VID_0;
            break;
        case AVC_COMP_VIDEO_1:
            eInputId = AUD_INPUT_ID_COMP_VID_1;
            break;
        case AVC_COMP_VIDEO_2:
            eInputId = AUD_INPUT_ID_COMP_VID_2;
            break;
        case AVC_S_VIDEO_0:
            eInputId = AUD_INPUT_ID_S_VID_0;
            break;
        case AVC_Y_PB_PR_0:
            eInputId = AUD_INPUT_ID_YPBPR_0;
            break;
        case AVC_Y_PB_PR_1:
            eInputId = AUD_INPUT_ID_YPBPR_1;
            break;
        case AVC_VGA:
            eInputId = AUD_INPUT_ID_VGA_0;
            break;
        case AVC_HDMI:
            eInputId = AUD_INPUT_ID_DVI_0;
            break;
        case AVC_S_VIDEO_1:
            eInputId = AUD_INPUT_ID_S_VID_1;
            break;
        case AVC_S_VIDEO_2:
            eInputId = AUD_INPUT_ID_S_VID_2;
            break;
        default:
            eInputId = AUD_INPUT_ID_COMP_VID_0;
    }

    AUD_AvMuxSel(eInputId);

}

void AUD_AvMuxSel(AUD_INPUT_ID_T eInputId)
{
    UINT32 u4GpioSelNum1 = UNUSED_GPIO;
    UINT32 u4GpioSelNum2 = UNUSED_GPIO;
    UINT32 u4GpioSelNum3 = UNUSED_GPIO;
    UINT32 u4GpioSelNum4 = UNUSED_GPIO;
    INT32  i4GpioSel1Value = NO_USED;
    INT32  i4GpioSel2Value = NO_USED;
    INT32  i4GpioSel3Value = NO_USED;
    INT32  i4GpioSel4Value = NO_USED;
    UINT8  u1AacChlSel;
    INT32  i4Out = 1;
    static BOOL fgInit = FALSE;
    AUD_INPUT_SW_GPIO_T* prInputSwGpio;
    AUD_INPUT_MUX_SEL_TABLE_T* prInputMuxSelTable;

    AUD_AdcEnable(FALSE);               // Mute ADC to avoid AV mux pop noise

    // Query ADAC config
    UNUSED(DRVCUST_OptQuery(eAudioInputSwGpioSel, (UINT32 *)(void *)&prInputSwGpio));
#ifdef CC_AUD_SPECIAL_INPUT_MUX
    if (AUD_GetTargetCountry() == COUNTRY_EU)
    {
        UNUSED(DRVCUST_OptQuery(eAudioSpecialInputMuxSelTable, (UINT32 *)(void *)&prInputMuxSelTable));
    }
    else
    {
        UNUSED(DRVCUST_OptQuery(eAudioInputMuxSelTable, (UINT32 *)(void *)&prInputMuxSelTable));
    }
#else
    UNUSED(DRVCUST_OptQuery(eAudioInputMuxSelTable, (UINT32 *)(void *)&prInputMuxSelTable));

#ifdef CC_AUD_USE_NVM
    if (!Aud_NVM_GetCountry())
    {
        if (!fgInit)
            prInputMuxSelTable->prAudInMuxSel += ((INT32)(prInputMuxSelTable->u4Size)) / 2;
    }
#endif // CC_AUD_USE_NVM

#endif // CC_AUD_SPECIAL_INPUT_MUX

    u1AacChlSel = prInputMuxSelTable->prAudInMuxSel[eInputId].u1AdcMuxSel;
    if (u1AacChlSel >= (UINT8)AUD_INPUT_ID_MAX)
    {
        LOG(0, "Invalid AdacChlSel %d\n", (UINT32)u1AacChlSel);
    }

    // Set adac channel
    ADAC_CodecAdChlSel(prInputMuxSelTable->prAudInMuxSel[eInputId].u1AdcMuxSel);

    // Load configuration
    u4GpioSelNum1 = prInputSwGpio->u4AudInSwGpioNum1;
    u4GpioSelNum2 = prInputSwGpio->u4AudInSwGpioNum2;
    u4GpioSelNum3 = prInputSwGpio->u4AudInSwGpioNum3;
    u4GpioSelNum4 = prInputSwGpio->u4AudInSwGpioNum4;
    i4GpioSel1Value = (INT32)prInputMuxSelTable->prAudInMuxSel[eInputId].u1SwGpioNum1Val;
    i4GpioSel2Value = (INT32)prInputMuxSelTable->prAudInMuxSel[eInputId].u1SwGpioNum2Val;
    i4GpioSel3Value = (INT32)prInputMuxSelTable->prAudInMuxSel[eInputId].u1SwGpioNum3Val;
    i4GpioSel4Value = (INT32)prInputMuxSelTable->prAudInMuxSel[eInputId].u1SwGpioNum4Val;

    // Setup gpio  => Init gpio
    if (!fgInit)
    {
        if (u4GpioSelNum1 != UNUSED_GPIO)
        {
            //UNUSED(BSP_PinGpioSel(u4GpioSelNum1)); // FIXME !! Gpio group bounding bug,
            UNUSED(GPIO_Enable((INT32)u4GpioSelNum1, (INT32 *)&i4Out));
        }
        if (u4GpioSelNum2 != UNUSED_GPIO)
        {
            //UNUSED(BSP_PinGpioSel(u4GpioSelNum2)); // FIXME !! Gpio group bounding bug,
            UNUSED(GPIO_Enable((INT32)u4GpioSelNum2, (INT32 *)&i4Out));
        }
        if (u4GpioSelNum3 != UNUSED_GPIO)
        {
            //UNUSED(BSP_PinGpioSel(u4GpioSelNum3));    // FIXME !! Gpio group bounding bug,
            UNUSED(GPIO_Enable((INT32)u4GpioSelNum3, (INT32 *)&i4Out));
        }
        if (u4GpioSelNum4 != UNUSED_GPIO)
        {
            //UNUSED(BSP_PinGpioSel(u4GpioSelNum4));    // FIXME !! Gpio group bounding bug,
            UNUSED(GPIO_Enable((INT32)u4GpioSelNum4, (INT32 *)&i4Out));
        }

        fgInit = TRUE;
    }

    // Setup gpio  => write gpio
    if ((u4GpioSelNum1 != UNUSED_GPIO) && (i4GpioSel1Value != SW_NO_USED))
    {
        UNUSED(GPIO_Output((INT32)u4GpioSelNum1, (INT32 *)&i4GpioSel1Value));
    }
    if ((u4GpioSelNum2 != UNUSED_GPIO) && (i4GpioSel2Value != SW_NO_USED))
    {
        UNUSED(GPIO_Output((INT32)u4GpioSelNum2, (INT32 *)&i4GpioSel2Value));
    }
    if ((u4GpioSelNum3 != UNUSED_GPIO) && (i4GpioSel3Value != SW_NO_USED))
    {
        UNUSED(GPIO_Output((INT32)u4GpioSelNum3, (INT32 *)&i4GpioSel3Value));
    }
    if ((u4GpioSelNum4 != UNUSED_GPIO) && (i4GpioSel4Value != SW_NO_USED))
    {
        UNUSED(GPIO_Output((INT32)u4GpioSelNum4, (INT32 *)&i4GpioSel4Value));
    }

    //Setup DVD GPIO
    _eInputIdToDvdGpioConfig = eInputId;
    AUD_DspDvdEnable();

    if (_IsUseMT8292())
    {
        x_thread_delay(250);
    }

    AUD_AdcEnable(TRUE);               // Un-mute ADC
}

INT32 _AUD_GetAudFifo(UINT32 * pu4Fifo1Start, UINT32 * pu4Fifo1SEnd, UINT32 * pu4Fifo2Start, UINT32 * pu4Fifo2End)
{
    if ((pu4Fifo1Start != NULL) &&
            (pu4Fifo1SEnd != NULL) &&
            (pu4Fifo2Start != NULL) &&
            (pu4Fifo2End != NULL))
    {
        *pu4Fifo1Start = DSP_PHYSICAL_ADDR(u4GetAFIFOStart(AUD_DEC_MAIN));
        *pu4Fifo1SEnd = DSP_PHYSICAL_ADDR(u4GetAFIFOEnd(AUD_DEC_MAIN));
        *pu4Fifo2Start = DSP_PHYSICAL_ADDR(u4GetAFIFOStart(AUD_DEC_AUX));
        *pu4Fifo2End = DSP_PHYSICAL_ADDR(u4GetAFIFOEnd(AUD_DEC_AUX));
    }
    else
    {
        return AUD_FAIL;
    }

    LOG(10, "Audio Fifo(%x,%x)(%x,%x)\n", DSP_PHYSICAL_ADDR(u4GetAFIFOStart(AUD_DEC_MAIN)),
        DSP_PHYSICAL_ADDR(u4GetAFIFOEnd(AUD_DEC_MAIN)), DSP_PHYSICAL_ADDR(u4GetAFIFOStart(AUD_DEC_AUX)),
        DSP_PHYSICAL_ADDR(u4GetAFIFOEnd(AUD_DEC_AUX)));

    return AUD_OK;
}

void _AUD_SpdifInPinMuxEnable(void)
{
    /*
        UINT32 u4Reg1 = 0;
        UINT32 u4Reg2 = 0;

        u4Reg1 = (CKGEN_READ32(REG_PINMUX_SEL_1) & ~(0xc000)) | 0x50000000;
        u4Reg2 = (CKGEN_READ32(REG_PINMUX_SEL_2) & ~(0x0ff003ff)) | 0x0f501155;

        CKGEN_WRITE32(REG_PINMUX_SEL_1, u4Reg1);
        CKGEN_WRITE32(REG_PINMUX_SEL_2, u4Reg2);
    */
}

void AUD_SetSrc(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    UNUSED(_aeDecFmt);

    _aeStrSrcConf[u1DecId] = eStreamFrom;
    _aeDecFmt[u1DecId] = eDecFmt;
    _fgChangeSource[u1DecId] = TRUE;
}

// *********************************************************************
// Function : void AUD_DspDvdEnable(void)
// Description : turn on / off dvd gpio by input source and linein select
// Parameter :  none
// Return    :  None.
// *********************************************************************
void AUD_DspDvdEnable(void)
{
    UINT32 u4DvdGpio;
    AUD_INPUT_ID_T eDvdInputChl;
    AUD_DVD_GPIO_POLARITY_T eDvdPolariy;
    INT32 i4RegAoutSwitch;

    //Get DVD input source and DVD gpio Num
    u4DvdGpio = DRVCUST_OptGet(eAudioDvdGpio);
    eDvdInputChl = (AUD_INPUT_ID_T)DRVCUST_OptGet(eAudioDvdInputChl);

    eDvdPolariy = (AUD_DVD_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioDvdPolarity);

    LOG(10, "-------- AUD_DspDvdEnable Configuration -------\n");
    LOG(10, "           DVD Gpio : %d\n", u4DvdGpio);
    LOG(10, "           Dvd Input Channel : %d\n", eDvdInputChl);
    LOG(10, "           DVD Polarity : %d\n", eDvdPolariy);
    LOG(10, "           Decoder Stream Source : %d\n", _aeStrSrcConf[AUD_DEC_MAIN]);
    LOG(10, "           _eInputIdToDvdGpioConfig = %d\n", _eInputIdToDvdGpioConfig);
    LOG(10, "-------- AUD_DspDvdEnable Configuration -------\n");

    if ((_aeStrSrcConf[AUD_DEC_MAIN] != AUD_STREAM_FROM_LINE_IN)  ||
            (_eInputIdToDvdGpioConfig != eDvdInputChl))
    {
        if (eDvdPolariy == AUD_DVD_GPIO_HIGH_ENALBE)
        {
            i4RegAoutSwitch = 0;
        }
        else
        {
            i4RegAoutSwitch = 1;
        }
        LOG(10, "Aout Switch (1) : %d\n", i4RegAoutSwitch);
        UNUSED(GPIO_Output((INT32)u4DvdGpio, (INT32 *)&i4RegAoutSwitch));
    }
    else if ((u4DvdGpio != 0xffffffff) && (_eInputIdToDvdGpioConfig == eDvdInputChl)
             && (_aeStrSrcConf[AUD_DEC_MAIN] == AUD_STREAM_FROM_LINE_IN) )
    {
        if (eDvdPolariy == AUD_DVD_GPIO_HIGH_ENALBE)
        {
            i4RegAoutSwitch = 1;
        }
        else
        {
            i4RegAoutSwitch = 0;
        }
        LOG(10, "Aout Switch (2) : %d\n", i4RegAoutSwitch);
        UNUSED(GPIO_Output((INT32)u4DvdGpio, (INT32 *)&i4RegAoutSwitch));
    }
    else
    {
        LOG(3, "Invalid eStreamFrom in AUD_DspDvdEnable\n");
    }
}

// *********************************************************************
// Function : void AUD_DspDvdDisable(void)
// Description : turn off dvd gpio
// Parameter :  none
// Return    :  None.
// *********************************************************************
void AUD_DspDvdDisable(void)
{
    UINT32 u4DvdGpio;
    AUD_INPUT_ID_T u4DvdInputChl;
    AUD_DVD_GPIO_POLARITY_T eDvdPolariy;
    INT32 i4RegAoutSwitch;

    //Get DVD input source and DVD gpio Num
    u4DvdGpio = DRVCUST_OptGet(eAudioDvdGpio);
    u4DvdInputChl = (AUD_INPUT_ID_T)DRVCUST_OptGet(eAudioDvdInputChl);

    eDvdPolariy = (AUD_DVD_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioDvdPolarity);

    LOG(8, "AUD_DspDvdDisable Configuration -------\n");
    LOG(8, "DVD Gpio : %d\n", u4DvdGpio);
    LOG(8, "Dvd Input Channel : %d\n", u4DvdInputChl);
    LOG(8, "DVD Polarity : %d\n", eDvdPolariy);
    LOG(8, "Decoder Stream Source : %d\n", _aeStrSrcConf[AUD_DEC_MAIN]);
    LOG(8, "_eInputIdToDvdGpioConfig = %d\n", _eInputIdToDvdGpioConfig);
    if ((u4DvdGpio != 0xffffffff) && (_eInputIdToDvdGpioConfig == u4DvdInputChl)
            && (_aeStrSrcConf[AUD_DEC_MAIN] == AUD_STREAM_FROM_LINE_IN) )
    {
        if (eDvdPolariy == AUD_DVD_GPIO_HIGH_ENALBE)
        {
            i4RegAoutSwitch = 0;
        }
        else
        {
            i4RegAoutSwitch = 1;
        }
        LOG(8, "Aout Switch : %d\n", i4RegAoutSwitch);
        UNUSED(GPIO_Output((INT32)u4DvdGpio, (INT32 *)&i4RegAoutSwitch));
    }
    else
    {
        LOG(3, "Input source is not DVD stream\n");
    }
}

AUD_INPUT_ID_T AUD_GetAvInputSrcId(void)
{
    return _eInputIdToDvdGpioConfig;
}

AUD_DEC_STREAM_FROM_T AUD_GetStrSrc(UINT8 u1DedId)
{
    return _aeStrSrcConf[u1DedId];
}

void AUD_SPDIF_Output_HDMI_Rx(BOOL fgEnable)
{
    UINT32 u4Data;
#ifndef CC_MT5391_AUD_SUPPORT
    u4Data = AUD_READ32(SPOCFG0);
    if (fgEnable)
    {
        u4Data |= HDMI_RX_SPDIF;
#if defined(CC_MT8223)
		BSP_PinSet(ASPDIF,PINMUX_FUNCTION1);//by xiaoyi 8223 should setting pinmux more.
#endif
    }
    else
    {
        u4Data &= (~(UINT32)HDMI_RX_SPDIF);
#if defined(CC_MT8223)
		BSP_PinSet(ASPDIF,PINMUX_FUNCTION0);//by xiaoyi 8223 should setting pinmux more.
#endif
    }
    AUD_WRITE32(SPOCFG0, u4Data);
#else
    u4Data = AUD_READ32(REG_AU_PAD) & (~SPDIF_OUT_SEL_MSK);
    if (fgEnable)
    {
        u4Data |= HDMI_RX_SPDIF;
    }
    else
    {
        u4Data |= AUDIO_SPDIF;
    }
    AUD_WRITE32(REG_AU_PAD, u4Data);
#endif
}

UINT32 AUD_DspGetPc(void)
{
    return AUD_READ32(REG_DSP_PC);
}

#if 1 // PROBE_RELATED_API
static void AUD_DspProbeEnable(void)
{
    if (!(AUD_READ32(REG_DSP_CTRL_R) & 0x80))
    {
        AUD_WRITE32(REG_DSP_CTRL_R, 0x80 | AUD_READ32(REG_DSP_CTRL_R));
    }
}

UINT32 AUD_DspProbePause(void)
{
    AUD_DspProbeEnable();
    AUD_WRITE32(REG_DSP_PB_CTRL, 0x100);
    return 0;
}

UINT32 AUD_DspProbeGo(void)
{
    AUD_DspProbeEnable();
    AUD_WRITE32(REG_DSP_PB_CTRL, 0x000);
    return 0;
}

UINT32 AUD_DspProbeStep(UINT32 u4Count)
{
    UINT32 u4Data;
    u4Data = u4Count << 8;
    AUD_DspProbeEnable();
    AUD_WRITE32(REG_DSP_PB_CTRL, u4Data);
    return 0;
}

UINT32 fgAUD_DspProbeHalt(void)
{
    AUD_DspProbeEnable();
    return (AUD_READ32(REG_DSP_PB_CTRL)&0x1);
}

UINT32 u4AUD_DspProbeRead(UINT32 u4Addr)
{
    UINT16 u2Addr;
    u2Addr = u4Addr & 0xFFFF;
    AUD_DspProbeEnable();
    AUD_WRITE32(REG_DSP_PB_INDEX, u2Addr);
    return (AUD_READ32(REG_DSP_PB_DATA));
}

UINT32 AUD_DspProbeWrite(UINT32 u4Addr, UINT32 u4Data)
{
    UINT16 u2Addr;
    u2Addr = u4Addr & 0xFFFF;
    AUD_DspProbeEnable();
    AUD_WRITE32(REG_DSP_PB_WRDX, u2Addr);
    AUD_WRITE32(REG_DSP_PB_WR, (u4Data & 0xFFFFFF));
    return 0;
}

UINT32 u4AUD_DspProbeStopOnWrite(UINT32 u4Addr)
{
    UINT32 u4Mask;
    UINT32 u4Temp;

    UNUSED(u4Addr);
    UNUSED(u4Mask);

    u4Addr = u4Addr & 0xFFFF;

    if (u4Addr != 0)
    {
        u4Mask = 0x1FFFF;
        AUD_DspProbeEnable();
        AUD_WRITE32 (REG_DSP_PB_HIDX, 0x084);
        AUD_WRITE32 (REG_DSP_PB_CMSK, u4Mask);
        AUD_WRITE32 (REG_DSP_PB_CTAR, (0x010000 | u4Addr));
        AUD_WRITE32 (REG_DSP_PB_TAR, 0x0100);
    }
    else
    {
        u4Temp = AUD_READ32(REG_DSP_PB_TAR);
        AUD_WRITE32 (REG_DSP_PB_TAR, (u4Temp & 0xfffffeff));
    }

    return 0;
}

UINT32 u4AUD_DspProbeChkStopOnWrite(BOOL *fgEnable)
{
    UINT32 u4Addr;

    *fgEnable = (AUD_READ32(REG_DSP_PB_TAR) & (0x100)) >> 8;
    u4Addr = AUD_READ32(REG_DSP_PB_CTAR) & (0xFFFF);

    return u4Addr;

}


UINT32 u4AUD_DspProbePBCfg(BOOL fgSet, UINT32 u4Value)
{
    AUD_DspProbeEnable();
    if (fgSet)
    {
        AUD_WRITE32(REG_DSP_PB_STOP, u4Value);
        return 0;
    }
    return (AUD_READ32(REG_DSP_PB_STOP));
}

UINT32 u4AUD_DspProbePB(BOOL fgSet, UINT32 u4BPNum, UINT32 u4Addr)
{
    AUD_DspProbeEnable();
    if (fgSet)
    {
        if (u4BPNum < 4)
        {
            AUD_WRITE32((REG_DSP_PB_STP0 + (u4BPNum << 2)), u4Addr);
            return 0;
        }
        return 0;
    }
    if (u4BPNum < 4)
    {
        return(AUD_READ32((REG_DSP_PB_STP0 + (u4BPNum << 2))));
    }
    return 0xFFFFFFFF;
}
#endif

UINT32 AUD_AudioDescriptionChlSel(UINT8 u1DecId, AUD_CHL_DEF_T eChl)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    //Customization ... todo
#ifdef CC_MT5391_AUD_SUPPORT
    _MT8297_DacChlSel(u1DecId, eChl);
#else
    //_MT536xInternal_DacChlSel(u1DecId, eChl); //SHINK_8223_CODE
#endif

    return 0;
}

static void AUD_LineinLRDetect(BOOL fgEnable)
{
    UNUSED(AUD_LineinLRDetect);

    if (fgEnable)
    {
        AUD_WRITE32(REG_AUD_X_CFG, (AUD_READ32(REG_AUD_X_CFG) | (LR_DETECT_ON)));
    }
    else
    {
        AUD_WRITE32(REG_AUD_X_CFG, (AUD_READ32(REG_AUD_X_CFG) & (~LR_DETECT_ON)));
    }
}

static void _AudDelay(UINT32 u4Value)
{
    volatile UINT32 u4Cnt;

    volatile UINT32 u4Loop;

    UNUSED(_AudDelay);
    for (u4Cnt = 0; u4Cnt < u4Value; u4Cnt++)
    {
        for (u4Loop = 0; u4Loop < 25/*10*/; u4Loop++)
        {
            ;
        }
    }
}
BOOL AUD_IsHdmiClkExist(void)
{
    BOOL fgRet = TRUE;
    UINT32 u4Reg = 0;
    CRIT_STATE_T crit;

    UNUSED(AUD_IsHdmiClkExist);
    // Step 0. Reset HDMI COUNTER
    crit = x_crit_start();

    vIO32WriteFldAlign(CKGEN_AUD_CKCFG, 1, FLD_RST_HDMI_CNT);

    // Step 1. Check COUNTER
    _AudDelay(10);// HDMI_MCLK = 12.288MHz, CPU_CLK = x00MHz,
    // needs to wait for some CPU cycles

    u4Reg = IO32ReadFld(CKGEN_AUD_CKCFG, FLD_HDMI_CNT);

    x_crit_end(crit);

    if (u4Reg != 0)
    {
        fgRet = TRUE;
    }
    else
    {
        fgRet = FALSE;
    }

    return fgRet;
    //UNUSED(fgRet);
    //return FALSE;
}

