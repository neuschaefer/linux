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
 * @file DVI/HDMI/CCIR decoder driver
 *
 * @author yj_chiang@mtk.com.tw
 * @entry points
 * @note init -> bApiVideoMainSubSrc -> _bDviExtSrc -> vDviConnect
 *	 -> vDviChkModeChange -> vDviInitial -> vDviModeDetect
 * @note vDviModeDetect -> DVI_SEARCH_STATE -> bDviStdTimingSearch
 *	 !MODE_NOSIGNAL -> fgIsVgaTiming -> DVI_WAIT_AUTO_STATE (auto position) -> DVI_MODE_CHG_DONE_STATE
 *	 MODE_NOSIGNAL -> DVI_MODE_CHG_DONE_STATE
 * @note HLEN, VLEN -> HCLK, VCLK -> timing table
 */

#define _DRV_DVI_C_

#include "general.h"

#include "x_printf.h"
// Kernel
//#include "hw_bim.h"  // no use
#include "util.h"
// vdo kernel
#include "hw_vdoin.h"
#include "hw_dvi.h"
#include "hw_sys.h"
#include "hw_ckgen.h"
#include "drv_vdoclk.h"
// sync
#include "drv_async.h"
#include "drv_dsync.h"
// vga
#include "drv_vga.h"
#include "drv_auto.h"
#include "vga_auto.h"
#include "vga_table.h"
// source
#include "source_select.h"
// autocolor
#include "drv_autocolor.h"
// hdtv
#include "drv_hdtv.h"
// dvi
#include "drv_dvi.h"
// scpos
#include "drv_scaler.h"
// ycproc
#include "drv_ycproc.h"
// display
#include "drv_display.h"
// video API
#include "video_def.h"
// hdmi
#if 1 // SUPPORT_HDMI_INPUT
#include "drv_hdmi.h"
#include "hw_hdmi.h"
#endif
#include "nptv_debug.h"
#include <x_lint.h>
#include "drvcust_if.h"

#include "hw_dsync.h"
//#include "drv_scpos.h"
#include "drv_video.h"
#include "api_notify.h"
#include "vdo_misc.h"
#include "srm_drvif.h"
#include "drv_di.h"
#include "mute_if.h"
#include "drv_tdtv.h"
#ifdef CC_AUD_HDMI_SPDIF_CFG
#include "aud_if.h"
#endif

#ifdef CC_AUD_HDMI_SPDIF_CFG
AUD_HDMI_CHL_STATUS aud_ch_info;
#endif

#if (defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
#include "drv_scaler_drvif.h"
#endif

/* compare HTOTAL when searching timing */
#define USE_DVI_HVTOTAL_SEARCH 1
#define MAX_DVI_MD_CHG_CNT 35
#define MAX_DVI_OFF_ON_CNT 35

#define DEBUG_DVI_MD_CHG 0
#define DEBUG_DVI_SET_MODE_CHG_FLAG 0
#define MT8293_8202_PROPRIETARY     0 // remove from 5372

#define DVI_DEBUG 1
#define ENABLE_DE_MODE 1
#define FROM_HDMIDEC 1
#define NEW_MODE_CHANGE_FLOW	1
#define USE_HDCP_CHECK_STABLE 	1
#define Slove_262_Half_Line_DVD	1
#define Seprate_AVWidth			1

#define Enable_DVI_DE_VHS_END 1

#define RTCK_AUTO_CTRL_V1 1 // should be enabled

#define HDMI_RTCK_AUTO_Release_FLOW	        1
#define ClockStableCNT		        5
#define AACEnable		1	// YT.Chou command 

#define NotSupport_80Hz_About	0
#define Diff(a, b)  (((a)>(b))?((a)-(b)):((b)-(a)))
enum
{
    DVI_SEARCH_STATE = 0,
    DVI_WAIT_AUTO_STATE,
    DVI_MODE_CHG_DONE_STATE
};
enum
{
    DVI_NO_SIGNAL,
    DVI_CHK_MODECHG,
    DVI_WAIT_STABLE
};

#ifdef CC_SUPPORT_HDMI_FBOOTING
extern UINT32 _u4HdmiLoops;
extern UINT8 _u1HdmiSrcIn;
extern UINT8 _u1HdcpGood;
#endif
////////////////////////////////////////////////////////////////////////////////
// CCIR Settings
#define CCIR16_SEPARATE 0
#define CCIR16_EMBEDDED 1
#define CCIR8_SEPARATE 2
#define CCIR8_EMBEDDED 3

#define CCIR_FORMAT CCIR8_EMBEDDED
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Global Variable Declaration
//UINT8   _bSync1Vsyn2; // for auto color interaction
UINT8   _bDviVclk;
UINT16   _wDviHclk; // timing table: table value; otherwise, the value used by timing search
UINT16   _wDviWidth;
UINT16   _wDviHeight;
BOOL    _IsDviDetectDone;
UINT8   _bDviTiming;
UINT16  wVSyncWidth;
UINT8   _bDviExtSrc = SV_DV_HDMI;

UINT32   _bDviMdChgCnt;
UINT8   _bDviDeChgCnt;
UINT8   _bDviPixClkChgCnt;
#if CCIR_decode_444
UINT8 _bCCIRNeedModeChgCnt;  // [DTV00082304]
#endif
UINT8  _bDviNoSigCnt ;

UINT16 _wDviTmpHtotal;
UINT16 _wDviTmpVtotal;
UINT16   _wDviVTotal;
UINT16  _wDviHtotal;
UINT32  _wHDMIPixelClk;
UINT32 _wDviVstartDefault;
UINT8 _bPWOFFCnt;
UINT8 _bInfoChgCnt;
#ifdef CC_AUD_HDMI_SPDIF_CFG
UINT8 _bInfoAudioInfoStableCnt;
#endif
UINT8   _bDVIEQFlag;
UINT8 _bNotSupportCnt;
UINT8 _bCH0Status;
UINT8 _bCH0StableCount;
//UINT8   _bDviFlag;
UINT8   _bDviModeDetState;
UINT8  _bDviRetry ;
UINT8  _bDviModeChged;
UINT8  _bXpcStable;
#if USE_HDCP_CHECK_STABLE
UINT8  _bHDCPStableCount;
UINT8 _bPreHDCPstatus;
UINT8 _bCurHDCPstatus;
#endif
UINT8  _bEQCH0, _bEQCH1, _bEQCH2;
UINT8 _bForce2DFlag = 0, _bUseLorRFlag = 0;
static UINT8 _bBoundary_flag = 0;
#if NEW_MODE_CHANGE_FLOW
UINT8 _bDviChkState;
HAL_TIME_T _rDviModeChgTime;
#endif
#define IC5381_debug 1
#define Scaler_Eat_2line 0
#if ENABLE_DE_MODE
UINT8 bEnDVIDE = SV_ON;
// PRIVATE UINT8 bRefreshRate;
#endif

#if HDMI_RTCK_AUTO_Release_FLOW
UINT8 dataXCLK1, dataXCLK2, readXCLKCnt;
UINT8 dataXCLK3;
UINT8 fgXlkStable;
UINT8 fgXlkStableCNT;
UINT32 _bDviMdChgXLKStableCnt;
#endif

#ifdef MT5360B_WA4
UINT32 u4DviAvStart = 0;
#define AVSTART_DELTA 4
#endif

UINT8 _bHResChgIntDetectFlg;  // jiewen, 20090114

UINT8 _bDviMdChgCntTHR = 1;
UINT8 _bDviDeChgCntTHR = 1;
UINT8 _bDviPixClkChgCntTHR = 5;
#if CCIR_decode_444
UINT8 _bCCIRNeedModeChgCntTHR = 3;  // [DTV00082304]
#endif
UINT32 _wDviTmpTHR = 2;
UINT32  _wDVI_WAIT_STABLE_COUNTTHR = 0;
E_TDTV_DECODER_INPUT _bCurInpMode;
UINT32 _bQueryMode;
UINT32 _bCurQueryMode;

////////////////////////////////////////////////////////////////////////////////
extern UINT8 _bHDMIScanInfo;
extern UINT8 _bHDMIAspectRatio;
extern UINT8 _bHDMIAFD;
extern UINT8 _bHDMIHDCPStatus;
extern UINT8 _bHDMI422Input;
extern UINT8 _bHDMIITCFlag;
extern UINT8 _bHDMIITCContent;
extern UINT32 _wDVI_WAIT_STABLE_COUNT;
extern  UINT32 _wHDMIBypassFlag;
extern UINT32 _wDVI_WAIT_NOSIGNAL_COUNT;
extern UINT8 _bNodifyConnect;
extern UINT8 _bHDMIColorSpace;
//extern UINT32 _u4MainMuteMask;  // jiewen, 20090114
extern UINT32 _u4PipMuteMask;
extern UINT8 _bHPDCycle;
extern UINT8 _bFixedEQReset;
#ifdef CC_BYPASS_AVI_INFOFRAME
extern UINT8 _bDVIAVIinfoBypass;
#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef CC_AUD_HDMI_SPDIF_CFG
AUD_HDMI_CHL_STATUS aud_ch_info;
#endif

// Local Function
/**
 * @brief 	Set CCIR input horizontal start.
 * @param	wStart - CCIR input horizontal start
 * @retval	None
 */
void vCCIRSetAvStart(UINT16 wStart)
{
    vRegWriteFldAlign(CCIR_01, wStart, CCH_AV_START);
}

UINT16 wCCIRGetAvStart(void)
{

    return RegReadFldAlign(CCIR_01, CCH_AV_START);
}
/**
 * @brief 	Set CCIR input horizontal start.
 * @param	wStart - CCIR AV width
 * @retval	None
 */
void vCCIRSetAvWidth(UINT16 wWidth)
{
    wWidth += DECODER_ADD_WIDTH ;
    vRegWriteFldAlign(DVI_03, wWidth, CCH_AV_WIDTH);
}

UINT16 wCCIRGetAvWidth(void)
{

    return RegReadFldAlign(DVI_03, CCH_AV_WIDTH);
}
/**
 * @brief 	Set CCIR input vertical start for position adjust.
 * @param	wStart - CCIR input vertical start
 * @retval	None
 */
void vCCIRSetVsStart(UINT16 wStart)
{
    vRegWriteFldAlign(CCIR_01, wStart, CCH_VS_START);
}

UINT16 wCCIRGetVsStart(void)
{
    return RegReadFldAlign(CCIR_01, CCH_VS_START);
}
/**
 * @brief 	Set CCIR input vertical start for position adjust.
 * @param	wStart - CCIR VDE width
 * @retval	None
 */
void vCCIRSetVsWidth(UINT16 wStart)
{
    vRegWriteFldAlign(DVI_03, wStart, CCH_VDE_WIDTH);
}

UINT16 wCCIRGetVsWidth(void)
{
    return RegReadFldAlign(DVI_03, CCH_VDE_WIDTH);
}
/**
 * @brief 	Set DVI input horizontal start.
 * @param	wStart - DVI input horizontal start
 * @retval	None
 */
void vDviSetAvStart(UINT16 wStart)
{
    vRegWriteFldAlign(DVI_00, wStart, DVI_AV_START12_0);
}

UINT16 wDviGetAvStart(void)
{
    return RegReadFldAlign(DVI_00, DVI_AV_START12_0);
}
/**
 * @brief 	Set DVI input horizontal width.
 * @param	wWidth - DVI input horizontal width.
 * @retval	None
 */
void vDviSetAvWidth(UINT16 wWidth)
{
    //vRegWriteFldAlign(DVI_00,wWidth+3,DVI_AV_WIDTH);//prevent the problem of pink line in right side by W.C Shih
    wWidth += DECODER_ADD_WIDTH ;
    vRegWriteFldAlign(DVI_00, wWidth, DVI_AV_WIDTH);//modify by ciwu
}

UINT16 wDviGetAvWidth(void)
{
    return RegReadFldAlign(DVI_00, DVI_AV_WIDTH);
}

/**
 * @brief 	Set DVI input vertical start for position adjust.
 * @param	wStart - DVI input vertical start
 * @retval	None
 */
void vDviSetVsStart(UINT16 wStart)
{
    vRegWriteFldAlign(DVI_01, wStart, DVI_VDE_START11_0);
}

UINT16 wDviGetVsStart(void)
{
    return RegReadFldAlign(DVI_01, DVI_VDE_START11_0);
}

/**
 * @brief 	Set DVI input vertical width.
 * @param	wWidth - DVI input vertical width.
 * @retval	None
 */
void vDviSetVsWidth(UINT16 wWidth)
{
    vRegWriteFldAlign(DVI_01, wWidth, DVI_VDE_WIDTH);
}

UINT16 wDviGetVsWidth(void)
{
    return RegReadFldAlign(DVI_01, DVI_VDE_WIDTH);
}
UINT8 bDVICENStatus(void)
{
    return RegReadFldAlign(DVI_00, DVI_CEN_SEL);
}

#if ENABLE_DE_MODE

UINT16 wDVIDEWidth(void)
{
    return (RegReadFldAlign(DVI_HDTV_01, STA_DVI_HDELEN));
}

PRIVATE UINT16 wDVIDEHeight(void)
{
    return RegReadFldAlign(DVI_HDTV_02, STA_DVI_VTOTAL) - RegReadFldAlign(DVI_HDTV_02, STA_DVI_VSLEN);
}

UINT16 wDVIDEHTotal(void)
{
    return  (RegReadFldAlign(DVI_HDTV_01, STA_DVI_HTOTAL) + 1);
}
UINT8 bCCIRCENStatus(void)
{
    return RegReadFldAlign(CCIR_00, CCH_CEN_SEL);
}
UINT16 wCCIRHTotal(void)
{
    return  (RegReadFldAlign(DVI_HDTV_03, STA_CCH_HTOTAL) + 1);
}
PRIVATE UINT16 wDVIDEVsyncWidth(void)
{
    return RegReadFldAlign(DVI_HDTV_02, STA_DVI_VSLEN);
}
UINT16 wCCIRVsyncWidth(void)
{
    return RegReadFldAlign(DVI_HDTV_04, STA_CCH_VSLEN);
}
PRIVATE UINT16 wDVIDEVTotal(void)
{
    return (wDVIDEHeight() + wDVIDEVsyncWidth());
}
PRIVATE void vEnterDEMode(UINT8 bonoff)
{
    if (bonoff == SV_ON)
    {
#if CCIR_decode_444
        vCCIRSetAvStart(0);
#else
        vRegWrite4BMsk(DVI_02, Fld2Msk32(DVI_DE_MODE), Fld2Msk32(DVI_DE_MODE));// enable DE mode
        vDviSetAvStart(0);
#endif
    }
    else
    {
#if !CCIR_decode_444
        vRegWrite4BMsk(DVI_02, 0, Fld2Msk32(DVI_DE_MODE)); // disable DE mode
#endif
    }
}
#endif



#if FROM_HDMIDEC
#define wDVIGetVtotal()	wHDMIVTotal()
#define wDVIGetHtotal()	wHDMIHTotal()
#define bDVIGetHAct()	fgHDMIHsyncAct()
#define bDVIGetWidth()	wHDMIResoWidth()
#define bDVIGetHeight()	wHDMIResoHeight()
#else
#define wDVIGetVtotal() wDVIDEVTotal()
#define wDVIGetHtotal() wDVIDEHTotal()
#define bDVIGetHAct()	fgDSHAct()
#define bDVIGetWidth()	wDVIDEWidth()
#define bDVIGetHeight() wDVIDEHeight()
#endif

/**
 * @brief 	Calculate hsync clock (27MHz domain) by measured HLEN/VLEN.
 * @param	None
 * @retval	hsync clock (measure in 27MHz domain)
 */
PRIVATE UINT16 wDviIHSClock(void)
{
#if FROM_HDMIDEC
    return dwHDMILineFreq();
#else

    UINT32 lhsync_freq;
    //multiply 2 for increasing 1bit resolution
    //multiply 100000 for Mega scale
    lhsync_freq = wDSHtotalMeasure();
    if (lhsync_freq == 0)
    {
        return 1;
    }
    lhsync_freq = (CRYSTAL * 1000000 * 2) / lhsync_freq;
    lhsync_freq = lhsync_freq / 100;	// resolution be with 100Hz as unit
    lhsync_freq = ((lhsync_freq + 1) >> 1);

    return (UINT16) lhsync_freq;
#endif

}

/**
 * @brief 	Calculate vsync clock (27MHz domain) by measured HLEN/VLEN.
 * @param	None
 * @retval	vsync clock (measure in 27MHz domain)
 */
PRIVATE UINT8 bDviIVSClock(void)
{
#if !FROM_HDMIDEC
    UINT32 lvsync_freq;
#endif

#if FROM_HDMIDEC
    return bHDMIRefreshRate();
#else

    //_wDviVTotal = wDSVtotalMeasure();
    lvsync_freq = (UINT32)_wDviVTotal * wDSHtotalMeasure();
    if (lvsync_freq == 0)
    {
        return 1;
    }
    lvsync_freq = (CRYSTAL * 1000000 * 2) / lvsync_freq;	// *2 add counting resolution
    lvsync_freq = ((lvsync_freq + 1) >> 1);
    return ((UINT8) lvsync_freq);
#endif
}
/**
* @brief 	vDVISetModeCHG(void);Flag responses mode change
* @param	None
* @retval	None
*/
void vDVISetModeCHG(void)
{
    if (_bDviModeChged)
    {
        return ;
    }

    //Enable field indicator detection
    vRegWriteFldAlign(DVI_00, 1, DVI_FLD_DET);
    //vRegWriteFldAlign(TMDS_CTRL0, 1, HDMI_RTCK_AUTO);
#if NEW_MODE_CHANGE_FLOW
    HAL_GetTime(&_rDviModeChgTime);
#endif
    _bDviModeChged = 1 ;
    _bPWOFFCnt = 0;
    _bXpcStable = 0;
    _bCH0Status = 0;
    _bEQCH0 = 0xf;_bEQCH1 = 0xf;_bEQCH2 = 0xf;
    if (fgIsMainDVI())
    {
        // test for fast mode change /done bug
        vClrMainFlg(MAIN_FLG_MODE_DET_DONE);
        vSetMainFlg(MAIN_FLG_MODE_CHG);
#if DVI_DEBUG
        LOG(3, "Main Mode Chg #1 in vDviInitial\n");
#endif
    }
    if (fgIsPipDVI())
    {
        // test for fast mode change /done bug
        vClrPipFlg(PIP_FLG_MODE_DET_DONE);
        vSetPipFlg(PIP_FLG_MODE_CHG);
#if DVI_DEBUG
        LOG(3, "Pip Mode Chg #1 in vDviInitial\n");
#endif
    }

}

void vDVISetModeDone(void)
{

    if (!_bDviModeChged)
    {
        return ;
    }
#if 0
#if !Support_HW_Adaptive_EQ
    if (_bFixedEQReset == 1)
    {
        _bFixedEQReset = 2;
        bHDMIPHYReset(4);
    }
#endif
#endif
    bHDMIAPPL2Reset();
    //vRegWriteFldAlign(TMDS_CTRL0, 1, HDMI_RTCK_AUTO);
    //vUtDelay2us(1);
    //vRegWriteFldAlign(TMDS_CTRL0, 0, HDMI_RTCK_AUTO);

    vRegWrite4B(INTR_STATE0, Fld2Msk32(INTR2_HDMI_MODE) | Fld2Msk32(INTR2_CKDT) | Fld2Msk32(INTR2_SCDT) | Fld2Msk32(INTR2_CLK_CHG));
    vRegWrite4B(INTR_STATE1, Fld2Msk32(INTR5_VRESCHG) | Fld2Msk32(INTR5_HRESCHG) | Fld2Msk32(INTR5_POLCHG) | Fld2Msk32(INTR5_INTERLACEOUT));
    bHDMIPHYReset(HDMI_RST_DEEPCOLOR);
    vUtDelay1ms(1);
    vRegWriteFldAlign(MUTE_SET, 1, TDFIFO_SYNC_EN);  // jiewen, 20090114
    vUtDelay1ms(1);
    vRegWriteFldAlign(MUTE_SET, 0, TDFIFO_SYNC_EN);

    _bHResChgIntDetectFlg = 1;  // jiewen, 20090114

    _bHDMIColorSpace = bHDMIInputType();
    _bNodifyConnect = 0;
    if (fgIsMainDVI())
    {
        _vDrvVideoSetMute(MUTE_MODULE_HDMI, SV_VP_MAIN, 0, FALSE);
        vSetMainFlg(MAIN_FLG_MODE_DET_DONE);
#if DVI_DEBUG
        LOG(3, "Main Mode Done\n");
#endif
    }
    if (fgIsPipDVI())
    {
        _vDrvVideoSetMute(MUTE_MODULE_HDMI, SV_VP_PIP, 0, FALSE);
        vSetPipFlg(PIP_FLG_MODE_DET_DONE);
#if DVI_DEBUG
        LOG(3, "Pip Mode Donel\n");
#endif
    }
    _bDviModeChged = 0 ;

    #ifdef CC_AUD_HDMI_SPDIF_CFG
	aud_ch_info.eACPType=(HDMI_ACPTYPE)bHDMIACPType();
	aud_ch_info.u1Category=bHDMIChannelStCategoryCode();
	aud_ch_info.u1CopyRight=bHDMIChannelStCopyright();
	aud_ch_info.eDataType=(HDMI_DATATYPE)bHDMI_IsPCM();
	aud_ch_info.eSampeFreq=(HDMI_AUD_FS)bHDMIGetAudSampleFreq();
	AUD_CfgSpdifByHDMI(&aud_ch_info);
    #endif
}


#if ENABLE_DE_MODE
UINT32 dwDviPIXClock(void)
{
#if CCIR_decode_444
    return (wDviIHSClock()*(wHDMIHTotal()));//wCCIRHTotal it has some problem when change 480p60,422 not oversample to 480i 422 oversample.
#else
    return (wDviIHSClock()*(wDVIDEHTotal()));
#endif
}
#endif

/**
 * @brief 	Adjust DVI-related polarity setting according to sync process polarity measure result.
 * @param	None
 * @retval	None
 */
PRIVATE void vDviPolarityUniform(void)
{
#if ENABLE_DE_MODE
    if (bEnDVIDE == 1)
    {
        vRegWrite4BMsk(CCIR_00, 0, Fld2Msk32(CCH_HS_POL));	//because h,v of  DVI decoder  is decoded  from DE signal , not inverse
        vRegWrite4BMsk(CCIR_00, 0, Fld2Msk32(CCH_VS_POL));  //because h,v of  DVI decoder  is decoded  from DE signal , not inverse
        return;
    }
#else

    // Goal: H- V+
#if FROM_HDMIDEC
    if (fgHDMIHsyncPolarity())
#else
        if (fgDSHPol())
#endif
        {
            //LogS("H+ then INV");
            vRegWrite4BMsk(CCIR_00, Fld2Msk32(CCH_HS_POL), Fld2Msk32(CCH_HS_POL));
            //LogS("H+ then INV HYSNC_LOCK");
            vRegWrite4BMsk(DSYNC_02, Fld2Msk32(DSYNC_HSYNC_LOCK_INV), Fld2Msk32(DSYNC_HSYNC_LOCK_INV));
        }
        else
        {
            //LogS("H- then not INV");
            vRegWrite4BMsk(CCIR_00, 0, Fld2Msk32(CCH_HS_POL));
            //LogS("H- then not INV HYSNC_LOCK");
            vRegWrite4BMsk(DSYNC_02, 0, Fld2Msk32(DSYNC_HSYNC_LOCK_INV));
        }

#if FROM_HDMIDEC
    if (fgHDMIVsyncPolarity())
#else
        if (fgDSVPol())
#endif
        {
            //LogS("V+ then not INV");
            vRegWrite4BMsk(CCIR_00, 0, Fld2Msk32(CCH_VS_POL));
            //LogS("V+ then not INV VSYNC_OUTP_INV");
            vRegWrite4BMsk(DSYNC_01, 0 , Fld2Msk32(VSYNC_OUTP_INV));
        }
        else
        {
            //LogS("V- then INV");
            vRegWrite4BMsk(CCIR_00, Fld2Msk32(CCH_VS_POL) , Fld2Msk32(CCH_VS_POL));
            //LogS("V- then INV VSYNC_OUTP_INV");
            vRegWrite4BMsk(DSYNC_01, Fld2Msk32(VSYNC_OUTP_INV) , Fld2Msk32(VSYNC_OUTP_INV));
        }
#endif
}

/**
 * @brief 	DVI initial function for each mode change or connect/disconnect.
 * @param	None
 * @retval	None
 */
void vDviInitial(void)
{
    // Software Initialize
    //_IsDviDetectDone = FALSE;
    _bDviTiming = MODE_WAIT;
    _bDviModeDetState = DVI_SEARCH_STATE;
    _bDviMdChgCnt = 0;
    _bDviDeChgCnt = 0 ;
    _bDviPixClkChgCnt = 0;
#if CCIR_decode_444
    _bCCIRNeedModeChgCnt = 0;  // [DTV00082304]
#endif
    _bDviNoSigCnt = 0 ;
    _wDviVstartDefault = 0;
    _bNotSupportCnt = 0;
#if USE_HDCP_CHECK_STABLE
    _bHDCPStableCount = 0;
#endif
    // Hardware Initialize
#if ENABLE_DE_MODE
    if (bEnDVIDE == 1)
    {
        vEnterDEMode(SV_ON);
    }
    else
    {
        vEnterDEMode(SV_OFF);
    }
#endif

    // Set Polarity
    vDviPolarityUniform();
    // reset DVI's blank level
    vRegWrite4BMsk(DVI_02, Fld2Msk32(DVI_BLANK_RESET) , Fld2Msk32(DVI_BLANK_RESET));
    // reset DVI_HVLD_SEL
    //vWriteVDOINMsk(CCIR00, 0, DVI_HVLD_SEL, BANK0);

    // Scientific Atlanta cable box model 4250HDC 262.5Line

#if 0 //replace by 20022420[31] DVI_VHS_END_MODE
    vRegWriteFldAlign(DVI_01, 1, DVI_HDELEN_SEL);	// [23] set 0 [11] set 1
#endif
    vDVISetModeCHG();
#if (CCIR_decode_444 == 1)
    vRegWriteFldAlign(LPF_PIP_00, 0, DVI_EN);
#ifdef CC_MT5365
    vRegWriteFldAlign(SYS_05, 0x1, CCH_BYPASS);  // [DTV00080852], bypass one circuit of CCH, which is only for 5365/66
#endif
#else
    vRegWriteFldAlign(LPF_PIP_00, 1, DVI_EN);
#endif

}

/**
 * @brief 	Check 3D packet vaild or not
 * @retval	1 is vaild
 */
UINT8 bHDMI3DPacketVaild(void)
{
    HDMI_3D_INFOFRAME* info;
    info = API_HDMI_3D_Get_InfoFrame();

    if (info->HDMI_3D_Enable)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

#define FP_progressive	1
#define FP_interlace 		2
/**
 * @brief 	Check 3D progres or interlace frame packing timing
 * @retval	1 is progressive ,2 is interlace,3 is other
 */
UINT8 bHDMI3DIPCheck(void)
{
    
    if ((_bDviTiming == MODE_3D_720p_50_FP)||(_bDviTiming == MODE_3D_720p_60_FP)||(_bDviTiming == MODE_3D_1080p_24_FP)||
    	(_bDviTiming == MODE_3D_480p_60_FP)||(_bDviTiming == MODE_3D_576p_50_FP)||(_bDviTiming == MODE_3D_720p_24_FP)||
    	(_bDviTiming == MODE_3D_720p_30_FP)||(_bDviTiming == MODE_3D_1080p_30_FP))
    {
        return FP_progressive;
    }
    if((_bDviTiming == MODE_3D_1080I_60_FP)||(_bDviTiming == MODE_3D_480I_60_FP)
    	||(_bDviTiming == MODE_3D_576I_60_FP)||(_bDviTiming == MODE_3D_1080I_50_FP))
    {
        return FP_interlace;
    }
    return 3;
}
/**
 * @brief 	guery AP force 3D mode
 * @param	
 * @retval
 */
UINT32 wDviQueryAP3DSetting(void)
{

	HDMI_3D_INFOFRAME* info;
    	info = API_HDMI_3D_Get_InfoFrame();

	    if(bHDMI3DPacketVaild() == 0) //input 2D mode
	    {
	    	if(fgHDMIinterlaced())
	    	{
	    		_bCurInpMode=E_TDTV_DECODER_INPUT_2D_I;
	    	}
	    	else
	    	{
	    		_bCurInpMode=E_TDTV_DECODER_INPUT_2D_P;
	    	}
	    }
	    else  //input 3D mode
	    {
	    	if(FP_progressive==bHDMI3DIPCheck())
	    	{
	    		_bCurInpMode=E_TDTV_DECODER_INPUT_FP_P;
	    	}
	    	else if(FP_interlace==bHDMI3DIPCheck())
	    	{
	    		_bCurInpMode=E_TDTV_DECODER_INPUT_FP_I;
	    	}
	    	else if((_bDviTiming==MODE_525I)||(_bDviTiming==MODE_625I)||(_bDviTiming==MODE_1080i_50)||(_bDviTiming==MODE_1080i))
	    	{
	    		 if (info->HDMI_3D_Structure == HDMI_3D_Structure_SideBySideHalf)
	    		{
	    		 	_bCurInpMode=E_TDTV_DECODER_INPUT_SBS_I;
	    		 }
	    		if (info->HDMI_3D_Structure == HDMI_3D_Structure_TopBottom)
	    		 {
	    		 	_bCurInpMode=E_TDTV_DECODER_INPUT_TB_I;
	    		 }
	    	}
	    	else 
	    	{
	    		if (info->HDMI_3D_Structure == HDMI_3D_Structure_SideBySideHalf)
	    		{
	    			_bCurInpMode=E_TDTV_DECODER_INPUT_SBS_P;
	    		 }
	    		if (info->HDMI_3D_Structure == HDMI_3D_Structure_TopBottom)
	    		{
	    			 _bCurInpMode=E_TDTV_DECODER_INPUT_TB_P;
	    		}	
	    	}

	    }
        //_bQueryMode=u4DrvTDTVHDMIModeQuery((E_TDTV_DECODER_INPUT)_bCurInpMode);
	    return u4DrvTDTVHDMIModeQuery((E_TDTV_DECODER_INPUT)_bCurInpMode);
}

void vDviTDTVCB(void *pvArgv)
{    
    _bCurQueryMode=wDviQueryAP3DSetting();
   // _bDviChkState = DVI_WAIT_STABLE;
   // vDviInitial();    
}

/**
 * @brief 	check AP rule
 * @param	
 * @retval	1 is decsion by DVI decord, 0 is force
 */
UINT8 bDviBypassAp3DRule(void)
{

	E_TDTV_DECODER_INPUT inputtype;
    E_TDTV_DECODER_OUTPUT outputtype;
	inputtype=(E_TDTV_DECODER_INPUT)(_bQueryMode&0xffff);
    outputtype=(E_TDTV_DECODER_OUTPUT)((_bQueryMode&0xffff0000)>>16);
	
	if (!_fgForce3D)
	{
		if(outputtype==E_TDTV_DECODER_OUTPUT_NATIVE)
		{
			if(_bCurInpMode==E_TDTV_DECODER_INPUT_FP_P && inputtype==E_TDTV_DECODER_INPUT_2D_P)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
	   	else
	    {
			if(outputtype==E_TDTV_DECODER_OUTPUT_3D_2_2D)	
			{
				return 0;
			}
			else 
			{
				return 1;
			}
	    }
	}
	else
	{
		return 1;
	}
}
/**
* @param  u1Force2D    0: Auto  1: Force 2D
 * @param  u1LR         0: Display L frame only, 1: Display R frame only
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
UINT32 wDVIDecoder3DForce2D(UINT8 u1Force2D, UINT8 u1LR)
{
    if ((_bForce2DFlag != u1Force2D) || (_bUseLorRFlag != u1LR))
    {
        _bForce2DFlag = u1Force2D;
        _bUseLorRFlag = u1LR;
        _bDviChkState = DVI_WAIT_STABLE  ;
        vDviInitial();
        return 1;
    }
    return 0;
}
/**
 * @brief 	Check 3D support timing
 * @retval	1 is support ,0 is not support
 */
UINT8 bHDMI3DTiming(void)
{
    if ((_bDviTiming >= MODE_3D_720p_50_FP) && (_bDviTiming < MODE_MAX) )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief Call by PE
 * @retval	1 is free run ,0 is decide by DVI decorder
 */
void bHDMI3DLRFreeRunControl(UINT8 bvalue)
{
	if(1==bvalue)
	{
        vRegWriteFldAlign(LPF_PIP_00, 1, DVI_3D_VSYNC_TOG);            
        vRegWriteFldAlign(LPF_PIP_00, 0, DVI_3D_MANUAL );  
	}
	else
	{
        vRegWriteFldAlign(LPF_PIP_00, 0, DVI_3D_VSYNC_TOG);            
        vRegWriteFldAlign(LPF_PIP_00, 0, DVI_3D_MANUAL );  
	}
}
void vHDMI3DSetConfig(void)
{

UINT32 wVactivewidth;
UINT32 wVprochwidth;
    if(bHDMI3DPacketVaild() == 0)
    {
        vRegWriteFldAlign(VFE_EF, 0x0, CCH_3D_VACT);
        vRegWriteFldAlign(VFE_EF, 0x0, CCH_3D_VACT_SP1);
        vRegWriteFldAlign(VFE_EF, 0x0, CCH_3D_VACT_SP2);
        vRegWriteFldAlign(VFE_EF, 0x0, CCH_3D_SEL);
        vRegWriteFldAlign(VFE_EF, 0x0, CCH_3D_MODE);
        vRegWriteFldAlign(LPF_PIP_00, 0x0, CCH_3D_INTTGL);
        return;
    }
    if(1==bDviBypassAp3DRule())
    {
    	wVprochwidth=Get_VGAMODE_IPV_STA(_bDviTiming);
    	if(FP_progressive==bHDMI3DIPCheck()) //progressive frame packing timing
    	{
    	     wVactivewidth=(Get_VGAMODE_IPV_LEN(_bDviTiming) - Get_VGAMODE_IPV_STA(_bDviTiming)) >> 1;	
            vRegWriteFldAlign(VFE_EF, wVactivewidth-1, CCH_3D_VACT);
            vRegWriteFldAlign(VFE_EF, wVprochwidth-1, CCH_3D_VACT_SP1);
            vRegWriteFldAlign(VFE_EF, 0x1, CCH_3D_SEL);
            vRegWriteFldAlign(VFE_EF, 0x2, CCH_3D_MODE);
            vRegWriteFldAlign(LPF_PIP_00, 0x0, CCH_3D_INTTGL);
            vRegWriteFldAlign(LPF_PIP_00, 0x0, DVI_3D_MANUAL);//work around
    	}
      	else if(FP_interlace==bHDMI3DIPCheck()) //interlace frame packing timing
       {
            wVactivewidth=(Get_VGAMODE_IPV_LEN(_bDviTiming) - Get_VGAMODE_IPV_STA(_bDviTiming) * 3 - 2) >> 2;//22(real), 23(by decoder), 22(by decoder, this value is in timing table), 23(by decoder)
            vRegWriteFldAlign(VFE_EF, wVactivewidth-1, CCH_3D_VACT);
            vRegWriteFldAlign(VFE_EF, wVprochwidth, CCH_3D_VACT_SP1);
            vRegWriteFldAlign(VFE_EF, wVprochwidth-1, CCH_3D_VACT_SP2);
            vRegWriteFldAlign(VFE_EF, 0x1, CCH_3D_SEL);
            vRegWriteFldAlign(VFE_EF, 0x3, CCH_3D_MODE);
            vRegWriteFldAlign(LPF_PIP_00, 0x0, CCH_3D_INTTGL);
            vRegWriteFldAlign(LPF_PIP_00, 0x1, DVI_3D_MANUAL);//work around
       }
      	else
       {
            vRegWriteFldAlign(VFE_EF, 0x0, CCH_3D_VACT);
            vRegWriteFldAlign(VFE_EF, 0x0, CCH_3D_VACT_SP1);
            vRegWriteFldAlign(VFE_EF, 0x0, CCH_3D_SEL);
            vRegWriteFldAlign(VFE_EF, 0x0, CCH_3D_MODE);
            vRegWriteFldAlign(LPF_PIP_00, 0x0, CCH_3D_INTTGL);
            vRegWriteFldAlign(LPF_PIP_00, 0x0, DVI_3D_MANUAL);//work around
       }
    }
    else
    {
            vRegWriteFldAlign(VFE_EF, 0x0, CCH_3D_VACT);
            vRegWriteFldAlign(VFE_EF, 0x0, CCH_3D_VACT_SP1);
            vRegWriteFldAlign(VFE_EF, 0x0, CCH_3D_VACT_SP2);
            vRegWriteFldAlign(VFE_EF, 0x0, CCH_3D_SEL);
            vRegWriteFldAlign(VFE_EF, 0x0, CCH_3D_MODE);
            vRegWriteFldAlign(LPF_PIP_00, 0x0, CCH_3D_INTTGL);
        }
    }

#define Vtotal_Get	1
#define VActive_Get	2
#define Htotal_Get	3
#define HActive_Get	4
#define Reflash_rate_Get	5
/**
 * @brief 	Get htotal,vtotal,hactive,vactive of 2D mode force 3D
 * @param 1
 * @retval type value 1=Vtotal , 2=Vactive , 3=Htotal, 4=Hactive,  5=reflash rate
 */
UINT16 wHDMIGet2DmodeForce3DHVParameter(UINT8 bType,UINT16 wValue)
{
    E_TDTV_DECODER_INPUT inputtype;
    E_TDTV_DECODER_OUTPUT outputtype;
    inputtype=(E_TDTV_DECODER_INPUT)(_bQueryMode&0xffff);
    outputtype=(E_TDTV_DECODER_OUTPUT)((_bQueryMode&0xffff0000)>>16);
    
    if(bType==Vtotal_Get ||bType==VActive_Get)
    {
        if(outputtype==E_TDTV_DECODER_OUTPUT_3D_2_2D)
        {
            if(inputtype==E_TDTV_DECODER_INPUT_SBS_P)
            {
                return wValue;
            }
            
            if(inputtype==E_TDTV_DECODER_INPUT_SBS_I)
            {
                if(bType==Vtotal_Get)
                {
                    if (fgIsVideoTiming(_bDviTiming))
                    {
                        return Get_VGAMODE_IVTOTAL(_bDviTiming);
                    }
                    else
                    {
                        return wValue;   
                    }
                }
                if(bType==VActive_Get)
                {
                    if (fgIsVideoTiming(_bDviTiming))
                    {
                        return Get_VGAMODE_IPV_LEN(_bDviTiming);
                    }
                    else
                    {
                        return wValue;
                    }
                }
            }
            
            if(inputtype==E_TDTV_DECODER_INPUT_TB_P ||inputtype==E_TDTV_DECODER_INPUT_TB_I)
            {
                if(bType==Vtotal_Get)
                {
                    if (fgIsVideoTiming(_bDviTiming))
                    {
                        return Get_VGAMODE_IVTOTAL(_bDviTiming);
                    }
                    else
                    {
                        return wValue;   
                    }
                }
                if(bType==VActive_Get)
                {
                    if (fgIsVideoTiming(_bDviTiming))
                    {
                        return Get_VGAMODE_IPV_LEN(_bDviTiming)/2;
                    }
                    else
                    {
                        return wValue/2;
                    }
                }
            }
               
            if(inputtype==E_TDTV_DECODER_INPUT_FP_P ||inputtype==E_TDTV_DECODER_INPUT_FP_I)
            {
                if(bType==Vtotal_Get)
                {
                    if((FP_interlace==bHDMI3DIPCheck())||(FP_progressive==bHDMI3DIPCheck())) 
                    {
                        return Get_VGAMODE_IVTOTAL(_bDviTiming); // Vtotal can't divide by 2, otherwise ddds won't lock.
                    }
                }
                else if(bType==VActive_Get)
                {
                    if(FP_progressive==bHDMI3DIPCheck()) 
                    {
                        return (Get_VGAMODE_IPV_LEN(_bDviTiming) - Get_VGAMODE_IPV_STA(_bDviTiming)) >> 1;
                    }	
                    if(FP_interlace==bHDMI3DIPCheck()) 
                    {
                        return (Get_VGAMODE_IPV_LEN(_bDviTiming) - Get_VGAMODE_IPV_STA(_bDviTiming) * 3 - 2) >> 2;//22(real), 23(by decoder), 22(by decoder, this value is in timing table), 23(by decoder)
                    }
                }
                else
                {
                    return wValue/2;
                }
            }
    	}
    }
    
    if(bType==Htotal_Get ||bType==HActive_Get)
    {
        if(outputtype==E_TDTV_DECODER_OUTPUT_3D_2_2D)
        {
            if(inputtype==E_TDTV_DECODER_INPUT_SBS_P ||inputtype==E_TDTV_DECODER_INPUT_SBS_I)
            {
                if(bType==Htotal_Get)
                {
                    if (fgIsVideoTiming(_bDviTiming))
                    {
                        return Get_VGAMODE_IHTOTAL(_bDviTiming);
                    }
                    else
                    {
                        return wValue;
                    }
                }
                if(bType==HActive_Get)
                {
                    if (fgIsVideoTiming(_bDviTiming))
                    {
                        if (bHDMI422Input() && ((_bDviTiming == MODE_525I) || ( _bDviTiming == MODE_625I)))
                        {
                            return (Get_VGAMODE_IPH_WID(_bDviTiming) >> 1 )/2;
                        }
                        else		//480i 444 has two pixel repeat type, Y0 Y0 Y1 Y1 Y2 Y2 or Y1 Y2 Y3 Y4 Y5 , drop pixel will be decided by PE , Hactive must be 1440
                        {
                            return (Get_VGAMODE_IPH_WID(_bDviTiming) >> (RegReadFldAlign(CCIR_00, CCH_CEN_SEL)))/2;
                        }
                    }
                    else
                    {
                        return wValue/2;
                    }
                }	
            }
            
            if(inputtype==E_TDTV_DECODER_INPUT_TB_P ||inputtype==E_TDTV_DECODER_INPUT_TB_I)
            {
                if(bType==Htotal_Get)
                {
                    if (fgIsVideoTiming(_bDviTiming))
                    {
                        return Get_VGAMODE_IHTOTAL(_bDviTiming);
                    }
                    else
                    {
                        return wValue;
                    }
                }
                if(bType==HActive_Get)
                {
                    if (fgIsVideoTiming(_bDviTiming))
                    {
                        if (bHDMI422Input() && ((_bDviTiming == MODE_525I) || ( _bDviTiming == MODE_625I)))
                        {
                            return (Get_VGAMODE_IPH_WID(_bDviTiming) >> 1 );
                        }
                        else		//480i 444 has two pixel repeat type, Y0 Y0 Y1 Y1 Y2 Y2 or Y1 Y2 Y3 Y4 Y5 , drop pixel will be decided by PE , Hactive must be 1440
                        {
                            return (Get_VGAMODE_IPH_WID(_bDviTiming) >> (RegReadFldAlign(CCIR_00, CCH_CEN_SEL)));
                        }
                    }
                    else
                    {
                        return wValue;
                    }
                }
            }
        }
    }
return wValue;
}

/**
 * @brief 	Get htotal,vtotal,hactive,vactive of 3D
 * @param 1
 * @retval type value 1=Vtotal , 2=Vactive , 3=Htotal, 4=Hactive,  5=reflash rate
 */
UINT16 wHDMI3DGetHVParameter(UINT8 bType)
{
    HDMI_3D_INFOFRAME * pInfo;
    pInfo = API_HDMI_3D_Get_InfoFrame();
	if(1==bDviBypassAp3DRule())
	{
	    if (bType == Vtotal_Get)
	    {
	        switch (pInfo->HDMI_3D_Structure)
	        {
	            case HDMI_3D_Structure_FramePacking:
	               if((FP_interlace==bHDMI3DIPCheck())||(FP_progressive==bHDMI3DIPCheck())) 
	                {
	                    return Get_VGAMODE_IVTOTAL(_bDviTiming) >> 1;
	                }
	                else
	                {
	                    if (fgHDMIinterlaced())
	                    {
	                        return wDVIGetVtotal();
	                    }
	                    else
	                    {
	                        return wDVIGetVtotal() >> 1;
	                    }
	                }

	            case HDMI_3D_Structure_LineAlternative:
	                return wDVIGetVtotal() >> 1 ;
	            case HDMI_3D_Structure_SideBySideFull:
	            case HDMI_3D_Structure_SideBySideHalf:
	            case HDMI_3D_Structure_TopBottom:
	                if (fgHDMIinterlaced())
	                {
    	                if ( fgIsVideoTiming(_bDviTiming))
    	                {
    	                    return Get_VGAMODE_IVTOTAL(_bDviTiming);
    	                }
    	                else
    	                {
	                    return wDVIGetVtotal() << 1;
	                }
	                }
	                else
	                {
	                    return wDVIGetVtotal();
	                }
	            
	            default:
	                return wDVIGetVtotal();
	        }

	    }
	    if (bType == VActive_Get)
	    {
	        switch (pInfo->HDMI_3D_Structure)
	        {

	            case HDMI_3D_Structure_FramePacking:
			  if(FP_progressive==bHDMI3DIPCheck()) 
	                {
	                    return (Get_VGAMODE_IPV_LEN(_bDviTiming) - Get_VGAMODE_IPV_STA(_bDviTiming)) >> 1;
	                }
	                else if(FP_interlace==bHDMI3DIPCheck()) 
	                {
	                    return (Get_VGAMODE_IPV_LEN(_bDviTiming) - Get_VGAMODE_IPV_STA(_bDviTiming) * 3 - 2) >> 1;//22(real), 23(by decoder), 22(by decoder, this value is in timing table), 23(by decoder)
	                }
	                else
	                {
	                    if (fgHDMIinterlaced())
	                    {
	                        return bDVIGetHeight();
	                    }
	                    else
	                    {
	                        return bDVIGetHeight() >> 1;
	                    }
	                }

	            case HDMI_3D_Structure_LineAlternative:
	                return bDVIGetHeight() >> 1 ;
	            case HDMI_3D_Structure_SideBySideFull:
	            case HDMI_3D_Structure_SideBySideHalf:
	            case HDMI_3D_Structure_TopBottom:
	                if (fgHDMIinterlaced())
	                {
	                    if ( fgIsVideoTiming(_bDviTiming))
    	                {
    	                     return Get_VGAMODE_IPV_LEN(_bDviTiming);
    	                }
    	                else
    	                {
	                    return bDVIGetHeight() << 1 ;
	                }
	                }
	                else
	                {
	                    return bDVIGetHeight();

	                }
	            
	            default:
	                return bDVIGetHeight();
	        }
	    }
	    if (bType == Htotal_Get)//Htotal
	    {
	        switch (pInfo->HDMI_3D_Structure)
	        {
	            case HDMI_3D_Structure_FramePacking:
	                if(FP_progressive==bHDMI3DIPCheck()) 
	                {
	                    return Get_VGAMODE_IHTOTAL(_bDviTiming);
	                }
	                else
	                {
	                    return wDVIGetHtotal();
	                }

	            case HDMI_3D_Structure_LineAlternative:
	            case HDMI_3D_Structure_SideBySideFull:
	            case HDMI_3D_Structure_SideBySideHalf:
	            case HDMI_3D_Structure_TopBottom:
	            default:
	                return wDVIGetHtotal();
	        }

	    }
	    if (bType == HActive_Get)
	    {
	        switch (pInfo->HDMI_3D_Structure)
	        {
	            case HDMI_3D_Structure_FramePacking:
	            case HDMI_3D_Structure_LineAlternative:
	            case HDMI_3D_Structure_SideBySideFull:
	            case HDMI_3D_Structure_SideBySideHalf:
	            case HDMI_3D_Structure_TopBottom:
	            default:
	            if (fgIsVideoTiming(_bDviTiming))
	            {
			#if CCIR_decode_444  //480i 422 input must Y0 Y0 Y1 Y1 Y2 Y2 pixel repeat, CCIR decoder will drop pxiel automatically and Hactive must be 720
	                if (bHDMI422Input() && ((_bDviTiming == MODE_525I) || ( _bDviTiming == MODE_625I)))
	                {
	                    return (Get_VGAMODE_IPH_WID(_bDviTiming) >> 1 );
	                }
	                else		//480i 444 has two pixel repeat type, Y0 Y0 Y1 Y1 Y2 Y2 or Y1 Y2 Y3 Y4 Y5 , drop pixel will be decided by PE , Hactive must be 1440
	                {
	                    return (Get_VGAMODE_IPH_WID(_bDviTiming) >> (RegReadFldAlign(CCIR_00, CCH_CEN_SEL)));
	                }
			#else
	                return (Get_VGAMODE_IPH_WID(_bDviTiming) >> (RegReadFldAlign(DVI_00, DVI_CEN_SEL)));
			#endif
	            }
	            else
	            {
	                return bDVIGetWidth();
	            }
	        }
	    }
	    if (bType == Reflash_rate_Get)
	    {
	        switch (pInfo->HDMI_3D_Structure)
	        {
	            case HDMI_3D_Structure_FramePacking:
	                    if (_bDviTiming == MODE_DE_MODE)
	                    {
	                        return _bDviVclk * 2;
	                    }
	                    else if(FP_interlace==bHDMI3DIPCheck()) 
	                    {
	                        return Get_VGAMODE_IVF(_bDviTiming)*4;
	                    }
	                    else
	                    {
	                        return Get_VGAMODE_IVF(_bDviTiming)*2;
	                    }
	            case HDMI_3D_Structure_LineAlternative:
	            case HDMI_3D_Structure_SideBySideFull:
	            case HDMI_3D_Structure_SideBySideHalf:
	            case HDMI_3D_Structure_TopBottom:
	            default:
	                return _bDviVclk;
	        }
	    }
	}
	else
	{
		switch(bType)
		{
			case Vtotal_Get:
				return wHDMIGet2DmodeForce3DHVParameter(1,wDVIGetVtotal());
			case VActive_Get:
				return wHDMIGet2DmodeForce3DHVParameter(2,bDVIGetHeight());
			case Htotal_Get:
				return wHDMIGet2DmodeForce3DHVParameter(3,wDVIGetHtotal());
			case HActive_Get:
				return wHDMIGet2DmodeForce3DHVParameter(4,bDVIGetWidth());
			case Reflash_rate_Get:
				return _bDviVclk;
			default:
			       return 0;
		}
	}
    return 0;
}

UINT16 wDviGetTableHactive(void)
{
    UINT16 wHDMIHactive;
    wHDMIHactive = wHDMIResoWidth();
    if ( fgIsVideoTiming(_bDviTiming))
    {
        if ((wHDMIHactive > (Get_VGAMODE_IPH_WID(_bDviTiming) - 40))
                && (wHDMIHactive < (Get_VGAMODE_IPH_WID(_bDviTiming) + 40)))
        {
            return wHDMIHactive;
        }
        else
        {
            return Get_VGAMODE_IPH_WID(_bDviTiming);
        }
    }
    else
    {
        return  wHDMIHactive ;
    }
}

/**
 * @brief 	when input is SBS or TB then output is 3d to 2D
 * @param None
 * @retval None
 */
void vHDMI3Dto2DSpecialPorchAndWidth(void)
{
    	E_TDTV_DECODER_INPUT inputtype;
    	E_TDTV_DECODER_OUTPUT outputtype;
    	inputtype=(E_TDTV_DECODER_INPUT)(_bQueryMode&0xffff);
	outputtype=(E_TDTV_DECODER_OUTPUT)((_bQueryMode&0xffff0000)>>16);;

    		if((inputtype==E_TDTV_DECODER_INPUT_SBS_I) ||(inputtype==E_TDTV_DECODER_INPUT_SBS_P))
   	 	{
   			if(outputtype==E_TDTV_DECODER_OUTPUT_3D_2_2D)
   			{
   				if (0 == _bUseLorRFlag)//0 is L, 1 is R.
	                    {
	                        vCCIRSetAvStart(0);
	                    }
	                    else
	                    {
	                        vCCIRSetAvStart(wDviGetTableHactive() / 2);
	                    }
	                    vCCIRSetAvWidth( wDviGetTableHactive() / 2);
	                    vCCIRSetVsStart( wVSyncWidth - 3);
	                    vCCIRSetVsWidth(_wDviHeight);
	   		}
    		}
    		if((inputtype==E_TDTV_DECODER_INPUT_TB_I) ||(inputtype==E_TDTV_DECODER_INPUT_TB_P))
   	 	{
   			if(outputtype==E_TDTV_DECODER_OUTPUT_3D_2_2D)
   			{
	                    vCCIRSetAvStart(0);
	                    vCCIRSetAvWidth( _wDviWidth);
	                    if (!_bUseLorRFlag) //0 is L , 1 is R
	                    {
	                        vCCIRSetVsStart( wVSyncWidth - 3);
	                    }
	                    else
	                    {
	                        vCCIRSetVsStart( wVSyncWidth - 3 + (_wDviHeight / 2));
	                    }
	                    vCCIRSetVsWidth(_wDviHeight / 2);
   			}
    		}

		if((inputtype==E_TDTV_DECODER_INPUT_FS_I) ||(inputtype==E_TDTV_DECODER_INPUT_FS_P))
   	 	{
   			if(outputtype==E_TDTV_DECODER_OUTPUT_3D_2_2D)
   			{
	                    vCCIRSetAvStart(0);
	                    vCCIRSetAvWidth( wDviGetTableHactive());
	                    vCCIRSetVsStart( wVSyncWidth - 3);
	                    vCCIRSetVsWidth(_wDviHeight);
   			}
    	}

   		if(outputtype==E_TDTV_DECODER_OUTPUT_NATIVE)
   		{
	       	vCCIRSetAvStart(0);
	              vCCIRSetAvWidth( wDviGetTableHactive());
	              vCCIRSetVsStart( wVSyncWidth - 3);
	              vCCIRSetVsWidth(_wDviHeight);
	   	}


}
/**
 * @brief 	2D/3D Set H_AV_width, H_AV_start,VDE_width, VDE_start
 * @param None
 * @retval None
 */
void vHDMISetDefaultPorchAndWidth(void)
{
    E_TDTV_DECODER_INPUT inputtype;
    E_TDTV_DECODER_OUTPUT outputtype;
    UINT16 wVactive, wVblank;
    UINT8 fg3DSettingFailed = 0;//0: succeed; 1: failed.
    HDMI_3D_INFOFRAME * pInfo;
    pInfo = API_HDMI_3D_Get_InfoFrame();

    inputtype=(E_TDTV_DECODER_INPUT)(_bQueryMode&0xffff);
    outputtype=(E_TDTV_DECODER_OUTPUT)((_bQueryMode&0xffff0000)>>16);

    Printf("----------vHDMISetDefaultPorchAndWidth----------\n");

    //if (1 == pInfo->HDMI_3D_Enable)//3D
    if (bHDMI3DPacketVaild())
    {
        Printf("-------3D-------\n");
        if(0==bDviBypassAp3DRule()) 
        {
            Printf("-----force 2D mode, HDMI_3D_Structure = %d-----\n", pInfo->HDMI_3D_Structure);
            switch (pInfo->HDMI_3D_Structure)
            {
                case HDMI_3D_Structure_FramePacking:
                	if((inputtype==E_TDTV_DECODER_INPUT_FP_P ||inputtype==E_TDTV_DECODER_INPUT_FP_I) &&outputtype==E_TDTV_DECODER_OUTPUT_3D_2_2D)
                	{
	                    if (_bDviTiming == MODE_DE_MODE)
	                    {
	                        wVactive = _wDviWidth / 2;
	                        wVblank = wVSyncWidth;//should be verify, not accurately
	                    }
	                    else if(FP_interlace==bHDMI3DIPCheck()) 
	                    {
	                        wVactive = (Get_VGAMODE_IPV_LEN(_bDviTiming) - Get_VGAMODE_IPV_STA(_bDviTiming) * 3 - 2) / 4;// field active
	                        wVblank = Get_VGAMODE_IPV_STA(_bDviTiming);	// small one, 22, not 23.
	                    }
	                    else
	                    {
	                        wVactive = (Get_VGAMODE_IPV_LEN(_bDviTiming) - Get_VGAMODE_IPV_STA(_bDviTiming)) / 2;
	                        wVblank = Get_VGAMODE_IPV_STA(_bDviTiming);
	                    }
	                    vCCIRSetAvStart(0);
	                    vCCIRSetAvWidth( _wDviWidth);
	                    if (0 == _bUseLorRFlag)//0 is L, 1 is R.
	                    {
	                        vCCIRSetVsStart( wVSyncWidth - 3);
	                    }
	                    else
	                    {
	                       if(FP_interlace==bHDMI3DIPCheck()) 
	                        {
	                            vCCIRSetVsStart( wVSyncWidth - 3 + wVactive + wVblank);//wVblank is different with 3D mode
	                        }
	                        else
	                        {
	                            vCCIRSetVsStart( wVSyncWidth - 3 + wVactive + wVblank);
	                        }
	                    }
	                    vCCIRSetVsWidth(wVactive);
                	}
                	else
                	{
                		vHDMI3Dto2DSpecialPorchAndWidth();
                	}
                    break;

                case HDMI_3D_Structure_LineAlternative:
                    fg3DSettingFailed = 1;
                    Printf("LineAlternative can not force 3D to 2D\n");
                    break;

                case HDMI_3D_Structure_SideBySideFull:
                case HDMI_3D_Structure_SideBySideHalf:
                	if((inputtype==E_TDTV_DECODER_INPUT_SBS_P ||inputtype==E_TDTV_DECODER_INPUT_SBS_I) &&outputtype==E_TDTV_DECODER_OUTPUT_3D_2_2D)
                	{
	                    if (0 == _bUseLorRFlag)//0 is L, 1 is R.
	                    {
	                        vCCIRSetAvStart(0);
	                    }
	                    else
	                    {
	                        vCCIRSetAvStart(wDviGetTableHactive() / 2);
	                    }
	                    vCCIRSetAvWidth( wDviGetTableHactive() / 2);
	                    vCCIRSetVsStart( wVSyncWidth - 3);
	                    vCCIRSetVsWidth(_wDviHeight);
                	}
                	else
                	{
                		vHDMI3Dto2DSpecialPorchAndWidth();
                	}
                    break;

                case HDMI_3D_Structure_TopBottom:
                	if((inputtype==E_TDTV_DECODER_INPUT_TB_P ||inputtype==E_TDTV_DECODER_INPUT_TB_I) &&outputtype==E_TDTV_DECODER_OUTPUT_3D_2_2D)
                	{
	                    vCCIRSetAvStart(0);
	                    vCCIRSetAvWidth( _wDviWidth);
	                    if (!_bUseLorRFlag) //0 is L , 1 is R
	                    {
	                        vCCIRSetVsStart( wVSyncWidth - 3);
	                    }
	                    else
	                    {
	                        vCCIRSetVsStart( wVSyncWidth - 3 + (_wDviHeight / 2));
	                    }
	                    vCCIRSetVsWidth(_wDviHeight / 2);
                	}
                	else
                	{
                		vHDMI3Dto2DSpecialPorchAndWidth();
                	}
                    break;

                default:
                    vCCIRSetAvStart(0x0);

                    if ((_bDviTiming == MODE_525I) || (_bDviTiming == MODE_625I))
                    {
                        if (bHDMI422Input())
                        {
                            vCCIRSetAvWidth( wDviGetTableHactive() / 2);
                        }
                        else
                        {
                            vCCIRSetAvWidth( wDviGetTableHactive());
                        }
                    }
                    else
                    {
                        vCCIRSetAvWidth( wDviGetTableHactive());
                    }
                    vCCIRSetVsStart( wVSyncWidth - 3);
                    vCCIRSetVsWidth(_wDviHeight);
                    break;
            }

        }
        else//play 3D
        {
            Printf("-----3D playing mode, HDMI_3D_Structure = %d-----\n", pInfo->HDMI_3D_Structure);
            switch (pInfo->HDMI_3D_Structure)
            {
                case HDMI_3D_Structure_FramePacking:
                    wVactive = (Get_VGAMODE_IPV_LEN(_bDviTiming) - Get_VGAMODE_IPV_STA(_bDviTiming)) / 2;
                    wVblank = Get_VGAMODE_IPV_STA(_bDviTiming);
                    if (_bDviTiming == MODE_DE_MODE)
                    {
                        fg3DSettingFailed = 1;
                        break;
                    }
                    vCCIRSetAvStart(0);
                    vCCIRSetAvWidth(_wDviWidth);
                    vCCIRSetVsStart( wVSyncWidth - 2);
                    vCCIRSetVsWidth(wVactive);
                    break;

                case HDMI_3D_Structure_LineAlternative:
                case HDMI_3D_Structure_SideBySideFull:
                case HDMI_3D_Structure_SideBySideHalf:
                case HDMI_3D_Structure_TopBottom:
                default:
                    vCCIRSetAvStart(0);
                    vCCIRSetAvWidth( wDviGetTableHactive());
                    vCCIRSetVsStart( wVSyncWidth - 3);
                    vCCIRSetVsWidth(_wDviHeight);
                    break;
            }
        }
    }

    //if ((0 == pInfo->HDMI_3D_Enable) || (1 == fg3DSettingFailed))//real 2D, or 3D setting failed
    if ((0 == bHDMI3DPacketVaild()) || (1 == fg3DSettingFailed))
    {

    	if(1==bDviBypassAp3DRule())
    	{
	        Printf("Real 2D, _bDviTiming=%d, wDviGetTableHactive()=%d, wVSyncWidth=%d, _wDviHeight=%d\n", _bDviTiming, wDviGetTableHactive(), wVSyncWidth, _wDviHeight);
	        /*-------------- CCIR set AvStart-----------------*/
	        vCCIRSetAvStart(0x0);

	        if ((_bDviTiming == MODE_525I) || (_bDviTiming == MODE_625I))
	        {
	#if CCIR_decode_444
	            if (bHDMI422Input())
	            {
	                vCCIRSetAvWidth( wDviGetTableHactive() / 2);
	            }
	            else
	            {
	                vCCIRSetAvWidth( wDviGetTableHactive());
	            }
	#else
	            vCCIRSetAvWidth( wDviGetTableHactive() / 2);
	#endif
	        }
	        else
	        {
	            vCCIRSetAvWidth( wDviGetTableHactive());
	        }
	        /*-------------- DVI set AvStart-----------------*/
	        vDviSetAvStart(0x0);
	        vDviSetAvWidth( wDviGetTableHactive());

	        vDviSetVsStart( wVSyncWidth - 3);
	        vCCIRSetVsStart( wVSyncWidth - 3);

	        vDviSetVsWidth(_wDviHeight);
	        vCCIRSetVsWidth(_wDviHeight);
    	}
    	else	 //input 2D force 3D to 2D
    	{
		vHDMI3Dto2DSpecialPorchAndWidth();
    	}
    }
}
/**
 * @brief 	DVI Timing search function.
 * @param	bMode: 0 - VGA Timing Search, 1 - Video Timing Search
 * @retval	The found DVI timing or return MODE_NOSUPPORT
 */
//PRIVATE UINT8 bDviStdTimingSearch(UINT8 bMode)
UINT8 bDviStdTimingSearch(UINT8 bMode)
{
    UINT8 bSearch;
    UINT8 bSearchEnd;

#if USE_DVI_HVTOTAL_SEARCH
    UINT16 wTmp;
#endif

    if (bMode)
    {
        // Video Mode
        bSearch = HDTV_SEARCH_START;
        bSearchEnd = HDTV_SEARCH_END;
    }
    else
    {
        // VGA Mode
        bSearch = DVI_SEARCH_START;
        bSearchEnd = DVI_SEARCH_END;
    }

#if DVI_DEBUG
    LOG(3, "vsync frequency: %d\n", _bDviVclk);
    LOG(3, "hsync frequency: %d\n", _wDviHclk);
    LOG(3, "Htotal: %d\n", _wDviHtotal);
#endif

    do
    {
#ifdef CC_BYPASS_AVI_INFOFRAME
        if (Get_HDMIMODE_SupportVideo(bSearch) == TRUE)
#endif
        {
            if ((_bDviVclk >= (Get_VGAMODE_IVF(bSearch) - 2)) &&
            (_bDviVclk <= (Get_VGAMODE_IVF(bSearch) + 2)))
            {
                if ((_wDviHclk >= (Get_VGAMODE_IHF(bSearch) - 5)) &&
                (_wDviHclk <= (Get_VGAMODE_IHF(bSearch) + 5)))
                {
#if FROM_HDMIDEC
                    // check interlaced
                    if ((/*!bHDMI3DPacketVaild() && */Get_VGAMODE_INTERLACE(bSearch)) != (fgHDMIinterlaced()))
                    {
                        LOG(3, "Get_VGAMODE_INTERLACE(bSearch): %d\n", Get_VGAMODE_INTERLACE(bSearch));
                        LOG(3, "fgHDMIinterlaced: %d\n", fgHDMIinterlaced());
                        continue;
                    }
#endif
#if USE_DVI_HVTOTAL_SEARCH
                    // if (!bMode)
                    {
                        wTmp = _wDviHtotal ;
                        LOG(6, " timing search 3\n");
                        //vDSSetCLKMeasure();
                        LOG(1, "bSearch= %d\n", bSearch);
                         LOG(1, "_wDviHtotal= %d\n", _wDviHtotal);
                        LOG(1, "Get_VGAMODE_IHTOTAL= %d\n", Get_VGAMODE_IHTOTAL(bSearch) );
                         LOG(1, "bDVIGetHeight()= %d\n", bDVIGetHeight());
                         LOG(1, "Get_VGAMODE_IPV_LEN=%d\n", Get_VGAMODE_IPV_LEN(bSearch));
                        if ((wTmp > (Get_VGAMODE_IHTOTAL(bSearch) - 40)) &&
                                (wTmp < (Get_VGAMODE_IHTOTAL(bSearch) + 40)))
                        {
                        LOG(1,"bSearch=%d\n",bSearch);
                        LOG(1,"UNRECRX0_0=%x\n",u1RegRead1B(UNRECRX0_0));
                       LOG(1,"INTR_STATE0=%x\n",RegReadFldAlign(INTR_STATE0, INTR3_NEW_UNREC));
                       LOG(1,"UNRECRX1_3=%x\n",u1RegRead1B(UNRECRX1_3) >> 5);
                        	  if (bHDMI3DPacketVaild())
                        	  {                   
                        	  	if(((bDVIGetHeight()<<fgHDMIinterlaced()) > (Get_VGAMODE_IPV_LEN(bSearch) - 3)) && ((bDVIGetHeight()<<fgHDMIinterlaced()) < (Get_VGAMODE_IPV_LEN(bSearch) + 3)))
                        	  	{
                        	  		LOG(6, " 3D timing table: %d\n", bSearch);
	                            	LOG(6, "HTOTAL in timing table: %d\n", Get_VGAMODE_IHTOTAL(bSearch));
	                            	return bSearch;
                        	  	}
                        	  }
                        	  else
                        	  {
                        	  	 LOG(6, " timing table: %d\n", bSearch);
	                            LOG(6, "HTOTAL in timing table: %d\n", Get_VGAMODE_IHTOTAL(bSearch));
	                            return bSearch;
                        	  }
                        }
                    }
                    // else
#else
                    {
                        return bSearch;
                    }
#endif
                }
            }
        }
    }
    while (++bSearch <= bSearchEnd);

    // LOG(6, "timing search failed\n");
    return MODE_NOSUPPORT;
}
	
/**
 * @brief 	DVI set input paramter function.
 * @param	bMode - DVI Timing.
 * @retval	None
 */
PRIVATE void vDviSetInputCapture(UINT8 bMode)
{
    UNUSED(bMode);

    //disable field indicator detection when it is not interlaced signal
    if ((fgHDMIinterlaced() == 0) && ( Get_VGAMODE_INTERLACE(_bDviTiming) == 0))
    {
        vRegWriteFldAlign(DVI_00, 0, DVI_FLD_DET); //disable DVI field indicator detection
    }
    else
    {
        vRegWriteFldAlign(DVI_00, 1, DVI_FLD_DET); //disable DVI field indicator detection
    }

    //Set blank level depend on HDMI color space when data not in DE signal
#if CCIR_decode_444
    if (bHDMI422Input())
    {
        vRegWriteFldAlign(CCIR_00, 0 , CCH_DE0_OFF);
        vRegWriteFldAlign(CCIR_00, 0 , CCH_YC);
    }
    else
    {
        vRegWriteFldAlign(CCIR_00, 1 , CCH_DE0_OFF);
        vRegWriteFldAlign(CCIR_00, (bHDMIInputType() ? 0x00 : 1) , CCH_YC);
    }
#else
    vRegWrite4BMsk(DVI_02, Fld2Msk32(DVI_DE0_OFF) , Fld2Msk32(DVI_DE0_OFF));
    vRegWrite4BMsk(DVI_02, (bHDMIInputType() ? 0x00 : Fld2Msk32(DVI_YC)) , Fld2Msk32(DVI_YC));
#endif

    // Set DVI_CEN_SEL for sub-sampling (The input clock is doubled in these two timings.)
    //reset sub_sampling
    vRegWriteFldAlign(CCIR_00, 0x0 , CCH_CEN_SEL);

    	vHDMI3DSetConfig();
    	
    if ((_bDviTiming == MODE_525I) || (_bDviTiming == MODE_625I) || \
            (_bDviTiming == MODE_480P_OVERSAMPLE) || (_bDviTiming == MODE_576P_OVERSAMPLE))
    {
#if CCIR_decode_444
        if (bHDMI422Input())
        {
            vRegWriteFldAlign(CCIR_00, 0, CCH_CEN_SEL);
            vRegWriteFldAlign(CCIR_00, 0x1, CCH_CLK_DIV);
        }
        else
        {
            vRegWriteFldAlign(CCIR_00, 1, CCH_CEN_SEL);
            vRegWriteFldAlign(CCIR_00, 0x0, CCH_CLK_DIV);
        }
#else
        vRegWriteFldAlign(DVI_00, 1, DVI_CEN_SEL);
        vRegWriteFldAlign(CCIR_00, 0x1, CCH_CLK_DIV);
#endif

    }
    else
    {
#if CCIR_decode_444
        vRegWriteFldAlign(CCIR_00, 0, CCH_CEN_SEL);
#else
        vRegWriteFldAlign(DVI_00, 0x0, DVI_CEN_SEL);
#endif
        vRegWriteFldAlign(CCIR_00, 0x0, CCH_CLK_DIV);
    }

    if (RegReadFldAlign(VID_CH_MAP, INTERLACEDOUT))
    {
        vRegWriteFldAlign(VID_SET, 1, C_INT_BYP);
        vRegWriteFldAlign(LPF_PIP_00, 1, CCH_FIX_VCNT);
        vRegWriteFldAlign(SYS_02, 1, CCH_VCNT_OFFSET_F0);
        vRegWriteFldAlign(SYS_02, 0, CCH_VCNT_OFFSET_F1);
        vRegWriteFldAlign(DVI_02, 0x1, DVI_FIX_VCNT);
        vRegWriteFldAlign(DVI_02, 0x1, DVI_VCNT_OFFSET_F0);
#if CCIR_decode_444
        vRegWriteFldAlign(SYS_01, 1, CCH_VSLEN_SEL);
        vUtDelay1ms(20);
        wVSyncWidth = wCCIRVsyncWidth();
        vRegWriteFldAlign(SYS_01, 0, CCH_VSLEN_SEL);
#else
        vRegWriteFldAlign(SYS_01, 1, DVI_VSLEN_SEL);
        vUtDelay1ms(20);
        wVSyncWidth = wDVIDEVsyncWidth();
        vRegWriteFldAlign(SYS_01, 0, DVI_VSLEN_SEL);
#endif
        wVSyncWidth = wVSyncWidth + 1;
    }
    else
    {
        vRegWriteFldAlign(VID_SET, 0, C_INT_BYP);
        vRegWriteFldAlign(LPF_PIP_00, 0, CCH_FIX_VCNT);
        vRegWriteFldAlign(SYS_02, 0, CCH_VCNT_OFFSET_F0);
        vRegWriteFldAlign(DVI_02, 0x0, DVI_FIX_VCNT);
        vRegWriteFldAlign(DVI_02, 0x0, DVI_VCNT_OFFSET_F0);
#if CCIR_decode_444
        wVSyncWidth = wCCIRVsyncWidth();
#else
        wVSyncWidth = wDVIDEVsyncWidth();
#endif

    }

    vHDMISetDefaultPorchAndWidth();

    //FIXME,CIWU,for CCIR 444
    // reset the blank measurement + hold the blanking measurement
    vRegWrite4BMsk(DVI_02, Fld2Msk32(DVI_BLANK_RESET) | Fld2Msk32(DVI_BLANK_HOLD) , Fld2Msk32(DVI_BLANK_RESET) | Fld2Msk32(DVI_BLANK_HOLD));

    vRegWriteFldAlign(CCIR_01, 0x0, CCH_FLD_VPOS);	// when 1152i set 1

#if 0 //replace by 20022420[31] DVI_VHS_END_MODE
    if (_bDviTiming == MODE_525I)//for Sony San-diego Pace STB 480i issue
    {
        //vRegWriteFldAlign(DVI_01,0x1,DVI_HDELEN_SEL);
    }
    else
    {
        vRegWriteFldAlign(DVI_01, 0x0, DVI_HDELEN_SEL);
    }
#endif

    if (_bHdmiCmd != 0x20)
    {
        if (RegReadFldAlign(VID_VRES, VID_VRES_MUTE))
        {
            vRegWriteFldAlign(VID_VRES, 0x1, VRES_MUTE_CLR);
            vRegWriteFldAlign(VID_VRES, 0x0, VRES_MUTE_CLR);
        }
    }
}


/**
 * @brief 	DVI Call-back function for source select connect or disconnect.
 * @param	bchannel - SV_VP_MAIN / SV_VP_PIP
 * @param	fgIsOn - SV_ON/SV_OFF
 * @retval	None
 */
void vDviConnect(UINT8 bchannel, UINT8 fgIsOn)
{
    //UINT32 HDMIBypassFlag;
    SRM_TV_MODE_T eNewMode = SRM_GetTvMode();
#if ENABLE_DE_MODE
    if (bEnDVIDE == 1)
    {
        vEnterDEMode(SV_ON);
    }
    else
    {
        vEnterDEMode(SV_OFF);
    }
#endif

    if (fgIsOn == SV_ON)
    {
        bHDMIAudioSigmaDeltaSetting(0x0, 0x0, 0x0);
#if DVI_DEBUG
        LOG(6, "vDviConnect On\n");
#endif
#if IC_5365_VERIFY
        vIO32WriteFldAlign(0xf000d158, 0x1, Fld(1, 7, AC_MSKB0));
#endif
        _bDviExtSrc = SV_DV_HDMI;
        if (bchannel == SV_VP_MAIN)
        {
            // Set Color System
            switch (_bDviExtSrc)
            {
                case SV_DV_DVI:
                    _bMAIN_COLOR_SYS = SV_COLOR_RGB;
                    break;
                case SV_DV_HDMI:
                    /* Separate Analog/Digital of YPbPr source.*/
                    //mtk01140 20070705 538x HDMI input format change to always 444
                    //todo: must separate by timing
                    //	PC => SV_COLOR_HDMI_RGB,
                    //	Video 709 => SV_COLOR_HDMI_422_709
                    //	Video 601 => SV_COLOR_HDMI_422_601
                    if (bHDMIInputType())
                    {
                        //RGB
                        _bMAIN_COLOR_SYS = SV_COLOR_HDMI_RGB;
                    }
                    else
                    {
                        //Ycbcr
                        if (bHDMIClocrimetry())
                        {
                            //601
                            _bMAIN_COLOR_SYS = SV_COLOR_HDMI_422_601;
                        }
                        else
                        {
                            //709
                            _bMAIN_COLOR_SYS = SV_COLOR_HDMI_422_709;
                        }
                    }
//					_bMAIN_COLOR_SYS = SV_COLOR_YPBPR_DIGITAL;
                    break;
                case SV_DV_CCIR:
                    _bMAIN_COLOR_SYS = SV_COLOR_YCBCR;
                    break;
                default:
                    break;
            }
            // End
            if (fgIsExtSrcCCIR())
            {	//need to modify???
                vRegWriteFldAlign(OMUX_00, 0x2 , OMUX_MAIN_SEL);// When in CCIR input the output mux must be set to CCH
            }
            vVDOINIrqOn(MSK_MAIN_DET);//need to modify???
            _rDVIStat.bIsMain = TRUE;
#if NEW_MODE_CHANGE_FLOW
            _bDviChkState = DVI_NO_SIGNAL;
#endif
        }
        else
        {
            // Set Color System
            switch (_bDviExtSrc)
            {
                case SV_DV_DVI:
                    _bPIP_COLOR_SYS = SV_COLOR_RGB;
                    break;
                case SV_DV_HDMI:
                    /* Separate Analog/Digital of YPbPr source.*/
                    //mtk01140 20070705 538x HDMI input format change to always 444
                    //todo: must separate by timing
                    //	PC => SV_COLOR_HDMI_RGB,
                    //	Video 709 => SV_COLOR_HDMI_422_709
                    //	Video 601 => SV_COLOR_HDMI_422_601
                    if (bHDMIInputType())
                    {
                        //RGB
                        _bPIP_COLOR_SYS = SV_COLOR_HDMI_RGB;
                    }
                    else
                    {
                        //Ycbcr
                        if (bHDMIClocrimetry())
                        {
                            //601
                            _bPIP_COLOR_SYS = SV_COLOR_HDMI_422_601;
                        }
                        else
                        {
                            //709
                            _bPIP_COLOR_SYS = SV_COLOR_HDMI_422_709;
                        }
                    }
//					_bPIP_COLOR_SYS = SV_COLOR_YPBPR_DIGITAL;
                    break;
                case SV_DV_CCIR:
                    _bPIP_COLOR_SYS = SV_COLOR_YCBCR;
                    break;
                default:
                    break;
            }
            // End
            if (fgIsExtSrcCCIR())
            {
                //vWriteVDOINMsk(OMUX00, 0x04, 0x07, 0);	// When in CCIR input the output mux must be set to CCH
                vRegWriteFldAlign(OMUX_00, 0x4 , OMUX_PIP_SEL);
            }
            vVDOINIrqOn(MSK_PIP_DET);
            _rDVIStat.bIsPip = TRUE;
        }

        vRegWrite4BMsk(DSYNC_00, 0, Fld2Msk32(BDHSYNCP) | Fld2Msk32(BDVSYNCP));

        /*-------------------------------DVI decoder initial------------------------------*/
        vRegWriteFldAlign(LPF_PIP_00, 0, DVI_3D_DISP_POL);
        vRegWriteFldAlign(DVI_00, 1, DVI_HS_SEL);
        vRegWriteFldAlign(DVI_00, 1, DVI_VS_SEL);
#if Enable_DVI_DE_VHS_END
        vRegWriteFldAlign(SYS_01, 1, DVI_DE_VHS_END);	// WT solve 262.5 line dvd player
        vRegWriteFldAlign(LPF_PIP_00, 1, DVI_VHS_END_MODE);
        vRegWriteFldAlign(CCIR_01, 1, DVI_HMID_SEL);
#else
        vRegWriteFldAlign(SYS_01, 0, DVI_DE_VHS_END);
#endif
        vRegWrite4BMsk(DVI_03, 0x0, Fld2Msk32(DVI_MSB_SW)); //layout config demo turnkey for HDMI
        vRegWrite4BMsk(DVI_03, 0x0, Fld2Msk32(DVI_RB_SW));
        //vRegWriteFldAlign(DVI_03,1,DVI_MUTE_SEL);//DVI mute from HDMI
#if CCIR_decode_444
        vRegWriteFldAlign(CCIR_01, 1, DVI_MUTE_SEL);//DVI mute from CCIR
#else
        vRegWriteFldAlign(CCIR_01, 0, DVI_MUTE_SEL);//DVI mute from HDMI
#endif
        vRegWriteFldAlign(DVI_00, 0, DVI_FLD_DET_INI);//field detect inital value
        vRegWriteFldAlign(DVI_00, 1, DVI_FLD_DET); //field indicator detection enable


        /*-------------------------------CCIR decoder initial------------------------------*/
        if (fgIsExtSrcCCIR()) // ccir video 8 from External CCIR or TTL  pin
        {
            vRegWriteFldAlign(CCIR_00, 0, CCIR656_VSRC);// ccir video 8 from External CCIR or TTL  pin
            vRegWriteFldAlign(CCIR_00, 1, CCH_SP1_FLD);//output filed from SP1
            vRegWriteFldAlign(CCIR_00, 0, CCH_MUTE_SEL);//CCIR mute sel from SP1
#if (CCIR_FORMAT==CCIR16_SEPARATE)
            vRegWrite4BMsk(CCIR_00, 0x0, Fld2Msk32(CCH_SRC8_SEL));// CCH_SRC_SEL ==> Video 16
            vRegWriteFldAlign(CCIR_00, 0x3 , CCH_MODE);// CCH_MODE ==> SEPARATE
#elif (CCIR_FORMAT==CCIR16_EMBEDDED)
            vRegWrite4BMsk(CCIR_00, 0x0, Fld2Msk32(CCH_SRC8_SEL));// CCH_SRC_SEL ==> Video 16
            vRegWriteFldAlign(CCIR_00, 0x2 , CCH_MODE); // CCH_MODE ==> Embedded format 2
#elif (CCIR_FORMAT==CCIR8_SEPARATE)
            vRegWrite4BMsk(CCIR_00, Fld2Msk32(CCH_SRC8_SEL), Fld2Msk32(CCH_SRC8_SEL)); // CCH_SRC_SEL ==> Video 8
            vRegWriteFldAlign(CCIR_00, 0x3 , CCH_MODE);// CCH_MODE ==> SEPARATE
            vRegWrite4BMsk(CCIR_00, 0x0, Fld2Msk32(CCIR656_VSRC)); // CCIR656_VSRC ==> ccir video 8 from analog pin
#elif (CCIR_FORMAT==CCIR8_EMBEDDED)
            vRegWrite4BMsk(CCIR_00, Fld2Msk32(CCH_SRC8_SEL), Fld2Msk32(CCH_SRC8_SEL)); // CCH_SRC_SEL ==> Video 8
            vRegWriteFldAlign(CCIR_00, 0x2 , CCH_MODE); // CCH_MODE ==> Embedded format 2
            vRegWrite4BMsk(CCIR_00, 0x0, Fld2Msk32(CCIR656_VSRC)); // CCIR656_VSRC ==> ccir video 8 from analog pin
#endif
        }
        else // CCIR input from HDMI
        {
            vRegWriteFldAlign(LPF_PIP_00, 0, CCH_3D_DISP_POL);
#if Enable_DVI_DE_VHS_END
            vRegWriteFldAlign(SYS_01, 1, CCH_VHS_END);;	// WT solve 262.5 line dvd player
            vRegWriteFldAlign(LPF_PIP_00, 1, CCH_VHS_END_MODE);
            vRegWriteFldAlign(CCIR_01, 1, CCH_HMID_SEL);
#else
            vRegWriteFldAlign(SYS_01, 0, CCH_VHS_END);
#endif
            vRegWriteFldAlign(CCIR_00, 1, CCIR656_VSRC);// ccir video 8 from DVI pin
            vRegWriteFldAlign(CCIR_00, 1, CCH_MUTE_SEL);//CCIR mute sel from HDMI
            vRegWrite4BMsk(CCIR_00, 0x0, Fld2Msk32(CCH_SP1_FLD));// CCH_FLD_DET
            //CCIR initial for 422
            vRegWriteFldAlign(CCIR_00, 1, CCH_LSB_EN);//12bit
#if CCIR_decode_444
            vRegWriteFldAlign(CCIR_00, 0, CCIR656_VSRC);
#else
            vRegWriteFldAlign(CCIR_00, 1, CCIR656_VSRC);
#endif

            vRegWriteFldAlign(CCIR_00, 0, CCH_SRC8_SEL);//video16
            vRegWriteFldAlign(CCIR_00, 1, CCH_CH_SW);
            vRegWriteFldAlign(CCIR_00, 0x3, CCH_MODE);
            vRegWriteFldAlign(CCIR_00, 1, CCH_FLD_DET);
            vRegWriteFldAlign(CCIR_00, 0, CCH_FLD_DET_INI);
            vRegWriteFldAlign(CCIR_00, 1, CCH_HS_SEL);
            vRegWriteFldAlign(CCIR_00, 1, CCH_VS_SEL);
            // SP1_HS_SEL/SP1_VS_SEL ==> sync process h/v sync source select from ccir
            //vWriteVDOINMsk(VSRC00, SP1_HS_DVI|SP1_VS_DVI, SP1_HS_SEL|SP1_VS_SEL, BANK3);//need to modify???
        }

        _wDviHtotal = 0 ;
        _wHDMIPixelClk = 0;
        _bDviModeChged = 0 ;
        vDviInitial();
        _IsDviDetectDone = TRUE;

    }
    else if (fgIsOn == SV_OFF)
    {
        if (fgIsMainDVI())
        {
            _vDrvVideoSetMute(MUTE_MODULE_HDMI, SV_VP_MAIN, 0, FALSE);
        }
        else if (fgIsPipDVI())
        {
            _vDrvVideoSetMute(MUTE_MODULE_HDMI, SV_VP_PIP, 0, FALSE);
        }
        if (eNewMode == SRM_TV_MODE_TYPE_NORMAL)
        {
            if (!_wHDMIBypassFlag)
            {
                _bHDMIState = HDMI_STATE_INIT;//add by ciwu
            }
        }
#if DVI_DEBUG
        LOG(6, "vDviConnect Off\n");
#endif
        _bDviExtSrc = SV_DV_NONE;
        _IsDviDetectDone = TRUE;

        if (bchannel == SV_VP_MAIN)
        {
            vVDOINIrqOff(MSK_MAIN_DET);//need to modify??
            _rDVIStat.bIsMain = FALSE;
        }
        else
        {
            vVDOINIrqOff(MSK_PIP_DET);//need to modify??
            _rDVIStat.bIsPip = FALSE;
        }
     
    }
}
/**
 * @brief 	DVI call-back function for getting DVI input signal HTotal.
 * @param	None
 * @retval	HTotal of input signal.
 */
UINT16 wDviInputHTotal(void)
{
    UINT16 wHTotal = 0;
    if (bHDMI3DPacketVaild())
    {
        wHTotal = wHDMI3DGetHVParameter(3);
    }
    else
    {
        if (_bDviTiming == MODE_DE_MODE)
        {
            wHTotal =  _wDviHtotal ;
        }
        else if (_bDviTiming < MAX_TIMING_FORMAT)
        {
            wHTotal =  Get_VGAMODE_IHTOTAL(_bDviTiming);
        }
        if(1==bDviBypassAp3DRule())
        {
		#if CCIR_decode_444
		        if (bCCIRCENStatus() || (1 == RegReadFldAlign(CCIR_00, CCH_CLK_DIV)))
		#else
		        if (bDVICENStatus())
		#endif
		        {
		            wHTotal = wHTotal / 2 ;
		        }
        }
        else
        {
        	wHTotal=wHDMIGet2DmodeForce3DHVParameter(3,wHTotal);
        }
    }
    return wHTotal;
}

/**
 * @brief 	DVI call-back function for getting DVI input signal width.
 * @param	None
 * @retval	Height of input signal.
 */
UINT16 wDviInputVTotal(void)
{
    UINT16 wVTotal = 0;

    if (bHDMI3DPacketVaild())
    {
        wVTotal = wHDMI3DGetHVParameter(1);
    }
    else
    {
        if (_bDviTiming == MODE_DE_MODE)
        {
            wVTotal = _wDviVTotal ;
        }
        else if (_bDviTiming < MAX_TIMING_FORMAT)
        {
            wVTotal =  Get_VGAMODE_IVTOTAL(_bDviTiming);
        }
        if(1==bDviBypassAp3DRule())
        {
        }
        else
        {
        	wVTotal=wHDMIGet2DmodeForce3DHVParameter(1,wVTotal);
        }
    }
    return wVTotal;
}

/**
 * @brief 	DVI call-back function for getting DVI input signal width.
 * @param	None
 * @retval	Hactive of input signal.
 */
UINT16 wDviInputWidth(void)
{
#if ENABLE_DE_MODE
    if (_IsDviDetectDone &&  bEnDVIDE)
    {
        if (bHDMI3DPacketVaild())//input 3d
        {
            return wHDMI3DGetHVParameter(4);
        }
        else//input 2d
        {
         	if(1==bDviBypassAp3DRule())
         	{
	            if (fgIsVideoTiming(_bDviTiming))
	            {
	#if CCIR_decode_444  //480i 422 input must Y0 Y0 Y1 Y1 Y2 Y2 pixel repeat, CCIR decoder will drop pxiel automatically and Hactive must be 720
	                if (bHDMI422Input() && ((_bDviTiming == MODE_525I) || ( _bDviTiming == MODE_625I)))
	                {
	                    return (Get_VGAMODE_IPH_WID(_bDviTiming) >> 1 );
	                }
	                else		//480i 444 has two pixel repeat type, Y0 Y0 Y1 Y1 Y2 Y2 or Y1 Y2 Y3 Y4 Y5 , drop pixel will be decided by PE , Hactive must be 1440
	                {
	                    return (Get_VGAMODE_IPH_WID(_bDviTiming) >> (RegReadFldAlign(CCIR_00, CCH_CEN_SEL)));
	                }
	#else
	                return (Get_VGAMODE_IPH_WID(_bDviTiming) >> (RegReadFldAlign(DVI_00, DVI_CEN_SEL)));
	#endif
	            }
	            else
	            {
	                return _wDviWidth ;
	            }
         	}
         	else
         	{
         		return wHDMIGet2DmodeForce3DHVParameter(4,_wDviWidth);
         	}
        }
    }
    else
    {
        return 720;
    }
#endif
}

/**
 * @brief 	DVI call-back function for getting DVI input signal height.
 * @param	None
 * @retval	Vactive of input signal.
 */
UINT16 wDviInputHeight(void)
{
    if (_IsDviDetectDone &&  bEnDVIDE )
    {
        if (bHDMI3DPacketVaild())//input is 3d
        {
            return wHDMI3DGetHVParameter(2);
        }
        else//input is 2d
        {
        	if(1==bDviBypassAp3DRule())
         	{
	            if (fgIsVideoTiming(_bDviTiming))
	            {
	                return Get_VGAMODE_IPV_LEN(_bDviTiming);
	            }
	            else
	            {
	                return _wDviHeight;
	            }
        	}
        	else
        	{
        		return wHDMIGet2DmodeForce3DHVParameter(2,_wDviHeight);
        	}
        }
    }

    return (480);
}

/**
 * @brief Exported to display driver, video mainloop, video "ISR", middleware interface.
 * Call this function when signal is stable. DVI driver use another function to get refresh rate.
 * @param None
 * @return Refresh rate of input signal.
 */
UINT8 bDviRefreshRate(void)
{
    //UINT32 lvsync_freq;

#if 0
#if (ENABLE_DE_MODE && FROM_HDMIDEC)
    if ((bEnDVIDE == 1) && (_bDviTiming == MODE_DE_MODE))
    {
        lvsync_freq = (UINT32)(wDSVtotalMeasure() + wDVIDEVsyncWidth()) * wDSHtotalMeasure();
        if (lvsync_freq == 0)
        {
            return 1;
        }
        lvsync_freq = (CRYSTAL * 1000000 * 2) / lvsync_freq;	// *2 add counting resolution
        lvsync_freq = ((lvsync_freq + 1) >> 1);
        if (lvsync_freq == 73)
            lvsync_freq = 72; //For motorcycle CQ Problem !!!!

        return ((UINT8) lvsync_freq);
    }
#endif
#else
    if (bHDMI3DPacketVaild())//3d
    {
        return wHDMI3DGetHVParameter(5);
    }
    else
    {
        if ((bEnDVIDE == 1) && (_bDviTiming == MODE_DE_MODE))
        {
            return _bDviVclk ;
        }
        else
        {
            if ((_IsDviDetectDone) &&
                    (_bDviTiming != MODE_NOSIGNAL) &&
                    (_bDviTiming < MAX_TIMING_FORMAT))
            {
                return Get_VGAMODE_IVF(_bDviTiming);
            }
            else
            {
                return 0;
            }
        }
    }
#endif

}

/**
 * @brief 	DVI call-back function used by DI/Display/Scaler driver
 * @param	None
 * @retval	0 - progressive, 1 - interlace
 */
UINT8 bDviInterlace(void)
{
#if ENABLE_DE_MODE
    if (_bDviTiming == MODE_DE_MODE)
    {
        //return fgHDMIinterlaced();
        // for DE mode , only support progressive signal because interlace signal need MDDI fine tune
        return 0 ;
    }
#endif

    if ((_IsDviDetectDone) &&
            (_bDviTiming != MODE_NOSIGNAL) &&
            (_bDviTiming < MAX_TIMING_FORMAT))
    {
    	if(FP_interlace==bHDMI3DIPCheck()) 
    	{
    		 if(0==bDviBypassAp3DRule()) 
    		 {
    		 	 return 0 ;//force 3D to 2D, it must notify progress timing
    		 }
    		 else
    		 {
    			return 1;
    		 }
    	}
    	else
    	{
        return (Get_VGAMODE_INTERLACE(_bDviTiming));
    }
    }
    else
    {
        return (0);
    }
}

/**
 * @brief 	DVI call-back function for vVdoMainState, vVdoPipState polling update video status to UI.
 * @param	None
 * @retval	SV_VDO_UNKNOWN / SV_VDO_NOSIGNAL / SV_VDO_NOSUPPORT / SV_VDO_STABLE
 */
UINT8 bDviSigStatus(void)
{
#if ENABLE_DE_MODE
    if (_bDviTiming == MODE_DE_MODE)
    {
        return (UINT8)(SV_VDO_STABLE);
    }
#endif

    if (!_IsDviDetectDone)
    {
        return (UINT8)SV_VDO_UNKNOWN;
    }
    else
    {
        switch (_bDviTiming)
        {
            case MODE_NOSIGNAL:
            case MODE_WAIT:
                return (UINT8)(SV_VDO_NOSIGNAL);

            case MODE_NOSUPPORT:
                return (UINT8)(SV_VDO_NOSUPPORT);

            default:
                return (UINT8)(SV_VDO_STABLE);
        }
    }
}

/**
 * @brief 	DVI call-back function for video timing check
 * @param	None
 * @retval	1: video timing, 0: PC timing
 */
UINT8 bDviIsVideoTiming(void)
{
    return((fgIsVideoTiming(_bDviTiming)) ? 1 : 0);
}
UINT8 b3Dfldstatus1,b3Dfldstatus2,b3Dfldstatus3;
UINT8 b3Dfld0=0,b3Dfld1=0;
/**
 * @brief 	DVI ISR - Set flags for DVI related mainloop.
 *					DVI_OUTPUTV		==> vDviModeDetect() + vDviChkModeChg()
 *					HDMI_CHECK		==> vHDMIMainloop()
 *					_bSync1Vsync2	==> for auto color (only used in external ADC input).
 * @param	None
 * @retval	None
 */
void vDviISR(void)
{
b3Dfld0=0;
b3Dfld1=0;
#if 0
    if (fgIsVdoIntScpos())
    {
        vSetDviFlg(DVI_OUTPUTV);
        _bSync1Vsyn2 = TRUE; // for auto color
    }
#endif
 if (!_bDviModeChged  &&(FP_interlace==bHDMI3DIPCheck()))
 	{
 		b3Dfldstatus1=RegReadFldAlign(DVI_HDTV_03, STA_CCH_3D_ST);
 		b3Dfldstatus2=RegReadFldAlign(DVI_HDTV_03, STA_CCH_3D_ST);
 		b3Dfldstatus3=RegReadFldAlign(DVI_HDTV_03, STA_CCH_3D_ST);
 		//LOG(2, "b3Dfldstatus1=%d b3Dfldstatus2=%d b3Dfldstatus3=%d\n",b3Dfldstatus1,b3Dfldstatus2,b3Dfldstatus3);
 		if((b3Dfldstatus1==2) || (b3Dfldstatus1==3)||(b3Dfldstatus1==6)||(b3Dfldstatus1==7))
 		{
 			b3Dfld0++;
 		}
 		else
 		{
 			b3Dfld1++;
 		}
 		if((b3Dfldstatus2==2) || (b3Dfldstatus2==3)||(b3Dfldstatus2==6)||(b3Dfldstatus2==7))
 		{
 			b3Dfld0++;
 		}
 		else
 		{
 			b3Dfld1++;
 		}
 		if((b3Dfldstatus3==2) || (b3Dfldstatus3==3)||(b3Dfldstatus3==6)||(b3Dfldstatus3==7))
 		{
 			b3Dfld0++;
 		}
 		else
 		{
 			b3Dfld1++;
 		}
	}
   if(b3Dfld0 > b3Dfld1)
   	{
   		 vRegWriteFldAlign(LPF_PIP_00, 0x1, DVI_3D_DISP_VAL);
   		 //LOG(2, "DVI_3D_DISP_VAL=0 \n");
   	}
   else
   	{
   		vRegWriteFldAlign(LPF_PIP_00, 0x0, DVI_3D_DISP_VAL);
   		//LOG(2, "DVI_3D_DISP_VAL=1 \n");
   	}
}

extern UINT8 u1HdmiColorMode;
UINT8 bGetSupportHdmiModeVideoTiming(void)
{
    UINT8 _u8Timing = 0;
    UINT16 _u16Width = wHDMIResoWidth();
    UINT16 _u16Height = wHDMIResoHeight();
    UINT8 _u8Rate = bHDMIRefreshRate();

    if ((_u16Width == 1280) && (_u16Height == 720))
    {
        if (_u8Rate == 60)
        {
            _u8Timing = MODE_720p_60;
        }
        else if (_u8Rate == 50)
        {
            _u8Timing = MODE_720p_50;
        }
    }
    else if ((_u16Width == 1920) && (_u16Height == 1080))
    {
        if (_u8Rate == 60)
        {
            _u8Timing = MODE_1080p_60;
        }
        else if (_u8Rate == 50)
        {
            _u8Timing = MODE_1080p_50;
        }
        else if (_u8Rate == 30)
        {
            _u8Timing = MODE_1080p_30;
        }
        else if (_u8Rate == 25)
        {
            _u8Timing = MODE_1080p_25;
        }
        else if (_u8Rate == 24)
        {
            _u8Timing = MODE_1080p_24;
        }
    }
    else if (_u16Width == 720)
    {
        if ((_u8Rate == 60) && (_u16Height == 480))
        {
            _u8Timing = MODE_480P;
        }
        else if ((_u8Rate == 50) && (_u16Height == 576))
        {
            _u8Timing = MODE_576P;
        }
    }
    else
    {
        if (!bDviInterlace())
        {
            _u8Timing = _bDviTiming;
        }
    }

    return _u8Timing ;
}

void vDVIFixColor(UINT8 bPath)
{
    if (bGetSupportHdmiModeVideoTiming() != 0)
    {
        switch (u1HdmiColorMode)
        {
            case SV_HDMI_MODE_AUTO:
                break;

            case SV_HDMI_MODE_GRAPHIC:
                _bDviTiming = MODE_DE_MODE;
                break;

            case SV_HDMI_MODE_VIDEO:
                _bDviTiming = bGetSupportHdmiModeVideoTiming();
                break;
            default:
                break;
        }
    }
}

// End
////////////////////////////////////////////////////////////////////////////////
extern UINT8 _bBdThrsh;
/**
 * @brief 	When DVI signal is not stable, check signal activity and then get hlen/vlen for timing search.
 * @param	None
 * @retval	None
 */
void vDviModeDetect(void)
{
#if ENABLE_DE_MODE
    //UINT16 wHDMIWitdh, wHDMIHeight;
    //UINT8 i;
#endif
    UINT16 Vproch_tmp;

    // Source select will do mode detect and INT will do ,too
#ifndef CC_SUPPORT_HDMI_FBOOTING
    if (!(fgIsMainDVI() || fgIsPipDVI()) || fgIsDviFlgSet(DVI_DEBUG_MODE))
    {
        return;
    }
#endif

#if NEW_MODE_CHANGE_FLOW
#ifndef CC_SUPPORT_HDMI_FBOOTING
    if (fgIsExtSrcHDMI() && !((_bHDMIState == HDMI_STATE_AUTH) || (_bHDMIState == HDMI_STATE_PWOFF)))
    {
        return;
    }
#else
	if (!((_bHDMIState == HDMI_STATE_AUTH) || (_bHDMIState == HDMI_STATE_PWOFF)))
	{
		return;
	}
#endif
#endif

    switch (_bDviModeDetState)
    {
            // Timing Search State
        case DVI_SEARCH_STATE:

            vRegWrite4BMsk(VID_SET, Fld2Msk32(AV_MUTE_CLR), Fld2Msk32(AV_MUTE_CLR));
            /* check signal activity */

            if (!bDVIGetHAct())
            {
                _bDviTiming = MODE_NOSIGNAL;
#if DVI_DEBUG
                LOG(6, "md: no signal\n");
#endif
            }
            else
            {

                /* search timing */
                if (fgIsExtSrcCCIR())
                {
                    // CCIR search Video Timing only
                    _bDviTiming = bDviStdTimingSearch(1);
                }
                else
                {

                    _wDviHclk = wDviIHSClock();
                    _bDviVclk = bDviIVSClock();

                    _bDviTiming = bDviStdTimingSearch(1);

#if !ENABLE_DE_MODE
                    if (_bDviTiming == MODE_NOSUPPORT)
                    {
                        _bDviTiming = bDviStdTimingSearch(0);
                    }
#else
                    // ambiguous mode with 1280x720x60Hz - CVT1260H in QD882
                    if (_bDviTiming == MODE_720p_60)
                    {
						if (_wDviHtotal >=1660)
						{
							_bDviTiming = MODE_DE_MODE;
						}
                    }
                    else if (_bDviTiming == MODE_NOSUPPORT)
                    {
                        _bDviTiming = MODE_DE_MODE;
                    }
#endif
                }
            }

#if NotSupport_80Hz_About
            BOOL HDMI_Interlace;
            HDMI_Interlace = fgHDMIinterlaced();

            //Check HDMI Debug Message
            if ((HDMI_Interlace == 1) && (_bDviVclk > 80))
            {
                _bDviTiming = MODE_NOSUPPORT;
            }
#endif

#if NotSupport_80Hz_About
            // Check DVI Debug Message
            if (_bDviTiming != MODE_NOSUPPORT)
            {
                if (_bDviVclk > 80)
                {
                    _bDviTiming = MODE_NOSUPPORT;
                }
            }
#endif

            if (_bDviTiming == MODE_DE_MODE)
            {
#if 0
                if ((_bDviVclk < 49) && (_bDviVclk > 0))
                {
                    _bDviTiming = MODE_NOSUPPORT;
                }
#endif
                /*Sony suggest use VID code to check interlace when timing detect error*/
                if ((wDviInputWidth() == 1920) && (wDviInputHeight() == 540) && (bDviRefreshRate() == 60) && (u1RegRead1B(AVIRX1_3) == 5))
                {
                    _bDviTiming = MODE_1080i;
                }
                if ((wDviInputWidth() == 1920) && (wDviInputHeight() == 540) && (bDviRefreshRate() == 50) && (u1RegRead1B(AVIRX1_3) == 20))
                {
                    _bDviTiming = MODE_1080i_50;
                }
                if ((wDviInputWidth() == 1440) && (wDviInputHeight() == 240) && (bDviRefreshRate() == 60) && ((u1RegRead1B(AVIRX1_3) == 6) || (u1RegRead1B(AVIRX1_3) == 7)))
                {
                    _bDviTiming = MODE_525I;
                }
                if ((wDviInputWidth() == 1440) && (wDviInputHeight() == 288) && (bDviRefreshRate() == 50) && ((u1RegRead1B(AVIRX1_3) == 21) || (u1RegRead1B(AVIRX1_3) == 22)))
                {
                    _bDviTiming = MODE_625I;
                }
            }

            if (_bDviTiming == MODE_NOSUPPORT)
            {
                if (_bNotSupportCnt++ < 90)
                {
                    LOG(6, "md: MODE_NOSUPPORT %d found\n", _bNotSupportCnt);
                    break;
                }
            }

            //fix 576p ambiguous with 720x350 and 720x400 50Hz problem
            if (_bDviTiming == MODE_576P || _bDviTiming == MODE_480P)
            {
                _wDviHeight =  bDVIGetHeight();
                if ((_wDviHeight  <= (Get_VGAMODE_IPV_LEN(_bDviTiming) - 20)) || (_wDviHeight  >= (Get_VGAMODE_IPV_LEN(_bDviTiming) + 20)))
                {
                    _bDviTiming = MODE_DE_MODE;
                }
            }
            if ((wDviInputWidth() == 1280) && (wDviInputHeight() == 720) && (bDviRefreshRate() == 60))
            {
                _bDviTiming = MODE_720p_60;
            }


#ifdef CC_BYPASS_AVI_INFOFRAME
            //if(_bDVIAVIinfoBypass==0x0)//mark by ciwu for davis ask
            {
                if ((_bDviTiming == MODE_HDMI_640_480P) || (_bDviTiming == MODE_480P) || (_bDviTiming == MODE_480P_OVERSAMPLE) || (_bDviTiming == MODE_720p_60) ||
                (_bDviTiming == MODE_1080p_60))
                {
                    printf("u1RegRead1B(AVIRX0_0)=%x\n", u1RegRead1B(AVIRX0_0));
                    if (u1RegRead1B(AVIRX0_0) != 0x82)
                    {
                        _bDviTiming = MODE_DE_MODE;
                    }
                }
            }
#endif

            if (_bDviTiming >= MODE_MAX && _bDviTiming < MODE_DE_MODE ) // BP_1000, Power off, green line issue
            {
                _bDviTiming = MODE_NOSIGNAL;
            }

            _bDviModeDetState = DVI_MODE_CHG_DONE_STATE;

#if (INFORM_MDCHG && FROM_HDMIDEC)
            // clear the counter of HDMI ModeCHG after mode detect done
            //UNUSED(fgHDMIQueryModeChange());
#endif

            // Mode found among timing table
            if (((_bDviTiming > MODE_NOSIGNAL) && (_bDviTiming < MAX_TIMING_FORMAT)) || (_bDviTiming == MODE_DE_MODE))
            {
                //_wDviVTotal = wDSVtotalMeasure();

#if DVI_DEBUG
                LOG(3, "md: timing %d found\n", _bDviTiming);
#endif

                vDviPolarityUniform();

                if (_bDviTiming != MODE_DE_MODE)
                {
                    _wDviHclk = Get_VGAMODE_IHF(_bDviTiming);
                    _bDviVclk = Get_VGAMODE_IVF(_bDviTiming);
                }

                _bDviModeDetState = DVI_WAIT_AUTO_STATE;

            }
            else if (_bDviTiming != MODE_NOSIGNAL)
            {
                _bDviTiming = MODE_NOSUPPORT;
#if DVI_DEBUG
                LOG(6, "md: timing not support\n");
#endif
            }

            _bDviRetry = 0;
            _wDviWidth =  bDVIGetWidth() ;
            _wDviHeight =  bDVIGetHeight();

            break;

            // Wait DVI Auto Done state
        case DVI_WAIT_AUTO_STATE:

            if ((_wDviWidth != bDVIGetWidth()) || (_wDviHeight != bDVIGetHeight()))
            {
                if (_bDviRetry++ < 10)
                {
                    _wDviWidth =  bDVIGetWidth() ;
                    _wDviHeight =  bDVIGetHeight();
                    break;
                }
            }

#if 1 //TODO: support this feature in interlace video
            //if (!fgHDMIinterlaced())
            if (!fgHDMIinterlaced() && (!bHDMI3DTiming()))
            {
                Vproch_tmp = wDVIDEVsyncWidth() - 2;
                // for issue 1315, video freeze issue with BDP-S550.
                // check Vs start value.
                if (Vproch_tmp > 200)
                {
                    //Printf(">>> 720P wDVIDEVsyncWidth()-2: %d\n", wDVIDEVsyncWidth()-2);
                    break;
                }
#if CCIR_decode_444
                vCCIRSetVsStart(Vproch_tmp);
#else
                vDviSetVsStart(Vproch_tmp);
#endif
            }
#endif

            _bDviModeDetState = DVI_MODE_CHG_DONE_STATE;

            break;

            // Mode Chg Done State
        case DVI_MODE_CHG_DONE_STATE:
            vRegWrite4BMsk(VID_SET, 0x0, Fld2Msk32(AV_MUTE_CLR));

            //vHDMISetColorRalated();
#if NEW_MODE_CHANGE_FLOW
            if (_bDviTiming == MODE_NOSIGNAL)
            {
                _bDviChkState = DVI_NO_SIGNAL;
            }
            else
            {
                _bDviChkState = DVI_CHK_MODECHG;
            }
#endif
            _bQueryMode=wDviQueryAP3DSetting();
            _bCurQueryMode = _bQueryMode;
            // set input capture
            vDviSetInputCapture(_bDviTiming);
            #if CCIR_decode_444
                _wDviVstartDefault = wCCIRGetVsStart();
            #else
            _wDviVstartDefault = wDviGetVsStart();
            #endif
            vDVISetModeDone();
            //Printf("Mode Chg Done: %d\n", _bDviTiming);
            _IsDviDetectDone = TRUE;
            _bDviModeDetState = DVI_SEARCH_STATE;

            if (fgIsMainDVI())
            {
                vDVIFixColor(SV_VP_MAIN);
            }
            if (fgIsPipDVI())
            {
                vDVIFixColor(SV_VP_PIP);
            }

            break;
        default:
            break;
    }

}

void vHdmiInformModeChg(void)
{
    if ((_bDviTiming == MODE_NOSIGNAL) || (!_IsDviDetectDone))
    {
        return ;
    }
    vDVISetModeCHG();
    _bDviChkState = DVI_WAIT_STABLE ;
    _bDviMdChgCnt = 0;
    _wDviHtotal = 0 ;
    _wHDMIPixelClk = 0;

}

#if USE_HDCP_CHECK_STABLE
/**
 * @brief 	Judge whether the HDCP is stable through the current HDCP status and the value of HDCPStableCount.
 * @param	None
 * @retval	stable: 1   not stable: 0
 */
UINT8 bChkHDCPStable(void)
{
    _bCurHDCPstatus = u1RegRead1B(HDCP_STAT_2) & 0x30; //00: No HDCP; 01: Authenticating; 11: AUTH Done

#if DVI_DEBUG
    if (_bCurHDCPstatus != _bPreHDCPstatus)
    {
        LOG(6, "------ HDCPStatus: 0x%x, 0x%x, %d ------\n", _bPreHDCPstatus, _bCurHDCPstatus, _bHDCPStableCount);
    }
#endif

    if (_bCurHDCPstatus == 0x30)
    {
        if (_bPreHDCPstatus == 0x30)
        {
            _bHDCPStableCount++;
            if(_bHDCPStableCount==100)
            {
            	vRegWrite4B(INTR_STATE0, Fld2Msk32(INTR3_NEW_UNREC));
            }
            if (_bHDCPStableCount == 150)
            {
#if DVI_DEBUG
                LOG(6, "HDCP: type1 stable!..........\n");
#endif
                return 1;
            }
        }
        else
        {
            _bHDCPStableCount = 0;
        }
        _bPreHDCPstatus = _bCurHDCPstatus;
    }
    else if (_bCurHDCPstatus == 0x00)
    {
        if (_bPreHDCPstatus == 0x00)
        {
            _bHDCPStableCount++;
            if(_bHDCPStableCount==100)
            {
            	vRegWrite4B(INTR_STATE0, Fld2Msk32(INTR3_NEW_UNREC));
            }
            if (_bHDCPStableCount == 120)
            {
#if DVI_DEBUG
                LOG(6, "HDCP: type2 stable!..........\n");
#endif
                return 1;
            }
        }
        else
        {
            _bHDCPStableCount = 0;
        }
        _bPreHDCPstatus = _bCurHDCPstatus;
    }
    else if (_bCurHDCPstatus == 0x10)
    {
        if (_bPreHDCPstatus == 0x10)
        {
            _bHDCPStableCount++;
            if(_bHDCPStableCount==100)
            {
            	vRegWrite4B(INTR_STATE0, Fld2Msk32(INTR3_NEW_UNREC));
            }
            if (_bHDCPStableCount == 150)
            {
#if DVI_DEBUG
                LOG(6, "HDCP: type3 stable!..........\n");
#endif
                return 1;
            }
        }
        else
        {
            _bHDCPStableCount = 0;
        }
        _bPreHDCPstatus = _bCurHDCPstatus;
    }

    return 0;
}
#endif

/**
 * @brief 	Polling function for mode change every output vsync
 * @param	None
 * @retval	None
 */

extern UINT8 _bUnplugFlag;
extern void vTV3DGetForce2D(UINT8 bPath, UINT8 *pu1Force2D, UINT8 *pu1LR);
//UINT8 bTest = 0;
#if SUPPORT_SBS_CONVERT
extern BOOL _fgScaleTriggerHdmiMch;
#endif

#if (defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
extern BOOL _fgScaleTriggerHdmiMchFor3DPR;
#endif

void vDviChkModeChange(void)
{
    UINT16 wvtemp;
    UINT16 whtemp;
    UINT32 wWaitCnt;
#if Support_HW_Adaptive_EQ
    UINT8 bEQCH0_tmp, bEQCH1_tmp, bEQCH2_tmp;
#endif
    //UINT16 wTmp;
    HAL_TIME_T rCurTime;
#if USE_HDCP_CHECK_STABLE
    //UINT8 bHDCPstatus;
    //static UINT8 bHDCPStableCount=0;
    UINT8 fgIsHDCPStable;
#endif
    UINT8 bForceFlag = 0;
    UINT8 bLOrRFlag = 0;

    if (fgIsMainDVI())
    {
        vTV3DGetForce2D(0, &bForceFlag, &bLOrRFlag);//main
    }
    else if (fgIsPipDVI())
    {
        vTV3DGetForce2D(1, &bForceFlag, &bLOrRFlag);//sub

    }

#ifndef CC_SUPPORT_HDMI_FBOOTING
    if (!(fgIsMainDVI() || fgIsPipDVI()) || fgIsDviFlgSet(DVI_DEBUG_MODE))
    {
        return;
    }
#endif

    if ((_bHdmiCmd == 0x30) || (_bHdmiCmd == 0x40))
    {
        return;
    }

    // check unstable than time out for mode detect done
    if (_bDviModeChged)
    {
        //if((_bHDMIState == HDMI_STATE_PWON) || (_bHDMIState == HDMI_STATE_PWON2))
        //{
        //	return;
        //}

        if (bDVIGetHAct())
        {
            wWaitCnt = 20;
        }
        else
        {
            wWaitCnt = _wDVI_WAIT_NOSIGNAL_COUNT;
        }

        HAL_GetTime(&rCurTime);
        if ((rCurTime.u4Seconds - _rDviModeChgTime.u4Seconds) > wWaitCnt)
        {
            LOG(6, "mc:  Time out to force mode detect done \n");
            if (bDVIGetHAct())
            {
                _bDviTiming = MODE_NOSUPPORT;
            }
            else
            {
                _bDviTiming = MODE_NOSIGNAL;
            }
            _wDviHtotal = 0 ;
            _wHDMIPixelClk = 0;
            //_bDviTiming = MODE_NOSIGNAL;
            _bDviMdChgCnt = 0 ;
            _bDviChkState = DVI_NO_SIGNAL ;
            vDVISetModeDone();
        }
    }
    if (fgIsExtSrcHDMI() && !((_bHDMIState == HDMI_STATE_AUTH) || (_bHDMIState == HDMI_STATE_PWOFF)))
    {
        return;
    }

    if ((_bDviChkState != DVI_NO_SIGNAL) || _bUnplugFlag)
    {
        if (_bUnplugFlag)
        {
            _bUnplugFlag = 0;

#if DVI_DEBUG
            LOG(6, "mc: from activity detected to no signal when _bUnplugFlag\n");
#endif
            vDviInitial();
            //vDVISetModeCHG();
            _wDviHtotal = 0 ;
            _wHDMIPixelClk = 0;
            _bDviTiming = MODE_NOSIGNAL;
            _bDviMdChgCnt = 0 ;
            _bDviChkState = DVI_NO_SIGNAL ;
            vDVISetModeDone();
        }
        else
        {
            if (!bDVIGetHAct())
            {
#ifdef CC_Support_Sil9285
                if (++_bDviNoSigCnt >= 300)
#else
                if (++_bDviNoSigCnt >= 200)
#endif
                {
#if DVI_DEBUG
                    LOG(6, "mc: from activity detected to no signal\n");
#endif
                    vDviInitial();
                    //vDVISetModeCHG();
                    vDVISetModeDone();
                    _wDviHtotal = 0 ;
                    _wHDMIPixelClk = 0;
                    _bDviTiming = MODE_NOSIGNAL;
                    _bDviMdChgCnt = 0 ;
                    _bDviChkState = DVI_NO_SIGNAL ;
                }
            }
            else
            {
                _bDviNoSigCnt =  0 ;
            }
        }
    }

    whtemp = wDVIGetHtotal();
    wvtemp = wDVIGetVtotal();
    LOG(10, "mc: whtemp: %d\n", whtemp);
    LOG(10, "mc: wvtemp: %d\n", wvtemp);

    switch (_bDviChkState)
    {
        case DVI_NO_SIGNAL :
            if (bDVIGetHAct())
            {
#if DVI_DEBUG
                LOG(6, "mc: from no signal to wait stable\n");
#endif
                _bDVIEQFlag = 1;
                _bDviChkState = DVI_WAIT_STABLE ;
                _bDviMdChgCnt = 0;
                return ;
            }
            else
            {
                _bPWOFFCnt++;
#ifdef CC_Support_Sil9285
                if (_bDviModeChged && _bPWOFFCnt > 60)
#else
                if (_bDviModeChged && _bPWOFFCnt > 3)
#endif
                {
                    LOG(6, "mc: DVI_NO_SIGNAL to force mode done \n");
                    _bPWOFFCnt = 0;
                    _bDviTiming = MODE_NOSIGNAL;
                    vDVISetModeDone();
                }
            }

            break;
        case DVI_CHK_MODECHG:
#if 0
            if ((_bForce2DFlag != bForceFlag) || (_bUseLorRFlag != bLOrRFlag))
            {
                wDVIDecoder3DForce2D(bForceFlag, bLOrRFlag);//trigger mode change.
            }
#else
		    			
    	    if((_bQueryMode!=_bCurQueryMode)||(bLOrRFlag!=_bUseLorRFlag))
	    {
                _bQueryMode=_bCurQueryMode;
                _bUseLorRFlag=bLOrRFlag;
	    	    _bDviChkState = DVI_WAIT_STABLE  ;
		        vDviInitial();
	    }
	
#endif
            if (RegReadFldAlign(INTR_STATE1, INTR5_HRESCHG) && _bHResChgIntDetectFlg)
            {

                //if ((fgIsMainDVI() && (_u4MainMuteMask != 0)) || (fgIsPipDVI() && (_u4PipMuteMask != 0)))
                if ((fgIsMainDVI() && (_u4DrvVideoGetMute(SV_VP_MAIN) != 0)) || (fgIsPipDVI() && (_u4DrvVideoGetMute(SV_VP_PIP) != 0)))
                {
                    bHDMIPHYReset(HDMI_RST_DEEPCOLOR);
                    vRegWriteFldAlign(MUTE_SET, 1, TDFIFO_SYNC_EN);
                    vRegWriteFldAlign(MUTE_SET, 0, TDFIFO_SYNC_EN);
                    vRegWrite4B(INTR_STATE1, Fld2Msk32(INTR5_HRESCHG));
                    _bHResChgIntDetectFlg = 0;

                }
            }
            if (bHDMIInputType() != _bHDMIColorSpace)
            {
                if (fgIsMainDVI())
                {
                    vDVIFixColor(SV_VP_MAIN);
                }
                if (fgIsPipDVI())
                {
                    vDVIFixColor(SV_VP_PIP);
                }
                //_bHDMIColorSpace=bHDMIInputType();
                LOG(6, "mc: color space change and trigger mode change bHDMIInputType() =%d \n", bHDMIInputType() );
                _bDVIEQFlag = 0;
                _bDviChkState = DVI_WAIT_STABLE  ;
                vDviInitial();
            }

	       #ifdef CC_AUD_HDMI_SPDIF_CFG
		 #if 0
		if((aud_ch_info.eACPType!=(HDMI_ACPTYPE)bHDMIACPType()) ||(aud_ch_info.u1Category!=bHDMIChannelStCategoryCode()) ||(aud_ch_info.u1CopyRight!=bHDMIChannelStCopyright())
		||(aud_ch_info.eDataType!=(HDMI_DATATYPE)bHDMI_IsPCM())||(aud_ch_info.eSampeFreq!=(HDMI_AUD_FS)bHDMIGetAudSampleFreq()))
		{
		#endif
		if((aud_ch_info.eACPType!=(HDMI_ACPTYPE)bHDMIACPType()) ||(aud_ch_info.eDataType!=(HDMI_DATATYPE)bHDMI_IsPCM())
		||(aud_ch_info.eSampeFreq!=(HDMI_AUD_FS)bHDMIGetAudSampleFreq()))
		{
			if(_bInfoAudioInfoStableCnt++>60)
			{
				_bInfoAudioInfoStableCnt=0;
			aud_ch_info.eACPType=(HDMI_ACPTYPE)bHDMIACPType();
			aud_ch_info.u1Category=bHDMIChannelStCategoryCode();
			aud_ch_info.u1CopyRight=bHDMIChannelStCopyright();
			aud_ch_info.eDataType=(HDMI_DATATYPE)bHDMI_IsPCM();
			aud_ch_info.eSampeFreq=(HDMI_AUD_FS)bHDMIGetAudSampleFreq();
			AUD_CfgSpdifByHDMI(&aud_ch_info);
			}
			else
			{
				_bInfoAudioInfoStableCnt=0;
			}
		}
	    	#endif

            if ((_bHDMIScanInfo != bHDMIScanInfo()) || (_bHDMIAspectRatio != bHDMIAspectRatio()) || (_bHDMIAFD != bHDMIAFD())
                    || (_bHDMI422Input != bHDMI422Input()) || (_bHDMIITCFlag != bHDMIITCFlag()) || (_bHDMIITCContent != bHDMIITCContent()) || (_bHDMIHDCPStatus != bHDMIHDCPStatusGet()))
            {
                if (_bInfoChgCnt++ > 3)
                {
                    _bInfoChgCnt = 0;
                    _bHDMIScanInfo = bHDMIScanInfo();
                    _bHDMIAspectRatio = bHDMIAspectRatio();
                    _bHDMIAFD = bHDMIAFD();
                    _bHDMI422Input = bHDMI422Input();
                    _bHDMIHDCPStatus = bHDMIHDCPStatusGet();
                    _bHDMIITCFlag = bHDMIITCFlag();
                    _bHDMIITCContent = bHDMIITCContent();
                    if (_rDVIStat.bIsMain)
                    {
                        if (_bDviTiming == MODE_NOSUPPORT)
                        {
                            vApiNotifyResolutionChg(SV_VP_MAIN, SV_VDO_NOSUPPORT);
                        }
                        else
                        {
                            vApiNotifyResolutionChg(SV_VP_MAIN, SV_VDO_STABLE);
                        }
                    }
                    else
                    {
                        if (_bDviTiming == MODE_NOSUPPORT)
                        {
                            vApiNotifyResolutionChg(SV_VP_PIP, SV_VDO_NOSUPPORT);
                        }
                        else
                        {
                            vApiNotifyResolutionChg(SV_VP_PIP, SV_VDO_STABLE);
                        }
                    }
                }
            }
            else
            {
                _bInfoChgCnt = 0;
            }

            if (((whtemp >= (_wDviHtotal - 5)) && (whtemp <= (_wDviHtotal + 5))) &&
                    ((wvtemp >= (_wDviVTotal - 2)) && (wvtemp <= (_wDviVTotal + 2))))
            {
                _bDviMdChgCnt = 0;
            }
            else
            {
                _bDviMdChgCnt++ ;
                LOG(3, "mc:H V total change and trigger mode change....................\n");
            }

            if (((bDVIGetWidth() >= (_wDviWidth - 3)) && (bDVIGetWidth() <= (_wDviWidth + 3))) &&
                    ((bDVIGetHeight() >= (_wDviHeight - 3)) && (bDVIGetHeight() <= (_wDviHeight + 3))))
            {
                _bDviDeChgCnt = 0;
            }
            else
            {
                _bDviDeChgCnt++ ;
                LOG(3, "mc:H V active width change and trigger mode change....................\n");
            }
            if ((dwDviPIXClock() >= (_wHDMIPixelClk - (_wHDMIPixelClk / 10))) && (dwDviPIXClock() <= (_wHDMIPixelClk + (_wHDMIPixelClk / 10))))
            {
                _bDviPixClkChgCnt = 0;
            }
            else
            {
                _bDviPixClkChgCnt++ ;
                LOG(3, "mc:Pixel clock  change and trigger mode change....................\n");
            }

#if CCIR_decode_444
            //[DTV00082304], change color space between ycbcr422 and ycbcr444, need trigger mode change.
            if (((bHDMI422Input() == 1) && (1 == RegReadFldAlign(SYS_CTRL, IDCK)) && (0 == RegReadFldAlign(CCIR_00, CCH_CLK_DIV)))
                    || ((bHDMI422Input() == 0) && (1 == RegReadFldAlign(SYS_CTRL, IDCK)) && (1 == RegReadFldAlign(CCIR_00, CCH_CLK_DIV))))
            {
                _bCCIRNeedModeChgCnt ++;
            }
            else
            {
                _bCCIRNeedModeChgCnt = 0;
            }
#endif

            //if ((_bDviMdChgCnt > 1)  || (_bDviDeChgCnt > 1))
            #if CCIR_decode_444
               //[DTV00082304], change color space between ycbcr422 and ycbcr444, need trigger mode change.
               if (((_bDviMdChgCnt > _bDviMdChgCntTHR)  || (_bDviDeChgCnt > _bDviDeChgCntTHR) || (_bDviPixClkChgCnt > _bDviPixClkChgCntTHR) || (_bCCIRNeedModeChgCnt > _bCCIRNeedModeChgCntTHR)) || (_bHdmiMD != _bHdmiMode)
                   #if SUPPORT_SBS_CONVERT
                   || _fgScaleTriggerHdmiMch
                   #endif
                   #if (defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))           
                   || _fgScaleTriggerHdmiMchFor3DPR
                   #endif
                   )
            #else
               if (((_bDviMdChgCnt > _bDviMdChgCntTHR)  || (_bDviDeChgCnt > _bDviDeChgCntTHR) || (_bDviPixClkChgCnt > _bDviPixClkChgCntTHR)) || (_bHdmiMD != _bHdmiMode)
                   #if SUPPORT_SBS_CONVERT
                   || _fgScaleTriggerHdmiMch
                   #endif
                   #if (defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))           
                   || _fgScaleTriggerHdmiMchFor3DPR
                   #endif
                   )
            #endif
            {
                #if SUPPORT_SBS_CONVERT
                _fgScaleTriggerHdmiMch = SV_FALSE;
                #endif
                
                #if (defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
                _fgScaleTriggerHdmiMchFor3DPR = SV_FALSE;
                #endif
               
                bHDMIPHYReset(HDMI_RST_ALL);
                LOG(3, "HDMI Analog PHY Reset @ CHK_MODECHG........\n");
                if (fgIsMainDVI())
                {
                    _vDrvVideoSetMute(MUTE_MODULE_HDMI, SV_VP_MAIN, FOREVER_MUTE, FALSE);
                }
                else if (fgIsPipDVI())
                {
                    _vDrvVideoSetMute(MUTE_MODULE_HDMI, SV_VP_PIP, FOREVER_MUTE, FALSE);
                }

                /*Because Master 1025D's resolution  change is HDMI->DVI->HDMI*/
                if (_bHdmiMD != _bHdmiMode)
                {
                    _bHdmiMD = _bHdmiMode;
                }

                LOG(6, "mc: check mode change to wait stable %d \n", _bDviMdChgCnt);
                _bDviChkState = DVI_WAIT_STABLE  ;
                //vRegWriteFldAlign(TMDS_CTRL0, 0, HDMI_RTCK_AUTO);

#if HDMI_RTCK_AUTO_Release_FLOW
                fgXlkStable = 0;
                fgXlkStableCNT = 0;
                _bDviMdChgXLKStableCnt = 0;
#endif
                _bDVIEQFlag = 0;
                _bDviMdChgCnt = 0 ;
                if (fgIsVgaTiming(_bDviTiming ) || fgIsVideoTiming(_bDviTiming))
                {
                    //vDVISetModeCHG();
                    vDviInitial();
                }

            }
            break;
        case DVI_WAIT_STABLE:

            // if CLK change
            if (RegReadFldAlign(INTR_STATE0, INTR2_CLK_CHG))
            {
                bHDMIPHYReset(HDMI_RST_DEEPCOLOR);
                //vRegWriteFldAlign(TMDS_CTRL0, 1, HDMI_RTCK_AUTO);
                vRegWriteFldAlign(MUTE_SET, 1, TDFIFO_SYNC_EN);
                vUtDelay2us(1);
                //vRegWriteFldAlign(TMDS_CTRL0, 0, HDMI_RTCK_AUTO);
                vRegWriteFldAlign(MUTE_SET, 0, TDFIFO_SYNC_EN);
                vUtDelay2us(50);
                vRegWrite4B(INTR_STATE0, Fld2Msk32(INTR2_CLK_CHG));
                LOG(3, "\n>>>> DVI_WAIT_STABLE #1, HDMI_RTCK_AUTO Toggle\n");
            }

            else if (_bXpcStable == 0)
            {
                if (_bIsXpcStable())
                {
                    _bXpcStable = 1;
                    bHDMIPHYReset(HDMI_RST_DEEPCOLOR);
                    //vRegWriteFldAlign(TMDS_CTRL0, 1, HDMI_RTCK_AUTO);
                    vRegWriteFldAlign(MUTE_SET, 1, TDFIFO_SYNC_EN);
                    vUtDelay2us(1);
                    //vRegWriteFldAlign(TMDS_CTRL0, 0, HDMI_RTCK_AUTO);
                    vRegWriteFldAlign(MUTE_SET, 0, TDFIFO_SYNC_EN);

                    LOG(3, "\n>>>> DVI_WAIT_STABLE #5, HDMI_RTCK_AUTO Toggle\n");
                }
            }


            if (((whtemp >= (_wDviTmpHtotal - _wDviTmpTHR)) && (whtemp <= (_wDviTmpHtotal + _wDviTmpTHR))) &&
                    ((wvtemp >= (_wDviTmpVtotal - _wDviTmpTHR)) && (wvtemp <= (_wDviTmpVtotal + _wDviTmpTHR))))
            {
#if Support_HW_Adaptive_EQ
                if ((RegReadFldAlign(TMDS_CTRL_STATUS1, HDMI_CH0_STATUS)&0x100) == 0x100)
                {
                    bEQCH0_tmp = RegReadFldAlign(TMDS_CTRL_STATUS1, HDMI_CH0_EQERR);
                    bEQCH1_tmp = RegReadFldAlign(TMDS_CTRL_STATUS2, HDMI_CH1_EQERR);
                    bEQCH2_tmp = RegReadFldAlign(TMDS_CTRL_STATUS3, HDMI_CH2_EQERR);
                    LOG(6, "==== EQ_temp: 0x%x, 0x%x, 0x%x ====\n", bEQCH0_tmp, bEQCH1_tmp, bEQCH2_tmp);
                    if ((Diff(bEQCH0_tmp, _bEQCH0) >= 3) || (Diff(bEQCH1_tmp, _bEQCH1) >= 3) || (Diff(bEQCH2_tmp, _bEQCH2) >= 3))
                    {
                        _bEQCH0 = bEQCH0_tmp;_bEQCH1 = bEQCH1_tmp;_bEQCH2 = bEQCH2_tmp;
                        bHDMIPHYReset(HDMI_RST_EQ);
                        LOG(3, "mc: Reset HDMI PHY EQ again #1.....................\n");
                    }
                    else if ((Diff(bEQCH0_tmp, bEQCH1_tmp) >= 3) || (Diff(bEQCH0_tmp, bEQCH2_tmp) >= 3) || (Diff(bEQCH1_tmp, bEQCH2_tmp) >= 3))
                    {
                        _bEQCH0 = bEQCH0_tmp;_bEQCH1 = bEQCH1_tmp;_bEQCH2 = bEQCH2_tmp;
                        bHDMIPHYReset(HDMI_RST_EQ);
                        LOG(3, "mc: Reset HDMI PHY EQ again #2.....................\n");
                    }
                }
#endif
#if USE_HDCP_CHECK_STABLE
#if 0
                bHDCPstatus = u1RegRead1B(HDCP_STAT_2) & 0x30; //00: No HDCP; 01: Authenticating; 11: AUTH Done

                if (bHDCPstatus == 0x30)
                {
                    bHDCPStableCount++;
                }
                else
                {
                    bHDCPStableCount = 0;
                }
#endif
                fgIsHDCPStable = bChkHDCPStable();
#endif
                if (_bHdmiMD != _bHdmiMode)
                {
                    _bHdmiMD = _bHdmiMode;
                }

                if (_bNodifyConnect)
                {

                    UNUSED(DRVCUST_OptQuery(eDVIWaitStableCount, &_wDVI_WAIT_STABLE_COUNT));
                    //_wDVI_WAIT_STABLE_COUNT+=100;

                }
                else
                {
                    //_wDVI_WAIT_STABLE_COUNT= 120;
                    UNUSED(DRVCUST_OptQuery(eDVIWaitStableCount, &_wDVI_WAIT_STABLE_COUNT));

                }

                if (_wDVI_WAIT_STABLE_COUNTTHR != 0)
                {
                    _wDVI_WAIT_STABLE_COUNT = _wDVI_WAIT_STABLE_COUNTTHR;//am612
                }

#ifdef CC_SUPPORT_HDMI_FBOOTING
			if (((_u1HdmiSrcIn == 1) && (bDviSigStatus() == SV_VDO_STABLE)) || _u1HdcpGood > 65)
			{
			    _wDVI_WAIT_STABLE_COUNT = 1;
			    fgIsHDCPStable = 1;
			}
#endif			


#if USE_HDCP_CHECK_STABLE
                //if((_bDviMdChgCnt++ > _wDVI_WAIT_STABLE_COUNT)||(bHDCPStableCount>=30) )
                if (fgIsHDCPStable == 1)
#else
	 	if(_bDviMdChgCnt==(_wDVI_WAIT_STABLE_COUNT-20))
            	{
            		vRegWrite4B(INTR_STATE0, Fld2Msk32(INTR3_NEW_UNREC));
            	}
                if (_bDviMdChgCnt++ > _wDVI_WAIT_STABLE_COUNT)
#endif
                {
#if 0
                    if (_bDviMdChgCnt++ > (_wDVI_WAIT_STABLE_COUNT + 60))
                    {

#if (RTCK_AUTO_CTRL_V1 == 1)
// do nothing
#elif HDMI_RTCK_AUTO_Release_FLOW
                        if (RegReadFldAlign(TMDS_CTRL0, HDMI_RTCK_AUTO) == 1)
                        {
                            vRegWriteFldAlign(TMDS_CTRL0, 0, HDMI_RTCK_AUTO);
                        }
                        else
                        {
                            vRegWriteFldAlign(TMDS_CTRL0, 1, HDMI_RTCK_AUTO);
                        }
                        vRegWriteFldAlign(TMDS_CTRL0, 0, HDMI_RTCK_AUTO);
#endif

                    }
#endif
#if USE_HDCP_CHECK_STABLE
                    LOG(3, "\n=====WT=======Check V Stable =%d  _bHDCPStableCount=%d \n", _bDviMdChgCnt, _bHDCPStableCount);
#else
                    LOG(3, "\n=====WT=======Check V Stable =%d\n", _bDviMdChgCnt);
#endif
                    if (RegReadFldAlign(VID_VRES, VID_VRES_STB))
                        //if(RegReadFldAlign(VID_VRES,VID_VRES_STB)||(RegReadFldAlign(VID_MODE,ENDITHER)==1))
                    {

                        if (_bHdmiCmd != 0x20)
                        {
                            if (RegReadFldAlign(VID_VRES, VID_VRES_MUTE))
                            {
                                vRegWriteFldAlign(VID_VRES, 0x1, VRES_MUTE_CLR);
                                vRegWriteFldAlign(VID_VRES, 0x0, VRES_MUTE_CLR);
                            }
                        }
                        LOG(6, "mc:wait stable to timing search \n");
                        //vRegWrite4B(INTR_STATE0, Fld2Msk32(INTR3_NEW_UNREC));
                        vDviInitial();
                        _bForce2DFlag = bForceFlag;
                        _bUseLorRFlag = bLOrRFlag;
                        _IsDviDetectDone = FALSE;
                        _wDviHtotal = wDVIGetHtotal();
                        _wDviVTotal = wDVIGetVtotal();
                        _wHDMIPixelClk = dwDviPIXClock();

                        // [DTV00214389]
                        _bHDMIScanInfo = bHDMIScanInfo();
                        _bHDMIAspectRatio = bHDMIAspectRatio();
                        _bHDMIAFD = bHDMIAFD();
                        _bHDMI422Input = bHDMI422Input();
                        _bHDMIHDCPStatus = bHDMIHDCPStatusGet();
                        _bHDMIITCFlag = bHDMIITCFlag();
                        _bHDMIITCContent = bHDMIITCContent();
                        ///////////////////////////////////////////

                        //vRegWriteFldAlign(TMDS_CTRL0, 0, HDMI_RTCK_AUTO);
                    }
                }
#if DVI_DEBUG
                LOG(3, "_bDviMdChgCnt =%d  H/V Clk Chg, V:%d H:%d \n", _bDviMdChgCnt, wvtemp, whtemp);
                LOG(3, "pre H/V Clk Chg, V:%d H:%d\n", _wDviVTotal, _wDviHtotal);
#endif
            }
            else
            {
                LOG(3, "reset _bDviMdChgCnt =0\n");
                //if(1 == bTest)
                //{
                //LOG(3, "reset C_DATA_SYNC_AUTO\n");
      		  bHDMIPHYReset(HDMI_RST_RTCK);
                bHDMIPHYReset(HDMI_RST_DEEPCOLOR);
                //}

                _bDviMdChgCnt = 0;
#if USE_HDCP_CHECK_STABLE
                _bHDCPStableCount = 0;
#endif
#if HDMI_RTCK_AUTO_Release_FLOW
                _bDviMdChgXLKStableCnt = 0;
#endif
            }
            _wDviTmpHtotal = whtemp;
            _wDviTmpVtotal = wvtemp;

            break;
        default:
            break;
    }
}


extern UINT16 wDviGetPorch(UINT8 bPath, UINT8 bPorchType)
{
    UINT16  wHRef, wVref , wHproch, wAVStart, wVsStart;

    //wHRef=  (_wDviHtotal - _wDviWidth);
    wHproch =  (_wDviHtotal - wDviGetTableHactive()) ;
    wVref = _wDviVstartDefault ;
#if CCIR_decode_444
    wAVStart = wCCIRGetAvStart();
    wVsStart = wCCIRGetVsStart();
#else
    wAVStart = wDviGetAvStart();
    wVsStart = wDviGetVsStart();
#endif
    switch (bPorchType)
    {
        case 	SV_HPORCH_CURRENT:

#if ENABLE_DE_MODE
            //if DE mode H need to add H porch
            // if porch is neative than = _wDviHtotal - val
            // min value = _wDviHtotal - bRef
            if (wAVStart  >=  (_wDviHtotal - wHproch))
            {
                // avstart - _wDviHtotal = -
                wHRef =  ( wHproch  + wAVStart - _wDviHtotal) ;
                //wHRef = (wHRef  + wDviGetAvStart()) ;
            }
            else
            {
                wHRef = (wHproch  + wAVStart) ;
                //wHRef =  ( wHRef  + wDviGetAvStart() - _wDviHtotal) ;
            }
            return wHRef ;
#else
            return wAVStart ;
#endif
        case 	SV_HPORCH_DEFAULT:
            return wHproch;
        case	SV_VPORCH_CURRENT:
            if (_bBoundary_flag == TRUE)
            {
                wVref = wVsStart - 1 ;
            }
            else
            {
                wVref = wVsStart ;
            }
            //wVref = abs(wDviGetVsStart() - wVref) ;
            return  wVref;
        case	SV_VPORCH_DEFAULT:
            return  wVref;
        case SV_HPORCH_MAX:
            return  2*wHproch;
            //return  2*(wDrvVideoGetHTotal(bPath)-wDrvVideoInputWidth(bPath));
        default: /*MinMax*/
            return wDrvVideoPorchStd(bPath, bPorchType);
    }
}


void vDviSetPorch(UINT8 bPath, UINT8 bPorchType, UINT16 wValue)
{
    UINT16 wHRef, wVref;
    UINT32 u4PixelRepeat;
    UINT16 wAVStartValue;
    UINT16 wVproch_tmp;
    
    //E_TDTV_DECODER_INPUT inputtype;
    E_TDTV_DECODER_OUTPUT outputtype;
	//inputtype=(E_TDTV_DECODER_INPUT)(_bQueryMode&0xffff);
    outputtype=(E_TDTV_DECODER_OUTPUT)((_bQueryMode&0xffff0000)>>16);
    //UINT16 Vlength1,Vlength2;
    //UINT16 VlengthCnt;

    //wHRef=  (_wDviHtotal - _wDviWidth); // H= 0 ;
    wHRef =  (_wDviHtotal - wDviGetTableHactive()); // H= 0 ;
    wVref = _wDviVstartDefault ;
    UNUSED(wVref);

    if(outputtype==E_TDTV_DECODER_OUTPUT_NATIVE)
    {
    //to get if pixel repetition is on or off 1:means on
    u4PixelRepeat = RegReadFldAlign(DVI_00, DVI_CEN_SEL);
    /*-----------------------set HPorch--------------------------*/
    if (bPorchType == SV_HPORCH_CURRENT)
    {
        if (wValue <= wHRef)
        {
            if (_bBoundary_flag == TRUE)
            {
                _bBoundary_flag = FALSE;
                wVproch_tmp = wDviGetPorch(0, SV_VPORCH_CURRENT);
                vDviSetVsStart(wVproch_tmp - 1);
                vCCIRSetVsStart(wVproch_tmp - 1);
            }
            if (u4PixelRepeat == 1) //if pixel repetition is on, HPorch have to add 2.
            {
                wAVStartValue = _wDviHtotal + (wValue - wHRef) * 2;
            }
            else
            {
                wAVStartValue = _wDviHtotal + wValue - wHRef;
            }

            vCCIRSetAvStart(_wDviHtotal + wValue - wHRef);
        }
        else
        {
            if (_bBoundary_flag == FALSE)
            {
                wVproch_tmp = wDviGetPorch(0, SV_VPORCH_CURRENT);
                vDviSetVsStart(wVproch_tmp + 1);
                vCCIRSetVsStart(wVproch_tmp + 1);
                _bBoundary_flag = TRUE;
            }
            if (u4PixelRepeat == 1)
            {
                wAVStartValue = (wValue - wHRef) * 2;
            }
            else
            {
                wAVStartValue = wValue - wHRef ;
            }
            vCCIRSetAvStart(wValue - wHRef);
        }

        vDviSetAvStart(wAVStartValue);

    }
    /*-----------------------set VPorch--------------------------*/
    else
    {
        if (_bBoundary_flag == TRUE)
        {
            vDviSetVsStart(wValue + 1);
            vCCIRSetVsStart(wValue + 1);
        }
        else
        {
            vDviSetVsStart(wValue);
            vCCIRSetVsStart(wValue);
        }
    }
}
}

void vDVIDebugMsg(void)
{
    UINT16 tmp;
    HAL_TIME_T rTimerStartPoint, rTimerCurrPoint, rTimerDelta;
    BOOL fgDisp_R = 0;
    HDMI_3D_INFOFRAME * pInfo;

    pInfo = API_HDMI_3D_Get_InfoFrame();
    HAL_GetTime(&rTimerStartPoint);

    if (bHDMI3DPacketVaild())
    {
        do
        {
            HAL_GetTime(&rTimerCurrPoint);

            if (fgDisp_R != RegReadFldAlign(DVI_HDTV_02, MAIN_DISP_R))
            {
                fgDisp_R = !fgDisp_R;
                Printf("%d.%6d: fgDisp_R = %d\n", rTimerCurrPoint.u4Seconds, rTimerCurrPoint.u4Micros, fgDisp_R);
            }

            HAL_GetDeltaTime(&rTimerDelta, &rTimerStartPoint, &rTimerCurrPoint);
        }
        while ((rTimerDelta.u4Seconds * 1000000 + rTimerDelta.u4Micros) < 1000000);

        if (pInfo ->HDMI_3D_Structure == HDMI_3D_Structure_FramePacking)
        {
            Printf("HDMI_3D_Structure_FramePacking\n");
        }
        if (pInfo ->HDMI_3D_Structure == HDMI_3D_Structure_LineAlternative)
        {
            Printf("HDMI_3D_Structure_LineAlternative\n");
        }
        if (pInfo ->HDMI_3D_Structure == HDMI_3D_Structure_SideBySideFull)
        {
            Printf("HDMI_3D_Structure_SideBySideFull\n");
        }
        if (pInfo ->HDMI_3D_Structure == HDMI_3D_Structure_LDepth)
        {
            Printf("HDMI_3D_Structure_LDepth\n");
        }
        if (pInfo ->HDMI_3D_Structure == HDMI_3D_Structure_LDepthGraph)
        {
            Printf("HDMI_3D_Structure_LDepthGraph\n");
        }
        if (pInfo ->HDMI_3D_Structure == HDMI_3D_Structure_TopBottom)
        {
            Printf("HDMI_3D_Structure_TopBottom\n");
        }
        if (pInfo ->HDMI_3D_Structure == HDMI_3D_Structure_SideBySideHalf)
        {
            Printf("HDMI_3D_Structure_SideBySideHalf\n");
        }
        Printf("3D signal\n");
    }
    else
    {
        Printf("2D signal\n");
    }
    if (RegReadFldAlign(STA_SYNC1_03, CSYNC_ACT))
    {
        LOG(6, "CSYNC_ACT: 1\n");
    }
    else
    {
        LOG(6, "CSYNC_ACT: 0\n");
    }

    if (RegReadFldAlign(STA_SYNC1_03, VSYNC_ACT))
    {
        LOG(6, "VSYNC_ACT: 1\n");
    }
    else
    {
        LOG(6, "VSYNC_ACT: 0\n");
    }

    if (RegReadFldAlign(STA_SYNC1_03, HSYNC_ACT))
    {
        LOG(6, "HSYNC_ACT: 1\n");
    }
    else
    {
        LOG(6, "HSYNC_ACT: 0\n");
    }

    if (RegReadFldAlign(STA_SYNC1_03, VSYNC_P))
    {
        LOG(6, "VSYNC_P: 1\n");
    }
    else
    {
        LOG(6, "VSYNC_P: 0\n");
    }

    if (RegReadFldAlign(STA_SYNC1_03, HSYNC_P))
    {
        LOG(6, "HSYNC_P: 1\n");
    }
    else
    {
        LOG(6, "HSYNC_P: 0\n");
    }

    if (RegReadFldAlign(STA_SYNC1_03, FIELD_PIX))
    {
        LOG(6, "FIELD: 1\n");
    }
    else
    {
        LOG(6, "FIELD: 0\n");
    }

    if (RegReadFldAlign(STA_SYNC1_03, MODE_CHG_STA))
    {
        LOG(6, "MODE_CHG_STA: 1\n");
    }
    else
    {
        LOG(6, "MODE_CHG_STA: 0\n");
    }

    LOG(3, "HLEN: %x\n", wDSHtotalMeasure());
    LOG(3, "VLEN: %x\n", wDSVtotalMeasure());
    // HLEN_PIX:STA2_32
    // 0x20022bf0[11:0]
    tmp = (RegReadFldAlign(STA_SYNC1_06, H_LEN_PIX11_8) << 8) | RegReadFldAlign(STA_SYNC1_04, H_LEN_PIX7_0);
    UNUSED(tmp);
    LOG(3, "HLEN_PIX:%d\n", tmp);

    tmp = RegReadFldAlign(DVI_00, DVI_AV_START12_0);
    UNUSED(tmp);
    LOG(3, "DVI AV START %x\n", tmp);

    LOG(3, "DVI AV WITDH %x\n", RegReadFldAlign(DVI_00, DVI_AV_WIDTH));

    tmp = RegReadFldAlign(DVI_01, DVI_VDE_START11_0);
    UNUSED(tmp);
    LOG(3, "DVI VS START %x\n", tmp);
    LOG(3, "DVI VS WIDTH %x\n", RegReadFldAlign(DVI_01, DVI_VDE_WIDTH));
#if 0
    LOG(6, "DVI LEFTBC %x\n", wDrvDVIGetLeftBound());
    LOG(6, "DVI RIGHTBC %x\n", wDrvDVIGetRightBound());
    LOG(6, "DVI TOPBC %x\n", wDrvDVIGetTopBound());
    LOG(6, "DVI BOTTOMBC %x\n", wDrvDVIGetBottomBound());
#endif

    if (RegReadFldAlign(DVI_02, DVI_DE_MODE))
    {
        printf("DE mode\n");

#if ENABLE_DE_MODE
        printf("DVI DE VSYNC WIDTH %x\n", wDVIDEVsyncWidth());
        printf("DVI DE active width %d\n", wDVIDEWidth());
        printf("DVI DE active height %d\n", wDVIDEHeight());
        printf("DVI DE HTOTAL %d\n", wDVIDEHTotal());
        printf("DVI DE VTOTAL %d\n", wDVIDEVTotal());
#endif
    }
    else
    {
        LOG(3, "Non-DE mode\n");
    }

    if (RegReadFldAlign(DVI_02, DVI_DE_SP))
    {
        LOG(3, "DVI output port to SP1\n");
    }
    else
    {
        LOG(3, "DVI input port to SP1\n");
    }

    if (RegReadFldAlign(DVI_01, DVI_VS_SEL))
    {
        LOG(3, "programmed V\n");
    }
    else
    {
        LOG(3, "delay from input V\n");
    }

    if (RegReadFldAlign(DVI_01, DVI_HS_SEL))
    {
        LOG(3, "programmed H\n");
    }
    else
    {
        LOG(3, "delay from input H\n");
    }

    // data boundary related
    //LOG(3, "boundary detection data threshold: %x\n", RegReadFldAlign(DSYNC_00,BDDATATH));
    //LOG(3, "boundary detection channel selection: %x\n", RegReadFldAlign(DSYNC_00,BDINSEL));
    //LOG(6, "boundary detection R7_SEL: %x\n", (bReadVDOIN(VSRC00, 3) & 0x10));
    // data boundary input hsync palarity selection
    // data boundary input vsync palarity selection

    // software part
    // status of DVI decoder interface
    printf("sw: DVI width: %d\n", wDviInputWidth());
    printf("sw: DVI height: %d\n", wDviInputHeight());
    printf("sw: DVI refresh rate: %d\n", bDviRefreshRate());
    printf("sw: DVI interlace/progressive: %d\n", bDviInterlace());
    printf("sw: DVI signal status: %x\n", bDviSigStatus());
    // timing index
    printf("sw: _bDviTiming: %d\n", _bDviTiming);
#if ENABLE_DE_MODE
    printf("sw: pixel clock: %d\n", dwDviPIXClock());
#endif
    // h, v frequency
    //LOG(6, "sw: _bDviTmpVclk: %d\n", _bDviTmpVclk);
    //LOG(6, "sw: _wDviTmpHclk: %d\n", _wDviTmpHclk);
    printf("sw: _bDviVclk: %d\n", _bDviVclk);
    printf("sw: _wDviHclk: %d\n", _wDviHclk);
    printf("sw: vsync frequency: %d\n", bDviIVSClock());
    printf("sw: hsync frequency: %d\n", wDviIHSClock());
    printf("_bHDMIColorSpace (1=RGB , 0 =YCBCR) : %d\n", _bHDMIColorSpace);
#if ENABLE_DE_MODE
    // LOG(6, "sw: bRefreshRate: %d\n", bRefreshRate);
#endif
}

void vOmuxPatternGenOnOff(UINT8 bPath, UINT8 bMode)
{
#if 0
    if (bPath == SV_VP_MAIN)
    {
        if (bMode)
        {
            vRegWriteFldAlign(OMUX_00, 0xf, OMUX_MAIN_RAMP_EN);
        }
        else
        {
            vRegWriteFldAlign(OMUX_00, 0x0, OMUX_MAIN_RAMP_EN);
        }
    }
    else
    {
        if (bMode)
        {
            vRegWriteFldAlign(OMUX_00, 0xf, OMUX_PIP_RAMP_EN);
        }
        else
        {
            vRegWriteFldAlign(OMUX_00, 0x0, OMUX_PIP_RAMP_EN);
        }
    }
#else
    UINT8 bRamp, bFirstLine ;

    switch (bMode)
    {
        case 0: //off
        {
            bRamp = 0;
            bFirstLine = 0 ;
            break;
        }
        case 1: //ramp
        {
            bRamp = 0xf ;
            bFirstLine = 0 ;
            break;
        }
        case 2 : //full screen
        {
            bRamp = 0x0 ;
            bFirstLine = 1 ;
            break;
        }
        case 3 : //all
        {
            bRamp = 0xff ;
            bFirstLine = 1 ;
            break;
        }
        default:
            return ;
    }
    if (bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(OMUX_00, bRamp, OMUX_MAIN_RAMP_EN);
    }
    else
    {
        vRegWriteFldAlign(OMUX_00, bRamp, OMUX_PIP_RAMP_EN);
    }
    vRegWriteFldMulti(SYS_05, P_Fld(bFirstLine, FIRST_LINE_EN) | P_Fld(1, FIRST_LINE_V_POL) | P_Fld(bFirstLine, RAND_EN) | P_Fld(bPath, RAND_CH_SEL));

#endif
}

#ifdef CC_AUD_HDMI_SPDIF_CFG
 AUD_OUTPUT_STATE u1OutputState;
void AUD_CfgSpdifByHDMI(AUD_HDMI_CHL_STATUS * uChlStatus)
{
    u1OutputState = DigitalMute_AnalogMute;
    switch(uChlStatus->eACPType)
    {
        case ACP_NONE:
            switch(uChlStatus->eDataType)
            {
                case PCMData:
                    switch(uChlStatus->eSampeFreq)
                    {
                        case FS_32KHZ:
                        case FS_44KHZ:
                        case FS_48KHZ:
                            u1OutputState = DigitalUnMute_AnalogUnMute;
                            break;
                        default:
                            u1OutputState = DigitalMute_AnalogUnMute;
                            break;
                    }
                    break;
                case RAWData:
                    break;
                default:
                    break;
            }
            break;
        case ACP_CD:
            ///u1DecState = DEC_STOP;
            switch(uChlStatus->eDataType)
            {
                case PCMData:
                    switch(uChlStatus->eSampeFreq)
                    {
                        case FS_32KHZ:
                        case FS_44KHZ:
                        case FS_48KHZ:
                            u1OutputState = DigitalUnMute_AnalogMute;
                            break;
                        default:
                            break;
                    }
                    break;
                case RAWData:
                    break;
                default:
                    break;
            }
            break;
        case ACP_DVD_AUDIO:
        case ACP_SACD:
            break;
        default:
            break;
    }

    //_u1OutputState = u1OutputState;
    if(_rDVIStat.bIsMain)
    {
	vApiNotifyResolutionChg(SV_VP_MAIN,SV_VDO_STABLE);					
    }
   else
   {
	vApiNotifyResolutionChg(SV_VP_PIP,SV_VDO_STABLE);
   }
    //AUD_SpdifByHDMINtfy();

    // Notify middleware decoder status
    //pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_PROTECT_INFO, (UINT32)u1OutputState, 0);
    //LOG(5, "HDMI Change Channel State = %d.\n", u1OutputState);

    ///Set call follow func after MW stop dsp 
}
UINT8 API_Get_Audio_SPDIF_STATUS(void)
{
	return (UINT8) u1OutputState;
}
#endif
