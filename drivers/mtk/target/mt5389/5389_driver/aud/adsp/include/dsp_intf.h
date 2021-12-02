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
 * Copyright (c) 2006, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: kai.wang $
 * $Date: 2012/09/21 $
 * $RCSfile: dsp_intf.h,v $
 * $Revision: #3 $
 *
 *---------------------------------------------------------------------------*/

/** @file dsp_intf.h
 *  Brief of file dsp_intf.h.
 * This files contains
 * 1. The interface between Dsp module and other RISC module
 * 2. The interface between Emulation program and Dsp Module
 */

#ifndef DSP_INTF_H
#define DSP_INTF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"
#include "aud_if.h"
#include "aud_drvif.h"

LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

#define AUD_STATISTICS

#define _fgDecTblFromFlash         TRUE
#define _fgDec2TblFromFlash        TRUE
#define _fgDec3TblFromFlash        TRUE
#ifdef CC_AUD_4_DECODER_SUPPORT
#define _fgDec4TblFromFlash        TRUE
#endif
#define _fgDecFromFlash            TRUE
#define _fgDec2FromFlash           TRUE
#define _fgDec3FromFlash           TRUE
#define _fgCommRAMFromFlash        TRUE
#define _fgPostRamFromFlash        TRUE

// New Dolby Tech. license SW control
#ifdef CC_AUD_NEW_DOLBY_SUPPORT_AC3
#define IS_SW_SupportAC3()          TRUE
#else
#define IS_SW_SupportAC3()          FALSE
#endif
#ifdef CC_AUD_NEW_DOLBY_SUPPORT_EAC3
#define IS_SW_SupportEAC3()         TRUE
#else
#define IS_SW_SupportEAC3()         FALSE
#endif
#ifdef CC_AUD_NEW_DOLBY_SUPPORT_DDCO
#define IS_SW_SupportDDCO()         TRUE 
#else
#define IS_SW_SupportDDCO()         FALSE 
#endif
#ifdef CC_AUD_NEW_DOLBY_SUPPORT_MS10
#define IS_SW_SupportMS10()         TRUE 
#else
#define IS_SW_SupportMS10()         FALSE 
#endif


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

// DSP memory space define
#define DSP_BUF_BLOCK (0x1000000)     // 16MB

// Timeout define
#define DSP_TIMEOUT (100000)

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
extern BOOL _IsDualDecMode(void);
extern void vDspSendSample(UINT8 u1DecId);
extern void vDspFlushDone(UINT8 u1DecId);
extern void vDspAdacFmt (UINT8 u1DecId, UINT32 u4ClkFmt);
extern void vFlash2DramDma (UINT32 u4Dest, UINT32 u4Src, UINT32 u4Len);
extern void vFlash2DramDmaShift(UINT32 u4Dest, UINT32 u4Src , UINT32 u4Len);
extern UINT32 u4GetDspImgAddr (void);
extern UINT32 u4GetFlashData (UINT32 u4Addr);

extern void vDspMemInit (UINT8 * puWorkingBuffer);
extern UINT32 u4GetDspFlashVersion (void);

extern UINT32 u4GetMixSoundStartByteAddr(void);
extern UINT32 u4GetMixSoundBufSize(void);
extern UINT32 u4GetMixSoundStartByteAddr2(void);
extern UINT32 u4GetMixSoundBufSize2(void);
extern UINT32 u4GetMixSoundStartByteAddr3(void); //ALSA_MIXSND_PATH
extern UINT32 u4GetMixSoundBufSize3(void); //ALSA_MIXSND_PATH
extern UINT16 u2GetMixSoundStatus(void);

extern void AUD_UopCommandDone(UCHAR ucDecId, UINT32 u4Command);
extern void vDspFlowControlNotify(UINT32 u4DspRIntData);
extern BOOL fgDspRealPlay(UINT8 u1DecId);
extern BOOL fgGetDspCodeRegion(UINT32* pu4StartAddr, UINT32* pu4EndAddr);

extern BOOL fgGetDspCmd(UINT32 pu4Cmd);
extern void vDspTvSysDetectedNotify(UINT8 u1DecId, TV_AUD_SYS_T eTvSys);
extern void vDspFMRadioDetectionNotify(AUD_FM_RADIO_DET_T u4Msg);
extern void vDspHdevModeChangeNotify(UINT32 u4Msg);
extern void vDspTvSysChangeNotify(TV_AUD_SYS_T eTvSys);
extern void AUD_GetStreamInfo(UINT8 u1DecId, UINT8 *pu1Acmode, UINT8 *pu1SmpRate, UINT32 * pu4DataRate);
extern void AUD_SetSampleFreq(UINT8 u1DecId, SAMPLE_FREQ_T eSmpFreq);
extern void AUD_DrvMMAoutEnableNotify(UINT8 u1DecId);
/* log for interrupt handler*/
extern void vLogFlowControl(UINT32 u4DspRIntData);

extern UINT8 u1GetAacChNum(UINT8 u1DecId);
extern UINT8 u1GetAacVersion(UINT8 u1DecId);
extern UINT8 u1GetAacTSFormat(UINT8 u1DecId);

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

extern void DSP_ShareInforDump(UINT8 u1Group);
extern void DSP_ShareInforWriteWord(UINT32 ADDR, UINT16 VALUE);
extern void Dsp_WriteDspCommDram(UINT32 u4SrcAddr, UINT32 u4Value);
extern void DSP_CommDramDump(UINT32 u4Address, UINT32 u4Len);
extern void DSP_NormDramDump(UINT8 u1Page, UINT32 u4Address, UINT32 u4Len);
extern void DSP_CmptDramDump(UINT8 u1Page, UINT32 u4Address, UINT32 u4Len);

extern UINT32 DSP_CommDramRead(UINT32 u4Address);
#ifdef CC_AUD_FIR_SUPPORT
extern UINT32 DSP_CommDramAddr(UINT32 u4Address);  //CC_AUD_FIR_SUPPORT
#endif
extern UINT32 DSP_DecDramRead(UINT32 u4Address);
extern UINT32 DSP_Dec2DramRead(UINT32 u4Address);

extern UINT32 DSP_CommDramRead(UINT32 u4Address);
extern void DSP_GetAFIFOVirtualAddr(UINT8 u1DecId, UINT32 * pu4AddrStart, UINT32 * pu4AddrEnd);
extern void DSP_GetAFIFOPhysicalAddr(UINT8 u1DecId, UINT32 * pu4AddrStart, UINT32 * pu4AddrEnd);
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT 
extern void DSP_GetHDMIParserFIFOVirtualAddr(UINT32 * pu4AddrStart, UINT32 * pu4AddrEnd);
extern void DSP_GetHDMIParserFIFOPhysicalAddr(UINT32 * pu4AddrStart, UINT32 * pu4AddrEnd);
#endif
extern UINT32 DSP_GetDspReadPtrPhysicalAddr(UINT8 u1DecId);

extern UINT8 DSP_GetAcmod(UINT8 u1DecId);
extern UINT8 DSP_GetAc3DecType(UINT8 u1DecId);
extern void DSP_DrvResume(void);

extern UINT32 DSP_InternalLogicalAddr(UINT32 u4Addr);
extern UINT32 DSP_PhysicalAddr(UINT32 u4Addr);
extern void DSP_SetStartPtsToShm(UINT8 u1DecId, UINT32 u4Pts, UINT32 u4Addr);
extern void DSP_SetAudLogPeriod(UINT32 u4Period);
extern void DSP_SetStcDiff(UCHAR ucDecId, UINT16 u2StcWorst);
extern INT32 DSP_SendPts(UINT8 u1DecId, UINT32 u4Pts, UINT32 u4ParPnt);
extern void DSP_ResetDec(UINT8 u1DecId);
extern void DSP_LoadRamCode(UINT32 u4Type);
extern UINT32 DSP_VirtualAddr(UINT32 u4Addr);
extern BOOL DSP_RealStop(UINT8 u1DecId);
extern void DSP_SetAIFMux(UINT8 uAIFPath);
extern void DSP_GetAtvTvSys(MW_TV_AUD_SYS_T * prTvAudSys);
extern void vDspSetFMRadioNoiseThreshold(UINT32 u4Thre);
extern void vDspSetFMRadioInBandThreshold(UINT32 u4Thre);
extern void vDspSetFMRadioRatioThreshold(INT32 i4Thre);
extern UINT32 vDspGetFMRadioNoiseThreshold(void);
extern UINT32 vDspGetFMRadioInBandThreshold(void);
extern INT32 vDspGetFMRadioRatioThreshold(void);

//#ifdef DSP_SUPPORT_NPTV
 //MTS API
extern void DSP_SetDemodOutputMode (UINT8 u1Output);
extern void DSP_SetPALOutput (UINT8 u1UserMode, UINT8 u1EnforcedMode);
extern void vDspDemodDetModeNotify(UINT32 u4IntGroup , UINT32 u4IntVector);
extern void DSP_SetPALFineVolume (UINT8 u1Value);
extern void DSP_SetNICAMFineVolume (UINT8 u1Value);
extern void DSP_SetAMFineVolume (UINT8 u1Value);
extern void DSP_SetA2FineVolume (UINT8 u1Value);
extern void DSP_SetMTSFineVolume (UINT8 u1Value);
extern void DSP_SetSAPFineVolume (UINT8 u1Value);
extern void DSP_SetFmfmMonoFineVolume (UINT8 u1Value);
extern void DSP_SetFmfmDualFineVolume (UINT8 u1Value);
extern void DSP_SetFmRadioFineVolume (UINT8 u1Value);
extern void DSP_AudioSetChannelSrc (TV_AUD_SYS_T eTvSys);
extern TV_AUD_SYS_T DSP_ReturnChannelSource(void);
extern void DSP_SetTvSrcMode (TV_AUD_SYS_T eAudSys);
extern void DSP_SetAtvOutputMode(UINT8 u1DecId, AUD_SOUND_MODE_T eSoundMode);
extern UINT32 DSP_GetEQSpectrumAddr(UINT8 u1DecId);
extern void DSP_SetStcDiffBound(UINT8 uStcDiffLo, UINT8 uStcDiffHi, UINT8 uStcDiffWs);
extern UINT8 u4GetStcDiffFactor(AUD_STC_DIFF_SET eStcDiffType);
extern void DSP_SetDualDecMode(BOOL fgEnable);
#ifdef CC_AUD_SKYPE_SUPPORT
extern void DSP_SetAuxMixMainMode(BOOL fgEnable);
#endif
#ifdef CC_AUD_PCM_LINE_IN_LPF
extern void DSP_SetPCMLineInLPF(BOOL fgEnable);
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
extern void DSP_Set4thDecMode(BOOL fgEnable);
#endif
#ifdef CC_MT5391_AUD_3_DECODER
extern void DSP_SetTriDecMode(BOOL fgEnable);
#endif
#if (defined(CC_AUD_SUPPORT_MS10) && defined(CC_MT5391_AUD_3_DECODER))
extern void DSP_SetAdFreeRun(BOOL fgEnable);
#endif
extern void DSP_SetAsrcTrackingMode(BOOL fgEnable);
extern void DSP_DualDecAtvNotifyChlInfor(void);
extern TV_AUD_SYS_T DSP_GetAtvTvSystem(void);
extern AUD_TYPE_T DSP_GetAudChlInfo(UINT8 u1DecId);
extern void DSP_SetEuroCanalPlusMode(BOOL fgEnable);
extern BOOL DSP_IsEuroCanalPlusMode(void);
extern void DSP_SetDetTvSrcMode(TV_AUD_SYS_T eAudSys);
extern TV_AUD_SYS_T DSP_GetDetTvSrcMode(TV_AUD_SYS_T eAudSys);
extern TV_AUD_SYS_T DSP_GetTvSrcMode (void);
extern void DSP_ClearSoundMode(UINT8 u1DecId);
extern AUD_SOUND_MODE_T DSP_GetSoundMode(UINT8 u1DecId);
extern AUD_TYPE_T DSP_GetMtsSrcTyp(UINT8 u1DecId);
extern void DSP_LogAtvTvSys(void);
extern UINT32 DSP_GetMpegFrameSize(UINT8 u1DecId);

//#endif
#ifdef FRAC_SPEED_SUPPORT
//sunman for play speed
extern void DSP_SpeedSetMode(UINT32 u4Mode);
#endif

#ifdef CC_AUD_SILENCE_SUPPORT
extern void DSP_SilenceSetMode(BOOL fgEnable);
extern void DSP_SilenceSetThreshold(UINT32 u1Threshold);
extern void DSP_SilenceSetWait(UINT32 u1Wait);
extern void DSP_SilenceSetFixGain(UINT32 u4AttackFixGain);
extern void DSP_SilenceSetReleaseStep(UINT32 u4ReleaseStep);
extern void DSP_SilenceSetAttackStep(UINT32 u4AttackStep);
extern void DSP_QrySilenceMode(UINT32 *u4Enable);
extern void DSP_QrySilenceSetThreshold(UINT32 *u4Threshold);
extern void DSP_QrySilencePeakValue(UINT32 *u4PeakValue);
extern void DSP_QrySilenceWait(UINT32 *u4Wait);
extern void DSP_QrySilenceAttackFixGain(UINT32 *u4AttackFixGain);
extern void DSP_QrySilenceReleaseStep(UINT32 *u4ReleaseStep);
extern void DSP_QrySilenceAttackStep(UINT32 *u4AttackStep);
#endif


#ifdef DSP_SUPPORT_SRSTSXT
extern void DSP_SRSTSXTSetSpkSize(UINT8 u1Mode);
extern void DSP_SRSTSXTSetMode(UINT8 u1Mode);
extern void DSP_SRSTSXTTBLvl(UINT8 u1Mode);
extern void DSP_SRSTSXTElev(UINT8 u1Mode);
extern void DSP_SRSTSXTinputgain(UINT32 u4Mode);
#endif

/* command queue related functions */
extern void DSP_SendDspTaskCmd(UINT32 u4Cmd);

#ifdef CC_AUD_BBE_SUPPORT
extern void DSP_SetBbeLevel(UINT8 u1Index);
extern void DSP_SetBbeProcess(UINT8 u1Index);
extern void DSP_SetBbeLoContour(UINT8 u1Index);
extern void DSP_SetBbe3dGain(UINT8 u1Index);
#endif

#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
extern void DSP_SetCDNotchQandFc(UINT32 u4UserQ, UINT32 u4UserFc);
#endif
#ifdef KARAOKE_SUPPORT
extern void DSP_SetKeyshiftFlag(BOOL fgflag);
extern void DSP_SetKeyshiftKey(INT8 u1key);
#endif
extern void DSP_SetPcmLrckMode(UINT8 u1DecId, BOOL fgEable);
extern void DSP_SetLineInLrInverse(UINT8 u1DecId, BOOL fgEable);
extern void DSP_SetPcmType(UINT8 u1DecId, PCM_INPUT_TYPE_T eType);
extern void DSP_SetPcmFs(UINT8 u1DecId, SAMPLE_FREQ_T eFs);
extern void DSP_SetAc3Endian (UINT8 u1DecId, DATA_ENDIAN_T eEndian);
extern UINT32 DSP_GetDataRate(UINT8 u1DecId);
extern UINT8 DSP_GetSampleRate(UINT8 u1DecId);
extern void DSP_Get_VSurr_Cfg(AUD_VSURR_CFG_T* prVsurrCfg);
extern void DSP_Set_VSurr_Cfg(AUD_VSURR_CFG_TYPE_T eVSCfgType, UINT32 u4VSParameter);
extern void DSP_GetDtvAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo);
extern void vDspAOutEnable(UINT8 u1DecId);
extern UINT32 u4ReadD2RInfo_AputBank(UINT8 u1DecId);

#ifdef __MODEL_slt__
void DSP_ChangeSourceMode(UINT8 u1Mode);
#endif

//#ifdef	DATA_DISC_WMA_SUPPORT
extern UINT32 DSP_GetWmaErrorCode(void);
extern UINT32 DSP_GetWmaFrameSize(void);
//#endif

#if 1///def CC_VORBIS_SUPPORT    ///CC_VORBIS_SUPPORT_ERR
extern UINT32 DSP_GetVorbisErrorCode(void);
#endif

extern BOOL _GetMpegAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo);
extern BOOL _GetMpegAudInfoWithLayer(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo, UINT8 *prMPEGLayer);

extern void DSP_SetMp3PreparsingMode(BOOL fgEnable);
extern BOOL DSP_IsMp3Vbr(void);
extern UINT8 DSP_GetEffectChannelNum(void);
extern void DSP_FlushInvalidateDCache(UINT8 u1Type, UINT8 u1Block);
extern void DSP_FlushInvalidateDCacheFree(UINT32 u4Addr, UINT32 u4Len);
extern void DSP_FlushInvalidateDCacheSmall(UINT8 u1Type, UINT8 u1Block, UINT32 u4Offset, UINT32 u4Len);
extern void DSP_FlushInvalidateDCacheSmall2(UINT32 u4CommonIdx) ;
extern void DSP_InvalidateDCache(UINT32 u4Addr, UINT32 u4Len);

#define DSP_INTERNAL_ADDR(u4Addr)	DSP_InternalLogicalAddr(u4Addr)
#define DSP_PHYSICAL_ADDR(u4Addr)	DSP_PhysicalAddr(u4Addr)
#define DSP_VIRTUAL_ADDR(u4Addr)	DSP_VirtualAddr(u4Addr)

///#ifdef MP3ENC_SUPPORT
extern UINT32 DSP_CommonDramRead(UINT16 u2Addr);
#define DSP_COMMON_DRAM_READ(u2Addr) DSP_CommonDramRead(u2Addr)
///#endif

//static AUD_WMAErr_NOTIFY _hWMAErrNotifyFunc = NULL;

#if 1///def CC_VORBIS_SUPPORT ///def DSP_VORBIS_SUPPORT
extern UINT8 _uVorbisMkTbl[AUD_DEC_MAX];
extern UINT8 _uVorbisTblDone[AUD_DEC_MAX];
extern UINT8 _uVorbisIntDec;
extern BOOL fgMakeVorbisCodebook(void);
extern BOOL fgIsDspVorbisTblDone (UINT8 u1DecId);
extern void DspVorbisCodeBookResult(BOOL fgStatus);
#endif

extern void DSP_SetCapability(void);
extern UINT32 DSP_GetCapability(UINT8 u1Idx);
extern BOOL DSP_IsFormatValid(UINT32 u4FormatInfo);
extern UINT32 DSP_GetChannelDelay(UINT8 u1ChIdx);  //QUERY_DELAY
extern void DSP_SetPtsStcCtrlFlag(UINT32 u4Flag);
extern UINT16 DSP_GetPtsStcCtrlFlag(void);
extern void DSP_SetASRCNumeratorQuarter(BOOL fgEnable);
extern BOOL DSP_IsSupportAC3(void);
extern BOOL DSP_IsSupportEAC3(void);
extern BOOL DSP_IsSupportDDCO(void);
extern void DSP_SetMixsndVolume(UINT32 u4Volume);
#endif                          // DSP_INTF_H
