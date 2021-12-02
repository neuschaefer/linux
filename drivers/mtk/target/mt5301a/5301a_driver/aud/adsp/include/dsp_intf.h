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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: dsp_intf.h,v $
 * $Revision: #1 $
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
//#include "dsp_common.h"
//#include "dsp_general.h"
//#include "d2rc_shm.h"
//#include "dsp_table.h"
#include "aud_if.h"
#include "aud_drvif.h"
/*
// Customer Define
#include "dsp_general.h"
#include "dsp_config.h"
#include "dsp_common.h"
#include "dsp_custom.h"
#include "drv_adsp.h"
#include "dsp_var_def.h"
#include "dsp_rg_ctl.h"
#include "ctrl_para.h"
#include "dsp_table.h"
#include "dsp_data.h"
#include "dsp_reg.h"
#include "dsp_func.h"

#ifndef EMULATION_FPGA

#include "dsp_macro.h"
#include "dsp_data.h"
#include "d2rc_int.h"
#include "dsp_msg_map.h"
#include "dsp_shm.h"
#include "dsp_uop.h"
#include "aud_debug.h"
#include "aud_if.h"
#include "aud_drvif.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_timer.h"
#else

#endif

*/

LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

#define AUD_STATISTICS

#define  _fgDecTblFromFlash         TRUE
#define  _fgDec2TblFromFlash        TRUE
#define  _fgMixTblFromFlash         TRUE
#define  _fgDecFromFlash            TRUE
#define  _fgDec2FromFlash           TRUE
#define  _fgMixFromFlash            TRUE
#define  _fgCommRAMFromFlash        TRUE
#define  _fgPostRamFromFlash        TRUE

#if (0)
#ifdef CC_MT5360B_AUD_FIX_PROM
#define  _fgRamBoot                 FALSE
#else
#ifdef CC_MT5391_AUD_SUPPORT
  #ifdef CC_MT5392B
     #define  _fgRamBoot                 FALSE
  #else
     #define  _fgRamBoot                 TRUE
  #endif	 
#else
    #ifdef CC_MT5360B_DSP_ROM_BOOT
#define  _fgRamBoot                 FALSE    
    #else
#define  _fgRamBoot                 TRUE
    #endif
#endif
#endif
#endif
//#define  SIF_ADC   //SIF ADC dump dram data 


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

// DSP memory space define
#define DSP_BUF_BOUNDARY                 (0x10000000)           // 256MB
#define DSP_BUF_BLOCK                       (0x1000000)           // 16MB

// General define
#define FIRST_DECODER  						(0x0)
#define SECOND_DECODER 						(0x1)
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
#define THIRD_DECODER 						(0x2)
#endif

// Timeout define
#define DSP_TIMEOUT    							(100000)

// DSP working buffer size
#if 0 //#ifdef __MODEL_slt__
#define DSP_WORK_BUF_SIZE               (0x2e0000)
#else
 #ifndef SIF_ADC
#define DSP_WORK_BUF_SIZE               (1*1024*1024) 
 #else
#define DSP_WORK_BUF_SIZE               (4*1024*1024)  
 #endif
#endif

// For EMULATION_FPGA
#define  vUpdateShmVol()
#define  UpdateSpectrum()
#define  UpdatePTSInfo()
#define  SLEEP(arg1)

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
extern BOOL _IsDualDecMode(void);
extern void vDspSendSample(BOOL fgWaitInitSync);
extern void vDspFlushDone (void);
extern void vDspSendSampleDec2(BOOL fgWaitInitSync);
extern void vDspAOutEnableDec2 (void);
extern void vDspFlushDoneDec2 (void);
extern void vDspAdacFmt (UINT32 u4ClkFmt, BOOL fgFirstDecoder);
extern void vSetAdac (UINT32 u4Cmd, UINT32 u4Arg1, UINT32 u4Arg2);
extern void vDspSetDeEmphDec (UINT8 uStrTyp);
extern void vDspSetDeEmphDec2 (UINT8 uStrTyp);

#ifdef CC_MT5391_AUD_3_DECODER
extern void vDspSendSampleDec3(void);
extern void vDspAOutEnableDec3(void);
extern void vDspFlushDoneDec3(void);
#endif

extern void vFlash2DramDma (UINT32 u4Dest, UINT32 u4Src, UINT32 u4Len);
extern UINT32 u4GetDspImgAddr (void);
extern UINT32 u4GetFlashData (UINT32 u4Addr);

#define ReadIntfDram(addr,data) ((data) = (*((UINT32 *)(addr))))
extern void vDspMemInit (UINT8 * puWorkingBuffer);
extern void vShowError (UINT32 u4Msg);
extern UINT32 u4GetDspFlashVersion (void);

extern UINT32 u4GetMixSoundStartByteAddr(void);
extern UINT32 u4GetMixSoundBufSize(void);
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
extern void  AUD_SetSampleFreq(UINT8 u1DecId, SAMPLE_FREQ_T eSmpFreq);
extern void AUD_DrvMMAoutEnableNotify(UINT8 u1DecId);
/* log for interrupt handler*/
extern void vLogFlowControl(UINT32 u4DspRIntData);

extern UINT8 u1GetAacChNum(UINT8 u1DecId);
extern UINT8 u1GetAacVersion(UINT8 u1DecId);
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

extern void 		 DSP_ShareInforDump(UINT8 u1Group);
extern void 		 DSP_ShareInforWriteWord(UINT32 ADDR, UINT16 VALUE);
extern void 		 DSP_CommDramDump(UINT32 u4Address, UINT32 u4Len);
extern UINT32        DSP_CommDramRead(UINT32 u4Address);
extern UINT32        DSP_DecDramRead(UINT32 u4Address);
extern UINT32        DSP_Dec2DramRead(UINT32 u4Address);

extern UINT32        DSP_CommDramRead(UINT32 u4Address);
extern void 		 DSP_GetAFIFOVirtualAddr(UINT8 uDecIndex, UINT32 * pu4AddrStart, UINT32 * pu4AddrEnd);
extern void 		 DSP_GetAFIFOPhysicalAddr(UINT8 uDecIndex, UINT32 * pu4AddrStart, UINT32 * pu4AddrEnd);

extern UINT32 DSP_GetDspReadPtrPhysicalAddr(UINT8 u1DecId);

extern UINT8 	 DSP_GetAcmod(UINT8 u1DecId);

extern UINT32 DSP_InternalLogicalAddr(UINT32 u4Addr);
extern UINT32 DSP_PhysicalAddr(UINT32 u4Addr);
extern void 		 DSP_SetStartPtsToShm(UINT8 u1DecId, UINT32 u4Pts, UINT32 u4Addr);
extern void 		 DSP_SetAudLogPeriod(UINT32 u4Period);
extern void 		 DSP_SetStcDiff(UCHAR ucDecId, UINT16 u2StcWorst);
extern INT32   DSP_SendPts(UINT8 u1DecId, UINT32 u4Pts, UINT32 u4ParPnt);
extern void 		 DSP_ResetDec(UINT8 u1DecId);
extern void      DSP_LoadRamCode(UINT32 u4Type);
extern UINT32 DSP_VirtualAddr(UINT32 u4Addr);
extern BOOL   DSP_RealStop(UINT8 u1DecId);
extern void 		DSP_SetAIFMux(UINT8 uAIFPath);
extern void     DSP_GetAtvTvSys(MW_TV_AUD_SYS_T * prTvAudSys);

//#ifdef DSP_SUPPORT_NPTV
 //MTS API
extern void 		DSP_SetDemodOutputMode (UINT8 u1Output);
extern void 		DSP_SetPALOutput (UINT8 u1UserMode, UINT8 u1EnforcedMode);
extern void 		vDspDemodDetModeNotify(UINT32 u4IntGroup , UINT32 u4IntVector);
extern void     DSP_SetPALFineVolume (UINT8 u1Value); 
extern void     DSP_SetNICAMFineVolume (UINT8 u1Value); 
extern void     DSP_SetAMFineVolume (UINT8 u1Value); 
extern void     DSP_SetA2FineVolume (UINT8 u1Value); 
extern void     DSP_SetMTSFineVolume (UINT8 u1Value); 
extern void     DSP_SetSAPFineVolume (UINT8 u1Value); 
extern void     DSP_SetFmfmMonoFineVolume (UINT8 u1Value); 
extern void     DSP_SetFmfmDualFineVolume (UINT8 u1Value); 
extern void     DSP_AudioSetChannelSrc (TV_AUD_SYS_T eTvSys); 
extern TV_AUD_SYS_T    DSP_ReturnChannelSource(void);
extern void DSP_SetTvSrcMode (TV_AUD_SYS_T eAudSys);
extern void DSP_SetAtvOutputMode(UINT8 u1DecId, AUD_SOUND_MODE_T eSoundMode);
extern UINT32 DSP_GetEQSpectrumAddr(UINT8 u1DecId);
extern void DSP_SetStcDiffBound(UINT8 uStcDiffLo, UINT8 uStcDiffHi, UINT8 uStcDiffWs);
extern UINT8 u4GetStcDiffFactor(AUD_STC_DIFF_SET eStcDiffType);
extern void DSP_SetDualDecMode(BOOL fgEnable);
extern void DSP_SetTriDecMode(BOOL fgEnable);
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


#if (1) //def DSP_SUPPORT_SRSTSXT
extern void DSP_SRSTSXTSetSpkSize(UINT8 u1Mode);
extern void DSP_SRSTSXTSetMode(UINT8 u1Mode);
extern void DSP_SRSTSXTTBLvl(UINT8 u1Mode);
extern void DSP_SRSTSXTElev(UINT8 u1Mode);
extern void DSP_SRSTSXTinputgain(UINT32 u4Mode);
#endif

/* command queue related functions */
extern void 		DSP_SendDspTaskCmd(UINT32 u4Cmd);

#if (1) //def DSP_SUPPORT_SRSWOW
extern void DSP_SRSWOWSetMode(UINT8 u1Mode);
extern void DSP_SRSWOWSetSpkSize(UINT8 u1Mode);
extern void DSP_SRSWOWBright(UINT8 u1Mode);
extern void DSP_SRSWOWTBLvl(UINT8 u1Mode);
extern void DSP_SRSWOWWidth(UINT8 u1Mode);
#endif

extern void DSP_SetBbeLevel(UINT8 u1Index);
extern void DSP_SetBbeProcess(UINT8 u1Index);
extern void DSP_SetBbeLoContour(UINT8 u1Index);
extern void DSP_SetBbe3dGain(UINT8 u1Index);
extern void DSP_SetPcmLrckMode(UINT8 u1DecId, BOOL fgEable);
extern void DSP_SetLineInLrInverse(UINT8 u1DecId, BOOL fgEable);
extern void DSP_SetPcmType(UINT8 u1DecId, PCM_INPUT_TYPE_T eType);
extern void DSP_SetPcmFs(UINT8 u1DecId, SAMPLE_FREQ_T eFs);
extern UINT32 DSP_GetDataRate(UINT8 u1DecId);
extern UINT8 DSP_GetSampleRate(UINT8 u1DecId);
extern void DSP_Get_VSurr_Cfg(AUD_VSURR_CFG_T* prVsurrCfg);
extern void DSP_Set_VSurr_Cfg(AUD_VSURR_CFG_TYPE_T eVSCfgType, UINT32 u4VSParameter);
extern void DSP_GetDtvAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo); 
extern void vDspAOutEnable(void);
extern UINT32 u4ReadD2RInfo_AputBank(UINT8 u1DecId);

#ifdef __MODEL_slt__
void DSP_ChangeSourceMode(UINT8 u1Mode);
#endif

//#ifdef	DATA_DISC_WMA_SUPPORT
extern UINT32 DSP_GetWmaErrorCode(void);
extern UINT32 DSP_GetWmaFrameSize(void);
//#endif

extern BOOL _GetMpegAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo);
extern void DSP_SetMp3PreparsingMode(BOOL fgEnable);
extern BOOL DSP_IsMp3Vbr(void);

#define DSP_INTERNAL_ADDR(u4Addr)	DSP_InternalLogicalAddr(u4Addr)
#define DSP_PHYSICAL_ADDR(u4Addr)	DSP_PhysicalAddr(u4Addr)
#define DSP_VIRTUAL_ADDR(u4Addr)		DSP_VirtualAddr(u4Addr)


#ifdef __MODEL_slt__
extern BOOL DSP_AtvFifoFault(void);
#endif

//static AUD_WMAErr_NOTIFY _hWMAErrNotifyFunc = NULL; 

 //EMULATION_FPGA
#if 0

#ifdef DSP_SUPPORT_NPTV
#define vDspDemodDetModeNotify(a)
#endif
#define AUD_UopCommandDone(a,b)
#define vDspSetDeEmphDec(a)
#define vDspSetDeEmphDec2(a)
#define x_thread_delay(a)
#define vLogFlowControl(a)
#define vDspFlowControlNotify(a)
#define fgInitDspMidProtect() TRUE

#define DATA_DISC_WMA_SUPPORT

#define  DSP_BUSY  (0x1)
#define  DSP_OK    (0)
#define DSP_FAIL   (-1)


typedef enum
{
  AV_SYNC_FREE_RUN = 0, // no need to syn
  AV_SYNC_SLAVE,        // syn to STC
  AV_SYNC_AUDIO_MASTER, // update A-PTS to A-STC, may use in PCR or HardDisk playback
} AV_SYNC_MODE_T;

extern UINT32 _u4DspSIntLD;
extern UINT8 _uDspSIntAddr;
extern UINT32 _u4DspSIntSD;
extern UINT32 _u4ShareMem;        // Risc 8032 share mem
extern DSP_CMD_T _rDspCmd;
extern UINT8 _uDspRIntAddr;
extern UINT32 _u4DspRIntData;
extern UINT32 _u4DspTableAddr;
extern UINT8 _uDspMixStrTyp;
extern PBS_BLK_SET_T _rPbsBlk[2];

#define MAX_T0_COUNTER_NS 16
extern UINT32 _pu4T0[MAX_T0_COUNTER_NS];
extern UINT32 _pu4T1[MAX_T0_COUNTER_NS];
extern UINT32 _u4PrsPts;
extern UINT32 _u4PrsPtsDec2;

extern BOOL _fgPtsEnable;
extern UINT32 _u4PTSSize;
extern UINT32 _u4DspDumpDramLen;
extern UINT32 _u4DspWRiscData;    // use in dsp interrupt
extern UINT32 _u4Bond;

extern BOOL _fgDecTblFromFlash;
extern BOOL _fgDec2TblFromFlash;
extern BOOL _fgMixTblFromFlash;
extern BOOL _fgDecFromFlash;
extern BOOL _fgDec2FromFlash;
extern BOOL _fgMixFromFlash;
extern BOOL _fgCommRAMFromFlash;
extern BOOL _fgPostRamFromFlash;
extern BOOL _fgRamBoot;
extern BOOL _fgRamBootRamFromFlash;

// 8032 Emulation
extern UINT8 _uSharedGrpPtr;
extern UINT8 _uSharedItemPtr;
void vLoadDspDRAMTable (UINT32 u4Type);

#ifdef __cplusplus
extern "C"
{
#endif                          /* __cplusplus */


/*--------------------- Function Interface ----------------------*/
    void SLEEP (int value);
    BOOL WriteDspCommDram (UINT32 u4Addr, UINT32 u4Data);
    BOOL WriteDspDecDram (UINT32 u4Addr, UINT32 u4Data);
    UINT32 dReadDspCommDram (UINT32 u4Addr);
    UINT32 dReadDspDecDram (UINT32 u4Addr, UINT32 u4Data);
    BOOL ReadDspDecDramA (UINT32 u4Addr, UINT32 * pu4Data);
    BOOL ReadDramA (UINT32 u4Addr, UINT32 * pu4Data);
    void SendDSPInt (UINT8 uAddr, UINT32 u4Data);
    DWORD ReadREG (DWORD u4Addr);
    void WriteREG (DWORD u4Addr, DWORD u4Data);
    DWORD ReadINTREG (void);
    void WriteINTREG (void);
    void DBGLogB (UINT8 uV1, UINT8 uV2, UINT8 uV3, UINT8 uV4);
    void vDspIRQ (void);
    void DumpDram (UINT32 u4Len, UINT32 u4Adr);
    BOOL fgCheckCompareResult ();
    UINT32 dwQueryEmphasis (UINT32 u4Idx);
//void vDspIntSvc (void);
    void vSetAdac (UINT32 u4Cmd, UINT32 u4Arg1, UINT32 u4Arg2);
    DWORD u4GetDspImgAddr (void);
// DspInt
    void vDspSendSample (void);
    void vDspAOutEnable (void);
    void vDspFlushDone (void);
    void vDspSendSampleDec2 (void);
    void vDspAOutEnableDec2 (void);
    void vDspFlushDoneDec2 (void);
// EndDspInt
// DspOP
    void vDspDecodeSpeedUp (void);
// EndDSPOP
// DspInit
    void vDspInitDiscType (void);
// EndDspInit
// *********************************************************************
// Routine to notify shared data changed
// *********************************************************************
    void vNotifySharedInfo (UINT16 u2Item1, UINT16 u2Item2, UINT16 u2Item3);
    void vNotifySharedGroup (UINT8 uGroup);
    void vUopNotifySharedInfo (UINT16 u2Item1, UINT16 u2Item2, UINT16 u2Item3);
    void vUopNotifySharedGroup (UINT8 uGroup);
// *********************************************************************
    void vChangePowerMode (UINT8 uValue);
    void vNotifyShareEntry (UINT8 uAddr, UINT8 uValue);
// *********************************************************************

// Emulation for flash to DRAM DMA function
    void vFlash2DramDma (UINT32 u4Dest, UINT32 u4Src, UINT32 u4Len);
    UINT32 u4GetFlashData (UINT32 u4Addr);
    extern void vShowError (UINT32 ErrorCode);
// Emulation only on DspState
    void UpdatePTSInfo (void);
    void UpdateSpectrum (void);
    void vUpdateShmVol (void);


// Dspintf
    UINT32 u4GetDspFlashVersion (void);
    BOOL fgIsMultiChannelOut (void);

    UINT32 u4GetMixSoundStartByteAddr(void);
    UINT32 u4GetMixSoundBufSize(void);
    UINT16 u2GetMixSoundStatus(void);
    void LOG(char ucLevel,char* cStr,...);
/* command queue related functions */
    void DSP_SendDspTaskCmd(UINT32 u4Cmd);
    BOOL fgGetDspCmd(UINT32 pu4Cmd);

#ifdef __cplusplus
}
#endif                          /* __cplusplus */

#define uReadDspShmBYTE(addr) ((const UINT8)*((UINT8*)(_u4ShareMem+addr)))
#define u2ReadDspShmWORD(addr) ((const UINT16)*((UINT16*)(_u4ShareMem+addr)))
#define u4ReadDspShmDWRD(addr) ((const UINT32)*((UINT32*)(_u4ShareMem+addr)))

#define vWriteDspShmBYTE(addr,value) *((UINT8*)(_u4ShareMem + addr))=(UINT8)(value)
#define vWriteDspShmWORD(addr,value) *((UINT16*)(_u4ShareMem + addr))=(UINT16)(value)
#define vWriteDspShmDWRD(addr,value) *((UINT32*)(_u4ShareMem + addr))=(UINT32)(value)

#define INFS44K 0x00
#define INFS48K 0x01
#define INFS88K 0x02
#define INFS96K 0x03
#define INFS176K 0x04
#define INFS192K 0x05

#endif                          // EMULATION_FPGA

#endif                          // DSP_INTF_H
