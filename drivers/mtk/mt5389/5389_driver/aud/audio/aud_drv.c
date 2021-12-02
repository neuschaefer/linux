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
 * $Author: kai.wang $
 * $Date: 2012/11/28 $
 * $RCSfile: aud_drv.c,v $
 * $Revision: #10 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_drv.c
 *  Brief of file aud_drv.c.
 *  Details of file aud_drv.c (optional).
 */

#define AUD_EXTERN_FILE

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
#include "mute_if.h"

#include "aud_drv.h"
#include "aud_if.h"
#include "spdif_drvif.h"
#include "aud_hw.h"
#include "aud_cfg.h"
#include "dsp_intf.h"
#include "aud_detect.h"
#include "aud_notify.h"
#include "adac_if.h"
#include "aud_pll.h"
#include "aud_dsp_cfg.h"

#include "../adsp/include_API/dsp_uop.h"
#include "../adsp/include_API/dsp_common.h"
#ifdef ENABLE_MULTIMEDIA
#include "swdmx_if.h"
#endif

#ifdef CC_ENABLE_AV_SYNC
#include "srm_drvif.h"
#include "vdo_misc.h"
#include "sv_const.h"
#include "mute_if.h"
#include "vdo_if.h"     //CC_AUD_QUERY_VDO_INFO
#include "video_timing.h"
#endif

#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
#include "pd_atd_if.h"
#endif

#include "x_aud_dec.h"
#include "dsp_shm.h" //FLASHLITE_CONFLICT

#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
#include "oss_adap.h"
#endif


#ifdef LINUX_TURNKEY_SOLUTION
#include "dsp_common.h"
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/param.h>
#endif // __KERNEL__

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
#ifdef CC_AUD_4_DECODER_SUPPORT
#define ADRV_CMD_Q_NAME4        "AUD CMDQ4"
#endif

#define ADRV_CMD_Q_DUAL_DEC     "AUD_DUAL_DEC_CMD_Q"
#define ADRV_APLL_CMD_Q         "AUD_APLL_CMD_Q"
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
#define PTS_THRESHOLD_MAX       ((UINT32)(PTS_AC3_CNT*120))
#define PTS_THRESHOLD_NOT_VALID       ((UINT32)(PTS_AC3_CNT*2000))
#define PTS_DRIFT_MAX           ((UINT32)12000)

//#define PTS_DRIFT_MAX           12000

#define TRANSFER_SZ             (UINT32)(0x00000400)

#define AUD_EVENT_TIMEOUT       (UINT32)(1500*0.75)
// Light: Since DTV needs to wait for 1st I frame, it takes longer time to wait for the event.
#define AUD_EVENT_TIMEOUT_DTV    (UINT32)(4500)//(UINT32)(1600)
#define AUD_EVENT_TIMEOUT_DTV_AD    (UINT32)(500)   //AD check if data available timeout
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

// Feeder relative setting
#define FEEDER_REQUEST_DATA_SIZE  (16*1024)

// Waiting for analog tuner lock signal ..
//2007/12/27 3000->1000, because it takes too long and would block dec0 starting play when there is no ATV source
//           under source HDMI and AV.
#define ATUNER_WAITING_TIME_OUT 500//from 1000 -> 500, to prevent change channel to decoded ATV channel will take a long time to wait semiphone un-lock
#ifdef CC_ENABLE_AV_SYNC
#define VDP_WAITING_TIME_OUT 1500
#define AUD_MUTE_WITH_VDO_TIMEOUT 4000 //7000ms
#define AUD_MUTE_WITH_AV_VDO_TIMEOUT 4000 //4000ms
#define AUD_PIP_MUTE_WITH_VDO_TIMEOUT 100 //100ms
#define AUD_EXTRA_MUTE_AFTER_VDP            200 //200ms
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
#define MM_AAC_CHECK_PROFILE_THREASHOLD DTV_LOCK_CHECK_THREASHOLD + (1000/AUD_PTR_UPDATE_TIMEOUT)
#define DTV_DECODE_ERR_CHECK_THREASHOLD (5000/AUD_PTR_UPDATE_TIMEOUT)   //5s, same as video
#define MM_AC3_CHECK_PROFILE_THREASHOLD DTV_LOCK_CHECK_THREASHOLD

#define MM_SYNC_INFO_NUM 1024
#define MM_TICK_INFO_NUM 100

#define MPEG_FRAME_SIZE 0x870 //1152 sample for 48 kHZ, in 90K Hz Clock.

//  -- DSP Force --
#define DSP_FORCE_RESET_ENABLE
#define DSP_MONITOR_THREAD_PERIOD       100
#define DSP_RESUCE_THRESHOLD            (3 * 1000)
#define DSP_RESUCE_THRESHOLD_1          (5 * 1000)
#define DSP_MONITOR_THREAD_START_TIME   (10 * 1000)
//

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

extern UINT8 _guAudDemodTypeChange;
extern UINT8 _guAudDspTrigAtvFmtchg;
extern AUD_FMT_T _eAtvFmtChgDecFmt;

#ifdef ENABLE_MULTIMEDIA
extern UINT32 u4AudDspPts[];
#endif

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
extern void _AUD_DspSetIEC(UINT8 u1DecId);
extern AUD_IEC_T _GetIECFlag(UINT8 u1DecId);
#ifndef CC_AUD_SUPPORT_MS10
extern void _AUD_DspSetDDCOEnable(BOOL fgEnable);
#endif
#ifdef FIX_AUD_UOP_TIMEOUT
extern void AUD_WaitUOPLockSema(UINT8 u1DecId);
#endif
extern void AUD_WaitUopCommandDone(UINT8 u1DecId);
extern void vDspSetFifoReadPtr(UINT8 u1DecId, UINT32 u4ReadPtr);
extern void vDspSetSyncMode(UINT8 u1DecId, AV_SYNC_MODE_T eMode);
extern void vAudOutReInit(UINT8 ucDecId, BOOL fgEnable);
extern BOOL fgDspReadPtrSet (UCHAR ucDecId, UINT32 u4Address);
//extern BOOL fgApiVideoStable(UINT8 bVideoPath);
//Replace by _u4DrvVideoGetMute
//extern BOOL fgApiVideoStable(UINT8 u1VideoFrame); // 0: main frame, 1: sub frame for PIP/POP

extern void AUD_SetSrc(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt);
extern void AUD_DspDvdEnable(void);
extern BOOL _DMX_GetInterruptThreshold(DMX_PID_TYPE_T ePidType, UINT32* pu4Bytes);
extern BOOL _DMX_SetInterruptThreshold(DMX_PID_TYPE_T ePidType, UINT32 u4Bytes);
extern UINT8 u1HDMIGetAudioSamplingFreq(void);
extern void bHDMIClockDetect(void);

void AUD_ClearFifo(UCHAR ucDecId);
void AUD_GetAudioPes(UINT8 u1DecId, PSR_AUDIO_PES_T * prAudioPes);
void AUD_FixWarning(void);
BOOL AUD_UpdateAudioFifo(UINT8 u1DecId, UINT32 u4SartAddress, UINT32 u4Size, UINT32 u4WritePtr);
void AUD_SetStcThreshold(UINT8 u1DecId, UINT16 u2Threshold);
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
BOOL AUD_DrvCheckHDMISize(AUD_FIFO_INFO_T arfifo,UINT32 u4CheckSize);
BOOL AUD_DrvCheckIECSyncWord(AUD_HDMI_PARSER_INFO_T * prParserInfo);
UINT16 AUD_DrvSwapWord(UINT16 u2swap);
UINT32 AUD_DrvGetCircularAddress(UINT32 u4StartAddress,UINT32 u4EndAddress,UINT32 u4Address);
#endif

extern UINT8 bHDMIAudStatus(void);
extern void ADAC_HdmiUnstableSpkEnable(BOOL fgFlag);
extern UINT32 AUD_GetDecFrameCnt(UINT8 u1DecId);
extern BOOL AUD_IsPcmDecMute(UINT8 u1DecId);

extern INT32 IS_MultiMediaSupport(void);
extern void AUD_ClearPtsInISR(UINT8 u1DecId);

#ifdef CC_ENABLE_AV_SYNC
//extern void vDrvSetAudioTypeChangeNotify(UINT8 notifyValue);
extern void AUD_UnMuteWithVideo(void);
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
extern BOOL AUD_DrvDspCallBackProcess(UINT8 u1DecId, AUD_DEC_CMD_T eCmd);
extern void AUD_ClkSetToInternal(UINT8 u1DecId, AUD_FMT_T eFmt);
extern UINT32 u4ReadD2RInfo_StcDiff(UINT8 u1DecId);
extern BOOL DSP_GetAoutEnable(UINT8 u1DecId);
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
static void _AudDtvOnPlayThread(void* pvArg);
static void _DtvResetLockStatus(UINT8 u1DecId);
inline void _AudDrvThread_WaitResume(UINT8 u1DecId);
#ifdef DSP_FORCE_RESET_ENABLE
static void _AudDspMonitorThread(void* pvArg);  //  -- DSP Force --
#endif
static void _AudDrvResetDSP(UINT8 u1DecId);     //  -- DSP Force --
#ifdef CC_AUD_NCSTOOL_SUPPORT
extern void _AudToolThread(void* pvArg);
#endif
#ifdef CC_ENABLE_AV_SYNC
static void _AudAvSyncThread(void* pvArg);
#endif
#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
static TV_AUD_SYS_T _AudGetTvSysFormat(MW_TV_AUD_SYS_T rAudTvSysFromMW);
static UINT8 _AudTvSys2Tuner(UINT32 u4TvSysMask);
#endif
//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
static void _AudDataUploadThread(void* pvArg);
static void _AudMuxerInit(void);
//#endif

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
static void _AudHDMIParserThread(void* pvArg);
#endif

#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
static void _AudDataTransferThread(void* pvArg);
#endif

#ifdef CC_AUD_SKYPE_ZERO_DELAY
bool _fgSkypeMode = FALSE;
extern BOOL FeederGetSkypeSourceFlag(void);
#endif

extern BOOL _fgAudMMnotifySWDMX[AUD_DEC_MAX];

//---------------------------------------------------------------------------
// Global variables
//---------------------------------------------------------------------------
UINT32 _gu4AudCtrlType;
UINT32 _gu1AoutDramErr;

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static BOOL _fgAoutTimeoutEnable = FALSE;

static UINT32 _u4FeedStreamResult;
static INT32  _i4MemStrLoopCnt;
static BOOL   _fgInfinite = FALSE;
static BOOL   _fgForceStopStrmDma = FALSE;
static BOOL _fgMMAudioOnly = FALSE;
#ifdef CC_ENABLE_AV_SYNC
static BOOL _fgAdMode = FALSE;
#endif

// Thread hanlder
static HANDLE_T _hAudDrvThread[AUD_DEC_MAX] = {NULL_HANDLE, NULL_HANDLE, NULL_HANDLE, NULL_HANDLE};;
static HANDLE_T _hAudApllMonThread;
static HANDLE_T _hAudFeedStrThread = NULL_HANDLE;
static HANDLE_T _hAudFeederThread = NULL_HANDLE;
#ifdef DSP_FORCE_RESET_ENABLE
static HANDLE_T _hAudDspMonThread; //  -- DSP Force --
#endif
static HANDLE_T _hAudDtvThread[AUD_DEC_MAX] = {NULL_HANDLE, NULL_HANDLE, NULL_HANDLE, NULL_HANDLE};
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
static HANDLE_T _hAudHDMIParserThread = NULL_HANDLE;
#endif
#ifdef CC_AUD_NCSTOOL_SUPPORT
static HANDLE_T _hAudToolThread = NULL_HANDLE;
extern void vAllocateSWReg(void);
#endif

#ifdef CC_ENABLE_AV_SYNC
static HANDLE_T _hAudAVSyncThread = NULL_HANDLE;
#endif
//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
static HANDLE_T _hAudDataUploadThread = NULL_HANDLE;
static HANDLE_T _hSemaDataUploadWait;
//#endif
#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
static HANDLE_T _hAudDataTransferThread = NULL_HANDLE;
static HANDLE_T _hSemaDataTransferWait;
static HANDLE_T _hSemaDataTransferWait1;

#endif

// Semaphore and message queue
static HANDLE_T _hSemaLoadCode;
static HANDLE_T _ahSemaUopComDone[AUD_DEC_MAX];
static HANDLE_T _hDetectionSyncSema;
static HANDLE_T _hFeedStreamSyncSema;
static HANDLE_T _ahAudCmdQueue[AUD_DEC_MAX];
static HANDLE_T _hAudApllCmdQueue;

static DMX_AUDIO_PES_T _arAudPes[AUD_DEC_MAX];

static HANDLE_T _hAtunerSyncSema;
#ifdef CC_ENABLE_AV_SYNC
static HANDLE_T _hVdpSyncSema;
static AUD_SRM_VDO_DELAY_TBL *rAudSrmVdoDelayTblDtv = NULL; //CC_AUD_QUERY_VDO_INFO
static AUD_SRM_VDO_DELAY_TBL *rAudSrmVdoDelayTblHdmi = NULL;
static BOOL fgAudVdoDelayTblInit = FALSE;
#endif

static HANDLE_T _arAudDrvStateSema[AUD_DEC_MAX];

static BOOL      _afgDtvOnPlayThreadGo[AUD_DEC_MAX] = {FALSE, FALSE, FALSE, FALSE};

static AUD_DECODER_T _arAudDecoder[AUD_DEC_MAX];
#ifdef CC_ENABLE_AV_SYNC
static BOOL  _fgVdpReady = FALSE;
static BOOL  _fgIgnoreVdpInfo;
static BOOL  _fgVdpChangeState = FALSE;
UINT16      _u6VideoSetCHdelay=0;
BOOL        _fgwaitForAoutReinit=FALSE;
BOOL        _fgSetChannelDelay=FALSE;
BOOL        _fgwaitForPIPAoutReinit=FALSE;
UINT8        _GetVideoCBCount=0;
UINT16      _u2AudDrvDtvUNMuteCnt = 0;
UINT16      _u2AudDrvExtraMuteCnt = 0;
//UINT16     _u2AudDrvDtvPIPUNMuteCnt = 0;
static BOOL  _fgAVSyncThreadGo = FALSE;
static BOOL  _fgAVSyncThreadMute = FALSE;
static BOOL  _fgAudDrvExtraMute = FALSE;
#endif
static BOOL _fgDualDecModeDrv = FALSE;

static BOOL _fgAudDrvThreadInit = FALSE;

static BOOL _afgFirstIPicArrived[AUD_DEC_MAX] = {FALSE, FALSE, FALSE, FALSE};
static BOOL _afgIssuePlayComToDsp[AUD_DEC_MAX] = {FALSE, FALSE, FALSE, FALSE};
static BOOL _afgAtvVideoDisplayFlag[AUD_DEC_MAX] = {FALSE, FALSE, FALSE, FALSE};

//static BOOL _afgUseModifiedPts[2] = {FALSE, FALSE};
static UINT8 _uAudioDelayPTSFlag[AUD_DEC_MAX] = {0, 0, 0, 0};

//static UINT16 _uAudioEventTimeout[2] = {AUD_EVENT_TIMEOUT, AUD_EVENT_TIMEOUT};
// Video PIP/POP displayed frame indicator, 0: display main frame, 1: display sub frame.
static UINT8 _u1VideoFrameFlag = 0;
static BOOL _fgDDBannerOn = FALSE;

// Dual decoder variables
static AUD_DRV_STATE_T _arAudDrvState[AUD_DEC_MAX] = {AUD_UNINITIALIZED, AUD_UNINITIALIZED, AUD_UNINITIALIZED, AUD_UNINITIALIZED};

static PARAM_MPV_T* _prMPV;
static DRV_DBASE_T* prDbase;

// For memory clip
static AUD_CLIP_NOTIFY _hClipNotifyFunc = NULL;

// Feeder varaibles
static FEEDER_TRANSMIT _rFeedTransmitInfo;
//static BOOL _fgDtvLock = TRUE;
static BOOL _afgDtvLock[AUD_DEC_MAX] = {FALSE, FALSE, FALSE, FALSE};
static BOOL _afgDtvPesLock[AUD_DEC_MAX] = {FALSE, FALSE, FALSE, FALSE};

// HDMI relative
static BOOL  _fgHdmiMute = FALSE;
static UINT8 _u1HDMIMuteCnt = 0;
static UINT8 _u1HDMIUnDetectCnt = 0;

// DTV lock status
static UINT32 _au4LastPesCnt[AUD_DEC_MAX] = {0, 0, 0, 0};
static UINT32 _au4FrameCnt[AUD_DEC_MAX] = {0, 0, 0, 0};
static UINT32 _au4LastFrameCnt[AUD_DEC_MAX] = {0, 0, 0, 0};
static UINT32 _au4PesCnt[AUD_DEC_MAX] = {0, 0, 0, 0};
static UINT8  _au1NoFrameCntIncrease[AUD_DEC_MAX] = {0, 0, 0, 0};
static UINT8  _au1NoPesCntIncrease[AUD_DEC_MAX] = {0, 0, 0, 0};
static AUD_DECODE_STATUS_T  _aeDecStatus[AUD_DEC_MAX];
static BOOL   _fgMMSet[AUD_DEC_MAX] = {FALSE,FALSE,FALSE,FALSE};    //MM EOS relative
static AUD_DECODE_STATUS_T  _aeHDMIDecStatus[AUD_DEC_MAX];
static BOOL _fgWaitAvSync[AUD_DEC_MAX] = {FALSE,FALSE,FALSE,FALSE};
static BOOL _fgAudOutReady[AUD_DEC_MAX] = {FALSE,FALSE,FALSE,FALSE};

// MM : MultiMedia Releated.
static AUD_MM_SYNC_INFO_T _aeMMSyncInfo[MM_SYNC_INFO_NUM];
//static AUD_MM_SYNC_INFO_T* _paeMMSyncInfo;
static UINT16 u2CurrIdx = 0;
static UINT16 u2LatestIdx = 0;
static BOOL fgLoopBack = FALSE;
static AUD_MM_TICK_INFO_T _aeMMTickInfo[MM_TICK_INFO_NUM];
static UINT16 u2TickCurrIdx = 0;
static UINT32 u4OldPts = 0;
static BOOL _bPanFadeUpdateEn = TRUE;
static BOOL _bPanEnable = TRUE;
static BOOL _bFadeEnable = TRUE;

static CHAR * _aszCodeType[] =
{
    "UNKNOWN",      // 0
    "MPEG",
    "AC3",
    "PCM",
    "MP3",
    "AAC",          // 5
    "DTS",
    "WMA",
    "RA",
    "HDCD",
    "MLP",          // 10
    "MTS",
    "CANAL+",
    "PAL",
    "A2",
    "FMFM",         // 15
    "NICAM",
    "TTXAAC",
    "DETECTOR",
    "MINER",
    "LPCM",         // 20
    "FM_RADIO",
    "FM_RADIO_DET",
    "SBCDEC",
    "SBCENC",
    "MP3ENC",       // 25, MP3ENC_SUPPORT
    "G711DEC",
    "G711ENC",
    "DRA",
    "COOK",
    "G729DEC",      // 30
    "VORBIS"        // 31, CC_VORBIS_SUPPORT
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
    "AUD_CMD_STOPSTREAM",
    "AUD_CMD_UNMUTE"
};

static DRV_DBASE_T* _prDbase = NULL;
static UINT32 _au4TvSysMask[2] = {0, 0}; //2008/01/16
#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
static UINT32 _au4TvAudSysMask[2] = {0, 0}; //2008/01/16
static ISO_3166_COUNT_T _au4TvSysCountry =  "";
#endif
#ifdef DSP_FORCE_RESET_ENABLE
static UINT32 u4DspResueTrigger = 0;     //  -- DSP Force --
#endif
static BOOL _fgAudDrvForceReset = FALSE; //  -- DSP Force --

static UINT8 _u1SwdmxSrcId[AUD_DEC_NUM] = {0};

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
static AUD_HDMI_PARSER_INFO_T _arParserInfo[AUD_DEC_NUM];
static UINT16 _au2Pc[AUD_RAW_NUM]={0x1,0x4,0x5,0x6,0x7,0x8,0x9,0xb,0xc,0xd,0x11,0x15,0x16};
#endif

static BOOL   _fgFeedStrmFinshed = TRUE;    // Feed_and_Auddrv Thread sync

//		-- 	DUMP AFIFO		--	//
#ifdef LINUX_TURNKEY_SOLUTION
extern int StartAudDataDump(UINT32 u1DecId, char* fileName);
extern void StopAudDataDump(UINT32 u1DecId);
extern void AudDataDumpQueryInfo(void);
extern BOOL GetAudDataDumpAutoSave(UINT32 u1DecId);
extern void SetAudDataDumpAutoSave(UINT32 u1DecId, BOOL bAutoSave);
static void AudDataDumpNextFile(UINT32 u1DecId);

#define MAX_PATH 256

static const char *AudDataDumpThreadName[AUD_DEC_NUM] = {"_AudDataDumpThread 0", "_AudDataDumpThread 1", 
														 "_AudDataDumpThread 2", "_AudDataDumpThread 3"};
static HANDLE_T _hAudDataArrived[AUD_DEC_NUM] = {NULL_HANDLE, NULL_HANDLE, NULL_HANDLE, NULL_HANDLE};
static HANDLE_T _hAudDataDumpThread[AUD_DEC_NUM] = {NULL_HANDLE, NULL_HANDLE, NULL_HANDLE, NULL_HANDLE};
static HANDLE_T _hAudDataDumpThreadState[AUD_DEC_NUM] = {-1, -1, -1, -1};
static BOOL _fgForceStopAudData[AUD_DEC_NUM] = {FALSE, FALSE, FALSE, FALSE};
struct file *_pAudDumpFile[AUD_DEC_NUM] = {NULL, NULL, NULL, NULL};
static UINT32 u4AudDataCurRp[AUD_DEC_NUM] = {NULL, NULL, NULL, NULL};
static BOOL _fgAudDataDumpAutoSave[AUD_DEC_NUM] = { FALSE, FALSE, FALSE, FALSE };
static BOOL _fgAudDataDumpNextFile[AUD_DEC_NUM] = { FALSE, FALSE, FALSE, FALSE };
static char _szAudDataDumpFileName[AUD_DEC_NUM][MAX_PATH] = { "\0", "\0", "\0", "\0" };

void write_to_file(struct file *filep, const char *addr, UINT32 size) 
{
	mm_segment_t oldfs;
	oldfs = get_fs();	
	set_fs(KERNEL_DS);
	
	filep->f_op->write(filep, (const char *)addr, (size_t)size, &filep->f_pos);
	
	set_fs(oldfs);				
}

static void _AudDataDumpThread(void* pvArg) 
{
	UINT32 u1DecId = AUD_DEC_MAIN;
	UINT32 u4Rp;
    UINT32 u4Wp;
    UINT32 u4DataSZ;
	UINT32 u4FifoSA;
	UINT32 u4FifoEA;

	UINT32 runCount = 0;
	UINT32 dataSize = 0;

	if (pvArg != NULL)
    {
        u1DecId = *(UINT32 *)pvArg;
    }

    _fgAudDataDumpNextFile[u1DecId] = FALSE;
	_hAudDataDumpThreadState[u1DecId] = 1;
	LOG(0, "_AudDataDumpThread %d is created !\n", u1DecId);
	
	UNUSED(AUD_GetAudFifo(u1DecId, &u4FifoSA, &u4FifoEA));
	u4FifoSA = VIRTUAL(u4FifoSA);
	u4FifoEA = VIRTUAL(u4FifoEA);
	u4AudDataCurRp[u1DecId] = u4FifoSA;
	
	while (1) 
	{

		if (_fgForceStopAudData[u1DecId]) 
		{
			break;
		}
		
		VERIFY(x_sema_lock(_hAudDataArrived[u1DecId], X_SEMA_OPTION_WAIT) == OSR_OK);

        if ((_fgAudDataDumpAutoSave[u1DecId]) && (_fgAudDataDumpNextFile[u1DecId]) && AUD_IsDecoderPlay(u1DecId)) 
        {
             AudDataDumpNextFile(u1DecId);
             runCount = 0;
             dataSize = 0;
             LOG(5, "_AudDataDumpThread %d opens a new file \"%s\" to dump.\n", u1DecId, _szAudDataDumpFileName[u1DecId]);
        }
        _fgAudDataDumpNextFile[u1DecId] = FALSE;
        
		AUD_GetRWPtr(u1DecId, &u4Rp, &u4Wp, &u4DataSZ);
		u4Wp = VIRTUAL(u4Wp);
		u4Rp = u4AudDataCurRp[u1DecId];
		
		if (_pAudDumpFile[u1DecId] && AUD_IsDecoderPlay(u1DecId))  
		{
            if (u4Wp >= u4FifoEA)
            {
                LOG(0, "_AudDataDumpThread %d, wrong WP(0x%x)!!\n", u1DecId, u4Wp);
                u4Wp = u4FifoSA;
            }
            
			if(u4Wp < u4Rp)
			{
				write_to_file(_pAudDumpFile[u1DecId], (const char *) u4Rp, u4FifoEA - u4Rp);
				write_to_file(_pAudDumpFile[u1DecId], (const char *) u4FifoSA, u4Wp - u4FifoSA);
				dataSize += u4FifoEA - u4Rp;
				dataSize += u4Wp - u4FifoSA;
			} 
			else 
			{
				write_to_file(_pAudDumpFile[u1DecId], (const char *) u4Rp, u4Wp - u4Rp);
				dataSize += u4Wp - u4Rp;
			}
			u4AudDataCurRp[u1DecId] = u4Wp;
		}

        runCount ++;
		LOG(5, "_AudDataDumpThread %d is active , runCount = %d , dataSize = 0x%x \n", u1DecId, runCount, dataSize);
	}

	LOG(0, "_AudDataDumpThread %d exited !\n", u1DecId);
	_hAudDataDumpThreadState[u1DecId] = -1;
}

int StartAudDataDump(UINT32 u1DecId, char* fileName) 
{
	mm_segment_t oldfs;
	oldfs = get_fs();	
	set_fs(KERNEL_DS);

	if (u1DecId >= AUD_DEC_NUM) 
	{
		LOG(0, "Invalid decode ID : %d \n", u1DecId);
		goto fail_invalid_arg;
	}
	
	if (_hAudDataDumpThreadState[u1DecId] != -1) 
	{
		LOG(0, "%s already running .\n", AudDataDumpThreadName[u1DecId]);		
		goto fail_already_running;
	}
	
	_pAudDumpFile[u1DecId] = filp_open(fileName, (O_CREAT | O_WRONLY ), 0);
	if((IS_ERR(_pAudDumpFile[u1DecId])) || (NULL == _pAudDumpFile[u1DecId]))
	{
		LOG(0, "_AudCmdDump() create file failed.\n");
		goto fail_create_file;
	}

	if(OSR_OK != x_thread_create(&_hAudDataDumpThread[u1DecId], AudDataDumpThreadName[u1DecId], AUD_DRV_THREAD_STACK_SIZE, 
									AUD_DRV_THREAD_PRIORITY, _AudDataDumpThread, sizeof(UINT32), (void *)&u1DecId)) 
	{
		LOG(0, "Failed to start %s .\n", AudDataDumpThreadName[u1DecId]);
		goto fail_start_thread;
	}

    x_strcpy(_szAudDataDumpFileName[u1DecId], fileName);
	LOG(0, "Dumping audio fifo %d to \"%s\" ...\n", u1DecId, fileName);
	set_fs(oldfs);
	return 0;
	
	fail_start_thread:
		filp_close(_pAudDumpFile[u1DecId], 0);		
	fail_create_file:
		_pAudDumpFile[u1DecId] = NULL;
	fail_already_running:
	fail_invalid_arg:
		set_fs(oldfs);
		return -1;
}

void StopAudDataDump(UINT32 u1DecId)
{
	UINT32 count = 0;
	mm_segment_t oldfs;

	if (u1DecId >= AUD_DEC_NUM) 
	{
		LOG(0, "Invalid decode ID : %d \n", u1DecId);
		return;
	}

	_fgForceStopAudData[u1DecId] = TRUE;
	VERIFY(x_sema_unlock(_hAudDataArrived[u1DecId]) == OSR_OK);

	while (_hAudDataDumpThreadState[u1DecId] != -1) 
	{
		x_thread_delay(5);
		count ++;
		if (count >= 5) {
			LOG(0, "Stop _AudDataDumpThread %d fail !\n", u1DecId);
		}
	}
	
	_fgForceStopAudData[u1DecId] = FALSE;
	if (_pAudDumpFile[u1DecId]) 
	{
		oldfs = get_fs();
        set_fs(KERNEL_DS);
		filp_close(_pAudDumpFile[u1DecId], 0);		
		_pAudDumpFile[u1DecId] = NULL;
		set_fs(oldfs);
	}

	LOG(0, "Stop data dump %d OK !\n", u1DecId);
}

void AudDataDumpQueryInfo(void)
{
    UINT8 i;

    for (i = 0; i < AUD_DEC_NUM; i++)
    {
        LOG(0, "#%s: %s\n", AudDataDumpThreadName[i], (_hAudDataDumpThreadState[i] == 1) ? "running" : "exited");
        if ((_hAudDataDumpThreadState[i] == 1) && (_pAudDumpFile != NULL))
        {
            LOG(0, "  Dumping to file: %s\n\n", _szAudDataDumpFileName[i]);
        }
        else
        {
            LOG(0, "\n");
        }
    }
}

BOOL GetAudDataDumpAutoSave(UINT32 u1DecId)
{
    if (u1DecId >= AUD_DEC_NUM)
    {
        LOG(0, "Invalid decode ID : %d \n", u1DecId);
        return FALSE;
    }

    return _fgAudDataDumpAutoSave[u1DecId];
}

void SetAudDataDumpAutoSave(UINT32 u1DecId,BOOL bAutoSave)
{
    if (u1DecId >= AUD_DEC_NUM)
    {
        LOG(0, "Invalid decode ID : %d \n", u1DecId);
        return;
    }

    _fgAudDataDumpAutoSave[u1DecId] = bAutoSave;
}

void AudDataDumpNextFile(UINT32 u1DecId)
{
    mm_segment_t oldfs;
    INT32 i;
    UINT32 len, iR = 0, iDot, id;
    char *fileName, szTmp[MAX_PATH];
    char szID[16];

    fileName = _szAudDataDumpFileName[u1DecId];
    len = x_strlen(fileName);
    iDot = len;
    for (i = len-1; i >= 0; i--)
    {
        switch (fileName[i])
        {
            case '(':
                if (iR > i)
                {
                    if ((iR == i+1) || (iR - i) > sizeof(szID))
                    {
                        id = 1;
                    }
                    else
                    {
                        x_strncpy(szID, fileName+i+1, iR - i - 1);
                        szID[iR - i - 1] = '\0';
                        id = (UINT32)StrToInt(szID);
                        id++;
                    }
                    SPrintf(szID, "%d", id);
                    x_strcpy(szTmp, fileName+iR);
                    x_strcpy(fileName+i+1, szID);
                    x_strcpy(fileName+i+1+x_strlen(szID), szTmp);
                    
                    goto get_file_name_end;
                }
                break;
            case ')':
                if ((iDot < len) || (i == len-1))
                    iR = i;
                break;
            case '/':
                goto for_end;
            case '.':
                if (iDot == len)
                {
                    iDot = i;
                    if ((i == 0) || (fileName[i-1] != ')'))
                    {
                        goto for_end;
                    }
                }
                break;
        }
    }
for_end:
    if (iDot < len)
    {
        x_strcpy(szID, "(1)");
        x_strcpy(szTmp, fileName+iDot);
        x_strcpy(fileName+iDot, szID);
        x_strcpy(fileName+iDot+x_strlen(szID), szTmp);
    }
    else
    {
        x_strcpy(szID, "(1)");
        x_strcpy(fileName+len, szID); 
    }

get_file_name_end:
    oldfs = get_fs();
    set_fs(KERNEL_DS);
    
    if (_pAudDumpFile[u1DecId])
    {
		filp_close(_pAudDumpFile[u1DecId], 0);		
    }
    
    _pAudDumpFile[u1DecId] = filp_open(fileName, (O_CREAT | O_WRONLY ), 0);
	if((IS_ERR(_pAudDumpFile[u1DecId])) || (NULL == _pAudDumpFile[u1DecId]))
	{
		LOG(0, "AudDataDumpNextFile() create file failed.\n");
        _pAudDumpFile[u1DecId] = NULL;
	}
    
    set_fs(oldfs);
}
#endif

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
        if (_GetVideoCBCount ==1 && (_fgMMAudioOnly == FALSE))  //only set channel delay when _GetVideoCBCount=1
        {
            LOG(1," [SRM_AVSYNC]######### _GetVideoCBCount=1, can set channel dealy #########\n ", u2DelayMs);

			// Move this to AUD_DrvSetChDelay
#if 0//def CC_AUD_SKYPE_ZERO_DELAY
            _fgSkypeMode = FeederGetSkypeSourceFlag();
            if ((_arAudDecoder[AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)&&(_fgSkypeMode))
            {
                LOG(1," [SRM_AVSYNC]######### Set zero channel dealy for Skype#########\n ");
                AUD_DrvSetChDelay(0);
            }
            else
            {
            AUD_DrvSetChDelay(u2DelayMs);
        }
#else
            AUD_DrvSetChDelay(u2DelayMs);
#endif
        }
        _fgVdpChangeState = FALSE;
    }
    else  //UNStable Callback : means SRM mode change.
    {
        LOG(7, "[SRM_AVSYNC] @@@ VDP Change State......\n");
        _fgVdpChangeState = TRUE;
    #if 1 //move from WAIT_STOP to here, since this callback may come before or after WAIT_STOP
    //    _fgVdpReady = FALSE;
        _fgIgnoreVdpInfo = FALSE;
        _fgwaitForAoutReinit=FALSE;
        _GetVideoCBCount=0;
        x_sema_lock(_hVdpSyncSema, X_SEMA_OPTION_NOWAIT); //make sure this is locked for later use.
    #endif
    }

}

#ifdef CC_AUD_QUERY_VDO_INFO
static INT16 _AudSrmGetVdoInfo()
{
    VDO_INPUT_INFO rVdoInfo;
    AUD_SRM_VDO_HEIGHT_T eAudSrmVdoH;
    AUD_SRM_VDO_IP_T eAudSrmVdoIP;
    AUD_SRM_VDO_RATE_T eAudSrmVdoRate;
    INT16 i2Delay = 0;

    // 1. Get Video info
    DrvVideoGetInputTimingInfo(SV_VP_MAIN, &rVdoInfo);
    LOG(3,"Video : H=%d, W=%d, T=%d, I=%d, R=%d, P=%d\n",
        rVdoInfo.u2Height, rVdoInfo.u2Width, rVdoInfo.u1Timing,
        rVdoInfo.u1Interlace,rVdoInfo.u1RefreshRate, rVdoInfo.u1PanelDelay);
    // 2. Transform to audio emu types.
    switch (rVdoInfo.u2Height)
    {
        case 480:
            eAudSrmVdoH = AUD_SRM_VDO_480;
            break;
        case 576:
            eAudSrmVdoH = AUD_SRM_VDO_576;
            break;
        case 720:
            eAudSrmVdoH = AUD_SRM_VDO_720;
            break;
        case 1080:
            eAudSrmVdoH = AUD_SRM_VDO_1080;
            break;
        default:
            eAudSrmVdoH = AUD_SRM_VDO_480;
            break;
    }

    if (rVdoInfo.u1Interlace == 0)
    {
        eAudSrmVdoIP = AUD_SRM_VDO_P;
    }
    else
    {
        eAudSrmVdoIP = AUD_SRM_VDO_I;
    }

    if (rVdoInfo.u1RefreshRate == 50)
    {
        eAudSrmVdoRate = AUD_SRM_VDO_50HZ;
    }
    else
    {
        eAudSrmVdoRate = AUD_SRM_VDO_60HZ;
    }

    // Check Table
    if (!fgAudVdoDelayTblInit)
    {
        if ((DRVCUST_OptQuery(eAudioVdoDelayTableDtv, (UINT32*)&rAudSrmVdoDelayTblDtv) == 0) &&
             (DRVCUST_OptQuery(eAudioVdoDelayTableHdmi, (UINT32*)&rAudSrmVdoDelayTblHdmi) == 0) )
        {
            fgAudVdoDelayTblInit = TRUE;
        }
    }
    if (_AudGetStrSource(AUD_DEC_MAIN) == AUD_STREAM_FROM_DIGITAL_TUNER)
    {
        if (rAudSrmVdoDelayTblDtv != 0)
        {
            i2Delay = rAudSrmVdoDelayTblDtv->i2Delay[eAudSrmVdoH][eAudSrmVdoIP][eAudSrmVdoRate];
        }
    }
    else if (_AudGetStrSource(AUD_DEC_MAIN) == AUD_STREAM_FROM_HDMI)
    {
        if (rAudSrmVdoDelayTblHdmi != 0)
        {
            i2Delay = rAudSrmVdoDelayTblHdmi->i2Delay[eAudSrmVdoH][eAudSrmVdoIP][eAudSrmVdoRate];
        }
    }

    return i2Delay;
}
#endif //CC_AUD_QUERY_VDO_INFO

void _AudSrmAdjustDelayByAudFmt(UINT16* u2Delay)
{
    AUD_DEC_STREAM_FROM_T eAudDecStreamFrom;
    AUD_FMT_T              eAudDecFormat;
    UINT8                      u1Version;
    INT16 i2CustDelayMs = 0;
    UINT8 u1CustValueType = 0;

    UNUSED(u2Delay);
    UNUSED(eAudDecFormat);
    UNUSED(eAudDecStreamFrom);
    UNUSED(i2CustDelayMs);
    UNUSED(u1CustValueType);
    UNUSED(u1Version);
    UNUSED(rAudSrmVdoDelayTblDtv);  //CC_AUD_QUERY_VDO_INFO
    UNUSED(rAudSrmVdoDelayTblHdmi);
    UNUSED(fgAudVdoDelayTblInit);


    //===== 1. Adjust according to Audio Source and Format.
    if (_AudGetStrSource(AUD_DEC_MAIN) == AUD_STREAM_FROM_DIGITAL_TUNER)
    {
	    if(AUD_GetTargetCountry() == COUNTRY_EU)
	    {
            AUD_DRVGetDecodeType(AUD_DEC_AUX, &eAudDecStreamFrom, &eAudDecFormat);
#if 0 //Remove this adjustment since it's just based on a test in MT5362B
            if (eAudDecFormat==AUD_FMT_MPEG)
            {
                *u2Delay=_u6VideoSetCHdelay+50;
                LOG(3,"@@@@@ It is MPEG @@@@@@\n");
            }
            else if(eAudDecFormat==AUD_FMT_AC3)
            {
                *u2Delay=_u6VideoSetCHdelay+60;
                LOG(3,"@@@@@ It is AC3 @@@@@@\n");
            }
            else if(eAudDecFormat==AUD_FMT_AAC)
            {
                // Get AAC version
                u1Version = u1GetAacVersion(AUD_DEC_AUX);
                if (u1Version==0x0) //AAC_LC
                {
                    *u2Delay=_u6VideoSetCHdelay;
                    LOG(3,"@@@@@ It is AAC_LC @@@@@@\n");
                }
                else//HE-AAC
                {
                    *u2Delay= 0;
                    LOG(3,"@@@@@ It is HE-AAC @@@@@@\n");
                }
            }
            else
#endif //if 0
            {
                *u2Delay=_u6VideoSetCHdelay;
                LOG(3,"@@@@@ It is DTV @@@@@@\n");
            }
	    }// EU DTV
    }
    else if (_AudGetStrSource(AUD_DEC_MAIN) == AUD_STREAM_FROM_HDMI)
    {
        AUD_DRVGetDecodeType(AUD_DEC_MAIN, &eAudDecStreamFrom, &eAudDecFormat);
        if(eAudDecFormat==AUD_FMT_PCM)
        {
            *u2Delay = (_u6VideoSetCHdelay <= 37)? 0 : (_u6VideoSetCHdelay - 37); //PCM Prebuffer.
             LOG(3,"@@@@@ It is HDMI PCM @@@@@@\n");
        }
        *u2Delay = 0;
        return; //HDMI don't take table value.
    }
    else
    {
        *u2Delay=_u6VideoSetCHdelay;
        LOG(3,"@@@@@ It is others  & NOT DTV source@@@@@@\n");
        return;
    }

    //=== 2. Get Value from Cust Table according to Video Info ===
#ifdef CC_AUD_QUERY_VDO_INFO
    i2CustDelayMs = _AudSrmGetVdoInfo();
    LOG(3,"[SRM_AVSYNC] Add Table value %d \n", i2CustDelayMs);
    *u2Delay += i2CustDelayMs;
#endif

}

#endif //CC_ENABLE_AV_SYNC

static BOOL _AudDmxDtvInitPlaybackFifoFull(UINT8 u1DecId)
{
    UINT32 u4WritePtr;
//    UINT32 u4ReadPtr;
    UINT32 u4FifoEnd;
    UINT32 u4FifoStart;

    UNUSED(_AUD_GetAudFifo(u1DecId, &u4FifoStart, &u4FifoEnd));
//    _AUD_DMX_GetAudPtr(u1DecId, &u4ReadPtr, &u4WritePtr);
    _AUD_DMX_GetAudWrtPtr(u1DecId, &u4WritePtr);

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

 void AUD_DrvSetMMAudioOnly(BOOL fgMMAudioOnly)
{
    UNUSED(_fgMMAudioOnly);
	_fgMMAudioOnly = fgMMAudioOnly;
}

#if (defined(CC_AUD_SUPPORT_MS10) && defined(CC_MT5391_AUD_3_DECODER))
static void _AudWaitEventAD(UINT32 * pu4Event, BOOL fgWait)
{
    //first check if there is data in 500ms
    if (!_AudWaitEvent(AUD_DEC_THIRD, pu4Event, fgWait, AUD_EVENT_TIMEOUT_DTV_AD))
    {
        //no data in 500ms, set AD flag to one buffer mode
        if (_arAudDecoder[AUD_DEC_THIRD].u4ReceivePesCount ==0)
        {
            DSP_SetAdFreeRun(TRUE);
        }
        //wait another 4000ms
        if (!_AudWaitEvent(AUD_DEC_THIRD, pu4Event, fgWait, AUD_EVENT_TIMEOUT_DTV- AUD_EVENT_TIMEOUT_DTV_AD))
        {
            vDspSetFifoReadPtr(AUD_DEC_THIRD, _arAudDecoder[AUD_DEC_THIRD].rCurrentPes.u4Wp);
            DSP_SetStartPtsToShm(AUD_DEC_THIRD, (UINT32)(_arAudDecoder[AUD_DEC_THIRD].rCurrentPes.u4Pts & 0xffffffff), _arAudDecoder[AUD_DEC_THIRD].rCurrentPes.u4Wp);
            *pu4Event = (UINT32)AUD_CMD_AVSYNC;
            LOG(7, "NO AV sync message retrieve in DTV AD Path\n");
        }
    }
    return;
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

        eCmd = (AUD_DEC_CMD_T)u4Event;
        /* Check if decoder state is the same as command */
        if(((eCmd  == AUD_CMD_PLAY) && (_arAudDecoder[u1DecId].eDecState == AUD_DEC_PLAYING)) ||
           ((eCmd == AUD_CMD_STOP) && (_arAudDecoder[u1DecId].eDecState == AUD_DEC_STOP)) ||
           ((eCmd == AUD_CMD_PAUSE) && (_arAudDecoder[u1DecId].eDecState == AUD_DEC_PAUSE)))
        {
		    LOG(7, "Aud dec(%d) thread Get duplicated <%s> event\n", u1DecId, _paszAudCmd[u4Event]);
            AUD_CommandDone(u1DecId, eCmd, AUD_OK);
            i4CmdOk = FALSE;
        }
        else if (AUD_DrvDspCallBackProcess(u1DecId, eCmd))
        {
            i4CmdOk = FALSE;
            LOG(5, " DSP Callback AUD_DRVCmd %s\n",_paszAudCmd[eCmd]);
        }
        else
        {
            /* Check command and decoder event flag */
            if ((1 << (UINT8)eCmd) & _arAudDecoder[u1DecId].u4EventFlag)
            {
                i4CmdOk = TRUE;
				LOG(7, "Aud dec(%d) thread Get valid <%s> event and wake up\n", u1DecId, _paszAudCmd[u4Event]);
            }
            else
            {
                //avoid swdmx hang by error resume command
                //ERROR CMD must command done to release semaphone!!! 2011/07/07
                AUD_CommandDone(u1DecId, eCmd, AUD_OK);
                LOG(0, "ERROR AUD_DRVCmd %s\n",_paszAudCmd[eCmd]);
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
    UINT16 		u2WaitTime;
    UINT8                      u1Version;

    UNUSED(eAudDecFormat);
    UNUSED(u1Version);

    // Jessica: remove to avoid wait VDP timeout
    //UNUSED(x_sema_lock(_hVdpSyncSema, X_SEMA_OPTION_NOWAIT));
    if (_fgVdpChangeState)
    {
        u2WaitTime = VDP_WAITING_TIME_OUT;
        LOG(5, "[SRM_AVSYNC] >> Normal VDP Wait Condition.....................\n");
    }
    else
    {
        _fgwaitForAoutReinit=FALSE;
        u2WaitTime = 10;
        LOG(5, "[SRM_AVSYNC] >> Short VDP Wait Condition.....................\n");
    }

    i4Ret = x_sema_lock_timeout(_hVdpSyncSema, u2WaitTime);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));
  //  if (i4Ret == OSR_TIMEOUT)
    {
   //     LOG(3, "[SRM_AVSYNC] _WaitVdpReady timeout\n");
        u2Delay = _u6VideoSetCHdelay;
        _AudSrmAdjustDelayByAudFmt(&u2Delay);
        LOG(3, "[SRM_AVSYNC] Set Delay (%d)\n", u2Delay);

        //AUD_DspInputChannelDelay(_arAudDecoder[AUD_DEC_MAIN].eStreamFrom);
        //AUD_DspChannelDelay(((_u6VideoSetCHdelay*34)/5)+(u2Delay_init*34/5), AUD_CH_ALL, AUD_DEC_MAIN);
#ifdef CC_AUD_SKYPE_ZERO_DELAY
        _fgSkypeMode = FeederGetSkypeSourceFlag();
        if ((_arAudDecoder[AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)&&(_fgSkypeMode))
        {
            LOG(1," [_WaitVdpReady]######### Set zero channel delay for Skype #########\n ");
            AUD_DspChannelDelay(0, AUD_CH_ALL, AUD_DEC_MAIN);
        }
        else
        {
        	AUD_DspChannelDelay(((u2Delay*34)/5), AUD_CH_ALL, AUD_DEC_MAIN);
        }
#else
        AUD_DspChannelDelay(((u2Delay*34)/5), AUD_CH_ALL, AUD_DEC_MAIN);
#endif
        _fgIgnoreVdpInfo = TRUE;
        _fgwaitForAoutReinit=TRUE; //This flag should set TRUE with Set Ch Delay.
        _fgVdpChangeState = FALSE;
    }
    //_fgwaitForAoutReinit=TRUE;
    _fgwaitForPIPAoutReinit=TRUE;
    //AUD_DspDecPlayMuteEnable(AUD_DEC_MAIN, FALSE);
    //LOG(3,"########### For AV sync , UN-MUTE Audio  2!!!! ######\n");
}
#endif

static void _DrvThreadInit(void)
{
    UINT8 u1DecId;

#ifdef CC_AUD_NCSTOOL_SUPPORT
    //Printf("!!!!!!!!!! Allocate SW Reg !!!!!!!!!!\n");
    vAllocateSWReg();
#endif

//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
    _AudMuxerInit();
//#endif

    if (!_fgAudDrvThreadInit)
    {
        VERIFY(x_sema_create(&_hAtunerSyncSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
#ifdef CC_ENABLE_AV_SYNC
        VERIFY(x_sema_create(&_hVdpSyncSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
#endif
        VERIFY(x_sema_create(&_hSemaLoadCode, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY(x_sema_create(&_ahSemaUopComDone[AUD_DEC_MAIN], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY(x_sema_create(&_ahSemaUopComDone[AUD_DEC_AUX], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY(x_sema_create(&_ahSemaUopComDone[AUD_DEC_THIRD], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY(x_sema_create(&_ahSemaUopComDone[AUD_DEC_4TH], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

        VERIFY(x_sema_create(&_hDetectionSyncSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY(x_sema_create(&_hFeedStreamSyncSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

        VERIFY(x_sema_create(&_arAudDrvStateSema[AUD_DEC_MAIN], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY(x_sema_create(&_arAudDrvStateSema[AUD_DEC_AUX], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY(x_sema_create(&_arAudDrvStateSema[AUD_DEC_THIRD], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY(x_sema_create(&_arAudDrvStateSema[AUD_DEC_4TH], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);

#ifdef LINUX_TURNKEY_SOLUTION
                VERIFY(x_sema_create(&_hAudDataArrived[AUD_DEC_MAIN], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
                VERIFY(x_sema_create(&_hAudDataArrived[AUD_DEC_AUX], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
                VERIFY(x_sema_create(&_hAudDataArrived[AUD_DEC_THIRD], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
                VERIFY(x_sema_create(&_hAudDataArrived[AUD_DEC_4TH], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
#endif

//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
        VERIFY(x_sema_create(&_hSemaDataUploadWait, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
//#endif
#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
		VERIFY(x_sema_create(&_hSemaDataTransferWait, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
		VERIFY(x_sema_create(&_hSemaDataTransferWait1, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);

#endif

        VERIFY((x_msg_q_create(&_hAudApllCmdQueue, ADRV_APLL_CMD_Q, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
        VERIFY((x_msg_q_create(&_ahAudCmdQueue[AUD_DEC_MAIN], ADRV_CMD_Q_NAME1, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
        VERIFY((x_msg_q_create(&_ahAudCmdQueue[AUD_DEC_AUX], ADRV_CMD_Q_NAME2, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
#ifdef CC_MT5391_AUD_3_DECODER
        VERIFY((x_msg_q_create(&_ahAudCmdQueue[AUD_DEC_THIRD], ADRV_CMD_Q_NAME3, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
        VERIFY((x_msg_q_create(&_ahAudCmdQueue[AUD_DEC_4TH], ADRV_CMD_Q_NAME4, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
#endif

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
        VERIFY(x_sema_create(&_arParserInfo[AUD_DEC_MAIN]._hSemaHDMIParserThreadWait, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
#endif


        // Create audio thread1
        u1DecId = AUD_DEC_MAIN;
        _ChangeAudioState(u1DecId, AUD_TRIGGER_ADSP);
        VERIFY(x_thread_create(&_hAudDrvThread[u1DecId], AUD_DRV_THREAD1_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudDrvThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);

        // Create audio thread2
        u1DecId = AUD_DEC_AUX;
        _ChangeAudioState(u1DecId, AUD_WAIT_POWER_ON);
        VERIFY(x_thread_create(&_hAudDrvThread[u1DecId], AUD_DRV_THREAD2_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudDrvThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);

#ifdef CC_MT5391_AUD_3_DECODER
        // Create audio thread3
        u1DecId = AUD_DEC_THIRD;
         _ChangeAudioState(u1DecId, AUD_WAIT_POWER_ON);
        VERIFY(x_thread_create(&_hAudDrvThread[u1DecId], AUD_DRV_THREAD3_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudDrvThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
#endif

#ifdef CC_AUD_4_DECODER_SUPPORT
        // Create audio thread4
        u1DecId = AUD_DEC_4TH;
         _ChangeAudioState(u1DecId, AUD_WAIT_POWER_ON);
        VERIFY(x_thread_create(&_hAudDrvThread[u1DecId], AUD_DRV_THREAD4_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudDrvThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
#endif

        // Create apll monitor thread
        VERIFY(x_thread_create(&_hAudApllMonThread, AUD_APLL_MONITOR_THREAD_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudApllMonitorThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);

#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0521  -- 2
        u1DecId = AUD_DEC_MAIN;
        // Create DTV playback thread
        VERIFY(x_thread_create(&_hAudDtvThread[u1DecId], AUD_DRV_DTV_MAIN_THREAD_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudDtvOnPlayThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
#else
        u1DecId = AUD_DEC_AUX;
        // Create DTV playback thread
        VERIFY(x_thread_create(&_hAudDtvThread[u1DecId], AUD_DRV_DTV_AUX_THREAD_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudDtvOnPlayThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
#endif

    // Create DSP monitor thread  //  -- DSP Force --
#ifdef DSP_FORCE_RESET_ENABLE
        VERIFY(x_thread_create(&_hAudDspMonThread, AUD_DSP_MONITOR_THREAD_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudDspMonitorThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
#endif

#ifdef CC_AUD_NCSTOOL_SUPPORT
        //Printf("!!!!!!!!!! Create _AudToolThread !!!!!!!!!!\n");
        VERIFY(x_thread_create(&_hAudToolThread, AUD_TOOL_THREAD_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudToolThread, 0, NULL) == OSR_OK);
#endif

#ifdef CC_ENABLE_AV_SYNC
        VERIFY(x_thread_create(&_hAudAVSyncThread, AUD_DRV_AV_SYNC_THREAD, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudAvSyncThread, 0, NULL) == OSR_OK);
#endif

//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
        VERIFY(x_thread_create(&_hAudDataUploadThread, AUD_DRV_DATA_UPLOAD_THREAD, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudDataUploadThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
//#endif

#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
		VERIFY(x_thread_create(&_hAudDataTransferThread, AUD_DRV_DATA_TRANSFER_THREAD, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudDataTransferThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
#endif

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
        u1DecId = AUD_DEC_MAIN;
        VERIFY(x_thread_create(&_hAudHDMIParserThread, AUD_DRV_Parser_THREAD, AUD_PRE_PARSER_STACK_SIZE, AUD_PRE_PARSER_PRIORITY,
            _AudHDMIParserThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);                      
#endif

#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
		_AudOssAdap_Init();
#endif
        _fgAudDrvThreadInit = TRUE;
    }
}

static void _ChangeAudioState(UINT8 u1DecId, AUD_DRV_STATE_T eNewState)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    VERIFY(x_sema_lock(_arAudDrvStateSema[u1DecId], X_SEMA_OPTION_WAIT) == OSR_OK);
    if (_arAudDrvState[u1DecId] != eNewState)
    {
        LOG(7, "Aud dec(%d) thread ChangeAudioState (%s -> %s)\n",u1DecId,_paszAudState[_arAudDrvState[u1DecId]], _paszAudState[eNewState]);
        _arAudDrvState[u1DecId] = eNewState;
    }
    VERIFY(x_sema_unlock(_arAudDrvStateSema[u1DecId]) == OSR_OK);

}

static void _FlushAudioFifo(UINT8 u1DecId)
{
    UINT32 u4FifoStart;
    UINT32 u4FifoEnd;

    AUD_DEC_ID_VALIDATE(u1DecId);

    DSP_GetAFIFOPhysicalAddr(u1DecId, &u4FifoStart, &u4FifoEnd);
    LOG( 9, "Flush audio fifo = (%x,%x)\n", u4FifoStart, u4FifoEnd);

    VERIFY(u4FifoEnd >= u4FifoStart);
    x_memset((VOID*)VIRTUAL(u4FifoStart), 0, u4FifoEnd - u4FifoStart);
    DSP_FlushInvalidateDCacheFree( u4FifoStart, u4FifoEnd - u4FifoStart);
}

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT 
static void _FlushParserFifo(void)
{
    UINT32 u4FifoStart;
    UINT32 u4FifoEnd;


    DSP_GetHDMIParserFIFOPhysicalAddr(&u4FifoStart, &u4FifoEnd);
    LOG( 5, "Flush parser fifo = (%x,%x)\n", u4FifoStart, u4FifoEnd);

    VERIFY(u4FifoEnd >= u4FifoStart);
    x_memset((VOID*)VIRTUAL(u4FifoStart), 0, u4FifoEnd - u4FifoStart);
    DSP_FlushInvalidateDCacheFree( u4FifoStart, u4FifoEnd - u4FifoStart);
}
#endif

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
    AudDecNfyFct  pfAudDecNfy = NULL;
    if (pvArg != NULL)
    {
        u1DecId = *(UINT8 *)pvArg;
    }

    AUD_DEC_ID_VALIDATE(u1DecId);
    if (u1DecId == AUD_DEC_MAIN)
    {
        AUD_CLR_BIT(REG_BS_CFG, PAR_SEL);
        AUD_CLR_BIT(REG_BS_CFG, DEC3_PAR_SEL);
        AUD_CLR_BIT(REG_BS_CFG, DEC4_PAR_SEL);
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        AUD_SET_BIT(REG_BS_CFG, PAR_SEL);
        AUD_CLR_BIT(REG_BS_CFG, DEC3_PAR_SEL);
        AUD_CLR_BIT(REG_BS_CFG, DEC4_PAR_SEL);
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        AUD_CLR_BIT(REG_BS_CFG, PAR_SEL);
        AUD_SET_BIT(REG_BS_CFG, DEC3_PAR_SEL);
        AUD_CLR_BIT(REG_BS_CFG, DEC4_PAR_SEL);
    }
    else
    {
        AUD_CLR_BIT(REG_BS_CFG, PAR_SEL);
        AUD_CLR_BIT(REG_BS_CFG, DEC3_PAR_SEL);
        AUD_SET_BIT(REG_BS_CFG, DEC4_PAR_SEL);
    }

    u4DesAddr = u4GetAFIFOStart(u1DecId);
    u4AFifofSZ = u4GetAFIFOEnd(u1DecId) - u4GetAFIFOStart(u1DecId);
    u4Residual = _arAudDecoder[u1DecId].rMemBuffer.u4Length;
    u4StreamAddr = PHYSICAL((UINT32)_arAudDecoder[u1DecId].rMemBuffer.pData);

    _i4MemStrLoopCnt = (INT32)_arAudDecoder[u1DecId].rMemBuffer.u4LoopCount;
    _fgInfinite = (_i4MemStrLoopCnt <= 0) ? TRUE : FALSE;
    _fgForceStopStrmDma = FALSE;
    _u4FeedStreamResult = (UINT32)AUD_OK;

    _fgFeedStrmFinshed = FALSE; // Feed_and_Auddrv Thread sync

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
	        _fgFeedStrmFinshed = TRUE;  // Feed_and_Auddrv Thread sync
	        x_thread_exit();            // In Linux, if doesn't call exit to free handle, finally it will get "OUT OF Handle" when creating this thread.
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
            _fgFeedStrmFinshed = TRUE;	// Feed_and_Auddrv Thread sync
            x_thread_exit();
            return;
        }

        if ((AUD_IsFifoEmpty(u1DecId)) || (u4Retry > 100))
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
    // Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy));
    pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_AUD_CLIP_PLAY_DONE, 0, 0);
    LOG(9, "Dec(%d) stream size 0x%08x finished\n", u1DecId,
           _arAudDecoder[u1DecId].rMemBuffer.u4Length);

    _fgFeedStrmFinshed = TRUE;    // Feed_and_Auddrv Thread sync
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

    AUD_DEC_ID_VALIDATE_2(u1DecId);

    while (1)
    {
        VERIFY(x_sema_lock(_hSemaFeederThreadWait, X_SEMA_OPTION_WAIT) == OSR_OK);

        fgFeedInit = FALSE;
        fgFeedEnd = FALSE;

        if (u1DecId == AUD_DEC_MAIN)
        {
            AUD_CLR_BIT(REG_BS_CFG, PAR_SEL);
            AUD_CLR_BIT(REG_BS_CFG, DEC3_PAR_SEL);
            AUD_CLR_BIT(REG_BS_CFG, DEC4_PAR_SEL);
        }
        else if (u1DecId == AUD_DEC_AUX)
        {
            AUD_SET_BIT(REG_BS_CFG, PAR_SEL);
            AUD_CLR_BIT(REG_BS_CFG, DEC3_PAR_SEL);
            AUD_CLR_BIT(REG_BS_CFG, DEC4_PAR_SEL);
        }
        else if (u1DecId == AUD_DEC_THIRD)
        {
            AUD_CLR_BIT(REG_BS_CFG, PAR_SEL);
            AUD_SET_BIT(REG_BS_CFG, DEC3_PAR_SEL);
            AUD_CLR_BIT(REG_BS_CFG, DEC4_PAR_SEL);
        }
        else
        {
            AUD_CLR_BIT(REG_BS_CFG, PAR_SEL);
            AUD_CLR_BIT(REG_BS_CFG, DEC3_PAR_SEL);
            AUD_SET_BIT(REG_BS_CFG, DEC4_PAR_SEL);
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

void AUD_SentReaptNumber(UINT8 u1DecId, UINT32 ReaptNumber)
{
    _arAudDecoder[u1DecId].rMemBuffer.u4LoopCount = ReaptNumber;
}

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
static void _AudHDMIParserThread(void* pvArg)
{
    UINT8 u1DecId = AUD_DEC_MAIN;
    UINT32 u4TransferTotalSZ = 0;
    UINT32 u4DesAddr = 0;
    UINT32 u4StreamAddr = 0;
    UINT32 u4TransferSZ = 0;
    //UINT32 u4SkipDataSize;  //don't skip dummy data
    UINT32 u4Pd = 0;
    if (pvArg != NULL)
    {
        u1DecId = *(UINT8 *)pvArg;
    }
    AUD_DEC_ID_VALIDATE_2(u1DecId);//fix clockwork check
    _arParserInfo[u1DecId].eDSPBuffer.u4FifoStartAddr = PHYSICAL(u4GetAFIFOStart(u1DecId));
    _arParserInfo[u1DecId].eDSPBuffer.u4FifoEndAddr   = PHYSICAL(u4GetAFIFOEnd(u1DecId));
    _arParserInfo[u1DecId].eDSPBuffer.u4FifoSize      = u4GetAFIFOEnd(u1DecId) - u4GetAFIFOStart(u1DecId);
    _arParserInfo[u1DecId].eHDMIBuffer.u4FifoStartAddr = PHYSICAL((UINT32)u4GetHDMIParserFIFOStart());
    _arParserInfo[u1DecId].eHDMIBuffer.u4FifoEndAddr= PHYSICAL((UINT32)u4GetHDMIParserFIFOEnd());
    _arParserInfo[u1DecId].eHDMIBuffer.u4FifoSize= u4GetHDMIParserFIFOEnd()- u4GetHDMIParserFIFOStart();

    while (1)
    {
        VERIFY(x_sema_lock(_arParserInfo[u1DecId]._hSemaHDMIParserThreadWait, X_SEMA_OPTION_WAIT) == OSR_OK); 
        _arParserInfo[u1DecId].eHDMIParserSta = AUD_HDMI_PARSER_INIT;
        while(_arParserInfo[u1DecId]._fgHDMIParserThreadGo)
        {
            switch(_arParserInfo[u1DecId].eHDMIParserSta )
            {
                case AUD_HDMI_PARSER_INIT:
                    u4DesAddr = _arParserInfo[u1DecId].eDSPBuffer.u4FifoStartAddr;
                    u4StreamAddr = _arParserInfo[u1DecId].eHDMIBuffer.u4FifoStartAddr;                    
                    _arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr = _arParserInfo[u1DecId].eHDMIBuffer.u4FifoStartAddr;
                    _arParserInfo[u1DecId].eSpdifInfo.u4Size = 0;
                    _arParserInfo[u1DecId].eSpdifInfo.u4Wp = _arParserInfo[u1DecId].eHDMIBuffer.u4FifoStartAddr;
                    _arParserInfo[u1DecId].u4LastTransferPtr = _arParserInfo[u1DecId].eHDMIBuffer.u4FifoStartAddr;
                    _arParserInfo[u1DecId].u4PaPb = 0;
                    _arParserInfo[u1DecId].eHDMIParserSta = AUD_HDMI_PARSER_FIND_VALID_FRAME;
                    _arParserInfo[u1DecId].u4NoRawDataSize = 0;
                    _arParserInfo[u1DecId].u4NoTransferSize = 0;
                    _arParserInfo[u1DecId].e_aud_det_fmt = _arAudDecoder[u1DecId].eDecFormat;
                    _arParserInfo[u1DecId]._fgFmtChg = FALSE;
                    _arParserInfo[u1DecId]._fgDTSCD = FALSE;
                    _arParserInfo[u1DecId]._fgRaw = FALSE;
                    break;
        /*
                case   AUD_HDMI_PARSER_BUFFERING:
                    if(AUD_DrvCheckHDMISize(_arParserInfo[u1DecId].eHDMIBuffer,HDMI_BUFFER_SIZE))
                    {
                        _arParserInfo[u1DecId].eHDMIParserSta = AUD_HDMI_PARSER_FIND_VALID_FRAME;
                    }
                    else
                    {
                        LOG(5,"wait 3 frame buffer.\n");
                        x_thread_delay(1);
                    }
                    break;
                    */

#ifdef CC_AUD_HDMI_PARSER_3_0_SUPPORT                    
                case AUD_HDMI_PARSER_FIND_VALID_FRAME:
                    if(AUD_DrvCheckHDMISize(_arParserInfo[u1DecId].eHDMIBuffer,6))
                    {
                        DSP_InvalidateDCache(_arParserInfo->eHDMIBuffer.u4FifoPtr,6);
                        if(AUD_DrvCheckIECSyncWord(&_arParserInfo[u1DecId]))
                        {
                            u4Pd = AUD_DrvSwapWord(AUD_READIEC16(VIRTUAL(AUD_DrvGetCircularAddress(u4GetHDMIParserFIFOStart(),u4GetHDMIParserFIFOEnd(),_arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr+2))));
                            if(_arParserInfo[u1DecId].eSpdifInfo.u2format==0x15)  //0x15 means E-AC3
                            {
                                u4TransferTotalSZ = u4Pd;//(u4Pd+1)&0xFFFFFFFE;  //EAC3 pd is in byte //16bit alignment
                            }
                            else
                            {
                                u4TransferTotalSZ = u4Pd/8;//(u4Pd/8+1)&0xFFFFFFFE;  //other format pd is in bit,but we transfer via parser is in  byte    //16bit alignment
                            }
                            _arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr = AUD_DrvGetCircularAddress(u4GetHDMIParserFIFOStart(),u4GetHDMIParserFIFOEnd(),_arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr+4);                            
                            _arParserInfo[u1DecId].u4NoRawDataSize = 8;                            
                            _arParserInfo[u1DecId].eSpdifInfo.u4Size = u4Pd;
                            _arParserInfo[u1DecId].e_aud_det_fmt = AUD_DrvPcToFormat(_arParserInfo[u1DecId].eSpdifInfo.u2format);
                            _arParserInfo[u1DecId].eHDMIParserSta = AUD_HDMI_PARSER_WAIT_VALID_FRAME;
                        }
                        else
                        {

                            if(_arParserInfo[u1DecId]._fgDTSSW)
                            {
                                _arParserInfo[u1DecId].u4NoRawDataSize = 2;                                
                                _arParserInfo[u1DecId].e_aud_det_fmt = AUD_DrvPcToFormat(_arParserInfo[u1DecId].eSpdifInfo.u2format);  //for DTS CD ,we set pc as 0XB in function AUD_DrvCheckIECSyncWord
                            }
                            _arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr = AUD_DrvGetCircularAddress(u4GetHDMIParserFIFOStart(),u4GetHDMIParserFIFOEnd(),_arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr+2);
                            _arParserInfo[u1DecId].u4NoRawDataSize += 2;
                            _arParserInfo[u1DecId].u4NoTransferSize += 2;  
                            _arParserInfo[u1DecId].eHDMIParserSta = AUD_HDMI_PARSER_FIND_VALID_FRAME;
                             if(_arParserInfo[u1DecId].u4NoRawDataSize >= HDMI_PARSER_DETECT_PCM_SIZE)
                            {
                                _arParserInfo[u1DecId]._fgRaw = FALSE;
                                _arParserInfo[u1DecId]._fgDTSCD =FALSE;
                                _arParserInfo[u1DecId].e_aud_det_fmt = AUD_FMT_PCM;
                            }
                            // transfer all DTS CD data, the valid data length is not the same as FSIZE in DTS header 
                            if((_arAudDecoder[u1DecId].eDecFormat == AUD_FMT_PCM)||_arParserInfo[u1DecId]._fgDTSCD)
                            {
                                if(_arParserInfo[u1DecId].u4NoTransferSize>=HDMI_PARSER_PCM_TRANSFER_SIZE)
                                {
                                    u4TransferTotalSZ = _arParserInfo[u1DecId].u4NoTransferSize;
                                    _arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr = _arParserInfo[u1DecId].u4LastTransferPtr;                                
                                    _arParserInfo[u1DecId].eHDMIParserSta = AUD_HDMI_PARSER_WAIT_VALID_FRAME;
                                }
                            }
                        }
                        if(_arParserInfo[u1DecId].e_aud_det_fmt!=_arAudDecoder[u1DecId].eDecFormat)
                        {
                            _arParserInfo[u1DecId]._fgFmtChg = TRUE;
                        }
                        else
                        {
                            if(_arParserInfo[u1DecId]._fgFmtChg)
                            {
                                LOG(7,"Format change two times.\n");
                            }
                            _arParserInfo[u1DecId]._fgFmtChg = FALSE;
                        }
                    }
                    else
                    {
                        LOG(9,"check sync word not enough data.\n");
                        x_thread_delay(1);
                    }
                    break;
#else
                    case AUD_HDMI_PARSER_FIND_VALID_FRAME:
                       if(AUD_DrvCheckHDMISize(_arParserInfo[u1DecId].eHDMIBuffer,6))
                       {
                           if(AUD_DrvCheckIECSyncWord(&_arParserInfo[u1DecId]))
                           {
                               _arParserInfo[u1DecId].eHDMIParserSta = AUD_HDMI_PARSER_WAIT_VALID_FRAME;
                               u4Pd = AUD_DrvSwapWord(AUD_READIEC16(VIRTUAL(AUD_DrvGetCircularAddress(u4GetHDMIParserFIFOStart(),u4GetHDMIParserFIFOEnd(),_arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr+2))));
                               if(_arParserInfo[u1DecId].eSpdifInfo.u2format==0x15)
                               {
                                   u4TransferTotalSZ = u4Pd;//(u4Pd+1)&0xFFFFFFFE;  //EAC3 pd is in byte
                               }
                               else
                               {
                                   u4TransferTotalSZ = u4Pd/8;//(u4Pd/8+1)&0xFFFFFFFE;  //other format pd is in bit,but we transfer via parser is in  byte    
                               }
                               
                               if(u4TransferTotalSZ==0)
                               {
                                   _arParserInfo[u1DecId].eHDMIParserSta = AUD_HDMI_PARSER_FIND_VALID_FRAME;
                               }
                               else
                               {
                                   _arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr = AUD_DrvGetCircularAddress(u4GetHDMIParserFIFOStart(),u4GetHDMIParserFIFOEnd(),_arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr+4);
                                   _arParserInfo[u1DecId].eSpdifInfo.u4Size = u4Pd;
                               }
                           }
                           else
                           {
                               _arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr = AUD_DrvGetCircularAddress(u4GetHDMIParserFIFOStart(),u4GetHDMIParserFIFOEnd(),_arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr+2);
                           }
                       }
                       else
                       {
                           LOG(9,"check sync word not enough data.\n");
                           x_thread_delay(1);
                       }
                       break;
                    
#endif
                case  AUD_HDMI_PARSER_WAIT_VALID_FRAME: 
                    if(AUD_DrvCheckHDMISize(_arParserInfo[u1DecId].eHDMIBuffer,u4TransferTotalSZ+1))
                    { 
                        _arParserInfo[u1DecId].eSpdifInfo.u4Wp = _arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr;
                        _arParserInfo[u1DecId].eHDMIParserSta = AUD_HDMI_PARSER_TANSFER_FRAME;
                    }
                    else
                    {
                        x_thread_delay(1);
                    }
                    break;
                case   AUD_HDMI_PARSER_TANSFER_FRAME:
                   while(u4TransferTotalSZ>0)
                   {
                       u4StreamAddr = _arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr;
                       if((u4StreamAddr+u4TransferTotalSZ)>u4GetHDMIParserFIFOEnd())
                       {
                           u4TransferSZ = u4GetHDMIParserFIFOEnd()-u4StreamAddr;
                       }
                       else
                       {
                           u4TransferSZ = u4TransferTotalSZ;
                       }                             
                       //parser max transfer size=15.5M always > raw data frame size
#ifdef CC_AUD_HDMI_PARSER_3_0_SUPPORT                    
                       if(!_arParserInfo[u1DecId]._fgFmtChg)
#endif                        
                       {
                           if (!PSR_SoftTransfer(PSR_STREAM_AUDIO, u1DecId,
                           u4StreamAddr, u4TransferSZ ,
                           u4DesAddr, u4GetAFIFOStart(u1DecId),
                           u4GetAFIFOEnd(u1DecId), FALSE))
                           {
                               LOG(5, "Aud soft transter fail/terminated\n");
                           }
                       }
                       _arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr = AUD_DrvGetCircularAddress(u4GetHDMIParserFIFOStart(),u4GetHDMIParserFIFOEnd(),_arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr+u4TransferSZ);
                       _arParserInfo[u1DecId].u4LastTransferPtr = _arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr; 
                       u4TransferTotalSZ -= u4TransferSZ;
                       _arParserInfo[u1DecId].u4NoRawDataSize += u4TransferSZ;
                       u4DesAddr = AUD_DrvGetCircularAddress(u4GetAFIFOStart(u1DecId),u4GetAFIFOEnd(u1DecId),u4DesAddr+u4TransferSZ);
                   }
                   _arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr = AUD_DrvGetCircularAddress(u4GetHDMIParserFIFOStart(),u4GetHDMIParserFIFOEnd(),((_arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr+1)&0xFFFFFFFE));
                   _arParserInfo[u1DecId].eHDMIParserSta = AUD_HDMI_PARSER_FIND_VALID_FRAME;
                   _arParserInfo[u1DecId].u4NoTransferSize = 0;  
                   break;
                    
            }
        }
    }
}


#endif

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

            if ((_arAudDecoder[u1DevId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
                &&(_arAudDecoder[AUD_DEC_MAIN].eSynMode != AV_SYNC_SLAVE))
            {
                // For multimedia and not Timeshift case, always return TRUE
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
	      case AUD_FMT_DTS:
                u4StreamFormat = AUD_DTS_DEC1;
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
            case AUD_FMT_SBCDEC:
                u4StreamFormat = AUD_SBC_DEC1;
                break;
            case AUD_FMT_G711DEC:
                u4StreamFormat = AUD_G711_DEC1;
                break;
            case AUD_FMT_G711ENC:
                u4StreamFormat = AUD_G711_ENC1;
                break;
            case AUD_FMT_DRA:
                u4StreamFormat = AUD_DRA_DEC1;
                break;
            case AUD_FMT_COOK:
                u4StreamFormat = AUD_COOK_DEC1;
                break;
            case AUD_FMT_G729DEC:
                u4StreamFormat = AUD_G729_DEC1;
                break;
    #if 1///def CC_VORBIS_SUPPORT
            case AUD_FMT_VORBISDEC:
                u4StreamFormat = AUD_VORBIS_DEC1;
                break;
    #endif
            case AUD_FMT_WMAPRO:
                u4StreamFormat = AUD_WMAPRO_DEC1;
                break;
            case AUD_FMT_DSP_AMR:
                u4StreamFormat = AUD_AMR_DEC1;
                break;
            case AUD_FMT_DSP_AWB:
                u4StreamFormat = AUD_AWB_DEC1;
                break;
            default:
                u4StreamFormat = AUD_PINK_DEC1;
                break;
        }
        // load ADSP decoder's code while decoder is stopped
        DSP_ResetDec(u1DecId);
        LOG( 7,"LoadAdspCode(0) = %s\n", _aszCodeType[eStreamFormat]);
        DSP_LoadRamCode(u4StreamFormat);
    }
    else if (u1DecId == AUD_DEC_AUX)  // second decoder
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
            case AUD_FMT_DTS:
                u4StreamFormat = AUD_DTS_DEC2;
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
            case AUD_FMT_MP3:
                u4StreamFormat = AUD_MP3_DEC2;
                break;
            case AUD_FMT_LPCM:
                u4StreamFormat = AUD_LPCM_DEC2;
                break;
            case AUD_FMT_DRA:
                u4StreamFormat = AUD_DRA_DEC2;
                break;
            case AUD_FMT_G729DEC:
                u4StreamFormat = AUD_G729_DEC2;
                break;
    #if 1///def CC_VORBIS_SUPPORT
            case AUD_FMT_VORBISDEC:
                u4StreamFormat = AUD_VORBIS_DEC2;
                break;
    #endif
            case AUD_FMT_COOK:
                u4StreamFormat = AUD_COOK_DEC2;
                break;
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
    else if (u1DecId == AUD_DEC_THIRD)
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
    #ifdef MP3ENC_SUPPORT
            case AUD_FMT_MP3ENC:
                u4StreamFormat = AUD_MP3ENC_DEC3;
                break;
    #endif
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
#ifdef CC_AUD_4_DECODER_SUPPORT
    else if (u1DecId == AUD_DEC_4TH)
    {
        switch(eStreamFormat)
        {
            case AUD_FMT_SBCENC:
                u4StreamFormat = AUD_SBCENC_DEC4;
                break;
            case AUD_FMT_PCM:
                u4StreamFormat = AUD_PCM_DEC4;
                break;
            default:
                LOG(1, "Invalid switch in _LoadAdspCode\n");
                break;
        }
        // load ADSP decoder's code while decoder is stopped
        DSP_ResetDec(u1DecId);
        LOG( 7,"LoadAdspCode(3) = %s\n", _aszCodeType[eStreamFormat]);
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
#ifdef CC_FPGA
    return TRUE;
#else
    //return(fgApiVideoStable(_u1VideoFrameFlag));
    return (!(BOOL)_u4DrvVideoGetMute((UINT32)_u1VideoFrameFlag));
#endif
}

#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0521
static BOOL _AvSyncControl(UINT8 u1DecId)
{
    BOOL fgRet = FALSE;

    // Only DTV & ATV need AV synchronization
    if (((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)||(_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) ) &&
        (AUD_GetAvSynMode(u1DecId) != AV_SYNC_FREE_RUN) &&
        (!_AudDmxDtvInitPlaybackFifoFull(u1DecId)))
    {
        fgRet = TRUE;
    }
    else if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_LINE_IN) &&
        (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) &&
        (!_afgAtvVideoDisplayFlag[u1DecId]) &&
        (!_IsVideoStable()) &&
        (_arAudDecoder[u1DecId].eDecFormat !=  AUD_FMT_DETECTOR))
    {
        fgRet = TRUE;
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
#ifdef CC_AUD_SKYPE_SUPPORT
             (u1DecId == AUD_DEC_MAIN) &&
#endif
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

    _prMPV = &(prDbase->rMpv[0]);

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

            if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
            {
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

            if (fgFreeRun == AUD_DspIsWithVideo(u1DecId))
            {
                AUD_DspSetWithVideo(u1DecId, (!fgFreeRun));
            }
        }
    }
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

#ifndef CC_AUD_HDMI_PARSER_2_0_SUPPORT
/* Patch for wrong HDMI content from player
    Do not decode DDP and skip CRC error mute  if HDMI source to avoid this kind of HDMI content.
*/
static UINT32 _gu4HDMIcase = 0;

void _AudSetHDMICase (UINT32 u4HDMI)
{
    _gu4HDMIcase = u4HDMI;
}

UINT32 _AudGetHDMICase (void)
{
    return _gu4HDMIcase;
}
#endif


static SAMPLE_FREQ_T _eHdmiSmpRate = FS_48K;

static void _DecoderInitSetup(UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt)
{
    UINT8 u1HdmiAudSf;
    SAMPLE_FREQ_T eAudDrvSampleFreq;
    UINT32 u4Temp;


#ifdef CC_AUD_4_DECODER_SUPPORT
    if (u1DecId == AUD_DEC_4TH)
    {
        if (eDecFmt == AUD_FMT_SBCENC)
        {
            DSP_Set4thDecMode(TRUE);
        }
        else
        {
            DSP_Set4thDecMode(FALSE);
        }
    }
#endif

#ifdef CC_MT5391_AUD_3_DECODER
    if (u1DecId == AUD_DEC_MAIN)
    {
        DSP_SetDualDecMode(FALSE);
        AUD_ClkCfgDualDecMode(FALSE); //0714
        DSP_SetAsrcTrackingMode(FALSE);
    }
#else
    {
        // In Single Decoder mode, clear all mutliple decoder related info.
        DSP_SetDualDecMode(FALSE);
        AUD_ClkCfgDualDecMode(FALSE);
        DSP_SetAsrcTrackingMode(FALSE);
    }
#endif

#ifndef CC_AUD_HDMI_PARSER_2_0_SUPPORT
    /* Patch for wrong HDMI content from player
        Do not decode DDP and skip CRC error mute if HDMI source to avoid this kind of HDMI content.
    */
    if (u1DecId == AUD_DEC_MAIN && eStreamFrom == AUD_STREAM_FROM_HDMI)
    {
        _AudSetHDMICase (1); // set HDMI work around
    }
    else
    {
        _AudSetHDMICase (0);
    }
#else
    if ((u1DecId == AUD_DEC_MAIN) && (eStreamFrom == AUD_STREAM_FROM_HDMI) &&
    	(_arParserInfo[u1DecId].eSpdifInfo.u2format == 0x15))
    {
        // HDMI, E-AC-3
        DSP_SetASRCNumeratorQuarter (TRUE);
    }
    else
    {
        DSP_SetASRCNumeratorQuarter (FALSE);
    }
#endif

    if (eDecFmt == AUD_FMT_PCM)
    {
    #ifdef CC_AUD_PCM_LINE_IN_LPF
        DSP_SetPCMLineInLPF(FALSE);
    #endif
        if ((eStreamFrom == AUD_STREAM_FROM_MEMORY) ||
            (eStreamFrom == AUD_STREAM_FROM_FEEDER))
        {
            DSP_SetPcmType(u1DecId, _arAudDecoder[u1DecId].rPcmSetting.ePcmDataInfo);
            DSP_SetPcmFs(u1DecId, FS_48K); // default set as 48K
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

            u4Temp = (UINT16)DRVCUST_OptGet(eAudioPcmPrebufBankNum);
            if ((_eHdmiSmpRate == FS_32K) && (u4Temp>3))
            {
                //LOG(0,"<============= AUD_DspSetPcmPreBuffer====  32\n");
                AUD_DspSetPcmPreBuffer(AUD_DEC_MAIN, (u4Temp-3));
            }
            else
            {
                //LOG(0,"<============= AUD_DspSetPcmPreBuffer====  44\n");
                AUD_DspSetPcmPreBuffer(AUD_DEC_MAIN, u4Temp);
            }

            DSP_SetAsrcTrackingMode(TRUE);
        }
        else if (eStreamFrom == AUD_STREAM_FROM_LINE_IN)
        {
#ifdef USE_16BIT_ALIGNMENT
            DSP_SetPcmType(u1DecId, PCM_16_BITS_BIG_ENDIAN);
#else
            DSP_SetPcmType(u1DecId, PCM_24_BITS);
#endif
            ///DSP_SetPcmFs(u1DecId, FS_52K); // default set as 52K
            ///DSP_SetPcmFs(u1DecId, FS_62K); // default set as 62K
            #ifdef CC_FPGA
            DSP_SetPcmFs(u1DecId, FS_48K); // default set as 48K for FPGA
            #else
            #if defined(CC_MT5368) || defined(CC_MT5396)
            DSP_SetPcmFs(u1DecId, FS_48K); // recover back to 48K for 5368/96 initial stage
            #else
			#if defined(CC_MT5389)
                DSP_SetPcmFs(u1DecId, FS_56K); // for 5389 ATV spur workaround by yi.xiao
            #else
			    DSP_SetPcmFs(u1DecId, FS_64K_SRC); // default set as 64K
			#endif
            #endif
            #endif
        #ifdef CC_AUD_PCM_LINE_IN_LPF
            DSP_SetPCMLineInLPF(TRUE);
        #endif
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
#ifndef CC_AUD_SKYPE_SUPPORT
            DSP_SetPcmFs(u1DecId, FS_48K); // default set as 48K
#endif
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
    else  if (eDecFmt == AUD_FMT_G711DEC)
    {
 #ifdef DSP_G711_A_LAW
            DSP_SetPcmType(u1DecId, PCM_A_LAW_LITTLE_ENDIAN);
 #else
            DSP_SetPcmType(u1DecId, PCM_U_LAW_LITTLE_ENDIAN);
 #endif
            DSP_SetPcmFs(u1DecId, FS_32K);
    }
    else  if (eDecFmt == AUD_FMT_G711ENC)
    {
 #ifdef DSP_G711_A_LAW
            DSP_SetPcmType(u1DecId, PCM_A_LAW_LITTLE_ENDIAN);
 #else
            DSP_SetPcmType(u1DecId, PCM_U_LAW_LITTLE_ENDIAN);
 #endif
    }

#ifdef CC_MT5391_AUD_3_DECODER
    if ( (eDecFmt == AUD_FMT_MPEG) ||
          (eDecFmt == AUD_FMT_AAC) ||
#ifdef CC_AUD_SKYPE_SUPPORT
           (eDecFmt == AUD_FMT_PCM) ||
#endif
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
    // Turn off stream input -- move to beginning of the function,
    //                          because sometimes change format is happened during no HDMI clock, and DSP can't STOP.
    // AUD_StrInputEnable(u1DecId, _arAudDecoder[u1DecId].eStreamFrom, _arAudDecoder[u1DecId].eDecFormat, FALSE, 0);
    if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_HDMI)
    {
        AUD_ClkSetToInternal(u1DecId, AUD_FMT_PCM); //Format is not important here..
    }
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
    if(_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_HDMI)
    {
        AUD_DrvSetFgHDMIParserthread(u1DecId,FALSE);
    }

    if ((u1DecId == AUD_DEC_MAIN) && (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_HDMI) &&
    	(_arParserInfo[u1DecId].eSpdifInfo.u2format == 0x15))
    {
        // HDMI, E-AC-3
        DSP_SetASRCNumeratorQuarter (TRUE);
    }
    else
    {
        DSP_SetASRCNumeratorQuarter (FALSE);
    }
#endif

    #ifdef FIX_AUD_UOP_TIMEOUT
    AUD_WaitUOPLockSema(u1DecId);
    #endif

    if (u1DecId == AUD_DEC_MAIN)
    {
        DSP_SendDspTaskCmd(DSP_STOP);
    }
#ifdef CC_MT5391_AUD_3_DECODER
    else if (u1DecId == AUD_DEC_THIRD)
    {
        DSP_SendDspTaskCmd(DSP_STOP_DEC3);
    }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    else if (u1DecId == AUD_DEC_4TH)
    {
        DSP_SendDspTaskCmd(DSP_STOP_DEC4);
    }
#endif
    else
    {
        DSP_SendDspTaskCmd(DSP_STOP_DEC2);
    }

    AUD_WaitUopCommandDone(u1DecId);

    //MT5387Merge
    // Turn off stream input -- move to beginning of the function,
    //                          because sometimes change format is happened during no HDMI clock, and DSP can't STOP.
    AUD_StrInputEnable(u1DecId, _arAudDecoder[u1DecId].eStreamFrom, _arAudDecoder[u1DecId].eDecFormat, FALSE, 0);

    VERIFY(x_sema_lock(_hSemaLoadCode, X_SEMA_OPTION_WAIT) == OSR_OK);
    _LoadAdspCode(u1DecId, _arAudDecoder[u1DecId].eDecFormat);
    VERIFY(x_sema_unlock(_hSemaLoadCode) == OSR_OK);

    if ((_arAudDecoder[u1DecId].eDecFormat == AUD_FMT_AC3) &&
        (u1DecId <= AUD_DEC_THIRD))
    {
       _FlushAudioFifo(u1DecId);
    }

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT 
        _FlushParserFifo();
#endif   

    if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)||
         (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))
    {
        // Update DSP read pointer and pts
        AUD_ResetDspReadPtr(u1DecId);
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

#ifdef FIX_AUD_UOP_TIMEOUT
    AUD_WaitUOPLockSema(u1DecId);
#endif

    if (u1DecId == AUD_DEC_MAIN)
    {
        DSP_SendDspTaskCmd(DSP_PLAY);
    }
#ifdef CC_MT5391_AUD_3_DECODER
    else if (u1DecId == AUD_DEC_THIRD)
    {
        DSP_SendDspTaskCmd(DSP_PLAY_DEC3);
    }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    else if (u1DecId == AUD_DEC_4TH)
    {
        //Printf("DSP_PLAY_DEC4 #1\n"); //SBCENC_TEST
        DSP_SendDspTaskCmd(DSP_PLAY_DEC4);
    }
#endif
    else
    {
        DSP_SendDspTaskCmd(DSP_PLAY_DEC2);
    }

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
        // Mute amplifier
        ADAC_HdmiUnstableSpkEnable(!fgMute);
    }
    _fgHdmiMute = fgMute;
}

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
        if (eMode == AUD_FMT_UNKNOWN)
        {
             AUD_SPDIF_Output_HDMI_Rx(TRUE);
        }
        else
        {
            _u1HDMIUnDetectCnt = 0;
            eDecStatus = AUD_DECODE_NORMAL;
            AUD_SPDIF_Output_HDMI_Rx(FALSE); // It is set at setting IEC
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
        else if ((eMode == AUD_FMT_AAC) && (_arAudDecoder[AUD_DEC_MAIN].eDecFormat != AUD_FMT_AAC))
        {
             *peDecFmt = AUD_FMT_AAC;
            LOG(3, "<< HDMI detect as AAC >>\n" );
            LOG(3, "################## Detected AAC and -- HDMI Mute! #################\n" );
             i4Ret = TRUE;
        }
        else if ((eMode == AUD_FMT_DTS) && (_arAudDecoder[AUD_DEC_MAIN].eDecFormat != AUD_FMT_DTS))
        {
             *peDecFmt = AUD_FMT_DTS;
            LOG(3, "<< HDMI detect as DTS >>\n" );
            LOG(3, "################## Detected DTS and -- HDMI Mute! #################\n" );
             i4Ret = TRUE;
        }
        else if ((eMode == AUD_FMT_MPEG) &&
                 (_arAudDecoder[AUD_DEC_MAIN].eDecFormat != AUD_FMT_MPEG) &&
                 (_arAudDecoder[AUD_DEC_MAIN].eDecFormat != AUD_FMT_MP3))
        {
             *peDecFmt = AUD_FMT_MPEG;
            LOG(3, "<< HDMI detect as MPEG >>\n" );
             i4Ret = TRUE;
        }
        else if (eMode == AUD_FMT_UNKNOWN)
        {
            LOG(3, "<< HDMI unknown >>\n" );
            _u1HDMIUnDetectCnt++;
#ifndef CC_AUD_HDMI_PARSER_2_0_SUPPORT       
            if (_u1HDMIUnDetectCnt > 5)
#else                
            if ((_u1HDMIUnDetectCnt > 5)||_arParserInfo[AUD_DEC_MAIN]._fgRaw)
#endif
            {
                //if (_arAudDecoder[AUD_DEC_MAIN].eDecFormat != AUD_FMT_AC3)
                {
                    // Default decoder type is AC3 to prevent from noises.
                    *peDecFmt = AUD_FMT_AC3;
                    LOG(3, "<< HDMI unknown >>===> Change to AC3\n" );
                    _u1HDMIUnDetectCnt = 0;
                    i4Ret = TRUE;
                }
            }
        }
    }
    else
    {
        LOG(3, "<< HDMI undetect >>\n" );
        eDecStatus = AUD_DECODE_UNKNOWN;
        _u1HDMIUnDetectCnt ++;
#ifndef CC_AUD_HDMI_PARSER_2_0_SUPPORT       
        if (_u1HDMIUnDetectCnt > 5)
#else                
        if ((_u1HDMIUnDetectCnt > 5)||_arParserInfo[AUD_DEC_MAIN]._fgRaw)
#endif
        {
            if (_arAudDecoder[AUD_DEC_MAIN].eDecFormat != AUD_FMT_AC3)
            {
            // Default decoder type is AC3 to prevent from noises.
            *peDecFmt = AUD_FMT_AC3;
                LOG(3, "<< HDMI undetect >> --------> Change to AC3\n" );
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

void _AudHdmiOnPlayStateHandler(UINT8 u1DecId)
{
    UINT8      u1HdmiUnstableMute = 0;
    BOOL    b_ChkHdmiFmt = TRUE;
    static UINT8 u1ChkHdmicount = 0;
    UINT32 u4Temp;
    AUD_FMT_T  eDecFmt=AUD_FMT_UNKNOWN;
    UNUSED(eDecFmt);

    // ONLY APPLY on MAIN decoder
    if (u1DecId != AUD_DEC_MAIN)
    {
        ASSERT(0);
    }

    //[DTV00081796] change detection period from 200ms to 80ms to avoid noise for PCM->AC3
    if (b_ChkHdmiFmt && ((u1ChkHdmicount%4)==0)) // For hdmi pop noise.
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
            if(_arAudDecoder[u1DecId].eDecFormat != eDecFmt)
            {
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
    }

    u1HdmiUnstableMute = bHDMIAudStatus();

    // Check HDMI unstable case and apply muting
    if (u1HdmiUnstableMute != HDMI_AUD_OK)
    {
        if (_u1HDMIMuteCnt != 0)
        {
            LOG(6, "################## HDMI unstable -- HDMI Mute! #################\n");
        }

        _u1HDMIMuteCnt = (UINT8)(AIN_CHANGE_TYPE_THRESHOLD + u1HdmiUnstableMute);
        _AudHdmiMute(TRUE);
        LOG(7, "HDMI mute count = %d\n", _u1HDMIMuteCnt);
    }

#if 0//remove HMDI clock check becuase is always exits to avoid misdetection
    // Check if HDMI clock exist.
    if (!AUD_IsHdmiClkExist())
    {
        if (_u1HDMIMuteCnt != 0)
        {
           LOG(7, "################## HDMI CLK unstable -- HDMI Mute! #################\n");
        }

        _u1HDMIMuteCnt = (UINT8)(AIN_CHANGE_TYPE_THRESHOLD + u1HdmiUnstableMute);
        _AudHdmiMute(TRUE);

        bHDMIClockDetect();

        LOG(7, "####### HDMI NOT Exist, toggle HDMI clock 1 ######\n");
        LOG(7, "HDMI CLK mute count = %d\n", _u1HDMIMuteCnt);
    }
#endif
    // Polling HDMI sampling rate
	if ((u1ChkHdmicount%4) == 0)
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

    // PCM detect Raw mute
	if ((u1ChkHdmicount%10) == 0)
    {
        if (AUD_Is_PCMDetectRaw())
        {
            if (_u1HDMIMuteCnt == 0)
            {
                _u1HDMIMuteCnt = AIN_CHANGE_TYPE_THRESHOLD;
                _AudHdmiMute(TRUE);
                LOG(5, "XXXX PCM Detect Raw Mute XXXX \n");
            }
            AUD_PCM_Detect_Raw_Mute(FALSE);
        }
    }

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
                u4Temp = (UINT16)DRVCUST_OptGet(eAudioPcmPrebufBankNum);
                if ((_eHdmiSmpRate == FS_32K) && (u4Temp>3))
                {
                    AUD_DspSetPcmPreBuffer(AUD_DEC_MAIN, (u4Temp-3));
                }
                else
                {
                    AUD_DspSetPcmPreBuffer(AUD_DEC_MAIN, u4Temp);
                }

                LOG(3, " \n << Re-start PCM decoder >> \n");
                _ChangeDecFmtOnPlayState(u1DecId);
                u1ChkHdmicount = 0;
            }
            LOG(3, "@@@@@@@@@@@@@@@@@@@@ HDMI UnMute due to counter is 0! @@@@@@@@@@@@@@@@@@@@ \n" );
            _AudHdmiMute(FALSE);
        }
    }

    u1ChkHdmicount =  u1ChkHdmicount+1 ;
}


// *********************************************************************
// Function : void _AudApllMonitorThread(void)
// Description :
// Parameter : none
// Return    : None
// *********************************************************************
static void _AudApllMonitorThread(void* pvArg)
{
    UINT32 u4Event;
    UINT16 u2MsgIdx;
    SIZE_T zMsgSize = sizeof(UINT32);

//sunman_aout
#ifdef BURN_IN_AOUT_TEST //sunman_aout
    static UINT32 u4PrevOutputUnderRun = 0;
    UINT32 u4OutputUnderRun = 0;
    UINT32 u4AoutCrcFail = 0;

    _gu1AoutDramErr=0;
#endif

    UNUSED(pvArg);

    while (1)
    {
        VERIFY(x_msg_q_receive(&u2MsgIdx, &u4Event, &zMsgSize,
        &_hAudApllCmdQueue, 1, X_MSGQ_OPTION_WAIT) == OSR_OK);

#ifndef CC_DISABLE_SMART_PCR
        _AUD_DspAsrcSetSmartVCXO(u4Event);
#endif

#ifdef BURN_IN_AOUT_TEST //sunman_aout
        u4PrevOutputUnderRun = u4ReadDspSram(SRAM_DECODER_OUTPUT_UNDERRUN);
        AUD_WRITE32(REG_AOUT_CRC_CTL, 0x2);
        AUD_WRITE32(REG_AOUT_CRC_CTL, 0x11);
        x_thread_delay(20);
        LOG(3, "\n ===== Aout Dram Burning Test Begin ===== \n");
        while (1)
        {
            u4AoutCrcFail = (AUD_READ32(REG_AOUT_CRC_CTL) & 0x400);
            if (u4AoutCrcFail == 0x400)
            {
                LOG(3," CRC Fail Raised, check underrun ...........\n");
                u4OutputUnderRun = u4ReadDspSram(SRAM_DECODER_OUTPUT_UNDERRUN);
                if (u4OutputUnderRun == u4PrevOutputUnderRun)
                {
                    _gu1AoutDramErr++;
                    LOG(3, "Failed !!!!!!!!!!!!!! Underrun is not increased (%d, %d)...... \n", u4PrevOutputUnderRun, u4OutputUnderRun);
                }
                else
                {
                    LOG(3, "It's ok .. Underrun is increased...... (%d, %d)\n", u4PrevOutputUnderRun, u4OutputUnderRun);
                }
                u4PrevOutputUnderRun = u4OutputUnderRun;
                AUD_WRITE32(REG_AOUT_CRC_CTL, 0x2);
                AUD_WRITE32(REG_AOUT_CRC_CTL, 0x11);
            }
            x_thread_delay(20);
        }
#else
        x_thread_delay(100);
#endif
    }
}


//2008/01/15 added by ling
#ifndef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
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
#endif

 void _AudSetTvSysMask(UINT8 u1DecId, UINT32 u4Mask)
{
     _au4TvSysMask[u1DecId]=u4Mask;
}

#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
   void _AudSetTvAudSysMask(UINT8 u1DecId, UINT32 u4TvMask, UINT32 u4TvAudMask, ISO_3166_COUNT_T country_code)
  {
       _au4TvSysMask[u1DecId]= u4TvMask;
       _au4TvAudSysMask[u1DecId]= u4TvAudMask;
       x_strncpy( (CHAR *) _au4TvSysCountry, (CHAR *) country_code, ISO_3166_COUNT_LEN);
  }
#endif

  UINT32 _AudGetTvSysMask(UINT8 u1DecId)
 {
	return  _au4TvSysMask[u1DecId];
 }





#ifndef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
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
            eAudFmt = AUD_FMT_MTS; // FIXME
            break;
        default:
            eAudFmt = AUD_FMT_PAL;
            LOG(1, "Invalid _AudAtvFmtDetection\n");
    }

    return eAudFmt;
}
#else
 AUD_FMT_T _AudAtvFmtDetection(void)
{
    TV_AUD_SYS_T eTvSys = SV_NONE_DETECTED;
    AUD_FMT_T    eAudFmt;
    AudDecNfyFct   pfAudDecNfy = NULL;
    MW_TV_AUD_SYS_T rAudTvSys,rAudTvSysFromMW;
    UINT8 u1DecState = 0;
    UINT32 u4CandTVSysMask,u4TunerSysMask;
    UINT32 u4AddTVSysMask = 0;      //to remove build warning
    UINT8 u1DecId = AUD_DEC_MAIN;

    // Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy) == TRUE);

    rAudTvSysFromMW.u4TvSysMask = _au4TvSysMask[u1DecId] ; //from _AudSetTvAudSysMask
    rAudTvSysFromMW.u4AudSysMask = _au4TvAudSysMask[u1DecId]; //from _AudSetTvAudSysMask


    if (x_strncmp( (CHAR *)_au4TvSysCountry, "GBR", sizeof("GBR")) == 0)
    {
        //I
        u4CandTVSysMask = AUD_TV_SYS_MASK_I;
    }
    else if (x_strncmp( (CHAR *)_au4TvSysCountry, "RUS", sizeof("RUS")) == 0)
    {
        //DK
        u4CandTVSysMask = AUD_TV_SYS_MASK_D | AUD_TV_SYS_MASK_K;
    }
    else
    {
        //BG
        u4CandTVSysMask = AUD_TV_SYS_MASK_B | AUD_TV_SYS_MASK_G;
    }
    u4TunerSysMask = rAudTvSysFromMW.u4TvSysMask & 0xFFFF;

    // Waiting for analog tuner lock signal ..
    _WaitAtunerLockSignal();    //TODO: check if need to do this every time after we config tuner BW

    UNUSED(AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_ANALOG_TUNER, AUD_FMT_DETECTOR));
    UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, TRUE));
    UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, TRUE));  // for 3 decoder, may send MAIN decoder notify.


    if (u4TunerSysMask == u4CandTVSysMask)
    {
        //MW setting = candidate list, jump to state 1
        u1DecState = 1;
    }

    //check if TV sys is L or M
    if ( (rAudTvSysFromMW.u4TvSysMask & AUD_TV_SYS_MASK_L) == AUD_TV_SYS_MASK_L ||
         (rAudTvSysFromMW.u4TvSysMask & AUD_TV_SYS_MASK_L_PRIME) == AUD_TV_SYS_MASK_L_PRIME)
    {
        rAudTvSys.u4TvSysMask = rAudTvSysFromMW.u4TvSysMask;
        rAudTvSys.u4AudSysMask = rAudTvSysFromMW.u4AudSysMask;
        eTvSys = SV_SECAM_L;
        LOG(1, "MW set _AudAtvFmtDetection = SV_SECAM_L, RETURN!\n");
    }
    else if ( (rAudTvSysFromMW.u4TvSysMask & AUD_TV_SYS_MASK_M) == AUD_TV_SYS_MASK_M)
    {
        rAudTvSys.u4TvSysMask = rAudTvSysFromMW.u4TvSysMask;
        rAudTvSys.u4AudSysMask = rAudTvSysFromMW.u4AudSysMask;
        eTvSys = SV_MTS;
        LOG(1, "MW set _AudAtvFmtDetection = SV_MTS, RETURN!\n");
    }
    else
    {
        //detection loop
        //TODO: MT5365 should not need this loop
        AUD_DspSetDetectDefaultMode(0); // PAL-DK
        DSP_SetDetTvSrcMode(SV_SECAM_DK);
        while (1)
        {
            //config tuner BW
            AtdEU_AudSetSystem(_AudTvSys2Tuner(u4TunerSysMask),0);  //TODO: need new 5365 API


            // Try lock to make sure the semaphore was locked
            UNUSED(x_sema_lock(_hDetectionSyncSema, X_SEMA_OPTION_NOWAIT));
            UNUSED(AUD_DSPCmdPlay(AUD_DEC_MAIN));
            AUD_DetectionSyncSema(TRUE);
            UNUSED(AUD_DSPCmdStop(AUD_DEC_MAIN));

            DSP_GetAtvTvSys(&rAudTvSys);
            eTvSys = DSP_GetAtvTvSystem();
#if 0
            if (((rAudTvSys.u4TvSysMask == 0) && (rAudTvSys.u4AudSysMask == 0)))
            {
                //detector might be stopped before results are notified
                u1DecState = 0xFF;
                LOG(0,"Sound System Auto Detection Failed...\n");
                break;
            }
#endif
            //state 0: check if match MW setting
            if (u1DecState == 0)
            {
                LOG(5,"=====AUTO DETECT STATE 0...\n");
                if ((rAudTvSys.u4TvSysMask & 0xFFFF)== (rAudTvSysFromMW.u4TvSysMask & 0xFFFF))
                {
                    LOG(5,"Detected,match MW setting \n");
                    break;
                }
                LOG(5,"Not Detected\n");
                u1DecState = 1;
                //set tuner to candidate list
                u4TunerSysMask = u4CandTVSysMask;
            }
            //state 1:  check if match candidate list
            else if (u1DecState == 1)
            {
                LOG(5,"=====AUTO DETECT STATE 1...\n");
                if ((rAudTvSys.u4TvSysMask & 0xFFFF) == u4CandTVSysMask )
                {
                    LOG(5,"Detected,match candidate list  \n");
                    break;
                }
                else
                {
                    //add in additional list
                    u4AddTVSysMask = rAudTvSys.u4TvSysMask;
                    //set tuner to additional list
                    u4TunerSysMask = u4AddTVSysMask;
                    LOG(5,"Not Detected\n");
                }
                u1DecState = 3;
            }
            //state 3:  check if match additional list
            else if (u1DecState == 3)
            {
                LOG(5,"=====AUTO DETECT STATE 3...\n");
                if ((rAudTvSys.u4TvSysMask != u4AddTVSysMask) || (eTvSys < SV_A2_BG))       //block TV sys not in EU
                {
                    //cannot find valid system, use setting from MW
                    u1DecState = 0xFF;
                    LOG(0,"cannot find valid system\n");
                }
                else
                {
                    LOG(5,"Detected,match additional list  \n");
                }
                break;

            }


        }
        if (u1DecState == 0xFF)
        {
            //cannot find valid system, use setting from MW
            eTvSys = _AudGetTvSysFormat(rAudTvSysFromMW);
            rAudTvSys.u4TvSysMask = rAudTvSysFromMW.u4TvSysMask;
            rAudTvSys.u4AudSysMask = rAudTvSysFromMW.u4AudSysMask;
        }
        AtdEU_AudSetSystem(_AudTvSys2Tuner(rAudTvSys.u4TvSysMask),1);    //TODO: need new 5365 API
    }

    UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, FALSE));
    UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, FALSE));



    if (pfAudDecNfy != NULL)
    {
        pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_TV_SYS_DETECTED,
                        rAudTvSys.u4TvSysMask,rAudTvSys.u4AudSysMask);
    }


    LOG(5,"Set TV system = %d\n",eTvSys);
    _au4TvSysMask[u1DecId]=rAudTvSys.u4TvSysMask;
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
        case SV_SECAM_L:
            eAudFmt = AUD_FMT_PAL;
            break;
        case SV_MTS:
            eAudFmt = AUD_FMT_MTS;
            break;
        case SV_NONE_DETECTED:
            eAudFmt = AUD_FMT_UNKNOWN;
            break;
        default:
            eAudFmt = AUD_FMT_PAL;
            LOG(1, "Invalid _AudAtvFmtDetection\n");
    }

    return eAudFmt;
}


TV_AUD_SYS_T _AudGetTvSysFormat(MW_TV_AUD_SYS_T rAudTvSysFromMW)
{
    TV_AUD_SYS_T eTvSys = SV_PAL_BG;
    if (((rAudTvSysFromMW.u4AudSysMask & AUD_SYS_MASK_FM_A2) == AUD_SYS_MASK_FM_A2) ||
        ((rAudTvSysFromMW.u4AudSysMask & AUD_SYS_MASK_FM_A2_DK1) == AUD_SYS_MASK_FM_A2_DK1) ||
        ((rAudTvSysFromMW.u4AudSysMask & AUD_SYS_MASK_FM_A2_DK2) == AUD_SYS_MASK_FM_A2_DK2))
    {
        if (((rAudTvSysFromMW.u4TvSysMask & AUD_TV_SYS_MASK_B) == AUD_TV_SYS_MASK_B) &&
            ((rAudTvSysFromMW.u4TvSysMask & AUD_TV_SYS_MASK_G) == AUD_TV_SYS_MASK_G))
        {
            eTvSys =  SV_A2_BG;
        }
        else if (((rAudTvSysFromMW.u4AudSysMask & AUD_SYS_MASK_FM_A2) == AUD_SYS_MASK_FM_A2))
        {
            eTvSys = SV_A2_DK3;
        }
        else if (((rAudTvSysFromMW.u4AudSysMask & AUD_SYS_MASK_FM_A2_DK1) == AUD_SYS_MASK_FM_A2_DK1))
        {
            eTvSys = SV_A2_DK1;
        }
        else if (((rAudTvSysFromMW.u4AudSysMask & AUD_SYS_MASK_FM_A2_DK2) == AUD_SYS_MASK_FM_A2_DK2))
        {
            eTvSys = SV_A2_DK2;
        }
    }
    else if (((rAudTvSysFromMW.u4AudSysMask & AUD_SYS_MASK_FM_MONO) == AUD_SYS_MASK_FM_MONO) ||
        ((rAudTvSysFromMW.u4AudSysMask & AUD_SYS_MASK_AM) == AUD_SYS_MASK_AM) ||
        ((rAudTvSysFromMW.u4AudSysMask & AUD_SYS_MASK_NICAM) == AUD_SYS_MASK_NICAM))
    {
        if ((rAudTvSysFromMW.u4TvSysMask & AUD_TV_SYS_MASK_I) == AUD_TV_SYS_MASK_I)
        {
            eTvSys =  SV_PAL_I;
        }
        else if (((rAudTvSysFromMW.u4TvSysMask & AUD_TV_SYS_MASK_D) == AUD_TV_SYS_MASK_D) &&
            ((rAudTvSysFromMW.u4TvSysMask & AUD_TV_SYS_MASK_K) == AUD_TV_SYS_MASK_K))
        {
            eTvSys = SV_PAL_DK;
        }
        else if (((rAudTvSysFromMW.u4TvSysMask & AUD_TV_SYS_MASK_B) == AUD_TV_SYS_MASK_B) &&
                 ((rAudTvSysFromMW.u4TvSysMask & AUD_TV_SYS_MASK_G) == AUD_TV_SYS_MASK_G))
        {
            eTvSys = SV_PAL_BG;
        }
        else if (((rAudTvSysFromMW.u4TvSysMask & AUD_TV_SYS_MASK_L) == AUD_TV_SYS_MASK_L))
        {
            eTvSys = SV_SECAM_L;
        }
        else if (((rAudTvSysFromMW.u4TvSysMask & AUD_TV_SYS_MASK_L_PRIME) == AUD_TV_SYS_MASK_L_PRIME))
        {
            eTvSys = SV_SECAM_L_PLUM;
        }
    }
    else
    {
        LOG(0,"MW set sound system error, Set system as PAL_BG\n");
        eTvSys = SV_PAL_BG;     //TODO
    }
    return eTvSys;
}

UINT8 _AudTvSys2Tuner(UINT32 u4TvSysMask)
{
    if (((u4TvSysMask & AUD_TV_SYS_MASK_B) == AUD_TV_SYS_MASK_B) &&
        ((u4TvSysMask & AUD_TV_SYS_MASK_G) == AUD_TV_SYS_MASK_G))
    {
        LOG(5,"Set tuner as MOD_PAL_BG\n");
        return(MOD_PAL_BG);
    }
    else if (((u4TvSysMask & AUD_TV_SYS_MASK_D) == AUD_TV_SYS_MASK_D) &&
        ((u4TvSysMask & AUD_TV_SYS_MASK_K) == AUD_TV_SYS_MASK_K))
    {
        LOG(5,"Set tuner as MOD_PAL_DK\n");
        return(MOD_PAL_DK);

    }
    else if (((u4TvSysMask & AUD_TV_SYS_MASK_I) == AUD_TV_SYS_MASK_I))
    {
        LOG(5,"Set tuner as MOD_PAL_I\n");
        return(MOD_PAL_I);
    }
    else
    {
        LOG(0,"Error Set tuner mode, Set tuner as MOD_PAL_BG\n");
        return(MOD_PAL_BG);     //TODO
    }
}
#endif

#ifdef LINUX_TURNKEY_SOLUTION
BOOL skip_create_feed_stream = FALSE;
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
    UINT16 u2FeedStreamWaitCnt =0; // Feed_and_Auddrv Thread sync

    if (pvArg != NULL)
    {
        u1DecId = *(UINT8 *)pvArg;
    }

    AUD_DEC_ID_VALIDATE(u1DecId);

    while (1)
    {
        switch (_arAudDrvState[u1DecId])
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
            else if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_FEEDER))
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
                    AUD_GetAudioPts(u1DecId, &u4OldPts);
                    AUD_MMQueueSyncInfo(u1DecId,0,0,TRUE); //Reset MM Sync info
                    AUD_MMQueueTickInfo(u1DecId,0,0,TRUE); //Reset MM Sync info
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
#if 0 //def CC_ENABLE_AV_SYNC
                if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_LINE_IN) ||
                    ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) &&
                    (u1DecId == AUD_DEC_MAIN)))
                {
                    _fgAVSyncThreadGo = TRUE;
                }
#endif
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
#if 0//def CC_ENABLE_AV_SYNC
                if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_LINE_IN) ||
                    ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) &&
                    (u1DecId == AUD_DEC_MAIN)))
                {
                    _fgAVSyncThreadGo = FALSE;
                }
#endif
                _ChangeAudioState(u1DecId, eNewState);
                break;
            }
            break;

            case AUD_WAIT_PLAY:
                _MT5365_PopMute(FALSE);
                if (!fgWaitPlayIssuePause)
                {
                    _arAudDecoder[u1DecId].eDecState = AUD_DEC_PLAYING;
                    //For DTV00080543==>move clear pes count to _AudConnect
                    if (_arAudDecoder[u1DecId].eStreamFrom != AUD_STREAM_FROM_MULTI_MEDIA)
                    {
                        _arAudDecoder[u1DecId].u4ReceivePesCount = 0;
                    }
                    _arAudDecoder[u1DecId].u4ReceiveValidPesCount = 0;
                    //_afgFirstIPicArrived[u1DecId] = FALSE;
                    _aeHDMIDecStatus[u1DecId] = AUD_DECODE_UNKNOWN;
                    _fgAoutTimeoutEnable = FALSE;
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
						while (!_fgFeedStrmFinshed)   // Feed_and_Auddrv Thread sync
						{
							x_thread_delay(10);       // If FeedStream is not exit yet, should wait. Or create thresd will fail.
							u2FeedStreamWaitCnt ++;
							if (u2FeedStreamWaitCnt > 10) break; //max watiung is 100ms.
						}
                    }

                    if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
                    {
                        _fgMMSet[u1DecId]=FALSE;
                        //For DTV00080543==>move clear EOS Flag to _AudConnect
                        //_AudClearEOSFlag(u1DecId);
                    }

#ifdef CC_MT5391_AUD_3_DECODER
                    if ((u1DecId == AUD_DEC_AUX) && (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER))
#else
                    if ((u1DecId == AUD_DEC_MAIN) && (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER))
#endif
                    {
                        _AUD_DspAsrcResetSmartVCXO();
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
                        if (//(_arAudDecoder[u1DecId].eStreamFrom != AUD_STREAM_FROM_MULTI_MEDIA) &&
                            (_arAudDecoder[u1DecId].eStreamFrom != AUD_STREAM_FROM_FEEDER) &&
                            (_arAudDecoder[u1DecId].eStreamFrom != AUD_STREAM_FROM_MEMORY) &&
                            (_arAudDecoder[u1DecId].eDecFormat != AUD_FMT_FMRDO) )
                        {
                        	if((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) && (_fgMMAudioOnly))
                        	{
                        		_fgVdpReady = TRUE;
							}
							else
							{
                            	AUD_DspDecPlayMuteEnable(AUD_DEC_MAIN, TRUE);
                            	LOG(3,"[SRM_AVSYNC] ########### For AV sync , MUTE Audio !!!! ######\n");
                            	_fgVdpReady = FALSE;
							}
                        }
                        else
                        {
                            _fgVdpReady = TRUE;
                        }
                        if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
                        {
                            _fgSetChannelDelay = TRUE;
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
                    #if 0 //This is DTV00134479, it's for 5360 + 8292 isse.
                    if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_LINE_IN)
                    {
                        x_thread_delay(300);
                    }
                    #endif

                    AUD_StrInputEnable(u1DecId, _arAudDecoder[u1DecId].eStreamFrom, _arAudDecoder[u1DecId].eDecFormat, TRUE, _arAudDecoder[u1DecId].u4ReceivePesCount);
                    //koro: increase dmx interrupt for AAC
                    if ((_arAudDecoder[AUD_DEC_AUX].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) &&
                        (_arAudDecoder[AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) &&
                        ((_arAudDecoder[AUD_DEC_AUX].eDecFormat == AUD_FMT_AAC) || (_arAudDecoder[AUD_DEC_THIRD].eDecFormat == AUD_FMT_AAC)))
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
                    LOG(1,"######### NEED SYNC: Aud dec(%d) wait AVSYNC CMD to real play\n",u1DecId);
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

                    _fgWaitAvSync[u1DecId] = TRUE;
                    //if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)||
                    //    (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))
                    if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
                    {
#ifdef  CC_AUD_DDBANNER
                        _AUD_DDBannerEnable(u1DecId, TRUE); //Andrew : For DD test only
#endif
#if (defined(CC_AUD_SUPPORT_MS10) && defined(CC_MT5391_AUD_3_DECODER))
                        if (u1DecId == AUD_DEC_THIRD)
                        {
                            _AudWaitEventAD(&u4Event, FALSE);
                        }
                        else
#endif
                        {
                            // DTV must have Avsync command
                            // Light: Since DTV needs to wait for 1st I frame, it takes longer time to wait for tne event.
                            if (!_AudWaitEvent(u1DecId, &u4Event, FALSE, AUD_EVENT_TIMEOUT_DTV))
                            {
                                vDspSetFifoReadPtr(u1DecId, _arAudDecoder[u1DecId].rCurrentPes.u4Wp);
                                DSP_SetStartPtsToShm(u1DecId, (UINT32)(_arAudDecoder[u1DecId].rCurrentPes.u4Pts & 0xffffffff), _arAudDecoder[u1DecId].rCurrentPes.u4Wp);
                                u4Event = (UINT32)AUD_CMD_AVSYNC;
                                LOG(7, "NO AV sync message retrieve in DTV\n");
                            }
                         }
                    }
                    else if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
                    {
#ifdef  CC_AUD_DDBANNER
                        _AUD_DDBannerEnable(u1DecId, TRUE); //Andrew : For DD test only
#endif
                        //For DTV00080543==>aud_drv self trigger AUD_CMD_AVSYNC
                        // to avoid PES has been sent done for small size file
                        if (((_arAudDecoder[u1DecId].u4ReceivePesCount != 0) &&
                             (_arAudDecoder[u1DecId].eSynMode == AV_SYNC_AUDIO_MASTER)) ||
                            AUD_Is_MMDecFmtScramble(u1DecId))
                        {
                            vDspSetFifoReadPtr(u1DecId, DSP_INTERNAL_ADDR(u4GetAFIFOStart(u1DecId)));
                            DSP_SetStartPtsToShm(u1DecId, _arAudDecoder[u1DecId].u4StartPts, DSP_INTERNAL_ADDR(u4GetAFIFOStart(u1DecId)));
                            VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_AVSYNC));
                            LOG(7, "===>aud_drv self trigger AUD_CMD_AVSYNC in  MM\n");
                        }
                        // MM must have Avsync command
                        // Laihui: MM also need to wait data until timeout, longer then DTV. Cause file maybe a lot of error data.
                        if (!_AudWaitEvent(u1DecId, &u4Event, FALSE, AUD_EVENT_TIMEOUT_MM))
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
                    else if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER)
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
                      #ifdef FIX_AUD_UOP_TIMEOUT
                        AUD_WaitUOPLockSema(u1DecId);
                      #endif

                        if (u1DecId == AUD_DEC_MAIN)
                        {
                            DSP_SendDspTaskCmd(DSP_PLAY);
                        }
#ifdef CC_MT5391_AUD_3_DECODER
                        else if (u1DecId == AUD_DEC_THIRD)
                        {
                            DSP_SendDspTaskCmd(DSP_PLAY_DEC3);
                        }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
                        else if (u1DecId == AUD_DEC_4TH)
                        {
                            DSP_SendDspTaskCmd(DSP_PLAY_DEC4);
                        }
#endif
                        else
                        {
                            DSP_SendDspTaskCmd(DSP_PLAY_DEC2);
                        }

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
                	    (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))
                    {
                        UINT32 u4FifoStart;
                        UINT32 u4FifoEnd;

                        UNUSED(_AUD_GetAudFifo(u1DecId, &u4FifoStart, &u4FifoEnd));
                        vDspSetFifoReadPtr(u1DecId, u4FifoStart);
                    }

                #ifdef FIX_AUD_UOP_TIMEOUT
                    AUD_WaitUOPLockSema(u1DecId);
                #endif

                    if (u1DecId == AUD_DEC_MAIN)
                    {
                        DSP_SendDspTaskCmd(DSP_PLAY);
                        _afgIssuePlayComToDsp[AUD_DEC_MAIN] = TRUE;
                        LOG(1, " DSP_SendDspTaskCmd(DSP_PLAY_DEC1);\n");
                    }
#ifdef CC_MT5391_AUD_3_DECODER
                    else if (u1DecId == AUD_DEC_THIRD)
                    {
                        DSP_SendDspTaskCmd(DSP_PLAY_DEC3);
                        _afgIssuePlayComToDsp[AUD_DEC_THIRD] = TRUE;
                        LOG(1, " DSP_SendDspTaskCmd(DSP_PLAY_DEC3);\n");
                    }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
                    else if (u1DecId == AUD_DEC_4TH)
                    {
                        //Printf("DSP_PLAY_DEC4: #3\n"); //SBCENC_TEST
                        DSP_SendDspTaskCmd(DSP_PLAY_DEC4);
                        _afgIssuePlayComToDsp[AUD_DEC_4TH] = TRUE;
                        LOG(1, " DSP_SendDspTaskCmd(DSP_PLAY_DEC4);\n");
                    }
#endif
                    else
                    {
                        DSP_SendDspTaskCmd(DSP_PLAY_DEC2);
                        _afgIssuePlayComToDsp[AUD_DEC_AUX] = TRUE;
                        LOG(1, " DSP_SendDspTaskCmd(DSP_PLAY_DEC2);\n");
                    }

                    AUD_WaitUopCommandDone(u1DecId);
                    LOG(3, "Aud decoder %d Play\n",u1DecId);

                    // Create the feed stream thread for audio clip play
                    if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MEMORY)
                    {
                    #ifdef LINUX_TURNKEY_SOLUTION
                        if (!skip_create_feed_stream)
                        {
                    #endif
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
                    #ifdef LINUX_TURNKEY_SOLUTION
                            Printf("Create FeedStreamThread\n");
                        }
                    #endif
                    }
                    else if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_FEEDER)
                    {
                        static BOOL fgThreadCreate = FALSE;

                        if (!fgThreadCreate)
                        {
                            fgThreadCreate = TRUE;

                            VERIFY(x_sema_create(&_hSemaFeederThreadWait, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

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
                                                             // Create the copy data thread for data from AFIFO3
                _ChangeAudioState(u1DecId, AUD_ON_PLAY);
                /* Release Play command API waiting semaphore */
                AUD_CommandDone(u1DecId, AUD_CMD_PLAY, AUD_OK);
            }
#ifdef CC_ENABLE_AV_SYNC
            if (u1DecId == AUD_DEC_MAIN)
            {
                if ((_arAudDecoder[u1DecId].eDecFormat != AUD_FMT_DETECTOR) && (!_fgVdpReady)&&(_arAudDrvState[AUD_DEC_MAIN] == AUD_ON_PLAY))
                {
                    _WaitVdpReady();
                }
                else
                {
                    //Unmute audio, just in case there is nobody call unmute
                    AUD_DspDecPlayMuteEnable(AUD_DEC_MAIN, FALSE);
                    LOG(3,"[SRM_AVSYNC] ########### No need to Wait VdpReady ........\n");
                }
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
            #ifdef FIX_AUD_UOP_TIMEOUT
            AUD_WaitUOPLockSema(u1DecId);
            #endif

            if (u1DecId == AUD_DEC_MAIN)
            {
                DSP_SendDspTaskCmd(DSP_STOP);
            }
#ifdef CC_MT5391_AUD_3_DECODER
            else if (u1DecId == AUD_DEC_THIRD)
            {
                DSP_SendDspTaskCmd(DSP_STOP_DEC3);
            }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
            else if (u1DecId == AUD_DEC_4TH)
            {
                DSP_SendDspTaskCmd(DSP_STOP_DEC4);
            }
#endif
            else
            {
                DSP_SendDspTaskCmd(DSP_STOP_DEC2);
            }

            AUD_WaitUopCommandDone(u1DecId);

            // Turn off stream input
            AUD_StrInputEnable(u1DecId, _arAudDecoder[u1DecId].eStreamFrom, _arAudDecoder[u1DecId].eDecFormat, FALSE, 0);

            VERIFY(x_sema_lock(_hSemaLoadCode, X_SEMA_OPTION_WAIT) == OSR_OK);
            if ((u1DecId == AUD_DEC_AUX) && (_guAudDspTrigAtvFmtchg))
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
            if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) ||
                (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))
            {
                AUD_ResetDspReadPtr(u1DecId);
                DSP_SetStartPtsToShm(u1DecId, (UINT32)(_arAudPes[u1DecId].u4Pts & 0xffffffff), _arAudPes[u1DecId].u4Wp);
            }

            // Setup individual decoder setting by source
            _DecoderInitSetup(u1DecId, _arAudDecoder[u1DecId].eStreamFrom, _arAudDecoder[u1DecId].eDecFormat);

            // Turn on stream input
            AUD_StrInputEnable(u1DecId, _arAudDecoder[u1DecId].eStreamFrom, _arAudDecoder[u1DecId].eDecFormat, TRUE, 0);

            #ifdef FIX_AUD_UOP_TIMEOUT
            AUD_WaitUOPLockSema(u1DecId);
            #endif

            if (u1DecId == AUD_DEC_MAIN)
            {
                DSP_SendDspTaskCmd(DSP_PLAY);
            }
#ifdef CC_MT5391_AUD_3_DECODER
            else if (u1DecId == AUD_DEC_THIRD)
            {
                DSP_SendDspTaskCmd(DSP_PLAY_DEC3);
            }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
            else if (u1DecId == AUD_DEC_4TH)
            {
                DSP_SendDspTaskCmd(DSP_PLAY_DEC4);
            }
#endif
            else
            {
                DSP_SendDspTaskCmd(DSP_PLAY_DEC2);
            }

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

            #ifdef FIX_AUD_UOP_TIMEOUT
            AUD_WaitUOPLockSema(u1DecId);
            #endif

            if (u1DecId == AUD_DEC_MAIN)
            {
                DSP_SendDspTaskCmd(DSP_PAUSE);
            }
#ifdef CC_MT5391_AUD_3_DECODER
            else if (u1DecId == AUD_DEC_THIRD)
            {
                DSP_SendDspTaskCmd(DSP_PAUSE_DEC3);
            }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
            else if (u1DecId == AUD_DEC_4TH)
            {
                DSP_SendDspTaskCmd(DSP_PAUSE_DEC4);
            }
#endif
            else
            {
                DSP_SendDspTaskCmd(DSP_PAUSE_DEC2);
            }

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

                #ifdef FIX_AUD_UOP_TIMEOUT
                AUD_WaitUOPLockSema(u1DecId);
                #endif

                if (u1DecId == AUD_DEC_MAIN)
                {
                    DSP_SendDspTaskCmd(DSP_RESUME);
                }
#ifdef CC_MT5391_AUD_3_DECODER
                else if (u1DecId == AUD_DEC_THIRD)
                {
                    DSP_SendDspTaskCmd(DSP_RESUME_DEC3);
                }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
                else if (u1DecId == AUD_DEC_4TH)
                {
                    DSP_SendDspTaskCmd(DSP_RESUME_DEC4);
                }
#endif
                else
                {
                    DSP_SendDspTaskCmd(DSP_RESUME_DEC2);
                }

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
            _fgWaitAvSync[u1DecId] = FALSE;
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
                DSP_SetLineInLrInverse(u1DecId,FALSE);
            }

            if (u1DecId == AUD_DEC_AUX)
            {
                _AUD_DspStopMuteEnable(AUD_DEC_AUX, TRUE);
                x_thread_delay(10); // Wait for mute finished
            }

            if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_HDMI)
            {
                //turn off source first, because HDMI may loss clock
                //AUD_StrInputEnable(u1DecId, _arAudDecoder[u1DecId].eStreamFrom, _arAudDecoder[u1DecId].eDecFormat, FALSE, 0);
                if (!AUD_IsHdmiClkExist())
                {
                    AUD_ClkSetToInternal(u1DecId, AUD_FMT_PCM); //Format is not important here..
                }
            }

            #ifdef FIX_AUD_UOP_TIMEOUT
            AUD_WaitUOPLockSema(u1DecId);
            #endif

            if (u1DecId == AUD_DEC_MAIN)
            {
                DSP_SendDspTaskCmd(DSP_STOP);
            }
#ifdef CC_MT5391_AUD_3_DECODER
            else if (u1DecId == AUD_DEC_THIRD)
            {
                DSP_SendDspTaskCmd(DSP_STOP_DEC3);
            }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
            else if (u1DecId == AUD_DEC_4TH)
            {
                DSP_SendDspTaskCmd(DSP_STOP_DEC4);
            }
#endif
            else
            {
                DSP_SendDspTaskCmd(DSP_STOP_DEC2);
            }

            AUD_WaitUopCommandDone(u1DecId);
            _FlushAudioFifo(u1DecId); // dirty fix for parser cann't reset single channel write pointer
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT 
            _FlushParserFifo();
#endif 
            {
                AUD_DEC_CMD_T eDecCmd;
                UINT32 u4CmdResult = AUD_OK;

                eDecCmd = (_arAudDrvState[u1DecId] == AUD_WAIT_STOP) ?
                    AUD_CMD_STOP : AUD_CMD_STOPSTREAM;
                u4CmdResult = (_arAudDrvState[u1DecId] == AUD_WAIT_STOP) ?
                        AUD_OK : _u4FeedStreamResult;

                _ChangeAudioState(u1DecId, AUD_IDLE);
                AUD_CommandDone(u1DecId, eDecCmd, u4CmdResult);
            }
            // Terminate the feed stream thread
            if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MEMORY)
            {
                VERIFY(x_sema_unlock(_hFeedStreamSyncSema) == OSR_OK);
                _hAudFeedStrThread = NULL_HANDLE;
            }

            // Turn off stream input
            AUD_StrInputEnable(u1DecId, _arAudDecoder[u1DecId].eStreamFrom, _arAudDecoder[u1DecId].eDecFormat, FALSE, 0);

            if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_HDMI)
            {
                _AudHdmiMute(FALSE);
                _u1HDMIMuteCnt = 0; // Reset mute counter
            }
                        
#ifdef LINUX_TURNKEY_SOLUTION
            // Set next file flag
            if (_hAudDataDumpThreadState[u1DecId] == 1)
            {
                _fgAudDataDumpNextFile[u1DecId] = TRUE;
            }
#endif
            
            // Clear video scrambled flag
            AUD_VideoScrambled(FALSE);
            
            //_ChangeAudioState(u1DecId, AUD_IDLE);
#ifndef CC_AUD_SUPPORT_MS10
            _AUD_DspSetDDCOEnable(FALSE);
#endif
			//restroe Main decoder volume when AD disable
			if ((u1DecId == AUD_DEC_THIRD) && (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER))
			{
				AUD_DspAdFadeVolumeSet(0);
				AUD_DspAdFadeVolumeUpdate(AUD_DEC_MAIN, AUD_CH_ALL);
				AUD_DspAdPanVolumeSet(AUD_DEC_THIRD, AUD_CH_FRONT_RIGHT, 0);
				AUD_DspAdPanVolumeSet(AUD_DEC_THIRD, AUD_CH_FRONT_LEFT, 0);
				AUD_DspAdPanVolumeUpdate(AUD_DEC_THIRD, AUD_CH_FRONT_RIGHT);
				AUD_DspAdPanVolumeUpdate(AUD_DEC_THIRD, AUD_CH_FRONT_LEFT);
			}

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
    }//end of _fgDrvThreadGo    //lint -e{818}
}

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

void _AUD_AtvAvInitialSync(BOOL fgStable)
{
#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0521
    if ((_arAudDecoder[AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) && (fgStable))
    {
        VERIFY(AUD_DRVCmd(AUD_DEC_MAIN, AUD_CMD_AVSYNC));
    }
#else
    if ((_arAudDecoder[AUD_DEC_AUX].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) && (fgStable))
    {
        VERIFY(AUD_DRVCmd(AUD_DEC_AUX, AUD_CMD_AVSYNC));
        VERIFY(AUD_DRVCmd(AUD_DEC_MAIN, AUD_CMD_AVSYNC));
    }
#endif

    LOG(1,"#### GOT _AUD_AtvAvInitialSync######\n");

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
    _fgAudOutReady[u1DecId] = FALSE;
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
        _fgAudOutReady[u1DecId] = TRUE;
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
    _au4LastPesCnt[u1DecId] = _au4PesCnt[u1DecId];

#if 0
    LOG(5, " $$$$$$ Lock (%d) PesLock (%d), Cnt = %d  $$$$$$\n",
             _afgDtvLock[u1DecId], _au4PesCnt[u1DecId], _au4FrameCnt[u1DecId]);
#endif


#ifdef DATA_DISC_WMA_SUPPORT
    if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
        ((_arAudDecoder[u1DecId].eDecFormat == AUD_FMT_WMA) || (_arAudDecoder[u1DecId].eDecFormat == AUD_FMT_WMAPRO) ) &&
        (DSP_GetWmaErrorCode() != 0))		//0 means normal stop
    {
        //WMA goes here because error
        eDecStatus = AUD_DECODE_NOT_SUPPORT;
    }
    else if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
        (_arAudDecoder[u1DecId].eDecFormat == AUD_FMT_AAC) &&
        (_afgDtvPesLock[u1DecId]) &&
        (_au1NoFrameCntIncrease[u1DecId] > MM_AAC_CHECK_PROFILE_THREASHOLD) &&
        (!_fgAudOutReady[u1DecId]))
    {
        //AAC profile not support
        eDecStatus = AUD_DECODE_NOT_SUPPORT;
    }
    else if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
         (_arAudDecoder[u1DecId].eDecFormat == AUD_FMT_AC3) &&
         (_afgDtvPesLock[u1DecId]) &&
         (_au1NoFrameCntIncrease[u1DecId] > MM_AC3_CHECK_PROFILE_THREASHOLD) &&
         (!DSP_IsSupportEAC3()) && (!DSP_GetAoutEnable(u1DecId)))
    {
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
              (_arAudDecoder[u1DecId].eDecFormat == AUD_FMT_MP3) ||
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
#if 1///def CC_VORBIS_SUPPORT ///CC_VORBIS_SUPPORT_ERR
    else if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
        (_arAudDecoder[u1DecId].eDecFormat == AUD_FMT_VORBISDEC) &&
        (DSP_GetVorbisErrorCode() != 0))
    {
        ///LOG(0,"@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n.");
        //Vorbis goes here because header parsing error
        eDecStatus = AUD_DECODE_NOT_SUPPORT;
    }
#endif
    else
    {
        if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
            (_arAudDecoder[u1DecId].eSynMode != AV_SYNC_FREE_RUN))  //when free run, dtvonplaythread may turn on before PES is sent
        {
#ifdef CC_AUD_SKYPE_SUPPORT
            AUD_DrvMMNoDataNotify(u1DecId);
#else
            AUD_DrvMMNoDataNotify(AUD_DEC_MAIN);
#endif
        }
        eDecStatus = AUD_DECODE_NO_DATA;
    }

    // Notify middleware decoder status
    VERIFY(x_sema_lock(_arAudDrvStateSema[u1DecId], X_SEMA_OPTION_WAIT) == OSR_OK);

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

    VERIFY(x_sema_unlock(_arAudDrvStateSema[u1DecId]) == OSR_OK);

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
        pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_MM_EOS, (UINT32)(_arAudDecoder[u1DecId].rCurrentPes.fgForward), 0);
        LOG(5, "MM send EOS notify done, decoder_id = %d, fgForward = %d\n", u1DecId, _arAudDecoder[u1DecId].rCurrentPes.fgForward);
    }
}

#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0521  -- 2
static void _AudDtvOnPlayThread(void* pvArg)
{
    UNUSED(pvArg);

    while (1)
    {
        if (_afgDtvOnPlayThreadGo[AUD_DEC_MAIN] || _afgDtvOnPlayThreadGo[AUD_DEC_AUX])
        {
            UINT8 u1DecId = AUD_DEC_MAIN;

            // Update main decoder
            u1DecId = AUD_DEC_MAIN;

            #if 0//def CC_ENABLE_AV_SYNC
            AUD_UnMuteWithVideo();
            #endif

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
                        LOG(7, "1. Buffer pointer range checking...\n");
                    }
                }
                // DTV lock check
                _DtvLockCheck(u1DecId);
                AUD_DrvLogStcDiff(u1DecId);

                if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))
                {
                    if (_arAudDecoder[u1DecId].rCurrentPes.fgEOS == TRUE)
                    {
                        if ((AUD_IsFifoEmpty(u1DecId)))
                        {
                            //for MM auto stop. we need to stop dsp once FIFO emtpy true.. 20111208
                            LOG(5, "Aud FIFO empty driver auto stop decoder(%d) \n",u1DecId);
                            AUD_WaitDspFlush(u1DecId);
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
                    AUD_DrvCheckSpeedTime(u1DecId);
                }
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
                        LOG(7, "2. Buffer pointer range checking...\n");
                    }
                }
                // DTV lock check
                _DtvLockCheck(u1DecId);
                AUD_DrvLogStcDiff(u1DecId);
            }
        }

        x_thread_delay(AUD_PTR_UPDATE_TIMEOUT);
        //LOG(1, "  ############################ _AudDtvOnPlayThread exit ####################\n");
    }
}
#else
#ifndef CC_AUD_SKYPE_SUPPORT
static void _AudDtvOnPlayThread(void* pvArg)
{
    UNUSED(pvArg);

    while (1)
    {
        if (_afgDtvOnPlayThreadGo[AUD_DEC_AUX] || _afgDtvOnPlayThreadGo[AUD_DEC_THIRD] || _afgDtvOnPlayThreadGo[AUD_DEC_MAIN])
        {
            UINT8 u1DecId = AUD_DEC_AUX;

            #if 0 //def CC_ENABLE_AV_SYNC
            AUD_UnMuteWithVideo();
            #endif

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
                        LOG(7, "3. Buffer pointer range checking...\n");
                    }
                }
                // DTV lock check
                _DtvLockCheck(u1DecId);
                AUD_DrvLogStcDiff(u1DecId);

                if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))
                {
                    if (_arAudDecoder[u1DecId].rCurrentPes.fgEOS == TRUE)
                    {
                        if ((AUD_IsFifoEmpty(u1DecId)))
                        {
                            //for MM auto stop. we need to stop dsp once FIFO emtpy true.. 20111208
                            LOG(5, "Aud FIFO empty driver auto stop decoder(%d) \n",u1DecId);
                            AUD_WaitDspFlush(u1DecId);
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
                    AUD_DrvCheckSpeedTime(u1DecId);
                }
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
                        LOG(5, "4. Buffer pointer range checking...\n");
                    }
                }
                // DTV lock check
                _DtvLockCheck(u1DecId);
                AUD_DrvLogStcDiff(u1DecId);

                // If AD played normally, update main decoder "master volume" and "channel volume"
                if (_bPanFadeUpdateEn == TRUE)
                {
                    //fade
                    AUD_DspAdFadeVolumeUpdate(AUD_DEC_MAIN, AUD_CH_ALL);

                    //pan
                    AUD_DspAdPanVolumeUpdate(AUD_DEC_THIRD, AUD_CH_FRONT_RIGHT);
                    AUD_DspAdPanVolumeUpdate(AUD_DEC_THIRD, AUD_CH_FRONT_LEFT);
                }
            }
            else
            {
                AUD_DspAdFadeVolumeSet(0);
                AUD_DspAdFadeVolumeUpdate(AUD_DEC_MAIN, AUD_CH_ALL);
                AUD_DspAdPanVolumeSet(AUD_DEC_THIRD, AUD_CH_FRONT_RIGHT, 0);
                AUD_DspAdPanVolumeSet(AUD_DEC_THIRD, AUD_CH_FRONT_LEFT, 0);
                AUD_DspAdPanVolumeUpdate(AUD_DEC_THIRD, AUD_CH_FRONT_RIGHT);
                AUD_DspAdPanVolumeUpdate(AUD_DEC_THIRD, AUD_CH_FRONT_LEFT);
            }
        }

        x_thread_delay(AUD_PTR_UPDATE_TIMEOUT);
        //LOG(1, "  ############################ _AudDtvOnPlayThread exit ####################\n");
    }
}
#else
//Note that this thread is for both DTV/SWDMX sources
static void _AudDtvOnPlayThread(void* pvArg)
{
    UNUSED(pvArg);

    while (1)
    {
        if (_afgDtvOnPlayThreadGo[AUD_DEC_AUX] || _afgDtvOnPlayThreadGo[AUD_DEC_THIRD] || _afgDtvOnPlayThreadGo[AUD_DEC_MAIN])
        {
            UINT8 u1DecId = AUD_DEC_MAIN;

            if ((_afgDtvOnPlayThreadGo[u1DecId])&&(_arAudDecoder[AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))
            {   // updating read/write pointer to DMX and DSP
                UINT32 u4ReadPtr;
                UINT32 u4WritePtr;

                if (_DmxPidIdxValid(u1DecId))
                {
                    _AUD_DMX_GetAudPtr(u1DecId, &u4ReadPtr, &u4WritePtr);
                    //LOG(10, "rp, wp = (0x%08x, 0x%08x)\n",u4ReadPtr, u4WritePtr);
                    if (!DMX_UpdateReadPointer(_DecIdToPidIdx(u1DecId), u4ReadPtr, 0))
                    {
                        LOG(5, "3. Buffer pointer range checking...\n");
                    }
                }
                
                // DTV lock check
                _DtvLockCheck(u1DecId);
                AUD_DrvLogStcDiff(u1DecId);

                if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))
                {
                    if (_arAudDecoder[u1DecId].rCurrentPes.fgEOS == TRUE)
                    {
                        if ((AUD_IsFifoEmpty(u1DecId)))
                        {
                            //for MM auto stop. we need to stop dsp once FIFO emtpy true.. 20111208
                            LOG(5, "Aud FIFO empty driver auto stop decoder(%d) \n",u1DecId);
                            AUD_WaitDspFlush(u1DecId);
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
                        LOG(7, "4. Buffer pointer range checking...\n");
                    }
                }
                // DTV lock check
                _DtvLockCheck(u1DecId);
                AUD_DrvLogStcDiff(u1DecId);

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
                        if (!AUD_IsMMAoutEnable(AUD_DEC_AUX))
                        {
                        	 x_thread_delay(45);
                        }
                    }
                }
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
                        LOG(5, "4. Buffer pointer range checking...\n");
                    }
                }
                // DTV lock check
                _DtvLockCheck(u1DecId);
                AUD_DrvLogStcDiff(u1DecId);

                // If AD played normally, update main decoder "master volume" and "channel volume"
                if (_bPanFadeUpdateEn == TRUE)
                {
                    //fade
                    AUD_DspAdFadeVolumeUpdate(AUD_DEC_MAIN, AUD_CH_ALL);

                    //pan
                    AUD_DspAdPanVolumeUpdate(AUD_DEC_THIRD, AUD_CH_FRONT_RIGHT);
                    AUD_DspAdPanVolumeUpdate(AUD_DEC_THIRD, AUD_CH_FRONT_LEFT);
                }

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
                    }
                }
            }
            else
            {
                AUD_DspAdFadeVolumeSet(0);
                AUD_DspAdFadeVolumeUpdate(AUD_DEC_MAIN, AUD_CH_ALL);
                AUD_DspAdPanVolumeSet(AUD_DEC_THIRD, AUD_CH_FRONT_RIGHT, 0);
                AUD_DspAdPanVolumeSet(AUD_DEC_THIRD, AUD_CH_FRONT_LEFT, 0);
                AUD_DspAdPanVolumeUpdate(AUD_DEC_THIRD, AUD_CH_FRONT_RIGHT);
                AUD_DspAdPanVolumeUpdate(AUD_DEC_THIRD, AUD_CH_FRONT_LEFT);
            }
        }

        x_thread_delay(AUD_PTR_UPDATE_TIMEOUT);
        //LOG(1, "  ############################ _AudDtvOnPlayThread exit ####################\n");
    }
}
#endif
#endif

// Just fix warning message for -O2
void AUD_FixWarning(void)
{
    UNUSED(_paszAudState);
    UNUSED(_paszAudCmd);
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
    AUD_DEC_ID_VALIDATE_2(u1DecId);

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
    LOG(7, "Aud dec(%d) AUD_UopCommandDone 0x%x done\n", u1DecId, u4Command);
    VERIFY(x_sema_unlock(_ahSemaUopComDone[u1DecId]) == OSR_OK);
}

#ifdef FIX_AUD_UOP_TIMEOUT
void AUD_WaitUOPLockSema(UINT8 u1DecId)
{
    INT32 i4Ret;

    // Try lock to make sure the semaphore was locked
    i4Ret = x_sema_lock(_ahSemaUopComDone[u1DecId], X_SEMA_OPTION_NOWAIT);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
}
#endif

void AUD_WaitUopCommandDone(UINT8 u1DecId)
{
    INT32 i4Ret;

    AUD_DEC_ID_VALIDATE(u1DecId);

	LOG(7, "Aud dec(%d) thread sleep to WaitUopCommand  to be done by DSP \n",u1DecId);

    #ifndef FIX_AUD_UOP_TIMEOUT
    // Try lock to make sure the semaphore was locked
    i4Ret = x_sema_lock(_ahSemaUopComDone[u1DecId], X_SEMA_OPTION_NOWAIT);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
    #endif

    // Real lock ..
    i4Ret = x_sema_lock_timeout(_ahSemaUopComDone[u1DecId], 5000);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));

	LOG(7, "Aud dec(%d) thread wake up for UopCommandDone\n",u1DecId);
    if (i4Ret == OSR_TIMEOUT)
    {
        LOG(1, "Aud wait command TIMEOUT\n");
        #ifdef DSP_FORCE_RESET_ENABLE
        u4DspResueTrigger = 1;  //  -- DSP Force --
        #endif
    }
}

void AUD_ResetDspReadPtr(UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
	if (!DMX_UpdateReadPointer(_DecIdToPidIdx(u1DecId), _arAudPes[u1DecId].u4Wp, 0))
    {
        LOG(7, "Buffer pointer range checking...\n");
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
    UINT8 u1DecIdTmp;
    LOG(7, "##### Dec(%d) AUD_SetStartPts = 0x%08x\n", u1DecId, u4Pts);
#ifndef CC_MT5391_AUD_3_DECODER
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
#if 0//def CC_MT5391_AUD_3_DECODER
        //koro: for DTV case, must set decoder 1
        if (_arAudDecoder[AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER &&
            _arAudDecoder[AUD_DEC_AUX].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER &&
            u1DecId == AUD_DEC_MAIN)
        {
            u1DecId = AUD_DEC_AUX;    //set decoder 1
        }
#endif
#ifndef CC_MT5391_AUD_3_DECODER
        for (u1DecIdTmp = 0; u1DecIdTmp <=AUD_DEC_AUX; u1DecIdTmp++)
#else
        for (u1DecIdTmp = 0; u1DecIdTmp <=AUD_DEC_THIRD; u1DecIdTmp++)
#endif
        {
            //set all decoders (video only set one time, but dec2, dec3 both need this info!
            _arAudDecoder[u1DecIdTmp].u4StartPts = u4Pts;
            _afgFirstIPicArrived[u1DecIdTmp] = TRUE;
            _arAudDecoder[u1DecIdTmp].u4PesCntAfterIPicReceived = 0;
        }
        return TRUE;
    }
}

void AUD_PanFadePesUpdateEn(BOOL bEnable)
{
    _bPanFadeUpdateEn = bEnable;
}

void AUD_SetAdPanEnable(BOOL bEnable)
{
    _bPanEnable = bEnable;
}

void AUD_SetAdFadeEnable(BOOL bEnable)
{
    _bFadeEnable = bEnable;
}

BOOL AUD_GetAdPanEnable(void)
{
    return _bPanEnable;
}

BOOL AUD_GetAdFadeEnable(void)
{
    return _bFadeEnable;
}

BOOL AUD_SendAudioPes(const DMX_AUDIO_PES_T* prPes)
{
    UINT8 u1DecId;
    DMX_AUDIO_PES_T rPes = {0};
    STC_CLOCK rStc;
#ifndef CC_MT5391_AUD_3_DECODER
    static UINT32 u4DeltaPts[2] = {0,0};
    BOOL fgDulplicatedPts[2] = {FALSE,FALSE};
    static UINT32 u4LastPts[2] = {0,0};
    static UINT32 u4StartDecWp[2] = {0,0};
    static UINT32 u4PrevWp[2] = {0,0};
//    static UINT32 u4StcPtsDiff[2] = {0,0};
//    static UINT32 u4CurMaxStcPtsDiff[2] = {0,0};
#else
    static UINT32 u4DeltaPts[3] = {0,0,0};
    BOOL fgDulplicatedPts[3] = {FALSE,FALSE,FALSE};
    static UINT32 u4LastPts[3] = {0,0,0};
    static UINT32 u4StartDecWp[3] = {0,0,0};
    static UINT32 u4PrevWp[3] = {0,0,0};
//    static UINT32 u4StcPtsDiff[3] = {0,0,0};
//    static UINT32 u4CurMaxStcPtsDiff[3] = {0,0,0};
#endif

    UINT32 u4WpOffset;
    UINT32 u4ShiftDecWp;
    UINT32 u4DeltaWp;

    static UINT8 u1ADPanPrev = 0;
    static UINT8 u1ADFadPrev = 0;

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

    x_memcpy((VOID *)VIRTUAL((UINT32)&rPes), (const VOID *)prPes, sizeof(DMX_AUDIO_PES_T));
    x_memcpy((VOID *)VIRTUAL((UINT32)&_arAudPes[u1DecId]), (const VOID *)prPes, sizeof(DMX_AUDIO_PES_T));

    // Check if dulplicated PTS
    fgDulplicatedPts[u1DecId] = (rPes.u4Pts == u4LastPts[u1DecId]) ? TRUE : FALSE;

    if(_arAudDecoder[u1DecId].u4ReceiveValidPesCount == 0)
    {
        AUD_MMQueueSyncInfo(u1DecId,0,0,TRUE); //Reset MM Sync info
        AUD_MMQueueTickInfo(u1DecId,0,0,TRUE); //Reset MM Sync info
        AUD_DspSetWithVideo(u1DecId, FALSE);  //Avoid dsp reset sram but driver doesn't reset share memory.
    }
    // Check if PTS = 0
    if (rPes.u4Pts == 0)
    {
        fgDulplicatedPts[u1DecId] = TRUE;
        rPes.u4Pts = u4LastPts[u1DecId];
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
    x_memcpy((VOID *)VIRTUAL((UINT32)&_arAudDecoder[u1DecId].rCurrentPes), (const VOID *)&rPes, sizeof(DMX_AUDIO_PES_T));

   //check if this PES is for EOS flag
   if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA )
   {
       if (_arAudDecoder[u1DecId].rCurrentPes.fgEOS == TRUE)
       {
            LOG(5, "Audio received EOS Pes u1DecId=%d, fgForward=%d\n", u1DecId, _arAudDecoder[u1DecId].rCurrentPes.fgForward);
            return TRUE;
       }
   }

    if ((prPes->u1DeviceId == AUD_DEC_THIRD) && (prPes->fgContainAD == TRUE)) // && (_aeDecStatus[u1DecId] == AUD_DECODE_NORMAL))
    {
        if (_bPanFadeUpdateEn == TRUE)
        {
            if (_bFadeEnable == TRUE)
            {
                AUD_DspAdFadeVolumeSet(rPes.u1ADFad);

                if (prPes->u1ADFad != u1ADFadPrev)
                {
                    LOG(3, "AUD_SendAudioPes prPes->u1ADFad = 0x%x, prPes->u1ADPan = 0x%x\n", prPes->u1ADFad, prPes->u1ADPan);
                    u1ADFadPrev = prPes->u1ADFad;
                }
            }
            else
            {
                AUD_DspAdFadeVolumeSet(0);
            }
            if (_bPanEnable == TRUE)
            {
                if (prPes->u1ADPan != u1ADPanPrev)
                {
                    LOG(3, "AUD_SendAudioPes prPes->u1ADFad = 0x%x, prPes->u1ADPan = 0x%x\n", prPes->u1ADFad, prPes->u1ADPan);
                    u1ADPanPrev = prPes->u1ADPan;
                }

                if (rPes.u1ADPan & 0x80)
                {
                    if (rPes.u1ADPan < 0xeb)
                    {
                        rPes.u1ADPan = 0xeb;
                    }
                    rPes.u1ADPan = 0x100 - rPes.u1ADPan;

                    AUD_DspAdPanVolumeSet(u1DecId, AUD_CH_FRONT_RIGHT, rPes.u1ADPan);
                    AUD_DspAdPanVolumeSet(u1DecId, AUD_CH_FRONT_LEFT, 0);
                }
                else
                {
                    if (rPes.u1ADPan > 0x15)
                    {
                        rPes.u1ADPan = 0x15;
                    }

                    AUD_DspAdPanVolumeSet(u1DecId, AUD_CH_FRONT_LEFT, rPes.u1ADPan);
                    AUD_DspAdPanVolumeSet(u1DecId, AUD_CH_FRONT_RIGHT, 0);
                }
            }
            else
            {
                AUD_DspAdPanVolumeSet(u1DecId, AUD_CH_FRONT_LEFT, 0);
                AUD_DspAdPanVolumeSet(u1DecId, AUD_CH_FRONT_RIGHT, 0);
            }
        }
    }

#ifdef LINUX_TURNKEY_SOLUTION
    if (_hAudDataDumpThreadState[u1DecId] == 1)
    {
        if (_arAudDecoder[u1DecId].u4ReceivePesCount == 1)
        { // Reset read pointer
            u4AudDataCurRp[u1DecId]= VIRTUAL(u4GetAFIFOStart(u1DecId));
            LOG(5, "_AudDataDump %d resets read pointer.\n", u1DecId);
        }
        VERIFY(x_sema_unlock(_hAudDataArrived[u1DecId]) == OSR_OK);
    }
#endif

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
    if ((_arAudDecoder[u1DecId].u4ReceiveValidPesCount == 1) && (!fgDulplicatedPts[u1DecId]))
    {
        x_memcpy((VOID *)VIRTUAL((UINT32)&_arAudDecoder[u1DecId].rFirstReceivePes), (const VOID *)&rPes, sizeof(DMX_AUDIO_PES_T));
#ifdef CC_MT5391_AUD_3_DECODER
        if ((u1DecId == AUD_DEC_AUX)&&(_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER))
#else
        if ((u1DecId == AUD_DEC_MAIN)&&(_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER))
#endif
        {
            if ((AUD_DspGetSpdifUpdateMode(SPDIF_REG_TYPE_USER, AUD_STREAM_FROM_DIGITAL_TUNER) == IEC_CHANNEL_STATUS_BY_SOURCE) &&
                 (_arAudDecoder[u1DecId].eDecState == AUD_DEC_PLAYING) )
            {
                UINT8 u1Category;

                LOG (3, "DTV SPDIF by source: %d\n", _arAudDecoder[u1DecId].rFirstReceivePes.fgCopyright);

                if (_arAudDecoder[u1DecId].rFirstReceivePes.fgCopyright != TRUE)
                {
                    // Set copy free
                    u1Category = AUD_DspGetSpdifCategoryCode(SPDIF_REG_TYPE_USER, AUD_STREAM_FROM_DIGITAL_TUNER);
                    u1Category = u1Category & IEC_CATEGORY_LBIT_MASK;
                    AUD_DspSpdifSetInfo(AUD_STREAM_FROM_DIGITAL_TUNER, IEC_COPY_FREELY, u1Category);
                }
                else
                {
                    // Set copy never
                    u1Category = AUD_DspGetSpdifCategoryCode(SPDIF_REG_TYPE_USER, AUD_STREAM_FROM_DIGITAL_TUNER);
                    u1Category = u1Category | IEC_CATEGORY_LBIT;
                    AUD_DspSpdifSetInfo(AUD_STREAM_FROM_DIGITAL_TUNER, IEC_COPY_NEVER, u1Category);
                }
            }
        }

        u4DeltaPts[u1DecId] = 0;
#if 0
        if (AUD_GetTargetCountry() == COUNTRY_EU)
        {
            u4StcPtsDiff[u1DecId] = 0;
            u4CurMaxStcPtsDiff[u1DecId] = 0;
	    }
#endif
        if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) ||
            (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))
        {
            _ChkAudioOnlyControl(u1DecId, TRUE);        //Andrew Wen 2007/6/22 Audio only program set sync mode to free run.
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
        x_memcpy((VOID *)VIRTUAL((UINT32)&_arAudDecoder[u1DecId].rFirstAvaibleReceivePes), (const VOID *)&rPes, sizeof(DMX_AUDIO_PES_T));
    }

#if 0
    if (((AUD_GetTargetCountry() == COUNTRY_EU)
        && (_arAudDecoder[u1DecId].eStreamFrom != AUD_STREAM_FROM_MULTI_MEDIA))
        ||((_arAudDecoder[AUD_DEC_MAIN].eSynMode == AV_SYNC_SLAVE)&&  //  for timeshift case ,MM source Main dec is systerm master mode
        (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)))
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
                //else if ((prPes->u4Pts - MPEG_FRAME_SIZE)< rStc.u4Base)
                else if (_arAudDecoder[u1DecId].eDecFormat == AUD_FMT_MPEG)
                {
                    u4CurrentStcPtsDiff = MPEG_FRAME_SIZE;   //Should leave enough time for MPEG decoder.
                }
                else if (_arAudDecoder[u1DecId].eDecFormat == AUD_FMT_AAC)
                {
                    u4CurrentStcPtsDiff = MPEG_AAC_FRAME_SIZE;   //Should leave enough time forAAC decoder.
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
#endif
    if ((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) ||
        (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))
    {
        // Check if audio only program
        _ChkAudioOnlyControl(u1DecId, FALSE);  //Andrew Wen 2007/6/22 Audio only program set sync mode to free run.
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
            AUD_SetLPCMFmt(u1DecId, rPes.u4Info);
        }
    }
    if ((STC_GetSrc(STC_SRC_A1, &rStc) == STC_VALID) &&
        (_arAudDecoder[u1DecId].eSynMode != AV_SYNC_FREE_RUN) &&
        (!fgDulplicatedPts[u1DecId]))
    {
        if (_IsPtsValid(rPes.u1PidIndex, u1DecId, &rPes))
        {
            if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
            {
                if (rPes.u4Pts == _arAudDecoder[u1DecId].u4StartPts)
                {
                    AUD_MMQueueSyncInfo(u1DecId, _arAudDecoder[u1DecId].u4StartPts, DSP_INTERNAL_ADDR(u4GetAFIFOStart(u1DecId)), FALSE); 
                }
                else
                {
                    //MM queue av sync info
                    AUD_MMQueueSyncInfo(u1DecId, rPes.u4Pts, rPes.u4Wp, FALSE);
                }
#ifdef TIME_SHIFT_SUPPORT
                //MM queue tick info
                AUD_MMQueueTickInfo(u1DecId, rPes.u4TickNum, rPes.u4Wp, FALSE);
#endif
            }
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
            if ((AUD_GetSyncDbgLvl()&AUD_DBG_SYNC_SEND_PTS) == AUD_DBG_SYNC_SEND_PTS)
            {
                LOG(5," PTS=%x, Wp=%x, Swp=%x, delta=%d, stc=%x\n", rPes.u4Pts, rPes.u4Wp, u4ShiftDecWp,u4DeltaWp ,rStc.u4Base);
            }

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
            if((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
                (_arAudDecoder[u1DecId].eSynMode != AV_SYNC_SLAVE))
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

        if((_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
           (_arAudDecoder[u1DecId].eSynMode != AV_SYNC_SLAVE))
        {
            x_memcpy((VOID *)VIRTUAL((UINT32)&_arAudDecoder[u1DecId].rFristDecodePes),
                     (const VOID *)&rPes, sizeof(PSR_AUDIO_PES_T));
            vDspSetFifoReadPtr(u1DecId, DSP_INTERNAL_ADDR(u4GetAFIFOStart(u1DecId)));
            DSP_SetStartPtsToShm(u1DecId, _arAudDecoder[u1DecId].u4StartPts, DSP_INTERNAL_ADDR(u4GetAFIFOStart(u1DecId)));

            VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_AVSYNC));
            _afgIssuePlayComToDsp[u1DecId] = TRUE;
            LOG(5, "===> Dec (%d) First decode PTS (0x%x), PRS (0x%x), Aud PES cnt (%d) for multimedia\n",
                u1DecId, _arAudDecoder[u1DecId].u4StartPts, u4StartDecWp[u1DecId], _arAudDecoder[u1DecId].u4ReceivePesCount);
        }
#ifdef CC_MT5391_AUD_3_DECODER
        else if ((u1DecId == AUD_DEC_THIRD) && (_afgIssuePlayComToDsp[AUD_DEC_AUX]) &&
                    (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER))
        {
            x_memcpy((VOID *)VIRTUAL((UINT32)&_arAudDecoder[u1DecId].rFristDecodePes),
                     (const VOID *)&rPes, sizeof(PSR_AUDIO_PES_T));
            vDspSetFifoReadPtr(u1DecId, u4StartDecWp[u1DecId]);
            DSP_SetStartPtsToShm(u1DecId, rPes.u4Pts, u4StartDecWp[u1DecId]);
            VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_AVSYNC));
            _afgIssuePlayComToDsp[u1DecId] = TRUE;
            LOG(5, "===> AD on Dec (%d) First decode PTS (0x%x), AudPTS = 0x%x, Aud PES cnt (%d) \n",
                u1DecId, _arAudDecoder[u1DecId].rFristDecodePes.u4Pts, (UINT32)(rPes.u4Pts & 0xffffffff), _arAudDecoder[u1DecId].u4ReceivePesCount);
        }
#endif
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
                x_memcpy((VOID *)VIRTUAL((UINT32)&_arAudDecoder[u1DecId].rFristDecodePes), (const VOID *)&rPes, sizeof(DMX_AUDIO_PES_T));
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
                    LOG(7, "5. Buffer pointer range checking...\n");
                }
                LOG(11, "1. Update RP = 0x%08x\n", DSP_PHYSICAL_ADDR(rPes.u4Wp));
            }
        }        // Fixed ES id,
                 // Video always use ES id 0. Workaround for some POP case, middleware doesn't set av-sync mode correctly
        else if (MPV_IsDisplay(/*(UINT8)STC_GetVdoDeviceId(u1DecId))*/0))
        {
            x_memcpy((VOID *)VIRTUAL((UINT32)&_arAudDecoder[u1DecId].rFristDecodePes),
                     (const VOID *)&rPes, sizeof(PSR_AUDIO_PES_T));
            vDspSetFifoReadPtr(u1DecId, u4StartDecWp[u1DecId]);
            DSP_SetStartPtsToShm(u1DecId, rPes.u4Pts, u4StartDecWp[u1DecId]);
            VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_AVSYNC));
            _afgIssuePlayComToDsp[u1DecId] = TRUE;
            LOG(5, "===> Dec (%d) First decode PTS (0x%x), AudPTS = 0x%x, Aud PES cnt (%d) \n",
                u1DecId, _arAudDecoder[u1DecId].rFristDecodePes.u4Pts, (UINT32)(rPes.u4Pts & 0xffffffff), _arAudDecoder[u1DecId].u4ReceivePesCount);
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
                x_memcpy((VOID *)VIRTUAL((UINT32)&_arAudDecoder[u1DecId].rFristDecodePes), (const VOID *)&rPes, sizeof(PSR_AUDIO_PES_T));
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
                        LOG(7, "6. Buffer pointer range checking...\n");
                    }
                    LOG(11, "2. Update RP = 0x%08x\n", DSP_PHYSICAL_ADDR(u4StartDecWp[u1DecId]));
                }
            }
        }/* (_afgFirstIPicArrived[ucDevId] == FALSE) */
    }
    else if ((_arAudDecoder[u1DecId].eDecState == AUD_DEC_PLAYING) &&
        (_arAudDecoder[u1DecId].eSynMode == AV_SYNC_FREE_RUN) &&
        (_fgWaitAvSync[u1DecId]) && (!_afgIssuePlayComToDsp[u1DecId]))
    {
        x_memcpy((VOID *)VIRTUAL((UINT32)&_arAudDecoder[u1DecId].rFristDecodePes),(const VOID *)&rPes, sizeof(PSR_AUDIO_PES_T));
        vDspSetFifoReadPtr(u1DecId, u4StartDecWp[u1DecId]);
        DSP_SetStartPtsToShm(u1DecId, rPes.u4Pts, u4StartDecWp[u1DecId]);
        VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_AVSYNC));
        _afgIssuePlayComToDsp[u1DecId] = TRUE;
        LOG(5, "===>Send Av sync command in free run mode it is being waited\n");
        LOG(5, "===> Dec (%d) First decode PTS (0x%x), Aud PES cnt (%d) \n",
            u1DecId, _arAudDecoder[u1DecId].rFristDecodePes.u4Pts, _arAudDecoder[u1DecId].u4ReceivePesCount);
    }
    else if ((_arAudDecoder[u1DecId].eDecState == AUD_DEC_STOP) && (!_afgIssuePlayComToDsp[u1DecId]) &&
             (_arAudDecoder[u1DecId].eSynMode != AV_SYNC_FREE_RUN) &&
             (_arAudDecoder[u1DecId].eStreamFrom != AUD_STREAM_FROM_MULTI_MEDIA))  ////koro: multi-media decode from begin of AFIFO
    {
        if (!DMX_UpdateReadPointer(_DecIdToPidIdx(u1DecId), DSP_PHYSICAL_ADDR(rPes.u4Wp), 0))
        {
            LOG(7, "7. Buffer pointer range checking...\n");
        }
    }

    return TRUE;

}

void AUD_GetAudioPes(UINT8 u1DecId, PSR_AUDIO_PES_T * prAudioPes)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    x_memcpy((VOID *)VIRTUAL((UINT32)prAudioPes), (const VOID *)&_arAudDecoder[u1DecId].rCurrentPes, sizeof(PSR_AUDIO_PES_T));
}

void AUD_GetAudioPts(UINT8 u1DecId, UINT32 * prAudioPts)
{
    UINT32 u4ReadPtr;
    UINT32 u4WritePtr;
    UINT32 u4PTS;
#ifdef ENABLE_MULTIMEDIA
    UINT32 u4RefPts;
#endif

    AUD_DEC_ID_VALIDATE(u1DecId);

    _AUD_DMX_GetAudPtr(u1DecId, &u4ReadPtr, &u4WritePtr);
    UNUSED(u4WritePtr);

    {
        LOG(8, "AUD_GetAudioPts[0]  = 0x%x,0x%x\n",_aeMMSyncInfo[0].u4PTS,_aeMMSyncInfo[0].u4PtsWP);
        LOG(8, "AUD_GetAudioPts[%d] = 0x%x,0x%x\n",u2CurrIdx,_aeMMSyncInfo[u2CurrIdx].u4PTS,_aeMMSyncInfo[u2CurrIdx].u4PtsWP);
        LOG(8, "AUD_GetAudioPts[%d] = 0x%x,0x%x\n",MM_SYNC_INFO_NUM-1,_aeMMSyncInfo[MM_SYNC_INFO_NUM-1].u4PTS,_aeMMSyncInfo[MM_SYNC_INFO_NUM-1].u4PtsWP);
    }

    AUD_MMFindSyncInfo(u1DecId,&u4PTS,u4ReadPtr);

#ifdef ENABLE_MULTIMEDIA
    u4RefPts = u4AudDspPts[0];
    if ((u4PTS == 0) && (u4RefPts != 1))
    {
        LOG(5, "Swdmxdsppts=%d\n",u4RefPts);
        *prAudioPts = u4RefPts;
    }
    else if ((u4RefPts != 1) && (u4PTS > (u4RefPts+90000)))
    {
        *prAudioPts = u4RefPts;
    }
    else
#endif
    {
        *prAudioPts = u4PTS;
    }
}

void AUD_DrvGetAudioTrickPts(UINT8 u1DecId, UINT32 *prAudioPts, BOOL fgOldPts)
{
    UINT32 u4PTS;

    if (fgOldPts)
    {
        u4PTS = u4OldPts;
    }
    else
    {
        AUD_GetAudioPts(u1DecId, &u4PTS);
    }
    *prAudioPts = u4PTS;
}

/*
void AUD_GetFisrtAudioPes(UINT8 u1DecId, PSR_AUDIO_PES_T * prAudioPes)
{
     VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
     x_memcpy((VOID *)VIRTUAL(prAudioPes), (const VOID *)&_arAudDecoder[u1DecId].rFirstReceivePes, sizeof(PSR_AUDIO_PES_T));
}

void AUD_GetFisrtAudioDecodePes(UINT8 u1DecId, PSR_AUDIO_PES_T * prAudioPes)
{
     VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
     x_memcpy((VOID *)VIRTUAL(prAudioPes), (const VOID *)&_arAudDecoder[u1DecId].rFristDecodePes, sizeof(PSR_AUDIO_PES_T));
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
            #ifdef FIX_AUD_UOP_TIMEOUT
            AUD_WaitUOPLockSema(u1DecId);
            #endif
            DSP_SendDspTaskCmd(DSP_STOP);
            AUD_WaitUopCommandDone(u1DecId);
            vDspSetFifoReadPtr(u1DecId, u4SartAddress);
            if (!fgDspReadPtrSet(u1DecId, u4WritePtr))
            {
                LOG(5, "DspReadPtrSet time out\n");
            }
            #ifdef FIX_AUD_UOP_TIMEOUT
            AUD_WaitUOPLockSema(u1DecId);
            #endif
            DSP_SendDspTaskCmd(DSP_PLAY);
            AUD_WaitUopCommandDone(u1DecId);
        }
#ifndef CC_MT5391_AUD_3_DECODER
        else // u1DecId == AUD_DEC_AUX
#else
        else if (u1DecId == AUD_DEC_AUX)
#endif
        {
            #ifdef FIX_AUD_UOP_TIMEOUT
            AUD_WaitUOPLockSema(u1DecId);
            #endif
            DSP_SendDspTaskCmd(DSP_STOP_DEC2);
            AUD_WaitUopCommandDone(u1DecId);
            vDspSetFifoReadPtr(u1DecId, u4SartAddress);
            if (!fgDspReadPtrSet(u1DecId, u4WritePtr))
            {
                LOG(5, "DspReadPtrSet time out\n");
            }
            #ifdef FIX_AUD_UOP_TIMEOUT
            AUD_WaitUOPLockSema(u1DecId);
            #endif
            DSP_SendDspTaskCmd(DSP_PLAY_DEC2);
            AUD_WaitUopCommandDone(u1DecId);
        }
#ifdef CC_MT5391_AUD_3_DECODER
        else  // u1DecId == AUD_DEC_THIRD
        {
            #ifdef FIX_AUD_UOP_TIMEOUT
            AUD_WaitUOPLockSema(u1DecId);
            #endif
            DSP_SendDspTaskCmd(DSP_STOP_DEC3);
            AUD_WaitUopCommandDone(u1DecId);
            vDspSetFifoReadPtr(u1DecId, u4SartAddress);
            if (!fgDspReadPtrSet(u1DecId, u4WritePtr))
            {
                LOG(5, "DspReadPtrSet time out\n");
            }
            #ifdef FIX_AUD_UOP_TIMEOUT
            AUD_WaitUOPLockSema(u1DecId);
            #endif
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
    LOG(5,"CMD: set AVSyncMode: Dec(%d) Mode(%d)\n", u1DecId, (UINT8)eSynMode);

    AUD_DEC_ID_VALIDATE(u1DecId);

    _arAudDecoder[u1DecId].eSynMode = eSynMode;
    vDspSetSyncMode(u1DecId, eSynMode);
#ifndef CC_AUD_SKYPE_SUPPORT
    if (u1DecId == AUD_DEC_THIRD)  // decoder 3 use FIFO0.
    {
        u1DecId =AUD_DEC_MAIN;
    }
#endif

    // 2008/10/16 Jessica: There is only 1 STC module. STC ID is forced to be 0
    if (STC_SetSyncTarget(AV_SYNC_TARGET_AUD, STC_SRC_A1, u1DecId) == STC_INVALID)
    {
        LOG(5, "STC_SetSyncTarget fail\n");
    }
//    VERIFY(STC_SetPtsDrift(u1DecId, PTS_DRIFT) == STC_VALID);
//  STC_SetAudDeviceId(u1DecId, u1DecId);
}

AUD_MM_MODE_T AUD_DrvGetMultimediaMode(UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    return _arAudDecoder[u1DecId].eMultimediaMode;
}

void AUD_DrvSetMultimediaMode(UINT8 u1DecId, AUD_MM_MODE_T eMultimediaMode)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    _arAudDecoder[u1DecId].eMultimediaMode = eMultimediaMode;
}

void AUD_DrvSetAdMode(BOOL fgAdMode)
{
    UNUSED(fgAdMode);
#ifdef CC_ENABLE_AV_SYNC
    _fgAdMode = fgAdMode;
#endif
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
    x_memcpy((VOID *)VIRTUAL((UINT32)prAudDec), (const VOID *)&_arAudDecoder[u1DecId], sizeof(AUD_DECODER_T));
}

void AUD_DRVInit(void)
{
    INT32 i;
    CRIT_STATE_T _rCritState;
    CRIT_STATE_T rState;

    for (i=0; i<AUD_DEC_NUM; i++)
    {
        // Clear decoder info ..
        x_memset((VOID *)VIRTUAL((UINT32)&(_arAudDecoder[i])), 0, sizeof(AUD_DECODER_T));

        // Variable intial value
        _arAudDecoder[i].rPcmSetting.ePcmDataInfo = PCM_24_BITS;
        _arAudDecoder[i].rPcmSetting.eSampleFreq = FS_44K;
        _arAudDecoder[i].fgMMAoutEnable = FALSE;
        _arAudDecoder[i].fgMMAoutReady = FALSE;
        _arAudDecoder[i].eMultimediaMode = AUD_MM_MODE_NORMAL;
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

    // Init AvSync setting
    // 2008/10/16 Jessica: There is only 1 STC module. STC ID is forced to be 0
    VERIFY(STC_SetSyncTarget(AV_SYNC_TARGET_AUD, STC_SRC_A1, AUD_DEC_MAIN) == STC_VALID);
    VERIFY(STC_SetSyncTarget(AV_SYNC_TARGET_AUD, STC_SRC_A1, AUD_DEC_AUX) == STC_VALID);

    //Andrew Wen 2007/6/22 Audio only program set sync mode to free run.
    prDbase = DBS_Lock(&rState);
    ASSERT(prDbase != NULL);
    VERIFY(DBS_Unlock(prDbase, rState));
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

    // Save setting to _arAudDecoder
    _arAudDecoder[u1DecId].eStreamFrom = eStreamFrom;
    _arAudDecoder[u1DecId].eDecFormat =  eDecFmt;

    AUD_SetSrc(u1DecId,eStreamFrom, eDecFmt);
    // Setup individual decoder setting by source
    _DecoderInitSetup(u1DecId, eStreamFrom, eDecFmt);

    // Setup volume gain
    AUD_DspCfgSetDecType(u1DecId,  eStreamFrom, eDecFmt);

    VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_LOADCODE));

    // set SDPIF
    // NOTICE: this function uses _bDspStrTyp and _bDspStrTypDec2 to determine the enable case.
    // Thus, it must calls after DSP code is loaded. (_bDspStrTyp is set at loading DSP code)
	if ((u1DecId!=AUD_DEC_AUX)||(_arAudDecoder[AUD_DEC_MAIN].eStreamFrom != AUD_STREAM_FROM_HDMI))
	{
		_AUD_DspSetIEC(u1DecId);
	}
    //Setup DVD gpio
    //AUD_SetSrc(u1DecId,eStreamFrom, eDecFmt);
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
        AUD_CLR_BIT(REG_BS_CFG, PAR_SEL);
        AUD_CLR_BIT(REG_BS_CFG, DEC3_PAR_SEL);
        AUD_CLR_BIT(REG_BS_CFG, DEC4_PAR_SEL);
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        AUD_SET_BIT(REG_BS_CFG, PAR_SEL);
        AUD_CLR_BIT(REG_BS_CFG, DEC3_PAR_SEL);
        AUD_CLR_BIT(REG_BS_CFG, DEC4_PAR_SEL);
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        AUD_CLR_BIT(REG_BS_CFG, PAR_SEL);
        AUD_SET_BIT(REG_BS_CFG, DEC3_PAR_SEL);
        AUD_CLR_BIT(REG_BS_CFG, DEC4_PAR_SEL);
    }
    else
    {
        AUD_CLR_BIT(REG_BS_CFG, PAR_SEL);
        AUD_CLR_BIT(REG_BS_CFG, DEC3_PAR_SEL);
        AUD_SET_BIT(REG_BS_CFG, DEC4_PAR_SEL);
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
        prTransmit->u4BufferSize = 0; // Not support yet
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
    }
}

//-----------------------------------------------------------------------------
// AUD_DrvAc3Setting
//
/** AC3 format setting
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      prPcmSetting
 */
//-----------------------------------------------------------------------------
void AUD_DrvAc3Setting(UINT8 u1DecId, const AUD_AC3_SETTING_T * prAc3Setting)
{
    if (prAc3Setting != NULL)
    {
        DSP_SetAc3Endian(u1DecId, prAc3Setting->eEndian);
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
#ifdef CC_ENABLE_AV_SYNC
	if (_u1VideoFrameFlag != u1VideoPath)
	{
		_fgVdpChangeState = FALSE;
		LOG(3,"[SRM_AVSYNC] !!! Change Video Path --> Go to Short Mode !!!\n");
	}
#endif
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
             LOG(9, "AudApllCmdQueue Full ...\n");
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
         LOG(9, "AudApllCmdQueue Full ...\n");
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
                LOG(7, "8. Buffer pointer range checking...\n");
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
                #ifdef FIX_AUD_UOP_TIMEOUT
                AUD_WaitUOPLockSema(u1DecId);
                #endif
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
	        UINT32 u4FifoStart;
	        UINT32 u4FifoEnd;

	        UNUSED(_AUD_GetAudFifo(u1DecId, &u4FifoStart, &u4FifoEnd));
            vDspSetFifoReadPtr(u1DecId, u4FifoStart);
        }

        #ifdef FIX_AUD_UOP_TIMEOUT
        AUD_WaitUOPLockSema(u1DecId);
        #endif

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

void AUD_MMQueueTickInfo(UINT8 u1DecId, UINT32 u4Tick, UINT32 u4PTSWp, BOOL fgReset)
{
    static UINT16 u2Idx = 0;
    UINT16 i;
    BOOL fgUpdate;

	UINT32 u4FifoStart;
	UINT32 u4FifoEnd;

    UNUSED(_AUD_GetAudFifo(u1DecId, &u4FifoStart, &u4FifoEnd));
    UNUSED(u1DecId);

    if (fgReset)
    {
        u2Idx = 0;
        //u2TickLatestIdx = 0;
        //fgTickLoopBack = FALSE;
        for (i=0; i<MM_TICK_INFO_NUM; i++)
        {
            _aeMMTickInfo[i].u4Tick = 0;
            _aeMMTickInfo[i].u4PtsWP = 0xfffffff;
        }
    }
    else
    {
        if (u2Idx >= MM_TICK_INFO_NUM)
        {
            LOG(7," ### MM Tick Info - The deepest u2Idx = %d ###\n", u2Idx);
            LOG(7,"_aeMMTickInfo[0]  = 0x%x,0x%x\n",_aeMMTickInfo[0].u4Tick,_aeMMTickInfo[0].u4PtsWP);
            LOG(7,"_aeMMTickInfo[%d] = 0x%x,0x%x\n",u2Idx-1,_aeMMTickInfo[u2Idx-1].u4Tick,_aeMMTickInfo[u2Idx-1].u4PtsWP);
            u2Idx = 0;
            //fgTickLoopBack = TRUE;
        }
        LOG(9,"MM Q %d, Tick 0x%x, Wp 0x%x\n",u2Idx,u4Tick,u4PTSWp);

        fgUpdate = TRUE;
        if(u2Idx == 0)
        {
            if(_aeMMTickInfo[MM_TICK_INFO_NUM - 1].u4Tick == u4Tick)
            {
                fgUpdate = FALSE;
                _aeMMTickInfo[MM_TICK_INFO_NUM - 1].u4PtsWP = u4PTSWp;
            }
        }
        else
        {
            if(_aeMMTickInfo[u2Idx - 1].u4Tick == u4Tick)
            {
                fgUpdate = FALSE;
                _aeMMTickInfo[u2Idx - 1].u4PtsWP = u4PTSWp;
            }
        }
        if(fgUpdate)
        {
            _aeMMTickInfo[u2Idx].u4Tick = u4Tick;
            _aeMMTickInfo[u2Idx].u4PtsWP = u4PTSWp;
            u2TickCurrIdx = u2Idx;
            u2Idx += 1;
        }
    }


}

BOOL AUD_MMFindTickInfo(UINT8 u1DecId, UINT32* u4Tick)  //, UINT32 u4PTSRp)
{

    UINT16 i;
    UINT16 u2Idx;
    BOOL fgFindFifoEnd = FALSE;
    BOOL fgFound = FALSE;
    UINT32 u4PTSRp;
    UINT32 u4ReadPtr;
    UINT32 u4WritePtr;

	_AUD_DMX_GetAudPtr(u1DecId, &u4ReadPtr, &u4WritePtr);

    UNUSED(u1DecId);
    UNUSED(u2Idx);
    UNUSED(u4WritePtr);

	u4PTSRp = u4ReadPtr;
    u4PTSRp &= 0xffffff;
    *u4Tick = 0;

    u2Idx = u2TickCurrIdx;
    VERIFY(u2Idx < MM_TICK_INFO_NUM);
    if (u2Idx < MM_TICK_INFO_NUM)
    {
        for (i=0; i<MM_TICK_INFO_NUM; i++)
        {
            if (fgFindFifoEnd)
            {
                if (u4PTSRp >= _aeMMTickInfo[u2Idx].u4PtsWP)
                {
                    LOG(5,"Request Rp = 0x%x\n", u4PTSRp);
                    LOG(5,"MM Find idx = %d, Rp = 0x%x, Tick = 0x%x @@@@@@@@@@@@@@@@@@@@@\n",u2Idx,
                    _aeMMTickInfo[u2Idx].u4PtsWP, _aeMMTickInfo[u2Idx].u4Tick);
                    *u4Tick = _aeMMTickInfo[u2Idx].u4Tick;
                    fgFound = TRUE;
                    break;
                }
            }
            else
            {
                if (u4PTSRp <= _aeMMTickInfo[u2Idx].u4PtsWP)
                {
                    fgFindFifoEnd = TRUE;
                }
            }

            if (u2Idx == 0)
            {
                u2Idx = MM_TICK_INFO_NUM;
            }
            u2Idx--;
        }
    }
    else
    {
        LOG(7,"MM Tick error\n");
    }
    return fgFound;
}

void AUD_MMQueueSyncInfo(UINT8 u1DecId, UINT32 u4PTS, UINT32 u4PTSWp, BOOL fgReset)
{
    static UINT16 u2Idx = 0;
    UINT16 i;

    UNUSED(u1DecId);
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
    UINT16 u2Idx, u2PreIdx;
    BOOL fgOverRead = FALSE; 
    BOOL fgFound = FALSE;
    UINT32 u4CurWp;
    UINT32 u4FindWp, u4FindPts; 
    UINT32 u4FifoStart, u4FifoEnd;
    UINT32 u4Pos, u4Size, u4Wp1, u4Wp2, u4Pts1, u4Pts2;
    UNUSED(u1DecId);
    UNUSED(u2Idx);
    UNUSED(u2PreIdx);

    *u4PTS = 0;
    _AUD_DMX_GetAudPtr(u1DecId, &u4PTSRp, &u4CurWp); 
    u4PTSRp &= 0xffffff;
    u4CurWp &= 0xffffff;

    u2Idx = u2CurrIdx; 
    VERIFY(u2Idx < MM_SYNC_INFO_NUM); 
    u2PreIdx = (u2Idx > 0) ? (u2Idx - 1) : (MM_SYNC_INFO_NUM - 1);

    //check if RP over the newest WP in the Queue
    if ((_aeMMSyncInfo[u2Idx].u4PtsWP != 0xfffffff) && (_aeMMSyncInfo[u2PreIdx].u4PtsWP != 0xfffffff))
    {
        if (u4CurWp >= _aeMMSyncInfo[u2Idx].u4PtsWP)
        {
            if ((u4PTSRp >= _aeMMSyncInfo[u2Idx].u4PtsWP) && (u4PTSRp < u4CurWp))
            {
                u4FindWp =  _aeMMSyncInfo[u2Idx].u4PtsWP;
                u4FindPts=  _aeMMSyncInfo[u2Idx].u4PTS; 
                fgOverRead = TRUE; 
            }
        }
        else
        {
            if ((u4PTSRp >= _aeMMSyncInfo[u2Idx].u4PtsWP) || (u4PTSRp < u4CurWp))
            {
                u4FindWp =  _aeMMSyncInfo[u2Idx].u4PtsWP; 
                u4FindPts=  _aeMMSyncInfo[u2Idx].u4PTS; 
                fgOverRead = TRUE; 
            }
        }
    }
    
    if (!fgOverRead)
    {
        UINT16 i;
        for (i=0; i<MM_SYNC_INFO_NUM; i++)
        {
            u2PreIdx = (u2Idx > 0) ? (u2Idx - 1) : (MM_SYNC_INFO_NUM - 1);
            if ((_aeMMSyncInfo[u2Idx].u4PtsWP != 0xfffffff) && (_aeMMSyncInfo[u2PreIdx].u4PtsWP != 0xfffffff))
            {
                if (_aeMMSyncInfo[u2Idx].u4PtsWP > _aeMMSyncInfo[u2PreIdx].u4PtsWP)
                {
                    if ((u4PTSRp >= _aeMMSyncInfo[u2PreIdx].u4PtsWP) && (u4PTSRp < _aeMMSyncInfo[u2Idx].u4PtsWP))
                    {
                        u4FindWp = _aeMMSyncInfo[u2PreIdx].u4PtsWP;
                        u4FindPts = _aeMMSyncInfo[u2PreIdx].u4PTS; 
                        fgFound = TRUE;
                        break;
                    }
                }
                else
                {
                    if ((u4PTSRp >= _aeMMSyncInfo[u2PreIdx].u4PtsWP) || (u4PTSRp < _aeMMSyncInfo[u2Idx].u4PtsWP))
                    {
                        u4FindWp = _aeMMSyncInfo[u2PreIdx].u4PtsWP;
                        u4FindPts = _aeMMSyncInfo[u2PreIdx].u4PTS; 
                        fgFound = TRUE;
                        break;
                    }
                } 
            }
            else
            {
                break;
            }
            
            if (u2Idx == 0)
            {
                u2Idx = MM_SYNC_INFO_NUM;
            }
            u2Idx--;
        }
    }
    
    if (fgFound || fgOverRead)
    {
        u4FifoStart = u4GetAFIFOStart(u1DecId);
        u4FifoEnd = u4GetAFIFOEnd(u1DecId);
        u4Wp1 = _aeMMSyncInfo[u2PreIdx].u4PtsWP;
        u4Wp2 = _aeMMSyncInfo[u2Idx].u4PtsWP;      
        u4Pts1 = _aeMMSyncInfo[u2PreIdx].u4PTS;
        u4Pts2 = _aeMMSyncInfo[u2Idx].u4PTS; 
        u4Pos = (u4PTSRp >= u4FindWp) ? (u4PTSRp - u4FindWp) : (u4PTSRp + u4FifoEnd - u4FindWp - u4FifoStart);
        u4Size = (u4Wp2 >= u4Wp1) ? (u4Wp2 - u4Wp1) : (u4Wp2 + u4FifoEnd - u4Wp1 - u4FifoStart);
        if (u4Size > 0)
        {
            *u4PTS = u4FindPts + (UINT32)u8Div6432((UINT64)u4Pos * (UINT64)(u4Pts2 - u4Pts1), (UINT64)u4Size, NULL) ; 
        }
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

	//add condition to avoid duplicated dsp cmd to lower message queue loading
	//also..when we pause to debug DSP.. _Dtv_lockCheck will think there is no data and make SWDMX send this cmd crazily. thus system hung!
    if (_fgAudMMnotifySWDMX[u1DecId])
    {
    	_fgAudMMnotifySWDMX[u1DecId] = FALSE;
	    _arAudDecoder[u1DecId].fgMMAoutEnable = TRUE;

	    if (u1DecId == AUD_DEC_MAIN)
	    {
	        LOG(5, "AUD_MMAoutEnable for MAIN decoder \n");
	        DSP_SendDspTaskCmd(DSP_CLEAR_AOUT_WAIT);
	    }
	    else if (u1DecId == AUD_DEC_AUX)
	    {
	        LOG(5, "AUD_MMAoutEnable for AUX decoder \n");
	        DSP_SendDspTaskCmd(DSP_CLEAR_AOUT_WAIT_DEC2);
	    }
	    else
	    {
	        LOG(5, "Not support AUD_MMAoutEnable for decoder 3 \n");
	    }
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
	// We don't set _fgVdpReady here
  //  _fgVdpReady = fgEnable;
    UNUSED(x_sema_unlock(_hVdpSyncSema));
}

BOOL AUD_IsIgnoreVdpInfo (void)
{
    return _fgIgnoreVdpInfo;
}

void AUD_UnMuteWithVideo(void)
{
    UINT32 u4VdoMuteFlag;

    if (_u1VideoFrameFlag == 0) //0: PIP MAIN, 1: PIP Sub
    {
        u4VdoMuteFlag = _u4DrvVideoGetMute(SV_VP_MAIN);
    }
    else
    {
        u4VdoMuteFlag = _u4DrvVideoGetMute(SV_VP_PIP);
    }
    if ((u4VdoMuteFlag == 0)&& _fgAVSyncThreadMute)
    {
        _fgAVSyncThreadMute = FALSE;
        _u2AudDrvDtvUNMuteCnt = _u2AudDrvExtraMuteCnt;
    }



    //LOG(7,"Aud in PIP path %d, VideoMute is %d now, %d\n", _u1VideoFrameFlag, u4VdoMuteFlag, _u2AudDrvDtvUNMuteCnt);
    if (_u2AudDrvDtvUNMuteCnt>0)
    {
        _u2AudDrvDtvUNMuteCnt--;
    }
 /*
    if (_u2AudDrvDtvPIPUNMuteCnt>0)
    {
        _u2AudDrvDtvPIPUNMuteCnt--;
    }
    */
    if (_u2AudDrvDtvUNMuteCnt == 0)
    {
        LOG(7,"AUDIO UNMUTE!! Aud in PIP path %d, VideoMute is %d now, %d\n", _u1VideoFrameFlag, u4VdoMuteFlag, _u2AudDrvDtvUNMuteCnt);
        AUD_DspDecPlayMuteEnable(AUD_DEC_MAIN, FALSE);
        _u2AudDrvDtvUNMuteCnt = 0;
        //_u2AudDrvDtvPIPUNMuteCnt = 0;
        _fgAVSyncThreadGo = FALSE;
    }
}
#endif


void _AudClearEOSFlag(UINT8 u1DecId)
{
    _arAudDecoder[u1DecId].rCurrentPes.fgEOS = FALSE;
}

void AUD_DrvClearPesCnt(UINT8 u1DecId)
{
    _arAudDecoder[u1DecId].u4ReceivePesCount = 0;
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
#ifdef CC_53XX_SWDMX_V2
		if (!_fgAoutTimeoutEnable && _fgAudMMnotifySWDMX[u1DecId])
		{
		    SWDMX_AudioNotify(_u1SwdmxSrcId[u1DecId], ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE);
		}
#else
		if (!_fgAoutTimeoutEnable && _fgAudMMnotifySWDMX[u1DecId])
		{
			SWDMX_AudioNotify(ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE);
        }
#endif
#endif
    }
}

void AUD_DrvMMNoDataNotify(UINT8 u1DecId)
{
    if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
    {
        #ifdef ENABLE_MULTIMEDIA
        #ifdef CC_53XX_SWDMX_V2
        SWDMX_AudioNotify(_u1SwdmxSrcId[u1DecId], ENUM_SWDMX_AUDIO_NOTIFY_NO_DATA);
        #else
        SWDMX_AudioNotify(ENUM_SWDMX_AUDIO_NOTIFY_NO_DATA);
        #endif
        #endif
    }
}

#ifndef CC_AUD_SKYPE_SUPPORT
void AUD_DrvMMAoutEnable_Flag(BOOL fgEnable)
{
    _arAudDecoder[AUD_DEC_MAIN].fgMMAoutEnable = fgEnable;
}

void AUD_DrvMMAoutReady_Flag(BOOL fgEnable)
{
    _arAudDecoder[AUD_DEC_MAIN].fgMMAoutReady = fgEnable;
}
#else
void AUD_DrvMMAoutEnable_Flag(UINT8 u1DecId, BOOL fgEnable)
{
    _arAudDecoder[u1DecId].fgMMAoutEnable = fgEnable;
}

void AUD_DrvMMAoutReady_Flag(UINT8 u1DecId, BOOL fgEnable)
{
    _arAudDecoder[u1DecId].fgMMAoutReady = fgEnable;
}
#endif

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

#ifdef DSP_FORCE_RESET_ENABLE
// msut add AD protection!!!!!! by Shawn
static void _AudDspMonitorThread(void* pvArg) //  -- DSP Force --
{
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
}
#endif

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

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
BOOL AUD_DrvCheckIECSyncWord(AUD_HDMI_PARSER_INFO_T * prParserInfo)
{
    UINT16 u2SyncWord;    //16 bit word to check
    UINT8 i;

    u2SyncWord = AUD_DrvSwapWord(AUD_READIEC16(VIRTUAL(prParserInfo->eHDMIBuffer.u4FifoPtr)));
    prParserInfo->u4PaPb = (prParserInfo->u4PaPb<<16)|u2SyncWord;
    if((prParserInfo->u4PaPb == IEC_SYNCWORD_DTS_14)||(prParserInfo->u4PaPb == IEC_SYNCWORD_DTS_16))
    {
        prParserInfo->eSpdifInfo.u2format = 0xB; //find DTS header set pc as dts
        prParserInfo->_fgDTSCD = TRUE;   //DTS CD parser flag
        prParserInfo->_fgDTSSW = TRUE;  //DTS CD detect flg
        LOG(9,"Detect as DTS CD.\n");
    }
    else
    {
        prParserInfo->_fgDTSSW = FALSE;
    }
    if(prParserInfo->u4PaPb!=IEC_SYNCWORD_PA_PB)
    {
        return FALSE;
    }
    else
    {
        
        prParserInfo->_fgDTSCD = FALSE; //find papb,so it's not DTS CD,DTS CD don't have papb
        prParserInfo->u4PaPb = 0;
        prParserInfo->eHDMIBuffer.u4FifoPtr = AUD_DrvGetCircularAddress(u4GetHDMIParserFIFOStart(),u4GetHDMIParserFIFOEnd(),prParserInfo->eHDMIBuffer.u4FifoPtr+2);
        u2SyncWord = AUD_DrvSwapWord(AUD_READIEC16(VIRTUAL(prParserInfo->eHDMIBuffer.u4FifoPtr)))&0x1f;//pc bits of pc 0~4 data type
        for(i=0;i<AUD_RAW_NUM;i++)
        {
            if(u2SyncWord == _au2Pc[i])
            {
                break;
            }
        }
        prParserInfo->eSpdifInfo.u2format = u2SyncWord;   
        LOG(9,"prParserInfo->eSpdifInfo.u2format = 0x%x.\n",prParserInfo->eSpdifInfo.u2format);
        if(i<AUD_RAW_NUM)
        {
            return TRUE;
                
        }
        else
        {
            LOG(9,"unknow pc: %x.\n",u2SyncWord);
            return FALSE;
        }
    }    
}

UINT16 AUD_DrvSwapWord(UINT16 u2swap)
{
    return(((u2swap&0xff)<<8)|((u2swap&0xff00)>>8));
}
UINT32 AUD_DrvGetCircularAddress(UINT32 u4StartAddress,UINT32 u4EndAddress,UINT32 u4Address)
{
    if(u4Address>=PHYSICAL(u4EndAddress))
    {
        return (u4Address-u4EndAddress+u4StartAddress);
    }
    else if(u4Address<PHYSICAL(u4StartAddress))
    {
        return (u4Address+u4EndAddress-u4StartAddress);
    }
    else
    {
        return u4Address;
    }
}
void AUD_DrvSetFgHDMIParserthread(UINT8 u1DecId,BOOL fgHDMIParserThread)
{
    if(_arParserInfo[u1DecId]._fgHDMIParserThreadGo!= fgHDMIParserThread)
    {
        _arParserInfo[u1DecId]._fgHDMIParserThreadGo = fgHDMIParserThread;
        if(fgHDMIParserThread)
        {
               VERIFY(x_sema_unlock(_arParserInfo[u1DecId]._hSemaHDMIParserThreadWait) == OSR_OK);
        }
    }
}

BOOL AUD_DrvCheckHDMISize(AUD_FIFO_INFO_T arfifo,UINT32 u4CheckSize)
{
	UINT32 hdmi_wp = AUD_GetLineinWP();
    UINT32 u4RemainSize;
    
    if((hdmi_wp<arfifo.u4FifoStartAddr)||(hdmi_wp>=arfifo.u4FifoEndAddr)) 
    {
        return FALSE;
    }
    if(hdmi_wp<arfifo.u4FifoPtr)
    {
        u4RemainSize = hdmi_wp+arfifo.u4FifoEndAddr-arfifo.u4FifoStartAddr-arfifo.u4FifoPtr;
    }
    else
    {
        u4RemainSize = hdmi_wp - arfifo.u4FifoPtr;
    }
    if(u4RemainSize>u4CheckSize)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#ifdef CC_AUD_HDMI_PARSER_3_0_SUPPORT
void AUD_DrvGetParserFormat(AUD_FMT_T *peDecFmt)
{
    *peDecFmt = _arParserInfo[AUD_DEC_MAIN].e_aud_det_fmt;
}

void AUD_DrvGetParserDetect(BOOL *pfgDeteced)
{
    if(_arParserInfo[AUD_DEC_MAIN].e_aud_det_fmt == AUD_FMT_PCM)
    {
        *pfgDeteced = TRUE;
    }
    if((_arParserInfo[AUD_DEC_MAIN].u4NoRawDataSize>HDMI_PARSER_NON_DETECT_SIZE)&&(_arParserInfo[AUD_DEC_MAIN].u4NoRawDataSize<HDMI_PARSER_DETECT_PCM_SIZE))
    {
        *pfgDeteced = FALSE;
    }
    else
    {
        *pfgDeteced = TRUE;
    }
}

AUD_FMT_T AUD_DrvPcToFormat(UINT16 u2Pc)
{
    AUD_FMT_T e_aud_fmt = AUD_FMT_UNKNOWN;
    _arParserInfo[AUD_DEC_MAIN]._fgRaw = TRUE;
 
    switch(u2Pc)
    {
        case 0x1:  
          if(AUD_DspIsSupportAC3())
          {
              e_aud_fmt = AUD_FMT_AC3;
          }
          break;
        case 0x15:    
          if(AUD_DspIsSupportEAC3())
          {
              e_aud_fmt = AUD_FMT_AC3;
          }
          break;
        case 0x4:     
        case 0x5:     
        case 0x6:     
        case 0x8:     
        case 0x9:     
          e_aud_fmt = AUD_FMT_MPEG;
          break;  
        case 0x7:     
          if(AUD_DspIsFormatValid(AUD_FMT_AAC))
          {
              e_aud_fmt = AUD_FMT_AAC;
          }
          break;
          case 0xB:     
          case 0xC:     
          case 0xD:     
          if(AUD_DspIsFormatValid(AUD_FMT_DTS))
          {
              e_aud_fmt = AUD_FMT_DTS;
          }
          break; 
        case 0x3:
          e_aud_fmt =  _arParserInfo[AUD_DEC_MAIN].e_aud_det_fmt; 
          break;
        default :     
          _arParserInfo[AUD_DEC_MAIN]._fgRaw = FALSE;  
          e_aud_fmt = AUD_FMT_UNKNOWN;
          break;   
    }
    return e_aud_fmt;
}

#endif


#endif

extern void AUD_SET_SWDMXSrcId(UINT8 u1DecId, UINT8 u1SwdmxSrcId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    _u1SwdmxSrcId[u1DecId] = u1SwdmxSrcId;
    UNUSED(_u1SwdmxSrcId[u1DecId]);
}

extern void AUD_GET_SWDMXSrcId(UINT8 u1DecId, UINT8* u1SwdmxSrcId)
{
    UNUSED(AUD_GET_SWDMXSrcId);
    AUD_DEC_ID_VALIDATE(u1DecId);
    *u1SwdmxSrcId = _u1SwdmxSrcId[u1DecId];
}


void AUD_DrvCallback(UINT8 u1DecId, AUD_DSP_CB_T eAudDspCbType)
{
    UNUSED(u1DecId);
    UNUSED(AUD_DrvCallback);

    switch (eAudDspCbType)
    {
    case AUD_DSP_CB_UNMUTE:
        AUD_DRVCmd(u1DecId, AUD_CMD_UNMUTE);
        break;
    default:
        break;
    }
}

BOOL AUD_DrvDspCallBackProcess(UINT8 u1DecId, AUD_DEC_CMD_T eCmd)
{
    BOOL fgRet = FALSE;  //Set TRUE means this is a DSP callback command, default is FALSE.

    UNUSED(u1DecId);
    switch (eCmd)
    {
    case AUD_CMD_UNMUTE:
#ifdef CC_ENABLE_AV_SYNC
        _u2AudDrvExtraMuteCnt = 0;
        if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
        {
#ifdef CC_MT5391_AUD_3_DECODER  //xiaowen chen
            if(_arAudDecoder[AUD_DEC_AUX].eSynMode == AV_SYNC_FREE_RUN)//EU Audio Only Channel
            {
                _u2AudDrvDtvUNMuteCnt = (AUD_PIP_MUTE_WITH_VDO_TIMEOUT/AUD_PTR_UPDATE_TIMEOUT);
            }
#else
            if(_arAudDecoder[AUD_DEC_MAIN].eSynMode == AV_SYNC_FREE_RUN)//US Audio Only Channel
            {
                _u2AudDrvDtvUNMuteCnt = (AUD_PIP_MUTE_WITH_VDO_TIMEOUT/AUD_PTR_UPDATE_TIMEOUT);
            }
#endif
            else
            {
                _u2AudDrvDtvUNMuteCnt = (AUD_MUTE_WITH_VDO_TIMEOUT/AUD_PTR_UPDATE_TIMEOUT);
            }
#ifdef CC_ENABLE_AV_SYNC
            if (_fgAdMode)
            {
                //AD will be turned on, set extra delay
                _u2AudDrvExtraMuteCnt = (AUD_EXTRA_MUTE_AFTER_VDP/AUD_PTR_UPDATE_TIMEOUT);
                LOG(7,"======> DTV AV SYNC EXTRA COUNT = %d\n",_u2AudDrvExtraMuteCnt);
            }
#endif
            LOG(7,"======> DTV AV SYNC COUNT = %d\n",_u2AudDrvDtvUNMuteCnt);
        }
        else if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_LINE_IN)
        {
            _u2AudDrvDtvUNMuteCnt = (AUD_MUTE_WITH_AV_VDO_TIMEOUT/AUD_PTR_UPDATE_TIMEOUT);
            LOG(7,"======> Line IN AV SYNC COUNT = %d\n",_u2AudDrvDtvUNMuteCnt);
        }
        else if (_arAudDecoder[u1DecId].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER)
        {
            _u2AudDrvDtvUNMuteCnt = (AUD_MUTE_WITH_AV_VDO_TIMEOUT/AUD_PTR_UPDATE_TIMEOUT);
            LOG(7,"======> ATV AV SYNC COUNT = %d\n",_u2AudDrvDtvUNMuteCnt);
        }
        else
        {
            _u2AudDrvDtvUNMuteCnt = 0;
            AUD_DspDecPlayMuteEnable(AUD_DEC_MAIN, FALSE);
        }
        if(_u2AudDrvDtvUNMuteCnt != 0)
        {
            LOG(0,"======> AV Sync Thread GoGoGo <=======\n ");
            _fgAVSyncThreadGo = TRUE;
            _fgAVSyncThreadMute = TRUE;
            _fgAudDrvExtraMute = TRUE;
        }
        ///_u2AudDrvDtvPIPUNMuteCnt = (AUD_PIP_MUTE_WITH_VDO_TIMEOUT/AUD_PTR_UPDATE_TIMEOUT);
#endif
        fgRet = TRUE;
        break;
    default:
        break;
    }

    return fgRet;
}

#ifdef CC_ENABLE_AV_SYNC
static void _AudAvSyncThread(void* pvArg)
{
	while(1)
	{
        if(_fgAVSyncThreadGo)
        {
            if (_arAudDecoder[AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
            {
                if(AUD_GetVideoScrambleStatus() && _fgAudDrvExtraMute)
                {
                    _fgAudDrvExtraMute = FALSE;
                    _u2AudDrvDtvUNMuteCnt = _u2AudDrvExtraMuteCnt;
                    LOG(7,"========= SCRAMBLE VIDEO , AUDIO FREE RUN =========\n");
                }
            }
            //LOG(7,"========= _AudAvSyncThread =========\n");
            AUD_UnMuteWithVideo();
        }
        x_thread_delay(AUD_PTR_UPDATE_TIMEOUT);
	}
}
#endif

//#ifdef ENABLE_AUDIO_RECORD_SUPPORT

//#include "dsp_rg_ctl.h"

static UINT32 _gu4AudUploadMuxerCount = 0;
static UINT32 _gu4AudUploadQueueIndex = 0xffffffff;
static UINT8 _gu1AudUploadDataFlag = 0;

static UINT32 _gu4AudUploadDataPts = 0;
static UINT32 _gu4AudUploadDataPtsPrev = 0xffffffff;
static UINT32 _gu4AudUploadDataAddr = 0;
static UINT32 _gu4AudUploadDataSize = 0;

static UINT32 _gu4AudUploadDataBaseAdr = 0;
static UINT32 _gu4AudUploadDataTotalSize = 0;

static UINT8 _gu1AudUploadMode = 0; //0-off, 1-PCM, 2-MP3, 3-SBC

//#define ENC_TEST        /// Only for Test
#ifdef ENC_TEST
#define OUT_BUF_SIZE 1024*1024    ///1024*1024
static UINT8 _gu1OutBuf[OUT_BUF_SIZE];
static UINT32 _gu4OutBufLen = 0;
#endif

#ifdef CC_AUD_4_DECODER_SUPPORT
static UINT32 _gu4AudReadPtr = 0;
#endif

#define AUD_UPLOAD_DATA_QUEUE_MAX_NUM 128
#define AUD_DEC_UPLOAD_NUM  1

typedef struct __audUploadQueue {
    UINT32 u4StartAddr; // in bytes
    UINT32 u4Size; // in bytes
} _audUploadQueue_t;

typedef enum
{
    PCM_INIT_BITS = -1,
    PCM_16BITS = 0,
    PCM_24BITS = 1,

} UPLOAD_PCM_BITS;

typedef enum
{
    PCM_INIT_CH = -1,
    PCM_2CH = 0,
    PCM_1CH = 1,
    PCM_4CH = 2,

} UPLOAD_PCM_CHANNEL;

typedef enum
{
    PCM_INIT_SR = -1,
    PCM_48K = 0,
    PCM_44K = 1,
    PCM_32K = 2,

} UPLOAD_PCM_FS;

typedef struct __audUploadPCMInfo {
    UPLOAD_PCM_BITS     uPCMBits;        // in bytes
    UPLOAD_PCM_CHANNEL  uPCMChannels;    // in bytes
    UPLOAD_PCM_FS       uPCMSampleRate;  // in bytes
} _audUploadPCMInfo_t;

static _audUploadQueue_t _gu4AudUploadQueue[AUD_UPLOAD_DATA_QUEUE_MAX_NUM];
static _audUploadPCMInfo_t _guAudUploadPCMInfo[AUD_DEC_UPLOAD_NUM];

void _AUD_DspDataUploadNotify (UINT32 u4Index)  /// call by dsp_irq.c
{
    _gu4AudUploadQueueIndex = u4Index; //Daniel Hack??
    if (_gu1AudUploadDataFlag == 0x1)
    {
        VERIFY(x_sema_unlock(_hSemaDataUploadWait) == OSR_OK);
    }
}

UINT32 _AudDataUploadGetPts (void)
{
   return  _gu4AudUploadDataPts;
}

void _AUD_DataUploadSetMode(UINT8 u1Mode)
{
#ifdef CC_AUD_4_DECODER_SUPPORT
    if (_gu1AudUploadMode == 3)
    {
        AUD_DSPCmdStop(AUD_DEC_4TH);
        _gu4AudReadPtr = 0;
    }
#endif

    _gu1AudUploadMode = u1Mode;

#ifdef CC_AUD_4_DECODER_SUPPORT
    if (_gu1AudUploadMode == 3)
    {
        _gu4AudReadPtr = u4GetSBCEncFIFOStart();
        AUD_SetDecType(AUD_DEC_4TH, AUD_STREAM_FROM_OTHERS, AUD_FMT_SBCENC);
        AUD_DSPCmdPlay(AUD_DEC_4TH);
    }
#endif
}

UINT8 _AUD_DataUploadGetMode(void)
{
    return _gu1AudUploadMode;
}

void _AUD_UploadPCMInfo_Init(void)
{
    /// Set initial value for MW query.
    _guAudUploadPCMInfo[AUD_DEC_MAIN].uPCMBits = PCM_16BITS;
    _guAudUploadPCMInfo[AUD_DEC_MAIN].uPCMChannels = PCM_2CH;
    _guAudUploadPCMInfo[AUD_DEC_MAIN].uPCMSampleRate = PCM_48K;
}

void _AUD_DataUpload_Init (void)
{
    _gu1AudUploadDataFlag = 0;
    _gu4AudUploadMuxerCount = 0;
    _gu4AudUploadDataPts = 0;
    _gu4AudUploadDataPtsPrev = 0xffffffff;
    _gu4AudUploadDataAddr = 0;
    _gu4AudUploadDataSize = 0;
    _gu4AudUploadQueueIndex = 0xffffffff;

#ifdef ENC_TEST   /// Only for Test
    _gu4OutBufLen = 0;
#endif
}

void _AUD_DataUpload_start (void)
{
    _gu1AudUploadDataFlag = 0x1;
}

void _AUD_DataUploadQueueInit (UINT32 u4BaseAddr, UINT32 u4BlockSize, UINT32 u4BlockNum)
{
    UINT32 u4Blk, u4StartAddr;

    VERIFY(u4BlockNum <= uReadShmUINT8(B_UPLOAD_BLOCK_NUM));  /// AUD_UPLOAD_DATA_QUEUE_MAX_NUM);

    u4StartAddr = u4BaseAddr;

    _gu4AudUploadDataBaseAdr = u4BaseAddr;
    _gu4AudUploadDataTotalSize = u4BlockNum * u4BlockSize;

    for (u4Blk = 0; u4Blk < u4BlockNum; u4Blk ++)
    {
        _gu4AudUploadQueue[u4Blk].u4StartAddr = u4StartAddr;
        _gu4AudUploadQueue[u4Blk].u4Size = u4BlockSize;
        u4StartAddr += u4BlockSize;
    }
}

//-------------------------------------------------------------------------
/** _AUD_ReadPsrStc1
 *  Read Parser STC 1
 */
//-------------------------------------------------------------------------
UINT32 _AUD_ReadPsrStc1 (void)
{
    return PARSER_READ32(REG_V_STC_1);
}

//-------------------------------------------------------------------------
/** _AUD_SetPsrStc1
 *  Set Parser STC 1
 */
//-------------------------------------------------------------------------
void _AUD_SetPsrStc1 (UINT32 u4Val)
{
    PARSER_WRITE32(REG_V_STC_1, u4Val);
}


///#define AUD_UPLOAD_QUEUE_BLOCKSIZE 0x400
///#define AUD_UPLOAD_QUEUE_BLOCKNUM 64
///#define AUD_UPLOAD_BUFFER_BASE (0x39D00 * 4)

static void _AudMuxerInit (void)
{
    _gu4AudUploadMuxerCount = 0;
    _gu4AudUploadQueueIndex = 0xffffffff;

    _AUD_UploadPCMInfo_Init();

    _AUD_DataUploadQueueInit (_u4DspCmptBuf[TYPE_COMM_DRAM_IDX] + AUD_UPLOAD_BUFFER_BASE,
                                u2ReadShmUINT16(W_UPLOAD_BLOCK_SIZE),
                                uReadShmUINT8(B_UPLOAD_BLOCK_NUM));

    // enable upload data function
    //_gu1AudUploadDataFlag = 0x1;
    _gu1AudUploadDataFlag = 0;

    _gu4AudUploadDataPts = 0;

    _AUD_SetPsrStc1 (0);
}

static void _AudInformMuxer(UINT32 u4Pts, UINT32* u4StartAddr, UINT32 u4Len, UINT32 u4Index)
{
    // send msg to mux
    // picSa, picSz, PTS, picType
#ifdef CC_ATV_PVR_SUPPORT
    AENC_PES_INFO_T rAEncPes;
#endif
//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
AudDecNfyFct   pfAudDecNfy=NULL;
AudDecNfyFct   pfAudDecNfy1=NULL;
//#endif


#ifdef ENC_TEST   /// Only for Test
    if ((_gu4OutBufLen + u4Len) <= OUT_BUF_SIZE)
    {
        ///Make sure DRAM cache had flushed.
        //DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_COMM);
        DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);

        LOG (7, "Data upload to Muxer = (%d) Pts = 0x%X, Start Addr = 0x%X, length = 0x%X, location = 0x%X\n",
            u4Index, u4Pts, u4StartAddr, u4Len, &_gu1OutBuf[_gu4OutBufLen]);

        LOG (7, "0x%x \n", (UINT16 *)VIRTUAL(u4StartAddr));    ///first 2 byte


        x_memcpy ((VOID *)VIRTUAL(&_gu1OutBuf[_gu4OutBufLen]), (VOID *)VIRTUAL(u4StartAddr), u4Len);

        LOG (7, "0x%02x%02x   0x%02x%02x\n", _gu1OutBuf[_gu4OutBufLen], _gu1OutBuf[_gu4OutBufLen+1], _gu1OutBuf[_gu4OutBufLen+2], _gu1OutBuf[_gu4OutBufLen+3]);

        if((_gu1OutBuf[_gu4OutBufLen]==0)&&(_gu1OutBuf[_gu4OutBufLen+1]==0)&&(_gu1OutBuf[_gu4OutBufLen+2]==0)&&(_gu1OutBuf[_gu4OutBufLen+3]==0))
        {
            LOG (7, "@@@@@@@@@@@@@@@\n");
        }

        _gu4OutBufLen += u4Len;
    }
    else
    {
        LOG(7, "Buffer Address = (0x%x), Length = (0x%x).\n", VIRTUAL(&_gu1OutBuf[0]), _gu4OutBufLen);
    }
#else
    ///Make sure DRAM cache had flushed.
    //DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_COMM);
    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);
    LOG (7, "Data upload to Muxer = (%d) Pts = 0x%X, Start Addr = 0x%X, length = 0x%X\n",
            u4Index, u4Pts, u4StartAddr, u4Len);
#endif

#ifdef CC_ATV_PVR_SUPPORT
    rAEncPes.u4FifoStart = 0;
    rAEncPes.u4FifoEnd = 0;
    rAEncPes.u4StartAddr = u4StartAddr;
    rAEncPes.u4FrameSize = u4Len;
    rAEncPes.u4Pts = u4Pts;
    MUXER_SendAudioPes(0, &rAEncPes);
#endif
//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
    // Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy) == TRUE);
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy1) == TRUE);

    if (pfAudDecNfy != NULL)
    {
        pfAudDecNfy((void *)AUD_NFY_STREAM, 0, AUD_COND_UPLOAD_INFO_CHG, (UINT32)u4StartAddr, u4Len);
    }
	if (pfAudDecNfy1 != NULL)
    {
        pfAudDecNfy1((void *)AUD_NFY_YW, 0, (AUD_COND_NFY_T)AUD_YW_COND_UPLOAD_OK, (UINT32)u4StartAddr, u4Len);
    }
//#endif

}

static void _AudDataUploadSetPts (UINT32 u4Pts)
{
    _gu4AudUploadDataPts = u4Pts;
}

static void _AudDataUploadThread(void* pvArg)
{
    //UINT32 *u4QueueAddr;
    UINT32 u4UploadAddr, u4UploadSize;
    UINT32 u4Time;

	#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
	AUD_OSS_ADAP_MSG_T tMsg;
	AUD_OSS_ADAP_DEV_T devId;
	AUD_OSS_ADAP_ST_TYPE_T status;
	AUD_OSS_ADAP_SRC_TYPE_T source;
	#endif
    while (1)
    {
        VERIFY(x_sema_lock(_hSemaDataUploadWait, X_SEMA_OPTION_WAIT) == OSR_OK);

        u4UploadAddr = _gu4AudUploadQueue[_gu4AudUploadQueueIndex].u4StartAddr;
        u4UploadSize = (UINT32)(u2ReadShmUINT16(W_UPLOAD_BLOCK_SIZE) * uReadShmUINT8(B_UPLOAD_BLK_INT_CNT));

        u4Time = _AUD_ReadPsrStc1();
        _AudDataUploadSetPts(u4Time);
        _gu4AudUploadDataAddr = u4UploadAddr;
        _gu4AudUploadDataSize = u4UploadSize;

        _AudInformMuxer(u4Time, (UINT32 *)u4UploadAddr, u4UploadSize, _gu4AudUploadQueueIndex);

		#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
		devId = _AudOssAdap_GetDevId();
		status = _AudOssAdap_GetStatus();
		source = _AudOssAdap_GetSourceType();
		if((AUD_OSS_ADAP_ST_WORKING == status) && (AUD_OSS_ADAP_UPLOAD == source))
		{

			tMsg.devId = devId;
			tMsg.cmd = AUD_OSS_ADAP_TRANSFER;
			tMsg.dataAddr = (VIRTUAL(_gu4AudUploadDataAddr));
			tMsg.dataSize = _gu4AudUploadDataSize;

			_AudOssAdap_SendMsg(tMsg);
		}
		#endif

        _gu4AudUploadMuxerCount ++;
    }
}

void _AUD_GetUploadInfo(UINT32* u4Pts,UINT32* u4Addr, UINT32* u4Size)
{
#ifdef CC_AUD_4_DECODER_SUPPORT
    if (_gu1AudUploadMode==3)
    {
        UINT32 u4FifoSA,u4FifoEA,u4WP;

        //SBCEnc
        *u4Pts = 0;
        *u4Addr = 0;
        *u4Size = 0;

        u4FifoSA = u4GetSBCEncFIFOStart();
        u4FifoEA = u4GetSBCEncFIFOEnd();
        u4WP = u4GetSBCEncWritePnt();

        if ((u4WP >= u4FifoSA) && (u4WP < u4FifoEA))
        {
            if (_gu4AudReadPtr < u4WP)
            {
                if ((u4WP - _gu4AudReadPtr) > 0x1000)
                {
                    *u4Addr = _gu4AudReadPtr;
                }
            }
            else if (_gu4AudReadPtr > u4WP)
            {
                if (((u4FifoEA - u4FifoSA) - (_gu4AudReadPtr - u4WP)) > 0x1000)
                {
                    *u4Addr = _gu4AudReadPtr;
                }
            }
        }

        if (*u4Addr)
        {
            _gu4AudReadPtr += 0x1000;
            if (_gu4AudReadPtr == u4FifoEA)
                _gu4AudReadPtr = u4FifoSA;
            *u4Size = 0x1000;
        }
    }
    else
#endif
    {
        *u4Pts = _gu4AudUploadDataPts;
        *u4Addr = _gu4AudUploadDataAddr;
        *u4Size = _gu4AudUploadDataSize;
        if (_gu4AudUploadDataPtsPrev == _gu4AudUploadDataPts)
        {
            *u4Size = 0;
        }
        _gu4AudUploadDataPtsPrev = _gu4AudUploadDataPts;
    }
}

void _AUD_GetYWUploadInfo(UINT32* u4base, UINT32* u4Size)
{
    *u4base = _gu4AudUploadDataBaseAdr;
    *u4Size = _gu4AudUploadDataTotalSize;
}

void _AUD_GetUploadPCMInfo(UINT32* uPCMBits, UINT32* uPCMChannels, UINT32* uPCMSampleRate)
{
  if (_arAudDecoder[AUD_DEC_MAIN].eDecState == AUD_DEC_PLAYING)
    {
        *uPCMBits = _guAudUploadPCMInfo[AUD_DEC_MAIN].uPCMBits;
        *uPCMChannels = _guAudUploadPCMInfo[AUD_DEC_MAIN].uPCMChannels;
        *uPCMSampleRate = _guAudUploadPCMInfo[AUD_DEC_MAIN].uPCMSampleRate;
    }
    else
    {
        *uPCMBits = (UINT32)PCM_INIT_BITS;
        *uPCMChannels = (UINT32)PCM_INIT_CH;
        *uPCMSampleRate =(UINT32) PCM_INIT_SR;
    }
}

BOOL _AUD_Dec3_Is_Mp3Enc(void)
{
    if(_arAudDecoder[AUD_DEC_THIRD].eDecFormat != AUD_FMT_MP3ENC)
    {
        return FALSE;
    }
    return TRUE;
}

//#endif

#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER

static UINT32 _gu4AudDataTransferAddr = 0;
static UINT32 _gu4AudDataTransferSize = 0;

void _AudDataTransferNotify (void)  /// call by dsp_irq.c
{
	VERIFY(x_sema_unlock(_hSemaDataTransferWait) == OSR_OK);
}

void _AudDataTransferLock(void)  /// call by dsp_irq.c
{
	VERIFY(x_sema_lock(_hSemaDataTransferWait1, X_SEMA_OPTION_WAIT) == OSR_OK);
}


void _AudDataTransferUpdate(UINT32 u4Addr, UINT32 u4Size)
{
	_gu4AudDataTransferAddr = u4Addr;
	_gu4AudDataTransferSize = u4Size;
}
static void _AudDataTransferThread(void* pvArg)
{
	AUD_OSS_ADAP_MSG_T tMsg;
	AUD_OSS_ADAP_DEV_T devId;
	AUD_OSS_ADAP_ST_TYPE_T status;
    while (1)
    {
        VERIFY(x_sema_lock(_hSemaDataTransferWait, X_SEMA_OPTION_WAIT) == OSR_OK);

		devId = _AudOssAdap_GetDevId();
		status = _AudOssAdap_GetStatus();
		if(AUD_OSS_ADAP_ST_WORKING == status)
		{
			tMsg.devId = devId;
			tMsg.cmd = AUD_OSS_ADAP_TRANSFER;
			tMsg.dataAddr = _gu4AudDataTransferAddr;
			tMsg.dataSize = _gu4AudDataTransferSize;

			_AudOssAdap_SendMsg(tMsg);

			VERIFY(x_sema_unlock(_hSemaDataTransferWait1) == OSR_OK);
		}
    }
}

#endif


void AUD_DrvCheckSpeedTime(UINT8 u1DecId)
{
    static UINT16 _u2Cnt = 0;
    UINT16 u2Speed = 0;
    UINT32 u4ReadPtr;
    UINT32 u4WritePtr;
    UINT32 u4PTS;
    UINT32 u4DataBitrate;
    UINT32 u4Offset = 0;

    UNUSED(AUD_DrvCheckSpeedTime);
    UNUSED(u4WritePtr);
    AUD_DEC_ID_VALIDATE(u1DecId);

    u4DataBitrate = DSP_GetDataRate(u1DecId);
    if (u4DataBitrate > 64000)   //only handle low bitrate file.
    {
        return;
    }

    _AUD_DMX_GetAudPtr(u1DecId, &u4ReadPtr, &u4WritePtr);
    _AUD_DspGetSpeed(u1DecId, &u2Speed);
    if (u2Speed > 0)            //only for Fast Forward
    {
        if (_u2Cnt < (500/AUD_PTR_UPDATE_TIMEOUT))  //every 500 ms update once.
        {
            _u2Cnt++;
            return;
        }
        _u2Cnt = 0;
        AUD_MMFindSyncInfo(u1DecId,&u4PTS,u4ReadPtr);

        if(u4PTS == 0)
        {
            return;
        }

        if (u4DataBitrate <= 12000)
        {
            u4Offset = 93000;   // 1.03 s
        }
        else if ((u4DataBitrate > 12000) && (u4DataBitrate <= 32000))
        {
            u4Offset = 50000;   // 0.55 s
        }
        else if (u4DataBitrate > 32000)
        {
            u4Offset = 15000;   // 0.16 s
        }

        if (u4PTS >= u4Offset)     // for aout is slower than Rp
        {
            u4PTS -= u4Offset;
        }
        
        if (u4PTS < STC_GetStcValue(STC_SRC_A1))
        {
            return;
        }
        
        STC_StopStc(STC_SRC_A1);
        STC_SetStcValue(STC_SRC_A1, u4PTS);
        //LOG(6, "update STC = 0x%x\n",u4PTS );
        return;
    }
    else
    {
        _u2Cnt = 0;
    }
}

//-----------------------------------------------------------------------------
/** AUD_DrvThreadWakeup
 *  Driver thread wakeup
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DrvThreadWakeup(UINT8 u1DecId)
{
    if (u1DecId == AUD_DEC_MAIN)
    {
        vADSPPowerON();
    }

    while (fgADSPIsIdle())
    {
        x_thread_delay(1);
    }

    _ChangeAudioState(u1DecId, AUD_IDLE);

    _arAudDecoder[u1DecId].eDecState = AUD_DEC_STOP;
    _afgIssuePlayComToDsp[u1DecId] = FALSE;

    // Set event for IDLE state
    _AudSetEvent(u1DecId, AUD_CMD_FLAG_PLAY   |
        AUD_CMD_FLAG_STOP   |
        AUD_CMD_FLAG_RESET  |
        AUD_CMD_FLAG_RESUME |
        AUD_CMD_FLAG_PAUSE |
        AUD_CMD_FLAG_LOADCODE);
}

//-----------------------------------------------------------------------------
/** AUD_DrvSuspend
 *  Driver suspend
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DrvSuspend(void)
{
    VERIFY(x_sema_unlock(_arAudDrvStateSema[AUD_DEC_MAIN]) == OSR_OK);
    VERIFY(x_sema_unlock(_ahSemaUopComDone[AUD_DEC_MAIN]) == OSR_OK);

    VERIFY(x_sema_unlock(_arAudDrvStateSema[AUD_DEC_AUX]) == OSR_OK);
    VERIFY(x_sema_unlock(_ahSemaUopComDone[AUD_DEC_AUX]) == OSR_OK);

#ifdef CC_MT5391_AUD_3_DECODER
    VERIFY(x_sema_unlock(_arAudDrvStateSema[AUD_DEC_THIRD]) == OSR_OK);
    VERIFY(x_sema_unlock(_ahSemaUopComDone[AUD_DEC_THIRD]) == OSR_OK);
#endif

#ifdef CC_AUD_4_DECODER_SUPPORT
    VERIFY(x_sema_unlock(_arAudDrvStateSema[AUD_DEC_4TH]) == OSR_OK);
    VERIFY(x_sema_unlock(_ahSemaUopComDone[AUD_DEC_4TH]) == OSR_OK);
#endif
}

void AUD_DrvLogStcDiff(UINT8 u1DecId)
{

    INT32 i4StcLog = 0;
    UINT32 u4Temp = 0;

    UNUSED(AUD_DrvLogStcDiff);
    UNUSED(i4StcLog);
    UNUSED(u4Temp);

    u4Temp = u4ReadD2RInfo_StcDiff(u1DecId);

    if (u4Temp != 0)
    {
        if (u4Temp & 0x800000)
    	{
            // (-) negative
            i4StcLog = (INT32)(u4Temp | 0xFF000000);
        }
        else
        {
            // (+) positive
            i4StcLog = (INT32)u4Temp;
        }
        if ((AUD_GetSyncDbgLvl()&AUD_DBG_SYNC_STC_DIFF) == AUD_DBG_SYNC_STC_DIFF)
        {
            LOG(5,"Dec(%d) Stc_diff = %d\n", u1DecId, i4StcLog);
        }
    }
}

#ifdef LINUX_TURNKEY_SOLUTION
#include <linux/module.h>

void AUD_InitALSAPlayback(void)
{
    AUD_PCM_SETTING_T  rPcmSetting;

    rPcmSetting.eSampleFreq = FS_48K;
    rPcmSetting.ePcmDataInfo = PCM_16_BITS_LITTLE_ENDIAN;
    skip_create_feed_stream = 1;

    AUD_DSPCmdStop(AUD_DEC_MAIN);
    AUD_DspProcMode(AUD_DEC_MAIN, 0xdc00);
    AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_MEMORY, AUD_FMT_PCM);
    AUD_PcmSetting(AUD_DEC_MAIN, (const AUD_PCM_SETTING_T *)&rPcmSetting);
    AUD_DSPCmdPlay(AUD_DEC_MAIN);
}
EXPORT_SYMBOL(AUD_InitALSAPlayback);

void AUD_DeInitALSAPlayback(void)
{
    AUD_DSPCmdStop(AUD_DEC_MAIN);
    skip_create_feed_stream = 0;
}
EXPORT_SYMBOL(AUD_DeInitALSAPlayback);

void AUD_InitALSAPlayback_MixSnd(void)
{
    //DANIEL_HACK
    AUD_Init();
    AUD_OutPadEnable(AUD_DEC_MAIN, TRUE);
    AUD_OutPadEnable(AUD_DEC_AUX, TRUE);
    ADAC_Mute(FALSE);
    //DANIEL_HACK
#ifdef ALSA_MIXSND_PATH
    //AUD_EnableMixSndClip();
    AUD_EnableAlsaMixSnd();
    AUD_PlayMixSndRingFifo(48000,1,16, u4GetMixSoundBufSize3()); //init value
#else
    AUD_EnableMixSndClip();
    AUD_PlayMixSndClip(0);
#endif
}
EXPORT_SYMBOL(AUD_InitALSAPlayback_MixSnd);

void AUD_DeInitALSAPlayback_MixSnd(void)
{
#ifdef ALSA_MIXSND_PATH
    AUD_StopMixSndRingFifo();
    #if !defined(CC_MT5368) && !defined(CC_MT5396) && !defined(CC_MT5389)
    AUD_DisableMixSndClip();
    #endif
#else
    AUD_StopMixSndClip(0);
    AUD_DisableMixSndClip();
#endif
}
EXPORT_SYMBOL(AUD_DeInitALSAPlayback_MixSnd);

void AUD_InitALSARecordLineIn(void)
{
    UINT32 u4FifoSA;
    UINT32 u4FifoEA;
    UINT32 u4FifoSZ;

#if 0
    //Use AFIFO1 0x48000 size for test
    u4FifoSA = u4GetAFIFOStart(0);
    u4FifoEA = u4GetAFIFOEnd(0);
    //TEST
    //u4FifoSZ = ((u4FifoEA - u4FifoSA)&0xffff0000); //size 4096 bytes aligned
    //u4FifoEA = u4FifoSA + u4FifoSZ;
    //TEST
#else
    //USE AFIFO2 0x21c00 size for test (but we only use 0x20000)
    u4FifoSA = u4GetAFIFOStart(1);
    u4FifoEA = u4GetAFIFOEnd(1);
    //TEST
    u4FifoSZ = ((u4FifoEA - u4FifoSA)&0xffff0000); //size 0x10000 bytes aligned (just for test)
    u4FifoEA = u4FifoSA + u4FifoSZ;
    //TEST
#endif

    //TODO
    (*((volatile UINT32*)(0xf000501c)))=0x8c0; //Disable Line-In
    (*((volatile UINT32*)(0xf0005010)))=0x172e172e;
    (*((volatile UINT32*)(0xf0005024)))= (((u4FifoSA&0xffff00)<<8) | ((u4FifoEA&0xffff00)>>8));
    //(*((volatile UINT32*)(0xf0005040)))=0x300610;
    (*((volatile UINT32*)(0xf000d26c)))=0x00170001; //Original - 0x110001
    (*((volatile UINT32*)(0xf0005248)))=0x004804c0; //Input Selection   ([15:12] = F, power down internal adc)
    (*((volatile UINT32*)(0xf000501c)))=0x8c9; //Enable Line-In + LSB 16bits
}
EXPORT_SYMBOL(AUD_InitALSARecordLineIn);

void AUD_DeInitALSARecordLineIn(void)
{
    //TODO
    (*((volatile UINT32*)(0xf000501c)))=0x8c0; //Disable Line-In
    (*((volatile UINT32*)(0xf000d26c)))=0x00110001;
}
EXPORT_SYMBOL(AUD_DeInitALSARecordLineIn);

void AUD_InitALSARecordSpeaker(void)
{
    AUD_DspSetUploadMode(4);
    AUD_DspUploadDataEnable();
}
EXPORT_SYMBOL(AUD_InitALSARecordSpeaker);

void AUD_DeInitALSARecordSpeaker(void)
{
    AUD_DspSetUploadMode(0);
    AUD_DspUploadDataEnable();
}
EXPORT_SYMBOL(AUD_DeInitALSARecordSpeaker);

void AUD_InitALSARecordSBC(void)
{
    AUD_DspSetUploadMode(3);
}
EXPORT_SYMBOL(AUD_InitALSARecordSBC);

void AUD_DeInitALSARecordSBC(void)
{
    AUD_DspSetUploadMode(0);
}
EXPORT_SYMBOL(AUD_DeInitALSARecordSBC);

#endif
