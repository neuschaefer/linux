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

/**
 * @file HDMI control driver
 *
 * @note PWR5V -> vHDMIHPDHigh()
 * @note !PWR5V -> vHDMIHPDLow()
 * @note INIT(see below) -> PWOFF -> PWR5V -> PWON -> PWON2 (TERM_ON, RX_EN, HPD_HIGH)
 * -> SCDT (reset fifo) -> AUTH
 * @note !PWR5V -> INIT (HPD_LOW, TERM_OFF, RESET, MUTE)
 * @note vVdoModeChgDone -> fgApiVideoIsVgaMode -> _rMChannel.bIs444 = SV_TURE or SV_FALSE
 *       fgApiVideoIsVgaMode -> _bHdmiMode -> _bDviTiming -> _rMChannel.bIs444 -> _bMAIN_COLOR_SYS = SV_COLOR_RGB
 *
 * @author chiachi_chiang@mtk.com.tw
 * @author darren_weng@mtk.com.tw
 * @author daniel_hsiao@mtk.com.tw
 */
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "general.h"
#include "hw_hdmi.h"
#include "drv_hdmi.h"
#include "hw_dvi.h"
#include "hw_sys.h"
#include "drv_video.h"
#include "video_def.h"
#include "util.h"
#include "hw_vdoin.h"
//#include "hw_hdtv.h"
//#include "api_audio.h"
#include "util.h"
#include "aud_if.h"
#include "aud_drvif.h"
#include "drv_ycproc.h"
#include "nptv_debug.h"
#include "x_timer.h"
#include "x_pinmux.h"
#include "x_printf.h"
#include "x_hal_5381.h"
#include "eeprom_if.h"
#include "drv_dvi.h"
#include "x_ckgen.h"
#include "x_bim.h"
#include "drvcust_if.h"
#include "x_ckgen.h"
#include "hw_ckgen.h"
#include "eepdef.h"
#include "drv_dvi.h"
#include "hw_pdwnc.h"
#include "drv_tdtv_drvif.h"
#if defined(CC_BYPASS_AVI_INFOFRAME) || defined(CC_SUPPORT_HDMI_FBOOTING)
#include "api_eep.h"
#endif
//#include "X_pdwnc.h"
LINT_EXT_HEADER_END
#define IC_3x3color_test 1
#define DEBUG_HDMI_STATE_CHG 0
#define DEBUG_HDMI_INT 0
#define HDMI_DEBUG 1
#define HDMI_PING 0
#define MT8202_ECN 1
#define HDMI_SELFTESTING 0
#define ECO_0914 1
#define TMDS_EQ_CUSTOM 1 // josh
#define CCIR_HV_Position 0
#define AUD_MUTE_PERIOD                2   // 2 second

#define HDMI_Audio_NewFlow  1

#define ANA_INTF_1_31_16 Fld(16, 16, AC_FULLW32) //31:16

#define PixelInvCutFreq      160 // 2 second

#define Enable_HW_Mute		1
#define FLD_HDMI_CLK_Crystral Fld(1, 20, AC_MSKB2) //20

#define Protect_XCLKInPCLk		1
#define 	XCLKInPCLkCNT		5
#define   XCLKInPCLkCNTMid    2	// (XCLKInPCLkCNT-1)/2
// SLT test
#ifdef __MODEL_slt__
#define __HDMI_SLT__ 1  // for SLT testing
#endif

#ifdef CC_SUPPORT_HDMI_FBOOTING
UINT32 _u4HdmiLoops = 0;
UINT8 u1HdmiInit = 0;
UINT8 _u1HdmiPwrOn = 1;
UINT8 _u1HdmiSrcIn = 0;
UINT8 _u1HdcpGood = 0;
#endif

#define ReHPD    0
UINT8   _bHDMIAudioInit;
UINT8   _bAudHdmiFlg;
UINT8   _bHDMISampleChange;
UINT8   _bSmpFrq = AUD_FS_44K;
UINT8   _bHDMIRangeMode = DRV_CUSTOM_HDMI_RANGE_AUTO;
static UINT8 _bHdmiAudFreq;
PRIVATE BOOL _fgVideoOn = FALSE;
//PRIVATE BOOL _fgVideoStableAudioUnstable = FALSE;
//Audio format configuration
PRIVATE AIN_CFG_T _rAudCfg;
HAL_TIME_T _rHdmiPlugWaitTime;
HAL_TIME_T _rHdmiLowPlugWaitTime;
HAL_TIME_T _rHdmiUnplugTime;

#ifdef CC_SUPPORT_HDMI_CLK5VDET
UINT8  _bNonStdDev;
UINT8  _b5VActive;
UINT8  _b5VDetCnt;
UINT8  _bTmdsActive;
UINT8  _bTmdsDetCnt;
UINT8  _bHpdPulse;
#endif

#if HDMI_Audio_NewFlow
UINT8 _bHDMIAudFIFOflag;
#endif

/* Force 3D */
BOOL  _fgForce3D;
UINT8 _bForce3DStructure;
UINT8 _bForce3DExtData;
#ifdef CC_Support_Sil9285
extern void vHDMISwitchMainLoop(void);
#endif

extern UINT8   _bIntMuteCnt ;

static BOOL _fgAudMute = FALSE;

#define IS_AUD_MUTE()   _fgAudMute

#define hdmi_audio 0

// Configuration
#define HDMI_AUD_FMT_CHG_PROTECT

// Constant define
#define HDMI_AUD_OK               0
#define HDMI_AUD_UNSTABLE_LVL_1   1
#define HDMI_AUD_UNSTABLE_LVL_2   2
#define HDMI_AUD_UNSTABLE_LVL_3   3

#define DDC_0 0x80E0		//	PADCFG3
#define DDC_12 0x80EC	//	PADCFG6

#define DDC_32 Fld(32, 0, AC_MSKDW)


extern UINT32   _bDviMdChgCnt;
extern UINT8   _bDviDeChgCnt;
extern UINT8   _bDviPixClkChgCnt;

extern UINT8 _bDviModeChged;
extern void vCloseIec (void);
extern BOOL _AUD_IsSpdifTypeOff(void);
extern void vOpenIec(void);
extern void vHDMISwitchInit(void);
extern UINT8 bHDMIPort5VStatus(UINT8);

#ifdef CC_support_i2c_switch
extern void vHDMISwitchDDCBusCtrl(UINT8,UINT8);
extern void vHDMISwitchCLKTerm(UINT8,UINT8);
#endif

#define MT53XX_PREAMBLE_CTI_MASK  (0xf << 8)
#define MT53XX_AUD_MUTE     (0x1 << 25)
#define MT53XX_BYP_SYNC     (0x1 << 17)
#define MT53XX_BYP_DVIFILT  (0x1 << 18)

#define MT53XX_FIFO_RESET   (0x1 << 9)

#define HPD_5V	1

#define Enable_TDFIFO_RESET	1    // WT.Chang command for deep color

#if !HPD_5V
PRIVATE void vHDMITMDSReset(void)
{
    /* reset FIFO */
    vRegWrite4BMsk(TMDS_CTRL0, 0x0, Fld2Msk32(PCLK_INV) | Fld2Msk32(TMDS_SP_RST_) | Fld2Msk32(TMDS_RT_RST_) | Fld2Msk32(TMDS_RX_RST_));
    vRegWrite4BMsk(TMDS_CTRL0,
                   Fld2Msk32(TMDS_SP_RST_) | Fld2Msk32(TMDS_RT_RST_) | Fld2Msk32(TMDS_RX_RST_),
                   Fld2Msk32(TMDS_SP_RST_) | Fld2Msk32(TMDS_RT_RST_) | Fld2Msk32(TMDS_RX_RST_));

}
#endif

PRIVATE void vHDMIHDCPRst(void)
{
    vRegWrite4BMsk(SRST, Fld2Msk32(HDCP_RST), Fld2Msk32(HDCP_RST));
    vRegWrite4BMsk(SRST, 0 , Fld2Msk32(HDCP_RST));
}
/**
 * Return the average of (VID_XPCNT*100).
 * NOTE: CAN NOT CALL wHDMIXPCCNT FREQUENTLY
 */

#define DATA_CNT_MAX 2
static UINT32 wHDMIXPCCNT(void)
{
    UINT32 datacnt[DATA_CNT_MAX];
    UINT32 idx, i, tmp;
#if Protect_XCLKInPCLk
    UINT32 j, k, tmp_clk[5], tmp_clk_buffer;
#endif

    for (i = 0 ; i < DATA_CNT_MAX ; i++)
    {
        datacnt[i] = 0;
    }

    idx = 0;
    // TODO
    for (i = 0; i < 10; i++)
    {

#if Protect_XCLKInPCLk
        for (j = 0; j < XCLKInPCLkCNT; j++)
        {
            tmp_clk[j] = ((((UINT32)u1RegRead1B(VID_CRC_OUT_2) & 0x7) << 8) | (UINT32)u1RegRead1B(VID_CRC_OUT_3)) + 1;
            vUtDelay2us(1);
        }
        for (k = XCLKInPCLkCNT;k > 0;k--)
        {
            for (j = 0;j < k - 1;j++)
            {
                if (tmp_clk[j] > tmp_clk[j+1])
                {
                    tmp_clk_buffer = tmp_clk[j];
                    tmp_clk[j] = tmp_clk[j+1];
                    tmp_clk[j+1] = tmp_clk_buffer;
                }
            }
        }
        tmp = tmp_clk[XCLKInPCLkCNTMid];
#else
        tmp = (((u1RegRead1B(VID_CRC_OUT_2) & 0x7) << 8) | u1RegRead1B(VID_CRC_OUT_3)) + 1;
#endif
        if (datacnt[0] == tmp)
        {
            continue;
        }
        if (datacnt[1] == tmp)
        {
            continue;
        }

        if (idx < DATA_CNT_MAX)
        {
            datacnt[idx] = tmp;
        }

        idx++;
        if (idx == 2)
        {
            break;
        }
    }

    if (idx == 2)
    {
        return (datacnt[0] + datacnt[1])*(100 / 2);
    }
    else
    {
        return ((datacnt[0])*(100));
    }
}

static UINT32 dwHDMIPixelFreq(void)
{
    UINT32 pfreq;
    UINT32 div;

    div = wHDMIXPCCNT();
    if (div == 0)
    {
        return 1;
    }
    pfreq = ( (UINT32)(1024 * 27 * 1000) * 100);
    pfreq /= (div);

    return pfreq;
}
UINT8 bHDMIAUDIOSampleRateCal(void)
{
    UINT32 wCTS_HW, wN_HW;
    UINT32 wAudSampleRate;
    UINT8 btmp;
    static UINT8 Ori_audio_FS;

    btmp = 20;
    wCTS_HW = (RegReadFldAlign(CTS_HVAL, CTS_VAL_HW19_16) << 16) | (RegReadFldAlign(CTS_HVAL, CTS_VAL_HW15_8) << 8) | RegReadFldAlign(CTS_HVAL, CTS_VAL_HW7_0);
    wN_HW = (RegReadFldAlign(N_HVAL, N_VAL_HW) << 16) | (RegReadFldAlign(N_SVAL, N_VAL_HW15_8) << 8) | RegReadFldAlign(N_SVAL, N_VAL_HW7_0);
    //wAudSampleRate= (dwHDMIPixelFreq()*10/128) *(wN_HW/wCTS_HW);
    wAudSampleRate = (((dwHDMIPixelFreq() * 1000) / wCTS_HW) * ((wN_HW * 100) / 128)) / 10000;
    //LOG(6, " HW CTS =%d  and N =%d , AudSample rate =%d\n", wCTS_HW,wN_HW,wAudSampleRate);
    LOG(8, " HW CTS =%d  and N =%d\n", wCTS_HW, wN_HW);
    if (bHDMIDeepColorStatus() == 0x1)
    {
        wAudSampleRate = (wAudSampleRate * 10) / 8;
    }
    else if (bHDMIDeepColorStatus() == 0x2)
    {
        wAudSampleRate = (wAudSampleRate * 12) / 8;
    }
    else if (bHDMIDeepColorStatus() == 0x3)
    {
        wAudSampleRate = (wAudSampleRate * 16) / 8;
    }
    LOG(8, "AudSample rate =%d\n", wAudSampleRate);

    if (wAudSampleRate > (320 - btmp) && wAudSampleRate < (320 + btmp))
    {
        if (Ori_audio_FS != AUD_FS_32K)
        {
            Ori_audio_FS = AUD_FS_32K;
        }
        return AUD_FS_32K;
    }
    else if (wAudSampleRate > (441 - btmp) && wAudSampleRate < (441 + btmp))
    {
        if (Ori_audio_FS != AUD_FS_44K)
        {
            Ori_audio_FS = AUD_FS_44K;
        }
        return AUD_FS_44K;
    }
    else if (wAudSampleRate > (480 - btmp) && wAudSampleRate < (480 + btmp))
    {
        if (Ori_audio_FS != AUD_FS_48K)
        {
            Ori_audio_FS = AUD_FS_48K;
        }
        return AUD_FS_48K;
    }
    else if (wAudSampleRate > (880 - btmp) && wAudSampleRate < (880 + btmp))
    {

        if (Ori_audio_FS != AUD_FS_88K)
        {
            Ori_audio_FS = AUD_FS_88K;
        }
        return AUD_FS_88K;
    }
    else if (wAudSampleRate > (960 - btmp) && wAudSampleRate < (960 + btmp))
    {
        if (Ori_audio_FS != AUD_FS_96K)
        {
            Ori_audio_FS = AUD_FS_96K;
        }
        return AUD_FS_96K;
    }
    else if (wAudSampleRate > (1760 - btmp) && wAudSampleRate < (1760 + btmp))
    {
        if (Ori_audio_FS != AUD_FS_176K)
        {
            Ori_audio_FS = AUD_FS_176K;
        }
        return AUD_FS_176K;
    }
    else if (wAudSampleRate > (1920 - btmp) && wAudSampleRate < (1920 + btmp))
    {
        if (Ori_audio_FS != AUD_FS_192K)
        {
            Ori_audio_FS = AUD_FS_192K;
        }
        return AUD_FS_192K;
    }
    else
    {
        return AUD_FS_UNKNOWN;
    }

}
/**
 * @brief 	HDMI Audio Config (audio initial)
 * @param	None
 * @retval	None
 */
PRIVATE void vHDMIAudConfig(void)
{
    // Load audio configuration
    if (!AUD_GetAinCfg(AUD_STREAM_FROM_HDMI, &_rAudCfg))
    {
        // If audio input not initialed, use the following setting
        _rAudCfg.eStrSrc =      AUD_STREAM_FROM_HDMI;
        _rAudCfg.eFormat =      FORMAT_RJ;
        _rAudCfg.eBits =        DAC_24_BIT;
        _rAudCfg.eCycle =       LRCK_CYC_32;
        _rAudCfg.eSampleFreq =  MCLK_256FS;
        _rAudCfg.fgDataInvert = FALSE;
        _rAudCfg.fgLRInvert =   FALSE;
    }

    // Setup audio format
    if (_rAudCfg.fgLRInvert)
    {
        vRegWrite4BMsk(I2S_CTRL, Fld2Msk32(WS), Fld2Msk32(WS));// AIN_LR_NonINV
    }
    else
    {
        vRegWrite4BMsk(I2S_CTRL, 0, Fld2Msk32(WS));	// AIN_LR_INV
    }

    if (_rAudCfg.eFormat == FORMAT_RJ)
    {
        vRegWrite4BMsk(I2S_CTRL, Fld2Msk32(JUSTIFY) | Fld2Msk32(HDMI_1ST_BIT), Fld2Msk32(JUSTIFY) | Fld2Msk32(HDMI_1ST_BIT));// Right-Justified
    }
    else if (_rAudCfg.eFormat == FORMAT_LJ)
    {
        vRegWrite4BMsk(I2S_CTRL, Fld2Msk32(HDMI_1ST_BIT), Fld2Msk32(HDMI_1ST_BIT));// Left-Justified
    }
    else if (_rAudCfg.eFormat == FORMAT_I2S)
    {
        vRegWrite4BMsk(I2S_CTRL, 0, Fld2Msk32(JUSTIFY) | Fld2Msk32(HDMI_1ST_BIT));// I2S
    }

    if (_rAudCfg.eCycle == LRCK_CYC_16)
    {
        vRegWrite4BMsk(I2S_CTRL, Fld2Msk32(SIZE), Fld2Msk32(SIZE));//LR_Cycles 16
    }
    else if (_rAudCfg.eCycle == LRCK_CYC_32)
    {
        vRegWrite4BMsk(I2S_CTRL, 0, Fld2Msk32(SIZE));//LR_Cycles 32
    }

    // Sample edge is falling
    vRegWrite4BMsk(I2S_CTRL, Fld2Msk32(CLK_EDGE), Fld2Msk32(CLK_EDGE));

    // Setup sampling frequency MCLK
    if (_rAudCfg.eSampleFreq == MCLK_128FS)
    {
        vRegWriteFldAlign(ACR_CTRL1, 0x0, FS_VAL_SW);
    }
    else if (_rAudCfg.eSampleFreq == MCLK_256FS)
    {
        vRegWriteFldAlign(ACR_CTRL1, 0x1, FS_VAL_SW);
    }
    else if (_rAudCfg.eSampleFreq == MCLK_384FS)
    {
        vRegWriteFldAlign(ACR_CTRL1, 0x2, FS_VAL_SW);
    }
    else if (_rAudCfg.eSampleFreq == MCLK_512FS)
    {
        vRegWriteFldAlign(ACR_CTRL1, 0x3, FS_VAL_SW);
    }

    // Enable CLK
    vRegWrite4BMsk(AUDRX_CTRL, Fld2Msk32(I2S_MODE), Fld2Msk32(I2S_MODE));
    vRegWrite4BMsk(I2S_CTRL, Fld2Msk32(MCLK_EN) | Fld2Msk32(SD0_EN) | Fld2Msk32(SD1_EN) | Fld2Msk32(SD2_EN), Fld2Msk32(MCLK_EN) | Fld2Msk32(SD0_EN) | Fld2Msk32(SD1_EN) | Fld2Msk32(SD2_EN));
    vRegWrite4BMsk(I2S_CTRL, 0, Fld2Msk32(PCM));

#ifdef HDMI_AUD_FMT_CHG_PROTECT

    // Enable auto audio configuration
    // => Auto mute as one of the following condition raised
    //    a. Audio FIFO Underrun INTR4[0]
    //    b. Audio FIFO Overrun INTR4[1]
    //    c. CTS Reused INTR4[2]
    //    d. Fs Changed INTR5[0]
    //
    //    Clear condtion by the following operation
    //    -> vRegWrite4BMsk(INTR_MASK1, Fld2Msk32(INTR5_AUDIO_MUTE), Fld2Msk32(INTR5_AUDIO_MUTE));

    // Setup auto mute condition a+b+d
    vRegWrite4BMsk(AEC_CTRL, (0x16 << 24), Fld2Msk32(EXP_EN_15_8));
#if !HDMI_Audio_NewFlow
    // Enable auto audio configuration
    vRegWrite4BMsk(AEC_CTRL, Fld2Msk32(AAC_EN), Fld2Msk32(AAC_EN));
#endif
    // Enable AAC to control SD0~3
    vRegWrite4BMsk(AEC_CTRL, Fld2Msk32(AAC_OUT_OFF_EN), Fld2Msk32(AAC_OUT_OFF_EN));

#endif

    // FIXME
    vHDMIAudioOutOn();
}

void vHDMIAudChannelStatus(BOOL* pfgCopyright, UINT8* pu1CategoryCode, 
                                UINT8* p1WordLength, BOOL* pfgIsPcm)
{
    if (pfgCopyright)
    {
        // 0: copyright is asserted, 1: No copyright is asserted
        *pfgCopyright = !((BOOL)(RegReadFldAlign(AUDRX_CTRL, AUD_CH_STAT1_2)));
    }
    if (pfgIsPcm)
    {
        // 0: PCM sample, 1: other purpose
        *pfgIsPcm = !((BOOL)(RegReadFldAlign(AUDRX_CTRL, AUD_CH_STAT1_1)));
    }
    if (pu1CategoryCode)
    {

        *pu1CategoryCode = RegReadFldAlign(AUDRX_CTRL, AUD_CH_STAT2);
    }
    if (p1WordLength)
    {
        UINT8 u1Length;
        UINT8 u1WordLength = 24;

        u1Length = RegReadFldAlign(CHST1, AUD_LENGTH);
            
        if (RegReadFldAlign(CHST1, AUD_LENGTH_MAX))
        {
            switch(u1Length)
            {
            case 1:
               u1WordLength = 16;
               break;
            case 2:
               u1WordLength = 18;
               break;
            case 4:
               u1WordLength = 19;
               break; 
            case 5:
               u1WordLength = 20;
               break;  
            case 6:
               u1WordLength = 17;
               break;   
            default:
               u1WordLength = 24;
            }
            
        }
        else
        {
            switch(u1Length)
            {
            case 1:
               u1WordLength = 20;
               break;
            case 2:
               u1WordLength = 22;
               break;
            case 4:
               u1WordLength = 23;
               break; 
            case 5:
               u1WordLength = 24;
               break;  
            case 6:
               u1WordLength = 21;
               break;   
            default:
               u1WordLength = 24;
            }
        }
        
        *p1WordLength = u1WordLength;
    }
}

#if !HDMI_Audio_NewFlow
UINT8 bHDMIAudStatus(void)
{
    UINT8 u1UnstableLvl = HDMI_AUD_OK;

    // Note that the checking should based on the the level from low to high

    if (RegReadFldAlign(INTR_STATE0, INTR1_PLL_UNLOCKED))
    {
        vRegWrite4B(INTR_STATE0, Fld2Msk32(INTR1_PLL_UNLOCKED));
        LOG(8, "Audio PLL unlocked\n");
        //u1UnstableLvl = HDMI_AUD_UNSTABLE_LVL_1;
        u1UnstableLvl = HDMI_AUD_OK;
    }

    if (RegReadFldAlign(INTR_STATE1, INTR4_CTS_REUSED_ERR))
    {
        vRegWrite4B(INTR_STATE1, Fld2Msk32(INTR4_CTS_REUSED_ERR));
        //u1UnstableLvl = HDMI_AUD_UNSTABLE_LVL_1;
        u1UnstableLvl = HDMI_AUD_OK;
        LOG(8, "CTS Reused\n");

    }
    if (RegReadFldAlign(INTR_STATE0, INTR1_FIFO_ERR))
    {

        vRegWrite4B(INTR_STATE0, Fld2Msk32(INTR1_FIFO_ERR));
        LOG(8, "Audio FIFO error\n");
        u1UnstableLvl = HDMI_AUD_OK;
    }

    if (_bDviModeChged)
    {
        u1UnstableLvl = HDMI_AUD_UNSTABLE_LVL_1;
        LOG(8, "Video not stable.. mute audio..\n");
    }
    if (RegReadFldAlign(INTR_STATE1, INTR5_AUDIO_MUTE))
    {
        vRegWrite4B(INTR_STATE1, Fld2Msk32(INTR5_AUDIO_MUTE));
        u1UnstableLvl = HDMI_AUD_UNSTABLE_LVL_1;
        LOG(8, "Audio Auto Configure muted ...\n");
    }
    if (RegReadFldAlign(INTR_STATE1, INTR4_UNDERRUN))
    {
        vRegWrite4B(INTR_STATE1, Fld2Msk32(INTR4_UNDERRUN));
        u1UnstableLvl = HDMI_AUD_UNSTABLE_LVL_2;
        LOG(8, "FIFO Under-Run\n");
    }
    if (RegReadFldAlign(INTR_STATE1, INTR4_OVERRUN))
    {
        vRegWrite4B(INTR_STATE1, Fld2Msk32(INTR4_OVERRUN));
        u1UnstableLvl = HDMI_AUD_UNSTABLE_LVL_2;
        LOG(8, "FIFO Overrun\n");
    }
    if (RegReadFldAlign(INTR_STATE1, INTR5_AUD_SAMPLE_F))
    {
        vRegWrite4B(INTR_STATE1, Fld2Msk32(INTR5_AUD_SAMPLE_F));
        u1UnstableLvl = HDMI_AUD_UNSTABLE_LVL_2;
        LOG(8, "Audio Fs Sample Rate Change\n");
        vHDMIHandleAudFmtChange();
    }

    return u1UnstableLvl;
}
#else
UINT8 bHDMIAudStatus(void)
{
    return _bHDMIAudFIFOflag;
}
#endif
/**
 * @brief 	fgHDMIAudFifoFault
 * @param	None
 * @retval	None
 */
PRIVATE BOOL fgHDMIAudFifoFault(void)
{
    return (RegReadFldAlign(INTR_STATE1, INTR4_UNDERRUN) | RegReadFldAlign(INTR_STATE1, INTR4_OVERRUN)
            | RegReadFldAlign(INTR_STATE1, INTR5_AUD_SAMPLE_F)) ? TRUE : FALSE;
}

PRIVATE void vHDMIMuteAudio(void)
{
#ifdef CC_SUPPORT_HDMI_FBOOTING
	if (!fgIsExtSrcHDMI())
	{
		return;
	}
#endif
    if (!IS_AUD_MUTE())
    {
        vRegWrite4BMsk(CHST1, Fld2Msk32(CH0_MUTE), Fld2Msk32(CH0_MUTE));
        _fgAudMute = TRUE;
        AUD_HdmiModeMute(AUD_DEC_MAIN, TRUE);
        LOG(9, " [AUD_HDMI] AUDIO HDMI MUTE AUDIO\n");
    }
}

PRIVATE void vHDMIUnMuteAudio(void)
{
#ifdef CC_SUPPORT_HDMI_FBOOTING
	if (!fgIsExtSrcHDMI())
	{
		return;
	}
#endif
    if (IS_AUD_MUTE())
    {
        vRegWrite4BMsk(CHST1, 0, Fld2Msk32(CH0_MUTE));
        _fgAudMute = FALSE;
        AUD_HdmiModeMute(AUD_DEC_MAIN, FALSE);
        LOG(9, " [AUD_HDMI] AUDIO HDMI UNMUTE AUDIO\n");
    }
}

/**
 * @brief 	vHDMIHandleAudFifoFault
 * @param	None
 * @retval	None
 */
PRIVATE void vHDMIHandleAudFifoFault(void)
{
    UINT32 HW_CTS_Value;
    UINT8 u1Fs;

    HW_CTS_Value = (UINT32)RegReadFldAlign(CTS_HVAL, CTS_VAL_HW7_0) | ((UINT32)RegReadFldAlign(CTS_HVAL, CTS_VAL_HW15_8) << 8) | ((UINT32)RegReadFldAlign(CTS_HVAL, CTS_VAL_HW19_16) << 16);

#if !HDMI_Audio_NewFlow
    // Audio be muted ..
    if (u4RegRead4B(AUDP_STAT) & MT53XX_AUD_MUTE)
    {
        vRegWrite4B(AUDP_STAT, (u4RegRead4B(AUDP_STAT) & (~MT53XX_AUD_MUTE)));
        LOG(8,  "##################### (Audio be muted) #####################\n");
    }

    if (fgHDMIAudFifoFault())
    {
        LOG(11, "Audio FIFO: %x\n", RegReadFldAlign(INTR_STATE1, INTR_STATE1_0));//LOG(11, "Audio FIFO: %x\n", RegReadFldAlign(INTR_MASK0,INTR_MASK0_0));
        LOG(11, "[AUD_HDMI] Reset audio fifo\n");
        // HDMI audio fifo reset
        vRegWrite4B(SRST, u4RegRead4B(SRST) | MT53XX_FIFO_RESET);
        vRegWrite4B(SRST, u4RegRead4B(SRST) & ~MT53XX_FIFO_RESET);

        // FOR san diego setop box channel change
        //	    vRegWriteFldAlign(SRST, 1, AAC_RST);
        //   vUtDelay1ms(1);
        //   vRegWriteFldAlign(SRST, 0, AAC_RST);
        //

        vUtDelay2us(500);

        // Clear interrupt
        vRegWrite4B(INTR_STATE1, Fld2Msk32(INTR4_OVERRUN) | Fld2Msk32(INTR4_UNDERRUN) | Fld2Msk32(INTR5_AUDIO_MUTE));
        //vRegWrite4BMsk(INTR_MASK0, Fld2Msk32(INTR4_OVERRUN), Fld2Msk32(INTR4_OVERRUN));
    }
#endif

    if (_fgVideoOn)
    {
        u1Fs = bHDMIAUDIOSampleRateCal();
        if (u1Fs == AUD_FS_UNKNOWN)
        {
            vHDMIMuteAudio();
        }
        else
        {
            if ((RegReadFldAlign(SRST, DEEP_STA) == 0x0) && (650000 < HW_CTS_Value || 81000 == HW_CTS_Value))
            {
                vHDMIMuteAudio();
            }
            else
            {
                vHDMIUnMuteAudio();
            }
        }
    }
}

#if 0
PRIVATE BOOL fgHDMIAudFmtChange(void)
{
    BOOL fgRet = FALSE;
    if (RegReadFldAlign(INTR_STATE1, INTR5_AUD_SAMPLE_F) > 0)
    {
        fgRet = TRUE;
    }
    // Clear interrupt
    vRegWrite4BMsk(INTR_STATE1, Fld2Msk32(INTR5_AUD_SAMPLE_F), Fld2Msk32(INTR5_AUD_SAMPLE_F));

    return fgRet;
}
#endif

UINT8 bHDMIGetAudSampleFreq(void)
{
    return RegReadFldAlign(LK_THRS_SVAL, RHDMI_AUD_SAMPLE_F);//read sample rate
}

void vHDMIHandleAudFmtChange(void)
{
    UINT8 u1Fs;
    // Audio sampling rate change
    u1Fs = bHDMIAUDIOSampleRateCal();

    if (u1Fs == AUD_FS_192K)
    {
        AUD_AoutDacFs(AUD_DEC_MAIN, FS_192K); // FIXME ! Support dual decoder later
        vHDMIAudConfig();
        LOG(6, "HDMI audio sampling rate change to 192K\n");
    }
    if (u1Fs == AUD_FS_176K)
    {
        AUD_AoutDacFs(AUD_DEC_MAIN, FS_176K); // FIXME ! Support dual decoder later
        vHDMIAudConfig();
        LOG(6, "HDMI audio sampling rate change to 176K\n");
    }
    if (u1Fs == AUD_FS_48K)
    {
        AUD_AoutDacFs(AUD_DEC_MAIN, FS_48K); // FIXME ! Support dual decoder later
        vHDMIAudConfig();
        LOG(6, "HDMI audio sampling rate change to 48K\n");
    }
    if (u1Fs == AUD_FS_96K)
    {
        AUD_AoutDacFs(AUD_DEC_MAIN, FS_96K); // FIXME ! Support dual decoder later
        vHDMIAudConfig();
        LOG(6, "HDMI audio sampling rate change to 96K\n");
    }
    else if (u1Fs == AUD_FS_88K)
    {
        AUD_AoutDacFs(AUD_DEC_MAIN, FS_88K); // FIXME ! Support dual decoder later
        vHDMIAudConfig();
        LOG(6, "HDMI audio sampling rate change to 88K\n");
    }
    else if (u1Fs == AUD_FS_32K)
    {
        AUD_AoutDacFs(AUD_DEC_MAIN, FS_32K); // FIXME ! Support dual decoder later
        vHDMIAudConfig();
        LOG(6, "HDMI audio sampling rate change to 32K\n");
    }
    else
    {
        // Default set to 48kHz
        AUD_AoutDacFs(AUD_DEC_MAIN, FS_48K); // FIXME ! Support dual decoder later
        vHDMIAudConfig();
        LOG(6, "HDMI audio sampling rate change to 44K\n");
    }
}



/*
UINT8 vHDMIAinGetMode(void)
{
    UINT8 u1Mode;
    u1Mode = RegReadFldAlign(AUDRX_CTRL, AUD_CH_STAT1_1);//read mode

    return (u1Mode);
}
*/
void vHDMISpdifOutEnable(BOOL fgEnable)
{
    if (fgEnable == (BOOL)TRUE)
    {
        vRegWrite4BMsk(AUDRX_CTRL, Fld2Msk32(HW_MUTE_EN), Fld2Msk32(HW_MUTE_EN));
        LOG(3, "HDMI SPDIF output enable\n");
    }
    else
    {
        vRegWrite4BMsk(AUDRX_CTRL, 0, Fld2Msk32(HW_MUTE_EN));
        LOG(3, "HDMI SPDIF output disable\n");
    }
}

/**
 * @brief 	read HDMI audio type
 * @param	None
 * @retval	0=PCM ,1 =none PCM
 */
UINT8 bHDMI_IsPCM(void)
{
    return RegReadFldAlign(AUDRX_CTRL, AUD_CH_STAT1_1);
}

/**
 * @brief 	read HDMI audio length
 * @param	None
 * @retval
 */
UINT8 bHDMIGetAudioBitWidth(void)
{
    UINT8 length;
    length = RegReadFldAlign(CHST1, AUD_LENGTH);
    if (RegReadFldAlign(CHST1, AUD_LENGTH_MAX))
    {
        switch (length)
        {
            case 0x1:
                return HDMI_AUD_Length_20bits;
            case 0x6:
                return HDMI_AUD_Length_21bits;
            case 0x2:
                return HDMI_AUD_Length_22bits;
            case 0x4:
                return HDMI_AUD_Length_23bits;
            case 0x5:
                return HDMI_AUD_Length_24bits;
            default:
                return HDMI_AUD_Length_Unknown;
        }
    }
    else
    {
        switch (length)
        {
            case 0x1:
                return HDMI_AUD_Length_16bits;
            case 0x6:
                return HDMI_AUD_Length_17bits;
            case 0x2:
                return HDMI_AUD_Length_18bits;
            case 0x4:
                return HDMI_AUD_Length_19bits;
            case 0x5:
                return HDMI_AUD_Length_20bits;
            default:
                return HDMI_AUD_Length_Unknown;
        }
    }

}


#if HDMI_Audio_NewFlow
void vHDMIAudErrorHandler(void)
{
    if ((_bHdmiMode == 0) || (_bHDMIState != HDMI_STATE_AUTH) || _bDviModeChged)
    {
        // mute audio
        vRegWriteFldAlign(AUDP_STAT, 1, AUDIO_MUTE);
        vRegWriteFldAlign(AEC_CTRL, 0, AAC_EN);
        vRegWrite4B(INTR_STATE1, Fld2Msk32(INTR5_AUDIO_MUTE));
        _bHDMIAudFIFOflag = 1;
    }
    else
    {
        AUD_DEC_STREAM_FROM_T eAudStreamFrom;
        AUD_GetStreamFrom(AUD_DEC_MAIN, &eAudStreamFrom);        
        if (fgHDMIAudFifoFault() && (eAudStreamFrom == AUD_STREAM_FROM_HDMI))
        {
            bHDMIAPPL2Reset();//in Sony BDP S301 , when video is stable, audio is still unstable, it need to reset Apll continued.             
            if (RegReadFldAlign(INTR_STATE0, INTR2_NEW_AUD_PKT))
            {
                vRegWrite4B(INTR_STATE0, Fld2Msk32(INTR2_NEW_AUD_PKT));

                _bHDMIAudFIFOflag = 1;
                vRegWriteFldAlign(AEC_CTRL, 0, AAC_EN);
                //vRegWriteFldAlign(SRST, 1, ACR_RST);
                //vRegWriteFldAlign(SRST, 0, ACR_RST);
                vRegWriteFldAlign(SRST, 1, FIFO_RST);
                vUtDelay2us(3);
                vRegWriteFldAlign(SRST, 0, FIFO_RST);
                vUtDelay2us(500);

                if (RegReadFldAlign(INTR_STATE1, INTR5_AUD_SAMPLE_F))
                {
                    vHDMIHandleAudFmtChange();
                }

                // Clear interrupt
                vRegWrite4B(INTR_STATE1, Fld2Msk32(INTR4_OVERRUN) | Fld2Msk32(INTR4_UNDERRUN) | Fld2Msk32(INTR5_AUD_SAMPLE_F));
                vRegWrite4B(INTR_STATE1, Fld2Msk32(INTR5_AUDIO_MUTE));
                vUtDelay2us(100);
            }
        }
        else
        {
            // normal case
            _bHDMIAudFIFOflag = 0;
            vRegWriteFldAlign(AEC_CTRL, 1, AAC_EN);
            vRegWriteFldAlign(AUDP_STAT, 0, AUDIO_MUTE);

            if ((u1RegRead1B(I2S_CTRL_3) & 0xf8) == 0x0)
            {
                vRegWriteFldAlign(AEC_CTRL, 0, AAC_EN);
                vRegWriteFldAlign(AEC_CTRL, 1, AAC_EN);
            }
        }
    }

    if (_bHDMIAudFIFOflag == 1)
    {
        _bHdmiAudFreq = AUD_FS_48K;   // default: 48k
    }
    else
    {
        //_bHdmiAudFreq = (UINT8)(RegReadFld(CHST1, AUD_SAMPLE_F) & 0xf);
        _bHdmiAudFreq = bHDMIAUDIOSampleRateCal();//modify by ciwu,using HW N/CTS to calculate Fs
    }
}
#endif
/*****************************************************************************
 * HDMI State Machine Declaration
*****************************************************************************/


static CHAR * _aszHdmiState[] =
{
    "HDMI_STATE_NOTREADY",
    "HDMI_STATE_INIT",
    "HDMI_STATE_PWOFF",
    "HDMI_STATE_PWON",
    "HDMI_STATE_PWON2",
    "HDMI_STATE_SCDT",
    "HDMI_STATE_AUTH"
};

/*****************************************************************************
 *If it want to use MT8202 internal audio clk 27mhz,please turn on this function.
*****************************************************************************/
#define INTER_AUDCLK 1


/*****************************************************************************
 * Variable Declaration
*****************************************************************************/

UINT8   _bHDMIState;
UINT8   _bHdmiFlag;
UINT8   _bHdmiCmd;
UINT8   _bHdmiCnt;
UINT8   _bHdmiPwOnDelay;
UINT16   _bHPDdelay;
UINT16  _bCKDTcnt;
UINT8  _bHdmiPlug ;
UINT8  _bHdmiLowPlug ;
// HDMI Status
UINT8   _bHdmiAudFs;
UINT8   _bHdmiMode; // 0 - DVI, 1 - HDMI
UINT8   _bHdmiMD;
UINT8   _bHDMICurrSwitch;
#if MUTE_TEMP
// PRIVATE UINT8 bHDMIdelayTDMSReset;
//#define HDMI_OFFON_MUTE 60 // off to on
#define HDMI_HPD_ONDELAY 3000
#define HDMI_MUTE_COUNT 20 // mode change
#endif
#define HDMI_XPC_STABLE_CNT 30 // unit, per Vsync, xclk in pclk stable count
#if 0 //INFORM_MDCHG
UINT8 bHDMIMCCnt; // counter for mode change
#endif
#define SAVE_WEAK_IC 0
#if SAVE_WEAK_IC
UINT8 bHDMIBadSync;
#endif

UINT8 _bAVIInfo_tmp;
UINT8   _bHDMIScanInfo;
UINT8 _bHDMIAspectRatio;
UINT8 _bHDMIAFD;
UINT8 _bHDMIHDCPStatus;
UINT8 _bHDMI422Input;
UINT8 _bHDMIITCFlag;
UINT8 _bHDMIITCContent;
UINT8 _bIntr_CK_CHG;
UINT8 _bSCDTdelay;
UINT8 _bNEW_AVI_Info;
UINT8 _bACPCount;
UINT8 _bHPD_Indep_Ctrl;	//  1 is 5v detect , 0 is CKDT detect
UINT32 _wHDMI_OFFON_MUTE_COUNT;
UINT32 _wDVI_WAIT_STABLE_COUNT;
UINT32 _wHDMIBypassFlag;
UINT32 _wDVI_WAIT_NOSIGNAL_COUNT;
UINT32 _wHDMI_WAIT_SCDT_STABLE_COUNT;
#if (TMDS_EQ_CUSTOM == 1)
UINT32 _wHDMI_EQ_ZERO_VALUE;
UINT32 _wHDMI_EQ_BOOST_VALUE;
UINT32 _wHDMI_EQ_SEL_VALUE;
UINT32 _wHDMI_EQ_GAIN_VALUE;
UINT32 _wHDMI_LBW_VALUE;
#endif
UINT32 _wHDMI_HDCP_MASk1;
UINT32 _wHDMI_HDCP_MASk2;
#if !PATCH_PIONEER_DV696_IR_OFF_ON_PURPLE_AND_SNOW_NOISE
UINT8 _bPreHPDLowDelay;
#endif
UINT32 _bHDP_Value;
UINT8 _bHDMIConnectFlag;
UINT8 _bNodifyConnect;
UINT8 _bUnplugFlag;
UINT8 _bUnplugCount;
UINT8 _bForceHPDLow;
UINT8 _bHDMIColorSpace;
#define ATC_720p_fix 1
UINT8   _bEQFlag;
UINT8 _bHPDCycle;
UINT8 _bCKDTMonitor;
UINT8 _bFixedEQReset;
#ifdef CC_BYPASS_AVI_INFOFRAME
UINT8 _bDVIAVIinfoBypass;
#endif
#ifdef CC_Support_Sil9285
UINT8 _bHDMI9287switchstopflag;
#ifdef CC_PHI_DREAMCATCHER_DANGERIOUS
UINT8 _bHDMI9287debugflagprint = 0;
#endif
#endif

static UINT32 u4PreXpcCnt;
static UINT32 u4CurXpcCnt;
static UINT8 _bXpcStableCnt;

UINT8 bHDMIHDCPStatusGet(void)
{
    if (RegReadFldAlign(HDCP_STAT, HDCP_AUTH) && RegReadFldAlign(HDCP_STAT, HDCP_DECRYPT))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

UINT8 bHDMIHDCPKeyCheck(void)
{
//    UINT32 u4Idx;
    UINT32 u4Data[4];
    INT32 fgSet;

    fgSet = EEPHDCP_Init();
    if (fgSet)
    {
        LOG(2, "EEPHDCP_Init fail\n");
        return 0;
    }

    // write to hdcp sram pointer.
    IO_WRITE32(BIM_BASE, REG_RW_HDCP_ADDR, 0);

    u4Data[0] = IO_READ32(BIM_BASE, REG_RW_HDCP_DATA);
    u4Data[1] = IO_READ32(BIM_BASE, REG_RW_HDCP_DATA);
    u4Data[2] = IO_READ32(BIM_BASE, REG_RW_HDCP_DATA);
    u4Data[3] = IO_READ32(BIM_BASE, REG_RW_HDCP_DATA);
    if ((u4Data[0] == 0xffffffff) && (u4Data[1] == 0xffffffff))
    {
        LOG(2, "NO HDCP key in EEPROM\n");
        return 0;

    }

    return 1;
}
void  vHDMISetEQRsel(UINT8 u4Value)
{
}

UINT32  vHDMIGetEQSetting(void)
{
    //printf("EQ value=0x%x",RegReadFldAlign(TMDS_CTRL1,EQSEL));
      #if !Support_HW_Adaptive_EQ  // fix EQ
		if ((u1RegRead1B(ANA_INTF_1_1) & 0x06) == 0x06)
		{
			printf("Fix EQ!\n");
			printf("HDMI_EQ_FILT: 0x%x\n", RegReadFldAlign(ANA_INTF_1, RG_HDMI_EQ_FILT));
			printf("Max->Min: 0x7, 0x6, 0x5, 0x4, 0x3, 0x2, 0x1, 0x0, 0xf, 0xe, 0xd, 0xc, 0xb, 0xa, 0x9, 0x8\n");
			printf("HDMI_EQBIAS(default=0x4): 0x%x\n", RegReadFldAlign(ANA_INTF_1,RG_HDMI_EQBIAS));
			printf("HDMI_PLLBIAS(default=0x2): 0x%x\n", RegReadFldAlign(ANA_INTF_2,RG_HDMI_PLLBIAS));
			printf("HDMI_EQ_GAIN(default=0x0): 0x%x\n", RegReadFldAlign(ANA_INTF_2,RG_HDMI_EQ_GAIN));
		}
	#else
		if ((u1RegRead1B(ANA_INTF_1_1) & 0x06) == 0x0)
		{
			printf("Adaptive EQ!\n");
			printf("0x22E60[24]: 0x%x\n", ((RegReadFldAlign(TMDS_CTRL_STATUS1, HDMI_CH0_STATUS)&0x100)>>8));
			printf("R channel EQ: 0x%x\n", RegReadFldAlign(TMDS_CTRL_STATUS1, HDMI_CH0_EQERR));
			printf("G channel EQ: 0x%x\n", RegReadFldAlign(TMDS_CTRL_STATUS2, HDMI_CH1_EQERR));
			printf("B channel EQ: 0x%x\n", RegReadFldAlign(TMDS_CTRL_STATUS3, HDMI_CH2_EQERR));
			printf("Max->Min: 0x7, 0x6, 0x5, 0x4, 0x3, 0x2, 0x1, 0x0, 0xf, 0xe, 0xd, 0xc, 0xb, 0xa, 0x9, 0x8\n");
		}
	#endif
    return 0;
}
#ifdef CC_BYPASS_AVI_INFOFRAME
/**
 * @brief 	Bypass AVI infoframe check when DVI decode search timing
 * @param	None
 * @retval	1=bypass ,0 =org
 */
void vHDMIDVIBypassAVIinfo(UINT8 value)
{
    //UNUSED(fgApiEepromWriteByte(EEP_HDMI_AVIINFO_BYPASS,value));
    _bDVIAVIinfoBypass = value;
}
UINT8 bHDMIGetDVIBypassAVIinfo(void)
{
    return _bDVIAVIinfoBypass;
}
#endif

UINT32 bHDMIGetHPDAdjust(void)
{
    return _bHDP_Value;
}
void vHDMIHPDAdjust(UINT32 value)
{
    _wHDMI_OFFON_MUTE_COUNT = value;
    _bHDP_Value = value ;
}

UINT8 bHDMIColorFormat(UINT8 method)
{
    switch (method)
    {
        default:
        case 0:	//auto detect
            if (_bHdmiMode)
            {
                if (u1RegRead1B(AVIRX0_0) != 0x82)
                {
                    return SV_COLOR_HDMI_RGB;
                }
            }
            if ((u1RegRead1B(AVIRX1_0)& 0x60) == 0x00)
            {
                return SV_COLOR_HDMI_RGB;
            }
            else if ((u1RegRead1B(AVIRX1_0) & 0x60) == 0x20)
            {
                if ((u1RegRead1B(AVIRX1_0)  & 0x7) == 0x1)
                {
                    return SV_COLOR_HDMI_XVYCC_601;
                }
                else if ((u1RegRead1B(AVIRX1_0)  & 0x7) == 0x1)
                {
                    return SV_COLOR_HDMI_XVYCC_709;
                }
                else
                {
                    if (bHDMIClocrimetry())
                    {
                        //601
                        return  SV_COLOR_HDMI_422_601;
                    }
                    else
                    {
                        //709
                        return  SV_COLOR_HDMI_422_709;
                    }
                }


            }
            else if ((u1RegRead1B(AVIRX1_0)  & 0x60) == 0x40)
            {
                if ((u1RegRead1B(AVIRX1_0)  & 0x7) == 0x1)
                {
                    return SV_COLOR_HDMI_XVYCC_601;
                }
                else if ((u1RegRead1B(AVIRX1_0)  & 0x7) == 0x1)
                {
                    return SV_COLOR_HDMI_XVYCC_709;
                }
                else
                {
                    if (bHDMIClocrimetry())
                    {
                        //601
                        return  SV_COLOR_HDMI_444_601;
                    }
                    else
                    {
                        //709
                        return  SV_COLOR_HDMI_444_709;
                    }
                }
            }
            break;
        case 1:	//force detect by AVI infoframe
            if ((u1RegRead1B(AVIRX1_0)& 0x60) == 0x00)
            {
                return SV_COLOR_HDMI_RGB;
            }
            else if ((u1RegRead1B(AVIRX1_0) & 0x60) == 0x20)
            {
                if (bHDMIClocrimetry())
                {
                    //601
                    return  SV_COLOR_HDMI_422_601;
                }
                else
                {
                    //709
                    return  SV_COLOR_HDMI_422_709;
                }
            }
            else if ((u1RegRead1B(AVIRX1_0)  & 0x60) == 0x40)
            {
                if (bHDMIClocrimetry())
                {
                    //601
                    return  SV_COLOR_HDMI_444_601;
                }
                else
                {
                    //709
                    return  SV_COLOR_HDMI_444_709;
                }
            }

            break;
        case 2:	//force detect by GAMUT infoframe
            if ((u1RegRead1B(GAMUTRX0_3)& 0x7) == 0x00)
            {
                return SV_COLOR_HDMI_RGB;
            }
            else if ((u1RegRead1B(AVIRX1_0) & 0x7) == 0x1)
            {
                return SV_COLOR_HDMI_XVYCC_601;
            }
            else if ((u1RegRead1B(AVIRX1_0)  & 0x7) == 0x2)
            {
                return SV_COLOR_HDMI_XVYCC_709;
            }
            break;

    }
    /* not auto detect & invalid info-frame color-space */
    return SV_COLOR_HDMI_RGB;

}

/**
 * @brief 	HDMI HPD control bypass
 * @param	None
 * @retval	1=bypass ,0 =org
 */
void vHDMIHPDIndepCtrl(UINT8 cmd)
{
    _bHPD_Indep_Ctrl = cmd;
}

#if ATC_720p_fix
UINT8 TMDS_delay;
#endif
/**
 * @brief 	HDMI input is RGB /Ycbcr
 * @param	None
 * @retval	1=RGB ,0 =YCBCR
 */
UINT8 bHDMIInputType(void)
{
    UINT8 bReadData;
    if (_bHdmiMode)
    {
        if (u1RegRead1B(AVIRX0_0) != 0x82)
        {
            return 1;
        }
        if (RegReadFldAlign(INTR_STATE0, INTR3_NEW_AVI))
        {
            _bNEW_AVI_Info = 1;
            bReadData = u1RegRead1B(AVIRX1_0);
            vRegWrite4B(INTR_STATE0, 0x1 << 24);
            _bAVIInfo_tmp = bReadData;
        }
        else
        {
            bReadData = _bAVIInfo_tmp;
        }

        if ((bReadData & 0x60) == 0x00) // RGB
            return 1;
        else //Ycbcr
            return 0;
    }
    else
        return 1;
}
/**
 * @brief 	HDMI Clocrimetry
 * @param	None
 * @retval	1=ITU601 ,0 =ITU709
 */
UINT8 bHDMIClocrimetry(void)
{
    UINT8 bReadData;
    bReadData = u1RegRead1B(AVIRX1_1);
    if ((bReadData&0xc0) == 0x40)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief 	vHDMIApllClkReset
 * @param	None
 * @retval	None
 */
void vHDMIApllClkReset(void)
{
    vRegWrite4BMsk(TMDS_CTRL0, 0, Fld2Msk32(PCLK_INV));
    vRegWrite4BMsk(TMDS_CTRL0, Fld2Msk32(PCLK_INV), Fld2Msk32(PCLK_INV));
    vRegWrite4BMsk(TMDS_CTRL0, 0, Fld2Msk32(PCLK_INV));
    vRegWrite4BMsk(TMDS_CTRL0, Fld2Msk32(PCLK_INV), Fld2Msk32(PCLK_INV));
    vRegWrite4BMsk(TMDS_CTRL0, 0, Fld2Msk32(PCLK_INV));	//add by ciwu
    //vRegWrite4BMsk(TMDS_CTRL0, Fld2Msk32(PCLK_INV),Fld2Msk32(PCLK_INV));
}

/**
 * @brief 	HDMI Clock Detect
 * @param	None
 * @retval	None
 */
void bHDMIClockDetect(void)
{
    UINT8 bReadData;
    bReadData = u1RegRead1B(SRST_2);
    if ((bReadData&0x02) != 0x02)
    {
        vHDMIApllClkReset();
    }
}

void vHDMIDDCBusCTRL(UINT8 bOnOff)
{
	if(bOnOff)
	{
		vRegWrite1B(SYS_CTRL_1, 0x95);
	}
	else
	{
		vRegWrite1B(SYS_CTRL_1, 0x85);
	}
	#ifdef CC_support_i2c_switch
		vHDMISwitchDDCBusCtrl(_bHDMICurrSwitch,bOnOff);
	#endif
}
void vHDMITMDSCTRL(UINT8 bOnOff)
{
    if (!_bHPD_Indep_Ctrl)
    {
        // PD_TERM: Enable TMDS-PHY termination 50-ohm resistance
        if (bOnOff)
        {
            /*turn on TMDS*/
            vRegWrite1B(PD_SYS_2, 0xff);
            vRegWriteFldAlign(ANA_INTF_2, 0x1, RG_HDMI_TERM_EN);
        }
#ifndef __HDMI_SLT__
        else
        {
            /* turn off TMDS*/
            vRegWrite1B(PD_SYS_2, 0xcf);
            vRegWriteFldAlign(ANA_INTF_2, 0x0, RG_HDMI_TERM_EN);
        }
#endif
#ifdef CC_support_i2c_switch
	vHDMISwitchCLKTerm(_bHDMICurrSwitch,bOnOff);
#endif
    }
}

void bHDMIAPPL2Reset(void)
{
    CRIT_STATE_T rCritState;

    rCritState = x_crit_start();
#if IC_5365_VERIFY
    vIO32WriteFldAlign(0xf000d134, 0x0, Fld(1, 30, AC_MSKB3));
    vIO32WriteFldAlign(0xf000d134, 0x1, Fld(1, 30, AC_MSKB3));
#endif
    x_crit_end(rCritState);
}
void bHDMIAudioSigmaDeltaSetting(UINT32 value1, UINT8 value2, UINT8 bSDINOVR)
{
    vRegWriteFldAlign(APLL0, 0x3, SDREF);
    vRegWriteFldAlign(APLL1, bSDINOVR, SDINOVR);
    vRegWrite4B(CTS_HW_ADP3, value1);
    vRegWriteFldAlign(CTS_HW_ADP4, value2, SDINOVRVAL_39_32);
    if (bSDINOVR)
    {
        bHDMIAPPL2Reset();
    }
}
void bHDMIPHYReset(UINT8 resettype)
{
#if Support_HW_Adaptive_EQ
    UINT8 bCH0EQ;

    bCH0EQ = RegReadFldAlign(TMDS_CTRL_STATUS1, HDMI_CH0_EQERR);
    vRegWriteFldAlign(ANA_INTF_1, bCH0EQ, RG_HDMI_EQ_FILT);
#endif

    if (resettype == HDMI_RST_ALL)
    {
        // vco reset
        vRegWriteFldAlign(ANA_INTF_2, 1, RG_HDMI_VCO_RSTB_SW_EN);
        vRegWriteFldAlign(ANA_INTF_2, 0, RG_HDMI_VCO_RSTB_SW);
        vUtDelay2us(5);
        vRegWriteFldAlign(ANA_INTF_2, 1, RG_HDMI_VCO_RSTB_SW);
        vRegWriteFldAlign(ANA_INTF_2, 0, RG_HDMI_VCO_RSTB_SW);
        vRegWriteFldAlign(ANA_INTF_2, 0, RG_HDMI_VCO_RSTB_SW_EN);
        
        vRegWrite4B(ANA_INTF_0, 0xa0000000);
        vRegWrite4B(ANA_INTF_0, 0x80000000);
#if Support_HW_Adaptive_EQ
        vRegWrite4B(ANA_INTF_1, ((((UINT32)bCH0EQ)<<16)|0x1a800696));
        vRegWrite4B(ANA_INTF_1, ((((UINT32)bCH0EQ)<<16)|0x1a800096));
#endif
        vRegWriteFldAlign(TMDS_CTRL0, 1, C_DATA_SYNC_AUTO);
        vRegWriteFldAlign(TMDS_CTRL0, 0, C_DATA_SYNC_AUTO);

        vRegWriteFldAlign(ANA_INTF_2, 1, RG_HDMI_VCO_RSTB_SW_EN);//open tmds clock pll reset gate;
        vRegWriteFldAlign(ANA_INTF_2, 0, RG_HDMI_VCO_RSTB_SW);//reset tmds clock pll;
        vRegWriteFldAlign(ANA_INTF_2, 1, RG_HDMI_VCO_RSTB_SW);//release tmds clock pll reset;
		vRegWriteFldAlign(ANA_INTF_2, 0, RG_HDMI_VCO_RSTB_SW_EN);//close tmds clock pll reset gate;

        vRegWriteFldAlign(ANA_INTF_3, 0, RG_HDMI_DEEPCLRCLK_PDB);
        vRegWriteFldAlign(ANA_INTF_3, 0, RG_HDMI_DEEPCLRCLK_RSTN);
        vRegWriteFldAlign(ANA_INTF_3, 1, RG_HDMI_DEEPCLRCLK_PDB);
        vRegWriteFldAlign(ANA_INTF_3, 1, RG_HDMI_DEEPCLRCLK_RSTN);
    }
    if (resettype == HDMI_RST_EQ)
    {
        vRegWrite4B(ANA_INTF_1, ((((UINT32)bCH0EQ)<<16)|0x1a800696));
        vRegWrite4B(ANA_INTF_1, ((((UINT32)bCH0EQ)<<16)|0x1a800096));
    }
    if (resettype == HDMI_RST_DEEPCOLOR)
    {
        vRegWriteFldAlign(ANA_INTF_3, 0, RG_HDMI_DEEPCLRCLK_PDB);
        vRegWriteFldAlign(ANA_INTF_3, 0, RG_HDMI_DEEPCLRCLK_RSTN);
        vRegWriteFldAlign(ANA_INTF_3, 1, RG_HDMI_DEEPCLRCLK_PDB);
        vRegWriteFldAlign(ANA_INTF_3, 1, RG_HDMI_DEEPCLRCLK_RSTN);
    }
    if (resettype == HDMI_RST_FIXEQ)
    {
        vRegWrite4B(ANA_INTF_1, 0x1a830696);
        vRegWrite4B(ANA_INTF_1, 0x1a830096);
        //vUtDelay2us(300);
        vUtDelay1ms(1);
        vRegWrite4B(ANA_INTF_1, 0x1a830696);
    }
    if (resettype == HDMI_RST_RTCK)
    {
        vRegWriteFldAlign(TMDS_CTRL0, 1, C_DATA_SYNC_AUTO);
        vRegWriteFldAlign(TMDS_CTRL0, 0, C_DATA_SYNC_AUTO);
    }
#if IC_5365_VERIFY
    vIO32WriteFldAlign(0xf000d134, 0x0, Fld(1, 3, AC_MSKB0));//feedback post divider set to 0
#endif
}
#ifdef CC_FAST_INIT
/**
 * @brief 	HDMI resume Initial function
 * @param	None
 * @retval	None
 */
void vHDMI_pm_resume(void)
{
    UINT8 i;
    // 1. HDMI Chip Availability testing

    _bHDMIState = HDMI_STATE_INIT;

    ///////////////////////////////////////////////////////
    // 2. Software Initial
    _bHPD_Indep_Ctrl = 0;
#if !HPD_5V
    _bForceHPDLow = 1;	//when TV power on, no resistance between GPIO and power5v, HPD always High, Pioneer 969 neglect termination disapper and
#endif					//just care about HPD,thus HPD must toggle again

    _bHdmiFlag = 0;
    _bHdmiCmd = 0;
    _bHdmiCnt = 0;
    _bHdmiAudFs = 0xf;
    _bHdmiMode = 0;
    _bHDMISampleChange = 0;
    _bHDMIAudioInit = 0;
    _bHdmiMD = 0;
    _bHDMIScanInfo = bHDMIScanInfo();
    _bHDMIAspectRatio = bHDMIAspectRatio();
    _bHDMIAFD = bHDMIAFD();
    _bHDMIHDCPStatus = bHDMIHDCPStatusGet();
    _bHDMI422Input = 0;
    _bHDMIITCFlag = 0;
    _bHDMIITCContent = 0;
    _bIntr_CK_CHG = 0;
    _bNEW_AVI_Info = 0;
    _bACPCount = 0;
    _bHDMIConnectFlag = 0;
    _bNodifyConnect = 0;
    _bUnplugFlag = 0;
    _bUnplugCount = 0;
    _bHDMIColorSpace = 0;
    _bHPDCycle = 0;
    _bCKDTMonitor = 0;
    _bFixedEQReset = 0;
#ifdef CC_Support_Sil9285
    _bHDMI9287switchstopflag = 0;
#ifdef CC_PHI_DREAMCATCHER_DANGERIOUS
	_bHDMI9287debugflagprint = 0;
#endif
#endif
#ifdef CC_BYPASS_AVI_INFOFRAME
    _bDVIAVIinfoBypass = bApiEepromReadByte(EEP_HDMI_AVIINFO_BYPASS); //wait EEP_HDMI_AVIINFO_BYPASS implement
#endif

#if HDMI_Audio_NewFlow
    _bHDMIAudFIFOflag = 0;
#endif

	_fgForce3D = FALSE;
	_bForce3DStructure = 0;  // FP
	_bForce3DExtData = 0;
#ifdef CC_SUPPORT_HDMI_CLK5VDET
    _bNonStdDev = 0;
    _b5VActive = 0;
    _b5VDetCnt = 0;
    _bTmdsActive = 0;
    _bTmdsDetCnt = 0;
    _bHpdPulse = 0;
#endif

    u4PreXpcCnt = 0;
    u4CurXpcCnt = 0;
    _bXpcStableCnt = 0;

    _bHdmiAudFreq = AUD_FS_48K;  // 48 k

    _bHDMICurrSwitch = (UINT8) HDMI_SWITCH_INIT;
    if (_bHDMIState == HDMI_STATE_NOTREADY)
    {
        return;
    }

#if (TMDS_EQ_CUSTOM == 1)
    UNUSED(DRVCUST_OptQuery(eHDMIEQZEROVALUE, &_wHDMI_EQ_ZERO_VALUE));
    UNUSED(DRVCUST_OptQuery(eHDMIEQBOOSTVALUE, &_wHDMI_EQ_BOOST_VALUE));
    UNUSED(DRVCUST_OptQuery(eHDMIEQSELVALUE, &_wHDMI_EQ_SEL_VALUE));
    UNUSED(DRVCUST_OptQuery(eHDMIEQGAINVALUE, &_wHDMI_EQ_GAIN_VALUE));
    UNUSED(DRVCUST_OptQuery(eHDMILBWVALUE, &_wHDMI_LBW_VALUE));
#endif
    UNUSED(DRVCUST_OptQuery(eHDMIHDCPMASK1, &_wHDMI_HDCP_MASk1));
    UNUSED(DRVCUST_OptQuery(eHDMIHDCPMASK2, &_wHDMI_HDCP_MASk2));
    ///////////////////////////////////////////////////////
    // 3. HW Initial
    /* SYS_CTRL1
       bit 0 power down mode
       bit 1 output clock invert
       bit 2 video bus mode
       bit 3 pixel bus mode
       bit 5:4 input pixel clock peplication
       bit 7:6 output pixel clock dvider
     */
    vRegWrite1B(SYS_CTRL_0, 0x07);
    vRegWrite4BMsk(SYS_CTRL, Fld2Msk32(RX_EN), Fld2Msk32(RX_EN));

    /* power on all block */
    vRegWrite1B(PD_SYS_0, 0x01);
    vRegWrite1B(PD_SYS_2, 0xCf);
    //vRegWrite1B(PD_SYS_3,0x3f);  // except Tri-state Audio/Video output
    vRegWrite1B(PD_SYS_3, 0xff); // except Tri-state Audio/Video output
    vRegWrite1B(PD_SYS_3, 0xf3);	//solve analog PHY bandgap start-up
    vRegWrite1B(PD_SYS_3, 0xff); //solve analog PHY bandgap start-up


    /*analog setting*/
    vRegWrite4B(ANA_IF, 0x0);
    vRegWrite4B(ANA_INTF_0, 0xa0000000);
    vRegWrite4B(ANA_INTF_0, 0x80000000);
#if !Support_HW_Adaptive_EQ  //fixed EQ
    vRegWrite4B(ANA_INTF_1, 0x1a830696);
    //vRegWriteFldAlign(ANA_INTF_1, 1, RG_HDMI_EQ_RST);
    //vRegWriteFldAlign(ANA_INTF_1, 1, RG_HDMI_EQ_SWRSTSEL);
    //vRegWriteFldAlign(ANA_INTF_1, 0x3, RG_HDMI_EQ_FILT);//fixed EQ value,0x0~0xf
#else
    vRegWrite4B(ANA_INTF_1, 0x1ac00696);
    vRegWrite4B(ANA_INTF_1, 0x1a800096);
#endif
    vRegWrite4B(ANA_INTF_2, 0x11AAF000);
    vRegWrite4B(ANA_INTF_3, 0xE4280000);
    vRegWrite4B(ANA_INTF_3, 0xE42C0000);

#ifdef CC_MT5366P2V1
	vRegWriteFldAlign(ANA_INTF_2, 0x5, RG_HDMI_KPGAIN);
#endif

    /*TMDS setting*/
    vRegWrite4B(TMDS_CTRL0, 0x00000000);//reset TMDS PHY digital part
    vRegWrite4B(TMDS_CTRL0, 0x07000002);//invert CH 1,2,3
    vRegWriteFldAlign(TMDS_CTRL0, 1, C_DATA_SYNC_AUTO);


    vHDMITMDSCTRL(FALSE);

    /* ACR_CTRL3
       bit 1 enable for simplified window for pll_unlock
       bit 2 MCLK Loop back.
       bit 6:3 Threshold for CTS Change
     */
    vRegWrite1B(ACR_CTRL3_0, 0x5C);

    /*LKWIN_SVAL
      bit 4:1 most recent count must be within this margin to
      enable the stability counter to increment
     */
    //vRegWrite4B(UPLL_HVAL,0x1F060030);

    /*LKTHRESH
      PLL Lock Stability Threshold
     */
    vRegWrite1B(LK_THRS_SVAL_0, 0xff);
    vRegWrite1B(LK_THRS_SVAL_1, 0xff);
    /* I2S CTRL1
       bit 0 WS to SD shift First bit
       bit 1 SD data direction
       bit 2 SD Justification
       bit 3 UINT16 Select Left/right Polarity
       bit 4 MSB sign-extended
       bit 5 UINT16 size
       bit 6 sample clock edge
       bit 7 send invalid data enable
     */
    vRegWrite1B(I2S_CTRL_2, 0x49);

    /*HDMI_CRIT1
      Preamble Criteria: selects the required number of repetitions
      for a valid preamble
     */
    vRegWriteFldAlign(AUDP_STAT, 0x6, PREAMBLE_CRI);
    //vRegWriteFldAlign(AUDP_STAT,0x0,PREAMBLE_CRI);
    /*HDMI_CRIT2
      Preamble Criteria: selects the required number of repetitions
      for a valid preamble
     */
    vRegWrite1B(TEST_CTRL_0, 0xC);
    /*ACR_CFG
      ACR PLL configuration
     */
    vRegWrite4B(APLL0, 0x21003680);

    /* int output polarity# */
    vRegWrite1B(INTR_STATE0_0, 0x2);//	vRegWrite1B(INTR_MASK1_1,0x2);

    /* set key mask to zero */
    vRegWrite1B(EPST_3, 0x00);
    vRegWrite1B(KS_MASK_0, 0x00);

    //Enable Audio SPDIF Output
    vRegWrite1B(AUDRX_CTRL_1, 0x1d);

    // KSCL_H
    vRegWrite4BMsk(VID_SET, Fld2Msk32(KSCL_H), Fld2Msk32(KSCL_H));

    // HDCP Keymask
    vRegWrite1B(EPST_3, _wHDMI_HDCP_MASk1);		//vRegWrite1B(EPST_3,0xff);
    vRegWrite1B(KS_MASK_0, _wHDMI_HDCP_MASk2);	//vRegWrite1B(KS_MASK_0,0xc3);

    // setting to avoid TMDS reset -> vHDMITMDSReset
    vRegWrite4BMsk(AUDP_STAT, Fld2Msk32(BYP_SYNC ) | Fld2Msk32(BYP_DVIFILT ) , Fld2Msk32(BYP_SYNC ) | Fld2Msk32(BYP_DVIFILT ));

    // Clear Audio mute
    vRegWrite4B(AUDP_STAT, (u4RegRead4B(AUDP_STAT) & (~MT53XX_AUD_MUTE)));

    // increase the PCCNT resolution  from 128 to 1024 by adam
    vRegWrite4BMsk(VID_CRC_OUT, Fld2Msk32(XCLK_IN_PCLK_SEL), Fld2Msk32(XCLK_IN_PCLK_SEL));

    //decode di_DE and vi_DE by both preamble and guard-band
    vRegWrite4BMsk(MUTE_SET, 0x0, Fld2Msk32(BYPASS_DI_GB));

    //modify 1080p twinkling dot
    //CKGEN_WRITE32(REG_RGBCLK_CFG, CKGEN_READ32(REG_RGBCLK_CFG) |RGB_INV_SEL );
    vIO32WriteFldAlign(CKGEN_RGBCLK_CFG, 1, FLD_RGB_INV_SEL);

    //deep color mode must refer output video clock not TMDS clock
    vRegWriteFldAlign(VID_CRC_OUT, 0x1, XCLK_IN_DPCLK);
    vRegWrite4B(DIGITAL_CKDT, 0x102b1b38);

    //enable  HW GAMUT packet decoder
    vRegWriteFldAlign(N_HDMI_CTRL, 0x1, TT0_GAMUT_EN);

#if Enable_TDFIFO_RESET
    vRegWriteFldAlign(MUTE_SET, 0x1, TDFIFO_SYNC_EN);
#endif

#if Enable_HW_Mute
    vRegWriteFldAlign(VID_VRES, 0x1, VRES_MUTE_AUTO_CLR);
    /*disable external 27MHZ clk input when no signal, XBOX issue for plug/unplug */
    vIO32WriteFldAlign(CKGEN_MISC_CKCFG, 0x0, FLD_HDMI_CLK_Crystral);

    vRegWriteFldAlign(VID_HRES, 0x3, HCHG_CNT_THR);
    vRegWriteFldAlign(VID_HRES, 0xf, HSTB_CNT_THR);
    vRegWriteFldAlign(VID_VRES, 0x3, VSTB_CNT_THR);
    vRegWriteFldAlign(VID_SET, 0x1, MUTE_CKDT);
    vRegWriteFldAlign(VID_HRES, 0x4, HRES_THR); //for Iris project sony DSC T500 and Foxconn project  Setop Box model¡GScienfitic Atlanta 8300HDC flash use
    vRegWriteFldAlign(VID_VRES, 0x2, VCHG_CNT_THR);//for TG 35 ,when change Vtotal in 1080i and 480i, HW auto mute enable and doesn't unmute
#endif

    //if (DRVCUST_OptGet(eHDMISupportExtSwitch) == 1)
    //{
    vHDMISwitchInit();
    //}
    UNUSED(DRVCUST_OptQuery(eHDMIOffOnMuteCount, &_wHDMI_OFFON_MUTE_COUNT));
    UNUSED(DRVCUST_OptQuery(eDVIWaitStableCount, &_wDVI_WAIT_STABLE_COUNT));
    UNUSED(DRVCUST_OptQuery(eHDMIBypassInitialFlow, &_wHDMIBypassFlag));
    UNUSED(DRVCUST_OptQuery(eDVIWaitNosignalCount, &_wDVI_WAIT_NOSIGNAL_COUNT));
    UNUSED(DRVCUST_OptQuery(eDVIWaitSCDTStableCount, &_wHDMI_WAIT_SCDT_STABLE_COUNT));
    vRegWriteFldAlign(INTR_MASK0, 0x1, NEW_ACP_ONLY);

    //initial DDC line Schmitt trigger
    vRegWriteFldAlign(DDC_0, 0x202020, DDC_32);
    vRegWriteFldAlign(DDC_12, 0x20202020, DDC_32);

#ifndef __HDMI_SLT__
    for (i = 1;i <= 4;i++)
    {
        vHDMIHPDHigh(i);
    }
#endif

       
#if ReHPD
    _bCKDTcnt = 0;
#endif

    //#if IC_5365_VERIFY
#ifdef CC_MT5365
    vIO32WriteFldAlign(0xf000d158, 0x1, Fld(1, 7, AC_MSKB0));
#endif
#ifdef CC_MT5395
    vIO32WriteFldAlign(0xf000d150, 0x1, Fld(1, 7, AC_MSKB0));
#endif
    vIO32WriteFldAlign(0xf000d134, 0x0, Fld(1, 3, AC_MSKB0));//feedback post divider set to 0
    vIO32WriteFldAlign(0xf000d134, 0x0, Fld(1, 31, AC_MSKB3));
    vIO32WriteFldAlign(0xf000d134, 0x1, Fld(1, 30, AC_MSKB3));
    vIO32WriteFldAlign(0xf000d134, 0x1, Fld(1, 16, AC_MSKB2));
    //vHDMI_INTERNAL_EDID_Init();


    vRegWriteFldAlign(VID_HRES, 0x1, VRES_CHG_DET_SEL);//detect both progressive and interlace input v resolution change
    vRegWriteFldAlign(VID_HRES, 0x1, INTERLACE_DET_SEL);//If this bit set to 1, Interlace out status is assert when the fallowing condition meets:1. 0 < line difference between 2 field < INTERLACE_LINE_DIFF_TH
    vRegWriteFldAlign(VID_BLANK, 0x2, INTERLACE_LINE_DIFF_TH);//for INTERLACE_DET_SEL
    /*enable I2C deglitch*/
    vIO32WriteFldAlign(PDWNC_EDID_DEV0, 1, FLD_DEGLITCH0_EN);
    vIO32WriteFldAlign(PDWNC_EDID_DEV0, 3, FLD_DE_CNT0);
    vIO32WriteFldAlign(PDWNC_EDID_DEV1, 1, FLD_DEGLITCH1_EN);
    vIO32WriteFldAlign(PDWNC_EDID_DEV1, 3, FLD_DE_CNT1);
    vIO32WriteFldAlign(PDWNC_EDID_DEV2, 1, FLD_DEGLITCH2_EN);
    vIO32WriteFldAlign(PDWNC_EDID_DEV2, 3, FLD_DE_CNT2);
    vIO32WriteFldAlign(PDWNC_EDID_DEV3, 1, FLD_DEGLITCH3_EN);
    vIO32WriteFldAlign(PDWNC_EDID_DEV3, 3, FLD_DE_CNT3);

#if Support_3D
    vRegWriteFldAlign(N_HDMI_CTRL, 0x1, TT2_EXT_PKT_EN);
    vRegWriteFldAlign(N_HDMI_CTRL3, 0x81, EXT_PKT_DEC);
#endif
    vRegWriteFldAlign(ANA_INTF_3, 0, RG_HDMI_LFRCTL_SW);//for HD860 flash black mute/snow noise/not support.
    vRegWriteFldAlign(ANA_INTF_3, 0x6, RG_HDMI_CHPP);

    vRegWriteFldAlign(VID_SET, 0, CEA_NEW_UNREC_SEL);
    vRegWriteFldAlign(INTR_MASK0, 1, NEW_UNREC_ONLY);//interrupt at every 3D inforframe.
    (UINT32)EEPHDCP_Init();
    UNUSED(EEPHDCP_SetSramDevice(1, 0xA0));
    vHDMIHDCPRst();
}
#endif
/**
 * @brief 	HDMI Initial function
 * @param	None
 * @retval	None
 */
void vHDMIInit(void)
{
//	#if HDMI_TI_SWITCH
//	INT32 i;
//	#endif
#ifdef CC_SUPPORT_HDMI_FBOOTING
    UINT8 u1CrntSrc = 0;
    UINT8 u1CrntPort = 0;
#endif

    UINT8 i;
    // 1. HDMI Chip Availability testing
#ifdef CC_SUPPORT_HDMI_FBOOTING
	if (u1HdmiInit == 1)
	{
	    Printf("hdmi re-init\n");
		return;
	}
#endif

    _bHDMIState = HDMI_STATE_INIT;

    ///////////////////////////////////////////////////////
    // 2. Software Initial
    _bHPD_Indep_Ctrl = 0;
#if !HPD_5V
    _bForceHPDLow = 1;	//when TV power on, no resistance between GPIO and power5v, HPD always High, Pioneer 969 neglect termination disapper and
#endif					//just care about HPD,thus HPD must toggle again

    _bHdmiFlag = 0;
    _bHdmiCmd = 0;
    _bHdmiCnt = 0;
    _bHdmiAudFs = 0xf;
    _bHdmiMode = 0;
    _bHDMISampleChange = 0;
    _bHDMIAudioInit = 0;
    _bHdmiMD = 0;
    _bHDMIScanInfo = bHDMIScanInfo();
    _bHDMIAspectRatio = bHDMIAspectRatio();
    _bHDMIAFD = bHDMIAFD();
    _bHDMIHDCPStatus = bHDMIHDCPStatusGet();
    _bHDMI422Input = 0;
    _bHDMIITCFlag = 0;
    _bHDMIITCContent = 0;
    _bIntr_CK_CHG = 0;
    _bNEW_AVI_Info = 0;
    _bACPCount = 0;
    _bHDMIConnectFlag = 0;
    _bNodifyConnect = 0;
    _bUnplugFlag = 0;
    _bUnplugCount = 0;
    _bHDMIColorSpace = 0;
    _bHPDCycle = 0;
    _bCKDTMonitor = 0;
    _bFixedEQReset = 0;
#ifdef CC_Support_Sil9285
    _bHDMI9287switchstopflag = 0;
#ifdef CC_PHI_DREAMCATCHER_DANGERIOUS
	_bHDMI9287debugflagprint = 0;
#endif
#endif
#ifdef CC_BYPASS_AVI_INFOFRAME
    _bDVIAVIinfoBypass = bApiEepromReadByte(EEP_HDMI_AVIINFO_BYPASS); //wait EEP_HDMI_AVIINFO_BYPASS implement
#endif

#if HDMI_Audio_NewFlow
    _bHDMIAudFIFOflag = 0;
#endif

	_fgForce3D = FALSE;
	_bForce3DStructure = 0;  // FP
	_bForce3DExtData = 0;

#ifdef CC_SUPPORT_HDMI_CLK5VDET
    _bNonStdDev = 0;
    _b5VActive = 0;
    _b5VDetCnt = 0;
    _bTmdsActive = 0;
    _bTmdsDetCnt = 0;
    _bHpdPulse = 0;
#endif

    u4PreXpcCnt = 0;
    u4CurXpcCnt = 0;
    _bXpcStableCnt = 0;

    _bHdmiAudFreq = AUD_FS_48K;  // 48 k

    _bHDMICurrSwitch = (UINT8) HDMI_SWITCH_INIT;
    if (_bHDMIState == HDMI_STATE_NOTREADY)
    {
        return;
    }

#if (TMDS_EQ_CUSTOM == 1)
    UNUSED(DRVCUST_OptQuery(eHDMIEQZEROVALUE, &_wHDMI_EQ_ZERO_VALUE));
    UNUSED(DRVCUST_OptQuery(eHDMIEQBOOSTVALUE, &_wHDMI_EQ_BOOST_VALUE));
    UNUSED(DRVCUST_OptQuery(eHDMIEQSELVALUE, &_wHDMI_EQ_SEL_VALUE));
    UNUSED(DRVCUST_OptQuery(eHDMIEQGAINVALUE, &_wHDMI_EQ_GAIN_VALUE));
    UNUSED(DRVCUST_OptQuery(eHDMILBWVALUE, &_wHDMI_LBW_VALUE));
#endif
    UNUSED(DRVCUST_OptQuery(eHDMIHDCPMASK1, &_wHDMI_HDCP_MASk1));
    UNUSED(DRVCUST_OptQuery(eHDMIHDCPMASK2, &_wHDMI_HDCP_MASk2));
    ///////////////////////////////////////////////////////
    // 3. HW Initial
    /* SYS_CTRL1
       bit 0 power down mode
       bit 1 output clock invert
       bit 2 video bus mode
       bit 3 pixel bus mode
       bit 5:4 input pixel clock peplication
       bit 7:6 output pixel clock dvider
     */
    vRegWrite1B(SYS_CTRL_0, 0x07);
    vRegWrite4BMsk(SYS_CTRL, Fld2Msk32(RX_EN), Fld2Msk32(RX_EN));

    /* power on all block */
    vRegWrite1B(PD_SYS_0, 0x01);
    vRegWrite1B(PD_SYS_2, 0xCf);
    //vRegWrite1B(PD_SYS_3,0x3f);  // except Tri-state Audio/Video output
    vRegWrite1B(PD_SYS_3, 0xff); // except Tri-state Audio/Video output
    vRegWrite1B(PD_SYS_3, 0xf3);	//solve analog PHY bandgap start-up
    vRegWrite1B(PD_SYS_3, 0xff); //solve analog PHY bandgap start-up


    /*analog setting*/
    vRegWrite4B(ANA_IF, 0x0);
    vRegWrite4B(ANA_INTF_0, 0xa0000000);
    vRegWrite4B(ANA_INTF_0, 0x80000000);
#if !Support_HW_Adaptive_EQ  //fixed EQ
    vRegWrite4B(ANA_INTF_1, 0x1a830696);
    //vRegWriteFldAlign(ANA_INTF_1, 1, RG_HDMI_EQ_RST);
    //vRegWriteFldAlign(ANA_INTF_1, 1, RG_HDMI_EQ_SWRSTSEL);
    //vRegWriteFldAlign(ANA_INTF_1, 0x3, RG_HDMI_EQ_FILT);//fixed EQ value,0x0~0xf
#else
    vRegWrite4B(ANA_INTF_1, 0x1ac00696);
    vRegWrite4B(ANA_INTF_1, 0x1a800096);
#endif
    vRegWrite4B(ANA_INTF_2, 0x11AAF000);
    vRegWrite4B(ANA_INTF_3, 0xE4280000);
    vRegWrite4B(ANA_INTF_3, 0xE42C0000);

#ifdef CC_MT5366P2V1
	vRegWriteFldAlign(ANA_INTF_2, 0x5, RG_HDMI_KPGAIN);
#endif

    /*TMDS setting*/
    vRegWrite4B(TMDS_CTRL0, 0x00000000);//reset TMDS PHY digital part
    vRegWrite4B(TMDS_CTRL0, 0x07000002);//invert CH 1,2,3
    vRegWriteFldAlign(TMDS_CTRL0, 1, C_DATA_SYNC_AUTO);



    /* TMDS Equalization Control */
#if (TMDS_EQ_CUSTOM == 1)
    // vRegWriteFldAlign(ANA_INTF_0,_wHDMI_EQ_ZERO_VALUE,RG_HDMI_ZERO);
    //LOG(1, "\n*********EQ SETTING: EQ_SEL = %d, EQ_GAIN = %d, HDMI_LBW = %d.*********\n", _wHDMI_EQ_SEL_VALUE, _wHDMI_EQ_GAIN_VALUE, _wHDMI_LBW_VALUE);
    //vRegWriteFldAlign(TMDS_CTRL1,_wHDMI_EQ_SEL_VALUE, EQSEL);
    //vRegWriteFldAlign(TMDS_CTRL1,_wHDMI_EQ_GAIN_VALUE, EQ_GAIN);
    //vRegWriteFldAlign(TMDS_CTRL0,_wHDMI_LBW_VALUE, HDMI_LBW);
#endif

    vHDMITMDSCTRL(FALSE);

    /* ACR_CTRL3
       bit 1 enable for simplified window for pll_unlock
       bit 2 MCLK Loop back.
       bit 6:3 Threshold for CTS Change
     */
    vRegWrite1B(ACR_CTRL3_0, 0x5C);

    /*LKWIN_SVAL
      bit 4:1 most recent count must be within this margin to
      enable the stability counter to increment
     */
    //vRegWrite4B(UPLL_HVAL,0x1F060030);

    /*LKTHRESH
      PLL Lock Stability Threshold
     */
    vRegWrite1B(LK_THRS_SVAL_0, 0xff);
    vRegWrite1B(LK_THRS_SVAL_1, 0xff);
    /* I2S CTRL1
       bit 0 WS to SD shift First bit
       bit 1 SD data direction
       bit 2 SD Justification
       bit 3 UINT16 Select Left/right Polarity
       bit 4 MSB sign-extended
       bit 5 UINT16 size
       bit 6 sample clock edge
       bit 7 send invalid data enable
     */
    vRegWrite1B(I2S_CTRL_2, 0x49);

    /*HDMI_CRIT1
      Preamble Criteria: selects the required number of repetitions
      for a valid preamble
     */
    vRegWriteFldAlign(AUDP_STAT, 0x6, PREAMBLE_CRI);
    //vRegWriteFldAlign(AUDP_STAT,0x0,PREAMBLE_CRI);
    /*HDMI_CRIT2
      Preamble Criteria: selects the required number of repetitions
      for a valid preamble
     */
    vRegWrite1B(TEST_CTRL_0, 0xC);
    /*ACR_CFG
      ACR PLL configuration
     */
    vRegWrite4B(APLL0, 0x21003680);

    /* int output polarity# */
    vRegWrite1B(INTR_STATE0_0, 0x2);//	vRegWrite1B(INTR_MASK1_1,0x2);

    /* set key mask to zero */
    vRegWrite1B(EPST_3, 0x00);
    vRegWrite1B(KS_MASK_0, 0x00);

    //Enable Audio SPDIF Output
    vRegWrite1B(AUDRX_CTRL_1, 0x1d);

    // KSCL_H
    vRegWrite4BMsk(VID_SET, Fld2Msk32(KSCL_H), Fld2Msk32(KSCL_H));

    // HDCP Keymask
    vRegWrite1B(EPST_3, _wHDMI_HDCP_MASk1);		//vRegWrite1B(EPST_3,0xff);
    vRegWrite1B(KS_MASK_0, _wHDMI_HDCP_MASk2);	//vRegWrite1B(KS_MASK_0,0xc3);

    // setting to avoid TMDS reset -> vHDMITMDSReset
    vRegWrite4BMsk(AUDP_STAT, Fld2Msk32(BYP_SYNC ) | Fld2Msk32(BYP_DVIFILT ) , Fld2Msk32(BYP_SYNC ) | Fld2Msk32(BYP_DVIFILT ));

    // Clear Audio mute
    vRegWrite4B(AUDP_STAT, (u4RegRead4B(AUDP_STAT) & (~MT53XX_AUD_MUTE)));

    // increase the PCCNT resolution  from 128 to 1024 by adam
    vRegWrite4BMsk(VID_CRC_OUT, Fld2Msk32(XCLK_IN_PCLK_SEL), Fld2Msk32(XCLK_IN_PCLK_SEL));

    //decode di_DE and vi_DE by both preamble and guard-band
    vRegWrite4BMsk(MUTE_SET, 0x0, Fld2Msk32(BYPASS_DI_GB));

    //modify 1080p twinkling dot
    //CKGEN_WRITE32(REG_RGBCLK_CFG, CKGEN_READ32(REG_RGBCLK_CFG) |RGB_INV_SEL );
    vIO32WriteFldAlign(CKGEN_RGBCLK_CFG, 1, FLD_RGB_INV_SEL);

    //deep color mode must refer output video clock not TMDS clock
    vRegWriteFldAlign(VID_CRC_OUT, 0x1, XCLK_IN_DPCLK);
    vRegWrite4B(DIGITAL_CKDT, 0x102b1b38);

    //enable  HW GAMUT packet decoder
    vRegWriteFldAlign(N_HDMI_CTRL, 0x1, TT0_GAMUT_EN);

#if Enable_TDFIFO_RESET
    vRegWriteFldAlign(MUTE_SET, 0x1, TDFIFO_SYNC_EN);
#endif

#if Enable_HW_Mute
    vRegWriteFldAlign(VID_VRES, 0x1, VRES_MUTE_AUTO_CLR);
    /*disable external 27MHZ clk input when no signal, XBOX issue for plug/unplug */
    vIO32WriteFldAlign(CKGEN_MISC_CKCFG, 0x0, FLD_HDMI_CLK_Crystral);

    vRegWriteFldAlign(VID_HRES, 0x3, HCHG_CNT_THR);
    vRegWriteFldAlign(VID_HRES, 0xf, HSTB_CNT_THR);
    vRegWriteFldAlign(VID_VRES, 0x3, VSTB_CNT_THR);
    vRegWriteFldAlign(VID_SET, 0x1, MUTE_CKDT);
    vRegWriteFldAlign(VID_HRES, 0x4, HRES_THR); //for Iris project sony DSC T500 and Foxconn project  Setop Box model¡GScienfitic Atlanta 8300HDC flash use
    vRegWriteFldAlign(VID_VRES, 0x2, VCHG_CNT_THR);//for TG 35 ,when change Vtotal in 1080i and 480i, HW auto mute enable and doesn't unmute
#endif

    //vIO32WriteFldAlign(HDCP_DEV,0x1,HDCP_SRAM_SW_RST);
    //vIO32WriteFldAlign(HDCP_DEV,0x0,HDCP_SRAM_SW_RST);

    //if (DRVCUST_OptGet(eHDMISupportExtSwitch) == 1)
    //{
    vHDMISwitchInit();
    //}
    UNUSED(DRVCUST_OptQuery(eHDMIOffOnMuteCount, &_wHDMI_OFFON_MUTE_COUNT));
    UNUSED(DRVCUST_OptQuery(eDVIWaitStableCount, &_wDVI_WAIT_STABLE_COUNT));
    UNUSED(DRVCUST_OptQuery(eHDMIBypassInitialFlow, &_wHDMIBypassFlag));
    UNUSED(DRVCUST_OptQuery(eDVIWaitNosignalCount, &_wDVI_WAIT_NOSIGNAL_COUNT));
    UNUSED(DRVCUST_OptQuery(eDVIWaitSCDTStableCount, &_wHDMI_WAIT_SCDT_STABLE_COUNT));
    vRegWriteFldAlign(INTR_MASK0, 0x1, NEW_ACP_ONLY);

    //initial DDC line Schmitt trigger
    vRegWriteFldAlign(DDC_0, 0x202020, DDC_32);
    vRegWriteFldAlign(DDC_12, 0x20202020, DDC_32);

#ifdef CC_SUPPORT_HDMI_FBOOTING
    u1CrntSrc = bApiEepromReadByte(EEP_HDMI_CRNT_SOURCE);
    u1CrntPort= bApiEepromReadByte(EEP_HDMI_CRNT_PORT) + 1;

     if (u1CrntSrc == DRV_CUSTOM_MISC_CRNT_INP_SRC_HDMI)
     {
         vHDMISetSwitch(u1CrntPort);        
     }
     else
     {
         u1CrntPort = 0;
     }
     
#ifndef __HDMI_SLT__
     for (i = 1;i <= 4;i++)
     {
         if (i != u1CrntPort)
         {
             vHDMIHPDHigh(i);
         }
     }
#endif
     
     
#else           
    
#ifndef __HDMI_SLT__
    for (i = 1;i <= 4;i++)
    {
        vHDMIHPDHigh(i);
    }
#endif

#endif
       
#if ReHPD
    _bCKDTcnt = 0;
#endif

    //#if IC_5365_VERIFY
#ifdef CC_MT5365
    vIO32WriteFldAlign(0xf000d158, 0x1, Fld(1, 7, AC_MSKB0));
#endif
#ifdef CC_MT5395
    vIO32WriteFldAlign(0xf000d150, 0x1, Fld(1, 7, AC_MSKB0));
#endif
    vIO32WriteFldAlign(0xf000d134, 0x0, Fld(1, 3, AC_MSKB0));//feedback post divider set to 0
    vIO32WriteFldAlign(0xf000d134, 0x0, Fld(1, 31, AC_MSKB3));
    vIO32WriteFldAlign(0xf000d134, 0x1, Fld(1, 30, AC_MSKB3));
    vIO32WriteFldAlign(0xf000d134, 0x1, Fld(1, 16, AC_MSKB2));
    //vHDMI_INTERNAL_EDID_Init();


    vRegWriteFldAlign(VID_HRES, 0x1, VRES_CHG_DET_SEL);//detect both progressive and interlace input v resolution change
    vRegWriteFldAlign(VID_HRES, 0x1, INTERLACE_DET_SEL);//If this bit set to 1, Interlace out status is assert when the fallowing condition meets:1. 0 < line difference between 2 field < INTERLACE_LINE_DIFF_TH
    vRegWriteFldAlign(VID_BLANK, 0x2, INTERLACE_LINE_DIFF_TH);//for INTERLACE_DET_SEL
    /*enable I2C deglitch*/
    vIO32WriteFldAlign(PDWNC_EDID_DEV0, 1, FLD_DEGLITCH0_EN);
    vIO32WriteFldAlign(PDWNC_EDID_DEV0, 3, FLD_DE_CNT0);
    vIO32WriteFldAlign(PDWNC_EDID_DEV1, 1, FLD_DEGLITCH1_EN);
    vIO32WriteFldAlign(PDWNC_EDID_DEV1, 3, FLD_DE_CNT1);
    vIO32WriteFldAlign(PDWNC_EDID_DEV2, 1, FLD_DEGLITCH2_EN);
    vIO32WriteFldAlign(PDWNC_EDID_DEV2, 3, FLD_DE_CNT2);
    vIO32WriteFldAlign(PDWNC_EDID_DEV3, 1, FLD_DEGLITCH3_EN);
    vIO32WriteFldAlign(PDWNC_EDID_DEV3, 3, FLD_DE_CNT3);

#if Support_3D
    fpDrvRegisterTDTVModeChangeCB(E_TDTV_CB_HDMI, (vTDTVModeChangeCB)vDviTDTVCB);

    vRegWriteFldAlign(N_HDMI_CTRL, 0x1, TT2_EXT_PKT_EN);
    vRegWriteFldAlign(N_HDMI_CTRL3, 0x81, EXT_PKT_DEC);
#endif
    vRegWriteFldAlign(ANA_INTF_3, 0, RG_HDMI_LFRCTL_SW);//for HD860 flash black mute/snow noise/not support.
    vRegWriteFldAlign(ANA_INTF_3, 0x6, RG_HDMI_CHPP);

    vRegWriteFldAlign(VID_SET, 0, CEA_NEW_UNREC_SEL);
    vRegWriteFldAlign(INTR_MASK0, 1, NEW_UNREC_ONLY);//interrupt at every 3D inforframe.

#ifdef CC_SUPPORT_HDMI_FBOOTING
    vRegWriteFldAlign(SYS_00, 1, HDMI_RST); //hdmi core reset
    vRegWriteFldAlign(SYS_00, 0, HDMI_RST);
    u1HdmiInit = 1;	
#endif

	#ifdef CC_AUD_HDMI_SPDIF_CFG
	vRegWriteFldAlign(N_HDMI_CTRL3, 0x1, ACP_CLR_EN);
	vRegWriteFldAlign(N_HDMI_CTRL3, 0x2, ACP_VS_SET); // clear ACP,spec is 600ms, set 2 is 16 x 22E28 bit[11:10] + 16=3x16=48frame , 48x16ms=768ms
	#endif
}

/**
 * @brief HDMI Power Off function
 * @param None
 * @retval None
 */
void vHDMIPowerOff(void)
{
    if (_bHDMIState == HDMI_STATE_NOTREADY)
    {
        return;
    }

    _bHDMIState = HDMI_STATE_INIT;

    vHDMIHPDLow(_bHDMICurrSwitch);

    vRegWriteFldAlign(ANA_INTF_1, 0, RG_HDMI_TERM_EN);
    //vRegWriteFldAlign(ANA_INTF_2,0x0,RG_HDMI_SW_EN);
    //vHDMITMDSIndepCTRL(0);
}


/**
 * @brief 	Update HDMI Audio Parameter
 * @param	None
 * @retval	None
 */
void vHDMIUpdateAudParm(void)
{
    AIN_CFG_T rAudCfg;

    // Load audio configuration
    UNUSED(AUD_GetAinCfg(AUD_STREAM_FROM_HDMI, &rAudCfg));

    //To set bit number
    if (RegReadFldAlign(CHST1, AUD_LENGTH_MAX))		//max 24bits mode
    {
        switch (RegReadFldAlign(CHST1, AUD_LENGTH))
        {
            case 0x01:	//20bit
                rAudCfg.eBits = DAC_20_BIT;
                break;
            default:		//24bit
                rAudCfg.eBits = DAC_24_BIT;
                break;
        }
    }
    else					//max 20bits mode
    {
        switch (RegReadFldAlign(CHST1, AUD_LENGTH))
        {
            case 0x01:	//16bit
                rAudCfg.eBits = DAC_16_BIT;
                break;
            case 0x02:	//18bit
                rAudCfg.eBits = DAC_18_BIT;
                break;
            default:		//20bit
                rAudCfg.eBits = DAC_20_BIT;
                break;
        }
    }

    AUD_AinCfg(AUD_STREAM_FROM_HDMI, (const AIN_CFG_T * )&rAudCfg);
}

/**
 * @brief 	HDMI Mode video setting
 * @param	None
 * @retval	None
 */
PRIVATE void vHDMIVideoHdmiSetting(void)
{
    UINT8 bReadData;

#if HDMI_DEBUG
    //LOG(6, "vHDMIVideoHdmiSetting\n");
#endif
    //LOG(3, "Check NO_AVI = %d\n",u1RegRead1B(INTR_MASK0_0)&0x10);
    //LOG(3, "color space  = %d\n",u1RegRead1B(AVIRX1_0)&0x60);
    /* HDMI mode */

    // Check NO_AVI

    bReadData = u1RegRead1B(INTR_STATE1_0);
    if (bReadData&0x10)
    {
        vRegWrite1B(INTR_STATE1_0, bReadData);
        return;
    }

#if IC_3x3color_test

    vRegWrite1B(VID_MODE_0, 0x00);
    vRegWrite1B(VID_MODE_1, 0x00);

    // check AVI infoframe packet type code
    if (u1RegRead1B(AVIRX0_0) != 0x82)
    {
        // default: RGB
        //vRegWrite1B(VID_MODE_0,0x00);
        //vRegWrite1B(VID_MODE_1,0x00);
        //vRegWrite1B(VID_MODE_2,0x08);// RGB to YCbCr
        vRegWriteFldAlign(VID_MODE, 0x0, ENSYNCCODES);
        if (fgIsMainDVI())
        {
#if CCIR_decode_444
            vRegWriteFldAlign(CCIR_00, 1, CCH_SRC24_SEL);
            vRegWriteFldAlign(CCIR_00, 1, CCH_CBCR_SW);
            vRegWriteFldAlign(OMUX_00, 0x5, OMUX_MAIN_SEL);
#else
            vRegWriteFldAlign(OMUX_00, 0x6, OMUX_MAIN_SEL);
#endif
        }
        //else if(fgIsPipDVI())
        if (fgIsPipDVI())
        {
#if CCIR_decode_444
            vRegWriteFldAlign(CCIR_00, 1, CCH_SRC24_SEL);
            vRegWriteFldAlign(CCIR_00, 1, CCH_CBCR_SW);
            vRegWriteFldAlign(OMUX_00, 0x5, OMUX_PIP_SEL);
#else
            vRegWriteFldAlign(OMUX_00, 0x6, OMUX_PIP_SEL);
#endif
        }
        vRegWriteFldAlign(VID_MODE, 0x00, ENUPSAMPLE);
        //vRegWrite1B(VID_MODE_3, 0x10);//B
        //vRegWrite1B(VID_BLANK_0, 0x10);//G
        //vRegWrite1B(VID_BLANK_1, 0x10);//R
        vRegWrite1B(VID_MODE_3, 0x00);//B
        vRegWrite1B(VID_BLANK_0, 0x00);//G
        vRegWrite1B(VID_BLANK_1, 0x00);//R

        return;
    }
    /*
    AVI DBYTE1
    [6:5] Y1 Y0
    	00 - RGB
    	01 - YCbCr 422
    	10 - YCbCr 444
    */
    bReadData = u1RegRead1B(AVIRX1_0);
    if ((bReadData & 0x60) == 0x00) // RGB
    {
        /*
        AVI DBYTE2
        [7:6] C1 C0
        	00 - No Data
        	01 - ITU601
        	10 - ITU709
        */
        bReadData = u1RegRead1B(AVIRX1_1);

        /* RGB to YCbCr Space Convert: 1=BT709, 0=BT601) */
        //vRegWrite1B(VID_MODE_0,((bReadData&0xc0)==0x40) ? 0x00 : 0x01);
        //vRegWrite1B(VID_MODE_1,0x00);
        // RGB to YCBCr Space Convert
        //vRegWrite1B(VID_MODE_2,0x08); // RGB to YCbCr
        vRegWriteFldAlign(VID_MODE, 0x0, ENSYNCCODES);
        if (fgIsMainDVI())
        {
#if CCIR_decode_444
            vRegWriteFldAlign(CCIR_00, 1, CCH_SRC24_SEL);
            vRegWriteFldAlign(CCIR_00, 1, CCH_CBCR_SW);
            vRegWriteFldAlign(OMUX_00, 0x5, OMUX_MAIN_SEL);
#else
            vRegWriteFldAlign(OMUX_00, 0x6, OMUX_MAIN_SEL);
#endif
        }
        //else if(fgIsPipDVI())
        if (fgIsPipDVI())
        {
#if CCIR_decode_444
            vRegWriteFldAlign(CCIR_00, 1, CCH_SRC24_SEL);
            vRegWriteFldAlign(CCIR_00, 1, CCH_CBCR_SW);
            vRegWriteFldAlign(OMUX_00, 0x5, OMUX_PIP_SEL);
#else
            vRegWriteFldAlign(OMUX_00, 0x6, OMUX_PIP_SEL);
#endif
        }
        vRegWriteFldAlign(VID_MODE, 0x00, ENUPSAMPLE);
	  if ((u1RegRead1B(AVIRX1_2)  & 0x0c) == 0x04)   // Limited Range: 16 ~ 235.
        {
	      vRegWrite1B(VID_MODE_3, 0x10);
            vRegWrite1B(VID_BLANK_0, 0x10);
            vRegWrite1B(VID_BLANK_1, 0x10);
        }
        else if ((u1RegRead1B(AVIRX1_2)  & 0x0c) == 0x08)  // Full range: 0 ~ 255
        {
	      vRegWrite1B(VID_MODE_3, 0x00);
            vRegWrite1B(VID_BLANK_0, 0x00);
            vRegWrite1B(VID_BLANK_1, 0x00);
        }
        

    }
    else // YCbCR
    {
        //vRegWrite1B(VID_MODE_0,0x00);
        //vRegWrite1B(VID_MODE_1,0x00);
        // if 4:2:2 then do 4:2:2 to 4:4:4 up sample
        //vRegWriteFldAlign(VID_MODE, ((bReadData&0x60)==0x20) ? 0x1: 0x00, ENUPSAMPLE);
        vRegWriteFldAlign(VID_MODE, 0x00, ENUPSAMPLE);
        if ((u1RegRead1B (AVIRX1_0)&0x60) == 0x20 )	//422 is from HDMI to CCIR decoder
        {
            //vRegWriteFldAlign(VID_MODE, 0x1, ENSYNCCODES);
            if (fgIsMainDVI())
            {
#if CCIR_decode_444
                vRegWriteFldAlign(CCIR_00, 0, CCH_SRC24_SEL);
                vRegWriteFldAlign(CCIR_00, 0, CCH_CBCR_SW);
#endif
                vRegWriteFldAlign(OMUX_00, 0x5, OMUX_MAIN_SEL);
            }
            //	else if(fgIsPipDVI())
            if (fgIsPipDVI())
            {
#if CCIR_decode_444
                vRegWriteFldAlign(CCIR_00, 0, CCH_SRC24_SEL);
                vRegWriteFldAlign(CCIR_00, 0, CCH_CBCR_SW);
#endif
                vRegWriteFldAlign(OMUX_00, 0x5, OMUX_PIP_SEL);
            }
        }
        else		//444
        {
            //vRegWriteFldAlign(VID_MODE, 0x0, ENSYNCCODES);
            if (fgIsMainDVI())
            {
#if CCIR_decode_444
                vRegWriteFldAlign(CCIR_00, 1, CCH_SRC24_SEL);
                vRegWriteFldAlign(CCIR_00, 1, CCH_CBCR_SW);
                vRegWriteFldAlign(OMUX_00, 0x5, OMUX_MAIN_SEL);
#else
                vRegWriteFldAlign(OMUX_00, 0x6, OMUX_MAIN_SEL);
#endif
            }
            //	else if(fgIsPipDVI())
            if (fgIsPipDVI())
            {
#if CCIR_decode_444
                vRegWriteFldAlign(CCIR_00, 1, CCH_SRC24_SEL);
                vRegWriteFldAlign(CCIR_00, 1, CCH_CBCR_SW);
                vRegWriteFldAlign(OMUX_00, 0x5, OMUX_PIP_SEL);
#else
                vRegWriteFldAlign(OMUX_00, 0x6, OMUX_PIP_SEL);
#endif
            }
        }
        //vRegWrite1B(VID_MODE_2,((bReadData&0x60)==0x20) ? 0x04 : 0x00);
        vRegWrite1B(VID_MODE_3, 0x80);// Cr
        vRegWrite1B(VID_BLANK_1, 0x80);// Cb
        vRegWrite1B(VID_BLANK_0, 0x10);// Y
    }
#endif
}


void vHDMITMDSIndepCTRL(UINT8 bOnOff)
{

    // PD_TERM: Enable TMDS-PHY termination 50-ohm resistance
    if (bOnOff)
    {
        /*turn on TMDS*/
        vRegWrite1B(PD_SYS_2, 0xff);
        vRegWriteFldAlign(ANA_INTF_1, 0x1, RG_HDMI_TERM_EN);
        //vRegWrite1B(SYS_CTRL_1, 0x95);
        vHDMIDDCBusCTRL(TRUE);
    }
#ifndef __HDMI_SLT__
    else
    {
        /* turn off TMDS*/
        vRegWrite1B(PD_SYS_2, 0xcf);
        vRegWriteFldAlign(ANA_INTF_1, 0x0, RG_HDMI_TERM_EN);
        //vRegWrite1B(SYS_CTRL_1, 0x85);
        vHDMIDDCBusCTRL(FALSE);
    }
#endif
}

static void vLogHdmiStateChange(UINT8 u1HdmiState)
{
    static UINT8 u1HdmiStateOld = 0;

    // Check if the state is correct
    if (u1HdmiState <= HDMI_STATE_AUTH)
    {
        if (u1HdmiStateOld != u1HdmiState && u1HdmiStateOld <= HDMI_STATE_AUTH)
        {
            LOG(6,"_bHDMIState change from %s to %s\n", _aszHdmiState[u1HdmiStateOld], _aszHdmiState[u1HdmiState]);
            u1HdmiStateOld = u1HdmiState;
        }
    }
    else // Undefined state
    {
        if (u1HdmiStateOld != u1HdmiState && u1HdmiStateOld <= HDMI_STATE_AUTH)
        {
            LOG(6,"_bHDMIState change from %s to UNDEFINED_STATE\n", _aszHdmiState[u1HdmiStateOld]);
            u1HdmiStateOld = HDMI_STATE_NOTREADY;
        }
    }

    // Suppress warning messages for release version
    UNUSED(_aszHdmiState);
}

void vHDMISetColorRalated(void)
{
    UINT8 bWriteData;
    UINT8 bReadData;

    /* VIDEO - check hdmi mode */
    //if (fgIsHdmiFlgSet(HDMI_MODE_CHG))
    {
        if (_bHdmiMode)
        {
            /* HDMI mode */

            /* Video Setting */
            vHDMIVideoHdmiSetting();

#if 1
            /* 2x pixel clock setting*/
            bReadData = u1RegRead1B(AVIRX2_0) & 0xf;
            if (bReadData != 0)
            {
                bWriteData = 0x17;
            }
            else
            {
                bWriteData = 0x07;
            }
            vRegWrite1B(SYS_CTRL_0, bWriteData);// ICLK
#endif
        }
        else
        {
            /* DVI mode */
#if HDMI_DEBUG
            // LOG(6, "DVI mode\n");
#endif
            vRegWrite1B(SYS_CTRL_0, 0x07);// ICLK
            // ToDo - use VRes to detect HDTV input.
            //bReadData =u1RegRead1B(AVIRX1_1);
            //vRegWrite1B(VID_MODE_0,((bReadData&0xc0)==0x40) ? 0x00 : 0x01);
            vRegWrite1B(VID_MODE_1, 0x0);
            // RGB to YCBCr Space Convert
            //vRegWrite1B(VID_MODE_2,fgIsVideoTiming(_bDviTiming) ? 0x08 : 0x00);// video timing first (customized)
            vRegWrite1B(VID_MODE_3, 0x10);
            vRegWrite1B(VID_BLANK_0, 0x10);
            vRegWrite1B(VID_BLANK_1, 0x10);
            vRegWriteFldAlign(VID_MODE, 0x0, ENSYNCCODES);
            if (fgIsMainDVI())
            {
#if CCIR_decode_444
                vRegWriteFldAlign(CCIR_00, 1, CCH_SRC24_SEL);
                vRegWriteFldAlign(CCIR_00, 1, CCH_CBCR_SW);
                vRegWriteFldAlign(OMUX_00, 0x5, OMUX_MAIN_SEL);
#else
                vRegWriteFldAlign(OMUX_00, 0x6, OMUX_MAIN_SEL);
#endif
            }
            //else if(fgIsPipDVI())
            if (fgIsPipDVI())
            {
#if CCIR_decode_444
                vRegWriteFldAlign(CCIR_00, 1, CCH_SRC24_SEL);
                vRegWriteFldAlign(CCIR_00, 1, CCH_CBCR_SW);
                vRegWriteFldAlign(OMUX_00, 0x5, OMUX_PIP_SEL);
#else
                vRegWriteFldAlign(OMUX_00, 0x6, OMUX_PIP_SEL);
#endif
            }
        }

        // vClrHdmiFlg(HDMI_MODE_CHG);
    }
}



UINT32 vGetHDMIEQStatus(void)
{
#if 0
    UINT32 btmp, bAverage;
    UINT16 j;

    for (bAverage = 0, j = 0; j < 64; j++)	//average
    {
        vUtDelay2us(33);		//4096/2.5/25M
        btmp = RegReadFldAlign(HDMI_SPH, STA_HDMI_SPH);
        bAverage += btmp;
    }

    return(bAverage);
#endif
    return(0);
}



UINT8 vHDMIVResCHG(void)
{
    static UINT16 HDMIVRes, tmp;

    tmp =
        (RegReadFldAlign (VID_VRES, VID_VRES_11_8) << 8) +
        RegReadFldAlign (VID_VRES, VID_VRES_7_0);

    if ((tmp > HDMIVRes + 2) || (tmp < HDMIVRes - 2))	//for interlace mode
    {
        HDMIVRes = tmp;
        return(1);
    }
    else
    {
        return(0);
    }

}
UINT8 vHDMIClockCHG(void)
{
    static UINT16 HDMIClock, tmp;

    tmp = (RegReadFldAlign (VID_CRC_OUT, AAC_XCLK_IN_PCLK_10_8) << 8) + \
          RegReadFldAlign (VID_CRC_OUT, AAC_XCLK_IN_PCLK_7_0);

    if (tmp <= 1)					// 0xff + 1 = 0x100
        tmp = tmp + 0x100;

    if ((tmp > HDMIClock + 2) || (tmp < HDMIClock - 2))
    {
        HDMIClock = tmp;
        return(1);
    }
    else
    {
        return (0);
    }
}

UINT8 bHDMIMCMHdcpWrite(void)
{
    return 1;
}

UINT8 vHDMIHResCHG(void)
{
    static UINT16 HDMIHRes = 0, tmp;

    tmp = RegReadFldAlign(VID_HRES, VID_HRES_12_0);
    if ((tmp > HDMIHRes + 6) || (tmp < HDMIHRes - 6))
    {
        HDMIHRes = tmp;
        return(1);
    }
    else
    {
        return(0);
    }

}

typedef enum
{
	PQL_HDMI_RGB = 0,
	PQL_HDMI_YCBCR,
	PQL_HDMI_XVYCC,
	PQL_DVI_RGB,
	PQL_HDMI_UNKNOWN,
}PQL_HDMI_COLOR_DOMAIN_T;

PQL_HDMI_COLOR_DOMAIN_T bGetHDMIColorDomain(void)
{
	
    if (_bHdmiMode)
    {
		UINT8 bReadData;
		if (u1RegRead1B(AVIRX0_0) != 0x82)
        {
            return PQL_HDMI_RGB;
        }
        if (RegReadFldAlign(INTR_STATE0, INTR3_NEW_AVI))
        {
            _bNEW_AVI_Info = 1;
            bReadData = u1RegRead1B(AVIRX1_0);
            vRegWrite4B(INTR_STATE0, 0x1 << 24);
            _bAVIInfo_tmp = bReadData;
        }
        else
        {
            bReadData = _bAVIInfo_tmp;
        }

        if ((bReadData & 0x60) == 0x00) // RGB
        {
        	return PQL_HDMI_RGB;
        }
        else if((bReadData & 0x60)==0x60)  // xvYCC
        {
			bReadData=u1RegRead1B(AVIRX1_2);
			if((bReadData&0x07)<2)
			{
				return PQL_HDMI_XVYCC;
			}
			else
			{
				return PQL_HDMI_UNKNOWN;
			}
        }
		else  // YCBCR
        {
        	return PQL_HDMI_YCBCR;
		}
    }
    else
    {
    	return PQL_DVI_RGB;
    }
}


static HDMI_3D_INFOFRAME info_3Ddata;
HDMI_3D_INFOFRAME* API_HDMI_3D_Get_InfoFrame(void)
{
    HDMI_3D_INFOFRAME* info = &info_3Ddata;

	if (!_fgForce3D)
	{
	    if ((u1RegRead1B(UNRECRX0_0) == 0x81) && (1 == RegReadFldAlign(INTR_STATE0, INTR3_NEW_UNREC)))
	    {
	        if ((u1RegRead1B(UNRECRX1_3) >> 5) == 0x2)
	        {
	            //#ifndef CC_SCPOS_3DTV_SUPPORT
	               // #if SUPPORT_SBS_CONVERT   
	                //info->HDMI_3D_Enable = 0;//force decoder send 2d information 
	               // #else
	                //info->HDMI_3D_Enable = 1;
	                //#endif
	            //#else
	            info->HDMI_3D_Enable = 1;
	            //#endif
	            info->HDMI_3D_Video_Format = u1RegRead1B(UNRECRX1_3) >> 5;
	            info->HDMI_3D_Structure = u1RegRead1B(UNRECRX2_0) >> 4;
	            if (info->HDMI_3D_Structure >= HDMI_3D_Structure_SideBySideHalf)
	            {
	                info->HDMI_3D_EXTDATA = u1RegRead1B(UNRECRX2_1) >> 4;
	            }
	            else
	            {
	                info->HDMI_3D_EXTDATA = 0;
	            }
	        }
	        else
	        {
	            info->HDMI_3D_Enable = 0;
	        }
	    }
	    else
	    {
	        info->HDMI_3D_Enable = 0;
	    }
	}
	else  // force 3D
	{
		info->HDMI_3D_Enable = 1;
		info->HDMI_3D_Video_Format = 0x2;
		info->HDMI_3D_Structure = _bForce3DStructure;
		if (info->HDMI_3D_Structure >= HDMI_3D_Structure_SideBySideHalf)
	    {
	        info->HDMI_3D_EXTDATA = _bForce3DExtData;
	    }
	    else
	    {
	        info->HDMI_3D_EXTDATA = 0;
	    }
	}
    return info;
}

static HDMI_INFOFRAME_DESCRIPTION info_data;
HDMI_INFOFRAME_DESCRIPTION* API_HDMI_Get_InfoFrame(UINT8 frame)
{
    UINT8 i;
    UINT32 u2startaddr=0;
    HDMI_INFOFRAME_DESCRIPTION* info = &info_data;
    switch (frame)
    {
        case HDMI_InfoFrame_AVI:
            u2startaddr = AVIRX0;
            info->framesize = 5;
            break;
        case HDMI_InfoFrame_SPD:
            u2startaddr = SPDRX0;
            info->framesize = 8;
            break;
        case HDMI_InfoFrame_AUDIO:
            u2startaddr = AUDRX0;
            info->framesize = 4;
            break;
        case HDMI_InfoFrame_MPEG:
            u2startaddr = MPEGRX0;
            info->framesize = 8;
            break;
        case HDMI_InfoFrame_UNREC:
            u2startaddr = UNRECRX0;
            info->framesize = 8;
            break;
        case HDMI_InfoFrame_GAMUT:
            u2startaddr = GAMUTRX0;
            info->framesize = 8;
            break;
        case HDMI_InfoFrame_ACP:
		    info->framesize = 8;
		    if (RegReadFldAlign(INTR_STATE1, INTR6_NEW_ACP))
		    {
				vRegWrite4B(INTR_STATE1, Fld2Msk32(INTR6_NEW_ACP));
				u2startaddr = ACPRX0;
			}
			else
			{
				u2startaddr = 0;
			}
            break;
	  case HDMI_InfoFrame_VSDB:
		info->framesize = 8;
		if(bHDMI3DPacketVaild() == 0) //input 2D mode
		{
			u2startaddr = 0;
		}
		else
		{
			u2startaddr = UNRECRX0;
		}
            break;
        default:
            u2startaddr = AVIRX0;
            info->framesize = 5;
            break;

    }

    LOG(10, "HDMIInfoFrame size =%d\n", info->framesize);
    for (i = 0;i < (info->framesize);i++)
    {
      if (i < 10)
      {
	  if (u2startaddr == 0)
	  {
		info->framedata[i] = 0;
	  }
	  else
	  {
        info->framedata[i] = u4RegRead4B(u2startaddr + (i * 4));
	  }
        LOG(10, "HDMIInfoFrame=%x\n", info->framedata[i]);
      }
      else
      {
      	break;	
      }

    }

    return info;
}
UINT32 wHDMIInfoFrame[10];
void  vHDMIGetInfoFrame(UINT8 frame)
{
    UINT8 num, i;
    UINT32 startaddr=0;
    switch (frame)
    {
        case HDMI_InfoFrame_AVI:
            startaddr = AVIRX0;
            num = 5;
            break;
        case HDMI_InfoFrame_SPD:
            startaddr = SPDRX0;
            num = 8;
            break;
        case HDMI_InfoFrame_AUDIO:
            startaddr = AUDRX0;
            num = 4;
            break;
        case HDMI_InfoFrame_MPEG:
            startaddr = MPEGRX0;
            num = 8;
            break;
        case HDMI_InfoFrame_UNREC:
            startaddr = UNRECRX0;
            num = 8;
            break;
        case HDMI_InfoFrame_GAMUT:
            startaddr = GAMUTRX0;
            num = 8;
            break;
        case HDMI_InfoFrame_ACP:
            startaddr = ACPRX0;
            num = 8;
            break;
        default:
            startaddr = AVIRX0;
            num = 5;
            break;

    }

    for (i = 0;i < num;i++)
    {
        if (i < 10)
        {
        	wHDMIInfoFrame[i] = u4RegRead4B(startaddr + (i * 4));
        	LOG(6, "HDMIInfoFrame=%x\n", wHDMIInfoFrame[i]);
      	}
      	else
      	{
      		break;
      	}
    }
}
UINT16 wHDMIInfoFrameMask(void)
{
    UINT16 PacketMask;
    if (_bNEW_AVI_Info)
    {
        _bNEW_AVI_Info = 0;
        PacketMask = ((u1RegRead1B(INTR_STATE0_3) & 0x1f) | 0x1) | (RegReadFldAlign(INTR_STATE1, INTR7_GAMUT) << HDMI_InfoFrame_GAMUT);
    }
    else
    {
        PacketMask = (u1RegRead1B(INTR_STATE0_3) & 0x1f) | (RegReadFldAlign(INTR_STATE1, INTR7_GAMUT) << HDMI_InfoFrame_GAMUT) ;
    }
    vRegWrite4B(INTR_STATE0, 0x1e << 24);
    vRegWrite4B(INTR_STATE1, 0x1 << 26);//vRegWrite4B(N_HDMI_CTRL,0x1<<2);

    return PacketMask;
}
UINT8 bHDMIDeepColorStatus(void)
{
    return RegReadFldAlign(SRST, DEEP_STA);
}

/**
 * @brief 	vXpcCount
 * @param	None
 * @retval	None
 */
static void vXpcStableCount(void)
{
    if (RegReadFldAlign (SRST, SCDT) && \
            ((_bHDMIState == HDMI_STATE_SCDT) || (_bHDMIState == HDMI_STATE_AUTH)))
    {

        u4CurXpcCnt = wHDMIXPCCNT();

        // CKDT stable counting
        if (RANGE_CHECKING(u4CurXpcCnt, u4PreXpcCnt, 3))
        {

            if (_bXpcStableCnt < 255) _bXpcStableCnt++;
        }
        else
        {

            if (_bXpcStableCnt == 255)
            {
                _bXpcStableCnt--;
            }
            else
            {
                _bXpcStableCnt = 0;
            }
        }

        u4PreXpcCnt = u4CurXpcCnt;
    }
}

/**
 * @brief 	_bIsXpcStable
 * @param	None
 * @retval	1: stable, 0: unstable.
 */
UINT8 _bIsXpcStable(void)
{
    return ((_bXpcStableCnt > HDMI_XPC_STABLE_CNT) ? 1 : 0);
}

static UINT8 _u8SkipHpd = 0xff;
extern UINT8   _bIntMuteCnt  ;
extern UINT8   _bAdoFlowFlg  ;
/**
 * @brief 	HDMI Mainloop
 * @param	None
 * @retval	None
 */
void vHDMIMainLoop(void)
{
    //UINT8 bReadData;//,bPHYCount;
    //UINT8 bI2CWriteData;
    UINT32 i4_ret, dReadData;
    static UINT8 dReadData1 = 0xFF, dReadHDCPChangFlag = 0;
    static UINT8 bInitHDCP = 0;
    //static UINT8 bResChange = 0;
    HAL_TIME_T rDelta;
#ifndef CC_Support_Sil9285
    HAL_TIME_T rTimeTmp;
#endif
#ifdef CC_SUPPORT_HDMI_CLK5VDET
    HAL_TIME_T rTtmp;
    UINT32 _wHpdCnt;
    UINT32 _wDeltaCur;
#endif

#ifdef CC_SUPPORT_HDMI_FBOOTING
    if (_u4HdmiLoops < 500)
    {
        _u4HdmiLoops++; 
        if ((_u4HdmiLoops % 30) == 0)
        {
            LOG(6,"## _u4HdmiLoops %d\n",_u4HdmiLoops);
        }
    }
#endif
    //UINT8 bPWR5V_RX1_Status;

    if (bInitHDCP == 0)
    {
        bInitHDCP++;
        return;
    }
    else if (bInitHDCP == 1)
    {
        // load HDCP key form EEP to SRAM
        bInitHDCP++;
        i4_ret = (UINT32)EEPHDCP_Init();	//need to modify ??
        if (i4_ret != 0)
        {
            LOG(6, "fail on load HDCP\n");
        }
        UNUSED(EEPHDCP_SetSramDevice(1, 0xA0));
        vHDMIHDCPRst();
        return;
    }

//	if(bHDMIInputType() && fgIsVideoTiming(_bDviTiming))//RGB
//		vRegWriteFldAlign(VID_CH_MAP,0x4,CHANNEL_MAP);
//	else
    vRegWriteFldAlign(VID_CH_MAP, 0x0, CHANNEL_MAP);

#ifndef CC_SUPPORT_HDMI_FBOOTING
    if (!fgIsExtSrcHDMI())
    {
        return;
    }
#endif

    if ((_bHdmiCmd == 6) || (_bHdmiCmd == 7) || (_bHdmiCmd == 0x40))
    {
        return;
    }
    //_bHdmiCmd = 0;

    if (_bHDMIConnectFlag)
    {
        _bHDMIConnectFlag = 0;
        _bNodifyConnect = 1;
        if (fgIsMainDVI())
        {
            //vApiNotifySignalDetcet(SV_VP_MAIN);
        }
        if (fgIsPipDVI())
        {
            //vApiNotifySignalDetcet(SV_VP_PIP);
        }

    }

#if HDMI_Audio_NewFlow
    vHDMIAudErrorHandler();
#endif
    //read state
    //	bit0 SCDT
    //	bit1 CKDT
    //	bit2 VSYNC
    //	bit3 PWR5V
#if defined(CC_Support_Sil9285)
    //vHDMISwitchMainLoop();
    if (!( RegReadFldAlign(SRST, CKDTIN_HDMI) || RegReadFldAlign(SRST, CKDT) ))
    {
        if (_bHDMIState == HDMI_STATE_PWOFF)
        {
            return;
        }
        else
        {
#if HDMI_DEBUG
            LOG(6, "HDMI_STATE_INIT #1\n");
#endif
            _bHDMIState = HDMI_STATE_INIT;
        }
    }
    else
    {
        if (_bHDMIState == HDMI_STATE_PWOFF)
        {
#if HDMI_DEBUG
            LOG(6, "HDMI_STATE_PWON\n");
#endif
            _bHDMIState = HDMI_STATE_AUTH;
        }
    }

#elif defined(CC_SUPPORT_HDMI_CLK5VDET)

    // check 5V status
    if (bHDMIPort5VStatus(_bHDMICurrSwitch))
    {
        if (_b5VDetCnt < 6)
        {
            _b5VDetCnt++;
        }
        else
        {
            _b5VActive = 1;
            _bNonStdDev = 0;
        }
    }
    else
    {
        if (_b5VDetCnt > 5)
        {
            _b5VDetCnt--;
        }
        else
        {
            _b5VActive = 0;
        }
    }


    // check Sync status
    if (RegReadFldAlign(SRST, SCDT))
    {
        if (_bTmdsDetCnt < 180)
        {
            _bTmdsDetCnt++;
        }

        if (_bTmdsDetCnt > 10)
        {
            if (_b5VActive == 0)
            {
                _bNonStdDev = 1;
            }

            _bTmdsActive = 1;
        }
    }
    else
    {
        if (_bTmdsDetCnt > 0)
        {
            _bTmdsDetCnt--;
        }
        else
        {
            _bTmdsActive = 0;
            _bNonStdDev = 0;
        }
    }

    // HPD pulse
    if (_bHpdPulse == 0)
    {
        HAL_GetTime(&rTtmp);
        HAL_GetDeltaTime(&rDelta, &_rHdmiUnplugTime, &rTtmp);
        _wDeltaCur = (1000000 * rDelta.u4Seconds) + rDelta.u4Micros;

        if (_b5VActive == 0)
        {
            _wHpdCnt = 2000000;  //us
        }
        else
        {
            _wHpdCnt = _wHDMI_OFFON_MUTE_COUNT * 1000;
        }

        if (_wDeltaCur > _wHpdCnt)
        {
            //vRegWrite1B(SYS_CTRL_1, 0x95);
            vHDMIDDCBusCTRL(TRUE);
            vHDMITMDSCTRL(TRUE);
            vHDMIHPDHigh(_bHDMICurrSwitch);
            _bHpdPulse = 1;
        }
        else
        {
            return;
        }

    }

    if (_b5VActive || ((_bNonStdDev == 1) && _bTmdsActive))
    {
        if (_bHDMIState == HDMI_STATE_PWOFF)
        {
            LOG(6, "HDMI_STATE_PWON\n");
            _bHDMIState = HDMI_STATE_PWON;
            _bHDMIConnectFlag = 1;
            _bUnplugFlag = 0;
        }
    }
    else
    {
        if (_bHDMIState == HDMI_STATE_PWOFF)
        {
            return;
        }
        else
        {
            LOG(6, "HDMI_STATE_INIT #1\n");
            _bHDMIState = HDMI_STATE_INIT;
            _bHDMIConnectFlag = 0;
            _bUnplugFlag = 1;
        }

    }


#else

//-----------------------------------5V detect ----------------------------------------
    if (((_bHDMICurrSwitch == HDMI_SWITCH_1) && !(bHDMIPort5VStatus(HDMI_SWITCH_1))) ||
            ((_bHDMICurrSwitch == HDMI_SWITCH_2) && !(bHDMIPort5VStatus(HDMI_SWITCH_2)))  ||
            ((_bHDMICurrSwitch == HDMI_SWITCH_3) && !(bHDMIPort5VStatus(HDMI_SWITCH_3)))  ||
            ((_bHDMICurrSwitch == HDMI_SWITCH_4) && !(bHDMIPort5VStatus(HDMI_SWITCH_4))))
    {

        if (_bHDMIState == HDMI_STATE_PWOFF)
        {
            if (_u8SkipHpd == 0)
            {
                HAL_GetTime(&rTimeTmp);
                HAL_GetDeltaTime(&rDelta, &_rHdmiUnplugTime, &rTimeTmp);
                if (rDelta.u4Seconds > 1)
                {
                    _u8SkipHpd = 1;
                }
            }
            return;
        }
        else
        {
#if HDMI_DEBUG
            LOG(6, "HDMI_STATE_INIT #1\n");
#endif
            _bHDMIState = HDMI_STATE_INIT;
            _bUnplugFlag = 1;
        }

    }
    else
    {
        if (_bHDMIState == HDMI_STATE_PWOFF)
        {
#if HDMI_DEBUG
            LOG(6, "HDMI_STATE_PWON\n");
#endif
            _bHDMIState = HDMI_STATE_PWON;
            _bHDMIConnectFlag = 1;
        }
    }

#endif

    /*---------CKDT monitor----------------*/
    if (!RegReadFldAlign(SRST, CKDT))
    {
        _bCKDTMonitor++;
        if (_bCKDTMonitor >= 3)
        {
            _bCKDTMonitor = 3;
        }
    }
    else
    {
#if !Support_HW_Adaptive_EQ
        //if(_bFixedEQReset!=2)
        if (_bFixedEQReset == 0)
        {
            //_bFixedEQReset++;
            _bFixedEQReset = 1;
            bHDMIPHYReset(HDMI_RST_FIXEQ);
        }
#endif
        if (_bCKDTMonitor == 3)
        {
            bHDMIPHYReset(HDMI_RST_ALL);
            LOG(3, "HDMI Analog PHY Reset ........\n");
        }
        _bCKDTMonitor = 0;
    }

//--------------------------------------------------------------------------

    vXpcStableCount();
    vLogHdmiStateChange(_bHDMIState);
    switch (_bHDMIState)
    {
        case HDMI_STATE_INIT:
#ifdef CC_Support_Sil9285
            vHDMITMDSCTRL(TRUE);
#else

#if HPD_5V
            {
#ifdef __HDMI_SLT__
                //vRegWrite1B(SYS_CTRL_1, 0x95);//enable DDCDLY DDC_EN RX_EN
                vHDMIDDCBusCTRL(TRUE);
                vHDMIHPDLow(_bHDMICurrSwitch);
                vHDMITMDSCTRL(TRUE);
#else
                vHDMIHPDLow(_bHDMICurrSwitch);
                vHDMITMDSCTRL(FALSE);
                //vRegWrite1B(SYS_CTRL_1, 0x85);//disable DDCDLY DDC_EN RX_EN
                vHDMIDDCBusCTRL(FALSE);
#endif

            }
#else
            {
                _bCKDTcnt = 0;
            }
#endif
#endif
            //vRegWrite1B(SRST_1,0x0b);//mark by ciwu
            //vRegWrite1B(SRST_1,0x00);//mark by ciwu
            //vIO32WriteFldAlign(HDCP_DEV,0x1,HDCP_SRAM_SW_RST);
            //vIO32WriteFldAlign(HDCP_DEV,0x0,HDCP_SRAM_SW_RST);
            //vIO32WriteFldAlign((IO_VIRT + 0x22400), 0x1, Fld(1, 5, AC_MSKB0));//HDMI reset
            //vIO32WriteFldAlign((IO_VIRT + 0x22400), 0x0, Fld(1, 5, AC_MSKB0));
            vRegWriteFldAlign(SYS_00, 1, HDMI_RST); //hdmi core reset
            vRegWriteFldAlign(SYS_00, 0, HDMI_RST);

            //vHDMITMDSReset();
#if MUTE_TEMP
            vHDMIVideoOutOff();
            // bHDMIdelayTDMSReset = 0;
#endif
            _bHdmiPwOnDelay = 0;
            _bHdmiPlug = 0;
            _bHdmiLowPlug = 0;
            _bHPDCycle = 0;
#if ATC_720p_fix
            TMDS_delay = 0;
#endif
            dReadHDCPChangFlag = 0;
#if !PATCH_PIONEER_DV696_IR_OFF_ON_PURPLE_AND_SNOW_NOISE
            _bPreHPDLowDelay = 0;
#endif

#ifdef CC_SUPPORT_HDMI_FBOOTING
            _u1HdmiSrcIn = 0;
            _u1HdcpGood = 0;
#endif

            vHDMIMuteAudio();
            vHDMIAudConfig();

            // Toggle TMDS_SRS_SEL bit 0 (PCLK_INV) to force HDMI output audio MCLK
            // This workaround is for MT537X that have clock switching problem
            //vHDMIApllClkReset();
            _u8SkipHpd = 0;
            HAL_GetTime(&_rHdmiUnplugTime);

            _bHDMIState = HDMI_STATE_PWOFF;

#if defined(CC_SUPPORT_HDMI_CLK5VDET)
            _bHpdPulse = 0;
            vRegWriteFldAlign(TMDS_CTRL0, 1, HDMI_RTCK_AUTO);
#endif

#if HDMI_DEBUG
            LOG(6, "HDMI_STATE_PWOFF\n");
#endif
            break;

        case HDMI_STATE_PWOFF:
            break;
        case HDMI_STATE_PWON:

#if defined(CC_SUPPORT_HDMI_CLK5VDET)
            _bHDMIState = HDMI_STATE_PWON2;
#else
            vRegWriteFldAlign(TMDS_CTRL0, 1, C_DATA_SYNC_AUTO);
#if !PATCH_PIONEER_DV696_IR_OFF_ON_PURPLE_AND_SNOW_NOISE
            if (_bPreHPDLowDelay++ > 5)
#endif
            {

#if !HPD_5V
                {
#ifndef __HDMI_SLT__    // always keep HPD high
                    vHDMIHPDLow(_bHDMICurrSwitch);//add by ciwu
                    vHDMITMDSCTRL(FALSE);//modify by ciwu
                    //vRegWrite1B(SYS_CTRL_1, 0x85);//disable DDCDLY DDC_EN RX_EN
                    vHDMIDDCBusCTRL(FALSE);
                    vHDMITMDSReset();
#endif
                }
#endif
                HAL_TIME_T rCurHPDLowTime;
                HAL_GetTime(&rCurHPDLowTime);
                if (!_bHdmiLowPlug)
                {

                    _bHdmiLowPlug = 1;
                    HAL_GetTime(&_rHdmiLowPlugWaitTime);
                }
                HAL_GetDeltaTime(&rDelta, &_rHdmiLowPlugWaitTime, &rCurHPDLowTime);

#ifdef CC_SUPPORT_HDMI_FBOOTING
  		        if(((1000000*rDelta.u4Seconds)+rDelta.u4Micros)>((300)*1000)) // 300ms
  		        {
                    if (_u1HdmiPwrOn == 1)
                    {
                        _u1HdmiPwrOn = 0;
                        _u8SkipHpd = 1;
                    }
  		        }

  		        if(((1000000*rDelta.u4Seconds)+rDelta.u4Micros)>((200)*1000)) // 200ms
				{
 	  			   vHDMITMDSCTRL(TRUE);
	  	        }

#else

                if (((1000000*rDelta.u4Seconds) + rDelta.u4Micros) > ((330)*1000)) // 340ms
                {
                    vHDMITMDSCTRL(TRUE);
                }
#endif

                if ((((1000000*rDelta.u4Seconds) + rDelta.u4Micros) > ((_wHDMI_OFFON_MUTE_COUNT + 0)*1000)) || (_u8SkipHpd == 1))
                {
                    //vRegWrite1B(SYS_CTRL_1, 0x95);// enable DDCDLY DDC_EN RX_EN
                    vHDMIDDCBusCTRL(TRUE);
                    vHDMITMDSCTRL(TRUE);
                    _bHdmiPwOnDelay = 0;
                    _bHPDdelay = 0;
                    _bHDMIState = HDMI_STATE_PWON2;
                }
            }
#if HDMI_DEBUG
            LOG(6, "HDMI_STATE_PWON\n");
#endif
            vHDMIMuteAudio();
#endif
            break;
        case HDMI_STATE_PWON2:
            //vRegWrite1B(SYS_CTRL_1,0x95);// enable DDCDLY DDC_EN RX_EN
            //vHDMITMDSCTRL(TRUE);
#ifndef __HDMI_SLT__
            vHDMIHPDHigh(_bHDMICurrSwitch);
#endif
            _bEQFlag = 0;
#if !HPD_5V
            {
                if (RegReadFldAlign(SRST, CKDTIN_HDMI) || RegReadFldAlign(SRST, CKDT) )
                {
                    _bHDMIState = HDMI_STATE_SCDT;
                    _bHPDdelay = 0;
                }
                else if (_bHPDdelay++ == HDMI_HPD_ONDELAY)
                {
                    _bHPDdelay = 0;
                    _bHDMIState = HDMI_STATE_SCDT;
                }
            }
#else
            {
                _bHDMIState = HDMI_STATE_SCDT;
            }
#endif

#if HDMI_DEBUG
            LOG(6, "HDMI_STATE_PWON2\n");
#endif
            break;

        case HDMI_STATE_SCDT:
            _bHPDCycle = 1;
#if 0//#if ATC_720p_fix
            // for ATC test
            vRegWriteFldAlign(TMDS_CTRL0, 0,  TMDS_SP_RST_);
            vRegWriteFldAlign(TMDS_CTRL0, 1,  TMDS_SP_RST_);
            if (TMDS_delay++ > 2)//default is 2
#endif
            {
                dReadData = u4RegRead4B(SRST);
                if (RegReadFldAlign(SRST, SCDT))
                {
#if !HPD_5V
                    {
                        _bCKDTcnt = 0;
                    }
#endif
                    HAL_TIME_T rCurTime;
                    HAL_GetTime(&rCurTime);
                    vRegWrite4B(INTR_STATE0, 0x1 << 16);
                    if (!_bHdmiPlug)
                    {

                        _bHdmiPlug = 1;
                        HAL_GetTime(&_rHdmiPlugWaitTime);
                    }


                    dReadData = u1RegRead1B(HDCP_STAT_2) & 0x30; //00: No HDCP; 01: Authenticating; 11: AUTH Done

                    if (dReadData1 != dReadData)
                    {
                        dReadData1 = dReadData;
                        HAL_GetTime(&_rHdmiPlugWaitTime);
                    }

                    if (dReadData == 0x30)
                    {
                        dReadHDCPChangFlag++;
#ifdef CC_SUPPORT_HDMI_FBOOTING
					if (_u1HdcpGood < 255) _u1HdcpGood++;
#endif					
				}
#ifdef CC_SUPPORT_HDMI_FBOOTING
				else
				{
				    _u1HdcpGood = 0;
                    }
                vHDMIAudConfig();
         		vHDMIMuteAudio();
#endif

                    //if (((rCurTime.u4Seconds - _rHdmiPlugWaitTime.u4Seconds) > _wHDMI_WAIT_SCDT_STABLE_COUNT)	 || (dReadData==0x30) )
                    if (((rCurTime.u4Seconds - _rHdmiPlugWaitTime.u4Seconds) > (_wHDMI_WAIT_SCDT_STABLE_COUNT))	 || (dReadHDCPChangFlag == 30) )
                    {
#if HDMI_DEBUG
                        LOG(6, "HDMI_STATE_AUTH\n");
#endif
                        _bSCDTdelay = 0;
                        _bHDMIState = HDMI_STATE_AUTH;

#ifdef CC_SUPPORT_HDMI_FBOOTING
                _u1HdmiSrcIn = 1;
#endif
					

#if 1
                        //when 3x3 matrix is RGB but player output is Ycbcr, AVI infofrmae has changed  to Ycbcr but FW doesn't change
                        if (u1RegRead1B(AVIRX0_0) == 0x82)
                        {
                            _bAVIInfo_tmp = u1RegRead1B(AVIRX1_0);
                            vRegWrite4B(INTR_STATE0, 0x1 << 24);
                            _bNEW_AVI_Info = 1;
                        }
#endif
                    }
                }

            }
            break;

        case HDMI_STATE_AUTH:

#ifdef CC_SUPPORT_HDMI_FBOOTING

              if (_bDviModeChged)
              {
                  vHDMIMuteAudio();
              }
              
			  dReadData = u1RegRead1B(HDCP_STAT_2) & 0x30; //00: No HDCP; 01: Authenticating; 11: AUTH Done			
			  if ((dReadData == 0x30) && (_u1HdcpGood < 255))
			  {
			      _u1HdcpGood++;
			  }
#endif

			
#if ATC_720p_fix
            TMDS_delay = 0;
#endif

            if (!(RegReadFldAlign(SRST, SCDT)))
            {
                if (_bSCDTdelay++ > 30)
                {
#if HDMI_DEBUG
                    LOG(6, "SCDT =%x\n", RegReadFldAlign(SRST, SCDT));
                    LOG(6, "HDMI_STATE_SCDT #2\n");
#endif
                    dReadHDCPChangFlag = 0;
                    _bHDMIState = HDMI_STATE_SCDT;
#ifdef CC_SUPPORT_HDMI_FBOOTING
                _u1HdmiSrcIn = 0;
                _u1HdcpGood = 0;
                _u4HdmiLoops = 0;
                vHDMIMuteAudio();
#endif
                }
                #ifdef CC_SUPPORT_HDMI_FBOOTING
                // mtk70117 add for cr DTV00089486 begin
                else if(_bSCDTdelay>=3)
                {
                		_u1HdcpGood = 0;
                }
                // mtk70117 add for cr DTV00089486 end
                #endif
                break;
            }
            else
            {
                _bSCDTdelay = 0;
            }
            _bEQFlag = 0;

            vHDMIVideoOutOn();
            // Check HDMI Mode
            _bHdmiMode = u1RegRead1B(AUDP_STAT_0) & 0x1;
            if (!_bHdmiMode && RegReadFldAlign(AUDP_STAT, HDMI_MUTE))
            {
                vRegWriteFldAlign(SRST, 1,  SW_RST);
                vRegWriteFldAlign(SRST, 0,  SW_RST);
            }

            //if ((u1RegRead1B(ACPRX0_0) == 0x4 ) && (u1RegRead1B(ACPRX0_1) > 1) )
            if (u1RegRead1B(ACPRX0_0) == 0x4)
            {
                //LOG(3, " INTR6_NEW_ACP=%d\n",RegReadFldAlign(INTR_STATE1,INTR6_NEW_ACP));
                if (RegReadFldAlign(INTR_STATE1, INTR6_NEW_ACP))
                {
                    _bACPCount = 0;
                    vRegWrite4B(INTR_STATE1, Fld2Msk32(INTR6_NEW_ACP));
                    //LOG(3, " INTR6_NEW_ACP in mute=%d\n",RegReadFldAlign(INTR_STATE1,INTR6_NEW_ACP));
			 if (u1RegRead1B(ACPRX0_1) > 1)
			 {
	                    //vHDMIMuteAudio();
	                    vCloseIec();
	                    //LOG(3, "ACP >=2 ,Mute now\n");
			 }
			 else if (!u1RegRead1B(ACPRX0_1))
			 {
				vOpenIec();
			 }
                }
                else
                {
                    _bACPCount++;
                }
                //LOG(3, " _bACPCount=%d\n",_bACPCount);
                if (_bACPCount > 50 )
                {
                    if(_AUD_IsSpdifTypeOff())
                    {
                        vOpenIec();
                    }
                    _bACPCount = 60;
                }

            }
            if (_bHdmiMode)
            {
                // Check audio underrun & overrun
                vHDMIHandleAudFifoFault();
                //_bACPCount=0;

                // Move to bHDMIAudStatus() to avoid race condition between audio/hdmi thread
                //LOG(3, "ACP <2 ,UNMute now\n");
            }


            vHDMISetColorRalated();
            break;

        default:
            _bHDMIState = HDMI_STATE_INIT;
            break;
    }

}


#if 1
/**
 * @brief Enable HDMI Video Output
 * @param none
 * @retval none
 */
void vHDMIVideoOutOn(void)
{
    //_fgVideoOn = TRUE;
    //return;
    // UINT8 bI2CWriteData;

    if (_bHDMIState == HDMI_STATE_NOTREADY)
    {
        return;
    }

    _fgVideoOn = TRUE;
}

BOOL fgHDMIHsyncAct(void)
{
#if CHECHCKDT
    if ((u1RegRead1B(SRST_2) & 0x01) == 0x01)//modify by ciwu
#else
    if (((u1RegRead1B(SRST_2) & 0x01) == 0x01) && _fgVideoOn == TRUE)
#endif
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**
 * @brief	Disable HDMI Video Output
 * @param	None
 * @retval	None
 */
void vHDMIVideoOutOff(void)
{
    //_fgVideoOn = TRUE;
    //return;

    // UINT8 bI2CWriteData;

    if (_bHDMIState == HDMI_STATE_NOTREADY)
    {
        return;
    }

    LOG(8, "Block Video\n");
    _fgVideoOn = FALSE;
}
#endif

/**
 * @brief		Enable HDMI Video Output
 * @param	None
 * @retval	None
 */
void vHDMIAudioOutOn(void)
{
    //UINT8 bI2CWriteData;

    if (_bHDMIState == HDMI_STATE_NOTREADY)
    {
        return;
    }

    // Open APLL
    vRegWrite4BMsk(PD_SYS, Fld2Msk32(PD_APLL) | Fld2Msk32(PD_AO), Fld2Msk32(PD_APLL) | Fld2Msk32(PD_AO));

#if 0//SUPPORT_DTV5351_LOOSELY
#if FIX_DIGITAL_AUDIO_CLK_TRISTATE
    //Turn on MCLK  by Adam 2005/9/26 for DTV
    if (BSP_GetIcVersion() == IC_VER_AA)
    {
        vRegWrite4BMsk(I2S_CTRL, 0, Fld2Msk32(SD1_EN));
        vRegWrite4BMsk(I2S_CTRL, Fld2Msk32(SD3_EN), Fld2Msk32(SD3_EN));
    }
    else if (BSP_GetIcVersion() >= IC_VER_AB)
    {
        vRegWrite4BMsk(I2S_CTRL, Fld2Msk32(SD1_EN) | Fld2Msk32(SD3_EN), Fld2Msk32(SD1_EN) | Fld2Msk32(SD3_EN));
    }
    //End by Adam
#endif
#endif
    vSetHdmiFlg(HDMI_AUDIO_ON);
}

/**
 * @brief
 * @param	None
 * @retval	None
 */
void vHDMIAudioOutOff(void)
{
    //UINT8 bI2CWriteData;

    if (_bHDMIState == HDMI_STATE_NOTREADY)
    {
        return;
    }

    //LOG(9, "HDMI AudioOut Off\n");

    //bI2CWriteData = bHDMIReadDevH(0x3f);
    //vHDMIWriteDevH(0x3F, (bI2CWriteData&0x7f));
#if 0//SUPPORT_DTV5351_LOOSELY
#if FIX_DIGITAL_AUDIO_CLK_TRISTATE
    //tri-state MCLK  by Adam 2005/9/26 fort DTV
    //bI2CWriteData = bHDMIReadDevH(HDMIH_I2S_CTRL2);
    //vHDMIWriteDevH(HDMIH_I2S_CTRL2, (bI2CWriteData & 0xF7)); // clear MCLKEN bit
    //End by Adam
#endif
#endif
    vClrHdmiFlg(HDMI_AUDIO_ON);

    vHDMIMuteAudio();

}

#if 0//INFORM_MDCHG
BOOL fgHDMIQueryModeChange(void)
{
    if (bHDMIMCCnt > 0)
    {
        bHDMIMCCnt = 0;
        return 1;
    }
    return 0;
}
#endif

/**
 * HDMI CRC check
 *
 * @NOTE units test: fgHDMICRC(1); under interlaced signal
 * @NOTE units test: fgHDMICRC(100); under interlaced signal
 */
BOOL fgHDMICRC(INT16 ntry)
{
    UINT8 idx;
    UINT8 result[2][3];
    UINT8 tmp[3];

    idx = 0;
    result[0][0] = 0;
    result[0][1] = 0;
    result[0][2] = 0;
    result[1][0] = 0;
    result[1][1] = 0;
    result[1][2] = 0;

    LOG(6, "fgHDMICRC: %d\n", ntry);

    while (ntry > 0)
    {
        ntry--;
        vUtDelay10ms(1); // NOTE: IT IS NECESSARY
        if (idx > 2)
        {
            LOG(6, "CRC fail\n");
            return 0;
        }
        vRegWrite1B(VID_CRC_CHK_2, 0x8c);// clr
        LOG(6, "CRC clear status\n");

        while (RegReadFldAlign(VID_CRC_CHK, STA_CRC_RDY) != 0x0)
        {
            vUtDelay1ms(1);

        }
        while (u1RegRead1B(VID_CRC_CHK_3) != 0x00)
        {
            vUtDelay1ms(1);
        }
        while (u1RegRead1B(VID_CRC_CHK_0)  != 0x00)
        {
            vUtDelay1ms(1);
        }
        while (u1RegRead1B(VID_CRC_CHK_1) != 0x00)
        {
            vUtDelay1ms(1);
        }

        if (( u1RegRead1B(VID_CRC_CHK_2)&0x81) == 0x0)
        {

            vRegWrite1B(VID_CRC_CHK_2, 0x0d);// start trigger
            //LOG(6, "CRC start\n");
            //while (u1RegRead1B(VID_CRC_CHK_2)  != 0x8d)
            while (RegReadFldAlign(VID_CRC_CHK, STA_CRC_RDY) != 0x1)
            {
                vUtDelay1ms(1);
                //LOG(6, "delay 1ms for ready bit\n");
            }
            vRegWrite1B(VID_CRC_CHK_2, 0x0c);

            if ( RegReadFldAlign(VID_CRC_CHK, STA_CRC_RDY) == 0x1 )
            {
                //LOG(6, "CRC ready\n");
                tmp[0] = u1RegRead1B(VID_CRC_CHK_3);
                tmp[1] = u1RegRead1B(VID_CRC_CHK_0);
                tmp[2] = u1RegRead1B(VID_CRC_CHK_1) ;
                // vUtDelay10ms(2);
                // compare and update result if necessary
                if ((tmp[0] == result[0][0]) && (tmp[1] == result[0][1]) && (tmp[2] == result[0][2]))
                {
                    continue;
                }
                if ((tmp[0] == result[1][0]) && (tmp[1] == result[1][1]) && (tmp[2] == result[1][2]))
                {
                    continue;
                }
                VERIFY((idx < 3));
                /*lint -e661*/ /*lint -e662*/
                result[idx][0] = tmp[0];
                result[idx][1] = tmp[1];
                result[idx][2] = tmp[2];
                /*lint +e661*/ /*lint +e662*/
                idx++;
                continue;
            }
            else
            {
                LOG(6, "CRC is not ready\n");
                return 0;
            }
        }
        else
        {
            LOG(6, "reset CRC fail");
            return 0;
        }
    }

    if (u1RegRead1B(VID_CH_MAP_1) & 0x04)
    {
        LOG(6, "interlace signal\n");
    }
    else
    {
        LOG(6, "progressive signal\n");
    }

    if (idx == 1)
    {
        LOG(6, "assume progressive signal\n");
        LOG(6, "CRC result:\n");
        LOG(6, "%x %x %x\n", result[0][0], result[0][1], result[0][2]);
    }
    else if (idx == 2)
    {
        LOG(6, "assume interlaced signal\n");
        LOG(6, "CRC result:\n");
        LOG(6, "%x %x %x\n", result[0][0], result[0][1], result[0][2]);
        LOG(6, "%x %x %x\n", result[1][0], result[1][1], result[1][2]);
    }

    return 1;
}

/**
 * DVI CRC check
 *
 * @NOTE units test: fgDVICRC(1); under interlaced signal
 * @NOTE units test: fgDVICRC(100); under interlaced signal
 */
BOOL fgDVICRC(INT16 ntry)
{
    UINT8 idx;
    UINT8 result[2][3];
    UINT8 tmp[3];

    idx = 0;
    result[0][0] = 0;
    result[0][1] = 0;
    result[0][2] = 0;
    result[1][0] = 0;
    result[1][1] = 0;
    result[1][2] = 0;

    LOG(6, "fgDVICRC: %d\n", ntry);

    while (ntry > 0)
    {
        ntry--;
        vUtDelay1ms(15); // NOTE: IT IS NECESSARY
        if (idx > 2)
        {
            LOG(6, "CRC fail\n");
            return 0;
        }

        // calculate CRC

        // select crc path
        if (fgIsMainDVI())
        {
            vRegWriteFldAlign(SYS_04, 0, CRC_SRC_SEL);
        }
        else
        {
            vRegWriteFldAlign(SYS_04, 1, CRC_SRC_SEL);
        }


        // CRC init
        vRegWrite4BMsk(DVI_02, 0x0, Fld2Msk32(DVI_CRC_START) | Fld2Msk32(DVI_CRC_CLR));

        // CRC clear
        vRegWrite4BMsk(DVI_02, Fld2Msk32(DVI_CRC_CLR), Fld2Msk32(DVI_CRC_CLR));
        vRegWrite4BMsk(DVI_02, 0x0, Fld2Msk32(DVI_CRC_CLR));

        while (!(RegReadFldAlign(DVI_HDTV_05, DVI_STA_CRC_OUT_23_16) == 0) && (RegReadFldAlign(DVI_HDTV_05, DVI_STA_CRC_OUT_15_8) == 0) && (RegReadFldAlign(DVI_HDTV_05, DVI_STA_CRC_OUT_7_0) == 0))
        {
            vUtDelay1ms(1);
        }
        // CRC start
        vRegWrite4BMsk(DVI_02, Fld2Msk32(DVI_CRC_START), Fld2Msk32(DVI_CRC_START));
        while (!(RegReadFldAlign(DVI_HDTV_05, DVI_STA_CRC_RDY)) )
        {
            vUtDelay1ms(1);
        }
        // LOG(6, "%x %x %x\n", bReadDirVIN(STA26_2), bReadDirVIN(STA26_1), bReadDirVIN(STA26_0));


        // store CRC result
        tmp[0] = RegReadFldAlign(DVI_HDTV_05, DVI_STA_CRC_OUT_23_16);
        tmp[1] = RegReadFldAlign(DVI_HDTV_05, DVI_STA_CRC_OUT_15_8);
        tmp[2] = RegReadFldAlign(DVI_HDTV_05, DVI_STA_CRC_OUT_7_0);

        // compare and update result if necessary
        if ((tmp[0] == result[0][0]) && (tmp[1] == result[0][1]) && (tmp[2] == result[0][2]))
        {
            continue;
        }
        if ((tmp[0] == result[1][0]) && (tmp[1] == result[1][1]) && (tmp[2] == result[1][2]))
        {
            continue;
        }
        VERIFY(idx < 3);
        /*lint -e661*/ /*lint -e662*/
        result[idx][0] = tmp[0];
        result[idx][1] = tmp[1];
        result[idx][2] = tmp[2];
        /*lint +e661*/ /*lint +e662*/
        idx++;
        continue;
    }

    if (idx == 1)
    {
        LOG(6, "assume progressive signal\n");
        LOG(6, "CRC result:\n");
        LOG(6, "%x %x %x\n", result[0][0], result[0][1], result[0][2]);
    }
    else if (idx == 2)
    {
        LOG(6, "assume interlaced signal\n");
        LOG(6, "CRC result:\n");
        LOG(6, "%x %x %x\n", result[0][0], result[0][1], result[0][2]);
        LOG(6, "%x %x %x\n", result[1][0], result[1][1], result[1][2]);
    }

    return 1;
}
/**
 * HDMI analog PHY CRC check
 *
 * @NOTE units test: fgHDMIPHYCRC(1); under interlaced signal
 * @NOTE units test: fgHDMIPHYCRC(100); under interlaced signal
 */
BOOL fgHDMIPHYCRC(INT16 ntry)
{
    UINT8 idx;
    UINT8 result[2][3];
    UINT8 tmp[3];

    idx = 0;
    result[0][0] = 0;
    result[0][1] = 0;
    result[0][2] = 0;
    result[1][0] = 0;
    result[1][1] = 0;
    result[1][2] = 0;

    LOG(6, "fgHDMIPHYCRC: %d\n", ntry);

    while (ntry > 0)
    {
        ntry--;
        vUtDelay10ms(1); // NOTE: IT IS NECESSARY
        if (idx > 2)
        {
            LOG(6, "HDMI analog PHY CRC fail\n");
            return 0;
        }
        vRegWrite4B(HDMI_SPH, Fld2Msk32(HDMI_PHY_CRC_RDY));
        vRegWriteFldAlign(TMDS_CTRL1, 3, PHY_C_CRC_MODE);
        LOG(6, "HDMI analog PHY CRC clear status\n");

        while (RegReadFldAlign(HDMI_SPH, HDMI_PHY_CRC_RDY) != 0x0)
        {
            vUtDelay1ms(1);

        }
        while (u1RegRead1B(HDMI_SPH_0) != 0x00)
        {
            vUtDelay1ms(1);
        }
        while (u1RegRead1B(HDMI_SPH_1)  != 0x00)
        {
            vUtDelay1ms(1);
        }
        while (u1RegRead1B(HDMI_SPH_2) != 0x00)
        {
            vUtDelay1ms(1);
        }

        if ((RegReadFldAlign(HDMI_SPH, HDMI_PHY_CRC_RDY) == 0x0) && (RegReadFldAlign(TMDS_CTRL1, PHY_C_CRC_START) == 0x0) )
        {

            vRegWrite1B(TMDS_CTRL1_1, 0x07);// start trigger
            //LOG(6, "CRC start\n");
            //while (u1RegRead1B(VID_CRC_CHK_2)  != 0x8d)
            while (RegReadFldAlign(HDMI_SPH, HDMI_PHY_CRC_RDY) != 0x1)
            {
                vUtDelay1ms(1);
                //LOG(6, "delay 1ms for ready bit\n");
            }
            vRegWrite1B(TMDS_CTRL1_1, 0x06);

            if ( RegReadFldAlign(HDMI_SPH, HDMI_PHY_CRC_RDY) == 0x1 )
            {
                //LOG(6, "CRC ready\n");
                tmp[0] = u1RegRead1B(HDMI_SPH_0);
                tmp[1] = u1RegRead1B(HDMI_SPH_1);
                tmp[2] = u1RegRead1B(HDMI_SPH_2) ;
                // vUtDelay10ms(2);
                // compare and update result if necessary
                if ((tmp[0] == result[0][0]) && (tmp[1] == result[0][1]) && (tmp[2] == result[0][2]))
                {
                    continue;
                }
                if ((tmp[0] == result[1][0]) && (tmp[1] == result[1][1]) && (tmp[2] == result[1][2]))
                {
                    continue;
                }
                VERIFY((idx < 3));
                /*lint -e661*/ /*lint -e662*/
                result[idx][0] = tmp[0];
                result[idx][1] = tmp[1];
                result[idx][2] = tmp[2];
                /*lint +e661*/ /*lint +e662*/
                idx++;
                continue;
            }
            else
            {
                LOG(6, "HDMI analog PHY CRC is not ready\n");
                return 0;
            }
        }
        else
        {
            LOG(6, "HDMI analog PHY reset CRC fail");
            return 0;
        }
    }

    if (u1RegRead1B(VID_CH_MAP_1) & 0x04)
    {
        LOG(6, "interlace signal\n");
    }
    else
    {
        LOG(6, "progressive signal\n");
    }

    if (idx == 1)
    {
        LOG(6, "assume progressive signal\n");
        LOG(6, "HDMI analog PHY CRC result:\n");
        LOG(6, "%x %x %x\n", result[0][0], result[0][1], result[0][2]);
    }
    else if (idx == 2)
    {
        LOG(6, "assume interlaced signal\n");
        LOG(6, "HDMI analog PHY CRC result:\n");
        LOG(6, "%x %x %x\n", result[0][0], result[0][1], result[0][2]);
        LOG(6, "%x %x %x\n", result[1][0], result[1][1], result[1][2]);
    }

    return 1;
}

UINT8 bHDMIAVIPixelCount(void)
{

    return (u1RegRead1B(AVIRX2_0)  & 0x0f);
}

UINT16 wHDMIResoWidth(void)
{
    UINT16 tmp;

    //tmp = (((UINT16)u1RegRead1B(VID_BLANK_3) << 8) | ((UINT16)u1RegRead1B(VID_BLANK_2)));
    tmp = ((UINT16)RegReadFldAlign(VID_BLANK, VID_DEPIXELS_11_0));
    if ((u1RegRead1B(SYS_CTRL_0) & 0x30) == 0x10) // ICLK
    {
        tmp <<= 1;
    }

    return tmp;
}

UINT16 wHDMIResoHeight(void)
{
    UINT16 tmp;

    //tmp = ((( (UINT16)u1RegRead1B(VID_STAT_1) & 0x0f) << 8) | (UINT16)u1RegRead1B(VID_STAT_0) );
    tmp = ((UINT16)RegReadFldAlign(VID_STAT, VID_DELINES_11_0));

    return tmp;
}

UINT16 wHDMIHTotal(void)
{
    UINT16 tmp;

    //tmp = ((( (UINT16)u1RegRead1B(VID_HRES_3) & 0x1f) << 8) |  (UINT16)u1RegRead1B(VID_HRES_2));
    tmp = ((UINT16)RegReadFldAlign(VID_HRES, VID_HRES_12_0));
    if ((u1RegRead1B(SYS_CTRL_0) & 0x30) == 0x10) // ICLK
    {
        tmp <<= 1;
    }

    return tmp;
}

UINT16 wHDMIVTotal(void)
{
    UINT16 tmp;

    tmp = ((((UINT16)u1RegRead1B(VID_VRES_1) & 0x0f) << 8) | (UINT16)u1RegRead1B(VID_VRES_0));

    return tmp;
}

#ifdef CC_AUD_HDMI_SPDIF_CFG
UINT8 bHDMIACPType(void)
{
    if ((u1RegRead1B(ACPRX0_0) != 0x4) || (!_bHdmiMode))
    {
        return 0;
    }
    else
    {
        return (u1RegRead1B(ACPRX0_1));
    }
}
UINT8 bHDMIChannelStCategoryCode(void)
{
        return RegReadFldAlign(AUDRX_CTRL,AUD_CH_STAT2);
}

UINT8 bHDMIChannelStCopyright(void)
{
        return RegReadFldAlign(AUDRX_CTRL,AUD_CH_STAT1_2);
}

#endif

UINT8 bHDMIScanInfo(void)
{
    if ((u1RegRead1B(AVIRX0_0) != 0x82) || (!_bHdmiMode))
    {
        return 0;
    }
    else
    {
        return (u1RegRead1B(AVIRX1_0)  & 0x03);
    }

}
UINT8 bHDMIAspectRatio(void)
{
    if ((u1RegRead1B(AVIRX0_0) != 0x82) || (!_bHdmiMode))
    {
        return 0;
    }
    else
    {
        return ((u1RegRead1B(AVIRX1_1)  & 0x30) >> 4);
    }

}
UINT8 bHDMIAFD(void)
{
    if ((u1RegRead1B(AVIRX0_0) != 0x82) || (!_bHdmiMode))
    {
        return 0;
    }
    else
    {
        return (u1RegRead1B(AVIRX1_1)  & 0xf);
    }

}
UINT8 bHDMI422Input(void)
{
    if ((u1RegRead1B(AVIRX0_0) != 0x82) || (!_bHdmiMode))
    {
        return 0;
    }
    else
    {
        if ((u1RegRead1B (AVIRX1_0)&0x60) == 0x20 )
        {
            return 1;
        }
        else
        {
            return 0;
        }

    }

}
UINT8 bHDMIITCFlag(void)
{
    if ((u1RegRead1B(AVIRX0_0) != 0x82) || (!_bHdmiMode))
    {
        return 0;
    }
    else
    {
        if ((u1RegRead1B (AVIRX1_2)&0x80) == 0x80 )
        {
            return 1;
        }
        else
        {
            return 0;
        }

    }

}
UINT8 bHDMIITCContent(void)
{
    if ((u1RegRead1B(AVIRX0_0) != 0x82) || (!_bHdmiMode))
    {
        return 0;
    }
    else
    {
        return (u1RegRead1B (AVIRX2_0)&0x30) >> 4 ;

    }

}
/*
full range: 0 ~ 255.
limited range: 16 ~ 235.
   00: Default, depend on video format.
         PC timing: full range.
         Video timing: limited range.
   01: limited range.
   10: full range.
   11: Reserved.
*/
UINT8 bHDMIRgbRange(void)
{
    UINT8 bMode;

    if ((u1RegRead1B(AVIRX0_0) != 0x82) || (!_bHdmiMode))
    {
        return SV_HDMI_RANGE_FORCE_AUTO;
    }
    else
    {
        bMode = bGetHDMIRangeMode();
        switch (bMode)
        {
            default:
            case SV_HDMI_RANGE_FORCE_AUTO:
                return ((u1RegRead1B(AVIRX1_2) & 0x0c) >> 2);
            case SV_HDMI_RANGE_FORCE_LIMIT:
                return SV_HDMI_RANGE_FORCE_LIMIT;
            case SV_HDMI_RANGE_FORCE_FULL:
                return SV_HDMI_RANGE_FORCE_FULL;
        }
    }
}

UINT8 bGetHDMIRangeMode(void)
{
    return _bHDMIRangeMode;
}

void vSetHDMIRangeMode(UINT8 bMode)
{
    _bHDMIRangeMode = bMode;
}

PRIVATE void fgHDMIAVIFrame(void)
{
    if ((u1RegRead1B(INTR_STATE1_0) & 0x10) == 0x10)
    {
        printf("no avi infoframe\n");
    }
    else
    {
        if (u1RegRead1B(AVIRX0_0) != 0x82)
        {
            printf("invalid avi infoframe\n");
            return;
        }

        LOG(3, "detect avi infoframe\n");
        if ((u1RegRead1B(AVIRX1_0)& 0x60) == 0x00)
        {
            printf("RGB color space\n");
        }
        else if ((u1RegRead1B(AVIRX1_0) & 0x60) == 0x20)
        {
            printf("YCbCr 422 color space\n");
        }
        else if ((u1RegRead1B(AVIRX1_0)  & 0x60) == 0x40)
        {
            printf("YCbCr 444 color space\n");
        }

        if ((u1RegRead1B(AVIRX1_0)  & 0x03) == 0x01)
        {
            printf("Overscanned(TV)\n");
        }
        else if ((u1RegRead1B(AVIRX1_0)  & 0x03) == 0x02)
        {
            printf("Underscanned(PC)\n");
        }

        if ((u1RegRead1B(AVIRX1_1)  & 0xc0) == 0x40)
        {
            printf("ITU 601(SDTV)\n");
        }
        else if ((u1RegRead1B(AVIRX1_1) & 0xc0) == 0x80)
        {
            printf("ITU 709(HDTV)\n");
        }

        if ((u1RegRead1B(AVIRX1_1)  & 0x30) == 0x10)
        {
            printf("4:3\n");
        }
        else if ((u1RegRead1B(AVIRX1_1)  & 0x30) == 0x20)
        {
            printf("16:9\n");
        }

        if ((u1RegRead1B(AVIRX1_2)  & 0x0c) == 0x04)
        {
            printf("Limited Range\n");
        }
        else if ((u1RegRead1B(AVIRX1_2)  & 0x0c) == 0x08)
        {
            printf("Full range\n");
        }

        printf("pixel replication %x\n", u1RegRead1B(AVIRX2_0) );
    }
}

PRIVATE void fgHDMISyncDetect(void)
{
    if (u1RegRead1B(SRST_2)& 0x01)
    {
        LOG(6, "sync detect\n");
    }
    if (u1RegRead1B(SRST_2)& 0x02)
    {
        LOG(6, "clock detect\n");
    }
    if (u1RegRead1B(SRST_2) & 0x04)
    {
        LOG(6, "vsync detect\n");
    }
    if (u1RegRead1B(SRST_2)& 0x08)
    {
        LOG(6, "RX0 pwr5v detect\n");
    }
    if (u1RegRead1B(SRST_2)& 0x40)
    {
        LOG(6, "RX1 pwr5v detect\n");
    }
    if (u1RegRead1B(SRST_2)& 0x80)
    {
        LOG(6, "RX2 pwr5v detect\n");
    }
}

BOOL fgHDMIinterlaced(void)
{
    if ((u1RegRead1B(VID_CH_MAP_1)  & 0x04) == 0x04)
    {
        // LOG(6, "interface signal\n");
        return 1;
    }
    else
    {
        // LOG(6, "progressive signal\n");
        return 0;
    }
}


/**
 * @brief bHDMIRefreshRate
 *
 * return the frame rate.
 * @note range: 49~51 -> 50; 59~61 -> 60
 */
UINT8 bHDMIRefreshRate(void)
{
    UINT32 pfreq;
    UINT8 rate;
    UINT32 dwtmp;
    dwtmp = wHDMIHTotal() * wHDMIVTotal();
    if (dwtmp == 0) // avoid divide by zero
    {
        return 1;
    }

    pfreq = dwHDMIPixelFreq();
    //rate = (pfreq*1000) / dwtmp;
    rate = ((pfreq * 1000) + (dwtmp - 1)) / dwtmp; //modify by ciwu
    // LOG(6, "rate: %d\n", rate);

    if ((rate <= 51) && (rate >= 49))
    {
        rate = 50;
    }
    else if ((rate <= 57) && (rate >= 55))
    {
        rate = 56;
    }
    else if ((rate <= 61) && (rate >= 59))
    {
        rate = 60;
    }
    else if ((rate <= 68) && (rate >= 65))
    {
        rate = 67;
    }
    else if ((rate <= 71) && (rate >= 69))
    {
        rate = 70;
    }
    else if ((rate <= 73) && (rate >= 71))
    {
        rate = 72;
    }
    else if ((rate <= 76) && (rate >= 74))
    {
        rate = 75;
    }
    else if ((rate <= 86) && (rate >= 84))
    {
        rate = 85;
    }
    return rate;
}

UINT32 dwHDMILineFreq(void)
{
    UINT32 ret;
    UINT16 wDiv;

    wDiv = wHDMIHTotal();
    if (wDiv == 0)
    {
        return 1;
    }

    ret = ((dwHDMIPixelFreq() * 10) / (wDiv));

    return ret;
}

PRIVATE void vHDMIHDMIMode(void)
{
    if (u1RegRead1B(AUDP_STAT_0)& 0x01)
    {
        printf("HDMI mode\n");
    }
    else
    {
        printf("DVI mode\n");
    }
}

PRIVATE void vHDMIAVMuteSta(void)
{
    if (u1RegRead1B(AUDP_STAT_0)& 0x04)
    {
        LOG(3, "AV mute\n");
    }
}

PRIVATE void vHDMIPolSta(void)
{
    if (u1RegRead1B(VID_CH_MAP_1) & 0x01)
    {
        LOG(6, "HSYNC Positive Polarity\n");
    }
    else
    {
        LOG(6, "HSYNC Negative Polarity\n");
    }
    if (u1RegRead1B(VID_CH_MAP_1) & 0x02)
    {
        LOG(6, "VSYNC Positive Polarity\n");
    }
    else
    {
        LOG(6, "VSYNC Negative Polarity\n");
    }
}

BOOL fgHDMIHsyncPolarity(void)
{
    if (u1RegRead1B(VID_CH_MAP_1) & 0x01)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

BOOL fgHDMIVsyncPolarity(void)
{
    if (u1RegRead1B(VID_CH_MAP_1)  & 0x02)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * CORE_ISO_EN	Bypass output data from TMDS_CORE.
 */
void vHDMIBypassVdo(void)
{
    // UINT8 bI2CWriteData;

    // CORE_ISO_EN: Bypass output data from TMDS_CORE
    // vHDMIWriteDevH(0x3a, bHDMIReadDevH(0x3a)|0x01);
    // LOG(6, "0x683a[0]\n");

    // VIDEO_MUTE: Video Mute (send blanking value to output)
    // bI2CWriteData = bHDMIReadDevH(0x37);
    // vHDMIWriteDevH(0x37, (bI2CWriteData | 0x01));

    // reset FIFO
    //vRegWrite4BMsk(TMDS_CTRL0, 0x0, Fld2Msk32(PCLK_INV) |Fld2Msk32(TMDS_SP_RST_) |Fld2Msk32(TMDS_RT_RST_) |Fld2Msk32(TMDS_RX_RST_));
    //vRegWrite4BMsk(TMDS_CTRL0,
    // Fld2Msk32(TMDS_SP_RST_) |Fld2Msk32(TMDS_RT_RST_) |Fld2Msk32(TMDS_RX_RST_),
    //  Fld2Msk32(TMDS_SP_RST_) |Fld2Msk32(TMDS_RT_RST_) |Fld2Msk32(TMDS_RX_RST_));

}

PRIVATE void vHDMIColorRelated(void)
{
    if (RegReadFldAlign(VID_MODE, ENDOWNSAMPLE))
    {
        LOG(3, "down sample: 444 to 422\n");
    }
    if (RegReadFldAlign(VID_MODE, ENUPSAMPLE))
    {
        LOG(3, "up sample: 422 to 444\n");
    }

    LOG(3, "no color transform\n");
}

PRIVATE void vHDMIIntSta1(void)
{
    UINT8 sta;

    sta = u1RegRead1B(INTR_STATE0_1);
    UNUSED(sta);

    if (RegReadFldAlign(INTR_STATE0, INTR1_AUTH_DONE))
    {
        LOG(3, "AUTH DONE\n");
    }
    if (RegReadFldAlign(INTR_STATE0, INTR1_AUTH_START))
    {
        LOG(3, "AUTHSTART\n");
    }
    if (RegReadFldAlign(INTR_STATE0, INTR1_BCH_PKT_ERR_ALERT))
    {
        LOG(3, "ECCERR\n");
    }
    if (RegReadFldAlign(INTR_STATE0, INTR1_FIFO_ERR))
    {
        LOG(3, "Audio FIFO error\n");
    }
    if (RegReadFldAlign(INTR_STATE0, INTR1_PLL_UNLOCKED))
    {
        LOG(3, "Audio PLL unlocked\n");
    }
    if (RegReadFldAlign(INTR_STATE0, INTR1_PKT_ERR))
    {
        LOG(3, "Audio N/CTS packet decode error\n");
    }
    if (RegReadFldAlign(INTR_STATE0, INTR1_HW_N_CHG))
    {
        LOG(3, "ACRHWN\n");
    }
    if (RegReadFldAlign(INTR_STATE0, INTR1_HW_CTS_CHG))
    {
        LOG(3, "ACRHWCTS\n");
    }
    vRegWrite4B(INTR_STATE0, 0xff << 8);
}
PRIVATE void vHDMIIntSta2(void)
{
    UINT8 sta;

    sta = u1RegRead1B(INTR_STATE0_2);
    UNUSED(sta);

    if (RegReadFldAlign(INTR_STATE0, INTR2_CLK_CHG))
    {
        LOG(3, "Video Clock Frequency Changed\n");
    }
    if (RegReadFldAlign(INTR_STATE0, INTR2_NEW_AUD_PKT))
    {
        LOG(3, "Received Audio Packet\n");
    }
    if (RegReadFldAlign(INTR_STATE0, INTR2_GOT_CTS))
    {
        LOG(3, "Received CTS Packet\n");
    }
    if (RegReadFldAlign(INTR_STATE0, INTR2_SCDT))
    {
        LOG(3, "Sync Detect change detected\n");
    }
    if (RegReadFldAlign(INTR_STATE0, INTR2_CKDT))
    {
        LOG(3, "Clock Detect change detected\n");
    }
    if (RegReadFldAlign(INTR_STATE0, INTR2_SOFT_INTR_EN))
    {
        LOG(3, "Software-Induced Interrupt\n");
    }
    if (RegReadFldAlign(INTR_STATE0, INTR2_VSYNC))
    {
        LOG(3, "VSYNC Active Edge recognized\n");
    }
    if (RegReadFldAlign(INTR_STATE0, INTR2_HDMI_MODE))
    {
        LOG(3, "HDMI Mode change detected\n");
    }

    vRegWrite4B(INTR_STATE0, 0xff << 16);
}
PRIVATE void vHDMIIntSta3(void)
{
    UINT8 sta;

    sta = u1RegRead1B(INTR_STATE0_3);
    UNUSED(sta);

    if (RegReadFldAlign(INTR_STATE0, INTR3_NEW_AVI))
    {
        LOG(3, "NEW_AVI\n");
    }
    if (RegReadFldAlign(INTR_STATE0, INTR3_NEW_SPD))
    {
        LOG(3, "NEWSP\n");
    }
    if (RegReadFldAlign(INTR_STATE0, INTR3_NEW_AUD))
    {
        LOG(3, "NEW_AUD\n");
    }
    if (RegReadFldAlign(INTR_STATE0, INTR3_NEW_MPEG))
    {
        LOG(3, "NEW_MPEG\n");
    }
    if (RegReadFldAlign(INTR_STATE0, INTR3_NEW_UNREC))
    {
        LOG(3, "NEW_UNR\n");
    }
    if (RegReadFldAlign(INTR_STATE0, INTR3_P_ERR))
    {
        LOG(3, "SPDIFERR\n");
    }
    if (RegReadFldAlign(INTR_STATE0, INTR3_CP_SET_MUTE))
    {
        LOG(3, "SETMUTE\n");
    }
    if (RegReadFldAlign(INTR_STATE0, INTR3_CEA_NEW_CP))
    {
        LOG(3, "NEW_CP\n");
    }

    vRegWrite4B(INTR_STATE0, (UINT32)0xff << 24);
}
PRIVATE void vHDMIIntSta4(void)
{
    UINT8 sta;

    sta = u1RegRead1B(INTR_STATE1_0);

    if (RegReadFldAlign(INTR_STATE1, INTR4_UNDERRUN))
    {
        printf("FIFO Under-Run\n");
    }
    if (RegReadFldAlign(INTR_STATE1, INTR4_OVERRUN))
    {
        printf("FIFO Overrun\n");
    }
    if (RegReadFldAlign(INTR_STATE1, INTR4_CTS_REUSED_ERR))
    {
        LOG(3, "CTS Reused\n");
    }
    if (RegReadFldAlign(INTR_STATE1, INTR4_CTS_DROPPED_ERR))
    {
        LOG(3, "CTS Dropped\n");
    }
    if (RegReadFldAlign(INTR_STATE1, INTR4_NO_AVI))
    {
        LOG(3, "NO AVI Received\n");
    }
    if (RegReadFldAlign(INTR_STATE1, INTR4_T4_PKT_ERR_ALERT))
    {
        LOG(3, "TERC4 Error\n");
    }
    if (RegReadFldAlign(INTR_STATE1, INTR4_HDCP_PKT_ERR_ALERT))
    {
        LOG(3, "HDCP Error\n");
    }
    vRegWrite1B(INTR_STATE1_0, sta);
}
PRIVATE void vHDMIIntSta5(void)
{
    UINT8 sta;

    sta = u1RegRead1B(INTR_STATE1_1);

    /**
     * VRCHG, HRCHG has unstable status, which depending on CSC_R2Y.
     * See P.29
     */
    if (RegReadFldAlign(INTR_STATE1, INTR5_AUD_SAMPLE_F))
    {
        printf("Audio Fs Sample Rate Change\n");
    }
    if (RegReadFldAlign(INTR_STATE1, INTR5_INTERLACEOUT))
    {
        LOG(3, "Interlaced Status Change\n");
    }
    /*
    if (RegReadFldAlign(INTR_MASK1,INTR5_POLCHG))
    {
    	LOG(6, "Palarity Change on VSYNC or HSYNC\n");
    }
    */
    if (RegReadFldAlign(INTR_STATE1, INTR5_HRESCHG))
    {
        printf("Horizontal resolution change\n");
    }
    if (RegReadFldAlign(INTR_STATE1, INTR5_VRESCHG))
    {
        printf("Vertical resolution change\n");
    }
    if (RegReadFldAlign(INTR_STATE1, INTR5_BCH_AUDIO_ALERT))
    {
        LOG(3, "Audio-Related Link Error\n");
    }
    if (RegReadFldAlign(INTR_STATE1, INTR5_AUDIO_MUTE))
    {
        printf("Audio Auto Configure muted ...\n");
    }
    if (RegReadFldAlign(INTR_STATE1, INTR5_FN_CHG))
    {
        printf("Fn Change\n");
    }
    vRegWrite1B(INTR_STATE1_1, sta);
}
PRIVATE void vHDMIIntSta6(void)
{


    if (RegReadFldAlign(INTR_STATE1, INTR6_PWR5V_RX0))
    {
        LOG(3, "Cable1 Unplug Interrupt\n");
    }
    if (RegReadFldAlign(INTR_STATE1, INTR6_PWR5V_RX1))
    {
        LOG(3, "Cable2 Unplug Interrupt\n");
    }
    if (RegReadFldAlign(INTR_STATE1, INTR6_PWR5V_RX2))
    {
        LOG(3, "Cable3 Unplug Interrupt\n");
    }
    if (RegReadFldAlign(INTR_STATE1, INTR6_NEW_ACP))
    {
        LOG(3, "NEW_ACP\n");
    }
    vRegWrite1B(INTR_STATE1_0, u1RegRead1B(INTR_STATE1_0));
}

void vHDMIHDCPKey(void)
{

    LOG(6, "Key Mask %x %x\n", u1RegRead1B(EPST_3),  u1RegRead1B(KS_MASK_0));

    LOG(6, "0x6032: [5]: DECRYPT, [4]: AUTH_EN\n");
    LOG(6, "%x\n", u1RegRead1B(HDCP_STAT_2));

    LOG(6, "BKSV: Receiver's KSV\n");
    LOG(6, "%x\n", u1RegRead1B(SHD_BKSV0_2));
    LOG(6, "%x\n", u1RegRead1B(SHD_BKSV0_3));
    LOG(6, "%x\n", u1RegRead1B(SHD_BKSV1_0));
    LOG(6, "%x\n", u1RegRead1B(SHD_BKSV1_1));
    LOG(6, "%x\n", u1RegRead1B(SHD_BKSV1_2));

    LOG(6, "AKSV: Transmitter's KSV\n");
    LOG(6, "%x\n", u1RegRead1B(SHD_AKSV_1));
    LOG(6, "%x\n", u1RegRead1B(SHD_AKSV_2));
    LOG(6, "%x\n", u1RegRead1B(SHD_AKSV_3));
    LOG(6, "%x\n", u1RegRead1B(SHD_AN0_0));
    LOG(6, "%x\n", u1RegRead1B(SHD_AN0_1));

    LOG(6, "Ri\n");
    LOG(6, "%x\n", u1RegRead1B(SHD_BKSV1_3));
    LOG(6, "%x\n", u1RegRead1B(SHD_AKSV_0));

    LOG(6, "AN\n");
    LOG(6, "%x\n", u1RegRead1B(SHD_AN0_2));
    LOG(6, "%x\n", u1RegRead1B(SHD_AN0_3));
    LOG(6, "%x\n", u1RegRead1B(SHD_AN1_0));
    LOG(6, "%x\n", u1RegRead1B(SHD_AN1_1));
    LOG(6, "%x\n", u1RegRead1B(SHD_AN1_2));
    LOG(6, "%x\n", u1RegRead1B(SHD_AN1_3));
    LOG(6, "%x\n", u1RegRead1B(SHD_BSTATUS_0));
    LOG(6, "%x\n", u1RegRead1B(SHD_BSTATUS_1));
}

void vHDMIReloadHDCPKey(void)
{
    vRegWrite1B(EPST_2, 0x40);
    vRegWrite1B(EPST_1, 0x00);
    vRegWrite1B(EPST_2, 0x20);
    LOG(6, "60fa: %x\n", u1RegRead1B(EPST_2));
}

/**
 * @example: mode=3, vHDMIHDCPSelfBist(3)
 */
void vHDMIHDCPSelfBist(UINT8 mode)
{
    vRegWrite1B(EPST_1, 0x00);
    vRegWrite1B(EPST_2, 0x00);

    LOG(6, "0x60f9: %d\n",  u1RegRead1B(EPST_1));
    vRegWrite1B(EPST_2, ( u1RegRead1B(EPST_2)&0xe0) | mode);
    vUtDelay1s(2);
    LOG(6, "60f9: %x\n", u1RegRead1B(EPST_1)); // Return 1 if hdcp bist is successful
}

/**
 * mode == 3
 */
UINT8 bHDMIHDCPSelfBist(UINT8 mode)
{
    vRegWrite1B(EPST_1, 0x00);
    vRegWrite1B(EPST_2, 0x00);
    //LOG(6, "0x60f9: %d\n", bHDMIReadDevL(0xf9));
    vRegWrite1B(EPST_2, ( u1RegRead1B(EPST_2)&0xe0) | mode);
    vUtDelay1s(2);
    //LOG(6, "60f9: %x\n", bHDMIReadDevL(0xf9)); // Return 1 if hdcp bist is successful
    if (u1RegRead1B(EPST_1) == 0x01)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

PRIVATE void vHDMIIntPattern(void)
{
    LOG(6, "0x6012: [7]:H, [6]:V, [2]:R, [1]:G, [0]:B\n");
}

PRIVATE void vHDMIReset(void)
{
    LOG(6, "0x6005:[0]:SW_RST, 0x5:[1]:FIFO_RST, 0x90:[1]:TMDS_RESETB\n");
    LOG(6, "0x6090:[1]: TMDS_RESETB\n");
}

#if 0
void vHDMIvLoadColorMatrix(BYTE idx)
{
    vLoadColorMatrix(SV_VP_MAIN, COLOR_TRANSFORM[idx]);
}
#endif

PRIVATE void vHDMIQuerybHDMIState(void)
{
    printf("_bHDMIState: %x\n", _bHDMIState);
}

void vHDMISetHDMIState(UINT8 tmp)
{
    _bHDMIState = tmp;
    vHDMIQuerybHDMIState();
}

void vHDMIStopFw(void)
{
    _bHdmiCmd = 6;
}

void vHDMIResumeFw(void)
{
    _bHdmiCmd = 0;
}
/*
MT5387 -> CC_MT5387
MT5363 -> CC_MT5387 and CC_MT5363
MT5365 -> CC_MT5365
MT5395 -> CC_MT5395
*/
void vHDMIDebug(UINT8 cmd, UINT32 value)
{
//UINT32 EQvalue;

    switch (cmd)
    {

            /*1. toggle TDFIFO and RTCKAUTO*/
        case 1:
#if 0
#if defined(CC_MT5387) ||defined(CC_MT5363)
            vRegWriteFldAlign(TMDS_CTRL0, 1, HDMI_RTCK_AUTO);
            vUtDelay2us(1);
            vRegWriteFldAlign(TMDS_CTRL0, 0, HDMI_RTCK_AUTO);
#endif
#endif
            vRegWriteFldAlign(MUTE_SET, 1, TDFIFO_SYNC_EN);
            vRegWriteFldAlign(MUTE_SET, 0, TDFIFO_SYNC_EN);
            break;

            /*2. HDMI reset*/
        case 2:
            vRegWrite1B(SRST_1, 0x0f);
            vRegWrite1B(SRST_1, 0x00);
            break;

            /*3. HDMI Core  reset*/
        case 3:
            vIO32WriteFldAlign((IO_VIRT + 0x22400), 0x1, Fld(1, 5, AC_MSKB0));
            vIO32WriteFldAlign((IO_VIRT + 0x22400), 0x0, Fld(1, 5, AC_MSKB0));
            break;

            /*4. DVI pattern*/
        case 4:
            if (fgIsMainDVI())
                vRegWriteFldAlign(OMUX_00, value, OMUX_MAIN_RAMP_EN);
            if (fgIsPipDVI())
                vRegWriteFldAlign(OMUX_00, value, OMUX_PIP_RAMP_EN);
            break;

            /*5. EQ adjust*/
        case 5:
#if 0
#ifdef CC_MT5387
            vRegWriteFldAlign(TMDS_CTRL1, value, EQSEL);
#endif
#endif
            break;

            /*6. Disable HW auto mute*/
        case 6:
            vRegWriteFldAlign(VID_VRES, 0x1, VRES_MUTE_CLR);
            _bHdmiCmd = 0x20;
            break;

            /*7. Reset audio FIFO*/
        case 7:
            vRegWriteFldAlign(AEC_CTRL, 0, AAC_EN);
            vRegWriteFldAlign(SRST, 1, FIFO_RST);
            vUtDelay2us(3);
            vRegWriteFldAlign(SRST, 0, FIFO_RST);
            vUtDelay2us(500);
            vRegWrite4B(INTR_STATE1, Fld2Msk32(INTR4_OVERRUN) | Fld2Msk32(INTR4_UNDERRUN) | Fld2Msk32(INTR5_AUD_SAMPLE_F));
            vRegWrite4B(INTR_STATE1, Fld2Msk32(INTR5_AUDIO_MUTE));
            vRegWriteFldAlign(AEC_CTRL, 1, AAC_EN);
            vRegWriteFldAlign(AUDP_STAT, 0, AUDIO_MUTE);
            break;

            /*8. Stop DVI detect*/
        case 8:
            _bHdmiCmd = 0x30;
            break;

            /*9. Stop HDMI and DVI detect*/
        case 9:
            _bHdmiCmd = 0x40;
            break;

            /*10. Get EQ setting*/
        case 10:
            vHDMIGetEQSetting();
            break;
                    /*10. Get EQ setting*/
        case 11:
            bHDMIPHYReset(value);
            break;

        default:
            break;
    }
}
void vHDMIDebugMsg(void)
{
    fgHDMISyncDetect();
    printf("width: %d\n", wHDMIResoWidth());
    printf("height: %d\n", wHDMIResoHeight());
    printf("htotal: %d\n", wHDMIHTotal());
    printf("vtotal: %d\n", wHDMIVTotal());
    printf("AVG XPCCNT: %d\n", wHDMIXPCCNT());
    printf("pixel freq: %d\n", dwHDMIPixelFreq());
    printf("line freq: %d\n", dwHDMILineFreq());
    printf("refresh rate: %d\n", bHDMIRefreshRate());
    //LOG(3, ":EQ BOOST %x\n", RegReadFldAlign(TMDS_CTRL1, EQSEL));
    if (fgHDMIinterlaced())
    {
        printf("interface signal\n");
    }
    else
    {
        printf("progressive signal\n");
    }
    vHDMIPolSta();
    vHDMIColorRelated();
    vHDMIHDMIMode();
    fgHDMIAVIFrame();
    vHDMIAVMuteSta();
    vHDMIQuerybHDMIState();

    vHDMIIntPattern();
    vHDMIReset();

    vHDMIIntSta1();
    vHDMIIntSta2();
    vHDMIIntSta3();
    vHDMIIntSta4();
    vHDMIIntSta5();
    vHDMIIntSta6();

    LOG(6, "sw: _bHdmiMode: %d\n", _bHdmiMode);
}


#if HDMI_SELFTESTING
PRIVATE void vHDMICheckSMT(void)
{
    if ((CKGEN_READ32(0x340) & 0x00000180) != 0x00000180)
    {
        LOG(6, "SMT setting error\n");
    }
}

PRIVATE void vHDMICheckI2CPINMUX(void)
{
    // HDCP I2C
    if ((CKGEN_READ32(0x400) & 0x03000000) == 0x01000000)
    {
        LOG(6, "HDCP I2C\n");
    }
    else if ((CKGEN_READ32(0x400) & 0x03000000) == 0x00000000)
    {
        LOG(6, "HDCP GPIO\n");
    }
    // EDID I2C
    if ((CKGEN_READ32(0x400) & 0x00C00000) == 0x00400000)
    {
        LOG(6, "EDID I2C\n");
    }
    else if ((CKGEN_READ32(0x400) & 0x00C00000) == 0x00000000)
    {
        LOG(6, "EDID GPIO\n");
    }
}

PRIVATE void vHDMICheckHDCPSram(void)
{
    // check EN first
    // check content
}

PRIVATE void vHDMICheckDVIClk(void)
{
    // pre_rgb_clk_sel
    if ((CKGEN_READ32(0x2a4) & 0x10000000) != 0x10000000)
    {
        LOG(6, "pre_rgb_clk_sel error\n");
    }
    // rgb_inv_sel
    if ((CKGEN_READ32(0x2a4) & 0x00001000) != 0x00001000)
    {
        LOG(6, "rgb_inv_sel: 1\n");
    }
    else
    {
        LOG(6, "rgb_inv_sel: 0\n");
    }

    if ((u1RegRead1B(TMDS_CTRL0_0) & 0x01) == 0x01)
    {
        LOG(6, "PCLK_INV: 1\n");
    }
    else
    {
        LOG(6, "PCLK_INV: 0\n");
    }
    // rgb_delay_set
    LOG(6, "rgb_delay_set: %x\n", (CKGEN_READ32(0x2a4)&0x0000007f));
    // cch_clk_stop
    if ((CKGEN_READ32(0x29c) & 0x00000020) == 0x00000020)
    {
        LOG(6, "CCH_CLK STOP!\n");
    }
    // dvi_clk_stop
    if ((CKGEN_READ32(0x29c) & 0x00000010) == 0x00000010)
    {
        LOG(6, "DVI_CLK STOP!\n");
    }
    // dvick_in_sel ?

    // 0x6093[6][5][4]
}

PRIVATE void vHDMICheckInterface(void)
{
    // byte order
    // 0x6056
    // bit order
    // 0x6010
}
#endif



/**
 *
 */
BOOL fgHDMISupportAudio(void)
{
    if (_bHdmiMode == 1)
    {
        return 1;
    }

    return 0;
}


/**
 * HDMI audio clock check
 *
 * @retval	TRUE: have audio clock
 * @retval	FALSE: no audio clock
 */
BOOL fgHDMIAudioClkOk(void)
{
    BOOL ret;
    if (u1RegRead1B(AUDP_STAT_0) == 0)
    {
        ret = 0;
    }
    else
    {
        ret = 1;
    }
    return ret;
    //return (RegReadFldAlign(AUDP_STAT,AUDP_STAT_0) == 0) ? FALSE : TRUE;
}


/**
 * HDMI get audio sampling frequency
 *
 * @retval	AUD_SAMPLE_F value in 0x22d30
 */
UINT8 u1HDMIGetAudioSamplingFreq(void)
{
#if 1
    return (_bHdmiAudFreq);
#else
    //UINT8 u1Fs;

    //u1Fs = bHDMIAUDIOSampleRateCal();

    return RegReadFld(CHST1, AUD_SAMPLE_F);
    //return u1Fs;
#endif
}

void vHDMIPowerOnOff(UINT8 power)	// 1: Normal 0: Power Down
{
    if (power)
    {
        vRegWriteFldAlign(PD_SYS, 1, PD_TOTAL);
    }
    else
    {
        vRegWriteFldAlign(PD_SYS, 0, PD_TOTAL);
    }

}

#ifdef CC_HDMI_ARC_GPIO_CTRL
void vHdmiSetARCOnOff(UINT8 bOnOff)
{
    INT32 dGpioVal = 0;
	UINT32 u4Gpio = 0;
	UINT32 u4GpioPolarity = 0;

    if ((0 == DRVCUST_InitQuery(eCecArcGpio, &u4Gpio)) && (0 == DRVCUST_InitQuery(eCecArcGpioEnablePolarity, &u4GpioPolarity)))
	{
		if(1 == bOnOff)
	    {
	    	dGpioVal = ((u4GpioPolarity == 1)?1:0);
	        Printf("------ARC on, GPIO %d set to %d------\n", u4Gpio, dGpioVal);
	        GPIO_Output(u4Gpio, &dGpioVal);
	    }
		else if(0 == bOnOff)
	    {
	    	dGpioVal = ((u4GpioPolarity == 0)?1:0);
	        Printf("------ARC off, GPIO %d set to %d------\n", u4Gpio, dGpioVal);
	        GPIO_Output(u4Gpio, &dGpioVal);
	    }
	}
}
#endif
/******************************************************************************/

