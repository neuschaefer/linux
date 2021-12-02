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
 * $Date: 2012/06/28 $
 * $RCSfile: aud_drv.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_drv.h
 *  Brief of file aud_drv.h.
 *  Details of file aud_drv.h (optional).
 */

#ifndef _AUD_DRV_H_
#define _AUD_DRV_H_

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_typedef.h"
//#include "x_aud_dec.h"
#include "aud_if.h"
#include "dmx_if.h"
#include "feeder_if.h"

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

/* move thread priority and stack size setting to drv_common.h */
/*
#define AUD_DRV_THREAD_STACK_SIZE               2048
#define AUD_DRV_THREAD_PRIORITY            		101
*/
#define AUD_DRV_THREAD1_NAME                    "AudDrv1"
#define AUD_DRV_THREAD2_NAME                    "AudDrv2"
#ifdef CC_MT5391_AUD_3_DECODER
#define AUD_DRV_THREAD3_NAME                    "AudDrv3"
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
#define AUD_DRV_THREAD4_NAME                    "AudDrv4"
#endif
#define AUD_DRV_FEEDSTR_THREAD_NAME             "AudFeedStr"
#define AUD_APLL_MONITOR_THREAD_NAME      	    "AudApllMon"
#define AUD_DSP_MONITOR_THREAD_NAME             "AudDspMon"  //  -- DSP Force --
#define AUD_DRV_FEEDER_THREAD                   "AudFeeder"
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
#define AUD_DRV_Parser_THREAD                   "AudHDMIParser"
#endif
#ifdef CC_AUD_NCSTOOL_SUPPORT
#define AUD_TOOL_THREAD_NAME                    "AudTool"
#endif
#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
#define AUD_DRV_FEEDMIXSND_THREAD_NAME          "AudFeedMixSnd"
#define AUD_DRV_ALSA_FEEDMIXSND_THREAD_NAME          "AudAlsaFeedMixSnd"
#endif
//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
#define AUD_DRV_DATA_UPLOAD_THREAD                   "AudDataUpload"
//#endif

#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
#define AUD_DRV_DATA_TRANSFER_THREAD                   "AudDataTransfer"
#endif

#ifdef CC_ENABLE_AV_SYNC
#define AUD_DRV_AV_SYNC_THREAD                  "AudSync"
#endif
//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

typedef enum _DECODER_STATE_T
{
    AUD_DEC_STOP = 0,
    AUD_DEC_PLAYING,
    AUD_DEC_PAUSE,
    AUD_DEC_INIT
}   DECODER_STATE_T;

typedef struct _AUD_DECODER_T
{
    AUD_FMT_T    					eDecFormat;
    AUD_DEC_STREAM_FROM_T           eStreamFrom;
    MEM_BUFFER_INFO_T 		        rMemBuffer;
    AV_SYNC_MODE_T                  eSynMode;
    AUD_MM_MODE_T                   eMultimediaMode;
    AV_SYNC_STC_SELECT_T            eStcId;
    DECODER_STATE_T                 eDecState;
    BOOL                            fgAvSynLock;
    BOOL                            fgOperationMode; // FALSE: push mode TRUE:pull mode
    BOOL                            fgMMAoutReady;
    BOOL                            fgMMAoutEnable;
    UINT32                          u4EventFlag;
    DMX_AUDIO_PES_T                 rFirstReceivePes;
    DMX_AUDIO_PES_T                 rFirstAvaibleReceivePes;
    DMX_AUDIO_PES_T                 rFristDecodePes;
    DMX_AUDIO_PES_T                 rCurrentPes;
    UINT32                          u4StartPts;
    UINT32                          u4ReceivePesCount;
    UINT32                          u4ReceiveValidPesCount;
    UINT32                          u4PesCntAfterIPicReceived;
    UINT8                           u1DmxPidIdx;
    AUD_NFY_INFO_T                  rNfyInfo;
    AUD_PCM_SETTING_T               rPcmSetting;
    UINT32                          u4FifoStart;
    UINT32                          u4FifoEnd;
    UINT32                          u4FifoSz;
} AUD_DECODER_T;

typedef struct
{
    UINT32       u4PtsWP;
    UINT32       u4PTS; 
} AUD_MM_SYNC_INFO_T;
typedef struct
{
    UINT32       u4PtsWP;
    UINT32       u4Tick; 
} AUD_MM_TICK_INFO_T;

typedef enum _AUD_DSP_CB_T
{
    AUD_DSP_CB_UNMUTE= 0,
    AUD_DSP_CB_NONE    
} AUD_DSP_CB_T;
//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

extern void AUD_DRVInit(void);
extern AUD_DRV_STATE_T AUD_DRVGetAudioState(UINT8 ucDecId);
extern BOOL AUD_DRVCmd(UINT8 ucDecId, AUD_DEC_CMD_T eCmd);
extern BOOL AUD_DRVSetDecodeType(UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt);
extern BOOL AUD_DRVSetStreamFrom(UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T eStreamFrom);
extern BOOL AUD_DRVSetDecMemBuffer(UINT8 ucDecId, const MEM_BUFFER_INFO_T * prDecMemBuffer);
extern BOOL AUD_DRVChangeDecodeFormat(UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt);
//extern BOOL AUD_SendPts(UCHAR ucChannel, UCHAR ucDevId, const PSR_AUDIO_PES_T * prPes);
extern AV_SYNC_MODE_T AUD_GetAvSynMode(UCHAR ucDecId);
extern void AUD_SetAvSynMode(UCHAR ucDecId, AV_SYNC_MODE_T eSynMode);
extern AUD_MM_MODE_T AUD_DrvGetMultimediaMode(UINT8 u1DecId);
extern void AUD_DrvSetMultimediaMode(UINT8 u1DecId, AUD_MM_MODE_T eMultimediaMode);
extern void AUD_DrvSetAdMode(BOOL fgAdMode);
extern BOOL AUD_AvSynLock(UCHAR ucDecId); // Check if AV is syn
extern void AUD_SetInputMode(UCHAR ucDecId, BOOL fgMode);
extern BOOL AUD_GetInputMode(UCHAR ucDecId);// FALSE: push mode TRUE:pull mode
extern AV_SYNC_STC_SELECT_T AUD_GetStcId(UCHAR ucDecId);
extern void AUD_SetStcId(UCHAR ucDecId, AV_SYNC_STC_SELECT_T eStcId);
extern BOOL AUD_GetOperationMode(UCHAR ucDecId); // FALSE: push mode TRUE:pull mode
extern void AUD_SetOperationMode(UCHAR ucDecId, BOOL fgMode); // FALSE: push mode TRUE:pull mode
//extern void AUD_GetFisrtAudioDecodePes(UINT8 u1DecId, PSR_AUDIO_PES_T * prAudioPes);
extern void AUD_GetDecoderStatus(UINT8 u1DecId, AUD_DECODER_T *prAudDec);
extern BOOL AUD_DRVGetDecodeType(UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T * peStreamFrom, 
	                             AUD_FMT_T * peDecType);
extern BOOL AUD_DRVGetStreamFrom(UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T * peStreamFrom);
//extern void AUD_GetFisrtAudioDecodePes(UINT8 u1DecId, PSR_AUDIO_PES_T * prAudioPes);
extern void AUD_DrvSetNfy(UINT8 u1DecId, const AUD_NFY_INFO_T * prAudNfyInfo);
extern void AUD_DrvGetNfy(UINT8 u1DecId, AUD_NFY_INFO_T * prAudNfyInfo);
extern void _AUD_AtvAvInitialSync(BOOL fgStable);
extern void AUD_ResetDspReadPtr(UINT8 u1DecId);
extern BOOL AUD_IsDualDecMode(void);
extern BOOL AUD_IsSpdifInSrc(UINT8 u1DecId);
extern void AUD_DetectionSyncSema(BOOL fgLock);
extern void AUD_StopMemStr(void);
extern void AUD_AudioPTSDelayEnable(UINT8 u1DecId, UINT8 uDlyFactor);
extern BOOL AUD_DrvFeederSetRequestInfo(FEEDER_TRANSMIT *prTransmit);
extern void AUD_ScartOutInit(void);
extern void AUD_AtunerSigStable(void);
extern void AUD_DrvPcmSetting(UINT8 u1DecId, const AUD_PCM_SETTING_T * prPcmSetting);
extern void AUD_DrvSetNotifyFunc(UINT8 u1DecId, AUD_CLIP_NOTIFY pfClipNotify);
extern BOOL AUD_DrvDtvLock(UINT8 u1DecId);
extern void AUD_DrvATunerLock(void);
extern void AUD_DrvSetAvSynMode(UINT8 u1DecId, AV_SYNC_MODE_T eSynMode);
extern void AUD_DrvSetPIPVideoPath(UINT8 u1DecId, UINT8 u1VideoPath);
extern void _AudSetTvSysMask(UINT8 u1DecId, UINT32 u4Mask);
extern UINT32 _AudGetTvSysMask(UINT8 u1DecId);
#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
extern void _AudSetTvAudSysMask(UINT8 u1DecId, UINT32 u4TvMask, UINT32 u4TvAudMask, ISO_3166_COUNT_T country_code);
#endif
extern AUD_FMT_T _AudAtvFmtDetection(void);
extern void  AUD_DspHdmiChangeFormatMuteEnable(UINT8 u1DecId, BOOL fgEnable);
extern BOOL AUD_DrvDtvLock(UINT8 u1DecId);
extern void AUD_SendApllAdjustCmd(UINT32 u4Level);
extern void AUD_MMQueueSyncInfo(UINT8 u1DecId, UINT32 u4PTS, UINT32 u4PTSWp, BOOL fgReset);
extern void AUD_MMFindSyncInfo(UINT8 u1DecId, UINT32* u4PTS, UINT32 u4PTSWp);
extern void AUD_MMQueueTickInfo(UINT8 u1DecId, UINT32 u4Tick, UINT32 u4PTSWp, BOOL fgReset);
extern void AUD_DrvMMAoutEnable(UINT8 u1DecId, BOOL fgEnable);
#ifndef CC_AUD_SKYPE_SUPPORT
extern void AUD_DrvMMAoutEnable_Flag(BOOL fgEnable);
extern void AUD_DrvMMAoutReady_Flag(BOOL fgEnable);
#else
extern void AUD_DrvMMAoutEnable_Flag(UINT8 u1DecId, BOOL fgEnable);
extern void AUD_DrvMMAoutReady_Flag(UINT8 u1DecId, BOOL fgEnable);
#endif
void AUD_SentReaptNumber(UINT8 u1DecId, UINT32 ReaptNumber);
extern BOOL AUD_IsMMAVInitSync(UINT8 u1DecId);
extern BOOL AUD_IsMMAoutEnable(UINT8 u1DecId);
extern BOOL AUD_IsMMAoutReady(UINT8 u1DecId);
extern void _AudClearEOSFlag(UINT8 u1DecId);
extern void AUD_DrvClearPesCnt(UINT8 u1DecId);
extern AUD_DEC_STREAM_FROM_T _AudGetStrSource(UINT8 u1DecId);
extern AUD_DECODER_T * AUD_DrvGetDecodeState(UINT8 u1DecId);
extern void AUD_DrvMMAoutEnableNotify(UINT8 u1DecId);
extern void AUD_DrvMMNoDataNotify(UINT8 u1DecId);

extern void _AUD_DspStopMuteEnable(UINT8 u1DecId, BOOL fgEnable);
extern AUD_FMT_T _AudGetStrFormat(UINT8 u1DecId);
#ifdef CC_ENABLE_AV_SYNC
extern BOOL AUD_IsIgnoreVdpInfo (void);
extern void AUD_DspDecPlayMuteEnable(UINT8 u1DecId, BOOL fgEnable);
extern void AUD_VdpReady(BOOL fgEnable);
extern void _AudSrmStableCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);
extern void _AudSrmAdjustDelayByAudFmt(UINT16* u2Delay);
extern BOOL        _fgwaitForAoutReinit;
extern BOOL        _fgSetChannelDelay;
extern BOOL        _fgwaitForPIPAoutReinit;
#endif
extern UINT8 AUD_DrvGetPIPVideoPath(void);

extern void AUD_SET_SWDMXSrcId(UINT8 u1DecId, UINT8 u1SwdmxSrcId);
extern void AUD_GET_SWDMXSrcId(UINT8 u1DecId, UINT8* u1SwdmxSrcId);
extern void AUD_DrvCallback(UINT8 u1DecId, AUD_DSP_CB_T eAudDspCbType);
extern void AUD_GetAudioPts(UINT8 u1DecId, UINT32 * prAudioPts);
extern void AUD_DrvGetAudioTrickPts(UINT8 u1DecId, UINT32 *prAudioPts, BOOL fgOldPts);

//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
extern void _AUD_DspDataUploadNotify (UINT32 u4Index);
//#endif

extern BOOL AUD_GetAdPanEnable(void);
extern BOOL AUD_GetAdFadeEnable(void);

extern void AUD_DrvCheckSpeedTime(UINT8 u1DecId);

extern void AUD_DrvThreadWakeup(UINT8 u1DecId);
extern void AUD_DrvSuspend(void);
extern void AUD_DrvLogStcDiff(UINT8 u1DecId);
#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
extern void _AudDataTransferNotify (void);
extern void _AudDataTransferLock(void);
extern void _AudDataTransferUpdate(UINT32 u4Addr, UINT32 u4Size);
#endif


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

#endif /* _AUD_DRV_H_ */

