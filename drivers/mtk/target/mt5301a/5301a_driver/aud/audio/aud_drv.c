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
 * $RCSfile: aud_drv.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_drv.c
 *  Brief of file aud_drv.c.
 *  Details of file aud_drv.c (optional).
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
// KERNEL



LINT_EXT_HEADER_BEGIN
#include "x_util.h"
#include "x_typedef.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_printf.h"
#include "x_assert.h"
#include "aud_drvif.h"
#include "x_hal_926.h"
#include "x_ckgen.h"
#include "x_pinmux.h"
//#include "x_pinmux.h"

#include "aud_debug.h"
#include "psr_drvif.h"
#include "drv_common.h"
//#include "drv_dsp_cfg.h"
#include "drv_adsp.h"
//#include "drv_stc.h"
#include "stc_drvif.h"
//#include "drv_slt.h"
#include "mpv_drvif.h"
#include "drvcust_if.h"
#include "drv_dbase.h"

#include "aud_drv.h"
#include "aud_if.h"
#include "spdif_drvif.h"
#include "aud_hw.h"
#include "aud_cfg.h"
#include "dsp_intf.h"
#include "aud_detect.h"
#include "aud_notify.h"
#include "adac_if.h"

#include "../adsp/include_API/dsp_uop.h"
#include "../adsp/include_API/dsp_common.h"
#ifdef ENABLE_MULTIMEDIA
#include "swdmx_if.h"
#endif

#ifdef CC_ENABLE_AV_SYNC
#include "srm_drvif.h"
#include "vdo_misc.h"
#include "sv_const.h"
#endif
LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

#define USE_16BIT_ALIGNMENT

#define CC_AUD_SUPPORT_MM_AV_INIT_SYNC

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define ADRV_CMD_Q_NAME1        "AUD CMDQ1"
#define ADRV_CMD_Q_NAME2        "AUD CMDQ2"
#ifdef CC_MT5391_AUD_3_DECODER
#define ADRV_CMD_Q_NAME3        "AUD CMDQ3"
#endif
#define ADRV_CMD_Q_DUAL_DEC     "AUD_DUAL_DEC_CMD_Q"
#define ADRV_APLL_CMD_Q         "AUD_APLL_CMD_Q"
#define AUD_DRV_DUAL_DEC_THREAD_NAME "AUD_DUAL_THREAD"
#define AUD_DRV_DTV_MAIN_THREAD_NAME "AUD_DTV_THREAD_MAIN"
#define AUD_DRV_DTV_AUX_THREAD_NAME "AUD_DTV_THREAD_AUX"
#ifdef CC_MT5391_AUD_3_DECODER
#define AUD_DRV_DTV_THIRD_THREAD_NAME "AUD_DTV_THREAD_THIRD"
#endif
#define AUD_CMD_QUEUE_SIZE      32

#define PES_CNT_THRESHOLD       50
// Light: DTV takes longer time to wait for I frame.
#define PES_CNT_THRES_I_FRAME   200
#define PES_VALID_CNT           3
#define PTS_THRESHOLD           2800
#define PTS_AC3_CNT             2880
#define PTS_THRESHOLD_MAX       ((UINT32)(PTS_AC3_CNT*60))
#define PTS_THRESHOLD_NOT_VALID       ((UINT32)(PTS_AC3_CNT*2000))
#define PTS_DRIFT_MAX           ((UINT32)12000)

//#ifdef CC_AUD_DVBT_SUPPORT
//#define PTS_DRIFT_MAX           12000
//#endif

#define TRANSFER_SZ             (UINT32)(0x00000400) 

#define AUD_EVENT_TIMEOUT       (UINT32)(1500*0.75)
// Light: Since DTV needs to wait for 1st I frame, it takes longer time to wait for the event.
#define AUD_EVENT_TIMEOUT_DTV    (UINT32)(4500)//(UINT32)(1600)
// Laihui: MM wait longer than DTV, for some file have error data in head
#define AUD_EVENT_TIMEOUT_MM    (UINT32)(8500)

#define AUD_HDMI_DETECT_PERIOD  (20)  // For HDMI pop noise
#define AUD_PTR_UPDATE_TIMEOUT  (30)
#define APLL_MON_THREAD_PERIOD  (300)

#define DTV_FIFO_FULL_THRESHOLD      2000
#define DTV_AUD_FIFO_FULL_THRESHOLD  188

// Dual decoder time out ... 
// for dual decoder thread synchronize main/aux decoder thread
#define DUAL_DEC_SYNC_TIME_OUT 1000

// for input source from buffer agent (Buffer agent thread)
#define BAGT_BUF_NUM 2
#define BAGT_BLK_ALIGN 1
#define CONSUMED_Q_NUM 20

// Feeder relative setting
#define FEEDER_REQUEST_DATA_SIZE  (16*1024)

// Waiting for analog tuner lock signal ..
//2007/12/27 3000->1000, because it takes too long and would block dec0 starting play when there is no ATV source 
//           under source HDMI and AV.
#define ATUNER_WAITING_TIME_OUT 500//from 1000 -> 500, to prevent change channel to decoded ATV channel will take a long time to wait semiphone un-lock
#ifdef CC_ENABLE_AV_SYNC
#define VDP_WAITING_TIME_OUT 1500
#endif
#define AIN_CHANGE_TYPE_THRESHOLD (500/AUD_HDMI_DETECT_PERIOD)
#define FEED_STREAM_WAITING_TIME_OUT    100
//#define HDMI_WAIT_VIDEO_THRESHOLD (100/AUD_HDMI_DETECT_PERIOD)

#define HDMI_AUD_OK               0
#define HDMI_AUD_UNSTABLE_LVL_1   1
#define HDMI_AUD_UNSTABLE_LVL_2   2
#define HDMI_AUD_UNSTABLE_LVL_3   3

// DTV lock/unlock checking threshold, ie. 2000 ms
#define DTV_LOCK_CHECK_THREASHOLD (2000/AUD_PTR_UPDATE_TIMEOUT)
#define DTV_DECODE_ERR_CHECK_THREASHOLD (5000/AUD_PTR_UPDATE_TIMEOUT)   //5s, same as video

#define MM_SYNC_INFO_NUM 1024

#define MPEG_FRAME_SIZE 0x870 //1152 sample for 48 kHZ, in 90K Hz Clock.

//  -- DSP Force --
//#define DSP_FORCE_RESET_ENABLE
#define DSP_MONITOR_THREAD_PERIOD       100
#define DSP_RESUCE_THRESHOLD            (3 * 1000)
#define DSP_RESUCE_THRESHOLD_1          (5 * 1000)
#define DSP_MONITOR_THREAD_START_TIME   (10 * 1000)
//


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

#define DSP_MT5371_OFFSET                   0x480
#define DSP_SRAM_DECODE_FRAME_COUNT         (0x1097 + DSP_MT5371_OFFSET)
#define DSP_SRAM_DECODE_FRAME_COUNT_DEC2    (0x2D97)    ///[Joel](0x2a97) /*(0x1117 + DSP_MT5371_OFFSET)*/
#ifdef CC_MT5391_AUD_SUPPORT
#define DSP_SRAM_DECODE_FRAME_COUNT_DEC3    (0x0a17) 
#else
#define DSP_SRAM_DECODE_FRAME_COUNT_DEC3    (0x2997) 
#endif
//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

extern UINT8 _guAudDemodTypeChange;
extern UINT8 _guAudDspTrigAtvFmtchg;
extern AUD_FMT_T _eAtvFmtChgDecFmt;

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
extern void _AUD_DspSetIEC(UINT8 u1DecId);
extern AUD_IEC_T _GetIECFlag(UINT8 u1DecId);
extern void _AUD_DspSetDDCOEnable(BOOL fgEnable);
extern void AUD_WaitUopCommandDone(UINT8 u1DecId);
extern void vDspSetFifoReadPtr(UINT8 u1DecId, UINT32 u4ReadPtr);
extern void vDspSetSyncMode(UINT8 u1DecId, AV_SYNC_MODE_T eMode);
extern void vAudOutReInit(UINT8 ucDecId, BOOL fgEnable);
extern BOOL fgDspReadPtrSet (UCHAR ucDecId, UINT32 u4Address);
//extern BOOL fgApiVideoStable(UINT8 bVideoPath);
extern BOOL fgApiVideoStable(UINT8 u1VideoFrame); // 0: main frame, 1: sub frame for PIP/POP

extern void AUD_SetSrc(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt);
extern void AUD_DspDvdEnable(void);
extern void AUD_DualDecCommandDone(AUD_DUAL_DEC_CMD_T eAudDecCmd, UINT32 u4Result);
extern BOOL _DMX_GetInterruptThreshold(DMX_PID_TYPE_T ePidType, UINT32* pu4Bytes);
extern BOOL _DMX_SetInterruptThreshold(DMX_PID_TYPE_T ePidType, UINT32 u4Bytes);
extern UINT8 u1HDMIGetAudioSamplingFreq(void);
extern void bHDMIClockDetect(void);
extern BOOL AUD_IsHdmiClkExist(void);

void AUD_ClearFifo(UCHAR ucDecId);
void AUD_GetAudioPes(UINT8 u1DecId, PSR_AUDIO_PES_T * prAudioPes);
void AUD_FixWarning(void);
BOOL AUD_UpdateAudioFifo(UINT8 u1DecId, UINT32 u4SartAddress, UINT32 u4Size, UINT32 u4WritePtr);
void AUD_SetStcThreshold(UINT8 u1DecId, UINT16 u2Threshold);

extern UINT8 bHDMIAudStatus(void);
extern void ADAC_HdmiUnstableSpkEnable(BOOL fgFlag);
extern UINT32 AUD_GetDecFrameCnt(UINT8 u1DecId);
extern BOOL AUD_IsPcmDecMute(UINT8 u1DecId);

extern INT32 IS_MultiMediaSupport(void);
extern UINT32 dwGetDspAckPTS (BOOL fgFirstDecoder);
extern void AUD_ClearPtsInISR(UINT8 u1DecId);

#ifdef CC_ENABLE_AV_SYNC
//extern void vDrvSetAudioTypeChangeNotify(UINT8 notifyValue);
#endif

#ifdef ENABLE_MULTIMEDIA
extern BOOL _SWDMX_AudioNotify(ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType);
#endif
extern UINT32 u4ReadD2RInfo_AputBank(UINT8 u1DecId);
extern UINT32 u4ReadD2RInfo_InputUnderRun(UINT8 u1DecId);  //-- DSP Force --
extern void vDSPClearDecType(void);                  //-- DSP Force --
extern INT32 u4ADSP_DspReset(void);                  //-- DSP Force --
extern void AUD_DSPCmdSema_Lock(void);            //-- DSP Force --
extern void AUD_DSPCmdSema_UnLock(void); //-- DSP Force --
extern UINT32 u4ReadD2RInfo_ReadPtr(UINT8 u1DecId); //-- DSP Force --

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

static void _DrvThreadInit(void);
static void _ChangeAudioState(UINT8 u1DecId, AUD_DRV_STATE_T eNewState);
//static void _FlushAudioFifo(UINT8 u1DecId);
static void _AudDrvThread(void* pvArg);
static void _AudFeedStreamThread(const void* pvArg);
static void _AudSetEvent(UINT8 u1DecId, UINT32 u4Event);
static BOOL _AudWaitEvent(UINT8 u1DecId, UINT32 * pu4Event, BOOL fgWait, UINT32 u2TimeOut);
static BOOL _IsPtsValid(UINT8 u1PidIdx, UINT8 ucDevId, const DMX_AUDIO_PES_T * prPes);
static void _LoadAdspCode(UINT8 u1DecId, AUD_FMT_T eStreamFormat);
//static void _UpdatePtsToShm(UINT8 u1DecId, UINT32 u4Pts, UINT32 u4Addr);
static void _AudApllMonitorThread(void* pvArg);
#ifndef CC_MT5391_AUD_3_DECODER
static void _AudDualDecDrvThread(void* pvArg);
#endif
static void _AudDtvOnPlayThread(void* pvArg);
static void _DtvResetLockStatus(UINT8 u1DecId);
inline void _AudDrvThread_WaitResume(UINT8 u1DecId);
static void _AudDspMonitorThread(void* pvArg); //  -- DSP Force --
static void _AudDrvResetDSP(UINT8 u1DecId);     //        -- DSP Force --

//---------------------------------------------------------------------------
// Global variables
//---------------------------------------------------------------------------
UINT32 _gu4AudCtrlType;
UINT32 _gu1AoutDramErr;
#ifndef CC_MT5391_AUD_3_DECODER
AUD_DECODER_T arAudDecoderOld[2];
#else
AUD_DECODER_T arAudDecoderOld[3];
#endif


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static BOOL _fgAoutTimeoutEnable = FALSE;

static UINT32 _u4FeedStreamResult;
static INT32  _i4MemStrLoopCnt;
static BOOL   _fgInfinite = FALSE;
static BOOL   _fgForceStopStrmDma = FALSE;

static BOOL   fgDualDmxMode = FALSE;   

// Thread hanlder
static HANDLE_T _hAudDrvThread;
static HANDLE_T _hAudApllMonThread;
static HANDLE_T _hAudFeedStrThread = NULL;
static HANDLE_T _hAudBagtThread = NULL;
static HANDLE_T _hAudFeederThread = NULL;
static HANDLE_T _hAudDspMonThread; //  -- DSP Force --
#ifndef CC_MT5391_AUD_3_DECODER
static HANDLE_T _hAudDtvThread[2] = {NULL, NULL};
#else
static HANDLE_T _hAudDtvThread[3] = {NULL, NULL, NULL};
#endif
// Semaphore and message queue
static HANDLE_T _hSemaLoadCode;
#ifndef CC_MT5391_AUD_3_DECODER
static HANDLE_T _ahSemaUopComDone[2];
#else
static HANDLE_T _ahSemaUopComDone[3];
#endif
static HANDLE_T _hDualDecSyncSema;
static HANDLE_T _hDetectionSyncSema;
static HANDLE_T _hFeedStreamSyncSema;
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
static HANDLE_T _ahAudCmdQueue[2];
#else
static HANDLE_T _ahAudCmdQueue[3];
#endif
static HANDLE_T _hAudDualDecCmdQueue;
static HANDLE_T _hAudApllCmdQueue;
#ifndef CC_MT5391_AUD_3_DECODER
static DMX_AUDIO_PES_T _arAudPes[2];
#else
static DMX_AUDIO_PES_T _arAudPes[3];
#endif
static HANDLE_T _hAtunerSyncSema;

#ifdef CC_ENABLE_AV_SYNC
static HANDLE_T _hVdpSyncSema;
#endif

#ifdef CC_MT5391_AUD_3_DECODER
static HANDLE_T _arAudDrvStateSema[3];
#endif

#ifndef CC_MT5391_AUD_3_DECODER
static BOOL      _afgDtvOnPlayThreadGo[2] = {FALSE, FALSE};
static UINT32 _u4DualDecEventFlag = 0;
#else
static BOOL      _afgDtvOnPlayThreadGo[3] = {FALSE, FALSE,FALSE};
#endif
static AUD_DECODER_T _arAudDecoder[3];
#ifdef CC_ENABLE_AV_SYNC
static BOOL  _fgVdpReady = FALSE;
static BOOL  _fgIgnoreVdpInfo;
static BOOL  _fgVdpChangeState = FALSE;
UINT8        _u6VideoSetCHdelay=0;
BOOL        _fgwaitForAoutReinit=FALSE;
UINT8        _GetVideoCBCount=0;
#endif
static BOOL _fgDualDecModeDrv = FALSE;

static BOOL _fgAudDrvInitiated = FALSE;
static BOOL _fgAudDrvThreadInit = FALSE;
#ifndef CC_MT5391_AUD_3_DECODER
static BOOL _afgFirstIPicArrived[2] = {FALSE, FALSE};
#else
static BOOL _afgFirstIPicArrived[3] = {FALSE, FALSE, FALSE};
#endif
#ifndef CC_MT5391_AUD_3_DECODER
static BOOL _afgIssuePlayComToDsp[2] = {FALSE, FALSE};
#else
static BOOL _afgIssuePlayComToDsp[3] = {FALSE, FALSE,FALSE};
#endif
#ifndef CC_MT5391_AUD_3_DECODER
static BOOL _afgAtvVideoDisplayFlag[2] = {FALSE, FALSE};
#else
static BOOL _afgAtvVideoDisplayFlag[3] = {FALSE, FALSE, FALSE};
#endif
//static BOOL _afgUseModifiedPts[2] = {FALSE, FALSE};
#ifndef CC_MT5391_AUD_3_DECODER
static UINT8 _uAudioDelayPTSFlag[2] = {0, 0};
#else
static UINT8 _uAudioDelayPTSFlag[3] = {0, 0, 0};
#endif
//static UINT16 _uAudioEventTimeout[2] = {AUD_EVENT_TIMEOUT, AUD_EVENT_TIMEOUT};
// Video PIP/POP displayed frame indicator, 0: display main frame, 1: display sub frame.
static UINT8 _u1VideoFrameFlag = 0;
#ifndef CC_MT5391_AUD_3_DECODER    
    static BOOL _fgIsFirstTime[2] = {TRUE, TRUE};
#else
    static BOOL _fgIsFirstTime[3] = {TRUE, TRUE, TRUE};
#endif
static BOOL _fgApllMonThrRun = FALSE;
static BOOL _fgDDBannerOn = FALSE;

// Dual decoder variables
static AUD_DRV_STATE_T _arAudDrvState[3] = {AUD_UNINITIALIZED, AUD_UNINITIALIZED, AUD_UNINITIALIZED};
static AUD_DUAL_DEC_DRV_STATE_T _rAudDualDrvState = AUD_DUAL_DEC_UNINITIALIZED;
static AUD_DEC_STREAM_FROM_T    _eDualDecStreamSrc = AUD_STREAM_FROM_OTHERS;
static AUD_DEC_STREAM_FROM_T    _eDualDecStreamSrcOrg = AUD_STREAM_FROM_OTHERS;
static AUD_FMT_T                _eDualDecStreamFmt;
//static AUD_FMT_T                _eDualDecLastAtvStreamFmt = AUD_FMT_PAL;

static PARAM_MPV_T* _prMPV;     
static DRV_DBASE_T* prDbase;

//buffer agent thread variables
static UINT32 _u4CurrRqstId;
static UINT32 _u4CurrOffset;
static AUD_BUF_AGT_SRC_ALIGN_INFO_T _rAudBagtInfo;
static BAGT_FUNC_INFO_T _rAudBagtFctInfo;
static AUD_BAGT_NFY_INFO_T _rAudBagtAck;
static BOOL _fgBagtStop = FALSE;
static BOOL _fgBagtWaitFlush = FALSE;
static UINT32 _u4LastRp;
static UINT8 _u1Timer;

// For memory clip
static AUD_CLIP_NOTIFY _hClipNotifyFunc = NULL; 


// Feeder varaibles
static FEEDER_TRANSMIT _rFeedTransmitInfo;
//static BOOL _fgDtvLock = TRUE;

#ifndef CC_MT5391_AUD_3_DECODER
static BOOL _afgDtvLock[2] = {FALSE, FALSE};
static BOOL _afgDtvPesLock[2] = {FALSE, FALSE};
#else
static BOOL _afgDtvLock[3] = {FALSE, FALSE,FALSE};
static BOOL _afgDtvPesLock[3] = {FALSE, FALSE,FALSE};
#endif
// HDMI relative
static BOOL  _fgHdmiMute = FALSE;
static UINT8 _u1HDMIMuteCnt = 0;
static UINT8 _u1HDMIUnDetectCnt = 0;

// DTV lock status
#ifndef CC_MT5391_AUD_3_DECODER
static UINT32 _au4LastPesCnt[2] = {0, 0};
static UINT32 _au4FrameCnt[2] = {0, 0};  
static UINT32 _au4LastFrameCnt[2] = {0, 0};
static UINT32 _au4PesCnt[2] = {0, 0};
static UINT8  _au1NoFrameCntIncrease[2] = {0, 0};
static UINT8  _au1NoPesCntIncrease[2] = {0, 0};
static AUD_DECODE_STATUS_T  _aeDecStatus[2];
static BOOL   _fgMMSet[2] = {FALSE,FALSE};    //MM EOS relative
static AUD_DECODE_STATUS_T  _aeHDMIDecStatus[2];
#else
static UINT32 _au4LastPesCnt[3] = {0, 0, 0};
static UINT32 _au4FrameCnt[3] = {0, 0, 0};  
static UINT32 _au4LastFrameCnt[3] = {0, 0, 0};
static UINT32 _au4PesCnt[3] = {0, 0, 0};
static UINT8  _au1NoFrameCntIncrease[3] = {0, 0, 0};
static UINT8  _au1NoPesCntIncrease[3] = {0, 0, 0};
static AUD_DECODE_STATUS_T  _aeDecStatus[3];
static BOOL   _fgMMSet[3] = {FALSE,FALSE,FALSE};    //MM EOS relative
static AUD_DECODE_STATUS_T  _aeHDMIDecStatus[3];
#endif

// MM : MultiMedia Releated.
static AUD_MM_SYNC_INFO_T _aeMMSyncInfo[MM_SYNC_INFO_NUM];
//static AUD_MM_SYNC_INFO_T* _paeMMSyncInfo;
static UINT16 u2CurrIdx = 0;
static UINT16 u2LatestIdx = 0;
static BOOL fgLoopBack = FALSE;


static CHAR * _aszAudDualState[] =
{
    "DUAL_DEC_UNINITIALIZED",
    "DUAL_DEC_INIT_AUX_DECODER",
    "DUAL_DEC_PLAY",
    "DUAL_DEC_STOP",
    "DUAL_DEC_IDLE"
};

static CHAR * _aszAudDualCmd[] =
{
    "AUD_DUAL_DEC_CMD_PLAY",
    "AUD_DUAL_DEC_CMD_STOP"
};

static CHAR * _aszCodeType[] =
{
    "UNKNOWN",
    "MPEG",
    "AC3",
    "PCM",
    "MP3",
    "AAC",
    "DTS",
    "WMA",
    "RA",
    "HDCD",
    "MLP",
    "MTS",
    "CANAL+",
    "PAL",
    "A2",
    "FMFM",
    "NICAM",
    "TTXAAC",
    "DETECTOR",
    "MINER",
    "LPCM",
    "FM_RADIO",
    "FM_RADIO_DET"
};

static CHAR * _paszAudState[] =
{
    "UNINITIALIZED",      
    "TRIGGER_ADSP",
    "WAIT_POWER_ON",
    "INIT",
    "WAIT_CMD",
    "WAIT_PLAY",
    "WAIT_PAUSE",
    "WAIT_RESUME",
    "WAIT_STOP",
    "WAIT_STOPSTREAM",
    "IDLE",
    "ON_PLAY",
    "ON_PAUSE",
    "STOPPED",
    "DECODE_INIT",
    "CHANGE_FORMAT"
};

static CHAR * _paszAudCmd[] =
{
    "AUD_CMD_PLAY",
    "AUD_CMD_STOP",
    "AUD_CMD_RESET",
    "AUD_CMD_PAUSE",
    "AUD_CMD_AVSYNC",
    "AUD_CMD_LOADCODE",
    "AUD_CMD_RESUME",
    "AUD_CMD_CHANGE_FORMAT",
    "AUD_CMD_STOPSTREAM"
};

static DRV_DBASE_T* _prDbase = NULL;
static UINT32 _au4TvSysMask[2] = {0, 0}; //2008/01/16
static UINT32 u4DspResueTrigger = 0;     //  -- DSP Force --
static BOOL _fgAudDrvForceReset = FALSE; //  -- DSP Force --

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

#ifdef CC_ENABLE_AV_SYNC

//22.implement the callback function
void _AudSrmStableCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4)
{
    UINT16 u2DelayMs = 0;

    if (u4Arg4 == AUD_VIDEO_IS_STABLE) //Stable Callback : means SRM is ready.
    {
        x_thread_delay(20);//wait video update finish

        // 1.query vdp
        u2DelayMs = bDrvVideoGetFrameDelay(SV_VP_MAIN);
        _GetVideoCBCount = _GetVideoCBCount+1;
        LOG(1," [SRM_AVSYNC]>>>>>>>>>>> _AudSrmStableCb (%d) , %d <<<<<<<<<< \n ", u2DelayMs, _GetVideoCBCount);
        
       // 2.control aud, AUD_DrvSetChDelay
        if (_GetVideoCBCount ==1)  //only set channel delay when _GetVideoCBCount=1
        {
            LOG(1," [SRM_AVSYNC]######### _GetVideoCBCount=1, can set channel dealy #########\n ", u2DelayMs);
            AUD_DrvSetChDelay(u2DelayMs);
        }   
        _fgVdpChangeState = FALSE;
    }
    else  //UNStable Callback : means SRM mode change.
    {
        LOG(7, "[SRM_AVSYNC] @@@ VDP Change State......\n");
        _fgVdpChangeState = TRUE;
        #if 1 //move from WAIT_STOP to here, since this callback may come before or after WAIT_STOP
        _fgVdpReady = FALSE;
        _fgIgnoreVdpInfo = FALSE;
        _fgwaitForAoutReinit=FALSE;
        _GetVideoCBCount=0;     
        x_sema_lock(_hVdpSyncSema, X_SEMA_OPTION_NOWAIT); //make sure this is locked for later use.
        #endif        
    }

}

#endif

static BOOL _AudDmxDtvInitPlaybackFifoFull(UINT8 u1DecId)
{
    UINT32 u4Fifo1Start;
    UINT32 u4Fifo1End;
    UINT32 u4Fifo2Start;
    UINT32 u4Fifo2End;
    UINT32 u4WritePtr;
//    UINT32 u4ReadPtr;
    UINT32 u4FifoEnd;
    UINT32 u4FifoStart;
            	        
    UNUSED(_AUD_GetAudFifo(&u4Fifo1Start, &u4Fifo1End, &u4Fifo2Start, &u4Fifo2End));
//    _AUD_DMX_GetAudPtr(u1DecId, &u4ReadPtr, &u4WritePtr);
    _AUD_DMX_GetAudWrtPtr(u1DecId, &u4WritePtr);
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
    u4FifoStart = (u1DecId == AUD_DEC_MAIN) ? u4Fifo1Start : u4Fifo2Start;
    u4FifoEnd = (u1DecId == AUD_DEC_MAIN) ? u4Fifo1End : u4Fifo2End;
#else
    // MAIN and THIRD(AD) decoder used Fifo1
    u4FifoStart = (u1DecId == AUD_DEC_AUX) ? u4Fifo2Start : u4Fifo1Start;
    u4FifoEnd = (u1DecId == AUD_DEC_AUX) ? u4Fifo2End : u4Fifo1End;
#endif

    if (_arAudDecoder[u1DecId].u4ReceivePesCount == 0)
    {
        return FALSE;
    }
    else
    {
        // Check if write pointer is within audio fifo
        VERIFY((u4WritePtr >= u4FifoStart) && (u4FifoEnd >= u4WritePtr));

        // Check fifo full
        if ((u4FifoEnd - u4WritePtr) < DTV_AUD_FIFO_FULL_THRESHOLD)
        {
            return TRUE;
        }
    }
    
    return FALSE;
}

static void _AudSetEvent(UINT8 u1DecId, UINT32 u4Event)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    _arAudDecoder[u1DecId].u4EventFlag = u4Event;
}
#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0521
static void _AudDualDecSetEventFlag(UINT32 u4EventFlag)
{
    _u4DualDecEventFlag = u4EventFlag;
}
#endif
static void _AudBagtRqstInit(void)
{
    static BOOL fgInit = FALSE;

    // Init at once
    if (!fgInit)
    {
        //information initialization
        x_memset((VOID *)&_rAudBagtFctInfo, 0, sizeof(BAGT_FUNC_INFO_T));
        _rAudBagtFctInfo.fgRegistered = FALSE;
        _rAudBagtFctInfo.u4Handler = 1000;
        _rAudBagtFctInfo.AudBagtDataReqFct = NULL;
        fgInit = TRUE;
    }    
}

static void _AudBagtRqstFct(    UINT16                  ui2_buf_agt_id,
                                                     AUD_BUF_AGT_DATA_BLK_T*     pt_data_blk,
                                                     AUD_2_BAGT_NFY_COND_T eType)
{
    _AudBagtRqstInit();
    if ((_rAudBagtFctInfo.fgRegistered ) && (_rAudBagtFctInfo.AudBagtDataReqFct != NULL))
    {
        _rAudBagtFctInfo.AudBagtDataReqFct(ui2_buf_agt_id, pt_data_blk, eType);
    }
}

static BOOL _AUD_GetBagtRqstFunc(AudBagtDataReqFct * pfAudBagtRqstFct)
{
    if (pfAudBagtRqstFct)
    {
        *pfAudBagtRqstFct = _AudBagtRqstFct;
    }
    return TRUE;
}
#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0521
static BOOL _AudDualDecNotify(AUD_DUAL_DEC_CMD_T eDualDecCmd, BOOL fgResult)
{
    AudDecNfyFct   pfAudDecNfy = NULL;
    UINT8          u1DecoderId = AUD_DEC_MAIN;
    
    LOG(7, "AUD Dual Decoder Command %s done\n", _aszAudDualCmd[eDualDecCmd]);

    // Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy));

    // Notify middleware 
    if ((eDualDecCmd == AUD_DUAL_DEC_CMD_PLAY)  ||
        (eDualDecCmd == AUD_DUAL_DEC_CMD_STOP))
    {
        UINT32 u4Cmd = 0;
        if (eDualDecCmd == AUD_DUAL_DEC_CMD_PLAY)
        {
            u4Cmd = (UINT32)AUD_CMD_PLAY;
        }
        else if (eDualDecCmd == AUD_DUAL_DEC_CMD_STOP)
        {
            u4Cmd = (UINT32)AUD_CMD_STOP;
        }
        else
        {
            LOG(1, "Invalid _AudDualDecNotify parameter\n");
            ASSERT(0);
        }
        
        if (pfAudDecNfy)
        {
            pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecoderId,
                        AUD_COND_CTRL_DONE, u4Cmd, AUD_OK);
        }
    }
    UNUSED(fgResult);
    return TRUE;
}

static BOOL _AudDualDecWaitEvent(AUD_DUAL_DEC_CMD_T * peEvent, BOOL fgWait)
{
    UINT32 u4Event;
    UINT16 u2MsgIdx;
    SIZE_T zMsgSize = sizeof(UINT32);
    AUD_DUAL_DEC_CMD_T eCmd;
    INT32 i4CmdOk;
    INT32 i4Result;
  
    do {
        if (!fgWait)
        {
            i4Result = x_msg_q_receive_timeout(&u2MsgIdx, &u4Event, &zMsgSize, 
                               &_hAudDualDecCmdQueue, 1, (UINT32)AUD_EVENT_TIMEOUT);
            VERIFY((i4Result == OSR_OK) || (i4Result == OSR_TIMEOUT));
            if (i4Result == OSR_TIMEOUT)
            {
                return FALSE;
            }
        }
        else // wait until get new event
        {
            VERIFY(x_msg_q_receive(&u2MsgIdx, &u4Event, &zMsgSize, 
                               &_hAudDualDecCmdQueue, 1, X_MSGQ_OPTION_WAIT) == OSR_OK);
        }
        LOG(9, "Dual Dec Thread Get %s event\n", _aszAudDualCmd[u4Event]);

        eCmd = (AUD_DUAL_DEC_CMD_T)u4Event;
        
        // Check if decoder state is the same as command
        if (((eCmd == AUD_DUAL_DEC_CMD_PLAY) && (_rAudDualDrvState == AUD_DUAL_DEC_PLAY)) ||
           ((eCmd == AUD_DUAL_DEC_CMD_STOP) && (_rAudDualDrvState == AUD_DUAL_DEC_STOP)) ||
           ((eCmd == AUD_DUAL_DEC_CMD_STOP) && (_rAudDualDrvState == AUD_DUAL_DEC_IDLE)))
        {
            if (eCmd == AUD_DUAL_DEC_CMD_PLAY)
            {
                // Notify upper layer
                UNUSED(_AudDualDecNotify(AUD_DUAL_DEC_CMD_PLAY, AUD_OK));
                // Release semaphore
                AUD_DualDecCommandDone(AUD_DUAL_DEC_CMD_PLAY, AUD_OK);
            }    
            else if (eCmd == AUD_DUAL_DEC_CMD_STOP)
            {
                // Notify upper layer
                UNUSED(_AudDualDecNotify(AUD_DUAL_DEC_CMD_STOP, AUD_OK));
                // Release semaphore
                AUD_DualDecCommandDone(AUD_DUAL_DEC_CMD_STOP, AUD_OK);
            }    
            i4CmdOk = FALSE;   
        }
        else
        {
            /* Check command and decoder event flag */
            if ((1 << (UINT8)eCmd) & _u4DualDecEventFlag)
            {
                i4CmdOk = TRUE;
            }
            else
            {
                LOG(5, "ERROR DualDec command %s\n", _aszAudDualCmd[eCmd]);
                i4CmdOk = FALSE;
            }
        }
    } while (!i4CmdOk);

    if (peEvent != NULL)
    {
        *peEvent = eCmd;
    }
    
    return TRUE;
}
#endif

static BOOL _AudWaitEvent(UINT8 u1DecId, UINT32 * pu4Event, BOOL fgWait, UINT32 u2TimeOut)
{
    UINT32 u4Event;
    UINT16 u2MsgIdx;
    SIZE_T zMsgSize = sizeof(UINT32);
    AUD_DEC_CMD_T eCmd;
    INT32 i4CmdOk;
    INT32 i4Result;
    AUD_DEC_ID_VALIDATE(u1DecId);

    do {
        if (!fgWait)
        {
            i4Result = x_msg_q_receive_timeout(&u2MsgIdx, &u4Event, &zMsgSize, 
        	                   &_ahAudCmdQueue[u1DecId], 1, u2TimeOut);
            VERIFY((i4Result == OSR_OK) || (i4Result == OSR_TIMEOUT));
            if (i4Result == OSR_TIMEOUT)
            {
                return FALSE;
            }
        }
        else // wait until get new event
        {
            VERIFY(x_msg_q_receive(&u2MsgIdx, &u4Event, &zMsgSize, 
                               &_ahAudCmdQueue[u1DecId], 1, X_MSGQ_OPTION_WAIT) == OSR_OK);
        }
        LOG(9, "Dec (%d) Get %s event\n", u1DecId, _paszAudCmd[u4Event]);

#if 0   // Jessica: remove to prevent the case that parser pointer is not ready
        /* Log DSP read/write pointer for debug */
        if (((AUD_DEC_CMD_T)u4Event == AUD_CMD_PLAY) || 
            ((AUD_DEC_CMD_T)u4Event == AUD_CMD_STOP))
        {
            UINT32 u4RpDec1, u4WpDec1;
            UINT32 u4RpDec2, u4WpDec2;
            UINT32 u4DataSZDec1, u4DataSZDec2;
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
            UINT32 u4RpDec3, u4WpDec3;
            UINT32 u4DataSZDec3;
#endif
      
            /* Get read/write pointer */
            AUD_GetRWPtr(AUD_DEC_MAIN, &u4RpDec1, &u4WpDec1, &u4DataSZDec1);
            AUD_GetRWPtr(AUD_DEC_AUX, &u4RpDec2, &u4WpDec2, &u4DataSZDec2);
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
            AUD_GetRWPtr(AUD_DEC_THIRD, &u4RpDec3, &u4WpDec3, &u4DataSZDec3);
#endif            
    
            LOG(9, "Decoder 1 rp,wp,size = [0x%08x, 0x%08x, %u]\n",u4RpDec1,u4WpDec1,u4DataSZDec1);
            LOG(9, "Decoder 2 rp,wp,size = [0x%08x, 0x%08x, %u]\n",u4RpDec2,u4WpDec2,u4DataSZDec2);
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
            LOG(9, "Decoder 3 rp,wp,size = [0x%08x, 0x%08x, %u]\n",u4RpDec3,u4WpDec3,u4DataSZDec3);
#endif                        
        }   
#endif        
            
        eCmd = (AUD_DEC_CMD_T)u4Event;
        /* Check if decoder state is the same as command */
        if(((eCmd  == AUD_CMD_PLAY) && (_arAudDecoder[u1DecId].eDecState == AUD_DEC_PLAYING)) ||
           ((eCmd == AUD_CMD_STOP) && (_arAudDecoder[u1DecId].eDecState == AUD_DEC_STOP)) ||
           ((eCmd == AUD_CMD_PAUSE) && (_arAudDecoder[u1DecId].eDecState == AUD_DEC_PAUSE)))
        {
            AUD_CommandDone(u1DecId, eCmd, AUD_OK);
            i4CmdOk = FALSE;   
        }
        else
        {
            /* Check command and decoder event flag */
            if ((1 << (UINT8)eCmd) & _arAudDecoder[u1DecId].u4EventFlag)
            {
                i4CmdOk = TRUE;
            }
            else
            {
                LOG(5, "ERROR AUD_DRVCmd %s\n",_paszAudCmd[eCmd]);
                i4CmdOk = FALSE;
            }
        }
    } while (!i4CmdOk);

    if (pu4Event != NULL)
    {
        *pu4Event = u4Event;
    }
    
    return TRUE;
}

static void _WaitAtunerLockSignal(void)
{
    INT32 i4Ret;
    
    i4Ret = x_sema_lock_timeout(_hAtunerSyncSema, ATUNER_WAITING_TIME_OUT);  
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));
        
    if (i4Ret == OSR_TIMEOUT)
    {
        LOG(1, "_WaitAtunerLockSignal timeout\n");
    }
}
#ifdef CC_ENABLE_AV_SYNC
void _WaitVdpReady(void)
{
    INT32 i4Ret;
    AUD_FMT_T              eAudDecFormat;
    UINT16                     u2Delay;
    UINT8                      u1Version;
    UINT16 u2WaitTime;

    // Jessica: remove to avoid wait VDP timeout
    //UNUSED(x_sema_lock(_hVdpSyncSema, X_SEMA_OPTION_NOWAIT));
    if (_fgVdpChangeState)
    {
        u2WaitTime = VDP_WAITING_TIME_OUT;
        LOG(5, "[SRM_AVSYNC] >> Normal VDP Wait Condition.....................\n");
    }
    else
    {
        u2WaitTime = 300;        
        LOG(5, "[SRM_AVSYNC] >> Short VDP Wait Condition.....................\n");        
    }
    
        
    i4Ret = x_sema_lock_timeout(_hVdpSyncSema, u2WaitTime);  
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));
    if (i4Ret == OSR_TIMEOUT)
    {
        LOG(3, "[SRM_AVSYNC] _WaitVdpReady timeout\n");
        eAudDecFormat = _arAudDecoder[AUD_DEC_AUX].eDecFormat;
                
        if (_arAudDecoder[AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
        {
            if (eAudDecFormat==AUD_FMT_MPEG)
            {
                //u2Delay=_u6VideoSetCHdelay+80;
                u2Delay=_u6VideoSetCHdelay+50;
                LOG(3,"@@@@@ It is MPEG @@@@@@\n");        
            }
            else if(eAudDecFormat==AUD_FMT_AC3)
            {
                //u2Delay=_u6VideoSetCHdelay+120;   
                u2Delay=_u6VideoSetCHdelay+60;  
                LOG(3,"@@@@@ It is AC3 @@@@@@\n");
            }
            else if(eAudDecFormat==AUD_FMT_AAC)
            {
                // Get AAC version
                u1Version = u1GetAacVersion(AUD_DEC_AUX);
                if (u1Version==0x0) //AAC_LC
                {            
                    u2Delay=_u6VideoSetCHdelay;               
                    LOG(3,"@@@@@ It is AAC_LC @@@@@@\n");
                }          
                else//HE-AAC
                {
                    u2Delay= 0;                      
                    LOG(3,"@@@@@ It is HE-AAC @@@@@@\n");
                }   
            }
            else
            {
                u2Delay=_u6VideoSetCHdelay;        
                LOG(3,"@@@@@ It is others @@@@@@\n");
            }
        }
        else
        {
            u2Delay=0;        
            LOG(3,"@@@@@ It is others  & NOT DTV source@@@@@@\n");
        }

        //AUD_DspInputChannelDelay(_arAudDecoder[AUD_DEC_MAIN].eStreamFrom);
        //AUD_DspChannelDelay(((_u6VideoSetCHdelay*34)/5)+(u2Delay_init*34/5), AUD_CH_ALL, AUD_DEC_MAIN);
        AUD_DspChannelDelay(((u2Delay*34)/5), AUD_CH_ALL, AUD_DEC_MAIN);
        _fgIgnoreVdpInfo = TRUE;
        _fgwaitForAoutReinit=TRUE; //This flag should set TRUE with Set Ch Delay.
        _fgVdpChangeState = FALSE;
    }
    //_fgwaitForAoutReinit=TRUE;
    //AUD_DspDecPlayMuteEnable(AUD_DEC_MAIN, FALSE);     
    //LOG(3,"########### For AV sync , UN-MUTE Audio  2!!!! ######\n");
}
#endif

static void _DrvThreadInit(void)
{
    if (!_fgAudDrvThreadInit)
    {
        UINT8 u1DecId;

        VERIFY(x_sema_create(&_hAtunerSyncSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
#ifdef CC_ENABLE_AV_SYNC
        VERIFY(x_sema_create(&_hVdpSyncSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
#endif
        VERIFY(x_sema_create(&_hSemaLoadCode, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY(x_sema_create(&_ahSemaUopComDone[AUD_DEC_MAIN], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY(x_sema_create(&_ahSemaUopComDone[AUD_DEC_AUX], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
#ifdef CC_MT5391_AUD_3_DECODER
        VERIFY(x_sema_create(&_ahSemaUopComDone[AUD_DEC_THIRD], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
#endif                                

#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0521
        VERIFY(x_sema_create(&_hDualDecSyncSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
#endif
        VERIFY(x_sema_create(&_hDetectionSyncSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY(x_sema_create(&_hFeedStreamSyncSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

#ifdef CC_MT5391_AUD_3_DECODER
        VERIFY(x_sema_create(&_arAudDrvStateSema[AUD_DEC_MAIN], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY(x_sema_create(&_arAudDrvStateSema[AUD_DEC_AUX], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY(x_sema_create(&_arAudDrvStateSema[AUD_DEC_THIRD], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
#endif
        
#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0521       
        VERIFY((x_msg_q_create(&_hAudDualDecCmdQueue, ADRV_CMD_Q_DUAL_DEC, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
#endif
        VERIFY((x_msg_q_create(&_hAudApllCmdQueue, ADRV_APLL_CMD_Q, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
        VERIFY((x_msg_q_create(&_ahAudCmdQueue[AUD_DEC_MAIN], ADRV_CMD_Q_NAME1, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
        VERIFY((x_msg_q_create(&_ahAudCmdQueue[AUD_DEC_AUX], ADRV_CMD_Q_NAME2, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
            VERIFY((x_msg_q_create(&_ahAudCmdQueue[AUD_DEC_THIRD], ADRV_CMD_Q_NAME3, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
#endif             
        
        // Create audio thread1
        u1DecId = AUD_DEC_MAIN;
        VERIFY(x_thread_create(&_hAudDrvThread, AUD_DRV_THREAD1_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
               _AudDrvThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
        
        // Create audio thread2 
        u1DecId = AUD_DEC_AUX;
        VERIFY(x_thread_create(&_hAudDrvThread, AUD_DRV_THREAD2_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
               _AudDrvThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0521
        VERIFY(x_thread_create(&_hAudDrvThread, AUD_DRV_DUAL_DEC_THREAD_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
               _AudDualDecDrvThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
#endif               
#ifdef CC_MT5391_AUD_3_DECODER
        // Create audio thread3
        u1DecId = AUD_DEC_THIRD;
        VERIFY(x_thread_create(&_hAudDrvThread, AUD_DRV_THREAD3_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
               _AudDrvThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
#endif

        // Create apll monitor thread
        VERIFY(x_thread_create(&_hAudApllMonThread, AUD_APLL_MONITOR_THREAD_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
               _AudApllMonitorThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);

#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0521  -- 2
        u1DecId = AUD_DEC_MAIN;
        // Create DTV playback thread
        VERIFY(x_thread_create(&_hAudDtvThread[AUD_DEC_MAIN], AUD_DRV_DTV_MAIN_THREAD_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
               _AudDtvOnPlayThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
#else
        u1DecId = AUD_DEC_AUX;
        // Create DTV playback thread
        VERIFY(x_thread_create(&_hAudDtvThread[AUD_DEC_AUX], AUD_DRV_DTV_AUX_THREAD_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
               _AudDtvOnPlayThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
     //   u1DecId = AUD_DEC_THIRD;
        // Create DTV playback thread
     //   VERIFY(x_thread_create(&_hAudDtvThread[AUD_DEC_THIRD], AUD_DRV_DTV_THIRD_THREAD_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
     //          _AudDtvOnPlayThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);               
#endif
   
        // Create DSP monitor thread  //  -- DSP Force --
#ifdef DSP_FORCE_RESET_ENABLE        
        VERIFY(x_thread_create(&_hAudDspMonThread, AUD_DSP_MONITOR_THREAD_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
               _AudDspMonitorThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
#endif

    }
    _fgAudDrvThreadInit = TRUE;
}

#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0521       
static void _ChangeDualDecDrvState(AUD_DUAL_DEC_DRV_STATE_T eNewState)
{
    if (_rAudDualDrvState == eNewState)
    {
        return ;
    }   

    LOG(7, "ChangeDualDecDrvState(dual) (%s -> %s)\n",_aszAudDualState[_rAudDualDrvState], _aszAudDualState[eNewState]);
    _rAudDualDrvState = eNewState;
}
#endif

#ifndef CC_MT5391_AUD_3_DECODER 
static void _ChangeAudioState(UINT8 u1DecId, AUD_DRV_STATE_T eNewState)
{

    AUD_DEC_ID_VALIDATE(u1DecId);
    if (_arAudDrvState[u1DecId] == eNewState)
    {
        return ;
    }   

    LOG(7, "ChangeAudioState (%d) (%s -> %s)\n",u1DecId,_paszAudState[_arAudDrvState[u1DecId]], _paszAudState[eNewState]);
    _arAudDrvState[u1DecId] = eNewState;
}
#else
static void _ChangeAudioState(UINT8 u1DecId, AUD_DRV_STATE_T eNewState)
{

    AUD_DEC_ID_VALIDATE(u1DecId);

    VERIFY(x_sema_lock(_arAudDrvStateSema[u1DecId], X_SEMA_OPTION_WAIT) == OSR_OK);
    if (_arAudDrvState[u1DecId] != eNewState)
    {
        LOG(7, "ChangeAudioState (%d) (%s -> %s)\n",u1DecId,_paszAudState[_arAudDrvState[u1DecId]], _paszAudState[eNewState]);
        _arAudDrvState[u1DecId] = eNewState;
    }
    VERIFY(x_sema_unlock(_arAudDrvStateSema[u1DecId]) == OSR_OK);
    
}
#endif

static void _FlushAudioFifo(UINT8 u1DecId)
{
    UINT32 u4FifoStart;
    UINT32 u4FifoEnd;

    AUD_DEC_ID_VALIDATE(u1DecId);
    DSP_GetAFIFOPhysicalAddr(u1DecId, &u4FifoStart, &u4FifoEnd);
    LOG( 9, "Flush audio fifo = (%x,%x)\n", u4FifoStart, u4FifoEnd);

    VERIFY(u4FifoEnd >= u4FifoStart);
    x_memset(VIRTUAL((void *)u4FifoStart), 0, u4FifoEnd - u4FifoStart);
    HalFlushInvalidateDCache();
}

static void _AudFeedStreamThread(const void* pvArg)
{
    UINT8 u1DecId = AUD_DEC_MAIN;
    UINT32 u4Residual;
    UINT32 u4TransferSZ;
    UINT32 u4DesAddr;
    UINT32 u4AFifofSZ;
    UINT32 u4StreamAddr;
    UINT32 u4Retry = 0;
    INT32 i4Ret;
    
    if (pvArg != NULL)
    {
        u1DecId = *(UINT8 *)pvArg;
    }
    
    AUD_DEC_ID_VALIDATE(u1DecId);
    //FIXmt5381
    if (u1DecId == AUD_DEC_MAIN)
    {
	    AUD_CLR_BIT(REG_BS_CFG, PAR_SEL);
    }
    else
    {
	    AUD_SET_BIT(REG_BS_CFG, PAR_SEL);
    }

    u4DesAddr = u4GetAFIFOStart(u1DecId);
    u4AFifofSZ = u4GetAFIFOEnd(u1DecId) - u4GetAFIFOStart(u1DecId);
    u4Residual = _arAudDecoder[u1DecId].rMemBuffer.u4Length;
    u4StreamAddr = PHYSICAL((UINT32)_arAudDecoder[u1DecId].rMemBuffer.pData);
    
    _i4MemStrLoopCnt = (INT32)_arAudDecoder[u1DecId].rMemBuffer.u4LoopCount;
    _fgInfinite = (_i4MemStrLoopCnt <= 0) ? TRUE : FALSE;
    _fgForceStopStrmDma = FALSE;
    _u4FeedStreamResult = (UINT32)AUD_OK;
    
    i4Ret = x_sema_lock(_hFeedStreamSyncSema, X_SEMA_OPTION_NOWAIT);    
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
    
    while (u4Residual > 0)
    {
        u4TransferSZ = (u4Residual > TRANSFER_SZ) ? TRANSFER_SZ : u4Residual;

        LOG(9, "Dec(%d) Des: 0x%08x, Size: 0x%08x, Res: 0x%08x, Loop Cnt: %d\n", 
               u1DecId, u4DesAddr, u4TransferSZ, u4Residual, _i4MemStrLoopCnt);

        if (!PSR_SoftTransfer(PSR_STREAM_AUDIO, u1DecId, 
                              u4StreamAddr, u4TransferSZ, 
                              u4DesAddr, u4GetAFIFOStart(u1DecId), 
                              u4GetAFIFOEnd(u1DecId), FALSE))
        {
            _u4FeedStreamResult = (UINT32)AUD_FAIL; // error
            LOG(5, "Aud soft transter fail/terminated\n");
            break;
        }

        if (_fgForceStopStrmDma)
        {
            return;
        }

        u4DesAddr = ((u4DesAddr + u4TransferSZ) < u4GetAFIFOEnd(u1DecId)) ?
                     (u4DesAddr + u4TransferSZ) : ((u4DesAddr + u4TransferSZ) - u4AFifofSZ);

        u4Residual -= u4TransferSZ;
        u4StreamAddr += u4TransferSZ;

        // Check loop count and end condition
        if (u4Residual == 0)
        {
            _i4MemStrLoopCnt--;
            if ((_i4MemStrLoopCnt > 0) || _fgInfinite)
            {
                u4Residual = _arAudDecoder[u1DecId].rMemBuffer.u4Length;
                u4StreamAddr = (UINT32)_arAudDecoder[u1DecId].rMemBuffer.pData;
            }    
        }
        if (_arAudDecoder[u1DecId].eDecFormat != AUD_FMT_PCM)
        {
            x_thread_delay(10); /*if data is garbage, hand over control*/
        }    
    }

    // Check if the audio fifo has been empty
     while (1)
    {
     if (_fgForceStopStrmDma)
        {
            return;
        }
        
     if ((AUD_IsFifoEmpty(u1DecId)) || (u4Retry > 50))
        {
            break;
        }
     
     if((_arAudDecoder[u1DecId].eDecState != AUD_DEC_PAUSE)&&(_arAudDecoder[u1DecId].eDecState != AUD_DEC_INIT))
        {
            u4Retry++;
        }
     i4Ret = x_sema_lock_timeout(_hFeedStreamSyncSema, FEED_STREAM_WAITING_TIME_OUT);  
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));
        //x_thread_delay(100);
        
    } 
    UNUSED(AUD_DRVCmd(u1DecId, AUD_CMD_STOPSTREAM));

    if (_hClipNotifyFunc != NULL)
    {
        _hClipNotifyFunc(u1DecId, 0, 0);
    }
    
    LOG(9, "Dec(%d) stream size 0x%08x finished\n", u1DecId,
           _arAudDecoder[u1DecId].rMemBuffer.u4Length);
}

static UINT32 _GetWpMinusRp (UINT8 u1DecId, UINT32 u4WpAddr)
{
    UINT32 u4Rp;
    UINT32 u4AFifofSZ;
    u4Rp = DSP_GetDspReadPtrPhysicalAddr(u1DecId);
    u4Rp = (u4Rp<u4GetAFIFOStart(u1DecId)) ?
        u4GetAFIFOStart(u1DecId) : u4Rp;
        
    u4AFifofSZ = u4GetAFIFOEnd(u1DecId) - u4GetAFIFOStart(u1DecId);

    return ((u4WpAddr < u4Rp) ? 
        ((u4WpAddr - u4Rp)+u4AFifofSZ) : (u4WpAddr - u4Rp)); 
}

static BOOL _IsLastBlkPlayDone (UINT8 u1DecId, UINT32 u4RpAddr, UINT32 u4WpAddr)
{
    UINT32 u4Rp;
    
    u4Rp = DSP_GetDspReadPtrPhysicalAddr(u1DecId);
    LOG(8, " _IsLastBlkPlayDone: Rp=0x%08x; C=0x%08x; D=0x%08x  \n"
        , u4Rp,u4RpAddr,u4WpAddr);

    // check if the end of data, need to time-out
    if (u4Rp == _u4LastRp)
    {
        _u1Timer++;
        if (_u1Timer>30)   
        {
            if (_arAudDrvState[u1DecId]==AUD_ON_PAUSE)
            {
    	        _u1Timer = 0;
                return FALSE;
            }
            LOG(8, " read pointer is not moving \n");
            return TRUE;
        }
    }
    else
    {
    	_u1Timer = 0;
    }
    
    //check data consumed
    if ((u4RpAddr < u4WpAddr) && 
            ((u4Rp >= u4WpAddr) || ( _u4LastRp > u4Rp) || ( u4RpAddr > u4Rp) )) 
    {
        return TRUE;      
    }
    // (u4RpAddr > u4WpAddr)
    else  if (((u4Rp >= u4WpAddr) && (u4Rp < u4RpAddr) ) ||
                ((u4Rp >= u4WpAddr) && (_u4LastRp < u4RpAddr)&& (u4Rp > u4RpAddr)) ||
                (((u4Rp >= u4WpAddr)||(_u4LastRp < u4WpAddr)) && (_u4LastRp > u4Rp)) )  
    {
        return TRUE;
    }
    _u4LastRp = u4Rp;
    return FALSE;
}

// thread to handle  MP3 playback, source form buffer agent
static void _AudBagtThread (const void* pvArg)
{
    UINT8 u1DecId = AUD_DEC_MAIN;
    UINT32 u4TransferSZ;
    UINT32 u4Residual;
    UINT32 u4BlkSize;
    UINT32 u4StreamAddr;
    UINT32 u4AFifofSZ;
    UINT32 u4DesAddr;
    UINT32 au4WpAddr[CONSUMED_Q_NUM];
    UINT32 au4RpAddr[CONSUMED_Q_NUM];
    AUD_BAGT_BUF_T arBufddr[BAGT_BUF_NUM];
    UINT32 i;
    UINT8 u1BufId = 0;   //0 ~ (BAGT_BUF_NUM-1)
    AudBagtDataReqFct pfAudBagtRqst = NULL;
    AUD_BUF_AGT_DATA_BLK_T  r_blk_request;
    AUD_BUF_AGT_DATA_BLK_T  ar_blk_consume[CONSUMED_Q_NUM];
    UINT32 u4Idx = 0;    //always request (u4Idx+1)th block when transfering (u4Idx)th block to AFIFO
    UINT32 u4IdxPlay = 0;
    BOOL fgWraped = FALSE;
    UINT32 u4CheckSize;
        
    if (pvArg != NULL)
    {
        u1DecId = *(UINT8 *)pvArg;
    }

    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));

    if (u1DecId == AUD_DEC_MAIN)
    {
	    AUD_CLR_BIT(REG_BS_CFG, PAR_SEL);
    }
    else
    {
	    AUD_SET_BIT(REG_BS_CFG, PAR_SEL);
    }

    // buffer agent initilization
    _fgBagtStop = FALSE;
     _fgBagtWaitFlush = TRUE;
    _u4CurrRqstId = 0;
    _u4CurrOffset = 0;
    _u4LastRp = 0;
    _u1Timer = 0;
    _rAudBagtAck.eNtyCond= AUD_BUF_AGT_NFY_COND_UNKNOWN;
    VERIFY( _AUD_GetBagtRqstFunc(&pfAudBagtRqst));
    u4DesAddr = u4GetAFIFOStart(u1DecId);
    u4AFifofSZ = u4GetAFIFOEnd(u1DecId) - u4GetAFIFOStart(u1DecId);
    u4BlkSize = u4AFifofSZ;
    x_memset(au4RpAddr, 0, CONSUMED_Q_NUM);        
    x_memset(au4WpAddr, 0, CONSUMED_Q_NUM);        

        
    //check block size
                
    _rAudBagtInfo.ui4_min_blk_size = (UINT32)((_rAudBagtInfo.ui4_min_blk_size == 0)?
                 1 : _rAudBagtInfo.ui4_min_blk_size);
            
    u4BlkSize = u4AFifofSZ-(u4AFifofSZ % (_rAudBagtInfo.ui4_min_blk_size));
            
    au4WpAddr[(u4Idx% CONSUMED_Q_NUM)] =  ((u4DesAddr + u4BlkSize) <= u4GetAFIFOEnd(u1DecId)) ?
                     (u4DesAddr + u4BlkSize) : ((u4DesAddr + u4BlkSize) - u4AFifofSZ);
    au4RpAddr[(u4Idx% CONSUMED_Q_NUM)] = 0;   // fix lint warning

    // allocate buffers for buffer agent and set correct alignment
    for (i = 0; i <BAGT_BUF_NUM; i++)
    {
        arBufddr[i].pui1_buff = x_mem_alloc(u4BlkSize + _rAudBagtInfo.ui4_align);
        if (arBufddr[i].pui1_buff == NULL)
        {
            LOG(8, " Fail to creat buffer(%d) for buffer agent\n", i);
            UNUSED(AUD_DRVCmd(u1DecId, AUD_CMD_STOP));
        }
        else
        {
            arBufddr[i].pui1AllignedAddr = (_rAudBagtInfo.ui4_align == 0) ? 
                ((UINT32)arBufddr[i].pui1_buff) : 
                ( ((UINT32)arBufddr[i].pui1_buff + _rAudBagtInfo.ui4_align) -  
                    (((UINT32)arBufddr[i].pui1_buff)%(_rAudBagtInfo.ui4_align)));              
        }
    }

    x_memset(&r_blk_request, 0, sizeof(AUD_BUF_AGT_DATA_BLK_T));
    r_blk_request.pui1_buff = (UINT8*)arBufddr[u1BufId].pui1AllignedAddr;
    r_blk_request.ui4_req_id = u4Idx ;
    r_blk_request.u4Size = u4BlkSize;
    x_memset(&(ar_blk_consume[0]), 0, sizeof(AUD_BUF_AGT_DATA_BLK_T)*CONSUMED_Q_NUM);
    ar_blk_consume[(u4Idx% CONSUMED_Q_NUM)].pui1_buff =  (UINT8*)arBufddr[u1BufId].pui1AllignedAddr;
    ar_blk_consume[(u4Idx% CONSUMED_Q_NUM)].ui4_req_id = u4Idx;
    ar_blk_consume[(u4Idx% CONSUMED_Q_NUM)].u4Size = u4BlkSize;

    // reqest the first block
    if (pfAudBagtRqst)
    {
        pfAudBagtRqst( _rAudBagtInfo.ui2_buf_agt_id, &r_blk_request, AUD_2_BAGT_NFY_COND_DATA_REQ);
    }  
      
    while (1)
    {
        // check if need stop
        if ( _fgBagtStop)
        {
            // send flush command
            if (pfAudBagtRqst)
            {
                pfAudBagtRqst( _rAudBagtInfo.ui2_buf_agt_id, &r_blk_request, AUD_2_BAGT_NFY_COND_FLUSH_REQ);
            }
            break;
        }

        // check if need to send data consumed notification
        if ((u4IdxPlay < u4Idx)||(fgWraped))
        {
            u4CheckSize = fgWraped ? (u4Idx + (sizeof(u4IdxPlay)- u4IdxPlay)) : (u4Idx - u4IdxPlay);
            for (i = 0; i <u4CheckSize; i++)
            {

                _u4LastRp = (_u4LastRp == 0) ? 
                        au4RpAddr[(u4IdxPlay% CONSUMED_Q_NUM)] : _u4LastRp;      
                
                if (_IsLastBlkPlayDone(u1DecId, au4RpAddr[(u4IdxPlay% CONSUMED_Q_NUM)], 
                                                      au4WpAddr[(u4IdxPlay% CONSUMED_Q_NUM)]))
                {
                    if (pfAudBagtRqst)
                    {
                        pfAudBagtRqst( _rAudBagtInfo.ui2_buf_agt_id, 
                            &ar_blk_consume[(u4IdxPlay% CONSUMED_Q_NUM)], 
                                AUD_2_BAGT_NFY_COND_DATA_CONSUMED);
                    }
                    LOG(7, "block (%d) play done\n",ar_blk_consume[(u4IdxPlay% CONSUMED_Q_NUM)].ui4_req_id);
                    u4IdxPlay++;
                    fgWraped = (u4IdxPlay == 0) ? FALSE:fgWraped;
                    _u4LastRp = 0;
                    _u1Timer = 0;
                    _u4CurrRqstId = u4IdxPlay;
                    _u4CurrOffset =0;
                }
                else
                {
                    break;
                }
            }
        }
        
        if ((_u4LastRp == 0) && ((u4Idx!= 0)||(fgWraped)))
        {
            _u4CurrRqstId = u4IdxPlay-1;
            _u4CurrOffset =  ar_blk_consume[((u4IdxPlay-1) % CONSUMED_Q_NUM)].u4Size;
        }
        else
        {
            // update _u4CurrRqstId and _u4CurrOffset
            _u4CurrRqstId = u4IdxPlay;
            _u4CurrOffset = ar_blk_consume[(u4IdxPlay % CONSUMED_Q_NUM)].u4Size - 
                _GetWpMinusRp(u1DecId, au4WpAddr[(u4IdxPlay % CONSUMED_Q_NUM)]);  
        }
        LOG(7, " : RqstId=%d; Offset=%d \n", _u4CurrRqstId,_u4CurrOffset);  
        
        if (_rAudBagtAck.fgAlignChanged)
        {
            // new alignment and min block size info
            // release the original memory and reallocate another one
            LOG(4, " reset alignment info \n");
            for (i = 0; i <BAGT_BUF_NUM; i++)
            {
                if (arBufddr[i].pui1_buff != NULL)
                {
                    x_mem_free(arBufddr[i].pui1_buff);
                }
            }
            
            _rAudBagtInfo.ui4_min_blk_size = (UINT32)((_rAudBagtInfo.ui4_min_blk_size == 0)?
                 1 : _rAudBagtInfo.ui4_min_blk_size);
            
            u4BlkSize = u4AFifofSZ-(u4AFifofSZ % (_rAudBagtInfo.ui4_min_blk_size));
            
            // allocate buffers for buffer agent and set correct alignment
            for (i = 0; i <BAGT_BUF_NUM; i++)
            {
                arBufddr[i].pui1_buff = x_mem_alloc(u4BlkSize + _rAudBagtInfo.ui4_align);
                if (arBufddr[i].pui1_buff == NULL)
                {
                    LOG(8, " Fail to creat buffer(%d) for buffer agent\n", i);
                    UNUSED(AUD_DRVCmd(u1DecId, AUD_CMD_STOP));
                }
                else
                {
                    arBufddr[i].pui1AllignedAddr = (_rAudBagtInfo.ui4_align == 0) ? 
                        ((UINT32)arBufddr[i].pui1_buff) : 
                        ( ((UINT32)arBufddr[i].pui1_buff + _rAudBagtInfo.ui4_align) -  
                            (((UINT32)arBufddr[i].pui1_buff)%(_rAudBagtInfo.ui4_align)));              
                }
            }
    
            r_blk_request.pui1_buff = (UINT8*)arBufddr[u1BufId].pui1AllignedAddr;
            r_blk_request.ui4_req_id = u4Idx ;
            r_blk_request.u4Size = u4BlkSize;

            _rAudBagtAck.fgAlignChanged = FALSE;
        }

        if (_rAudBagtAck.eNtyCond == AUD_BUF_AGT_NFY_COND_DATA_REQ_DONE)
        {
            // if request done,  
            // 1. set the curent copy info and change Nty Cond
            // 2.request next blk. 3.copy current blk to AFIFO.(4k bytes/time)
            // 4.send data consumed notify
            
            // 1. set the curent copy info
            u4Residual = _rAudBagtAck.u4Size;
            u4StreamAddr = (UINT32)_rAudBagtAck.pui1_buff;
            _rAudBagtAck.eNtyCond = AUD_BUF_AGT_NFY_COND_NONE;
            // update request size;
            ar_blk_consume[(u4Idx% CONSUMED_Q_NUM)].u4Size = u4Residual;
            ar_blk_consume[(u4Idx% CONSUMED_Q_NUM)].pui1_buff =  _rAudBagtAck.pui1_buff;
            au4WpAddr[(u4Idx% CONSUMED_Q_NUM)] = ((u4DesAddr + u4Residual) <= u4GetAFIFOEnd(u1DecId)) ?
                     (u4DesAddr + u4Residual) : ((u4DesAddr + u4Residual) - u4AFifofSZ);
            
            // 2.request next blk
            u1BufId = (UINT8)((u1BufId+1)% BAGT_BUF_NUM);
            r_blk_request.pui1_buff = (UINT8*)arBufddr[u1BufId].pui1AllignedAddr;
            r_blk_request.ui4_req_id = u4Idx + 1 ;
            r_blk_request.u4Size = u4BlkSize;
            ar_blk_consume[((u4Idx+1)% CONSUMED_Q_NUM)].pui1_buff =  (UINT8*)arBufddr[u1BufId].pui1AllignedAddr;
            ar_blk_consume[((u4Idx+1)% CONSUMED_Q_NUM)].ui4_req_id = u4Idx + 1;
            ar_blk_consume[((u4Idx+1)% CONSUMED_Q_NUM)].u4Size = u4BlkSize;
            
            if (pfAudBagtRqst)
            {
                pfAudBagtRqst( _rAudBagtInfo.ui2_buf_agt_id, &r_blk_request, AUD_2_BAGT_NFY_COND_DATA_REQ);
            }
            
            // 3.copy current blk to AFIFO.
            while (u4Residual > 0)
            {
                if ( _fgBagtStop )
                {
                   break;
                }
                
                // check if need send data consumed notification
                if ((u4IdxPlay < u4Idx)||(fgWraped))
                {
                    u4CheckSize = fgWraped ? (u4Idx + (sizeof(u4IdxPlay)- u4IdxPlay)) : (u4Idx - u4IdxPlay);
                    for (i = 0; i <u4CheckSize; i++)
                    {
                        _u4LastRp = (_u4LastRp == 0) ? 
                                au4RpAddr[(u4IdxPlay% CONSUMED_Q_NUM)] : _u4LastRp;    
                        
                        if (_IsLastBlkPlayDone(u1DecId, au4RpAddr[(u4IdxPlay% CONSUMED_Q_NUM)], 
                                                              au4WpAddr[(u4IdxPlay% CONSUMED_Q_NUM)]))
                        {
                            if (pfAudBagtRqst)
                            {
                                pfAudBagtRqst( _rAudBagtInfo.ui2_buf_agt_id, 
                                    &ar_blk_consume[(u4IdxPlay% CONSUMED_Q_NUM)], 
                                    AUD_2_BAGT_NFY_COND_DATA_CONSUMED);
                            }
                            LOG(7, "block (%d) play done\n",ar_blk_consume[(u4IdxPlay% CONSUMED_Q_NUM)].ui4_req_id);
                            u4IdxPlay++;
                            fgWraped = (u4IdxPlay == 0) ? FALSE:fgWraped;
                            _u4LastRp = 0;
                            _u1Timer = 0;
                            _u4CurrRqstId = u4IdxPlay;
                            _u4CurrOffset =0;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                
                // update _u4CurrRqstId and _u4CurrOffset
                _u4CurrRqstId = u4IdxPlay;
                _u4CurrOffset = ar_blk_consume[(u4IdxPlay % CONSUMED_Q_NUM)].u4Size - 
                    _GetWpMinusRp(u1DecId, au4WpAddr[(u4IdxPlay % CONSUMED_Q_NUM)]);  
                LOG(7, " : RqstId=%d; Offset=%d \n", _u4CurrRqstId,_u4CurrOffset);
    
                // transfer data 
                u4TransferSZ = (u4Residual > TRANSFER_SZ) ? TRANSFER_SZ : u4Residual ;
                LOG(8, "adr: 0x%08x  Des: 0x%08x, Size: 0x%08x, Res: 0x%08x\n", 
                            u4StreamAddr, u4DesAddr, u4TransferSZ, u4Residual);

                // Check if MultiMedia Support
                if ((IS_MultiMediaSupport()==0) && 
                    _arAudDecoder[u1DecId].eDecFormat == AUD_FMT_MP3)
                {
                    x_thread_delay(10);
                }
                else if (!PSR_SoftTransfer(PSR_STREAM_AUDIO, u1DecId, 
                                          u4StreamAddr, u4TransferSZ, 
                                          u4DesAddr, u4GetAFIFOStart(u1DecId), 
                                          u4GetAFIFOEnd(u1DecId), FALSE))
                {
                   LOG(1, "BAGT soft transter fail/terminated\n");
                    break;
                }
                
                u4DesAddr = ((u4DesAddr + u4TransferSZ) < u4GetAFIFOEnd(u1DecId)) ?
                     (u4DesAddr + u4TransferSZ) : ((u4DesAddr + u4TransferSZ) - u4AFifofSZ);
                u4Residual -= u4TransferSZ;
                u4StreamAddr += u4TransferSZ;
            }
            
             // 4.send data consumed notify
            LOG(7, "block (%d) transfer done\n",u4Idx);
            au4WpAddr[(u4Idx% CONSUMED_Q_NUM)] = u4DesAddr;
            au4RpAddr[(u4Idx% CONSUMED_Q_NUM)] = DSP_GetDspReadPtrPhysicalAddr(u1DecId);
            ar_blk_consume[(u4Idx% CONSUMED_Q_NUM)].ui4_req_id = u4Idx;
            u4Idx++; 
            fgWraped = (u4Idx==0) ? TRUE :fgWraped;
        }
        else if (_rAudBagtAck.eNtyCond == AUD_BUF_AGT_NFY_COND_DATA_REQ_ERROR) 
        {
            // request error, request again
            /*
            r_blk_request.pui1_buff = (UINT8*)arBufddr[u1BufId].pui1AllignedAddr;
            r_blk_request.ui4_req_id = u4Idx + 1 ;
            r_blk_request.u4Size = u4BlkSize;
            */
            _rAudBagtAck.eNtyCond = AUD_BUF_AGT_NFY_COND_NONE;
            if (pfAudBagtRqst)
            {
                pfAudBagtRqst( _rAudBagtInfo.ui2_buf_agt_id, &r_blk_request, AUD_2_BAGT_NFY_COND_DATA_REQ);
            }
        }    
        /*
        else if ((_rAudBagtAck.eNtyCond == AUD_BUF_AGT_NFY_COND_DATA_REQ_CANCELED) ||
                             (_rAudBagtAck.eNtyCond == AUD_BUF_AGT_NFY_COND_FLUSH_DONE) )
        {
            // if request canceled : break
            //LOG(1, "AUD_BUF_AGT_NFY_COND_DATA_REQ_CANCELED\n");
            //AUD_DRVCmd(u1DecId, AUD_CMD_STOP);
            break;
        }
        */
        else
        {
            x_thread_delay(5);
        }
    } //end of while (1)

    _u4CurrRqstId = 0;
    _u4CurrOffset = 0;
    
    // free buffers 
    for (i = 0; i <BAGT_BUF_NUM; i++)
    {
        if (arBufddr[i].pui1_buff != NULL)
        {
            x_mem_free(arBufddr[i].pui1_buff);
        }
    }
    
    // destroy thread
    x_thread_exit();
}

static BOOL _fgFeederThreadGo = FALSE;

#define FEEDER_EMPTY_CNT_THREASHOLD 3


static HANDLE_T _hSemaFeederThreadWait;

static void _AudFeederThread (const void* pvArg)
{
    UINT8 u1DecId = AUD_DEC_MAIN;
    UINT32 u4TransferSZ;
    UINT32 u4DesAddr;
    UINT32 u4AFifofSZ;
    UINT32 u4StreamAddr; 
    FEEDER_TRANSMIT_COND rFeederTransmitData = {0};
    BOOL   fgFeedInit = FALSE;
    BOOL   fgFeedEnd = FALSE;

    if (pvArg != NULL)
    {
        u1DecId = *(UINT8 *)pvArg;
    }
    
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));

    while (1)
    {
        VERIFY(x_sema_lock(_hSemaFeederThreadWait, X_SEMA_OPTION_WAIT) == OSR_OK);

        fgFeedInit = FALSE;
        fgFeedEnd = FALSE;

        //FIXmt5381
        if (u1DecId == AUD_DEC_MAIN)
        {
	        AUD_CLR_BIT(REG_BS_CFG, PAR_SEL);
        }
        else
        {
	        AUD_SET_BIT(REG_BS_CFG, PAR_SEL);
        }

        // Initial setup
        _fgFeederThreadGo = TRUE;
        u4DesAddr = PHYSICAL(u4GetAFIFOStart(u1DecId));
        u4AFifofSZ = u4GetAFIFOEnd(u1DecId) - u4GetAFIFOStart(u1DecId);
        u4StreamAddr = PHYSICAL((UINT32)_arAudDecoder[u1DecId].rMemBuffer.pData);

        while (_fgFeederThreadGo)
        {
            INT32 i4Ret;
            
            // Get data source
            if (_rFeedTransmitInfo.pfnTransHandle)
            {
                i4Ret = _rFeedTransmitInfo.pfnTransHandle(FEEDER_AUDIO_SOURCE, 
                         FEEDER_REQUEST_DATA_SIZE, 0, &rFeederTransmitData, 0);
                UNUSED(i4Ret);
       
                u4StreamAddr = PHYSICAL(rFeederTransmitData.u4BufAddr);
                u4TransferSZ = rFeederTransmitData.u4TransmitSize;

                LOG(9, "Feeder Dec(%d) Src: 0x%08x, Des: 0x%08x, Size: 0x%08x\n", 
                    u1DecId, u4StreamAddr, u4DesAddr, u4TransferSZ);

                if (u4TransferSZ > 0)
                {
                    if (!PSR_SoftTransfer(PSR_STREAM_AUDIO, u1DecId, 
                                      u4StreamAddr, u4TransferSZ, 
                                      u4DesAddr, u4GetAFIFOStart(u1DecId), 
                                      u4GetAFIFOEnd(u1DecId), FALSE))
                    {
                        LOG(5, "Aud soft transter fail/terminated\n");
                        break;
                    }

                    if ((_rFeedTransmitInfo.pfnNotify) && !fgFeedInit)
                    {
                        INT32 i4Ret;
                        i4Ret = _rFeedTransmitInfo.pfnNotify(FEEDER_AUDIO_SOURCE, 
                                        FEEDER_NOTIFY_FILEBEGIN, 0);
                        UNUSED(i4Ret);
                        fgFeedInit = TRUE;
                    }
                }
                else // u4TransferSZ = 0  =>  no data
                {
                    x_thread_delay(50);
            
                    if ((_rFeedTransmitInfo.pfnNotify) && fgFeedInit && !fgFeedEnd)
                    {
                        if (AUD_IsFifoEmpty(u1DecId))
                        {
                            i4Ret = _rFeedTransmitInfo.pfnNotify(FEEDER_AUDIO_SOURCE, 
                                        FEEDER_NOTIFY_FILEEND, 0);
                            fgFeedEnd = TRUE;            
                            UNUSED(i4Ret);
                        }    
                    }
                }

                u4DesAddr = ((u4DesAddr + u4TransferSZ) < u4GetAFIFOEnd(u1DecId)) ?
                             (u4DesAddr + u4TransferSZ) : ((u4DesAddr + u4TransferSZ) - u4AFifofSZ);
        
            }
            else
            {
                x_thread_delay(50);
            }

        }
    }
    // destroy thread
    //x_thread_exit();  //FIXME!! 2007/11/22 The statement is unreachable (no break condition in while(1)), so mark it temporary.
}

static void _LogPts(UINT8 u1PidIdx, UINT8 u1DevId, const DMX_AUDIO_PES_T * prPes)
{
    STC_CLOCK rStc;
    static UINT32 u4PayloadSize = 0;
    static UINT32 u4LastTimeWp = 0;

      
    u4PayloadSize = (prPes->u4Wp >= u4LastTimeWp) ? (prPes->u4Wp - u4LastTimeWp) : 
                    (_arAudDecoder[u1DevId].u4FifoSz + prPes->u4Wp - u4LastTimeWp);
    u4LastTimeWp = prPes->u4Wp;
    

    if (STC_GetSrc(STC_SRC_A1, &rStc) == STC_VALID)  //Audio Description - we only have 1 STC
    {
        if (prPes->u4Pts >= rStc.u4Base)
        {
            //lint -e{613}
            LOG(11, "PidIdx:%u, dev: %u, PTS: 0x%08x, Payload: 0x%08x, size: %d, STC:0x%08x, delta: %d\n",
                    u1PidIdx, u1DevId, (UINT32)(prPes->u4Pts & 0xffffffff),
                    prPes->u4Wp, u4PayloadSize, rStc.u4Base, prPes->u4Pts - rStc.u4Base);                
        }
        else
        {
            //lint -e{613}
            LOG(11, "PidIdx:%u, dev: %u, PTS: 0x%08x, Payload: 0x%08x, size: %d, STC:0x%08x, delta: -%d\n",
                    u1PidIdx, u1DevId, (UINT32)(prPes->u4Pts & 0xffffffff),
                    prPes->u4Wp, u4PayloadSize, rStc.u4Base, rStc.u4Base - prPes->u4Pts);          
        }
   }

    UNUSED(u4PayloadSize);
    
}


static BOOL _IsPtsValid(UINT8 u1PidIdx, UINT8 u1DevId, const DMX_AUDIO_PES_T * prPes)
{
    static UINT32 u4PayloadSize = 0;
    static UINT32 u4LastTimeWp = 0;
    static UINT32 u4OldPts = 0;
    static UINT8 u1PtsErrorFlag = 0;
    static UINT8 u1FirstPts = TRUE;
    STC_CLOCK rStc;

    if (prPes != NULL)
    {
        u4PayloadSize = prPes->u4Wp - u4LastTimeWp;
        u4LastTimeWp = prPes->u4Wp;

        //if (STC_GetSrc((u1DevId == AUD_DEC_MAIN) ? STC_SRC_A1 : STC_SRC_A2, &rStc) == STC_VALID)
        if (STC_GetSrc(STC_SRC_A1, &rStc) == STC_VALID)  //Audio Description - we only have 1 STC
        {
            if (prPes->u4Pts >= rStc.u4Base)
            {
                //lint -e{613}
                LOG(11, "PidIdx:%u, dev: %u, PTS: 0x%08x, Payload: 0x%08x, size: %d, STC:0x%08x, delta: %d\n",
                    u1PidIdx, u1DevId, (UINT32)(prPes->u4Pts & 0xffffffff),
                    prPes->u4Wp, u4PayloadSize, rStc.u4Base, prPes->u4Pts - rStc.u4Base);                
            }
            else
            {
                //lint -e{613}
                LOG(11, "PidIdx:%u, dev: %u, PTS: 0x%08x, Payload: 0x%08x, size: %d, STC:0x%08x, delta: -%d\n",
                    u1PidIdx, u1DevId, (UINT32)(prPes->u4Pts & 0xffffffff),
                    prPes->u4Wp, u4PayloadSize, rStc.u4Base, rStc.u4Base - prPes->u4Pts);          
            }
#if 0
            {
                UINT32 u4ReadPtr = 0;
                UINT32 u4WritePtr = 0;
                _AUD_DMX_GetAudPtr(u1DevId, &u4ReadPtr, &u4WritePtr);
                LOG(10, "RP: 0x%08x, WP: 0x%08x, DMX WP: 0x%08x\n", u4ReadPtr, u4WritePtr, prPes->u4Wp);
            }
#endif
            // To suppress warning messages for release version
            UNUSED(u4PayloadSize);

            if (_arAudDecoder[u1DevId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
            {
                // For multimedia case, always return TRUE
                return TRUE;
            }

            // Check if PTS < STC
            if (prPes->u4Pts < rStc.u4Base)
            {
                LOG(11, "PTS error: invalid PTS (PTS - STC < 0)\n");
                return FALSE;
            }            

            // Check if sudden different PTS occurs
            if (u1FirstPts)
            {
                u4OldPts = prPes->u4Pts;
                u1FirstPts = FALSE;
            }
            else if (((u4OldPts > prPes->u4Pts) && (u4OldPts - prPes->u4Pts > 1000000)) ||
                ((u4OldPts < prPes->u4Pts) && (prPes->u4Pts -u4OldPts > 1000000)))
            {
                if (u1PtsErrorFlag == 0)
                {
                    u1PtsErrorFlag = 1;
                    LOG(9, "PTS error: difference too large (%d -> %d)\n", u4OldPts, prPes->u4Pts);
                    return FALSE;
                }
                else
                {
                    u1PtsErrorFlag = 0;
                    u4OldPts = prPes->u4Pts;
                }
            }
            else
            {
                u1PtsErrorFlag = 0;
                u4OldPts = prPes->u4Pts;
            }

            return TRUE;
        }
    }

    return FALSE;
}

static void _LoadAdspCode(UINT8 u1DecId, AUD_FMT_T eStreamFormat)
{
    UINT32 u4StreamFormat = AUD_PINK_DEC1;
    
    AUD_DEC_ID_VALIDATE(u1DecId);
    
    if (u1DecId == AUD_DEC_MAIN)  // first decoder
    {
        switch(eStreamFormat)
        {
            case AUD_FMT_MPEG:
                u4StreamFormat = AUD_MPEG_DEC1;
                break;
            case AUD_FMT_AC3:
                u4StreamFormat = AUD_AC3_DEC1;
                break;
			case AUD_FMT_AAC:		
				u4StreamFormat = AUD_AAC_DEC1;
				break;	
            case AUD_FMT_MP3:
                u4StreamFormat = AUD_MP3_DEC1;
                break;
            case AUD_FMT_WMA:
                u4StreamFormat = AUD_WMA_DEC1;
                break;    
            case AUD_FMT_PCM:
                u4StreamFormat = AUD_PCM_DEC1;
                break;
            case AUD_FMT_MTS:
                u4StreamFormat = AUD_MTS_DEC1;
                break;  
            case AUD_FMT_A2:
                u4StreamFormat = AUD_A2DEC_DEC1;    
                break;
            case AUD_FMT_PAL:
                u4StreamFormat = AUD_PAL_DEC1;  
                break;
            case AUD_FMT_FMFM:
                u4StreamFormat = AUD_FMFM_DEC1; 
                break;
            case AUD_FMT_DETECTOR:
                u4StreamFormat = AUD_DETECTOR_DEC1; 
                break;
            case AUD_FMT_MINER:
                u4StreamFormat = AUD_MINER_DEC1; 
                break;    
            case AUD_FMT_UNKNOWN:
                u4StreamFormat = AUD_PINK_DEC1;
                break;              
            case AUD_FMT_LPCM:
                u4StreamFormat = AUD_LPCM_DEC1;
                break;
            case AUD_FMT_FMRDO:
                u4StreamFormat = AUD_FMRDO_DEC1;
                break;
            case AUD_FMT_FMRDO_DET:
                u4StreamFormat = AUD_FMRDODET_DEC1;
                break;
             case AUD_FMT_SBC:
                u4StreamFormat = AUD_SBC_DEC1;
                break;
            default:
                u4StreamFormat = AUD_PINK_DEC1;
                break;
        }
        // load ADSP decoder's code while decoder is stopped
        DSP_ResetDec(u1DecId);
        LOG( 7,"LoadAdspCode(0) = %s\n", _aszCodeType[eStreamFormat]);
        DSP_LoadRamCode(u4StreamFormat);
        #ifdef SIF_ADC          
		vDspLoadRamCode (AUD_AININTRAM);
             if (u4StreamFormat == AUD_MINER_DEC1) 
             {
             vDspLoadRamCode (AUD_AININTRAM_MINER);
             }
        #endif
    }
#ifndef CC_MT5391_AUD_3_DECODER    
//#if 1
    else // Aux decoder
#else
    else if (u1DecId == AUD_DEC_AUX)  // second decoder
#endif
    {
        switch(eStreamFormat)
        {
            case AUD_FMT_MPEG:
                u4StreamFormat = AUD_MPEG_DEC2;
                break;
            case AUD_FMT_AC3:
                u4StreamFormat = AUD_AC3_DEC2;
                break;
			case AUD_FMT_AAC:						
				u4StreamFormat = AUD_AAC_DEC2;				
				break;	
			case AUD_FMT_PCM:
                u4StreamFormat = AUD_PCM_DEC2;
                break;
            case AUD_FMT_MTS:
                u4StreamFormat = AUD_MTS_DEC2;
                break;  
            case AUD_FMT_A2:
                u4StreamFormat = AUD_A2DEC_DEC2;    
                break;
            case AUD_FMT_PAL:
                u4StreamFormat = AUD_PAL_DEC2;  
                break;
            case AUD_FMT_FMFM:
                u4StreamFormat = AUD_FMFM_DEC2; 
                break;
            case AUD_FMT_DETECTOR:
                u4StreamFormat = AUD_DETECTOR_DEC2; 
                break;
            case AUD_FMT_MINER:
                u4StreamFormat = AUD_MINER_DEC2; 
                break;    
            #ifdef CC_AUDIO_DSP_JPG_SUPPORT ///ADSP_JPEG_DEC2           
            case AUD_FMT_JPEG:
                u4StreamFormat = AUD_JPEG_DEC2;
                break;
            #endif   
            default:
                LOG(1, "Invalid switch in _LoadAdspCode\n");
                break;
        }
        // load ADSP decoder's code while decoder is stopped
        DSP_ResetDec(u1DecId);
        LOG( 7,"LoadAdspCode(1) = %s\n", _aszCodeType[eStreamFormat]);
        DSP_LoadRamCode(u4StreamFormat);
    }
#ifdef CC_MT5391_AUD_3_DECODER 
//#if 0
    else
    {
        switch(eStreamFormat)
        {
            case AUD_FMT_MPEG:
                u4StreamFormat = AUD_MPEG_DEC3;
                break;           
            case AUD_FMT_PCM:
                u4StreamFormat = AUD_PCM_DEC3;
                break;                        
            case AUD_FMT_AAC:
                u4StreamFormat = AUD_AAC_DEC3;
                break;                        
            case AUD_FMT_AC3:
                u4StreamFormat = AUD_AC3_DEC3;
                break;                        
            default:
                LOG(1, "Invalid switch in _LoadAdspCode\n");
                break;
        }
        // load ADSP decoder's code while decoder is stopped
        DSP_ResetDec(u1DecId);
        LOG( 7,"LoadAdspCode(2) = %s\n", _aszCodeType[eStreamFormat]);
        DSP_LoadRamCode(u4StreamFormat);
    }
#endif    
}

/*
static UINT8 _PidIdxToDecId(UINT8 u1PidIdx)
{
    INT32 i;
    
    for (i=0, i< AUD_DEC_NUM; i++)
    {
        if (_arAudDecoder[AUD_DEC_MAIN].u1DmxPidIdx == u1PidIdx)
        {
            break;
        }
    }   

    UNUSED(u1PidIdx);
    return AUD_DEC_MAIN;
}
*/

static UINT8 _DecIdToPidIdx(UINT8 u1DecId)
{
    return _arAudDecoder[u1DecId].u1DmxPidIdx;
}

static BOOL _DmxPidIdxValid(UINT8 u1DecId)
{
    return (_arAudDecoder[u1DecId].u4ReceivePesCount > 0) ? TRUE : FALSE;
}

static BOOL _IsVideoStable(void)
{
    return(fgApiVideoStable(_u1VideoFrameFlag));
}

#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0521  
static BOOL _AvSyncControl(UINT8 u1DecId)
{
    BOOL fgRet = FALSE;

	if(AUD_GetTargetCountry() != COUNTRY_EU)
	{
        // Only DTV & ATV need AV synchronization
        if (((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)||(_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) ) &&
            (AUD_GetAvSynMode(u1DecId) != AV_SYNC_FREE_RUN) &&
            (!_AudDmxDtvInitPlaybackFifoFull(u1DecId)))
        {
            fgRet = TRUE;
        }
//#ifndef CC_AUD_DVBT_SUPPORT    
        else if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_LINE_IN) &&
        	       (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) &&
                 (!_afgAtvVideoDisplayFlag[u1DecId]) &&
                 (!_IsVideoStable()) &&
                 (_arAudDecoder[u1DecId].eDecFormat !=  AUD_FMT_DETECTOR))
        {
            fgRet = TRUE;
        }
	}
//#else
    else		
    {  
        if (((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)||(_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)) &&
            (AUD_GetAvSynMode(u1DecId) != AV_SYNC_FREE_RUN) &&
            (!_AudDmxDtvInitPlaybackFifoFull(u1DecId)))
        {
            fgRet = TRUE;
        }

        else if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_LINE_IN) &&
                 (_eDualDecStreamSrc == AUD_STREAM_FROM_ANALOG_TUNER) &&
                 (!_afgAtvVideoDisplayFlag[u1DecId]) &&
                 (!_IsVideoStable()) &&
                 (_arAudDecoder[u1DecId].eDecFormat !=  AUD_FMT_DETECTOR))
//#endif
        {
            fgRet = TRUE;
        }
    }
    return fgRet;
}
#else
static BOOL _AvSyncControl(UINT8 u1DecId)
{
    BOOL fgRet = FALSE;

    // Only DTV & ATV need AV synchronization
    if (((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)||(_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)) &&
        (AUD_GetAvSynMode(u1DecId) != AV_SYNC_FREE_RUN) &&
        (!_AudDmxDtvInitPlaybackFifoFull(u1DecId))&&((u1DecId==AUD_DEC_AUX)||(u1DecId==AUD_DEC_THIRD)))
    {
        fgRet = TRUE;
    }   
    else if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
            (AUD_GetAvSynMode(u1DecId) != AV_SYNC_FREE_RUN) &&
            (!_AudDmxDtvInitPlaybackFifoFull(u1DecId)))
    {
        fgRet = TRUE;
    }   
    else if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) &&
             (!_afgAtvVideoDisplayFlag[u1DecId]) &&
             (!_IsVideoStable()) &&
             ((_arAudDecoder[u1DecId].eDecFormat !=  AUD_FMT_DETECTOR)&&(_arAudDecoder[u1DecId].eDecFormat !=  AUD_FMT_FMRDO)
                 &&(_arAudDecoder[u1DecId].eDecFormat !=  AUD_FMT_FMRDO_DET))
             )
    {
        fgRet = TRUE;
    }
    else if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_LINE_IN) &&
             (!_afgAtvVideoDisplayFlag[u1DecId]) &&
             (!_IsVideoStable()))
    {
        fgRet = TRUE;
    }    

    return fgRet;
}
#endif

static void _ChkAudioOnlyControl(UINT8 u1DecId, BOOL fgReset)
{
    //Andrew Wen 2007/6/22 Audio only program set sync mode to free run.
    static UINT32 u4OldStcBase = 0;
    BOOL fgFreeRun = FALSE;
    STC_CLOCK rStc;
    static UINT32 u4LastRetrieveNs = 0;

    //if (STC_GetSrc((u1DecId == AUD_DEC_MAIN) ? STC_SRC_A1 : STC_SRC_A2, &rStc) != STC_VALID)
    if (STC_GetSrc(STC_SRC_A1, &rStc) != STC_VALID) //Audio Description
    {
        if (fgReset)
        {        
            u4OldStcBase = 0;    
        }
        return;                   
    }
   
    if (fgReset)
    {
        u4OldStcBase = rStc.u4Base;
        
        //AUD_SetAvSynMode(u1DecId, AV_SYNC_SLAVE);
        if (_arAudDecoder[u1DecId].eSynMode == AV_SYNC_FREE_RUN)
        {
            LOG(7,"Dec(%d) Middleware sets start sync mode = Free run, u4RetrieveNs=%x, u4DecOkNs=%x\n", u1DecId, _prMPV->u4RetrieveNs, _prMPV->u4DecOkNs);
        }
        else if (_arAudDecoder[u1DecId].eSynMode == AV_SYNC_SLAVE)
        {
            LOG(7,"Dec(%d) Middleware sets start sync mode = AV sync, u4RetrieveNs=%x, u4DecOkNs=%x\n", u1DecId, _prMPV->u4RetrieveNs, _prMPV->u4DecOkNs);
        }
        else
        {
            LOG(7,"Dec(%d) Middleware sets start sync mode = Audio master, u4RetrieveNs=%x, u4DecOkNs=%x\n", u1DecId, _prMPV->u4RetrieveNs, _prMPV->u4DecOkNs);
        }
        
        u4LastRetrieveNs = _prMPV[0].u4RetrieveNs;
    }
    else
    {        
        _prMPV = &(prDbase->rMpv[0]);
        if ((rStc.u4Base - u4OldStcBase) > 90000)
        {
            /*
            LOG(7,"# STC=%x\n", rStc.u4Base);
            LOG(7,"  u4RetrieveNs=%x, u4DecOkNs=%x, mode =%d\n", _prMPV->u4RetrieveNs, _prMPV->u4DecOkNs, _arAudDecoder[u1DecId].eSynMode);
            LOG(7,"  pts_stc = %x, %x\n",u4ReadDspSram(0x156a), u4ReadDspSram(0x156b));
            */
            u4OldStcBase = rStc.u4Base;            
            if (((u4LastRetrieveNs - _prMPV[0].u4RetrieveNs) ==0) && (_prMPV[0].u4DecOkNs == 0))
            {            
                fgFreeRun = TRUE;
            }
            u4LastRetrieveNs = _prMPV[0].u4RetrieveNs;
                        
            if ((fgFreeRun) && (_arAudDecoder[u1DecId].eSynMode != AV_SYNC_FREE_RUN)
                                && (_afgIssuePlayComToDsp[u1DecId]))
            {
                LOG(7,"_ChkAudioOnlyControl Set Sync to Free Run\n");
                AUD_SetAvSynMode(u1DecId, AV_SYNC_FREE_RUN);

                // Clear PTS in DSP
                AUD_ClearPtsInISR(u1DecId);
            }
            else if ((!fgFreeRun) && (_arAudDecoder[u1DecId].eSynMode == AV_SYNC_FREE_RUN))
            {
                LOG(7,"_ChkAudioOnlyControl Set Sync to Slave Mode\n");
                AUD_SetAvSynMode(u1DecId, AV_SYNC_SLAVE);
            }
        }
    }    
}
void _AUD_DrvApllMonThreadGo(BOOL fgGo)
{
    _fgApllMonThrRun = fgGo;    
}

static SAMPLE_FREQ_T _GetHdmiSmpRate(void)
{
    UINT8 u1HdmiAudSf;
    SAMPLE_FREQ_T eAudDrvSampleFreq;
    
    u1HdmiAudSf = (u1HDMIGetAudioSamplingFreq() & 0x0F);

    if (u1HdmiAudSf == 0)
    {
        eAudDrvSampleFreq = FS_44K;
    }
    else if (u1HdmiAudSf == 0x3)
    {
        eAudDrvSampleFreq = FS_32K;
    }
    else if (u1HdmiAudSf == 0x2)
    {
        eAudDrvSampleFreq = FS_48K;
    }
    else if (u1HdmiAudSf == 0x8)
    {
        eAudDrvSampleFreq = FS_88K;
    }
    else if (u1HdmiAudSf == 0xa)
    {
        eAudDrvSampleFreq = FS_96K;
    }
    else if (u1HdmiAudSf == 0xc)
    {
        eAudDrvSampleFreq = FS_176K;
    }
    else if (u1HdmiAudSf == 0xe)
    {
        eAudDrvSampleFreq = FS_192K;
    }
    else
    {   // 0x0 or default
        eAudDrvSampleFreq = FS_44K;
        LOG(3, "GetHdmiSmpRate (not indicate)\n");
    }

    return eAudDrvSampleFreq;
}

static SAMPLE_FREQ_T _eHdmiSmpRate = FS_48K;

static void _DecoderInitSetup(UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt)
{
    UINT8 u1HdmiAudSf;
    SAMPLE_FREQ_T eAudDrvSampleFreq;

#ifdef CC_MT5391_AUD_3_DECODER    
    if (u1DecId == AUD_DEC_MAIN)
    {
        DSP_SetDualDecMode(FALSE);
        AUD_ClkCfgDualDecMode(FALSE); //0714
    }    
#endif  

    if (eDecFmt == AUD_FMT_PCM)
    {
        if ((eStreamFrom == AUD_STREAM_FROM_MEMORY) ||
            (eStreamFrom == AUD_STREAM_FROM_BUF_AGT) ||
            (eStreamFrom == AUD_STREAM_FROM_FEEDER))
        {
            DSP_SetPcmType(u1DecId, _arAudDecoder[u1DecId].rPcmSetting.ePcmDataInfo);
            DSP_SetPcmFs(u1DecId, FS_44K); // default set as 44.1K
        }
        else if (eStreamFrom == AUD_STREAM_FROM_HDMI)
        {
            u1HdmiAudSf = (u1HDMIGetAudioSamplingFreq() & 0x0F);

            if (u1HdmiAudSf == 0)
            {   
                eAudDrvSampleFreq = FS_44K;
            }
            if (u1HdmiAudSf == 0x3)
            {
                eAudDrvSampleFreq = FS_32K;
            }
            else if (u1HdmiAudSf == 0x2)
            {
                eAudDrvSampleFreq = FS_48K;
            }
            else if (u1HdmiAudSf == 0x8)
            {
                eAudDrvSampleFreq = FS_88K;
            }
            else if (u1HdmiAudSf == 0xa)
            {
                eAudDrvSampleFreq = FS_96K;
            }
            else if (u1HdmiAudSf == 0xc)
            {
                eAudDrvSampleFreq = FS_176K;
            }
            else if (u1HdmiAudSf == 0xe)
            {
                eAudDrvSampleFreq = FS_192K;
            }
            else
            {   // 0x0 or default
                eAudDrvSampleFreq = FS_44K;
            }

#ifdef USE_16BIT_ALIGNMENT
            DSP_SetPcmType(u1DecId, PCM_16_BITS_BIG_ENDIAN);
#else
            DSP_SetPcmType(u1DecId, PCM_24_BITS);
#endif
            DSP_SetPcmFs(u1DecId, eAudDrvSampleFreq);

            _eHdmiSmpRate = eAudDrvSampleFreq;
            // Setup sampling rate to apll and dac

            LOG(5, " ------------------ Decoder init set sampling rate (%d) -----------\n", _eHdmiSmpRate);
            AUD_SetSampleFreq(AUD_DEC_MAIN, eAudDrvSampleFreq);            

        }
#ifdef CC_MT5391_AUD_3_DECODER     
        else if (eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
        {
            DSP_SetPcmType(u1DecId, PCM_24_BITS);
            DSP_SetPcmFs(u1DecId, FS_48K);
            DSP_SetDualDecMode(TRUE);
            AUD_ClkCfgDualDecMode(TRUE);//0714
        }
        else if (eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER)
        {
            DSP_SetPcmType(u1DecId, PCM_24_BITS);
            DSP_SetPcmFs(u1DecId, FS_32K);
            DSP_SetDualDecMode(TRUE);
            AUD_ClkCfgDualDecMode(TRUE);//0714
        }
#endif
        else if (eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
        {
            //DSP_SetPcmType(u1DecId, PCM_16_BITS_LITTLE_ENDIAN);
            DSP_SetPcmType(u1DecId, _arAudDecoder[u1DecId].rPcmSetting.ePcmDataInfo);
            DSP_SetPcmFs(u1DecId, FS_48K); // default set as 48K
        }	
        else // Other fix bit number
        {
#ifdef USE_16BIT_ALIGNMENT
            DSP_SetPcmType(u1DecId, PCM_16_BITS_BIG_ENDIAN);
#else
            DSP_SetPcmType(u1DecId, PCM_24_BITS);
#endif
            DSP_SetPcmFs(u1DecId, FS_48K); // default set as 48K
        }
        
    }    

#ifdef CC_MT5391_AUD_3_DECODER  
    if ( (eDecFmt == AUD_FMT_MPEG) || 
          (eDecFmt == AUD_FMT_AAC) || 
          (eDecFmt == AUD_FMT_AC3) )
    {
        if (u1DecId == AUD_DEC_THIRD)
        {
            DSP_SetTriDecMode(TRUE);
        }        
    }
#endif      
}

#if 0
// *********************************************************************
// Function : static BOOL _AudDetectFrameIncrease(void)
// Description : Only check Dec1
// Parameter : none
// Return    : Frame increasing (1) or Stopped (0)
// *********************************************************************
static BOOL _AudDetectFrameIncrease(void)
{
    BOOL fgRet = TRUE;
	static UINT32 _u4PrevCnt = 0xFFFFFFFF;
	UINT32 _u4CurCnt;

	_u4CurCnt = u4ReadDspSram(DSP_SRAM_DECODE_FRAME_COUNT);

	if(_u4PrevCnt != _u4CurCnt)
	{
		_u4PrevCnt = _u4CurCnt;
	}
	else
	{
		fgRet = FALSE;
	}
	
	return fgRet;	
}
#endif

static void _ChangeDecFmtOnPlayState(UINT8 u1DecId)
{
    
    //MT5387Merge  
    // Turn off stream input -- move to after STOP command,
    //                          Because it would turn off line-in and may cause DSP hold.
    //                          Add change clock source to internal, becasue we still need clock source for STOP DSP.
    //AUD_StrInputEnable(u1DecId, _arAudDecoder[u1DecId].eStreamFrom, _arAudDecoder[u1DecId].eDecFormat, FALSE, 0);
    if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_HDMI)
    {
        AUD_ClkSetToInternal(u1DecId, AUD_FMT_PCM); //Format is not important here..
    }
     
    if (u1DecId == AUD_DEC_MAIN)
    {
        DSP_SendDspTaskCmd(DSP_STOP);
    }    
#ifndef CC_MT5391_AUD_3_DECODER     
    else
    {
        DSP_SendDspTaskCmd(DSP_STOP_DEC2);
    }
#else
    else if (u1DecId == AUD_DEC_AUX)
    {
        DSP_SendDspTaskCmd(DSP_STOP_DEC2);
    }
    else
    {
        DSP_SendDspTaskCmd(DSP_STOP_DEC3);
    }
#endif
     
    AUD_WaitUopCommandDone(u1DecId);

    
    AUD_StrInputEnable(u1DecId, _arAudDecoder[u1DecId].eStreamFrom, _arAudDecoder[u1DecId].eDecFormat, FALSE, 0);
                    
    VERIFY(x_sema_lock(_hSemaLoadCode, X_SEMA_OPTION_WAIT) == OSR_OK);
    _LoadAdspCode(u1DecId, _arAudDecoder[u1DecId].eDecFormat);
    VERIFY(x_sema_unlock(_hSemaLoadCode) == OSR_OK);

    if (_arAudDecoder[u1DecId].eDecFormat == AUD_FMT_AC3)
    {
       _FlushAudioFifo(u1DecId);
    }

    // Update DSP read pointer and pts
    AUD_ResetDspReadPtr(u1DecId);
    if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)||
         (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))
    {
        DSP_SetStartPtsToShm(u1DecId, (UINT32)(_arAudPes[u1DecId].u4Pts & 0xffffffff), _arAudPes[u1DecId].u4Wp);
    }     
               
    // Turn on stream input
    AUD_StrInputEnable(u1DecId, _arAudDecoder[u1DecId].eStreamFrom, _arAudDecoder[u1DecId].eDecFormat, TRUE, 0);

    x_thread_delay(10);

    // Reset line-in interface after reseting HDMI audio clock to prevent from inverting L/R channels.
#ifndef AUD_HW_LR_INVERSE_DECT                
    AUD_LineInCtrlEnable(u1DecId,FALSE);
    AUD_LineInCtrlEnable(u1DecId,TRUE);
#endif    
     
#ifndef CC_MT5391_AUD_3_DECODER     
    DSP_SendDspTaskCmd((u1DecId == AUD_DEC_MAIN) ? DSP_PLAY : DSP_PLAY_DEC2);
#else
    if (u1DecId == AUD_DEC_MAIN) DSP_SendDspTaskCmd(DSP_PLAY);
    else if (u1DecId == AUD_DEC_AUX) DSP_SendDspTaskCmd(DSP_PLAY_DEC2);
    else DSP_SendDspTaskCmd(DSP_PLAY_DEC3);
#endif
     
    AUD_WaitUopCommandDone(u1DecId);
}

static void _AudHdmiMute(BOOL fgMute)
{
    if (fgMute != _fgHdmiMute)
    {
        // Mute channel volume
        AUD_DspHdmiChangeFormatMuteEnable(AUD_DEC_MAIN, fgMute);
        // Mute master volume
        AUD_HdmiModeMute(AUD_DEC_MAIN, fgMute);

    //#ifndef CC_AUD_DVBT_SUPPORT
    	if(AUD_GetTargetCountry() != COUNTRY_EU)
        {
            // Only for US model
            // Since internal dac may be used in SCART output
            ADAC_CodecHdmiUnstableMute(fgMute);
            #ifdef CC_AUD_AMP_STA333BW //Specific Digital Amp has pop noise.
            ADAC_HdmiUnstableSpkEnable(!fgMute);
            #endif            
    	}
    	else
    	{
    //#endif
            // Mute amplifier
            ADAC_HdmiUnstableSpkEnable(!fgMute);
    	}
    }
    _fgHdmiMute = fgMute; 
}

//#if !defined(CC_AUD_DVBT_SUPPORT) || defined(CC_AUD_DVBT_AC3_BYPASS)
// take off for driver merge US/EU
static BOOL _IsChangeHdmiSpdifAudFormat(AUD_FMT_T* peDecFmt)
{
    BOOL fgDetected = TRUE;
    INT32     i4Ret = FALSE;
    AUD_FMT_T eMode = AUD_FMT_PCM;
    AUD_DECODE_STATUS_T  eDecStatus;
    AudDecNfyFct  pfAudDecNfy = NULL;
    UINT32        u4Tmp = 0;
    
    // Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy) == TRUE);        
    
    // Get detected format
    AUD_AinGetMode(&fgDetected, &eMode);

    // Check detected format and current decoder format
    if (fgDetected)
    {
        // Setup Line-in mode for PCM/RAW
        if (eMode == AUD_FMT_PCM)
        {
            AUD_AinSetMode(0);
        }
        else
        {
            AUD_AinSetMode(1);
        }

        eDecStatus = AUD_DECODE_NOT_SUPPORT;        
        // SPDIF format not support, set SPDIF_out source to HDMI Rx
        if ((eMode == AUD_FMT_UNKNOWN) || (eMode == AUD_FMT_AC3) || (eMode == AUD_FMT_MPEG)) //add by xiaoyi for 8223 only support HDMI pcm format 100331
        {
             AUD_SPDIF_Output_HDMI_Rx(TRUE);
        }
        else
        {
            _u1HDMIUnDetectCnt = 0;
            eDecStatus = AUD_DECODE_NORMAL;             
            //AUD_SPDIF_Output_HDMI_Rx(FALSE); // It is set at setting IEC
        }

        // Check detected format and current decoder format
        if ((eMode == AUD_FMT_PCM) && (_arAudDecoder[AUD_DEC_MAIN].eDecFormat != AUD_FMT_PCM))
        {
             *peDecFmt = AUD_FMT_PCM;
            LOG(3, "<< HDMI detect as PCM >>\n" ); 
            LOG(3, "################## Detected PCM and -- HDMI Mute! #################\n" );
             i4Ret = TRUE;   
        }
        else if ((eMode == AUD_FMT_AC3) && (_arAudDecoder[AUD_DEC_MAIN].eDecFormat != AUD_FMT_AC3))
        {             
             *peDecFmt = AUD_FMT_AC3;
            LOG(3, "<< HDMI detect as AC3 >>\n" );
            LOG(3, "################## Detected AC3 and -- HDMI Mute! #################\n" );             
             i4Ret = TRUE;   
        }
        else if ((eMode == AUD_FMT_MPEG) && 
                 (_arAudDecoder[AUD_DEC_MAIN].eDecFormat != AUD_FMT_MPEG) &&
                 (_arAudDecoder[AUD_DEC_MAIN].eDecFormat != AUD_FMT_MP3))
        {             
             *peDecFmt = AUD_FMT_AC3;
            LOG(3, "<< HDMI detect as MPEG >>\n" );
             i4Ret = TRUE; 
        }
        else if (eMode == AUD_FMT_UNKNOWN) 
        {
            LOG(3, "<< HDMI unknown >>\n" );
            _u1HDMIUnDetectCnt++;
            if (_u1HDMIUnDetectCnt > 5)
            {
                if (_arAudDecoder[AUD_DEC_MAIN].eDecFormat != AUD_FMT_PCM)//AUD_FMT_AC3, edit by xiaoyi for 8223 only support HDMI pcm format 100331
                {
                    // Default decoder type is AC3 to prevent from noises.         
                    *peDecFmt = AUD_FMT_PCM;//AUD_FMT_AC3
                    LOG(3, "<< HDMI unknown >>===> Change to PCM\n" );//AC3
                    _u1HDMIUnDetectCnt = 0;
                    i4Ret = TRUE; 
                }
            }
        }
    }
    else
    {
        LOG(3, "<< HDMI undetect >>\n" );
        eDecStatus = AUD_DECODE_NOT_SUPPORT;        
        _u1HDMIUnDetectCnt ++;
        if (_u1HDMIUnDetectCnt > 5)
        {
            if (_arAudDecoder[AUD_DEC_MAIN].eDecFormat != AUD_FMT_PCM)//AUD_FMT_AC3, edit by xiaoyi for 8223 only support HDMI pcm format 100331
            {
            // Default decoder type is AC3 to prevent from noises.         
            *peDecFmt = AUD_FMT_PCM;//AUD_FMT_AC3          
                LOG(3, "<< HDMI undetect >> --------> Change to PCM\n" );//AC3
                _u1HDMIUnDetectCnt = 0;                
            i4Ret = TRUE; 
            }
        }
    }

    // Notify middleware decoder status       
    if ((pfAudDecNfy != NULL) && (eDecStatus != _aeHDMIDecStatus[AUD_DEC_MAIN]))
    {
        pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_AUD_DECODE_STATUS_CHG, 
                    (UINT32)eDecStatus, u4Tmp);        
    }
    _aeHDMIDecStatus[AUD_DEC_MAIN] = eDecStatus;
    
    return i4Ret;
}
//#endif //#ifndef CC_AUD_DVBT_SUPPORT    

void _AudHdmiOnPlayStateHandler(UINT8 u1DecId)
{
    UINT8      u1HdmiUnstableMute = 0;
    AUD_FMT_T  eDecFmt;
    BOOL    b_ChkHdmiFmt = TRUE;
    static UINT8 u1ChkHdmicount = 0;
    UNUSED(eDecFmt);
    
    // ONLY APPLY on MAIN decoder
    VERIFY((u1DecId == AUD_DEC_MAIN));

//#if !defined(CC_AUD_DVBT_SUPPORT) || defined(CC_AUD_DVBT_AC3_BYPASS)
// take off for driver merge US/EU
#if 0
#if !defined(CC_AUD_DVBT_AC3_BYPASS)
	if (AUD_GetTargetCountry() == COUNTRY_EU)
	{
	    b_ChkHdmiFmt = FALSE;
	}
	else
	{
	    b_ChkHdmiFmt = TRUE;
	}
#endif    
#endif

    //if (b_ChkHdmiFmt)
     if (b_ChkHdmiFmt && ((u1ChkHdmicount%10)==0)) // For hdmi pop noise.
    {
        // Check format change
        if (_IsChangeHdmiSpdifAudFormat(&eDecFmt))
        {
            // Already applied mute if change format
            LOG(3, "\n\n\n\n !!!!! HDMI/SPDIF change format !!!!!\n\n\n");

            // Set mute counter
            _u1HDMIMuteCnt = AIN_CHANGE_TYPE_THRESHOLD + HDMI_AUD_UNSTABLE_LVL_2;
            _AudHdmiMute(TRUE);     

            // Change decoder
            _arAudDecoder[u1DecId].eDecFormat = eDecFmt;   
#if 0
        // Check SPDIF output format
        if (eDecFmt == AUD_FMT_AC3)
        {
            AUD_DspIECConfig(u1DecId, AUD_IEC_CFG_RAW, TRUE);
        }
        else
        {
            AUD_DspIECConfig(u1DecId, AUD_IEC_CFG_PCM, TRUE);
        } 
#endif //if 0

            _ChangeDecFmtOnPlayState(u1DecId);
            _AUD_DspSetIEC(u1DecId); //Set SPDIF Type again because decoder type is changed.
        }
    }
//#endif

    u1HdmiUnstableMute = bHDMIAudStatus();
                        
    // Check HDMI unstable case and apply muting
    if (u1HdmiUnstableMute != HDMI_AUD_OK)
    {
        if (_u1HDMIMuteCnt != 0)
        {
            LOG(3, "################## HDMI unstable -- HDMI Mute! #################\n");
        }
        
        _u1HDMIMuteCnt = (UINT8)(AIN_CHANGE_TYPE_THRESHOLD + u1HdmiUnstableMute);
        _AudHdmiMute(TRUE);
        LOG(7, "HDMI mute count = %d\n", _u1HDMIMuteCnt);
    }

    // Check if HDMI clock exist.
    if (!AUD_IsHdmiClkExist()) 
    {
       if (_u1HDMIMuteCnt != 0)
       {
           LOG(3, "################## HDMI CLK unstable -- HDMI Mute! #################\n");
       }

       _u1HDMIMuteCnt = (UINT8)(AIN_CHANGE_TYPE_THRESHOLD + u1HdmiUnstableMute);
       _AudHdmiMute(TRUE);

      bHDMIClockDetect();
      LOG(3, "####### HDMI NOT Exist, toggle HDMI clock 1 ######\n");  
        
      LOG(7, "HDMI CLK mute count = %d\n", _u1HDMIMuteCnt);
    }
    
    // Polling HDMI sampling rate
	if ((u1ChkHdmicount%10) == 0)
    {
            SAMPLE_FREQ_T eHdmiSmpRate;    

            eHdmiSmpRate = _GetHdmiSmpRate();
            if (_eHdmiSmpRate != eHdmiSmpRate)
            {
                LOG(5, " ------------------ HDMI handler set sampling rate (%d) -----------\n", _eHdmiSmpRate);

                if (_u1HDMIMuteCnt == 0)
                {
                    _u1HDMIMuteCnt = AIN_CHANGE_TYPE_THRESHOLD;
                    _AudHdmiMute(TRUE);
                    LOG(5, "XXXX Sampling Rate Change XXXX \n");
                }
                
                DSP_SetPcmFs(u1DecId, eHdmiSmpRate);           
                AUD_SetSampleFreq(AUD_DEC_MAIN, eHdmiSmpRate);  
                _eHdmiSmpRate = eHdmiSmpRate;
            }
    }

#if 0
    if(!_IsVideoStable()){
        if (_u1HDMIMuteCnt != 0)
        {
            LOG(3, "################## Video not stable -- HDMI Mute! #################\n");
        }        
        _u1HDMIMuteCnt = (UINT8)(HDMI_WAIT_VIDEO_THRESHOLD);
        _AudHdmiMute(TRUE);
        LOG(7, "HDMI mute count = %d\n", _u1HDMIMuteCnt);
    }
#endif
    // Check unmute condition
    if (_u1HDMIMuteCnt > 0)
    {
        _u1HDMIMuteCnt --;
        LOG(7, "HDMI mute count = %d\n", _u1HDMIMuteCnt);
           
        if (_u1HDMIMuteCnt == 0)
        {
            // If format is PCM, then re-playback before signal stable.
            // Since there may be LRCK invert issue during clock unstable period
            if (_arAudDecoder[AUD_DEC_MAIN].eDecFormat == AUD_FMT_PCM)
            {
                LOG(3, " \n << Re-start PCM decoder >> \n");
                #if 1//ndef AUD_HW_LR_INVERSE_DECT
                _ChangeDecFmtOnPlayState(u1DecId);    
                #endif
                u1ChkHdmicount = 0;    
            }
            LOG(3, "@@@@@@@@@@@@@@@@@@@@ HDMI UnMute due to counter is 0! @@@@@@@@@@@@@@@@@@@@ \n" );
			if(_arAudDecoder[AUD_DEC_MAIN].eDecFormat == AUD_FMT_PCM)//add by xiaoyi for 8223 only support HDMI pcm format 100331
			{
                _AudHdmiMute(FALSE);    
			}
        }   
    }
    
    u1ChkHdmicount =  u1ChkHdmicount+1 ;
}

#if 1
// *********************************************************************
// Function : void _AudApllMonitorThread(void)
// Description :
// Parameter : none
// Return    : None
// *********************************************************************
#ifdef BURN_IN_AOUT_TEST //sunman_aout        
#define NONCACHE(addr)                                (((addr) & 0x0fffffff) | 0x30000000)
#endif
static void _AudApllMonitorThread(void* pvArg)
{
    UINT32 u4Event;
    UINT16 u2MsgIdx;
    SIZE_T zMsgSize = sizeof(UINT32);
    
 //sunman_aout
 #ifdef BURN_IN_AOUT_TEST //sunman_aout        
    UINT32 u4DramStartAddr, k;
    UINT32 u4ChlStrAddr[11], u4ChlCurAddr[11];
    UINT8 i, j, u1Tmp;
    UINT8* pu1Buf;
#endif
    
    _gu1AoutDramErr=0;  
    
    UNUSED(pvArg);
    
    while (_fgApllMonThrRun)
    {
        VERIFY(x_msg_q_receive(&u2MsgIdx, &u4Event, &zMsgSize, 
        &_hAudApllCmdQueue, 1, X_MSGQ_OPTION_WAIT) == OSR_OK);

        x_thread_delay(100);
#ifdef BURN_IN_AOUT_TEST //sunman_aout        
        u4DramStartAddr = DSP_VIRTUAL_ADDR((AUD_READ32(REG_RWD_BLK01) & 0xffff) << 8);
        for(i=0;i<11;i++){
            u4ChlStrAddr[i] = u4ChlCurAddr[i] = u4DramStartAddr + (u4ReadDspSram(0x280 + (i)) * 4);
        }    

        while(1)
        {  
             pu1Buf = (UINT8 *)u4ChlCurAddr[0];
#if 0
             for(j=0;j<3;j++){                   
                u1Tmp = IO_READ8(0, pu1Buf+j);                
                if (u1Tmp!=0xFF && u1Tmp!=0)
                    _gu1AoutDramErr += 1;                
            }
            if (u4ChlCurAddr[0] + 3 > u4ChlStrAddr[0+1] )
                    u4ChlCurAddr[0] = u4ChlStrAddr[0];
            else
                    u4ChlCurAddr[0] += 3;
#endif
            x_thread_delay(10);
            for(i=0;i<10;i++){
                pu1Buf = (UINT8 *)NONCACHE(u4ChlCurAddr[i]);
                for(j=0;j<3;j++){           
                    u1Tmp = IO_READ8(0, pu1Buf+j);                    
                    if(i==2){
                        if (u1Tmp!=0xFF && u1Tmp!=0){
                            LOG(2, "\tChannel %d error\n", i );
                            LOG(2, "Read back addr 0x%x = %x\n", (pu1Buf+j),u1Tmp );
                            u1Tmp = IO_READ8(0, pu1Buf+j);                    
                            LOG(2, "Read back addr 0x%x = %x\n", (pu1Buf+j),u1Tmp );
                            u1Tmp = IO_READ8(0, pu1Buf+j);                    
                            LOG(2, "Read back addr 0x%x = %x\n", (pu1Buf+j),u1Tmp );
                            u1Tmp = IO_READ8(0, pu1Buf+j);                    
                            LOG(2, "Read back addr 0x%x = %x\n", (pu1Buf+j),u1Tmp );
                            u1Tmp = IO_READ8(0, pu1Buf+j);                                                
                            LOG(2, "Read back addr 0x%x = %x\n", (pu1Buf+j),u1Tmp );
                            _gu1AoutDramErr += 1;        
                        }
                    }
                }                                
                if (u4ChlCurAddr[i] + 3 > u4ChlStrAddr[i+1] )
                    u4ChlCurAddr[i] = u4ChlStrAddr[i];            
                else
                    u4ChlCurAddr[i] += 3;
                x_thread_delay(2);    
            }
        }
#endif
    } 
}

#endif

//2008/01/15 added by ling
static void AudSetDetTvSrcMode(UINT8 u1DecId)
{
    if ((_au4TvSysMask[u1DecId] & TV_SYS_MASK_L) == TV_SYS_MASK_L)
    {
        AUD_DspSetDetectDefaultMode(4); // SECAM-L
    }
    else if ((_au4TvSysMask[u1DecId] & TV_SYS_MASK_L_PRIME) == TV_SYS_MASK_L_PRIME)
    {
        AUD_DspSetDetectDefaultMode(4); // SECAM-L or SECAM-L'
    }
    else
    {
        AUD_DspSetDetectDefaultMode(0); // PAL-DK
    }
}

 void _AudSetTvSysMask(UINT8 u1DecId, UINT32 u4Mask)
{
     _au4TvSysMask[u1DecId]=u4Mask;
}

  UINT32 _AudGetTvSysMask(UINT8 u1DecId)
 {
	return  _au4TvSysMask[u1DecId];
 }




 
 AUD_FMT_T _AudAtvFmtDetection(void)
{
    TV_AUD_SYS_T eTvSys;
    AUD_FMT_T    eAudFmt;

    // Waiting for analog tuner lock signal ..
    _WaitAtunerLockSignal();

    UNUSED(AUD_SetDecType(AUD_DEC_AUX, AUD_STREAM_FROM_ANALOG_TUNER, AUD_FMT_DETECTOR));
    UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, TRUE));

    //2008/01/15 added by ling
    AudSetDetTvSrcMode(AUD_DEC_MAIN);

    // Try lock to make sure the semaphore was locked
    UNUSED(x_sema_lock(_hDetectionSyncSema, X_SEMA_OPTION_NOWAIT));   
    UNUSED(AUD_DSPCmdPlay(AUD_DEC_AUX));
    AUD_DetectionSyncSema(TRUE);         
    UNUSED(AUD_DSPCmdStop(AUD_DEC_AUX));
    UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, FALSE));
    eTvSys = DSP_GetAtvTvSystem();
    DSP_SetTvSrcMode(eTvSys);
    
    switch (eTvSys)
    {
        case SV_A2_BG:
        case SV_A2_DK1:
        case SV_A2_DK2:
        case SV_A2_DK3:    
            eAudFmt = AUD_FMT_A2;
            break;
        case SV_PAL_I:
        case SV_PAL_BG:
        case SV_PAL_DK:
            eAudFmt = AUD_FMT_PAL;
            break;
        case SV_MTS:
            eAudFmt = AUD_FMT_PAL; // FIXME
            break;
        default:
            eAudFmt = AUD_FMT_PAL;
            LOG(1, "Invalid _AudAtvFmtDetection\n");
    }
    
    return eAudFmt;
}


#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0521  
static void _AudDualDecStatePlay(void)
{
    switch (_eDualDecStreamSrc)
    {
    case AUD_STREAM_FROM_ANALOG_TUNER:
        _fgDualDecModeDrv = TRUE; 
        DSP_SetDualDecMode(TRUE);
        AUD_ClkCfgDualDecMode(TRUE);
        // First make sure main decoder was stopped
        if (_arAudDrvState[AUD_DEC_MAIN] != AUD_IDLE)
        {
            UNUSED(AUD_DSPCmdStop(AUD_DEC_MAIN));
        }    
        
        switch (_eDualDecStreamSrcOrg)
        {
        case AUD_STREAM_FROM_OTHERS:
        case AUD_STREAM_FROM_DIGITAL_TUNER:               
        case AUD_STREAM_FROM_MULTI_MEDIA:
        case AUD_STREAM_FROM_ANALOG_TUNER:         
            if (!AUD_IsDecoderPlay(AUD_DEC_AUX))
            {
#ifdef CC_ACI_AUTO_DETECT
                // Do detection when change channel !
                AUD_FMT_T eAudFmt;
                eAudFmt = _AudAtvFmtDetection();
                UNUSED(AUD_SetDecType(AUD_DEC_AUX, AUD_STREAM_FROM_ANALOG_TUNER, eAudFmt));
#else
                UNUSED(AUD_SetDecType(AUD_DEC_AUX, AUD_STREAM_FROM_ANALOG_TUNER, _eDualDecStreamFmt));
#endif			
                UNUSED(AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM));
                UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, TRUE));
                UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, TRUE));

                UNUSED(AUD_DSPCmdPlay(AUD_DEC_AUX));
                AUD_DualDecSetSyncSema(TRUE);
                UNUSED(AUD_DSPCmdPlay(AUD_DEC_MAIN));            
            
                UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, FALSE));
                UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, FALSE));
                // Notify upper layer
                UNUSED(_AudDualDecNotify(AUD_DUAL_DEC_CMD_PLAY, AUD_OK));
                DSP_DualDecAtvNotifyChlInfor();
                // Send message to release semaphore
                AUD_DualDecCommandDone(AUD_DUAL_DEC_CMD_PLAY, AUD_OK);
            }
            else
            {
                UNUSED(AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM));
                UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, TRUE));
                UNUSED(AUD_DSPCmdPlay(AUD_DEC_MAIN)); 
                UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, FALSE));
                // Notify upper layer
                UNUSED( _AudDualDecNotify(AUD_DUAL_DEC_CMD_PLAY, AUD_OK));
                DSP_DualDecAtvNotifyChlInfor();
                // Send message to release semaphore
                AUD_DualDecCommandDone(AUD_DUAL_DEC_CMD_PLAY, AUD_OK);
            }
            break;
        case AUD_STREAM_FROM_SPDIF:
        case AUD_STREAM_FROM_LINE_IN:
        case AUD_STREAM_FROM_HDMI:
        case AUD_STREAM_FROM_MEMORY:
        case AUD_STREAM_FROM_BUF_AGT:    
            if (!AUD_IsDecoderPlay(AUD_DEC_AUX))
            {
#ifdef CC_ACI_AUTO_DETECT
                // Do detection when change channel !
                AUD_FMT_T eAudFmt;
                eAudFmt = _AudAtvFmtDetection();
                UNUSED(AUD_SetDecType(AUD_DEC_AUX, AUD_STREAM_FROM_ANALOG_TUNER, eAudFmt));
#else
                UNUSED(AUD_SetDecType(AUD_DEC_AUX, AUD_STREAM_FROM_ANALOG_TUNER, _eDualDecStreamFmt));
#endif
                UNUSED(AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM));
                UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, TRUE));
                UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, TRUE));

                UNUSED(AUD_DSPCmdPlay(AUD_DEC_AUX));
                AUD_DualDecSetSyncSema(TRUE);
                UNUSED(AUD_DSPCmdPlay(AUD_DEC_MAIN));            
            
                UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, FALSE));
                UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, FALSE));
                // Notify upper layer
                UNUSED(_AudDualDecNotify(AUD_DUAL_DEC_CMD_PLAY, AUD_OK));
                DSP_DualDecAtvNotifyChlInfor();
                // Send message to release semaphore
                AUD_DualDecCommandDone(AUD_DUAL_DEC_CMD_PLAY, AUD_OK);
            }
            else
            {
            UNUSED(AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM));
            UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, TRUE));
            UNUSED(AUD_DSPCmdPlay(AUD_DEC_MAIN));
            UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, FALSE));
            // Notify upper layer
            UNUSED(_AudDualDecNotify(AUD_DUAL_DEC_CMD_PLAY, AUD_OK));
            DSP_DualDecAtvNotifyChlInfor();
            // Send message to release semaphore
            AUD_DualDecCommandDone(AUD_DUAL_DEC_CMD_PLAY, AUD_OK);
            }
            break;
        default:
            LOG(1, "Invalid _AudDualDecStatePlay switch\n");
        }    
        break;
        
    case AUD_STREAM_FROM_DIGITAL_TUNER:
        _fgDualDecModeDrv = FALSE;
        DSP_SetDualDecMode(FALSE);
        AUD_ClkCfgDualDecMode(FALSE);
        // FIXME workaround ..
        AUD_AinEnable(AUD_DEC_MAIN, AUD_STREAM_FROM_LINE_IN, FALSE);
        switch (_eDualDecStreamSrcOrg)
        {
        case AUD_STREAM_FROM_OTHERS:
        case AUD_STREAM_FROM_DIGITAL_TUNER:            
	  case AUD_STREAM_FROM_MULTI_MEDIA:		
        case AUD_STREAM_FROM_ANALOG_TUNER:    
        case AUD_STREAM_FROM_SPDIF:
        case AUD_STREAM_FROM_LINE_IN:
        case AUD_STREAM_FROM_HDMI:
        case AUD_STREAM_FROM_MEMORY:
        case AUD_STREAM_FROM_BUF_AGT:    
            UNUSED(AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_DIGITAL_TUNER, _eDualDecStreamFmt));
            UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, TRUE));
            UNUSED(AUD_DSPCmdPlay(AUD_DEC_MAIN));
            UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, FALSE));
            // Notify upper layer
            UNUSED(_AudDualDecNotify(AUD_DUAL_DEC_CMD_PLAY, AUD_OK));
            // Send message to release semaphore
            AUD_DualDecCommandDone(AUD_DUAL_DEC_CMD_PLAY, AUD_OK);
            break;            
        default:
            LOG(1, "Invalid _AudDualDecStatePlay switch\n");    
        }    
        break;

    case AUD_STREAM_FROM_MULTI_MEDIA:
        _fgDualDecModeDrv = FALSE;
        DSP_SetDualDecMode(FALSE);
        AUD_ClkCfgDualDecMode(FALSE);
        // FIXME workaround ..
        AUD_AinEnable(AUD_DEC_MAIN, AUD_STREAM_FROM_LINE_IN, FALSE);
        switch (_eDualDecStreamSrcOrg)
        {
        case AUD_STREAM_FROM_OTHERS:
        case AUD_STREAM_FROM_DIGITAL_TUNER:   
	  case AUD_STREAM_FROM_MULTI_MEDIA:		
        case AUD_STREAM_FROM_ANALOG_TUNER:    
        case AUD_STREAM_FROM_SPDIF:
        case AUD_STREAM_FROM_LINE_IN:
        case AUD_STREAM_FROM_HDMI:
        case AUD_STREAM_FROM_MEMORY:
        case AUD_STREAM_FROM_BUF_AGT:   
	    {
	      UNUSED(AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_MULTI_MEDIA, _eDualDecStreamFmt));
            UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, TRUE));
            UNUSED(AUD_DSPCmdPlay(AUD_DEC_MAIN));
            UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, FALSE));
            // Notify upper layer
            UNUSED(_AudDualDecNotify(AUD_DUAL_DEC_CMD_PLAY, AUD_OK));
            // Send message to release semaphore
            AUD_DualDecCommandDone(AUD_DUAL_DEC_CMD_PLAY, AUD_OK);

	    }
            break;            
        default:
            LOG(1, "Invalid _AudDualDecStatePlay switch\n");    
        }    
        break;

    case AUD_STREAM_FROM_SPDIF:
    case AUD_STREAM_FROM_OTHERS:    
    case AUD_STREAM_FROM_LINE_IN:
    case AUD_STREAM_FROM_HDMI:
    case AUD_STREAM_FROM_MEMORY:
    case AUD_STREAM_FROM_BUF_AGT:        
        _fgDualDecModeDrv = FALSE;    
        DSP_SetDualDecMode(FALSE);
        AUD_ClkCfgDualDecMode(FALSE);
        switch (_eDualDecStreamSrcOrg)
        {
        case AUD_STREAM_FROM_OTHERS:
        case AUD_STREAM_FROM_DIGITAL_TUNER:            
	  case AUD_STREAM_FROM_MULTI_MEDIA:
        case AUD_STREAM_FROM_ANALOG_TUNER:    
        case AUD_STREAM_FROM_SPDIF:
        case AUD_STREAM_FROM_LINE_IN:
        case AUD_STREAM_FROM_HDMI:
        case AUD_STREAM_FROM_MEMORY:
        case AUD_STREAM_FROM_BUF_AGT:                    
            // Make sure AUX decoder was on play ..                        
            if (_arAudDrvState[AUD_DEC_AUX] != AUD_ON_PLAY)
            {
                AUD_FMT_T eAudFmt;
                eAudFmt = _AudAtvFmtDetection();
                UNUSED(AUD_SetDecType(AUD_DEC_AUX, AUD_STREAM_FROM_ANALOG_TUNER, eAudFmt));
                UNUSED(AUD_DSPCmdPlay(AUD_DEC_AUX));
            }
            
            // Aux decoder has ATV still on play, then play main decoder
            UNUSED(AUD_SetDecType(AUD_DEC_MAIN, _eDualDecStreamSrc, _eDualDecStreamFmt));
            UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, TRUE));
            UNUSED(AUD_DSPCmdPlay(AUD_DEC_MAIN));
            UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, FALSE));
            // Notify upper layer
            UNUSED(_AudDualDecNotify(AUD_DUAL_DEC_CMD_PLAY, AUD_OK));
            // Send message to release semaphore
            AUD_DualDecCommandDone(AUD_DUAL_DEC_CMD_PLAY, AUD_OK);
            break;
        default:
            LOG(1, "Invalid _AudDualDecStatePlay switch\n");    
        }    
        break;
    default:
        LOG(1, "Invalid _AudDualDecStatePlay switch\n");    
    }
}

static void _AudDualDecStateStop(void)
{
    AudDecNfyFct pfnAudDecNfyFunc = NULL;
  
    VERIFY(_AUD_GetNotifyFunc(&pfnAudDecNfyFunc));

#if 0    
    switch (_eDualDecStreamSrc)
    {
    case AUD_STREAM_FROM_ANALOG_TUNER:
    case AUD_STREAM_FROM_DIGITAL_TUNER:  
        _AUD_IgnoreDecNotify(AUD_DEC_MAIN, TRUE);
        _AUD_IgnoreDecNotify(AUD_DEC_AUX, TRUE);
        AUD_DSPCmdStop(AUD_DEC_MAIN);        
        AUD_DSPCmdStop(AUD_DEC_AUX);
        _AUD_IgnoreDecNotify(AUD_DEC_MAIN, FALSE);
        _AUD_IgnoreDecNotify(AUD_DEC_AUX, FALSE);
        // Notify upper layer
        _AudDualDecNotify(AUD_DUAL_DEC_CMD_STOP, AUD_OK);
        // Send message to release semaphore
        AUD_DualDecCommandDone(AUD_DUAL_DEC_CMD_STOP, AUD_OK);
        break;
    case AUD_STREAM_FROM_OTHERS:    
    case AUD_STREAM_FROM_SPDIF:
    case AUD_STREAM_FROM_LINE_IN:
    case AUD_STREAM_FROM_HDMI:
    case AUD_STREAM_FROM_MEMORY:
    case AUD_STREAM_FROM_BUF_AGT:
        _AUD_IgnoreDecNotify(AUD_DEC_MAIN, TRUE);
        AUD_DSPCmdStop(AUD_DEC_MAIN);
        _AUD_IgnoreDecNotify(AUD_DEC_MAIN, FALSE);
        // Notify upper layer
        _AudDualDecNotify(AUD_DUAL_DEC_CMD_STOP, AUD_OK);
        // Send message to release semaphore
        AUD_DualDecCommandDone(AUD_DUAL_DEC_CMD_STOP, AUD_OK);
        break;    
    default:
        LOG(1, "Invalid _AudDualDecStateStop switch\n");    
    }
 #else
    switch (_eDualDecStreamSrc)
    {
/*    
    case AUD_STREAM_FROM_ANALOG_TUNER:
        _AUD_IgnoreDecNotify(AUD_DEC_MAIN, TRUE);
        _AUD_IgnoreDecNotify(AUD_DEC_AUX, TRUE);
        AUD_DSPCmdStop(AUD_DEC_MAIN);        
        AUD_DSPCmdStop(AUD_DEC_AUX);
        _AUD_IgnoreDecNotify(AUD_DEC_MAIN, FALSE);
        _AUD_IgnoreDecNotify(AUD_DEC_AUX, FALSE);
        // Notify upper layer
        _AudDualDecNotify(AUD_DUAL_DEC_CMD_STOP, AUD_OK);
        // Send message to release semaphore
        AUD_DualDecCommandDone(AUD_DUAL_DEC_CMD_STOP, AUD_OK);
        break;
*/        
    case AUD_STREAM_FROM_ANALOG_TUNER:  
        //lint -fallthrough
    case AUD_STREAM_FROM_DIGITAL_TUNER:      
    case AUD_STREAM_FROM_MULTI_MEDIA:		
    case AUD_STREAM_FROM_OTHERS:    
    case AUD_STREAM_FROM_SPDIF:
    case AUD_STREAM_FROM_LINE_IN:
    case AUD_STREAM_FROM_HDMI:
    case AUD_STREAM_FROM_MEMORY:
    case AUD_STREAM_FROM_BUF_AGT:
        UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, TRUE));
        UNUSED(AUD_DSPCmdStop(AUD_DEC_MAIN));
        UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, FALSE));
        // Notify upper layer
        UNUSED(_AudDualDecNotify(AUD_DUAL_DEC_CMD_STOP, AUD_OK));
        // Send message to release semaphore
        AUD_DualDecCommandDone(AUD_DUAL_DEC_CMD_STOP, AUD_OK);
        break;    
    default:
        LOG(1, "Invalid _AudDualDecStateStop switch\n"); 
    }
 #endif
}        

// *********************************************************************
// Function : void _AudDrvThread(void)
// Description :
// Parameter : none
// Return    : None
// *********************************************************************
static void _AudDualDecDrvThread(void* pvArg)
{
    AUD_DUAL_DEC_CMD_T eEvent;
    UNUSED(pvArg);

    _ChangeDualDecDrvState(AUD_DUAL_DEC_INIT_AUX_DECODER);
    
    while (1)
    {
        switch (_rAudDualDrvState)
        {
        case AUD_DUAL_DEC_INIT_AUX_DECODER:
            // Busy waiting until DSP is ok ..
            while(_arAudDrvState[AUD_DEC_MAIN] != AUD_IDLE)
            {
                x_thread_delay(10);
            }
            // In this state, need to detect what kind of ATV format to play
            // FIXME !! TODO !!
            UNUSED(AUD_DualDecSetDecType(AUD_STREAM_FROM_ANALOG_TUNER, AUD_FMT_PAL));
            UNUSED(AUD_DualDecSetDecType(AUD_STREAM_FROM_DIGITAL_TUNER, AUD_FMT_MPEG));
            _ChangeDualDecDrvState(AUD_DUAL_DEC_IDLE);
            break;
        case AUD_DUAL_DEC_PLAY:
            _AudDualDecStatePlay();
            // Succesfully playback, then wait stop command 
            _AudDualDecSetEventFlag(AUD_DUAL_DEC_CMD_FLAG_STOP);
            UNUSED(_AudDualDecWaitEvent(&eEvent, TRUE));
            _ChangeDualDecDrvState(AUD_DUAL_DEC_STOP);
            break;    
        case AUD_DUAL_DEC_STOP:
            _AudDualDecStateStop();
            _ChangeDualDecDrvState(AUD_DUAL_DEC_IDLE);
            break;
        case AUD_DUAL_DEC_IDLE:
            _AudDualDecSetEventFlag(AUD_DUAL_DEC_CMD_FLAG_PLAY);
            UNUSED(_AudDualDecWaitEvent(&eEvent, TRUE));
            if (eEvent == AUD_DUAL_DEC_CMD_PLAY)
            {
                _ChangeDualDecDrvState(AUD_DUAL_DEC_PLAY);
            }
            break;
        default:
            LOG(1, "Invalid _AudDualDecDrvThread switch\n");        
        }
    }
}
#endif

// *********************************************************************
// Function : void _AudDrvThread(void)
// Description :
// Parameter : none
// Return    : None
// *********************************************************************
static void _AudDrvThread(void* pvArg)
{
    UINT8 u1DecId = AUD_DEC_MAIN;
    AUD_DRV_STATE_T eNewState = AUD_WAIT_STOP;
    UINT32 u4Event = 0;
    BOOL fgClipDone = FALSE;
    BOOL fgWaitPlayIssuePause = FALSE;
    BOOL fgWaitPlayOnPause = FALSE;
    if (pvArg != NULL)
    {
        u1DecId = *(UINT8 *)pvArg;
    }
    AUD_DEC_ID_VALIDATE(u1DecId);
    if (u1DecId == AUD_DEC_MAIN)
    {
        _ChangeAudioState(u1DecId, AUD_TRIGGER_ADSP);
    }    
    else if (u1DecId == AUD_DEC_AUX)
    {   
        _ChangeAudioState(u1DecId, AUD_WAIT_POWER_ON);
    }
#ifdef CC_MT5391_AUD_3_DECODER
    else if (u1DecId == AUD_DEC_THIRD)
    {
        _ChangeAudioState(u1DecId, AUD_WAIT_POWER_ON);
    }
#endif    
    
    while (1)
    {
        switch(_arAudDrvState[u1DecId])
        {
            case AUD_TRIGGER_ADSP:
                _arAudDecoder[u1DecId].eDecState = AUD_DEC_INIT;
                vADSPPowerON();
                _ChangeAudioState(u1DecId, AUD_WAIT_POWER_ON);
                _arAudDecoder[u1DecId].eDecState = AUD_DEC_INIT;
                break;

            case AUD_WAIT_POWER_ON:
                _arAudDecoder[u1DecId].eDecState = AUD_DEC_INIT;
            
                if (!fgADSPIsIdle())
                {
                    vSetSPDIFInCtrl(u1DecId,0); // FIXME !! Should move to AUD_HwInit()
                    _ChangeAudioState(u1DecId, AUD_IDLE);
                }
                else
                {
                    x_thread_delay(1);
                }    
                break;

            case AUD_DECODE_INIT:
                _arAudDecoder[u1DecId].eDecState = AUD_DEC_INIT;
                VERIFY(x_sema_lock(_hSemaLoadCode, X_SEMA_OPTION_WAIT) == OSR_OK);
                _LoadAdspCode(u1DecId, _arAudDecoder[u1DecId].eDecFormat);
                AUD_CommandDone(u1DecId, AUD_CMD_LOADCODE, AUD_OK);
                VERIFY(x_sema_unlock(_hSemaLoadCode) == OSR_OK);
                
                _ChangeAudioState(u1DecId, AUD_IDLE);

                break;

            case AUD_ON_PLAY:
                
                if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MEMORY)
                {
                    _AudSetEvent(u1DecId, AUD_CMD_FLAG_STOP  | 
                                          AUD_CMD_FLAG_STOPSTREAM |
                                          AUD_CMD_FLAG_RESET |
                                          AUD_CMD_FLAG_PAUSE);
                    VERIFY(_AudWaitEvent(u1DecId, &u4Event, TRUE, AUD_EVENT_TIMEOUT));
                    
                    switch (u4Event)
                    {   
                    case AUD_CMD_RESET:
                        _AudDrvResetDSP(u1DecId); //  -- DSP Force --
                        eNewState = AUD_ON_PLAY;
                        break;

                    case AUD_CMD_STOP:
                        eNewState = AUD_WAIT_STOP;
                        break;

                    case AUD_CMD_STOPSTREAM:
                        eNewState = AUD_WAIT_STOPSTREAM;
                        break;
                        
                    case AUD_CMD_PAUSE:
                        eNewState = AUD_WAIT_PAUSE;
                        break;
                        
                    default:
                        ASSERT(0);
                    }
                    _ChangeAudioState(u1DecId, eNewState);
                    break;
                    
                }
                else if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_BUF_AGT) ||
                         (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_FEEDER))
                {
                    _AudSetEvent(u1DecId, AUD_CMD_FLAG_STOP  | 
                                          AUD_CMD_FLAG_RESET |
                                          AUD_CMD_FLAG_PAUSE);
                    VERIFY(_AudWaitEvent(u1DecId, &u4Event, TRUE, AUD_EVENT_TIMEOUT));
                    
                    switch (u4Event)
                    {   
                    case AUD_CMD_RESET: 
                        _AudDrvResetDSP(u1DecId); //  -- DSP Force --
                        eNewState = AUD_ON_PLAY;
                        break;                        
                    case AUD_CMD_STOP:
                        eNewState = AUD_WAIT_STOP;
                        break;
                        
                    case AUD_CMD_PAUSE:
                        eNewState = AUD_WAIT_PAUSE;
                        break;
                        
                    default:
                        ASSERT(0);
                    }
                    _ChangeAudioState(u1DecId, eNewState);
                    break;
                    
                }
                else if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)||
				  (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))	
                {
                    _AudSetEvent(u1DecId, AUD_CMD_FLAG_STOP   | 
                                          AUD_CMD_FLAG_RESET |
                                          AUD_CMD_FLAG_PAUSE |
                                          AUD_CMD_FLAG_CHANGE_FORMAT);
                    
                    _afgDtvOnPlayThreadGo[u1DecId] = TRUE;

                    VERIFY(_AudWaitEvent(u1DecId, &u4Event, TRUE, AUD_EVENT_TIMEOUT));
                        
                    switch (u4Event)
                    {   
                    case AUD_CMD_RESET:
                        _AudDrvResetDSP(u1DecId);  //  -- DSP Force --
                        eNewState = AUD_ON_PLAY;                        
                        break;
                    case AUD_CMD_STOP:
                        //LOG(1, " ****** Got stop message in ON_PLAY state\n");
                        eNewState = AUD_WAIT_STOP;
                        _DtvResetLockStatus(u1DecId);
                        AUD_MMQueueSyncInfo(u1DecId,0,0,TRUE); //Reset MM Sync info
                        break;
                    case AUD_CMD_CHANGE_FORMAT:
                        eNewState = AUD_CHANGE_FORMAT;
                        break;
                    case AUD_CMD_PAUSE:
                        eNewState = AUD_WAIT_PAUSE;
                        break;			
                    default:
                        ASSERT(0);
                    }
                    //LOG(1, "  <<<<<<<<<<<<<<<<< set _fgDtvOnPlayThreadGo = FALSE <<<<<<<\n");
                    _afgDtvOnPlayThreadGo[u1DecId] = FALSE;
                    _ChangeAudioState(u1DecId, eNewState);
                    break;                 
                }
                else if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_HDMI) ||
                         (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_SPDIF))
                {                
                    _AudSetEvent(u1DecId, AUD_CMD_FLAG_STOP  | 
                                          AUD_CMD_FLAG_RESET |
                                          AUD_CMD_FLAG_CHANGE_FORMAT);
                    
                    if (!_AudWaitEvent(u1DecId, &u4Event, FALSE, AUD_HDMI_DETECT_PERIOD)) // No event got, then continue
                    {    
                        _AudHdmiOnPlayStateHandler(u1DecId);
                    }
                    else
                    {
                        switch (u4Event)
                        {   
                        case AUD_CMD_STOP:
                            // Move to WAIT_STOP state. When dsp is stopped, then unmute.
                            //_AudHdmiMute(FALSE);
                            //_u1HDMIMuteCnt = 0; // Reset mute counter                           
                            eNewState = AUD_WAIT_STOP;
                            break; 
                        case AUD_CMD_RESET:
                            _AudDrvResetDSP(u1DecId);  //  -- DSP Force --
                            eNewState = AUD_ON_PLAY;
                            break;
                        case AUD_CMD_CHANGE_FORMAT:
                            eNewState = AUD_CHANGE_FORMAT;
                            break;        
                        default:
                            ASSERT(0);
                        }    
                        _ChangeAudioState(u1DecId, eNewState);
                    }
                }
                else if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) && (u1DecId == AUD_DEC_AUX))
                {
                     
                    _AudSetEvent(u1DecId, AUD_CMD_FLAG_STOP  | 
                                          AUD_CMD_FLAG_RESET |
                                          AUD_CMD_FLAG_PAUSE |
                                          AUD_CMD_FLAG_CHANGE_FORMAT);                      
                    if (!_AudWaitEvent(u1DecId, &u4Event, FALSE, AUD_EVENT_TIMEOUT))
                    {
                        if (_guAudDspTrigAtvFmtchg)
                        {
                            //trigger ATV change format
                            eNewState = AUD_CHANGE_FORMAT;
                            _ChangeAudioState(u1DecId, eNewState);
                        }
                    }
                    else
                    {

                        switch (u4Event)
                        {   
                        case AUD_CMD_RESET:
                            _AudDrvResetDSP(u1DecId);  //  -- DSP Force --
                            eNewState = AUD_ON_PLAY;                            
                            break;
                        case AUD_CMD_STOP:
                            eNewState = AUD_WAIT_STOP;
                            break;
                    
                        case AUD_CMD_PAUSE:
                            eNewState = AUD_WAIT_PAUSE;
                            break;
                        case AUD_CMD_CHANGE_FORMAT:
                            eNewState = AUD_CHANGE_FORMAT;
                            break;        
                        default:
                            ASSERT(0);
                        }    
                        _ChangeAudioState(u1DecId, eNewState);
                    }
                }
                else
                {                
                    _AudSetEvent(u1DecId, AUD_CMD_FLAG_STOP  | 
                                          AUD_CMD_FLAG_RESET |
                                          AUD_CMD_FLAG_PAUSE |
                                          AUD_CMD_FLAG_CHANGE_FORMAT);
                    VERIFY(_AudWaitEvent(u1DecId, &u4Event, TRUE, AUD_EVENT_TIMEOUT));

                    switch (u4Event)
                    {   
                    case AUD_CMD_RESET:
                        _AudDrvResetDSP(u1DecId);  //  -- DSP Force --
                        eNewState = AUD_ON_PLAY;                        
                        break;                        
                    case AUD_CMD_STOP:
                        eNewState = AUD_WAIT_STOP;
                        break;
                    
                    case AUD_CMD_PAUSE:
                        eNewState = AUD_WAIT_PAUSE;
                        break;
                    case AUD_CMD_CHANGE_FORMAT:
                        eNewState = AUD_CHANGE_FORMAT;
                        break;        
                    default:
                        ASSERT(0);
                    }
                    _ChangeAudioState(u1DecId, eNewState);
                    break;
                }
                break;
                    
            case AUD_WAIT_PLAY:
                if (!fgWaitPlayIssuePause)
                {    
                    _arAudDecoder[u1DecId].eDecState = AUD_DEC_PLAYING;
                    _arAudDecoder[u1DecId].u4ReceivePesCount = 0;
                    _arAudDecoder[u1DecId].u4ReceiveValidPesCount = 0;
                    _afgFirstIPicArrived[u1DecId] = FALSE;
                    _aeHDMIDecStatus[u1DecId] = AUD_DECODE_UNKNOWN;                
                    _fgAoutTimeoutEnable = FALSE;
                    _fgIsFirstTime[u1DecId] = TRUE;	
#ifdef CC_ENABLE_AV_SYNC
                    _GetVideoCBCount=0;
                    LOG(5, "[SRM_AVSYNC] WAIT_PLAY !! Clear _GetVideoCBCount to 0!!!\n");
#endif                                        
                    //_afgIssuePlayComToDsp[u1DecId] = FALSE;  //Laihui:for initial twice keep update write point, move to idle state
                    
                    //clear previous 1st PTS info
                    DSP_SetStartPtsToShm(u1DecId, 0, DSP_INTERNAL_ADDR(u4GetAFIFOStart(u1DecId)));                        	
                                
                    if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MEMORY)
                    {
                        _fgForceStopStrmDma = TRUE;
                    }

                    if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
                    {
                        _fgMMSet[u1DecId]=FALSE;	
                        _AudClearEOSFlag(u1DecId);
                    }

                    if (u1DecId == AUD_DEC_MAIN)
                    {
#ifndef CC_ENABLE_AV_SYNC
                        AUD_DspInputChannelDelay(_arAudDecoder[u1DecId].eStreamFrom);
#else
                        //If SRM Callback comes before, this may clear all value which is set by SRM
                        //AUD_DspChannelDelay_initial(0, AUD_CH_ALL, AUD_DEC_MAIN);
#endif
                        AUD_DspInputChangeAVCPara(_arAudDecoder[u1DecId].eStreamFrom);
#ifdef CC_ENABLE_AV_SYNC
                        // if (!_fgVdpReady)
                        if ((_arAudDecoder[u1DecId].eStreamFrom != AUD_STREAM_FROM_MULTI_MEDIA) &&
                            (_arAudDecoder[u1DecId].eStreamFrom != AUD_STREAM_FROM_FEEDER) &&
                            (_arAudDecoder[u1DecId].eStreamFrom != AUD_STREAM_FROM_BUF_AGT) &&
                            (_arAudDecoder[u1DecId].eStreamFrom != AUD_STREAM_FROM_MEMORY))
                        {
                            AUD_DspDecPlayMuteEnable(AUD_DEC_MAIN, TRUE);
                            LOG(3,"[SRM_AVSYNC] ########### For AV sync , MUTE Audio !!!! ######\n");
                        }
                        else 
                        {
                            _fgVdpReady = TRUE;
                        }
#endif
                    }
                    else if (u1DecId == AUD_DEC_AUX)
                    {
                        _AUD_DspStopMuteEnable(AUD_DEC_AUX, FALSE);
                    }
#if 0 //Laihui:  For mt5391 already fix the AD phase error, don't need to swap LR anymore
                    if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_LINE_IN)
                    {
                        if (BSP_GetIcVersion() < IC_VER_5382P_AA)
                        {
                            DSP_SetPcmLrckMode(u1DecId,TRUE);
                        }
                        DSP_SetLineInLrInverse(u1DecId, (!DRVCUST_OptGet(eAudioLineInLrSwap)));
                    }
#endif

                    // Turn on stream input
                    #ifdef CC_MT5391   
                    if ((AUD_GetTargetCountry() == COUNTRY_US) &&
			                   (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_LINE_IN))
                    {
                        x_thread_delay(300);                
                    }
	                #else
                    if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_LINE_IN)
                    {
                        x_thread_delay(300);                
                    }	
	                #endif   
	                  
    	  
                    AUD_StrInputEnable(u1DecId, _arAudDecoder[u1DecId].eStreamFrom, _arAudDecoder[u1DecId].eDecFormat, TRUE, _arAudDecoder[u1DecId].u4ReceivePesCount);
                    //koro: increase dmx interrupt for AAC
                    if ( _arAudDecoder[AUD_DEC_AUX].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER &&
                        _arAudDecoder[AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER &&
                        (_arAudDecoder[AUD_DEC_AUX].eDecFormat == AUD_FMT_AAC || _arAudDecoder[AUD_DEC_THIRD].eDecFormat == AUD_FMT_AAC)  
                        )
                    {
                        _DMX_SetInterruptThreshold(DMX_PID_TYPE_ES_AUDIO,1+188*1);    // 2*188
                    }
                    else
                    {
                        _DMX_SetInterruptThreshold(DMX_PID_TYPE_ES_AUDIO,1+188*2); // 3*188
                    }
                }// (!fgWaitPlayIssuePause) 
                
                // Check if need AV synchronization control
                if (_AvSyncControl(u1DecId))
                {
                    LOG(1,"######### NEED SYNC ##########\n");
                    if (!fgWaitPlayIssuePause)
                    {
                    // Release Play command API waiting semaphore 
                    AUD_CommandDone(u1DecId, AUD_CMD_PLAY, AUD_OK);
                    }
                    // Wait Av-Sync or User Stop command 
                    _AudSetEvent(u1DecId, AUD_CMD_FLAG_STOP   | 
                                          AUD_CMD_FLAG_RESET  |
                                          AUD_CMD_FLAG_AVSYNC |
                                          AUD_CMD_FLAG_CHANGE_FORMAT |
                                          AUD_CMD_FLAG_PAUSE |
                                          AUD_CMD_FLAG_RESUME);
                    
                    //if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)||
                    //    (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))		
                    if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
                    {
#ifdef  CC_AUD_DDBANNER
                        _AUD_DDBannerEnable(u1DecId, TRUE); //Andrew : For DD test only
#endif
                        // DTV must have Avsync command
                        // Light: Since DTV needs to wait for 1st I frame, it takes longer time to wait for tne event.
                        if(!_AudWaitEvent(u1DecId, &u4Event, FALSE, AUD_EVENT_TIMEOUT_DTV))
                        {             
                            vDspSetFifoReadPtr(u1DecId, _arAudDecoder[u1DecId].rCurrentPes.u4Wp);
                            DSP_SetStartPtsToShm(u1DecId, (UINT32)(_arAudDecoder[u1DecId].rCurrentPes.u4Pts & 0xffffffff), _arAudDecoder[u1DecId].rCurrentPes.u4Wp);                        	
                            u4Event = (UINT32)AUD_CMD_AVSYNC;
                            LOG(7, "NO AV sync message retrieve in DTV\n");
                        }
                    }
                    else if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
                    {
#ifdef  CC_AUD_DDBANNER
                        _AUD_DDBannerEnable(u1DecId, TRUE); //Andrew : For DD test only
#endif
                        // MM must have Avsync command
                        // Laihui: MM also need to wait data until timeout, longer then DTV. Cause file maybe a lot of error data.
                        if(!_AudWaitEvent(u1DecId, &u4Event, FALSE, AUD_EVENT_TIMEOUT_MM))
                        {
                            //koro: if issue pause command in this state, should keep waiting for PES
                            if (!fgWaitPlayIssuePause)
                            {
                                vDspSetFifoReadPtr(u1DecId, _arAudDecoder[u1DecId].rCurrentPes.u4Wp);
                                DSP_SetStartPtsToShm(u1DecId, (UINT32)(_arAudDecoder[u1DecId].rCurrentPes.u4Pts & 0xffffffff), _arAudDecoder[u1DecId].rCurrentPes.u4Wp);                        	
                                u4Event = (UINT32)AUD_CMD_AVSYNC;
                                LOG(7, "NO AV sync message retrieve in MM\n");
                            }                                                   
                        }                                                                        
                    }
#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0521                      

                    else if ( (AUD_GetTargetCountry() != COUNTRY_EU ) && (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) )
                    {
                        // If wait Av sync message time out, play audio 
                        if (!_AudWaitEvent(u1DecId, &u4Event, FALSE, AUD_EVENT_TIMEOUT))
                        {
                            u4Event = (UINT32)AUD_CMD_AVSYNC;
                            LOG(7, "NO AV sync message retrieve in ATV \n");
                        }   
                    }
                    else if ( (AUD_GetTargetCountry() == COUNTRY_EU ) && ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) ||
                                  ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_LINE_IN) &&
                                  (_eDualDecStreamSrc == AUD_STREAM_FROM_ANALOG_TUNER))))
                    {
                        // If wait Av sync message time out, play audio 
                        if (!_AudWaitEvent(u1DecId, &u4Event, FALSE, AUD_EVENT_TIMEOUT))
                        {
                            u4Event = (UINT32)AUD_CMD_AVSYNC;
                            LOG(7, "NO AV sync message retrieve in ATV \n");
                        }   
                    }
#else                  
                   else if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) ||
                               (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_LINE_IN))                   
                    {
                        // If wait Av sync message time out, play audio 
                        if (!_AudWaitEvent(u1DecId, &u4Event, FALSE, AUD_EVENT_TIMEOUT))
                        {
                            u4Event = (UINT32)AUD_CMD_AVSYNC;
                            LOG(7, "NO AV sync message retrieve in ATV \n");
                        }   
                    }
#endif                    
                    switch (u4Event)
                    {   
                        case AUD_CMD_STOP:
                            //Laihui: 0428 If from line-in to idle, need to turn off line-in buffer, or other source,like multimedia, buf, will be affected by AFIFO. 
                            AUD_StrInputEnable(u1DecId, _arAudDecoder[u1DecId].eStreamFrom, _arAudDecoder[u1DecId].eDecFormat, FALSE, _arAudDecoder[u1DecId].u4ReceivePesCount);

                            //disable clear MM-related flags (done in swdmx_drvif.c, 20090915)
                            //AUD_MM_Set_Dec_Fmt_Conflict(u1DecId,FALSE);
                            //AUD_MM_Set_Dec_Fmt_Scramble(u1DecId,FALSE);    	
                            // Release Stop command API waiting semaphore 
                            AUD_CommandDone(u1DecId, AUD_CMD_STOP, AUD_OK);
                            _ChangeAudioState(u1DecId, AUD_IDLE);
                            fgWaitPlayIssuePause = FALSE;
                            fgWaitPlayOnPause= FALSE;
                            break;
                        
                        case AUD_CMD_AVSYNC:
                            if (u1DecId == AUD_DEC_MAIN)
                            {
                                DSP_SendDspTaskCmd(DSP_PLAY);
                            }
#ifndef CC_MT5391_AUD_3_DECODER                            
                            else
                            {
                                DSP_SendDspTaskCmd(DSP_PLAY_DEC2);
                            }
#else
                            else if (u1DecId == AUD_DEC_AUX)
                            {
                                DSP_SendDspTaskCmd(DSP_PLAY_DEC2);
                            }
                            else
                            {
                                DSP_SendDspTaskCmd(DSP_PLAY_DEC3);
                            }
#endif
                            _afgIssuePlayComToDsp[u1DecId] = TRUE;
                            AUD_WaitUopCommandDone(u1DecId);
                            LOG( 3,"Aud decoder %d Play\n",u1DecId);
                            _ChangeAudioState(u1DecId, AUD_ON_PLAY);
                            fgWaitPlayIssuePause = FALSE;
                            fgWaitPlayOnPause= FALSE;
                            break;
                        case AUD_CMD_CHANGE_FORMAT:
                            // Do nothing on this state, keep play back
                            AUD_CommandDone(u1DecId, AUD_CMD_CHANGE_FORMAT,TRUE);
                            fgWaitPlayIssuePause = FALSE;
                            fgWaitPlayOnPause= FALSE;
                            break;
                        case AUD_CMD_PAUSE:
                            // Do nothing on this state, keep play back
                            if (!fgWaitPlayOnPause)
                            {   
                                AUD_CommandDone(u1DecId, AUD_CMD_PAUSE,TRUE);
                                //_ChangeAudioState(u1DecId, AUD_WAIT_PAUSE);
                            }
                            fgWaitPlayIssuePause = TRUE;
                            fgWaitPlayOnPause= TRUE;
                            break;    
                        case AUD_CMD_RESUME:
                            // Do nothing on this state, keep play back
                            if (fgWaitPlayOnPause)
                            {   
                                AUD_CommandDone(u1DecId, AUD_CMD_RESUME,TRUE);
                                //_ChangeAudioState(u1DecId, AUD_WAIT_PAUSE);
                            }
                            fgWaitPlayOnPause = FALSE;
                            break;    
                        case AUD_CMD_RESET:
                            _AudDrvResetDSP(u1DecId); //  -- DSP Force --
                            eNewState = AUD_WAIT_PLAY;                            
                            break;
                        default:
                            ASSERT(0);
                    }
                }
                else  // No need of initial AV synchronization control
                {
                    LOG(1,"######### NO NEED SYNC ##########\n");

                    // In DTV case, set decoding start point at FIFO start
            	    if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) ||
            	        (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_BUF_AGT) ||
            	        (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))
                    {
            	        UINT32 u4Fifo1Start;
            	        UINT32 u4Fifo1End;
            	        UINT32 u4Fifo2Start;
            	        UINT32 u4Fifo2End;
            	        
            	        UNUSED(_AUD_GetAudFifo(&u4Fifo1Start, &u4Fifo1End, &u4Fifo2Start, &u4Fifo2End));
                        vDspSetFifoReadPtr(u1DecId, (u1DecId == AUD_DEC_MAIN) ? u4Fifo1Start : u4Fifo2Start);
                    }             
                    
                    if (u1DecId == AUD_DEC_MAIN)
                    {
                        DSP_SendDspTaskCmd(DSP_PLAY);
                        _afgIssuePlayComToDsp[AUD_DEC_MAIN] = TRUE;
                    }    
#ifndef CC_MT5391_AUD_3_DECODER                             
                    else
                    {
                        DSP_SendDspTaskCmd(DSP_PLAY_DEC2);
                        _afgIssuePlayComToDsp[AUD_DEC_AUX] = TRUE;
                    }
#else
                    else if (u1DecId == AUD_DEC_AUX)
                    {
                        DSP_SendDspTaskCmd(DSP_PLAY_DEC2);
                        _afgIssuePlayComToDsp[AUD_DEC_AUX] = TRUE;
                    }
                    else
                    {
                        DSP_SendDspTaskCmd(DSP_PLAY_DEC3);
                        _afgIssuePlayComToDsp[AUD_DEC_THIRD] = TRUE;
                        LOG(1, " DSP_SendDspTaskCmd(DSP_PLAY_DEC3);\n");
                    }                    
#endif
            	    
                    AUD_WaitUopCommandDone(u1DecId);
                    LOG(3, "Aud decoder %d Play\n",u1DecId);

                    // Create the feed stream thread for audio clip play
                    if (_arAudDecoder[u1DecId].eStreamFrom == 
                            AUD_STREAM_FROM_MEMORY)
                    {
                        // Create audio feed stream thread
                        if (OSR_OK != x_thread_create(&_hAudFeedStrThread,
                                                   AUD_DRV_FEEDSTR_THREAD_NAME,
                                                   AUD_DRV_THREAD_STACK_SIZE,
                                                   AUD_DRV_THREAD_PRIORITY,
                                                   (x_os_thread_main_fct) _AudFeedStreamThread,
                                                   sizeof(UINT8),
                                                   (void *)&u1DecId))
                        {
                            _u4FeedStreamResult = (UINT32)AUD_FAIL;
                            _ChangeAudioState(u1DecId, AUD_WAIT_STOPSTREAM);
                            break;
                        }
                    }
                    // Create the copy data thread for data from buffer agent
                    else if (_arAudDecoder[u1DecId].eStreamFrom == 
                            AUD_STREAM_FROM_BUF_AGT)
                    {
                         // Create audio buffer agent thread
                        if (OSR_OK != x_thread_create(&_hAudBagtThread,
                                                   AUD_DRV_BAGT_THREAD_NAME,
                                                   AUD_DRV_THREAD_STACK_SIZE,
                                                   AUD_DRV_THREAD_PRIORITY,
                                                   (x_os_thread_main_fct) _AudBagtThread,
                                                   sizeof(UINT8),
                                                   (void *)&u1DecId))
                        {
                            _ChangeAudioState(u1DecId, AUD_WAIT_STOP);
                            LOG(1, "fail to create buffer agent thread\n");
                            break;
                        }
                    }
                    // Create the copy data thread for data from buffer agent
                    else if (_arAudDecoder[u1DecId].eStreamFrom == 
                            AUD_STREAM_FROM_FEEDER)
                    {
                        static BOOL fgThreadCreate = FALSE;

                        if (!fgThreadCreate)
                        {
                            fgThreadCreate = TRUE;
                            
                            VERIFY(x_sema_create(&_hSemaFeederThreadWait, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
                            
                            // Create audio buffer agent thread
                            if (OSR_OK != x_thread_create(&_hAudFeederThread,
                                                   AUD_DRV_FEEDER_THREAD,
                                                   AUD_DRV_THREAD_STACK_SIZE,
                                                   AUD_DRV_THREAD_PRIORITY,
                                                   (x_os_thread_main_fct) _AudFeederThread,
                                                   sizeof(UINT8),
                                                   (void *)&u1DecId))
                            {
                                _ChangeAudioState(u1DecId, AUD_WAIT_STOP);
                                LOG(1, "fail to create audio feeder thread\n");
                                break;
                            }
                        }

                        // Release semaphore
                        LOG(5, "**** Feeder thead ON ****** \n");
                        VERIFY(x_sema_unlock(_hSemaFeederThreadWait) == OSR_OK);
                            
                    }
                    _ChangeAudioState(u1DecId, AUD_ON_PLAY);
                    /* Release Play command API waiting semaphore */
                    AUD_CommandDone(u1DecId, AUD_CMD_PLAY, AUD_OK);
                }    
#ifdef CC_ENABLE_AV_SYNC
                if ((u1DecId == AUD_DEC_MAIN) && (!_fgVdpReady)
                	&&(_arAudDrvState[AUD_DEC_MAIN] == AUD_ON_PLAY))
                {
                    _WaitVdpReady();
                }
                else
                {
                    //Unmute audio, just in case there is nobody call unmute
                    AUD_DspDecPlayMuteEnable(AUD_DEC_MAIN, FALSE);
                    //LOG(7,"[SRM_AVSYNC] ########### No need to Wait VdpReady ........\n");
                }
#endif
                break;

            case AUD_CHANGE_FORMAT:
#ifdef CC_ENABLE_AV_SYNC
                /*  //move to SRM unstable callback.
                _fgVdpReady = FALSE;
                _fgIgnoreVdpInfo = FALSE;
                _fgwaitForAoutReinit=FALSE;
                _GetVideoCBCount=0;
                _fgVdpChangeState = FALSE;
                */
#endif
                if (u1DecId == AUD_DEC_MAIN)
                {
                    DSP_SendDspTaskCmd(DSP_STOP);
                } 
#ifndef CC_MT5391_AUD_3_DECODER                
                else
                {
                    DSP_SendDspTaskCmd(DSP_STOP_DEC2);
                }
#else
                else if (u1DecId == AUD_DEC_AUX)
                {
                    DSP_SendDspTaskCmd(DSP_STOP_DEC2);
                }
                else
                {
                    DSP_SendDspTaskCmd(DSP_STOP_DEC3);
                }                
#endif
                AUD_WaitUopCommandDone(u1DecId);
                
                // Turn off stream input
                AUD_StrInputEnable(u1DecId, _arAudDecoder[u1DecId].eStreamFrom, _arAudDecoder[u1DecId].eDecFormat, FALSE, 0);

                VERIFY(x_sema_lock(_hSemaLoadCode, X_SEMA_OPTION_WAIT) == OSR_OK);
                if ((u1DecId == AUD_DEC_AUX)&&(_guAudDspTrigAtvFmtchg)) 
                {
                    _guAudDspTrigAtvFmtchg = 0;
                    _LoadAdspCode(AUD_DEC_AUX, _eAtvFmtChgDecFmt);
                    LOG(5, " ATV FMT CHG PLAY\n");
                }
                else
                {
                _LoadAdspCode(u1DecId, _arAudDecoder[u1DecId].eDecFormat);
                }    
                VERIFY(x_sema_unlock(_hSemaLoadCode) == OSR_OK);

                // Update DSP read pointer and pts
                AUD_ResetDspReadPtr(u1DecId);
                DSP_SetStartPtsToShm(u1DecId, (UINT32)(_arAudPes[u1DecId].u4Pts & 0xffffffff), _arAudPes[u1DecId].u4Wp);
                
                // Setup individual decoder setting by source
                _DecoderInitSetup(u1DecId, _arAudDecoder[u1DecId].eStreamFrom, _arAudDecoder[u1DecId].eDecFormat);

                // Turn on stream input
                AUD_StrInputEnable(u1DecId, _arAudDecoder[u1DecId].eStreamFrom, _arAudDecoder[u1DecId].eDecFormat, TRUE, 0);
#ifndef CC_MT5391_AUD_3_DECODER
                DSP_SendDspTaskCmd((u1DecId == AUD_DEC_MAIN) ? DSP_PLAY : DSP_PLAY_DEC2);
#else
                if (u1DecId == AUD_DEC_MAIN)   DSP_SendDspTaskCmd(DSP_PLAY);
                if (u1DecId == AUD_DEC_AUX)     DSP_SendDspTaskCmd(DSP_PLAY_DEC2);
                if (u1DecId == AUD_DEC_THIRD)  DSP_SendDspTaskCmd(DSP_PLAY_DEC3);
#endif
                AUD_WaitUopCommandDone(u1DecId);               
                AUD_CommandDone(u1DecId, AUD_CMD_CHANGE_FORMAT, TRUE);
                _ChangeAudioState(u1DecId, AUD_ON_PLAY);
                break;

            case AUD_ON_PAUSE:
                _AudSetEvent(u1DecId, AUD_CMD_FLAG_STOP  | 
                                      AUD_CMD_FLAG_RESET |
                                      AUD_CMD_FLAG_STOPSTREAM |
                                      AUD_CMD_FLAG_RESUME);
                VERIFY(_AudWaitEvent(u1DecId, &u4Event, TRUE, AUD_EVENT_TIMEOUT));
                    
                switch (u4Event)
                {   
                case AUD_CMD_RESET:
                    _AudDrvResetDSP(u1DecId);  //  -- DSP Force --
                    eNewState = AUD_ON_PAUSE;
                    break;
                case AUD_CMD_STOP:
                    eNewState = AUD_WAIT_STOP;
                    break;

                case AUD_CMD_RESUME:
                    eNewState = AUD_WAIT_RESUME;
                    break;

                case AUD_CMD_STOPSTREAM:
                    eNewState = AUD_ON_PAUSE;
                    //AUD_CommandDone(u1DecId,  AUD_CMD_STOPSTREAM, AUD_OK);
                    fgClipDone = TRUE;
                    break;
                        
                default:
                    ASSERT(0);
                }
                _ChangeAudioState(u1DecId, eNewState);
                break;
                
            case AUD_WAIT_PAUSE:
                _arAudDecoder[u1DecId].eDecState = AUD_DEC_PAUSE;

#ifndef CC_MT5391_AUD_3_DECODER    
                if (u1DecId == AUD_DEC_MAIN)
                {
                    DSP_SendDspTaskCmd(DSP_PAUSE);
                }    
                else
                {
                    DSP_SendDspTaskCmd(DSP_PAUSE_DEC2);
                }
#else
                if (u1DecId == AUD_DEC_MAIN)   DSP_SendDspTaskCmd(DSP_PAUSE);
                if (u1DecId == AUD_DEC_AUX)     DSP_SendDspTaskCmd(DSP_PAUSE_DEC2);
                if (u1DecId == AUD_DEC_THIRD)  DSP_SendDspTaskCmd(DSP_PAUSE_DEC3);
#endif                
                AUD_WaitUopCommandDone(u1DecId);
                LOG(3, "Aud decoder %d Pause\n", u1DecId);
                _ChangeAudioState(u1DecId, AUD_ON_PAUSE);
                AUD_CommandDone(u1DecId, AUD_CMD_PAUSE, AUD_OK);
                break;
                
            case AUD_WAIT_RESUME:
                if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
                {
                    _AudDrvThread_WaitResume(u1DecId);
                }
                else
                {
                    _arAudDecoder[u1DecId].eDecState = AUD_DEC_PLAYING;
#ifndef CC_MT5391_AUD_3_DECODER
                    if (u1DecId == AUD_DEC_MAIN)
                    {
                        DSP_SendDspTaskCmd(DSP_RESUME);
                    }    
                    else
                    {
                        DSP_SendDspTaskCmd(DSP_RESUME_DEC2);
                    }
#else
                    if (u1DecId == AUD_DEC_MAIN)   DSP_SendDspTaskCmd(DSP_RESUME);
                    if (u1DecId == AUD_DEC_AUX)     DSP_SendDspTaskCmd(DSP_RESUME_DEC2);
                    if (u1DecId == AUD_DEC_THIRD)  DSP_SendDspTaskCmd(DSP_RESUME_DEC3);
#endif                          
                    AUD_WaitUopCommandDone(u1DecId);
                    LOG(3, "Aud decoder %d RESUME\n", u1DecId);
                    _ChangeAudioState(u1DecId, AUD_ON_PLAY);
                    AUD_CommandDone(u1DecId, AUD_CMD_RESUME, AUD_OK);
                    if (fgClipDone)
                    {
                        fgClipDone = FALSE;
                        _ChangeAudioState(u1DecId, AUD_IDLE);
                        AUD_CommandDone(u1DecId, AUD_CMD_STOPSTREAM, AUD_OK);
                    }
                }
                
                break;
                
            case AUD_WAIT_STOP:
            case AUD_WAIT_STOPSTREAM:
                _arAudDecoder[u1DecId].eDecState = AUD_DEC_STOP;
#ifdef CC_ENABLE_AV_SYNC
                /*  //move to SRM unstable callback.
                _fgVdpReady = FALSE;
                _fgIgnoreVdpInfo = FALSE;
                 _fgwaitForAoutReinit=FALSE;
                 _GetVideoCBCount=0;
                _fgVdpChangeState = FALSE;                 
                */                
                AUD_DspChannelDelay_initial(0, AUD_CH_ALL, AUD_DEC_MAIN);                
                LOG(7,"[SRM_AVSYNC] ##### AUD_WAIT_STOP, Clear last delay settings................\n");             
#endif
                _arAudDecoder[u1DecId].u4ReceivePesCount = 0;
                _arAudDecoder[u1DecId].u4ReceiveValidPesCount = 0;
                _afgFirstIPicArrived[u1DecId] = FALSE;
                _arAudDecoder[u1DecId].fgMMAoutEnable = FALSE;
                _arAudDecoder[u1DecId].fgMMAoutReady = FALSE;
                _fgAoutTimeoutEnable = FALSE;                            
                if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MEMORY) 
                {
                    _fgForceStopStrmDma = TRUE;
                }
                
                if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_BUF_AGT) 
                {
                     _fgBagtStop = TRUE;
                }

                if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_FEEDER)
                {
                    _fgFeederThreadGo = FALSE;
                }
                if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_HDMI)
                {
                    // Disable HDMI SPDIF bypass
                    LOG(5,"Disable HDMI bypass mode when stopping\n");	
                    AUD_SPDIF_Output_HDMI_Rx(FALSE);
                }
                if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER)
                {
                    _guAudDspTrigAtvFmtchg = 0;
                    //Disable ATV format change if _arAudDecoder[u1DecId] == AUD_WAIT_STOP;
                }
                if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_LINE_IN)
                {
#if 0
                    if (BSP_GetIcVersion() < IC_VER_5382P_AA)
                    {
                        DSP_SetPcmLrckMode(u1DecId,FALSE);
                    }
#endif
                    DSP_SetLineInLrInverse(u1DecId,FALSE);
                }
                if (u1DecId == AUD_DEC_AUX)
                {
                    _AUD_DspStopMuteEnable(AUD_DEC_AUX, TRUE);
                    x_thread_delay(10); // Wait for mute finished 
                }

                if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_HDMI)
                {
                    //Need set clock to internal so we add this API. Don't call AUD_StrInputEnable because it will turn off line-in moduele and case dsp getbits blocked
                    AUD_ClkSetToInternal(u1DecId, AUD_FMT_PCM); //Format is not important here..                
                    //turn off source first, because HDMI may loss clock
                    //AUD_StrInputEnable(u1DecId, _arAudDecoder[u1DecId].eStreamFrom, _arAudDecoder[u1DecId].eDecFormat, FALSE, 0);
                }
                
#ifndef CC_MT5391_AUD_3_DECODER
                if (u1DecId == AUD_DEC_MAIN)
                {
                    DSP_SendDspTaskCmd(DSP_STOP);
                }    
                else
                {
                    DSP_SendDspTaskCmd(DSP_STOP_DEC2);
                }
#else
                if (u1DecId == AUD_DEC_MAIN)   DSP_SendDspTaskCmd(DSP_STOP);
                if (u1DecId == AUD_DEC_AUX)     DSP_SendDspTaskCmd(DSP_STOP_DEC2);
                if (u1DecId == AUD_DEC_THIRD)  DSP_SendDspTaskCmd(DSP_STOP_DEC3);
#endif                  
                AUD_WaitUopCommandDone(u1DecId);
                			   
                _FlushAudioFifo(u1DecId); // dirty fix for parser cann't reset single channel write pointer
                {
                    AUD_DEC_CMD_T eDecCmd;
                    UINT32 u4CmdResult = AUD_OK;
                    
                    eDecCmd = (_arAudDrvState[u1DecId] == AUD_WAIT_STOP) ?
                              AUD_CMD_STOP : AUD_CMD_STOPSTREAM;
                    u4CmdResult = (_arAudDrvState[u1DecId] == AUD_WAIT_STOP) ?
                                  AUD_OK : _u4FeedStreamResult;

                    _ChangeAudioState(u1DecId, AUD_IDLE);
                if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_BUF_AGT) && (_fgBagtWaitFlush))
                {
                     while (_rAudBagtAck.eNtyCond != AUD_BUF_AGT_NFY_COND_FLUSH_DONE)
                     {
                         x_thread_delay(5);
                     }
                     _fgBagtWaitFlush = FALSE;
                }
                    AUD_CommandDone(u1DecId, eDecCmd, u4CmdResult);
                }

                // Terminate the feed stream thread
                if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MEMORY) 
                {
                    VERIFY(x_sema_unlock(_hFeedStreamSyncSema) == OSR_OK);
                    _hAudFeedStrThread = NULL;
                }

                
                AUD_StrInputEnable(u1DecId, _arAudDecoder[u1DecId].eStreamFrom, _arAudDecoder[u1DecId].eDecFormat, FALSE, 0);
                if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_HDMI)
                {
                    _AudHdmiMute(FALSE);
                    _u1HDMIMuteCnt = 0; // Reset mute counter
                }
                //_ChangeAudioState(u1DecId, AUD_IDLE);
               
		 _AUD_DspSetDDCOEnable(FALSE);		
                break;

            case AUD_IDLE:
                _arAudDecoder[u1DecId].eDecState = AUD_DEC_STOP;
                _afgIssuePlayComToDsp[u1DecId] = FALSE;
                
                _AudSetEvent(u1DecId, AUD_CMD_FLAG_PLAY   | 
                                      AUD_CMD_FLAG_STOP   | 
                                      AUD_CMD_FLAG_RESET  |
                                      AUD_CMD_FLAG_RESUME |
                                      AUD_CMD_FLAG_PAUSE |
                                      AUD_CMD_FLAG_LOADCODE);
                VERIFY(_AudWaitEvent(u1DecId, &u4Event, TRUE, AUD_EVENT_TIMEOUT));

                switch (u4Event)
                {   
                    case AUD_CMD_PLAY:
                        eNewState = AUD_WAIT_PLAY;
                        break;
                    case AUD_CMD_STOP:
                        eNewState = AUD_WAIT_STOP;
                        break;
                    case AUD_CMD_RESET:
                        eNewState = AUD_IDLE;  //  -- DSP Force --
                        break;
                    case AUD_CMD_LOADCODE:
                        eNewState = AUD_DECODE_INIT;
                        break;
                    case AUD_CMD_PAUSE:
                        eNewState = AUD_IDLE;
                        AUD_CommandDone(u1DecId, AUD_CMD_PAUSE, AUD_OK);
                        break;
                    case AUD_CMD_RESUME:
                        eNewState = AUD_IDLE;
                        AUD_CommandDone(u1DecId, AUD_CMD_RESUME, AUD_OK);
                        break;    
                    default:
                        eNewState = AUD_IDLE;
                }
                _ChangeAudioState(u1DecId, eNewState);
                break;

            default:
                ASSERT(0);
        } // end of switch
    } //end of while (TRUE)
    
    //lint -e{818}
}

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

void _AUD_AtvAvInitialSync(BOOL fgStable)
{
//#ifndef CC_AUD_DVBT_SUPPORT        
	if(AUD_GetTargetCountry() != COUNTRY_EU)
	{
        if ((_arAudDecoder[AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER)&&(fgStable))
        {
            VERIFY(AUD_DRVCmd(AUD_DEC_MAIN, AUD_CMD_AVSYNC));
        }
	}
//#else
    else
    {
        #ifndef CC_MT5391_AUD_3_DECODER  //gallen 0521    
            if ((_arAudDecoder[AUD_DEC_AUX].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) ||
                ((_arAudDecoder[AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_LINE_IN) &&
                 (_eDualDecStreamSrc == AUD_STREAM_FROM_ANALOG_TUNER)))
        #else
            if ((_arAudDecoder[AUD_DEC_AUX].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) )
        #endif
            {
               if (fgStable== TRUE)
               {
                LOG(1,"#### GOT _AUD_AtvAvInitialSync######\n");
                VERIFY(AUD_DRVCmd(AUD_DEC_AUX, AUD_CMD_AVSYNC));
                VERIFY(AUD_DRVCmd(AUD_DEC_MAIN, AUD_CMD_AVSYNC));
               	}			
            }
    }
//#endif
    _afgAtvVideoDisplayFlag[AUD_DEC_MAIN] = fgStable;
#ifdef CC_MT5391_AUD_3_DECODER
    _afgAtvVideoDisplayFlag[AUD_DEC_AUX] = fgStable;
#endif
}

static BOOL _AudCheckDataRate(UINT8 u1DecId, UINT32 u4PesCnt) //Andrew : the function is for DD Test.
{
    static UINT32 u4LastDataBitrate = 0;
    UINT32 u4DataBitrate;
    static UINT32 u4PrevPesCnt = 0;
    static UINT32 u4PrevStc = 0;
    INT32 i4STCDiff = 0;
    STC_CLOCK rStc;
        
    u4DataBitrate = DSP_GetDataRate(AUD_DEC_MAIN);
    
    if (STC_GetSrc(STC_SRC_A1, &rStc) == STC_VALID) 
    {
        if (u4PesCnt==1)  //The first packet comes, reset prevoius STC
        {
           u4PrevStc = rStc.u4Base;
           i4STCDiff = 0;
        }
        else
        {
            i4STCDiff = (INT32)(rStc.u4Base - u4PrevStc);
            if (i4STCDiff < 0)
            {
                i4STCDiff = -i4STCDiff;
            }            
            u4PrevStc = rStc.u4Base;
        }
        LOG(5, "## u4PesCnt = %d, stc = (%d, %d), u4STCDiff=%d\n",u4PesCnt, u4PrevStc,rStc.u4Base ,i4STCDiff);
    }
    else
    {
        i4STCDiff = 0;
    }

    
    if (u4LastDataBitrate == 0xfffff) //640k to 32k, Stop and Play
    {
        LOG(5, "## _AudCheckDataRate,  Set Play\n");
        VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_PLAY));
        u4LastDataBitrate = u4DataBitrate;
        u4PrevPesCnt = u4PesCnt;
        return TRUE;        
    }    
    else if ((u4DataBitrate == 0x7d00) && (u4LastDataBitrate == 0x9c400)) //from 640k to 32k
    {
        LOG(5, "## _AudCheckDataRate,  u4LastDataBitrate == %x, u4DataBitrate =%x, Set Stop\n",u4LastDataBitrate,u4DataBitrate);
        VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_STOP));
        u4LastDataBitrate = 0xfffff;
        return TRUE;        
    }
    else
    {           
        if ((i4STCDiff > (90000*2)) &&          
            (u4PesCnt == (u4PrevPesCnt+1)) && 
            (u4DataBitrate == 0x7d00))                  //If audio packet received interval > 2 sec, and comes a 32k audio packet... 
        {
            LOG(1, "## _AudCheckDataRate, two audio packet received interval > 2 sec, Set Stop\n",u4LastDataBitrate,u4DataBitrate);
            VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_STOP));
            u4LastDataBitrate = 0xfffff;
            return TRUE;        
        }

        // Nothing to do ...
        u4PrevPesCnt = u4PesCnt;
        u4LastDataBitrate = u4DataBitrate;
        return FALSE;       
    }
}

static void _DtvResetLockStatus(UINT8 u1DecId)
{
    AudDecNfyFct  pfAudDecNfy = NULL;
    UINT32        u4Tmp = 0;
    
    // Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy) == TRUE);
   AUD_DEC_ID_VALIDATE(u1DecId);  //Fix Klocwork warning 
	

    _afgDtvLock[u1DecId] = FALSE;
    _afgDtvPesLock[u1DecId] = TRUE;  // Initial keep pes as lock for the case of 
                                     // initial of changing channel case

    _au4LastPesCnt[u1DecId] = 0;
    _au4LastFrameCnt[u1DecId] = 0;
    _au4FrameCnt[u1DecId] = 0;
    _au4PesCnt[u1DecId] = 0;
    _au1NoFrameCntIncrease[u1DecId] = 0;
    _au1NoPesCntIncrease[u1DecId] = 0;
    _aeDecStatus[u1DecId] = AUD_DECODE_UNKNOWN;
    
    // Notify middleware decoder status       
    if (pfAudDecNfy != NULL)
    {
        pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_DECODE_STATUS_CHG, 
                    (UINT32)_aeDecStatus[u1DecId], u4Tmp);        
    }

    //disable clear MM-related flags (done in swdmx_drvif.c, 20090915)
    //AUD_MM_Set_Dec_Fmt_Conflict(u1DecId,FALSE);
    //AUD_MM_Set_Dec_Fmt_Scramble(u1DecId,FALSE);
}

static void _DtvLockCheck(UINT8 u1DecId)
{
    AudDecNfyFct  pfAudDecNfy = NULL;
    UINT32        u4Tmp = 0;
    AUD_DECODE_STATUS_T  eDecStatus;
    //BOOL fgAudFifoEmpty =  FALSE;
    //UINT32        u4RetryThres = AUD_GetEmptyRetryThres();


    // Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy) == TRUE);

    // Get latest frame count and pes count
    _au4FrameCnt[u1DecId] = u4ReadD2RInfo_AputBank(u1DecId);
    _au4PesCnt[u1DecId]   = _arAudDecoder[u1DecId].u4ReceivePesCount;

    // Lock status update
    // Wait until decoding first frame, then change to lock
    if ((_au4LastPesCnt[u1DecId] != _arAudDecoder[u1DecId].u4ReceivePesCount) &&
        (_au4FrameCnt[u1DecId] > 0))
    {
        _afgDtvLock[u1DecId] = TRUE;
    }
    else
    {
        _afgDtvLock[u1DecId] = FALSE;    
    }

    // If no frame decoder more than threshold, then change to un-lock
    if (_au4LastFrameCnt[u1DecId] == _au4FrameCnt[u1DecId])
    {
        _au1NoFrameCntIncrease[u1DecId] ++;
        if (_au1NoFrameCntIncrease[u1DecId] > DTV_DECODE_ERR_CHECK_THREASHOLD)
        {
            _afgDtvLock[u1DecId] = FALSE;
            _au1NoFrameCntIncrease[u1DecId] = DTV_DECODE_ERR_CHECK_THREASHOLD;
        }
    }
    else
    {
        _au1NoFrameCntIncrease[u1DecId] = 0;
    }

       // Audio pes status update    
    if (_au4LastPesCnt[u1DecId] == _au4PesCnt[u1DecId])
    {
        _au1NoPesCntIncrease[u1DecId] ++;
        if (_au1NoPesCntIncrease[u1DecId] > DTV_LOCK_CHECK_THREASHOLD)
        {
            _afgDtvPesLock[u1DecId] = FALSE;
            _au1NoPesCntIncrease[u1DecId] = DTV_LOCK_CHECK_THREASHOLD;
        }
}
    else
    {
        _au1NoPesCntIncrease[u1DecId] = 0;
        _afgDtvPesLock[u1DecId] = TRUE;
    }


    _au4LastFrameCnt[u1DecId] = _au4FrameCnt[u1DecId];
    _au4LastPesCnt[u1DecId]   = _au4PesCnt[u1DecId];

#if 0
    LOG(5, " $$$$$$ Lock (%d) PesLock (%d), Cnt = %d  $$$$$$\n", 
             _afgDtvLock[u1DecId], _au4PesCnt[u1DecId], _au4FrameCnt[u1DecId]);
#endif


#ifdef DATA_DISC_WMA_SUPPORT    
    if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
        (_arAudDecoder[u1DecId].eDecFormat == AUD_FMT_WMA) &&
        (DSP_GetWmaErrorCode() != 0))		//0 means normal stop
    {
        //WMA goes here because error
        eDecStatus = AUD_DECODE_NOT_SUPPORT;
    }
    else if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
             (_arAudDecoder[u1DecId].eDecFormat == AUD_FMT_MPEG) &&
             (AUD_Is_MMDecFmtConflict(u1DecId))) 
    {
        eDecStatus = AUD_DECODE_FMT_CONFLICT;
    }
    else if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
             ((_arAudDecoder[u1DecId].eDecFormat == AUD_FMT_MPEG) ||
              (_arAudDecoder[u1DecId].eDecFormat == AUD_FMT_AC3) ||
              (_arAudDecoder[u1DecId].eDecFormat == AUD_FMT_AAC)) &&
             (AUD_Is_MMDecFmtScramble(u1DecId))) 
    {
        eDecStatus = AUD_DECODE_NOT_SUPPORT;
    }
    else if (_afgDtvPesLock[u1DecId] || _afgDtvLock[u1DecId])
#else
    if (_afgDtvPesLock[u1DecId] || _afgDtvLock[u1DecId])
#endif
    {    
        eDecStatus = AUD_DECODE_NORMAL;
        _fgAoutTimeoutEnable = FALSE;            
        if ((!_afgDtvLock[u1DecId]) && (_au1NoFrameCntIncrease[u1DecId] >= DTV_DECODE_ERR_CHECK_THREASHOLD))
        {
            //receive PES but cannot decode anymore => decode error
            eDecStatus = AUD_DECODE_ERROR;
        }

    }
    else
    {
        if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
            (!AUD_IsMMAoutReady(AUD_DEC_MAIN)) &&
            (_arAudDecoder[u1DecId].eSynMode != AV_SYNC_FREE_RUN))  //when free run, dtvonplaythread may turn on before PES is sent
        {
            AUD_DrvMMAoutReady_Flag(TRUE);
            AUD_DrvMMAoutEnableNotify(AUD_DEC_MAIN);
            _fgAoutTimeoutEnable = TRUE;            
        }
        eDecStatus = AUD_DECODE_NO_DATA;
    }

    // Notify middleware decoder status       
#ifdef CC_MT5391_AUD_3_DECODER    
    VERIFY(x_sema_lock(_arAudDrvStateSema[u1DecId], X_SEMA_OPTION_WAIT) == OSR_OK);
#endif    
    if ((pfAudDecNfy != NULL) && (eDecStatus != _aeDecStatus[u1DecId]) && (_arAudDrvState[u1DecId] == AUD_ON_PLAY))
    {
        pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_DECODE_STATUS_CHG, 
                    (UINT32)eDecStatus, u4Tmp);        
#ifdef CC_MT5391_AUD_3_DECODER
        if (_IsDualDecMode()&&(u1DecId == AUD_DEC_AUX)&&
           (_arAudDecoder[AUD_DEC_MAIN].eDecFormat == AUD_FMT_PCM))
        {
            pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_AUD_DECODE_STATUS_CHG, 
                        (UINT32)eDecStatus, u4Tmp);
        }            
#endif
    }
#ifdef CC_MT5391_AUD_3_DECODER    
    VERIFY(x_sema_unlock(_arAudDrvStateSema[u1DecId]) == OSR_OK);
#endif    

    _aeDecStatus[u1DecId] = eDecStatus;
              
}

static void _MMEOSNotify(UINT8 u1DecId)
{
    AudDecNfyFct  pfAudDecNfy = NULL;
    
    // Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy));

    if (pfAudDecNfy != NULL)
    {
       // Notify middleware decoder status       
       if (pfAudDecNfy != NULL)
       {
           pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_MM_EOS,0,0);
           LOG(5, "MM send EOS notify done\n");
       }
    }
}
#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0521  -- 2
static void _AudDtvOnPlayThread(void* pvArg)
{
    UNUSED(pvArg);

    #if defined(DEBUG) && defined(CC_DTV_PLAY_DEBUG)
    STC_CLOCK rStc;
    UINT32 u4StcTmp = 0;
    UINT32 u4DspPts;
    UINT32 u4PtsPrsPnt;
    #endif

    while (1)
    {
        if (_afgDtvOnPlayThreadGo[AUD_DEC_MAIN] || _afgDtvOnPlayThreadGo[AUD_DEC_AUX])
        {
            UINT8 u1DecId = AUD_DEC_MAIN;

            // Update main decoder
            u1DecId = AUD_DEC_MAIN;
            
            if (_afgDtvOnPlayThreadGo[u1DecId])
            {   // updating read/write pointer to DMX and DSP
                UINT32 u4ReadPtr;
                UINT32 u4WritePtr;

                if (_DmxPidIdxValid(u1DecId))
                {
                    _AUD_DMX_GetAudPtr(u1DecId, &u4ReadPtr, &u4WritePtr);
                    //LOG(10, "rp, wp = (0x%08x, 0x%08x)\n",u4ReadPtr, u4WritePtr);
                    if (!DMX_UpdateReadPointer(_DecIdToPidIdx(u1DecId), u4ReadPtr, 0))
                    {
                        LOG(5, "1.DMX_UpdateReadPointer fail\n"); 
                    }
                }        
                // DTV lock check
                _DtvLockCheck(u1DecId);
                
                if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))
                {
                    if (_arAudDecoder[u1DecId].rCurrentPes.fgEOS == TRUE)
                    {
                        if ((AUD_IsFifoEmpty(u1DecId)))
                        {
                            if (_fgMMSet[u1DecId]==FALSE)
                            {
                                _MMEOSNotify(u1DecId);  
                                _fgMMSet[u1DecId]=TRUE;
                            }
                        }
                        if (!AUD_IsMMAoutEnable(AUD_DEC_MAIN))
                        {
                        	 x_thread_delay(45);
                        }
                    }
                }
                // debug PTS message
                #if defined(DEBUG) && defined(CC_DTV_PLAY_DEBUG)
                if (STC_GetSrc(STC_SRC_A1, &rStc) == STC_VALID)
                {
                    u4StcTmp = rStc.u4Base;
                }
                else
                {
                    u4StcTmp = 0;
                }
                AUD_GetDspPtsInfo(u1DecId,&u4DspPts,&u4PtsPrsPnt);
                //LOG(9," DEC(%d) CurrentPTS 0x%x\n",u1DecId,dwGetDspAckPTS(FALSE));
                LOG(10,"SPTS[0x%x],Rp[0x%x],PRS[0x%x],stc=0x%x\n",                  
                      u4DspPts,(u4ReadPtr & 0xffffff),u4PtsPrsPnt,u4StcTmp);                  
                #endif
            }

            // Update aux decoder
            u1DecId = AUD_DEC_AUX;

            if (_afgDtvOnPlayThreadGo[u1DecId])
            {   // updating read/write pointer to DMX and DSP
                UINT32 u4ReadPtr;
                UINT32 u4WritePtr;

                if (_DmxPidIdxValid(u1DecId))
                {
                    _AUD_DMX_GetAudPtr(u1DecId, &u4ReadPtr, &u4WritePtr);
                    //LOG(10, "rp, wp = (0x%08x, 0x%08x)\n",u4ReadPtr, u4WritePtr);
                    if (!DMX_UpdateReadPointer(_DecIdToPidIdx(u1DecId), u4ReadPtr, 0))
                    {
                        LOG(5, "2.DMX_UpdateReadPointer fail\n"); 
                    }
                }        
                // DTV lock check
                _DtvLockCheck(u1DecId);
            }    
        }
       
        x_thread_delay(AUD_PTR_UPDATE_TIMEOUT);       
        //LOG(1, "  ############################ _AudDtvOnPlayThread exit ####################\n");
    }
}
#else
static void _AudDtvOnPlayThread(void* pvArg)
{
    UNUSED(pvArg);

    #if defined(DEBUG) && defined(CC_DTV_PLAY_DEBUG)
    STC_CLOCK rStc;
    UINT32 u4StcTmp = 0;
    UINT32 u4DspPts;
    UINT32 u4PtsPrsPnt;
    #endif    

    while (1)
    {
        if (_afgDtvOnPlayThreadGo[AUD_DEC_AUX] || _afgDtvOnPlayThreadGo[AUD_DEC_THIRD] || _afgDtvOnPlayThreadGo[AUD_DEC_MAIN])
        {
            UINT8 u1DecId = AUD_DEC_AUX;

            // Update main decoder
            // In multi media case, the main decoder is AUD_DEC_MAIN
            if (_arAudDecoder[AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
            {
                u1DecId = AUD_DEC_MAIN;
            }
            
            if (_afgDtvOnPlayThreadGo[u1DecId])
            {   // updating read/write pointer to DMX and DSP
                UINT32 u4ReadPtr;
                UINT32 u4WritePtr;

                if (_DmxPidIdxValid(u1DecId))
                {
                    _AUD_DMX_GetAudPtr(u1DecId, &u4ReadPtr, &u4WritePtr);
                    //LOG(10, "rp, wp = (0x%08x, 0x%08x)\n",u4ReadPtr, u4WritePtr);
                    if (!DMX_UpdateReadPointer(_DecIdToPidIdx(u1DecId), u4ReadPtr, 0))
                    {
                        LOG(5, "3.DMX_UpdateReadPointer fail\n"); 
                    }
                }        
                // DTV lock check
                _DtvLockCheck(u1DecId);
                
                if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))
                {
                    if (_arAudDecoder[u1DecId].rCurrentPes.fgEOS == TRUE)
                    {
                        if ((AUD_IsFifoEmpty(u1DecId)))
                        {
                            if (_fgMMSet[u1DecId]==FALSE)
                            {
                                _MMEOSNotify(u1DecId);  
                                _fgMMSet[u1DecId]=TRUE;
                            }
                        }
                        if (!AUD_IsMMAoutEnable(AUD_DEC_MAIN))
                        {
                        	 x_thread_delay(45);
                        }
                    }       
                }
                
                // debug PTS message
                #if defined(DEBUG) && defined(CC_DTV_PLAY_DEBUG)
                //LOG(9," DEC(%d) CurrentPTS 0x%x\n",u1DecId,dwGetDspAckPTS(FALSE));
                if (STC_GetSrc(STC_SRC_A1, &rStc) == STC_VALID)
                {
                    u4StcTmp = rStc.u4Base;
                }
                else
                {
                    u4StcTmp = 0;
                }
                AUD_GetDspPtsInfo(u1DecId,&u4DspPts,&u4PtsPrsPnt);
                LOG(10,"SPTS[0x%x],Rp[0x%x],PRS[0x%x],stc=0x%x\n",                  
                      u4DspPts,(u4ReadPtr & 0xffffff),u4PtsPrsPnt,u4StcTmp);
                #endif                
            }    

            // Update aux decoder
            u1DecId = AUD_DEC_THIRD;

            if (_afgDtvOnPlayThreadGo[u1DecId])
            {   // updating read/write pointer to DMX and DSP
                UINT32 u4ReadPtr;
                UINT32 u4WritePtr;

                if (_DmxPidIdxValid(u1DecId))
                {
                    _AUD_DMX_GetAudPtr(u1DecId, &u4ReadPtr, &u4WritePtr);
                    //LOG(10, "rp, wp = (0x%08x, 0x%08x)\n",u4ReadPtr, u4WritePtr);
                    if (!DMX_UpdateReadPointer(_DecIdToPidIdx(u1DecId), u4ReadPtr, 0))
                    {
                        LOG(5, "4.DMX_UpdateReadPointer fail\n"); 
                    }
                }        
                // DTV lock check
                _DtvLockCheck(u1DecId);
            }    
        }
       
        x_thread_delay(AUD_PTR_UPDATE_TIMEOUT);       
        //LOG(1, "  ############################ _AudDtvOnPlayThread exit ####################\n");
    }
}
#endif

// Just fix warning message for -O2
void AUD_FixWarning(void)
{
    UNUSED(_paszAudState);
    UNUSED(_paszAudCmd);
    UNUSED(_aszAudDualState);
    UNUSED(_aszAudDualCmd);
    UNUSED(_aszCodeType);
}

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------


void AUD_DetectionSyncSema(BOOL fgLock)
{
    if (fgLock)
    {
        INT32 i4Ret;
     
        // Real lock ..
        i4Ret = x_sema_lock_timeout(_hDetectionSyncSema, DUAL_DEC_SYNC_TIME_OUT);
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));
        
        if (i4Ret == OSR_TIMEOUT)
        {
            LOG(1, "AUD_DetectionSyncSema timeout\n");
        }
    }
    else
    {
        VERIFY(x_sema_unlock(_hDetectionSyncSema) == OSR_OK);   
    }   

}

void AUD_DualDecSetSyncSema(BOOL fgLock)
{
    if (fgLock)
    {
        INT32 i4Ret;
        // Try lock to make sure the semaphore was locked
        i4Ret = x_sema_lock(_hDualDecSyncSema, X_SEMA_OPTION_NOWAIT);    

        // Real lock ..
        i4Ret = x_sema_lock_timeout(_hDualDecSyncSema, DUAL_DEC_SYNC_TIME_OUT);
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));
        
        if (i4Ret == OSR_TIMEOUT)
        {
            LOG(1, "AUD_DualDecSetSyncSema timeout\n");
        }
    }
    else
    {
        VERIFY(x_sema_unlock(_hDualDecSyncSema) == OSR_OK);   
    }   
}

BOOL AUD_IsDualDecState(void)
{
    return (_rAudDualDrvState != AUD_DUAL_DEC_IDLE);
}

AUD_DEC_STREAM_FROM_T AUD_GetDualDecStrSrc(void)
{
    return _eDualDecStreamSrc;
}

BOOL AUD_IsDualDecMode(void)
{
    return _fgDualDecModeDrv;
}

void AUD_ScartOutInit(void)
{
    AUD_FMT_T eAudFmt;

    if ((_arAudDrvState[AUD_DEC_MAIN] != AUD_ON_PLAY) &&
        (_arAudDrvState[AUD_DEC_AUX] != AUD_ON_PLAY))
    {
        eAudFmt = _AudAtvFmtDetection();
        UNUSED(AUD_SetDecType(AUD_DEC_AUX, AUD_STREAM_FROM_ANALOG_TUNER, eAudFmt));
        UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, TRUE));
        UNUSED(AUD_DSPCmdPlay(AUD_DEC_AUX));
        UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, FALSE));
    }
}

BOOL AUD_IsSpdifInSrc(UINT8 u1DecId)
{
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
    
    if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_SPDIF)
        ||(_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_FEEDER)
        ||(_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MEMORY))
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

void AUD_UopCommandDone(UINT8 u1DecId, UINT32 u4Command)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    LOG(9, "Dec (%d) AUD_UopCommandDone %d\n", u1DecId, u4Command);
    VERIFY(x_sema_unlock(_ahSemaUopComDone[u1DecId]) == OSR_OK);
}

void AUD_WaitUopCommandDone(UINT8 u1DecId)
{
    INT32 i4Ret;
    
    // Try lock to make sure the semaphore was locked
    i4Ret = x_sema_lock(_ahSemaUopComDone[u1DecId], X_SEMA_OPTION_NOWAIT);    
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
    
    // Real lock ..
    i4Ret = x_sema_lock_timeout(_ahSemaUopComDone[u1DecId], 5000);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));
        
    if (i4Ret == OSR_TIMEOUT)
    {
        LOG(1, "Aud wait command TIMEOUT\n");
        u4DspResueTrigger = 1;  //  -- DSP Force --
    }
}

void AUD_ResetDspReadPtr(UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
	if (!DMX_UpdateReadPointer(_DecIdToPidIdx(u1DecId), _arAudPes[u1DecId].u4Wp, 0))
    {
        LOG(3, "**** DMX_UpdateReadPointer fail ****\n"); 
    }
    vDspSetFifoReadPtr(u1DecId, _arAudPes[u1DecId].u4Wp);
}

/******************************************************************************
* Function: AUD_SetStartPts
*
* Description:
*      For video to set start Pts when first I picture arrived.
*      For AV-Sync implementation
*
* Inputs: -
*      u1DecId: Audio decoder id (0: first decoder, 1: second decoder)
*      u8Pts:   Pts of first I picture
* Outputs: -
* 
* Returns: -
*      TRUE: Succeed
*      FALSE: Fail
******************************************************************************/
BOOL AUD_SetStartPts(UINT8 u1DecId, UINT32 u4Pts)
{
    LOG(7, "##### Dec(%d) AUD_SetStartPts = 0x%08x\n", u1DecId, u4Pts);
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1    
    if (u1DecId > AUD_DEC_AUX)
#else
    if (u1DecId > AUD_DEC_THIRD)
#endif
    {
        LOG(1, "Error: AUD_SetStartPts: invalud Dec ID\n", u1DecId, u4Pts);
        return FALSE; 
    }
    else /* (ucDevId == AUD_DEC_MAIN) || (ucDevId == AUD_DEC_AUX) */
    {
#ifdef CC_MT5391_AUD_3_DECODER
        //koro: for DTV case, must set decoder 1
        if (_arAudDecoder[AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER &&
            _arAudDecoder[AUD_DEC_AUX].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER &&
            u1DecId == AUD_DEC_MAIN)
        {
            u1DecId = AUD_DEC_AUX;    //set decoder 1
        }    
#endif
        _arAudDecoder[u1DecId].u4StartPts = u4Pts;
        _afgFirstIPicArrived[u1DecId] = TRUE;
        _arAudDecoder[u1DecId].u4PesCntAfterIPicReceived = 0;
        return TRUE;
    }       
}

BOOL AUD_SendAudioPes(const DMX_AUDIO_PES_T* prPes)
{
    UINT8 u1DecId;
    DMX_AUDIO_PES_T rPes;
    STC_CLOCK rStc;
#ifndef CC_MT5391_AUD_3_DECODER    
    static UINT32 u4DeltaPts[2] = {0,0};
    BOOL fgDulplicatedPts[2] = {FALSE,FALSE};
    static UINT32 u4LastPts[2] = {0,0};
    static UINT32 u4StartDecWp[2] = {0,0};
    static UINT32 u4PrevWp[2] = {0,0};
    static UINT32 u4StcPtsDiff[2] = {0,0};
    static UINT32 u4CurMaxStcPtsDiff[2] = {0,0};
#else
    static UINT32 u4DeltaPts[3] = {0,0,0};
    BOOL fgDulplicatedPts[3] = {FALSE,FALSE,FALSE};
    static UINT32 u4LastPts[3] = {0,0,0};
    static UINT32 u4StartDecWp[3] = {0,0,0};
    static UINT32 u4PrevWp[3] = {0,0,0};
    static UINT32 u4StcPtsDiff[3] = {0,0,0};
    static UINT32 u4CurMaxStcPtsDiff[3] = {0,0,0};
#endif

    UINT32 u4WpOffset;
    UINT32 u4ShiftDecWp;
    UINT32 u4DeltaWp;    

    if (prPes == NULL)
    {
        return FALSE;
    }

    // Check valid DevicedId;
    AUD_DEC_ID_VALIDATE(prPes->u1DeviceId);
    // Update DeviceId <--> PidIdx
    _arAudDecoder[prPes->u1DeviceId].u1DmxPidIdx = prPes->u1PidIndex;
    u1DecId = prPes->u1DeviceId;

    UNUSED(u4DeltaPts);
  
    x_memcpy(&rPes, (const VOID *)prPes, sizeof(DMX_AUDIO_PES_T));
    x_memcpy(&_arAudPes[u1DecId], (const VOID *)prPes, sizeof(DMX_AUDIO_PES_T));
    
    // Check if dulplicated PTS
    fgDulplicatedPts[u1DecId] = (rPes.u4Pts == u4LastPts[u1DecId]) ? TRUE : FALSE;

    // Check if PTS = 0
    if (rPes.u4Pts == 0)
    {
        if (_arAudDecoder[u1DecId].eSynMode != AV_SYNC_AUDIO_MASTER) 
        {
            fgDulplicatedPts[u1DecId] = TRUE;
            rPes.u4Pts = u4LastPts[u1DecId];
        }
        else
        {
#if 0 //Laihui: [DTV00135223] In multimedia path, not update PTS to DSP
            fgDulplicatedPts[u1DecId] = FALSE;
            u4LastPts[u1DecId] = rPes.u4Pts;
            _arAudDecoder[u1DecId].u4ReceiveValidPesCount++;
#else
            fgDulplicatedPts[u1DecId] = TRUE;
            rPes.u4Pts = u4LastPts[u1DecId];
#endif            
         }
    }
    else
    {
        u4LastPts[u1DecId] = rPes.u4Pts;
        _arAudDecoder[u1DecId].u4ReceiveValidPesCount++;
    }   
 
    if (!fgDulplicatedPts[u1DecId])
    {
        _LogPts(rPes.u1PidIndex, u1DecId, &rPes);
    }    
 
    AUD_DEC_ID_VALIDATE(u1DecId);
    _arAudDecoder[u1DecId].u4ReceivePesCount ++;


   // Store current PES data 
    x_memcpy(&_arAudDecoder[u1DecId].rCurrentPes, (const VOID *)&rPes, sizeof(DMX_AUDIO_PES_T));

   //check if this PES is for EOS flag                
   if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA )
   {
       if (_arAudDecoder[u1DecId].rCurrentPes.fgEOS == TRUE)
       {   
            return TRUE;
       }
   }
   
    // Address translation, translate virtual address to DSP internal address
    rPes.u4Wp = DSP_INTERNAL_ADDR(rPes.u4Wp);

    _AUD_DMX_UpdateWPtr(u1DecId, rPes.u4Wp);

    // FIXME !!
    if (_uAudioDelayPTSFlag[u1DecId])
    {
        rPes.u4Pts += PTS_AC3_CNT * (_uAudioDelayPTSFlag[u1DecId]);
    }
    
    if (_fgDDBannerOn)  //Andrew : For DD Test only.
    {
        if (_AudCheckDataRate(u1DecId, _arAudDecoder[u1DecId].u4ReceivePesCount))
        {
            return TRUE;
        }
    }
    

    // Laihui: add for update audio PTS and PTS-STC Delta to DBS 
    // Store current PTS to Dbs
#ifdef CC_GET_PCR_PTS_INFO
    _prDbase->rPts.u4AudioPts = _arAudDecoder[u1DecId].rCurrentPes.u4Pts;
    {
        STC_CLOCK rStc;
        UINT32 u4CurrentStcPtsDiff;
        
        if ((STC_GetSrc(AUD_DEC_MAIN, &rStc) == STC_VALID) &&  (!fgDulplicatedPts[u1DecId]))
        {
            if (_arAudDecoder[u1DecId].rCurrentPes.u4Pts < rStc.u4Base)
            {
                u4CurrentStcPtsDiff = rStc.u4Base - _arAudDecoder[u1DecId].rCurrentPes.u4Pts;
            }
            else
            {
                u4CurrentStcPtsDiff = _arAudDecoder[u1DecId].rCurrentPes.u4Pts - rStc.u4Base;
            }
            //Store STC-PTS diff to Dbs
	        _prDbase->rPts.u4AudioPtsStcDelta = u4CurrentStcPtsDiff;
	        LOG(11, "  DEC(%d) rPes.u4Wp = %08x, rPes.u4Pts=%08x \n", u1DecId, rPes.u4Wp ,rPes.u4Pts);
    	    LOG(11, "  _arAudDecoder[u1DecId].rCurrentPes.u4Pts=%08x\n", _arAudDecoder[u1DecId].rCurrentPes.u4Pts);
            LOG(11, "  rStc.u4Base=%08x\n", rStc.u4Base);
            LOG(11, "  u4CurrentStcPtsDiff=%08x\n", u4CurrentStcPtsDiff);        
        }
    }
#endif

    // Store first PES data 
    if ((_arAudDecoder[u1DecId].u4ReceiveValidPesCount == 1) &&
         _fgIsFirstTime[u1DecId])
    {
        x_memcpy(&_arAudDecoder[u1DecId].rFirstReceivePes, (const VOID *)&rPes, sizeof(DMX_AUDIO_PES_T));
        _fgIsFirstTime[u1DecId] = FALSE;
#ifdef CC_MT5391_AUD_3_DECODER
        if ((u1DecId == AUD_DEC_AUX)&&(_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER))
#else
        if ((u1DecId == AUD_DEC_MAIN)&&(_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER))
#endif
        {
            if ( (AUD_DspGetIecCopyright(AUD_STREAM_FROM_DIGITAL_TUNER) == IEC_CH_STATUS_BY_SOURCE) &&
                 (_arAudDecoder[u1DecId].eDecState == AUD_DEC_PLAYING) ) //When wait_stop the u4ReceiveValidPesCount may also be 1, but call this function would cause unnecessart Aout-Reinit.
            {   
                UINT8 u1Category;

                if (_arAudDecoder[u1DecId].rFirstReceivePes.fgCopyright != TRUE)
                {
                    // Set copy free
                    u1Category = AUD_DspGetIecCategoryCode(AUD_STREAM_FROM_DIGITAL_TUNER);
                    AUD_DspIecCopyright(FALSE, u1Category);
                }
                else
                {
                    // Set copy never
                    u1Category = AUD_DspGetIecCategoryCode(AUD_STREAM_FROM_NUM);
                    AUD_DspIecCopyright(TRUE, u1Category);                    
                }
            }
        }
        
        u4DeltaPts[u1DecId] = 0;

	    if(AUD_GetTargetCountry() == COUNTRY_EU)
	    {
            u4StcPtsDiff[u1DecId] = 0;
            u4CurMaxStcPtsDiff[u1DecId] = 0; 
	    }

    if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
    {
#ifndef __KERNEL__
        _ChkAudioOnlyControl(u1DecId, TRUE);        //Andrew Wen 2007/6/22 Audio only program set sync mode to free run.
#endif        
    }
        u4PrevWp[u1DecId] = DSP_INTERNAL_ADDR(u4GetAFIFOStart(u1DecId));
    }
    else if (_arAudDecoder[u1DecId].u4ReceiveValidPesCount == 2)
    {
        u4DeltaPts[u1DecId] = rPes.u4Pts - _arAudDecoder[u1DecId].rFirstReceivePes.u4Pts;
        LOG(9, "Delta PTS (%d) = %u\n",u1DecId,u4DeltaPts);
    }
    else if (_arAudDecoder[u1DecId].u4ReceiveValidPesCount == 3)
    {
        // Store first avaible PES data 
        x_memcpy(&_arAudDecoder[u1DecId].rFirstAvaibleReceivePes, (const VOID *)&rPes, sizeof(DMX_AUDIO_PES_T));
    }
 
    if( (AUD_GetTargetCountry() == COUNTRY_EU) 
        && (_arAudDecoder[u1DecId].eStreamFrom != AUD_STREAM_FROM_MULTI_MEDIA)
        && (u1DecId != AUD_DEC_THIRD) )   // decoder 3 doesn't have aout, don't consider decoder3 case. Or need to prepare 2 set of "u4CurrentStcPtsDiff" for both decoder.
    {
        {
            STC_CLOCK rStc;
            UINT32 u4CurrentStcPtsDiff;
            
            if ((STC_GetSrc(STC_SRC_A1, &rStc) == STC_VALID) &&
                (!fgDulplicatedPts[u1DecId]))
            {
                if (prPes->u4Pts < rStc.u4Base)
                {
                    u4CurrentStcPtsDiff = rStc.u4Base - prPes->u4Pts;
                }
                else if ((prPes->u4Pts - MPEG_FRAME_SIZE)< rStc.u4Base)
                {
                    u4CurrentStcPtsDiff = MPEG_FRAME_SIZE;   //Should leave enough time for MPEG decoder. 
                }
                else
                {
                    u4CurrentStcPtsDiff = 0;    
                }

                    if (u4CurrentStcPtsDiff > u4CurMaxStcPtsDiff[u1DecId])
                    {
                        u4CurMaxStcPtsDiff[u1DecId] = (u4CurrentStcPtsDiff > PTS_DRIFT_MAX) ?
                                             PTS_DRIFT_MAX : u4CurrentStcPtsDiff;
                        LOG(5, "Dec (%d) Stc-Ptr Diff = %x\n", u1DecId, u4CurMaxStcPtsDiff[u1DecId]);
                    }

                u4StcPtsDiff[u1DecId] = u4CurMaxStcPtsDiff[u1DecId];
            }

            rPes.u4Pts += u4StcPtsDiff[u1DecId];
        }
    }

    if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
    {
    // Check if audio only program
#ifndef __KERNEL__
    _ChkAudioOnlyControl(u1DecId, FALSE);  //Andrew Wen 2007/6/22 Audio only program set sync mode to free run.
#endif  
    }

    if (rPes.u4Wp >= u4PrevWp[u1DecId])
    {
        u4DeltaWp = rPes.u4Wp - u4PrevWp[u1DecId];
    }
    else
    {
        u4DeltaWp = DSP_INTERNAL_ADDR(u4GetAFIFOEnd(u1DecId))- DSP_INTERNAL_ADDR(u4GetAFIFOStart(u1DecId)) + rPes.u4Wp - u4PrevWp[u1DecId];
    }
    u4PrevWp[u1DecId] = rPes.u4Wp;
                
    if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
        (_arAudDecoder[u1DecId].eDecFormat == AUD_FMT_LPCM))
    {
        if (AUD_IsLpcmFmtPS()) //only for program stream.
        {
        AUD_SetLPCMFmt(rPes.u4Info);
    }
    }
                
    // Check if send PTS to DSP 
    if ((_arAudDrvState[u1DecId] == AUD_ON_PLAY) &&
        (STC_GetSrc(STC_SRC_A1, &rStc) == STC_VALID) && 
        (_arAudDecoder[u1DecId].eSynMode != AV_SYNC_FREE_RUN) &&
        (!fgDulplicatedPts[u1DecId]))
    {
        if (_IsPtsValid(rPes.u1PidIndex, u1DecId, &rPes))
        {
            if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
            {
                u4ShiftDecWp = rPes.u4Wp;
                //MM queue av sync info
                AUD_MMQueueSyncInfo(u1DecId, rPes.u4Pts, u4ShiftDecWp, FALSE);
            }
            else
            {
                if (_DMX_GetInterruptThreshold(DMX_PID_TYPE_ES_AUDIO, &u4WpOffset))
                {
                    if (u4DeltaWp < u4WpOffset)
                    {
                        u4WpOffset = u4DeltaWp;
                    }                    
                    u4ShiftDecWp = rPes.u4Wp - (u4WpOffset);
                    if (DSP_INTERNAL_ADDR(u4ShiftDecWp) < DSP_INTERNAL_ADDR(u4GetAFIFOStart(u1DecId)))
                    {
                         u4ShiftDecWp = DSP_INTERNAL_ADDR(u4GetAFIFOEnd(u1DecId)) - 
                                       (DSP_INTERNAL_ADDR(u4GetAFIFOStart(u1DecId)) - u4ShiftDecWp);
                    }
                }
                else
                {
                    u4ShiftDecWp = rPes.u4Wp;
                }
            }
            //LOG(7," PTS=%x, Wp=%x, Swp=%x, delta=%d, stc=%x\n", rPes.u4Pts, rPes.u4Wp, u4ShiftDecWp,u4DeltaWp ,rStc.u4Base);
            
            if (DSP_SendPts(u1DecId, rPes.u4Pts, u4ShiftDecWp) == DSP_FAIL)
            {
                LOG(7, "DspSendPts fail, DecId = %d\n", u1DecId);
            }
        }
    }

    // Check if send Play command to DSP ?
    if ((_arAudDecoder[u1DecId].eDecState == AUD_DEC_PLAYING) &&
        (_arAudDecoder[u1DecId].eSynMode != AV_SYNC_FREE_RUN) &&
        (!_afgIssuePlayComToDsp[u1DecId]))
    {
        //Andrew Wen 07/2/9 : The delay of dmux updating write pointer must be taken into consideration.
        if (!fgDulplicatedPts[u1DecId])
        {
            //koro: multi-media decode from begin of AFIFO
            if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
            {
                u4StartDecWp[u1DecId] = DSP_INTERNAL_ADDR(u4GetAFIFOStart(u1DecId));
            }
            else if (_DMX_GetInterruptThreshold(DMX_PID_TYPE_ES_AUDIO, &u4WpOffset))
            {
                if((_arAudDecoder[u1DecId].eDecFormat == AUD_FMT_WMA)||(_arAudDecoder[u1DecId].eDecFormat == AUD_FMT_LPCM) ||(_arAudDecoder[u1DecId].eDecFormat == AUD_FMT_PCM)) //for WMA /LPCM format, didn't shift write pointer!
                {
                    u4WpOffset = 0 ;
                }	

			
                if (u4DeltaWp < u4WpOffset)
                {
                    u4WpOffset = u4DeltaWp;
                }                    
                u4StartDecWp[u1DecId] = rPes.u4Wp - u4WpOffset;
                if (DSP_INTERNAL_ADDR(u4StartDecWp[u1DecId]) < DSP_INTERNAL_ADDR(u4GetAFIFOStart(u1DecId)))
                {
                    u4StartDecWp[u1DecId] = DSP_INTERNAL_ADDR(u4GetAFIFOStart(u1DecId));
                }
            }
            else
            {
                u4StartDecWp[u1DecId] = rPes.u4Wp;
            }

            LOG(7, "Dec(%d) u4StartDecWp=%x, rPes.u4Wp = %x, rPes.u4Pts=%x\n", u1DecId, u4StartDecWp[u1DecId],rPes.u4Wp ,rPes.u4Pts);            
        }
        
        if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
        {
            x_memcpy(&_arAudDecoder[u1DecId].rFristDecodePes, 
                     (const VOID *)&rPes, sizeof(PSR_AUDIO_PES_T));
            vDspSetFifoReadPtr(u1DecId, DSP_INTERNAL_ADDR(u4GetAFIFOStart(u1DecId)));
            DSP_SetStartPtsToShm(u1DecId, _arAudDecoder[u1DecId].u4StartPts, u4StartDecWp[u1DecId]);

            VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_AVSYNC));
            _afgIssuePlayComToDsp[u1DecId] = TRUE;
            LOG(5, "===> Dec (%d) First decode PTS (0x%x), PRS (0x%x), Aud PES cnt (%d) for multimedia\n", 
                u1DecId, _arAudDecoder[u1DecId].u4StartPts, u4StartDecWp[u1DecId], _arAudDecoder[u1DecId].u4ReceivePesCount);            
        }
        else if (_afgFirstIPicArrived[u1DecId])
        {   
            UINT64 u8DeltaPts = 0;
            
            u8DeltaPts = (_arAudDecoder[u1DecId].u4StartPts >= (UINT32)(rPes.u4Pts & 0xffffffff)) ? 
                         (_arAudDecoder[u1DecId].u4StartPts - (UINT32)(rPes.u4Pts & 0xffffffff)) :
                         ((UINT32)(rPes.u4Pts & 0xffffffff) - _arAudDecoder[u1DecId].u4StartPts);
            _arAudDecoder[u1DecId].u4PesCntAfterIPicReceived ++;
                 
            if ((u8DeltaPts < PTS_THRESHOLD)     ||
                (u8DeltaPts > PTS_THRESHOLD_MAX) ||
                ((UINT32)(rPes.u4Pts & 0xffffffff) >= _arAudDecoder[u1DecId].u4StartPts)  ||
                 (_arAudDecoder[u1DecId].u4PesCntAfterIPicReceived > (PES_CNT_THRESHOLD/2)))
            {
                x_memcpy(&_arAudDecoder[u1DecId].rFristDecodePes, (const VOID *)&rPes, sizeof(DMX_AUDIO_PES_T));
                vDspSetFifoReadPtr(u1DecId, u4StartDecWp[u1DecId]);
                DSP_SetStartPtsToShm(u1DecId, (UINT32)(rPes.u4Pts & 0xffffffff), u4StartDecWp[u1DecId]);
                VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_AVSYNC));
                _afgIssuePlayComToDsp[u1DecId] = TRUE;
                LOG(5, "===> Dec (%d) StartPTS = 0x%x, AudPTS = 0x%x, Aud PES cnt after I (%d) \n",
                    u1DecId, (UINT32)(_arAudDecoder[u1DecId].u4StartPts & 0xffffffff), 
                    (UINT32)(rPes.u4Pts & 0xffffffff),
                    _arAudDecoder[u1DecId].u4PesCntAfterIPicReceived);
            }
            else
            {
                vDspSetFifoReadPtr(u1DecId, u4StartDecWp[u1DecId]);
                if (!(fgDspReadPtrSet(u1DecId, u4StartDecWp[u1DecId])))
                {
                    LOG(1, "Dec(%d) fgDspReadPtrSet fail\n", u1DecId);
                    //ASSERT(0);
                }
                if (!DMX_UpdateReadPointer(_DecIdToPidIdx(u1DecId), DSP_PHYSICAL_ADDR(u4StartDecWp[u1DecId]), 0))
                {
                    LOG(3, "5.DMX_UpdateReadPointer fail\n"); 
                }
                LOG(11, "1. Update RP = 0x%08x\n", DSP_PHYSICAL_ADDR(rPes.u4Wp));
            }
        }        // Fixed ES id, 
                 // Video always use ES id 0. Workaround for some POP case, middleware doesn't set av-sync mode correctly       
        else if (MPV_IsDisplay(/*(UINT8)STC_GetVdoDeviceId(u1DecId))*/0))
        {
            x_memcpy(&_arAudDecoder[u1DecId].rFristDecodePes, 
                     (const VOID *)&rPes, sizeof(PSR_AUDIO_PES_T));
            vDspSetFifoReadPtr(u1DecId, u4StartDecWp[u1DecId]);
            DSP_SetStartPtsToShm(u1DecId, rPes.u4Pts, u4StartDecWp[u1DecId]);
            VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_AVSYNC));
            _afgIssuePlayComToDsp[u1DecId] = TRUE;
            LOG(5, "===> Dec (%d) First decode PTS (0x%x), Aud PES cnt (%d) \n", 
                u1DecId, _arAudDecoder[u1DecId].rFristDecodePes.u4Pts, _arAudDecoder[u1DecId].u4ReceivePesCount);
        }
        else /* (_afgFirstIPicArrived[ucDevId] == FALSE) */
        {
            UINT32 u4DeltaPts = 0;
            UINT32 u4FifoStart;
            UINT32 u4FifoEnd;
            DSP_GetAFIFOPhysicalAddr(u1DecId, &u4FifoStart, &u4FifoEnd);

            if (_arAudDecoder[u1DecId].u4ReceiveValidPesCount >= PES_VALID_CNT)
            {
                u4DeltaPts = ((UINT32)(_arAudDecoder[u1DecId].rFirstAvaibleReceivePes.u4Pts & 0xffffffff) >= (UINT32)(rPes.u4Pts & 0xffffffff)) ? 
                             ((UINT32)(_arAudDecoder[u1DecId].rFirstAvaibleReceivePes.u4Pts & 0xffffffff) - (UINT32)(rPes.u4Pts & 0xffffffff)) :
                             ((UINT32)(rPes.u4Pts & 0xffffffff) - (UINT32)(_arAudDecoder[u1DecId].rFirstAvaibleReceivePes.u4Pts & 0xffffffff));
            }
            else
            {
                u4DeltaPts = 0;
            }
            
            // Light: DTV takes longer time to wait for I frame.
            // Since we will update Rp to Demux, we don't need to consider FIFO full condition.
            // Wait for I frame for longer time since H.264 I frame comes late very often.
            if ((_arAudDecoder[u1DecId].u4ReceivePesCount > PES_CNT_THRES_I_FRAME) ||
                ((u4DeltaPts > PTS_THRESHOLD_MAX)&&(u4DeltaPts < PTS_THRESHOLD_NOT_VALID)))//||
                //((DSP_PHYSICAL_ADDR(u4FifoEnd) - DSP_PHYSICAL_ADDR(rPes.u4Wp)) < DTV_FIFO_FULL_THRESHOLD))
            { 
                /* No recieve I pic after a long time */
                x_memcpy(&_arAudDecoder[u1DecId].rFristDecodePes, (const VOID *)&rPes, sizeof(PSR_AUDIO_PES_T));
                vDspSetFifoReadPtr(u1DecId, u4StartDecWp[u1DecId]);
                DSP_SetStartPtsToShm(u1DecId, (UINT32)(rPes.u4Pts & 0xffffffff), u4StartDecWp[u1DecId]);
                VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_AVSYNC));
                _afgIssuePlayComToDsp[u1DecId] = TRUE;
                LOG(3, "Dec (%d) No recieve I after a long time. Aud PES cnt (%d)\n", u1DecId, _arAudDecoder[u1DecId].u4ReceivePesCount);
                LOG(3, "Dec (%d) DeltaPts (0x%x)\n", u1DecId, u4DeltaPts);                
                LOG(5, "Dec (%d) First decode PTS (0x%x), Aud PES cnt (%d) \n", 
                    u1DecId, (UINT32)(_arAudDecoder[u1DecId].rFristDecodePes.u4Pts & 0xffffffff), _arAudDecoder[u1DecId].u4ReceivePesCount);
            }
            else
            {
                if (!fgDulplicatedPts[u1DecId])
                {
                    /*
                    vDspSetFifoReadPtr(u1DecId, u4StartDecWp);
                    if (!(fgDspReadPtrSet(u1DecId, u4StartDecWp)))
                    {
                        LOG(1, "Dec(%d) fgDspReadPtrSet fail\n", u1DecId);
                    }
                    */
                    if (!DMX_UpdateReadPointer(_DecIdToPidIdx(u1DecId), DSP_PHYSICAL_ADDR(u4StartDecWp[u1DecId]), 0))
                    {
                        LOG(3, "6.DMX_UpdateReadPointer fail\n"); 
                    }
                    LOG(11, "2. Update RP = 0x%08x\n", DSP_PHYSICAL_ADDR(u4StartDecWp[u1DecId]));
                }
            }
        }/* (_afgFirstIPicArrived[ucDevId] == FALSE) */
    }
    else if ((_arAudDecoder[u1DecId].eDecState == AUD_DEC_STOP) && (!_afgIssuePlayComToDsp[u1DecId]) && 
             (_arAudDecoder[u1DecId].eSynMode != AV_SYNC_FREE_RUN) &&
             (_arAudDecoder[u1DecId].eStreamFrom != AUD_STREAM_FROM_MULTI_MEDIA))  ////koro: multi-media decode from begin of AFIFO
    {
        if (!DMX_UpdateReadPointer(_DecIdToPidIdx(u1DecId), DSP_PHYSICAL_ADDR(rPes.u4Wp), 0))
        {
            LOG(3, "7.DMX_UpdateReadPointer fail, %x\n"); 
        }
    }

    return TRUE;    

}

void AUD_GetAudioPes(UINT8 u1DecId, PSR_AUDIO_PES_T * prAudioPes)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    x_memcpy(prAudioPes, (const VOID *)&_arAudDecoder[u1DecId].rCurrentPes, sizeof(PSR_AUDIO_PES_T));
}

/*
void AUD_GetFisrtAudioPes(UINT8 u1DecId, PSR_AUDIO_PES_T * prAudioPes)
{
     VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
     x_memcpy(prAudioPes, (const VOID *)&_arAudDecoder[u1DecId].rFirstReceivePes, sizeof(PSR_AUDIO_PES_T));
}

void AUD_GetFisrtAudioDecodePes(UINT8 u1DecId, PSR_AUDIO_PES_T * prAudioPes)
{
     VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
     x_memcpy(prAudioPes, (const VOID *)&_arAudDecoder[u1DecId].rFristDecodePes, sizeof(PSR_AUDIO_PES_T));
}
*/

void AUD_ClearFifo(UCHAR ucDecId)
{
    UINT32 u4Rp;
    UINT32 u4Wp;
    UINT32 u4DataSZ;

    AUD_GetRWPtr(ucDecId, &u4Rp, &u4Wp, &u4DataSZ);
    vDspSetFifoReadPtr(ucDecId, u4Wp);
}

BOOL AUD_UpdateAudioFifo(UINT8 u1DecId, UINT32 u4SartAddress, UINT32 u4Size, UINT32 u4WritePtr)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Check if write pointer is within audio FIFO
    VERIFY((PHYSICAL(u4WritePtr) >= PHYSICAL(u4SartAddress)) && 
           ((PHYSICAL(u4SartAddress + u4Size)) >= PHYSICAL(u4WritePtr)));
    
    if (_arAudDecoder[u1DecId].eDecState == AUD_DEC_PLAYING)
    {
        if (u1DecId == AUD_DEC_MAIN)
        {
            DSP_SendDspTaskCmd(DSP_STOP);
            AUD_WaitUopCommandDone(u1DecId);
            vDspSetFifoReadPtr(u1DecId, u4SartAddress);
            if (!fgDspReadPtrSet(u1DecId, u4WritePtr))
            {
                LOG(5, "DspReadPtrSet time out\n");
            }
            DSP_SendDspTaskCmd(DSP_PLAY);
            AUD_WaitUopCommandDone(u1DecId);
        }
#ifndef CC_MT5391_AUD_3_DECODER 
//#if 1
        else // u1DecId == AUD_DEC_AUX
#else
        else if (u1DecId == AUD_DEC_AUX)
#endif
        {
            DSP_SendDspTaskCmd(DSP_STOP_DEC2);
            AUD_WaitUopCommandDone(u1DecId);
            vDspSetFifoReadPtr(u1DecId, u4SartAddress);
            if (!fgDspReadPtrSet(u1DecId, u4WritePtr))
            {
                LOG(5, "DspReadPtrSet time out\n");
            }
            DSP_SendDspTaskCmd(DSP_PLAY_DEC2);
            AUD_WaitUopCommandDone(u1DecId);
        }   
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
        else  // u1DecId == AUD_DEC_THIRD
        {
            DSP_SendDspTaskCmd(DSP_STOP_DEC3);
            AUD_WaitUopCommandDone(u1DecId);
            vDspSetFifoReadPtr(u1DecId, u4SartAddress);
            if (!fgDspReadPtrSet(u1DecId, u4WritePtr))
            {
                LOG(5, "DspReadPtrSet time out\n");
            }
            DSP_SendDspTaskCmd(DSP_PLAY_DEC3);
            AUD_WaitUopCommandDone(u1DecId);
        }
#endif  
    }
    else
    {
        vDspSetFifoReadPtr(u1DecId, u4SartAddress);
        if (!fgDspReadPtrSet(u1DecId, u4WritePtr))
        {
            LOG(5, "fgDspReadPtrSet fail\n");   
        }
    }
    return TRUE;
}

// FALSE: push mode TRUE:pull mode
BOOL AUD_GetOperationMode(UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    return _arAudDecoder[u1DecId].fgOperationMode;
}

// FALSE: push mode TRUE:pull mode
void AUD_SetOperationMode(UINT8 u1DecId, BOOL fgMode)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    _arAudDecoder[u1DecId].fgOperationMode = fgMode;
}

AV_SYNC_MODE_T AUD_GetAvSynMode(UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    return _arAudDecoder[u1DecId].eSynMode;
}

void AUD_DrvSetAvSynMode(UINT8 u1DecId, AV_SYNC_MODE_T eSynMode)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    
    _arAudDecoder[u1DecId].eSynMode = eSynMode;
    vDspSetSyncMode(u1DecId, eSynMode);

    if (u1DecId == AUD_DEC_THIRD)  // decoder 3 use FIFO0.
    {
        u1DecId =AUD_DEC_MAIN;
    }

    // 2008/10/16 Jessica: There is only 1 STC module. STC ID is forced to be 0
    if (STC_SetSyncTarget(AV_SYNC_TARGET_AUD, STC_SRC_A1, u1DecId) == STC_INVALID)
    {
        LOG(5, "STC_SetSyncTarget fail\n");
    }
//    VERIFY(STC_SetPtsDrift(u1DecId, PTS_DRIFT) == STC_VALID);
//  STC_SetAudDeviceId(u1DecId, u1DecId);
}

AV_SYNC_STC_SELECT_T AUD_GetStcId(UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    return _arAudDecoder[u1DecId].eStcId;
}

void AUD_SetStcId(UINT8 u1DecId, AV_SYNC_STC_SELECT_T eStcId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    _arAudDecoder[u1DecId].eStcId = eStcId;
}

// Check if AV is syn
BOOL AUD_AvSynLock(UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    return _arAudDecoder[u1DecId].fgAvSynLock;
}

/***************************************************************************
     Function : AUD_SetStcThreshold
  Description : STC thrshold setting
                Note: set the difference before play
    Parameter :
                u2Threshold  : STC Difference worst bound for system master in ms
    Return    : None
***************************************************************************/
void AUD_SetStcThreshold(UINT8 u1DecId, UINT16 u2Threshold)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    DSP_SetStcDiff(u1DecId, u2Threshold);
}

void AUD_GetDecoderStatus(UINT8 u1DecId, AUD_DECODER_T *prAudDec)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    x_memcpy(prAudDec, (const VOID *)&_arAudDecoder[u1DecId], sizeof(AUD_DECODER_T));
}

void AUD_DRVInit(void)
{
    if(!_fgAudDrvInitiated)
    {
        INT32 i;
        CRIT_STATE_T _rCritState;

        for (i=0; i<AUD_DEC_NUM; i++)
        {
            // Clear decoder info ..
            x_memset((void *)&(_arAudDecoder[i]), 0, sizeof(AUD_DECODER_T));

            // Variable intial value    
            _arAudDecoder[i].rPcmSetting.ePcmDataInfo = PCM_24_BITS;
            _arAudDecoder[i].rPcmSetting.eSampleFreq = FS_44K;
            _arAudDecoder[i].fgMMAoutEnable = FALSE;
            _arAudDecoder[i].fgMMAoutReady = FALSE;
            
        }
            
        // Init variables
        _rFeedTransmitInfo.eTargetMode = FEEDER_PULL_MODE;
        _rFeedTransmitInfo.u4BufferSize = 0;
        _rFeedTransmitInfo.u4Tag = 0;
        _rFeedTransmitInfo.pfnTransHandle = NULL;
        _rFeedTransmitInfo.pfnNotify = NULL;
        _rFeedTransmitInfo.pfnGetStatus = NULL;

	 _prDbase = DBS_Lock(&_rCritState);
       ASSERT(_prDbase);
       VERIFY(DBS_Unlock(_prDbase, _rCritState));

        // Dsp thread init
        vADSPTaskInit();  
        // Driver thread init
        _DrvThreadInit();
        // Detection thread init
        AUD_DetectInit();

#ifdef CC_ENABLE_AV_SYNC
        SRM_RegisterStableCB(_AudSrmStableCb);
#endif
    }

    // Init AvSync setting
    // 2008/10/16 Jessica: There is only 1 STC module. STC ID is forced to be 0
    VERIFY(STC_SetSyncTarget(AV_SYNC_TARGET_AUD, STC_SRC_A1, AUD_DEC_MAIN) == STC_VALID);
    VERIFY(STC_SetSyncTarget(AV_SYNC_TARGET_AUD, STC_SRC_A1, AUD_DEC_AUX) == STC_VALID);
    
    {   //Andrew Wen 2007/6/22 Audio only program set sync mode to free run.
        CRIT_STATE_T rState;
        prDbase = DBS_Lock(&rState);
        ASSERT(prDbase != NULL);     
        VERIFY(DBS_Unlock(prDbase, rState));
    }
    _fgAudDrvInitiated = TRUE;   
}

/****************************************************************************
** Function AUD_DRVGetAudioState(): Returns the current audio state.
**
** Inputs:  Decoder index
**
** Outputs: None.
**
** Returns: Current AudioState.
****************************************************************************/
AUD_DRV_STATE_T AUD_DRVGetAudioState(UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    return (_arAudDrvState[u1DecId]);
}

BOOL AUD_DualDecDrvCmd(AUD_DUAL_DEC_CMD_T eDulDecCmd)
{
    UINT32 u4Msg = (UINT32)eDulDecCmd;
    VERIFY((x_msg_q_send(_hAudDualDecCmdQueue, &u4Msg, sizeof(UINT32), 1)) == OSR_OK);
    return TRUE;
}

BOOL AUD_DualDecSetDecType(AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt)
{
    UNUSED(_eDualDecStreamSrcOrg);
    UNUSED(_eDualDecStreamFmt);	
    _eDualDecStreamSrcOrg = _eDualDecStreamSrc;
    _eDualDecStreamSrc = eStreamFrom;
    _eDualDecStreamFmt = eDecFmt;

    // Save latest ATV format
    //if (eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER)
    //{
    //    _eDualDecLastAtvStreamFmt = eDecFmt;    
    //}
    return TRUE;
}

BOOL AUD_DRVCmd(UINT8 u1DecId, AUD_DEC_CMD_T eCmd)
{
    UINT32 u4Msg = (UINT32)eCmd;
    AUD_DEC_ID_VALIDATE(u1DecId);
    VERIFY((x_msg_q_send(_ahAudCmdQueue[u1DecId], &u4Msg, sizeof(UINT32), 1)) == OSR_OK);
    return TRUE;
}

BOOL AUD_DRVChangeDecodeFormat(UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    _arAudDecoder[u1DecId].eStreamFrom = eStreamFrom;
    _arAudDecoder[u1DecId].eDecFormat =  eDecFmt;
    VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_CHANGE_FORMAT));
    return TRUE;
}

BOOL AUD_DRVSetDecodeType(UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // TRICKY HERE  !! SHOULD check in drv_hdmi.c
    // Check previous stream source, if souce is not HDMI, then unmute HDMI
    if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_HDMI) &&
        (eStreamFrom != AUD_STREAM_FROM_HDMI))
    {
        AUD_DspHdmiModeMuteEnable(u1DecId, FALSE);
        AUD_SPDIF_Output_HDMI_Rx(FALSE);
    }

#if (0)
    if(AUD_GetTargetCountry() == COUNTRY_EU){

        if (eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER)
        {
            _fgDualDecModeDrv = TRUE;    
        }
        else
        {
            _fgDualDecModeDrv = FALSE;        
        }
    }
#endif

    // Save setting to _arAudDecoder
    arAudDecoderOld[u1DecId].eDecFormat = _arAudDecoder[u1DecId].eDecFormat;
    _arAudDecoder[u1DecId].eStreamFrom = eStreamFrom;
    _arAudDecoder[u1DecId].eDecFormat =  eDecFmt;

    // Setup individual decoder setting by source
    _DecoderInitSetup(u1DecId, eStreamFrom, eDecFmt);
    
    // Setup volume gain 
    AUD_DspCfgSetDecType(u1DecId,  eStreamFrom, eDecFmt);
    
    VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_LOADCODE));

    // set SDPIF
    // NOTICE: this function uses _bDspStrTyp and _bDspStrTypDec2 to determine the enable case.
    // Thus, it must calls after DSP code is loaded. (_bDspStrTyp is set at loading DSP code)
    _AUD_DspSetIEC(u1DecId);

    //Setup DVD gpio
    AUD_SetSrc(u1DecId,eStreamFrom, eDecFmt);
    AUD_DspDvdEnable();

    return TRUE;
}


BOOL AUD_DRVSetStreamFrom(UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Save setting to _arAudDecoder
    _arAudDecoder[u1DecId].eStreamFrom = eStreamFrom;
    return TRUE;
}

BOOL AUD_DRVGetStreamFrom(UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T * peStreamFrom)
{
     AUD_DEC_ID_VALIDATE(u1DecId);

     if (peStreamFrom != NULL)
     {
		 *peStreamFrom = _arAudDecoder[u1DecId].eStreamFrom;
     }
     else
     {
		 return FALSE;
     }
     return TRUE;
}


BOOL AUD_DRVGetDecodeType(UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T * peStreamFrom, 
                          AUD_FMT_T * peDecType)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    if ((peStreamFrom != NULL) && (peDecType != NULL))
    {
        *peStreamFrom = _arAudDecoder[u1DecId].eStreamFrom;
        *peDecType = _arAudDecoder[u1DecId].eDecFormat;
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

BOOL AUD_DRVSetDecMemBuffer(UINT8 u1DecId, const MEM_BUFFER_INFO_T * prDecMemBuffer)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    if (prDecMemBuffer == NULL)
    {
        return FALSE;
    }
    
    _arAudDecoder[u1DecId].rMemBuffer.pData = prDecMemBuffer->pData;
    _arAudDecoder[u1DecId].rMemBuffer.u4Length = prDecMemBuffer->u4Length;
    _arAudDecoder[u1DecId].rMemBuffer.u4LoopCount = prDecMemBuffer->u4LoopCount;

    if (u1DecId == AUD_DEC_MAIN)
    {
        AUD_SET_BIT(REG_BS_CFG, PAR_SEL);
    }
    else
    {
        AUD_CLR_BIT(REG_BS_CFG, PAR_SEL);
    }   
    
    return TRUE;
}

void AUD_DrvSetNfy(UINT8 u1DecId, const AUD_NFY_INFO_T * prAudNfyInfo)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    if (prAudNfyInfo != NULL)
    {
        _arAudDecoder[u1DecId].rNfyInfo.pvTag = prAudNfyInfo->pvTag;
        _arAudDecoder[u1DecId].rNfyInfo.pfAudDecNfy = prAudNfyInfo->pfAudDecNfy;    
    }
}

void AUD_DrvGetNfy(UINT8 u1DecId, AUD_NFY_INFO_T * prAudNfyInfo)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
if (prAudNfyInfo != NULL)
    {
        prAudNfyInfo->pvTag = _arAudDecoder[u1DecId].rNfyInfo.pvTag;
        prAudNfyInfo->pfAudDecNfy = _arAudDecoder[u1DecId].rNfyInfo.pfAudDecNfy;    
    }
}

void AUD_StopMemStr(void)
{
    _i4MemStrLoopCnt = 0;
    _fgInfinite = FALSE;
    _fgForceStopStrmDma = TRUE;
}
    
/***************************************************************************
  Function : AUD_AudioPTSDelayEnable
  Description : First audio PTS would be increased . 
                For some low bitrate audio discontinues in the beginning.
  Parameter : 
  Return    : None
                                            Andrew Wen 2007/1/31
***************************************************************************/
void AUD_AudioPTSDelayEnable(UINT8 u1DecId, UINT8 uDlyFactor)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    _uAudioDelayPTSFlag[u1DecId] = uDlyFactor;    
}
    
/***************************************************************************
  Function : AUD_DrvBagtNfyFct
  Description : recieve the notification from buffer agent
  Parameter : 
  Return    : 
***************************************************************************/
BOOL AUD_DrvBagtNfyFct (
    const VOID*                     pv_tag,
    AUD_BUF_AGT_NFY_COND_T     e_nfy_cond,
    UINT32                    ui4_data_1,
    UINT32                    ui4_data_2)
{
    AUD_BUF_AGT_DATA_BLK_T*       pt_data_blk;

    _rAudBagtAck.eNtyCond=e_nfy_cond;
    if (ui4_data_1 != 0)
    {
        pt_data_blk = (AUD_BUF_AGT_DATA_BLK_T*)ui4_data_1;
        _rAudBagtAck.ui4_req_id = pt_data_blk->ui4_req_id;
        _rAudBagtAck.pui1_buff = pt_data_blk->pui1_buff;
        _rAudBagtAck.u4Size = pt_data_blk->u4Size;    
    }

   UNUSED(pv_tag);
   UNUSED(ui4_data_2);
    /*
    Printf("[AUD] data blk req_id %d, buf addr %x, buf size %d\n",
           pt_data_blk->ui4_req_id,
           pt_data_blk->pui1_buff,
           pt_data_blk->z_size);
*/
    return TRUE;
}

/***************************************************************************
  Function : AUD_DrvBagtSetSrcInfo
  Description : set source info from buffer agent 
  Parameter : 
  Return    : None
***************************************************************************/
void AUD_DrvBagtSetSrcInfo (UINT16 ui2_buf_agt_id, const AUD_BUF_AGT_SRC_ALIGN_INFO_T*  t_data_blk
                                                                                                                      , BOOL fgType)
{
    if (t_data_blk)
    {
        _rAudBagtInfo.ui4_align = (UINT32)((t_data_blk->ui4_align == 0) ? BAGT_BLK_ALIGN : t_data_blk->ui4_align);
        _rAudBagtInfo.ui4_min_blk_size = t_data_blk->ui4_min_blk_size;
        //_rAudBagtAck.eNtyCond = AUD_BUF_AGT_NFY_COND_ALIGN_INFO;
        _rAudBagtAck.fgAlignChanged = TRUE;
    }
    if (fgType)
    {
        _rAudBagtInfo.ui2_buf_agt_id = ui2_buf_agt_id;
        _rAudBagtAck.fgAlignChanged = FALSE;
    }
}

/***************************************************************************
  Function : AUD_RegBagtRqstFunc
  Description : register buffer agent data request function
  Parameter : 
  Return    : 
***************************************************************************/
BOOL AUD_RegBagtRqstFunc(UINT32 * pu4Handler, AudBagtDataReqFct AudBagtRqstFct)
{
    _AudBagtRqstInit();
    
    // Check if parameters invalid
    if ( (pu4Handler == NULL) || (AudBagtRqstFct == NULL))
    {
        return FALSE;
    }
    else
    {
        _rAudBagtFctInfo.fgRegistered = TRUE;
        *pu4Handler = _rAudBagtFctInfo.u4Handler;
        _rAudBagtFctInfo.AudBagtDataReqFct = AudBagtRqstFct;
        return TRUE;
    }
}

/***************************************************************************
  Function : AUD_DrvBagtGetRqstId
  Description : return current buffer agent request ID
  Parameter : 
  Return    : 
***************************************************************************/
UINT32 AUD_DrvBagtGetRqstId ()
{
    return (_u4CurrRqstId);
}

/***************************************************************************
  Function : AUD_DrvBagtGetCurrOffset
  Description : return current offset byte address of one block data
  Parameter : 
  Return    : 
***************************************************************************/
UINT32 AUD_DrvBagtGetCurrOffset ()
{
    return _u4CurrOffset;
}

//-----------------------------------------------------------------------------
// AUD_DrvFeederSetRequestFunc
//
/** For feeder to set request information.
 *
 *  @param     prTransmit   Notify function handler
 *
 *  @retval    TRUE   ok
 *  @retval    FALSE  fail
 */
//-----------------------------------------------------------------------------
BOOL AUD_DrvFeederSetRequestInfo(FEEDER_TRANSMIT *prTransmit)
{
    if (prTransmit == NULL)
    {
        return FALSE;
    }
    
    _rFeedTransmitInfo.u4Tag = prTransmit->u4Tag;
    
    if (prTransmit->eTargetMode == FEEDER_PULL_MODE)
    {
        _rFeedTransmitInfo.u4BufferSize   = prTransmit->u4BufferSize;
        _rFeedTransmitInfo.pfnTransHandle = prTransmit->pfnTransHandle;   
        _rFeedTransmitInfo.pfnGetStatus   = prTransmit->pfnGetStatus;
        _rFeedTransmitInfo.pfnNotify      = prTransmit->pfnNotify;
    }
    else if (prTransmit->eTargetMode == FEEDER_PUSH_MODE)
    {
        prTransmit->u4BufferSize = FEEDER_REQUEST_DATA_SIZE;
        _rFeedTransmitInfo.u4BufferSize = FEEDER_REQUEST_DATA_SIZE;
        prTransmit->u4BufferSize = NULL; // Not support yet
    }
    else
    {
        // Do nothing ..
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
// AUD_DrvPcmSetting
//
/** PCM format setting
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      prPcmSetting   
 */
//-----------------------------------------------------------------------------
void AUD_DrvPcmSetting(UINT8 u1DecId, const AUD_PCM_SETTING_T * prPcmSetting)
{
    if (prPcmSetting != NULL)
    {
        _arAudDecoder[u1DecId].rPcmSetting.ePcmDataInfo = prPcmSetting->ePcmDataInfo;
        _arAudDecoder[u1DecId].rPcmSetting.eSampleFreq = prPcmSetting->eSampleFreq;

        DSP_SetPcmType(u1DecId, prPcmSetting->ePcmDataInfo);
        DSP_SetPcmFs(u1DecId, prPcmSetting->eSampleFreq);
        AUD_CfgPcmConfig(u1DecId, prPcmSetting->eSampleFreq);
    }    
}

//-----------------------------------------------------------------------------
// AUD_DrvGetDecodeState
//
/** Get audio decoder status
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX) 
 */
//-----------------------------------------------------------------------------
AUD_DECODER_T * AUD_DrvGetDecodeState(UINT8 u1DecId)
{
    return  &_arAudDecoder[u1DecId];
}


void AUD_DrvSetNotifyFunc(UINT8 u1DecId, AUD_CLIP_NOTIFY pfClipNotify)
{
    UNUSED(u1DecId);
    UNUSED(_hClipNotifyFunc);
    _hClipNotifyFunc =  pfClipNotify;  
}


//-----------------------------------------------------------------------------
// AUD_DrvSetDDBanner
//
/** Turn on DDBanner.
 *
 *  @param     fgEnable     TRUE/FALSE
 *
 *  @retval    void
 */
//-----------------------------------------------------------------------------
void AUD_DrvSetDDBanner(BOOL fgEnable)  //Andrew : Enhance DD Banner
{
    _fgDDBannerOn = fgEnable; 
}

void AUD_DrvATunerLock(void)
{
    UNUSED(x_sema_unlock(_hAtunerSyncSema));
}

void AUD_DRV_PlayDualDmx(BOOL fgDualDmx) //Audio Description play dual demux
{
    UNUSED(fgDualDmxMode);
    fgDualDmxMode = fgDualDmx;
#ifndef CC_MT5391_AUD_3_DECODER    
//#if 1
    if (fgDualDmx)
    {       
        if (!AUD_IsDecoderPlay(AUD_DEC_AUX))
        {
            AUD_DSPCmdPlay(AUD_DEC_AUX);
        }       
    }
    else        
    {
        if (AUD_IsDecoderPlay(AUD_DEC_AUX))
        {
            AUD_DSPCmdStop(AUD_DEC_AUX);
        }                   
        else
        {
            AUD_CommandDone(AUD_DEC_AUX, AUD_CMD_STOP, AUD_OK);
        }
#else
    if (fgDualDmx)
    {       
        if (!AUD_IsDecoderPlay(AUD_DEC_THIRD))
        {
            AUD_DSPCmdPlay(AUD_DEC_THIRD);
        }       
    }
    else        
    {
        if (AUD_IsDecoderPlay(AUD_DEC_THIRD))
        {
            AUD_DSPCmdStop(AUD_DEC_THIRD);
        }                   
        else
        {
            AUD_CommandDone(AUD_DEC_THIRD, AUD_CMD_STOP, AUD_OK);
        }
#endif
    }
}

//-----------------------------------------------------------------------------
// AUD_DrvSetPIPVideoPath
//
/** Set PIP/POP video path information
 *
 *  @param     u1VideoPath     0: main frame, 1: sub frame
 *
 *  @retval    void
 */
//-----------------------------------------------------------------------------
void AUD_DrvSetPIPVideoPath(UINT8 u1DecId, UINT8 u1VideoPath)
{
    UNUSED(u1DecId);
    _u1VideoFrameFlag = u1VideoPath;
}

UINT8 AUD_DrvGetPIPVideoPath(void)
{
    UNUSED(AUD_DrvGetPIPVideoPath);
    return _u1VideoFrameFlag; //0 : main, 1: sub.
}

//-----------------------------------------------------------------------------
// AUD_DrvDtvLock
//
/** Check DTV lock status
 *
 *  @param     u1DecId     
 *
 *  @retval    TRUE        Lock
 *  @retval    FALSE       Unlock
 */
//-----------------------------------------------------------------------------

BOOL AUD_DrvDtvLock(UINT8 u1DecId)
{
    return _afgDtvLock[u1DecId];
}

//-----------------------------------------------------------------------------
/** AUD_SendApllAdjustCmd
 *  Send APLL MODIN adjust command
 *
 *  @param  u4Level    video calculated level (0 ~ 255. 128 as center)
 *                     bit 8: 0 (slower), 1 (faster)
 *                            
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_SendApllAdjustCmd(UINT32 u4Level)
{
    static UINT32 u4PrevLevel = 0;
    INT32 i4Ret=0;
    
    if (u4PrevLevel != u4Level)
    {
        u4PrevLevel = u4Level;

#ifndef BURN_IN_AOUT_TEST //sunman_aout
        i4Ret = x_msg_q_send(_hAudApllCmdQueue, &u4Level, sizeof(UINT32), 1);
#endif        

        if (i4Ret ==  OSR_TOO_MANY)
        {
             LOG(3, "AudApllCmdQueue Full ...\n");       
        }
        else if (i4Ret != OSR_OK)
        {
            LOG(1, "AUD_SendApllAdjustCmd  Fail !\n");
        }
    }      
    
}

//sunman_aout
#ifdef BURN_IN_AOUT_TEST
void AUD_SendAoutMonitorCmd(UINT32 u4Level)
{
    INT32 i4Ret=0;
    
    i4Ret = x_msg_q_send(_hAudApllCmdQueue, &u4Level, sizeof(UINT32), 1);

    if (i4Ret ==  OSR_TOO_MANY)
    {
         LOG(1, "AudApllCmdQueue Full ...\n");       
    }
    else if (i4Ret != OSR_OK)
    {
        LOG(1, "AUD_SendApllAdjustCmd  Fail !\n");
    }    
}
#endif

void AUD_SetAC3DataRateCfg(UINT8 u1DecId, UINT8 u1Config)
{
    if (u1Config)
    {
        // enlarge stc_diff bound.  check 0x82a4 ~ 82a6
        AUD_AudioSetStcDiffBound(AUD_DEC_MAIN,0,1,3);
        // add audio start decoding pts
        //AUD_AudioPTSDelayEnable(AUD_DEC_MAIN, 1);        
	}
	else
	{
        // enlarge stc_diff bound.  check 0x82a4 ~ 82a6
        AUD_AudioSetStcDiffBound(AUD_DEC_MAIN, 0, 0, 0);
        // add audio start decoding pts
        AUD_AudioPTSDelayEnable(AUD_DEC_MAIN, 0);
	}
}

inline void _AudDrvThread_WaitResume(UINT8 u1DecId)
{
    UINT32 u4Event = 0;

    _arAudDecoder[u1DecId].eDecState = AUD_DEC_PLAYING;    
    _arAudDecoder[u1DecId].u4ReceiveValidPesCount = 0;
    _afgFirstIPicArrived[u1DecId] = FALSE;
    _afgIssuePlayComToDsp[u1DecId] = FALSE;
        
    // Check if need AV synchronization control
    if (_AvSyncControl(u1DecId))
    {
        LOG(1,"######### NEED SYNC ##########\n");
        AUD_CommandDone(u1DecId, AUD_CMD_RESUME, AUD_OK);
        _arAudDecoder[u1DecId].eDecState = AUD_DEC_PLAYING;        
        
        // Wait Av-Sync or User Stop command 
        _AudSetEvent(u1DecId, AUD_CMD_FLAG_STOP   | 
                              AUD_CMD_FLAG_RESET  |
                              AUD_CMD_FLAG_AVSYNC |
                              AUD_CMD_FLAG_CHANGE_FORMAT);
        
        if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
        {                     
            UINT32 u4ReadPtr;
            UINT32 u4WritePtr;
            UINT32 u4PTS;
            INT32  i4StcDiff;
            STC_CLOCK rStc;

            _AUD_DMX_GetAudPtr(u1DecId, &u4ReadPtr, &u4WritePtr);

            {
                LOG(5,"_aeMMSyncInfo[0]  = 0x%x,0x%x\n",_aeMMSyncInfo[0].u4PTS,_aeMMSyncInfo[0].u4PtsWP);
                LOG(5,"_aeMMSyncInfo[%d] = 0x%x,0x%x\n",u2CurrIdx,_aeMMSyncInfo[u2CurrIdx].u4PTS,_aeMMSyncInfo[u2CurrIdx].u4PtsWP);
                LOG(5,"_aeMMSyncInfo[%d] = 0x%x,0x%x\n",MM_SYNC_INFO_NUM-1,_aeMMSyncInfo[MM_SYNC_INFO_NUM-1].u4PTS,_aeMMSyncInfo[MM_SYNC_INFO_NUM-1].u4PtsWP);                
            }
            AUD_MMFindSyncInfo(u1DecId,&u4PTS,u4ReadPtr);                        
            LOG(7, "==> rp, wp = (0x%08x, 0x%08x)\n", (u4ReadPtr & 0xffffff), (u4WritePtr & 0xffffff));
            i4StcDiff = 0;
            if (STC_GetSrc(STC_SRC_A1, &rStc) == STC_VALID)
            {
                i4StcDiff = (INT32)(u4PTS - rStc.u4Base);
                LOG(5, "# STC  = 0x%x\n", rStc.u4Base);                
                LOG(5, "# Diff = %d\n", i4StcDiff);
            }                                    
            if (!DMX_UpdateReadPointer(_DecIdToPidIdx(u1DecId), u4ReadPtr, 0))
            {
                LOG(3, "8.DMX_UpdateReadPointer fail\n"); 
            }     

            //Set Delay Here            
            if (i4StcDiff > 0)
            {
                i4StcDiff = (i4StcDiff/900) * 10;  // The delay is (diff / 90k)ms 
                if (i4StcDiff > 500) //at most 1000ms
                {                    
                    i4StcDiff = 500;
                }
                LOG(5,"######## !!!! Delay for %d ms #######################\n",i4StcDiff);
                x_thread_delay((UINT32)i4StcDiff);                
            }
            
            u4Event = (UINT32)AUD_CMD_AVSYNC;
        }
        
        switch (u4Event)
        {   
            case AUD_CMD_STOP:
            	_arAudDecoder[u1DecId].u4ReceivePesCount = 0;
                // Release Stop command API waiting semaphore 
                AUD_CommandDone(u1DecId, AUD_CMD_STOP, AUD_OK);
                _ChangeAudioState(u1DecId, AUD_IDLE);
                break;
                
            case AUD_CMD_AVSYNC:
                if (u1DecId == AUD_DEC_MAIN)
                {
                    DSP_SendDspTaskCmd(DSP_RESUME);
                }
#ifndef CC_MT5391_AUD_3_DECODER                            
                else
                {
                    DSP_SendDspTaskCmd(DSP_RESUME_DEC2);
                }
#else
                else if (u1DecId == AUD_DEC_AUX)
                {
                    DSP_SendDspTaskCmd(DSP_RESUME_DEC2);
                }
                else
                {
                    DSP_SendDspTaskCmd(DSP_RESUME_DEC3);
                }
#endif
                _afgIssuePlayComToDsp[u1DecId] = TRUE;
                AUD_WaitUopCommandDone(u1DecId);
                LOG( 3,"Aud decoder %d Resume\n",u1DecId);
                _ChangeAudioState(u1DecId, AUD_ON_PLAY);
                break;
            case AUD_CMD_CHANGE_FORMAT:
                _arAudDecoder[u1DecId].u4ReceivePesCount = 0;
                // Do nothing on this state, keep play back
                AUD_CommandDone(u1DecId, AUD_CMD_CHANGE_FORMAT,TRUE);
                break;
                
            default:
                ASSERT(0);
        }
    }
    else  // No need of initial AV synchronization control
    {
        LOG(1,"######### NO NEED SYNC ##########\n");
        AUD_CommandDone(u1DecId, AUD_CMD_RESUME, AUD_OK);
        _arAudDecoder[u1DecId].eDecState = AUD_DEC_PLAYING;                
        
	    if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
        {
	        UINT32 u4Fifo1Start;
	        UINT32 u4Fifo1End;
	        UINT32 u4Fifo2Start;
	        UINT32 u4Fifo2End;
	        
	        UNUSED(_AUD_GetAudFifo(&u4Fifo1Start, &u4Fifo1End, &u4Fifo2Start, &u4Fifo2End));
            vDspSetFifoReadPtr(u1DecId, (u1DecId == AUD_DEC_MAIN) ? u4Fifo1Start : u4Fifo2Start);
        }                	               
        if (u1DecId == AUD_DEC_MAIN)
        {
            DSP_SendDspTaskCmd(DSP_RESUME);
            _afgIssuePlayComToDsp[AUD_DEC_MAIN] = TRUE;
        }    
#ifndef CC_MT5391_AUD_3_DECODER                             
        else
        {
            DSP_SendDspTaskCmd(DSP_RESUME_DEC2);
            _afgIssuePlayComToDsp[AUD_DEC_AUX] = TRUE;
        }
#else
        else if (u1DecId == AUD_DEC_AUX)
        {
            DSP_SendDspTaskCmd(DSP_RESUME_DEC2);
            _afgIssuePlayComToDsp[AUD_DEC_AUX] = TRUE;
        }
        else
        {
            DSP_SendDspTaskCmd(DSP_RESUME_DEC3);
            _afgIssuePlayComToDsp[AUD_DEC_THIRD] = TRUE;
            LOG(1, " DSP_SendDspTaskCmd(DSP_RESUME_DEC3);\n");
        }                    
#endif
	    
        AUD_WaitUopCommandDone(u1DecId);
        LOG(3, "Aud decoder %d Resume\n",u1DecId);
        
        _ChangeAudioState(u1DecId, AUD_ON_PLAY);
    }                    

    return;
}

void AUD_MMQueueSyncInfo(UINT8 u1DecId, UINT32 u4PTS, UINT32 u4PTSWp, BOOL fgReset)
{
    static UINT16 u2Idx = 0;
    UINT16 i;        
	UINT32 u4Fifo1Start;
	UINT32 u4Fifo1End;
	UINT32 u4Fifo2Start;
	UINT32 u4Fifo2End;
	        
    UNUSED(_AUD_GetAudFifo(&u4Fifo1Start, &u4Fifo1End, &u4Fifo2Start, &u4Fifo2End));    

    UNUSED(u1DecId);
	UNUSED(u4Fifo2Start);
	UNUSED(u4Fifo2End);
    UNUSED(u2CurrIdx);	
    UNUSED(u2LatestIdx);    
    UNUSED(fgLoopBack);    

    if (fgReset)
    {
        u2Idx = 0; 
        u2LatestIdx = 0;
        fgLoopBack = FALSE;
        for (i=0; i<MM_SYNC_INFO_NUM; i++)
        {
            _aeMMSyncInfo[i].u4PTS = 0;
            _aeMMSyncInfo[i].u4PtsWP = 0xfffffff;        
        }
    }
    else
    {   
        if (u2Idx >= MM_SYNC_INFO_NUM)
        {
            LOG(7," ### MM Sync Info - The deepest u2Idx = %d ###\n", u2Idx);
            LOG(7,"_aeMMSyncInfo[0]  = 0x%x,0x%x\n",_aeMMSyncInfo[0].u4PTS,_aeMMSyncInfo[0].u4PtsWP);
            LOG(7,"_aeMMSyncInfo[%d] = 0x%x,0x%x\n",u2Idx-1,_aeMMSyncInfo[u2Idx-1].u4PTS,_aeMMSyncInfo[u2Idx-1].u4PtsWP);
            u2Idx = 0;
            fgLoopBack = TRUE;
        }
        LOG(9,"MM Q %d, P 0x%x, W 0x%x\n",u2Idx,u4PTS,u4PTSWp);
       
        _aeMMSyncInfo[u2Idx].u4PTS = u4PTS;
        _aeMMSyncInfo[u2Idx].u4PtsWP = u4PTSWp;
        u2CurrIdx = u2Idx;        
        u2Idx += 1;
    }
}

void AUD_MMFindSyncInfo(UINT8 u1DecId, UINT32* u4PTS, UINT32 u4PTSRp)
{
    UINT16 i;
    UINT16 u2Idx;
    BOOL fgFindFifoEnd = FALSE;

    UNUSED(u1DecId);
    UNUSED(u2Idx);    

    u4PTSRp &= 0xffffff;
    *u4PTS = 0;

    u2Idx = u2CurrIdx;        
    VERIFY(u2Idx < MM_SYNC_INFO_NUM);
    for (i=0; i<MM_SYNC_INFO_NUM; i++)
    {
        if (fgFindFifoEnd)
        {
            if (u4PTSRp >= _aeMMSyncInfo[u2Idx].u4PtsWP)
            {
                LOG(5,"Request Rp = 0x%x\n", u4PTSRp);
                LOG(5,"MM Find idx = %d, Rp = 0x%x, PTS = 0x%x @@@@@@@@@@@@@@@@@@@@@\n",u2Idx, 
                _aeMMSyncInfo[u2Idx].u4PtsWP, _aeMMSyncInfo[u2Idx].u4PTS);
                *u4PTS = _aeMMSyncInfo[u2Idx].u4PTS;
                break;
            }
        }    
        else
        {
            if (u4PTSRp <= _aeMMSyncInfo[u2Idx].u4PtsWP)
            {
                fgFindFifoEnd = TRUE;
            }            
        }
    
        if (u2Idx == 0)
        {
            u2Idx = MM_SYNC_INFO_NUM;
        }
        u2Idx--;        
    }    
}

//-----------------------------------------------------------------------------
// AUD_DrvDmxIntThreshold
//
/** Set DMX interrupt threshold.
 *
 *  @param[in]          u1DecId         Decoder ID  
 *                      u1Factor        threshold = 188 * u1Factor
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DrvDmxIntThreshold(UINT8 u1DecId, UINT8 u1Factor)
{
    if ((u1Factor > 0) &&
        (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER))
    {
        _DMX_SetInterruptThreshold(DMX_PID_TYPE_ES_AUDIO, (UINT32)(1+188*(u1Factor-1)));
        LOG(5, "Set DMX threshold to 188*%d\n", u1Factor);
    }
}

#if 0
void AUD_MMAllocSyncInfo(UINT8 u1DecId, BOOL fgFree)
{
    UINT32 u4Size;

    u4Size = sizeof(AUD_MM_SYNC_INFO_T) * MM_SYNC_INFO_NUM;
    if (fgFree)
    {
        VERIFY(BSP_FreeAlignedDmaMemory(_paeMMSyncInfo));
        _paeMMSyncInfo = 0;
        LOG(5, "Free MM av sync memory 0x%d \n",_paeMMSyncInfo);
    }
    else
    {
        _paeMMSyncInfo = (UINT8 *)BSP_AllocAlignedDmaMemory(u4Size, 64);    
        if (_paeMMSyncInfo == NULL)
        {
            LOG(5, "Memory request for MM av sync memory fail !!\n");
        }
    }
}
#endif

BOOL AUD_IsMMAVInitSync(UINT8 u1DecId)
{
    BOOL fgRet = FALSE;

#ifdef CC_AUD_SUPPORT_MM_AV_INIT_SYNC
    
    // Only DTV & ATV need AV synchronization
    if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
    {
        fgRet = TRUE;
    }
    else
    {
        fgRet = FALSE;
    }

#else

    UNUSED(u1DecId);
    fgRet = FALSE;

#endif
    
    return fgRet;    
}

void AUD_DrvMMAoutEnable(UINT8 u1DecId, BOOL fgEnable)
{
    UNUSED(fgEnable);

    _arAudDecoder[u1DecId].fgMMAoutEnable = TRUE;

    if (u1DecId == AUD_DEC_MAIN)
    {
        DSP_SendDspTaskCmd(DSP_CLEAR_AOUT_WAIT);    
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        DSP_SendDspTaskCmd(DSP_CLEAR_AOUT_WAIT_DEC2);    
    }
    else
    {
        LOG(3, "Not support AUD_MMAoutEnable for decoder 3 \n");
    }
}

BOOL AUD_IsMMAoutEnable(UINT8 u1DecId)
{
    return _arAudDecoder[u1DecId].fgMMAoutEnable;
}

BOOL AUD_IsMMAoutReady(UINT8 u1DecId)
{
    return _arAudDecoder[u1DecId].fgMMAoutReady;
}

#ifdef CC_ENABLE_AV_SYNC
void AUD_VdpReady(BOOL fgEnable)
{
    _fgVdpReady = fgEnable;
    UNUSED(x_sema_unlock(_hVdpSyncSema));
}

BOOL AUD_IsIgnoreVdpInfo (void)
{
    return _fgIgnoreVdpInfo;
}

#endif


void _AudClearEOSFlag(UINT8 u1DecId)
{
    _arAudDecoder[u1DecId].rCurrentPes.fgEOS = FALSE;
}

AUD_DEC_STREAM_FROM_T _AudGetStrSource(UINT8 u1DecId)
{
    return _arAudDecoder[u1DecId].eStreamFrom;
}

void AUD_DrvMMAoutEnableNotify(UINT8 u1DecId)
{
    UNUSED(_fgAoutTimeoutEnable);
    if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
    {
        #ifdef ENABLE_MULTIMEDIA
        if (!_fgAoutTimeoutEnable)
        {
        SWDMX_AudioNotify(ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE);
        }
        #endif
    }
}

void AUD_DrvMMAoutEnable_Flag(BOOL fgEnable)
{
    _arAudDecoder[AUD_DEC_MAIN].fgMMAoutEnable = fgEnable;
}

void AUD_DrvMMAoutReady_Flag(BOOL fgEnable)
{
    _arAudDecoder[AUD_DEC_MAIN].fgMMAoutReady = fgEnable;
}

AUD_FMT_T _AudGetStrFormat(UINT8 u1DecId)
{
    return _arAudDecoder[u1DecId].eDecFormat;
}

//-----------------------------------------------------------------------------
// AUD_Drv_ForceReset
//
/** AUD_Drv_ForceReset
 *
 *  @param[in]          
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_Drv_ForceReset(BOOL fgEnable) //  -- DSP Force --
{
    UNUSED(_fgAudDrvForceReset);
    _fgAudDrvForceReset = fgEnable;
}

// msut add AD protection!!!!!! by Shawn
static void _AudDspMonitorThread(void* pvArg) //  -- DSP Force --
{
#ifndef DSP_FORCE_RESET_ENABLE
    UNUSED(u4DspResueTrigger);
    UNUSED(_fgAudDrvForceReset);
    UNUSED(_hAudDspMonThread);
    UNUSED(_AudDspMonitorThread);
#else
    UINT32 u4AputBank = 0, u4PrvAputBank = 0xFFFFFFFF;
    UINT32 u4InputUnerRun = 0, u4PrvInputUnerRun = 0xFFFFFFFF;
    UINT32 u4ReadPtr = 0, u4PrvReadPtr = 0;    
    UINT32 u4Tick = 0, u4Tick1 = 0, u4Tick2 = 0;
    UINT8  u1DecId;
    UINT32 u4DspMayBeDead_st0 = 0;
    UINT32 u4DspMayBeDead_st1 = 0;
    UINT32 u4NeedToRescueDsp = 0, u4NeedToRecoverPlay = 0;
    INT8 i = 0;
    
    UNUSED(u4NeedToRecoverPlay);    
    x_thread_delay(DSP_MONITOR_THREAD_START_TIME); 

    while (1)
    {
        /* ====================================
            DSP Hang detection
           ==================================== */
        if ((_arAudDrvState[AUD_DEC_MAIN] == AUD_ON_PLAY)
        	&& (_arAudDecoder[AUD_DEC_MAIN].eDecState == AUD_DEC_PLAYING)         	
        	&& _afgIssuePlayComToDsp[AUD_DEC_MAIN]
        	&& (_arAudDecoder[AUD_DEC_MAIN].eDecFormat != AUD_FMT_DETECTOR)) //For detector        	
        {
            u4Tick ++;      
            u4InputUnerRun = u4ReadD2RInfo_InputUnderRun(AUD_DEC_MAIN);    

            LOG(11, "u4AputBank = %X(%X), u4InputUnerRun =%X(%X) [%d, %d, %d, %d]\n", 
            	u4AputBank, u4PrvAputBank, 
            	u4InputUnerRun, u4PrvInputUnerRun,
            	u4DspMayBeDead_st0,  u4NeedToRescueDsp, u4NeedToRecoverPlay, u4DspResueTrigger);
            
            //------------------------
            // check data input
            //------------------------
            // There are 3 cases to check if DSP "MAY BE DEAD":
            // 1. If u4InputUnerRun is 0, that means input may be not started. It may happens
            //     at the transient state. Thus, we need another longer threshold to avoid the false alarm.
            //     Once the case remains, detector think the DSP "MAY BE DEAD".
            // 2. If u4InputUnerRun != u4PrvInputUnerRun, that means the input buffer is still used.
            //     That is decoder is still "ALIVE" whether if demux is filling data or not.
            // 3. If u4InputUnerRun != u4PrvInputUnerRun, that means DSP "MAY BE DEAD". Detector
            //     must do further dectection.            
            if (u4InputUnerRun == 0)
            {                
                // for transient state
                if (u4Tick >= (DSP_RESUCE_THRESHOLD_1 / DSP_MONITOR_THREAD_PERIOD))
                {
                    u4DspMayBeDead_st0 = 1; //enter state 0
                }
                else
                {
                    u4DspMayBeDead_st0 = 0;
                    u4DspMayBeDead_st1 = 0;
                }
            }
            else if (u4InputUnerRun == u4PrvInputUnerRun)
            {
                u4DspMayBeDead_st0 = 1;
                u4Tick = 0;
            }
            else
            {
                // alive, reset info 
                u4Tick = 0;
            }  
            u4PrvInputUnerRun = u4InputUnerRun;

            //------------------------
            // check data output
            //------------------------
            // Once DSP "MAY BE DEAD", aput bank is checked. If it remains over the threshold,
            // detector think DSP is "DEAD".
            if (u4DspMayBeDead_st0)
            {
                u4Tick1 ++;
                u4AputBank = u4ReadD2RInfo_AputBank(AUD_DEC_MAIN);
                LOG(11,"u4AputBank = 0x%x, prev = 0x%x\n",u4AputBank,u4PrvAputBank);
                // DSP may be dead, check DSP output                
                if (u4AputBank == u4PrvAputBank)
                {                
                    if (u4Tick1 >= (DSP_RESUCE_THRESHOLD / DSP_MONITOR_THREAD_PERIOD))
                    {
                        // Goto state 1 for check readpointer
                        u4DspMayBeDead_st0 = 0;
                        u4DspMayBeDead_st1 = 1;
                        u4Tick1 = 0;
                        
                    }
                }
                else
                {
                    // alive, reset info                    
                    u4DspMayBeDead_st0 = 0;
                    u4DspMayBeDead_st1 = 0;                    
                    u4Tick1 = 0;
                }
            }
            u4PrvAputBank = u4AputBank;            

            //------------------------
            // check Read pointer
            //------------------------
            // 
            //check Read pointer as a last line of defense
            //
            if (u4DspMayBeDead_st1)
            {                
                u4Tick2 ++;            
                if ((_arAudDecoder[AUD_DEC_AUX].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)||
                    (_arAudDecoder[AUD_DEC_AUX].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER))
                {
                    u4ReadPtr = u4ReadD2RInfo_ReadPtr(AUD_DEC_AUX);
                }
                else
                {
                    u4ReadPtr = u4ReadD2RInfo_ReadPtr(AUD_DEC_MAIN);
                }
                
                LOG(11,"    u4ReadPtr = 0x%x, prev = 0x%x\n",u4ReadPtr,u4PrvReadPtr);
                // DSP may be dead, check DSP ReadPointer                
                if (u4ReadPtr == u4PrvReadPtr)
                {                
                    if (u4Tick2 >= (DSP_RESUCE_THRESHOLD / DSP_MONITOR_THREAD_PERIOD))
                    {
                        // DSP dead! Try ro rescure...
                        u4NeedToRescueDsp = 1;
                        u4NeedToRecoverPlay = 1; 
                        u4DspMayBeDead_st0 = 0;
                        u4DspMayBeDead_st1 = 0;
                        u4Tick2 = 0;
                    }
                }
                else
                {
                    // alive, reset info                    
                    u4DspMayBeDead_st0 = 0;
                    u4DspMayBeDead_st1 = 0;
                    u4Tick2 = 0;
                }
            }                        
            u4PrvReadPtr = u4ReadPtr;
        }
        else // decoder is not playing
        {
            u4DspMayBeDead_st0 = 0;
            u4DspMayBeDead_st1 = 0;
            u4Tick = 0;
            u4Tick1 = 0;           
            u4Tick2 = 0;
        }

        /* ====================================
            UOP Timeout Check
           ==================================== */
        if (u4DspResueTrigger)
        {
              LOG(0, "\n !!!!!!!!! UOP TIMEOUT !!!!!!!!!!!!!! \n");
              u4NeedToRescueDsp = 1;
              u4NeedToRecoverPlay = 0;
              u4DspResueTrigger = 0;
        }

        /* ====================================
            DSP Rescue Action
           ==================================== */
        if (u4NeedToRescueDsp)
        {
            LOG(0, "\n\n ###################==>DSP 0245789334 ! ##############\n\n");

            // critical section start for block MW's commands
            AUD_DSPCmdSema_Lock();
            u4ADSP_DspReset();  //Power off and Power on DSP
            vDSPClearDecType(); //clear all dsp code types to make sure loading code ok.

            // send RESET commmand to driver thread.
            for (i = (AUD_DEC_NUM-1); i >= 0; i--)
            {   
                u1DecId = (UINT8)i;
                LOG(0,">>>>>>>>>> Decoder %d <<<<<<<<<<<,,\n", u1DecId);
                VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_RESET));
                x_thread_delay(20);
            }
            
            // alive, reset info
            u4PrvAputBank = 0xFFFFFFFF;
            u4PrvInputUnerRun = 0xFFFFFFFF;
            u4PrvReadPtr = 0xFFFFFFFF;
            u4NeedToRescueDsp = 0;
            AUD_DSPCmdSema_UnLock();
        }

        
        x_thread_delay(DSP_MONITOR_THREAD_PERIOD);
    }
        #endif
    }

//-----------------------------------------------------------------------------
// _AudDrvResetDSP
//
/** Driver Force Reset flow, 
 *  reset related setting and decoder by _ChangeDecFmtOnPlayState
 *
 *  @param[in]          
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _AudDrvResetDSP(UINT8 u1DecId) //  -- DSP Force --
{    
    LOG(0," #  _AudDrv 1995 DSP[%d] = %d\n",u1DecId, _arAudDrvState[u1DecId] );

    AUD_DEC_ID_VALIDATE(u1DecId);
    
    if (_arAudDrvState[u1DecId] == AUD_ON_PLAY)
    {
        _AUD_IgnoreDecNotify(u1DecId, TRUE);
        
        // Reload code and stop/play dsp.
        _ChangeDecFmtOnPlayState(u1DecId);

        // Setup individual decoder setting by source <=== Don't do this, this reset system control and cause problem in dec3
        //_DecoderInitSetup(u1DecId, _arAudDecoder[u1DecId].eStreamFrom, _arAudDecoder[u1DecId].eDecFormat);
       
        x_thread_delay(50);
        
        if (u1DecId == AUD_DEC_MAIN)
        {
            // For MM files, need to triger play by self ...
            if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
            {
                AUD_DrvMMAoutEnable(u1DecId, TRUE);
            }

            // For non-line in source,turn off line-in.
            if ((_arAudDecoder[u1DecId].eStreamFrom != AUD_STREAM_FROM_HDMI) &&
                (_arAudDecoder[u1DecId].eStreamFrom != AUD_STREAM_FROM_LINE_IN))
            {
                AUD_LineInCtrlEnable(u1DecId,FALSE);
            }
        }   

        _AUD_IgnoreDecNotify(u1DecId, FALSE);        
    }        
}

    
