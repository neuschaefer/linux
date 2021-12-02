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
 * $RCSfile: aud_dsp_cfg.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_dsp_cfg.h
 *  Brief of file aud_dsp_cfg.h.
 *  Details of file aud_dsp_cfg.h (optional).
 */

#ifndef _AUD_DSP_CFG_H_
#define _AUD_DSP_CFG_H_

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_typedef.h"
#include "aud_if.h"

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
// Volume
#define MAX_VOL_LEVEL               (100)
#define VOL_ZERO_BASE               (0)

// Virutal surround constant
#define VSURR_GAIN_MAX  16

/*
// For equalizer preset
#define AUD_EQ_TYPE_NONE  0
#define AUD_EQ_ROCK       1
#define AUD_EQ_POP        2
#define AUD_EQ_LIVE       3
#define AUD_EQ_DANCE      4
#define AUD_EQ_TECHNO     5
#define AUD_EQ_CLASSIC    6
#define AUD_EQ_SOFT       7
#define AUD_EQ_TYPE_MAX   9
*/

#define AUD_EQ_BAND_1       1

#ifdef CC_AUD_7BAND_EQUALIZER
#define CH_EQ_BAND_NO   7
#define AUD_EQ_IIR_BAND_NUM     7
#define AUD_EQ_BAND_LAST       7
#else
#define CH_EQ_BAND_NO   5
#define AUD_EQ_IIR_BAND_NUM     8
#define AUD_EQ_BAND_LAST       5
#endif

#define EQ_BAND_DRY   0
#define EQ_BAND_MAX   (10 + 1)

#ifdef CC_AUD_EQ_SETA
#define EQ_BAND_GAIN_DB_MAX             10
#else
#define EQ_BAND_GAIN_DB_MAX             14
#endif

#define EQ_BAND_GAIN_INDEX_MAX          (((EQ_BAND_GAIN_DB_MAX*2)*2)+1) // range -14 ~ +14 dB, 0.5 dB/step
#define EQ_BAND_GAIN_SIDE_INDEX_MAX     (EQ_BAND_GAIN_INDEX_MAX >> 1)
#define EQ_BAND_GAIN_MID                (EQ_BAND_GAIN_INDEX_MAX >> 1)

#define EQ_DRY_GAIN_INDEX_MAX           (((10*2)*2)+1)  // range -10 ~ +10 dB, 0.5 dB/step
#define EQ_DRY_GAIN_SIDE_INDEX_MAX     (EQ_DRY_GAIN_INDEX_MAX >> 1)
#define EQ_DRY_GAIN_MID                 (EQ_DRY_GAIN_INDEX_MAX >> 1)

// Reverb constant
#define AUD_REVERB_MODE_OFF     (0)
#define AUD_REVERB_MODE_MAX     (7)

#define DRC_RANGE_MAX (9)

#define BYPASS_BASS_MANAGEMENT                                 (0x1 << 12) 
#define BYPASS_POST_PROCESSING                                 (0x1 << 13)
#define LFE_INPUT_GAIN_0_DB                                    (0x1 << 6)

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
extern  AUD_IEC_T _GetIECFlag(UINT8 u1DecId);
extern void _AUD_DspCfgSetDecType(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt);
extern void _AUD_DspFineTuneVolume(UINT8 u1DecId, AUD_CH_T eChannel, UINT32 u4ShmValue);
extern void _AUD_DspVolumeInit(UINT8 u1DecId);
extern void _AUD_DspEqCfgInit(void);
#ifdef CC_AUD_SOUND_MODE
extern void _AUD_DspPSMCfgInit(void);
extern void _AUD_DspPostSoundModeChange(UINT8 u1DecId, UINT8 u1ModeIdx);
extern void _AUD_DspVSMCfgInit(void);
#endif
extern void _AUD_DspPostSoundMode(UINT8 u1DecId, UINT8 u1ModeIdx);
#ifdef CC_AUD_VOLUME_OFFSET
extern void _AUD_DspVolumeOffset(UINT8 u1DecId,UINT8 u1Index);
#endif
extern void _AUD_DspChDelayInit(void);
extern void _AUD_DspSpdifInit(void);
extern void _AUD_DspReverbEnable(UINT8 u1DecId, BOOL fgEnable);
extern void _AUD_DspReverbMode(UINT8 u1DecId, UINT8 u1Mode) ;
extern void _AUD_DspChannelEqEnable(UINT8 u1DecId, BOOL fgEnable) ;
extern void _AUD_DspSpkSizeCfg(UINT8 u1DecId, UINT32 u4SpkSize);
extern void _AUD_DspBassBoostEnable(UINT8 u1DecId, BOOL fgEnable) ;
extern void _AUD_DspBassBoostGain(UINT8 u1DecId, UINT8 u1Level);
extern UINT32 _AUD_DspGetBassBoostGain(UINT8 u1DecId);
extern void _AUD_DspBassBoostGain(UINT8 u1DecId, UINT8 u1Level);
extern void _AUD_DspSuperBassEnable(UINT8 u1DecId, BOOL fgEnable);
extern void _AUD_DspClearBoostEnable(UINT8 u1DecId, BOOL fgEnable);
extern void _AUD_DspClearBoostGain(UINT8 u1DecId, UINT8 u1Level);
extern UINT32 _AUD_DspGetClearBoostGain(UINT8 u1DecId);
extern void _AUD_DspEqEnable(UINT8 u1DecId, BOOL fgEnable);
extern void _AUD_DspChEqBand(UINT8 u1DecId, UINT8 u1BandIdx, UINT8 u1Level, BOOL fgNotify) ;
extern void _AUD_DspChEqLoadPreset(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType) ;
extern void _AUD_DspGetEqType(UINT8 u1DecId, AUD_EQ_TYPE_T* eEqType);
extern void _AUD_DspChEqBandMode(UINT8 u1DecId, UINT8 u1BandIdx, UINT8 u1Level, BOOL fgNotify) ;
extern void _AUD_DspChEqLoadPresetChgMode(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType) ;
extern void _AUD_DspSetLimiterMode(UINT8 u1Mode);
extern void _AUD_DspSetLimiterFlag(UINT8 u1Flag);
extern void _AUD_DspSetLimiterThreshold(UINT32 u4Thre);
#ifdef CC_RATIO_LIMITER
extern void _AUD_DspSetLimiterGainRatio(UINT32 u4Ratio);
#endif
extern void _AUD_DspGetLimiterConfig(UINT8 *pu1Mode, UINT16 *pu2Flag, UINT32 *pu4Thre);
extern void _AUD_DspPL2Config(UINT8 uCtrl, UINT8 uMode);
extern void _AUD_DspPL2ConfigInit(UINT16 u2PL2cfg,UINT16 u2PL2Mode) ;
extern void _AUD_DspAC3KaraMode(UINT16 u2KaraMode,UINT8 uDecIndx) ;
extern void _AUD_DspAC3DualMono(UINT16 u2DMMode,UINT8 uDecIndx) ;
extern void _AUD_DspAC3CompMode(UINT16 u2CompMode,UINT8 uDecIndx) ;
extern UINT16 _AUD_DspGetAC3CompMode(UINT8 uDecIndx) ;
extern void _AUD_DspAC3DRCRange(UINT8 uDRCLevel,UINT8 u1DecId);
extern UINT8 _AUD_DspGetAC3DRCRange(UINT8 u1DecId);
extern void _AUD_DspSpeakerConfig(UINT32 u4SpkCfg,UINT8 uDecIndex);
extern void _AUD_DspSpeakerSizeConfig(UINT8 uDecIndex, BOOL fgFrontLarge, BOOL fgSurroundLarge, BOOL fgCenterLarge);
extern void _AUD_DspGetSpeakerConfig(UINT8 u1DecIndex, UINT32 *pu4SpkCfg);
extern void _AUD_DspSpeakerLargeSmallConfig(UINT8 u1DecIndex, UINT8 u1FrontPair, UINT8 u2Center, UINT8 u1Surround, UINT8 u1CH910);
extern void _AUD_DspSpeakerLsConfig(UINT8 u1DecIndex, UINT8 u1L, UINT8 u1R, UINT8 u1Ls, UINT8 u1Rs, UINT8 u1C);
extern void _AUD_DspSpeakerOutputConfig(UINT8 u1DecIndex, UINT8 u1SpkCfg);
extern void _AUD_DspSpeakerSubwooferEnable(UINT8 u1DecIndex, BOOL fgEnable);
extern void _AUD_DspChannelDelay(UINT16 u2Delay, AUD_CH_T eChIndex, UINT8 uDecIndex);
#ifdef CC_ENABLE_AV_SYNC
extern void _AUD_DspChannelDelay_initial(UINT16 u2Delay, AUD_CH_T eChIndex, UINT8 uDecIndex);
#endif
extern void _AUD_DspInputChannelDelay(AUD_DEC_STREAM_FROM_T eStreamFrom);
extern UINT8 _AUD_DspGetChannelVolume(UINT8 u1DecId, AUD_CH_T eChannel);
extern void _AUD_DspChannelVolume(UINT8 u1DecId, AUD_CH_T eChannel, UINT8 u1Value);
#ifdef CC_AUD_LINEOUT_VOL_OFFSET
extern void _AUD_DspLineOutVolOffset( UINT8 u1DecId, UINT8 u1Value);
#endif
extern void _AUD_DspChannelVolShmRawValue(UINT8 u1DecId, AUD_CH_T eChannel, UINT32 u4VolShm);
extern void _AUD_DspChannelMute(UINT8 u1DecId, AUD_CH_T eChannel, BOOL fgMute);
extern void _AUD_DspChannelVolGain(UINT8 u1DecId, AUD_CH_T eChannel, INT32 i4Value);
extern void _AUD_DspChannelOutPort(UINT8 u1DecId, AUD_CH_T eChannel, BOOL fgEnable);
extern void _AUD_DspBalance(UINT8 u1DecId, UINT8 u1Balance);
extern void _AUD_DspMasterVolumeFineTune(UINT8 u1DecId, UINT32 u4Volume);
extern void _AUD_DspMasterVolume(UINT8 u1DecId, UINT8 u1Volume);
extern void _AUD_DspMasterVolShmRawValue(UINT8 u1DecId, UINT32 u4VolShm);
extern void _AUD_DspAVCEnable(UINT8 u1DecId, BOOL fgEnable);
extern void _AUD_DspChangeAVCPara(UINT16 u2Type, UINT16 u2Value, UINT8 u1DecId);
extern void _AUD_DspChange2BandAVCPara(UINT8 u1Band, UINT16 u2Type, UINT16 u2Value, UINT8 u1DecId);
extern void _AUD_DspGetAVCPara(UINT16 u2Type, UINT16* u2Value, UINT8 u1DecId);
extern void _AUD_DspInputChangeAVCPara(AUD_DEC_STREAM_FROM_T eStreamFrom);
extern void _AUD_DspMuteEnable(UINT8 u1DecId, BOOL fgEnable);
extern void _AUD_DspStopMuteEnable(UINT8 u1DecId, BOOL fgEnable);
extern void _AUD_DspHdmiModeMuteEnable(UINT8 u1DecId, BOOL fgEnable);
extern void  AUD_DspHdmiChangeFormatMuteEnable(UINT8 u1DecId, BOOL fgEnable);
extern void _AUD_DspBassMngEnalbe(UINT8 u1DecId, BOOL fgEnable);
extern void _AUD_DspProcMode(UINT8 u1DecId, UINT16 u2Mode);
#ifndef CC_NEW_POST_MULTI_VS_SUPPORT
extern void _AUD_DspVirtualSurroundEnalbe(UINT8 u1DecId, BOOL fgEnable);
#else
extern void _AUD_DspVirtualSurroundEnalbe(UINT8 u1DecId, UINT8 u1ModeIdx);
extern void _AUD_DspVSModeChange(UINT8 u1DecId, UINT8 u1ModeIdx);
#endif
#ifdef CC_SET_VOLUME
extern void _AUD_DspSetVolTable(UINT8 u1Idx, UINT32 u4Vol);
#endif
#ifdef CC_AUD_NEW_CV_TABLE
extern void _AUD_DspSetCVTable(UINT8 u1Idx,UINT32 u4Vol);
extern UINT32 _AUD_DspGetCVTable(UINT8 u1VolTabIdx);
#endif
extern void _AUD_DspMonoDownMix(UINT8 u1DecId, BOOL fgEnable);
extern void _AUD_DspSetMatrixOutMode(UINT8 u1DecId, AUD_MATRIX_OUT_T eMode);
extern void _AUD_DspAutoDmx(UINT8 u1DecId, BOOL fgEnable);
extern void _AUD_DspBypassPostProcess(UINT8 u1DecId, BOOL fgBypass);
extern void _AUD_DspSoundEffectFlag(UINT8 u1DecId, BOOL fgFlag);
extern BOOL _AUD_DspIECConfig(AUD_IEC_T eIecCfg, BOOL fgEnable);
extern void _AUD_DspSetIEC(UINT8 u1DecId);
extern BOOL _AUD_DspIecChannel(AUD_IEC_CH_T eIecChannel);
extern BOOL _AUD_DspIecCopyright(UINT8 u1Enable, UINT8 u1CategoryCode);
extern UINT8 _AUD_DspGetIecCopyright(AUD_DEC_STREAM_FROM_T eStreamFrom);
extern UINT8 _AUD_DspGetIecCategoryCode(AUD_DEC_STREAM_FROM_T eStreamFrom);
extern void _AUD_DspMixsoundControl(UINT16 u1Mode);
extern void _AUD_DspSetDemodOutputMode(UINT8 u1Output);
extern void _AUD_DspSetSRSWOW(UINT8 u1Mode);
extern void _AUD_DspSetSRSWOWSpkSz(UINT8 u1Mode);
extern void _AUD_DspSetSRSWOWBright(UINT8 u1Mode);
extern void _AUD_DspSetSRSWOWTBLvl(UINT8 u1Mode);
extern void _AUD_DspSetSRSWOWWidth(UINT8 u1Mode);
extern void _AUD_DspSetSRSTSXTSpkSz(UINT8 u1Mode);
extern void _AUD_DspSetSRSTSXTSetMode(UINT8 u1Mode);
extern void _AUD_DspSetSRSTSXTElev(UINT8 u1Mode);
extern void _AUD_DspSetSRSTSXTinputgain(UINT32 u4Mode);
extern void _AUD_DspSetSRSTSXTTBLvl(UINT8 u1Mode);
extern void _AUD_DspSetDetectDefaultMode(UINT8 u1Mode);
extern void _AUD_DspSetUserDefinedEqCfg(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, UINT8 u1BandIdx, INT8 i1Value);
extern void _AUD_DspSetUserDefinedSuitEqCfg(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, INT8 i1Band1Value, 
                                 INT8 i1Band2Value, INT8 i1Band3Value, INT8 i1Band4Value, 
                                 INT8 i1Band5Value);
extern void _AUD_DspSetUserDefinedSuitEqCfg7Bands(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, INT8 i1Band1Value, 
                                 INT8 i1Band2Value, INT8 i1Band3Value, INT8 i1Band4Value, 
                                 INT8 i1Band5Value, INT8 i1Band6Value, INT8 i1Band7Value);
extern BOOL _AUD_DspGetUserDefineEqCfg(AUD_EQ_TYPE_T eEqType, UINT8 u1BandIdx, INT8* pi1Value);
extern UINT8 _AUD_DspGetUserEqBandNum(void);
extern void _AUD_DspGetEqBandInfo(AUD_EQ_BAND_INFO_T* prEqBandInfo);
extern INT16 _AUD_DspGetSrcVol(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc);
extern void _AUD_DspSetSrcVol(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc, INT16 i2Vol);
extern void _AUD_DspVolumeChange(UINT8 u1DecId, AUD_CH_T eAudChl);
extern void _AUD_DspSetAvSrcVol(AUD_INPUT_ID_T eInputSrcId , INT16 i2Vol);
extern AUD_INPUT_ID_T _AUD_DspGetAvInputSrcId(void);
extern void _AUD_DspSetAvSrcVol(AUD_INPUT_ID_T eInputSrcId , INT16 i2Vol);
extern AUD_INPUT_ID_T _AUD_DspGetAvInputSrcId(void);
extern AUD_INPUT_ID_T _AUD_DspGetAvInputSrcDvdId(void);
extern void _AUD_DspGetAtvTvSys(MW_TV_AUD_SYS_T * prTvAudSys);
extern TV_AUD_SYS_T _AUD_DspGetAtvTvSystem(void);
#ifdef CC_AUD_TV_SYS_FINE_TUNE_VOL
extern void _AUD_SetTVSysFineTuneVol(TV_AUD_SYS_T eAudSys);
#endif
extern void _AUD_DspSetTvSrcMode(TV_AUD_SYS_T eAudSys);
extern TV_AUD_SYS_T _AUD_DspGetTvSrcMode(void);
extern void _AUD_DspSetAtvOutputMode(UINT8 u1DecId, AUD_SOUND_MODE_T eSoundMode);
extern void _AUD_DspHdmiSpdifOutEnable(BOOL fgEnable);
extern UINT32 _AUD_DspGetSpectrumInfo(UINT8 u1DecId, UINT8 u1BandNum);
extern AUD_SOUND_MODE_T _AUD_DspGetSoundMode(UINT8 u1DecId);
extern void _AUD_DspSpectrumEnable(UINT8 u1DecId, BOOL fgEnable);
extern void _AUD_AudioSetStcDiffBound(UINT8 u1DecId, UINT8 uStcDiffLo, UINT8 uStcDiffHi, UINT8 uStcDiffWs);
extern void _AUD_DDBannerEnable(UINT8 u1DecId, BOOL fgEnable);
extern void _AUD_DspSetPalDetection ( AUD_FACTORY_PAL_DETECTION_ITEM_T eChangedItem,UINT16 u2Value);
extern UINT16 _AUD_DspGetPalDetection ( AUD_FACTORY_PAL_DETECTION_ITEM_T eItem) ;
extern void _AUD_DspSetA2Detection ( AUD_FACTORY_A2_DETECTION_ITEM_T eChangedItem,UINT16 u2Value) ;
extern UINT16 _AUD_DspGetA2Detection ( AUD_FACTORY_A2_DETECTION_ITEM_T eItem);
extern void _AUD_DspSetA2Threshold ( AUD_FACTORY_A2_THRESHOLD_ITEM_T eChangedItem,UINT16 u2Value) ;
extern UINT16 _AUD_DspGetA2Threshold ( AUD_FACTORY_A2_THRESHOLD_ITEM_T eItem);
extern void _AUD_DspSetMtsDetection ( AUD_FACTORY_MTS_DETECTION_ITEM_T eChangedItem,UINT16 u2Value);
extern UINT16 _AUD_DspGetMtsDetection ( AUD_FACTORY_MTS_DETECTION_ITEM_T eItem) ;
extern void _AUD_DspSetFmfmDetection ( AUD_FACTORY_FMFM_DETECTION_ITEM_T eChangedItem, UINT16 u2Value);
extern UINT16 _AUD_DspGetFmfmDetection ( AUD_FACTORY_FMFM_DETECTION_ITEM_T eItem);
extern void _AUD_DspSetHdevMode ( AUD_ATV_DECODER_TYPE_T eDecType, BOOL fgEnable);
extern BOOL _AUD_DspGetHdevMode (AUD_ATV_DECODER_TYPE_T eDecType);
extern void _AUD_DspSetAmMute( AUD_FACTORY_PAL_AM_MUTE_ITEM_T eChangedItem,UINT8 u1Value);
extern UINT8 _AUD_DspGetAmMute( AUD_FACTORY_PAL_AM_MUTE_ITEM_T eItem);
extern void _AUD_DspSetFmMute ( AUD_ATV_DECODER_TYPE_T eDecType, AUD_FACTORY_FM_MUTE_ITEM_T eChangedItem, UINT8 u1Value);
extern UINT8 _AUD_DspGetFmMute ( AUD_ATV_DECODER_TYPE_T eDecType,AUD_FACTORY_FM_MUTE_ITEM_T eItem);
extern void _AUD_DspSetCarrierShiftMode ( AUD_ATV_DECODER_TYPE_T eDecType, BOOL fgEnable);
extern BOOL _AUD_DspGetCarrierShiftMode (AUD_ATV_DECODER_TYPE_T eDecType);
extern void _AUD_DspSetNonEUMode (BOOL fgEnable) ;
extern BOOL _AUD_DspGetNonEUMode (void);
extern void _AUD_DspSetFMSatuMuteMode (AUD_ATV_DECODER_TYPE_T eDecType, BOOL fgEnable);
extern BOOL _AUD_DspGetFMSatuMuteMode (AUD_ATV_DECODER_TYPE_T eDecType);
extern void _AUD_DspSetPALFineVolume (UINT8 u1Index);
extern UINT8 _AUD_DspGetPALFineVolume (void);
extern void _AUD_DspSetNicamFineVolume (UINT8 u1Index);
extern UINT8 _AUD_DspGetNicamFineVolume (void) ;
extern void _AUD_DspSetAMFineVolume (UINT8 u1Index);
extern UINT8 _AUD_DspGetAMFineVolume (void);
extern void _AUD_DspSetA2FineVolume (UINT8 u1Index);
extern UINT8 _AUD_DspGetA2FineVolume (void);
extern void _AUD_DspSetMtsMonoFineVolume (UINT8 u1Index);
extern UINT8 _AUD_DspGetMtsMonoFineVolume (void) ;
extern void _AUD_DspSetSAPFineVolume(UINT8 u1Index);
extern UINT8 _AUD_DspGetSapFineVolume (void) ;
extern void _AUD_DspSetFmfmMonoFineVolume (UINT8 u1Index);
extern UINT8 _AUD_DspGetFmfmMonoFineVolume (void) ;
extern void _AUD_DspSetFmfmDualFineVolume(UINT8 u1Index);
extern UINT8 _AUD_DspGetFmfmDualFineVolume (void) ;
extern void _AUD_DspSetMtsPilotDetection (BOOL fgEnable) ;
extern BOOL _AUD_DspGetMtsPilotDetection (void) ;
extern void _AUD_DspSetSapMute( AUD_FACTORY_SAP_MUTE_ITEM_T eChangedItem, UINT8 u1Value); 
extern UINT8 _AUD_DspGetSapMute( AUD_FACTORY_SAP_MUTE_ITEM_T eItem);
extern void _AUD_DspChSetEqTable(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, INT8 eqValue[]);//Andrew Wen 07/8/7 Add EQ band gain CLI commands
extern void _AUD_DspChQryEqTable(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, INT8 eqValue[]);
extern void _AUD_DspSetFMSatuMuteTh( AUD_FACTORY_SATU_MUTE_ITEM_T eChangedItem, 
                                                                                        UINT8 u1Value) ;
extern UINT8 _AUD_DspGetFMSatuMuteTh( AUD_FACTORY_SATU_MUTE_ITEM_T eItem) ;

extern void _AUD_DspSetVolumeTable(UINT32* u4VolTabChkPnt);
extern UINT32 _AUD_DspGetVolumeTable(UINT8 u1VolTabIdx);
extern UINT32 _AUD_DspSpkSizeQry(UINT8 u1DecId);

extern void _AUD_DspMasterVolShmRawValue(UINT8 u1DecId, UINT32 u4VolShm);
extern void _AUD_DspSetPCMChannelNumber(UINT8 u1DecId, UINT16 u2ChannelNo);

extern UINT8 _AUD_DspGetVolumeBaseLevel(void);
extern void _AUD_DspProcMode(UINT8 u1DecId, UINT16 u2Mode);
extern void _AUD_DspADModeSet(UINT8 u1DecId, UINT16 u2Value);
extern void _AUD_DspADEnable(UINT8 u1DecId, BOOL fgEnable);

extern void _AUD_Surround_Get_Config(AUD_VSURR_CFG_T* prVsurrCfg);
extern void _AUD_Surround_Set_Config(AUD_VSURR_CFG_TYPE_T eVSCfgType, UINT32 u4VSParameter);
#endif //_AUD_DSP_CFG_H_

extern void _AUD_DspSetPcmPreBufferLength(UINT8 u1DecId, UINT16 u2Length);
extern UINT16 _AUD_DspGetPcmPreBufferLength(UINT8 u1DecId);
extern BOOL _AUD_GetNicamStatus(void);
#ifdef SUPPORT_PHILIPS_LOUDNESS
extern AUD_TYPE_T _AUD_GetAudInfo(UINT8 u1DecId) ;
#endif
#ifdef CC_AUD_LINEOUT_VOL_MODE
extern void _AUD_SetLineOutVolumeMode(UINT8 u1DecId, UINT8 fgMode);
extern BOOL AUD_GetLineOutVolumeMode(UINT8 u1DecId);
extern UINT8 _AUD_DspGetLineOutFixVol(void);
#endif
#ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
extern void _AUD_DspSrcLinoutVol(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc, INT16 i2Vol);
#endif
extern void _AUD_GetDtvAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo) ;
extern void _AUD_DspSpeed(UINT8 u1DecId, UINT16 u1Speed);
extern void _AUD_DspDownmixPosition(UINT8 u1DecId, UINT16 u1DMPosition);
extern void _AUD_DspMultiPairOutput(UINT8 u1DecId, UINT16 u1MultiPair);
extern void _AUD_DspLRDownmix(UINT8 u1DecId, BOOL fgEnable);



//#ifdef DATA_DISC_WMA_SUPPORT
extern void _AUD_DspWMAHeader(UINT8 u1DecId,AUD_WMA_CFG_T* prWmaInfo);
extern void AUD_DrvSetWMAErrFunc(UINT8 u1DecId, AUD_WMAErr_NOTIFY pfClipNotify);
//#endif
extern void _AUD_DspSetDualDecMode(BOOL fgEnable);

extern void _AUD_DspSetHPFEnable(BOOL fgEnable);
extern void _AUD_DspSetHPFFc(UINT16 u2Fc);
extern void _AUD_DspSetClipperEnable(BOOL fgEnable);
extern void _AUD_DspSetClipperValue(AUD_CH_T eChannel, UINT32 u4Value);
#ifdef CC_AUD_SPEAKER_HEIGHT
#ifndef CC_AUD_SPEAKER_HEIGHT_MODE
extern void _AUD_DspSetSPHEnable(BOOL fgEnable);
#else
extern void _AUD_DspSetSPHMode(UINT8 u1Mode);
#endif
#endif
extern void _AUD_DspSetPEQEnable(BOOL fgEnable);
extern void _AUD_DspSetPEQCfg(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 u4Frequency,
                            UINT16 u2BW, INT16 i2Gain);
extern void _AUD_DspSetPEQCfg2(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 u4Frequency,
                            AUD_PEQ_Q_TYPE_T eQValue, INT16 i2Gain);
extern void _AUD_DspSetVirBassEnable(BOOL fgEnable);
extern void _AUD_DspSetVirBassCfg(UINT8 u1DecId, UINT32 u4Frequency, UINT32 u4Gain, 
                        UINT32 u4Gain2);
extern void _AUD_DspSetDDCOEnable(BOOL fgEnable);
extern void _AUD_DspSetDDCOAgcEnable(BOOL fgEnable);
extern void _AUD_DspSetDDCOLFEEnable(BOOL fgEnable);
extern void _AUD_DspSetAoutReinit(void);
#ifdef CC_ENABLE_AV_SYNC
extern void AUD_DspDecPlayMuteEnable(UINT8 u1DecId, BOOL fgEnable);
#endif


extern UINT32 _AUD_NVM_Get_Profile(UINT32* pAudProfileStart, UINT32* pAudProfileEnd, UINT8* u1Profile);
extern UINT32 _AUD_NVM_Read(void);

extern void _AUD_DspGetMpegAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo);
extern void _AUD_DspSetMp3PreparsingMode(BOOL fgEnable);
extern BOOL _AUD_DspIsMp3Vbr(void);
#ifdef CC_AUD_USE_FLASH_AQ
extern BOOL Aud_FlashAQQuery(AUD_AQ_TYPE_T eAQDataType, VOID* *pData);
extern void Aud_FlashAQParsing(AUD_AQ_TYPE_T eAQDataType, UINT8* pData);
extern void Aud_FlashAQSetValue(void);
extern void _AUD_FlashAQPeriphSMChange(AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 u1ModeCase);
extern void _AUD_FlashAQPeriph_EXTPEQ(AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 u1ModeCase, SAMPLE_FREQ_T eFS);
#endif

