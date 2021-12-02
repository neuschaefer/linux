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
 * $Author: jifeng.fan $
 * $Date: 2012/11/06 $
 * $RCSfile: vdp_frc.c,v $
 * $Revision: #6 $
 *
 *---------------------------------------------------------------------------*/

/*****************************************************************************
*  Notes: (MT5351)
*  (1) Deint initialize, 20050613
*      Z(657a800) Y(657a800) X(657a800) W(657a800), Normal Mode (ucDeintEn = 1, ucDeintNextEn = 1)
*      Z(657a800) Y(657a800) X(657a800) W(65e6800), Normal Mode (ucDeintEn = 1, ucDeintNextEn = 1)
*  ->  Z(657a800) Y(657a800) X(65e6800) W(65e6800), Deintelace Mode (ucDeintEn = 1, ucDeintNextEn = 0)
*  ->  Z(657a800) Y(65e6800) X(65e6800) W(64a2800), Deinterlace Mode (ucDeintEn = 1, ucDeintNextEn = 0)
*      ucDeintNextEn
*  (2) 32 pull down bad edit detection, 20050626
*      Z Y X W   (Frame n - 1) (Check comb register)
*        Z Y X W (Frame n)
*      At frame n, if merge ZY, check YX comb, if merge YX, check XW comb
*      At frame n, if merge ZY, YX < XW comb, if merge YX, XW < YX comb
*      At frame n, if merge YX, XW comb < (XW + threshold) comb of frame n - 3
*  (3) display fail (no frame buffer) at deinterlace mode, 20050621
*      Z(65e6800) Y(65e6800) X(6f6b800) W(6f6b800), XZ(53) WY(0) YX(0) WX(0) Reg(2014)
*      Z(65e6800) Y(6f6b800) X(6f6b800) W(650e800), XZ(0) WY(57) YX(0) WX(1340) Reg(2016)
*      Z(6f6b800) Y(6f6b800) X(650e800) W(650e800), XZ(57) WY(54) YX(1218) WX(0) Reg(2014)
*      Z(6f6b800) Y(650e800) X(650e800) W(6f6b800), XZ(54) WY(68) YX(0) WX(1488) Reg(2016)
*  ->  Z(6f6b800) Y(650e800) X(650e800) W(6f6b800), XZ(68) WY(54) YX(103) WX(3815) Reg(2016)
*  ->  Z(6f6b800) Y(650e800) X(650e800) W(6f6b800), XZ(54) WY(68) YX(1488) WX(0) Reg(2016)
*  ->  Z(650e800) Y(650e800) X(64a2800) W(6f6b800), XZ(68) WY(54) YX(103) WX(3815) Reg(2014)
*      a) we should skip bad edit detection, twice
*      b) we should update both X and W together when new frame rx
*      b) we should leave 32/22 pd mode
*  (4) VDP_HalSetDeintX, 20050621
*      Z = Y, Y = X, W = X = New
*      Save WXYZ at FRC, easy for chroma ZX flip
*      Update Address after function _VdpDeintHandleCounter, which will design still field direction (YZ or YX)
*      If merge YZ, flip chroma address of Z & X
*  (5) Access Frame, 20050621
*      ucProgressiveSeqFlag, ucValid32Seq, or NOT prFrcPrm->ucForceInterlaceSrc
*  (6) Access Top, 20050621
*      According to MPEG2 top field first
*  (7) 32 Pull Down Sequence
*      1  2->2  2
*         2<-2->2  3           '<-' or '->'? '->' is better, looking forward
*            2<-2  3  3
*               2  3->3  4
*                  3<-3  4  4
*      Example:
*      XZ(44) WY(39) YX(626) WX(0) Reg(2014)
*      XZ(39) WY(28) YX(0) WX(553) Reg(2014)
*      (0) PD Mode 1 >> 0
*      Bad-Edit(2) (27, 1c, 0, 229)
*  (8) Increase 32 motion threshold from 8 to 64 for 神鬼戰士 & TS pattern, 20050628
*  (9) 32 flag detection, 20050628
*      8 >> 24, make it more difficult to enter 32 flag
*      enter 32 pull down mode in the future
*  (10) Error handle at deinterlace mode, 20050628
*      When we detect discontiuous termporal reference, we will pause
*      deinterlace for 2 field, which will reduce sawtooth when error happen
*  (11) 32 pull down bad edit detection, 20050630
*      Z Y X W   (Frame n - 1) (Check comb register)
*        Z Y X W (Frame n)
*      At frame n, if merge ZY, check YX comb, if merge YX, check XW comb (Skip)
*      At frame n, if merge ZY, YX < XW comb, if merge YX, XW < YX comb (Skip)
*      Check motion counter
*  (12) Refine AV Sync timing, STC' = STC + 1 Vsync Time, display 1 Vsync earlier
*  (13) Scene Change, 20050630
*      If Current YW motion > (Previous YW motion + Threshold)
*      Based on MT8205 motion register
*  (14) I to I without deinterlace, 20050707
*      50i to 60i or 60i to 50i, playback Top only (DC)
*  (15) Bug Fixed of (10)
*      I we pause deinterlace, a flicker will happen while changing mode
*      worst than sawtooth!
*****************************************************************************/

/*****************************************************************************
*  Notes: (MT5371)
*  (1) If multiple field change due to TFF/RFF (_VdpBaseVSyncNs), skip FRC process
*****************************************************************************/


/** @file vdp_frc.c
 *  Brief of file vdp_frc.c.
 *  Details of file vdp_frc.c (optional).
 *  Porting From DTV_X_IDTV0801 1.8.2.88
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "vdp_drvif.h"
#include "vdp_if.h"  // Trick Play at 20081001
#include "b2r_if.h"
#include "fbm_drvif.h"
#include "b2r_drvif.h"
#include "vdp_frc.h"
#include "vdp_vsync.h"
#include "vdp_hal.h"
#include "vdp_debug.h"
#include "drv_di.h"
#include "aud_drvif.h"
#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "vdec_drvif.h"
#include "mute_if.h"
#include "tve_if.h"
#include "x_bim.h"
#ifdef ENABLE_MULTIMEDIA
#include "swdmx_if.h"
#endif

#include "x_lint.h"
//LINT_EXT_HEADER_BEGIN
#ifdef __MODEL_slt__
#include "slt_if.h"
#endif
#include "gfx_if.h"
#include "gfx_drvif.h"
#include "drv_common.h"
#include "drv_dbase.h"
#ifdef CC_SCPOS_EN
#include "drv_scpos.h"
#include "drv_video.h"
#endif
#include "x_hal_5381.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_syslog.h"
#include "x_timer.h"
#ifdef TIME_MEASUREMENT
#include "x_time_msrt.h"
#include "u_time_msrt_name.h"
#endif
//LINT_EXT_HEADER_END

//LINT_SUPPRESS_BRACE(717)        // Info 717: do ... while(0);

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//#define CC_NETFLIX_SUPPORT

/// Handle 32 Flag
#define VDP_HANDLE_32FLAG

#define VDP_EARLY_DISPLAY
//CLI test for Nordig HDTV overscan and frame cropping
//#define CC_CLI_TEST

// Monitor TFF
//#define VDP_MONITOR_TFF

// Frc Test
//#define CC_FRC_TEST

// Force 32 flag error
//#define CC_FORCE_32_FLAG_ERR

// 32 flag enable/disable test
//#define VDP_TEST_32FLAG

// Delay B Frame Buffer Release
//#define CC_VDP_FULL_B_RACE_DELAY

// Work around, Interlace FRC Fail, Between B2R/SCPOS
//#define CC_VDP_NO_INTERLACE_FRC

//#define CC_VDP_FPGA

//#define CC_VDP_FORCE_FIELD_ENCODE

//#define CC_VDP_FRC_DEBUG

//#define WA_1080P_SUB_NO_FRC // 1080p at sub, no 24/30 to 60 conversion

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define VDP_STC_CLOCK                   (90000)                     // 90 KHz
#define VDP_WAIT_1ST_PTS_NS             (12)
#define VDP_WAIT_NONZERO_PTS_NS         (5)
#define VDP_ZERO_PTS_INGORE_FILTER      (4)

#ifdef MHEG5_IC_SUPPORT_CERT
/* Modify for passing DTG MHEG5 ICS Test Suite  */
#define VDP_INVALID_DRIFT		        (VDP_STC_CLOCK * 1 / 10) // 100ms
#else
#define VDP_INVALID_DRIFT		        (VDP_STC_CLOCK * 10)		 // 6 Seconds
#endif

#define VDP_EARLY_DISP_DRIFT		    (VDP_STC_CLOCK * 1)		 // 1 Seconds

#ifdef MHEG5_IC_SUPPORT_CERT
/* Modify for passing DTG MHEG5 ICS Test Suite  */
#define VDP_MM_PEND_MAX                 (VDP_STC_CLOCK * 1 / 10) // 100ms
#else
#define VDP_MM_PEND_MAX                 (VDP_STC_CLOCK * 20)     //20 Seconds
#endif

#define VDP_INVALID_ASYNC_NS            (10)
#define VDP_MAX_PTS_FILTER_LEN          (4)                         // 4 tap, Enhance for SEOUL_CH11_1_RF14
#define VDP_MAX_PTS_DRIFT               (900)                       // 10 ms, Enhance for SEOUL_CH11_1_RF14
#define VDP_KEEP_DROP_B_NS              (10)                        // Enhance for SEOUL_CH11_1_RF14
#ifndef CC_B2R_DISABLE_ONE_SECOND_FRC
#define VDP_MAX_ASYNC_REPEAT_NS         (15)
#else
#define VDP_MAX_ASYNC_REPEAT_NS         (150)                        // should not more than 125
#endif
#define VDP_MAX_DEINT_WIDTH             (720)
#define VDP_DEBUG_BUF_SIZE              (16)
#define VDP_AVSYNC_TH                   (50 * VDP_STC_CLOCK / 1000) // 50 ms
#define VDP_VSYNC_CB_THD                (20)                        // 20 VSync ~= 330 ms

// after field trail (ch48_f49_0530_signal84_V_Shake)
#define PD_32_FLAG_LOOP_SIZE            (12)                        // 0.5 second
#define PD_32_FLAG_LEAVE_LOOP_SIZE      (6)                         // 0.25 second
#define PD_32_FLAG_SEQ                  (4)

#define B2R_MAX_HEIGHT                  (3840)
#define B2R_MAX_WIDTH                   (2560)

#define VDP_DISPLAY_FAIL_FREEZE         (255)

#define VDP_TFF_STABLE_COUNT            (12)

#define VDP_VIDEO_DELAY_VSYNC           (4)

#define VDP_WAIT_UNMUTE_VSYNC           (255)

#define STC_PTS_DIFF    (7500)

#define VDP_LIP_SYNC_OK 0
#define VDP_LIP_SYNC_NG 1

#ifndef VDP_ADD_1_VSYNC_TIME
#define VDP_ADD_1_VSYNC_TIME
#endif

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef void (*VDP_SET_ADDR_FUNC)(UCHAR ucVdpId, UINT32 u4AddrY, UINT32 u4AddrC);

/** Brief of VDP_PRM_T
 */
typedef struct _VDP_FREEZE_PRM_T
{
    UINT32 u4OnOff;
    UINT32 u4CopyField;
    UINT32 u4Recovery;
    UINT32 u4UnFreeze;
    UINT32 u4CleanDispQ;
} VDP_FREEZE_PRM_T;

/** Brief of VDP_GFX_COPY_FIELD_PRM_T
 */
typedef struct _VDP_GFX_COPY_FIELD_PRM_T
{
    UCHAR ucFbgId;
    UCHAR ucFbId;
    UINT32 u4Init;
    HANDLE_T hSemaphore;
    HANDLE_T hMutex;
} VDP_GFX_COPY_FIELD_PRM_T;

/* VDP trigger mode + audio master to do trigger mode */
typedef struct _VDP_TRIG_AUD_PRM_T
{
    UCHAR ucTriggerReceived;
    UINT32 u4TriggerPts;
} VDP_TRIG_AUD_PRM_T;

/* VDP Seek mode structure */
typedef struct _VDP_SEEK_PRM_T
{
    UCHAR ucSeekMode;
    UINT32 u4SeekPts;
    UCHAR ucNotifyTrigger;
    UCHAR ucABNotifyTrigger;
    BOOL fgABReach;
} VDP_SEEK_PRM_T;

typedef struct _VDP_FRC_COPY_PRM_T
{
    FRC_PRM_T rPortPrm;
    UINT32 u4TriggerCopy;
} VDP_FRC_COPY_PRM_T;

typedef struct _VDP_LIPSYNC_OK_NG_PRM_T
{
    UCHAR ucLipSyncState; // OK or NG
    UCHAR ucNg2OkCbEn;
    UCHAR ucNg2OkCbDone;
    UINT32 u4NumFrmsOK; //Number of frames after Lip-sync is OK
    UINT32 u4OKThrd;
    UINT32 u4OKFrmCnt; // Number of Frames OK accumulated
    UCHAR ucOk2NgCbEn;
    UCHAR ucOk2NgCbDone;
    UINT32 u4NumFrmsNG; //Number of frames after Lip-sync is NG
    UINT32 u4NGThrd;
    UINT32 u4NGFrmCnt; // Number of Frames NG accumulated
} VDP_LIPSYNC_OK_NG_PRM_T;

typedef struct _VDP_B2R_CRC_PRM_T
{
    UINT32 u4DramIndex;
    UINT32 u4DramChkSum;
    UINT32 u4PixelChkSum;
    UINT32 u4PSCANCrc;
    UINT32 u4PSCANSrcCrc;
} VDP_B2R_CRC_PRM_T;

#ifdef VDP_AVS_ADJUST_STC
typedef struct _B2R_AVS_T
{
    INT64   i8Delta;
    UINT32  u4Delay;
    BOOL    fgTest;
} B2R_AVS_T;
#endif


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define FRC_INC_VSYNC_NS(pFrcPrm)                                        \
          do {                                                            \
            if (pFrcPrm->ucCurrentVsyncNs < 0xFF)                        \
            { pFrcPrm->ucCurrentVsyncNs++; }                            \
          } while (0)

#define FRC_SET_VSYNC_NS(pFrcPrm, ucNs, ucChgNs)                        \
          do {                                                            \
            pFrcPrm->ucCurrentVsyncNs = 0;                                \
            pFrcPrm->ucChangeFieldVsyncNs = ucChgNs;                    \
            pFrcPrm->ucTargerVsyncNs = ucNs;                            \
          } while (0)

#define FRC_DISP_NS(pFrcPrm)                                            \
    ((pFrcPrm->ucCurrentVsyncNs >= pFrcPrm->ucTargerVsyncNs) ? 0 : (pFrcPrm->ucTargerVsyncNs - pFrcPrm->ucCurrentVsyncNs))

#define FRC_CHANGE_FIELD_NS(pFrcPrm)                                    \
    ((pFrcPrm->ucCurrentVsyncNs >= pFrcPrm->ucChangeFieldVsyncNs) ? 0 : (pFrcPrm->ucChangeFieldVsyncNs - pFrcPrm->ucCurrentVsyncNs))

#define FRC_DISP_FINISH(pFrcPrm)                                        \
    (pFrcPrm->ucCurrentVsyncNs >= pFrcPrm->ucTargerVsyncNs)

#define FRC_CHANGE_FIELD(pFrcPrm)                                        \
    (pFrcPrm->ucCurrentVsyncNs == pFrcPrm->ucChangeFieldVsyncNs)

#define FRC_MEMSET(addr, value, len)    (VERIFY(x_memset((addr), value, len) == (addr)))


#define GUESS_32_STEP(tff, rff)         (((1 - tff) << 1) + rff)


//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

#ifdef CC_FAST_CHANGE_CHANNEL_LOG
EXTERN HAL_TIME_T rNvgtStartAvTime;
EXTERN HAL_TIME_T rNvgtVideoStartTime;
EXTERN HAL_TIME_T rNvgtAvSyncTime;
EXTERN UINT32 u4NvgtTimeAhead;
#endif

#ifdef VDP_AVS_ADJUST_STC
B2R_AVS_T _rB2rAvs;
#endif


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
extern BOOL STC_ChkStcStatusDrift(UINT8 u1StcId);

extern BOOL STC_ChkStcStatusValid(UINT8 u1StcId);

extern BOOL _VDP_FlushB2RChgFrameMsg(VOID);

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

//extern BOOL fgApiVideoStable(UINT8 u1VideoFrame); // 0: main frame, 1: sub frame for PIP/POP

#ifdef CC_B2R_GFX_COPY_FIELD
static void _VdpGfxRoutine(void* pvArg);
#endif

#ifdef CC_B2R_ENABLE_SEAMLESS_THREAD
static VOID _SeamlessRoutine(void* pvArg);
static VOID _VDP_SeamlessJob(FRC_PRM_T* prFrcPrm);
#endif

static void _VdpGfxCopyFieldInit(void);

static UINT32 _VdpGfxCopyFieldStart(UCHAR ucFbgId, UCHAR ucFbId);

static void _VdpFrmInit(UCHAR ucVdpId, UCHAR ucPort);

static void _VdpChangeFrameBuffer(FRC_PRM_T* prFrcPrm);

static void _VdpGetNextDisp(FRC_PRM_T* prFrcPrm);

static UCHAR _VdpPreLookNextDisp(FRC_PRM_T* prFrcPrm);

static void _VdpCalculateVsyncNs(FRC_PRM_T* prFrcPrm);

static void _VdpBaseVSyncNs(FRC_PRM_T* prFrcPrm);

static void _VdpFrameRateProc(FRC_PRM_T* prFrcPrm);

static void _VdpAVSyncProc(FRC_PRM_T* prFrcPrm);

static void _VdpChk32Flag(FRC_PRM_T* prFrcPrm);

static void _VdpCheckOutputMode(FRC_PRM_T* prFrcPrm);

static void _VdpErrNotify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);

static void _VdpFrcFrameBufferReady(UCHAR ucFbgId);

static void _VdpTriggerAudOn(UCHAR ucVdpId, UINT32 u4CurPts);

static UINT32 _VDP_FrcGetStc(UCHAR ucFbgId);

static void _VdpSendAfdChgMsg(FBM_SEQ_HDR_T* prSeqHdr, FRC_PRM_T* prFrcPrm);

/* VDP Seek done notification */
void _VDPSetSeek(UCHAR ucVdpId, UCHAR ucSet, UINT32 u4Pts);
void _VDPSetABRepeat(UCHAR ucVdpId, UCHAR ucSet);
static void _VDPGetSeek(UCHAR* pucSeekMode, UINT32* pu4Pts, UCHAR* pucNotifyTrigger);

#ifdef CC_FRC_TEST
static void _VdpFrcTest(FRC_PRM_T* prFrcPrm);
#endif

#ifdef VDP_AVS_ADJUST_STC
static VOID _VDP_SetAvsyncDelta(B2R_AVS_T* prAvs, UINT32 u4Delta,BOOL bAddFlg);
static VOID _VDP_SetDeltaInit(B2R_AVS_T* prAvs);
#endif

static void _VDP_FrcQueryStatus(UINT32 u4B2rId);


#ifdef DIVX_PLUS_CER
#ifdef   CC_B2R_CHAPTER_CHG
static VDP_CHAPTER_T* _VDP_FrcGetChapterObj(VOID);
static VDP_CHAPTER_T  _arVdpChapter;
static BOOL fgFirstSetChapter = FALSE;
#endif
#endif
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

#ifdef CC_B2R_ENABLE_CHG_FRAME_MSG
static HANDLE_T _hChgFrameQueue = (HANDLE_T)(NULL);
#endif

#ifdef CC_B2R_ENABLE_SEAMLESS_THREAD
static BOOL _fgSeamlessThreadInit = FALSE;
static HANDLE_T _hSeamlessThread = 0;
static HANDLE_T _hSeamlessSema = 0;
#endif

#if defined(CC_NETFLIX_SUPPORT) || defined(CC_RESIZE_SMALL_IMAGE)

static UINT32 _pu1YResizeFrame,_pu1CResizeFrame,_pu1YResizeFrame2,_pu1CResizeFrame2;
//static HAL_TIME_T _rTimeRstS, _rTimeRstE, _rTimeRstDt;
static BOOL _fgFirstFrame = TRUE;
#define IMG_RESIZE_ALIGN_SIZE 2047
#define IMG_RESIZE_ALIGN(X)  (((X) + IMG_RESIZE_ALIGN_SIZE) & (~IMG_RESIZE_ALIGN_SIZE))
#endif

static FBM_ASPECT_RATIO_T _rFbmAspectRatio;

static VDP_PRM_T    _arVdpPrm[B2R_NS];

static UINT32        _u4ReturnValue;

static STC_SPEED_TYPE_T _eSpeed[B2R_NS] = {STC_SPEED_TYPE_FORWARD_1X,STC_SPEED_TYPE_FORWARD_1X};
static BOOL _fgPauseMM[B2R_NS] = {FALSE,FALSE};

static INT32 _i4TimeShiftSpeed[B2R_NS] = {100,100};
static BOOL _fgTimeShift[B2R_NS] = {FALSE,FALSE};

static DRV_DBASE_T* _prDbase = NULL;

#ifdef VDP_HANDLE_32FLAG
static UINT32 _au4Pd32FlagCadence[2][PD_32_FLAG_SEQ] =
{
    {1, 0, 1, 0},
    {1, 0, 0, 1}
};
#endif

static VDP_CONF_T* _prVdpConf[VDP_NS];

static VDP_FREEZE_PRM_T _arVdpDtvFreeze[VDP_NS] =
{
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0}
};

#ifdef CC_B2R_GFX_COPY_FIELD
static VDP_GFX_COPY_FIELD_PRM_T _rVdpGfxCopyField =
{
    FBM_FBG_ID_UNKNOWN,
    FBM_FB_ID_UNKNOWN,
    0,
    0,
    0
};
#endif

/* VDP trigger mode + audio master to do trigger mode */
static VDP_TRIG_AUD_PRM_T _rVdpTrigAud =
{
    0,
    0
};

/* VDP Seek done notification */
//static VDP_SEEK_PRM_T _rVdpSeek =
VDP_SEEK_PRM_T _rVdpSeek =
{
    0,
    0,
    0,
    0,
    0
};



static VDP_FRC_COPY_PRM_T _rFrcCopy;

static VDP_LIPSYNC_OK_NG_PRM_T _rVdpLipSync =
{
    VDP_LIP_SYNC_NG,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};

#ifdef __MODEL_slt__
UINT32 b2rState = 0xff;
#endif

static BOOL _fgInvalidFrmRate = FALSE;
static BOOL _fgHandleSeqChg = FALSE;
static BOOL _fgEnableWaitSTC = FALSE;

static UCHAR _ucVsyncNotStableCount = 0;
static UCHAR _ucFrameRateUnkownCount = 0;

#ifdef VDP_B2R_PSCAN_CRC_FLOW
//extern UINT32 u4DramIndex;
extern UINT32 u4DRAMChkSum;
extern UINT32 u4PixelChkSum;
extern UINT32 u4PSCANCrc;
extern UINT32 u4PSCANSrcCrc;
#endif

#ifdef VDP_B2R_PSCAN_CRC_FLOW
VDP_B2R_CRC_PRM_T _rVdpB2rCrc;
static UINT32 _u4SetPts = 0xFFFFFFFF;
static BOOL _fgTriggerEnc = FALSE;
#endif

EXTERN UCHAR _arEs2Fbg[MAX_ES_NS];

#ifdef CC_3D_MM_DS_SUPPORT
//static HAL_TIME_T _rTimeRstS, _rTimeRstE, _rTimeRstDt;
static UINT8 _u1SubFrameReleadFbId = FBM_FB_ID_UNKNOWN;
static UINT8 _u1SubFrameLockFbId = FBM_FB_ID_UNKNOWN;
static UINT8 _u1SubFrameFbId = FBM_FB_ID_UNKNOWN;
static UINT8 _u1SubFrameFbgId = FBM_FBG_ID_UNKNOWN;
static UINT32 _u4SubFrameAddrY = 0;
static UINT32 _u4SubFrameAddrC = 0;
static BOOL _fgEos = FALSE;
#endif

#ifdef CC_B2R_3D_ERROR_SUPPORT
static UINT32 _u4MainFrameAddrY = NULL;
static UINT32 _u4MainFrameAddrC = NULL;
#endif


//wait NPTV stable
//#define CC_B2R_WAIT_NPTV_STABLE
#ifdef CC_B2R_WAIT_NPTV_STABLE
static BOOL _fgNPTVStable = FALSE;

static UINT32 _u4WaitNPTVStableCount = 0;

static UINT8 _u1PlayMode = FBM_FBG_MODE_NS;
#endif

//for netflix
static UINT32 _u4PreviousResizeWidth = 0;
static UINT32 _u4PreviousResizeHeight =0;

//for counting drift
static UINT32 _u4DriftCount = 0;
static UINT32 _u4MaxDriftCount = 0;
static UINT32 _u4ValidCount = 0;

//boot time profile
static BOOL _fgB2RProfile = FALSE;

#if defined(CC_B2R_EMULATION)
static UINT32 _au4B2rCrcArray[4096];
static UINT32 _au4B2rCrcArray2[4096];
static UINT32 _au4B2rCrcArray3[4096];
static UINT32 _au4B2rCrcArray4[4096];
static UINT32 _au4B2rCrcSave[4096];
static UINT32 _u4B2rCrcInx =0;
static UINT32 _u4InterruptCount =0;
static UINT32 _au4DispSuccessIdxArray[4096];
static UINT32 _u4DispSuccessIdx =0;
static UINT32 _u4CheckDispSuccessIdx =0;
BOOL _fgSetFirstAddr = FALSE;
static BOOL _fgDisableLateNotify = TRUE;
EXTERN UINT32 _B2rHdGetCrc(void);

static BOOL _bSaveB2rCrc = FALSE;
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

#ifdef CC_B2R_GFX_COPY_FIELD
//-----------------------------------------------------------------------------
/** Brief of _VdpGfxRoutine.
 */
//-----------------------------------------------------------------------------

static VOID _VDP_MemCpy(UINT8 *pu1TopStartAddr,
                        UINT32 u4TotalLines, UINT32 u4LineWidth)
{

    UINT8 *pu1BotStartAddr;
    UINT8 *pu1TopCurAddr;
    UINT8 *pu1BotCurAddr;
    UINT32 i;

    // bottom field start address
    pu1BotStartAddr = (UINT8 *)((UINT32)pu1TopStartAddr + u4LineWidth);

    // set top and bottom current address
    pu1TopCurAddr = pu1TopStartAddr;
    pu1BotCurAddr = pu1BotStartAddr;

    for (i = 0; i < u4TotalLines; i++)
    {
        //GFX_SetSrc(pu1TopCurAddr, (UINT32)CM_RGB_CLUT8, u4Pitch);

        //GFX_SetDst(pu1BotCurAddr, (UINT32)CM_RGB_CLUT8, u4Pitch);

        //GFX_BitBlt(0, 0, 0, 0, u4LineWidth, 1);
        x_memcpy(pu1BotCurAddr,pu1TopCurAddr,u4LineWidth);
        pu1TopCurAddr = (UINT8 *)((UINT32)pu1TopCurAddr + (2 * u4LineWidth));
        pu1BotCurAddr = (UINT8 *)((UINT32)pu1BotCurAddr + (2 * u4LineWidth));

    } // ~for
}


static void _VdpGfxRoutine(void* pvArg)
{
    UNUSED(pvArg);

    while (1)
    {
        FBM_SEQ_HDR_T* prSeqHdr;
        UINT32 u4AddrY;
        UINT32 u4AddrC;

        // wait semaphore
        VERIFY (x_sema_lock(_rVdpGfxCopyField.hSemaphore, X_SEMA_OPTION_WAIT) == OSR_OK);

        // mutex
        VERIFY (x_sema_lock(_rVdpGfxCopyField.hMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

        prSeqHdr = FBM_GetFrameBufferSeqHdr(_rVdpGfxCopyField.ucFbgId);

        FBM_GetFrameBufferAddr(_rVdpGfxCopyField.ucFbgId, _rVdpGfxCopyField.ucFbId, &u4AddrY, &u4AddrC);

        if ((u4AddrY != 0) && (u4AddrC != 0))
        {
            UINT32 u4FbWidth;
            UINT32 u4TotalLine;
            UINT32 u4LineWidth;

            LOG(3, "Copy (%d, %d)\n", prSeqHdr->u2LineSize, prSeqHdr->u2VSize);

            // Y Block = 64 * 32
            u4FbWidth = prSeqHdr->u2LineSize;
            u4LineWidth = 64;
            u4FbWidth = (u4FbWidth + (u4LineWidth - 1)) & (~(u4LineWidth - 1));
            u4TotalLine = prSeqHdr->u2VSize * (u4FbWidth / u4LineWidth);

            u4TotalLine >>= 1;  // 1 Line of vdp = 1 Line of gfx

            LOG(3, "Copy Y(%d %d)\n", u4TotalLine, u4LineWidth);

            //GFX_CopyTopfieldToBotfield((UINT8 *)u4AddrY, u4TotalLine, u4LineWidth);
            _VDP_MemCpy((UINT8 *)u4AddrY, u4TotalLine, u4LineWidth);

            // C Block = (32 * 2) * 16
//            u4FbWidth = prSeqHdr->u2LineSize;
//            u4LineWidth = 64;
//            u4FbWidth = (u4FbWidth + (u4LineWidth - 1)) & (~(u4LineWidth - 1));
//            u4TotalLine = (prSeqHdr->u2VSize / 2) * (u4FbWidth / u4LineWidth);
            u4TotalLine >>= 1;  // C = Y/2

            LOG(3, "Copy C(%d %d)\n", u4TotalLine, u4LineWidth);

            //GFX_CopyTopfieldToBotfield((UINT8 *)u4AddrC, u4TotalLine, u4LineWidth);
            _VDP_MemCpy((UINT8 *)u4AddrC, u4TotalLine, u4LineWidth);

            LOG(3, "Copy Finish\n");
        }
        else
        {
            ASSERT(0);
        }

        // mutex
        VERIFY (x_sema_unlock(_rVdpGfxCopyField.hMutex) == OSR_OK);
    }
}

#endif

//-----------------------------------------------------------------------------
/** Brief of _VdpGfxCopyFieldInit.
 */
//-----------------------------------------------------------------------------
static void _VdpGfxCopyFieldInit(void)
{
#ifdef CC_B2R_GFX_COPY_FIELD
    if (_rVdpGfxCopyField.u4Init == 0)
    {
        HANDLE_T hThread1;

        // create semaphore
        VERIFY (x_sema_create(&_rVdpGfxCopyField.hSemaphore, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

        // create semaphore
        VERIFY (x_sema_create(&_rVdpGfxCopyField.hMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);

        // create thread, priority = higher than mpv but lower than vdp
        VERIFY (x_thread_create(&hThread1, "VDP-G", VSYNC_STACK_SIZE, (MPV_THREAD_PRIORITY - 1),
                                _VdpGfxRoutine, 0, NULL) == OSR_OK);

        _rVdpGfxCopyField.u4Init = 1;
    }
#endif
}

//-----------------------------------------------------------------------------
/** Brief of _VdpGfxCopyFieldStart.
 */
//-----------------------------------------------------------------------------
static UINT32 _VdpGfxCopyFieldStart(UCHAR ucFbgId, UCHAR ucFbId)
{
#ifdef CC_B2R_GFX_COPY_FIELD
    UCHAR ucFbNs;
    UCHAR ucFRefFbId;
    UCHAR ucBRefFbId;

    if (ucFbgId == FBM_FBG_ID_UNKNOWN)
    {
        LOG(3, "Start Fail 1\n");
        return 0;
    }

    ucFbNs = FBM_GetFrameBufferNs(ucFbgId);

    if (ucFbId >= ucFbNs)
    {
        LOG(3, "Start Fail 2\n");
        return 0;
    }

    if (FBM_CheckFrameBufferStatus(ucFbgId, ucFbId, FBM_FB_STATUS_LOCK) == 0)
    {
        LOG(3, "Start Fail 3\n");
        return 0;
    }

    FBM_GetRefFrameBuffer(ucFbgId, &ucFRefFbId, &ucBRefFbId);

    if ((ucFbId == ucFRefFbId) || (ucFbId == ucBRefFbId))
    {
        LOG(3, "Start Fail 4\n");
        return 0;
    }

    _rVdpGfxCopyField.ucFbgId = ucFbgId;
    _rVdpGfxCopyField.ucFbId = ucFbId;

    // release semaphore
    VERIFY (x_sema_unlock(_rVdpGfxCopyField.hSemaphore) == OSR_OK);

    return 1;
#else
    UNUSED(ucFbgId);
    UNUSED(ucFbId);

    return 1;
#endif
}


#ifdef CC_B2R_ENABLE_SEAMLESS_THREAD
static VOID _SeamlessRoutine(void* pvArg)
{
    UNUSED(pvArg);

    while (1)
    {
        //wait sema
        VERIFY (x_sema_lock(_hSeamlessSema, X_SEMA_OPTION_WAIT) == OSR_OK);
        _VDP_SeamlessJob(&_arVdpPrm[0].rPortPrm);
    }
}
#endif

#ifdef B2R_AUTO_TEST
void B2R_VirtualCompareCrc(UINT32 u4Idx)
{
    UINT32 u4RegCrc;
    UINT32 u4FrmCrc;

    u4RegCrc = _B2rHdGetCrc();
    u4FrmCrc = FBM_VirtualGetFrmCrc(u4Idx);

    if( u4RegCrc !=  u4FrmCrc )
    {
        LOG(0,"Idx(%d) CRC not equal! (R=0x%x,F=0x%x)\n",u4Idx,u4RegCrc,u4FrmCrc);
    }

    return;
}
#endif

#ifdef CC_3D_MM_DS_SUPPORT
static UINT8 _VDP_GetAdditionalFbgId(FRC_PRM_T* prFrcPrm)
{
    UINT32 u4idx =0;

    _u1SubFrameFbgId = FBM_FBG_ID_UNKNOWN;
    for(u4idx=0 ; u4idx < MAX_ES_NS ; u4idx++)
    {
        if(_arEs2Fbg[u4idx] != FBM_FBG_ID_UNKNOWN && _arEs2Fbg[u4idx] != prFrcPrm->ucFbgId)
        {
            _u1SubFrameFbgId = _arEs2Fbg[u4idx];
        }
    }
    if(_u1SubFrameFbgId == FBM_FB_ID_UNKNOWN)
    {
        _u1SubFrameReleadFbId = FBM_FB_ID_UNKNOWN;
        _u1SubFrameLockFbId = FBM_FB_ID_UNKNOWN;
        _u1SubFrameFbId = FBM_FB_ID_UNKNOWN;
        _u4SubFrameAddrY = 0;
        _u4SubFrameAddrC = 0;
    }
    return _u1SubFrameFbgId;
}

static VOID _VDP_GetSubFrameFb(FRC_PRM_T* prFrcPrm,UINT32* pu4AddrY,UINT32* pu4AddrC)
{
    //get another video frame
    if(_u1SubFrameFbgId != 0xff)
    {
        //UINT8 u1DecoderSrcId = FBM_GetDecoderSrcId(_u1SubFrameFbgId);
        UINT8 u1DecoderSrcId = FBM_GetDecoderSrcId(prFrcPrm->ucFbgId);
        UINT8 u1FbId;

        FBM_SetFrameBufferFlag(_u1SubFrameFbgId, FBM_FLAG_DISPLAYING);

        if(u1DecoderSrcId >= B2R_NS)
        {
            LOG(3,"_VDP_GetSubFrameFb u1DecoderSrcId > B2R_NS \n");
            return;
        }

        u1FbId = FBM_GetFrameBufferFromDispQ(_u1SubFrameFbgId);

        if(u1FbId != FBM_FB_ID_UNKNOWN)
        {
            FBM_GetFrameBufferAddr(_u1SubFrameFbgId,u1FbId, pu4AddrY, pu4AddrC);

            FBM_SetFrameBufferStatus(_u1SubFrameFbgId, u1FbId, FBM_FB_STATUS_LOCK);

            if (FBM_ChkFrameBufferPicFlag(_u1SubFrameFbgId, u1FbId, FBM_MM_EOS_FLAG))
            {
                FBM_ClrFrameBufferPicFlag(_u1SubFrameFbgId, u1FbId, FBM_MM_EOS_FLAG);
                _VDP_VsyncNotify(VDP_MSG_MM_CB, (UINT32)prFrcPrm->ucVdpId, VDP_COND_EOS, FBM_GetDecoderSrcId(_u1SubFrameFbgId));
                _fgEos = TRUE;
            }

            if(_u1SubFrameReleadFbId != FBM_FB_ID_UNKNOWN)
            {
                FBM_SetFrameBufferStatus(_u1SubFrameFbgId, _u1SubFrameReleadFbId, FBM_FB_STATUS_EMPTY);
            }

            if(_u1SubFrameLockFbId != FBM_FB_ID_UNKNOWN)
            {
                _u1SubFrameReleadFbId = _u1SubFrameLockFbId;
            }
            _u1SubFrameFbId = u1FbId;
            _u1SubFrameLockFbId = u1FbId;

        }
    }
    else
    {
        _u1SubFrameReleadFbId = 0xff;
        *pu4AddrY = 0;
        *pu4AddrC = 0;
    }

}
#endif

static VOID _VDP_SeamlessJob(FRC_PRM_T* prFrcPrm)
{
    UINT32 u4AddrY      =   0x0;
    UINT32 u4AddrC      =   0x0;
    UINT32 u4YSize      =   0x0;
    UINT32 u4RYSize     =   0x0;
    UINT32 u4FbmWith    =   0x0;
    UINT32 u4FbmHeight  =   0x0;
    UINT8 u1DecoderSrcId = FBM_GetDecoderSrcId(prFrcPrm->ucFbgId);
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep = NULL;

    if (prFrcPrm->ucVdpId >= VDP_NS)
    {
        ASSERT(prFrcPrm->ucVdpId < VDP_NS);
        return;
    }

    if(u1DecoderSrcId >= B2R_NS)
    {
        LOG(3,"_VDP_GetSubFrameFb u1DecoderSrcId > B2R_NS \n");
        return;
    }

    FBM_GetFrameBufferAddr(prFrcPrm->ucFbgId,prFrcPrm->ucFbId, &u4AddrY, &u4AddrC);

#ifdef B2R_AUTO_TEST
    {
        UINT32 u4PlayIdx;
        u4PlayIdx = FBM_VirtualGetPlayIdx();

        // if( 0xff != u4PlayIdx )
        {
            if( u4PlayIdx > 0x0 )
            {
                B2R_VirtualCompareCrc( (u4PlayIdx - 1 ) );
            }

            FBM_VirtualSetPlayIdx( u4PlayIdx + 1 );
        }
    }
#endif


#if defined(CC_NETFLIX_SUPPORT) || defined(CC_RESIZE_SMALL_IMAGE)
    {
        FBM_PIC_HDR_T* prFbmPicHdr = NULL;
        FBM_SEQ_HDR_T* prFbmSeqHdr = NULL;
        UINT32 u4YResizeAddr,u4CResizeAddr;
        RZ_VDO_SCL_PARAM_SET_T rScaleParamV;
        FBM_POOL_T* prFbmPool;

        x_memset(&rScaleParamV,0,sizeof(RZ_VDO_SCL_PARAM_SET_T));

        prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prFrcPrm->ucFbgId);
        prFbmPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, prFrcPrm->ucFbId);
        //LOG(1,"Pts(%x) FbId(%d)\n",prFbmPicHdr->u4PTS,prFrcPrm->ucFbId);

        if ((prFbmPicHdr != NULL) && (prFbmSeqHdr != NULL))
        {
            UINT8 u1PicType = VDEC_PIC_TYPE_I;

            if(prFbmPicHdr->ucPicCdTp ==MPEG_PIC_TYPE_I)
            {
                u1PicType = VDEC_PIC_TYPE_I;
            }
            else if(prFbmPicHdr->ucPicCdTp ==MPEG_PIC_TYPE_P)
            {
                u1PicType = VDEC_PIC_TYPE_P;
            }
            else if(prFbmPicHdr->ucPicCdTp ==MPEG_PIC_TYPE_B)
            {
                u1PicType = VDEC_PIC_TYPE_B;
            }
            else
            {
                u1PicType = VDEC_PIC_TYPE_I;
            }

            _VDP_VsyncNotify(VDP_MSG_GET_PIC_TYPE_CB, prFrcPrm->ucVdpId,u1DecoderSrcId,u1PicType);

#if defined(CC_B2R_EMULATION)
            _au4DispSuccessIdxArray[_u4DispSuccessIdx] = _u4InterruptCount;

            if(_bSaveB2rCrc)
            {
                _au4B2rCrcArray2[_u4DispSuccessIdx++] = prFbmPicHdr->i4TemporalRef;
            }
            else
            {
                _au4B2rCrcArray3[_u4DispSuccessIdx++] = prFbmPicHdr->i4TemporalRef;
            }
#endif

#if defined(CC_NETFLIX_SUPPORT)

            prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_MPEG);

            if (((prFbmSeqHdr->fgResizeSmallPic)
                    || (prFbmPicHdr->fgSeamlessDisp && (prFbmPool != NULL) && (prFbmPool->u4Size > (6*1024*1024))))
                    /*#ifdef CC_3D_MM_DS_SUPPORT
                    && (_u1SubFrameFbgId == FBM_FBG_ID_UNKNOWN)
                    #endif*/
               )
#else //ifdef CC_RESIZE_SMALL_IMAGE
            if (((prFbmSeqHdr->fgResizeSmallPic) || (prFbmPicHdr->fgSeamlessDisp))
                    /*#ifdef CC_3D_MM_DS_SUPPORT
                    && (_u1SubFrameFbgId == FBM_FBG_ID_UNKNOWN)
#endif*/
                )
#endif
            {
                _pu1YResizeFrame = 0;
                _pu1CResizeFrame = 0;
                _pu1YResizeFrame2 = 0;
                _pu1CResizeFrame2 = 0;

#ifdef CC_RESIZE_SMALL_IMAGE
                if (prFbmSeqHdr->fgResizeSmallPic)
                {
                    UINT32 u4AddrYTmp;
                    UINT32 u4AddrCTmp;
                    UINT32 u4HeightAlign = (((prFbmPicHdr->u4PicHeight) + 31) & (~31));
                    // Use rest memory of original frame buffer
                    FBM_GetFrameBufferAddr(prFrcPrm->ucFbgId, (UCHAR)0, &u4AddrYTmp, &u4AddrCTmp);
                    u4YSize = prFbmPicHdr->u4PicWidthPitch * u4HeightAlign;

                    if ((u4AddrYTmp > 0) && (u4AddrCTmp > 0))
                    {
                        _pu1YResizeFrame = (u4AddrYTmp + (UINT32)u4YSize);
                        _pu1CResizeFrame = (u4AddrCTmp + (UINT32)(u4YSize/2));
                    }

                    if ((u4AddrYTmp > 0) && (u4AddrCTmp > 0))
                    {
                        FBM_GetFrameBufferAddr(prFrcPrm->ucFbgId, (UCHAR)1, &u4AddrYTmp, &u4AddrCTmp);
                        _pu1YResizeFrame2 = (u4AddrYTmp + (UINT32)u4YSize);
                        _pu1CResizeFrame2 = (u4AddrCTmp + (UINT32)(u4YSize/2));
                    }
                }
#endif

#ifdef CC_NETFLIX_SUPPORT
                if (prFbmPicHdr->fgSeamlessDisp)
                {
#ifdef ENABLE_MULTIMEDIA
                    UCHAR  ucFbNS   = 0x0;

                    ucFbNS = FBM_GetFrameBufferNs(prFrcPrm->ucFbgId);
                    FBM_GetFrameBufferSize(prFrcPrm->ucFbgId,&u4FbmWith,&u4FbmHeight);
                    u4YSize = (u4FbmWith*u4FbmHeight);
                    //VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep = NULL;
                    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ES0);

                    if(prVdecEsInfoKeep->eCurFMT == VDEC_FMT_RV)
                    {
                        _pu1YResizeFrame    = (prFbmPool->u4Addr +
                                               (((ucFbNS*u4YSize*3)>>1)+
                                                FBM_FBG_TYPE_PAL_Y_SIZE + FBM_FBG_TYPE_PAL_C_SIZE));
                        _pu1CResizeFrame    = _pu1YResizeFrame  + FBM_FBG_TYPE_PAL_Y_SIZE;
                        _pu1YResizeFrame2   = _pu1CResizeFrame  + FBM_FBG_TYPE_PAL_C_SIZE;
                        _pu1CResizeFrame2   = _pu1YResizeFrame2 + FBM_FBG_TYPE_PAL_Y_SIZE;
                    }
                    else
                    {
                        /*_pu1YResizeFrame    = (prFbmPool->u4Addr +
                                               (((ucFbNS*u4YSize*3)>>1)+
                                                FBM_RESIZE_RPR_BUF_JUMP_SIZE));*/
                        _pu1YResizeFrame = prFbmPool->u4Addr + FBM_GetRunningVdecMemSize(prFrcPrm->ucFbgId);
                        _pu1CResizeFrame    = _pu1YResizeFrame  + u4YSize;
                        _pu1YResizeFrame2   = _pu1CResizeFrame  + (u4YSize/2);
                        _pu1CResizeFrame2   = _pu1YResizeFrame2 + u4YSize;
                    }

#endif
                }
#endif

                if (_fgFirstFrame)
                {
                    u4YResizeAddr = (UINT32)IMG_RESIZE_ALIGN(_pu1YResizeFrame);
                    u4CResizeAddr = (UINT32)IMG_RESIZE_ALIGN(_pu1CResizeFrame);
                }
                else
                {
                    u4YResizeAddr = (UINT32)IMG_RESIZE_ALIGN(_pu1YResizeFrame2);
                    u4CResizeAddr = (UINT32)IMG_RESIZE_ALIGN(_pu1CResizeFrame2);
                }

                if ((prFbmPicHdr->u4PicWidth > 0) &&
                        (prFbmPicHdr->u4PicHeight > 0) &&
                        (u4YResizeAddr) &&
                        (u4CResizeAddr))
                {
                    // FBM ready
                    UINT32 u4Xaddr=0;
                    UINT32 u4Yaddr=0;
                    //VDP_REGION_T rSrcRegion;
                    //UCHAR ucSrcFullRegion = 0;

                    if (_fgFirstFrame)
                    {
                        _fgFirstFrame = FALSE;
                    }
                    else
                    {
                        _fgFirstFrame = TRUE;
                    }
                    if (prFbmPicHdr->fgSeamlessDisp)
                    {
                        //clear target buffer
                        //if(prFbmSeqHdr->fgCleanSeamlessBuffer)

                        if (_u4PreviousResizeWidth != prFbmSeqHdr->u4ResizeWidth || _u4PreviousResizeHeight != prFbmSeqHdr->u4ResizeHeight)
                        {
                            //GFX_Memset((UINT8*)IMG_RESIZE_ALIGN(_pu1YResizeFrame),u4FbmWith,u4FbmHeight,0);
                            //GFX_Memset((UINT8*)IMG_RESIZE_ALIGN(_pu1CResizeFrame),u4FbmWith,u4FbmHeight/2,0x80);

                            //GFX_Memset((UINT8*)IMG_RESIZE_ALIGN(_pu1YResizeFrame2),u4FbmWith,u4FbmHeight,0);
                            //GFX_Memset((UINT8*)IMG_RESIZE_ALIGN(_pu1CResizeFrame2),u4FbmWith,u4FbmHeight/2,0x80);

                            GFX_Memset((UINT8*)IMG_RESIZE_ALIGN(u4YResizeAddr),u4FbmWith,u4FbmHeight,0);
                            GFX_Memset((UINT8*)IMG_RESIZE_ALIGN(u4CResizeAddr),u4FbmWith,u4FbmHeight/2,0x80);
                        }

                        _u4PreviousResizeWidth = prFbmSeqHdr->u4ResizeWidth;
                        _u4PreviousResizeHeight = prFbmSeqHdr->u4ResizeHeight;

                        rScaleParamV.u4VdoCbCrSwap =0;
                        
                        //calculate aspect ratio
                        if (prFbmSeqHdr->u4ResizeWidth < prFbmSeqHdr->u2HSize)
                        {
                            u4Xaddr = (prFbmSeqHdr->u2HSize - prFbmSeqHdr->u4ResizeWidth)/2;
                            if(u4Xaddr %2 != 0)
                            {
                                rScaleParamV.u4VdoCbCrSwap =1;
                            }
                        }
                        if (prFbmSeqHdr->u4ResizeHeight < prFbmSeqHdr->u2VSize)
                        {
                            u4Yaddr = (prFbmSeqHdr->u2VSize - prFbmSeqHdr->u4ResizeHeight)/2;
                            /*if(u4Yaddr %2 !=0)
                            {
                                u4Yaddr -= 1;
                            }*/
                        }
                    }
                    else
                    {
                        u4Yaddr = 0;
                        u4Xaddr = 0;
                    }

#if defined(CC_MT5395) || defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)// Force raster out for VENC
                    if(prFbmSeqHdr->fgRasterOrder)
                    {
                        rScaleParamV.u4IsRsIn = 1;
                    }
                    else
                    {
                        rScaleParamV.u4IsRsIn = 0;
                    }
#else
                    rScaleParamV.u4IsRsIn = 0;
#endif
                    rScaleParamV.u4InMode = E_RZ_VDO_OUTMD_420;
                    rScaleParamV.u4YSrcBase = u4AddrY;
                    rScaleParamV.u4YSrcBufLen = prFbmPicHdr->u4PicWidthPitch;
                    rScaleParamV.u4YSrcHOffset = 0;
                    rScaleParamV.u4YSrcVOffset = 0;
                    rScaleParamV.u4YSrcW = prFbmPicHdr->u4PicWidth;
                    rScaleParamV.u4YSrcH = prFbmPicHdr->u4PicHeight;
                    rScaleParamV.u4CSrcBase = u4AddrC;
                    rScaleParamV.u4CSrcHOffset = 0;
                    rScaleParamV.u4CSrcVOffset = 0;
                    rScaleParamV.u4CSrcW = prFbmPicHdr->u4PicWidth >> 1;
                    rScaleParamV.u4CSrcH = prFbmPicHdr->u4PicHeight >> 1;
#if defined(CC_MT5395) || defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)// Force raster out for VENC
                    if(prFbmSeqHdr->fgRasterOrder)
                    {
                        rScaleParamV.u4IsRsOut = 1;
                    }
                    else
                    {
                        rScaleParamV.u4IsRsOut = 0;
                    }
#else
                    rScaleParamV.u4IsRsOut = 0;
#endif
                    rScaleParamV.u4OutMode = E_RZ_VDO_OUTMD_420;
                    rScaleParamV.u4IsVdo2Osd = 0;
                    rScaleParamV.u4YTgBase = u4YResizeAddr;
                    rScaleParamV.u4YTgCM = E_RZ_VDO_OUTMD_420;
                    rScaleParamV.u4YTgHOffset = u4Xaddr;
                    rScaleParamV.u4YTgVOffset = u4Yaddr;

                    if (prFbmSeqHdr->fgResizeSmallPic)
                    {
                        if (prFbmPicHdr->u4PicWidth < FBM_IMG_RESIZE_LIMITATION)
                        {
                            rScaleParamV.u4YTgW = FBM_IMG_RESIZE_LIMITATION;
                        }
                        else if (prFbmPicHdr->u4PicWidth > FBM_IMG_RESIZE_LIMITATION_MAX)
                        {
                            rScaleParamV.u4YTgW = FBM_IMG_RESIZE_LIMITATION_MAX;
                        }
                        else
                        {
                            rScaleParamV.u4YTgW = prFbmSeqHdr->u2HSize&(0xfff0);
                        }

                        if (prFbmPicHdr->u4PicHeight < FBM_IMG_RESIZE_LIMITATION)
                        {
                            rScaleParamV.u4YTgH = FBM_IMG_RESIZE_LIMITATION;
                        }
                        else if (prFbmPicHdr->u4PicHeight > FBM_IMG_RESIZE_LIMITATION_MAX)
                        {
                            rScaleParamV.u4YTgH = FBM_IMG_RESIZE_LIMITATION_MAX;
                        }
                        else
                        {
                            rScaleParamV.u4YTgH = prFbmSeqHdr->u2VSize;
                        }
                        rScaleParamV.u4YTgBufLen= prFbmSeqHdr->u2LineSize;
                    }
                    else
                    {
                        //rScaleParamV.u4YTgW = prFbmSeqHdr->u4ResizeWidth;
                        //rScaleParamV.u4YTgH = prFbmSeqHdr->u4ResizeHeight;

                        rScaleParamV.u4YTgW = prFbmSeqHdr->u4ResizeWidth;
                        rScaleParamV.u4YTgH = prFbmSeqHdr->u4ResizeHeight;

                        rScaleParamV.u4YTgBufLen= prFbmSeqHdr->u2LineSize;
                    }

                    rScaleParamV.u4CTgBase = (UINT32)u4CResizeAddr;
                    if(rScaleParamV.u4YTgW&0x1)
                    {
                        rScaleParamV.u4YTgW--;
                    }

                    if(rScaleParamV.u4YTgH&0x1)
                    {
                        rScaleParamV.u4YTgH--;
                    }

                    rScaleParamV.u4CTgW=rScaleParamV.u4YTgW>>1;
                    rScaleParamV.u4CTgH=rScaleParamV.u4YTgH>>1;
#if defined(CC_MT5395) || defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)// Force raster out for VENC
                    if(prFbmSeqHdr->fgRasterOrder)
                    {
                        rScaleParamV.u4OutSwap = 0;
                        rScaleParamV.u4SrcSwap = 0;
                    }
                    else
                    {
                        rScaleParamV.u4OutSwap = 6;
                        rScaleParamV.u4SrcSwap = 6;
                    }
#else
                    rScaleParamV.u4OutSwap = 6;
                    rScaleParamV.u4SrcSwap = 6;
#endif
                    //  check resize width&height is identical with source region.

                    /*VDP_GetSrcRegion(prFrcPrm->ucVdpId,&ucSrcFullRegion,&rSrcRegion);

                    if( ( rSrcRegion.u4Width != rScaleParamV.u4YTgW ) ||
                        ( rSrcRegion.u4Height != rScaleParamV.u4YTgH ) )
                    {
                        LOG(0,"resize warrning r(%d,%d), s(%d,%d) \n",
                            rScaleParamV.u4YTgW,
                            rScaleParamV.u4YTgH,
                            rSrcRegion.u4Width,
                            rSrcRegion.u4Height);

                        UNUSED(ucSrcFullRegion);
                        UNUSED(rSrcRegion);
                    }
                    */

                    //  check RPR resize address whether out of fbm pool or not.
                    u4RYSize = (prFbmSeqHdr->u4ResizeWidth*prFbmSeqHdr->u4ResizeHeight);
                    if( ( _pu1CResizeFrame2 + (u4RYSize/2) ) >= (prFbmPool->u4Addr + prFbmPool->u4Size ) )
                    {
                        LOG(0,"RPR resize address out of fbm pool!!! \n");
                    }

                    IMGRZ_Lock();
                    IMGRZ_ReInit();
                    IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
                    IMGRZ_Scale((void *)(&rScaleParamV));
                    IMGRZ_Flush();
                    IMGRZ_Wait();
                    IMGRZ_Unlock();
                }

                _VDP_HalSetAddr(prFrcPrm->ucVdpId, u4YResizeAddr, u4CResizeAddr);
#ifdef CC_3D_MM_DS_SUPPORT
                if(((_eSpeed[u1DecoderSrcId] == STC_SPEED_TYPE_FORWARD_1X) || (_eSpeed[u1DecoderSrcId] != STC_SPEED_TYPE_FORWARD_1X && prFrcPrm->u1B2R3DTrickModeType == B2R_3D_TRICK_MODE_3D_VIEW))
                        && (prFrcPrm->fgB2R3DEnable)
                        && (prFrcPrm->u1B2R3DType == B2R_3D_FRAME_SEQUENTIAL))
                {
                    _VDP_HalSetRViewAddr(prFrcPrm->ucVdpId,_u4SubFrameAddrY,_u4SubFrameAddrC);
                }
                else
                {
                    _VDP_HalSetRViewAddr(prFrcPrm->ucVdpId,u4YResizeAddr,u4CResizeAddr);
                }
#endif
            }
            else
            {
                //for VP6 sw decoder
                FBM_SEQ_HDR_T* prSeqHdr = NULL;
                UINT32 u4AddrYOffset,u4AddrCOffset;

                prSeqHdr = FBM_GetFrameBufferSeqHdr(prFrcPrm->ucFbgId);
                if(prSeqHdr != NULL)
                {
                    u4AddrYOffset = prSeqHdr->u4YAddrOffset;
                    u4AddrCOffset = prSeqHdr->u4CAddrOffset;
                }
                else
                {
                    u4AddrYOffset = 0;
                    u4AddrCOffset = 0;
                }


#ifdef CC_B2R_3D_ERROR_SUPPORT
                _u4MainFrameAddrY = u4AddrY+u4AddrYOffset;
                _u4MainFrameAddrC = u4AddrC+u4AddrCOffset;
#endif
                _VDP_HalSetAddr(prFrcPrm->ucVdpId, u4AddrY+u4AddrYOffset, u4AddrC+u4AddrCOffset);
#ifdef CC_3D_MM_DS_SUPPORT
                if(((_eSpeed[u1DecoderSrcId] == STC_SPEED_TYPE_FORWARD_1X) || (_eSpeed[u1DecoderSrcId] != STC_SPEED_TYPE_FORWARD_1X && prFrcPrm->u1B2R3DTrickModeType == B2R_3D_TRICK_MODE_3D_VIEW))
                        && (prFrcPrm->fgB2R3DEnable)
                        && (prFrcPrm->u1B2R3DType == B2R_3D_FRAME_SEQUENTIAL))
                {
                    _VDP_HalSetRViewAddr(prFrcPrm->ucVdpId,_u4SubFrameAddrY,_u4SubFrameAddrC);
                }
                else
                {
                    _VDP_HalSetRViewAddr(prFrcPrm->ucVdpId,u4AddrY+u4AddrYOffset,u4AddrC+u4AddrCOffset);
                }
#endif
            }
        }
    }
#else
    _VDP_HalSetAddr(prFrcPrm->ucVdpId, u4AddrY, u4AddrC);
#ifdef CC_3D_MM_DS_SUPPORT
    if(((_eSpeed[u1DecoderSrcId] == STC_SPEED_TYPE_FORWARD_1X) || (_eSpeed[u1DecoderSrcId] != STC_SPEED_TYPE_FORWARD_1X && prFrcPrm->u1B2R3DTrickModeType == B2R_3D_TRICK_MODE_3D_VIEW))
            && (prFrcPrm->fgB2R3DEnable)
            && (prFrcPrm->u1B2R3DType == B2R_3D_FRAME_SEQUENTIAL))
    {
        _VDP_HalSetRViewAddr(prFrcPrm->ucVdpId,_u4SubFrameAddrY,_u4SubFrameAddrC);
    }
    else
    {
        _VDP_HalSetRViewAddr(prFrcPrm->ucVdpId,u4AddrY,u4AddrC);
    }

#endif
#endif

#if 0
    /* Check whether b2r update stc or not */

    UCHAR ucAvSyncMode;
    UCHAR ucStcSrc;

    FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);

    if ( VDP_GetB2rUpdateStc() || ( AV_SYNC_FREE_RUN == ucAvSyncMode ) )
    {
        if ( prFrcPrm->ucVdpId < VDP_NS )
        {
            VDP_SetB2rStc(_VDP_GetDispingPts(prFrcPrm->ucVdpId));
            STC_SetStcValue(VDP_GetB2rStc());
        }
    }
#endif

    return;
}


//-----------------------------------------------------------------------------
/** Brief of _VdpFrmInit.
 */
//-----------------------------------------------------------------------------
static void _VdpFrmInit(UCHAR ucVdpId, UCHAR ucPort)
{
    FRC_PRM_T rVdpPrm;

    UNUSED(ucPort);

    // backup those we don't want to reset
    rVdpPrm = _arVdpPrm[ucVdpId].rPortPrm;

    if (ucVdpId >= VDP_NS)
    {
        ASSERT(ucVdpId < VDP_NS);
        return;
    }

    _u4DriftCount = 0;
    _u4MaxDriftCount = 0;
    _u4ValidCount = 0;

    _ucFrameRateUnkownCount=0x0;

#ifdef VDP_AVS_ADJUST_STC
    _VDP_SetDeltaInit(&_rB2rAvs);
#endif

    FBM_RegCbFunc(FBM_CB_FUNC_FB_READY_IND, (UINT32)_VdpFrcFrameBufferReady);

    _VdpGfxCopyFieldInit();

    FRC_MEMSET((void *)&(_arVdpPrm[ucVdpId].rPortPrm), 0, sizeof(FRC_PRM_T));

#ifdef CC_SUPPORT_TVE
    _vDrvVideoSetMute(MUTE_MODULE_DTV, ucVdpId,
                      FOREVER_MUTE, FALSE);
#else
    _VDP_HalForceBg(ucVdpId, 1);
#endif

    _arVdpPrm[ucVdpId].rPortPrm.ucVdpId = ucVdpId;

    // Output Info, can not be init again
    _arVdpPrm[ucVdpId].rPortPrm.ucOutFrameRate = _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate;
    _arVdpPrm[ucVdpId].rPortPrm.ucProgressiveOut = _prVdpConf[ucVdpId]->rOutInfo.ucPrg;
    if (_arVdpPrm[ucVdpId].rPortPrm.ucOutFrameRate)
    {
        _arVdpPrm[ucVdpId].rPortPrm.u4OutStcPeriod =
            (UINT32)(VDP_STC_CLOCK / _arVdpPrm[ucVdpId].rPortPrm.ucOutFrameRate);
    }
    else
    {
        _arVdpPrm[ucVdpId].rPortPrm.u4OutStcPeriod = (VDP_STC_CLOCK / 60);
    }

#ifdef CC_B2R_WAIT_NPTV_STABLE
    _u4WaitNPTVStableCount =0;
    _fgNPTVStable = FALSE;
#endif

    // default value
    _eSpeed[ucVdpId] = STC_SPEED_TYPE_FORWARD_1X;
    _u4PreviousResizeWidth = 0;
    _u4PreviousResizeHeight =0;

#ifdef CC_3D_MM_DS_SUPPORT
    _u1SubFrameReleadFbId = FBM_FB_ID_UNKNOWN;
    _u1SubFrameLockFbId = FBM_FB_ID_UNKNOWN;
    _u1SubFrameFbId = FBM_FB_ID_UNKNOWN;
    _u1SubFrameFbgId = FBM_FBG_ID_UNKNOWN;
#endif

    _arVdpPrm[ucVdpId].rPortPrm.ucFbgId = FBM_FBG_ID_UNKNOWN;
    _arVdpPrm[ucVdpId].rPortPrm.ucFbId = FBM_FB_ID_UNKNOWN;
    _arVdpPrm[ucVdpId].rPortPrm.ucReleaseFbId = FBM_FB_ID_UNKNOWN;
    _arVdpPrm[ucVdpId].rPortPrm.ucPendingFbId = FBM_FB_ID_UNKNOWN;
    _arVdpPrm[ucVdpId].rPortPrm.ucLastFbId = FBM_FB_ID_UNKNOWN;
    _arVdpPrm[ucVdpId].rPortPrm.fgReleaseDispQ= FALSE;

    _arVdpPrm[ucVdpId].rPortPrm.ucLastAcsFrame = ~0;
    _arVdpPrm[ucVdpId].rPortPrm.ucLastAcsAuto = ~0;

    _arVdpPrm[ucVdpId].rPortPrm.u4CbSyncEn = rVdpPrm.u4CbSyncEn;
    _arVdpPrm[ucVdpId].rPortPrm.u4CbSyncThrsd= rVdpPrm.u4CbSyncThrsd;
    _arVdpPrm[ucVdpId].rPortPrm.u4CbSyncChkFrmCnt= rVdpPrm.u4CbSyncChkFrmCnt;

    _arVdpPrm[ucVdpId].rPortPrm.pVdpPrm = &_arVdpPrm[ucVdpId];

    _arVdpPrm[ucVdpId].rPortPrm.prFbCounter = &(_prDbase->rVdp.arFbCounter[ucVdpId][0]);
    _arVdpPrm[ucVdpId].rPortPrm.u4B2rSkipFrameNo = 0;

    _rFbmAspectRatio.u1AspectRatioType = MPEG_ASPECT_RATIO_4_3;
    _rFbmAspectRatio.u2AspectHorizSize = 0;
    _rFbmAspectRatio.u2AspectVertSize =0;

#ifdef CC_3D_MM_DS_SUPPORT
    _fgEos = FALSE;
    UNUSED(_fgEos);
#endif

    FRC_MEMSET((void *)_arVdpPrm[ucVdpId].rPortPrm.prFbCounter, 0, sizeof(PARAM_VDP_FB_COUNTER_T));

#ifdef CC_B2R_ENABLE_CHG_FRAME_MSG
    _VDP_FlushB2RChgFrameMsg();
#endif

    // for _VdpCheckOutputMode, if ucPmxMode != VDP_B2R_MODE_UNKNOWN, no output change
    //[DTV00129735] DTV SCART out consideration, not to reset PMX mode
    _prVdpConf[ucVdpId]->rOutInfo.ucPmxMode = VDP_B2R_MODE_UNKNOWN;
    //LOG(1, "_prVdpConf[%d]->rOutInfo.ucPmxMode = %d;\n", ucVdpId, VDP_B2R_MODE_UNKNOWN);

#ifdef DIVX_PLUS_CER
#ifdef   CC_B2R_CHAPTER_CHG
    if(!fgFirstSetChapter)
    {
        x_memset(&_arVdpChapter,0,sizeof(VDP_CHAPTER_T));
    }
    else
    {
        fgFirstSetChapter = FALSE;
    }
#endif
#endif

    _VDP_VsyncNotify(VDP_MSG_UNMUTE_CB, (UINT32)ucVdpId, 0, 0);
}

//-----------------------------------------------------------------------------
/** Brief of _VdpChangeFrameBuffer.
 */
//-----------------------------------------------------------------------------
static void _VdpChangeFrameBuffer(FRC_PRM_T* prFrcPrm)
{
    UCHAR ucAvSyncMode;
    UCHAR ucStcSrc;
    UCHAR ucOriginalFlow = 1;
    UINT8 u1DecoderSrcId;
    UCHAR ucPlayMode;

    if (prFrcPrm == NULL)
    {
        return;
    }



    if (prFrcPrm->ucVdpId >= VDP_NS)
    {
        ASSERT(prFrcPrm->ucVdpId < VDP_NS);
        return;
    }

    if( FBM_CheckFbg(prFrcPrm->ucFbgId) )
    {
        return;
    }

    if (prFrcPrm->ucFbgId != FBM_FBG_ID_UNKNOWN)
    {
        UCHAR ucFrcMode;
        UCHAR ucOldTargetNs;
        UCHAR ucOldChangeFieldNs;

        // release last frame buffer (next ISR)
        // if we release it here, tearing will happen since this frame buffer is still displaying
        ASSERT(prFrcPrm->ucReleaseFbId == FBM_FB_ID_UNKNOWN);
        prFrcPrm->ucReleaseFbId = prFrcPrm->ucFbId;

        // backup for fail
        ucOldTargetNs = prFrcPrm->ucTargetNs;
        ucOldChangeFieldNs = prFrcPrm->ucChangeFieldNs;

        u1DecoderSrcId = FBM_GetDecoderSrcId(prFrcPrm->ucFbgId);

        if(u1DecoderSrcId >= B2R_NS)
        {
            ASSERT(0);
            return;
        }
#if 1
        if (_fgEnableWaitSTC)
        {
#ifdef ENABLE_MULTIMEDIA
            VDEC_ES_INFO_T *prVdecEsInfo;
            prVdecEsInfo = _VDEC_GetEsInfo(ES0);
#endif
            /* 20081113 MM start, STC will be updated by AUDIO but not yet updated */
            FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);
            if((prFrcPrm->ucReady == 0) &&
                    ((ucAvSyncMode == VID_SYNC_MODE_SLAVE_MM)
#ifdef ENABLE_MULTIMEDIA
                     ||
                     (prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
#endif
                    ))
            {
                UCHAR ucPreLookFbId;
                FBM_PIC_HDR_T* prPreLookNextPicHdr;
                STC_CLOCK rStcClk;

                ucPreLookFbId = _VdpPreLookNextDisp(prFrcPrm);
                if (ucPreLookFbId != FBM_FB_ID_UNKNOWN)
                {
                    if (STC_GetSrc(ucStcSrc, &rStcClk) == STC_VALID)
                    {
                        UINT32 u4Delta;

                        prPreLookNextPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, ucPreLookFbId);
                        if (prPreLookNextPicHdr != NULL)
                        {
                            if ((prPreLookNextPicHdr->u4PTS) > (rStcClk.u4Base))
                            {
                                u4Delta = ((prPreLookNextPicHdr->u4PTS) - (rStcClk.u4Base));

                                if (u4Delta >= ((UINT32)0x80000000))
                                {
                                    // handle wrap around
                                    u4Delta = (0xFFFFFFFF - u4Delta);
                                }
                            }
                            else
                            {
                                u4Delta = ((rStcClk.u4Base) - (prPreLookNextPicHdr->u4PTS));

                                if (u4Delta >= ((UINT32)0x80000000))
                                {
                                    // handle wrap around
                                    u4Delta = (0xFFFFFFFF - u4Delta);
                                }
                            }
                            LOG(3, "--- PTS(%X), STC(%X), Diff(%d), OutStcPeriod(%d)\n",
                                (prPreLookNextPicHdr->u4PTS), rStcClk.u4Base,
                                u4Delta, prFrcPrm->u4OutStcPeriod);
#if 1
                            if (u4Delta > STC_PTS_DIFF)
                            {
                                ucOriginalFlow = 0;
                            }
#endif
                        }
                    }
                }
            }
            /* 20081113 MM start, STC will be updated by AUDIO but not yet updated */
        }
#endif

        if (ucOriginalFlow == 1)
        {
            if(u1DecoderSrcId < B2R_NS)
            {
                if (((_arVdpDtvFreeze[prFrcPrm->ucVdpId].u4OnOff == 0) ||
                        (prFrcPrm->ucReady == 0)) &&
                        (_arVdpDtvFreeze[prFrcPrm->ucVdpId].u4Recovery == 0) &&
                        (_fgPauseMM[u1DecoderSrcId] == 0))
                {
                    _VdpGetNextDisp(prFrcPrm);
                }
                else
                {
                    prFrcPrm->ucTargetNs = 0;
                }
            }
            else
            {
                ASSERT(0);
            }
        }
        else
        {
            prFrcPrm->ucTargetNs = 0;
        }

        if ((prFrcPrm->ucTargetNs != 0) &&
                (prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN))
        {
            BOOL fgFastForward;
            UINT32 u4AddrY;
            UINT32 u4AddrC;


#ifdef VDP_RACE_LOG
            if (prFrcPrm->ucPicCdTp == MPEG_PIC_TYPE_B)
            {
                VDP_FrcRacingLog('b');
                VDP_FrcRacingLog('0' + prFrcPrm->ucFbId);
            }
            else
            {
                VDP_FrcRacingLog('r');
                VDP_FrcRacingLog('0' + prFrcPrm->ucFbId);
            }
#endif


#if 0
// for Test, slow motion
            prFrcPrm->ucTargetNs *= 10;
            prFrcPrm->ucChangeFieldNs *= 10;
#endif

            // update VSYNC Ns
            FRC_SET_VSYNC_NS(prFrcPrm, prFrcPrm->ucTargetNs, prFrcPrm->ucChangeFieldNs);

            if ((prFrcPrm->ucWrongField) || (prFrcPrm->ucMultipleField))
            {
                ucFrcMode = VDP_FRC_NONE;
            }
            else
            {
                ucFrcMode = VDP_FRC_NORMAL;
            }

            prFrcPrm->ucStopAuto = 0;

            if (prFrcPrm->ucNewFbArrive != 0)
            {
                prFrcPrm->ucNewFbArrive = 0;

                // Display Fail Count -1
                if (prFrcPrm->prFbCounter)
                {
                    if (prFrcPrm->prFbCounter->u4DispFail > 0)
                    {
                        prFrcPrm->prFbCounter->u4DispFail--;
                        //LOG(3, "DispFail--\n");
                    }
                    ucFrcMode = VDP_FRC_CANCEL;
                }
            }

#ifdef CC_VDP_NO_INTERLACE_FRC
            if (
#if 0 //5387
                (_prVdpConf[prFrcPrm->ucVdpId]->ucDispMode != 0) ||
#else
                (getScalerMode(prFrcPrm->ucVdpId) != 0) ||
#endif
                (_prVdpConf[prFrcPrm->ucVdpId]->ucBobMode != 0))
            {
                ucFrcMode = VDP_FRC_NONE;
            }
#endif

            /* Step Forward at 20081030 */
            if ((VDP_HalGetStepForward(u1DecoderSrcId) == VDP_PLAY_STEP_FORWARD))
            {
                if (prFrcPrm->ucRealChangeFb == 0)
                {
                    LOG(3, "STEP MM wait STC Queue 1 Frame\n");
                }
                else
                {
                    LOG(3, "STEP Real change FB PAUSE\n");
                    /* Play One Frame and then Pause */
                    VDP_SetPauseMm(prFrcPrm->ucVdpId, TRUE);

                    if (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_PSEUDO_EOS_FLAG))
                    {
                        _VDP_VsyncNotify(VDP_MSG_MM_STEP_FIN_CB, prFrcPrm->ucVdpId, 0, VDP_SEEK_STEP_NO_DATA);
                    }
                    else
                    {
                        _VDP_VsyncNotify(VDP_MSG_MM_STEP_FIN_CB, prFrcPrm->ucVdpId, 0, VDP_SEEK_STEP_OK);
                    }
                }
            }

            if(u1DecoderSrcId < B2R_NS)
            {
                if (_eSpeed[u1DecoderSrcId] <= STC_SPEED_TYPE_FORWARD_1_DOT_5X)
                {
                    fgFastForward = TRUE;
                }
                else
                {
                    fgFastForward = FALSE;
                }
            }
            else
            {
                fgFastForward = TRUE;
            }
            UNUSED(fgFastForward);

            if(!_rVdpSeek.fgABReach)
            {
                if (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_PSEUDO_EOS_FLAG))
                {
                    FBM_ClrFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_PSEUDO_EOS_FLAG);

                    if(!prFrcPrm->rAB.fgValid)   // if in AB repeat mode, no need to notify EOS
                    {

                        _VDP_VsyncNotify(VDP_MSG_MM_CB, (UINT32)prFrcPrm->ucVdpId, VDP_COND_EOS, FBM_GetDecoderSrcId(prFrcPrm->ucFbgId));
                        LOG(3, "FBM_MM_PSEUDO_EOS_FLAG Notify\n");

                    }
                    else
                    {
                        _VDP_VsyncNotify(VDP_MSG_MM_CB, (UINT32)prFrcPrm->ucVdpId, VDP_COND_RANGE_AB_DONE, fgFastForward);
                        LOG(3, "VDP_COND_RANGE_AB_DONE Notify\n");
                    }
                }
            }

            /* Trick Play at 20081001 */
            FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);

#if 1 // Display Success should not set Frame Repeat in Trick mode
            // for CR DTV00144143
            //UCHAR ucPlayMode;
            FBM_GetPlayMode(prFrcPrm->ucFbgId,&ucPlayMode);
            if( FBM_FBG_MM_MODE == ucPlayMode )
            {
                static UINT32 u4PrePTS = 0xFFFFFFFF;
                if (prFrcPrm->u4Pts == u4PrePTS)
                {
                    ucFrcMode = VDP_FRC_TRICK_PLAY;
                }
                u4PrePTS = prFrcPrm->u4Pts;
                LOG(10,"display success : Pts(%x),Fbid(%d)\n",prFrcPrm->u4Pts,prFrcPrm->ucFbId);
            }
#endif

            _VDP_HalSetFrc(prFrcPrm->ucVdpId,
                           prFrcPrm->ucTargetNs,
                           prFrcPrm->ucChangeFieldNs,
                           ucFrcMode);

            // update frame buffer addr
            FBM_GetFrameBufferAddr(prFrcPrm->ucFbgId,prFrcPrm->ucFbId, &u4AddrY, &u4AddrC);

            if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, (FBM_FLAG_RACING_HALF_B | FBM_FLAG_RACING_FULL_B)))
            {
                if (prFrcPrm->ucPicCdTp == MPEG_PIC_TYPE_B)
                {
                    _VDP_HalSetRacingMode(prFrcPrm->ucVdpId, VDP_RACING_MODE_B, 1);
                }
                else
                {
                    _VDP_HalSetRacingMode(prFrcPrm->ucVdpId, VDP_RACING_MODE_B, 0);
                }
            }

            if(FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_TRACKING_MODE_ENABLE))
            {
                if(prFrcPrm->fgTrackingMode)
                {
                    _VDP_HalSetTrackingMode(prFrcPrm->ucVdpId,TRUE);
                }
                else
                {
                    _VDP_HalSetTrackingMode(prFrcPrm->ucVdpId,FALSE);
                }
            }

#ifndef CC_B2R_ENABLE_SEAMLESS_THREAD
            _VDP_SeamlessJob(prFrcPrm);
#else
            VERIFY (x_sema_unlock(_hSeamlessSema) == OSR_OK);
#endif

#ifdef __MODEL_slt__
            SLT_CurrentPts(SLT_TYPE_VDO, prFrcPrm->u4Pts);
#endif

            prFrcPrm->u4FrameRepeated = 0;
            prFrcPrm->u4FrameSuccess++;

#ifdef CC_VDP_NO_INTERLACE_FRC
            prFrcPrm->ucDisplayFailNs = 0;
#endif
        }
        else
        {
#ifdef VDP_RACE_LOG
            VDP_FrcRacingLog('e');
#endif
            if (prFrcPrm->ucVdpId >= VDP_NS)
            {
                ASSERT(prFrcPrm->ucVdpId < VDP_NS);
                return;
            }

#ifdef B2R_AUTO_TEST
            if( !FBM_VirtualIsActive() && FBM_VirtualIsRestart() )
            {
                FBM_VirtualMpegReStart();
            }
#endif

            LOG(10,"display fail : Pts(%x),Fbid(%d)\n",prFrcPrm->u4Pts,prFrcPrm->ucLastFbId);
            // Pending New FB (MPEG) at get display Q fail, except
            // 1) not ready
            // 2) freeze
            // 3) Fail Count < 2 (當沒有訊號時, u4FrameRepeated 一直增加, 不需要 Pending New FB)
            // 4) Success >= 4 (當連續有訊號才啟動 Pending, 比較保險, 同時包含了 Fail Count < 2) (prFrcPrm->u4FrameSuccess >= 4)
            // 5) Racing Only ?!
            // 6) ...
            if ((prFrcPrm->ucReady != 0) &&
                    (_arVdpDtvFreeze[prFrcPrm->ucVdpId].u4OnOff == 0) &&
                    (prFrcPrm->ucMMWaitSTC != 1))
            {
                if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, (FBM_FLAG_TRACKING_MODE_ENABLE|FBM_FLAG_RACING_FULL_B|FBM_FLAG_RACING_HALF_B)))
                {
                    prFrcPrm->ucPendingNewFb = 1;
                }
                else if ((FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_DEBLOCKING)) &&
                         (FBM_GetFrameBufferNs(prFrcPrm->ucFbgId) <= 6))
                {
                    // 5FB Deblocking
                    prFrcPrm->ucPendingNewFb = 1;
                }
                else if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_THREE_B))
                {
                    prFrcPrm->ucPendingNewFb = 1;
                }

                prFrcPrm->ucPendingNewFb = 1;
            }

            if (prFrcPrm->ucNewFbArrive != 0)
            {
                prFrcPrm->ucNewFbArrive = 0;

                // possible
                // SS: againg test with power on/off.
//                ASSERT(0);  // possible?
            }
            if(u1DecoderSrcId < B2R_NS)
            {
                if (_arVdpDtvFreeze[prFrcPrm->ucVdpId].u4OnOff != 0)
                {
                    if (prFrcPrm->ucProgressiveSeqFlag == 0)
                    {
                        if (_arVdpDtvFreeze[prFrcPrm->ucVdpId].u4CopyField == 0)
                        {
                            _arVdpDtvFreeze[prFrcPrm->ucVdpId].u4CopyField =
                                _VdpGfxCopyFieldStart(prFrcPrm->ucFbgId, prFrcPrm->ucFbId);
                        }
                    }

                    if (_arVdpDtvFreeze[prFrcPrm->ucVdpId].u4CleanDispQ == 0)
                    {
                        FBM_ReleaseDispQ(prFrcPrm->ucFbgId);
                        _arVdpDtvFreeze[prFrcPrm->ucVdpId].u4CleanDispQ = 1;
                    }
                }
                else if (_fgPauseMM[u1DecoderSrcId] != 0)
                {
                    if (prFrcPrm->ucProgressiveSeqFlag == 0)
                    {
                        UINT32 u4CopyField;
                        u4CopyField = _VdpGfxCopyFieldStart(prFrcPrm->ucFbgId, prFrcPrm->ucFbId);

                        if (u4CopyField == 0)
                        {
                            LOG(5, "No Gfx copy field");
                        }
                    }
                }
            }
            else
            {
                ASSERT(0);
            }
            // Racing Mode
            // Current FB is already release, can not display again
            // display something else
            if ((prFrcPrm->ucReleased) &&
                    ((FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, (FBM_FLAG_TRACKING_MODE_ENABLE|FBM_FLAG_RACING_FULL_B|FBM_FLAG_RACING_HALF_B))) == 0))
            {
                UCHAR ucTempFbId;
                UINT32 u4AddrY;
                UINT32 u4AddrC;

                // Get a temp FB for display
                ucTempFbId = FBM_GetReadyFrameBuffer(prFrcPrm->ucFbgId);

                // update frame buffer addr
                FBM_GetFrameBufferAddr(prFrcPrm->ucFbgId, ucTempFbId, &u4AddrY, &u4AddrC);

                _VDP_HalSetAddr(prFrcPrm->ucVdpId, u4AddrY, u4AddrC);

                LOG(3, "Disp Fail, Temp Fb(%d)\n", ucTempFbId);
            }

            // Visual Quality Tuning
            // Handle display next fail
            // T B T B B B T B (Old)
            //         ^^^ Fail
            // T B T B T B T B (New)
            //         ^^^ Fail

            // restore
            prFrcPrm->ucReleaseFbId = FBM_FB_ID_UNKNOWN;
            prFrcPrm->ucTargetNs = ucOldTargetNs;

            //// TODO, system crash
#if 0
            prFrcPrm->ucChangeFieldNs = ucOldChangeFieldNs;

            prFrcPrm->u4FrameRepeated++;

            ucFrcMode = VDP_FRC_NORMAL;

            prFrcPrm->ucStopAuto = 0;
#endif

            prFrcPrm->u4FrameSuccess = 0;
            /*if (((prFrcPrm->u4FrameRepeated < 2) &&
                    (_fgPauseMM[u1DecoderSrcId] == 0) &&
                    (prFrcPrm->ucMMWaitSTC != 1)) || fgMainMuteOn())*/
            if (FALSE)
            {
                // reduce change to STOP AUTO

                prFrcPrm->ucChangeFieldNs = ucOldChangeFieldNs;

                prFrcPrm->u4FrameRepeated++;

                if (prFrcPrm->ucMultipleField)
                {
                    ucFrcMode = VDP_FRC_NONE;
                }
                else
                {
                    ucFrcMode = VDP_FRC_NORMAL;
                }

                prFrcPrm->ucStopAuto = 0;
            }
            else
            {
                /* MM Pause or MM pending for AVS-R also STOP AUTO */
                prFrcPrm->ucChangeFieldNs = ucOldTargetNs;

                /* 2-3 pulldown sequence. Repeat on 3-field display frame
                   To avoid B2R repeat count as odd number. */
                if ((prFrcPrm->ucWrongField) || (prFrcPrm->ucMultipleField))
                {
                    if ((prFrcPrm->ucTargetNs == 3) && (prFrcPrm->ucProgressiveOut == 0))
                    {
                        prFrcPrm->ucTargetNs = 2;
                        prFrcPrm->ucChangeFieldNs = 2;
                    }
                }

                prFrcPrm->ucMultipleField = 0;

                ucFrcMode = VDP_FRC_STOP_AUTO;

                prFrcPrm->ucStopAuto = 1;

                if (VDP_HalGetTrickMode(u1DecoderSrcId) != VDP_PLAY_TRICK)
                {
                    UINT32 u4Ret;
                    u4Ret = _VdpGfxCopyFieldStart(prFrcPrm->ucFbgId,prFrcPrm->ucFbId);
                    if (u4Ret ==0 )
                    {
                        LOG(5,"Gfx doesn't copy top field to bottom field\n");
                    }
                }
            }

            // Recovery from mpeg freeze to normal playback
            if (_arVdpDtvFreeze[prFrcPrm->ucVdpId].u4Recovery != 0)
            {
                UINT32 u4NextPts;

                _arVdpDtvFreeze[prFrcPrm->ucVdpId].u4Recovery = 0;

                u4NextPts = FBM_GetNextDispQPts(prFrcPrm->ucFbgId);

                if (u4NextPts != 0)
                {
                    FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);

#ifndef CC_VDP_FPGA
                    if (ucAvSyncMode != VID_SYNC_MODE_NONE)
                    {
                        STC_CLOCK rStcClk;

                        if (STC_GetSrc(ucStcSrc, &rStcClk) == STC_VALID)
                        {
                            INT32 i4Delta;
                            UINT32 u4Delta;

                            if (u4NextPts > rStcClk.u4Base)
                            {
                                u4Delta = (u4NextPts - rStcClk.u4Base);

                                if (u4Delta < ((UINT32)0x80000000))
                                {
                                    i4Delta = - (INT32) u4Delta;
                                }
                                else
                                {
                                    // handle wrap around
                                    u4Delta = (0xFFFFFFFF - u4Delta);
                                    i4Delta = (INT32) u4Delta;
                                }
                            }
                            else
                            {
                                u4Delta = (rStcClk.u4Base - u4NextPts);

                                if (u4Delta < ((UINT32)0x80000000))
                                {
                                    i4Delta = (INT32) u4Delta;
                                }
                                else
                                {
                                    // handle wrap around
                                    u4Delta = (0xFFFFFFFF - u4Delta);
                                    i4Delta = - (INT32) u4Delta;
                                }
                            }

                            if (i4Delta < 0)
                            {
                                // PTS slower than STC, repeat

                                UINT32 u4OutFrameStc;
                                UINT32 u4Step;

                                u4OutFrameStc = (prFrcPrm->ucTargetNs * prFrcPrm->u4OutStcPeriod);

                                if (u4OutFrameStc != 0)
                                {
                                    u4Step = (u4Delta / u4OutFrameStc);

                                    if (u4Step != 0)
                                    {
                                        prFrcPrm->ucChangeFieldNs *= u4Step;
                                        prFrcPrm->ucTargetNs *= u4Step;

                                        LOG(3, "Freeze-R %d PTS(0x%x) STC(0x%x)\n", prFrcPrm->ucTargetNs, u4NextPts, rStcClk.u4Base);
                                    }
                                }
                            }
                            else
                            {
                                // imposible !
                            }
                        }
                    }
#endif
                }
            }

#ifdef CC_VDP_NO_INTERLACE_FRC
#if 0 //5387
            if (_prVdpConf[prFrcPrm->ucVdpId]->ucDispMode != 0)
#else
            if (getScalerMode(prFrcPrm->ucVdpId) != 0)
#endif
            {
                ucFrcMode = VDP_FRC_NONE;
            }

            // Display Mode ON & Interlace Output mode
            if ((_arVdpDtvFreeze[prFrcPrm->ucVdpId].u4OnOff == 0) &&
#if 0 //5387
                    (_prVdpConf[prFrcPrm->ucVdpId]->ucDispMode != 0) &&
#else
                    (getScalerMode(prFrcPrm->ucVdpId) != 0) &&
#endif
                    (prFrcPrm->ucProgressiveOut == 0))
            {
                if (prFrcPrm->ucDisplayFailNs != VDP_DISPLAY_FAIL_FREEZE)
                {
                    prFrcPrm->ucDisplayFailNs += prFrcPrm->ucTargetNs;

                    // Fail > 1/4 Second
                    if (prFrcPrm->ucDisplayFailNs >= (prFrcPrm->ucOutFrameRate >> 2))
                    {
                        LOG(1,"Display Fail Start Gfx Copy Field\n");

                        prFrcPrm->ucDisplayFailNs = VDP_DISPLAY_FAIL_FREEZE;

                        _rVdpGfxCopyField.ucFbgId = prFrcPrm->ucFbgId;
                        _rVdpGfxCopyField.ucFbId = prFrcPrm->ucFbId;
                        // release semaphore, use gfx to copy top field to bottom field
                        VERIFY (x_sema_unlock(_rVdpGfxCopyField.hSemaphore) == OSR_OK);
                    }
                }
            }
#endif

            // update VSYNC Ns
            FRC_SET_VSYNC_NS(prFrcPrm, prFrcPrm->ucTargetNs, prFrcPrm->ucChangeFieldNs);

#if 0 /* display fail, no need to pause in step forward state */
            /* Step Forward at 20081030 */
            if ((VDP_HalGetStepForward() == VDP_PLAY_STEP_FORWARD))
            {
                /* Play One Frame and then Pause */
                VDP_SetPauseMm(prFrcPrm->ucVdpId, TRUE);
            }
#endif

#if 1
            /* Trick Play at 20081001 */
            FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);
            if ((VDP_HalGetTrickMode(u1DecoderSrcId)==VDP_PLAY_TRICK) && (ucAvSyncMode==VID_SYNC_MODE_NONE))
            {
#if 0
                if (prFrcPrm->ucPendingNewFb == 1)
                {
                    /* 4x up FF or 2x up FB will trigger Frame Repeat
                    Not to allow Late Notify since Frame Repeat is triggered */
                    prFrcPrm->ucPendingNewFb = 0;
                }
#endif
                //ucFrcMode = VDP_FRC_TRICK_PLAY;
            }




#endif

#if 0 // Display Success should not set Frame Repeat in Trick mode
            // for CR DTV00144143
            UCHAR ucPlayMode;
            FBM_GetPlayMode(prFrcPrm->ucFbgId,&ucPlayMode);
            if( FBM_FBG_MM_MODE == ucPlayMode )
            {
                static UINT32 u4PrePTS = 0xFFFFFFFF;

                if(prFrcPrm->u4Pts == u4PrePTS)
                {
                    ucFrcMode = VDP_FRC_NONE;
                }

                u4PrePTS = prFrcPrm->u4Pts;

                LOG(1,"display success : Pts(%x),Fbid(%d)\n",prFrcPrm->u4Pts,prFrcPrm->ucFbId);
            }
#endif
            _VDP_HalSetFrc(prFrcPrm->ucVdpId,
                           prFrcPrm->ucTargetNs,
                           prFrcPrm->ucChangeFieldNs,
                           ucFrcMode);

            // release current frame buffer until new buffer is ready
            prFrcPrm->ucReleaseFbId = FBM_FB_ID_UNKNOWN;
        }

        if ((prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN) && (prFrcPrm->ucTargetNs != 0))
        {
            LOG(11, "%d %d %d Ns(%d) (%d, %d) (%d %d %d)\n",
                prFrcPrm->ucVdpId, prFrcPrm->ucFbgId, prFrcPrm->ucFbId,
                prFrcPrm->ucTargetNs, prFrcPrm->u4InFrameStc, prFrcPrm->u4OutFrameStc,
                prFrcPrm->ucRff, prFrcPrm->ucTff, prFrcPrm->i4TemporalRef);

            LOG(9, "NF (%d %d) Ns(%d, %d) B(%d) TFF(%d) FRC(%d) R(%d)\n",
                prFrcPrm->ucFbId, prFrcPrm->i4TemporalRef,
                prFrcPrm->ucTargetNs, prFrcPrm->ucChangeFieldNs,
                prFrcPrm->pVdpPrm->ucBottom, prFrcPrm->ucTff,
                ucFrcMode, prFrcPrm->ucReleaseFbId);

#ifdef CC_VDP_FRC_DEBUG
            LOG(4, "NS(%d)TR(%d)FR(%d)T/RFF(%d,%d)PTS(0x%06x)DP(%d)STC(0x%06x)D(%d)\n",
                prFrcPrm->ucTargetNs,
                prFrcPrm->i4TemporalRef,
                prFrcPrm->ucInFrameRate,
                prFrcPrm->ucTff,
                prFrcPrm->ucRff,
                prFrcPrm->u4Pts,
                (prFrcPrm->u4Pts - prFrcPrm->u4PtsBackup),
                prFrcPrm->u4Stc,
                prFrcPrm->i4PtsStcDelta);

            prFrcPrm->u4PtsBackup = prFrcPrm->u4Pts;
#endif
        }
    }
}

#ifdef DIVX_PLUS_CER
#ifdef   CC_B2R_CHAPTER_CHG
VOID _VDP_FrcSetChapterEnable(VDP_CHAPTER_T*    prChapter)
{
    x_memcpy(&_arVdpChapter, prChapter, sizeof(VDP_CHAPTER_T) );
    if(_arVdpChapter.fgFirstSetChapter)
    {
        fgFirstSetChapter = TRUE;
    }
    _arVdpChapter.fgIsSetEndPts = TRUE;
    _arVdpChapter.fgPending     = FALSE;
    return;
}

static VDP_CHAPTER_T* _VDP_FrcGetChapterObj(VOID)
{
    return &_arVdpChapter;
}
#endif
#endif


//-----------------------------------------------------------------------------
/** Brief of _VdpGetNextDisp.
 */
//-----------------------------------------------------------------------------
#ifdef CC_B2R_EMULATION
static void _VdpSetCrcSequence(FRC_PRM_T* prFrcPrm)
{
    if(_u4DispSuccessIdx > _u4CheckDispSuccessIdx)
    {
        if(prFrcPrm->ucProgressiveSeqFlag) //progressive source
        {
            if(_u4InterruptCount == _au4DispSuccessIdxArray[_u4CheckDispSuccessIdx] + 2)
            {
                _u4CheckDispSuccessIdx++;

                if(_bSaveB2rCrc)
                {
                    _au4B2rCrcSave[_u4B2rCrcInx++] = _B2rHdGetCrc();
                }
                else
                {
                    _au4B2rCrcArray[_u4B2rCrcInx++] = _B2rHdGetCrc();
                    if((_au4B2rCrcArray[_u4B2rCrcInx-1] != _au4B2rCrcSave[_u4B2rCrcInx-1])
                            && _u4B2rCrcInx!=1)
                    {
                        LOG(1,"B2R CRC error idx = %d\n",(_u4B2rCrcInx-1));
                    }
                }
            }
        }
        else //interlace source
        {
            if((_u4InterruptCount == _au4DispSuccessIdxArray[_u4CheckDispSuccessIdx] + 2)
                    || (_u4InterruptCount == _au4DispSuccessIdxArray[_u4CheckDispSuccessIdx] + 3))
            {
                if(_u4InterruptCount == _au4DispSuccessIdxArray[_u4CheckDispSuccessIdx] + 3)
                {
                    _u4CheckDispSuccessIdx++;
                }

                if(_bSaveB2rCrc)
                {
                    _au4B2rCrcSave[_u4B2rCrcInx++] = _B2rHdGetCrc();
                }
                else
                {
                    _au4B2rCrcArray[_u4B2rCrcInx++] = _B2rHdGetCrc();
                    if((_au4B2rCrcArray[_u4B2rCrcInx-1] != _au4B2rCrcSave[_u4B2rCrcInx-1])
                            && _u4B2rCrcInx!=1)
                    {
                        LOG(1,"B2R CRC error idx = %d\n",(_u4B2rCrcInx-1));
                    }
                }
            }
        }

    }
}
#endif

static void _VdpSendAfdChgMsg(FBM_SEQ_HDR_T * prSeqHdr, FRC_PRM_T* prFrcPrm)
{    
    _prVdpConf[prFrcPrm->ucVdpId]->u4Afd = (UINT32) prSeqHdr->ucActFmt;
    _prVdpConf[prFrcPrm->ucVdpId]->u4AspectRatio = (UINT32) prSeqHdr->ucAspRatInf;
    _rFbmAspectRatio.u1AspectRatioType = prSeqHdr->ucAspRatInf;
    _rFbmAspectRatio.u2AspectHorizSize = prSeqHdr->u4AspectRatioWidth;
    _rFbmAspectRatio.u2AspectVertSize = prSeqHdr->u4AspectRatioHeight;

    if (prFrcPrm->ucReady ==1)
    {
        //LOG(1, "Aspect Chg %d \n",_prVdpConf[ucVdpId]->u4AspectRatio);
        Printf("Aspect Chg %d \n",_prVdpConf[prFrcPrm->ucVdpId]->u4AspectRatio);
        _VDP_VsyncNotify(VDP_MSG_AFD_CB, (UINT32)(prFrcPrm->ucVdpId), (UINT32) prSeqHdr->ucActFmt, (UINT32) (&_rFbmAspectRatio));
    }
}

static void _VdpGetNextDisp(FRC_PRM_T* prFrcPrm)
{
    UCHAR ucNoNextDisplay;
    BOOL fgGstPlayBack;
    UCHAR ucBackupFbId;
    UCHAR ucAvSyncMode;
    UCHAR ucStcSrc;
    UINT32 ui4_stc = 0x0;
    UINT8 u1DecoderSrcId;
    UINT8 u1AppMode =0;
#ifdef CC_3D_MM_DS_SUPPORT
    UINT8 u1AppModeSub =0;
#endif
    FBM_SEQ_HDR_T* prSeqHdr = NULL;
    UCHAR ucSeekMode;
    UINT32 u4SeekPts;
    UCHAR ucSeekNotifyTrigger;
#ifdef DIVX_PLUS_CER
#ifdef CC_B2R_CHAPTER_CHG
    VDP_CHAPTER_T* prChapter = _VDP_FrcGetChapterObj();
#endif
#endif

#ifdef ENABLE_MULTIMEDIA
    VDEC_ES_INFO_T* prVdecEsInfo = NULL;

#endif

    if (prFrcPrm == NULL)
    {
        return;
    }

    u1DecoderSrcId = FBM_GetDecoderSrcId(prFrcPrm->ucFbgId);
    u1AppMode = FBM_GetFbgAppMode(prFrcPrm->ucFbgId);

#ifdef ENABLE_MULTIMEDIA
    if(u1DecoderSrcId < VDEC_MAX_ES)
    {
        prVdecEsInfo = _VDEC_GetEsInfo(u1DecoderSrcId);
    }
    else
    {
        LOG(0,"u1DecoderSrcId > VDEC_MAX_ES , oops \n");
        prVdecEsInfo = _VDEC_GetEsInfo(ES0);
    }

    if(prVdecEsInfo == NULL)
    {
        LOG(0,"prVdecEsInfo == NULL, oops\n");
        return;
    }
#endif


    ucBackupFbId = prFrcPrm->ucFbId;
    prFrcPrm->ucTargetNs = 0;
    ucNoNextDisplay = 0;
    prFrcPrm->ucMMWaitSTC = 0;
    prFrcPrm->ucRealChangeFb = 0;
    fgGstPlayBack = FALSE;

    do
    {

#ifdef CC_3D_MM_DS_SUPPORT
        _VDP_GetAdditionalFbgId(prFrcPrm);
        if(_u1SubFrameFbgId != FBM_FBG_ID_UNKNOWN)
        {
            u1AppModeSub = FBM_GetFbgAppMode(_u1SubFrameFbgId);
        }
#endif

        if (prFrcPrm->ucPendingFbId != FBM_FB_ID_UNKNOWN)
        {
            /* MM may pend new FB, Can not assert */
            //ASSERT(prFrcPrm->ucReady == 0);
            prFrcPrm->ucFbId = prFrcPrm->ucPendingFbId;
            prFrcPrm->ucPendingFbId = FBM_FB_ID_UNKNOWN;
        }
        else
        {
#ifdef CC_3D_MM_DS_SUPPORT
            if(u1DecoderSrcId >= B2R_NS)
            {
                LOG(3,"WARNING u1DecoderSrcId >= B2R_NS \n");
                u1DecoderSrcId = B2R_1;
            }

            if(/*(_u1SubFrameFbgId != FBM_FBG_ID_UNKNOWN)*/
                    /*&&*/ ((_eSpeed[u1DecoderSrcId] == STC_SPEED_TYPE_FORWARD_1X) || (_eSpeed[u1DecoderSrcId] != STC_SPEED_TYPE_FORWARD_1X && prFrcPrm->u1B2R3DTrickModeType == B2R_3D_TRICK_MODE_3D_VIEW))
                    && ((u1AppMode == FBM_FBG_APP_Frame_Pack_3D) || (u1AppModeSub == FBM_FBG_APP_Frame_Pack_3D))
                    /*&& (prFrcPrm->fgB2R3DEnable)
                    && (prFrcPrm->u1B2R3DType == B2R_3D_FRAME_SEQUENTIAL)*/)
            {
                if(FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId) && FBM_CheckFrameBufferDispQ(_u1SubFrameFbgId))
                {
                    FBM_PIC_HDR_T* prLeftPicHdr = NULL;
                    FBM_PIC_HDR_T* prRightPicHdr = NULL;

                    prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                    _VDP_GetSubFrameFb(prFrcPrm,&_u4SubFrameAddrY,&_u4SubFrameAddrC);

                    prRightPicHdr = FBM_GetFrameBufferPicHdr(_u1SubFrameFbgId, _u1SubFrameFbId);
                    prLeftPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, prFrcPrm->ucFbId);

                    if (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_EOS_FLAG))
                    {
                        FBM_ClrFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_EOS_FLAG);
                        _VDP_VsyncNotify(VDP_MSG_MM_CB, (UINT32)prFrcPrm->ucVdpId, VDP_COND_EOS, FBM_GetDecoderSrcId(prFrcPrm->ucFbgId));
                        _fgEos = TRUE;
                    }

#if 1
                    if(prRightPicHdr != NULL && prLeftPicHdr!= NULL)
                    {
                        if(prRightPicHdr->i4TemporalRef != prLeftPicHdr->i4TemporalRef)
                        {
                            LOG(3,"Frame sequential 3D bitstream L/R mitmatch , L : %d , R : %d\n",prLeftPicHdr->i4TemporalRef,prRightPicHdr->i4TemporalRef);
                        }
                    }
#else
                    if((prRightPicHdr->u4PTS > prLeftPicHdr->u4PTS))
                    {
                        while((prRightPicHdr->u4PTS > prLeftPicHdr->u4PTS) && !_fgEos)
                        {
                            if(((prRightPicHdr->u4PTS - prLeftPicHdr->u4PTS) > 500) && ((prRightPicHdr->u4PTS - prLeftPicHdr->u4PTS) < 90000))
                            {
                                FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_LOCK);
                                FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_EMPTY);
                                while(!FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId) && !_fgEos)
                                {
                                    x_thread_delay(1);
                                }
                                prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                                prLeftPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, prFrcPrm->ucFbId);

                                if (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_EOS_FLAG))
                                {
                                    FBM_ClrFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_EOS_FLAG);
                                    _VDP_VsyncNotify(VDP_MSG_MM_CB, (UINT32)prFrcPrm->ucVdpId, VDP_COND_EOS, FBM_GetDecoderSrcId(prFrcPrm->ucFbgId));
                                    _fgEos = TRUE;
                                }
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    else
                    {
                        while((prLeftPicHdr->u4PTS > prRightPicHdr->u4PTS) && !_fgEos)
                        {
                            if(((prLeftPicHdr->u4PTS - prRightPicHdr->u4PTS) > 500) && ((prLeftPicHdr->u4PTS - prRightPicHdr->u4PTS) < 90000))
                            {
                                while(!FBM_CheckFrameBufferDispQ(_u1SubFrameFbgId) && !_fgEos)
                                {
                                    x_thread_delay(1);
                                }

                                _VDP_GetSubFrameFb(prFrcPrm,&_u4SubFrameAddrY,&_u4SubFrameAddrC);

                                prRightPicHdr = FBM_GetFrameBufferPicHdr(_u1SubFrameFbgId, _u1SubFrameFbId);

                                if (FBM_ChkFrameBufferPicFlag(_u1SubFrameFbgId, _u1SubFrameFbId, FBM_MM_EOS_FLAG))
                                {
                                    FBM_ClrFrameBufferPicFlag(_u1SubFrameFbgId, _u1SubFrameFbId, FBM_MM_EOS_FLAG);
                                    _VDP_VsyncNotify(VDP_MSG_MM_CB, (UINT32)prFrcPrm->ucVdpId, VDP_COND_EOS, FBM_GetDecoderSrcId(_u1SubFrameFbgId));
                                    _fgEos = TRUE;
                                }
                            }
                            else
                            {
                                break;
                            }
                        }
                    }

                    if(prRightPicHdr != NULL && prLeftPicHdr!= NULL)
                    {
                        if(prRightPicHdr->u4PTS != prLeftPicHdr->u4PTS)
                        {
                            LOG(3,"Frame sequential 3D bitstream L/R mitmatch , L : %d , R : %d\n",prLeftPicHdr->u4PTS,prRightPicHdr->u4PTS);
                        }
                    }
#endif
                }
                else
                {
#ifdef CC_B2R_3D_ERROR_SUPPORT
                    UCHAR ucTmpFbId = FBM_FB_ID_UNKNOWN;
                    if(!FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId,FBM_FLAG_3D_ERROR_HANDLE_DISABLE))
                    {
                        if((FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId) != 0)
                                && (FBM_CheckFrameBufferDispQ(_u1SubFrameFbgId) ==0))
                        {
                            ucTmpFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                            FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, ucTmpFbId, FBM_FB_STATUS_LOCK);
                            FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, ucTmpFbId, FBM_FB_STATUS_EMPTY);
                        }
                        else if((FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId) == 0)
                                && (FBM_CheckFrameBufferDispQ(_u1SubFrameFbgId) !=0))
                        {
                            ucTmpFbId = FBM_GetFrameBufferFromDispQ(_u1SubFrameFbgId);
                            FBM_SetFrameBufferStatus(_u1SubFrameFbgId, ucTmpFbId, FBM_FB_STATUS_LOCK);
                            FBM_SetFrameBufferStatus(_u1SubFrameFbgId, ucTmpFbId, FBM_FB_STATUS_EMPTY);
                        }
                        if(!prFrcPrm->fgB2R3DError && prFrcPrm->fgB2R3DEnable && !prFrcPrm->fgB2RForce2D)
                        {
                            prFrcPrm->u4B2R3DErrorCount++;
                            if(prFrcPrm->u4B2R3DErrorCount > B2R_3D_ERROR_COUNT)
                            {
                                prFrcPrm->fgB2RForce2D = TRUE;
                                prFrcPrm->u1B2RForce2DType = B2R_FORCE_2D_L_VIEW;
                                //FBM_SEQ_HDR_T* prSeqHdr;
                                //prSeqHdr = FBM_GetFrameBufferSeqHdr(prFrcPrm->ucFbgId);
                                //prSeqHdr->fgB2RForce2D = TRUE;
                                //prSeqHdr->u1B2RForce2DType = B2R_FORCE_2D_L_VIEW;
                                //FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_3D_SEQ_CHG);
                                prFrcPrm->fgB2R3DError = TRUE;
                            }
                        }
                        prFrcPrm->u4B2R3DSuccessCount = 0;
                    }
#endif
                    prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
                }
            }
            else
#endif
            {
#ifdef CC_B2R_ENABLE_CHG_FRAME_MSG

                {
                    UINT16 u2MsgQIdx;
                    SIZE_T zMsgSize;
                    VDP_B2R_CHG_FRAME_MSG_T rMsg;

                    if(u1AppMode == FBM_FBG_APP_OMX_DISP)
                    {
                        fgGstPlayBack = TRUE;

                        zMsgSize = sizeof(VDP_B2R_CHG_FRAME_MSG_T);
                        if(x_msg_q_receive(&u2MsgQIdx, &rMsg, &zMsgSize,
                        &(_hChgFrameQueue), 1, X_MSGQ_OPTION_NOWAIT) != OSR_OK)

                        {
                            prFrcPrm->ucFbId = 0xFF;
                        }
                        else
                        {
                            if(_eSpeed[u1DecoderSrcId] != STC_SPEED_TYPE_FORWARD_1X)
                            {
                                static UINT32 _u4Counter = 0;
                                if(_eSpeed[u1DecoderSrcId] == STC_SPEED_TYPE_FORWARD_2X)
                                {
                                    /*if((FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId) >= 2))
                                    {
                                        prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                                        FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_LOCK);
                                        FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_EMPTY);
                                        prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                                    }
                                    else
                                    {
                                        prFrcPrm->ucFbId = 0xFF;
                                    }*/
                                    if(_u4Counter % 2 ==0)
                                    {
                                        prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                                        if(prFrcPrm->ucFbId != 0xff)
                                        {
                                            FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_LOCK);
                                            FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_EMPTY);
                                            //prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                                            _u4Counter++;
                                        }
                                    }
                                    else
                                    {
                                        prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                                        if(prFrcPrm->ucFbId != 0xff)
                                        {
                                            _u4Counter++;
                                        }
                                    }
                                }
                                else //1.5x case
                                {
                                    if(_u4Counter % 3 ==0)
                                    {
                                        prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                                        if(prFrcPrm->ucFbId != 0xff)
                                        {
                                            FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_LOCK);
                                            FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_EMPTY);
                                            //prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                                            _u4Counter++;
                                        }
                                    }
                                    else
                                    {
                                        prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                                        if(prFrcPrm->ucFbId != 0xff)
                                        {
                                            _u4Counter++;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                            }
                        }
                    }
                    else
                    {
                        prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                    }
                }
#else
                prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
#endif
            }
            prFrcPrm->ucRealChangeFb = 1;
        }

        // vdp lock after release protection
        if (prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN)
        {
            FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_LOCK);

            if (FBM_CheckFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_LOCK) == 0)
            {
                prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
            }
        }

        if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_AFD_CHG_ONLY))
        {
            FBM_SEQ_HDR_T* prSeqHdr = FBM_GetFrameBufferSeqHdr(prFrcPrm->ucFbgId);
            if (prSeqHdr)
            {
                FBM_ClrFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_AFD_CHG_ONLY);
                _VdpSendAfdChgMsg(prSeqHdr, prFrcPrm);
            }
        }

        if (prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN)
        {
            UCHAR ucFrameEncode;
            FBM_PIC_HDR_T* prNextPicHdr;

            //FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_LOCK);

            //for DTV00320479
            prFrcPrm->fgFastForward = !(FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_BACKWARD_FLAG));

            // backup picture header info
            if (prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN)
            {
                prNextPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, prFrcPrm->ucFbId);
            }
            else
            {
                prNextPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, prFrcPrm->ucLastFbId);
            }

            if (prNextPicHdr != NULL)
            {
                FBM_PIC_HDR_T *prPic;
                BOOL fgABReach      = FALSE;
                BOOL fgFastForward;


                if(u1DecoderSrcId < B2R_NS)
                {
                    if (_eSpeed[u1DecoderSrcId] <= STC_SPEED_TYPE_FORWARD_1_DOT_5X)
                    {
                        fgFastForward = TRUE;
                    }
                    else
                    {
                        fgFastForward = FALSE;
                    }
                }
                else
                {
                    fgFastForward = TRUE;
                }

                if (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_SYNC_POINT_FLAG))
                {
                    FBM_ClrFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_SYNC_POINT_FLAG);
                    _VDP_VsyncNotify(VDP_MSG_MM_CB, (UINT32)prFrcPrm->ucVdpId, VDP_COND_SYNC_POINT, 0);
                }

                if (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_3D_CHG_FLAG))
                {
                    FBM_ClrFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_3D_CHG_FLAG);
                    FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_3D_SEQ_CHG);
                }

#if 1
                if(!_rVdpSeek.fgABReach)
                {

#if 0
                    if( VDP_HalGetStepForward(u1DecoderSrcId) == VDP_PLAY_STEP_FORWARD )
                    {
                        FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);

#ifdef CC_53XX_SWDMX_V2
                        STC_SetStcValue(ucStcSrc, prFrcPrm->u4Pts);
#else
                        STC_SetStcValue(prFrcPrm->u4Pts);
#endif

                        LOG(5," Step PTS update STC(0x%x)\n",prFrcPrm->u4Pts);
                    }
#endif

                    if (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_EOS_FLAG))
                    {

                        FBM_ClrFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_EOS_FLAG);

                        /* EOS Frame not for display, Use Last Frame for display and Not release Last Frame */
                        FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_EMPTY);
                        //prFrcPrm->ucFbId = prFrcPrm->ucLastFbId;
                        //DTV00356741
                        prFrcPrm->ucFbId = prFrcPrm->ucReleaseFbId;
                        prFrcPrm->ucReleaseFbId = FBM_FB_ID_UNKNOWN;

                        /* Seek Finish Notify at 20090110 */
                        //UCHAR ucSeekMode;
                        //UINT32 u4SeekPts;
                        //UCHAR ucSeekNotifyTrigger;
                        /* Step Forward at 20081030 */
                        if ((VDP_HalGetStepForward(u1DecoderSrcId) == VDP_PLAY_STEP_FORWARD))
                        {
                            /* Play One Frame and then Pause */
                            VDP_SetPauseMm(prFrcPrm->ucVdpId, TRUE);
                        }

                        _VDPGetSeek(&ucSeekMode, &u4SeekPts, &ucSeekNotifyTrigger);
                        if ((ucSeekMode != 0) && (ucSeekNotifyTrigger != 0))
                        {
                            /* Seek Finish notify trigger status clear */
                            _rVdpSeek.ucNotifyTrigger = 0;
                            /* Seek Finish Notification */
                            _VDP_VsyncNotify(VDP_MSG_MM_SEEK_FIN_CB, prFrcPrm->ucVdpId, u4SeekPts, VDP_SEEK_STEP_OK);
                        }

                        if(!prFrcPrm->rAB.fgValid)	 // if in AB repeat mode, no need to notify EOS
                        {
                            _VDP_VsyncNotify(VDP_MSG_MM_CB, (UINT32)prFrcPrm->ucVdpId, VDP_COND_EOS, FBM_GetDecoderSrcId(prFrcPrm->ucFbgId));
                            LOG(3, "FBM_MM_EOS_FLAG Notify\n");
                        }
                        else
                        {
                            _VDP_VsyncNotify(VDP_MSG_MM_CB, (UINT32)prFrcPrm->ucVdpId, VDP_COND_RANGE_AB_DONE, fgFastForward);
                            LOG(3, "VDP_COND_RANGE_AB_DONE Notify\n");
                        }
                        return;
                    }
                }

                prPic = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, prFrcPrm->ucFbId);
                if((prPic) && (prFrcPrm->rAB.fgValid))
                {
                    BOOL fgCheckPts = TRUE;
                    BOOL fgTempref = TRUE;

                    if(u1DecoderSrcId < B2R_NS)
                    {
                        if ((_eSpeed[u1DecoderSrcId] >= STC_SPEED_TYPE_FORWARD_1_DIV_2X) &&
                                (_eSpeed[u1DecoderSrcId] <= STC_SPEED_TYPE_FORWARD_1_DOT_5X))
                        {
                            fgTempref = FALSE;
                        }
                    }
#ifdef ENABLE_MULTIMEDIA
                    if((prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG1_DAT) ||
                            (prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_PS) ||
                            (prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_TS) ||
                            (prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_TS_192) ||
                            (prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
                            (prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
                            (prVdecEsInfo->eContainerType == SWDMX_FMT_VC1_ES) ||
                            (prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_VIDEO_ES) ||
                            (prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG4_VIDEO_ES) ||
                            (prVdecEsInfo->eContainerType == SWDMX_FMT_H264_VIDEO_ES) ||
                            /*for avs video es, pts and stc value may be error, use file position*/
                            (prVdecEsInfo->eContainerType == SWDMX_FMT_AVS_VIDEO_ES)
                      )
                    {
                        fgCheckPts = FALSE;
                    }
#endif
                    ui4_stc = _VDP_FrcGetStc(prFrcPrm->ucFbgId);

                    if(prFrcPrm->rAB.fgPosition)
                    {
                        if(fgFastForward)
                        {
                            if( ((fgCheckPts) && (prPic->u4PTS >= prFrcPrm->rAB.rB.u8Pts) &&
                                    (ui4_stc >= prFrcPrm->rAB.rB.u8Stc)) ||
                                    ((!fgCheckPts) &&((prPic->u8OffsetDisp >= prFrcPrm->rAB.rB.u8OffsetDisp) &&
                                                      ((fgTempref) ||
                                                       (prPic->i4TemporalRef == prFrcPrm->rAB.rB.i4TemporalRef)))))
                            {
                                fgABReach = TRUE;
                            }
                        }
                        else
                        {
                            if(((fgCheckPts) && (prPic->u4PTS <= prFrcPrm->rAB.rA.u8Pts)) ||
                                    ((!fgCheckPts) &&
                                     (prPic->u8OffsetDisp <= prFrcPrm->rAB.rA.u8OffsetDisp)))
                            {
                                fgABReach = TRUE;
                            }
                        }
                    }
                    else
                    {
                        if(fgFastForward)
                        {
                            if( ( (UINT64)prPic->u4PTS >= prFrcPrm->rAB.rB.u8Pts ) &&
                                    ((  ui4_stc >= prFrcPrm->rAB.rB.u8Stc) ||
                                     ((VDP_HalGetStepForward(u1DecoderSrcId) == VDP_PLAY_STEP_FORWARD)&&
                                      (prFrcPrm->rAB.rA.u8Pts != prFrcPrm->rAB.rB.u8Pts))))
                            {
                                fgABReach = TRUE;
                            }
                        }
                        else
                        {
                            if((UINT64)prPic->u4PTS <= prFrcPrm->rAB.rA.u8Pts)
                            {
                                fgABReach = TRUE;
                            }
                        }
                    }
                    if((fgABReach) && (_rVdpSeek.ucABNotifyTrigger != 0))
                    {

                        // if AB reach, driver should send SEEK DONE / STEP DONE notify mw before AB DONE notify ,
                        // It's mw flow request
                        if ((VDP_HalGetStepForward(u1DecoderSrcId) == VDP_PLAY_STEP_FORWARD))
                        {
                            if (prFrcPrm->ucRealChangeFb == 0)
                            {
                                LOG(3, "STEP MM wait STC Queue 1 Frame\n");
                            }
                            else
                            {
                                LOG(3, "STEP Real change FB PAUSE\n");
                                /* Play One Frame and then Pause */
                                VDP_SetPauseMm(prFrcPrm->ucVdpId, TRUE);

                                if (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_PSEUDO_EOS_FLAG))
                                {
                                    _VDP_VsyncNotify(VDP_MSG_MM_STEP_FIN_CB, prFrcPrm->ucVdpId, 0, VDP_SEEK_STEP_NO_DATA);
                                }
                                else
                                {
                                    _VDP_VsyncNotify(VDP_MSG_MM_STEP_FIN_CB, prFrcPrm->ucVdpId, 0, VDP_SEEK_STEP_OK);
                                }
                            }
                        }

                        /* Seek Finish Notify at 20081212 */
                        //UCHAR ucSeekMode;
                        //UINT32 u4SeekPts;
                        //UCHAR ucSeekNotifyTrigger;

                        _VDPGetSeek(&ucSeekMode, &u4SeekPts, &ucSeekNotifyTrigger);
                        if ((ucSeekMode != 0) && (ucSeekNotifyTrigger != 0) /*&&
						   (u4SeekPts <= prFrcPrm->u4Pts)*/)
                        {
                            LOG(3, "+++++ Seek trigger notify\n");
                            /* Seek Finish notify trigger status clear */
                            _rVdpSeek.ucNotifyTrigger = 0;
                            /* Seek Finish Notification */
                            if (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_PSEUDO_EOS_FLAG))
                            {
                                _VDP_VsyncNotify(VDP_MSG_MM_SEEK_FIN_CB, prFrcPrm->ucVdpId, u4SeekPts, VDP_SEEK_STEP_NO_DATA);
                                //_VDP_VsyncNotify(VDP_MSG_MM_CB, (UINT32)prFrcPrm->ucVdpId, VDP_COND_EOS, FBM_GetDecoderSrcId(prFrcPrm->ucFbgId));
                            }
                            else
                            {
                                _VDP_VsyncNotify(VDP_MSG_MM_SEEK_FIN_CB, prFrcPrm->ucVdpId, u4SeekPts, VDP_SEEK_STEP_OK);
                            }
                        }

                        _rVdpSeek.ucABNotifyTrigger = 0;
                        _rVdpSeek.fgABReach = TRUE;
                        Printf("***VDP_COND_RANGE_AB_DONE Notify****");
                        FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);
#ifdef CC_53XX_SWDMX_V2
                        STC_StopStc(ucStcSrc);
#else
                        STC_StopStc();
#endif
                        _VDP_VsyncNotify(VDP_MSG_MM_CB, (UINT32)prFrcPrm->ucVdpId, VDP_COND_RANGE_AB_DONE, fgFastForward);
                        LOG(3, "VDP_COND_RANGE_AB_DONE Notify\n");
                    }
                }
#endif
                // Check Temporal reference
                if (prFrcPrm->ucLastFbId != FBM_FB_ID_UNKNOWN)
                {
                    if ((prNextPicHdr->i4TemporalRef!= (1 + prFrcPrm->i4TemporalRef)) &&
                            (prNextPicHdr->i4TemporalRef != 0))
                    {
                        // [LOG] Discontinuous temporal reference
                        SYSLOG(VDP_PREFIX + 97, prFrcPrm->ucFbgId, prFrcPrm->i4TemporalRef, prNextPicHdr->i4TemporalRef );

                        LOG(10, "Fbg(%d) Temporal Ref %d >> %d\n"
                            , prFrcPrm->ucFbgId
                            , prFrcPrm->i4TemporalRef
                            , prNextPicHdr->i4TemporalRef);

                        if (prFrcPrm->prFbCounter)
                        {
                            prFrcPrm->prFbCounter->u4DispSkip++;
                        }
                    }
                }
#ifdef VDP_RACE_LOG
//                VDP_FrcRacingLog('0' + ((prNextPicHdr->i4TemporalRef) & 0xF));
#endif

                prFrcPrm->fgProgress2Interlace = prNextPicHdr->fgProgressive2Interlace;
                prFrcPrm->u4Pts = prNextPicHdr->u4PTS;
                prFrcPrm->i4TemporalRef = prNextPicHdr->i4TemporalRef;
                prFrcPrm->ucPicCdTp = prNextPicHdr->ucPicCdTp;
                prFrcPrm->fgTrackingMode = prNextPicHdr->fgTrackingMode;

                FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);

                prFrcPrm->ucLastProgressiveFrame = prFrcPrm->ucProgressiveFrame;
                prFrcPrm->ucProgressiveFrame = prNextPicHdr->fgProgressiveFrm;

                prFrcPrm->ucLastRff = prFrcPrm->ucRff;
                prFrcPrm->ucRff = prNextPicHdr->fgRepFirstFld;
                prFrcPrm->ucTff = prNextPicHdr->fgTopFldFirst;
                prFrcPrm->u4TotlaTimeOffset = prNextPicHdr->u4TotlaTimeOffset;
                prFrcPrm->u8FilePosition = prNextPicHdr->u8Offset;
                prFrcPrm->u8FilePositionI = prNextPicHdr->u8OffsetI;
                prFrcPrm->u4TickNum = prNextPicHdr->u4TickNum;
                prFrcPrm->u2DecodingOrder = prNextPicHdr->u2DecodingOrder;
                prFrcPrm->fgDummyFrame = prNextPicHdr->fgDummyFrame;
                prNextPicHdr->fgDummyFrame = 0;
                x_memcpy(&prFrcPrm->rExtra, &prNextPicHdr->rExtra, sizeof(VDP_PIC_EXTRA_INTO_T));

                if (prFrcPrm->ucVdpId < VDP_NS)
                {
                    _prVdpConf[prFrcPrm->ucVdpId]->u4QpAvg = prNextPicHdr->u4QpAvg;
                }

                /* VDP trigger mode + audio master to do trigger mode */
                /* For Trick to Normal or Audio language change */
#ifndef CC_B2R_WAIT_NPTV_STABLE
                if (!(VDP_HalGetStepForward(u1DecoderSrcId) == VDP_PLAY_STEP_FORWARD))
                {
                    /* Trigger Audio while not step forward state */
                    _VdpTriggerAudOn(prFrcPrm->ucVdpId, prFrcPrm->u4Pts);
                }
#endif

#ifdef CC_FORCE_32_FLAG_ERR
                if (prFrcPrm->ucValid32Seq)
                {
                    static UINT32 u4Force32Idx = 0;

                    if (++u4Force32Idx >= 16)
                    {
                        if (prFrcPrm->ucRff)
                        {
                            if (prFrcPrm->ucTff)
                            {
                                prFrcPrm->ucTff = 0;
                            }
                            else
                            {
                                prFrcPrm->ucTff = 1;
                            }
                        }
                    }

                    if (u4Force32Idx >= (16 + (PD_32_FLAG_LEAVE_LOOP_SIZE / 2)))
                    {
                        u4Force32Idx = 0;
                    }
                }
#endif
            }
            else
            {
                ASSERT(0);
            }

#if 1 // def CC_MT5360        
            //for CLI frame cropping test
#ifdef CC_CLI_TEST
            if ((_prVdpConf[prFrcPrm->ucVdpId]->u4MpegHeight == 1088) &&
                    ((_prVdpConf[prFrcPrm->ucVdpId]->u4FrmCropBtm - _prVdpConf[prFrcPrm->ucVdpId]->u4FrmCropTop) == 1080))
            {
                vDrvDISetDtvClip(prFrcPrm->ucVdpId, _prVdpConf[prFrcPrm->ucVdpId]->u4FrmCropTop);
            }
#endif
#endif

            //For CLI NoOverscan test
#ifdef CC_CLI_TEST
            {
                static UCHAR _ucNoOverScan = 0;

                if (_prVdpConf[prFrcPrm->ucVdpId]->ucNoOverScan != _ucNoOverScan)
                {
                    _ucNoOverScan = _prVdpConf[prFrcPrm->ucVdpId]->ucNoOverScan;
                    _u4ReturnValue |= VDP_EVENT_SEQ_CHG;
                }
            }
#endif
			
            // Sequence Header Change, update size, frame rate and aspect ratio
            if ((FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SEQ_CHG) ||
                    FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_3D_SEQ_CHG) ||
                    (_fgInvalidFrmRate) ||
                    (!prFrcPrm->ucSeqInfoValid)) && (prFrcPrm->ucNewFbArrive == 0))
            {

                UCHAR ucVdpId;
                UINT32 u4SeqWidth;
                UINT32 u4SeqHeight;

                prFrcPrm->ucSeqInfoValid = 1;
                ucVdpId = prFrcPrm->ucVdpId;

                if (ucVdpId >= VDP_NS)
                {
                    return;
                }

                // Prepare Input Frame Rate
                prSeqHdr = FBM_GetFrameBufferSeqHdr(prFrcPrm->ucFbgId);

                if (prSeqHdr == NULL)
                {
                    LINT_SUPPRESS_BRACE(527)

                    ASSERT(0);
                    return;
                }

                // Store MPEG Size
                _prVdpConf[ucVdpId]->u4MpegHeight = prSeqHdr->u2OrgVSize;
                _prVdpConf[ucVdpId]->u4MpegWidth = prSeqHdr->u2OrgHSize;

#if 0 /* RR mode removal */
                // Handle Half Reference Mode
                if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_REDUCE_REF))
                {
                    _VDP_HalSetExpansion(ucVdpId, prSeqHdr->fgRRYEn, prSeqHdr->fgRRCEn,prSeqHdr->fgYRatio34, prSeqHdr->fgCRatio34);
                }
                else
                {
                    _VDP_HalSetExpansion(ucVdpId, 0, 0, 0, 0);
                }
#endif

                // Handle Racing Mode
                if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_RACING_FULL_B))
                {
                    _VDP_HalSetRacingMode(ucVdpId, VDP_RACING_MODE_MBL, (UINT32)prSeqHdr->u2VSize);
                    _VDP_HalSetRacingMode(ucVdpId, VDP_RACING_MODE_ONOFF, 1);
                    _VDP_HalSetRacingMode(ucVdpId, VDP_RACING_MODE_COMPACT, 0);
                }
                else if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_RACING_HALF_B))
                {
                    _VDP_HalSetRacingMode(ucVdpId, VDP_RACING_MODE_MBL, (UINT32)prSeqHdr->u2VSize);
                    _VDP_HalSetRacingMode(ucVdpId, VDP_RACING_MODE_ONOFF, 1);
                    _VDP_HalSetRacingMode(ucVdpId, VDP_RACING_MODE_COMPACT, 1);
                }
                else
                {
                    _VDP_HalSetRacingMode(ucVdpId, VDP_RACING_MODE_ONOFF, 0);
                }

                if (prSeqHdr->ucFrmRatCod == MPEG_FRAME_RATE_UNKNOWN)
                {
                    prSeqHdr->ucFrmRatCod = FBM_CalFrmRate(prFrcPrm->ucFbgId);

                    if (prSeqHdr->ucFrmRatCod == MPEG_FRAME_RATE_UNKNOWN)
                    {
                        _fgInvalidFrmRate = TRUE;
                    }
                    else
                    {
                        _fgInvalidFrmRate = FALSE;
                    }
                }

#if 0//def CC_B2R_ENABLE_CHG_FRAME_MSG
                FBM_GetPlayMode(prFrcPrm->ucFbgId,&_u1PlayMode);
                if( FBM_FBG_MM_MODE == _u1PlayMode)
                {
                    switch(prSeqHdr->ucFrmRatCod)
                    {
                    case MPEG_FRAME_RATE_24_:
                    case MPEG_FRAME_RATE_24:
                        prSeqHdr->ucFrmRatCod = FBM_FRAME_RATE_60;
                        break;

                    case MPEG_FRAME_RATE_25:
                        prSeqHdr->ucFrmRatCod = FBM_FRAME_RATE_50;
                        break;

                    case MPEG_FRAME_RATE_30_:
                    case MPEG_FRAME_RATE_30:
                        prSeqHdr->ucFrmRatCod = FBM_FRAME_RATE_60;
                        break;

                    case MPEG_FRAME_RATE_50:
                        prSeqHdr->ucFrmRatCod = FBM_FRAME_RATE_50;
                        break;

                    case MPEG_FRAME_RATE_60_:
                    case MPEG_FRAME_RATE_60:
                        prSeqHdr->ucFrmRatCod = FBM_FRAME_RATE_60;
                        break;
                    case MPEG_FRAME_RATE_UNKNOWN:
                        prSeqHdr->ucFrmRatCod = FBM_FRAME_RATE_60;
                        break;
                    }
                }
#endif

                switch (prSeqHdr->ucFrmRatCod)
                {
                case MPEG_FRAME_RATE_24_:
                case MPEG_FRAME_RATE_24:
                    prFrcPrm->ucInFrameRate = FBM_FRAME_RATE_24;
                    break;

                case MPEG_FRAME_RATE_25:
                    prFrcPrm->ucInFrameRate = FBM_FRAME_RATE_25;
                    break;

                case MPEG_FRAME_RATE_30_:
                case MPEG_FRAME_RATE_30:
                    prFrcPrm->ucInFrameRate = FBM_FRAME_RATE_30;
                    break;

                case MPEG_FRAME_RATE_50:
                    prFrcPrm->ucInFrameRate = FBM_FRAME_RATE_50;
                    break;

                case MPEG_FRAME_RATE_60_:
                case MPEG_FRAME_RATE_60:
                    prFrcPrm->ucInFrameRate = FBM_FRAME_RATE_60;
                    break;

                case MPEG_FRAME_RATE_UNKNOWN:
                default:
                    _VdpErrNotify(1, prFrcPrm->ucVdpId, prFrcPrm->ucFbgId, prSeqHdr->ucFrmRatCod);
                    LOG(1, "Unknown Frame Rate %d\n", prSeqHdr->ucFrmRatCod);
                    prFrcPrm->ucInFrameRate = FBM_FRAME_RATE_60;
                    break;
                }

                if (prSeqHdr->ucFrmRatCod != MPEG_FRAME_RATE_UNKNOWN)
                {
                    if (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate != prSeqHdr->ucFrmRatCod)
                    {
                        _prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate = prSeqHdr->ucFrmRatCod;

                        _u4ReturnValue |= VDP_EVENT_SEQ_CHG;
                    }
                }

                // General case
                // fps    : STC Period
                // 24    : 3750
                // 25    : 3600
                // 30    : 3000
                // 50    : 1800
                // 60    : 1500
                prFrcPrm->u4InStcPeriodBase = VDP_STC_CLOCK / prFrcPrm->ucInFrameRate;

                prFrcPrm->ucProgressiveSeqFlag = prSeqHdr->fgProgressiveSeq;

                if (prSeqHdr->fgCropping && prNextPicHdr&&(!prNextPicHdr->fgSeamlessDisp))
                {
                    // If cropping info is provided, the correct video size is identified by cropping size
                    // ** For seamless mode, cropping is handled by seamless job, so don't care here
                    u4SeqWidth = prSeqHdr->u4CropWidth;
                    u4SeqHeight = prSeqHdr->u4CropHeight;
                }
                else
                {
                    u4SeqWidth = (UINT32) prSeqHdr->u2HSize;
                    u4SeqHeight = (UINT32) prSeqHdr->u2VSize;
                }

                // because of the 420 sample method
                // progressive : the height should be a multiple of 2
                // interlace : the height should be a multiple of 4
                if (prSeqHdr->fgProgressiveSeq != 0)
                {
                    u4SeqHeight &= 0xFFFFFFFE;
                }
                else
                {
                    u4SeqHeight &= 0xFFFFFFFC;
                }


#ifdef CC_B2R_3D_SUPPROT
                if(!prSeqHdr->fgB2R3DEnable)
#endif
                {
                    if (u4SeqWidth > B2R_MAX_WIDTH)
                    {
                        u4SeqWidth = B2R_MAX_WIDTH;
                    }

                    if (u4SeqHeight > B2R_MAX_HEIGHT)
                    {
                        u4SeqHeight = B2R_MAX_HEIGHT;
                    }
                }


                if ((!prFrcPrm->ucSeqInfoValid)
                        || (_prVdpConf[ucVdpId]->u4SrcLineSize != (UINT32) prSeqHdr->u2LineSize)
                        || (_prVdpConf[ucVdpId]->u4SrcWidth != u4SeqWidth)
                        || (_prVdpConf[ucVdpId]->u4SrcHeight != u4SeqHeight)
                        || (_prVdpConf[ucVdpId]->ucNoOverScan!= (UCHAR)prSeqHdr->fgNoOverScan)
#ifdef CC_B2R_3D_SUPPROT
                        || (_arVdpPrm[ucVdpId].rPortPrm.fgB2R3DEnable != prSeqHdr->fgB2R3DEnable)
                        || (_arVdpPrm[ucVdpId].rPortPrm.u1B2R3DType != prSeqHdr->u1B2R3DType)
                        || (_arVdpPrm[ucVdpId].rPortPrm.fgB2RForce2D != prSeqHdr->fgB2RForce2D)
                        || (_arVdpPrm[ucVdpId].rPortPrm.u1B2RForce2DType != prSeqHdr->u1B2RForce2DType)
                        || (_arVdpPrm[ucVdpId].rPortPrm.u4B2R3DWidthOffset != prSeqHdr->u4B2R3DWidthOffset)
                        || (_arVdpPrm[ucVdpId].rPortPrm.u4B2R3DHeightOffset != prSeqHdr->u4B2R3DHeightOffset)
                        || (_arVdpPrm[ucVdpId].rPortPrm.u1B2R3DTrickModeType != prSeqHdr->u1B2R3DTrickModeType)
#endif
                   )
                {
                    // Update Sequence Parameter to hardware
                    // do not update everything, since display mode will flash
                    _u4ReturnValue |= VDP_EVENT_SEQ_CHG;
                }
                else
                {
                    // Optional Sequence Change
                    _u4ReturnValue |= VDP_EVENT_OPT_SEQ_CHG;
                }

#ifdef CC_B2R_3D_SUPPROT

                if(prSeqHdr->fgB2R3DEnable)
                {
                    if(prSeqHdr->u1B2R3DType == B2R_3D_SIDE_BY_SIDE_REAL_D || prSeqHdr->u1B2R3DType == B2R_3D_SIDE_BY_SIDE)
                    {
                        u4SeqWidth/=2;
                    }
                    else if(prSeqHdr->u1B2R3DType == B2R_3D_TOP_N_BOTTOM)
                    {
                        u4SeqHeight/=2;
                    }
                }

                _arVdpPrm[ucVdpId].rPortPrm.fgB2RForce2D = prSeqHdr->fgB2RForce2D;
                _arVdpPrm[ucVdpId].rPortPrm.u1B2RForce2DType = prSeqHdr->u1B2RForce2DType;
                _arVdpPrm[ucVdpId].rPortPrm.fgB2R3DEnable = prSeqHdr->fgB2R3DEnable;
                _arVdpPrm[ucVdpId].rPortPrm.u1B2R3DType = prSeqHdr->u1B2R3DType;
                _arVdpPrm[ucVdpId].rPortPrm.u4B2R3DWidthOffset = prSeqHdr->u4B2R3DWidthOffset;
                _arVdpPrm[ucVdpId].rPortPrm.u4B2R3DHeightOffset = prSeqHdr->u4B2R3DHeightOffset;
                _arVdpPrm[ucVdpId].rPortPrm.u1B2R3DTrickModeType = prSeqHdr->u1B2R3DTrickModeType;
#endif
                // Update Sequence Parameter to Vdp_If
                _prVdpConf[ucVdpId]->u4SrcLineSize = prSeqHdr->u2LineSize;
                _prVdpConf[ucVdpId]->u4SrcWidth = u4SeqWidth;
                _prVdpConf[ucVdpId]->u4SrcHeight = u4SeqHeight;
                _prVdpConf[ucVdpId]->u4FrameRate = prFrcPrm->ucInFrameRate;
                _prVdpConf[ucVdpId]->ucColourPrimary = prSeqHdr->ucColourPrimaries;
                _prVdpConf[ucVdpId]->ucTransCharacter = prSeqHdr->ucTransCharacter;
                _prVdpConf[ucVdpId]->ucMatrixCoeff = prSeqHdr->ucMatrixCoeff;
                _prVdpConf[ucVdpId]->ucColorDescrip = prSeqHdr->fgColorDescrip;
                _prVdpConf[ucVdpId]->ucNoOverScan = prSeqHdr->fgNoOverScan;
                _prVdpConf[ucVdpId]->u4FrmCropTop = prSeqHdr->u4FrmCropTop;
                _prVdpConf[ucVdpId]->u4FrmCropBtm = (u4SeqHeight - prSeqHdr->u4FrmCropBtm);

#if 1 // def CC_MT5360            
                if ((_prVdpConf[ucVdpId]->u4SrcHeight == 1088) &&
                        ((_prVdpConf[ucVdpId]->u4FrmCropBtm - _prVdpConf[ucVdpId]->u4FrmCropTop) == 1080))
                {
                    vDrvDISetDtvClip(ucVdpId, _prVdpConf[ucVdpId]->u4FrmCropTop);
                }
#endif

                if ((_prVdpConf[ucVdpId]->u4Afd != (UINT32) prSeqHdr->ucActFmt) ||
                        (_prVdpConf[ucVdpId]->u4AspectRatio != (UINT32) prSeqHdr->ucAspRatInf) ||
                        (!prFrcPrm->ucSeqInfoValid))
                {
                    _VdpSendAfdChgMsg(prSeqHdr, prFrcPrm);
                }

					_VdpCheckOutputMode(prFrcPrm);

                FBM_ClrFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SEQ_CHG);
                FBM_ClrFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_3D_SEQ_CHG);

                if (prFrcPrm->ucReady == 0)
                {
                    prFrcPrm->ucHwFrameEncode = prFrcPrm->ucProgressiveFrame;
                    _VDP_HalSetChromaMode(prFrcPrm->ucVdpId, prFrcPrm->ucHwFrameEncode, prFrcPrm->ucTff);
                }

                LOG(7, "Seq Chg %d %d %d %d %d\n",
                    prSeqHdr->u2LineSize, prSeqHdr->u2HSize, prSeqHdr->u2VSize,
                    prFrcPrm->ucInFrameRate, prSeqHdr->ucAspRatInf);
                // Force update frame encode
                prFrcPrm->ucHwFrameEncode = 255;

                // set raster mode or block mode for B2R hardware
                _VDP_HalSetFormat(prFrcPrm->ucVdpId,prSeqHdr->fg422Mode,(UINT32)prSeqHdr->fgRasterOrder);
                _fgHandleSeqChg = FALSE;
            }
            else if ((FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SEQ_CHG) ||
                      FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_3D_SEQ_CHG) ||
                      (_fgInvalidFrmRate) ||
                      (!prFrcPrm->ucSeqInfoValid)) && (prFrcPrm->ucNewFbArrive == 1))
            {
                _fgHandleSeqChg = TRUE;
            }

#ifdef VDP_DEBUG_TR
            // Check Temporal reference
            if (prFrcPrm->ucLastFbId != FBM_FB_ID_UNKNOWN)
            {
                FBM_PIC_HDR_T* prLastPicHdr;

                prLastPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, prFrcPrm->ucLastFbId);

                if (prNextPicHdr != NULL)
                {
                    INT32 i4TemporalRef;

                    if (prNextPicHdr->ucPicCdTp == MPEG_PIC_TYPE_B)
                    {
                        prFrcPrm->szDebugBuf[prFrcPrm->u4DebugIdx] = 'B';
                    }
                    else if (prNextPicHdr->ucPicCdTp == MPEG_PIC_TYPE_P)
                    {
                        prFrcPrm->szDebugBuf[prFrcPrm->u4DebugIdx] = 'P';
                    }
                    else
                    {
                        prFrcPrm->szDebugBuf[prFrcPrm->u4DebugIdx] = 'I';
                    }

                    i4TemporalRef = (prNextPicHdr->i4TemporalRef & 0xF);
                    if (i4TemporalRef < 10)
                    {
                        prFrcPrm->szDebugBufTr[prFrcPrm->u4DebugIdx] = '0' + i4TemporalRef;
                    }
                    else
                    {
                        prFrcPrm->szDebugBufTr[prFrcPrm->u4DebugIdx] = 'A' + (i4TemporalRef - 10);
                    }

                    if (++prFrcPrm->u4DebugIdx >= (VDP_DEBUG_BUF_SIZE - 1))
                    {
                        prFrcPrm->u4DebugIdx = 0;
                    }
                }
            }
#endif
            // Backup Last Frame Buffer ID
            prFrcPrm->ucLastFbId = prFrcPrm->ucFbId;

            // Frame Rate Conversion
            _VdpCalculateVsyncNs(prFrcPrm);

            if((prSeqHdr)&&(_fgInvalidFrmRate)&&(_ucFrameRateUnkownCount <= 0x5))
            {
                if( ( FBM_FB_ID_UNKNOWN != prFrcPrm->ucFbId) &&
                        ( MPEG_FRAME_RATE_UNKNOWN == prSeqHdr->ucFrmRatCod ) )
                {
                    // pending for unkown frame rate
                    prFrcPrm->ucTargetNs        = 0;
                    prFrcPrm->ucChangeFieldNs   = 0;
                    prFrcPrm->u4InFrameStc      = 0;
                    prFrcPrm->u4OutFrameStc     = 0;
                    prFrcPrm->ucPendingFbId     = prFrcPrm->ucFbId;
                    prFrcPrm->ucLastFbId        = FBM_FB_ID_UNKNOWN;
                    prFrcPrm->ucFbId            = FBM_FB_ID_UNKNOWN;

                    _ucFrameRateUnkownCount++;
                    LOG(3, "pending for unkown frame rate(%d) \n",_ucFrameRateUnkownCount);
                }
            }

            // Dummy frame contain frame infomation but not to display
            if (prFrcPrm->fgDummyFrame)
            {
                // not display this frame
                prFrcPrm->ucTargetNs = 0;
                prFrcPrm->ucChangeFieldNs = 0;
            }

            if(prFrcPrm->rAB.fgValid)
            {
                if(_rVdpSeek.fgABReach)
                {
                    // not display this frame
                    prFrcPrm->ucTargetNs = 0;
                    prFrcPrm->ucChangeFieldNs = 0;
                }
            }

            if (prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN)
            {
                UCHAR ucVdpId = prFrcPrm->ucVdpId;

                if (ucVdpId >= VDP_NS)
                {
                    return;
                }

                /* prFrcPrm->ucTargetNs should be 0 if  prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN
                   for AVS-R case */
                if (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_NOT_DISPLAY_FLAG))
                {
                    FBM_ClrFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_NOT_DISPLAY_FLAG);

                    prFrcPrm->ucTargetNs = 0;
                    LOG(7, "No display (%d %d %d)\n", prFrcPrm->ucVdpId, prFrcPrm->ucFbgId, prFrcPrm->ucFbId);
                }
#ifdef DIVX_PLUS_CER
#ifdef CC_B2R_CHAPTER_CHG
                //VDP_CHAPTER_T* prChapter = _VDP_FrcGetChapterObj();

                if( prChapter->fgIsSetEndPts )
                {
                    prChapter->fgPending     = FALSE;

                    if(u1DecoderSrcId < B2R_NS)
                    {
                        if( ( _eSpeed[u1DecoderSrcId] <= STC_SPEED_TYPE_REWIND_1024X ) &&
                                ( _eSpeed[u1DecoderSrcId] >= STC_SPEED_TYPE_REWIND_1X ) )
                        {
                            if( prFrcPrm->u4Pts < prChapter->u8StartPts * 90000 || prFrcPrm->u4Pts == 0)
                            {
                                _VDP_VsyncNotify(VDP_MSG_MM_CB, (UINT32)prFrcPrm->ucVdpId, VDP_COND_CHAP_DONE, 0);
                                LOG(3, "VDP_COND_CHAP_DONE Notify (BACKWARD)\n");
                                prChapter->fgIsSetEndPts = FALSE;
                                prChapter->fgPending     = TRUE;
                            }
                            else if ( prChapter->u8StartPts == 0 && prChapter->u8EndPts == (UINT64)(-1))
                            {
                                prChapter->fgIsSetEndPts = FALSE;
                                prChapter->fgPending     = TRUE;
                            }
                        }
                        else
                        {
                            if( prFrcPrm->u4Pts >= prChapter->u8EndPts * 90000 )
                            {

                                //CR[DTV00348906], seek the charpter using the number key when pause status.
                                //Don't send the step done(Seek Done in MW) because of this charpter done to MW.
                                //Add the step done after charpter done when this case.

                                if ((VDP_HalGetStepForward(u1DecoderSrcId) == VDP_PLAY_STEP_FORWARD))
                                {
                                    if (prFrcPrm->ucRealChangeFb == 0)
                                    {
                                        LOG(3, "STEP MM wait STC Queue 1 Frame\n");
                                    }
                                    else
                                    {
                                        LOG(3, "STEP Real change FB PAUSE\n");
                                        /* Play One Frame and then Pause */
                                        VDP_SetPauseMm(prFrcPrm->ucVdpId, TRUE);

                                        if (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_PSEUDO_EOS_FLAG))
                                        {
                                            _VDP_VsyncNotify(VDP_MSG_MM_STEP_FIN_CB, prFrcPrm->ucVdpId, 0, VDP_SEEK_STEP_NO_DATA);
                                        }
                                        else
                                        {
                                            _VDP_VsyncNotify(VDP_MSG_MM_STEP_FIN_CB, prFrcPrm->ucVdpId, 0, VDP_SEEK_STEP_OK);
                                        }
                                    }
                                }

                                _VDP_VsyncNotify(VDP_MSG_MM_CB, (UINT32)prFrcPrm->ucVdpId, VDP_COND_CHAP_DONE, 0);
                                LOG(3, "VDP_COND_CHAP_DONE Notify (FORWARD)\n");

                                prChapter->fgIsSetEndPts = FALSE;
                                prChapter->fgPending     = TRUE;
                            }
                            else if ( prChapter->u8StartPts == 0 && prChapter->u8EndPts == (UINT64)(-1))
                            {
                                prChapter->fgIsSetEndPts = FALSE;
                                prChapter->fgPending     = TRUE;
                            }
                        }
                    }
                }


                if(prChapter->fgPending)
                {
                    prFrcPrm->ucTargetNs        = 0;
                    prFrcPrm->ucChangeFieldNs   = 0;
                    prFrcPrm->u4InFrameStc      = 0;
                    prFrcPrm->u4OutFrameStc     = 0;
                    prFrcPrm->ucPendingFbId     = prFrcPrm->ucFbId;
                    prFrcPrm->ucLastFbId        = FBM_FB_ID_UNKNOWN;
                    prFrcPrm->ucFbId            = FBM_FB_ID_UNKNOWN;
                }

#endif
#endif

#ifdef CC_B2R_3D_ERROR_SUPPORT
                if(!FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId,FBM_FLAG_3D_ERROR_HANDLE_DISABLE))
                {
                    if(prFrcPrm->fgB2R3DError && prFrcPrm->fgB2R3DEnable && !prFrcPrm->fgB2RForce2D)
                    {
                        prFrcPrm->u4B2R3DSuccessCount++;
                        if(prFrcPrm->u4B2R3DSuccessCount > B2R_3D_ERROR_RECOVER_COUNT)
                        {
                            FBM_SEQ_HDR_T* prSeqHdr;
                            prSeqHdr = FBM_GetFrameBufferSeqHdr(prFrcPrm->ucFbgId);
                            prSeqHdr->fgB2RForce2D = FALSE;
                            prSeqHdr->u1B2RForce2DType = B2R_FORCE_2D_L_VIEW;
                            FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_3D_SEQ_CHG);
                            prFrcPrm->fgB2R3DError = FALSE;
                        }
                    }
                    prFrcPrm->u4B2R3DErrorCount =0;
                }
#endif
                prFrcPrm->fgDispFail = FALSE;
            }
            else
            {
#ifdef CC_B2R_3D_ERROR_SUPPORT
                if(!prFrcPrm->fgB2R3DError && prFrcPrm->fgB2R3DEnable && !prFrcPrm->fgB2RForce2D)
                {
                    prFrcPrm->u4B2R3DErrorCount++;
                    if(prFrcPrm->u4B2R3DErrorCount > B2R_3D_ERROR_COUNT)
                    {
                        FBM_SEQ_HDR_T* prSeqHdr;
                        prSeqHdr = FBM_GetFrameBufferSeqHdr(prFrcPrm->ucFbgId);
                        prSeqHdr->fgB2RForce2D = TRUE;
                        prSeqHdr->u1B2RForce2DType = B2R_FORCE_2D_L_VIEW;
                        FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_3D_SEQ_CHG);
                        prFrcPrm->fgB2R3DError = TRUE;
                    }
                }
                prFrcPrm->u4B2R3DSuccessCount = 0;
#endif
                prFrcPrm->fgDispFail = TRUE;
            }

            /* Seek Finish Notify at 20081212 */
            //UCHAR ucSeekMode;
            //UINT32 u4SeekPts;
            //UCHAR ucSeekNotifyTrigger;

            _VDPGetSeek(&ucSeekMode, &u4SeekPts, &ucSeekNotifyTrigger);
            if ((ucSeekMode != 0) && (ucSeekNotifyTrigger != 0) /*&&
               (u4SeekPts <= prFrcPrm->u4Pts)*/)
            {
                LOG(3, "+++++ Seek trigger notify\n");
                /* Seek Finish notify trigger status clear */
                _rVdpSeek.ucNotifyTrigger = 0;
                /* Seek Finish Notification */
                if (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_PSEUDO_EOS_FLAG))
                {
                    _VDP_VsyncNotify(VDP_MSG_MM_SEEK_FIN_CB, prFrcPrm->ucVdpId, u4SeekPts, VDP_SEEK_STEP_NO_DATA);
                }
                else
                {
                    _VDP_VsyncNotify(VDP_MSG_MM_SEEK_FIN_CB, prFrcPrm->ucVdpId, u4SeekPts, VDP_SEEK_STEP_OK);
                }
            }

#ifdef CC_B2R_WAIT_NPTV_STABLE
            //UCHAR ucPlayMode;
            //FBM_GetPlayMode(prFrcPrm->ucFbgId,&ucPlayMode);
            //if( FBM_FBG_MM_MODE == ucPlayMode )
            {
                /* VDP trigger mode + audio master to do trigger mode */
                /* For Trick to Normal or Audio language change */
                //if(!((VDP_HalGetStepForward(u1DecoderSrcId) == VDP_PLAY_STEP_FORWARD)) && prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN)
                if(!((VDP_HalGetStepForward(u1DecoderSrcId) == VDP_PLAY_STEP_FORWARD))
                        && prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN && !_u4DrvVideoGetMute(prFrcPrm->ucVdpId))

                {
                    /* Trigger Audio while not step forward state */
                    _VdpTriggerAudOn(prFrcPrm->ucVdpId, prFrcPrm->u4Pts);
                }
            }
#endif
            // if no field change required, ucChangeFieldNs = 0
            if (prFrcPrm->ucTargetNs < prFrcPrm->ucChangeFieldNs)
            {
                LOG(5, "ucTargetNs > ucChangeFieldNs\n");
            }

            // frame/field encode
            // According to suggestion of DVD Benchmark
            if (prFrcPrm->ucProgressiveFrame)
            {
                ucFrameEncode = 1;
            }
            else if (prFrcPrm->ucLastProgressiveFrame && prFrcPrm->ucLastRff)
            {
                ucFrameEncode = 1;
            }
            else
            {
                ucFrameEncode = 0;
            }

            // frame/field encode type change
            if (prFrcPrm->ucTargetNs != 0)
            {
#ifdef CC_VDP_FORCE_FIELD_ENCODE
                if (ucFrameEncode != prFrcPrm->ucHwFrameEncode)
                {
                    prFrcPrm->ucHwFrameEncode = ucFrameEncode;
                    _VDP_HalSetChromaMode(prFrcPrm->ucVdpId, 0, 0);
                }
#else
                // 5381
                // 1) frame encode & TFF stable >> frame encode
                // 2) else field encode

                if (ucFrameEncode != 0)
                {
                    if (prFrcPrm->ucLastTff == prFrcPrm->ucTff)
                    {
                        if (prFrcPrm->ucTffStableCounter < VDP_TFF_STABLE_COUNT)
                        {
                            prFrcPrm->ucTffStableCounter++;
                        }
                    }
                    else
                    {
                        prFrcPrm->ucTffStableCounter = 0;
                    }

                    if (prFrcPrm->ucTffStableCounter < VDP_TFF_STABLE_COUNT)
                    {
                        ucFrameEncode = 0;
                    }
                }

                prFrcPrm->ucLastTff = prFrcPrm->ucTff;

                if (ucFrameEncode != prFrcPrm->ucHwFrameEncode)
                {
                    prFrcPrm->ucHwFrameEncode = ucFrameEncode;
                    _VDP_HalSetChromaMode(prFrcPrm->ucVdpId, prFrcPrm->ucHwFrameEncode, prFrcPrm->ucTff);
                }
#endif
            }

            // clear log
            if (prFrcPrm->prFbCounter)
            {
                if ((prFrcPrm->ucReady == 0) &&
                        (prFrcPrm->ucTargetNs != 0))
                {
                    prFrcPrm->prFbCounter->u4DispOk = 0;
                    prFrcPrm->prFbCounter->u4DispFail = 0;
                    prFrcPrm->prFbCounter->u4DispSkip = 0;
                    prFrcPrm->prFbCounter->u4DispRepeat = 0;
                    prFrcPrm->prFbCounter->u4DispDrop = 0;

#ifdef CHANNEL_CHANGE_LOG
                    STC_CLOCK rStcClk;
                    UCHAR ucAvSyncMode;
                    UCHAR ucStcSrc;
                    HAL_TIME_T dt;

                    FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);
                    STC_GetSrc(ucStcSrc, &rStcClk);
                    HAL_GetTime(&dt);
                    LOG(0, " %u.%06u s [AV SYNC] 8 VDP 1st Video (PTS,STC)=(0x%x,0x%x)\n", dt.u4Seconds, dt.u4Micros, prFrcPrm->u4Pts, rStcClk.u4Base);
#endif
#ifdef TIME_MEASUREMENT
                    TMS_DIFF_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME, "REC_1st_VID, B2R");
#endif
                    /* boot up measurement in release version, record once.*/
                    if (!_fgB2RProfile)
                    {
                        x_os_drv_set_timestamp("REC_1st_VID, B2R");
                        _fgB2RProfile = TRUE;
                    }
                }

                prFrcPrm->prFbCounter->u4DispOk++;
            }
            else
            {
                ASSERT(0);
            }

            if (prFrcPrm->ucTargetNs == 0)
            {
                if (prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN)
                {
                    LOG(7, "D (%d %d %d)\n",
                        prFrcPrm->ucVdpId, prFrcPrm->ucFbgId, prFrcPrm->ucFbId);

                    FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_EMPTY);
                }
                else
                {
                    ASSERT(prFrcPrm->ucPendingFbId != FBM_FB_ID_UNKNOWN);
                    ucNoNextDisplay = 1;
                    prFrcPrm->ucLastFbId = FBM_FB_ID_UNKNOWN;
                }
            }

            // [LOG] VDP get display FB success
            SYSLOG(VDP_PREFIX + prFrcPrm->ucFbgId, prFrcPrm->ucFbId, prFrcPrm->prFbCounter->u4DispOk, prFrcPrm->prFbCounter->u4DispFail);

        }
        else
        {
            UCHAR ucAvSyncMode;
            UCHAR ucStcSrc;

            prFrcPrm->fgDispFail = TRUE;

            if (_fgHandleSeqChg)
            {

                UCHAR ucVdpId;
                FBM_SEQ_HDR_T* prSeqHdr;
                UINT32 u4SeqWidth;
                UINT32 u4SeqHeight;

                prFrcPrm->ucSeqInfoValid = 1;
                ucVdpId = prFrcPrm->ucVdpId;
                if (ucVdpId >= VDP_NS)
                {
                    ASSERT(0);
                    return;
                }

                // Prepare Input Frame Rate
                prSeqHdr = FBM_GetFrameBufferSeqHdr(prFrcPrm->ucFbgId);

                if (prSeqHdr == NULL)
                {
                    LINT_SUPPRESS_BRACE(527)

                    ASSERT(0);
                    return;
                }

                // Store MPEG Size
                _prVdpConf[ucVdpId]->u4MpegHeight = prSeqHdr->u2OrgVSize;
                _prVdpConf[ucVdpId]->u4MpegWidth = prSeqHdr->u2OrgHSize;

#if 0
                // Handle Half Reference Mode
                if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_REDUCE_REF))
                {
                    _VDP_HalSetExpansion(ucVdpId, prSeqHdr->fgRRYEn, prSeqHdr->fgRRCEn,prSeqHdr->fgYRatio34, prSeqHdr->fgCRatio34);
                }
                else
                {
                    _VDP_HalSetExpansion(ucVdpId, 0, 0, 0, 0);
                }
#endif

                // Handle Racing Mode
                if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_RACING_FULL_B))
                {
                    _VDP_HalSetRacingMode(ucVdpId, VDP_RACING_MODE_MBL, (UINT32)prSeqHdr->u2VSize);
                    _VDP_HalSetRacingMode(ucVdpId, VDP_RACING_MODE_ONOFF, 1);
                    _VDP_HalSetRacingMode(ucVdpId, VDP_RACING_MODE_COMPACT, 0);
                }
                else if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_RACING_HALF_B))
                {
                    _VDP_HalSetRacingMode(ucVdpId, VDP_RACING_MODE_MBL, (UINT32)prSeqHdr->u2VSize);
                    _VDP_HalSetRacingMode(ucVdpId, VDP_RACING_MODE_ONOFF, 1);
                    _VDP_HalSetRacingMode(ucVdpId, VDP_RACING_MODE_COMPACT, 1);
                }
                else
                {
                    _VDP_HalSetRacingMode(ucVdpId, VDP_RACING_MODE_ONOFF, 0);
                }

                if (prSeqHdr->ucFrmRatCod == MPEG_FRAME_RATE_UNKNOWN)
                {
                    prSeqHdr->ucFrmRatCod = FBM_CalFrmRate(prFrcPrm->ucFbgId);

                    if (prSeqHdr->ucFrmRatCod == MPEG_FRAME_RATE_UNKNOWN)
                    {
                        _fgInvalidFrmRate = TRUE;
                    }
                    else
                    {
                        _fgInvalidFrmRate = FALSE;
                    }
                }

#if 0//def CC_B2R_ENABLE_CHG_FRAME_MSG
                FBM_GetPlayMode(prFrcPrm->ucFbgId,&_u1PlayMode);
                if( FBM_FBG_MM_MODE == _u1PlayMode)
                {
                    switch(prSeqHdr->ucFrmRatCod)
                    {
                    case MPEG_FRAME_RATE_24_:
                    case MPEG_FRAME_RATE_24:
                        prSeqHdr->ucFrmRatCod = FBM_FRAME_RATE_60;
                        break;

                    case MPEG_FRAME_RATE_25:
                        prSeqHdr->ucFrmRatCod = FBM_FRAME_RATE_50;
                        break;

                    case MPEG_FRAME_RATE_30_:
                    case MPEG_FRAME_RATE_30:
                        prSeqHdr->ucFrmRatCod = FBM_FRAME_RATE_60;
                        break;

                    case MPEG_FRAME_RATE_50:
                        prSeqHdr->ucFrmRatCod = FBM_FRAME_RATE_50;
                        break;

                    case MPEG_FRAME_RATE_60_:
                    case MPEG_FRAME_RATE_60:
                        prSeqHdr->ucFrmRatCod = FBM_FRAME_RATE_60;
                        break;
                    case MPEG_FRAME_RATE_UNKNOWN:
                        prSeqHdr->ucFrmRatCod = FBM_FRAME_RATE_60;
                        break;
                    }
                }
#endif

                switch (prSeqHdr->ucFrmRatCod)
                {
                case MPEG_FRAME_RATE_24_:
                case MPEG_FRAME_RATE_24:
                    prFrcPrm->ucInFrameRate = FBM_FRAME_RATE_24;
                    break;

                case MPEG_FRAME_RATE_25:
                    prFrcPrm->ucInFrameRate = FBM_FRAME_RATE_25;
                    break;

                case MPEG_FRAME_RATE_30_:
                case MPEG_FRAME_RATE_30:
                    prFrcPrm->ucInFrameRate = FBM_FRAME_RATE_30;
                    break;

                case MPEG_FRAME_RATE_50:
                    prFrcPrm->ucInFrameRate = FBM_FRAME_RATE_50;
                    break;

                case MPEG_FRAME_RATE_60_:
                case MPEG_FRAME_RATE_60:
                    prFrcPrm->ucInFrameRate = FBM_FRAME_RATE_60;
                    break;

                case MPEG_FRAME_RATE_UNKNOWN:
                default:
                    _VdpErrNotify(1, prFrcPrm->ucVdpId, prFrcPrm->ucFbgId, prSeqHdr->ucFrmRatCod);
                    LOG(1, "Unknown Frame Rate %d\n", prSeqHdr->ucFrmRatCod);
                    prFrcPrm->ucInFrameRate = FBM_FRAME_RATE_60;
                    break;
                }

                if (prSeqHdr->ucFrmRatCod != MPEG_FRAME_RATE_UNKNOWN)
                {
                    if (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate != prSeqHdr->ucFrmRatCod)
                    {
                        _prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate = prSeqHdr->ucFrmRatCod;

                        _u4ReturnValue |= VDP_EVENT_SEQ_CHG;
                    }
                }

                // General case
                // fps    : STC Period
                // 24    : 3750
                // 25    : 3600
                // 30    : 3000
                // 50    : 1800
                // 60    : 1500
                prFrcPrm->u4InStcPeriodBase = VDP_STC_CLOCK / prFrcPrm->ucInFrameRate;

                prFrcPrm->ucProgressiveSeqFlag = prSeqHdr->fgProgressiveSeq;

                u4SeqWidth = (UINT32) prSeqHdr->u2HSize;
                u4SeqHeight = (UINT32) prSeqHdr->u2VSize;

                // because of the 420 sample method
                // progressive : the height should be a multiple of 2
                // interlace : the height should be a multiple of 4
                if (prSeqHdr->fgProgressiveSeq != 0)
                {
                    u4SeqHeight &= 0xFFFFFFFE;
                }
                else
                {
                    u4SeqHeight &= 0xFFFFFFFC;
                }


#ifdef CC_B2R_3D_SUPPROT
                if(!prSeqHdr->fgB2R3DEnable)
#endif
                {
                    if (u4SeqWidth > B2R_MAX_WIDTH)
                    {
                        u4SeqWidth = B2R_MAX_WIDTH;
                    }

                    if (u4SeqHeight > B2R_MAX_HEIGHT)
                    {
                        u4SeqHeight = B2R_MAX_HEIGHT;
                    }
                }

                if ((!prFrcPrm->ucSeqInfoValid)
                        || (_prVdpConf[ucVdpId]->u4SrcLineSize != (UINT32) prSeqHdr->u2LineSize)
                        || (_prVdpConf[ucVdpId]->u4SrcWidth != u4SeqWidth)
                        || (_prVdpConf[ucVdpId]->u4SrcHeight != u4SeqHeight)
                        || (_prVdpConf[ucVdpId]->ucNoOverScan!= (UCHAR)prSeqHdr->fgNoOverScan)
#ifdef CC_B2R_3D_SUPPROT
                        || (_arVdpPrm[ucVdpId].rPortPrm.fgB2R3DEnable != prSeqHdr->fgB2R3DEnable)
                        || (_arVdpPrm[ucVdpId].rPortPrm.u1B2R3DType != prSeqHdr->u1B2R3DType)
                        || (_arVdpPrm[ucVdpId].rPortPrm.fgB2RForce2D != prSeqHdr->fgB2RForce2D)
                        || (_arVdpPrm[ucVdpId].rPortPrm.u1B2RForce2DType != prSeqHdr->u1B2RForce2DType)
                        || (_arVdpPrm[ucVdpId].rPortPrm.u4B2R3DWidthOffset != prSeqHdr->u4B2R3DWidthOffset)
                        || (_arVdpPrm[ucVdpId].rPortPrm.u4B2R3DHeightOffset != prSeqHdr->u4B2R3DHeightOffset)
                        || (_arVdpPrm[ucVdpId].rPortPrm.u1B2R3DTrickModeType != prSeqHdr->u1B2R3DTrickModeType)
#endif
                   )

                {
                    // Update Sequence Parameter to hardware
                    // do not update everything, since display mode will flash
                    _u4ReturnValue |= VDP_EVENT_SEQ_CHG;
                }
                else
                {
                    // Optional Sequence Change
                    _u4ReturnValue |= VDP_EVENT_OPT_SEQ_CHG;
                }

#ifdef CC_B2R_3D_SUPPROT
                if(prSeqHdr->fgB2R3DEnable)
                {
                    if(prSeqHdr->u1B2R3DType == B2R_3D_SIDE_BY_SIDE_REAL_D || prSeqHdr->u1B2R3DType == B2R_3D_SIDE_BY_SIDE)
                    {
                        u4SeqWidth/=2;
                    }
                    else if(prSeqHdr->u1B2R3DType == B2R_3D_TOP_N_BOTTOM)
                    {
                        u4SeqHeight/=2;
                    }
                }

                _arVdpPrm[ucVdpId].rPortPrm.fgB2RForce2D = prSeqHdr->fgB2RForce2D;
                _arVdpPrm[ucVdpId].rPortPrm.u1B2RForce2DType = prSeqHdr->u1B2RForce2DType;
                _arVdpPrm[ucVdpId].rPortPrm.fgB2R3DEnable = prSeqHdr->fgB2R3DEnable;
                _arVdpPrm[ucVdpId].rPortPrm.u1B2R3DType = prSeqHdr->u1B2R3DType;
                _arVdpPrm[ucVdpId].rPortPrm.u4B2R3DWidthOffset = prSeqHdr->u4B2R3DWidthOffset;
                _arVdpPrm[ucVdpId].rPortPrm.u4B2R3DHeightOffset = prSeqHdr->u4B2R3DHeightOffset;
                _arVdpPrm[ucVdpId].rPortPrm.u1B2R3DTrickModeType = prSeqHdr->u1B2R3DTrickModeType;
#endif
                // Update Sequence Parameter to Vdp_If
                _prVdpConf[ucVdpId]->u4SrcLineSize = prSeqHdr->u2LineSize;
                _prVdpConf[ucVdpId]->u4SrcWidth = u4SeqWidth;
                _prVdpConf[ucVdpId]->u4SrcHeight = u4SeqHeight;
                _prVdpConf[ucVdpId]->u4FrameRate = prFrcPrm->ucInFrameRate;
                _prVdpConf[ucVdpId]->ucColourPrimary = prSeqHdr->ucColourPrimaries;
                _prVdpConf[ucVdpId]->ucTransCharacter = prSeqHdr->ucTransCharacter;
                _prVdpConf[ucVdpId]->ucMatrixCoeff = prSeqHdr->ucMatrixCoeff;
                _prVdpConf[ucVdpId]->ucColorDescrip = prSeqHdr->fgColorDescrip;
                _prVdpConf[ucVdpId]->ucNoOverScan = prSeqHdr->fgNoOverScan;
                _prVdpConf[ucVdpId]->u4FrmCropTop = prSeqHdr->u4FrmCropTop;
                _prVdpConf[ucVdpId]->u4FrmCropBtm = (u4SeqHeight - prSeqHdr->u4FrmCropBtm);

#if 1 // def CC_MT5360            
                if ((_prVdpConf[ucVdpId]->u4SrcHeight == 1088) &&
                        ((_prVdpConf[ucVdpId]->u4FrmCropBtm - _prVdpConf[ucVdpId]->u4FrmCropTop) == 1080))
                {
                    vDrvDISetDtvClip(ucVdpId, _prVdpConf[ucVdpId]->u4FrmCropTop);
                }
#endif

                if ((_prVdpConf[ucVdpId]->u4Afd != (UINT32) prSeqHdr->ucActFmt) ||
                        (_prVdpConf[ucVdpId]->u4AspectRatio != (UINT32) prSeqHdr->ucAspRatInf) ||
                        (!prFrcPrm->ucSeqInfoValid))
                {
                    _VdpSendAfdChgMsg(prSeqHdr, prFrcPrm);
#ifdef CC_B2R_WAIT_NPTV_STABLE
                    _u4WaitNPTVStableCount = 0;
#endif
                }
				
					_VdpCheckOutputMode(prFrcPrm);

                FBM_ClrFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SEQ_CHG);
                FBM_ClrFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_3D_SEQ_CHG);

                if (prFrcPrm->ucReady == 0)
                {
                    prFrcPrm->ucHwFrameEncode = prFrcPrm->ucProgressiveFrame;
                    _VDP_HalSetChromaMode(prFrcPrm->ucVdpId, prFrcPrm->ucHwFrameEncode, prFrcPrm->ucTff);
                }

                LOG(7, "Seq Chg %d %d %d %d %d\n",
                    prSeqHdr->u2LineSize, prSeqHdr->u2HSize, prSeqHdr->u2VSize,
                    prFrcPrm->ucInFrameRate, prSeqHdr->ucAspRatInf);
                // Force update frame encode
                prFrcPrm->ucHwFrameEncode = 255;

                // set raster mode or block mode for B2R hardware
                _VDP_HalSetFormat(prFrcPrm->ucVdpId,prSeqHdr->fg422Mode,(UINT32)prSeqHdr->fgRasterOrder);

                _fgHandleSeqChg = FALSE;
            }

#ifndef VDP_B2R_PSCAN_CRC_FLOW
            // ask mpeg decode to skip one picture due to repeat, if AV Sync NOT enable
            FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);
            if (ucAvSyncMode == VID_SYNC_MODE_NONE )
            {
//#ifdef ENABLE_MULTIMEDIA
//                if(prVdecEsInfo->eMMSrcType != SWDMX_SRC_TYPE_NETWORK_SKYPE)
//#endif
                if((u1AppMode != FBM_FBG_APP_OMX_DISP) && (u1AppMode != FBM_FBG_APP_SKYPE))
                {
                    FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SKIP_PIC);
                }
            }
#endif
            ucNoNextDisplay = 1;

            if (prFrcPrm->prFbCounter)
            {
                prFrcPrm->prFbCounter->u4DispFail++;
                //LOG(3, "DispFail++\n");
            }
            else
            {
                ASSERT(0);
            }

#ifdef SYSTEM_LOG
            if (prFrcPrm->ucReady)
            {
                // [LOG] VDP get display FB fail
                SYSLOG(VDP_PREFIX + 98, prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4DispOk, prFrcPrm->prFbCounter->u4DispFail);
            }
#endif
        }
    } while ((prFrcPrm->ucTargetNs == 0) && (ucNoNextDisplay == 0) && !prFrcPrm->fgDummyFrame && !fgGstPlayBack);

    if (prFrcPrm->ucTargetNs == 0)
    {
        prFrcPrm->ucFbId = ucBackupFbId;
    }
}

//-----------------------------------------------------------------------------
/** Brief of _VdpCalculateVsyncNs.
 * reference 8105 vsv_sync.c (fgVsvCalculateVsyncNs)
 * Description : Calculate Ns at Normal mode and Trick mode
 *        (1) Calculate Base Vsync Ns
 *        (2) Normal mode: (A) Farme Rate Conversion (B) AV SYNC
 *        (3) Trick mode: Multiply Vsync Order (according to Trick speed)
 * VSync NS: Combine, VSync, CCNs, InverseField ?
 */
//-----------------------------------------------------------------------------
static void _VdpCalculateVsyncNs(FRC_PRM_T* prFrcPrm)
{
    if (prFrcPrm == NULL)
    {
        return;
    }

    _VdpBaseVSyncNs(prFrcPrm);

    _VdpFrameRateProc(prFrcPrm);

    _VdpAVSyncProc(prFrcPrm);

#ifdef CC_FRC_TEST
    _VdpFrcTest(prFrcPrm);
#endif
}

//-----------------------------------------------------------------------------
/** Brief of _VdpChk32Flag.
 * reference 8105 vsv_sync.c (vVsvChk32Flag)
 */
//-----------------------------------------------------------------------------
static void _VdpChk32Flag(FRC_PRM_T* prFrcPrm)
{
#ifdef VDP_HANDLE_32FLAG
    UCHAR ucLast32Seq;

    if (prFrcPrm == NULL)
    {
        return;
    }

    ucLast32Seq = prFrcPrm->ucValid32Seq;

    // check the following 32 flag sequence
    // TFF Sequence    : 1 0 0 1 1 0 0 1 ...
    // RFF Sequence    : 1 0 1 0 1 0 1 0 ...

    if (prFrcPrm->ucValid32Seq == 0)
    {
        UINT32 u4Step;

        u4Step = prFrcPrm->ucPd32SeqStep & (PD_32_FLAG_SEQ - 1);

        if ((prFrcPrm->ucRff == _au4Pd32FlagCadence[0][u4Step]) &&
                (prFrcPrm->ucTff == _au4Pd32FlagCadence[1][u4Step]))
        {
            // valid cadence

            if (++(prFrcPrm->ucPd32SeqStep) >= PD_32_FLAG_LOOP_SIZE)
            {
                prFrcPrm->ucValid32Seq = 1;
                prFrcPrm->ucRelease32Cnt = 0;
            }
        }
        else
        {
            prFrcPrm->ucPd32SeqStep = GUESS_32_STEP(prFrcPrm->ucTff, prFrcPrm->ucRff);
        }
    }
    else
    {
        UINT32 u4Step;

        u4Step = (prFrcPrm->ucPd32SeqStep++) & (PD_32_FLAG_SEQ - 1);

        if ((prFrcPrm->ucRff == _au4Pd32FlagCadence[0][u4Step]) &&
                (prFrcPrm->ucTff == _au4Pd32FlagCadence[1][u4Step]))
        {
            prFrcPrm->ucRelease32Cnt = 0;
        }
        else
        {
            // invalid cadence

//            LOG(1, "Invalid 32 (%d %d %d) (%d)\n",
//                prFrcPrm->ucPd32SeqStep,
//                prFrcPrm->ucTff,
//                prFrcPrm->ucRff,
//                prFrcPrm->ucRelease32Cnt);

            if (++(prFrcPrm->ucRelease32Cnt) >= PD_32_FLAG_LEAVE_LOOP_SIZE)
            {
                prFrcPrm->ucValid32Seq = 0;
                prFrcPrm->ucPd32SeqStep = 0;
            }
            else
            {
                // guess next cadence
                prFrcPrm->ucPd32SeqStep = GUESS_32_STEP(prFrcPrm->ucTff, prFrcPrm->ucRff);
            }
        }
    }

    if (ucLast32Seq != prFrcPrm->ucValid32Seq)
    {
        LOG(3, "32 Flag %d >> %d\n", ucLast32Seq, prFrcPrm->ucValid32Seq);
    }
#else

    // ignore 32 flag in MT5371 project

#ifdef VDP_TEST_32FLAG
    do
    {
        static UINT32 u4Test32Flag = 0;

        if (++u4Test32Flag >= 64)
        {
            u4Test32Flag = 0;

            if (prFrcPrm->ucValid32Seq == 0)
            {
                prFrcPrm->ucValid32Seq = 1;
            }
            else
            {
                prFrcPrm->ucValid32Seq = 0;
            }

            LOG(3, "32 Flag >> %d\n", prFrcPrm->ucValid32Seq);
        }
    }
    while (0);
#else
    UNUSED(prFrcPrm);
#endif

#endif
}

//-----------------------------------------------------------------------------
/** Brief of _VdpBaseVSyncNs.
 * reference 8105 vsv_sync.c (fgVsvBaseVSyncNs)
 * Description : Get Base VSync Ns according to RFF, TFF, and PScan state
 *        (1) VSyncNs: duration to display the frame
 *        (2) CCNs:0 Field numbers for Close Caption
 *        (3) CombineNs: duration to combine with previous frame when in PScan output
 */
//-----------------------------------------------------------------------------
static void _VdpBaseVSyncNs(FRC_PRM_T* prFrcPrm)
{
#ifdef VDP_MONITOR_TFF
    static UCHAR ucLastTff = 1;
#endif

    if (prFrcPrm == NULL)
    {
        return;
    }

    _VdpChk32Flag(prFrcPrm);

#ifdef VDP_MONITOR_TFF
    if ((prFrcPrm->ucValid32Seq == 0) &&
            (ucLastTff != prFrcPrm->ucTff))
    {
        LOG(1, "TFF %d >> %d (RFF %d %d)\n"
            , ucLastTff, prFrcPrm->ucTff
            , prFrcPrm->ucLastRff, prFrcPrm->ucRff);
    }
    ucLastTff = prFrcPrm->ucTff;
#endif

    prFrcPrm->u4InStcPeriod = prFrcPrm->u4InStcPeriodBase;

    prFrcPrm->ucMultipleField = 0;

    // 20080209 William: SGP, StickyVideo2.trp
    // Handle RFF, we should not consider frame rate
    if (prFrcPrm->ucProgressiveSeqFlag)        // Progressive Sequence
    {
        if (prFrcPrm->ucProgressiveFrame == 0)  // Progressive Frame
        {
            // Progressive Sequence with Interlace Frame ?! (Bad Edit?)
            if (prFrcPrm->ucRff)
            {
                // According to 13818-2, If Progressive Frame = 0, RFF shall be 0.
                LOG(3, "Warning! Progressive Frame = 0 and RFF = 1\n");
            }
        }

        // Acoording to 13818-2, top_field_first section

        if ((prFrcPrm->ucRff) && (prFrcPrm->ucTff))
        {
            // 3 Frame for PRGS_SEQ + REPEAT_1ST_Fld + TOP_FLS_FIRST
            prFrcPrm->u4InStcPeriod *= 3;

            if (!prFrcPrm->ucProgressiveOut)
            {
                // although according to spec, it should play 6 vsync
                // but we only encounter streams that will error if we play 6 vsync, but will correct if we play 3 vsync,
                // this is the stream error, but we modify our code to cover the stream error

                prFrcPrm->u4InStcPeriod /= 2;
            }

            prFrcPrm->ucMultipleField = 1;
        }
        else if (prFrcPrm->ucRff)
        {
            // 2 Frame for PRGS_SEQ + REPEAT_1ST_Fld
            prFrcPrm->u4InStcPeriod *= 2;

            if (!prFrcPrm->ucProgressiveOut)
            {
                // although according to spec, it should play 4 vsync
                // but we only encounter streams that will error if we play 4 vsync, but will correct if we play 2 vsync,
                // this is the stream error, but we modify our code to cover the stream error

                prFrcPrm->u4InStcPeriod /= 2;
            }

            prFrcPrm->ucMultipleField = 1;
        }
        else
        {
            // 1 Frame for PRGS_SEQ
        }
    }
    else                                // Interlace Sequence
    {
        // According to 13818-2, repeat_first_field section
        if (prFrcPrm->ucRff)
        {
            if (prFrcPrm->ucProgressiveFrame == 0)  // Progressive Frame
            {
                // According to 13818-2, If Progressive Frame = 0, RFF shall be 0.
                LOG(3, "Warning! Progressive Frame = 0 and RFF = 1\n");
            }

            prFrcPrm->u4InStcPeriod *= 3;
            prFrcPrm->u4InStcPeriod /= 2;

            prFrcPrm->ucMultipleField = 1;
        }
    }
}

//-----------------------------------------------------------------------------
/** Brief of _VdpFrameRateProc.
 * reference 8105 vsv_sync.c (fgVsvFrameRateProc)
 * Frame Rate Convert Procedure
 * PS_FILM_XX, first number: target frame_no, 2nd number: combine frame_no
 */
//-----------------------------------------------------------------------------
static void _VdpFrameRateProc(FRC_PRM_T* prFrcPrm)
{
    UINT32 u4InStcPeriod;
    UINT32 u4OutStcPeriod;
    UINT32 u4OutVSyncNs;
    UINT32 u4InFrameStc;
    UINT32 u4OutFrameStc;

    UINT32 u4StcDiffVSyncNs;

    if (prFrcPrm == NULL)
    {
        return;
    }

    u4InStcPeriod = prFrcPrm->u4InStcPeriod;
    u4OutStcPeriod = prFrcPrm->u4OutStcPeriod;

    if (u4OutStcPeriod == 0)
    {
        return;
    }

    u4OutVSyncNs = u4InStcPeriod / u4OutStcPeriod;

    u4InFrameStc = prFrcPrm->u4InFrameStc + u4InStcPeriod;
    u4OutFrameStc = prFrcPrm->u4OutFrameStc + (u4OutStcPeriod * u4OutVSyncNs);

    if (u4InFrameStc >= u4OutFrameStc)    // Check to see if we need to repeat?
    {
        u4StcDiffVSyncNs = (u4InFrameStc - u4OutFrameStc) / u4OutStcPeriod;

        if (u4StcDiffVSyncNs > 0)
        {
            if ((prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_30) ||
                    (prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_25))        // Interlace Source
            {
                if (prFrcPrm->ucValid32Seq)    // 32 flag sequence valid, repeat at NOT RFF
                {
                    if (!prFrcPrm->ucRff)
                    {
                        // 一次到位的做法
                        u4OutVSyncNs += u4StcDiffVSyncNs;
                    }
                }
                else    // Progressive/Interlace Output
                {
                    // 一次到位的做法
                    u4OutVSyncNs += u4StcDiffVSyncNs;
                }
            }
            else                                                    // Progressive Source
            {
                // 一次到位的做法
                u4OutVSyncNs += u4StcDiffVSyncNs;
            }
        }
    }
    else                                // Check to see if we need to skip?
    {
        // It is possible
        // At 30 >> 25 FRC case, we will output 2 2 1 2 2 1 ...
        // When this case integrate will deinterlace, we add a constraint "Don't skip 1st B's field (IBBPBB...)"
        // When we change back to 30, we will have (u4InFrameStc < u4OutFrameStc)

        LOG(3, "FRC OutFrameStc(%d) > InFrameStc(%d) \n", prFrcPrm->u4OutFrameStc, prFrcPrm->u4InFrameStc);

        while ((u4OutVSyncNs) && (u4OutFrameStc > u4InFrameStc))
        {
            u4OutFrameStc -= u4OutStcPeriod;
            u4OutVSyncNs--;
        }
    }

    //LOG(3,"u4OutVSyncNs = %d u4InStcPeriod = %d u4OutStcPeriod = %d u4InFrameStc = %d u4OutFrameStc = %d\n", u4OutVSyncNs,u4InStcPeriod,u4OutStcPeriod,u4InFrameStc,u4OutFrameStc);

    if(_fgTimeShift[0] && (_i4TimeShiftSpeed[0] == 200 || _i4TimeShiftSpeed[0] == 50))
    {
        if(_i4TimeShiftSpeed[0] == 200)
        {
            prFrcPrm->ucTargetNs = u4OutVSyncNs / 2;
        }
        else if(_i4TimeShiftSpeed[0] == 50)
        {
            prFrcPrm->ucTargetNs = u4OutVSyncNs * 2;
        }
    }
    else
    {
        prFrcPrm->ucTargetNs = u4OutVSyncNs;
    }


    prFrcPrm->ucChangeFieldNs = prFrcPrm->ucTargetNs;

    // Field Procedure
    // MPEG1 (Progressive Sequence = 1) 暫時算是 Progressive Source
    prFrcPrm->ucInterlace2Interlace = 0;
    if (prFrcPrm->ucProgressiveOut)        // Progressive output
    {
        // Different with MT5351
        // I2P or P2P
        prFrcPrm->ucChangeFieldNs = 0;
    }
    else                                // interlace output
    {
        // Different with MT5351
        // P2I or I2I

        prFrcPrm->ucWrongField = 0;

        if ((prFrcPrm->ucProgressiveSeqFlag) ||
                (prFrcPrm->ucValid32Seq))
        {
            // P2I, Top field first is don't care
        }
        else
        {
            if ((prFrcPrm->pVdpPrm->ucBottom == prFrcPrm->ucTff) &&
                    (prFrcPrm->ucStopAuto == 0) &&
                    ((prFrcPrm->ucLastRff == 0) && (prFrcPrm->ucRff == 0)))
            {

                if ((((prFrcPrm->ucInFrameRate ==FBM_FRAME_RATE_30) ||
                        (prFrcPrm->ucInFrameRate ==FBM_FRAME_RATE_60)) &&
                        (prFrcPrm->ucOutFrameRate ==FBM_FRAME_RATE_60)) ||
                        (((prFrcPrm->ucInFrameRate ==FBM_FRAME_RATE_25) ||
                          (prFrcPrm->ucInFrameRate ==FBM_FRAME_RATE_50)) &&
                         (prFrcPrm->ucOutFrameRate ==FBM_FRAME_RATE_50)))
                {
                    // switch to correct field, please consider fast change channel

                    if ((prFrcPrm->ucTargetNs & 1) != 1)
                    {
                        prFrcPrm->ucTargetNs++;
                        prFrcPrm->ucChangeFieldNs = 0;
                        prFrcPrm->ucWrongField = 1;
                    }

                    LOG(3, "Wrong Field, Odd(%d) Tff(%d)\n", prFrcPrm->pVdpPrm->ucBottom, prFrcPrm->ucTff);
                }
                else
                {
                    LOG(7, "No TargetNs++\n");
                }
            }
        }
        prFrcPrm->ucChangeFieldNs /= 2;
    }

    // local STC Accumulate
    prFrcPrm->u4InFrameStc += u4InStcPeriod;
    prFrcPrm->u4OutFrameStc += (u4OutStcPeriod * u4OutVSyncNs);

    // prevent overflow
    if ((prFrcPrm->u4InFrameStc > 0x80000000) &&
            (prFrcPrm->u4OutFrameStc > 0x80000000))
    {
        prFrcPrm->u4InFrameStc -= 0x80000000;
        prFrcPrm->u4OutFrameStc -= 0x80000000;
    }
}

void VDP_ResetPts(void)
{
    _arVdpPrm[0].rPortPrm.u4LastPts = 0;
}

#ifdef VDP_AVS_ADJUST_STC
static VOID _VDP_SetAvsyncDelta(B2R_AVS_T* prAvs, UINT32 u4Delta,BOOL bAddFlg)
{
    if( u4Delta < prAvs->u4Delay )
    {
        return;
    }

    if( bAddFlg )
    {
        prAvs->i8Delta += (u4Delta - prAvs->u4Delay);
    }
    else
    {
        prAvs->i8Delta -= (u4Delta + prAvs->u4Delay);
    }

    return;
}

static VOID _VDP_SetDeltaInit(B2R_AVS_T* prAvs)
{
    prAvs->i8Delta = 0x0;
    prAvs->u4Delay = 30000;
}

INT64 _VDP_GetAvsyncDelta(void)
{
    return _rB2rAvs.i8Delta;
}

#endif


//-----------------------------------------------------------------------------
/** Brief of _VdpAVSyncProc.
 * reference 8105 vsv_sync.c (fgVsvAVSyncProc)
 * Description : AV Sync procedure
 *  when AV difference < VIDEO_SYNC_THRESHOLD, no need skip/repeat
 *  if VIDEO_SYNC_THRESHOLD < AV difference < VIDEO_SYNC_RST_THRESHOLD, skip/repeat 1 frame
 *  if VIDEO_SYNC_RST_THRESHOLD < AV difference < VIDEO_WAIT_SYNC_THRESHOLD, skip/repeat 5 frame
 *  if AV difference > VIDEO_WAIT_SYNC_THRESHOLD, stall decode
 * *********************************************************************
 */
//-----------------------------------------------------------------------------
static void _VdpAVSyncProc(FRC_PRM_T* prFrcPrm)
{
#ifndef CC_VDP_FPGA

    UCHAR ucAvSyncMode;
    UCHAR ucStcSrc;
    UCHAR ucB2RID;

    if (prFrcPrm == NULL)
    {
        return;
    }

    if (prFrcPrm->ucVdpId >= VDP_NS)
    {
        ASSERT(prFrcPrm->ucVdpId < VDP_NS);
        return;
    }

    if (prFrcPrm->u4CbPtsEn)
    {
        if ((prFrcPrm->u4Pts >= prFrcPrm->u4CbPts) ||
                ((prFrcPrm->u4CbPts - prFrcPrm->u4Pts) > 0x80000000))    // normal & warp case
        {
            _VDP_VsyncNotify(VDP_MSG_CC_CB, (UINT32)prFrcPrm->ucVdpId, prFrcPrm->u4CbPts, prFrcPrm->u4CbArg);

            prFrcPrm->u4CbPtsEn = 0;
        }
        else
        {
            // call back protection
            if (prFrcPrm->ucReady)
            {
                // invalid PTS detection
                if ((prFrcPrm->u4CbLastPts != 0) &&
                        (prFrcPrm->u4Pts < prFrcPrm->u4CbLastPts) &&
                        ((prFrcPrm->u4CbLastPts - prFrcPrm->u4Pts) < 0x80000000))
                {
                    _VDP_VsyncNotify(VDP_MSG_CC_CB, (UINT32)prFrcPrm->ucVdpId, prFrcPrm->u4CbPts, (prFrcPrm->u4CbArg | VDP_CB_ERR));

                    prFrcPrm->u4CbPtsEn = 0;

                    LOG(5, "P2(%x)\n", prFrcPrm->u4CbPts);
                }
                else
                {
                    prFrcPrm->u4CbLastPts = prFrcPrm->u4Pts;
                }
            }
        }
    }


#ifdef __MODEL_slt__
    //SLT_CurrentPts(SLT_TYPE_VDO, prFrcPrm->u4Pts);
#endif

    if (prFrcPrm->ucTargetNs == 0)
    {
        return;
    }

    //we don't perform av sync in (timeshift && speed != 1x)
    if(_fgTimeShift[0] && (_i4TimeShiftSpeed[0] == 200 || _i4TimeShiftSpeed[0] == 50))
    {
        return;
    }

    // We can do only 1 AVS-R per second
    if (prFrcPrm->u4PtsAvSyncNotAllow > 0)
    {
        prFrcPrm->u4PtsAvSyncNotAllow--;
    }

    FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);

    if (prFrcPrm->prFbCounter == NULL)
    {
        ASSERT(prFrcPrm->prFbCounter);
        return;
    }

    // skip AV Sync for Late Notify
    if (prFrcPrm->ucNewFbArrive != 0)
    {
        return;
    }

    if (prFrcPrm->ucProgressiveSeqFlag == 0)
    {
        // if drift > 3 secs then free run
        if (_u4MaxDriftCount >= 100)
        {
            // count valid
            if (STC_ChkStcStatusValid(0))
            {
                ++_u4ValidCount;
            }
            else
            {
                _u4ValidCount = 0;
            }

            // if valid > 5 secs then restore the av sync
            if (_u4ValidCount >= 155)
            {
                _u4MaxDriftCount = 0;
            }

            return;
        }

        // count drift
        if (STC_ChkStcStatusDrift(0))
        {
            ++_u4DriftCount;
        }
        else
        {
            _u4DriftCount = 0;
        }

        _u4MaxDriftCount = MAX(_u4MaxDriftCount, _u4DriftCount);
    }
    ucB2RID = _prVdpConf[prFrcPrm->ucVdpId]->ucB2rId;

    if (ucAvSyncMode != VID_SYNC_MODE_NONE)
    {
        STC_CLOCK rStcClk;
        if (STC_GetSrc(ucStcSrc, &rStcClk) == STC_VALID)
        {
            UINT32 u4Delta;
            INT32 i4Delta;
            INT32 i4PtsDrift;
            UINT32 u4OutFrameStc;

            UINT32 u4ZeroPtsNs;
            UINT32 u4PtsDelta;
            VDEC_ES_INFO_T* prVdecEsInfo;

            prVdecEsInfo = _VDEC_GetEsInfo(ES0);

#ifdef CC_VDP_FRC_DEBUG
            prFrcPrm->u4Stc = rStcClk.u4Base;
#endif

            u4OutFrameStc = (prFrcPrm->ucTargetNs * prFrcPrm->u4OutStcPeriod);

#ifdef VDP_ADD_1_VSYNC_TIME
            // The actual display time is 1 Vsync later
            rStcClk.u4Base += prFrcPrm->u4OutStcPeriod;
#endif


#ifdef CC_B2R_WAIT_NPTV_STABLE
            //wait for NPTV stable
            //UCHAR ucPlayMode;
            FBM_GetPlayMode(prFrcPrm->ucFbgId,&_u1PlayMode);
            if( (FBM_FBG_MM_MODE == _u1PlayMode) && (prFrcPrm->ucReady ==1) && (!_fgNPTVStable) && !prFrcPrm->fgB2R3DEnable)
            {
                //static BOOL _fgNPTVStable = TRUE;

                if(_u4DrvVideoGetMute(prFrcPrm->ucVdpId) && (prFrcPrm->ucFbId !=FBM_FB_ID_UNKNOWN))
                {
                    //if(_fgNPTVStable)
                    {
                        //_VDP_VsyncNotify(VDP_MSG_NOTIFY, (UINT32)prFrcPrm->ucVdpId, VDP_B2R_START_PLAY, 0);
                    }

                    FBM_SetFrameBufferGlobalFlag(prFrcPrm->ucFbgId, FBM_FLAG_FB_NO_TIMEOUT);

                    _fgNPTVStable = FALSE;

                    _u4WaitNPTVStableCount++;

                    // pending for the 1st PTS
                    prFrcPrm->ucTargetNs = 0;
                    prFrcPrm->ucChangeFieldNs = 0;

                    prFrcPrm->u4InFrameStc = 0;
                    prFrcPrm->u4OutFrameStc = 0;

                    prFrcPrm->ucPendingFbId = prFrcPrm->ucFbId;
                    prFrcPrm->ucLastFbId = FBM_FB_ID_UNKNOWN;
                    prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;

                    LOG(5, "----Pending for wating NPTV stable %d \n",_u4WaitNPTVStableCount);
                    return;
                }
                else if((!_u4DrvVideoGetMute(0) /*|| (_u4WaitNPTVStableCount >= 150)*/) && !_fgNPTVStable)
                {
                    _u4WaitNPTVStableCount =0;
                    _fgNPTVStable = TRUE;

                    //LOG(3,"wait %d vsyncs for NPTV statlbe\n",_u4WaitNPTVStableCount);

#ifdef CC_53XX_SWDMX_V2
                    STC_StopStc(ucStcSrc);
                    STC_SetStcValue(ucStcSrc,(UINT32)(prFrcPrm->u4Pts));
                    rStcClk.u4Base = (UINT32)(prFrcPrm->u4Pts);
                    STC_StartStc(ucStcSrc);
#else
                    STC_StopStc();
                    STC_SetStcValue((UINT32)(prFrcPrm->u4Pts));
                    rStcClk.u4Base = (UINT32)(prFrcPrm->u4Pts);
                    STC_StartStc();
#endif
                    rStcClk.u4Base = prFrcPrm->u4Pts;

                    FBM_ClrFrameBufferGlobalFlag(prFrcPrm->ucFbgId, FBM_FLAG_FB_NO_TIMEOUT);
#ifdef CC_B2R_WAIT_NPTV_STABLE
                    FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_DISPLAYING);
#endif
                    LOG(3, "----ready for waiting NPTV stable\n");
                }
            }
            else if(prFrcPrm->fgB2R3DEnable)
            {
                _fgNPTVStable = TRUE;
            }
#endif
            //wait for NPTV statble
            // Zero PTS
            u4ZeroPtsNs = prFrcPrm->u4ZeroPtsNs;
            if (prFrcPrm->u4Pts == 0)
            {
                LOG(9, "Zero PTS (%d) Delta(%d)\n", prFrcPrm->u4ZeroPtsNs, prFrcPrm->u4PtsDelta);

                prFrcPrm->u4ZeroPtsNs++;

                if (prFrcPrm->ucReady == 0)
                {
                    if ((ucAvSyncMode == VID_SYNC_MODE_SLAVE_MM)
                            || (ucAvSyncMode == VID_SYNC_MODE_MASTER) /* 2x Trick Play */)
                    {
                        // MM playback receive 1st PTS as 0, Start to Play
                        LOG(3, "MM playback receive 1st PTS as 0, Start to Play\n");
                        prFrcPrm->u4PtsAsyncNs = 0;
                    }
                    else
                    {
                        if (prFrcPrm->u4PtsAsyncNs < VDP_WAIT_NONZERO_PTS_NS)
                        {
                            // pending for the 1st PTS
                            prFrcPrm->ucTargetNs = 0;
                            prFrcPrm->ucChangeFieldNs = 0;

                            prFrcPrm->u4InFrameStc = 0;
                            prFrcPrm->u4OutFrameStc = 0;

                            // drop this frame, wait for another frame with PTS != 0
                            /*
                                                        prFrcPrm->ucPendingFbId = prFrcPrm->ucFbId;
                                                        prFrcPrm->ucLastFbId = FBM_FB_ID_UNKNOWN;
                                                        prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
                            */
                            prFrcPrm->u4PtsAsyncNs++;
                        }
                        else
                        {
                            // STC still invalid after 12 vsync, forget it, go display!
                            LOG(3, "Force Video Start 1\n");
                            prFrcPrm->u4PtsAsyncNs = 0;
                        }
                    }

                    prFrcPrm->prFbCounter->u4SyncMode = FBM_FBG_AVSYNC_NO_PTS;
                    FBM_SetAVSyncStatus(prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4SyncMode);
                    return;

                }
                else
                {
#if 0
                    if ((ucAvSyncMode == VID_SYNC_MODE_SLAVE_MM)
                            || (ucAvSyncMode == VID_SYNC_MODE_MASTER) /* 2x Trick Play */)
                    {
                        //Calculate PTS for no PTS picture when trick mode

                        UINT32 u4Diff;

                        if (prFrcPrm->u4LastPts == 0)
                        {
                            //prFrcPrm->u4Pts = rStcClk.u4Base;
                            // Pibben: Why use STC value at above calculation?
                            prFrcPrm->u4Pts = 0;
                        }
                        else
                        {
                            u4Diff = 0xFFFFFFFF - prFrcPrm->u4LastPts;

                            if (u4Diff >= u4OutFrameStc)
                            {
                                prFrcPrm->u4Pts = prFrcPrm->u4LastPts + u4OutFrameStc;
                            }
                            else
                            {
                                prFrcPrm->u4Pts = (u4OutFrameStc - u4Diff);
                            }
                            if (prFrcPrm->ucRealChangeFb == 0)
                            {
                                /* MM pending for last frame, no PTS accumulation */
                                prFrcPrm->u4Pts = prFrcPrm->u4LastPts;
                                LOG(9, "MM pending for last frame, no PTS accumulation\n");
                            }
                        }

                        LOG(9, "AVS-C PTS (0x%x)\n", prFrcPrm->u4Pts);
                        prFrcPrm->u4ZeroPtsNs = 0;

                        prFrcPrm->u4KeepDropB = 0;

                        // Backup last PTS
                        prFrcPrm->u4LastPts = prFrcPrm->u4Pts;

                    }
                    else if (ucAvSyncMode == VID_SYNC_MODE_SLAVE)
#endif
                    {
                        // Handle Keep Drop B
                        if (prFrcPrm->u4KeepDropB != 0)
                        {
                            LOG(3, "AVS-D VLD Zero PTS MPV Q(%d)\n", _prDbase->rMpv[0].u4EsmQCnt);

                            FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SKIP_PIC_NO_PTS);
                        }

                        prFrcPrm->prFbCounter->u4SyncMode = FBM_FBG_AVSYNC_NO_PTS;
                        FBM_SetAVSyncStatus(prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4SyncMode);
                        return;
                    }
                }
            }
            else // if current pts !=0 , calculate difference between current frame and last frame
            {
#if 0
                if (((prFrcPrm->u4CbSyncEn) && (prFrcPrm->u4CbSyncDone == 0)) ||
                        (_rVdpLipSync.ucNg2OkCbEn == 1) ||
                        (_rVdpLipSync.ucOk2NgCbEn == 1))
                {
                    UINT32 u4AVDiff;
                    //STC_CLOCK rClock;

#if 0
                    STC_GetAudioPts(&rClock);


                    if (rClock.u4Base >= prFrcPrm->u4Pts)
                    {
                        u4AVDiff = rClock.u4Base - prFrcPrm->u4Pts;
                    }
                    else
                    {
                        u4AVDiff = prFrcPrm->u4Pts - rClock.u4Base;
                    }

                    if (u4AVDiff >= 0x80000000)
                    {
                        // handle wrap around
                        u4AVDiff = (0xFFFFFFFF - u4AVDiff);

                    }

                    LOG(1," (0x%x, 0x%x, %d)\n", rClock.u4Base, prFrcPrm->u4Pts, u4AVDiff);

#else

                    if (rStcClk.u4Base >= prFrcPrm->u4Pts)
                    {
                        u4AVDiff = rStcClk.u4Base - prFrcPrm->u4Pts;
                    }
                    else
                    {
                        u4AVDiff = prFrcPrm->u4Pts - rStcClk.u4Base;
                    }

                    if (u4AVDiff >= 0x80000000)
                    {
                        // handle wrap around
                        u4AVDiff = (0xFFFFFFFF - u4AVDiff);

                    }

                    LOG(9," (0x%x, 0x%x, Diff: %d)\n", rStcClk.u4Base, prFrcPrm->u4Pts, u4AVDiff);

#endif

                    if ((prFrcPrm->u4CbSyncEn) && (prFrcPrm->u4CbSyncDone == 0))
                    {
                        if (u4AVDiff < prFrcPrm->u4CbSyncThrsd)
                        {
                            prFrcPrm->u4CbSyncFrmCnt++;

                            if (prFrcPrm->u4CbSyncFrmCnt >= prFrcPrm->u4CbSyncChkFrmCnt)
                            {
                                _VDP_VsyncNotify(VDP_MSG_LIPSYNC_CB, prFrcPrm->ucVdpId, 0, 0);
                                LOG(9, "LIP Sync callback\n");
                                prFrcPrm->u4CbSyncDone = 1;
                            }
                        }
                        else
                        {
                            prFrcPrm->u4CbSyncFrmCnt = 0;
                        }
                    }

                    if((_rVdpLipSync.ucLipSyncState == VDP_LIP_SYNC_NG) &&
                            (_rVdpLipSync.ucNg2OkCbEn == 1))
                    {
                        /* _rVdpLipSync.u4OKThrd should equal to _rVdpLipSync.u4NGThrd? */
                        if (u4AVDiff <= _rVdpLipSync.u4OKThrd)
                        {
                            if(_rVdpLipSync.u4OKFrmCnt < _rVdpLipSync.u4NumFrmsOK)
                            {
                                _rVdpLipSync.u4OKFrmCnt++;
                            }
                            else
                            {
                                _rVdpLipSync.ucLipSyncState = VDP_LIP_SYNC_OK;
                                _rVdpLipSync.u4NGFrmCnt = 0;
                                /* Lip Sync state transition callback */
                                _VDP_VsyncNotify(VDP_MSG_LIPSYNC_NG_TO_OK_CB, prFrcPrm->ucVdpId, 0, 0);
                                LOG(0, "Lip Sync state transition to OK callback\n");
                            }
                        }
                        else
                        {
                            _rVdpLipSync.u4OKFrmCnt = 0;
                        }
                    }
                    else if((_rVdpLipSync.ucLipSyncState == VDP_LIP_SYNC_OK) &&
                            (_rVdpLipSync.ucOk2NgCbEn == 1))
                    {
                        if (u4AVDiff > _rVdpLipSync.u4NGThrd)
                        {
                            if(_rVdpLipSync.u4NGFrmCnt < _rVdpLipSync.u4NumFrmsNG)
                            {
                                _rVdpLipSync.u4NGFrmCnt++;
                            }
                            else
                            {
                                _rVdpLipSync.ucLipSyncState = VDP_LIP_SYNC_NG;
                                _rVdpLipSync.u4OKFrmCnt = 0;
                                /* Lip Sync state transition callback */
                                _VDP_VsyncNotify(VDP_MSG_LIPSYNC_OK_TO_NG_CB, prFrcPrm->ucVdpId, 0, 0);
                                LOG(0, "Lip Sync state transition to NG callback\n");
                            }
                        }
                        else
                        {
                            _rVdpLipSync.u4NGFrmCnt = 0;
                        }
                    }

                }
#else
                //if (/*(prFrcPrm->u4CbSyncEn) && */(prFrcPrm->u4CbSyncDone == 0))
                {
                    //if (u4AVDiff < prFrcPrm->u4CbSyncThrsd)
                    {
                        //prFrcPrm->u4CbSyncFrmCnt++;

                        //if (prFrcPrm->u4CbSyncFrmCnt >= prFrcPrm->u4CbSyncChkFrmCnt)
                        {
                            //_VDP_VsyncNotify(VDP_MSG_LIPSYNC_CB, prFrcPrm->ucVdpId, 0, 0);
                            //LOG(11, "LIP Sync callback\n");
                            //prFrcPrm->u4CbSyncDone = 1;
                        }
                    }
                    //else
                    //{
                    //    prFrcPrm->u4CbSyncFrmCnt = 0;
                    //}
                }
#endif

                prFrcPrm->u4ZeroPtsNs = 0;

                if (u4ZeroPtsNs != 0)
                {
                    prFrcPrm->u4KeepDropB = 0;
                }

                if (prFrcPrm->u4LastPts != 0)
                {
                    // Calculate PTS Delta
                    if (prFrcPrm->u4Pts > prFrcPrm->u4LastPts)
                    {
                        u4PtsDelta = (prFrcPrm->u4Pts - prFrcPrm->u4LastPts);
                    }
                    else
                    {
                        // handle wrap around
                        u4PtsDelta = (0xFFFFFFFF - prFrcPrm->u4LastPts);
                        u4PtsDelta += prFrcPrm->u4Pts;
                    }

                    if (prFrcPrm->u4PtsDelta != 0)
                    {
                        prFrcPrm->u4PtsDelta -= (prFrcPrm->u4PtsDelta / VDP_MAX_PTS_FILTER_LEN);
                        prFrcPrm->u4PtsDelta += (u4PtsDelta / VDP_MAX_PTS_FILTER_LEN);
                    }
                    else
                    {
                        // Filter initial
                        prFrcPrm->u4PtsDelta = u4PtsDelta;
                    }
                }

                // Backup last PTS
                prFrcPrm->u4LastPts = prFrcPrm->u4Pts;
            }

            //calcus difference between current pts and STC
            if (prFrcPrm->u4Pts > rStcClk.u4Base)
            {
                u4Delta = (prFrcPrm->u4Pts - rStcClk.u4Base);

                if (u4Delta < 0x80000000)
                {
                    i4Delta = - (INT32) u4Delta;
                }
                else
                {
                    // handle wrap around
                    u4Delta = (0xFFFFFFFF - u4Delta);
                    i4Delta = (INT32) u4Delta;

                    // [LOG] AVSYNC, V-PTS, Wrap around
                    SYSLOG(SYNC_PREFIX + 98, 20 + prFrcPrm->ucFbgId, prFrcPrm->u4Pts, rStcClk.u4Base);
                }
            }
            else
            {
                u4Delta = (rStcClk.u4Base - prFrcPrm->u4Pts);

                if (u4Delta < 0x80000000)
                {
                    i4Delta = (INT32) u4Delta;
                }
                else
                {
                    // handle wrap around
                    u4Delta = (0xFFFFFFFF - u4Delta);
                    i4Delta = - (INT32) u4Delta;

                    // [LOG] AVSYNC, V-PTS, Wrap around
                    SYSLOG(SYNC_PREFIX + 98, prFrcPrm->ucFbgId, prFrcPrm->u4Pts, rStcClk.u4Base);
                }
            }

#ifdef CC_VDP_FRC_DEBUG
            prFrcPrm->i4PtsStcDelta = i4Delta;
#endif

#ifdef CC_GET_PCR_PTS_INFO
            _prDbase->rPts.u4VideoPts = prFrcPrm->u4Pts;
            _prDbase->rPts.u4VideoPtsStcDelta = u4Delta;
            _prDbase->rPts.i4VideoPtsStcDelta = i4Delta;

            if (u4Delta >= 27000)     // 300ms
            {
                _prDbase->rPts.fgVideoSync = FALSE;
            }
            else
            {
                _prDbase->rPts.fgVideoSync = TRUE;
            }
#endif

            LOG(7, "PTS %x, STC %x, Delta (%d, %d)\n", prFrcPrm->u4Pts, rStcClk.u4Base, i4Delta, prFrcPrm->i4PtsDrift);
#ifdef CC_GET_PCR_PTS_INFO
            if (prFrcPrm->ucReady != 0)
            {
                HAL_TIME_T rTime;
                HAL_TIME_T rDeltaTime;

                HAL_GetTime(&rTime);
                HAL_GetDeltaTime(&rDeltaTime, &(prFrcPrm->rLastAvSyncTime), &rTime);

                // check if AV sync lost for a long time
                if ((rDeltaTime.u4Seconds >= (10 + 5)) &&
                        (u4Delta > VDP_INVALID_DRIFT))
                {
                    // try to sync video PTS to audio PTS
                    LOG(4, "V2A Delta(%d)\n", u4Delta);

                    if (prFrcPrm->u4Pts > _prDbase->rPts.u4AudioPts)
                    {
                        u4Delta = (prFrcPrm->u4Pts - _prDbase->rPts.u4AudioPts);

                        if (u4Delta < 0x80000000)
                        {
                            i4Delta = - (INT32) u4Delta;
                        }
                        else
                        {
                            // handle wrap around
                            u4Delta = (0xFFFFFFFF - u4Delta);
                            i4Delta = (INT32) u4Delta;
                        }
                    }
                    else
                    {
                        u4Delta = (_prDbase->rPts.u4AudioPts - prFrcPrm->u4Pts);

                        if (u4Delta < 0x80000000)
                        {
                            i4Delta = (INT32) u4Delta;
                        }
                        else
                        {
                            // handle wrap around
                            u4Delta = (0xFFFFFFFF - u4Delta);
                            i4Delta = - (INT32) u4Delta;
                        }
                    }
                }
            }
            else
            {
                // 1st picture after change channel

#if 1
                // solution #2
#else

                // solution #1

                if (u4Delta > VDP_INVALID_DRIFT)
                {
                    UINT32 u4WaitThreshold = 0;

                    LOG(4, "1st V-Drift A-PTS %x, WAIT(%d)\n", _prDbase->rPts.u4AudioPts, prFrcPrm->u4PtsAsyncNs);

                    switch (_prVdpConf[prFrcPrm->ucVdpId]->rOutInfo.ucPmxMode)
                    {
                    case VDP_B2R_MODE_480I:
                    case VDP_B2R_MODE_480P:
                    case VDP_B2R_MODE_576I:
                    case VDP_B2R_MODE_576P:
                        u4WaitThreshold = 30;
                        break;

                    case VDP_B2R_MODE_720P:
                        u4WaitThreshold = 24;
                        break;

                    case VDP_B2R_MODE_1080I:
                    default:
                        u4WaitThreshold = 16;
                        break;
                    }

                    if (prFrcPrm->u4PtsAsyncNs < u4WaitThreshold)
                    {
                        // pending for the 1st PTS
                        prFrcPrm->ucTargetNs = 0;
                        prFrcPrm->ucChangeFieldNs = 0;

                        prFrcPrm->u4InFrameStc = 0;
                        prFrcPrm->u4OutFrameStc = 0;

                        prFrcPrm->ucPendingFbId = prFrcPrm->ucFbId;
                        prFrcPrm->ucLastFbId = FBM_FB_ID_UNKNOWN;
                        prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;

                        prFrcPrm->u4PtsAsyncNs++;
                    }
                }
#endif
            }
#endif

            // handle PTS drift
#ifdef CC_VDO_PTS_FILTER

            // do not handle PTS drift while Fast Channel Changing
#ifdef CC_FAST_CHANGE_CHANNEL
            if (prFrcPrm->ucFastChangeEn == 0)
            {
#endif

                // Handle Keep Drop B
                if (prFrcPrm->u4KeepDropB != 0)
                {
                    if ((i4Delta > 0) && (u4Delta > u4OutFrameStc))
                    {
                        FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SKIP_PIC);
                    }
                    else
                    {
                        prFrcPrm->u4KeepDropB = 0;
                    }
                }

#ifndef CC_B2R_DISABLE_PTS_IIR
                if ((prFrcPrm->ucReady) &&
                        (_arVdpDtvFreeze[prFrcPrm->ucVdpId].u4UnFreeze == 0) &&
                        (u4ZeroPtsNs == 0) && (ucAvSyncMode == VID_SYNC_MODE_SLAVE))
                {
                    if (prVdecEsInfo != NULL)
                    {
#ifdef ENABLE_MULTIMEDIA
                        if (!((prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) && (prFrcPrm->fgReleaseDispQ)))
#endif
                        {
                            i4PtsDrift = i4Delta - prFrcPrm->i4PtsDrift;
                            prFrcPrm->i4PtsDrift += (i4PtsDrift / VDP_MAX_PTS_FILTER_LEN);

                            if ((i4PtsDrift > VDP_MAX_PTS_DRIFT) ||
                                    (i4PtsDrift < -VDP_MAX_PTS_DRIFT))
                            {
                                LOG(1, "PTS Jitter! (%d, %d, %d)\n", i4PtsDrift, i4Delta, prFrcPrm->i4PtsDrift);

                                prFrcPrm->prFbCounter->u4SyncMode = FBM_FBG_AVSYNC_PTS_JITTER;
                                FBM_SetAVSyncStatus(prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4SyncMode);
                                return;
                            }
                            else
                            {
                                UNUSED(prVdecEsInfo);
                                prFrcPrm->fgReleaseDispQ = FALSE;
                            }
                        }
                    }
                    else
                    {
                        LOG(5,"_VdpAVSyncProc : prVdecEsInfo == NULL\n");
                    }
                }
                else
#else
                UNUSED(prVdecEsInfo);
                UNUSED(i4PtsDrift);
#endif
                {
                    if (_arVdpDtvFreeze[prFrcPrm->ucVdpId].u4UnFreeze != 0)
                    {
                        prFrcPrm->u4PtsAvSyncNotAllow = 0;
                        _arVdpDtvFreeze[prFrcPrm->ucVdpId].u4UnFreeze = 0;
                    }

                    prFrcPrm->i4PtsDrift = i4Delta;
                }

#ifdef CC_FAST_CHANGE_CHANNEL
            }
#endif

#else
            UNUSED(i4PtsDrift);
#endif

            if (ucAvSyncMode == VID_SYNC_MODE_SLAVE)
            {
                if ((prFrcPrm->ucReady != 0) && (u4Delta > VDP_INVALID_DRIFT))
                {
                    // Bad Edit Program
                    prFrcPrm->u4KeepDropB = 0;
                    prFrcPrm->prFbCounter->u4SyncMode = FBM_FBG_AVSYNC_PTS_JITTER;
                    FBM_SetAVSyncStatus(prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4SyncMode);
                    return;
                }
            }

            HAL_GetTime(&(prFrcPrm->rLastAvSyncTime));

            if (i4Delta < 0)
            {
                // PTS slower than STC, repeat
                // [LOG] AVSYNC, V-PTS slower than STC
//                SYSLOG(SYNC_PREFIX + prFrcPrm->ucFbgId, rStcClk.u4Base, u4Delta, prFrcPrm->i4TemporalRef);

                prFrcPrm->prFbCounter->u4SyncMode = FBM_FBG_AVSYNC_NORMAL;
                FBM_SetAVSyncStatus(prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4SyncMode);

                if (u4Delta > u4OutFrameStc && ucB2RID<B2R_NS)
                {
                    UINT32 u4Step;
                    UINT32 u4TargetNs;

                    if(u4OutFrameStc != 0)
                    {
                        _arVdpPrm[ucB2RID].rPortPrm.u4B2rSkipFrameNo = u4Delta / u4OutFrameStc;
                    }
                    else
                    {
                        _arVdpPrm[ucB2RID].rPortPrm.u4B2rSkipFrameNo = 0;
                    }
                    prFrcPrm->prFbCounter->u4SyncMode = FBM_FBG_AVSYNC_REPEAT;
                    FBM_SetAVSyncStatus(prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4SyncMode);

#ifndef CC_B2R_DISABLE_ONE_SECOND_FRC
                    // protection
                    if ((ucAvSyncMode == VID_SYNC_MODE_SLAVE) && (prFrcPrm->u4PtsAvSyncNotAllow))
                    {
                        return;
                    }
#endif

                    if (prFrcPrm->ucReady == 0)
                    {
#ifdef VDP_EARLY_DISPLAY

                        if (u4Delta > VDP_EARLY_DISP_DRIFT && u4Delta < VDP_INVALID_DRIFT)
                        {
                            // pending for the 1st PTS
                            prFrcPrm->ucTargetNs = 0;
                            prFrcPrm->ucChangeFieldNs = 0;

                            prFrcPrm->u4InFrameStc = 0;
                            prFrcPrm->u4OutFrameStc = 0;

                            prFrcPrm->ucPendingFbId = prFrcPrm->ucFbId;
                            prFrcPrm->ucLastFbId = FBM_FB_ID_UNKNOWN;
                            prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;

#ifdef VDP_AVS_ADJUST_STC
                            _VDP_SetAvsyncDelta(&_rB2rAvs,u4Delta,TRUE);
#endif
                            LOG(3, "----Pending for the 1st PTS\n");


                            return;
                        }
                        else if(u4Delta > VDP_INVALID_DRIFT)
                        {
                            // Bad Edit Program
                            LOG(3, "Force Video Start 999 for bad edit bitstream\n");
                            prFrcPrm->u4PtsAsyncNs = 0;
                            return ;
                        }

                        else if (_ucVsyncNotStableCount > 0)
                        {
                            if (_ucVsyncNotStableCount == 0xFF)
                            {
                                _ucVsyncNotStableCount = prFrcPrm->ucTargetNs;
                            }

                            // pending for the 1st PTS
                            prFrcPrm->ucTargetNs = 0;
                            prFrcPrm->ucChangeFieldNs = 0;

                            prFrcPrm->u4InFrameStc = 0;
                            prFrcPrm->u4OutFrameStc = 0;

                            prFrcPrm->ucPendingFbId = prFrcPrm->ucFbId;
                            prFrcPrm->ucLastFbId = FBM_FB_ID_UNKNOWN;
                            prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;

                            _ucVsyncNotStableCount--;
                            LOG(9, "VSYNC not stable (%d)!\n", _ucVsyncNotStableCount);
                            return;
                        }

                        _prVdpConf[prFrcPrm->ucVdpId]->ucEarlyDisp = 1;
                        LOG(3, "ucEarlyDisp VDP(%d)\n", prFrcPrm->ucVdpId);

#ifdef VDP_MUTE_EARLY_DISPLAY
                        _vDrvVideoSetMute(MUTE_MODULE_DTV, prFrcPrm->ucVdpId,
                                          FOREVER_MUTE, FALSE);
                        LOG(3, "DTV Mute Early Display -1 VDP(%d)\n", prFrcPrm->ucVdpId);

#ifdef CC_SUPPORT_TVE
                        if ((_prVdpConf[VDP_1]->ucB2rId == B2R_1) &&
                                (_prVdpConf[VDP_2]->ucB2rId == B2R_1)&&
                                (_prVdpConf[VDP_1]->ucEnable == 1) &&
                                (_prVdpConf[VDP_2]->ucEnable == 1) )
                        {
                            _prVdpConf[1-prFrcPrm->ucVdpId]->ucEarlyDisp = 1;
                            LOG(3, "ucEarlyDisp VDP(%d)\n", (1-prFrcPrm->ucVdpId));

                            _vDrvVideoSetMute(MUTE_MODULE_DTV, (1 - prFrcPrm->ucVdpId),
                                              FOREVER_MUTE, FALSE);
                            LOG(3, "DTV Mute Early Display -2 VDP(%d)\n", prFrcPrm->ucVdpId);
                        }
#endif
#endif

                        u4Step = 1 + (u4Delta / u4OutFrameStc);
                        prFrcPrm->i4PtsDrift += (INT32)(u4OutFrameStc * (u4Step - 1));

                        prFrcPrm->ucTargetNs *= u4Step;

                        if (prFrcPrm->ucTargetNs != 2)
                        {
                            prFrcPrm->ucChangeFieldNs = 0;
                        }

                        if (prFrcPrm->ucProgressiveOut)
                        {
                            prFrcPrm->u4WaitUnmute = (prFrcPrm->ucTargetNs + VDP_VIDEO_DELAY_VSYNC) - 2;
                        }
                        else
                        {
                            prFrcPrm->u4WaitUnmute = prFrcPrm->ucTargetNs + VDP_VIDEO_DELAY_VSYNC;
                        }

                        if (prFrcPrm->u4WaitUnmute >= VDP_WAIT_UNMUTE_VSYNC)
                        {
                            prFrcPrm->u4WaitUnmute = VDP_WAIT_UNMUTE_VSYNC;
                        }

                        LOG(5, "-----Wait Unmute VSync(%d)  PTS(0x%x) \n", prFrcPrm->u4WaitUnmute, prFrcPrm->u4Pts);

                        LOG(3, "AVS-R Vdp(%d) Fbg(%d) %d PTS(0x%x) STC(0x%x)\n", prFrcPrm->ucVdpId, prFrcPrm->ucFbgId, prFrcPrm->ucTargetNs, prFrcPrm->u4Pts, rStcClk.u4Base);

                        return;

#else
#ifdef CC_FAST_CHANGE_CHANNEL
                        UINT32 u4Ahead;
                        u4Ahead = STC_GetFastChangeChangeAhead(ucStcSrc);

                        if (u4Ahead > u4Delta)
                        {
                            if (prFrcPrm->ucInterlace2Interlace)
                            {
                                if (prFrcPrm->ucWrongField == 0)
                                {
                                    // Playback single field only for FRC
                                    prFrcPrm->ucTargetNs++;
                                    prFrcPrm->ucChangeFieldNs = 0;
                                }
                            }
                            else
                            {
                                prFrcPrm->ucTargetNs++;
                            }

                            prFrcPrm->ucFastChangeEn = 1;

                            LOG(3, "Enable Fast Start PTS %x, STC %x, Delta (%d) Ahead(%d)\n", prFrcPrm->u4Pts, rStcClk.u4Base, i4Delta, u4Ahead);

#ifdef CC_FAST_CHANGE_CHANNEL_LOG
                            HAL_GetTime(&rNvgtVideoStartTime);
                            u4NvgtTimeAhead = u4Ahead;
#endif
                        }
                        else
                        {
#endif

                            if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_ES_FIFO_FULL))
                            {
                                // ES FIFO full, can not wait anymore
                                LOG(3, "Force Video Start 2\n");
                                prFrcPrm->u4PtsAsyncNs = 0;
                            }
                            else if (u4Delta > VDP_INVALID_DRIFT)
                            {
                                // Bad Edit Program
                                LOG(3, "Force Video Start 4\n");
                                prFrcPrm->u4PtsAsyncNs = 0;
                            }
                            else
                            {
                                // pending for the 1st PTS
                                prFrcPrm->ucTargetNs = 0;
                                prFrcPrm->ucChangeFieldNs = 0;

                                prFrcPrm->u4InFrameStc = 0;
                                prFrcPrm->u4OutFrameStc = 0;

                                prFrcPrm->ucPendingFbId = prFrcPrm->ucFbId;
                                prFrcPrm->ucLastFbId = FBM_FB_ID_UNKNOWN;
                                prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
                            }

                            return;

#ifdef CC_FAST_CHANGE_CHANNEL
                        }
#endif
#endif
                    }

                    // ES FIFO FULL, don't repeat anymore
                    if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_ES_FIFO_FULL))
                    {
                        return;
                    }

                    if (prFrcPrm->ucReady == 1)
                    {
#ifdef CC_FAST_CHANGE_CHANNEL
                        if (prFrcPrm->ucFastChangeEn == 1)
                        {
                            if (prFrcPrm->ucInterlace2Interlace)
                            {
                                if (prFrcPrm->ucWrongField == 0)
                                {
                                    // Playback single field only for FRC
                                    prFrcPrm->ucTargetNs++;
                                    prFrcPrm->ucChangeFieldNs = 0;
                                }
                            }
                            else
                            {
                                prFrcPrm->ucTargetNs++;
                            }

                            LOG(3, "Fast Start PTS %x, STC %x, Delta (%d)\n", prFrcPrm->u4Pts, rStcClk.u4Base, i4Delta);
                        }
                        else
                        {
#endif
#ifdef ENABLE_MULTIMEDIA
                            VDEC_ES_INFO_T *prVdecEsInfo;
                            prVdecEsInfo = _VDEC_GetEsInfo(ES0);
#endif

#ifdef VDP_RACE_LOG
                            VDP_FrcRacingLog('a');
                            VDP_FrcRacingLog('+');
#endif

                            if ((ucAvSyncMode == VID_SYNC_MODE_SLAVE_MM)
                                    || (ucAvSyncMode == VID_SYNC_MODE_MASTER) /* Trick Play */
#ifdef ENABLE_MULTIMEDIA
                                    || (prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
#endif
                               )

                            {
                                if (u4Delta > VDP_MM_PEND_MAX)
                                {
                                    if (ucAvSyncMode == VID_SYNC_MODE_MASTER)
                                    {
#ifdef CC_53XX_SWDMX_V2
                                        STC_SetStcValue(ucStcSrc, _VDP_GetDispingPts(prFrcPrm->ucVdpId));
#else
                                        STC_SetStcValue(_VDP_GetDispingPts(prFrcPrm->ucVdpId));
#endif
                                        LOG(3, "MM ----Force Set Stc for bad edit bitstream\n");
                                    }
                                    else
                                    {
                                        LOG(3, "MM ----Force Video Start for bad edit bitstream\n");
                                        return;
                                    }
                                }
                                else //if (u4Delta > VDP_EARLY_DISP_DRIFT)
                                {
                                    UCHAR ucSeekMode;
                                    UINT32 u4SeekPts;
                                    UCHAR ucSeekNotifyTrigger;

                                    _VDPGetSeek(&ucSeekMode, &u4SeekPts, &ucSeekNotifyTrigger);

                                    if ((ucSeekMode != 0) && (ucSeekNotifyTrigger != 0) /*&&
                                       (u4SeekPts <= prFrcPrm->u4Pts)*/)
                                    {
                                        LOG(3, "+++++MM ----Frame Repeat in Seek. force play \n");
                                        return;
                                    }

                                    // pending for the current PTS
                                    prFrcPrm->ucTargetNs = 0;
                                    prFrcPrm->ucChangeFieldNs = 0;

                                    prFrcPrm->u4InFrameStc = 0;
                                    prFrcPrm->u4OutFrameStc = 0;

                                    prFrcPrm->ucPendingFbId = prFrcPrm->ucFbId;
                                    prFrcPrm->ucLastFbId = FBM_FB_ID_UNKNOWN;
                                    prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
                                    prFrcPrm->ucMMWaitSTC = 1;

                                    LOG(3, " MM ----Pending for current PTS\n");
                                    return;
                                }
                                //LOG(3, "MM -- Repeat Last Frame\n\r");
                            }
                            else if (ucAvSyncMode == VID_SYNC_MODE_SLAVE)
                            {
                                // do not repeat B at racing mode
                                if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, (FBM_FLAG_RACING_HALF_B | FBM_FLAG_RACING_FULL_B)))
                                {
                                    if (prFrcPrm->ucPicCdTp == MPEG_PIC_TYPE_B)
                                    {
                                        return;
                                    }
                                }

                                if(FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_TRACKING_MODE_ENABLE))
                                {
                                    if(prFrcPrm->fgTrackingMode)
                                    {
                                        return;
                                    }
                                }

                                u4Step = 1 + (u4Delta / u4OutFrameStc);

                                if ((ucAvSyncMode == VID_SYNC_MODE_SLAVE) && (u4Step > VDP_MAX_ASYNC_REPEAT_NS))
                                {
                                    u4Step = VDP_MAX_ASYNC_REPEAT_NS;
                                }

                                prFrcPrm->prFbCounter->u4DispRepeat += (prFrcPrm->ucTargetNs * (u4Step - 1));

                                prFrcPrm->i4PtsDrift += (INT32)(u4OutFrameStc * (u4Step - 1));

                                u4TargetNs = (prFrcPrm->ucTargetNs*u4Step);
                                if( u4TargetNs >= 0xFF )
                                {
                                    prFrcPrm->ucTargetNs = 250;
                                    prFrcPrm->ucChangeFieldNs =(prFrcPrm->ucTargetNs/2);
                                }
                                else
                                {
                                    prFrcPrm->ucTargetNs = (UCHAR)u4TargetNs;
                                    prFrcPrm->ucChangeFieldNs *= u4Step;
                                }

                                // protection
#ifndef CC_DCR_TEST
                                prFrcPrm->u4PtsAvSyncNotAllow = prFrcPrm->ucInFrameRate;
#endif
                                // [LOG] AVSYNC, V-PTS Repeat
                                SYSLOG(SYNC_PREFIX + 20 + prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4DispOk, prFrcPrm->prFbCounter->u4DispFail, prFrcPrm->ucTargetNs);

                                /* Video Frame repeat notification */
                                _VDP_VsyncNotify(VDP_MSG_REPEAT_DROP_CB, prFrcPrm->ucVdpId,
                                                 prFrcPrm->prFbCounter->u4SyncMode, 0);

                                LOG(3, "AVS-R Vdp(%d) Fbg(%d) %d PTS(0x%x) STC(0x%x)\n", prFrcPrm->ucVdpId, prFrcPrm->ucFbgId, prFrcPrm->ucTargetNs, prFrcPrm->u4Pts, rStcClk.u4Base);
                            }

#ifdef CC_FAST_CHANGE_CHANNEL
                        }
#endif
                    }
                }
#ifdef CC_FAST_CHANGE_CHANNEL
                else
                {
                    if (prFrcPrm->ucFastChangeEn)
                    {
                        LOG(3, "Fast Stop 1\n");
                        prFrcPrm->ucFastChangeEn = 0;
#ifdef CC_FAST_CHANGE_CHANNEL_LOG
                        HAL_GetTime(&rNvgtAvSyncTime);
#endif
                    }
                }
#endif
            }
            else
            {
                // PTS faster than STC, drop
                // [LOG] AVSYNC, V-PTS faster than STC
//                SYSLOG(SYNC_PREFIX + 10 + prFrcPrm->ucFbgId, rStcClk.u4Base, u4Delta, prFrcPrm->i4TemporalRef);

#ifdef CC_FAST_CHANGE_CHANNEL
                if (prFrcPrm->ucFastChangeEn)
                {
                    LOG(3, "Fast Stop 2\n");
                    prFrcPrm->ucFastChangeEn = 0;
#ifdef CC_FAST_CHANGE_CHANNEL_LOG
                    HAL_GetTime(&rNvgtAvSyncTime);
#endif
                }
#endif

                prFrcPrm->prFbCounter->u4SyncMode = FBM_FBG_AVSYNC_NORMAL;
                FBM_SetAVSyncStatus(prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4SyncMode);

                if (u4Delta > u4OutFrameStc && ucB2RID<B2R_NS)
                {

                    if(u4OutFrameStc != 0)
                    {
                        _arVdpPrm[ucB2RID].rPortPrm.u4B2rSkipFrameNo = u4Delta / u4OutFrameStc;
                    }
                    else
                    {
                        _arVdpPrm[ucB2RID].rPortPrm.u4B2rSkipFrameNo = 0;
                    }
                    prFrcPrm->prFbCounter->u4SyncMode = FBM_FBG_AVSYNC_DROP;
                    FBM_SetAVSyncStatus(prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4SyncMode);

#ifndef CC_B2R_DISABLE_ONE_SECOND_FRC
                    // protection
                    if ((ucAvSyncMode == VID_SYNC_MODE_SLAVE) && (prFrcPrm->u4PtsAvSyncNotAllow))
                    {
                        return;
                    }
#endif
#ifdef VDP_AVS_ADJUST_STC
                    if( ( prFrcPrm->ucReady == 0 ) && (ucAvSyncMode == VID_SYNC_MODE_SLAVE) )
                    {
                        // DTV
                        if ( u4Delta > VDP_EARLY_DISP_DRIFT )
                        {
                            // pending for the 1st PTS
                            prFrcPrm->ucTargetNs = 0;
                            prFrcPrm->ucChangeFieldNs = 0;

                            prFrcPrm->u4InFrameStc = 0;
                            prFrcPrm->u4OutFrameStc = 0;

                            prFrcPrm->ucPendingFbId = prFrcPrm->ucFbId;
                            prFrcPrm->ucLastFbId = FBM_FB_ID_UNKNOWN;
                            prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;

                            _VDP_SetAvsyncDelta(&_rB2rAvs,u4Delta,FALSE);


                            LOG(3, "----D Pending for the 1st PTS\n");
                            return;
                        }
                    }
#endif
#ifdef VDP_RACE_LOG
                    VDP_FrcRacingLog('a');
                    VDP_FrcRacingLog('-');
#endif
                    if ((u4ZeroPtsNs != 0) && (u4Delta > prFrcPrm->u4PtsDelta))
                    {
                        // ask MPV to keep drop B if NOT Enough
                        FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SKIP_PIC_NO_PTS);
                        LOG(3, "AVS-D VLD Keep Drop B (Zero PTS) Vdp(%d)\n", prFrcPrm->ucVdpId);
                        prFrcPrm->u4KeepDropB = 1;
                    }
                    else if (u4Delta > (u4OutFrameStc * VDP_KEEP_DROP_B_NS))
                    {
                        // ask MPV to keep drop B if NOT Enough
                        FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SKIP_PIC);
                        LOG(3, "AVS-D VLD Keep Drop B Vdp(%d)\n", prFrcPrm->ucVdpId);
                        prFrcPrm->u4KeepDropB = 1;
                    }

                    // ask Mpv to drop at racing mode
                    if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, (FBM_FLAG_TRACKING_MODE_ENABLE|FBM_FLAG_RACING_FULL_B | FBM_FLAG_RACING_HALF_B)))
                    {
                        FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SKIP_PIC);
                        LOG(3, "AVS-D VLD Vdp(%d) Fbg(%d) PTS(0x%x) STC(0x%x)\n", prFrcPrm->ucVdpId, prFrcPrm->ucFbgId, prFrcPrm->u4Pts, rStcClk.u4Base);

                        return;
                    }

                    // protection
#ifndef CC_DCR_TEST
                    prFrcPrm->u4PtsAvSyncNotAllow = prFrcPrm->ucInFrameRate;
#endif

                    if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SKIP_PIC))
                    {
                        if(!(prFrcPrm->fgB2R3DEnable && prFrcPrm->u1B2R3DType == B2R_3D_FRAME_SEQUENTIAL))
                        {
                            // ask MPV to drop if Display Queue is Empty
                            if (FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId) != 0)
                            {
                                prFrcPrm->i4PtsDrift -= (INT32)u4OutFrameStc;

                                prFrcPrm->ucChangeFieldNs = 0;
                                prFrcPrm->ucTargetNs = 0;

                                // [LOG] AVSYNC, V-PTS Drop
                                SYSLOG(SYNC_PREFIX + 30 + prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4DispOk, prFrcPrm->prFbCounter->u4DispFail, prFrcPrm->ucTargetNs);
                                LOG(3, "AVS-D Vdp(%d) Fbg(%d) PTS(0x%x) STC(0x%x)\n", prFrcPrm->ucVdpId, prFrcPrm->ucFbgId, prFrcPrm->u4Pts, rStcClk.u4Base);
                            }
                            else
                            {
                                LOG(3, "[Warning] Can not drop at both VDP & MPV\n");
                            }
                        }
                    }
                    else
                    {
                        UINT32 u4EsmQCnt;

                        u4EsmQCnt = _prDbase->rMpv[0].u4EsmQCnt;

                        if (u4EsmQCnt > 1)
                        {
                            // ask MPV to drop
                            FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SKIP_PIC);
                            LOG(3, "AVS-D VLD Vdp(%d) Fbg(%d) PTS(0x%x) STC(0x%x) Q(%d)\n", prFrcPrm->ucVdpId, prFrcPrm->ucFbgId, prFrcPrm->u4Pts, rStcClk.u4Base, u4EsmQCnt);
                        }
                        else
                        {
                            if (FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId) != 0)
                            {
                                prFrcPrm->i4PtsDrift -= (INT32)u4OutFrameStc;

                                prFrcPrm->ucChangeFieldNs = 0;
                                prFrcPrm->ucTargetNs = 0;

                                // [LOG] AVSYNC, V-PTS Drop
                                SYSLOG(SYNC_PREFIX + 30 + prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4DispOk, prFrcPrm->prFbCounter->u4DispFail, prFrcPrm->ucTargetNs);
                                LOG(3, "AVS-D Vdp(%d) Fbg(%d) PTS(0x%x) STC(0x%x)\n", prFrcPrm->ucVdpId, prFrcPrm->ucFbgId, prFrcPrm->u4Pts, rStcClk.u4Base);
                            }
                            else
                            {
                                LOG(3, "[Warning] Can not drop at VDP & MPV\n");
                            }
                        }
                    }

                    /* Video Frame drop notification */
                    /*_VDP_VsyncNotify(VDP_MSG_REPEAT_DROP_CB, prFrcPrm->ucVdpId,
                                     prFrcPrm->prFbCounter->u4SyncMode, 0);*/

                    prFrcPrm->prFbCounter->u4DispDrop++;

#if 0
                    // ask MPV to drop if Display Queue is Empty
                    if (FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId) == 0)
                    {
                        // [LOG] AVSYNC, V-PTS Don't Drop (Display Queue Empty)
                        SYSLOG(SYNC_PREFIX + 40 + prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4DispOk, prFrcPrm->prFbCounter->u4DispFail, prFrcPrm->ucTargetNs);

                        FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SKIP_PIC);
                        LOG(3, "AVS-D VLD Vdp(%d) Fbg(%d) PTS(0x%x) STC(0x%x)\n", prFrcPrm->ucVdpId, prFrcPrm->ucFbgId, prFrcPrm->u4Pts, rStcClk.u4Base);

                        return;
                    }

                    prFrcPrm->prFbCounter->u4DispDrop++;

                    prFrcPrm->i4PtsDrift -= (INT32)u4OutFrameStc;

                    prFrcPrm->ucChangeFieldNs = 0;
                    prFrcPrm->ucTargetNs = 0;

                    // [LOG] AVSYNC, V-PTS Drop
                    SYSLOG(SYNC_PREFIX + 30 + prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4DispOk, prFrcPrm->prFbCounter->u4DispFail, prFrcPrm->ucTargetNs);
                    LOG(3, "AVS-D Vdp(%d) Fbg(%d) PTS(0x%x) STC(0x%x)\n", prFrcPrm->ucVdpId, prFrcPrm->ucFbgId, prFrcPrm->u4Pts, rStcClk.u4Base);
#endif
                }
            }
        }
        else if (prFrcPrm->ucReady == 0)
        {
            if (prFrcPrm->u4PtsAsyncNs < VDP_WAIT_1ST_PTS_NS)
            {
                // pending for the 1st PTS
                prFrcPrm->ucTargetNs = 0;
                prFrcPrm->ucChangeFieldNs = 0;

                prFrcPrm->u4InFrameStc = 0;
                prFrcPrm->u4OutFrameStc = 0;

                prFrcPrm->ucPendingFbId = prFrcPrm->ucFbId;
                prFrcPrm->ucLastFbId = FBM_FB_ID_UNKNOWN;
                prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;

                prFrcPrm->u4PtsAsyncNs++;
            }
            else
            {
                // STC still invalid after 12 vsync, forget it, go display!
                LOG(3, "Force Video Start 3\n");
                prFrcPrm->u4PtsAsyncNs = 0;
            }

            prFrcPrm->prFbCounter->u4SyncMode = FBM_FBG_AVSYNC_WAIT_1ST;
            FBM_SetAVSyncStatus(prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4SyncMode);
        }
        else
        {
#ifdef CC_GET_PCR_PTS_INFO
            UINT32 u4Delta;
            INT32 i4Delta;

            if (prFrcPrm->u4Pts > rStcClk.u4Base)
            {
                u4Delta = (prFrcPrm->u4Pts - rStcClk.u4Base);

                if (u4Delta < 0x80000000)
                {
                    i4Delta = - (INT32) u4Delta;
                }
                else
                {
                    // handle wrap around
                    u4Delta = (0xFFFFFFFF - u4Delta);
                    i4Delta = (INT32) u4Delta;
                }
            }
            else
            {
                u4Delta = (rStcClk.u4Base - prFrcPrm->u4Pts);

                if (u4Delta < 0x80000000)
                {
                    i4Delta = (INT32) u4Delta;
                }
                else
                {
                    // handle wrap around
                    u4Delta = (0xFFFFFFFF - u4Delta);
                    i4Delta = - (INT32) u4Delta;
                }
            }

            _prDbase->rPts.u4VideoPts = prFrcPrm->u4Pts;
            _prDbase->rPts.u4VideoPtsStcDelta = u4Delta;
            _prDbase->rPts.i4VideoPtsStcDelta = i4Delta;

            if (u4Delta >= 27000)     // 300ms
            {
                _prDbase->rPts.fgVideoSync = FALSE;
            }
            else
            {
                _prDbase->rPts.fgVideoSync = TRUE;
            }
#endif

            prFrcPrm->prFbCounter->u4SyncMode = FBM_FBG_AVSYNC_STC_INVALID;
            FBM_SetAVSyncStatus(prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4SyncMode);
        }
    }
    else
    {
#ifdef CC_GET_PCR_PTS_INFO
        STC_CLOCK rStcClk;

        if (STC_GetSrc(0, &rStcClk) == STC_VALID)
        {
            UINT32 u4Delta;

            if (prFrcPrm->u4Pts > rStcClk.u4Base)
            {
                u4Delta = (prFrcPrm->u4Pts - rStcClk.u4Base);
            }
            else
            {
                u4Delta = (rStcClk.u4Base - prFrcPrm->u4Pts);
            }

            _prDbase->rPts.u4VideoPts = prFrcPrm->u4Pts;
            _prDbase->rPts.u4VideoPtsStcDelta = u4Delta;
        }
#endif

        prFrcPrm->prFbCounter->u4SyncMode = FBM_FBG_AVSYNC_DISABLE;
        FBM_SetAVSyncStatus(prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4SyncMode);
    }
#endif
}

//-----------------------------------------------------------------------------
/** Brief of _VdpCheckOutputMode.
 */
//-----------------------------------------------------------------------------
static void _VdpCheckOutputMode(FRC_PRM_T* prFrcPrm)
{
    UCHAR ucVdpId;
    UCHAR ucPmxMode;
    UCHAR ucPlayMode;
    UCHAR ucProgressive;
    BOOL fgDoubleClock = FALSE;

    if (prFrcPrm == NULL)
    {
        return;
    }

    ucVdpId = prFrcPrm->ucVdpId;

    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    if(_fgTimeShift[0] && prFrcPrm->ucProgressiveSeqFlag)
    {
        fgDoubleClock = TRUE;
    }

    ucProgressive = prFrcPrm->ucProgressiveSeqFlag;

    FBM_GetPlayMode(prFrcPrm->ucFbgId, &ucPlayMode);
    if ((prFrcPrm->ucProgressiveSeqFlag==0) &&
            ((_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_24_) ||
             (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_24) ||
             (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_50) ||
             (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_60_) ||
             (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_60)) &&
            (prFrcPrm->ucProgressiveFrame!= 0) &&
            (ucPlayMode == FBM_FBG_MM_MODE))
    {
        LOG(3, "Interlace Seq but Progressive Pic\n");
        ucProgressive = 1;
    }


    if (ucProgressive)
    {
        // Progressive Source

        if (((prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_25) ||
                (prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_50)) &&
                (_prVdpConf[ucVdpId]->u4SrcWidth <= 720) &&
                (_prVdpConf[ucVdpId]->u4SrcHeight <= 576))
        {
#ifndef CC_B2R_ENABLE_OUTPUT_FRC
            if (prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_25)
            {
                ucPmxMode = VDP_B2R_MODE_576P_25;
            }
            else
#endif
            {
                ucPmxMode = VDP_B2R_MODE_576P_50;
            }

            if(fgDoubleClock)
            {
                if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_50)
                {
                    ucPmxMode = VDP_B2R_MODE_576P_100;
                }
                else if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_25)
                {
                    ucPmxMode = VDP_B2R_MODE_576P_50;
                }
            }
#ifdef CC_B2R_3D_SUPPROT
            if(prFrcPrm->fgB2R3DEnable && !prFrcPrm->fgB2RForce2D)
            {
                if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_50)
                {
                    ucPmxMode = VDP_B2R_MODE_576P_100;
                }
                else if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_25)
                {
                    ucPmxMode = VDP_B2R_MODE_576P_50;
                }
            }
#endif
        }
        else if ((_prVdpConf[ucVdpId]->u4SrcWidth <= 720) &&
                 (_prVdpConf[ucVdpId]->u4SrcHeight <= 480))
        {
#ifndef CC_B2R_ENABLE_OUTPUT_FRC
            if (prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_24)
            {
                ucPmxMode = VDP_B2R_MODE_480P_24;
            }
            else if (prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_30)
            {
                ucPmxMode = VDP_B2R_MODE_480P_30;
            }
            else
#endif
            {
                ucPmxMode = VDP_B2R_MODE_480P_60;
            }

            if(fgDoubleClock)
            {
                if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_24)
                {
                    ucPmxMode = VDP_B2R_MODE_480P_48;
                }
                else if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_60)
                {
                    ucPmxMode = VDP_B2R_MODE_480P_120;
                }
                else if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_30)
                {
                    ucPmxMode = VDP_B2R_MODE_480P_60;
                }
            }
#ifdef CC_B2R_3D_SUPPROT
            if(prFrcPrm->fgB2R3DEnable && !prFrcPrm->fgB2RForce2D)
            {
                if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_24)
                {
                    ucPmxMode = VDP_B2R_MODE_480P_48;
                }
                else if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_60)
                {
                    ucPmxMode = VDP_B2R_MODE_480P_120;
                }
                else if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_30)
                {
                    ucPmxMode = VDP_B2R_MODE_480P_60;
                }
            }
#endif
        }
		#ifdef CC_MT5396	
        else if ((_prVdpConf[ucVdpId]->u4SrcWidth <= 1280) &&   // 1408 = 1280 * 1.1
                 (_prVdpConf[ucVdpId]->u4SrcHeight <= 720) && (_prVdpConf[ucVdpId]->u4SrcHeight > 480))      // 792 = 720 * 1.1
        #else
		else if ((_prVdpConf[ucVdpId]->u4SrcWidth <= 1280) &&   // 1408 = 1280 * 1.1
                 (_prVdpConf[ucVdpId]->u4SrcHeight <= 720))      // 792 = 720 * 1.1
        #endif
        {
#ifndef CC_B2R_ENABLE_OUTPUT_FRC
            switch (prFrcPrm->ucInFrameRate)
            {
            case FBM_FRAME_RATE_24:
                ucPmxMode = VDP_B2R_MODE_720P_24;
                break;
            case FBM_FRAME_RATE_25:
                ucPmxMode = VDP_B2R_MODE_720P_25;
                break;
            case FBM_FRAME_RATE_30:
                ucPmxMode = VDP_B2R_MODE_720P_30;
                break;
            case FBM_FRAME_RATE_50:
                ucPmxMode = VDP_B2R_MODE_720P_50;
                break;
            case FBM_FRAME_RATE_60:
            default:
                ucPmxMode = VDP_B2R_MODE_720P_60;
                break;
            }
#else
            switch (prFrcPrm->ucInFrameRate)
            {
            case FBM_FRAME_RATE_25:
            case FBM_FRAME_RATE_50:
                ucPmxMode = VDP_B2R_MODE_720P_50;
                break;
            case FBM_FRAME_RATE_24:
            case FBM_FRAME_RATE_30:
            case FBM_FRAME_RATE_60:
            default:
                ucPmxMode = VDP_B2R_MODE_720P_60;
                break;
            }
#endif
            if(fgDoubleClock)
            {
                if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_60)
                {
                    ucPmxMode = VDP_B2R_MODE_720P_120;
                }
                else if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_50)
                {
                    ucPmxMode = VDP_B2R_MODE_720P_100;
                }
                else if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_24)
                {
                    ucPmxMode = VDP_B2R_MODE_720P_48;
                }
                else if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_25)
                {
                    ucPmxMode = VDP_B2R_MODE_720P_50;
                }
                else if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_30)
                {
                    ucPmxMode = VDP_B2R_MODE_720P_60;
                }
            }
#ifdef CC_B2R_3D_SUPPROT
            if(prFrcPrm->fgB2R3DEnable && !prFrcPrm->fgB2RForce2D)
            {
                if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_60)
                {
                    ucPmxMode = VDP_B2R_MODE_720P_120;
                }
                else if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_50)
                {
                    ucPmxMode = VDP_B2R_MODE_720P_100;
                }
                else if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_24)
                {
                    ucPmxMode = VDP_B2R_MODE_720P_48;
                }
                else if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_25)
                {
                    ucPmxMode = VDP_B2R_MODE_720P_50;
                }
                else if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_30)
                {
                    ucPmxMode = VDP_B2R_MODE_720P_60;
                }
            }
#endif

            if (_prVdpConf[ucVdpId]->u4SrcHeight > 720)
            {
                LOG(1, "Force Image Height %d >> 720\n", _prVdpConf[ucVdpId]->u4SrcHeight);
                _prVdpConf[ucVdpId]->u4SrcHeight = 720;
            }
            if (_prVdpConf[ucVdpId]->u4SrcWidth > 1280)
            {
                LOG(1, "Force Image Width %d >> 1280\n", _prVdpConf[ucVdpId]->u4SrcWidth);
                _prVdpConf[ucVdpId]->u4SrcWidth = 1280;
            }
        }
        else if ((_prVdpConf[ucVdpId]->u4SrcWidth <= 1440) &&
                 (_prVdpConf[ucVdpId]->u4SrcHeight <= 480))
        {
            switch (prFrcPrm->ucInFrameRate)
            {
            case FBM_FRAME_RATE_24:
                ucPmxMode = VDP_B2R_MODE_1440p_24;
                break;
            case FBM_FRAME_RATE_25:
            case FBM_FRAME_RATE_50:
            case FBM_FRAME_RATE_30:
            case FBM_FRAME_RATE_60:
            default:
                ucPmxMode = VDP_B2R_MODE_1440p_60;
                break;
            }
        }
        else if (((_prVdpConf[ucVdpId]->u4SrcWidth <= 1280) &&
                  (_prVdpConf[ucVdpId]->u4SrcHeight <= 1440)) ||
                 ((_prVdpConf[ucVdpId]->u4SrcWidth <= 1024) &&
                  (_prVdpConf[ucVdpId]->u4SrcHeight <= 1536)))
        {
            switch (prFrcPrm->ucInFrameRate)
            {
#ifdef CC_MT5396	//#ifdef CC_MT5396 add for CR DTV00351047 when mm video is 1024*768(tab) => v_total too large to do MJC 3D in mt5396
            case FBM_FRAME_RATE_24:
                ucPmxMode = (_prVdpConf[ucVdpId]->u4SrcHeight <= 768)? VDP_B2R_MODE_960p_24 : VDP_B2R_MODE_1280p_24;
                break;
            case FBM_FRAME_RATE_25:
                ucPmxMode = (_prVdpConf[ucVdpId]->u4SrcHeight <= 768)? VDP_B2R_MODE_960p_25 : VDP_B2R_MODE_1280p_25;
                break;
            case FBM_FRAME_RATE_50:
                ucPmxMode = (_prVdpConf[ucVdpId]->u4SrcHeight <= 768)? VDP_B2R_MODE_960p_50 : VDP_B2R_MODE_1280p_50;
                break;
            case FBM_FRAME_RATE_30:
                ucPmxMode = (_prVdpConf[ucVdpId]->u4SrcHeight <= 768)? VDP_B2R_MODE_960p_30 : VDP_B2R_MODE_1280p_60;
                break;
            case FBM_FRAME_RATE_60:
            default:
                ucPmxMode = (_prVdpConf[ucVdpId]->u4SrcHeight <= 768)? VDP_B2R_MODE_960p_60 : VDP_B2R_MODE_1280p_60;
                break;
#else
            case FBM_FRAME_RATE_24:
                ucPmxMode = VDP_B2R_MODE_1280p_24;
                break;
            case FBM_FRAME_RATE_25:
            case FBM_FRAME_RATE_50:
                ucPmxMode = VDP_B2R_MODE_1280p_50;
                break;
            case FBM_FRAME_RATE_30:
            case FBM_FRAME_RATE_60:
            default:
                ucPmxMode = VDP_B2R_MODE_1280p_60;
                break;
#endif
            }
        }

#ifdef CC_MT5396

        else if ((_prVdpConf[ucVdpId]->u4SrcWidth <= 1920) &&   //add for CR DTV00351047 when mm video is 1920*540(SBS) => v_total too large to do MJC 3D
                 (_prVdpConf[ucVdpId]->u4SrcHeight <= 720))
        {
            switch (prFrcPrm->ucInFrameRate)
            {
            case FBM_FRAME_RATE_24:
                ucPmxMode = VDP_B2R_MODE_960p_24;
                break;
            case FBM_FRAME_RATE_25:
                ucPmxMode = VDP_B2R_MODE_960p_25;
                break;
            case FBM_FRAME_RATE_50:
                ucPmxMode = VDP_B2R_MODE_960p_50;
                break;
            case FBM_FRAME_RATE_30:
                ucPmxMode = VDP_B2R_MODE_960p_30;
                break;
            case FBM_FRAME_RATE_60:
            default:
                ucPmxMode = VDP_B2R_MODE_960p_60;
                break;
            }
        }
        else if ((_prVdpConf[ucVdpId]->u4SrcWidth == 2560) &&   // 21:9
                 (_prVdpConf[ucVdpId]->u4SrcHeight == 1080))
        {
            ucPmxMode = VDP_B2R_MODE_21_9_30;
        }
#endif
        else
        {
            switch (prFrcPrm->ucInFrameRate)
            {
            case FBM_FRAME_RATE_25:
#ifndef CC_B2R_ENABLE_OUTPUT_FRC
                ucPmxMode = VDP_B2R_MODE_1080P_25;
                break;
#endif
            case FBM_FRAME_RATE_50:
                ucPmxMode = VDP_B2R_MODE_1080P_50;
                break;
            case FBM_FRAME_RATE_24:
#ifndef CC_B2R_ENABLE_OUTPUT_FRC
                ucPmxMode = VDP_B2R_MODE_1080P_24;
                break;
#endif
            case FBM_FRAME_RATE_30:
#ifndef CC_B2R_ENABLE_OUTPUT_FRC
                ucPmxMode = VDP_B2R_MODE_1080P_30;
                break;
#endif
            case FBM_FRAME_RATE_60:
            default:
                ucPmxMode = VDP_B2R_MODE_1080P_60;
                break;
            }

            if(fgDoubleClock)
            {
                if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_24)
                {
                    ucPmxMode = VDP_B2R_MODE_1080P_48;
                }
                else if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_30)
                {
                    ucPmxMode = VDP_B2R_MODE_1080P_60;
                }
                else if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_25)
                {
                    ucPmxMode = VDP_B2R_MODE_1080P_50;
                }
            }

#ifdef CC_B2R_3D_SUPPROT
            if(prFrcPrm->fgB2R3DEnable && !prFrcPrm->fgB2RForce2D)
            {
                if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_24)
                {
                    ucPmxMode = VDP_B2R_MODE_1080P_48;
                }
                else if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_30)
                {
                    ucPmxMode = VDP_B2R_MODE_1080P_60;
                }
                else if(prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_25)
                {
                    ucPmxMode = VDP_B2R_MODE_1080P_50;
                }
            }
#endif

            /*if (_prVdpConf[ucVdpId]->u4SrcHeight > 1080)
            {
                LOG(1, "Force Image Height %d >> 1080\n", _prVdpConf[ucVdpId]->u4SrcHeight);
                _prVdpConf[ucVdpId]->u4SrcHeight = 1080;
            }
            if (_prVdpConf[ucVdpId]->u4SrcWidth > 1920)
            {
                LOG(1, "Force Image Width %d >> 1920\n", _prVdpConf[ucVdpId]->u4SrcWidth);
                _prVdpConf[ucVdpId]->u4SrcWidth = 1920;
            }*/

            //add for new resolution
#if 0
            if( _prVdpConf[ucVdpId]->u4SrcWidth <1920 &&_prVdpConf[ucVdpId]->u4SrcHeight > 1080) // 1280x1440
            {
                switch (prFrcPrm->ucInFrameRate)
                {
                case FBM_FRAME_RATE_24:
                    ucPmxMode = VDP_B2R_MODE_1280p_24;
                case FBM_FRAME_RATE_25:
                case FBM_FRAME_RATE_50:
                case FBM_FRAME_RATE_30:
                case FBM_FRAME_RATE_60:
                default:
                    ucPmxMode = VDP_B2R_MODE_1280p_60;
                    break;
                }
            }
#endif
        }
    }
    else
    {
        // Interlace Source

        if (((prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_25) ||
                (prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_50)) &&
                (_prVdpConf[ucVdpId]->u4SrcWidth <= 720) &&
                (_prVdpConf[ucVdpId]->u4SrcHeight <= 576))
        {
            ucPmxMode = VDP_B2R_MODE_576I_50;
            if(fgDoubleClock || (prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_50))
            {
                ucPmxMode = VDP_B2R_MODE_576I_100;
            }
        }
        else if ((_prVdpConf[ucVdpId]->u4SrcWidth <= 720) &&
                 (_prVdpConf[ucVdpId]->u4SrcHeight <= 480))
        {
            ucPmxMode = VDP_B2R_MODE_480I_60;
            if(fgDoubleClock)
            {
                ucPmxMode = VDP_B2R_MODE_480I_120;
            }
        }
        else
        {
            if ((prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_25) ||
                    (prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_50))
            {
                // 50 Hz
                ucPmxMode = VDP_B2R_MODE_1080I_50;
            }
            else
            {
                // 60 Hz
                ucPmxMode = VDP_B2R_MODE_1080I_60;
            }


            if(fgDoubleClock)
            {
                if ((prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_25) ||
                        (prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_50))
                {
                    ucPmxMode = VDP_B2R_MODE_1080I_100;
                }
                else
                {
                    ucPmxMode = VDP_B2R_MODE_1080I_120;
                }
            }
            if(prFrcPrm->fgB2R3DEnable && !prFrcPrm->fgB2RForce2D)
            {
                if ((prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_25) ||
                        (prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_50))
                {
                    //todo
                }
                else
                {
                    ucPmxMode = VDP_B2R_MODE_1080I_120;
                }
            }

            if (_prVdpConf[ucVdpId]->u4SrcHeight > 1080)
            {
                LOG(1, "Force Image Height %d >> 1080\n", _prVdpConf[ucVdpId]->u4SrcHeight);
                _prVdpConf[ucVdpId]->u4SrcHeight = 1080;
            }
            if (_prVdpConf[ucVdpId]->u4SrcWidth > 1920)
            {
                LOG(1, "Force Image Width %d >> 1920\n", _prVdpConf[ucVdpId]->u4SrcWidth);
                _prVdpConf[ucVdpId]->u4SrcWidth = 1920;
            }
        }
    }

    prFrcPrm->ucNotSupport = 0;

    // B2R Output Mode Change
    if (ucPmxMode != _prVdpConf[ucVdpId]->rOutInfo.ucPmxMode)
    {
        LOG(3, "B2R Output Change %d >> %d\n",
            _prVdpConf[ucVdpId]->rOutInfo.ucPmxMode,
            ucPmxMode);

        _ucVsyncNotStableCount = 0xFF;

        switch (ucPmxMode)
        {
        case VDP_B2R_MODE_480I_60:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 0;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 60;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 720;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 480;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 858;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 525;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 1;
            if (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_30)
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_27_027;
            }
            else
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_27_000;
            }
            break;
        case VDP_B2R_MODE_480I_120:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 0;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 120;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 720;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 480;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 858;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 525;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 1;
            if (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_30)
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_54_054;
            }
            else
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_54_000;
            }
            break;

        case VDP_B2R_MODE_576I_50:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 0;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 50;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 720;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 576;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 864;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 625;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 1;
            _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_27_000;
            break;
        case VDP_B2R_MODE_576I_100:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 0;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 100;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 720;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 576;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 864;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 625;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 1;
            _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_54_000;
            break;
        case VDP_B2R_MODE_1080I_60:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 0;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 60;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1920;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 1080;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 2200;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 1125;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 1;
            if (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_30)
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_500;
            }
            else
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_352;
            }
            break;
        case VDP_B2R_MODE_1080I_50:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 0;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 50;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1920;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 1080;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 2640;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 1125;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 1;
            _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_500;
            break;
        case VDP_B2R_MODE_1080I_100:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 0;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 100;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1920;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 1080;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 2640;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 1125;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_500;
            break;

        case VDP_B2R_MODE_1080I_120:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 0;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 120;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1920;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 1080;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 2200;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 1125;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            if (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_30)
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_500;
            }
            else
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_352;
            }
            break;

        case VDP_B2R_MODE_480P_60:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 60;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 720;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 480;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 858;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 525;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            if ((_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_60) ||
                    (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_24) ||
                    (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_30)
               )
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_27_027;
            }
            else
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_27_000;
            }
            break;
        case VDP_B2R_MODE_576P_100:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 100;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 720;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 576;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 864;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 625;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_54_000;
            break;
        case VDP_B2R_MODE_576P_50:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 50;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 720;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 576;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 864;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 625;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_27_000;
            break;

        case VDP_B2R_MODE_720P_60:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 60;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1280;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 720;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 1650;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 750;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            if ((_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_60)
                    ||(_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_24)
                    ||(_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_30)
               )
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_250;
            }
            else
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_176;
            }
            break;

        case VDP_B2R_MODE_720P_48:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 48;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1280;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 720;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 1650;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 750;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 2;
            if (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_24)
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_500;
            }
            else
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_352;
            }
            break;
        case VDP_B2R_MODE_720P_50:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 50;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1280;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 720;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 1980;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 750;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_250;
            break;

        case VDP_B2R_MODE_480P_24:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 24;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 720;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 480;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 858;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 525;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 2;
            if (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_24)
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_27_027;
            }
            else
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_27_000;
            }
            break;

        case VDP_B2R_MODE_480P_30:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 30;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 720;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 480;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 858;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 525;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 1;
            if (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_30)
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_27_027;
            }
            else
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_27_000;
            }
            break;

        case VDP_B2R_MODE_576P_25:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 25;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 720;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 576;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 864;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 625;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 1;
            _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_27_000;
            break;

        case VDP_B2R_MODE_720P_24:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 24;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1280;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 720;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 1650;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 750;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 2;
            if (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_24)
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_250;
            }
            else
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_176;
            }
            break;

        case VDP_B2R_MODE_720P_25:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 25;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1280;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 720;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 1980;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 750;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 1;
            _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_250;
            break;

        case VDP_B2R_MODE_720P_30:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 30;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1280;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 720;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 1650;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 750;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 1;
            if (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_30)
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_250;
            }
            else
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_176;
            }
            break;
        case VDP_B2R_MODE_1440p_24:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 24;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1440;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 480;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 1716;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 525;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 2;
            if (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_24)
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_54_054;
            }
            else
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_54_000;
            }
            break;
        case VDP_B2R_MODE_1440p_60:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 60;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1440;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 480;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 1716;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 525;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            if ((_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_60)
                    ||(_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_24)
                    ||(_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_30)
               )
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_54_054;
            }
            else
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_54_000;
            }
            break;
        case VDP_B2R_MODE_1280p_24:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 24;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1280;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 1440;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 1500;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 1650;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 2;
            if (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_24)
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_500;
            }
            else
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_352;
            }
            break;
        case VDP_B2R_MODE_1280p_60:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 60;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1280;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 1440;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 1500;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 1650;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            if ((_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_60)
                    ||(_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_24)
                    ||(_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_30)
               )
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_500;
            }
            else
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_352;
            }
            break;
        case VDP_B2R_MODE_1280p_25:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 25;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1280;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 1440;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 1800;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 1650;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_250;
            break;            
        case VDP_B2R_MODE_1280p_50:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 50;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1280;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 1440;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 1800;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 1650;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_500;
            break;
        case VDP_B2R_MODE_1080P_24:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 24;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1920;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 1080;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 2750;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 1125;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            if (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_24)
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_250;
            }
            else
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_176;
            }
            break;

        case VDP_B2R_MODE_1080P_25:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 25;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1920;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 1080;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 2640;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 1125;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_250;
            break;

        case VDP_B2R_MODE_1080P_30:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 30;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1920;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 1080;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 2200;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 1125;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            if (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_30)
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_250;
            }
            else
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_176;
            }
            break;

            // Should we support 1080p 50/60?
        case VDP_B2R_MODE_1080P_50:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 50;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1920;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 1080;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 2640;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 1125;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_500;
            break;

#ifdef CC_MT5396
        case VDP_B2R_MODE_21_9_30:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 30;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 2560;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 1080;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 4400;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 1125;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_500;
            break;
        case VDP_B2R_MODE_960p_24:                         //add for CR DTV00351047 when mm video is 1920*540 => v_total too large to MJC
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 24;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1920;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 720;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 3648;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 848;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            if (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_24)
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_250;
            }
            else
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_176;
            }
            break;
        case VDP_B2R_MODE_960p_25:                        //add for CR DTV00351047 when mm video is 1920*540(SBS) => v_total too large to do MJC 3D
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 25;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1920;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 720;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 3502;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 848;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_250;
            break;
        case VDP_B2R_MODE_960p_30:                       //add for CR DTV00351047 when mm video is 1920*540(SBS) => v_total too large to do MJC 3D
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 30;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1920;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 720;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 2919;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 848;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            if (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_30)
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_250;
            }
            else
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_74_176;
            }
            break;
        case VDP_B2R_MODE_960p_50:                       //add for CR DTV00351047 when mm video is 1920*540(SBS) => v_total too large to do MJC 3D
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 50;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1920;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 720;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 3502;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 848;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_500;
            break;
        case VDP_B2R_MODE_960p_60:                       //add for CR DTV00351047 when mm video is 1920*540(SBS) => v_total too large to do MJC 3D
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 60;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1920;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 720;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 2919;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 848;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            if ((_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_60)
                    ||(_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_24)
                    ||(_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_30)
               )
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_500;
            }
            else
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_352;
            }
            break;
#endif

        case VDP_B2R_MODE_480P_48:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 48;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 720;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 480;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 858;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 525;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 3;
            if (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_24)
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_27_027;
            }
            else
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_27_000;
            }
            break;
        case VDP_B2R_MODE_480P_120:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 120;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 720;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 480;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 858;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 525;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            if ((_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_60) ||
                    (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_24) ||
                    (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_30)
               )
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_54_054;
            }
            else
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_54_000;
            }
            break;
        case VDP_B2R_MODE_720P_120:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 120;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1280;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 720;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 1650;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 750;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            if ((_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_60)
                    ||(_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_24)
                    ||(_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_30)
               )
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_500;
            }
            else
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_352;
            }
            break;
        case VDP_B2R_MODE_720P_100:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 100;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1280;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 720;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 1980;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 750;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_500;
            break;
        case VDP_B2R_MODE_1080P_48:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 48;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1920;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 1080;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 2750;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 1125;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            if (_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_24)
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_500;
            }
            else
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_352;
            }
            break;
            // Should we support 1080p 50/60?
        case VDP_B2R_MODE_1080P_60:
        default:
            _prVdpConf[ucVdpId]->rOutInfo.ucPrg = 1;
            _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate = 60;
            _prVdpConf[ucVdpId]->rOutInfo.u4Width = 1920;
            _prVdpConf[ucVdpId]->rOutInfo.u4Height = 1080;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalWidth = 2200;
            _prVdpConf[ucVdpId]->rOutInfo.u4TotalHeight = 1125;
            _prVdpConf[ucVdpId]->rOutInfo.ucDoubleClk = 0;
            if ((_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_60)
                    ||(_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_24)
                    ||(_prVdpConf[ucVdpId]->rOutInfo.ucInputFrameRate == MPEG_FRAME_RATE_30)
               )
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_500;
            }
            else
            {
                _prVdpConf[ucVdpId]->rOutInfo.u4ClkMode = VDP_B2R_CLK_148_352;
            }

            break;
        }

        _prVdpConf[ucVdpId]->rOutInfo.ucPmxMode = ucPmxMode;

        // Update vdp_frc parameter
        prFrcPrm->ucOutFrameRate = _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate;
        prFrcPrm->ucProgressiveOut = _prVdpConf[ucVdpId]->rOutInfo.ucPrg;

        if (prFrcPrm->ucOutFrameRate)
        {
            prFrcPrm->u4OutStcPeriod = VDP_STC_CLOCK / prFrcPrm->ucOutFrameRate;
        }
        else
        {
            prFrcPrm->u4OutStcPeriod = (VDP_STC_CLOCK / 60);
        }

        _VDP_HalStcPeriod(ucVdpId,prFrcPrm->u4OutStcPeriod);

        // notify SCPOS
        _VDP_VsyncNotify(VDP_MSG_NOTIFY, (UINT32)ucVdpId, VDP_B2R_OUTPUT_CHG, 0);

        _u4ReturnValue |= VDP_EVENT_SEQ_CHG;
    }
}

//-----------------------------------------------------------------------------
/** Brief of _VdpErrNotify.
 */
//-----------------------------------------------------------------------------
static void _VdpErrNotify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4)
{
    UNUSED(u4Arg1);
    UNUSED(u4Arg2);
    UNUSED(u4Arg3);
    UNUSED(u4Arg4);
}

//-----------------------------------------------------------------------------
/** Brief of _VdpFrcFrameBufferReady.
 */
//-----------------------------------------------------------------------------
void _VdpFrcFrameBufferReady(UCHAR ucFbgId)
{
    UCHAR ucIdx;
    UCHAR ucVdpId;
    UINT32 u4CurrentLine;

    // get video plane id
    ucVdpId = VDP_NS;
    for (ucIdx = 0; ucIdx < VDP_NS; ucIdx++)
    {
        if ((_arVdpPrm[ucIdx].rPortPrm.ucFbgId == ucFbgId) &&
                (_arVdpPrm[ucIdx].rPortPrm.ucPendingNewFb != 0))
        {
            ucVdpId = ucIdx;
            ucIdx = VDP_NS;
        }
    }
    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    if (FBM_ChkFrameBufferFlag(ucFbgId, FBM_FLAG_RELEASE_FBG))
    {
        return;
    }

    _VDP_VsyncMutexLock();

    // After Semaphore Wait, ucPendingNewFb can be reset by B2R VSYNC
    if (_arVdpPrm[ucVdpId].rPortPrm.ucPendingNewFb == 0)
    {
        _VDP_VsyncMutexUnlock();
        return;
    }

    // reset pending (do it once)
    _arVdpPrm[ucVdpId].rPortPrm.ucPendingNewFb = 0;

    // check active region (can we stilll update frame buffer now? or It is too late to update)
    u4CurrentLine = _VDP_HalInsideBlankRegion(ucVdpId);

    if (u4CurrentLine == 0)
    {
        // frame rate config core
        switch (_arVdpPrm[ucVdpId].ucMode)
        {
        case VDP_MODE_DEINT:
        case VDP_MODE_NORMAL:
        case VDP_MODE_BG:            // Background Mode, do nothing

            _arVdpPrm[ucVdpId].rPortPrm.ucNewFbArrive = 1;

#ifdef VDP_RACE_LOG
            VDP_FrcRacingLog('-');
#endif

            _VdpChangeFrameBuffer(&_arVdpPrm[ucVdpId].rPortPrm);

            ASSERT(_arVdpPrm[ucVdpId].rPortPrm.ucFbId != FBM_FB_ID_UNKNOWN);
//                LOG(2, "FBR (%d) (%d)\n", _arVdpPrm[ucVdpId].rPortPrm.ucFbId, _arVdpPrm[ucVdpId].rPortPrm.prFbCounter->u4DispFail);
            break;

        default:
            break;
        }
    }
    else
    {
//        LOG(1, "FB late (%d)\n", u4CurrentLine);
    }

    _VDP_VsyncMutexUnlock();
}


#ifdef CC_FRC_TEST
//-----------------------------------------------------------------------------
/** Brief of _VdpFrcTest.
 */
//-----------------------------------------------------------------------------
static void _VdpFrcTest(FRC_PRM_T* prFrcPrm)
{
    static UINT32 u4FrcTest = 0;

    if (prFrcPrm->ucReady == 0)
    {
        return;
    }

    u4FrcTest++;

#if 1
    if (u4FrcTest >= 128)
    {
        prFrcPrm->ucTargetNs = 20;
        prFrcPrm->ucChangeFieldNs = 10;

        u4FrcTest = 0;
    }
#else
    if ((u4FrcTest >= 32) && (prFrcPrm->ucFbId == 0))
    {
        HAL_TIME_T rResult;

        HAL_GetTime(&rResult);

        prFrcPrm->ucTargetNs = (rResult.u4Micros & 0xF);

        if (prFrcPrm->ucTargetNs <= 2)
        {
            prFrcPrm->ucTargetNs = 3;
        }

        prFrcPrm->ucChangeFieldNs = (rResult.u4Seconds % prFrcPrm->ucTargetNs);

        u4FrcTest = 0;
    }
#endif
}
#endif

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief of _VDP_FrcSetPtsCb.
 */
//-----------------------------------------------------------------------------
void _VDP_FrcSetPtsCb(UCHAR ucVdpId, UCHAR ucPort, UINT32 u4CbPts, UINT32 u4CbArg)
{
    UNUSED(ucPort);

    if (ucVdpId < VDP_NS)
    {
        _arVdpPrm[ucVdpId].rPortPrm.u4CbPtsEn = 1;
        _arVdpPrm[ucVdpId].rPortPrm.u4CbPts = u4CbPts;
        _arVdpPrm[ucVdpId].rPortPrm.u4CbArg = u4CbArg;
        _arVdpPrm[ucVdpId].rPortPrm.u4CbCntDown = 0;
        _arVdpPrm[ucVdpId].rPortPrm.u4CbLastPts = _arVdpPrm[ucVdpId].rPortPrm.u4Pts;

        return;
    }

    ASSERT(0);
}

static UINT32 _VDP_FrcGetStc(UCHAR ucFbgId)
{
    UCHAR ucAvSyncMode  = 0x0;
    UCHAR ucStcSrc      = 0x0;

    FBM_GetSyncStc(ucFbgId, &ucAvSyncMode, &ucStcSrc);

#ifdef CC_53XX_SWDMX_V2
    return STC_GetStcValue(ucStcSrc);
#else
    return STC_GetStcValue();
#endif
}


//-----------------------------------------------------------------------------
/** Brief of _VDP_FrcSetPtsCb.
 */
//-----------------------------------------------------------------------------
void _VDP_FrcSetLipSyncCb(UCHAR ucVdpId, UCHAR ucPort, UINT32 u4Thrsd, UINT32 u4FrmCnt)
{
    UNUSED(ucPort);

    if (ucVdpId < VDP_NS)
    {
        _arVdpPrm[ucVdpId].rPortPrm.u4CbSyncEn = 1;
        _arVdpPrm[ucVdpId].rPortPrm.u4CbSyncThrsd = (u4Thrsd * 90000);
        _arVdpPrm[ucVdpId].rPortPrm.u4CbSyncChkFrmCnt = u4FrmCnt;

        LOG(9, "(Thrsd: %d, FrmCnt:%d\n", u4Thrsd, u4FrmCnt);
        return;
    }

    ASSERT(0);
}

//-----------------------------------------------------------------------------
/** Brief of _VDP_FrcSetLipSyncNGOKCb.
 */
//-----------------------------------------------------------------------------
void _VDP_FrcSetLipSyncNGOKCb(UCHAR ucNgOk, UCHAR ucRemove, UINT32 u4Thrsd, UINT32 u4FrmCnt)
{
    if(ucRemove == 0)
    {
        if(ucNgOk == VDP_LIPSYNC_REG_NG_TO_OK)
        {
            _rVdpLipSync.ucNg2OkCbEn = 1;
            _rVdpLipSync.u4OKThrd = (u4Thrsd*90);
            _rVdpLipSync.u4NumFrmsOK = u4FrmCnt;
            _rVdpLipSync.u4OKFrmCnt = 0;
            _rVdpLipSync.ucLipSyncState = VDP_LIP_SYNC_NG;

            LOG(9, "Lip Sync NG to OK Thrd(%d), FrmCnt(%d)\n", u4Thrsd, u4FrmCnt);
        }
        else if(ucNgOk == VDP_LIPSYNC_REG_OK_TO_NG)
        {
            _rVdpLipSync.ucOk2NgCbEn = 1;
            _rVdpLipSync.u4NGThrd = (u4Thrsd*90);
            _rVdpLipSync.u4NumFrmsNG = u4FrmCnt;
            _rVdpLipSync.u4NGFrmCnt = 0;
            _rVdpLipSync.ucLipSyncState = VDP_LIP_SYNC_OK;

            LOG(9, "Lip Sync OK to NG Thrd(%d), FrmCnt(%d)\n", u4Thrsd, u4FrmCnt);
        }
        else
        {
            ASSERT(0);
        }
    }
    else
    {
        if(ucNgOk == VDP_LIPSYNC_REG_NG_TO_OK)
        {
            _rVdpLipSync.ucNg2OkCbEn = 0;
            LOG(9, "Remove Lip Sync NG to OK callback\n", u4Thrsd, u4FrmCnt);
        }
        else if(ucNgOk == VDP_LIPSYNC_REG_OK_TO_NG)
        {
            _rVdpLipSync.ucOk2NgCbEn = 0;
            LOG(9, "Remove Lip Sync OK to NG callback\n", u4Thrsd, u4FrmCnt);
        }
        else
        {
            ASSERT(0);
        }
        UNUSED(_rVdpLipSync);
    }
}


//-----------------------------------------------------------------------------
/** Brief of _VDP_FrcInit.
 */
//-----------------------------------------------------------------------------
void _VDP_FrcInit(void)
{
    UCHAR ucVdpId;
    CRIT_STATE_T _rCritState;

    _prDbase = DBS_Lock(&_rCritState);
    ASSERT(_prDbase);
    VERIFY(DBS_Unlock(_prDbase, _rCritState));

    for (ucVdpId = 0; ucVdpId < VDP_NS; ucVdpId++)
    {
        _prVdpConf[ucVdpId] = VDP_GetConf(ucVdpId);

        _arVdpPrm[ucVdpId].ucMode = VDP_MODE_BG;

        _VdpFrmInit(ucVdpId, 0);
    }

    /* FRC COPY parameter initialization */
    _rFrcCopy.u4TriggerCopy = 0;
    FRC_MEMSET((void *)&(_rFrcCopy.rPortPrm), 0, sizeof(FRC_PRM_T));

#ifdef CC_B2R_ENABLE_CHG_FRAME_MSG
    if(_hChgFrameQueue == (HANDLE_T)(NULL))
    {
        VERIFY(x_msg_q_create(&_hChgFrameQueue, "ChgFrameQ", sizeof(VDP_B2R_CHG_FRAME_MSG_T), 64) == OSR_OK);
    }
#endif

#ifdef CC_B2R_ENABLE_SEAMLESS_THREAD
    // create seamless playback thread
    if (!_fgSeamlessThreadInit)
    {
        _fgSeamlessThreadInit = TRUE;

        // create semaphore
        VERIFY(x_sema_create(&_hSeamlessSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

        VERIFY(x_thread_create(&_hSeamlessThread, "B2R_Sealmess_Thread", VSYNC_STACK_SIZE, VSYNC_THREAD_PRIORITY,
                               _SeamlessRoutine, 0, NULL)== OSR_OK);
    }
#endif
}

//-----------------------------------------------------------------------------
/** Brief of _VDP_FrcHandlerEvent.
 */
//-----------------------------------------------------------------------------
UINT32 _VDP_FrcHandlerEvent(UCHAR ucVdpId, UINT32 u4Event)
{
    FRC_PRM_T* _pFrcPrm;
    UCHAR ucFbgId;
    UCHAR ucOldFbgId;

    if (ucVdpId >= VDP_NS)
    {
        return 0;
    }

    // Handle configuration(input/output) changes
    u4Event &= (VDP_EVENT_MASTER_CHG |
                VDP_EVENT_INPUT_CHG |
                VDP_EVENT_OUTPUT_CHG |
                VDP_EVENT_MODE_CHG |
                VDP_EVENT_ENABLE_CHG);

#ifdef CC_SUPPORT_TVE
    // Handle  Main change from DTV to other source when TV Scart out.
    if ((_prVdpConf[VDP_1]->ucB2rId == B2R_1) &&
            (_prVdpConf[VDP_2]->ucB2rId == B2R_1)&&
            (_prVdpConf[VDP_1]->ucEnable == 0) &&
            (_prVdpConf[VDP_2]->ucEnable == 1) )
    {
        //LOG(0, "_VDP_FrcHandlerEvent _VDP_CopyFrcPrm\n\r");
        _rFrcCopy.u4TriggerCopy = 1;
        x_memcpy(&_rFrcCopy.rPortPrm,  &_arVdpPrm[VDP_1].rPortPrm, sizeof(FRC_PRM_T));
        //_VDP_CopyFrcPrm(VDP_1);
    }
#endif

    while (u4Event)
    {
        if (u4Event & VDP_EVENT_MASTER_CHG)
        {
            u4Event &= ~VDP_EVENT_MASTER_CHG;
        }

        if (u4Event & VDP_EVENT_ENABLE_CHG)
        {
            u4Event &= ~VDP_EVENT_ENABLE_CHG;

            if (!(_prVdpConf[ucVdpId]->ucEnable))
            {
                // clean port
                _pFrcPrm = &_arVdpPrm[ucVdpId].rPortPrm;

                // clean Fbg & VdpPrm
                if (_pFrcPrm->ucFbgId != FBM_FBG_ID_UNKNOWN)
                {

                    if ((_prVdpConf[VDP_1]->ucEnable == 0) &&
                            (_prVdpConf[VDP_2]->ucEnable == 0))
                    {
                        FBM_FlushLockFrameBuffer(_pFrcPrm->ucFbgId);
                        FBM_ClrFrameBufferFlag(_pFrcPrm->ucFbgId, FBM_FLAG_DISPLAYING);
                    }

                    _VdpFrmInit(ucVdpId, 0);

                    // Unlock Racing Semaphore
                    if ((_pFrcPrm->ucFbgId != FBM_FBG_ID_UNKNOWN) &&
                            FBM_ChkFrameBufferFlag(_pFrcPrm->ucFbgId, FBM_FLAG_RACING_HALF_B))
                    {
                        FBM_FbChangeField(_pFrcPrm->ucFbgId);
                    }
                }

                _VDP_HalForceBg(ucVdpId, 1);
            }

            // update input information
            u4Event |= VDP_EVENT_INPUT_CHG;

            // update output information
            u4Event |= VDP_EVENT_OUTPUT_CHG;

            // update mode information
            u4Event |= VDP_EVENT_MODE_CHG;

            LOG(7, "Enable change\n");
        }

        if (u4Event & VDP_EVENT_INPUT_CHG)
        {
            u4Event &= ~VDP_EVENT_INPUT_CHG;

            VERIFY(VDP_GetInput(ucVdpId, 0, &ucFbgId) == VDP_SET_OK);
            _pFrcPrm = &_arVdpPrm[ucVdpId].rPortPrm;

            if (ucFbgId != _pFrcPrm->ucFbgId)
            {
                ucOldFbgId = _pFrcPrm->ucFbgId;

                // clean old fbg
                if (ucOldFbgId != FBM_FBG_ID_UNKNOWN)
                {

                    if ((_prVdpConf[VDP_1]->ucEnable == 0) &&
                            (_prVdpConf[VDP_2]->ucEnable == 0))
                    {
                        FBM_FlushLockFrameBuffer(ucFbgId);
                    }

                    // Unlock Racing Semaphore
                    if (FBM_ChkFrameBufferFlag(ucOldFbgId, FBM_FLAG_RACING_HALF_B))
                    {
                        FBM_FbChangeField(ucOldFbgId);
                    }
                }

                // clean new fbg
                if (ucFbgId != FBM_FBG_ID_UNKNOWN)
                {

                    if ((_prVdpConf[VDP_1]->ucEnable == 0) &&
                            (_prVdpConf[VDP_2]->ucEnable == 0))
                    {
                        FBM_FlushLockFrameBuffer(ucFbgId);
                    }
                }

                _VdpFrmInit(ucVdpId, 0);
                _pFrcPrm->ucFbgId = ucFbgId;

                // update output information
                u4Event |= VDP_EVENT_OUTPUT_CHG;

                // update mode information
                u4Event |= VDP_EVENT_MODE_CHG;

                LOG(7, "Input change\n");
            }
        }

        if (u4Event & VDP_EVENT_MODE_CHG)
        {
            u4Event &= ~VDP_EVENT_MODE_CHG;

            _arVdpPrm[ucVdpId].ucMode = _prVdpConf[ucVdpId]->ucMode;

#if 0
            FBM_FlushLockFrameBuffer(_arVdpPrm[ucVdpId].rPortPrm.ucFbgId);

            VDP_FrcResetPort(ucVdpId, 0);

            // update output information
            u4Event |= VDP_EVENT_OUTPUT_CHG;
#endif
        }

        if (u4Event & VDP_EVENT_OUTPUT_CHG)
        {
#ifdef CC_SCPOS_EN
            u4Event &= ~VDP_EVENT_OUTPUT_CHG;
#else

            u4Event &= ~VDP_EVENT_OUTPUT_CHG;

            _arVdpPrm[ucVdpId].rPortPrm.ucOutFrameRate = _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate;
            _arVdpPrm[ucVdpId].rPortPrm.ucProgressiveOut = _prVdpConf[ucVdpId]->rOutInfo.ucPrg;

            if (_prVdpConf[ucVdpId]->rOutInfo.ucFrameRate)
            {
                _arVdpPrm[ucVdpId].rPortPrm.u4OutStcPeriod = VDP_STC_CLOCK / _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate;
            }
            else
            {
                _arVdpPrm[ucVdpId].rPortPrm.u4OutStcPeriod = (VDP_STC_CLOCK / 60);
            }

            LOG(7, "Output change (%d, %d, %d)\n", _prVdpConf[ucVdpId]->rOutInfo.ucPmxId, _prVdpConf[ucVdpId]->rOutInfo.ucFrameRate, _prVdpConf[ucVdpId]->rOutInfo.ucPrg);
#endif
        }
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of _VDP_FrcProc.
 */
//-----------------------------------------------------------------------------
UINT32 _VDP_FrcProc(UCHAR ucVdpId, UCHAR ucBottom, UCHAR ucRightView)
{
    FRC_PRM_T* prFrcPrm;

#ifdef ENABLE_MULTIMEDIA
    VDEC_ES_INFO_T *prVdecEsInfo;
#ifdef CC_B2R_SKYPE_SUPPORT
    //BOOL fgTVEEnable = FALSE;  // If TVEEnable then it is SCART out DTV
    prVdecEsInfo = _VDEC_GetEsInfo(ucVdpId);
#else
    prVdecEsInfo = _VDEC_GetEsInfo(ES0);
#endif
    UNUSED(prVdecEsInfo);
#endif

#ifdef __MODEL_slt__
    //HAL_TIME_T _rTime;
    //HAL_GetTime(&_rTime);
    //Printf("1-%06u\n", _rTime.u4Micros);
    b2rState = 1;
#endif

#if 0
#ifdef ENABLE_MULTIMEDIA
#ifdef CC_B2R_SKYPE_SUPPORT
    /*if(prVdecEsInfo != NULL)
    {
        if(prVdecEsInfo->eMMSrcType != SWDMX_SRC_TYPE_NETWORK_SKYPE)
        {
            ucVdpId = _B2R_TO_VDP(B2R_1); // Use THREAD VDP id pibben
            if (ucVdpId >= VDP_NS)
            {
                ASSERT(ucVdpId < VDP_NS);
                return 0;
            }
        }
    }*/
    //BOOL fgTVEEnable = FALSE;  // If TVEEnable then it is SCART out DTV
    //TVE_GetEnable(TVE_1, &fgTVEEnable);
    //if(!fgTVEEnable) // not SCART out case
    //{
    // do nothing
    //}
    if(_fgUseSubB2R)
    {
        // do nothing
    }
    else
#endif
    {
        ucVdpId = _B2R_TO_VDP(B2R_1); // Use THREAD VDP id pibben
        if (ucVdpId >= VDP_NS)
        {
            ASSERT(ucVdpId < VDP_NS);
            return 0;
        }
    }
#else
    ucVdpId = _B2R_TO_VDP(B2R_1); // Use THREAD VDP id pibben
    if (ucVdpId >= VDP_NS)
    {
        ASSERT(ucVdpId < VDP_NS);
        return 0;
    }
#endif
#endif

    if(ucVdpId >= VDP_NS)
    {
        return 0;
    }

    VDP_HalSetB2RMirrorFlip(ucVdpId);

    // clear return value
    _u4ReturnValue = 0;


    if((_arVdpPrm[ucVdpId].rPortPrm.fgB2R3DEnable) && (!_arVdpPrm[ucVdpId].rPortPrm.fgB2RForce2D)
            /*&& (_arVdpPrm[ucVdpId].rPortPrm.u1B2R3DType == B2R_3D_FRAME_SEQUENTIAL)*/
            && _arVdpPrm[ucVdpId].rPortPrm.ucProgressiveOut ==1)
    {
        if(ucRightView ==1)
        {
            LOG(10,"FrcProc R ");
        }
        else
        {
            LOG(10,"FrcProc L ");
        }

        LOG(10,"%d\n",_arVdpPrm[ucVdpId].rPortPrm.ucCurrentVsyncNs);

        if((_arVdpPrm[ucVdpId].rPortPrm.ucCurrentVsyncNs & 0x1) == 1)
        {
#ifdef CC_B2R_SKYPE_SUPPORT
            VDP_HalSetLRState(ucVdpId,FALSE);
#else
            VDP_HalSetLRState(FALSE);
#endif
        }
        else
        {
#ifdef CC_B2R_SKYPE_SUPPORT
            VDP_HalSetLRState(ucVdpId,TRUE);
#else
            VDP_HalSetLRState(TRUE);
#endif
        }
    }

#if 1//def CC_B2R_3D_MVC_INTERLACE_SUPPORT
    else if(_arVdpPrm[ucVdpId].rPortPrm.fgB2R3DEnable && !_arVdpPrm[ucVdpId].rPortPrm.fgB2RForce2D
            && _arVdpPrm[ucVdpId].rPortPrm.u1B2R3DType == B2R_3D_FRAME_SEQUENTIAL
            && _arVdpPrm[ucVdpId].rPortPrm.ucProgressiveOut ==0)
    {

        //VDP_HalSet3DMode(ucVdpId,B2R_3D_MODE_MANUAL_SET);
        if(_arVdpPrm[ucVdpId].rPortPrm.ucCurrentVsyncNs == 1 || _arVdpPrm[ucVdpId].rPortPrm.ucCurrentVsyncNs==3 || _arVdpPrm[ucVdpId].rPortPrm.ucCurrentVsyncNs==5)
        {
#ifdef CC_B2R_SKYPE_SUPPORT
            VDP_HalSetLRState(ucVdpId,FALSE);
#else
            VDP_HalSetLRState(FALSE);
#endif
        }
        else
        {
#ifdef CC_B2R_SKYPE_SUPPORT
            VDP_HalSetLRState(ucVdpId,TRUE);
#else
            VDP_HalSetLRState(TRUE);
#endif
        }

        if(_arVdpPrm[ucVdpId].rPortPrm.ucCurrentVsyncNs ==0)
        {
#ifdef CC_B2R_SKYPE_SUPPORT
            VDP_HalSetTBState(ucVdpId,TRUE);
#else
            VDP_HalSetTBState(TRUE);
#endif
        }
        /*if(_arVdpPrm[ucVdpId].rPortPrm.ucCurrentVsyncNs ==0 && _arVdpPrm[ucVdpId].rPortPrm.ucReady == 1)
        {
            if(!((ucRightView==1) && (ucBottom==1)))
            {
                return 0;
            }
        }*/

    }
#endif

    // Frame Rate Control Process is NOT required if NOT enable
#ifdef CC_SUPPORT_TVE
    if ((!(_prVdpConf[ucVdpId]->ucEnable)) && (!(_prVdpConf[1-ucVdpId]->ucEnable)))
    {
        return _u4ReturnValue;
    }
#else
    if (!(_prVdpConf[ucVdpId]->ucEnable))
    {
        return _u4ReturnValue;
    }
#endif

#ifdef VDP_RACE_LOG
    VDP_FrcRacingLog('s');
#endif

    // backup the current field information
    _arVdpPrm[ucVdpId].ucBottom = ucBottom;

    // reset pending
    _arVdpPrm[ucVdpId].rPortPrm.ucPendingNewFb = 0;

    prFrcPrm = &_arVdpPrm[ucVdpId].rPortPrm;

#ifdef CC_B2R_EMULATION
    _u4InterruptCount++;
    _VdpSetCrcSequence(prFrcPrm);
#endif

    if (_prVdpConf[ucVdpId]->ucEarlyDisp == 1)
    {
        if (prFrcPrm->u4WaitUnmute != 0)
        {
            prFrcPrm->u4WaitUnmute--;
        }
        else
        {
            _vDrvVideoSetMute(MUTE_MODULE_DTV, ucVdpId, 0, FALSE);
            _prVdpConf[ucVdpId]->ucEarlyDisp = 0;

            LOG(3, "DTV Unmute Early Display -1 VDP(%d)\n", ucVdpId);

#ifdef CC_SUPPORT_TVE
            /* DTV SCART out application, Main and Sub are both attached to B2R */
            /* 1 Sub is connected */
            /* 2 Sub's ucEarlyDisp is set */
            /* 3 Main is connected */
            /* In above case, sub(SCART out) is never un-mute */
            /* 20081120: While Main's ucEarlyDisp, Sub's will also be set for Mute */
            if (_prVdpConf[1-ucVdpId]->ucEarlyDisp == 1)
            {
                _vDrvVideoSetMute(MUTE_MODULE_DTV, (1 - ucVdpId), 0, FALSE);
                LOG(3, "DTV Unmute Early Display -2 VDP(%d)\n", ucVdpId);
                _prVdpConf[1-ucVdpId]->ucEarlyDisp = 0;
            }
#endif

#if 0
            {
                STC_CLOCK rStcClk;

                STC_GetSrc(0, &rStcClk);
                LOG(1, "DTV Unmute PTS(0x%x) STC(0x%x)\n", prFrcPrm->u4Pts, rStcClk.u4Base);
            }
#endif
        }
    }

    // FBG pending release
    if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_RELEASE_FBG))
    {
        FBM_ReleaseDispQ(prFrcPrm->ucFbgId);

        if (prFrcPrm->ucReady != 0)
        {
            LOG(1, "FBM_FLAG_RELEASE_FBG\n");

            if (prFrcPrm->ucReleaseFbId != FBM_FB_ID_UNKNOWN)
            {
                FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucReleaseFbId, FBM_FB_STATUS_EMPTY);
                prFrcPrm->ucReleaseFbId = FBM_FB_ID_UNKNOWN;
            }

            if (prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN)
            {
                FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_EMPTY);
                prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
            }

            prFrcPrm->ucReleased = 1;
            prFrcPrm->ucReady = 0;

            _VDP_HalForceBg(ucVdpId, 1);

            _VDP_VsyncNotify(VDP_MSG_NOTIFY, (UINT32)ucVdpId, VDP_B2R_NO_SIGNAL, 0);
        }

        return _u4ReturnValue;
    }

    // frame rate config core
    switch (_arVdpPrm[ucVdpId].ucMode)
    {
    case VDP_MODE_DEINT:        // Deinterlace Mode, Temp
    case VDP_MODE_NORMAL:        // Normal Mode
    case VDP_MODE_BG:            // Background Mode, do nothing
        // reference 8105 main_vdo.c (vMVdoDispProc)
        // single input port at normal mode, master port only

        // call back protection
        if ((prFrcPrm->u4CbPtsEn) && (prFrcPrm->ucReady))
        {
            if (++(prFrcPrm->u4CbCntDown) >= VDP_VSYNC_CB_THD)
            {
                _VDP_VsyncNotify(VDP_MSG_CC_CB, (UINT32)prFrcPrm->ucVdpId, prFrcPrm->u4CbPts, (prFrcPrm->u4CbArg | VDP_CB_ERR));

                prFrcPrm->u4CbPtsEn = 0;

                LOG(5, "P1(%x)\n", prFrcPrm->u4CbPts);
            }
        }

        // check fbm flag
        if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_CLEAR_FRC_PRM))
        {
            FBM_ClrFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_CLEAR_FRC_PRM);
            VDP_FrcResetPort(ucVdpId, 0);
        }

        // Increase Current Vsync Ns here or at ISR ?
        FRC_INC_VSYNC_NS(prFrcPrm);

        // 5381 Double Frame, Delay 1 VSync to free frame buffer
        // release last frame buffer immediately
        if (prFrcPrm->ucReleaseFbId != FBM_FB_ID_UNKNOWN)
        {
            if (prFrcPrm->ucReleased == 0)
            {
                FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucReleaseFbId, FBM_FB_STATUS_EMPTY);

#ifdef VDP_RACE_LOG
                VDP_FrcRacingLog('f');
                VDP_FrcRacingLog('0' + prFrcPrm->ucReleaseFbId);
#endif
            }
            prFrcPrm->ucReleased = 0;

            prFrcPrm->ucReleaseFbId = FBM_FB_ID_UNKNOWN;
        }

        // Half B Racing, Early Free B Frame Buffer
        if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_RACING_HALF_B))
        {
            // NOTE!!! Release prFrcPrm->ucFbId here instead of prFrcPrm->ucReleaseFbId
            if ((prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN) &&
                    (prFrcPrm->ucPicCdTp == MPEG_PIC_TYPE_B))
            {
                //// TODO!!! Early Free at VEI in the future
                if (prFrcPrm->ucReleased == 0)
                {
                    FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_EMPTY);

#ifdef VDP_RACE_LOG
                    VDP_FrcRacingLog('#');
                    VDP_FrcRacingLog('f');
                    VDP_FrcRacingLog('0' + prFrcPrm->ucFbId);
#endif
                }

                prFrcPrm->ucReleased = 1;
            }
        }

#ifdef VDP_B2R_PSCAN_CRC_FLOW
        if ((prFrcPrm->u4Pts==_u4SetPts) &&
                (((prFrcPrm->ucProgressiveOut == 0) && (FRC_CHANGE_FIELD(prFrcPrm))) ||
                 ((prFrcPrm->ucProgressiveOut != 0) && (prFrcPrm->ucCurrentVsyncNs == 1)))
           )
        {
            _rVdpB2rCrc.u4DramChkSum = u4DRAMChkSum;
            _rVdpB2rCrc.u4PixelChkSum = u4PixelChkSum;
            _rVdpB2rCrc.u4PSCANSrcCrc = u4PSCANSrcCrc;
            _rVdpB2rCrc.u4PSCANCrc = u4PSCANCrc;

            LOG(1, "(PTS, Bd, Bp, PcSrc, Pc) = (%08X, %08X, %08X, %08X, %08X)\n", prFrcPrm->u4Pts,
                _rVdpB2rCrc.u4DramChkSum, _rVdpB2rCrc.u4PixelChkSum, _rVdpB2rCrc.u4PSCANSrcCrc, _rVdpB2rCrc.u4PSCANCrc);
        }
#endif
        if (FRC_DISP_FINISH(prFrcPrm))
        {
            // Full B Racing, Early Free B Frame Buffer
            if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_TRACKING_MODE_ENABLE|FBM_FLAG_RACING_FULL_B))
            {
                // NOTE!!! Release prFrcPrm->ucFbId here instead of prFrcPrm->ucReleaseFbId
                if ((prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN) &&
                        ((prFrcPrm->ucPicCdTp == MPEG_PIC_TYPE_B) || (prFrcPrm->fgTrackingMode)))
                {
                    //// TODO!!! Early Free at VEI in the future
                    if (prFrcPrm->ucReleased == 0)
                    {
                        FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_EMPTY);

#ifdef VDP_RACE_LOG
                        VDP_FrcRacingLog('#');
                        VDP_FrcRacingLog('f');
                        VDP_FrcRacingLog('0' + prFrcPrm->ucFbId);
#endif
                    }

                    prFrcPrm->ucReleased = 1;
                }
            }
            else if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_RACING_HALF_B) &&
                     (prFrcPrm->ucPicCdTp == MPEG_PIC_TYPE_B))
            {
                // Half B, Last Field Display, Tell MPV no need to decode next field
                FBM_SetFbNextFieldPlayBackNs(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, 0);

#ifdef VDP_RACE_LOG
                VDP_FrcRacingLog('n');
                VDP_FrcRacingLog('0');
#endif

                // Half B, Last Field Display, Unlock Semaphore
                FBM_FbChangeField(prFrcPrm->ucFbgId);
            }

            _VdpChangeFrameBuffer(prFrcPrm);

#ifdef VDP_B2R_PSCAN_CRC_FLOW
            if (prFrcPrm->u4Pts == _u4SetPts)
            {
                /* Calculate PSCAN CRC at specified frame */
#if !(defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389))
                // Clear PSCAN CRC
                //IO_WRITE32(PSCAN_BASE, 0x05C, (IO_READ32(PSCAN_BASE, 0x05C)&0xDFFFFFFFF)|0x20000000);
                vDrvDISetCRCClr(1);
                // Trigger PSCAN CRC
                //IO_WRITE32(PSCAN_BASE, 0x05C, (IO_READ32(PSCAN_BASE, 0x05C)&0xEFFFFFFFF)|0x10000000);
                vDrvDISetCRCTrigger(1);
                LOG(2, "Trigger PSCAN CRC\n");
#endif

            }
#endif

#if 0 //def __KERNEL__
            if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_DEBLOCKING))
            {
                if (_prVdpConf[ucVdpId]->rOutInfo.ucPmxMode >= VDP_B2R_MODE_1080I)
                {
                    // 1080i/1080p only
                    // increase performance
                    // trigger VEI to free frame buffer
                    _VDP_HalSetRacingMode(ucVdpId, VDP_RACING_MODE_VEI, 0);
                }
            }
#endif

            if (prFrcPrm->ucReady == 0)
            {
                if ((prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN) &&
                        (prFrcPrm->ucNotSupport == 0))
                {
                    UCHAR ucAvSyncMode;
                    UCHAR ucStcSrc;
#ifdef TIME_MEASUREMENT
                    TMS_DIFF_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME, "B2R ready to play 1st Frame");
#endif

                    _VDP_HalForceBg(ucVdpId, 0);
                    prFrcPrm->ucReady = 1;

                    FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);

#ifndef CC_B2R_WAIT_NPTV_STABLE
                    FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_DISPLAYING);
#else
                    if( FBM_FBG_DTV_MODE == _u1PlayMode)
                    {
                        FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_DISPLAYING);
                    }
#endif
                    _VDP_VsyncNotify(VDP_MSG_UNMUTE_CB, (UINT32)ucVdpId, 1, 0);

                    if (_arVdpDtvFreeze[ucVdpId].u4OnOff != 0)
                    {
                        FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_FREEZE);
                        FBM_ReleaseDispQ(prFrcPrm->ucFbgId);
                    }

#ifdef TIME_MEASURE
                    if (prFrcPrm->prFbCounter)
                    {
                        HAL_TIME_T rResult;

                        HAL_GetDeltaTime(&rResult, &(_prDbase->rDmx.rEnableTime), &(prFrcPrm->prFbCounter->rStartTime));

                        LOG(1, "Video Time Measure (%d.%06d)\n", rResult.u4Seconds, rResult.u4Micros);
                    }
#endif


                    // notify SCPOS
                    _VDP_VsyncNotify(VDP_MSG_NOTIFY, (UINT32)ucVdpId, VDP_B2R_START_PLAY, 0);
                }
                else
                {
                    _VDP_HalForceBg(ucVdpId, 1);
                    FBM_ClrFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_DISPLAYING);
                }
            }
        }
#if 0       //// TODO, Performance Analysis, VLD/MC 會跑過頭, 480i at DDR2x2 Platform
        else if ((FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_RACING_FULL_B)) &&
                 (FRC_DISP_NS(prFrcPrm) == 1) &&
                 (prFrcPrm->ucPicCdTp == MPEG_PIC_TYPE_B))
        {
            // last vsync play, trigger VEI to free B frame buffer
            // increase performance
            _VDP_HalSetRacingMode(ucVdpId, VDP_RACING_MODE_VEI, 0);
        }
#endif
        else if ((FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_RACING_HALF_B)) &&
                 (FRC_CHANGE_FIELD(prFrcPrm)) &&
                 (prFrcPrm->ucPicCdTp == MPEG_PIC_TYPE_B))
        {
            // Half B, Last Field Display, Tell MPV to decode next field
            FBM_SetFbNextFieldPlayBackNs(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, 1);

#ifdef VDP_RACE_LOG
            VDP_FrcRacingLog('n');
            VDP_FrcRacingLog('1');
#endif

            // Half B, Last Field Display, Unlock Semaphore
            FBM_FbChangeField(prFrcPrm->ucFbgId);
        }
        break;

    default:
        break;
    }

#ifdef VDP_RACE_LOG
    // Print Racing Log
    VDP_FrcRacingLog((CHAR) 255);
#endif

#ifdef __MODEL_slt__
//HAL_GetTime(&_rTime);
//Printf("3-%06u\n", _rTime.u4Micros);
    b2rState = 0xff;
#endif

    return _u4ReturnValue;
}

//-----------------------------------------------------------------------------
/** Brief of _VDP_FrcProcEarly.
 */
//-----------------------------------------------------------------------------
void _VDP_FrcProcEarly(UCHAR ucVdpId, UCHAR ucBottom)
{
    FRC_PRM_T* prFrcPrm;

    UNUSED(ucBottom);

    if (ucVdpId >= VDP_NS)
    {
        ASSERT(ucVdpId < VDP_NS);
        return;
    }
    // Frame Rate Control Process is NOT required if NOT enable
    if (!(_prVdpConf[ucVdpId]->ucEnable))
    {
        return;
    }

#ifdef VDP_RACE_LOG
    VDP_FrcRacingLog('i');
#endif

    prFrcPrm = &_arVdpPrm[ucVdpId].rPortPrm;

    // FBG pending release
    if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_RELEASE_FBG))
    {
        if (prFrcPrm->ucReady != 0)
        {
            LOG(1, "FBM_FLAG_RELEASE_FBG\n");

            if (prFrcPrm->ucReleaseFbId != FBM_FB_ID_UNKNOWN)
            {
                FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucReleaseFbId, FBM_FB_STATUS_EMPTY);
                prFrcPrm->ucReleaseFbId = FBM_FB_ID_UNKNOWN;
            }

            if (prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN)
            {
                FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_EMPTY);
                prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
            }

            prFrcPrm->ucReleased = 1;
            prFrcPrm->ucReady = 0;

            _VDP_HalForceBg(ucVdpId, 1);

            _VDP_VsyncNotify(VDP_MSG_NOTIFY, (UINT32)ucVdpId, VDP_B2R_NO_SIGNAL, 0);
        }

        return;
    }

    // frame rate config core
    switch (_arVdpPrm[ucVdpId].ucMode)
    {
    case VDP_MODE_DEINT:        // Deinterlace Mode, Temp
    case VDP_MODE_NORMAL:        // Normal Mode
    case VDP_MODE_BG:            // Background Mode, do nothing

        if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_TRACKING_MODE_ENABLE|FBM_FLAG_RACING_FULL_B))
        {
            // Release B FB at Last Display
            if ((prFrcPrm->ucPicCdTp == MPEG_PIC_TYPE_B) || (prFrcPrm->fgTrackingMode))
            {
                // NOTE!!! Release prFrcPrm->ucFbId here instead of prFrcPrm->ucReleaseFbId
                if ((FRC_DISP_NS(prFrcPrm) == 1) &&
                        (prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN) &&
                        (prFrcPrm->ucReleased == 0))
                {
                    FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_EMPTY);

                    prFrcPrm->ucReleased = 1;

#ifdef VDP_RACE_LOG
                    VDP_FrcRacingLog('f');
                    VDP_FrcRacingLog('0' + prFrcPrm->ucFbId);
#endif
                }
            }
        }
        else if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_DEBLOCKING))
        {
            if (prFrcPrm->ucReleaseFbId != FBM_FB_ID_UNKNOWN)
            {
                if (prFrcPrm->ucReleased == 0)
                {
                    FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucReleaseFbId, FBM_FB_STATUS_EMPTY);

#ifdef VDP_RACE_LOG
                    VDP_FrcRacingLog('f');
                    VDP_FrcRacingLog('0' + prFrcPrm->ucReleaseFbId);
#endif
                }
                prFrcPrm->ucReleased = 0;

                prFrcPrm->ucReleaseFbId = FBM_FB_ID_UNKNOWN;
            }
        }

        break;

    default:
        break;
    }
}

//-----------------------------------------------------------------------------
/** Brief of _VDP_FrcGetRacingMode.
 */
//-----------------------------------------------------------------------------

UINT32 _VDP_FrcGetRacingMode(UCHAR ucVdpId)
{
    if (ucVdpId >= VDP_NS)
    {
        ASSERT(ucVdpId < VDP_NS);
        return 0;
    }

    if (FBM_ChkFrameBufferFlag(_arVdpPrm[ucVdpId].rPortPrm.ucFbgId, (FBM_FLAG_TRACKING_MODE_ENABLE|FBM_FLAG_RACING_HALF_B|FBM_FLAG_RACING_FULL_B)))
    {
        return 1;
    }

    return 0;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief of VDP_FrcResetPort.
 */
//-----------------------------------------------------------------------------
void VDP_FrcResetPort(UCHAR ucVdpId, UCHAR ucPort)
{
    FRC_PRM_T rVdpPrm;

    UNUSED(ucPort);

    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    // backup those we don't want to reset
    rVdpPrm = _arVdpPrm[ucVdpId].rPortPrm;

    _VdpFrmInit(ucVdpId, 0);

    // restore
    _arVdpPrm[ucVdpId].rPortPrm.ucFbgId = rVdpPrm.ucFbgId;
    _arVdpPrm[ucVdpId].rPortPrm.ucOutFrameRate = rVdpPrm.ucOutFrameRate;
    _arVdpPrm[ucVdpId].rPortPrm.u4OutStcPeriod = rVdpPrm.u4OutStcPeriod;
    _arVdpPrm[ucVdpId].rPortPrm.ucProgressiveOut = rVdpPrm.ucProgressiveOut;

    _arVdpPrm[ucVdpId].rPortPrm.prFbCounter = rVdpPrm.prFbCounter;

    _arVdpPrm[ucVdpId].rPortPrm.u4CbPtsEn = rVdpPrm.u4CbPtsEn;
    _arVdpPrm[ucVdpId].rPortPrm.u4CbPts = rVdpPrm.u4CbPts;
    _arVdpPrm[ucVdpId].rPortPrm.u4CbArg = rVdpPrm.u4CbArg;

    _arVdpPrm[ucVdpId].rPortPrm.u4CbSyncEn = rVdpPrm.u4CbSyncEn;
    _arVdpPrm[ucVdpId].rPortPrm.u4CbSyncThrsd= rVdpPrm.u4CbSyncThrsd;
    _arVdpPrm[ucVdpId].rPortPrm.u4CbSyncFrmCnt= rVdpPrm.u4CbSyncFrmCnt;


    // clean FBM_FLAG_DISPLAYING at next VSYNC
    FBM_ClrFrameBufferFlag(rVdpPrm.ucFbgId, FBM_FLAG_DISPLAYING);
}

//-----------------------------------------------------------------------------
/** Brief of VDP_FrcSetFreeze.
 */
//-----------------------------------------------------------------------------

void VDP_FrcSetFreeze(UCHAR ucVdpId, UINT32 u4OnOff)
{
    if (ucVdpId >= VDP_NS)
    {
        ASSERT(ucVdpId < VDP_NS);
        return;
    }

#ifdef CC_B2R_GFX_COPY_FIELD
    // mutex
    VERIFY (x_sema_lock(_rVdpGfxCopyField.hMutex, X_SEMA_OPTION_WAIT) == OSR_OK);
#endif

    if (u4OnOff != _arVdpDtvFreeze[ucVdpId].u4OnOff)
    {
        UCHAR ucFbgId;

        ucFbgId = _arVdpPrm[ucVdpId].rPortPrm.ucFbgId;

        if (ucFbgId != FBM_FBG_ID_UNKNOWN)
        {
            if (u4OnOff != 0)
            {
                FBM_SetFrameBufferFlag(ucFbgId, FBM_FLAG_FREEZE);

//                FBM_ReleaseDispQ(ucFbgId);
            }
            else
            {
                FBM_ClrFrameBufferFlag(ucFbgId, FBM_FLAG_FREEZE);
            }
        }

        if (u4OnOff == 0)
        {
            _arVdpDtvFreeze[ucVdpId].u4Recovery = 1;
            _arVdpDtvFreeze[ucVdpId].u4UnFreeze= 1;
        }
        else
        {
            _arVdpDtvFreeze[ucVdpId].u4Recovery = 0;
            _arVdpDtvFreeze[ucVdpId].u4UnFreeze= 0;
        }

        _arVdpDtvFreeze[ucVdpId].u4CleanDispQ = 0;
        _arVdpDtvFreeze[ucVdpId].u4CopyField = 0;
        _arVdpDtvFreeze[ucVdpId].u4OnOff = u4OnOff;
    }

#ifdef CC_B2R_GFX_COPY_FIELD
    // mutex
    VERIFY (x_sema_unlock(_rVdpGfxCopyField.hMutex) == OSR_OK);
#endif
}

BOOL VDP_FrcGetUnMuteStatus(UCHAR ucVdpId)
{
    if (ucVdpId < VDP_NS)
    {
        if (_arVdpPrm[ucVdpId].rPortPrm.ucReady != 0)
        {
            return TRUE;
        }
    }

    return FALSE;
}

UCHAR VDP_FrcGetLockFrameBuffer(UINT8 u1B2rId)
{
    UCHAR ucLockFbId;

    _VDP_VsyncMutexLock();
    if (u1B2rId < B2R_NS)
    {
        ucLockFbId = _arVdpPrm[u1B2rId].rPortPrm.ucFbId;
    }
    else
    {
        ucLockFbId = FBM_FB_ID_UNKNOWN;
    }
    _VDP_VsyncMutexUnlock();
    return ucLockFbId;
}


VOID _VDP_SetCurrentTick(UCHAR ucVdpId, UINT32 u4Tick)
{
    FBM_PIC_HDR_T* prLockPicHdr;
    UCHAR ucFbgId;
    UCHAR ucLockFbId;

    ucFbgId = _arVdpPrm[ucVdpId].rPortPrm.ucFbgId;
    ucLockFbId = _arVdpPrm[ucVdpId].rPortPrm.ucFbId;

    if ((ucLockFbId != FBM_FB_ID_UNKNOWN) && (ucFbgId  != FBM_FBG_ID_UNKNOWN))
    {
        prLockPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, ucLockFbId);

        if (prLockPicHdr != NULL)
        {
            prLockPicHdr->u4TickNum = u4Tick;
        }
    }
}

UINT32 _VDP_GetCurrentPts(UINT8 u1B2rId)
{
    FBM_PIC_HDR_T* prLockPicHdr;
    UCHAR ucFbgId;
    UCHAR ucLockFbId;

    ucFbgId = _arVdpPrm[u1B2rId].rPortPrm.ucFbgId;
    ucLockFbId = _arVdpPrm[u1B2rId].rPortPrm.ucFbId;

    if ((ucLockFbId != FBM_FB_ID_UNKNOWN) && (ucFbgId  != FBM_FBG_ID_UNKNOWN))
    {
        prLockPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, ucLockFbId);

        if (prLockPicHdr != NULL)
        {
            return prLockPicHdr->u4PTS;
        }
        else
        {
            return 0;
        }
    }

    return 0;
}

UINT32 _VDP_GetLastPts(UINT8 u1B2rId)
{
    return _arVdpPrm[u1B2rId].rPortPrm.u4LastPts;
}

UINT32 _VDP_GetDispingPts(UINT8 u1B2rId)
{
    return _arVdpPrm[u1B2rId].rPortPrm.u4Pts;
}

UCHAR _VDP_GetOutFrameRate(UINT8 u1B2rId)
{
    return _arVdpPrm[u1B2rId].rPortPrm.ucOutFrameRate;
}

UCHAR _VDP_GetFbg(UINT8 u1B2rId)
{
    if ( u1B2rId >= B2R_NS )
    {
        return FBM_FBG_ID_UNKNOWN;
    }

    return _arVdpPrm[u1B2rId].rPortPrm.ucFbgId;
}

UINT32 _VDP_GetFilePosition(UINT8 u1B2rId,
                            INT32 *pi4TempRef, UINT16 *pu2DecodingOrder,
                            UINT64 *pu8Offset, UINT64 *pu8OffsetI)
{
    FBM_PIC_HDR_T* prLockPicHdr;
    UCHAR ucFbgId;
    UCHAR ucLockFbId;

    ucFbgId = _arVdpPrm[u1B2rId].rPortPrm.ucFbgId;
    ucLockFbId = _arVdpPrm[u1B2rId].rPortPrm.ucFbId;

    if ((ucLockFbId != FBM_FB_ID_UNKNOWN) && (ucFbgId  != FBM_FBG_ID_UNKNOWN))
    {
        prLockPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, ucLockFbId);
        if (prLockPicHdr != NULL)
        {
            *pi4TempRef = prLockPicHdr->i4TemporalRef;
            *pu2DecodingOrder = prLockPicHdr->u2DecodingOrder;
            *pu8Offset = prLockPicHdr->u8Offset;
            *pu8OffsetI = prLockPicHdr->u8OffsetI;
            return VDP_SET_OK;
        }
        else
        {
            *pi4TempRef = 0;
            *pu2DecodingOrder = 0;
            *pu8Offset = 0;
            *pu8OffsetI = 0;
        }
    }
    else
    {
        *pi4TempRef = 0;
        *pu2DecodingOrder = 0;
        *pu8Offset = 0;
        *pu8OffsetI = 0;
    }
    return VDP_SET_ERROR;
}


UINT32 _VDP_GetPosInfo(UINT8 u1B2rId, VDP_POS_INTO_T *prPosInfo)
{
    FBM_PIC_HDR_T* prLockPicHdr;
    UCHAR ucFbgId;
    UCHAR ucLockFbId;


    if(!prPosInfo)
    {
        return VDP_SET_ERROR;
    }
    if (u1B2rId >= B2R_NS)
    {
        return VDP_SET_ERROR;
    }

    x_memset(prPosInfo, 0, sizeof(VDP_POS_INTO_T));

    ucFbgId = _arVdpPrm[u1B2rId].rPortPrm.ucFbgId;
    ucLockFbId = _arVdpPrm[u1B2rId].rPortPrm.ucFbId;

    if ((ucLockFbId != FBM_FB_ID_UNKNOWN) && (ucFbgId  != FBM_FBG_ID_UNKNOWN))
    {
        prLockPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, ucLockFbId);
        if (prLockPicHdr != NULL)
        {
            prPosInfo->i4TemporalRef = prLockPicHdr->i4TemporalRef;
            prPosInfo->u2DecodingOrder = prLockPicHdr->u2DecodingOrder;
            prPosInfo->u8Stc    = (UINT64)_VDP_FrcGetStc(ucFbgId);
            prPosInfo->u8Offset = prLockPicHdr->u8Offset;
            prPosInfo->u8OffsetI = prLockPicHdr->u8OffsetI;
            prPosInfo->u8OffsetDisp = prLockPicHdr->u8OffsetDisp;
            prPosInfo->u8Pts = (UINT64)prLockPicHdr->u4PTS;
            prPosInfo->u8PtsI = prLockPicHdr->rExtra.u8PTSI;
            prPosInfo->u8AudPts = prLockPicHdr->rExtra.u8AudPTS;
            prPosInfo->u8AudOffset = prLockPicHdr->rExtra.u8AudOffset;
            prPosInfo->u4Timestap = prLockPicHdr->u4Timestap;

            //DTV00316878 for avi file same PTS value when AB repeat in trick mode
            if(((_eSpeed[u1B2rId] >= STC_SPEED_TYPE_FORWARD_4X) &&
                    (_eSpeed[u1B2rId] <= STC_SPEED_TYPE_FORWARD_1024X))||
                    ((_eSpeed[u1B2rId] >= STC_SPEED_TYPE_REWIND_1X) &&
                     (_eSpeed[u1B2rId] <= STC_SPEED_TYPE_REWIND_1024X)))
            {
                prPosInfo->u8Stc  = prPosInfo->u8Pts;
            }
            LOG(0,"prPosInfo->u8Stc = %llu\n", prPosInfo->u8Stc);
            return VDP_SET_OK;
        }
    }
    return VDP_SET_ERROR;
}

UINT32 _VDP_SetABInfo(UINT8 u1B2rId, VDP_AB_INTO_T *prAB)
{
    if(!prAB)
    {
        return VDP_SET_ERROR;
    }
    if (u1B2rId >= B2R_NS)
    {
        return VDP_SET_ERROR;
    }
    x_memcpy(&_arVdpPrm[u1B2rId].rPortPrm.rAB, prAB, sizeof(VDP_AB_INTO_T));
    _VDPSetABRepeat(u1B2rId, prAB->fgValid);
    return VDP_SET_OK;
}

UCHAR _VDP_GetPendingFB(UINT8 u1B2rId)
{
    return _arVdpPrm[u1B2rId].rPortPrm.ucPendingFbId;
}

void _VDP_CleanPendingFB(UINT8 u1B2rId)
{
    _arVdpPrm[u1B2rId].rPortPrm.ucPendingFbId = FBM_FB_ID_UNKNOWN;
    _arVdpPrm[u1B2rId].rPortPrm.fgReleaseDispQ = TRUE;
}

void _VDP_SetReleaseDispQ(UINT8 u1B2rId)
{
    _arVdpPrm[u1B2rId].rPortPrm.fgReleaseDispQ = TRUE;
}

void _VDP_CopyFrcPrm(UCHAR ucVdpId)
{
    PARAM_VDP_FB_COUNTER_T* pTempFbCounter;
    UCHAR ucTempVdpId;
    UCHAR ucTempReady;
    struct _VDP_PRM_T* pTempVdpPrm;

    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    /* for correct dbs print message */
    pTempFbCounter = _arVdpPrm[1 - ucVdpId].rPortPrm.prFbCounter;

    /* Should NOT be copied */
    ucTempVdpId = _arVdpPrm[1 - ucVdpId].rPortPrm.ucVdpId;
    pTempVdpPrm = _arVdpPrm[1 - ucVdpId].rPortPrm.pVdpPrm;

    /* DTV SCART out, mute/unmute mechanism */
    ucTempReady = _arVdpPrm[1 - ucVdpId].rPortPrm.ucReady;

    if (_rFrcCopy.u4TriggerCopy == 1)
    {
        x_memcpy( &_arVdpPrm[1 - ucVdpId].rPortPrm,  &_rFrcCopy.rPortPrm, sizeof(FRC_PRM_T));
        _rFrcCopy.u4TriggerCopy = 0;
    }
    else
    {
        x_memcpy( &_arVdpPrm[1 - ucVdpId].rPortPrm,  &_arVdpPrm[ucVdpId].rPortPrm, sizeof(FRC_PRM_T));
    }

    /* for correct dbs print message */
    _arVdpPrm[1 - ucVdpId].rPortPrm.prFbCounter = pTempFbCounter;

    /* Should NOT be copied */
    _arVdpPrm[1 - ucVdpId].rPortPrm.ucVdpId = ucTempVdpId;
    _arVdpPrm[1 - ucVdpId].rPortPrm.pVdpPrm = pTempVdpPrm;

    /* Should be cleared */
    _arVdpPrm[ucVdpId].rPortPrm.ucPendingNewFb = 0; /* Check _VdpFrcFrameBufferReady */

    /* DTV SCART out, mute/unmute mechanism */
    if ((ucTempReady == 0) && (_arVdpPrm[1 - ucVdpId].rPortPrm.ucReady == 1))
    {
        /* ready from 0 to 1 */
        _vDrvVideoSetMute(MUTE_MODULE_DTV, (1-ucVdpId), 0, FALSE);
    }
}

/* VDP trigger mode + audio master to do trigger mode */
void _VDPTriggerAudReceive(UINT32 u4Pts)
{
    _rVdpTrigAud.ucTriggerReceived = 1;
    _rVdpTrigAud.u4TriggerPts = u4Pts;
}

static void _VdpTriggerAudOn(UINT8 u1B2rId, UINT32 u4CurPts)
{
    if (_rVdpTrigAud.ucTriggerReceived == 1)
    {
        if (u4CurPts >= _rVdpTrigAud.u4TriggerPts)
        {
            LOG(3, "VDP Trigger Aud: CurPTS(%X), SetPTS(%X)\n", u4CurPts, _rVdpTrigAud.u4TriggerPts);

            // _VDP_EventNofify(VDP_B2r2Vdp(u1B2rId), (UINT32)VDP_CB_FUNC_PLAY_DONE_IND, 0, 0);

            /* AUD trigger function */
#ifdef ENABLE_MULTIMEDIA
#ifdef CC_53XX_SWDMX_V2
            if(u1B2rId < B2R_NS)
            {
                UNUSED(SWDMX_SetInfo(_arVdpPrm[u1B2rId].rPortPrm.u4AttachedSrcId, eSWDMX_SET_AOUT_ENABLE, 0, 0, 0));
            }
            else
            {
                ASSERT(u1B2rId < B2R_NS);
            }
#else
            UNUSED(SWDMX_SetInfo(eSWDMX_SET_AOUT_ENABLE, 0, 0, 0));
#endif
#else
            // actually this is no meaning.
            AUD_MMAoutEnable(0, TRUE);
#endif

            _rVdpTrigAud.ucTriggerReceived = 0;
        }
    }
}

/* VDP Seek done notification */
void _VDPSetSeek(UCHAR ucVdpId, UCHAR ucSet, UINT32 u4Pts)
{
    UNUSED(ucVdpId);

    _rVdpSeek.u4SeekPts = u4Pts;
    _rVdpSeek.ucNotifyTrigger = 1;
    _rVdpSeek.ucSeekMode = 1;
}

/* VDP AB Repeat done notification */
void _VDPSetABRepeat(UCHAR ucVdpId, UCHAR ucSet)
{
    UNUSED(ucVdpId);

    if((ucSet) &&
            (_arVdpPrm[ucVdpId].rPortPrm.rAB.fgValid))
    {
        _rVdpSeek.ucABNotifyTrigger = 1;
        _rVdpSeek.fgABReach = FALSE;
        LOG(3, "ucABNotifyTrigger ENABLE\n");
    }
    else
    {
        _rVdpSeek.ucABNotifyTrigger = 0;
        _rVdpSeek.fgABReach = FALSE;
    }
}

static void _VDPGetSeek(UCHAR* pucSeekMode, UINT32* pu4Pts, UCHAR* pucNotifyTrigger)
{
    *pu4Pts = _rVdpSeek.u4SeekPts;
    *pucNotifyTrigger =  _rVdpSeek.ucNotifyTrigger;
    *pucSeekMode = _rVdpSeek.ucSeekMode;
}

/* VDP Set Wait STC while starts */
void _VDPSetWaitSTC(UCHAR ucVdpId, UCHAR ucSet)
{
    UNUSED(ucVdpId);

    if (ucSet != 0)
    {
        _fgEnableWaitSTC = TRUE;
    }
    else
    {
        _fgEnableWaitSTC = FALSE;
    }
}

/* VDP Get Wait STC while starts */
UCHAR _VDPGetWaitSTC(UCHAR ucVdpId)
{
    UNUSED(ucVdpId);

    if (_fgEnableWaitSTC)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

VOID _VDP_FrcStcSpeed(UINT8 u1B2rId,STC_SPEED_TYPE_T eSpeed)
{
    if(u1B2rId < B2R_NS)
    {
        _eSpeed[u1B2rId] = eSpeed;
    }
    else
    {
        ASSERT(0);
    }
}

STC_SPEED_TYPE_T _VDP_GetFrcStcSpeed(UINT8 u1B2rId)
{
    if(u1B2rId < B2R_NS)
    {
        return _eSpeed[u1B2rId];
    }
    else
    {
        ASSERT(0);
        return STC_SPEED_TYPE_FORWARD_1X;
    }
}

#ifdef VDP_B2R_PSCAN_CRC_FLOW
/* B2R CRC check on PTS value */
void _VDPSetChkPTS(UINT32 u4Pts)
{
    _u4SetPts = u4Pts;

    /* Reset status */
    _rVdpB2rCrc.u4DramChkSum = 0;
    _rVdpB2rCrc.u4PixelChkSum = 0;
    _rVdpB2rCrc.u4PSCANCrc = 0;
    _rVdpB2rCrc.u4PSCANSrcCrc = 0;
    LOG(3, "Set PTS (%d)\n", _u4SetPts);
}

void _VDP_SetEncTrigger(BOOL fgEnable)
{
    _fgTriggerEnc = fgEnable;
    UNUSED(_fgTriggerEnc);
}

#endif

//-----------------------------------------------------------------------------
/** Brief of _VdpPreLookNextDisp.
 */
//-----------------------------------------------------------------------------
static UCHAR _VdpPreLookNextDisp(FRC_PRM_T* prFrcPrm)
{
    return FBM_PreLookFrameBufferFromDispQ(prFrcPrm->ucFbgId);
}

static void _VDP_FrcQueryStatus(UINT32 u4B2rId)
{
    if (u4B2rId >= B2R_NS)
    {
        return;
    }

    LOG(1, "B2R(%d) 3D_E(%d) 3D_F2D(%d) 3D_T(%d) 3D_Offset(%d, %d)\n",
        u4B2rId,
        _arVdpPrm[u4B2rId].rPortPrm.fgB2R3DEnable,
        _arVdpPrm[u4B2rId].rPortPrm.fgB2RForce2D,
        _arVdpPrm[u4B2rId].rPortPrm.u1B2R3DType,
        _arVdpPrm[u4B2rId].rPortPrm.u4B2R3DWidthOffset,
        _arVdpPrm[u4B2rId].rPortPrm.u4B2R3DHeightOffset);

}
#ifdef VDP_RACE_LOG
//-----------------------------------------------------------------------------
/** Brief of VDP_FrcRacingLog.
 */
//-----------------------------------------------------------------------------
void VDP_FrcRacingLog(CHAR cLog)
{
#define VDP_RACE_LOG_POOL 128
#define VDP_RACE_LOG_BOUND (VDP_RACE_LOG_POOL - 64)
    static CHAR szDebug[VDP_RACE_LOG_POOL];
    static UINT32 u4DebugIdx = 0;
    static UINT32 u4Print = 0;
    static HANDLE_T hMutex = NULL;

    if (hMutex == NULL)
    {
        // create semaphore
        VERIFY(x_sema_create(&hMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);
    }

    VERIFY(x_sema_lock(hMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    // Print
    if (cLog == (CHAR)255)
    {
        if (u4DebugIdx >= VDP_RACE_LOG_BOUND)
        {
            szDebug[u4DebugIdx] = 0;
            if (u4Print != 0)
            {
                LOG(1, "%s\n", szDebug);
            }
            u4DebugIdx = 0;
            u4Print = 0;
        }

        VERIFY(x_sema_unlock(hMutex) == OSR_OK);

        return;
    }

    if ((cLog < 'a') ||(cLog > 'z'))
    {
        // If MPEG ready, print
        u4Print = 1;
    }

    if (u4DebugIdx < VDP_RACE_LOG_POOL)
    {
        szDebug[u4DebugIdx++] = cLog;
    }

#if 0
    // Full B
    do
    {
        static UINT32 u4Step = 0;
        // b >> B >> b >> F >> s

        switch (u4Step)
        {
        case 0:
            if (cLog == 'b')
            {
                u4Step = 1;
            }
            break;

        case 1:
            if (cLog == 'B')
            {
                u4Step = 2;
            }
            else if ((cLog == 'r') || (cLog == 'R'))
            {
                u4Step = 0;
            }
            break;

        case 2:
            if (cLog == 'b')
            {
                u4Step = 3;
            }
            else if ((cLog == 'r') || (cLog == 'R'))
            {
                u4Step = 0;
            }
            break;

        case 3:
            if (cLog == 'F')
            {
                // perfect
                u4Step = 0;

//            szDebug[u4DebugIdx++] = 'O';
            }
            else if ((cLog == 's') || (cLog == 'r') || (cLog == 'R') || (cLog == 'b'))
            {
                // fail
                u4Step = 0;

                szDebug[u4DebugIdx++] = '!';
            }
            break;


        default:
            u4Step = 0;
            break;
        }

    } while (0);
#endif

#if 0
    // Decode Ns Counter
    do
    {
        static UINT32 u4Step = 0;
        static UINT32 u4Sync = 0;

        switch (u4Step)
        {
        case 0:
            if ((cLog == 'B') ||(cLog == 'R'))
            {
                u4Step = 1;
                u4Sync = 0;
            }
            break;

        case 1:
            if (cLog == 'F')
            {
                u4Step = 0;

                szDebug[u4DebugIdx++] = '0' + u4Sync;
            }
            else if (cLog == 's')
            {
                u4Sync++;
            }
            break;

        default:
            u4Step = 0;
            break;
        }
    } while (0);

#endif

    VERIFY(x_sema_unlock(hMutex) == OSR_OK);
}
#endif

VDP_PRM_T* _VDP_GetVdpPrm(UINT8 u1B2rId)
{
    if (u1B2rId < VDP_NS)
    {
        return &(_arVdpPrm[u1B2rId]);
    }
    else
    {
        return NULL;
    }
}

VOID _VDP_SetPauseMM(UINT8 u1B2rId,BOOL fgPauseMM)
{
    if(u1B2rId < B2R_NS)
    {
        _fgPauseMM[u1B2rId] = fgPauseMM;
    }
    else
    {
        ASSERT(0);
    }
}

VOID _VDP_Set3DInfo(UINT8 u1B2rId,UINT8 u13DType,BOOL fg3DEnable)
{
    _arVdpPrm[u1B2rId].rPortPrm.fgB2R3DEnable = fg3DEnable;
    _arVdpPrm[u1B2rId].rPortPrm.u1B2R3DType = u13DType;
}

void VDP_FrcQueryStatus(void)
{
    UCHAR ucB2rId;

    for (ucB2rId = 0; ucB2rId < B2R_NS; ucB2rId++)
    {
        _VDP_FrcQueryStatus(ucB2rId);
    }
}

BOOL _VDP_IsStartToPlay(UCHAR u1B2rId)
{
#ifdef CC_B2R_WAIT_NPTV_STABLE
    if(_arVdpPrm[u1B2rId].rPortPrm.ucFbgId != 0xFF)
    {
        FRC_PRM_T* prFrcPrm = &_arVdpPrm[0].rPortPrm;
        FBM_GetPlayMode(_arVdpPrm[u1B2rId].rPortPrm.ucFbgId,&_u1PlayMode);

        if(prFrcPrm != NULL)
        {
            if(_u1PlayMode == FBM_FBG_MM_MODE && !_fgNPTVStable && !prFrcPrm->fgB2R3DEnable && (prFrcPrm->ucReady ==1) && _u4DrvVideoGetMute(prFrcPrm->ucVdpId))
            {
                return FALSE;
            }
            else
            {
                return TRUE;
            }
        }
        else
        {
            return TRUE;
        }
    }
    else
    {
        return TRUE;
    }
#else
    return TRUE;
#endif
}
VOID _VDP_SetTimeShiftSpeed(UINT8 u1B2rId,UINT32 u4SpeedValue)
{

    if(u1B2rId < B2R_NS)
    {
        _i4TimeShiftSpeed[u1B2rId] = u4SpeedValue;
    }
    else
    {
        LOG(3,"warning u1B2rId >= B2R_NS\n");
        _i4TimeShiftSpeed[0] = u4SpeedValue;
    }
    UNUSED(_i4TimeShiftSpeed);// make lint happy
}

VOID _VDP_SetTimeShiftMode(UINT8 u1B2rId,BOOL fgEnable)
{
    if(u1B2rId < B2R_NS)
    {
        _fgTimeShift[u1B2rId] = fgEnable;
    }
    else
    {
        LOG(3,"warning u1B2rId >= B2R_NS\n");
        _i4TimeShiftSpeed[0] = fgEnable;
    }
    UNUSED(_fgTimeShift);
    UNUSED(_i4TimeShiftSpeed);
}

UINT32 _VDP_TrickModeSkipFrameNo(UCHAR ucB2rId)
{
    return _arVdpPrm[ucB2rId].rPortPrm.u4B2rSkipFrameNo;
}


#ifdef CC_B2R_ENABLE_CHG_FRAME_MSG
BOOL _VDP_SendB2RChgFrameMsg(VDP_B2R_CHG_FRAME_MSG_T* prMsg)
{
    VDP_B2R_CHG_FRAME_MSG_T rMsg;
    x_memset(&rMsg,0,sizeof(VDP_B2R_CHG_FRAME_MSG_T));

    rMsg.u4VdpId = prMsg->u4VdpId;

    if (x_msg_q_send(_hChgFrameQueue, (void *)(&rMsg), sizeof(VDP_B2R_CHG_FRAME_MSG_T), 0) != OSR_OK)
    {
        LOG(5,"Msg Queue full for change frame buffer Msg queue\n");
        return FALSE;
    }

    //_VdpFrcFrameBufferReady((_arVdpPrm[0].rPortPrm).ucFbgId);

    return TRUE;
}

BOOL _VDP_FlushB2RChgFrameMsg(VOID)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    UINT16 u2Cnt;
    VDP_B2R_CHG_FRAME_MSG_T rMsg;

    zMsgSize = sizeof(VDP_B2R_CHG_FRAME_MSG_T);
    if(_hChgFrameQueue != (HANDLE_T)(NULL))
    {
        while (x_msg_q_receive(&u2MsgQIdx, &rMsg, &zMsgSize,
                               &(_hChgFrameQueue), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
        {
        }
        VERIFY(x_msg_q_num_msgs(_hChgFrameQueue, &u2Cnt) == OSR_OK);
        ASSERT(u2Cnt== 0);
    }
    return TRUE;
}
#endif
