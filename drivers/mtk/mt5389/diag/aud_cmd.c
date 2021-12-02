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
#define AUD_EXTERN_FILE

/*****************************************************************************
*  Audio : Diagnostic command
*****************************************************************************/
// KERNEL
#include "x_util.h"
#include "x_os.h"
#include "x_printf.h"
#include "x_stl_lib.h"

#define DEFINE_IS_LOG   CLI_IsLog
#include "x_debug.h"
#include "x_util.h"
#include "x_hal_5381.h"
#include "x_hal_io.h"

#include "aud_cmd.h"
#include "aud_if.h"
#include "sif_if.h"
#include "x_assert.h"
#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
#include "oss_adap.h"
#endif

#ifndef __KERNEL__
#include "../../mt53xx_com/53xx_com_driver/aud/adsp/include_API/dsp_common.h"
#else // __KERNEL__
#include "dsp_common.h"
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/param.h>
#endif // __KERNEL__

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
extern void AUD_OutPadEnable(UINT8 u1DecId, BOOL fgEnable);
extern void SPDIF_InInit(void) ;
extern BOOL SPDIF_InLockCheck(void) ;
extern void SPDIF_InRegisterDump(void) ;
extern void _MT5365_DacChlSel(UINT8 u1DacId,AUD_CHL_DEF_T eChl);
extern void _MT5365_DacChlSelQuery(void);
extern void _MT5365_Mute(UINT8 u1DacId,BOOL fgMute);
extern void _MT5365_MuteALRx(UINT8 u1ALRx,BOOL fgMute);
extern void _MT5365_InterPWMEnable(BOOL fgEnable);
extern void _MT5365_InterPWMGainSet(INT32 pgain);
extern UINT32 _MT5365_InterPWMGainGet(void);
extern void _MT5365_InterDacEnable(BOOL fgMute);
extern void _MT5365_Init(void);
extern UINT32 AUD_DspGetPc(void);
extern void AUD_GetRWPtr(UINT8 u1DecId, UINT32 * pu4Rp, UINT32 * pu4Wp, UINT32 *pu4Size);
extern UINT32 AUD_DspProbePause(void);
extern UINT32 AUD_DspProbeGo(void);
extern UINT32 AUD_DspProbeStep(UINT32 u4Count);
extern UINT32 fgAUD_DspProbeHalt(void);
extern UINT32 u4AUD_DspProbeRead(UINT32 u4Addr);
extern UINT32 AUD_DspProbeWrite(UINT32 u4Addr, UINT32 u4Data);
extern UINT32 u4AUD_DspProbePBCfg(BOOL fgSet,UINT32 u4Value);
extern UINT32 u4AUD_DspProbePB(BOOL fgSet,UINT32 u4BPNum, UINT32 u4Addr);
extern UINT32 u4AUD_DspProbeStopOnWrite(UINT32 u4Addr, UINT32 u4Data, UINT32 u4Mask);
extern UINT32 u4AUD_DspProbeChkStopOnWrite(BOOL *fgEnable, UINT32 *u4Data, UINT32 *u4Mask);

extern void AUD_AudioPTSDelayEnable(UINT8 u1DecId, UINT8 uDlyFactor);
extern void AUD_AoutPADMux(AUD_CH_T ePAD_SDATA0, AUD_CH_T ePAD_SDATA1, AUD_CH_T ePAD_SDATA2,
                    AUD_CH_T ePAD_SDATA3, AUD_CH_T ePAD_SDATA4, AUD_CH_T ePAD_SDATA5,
                    UINT8 PAD_MCLK0);
extern void AUD_AoutPADMuxQuery(void);
extern BOOL AUD_DspSetIecRawFlag(UINT8 u1Flag);
extern BOOL AUD_DspSetIecRawDelay(INT16 i2Delay);

extern void vWriteDspSram(UINT32 u4Addr,UINT32 u4Value);
#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
extern void vWriteDspSram32(UINT32 u4Addr,UINT32 u4Value);
#endif
extern UINT32 u4ReadDspSram(UINT32 u4Addr);
#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
extern UINT32 u4ReadDspSram32(UINT32 u4Addr);
#endif
#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
extern void AudAoutGainCtrl(UINT8 u1DecId, UINT8 CHID, UINT16 Gain);
#endif
extern void vADSPTaskInit(void);
//extern void AUD_GetAudioPes(UCHAR ucDecId, PSR_AUDIO_PES_T * prAudioPes);
extern AV_SYNC_MODE_T AUD_GetAvSynMode(UINT8 u1DecId);
extern void AUD_SetAvSynMode(UCHAR ucDecId, AV_SYNC_MODE_T eSynMode);
extern void AUD_AoutInvertData(UINT8 ucDecId, BOOL fgInvert);
extern void AUD_AoutFormat(UINT8 ucDecId, DATA_FORMAT_T eDataFormat);
extern void DSP_ShareInforDump(UINT8 u1Group);
//Light added for AVC
extern void DSP_ShareInforWriteWord(UINT32 ADDR, UINT16 VALUE);
extern void DSP_CommDramDump(UINT32 u4Address, UINT32 u4Len);
extern void DSP_NormDramDump(UINT8 u1Page, UINT32 u4Address, UINT32 u4Len);
extern void DSP_CmptDramDump(UINT8 u1Page, UINT32 u4Address, UINT32 u4Len);
extern void Dsp_WriteDspCommDram(UINT32 u4SrcAddr,UINT32 u4Value);
#ifdef CC_AUD_FIR_SUPPORT
extern UINT32 DSP_CommDramAddr(UINT32 u4Address) ; //CC_AUD_FIR_SUPPORT
#endif
extern void AudShowDspStatus(void);
extern void AudShowMtsStatus(UINT8 u1DecId);
extern void AudShowStatus(UINT8 u1DecId);
extern void AudShowDspVer(BOOL fgShowUnitChkSum);
extern void AudShowAsrcStatus(void);
extern void AudShowAsrcBufferStatus(UINT8 u1ChMsk,BOOL fgUpdateRWP);
extern void AudShowDspCapability(void);
extern void DSP_SetAudLogPeriod(UINT32 u4Period);
extern BOOL fgDspReadPtrSet (UCHAR ucDecId, UINT32 u4Address);
extern void AUD_AoutDacFs(UINT8 u1DecId, SAMPLE_FREQ_T eFs);
extern void SPDIF_InChSel(UINT8 u1Ch);
extern void AUD_AoutDrvCur(UINT8 u1DecId, UINT8 u1MckDrvCur, UINT8 u1BckDrvCur ,UINT8 u1LRckDrvCur , UINT8 u1AOSDATADrvCur);
extern void AUD_StopMemStr(void);
extern void AUD_ChlOutDump(UINT8 u1Chl);
extern void AUD_DspProcMode(UINT8 u1DecId, UINT16 u2Mode);
extern void AUD_AvMuxSel(AUD_INPUT_ID_T eInputId);
extern UINT8 ADAC_CodecRegRead(UINT8 u1Offset);
extern void ADAC_CodecRegWrite(UINT8 u1Offset, UINT8 u1Data);
extern UINT8 ADAC_AmpRegRead(UINT8 u1Offset);
extern void ADAC_AmpRegWrite(UINT8 u1Offset, UINT8 u1Data);
extern void DSP_SetDualDecMode(BOOL fgEnable);
#ifdef CC_MT5391_AUD_3_DECODER
extern void DSP_SetTriDecMode(BOOL fgEnable);
#endif
extern void AUD_ClkCfgDualDecMode(BOOL fgEnable);
extern void AUD_DspSetSRSTSXTinputgain(UINT32 u4Mode);
//extern void DSP_LogAtvTvSys(void);
extern UINT32 AUD_AudioDescriptionChlSel(UINT8 u1DecId, AUD_CHL_DEF_T eChl);
extern AUD_TYPE_T DSP_GetAudChlInfo(UINT8 u1DecId);
extern TV_AUD_SYS_T DSP_GetAtvTvSystem(void);
extern void DSP_LogAtvTvSys(void);
extern UINT32 AUD_GetDspCacheHitRate(UINT8 u1Cache);
extern void AUD_DumpUopCommand(void);
extern INT32 AUD_Read32ByteID (UINT8 *u1ID_p, UINT32 u4Num);
extern BOOL _AudFMRadioDetection(void);

#if (defined(DSP_SUPPORT_SRSTSHD) || defined(DSP_SUPPORT_SRSWOWHD))
extern void DSP_SRSTSHDSurrLvl(UINT8 u1Mode);
extern void DSP_SRSTSHDInpg(UINT8 u1Mode);
extern void DSP_SRSTSHDOutg(UINT8 u1Mode);
extern void DSP_SRSTSHDSetSpkSize(UINT8 u1Mode);
extern void DSP_SRSTSHDSetTSCtrl(void);
extern void DSP_SRSTSHDTBLvl(UINT8 u1Mode);
extern void DSP_SRSTSHDDef(UINT8 u1Mode);
extern void DSP_SRSTSHDElev(UINT8 u1Mode);
extern void DSP_SRSTSHDFntCCtrl(UINT8 u1Mode);
extern void DSP_SRSTSHDFntSCtrl(UINT8 u1Mode);
extern void DSP_SRSTSHDMode(UINT32 u4Mode);
#endif

//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
extern void AUD_DspSetUploadMode(UINT8 u1Mode);
extern void AUD_DspUploadDataEnable(void);
extern void AUD_DataUpload_Init (void);
extern void AUD_DataUpload_start (void);
extern BOOL AUD_Dec3_Is_Mp3Enc(void);
//#endif

#ifdef __MODEL_slt__ // by hotkey 0729 for compile
//Rice add for ADSP SLT
//extern void ADSP_SLT_Init(void);
//extern void ADSP_SLT_SetPatID(UINT8 u1PID);
extern INT32 SLT_SortingAud(void);
extern void SLT_FlashingAudInit(void);
extern INT32 SLT_DtvSorting(void);
extern INT32 SLT_AtvSorting(void);
extern INT32 SLT_IntAdSorting(void);
extern INT32 SLT_ROMSorting(void);
#endif

#ifdef BURN_IN_AOUT_TEST
extern UINT32 AudGetAoutStatus(void);
extern void AudResetAoutStatus(void);
extern void AudSetAoutTest(void);
#endif

extern void AUD_SPDIF_Output_HDMI_Rx(BOOL fgEnable);
extern void AudPll1Setting(void);
extern void AudPll2Setting(SAMPLE_FREQ_T eFs);
extern void AudPllPowerDown(UINT8 PLL_ID,BOOL fgPD);
extern UINT16 bDrvVideoGetFrameDelay(UINT8 bPath);

/******************************************************************************
* Declare the macros and reglist for RegTest functions
******************************************************************************/

#define AUD_BASE            AUDIO_BASE
#define AUD_REG_LENGTH      0

REG_TEST_T arAUDRgtList[] = {
    { 0x0000,   eRD_ONLY,   2,  0x00000007, 1,  0 },
    // End.
    { 0xffff, eNO_TYPE, -1 },
};

/* Declare the debug on/off/level and RegTest functions */
CLIMOD_DEBUG_FUNCTIONS(AUD)
//CLIMOD_REGTST_FUNCTIONS(AUD)
/* End of Declare */

/****************************************************************************
** Audio Command
****************************************************************************/

static INT32 _AudCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    AUD_Init();
    AUD_OutPadEnable(AUD_DEC_MAIN, TRUE);
    AUD_OutPadEnable(AUD_DEC_AUX, TRUE);
    ADAC_Mute(FALSE);

    return 1;
}

static INT32 _AudCmdSetDecType(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 ucCodeType;
    UINT8 ucStreamFrom;
    UINT8 u1DecId;
    AUD_FMT_T   eAudDecFormat;
    AUD_DEC_STREAM_FROM_T eAudDecStreamFrom;

    if (i4Argc == 3)
    {
        ucStreamFrom =  (UINT8)StrToInt(szArgv[1]);
        ucCodeType = (UINT8)StrToInt(szArgv[2]);
        // setup decode format
        eAudDecFormat = (AUD_FMT_T)ucCodeType;
        // stream from
        eAudDecStreamFrom = (AUD_DEC_STREAM_FROM_T)ucStreamFrom;
        AUD_SetDecType(AUD_DEC_MAIN, eAudDecStreamFrom, eAudDecFormat);
    }
    else if (i4Argc == 4)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
        ucStreamFrom =  (UINT8)StrToInt(szArgv[2]);
        ucCodeType = (UINT8)StrToInt(szArgv[3]);

        if (u1DecId >= AUD_DEC_NUM)
        {
            goto lbUsage;
        }
        // setup decode format
        eAudDecFormat = (AUD_FMT_T)ucCodeType;
        // stream from
        eAudDecStreamFrom = (AUD_DEC_STREAM_FROM_T)ucStreamFrom;
        AUD_SetDecType(u1DecId, eAudDecStreamFrom, eAudDecFormat);
    }
    else
    {
        goto lbUsage;
    }
    return 1;

lbUsage:
    Printf("Usage: setdec {[decoder id]} [stream from] [stream type]\n");

#ifdef CC_AUD_4_DECODER_SUPPORT
    Printf("[decoder id]    0:MAIN  1:AUX 2:THIRD 3:4TH\n");
#elif defined (CC_MT5391_AUD_3_DECODER)
    Printf("[decoder id]    0:MAIN  1:AUX 2:THIRD\n");
#else
    Printf("[decoder id]    0:MAIN  1:AUX \n");
#endif

    Printf("[stream from] 1:D-TUNER  2:A-TUNER 3:SPDIF 4.LINE-IN 5.HDMI 6.MEMORY (ex.Pink noise) 7:Reserve 8:INTERNAL\n");
    Printf("[stream type] 0:INTERNAL (ex.Pink noise) 1:MPEG  2:AC3  3:PCM  4:MP3  5:AAC 6:DTSDec  \n");
    Printf("              7:WMA  11:MTS  12:CANAL+  13:PAL  14:A2  18:DETECTOR  20:LPCM  \n");
    Printf("              21:FMRDO, 22:FMRDODET, 23:SBCDEC , 24:SRCENC, 25:MP3ENC\n");
    Printf("              26:G711DEC, 27:G711ENC, 28:DRA, 29:COOK\n");
    Printf("              30:G729DEC, 31:VORBISDEC, 32:WMAPro\n");
    return -1;
}

static INT32 _AudCmdDualDecMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Flag = FALSE;

    if (i4Argc < 2)
    {
        Printf("Usage: dual  [on/off]\n");
        Printf("[on/off]     1: on  \n");
        Printf("             0: off  \n");
        return -1;
    }

    if (i4Argc >= 2)
    {
        if (x_strcmp(szArgv[1], "on") == 0)
        {
            u1Flag = TRUE;
        }
        else if (x_strcmp(szArgv[1], "1") == 0)
        {
            u1Flag = TRUE;
        }
        else if (x_strcmp(szArgv[1], "off") == 0)
        {
            u1Flag = FALSE;
        }
        else if (x_strcmp(szArgv[1], "0") == 0)
        {
            u1Flag = FALSE;
        }
        else
        {
              Printf("Usage: dual  [on/off]\n");
              Printf("[on/off]     1: on  \n");
              Printf("             0: off  \n");
              return -1;
        }
     }

    DSP_SetDualDecMode(u1Flag);
    AUD_ClkCfgDualDecMode(u1Flag);

    return 1;
}

#ifdef CC_MT5391_AUD_3_DECODER
static INT32 _AudCmdTriDecMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Flag = FALSE;

    if (i4Argc < 2)
    {
        Printf("Usage: tridec  [on/off]\n");
        Printf("[on/off]     1: on  \n");
        Printf("             0: off  \n");
        return -1;
    }

    if (i4Argc >= 2)
    {
        if (x_strcmp(szArgv[1], "on") == 0)
        {
            u1Flag = TRUE;
        }
        else if (x_strcmp(szArgv[1], "1") == 0)
        {
            u1Flag = TRUE;
        }
        else if (x_strcmp(szArgv[1], "off") == 0)
        {
            u1Flag = FALSE;
        }
        else if (x_strcmp(szArgv[1], "0") == 0)
        {
            u1Flag = FALSE;
        }
        else
        {
              Printf("Usage: tridec  [on/off]\n");
              Printf("[on/off]     1: on  \n");
              Printf("             0: off  \n");
              return -1;
        }
     }

    DSP_SetTriDecMode(u1Flag);

    return 1;
}
#endif

static INT32 _AudCmdResume(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;

    if (i4Argc == 1)
    {
        AUD_DSPCmdResume(AUD_DEC_MAIN);
    }
    else if (i4Argc == 2)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
        if (u1DecId >= AUD_DEC_NUM)
        {
            goto lbUsage;
        }
        AUD_DSPCmdResume(u1DecId);
    }
    else
        goto lbUsage;

    return 1;

lbUsage:
    Printf("Usage: resume [decoder id]\n");
#ifdef CC_AUD_4_DECODER_SUPPORT
    Printf("[decoder id]    0:MAIN  1:AUX 2:THIRD 3:4TH\n");
#elif defined (CC_MT5391_AUD_3_DECODER)
    Printf("[decoder id]    0:MAIN  1:AUX 2:THIRD\n");
#else
    Printf("[decoder id]    0:MAIN  1:AUX \n");
#endif
    return -1;
}

static INT32 _AudCmdPause(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;

    if (i4Argc == 1)
    {
        AUD_DSPCmdPause(AUD_DEC_MAIN);
    }
    else if (i4Argc == 2)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
        if (u1DecId >= AUD_DEC_NUM)
        {
            goto lbUsage;
        }

        AUD_DSPCmdPause(u1DecId);
    }
    else
        goto lbUsage;

    return 1;

lbUsage:
    Printf("Usage: pause [decoder id]\n");
#ifdef CC_AUD_4_DECODER_SUPPORT
    Printf("[decoder id]    0:MAIN  1:AUX 2:THIRD 3:4TH\n");
#elif defined (CC_MT5391_AUD_3_DECODER)
    Printf("[decoder id]    0:MAIN  1:AUX 2:THIRD\n");
#else
    Printf("[decoder id]    0:MAIN  1:AUX \n");
#endif
    return -1;
}

static INT32 _AudCmdPlay(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;

    if (i4Argc == 1)
    {
        AUD_DSPCmdPlay(AUD_DEC_MAIN);
    }
    else if (i4Argc == 2)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
        if (u1DecId >= AUD_DEC_NUM)
        {
            goto lbUsage;
        }

        AUD_DSPCmdPlay(u1DecId);
#ifdef CC_MT5391_AUD_3_DECODER  //gallen 0521
    #if 1//def ENABLE_AUDIO_RECORD_SUPPORT
        if ((u1DecId == AUD_DEC_THIRD) && (!AUD_Dec3_Is_Mp3Enc()))
    #else
        if (u1DecId == AUD_DEC_THIRD)
    #endif
        {
            AUD_DspADEnable(AUD_DEC_MAIN, TRUE);
        }
#endif
    }
    else
        goto lbUsage;

    return 1;

lbUsage:
    Printf("Usage: play [decoder id]\n");

#ifdef CC_AUD_4_DECODER_SUPPORT
    Printf("[decoder id] 0:MAIN  1:AUX  2:THIRD 3:4TH\n");
#elif defined (CC_MT5391_AUD_3_DECODER)
    Printf("[decoder id] 0:MAIN  1:AUX  2:THIRD \n");
#else
    Printf("[decoder id] 0:MAIN  1:AUX \n");
#endif

    return -1;
}

static INT32 _AudCmdStop(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;

    if (i4Argc == 1)
    {
        AUD_DSPCmdStop(AUD_DEC_MAIN);
    }
    else if (i4Argc == 2)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
        if (u1DecId >= AUD_DEC_NUM)
        {
            goto lbUsage;
        }

        AUD_DSPCmdStop(u1DecId);
#ifdef CC_MT5391_AUD_3_DECODER  //gallen 0521
    #if 1 //def ENABLE_AUDIO_RECORD_SUPPORT
        if ((u1DecId == AUD_DEC_THIRD) && (!AUD_Dec3_Is_Mp3Enc()))
    #else
        if (u1DecId==AUD_DEC_THIRD)
    #endif
        {
            AUD_DspADEnable(AUD_DEC_MAIN, FALSE);
        }
#endif
    }
    else
        goto lbUsage;

    return 1;

lbUsage:
    Printf("Usage: stop [decoder id]\n");
#ifdef CC_AUD_4_DECODER_SUPPORT
    Printf("[decoder id] 0:MAIN  1:AUX  2:THIRD 3:4TH\n");
#elif defined (CC_MT5391_AUD_3_DECODER)
    Printf("[decoder id] 0:MAIN  1:AUX  2:THIRD \n");
#else
    Printf("[decoder id] 0:MAIN  1:AUX \n");
#endif
    return -1;
}

static INT32 _AudCmdQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;

    if (i4Argc == 1)
    {
        u1DecId = AUD_DEC_MAIN;
    }
    else if (i4Argc == 2)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
        if ((u1DecId >= AUD_DEC_NUM) && (u1DecId != 9))
        {
            goto lbUsage;
        }
    }
    else
    {
        goto lbUsage;
    }

    if (u1DecId == 9)
    {
        int i;
        for (i=0;
             #ifdef CC_AUD_4_DECODER_SUPPORT
             i<4
             #elif defined(CC_MT5391_AUD_3_DECODER)
             i<3
             #else
             i<2
             #endif
             ;i++)
        {
            AudShowStatus(i);
        }
    }
    else
    {
        AudShowStatus(u1DecId);
    }

    return 1;

lbUsage:
    Printf("Usage: query [decoder id]\n");
#ifdef CC_AUD_4_DECODER_SUPPORT
    Printf("[decoder id] 0:MAIN 1:AUX 2:THIRD 3:4TH 9: ALL\n");
#elif defined (CC_MT5391_AUD_3_DECODER)
    Printf("[decoder id] 0:MAIN 1:AUX 2:THIRD 9: ALL\n");
#else
    Printf("[decoder id] 0:MAIN 1:AUX 9: ALL\n");
#endif
    return -1;
}

#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
static INT32 _AudOssAdap_Cmd(INT32 i4Argc, const CHAR ** szArgv)
{
	AUD_OSS_ADAP_MSG_T tMsg;
	UINT32 devId;
	UINT32 cmd;
	UINT32 dataAddr = 0;
	UINT32 dataSize = 0;

	if(i4Argc != 3)
	{
		Printf("[_AudOssAdap_Cmd] argc error. \n");
		goto lbUsage;
	}

	
	devId = (UINT32)StrToInt(szArgv[1]);
	cmd   = (UINT32)StrToInt(szArgv[2]);

	tMsg.cmd = cmd;
	tMsg.devId = devId;
	tMsg.dataAddr = dataAddr;
	tMsg.dataSize = dataSize;

	_AudOssAdap_SendMsg(tMsg);

	return 0;

	lbUsage:
    Printf("Usage: cmd [devId] [cmd]\n");

    Printf("[devId] 0: DEVICE_OUT_USB_HEADPHONE, 1: DEVICE_OUT_BLUETOOTH_EARPHONE\n");
	Printf("[cmd] \n");
	Printf("0: AUD_OSS_ADAP_CONNECT\n");
	Printf("1: AUD_OSS_ADAP_START\n");
	Printf("2: AUD_OSS_ADAP_TRANSFER\n");
	Printf("3: AUD_OSS_ADAP_STOP\n");
	Printf("4: AUD_OSS_ADAP_DISCONNECT\n");

    return -1;
}
static INT32 _AudOssAdap_Ops(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 devId;
	
	if(i4Argc != 2)
	{
		Printf("[_AudOssAdap_Ops] argc error. \n");
		goto lbUsage;
	}

	devId = (UINT32)StrToInt(szArgv[1]);
	_AudOssAdap_Enable(devId);

	return 0;
	
	lbUsage:
    Printf("Usage: e [devId]\n");
	Printf("[devId] 0: DEVICE_OUT_USB_HEADPHONE, 1: DEVICE_OUT_BLUETOOTH_EARPHONE, 2: BUILD-IN SPEAKER\n");
	
    return -1;
}

static INT32 _AudOssAdap_SetSrc(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 srcType;
	
	if(i4Argc != 2)
	{
		Printf("[_AudOssAdap_SetSrc] argc error. \n");
		goto lbUsage;
	}

	srcType = (UINT32)StrToInt(szArgv[1]);

	_AudOssAdap_SetSourceType(srcType);
	return 0;
	
	lbUsage:
    Printf("Usage: s [srcType]\n");
	Printf("[srcType] 0: MIXSOUND, 1: UPLOADER\n");
	
    return -1;
}




#endif

static INT32 _AudCmdDspVer(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc == 1)
    {
       AudShowDspVer(0);
    }
    else
    {
       AudShowDspVer(1);
    }
    return 1;
}

#ifdef CC_AUD_4_DECODER_SUPPORT

#define USE_UPLOAD_PATH_TEST

extern UINT32 u4GetSBCEncFIFOStart(void);
extern UINT32 u4GetSBCEncFIFOEnd(void);
extern UINT32 u4GetSBCEncWritePnt(void);
static INT32 _AudCmdSBCEncTest(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Addr;
    UINT32 u4Size;
    #ifdef USE_UPLOAD_PATH_TEST
    UINT32 u4Pts,u4SrcAddr,u4SrcSize;
    #else
    UINT32 u4FifoSA;
    UINT32 u4FifoEA;
    UINT32 u4RP, u4WP;
    #endif

    if (i4Argc != 3)
    {
        Printf("Usage: sbcd [output addr] [output size]\n");
        return -1;
    }

    u4Addr = (UINT32)StrToInt(szArgv[1]);
    u4Size = (UINT32)StrToInt(szArgv[2]);

#ifdef USE_UPLOAD_PATH_TEST

    AUD_DspSetUploadMode(3);
    while (u4Size)
    {
        AUD_GetUploadInfo(&u4Pts, &u4SrcAddr, &u4SrcSize);
        if (u4SrcSize)
        {
            x_memcpy((VOID*)(VIRTUAL(u4Addr)),
                        (VOID*)(VIRTUAL(u4SrcAddr)),
                        u4SrcSize);
            u4Addr += u4SrcSize;
            u4Size -= u4SrcSize;
            Printf(".");
        }
        else
        {
            x_thread_delay(10);
        }
    }
    AUD_DspSetUploadMode(0);
#else
    u4FifoSA = u4GetSBCEncFIFOStart();
    u4FifoEA = u4GetSBCEncFIFOEnd();
    u4RP = u4FifoSA;

    AUD_SetDecType(AUD_DEC_4TH, AUD_STREAM_FROM_OTHERS, AUD_FMT_SBCENC);
    AUD_DSPCmdPlay(AUD_DEC_4TH);

    while (u4Size)
    {
        u4WP = u4GetSBCEncWritePnt();
        if ((u4WP >= u4FifoSA) && (u4WP < u4FifoEA)) //validity check
        {
            if (u4RP < u4WP)
            {
                if ((u4WP - u4RP) > 0x400)
                {
                    x_memcpy((VOID*)(VIRTUAL(u4Addr)),
                                (VOID*)(VIRTUAL(u4RP)),
                                0x400);
                    u4RP += 0x400;
                    if (u4RP == u4FifoEA)
                        u4RP = u4FifoSA;
                    u4Addr += 0x400;
                    u4Size -= 0x400;
                    Printf(".");
                    continue;
                }
            }
            else if (u4RP > u4WP)
            {
                if (((u4FifoEA - u4FifoSA) - (u4RP - u4WP)) > 0x400)
                {
                    x_memcpy((VOID*)(VIRTUAL(u4Addr)),
                                (VOID*)(VIRTUAL(u4RP)),
                                0x400);
                    u4RP += 0x400;
                    if (u4RP == u4FifoEA)
                        u4RP = u4FifoSA;
                    u4Addr += 0x400;
                    u4Size -= 0x400;
                    Printf(".");
                    continue;
                }
            }
        }

        x_thread_delay(10);
    }
    AUD_DSPCmdStop(AUD_DEC_4TH);

#endif

    return 1;
}
#endif

static INT32 _AudCmdSyncMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1SyncMode;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u1DecId =  (UINT8)StrToInt(szArgv[1]);

    if (u1DecId >= AUD_DEC_NUM)
    {
        goto lbUsage;
    }

    if (i4Argc == 2)
    {
        // Get AV sync mode
        AV_SYNC_MODE_T eSyncMode;
        eSyncMode = AUD_GetAvSynMode(u1DecId);
        if (eSyncMode == AV_SYNC_FREE_RUN)
        {
            Printf("Dec %d: Free run\n", u1DecId);
        }
        else if (eSyncMode == AV_SYNC_SLAVE)
        {
            Printf("Dec %d: AV sync\n", u1DecId);
        }
        else
        {
            Printf("Dec %d: Audio master\n", u1DecId);
        }
    }
    else
    {
        // Set AV sync mode
        u1SyncMode = (UINT8)StrToInt(szArgv[2]);
        AUD_SetAvSynMode(u1DecId, (AV_SYNC_MODE_T)u1SyncMode);
    }

    return 1;

lbUsage:
    Printf("Usage: sync [decoder id] [sync mode]\n");
#ifdef CC_AUD_4_DECODER_SUPPORT
    Printf("[decoder id] 0:MAIN  1:AUX  2:THIRD 3:4TH\n");
#elif defined (CC_MT5391_AUD_3_DECODER)
    Printf("[decoder id] 0:MAIN  1:AUX  2:THIRD \n");
#else
    Printf("[decoder id] 0:MAIN  1:AUX \n");
#endif
    Printf("[sync mode]  %d:FREE RUN %d:SLAVE MODE %d:AUDIO MASTER\n",
        (UINT8)AV_SYNC_FREE_RUN, (UINT8)AV_SYNC_SLAVE, (UINT8)AV_SYNC_AUDIO_MASTER);
    return -1;
}

static INT32 _AudCmdPts(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    DMX_AUDIO_PES_T rAudioPes = {0};

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    x_memset((VOID *)VIRTUAL((UINT32)&rAudioPes), 0, sizeof(DMX_AUDIO_PES_T));
    u1DecId =  (UINT8)StrToInt(szArgv[1]);
    //AUD_GetAudioPes(ucDecId, &rAudioPes);

    Printf( "dev: %u, PTS: 0x%08x, DTS: 0x%08x, Payload: 0x%08x, size: %u\n",
        u1DecId, (UINT32)(rAudioPes.u4Pts & 0xffffffff),
        (UINT32)(rAudioPes.u4Dts & 0xffffffff), rAudioPes.u4Wp, 0/*,
        rAudioPes.u2PayloadSize*/);

    return 1;

lbUsage:
    Printf("Usage: pts [decoder id]\n");
#ifdef CC_AUD_4_DECODER_SUPPORT
    Printf("[decoder id] 0:MAIN  1:AUX  2:THIRD 3:4TH\n");
#elif defined (CC_MT5391_AUD_3_DECODER)
    Printf("[decoder id] 0:MAIN  1:AUX  2:THIRD \n");
#else
    Printf("[decoder id] 0:MAIN  1:AUX \n");
#endif
    return -1;
}

static INT32 _AudCmdSetLogPeriod(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4LogPeriod;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u4LogPeriod = (UINT32)StrToInt(szArgv[1]);
    DSP_SetAudLogPeriod(u4LogPeriod);

    return 1;

lbUsage:
    Printf("Usage: sp [second]\n");
    return -1;
}

static INT32 _AudCmdSetStream(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8  u1DecId;
    UINT32 u4StreamAddr;
    UINT32 u4StreamLen;
    MEM_BUFFER_INFO_T rMemBuf;

    if (i4Argc < 4)
    {
        goto lbUsage;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u4StreamAddr = (UINT32)StrToInt(szArgv[2]);
    u4StreamLen = (UINT32)StrToInt(szArgv[3]);

    rMemBuf.pData = (UINT8 *)u4StreamAddr;
    rMemBuf.u4Length = u4StreamLen;

    AUD_SetDecMemBuffer(u1DecId, &rMemBuf);

    return 1;

lbUsage:
    Printf("Usage: str [decoder id] [addr] [len]\n");
    return -1;
}



/****************************************************************************
** spdif Command
****************************************************************************/

static INT32 _SpdifCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    SPDIF_InInit();

    return 1;

}

static INT32 _SpdifCmdReset(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    //TODO

    return 1;

}

static INT32 _SpdifCmdEnable(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    //TODO

    return 1;

}

static INT32 _SpdifCmdDisable(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    //TODO

    return 1;

}

static INT32 _SpdifCmdCheckLock(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    if(SPDIF_InLockCheck())
        Printf("Lock\n");
    else
        Printf("UnLock\n");

    return 1;
}

static INT32 _SpdifCmdSelectCh(INT32 i4Argc, const CHAR ** szArgv)
{

    UINT8 ucChannel;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    ucChannel = (UINT8)StrToInt(szArgv[1]);
    ucChannel = (ucChannel & ~0xf8);

    SPDIF_InChSel(ucChannel);

    return 1;

lbUsage:
    Printf("selectch [channel]\n");
    return -1;
}

static INT32 _SpdifCmdReadReg(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    SPDIF_InRegisterDump();

    return 1;
}

static INT32 _SpdifCmdWriteReg(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Addr;
    UINT8  ucData;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u4Addr = (UINT32)StrToInt(szArgv[1]);
    ucData = (UINT8)StrToInt(szArgv[2]);

#define AK4114_DEV_ADDR    0x10
#define SIF_CLK_DIV 0x100

    SIF_Write(SIF_CLK_DIV, AK4114_DEV_ADDR, u4Addr, &ucData, 1);

    return 1;

lbUsage:
    Printf("Usage: writerg [addr] [data]\n");
    return -1;
}

static INT32 _SpdifIecSet(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1IecCfg;
    UINT8 u1IecEnable;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u1IecCfg = (UINT8)StrToInt(szArgv[1]);
    u1IecEnable =  (UINT8)StrToInt(szArgv[2]);

    AUD_DspIECConfig((AUD_IEC_T)u1IecCfg, (BOOL)u1IecEnable);

    return 1;

lbUsage:
    Printf("Usage: iecs [iec mode] [enable]\n");
    Printf("[iec mode]  0:PCM 1:RAW \n");
    Printf("[enable]  0:OFF 1:ON \n");
    return -1;
}

//extern void vSetIecChannel(AUD_IEC_CH_T eIecChannel);
static INT32 _SpdifIecChannelSet(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1IecChannel;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u1IecChannel = (UINT8)StrToInt(szArgv[1]);

    UNUSED(AUD_DspIecChannel((AUD_IEC_CH_T)u1IecChannel));
    //vSetIecChannel((AUD_IEC_CH_T)u1IecChannel); //for CLI only, originally this CLI will only take effect when stop then play decoder

    return 1;

lbUsage:
    Printf("Usage: iecc [iec mode]\n");
    Printf("[channel] 0:L_R  1:LS_RS 2:C_SW 3:7_8 4:LINE_IN 5:9_10 6:AUX\n");
    return -1;
}

extern void vIecQuery(void);
static INT32 _SpdifIecQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    vIecQuery();
    return 1;
}

static INT32 _SpdifSetRawDelayFlag(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Flag;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u1Flag =  (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetIecRawFlag(u1Flag);

    return 1;

lbUsage:
    Printf("Usage: flag [control]\n");
    Printf("[control] 0 = Not sync to channel delay, 1 = sync to channel delay\n");
    return -1;
}

static INT32 _SpdifSetRawDelay(INT32 i4Argc, const CHAR ** szArgv)
{
    INT16 i2Delay;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    // handle '-' for negative
    if ((UINT8)*szArgv[1] == '-')
    {
        i2Delay =  -(INT16)StrToInt(szArgv[1] + 1);
    }
    else
    {
        i2Delay =  (INT16)StrToInt(szArgv[1]);
    }

    AUD_DspSetIecRawDelay(i2Delay);

    return 1;

lbUsage:
    Printf("Usage: delay [delay]\n");
    Printf("[control] delay value in ms\n");
    return -1;
}

static INT32 _SpdifBypass(INT32 i4Argc, const CHAR ** szArgv)
{
    INT8 u1Pin;
    INT8 u1Temp;
    u1Temp = PAD_DISTABLE;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u1Pin = (UINT8)StrToInt(szArgv[1]);

    if(u1Pin > PAD_DISTABLE)
    {
        Printf("Please check PIN number.\n");
        goto lbUsage;
    }

    AUD_SpdifInBypass(u1Pin);

    return 1;

lbUsage:
    Printf("SPDIF bypass PIN select [PIN]\n");
    Printf("0:AOSDATA1(97), 1:IF_AGC(98), 2:RF_AGC(99), %d:Disable By Pass. \n", u1Temp);
    return -1;
}

static char cStramFromName[AUD_STREAM_FROM_NUM][18] = {
"Others       ",
"Digital Tuner",
"Analog Tuner ",
"SPDIF In     ",
"Line In      ",
"HDMI In      ",
"Memory       ",
"Buffer Agent ",
"Feeder       ",
"MultiMedia   "
};

static char cSpdifRegModeName[SPDIF_REG_TYPE_NUM][18] = {
"Default",
"User",
};

static void _vSpdifListInstruction (void)
{
    int i;

    Printf("[input source]\n");
    for (i = 0; i < AUD_STREAM_FROM_NUM; i ++)
    {
        Printf("    %d = %s\n", i, &cStramFromName[i][0]);
    }
}

static INT32 _SpdifRegTypeReset(INT32 i4Argc, const CHAR ** szArgv)
{
    AUD_DEC_STREAM_FROM_T eInputSrc;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    eInputSrc =  (AUD_DEC_STREAM_FROM_T)StrToInt(szArgv[1]);

    AUD_DspResetSpdifReg (eInputSrc);

    return 1;

lbUsage:
    Printf("Usage: reg [input source]\n");
    _vSpdifListInstruction();
    return -1;
}

static INT32 _SpdifCopyrightSet(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1IecCopyright;
    AUD_DEC_STREAM_FROM_T eInputSrc;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    eInputSrc =  (AUD_DEC_STREAM_FROM_T)StrToInt(szArgv[1]);
    u1IecCopyright = (UINT8)StrToInt(szArgv[2]);
#ifdef CC_AUD_SUPPORT_SPDIF_V20
    AUD_DspSetSpdifCopyright(eInputSrc, u1IecCopyright);
#else
    AUD_DspSetSpdifCopyright(SPDIF_REG_TYPE_USER, eInputSrc, u1IecCopyright);
#endif

    return 1;

lbUsage:
    Printf("Usage: copy [input source] [copyright]\n");
    _vSpdifListInstruction();
    Printf("[copyright]\n");
    Printf("    0 = copyright is asserted, 1 = no copyright is asserted\n");
    return -1;
}

static INT32 _SpdifCategoryCodeSet(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1IecCategoryCode;
    AUD_DEC_STREAM_FROM_T eInputSrc;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    eInputSrc =  (AUD_DEC_STREAM_FROM_T)StrToInt(szArgv[1]);
    u1IecCategoryCode = (UINT8)StrToInt(szArgv[2]);
    AUD_DspSetSpdifCategoryCode(eInputSrc, u1IecCategoryCode);

    return 1;

lbUsage:
    Printf("Usage: code [input source] [category code]\n");
    _vSpdifListInstruction();
    return -1;
}

static INT32 _SpdifWordLengthSet(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1IecWordLength;
    AUD_DEC_STREAM_FROM_T eInputSrc;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    eInputSrc =  (AUD_DEC_STREAM_FROM_T)StrToInt(szArgv[1]);
    u1IecWordLength = (UINT8)StrToInt(szArgv[2]);
    AUD_DspSetSpdifWordLength(eInputSrc, u1IecWordLength);

    return 1;

lbUsage:
    Printf("Usage: word [input source] [word length]\n");
    _vSpdifListInstruction();
    Printf("[word length]\n");
    Printf("    0 = 24 bits, 2 = 20 bits, 3 = 16 bits\n");
    return -1;
}

static INT32 _SpdifSetInfo (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Copyright, u1CategoryCode;
    AUD_DEC_STREAM_FROM_T eInputSrc;

    if (i4Argc < 4)
    {
        goto lbUsage;
    }

    eInputSrc =  (AUD_DEC_STREAM_FROM_T)StrToInt(szArgv[1]);
    u1Copyright =  (UINT8)StrToInt(szArgv[2]);
    u1CategoryCode =  (UINT8)StrToInt(szArgv[3]);
    UNUSED(AUD_DspSpdifSetInfo(eInputSrc, u1Copyright, u1CategoryCode));

    return 1;

lbUsage:
    Printf("Usage: set [input source] [copy right] [category code]\n");
    _vSpdifListInstruction();
    return -1;
}

static INT32 _SpdifSetProtect (INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL bCpBit, bLbit;
    AUD_DEC_STREAM_FROM_T eInputSrc;

    if (i4Argc < 4)
    {
        goto lbUsage;
    }

    eInputSrc =  (AUD_DEC_STREAM_FROM_T)StrToInt(szArgv[1]);
    bCpBit =  (BOOL)StrToInt(szArgv[2]);
    bLbit =  (BOOL)StrToInt(szArgv[3]);
    UNUSED(AUD_DspSpdifSetCopyProtect(eInputSrc, bCpBit, bLbit));

    return 1;

lbUsage:
    Printf("Usage: protect [input source] [cp bit] [L bit]\n");
    _vSpdifListInstruction();
    return -1;
}

static INT32 _SpdifUpdateModeSet(INT32 i4Argc, const CHAR ** szArgv)
{
    AUD_DEC_STREAM_FROM_T eInputSrc;
    UINT8 eUpdate;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    eInputSrc =  (AUD_DEC_STREAM_FROM_T)StrToInt(szArgv[1]);
    eUpdate = (UINT8)StrToInt(szArgv[2]);
    AUD_DspSetSpdifUpdateMode(eInputSrc, eUpdate);

    return 1;

lbUsage:
    Printf("Usage: copy [input source] [update mode]\n");
    _vSpdifListInstruction();
    Printf("[update mode]\n");
    Printf("    0 = by setup, 1 = by source content\n");
    return -1;
}

static INT32 _SpdifGetChannelStatus(INT32 i4Argc, const CHAR ** szArgv)
{
    SPDIF_CHANNEL_STATUS_T info;

    if (i4Argc < 1)
    {
        goto lbUsage;
    }

    AUD_DspGetSpdifChannelStatus ((SPDIF_CHANNEL_STATUS_T *) &info);

    Printf ("\nSPDIF channel status\n");
    printf ("=============================================\n");
    Printf ("Mode                   0x%02x\n", info.u1Mode);
    Printf ("SampleMode             0x%02x\n", info.u1SampleMode);
    Printf ("Copyright              0x%02x\n", info.u1Copyright);
    Printf ("Pre-emphasis           0x%02x\n", info.u1PreEmphasis);
    Printf ("Category code          0x%02x (L-bit = 0x%x)\n", info.u1Category, (info.u1Category >> 7) & 0x1);
    Printf ("Source number          0x%02x\n", info.u1SourceNum);
    Printf ("Channel number         0x%02x\n", info.u1CannelNum);
    Printf ("Sampling rate          0x%02x\n", info.u1SampleRate);
    Printf ("Clock accuracy         0x%02x\n", info.u1ClockAccuray);
    Printf ("Word length            0x%02x\n", info.u1WordLength);
    Printf ("Original sample rate   0x%02x\n", info.u1OrgSampleRate);
    printf ("=============================================\n");
    return 1;

lbUsage:
    Printf("Usage: cs\n");
    return -1;
}

static INT32 _SpdifGetInfo(INT32 i4Argc, const CHAR ** szArgv)
{
    SPDIF_REG_TYPE_T eRegType;
    UINT8 u1CopyRight, u1CategoryCode, u1Word;
    UINT32 i;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    eRegType = (SPDIF_REG_TYPE_T)StrToInt(szArgv[1]);

    if (eRegType >= SPDIF_REG_TYPE_NUM)
    {
        goto lbUsage;
    }

    Printf ("\nSPDIF reg Info %d (%s):\n", eRegType, &cSpdifRegModeName[eRegType][0]);
    Printf ("-----------------------------------------------------------------\n");
    Printf ("Source               copyright       category code   word length\n");
    Printf ("-----------------------------------------------------------------\n");
    for (i = 0; i < AUD_STREAM_FROM_NUM; i ++)
    {
        u1CopyRight = AUD_DspGetSpdifCopyright(eRegType, (AUD_DEC_STREAM_FROM_T) i);
        u1CategoryCode = AUD_DspGetSpdifCategoryCode(eRegType, (AUD_DEC_STREAM_FROM_T) i);
        u1Word = AUD_DspGetSpdifWordLength(eRegType, (AUD_DEC_STREAM_FROM_T) i);
        if (AUD_DspGetSpdifUpdateMode(eRegType, (AUD_DEC_STREAM_FROM_T) i) == IEC_CHANNEL_STATUS_BY_SETUP)
        {
            Printf ("%2d %s     0x%02X            0x%02X            0x%02X\n",
            	i, &cStramFromName[i][0], u1CopyRight, u1CategoryCode, u1Word);
        }
        else
        {
            Printf ("%2d %s(S)  0x%02X            0x%02X            0x%02X\n",
            	i, &cStramFromName[i][0], u1CopyRight, u1CategoryCode, u1Word);
        }
    }
    Printf ("-----------------------------------------------------------------\n");
    Printf ("(S) = upadted by input source content.\n");

    return 1;

lbUsage:
    Printf("Usage: info [reg type]\n");
    Printf("[reg mode]\n");
    for (i = 0; i < SPDIF_REG_TYPE_NUM; i ++)
    {
        Printf("%d = %s\n", i, &cSpdifRegModeName[i][0]);
    }
    return -1;
}

static INT32 _SpdifAlwaysOutHdmi(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1IecBypass;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u1IecBypass = (UINT8)StrToInt(szArgv[1]);

    UNUSED(AUD_SPDIF_Output_HDMI_Rx((BOOL)u1IecBypass));

    return 1;

lbUsage:
    Printf("Usage: iecb [enable]\n");
    Printf("[enable]    0: normal mode, 1: hdmi always bypass to spdif mode\n");
    return -1;
}


/****************************************************************************
** adac Command
****************************************************************************/
static INT32 _AdacCmdQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    ADAC_SpeakerStateQuery();

    return 1;
}

static INT32 _AdacCmdDataFormat(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Format;
    UINT8 u1DecId;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Format = (UINT8)StrToInt(szArgv[2]);

    AUD_AoutFormat(u1DecId, (DATA_FORMAT_T)u1Format);

    return 1;

lbUsage:
    Printf("Usage: format [decoder id] [format]\n");
    Printf("[decoder id] 0:MAIN  1:AUX \n");
    Printf("[format]     0:FORMAT_RJ  1:FORMAT_LJ 2:FORMAT_I2S \n");
    return -1;
}

static INT32 _AdacCmdDataInvert(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Invert;
    UINT8 u1DecId;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Invert = (UINT8)StrToInt(szArgv[2]);

    AUD_AoutInvertData(u1DecId, (BOOL)u1Invert);
    return 1;

lbUsage:
    Printf("Usage: datainvert [decoder id] [format]\n");
    Printf("[decoder id] 0:MAIN  1:AUX \n");
    Printf("[invert]     0:Not Invert  1:Invert\n");
    return -1;

}

static INT32 _AdacCmdSampleFreq(INT32 i4Argc, const CHAR ** szArgv)
{
    //UINT8 u1Invert;
    //UINT8 u1DecId;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    //u1DecId = (UINT8)StrToInt(szArgv[1]);
    //u1Invert = (UINT8)StrToInt(szArgv[2]);

//    AUD_AoutDacFs(u1DecId, (MCLK_FREQUENCY_T)u1Invert);
    Printf("Not support right now! \n");
    return 1;

lbUsage:
    Printf("Usage: sf [decoder id] [format]\n");
    Printf("[decoder id] 0:MAIN  1:AUX \n");
    Printf("[invert]     0:MCK_128FS  1:MCK_192FS 2:MCK_256FS 3:MCK_384FS 4:MCK_512FS\n");
    return -1;
}

static INT32 _AdacCmdSelectFs(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Fs;
    //AUD_DEC_STREAM_FROM_T eStreamFrom;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u1DecId  =  (UINT8)StrToInt(szArgv[1]);
    u1Fs =  (UINT8)StrToInt(szArgv[2]);
    //eStreamFrom =  AUD_STREAM_FROM_HDMI;

    AUD_AoutDacFs(u1DecId, (SAMPLE_FREQ_T)u1Fs);
    return 1;

lbUsage:
    Printf("Usage: sf [decoder id] [fs]\n");
    Printf("[decoder id] 0:MAIN  1:AUX \n");
    Printf("[fs]     0:FS_16K  1:FS_22K 2:FS_24K 3:FS_32K 4:FS_44K\n"
    	        "         5:FS_48K  6:FS_64K 7:FS_88K 8:FS_96K 9:FS_176K 10:FS_192K\n");
    return -1;
}

static INT32 _AdacCmdDrvCur(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1MckDrv;
    UINT8 u1BckDrv;
    UINT8 u1LRckDrv;
    UINT8 u1AOSDATADrv;

    if (i4Argc < 4)
    {
        goto lbUsage;
    }

    u1DecId  =  (UINT8)StrToInt(szArgv[1]);
    u1MckDrv =  (UINT8)StrToInt(szArgv[2]);
    u1BckDrv =  (UINT8)StrToInt(szArgv[3]);
    u1LRckDrv =  (UINT8)StrToInt(szArgv[4]);
    u1AOSDATADrv =  (UINT8)StrToInt(szArgv[5]);


    AUD_AoutDrvCur(u1DecId, u1MckDrv, u1BckDrv ,u1LRckDrv , u1AOSDATADrv);

    return 1;

lbUsage:
    Printf("Usage: drv [decoder id] [MCLK driving] [BCLK driving] [LRCLK driving] [AOSDATA driving]\n");
    Printf("[decoder id] 0:MAIN  1:AUX \n");
    Printf("[MCLK driving]  2 ~ 8 (mA)\n");
    Printf("[BCLK driving]  2 ~ 8 (mA)\n");
    Printf("[LRCLK driving]  2 ~ 8 (mA)\n");
    Printf("[AOSDATA driving]  2 ~ 8 (mA)\n");

    return -1;
}

static INT32 _AdacCmdWM8776VolCtl(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Vol;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u1Vol  =  (INT16)StrToInt(szArgv[1]);

    ADAC_CodecRegWrite(0x79, u1Vol);

    return 1;

lbUsage:
    Printf("Usage: v [value]\n");
    Printf("[value] 0x00 -00dB \n");
    Printf("[value] 0x2f -00dB \n");
    Printf("[value] ---  ---dB \n");
    Printf("[value] 0x30 -73dB \n");
    Printf("[value] ---  ---dB \n");
    Printf("[value] 0x79 0dB \n");
    Printf("[value] 0x7a 1dB \n");
    Printf("[value] 0x7b 2dB \n");
    Printf("[value] 0x7c 3dB \n");
    Printf("[value] 0x7d 4dB \n");
    Printf("[value] 0x7e 5dB \n");
    Printf("[value] 0x7f 6dB \n");
    return -1;
}

static INT32 _AdacCmdCodecRead(INT32 i4Argc, const CHAR ** szArgv)
{
   UINT8 u1Addr;
    UINT8 u1Data;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u1Addr = (UINT8)StrToInt(szArgv[1]);
    u1Data = ADAC_CodecRegRead(u1Addr);

    Printf("(Add, Data) = (0x%x, 0x%x)\n", u1Addr, u1Data);

    return 1;

lbUsage:
    Printf("Usage: cr [addr]\n");
    return -1;
}

static INT32 _AdacCmdCodecWrite(INT32 i4Argc, const CHAR ** szArgv)
{
   UINT8 u1Addr;
    UINT8 u1Data;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u1Addr = (UINT8)StrToInt(szArgv[1]);
    u1Data = (UINT8)StrToInt(szArgv[2]);

    ADAC_CodecRegWrite(u1Addr, u1Data);

    return 1;

lbUsage:
    Printf("Usage: cw [addr] [data]\n");
    return -1;
}

static INT32 _AdacCmdCodecChlSel(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Sel;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u1Sel = (UINT8)StrToInt(szArgv[1]);

    ADAC_CodecAdChlSel(u1Sel);

    return 1;

lbUsage:
    Printf("Usage: sel [sel]\n");
    return -1;
}



static INT32 _AdacCmdAmpRead(INT32 i4Argc, const CHAR ** szArgv)
{
   UINT8 u1Addr;
    UINT8 u1Data;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u1Addr = (UINT8)StrToInt(szArgv[1]);
    u1Data = ADAC_AmpRegRead(u1Addr);

    Printf("(Add, Data) = (0x%x, 0x%x)\n", u1Addr, u1Data);

    return 1;

lbUsage:
    Printf("Usage: ar [addr]\n");
    return -1;
}

static INT32 _AdacCmdAmpWrite(INT32 i4Argc, const CHAR ** szArgv)
{
   UINT8 u1Addr;
    UINT8 u1Data;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u1Addr = (UINT8)StrToInt(szArgv[1]);
    u1Data = (UINT8)StrToInt(szArgv[2]);

    ADAC_AmpRegWrite(u1Addr, u1Data);

    return 1;

lbUsage:
    Printf("Usage: aw [addr] [data]\n");
    return -1;
}

static INT32 _AdacCmdAmpRamRead(INT32 i4Argc, const CHAR ** szArgv)
{
   UINT8 u1AmpID;
   UINT8 u1Bank;
   UINT8 u1Addr;
   UINT8 u1Data[3];
   UINT8 i;

    if (i4Argc < 4)
    {
        goto lbUsage;
    }

    u1AmpID = (UINT8)StrToInt(szArgv[1]);
    u1Bank = (UINT8)StrToInt(szArgv[2]);
    u1Addr = (UINT8)StrToInt(szArgv[3]);

    if (ADAC_DigiAmp_ReadRam((AUD_AMP_ID_T)u1AmpID, u1Bank, u1Addr, u1Data, FALSE))
    {
        for (i=0; i<3; i++)
        {
            Printf("0x%x ",u1Data[i]);
        }
        Printf("\n");
    }

    return 0;
lbUsage:
    Printf("Usage: arr [AmpID] [Bank] [Addr]\n");
    return -1;
}

static INT32 _AdacCmdAmpRamWrite(INT32 i4Argc, const CHAR ** szArgv)
{
   UINT8 u1AmpID;
   UINT8 u1Bank;
   UINT8 u1Addr;
   AUD_AQ_AMP_RAM_DATA_T eAmpData;

    if (i4Argc < 7)
    {
        goto lbUsage;
    }

    u1AmpID = (UINT8)StrToInt(szArgv[1]);
    u1Bank = (UINT8)StrToInt(szArgv[2]);
    u1Addr = (UINT8)StrToInt(szArgv[3]);
    eAmpData.u1Data[0] = (UINT8)StrToInt(szArgv[4]);
    eAmpData.u1Data[1] = (UINT8)StrToInt(szArgv[5]);
    eAmpData.u1Data[2] = (UINT8)StrToInt(szArgv[6]);

    if (ADAC_DigiAmp_WriteRam((AUD_AMP_ID_T)u1AmpID, u1Bank, u1Addr, eAmpData, FALSE))
    {
        Printf("Write Addr 0x%x Succrss.\n", u1Addr);
    }

    return 0;
lbUsage:
    Printf("Usage: arr [AmpID] [Bank] [Addr] [Data1] [Data2] [Data3]\n");
    return -1;
}


#if 0
static INT32 _AdacCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    return 1;
}
#endif

static INT32 _InterDacChlSel(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u1DecId;
    UINT32 u4Channel;

    if (i4Argc < 3)
    {
        if ((i4Argc == 2) && (szArgv[1][0] == 'q'))
        {
            Printf("[InterDac Channel Setting]\n");
            _MT5365_DacChlSelQuery();
            return 1;
        }
        Printf("Usage: ch [DAC id] [channel] or ch q\n");
        Printf("[DAC id]  0:FIRST  1:SECOND  2: THIRD 3: 4TH\n");
        Printf("[channel #]      0:L/R,     1:LS/RS,   2:C/Subwoofer\n");
        Printf("                 3:bypass, 4:downmix,  5:AUX\n");
		return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u4Channel = (AUD_CH_T)StrToInt(szArgv[2]);

    _MT5365_DacChlSel(u1DecId, (AUD_CHL_DEF_T)u4Channel);

    return 1;
}
#ifdef XY_RAM_TEST
static INT32 _InterDacXYRamTest(INT32 i4Argc, const CHAR ** szArgv)
{
	BOOL fgxy;
	if (i4Argc < 2)
	{
	    Printf("Usage: X Y Ram select [s]\n");
	    Printf("[s]   0 : X Ram test\n");
	    Printf("      1 : Y Ram test\n");
	}
    fgxy = (StrToInt(szArgv[1]) == 1) ? TRUE : FALSE;
	_MT5365_XYRamTest(fgxy);
	return 1;
}
#endif
static INT32 _InterDacMute(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fMute;
    UINT8 u1ALRx;

    if (i4Argc < 3)
    {
        Printf("Usage: mute [ALRx] [flag]\n");
        Printf("[ALRx] 0:ALR0   1:ALR1   2:ALR2   3:ALR3   4:ALL ALR\n");
        Printf("[flag] 1 : mute   0 : unmute\n");
        return -1;
    }
    u1ALRx = (INT8)StrToInt(szArgv[1]);
    fMute = (StrToInt(szArgv[2]) == 1) ? TRUE : FALSE;

    if(u1ALRx < 4)
    {
        _MT5365_MuteALRx(u1ALRx,fMute);
    }
    else if(u1ALRx == 4)
    {
        _MT5365_Mute(0, fMute);
    }
    return 1;
}
static INT32 _InterPWMEnable(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgEnable;
    if(i4Argc < 2)
    {
        Printf("Usage: pwm [flag]\n");
        Printf("[flag] 1:enable   0:disable\n");
        return -1;
    }
    fgEnable = (StrToInt(szArgv[1]) == 1)? TRUE:FALSE;
    _MT5365_InterPWMEnable(fgEnable);

    return 1;
}
static INT32 _InterPWMGain(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 pgain;
    if(i4Argc < 2)
    {
        Printf("current gain value : 0x%06x\n\n",_MT5365_InterPWMGainGet());

        Printf("Usage: pg [value]\n");
        Printf("[value]: 0x000000 ~ 0xffffff (+:0x0~0x7fffff   -:0x100000~0xffffff)\n");
        return -1;
    }
    pgain = (INT32)(StrToInt(szArgv[1]));
    _MT5365_InterPWMGainSet(pgain);
    return 1;
}

#if 0
static INT32 _InterDacRamp(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DacId;
    BOOL fMute;
    if (i4Argc < 3)
    {
        Printf("Usage: ramp [DAC id] [flag]\n");
        Printf("[DAC id]    0:FIRST  1:SECOND  2:THIRD  3:All\n");
        Printf("[flag]      1 : Ramp_down internal dac0, dac1 and dac2\n");
        Printf("            0 : Ramp_up internal dac0, and dac1 and dac2\n");
    }

    u1DacId = (UINT8)StrToInt(szArgv[1]);
    fMute = (StrToInt(szArgv[2]) == 1) ? TRUE : FALSE;

    _MT5365_MuteCLK(u1DacId,fMute);

    return 1;
}
#endif
static INT32 _InterDacEnable(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fEnable;

	if (i4Argc < 2)
	{
	    Printf("Usage: enable [flag]\n");
	    Printf("[flag]      1 : enable internal dac\n");
	    Printf("            0 : disable internal dac\n");
	}

    fEnable = (StrToInt(szArgv[1]) == 1) ? TRUE : FALSE;
	//Laihui: remove it temp
	_MT5365_InterDacEnable(fEnable);
	//Printf("Internal DAC already enable.\n");
	return 1;
}

static INT32 _InterDacCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    _MT5365_Init();

    return 1;
}

static INT32 _InterDacLRInvert(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fMute;
    UINT16 u1DACId;

    UNUSED(fMute);
    UNUSED(u1DACId);

	if (i4Argc < 3)
	{
	    Printf("Usage: inv [DAC ID][flag]\n");
        Printf("[DAC ID]  0:FIRST  1:SECOND  2: THIRD  3:ALL\n");
	    Printf("[flag]      1 : invert internal dac\n");
	    Printf("            0 : no-invert internal dac\n");
	}

    u1DACId = (UINT8)StrToInt(szArgv[1]);

    fMute = (StrToInt(szArgv[2]) == 1) ? TRUE : FALSE;
	//_MT8297_LRInvert(u1DACId,fMute);
	return 1;
}

static INT32 _InterADCFmt(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Format;
    UINT8 u1DecId;

    UNUSED(u1DecId);
    UNUSED(u1Format);

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u1Format = (UINT8)StrToInt(szArgv[1]);

    //_MT8297_ADCFmt((DATA_FORMAT_T)u1Format, MCLK_256FS);

    return 1;

lbUsage:
    Printf("Usage: adf [format]\n");
    Printf("[format]     0:FORMAT_RJ  1:FORMAT_LJ 2:FORMAT_I2S \n");
    return -1;
}

static INT32 _InterDACFmt(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Format;
    UINT8 u1DecId;

    UNUSED(u1DecId);
    UNUSED(u1Format);

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u1Format = (UINT8)StrToInt(szArgv[1]);

    //_MT8297_DACFmt((DATA_FORMAT_T)u1Format, MCLK_256FS);

    return 1;

lbUsage:
    Printf("Usage: daf [format]\n");
    Printf("[format]     0:FORMAT_RJ  1:FORMAT_LJ 2:FORMAT_I2S \n");
    return -1;
}

#if 0 //def __MODEL_slt__
/****************************************************************************
** DSP SLT Command
****************************************************************************/
static INT32 _ADSPSLTInit(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    Printf("_ADSPSLTInit\n");
    ADSP_SLT_Init();
    return 1;
}

static INT32 _ADSPSLTTest(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1PatID;
    enum
    {
        ADSP_TEST_PATTERNNULL           = 0,
        ADSP_TEST_PATTERN1,
        ADSP_TEST_PATTERN2,
        ADSP_TEST_PATTERN3,
        ADSP_TEST_PATTERN4,
        ADSP_TEST_PATTERN5,
        ADSP_TEST_PATTERN6,
        ADSP_PATTERN_CNT                = ADSP_TEST_PATTERN6
    };


    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u1PatID = (UINT8)StrToInt(szArgv[1]);

    if ( (u1PatID <= ADSP_TEST_PATTERNNULL) || (u1PatID>ADSP_PATTERN_CNT) )
    {
        goto lbUsage;
    }

    ADSP_SLT_SetPatID(u1PatID);

    return 1;

lbUsage:
    Printf("Usage: t    [pattern no.]                       \n");
    Printf("[pattern no.]       1 DSP Internal SRAM Test    \n");
    Printf("                    2 DSP Instruction Test      \n");
    Printf("                    3 DSP DRAM Interface Test   \n");
    Printf("                    4 Extra Test #1             \n");   //Getbits
    Printf("                    5 Extra Test #2             \n");   //Putbits
    Printf("                    6 Extra Test #3             \n");   //CORDIC

    return -1;

}
#endif

/****************************************************************************
** dsp Command
****************************************************************************/

static INT32 _AdspDSPInit(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    Printf("_AdspDSPInit\n");

    vADSPTaskInit();

    return 1;

}

static INT32 _AdspDSPPC(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    Printf("dsp pc = %x\n", AUD_DspGetPc());

    return 1;
}

static INT32 _AdspDSPShareInfo(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1GroupId,u1GroupNumber;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u1GroupId = StrToInt(szArgv[1]);
    if (u1GroupId !=64)
    {
        DSP_ShareInforDump(u1GroupId);
    }
    else
    {
        for(u1GroupNumber=0;u1GroupNumber<64;u1GroupNumber++)
        {
            DSP_ShareInforDump(u1GroupNumber);
        }
    }
    return 1;

lbUsage:
    Printf("Usage: sh [group_id]\n");
    Printf("  group_id: 0~63 (64 will dump all group info\n");
    return -1;
}

// Light added for AVC
static INT32 _AdspDSPSharInfoWrite(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1GroupId, u1ByteAddr,u1Size;
    UINT32 u4Value;

    if(i4Argc!=5) {
        Printf("Usage: shw [group id] [byte address] [value] [size]\n");
        return -1;
    }
    u1GroupId = StrToInt(szArgv[1]);
    u1ByteAddr = StrToInt(szArgv[2]);
    u4Value = StrToInt(szArgv[3]);
    u1Size = StrToInt(szArgv[4]);
    switch(u1Size)
    {
        case 2:
           DSP_ShareInforWriteWord(u1GroupId*32+u1ByteAddr,u4Value);
       break;
       default:
       break;
    }

    return 1;
}

static INT32 _AdspDSPCommonDram(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4SrcAddr;
    UINT32 u4Len;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
    {
        goto lbUsage;
    }

    u4SrcAddr = StrToInt(szArgv[1]);
    u4Len = StrToInt(szArgv[2]);

    if (u4Len == 0)
    {
        goto lbUsage;
    }
    DSP_CommDramDump(u4SrcAddr, u4Len);

    return 1;

    lbUsage:
    Printf("Usage: cm [addr] [len]\n");
    return -1;
}

static INT32 _AdspDSPCommonDramWrite(INT32 i4Argc, const CHAR ** szArgv)
	{
    UINT32 u4SrcAddr;
    UINT32 u4Value;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL))
    {
        goto lbUsage;
    }
    u4SrcAddr = StrToInt(szArgv[1]);
    u4Value= StrToInt(szArgv[2]);
    Dsp_WriteDspCommDram(u4SrcAddr, u4Value);
    return 1;

    lbUsage:
    Printf("Usage: cmw [addr] [value]\n");

    return -1;
}

static INT32 _AdspDSPNormDram(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Page;
    UINT32 u4SrcAddr;
    UINT32 u4Len;

    if (i4Argc != 4)
    {
        goto lbUsage;
    }

    u1Page = StrToInt(szArgv[1]);
    u4SrcAddr = StrToInt(szArgv[2]);
    u4Len = StrToInt(szArgv[3]);

    if (u4Len == 0)
    {
        goto lbUsage;
    }
    DSP_NormDramDump(u1Page, u4SrcAddr, u4Len);

    return 1;

    lbUsage:
    Printf("Usage: norm [page] [addr] [len]\n");
    return -1;
}

static INT32 _AdspDSPCmptDram(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Page;
    UINT32 u4SrcAddr;
    UINT32 u4Len;

    if (i4Argc != 4)
    {
        goto lbUsage;
    }

    u1Page = StrToInt(szArgv[1]);
    u4SrcAddr = StrToInt(szArgv[2]);
    u4Len = StrToInt(szArgv[3]);

    if (u4Len == 0)
    {
        goto lbUsage;
    }
    DSP_CmptDramDump(u1Page, u4SrcAddr, u4Len);

    return 1;

    lbUsage:
    Printf("Usage: cmpt [page] [addr] [len]\n");
    return -1;
}

static INT32 _AdspCmdPtr(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4RpDec1, u4WpDec1;
    UINT32 u4RpDec2, u4WpDec2;
    UINT32 u4DataSZDec1, u4DataSZDec2;
#ifdef CC_MT5391_AUD_3_DECODER
    UINT32 u4RpDec3, u4WpDec3;
    UINT32 u4DataSZDec3;
#endif

    // Get read/write pointer
    AUD_GetRWPtr(AUD_DEC_MAIN, &u4RpDec1, &u4WpDec1, &u4DataSZDec1);
    AUD_GetRWPtr(AUD_DEC_AUX, &u4RpDec2, &u4WpDec2, &u4DataSZDec2);
#ifdef CC_MT5391_AUD_3_DECODER
    AUD_GetRWPtr(AUD_DEC_THIRD, &u4RpDec3, &u4WpDec3, &u4DataSZDec3);
#endif

    Printf("Decoder 1 rp,wp,size = [0x%08x, 0x%08x, %u]\n",u4RpDec1,u4WpDec1,u4DataSZDec1);
    Printf("Decoder 2 rp,wp,size = [0x%08x, 0x%08x, %u]\n",u4RpDec2,u4WpDec2,u4DataSZDec2);
#ifdef CC_MT5391_AUD_3_DECODER
    Printf("Decoder 3 rp,wp,size = [0x%08x, 0x%08x, %u]\n",u4RpDec3,u4WpDec3,u4DataSZDec3);
#endif
    return 1;
}

static INT32 _AdspReadSram(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Addr, u4Count = 1, i = 0;
    UINT32 u4Tmp;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u4Addr = (UINT32)StrToInt(szArgv[1]);
    if (i4Argc == 3)
    {
        u4Count= (UINT16)StrToInt(szArgv[2]);
    }

    while (i < u4Count)
    {
        if ((i%4) == 0)
        {
            #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
            u4Tmp = u4ReadDspSram32(u4Addr);
            Printf("\nSRAM (0x%04x) | %08x ", u4Addr, u4Tmp);
            #else
            u4Tmp = u4ReadDspSram(u4Addr);
            Printf("\nSRAM (0x%04x) | 0x%06x ", u4Addr, u4Tmp);
            #endif
        }
        else
        {
            #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
            u4Tmp = u4ReadDspSram32(u4Addr);
            Printf("%08x ", u4Tmp);
            #else
            u4Tmp = u4ReadDspSram(u4Addr);
            Printf("0x%06x ", u4Tmp);
            #endif
        }
        u4Addr++;
        i++;
    }

    return 1;

lbUsage:
    Printf("Usage: reads [addr] [cnt]\n");
    return -1;
}

static INT32 _AdspWriteSram(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4Addr;
    UINT32  u4Data;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u4Addr = (UINT32)StrToInt(szArgv[1]);
    u4Data = (UINT32)StrToInt(szArgv[2]);

    #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
    vWriteDspSram32(u4Addr, u4Data);
    #else
    vWriteDspSram(u4Addr, u4Data);
    #endif

    return 1;

lbUsage:
    Printf("Usage: writes [addr] [data]\n");
    return -1;
}

static INT32 _AdspUpdateReadPtr(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4Addr = 0;
    UINT8  u1DecId = 0;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u4Addr = (UINT32)StrToInt(szArgv[2]);
    u1DecId = (UINT8)StrToInt(szArgv[1]);

    fgDspReadPtrSet(u1DecId, u4Addr);

    return 1;

lbUsage:
    Printf("Usage: rpts [DecId] [Address]\n");
    return -1;
}

static INT32 _AdspDSPQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    AudShowDspStatus();

    return 1;
}

static INT32 _AdspDolbyQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    AudShowDolbyInfo();

    return 1;
}

static INT32 _AdspMTSQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8  u1DecId = 0;
    if (i4Argc == 2)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
    }
    AudShowMtsStatus(u1DecId);

    return 1;
}

#if 1///def CC_VORBIS_SUPPORT    ///CC_VORBIS_SUPPORT_Q
static INT32 _AdspVorbisQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8  u1DecId = 0;
    if (i4Argc == 2)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
    }
    AudShowVorbisStatus(u1DecId);

    return 1;
}
#endif

static INT32 _AudPreBuffer(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc == 2)
    {
        UINT16 u2Length = 0;
        u2Length = (UINT8)StrToInt(szArgv[1]);
        AUD_DspSetPcmPreBuffer(AUD_DEC_MAIN, u2Length);
        return 1;
    }
    else
    {
        Printf("pcm pre-buffer bank setting \n");
        Printf("Usage: pb [delay bank no.] \n");
        Printf("1 bank is around 5.5 ms \n");
        Printf("current bank no. =  %d \n\n\n",AUD_DspGetPcmPreBuffer(AUD_DEC_MAIN));
        return -1;
     }
}

static INT32 _AudCacheHitRate(INT32 i4Argc, const CHAR ** szArgv) //sunman_5365
{
    UINT8  u1Cache = 0;
    UNUSED(i4Argc);
    UNUSED(szArgv);


    //u1Cache = 0: All, 1: Main, 2: Second, 3: Third
    if (i4Argc == 2)
    {
         u1Cache = (UINT8)StrToInt(szArgv[1]);
         Printf("DSP cache %d hit rate = %d (o/oo)\n", u1Cache, AUD_GetDspCacheHitRate(u1Cache));
    }
    else
    {
        Printf("Usage : aud.dsp.cache [cache]\n\t 0: All, 1: Main, 2: Second, 3: Third \n");
    }

    return 1;
}

static INT32 _AdspDSPChlDump(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Sel = 0;

    if (i4Argc == 2)
    {
        u1Sel = (UINT8)StrToInt(szArgv[1]);
    }
    else
    {
        Printf("Usage: c [channal #] \n");
        Printf("[channal #]           0  L   \n");
        Printf("                      1  R   \n");
        Printf("                      2  Ls   \n");
        Printf("                      3  Rs   \n");
        Printf("                      4  C    \n");
        Printf("                      5  SW  \n");
        Printf("                      6  Bypass L  \n");
        Printf("                      7  Bypass R  \n");
        Printf("                      8  dmx L  \n");
        Printf("                      9  dmx R  \n");
        return -1;
     }

    AUD_ChlOutDump(u1Sel);

    return 1;
}

static INT32 _AdspDSPStcDiffBound(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1StcDiffFactorLo = 0;
    UINT8 u1StcDiffFactorHi = 0;
    UINT8 u1StcDiffFactorWs = 0;

    if (i4Argc == 4)
    {
        u1StcDiffFactorLo = (UINT8)StrToInt(szArgv[1]);
        u1StcDiffFactorHi = (UINT8)StrToInt(szArgv[2]);
        u1StcDiffFactorWs = (UINT8)StrToInt(szArgv[3]);

        AUD_AudioSetStcDiffBound(AUD_DEC_MAIN, u1StcDiffFactorLo, u1StcDiffFactorHi, u1StcDiffFactorWs);
        printf("Set STC difference factor (lo,hi,worst) = (%d,%d,%d)\n", u1StcDiffFactorLo, u1StcDiffFactorHi, u1StcDiffFactorWs);
    }
    else
    {
        Printf("Usage: stcbd [factor Lo] [factor Hi] [factor Ws]\n");
        Printf("    factor : 0 ~ 255   \n");
        Printf("The STC difference bound would be x msec * (1+factor)\n");
        return -1;
     }

    return 1;
}


static INT32 _AudCmdUopBassMngEnable(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Enable;

    if (i4Argc < 3)
    {
        if ((i4Argc == 2) && (szArgv[1][0] == 'q'))
        {
            Printf("[bass management setting]\n");
            Printf("  DEC1: %d\n", AUD_DspGetBassMngEnable(AUD_DEC_MAIN));
            Printf("  DEC2: %d\n", AUD_DspGetBassMngEnable(AUD_DEC_AUX));
            return 1;
        }
        Printf("Usage: bs [decoder id] [on/off] or bs q\n");
        Printf("[decoder id]     0:FIRST      1:SECOND\n");
        Printf("[on/off]         0:OFF        1:ON \n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Enable = (UINT8)StrToInt(szArgv[2]);

    AUD_DspBassMngEnalbe(u1DecId, (BOOL)u1Enable);
    return 1;
}

#ifdef CC_AUD_FIR_SUPPORT
static INT32 _AudCmdUopFIREnable(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Enable;
    
    if (i4Argc < 2)
    {
        Printf("Usage: flag [mode] \n");
        Printf("[mode]         0:OFF 1:Mode1 2:Mode2 3:Mode3... \n");
        return -1;    
    }

    u1Enable = (UINT8)StrToInt(szArgv[1]);
   
    AUD_DspSetFIREnable(AUD_DEC_MAIN, u1Enable);
    return 1;
}

static INT32 _AudCmdUopGetPhyAddr(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    UINT32 u4DramAdrIdx;
    UINT32 u4VirAddr;
    UINT32 u4PhyAddr;

	u4DramAdrIdx = DSP_CommDramRead(0xAB5) + 0x1;//the address of coneq dram table

	u4VirAddr = DSP_CommDramAddr(u4DramAdrIdx);

    u4PhyAddr = PHYSICAL(u4VirAddr);

    Printf("CONEQ Table dump in address ==> 0x%08x\n", u4PhyAddr);
    return 0;
}

#endif

/******************************************************************************
* Function      : _AudCmdUop*
* Description   : accept UOP from RS232 command line
* Parameter     : None
* Return        : None
*******************************************************************************/
static INT32 _AudCmdUopPL2Init(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2PL2cfg;
    UINT16 u2PL2Mode;
    if (i4Argc < 3)
    {
        Printf("Usage: init [config] [mode]\n");
        Printf("[config:16 bit] value for W_PLIICONFIG\n");
        Printf("[mode:16 bit] value for W_PLIIMODE\n");
        return -1;
    }

    u2PL2cfg =  (UINT16)StrToInt(szArgv[1]);
    u2PL2Mode = (UINT16)StrToInt(szArgv[2]);

    AUD_DspPL2ConfigInit(u2PL2cfg,u2PL2Mode);
    return 1;
};
static INT32 _AudCmdUopPL2Switch(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2PL2Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: switch [mode]\n");
        Printf("[mode] 0:ON 1:OFF 2:AUTO\n");
        return -1;
    }

    u2PL2Mode =  (UINT16)StrToInt(szArgv[1]);

    AUD_DspPL2Config(PL2CTRL_SWITCH,u2PL2Mode);

    return 1;
};
static INT32 _AudCmdUopPL2Mode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2PL2Mode;
    UINT16 u2PL2cfg;
    if (i4Argc < 3)
    {
        Printf("Usage: mode [config] [manual mode]\n");
        Printf("[config]      0: MANUAL            1:AUTO \n");
        Printf("[manual mode] 0: PROLGIC EMULATION 1:VIRTUAL MODE\n");
        Printf("              2: MUSIC MODE        3:MOVIE MODE\n");
        Printf("              4: MATRIX MODE       7:CUSTOM MODE\n");
        return -1;
    }
    u2PL2cfg = (UINT16)StrToInt(szArgv[1]);
    u2PL2Mode =  (UINT16)StrToInt(szArgv[2]);

    u2PL2Mode = (u2PL2Mode <<1) | (u2PL2cfg & 0x1);
    AUD_DspPL2Config(PL2CTRL_MODE,u2PL2Mode);

    return 1;
};

static INT32 _AudCmdUopPL2Pano(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2PL2cfg;
    if (i4Argc < 2)
    {
        Printf("Usage: pano [config]\n");
        Printf("[config] 0: ON  1:OFF \n");
        return -1;
    }
    u2PL2cfg = (UINT16)StrToInt(szArgv[1]);
    AUD_DspPL2Config(PL2CTRL_PANORAMA,u2PL2cfg);

    return 1;
};
static INT32 _AudCmdUopPL2Dim(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2PL2cfg;
    if (i4Argc < 2)
    {
        Printf("Usage: dim [dimension control]\n");
        Printf("[dimension control] 0:-3,1:-2,2:-1,3:0,4:1,5:2,6:3 \n");
        return -1;
    }
    u2PL2cfg = (UINT16)StrToInt(szArgv[1]);
    AUD_DspPL2Config(PL2CTRL_DIMENSION,u2PL2cfg);

    return 1;
};
static INT32 _AudCmdUopPL2CWidth(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2PL2cfg;
    if (i4Argc < 2)
    {
        Printf("Usage: cw [center width spread value]\n");
        Printf("[center width spread value] 0~7 \n");
        return -1;
    }
    u2PL2cfg = (UINT16)StrToInt(szArgv[1]);
    AUD_DspPL2Config(PL2CTRL_C_WIDTH,u2PL2cfg);

    return 1;
};
static INT32 _AudCmdUopSpeakerConfig(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2DecId;
    UINT32 u4SpkCfg;
    if (i4Argc < 3)
    {
        if ((i4Argc == 2) && (szArgv[1][0] == 'q'))
        {
            Printf("[Speaker Config]\n");
            Printf("  #DEC1: 0x%08x\n", AUD_DspGetSpeakerConfig(AUD_DEC_MAIN));
            Printf("  #DEC2: 0x%08x\n", AUD_DspGetSpeakerConfig(AUD_DEC_AUX));
        }
        else
        {
            Printf("Usage: spkcfg [decoder id] [speaker config] or spkcfg q\n");
            Printf("[decoder id]     0:FIRST                 1:SECOND \n");
            Printf("[speaker config] 32 bits                          \n");
        }
        Printf("bit 0 ~ 2:\n");
        Printf("  b000: 2/0 (LT/RT downmix: prologic compatible)  \n");
        Printf("  b001: 1/0\n");
        Printf("  b010: 2/0 (LO/RO)\n");
        Printf("  b011: 3/0\n");
        Printf("  b100: 2/1\n");
        Printf("  b101: 3/1\n");
        Printf("  b110: 2/2\n");
        Printf("  b111: 3/2\n");
        Printf("bit 5: Subwoofer Channel present(1)/absent(0)\n");
        Printf("bit 12: Center Channel large(1)/small(0)\n");
        Printf("bit 13: Left Channel large(1)/small(0)\n");
        Printf("bit 14: Right Channel large(1)/small(0)\n");
        Printf("bit 15: Left Surround Channel large(1)/small(0)\n");
        Printf("bit 16: Right Surround Channel large(1)/small(0)\n");
        Printf("bit 19: Downmix CH9 Channel large(1)/small(0)\n");
        Printf("bit 20: Downmix CH10 Channel large(1)/small(0)\n");

        return -1;
    }
    u2DecId = (UINT16)StrToInt(szArgv[1]);
    u4SpkCfg = (UINT32)StrToInt(szArgv[2]);

    AUD_DspSpeakerConfig(u4SpkCfg, u2DecId);

    return 1;
}

static INT32 _AudCmdUopSpeakerLargeSmallConfig(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1FrontPair;
    UINT8 u1Center;
    UINT8 u1Surround;
    UINT8 u1CH910;

    if (i4Argc < 6)
    {
        if ((i4Argc == 2) && (szArgv[1][0]=='q'))
        {
            AUD_DspGetSpeakerLargeSmallConfig(AUD_DEC_MAIN, &u1FrontPair, &u1Center, &u1Surround, &u1CH910);
            Printf("[Speaker L/S Setting]\n");
            Printf("  Front:    %s\n", u1FrontPair ? "Large" : "Small");
            Printf("  Center:   %s\n", u1Center ? "Large" : "Small");
            Printf("  Surround: %s\n", u1Surround ? "Large" : "Small");
            Printf("  CH9/10:   %s\n", u1CH910 ? "Large" : "Small");
            return 1;
        }
        Printf("Usage: ls [decoder id] [FrontPair] [Center] [Surround] [CH9/10] or ls q\n");
        Printf("[decoder id] 0:FIRST  1:SECOND \n");
        Printf("[FrontPair]  0:SMALL  1:LARGE \n");
        Printf("[Center]     0:SMALL  1:LARGE \n");
        Printf("[Surround]   0:SMALL  1:LARGE \n");
        Printf("[CH9/10]     0:SMALL  1:LARGE \n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1FrontPair = (UINT8)StrToInt(szArgv[2]);
    u1Center = (UINT8)StrToInt(szArgv[3]);
    u1Surround = (UINT8)StrToInt(szArgv[4]);
    u1CH910 = (UINT8)StrToInt(szArgv[5]);

    //CLI_Parser("aud.uop.spkuop.bs 0 1");  // turn on bass management
    AUD_DspSpeakerLargeSmallConfig(u1DecId, u1FrontPair, u1Center, u1Surround,u1CH910);

    return 1;
}

static INT32 _AudCmdUopCutFreq(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT32 u4Freq;

    if (i4Argc < 3)
    {
        if ((i4Argc == 2) && (szArgv[1][0] == 'q'))
        {
            Printf("Cufoff Frequecy is %d\n", AUD_DspSpkSizeQry(AUD_DEC_MAIN));
            return 1;
        }
        Printf("Usage: cf [decoder id] ([cutoff freq]) or cf q\n");
        Printf("[decoder id]   0:FIRST  1:SECOND \n");
        Printf("[cutoff freq]  set frequency in xxx Hz \n");
        Printf("               70/100/200/300  Hz         \n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u4Freq = StrToInt(szArgv[2]);
    AUD_DspSpkSizeCfg(u1DecId, u4Freq);
    return 1;
}


static INT32 _AudCmdUopSpeakerOutputConfig(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Mode;

    if (i4Argc < 3)
    {
        if ((i4Argc == 2) && (szArgv[1][0] == 'q'))
        {
            Printf("[SPKCFG CH Setting]\n");
            Printf("  DEC1: %d\n", AUD_DspGetSpeakerOutputConfig(AUD_DEC_MAIN));
            Printf("  DEC2: %d\n", AUD_DspGetSpeakerOutputConfig(AUD_DEC_AUX));
            return 1;
        }
        Printf("Usage: ch [decoder id] [mode] or ch q\n");
        Printf("[decoder id]   0:FIRST  1:SECOND \n");
        Printf("[mode]         0: 2/0 (LT/RT downmix: prologic compatible)  \n");
        Printf("               1: 1/0\n");
        Printf("               2: 2/0 (LO/RO)\n");
        Printf("               3: 3/0\n");
        Printf("               4: 2/1\n");
        Printf("               5: 3/1\n");
        Printf("               6: 2/2\n");
        Printf("               7: 3/2\n");
        return -1;
    }
    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Mode = (UINT8)StrToInt(szArgv[2]);

    AUD_DspSpeakerOutputConfig(u1DecId, u1Mode);

    return 1;
}

static INT32 _AudCmdUopSpeakerSubwooferEnable(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Mode;

    if (i4Argc < 3)
    {
        if ((i4Argc == 2) && (szArgv[1][0]=='q'))
        {
            Printf("[Subwoofer Setting]\n");
            Printf("  DEC1: %d\n", AUD_DspGetSpeakerSubwooferEnable(AUD_DEC_MAIN));
            Printf("  DEC2: %d\n", AUD_DspGetSpeakerSubwooferEnable(AUD_DEC_AUX));
            return 1;
        }
        Printf("Usage: sw [decoder id] [mode] or sw q\n");
        Printf("[decoder id]   0:FIRST  1:SECOND \n");
        Printf("[mode]         0:OFF    1:ON \n");
        return -1;
    }
    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Mode = (UINT8)StrToInt(szArgv[2]);

    AUD_DspSpeakerSubwooferEnable(u1DecId, u1Mode);

    return 1;
}

#if 1 //channel dealy for NorDig
static INT32 _AudCmdUopChannelDelay(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u2ChanNum;
    UINT16 u2Delay;
    UINT16 u2DecId;
    if (i4Argc < 4 || (UINT16)StrToInt(szArgv[3]) > 25)
    {
        if ((i4Argc == 2) && (szArgv[1][0]=='q'))
        {
            AUD_CH_T ch;
            Printf("[Delay Setting] %s\n", AUD_DspGetDelayEnable(AUD_DEC_MAIN) ? "Enable" : "Disable");
            for (ch=AUD_CH_FRONT_LEFT;ch<=AUD_CH_AUX_FRONT_RIGHT;ch++)
            {
                if (ch == AUD_CH_FRONT_LEFT)
                {
                    Printf("  #DEC1\n");
                }
                else if (ch == AUD_CH_AUX_FRONT_LEFT)
                {
                    Printf("  #DEC2\n");
                }
                Printf("    CH%d: %d\n", ch, AUD_DspGetChannelDelay(ch));
            }
            return 1;
        }
        Printf("Usage: delaycfg [decoder id] [channel #] [channel delay] or delaycfg q\n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[channel #]      0(L), 1(R), 2(LS), 3(RS), 4(C), 5(SUB), 6(Bypass L), 7(Bypass R)\n");
        Printf("8(Downmix L), 9(Downmix R), 10(AUX L), 11(AUX R), 13(All), 14(LFE)\n");
        //Printf("[channel delay]  0.05 m/unit\n");
        Printf("[channel delay]  10ms /unit (0~25 unit)\n");
        return -1;
    }

    u2DecId = (UINT16)StrToInt(szArgv[1]);
    u2ChanNum = (UINT32)StrToInt(szArgv[2]);
    //u2Delay = (UINT32)StrToInt(szArgv[3]);
    u2Delay = (UINT32)StrToInt(szArgv[3])*68;//sunman

    AUD_DspChannelDelay(u2Delay,(AUD_CH_T)u2ChanNum,u2DecId);
    return 1;
}
#else
static INT32 _AudCmdUopChannelDelay(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u2ChanNum;
    UINT16 u2Delay;
    UINT16 u2DecId;
    if (i4Argc < 4)
    {
        Printf("Usage: delaycfg [decoder id] [channel #] [channel delay]\n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[channel #]      0(L), 1(R), 2(LS), 3(RS), 4(C), 5(SUB), 6(Bypass L), 7(Bypass R)\n");
        Printf("8(Downmix L), 9(Downmix R), 10(AUD L), 11(AUD R), 13(All), 14(LFE)\n");
        Printf("[channel delay]  0.05 m/unit\n");
        return -1;
    }

    u2DecId = (UINT16)StrToInt(szArgv[1]);
    u2ChanNum = (UINT32)StrToInt(szArgv[2]);
    u2Delay = (UINT32)StrToInt(szArgv[3]);

    AUD_DspChannelDelay(u2Delay,(AUD_CH_T)u2ChanNum,u2DecId);
    return 1;
}
#endif

static INT32 _AudCmdUopChVolume(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u1DecId;
    UINT32 u4Volume;
    AUD_CH_T eChannel;

    if (i4Argc < 4)
    {
        if ((i4Argc == 2) && (szArgv[1][0]=='q'))
        {
            Printf("[Channel Volume Setting] (%s)\n", AUD_DspGetChannelVolumeEnable(AUD_DEC_MAIN) ? "Enable" : "Disable");
            Printf(" #DEC1\n");
            for (u1DecId = 0;u1DecId<10;u1DecId++)
                Printf("  CH%d: %d(0x%x)\n", u1DecId, AUD_DspGetChannelVolume(AUD_DEC_MAIN,(AUD_CH_T)u1DecId), AUD_DspGetFineTuneVolume(AUD_DEC_MAIN, (AUD_CH_T)u1DecId));
            Printf(" #DEC2\n");
            for (u1DecId = 0;u1DecId<2;u1DecId++)
                Printf("  CH%d: %d(0x%x)\n", u1DecId, AUD_DspGetChannelVolume(AUD_DEC_AUX,(AUD_CH_T)u1DecId), AUD_DspGetFineTuneVolume(AUD_DEC_AUX, (AUD_CH_T)u1DecId));
            return 1;
        }

        Printf("Usage: cv [decoder id] [channel] [volume] or cv q \n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[channel #]      0:L, 1:R, 2:LS, 3:RS, 4:C, 5:SUBWOOFER\n");
        Printf("                 6:BYPASS L, 7: BYPASS R, 8: DMX L, 9: DMX R\n");
        Printf("[volume]         0 ~ 100\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    eChannel = (AUD_CH_T)StrToInt(szArgv[2]);
    u4Volume = (UINT8)StrToInt(szArgv[3]);

    AUD_DspChannelVolume(u1DecId, eChannel, u4Volume);
    return 1;
}
#ifdef CC_AUD_LINEOUT_VOL_OFFSET
static INT32 _AudCmdUopLineOutVolOffset(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u1DecId;
    UINT8 u1Volume;

    if (i4Argc < 3)
    {
        Printf("Usage: lovo [decoder id] [volumeoffset] \n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[volume]         0 ~ 100\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Volume = (UINT8)StrToInt(szArgv[2]);

    AUD_DspLineOutVolOffset(u1DecId,u1Volume);
    return 1;
}
#endif
#ifdef CC_AUD_VOLUME_OFFSET
static INT32 _AudCmdUopVolumeOffset(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u1DecId;
    UINT8 u1Idx;

    if (i4Argc < 3)
    {
        Printf("Usage: vo [decoder id] [volumeoffset_idx] \n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[Idx]         0 ~ 12\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Idx = (UINT8)StrToInt(szArgv[2]);

    AUD_DspVolumeOffset(u1DecId,u1Idx);
    return 1;

}
#endif
static INT32 _AudCmdUopChVolumeGain(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u1DecId;
    INT32  i4Gain;
    AUD_CH_T eChannel;

    if (i4Argc < 4)
    {
        if ((i4Argc == 2) && (szArgv[1][0] == 'q'))
        {
            Printf("[Extra Channel Volume Gain Setting]\n");
            Printf(" #DEC1\n");
            for (u1DecId = 0;u1DecId<10;u1DecId++)
                Printf("  CH%d: %d\n", u1DecId, AUD_DspGetChannelVolGain(AUD_DEC_MAIN,(AUD_CH_T)u1DecId));
            Printf(" #DEC2\n");
            for (u1DecId = 0;u1DecId<2;u1DecId++)
                Printf("  CH%d: %d\n", u1DecId, AUD_DspGetChannelVolGain(AUD_DEC_AUX,(AUD_CH_T)u1DecId));
            return 1;
        }

        Printf("Usage: cvg [decoder id] [channel] [gain] or cvg q\n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[channel #]      0:L, 1:R, 2:LS, 3:RS, 4:C, 5:SUBWOOFER,\n");
        Printf("                 6:Bypass L, 7: Bypass R, 8: Downmix L, 9: Dowmix R\n");
        Printf("[gain]           -128 ~ 96,  every 0.5dB per step \n");
        printf("                 -128:  -64   dB\n");
        printf("                 -127:  -63.5 dB\n");
        printf("                 -126:  -63   dB\n");
        printf("                     .        . \n");
        printf("                     .        . \n");
        printf("                     .        . \n");
        printf("                    0:    0   dB\n");
        printf("                    1:    0.5 dB\n");
        printf("                     .        . \n");
        printf("                     .        . \n");
	    printf("                   96:   48   dB\n");

        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    eChannel = (AUD_CH_T)StrToInt(szArgv[2]);
    i4Gain = (INT32)i4StrToInt(szArgv[3]);

    AUD_DspChannelVolGain(u1DecId, eChannel, i4Gain);
    return 1;
}

#ifdef CC_AUD_LINEOUT_VOL_MODE
static INT32 _AudCmdUopLinOutVolume(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Mode;

    if (i4Argc < 3)
    {
        Printf("Usage: lov [decoder id] [mode] [level] \n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[mode]   0:fixed      1:variable \n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Mode = (UINT8)StrToInt(szArgv[2]);

    AUD_DspSetLineOutVolumeMode(u1DecId, (UINT8)u1Mode);
    return 1;

}
#endif

static INT32 _AudCmdUopSrcVolume(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u1DecId;
    INT16 i2Volume;
    AUD_DEC_STREAM_FROM_T eStrmSrc;

    if (i4Argc < 4)
    {
        Printf("Usage: sv [decoder id] [source] [value] \n");
        Printf("[decoder id] 0:FIRST 1:SECOND 2:THIRD 3: 4TH\n");
        Printf("[source #]      0:OTHERS, 1:DIGITAL, 2:ANALOG, 3:SPDIF \n");
        printf("                4:LINE_IN, 5:HDMI, 6:MEMORY 7: BUFAGT 8: FEEDER 9: MM\n");
        Printf("[volume]        -128~36     every 0.5dB per step \n");
        printf("                  -128:  -64    dB\n");
        printf("                  -127:  -63.5 dB\n");
        printf("                  -126:  -63    dB\n");
        printf("                     .       . \n");
        printf("                     .       . \n");
        printf("                     .       . \n");
        printf("                     0:    0     dB\n");
        printf("                     .       . \n");
        printf("                     .       . \n");
        printf("                     .       . \n");
        printf("                    36:   18   dB\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    eStrmSrc = (AUD_DEC_STREAM_FROM_T)StrToInt(szArgv[2]);
    i2Volume = (INT16)i4StrToInt(szArgv[3]);

    AUD_DspSetSrcVol(u1DecId, eStrmSrc, i2Volume);
    return 1;
}

#ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
static INT32 _AudCmdUopSrcLinOutVolume(INT32 i4Argc,const CHAR** szArgv)
	{
		UINT16 u1DecId;
		UINT32 u4Volume;
		AUD_DEC_STREAM_FROM_T eStrmSrc;

		if (i4Argc < 4)
		{
			Printf("Usage: slov [decoder id] [source] [value] \n");
#ifdef CC_MT5391_AUD_3_DECODER
			Printf("[decoder id]	0:FIRST 	1:SECOND \n");
#else
			Printf("[decoder id]	0:FIRST 	1:SECOND	 2:THIRD \n");
#endif
			Printf("[source #]		0:OTHERS, 1:DIGITAL, 2:ANALOG, 3:SPDIF \n");
			printf("				4:LINE_IN, 5:HDMI, 6:MEMORY \n");
			Printf("[volume]		0~164	  every 0.5dB per step \n");
			printf("					 0:  -64	dB\n");
			printf("					 1:  -63.5 dB\n");
			printf("					 2:  -63	dB\n");
			printf("					 .		 . \n");
			printf("					 .		 . \n");
			printf("					 .		 . \n");
			printf("				  128:	  0 	dB\n");
			printf("					 .		 . \n");
			printf("					 .		 . \n");
			printf("					 .		 . \n");
			printf("				  164:	  18   dB\n");
			return -1;
		}

		u1DecId = (UINT8)StrToInt(szArgv[1]);
		eStrmSrc = (AUD_DEC_STREAM_FROM_T)StrToInt(szArgv[2]);
		u4Volume = (UINT8)StrToInt(szArgv[3]);

		AUD_DspSrcLinoutVol(u1DecId, eStrmSrc, (INT16)(u4Volume-128));
		return 1;
	}
#endif

static INT32 _AudCmdUopGetSrcVolume(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u1DecId;
    UINT32 u4Volume;
    UINT8 i;

    const CHAR* strStreamSrc[AUD_STREAM_FROM_NUM]= {"Others","DTuner","ATuner","SPDIF","Line_In","HDMI","Memory","Reserve", "Feeder","MultiMedia"};

    u1DecId = 0;

    for (i=0; i<AUD_STREAM_FROM_NUM; i++)
    {
        u4Volume = AUD_DspGetSrcVol(u1DecId, (AUD_DEC_STREAM_FROM_T)i);
        Printf("%s : %d\n", strStreamSrc[i] ,u4Volume);
    }

     return 1;
}

static INT32 _AudCmdUopAVC(INT32 i4Argc,const CHAR** szArgv)
{
#if 0
    UINT16 u2AVCflag;
    if (i4Argc != 2)
    {
        Printf("Usage: avc [en] \n");
        Printf("[en] en/dis AVC, 1 to enable AVC, 0 to disable AVC \n");
        return -1;
    }
    u2AVCflag = (UINT8)StrToInt(szArgv[1]);
    if(u2AVCflag==1)
        AUD_DspAVCEnable(0,TRUE);
    else if(u2AVCflag==0)
        AUD_DspAVCEnable(0,FALSE);
#else
    UINT16 u2Type;
    UINT16 u2Value;
    const CHAR *Argv;
    UINT16 u2CurrVal[8];
    UINT8 i = 0;

    for (i=0; i<8; i++)
    {
        AUD_DspGetAVCPara(i,&u2CurrVal[i],0);
    }
    if (u2CurrVal[0]==3)
    {
        u2CurrVal[0] = 1;
    }

    if (i4Argc != 3)
    {
_AVC_CLI_SHOW_ERR_MSG:
        Printf("Usage: avc [type] [val] \n");
        Printf("[type] 0, val: 1: Enable, 0: Disable ");
        Printf(" [Current : 0x%x]\n", u2CurrVal[0]);
        Printf("[type] 1, val: Target Level ");
        Printf(" [Current : 0x%x (%d)]\n", u2CurrVal[1], (INT16)u2CurrVal[1]);
        //Printf("[type] 2, val: Silence Level\n");
        //Printf(" [Current : 0x%x]\n", u2CurrVal[2]);
        Printf("[type] 3, val: Max. Gain Up ");
        Printf(" [Current : 0x%x]\n", u2CurrVal[3]);
        //Printf("[type] 4, val: Max. Gain Down ");
        //Printf(" [Current : 0x%x (%d)]\n", u2CurrVal[4], (INT16)u2CurrVal[4]);
        Printf("[type] 5, val: Adjust Rate ");
        Printf(" [Current : 0x%x]\n", u2CurrVal[5]);
        Printf("[type] 6, val: UI Adjust Rate ");
        Printf(" [Current : 0x%x]\n", u2CurrVal[6]);
        //Printf("[type] 7, val: Attack Threshold ");
        //Printf(" [Current : 0x%x]\n", u2CurrVal[7]);
        return -1;
    }
    u2Type = (UINT16)StrToInt(szArgv[1]);
    Argv = szArgv[2];
    if (Argv[0] == '-')
    {
        u2Value = (UINT16)StrToInt(&Argv[1]);
        u2Value = (UINT16)0xFFFF - (u2Value-1);
    }
    else
    {
        u2Value = (UINT16)StrToInt(szArgv[2]);
    }
    switch(u2Type)
    {
    case 0:     //En/Dis
    case 1:     //Target Lvl
    case 2:     //Silence Lvl
    case 3:     //Max Gain Up
    case 4:     //Max Gain Down
    case 5:     //Adj Rate
    case 6:     //UI Adj Rate
    case 7:     //Attact threshold
        AUD_DspChangeAVCPara(u2Type, (INT16)u2Value, FALSE);
        break;
    default:
        goto _AVC_CLI_SHOW_ERR_MSG;
    }
#endif
    return 1;
}

static INT32 _AudCmdUopMonoDownmix(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1kmode;

    if (i4Argc == 2)
    {
        u1DecId = AUD_DEC_MAIN;
        u1kmode = (UINT8)StrToInt(szArgv[1]);
    }
    else if (i4Argc == 3)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
        u1kmode = (UINT8)StrToInt(szArgv[2]);
    }
    else
    {
        Printf("Usage: md {[decoder id]} [en] \n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[en]             0:Disable  1:Enable \n");
        return -1;
    }

    AUD_DspMonoDownMix(u1DecId, (BOOL)u1kmode);
    return 1;
}

static INT32 _AudCmdUopKaraokeMixRatio(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1DecId;
    UINT32 u4kratio;

    if (i4Argc == 2)
    {
        u1DecId = AUD_DEC_MAIN;
        u4kratio = (UINT8)StrToInt(szArgv[1]);
    }
    else if (i4Argc == 3)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
        u4kratio = (UINT32)StrToInt(szArgv[2]);
    }
    else
    {
        Printf("Usage: kr {[decoder id]} [ratio] \n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[ratio]             0x0 ~ 0x800000 (for R channel)\n");
        return -1;
    }

    AUD_DspSetKaraokeMixRatio(u1DecId, u4kratio);
    return 1;
}

// 2006/08/28, created for tuning volume table
static INT32 _AudCmdUopFineVolume(INT32 i4Argc,const CHAR** szArgv)
{
    UINT32 u4Volume;
    UINT8 u1DecId;

    if (i4Argc == 2)
    {
        if (szArgv[1][0] == 'q')
        {
            Printf("[Fine Volume Setting]\n");
            Printf("  DEC1: 0x%08x\n", AUD_DspGetFineTuneVolume(AUD_DEC_MAIN,AUD_CH_ALL));
            Printf("  DEC2: 0x%08x\n", AUD_DspGetFineTuneVolume(AUD_DEC_AUX,AUD_CH_ALL));
            Printf("  DEC3: 0x%08x\n", AUD_DspGetFineTuneVolume(AUD_DEC_THIRD,AUD_CH_ALL));
            Printf("  DEC4: 0x%08x\n", AUD_DspGetFineTuneVolume(AUD_DEC_4TH, AUD_CH_ALL));
            return 1;
        }
        u1DecId = AUD_DEC_MAIN;
        u4Volume = StrToInt(szArgv[1]);
    }
    else if (i4Argc == 3)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
        u4Volume = StrToInt(szArgv[2]);
    }
    else
    {
        Printf("Usage: fv {[decoder id]} [value] or fv q\n");
        Printf("[decoder id]     0:FIRST 1:SECOND 2:THIRD 3: 4TH\n");
        Printf("[volume]         0x0 ~ 0x20000\n");
        Printf("                 0x0: mute, 0x20000: full scale (0 dB)\n");
        return -1;
    }
    // Check if it is valid volume value
    if(u4Volume > 0x20000)
    {
        Printf("Warning: volume[0x%08x] exceeds full scale 0 dB [0x20000]!\n",u4Volume);
    }

    AUD_DspFineTuneVolume(u1DecId, AUD_CH_ALL, u4Volume);

    return 1;
}

static INT32 _AudCmdUopVolume(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u1DecId;
    UINT32 u4Volume;

    if (i4Argc == 2)
    {
        if (szArgv[1][0] == 'q')
        {
            Printf("[Volume Setting]\n");
            Printf("  DEC1: %d\n", AUD_DspGetMasterVolume(AUD_DEC_MAIN));
            Printf("  DEC2: %d\n", AUD_DspGetMasterVolume(AUD_DEC_AUX));
            Printf("  DEC3: %d\n", AUD_DspGetMasterVolume(AUD_DEC_THIRD));
            Printf("  DEC4: %d\n", AUD_DspGetMasterVolume(AUD_DEC_4TH));
            return 1;
        }
        u1DecId = AUD_DEC_MAIN;
        u4Volume = (UINT8)StrToInt(szArgv[1]);
    }
    else if (i4Argc == 3)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
        u4Volume = (UINT8)StrToInt(szArgv[2]);
    }
    else
    {
        Printf("Usage: v {[decoder id]} [volume] or v q\n");
#if defined(CC_AUD_4_DECODER_SUPPORT)
        Printf("[decoder id]     0:FIRST 1:SECOND 2:THIRD 3: 4TH\n");
#elif defined(CC_MT5391_AUD_3_DECODER)
        Printf("[decoder id]     0:FIRST 1:SECOND 2:THIRD\n");
#else
        Printf("[decoder id]     0:FIRST 1:SECOND\n");
#endif
        Printf("[volume]         0 ~ 100\n");
        return -1;
    }

    AUD_DspMasterVolume(u1DecId, u4Volume);
    return 1;
}

#if 0
static UINT32 _StrToVol(const CHAR* pszStr)
{
    //Andrew wen 07/8/20 : Dynamic volume curve adjusetment.
    UINT8 u1Idx;
    UINT8 u1Len;
    UINT8 u1LenLeft;
    UINT32 u4ChkPnt = 0;
    UINT32 u4TmpVal = 0;

    if (pszStr == NULL)
    {
        return 0;
    }

    u1Len = x_strlen(pszStr);

    for (u1Idx=0; u1Idx<u1Len; u1Idx++)
    {
        if (pszStr[u1Idx] == '.')
            break;
    }

    if (u1Idx>0)
    {
        u4ChkPnt = (StrToInt(pszStr) * 10000);   //precision is 10^-4
    }

    u1LenLeft = u1Len - (u1Idx+1);
    u4TmpVal = StrToInt(&pszStr[u1Idx+1]);
    for (u1Idx=0; u1Idx<(4-u1LenLeft); u1Idx++)
    {
        u4TmpVal *= 10;
    }
    u4ChkPnt += u4TmpVal;

    //Printf("u4ChkPnt = %d\n", u4ChkPnt);
    return u4ChkPnt;
}
#endif //if 0
static INT32 _AudCmdUopVolumeCurve(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8  u1Idx;
    UINT32 u4VolTabChkPnt[VOL_TABLE_CHK_PNT];
    //UINT64 u4TmpVar = 0;

    if ( i4Argc != (VOL_TABLE_CHK_PNT+1) )
    {
        Printf("Usage:  vc  [Step 0] ~ [Step 6] \n");
        Printf("[Step n]     0xXXXXX (e.x. 0x20000) \n");
        return -1;
    }
    #if 0
    for (u1Idx=0; u1Idx < VOL_TABLE_CHK_PNT; u1Idx++)
    {
        u4TmpVar = (UINT64)(_StrToVol(szArgv[u1Idx+1]))<<17; //0x20000 = 2^17
        if (u4TmpVar==0)
        {
            u4VolTabChkPnt[u1Idx] = 0;
        }
        else
        {   //normalize u4VolTabChkPnt[i]  and  times 0x20000
            u4VolTabChkPnt[u1Idx] = (UINT32)(u4TmpVar / _StrToVol(szArgv[VOL_TABLE_CHK_PNT]));
        }
        Printf("Vole[%d] = 0x%x\n", u1Idx, u4VolTabChkPnt[u1Idx]);
    }
    #endif
    //check if volume curve if increasing
    for (u1Idx=0; u1Idx < VOL_TABLE_CHK_PNT; u1Idx++)
    {
        u4VolTabChkPnt[u1Idx] = (UINT32)StrToInt(szArgv[u1Idx+1]);
        if (u1Idx > 0)
        {
            if (u4VolTabChkPnt[u1Idx] < u4VolTabChkPnt[u1Idx-1])
            {
                Printf("FAIL!! Vol[%d] = 0x%x is less than Vol[%d] = 0x%x\n, Must be greater than or euqal to!!",
                u1Idx, u4VolTabChkPnt[u1Idx], u1Idx-1, u4VolTabChkPnt[u1Idx-1] );
                return -1;
            }
        }
        Printf("Vol[%d] = 0x%x\n", u1Idx, u4VolTabChkPnt[u1Idx]);
    }

    AUD_DspSetVolumeTable(u4VolTabChkPnt);
    return 1;
}

static INT32 _AudCmdUopQueryVolumeCurve(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8  u1Idx;

    if ( i4Argc != 1)
    {
        Printf("Usage:  vcq \n");
        return -1;
    }

    for (u1Idx=0; u1Idx < 101; u1Idx++) //FIXME !! should use definition.
    {
        if ((u1Idx % 5)==0)
        {
            Printf("\n");
        }
        Printf("0x%x ", AUD_DspGetVolumeTable(u1Idx) );
    }
    return 1;
}

#ifdef CC_SET_VOLUME
static INT32 _AudCmdUopSetVolTable(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8  u1Idx;
    UINT32 u4Volume;

    if ( i4Argc < 3)
    {
        Printf("Usage: svt [set] [volume] \n");
        Printf("[set]         0 ~ 100 \n");
        Printf("[volume] 0x0 ~ 0x20000 \n");
        return -1;

    }

        u1Idx = (UINT8)StrToInt(szArgv[1]);
        u4Volume = (UINT32)StrToInt(szArgv[2]);

   AUD_DspSetVolTable(u1Idx, u4Volume);
   return 1;
}
#endif

#ifdef CC_AUD_NEW_CV_TABLE
static INT32 _AudCmdUopSetChVolTable(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8  u1Idx;
    UINT32 u4Volume;

    if ( i4Argc < 3)
    {
        Printf("Usage: scvt [set] [volume] \n");
        Printf("[set]         0 ~ 100 \n");
        Printf("[volume] 0x0 ~ 0x20000 \n");
        return -1;

    }

        u1Idx = (UINT8)StrToInt(szArgv[1]);
        u4Volume = (UINT32)StrToInt(szArgv[2]);

   AUD_DspSetCVTable(u1Idx, u4Volume);
   return 1;
}

static INT32 _AudCmdUopQueryChannelVolumeCurve(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8  u1Idx;

    if ( i4Argc != 1)
    {
        Printf("Usage:  cvcq \n");
        return -1;
    }

    for (u1Idx=0; u1Idx < 101; u1Idx++) //FIXME !! should use definition.
    {
        if ((u1Idx % 5)==0)
        {
            Printf("\n");
        }
        Printf("0x%x ", AUD_DspGetCVTable(u1Idx) );
    }
    return 1;
}
#endif
static INT32 _AudCmdUopAutoDmx(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Enable;

    if (i4Argc < 3)
    {
        Printf("Usage: dm [decoder id] [On/Off] \n");
        Printf("[decoder id] 0:FIRST 1:SECOND \n");
        Printf("[On/Off] 0/1\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Enable = (UINT8)StrToInt(szArgv[2]);

    AUD_DspAutoDmx(u1DecId, (BOOL)u1Enable);
    return 1;
}
#ifdef CC_AUD_SOUND_MODE
static INT32 _AudCmdUopPostSoundMode(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1ModeIdx;

    if (i4Argc < 3)
    {
        Printf("Usage: psm [decoder id] [mode] \n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        //Printf("[Mode]         0:case 0     1:case 1      2:case 3... \n");
        Printf("[Mode]         0:Default     1:STANDARD      2:DYNAMIC     3:CLEAR_VOICE... \n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1ModeIdx = (UINT8)StrToInt(szArgv[2]);

   //AUD_DspPostSoundModeChange(u1DecId, u1ModeIdx);
   AUD_DspPostSoundMode(u1DecId, u1ModeIdx);
    return 1;
}

#endif
#ifndef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
static INT32 _AudCmdUopVirtualSurround(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Enable;

    if (i4Argc < 3)
    {
        Printf("Usage: vs [decoder id] [On/Off] \n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[On/Off]         1/0\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Enable = (UINT8)StrToInt(szArgv[2]);

    AUD_DspVirtualSurroundEnalbe(u1DecId, (BOOL)u1Enable);
    return 1;
}
#else
static INT32 _AudCmdUopVirtualSurround(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1ModeIdx;

    if (i4Argc < 3)
    {
        Printf("Usage: vs [decoder id] [On/Off] \n");
        Printf("[decoder id]     0:FIRST       1:SECOND \n");
        Printf("[Mode]    0:OFF  1:Normal  2:Mono to Stereo  3:Sports  4:Music  5:Cinema  6:Game\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1ModeIdx = (UINT8)StrToInt(szArgv[2]);

    AUD_DspVirtualSurroundEnalbe(u1DecId, u1ModeIdx);
    return 1;
}
#endif
static INT32 _AudCmdUopVSurrCfg(INT32 i4Argc,const CHAR** szArgv)
{
    AUD_VSURR_CFG_TYPE_T eType;
    UINT32 u4Parameter;
    AUD_VSURR_CFG_T rVSurrCfg;

    if ((i4Argc != 2) && (i4Argc != 3))
    {
        Printf("Usage: vscfg q\n");
        Printf("       Display surround parameter \n");
        //Printf("\n");
        Printf("       OR \n");
        //Printf("\n");
        Printf("Usage: vscfg [type] [parameter] \n");
        Printf("       [type]  0:Clarity      1:Width      2:L/R gain  3:Crosstalk  \n");
        Printf("               4:Output Gain  5:Bass Gain  6:Fo \n");
        Printf("       [parameter] \n");
        Printf("               Clarity     : Speech Clarity,       0 ~ 0x500000\n");
        Printf("               Width       : Sound Width,          0 ~ 0x500000\n");
        Printf("               L/R Gain    : L/R Boost Gain,       0 ~ 0x500000\n");
        Printf("               Crosstalk   : Crosstalk Boost Gain, 0 ~ 0x500000\n");
        Printf("               Output Gain : Output Boost Gain,    0 ~ 0x700000\n");
        Printf("               Bass Gain   : Bass Boost Gain,      0 ~ 0x500000\n");
        Printf("               FO          : Bass Cutoff,   0x7f41f2 ~ 0x7f4400\n");
        return -1;
    }

    if (i4Argc == 2)   //query config
    {
        AUD_Surround_Get_Config(&rVSurrCfg);
        Printf("The Settings of Virtual Surround are : \n");
        Printf("Enable      : %d\n", AUD_DspGetVirtualSurroundEnalbe(AUD_DEC_MAIN));
        Printf("Clarity     : %x\n", rVSurrCfg.u4Clarity);
        Printf("Width       : %x\n", rVSurrCfg.u4Width);
        Printf("L/R Gain    : %x\n", rVSurrCfg.u4LRGain);
        Printf("CrossTalk   : %x\n", rVSurrCfg.u4Xtalk);
        Printf("Output Gain : %x\n", rVSurrCfg.u4OutputGain);
        Printf("Bass Gain   : %x\n", rVSurrCfg.u4BassGain);
        Printf("FO          : %x\n", rVSurrCfg.u4Fo);
    }
    else if (i4Argc == 3)  //set config
    {
        eType = (AUD_VSURR_CFG_TYPE_T)StrToInt(szArgv[1]);
        u4Parameter = StrToInt(szArgv[2]);
        AUD_Surround_Set_Config(eType, u4Parameter);
        Printf("Set Type : %d, Value: %x\n", (UINT8)eType, u4Parameter);
    }

    return 0;
}

#ifdef CC_AUD_HPF_SUPPORT
static INT32 _AudCmdUopHpfEnable(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1Enable;

    if (i4Argc < 2)
    {
        Printf("Usage: hpf [On/Off] \n");
        Printf("[On/Off]         1/0\n");
        return -1;
    }

    u1Enable = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetHPFEnable((BOOL)u1Enable);
    return 1;
}

static INT32 _AudCmdUopHpfFc(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u2Fc;

    if (i4Argc < 2)
    {
        Printf("Usage: hpf_fc [fc] \n");
        Printf("[fc]         50~300\n");
        return -1;
    }

    u2Fc = (UINT16)StrToInt(szArgv[1]);

    AUD_DspSetHPFFc(u2Fc);
    return 1;
}
static INT32 _AudCmdUopHpf2Fc(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u2Fc;

    if (i4Argc < 2)
    {
        Printf("Usage: hpf2_fc [fc] \n");
        Printf("[fc]         50~300\n");
        return -1;
    }

    u2Fc = (UINT16)StrToInt(szArgv[1]);

    AUD_DspSetHPF2Fc(u2Fc);
    return 1;
}

#endif

#ifdef CC_AUD_CLIPPER_SUPPORT
static INT32 _AudCmdUopClipper(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1Enable;

    if (i4Argc < 2)
    {
        Printf("Usage: clip [On/Off] \n");
        Printf("[On/Off]         1/0\n");
        return -1;
    }

    u1Enable = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetClipperEnable((BOOL)u1Enable);
    return 1;
}

static INT32 _AudCmdUopClipperVal(INT32 i4Argc,const CHAR** szArgv)
{
    UINT32 u4Channel;
    UINT32 u4Val;

    if (i4Argc < 2)
    {
        Printf("Usage: clip [channel] [value] \n");
        Printf("[channel]       channel\n");
        Printf("[value]         val (0~0x7fffff)\n");
        return -1;
    }

    u4Channel = (UINT32)StrToInt(szArgv[1]);
    u4Val = (UINT32)StrToInt(szArgv[2]);

    AUD_DspSetClipperValue((AUD_CH_T)u4Channel, u4Val);
    return 1;
}
#endif

#ifdef CC_AUD_SPEAKER_HEIGHT
#ifndef CC_AUD_SPEAKER_HEIGHT_MODE
static INT32 _AudCmdUopSPH(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1Enable;

	if (i4Argc < 2)
    {
        Printf("Usage: sph [On/Off] \n");
        Printf("[On/Off]         1/0\n");
        return -1;
    }

    u1Enable = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetSPHEnable((BOOL)u1Enable);
    return 1;
}
#else
static INT32 _AudCmdUopSPH(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1Mode;

	if (i4Argc < 2)
    {
        Printf("Usage: sph [mode] \n");
        Printf("[mode]         0:OFF     1:2DB    2:4DB   3:6DB\n");
        return -1;
    }

    u1Mode = (UINT8)StrToInt(szArgv[1]);

   // AUD_DspSetSPHEnable((BOOL)u1Enable);
    AUD_DspSetSPHMode(u1Mode);
    return 1;
}
#endif
#endif

#ifdef CC_AUD_VBASS_SUPPORT
static INT32 _AudCmdUopVirBassCut(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u4CutOff;
    UINT8  u1CurrCutOff;

    u1CurrCutOff = AUD_GetVBassCutOff();
    if (i4Argc < 2)
    {
        Printf("Usage: vbcuut [Cut]\n");
        Printf("[Cut] 0 :50Hz, 1 :80Hz, 2:110Hz, 3:140Hz, 4:170Hz, 5:200Hz \n");
        Printf("Current it's  %d \n",u1CurrCutOff);
        return -1;
    }

    u4CutOff    = (UINT8)StrToInt(szArgv[1]);
    if ((u4CutOff >5))
    {
        Printf("Please input 0~5\n");
        return -1;
    }

    AUD_SetVBassCutOff(u4CutOff);
    AUD_DspSetVirBassEnable(FALSE);
    AUD_DspSetVirBassEnable(TRUE);
    return 0;
}

static INT32 _AudCmdUopVirBass(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1Enable;

	if (i4Argc < 2)
    {
        Printf("Usage: vb [On/Off] \n");
        Printf("[On/Off]         1/0\n");
        return -1;
    }

    u1Enable = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetVirBassEnable((BOOL)u1Enable);
    return 1;
}

static INT32 _AudCmdUopVBCfg(INT32 i4Argc,const CHAR** szArgv)
{
    UINT32 u4Frequency;
    UINT32 u4Gain1;
    UINT32 u4Gain2;
    BOOL   fgEnable;

    AUD_DspGetVirBassCfg(AUD_DEC_MAIN, &fgEnable, &u4Gain1, &u4Gain2);
    if (i4Argc < 3)
    {
        Printf("Usage: vbcfg [Gain1] [Gain2] \n");
        Printf("[Gain1--output gain(0x1000-->0x3000)] 0x1000 = 0DB\n");
        Printf("[Gain2--bass gain(0x0000-->0x2000)] 0x1000 = 0DB\n");
        Printf("Current setting : enable=%d, gain1=%d, gain2=%d \n",(UINT8)fgEnable, u4Gain1, u4Gain2);
        return -1;
    }

    u4Gain1    = (UINT32)StrToInt(szArgv[1]);
    u4Gain2 = (UINT32)StrToInt(szArgv[2]);
    u4Frequency = 0;//unused

    AUD_DspSetVirBassCfg(AUD_DEC_MAIN, u4Frequency, u4Gain1,u4Gain2);

    return 1;
}
#endif //CC_AUD_VBASS_SUPPORT

#ifdef CC_AUD_PEQ_SUPPORT
static INT32 _AudCmdUopPEQ(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1Enable;

    if (i4Argc < 2)
    {
        Printf("Usage: peq [On/Off] \n");
        Printf("[On/Off]         1/0\n");
        return -1;
    }

    u1Enable = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetPEQEnable((BOOL)u1Enable);
    return 1;
}

#if 0 //sharp Q setting
static INT32 _AudCmdUopPEQCfg2(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1PEQSet;
    UINT32 u4Frequency;
    AUD_PEQ_Q_TYPE_T eQValue;
    INT16 i2Gain;
    const CHAR *Argv;

    if (i4Argc < 5)
    {
        Printf("Usage: peq [set] [freq] [Q] [Gain] \n");
        Printf("[set]         0 ~ 8 \n");
        Printf("[u4Frequency] 20 ~ 20K \n");
        Printf("[Q]           eQValue \n");
        Printf("[Gain]        Gain -24 ~ 24 (-12dB ~ 12dB) with 0.5dB/step \n");
        return -1;
    }

    u1PEQSet    = (UINT8)StrToInt(szArgv[1]);
    u4Frequency = (UINT32)StrToInt(szArgv[2]);
    eQValue     = (AUD_PEQ_Q_TYPE_T)StrToInt(szArgv[3]);

    Argv = szArgv[4];
    if (Argv[0] == '-')
    {
        i2Gain = (UINT16)StrToInt(&Argv[1]);
        i2Gain = (UINT16)0xFFFF - (i2Gain-1);
    }
    else
    {
        i2Gain = (UINT16)StrToInt(szArgv[4]);
    }

    AUD_DspSetPEQCfg2(AUD_DEC_MAIN, u1PEQSet, u4Frequency, eQValue, i2Gain);

    return 1;
}
#else
static INT32 _AudCmdUopPEQCfg2(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1PEQSet;
    UINT32 u4Frequency;
    UINT32 eQValue;
    INT16 i2Gain;
    const CHAR *Argv;

    if (i4Argc < 5)
    {
        Printf("Usage: peq [set] [freq] [Q] [Gain] \n");
        Printf("[set]         0 ~ 8 \n");
        Printf("[u4Frequency] 20 ~ 20K \n");
        Printf("[Q]           0 ~ 100, 35 means 3.5, 100 means 10 \n");
        Printf("[Gain]        Gain -24 ~ 24 (-12dB ~ 12dB) with 0.5dB/step \n");
        return -1;
    }

    u1PEQSet    = (UINT8)StrToInt(szArgv[1]);
    u4Frequency = (UINT32)StrToInt(szArgv[2]);
    eQValue     = (UINT32)StrToInt(szArgv[3]);

    Argv = szArgv[4];
    if (Argv[0] == '-')
    {
        i2Gain = (UINT16)StrToInt(&Argv[1]);
        i2Gain = (UINT16)0xFFFF - (i2Gain-1);
    }
    else
    {
        i2Gain = (UINT16)StrToInt(szArgv[4]);
    }


    AUD_DspSetPEQCfg2(AUD_DEC_MAIN, u1PEQSet, u4Frequency, eQValue, i2Gain);

    return 1;
}
#endif


static INT32 _AudCmdUopPEQCfg(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1PEQSet;
    UINT32 u4Frequency;
    UINT32 u2BW;
    INT16 i2Gain;
    const CHAR *Argv;

    if (i4Argc < 5)
    {
        Printf("Usage: peq [set] [freq] [Bandwidth] [Gain] \n");
        Printf("[set]         0 ~ 8 \n");
        Printf("[u4Frequency] 20 ~ 20K \n");
        Printf("[Bandwidth]   0 ~ 23K \n");
        Printf("[Gain]        Gain -24 ~ 24 (-12dB ~ 12dB) with 0.5dB/step \n");
        return -1;
    }

    u1PEQSet    = (UINT8)StrToInt(szArgv[1]);
    u4Frequency = (UINT32)StrToInt(szArgv[2]);
    u2BW        = (UINT32)StrToInt(szArgv[3]);

    Argv = szArgv[4];
    if (Argv[0] == '-')
    {
        i2Gain = (UINT16)StrToInt(&Argv[1]);
        i2Gain = (UINT16)0xFFFF - (i2Gain-1);
    }
    else
    {
        i2Gain = (UINT16)StrToInt(szArgv[4]);
    }
	if(u2BW > 23000)
	{
		u2BW = 23000;
	}


    AUD_DspSetPEQCfg(AUD_DEC_MAIN, u1PEQSet, u4Frequency, u2BW, i2Gain);

    return 1;
}
#endif

static INT32 _AudCmdUopEqFlag(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Enable;

    if (i4Argc < 3)
    {
        Printf("Usage: flag      [decoder id] [on/off]\n");
        Printf("[decoder id]   0:FIRST  1:SECOND \n");
        Printf("[on/off]       1: on 0: off\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Enable = (UINT8)StrToInt(szArgv[2]);

    AUD_DspChannelEqEnable(u1DecId, (BOOL)u1Enable);

    return 1;
}

static INT32 _AudCmdUopSbassFlag(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Idx;
    UINT8 u1Enable;

    if (i4Argc < 4)
    {
        Printf("Usage: flag      [decoder id] [index] [on/off]\n");
        Printf("[decoder id]   0:FIRST  1:SECOND \n");
        Printf("[index]         0:normal  1: bass  2: pipe  3: treble\n");
        Printf("[on/off]       1: on 0: off\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Idx = (UINT8)StrToInt(szArgv[2]);
    u1Enable = (UINT8)StrToInt(szArgv[3]);

    switch(u1Idx)
    {
    case 0:
        AUD_DspBassBoostEnable(u1DecId, (BOOL)u1Enable);
        AUD_DspSuperBassEnable(u1DecId, (BOOL)0);
        AUD_DspClearBoostEnable(u1DecId, (BOOL)u1Enable);
        break;
    case 1:
        AUD_DspBassBoostEnable(u1DecId, (BOOL)u1Enable);
        break;
    case 2:
        AUD_DspSuperBassEnable(u1DecId, (BOOL)u1Enable);
        break;
    case 3:
        AUD_DspClearBoostEnable(u1DecId, (BOOL)u1Enable);
        break;
    default:
        break;
    }

    return 1;
}

static INT32 _AudCmdUopBassBoostGain(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Level;

    if (i4Argc < 3)
    {
        Printf("Usage: bb      [decoder id] [level]\n");
        Printf("[decoder id]   0:FIRST  1:SECOND \n");
        Printf("[level]        0 ~ 28 \n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Level = (UINT8)StrToInt(szArgv[2]);

    AUD_DspBassBoostGain(u1DecId, u1Level);
    return 1;
}

static INT32 _AudCmdUopClearBoostGain(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Level;

    if (i4Argc < 3)
    {
        Printf("Usage: cb      [decoder id] [level]\n");
        Printf("[decoder id]   0:FIRST  1:SECOND \n");
        Printf("[level]        0 ~ 28 \n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Level = (UINT8)StrToInt(szArgv[2]);

    AUD_DspClearBoostGain(u1DecId, u1Level);
    return 1;
}

static INT32 _AudCmdUopSbassTableQry(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4BassLevel, u4TrebleLevel;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    u4BassLevel = AUD_DspGetBassBoostGain(AUD_DEC_MAIN);
    u4TrebleLevel = AUD_DspGetClearBoostGain(AUD_DEC_MAIN);
    Printf("Bass Enable: %d\n", AUD_DspGetBassBoostEnable(AUD_DEC_MAIN));
    Printf("Bass level: %d\n", u4BassLevel);
    Printf("Treble Enable: %d\n", AUD_DspGetClearBoostEnable(AUD_DEC_MAIN));
    Printf("Treble level: %d\n", u4TrebleLevel);

    return 1;
}

#ifdef CC_AUD_SILENCE_SUPPORT
static INT32 _AudCmdUopSilenceFlag(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Enable;

    if (i4Argc != 2)
    {
        Printf("Usage: flag    [on/off]\n");
        Printf("[on/off]       1: on 0: off\n");
        return -1;
    }

    u1Enable = (UINT32)StrToInt(szArgv[1]);

    if(u1Enable == 1)
    {
		DSP_SilenceSetMode(TRUE);
	}
	else
	{
		DSP_SilenceSetMode(FALSE);
	}

    return 1;
}

static INT32 _AudCmdUopSilenceThreshold(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Threshold;

    if (i4Argc != 2)
    {
        Printf("Usage: thre    [threshold]\n");
        Printf("[threshold]       0x0~0x1000\n");
        return -1;
    }

    u4Threshold = (UINT32)StrToInt(szArgv[1]);
	DSP_SilenceSetThreshold(u4Threshold);

    return 1;
}

static INT32 _AudCmdUopSilenceWait(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Wait;

    if (i4Argc != 2)
    {
        Printf("Usage: wait    [wait time:ms]\n");
        Printf("[wait time:ms]       0~10000\n");
        return -1;
    }

    u4Wait = (UINT32)StrToInt(szArgv[1]);
	DSP_SilenceSetWait(u4Wait);

    return 1;
}

static INT32 _AudCmdUopSilenceFixGain(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4AttackFixGain;

    if (i4Argc != 2)
    {
        Printf("Usage: gain    [u4AttackFixGain]\n");
        Printf("[gain]       0x0~0x7fffff\n");
        return -1;
    }
    u4AttackFixGain = (UINT32)StrToInt(szArgv[1]);
	DSP_SilenceSetFixGain(u4AttackFixGain);
	return 1;
}

static INT32 _AudCmdUopSilenceReleaseStep(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4ReleaseStep;

    if (i4Argc != 2)
    {
        Printf("Usage: thr    [Release:ms]\n");
        Printf("[threshold]       0~1000\n");
        return -1;
    }
    u4ReleaseStep = (UINT32)StrToInt(szArgv[1]);
	DSP_SilenceSetReleaseStep(u4ReleaseStep);
	return 1;
}
static INT32 _AudCmdUopSilenceAttackStep(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4AttackStep;

    if (i4Argc != 2)
    {
        Printf("Usage: thr    [Attack:ms]\n");
        Printf("[threshold]       0~1000\n");
        return -1;
    }
	u4AttackStep = (UINT32)StrToInt(szArgv[1]);
	DSP_SilenceSetAttackStep(u4AttackStep);
	return 1;
}

static INT32 _AudCmdUopSilenceSetQry(INT32 i4Argc, const CHAR ** szArgv)
{
	if (i4Argc != 1)
	{
		return -1;
	}
	UINT32 u4Enable;
	UINT32 u4Threshold;
	UINT32 u4PeakValue;
	UINT32 u4Wait;
	UINT32 u4AttackFixGain;
	UINT32 u4ReleaseStep;
	UINT32 u4AttackStep;
    DSP_QrySilenceMode(&u4Enable);
    DSP_QrySilenceSetThreshold(&u4Threshold);
	DSP_QrySilencePeakValue(&u4PeakValue);
	DSP_QrySilenceWait(&u4Wait);
	DSP_QrySilenceAttackFixGain(&u4AttackFixGain);
	DSP_QrySilenceReleaseStep(&u4ReleaseStep);
    DSP_QrySilenceAttackStep(&u4AttackStep);
	Printf("Enable               %d\n", u4Enable);
	Printf("Threshold          0x%x\n", u4Threshold);
	Printf("Noise Level        0x%x\n", u4PeakValue);
	Printf("Attack Wait        %dms\n", u4Wait);
	Printf("Attack Gain        0x%x\n", u4AttackFixGain);
	Printf("Release            %dms\n", u4ReleaseStep);
	Printf("Attack             %dms\n", u4AttackStep);
	return 1;
}
#endif

static INT32 _AudCmdUopEqMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Mode;

    if (i4Argc < 3)
    {
        Printf("Usage: m     [decoder id] [mode]\n");
        Printf("[decoder id]  0:FIRST  1:SECOND \n");
        Printf("[mode]        0 NONE   \n");
        Printf("              1 ROCK   \n");
        Printf("              2 POP    \n");
        Printf("              3 LIVE   \n");
        Printf("              4 DANCE  \n");
        Printf("              5 TECHNO \n");
        Printf("              6 Classic\n");
        Printf("              7 Soft\n");
        Printf("              8 Custom 1\n");
        Printf("              9 Custom 2\n");
        Printf("              10 Custom 3\n");

        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Mode = (UINT8)StrToInt(szArgv[2]);

    AUD_DspChEqLoadPreset(u1DecId, (AUD_EQ_TYPE_T)u1Mode);
    return 1;
}

static INT32 _AudCmdUopEqTableQry(INT32 i4Argc, const CHAR ** szArgv)
{
    INT8  eqValue[CH_EQ_BAND_NO+1];
    int   i;
    AUD_EQ_TYPE_T eEqType;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    AUD_DspGetEqType(AUD_DEC_MAIN, &eEqType);
    AUD_DspChQryEqTable(AUD_DEC_MAIN, eEqType, eqValue);

    Printf("EQ Enable: %d\n", AUD_DspGetChannelEqEnable(AUD_DEC_MAIN));
    Printf("EQ type = %d\n", eEqType);
    for (i = 1; i <= CH_EQ_BAND_NO; i++)
    {
        printf ("band%d : %d\n", i, eqValue[i]);
    }
    return 1;
}

#ifdef CC_AUD_7BAND_EQUALIZER
static INT32 _AudCmdUopEqTableSet(INT32 i4Argc, const CHAR ** szArgv)
{
    INT8  eqValue[CH_EQ_BAND_NO+1];
    INT8  eqRange;
    int   i;
    AUD_EQ_TYPE_T eEqType;

    eqRange  = (INT8)StrToInt(szArgv[1]);
    if ((i4Argc < 5)||((eqRange !=0) && (eqRange !=1)) )
    {

        Printf("Usage: set  [range=0] [band1] [band2] [band3] [band4] \n");
        Printf("Usage: set  [range=1] [band5] [band6] [band7]\n");

        Printf("[bandn]: -28 ~ 28 (for -14dB ~ 14dB, 0.5 dB/Step) \n");
        return -1;
    }

    AUD_DspGetEqType(AUD_DEC_MAIN, &eEqType);

    if (eEqType == AUD_EQ_OFF)
    {
        eEqType = AUD_EQ_CUST1;
    }
    Printf("\n Your setting to current EQ type : %d\n", eEqType);
    AUD_DspChQryEqTable(AUD_DEC_MAIN, eEqType, eqValue);

    if (eqRange == 0)
    {
        for (i = 1; i <= 4; i++)
        {
            Printf("Band[%d] : ", i);
            eqValue[i] = (INT8)i4StrToInt(szArgv[i+1]);
            Printf("%d \n", eqValue[i] );
            //eqValue[i] -= (EQ_BAND_GAIN_DB_MAX * 2);
        }
    }
    else if (eqRange == 1)
    {
        for (i = 1; i <= 3; i++)
        {
            Printf("Band[%d] : ", i+4);
            eqValue[i+4] = (INT8)i4StrToInt(szArgv[i+1]);
            Printf("%d \n", eqValue[i+4] );
            //eqValue[i+4] -= (EQ_BAND_GAIN_DB_MAX * 2);
        }
    }

    eqValue[0] = 0;
    AUD_DspChSetEqTable(AUD_DEC_MAIN, eEqType, eqValue); //user defined EQ
    AUD_DspChEqLoadPreset(AUD_DEC_MAIN, eEqType);
    return 1;
}
#else
static INT32 _AudCmdUopEqTableSet(INT32 i4Argc, const CHAR ** szArgv)
{
    INT8  eqValue[CH_EQ_BAND_NO+1];
    int   i;
    AUD_EQ_TYPE_T eEqType;

    if (i4Argc < 6)
    {

        Printf("Usage: set  [band1] [band2] [band3] [band4] [band5]\n");
        Printf("[bandn]: -28 ~ 28 (for -14dB ~ 14dB, 0.5 dB/Step) \n");
        return -1;
    }

    AUD_DspGetEqType(AUD_DEC_MAIN, &eEqType);

    if (eEqType == AUD_EQ_OFF)
    {
        eEqType = AUD_EQ_CUST1;
    }
    Printf("\n Your setting to current EQ type : %d\n", eEqType);

    eqValue[0] = 0;
    for (i = 1; i <= CH_EQ_BAND_NO; i++)
    {
        Printf("Band[%d] : ", i);
        eqValue[i] = (INT8)i4StrToInt(szArgv[i]);
        Printf("%d \n", eqValue[i] );
        //eqValue[i] -= (EQ_BAND_GAIN_DB_MAX * 2);
    }
    AUD_DspChSetEqTable(AUD_DEC_MAIN, eEqType, eqValue); //user defined EQ
    AUD_DspChEqLoadPreset(AUD_DEC_MAIN, eEqType);
    return 1;
}
#endif

static INT32 _AudCmdUopLimiterMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Mode;

    if (i4Argc < 2)
    {
        Printf("Usage: m  [mode]\n");
        Printf("[mode]    0: off\n");
        Printf("          1: adaptive mode\n");
        Printf("          2: fixed mode\n");
        return -1;
    }

    u1Mode = (UINT8)StrToInt(szArgv[1]);
    AUD_DspSetLimiterMode(u1Mode);
    return 1;
}

static INT32 _AudCmdUopLimiterFlag(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2Flag = 0, u2Temp;
    UINT8 i;

    if (i4Argc < (AUD_LIMITER_NUM+1))
    {
        Printf("Usage: flag  [vol] [vsurr] [sbass] [EQ] [reverb] [speaker] [voice]\n");
        Printf("0: off   1: on\n");
        return -1;
    }

    for (i=1; i<=AUD_LIMITER_NUM; i++)
    {
        u2Temp = (UINT8)StrToInt(szArgv[i]);
        u2Temp = ((u2Temp & 0x01) << (i - 1));
        u2Flag |= u2Temp;
    }

    AUD_DspSetLimiterFlag(u2Flag);
    return 1;
}

static INT32 _AudCmdUopLimiterThre(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Thre;

    if (i4Argc < 2)
    {
        Printf("Usage: thre  [threshold]\n");
        Printf("[threshold]     0x0 ~ 0x7fffff\n");
        return -1;
    }

    u4Thre = (UINT32)StrToInt(szArgv[1]);
    AUD_DspSetLimiterThreshold(u4Thre);
    return 1;
}

#ifdef CC_RATIO_LIMITER
static INT32 _AudCmdUopLimiterGainRatio(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Ratio;

    if (i4Argc < 2)
    {
        Printf("Usage: ratio  [value]\n");
        Printf("[value]     0x0 ~ 0x7fffff\n");
        return -1;
    }

    u4Ratio = (UINT32)StrToInt(szArgv[1]);
    AUD_DspSetLimiterGainRatio(u4Ratio);
    return 1;
}
#endif

static INT32 _AudCmdUopLimiterQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Mode;
    UINT16 u2Flag;
    UINT32 u4Thre;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    AUD_DspGetLimiterConfig(&u1Mode, &u2Flag, &u4Thre);
    if (u1Mode == 0)
    {
        Printf("Mode: off\n");
    }
    else if (u1Mode == 1)
    {
        Printf("Mode: adaptive mode\n");
    }
    else
    {
        Printf("Mode: fixed mode\n");
    }

    Printf("Flag: 0x%x\n", u2Flag);
    Printf("Threshold: 0x%x\n", u4Thre);

    return 1;
}

static INT32 _AudCmdUopDownmixPosition(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DMPosition;

    if (i4Argc < 2)
    {
        Printf("Usage: dmp [Downmix Channel(CH9/10) Postion]\n");
        Printf("[mode] 0  do ALL post processing   \n");
        Printf("       1  do downmix after input source gain and before AVC\n");
        Printf("       2  Skip post-proc & do manual output matrix\n");
        Printf("       3  Skip post-proc & skip manual output matrix\n");
        return -1;
    }

    u1DMPosition = (UINT8)StrToInt(szArgv[2]);

    AUD_DspDownmixPosition(u1DMPosition);
    return 1;
}



static INT32 _AudCmdDspSpeed(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT16 u2Speed;

    if (i4Argc < 2)
    {
        Printf("Usage: cb     [decoder id] [FF Speed times]\n");
        Printf("[decoder id]  0:FIRST  1:SECOND \n");
        Printf("[mode]        0 normal play   \n");
        Printf("                 2  FF 2-times   \n");
        Printf("                 4  FF 4-times    \n");
        Printf("                 8  FF 8-times   \n");
        Printf("                16  FF 16-times  \n");
        Printf("                32 FF 32-times \n");


        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u2Speed = (UINT16)StrToInt(szArgv[2]);

    if(u2Speed != 0)
    {
        u2Speed --;
    }

    AUD_DspSpeed(u1DecId, u2Speed);
    return 1;
}

static INT32 _AudCmdUopMultiPairOutput(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT16 u2Flag;

    if (i4Argc < 2)
    {
        Printf("Usage: MultiPair [decoder id] [ON/OFF]\n");
        Printf("[decoder id]  0:FIRST  1:SECOND \n");
        Printf("[mode]        0  Didn't Suppot multi-pair output \n");
        Printf("                 1  Suppot multi-pair output(Copy CH9/10 to LS/RS/C/SUB)   \n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u2Flag = (UINT16)StrToInt(szArgv[2]);

    AUD_DspMultiPairOutput(u1DecId, u2Flag);
    return 1;
}

static INT32 _AudCmdUopLRDownMix(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL u1Flag;

    if (i4Argc < 2)
    {
        Printf("Usage: LRDMix [ON/OFF]\n");
        Printf("[mode]        0  Didn't do L/R downmix \n");
        Printf("                 1  Downmix to L/R channel after Reverbation and before BassManagement   \n");
        return -1;
    }
    if (i4Argc >= 2)
    {
        if (x_strcmp(szArgv[1], "on") == 0)
        {
            u1Flag = TRUE;
        }
        else if (x_strcmp(szArgv[1], "1") == 0)
        {
            u1Flag = TRUE;
        }
        else if (x_strcmp(szArgv[1], "off") == 0)
        {
            u1Flag = FALSE;
        }
        else if (x_strcmp(szArgv[1], "0") == 0)
        {
            u1Flag = FALSE;
        }
        else
        {
           Printf("Usage: LRDMix [ON/OFF]\n");
           Printf("[mode]        0  Didn't do L/R downmix \n");
           Printf("                 1  Downmix to L/R channel after Reverbation and before BassManagement   \n");
               return -1;
           }
     }
    AUD_DspLRDownmix(AUD_DEC_MAIN, u1Flag);
    return 1;
}


static INT32 _AudCmdDspUopDump(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    AUD_DumpUopCommand();
    return 1;
}

static INT32 _AudCfgAvInSwitch(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1InputId;

    if (i4Argc < 2)
    {
        Printf("Usage: isw    [source]\n");
        Printf("[source]      0 Composite video (CVBS) # 0   \n");
        Printf("              1 Composite video (CVBS) # 1   \n");
        Printf("              2 Composite video (CVBS) # 2   \n");
        Printf("              3 Composite video (CVBS) # 3   \n");
        Printf("              4 S-Video #0   \n");
        Printf("              5 S-Video #1  \n");
        Printf("              6 S-Video #2  \n");
        Printf("              7 YPbPr # 0 \n");
        Printf("              8 YPbPr # 1 \n");
        Printf("              9 YPbPr # 2 \n");
        Printf("              10 VGA, via headphone jet  # 0\n");
        Printf("              11 VGA, via headphone jet  # 1\n");
        Printf("              12 DVI  # 0 \n");
        Printf("              13 DVI  # 1 \n");
        Printf("              14 DVI  # 2 \n");
        Printf("              15 DVI  # 3 \n");
        Printf("              22 SCART0\n");
        Printf("              23 SCART1 \n");


        return -1;
    }

    u1InputId = (UINT8)StrToInt(szArgv[1]);
	AUD_AvMuxSel((AUD_INPUT_ID_T)u1InputId);

    return 1;
}

static INT32 _AudCfgChlOutSwitch(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Data0;
    UINT8 u1Data1;
    UINT8 u1Data2;
    UINT8 u1Data3;
    UINT8 u1Data4;
    //UINT8 u1Data5;    //MT5365 only has 5 I2S data pin (AOSDATA0~AOSDATA4)

    if (i4Argc < 6)
    {
        if ((i4Argc == 2) && (szArgv[1][0]=='q'))
        {
            Printf("[OSDATA setting]\n");
            AUD_AoutPADMuxQuery();
            return 1;
        }
        Printf("Usage: osw    [PAD_SDATA0] [PAD_SDATA1] [PAD_SDATA2] \n");
        Printf("              [PAD_SDATA3] [PAD_SDATA4]\n");
        Printf("       osw q\n");
        Printf("[PAD_DATA]    0 L/R   \n");
        Printf("              1 Ls/Rs \n");
        Printf("              2 C/Sw  \n");
        Printf("              3 Bypass  \n");
        Printf("              4 Downmix \n");
        Printf("              5 Aux decoder output \n");
        Printf("ex. osw 0 1 2 3 4\n");
        return -1;
    }

    u1Data0 = (UINT8)StrToInt(szArgv[1]);
    u1Data1 = (UINT8)StrToInt(szArgv[2]);
    u1Data2 = (UINT8)StrToInt(szArgv[3]);
    u1Data3 = (UINT8)StrToInt(szArgv[4]);
    u1Data4 = (UINT8)StrToInt(szArgv[5]);
    //u1Data5 = (UINT8)StrToInt(szArgv[6]);

    AUD_AoutPADMux((AUD_CH_T)(u1Data0*2), (AUD_CH_T)(u1Data1*2), (AUD_CH_T)(u1Data2*2),
                   (AUD_CH_T)(u1Data3*2), (AUD_CH_T)(u1Data4*2), AUD_CH_AUX_FRONT_LEFT, 0);

    return 1;
}

extern void AUD_SltCheckStream(void);
extern void AUD_WriteGoldenStream(void);

#ifdef LINUX_TURNKEY_SOLUTION
static UINT8* pTonePlayBuf;
#endif

static INT32 _AudCmdPlayTone(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Sel = 0;
    UINT8 u1DecId;
    MEM_BUFFER_INFO_T  rMemBuf;
    AUD_PCM_SETTING_T  rPcmSetting;

    if (i4Argc < 2)
    {
        Printf("Usage: tone [tone_sel] [dec_id]\n");
        Printf("[tone_sel]   0  1K tone 0dB\n");
        Printf("             1  1K tone -20dB\n");
        Printf("             2  1K tone -60dB\n");
        Printf("             3  1K tone -120dB\n");
        Printf("             4  20K tone 0dB\n");
        Printf("             5  4K tone 0dB\n");
        Printf("             6  200 tone 0dB\n");
        Printf("             7  200 tone -60dB\n");
        Printf("             8  400 tone 0dB\n");
        Printf("             9  1K/3K tone 0dB\n");
        Printf("             10  1K tone 0dB L channel only\n");
        Printf("[dec_id]     0: Main, 1: AUX, 2: 3rd, 3: 4th\n");
        return -1;
    }

    u1Sel = (UINT8)StrToInt(szArgv[1]);

    u1DecId = (i4Argc == 3) ? (UINT8)StrToInt(szArgv[2]) : AUD_DEC_MAIN;
    if (u1DecId >= AUD_DEC_NUM)
    {
        u1DecId = AUD_DEC_MAIN;
    }

    if (AUD_IsDecoderPlay(u1DecId))
    {
        AUD_StopMemStr();
        AUD_DSPCmdStop(u1DecId);
    }

    rMemBuf.u4LoopCount = 0; // Infinite loop

    rPcmSetting.eSampleFreq = FS_48K;
    rPcmSetting.ePcmDataInfo = PCM_24_BITS;

    AUD_DspProcMode(u1DecId, 0xdc00);
    AUD_SetDecType(u1DecId, AUD_STREAM_FROM_MEMORY, AUD_FMT_PCM);
    AUD_PcmSetting(u1DecId, (const AUD_PCM_SETTING_T *)&rPcmSetting);

#ifdef LINUX_TURNKEY_SOLUTION
    if (pTonePlayBuf)
    {
        BSP_FreeAlignedDmaMemory((UINT32)pTonePlayBuf);
        pTonePlayBuf = NULL;
    }
#endif

    AUD_StrSelect(u1DecId, u1Sel, &rMemBuf);

#ifdef LINUX_TURNKEY_SOLUTION
    pTonePlayBuf = (UINT8 *)BSP_AllocAlignedDmaMemory(rMemBuf.u4Length, 32);
    x_memcpy((VOID*)VIRTUAL((UINT32)pTonePlayBuf), (VOID*)VIRTUAL((UINT32)rMemBuf.pData), rMemBuf.u4Length);
    rMemBuf.pData = (UINT8*)VIRTUAL((UINT32)pTonePlayBuf);
#endif

    AUD_SetDecMemBuffer(u1DecId, &rMemBuf);
    AUD_DSPCmdPlay(u1DecId);

    return 1;
}

extern UINT8 _au1TableImpulse0dB[];
static INT32 _AdspDolbyImpulse(INT32 i4Argc, const CHAR ** szArgv)
{
    AUD_PCM_SETTING_T rPcmSetting;
    MEM_BUFFER_INFO_T  rMemBuf;
    	
    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (AUD_IsDecoderPlay(AUD_DEC_MAIN))
    {
        AUD_StopMemStr();
        AUD_DSPCmdStop(AUD_DEC_MAIN);
    }

    rMemBuf.u4LoopCount = 0; // Infinite loop

    rPcmSetting.eSampleFreq = FS_48K;
    rPcmSetting.ePcmDataInfo = PCM_24_BITS;

    AUD_DspProcMode(AUD_DEC_MAIN, 0xdc00);
    AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_MEMORY, AUD_FMT_PCM);
    AUD_PcmSetting(AUD_DEC_MAIN, (const AUD_PCM_SETTING_T *)&rPcmSetting);

#ifdef LINUX_TURNKEY_SOLUTION
    if (pTonePlayBuf)
    {
        BSP_FreeAlignedDmaMemory((UINT32)pTonePlayBuf);
        pTonePlayBuf = NULL;
    }

    rMemBuf.u4Length = 3*2*1024;

    pTonePlayBuf = (UINT8 *)BSP_AllocAlignedDmaMemory(rMemBuf.u4Length, 32);
    rMemBuf.pData = (UINT8*)VIRTUAL((UINT32)pTonePlayBuf);

    x_memset((VOID*)VIRTUAL((UINT32)pTonePlayBuf), 0, rMemBuf.u4Length);
    x_memcpy((VOID*)VIRTUAL((UINT32)pTonePlayBuf), (VOID*)VIRTUAL((UINT32)_au1TableImpulse0dB), 3*2);
#else
    // TODO!!!!
    LOG (0, "This function does not support for non-linux system!\n");
#endif

    AUD_SetDecMemBuffer(AUD_DEC_MAIN, &rMemBuf);
    AUD_DSPCmdPlay(AUD_DEC_MAIN);    

    LOG (0, "Left channel Impulse signal is playing...\n");
    LOG (0, "1) Please check if only left channel has signals.\n");
    LOG (0, "2) Please use frequency analyser to check the frequency response. It must be flat.\n");
    
    return 1;
}

static INT32 _AudCmdSoundEffect(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Flag = FALSE;

    if (i4Argc < 2)
    {
        Printf("Usage: soundeffect  [on/off]\n");
        Printf("[on/off]     1: on  \n");
        Printf("                 0: off  \n");
        return -1;
    }

    if (i4Argc >= 2)
    {
        if (x_strcmp(szArgv[1], "on") == 0)
        {
            u1Flag = TRUE;
        }
        else if (x_strcmp(szArgv[1], "1") == 0)
        {
            u1Flag = TRUE;
        }
        else if (x_strcmp(szArgv[1], "off") == 0)
        {
            u1Flag = FALSE;
        }
        else if (x_strcmp(szArgv[1], "0") == 0)
        {
            u1Flag = FALSE;
        }
        else
        {
               Printf("Usage: soundeffect  [on/off]\n");
               Printf("[on/off]     1: on  \n");
               Printf("                0: off  \n");
               return -1;
           }
     }

    AUD_DspSoundEffectFlag(AUD_DEC_MAIN, u1Flag);

    return 1;
}

static INT32 _AudCmdBypassPostProcessing(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2Flag = FALSE;
    UINT8 u1DecId = AUD_DEC_MAIN;

    if (i4Argc == 2)
    {
        u2Flag = (UINT16)StrToInt(szArgv[1]);
    }
    else if (i4Argc == 3)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
        u2Flag = (UINT16)StrToInt(szArgv[2]);
    }
    else
    {
        Printf("Usage: bypass [dec id] [on/off]\n");
        Printf("[dec id] 0: Main, 1: Aux, 2: Third, 3: 4th\n");
        Printf("[on/off] 1: on (0xf000), 0: 0ff\n");
        Printf("         0x8000: bypass delay\n");
        Printf("         0x4000: bypass trim\n");
        Printf("         0x2000: bypass post-proc\n");
        Printf("         0x1000: bypass bass-management\n");
        return -1;
    }

    AUD_DspBypassPostProcess(u1DecId, u2Flag);

    return 1;
}

#ifdef __MODEL_slt__
static INT32 _AudCmdSltSorting(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
    if (SLT_SortingAud() == 0)
    {
        Printf("Soring result (ok)");
    }
    else
    {
        Printf("Soring result (fail)");
    }
    return 1;
}

static INT32 _AudCmdSltFlashing(INT32 i4Argc, const CHAR ** szArgv)
{

    UNUSED(i4Argc);
    UNUSED(szArgv);
    SLT_FlashingAudInit();

    return 1;
}

static INT32 _AudCmdDtvSltSorting(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (SLT_DtvSorting() == 0)
    {
        Printf("DTV Soring result (ok)");
    }
    else
    {
        Printf("DTV Soring result (fail)");
    }

    return 1;
}

static INT32 _AudCmdAtvSltSorting(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (SLT_AtvSorting() == 0)
    {
        Printf("ATV Soring result (ok)");
    }
    else
    {
        Printf("ATV Soring result (fail)");
    }
    return 1;
}

static INT32 _AudCmdIntAdSltSorting(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (SLT_IntAdSorting() == 0)
    {
        Printf("Internal AD Soring result (ok)");
    }
    else
    {
        Printf("Internal AD Soring result (fail)");
    }
    return 1;
}

static INT32 _AudCmdROMSltSorting(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (SLT_ROMSorting() == 0)
    {
        Printf("ROM Soring result (ok)");
    }
    else
    {
        Printf("ROM Soring result (fail)");
    }
    return 1;
}
#endif

static INT32 _AudCmdStopMemStr(INT32 i4Argc, const CHAR ** szArgv)
{

    UNUSED(i4Argc);
    UNUSED(szArgv);

    AUD_StopMemStr();

    return 1;
}

#ifdef CC_AUD_MIXSOUND_SUPPORT
static INT32 _AudCmdMixSndQuery(INT32 i4Argc,const CHAR** szArgv)
{
    Printf("[MixSound Status]\n");
    Printf("  Enable: %d\n", AUD_DspGetMixSndEnable());
    Printf("  Gain: 0x%x\n", AUD_DspGetMixSndGain());
    Printf("  Stereo: %d\n", AUD_DspGetMixSndStereo());
    Printf("  Vol: %d\n", AUD_DspGetMixSndVol());
    Printf("  Loop: %d\n", AUD_DspGetMixSndLoop());
    return 1;
}

static INT32 _AudCmdMixSndLoop(INT32 i4Argc,const CHAR** szArgv)
{
    UINT32 u4Loop;
    if (i4Argc == 2)
    {
        u4Loop= (UINT32)StrToInt(szArgv[1]);
        if(u4Loop<=256)
        {
            AUD_DspSetMixSndLoop(u4Loop);
            return 1;
        }
    }

    Printf("Usage: mixsound loop [loop] \n");
    Printf("[mixsound loop] 0 ~ 256\n");
    return -1;
}

static INT32 _AudCmdMixSndVol(INT32 i4Argc,const CHAR** szArgv)
{
    UINT32 u4Volume;

    if (i4Argc == 2)
    {
        u4Volume = (UINT8)StrToInt(szArgv[1]);
        if(u4Volume<=100)
        {
            AUD_DspSetMixSndVol(u4Volume);
            return 1;
        }
    }

    Printf("Usage: mixsound volume [volume]\n");
    Printf("[mixsound volume] 0 ~ 100\n");
    return -1;
}

static INT32 _AudCmdMixSndFlag(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Flag;

    if (i4Argc != 2)
    {
        Printf("Usage: flag [on/off]\n");
        Printf("[on/off] 1: on 0: off\n");
        return -1;
    }

    u1Flag = (UINT8)StrToInt(szArgv[1]);
    AUD_DspMixSndEnable(u1Flag);
    return 1;
}

static INT32 _AudCmdMixSndPlay(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2Mode;

    u2Mode = (UINT16)StrToInt(szArgv[1]);

    if (i4Argc != 2)
    {
        goto lbUsage;
    }

    if ((u2Mode == 1)||(u2Mode == 2)||(u2Mode == 4)
        #ifdef NEW_MIXSOUND
        ||(u2Mode ==8)
        #endif
        )
    {
        #ifndef NEW_MIXSOUND
        #ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
        AUD_DisableMixSndClip();
        #endif
        #endif
        AUD_DspMixSndControl(u2Mode);
        return 1;
    }

lbUsage:
    Printf("Usage: p [mode]\n");
    Printf("[mode]   1 PLAY\n");
    Printf("         2 STOP\n");
    Printf("         4 FORCE STOP AND PLAY \n");
  #ifdef NEW_MIXSOUND
    Printf("         8 DISABLE\n");
  #endif
    return -1;
}

static INT32 _AudCmdMixSndClip(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1ClipIdx;

    if (i4Argc != 2)
    {
        Printf("Usage: str [clip idx]\n");
        Printf("[clip idx] 0 ~ 3\n");
        return -1;
    }

    u1ClipIdx = (UINT8)StrToInt(szArgv[1]);
    AUD_DspMixSndClip(u1ClipIdx);
    return 1;
}

#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY

static INT32 _AudCmdQueryMixSndClip(INT32 i4Argc, const CHAR ** szArgv)
{
    AUD_QueryMixSndClip();
    return 0;
}

static INT32 _AudCmdQueryMixSndClipDsp(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1StreamID;

    if (i4Argc != 2)
    {
        Printf("Usage: qdsp [u1StreamID]\n");
        return -1;
    }

    u1StreamID = (UINT8)StrToInt(szArgv[1]);
    AUD_DspShowMixSndStatus(u1StreamID);
    return 0;
}

static INT32 _AudCmdStopMixSndClip(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1StreamID;

    if (i4Argc != 2)
    {
        Printf("Usage: stop [u1StreamID]\n");
        return -1;
    }

    u1StreamID = (UINT8)StrToInt(szArgv[1]);
    AUD_StopMixSndClip(u1StreamID);

    return 0;
}

static INT32 _AudCmdPauseMixSndClip(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1StreamID;

    if (i4Argc != 2)
    {
        Printf("Usage: pause [u1StreamID]\n");
        return -1;
    }

    u1StreamID = (UINT8)StrToInt(szArgv[1]);

    AUD_PauseMixSndClip(u1StreamID);
    return 0;
}

static INT32 _AudCmdResumeMixSndClip(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1StreamID;

    if (i4Argc != 2)
    {
        Printf("Usage: resume [u1StreamID]\n");
        return -1;
    }

    u1StreamID = (UINT8)StrToInt(szArgv[1]);

    AUD_ResumeMixSndClip(u1StreamID);
    return 0;
}

static INT32 _AudCmdRepeatMixSndClip(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1StreamID;

    if (i4Argc != 2)
    {
        Printf("Usage: repeat [u1StreamID]\n");
        return -1;
    }

    u1StreamID = (UINT8)StrToInt(szArgv[1]);

    AUD_RepeatMixSndClip(u1StreamID);
    return 0;
}

static INT32 _AudCmdSetMixSndClip(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1StreamID = 0;
    UINT32 u4Addr;
    UINT32 u4Size;
    UINT32 u4Gain = 0x8000;
    UINT8 u1StereoOnOff = TRUE;
    UINT32 u4SampleRate = 48000;
    UINT8 u1BitDepth = 16;

    if (i4Argc != 8)
    {
        Printf("set [id] [addr] [size] [vol] [stereo on/off] [sample rate] [bit depth]\n");
        return -1;
    }

    u1StreamID = (UINT8)StrToInt(szArgv[1]);
    u4Addr = (UINT32)StrToInt(szArgv[2]);
    u4Size = (UINT32)StrToInt(szArgv[3]);
    u4Gain = (UINT32)StrToInt(szArgv[4]);
    u1StereoOnOff = (UINT8)StrToInt(szArgv[5]);
    u4SampleRate = (UINT32)StrToInt(szArgv[6]);
    u1BitDepth = (UINT8)StrToInt(szArgv[7]);

    AUD_SetMixSndClip(u1StreamID, u4Addr, u4Size, u4Gain, u1StereoOnOff, u4SampleRate, u1BitDepth);
    return 0;
}

static INT32 _AudCmdPlayMixSndClip(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1StreamID;
    if (i4Argc != 2)
    {
        Printf("Usage: play [u1StreamID]\n");
        return -1;
    }

    u1StreamID = (UINT8)StrToInt(szArgv[1]);

    AUD_PlayMixSndClip(u1StreamID);
    return 0;
}

static INT32 _AudCmdInitMixSndClip(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Init;
    if (i4Argc != 2)
    {
        Printf("Usage: init [0/1]\n");
        Printf("  0 - disable, 1 - enable\n");
        return -1;
    }

    u1Init = (UINT8)StrToInt(szArgv[1]);

    if (u1Init)
    {
        AUD_EnableMixSndClip();
    }
    else
    {
        AUD_DisableMixSndClip();
    }

    return 0;
}

static INT32 _AudCmdTestMixSndClip(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Addr;
    UINT32 u4Size;
    UINT32 u4Gain;
    UINT8 u1StereoOnOff;
    UINT32 u4SampleRate;
    UINT32 u4Clip;
    UINT32 u4LoopNum;

    UINT32 u4CurSize;
    if ((i4Argc != 8) && (i4Argc != 5))
    {
        Printf("Usage: test [addr] [size] [vol] [stereo on/off] [sample rate] [clip size] [loop num]\n");
        Printf("Usage: test [addr] [size] [clip size] [loop num]\n");
        return -1;
    }

    if (i4Argc == 8)
    {
        u4LoopNum = (UINT32)StrToInt(szArgv[7]);
    }
    else
    {
        u4LoopNum = (UINT32)StrToInt(szArgv[4]);
    }

    AUD_EnableMixSndClip();

    while (u4LoopNum--)
    {
        Printf("Loop %d - queue\n", u4LoopNum);
        u4Addr = (UINT32)StrToInt(szArgv[1]);
        u4Size = (UINT32)StrToInt(szArgv[2]);
        if (i4Argc == 8)
        {
            u4Gain = (UINT32)StrToInt(szArgv[3]);
            u1StereoOnOff = (UINT8)StrToInt(szArgv[4]);
            u4SampleRate = (UINT32)StrToInt(szArgv[5]);
            u4Clip = (UINT32)StrToInt(szArgv[6]);
        }
        else
        {
            u4Gain = 0x1000;
            u1StereoOnOff = TRUE;
            u4SampleRate = 48000;
            u4Clip = (UINT32)StrToInt(szArgv[3]);
        }

        //pre-queueing
        while (u4Size)
        {
            u4CurSize = ((u4Size > u4Clip) ? u4Clip : u4Size);
            if (!AUD_SetMixSndClip(0,u4Addr, u4CurSize, u4Gain, u1StereoOnOff, u4SampleRate, 16))
            {
                break;
            }
            u4Addr += u4CurSize;
            u4Size -= u4CurSize;
        }

        Printf("Loop %d - play\n", u4LoopNum);

        //trigger play
        AUD_PlayMixSndClip(0);

        Printf("Loop %d - repeat set and no delay\n", u4LoopNum);

        //keep-queueing until done
        while (u4Size)
        {
            u4CurSize = ((u4Size > u4Clip) ? u4Clip : u4Size);
            if (!AUD_SetMixSndClip(0,u4Addr, u4CurSize, u4Gain, u1StereoOnOff, u4SampleRate, 16))
            {
                x_thread_delay(10);
                continue;
            }
            u4Addr += u4CurSize;
            u4Size -= u4CurSize;
        }
    }

    return 0;
}

#ifdef MIXSOUND_MIPS_MEASURE
void vAoutMIPSQuery(void);
void vAoutMIPSReset(void);

void vMixSndMIPSQuery(void);
void vMixSndMIPSReset(void);

static INT32 _AudCmdQueryMixSndClipMIPS(INT32 i4Argc, const CHAR ** szArgv)
{
    vAoutMIPSQuery();
    vMixSndMIPSQuery();
    return 0;
}

static INT32 _AudCmdResetMixSndClipMIPS(INT32 i4Argc, const CHAR ** szArgv)
{
    vAoutMIPSReset();
    vMixSndMIPSReset();
    return 0;
}
#endif

static INT32 _AudCmdClearMixSndClip(INT32 i4Argc, const CHAR ** szArgv)
{
    AUD_ClearMixSndClip();
    return 0;
}

extern BOOL fgMixSndClipMute;
static INT32 _AudCmdMuteMixSndClip(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 2)
    {
        Printf("mclip.m [fgMute]\n");
        Printf("  0: Unmute, 1: Mute\n");
        Printf("  Current Setting: %d\n",fgMixSndClipMute); 
    }
    else
    {
        fgMixSndClipMute = (BOOL)StrToInt(szArgv[1]);
    }
    return 0;
}

//POLAR_DEBUG
#ifdef MIXSND_OUTPUT_DBG
extern void vSetMixSndDumpInfo(UINT32 u4Addr, UINT32 u4Size);
static INT32 _AudCmdMixSndDumpAlloc(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Size = 0;
    static UINT8* pMemPlayBufNew = 0;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (i4Argc != 2)
    {
        Printf("Usage: mda [Length]\n");
        Printf(" Length = 0 for free\n");
        return -1;
    }

    u4Size = (UINT32)StrToInt(szArgv[1]);
    if (u4Size == 0)
    {
        VERIFY(x_mem_free((UINT8*)pMemPlayBufNew));
        Printf( "Free memory 0x%x \n",pMemPlayBufNew);
        pMemPlayBufNew = 0;
        return 0;
    }
    else
    {
        VERIFY(x_mem_free((UINT8*)pMemPlayBufNew));
        Printf( "Free memory 0x%x \n",pMemPlayBufNew);
    }

    Printf( "Allocate memory size 0x%x for memory play...\n",u4Size);
    pMemPlayBufNew = (UINT8 *)x_mem_alloc_virtual(u4Size);
    if (pMemPlayBufNew == NULL)
    {
        printf( "Memory request for memory play fail !!\n");
        return -1;
    }

    Printf( "Allocate memory from 0x%x to 0x%x Success, len = 0x%x\n",pMemPlayBufNew,pMemPlayBufNew+u4Size,u4Size);
    Printf( "data.load.binary FILENAME 0x%x--0x%x /ny /swap \n",pMemPlayBufNew,pMemPlayBufNew+u4Size);

    vSetMixSndDumpInfo((UINT32)pMemPlayBufNew, u4Size);
    return 0;
}

extern void vResetMixSndDumpInfo(void);
static INT32 _AudCmdMixSndDumpReset(INT32 i4Argc, const CHAR **szArgv)
{
    vResetMixSndDumpInfo();
    return 0;
}

extern UINT8 u1DumpLocation; // 0: Before SW ASRC 1: After SW ASRC
static INT32 _AudCmdMixSndDumpSet(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc != 2)
    {
        Printf("Usage: mds [location]\n");
        Printf("  0: before SW SRC, 1: after SW SRC\n");
        Printf("  Current Setting: %d\n", u1DumpLocation);
    }
    else
    {
        u1DumpLocation = ((UINT8)StrToInt(szArgv[1])>0);
        Printf("Set location = %d\n", u1DumpLocation);
    }
    return 0;
}
#endif
//POLAR_DEBUG

#endif

#endif

static INT32 _AudCmdClipPlay(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8  u1DecId, u1ClipType;
    AUD_CLIP_TYPE_T rAudClip;

    if (i4Argc < 6)
    {
        goto lbUsage;
    }

    x_memset((VOID *)VIRTUAL((UINT32)&rAudClip), 0, sizeof(AUD_CLIP_TYPE_T));

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    if (u1DecId >= AUD_DEC_NUM)
    {
        u1DecId = AUD_DEC_MAIN;
    }

    rAudClip.u4OutputChlSelect = (UINT32)StrToInt(szArgv[2]);
    rAudClip.u4RepeatCount = (UINT32)StrToInt(szArgv[3]);

    u1ClipType = (UINT8)StrToInt(szArgv[4]);
    if (u1ClipType == 0)
    {
        // Play test tone
        rAudClip.fgPlayTestTone = TRUE;
        rAudClip.u4ToneFreq = (UINT32)StrToInt(szArgv[5]);
    }
    else
    {
        if (i4Argc < 8)
        {
            goto lbUsage;
        }

        // Play memory
        rAudClip.fgPlayTestTone = FALSE;
        rAudClip.eMemBuf.pData = (UINT8*)StrToInt(szArgv[5]);
        rAudClip.eMemBuf.u4Length = (UINT32)StrToInt(szArgv[6]);
        rAudClip.eAudFmt = (AUD_FMT_T)StrToInt(szArgv[7]);
    }

    AUD_SetAudClip(u1DecId, &rAudClip);
    AUD_PlayAudClip(u1DecId, rAudClip.u4OutputChlSelect,
                    rAudClip.fgPlayTestTone);
    return 0;

lbUsage:
    Printf("Usage: play [Dec Id] [Channel] [Loop] [Type] [Frequence]\n");
    Printf("   or  play [Dec Id] [Channel] [Loop] [Type] [Addr] [Len] [FMT]\n");
    Printf("[Dec Id]     0:FIRST        1:SECOND \n");
    Printf("[Channel]    1:L, 2:R, 4:LS, 8:RS, 16:C, 32:SUBWOOFER\n");
    Printf("[Loop]       0: Infinite    ~ loop counter\n");
    Printf("[Type]       0:Test tone    1:memory\n");
    Printf("[Frequence]  0 ~ 20K \n");
    Printf("[FMT]        0:OTHERS 1:MPEG   2:AC3   3:PCM    4:MP3	5:AAC \n");
    return -1;
}

static INT32 _AudCmdClipStop(INT32 i4Argc, const CHAR ** szArgv)
{
    if(i4Argc == 1)
    {
        AUD_StopAudClip(AUD_DEC_MAIN);
    }
    else if (i4Argc == 2)
    {
        UINT8 u1DecId = (UINT8)StrToInt(szArgv[1]);

        if (u1DecId > 1)
        {
            goto lbUsage;
        }
        AUD_StopAudClip(u1DecId);
    }
    else
    {
        goto lbUsage;
    }

    return 1;
lbUsage:
    Printf("Usage: stop [decoder id]\n");
    Printf("[decoder id] 0:MAIN  1:AUX \n");
    return -1;
}

static INT32 _AudCmdClipVolume(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8  u1Vol;
    UINT8  u1Channel;
    UINT8  u1DecId;

    if (i4Argc != 3)
    {
        goto lbUsage;
    }
    u1DecId = AUD_DEC_MAIN;

    u1Channel = (UINT8)StrToInt(szArgv[1]);
    u1Vol = (UINT8)StrToInt(szArgv[2]);

    if (u1Vol > 30)
    {
        u1Vol = 30;
    }
    Aud_SetAudClipVolume(u1DecId, u1Channel, u1Vol);
    return 0;
lbUsage:
    Printf("Usage: volume [Channel] [Vol]\n");
    Printf("[Channel]    0:L, 1:R, 2:LS, 3:RS, 4:C, 5:SUBWOOFER\n");
    Printf("[Vol]        0 ~ 30 \n");
    Printf("                    \n");
    Printf("             0: -20 dB \n");
    Printf("             1: -19 dB \n");
    Printf("             ......    \n");
    Printf("            30: +10 dB \n");
    return -1;
}

static INT32 _AudCmdReadBytePhysical(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4PhysicalAddr;
    UINT32 u4Length;
    UINT32 i;

    if (i4Argc == 2)
    {
        u4PhysicalAddr = StrToInt(szArgv[1]);
        u4Length = 1;
    }
    else if (i4Argc == 3)
    {
        u4PhysicalAddr = StrToInt(szArgv[1]);
        u4Length = StrToInt(szArgv[2]);

        if (u4Length == 0)
        {
            u4Length = 1;
        }
    }
    else
    {
        goto lbUsage;
    }

    for (i=0;i<u4Length;i++)
    {
        if ((i&0xf) == 0)
        {
            Printf("\n0x%08x: ", u4PhysicalAddr + i);
        }
        Printf("%02x ", *((UINT8*)(VIRTUAL(u4PhysicalAddr + i))));
    }
    Printf("\n");
    return 0;

lbUsage:

    Printf("Usage: r [physical address] [len]\n");
    Printf("  [len] is optional, no specify => 1\n");
    return -1;
}

extern void DSP_FlushInvalidateDCacheFree(UINT32 u4Addr, UINT32 u4Len);
static INT32 _AudCmdFlushDspMemory(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4PhysicalAddr;
    UINT32 u4Length;
    //UINT32 i;

    if (i4Argc == 2)
    {
        u4PhysicalAddr = StrToInt(szArgv[1]);
        u4Length = 1;
    }
    else if (i4Argc == 3)
    {
        u4PhysicalAddr = StrToInt(szArgv[1]);
        u4Length = StrToInt(szArgv[2]);

        if (u4Length == 0)
        {
            u4Length = 1;
        }
    }
    else
    {
        goto lbUsage;
    }

    DSP_FlushInvalidateDCacheFree(u4PhysicalAddr, u4Length);
    return 0;

lbUsage:

    Printf("Usage: flush [physical address] [len]\n");
    Printf("  [len] is optional, no specify => 1\n");
    return -1;
}

#ifdef LINUX_TURNKEY_SOLUTION
#ifdef __KERNEL__
static INT32 _AudCmdDumpFifo2File(INT32 i4Argc, const CHAR ** szArgv)
{
	CHAR * fn;
	UINT32 addr;
	UINT32 size;
	INT32 ret;
	mm_segment_t oldfs;
	struct file * filep = NULL;

	UINT8  decId;
	UINT32 u4RpDec;
	UINT32 u4WpDec;
	UINT32 u4DataSZDec;
	UINT32 u4FifoStr;
	UINT32 u4FifoEnd;
	
    if (i4Argc == 4)
    {
    	fn = (CHAR *)szArgv[1];
		addr = (UINT32)StrToInt(szArgv[2]);
		size = (UINT32)StrToInt(szArgv[3]);

		Printf("filename: %s  addr: 0x%x  size: 0x%x\n\n", fn, addr, size);
		
		oldfs = get_fs();
		set_fs(KERNEL_DS);
		filep = filp_open(fn,(O_CREAT | O_WRONLY | O_APPEND),0);
		if((IS_ERR(filep)) || (NULL == filep))
		{
			set_fs(oldfs);
			Printf("_AudCmdDump() create file failed.\n");
			return -1;
		}
		ret = (INT32)filep->f_op->write(filep, (const char *)addr, (size_t)size, &filep->f_pos);

		filp_close(filep, 0);
		set_fs(oldfs);
    }
	else if(i4Argc == 3)
	{
		fn = (CHAR *)szArgv[1];
		decId = (UINT8)StrToInt(szArgv[2]);

		if(decId > AUD_DEC_4TH)
		{
			goto lbUsage;
		}

		AUD_GetRWPtr(decId, &u4RpDec, &u4WpDec, &u4DataSZDec);
		UNUSED(AUD_GetAudFifo(decId, &u4FifoStr, &u4FifoEnd));

		u4RpDec = VIRTUAL(u4RpDec);
		u4WpDec = VIRTUAL(u4WpDec);
		u4FifoStr = VIRTUAL(u4FifoStr);
		u4FifoEnd = VIRTUAL(u4FifoEnd);

		oldfs = get_fs();
		set_fs(KERNEL_DS);
		filep = filp_open(fn,(O_CREAT | O_WRONLY | O_APPEND),0);
		if((IS_ERR(filep)) || (NULL == filep))
		{
			set_fs(oldfs);
			Printf("_AudCmdDump() create file failed.\n");
			return -1;
		}
		ret = (INT32)filep->f_op->write(filep, (const char *)u4WpDec, (size_t)(u4FifoEnd - u4WpDec), &filep->f_pos);
		ret = (INT32)filep->f_op->write(filep, (const char *)u4FifoStr, (size_t)(u4WpDec - u4FifoStr), &filep->f_pos);

		filp_close(filep, 0);
		set_fs(oldfs);
		
	}
	else
	{
		goto lbUsage;
	}

    return 1;

lbUsage:
    Printf("Usage1: dump [filename] [startaddr] [size]\n");
    Printf("[filename]  ex:\"/tmp/AudioTrack.bin\"\n");
	Printf("[startaddr] Virtual Addr\n");
	Printf("[size]      data Size\n\n");

	Printf("Usage2: dump [filename] [decId]\n");
	Printf("[filename]  ex:\"/tmp/AudDecFifo.bin\"\n");
	Printf("[decId] 0: AUD_DEC_MAIN  1: AUD_DEC_AUX  2: AUD_DEC_THIRD  3: AUD_DEC_4TH\n");
	
    return -1;
}

static INT32 _AudCmdUlFile2Dram(INT32 i4Argc, const CHAR ** szArgv)
{
	CHAR * fn = NULL;
	UINT32 addr = 0;
	UINT32 size = 0;
	INT32 ret;
	mm_segment_t oldfs;
	struct file * filep = NULL;

	if(i4Argc == 3)
	{
		fn = (CHAR *)szArgv[1];
		addr = (UINT32)StrToInt(szArgv[2]);

		oldfs = get_fs();
		set_fs(KERNEL_DS);
		filep = filp_open(fn,(O_RDONLY),0);
		if((IS_ERR(filep)) || (NULL == filep))
		{
			set_fs(oldfs);
			Printf("_AudCmdUlFile2Dram() open file failed.\n");
			return -1;
		}
		while ((ret = (INT32)filep->f_op->read(filep, (char *)(addr + size), 4096, &filep->f_pos)) != 0)
        {
            size += ret;
        }
        Printf("_AudCmdUlFile2Dram() from file %s to Dram Addr: 0x%08x Size: %x\n", fn, addr, size);

		filp_close(filep, 0);
		set_fs(oldfs);
	}
	else
	{
		goto lbUsage;
	}

    return 1;

lbUsage:
    Printf("ul [filename] [startaddr]\n");
    Printf("  [filename]  ex:\"/tmp/AudioTrack.bin\"\n");
	Printf("  [startaddr] Virtual Addr\n");

    return -1;
}

static INT32 _AudCmdDramAlloc(INT32 i4Argc, const CHAR ** szArgv)
{
    static UINT32 u4MalBufAddr = 0;
	static UINT32 u4MalBufSize = 0;

	if(i4Argc == 2)
	{
		u4MalBufSize = (UINT32)StrToInt(szArgv[1]);
        if(u4MalBufAddr)
        {
            x_mem_free((VOID*)u4MalBufAddr);
            Printf( "Free memory 0x%x size: 0x%x\n",u4MalBufAddr, u4MalBufSize);
            u4MalBufAddr = 0;
        }

        if (u4MalBufSize)
        {
            u4MalBufAddr = (UINT32)x_mem_alloc_virtual(u4MalBufSize);
            if (u4MalBufAddr == 0)
            {
                printf( "Allocate memory size: 0x%x fail!!\n", u4MalBufSize);
                return -1;
            }
            else
            {
                Printf( "Allocate memory addr: 0x%x size: 0x%x for dram upload ...\n",u4MalBufAddr, u4MalBufSize);            
            }
        }
	}
	else
	{
		goto lbUsage;
	}

    return 1;

lbUsage:
    Printf("# Memory Allocate Virtual Addr Continuous Dram\n");
    Printf("mal [u4MalBufSize]\n");
    Printf("  u4MalBufSize = 0 for free\n");
    Printf("  Current setting: u4MalBufAddr: 0x%x u4MalBufSize: 0x%x\n", u4MalBufAddr, u4MalBufSize);
    return -1;
}
#endif
#endif

#ifdef LINUX_TURNKEY_SOLUTION
extern int StartAudDataDump(UINT32 u1DecId, char* fileName);
extern void StopAudDataDump(UINT32 u1DecId);
extern void AudDataDumpQueryInfo(void);
extern BOOL GetAudDataDumpAutoSave(UINT32 u1DecId);
extern void SetAudDataDumpAutoSave(UINT32 u1DecId, BOOL bAutoSave);

static INT32 _AudCmdDumpFifo2FileInfinit(INT32 i4Argc, const CHAR ** szArgv)
{ 
	CHAR * fileName;
	UINT8  u1DecId;

    if (i4Argc == 3)
    {
		u1DecId = (UINT32)StrToInt(szArgv[1]);
    	fileName = (CHAR *)szArgv[2];
		StartAudDataDump(u1DecId, fileName);
    }
	else if(i4Argc == 2)
	{
        if (szArgv[1][0] == 'q')
        {
            AudDataDumpQueryInfo();
        }
        else
        {
    		u1DecId = (UINT32)StrToInt(szArgv[1]);
    		StartAudDataDump(u1DecId, "/mnt/usb/sda1/AudDecFifo.bin");
        }
	}
	else
	{
		goto lbUsage;
	}

    return 1;

lbUsage:
    Printf("Usage1: dumpLoop [decId] [filename]\n");
	Printf("[decId] 0: AUD_DEC_MAIN  1: AUD_DEC_AUX  2: AUD_DEC_THIRD  3: AUD_DEC_4TH\n");	
    Printf("[filename]  ex:\"/mnt/usb/sda1/AudDecFifo.bin\"\n\n");

	Printf("Usage2: dumpLoop [decId]\n");
	Printf("[decId] 0: AUD_DEC_MAIN  1: AUD_DEC_AUX  2: AUD_DEC_THIRD  3: AUD_DEC_4TH\n");
	Printf("Using default [fileName]=/mnt/usb/sda1/AudDecFifo.bin\n\n");

    Printf("Usage3: dumpLoop q\n");

    return -1;
}

static INT32 _AudCmdDumpFifo2FileInfinitStop(INT32 i4Argc, const CHAR ** szArgv)
{ 
	UINT8  u1DecId;
	
    if (i4Argc == 2) 
	{
		u1DecId = (UINT32)StrToInt(szArgv[1]);
		StopAudDataDump(u1DecId);
	} 
	else 
	{
		goto lbUsage;
	}
    return 1;

lbUsage:
    Printf("Usage1: dumpStop [decId]\n");
	Printf("[decId] 0: AUD_DEC_MAIN  1: AUD_DEC_AUX  2: AUD_DEC_THIRD  3: AUD_DEC_4TH\n");	
    return -1;
}


static INT32 _AudCmdDumpFifo2FileAutoSave(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1DecId;
    UINT8 i;
    BOOL bAutoSave;

    if (i4Argc == 2)
    {
        if (szArgv[1][0] == 'q')
        {
            Printf("Current dump auto save settings:\n");
            for (i = 0; i < AUD_DEC_NUM; i++)
            {
                Printf(" #DEC%d: %s\n", i, (GetAudDataDumpAutoSave(i) ? "true" : "false"));
            }
            Printf("\n");
            return 0;
        }
    }
    else if (i4Argc == 3)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
        bAutoSave = (BOOL)StrToInt(szArgv[2]);
        SetAudDataDumpAutoSave(u1DecId, bAutoSave);
        return 0;
    }

    Printf("Usage: dumpAS [decId] [autoSave] or dumpAS q\n");
    Printf("[decId] 0: AUD_DEC_MAIN  1: AUD_DEC_AUX  2: AUD_DEC_THIRD  3: AUD_DEC_4TH\n");
    Printf("[autoSave] 1: true  0: false\n");
    return -1;
}
#endif

static INT32 _AudCmdReadDwrdPhysical(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4PhysicalAddr;
    UINT32 u4Length;
    UINT32 i;

    if (i4Argc == 2)
    {
        u4PhysicalAddr = StrToInt(szArgv[1]);
        u4Length = 4;

        if ((u4PhysicalAddr&0x3) != 0)
        {
            goto lbUsage;
        }
    }
    else if (i4Argc == 3)
    {
        u4PhysicalAddr = StrToInt(szArgv[1]);
        u4Length = StrToInt(szArgv[2]);

        if ((u4PhysicalAddr&0x3) != 0)
        {
            goto lbUsage;
        }

        if (u4Length < 4)
        {
            u4Length = 4;
        }
    }
    else
    {
        goto lbUsage;
    }

    for (i=0;i<u4Length;i+=4)
    {
        if ((i&0xf) == 0)
        {
            Printf("\n0x%08x: ", u4PhysicalAddr + i);
        }
        Printf("%08x ", *((UINT32*)(VIRTUAL(u4PhysicalAddr + i))));
    }
    Printf("\n");
    return 0;

lbUsage:

    Printf("Usage: r4 [physical address] [len]\n");
    Printf("  [physical address] must be 4 bytes aligned\n");
    Printf("  [len] is optional, no specify => 4\n");
    return -1;
}

static INT32 _AudCmdGetPhyAddr(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VirAddr;
    UINT32 u4PhyAddr;

    if (i4Argc != 2)
    {
        Printf("Usage: phy [vir addr]\n");
        return -1;
    }

    u4VirAddr = StrToInt(szArgv[1]);
    u4PhyAddr = PHYSICAL(u4VirAddr);

    Printf("0x%08x(V) ==> 0x%08x(P)\n", u4VirAddr, u4PhyAddr);
    return 0;
}

static INT32 _AudCmdGetVirAddr(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VirAddr;
    UINT32 u4PhyAddr;

    if (i4Argc != 2)
    {
        Printf("Usage: vir [phy addr]\n");
        return -1;
    }

    u4PhyAddr = StrToInt(szArgv[1]);
    u4VirAddr = VIRTUAL(u4PhyAddr);

    Printf("0x%08x(P) ==> 0x%08x(V)\n", u4PhyAddr, u4VirAddr);
    return 0;
}

#if defined(__linux__)
static INT32 _AudCmdIsDecoderPlay(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Id;
    if (i4Argc != 2)
    {
        return -1;
    }
    u4Id = StrToInt(szArgv[1]);
    if(u4Id >= 2)
    {
        return -1;
    }
    return (INT32)AUD_IsDecoderPlay(u4Id);
}
static INT32 _AudCmdUopBalance(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Id, u4Val;
    if (i4Argc != 3)
    {
        return -1;
    }
    u4Id = StrToInt(szArgv[1]);
    if(u4Id >= 2)
    {
        return -1;
    }
    u4Val = StrToInt(szArgv[2]);
    AUD_DspBalance(u4Id, u4Val);
    return 0;
}
#endif

#if 1 // PROBE_RELATED_API
static INT32 _AudCmdProbePause(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
    Printf("Probe Pause");
    AUD_DspProbePause();
    return 0;
}

static INT32 _AudCmdProbeStep(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Count;
    if (i4Argc != 2)
    {
        Printf("Usage: Step [#]\n");
        Printf("#: step number 1~255\n");
        return -1;
    }
    u4Count = StrToInt(szArgv[1]);
    if(u4Count >= 255)
    {
        Printf("Usage: Step [#]\n");
        Printf("#: step number 1~255\n");
        return -1;
    }
    Printf("Probe Step %d\n", u4Count);
    AUD_DspProbeStep(u4Count);
    return 0;
}

static INT32 _AudCmdProbeGo(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
    Printf("Probe Go\n");
    AUD_DspProbeGo();
    return 0;
}

static INT32 _AudCmdProbeWrite(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Addr,u4Data;
    UINT8 fgPreviousPause; //ZY_FPGA

    if (i4Argc != 3)
    {
        Printf("Usage: Write [Addr] [Data]\n");
        Printf("Addr: SRAM address for Write\n");
        Printf("Data: SRAM data for Write\n");
        return -1;
    }

#if 1
    //ZY_FPGA
    fgPreviousPause = fgAUD_DspProbeHalt();
    if (!fgPreviousPause)
    {
        AUD_DspProbePause();
    }
    //ZY_FPGA
#else
    if (!fgAUD_DspProbeHalt())
    {
        Printf("DSP is not in halt state\n");
        Printf("Please execute [Pause] in advance\n");
        return -1;
    }
#endif

    u4Addr = StrToInt(szArgv[1]);
    u4Data = StrToInt(szArgv[2]);
    AUD_DspProbeWrite(u4Addr,u4Data);

	//ZY_FPGA
    if (!fgPreviousPause)
    {
        AUD_DspProbeGo();
    }
	//ZY_FPGA

    return 0;
}
#ifdef DSP_SUPPORT_SRSWOW
static INT32 _AudCmdUopSRSWOW(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;

    if (i4Argc < 2)
    {
        Printf("Usage: srswow  [mode]\n");
        Printf("[mode] 0: off 1: on\n");
        return -1;
    }

    u1Mode = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetSRSWOW(u1Mode);
    return 1;
}
#endif
static INT32 _AudCmdProbeRead(INT32 i4Argc, const CHAR **szArgv)
{
#if 0
    UINT32 u4Id, u4Val;
    if (i4Argc != 3)
    {
        return -1;
    }
    u4Id = StrToInt(szArgv[1]);
    if(u4Id >= 2)
    {
        return -1;
    }
    u4Val = StrToInt(szArgv[2]);
#endif
    UINT32 u4Addr;
    UINT16  u2Length;
    UINT16  u2Idx = 0;
    UINT8 fgPreviousPause; //ZY_FPGA

    if ((i4Argc != 2)&&(i4Argc != 3))
    {
        Printf("Usage: Read [Addr] ([Length]) \n");
        Printf("Addr: SRAM address for read\n");
        return -1;
    }

    #if 1
    //ZY_FPGA
    fgPreviousPause = fgAUD_DspProbeHalt();
    if (!fgPreviousPause)
    {
        AUD_DspProbePause();
    }
    //ZY_FPGA
    #else
    if (!fgAUD_DspProbeHalt())
    {
        Printf("DSP is not in halt state\n");
        Printf("Please execute [Pause] in advance\n");
        return -1;
    }
    #endif

    u4Addr = StrToInt(szArgv[1]);
    if (i4Argc == 2)
    {
        #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)) //ZY_FPGA
        Printf("0x%04x 0x%08x\n",u4Addr,u4AUD_DspProbeRead(u4Addr));
        #else
        Printf("Probe Address: 0x%04x Data: 0x%06x\n",u4Addr,u4AUD_DspProbeRead(u4Addr));
        #endif
    }
    else if (i4Argc == 3)
    {
        u2Length = StrToInt(szArgv[2]);
        if (u2Length > 0x100)
        {
            u2Length = 0x100;
        }
        Printf("Probe Address: 0x%x, length = %x \n",u4Addr,u2Length);
        for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
        {
            if ((u2Idx % 4) == 0)
            {
                Printf("\n0x%04x | ",u4Addr+u2Idx);
            }
            #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)) //ZY_FPGA
            Printf("%08x ",u4AUD_DspProbeRead(u4Addr+u2Idx));
            #else
            Printf("0x%06x ",u4AUD_DspProbeRead(u4Addr+u2Idx));
            #endif
        }
        Printf("\n");
    }

    //ZY_FPGA
    if (!fgPreviousPause)
    {
        AUD_DspProbeGo();
    }
    //ZY_FPGA

    return 0;
}

//ZY_FPGA
static INT32 _AudCmdProbeSramTest(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Addr;
    UINT16 u2Length;
    UINT16 u2Idx = 0;
    UINT8 u1PreviousHalt;
    UINT32 u4Val;

    if (i4Argc != 3)
    {
        Printf("Usage: sramtest [Addr] [Length]\n");
        return -1;
    }

    u1PreviousHalt = fgAUD_DspProbeHalt();
    if (!u1PreviousHalt)
    {
        AUD_DspProbePause();
    }

    u4Addr = StrToInt(szArgv[1]);
    u2Length = StrToInt(szArgv[2]);

    Printf("Sram Test - 0x%04x ~ 0x%04x\n", u4Addr, u4Addr + u2Length - 1);
    for (u2Idx=0;u2Idx < u2Length;u2Idx++)
    {
        AUD_DspProbeWrite(u4Addr + u2Idx, 0xa5a5a5a5);
    }

    for (u2Idx=0;u2Idx < u2Length;u2Idx++)
    {
        u4Val = u4AUD_DspProbeRead(u4Addr + u2Idx);
        if (u4Val != 0xa5a5a5a5)
        {
            Printf("Error addr: 0x%04x - 0x%08x<=>0xa5a5a5a5\n", u4Addr + u2Idx, u4Val);
        }
    }

    for (u2Idx=0;u2Idx < u2Length;u2Idx++)
    {
        AUD_DspProbeWrite(u4Addr + u2Idx, 0x5a5a5a5a);
    }

    for (u2Idx=0;u2Idx < u2Length;u2Idx++)
    {
        u4Val = u4AUD_DspProbeRead(u4Addr + u2Idx);
        if (u4Val != 0x5a5a5a5a)
        {
            Printf("Error addr: 0x%04x - 0x%08x<=>0x5a5a5a5a\n", u4Addr + u2Idx, u4Val);
        }
    }

    if (!u1PreviousHalt)
    {
        AUD_DspProbeGo();
    }

    return 0;
}

extern void vResetDSP(void);
static INT32 _AudCmdProbeReset(INT32 i4Argc, const CHAR **szArgv)
{
    Printf("Reset DSP\n");
    vResetDSP();
    return 0;
}

extern void AUD_QueryDspMemory(UINT8 u1MemSpace);
static INT32 _AudCmdQueryDspMemory(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc != 2)
    {
        Printf("qm [space id]\n");
        Printf("  0 - dram map\n"
               "  1 - dsp code\n"
               "  2 - normal\n"
               "  3 - compact\n"
               "  4 - afifo\n"
               "  5 - intbuf\n"
               "  6 - asrc\n"
               "  7 - aout\n"
               "  8 - upload\n"
               "  9 - mixsnd\n"
               );
        return -1;
    }
    AUD_QueryDspMemory((UINT8)StrToInt(szArgv[1]));
    return 0;
}

extern void AUD_DumpDspMemory(UINT8 u1Option, UINT32 u4Size);
static INT32 _AudCmdDumpDspMemory(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Option;
    UINT32 u4Size;
    if ((i4Argc != 2) && (i4Argc != 3))
    {
        Printf("dm [option] [size]\n");
        Printf("  0 - afifo1\n"
               "  1 - afifo2\n"
               "  2 - afifo3\n"
               "  3 - afifo4\n"
               "  4 - upload\n"
               "  5 - alsa buffer\n"
               "  6 - mixclip buffer (alsa)\n"
               );
        return -1;
    }

    u1Option = (UINT8)StrToInt(szArgv[1]);    
    if (i4Argc == 3)
    {
        u4Size = (UINT32)StrToInt(szArgv[2]);
    }
    else
    {
        u4Size = 0x100;
    }
        
    AUD_DumpDspMemory(u1Option, u4Size);
    return 0;
}

#if ((defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)) && !defined(CC_AUD_DISABLE_DSP_PAT))
extern void AUD_DspPatternTest(UINT8 u1PatId,UINT8 u1ShowHelp);
static INT32 _AudCmdPatTest(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1PatId;
    if (i4Argc != 2)
    {
        AUD_DspPatternTest(0,1);
        return -1;
    }

    u1PatId = (UINT8)StrToInt(szArgv[1]);
    AUD_DspPatternTest(u1PatId,1);
    return 0;
}

extern void AUD_DspPatternAutoTest(void);
static INT32 _AudCmdPatAuto(INT32 i4Argc, const CHAR **szArgv)
{
    AUD_DspPatternAutoTest();
    return 0;
}

#if 0
extern void AUD_FillAFIFO1Buffer(UINT8 u1Case,UINT32 u4Interval);
static INT32 _AudCmdFillBuf(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Case;
    UINT32 u4Interval;
    if (i4Argc != 3)
    {
        Printf("fillbuf [case] [interval]\n");
        Printf("  0: CH1~10 = 1K,2K,3K,4K,6K\n");
        Printf("  1: CH1=11h, CH2=22h, other=00h\n");
        Printf("  2: AC3 Raw (HBR=0,long-det=0,ch=2)\n");
        Printf("  4: E-AC3 Raw (HBR=1,long-det=1,ch=8)\n");
        Printf("  5: DTS-16 (HBR=0,long-det=0,ch=2)\n");
        Printf("  6: TTS-14 (HBR=0,long-det=0,ch=2)\n");
        return -1;
    }

    u1Case = (UINT8)StrToInt(szArgv[1]);
    u4Interval = (UINT32)StrToInt(szArgv[2]);

    AUD_FillAFIFO1Buffer(u1Case,u4Interval);
    return 0;
}
#endif

#endif
//ZY_FPGA

static INT32 _AudCmdShowRegs(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 i;
    UINT32 u4temp1,u4temp2;
    if (!fgAUD_DspProbeHalt())
    {
        Printf("DSP is not in halt state\n");
        Printf("Please execute [Pause] in advance\n");
        return -1;
    }

#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
	u4temp1= u4AUD_DspProbeRead(0x80)>>16;
	u4temp2= u4AUD_DspProbeRead(0x81)>>16;
    Printf("ACC: %08x %08x PREG: %08x %08x TREG0: %08x TREG1: %04x\n",
		   u4AUD_DspProbeRead(0x89),u4AUD_DspProbeRead(0x88), // ACC
		   u4AUD_DspProbeRead(0x8d),u4AUD_DspProbeRead(0x8c), // PREG
		   (u4AUD_DspProbeRead(0x0c)),(u4AUD_DspProbeRead(0x0d)>>16) // TREG0,TREG1
		   );
    Printf("ACCB: %08x %08x TREG2: %04x ARP: AR%x OV: %x OVM: %x INTM: %x DP: %04x\n",
		   u4AUD_DspProbeRead(0x8b),u4AUD_DspProbeRead(0x8a), // ACCB
		   (u4AUD_DspProbeRead(0x0e)>>16), // TREG2
		   ((u4temp1>>13)&0x7), ((u4temp1>>12) & 0x1), // ARx, OV
		   ((u4temp1>>11) &0x1),((u4temp1>>9) & 0x1), //OVM, INTM
		   (u4temp1 & 0x1ff) // DP
		   );
	Printf("ST0: %04x ST1: %04x TC: %x SXM: %x C: %x PM: %x DP(shifted): %06x\n",
		   u4temp1,u4temp2, // ST0 , ST1
		   ((u4temp2>>11) & 0x1),((u4temp2)>>10 & 0x1), //TC, SXM
		   ((u4temp2>>9) & 0x1), ((u4temp2) & 0x3), // C,PM
		   ((u4temp1 & 0x1ff)<<7)
		   );
	for(i=0;i<8;i++)
	{
		if(i==4) Printf("\n");
        Printf(" AR%d: %04x ",i, (u4AUD_DspProbeRead(i+0x10)>>16));
	}
	Printf("\n");
	for(i=0;i<8;i++)
	{
		if(i==4) Printf("\n");
        Printf(" V%d: %08x ",i, u4AUD_DspProbeRead(i+0x60));
	}
	Printf("\n");
	Printf("IMR: %04x IFR: %04x PMST: %04x RPTC: %04x BRCR: %04x PASR: %04x\n",
			(u4AUD_DspProbeRead(0x4)>>16),(u4AUD_DspProbeRead(0x6)>>16),// IMR,IFR
			(u4AUD_DspProbeRead(0x7)>>16),(u4AUD_DspProbeRead(0x8)>>16),// PMST,RPTC
			(u4AUD_DspProbeRead(0x9)>>16),(u4AUD_DspProbeRead(0xa)>>16)// BRCR,DBMR
		   );
	Printf("PAER: %04x DBMR: %04x INDX: %04x ARCR: %04x CBSR1: %04x CBER1: %04x\n",
			(u4AUD_DspProbeRead(0xb)>>16),(u4AUD_DspProbeRead(0xf)>>16),// PAER,DBMR
			(u4AUD_DspProbeRead(0x18)>>16),(u4AUD_DspProbeRead(0x19)>>16),// INDX,ARCR
			(u4AUD_DspProbeRead(0x1a)>>16),(u4AUD_DspProbeRead(0x1b)>>16)// CBSR1,CBER1
		   );
	Printf("CBSR2: %04x CBER2: %04x CBCR: %04x BMAR: %04x TFR: %04x TLR1: %04x\n",
			(u4AUD_DspProbeRead(0x1c)>>16),(u4AUD_DspProbeRead(0x1d)>>16),// CBSR2,CBER2
			(u4AUD_DspProbeRead(0x1e)>>16),(u4AUD_DspProbeRead(0x1f)>>16),// CBCR,BMAR
			(u4AUD_DspProbeRead(0x23)>>16),(u4AUD_DspProbeRead(0x24)>>16)// TFR,TLR1
		   );
	Printf("TFR: %04x TLR1: %04x TCR1: %04x TLR2: %04x TCR2: %04x RANDOM: %04x\n",
			(u4AUD_DspProbeRead(0x23)>>16),(u4AUD_DspProbeRead(0x24)>>16),// TFR,TLR1
			(u4AUD_DspProbeRead(0x25)>>16),(u4AUD_DspProbeRead(0x26)>>16),// TCR1,TLR2
			(u4AUD_DspProbeRead(0x27)>>16),(u4AUD_DspProbeRead(0x62)>>16)// TCR2,RANDOM
		   );
	Printf("PC: %04x MCS: %04x insd_d: %08x ld_d: %08x\n",
			u4AUD_DspProbeRead(0xa0),u4AUD_DspProbeRead(0xa2),// PC, MCS
			u4AUD_DspProbeRead(0xa8),u4AUD_DspProbeRead(0xa9)// ins_d,ld_d
		   );

    Printf("MMR(3Eh): %08x\n", u4AUD_DspProbeRead(0x3e));
    Printf("MMR(6Dh): %08x\n", u4AUD_DspProbeRead(0x6d));

    {
        UINT32 MONI_DSP_2 = u4AUD_DspProbeRead(0xac);
        Printf("ST0 - [15:13]ARP [12]OV [11]OVM [9]INTM [8:0]DP\n");
        Printf("ST1 - [15:13]ARB [12]CNF [11]TC [10]SXM [9]C [6]HM [4]XF [1:0]PM\n");
        Printf("MONI_DSP_2: %04x\n", MONI_DSP_2);
        Printf("  [15] ic_rd: %d [14] undef_ins_d: %d\n"
			"  [13] wdcmpt_cmd&wdcmpt_full: %d [12] wdram_cmd&wdram_full: %d\n"
			"  [11] ins_read: %d [10] dummy_wr: %d\n"
			"  [9] dummy_nw: %d [8] dummy_rd: %d\n"
			"  [7] dummy_d: %d [6] hold_met_wr: %d\n"
			"  [5] hold_rmpfc: %d [4] hold_getbs: %d\n"
			"  [3] hold_ld: %d [2] hold_e: %d\n"
			"  [1] hold_m: %d [0] hold_d: %d\n",
			((MONI_DSP_2>>15)&0x1), ((MONI_DSP_2>>14)&0x1), ((MONI_DSP_2>>13)&0x1), ((MONI_DSP_2>>12)&0x1),
			((MONI_DSP_2>>11)&0x1), ((MONI_DSP_2>>10)&0x1), ((MONI_DSP_2>>9)&0x1), ((MONI_DSP_2>>8)&0x1),
			((MONI_DSP_2>>7)&0x1), ((MONI_DSP_2>>6)&0x1), ((MONI_DSP_2>>5)&0x1), ((MONI_DSP_2>>4)&0x1),
			((MONI_DSP_2>>3)&0x1), ((MONI_DSP_2>>2)&0x1), ((MONI_DSP_2>>1)&0x1), ((MONI_DSP_2>>0)&0x1)
			);
    }

#else

    u4temp1= u4AUD_DspProbeRead(0x80)>>8;
    u4temp2= u4AUD_DspProbeRead(0x81)>>8;
    Printf("ACC: %06x%06x PREG: %06x%06x TREG0: %04x TREG1: %04x\n",
           u4AUD_DspProbeRead(0x89),u4AUD_DspProbeRead(0x88), // ACC
           u4AUD_DspProbeRead(0x8d),u4AUD_DspProbeRead(0x8c), // PREG
           (u4AUD_DspProbeRead(0x0c)>>8),(u4AUD_DspProbeRead(0x0d)>>8) // TREG0,TREG1
           );
    Printf("ACCB: %06x%06x TREG2: %04x ARP: AR%x OV: %x OVM: %x INTM: %x DP: %04x\n",
           u4AUD_DspProbeRead(0x8b),u4AUD_DspProbeRead(0x8a), // ACCB
           (u4AUD_DspProbeRead(0x0e)>>8), // TREG2
           ((u4temp1>>13)&0x7), ((u4temp1>>12) & 0x1), // ARx, OV
           ((u4temp1>>11) &0x1),((u4temp1>>9) & 0x1), //OVM, INTM
           (u4temp1 & 0x1ff) // DP
           );
    Printf("ST0: %04x ST1: %04x TC: %x SXM: %x C: %x PM: %x DP(shifted): %06x\n",
           u4temp1,u4temp2, // ST0 , ST1
           ((u4temp2>>11) & 0x1),((u4temp2)>>10 & 0x1), //TC, SXM
           ((u4temp2>>9) & 0x1), ((u4temp2) & 0x3), // C,PM
           ((u4temp1 & 0x1ff)<<7)
           );
    for(i=0;i<8;i++)
    {
        if(i==4) Printf("\n");
        Printf(" AR%d: %06x ",i, (u4AUD_DspProbeRead(i+0x10)>>8));
    }
    Printf("\n");
    for(i=0;i<8;i++)
    {
        if(i==4) Printf("\n");
        Printf(" V%d: %06x ",i, u4AUD_DspProbeRead(i+0x60));
    }
    Printf("\n");
    Printf("IMR: %04x IFR: %04x PMST: %04x RPTC: %04x BRCR: %04x PASR: %04x\n",
            (u4AUD_DspProbeRead(0x4)>>8),(u4AUD_DspProbeRead(0x6)>>8),// IMR,IFR
            (u4AUD_DspProbeRead(0x7)>>8),(u4AUD_DspProbeRead(0x8)>>8),// PMST,RPTC
            (u4AUD_DspProbeRead(0x9)>>8),(u4AUD_DspProbeRead(0xa)>>8)// BRCR,DBMR
           );
    Printf("PAER: %04x DBMR: %04x INDX: %04x ARCR: %04x CBSR1: %04x CBER1: %04x\n",
            (u4AUD_DspProbeRead(0xb)>>8),(u4AUD_DspProbeRead(0xf)>>8),// PAER,DBMR
            (u4AUD_DspProbeRead(0x18)>>8),(u4AUD_DspProbeRead(0x19)>>8),// INDX,ARCR
            (u4AUD_DspProbeRead(0x1a)>>8),(u4AUD_DspProbeRead(0x1b)>>8)// CBSR1,CBER1
           );
    Printf("CBSR2: %04x CBER2: %04x CBCR: %04x BMAR: %04x TFR: %04x TLR1: %04x\n",
            (u4AUD_DspProbeRead(0x1c)>>8),(u4AUD_DspProbeRead(0x1d)>>8),// CBSR2,CBER2
            (u4AUD_DspProbeRead(0x1e)>>8),(u4AUD_DspProbeRead(0x1f)>>8),// CBCR,BMAR
            (u4AUD_DspProbeRead(0x23)>>8),(u4AUD_DspProbeRead(0x24)>>8)// TFR,TLR1
           );
    Printf("TFR: %04x TLR1: %04x TCR1: %04x TLR2: %04x TCR2: %04x RANDOM: %04x\n",
            (u4AUD_DspProbeRead(0x23)>>8),(u4AUD_DspProbeRead(0x24)>>8),// TFR,TLR1
            (u4AUD_DspProbeRead(0x25)>>8),(u4AUD_DspProbeRead(0x26)>>8),// TCR1,TLR2
            (u4AUD_DspProbeRead(0x27)>>8),(u4AUD_DspProbeRead(0x62)>>8)// TCR2,RANDOM
           );
    Printf("PC: %04x MCS: %04x insd_d: %08x ld_d: %08x\n",
            u4AUD_DspProbeRead(0xa0),u4AUD_DspProbeRead(0xa2),// PC, MCS
            u4AUD_DspProbeRead(0xa8),u4AUD_DspProbeRead(0xa9)// ins_d,ld_d
           );

    //ZY_FPGA
    {
        UINT32 MONI_DSP_2 = u4AUD_DspProbeRead(0xac);
        Printf("ST0 - [15:13]ARP [12]OV [11]OVM [9]INTM [8:0]DP\n");
        Printf("ST1 - [15:13]ARB [12]CNF [11]TC [10]SXM [9]C [6]HM [4]XF [1:0]PM\n");
        Printf("MONI_DSP_2: %04x\n", MONI_DSP_2);
        Printf("  [15] ic_rd: %d [14] undef_ins_d: %d\n"
			"  [13] wdcmpt_cmd&wdcmpt_full: %d [12] wdram_cmd&wdram_full: %d\n"
			"  [11] ins_read: %d [10] dummy_wr: %d\n"
			"  [9] dummy_nw: %d [8] dummy_rd: %d\n"
			"  [7] dummy_d: %d [6] hold_met_wr: %d\n"
			"  [5] hold_rmpfc: %d [4] hold_getbs: %d\n"
			"  [3] hold_ld: %d [2] hold_e: %d\n"
			"  [1] hold_m: %d [0] hold_d: %d\n",
			((MONI_DSP_2>>15)&0x1), ((MONI_DSP_2>>14)&0x1), ((MONI_DSP_2>>13)&0x1), ((MONI_DSP_2>>12)&0x1),
			((MONI_DSP_2>>11)&0x1), ((MONI_DSP_2>>10)&0x1), ((MONI_DSP_2>>9)&0x1), ((MONI_DSP_2>>8)&0x1),
			((MONI_DSP_2>>7)&0x1), ((MONI_DSP_2>>6)&0x1), ((MONI_DSP_2>>5)&0x1), ((MONI_DSP_2>>4)&0x1),
			((MONI_DSP_2>>3)&0x1), ((MONI_DSP_2>>2)&0x1), ((MONI_DSP_2>>1)&0x1), ((MONI_DSP_2>>0)&0x1)
			);
    }
    //ZY_FPGA

#endif
    return 0;
}

static INT32 _AudCmdProbeGoThenShow(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
    Printf("Probe Go\n");
    AUD_DspProbeGo();
    x_thread_delay(10);
    _AudCmdShowRegs(0,NULL);
    return 0;
}

static INT32 _AudCmdProbeStepThenShow(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Count;
    if (i4Argc != 2)
    {
        Printf("Usage: stsr [#]\n");
        Printf("#: step number 1~255\n");
        return -1;
    }
    u4Count = StrToInt(szArgv[1]);
    if(u4Count >= 255)
    {
        Printf("Usage: stsr [#]\n");
        Printf("#: step number 1~255\n");
        return -1;
    }
    Printf("Probe Step %d\n", u4Count);
    AUD_DspProbeStep(u4Count);
    x_thread_delay(10);
    _AudCmdShowRegs(0,NULL);
    return 0;
}

static INT32 _AudCmdSetBPts(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4BPt;
    UINT32 u4Addr;
    UINT32 u4temp1;
    if (i4Argc < 4)
    {
        Printf("Usage: %s [+/-] [#] [Addr] \n",szArgv[0]);
        Printf("[+/-]: [enable/disable]\n");
        Printf("[#]: 0~3 number of break point\n");
        Printf("[Addr]: address of break point\n");
        return -1;
    }
    u4BPt = (UINT32)StrToInt(szArgv[2]);
    u4Addr = (UINT32) StrToInt(szArgv[3]);
#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
    if((szArgv[1][0]!='+'&&szArgv[1][0]!='-') || u4BPt> 3|| (u4Addr>0x1FFFF))
#else
    if((szArgv[1][0]!='+'&&szArgv[1][0]!='-') || u4BPt> 3|| (u4Addr>0xFFFF))
#endif
    {
        Printf("Usage: %s [+/-] [#] [Addr] \n",szArgv[0]);
        Printf("[+/-]: [enable/disable]\n");
        Printf("[#]: 0~3 number of break point\n");
        Printf("[Addr]: address of break point\n");
        return -1;
    }
    u4temp1 = u4AUD_DspProbePBCfg(FALSE, 0x0);
    if(szArgv[1][0]=='+')
    {
        u4AUD_DspProbePB(TRUE, u4BPt,u4Addr);
        u4AUD_DspProbePBCfg(TRUE, (u4temp1|(UINT32)(0x1<<u4BPt)));
        return 0;
    }
    u4temp1=u4temp1&(~((UINT32)(0x1<<u4BPt)));
    u4AUD_DspProbePBCfg(TRUE, (u4temp1|(((UINT32)(0x1<<(u4BPt+8))))));
    return 0;
}

static INT32 _AudCmdShowBPts(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 i;
    UINT32 u4temp1;
    UNUSED(i4Argc);
    UNUSED(szArgv);
    u4temp1 = u4AUD_DspProbePBCfg(FALSE, 0x0);
    for(i=0;i<4;i++)
    {
    #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
        Printf("%d Address: %05x ",i,u4AUD_DspProbePB(FALSE,i,0x0));
    #else
        Printf("%d Address: %04x ",i,u4AUD_DspProbePB(FALSE,i,0x0));
    #endif
        if(u4temp1 & (1<<i))
        {
            Printf(" ENABLED\n");
        }
        else
        {
            Printf(" DISABLED\n");
        }
    }
    return 0;
}

static INT32 _AudCmdStopOnWrite(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Addr, u4Data, u4Mask;
    BOOL   fgEnable;

    UNUSED(i4Argc);
    UNUSED(szArgv);
    UNUSED(fgEnable);

    if ((i4Argc != 4)&&(i4Argc != 2) && (i4Argc != 1))
    {
        Printf("Usage: sw [Addr] [Data] [Mask] \n");
        Printf("Addr: SRAM address, 0 for clear\n");
        return -1;
    }

    if (i4Argc == 1)
    {
        Printf("Usage: sw [Addr] [Data] [Mask]\n");
        u4Addr = u4AUD_DspProbeChkStopOnWrite(&fgEnable, &u4Data, &u4Mask);
        if(u4Mask == 0)
        {
            Printf("Current : Enable %d, Addr 0x%x\n",fgEnable,u4Addr);
        }
        else
        {
            Printf("Current : Enable %d, Addr 0x%x, Data 0x%x, Mask 0x%x\n",fgEnable,u4Addr, u4Data, u4Mask);
        }
    }
    else if(i4Argc == 2)
    {
        u4Addr = StrToInt(szArgv[1]);
        u4Data = 0;
        u4Mask = 0;
        u4AUD_DspProbeStopOnWrite(u4Addr, u4Data, u4Mask);
    }
    else if(i4Argc == 4)
    {
        u4Addr = StrToInt(szArgv[1]);
        u4Data = StrToInt(szArgv[2]);
        u4Mask = StrToInt(szArgv[3]);
        u4AUD_DspProbeStopOnWrite(u4Addr, u4Data, u4Mask);
    }
    return 0;
}

#endif

#ifdef DSP_SUPPORT_SRSWOW
static INT32 _AudCmdUopSRSWOWSpkSz(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;
    /*
        Speaker size table: 0: 40Hz, 1: 60Hz, 2: 100Hz, 3: 150Hz,
                            4: 200Hz, 5: 250Hz, 6: 300Hz, 7: 400Hz
    */
    if (i4Argc < 2)
    {
        Printf("Usage: wspk  [mode]\n");
        Printf("[mode] 0:  40Hz \n");
        Printf("       1:  60Hz \n");
        Printf("       2: 100Hz \n");
        Printf("       3: 150Hz \n");
        Printf("       4: 200Hz \n");
        Printf("       5: 250Hz \n");
        Printf("       6: 300Hz \n");
        Printf("       7: 400Hz \n");
        return -1;
    }

    u1Mode = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetSRSWOWSpkSz(u1Mode);
    return 1;
}

static INT32 _AudCmdUopSRSWOWBright(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;

    if (i4Argc < 2)
    {
        Printf("Usage: wbrt  [mode]\n");
        Printf("[mode] 0:   0% \n");
        Printf("       1:  20% \n");
        Printf("       2:  40% \n");
        Printf("       3: 100% \n");
        return -1;
    }

    u1Mode = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetSRSWOWBright(u1Mode);
    return 1;
}

static INT32 _AudCmdUopSRSWOWTBLvl(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;

    if (i4Argc < 2)
    {
        Printf("Usage: wtb  [mode]\n");
        Printf("[mode] 0:   0% \n");
        Printf("       1:  50% \n");
        Printf("       2:  60% \n");
        Printf("       3: 100% \n");
        return -1;
    }

    u1Mode = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetSRSWOWTBLvl(u1Mode);
    return 1;
}

static INT32 _AudCmdUopSRSWOWWidth(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;

    if (i4Argc < 2)
    {
        Printf("Usage: wwid  [mode]\n");
        Printf("[mode] 0:   0% \n");
        Printf("       1:  30% \n");
        Printf("       2:  80% \n");
        Printf("       3: 100% \n");
        return -1;
    }

    u1Mode = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetSRSWOWWidth(u1Mode);
    return 1;
}
#endif
#ifdef DSP_SUPPORT_SRSTSXT
static INT32 _AudCmdUopSRSTSXTMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1SrsMode;

    if (i4Argc < 2 )
    {
        Printf("Usage: enable [tsxtmode] \n");
        Printf("[tsxtmode]     0:SV_A_SRSTSXT_OFF   1:SV_A_SRSTSXT_3DMONO,  2:SV_A_SRSTSXT_STEREO \n");
        return -1;
    }

    u1SrsMode = (UINT8)StrToInt(szArgv[1]);
    AUD_DspSetSRSTSXTSetMode(u1SrsMode);
    return 0;
}

static INT32 _AudCmdUopSRSTSXTSpkSz(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;
    /*
        Speaker size table: 0: 40Hz, 1: 60Hz, 2: 100Hz, 3: 150Hz,
                            4: 200Hz, 5: 250Hz, 6: 300Hz, 7: 400Hz
    */
    if (i4Argc < 2)
    {
        Printf("Usage: tspk  [mode]\n");
        Printf("[mode] 0:  40Hz \n");
        Printf("       1:  60Hz \n");
        Printf("       2: 100Hz \n");
        Printf("       3: 150Hz \n");
        Printf("       4: 200Hz \n");
        Printf("       5: 250Hz \n");
        Printf("       6: 300Hz \n");
        Printf("       7: 400Hz \n");
        return -1;
    }

    u1Mode = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetSRSTSXTSpkSz(u1Mode);
    return 1;
}


static INT32 _AudCmdUopSRSTSXTElev(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;

    if (i4Argc < 2)
    {
        Printf("Usage: telv  [mode]\n");
        Printf("[mode] 0:   0% \n");
        Printf("       1:  10% \n");
        Printf("       2:  50% \n");
        Printf("       3: 100% \n");
        return -1;
    }

    u1Mode = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetSRSTSXTElev(u1Mode);
    return 1;
}

static INT32 _AudCmdUopSRSTSXTinputgain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: ing  [gain 32bits]\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    AUD_DspSetSRSTSXTinputgain(u4Mode);
    return 1;
}
static INT32 _AudCmdUopSRSTSXTTBLvl(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;

    if (i4Argc < 2)
    {
        Printf("Usage: ttb  [mode]\n");
        Printf("[mode] 0:   0% \n");
        Printf("       1:  38% \n");
        Printf("       2:  60% \n");
        Printf("       3: 100% \n");
        return -1;
    }

    u1Mode = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetSRSTSXTTBLvl(u1Mode);
    return 1;
}
#endif

#ifdef FRAC_SPEED_SUPPORT
//sunman for play speed
static INT32 _AudCmdUopPlaySpeed(INT32 i4Argc, const CHAR **szArgv)
{
	UINT32 u4Mode, u4step;

    if (i4Argc < 2)
    {
        Printf("Usage:fspeed  [s]\n");
        Printf("s: speed options  \n");
        return -1;
    }
    u4step = 0x0019999A ;  // 0.1  (0.24)
    //u4step = 0x000CCCCD ;  // 0.1  (1.23)
    u4Mode = (UINT32)StrToInt(szArgv[1]);// (1~40)
    u4Mode = u4Mode * u4step;
    u4Mode = u4Mode >> 2;

    if(u4Mode!=0)
	    u4Mode -= 1;//(0.24) 0x400001-1=0x400000

    DSP_SpeedSetMode(u4Mode);
    return 1;
}

#endif

#if (defined(DSP_SUPPORT_SRSTSHD) || defined(DSP_SUPPORT_SRSWOWHD))
//sunman_tshd
static INT32 _AudCmdUopSRSTSHDSetInit(INT32 i4Argc, const CHAR **szArgv){
	//aud.dsp.eq
	AUD_DspChannelEqEnable(0, 0);
       AUD_DspBassBoostEnable(0, 0);
       AUD_DspSuperBassEnable(0, 0);
       AUD_DspClearBoostEnable(0, 0);
	//aud.dsp.a
	AUD_DspChangeAVCPara(0, 0, FALSE);
	//aud.spdif.iecc
	UNUSED(AUD_DspIecChannel((AUD_IEC_CH_T)0));
        return 0;
}

static INT32   _AudCmdUopSRSTSHDInpGain(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: tinpg [lv] \n");
        Printf("Input gain = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSTSHDInpg(u1Mode);
    return 0;
}

static INT32	_AudCmdUopSRSTSHDOutGain(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: toutg [lv] \n");
        Printf("Output gain = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSTSHDOutg(u1Mode);
    return 0;
}

static INT32 _AudCmdUopSRSTSHDSurrLvl(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;
    u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: tsrlv  [lv] \n");
        Printf("surround level = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSTSHDSurrLvl(u1Mode);
    return 1;
}

static INT32 _AudCmdUopSRSTSHDSpkSz(INT32 i4Argc, const CHAR **szArgv)
{
	 UINT8 u1Mode;
        //Speaker size table: 0: 40Hz, 1: 60Hz, 2: 100Hz, 3: 150Hz,

    if (i4Argc < 2)
    {
        Printf("Usage: tspk  [mode]\n");
        Printf("[mode] 0:  40Hz \n");
        Printf("       1:  60Hz \n");
        Printf("       2: 100Hz \n");
        Printf("       3: 150Hz \n");
        return -1;
    }
    u1Mode = (UINT8)StrToInt(szArgv[1]);
   DSP_SRSTSHDSetSpkSize(u1Mode);
    return 1;
}

static INT32 _AudCmdUopSRSTSHDTBLvl(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;
    u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: ttb  [lv] \n");
        Printf("TruBass level = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSTSHDTBLvl(u1Mode);
    return 1;
}

static INT32 _AudCmdUopSRSTSHDDef(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;
    u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: tdef  [lv] \n");
        Printf("Definition level = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSTSHDDef(u1Mode);
    return 1;
}

static INT32 _AudCmdUopSRSTSHDElev(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: telv [lv] \n");
        Printf("Elevation level = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSTSHDElev(u1Mode);
    return 0;
}

static INT32 _AudCmdUopSRSTSHDFntCCtrl(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: fcc [lv] \n");
        Printf("Front Center Control level = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSTSHDFntCCtrl(u1Mode);
    return 0;
}

static INT32 _AudCmdUopSRSTSHDFntSCtrl(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: fsc [lv] \n");
        Printf("Front Space Control level = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSTSHDFntSCtrl(u1Mode);
    return 0;
}

static INT32 _AudCmdUopSRSTSHDMode(INT32 i4Argc, const CHAR **szArgv)
{
   UINT32 u4Mode;
   u4Mode = (UINT32)StrToInt(szArgv[1]);
    if (i4Argc < 2 )
    {
        Printf("Usage: tmode [mode] \n");
        return -1;
    }
    DSP_SRSTSHDMode(u4Mode);
    return 0;
}
#endif //#if (defined(DSP_SUPPORT_SRSTSHD) || defined(DSP_SUPPORT_SRSWOWHD))


#ifdef DSP_SUPPORT_SRSVIQ

extern void DSP_SRSVIQRefLvl(UINT32 u4Mode);
extern void DSP_SRSVIQMode(UINT32 u4Mode);
extern  void DSP_SRSVIQMaxGain(UINT32 u4Mode);
extern void DSP_SRSVIQInputGain(UINT32 u4Mode);
extern void DSP_SRSVIQOutputGain(UINT32 u4Mode);
extern void DSP_SRSVIQNoiseTh(UINT8 u1Mode);

//sunman_viq
static INT32 _AudCmdUopSRSVIQRefLvl(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if(i4Argc == 2)
    {
        u4Mode = (UINT32)StrToInt(szArgv[1]);
        if (u4Mode > 20)
           u4Mode = 20;
       u4Mode *=  0x66666 ;
    }
   else if(i4Argc == 3)
   {
        u4Mode = (UINT32)StrToInt(szArgv[2]);
   }
   else
   {
        Printf("Usage: ref  [level]\n");
        Printf("Value = 0.05 * [level] : level between 0~20\n");
        //Printf("Enter levl > 20 for using IR control\n");
        return -1;
    }
    DSP_SRSVIQRefLvl(u4Mode);
    return 1;
}

#ifdef VIQ_MODE_OLD
static INT32 _AudCmdUopSRSVIQMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: mode  [mode]\n");
        Printf("[mode] 0:   Noise off, Full range\n");
        Printf("           2:   Noise off, 40   Hz\n");
        Printf("           4:   Noise off, 110 Hz\n");
        Printf("           6:   Noise off, 200 Hz\n");
        Printf("           8:   Noise off, 315 Hz\n");
        Printf("          10:  Noise off, 410 Hz\n");

	 Printf("[mode] 1:  Noise on, Full range\n");
        Printf("           3:   Noise on, 40   Hz\n");
        Printf("           5:   Noise on, 110 Hz\n");
        Printf("           7:   Noise on, 200 Hz\n");
        Printf("           9:   Noise on, 315 Hz\n");
        Printf("          11:  Noise on, 410 Hz\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSVIQMode(u4Mode);
    return 1;
}
#else
static INT32 _AudCmdUopSRSVIQMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: mode  [mode]\n");
        Printf("[mode] 0:  Light\n");
        Printf("           1:   Normal\n");
        Printf("           2:   Heavy\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSVIQMode(u4Mode);
    return 1;
}
#endif

static INT32 _AudCmdUopSRSVIQMaxGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: maxg  [gain]\n");
        Printf("max gain = 0~256\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
     if ( u4Mode > 256 )
    {
        Printf("Usage: maxg  [gain]\n");
        Printf("max gain = 0~256\n");
        return -1;
    }

    DSP_SRSVIQMaxGain(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSVIQInputGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;

    if(i4Argc == 2)
    {
        u4Mode = (UINT32)StrToInt(szArgv[1]);
        if (u4Mode > 80)
           u4Mode = 80;
           u4Mode *=  0x1999a;
    }
    else if(i4Argc == 3)
   {
        u4Mode = (UINT32)StrToInt(szArgv[2]);
   }
   else
   {
        Printf("Usage: inpg  [gain] scale 2 (0~4)\n");
        Printf("[gain] = 0~80 step 0.05\n");
        return -1;
    }

    DSP_SRSVIQInputGain(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSVIQOutputGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if(i4Argc == 2)
    {
        u4Mode = (UINT32)StrToInt(szArgv[1]);
        if (u4Mode > 80)
           u4Mode = 80;
        u4Mode *=  0x1999a;
    }
   else if(i4Argc == 3)
   {
        u4Mode = (UINT32)StrToInt(szArgv[2]);
   }
   else
   {
        Printf("Usage: outpg  [gain] scale 2 (0~4)\n");
        Printf("[gain] = 0~80 step 0.05\n");
        return -1;
    }
    DSP_SRSVIQOutputGain(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSVIQDisableEQ(INT32 i4Argc, const CHAR **szArgv){
	//aud.dsp.eq
	AUD_DspChannelEqEnable(0, 0);
    AUD_DspBassBoostEnable(0, 0);
    AUD_DspSuperBassEnable(0, 0);
    AUD_DspClearBoostEnable(0, 0);
    AUD_DspVirtualSurroundEnalbe(0, 0);
    AUD_DspMasterVolume(0, 100);
	UNUSED(AUD_DspIecChannel( (AUD_IEC_CH_T)0));
	return 1;
}

static INT32 _AudCmdUopSRSVIQNoiseTh(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: tnoise [lv] \n");
        Printf("Noise TH = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSVIQNoiseTh(u1Mode);
    return 0;
}
#endif

#ifdef CC_AUD_SUPPORT_MS10
static INT32 _AudCmdBypassMDC(INT32 i4Argc, const CHAR **szArgv)
{

     UINT8 u1Enable;

    if (i4Argc < 2)
    {
        Printf("Usage: MDC Bypass [On/Off] \n");
        Printf("1: bypass, 0: no bypass\n");
        return -1;
    }

    u1Enable = (UINT8)StrToInt(szArgv[1]);

    AUD_DspDDCOEnable((BOOL)u1Enable);
    return 1;
}

static INT32 _AudCmdDtvDdtDmxMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Mode;

    if (i4Argc < 3)
    {
        Printf("  \n");
        Printf("Current mode : FIRST      ==> %d\n",AUD_DspGetDdtDmxRfMode(AUD_DEC_MAIN,TRUE));
        Printf("                       SECOND  ==> %d\n",AUD_DspGetDdtDmxRfMode(AUD_DEC_AUX,TRUE));
        Printf("                       THIRD     ==> %d\n",AUD_DspGetDdtDmxRfMode(AUD_DEC_THIRD,TRUE));
        Printf("  \n");
        Printf("Usage: dm [decoder id] [dmx mode]\n");
        Printf("[decoder id]       0:FIRST      1:SECOND        2: THIRD\n");
        Printf("[dmx mode] 0:Lt/Rt         1:Lo/Ro\n");
        return -1;
    }
    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Mode = (UINT8)StrToInt(szArgv[2]);

    if(u1Mode<2)
    {
        AUD_DspDdtDmxRfMode(u1Mode, u1DecId,TRUE);
    }

    return 1;
};


static INT32 _AudCmdDtvDdtRfMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Mode;

    if (i4Argc < 3)
    {
        Printf("  \n");
        Printf("Current mode : FIRST      ==> %d\n",AUD_DspGetDdtDmxRfMode(AUD_DEC_MAIN,FALSE));
        Printf("                       SECOND  ==> %d\n",AUD_DspGetDdtDmxRfMode(AUD_DEC_AUX,FALSE));
        Printf("                       THIRD     ==> %d\n",AUD_DspGetDdtDmxRfMode(AUD_DEC_THIRD,FALSE));
        Printf("  \n");
        Printf("Usage: rf [decoder id] [RF mode]\n");
        Printf("[decoder id]       0:FIRST                 1:SECOND         2: THIRD\n");
        Printf("[RF mode] 0:Line         1:RF\n");
        return -1;
    }
    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Mode = (UINT8)StrToInt(szArgv[2]);

    if(u1Mode<2)
    {
        AUD_DspDdtDmxRfMode(u1Mode, u1DecId,FALSE);
    }

    return 1;
};

static INT32 _AudCmdDtvDdtDualMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Mode;

    if (i4Argc < 3)
    {
        Printf("  \n");
        Printf("Current mode : FIRST      ==> %d\n",AUD_DspGetDdtDualMode(AUD_DEC_MAIN));
        Printf("                       SECOND  ==> %d\n",AUD_DspGetDdtDualMode(AUD_DEC_AUX));
        Printf("                       THIRD     ==> %d\n",AUD_DspGetDdtDualMode(AUD_DEC_THIRD));
        Printf("  \n");
        Printf("Usage: dual [decoder id] [Dual mode]\n");
        Printf("[decoder id]       0:FIRST                 1:SECOND         2: THIRD\n");
        Printf("[Dual mode] 0:Stereo         1:Left only          2: Right only\n");
        return -1;
    }
    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Mode = (UINT8)StrToInt(szArgv[2]);

    if(u1Mode<3)
    {
        AUD_DspDdtDualMode(u1Mode, u1DecId);
    }

    return 1;
};

static INT32 _AudCmdDtvDdtDrcRange(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Mode;

    if (i4Argc < 3)
    {
        Printf("  \n");
        Printf("Current mode : FIRST      ==> %d\n",AUD_DspGetDdtDrcRange(AUD_DEC_MAIN));
        Printf("                       SECOND  ==> %d\n",AUD_DspGetDdtDrcRange(AUD_DEC_AUX));
        Printf("                       THIRD     ==> %d\n",AUD_DspGetDdtDrcRange(AUD_DEC_THIRD));
        Printf("  \n");
        Printf("Usage: drc [decoder id] [Cut/Boost Factor]\n");
        Printf("[decoder id]       0:FIRST                 1:SECOND         2: THIRD\n");
        Printf("[Cut/Boost Factor] 0 10 20 50 80 90 100\n");
        return -1;
    }
    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Mode = (UINT8)StrToInt(szArgv[2]);

    if(u1Mode<=100)
    {
        AUD_DspDdtCompVal(u1Mode, u1DecId);
    }

    return 1;
};

#endif  // CC_AUD_SUPPORT_MS10

#ifdef DSP_SUPPORT_SRS_SSHD
//sunman_diag
extern void DSP_SRSPeqOn(UINT32 u4Mode);
extern void DSP_SRSPeqInputGain(UINT32 u4Mode);
extern void DSP_SRSPeqOutputGain(UINT32 u4Mode);
extern void DSP_SRSHpfOn(UINT32 u4Mode);
extern void DSP_SRSPEQHpfSetCoef(UINT32 u4Adr, UINT32 u4Coef);
extern void DSP_SRSPEQSetCoef(UINT32 u4Adr, UINT32 u4Coef);
extern void DSP_SRSHpfSetCoef(UINT32 u4Adr, UINT32 u4Coef);
extern void DSP_SRSLmtInputGain(UINT32 u4Mode);
extern void DSP_SRSLmtOutputGain(UINT32 u4Mode);
extern void DSP_SRSLmtBoost(UINT32 u4Mode);
extern void DSP_SRSLmtLvl(UINT32 u4Mode);
extern void DSP_SRSLmtDly(UINT32 u4Mode);
extern void DSP_SRSLmtOn(UINT32 u4Mode);
extern void DSP_SRSLmtCoeff(UINT32 u4Mode);
extern void DSP_SRSLmtThresh(UINT32 u4Mode);
static INT32 _AudCmdUopSRSPeqOn(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: on  [val]\n");
        Printf("val = 0 off\n");
        Printf("val = 1 on\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSPeqOn(u4Mode << (8+1));
    return 1;
}

static INT32 _AudCmdUopSRSPeqInputGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: inpg  [level]\n");
        Printf("Value = 0.05 * [level] : level between 0~20\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSPeqInputGain(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSPeqOutputGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: outpg  [level]\n");
        Printf("Value = 0.05 * [level] : level between 0~20\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSPeqOutputGain(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSHpfOn(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: hpf  [val]\n");
        Printf("val = 0 off\n");
        Printf("val = 1 on\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);

    DSP_SRSHpfOn(u4Mode << (8+2));
    return 1;
}

static INT32 _AudCmdUopSRSPeqSetCoef(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Adr, u4Coef, u4Type;
    if (i4Argc < 4)
    {
        Printf("Usage: sc  [type] [adr] [coef]\n");
        Printf("type = 0 : SRS PEQ\n");
        Printf("type = 1 : SRS 1st HFP\n");
        Printf("type = 2 : SRS 2nd HPF\n");
        return -1;
    }
    u4Type = (UINT32)StrToInt(szArgv[1]);
    u4Adr = (UINT32)StrToInt(szArgv[2]);
    u4Coef = (UINT32)StrToInt(szArgv[3]);

    if(u4Type==0)
    {
        DSP_SRSPEQSetCoef(u4Adr, u4Coef);
    }
    else if (u4Type==1)
    {
        DSP_SRSHpfSetCoef(u4Adr, u4Coef);
    }
    else if (u4Type==2)
    {
        DSP_SRSPEQHpfSetCoef(u4Adr, u4Coef);
    }
    else
    {
        Printf("Usage: sc  [type] [adr] [coef]\n");
        Printf("type = 0 : SRS PEQ\n");
        Printf("type = 1 : SRS 1st HFP\n");
        Printf("type = 2 : SRS 2nd HPF\n");
        return -1;
    }
    return 1;
}

//sunman_lmt
static INT32 _AudCmdUopSRSLmtOn(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: on  [val]\n");
        Printf("val = 0 off\n");
        Printf("val = 1 on\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);

    DSP_SRSLmtOn(u4Mode << (8+3));
    return 1;
}

static INT32 _AudCmdUopSRSLmtInputGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: inpg  [level] scale 2 (0~4)\n");
        Printf("Value = 0.05 * [level] : level between 0~80 (scale 2)\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSLmtInputGain(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSLmtOutputGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: outpg  [level] scale 2 (0~4)\n");
        Printf("Value = 0.05 * [level] : level between 0~80 (scale 2)\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSLmtOutputGain(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSLmtBoost(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: bst  [level]\n");
        Printf("Value = 0.001 * [level] : level between 1~256000\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    //u4Mode *= 32.76799609375;
    u4Mode *= 16777;
    u4Mode >>= 9;

    DSP_SRSLmtBoost(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSLmtLvl(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: lvl [level]\n");
        Printf("Value = 0.001 * [level] : level between 1~1000\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    //u4Mode *= 8388.607;
    u4Mode *= 4294967;
    u4Mode >>= 9;
    DSP_SRSLmtLvl(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSLmtDly(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: dly [count]\n");
        Printf("count = 6~240\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSLmtDly(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSLmtCoeff(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: LmtCoeff [level]\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSLmtCoeff(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSLmtThresh(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: LmtThresh [level]\n");       
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSLmtThresh(u4Mode);
    return 1;
}


#endif

#ifdef DSP_SUPPORT_SRS_PURESOUND
extern void DSP_SRSPuresoundTrubsssOn(UINT32 u4Mode);
extern void DSP_SRSAeqOn(UINT32 u4Mode);
extern void DSP_SRSAeqInputGain(UINT32 u4Mode);
extern void DSP_SRSAeqOutputGain(UINT32 u4Mode);
extern void DSP_SRSAeqFilterSize(UINT32 u4Mode);
extern void DSP_SRSLmtInputGain(UINT32 u4Mode);
extern void DSP_SRSLmtOutputGain(UINT32 u4Mode);
extern void DSP_SRSLmtBoost(UINT32 u4Mode);
extern void DSP_SRSLmtLvl(UINT32 u4Mode);
extern void DSP_SRSLmtDly(UINT32 u4Mode);
extern void DSP_SRSLmtOn(UINT32 u4Mode);
extern void DSP_SRSLmtCoeff(UINT32 u4Mode);
extern void DSP_SRSLmtThresh(UINT32 u4Mode);
static INT32 _AudCmdUopSRSLmtOn(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: on  [val]\n");
        Printf("val = 0 off\n");
        Printf("val = 1 on\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);

    DSP_SRSLmtOn(u4Mode << (8+3));
    return 1;
}

static INT32 _AudCmdUopSRSLmtInputGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: inpg  [level] scale 2 (0~4)\n");
        Printf("Value = 0.05 * [level] : level between 0~80 (scale 2)\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSLmtInputGain(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSLmtOutputGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: outpg  [level] scale 2 (0~4)\n");
        Printf("Value = 0.05 * [level] : level between 0~80 (scale 2)\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSLmtOutputGain(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSLmtBoost(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: bst  [level]\n");
        Printf("Value = 0.001 * [level] : level between 1~256000\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    //u4Mode *= 32.76799609375;
    u4Mode *= 16777;
    u4Mode >>= 9;

    DSP_SRSLmtBoost(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSLmtLvl(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: lvl [level]\n");
        Printf("Value = 0.001 * [level] : level between 1~1000\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    //u4Mode *= 8388.607;
    u4Mode *= 4294967;
    u4Mode >>= 9;
    DSP_SRSLmtLvl(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSLmtDly(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: dly [count]\n");
        Printf("count = 6~240\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSLmtDly(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSLmtCoeff(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: LmtCoeff [level]\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSLmtCoeff(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSLmtThresh(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: LmtThresh [level]\n");       
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSLmtThresh(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSAeqOn(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: on  [val]\n");
        Printf("val = 0 off\n");
        Printf("val = 1 on\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);

    DSP_SRSAeqOn(u4Mode << (8+1));
    return 1;
}

static INT32 _AudCmdUopSRSAeqInputGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: inpg  [level] scale 2 (0~4)\n");
        Printf("Value = 0.05 * [level] : level between 0~80 (scale 2)\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSAeqInputGain(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSAeqOutputGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: outpg  [level] scale 2 (0~4)\n");
        Printf("Value = 0.05 * [level] : level between 0~80 (scale 2)\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSAeqOutputGain(u4Mode);
    return 1;
}
static INT32 _AudCmdUopSRSAeqFilterSize(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: AeqFilterSize  [level]  (0~2)\n");
        Printf("level 0 default_aeq_filters.c\n");
		Printf("level 1 fir50_aeq_filters.c\n");
		Printf("level 2 iir_only_aeq_filters.c\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSAeqFilterSize(u4Mode);
    return 1;
}
static INT32 _AudCmdUopSRSPuresoundTrubassOn(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: on  [val]\n");
        Printf("val = 0 off\n");
        Printf("val = 1 on\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);

    DSP_SRSPuresoundTrubsssOn(u4Mode << (8+4));
    return 1;
}

#endif


#ifdef CC_AUD_BBE_SUPPORT
static INT32 _AudCmdUopBBEQuery(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 i1BbeMode;
	INT8   i1BbeLevel;
	UINT8 u1BbeProcess;
	UINT8 u1BbeLoContour;
	UINT8 u1Bbe3dGain;

	i1BbeMode = AUD_DspGetBbeMode();
	i1BbeLevel = AUD_DspGetBbeLevel();
	u1BbeProcess = AUD_DspGetBbeProcess();
	u1BbeLoContour = AUD_DspGetBbeLoContour();
	u1Bbe3dGain = AUD_DspGetBbe3dGain();

	Printf("BBE Mode=%d, Level=%d, LowGain=%d, HighGain=%d, 3D=%d\n",
		   i1BbeMode, i1BbeLevel, u1BbeLoContour, u1BbeProcess, u1Bbe3dGain);

    	return 1;
}

static INT32 _AudCmdUopBBEMode(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Mode;

    	if (i4Argc < 2)
    	{
       	Printf("Usage: mode  [mode]\n");
        	Printf("[mode] 0:  off,  1:  BBE,  2:  BBE VIVA4\n");
              return -1;
    	}
    	u1Mode = (UINT8)StrToInt(szArgv[1]);

    	AUD_DspBbeMode(AUD_DEC_MAIN, u1Mode); //for main decoder only
    	return 1;
}

static INT32 _AudCmdUopBBELevel(INT32 i4Argc, const CHAR **szArgv)
{
	INT8 i1Level;

    	if (i4Argc < 2)
    	{
       	Printf("Usage: level [level]\n");
        	Printf("[level] 0 ~ 5 \n");
        	return -1;
    	}
    	i1Level = (UINT8)StrToInt(szArgv[1]);

    	AUD_DspSetBbeLevel( i1Level); //for main decoder only
    	return 1;
}

static INT32 _AudCmdUopBBEHighGain(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Gain;

    	if (i4Argc < 2)
    	{
       	Printf("Usage: high [high gain]\n");
        	Printf("[high gain] 3 ~ 12 \n");
        	return -1;
    	}
    	u1Gain = (UINT8)StrToInt(szArgv[1]);

    	AUD_DspSetBbeProcess( u1Gain); //for main decoder only
	return 1;
}

static INT32 _AudCmdUopBBELowGain(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Gain;

    	if (i4Argc < 2)
    	{
       	Printf("Usage: low [low gain]\n");
        	Printf("[low gain] 3 ~ 12 \n");
        	return -1;
    	}
    	u1Gain = (UINT8)StrToInt(szArgv[1]);

    	AUD_DspSetBbeLoContour( u1Gain); //for main decoder only
	return 1;
}

static INT32 _AudCmdUopBBE3DGain(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Gain;

    	if (i4Argc < 2)
    	{
       	Printf("Usage: 3d [3d gain]\n");
        	Printf("[3d gain] 0 ~ 100 \n");
        	return -1;
    	}
    	u1Gain = (UINT8)StrToInt(szArgv[1]);

    	AUD_DspSetBbe3dGain( u1Gain); //for main decoder only
	return 1;
}

 #endif

#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
static INT32 _AudCmdUopCDNotchFlag(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Flag;

	if (i4Argc < 2)
    {
       	Printf("Usage: cdnotch.flag [enable]\n");
        Printf("[flag] 0(disable) 1(enable)\n");
        return -1;
    }

    u1Flag = (UINT8)StrToInt(szArgv[1]);
    AUD_DspClearSoundEnable(AUD_DEC_MAIN, u1Flag);

    return 1;
}

static INT32 _AudCmdUopCDNotchConfig(INT32 i4Argc, const CHAR **szArgv)
{
	UINT32	u4UserQInt, u4UserQFrac, u4UserQIdx, u4UserFc;
	const CHAR*	p1strQ, *p1strFc;
	CHAR	p1strTemp[2];


	if (i4Argc != 3)
    {
       	Printf("Usage: cdnotch.cfg [Q] [Fc]\n");
        Printf("[Q] 0 ~ 3.7 [Fc] 60Hz ~ 15kHz\n");
        return -1;
    }

	p1strFc = szArgv[2];
	u4UserFc = (UINT32)StrToInt(p1strFc);
	p1strQ = szArgv[1];

	switch ((UINT32)x_strlen(p1strQ))
	{
		case 1:
			u4UserQInt = (UINT32)StrToInt(p1strQ);
			u4UserQFrac = 0;
			break;
		case 3:
			p1strTemp[0] = p1strQ[0];
			p1strTemp[1] = '\0';
			u4UserQInt = (UINT32)StrToInt(p1strTemp);
			if (p1strQ[1] != '.')
			{
				Printf("Incorrect format of Q.\n");
				Printf("Q = x.x\n");
				return -1;
			}
			p1strTemp[0] = p1strQ[2];
			u4UserQFrac = (UINT32)StrToInt(p1strTemp);
			break;
		default:
			Printf("Incorrect format of Q.\n");
			Printf("Q = x.x\n");
			return -1;
	};

	u4UserQIdx = (u4UserQInt*10)+u4UserQFrac;
    AUD_DspSetCDNotchQandFc(u4UserQIdx, u4UserFc); //for main decoder only

	return 1;
}
#endif

#ifdef  KARAOKE_SUPPORT
static INT32 _AudCmdUopKeyShiftFlag(INT32 i4Argc, const CHAR **szArgv)
{
    BOOL    fgflag;
    INT32   u4flag;

    u4flag = StrToInt(szArgv[1]);

    if (i4Argc != 2 || (u4flag != 0 && u4flag != 1))
    {
       	Printf("Usage: keyshift.flag [1/0], 1:On, 0:Off\n");
        return -1;
    }

    fgflag = (u4flag) ? TRUE : FALSE;
    AUD_DspSetKeyshiftFlag(fgflag);

    return 1;
}
static INT32 _AudCmdUopKeyShiftKey(INT32 i4Argc, const CHAR **szArgv)
{
    INT8   i1key;

    i1key = (INT8)StrToInt(szArgv[1]);
    if ( (i4Argc != 2) || (i1key < 0) || (i1key > 32))
    {
       	Printf("Usage: keyshift.key [0~32(-16~+16)]\n");
        return -1;
    }

    AUD_DspSetKeyshiftKey(i1key-16);

    return 1;
}
#endif

#if 0
static INT32 _AudCmdUopSAPVolume(INT32 i4Argc,const CHAR** szArgv)
{

    UINT8 u1SAPIndex;
    u1SAPIndex = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2)
    {
        Printf("Usage: sap [table index]  \n");
        Printf("[table index]    0 ~ 40 : -10 dB to 10 dB step 0.5 dB  \n");
        return -1;
    }
   else  if (u1SAPIndex<41)
    {
        AUD_DspSetSAPFineVolume(u1SAPIndex);
        return 1;
    }
   else
    {
        Printf("Usage: sap [table index]  \n");
        Printf("[table index]    0 ~ 40 : -10 dB to 10 dB step 0.5 dB  \n");
        return -1;
    }
}
#endif

static INT32 _AudCmdDDbanner(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1DecId;
   UINT8 u1Enable;

    if (i4Argc < 3)
    {
        Printf("Usage: banner [decoder id] [On/Off] \n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[On/Off]         1/0\n");
		return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Enable = (UINT8)StrToInt(szArgv[2]);

    AUD_DDBannerEnable(u1DecId, (BOOL)u1Enable);
    return 0;
}

//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
static INT32 _AudCmdUopUploadData(INT32 i4Argc, const CHAR **szArgv)
{

     UINT8 u1Mode;

    if (i4Argc < 2)
    {
        Printf("Usage: upload [Format] \n");
        Printf("0: off, 1: PCM, 2: MP3, 3: SBC Encoder, 4: ALSA, 5: WiFi\n");
        return -1;
    }

    u1Mode = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetUploadMode(u1Mode);
    AUD_DspUploadDataEnable();
    return 1;
}

static INT32 _AudCmdDataUploadInit(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc < 1)
    {
        Printf("Usage: init\n");
        return -1;
    }

    AUD_DataUpload_Init();
    AUD_DataUpload_start();

    return 1;
}

extern UINT32 u4GetUploadFIFOStart(void);
extern UINT32 u4GetUploadFIFOEnd(void);
extern UINT32 u4GetUploadWritePnt(void);
static INT32 _AudCmdDataUploadDump(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Addr;
    UINT32 u4Size;
    UINT32 u4FifoSA;
    UINT32 u4FifoEA;
    UINT32 u4RP, u4WP;

    if (i4Argc != 3)
    {
        Printf("Usage: upd [output addr] [output size]\n");
        return -1;
    }

    u4Addr = (UINT32)StrToInt(szArgv[1]);
    u4Size = (UINT32)StrToInt(szArgv[2]);
    u4FifoSA = u4GetUploadFIFOStart();
    u4FifoEA = u4GetUploadFIFOEnd();

    Printf("Dump upload data - FIFO(%08x~%08x) to (%08x~%08x)\n", u4FifoSA, u4FifoEA, u4Addr, u4Addr+u4Size);

    u4RP = u4GetUploadWritePnt();
    if ((u4RP < u4FifoSA) || (u4RP >= u4FifoEA))
    {
        u4RP = u4FifoSA;
    }

    AUD_DspSetUploadMode(4);
    AUD_DspUploadDataEnable();

    while (u4Size)
    {
        u4WP = u4GetUploadWritePnt();
        if ((u4WP >= u4FifoSA) && (u4WP < u4FifoEA)) //validity check
        {
            if (u4RP < u4WP)
            {
                if ((u4WP - u4RP) > 0x400)
                {
                    x_memcpy((VOID*)(VIRTUAL(u4Addr)),
                                (VOID*)(VIRTUAL(u4RP)),
                                0x400);
                    u4RP += 0x400;
                    if (u4RP == u4FifoEA)
                        u4RP = u4FifoSA;
                    u4Addr += 0x400;
                    u4Size -= 0x400;
                    Printf(".");
                    continue;
                }
            }
            else if (u4RP > u4WP)
            {
                if (((u4FifoEA - u4FifoSA) - (u4RP - u4WP)) > 0x400)
                {
                    x_memcpy((VOID*)(VIRTUAL(u4Addr)),
                                (VOID*)(VIRTUAL(u4RP)),
                                0x400);
                    u4RP += 0x400;
                    if (u4RP == u4FifoEA)
                        u4RP = u4FifoSA;
                    u4Addr += 0x400;
                    u4Size -= 0x400;
                    Printf(".");
                    continue;
                }
            }
        }

        x_thread_delay(10);
    }

    AUD_DspSetUploadMode(0);
    AUD_DspUploadDataEnable();

    return 1;
}
//#endif

extern UINT32 u4GetAFIFOStart(UINT8 uDecIndex);
extern UINT32 u4GetAFIFOEnd(UINT8 uDecIndex);
extern UINT32 u4GetAWritePnt(UINT8 uDecIndex);
static INT32 _AudCmdADCDump(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Addr;
    UINT32 u4Size;
    UINT32 u4FifoSA;
    UINT32 u4FifoEA;
    UINT32 u4RP, u4WP;

    if (i4Argc != 3)
    {
        Printf("Usage: adcd [output addr] [output size]\n");
        return -1;
    }

    u4Addr = (UINT32)StrToInt(szArgv[1]);
    u4Size = (UINT32)StrToInt(szArgv[2]);
    u4FifoSA = u4GetAFIFOStart(1);
    u4FifoEA = u4GetAFIFOEnd(1);

    Printf("Dump line-in ADC data - FIFO(%08x~%08x) to (%08x~%08x)\n", u4FifoSA, u4FifoEA, u4Addr, u4Addr+u4Size);

    //TODO
    (*((volatile UINT32*)(0xf000501c)))=0x8c0; //Disable Line-In
    (*((volatile UINT32*)(0xf0005010)))=0x172e172e;
    (*((volatile UINT32*)(0xf0005024)))= (((u4FifoSA&0xffff00)<<8) | ((u4FifoEA&0xffff00)>>8));
    //(*((volatile UINT32*)(0xf0005040)))=0x300610;
    (*((volatile UINT32*)(0xf000d26c)))=0x00170001; //Original - 0x110001
    (*((volatile UINT32*)(0xf0005248)))=0x004804c0; //Input Selection   ([15:12] = F, power down internal adc)
    (*((volatile UINT32*)(0xf000501c)))=0x8c9; //Enable Line-In + LSB 16bits
    //TODO

    u4RP = (u4GetAWritePnt(1)&0xfffffc00);
    if ((u4RP < u4FifoSA) || (u4RP >= u4FifoEA))
    {
        u4RP = u4FifoSA;
    }

    while (u4Size)
    {
        u4WP = u4GetAWritePnt(1);
        if ((u4WP >= u4FifoSA) && (u4WP < u4FifoEA)) //validity check
        {
            if (u4RP < u4WP)
            {
                if ((u4WP - u4RP) > 0x400)
                {
                    x_memcpy((VOID*)(VIRTUAL(u4Addr)),
                                (VOID*)(VIRTUAL(u4RP)),
                                0x400);
                    u4RP += 0x400;
                    if (u4RP == u4FifoEA)
                        u4RP = u4FifoSA;
                    u4Addr += 0x400;
                    u4Size -= 0x400;
                    Printf(".");
                    continue;
                }
            }
            else if (u4RP > u4WP)
            {
                if (((u4FifoEA - u4FifoSA) - (u4RP - u4WP)) > 0x400)
                {
                    x_memcpy((VOID*)(VIRTUAL(u4Addr)),
                                (VOID*)(VIRTUAL(u4RP)),
                                0x400);
                    u4RP += 0x400;
                    if (u4RP == u4FifoEA)
                        u4RP = u4FifoSA;
                    u4Addr += 0x400;
                    u4Size -= 0x400;
                    Printf(".");
                    continue;
                }
            }
        }

        x_thread_delay(10);
    }

    //TODO
    (*((volatile UINT32*)(0xf000501c)))=0x8c0; //Disable Line-In
    (*((volatile UINT32*)(0xf000d26c)))=0x00110001;
    //TODO

    return 1;
}

static INT32 _AudCmdUopPostQuery(INT32 i4Argc, const CHAR **szArgv)
{
    //UINT8 u1ChId;
    UINT8 u1DownmixPosition;
    UINT8 u1MatrixMode;
    UINT8 u1PanFade;
    BOOL fgPostProcessingEnable;
#ifdef CC_AUD_SILENCE_SUPPORT
    UINT32 u4SilenceMode;
#endif
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    UINT16 u2AvcValue;
    UINT8 u1LimiterMode;
    UINT16 u2LimiterFlag;
    UINT32 u4LimiterThre;
    UINT8 u1MasterVolume;
    UINT32 u4MasterVolume;
    BOOL fgSurroundEnable;
    BOOL fgBassEnable;
    BOOL fgTrebleEnable;
    BOOL fgEqEnable;
    BOOL fgBassManagementEnable;
    BOOL fgChannelVolumeEnable;
    BOOL fgChannelDelayEnable;
    UINT16 u2SystemSetup;

    /*
    if (i4Argc != 2)
    {
        Printf("Usage: pq [ch_id]\n");
        Printf("  ch_id - 0(L/R),1(SL/SR),2(C/LFE),3(CH7/8),4(CH9/10),5[Aux L/R]\n");
        return -1;
    }
    u1ChId = (UINT8)StrToInt(szArgv[1]);
    */

    u1DownmixPosition = AUD_DspGetDownmixPosition();
    u1MatrixMode = AUD_DspGetMatrixOutMode(AUD_DEC_MAIN);
    u1PanFade = AUD_DspGetAdPanFadeSetting();
    fgPostProcessingEnable = AUD_DspGetPostProcessingEnable(AUD_DEC_MAIN);
#ifdef CC_AUD_SILENCE_SUPPORT
    DSP_QrySilenceMode(&u4SilenceMode);
#endif
    AUD_GetStreamFrom(AUD_DEC_MAIN,&eStreamFrom);
    AUD_DspGetAVCPara(0, &u2AvcValue, AUD_DEC_MAIN);
    AUD_DspGetLimiterConfig(&u1LimiterMode, &u2LimiterFlag, &u4LimiterThre);
    u1MasterVolume = AUD_DspGetMasterVolume(AUD_DEC_MAIN);
    u4MasterVolume = AUD_DspGetFineTuneVolume(AUD_DEC_MAIN,AUD_CH_ALL);
    fgSurroundEnable = AUD_DspGetVirtualSurroundEnalbe(AUD_DEC_MAIN);
    fgBassEnable = AUD_DspGetBassBoostEnable(AUD_DEC_MAIN);
    fgTrebleEnable = AUD_DspGetClearBoostEnable(AUD_DEC_MAIN);
    fgEqEnable = AUD_DspGetChannelEqEnable(AUD_DEC_MAIN);
    fgBassManagementEnable = AUD_DspGetBassMngEnable(AUD_DEC_MAIN);
    fgChannelVolumeEnable = AUD_DspGetChannelVolumeEnable(AUD_DEC_MAIN);
    fgChannelDelayEnable = AUD_DspGetDelayEnable(AUD_DEC_MAIN);
    u2SystemSetup = AUD_DspGetSystemSetup(AUD_DEC_MAIN);

    Printf("[DEC1 Post-Proc Path]\n");
    Printf("  fill_asrc_buf\n");
    Printf("  get_asrc_output_data\n");
    //ToDo: upload path if upload_flag[0]
    if (u1DownmixPosition == 3)
    {
        Printf("  ==>CH9/10 output here (wo post-proc)\n");
    }
    Printf("  matrix mode: %d\n", u1MatrixMode);
    if (u1PanFade == 1)
    {
        Printf("  ad_pan_fade\n");
    }
    Printf("  ==>CH7/8 output here (wo post-proc)\n");
    if (u1PanFade == 2)
    {
        Printf("  ad_pan_fade\n");
    }
    if (u1DownmixPosition == 2)
    {
        Printf("  ==>CH9/10 output here (wo post-proc)\n");
    }
    if (fgPostProcessingEnable)
    {
#ifdef CC_AUD_SILENCE_SUPPORT
        Printf("  Silence %s\n", u4SilenceMode ? "Enable" : "Disable");
#endif
        Printf("  InputSrcGain %d (sv,svq)\n",AUD_DspGetSrcVol(AUD_DEC_MAIN, eStreamFrom));
        Printf("  Reserve headroom 3bits\n");
        if (u1DownmixPosition == 1)
        {
            Printf("  ==>CH9/10 output here (before AVC)\n");
        }
        if (u1PanFade==3)
        {
            Printf("  ad_pan_fade\n");
        }
        Printf("    AVC: %s (a)\n", u2AvcValue ? "On" : "Off");
        Printf("    Master Volume: %d(0x%08x) (v,fv)\n", u1MasterVolume, u4MasterVolume);
        Printf("    Limiter Mode: %d (limiter)\n", u1LimiterMode);
        //Printf("    Downmix to LR if no up-mix\n");
        Printf("    Surround: %s (vs,vscfg)\n", fgSurroundEnable ? "On" : "Off");
        Printf("    Limiter Mode: %d (limiter)\n", u1LimiterMode);
        Printf("    Bass: %s (sbass)\n", fgBassEnable ? "On" : "Off");
        Printf("    Treble: %s (sbass)\n", fgTrebleEnable ? "On" : "Off");
        Printf("    Limiter Mode: %d (limiter)\n", u1LimiterMode);
        Printf("    Equalizer: %s (eq)\n", fgEqEnable ? "On" : "Off");
        Printf("    Limiter Mode: %d (limiter)\n", u1LimiterMode);
        Printf("  Recover headroom 3bits\n");
        Printf("  EQ spectrum\n");
        if (u2SystemSetup & 0x0040)
        {
            Printf("  =>Downmix to CH L/R\n");
        }
    }
    else
    {
        Printf("  Post-Proc bypass\n");
    }
    //ToDo: upload path if upload_flag[3]
    if (u1DownmixPosition == 0)
    {
        Printf("  ==>CH9/10 output here (w/ Post-Proc)\n");
    }
    if (u2SystemSetup & 0x1000)
    {
        Printf("  ==>Copy CH7/8 to CH LS/RS\n");
    }
    Printf("  Bass Management: %s (spkuop)\n", fgBassManagementEnable ? "On" : "Off");
    Printf("  == Post-Proc End ==\n");
    //ToDo: HPF Here
    //ToDo: _2ch_to_4ch
    Printf("  Channel Volume: %s (cv)\n", fgChannelVolumeEnable ? "On" : "Off");
    Printf("  Delay: %s (spkuop.delaycfg)\n", fgChannelDelayEnable ? "On" : "Off");

    return 1;
}

static INT32 _AudCmdUopDDCO(INT32 i4Argc, const CHAR **szArgv)
{

     UINT8 u1Enable;

    if (i4Argc < 2)
    {
        Printf("Usage: DDCO [On/Off] \n");
        Printf("[On/Off]         1/0\n");
        return -1;
    }

    u1Enable = (UINT8)StrToInt(szArgv[1]);

    AUD_DspDDCOEnable((BOOL)u1Enable);
    return 1;
}

static INT32 _AudCmdUopDDCOAgc(INT32 i4Argc, const CHAR **szArgv)
{

     UINT8 u1Enable;

    if (i4Argc < 2)
    {
        Printf("Usage: DDCO AGC [On/Off] \n");
        Printf("[On/Off]         1/0\n");
        return -1;
    }

    u1Enable = (UINT8)StrToInt(szArgv[1]);

    AUD_DspDDCOAgcEnable((BOOL)u1Enable);
    return 1;
}

static INT32 _AudCmdUopDDCOLFE(INT32 i4Argc, const CHAR **szArgv)
{

     UINT8 u1Enable;

    if (i4Argc < 2)
    {
        Printf("Usage: DDCO LFE LPF [On/Off] \n");
        Printf("[On/Off]         1/0\n");
        return -1;
    }

    u1Enable = (UINT8)StrToInt(szArgv[1]);

    AUD_DspDDCOLFEEnable((BOOL)u1Enable);
    return 1;
}

#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
static INT32 _AudCmdUopADV(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Enable;

    if (i4Argc < 2)
    {
        Printf("Usage: ADV  \n");
        Printf("[Off]         0\n");
        Printf("[light compression]         1\n");
        Printf("[medium compression]         2\n");
        Printf("[heavy compression]         3\n");
        return -1;
    }
    u1Enable = (UINT8)StrToInt(szArgv[1]);

    AUD_DspADVEnable((UINT8)u1Enable);
    return 1;
}

static INT32 _AudCmdUopADV_Query(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    _AudADVQuery();
    return 1;
}

static INT32 _AudCmdUopADVSetChGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Item;
    UINT32 u4value;

    if (i4Argc < 3)
    {
        Printf("Usage: Set ADV channel Gain {[item indx]} [value] \n");
        Printf("  \n");
        Printf("             [item indx]                    [value in dB (10.14)] \n");
        Printf("    0  :  chCalbGain_L                     0x0~ 0xffffff         \n");
        Printf("    1  :  chCalbGain_R                     0x0~ 0xffffff       \n");
        return -1;
    }
    else
    {
        u1Item = (UINT8)StrToInt(szArgv[1]);
        u4value = (UINT32)StrToInt(szArgv[2]);
        switch (u1Item)
        {
            case 0:
                AUD_DspADVSetChGain(0,u4value);
                break;
            case 1:
                AUD_DspADVSetChGain (1,u4value);
                break;
	      default:
              Printf("Invalid channel \n");
               return -1;

        }
    }
    UNUSED(i4Argc);
    UNUSED(szArgv);

    return 1;
}

static INT32 _AudCmdUopABXConfig(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1ItemId;
    UINT8 u1Val;

    if (i4Argc != 3)
    {
        Printf("Usage: abxcfg [item] [val]\n");
        AUD_DspABXQuery();
        return -1;
    }

    u1ItemId = (UINT8)StrToInt(szArgv[1]);
    u1Val = (UINT8)StrToInt(szArgv[2]);
    AUD_DspABXConfig(u1ItemId,u1Val);
    return 0;
}

static INT32 _AudCmdUopAeqFlag(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Enable;

    if (i4Argc < 3)
    {
        AUD_DspAEQQuery();
        Printf("\nUsage: aeq [decoder id] [on/off]\n");
        Printf("[decoder id]  0:FIRST  1:SECOND \n");
        Printf("[on/off]      1: on 0: off\n");
        return 1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Enable = (UINT8)StrToInt(szArgv[2]);

    AUD_DspAEQEnable(u1DecId, (BOOL)u1Enable);

    return 1;
}
#endif

static INT32 _AudCmdMemAlloc(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Size = 0;
    static UINT8* pMemPlayBuf = 0;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (i4Argc != 2)
    {
        Printf("Usage: t2 [Length]\n");
        Printf(" Length = 0 for free\n");
        return -1;
    }

    u4Size = (UINT32)StrToInt(szArgv[1]);
    if (u4Size == 0)
    {
        VERIFY(BSP_FreeAlignedDmaMemory((UINT32)pMemPlayBuf));
        Printf( "Free memory 0x%x \n",pMemPlayBuf);
        pMemPlayBuf = 0;
        return 0;
    }
    else
    {
        VERIFY(BSP_FreeAlignedDmaMemory((UINT32)pMemPlayBuf));
        Printf( "Free memory 0x%x \n",pMemPlayBuf);
    }

    Printf( "Allocate memory size 0x%x for memory play...\n",u4Size);
    pMemPlayBuf = (UINT8 *)BSP_AllocAlignedDmaMemory(u4Size, 32);
    if (pMemPlayBuf == NULL)
    {
        printf( "Memory request for memory play fail !!\n");
        return -1;
    }

    Printf("Allocate memory from 0x%x(0x%x) to 0x%x(0x%x) Success, len = 0x%x\n",pMemPlayBuf,VIRTUAL((UINT32)pMemPlayBuf),pMemPlayBuf+u4Size,VIRTUAL((UINT32)pMemPlayBuf+u4Size),u4Size);
    Printf( "data.load.binary FILENAME 0x%x--0x%x /ny /swap \n",VIRTUAL((UINT32)pMemPlayBuf),VIRTUAL((UINT32)pMemPlayBuf+u4Size));

    return 0;

}

#ifdef CC_AUD_SKYPE_SUPPORT
static INT32 _AudSkypeMixRingSnd(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1MixFlag;

    if (i4Argc != 2)
    {
        Printf("Usage: skprmix   [off/on]                \n");
        Printf("skprmix    0/1              \n");
        return -1;
    }

    u1MixFlag = (UINT8)StrToInt(szArgv[1]);
    AUD_DspSkypeRingMix((BOOL)u1MixFlag);
    return 1;
}
static INT32 _AudSkypeAuxMix2Main(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1MixFlag;

    if (i4Argc != 2)
    {
        Printf("Usage: skptxmix   [off/on]                \n");
        Printf("skprmix    0/1              \n");
        return -1;
    }

    u1MixFlag = (UINT8)StrToInt(szArgv[1]);
    AUD_DspAuxMixToMain((BOOL)u1MixFlag);
    return 1;
}
#endif

#ifdef CC_AUD_PCM_LINE_IN_LPF
static INT32 _AudDSPSetPCMLineInLPF(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Flag;

    if (i4Argc != 2)
    {
        Printf("Usage: PCM Line-in LPF.   [off/on]   \n");
        return -1;
    }

    u1Flag = (UINT8)StrToInt(szArgv[1]);
    AUD_DSPSetPCMLineInLPF((BOOL)u1Flag);
    return 1;
}
#endif

#ifdef CC_AUD_HDMI_SPDIF_CFG
static INT32 _AudHDMICfgSPDIFCLI(INT32 i4Argc, const CHAR **szArgv)
{
    AUD_OUTPUT_STATE u1OutputState;
    if (i4Argc < 2)
    {
        Printf("0: DigitalMute_AnalogMute.\n");
        Printf("1: DigitalUnMute_AnalogMute.\n");
        Printf("2: DigitalUnMute_AnalogUnMute.\n");
        Printf("3: DigitalMute_AnalogUnMute.\n");
    }
    u1OutputState = (AUD_OUTPUT_STATE)StrToInt(szArgv[1]);
    AUD_HdmiCfgAnalogOutput(u1OutputState);
    return 0;
}
#endif

static CHAR * _aszAudOutputMode[] =
{
    "MONO",
    "MONO",                  /* 1/0 */
    "MONO+SAP",              /* 1+sub-language */
    "A2 DUAL",             /* 1+1 */
    "STEREO",                /* 2/0 */
    "STEREO+SAP",            /* 2+sub-language */
    "STEREO_DOLBY_SURROUND", /* 2/0, dolby surround */
    "SURROUND_2CH",          /* 2/1 */
    "SURROUND",              /* 3/1 */
    "3_0",                   /* 3/0 */
    "4_0",                   /* 2/2 */
    "5_1",                   /* 3/2 */
    "7_1",                   /* 5/2 */
    "FM_MONO_NICAM_MONO",
    "FM_MONO_NICAM_STEREO",
    "FM_MONO_NICAM_DUAL",
    "OTHERS",
    "5_0",                   /* 3/2 */
    "JOINT_STEREO",
    "FMRDO_MONO",
    "FMRDO_STEREO",

	"MONO_LFE",              /* 1/0.L */
    "DUAL_MONO_LFE",         /* 1+1.L */
    "STEREO_LFE",            /* 2/0.L */
    "SURROUND_2CH_LFE",      /* 2/1.L */
    "SURROUND_LFE",          /* 3/1.L */
    "3_1",                   /* 3/0.L */
    "4_1"                    /* 2/2.L */
};

static CHAR * _aszAudSys[] =
{
	"NONE_DETECTED ",
	"MTS           ",
	"FM_FM         ",
	"NTSC_M        ",
	"A2_BG         ",
	"A2_DK1        ",
	"A2_DK2        ",
	"A2_DK3        ",
	"PAL_I         ",
	"PAL_BG        ",
	"PAL_DK        ",
	"SECAM_L       ",
	"SECAM_L_PLUM  ",
	"SECAM_BG      ",
	"SECAM_DK    "
};

static INT32 _AudCmdAtvquery(INT32 i4Argc, const CHAR ** szArgv)
{
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    AUD_FMT_T eDecType;
    UINT8 u1DecId;
    TV_AUD_SYS_T eTVAudSrcSys;

    u1DecId = (AUD_GetTargetCountry() == COUNTRY_EU) ? AUD_DEC_AUX : AUD_DEC_MAIN;
    Printf("  \n");

    AUD_GetDecodeType(u1DecId, &eStreamFrom, &eDecType);
    if  (eStreamFrom != AUD_STREAM_FROM_ANALOG_TUNER)
    {
        Printf("Current input source is not ATV\n");
        return -1 ;
    }
    UNUSED(eDecType);

    eTVAudSrcSys = AUD_DspGetTvSrcMode();

    Printf("Detected channel system:  %s\n",_aszAudSys[eTVAudSrcSys]);
    //Printf("Detected channel system:  %s\n",_aszAudSys[DSP_GetAtvTvSystem()]);
    Printf("Detected sound mode:      %s\n",_aszAudOutputMode[DSP_GetAudChlInfo(AUD_DEC_MAIN)]);
    return 1;
}
static INT32 _AudCmdAtvTvSys(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Sys;
    AUD_FMT_T eAudDecFormat;
    UINT8 u1DecId;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (i4Argc < 2)
    {
        DSP_LogAtvTvSys();
        Printf("     \n");
        Printf("Usage: sys  [sys]\n");
        Printf("        1: BTSC          8: PAL_I\n");
        Printf("        2: FMFM          9: PAL_BG\n");
        Printf("        3: A2_KOREA     10: PAL_DK\n");
        Printf("        4: A2_BG        11: SECAM_L\n");
        Printf("        5: A2_DK1       12: SECAM_L' \n");
        Printf("        6: A2_DK2       13: SECAM_BG\n");
        Printf("        7: A2_DK3       14: SECAM_DK\n");

        return -1;
    }

    u1Sys = (UINT8)StrToInt(szArgv[1]);

    switch ((TV_AUD_SYS_T)u1Sys)
    {
        case SV_MTS:
            eAudDecFormat = AUD_FMT_MTS;
            break;
        case SV_FM_FM:
            eAudDecFormat = AUD_FMT_FMFM;
            break;
        case SV_NTSC_M:
        case SV_A2_BG:
        case SV_A2_DK1:
        case SV_A2_DK2:
        case SV_A2_DK3:
            eAudDecFormat = AUD_FMT_A2;
            break;
        case SV_PAL_I:
        case SV_PAL_BG:
        case SV_PAL_DK:
        case SV_SECAM_L:
        case SV_SECAM_L_PLUM:
        case SV_SECAM_BG:
        case SV_SECAM_DK:
            eAudDecFormat = AUD_FMT_PAL;
            break;
        default:
            Printf("Invalid sound system.\n");
            return -1;
            //break;
    }

        u1DecId = (AUD_GetTargetCountry() == COUNTRY_EU) ? AUD_DEC_AUX:AUD_DEC_MAIN;
        AUD_DSPCmdStop(u1DecId);
        AUD_DspSetTvSrcMode((TV_AUD_SYS_T)u1Sys);
        AUD_SetDecType(u1DecId, AUD_STREAM_FROM_ANALOG_TUNER, eAudDecFormat);
        x_thread_delay(100);
        AUD_DSPCmdPlay(u1DecId);

    return 1;
}

#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
static INT32 _AudCmdAtvAutoTvSys(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Sys,u1Country;
    UNUSED(i4Argc);
    UNUSED(szArgv);
    AUD_FMT_T eAudDecFormat;
    UINT8 u1DecId;
    UINT32 u4TvSysMask, u4AudSysMask;
    ISO_3166_COUNT_T country_code;
    if (i4Argc < 3)
    {
        DSP_LogAtvTvSys();
        Printf("     \n");
        Printf("Usage: asys  [sys] [country]\n");
        Printf("[sys] \n");
        Printf("        1: MTS          8: PAL_I\n");
        Printf("        2: FMFM          9: PAL_BG\n");
        Printf("        3: A2_KOREA     10: PAL_DK\n");
        Printf("        4: A2_BG        11: SECAM_L\n");
        Printf("        5: A2_DK1       12: SECAM_L' \n");
        Printf("        6: A2_DK2       13: SECAM_BG\n");
        Printf("        7: A2_DK3       14: SECAM_DK\n");
        Printf("[country] \n");
        Printf("        1: UK\n");
        Printf("        2: France\n");
        Printf("        3: Russia\n");
        Printf("        4: Others\n");

        return -1;
    }

    u1Sys = (UINT8)StrToInt(szArgv[1]);
    u1Country = (UINT8)StrToInt(szArgv[2]);
    switch ((TV_AUD_SYS_T)u1Sys)
    {
        case SV_A2_BG:
            u4TvSysMask = AUD_TV_SYS_MASK_B |
								AUD_TV_SYS_MASK_G|
								AUD_TV_SYS_MASK_A2	;
            u4AudSysMask = AUD_SYS_MASK_FM_A2;
            break;
        case SV_A2_DK1:
            u4TvSysMask = AUD_TV_SYS_MASK_D |
								AUD_TV_SYS_MASK_K |
								AUD_TV_SYS_MASK_A2	;
	     u4AudSysMask = AUD_SYS_MASK_FM_A2_DK1;
            break;

        case SV_A2_DK2:
            u4TvSysMask = AUD_TV_SYS_MASK_D |
                                   AUD_TV_SYS_MASK_K |
                                   AUD_TV_SYS_MASK_A2  ;
            u4AudSysMask = AUD_SYS_MASK_FM_A2_DK2;
               break;
        case SV_A2_DK3:
            u4TvSysMask = AUD_TV_SYS_MASK_D |
                                   AUD_TV_SYS_MASK_K |
                                   AUD_TV_SYS_MASK_A2  ;
            u4AudSysMask = AUD_SYS_MASK_FM_A2;
               break;
        case SV_PAL_I:
	     u4TvSysMask = AUD_TV_SYS_MASK_I |
								AUD_TV_SYS_MASK_PAL;
	     u4AudSysMask = AUD_SYS_MASK_FM_MONO |
																AUD_SYS_MASK_NICAM;
            break;
        case SV_PAL_BG:
            u4TvSysMask = AUD_TV_SYS_MASK_B |
                                    AUD_TV_SYS_MASK_G |
                                    AUD_TV_SYS_MASK_PAL;
            u4AudSysMask = AUD_SYS_MASK_FM_MONO |
                                                                    AUD_SYS_MASK_NICAM;
            break;

        case SV_PAL_DK:
            u4TvSysMask = AUD_TV_SYS_MASK_D |
								AUD_TV_SYS_MASK_K |
								AUD_TV_SYS_MASK_PAL;
	     u4AudSysMask = AUD_SYS_MASK_FM_MONO |
																AUD_SYS_MASK_NICAM;
	     break;
        case SV_SECAM_L:
        case SV_SECAM_L_PLUM:
        case SV_SECAM_BG:
        case SV_SECAM_DK:
	     u4TvSysMask = AUD_TV_SYS_MASK_L |
															AUD_TV_SYS_MASK_PAL;
	     u4AudSysMask = AUD_SYS_MASK_FM_MONO |
																AUD_SYS_MASK_NICAM;
            break;
        default:
            Printf("Invalid sound system.\n");
            return -1;
            //break;
    }

    if (u1Country == 1)
    {
        x_strncpy( (CHAR *) country_code, "GBR", ISO_3166_COUNT_LEN);

    }
    else if (u1Country == 2)
    {
        x_strncpy( (CHAR *) country_code, "FRA", ISO_3166_COUNT_LEN);
    }
    else if (u1Country == 3)
    {
        x_strncpy( (CHAR *) country_code, "RUS", ISO_3166_COUNT_LEN);
    }
    else if (u1Country == 4)
    {
        x_strncpy( (CHAR *) country_code, "DEU", ISO_3166_COUNT_LEN);
    }
    else
    {
        Printf("Invalid country.\n");
        return -1;
    }

        u1DecId = ((GetTargetCountry() == COUNTRY_EU)||(GetTargetCountry() == COUNTRY_BZ)) ? AUD_DEC_AUX : AUD_DEC_MAIN;
        AUD_DSPCmdStop(u1DecId);

        //do sound system detection
        Aud_SetTvAudSystemMask(AUD_DEC_MAIN, u4TvSysMask, u4AudSysMask, country_code);

        eAudDecFormat = AudAtvFmtDetection();
        LOG(0,"#### Detect TV Sys = %d #######\n ", eAudDecFormat);
        if (eAudDecFormat != AUD_FMT_UNKNOWN)
        {
            AUD_SetDecType(u1DecId, AUD_STREAM_FROM_ANALOG_TUNER, eAudDecFormat);
            AUD_DSPCmdPlay(u1DecId);
        }
    return 1;
}
#endif


static CHAR * _aszAudSoundMode[] =
{
    "MONO",
    "MONO",
    "STEREO",
    "SUB_LANG",
    "DUAL1",
    "DUAL2",
    "NICAM_MONO",
    "NICAM_STEREO",
    "NICAM_DUAL1",
    "NICAM_DUAL2",
    "A2_DUAL1_DUAL2",
    "NICAM_DUAL1_DUAL2",
    "FM_MONO",
    "FM_STEREO"
};

static INT32 _AudCmdAtvSoundMode(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1UserMode;
    UINT8 u1DecId;

    AUD_DEC_STREAM_FROM_T eStreamFrom;
    AUD_FMT_T eDecType;
    u1DecId = (AUD_GetTargetCountry() == COUNTRY_EU) ? AUD_DEC_AUX : AUD_DEC_MAIN;
    Printf("  \n");

    AUD_GetDecodeType(u1DecId, &eStreamFrom, &eDecType);
    if  (eStreamFrom != AUD_STREAM_FROM_ANALOG_TUNER)
    {
        Printf("Current input source is not ATV\n");
		return -1;
    }
    UNUSED(eDecType);

    if (i4Argc < 2)
    {
        Printf("Detected sound mode is  %s\n",_aszAudOutputMode[DSP_GetAudChlInfo(AUD_DEC_MAIN)]);
        Printf("Current sound mode is  %s\n\n",_aszAudSoundMode[AUD_DspGetSoundMode(AUD_DEC_MAIN)]);
        Printf("Usage: um [user mode]  \n");
        Printf("[user mode]     1:MONO        6:NICAM_MONO   \n");
        Printf("                2:STEREO      7:NICAM_STEREO  \n");
        Printf("                3:SAP         8:NICAM_DUAL1    \n");
        Printf("                4:A2_DUAL1    9:NICAM_DUAL2   \n");
        Printf("                5:A2_DUAL2   10:A2_DUAL1+A2_DUAL2   \n");

        return -1;
    }

    u1UserMode = (UINT8)StrToInt(szArgv[1]);
   AUD_DspSetAtvOutputMode(u1DecId, (AUD_SOUND_MODE_T)u1UserMode);

    return 1;
}

static INT32 _AudCmdFMRadioSoundMode(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1UserMode;
    UINT8 u1DecId;

    AUD_DEC_STREAM_FROM_T eStreamFrom;
    AUD_FMT_T eDecType;
    u1DecId = AUD_DEC_MAIN;
    Printf("  \n");

    AUD_GetDecodeType(u1DecId, &eStreamFrom, &eDecType);
    if  (eStreamFrom != AUD_STREAM_FROM_ANALOG_TUNER)
    {
        Printf("Current input source is not FM radio\n");
		return -1;
    }
    UNUSED(eDecType);

    if (i4Argc < 2)
    {
        Printf("Detected sound mode is  %s\n",_aszAudOutputMode[DSP_GetAudChlInfo(AUD_DEC_MAIN)]);
        Printf("Current sound mode is  %s\n\n",_aszAudSoundMode[AUD_DspGetSoundMode(AUD_DEC_MAIN)]);
        Printf("Usage: fm2 [user mode]  \n");
        Printf("[user mode]     1:MONO        2:STEREO   \n");

        return -1;
    }

    u1UserMode = (UINT8)StrToInt(szArgv[1]);
    u1UserMode = u1UserMode + AUD_SOUND_MODE_FM_MONO - 1; // 0 is mono and 1 is stereo for fmrdodec
   AUD_DspSetAtvOutputMode(u1DecId, (AUD_SOUND_MODE_T)u1UserMode);

    return 1;
}
static INT32 _AudCmdAtvFacMts(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1Item;
    UINT16 u2value;
    UINT8 u1DecId;
    UINT8 u1Tmp;

    u1DecId = (AUD_GetTargetCountry() == COUNTRY_EU) ? AUD_DEC_AUX : AUD_DEC_MAIN;

    if (i4Argc >= 3)
    {
        u1Item = (UINT8)StrToInt(szArgv[1]);
        u2value = (UINT16)StrToInt(szArgv[2]);
        switch (u1Item)
            {
            case 0:
                AUD_DspSetMtsDetection(AUD_FACTORY_MTS_NUM_CHECK,(UINT16)u2value);
                break;
            case 1:
                AUD_DspSetMtsDetection (AUD_FACTORY_MTS_NUM_PILOT,(UINT16)u2value);
                break;
            case 2:
                AUD_DspSetMtsDetection (AUD_FACTORY_MTS_PILOT_HIGH,(UINT16)u2value);
                break;
            case 3:
                AUD_DspSetMtsDetection (AUD_FACTORY_MTS_PILOT_LOW,(UINT16)u2value);
                break;
            case 4:
                AUD_DspSetMtsDetection (AUD_FACTORY_MTS_NUM_SAP,(UINT16)u2value);
                break;
            case 5:
                AUD_DspSetMtsDetection (AUD_FACTORY_MTS_SAP_HIGH,(UINT16)u2value);
                break;
            case 6:
                AUD_DspSetMtsDetection (AUD_FACTORY_MTS_SAP_LOW,(UINT16)u2value);
                break;
            case 7:
                AUD_DSPCmdStop(u1DecId);
                AUD_DspSetHdevMode(AUD_ATV_DECODER_MTS, (BOOL)u2value);
                AUD_DSPCmdPlay(u1DecId);
                break;
            case 8:
                AUD_DspSetCarrierShiftMode(AUD_ATV_DECODER_MTS, (BOOL)u2value);
                break;
            case 9:
                AUD_DspSetFMSatuMuteMode(AUD_ATV_DECODER_MTS, (BOOL)u2value);
                break;
            case 10:
                AUD_DspSetFMSatuMuteTh (AUD_FACTORY_SATU_MUTE_HIGHER,(UINT8)u2value);
                break;
            case 11:
                AUD_DspSetFMSatuMuteTh (AUD_FACTORY_SATU_MUTE_LOWER,(UINT8)u2value);
                break;
            case 12:
                AUD_DspSetFmMute(AUD_ATV_DECODER_MTS,AUD_FACTORY_FM_MUTE,(UINT8)u2value);
                break;
            case 13:
                AUD_DspSetFmMute(AUD_ATV_DECODER_MTS,AUD_FACTORY_FM_MUTE_HIGH,(UINT8)u2value);
                break;
            case 14:
                AUD_DspSetFmMute(AUD_ATV_DECODER_MTS,AUD_FACTORY_FM_MUTE_LOW,(UINT8)u2value);
                break;
            case 15:
                AUD_DspSetMtsMonoFineVolume((UINT8)u2value);
                break;
            case 16:
                AUD_DspSetSAPFineVolume((UINT8)u2value);
                break;
            case 17:
                AUD_DspSetMtsPilotDetection((BOOL)u2value);
                break;
            case 18:
                AUD_DspSetSapMute(AUD_FACTORY_SAP_MUTE_LOWER,(UINT8)u2value);
                break;
            case 19:
                AUD_DspSetSapMute(AUD_FACTORY_SAP_MUTE_HIGHER,(UINT8)u2value);
                break;
            case 20:
                AUD_DspSetSapMute(AUD_FACTORY_SAP_HP_MUTE_HIGHER,(UINT8)u2value);
                break;
            case 21:
                AUD_DspSetSapMute(AUD_FACTORY_SAP_HP_MUTE_LOWER,(UINT8)u2value);
                break;
            case 22:
                u2value = (u2value << 1) + (UINT16)StrToInt(szArgv[3]);
                AUD_DspSetSapMute(AUD_FACTORY_SAP_FILTER_SEL,(UINT8)u2value);
                break;
            case 23:
                AUD_DspSetSapMute(AUD_FACTORY_SAP_RATIO,(UINT8)u2value);
                break;

            default:
                break;
            }
        return 1;
    }
    else
    {
        Printf("Usage: mts {[item indx]} [value] \n");
        Printf("  \n");
        Printf("             [item indx]                    [value range][current value]      \n");
        Printf("    0  :  numbers of check                     30~80         %d     \n",AUD_DspGetMtsDetection(AUD_FACTORY_MTS_NUM_CHECK));
        Printf("    1  :  numbers of pilot                      0~50         %d      \n",AUD_DspGetMtsDetection(AUD_FACTORY_MTS_NUM_PILOT));
        Printf("    2  :  higher pilot threshold              120~200       %d    \n",AUD_DspGetMtsDetection(AUD_FACTORY_MTS_PILOT_HIGH));
        Printf("    3  :  lower pilot threshold                80~150       %d     \n",AUD_DspGetMtsDetection(AUD_FACTORY_MTS_PILOT_LOW));
        Printf("    4  :  numbers of SAP                        0~50         %d      \n",AUD_DspGetMtsDetection(AUD_FACTORY_MTS_NUM_SAP));
        Printf("    5  :  higher SAP threshold                150~210       %d    \n",AUD_DspGetMtsDetection(AUD_FACTORY_MTS_SAP_HIGH));
        Printf("    6  :  lower SAP threshold                  70~130       %d    \n",AUD_DspGetMtsDetection(AUD_FACTORY_MTS_SAP_LOW));
        Printf("    7  :  HDEV mode                            0 or 1         %d     \n",AUD_DspGetHdevMode(AUD_ATV_DECODER_MTS));
        Printf("    8  :  carrier shift function               0 or 1         %d      \n",AUD_DspGetCarrierShiftMode(AUD_ATV_DECODER_MTS));
        Printf("    9  :  FM saturation mute                   0 or 1         %d    \n",AUD_DspGetFMSatuMuteMode(AUD_ATV_DECODER_MTS));
        Printf("   10  :  higher saturation mute threshold     25~256        %d      \n",AUD_DspGetFMSatuMuteTh (AUD_FACTORY_SATU_MUTE_HIGHER));
        Printf("   11  :  lower saturation mute threshold       1~25         %d    \n",AUD_DspGetFMSatuMuteTh (AUD_FACTORY_SATU_MUTE_LOWER));
        Printf("   12  :  FM carrier mute mode                 0 or 1         %d     \n",AUD_DspGetFmMute(AUD_ATV_DECODER_MTS,AUD_FACTORY_FM_MUTE));
        Printf("   13  :  higher carrier mute threshold       110~180       %d    \n",AUD_DspGetFmMute(AUD_ATV_DECODER_MTS,AUD_FACTORY_FM_MUTE_HIGH));
        Printf("   14  :  lower carrier mute threshold         20~80        %d   \n",AUD_DspGetFmMute(AUD_ATV_DECODER_MTS,AUD_FACTORY_FM_MUTE_LOW));
        Printf("   15  :  fine tune mono&stereo volume          0~40         %d     \n",AUD_DspGetMtsMonoFineVolume());
        Printf("   16  :  fine tune SAP volume                  0~40         %d    \n",AUD_DspGetSapFineVolume());
        Printf("   17  :  pilot offset detection               0 or 1         %d     \n",AUD_DspGetMtsPilotDetection());
        Printf("   18  :  SAP noise mute lower threshold       0~110         %d     \n",AUD_DspGetSapMute(AUD_FACTORY_SAP_MUTE_LOWER));
        Printf("   19  :  SAP noise mute higher threshold      0~256          %d     \n",AUD_DspGetSapMute(AUD_FACTORY_SAP_MUTE_HIGHER));
        Printf("   20  :  SAP HPF mute higher threshold       32~256          %d     \n",AUD_DspGetSapMute(AUD_FACTORY_SAP_HP_MUTE_HIGHER));
        Printf("   21  :  SAP HPF mute lower threshold         0~128         %d     \n",AUD_DspGetSapMute(AUD_FACTORY_SAP_HP_MUTE_LOWER));
        u1Tmp = AUD_DspGetSapMute(AUD_FACTORY_SAP_FILTER_SEL);
        Printf("   22  :  SAP Filter  [0:auto/1:manual] [0:big/1:small]  %d %d  \n", ((u1Tmp & 0x02)>>1), (u1Tmp & 0x01));
        Printf("   23  :  SAP ratio selection                0x0 ~ 0x30          %x     \n",AUD_DspGetSapMute(AUD_FACTORY_SAP_RATIO));
        return -1;
    }
}

static INT32 _AudCmdAtvFacA2(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1Item;
    UINT16 u2value;

    if (i4Argc == 3)
    {
        u1Item = (UINT8)StrToInt(szArgv[1]);
        u2value = (UINT16)StrToInt(szArgv[2]);
        switch (u1Item)
            {
            case 0:
                AUD_DspSetA2Detection (AUD_FACTORY_A2_NUM_CHECK,(UINT16)u2value);
                break;
            case 1:
                AUD_DspSetA2Detection (AUD_FACTORY_A2_NUM_DOUBLE,(UINT16)u2value);
                break;
            case 2:
                AUD_DspSetHdevMode(AUD_ATV_DECODER_A2, (BOOL)u2value);
                break;
            case 3:
                AUD_DspSetCarrierShiftMode(AUD_ATV_DECODER_A2, (BOOL)u2value);
                break;
            case 4:
                AUD_DspSetFmMute(AUD_ATV_DECODER_A2,AUD_FACTORY_FM_MUTE,(UINT8)u2value);
                break;
            case 5:
                AUD_DspSetFmMute(AUD_ATV_DECODER_A2,AUD_FACTORY_FM_MUTE_HIGH,(UINT8)u2value);
                break;
            case 6:
                AUD_DspSetFmMute(AUD_ATV_DECODER_A2,AUD_FACTORY_FM_MUTE_LOW,(UINT8)u2value);
                break;
            case 7:
                AUD_DspSetA2FineVolume((UINT8)u2value);
                break;
            case 8:
                AUD_DspSetFMSatuMuteMode(AUD_ATV_DECODER_A2, (BOOL)u2value);
                break;
            case 9:
                AUD_DspSetNonEUMode((BOOL)u2value);
                break;
            case 10:
                AUD_DspSetA2Threshold(AUD_FACTORY_A2_MONO2STEREO,(UINT16)u2value);
                break;
            case 11:
                AUD_DspSetA2Threshold(AUD_FACTORY_A2_STEREO2MONO,(UINT16)u2value);
                break;
	     case 12:
                AUD_DspSetA2Threshold(AUD_FACTORY_A2_WEAK_RF_LVL_MONO2STEREO,(UINT16)u2value);
                break;
            case 13:
                AUD_DspSetA2Threshold(AUD_FACTORY_A2_WEAK_RF_LVL_STEREO2MONO,(UINT16)u2value);
                break;
            default:
                break;
            }
        return 1;
    }
    else
    {
        Printf("Usage: a2 {[item indx]} [value] \n");
        Printf("  \n");
        Printf("             [item indx]                 [value range][current value]   \n");
        Printf("    0  :  numbers of check                   1~60          %d         \n",AUD_DspGetA2Detection (AUD_FACTORY_A2_NUM_CHECK));
        Printf("    1  :  numbers of double check            5~60          %d        \n",AUD_DspGetA2Detection (AUD_FACTORY_A2_NUM_DOUBLE));
        Printf("    2  :  HDEV mode                         0 or 1          %d      \n",AUD_DspGetHdevMode(AUD_ATV_DECODER_A2));
        Printf("    3  :  carrier shift function            0 or 1          %d    \n",AUD_DspGetCarrierShiftMode(AUD_ATV_DECODER_A2));
        Printf("    4  :  FM carrier mute mode              0 or 1          %d    \n",AUD_DspGetFmMute(AUD_ATV_DECODER_A2,AUD_FACTORY_FM_MUTE));
        Printf("    5  :  higher carrier mute threshold      0~96          %d  \n",AUD_DspGetFmMute(AUD_ATV_DECODER_A2,AUD_FACTORY_FM_MUTE_HIGH));
        Printf("    6  :  lower carrier mute threshold       0~96          %d     \n",AUD_DspGetFmMute(AUD_ATV_DECODER_A2,AUD_FACTORY_FM_MUTE_LOW));
        Printf("    7  :  fine tune A2 volume                0~40          %d    \n",AUD_DspGetA2FineVolume());
        Printf("    8  :  FM saturation mute                0 or 1          %d  \n",AUD_DspGetFMSatuMuteMode(AUD_ATV_DECODER_A2));
        Printf("    9  :  non-EU mode                      0 or 1          %d      \n",AUD_DspGetNonEUMode());
        Printf("   10  :  mono to stereo threshold(pilot)         64~1024     %d     \n",AUD_DspGetA2Threshold(AUD_FACTORY_A2_MONO2STEREO));
        Printf("   11  :  stereo to mono threshold(pilot)          0~512        %d      \n",AUD_DspGetA2Threshold(AUD_FACTORY_A2_STEREO2MONO));
        Printf("   12  :  mono to stereo threshold(weak RF)   0~57344     %d     \n",AUD_DspGetA2Threshold(AUD_FACTORY_A2_WEAK_RF_LVL_MONO2STEREO));
        Printf("   13  :  stereo to mono threshold(weak RF)   256~65535        %d      \n",AUD_DspGetA2Threshold(AUD_FACTORY_A2_WEAK_RF_LVL_STEREO2MONO));
        return -1;
    }
}

static INT32 _AudCmdAtvFacPal(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1Item;
    UINT16 u2value;
    UINT8 u1DecId;

    u1DecId = (AUD_GetTargetCountry() == COUNTRY_EU) ? AUD_DEC_AUX : AUD_DEC_MAIN;

    if (i4Argc == 3)
    {
        u1Item = (UINT8)StrToInt(szArgv[1]);
        u2value = (UINT16)StrToInt(szArgv[2]);
        switch (u1Item)
            {
            case 0:
                AUD_DspSetPalDetection (AUD_FACTORY_PAL_PARITY_ERROR,(UINT16)u2value);
                break;
            case 1:
                AUD_DSPCmdStop(u1DecId);
                AUD_DspSetHdevMode(AUD_ATV_DECODER_PAL, (BOOL)u2value);
                AUD_DSPCmdPlay(u1DecId);
                break;
            case 2:
                AUD_DspSetAmMute(AUD_FACTORY_PAL_AM_MUTE,(UINT8)u2value);
                break;
            case 3:
                AUD_DspSetAmMute(AUD_FACTORY_PAL_AM_MUTE_HIGH,(UINT8)u2value);
                break;
            case 4:
                AUD_DspSetAmMute(AUD_FACTORY_PAL_AM_MUTE_LOW,(UINT8)u2value);
                break;
            case 5:
                AUD_DspSetCarrierShiftMode(AUD_ATV_DECODER_PAL, (BOOL)u2value);
                break;
            case 6:
                AUD_DspSetFmMute(AUD_ATV_DECODER_PAL,AUD_FACTORY_FM_MUTE,(UINT8)u2value);
                break;
            case 7:
                AUD_DspSetFmMute(AUD_ATV_DECODER_PAL,AUD_FACTORY_FM_MUTE_HIGH,(UINT8)u2value);
                break;
            case 8:
                AUD_DspSetFmMute(AUD_ATV_DECODER_PAL,AUD_FACTORY_FM_MUTE_LOW,(UINT8)u2value);
                break;
            case 9:
                AUD_DspSetPALFineVolume((UINT8)u2value);
                break;
            case 10:
                AUD_DspSetAMFineVolume((UINT8)u2value);
                break;
            case 11:
                AUD_DspSetNicamFineVolume((UINT8)u2value);
                break;
            case 12:
                AUD_DspSetFMSatuMuteMode(AUD_ATV_DECODER_PAL,(BOOL)u2value);
                break;
            case 13:
                AUD_DspSetNonEUMode((BOOL)u2value);
                break;
            default:
                break;
            }
        return 1;
    }
    else
    {
        if (i4Argc == 2)
        {
            u1Item = (UINT8)StrToInt(szArgv[1]);
            if (u1Item == 14)
            {
                UNUSED(AUD_GetNicamStatus());
                return 1 ;
            }
        }
        Printf("Usage: pal {[item indx]} [value] \n");
        Printf("  \n");
        Printf("             [item indx]                [value range]  [current value]     \n");
        Printf("    0  :  parity error threshold           600~4096      %d    \n",AUD_DspGetPalDetection (AUD_FACTORY_PAL_PARITY_ERROR));
        Printf("    1  :  HDEV mode                        0 or 1           %d     \n",AUD_DspGetHdevMode(AUD_ATV_DECODER_PAL));
        Printf("    2  :  AM mute mode                     0 or 1           %d     \n",AUD_DspGetAmMute(AUD_FACTORY_PAL_AM_MUTE));
        Printf("    3  :  higher AM mute threshold          0~255          %d    \n",AUD_DspGetAmMute(AUD_FACTORY_PAL_AM_MUTE_HIGH));
        Printf("    4  :  lower AM mute threshold           0~255          %d  \n",AUD_DspGetAmMute(AUD_FACTORY_PAL_AM_MUTE_LOW));
        Printf("    5  :  carrier shift function           0 or 1           %d   \n",AUD_DspGetCarrierShiftMode(AUD_ATV_DECODER_PAL));
        Printf("    6  :  FM carrier mute mode             0 or 1           %d  \n",AUD_DspGetFmMute(AUD_ATV_DECODER_PAL,AUD_FACTORY_FM_MUTE));
        Printf("    7  :  higher carrier mute threshold     0~96           %d   \n",AUD_DspGetFmMute(AUD_ATV_DECODER_PAL,AUD_FACTORY_FM_MUTE_HIGH));
        Printf("    8  :  lower carrier mute threshold      0~96           %d    \n",AUD_DspGetFmMute(AUD_ATV_DECODER_PAL,AUD_FACTORY_FM_MUTE_LOW));
        Printf("    9  :  fine tune FM volume               0~40           %d    \n",AUD_DspGetPALFineVolume());
        Printf("   10  :  fine tune AM volume               0~40           %d    \n",AUD_DspGetAMFineVolume());
        Printf("   11  :  fine tune NICAM volume            0~40           %d  \n",AUD_DspGetNicamFineVolume());
        Printf("   12  :  FM saturation mute               0 or 1           %d  \n",AUD_DspGetFMSatuMuteMode(AUD_ATV_DECODER_PAL));
        Printf("   13  :  non-EU mode                      0 or 1           %d  \n",AUD_DspGetNonEUMode());
        Printf("   14  :  check NICAM status            read only     ");
        UNUSED(AUD_GetNicamStatus());
        Printf("  \n");
        return -1;
    }
}

static CHAR * _aszAudFormat[] = {
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
    "MLP"
};

/* Audio types. */
static CHAR * _aszAudACMode[] =
{
    "UNKNOWN",
    "MONO",
    "MONO_SUB",
    "DUAL_MONO",
    "STEREO",
    "STEREO_SUB",
    "STEREO_DOLBY_SURROUND",
    "SURROUND_2CH",
    "SURROUND",
    "3.0",
    "4.0",
    "5.1",
    "7.1",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "OTHERS",
    "5.0"
} ;

static INT32 _AudCmdAtvHdevDetQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 HdevDetResult;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    HdevDetResult = AudCmdAtvHdevDetQuery();
    Printf("CLI Result, HdevDetResult = %d!\n", HdevDetResult);

    return 1;

}

static INT32 _AudCmdDtvQuery (INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1DecId;
    AUD_DEC_INFO_T  rAudInfo;
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    AUD_FMT_T eDecType;
    u1DecId = (AUD_GetTargetCountry() == COUNTRY_EU) ? AUD_DEC_AUX : AUD_DEC_MAIN;
    Printf("  \n");

    AUD_GetDecodeType(u1DecId, &eStreamFrom, &eDecType);
    if  (eStreamFrom != AUD_STREAM_FROM_DIGITAL_TUNER)
    {
        Printf("Current input source is not DTV\n");
        return -1 ;
    }

    if (!AUD_IsDecoderPlay(u1DecId))
    {
        Printf("There is no audio decoder playing \n");
        return -1 ;
    }

    UNUSED(eDecType);

    AUD_GetDtvAudInfo(u1DecId, &rAudInfo);

    if (rAudInfo.e_aud_fmt > AUD_FMT_MLP)
    {
        rAudInfo.e_aud_fmt = AUD_FMT_UNKNOWN;
    }

    Printf(" Current audio format:  %s  \n",_aszAudFormat[rAudInfo.e_aud_fmt]);
    if(rAudInfo.e_aud_fmt == AUD_FMT_AC3)
    {
        Printf(" Extra info. of AC3: \n ");
	 Printf("  >>>Current audio decode type: ");
        switch(rAudInfo.e_dec_type)
        {
        case AUD_DECODE_TYPE_AC3:
	     Printf("AC3 \n");
	     break;
	 case AUD_DECODE_TYPE_EAC3:
	     Printf("E-AC3 \n");
	     break;
        default:
	     Printf("Unknown \n");
	     break;
        }
        Printf("   >>>Current stream");
        switch (rAudInfo.e_aud_type)
        {
        case AUD_TYPE_DUAL_MONO: // 1+1
            Printf("(1+1) contains no LFE \n");
            break;
        case AUD_TYPE_MONO: // 1/0
            Printf("(1/0) contains no LFE \n");
            break;
        case AUD_TYPE_STEREO: // 2/0
            Printf("(2/0) contains no LFE \n");
            break;
        case AUD_TYPE_3_0: // 3/0       
            Printf("(3/0) contains no LFE \n");
            break;
        case AUD_TYPE_SURROUND_2CH: // 2/1
            Printf("(2/1) contains no LFE \n");
            break;
        case AUD_TYPE_SURROUND: // 3/1
            Printf("(3/1) contains no LFE \n");
            break;
        case AUD_TYPE_4_0: // 2/2
            Printf("(2/2) contains no LFE \n");
            break;
        case AUD_TYPE_5_0: // 3/2
            Printf("(3/2) contains no LFE \n");
            break;
        case AUD_TYPE_MONO_LFE: // 1/0.L
            Printf("(1/0.L) contains LFE \n");
            break;
        case AUD_TYPE_STEREO_LFE: // 2/0.L
            Printf("(2/0.L) contains LFE \n");
            break;
        case AUD_TYPE_3_1: // 3/0.L
            Printf("(3/0.L) contains LFE \n");
            break;
        case AUD_TYPE_SURROUND_2CH_LFE: // 2/1.L
            Printf("(2/1.L) contains LFE \n");
            break;
        case AUD_TYPE_SURROUND_LFE: // 3/1.L
            Printf("(3/1.L) contains LFE \n");
            break;
        case AUD_TYPE_4_1: // 2/2.L
            Printf("(2/2.L) contains LFE \n");
            break;
        case AUD_TYPE_5_1: // 3/2.L
            Printf("(3/2.L) contains LFE \n");
            break;
        default: //
            Printf(" contains no LFE \n");
            break;
        }
    }
    Printf(" Sampling rate:         %d Hz  \n",rAudInfo.ui4_sample_rate);
    Printf(" Data rate:             %d bps\n",rAudInfo.ui4_data_rate);
    Printf(" Audio decoding mode:   %s\n",_aszAudACMode[rAudInfo.e_aud_type]);

    return 1 ;
}

static INT32 _AudCmdDtvPTSDelay(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1DecId;
    UINT8 uDelayFactor;

    if (i4Argc < 3)
    {
        Printf("Usage: PTSD [decoder id] [factor]\n");
        Printf("[decoder id]   0:FIRST                 1:SECOND \n");
        Printf("[factor] 0 ~ 255 \n\n");
        Printf("Audio start decoding PTS would increased (PTS_AC3_CNT * factor)\n");
        return -1;
    }
    else
    {
        u1DecId = StrToInt(szArgv[1]);
        uDelayFactor = StrToInt(szArgv[2]);
        AUD_AudioPTSDelayEnable(u1DecId, uDelayFactor);
	    Printf(" Set DelayFactor to Dec(%d) = %d\n", u1DecId, uDelayFactor);
	    return 1;
    }
}

static INT32 _AudCmdFMRadioDetect (INT32 i4Argc, const CHAR **szArgv)
{
    BOOL fm_det_result = 0;

    UNUSED(i4Argc);
    UNUSED(szArgv);
    fm_det_result = _AudFMRadioDetection();
    if (fm_det_result) Printf("CLI Result : FM channel detected!\n");
    else Printf("CLI Result : None FM channel detected!\n");

    return 1;
}

static INT32 _AudCmdFMRadioPlay (INT32 i4Argc, const CHAR **szArgv)
{

    UNUSED(i4Argc);
    UNUSED(szArgv);
    _AudPlayFMRadio();

    return 1;
}

static INT32 _AudCmdFMRadioQueryThre(INT32 i4Argc, const CHAR ** szArgv)
    {
         UINT32 u4Thre1,u4Thre2;
         INT32 i4Thre;

         u4Thre1 = vDspGetFMRadioNoiseThreshold();
         u4Thre2 = vDspGetFMRadioInBandThreshold();
         i4Thre = vDspGetFMRadioRatioThreshold();

         Printf("Noise Power Threshold = 0x%x \n",u4Thre1);
         Printf("In Band Power Threshold = 0x%x \n",u4Thre2);
         Printf("Ratio = %d	\n",i4Thre);

         return 1;
	}

static INT32 _AudCmdFMRadioSetFineVolume(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u1Vol;

    if (i4Argc < 2)
    {
        Printf("Usage: vol [vol index]\n");
        Printf("[vol index] 0 ~ 40, -10 ~ 10dB, 0.5dB per step\n");
        return -1;
    }

    u1Vol = (UINT8)StrToInt(szArgv[1]);
    AUD_DspSetFmRadioFineVolume((UINT8)u1Vol);

    return 1;
}

static INT32 _AudCmdFMRadioThre(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 uType;
    UINT32 u4Thre;

    if (i4Argc < 3)
    {
        Printf("Usage: fmthl [Type] [threshold]\n");
        Printf("[Type] 1: Noise Power   2: In band Power  3: Ratio\n");
        Printf("[threshold]     0x0 ~ 0x7fffff\n");
        return -1;
    }

    uType = (UINT8)StrToInt(szArgv[1]);
    u4Thre = (UINT32)StrToInt(szArgv[2]);
    if(uType == 1)
        vDspSetFMRadioNoiseThreshold(u4Thre);
    if(uType == 2)
        vDspSetFMRadioInBandThreshold(u4Thre);
    if(uType == 3)
        vDspSetFMRadioRatioThreshold((int)u4Thre);

    return 1;
}


static INT32 _AUD_MTS_CFO_IDX(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    AUD_MTS_CFO_IDX();

    return 1;
}

#if 0
#define AC3_KARA_DISABLE    ((UINT16)0x0001) /* disable karaoke mode */
#define AC3_KARA_AWARE      ((UINT16)0x0002) /* karaoke aware mode */
#define AC3_KARA_NONE       ((UINT16)0x0004) /* none (in capable mode)*/
#define AC3_KARA_V1         ((UINT16)0x0008) /* V1 only (in capable mode)*/
#define AC3_KARA_V2         ((UINT16)0x0010) /* V2 only (in capable mode)*/
#define AC3_KARA_BOTH       ((UINT16)0x0020) /* V1+V2   (in capable mode)*/
#define AC3_KARA_VOC_ASIST  ((UINT16)(0x0040) /* vocal assist mode */
#define AC3_KARA_NO_MELODY  ((UINT16)0x0080) /* melody off */
#endif

static INT32 _AudCmdDtvAC3KaraMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2DecId;
    UINT16 u2Mode;
    if (i4Argc < 3)
    {
        Printf("Usage: karaMode [decoder id] [karaoke mode]\n");
        Printf("[decoder id]   0:FIRST                 1:SECOND \n");
        Printf("[karaoke mode] 0:DISABLE               1:AWARE \n");
        Printf("               2:NONE (capable mode)   3:V1 ONLY (capable mode) \n");
        Printf("               4:V2 ONLY (capable mode)5:V1+V2 (capable mode) \n");
        return -1;
    }
    u2DecId = (UINT16)StrToInt(szArgv[1]);
    u2Mode = (UINT16)StrToInt(szArgv[2]);

    if(u2Mode<6)
    {
        AUD_DspAC3KaraMode(0x1<<u2Mode, u2DecId);
    }

    return 1;
};

#ifdef CC_AUD_SUPPORT_MS10
static INT32 _AudCmdDtvDDCControl (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2DecId;
    UINT16 u2Control;

    if (i4Argc < 3)
    {
        Printf("Usage: ddccontrol [decoder id] [control]\n");
        Printf("[decoder id]   0:FIRST                 1:SECOND \n");
        Printf("[control] DDC control \n");
        return -1;
    }
    u2DecId = (UINT16)StrToInt(szArgv[1]);
    u2Control = (UINT16)StrToInt(szArgv[2]);

    AUD_DspSetDDCControl (u2Control, u2DecId);

    return 1;
};

static INT32 _AudCmdDtvDDCStrmId (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2DecId;
    UINT16 u2StrmId;

    if (i4Argc < 3)
    {
        Printf("Usage: strmid [decoder id] [stream id]\n");
        Printf("[decoder id]   0:FIRST                 1:SECOND \n");
        Printf("[stream id] associated stream id \n");
        return -1;
    }
    u2DecId = (UINT16)StrToInt(szArgv[1]);
    u2StrmId = (UINT16)StrToInt(szArgv[2]);

    AUD_DspSetDDCStrmId (u2StrmId, u2DecId);

    return 1;
};

static INT32 _AudCmdDtvDDCMixer (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2DecId;
    UINT16 u2Mixer;

    if (i4Argc < 3)
    {
        Printf("Usage: mixer [decoder id] [stream id]\n");
        Printf("[decoder id]   0:FIRST                 1:SECOND \n");
        Printf("[mixer mode] 0: Main only, 1: Main+Assoc, 3: L = assoc, R = main \n");
        return -1;
    }
    u2DecId = (UINT16)StrToInt(szArgv[1]);
    u2Mixer = (UINT16)StrToInt(szArgv[2]);

    AUD_DspSetDDCMixer (u2Mixer, u2DecId);

    return 1;
};
#endif

#if 0
#define AC3_DM_STEREO           0 /* AC3 dual mono mode stereo*/
#define AC3_DM_L_MONO           1 /* AC3 dual mono mode left mono*/
#define AC3_DM_R_MONO           2 /* AC3 dual mono mode right mono*/
#define AC3_DM_M_MONO           3 /* AC3 dual mono mode mix mono*/
#endif
static INT32 _AudCmdDtvAc3DualMono(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2DecId;
    UINT16 u2Mode;
    if (i4Argc < 3)
    {
        Printf("Usage: dualmono [decoder id] [dual mono mode]\n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[dual mono mode] 0:STEREO                1:L MONO \n");
        Printf("                 2:R MONO                3:MIX MONO\n");
        return -1;
    }
    u2DecId = (UINT16)StrToInt(szArgv[1]);
    u2Mode = (UINT16)StrToInt(szArgv[2]);

    if(u2Mode<5)
    {
        AUD_DspAC3DualMono(u2Mode, u2DecId);
    }

    return 1;
};

static INT32 _AudCmdDtvDualMono(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Mode;

    if (i4Argc < 3)
    {
        if ((i4Argc == 2) && (szArgv[1][0] == 'q'))
        {
            Printf("[Dual/Mono Setting]\n");
            Printf("  DEC1: %d\n", AUD_DspGetMatrixOutMode(AUD_DEC_MAIN));
            Printf("  DEC2: %d\n", AUD_DspGetMatrixOutMode(AUD_DEC_AUX));
        }
        Printf("Usage: dualmono [decoder id] [dual mono mode] or dualmono q\n");
        Printf("[decoder id]     0:FIRST                     1:SECOND \n");
        Printf("[dual mono mode] 0:STEREO (Disable)      1:L MONO \n");
        Printf("                 2:R MONO                3:MIX MONO\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Mode = (UINT8)StrToInt(szArgv[2]);

    if (u1Mode < 5)
    {
        AUD_DspSetMatrixOutMode(u1DecId, (AUD_MATRIX_OUT_T)u1Mode);
    }

    return 1;
};

static INT32 _AudCmdAdFadePanPesUpdate(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1AdUpdateEnable;
    if (i4Argc < 2)
    {
        Printf("Usage: adupdate [ad pan fade pes update enable]\n");
        Printf("[ad pan fade pes update enable]  0: disable, 1: enable\n");
        return -1;
    }

    u1AdUpdateEnable = (UINT8)StrToInt(szArgv[1]);

    if (u1AdUpdateEnable == 0)
    {
        AUD_DspPanFadePesUpdateEn(FALSE);
    }
    else if (u1AdUpdateEnable == 1)
    {
        AUD_DspPanFadePesUpdateEn(TRUE);
    }
    return 1;
}


static INT32 _AudCmdAdFade(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Adfade;
    if (i4Argc < 3)
    {
        Printf("Usage: adfade [decoder id] [ad fade value]\n");
        Printf("[decoder id]     0:MAIN  1:AUX 2:THIRD\n");
        Printf("[adfade]  value 0~255\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Adfade = (UINT8)StrToInt(szArgv[2]);

    AUD_DspAdFadeVolumeSet(u1Adfade);
    AUD_DspAdFadeVolumeUpdate(u1DecId, AUD_CH_ALL);
    return 1;
}

static INT32 _AudCmdAdPanFadeEnable(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1AdFadeEnable;
    if (i4Argc < 2)
    {
        Printf("Usage: adpanfadeen [ad pan fade enable]\n");
        Printf("[ad pan fade enable]  0: disable, 1: enable\n");
        return -1;
    }

    u1AdFadeEnable = (UINT8)StrToInt(szArgv[1]);

    if (u1AdFadeEnable == 0)
    {
        AUD_DspAdPanFadeEnable(FALSE);
    }
    else if (u1AdFadeEnable == 1)
    {
        AUD_DspAdPanFadeEnable(TRUE);
    }
    return 1;
}

static INT32 _AudCmdAdPan(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1AdPan;
    if (i4Argc < 3)
    {
        Printf("Usage: adpan [decoder id] [ad fade value]\n");
        Printf("[decoder id]     0:MAIN 1:AUX 2:THIRD\n");
        Printf("[adpan]  value (0~255)\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1AdPan = (UINT8)StrToInt(szArgv[2]);

    if (u1AdPan & 0x80)
    {
        if (u1AdPan < 0xeb)
        {
            u1AdPan = 0xeb;
        }
        u1AdPan = 0x100 - u1AdPan;
        AUD_DspAdPanVolumeSet(u1DecId, AUD_CH_FRONT_RIGHT, u1AdPan);
        AUD_DspAdPanVolumeSet(u1DecId, AUD_CH_FRONT_LEFT, 0);
        AUD_DspAdPanVolumeUpdate(u1DecId, AUD_CH_FRONT_RIGHT);
        AUD_DspAdPanVolumeUpdate(u1DecId, AUD_CH_FRONT_LEFT);
    }
    else
    {
        if (u1AdPan > 0x15)
        {
            u1AdPan = 0x15;
        }
        AUD_DspAdPanVolumeSet(u1DecId, AUD_CH_FRONT_LEFT, u1AdPan);
        AUD_DspAdPanVolumeSet(u1DecId, AUD_CH_FRONT_RIGHT, 0);
        AUD_DspAdPanVolumeUpdate(u1DecId, AUD_CH_FRONT_RIGHT);
        AUD_DspAdPanVolumeUpdate(u1DecId, AUD_CH_FRONT_LEFT);
    }

    return 1;
}


#if 0
#define AC3_COMP_CUSTOM_0       0 /* AC3 compression mode custom 0*/
#define AC3_COMP_CUSTOM_1       1 /* AC3 compression mode custom 1*/
#define AC3_COMP_LINE           2 /* AC3 compression line mode P.S. default set as line mode*/
#define AC3_COMP_RF             3 /* AC3 compression RF mode*/
#define AC3_COMP_WLINE          4 /* AC3 compression wide range line mode*/
#endif
static INT32 _AudCmdDtvAc3CompMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT16 u2Mode;

    if (i4Argc < 3)
    {
        Printf("  \n");
        Printf("Current mode : FIRST  ==> %d\n",AUD_DspGetAC3CompMode(AUD_DEC_MAIN));
        Printf("               SECOND ==> %d\n",AUD_DspGetAC3CompMode(AUD_DEC_AUX));
        Printf("  \n");
        Printf("Usage: ac3cm [decoder id] [compression mode]\n");
        Printf("[decoder id]       0:FIRST                 1:SECOND \n");
        Printf("[compression mode] 0:custom mode 0         1:custom mode 1\n");
        Printf("                   2:LINE mode             3:RF mode\n");
        return -1;
    }
    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u2Mode = (UINT16)StrToInt(szArgv[2]);

    if(u2Mode<4)
    {
        AUD_DspAC3CompMode(u2Mode, u1DecId);
    }
    return 1;
};
static INT32 _AudCmdDtvAc3DrcRange(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT16 u2Mode;

    if (i4Argc < 3)
    {
        Printf("  \n");
        Printf("Current level : FIRST  ==> (%d) percentage\n",AUD_DspGetAC3DRCRange100(AUD_DEC_MAIN));
        Printf("                SECOND ==> (%d) percentage\n",AUD_DspGetAC3DRCRange100(AUD_DEC_AUX));
        Printf("  \n");
        Printf("Usage: ac3dr [decoder id] [percentage]\n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[range]          High cut and Low boost value     \n");
        Printf("                 0:0.0                   100:1.0    \n");
        return -1;
    }
    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u2Mode = (UINT16)StrToInt(szArgv[2]);

    if(u2Mode<=100)
    {
        AUD_DspAC3DRCRange100(u2Mode, u1DecId);
    }
    return 1;
}
static INT32 _AudCmdDtvAc3Query(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    AUD_DEC_INFO_T  rAudInfo;
    if(i4Argc==1)
    {
        u1DecId = AUD_DEC_MAIN;
    }
    else if(i4Argc==2)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
     }
    else
    {
        Printf("\n");
        Printf("Usage: q [decoder id]\n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
	      return -1;
    }
    Printf("\n");
    if (!AUD_IsDecoderPlay(u1DecId))
    {
        Printf("There is no audio decoder playing \n");
        Printf("Usage: q [decoder id]\n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        return -1 ;
    }

    AUD_GetDtvAudInfo(u1DecId, &rAudInfo);

    if (rAudInfo.e_aud_fmt != AUD_FMT_AC3)
    {
        Printf("There is no AC3/E-AC3 audio decoder playing in dec%d\n", u1DecId);
        Printf("Usage: q [decoder id]\n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        return -1;
    }

    Printf(" Current audio format:  %s  \n",_aszAudFormat[rAudInfo.e_aud_fmt]);
    if(rAudInfo.e_aud_fmt == AUD_FMT_AC3)
    {
        Printf(" Extra info. of AC3: \n ");
	 Printf("  >>>Current audio decode type: ");
        switch(rAudInfo.e_dec_type)
        {
        case AUD_DECODE_TYPE_AC3:
	     Printf("AC3 \n");
	     break;
	 case AUD_DECODE_TYPE_EAC3:
	     Printf("E-AC3 \n");
	     break;
        default:
	     Printf("Unknown \n");
	     break;
        }
        Printf("   >>>Current stream");
        switch (rAudInfo.e_aud_type)
        {
        case AUD_TYPE_DUAL_MONO: // 1+1
            Printf("(1+1) contains no LFE \n");
            break;
        case AUD_TYPE_MONO: // 1/0
            Printf("(1/0) contains no LFE \n");
            break;
        case AUD_TYPE_STEREO: // 2/0
            Printf("(2/0) contains no LFE \n");
            break;
        case AUD_TYPE_3_0: // 3/0       
            Printf("(3/0) contains no LFE \n");
            break;
        case AUD_TYPE_SURROUND_2CH: // 2/1
            Printf("(2/1) contains no LFE \n");
            break;
        case AUD_TYPE_SURROUND: // 3/1
            Printf("(3/1) contains no LFE \n");
            break;
        case AUD_TYPE_4_0: // 2/2
            Printf("(2/2) contains no LFE \n");
            break;
        case AUD_TYPE_5_0: // 3/2
            Printf("(3/2) contains no LFE \n");
            break;
        case AUD_TYPE_MONO_LFE: // 1/0.L
            Printf("(1/0.L) contains LFE \n");
            break;
        case AUD_TYPE_STEREO_LFE: // 2/0.L
            Printf("(2/0.L) contains LFE \n");
            break;
        case AUD_TYPE_3_1: // 3/0.L
            Printf("(3/0.L) contains LFE \n");
            break;
        case AUD_TYPE_SURROUND_2CH_LFE: // 2/1.L
            Printf("(2/1.L) contains LFE \n");
            break;
        case AUD_TYPE_SURROUND_LFE: // 3/1.L
            Printf("(3/1.L) contains LFE \n");
            break;
        case AUD_TYPE_4_1: // 2/2.L
            Printf("(2/2.L) contains LFE \n");
            break;
        case AUD_TYPE_5_1: // 3/2.L
            Printf("(3/2.L) contains LFE \n");
            break;
        default: //
            Printf(" contains no LFE \n");
            break;
        }
    }
    Printf(" Sampling rate:         %d Hz  \n",rAudInfo.ui4_sample_rate);
    Printf(" Data rate:             %d bps\n",rAudInfo.ui4_data_rate);
    Printf(" Audio decoding mode:   %s\n",_aszAudACMode[rAudInfo.e_aud_type]);

    return 1 ;
}
#ifdef CC_AUD_USE_NVM
static INT32 _AudCmdNvmRead(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 auNvmBuffer[0xFF];
    UINT8 u1Idx;

    if (i4Argc != 1)
    {
        Printf("Usage: read \n");
        return -1;
    }

    if (AUD_NVM_Operation(AUD_NVM_READ, auNvmBuffer, 0))
    {
        Printf("NVM Read Fail!\n");
        return -1;
    }

	Printf("Hex format : \n");
    for (u1Idx=0; u1Idx<0xFF; u1Idx++)
    {
        if (u1Idx%16 == 0)
        {
            Printf("\n[%02x] ", u1Idx);
        }
        Printf("%02x ", auNvmBuffer[u1Idx]);
    }
    Printf("\n");
    return 0;
}

static INT32 _AudCmdNvmWrite(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1NvmData;
    UINT16 u2Offset;

    if (i4Argc < 3)
    {
        Printf("Usage: write [offset] [Data] \n");
        return -1;
    }

    u2Offset = StrToInt(szArgv[1]);
    u1NvmData = (UINT8)StrToInt(szArgv[2]);

    if (AUD_NVM_Operation(AUD_NVM_WRITE, &u1NvmData, u2Offset))
    {
        Printf("NVM Write Fail!\n");
        return -1;
    }

    Printf("NVM Write Success.\n");
    return 0;
}

static INT32 _AudCmdNvmInit(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1NvmData;

    if (i4Argc != 1)
    {
        Printf("Usage: init \n");
        return -1;
    }

    if (AUD_NVM_Operation(AUD_NVM_INIT, &u1NvmData, 0))
    {
        Printf("NVM Init Fail!\n");
        return -1;
    }

    Printf("NVM Init Success.\n");
    return 0;
}

static INT32 _AudCmdProfileRead(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 pProfileStart,pProfileEnd;
    UINT8 u1Profile;
    UINT8 i;

    if (i4Argc != 1)
    {
        Printf("Usage: pfr \n");
        return -1;
    }

    if (AUD_NVM_Get_Profile(&pProfileStart, &pProfileEnd, &u1Profile))
    {
        printf("Get Profile address Fail!!");
        return -1;
    }
    printf("The Profile in EEPROM is %d\n", u1Profile);
    printf("Profile Start From 0x%x, End in 0x%x\n", pProfileStart, pProfileEnd);

    for(i=0; i<128; i++)    //FIXME!! nvm
    {
        if ((i % 16) == 0)
        {
            printf("\n");
        }
            printf("%02x ", *(UINT8*)(pProfileStart + i));
    }
    Printf("\n");
    return 0;
}
#endif //#ifdef CC_AUD_USE_NVM

static INT32 _AdspDSPROMID(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i;
    UINT32 u1IDbuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    UNUSED(i4Argc);
    UNUSED(szArgv);

    AUD_Read32ByteID ((UINT8 *) &u1IDbuf[0], 32);
    printf ("DSP 32 bytes ROM ID\n");
    for (i = 0; i < 8; i ++)
    {
        printf ("%08X ", u1IDbuf[i]);
    }
    printf ("\n");

    return 1;
}

static INT32 _AdspQChannelDelay(INT32 i4Argc, const CHAR ** szArgv)
{
//QUERY_DELAY
    UINT8 u1ChannelIdx;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    printf("Channel :\n(0)-C, \n(1)-L,  (2)-R, \n(3)-Ls, (4)-Rs, \n(5)-BypassL, (6)-ByPassR, \n(7)-Sw, \n(8)-DmxL,(9)-DmxR \n");
    for (u1ChannelIdx = 0; u1ChannelIdx < 10; u1ChannelIdx++)
    {
        printf(" Ch Id (%d) Delay is %d ms \n", u1ChannelIdx, AUD_DspQueryChannelDelay(u1ChannelIdx));
    }

	printf("Current Main path video delay is %d ms \n ",  bDrvVideoGetFrameDelay(0));

    return 0;
}


//================================================================================
#ifdef SUPPORT_FLASH_AQ

#if MULTIPLE_FLASH_AQ
extern UINT32 GetCurrentPanelIndex(void);
extern UINT16 vApiFlashPqGetQtyId(UINT8 bIndex);
#endif

static INT32 _AudCmdFlashAQReadEx(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2Offset;
    UINT8 u1Partition = 0;
    UINT8 u1Length = 1;
    UINT8 u1Data = 0;
    UINT8 i;
    UINT8 u1ID[FLASH_AQ_HDR_OFFSET] = {0,0,0,0,0,0,0,0,0,0,0,0};

    UNUSED(i4Argc);
    UNUSED(szArgv);

    u1Partition = (UINT8)DRVCUST_OptGet(eAudioFlashAQPartition);
    Printf("eAudioFlashAQPartition: %d\n", u1Partition);
    AUD_FlashAQReadEx(u1Partition, 0, u1ID, FLASH_AQ_HDR_OFFSET);
    Printf("  ID: %c%c%c%c\n",u1ID[0],u1ID[1],u1ID[2],u1ID[3]);
    Printf("  Version: %c%c%c%c%c%c%c%c\n",u1ID[4],u1ID[5],u1ID[6],u1ID[7],u1ID[8],u1ID[9],u1ID[10],u1ID[11]);
    #if MULTIPLE_FLASH_AQ
    Printf("  AQ(%d) offset: 0x%04x\n",
                vApiFlashPqGetQtyId(GetCurrentPanelIndex()),
                FLASH_AQ_HDR_OFFSET + vApiFlashPqGetQtyId(GetCurrentPanelIndex()) * (sizeof(rAudFlashAQ) - FLASH_AQ_HDR_OFFSET));
    #endif

    if ((i4Argc != 3)&&(i4Argc != 4))
    {
        Printf("nvm.rx [u1Partition] [u2Offset] [u1length]\n");
        return -1;
    }

    u1Partition = StrToInt(szArgv[1]);
    u2Offset = StrToInt(szArgv[2]);
    if (i4Argc == 4)
    {
        u1Length = (UINT8)StrToInt(szArgv[3]);
    }

    for (i=0; i<u1Length; i++)
    {
      AUD_FlashAQReadEx(u1Partition, u2Offset+i, &u1Data, 1);
      Printf("0x%x ", u1Data);
    }

    Printf("\n");
    return 0;
}
#endif

//================================================================================
#ifdef CC_AUD_USE_FLASH_AQ
static INT32 _AudCmdFlashAQWrite(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2Offset;
    UINT8  u1Data;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (i4Argc != 3)
    {
        Printf("nw [u2Offset][u1Data]\n");
        return -1;
    }

    u2Offset = StrToInt(szArgv[1]);
    u1Data = (UINT8)StrToInt(szArgv[2]);

    AUD_FlashAQWrite(u2Offset, &u1Data, 1);

    Printf("Write Data 0x%x to %d\n", u1Data,u2Offset);

    return 0;
}

static INT32 _AudCmdFlashAQRead(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2Offset;
    UINT8  u1Length = 1;
    UINT8 u1Data;
    UINT8 i;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    if ((i4Argc != 2)&&(i4Argc != 3))
    {
        Printf("nr [u2Offset][u1length]\n");
        return -1;
    }

    u2Offset = StrToInt(szArgv[1]);
    if (i4Argc == 3)
    {
        u1Length = (UINT8)StrToInt(szArgv[2]);
    }

    for (i=0; i<u1Length; i++)
    {
        AUD_FlashAQRead(u2Offset+i, &u1Data, 1);
        Printf("0x%x ", u1Data);
    }

    Printf("\n");
    return 0;
}

static INT32 _AudCmdFlashAQGetVersion(INT32 i4Argc, const CHAR ** szArgv)
{
    CHAR pTestVer[FLASH_AQ_VER_LENGTH+1];

    UNUSED(i4Argc);
    UNUSED(szArgv);

    pTestVer[FLASH_AQ_VER_LENGTH] = '\0';
    AUD_FlashAQRead(FLASH_AQ_VER_OFFSET, (UINT8*)pTestVer, FLASH_AQ_VER_LENGTH);

    Printf(" Your AQ Version on Flash is %s\n", pTestVer);
    return 0;
}

static INT32 _AudCmdFlashAQComVersion(INT32 i4Argc, const CHAR ** szArgv)
{
    CHAR pUppVersion[FLASH_AQ_VER_LENGTH+1];
    CHAR pTestVer[FLASH_AQ_VER_LENGTH+1];

    if ((i4Argc != 2)&&(i4Argc != 3))
    {
        Printf("vcmp [UppVersion] ([TestVersion])\n");
        Printf("     0.00.000 \n");
        return -1;
    }

    x_strcpy(pUppVersion,szArgv[1]);
    Printf("Your version is %s\n", pUppVersion);

    if (i4Argc == 2)
    {
        pTestVer[FLASH_AQ_VER_LENGTH] = '\0';
        AUD_FlashAQRead(FLASH_AQ_VER_OFFSET, (UINT8*)pTestVer, FLASH_AQ_VER_LENGTH);
    }
    else
    {
        x_strcpy(pTestVer,szArgv[2]);
    }

    if (AUD_FlashAQCompVersion(pTestVer,pUppVersion))
    {
        Printf("%s is >= %s\n", pUppVersion, pTestVer);
    }
    else
    {
        Printf("%s is < %s\n", pUppVersion, pTestVer);
    }

    return 0;
}

static INT32 _AudCmdFlashAQSetDefault(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    LOG(3, "Set default Data to Flash!\n");
    AUD_FlashAQSetDefault();

    return 0;
}

static INT32 _AudCmdFlashAQListAll(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 i;
    UINT32 j, u4Offset;
    UINT8 u1X,u1Y,u1Bytes;
    AUD_AQ_DSCRPT_T *eAudAQDscrpt;
    UINT8 u1Data;
    UINT32 u4Temp;
    UINT8* pBuffer;
    UINT32 u4TotalSize = 0;

    UNUSED(i4Argc);
    UNUSED(szArgv);
    UNUSED(u4Temp);
    u4Offset = 0;

    AUD_FlashAQGetDscrpt(&eAudAQDscrpt);

    pBuffer = (UINT8 *)BSP_AllocAlignedDmaMemory(AUD_FLASH_AQ_BUF_SIZE, 32);
    if (pBuffer == NULL)
    {
        Printf("Memory Allocation fail !\n");
    }

    Printf("The Data on flash are : \n");
    for (i=0; i<13; i++) //FIXME !! Add a API to query size, don't hard code.
    {
        u1X = eAudAQDscrpt[i].u1X;
        u1Y = eAudAQDscrpt[i].u1Y;
        u1Bytes = eAudAQDscrpt[i].u1Bytes;
        u4TotalSize += (u1X * u1Y * u1Bytes);
    }

    AUD_FlashAQRead(FLASH_AQ_VER_OFFSET, pBuffer, u4TotalSize);

    for (i=0; i<13; i++) //FIXME !! Add a API to query size, don't hard code.
    {
        Printf(" [%s] offset : %d ", eAudAQDscrpt[i].pChar, u4Offset);
        u1X = eAudAQDscrpt[i].u1X;
        u1Y = eAudAQDscrpt[i].u1Y;
        u1Bytes = eAudAQDscrpt[i].u1Bytes;
        for (j = 0; j < (u1X*u1Y*u1Bytes); j++)
        {
            u1Data = pBuffer[u4Offset];
            if ((j%16) == 0)
            {
                Printf("\n");
            }
            Printf("0x%02x ", u1Data);
            u4Offset += 1;
        }
        Printf("\n\n");
    }
#ifndef CC_AUD_NEW_AQ_PARSER
    //External PEQ
    u4Temp = (FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_AMP_FS_NUM * AQ_EXT_PEQ_ITEMS) * 3;
    AUD_FlashAQRead(u4Offset, pBuffer, u4Temp);

    Printf(" [PEQ offset] : %d ", u4Offset);
    for (j = 0; j < u4Temp; j++)
    {
        u1Data = pBuffer[u4Offset];
        if ((j%16) == 0)
        {
            Printf("\n");
        }
        Printf("0x%02x ", u1Data);
        u4Offset += 1;
    }
    Printf("The last Offset : %d\n", u4Offset);
#endif

    VERIFY(BSP_FreeAlignedDmaMemory((UINT8*)pBuffer));
    return 0;
}

static INT32 _AudCmdFlashAQFillDataStruct(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    AUD_FlashAQFillData(TRUE);

    return 0;
}
#endif /* CC_AUD_USE_FLASH_AQ */
//=================================================

#ifdef BURN_IN_AOUT_TEST
static INT32 _AdspAoutReset(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    if ((i4Argc != 2))
    {
        Printf("[usage] raout 1  to reset counter\n");
        return -1;
    }

    AudResetAoutStatus();
    Printf("Reset total aout fail counter to 0\n");
    return 0;
}

static INT32 _AdspAoutQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    Printf("Total Aout Dram Fail Counter is : %d\n", AudGetAoutStatus());
    return 0;
}

static INT32 _AdspAoutSet(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    AudSetAoutTest();
    return 0;
}
#endif

static INT32 _AudCmdAsrcQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1ChMsk;
    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (i4Argc != 2)
    {
        Printf("q [ChMsk]\n");
        Printf("  0x1 : CH01\n");
        Printf("  0x2 : CH23\n");
        Printf("  0x4 : CH45\n");
        Printf("  0x8 : CH67\n");
        Printf(" 0x10 : CH89\n");
        return -1;
    }
    AudShowAsrcStatus();

    u1ChMsk = (UINT8)StrToInt(szArgv[1]);
    AudShowAsrcBufferStatus(u1ChMsk,TRUE);
    return 1;
}

extern void DSP_SetAsrcTrackingMode(BOOL fgEnable);
static INT32 _AudCmdAsrcTrackingMode(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgEnable;

    if (i4Argc != 2)
    {
        Printf("t [on/off])\n");
        Printf(" 1 - on, 0 - off\n");
        return -1;
    }
    fgEnable = (BOOL)StrToInt(szArgv[1]);

    DSP_SetAsrcTrackingMode(fgEnable);
    return 1;
}

#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
static INT32 _AudAoutGain(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 chsel;
	UINT8 u1decid;
	UINT16 gain;
    if (i4Argc < 3)
    {
        Printf("Usage: aoutg  [dec] [channel] [gain]\n");
        Printf("[dec]        0  Main decoder \n");
        Printf("             1  Aux decoder \n");
        Printf("[channel]    0  all channels of decoder\n");
        Printf("             1  L ch      \n");
        Printf("             2  R ch      \n");
        Printf("             3  LS ch      \n");
        Printf("             4  RS ch      \n");
        Printf("             5  C ch  \n");
        Printf("             6  LFE ch \n");
        Printf("             7  ch7 \n");
        Printf("             8  ch8 \n");
        Printf("             9  ch 9  \n");
        Printf("             10 ch 10 \n");
		Printf("[gain]       0x1000   --> 0 db \n");
        Printf("             0x0      --> mute \n");
        return -1;
    }

	u1decid = (UINT8)StrToInt(szArgv[1]);
	chsel   = (UINT8)StrToInt(szArgv[2]);
	gain    = (UINT16)StrToInt(szArgv[3]);

	AudAoutGainCtrl(u1decid, chsel, gain);

    return 1;
}
#endif

static INT32 _AudCmdPll1Set(INT32 i4Argc, const CHAR ** szArgv)
{
    AudPll1Setting();

    return 1;
}

static INT32 _AudCmdPll2Set(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1ClkId;

    if (i4Argc != 2)
    {
        Printf("Usage: pll2 [clk]\n");
        Printf("[clk] 0:294.912MHz, 1:270.9508MHz\n");
        return -1;
    }
    else
    {
        u1ClkId = (UINT8)StrToInt(szArgv[1]);
        if (u1ClkId == 0)
        {
            AudPll2Setting(FS_48K);
        }
        else if (u1ClkId == 1)
        {
            AudPll2Setting(FS_44K);
        }
    }
    return 1;
}

static INT32 _AudCmdPllPowerDown(INT32 i4Argc, const CHAR ** szArgv)
{
   UINT8 APLLID;
   BOOL fgPD;

   if (i4Argc != 3)
   	{
		Printf ("Usage: pllpd [pllid] [fgPd]\n");
		Printf ("pllid: 0 APLL1; 1 APLL2\n");
		Printf ("fgPd:  1 PowerDown; 0 PowerOn\n");
   	}
   else
   	{

		APLLID = (UINT8)StrToInt(szArgv[1]);
		fgPD = (BOOL)StrToInt(szArgv[2]);
		AudPllPowerDown(APLLID,  fgPD);
   	}
   return 1;
}

//CC_AUD_SUPPORT_OPENMAX
#if 0 //def LINUX_TURNKEY_SOLUTION
extern UINT8 u1AudOmxGetBypassChVol(void);
extern void vAudOmxSetBypassChVol(UINT8 u1Enable);

static INT32 _AudCmdOmxBypassChVol(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 2)
    {
        Printf("omx.bc [fgEnable]\n");
        Printf("  0: disable bypass\n");
        Printf("  1: bypass\n");
        Printf("OMX Ch Vol Bypass: %s\n", u1AudOmxGetBypassChVol() ? "On" : "Off");            
    }
    else
    {
        vAudOmxSetBypassChVol((UINT8)StrToInt(szArgv[1]));    
    }
    return 1;
}

static INT32 _AudCmdOmxLPCMSetting(INT32 i4Argc, const CHAR ** szArgv)
{
	AUD_PCM_SETTING_T rPCMInfo;
    AUD_LPCM_INFO_T rLPCMInfo;
    UINT32 u4SampleFreq;

    if (i4Argc != 9)
    {
        Printf("omx.lpcm [u4SampleFreq] [eLPcmType] [ui2_num_ch] [ui2_block_align]\n");
        Printf("         [ui2_bits_per_sample] [b_big_endian] [b_info_not_specific] [b_signed_data]\n");
        Printf("  u4SampleFreq: 48000/44100/32000 ...\n");
        Printf("  eLPcmType: 0: Normal, 1: MS_ADPCM, 2: IMA_ADPCM, 3: A_LAW, 4: U_LAW\n");
    }
    else
    {
        //eSampleFreq
        u4SampleFreq = (UINT32)StrToInt(szArgv[1]);
        switch (u4SampleFreq/1000)
        {
             case 48:
                 rLPCMInfo.eSampleFreq = FS_48K;
                 break;
             case 32:
                 rLPCMInfo.eSampleFreq = FS_32K;
                 break;
             case 44:
                 rLPCMInfo.eSampleFreq = FS_44K;
                 break;
             case 8:
                 rLPCMInfo.eSampleFreq = FS_8K;
                 break;
             case 11:
                 rLPCMInfo.eSampleFreq = FS_11K;
                 break;
             case 12:
                 rLPCMInfo.eSampleFreq = FS_12K;
                 break;
            case 16:
                 rLPCMInfo.eSampleFreq = FS_16K;
                 break;
             case 22:
                 rLPCMInfo.eSampleFreq = FS_22K;
                 break;
             case 24:
                 rLPCMInfo.eSampleFreq = FS_24K;
                 break;
             default:
                 rLPCMInfo.eSampleFreq = FS_48K;
                 break;
        }

        rLPCMInfo.eLPcmType = (LPCM_INPUT_TYPE_T)StrToInt(szArgv[2]);
        rLPCMInfo.ui2_num_ch = (UINT16)StrToInt(szArgv[3]);
        rLPCMInfo.ui2_block_align = (UINT16)StrToInt(szArgv[4]);
        rLPCMInfo.ui2_bits_per_sample = (UINT16)StrToInt(szArgv[5]);
        rLPCMInfo.b_big_endian = (BOOL)StrToInt(szArgv[6]);
        rLPCMInfo.b_info_not_specific = (BOOL)StrToInt(szArgv[7]);
        rLPCMInfo.b_signed_data = (BOOL)StrToInt(szArgv[8]);

        rPCMInfo.eSampleFreq = rLPCMInfo.eSampleFreq;
        if (rLPCMInfo.b_big_endian)
        {
            rPCMInfo.ePcmDataInfo = PCM_16_BITS_BIG_ENDIAN;
        }
        else
        {
            rPCMInfo.ePcmDataInfo = PCM_16_BITS_LITTLE_ENDIAN;
        }

        AUD_PcmSetting(AUD_DEC_MAIN, &rPCMInfo);
        AUD_LPcmSetting(AUD_DEC_MAIN, &rLPCMInfo);
    }
	return 1;
}

static INT32 _AudCmdOmxWMASetting(INT32 i4Argc, const CHAR ** szArgv)
{
    AUD_WMA_CFG_T rWMAInfo = {0};

    if (i4Argc != 7)
    {
        Printf("omx.wma [u4Pktsz] [u2NumCh] [u4Sampersec] [u4Bpersec] [u4Blocksz] [u2Encoderopt]\n");
        Printf("  Using header detection mode\n");

        AUD_DspWMAHeader(AUD_DEC_MAIN, &rWMAInfo);
    }
    else
    {
        rWMAInfo.u2HdrSrc = 1;
        rWMAInfo.u4Pktsz = (UINT32)StrToInt(szArgv[1]);
        rWMAInfo.u2Numch = (UINT16)StrToInt(szArgv[2]);
        rWMAInfo.u4Sampersec = (UINT32)StrToInt(szArgv[3]);
        rWMAInfo.u4Bpersec = (UINT32)StrToInt(szArgv[4]);
        rWMAInfo.u4Blocksz = (UINT32)StrToInt(szArgv[5]);
        rWMAInfo.u2Encoderopt = (UINT16)StrToInt(szArgv[6]);

        AUD_DspWMAHeader(AUD_DEC_MAIN, &rWMAInfo);
    }
	return 1;
}

static INT32 _AudCmdOmxWMAProSetting(INT32 i4Argc, const CHAR ** szArgv)
{
    AUD_WMA_CFG_T rWMAInfo = {0};

    if (i4Argc != 10)
    {
        Printf("omx.wma10 [u2NumCh] [u4Sampersec] [u4Bpersec] [u4Blocksz] [u2Encoderopt]\n");
        Printf("        [u4ChannelMask] [u2BitsPerSample] [u2AdvEncoderopt] [u4AdvEncoderopt2]\n");

        AUD_DspWMAProHeader(AUD_DEC_MAIN,&rWMAInfo);
    }
    else
    {
        rWMAInfo.u2HdrSrc = 1;    
        rWMAInfo.u2Numch = (UINT16)StrToInt(szArgv[1]);
        rWMAInfo.u4Sampersec = (UINT32)StrToInt(szArgv[2]);
        rWMAInfo.u4Bpersec = (UINT32)StrToInt(szArgv[3]);
        rWMAInfo.u4Blocksz = (UINT32)StrToInt(szArgv[4]);
        rWMAInfo.u2Encoderopt = (UINT16)StrToInt(szArgv[5]);

        rWMAInfo.u4ChannelMask = (UINT32)StrToInt(szArgv[6]);
        rWMAInfo.u2BitsPerSample = (UINT16)StrToInt(szArgv[7]);
        rWMAInfo.u2AdvEncoderopt = (UINT16)StrToInt(szArgv[8]);
        rWMAInfo.u4AdvEncoderopt2 = (UINT32)StrToInt(szArgv[9]);


        AUD_DspWMAProHeader(AUD_DEC_MAIN,&rWMAInfo);
    }
	return 1;
}

static INT32 _AudCmdOmxCOOKSetting(INT32 i4Argc, const CHAR ** szArgv)
{
     AUD_COOK_CFG_T rCookCfg = {0};

    if (i4Argc != 8)
    {
        Printf("omx.cook [u2SampPerFrame] [u2ChanNum] [u2SampRate] [u2FrameSzInBytes] [u2RegnNum]\n");
        Printf("         [u2StartRegn] [u2CplQBits]\n");
    }
    else
    {
        rCookCfg.u2SampPerFrame = (UINT16)StrToInt(szArgv[1]);    
        rCookCfg.u2ChanNum = (UINT16)StrToInt(szArgv[2]);
        rCookCfg.u2SampRate = (UINT16)StrToInt(szArgv[3]);
        rCookCfg.u2FrameSzInBytes = (UINT16)StrToInt(szArgv[4]);
        rCookCfg.u2RegnNum = (UINT16)StrToInt(szArgv[5]);
        rCookCfg.u2StartRegn = (UINT16)StrToInt(szArgv[6]);        
        rCookCfg.u2CplQBits = (UINT16)StrToInt(szArgv[7]);
        AUD_DspCOOKParameter(AUD_DEC_MAIN, &rCookCfg);    
    }
	return 1;
}

extern UINT8 u1DtsBigEndian;

static INT32 _AudCmdOmxDTSSetting(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 2)
    {
        Printf("omx.dts [endian]\n");
        Printf("  0: little\n");
        Printf("  1: Big\n");
        Printf("  Current Setting: %d\n", u1DtsBigEndian);
    }
    else
    {
        u1DtsBigEndian = (UINT8)StrToInt(szArgv[1]);
    }
	return 1;
}

extern UINT32 u4Latency;

static INT32 _AudCmdOmxDelaySetting(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 2)
    {
        Printf("omx.delay [enable_log] [u4Latency]\n");
        Printf("  ret_path - 0: free_run, 1: RP\n");
        Printf("  enable_log - 0: disable 1: Enable\n");
        Printf("Current Setting: u4Latency(%x)\n", u4Latency);
    }
    else
    {
        u4Latency = (UINT32)StrToInt(szArgv[3]);
    }
	return 1;
}

extern UINT8 bOmxSettingBypass;
static INT32 _AudCmdOmxSettingBypass(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 2)
    {
        Printf("omx.bypass [bypass]\n");
        Printf("  current setting: %d\n", bOmxSettingBypass);
    }
    else
    {        
        bOmxSettingBypass = (UINT8)StrToInt(szArgv[1]);
    }
	return 1;
}

extern UINT32 u4AOmxDbgMsk;
static INT32 _AudCmdOmxSetDbgMsk(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 2)
    {
        Printf("omx.dm [Debug Mask]\n");
        Printf("  current setting: %08x\n", u4AOmxDbgMsk);
        Printf("  bit 0: (Reserved)\n");
        Printf("  bit 1: [LOG] Consume buffer\n");
        Printf("  bit 2: [LOG] Queue empty buffer\n");
        Printf("  bit 3: [CTL] Empty buffer done wait DSP consume\n");
        Printf("  bit 4: [LOG] PTS output log\n");
        Printf("  bit 5: [CTL] PTS interpolation test mode (under develop)\n");
        Printf("  bit 6: [LOG] Queue fill buffer log\n");
        Printf("  bit 7: [CTL] Disable pause/resume mechanism\n");
        Printf("  bit 8: [LOG] PTS input log\n");
    }
    else
    {        
        u4AOmxDbgMsk = (UINT32)StrToInt(szArgv[1]);
    }
    return 1;
}

extern void vAudOmxQuery(void);
static INT32 _AudCmdOmxQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    vAudOmxQuery();
    return 1;
}

extern INT32 i4AOmxPtsOffset;
static INT32 _AudCmdOmxPtsOffset(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 2)
    {
        Printf("omx.pts [pts offset\n");
        Printf("  Current Setting: %d\n", i4AOmxPtsOffset);
    }
    else
    {
        i4AOmxPtsOffset = i4StrToInt(szArgv[1]);
    }
    return 1;
}
#endif

static void _AudCmdAvSyncCurrentStatus(UINT16 u2Lvl)
{
    printf("\n");
    if ((u2Lvl & AUD_DBG_SYNC_SEND_PTS) == AUD_DBG_SYNC_SEND_PTS)
    {
        printf("DBG : Send PTS is ON\n");
    }
    else
    {
        printf("DBG : Send PTS is OFF\n");
    }

    if ((u2Lvl & AUD_DBG_SYNC_STC_DIFF) == AUD_DBG_SYNC_STC_DIFF)
    {
        printf("DBG : STC_DIFF is ON\n");
    }
    else
    {
        printf("DBG : STC_DIFF is OFF\n");
    }

    if ((u2Lvl & AUD_DBG_SYNC_PTS_FRESH) == AUD_DBG_SYNC_PTS_FRESH)
    {
        printf("DBG : PTS_REFRESH is ON\n");
    }
    else
    {
        printf("DBG : PTS_REFRESH is OFF\n");
    }
    printf("\n");
}
static INT32 _AudCmdAvSyncDbgLvl(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2CurrLvl = 0;
    UINT16 u2Item = 0;
    BOOL fgOnOff = 0;

    u2CurrLvl = AUD_GetSyncDbgLvl();

    if (i4Argc < 3)
    {
        printf("Usage : dbg [item] [0/1] \n");
        printf("   0: Send PTS \n");
        printf("   1: Stc Diff \n");
        printf("   2: PTS refresh \n");
        printf("  15: ALL \n");
        _AudCmdAvSyncCurrentStatus(u2CurrLvl);
        return -1;
    }

    u2Item = (UINT16)StrToInt(szArgv[1]);
    fgOnOff = (UINT16)StrToInt(szArgv[2]);
    if (u2Item == 15)   // ALL On or Off
    {
        if (fgOnOff)
        {
            u2CurrLvl = (UINT16)(AUD_DBG_SYNC_SEND_PTS|AUD_DBG_SYNC_STC_DIFF|AUD_DBG_SYNC_PTS_FRESH);
        }
        else
        {
            u2CurrLvl = (UINT16)0;
        }
    }
    else
    {
        if (fgOnOff)
        {
            u2CurrLvl |= (1<<u2Item);
        }
        else
        {
            u2CurrLvl &= (~(1<<u2Item));
        }
    }
    AUD_SetSyncDbgLvl(u2CurrLvl);
    _AudCmdAvSyncCurrentStatus(u2CurrLvl);

    return 0;
}

static INT32 _AudCmdAvSyncSetStcID(INT32 i4Argc, const CHAR ** szArgv) // 2_STC_Task
{
    UINT8 u1DecId;
    UINT16 u2StcId;

    AUD_DspGetStcId(AUD_DEC_MAIN, &u2StcId);
    printf("StcID of Dec Main is :%d \n", u2StcId);
    AUD_DspGetStcId(AUD_DEC_AUX, &u2StcId);
    printf("StcID of Dec Aux is :%d \n", u2StcId);
#ifdef CC_MT5391_AUD_3_DECODER
    AUD_DspGetStcId(AUD_DEC_THIRD, &u2StcId);
    printf("StcID of Dec Third is :%d \n\n", u2StcId);
#endif

    if (i4Argc < 3)
    {
        printf("Usage : sid [DecID] [StcID] \n");
        printf("   DecID: 0-Mian  1-Aux  2-Third \n");
        printf("   StcID: 0-Audio  1-Demux \n");        
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u2StcId = (UINT16)StrToInt(szArgv[2]);
    AUD_DspSetStcId(u1DecId, u2StcId);
    printf ("set Dec(%d) StcID to %d\n", u1DecId, u2StcId);

    return 0;
}

#if 0 // 2_STC_Task_DEBUG_only
extern void _DMX_FIXME_ReadSTC(UINT8 u1StcId, UINT32* u4StcBase);
static INT32 _AudCmdAvSyncReadStc(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Stc;

    _DMX_FIXME_ReadSTC(0, &u4Stc);
    printf("AUDIO : 0x%x\n", u4Stc);
    _DMX_FIXME_ReadSTC(1, &u4Stc);    
    printf("DMX STC1 : 0x%x\n", u4Stc);
    _DMX_FIXME_ReadSTC(2, &u4Stc);    
    printf("DMX STC2 : 0x%x\n", u4Stc);
}
#endif

#if 1 //For Debug only 
#define AUD_READ32(offset)                  IO_READ32(AUDIO_BASE, (offset))
INT32 _AudCmdDebug1(INT32 i4Argc, const CHAR **szArgv) //parson debug
{
    UINT16 i = 0;
    UINT16 u2Idx = 0;
    UINT16 u2Length = 0;
    UINT16 u4Addr = 0;
    UINT8 u1Data = 0;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    // ======== 1. Print PC for 20 Times.=========
    Printf ("\n This is 2011/8/5 21:21 Test image \n");
    
    Printf ("PC\n");
    for (i=0; i<60; i++)
    {
        //AUD_DspProbePause();
        Printf ("0x%x\n", u4AUD_DspProbeRead(0xa2));
        //AUD_DspProbeGo();
        x_thread_delay(10);
    }
    
    // ======== 2. SRAM 0, 0xA0, A1*7, A4, A7, 0x800, 0x980, 0x3680 ==========
    u4Addr = 0x3E8;
    Printf ("\n REG \n");
    Printf("0x3E8 is 0x%x\n",AUD_READ32(0x03E8));
    
    AUD_DspProbePause();
    u2Length = 12;
    u4Addr = 0x0;
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {
        if ((u2Idx % 8) == 0)
        {
            Printf("\nAddress: 0x%04x \n",u4Addr+u2Idx);
        }
        Printf("0x%06x ",u4AUD_DspProbeRead(u4Addr+u2Idx));           
    }
    Printf("\n");
    
    u2Length = 1;
    u4Addr = 0xA0;
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {
        if ((u2Idx % 8) == 0)
        {
            Printf("\nAddress: 0x%04x \n",u4Addr+u2Idx);
        }
        Printf("0x%06x ",u4AUD_DspProbeRead(u4Addr+u2Idx));           
    }
    Printf("\n");
    
    u2Length = 7;
    u4Addr = 0xA1;
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {
        if ((u2Idx % 8) == 0)
        {
            Printf("\nAddress: 0x%04x \n",u4Addr+u2Idx);
        }
        Printf("0x%06x ",u4AUD_DspProbeRead(u4Addr+u2Idx));           
    }
    Printf("\n");
    
    AUD_DspProbeGo();    
    x_thread_delay(10);
    u2Length = 1;
    u4Addr = 0xA4;
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {
        if ((u2Idx % 8) == 0)
        {
            Printf("\nAddress: 0x%04x \n",u4Addr+u2Idx);
        }
        Printf("0x%06x ",u4AUD_DspProbeRead(u4Addr+u2Idx));           
    }
    Printf("\n");
    AUD_DspProbePause();    

    u2Length = 1;
    u4Addr = 0xA7;
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {
        if ((u2Idx % 8) == 0)
        {
            Printf("\nAddress: 0x%04x \n",u4Addr+u2Idx);
        }
        Printf("0x%06x ",u4AUD_DspProbeRead(u4Addr+u2Idx));           
    }
    Printf("\n");    
    
    u2Length = 5;
    u4Addr = 0x51;
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {
        if ((u2Idx % 8) == 0)
        {
            Printf("\nAddress: 0x%04x \n",u4Addr+u2Idx);
        }
        Printf("0x%06x ",u4AUD_DspProbeRead(u4Addr+u2Idx));           
    }
    Printf("\n");    
    u2Length = 0x100;
    Printf ("0x800 \n");
    u4Addr = 0x800;
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {
        if ((u2Idx % 8) == 0)
        {
            Printf("\nAddress: 0x%04x \n",u4Addr+u2Idx);
        }
        Printf("0x%06x ",u4AUD_DspProbeRead(u4Addr+u2Idx));           
    }
    Printf("\n");
    Printf ("0x900 \n");
    u4Addr = 0x900;
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {
        if ((u2Idx % 8) == 0)
        {
            Printf("\nAddress: 0x%04x \n",u4Addr+u2Idx);
        }
        Printf("0x%06x ",u4AUD_DspProbeRead(u4Addr+u2Idx));           
    }
    Printf("\n");
    Printf ("0x3680 \n");    
    u4Addr = 0x3680;
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {
        if ((u2Idx % 8) == 0)
        {
            Printf("\nAddress: 0x%04x \n",u4Addr+u2Idx);
        }
        Printf("0x%06x ",u4AUD_DspProbeRead(u4Addr+u2Idx));           
    }
    Printf("\n");

    Printf("\n C: %04x S: %04x d: %08x d: %08x\n",
            u4AUD_DspProbeRead(0xa0),u4AUD_DspProbeRead(0xa2),// PC, MCS
            u4AUD_DspProbeRead(0xa8),u4AUD_DspProbeRead(0xa9)// ins_d,ld_d
    );   
    
    AUD_DspProbeGo();
    x_thread_delay(20);

    // ============== 3. Common Dram =======================
    Printf ("\n Common Dram \n");    
    DSP_CommDramDump(0x0, 1024);

    // ========== 4. Common Code. =================================
    Printf("\n");    
    Printf ("\n Common Program \n");        
    u2Length = 1024;
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {    
        if ((u2Idx % 4)==0)
        {
            Printf("\n");
        }
        u1Data = (*((UINT8*)(VIRTUAL((_u4DspICacheAddr[TYPE_COMM_ICACHE]<<8) + u2Idx))));
        Printf("0x%02x ", u1Data);
    }

    // ============== 5. Decoder1 Code. ============================
    Printf("\n");      
    Printf ("\n DEC1 \n");            
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {    
        if ((u2Idx % 4)==0)
        {
            Printf("\n");
        }    
        u1Data = (*((UINT8*)(VIRTUAL((_u4DspICacheAddr[TYPE_DEC1_ICACHE]<<8) + u2Idx))));
        Printf("0x%02x ", u1Data);
    }

    // ============= 6. Decoder2 Code =======================================
    Printf("\n");    
    Printf ("\n DEC2 \n");                
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {    
        if ((u2Idx % 4)==0)
        {
            Printf("\n");
        }    
        u1Data = (*((UINT8*)(VIRTUAL((_u4DspICacheAddr[TYPE_DEC2_ICACHE]<<8) + u2Idx))));
        Printf("0x%02x ", u1Data);
    }

    // ============= 7. Decoder3 Code =======================================
    Printf("\n");    
    Printf ("\n DEC3 \n");                
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {    
        if ((u2Idx % 4)==0)
        {
            Printf("\n");
        }    
        u1Data = (*((UINT8*)(VIRTUAL((_u4DspICacheAddr[TYPE_DEC3_ICACHE]<<8) + u2Idx))));
        Printf("0x%02x ", u1Data);
    }

    return 0;
}
#endif //if 1 for debug only


static INT32 _AdspGetCap(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
    AudShowDspCapability();    
    return 1;
}


/******************************************************************************
* Variable      : cli "aud" set command table
******************************************************************************/

static CLI_EXEC_T _arAudClipCmdTbl[] =
{
    {"play",    "p",    _AudCmdClipPlay,    NULL,   "play aud clip",    CLI_GUEST},
    {"stop",    "s",    _AudCmdClipStop,    NULL,   "stop aud clip",    CLI_GUEST},
    {"volume",  "v",    _AudCmdClipVolume,  NULL,   "set clip volume",    CLI_GUEST},

    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudTestCmdTbl[] =
{
    {"tone",        "t",    _AudCmdPlayTone, NULL,      "play tone", CLI_GUEST},
    {"stoptone",    "stop", _AudCmdStopMemStr, NULL,    "stop tone", CLI_GUEST},
    {"soundeffect", "s",    _AudCmdSoundEffect, NULL,       "sound effect on/off", CLI_GUEST},
    {"bypass",      "b",    _AudCmdBypassPostProcessing, NULL,      "bypass post processing", CLI_GUEST},
#ifdef __MODEL_slt__ // by hotkey 0729 for compile
    {"SltSorting",  "slt",  _AudCmdSltSorting, NULL,    "slt sorting", CLI_SUPERVISOR},
    {"DtvSlt",  "dtvslt",   _AudCmdDtvSltSorting, NULL, "Dtv slt sorting", CLI_SUPERVISOR},
    {"AtvSlt",  "atvslt",   _AudCmdAtvSltSorting, NULL, "Atv slt sorting", CLI_SUPERVISOR},
    {"IntAdSlt",  "iadslt",   _AudCmdIntAdSltSorting, NULL, "Internal AD slt sorting", CLI_SUPERVISOR},
    {"SltFlashing", "flash",    _AudCmdSltFlashing, NULL,   "burning slt pattern to flash", CLI_SUPERVISOR},
    {"SltROM", "romslt",    _AudCmdROMSltSorting, NULL,   "ROM test", CLI_SUPERVISOR},
#endif
    {"dd_banner",   "banner",	_AudCmdDDbanner, NULL,	"DD banner turn on for DD Test", CLI_GUEST },
    {"memalloc",    "mal",	_AudCmdMemAlloc, NULL,	"Allocate memory for test", CLI_GUEST },
#ifdef CC_AUD_SKYPE_SUPPORT
    {"skpringmix",  "skprmix",    _AudSkypeMixRingSnd, NULL,  "skype ring sound mix", CLI_GUEST},
    {"skptxmix",  "skptxmix",    _AudSkypeAuxMix2Main, NULL,  "skype local sound mix", CLI_GUEST},
#endif
#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
    {"AoutGain", "aoutg", _AudAoutGain, NULL, "Aout gain control" , CLI_GUEST},
#endif
#ifdef CC_AUD_PCM_LINE_IN_LPF
    {"pcmlineinlpf",  "pcmlpf",    _AudDSPSetPCMLineInLPF, NULL,  "PCM Line-in LPF", CLI_GUEST},
#endif
#ifdef CC_AUD_HDMI_SPDIF_CFG
    {"hdmispdif",   "hdmispdif", _AudHDMICfgSPDIFCLI, NULL, "HDMI Configure SPDIF Test", CLI_SUPERVISOR},
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudSpdifCmdTbl[] =
{
    {"init",        "i",    _SpdifCmdInit, NULL,        "Spdif-in init", CLI_SUPERVISOR},
    {"reset",       NULL,   _SpdifCmdReset, NULL,       "Spdif-in reset", CLI_SUPERVISOR},
    {"enable",      "e",    _SpdifCmdEnable, NULL,      "Spdif-in enalbe", CLI_SUPERVISOR},
    {"disable",     "d",    _SpdifCmdDisable, NULL,     "Spdif-in diable", CLI_SUPERVISOR},
    {"checklock",   "c",    _SpdifCmdCheckLock, NULL,   "Spdif-in check locked?", CLI_SUPERVISOR},
    {"selectch",    "s",    _SpdifCmdSelectCh, NULL,    "Spdif-in select channel", CLI_SUPERVISOR},
    {"readrg",      "r",    _SpdifCmdReadReg, NULL,     "Spdif-in read registers ex. readrg", CLI_SUPERVISOR},
    {"writerg",     "w",    _SpdifCmdWriteReg, NULL,    "Spdif-in write registers ex. writerg [addr] [value]", CLI_SUPERVISOR},
    {"iec_set",     "iecs", _SpdifIecSet, NULL,        "IEC setting [decoder id] [flag] [enable]", CLI_GUEST},
    {"iec_ch",      "iecc", _SpdifIecChannelSet, NULL, "IEC Channel setting [decoder id] [channel]", CLI_GUEST},
    {"iec_bypass",  "iecb", _SpdifAlwaysOutHdmi, NULL, "IEC output HDMI always [enable]", CLI_GUEST},
    {"iec_query",   "iecq", _SpdifIecQuery, NULL, "IEC setting query", CLI_GUEST},
    {"iecraw_delay_flag",   "flag", _SpdifSetRawDelayFlag, NULL, "IEC RAW delay sync with channel delay control", CLI_GUEST},
    {"iecraw_delay",   "delay", _SpdifSetRawDelay, NULL, "IEC RAW delay", CLI_GUEST},
    {"spdif in bypass",   "inbypass", _SpdifBypass, NULL, "Spdif input bypass to spdif output PIN select", CLI_GUEST},
    {"info",        "info", _SpdifGetInfo, NULL,        "SPDIF get setup", CLI_GUEST},
    {"resetreg",        "resetreg",  _SpdifRegTypeReset, NULL,        "SPDIF reset registers", CLI_GUEST},
    {"copy",        "cpy",  _SpdifCopyrightSet, NULL,        "SPDIF copyright setup", CLI_GUEST},
    {"code",        "code", _SpdifCategoryCodeSet, NULL,        "SPDIF category code setup", CLI_GUEST},
    {"word",        "word", _SpdifWordLengthSet, NULL,        "SPDIF word length setup", CLI_GUEST},
    {"setInfo",     "set",  _SpdifSetInfo, NULL,        "SPDIF information setup", CLI_GUEST},
    {"setProtect",     "protect",  _SpdifSetProtect, NULL,        "SPDIF copy protect setup", CLI_GUEST},
    {"update_mode",     "mode",  _SpdifUpdateModeSet, NULL,        "SPDIF set update mode", CLI_GUEST},
    {"channelstatus",     "cs",  _SpdifGetChannelStatus, NULL,        "SPDIF get channel status", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudAdacCmdTbl[] =
{
    {"query",      "q",    _AdacCmdQuery, NULL,   "Query speaker enable state", CLI_GUEST},
    {"format",      "f",    _AdacCmdDataFormat, NULL,   "Data format ex. format 0 1", CLI_GUEST},
    {"datainvert",  "d",    _AdacCmdDataInvert, NULL,   "Data invert ex.  datainvert 0 1", CLI_GUEST},
    {"samplefreq",  "sf",   _AdacCmdSampleFreq, NULL,   "DAC Sample Frequence ex.sf 0 1", CLI_GUEST},
    {"SelectFs",    "s",    _AdacCmdSelectFs, NULL,     "Select Fs ex. SelectFs 0x01", CLI_SUPERVISOR},
    {"drvcurrent",  "drv",  _AdacCmdDrvCur, NULL,       "Select MCLK BCLK driving current ex. drv 0 8 8", CLI_GUEST},
    {"volume",      "v",    _AdacCmdWM8776VolCtl, NULL, "WM8776 headphone volume control", CLI_GUEST},
    {"CodecRead",   "cr",   _AdacCmdCodecRead, NULL,    "WM8776/MT8291 register read", CLI_GUEST},
    {"CodecWrite",  "cw",   _AdacCmdCodecWrite, NULL,   "WM8776/MT8291 register write", CLI_GUEST},
    {"CodecChSel",  "ch",   _AdacCmdCodecChlSel, NULL,  "WM8776/MT8291 register write", CLI_GUEST},
    {"AmpRead",     "ar",   _AdacCmdAmpRead, NULL,  "Amplifier register read", CLI_GUEST},
    {"AmpWrite",    "aw",   _AdacCmdAmpWrite, NULL, "Amplifier register write", CLI_GUEST},
    {"AmpRamRead",  "arr",  _AdacCmdAmpRamRead, NULL, "Amplifier ram read", CLI_GUEST},
    {"AmpRamWrite", "awr",  _AdacCmdAmpRamWrite, NULL, "Amplifier ram write", CLI_GUEST},
    // last cli command record, NULL
    {
        NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
    }
};

static CLI_EXEC_T _arAudInterDacCmdTbl[] =
{
    {"channel sel", "ch",   _InterDacChlSel,    NULL,   "Internal DAC channel Select", CLI_GUEST},
    {"mute",        "m",    _InterDacMute,      NULL,	"Internal DAC Mute", CLI_GUEST},
    {"enable",      "e",    _InterDacEnable,    NULL,	"Internal DAC Enable", CLI_GUEST},
    {"init",        "i",    _InterDacCmdInit,   NULL,   "Internal DAC Init", CLI_SUPERVISOR},
    {"LRInvert",    "inv",  _InterDacLRInvert,   NULL,	"Internal LR invert", CLI_GUEST},
    {"ADFmt",       "adf",  _InterADCFmt,   NULL,    	"Internal DAC I2S format", CLI_GUEST},
    {"DAFmt",       "daf",  _InterDACFmt,   NULL,	    "Internal ADC I2S format", CLI_GUEST},
    {"PWM enable",  "pwm",  _InterPWMEnable,   NULL,	"PWM enable OR disable", CLI_GUEST},
    {"PWMgainSet",  "pg",   _InterPWMGain,   NULL,	    "Set PWM gain", CLI_GUEST},
    //{"ramp",        "rmp",  _InterDacRamp,      NULL,	"Internal DAC Ramp", CLI_GUEST},  // for pwm ramp function test
    //{"XYRamTest",   "xyt",  _InterDacXYRamTest, NULL,	"Internal DAC XY Ram Test", CLI_GUEST},  //fro PWM xy ram test
    // last cli command record, NULL
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#if 1 // PROBE_RELATED_API
static CLI_EXEC_T _arAudProbeCmdTbl[] =
{
    {"pause",       "p",    _AudCmdProbePause,  NULL,       "Probe: Pause", CLI_SUPERVISOR},
    {"step",        "st",   _AudCmdProbeStep,  NULL,       "Probe: Step",  CLI_SUPERVISOR},
    {"go",          "g",    _AudCmdProbeGo,     NULL,       "Probe: Go",    CLI_SUPERVISOR},
    {"read",        "r",    _AudCmdProbeRead,   NULL,       "Probe: Read",  CLI_SUPERVISOR},
    {"write",       "w",    _AudCmdProbeWrite,   NULL,      "Probe: Write",  CLI_SUPERVISOR},
    {"ShowRegs",    "sr",   _AudCmdShowRegs,   NULL,       "Probe: Show Registers",    CLI_SUPERVISOR},
    {"SetBPts",     "stb",  _AudCmdSetBPts,   NULL,       "Probe: Setup BreakPoints", CLI_SUPERVISOR},
    {"ShowBPts",    "sb",   _AudCmdShowBPts,   NULL,       "Probe: Show BreakPoints",  CLI_SUPERVISOR},
    {"StopOnWrite", "sw",   _AudCmdStopOnWrite, NULL,       "Probe: Stop on Write",   CLI_SUPERVISOR},
    {"gsr",         "gsr",  _AudCmdProbeGoThenShow, NULL, "Probe: Go then Show",   CLI_SUPERVISOR},
    {"stsr",        "stsr", _AudCmdProbeStepThenShow, NULL, "Probe: Go then Show",   CLI_SUPERVISOR},
    //ZY_FPGA
    {"sramtest",    "sramtest", _AudCmdProbeSramTest, NULL, "Sram Test", CLI_SUPERVISOR},
    {"reset",       "reset", _AudCmdProbeReset, NULL, "Reset DSP", CLI_SUPERVISOR},
    //ZY_FPGA
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

static CLI_EXEC_T _arAudDspCmdTbl[] =
{
    {"ptr",         "p",    _AdspCmdPtr, NULL,      "get AFIFO read/write pointer cmd ex. ptr", CLI_SUPERVISOR},
    {"init",        "i",    _AdspDSPInit, NULL,     "adsp task init ex. init", CLI_SUPERVISOR},
    {"pc",          NULL,   _AdspDSPPC, NULL,       "Get dsp pc ex. pc", CLI_SUPERVISOR},
#if 1 // PROBE_RELATED_API
    {"probe",       "pr",   NULL,  _arAudProbeCmdTbl, "dsp probe command", CLI_SUPERVISOR },
#endif
    {"reads",       "r",    _AdspReadSram, NULL,    "Read DSP sram ex. reads [addr]", CLI_GUEST},
    {"writes",      "w",    _AdspWriteSram, NULL,   "Write DSP sram ex. write [addr] [data]", CLI_GUEST},
    {"pts",         "pts",  _AudCmdPts, NULL,       "get pts ex.pts [DecId]", CLI_SUPERVISOR},
    {"updateReadPts",   "rpts", _AdspUpdateReadPtr, NULL,       "update read ptr ex.rpts [DecId] [Addr]", CLI_SUPERVISOR},
    {"SetLogPeriod",    "sp",   _AudCmdSetLogPeriod, NULL,      "set error log period ex.sp [second] ", CLI_SUPERVISOR},
    {"shareinfo",   "sh",   _AdspDSPShareInfo, NULL,        "read dsp share memory ex.sh [group] ", CLI_GUEST},
    {"commondram",  "cm",   _AdspDSPCommonDram, NULL,       "read dsp common memory ex.cm [addr] [length] ", CLI_SUPERVISOR},
    {"commondram",  "cmw",  _AdspDSPCommonDramWrite, NULL,       "write dsp common memory ex.cm [addr] [value] ", CLI_SUPERVISOR},
    {"normdram",  "norm",   _AdspDSPNormDram, NULL,       "read dsp normal dram", CLI_SUPERVISOR},
    {"cmptdram",  "cmpt",   _AdspDSPCmptDram, NULL,       "read dsp compact dram", CLI_SUPERVISOR},
    {"query",       "q",    _AdspDSPQuery, NULL,        "query dsp status", CLI_GUEST},
#ifdef BURN_IN_AOUT_TEST
    {"Aout CRC query",  "qaout",  _AdspAoutQuery, NULL,        "query aout status", CLI_GUEST},
    {"Aout CRC reset",  "raout",   _AdspAoutReset, NULL,        "Reset aout status", CLI_GUEST},
    {"Aout CRC set",  "saout",    _AdspAoutSet, NULL,        "Set aout test", CLI_GUEST},
#endif
    {"shareinfowrite",  "shw",  _AdspDSPSharInfoWrite, NULL,        "write dsp share memory ex.sh [group][addr][val][size] ", CLI_GUEST},
    {"chldump",     "c",    _AdspDSPChlDump, NULL,      "dump channel ouput data", CLI_GUEST},
    {"StcDiffBound",   "stcbd", _AdspDSPStcDiffBound, NULL, "set stc difference bound", CLI_GUEST},
    {"query mts",       "qmts",    _AdspMTSQuery, NULL,        "query mts status", CLI_GUEST},
#if 1///def CC_VORBIS_SUPPORT    ///CC_VORBIS_SUPPORT_Q
    {"query vorbis",    "qvobs",    _AdspVorbisQuery,   NULL,   "query vorbis status",  CLI_GUEST},
#endif
    {"pcm pre-buffer",       "pb",    _AudPreBuffer, NULL,        "pcm pre-buffer", CLI_GUEST},
    {"cache",       "cache",    _AudCacheHitRate, NULL,        "DSP cache hit rate", CLI_GUEST},
    {"ROMID",       "id",    _AdspDSPROMID, NULL,        "ROM ID", CLI_GUEST},
    {"qm",          "qm",    _AudCmdQueryDspMemory, NULL, "Query DSP Memory", CLI_GUEST},
    {"ver",         "v",    _AudCmdDspVer,          NULL,  "DSP Version Display", CLI_GUEST},
#if ((defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)) && !defined(CC_AUD_DISABLE_DSP_PAT))
    {"pat", "pat", _AudCmdPatTest, NULL, "Pattern Test", CLI_SUPERVISOR},
    {"patauto", "patauto", _AudCmdPatAuto, NULL, "Pattern Auto", CLI_SUPERVISOR},
    //{"fillbuf", "fillbuf", _AudCmdFillBuf, NULL, "Fill AOUT Buf", CLI_SUPERVISOR},
#endif
    {"Query Delay",   "qdly",   _AdspQChannelDelay, NULL,  "Query Channel Delay", CLI_GUEST},
    {"dinfo",   "dinfo", _AudCmdDebug1, NULL,  "DEBUG INFO ~~", CLI_GUEST},
	{"Get capability", "cap",   _AdspGetCap, NULL, "Get DSP supported decoding format", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudDolbyCmdTbl[] =
{
    {"query",       "q",    _AdspDolbyQuery, NULL,        "query Dolby related information", CLI_GUEST},
    {"impulse",       "impulse",    _AdspDolbyImpulse, NULL,        "generate impluse signal", CLI_GUEST},
    {"tone",       "t",    _AudCmdPlayTone, NULL,        "generate tone", CLI_GUEST},    
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};


#if 0 //def __MODEL_slt__
static CLI_EXEC_T _arAudDSPSLTCmdTbl[] =
{
    {"init",        "i",    _ADSPSLTInit,   NULL,       "adsp SLT init. once",  CLI_SUPERVISOR},
    {"test",        "t",    _ADSPSLTTest,   NULL,       "adsp SLT testing",     CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

static CLI_EXEC_T _arAudUopEqTbl[] =
{
    {"flag",        "flag",    _AudCmdUopEqFlag , NULL,   "EQ function on/off", CLI_GUEST},
    {"mode",        "m",     _AudCmdUopEqMode, NULL, "EQ mode configuration", CLI_GUEST},
    {"eqTableSet", "set", _AudCmdUopEqTableSet, NULL, "Set EQ Table", CLI_GUEST},
    {"eqTableQry", "q", _AudCmdUopEqTableQry, NULL, "Query EQ Table", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudUopSbassTbl[] =
{
    {"flag",        "flag", _AudCmdUopSbassFlag , NULL,   "sbass function on/off", CLI_GUEST},
    {"bassboost",   "bb",     _AudCmdUopBassBoostGain, NULL, "Bass boost gain configuration", CLI_GUEST},
    {"clearboost",  "cb",     _AudCmdUopClearBoostGain, NULL, "Clear boost gain configuration", CLI_GUEST},
    {"query",       "q", _AudCmdUopSbassTableQry, NULL, "query sbass Table", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_AUD_SILENCE_SUPPORT
static CLI_EXEC_T _arAudSilenceSetTbl[] =
{
    {"flag",        "flag", _AudCmdUopSilenceFlag , NULL,   "silence function on/off", CLI_GUEST},
    {"threshold",   "thre",     _AudCmdUopSilenceThreshold, NULL, "Silence Threshold configuration", CLI_GUEST},
    {"attack wait time", "wait", _AudCmdUopSilenceWait , NULL,   "Silence attack wait time  configuration", CLI_GUEST},
    {"attack fix gain", "gain", _AudCmdUopSilenceFixGain , NULL,   "Silence attack fix gain  configuration", CLI_GUEST},
    {"release step",  "rstep",     _AudCmdUopSilenceReleaseStep, NULL, "Silence Release Step configuration", CLI_GUEST},
    {"attack step",  "astep",     _AudCmdUopSilenceAttackStep, NULL, "Silence Attack Step configuration", CLI_GUEST},
    {"query",       "q", _AudCmdUopSilenceSetQry, NULL, "query Silence configuration", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

static CLI_EXEC_T _arAudUopLimiterTbl[] =
{
    {"mode",        "m", _AudCmdUopLimiterMode , NULL,   "limiter mode", CLI_GUEST},
    {"flag",   "flag",     _AudCmdUopLimiterFlag, NULL, "individual limiter flag", CLI_GUEST},
    {"threshold",  "thre",     _AudCmdUopLimiterThre, NULL, "limiter fixed mode threshold", CLI_GUEST},
#ifdef CC_RATIO_LIMITER
    {"ratio",  "ratio",     _AudCmdUopLimiterGainRatio, NULL, "limiter gain ratio", CLI_GUEST},
#endif
    {"query",       "q", _AudCmdUopLimiterQuery, NULL, "query limiter configuration", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudUopPL2Tbl[] =
{
    {"init",        "i",    _AudCmdUopPL2Init , NULL,   "initialization for prologic2", CLI_GUEST},
    {"switch",      "sw",   _AudCmdUopPL2Switch , NULL, "prologic2 On/Off/Auto switch", CLI_GUEST},
    {"mode",        "mode", _AudCmdUopPL2Mode , NULL,   "setup prologic2 mode        ", CLI_GUEST},
    {"pano",        "pano", _AudCmdUopPL2Pano , NULL,   "panorama on/off             ", CLI_GUEST},
    {"dim",         "dim",  _AudCmdUopPL2Dim , NULL,    "dimension control           ", CLI_GUEST},
    {"cw",          "cw",   _AudCmdUopPL2CWidth , NULL, "center width control        ", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#ifdef DSP_SUPPORT_SRSWOW
static CLI_EXEC_T _arAudUopSRSWOWTbl[] =
{
    {"OnOff",       "onoff", _AudCmdUopSRSWOW,       NULL,  "SRS WOW On/Off",   CLI_SUPERVISOR},
    {"wspk",        "wspk",  _AudCmdUopSRSWOWSpkSz,  NULL,  "Speaker Size",     CLI_SUPERVISOR},
    {"wbrt",        "wbrt",  _AudCmdUopSRSWOWBright, NULL,  "Brightness",       CLI_SUPERVISOR},
    {"wtb",         "wtb",   _AudCmdUopSRSWOWTBLvl,  NULL,  "TruBass Level",    CLI_SUPERVISOR},
    {"wwid",        "wwid",  _AudCmdUopSRSWOWWidth,  NULL,  "Width",            CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif
#ifdef DSP_SUPPORT_SRSTSXT
static CLI_EXEC_T _arAudUopSRSTSXTTbl[] =
{
    {"tspk",        "tspk",  _AudCmdUopSRSTSXTSpkSz,  NULL,  "Speaker Size",     CLI_SUPERVISOR},
    {"tsmode",      "tsmd",  _AudCmdUopSRSTSXTMode,  NULL,  "Mode",     CLI_SUPERVISOR},
    {"telv",        "telv",  _AudCmdUopSRSTSXTElev,   NULL,  "Elevation",        CLI_SUPERVISOR},
    {"ing",        "ing",  _AudCmdUopSRSTSXTinputgain,   NULL,  "input gain",        CLI_SUPERVISOR},
    {"ttb",         "ttb",   _AudCmdUopSRSTSXTTBLvl,  NULL,  "TruBass Level",    CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

#if (defined(DSP_SUPPORT_SRSTSHD) || defined(DSP_SUPPORT_SRSWOWHD))//sunman_tshd
static CLI_EXEC_T _arAudUopSRSTSHDTbl[] =
{
	{"tinit", "tinit",		_AudCmdUopSRSTSHDSetInit,	NULL,	"Initial settings ",        	CLI_SUPERVISOR},
	{"tinpg",	"tinpg",	_AudCmdUopSRSTSHDInpGain,	NULL,	"Input gain",	       	CLI_SUPERVISOR},
       {"toutg",	"toutg",	_AudCmdUopSRSTSHDOutGain,	NULL,	"Output gain",		       CLI_SUPERVISOR},
     	{"tsrlv",  	"tsrlv",	_AudCmdUopSRSTSHDSurrLvl,	NULL,	"Surround Level ",        	CLI_SUPERVISOR},
 	{"tspk",	"tspk",	_AudCmdUopSRSTSHDSpkSz,	NULL,	"Speaker Size",		CLI_SUPERVISOR},
	{"ttb",	"ttb",	_AudCmdUopSRSTSHDTBLvl,	NULL,	"TruBass Level",    		CLI_SUPERVISOR},
	{"tdef",	"tdef",	_AudCmdUopSRSTSHDDef,   	NULL,	"Definition Control",	CLI_SUPERVISOR},
	{"telv",	"telv",	_AudCmdUopSRSTSHDElev,		NULL,	"FOCUS Elevation",       CLI_SUPERVISOR},
	{"fcc",	"fcc",	_AudCmdUopSRSTSHDFntCCtrl,		NULL,	"TS/WOW Front Center Countrol",       CLI_SUPERVISOR},
	{"fsc",	"fsc",	_AudCmdUopSRSTSHDFntSCtrl,		NULL,	"TS/WOW Front Space Countrol",       CLI_SUPERVISOR},
       {"tmode","tmode",	_AudCmdUopSRSTSHDMode,	NULL,	"TSHD Mode",			CLI_SUPERVISOR},
	//{"tflag",	"tflag",	_AudCmdUopSRSTSHDSetFlag,	NULL,	"TSHD Flag",			CLI_SUPERVISOR},
	//{"tmode","tmode",	_AudCmdUopSRSTSHDSetLtRt_PL2M,	NULL,	"LtRt or PL2Music",	CLI_SUPERVISOR},
	//{"tsspk",	"tsspk",	_AudCmdUopSRSTSHDSubSpkSz,NULL,	"Sub Speaker Size",	CLI_SUPERVISOR},
	//{"tcofq",	"tcofq", 	_AudCmdUopSRSTSHDCOFREQ,	NULL,	"CrossOver Freq",		CLI_SUPERVISOR},
	//{"tts",	"tts",	_AudCmdUopSRSTSHDSetTSCtrl,   	NULL,	"TruSurround Control",	CLI_SUPERVISOR},
	//{"tlim",  	"tlim",	_AudCmdUopSRSTSHDLimiter,	NULL,	"Limiter Control",        	CLI_SUPERVISOR},
	//{"timod", "timod",	_AudCmdUopSRSTSHDInpuMode,	NULL,	"Set Input Mode ",        	CLI_SUPERVISOR},
	//{"tomod", "tomod",	_AudCmdUopSRSTSHDOutputMode,	NULL,	"Set Output Mode ",        	CLI_SUPERVISOR},
	//{"tbypass", "tbypass",		_AudCmdUopSRSTSHDSetBypass,	NULL,	"Set Bypass Mode ",        	CLI_SUPERVISOR},
	//{"tout", "tout",	 _AudCmdUopSRSTSHDSetSubToL,	NULL,	"Set Sub, Center to L, R ",        	CLI_SUPERVISOR},
     {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

#ifdef DSP_SUPPORT_SRSVIQ
//sunman_viq
static CLI_EXEC_T _arAudUopSRSVIQTbl[] =
{
    {"ref",        "ref",  _AudCmdUopSRSVIQRefLvl,  NULL,  "Reference level",     CLI_GUEST},
    {"mode",    "mode",  _AudCmdUopSRSVIQMode,  NULL,  "Mode",     CLI_GUEST},
    {"maxg",    "maxg",  _AudCmdUopSRSVIQMaxGain,  NULL,  "Max gain",     CLI_GUEST},
    {"inpg",    "inpg",  _AudCmdUopSRSVIQInputGain,  NULL,  "Input gain",     CLI_GUEST},
    {"outpg",    "outpg",  _AudCmdUopSRSVIQOutputGain,  NULL,  "Output gain",     CLI_GUEST},
    {"noeq",    "noeq",   _AudCmdUopSRSVIQDisableEQ,  NULL,  "Close EQ",     CLI_GUEST},
    {"tnoise",	 "tnoise",	_AudCmdUopSRSVIQNoiseTh, NULL,	"Noise TH",       CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

#ifdef DSP_SUPPORT_SRS_SSHD
//sunman_peq
static CLI_EXEC_T _arAudUopSRSPeqTbl[] =
{
    {"on",     "on",  _AudCmdUopSRSPeqOn,  NULL,  "on/off", CLI_GUEST},
    {"inpg",   "inpg",  _AudCmdUopSRSPeqInputGain,  NULL,  "input gain",     CLI_GUEST},
    {"outpg", "outpg",  _AudCmdUopSRSPeqOutputGain,  NULL,  "output gain",     CLI_GUEST},
    {"hpf",     "hpf",  _AudCmdUopSRSHpfOn,  NULL,  "hpf on/off",     CLI_GUEST},
    {"sc",     "sc",  _AudCmdUopSRSPeqSetCoef,  NULL,  "peq hpf Set coef",     CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};

//sunman_lmt
static CLI_EXEC_T _arAudUopSRSLmtTbl[] =
{
    {"on",     "on",  _AudCmdUopSRSLmtOn,  NULL,  "on/off",     CLI_GUEST},
    {"inpg",   "inpg",  _AudCmdUopSRSLmtInputGain,  NULL,  "input gain",     CLI_GUEST},
    {"outpg", "outpg",  _AudCmdUopSRSLmtOutputGain,  NULL,  "output gain",     CLI_GUEST},
    {"bst",   "bst",  _AudCmdUopSRSLmtBoost,  NULL,  "limiter boost",     CLI_GUEST},
    {"lvl", "lvl",  _AudCmdUopSRSLmtLvl,  NULL,  "limiter level",     CLI_GUEST},
    {"dly", "dly",  _AudCmdUopSRSLmtDly,  NULL,  "delay count",     CLI_GUEST},
    {"coeff", "coeff",  _AudCmdUopSRSLmtCoeff,  NULL,  "coeff level",     CLI_GUEST},
    {"thresh", "thresh",  _AudCmdUopSRSLmtThresh,  NULL,  "thresh level",     CLI_GUEST},

    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};

#endif

#ifdef DSP_SUPPORT_SRS_PURESOUND

//bofeng_aeq
static CLI_EXEC_T _arAudUopSRSPureTrubassTbl[] =
{
    {"on",     "on",  _AudCmdUopSRSPuresoundTrubassOn,  NULL,  "on/off", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};

static CLI_EXEC_T _arAudUopSRSAeqTbl[] =
{
    {"on",     "on",  _AudCmdUopSRSAeqOn,  NULL,  "on/off", CLI_GUEST},
    {"inpg",   "inpg",  _AudCmdUopSRSAeqInputGain,  NULL,  "input gain",     CLI_GUEST},
    {"outpg", "outpg",  _AudCmdUopSRSAeqOutputGain,  NULL,  "output gain",     CLI_GUEST},
    {"aeqsize",     "aeqsize",  _AudCmdUopSRSAeqFilterSize,  NULL,  "aeq filter size",     CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};

//bofeng_lmt
static CLI_EXEC_T _arAudUopSRSLmtTbl[] =
{
    {"on",     "on",  _AudCmdUopSRSLmtOn,  NULL,  "on/off",     CLI_GUEST},
    {"inpg",   "inpg",  _AudCmdUopSRSLmtInputGain,  NULL,  "input gain",     CLI_GUEST},
    {"outpg", "outpg",  _AudCmdUopSRSLmtOutputGain,  NULL,  "output gain",     CLI_GUEST},
    {"bst",   "bst",  _AudCmdUopSRSLmtBoost,  NULL,  "limiter boost",     CLI_GUEST},
    {"lvl", "lvl",  _AudCmdUopSRSLmtLvl,  NULL,  "limiter level",     CLI_GUEST},
    {"dly", "dly",  _AudCmdUopSRSLmtDly,  NULL,  "delay count",     CLI_GUEST},
    {"coeff", "coeff",  _AudCmdUopSRSLmtCoeff,  NULL,  "coeff level",     CLI_GUEST},
    {"thresh", "thresh",  _AudCmdUopSRSLmtThresh,  NULL,  "thresh level",     CLI_GUEST},

    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};

#endif


#ifdef CC_AUD_BBE_SUPPORT
static CLI_EXEC_T _arAudUopBBETbl[] =
{
    {"query",    "q", _AudCmdUopBBEQuery,  NULL, "Query BBE Config", CLI_GUEST},
    {"setmode", "mode",   _AudCmdUopBBEMode, NULL, "Set BBE Mode", CLI_GUEST},
    {"setlevel", "level", _AudCmdUopBBELevel,  NULL, "Set BBE Level", CLI_GUEST},
    {"sethigh", "high",  _AudCmdUopBBEHighGain, NULL, "Set BBE High Gain", CLI_GUEST},
    {"setlow", "low", _AudCmdUopBBELowGain,  NULL, "Query BBE Low Gain", CLI_GUEST},
    {"set3d" , "3d", _AudCmdUopBBE3DGain, NULL, "Query BBE Config", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

static CLI_EXEC_T _arAudUopSpkCfgTbl[] =
{
    {"spkcfg",     "spkcfg",    _AudCmdUopSpeakerConfig , NULL, "speaker config steup", CLI_GUEST},
    {"delaycfg",   "delaycfg",  _AudCmdUopChannelDelay , NULL,  "channel delay steup", CLI_GUEST},
    {"largesmall", "ls",        _AudCmdUopSpeakerLargeSmallConfig , NULL,   "speaker large/small config", CLI_GUEST},
    {"cutoff_freq","cf",        _AudCmdUopCutFreq , NULL,   "set cutoff frequency", CLI_GUEST},
    {"channel",    "ch",        _AudCmdUopSpeakerOutputConfig , NULL,   "speaker config", CLI_GUEST},
    {"subwoofer",  "sw",        _AudCmdUopSpeakerSubwooferEnable , NULL,    "subwoofer on/off", CLI_GUEST},
    {"bass",       "bs",        _AudCmdUopBassMngEnable , NULL, "bass management on/off", CLI_GUEST},
    {"autodm",     "dm",        _AudCmdUopAutoDmx , NULL,   "auto downmix on/off", CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
static CLI_EXEC_T _arAudUopADVTbl[] =
{
    {"ADV mode",     "mode",    _AudCmdUopADV , NULL, "ADV mode steup", CLI_GUEST},
    {"ADV query",   "q",  _AudCmdUopADV_Query , NULL,  "ADV query", CLI_GUEST},
    {"ADV set chGain",   "set",  _AudCmdUopADVSetChGain , NULL,  "ADV channel Gain setting", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
static CLI_EXEC_T _arAudUopCDNotchTbl[] =
{
    {"CDNotch Flag", "flag", _AudCmdUopCDNotchFlag, NULL, "CDNotch enable", CLI_GUEST},
		{"CDNotch Config", "cfg", _AudCmdUopCDNotchConfig, NULL, "CDNotch Q and Fc setup", CLI_GUEST},
		{NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

#ifdef CC_AUD_FIR_SUPPORT
static CLI_EXEC_T _arAudUopConeqTbl[] =
{
	{"CONEQ mode", 	"flag",		_AudCmdUopFIREnable, NULL,	"Set CONEQ flag", CLI_GUEST},
    {"CONEQ dump address", "q",  _AudCmdUopGetPhyAddr,       NULL, "Get CONEQ table address", CLI_GUEST},

	{NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif


#ifdef KARAOKE_SUPPORT
static CLI_EXEC_T _arAudUopKeyShiftTbl[] =
{
    {"Keyshift Flag", "flag", _AudCmdUopKeyShiftFlag, NULL, "Keyshift enable", CLI_GUEST},
	{"Keyshift key", "key", _AudCmdUopKeyShiftKey, NULL, "Keyshift key setup", CLI_GUEST},
	{NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};
#endif

static CLI_EXEC_T _arAudUopCmdTbl[] =
{
    {"fine volume", "fv",    _AudCmdUopFineVolume, NULL, "Audio Master Volume fine control", CLI_GUEST},
    {"volume",      "v",     _AudCmdUopVolume, NULL, "Audio Volume control", CLI_GUEST},
    {"vol_curve",   "vc",    _AudCmdUopVolumeCurve, NULL, "Audio Volume Curve Set", CLI_GUEST},
    {"vol_curve_query","vcq", _AudCmdUopQueryVolumeCurve, NULL, "Audio Volume Curve Query", CLI_GUEST},
#ifdef CC_SET_VOLUME
    {"set volume table", "svt",    _AudCmdUopSetVolTable, NULL, "Audio Volume table set ", CLI_GUEST},
#endif
#ifdef CC_AUD_NEW_CV_TABLE
    {"set CHVOl table", "scvt", _AudCmdUopSetChVolTable, NULL, "Audio Channle Volume table set ", CLI_GUEST},
    {"ch_vol_curve_query","cvcq", _AudCmdUopQueryChannelVolumeCurve, NULL, "Channel Audio Volume Curve Query", CLI_GUEST},
#endif
    {"ch volume",   "cv",     _AudCmdUopChVolume, NULL, "Audio Channel Volume control", CLI_GUEST},
    {"ch vol gain", "cvg",    _AudCmdUopChVolumeGain, NULL, "Audio Channel Volume Gain control", CLI_GUEST},
#ifdef CC_AUD_VOLUME_OFFSET
    {"volume offset", "vo",	  _AudCmdUopVolumeOffset, NULL, "Audio Channel Volume offset", CLI_GUEST},
#endif
#ifdef CC_AUD_LINEOUT_VOL_OFFSET
    {"linout vol offset",	"lovo",	  _AudCmdUopLineOutVolOffset, NULL, "Audio Line out Volume offset", CLI_GUEST},
#endif
#ifdef CC_AUD_LINEOUT_VOL_MODE
    {"linout vol", "lov",	  _AudCmdUopLinOutVolume, NULL, "Audio Line out Volume control", CLI_GUEST},
#endif
#ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
    {"src linout vol", "slov",	  _AudCmdUopSrcLinOutVolume, NULL, "Audio Source Line out Volume set", CLI_GUEST},
#endif
    {"src volume",  "sv",  _AudCmdUopSrcVolume, NULL, "Audio Source Volume control", CLI_GUEST},
    {"src volume query", "svq", _AudCmdUopGetSrcVolume, NULL, "Query Audio Source Volume", CLI_GUEST},
    {"equalizer",   "eq",     NULL, _arAudUopEqTbl, "EQ configuration", CLI_GUEST},
    {"sbass",   "sbass",     NULL, _arAudUopSbassTbl, "Bass/treble configuration", CLI_GUEST},
#ifdef CC_AUD_SILENCE_SUPPORT
    {"Silence set",   "silence",   NULL,  _arAudSilenceSetTbl, "Slience fuction set", CLI_GUEST},//for silence fuction
#endif
    {"limiter",   "limiter",     NULL, _arAudUopLimiterTbl, "Limiter configuration", CLI_GUEST},
    {"pl2cfg",      "pl2cfg", NULL, _arAudUopPL2Tbl,"Prologic 2 related configuration", CLI_GUEST},
    {"spkuop",      "spkuop", NULL, _arAudUopSpkCfgTbl,"Speaker related configuration", CLI_GUEST},
    {"AVC",      "a",     _AudCmdUopAVC, NULL, "Automatic Volume Control", CLI_GUEST},
    {"MonoDownmix", "md",     _AudCmdUopMonoDownmix, NULL, "Downmix to mono", CLI_SUPERVISOR},
    {"Karaoke mix ratio", "kr",     _AudCmdUopKaraokeMixRatio, NULL, "Karaoke mix ratio", CLI_SUPERVISOR},
#ifdef FRAC_SPEED_SUPPORT
    {"playSpeed", "fspeed",  _AudCmdUopPlaySpeed, NULL, "Set fractional play speed", CLI_GUEST},  //sunman for play speed
#endif
#if defined(__linux__)
    {"balance",      "bl",     _AudCmdUopBalance, NULL, "Set L/R balance", CLI_GUEST},
#endif
#ifdef DSP_SUPPORT_SRSWOW
    {"srswow",      "wow", NULL, _arAudUopSRSWOWTbl, "Set SRS WOW", CLI_GUEST},
#endif
#ifdef DSP_SUPPORT_SRSTSXT
    {"srstsxt",     "tsxt", NULL, _arAudUopSRSTSXTTbl, "Set SRS TSXT", CLI_GUEST},
#endif
    {"virtualsurround",   "vs",     _AudCmdUopVirtualSurround, NULL, "Virtual Surround flag", CLI_GUEST},
    {"vsurround config",   "vscfg",     _AudCmdUopVSurrCfg, NULL, "Virtual Surround Config", CLI_GUEST},
#if (defined(DSP_SUPPORT_SRSTSHD) || defined(DSP_SUPPORT_SRSWOWHD))
    {"srstshd",     "tshd", NULL, _arAudUopSRSTSHDTbl, "Set SRS TSHD", CLI_GUEST},  //sunman_tshd
#endif
#ifdef DSP_SUPPORT_SRSVIQ
    {"VIQ",      "viq", NULL,  _arAudUopSRSVIQTbl, "SRS Volume IQ", CLI_GUEST},  	//sunman_viq
#endif
#ifdef DSP_SUPPORT_SRS_SSHD
    {"peq",    "peq", NULL,  _arAudUopSRSPeqTbl, "SRS PEQ", CLI_GUEST},  	//sunman_peq
    {"lmt",    "lmt", NULL,  _arAudUopSRSLmtTbl, "SRS Limiter", CLI_GUEST},  //sunman_lmt
#endif
#ifdef DSP_SUPPORT_SRS_PURESOUND
	{"purethd",    "purethd", NULL,  _arAudUopSRSPureTrubassTbl, "SRS PURE THD", CLI_GUEST}, 	//bofeng_thd
    {"aeq",    "aeq", NULL,  _arAudUopSRSAeqTbl, "SRS AEQ", CLI_GUEST},  	//bofeng_aeq
    {"lmt",    "lmt", NULL,  _arAudUopSRSLmtTbl, "SRS Limiter", CLI_GUEST},  //bofeng_lmt
#endif
#ifdef CC_AUD_SOUND_MODE
    {"post sound mode",   "psm",     _AudCmdUopPostSoundMode, NULL, "Sound mode change", CLI_GUEST},
#endif
    //{"surround mode",   "vsm",     _AudCmdUopVSMode, NULL, "surround mode change", CLI_GUEST},
#ifdef CC_AUD_BBE_SUPPORT
    {"bbe",         "bbe", NULL, _arAudUopBBETbl, "Set BBE Function", CLI_GUEST},
#endif
#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
	{"cfg-depth notch", "cdnotch", NULL, _arAudUopCDNotchTbl, "Set CDNotch Parameters", CLI_GUEST},
#endif
#ifdef KARAOKE_SUPPORT
	{"keyshift", "keyshift", NULL, _arAudUopKeyShiftTbl, "Set Keyshift Parameters", CLI_GUEST},
#endif
#ifdef CC_AUD_HPF_SUPPORT
    {"hpf",         "hpf",      _AudCmdUopHpfEnable, NULL,  "Set HPF enable",    CLI_GUEST},
    {"hpf_fc",      "fc",       _AudCmdUopHpfFc, NULL,      "Set HPF Fc",        CLI_GUEST},
    {"hpf2_fc",      "fc",       _AudCmdUopHpf2Fc, NULL,      "Set HPF2 Fc",        CLI_GUEST},
#endif
#ifdef CC_AUD_CLIPPER_SUPPORT
    {"clipper",     "clip",     _AudCmdUopClipper, NULL,    "Set clipper enable",CLI_GUEST},
    {"clip_val",    "clipval",  _AudCmdUopClipperVal, NULL, "Set clipper value", CLI_GUEST},
#endif
#ifdef CC_AUD_PEQ_SUPPORT
    {"peq",         "peq",      _AudCmdUopPEQ, NULL,        "Set PEQ",           CLI_GUEST},
    {"peqc",        "peqc",     _AudCmdUopPEQCfg, NULL,     "Set PEQ Configuration", CLI_GUEST},
    {"peqc2",       "peqc2",    _AudCmdUopPEQCfg2, NULL,    "Set PEQ Configuration", CLI_GUEST},
#endif
#ifdef CC_AUD_SPEAKER_HEIGHT
    {"sph", 		"sph",		_AudCmdUopSPH, NULL,		"Set Speaker Height", CLI_GUEST},
#endif
#ifdef CC_AUD_VBASS_SUPPORT
    {"vb",			"vb",		_AudCmdUopVirBass, NULL,		"Set Virtual Bass", CLI_GUEST},
    {"vbcfg",		"vbcfg",	_AudCmdUopVBCfg, NULL,		"Set Virtual Bass config", CLI_GUEST},
    {"vb cut",		"vbcut",	_AudCmdUopVirBassCut, NULL, "Set Virtual Bass Cutoff", CLI_GUEST},
#endif //#CC_AUD_VBASS_SUPPORT
    {"FF SPEED",    "speed",    _AudCmdDspSpeed , NULL,     "DSP Speed", CLI_GUEST},
    {"DMixPos",     "dmp",      _AudCmdUopDownmixPosition , NULL, "Downmix channel position", CLI_GUEST},
    {"MultiPair",   "MultiPair",_AudCmdUopMultiPairOutput , NULL, "Multi-Pair output", CLI_GUEST},
    {"LR DownMix",  "LRDMix",   _AudCmdUopLRDownMix, NULL, "LR DownMix", CLI_GUEST},
    {"dump",        "dump",     _AudCmdDspUopDump , NULL, "DSP Dump", CLI_GUEST},
    {"DDCO",        "DDCO",     _AudCmdUopDDCO , NULL, "DDCO setting", CLI_GUEST},
    {"DDCO AGC",        "DDCOAgc",     _AudCmdUopDDCOAgc, NULL, "DDCO AGC setting", CLI_GUEST},
    {"DDCO LFE LFP",        "DDCOLFE",     _AudCmdUopDDCOLFE, NULL, "DDCO AGC setting", CLI_GUEST},
    {"dualmono",    "dm",    _AudCmdDtvDualMono , NULL,    "dual-mono steup", CLI_SUPERVISOR},
#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
    {"ADV",         "ADV", NULL, _arAudUopADVTbl, "Set ADV Function", CLI_GUEST},
    {"ABX Config",  "abxcfg",   _AudCmdUopABXConfig, NULL, "ABX Config", CLI_GUEST},
    {"aeq",         "aeq",      _AudCmdUopAeqFlag,   NULL, "Set AEQ Flag", CLI_GUEST},
#endif
//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
    {"upload",        "up",     _AudCmdUopUploadData , NULL, "Upload data enable", CLI_GUEST},
    {"uploadinit",    "upi",    _AudCmdDataUploadInit, NULL, "Upload data init", CLI_GUEST},
    {"uploaddump",    "upd",    _AudCmdDataUploadDump, NULL, "Upload data dump", CLI_GUEST},
//#endif
    {"adupdate",    "adupdate",    _AudCmdAdFadePanPesUpdate , NULL,    "ad fade pan update setup", CLI_GUEST},
    {"adfade",    "adfade",    _AudCmdAdFade , NULL,    "ad fade setup", CLI_GUEST},
    {"adpan",    "adpan",    _AudCmdAdPan , NULL,    "ad pan setup", CLI_GUEST},
    {"adpanfaden",    "adpanfaden",   _AudCmdAdPanFadeEnable , NULL,    "ad pan fade enable", CLI_GUEST},
#ifdef CC_AUD_4_DECODER_SUPPORT
    {"sbcdump",   "sbcd", _AudCmdSBCEncTest, NULL, "SBC Encode Test", CLI_SUPERVISOR},
#endif
    {"adcdump",   "adcd", _AudCmdADCDump, NULL, "SBC Encode Test", CLI_SUPERVISOR},
    {"postq",     "pq",   _AudCmdUopPostQuery, NULL, "Post-Proc Query", CLI_GUEST},
    
#ifdef CC_AUD_FIR_SUPPORT
		{"coneq",	  "coneq", NULL, _arAudUopConeqTbl,  "Set Coneq Function", CLI_GUEST},
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudCfgCmdTbl[] =
{
    {"AvInSwitch",    "isw",     _AudCfgAvInSwitch,   NULL, "Av input component select", CLI_GUEST},
    {"ChlOutCfg",     "osw",     _AudCfgChlOutSwitch, NULL, "Out channel switch", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudAtvCmdTbl[] =
{
    {"query",     "q",    _AudCmdAtvquery, NULL, "query ATV audio informantion", CLI_SUPERVISOR},
    {"Tvsys",     "sys",    _AudCmdAtvTvSys, NULL, "Tv channel system: B/G, D/K, I, L, M", CLI_SUPERVISOR},
#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
    {"Tvsys auto",     "asys",    _AudCmdAtvAutoTvSys, NULL, "Set tvsys by detection", CLI_SUPERVISOR},
#endif
    {"sound mode",      "sm",     _AudCmdAtvSoundMode, NULL, "ATV Sound Mode: mono, stereo, dual,...", CLI_GUEST},
    {"MTS system", "mts",    _AudCmdAtvFacMts, NULL, "BTSC system factory options", CLI_GUEST},
    {"A2 system", "a2",     _AudCmdAtvFacA2, NULL, "A2 system factory options", CLI_GUEST},
    {"PAL system", "pal",     _AudCmdAtvFacPal, NULL, "PAL system factory options", CLI_GUEST},
    {"Query carrier shift",  "CFO",     _AUD_MTS_CFO_IDX, NULL, "Query MTS Carrier Shift", CLI_GUEST},
    {"query OVM  Detec",     "qovm",    _AudCmdAtvHdevDetQuery, NULL, "query ATV Hdev detection result", CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudDtvAC3Tbl[] =
{
    {"ac3kara",        "ac3ka",    _AudCmdDtvAC3KaraMode , NULL,    "ac3 karaoke mode steup", CLI_SUPERVISOR},
    {"ac3dualmono",    "ac3dm",    _AudCmdDtvAc3DualMono , NULL,    "ac3 dual mono mode steup", CLI_SUPERVISOR},
    {"ac3compmode",    "ac3cm",    _AudCmdDtvAc3CompMode , NULL,    "ac3 compresssion mode steup", CLI_SUPERVISOR},
    {"ac3drcrange",    "ac3dr",    _AudCmdDtvAc3DrcRange , NULL,    "ac3 drc range  steup", CLI_SUPERVISOR},
    {"ac3query", "q", _AudCmdDtvAc3Query, NULL, "ac3 query decoder status", CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_AUD_SUPPORT_MS10
static CLI_EXEC_T _arAudDtvDDCTbl[] =
{
    {"ddccontrol",        "ddc",    _AudCmdDtvDDCControl, NULL,    "DDC control steup", CLI_SUPERVISOR},
    {"strmid",        "id",    _AudCmdDtvDDCStrmId , NULL,    "DDC associated stream id steup", CLI_SUPERVISOR},
    {"mixer",        "m",    _AudCmdDtvDDCMixer , NULL,    "DDC mixer steup", CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

#ifdef CC_AUD_SUPPORT_MS10
static CLI_EXEC_T _arAudDtvDDTTbl[] =
{
    {"MDC bypass",          "mdc",    _AudCmdBypassMDC , NULL,    "DDT bypass mode", CLI_SUPERVISOR},
    {"dmx mode",            "dmx",    _AudCmdDtvDdtDmxMode , NULL,    "DDT downmix mode", CLI_SUPERVISOR},
    {"RF mode",             "rf",    _AudCmdDtvDdtRfMode , NULL,    "DDC RF/Line in mode", CLI_SUPERVISOR},
    {"Dual mode",           "dual",    _AudCmdDtvDdtDualMode , NULL,    "DDC dual mode", CLI_SUPERVISOR},
    {"DRC range",           "drc",    _AudCmdDtvDdtDrcRange , NULL,    "DDC DRC Cut/Boost factor", CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

static CLI_EXEC_T _arAudDtvCmdTbl[] =
{
    {"query DTV",     "q",    _AudCmdDtvQuery, NULL, "Query DTV decoder informantion", CLI_SUPERVISOR},
    {"ptsdly",      "ptsd",	_AudCmdDtvPTSDelay,     NULL,  "Delay audio startup by increasing PTS", CLI_GUEST},
    {"Set AC3",     "ac3",    NULL, _arAudDtvAC3Tbl, "Set AC3 decoder informantion", CLI_SUPERVISOR},
#ifdef CC_AUD_SUPPORT_MS10
    {"Set DDC",     "ddc",    NULL, _arAudDtvDDCTbl, "Set DDC decoder informantion", CLI_SUPERVISOR},
    {"Set DDT",     "ddt",    NULL, _arAudDtvDDTTbl, "Set DDT decoder informantion", CLI_SUPERVISOR},
#endif

    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudFMRadioCmdTbl[] =
{
    {"detect FM",     "det",    _AudCmdFMRadioDetect, NULL, "Detect FM radio channel", CLI_SUPERVISOR},
    {"play FM",       "play",   _AudCmdFMRadioPlay,   NULL, "Play FM radio channel", CLI_GUEST},
    {"FM stereo",     "fm2",   _AudCmdFMRadioSoundMode,   NULL, "Setup FM radio channel", CLI_GUEST},
    {"FM THL",        "fmthl",   _AudCmdFMRadioThre,   NULL, "Setup FM radio threshold", CLI_GUEST},
    {"FM Query THL",        "fmthlq",   _AudCmdFMRadioQueryThre,   NULL, "Query FM radio threshold", CLI_GUEST},
    {"vol",        "vol",   _AudCmdFMRadioSetFineVolume,   NULL, "Fine tune volume", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudIoCmdTbl[] =
{
    {"spdif",       NULL,   NULL, _arAudSpdifCmdTbl,    "spdif command", CLI_GUEST },
    {"adac",        NULL,   NULL, _arAudAdacCmdTbl,     "adac command", CLI_GUEST },
    {"interdac",    NULL,   NULL, _arAudInterDacCmdTbl, "inter dac command", CLI_GUEST },
    {"cfg",         NULL,   NULL, _arAudCfgCmdTbl,      "configuration", CLI_GUEST },
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudNvmCmdTbl[] =
{
#ifdef CC_AUD_USE_NVM
    {"Read",       "r",     _AudCmdNvmRead,  NULL,  "Read  NVM", CLI_GUEST},
    {"Wrtie",      "w",     _AudCmdNvmWrite, NULL,  "Write NVM",  CLI_GUEST},
    {"Initialize", "init",  _AudCmdNvmInit,  NULL,  "Initialize NVM", CLI_GUEST},
    {"ProfileRead", "pfr",  _AudCmdProfileRead, NULL, "Profile Read", CLI_GUEST},
#endif
#ifdef CC_AUD_USE_FLASH_AQ
    {"AQ write", "w",	_AudCmdFlashAQWrite, NULL,	"AQ Write", CLI_GUEST },
    {"AQ read",  "r",	_AudCmdFlashAQRead, NULL,	"AQ Read", CLI_GUEST },
    {"AQ version", "v",	_AudCmdFlashAQGetVersion, NULL, "AQ version", CLI_GUEST },
    {"AQ version cmp", "vc", _AudCmdFlashAQComVersion, NULL, "AQ version compare", CLI_GUEST },
    {"AQ set default", "df", _AudCmdFlashAQSetDefault  , NULL, "AQ put default to flash", CLI_GUEST },
    {"AQ list all", "l", _AudCmdFlashAQListAll  , NULL, "AQ list all data on flash", CLI_GUEST },
    {"AQ fill to dram", "fill", _AudCmdFlashAQFillDataStruct  , NULL, "AQ fill data from flash to data structure", CLI_GUEST },
#endif //CC_AUD_USE_FLASH_AQ
#ifdef SUPPORT_FLASH_AQ
    {"AQ read extension", "rx", _AudCmdFlashAQReadEx, NULL, "AQ Read extension", CLI_GUEST},
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudAsrcCmdTbl[] =
{
    {"query ASRC",     "q",    _AudCmdAsrcQuery, NULL, "Query ASRC status", CLI_SUPERVISOR},
    {"track mode",     "t",    _AudCmdAsrcTrackingMode, NULL, "Enable ASRC Tracking Mode", CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
static CLI_EXEC_T _arAudOssCmdTbl[] =
{
	{"Oss_src",  "s",    _AudOssAdap_SetSrc, NULL,  "Oss Adaptation set src", CLI_SUPERVISOR},
	{"Oss_cmd",  "cmd",    _AudOssAdap_Cmd, NULL,  "Oss Adaptation command", CLI_SUPERVISOR},
	{"Oss_Ops",  "e",    _AudOssAdap_Ops, NULL,  "Oss Adaptation Operations", CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif


#ifdef CC_AUD_MIXSOUND_SUPPORT
static CLI_EXEC_T _arAudMixSndCmdTbl[] =
{
    //CC_AUD_MIXSND_CLI1
    {"mixsnd volume", "v",    _AudCmdMixSndVol,  NULL, "Mixsound Volume control", CLI_GUEST},
    {"mixsnd loop",   "l",    _AudCmdMixSndLoop, NULL, "Mixsound Loop control", CLI_GUEST},
    {"mixsnd query",  "q",    _AudCmdMixSndQuery,NULL, "Mixsound status query", CLI_GUEST},
    {"mixsnd flag",     "flag", _AudCmdMixSndFlag, NULL, "Mixsound flag", CLI_GUEST},
    {"mixsnd play",     "p",    _AudCmdMixSndPlay, NULL, "Mixsound playback", CLI_GUEST},
    {"mixsnd source",   "str",  _AudCmdMixSndClip, NULL, "Select clip source", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
static CLI_EXEC_T _arAudMixSndClipCmdTbl[] =
{
    {"init",   "init",    _AudCmdInitMixSndClip,  NULL,  "Audio Mixsound Clip Init", CLI_SUPERVISOR},
    {"set",    "set",     _AudCmdSetMixSndClip,   NULL,  "Audio Mixsound Clip Set", CLI_SUPERVISOR},
    {"play",   "play",    _AudCmdPlayMixSndClip,   NULL,  "Audio Mixsound Clip Play", CLI_SUPERVISOR},
    {"stop",   "stop",    _AudCmdStopMixSndClip,   NULL,  "Audio Mixsound Clip Stop", CLI_SUPERVISOR},
    {"pause",  "pause",   _AudCmdPauseMixSndClip,   NULL,  "Audio Mixsound Clip Pause", CLI_SUPERVISOR},
    {"resume", "resume",  _AudCmdResumeMixSndClip,   NULL,  "Audio Mixsound Clip Resume", CLI_SUPERVISOR},
    {"repeat", "repeat",  _AudCmdRepeatMixSndClip,   NULL,  "Audio Mixsound Clip Repeat", CLI_SUPERVISOR},
    {"q",      "q",       _AudCmdQueryMixSndClip, NULL, "Audio Mixsound Clip Query", CLI_SUPERVISOR},
    {"qdsp",   "qdsp",    _AudCmdQueryMixSndClipDsp, NULL, "Audio Mixsound Clip Query(DSP)", CLI_SUPERVISOR},
    {"test",   "test",    _AudCmdTestMixSndClip, NULL, "Audio Mixsound Clip Test", CLI_SUPERVISOR},
    #ifdef MIXSOUND_MIPS_MEASURE
    {"mq",     "mq",      _AudCmdQueryMixSndClipMIPS, NULL, "Audio MixSound Clip Query(MIPS)", CLI_SUPERVISOR},
    {"mr",     "mr",      _AudCmdResetMixSndClipMIPS, NULL, "Audio MixSound Clip Reset(MIPS)", CLI_SUPERVISOR},
    #endif
    {"clr",    "clr",     _AudCmdClearMixSndClip, NULL, "Audio MixSound Clip Clear", CLI_SUPERVISOR},
    {"m",      "m",       _AudCmdMuteMixSndClip,  NULL, "Audio MixSound Clip Mute", CLI_SUPERVISOR},
    #ifdef MIXSND_OUTPUT_DBG
    {"mda",  "mda",	_AudCmdMixSndDumpAlloc, NULL,	"Allocate memory for MixSnd Dump", CLI_GUEST },
    {"mdr",  "mdr", _AudCmdMixSndDumpReset, NULL, "Reset MixSnd Dump", CLI_GUEST },
    {"mds",  "mds", _AudCmdMixSndDumpSet, NULL, "Set MixSnd Dump Location", CLI_GUEST},
    #endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif
#endif

static CLI_EXEC_T _arAudDramCmdTbl[] =
{
    {"qm",  "qm",   _AudCmdQueryDspMemory,   NULL, "Query AUD Dram Map", CLI_GUEST},
    {"dm",  "dm",   _AudCmdDumpDspMemory,    NULL, "dump dsp memory", CLI_GUEST},
    {"r",   "r",    _AudCmdReadBytePhysical, NULL, "Read physical memory in bytes", CLI_GUEST},
    {"r4",  "r4",   _AudCmdReadDwrdPhysical, NULL, "Read physical memory in dwrds", CLI_GUEST},
    {"phy", "phy",  _AudCmdGetPhyAddr,       NULL, "Virtual to Physical convert", CLI_GUEST},
    {"vir", "vir",  _AudCmdGetVirAddr,       NULL, "Physical to Virtual convert", CLI_GUEST},
    {"sh",  "sh",   _AdspDSPShareInfo,       NULL, "read dsp share memory: sh [group]", CLI_GUEST},
    {"shw", "shw",  _AdspDSPSharInfoWrite,   NULL, "write dsp share memory: sh [group] [addr] [val] [size]", CLI_GUEST},
    {"cm",  "cm",   _AdspDSPCommonDram,      NULL, "read dsp common memory: cm [addr] [length]", CLI_SUPERVISOR},
    {"cmw", "cmw",  _AdspDSPCommonDramWrite, NULL, "write dsp common memory: cm [addr] [value]", CLI_SUPERVISOR},
    {"norm","norm", _AdspDSPNormDram,        NULL, "read dsp normal dram", CLI_SUPERVISOR},
    {"cmpt","cmpt", _AdspDSPCmptDram,        NULL, "read dsp compact dram", CLI_SUPERVISOR},
    {"chldump","c", _AdspDSPChlDump,         NULL, "dump channel ouput data", CLI_GUEST},
    {"flush", "flush", _AudCmdFlushDspMemory, NULL, "Flush DSP memory", CLI_GUEST},
	#ifdef __KERNEL__
	{"dump", "dump", _AudCmdDumpFifo2File, NULL, "dump Fifo data to file", CLI_GUEST},
    {"ul", "ul", _AudCmdUlFile2Dram, NULL, "Upload file data to dram", CLI_GUEST},
    {"mal", "mal", _AudCmdDramAlloc, NULL, "Allocate Dram", CLI_GUEST},
	#endif
#ifdef LINUX_TURNKEY_SOLUTION
    {"dumpLoop", "dumpLoop", _AudCmdDumpFifo2FileInfinit, NULL, "dump Fifo data to file infinitely", CLI_GUEST},    
    {"dumpStop", "dumpStop", _AudCmdDumpFifo2FileInfinitStop, NULL, "stop dumpLoop", CLI_GUEST},        
    {"dumpAutoSave", "dumpAS", _AudCmdDumpFifo2FileAutoSave, NULL, "Set dumpLoop auto save to a new file", CLI_GUEST}, 
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#if 0 //def LINUX_TURNKEY_SOLUTION
//CC_AUD_SUPPORT_OPENMAX
static CLI_EXEC_T _arAudOmxCmdTbl[] =
{
    {"bc",  "bc",   _AudCmdOmxBypassChVol,   NULL, "Bypass Ch Vol", CLI_GUEST},
    {"lpcm", "lpcm", _AudCmdOmxLPCMSetting,  NULL, "LPCM Setting",  CLI_GUEST},
    {"wma", "wma", _AudCmdOmxWMASetting, NULL, "WMA Setting", CLI_GUEST},
    {"wma10", "wma10", _AudCmdOmxWMAProSetting, NULL, "WMA10 Setting", CLI_GUEST},    
    {"cook", "cook", _AudCmdOmxCOOKSetting, NULL, "Cook Setting", CLI_GUEST},
    {"dts",  "dts",  _AudCmdOmxDTSSetting, NULL, "DTS Setting", CLI_GUEST},
    {"delay", "delay", _AudCmdOmxDelaySetting, NULL, "Delay Setting", CLI_GUEST},
    {"bypass", "bypass", _AudCmdOmxSettingBypass, NULL, "Bypass OMX Setting", CLI_GUEST},
    {"dm","dm",_AudCmdOmxSetDbgMsk,NULL,"Set Debug Mask", CLI_GUEST},
    {"q", "q", _AudCmdOmxQuery, NULL,"Query", CLI_GUEST},
    {"pts", "pts", _AudCmdOmxPtsOffset, NULL, "Set PTS Offset", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
//CC_AUD_SUPPORT_OPENMAX
#endif

static CLI_EXEC_T _arAudAVSyncCmdTbl[] =
{
    {"debug",  "d",   _AudCmdAvSyncDbgLvl,   NULL, "Set AV Sync Debug On/Off", CLI_GUEST},
    {"stcid",  "sid", _AudCmdAvSyncSetStcID,   NULL, "Set Audio STC ID", CLI_GUEST},        
#if 0 // 2_STC_Task_DEBUG_only    
    {"stc",    "s",   _AudCmdAvSyncReadStc,  NULL, "Read all STC", CLI_GUEST},            
#endif    
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};


/******************************************************************************
* Variable      : cli "aud" command table
******************************************************************************/

static CLI_EXEC_T _arAudCmdTbl[] =
{
    {"init",    "i",    _AudCmdInit, NULL,          "Aud init", CLI_SUPERVISOR},
    CLIMOD_DEBUG_CLIENTRY(AUD),
    //CLIMOD_REGTST_CLIENTRY(AUD),
    {"setdec",  "s",    _AudCmdSetDecType,   NULL,  "Aud set decoding type", CLI_SUPERVISOR},
    {"resume",  NULL,   _AudCmdResume,       NULL,  "Aud resume", CLI_SUPERVISOR},
    {"pause",   NULL,   _AudCmdPause,        NULL,  "Aud pause", CLI_SUPERVISOR},
    {"play",    NULL,   _AudCmdPlay,         NULL,  "Aud play", CLI_SUPERVISOR},
    {"stop",    NULL,   _AudCmdStop,         NULL,  "Aud stop", CLI_SUPERVISOR},
    {"dual",    "dual", _AudCmdDualDecMode, NULL,  "dual", CLI_SUPERVISOR},
#ifdef CC_MT5391_AUD_3_DECODER
    {"tri",     "tridec", _AudCmdTriDecMode, NULL,  "tri_dec", CLI_SUPERVISOR},
#endif
    {"sync",    NULL,   _AudCmdSyncMode,     NULL,  "Aud sync mode setting", CLI_SUPERVISOR},
    {"stream",  "str",  _AudCmdSetStream,    NULL,  "Playback stream src", CLI_SUPERVISOR},
    {"query",   "q",    _AudCmdQuery,        NULL,  "Audio status query", CLI_SUPERVISOR},
	#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
	{"oss", 	NULL,	NULL,  _arAudOssCmdTbl,  "OSS adaption layer command", CLI_GUEST},
	#endif
#ifdef CC_AUD_MIXSOUND_SUPPORT
    {"m",       NULL,   NULL, _arAudMixSndCmdTbl,   "Audio mixsound playback", CLI_GUEST},
#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
    {"mclip",   NULL,   NULL, _arAudMixSndClipCmdTbl, "Audio mixsound clip playback", CLI_SUPERVISOR},
#endif
    {"pll1",    NULL,   _AudCmdPll1Set,         NULL,  "Aud Pll1 setting", CLI_SUPERVISOR},
    {"pll2",    NULL,   _AudCmdPll2Set,         NULL,  "Aud Pll2 setting", CLI_SUPERVISOR},
    {"pllpd",   NULL,   _AudCmdPllPowerDown,    NULL,  "Aud Pll PoweDown", CLI_SUPERVISOR},
#endif
    {"dsp",     NULL,   NULL, _arAudDspCmdTbl,      "dsp command", CLI_GUEST },
    {"uop",     NULL,   NULL, _arAudUopCmdTbl,      "audio uop", CLI_GUEST },
    {"t",       NULL,   NULL, _arAudTestCmdTbl,     "test", CLI_GUEST },
    {"Clip",    NULL,   NULL, _arAudClipCmdTbl,     "Aud Clip", CLI_GUEST },
#if 0 //def __MODEL_slt__
    {"dspslt",  NULL,   NULL, _arAudDSPSLTCmdTbl,   "DSP SLT", CLI_SUPERVISOR },
#endif
#if defined(__linux__)
    {"isplay",  "ip",   _AudCmdIsDecoderPlay, NULL,     "Check if audio decoder is playing", CLI_SUPERVISOR},
#endif
    {"atv",     NULL,  NULL,  _arAudAtvCmdTbl,  "ATV command", CLI_SUPERVISOR},
    {"dtv",     NULL,  NULL,  _arAudDtvCmdTbl,  "DTV command", CLI_SUPERVISOR},
    {"fmrdo",   NULL, NULL, _arAudFMRadioCmdTbl, "FM Radio command", CLI_SUPERVISOR},
    {"io",      NULL,  NULL,  _arAudIoCmdTbl,  "I/O command", CLI_GUEST},
    {"nvm",     "nvm",   NULL, _arAudNvmCmdTbl,  "Nvm Read/Wrtie", CLI_GUEST },
    {"src",     NULL,   NULL, _arAudAsrcCmdTbl,  "ASRC status query", CLI_SUPERVISOR},
    {"dram",    NULL,   NULL, _arAudDramCmdTbl,  "DRAM command", CLI_GUEST},
#if 0 //def LINUX_TURNKEY_SOLUTION    
    {"omx",     NULL,   NULL, _arAudOmxCmdTbl,   "OMX command", CLI_GUEST}, //CC_AUD_SUPPORT_OPENMAX
#endif    
    {"syncdbg", "sd",   NULL, _arAudAVSyncCmdTbl,  "AV Sync Debug command", CLI_GUEST},
    {"dolby",     "db",   NULL, _arAudDolbyCmdTbl,      "Dolby related command", CLI_GUEST },
    {NULL,      NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

CLI_MAIN_COMMAND_ITEM(Aud)
{
    "aud",
    NULL,
    NULL,
    _arAudCmdTbl,
    "Aud command",
    CLI_GUEST
};

#if (0)
#ifdef __MW_CLI_DEF__
CLI_EXEC_T* GetAudMwCmdTbl(void)
#else
CLI_EXEC_T* GetAudCmdTbl(void)
#endif
{
    return &_rAudModCmdTbl;
}
#endif

