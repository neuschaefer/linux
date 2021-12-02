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
//#include "drv_dsp_cfg.h"
#include "sif_if.h"
#include "x_assert.h"
#ifndef __KERNEL__
    #include "../5301a_driver/aud/adsp/include_API/dsp_common.h"
#else // __KERNEL__
	#include "dsp_common.h"
#endif // __KERNEL__

//#include "x_aud_dec.h"



//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
extern void AUD_OutPadEnable(UINT8 u1DecId, BOOL fgEnable);
extern void SPDIF_InInit(void) ;
extern BOOL SPDIF_InLockCheck(void) ;
extern void SPDIF_InRegisterDump(void) ;
#ifdef CC_MT5391_AUD_SUPPORT
extern void _MT8297_Init(void);
extern void _MT8297_Mute(BOOL fgEnable);
extern void _MT8297_DacChlSel(UINT8 u1DacId,AUD_CHL_DEF_T eChl);
extern void _MT8297_InterDacPGAGain(UINT8 u1DacId,UINT8 u1Volume);
extern void _MT8297_InterHPPGAGain(UINT8 u1Volume);
#else
extern void _MT536xInternal_Init(void);
extern void _MT536xInternal_DacChlSel(UINT8 u1DacId,AUD_CHL_DEF_T eChl);
extern void _MT536xInternal_Mute(UINT8 u1DacId,BOOL fgEnable);
#endif

extern void _MT8292_SetPGAGain(UINT8 u1DacId, UINT8 u1Data);
extern void _MT8292_SetHpGain(UINT8 u1Data);

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
extern UINT32 u4AUD_DspProbeStopOnWrite(UINT32 u4Addr);
extern UINT32 u4AUD_DspProbeChkStopOnWrite(BOOL *fgEnable);

extern void AUD_AudioPTSDelayEnable(UINT8 u1DecId, UINT8 uDlyFactor);
extern void AUD_AoutPADMux(AUD_CH_T ePAD_SDATA0, AUD_CH_T ePAD_SDATA1, AUD_CH_T ePAD_SDATA2,
                    AUD_CH_T ePAD_SDATA3, AUD_CH_T ePAD_SDATA4, AUD_CH_T ePAD_SDATA5,
                    UINT8 PAD_MCLK0);
extern void vWriteDspSram(UINT32 u4Addr,UINT32 u4Value);
extern UINT32 u4ReadDspSram(UINT32 u4Addr);
extern void vADSPTaskInit(void);
//extern void AUD_GetAudioPes(UCHAR ucDecId, PSR_AUDIO_PES_T * prAudioPes);
extern AV_SYNC_MODE_T AUD_GetAvSynMode(UINT8 u1DecId);
extern void AUD_SetAvSynMode(UCHAR ucDecId, AV_SYNC_MODE_T eSynMode);
//extern void vDspIECConfig2 (BOOL fgFirstDecoder, UINT16 u2Flag);
extern void AUD_AoutInvertData(UINT8 ucDecId, BOOL fgInvert);
extern void AUD_AoutFormat(UINT8 ucDecId, DATA_FORMAT_T eDataFormat);
extern void DSP_ShareInforDump(UINT8 u1Group);
//Light added for AVC
extern void DSP_ShareInforWriteWord(UINT32 ADDR, UINT16 VALUE);

extern void DSP_CommDramDump(UINT32 u4Address, UINT32 u4Len);
extern void AudShowDspStatus(void);
extern void AudShowMtsStatus(UINT8 u1DecId);
extern void AudShowStatus(UINT8 u1DecId);
extern void DSP_SetAudLogPeriod(UINT32 u4Period);
extern BOOL fgDspReadPtrSet (UCHAR ucDecId, UINT32 u4Address);
extern void AUD_AoutDacFs(UINT8 u1DecId, SAMPLE_FREQ_T eFs);
extern void SPDIF_InChSel(UINT8 u1Ch);
extern void AUD_AoutDrvCur(UINT8 u1DecId, UINT8 u1MckDrvCur, UINT8 u1BckDrvCur ,UINT8 u1LRckDrvCur , UINT8 u1AOSDATADrvCur);
extern void AUD_StopMemStr(void);
extern void AUD_ChlOutDump(UINT8 u1Chl);
extern void AUD_DspProcMode(UINT8 u1DecId, UINT16 u2Mode);
//extern void _ADC_WM8776_DacVolCtl(INT16 i2Value);
//extern void _ADC_WM8776Write(UINT8 u1Addr, UINT8 u1Data);
//xtern void AUD_AVMuxSel(UINT8 u1Sel);
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
extern UINT32 AUD_GetDspCacheHitRate(void);
extern void AUD_DumpUopCommand(void);
extern INT32 AUD_Read32ByteID (UINT8 *u1ID_p, UINT32 u4Num);
extern BOOL _AudFMRadioDetection(void);

#ifdef DSP_SUPPORT_SRSTSHD //sunman_tshd
extern void DSP_SRSTSHDSurrLvl(UINT8 u1Mode);
extern void DSP_SRSTSHDInpg(UINT8 u1Mode);
extern void DSP_SRSTSHDOutg(UINT8 u1Mode);
extern void DSP_SRSTSHDSetSpkSize(UINT8 u1Mode);
extern void DSP_SRSTSHDSetTSCtrl(void);
extern void DSP_SRSTSHDTBLvl(UINT8 u1Mode);
extern void DSP_SRSTSHDDef(UINT8 u1Mode);
extern void DSP_SRSTSHDElev(UINT8 u1Mode);
extern void DSP_SRSTSHDMode(UINT32 u4Mode);
#endif

// FIXME
//extern BOOL fgDrvTunerSetSystem(UINT8 bSubSysIdx);

#ifdef __MODEL_slt__
//Rice add for ADSP SLT
extern void ADSP_SLT_Init(void);
extern void ADSP_SLT_SetPatID(UINT8 u1PID);
extern void SLT_FlashingAudInit(void);

#endif

#ifdef BURN_IN_AOUT_TEST
extern UINT32 AudGetAoutStatus(void);
extern void AudResetAoutStatus(void);
extern void AudSetAoutTest(void);
#endif 

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
    UINT8 ucDecId;
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
        ucDecId = (UINT8)StrToInt(szArgv[1]);
        ucStreamFrom =  (UINT8)StrToInt(szArgv[2]);
        ucCodeType = (UINT8)StrToInt(szArgv[3]);
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
        if (ucDecId > 1)
#else        
        if (ucDecId > 2) // 0,1,2 is allowed in mt5391
#endif
        {
            goto lbUsage; 
        }
        // setup decode format
        eAudDecFormat = (AUD_FMT_T)ucCodeType;
        // stream from
        eAudDecStreamFrom = (AUD_DEC_STREAM_FROM_T)ucStreamFrom;
        AUD_SetDecType(ucDecId, eAudDecStreamFrom, eAudDecFormat);
    }
    else
    {
        goto lbUsage;
    }       
    return 1;

lbUsage:
    Printf("Usage: setdec {[decoder id]} [stream from] [stream type]\n");
#ifndef CC_MT5391_AUD_3_DECODER 
//#if 1
    Printf("[decoder id]    0:MAIN  1:AUX \n");
#else
    Printf("[decoder id]    0:MAIN  1:AUX 2:THIRD\n");    
#endif    
    Printf("[stream from] 1:D-TUNER  2:A-TUNER 3:SPDIF 4.LINE-IN 5.HDMI 6.MEMORY (ex.Pink noise) 7:BUFFER AGENT 8:INTERNAL 9:MULTI MEDIA\n");
    Printf("[stream type] 0:INTERNAL (ex.Pink noise) 1:MPEG  2:AC3  3:PCM  4:MP3  5:AAC  \n");
    Printf("              7:WMA  11:MTS  12:CANAL+  13:PAL  14:A2  18:DETECTOR  20:LPCM  \n");
    Printf("              21 : FMRDO, 22:FMRDODET, 23:SBC, 24:JPEG \n");
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

static INT32 _AudCmdDualDecSetDecType(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 ucCodeType;
    UINT8 ucStreamFrom;
    AUD_FMT_T   eAudDecFormat;
    AUD_DEC_STREAM_FROM_T eAudDecStreamFrom;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    ucStreamFrom =  (UINT8)StrToInt(szArgv[1]);
    ucCodeType = (UINT8)StrToInt(szArgv[2]);
   
    // setup decode format
    eAudDecFormat = (AUD_FMT_T)ucCodeType;

    // stream from
    eAudDecStreamFrom = (AUD_DEC_STREAM_FROM_T)ucStreamFrom;

    AUD_DSPCmdDualDecSetDecType(eAudDecStreamFrom, eAudDecFormat);
    
    return 1;

lbUsage:
    Printf("Usage: ds [stream from] [stream type]\n");
    Printf("[stream from] 1:D-TUNER  2:A-TUNER 3:SPDIF 4.LINE-IN 5.HDMI 6.MEMORY (ex.Pink noise) 7:INTERNAL\n");
    Printf("[stream type] 0:INTERNAL (ex.Pink noise) 1:MPEG  2:AC3  3:PCM  4:MP3 \n");
    Printf("                     11:MTS  12:A2  13:PAL  14:FMFM  15:NICAM  17:DETECTOR\n");
    return -1;      
}

static INT32 _AudCmdDualDecPlay(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    AUD_DSPCmdDualDecPlay(TRUE);
    return 1;
}

static INT32 _AudCmdDualDecStop(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    AUD_DSPCmdDualDecStop(TRUE);
    return 1;
}

static INT32 _AudCmdResume(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;

    if(i4Argc == 1)
    {
        AUD_DSPCmdResume(AUD_DEC_MAIN);
    }
    else if (i4Argc == 2)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
        if (u1DecId > 1)
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
    Printf("[decoder id] 0:MAIN  1:AUX \n");
    return -1;     
}

static INT32 _AudCmdPause(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;

    if(i4Argc == 1)
    {
        AUD_DSPCmdPause(AUD_DEC_MAIN);
    }
    else if (i4Argc == 2)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
        if (u1DecId > 1)
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
    Printf("[decoder id] 0:MAIN  1:AUX \n");
    return -1;     
}

static INT32 _AudCmdPlay(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;

    if(i4Argc == 1)
    {
        AUD_DSPCmdPlay(AUD_DEC_MAIN);
    }
    else if (i4Argc == 2)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
#ifndef CC_MT5391_AUD_3_DECODER 
//#if 1
        if (u1DecId > 1)
#else
        if (u1DecId > 2)
#endif
        {
            goto lbUsage; 
        }
        AUD_DSPCmdPlay(u1DecId);
#ifdef CC_MT5391_AUD_3_DECODER  //gallen 0521        
        if (u1DecId==AUD_DEC_THIRD) {
            AUD_DspADEnable(AUD_DEC_MAIN, TRUE);
        }    
#endif            
    }    
    else 
        goto lbUsage; 
    
    return 1;

lbUsage:
    Printf("Usage: play [decoder id]\n");
#ifndef CC_MT5391_AUD_3_DECODER     
//#if 1
    Printf("[decoder id] 0:MAIN  1:AUX \n");
#else
    Printf("[decoder id] 0:MAIN  1:AUX  2:THIRD \n");
#endif
    return -1;     
}

static INT32 _AudCmdStop(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;

    if(i4Argc == 1)
    {
        AUD_DSPCmdStop(AUD_DEC_MAIN);
    }
    else if (i4Argc == 2)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
#ifndef CC_MT5391_AUD_3_DECODER 
        if (u1DecId > 1)
#else
        if (u1DecId > 2)
#endif
        {
            goto lbUsage; 
        }
        AUD_DSPCmdStop(u1DecId);
#ifdef CC_MT5391_AUD_3_DECODER  //gallen 0521        
        if (u1DecId==AUD_DEC_THIRD) {
            AUD_DspADEnable(AUD_DEC_MAIN, FALSE);
        }            
#endif        
    }    
    else 
        goto lbUsage; 
    
    return 1;

lbUsage:
    Printf("Usage: stop [decoder id]\n");
#ifndef CC_MT5391_AUD_3_DECODER   
//#if 1
    Printf("[decoder id] 0:MAIN  1:AUX \n");
#else
    Printf("[decoder id] 0:MAIN  1:AUX  2:THIRD \n");
#endif
    return -1;     
}

static INT32 _AudCmpQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;

    if(i4Argc == 1)
    {
        u1DecId = AUD_DEC_MAIN;
    }
    else if (i4Argc == 2)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
        if (u1DecId > 1)
#else
        if (u1DecId > 2)
#endif
        {
            goto lbUsage; 
        }
    }    
    else 
        goto lbUsage;

    AudShowStatus(u1DecId);
    
    return 1;

lbUsage:
    Printf("Usage: query [decoder id]\n");
#ifndef CC_MT5391_AUD_3_DECODER  
//#if 1
    Printf("[decoder id] 0:MAIN  1:AUX \n");
#else
    Printf("[decoder id] 0:MAIN  1:AUX  2:THIRD \n");
#endif
    return -1;     
}

static INT32 _AudCmdSyncMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR ucDecId;
    UCHAR ucSyncMode;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    ucDecId =  (UINT8)StrToInt(szArgv[1]);

#ifndef CC_MT5391_AUD_3_DECODER 
//#if 1
    if (ucDecId > 1)
#else
        if (ucDecId > 2)
#endif
    {
        goto lbUsage;
    }

    if (i4Argc == 2)
    {
        // Get AV sync mode
        AV_SYNC_MODE_T eSyncMode;
        eSyncMode = AUD_GetAvSynMode(ucDecId);
        if (eSyncMode == AV_SYNC_FREE_RUN)
        {
            Printf("Dec %d: Free run\n", ucDecId);
        }
        else if (eSyncMode == AV_SYNC_SLAVE)
        {
            Printf("Dec %d: AV sync\n", ucDecId);
        }
        else
        {
            Printf("Dec %d: Audio master\n", ucDecId);
        }
    }
    else
    {
        // Set AV sync mode
        ucSyncMode = (UINT8)StrToInt(szArgv[2]);
        AUD_SetAvSynMode(ucDecId, (AV_SYNC_MODE_T)ucSyncMode);
    }
    
    return 1;

lbUsage:
    Printf("Usage: sync [decoder id] [sync mode]\n");
#ifndef CC_MT5391_AUD_3_DECODER 
//#if 1
    Printf("[decoder id] 0:MAIN  1:AUX \n");
#else
    Printf("[decoder id] 0:MAIN  1:AUX  2:THIRD \n");
#endif
    Printf("[sync mode]  %d:FREE RUN %d:SLAVE MODE %d:AUDIO MASTER\n", 
        (UINT8)AV_SYNC_FREE_RUN, (UINT8)AV_SYNC_SLAVE, (UINT8)AV_SYNC_AUDIO_MASTER);
    return -1;      
}

static INT32 _AudCmdPts(INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR ucDecId;
    DMX_AUDIO_PES_T rAudioPes;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    x_memset(&rAudioPes, 0, sizeof(DMX_AUDIO_PES_T));
    ucDecId =  (UINT8)StrToInt(szArgv[1]);
    //AUD_GetAudioPes(ucDecId, &rAudioPes);

    Printf( "dev: %u, PTS: 0x%08x, DTS: 0x%08x, Payload: 0x%08x, size: %u\n",
        ucDecId, (UINT32)(rAudioPes.u4Pts & 0xffffffff),
        (UINT32)(rAudioPes.u4Dts & 0xffffffff), rAudioPes.u4Wp, 0/*,
        rAudioPes.u2PayloadSize*/);
    
    return 1;

lbUsage:
    Printf("Usage: pts [decoder id]\n");
    Printf("[decoder id] 0:MAIN  1:AUX \n");
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

static INT32 _SpdifIecChannelSet(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1IecChannel;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }
  
    u1IecChannel = (UINT8)StrToInt(szArgv[1]);
  
    UNUSED(AUD_DspIecChannel((AUD_IEC_CH_T)u1IecChannel));
    
    return 1;

lbUsage:
    Printf("Usage: iec [iec mode]\n");
    Printf("[channel]    0:L_R  1:LS_RS 2:C_SW 3:7_8 4:LINE_IN 5:9_10 6:AUX\n");
    return -1;
}


    
/****************************************************************************
** adac Command
****************************************************************************/

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
        Printf("Usage: ch [DAC id] [channel] \n");
        Printf("[DAC id]  0:FIRST  1:SECOND  2: THIRD \n");
        Printf("[channel #]      0:L/R,     1:LS/RS,   2:C/Subwoofer\n");
        Printf("                 3:bypass, 4:downmix,  5:AUX\n");
		return -1;    
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u4Channel = (AUD_CH_T)StrToInt(szArgv[2]);
   
#ifdef CC_MT5391_AUD_SUPPORT
    _MT8297_DacChlSel(u1DecId, (AUD_CHL_DEF_T)u4Channel);
#else
    _MT536xInternal_DacChlSel(u1DecId, (AUD_CHL_DEF_T)u4Channel);
#endif
    return 1;
}

static INT32 _InterDacMute(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DacId;
    BOOL fMute;
    
    if (i4Argc < 3)
    {
        Printf("Usage: mute [DAC id] [flag]\n");
        Printf("[DAC id]    0:FIRST  1:SECOND  2:THIRD  3:All\n");
        Printf("[flag]      1 : mute internal dac0, dac1 and dac2\n");
        Printf("            0 : unmute internal dac0, and dac1 and dac2\n");
    }
    
    u1DacId = StrToInt(szArgv[1]);
    fMute = (StrToInt(szArgv[2]) == 1) ? TRUE : FALSE;
#ifdef CC_MT5391_AUD_SUPPORT
    _MT8297_Mute(u1DacId, fMute);
#else
    _MT536xInternal_Mute(u1DacId, fMute);
#endif
    return 1;
}

static INT32 _InterDacEnable(INT32 i4Argc, const CHAR ** szArgv)
{
    //BOOL fEnable;
    
	if (i4Argc < 2)
	{
	    Printf("Usage: enable [flag]\n");
	    Printf("[flag]      1 : enable internal dac\n");
	    Printf("            0 : disable internal dac\n");
	}
    
    //fEnable = (StrToInt(szArgv[1]) == 1) ? TRUE : FALSE;
	//Laihui: remove it temp
	//_ADAC_InterDacEnable(fEnable);
	Printf("Internal DAC already enable.\n");
	return 1;
}

static INT32 _InterDacCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
    
#ifdef CC_MT5391_AUD_SUPPORT
    _MT8297_Init();
#else
    _MT536xInternal_Init();
#endif
    return 1;
}

#ifdef CC_MT5391_AUD_SUPPORT
static INT32 _InterDacPGAGain(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Vol;
    UINT16 u1DACId;

    if (i4Argc < 3)
    {
        Printf("Usage: pg [DAC id] [value] \n");
        Printf("[DAC id]  0:FIRST  1:SECOND  2: THIRD \n");
        Printf("[value] 0x00  mute \n");
        Printf("        0x01 -36dB \n");
        Printf("        0x02 -32dB \n");
        Printf("        0x03 -28dB \n");
        Printf("        0x04 -24dB \n");
        Printf("         ...   ... \n");
        Printf("        0x0B -10dB \n");
        Printf("        0x0C  -8dB \n");
        Printf("        0x0D  -7dB \n");
        Printf("        0x0E  -6dB \n");
        Printf("        0x0F  -5dB \n");
        Printf("         ...   ... \n");
        Printf("        0x14   0dB \n");
        Printf("        0x15   1dB \n");
        Printf("        0x16   2dB \n");
        Printf("         ...   ... \n");
        Printf("        0x23  15dB \n");

		return -1;    
    }

    u1DACId = (UINT8)StrToInt(szArgv[1]); 
    u1Vol  =  (INT16)StrToInt(szArgv[2]);

    _MT8297_InterDacPGAGain(u1DACId, u1Vol);
    return 1;
}

static INT32 _InterHeadphonePGAGain(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Vol;
    //UINT16 u1DACId;

    if (i4Argc < 2)
    {
        Printf("Usage: hpg [value] \n");
        Printf("[value] 0x00  mute \n");
        Printf("        0x01 -36dB \n");
        Printf("        0x02 -32dB \n");
        Printf("        0x03 -28dB \n");
        Printf("        0x04 -24dB \n");
        Printf("         ...   ... \n");
        Printf("        0x0B -10dB \n");
        Printf("        0x0C  -8dB \n");
        Printf("        0x0D  -7dB \n");
        Printf("        0x0E  -6dB \n");
        Printf("        0x0F  -5dB \n");
        Printf("         ...   ... \n");
        Printf("        0x14   0dB \n");
        Printf("        0x15   1dB \n");
        Printf("        0x16   2dB \n");
        Printf("         ...   ... \n");
        Printf("        0x2C  24dB \n");

		return -1;    
    }

    u1Vol  =  (INT16)StrToInt(szArgv[1]);

    _MT8297_InterHPPGAGain(u1Vol);
   
    return 1;
}
#endif


#ifdef __MODEL_slt__
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
    UINT8 u1GroupId;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u1GroupId = StrToInt(szArgv[1]);
    DSP_ShareInforDump(u1GroupId);
    
    return 1;   

    lbUsage:
    Printf("Usage: sh [group id]\n");
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

static INT32 _AdspCmdPtr(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4RpDec1, u4WpDec1;
    UINT32 u4RpDec2, u4WpDec2;
    UINT32 u4DataSZDec1, u4DataSZDec2;
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
    UINT32 u4RpDec3, u4WpDec3;
    UINT32 u4DataSZDec3;
#endif
    
    // Get read/write pointer
    AUD_GetRWPtr(AUD_DEC_MAIN, &u4RpDec1, &u4WpDec1, &u4DataSZDec1);
    AUD_GetRWPtr(AUD_DEC_AUX, &u4RpDec2, &u4WpDec2, &u4DataSZDec2);
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
    AUD_GetRWPtr(AUD_DEC_THIRD, &u4RpDec3, &u4WpDec3, &u4DataSZDec3);
#endif
    
    Printf("Decoder 1 rp,wp,size = [0x%08x, 0x%08x, %u]\n",u4RpDec1,u4WpDec1,u4DataSZDec1);
    Printf("Decoder 2 rp,wp,size = [0x%08x, 0x%08x, %u]\n",u4RpDec2,u4WpDec2,u4DataSZDec2);
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
    Printf("Decoder 3 rp,wp,size = [0x%08x, 0x%08x, %u]\n",u4RpDec3,u4WpDec3,u4DataSZDec3);
#endif    
    return 1;
}

static INT32 _AdspReadSram(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Addr, u4Count = 1, i = 0;
 //   UINT8  ucData;

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
            Printf("\nSRAM (0x%04x) | 0x%06x ", u4Addr, u4ReadDspSram(u4Addr));
        }
        else
        {
            Printf("0x%06x ", u4ReadDspSram(u4Addr));
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
    
    vWriteDspSram(u4Addr, u4Data);

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

//sunman_aout
#ifdef BURN_IN_AOUT_TEST
static UINT8 _gu8IsAoutSet = 0;

static INT32 _AdspAoutQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
    
    UINT32 u4Result;

    if(_gu8IsAoutSet==0){
        Printf("Please run aud.dsp.saout first !\n");
        return -1;
    }    
    //AudShowDspStatus();
    u4Result = AudGetAoutStatus();
    if(u4Result > 0)
        printf("Aout Err occurs!!\n Count =%d", u4Result);
    else
        printf("Aout OK\n");
    return 1;   
}

static INT32 _AdspAoutReset(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    if(_gu8IsAoutSet==0){
        Printf("Please run aud.dsp.saout first !\n");
        return -1;
    }    
    //AudShowDspStatus();
    AudResetAoutStatus();
    printf("Aout Reset OK\n" );
    return 1;   
}

static INT32 _AdspAoutSet(INT32 i4Argc, const CHAR ** szArgv)
{
    MEM_BUFFER_INFO_T  rMemBuf;
    AUD_PCM_SETTING_T  rPcmSetting;

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
    AUD_StrSelect(AUD_DEC_MAIN, 99, &rMemBuf);
    AUD_SetDecMemBuffer(AUD_DEC_MAIN, &rMemBuf);
    AUD_DSPCmdPlay(AUD_DEC_MAIN);

    x_thread_delay(100);
    _gu8IsAoutSet = 1;
    AudSetAoutTest();
    return 1;
}
#endif


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

static INT32 _AudCacheHitRate(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    Printf("DSP cache hit rate = %d (o/oo)\n", AUD_GetDspCacheHitRate());
    
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
        Printf("[channal #]     0  L   \n");
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
        Printf("Usage: bass [decoder id] [on/off] \n");
        Printf("[decoder id]     0:FIRST      1:SECOND\n");
        Printf("[on/off]         0:OFF        1:ON \n");
        return -1;    
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Enable = (UINT8)StrToInt(szArgv[2]);
   
    AUD_DspBassMngEnalbe(u1DecId, (BOOL)u1Enable);
    return 1;
}

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
        Printf("Usage: spkcfg [decoder id] [speaker config]\n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[speaker config] 32 bits                          \n");
        Printf(" bit 0 ~ 2:\n");
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
	 Printf("bit  19: Downmix CH9 Channel large(1)/small(0)\n");
        Printf("bit  20: Downmix CH10 Channel large(1)/small(0)\n");
	
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
        Printf("Usage: ls [decoder id] [FrontPair] [Center] [Surround]\n");
        Printf("[decoder id]   0:FIRST  1:SECOND \n");
        Printf("[FrontPair]    0:SMALL  1:LARGE \n");
        Printf("[Center]       0:SMALL  1:LARGE \n");
        Printf("[Surround]     0:SMALL  1:LARGE \n");
	 Printf("[CH9/10]     0:SMALL  1:LARGE \n");	
        return -1;
    }
    
    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1FrontPair = (UINT8)StrToInt(szArgv[2]);
    u1Center = (UINT8)StrToInt(szArgv[3]);
    u1Surround = (UINT8)StrToInt(szArgv[4]);
    u1CH910 = (UINT8)StrToInt(szArgv[5]);
    
//    CLI_Parser("aud.uop.spkuop.bs 0 1");  // turn on bass management
  
    AUD_DspSpeakerLargeSmallConfig(u1DecId, u1FrontPair, u1Center, u1Surround,u1CH910);

    return 1;
}

static INT32 _AudCmpUopCutFreq(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT32 u4Freq;
    
    if ((i4Argc != 3) && (i4Argc != 2))
    {
        Printf("Usage: cf [decoder id] ([cutoff freq])\n");
        Printf("[decoder id]   0:FIRST  1:SECOND \n");
        Printf("[cutoff freq]  set frequency in xxx Hz \n");
        Printf("               70/100/200/300  Hz         \n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc == 2)
    {        
        Printf("Cufoff Frequecy is %d\n", AUD_DspSpkSizeQry(u1DecId));
        return 1;
    }
            
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
        Printf("Usage: spkcfg [decoder id] [mode]\n");
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
        Printf("Usage: sw [decoder id] [mode]\n");
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
static INT32 _AudCmpUopChannelDelay(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u2ChanNum;
    UINT16 u2Delay;
    UINT16 u2DecId;
    if (i4Argc < 4 || (UINT16)StrToInt(szArgv[3]) > 25)
    {
        Printf("Usage: delaycfg [decoder id] [channel #] [channel delay]\n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[channel #]      0(L), 1(R), 2(LS), 3(RS), 4(C), 5(SUB), 6(Bypass L), 7(Bypass R)\n");
        Printf("8(Downmix L), 9(Downmix R), 10(AUD L), 11(AUD R), 13(All), 14(LFE)\n");
       // Printf("[channel delay]  0.05 m/unit\n");
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
static INT32 _AudCmpUopChannelDelay(INT32 i4Argc,const CHAR** szArgv)
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

static INT32 _AudCmpUopChVolume(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u1DecId;
    UINT32 u4Volume;
    AUD_CH_T eChannel;
    
    if (i4Argc < 4)
    {
        Printf("Usage: cv [decoder id] [channel] [volume] \n");
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
static INT32 _AudCmpUopLineOutVolOffset(INT32 i4Argc,const CHAR** szArgv)
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
static INT32 _AudCmpUopVolumeOffset(INT32 i4Argc,const CHAR** szArgv)
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
static INT32 _AudCmpUopChVolumeGain(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u1DecId;
    INT32  i4Gain;
    AUD_CH_T eChannel;
    
    if (i4Argc < 4)
    {
        Printf("Usage: ch volume gain [decoder id] [channel] [gain] \n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[channel #]      0:L, 1:R, 2:LS, 3:RS, 4:C, 5:SUBWOOFER, 6:ALL\n");
        Printf("[gain]           0 ~ 224,  every 0.5dB per step \n");
        printf("                    0:  -64   dB\n");
        printf("                    1:  -63.5 dB\n");
        printf("                    2:  -63   dB\n");
        printf("                     .        . \n");
        printf("                     .        . \n");
        printf("                     .        . \n");
        printf("                  128:    0   dB\n");
        printf("                  129:    0.5 dB\n");
        printf("                     .        . \n");
        printf("                     .        . \n");
	    printf("                  224:   48   dB\n");
		return -1;    
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    eChannel = (AUD_CH_T)StrToInt(szArgv[2]);
    i4Gain = (INT32)StrToInt(szArgv[3]);
   
    AUD_DspChannelVolGain(u1DecId, eChannel, (INT32) (i4Gain - 128));
    return 1;
}

#ifdef CC_AUD_LINEOUT_VOL_MODE
static INT32 _AudCmpUopLinOutVolume(INT32 i4Argc,const CHAR** szArgv)
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
    UINT32 u4Volume;
    AUD_DEC_STREAM_FROM_T eStrmSrc;
    
    if (i4Argc < 4)
    {
        Printf("Usage: sv [decoder id] [source] [value] \n");
#ifdef CC_MT5391_AUD_3_DECODER        
//#if 1
        Printf("[decoder id]    0:FIRST     1:SECOND \n");
#else
        Printf("[decoder id]    0:FIRST     1:SECOND     2:THIRD \n");
#endif
        Printf("[source #]      0:OTHERS, 1:DIGITAL, 2:ANALOG, 3:SPDIF \n");
        printf("                                      4:LINE_IN, 5:HDMI, 6:MEMORY \n");
        Printf("[volume]        0~164     every 0.5dB per step \n");
        printf("                     0:  -64    dB\n");
        printf("                     1:  -63.5 dB\n");
        printf("                     2:  -63    dB\n");
        printf("                     .       . \n");
        printf("                     .       . \n");
        printf("                     .       . \n");
        printf("                  128:    0     dB\n");
        printf("                     .       . \n");
        printf("                     .       . \n");
        printf("                     .       . \n");
        printf("                  164:    18   dB\n");
        return -1;    
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    eStrmSrc = (AUD_DEC_STREAM_FROM_T)StrToInt(szArgv[2]);
    u4Volume = (UINT8)StrToInt(szArgv[3]);
   
    AUD_DspSetSrcVol(u1DecId, eStrmSrc, (INT16)(u4Volume-128));
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
    
    const CHAR* strStreamSrc[AUD_STREAM_FROM_NUM]= {"Others","DTuner","ATuner","SPDIF","Line_In","HDMI","Memory","Buffer Agent", "Feeder"}; 

    u1DecId = 0;
    
    for (i=0; i<AUD_STREAM_FROM_NUM; i++)
    {
        u4Volume = AUD_DspGetSrcVol(u1DecId, (AUD_DEC_STREAM_FROM_T)i);
        Printf("%s : %d\n", strStreamSrc[i] ,u4Volume);
    }

     return 1;
}

/*
static INT32 _AudCmpUopGetChVolume(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u1DecId;
    AUD_CH_T eChannel;
    UINT8 u1Value;
    
    if (i4Argc < 3)
    {
        Printf("Usage: chdelay [decoder id] [value] \n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[channel #]      0:L, 1:R, 2:LS, 3:RS, 4:C, 5:SUBWOOFER, 6:ALL\n");
        return -1;    
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    eChannel = (AUD_CH_T)StrToInt(szArgv[2]);
   
    u1Value = AUD_DspGetChannelVolume(u1DecId, eChannel);
    Printf("Channel %d, volume = %d\n", eChannel, u1Value);
    return 1;
}
*/

static INT32 _AudCmpUopAVC(INT32 i4Argc,const CHAR** szArgv)
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

static INT32 _AudCmpUopMonoDownmix(INT32 i4Argc,const CHAR** szArgv)
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

static INT32 _AudCmpUopKaraokeMixRatio(INT32 i4Argc,const CHAR** szArgv)
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
static INT32 _AudCmpUopFineVolume(INT32 i4Argc,const CHAR** szArgv)
{
    UINT32 u4Volume;
    UINT8 u1DecId;
    
    if (i4Argc == 2)
    {
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
        Printf("Usage: fv {[decoder id]} [value] \n");
#ifdef CC_MT5391_AUD_3_DECODER 
//#if 1
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
#else
        Printf("[decoder id]     0:FIRST                 1:SECOND                 2:THIRD \n");
#endif
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

static INT32 _AudCmpUopVolume(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u1DecId;
    UINT32 u4Volume;
    
    if (i4Argc == 2)
    {
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
        Printf("Usage: volume {[decoder id]} [volume] \n");
#ifndef CC_MT5391_AUD_3_DECODER             
//#if 1
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
#else
        Printf("[decoder id]     0:FIRST                 1:SECOND                 2:THIRD \n");
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
static INT32 _AudCmpUopVolumeCurve(INT32 i4Argc,const CHAR** szArgv)
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

static INT32 _AudCmpUopQueryVolumeCurve(INT32 i4Argc,const CHAR** szArgv)
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
static INT32 _AudCmpUopSetVolTable(INT32 i4Argc,const CHAR** szArgv)
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
static INT32 _AudCmpUopSetChVolTable(INT32 i4Argc,const CHAR** szArgv)
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

static INT32 _AudCmpUopQueryChannelVolumeCurve(INT32 i4Argc,const CHAR** szArgv)
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
static INT32 _AudCmpUopAutoDmx(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Enable;
    
    if (i4Argc < 3)
    {
        Printf("Usage: chdelay [decoder id] [On/Off] \n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[On/Off]         0/1\n");
        return -1;    
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Enable = (UINT8)StrToInt(szArgv[2]);
   
    AUD_DspAutoDmx(u1DecId, (BOOL)u1Enable);
    return 1;
}
#ifdef CC_AUD_SOUND_MODE
static INT32 _AudCmpUopPostSoundMode(INT32 i4Argc,const CHAR** szArgv)
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
#ifndef CC_NEW_POST_MULTI_VS_SUPPORT
static INT32 _AudCmpUopVirtualSurround(INT32 i4Argc,const CHAR** szArgv)
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
#if 0
static INT32 _AudCmpUopVSMode(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1ModeIdx;
    
    if (i4Argc < 3)
    {
        Printf("Usage: vsm [decoder id] [mode] \n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[Mode]         0:case 0     1:case 1      2:case 3... \n");
        return -1;    
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1ModeIdx = (UINT8)StrToInt(szArgv[2]);
   
    AUD_DspVSModeChange(u1DecId, u1ModeIdx);
    return 1;
}
#endif
static INT32 _AudCmpUopVirtualSurround(INT32 i4Argc,const CHAR** szArgv)
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
static INT32 _AudCmpUopVSurrCfg(INT32 i4Argc,const CHAR** szArgv)
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

static INT32 _AudCmpUopHpfEnable(INT32 i4Argc,const CHAR** szArgv)
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

static INT32 _AudCmpUopHpfFc(INT32 i4Argc,const CHAR** szArgv)
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

static INT32 _AudCmpUopClipper(INT32 i4Argc,const CHAR** szArgv)
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

static INT32 _AudCmpUopClipperVal(INT32 i4Argc,const CHAR** szArgv)
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

#ifdef CC_AUD_SPEAKER_HEIGHT
#ifndef CC_AUD_SPEAKER_HEIGHT_MODE
static INT32 _AudCmpUopSPH(INT32 i4Argc,const CHAR** szArgv)
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
static INT32 _AudCmpUopSPH(INT32 i4Argc,const CHAR** szArgv)
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

#ifdef CC_NEW_POST_SUPPORT
static INT32 _AudCmpUopPEQ(INT32 i4Argc,const CHAR** szArgv)
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

static INT32 _AudCmpUopPEQCfg2(INT32 i4Argc,const CHAR** szArgv)
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

static INT32 _AudCmpUopPEQCfg(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1PEQSet;
    UINT32 u4Frequency;
    UINT16 u2BW;
    INT16 i2Gain;
    const CHAR *Argv;
    
    if (i4Argc < 5)
    {
        Printf("Usage: peq [set] [freq] [Bandwidth] [Gain] \n");
        Printf("[set]         0 ~ 8 \n");
        Printf("[u4Frequency] 20 ~ 20K \n");
        Printf("[Bandwidth]   xxHz \n");
        Printf("[Gain]        Gain -24 ~ 24 (-12dB ~ 12dB) with 0.5dB/step \n");
        return -1;    
    }

    u1PEQSet    = (UINT8)StrToInt(szArgv[1]);
    u4Frequency = (UINT32)StrToInt(szArgv[2]);
    u2BW        = (UINT16)StrToInt(szArgv[3]);
   
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

    AUD_DspSetPEQCfg(AUD_DEC_MAIN, u1PEQSet, u4Frequency, u2BW, i2Gain);

    return 1;
}
#endif

static INT32 _AudCmpUopReverbMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Mode;
   
    if (i4Argc < 3)
    {
        Printf("Usage: reverb [decoder id] [mode]\n");        
        Printf("[decoder id]   0:FIRST  1:SECOND \n");        
        Printf("[mode]         0:OFF \n");        
        Printf("               1:Concert     \n");        
        Printf("               2:Living Room \n");        
        Printf("               3:Hall        \n");         
        Printf("               4:Bathroom    \n");        
        Printf("               5:Cave        \n");        
        Printf("               6:Arena       \n");        
        Printf("               7:Church      \n");
        return -1;
    }
    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Mode = (UINT8)StrToInt(szArgv[2]);
    
    AUD_DspReverbMode(u1DecId, u1Mode);
    
    return 1;
}

static INT32 _AudCmpUopEqFlag(INT32 i4Argc, const CHAR ** szArgv)
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

static INT32 _AudCmpUopSbassFlag(INT32 i4Argc, const CHAR ** szArgv)
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

static INT32 _AudCmpUopBassBoostGain(INT32 i4Argc, const CHAR ** szArgv)
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

static INT32 _AudCmpUopClearBoostGain(INT32 i4Argc, const CHAR ** szArgv)
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

static INT32 _AudCmpUopSbassTableQry(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4BassLevel, u4TrebleLevel;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    u4BassLevel = AUD_DspGetBassBoostGain(AUD_DEC_MAIN);
    u4TrebleLevel = AUD_DspGetClearBoostGain(AUD_DEC_MAIN);
    Printf("Bass level: %d\n", u4BassLevel);
    Printf("Treble level: %d\n", u4TrebleLevel);
    
    return 1;
}

static INT32 _AudCmpUopEqMode(INT32 i4Argc, const CHAR ** szArgv)
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

static INT32 _AudCmpUopEqTableQry(INT32 i4Argc, const CHAR ** szArgv)
{
    INT8  eqValue[CH_EQ_BAND_NO+1];
    int   i;
    AUD_EQ_TYPE_T eEqType;

    UNUSED(i4Argc);
    UNUSED(szArgv);
   
    AUD_DspGetEqType(AUD_DEC_MAIN, &eEqType);
    AUD_DspChQryEqTable(AUD_DEC_MAIN, eEqType, eqValue);

    Printf("Current EQ type = %d\n", eEqType);
    for (i = 1; i <= CH_EQ_BAND_NO; i++)
    {
        printf ("band%d : %d\n", i, eqValue[i]+(EQ_BAND_GAIN_DB_MAX * 2));
    }
    return 1;
}

#ifdef CC_AUD_7BAND_EQUALIZER
static INT32 _AudCmpUopEqTableSet(INT32 i4Argc, const CHAR ** szArgv)
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
        
        Printf("[bandn]: 0 ~ 56 (for -14dB ~ 14dB, 0.5 dB/Step) \n");                                 
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
            eqValue[i] = (INT8)StrToInt(szArgv[i+1]);
            Printf("%d \n", eqValue[i] );
            eqValue[i] -= (EQ_BAND_GAIN_DB_MAX * 2);
        }        
    }
    else if (eqRange == 1)
    {
        for (i = 1; i <= 3; i++)
        {
            Printf("Band[%d] : ", i+4);
            eqValue[i+4] = (INT8)StrToInt(szArgv[i+1]);
            Printf("%d \n", eqValue[i+4] );
            eqValue[i+4] -= (EQ_BAND_GAIN_DB_MAX * 2);
        }        
    }
    
    eqValue[0] = 0;
    AUD_DspChSetEqTable(AUD_DEC_MAIN, eEqType, eqValue); //user defined EQ
    AUD_DspChEqLoadPreset(AUD_DEC_MAIN, eEqType);
    return 1;
}
#else
static INT32 _AudCmpUopEqTableSet(INT32 i4Argc, const CHAR ** szArgv)
{
    INT8  eqValue[CH_EQ_BAND_NO+1];
    int   i;
    AUD_EQ_TYPE_T eEqType;
   
    if (i4Argc < 6)
    {

        Printf("Usage: set  [band1] [band2] [band3] [band4] [band5]\n"); 
        Printf("[bandn]: 0 ~ 56 (for -14dB ~ 14dB, 0.5 dB/Step) \n");                                 
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
        eqValue[i] = (INT8)StrToInt(szArgv[i]);
        Printf("%d \n", eqValue[i] );
        eqValue[i] -= (EQ_BAND_GAIN_DB_MAX * 2);
    }    
    AUD_DspChSetEqTable(AUD_DEC_MAIN, eEqType, eqValue); //user defined EQ
    AUD_DspChEqLoadPreset(AUD_DEC_MAIN, eEqType);
    return 1;
}
#endif

static INT32 _AudCmpUopLimiterMode(INT32 i4Argc, const CHAR ** szArgv)
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

static INT32 _AudCmpUopLimiterFlag(INT32 i4Argc, const CHAR ** szArgv)
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

static INT32 _AudCmpUopLimiterThre(INT32 i4Argc, const CHAR ** szArgv)
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
static INT32 _AudCmpUopLimiterGainRatio(INT32 i4Argc, const CHAR ** szArgv)
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

static INT32 _AudCmpUopLimiterQuery(INT32 i4Argc, const CHAR ** szArgv)
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
    UINT8 u1DecId;
    UINT16 u2DMPosition;
   
    if (i4Argc < 2)
    {
        Printf("Usage: cb     [decoder id] [Downmix Channel Postion]\n");        
        Printf("[decoder id]  0:FIRST  1:SECOND \n");        
        Printf("[mode]        0  Downmix channel(CH9/10) do ALL post processing   \n");
        Printf("                 1  doing downmix after input source gain and before AVC   \n");
        Printf("                 2   Reserved    \n");
        Printf("                 3  Downmix channel(CH9/10) do not do post processing   \n");                                     
        return -1;       
    }
    
    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u2DMPosition = (UINT16)StrToInt(szArgv[2]);

    AUD_DspDownmixPosition(u1DecId, u2DMPosition);
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

    AUD_DspSpeed(u1DecId, u2Speed);
    return 1;
}

static INT32 _AudCmdUopMultiPairOutput(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT16 u2Flag;
   
    if (i4Argc < 2)
    {
        Printf("Usage: cb     [decoder id] [ON/OFF]\n");        
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
        Printf("Usage: cb     [ON/OFF]\n");        
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
           Printf("Usage: cb     [ON/OFF]\n");        
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
    //UINT8 u1Data4; //MT5388 only has 4 I2S data pin (AOSDATA0~AOSDATA3)
    //UINT8 u1Data5;

    if (i4Argc < 5)
    {
        Printf("Usage: osw    [PAD_SDATA0] [PAD_SDATA1] [PAD_SDATA2] [PAD_SDATA3] \n");
        Printf("[PAD_DATA]    0 L/R   \n");
        Printf("              1 Ls/Rs \n");
        Printf("              2 C/Sw  \n");
        Printf("              3 Bypass  \n");
        Printf("              4 Downmix \n"); 
        Printf("              5 Aux decoder output \n");  
        Printf("ex. osw 0 1 2 3 \n");

        return -1;
    }

    u1Data0 = (UINT8)StrToInt(szArgv[1]);
    u1Data1 = (UINT8)StrToInt(szArgv[2]);
    u1Data2 = (UINT8)StrToInt(szArgv[3]);
    u1Data3 = (UINT8)StrToInt(szArgv[4]);
    //u1Data4 = (UINT8)StrToInt(szArgv[5]);
    //u1Data5 = (UINT8)StrToInt(szArgv[6]);

    /*
    AUD_AoutPADMux((AUD_CH_T)(u1Data0*2), (AUD_CH_T)(u1Data1*2), (AUD_CH_T)(u1Data2*2), 
                   (AUD_CH_T)(u1Data3*2), (AUD_CH_T)(u1Data4*2), (AUD_CH_T)(u1Data5*2), 0);
    */                   
    
    AUD_AoutPADMux((AUD_CH_T)(u1Data0*2), (AUD_CH_T)(u1Data1*2), (AUD_CH_T)(u1Data2*2), 
                   (AUD_CH_T)(u1Data3*2), (AUD_CH_T)AUD_CH_FRONT_LEFT, (AUD_CH_T)AUD_CH_FRONT_LEFT, 0);
    return 1;
}

static INT32 _Aud8292PgaGain(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DacId, u1Data;
    
    if (i4Argc < 3)
    {
        Printf("Usage: pg [DAC id] [value] \n");
        Printf("[DAC id]  0:FIRST  1:SECOND  2: THIRD \n");
        Printf("[value] 0x00  -36dB --------- (4dB/step)\n");
        Printf("        0x01 -32dB \n");
        Printf("        0x02 -28dB \n");
        Printf("        0x03 -24dB --------- (2dB/step)\n");
        Printf("        0x04 -22dB \n");
        Printf("        0x05 -20dB \n");
        Printf("         ...   ... \n");
        Printf("        0x0A -10dB \n");
        Printf("        0x0B -8dB \n");        
        Printf("        0x0C  -7dB --------- (1dB/step)\n");
        Printf("        0x0E  -6dB \n");
        Printf("         ...   ... \n");
        Printf("        0x13   0dB \n");
        Printf("        0x14   1dB \n");
        Printf("         ...   ... \n");
        Printf("        0x21  14dB \n");
        Printf("        0x22  15dB \n");

        return -1;    
    }

    u1DacId = (UINT8)StrToInt(szArgv[1]); 
    u1Data  =  (INT16)StrToInt(szArgv[2]);

    _MT8292_SetPGAGain(u1DacId, u1Data);
    
    return 1;
}

static INT32 _Aud8292HpGain(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Data;
    
    if (i4Argc < 2)
    {
        Printf("Usage: hpg [value] \n");
        Printf("[value] 0x00  -36dB --------- (4dB/step)\n");
        Printf("        0x01 -32dB \n");
        Printf("        0x02 -28dB \n");
        Printf("        0x03 -24dB --------- (2dB/step)\n");
        Printf("        0x04 -22dB \n");
        Printf("        0x05 -20dB \n");
        Printf("         ...   ... \n");
        Printf("        0x0A -10dB \n");
        Printf("        0x0B  -8dB \n");        
        Printf("        0x0C  -7dB --------- (1dB/step)\n");
        Printf("        0x0E  -6dB \n");
        Printf("         ...   ... \n");
        Printf("        0x13   0dB \n");
        Printf("        0x14   1dB \n");
        Printf("         ...   ... \n");
        Printf("        0x27  20dB \n");
        Printf("        0x28  21dB \n");

        return -1;    
    }

    u1Data = (UINT8)StrToInt(szArgv[1]); 

    _MT8292_SetHpGain(u1Data);
    
    return 1;
}

extern void AUD_SltCheckStream(void);
extern void AUD_WriteGoldenStream(void);

static INT32 _AudCmdPlayTone(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Sel = 0;
    UINT8 u1DecId;
    MEM_BUFFER_INFO_T  rMemBuf;
    AUD_PCM_SETTING_T  rPcmSetting;
  
    if (i4Argc < 2)
    {
        Printf("Usage: tone  [tone select]\n");             
        Printf("[select]     0  1K tone 0dB\n");
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
#ifdef BURN_IN_AOUT_TEST
        Printf("             99  Dram test pattern\n");//sunman_aout
#endif        
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
    AUD_StrSelect(u1DecId, u1Sel, &rMemBuf);
    AUD_SetDecMemBuffer(u1DecId, &rMemBuf);
    AUD_DSPCmdPlay(u1DecId);
    
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
    UINT8 u1Flag = FALSE;
  
    if (i4Argc < 2)
    {
        Printf("Usage: bypass  [on/off]\n");             
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
               Printf("Usage: bypass  [on/off]\n");             
               Printf("[on/off]     1: on  \n");
               Printf("                0: off  \n");                  
               return -1;       
           }
     }

    AUD_DspBypassPostProcess(AUD_DEC_MAIN, u1Flag);
    
    return 1;
}

static INT32 _AudCmdAgcFlag(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Flag = FALSE;

    if (i4Argc < 2)
    {
        Printf("Usage: agc  [on/off]\n");             
        Printf("[on/off]      1: on  \n");
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
               Printf("Usage: agc  [on/off]\n");             
               Printf("[on/off]     1: on  \n");
               Printf("                0: off  \n");                  
               return -1;       
           }
     }
    
    AUD_DspAVCEnable(AUD_DEC_MAIN,u1Flag);
        
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

static INT32 _AudCmdBagt(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
    UINT32 u4Handler2 = NULL;
    
    AUD_BagtSetSrcInfoTest();
    AUD_RegBagtReqestFunc(&u4Handler2, (AudBagtDataReqFct)AUD_BagtDataReqTest);
    return 1;
}
#ifdef SUPPORT_MIXSOUND
static INT32 _AudCmdMixsound(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 mode;
    
    mode = (UINT16)StrToInt(szArgv[1]);
    
    if (i4Argc != 2)
    {
        Printf("Usage: m      [mode]                \n");
        Printf("[mode]        1 PLAY                \n");
        Printf("              2 STOP                \n");
        Printf("              4 FORCE STOP AND PLAY \n");
        return -1;       
    }

    if ((mode == 1)||(mode == 2)||(mode == 4))
    {
        AUD_DspMixsoundControl(mode);
    }
    else
    {
        Printf("Usage: m      [mode]                \n");
        Printf("[mode]        1 PLAY                \n");
        Printf("              2 STOP                \n");
        Printf("              4 FORCE STOP AND PLAY \n");
        return -1;       
    }

    return 1;
}

#endif

static INT32 _AudCmdTestDmxToDec2(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 2)
    {
        Printf("Usage: t2d [0/1]\n");
        Printf("Open demux to decoder 2\n");
        return -1;       
    }

    UINT8 u1OnOff = (UINT8)StrToInt(szArgv[1]);

    if (u1OnOff) //only can be test on channel 191
    {
        Printf("Turn ON Dmx audio of pid 0x134 !\n");
        CLI_Parser("dmx.sp 18 0x134 audio 1");        
        CLI_Parser("dmx.q");
    }
    else
    {
        Printf("Turn OFF Dmx audio of pid 0x134 !\n");
        CLI_Parser("dmx.fp 18");        
        CLI_Parser("dmx.q");   
    }
    
#if 1    
    if (u1OnOff)
    {
        AUD_SetDecType(AUD_DEC_AUX, AUD_STREAM_FROM_DIGITAL_TUNER, AUD_FMT_MPEG);
        AUD_DSPCmdDualDecPlay(1);
        AUD_AudioDescriptionChlSel(AUD_DEC_MAIN, AUD_CHL_DOWNMIX);  
    }
    else
    {
        AUD_DSPCmdDualDecStop(1);
        AUD_AudioDescriptionChlSel(AUD_DEC_MAIN, AUD_CHL_L_R);          
    }
#endif    

    return 0;
}


static INT32 _AudCmdClipPlay(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8  u1DecId, u1ClipType;
    AUD_CLIP_TYPE_T rAudClip;
    
    if (i4Argc < 6)
    {
        goto lbUsage;       
    }
    
    x_memset(&rAudClip, 0, sizeof(AUD_CLIP_TYPE_T));
    
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
        rAudClip.eMemBuf.pData = (UINT8 *)StrToInt(szArgv[5]);
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
static INT32 _AudCmpUopBalance(INT32 i4Argc, const CHAR **szArgv)
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
    if (i4Argc != 3)
    {
        Printf("Usage: Write [Addr] [Data]\n");
        Printf("Addr: SRAM address for Write\n");
        Printf("Data: SRAM data for Write\n");
        return -1;       
    }
    if (!fgAUD_DspProbeHalt())
    {
        Printf("DSP is not in halt state\n");
        Printf("Please execute [Pause] in advance\n");
        return -1;
    }    
    u4Addr = StrToInt(szArgv[1]);
    u4Data = StrToInt(szArgv[2]);
    AUD_DspProbeWrite(u4Addr,u4Data);
    return 0;
}
#ifdef DSP_SUPPORT_SRSWOW  
static INT32 _AudCmpUopSRSWOW(INT32 i4Argc, const CHAR **szArgv)
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
    UINT32 u4Addr=0;
    UINT16  u2Length;
    UINT16  u2Idx = 0;

    if ((i4Argc != 2)&&(i4Argc != 3))
    {
        Printf("Usage: Read [Addr] ([Length]) \n");
        Printf("Addr: SRAM address for read\n");
        return -1;       
    }
    if (!fgAUD_DspProbeHalt())
    {
        Printf("DSP is not in halt state\n");
        Printf("Please execute [Pause] in advance\n");
        return -1;
    }

    u4Addr = StrToInt(szArgv[1]);
    if (i4Argc == 2)
    {
    Printf("Probe Address: 0x%04x Data: 0x%06x\n",u4Addr,u4AUD_DspProbeRead(u4Addr));
    }
    else if (i4Argc == 3)
    {
        u2Length = StrToInt(szArgv[2]);
        if (u2Length > 0x100)
        {
            u2Length = 0x100;
        }
        Printf("Probe Address: %x, length = %x \n",u4Addr,u2Length);
        for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
        {
            if ((u2Idx % 8) == 0)
            {
                Printf("\nAddress: 0x%04x \n",u4Addr+u2Idx);
            }
            Printf("0x%06x ",u4AUD_DspProbeRead(u4Addr+u2Idx));           
        }
        Printf("\n");
    }
    return 0;
}

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
    if((szArgv[1][0]!='+'&&szArgv[1][0]!='-') || u4BPt> 3|| (u4Addr>0xFFFF))
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
        Printf("%d Address: %04x ",i,u4AUD_DspProbePB(FALSE,i,0x0));
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
    UINT32 u4Addr;
    BOOL   fgEnable; 
    
    UNUSED(i4Argc);
    UNUSED(szArgv);
    UNUSED(fgEnable);

    if ((i4Argc != 2)&&(i4Argc != 1))
    {
        Printf("Usage: sw [Addr] \n");
        Printf("Addr: SRAM address, 0 for clear\n");
        return -1;       
    }

    if (i4Argc == 1)
    {
        Printf("Usage: sw [Addr] \n");
        u4Addr = u4AUD_DspProbeChkStopOnWrite(&fgEnable);
        Printf("Current : Enable %d, Addr 0x%x\n",fgEnable,u4Addr);
    }
    else
    {      
        u4Addr = StrToInt(szArgv[1]);    
        u4AUD_DspProbeStopOnWrite(u4Addr);
    }
    
    return 0;
}

#endif 

#ifdef DSP_SUPPORT_SRSWOW 
static INT32 _AudCmpUopSRSWOWSpkSz(INT32 i4Argc, const CHAR **szArgv)
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

static INT32 _AudCmpUopSRSWOWBright(INT32 i4Argc, const CHAR **szArgv)
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

static INT32 _AudCmpUopSRSWOWTBLvl(INT32 i4Argc, const CHAR **szArgv)
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

static INT32 _AudCmpUopSRSWOWWidth(INT32 i4Argc, const CHAR **szArgv)
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
static INT32 _AudCmpUopSRSTSXTMode(INT32 i4Argc, const CHAR **szArgv)
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

static INT32 _AudCmpUopSRSTSXTSpkSz(INT32 i4Argc, const CHAR **szArgv)
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


static INT32 _AudCmpUopSRSTSXTElev(INT32 i4Argc, const CHAR **szArgv)
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

static INT32 _AudCmpUopSRSTSXTinputgain(INT32 i4Argc, const CHAR **szArgv)
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
static INT32 _AudCmpUopSRSTSXTTBLvl(INT32 i4Argc, const CHAR **szArgv)
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
static INT32 _AudCmpUopPlaySpeed(INT32 i4Argc, const CHAR **szArgv)
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

#ifdef DSP_SUPPORT_SRSTSHD
//sunman_tshd
static INT32 _AudCmpUopSRSTSHDSetInit(INT32 i4Argc, const CHAR **szArgv){
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

static INT32   _AudCmpUopSRSTSHDInpGain(INT32 i4Argc, const CHAR **szArgv)
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

static INT32	_AudCmpUopSRSTSHDOutGain(INT32 i4Argc, const CHAR **szArgv)
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

static INT32 _AudCmpUopSRSTSHDSurrLvl(INT32 i4Argc, const CHAR **szArgv)
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

static INT32 _AudCmpUopSRSTSHDSpkSz(INT32 i4Argc, const CHAR **szArgv)
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

static INT32 _AudCmpUopSRSTSHDTBLvl(INT32 i4Argc, const CHAR **szArgv)
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

static INT32 _AudCmpUopSRSTSHDDef(INT32 i4Argc, const CHAR **szArgv)
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

static INT32 _AudCmpUopSRSTSHDElev(INT32 i4Argc, const CHAR **szArgv)
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

static INT32 _AudCmpUopSRSTSHDMode(INT32 i4Argc, const CHAR **szArgv)
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
#endif //DSP_SUPPORT_SRSTSHD


#ifdef CC_AUD_BBE_SUPPORT  
static INT32 _AudCmpUopBBEQuery(INT32 i4Argc, const CHAR **szArgv)
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

static INT32 _AudCmpUopBBEMode(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Mode;

    	if (i4Argc < 2)
    	{
       	Printf("Usage: mode  [mode]\n");
        	Printf("[mode] 0:  off,  1:  BBE,  2:  BBE VIVA4\n");
              return -1;
    	}
    	u1Mode = (UINT8)StrToInt(szArgv[1]);

    	AUD_DspBbeMode(0, u1Mode); //for main decoder only
    	return 1;	
}

static INT32 _AudCmpUopBBELevel(INT32 i4Argc, const CHAR **szArgv)
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

static INT32 _AudCmpUopBBEHighGain(INT32 i4Argc, const CHAR **szArgv)
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

static INT32 _AudCmpUopBBELowGain(INT32 i4Argc, const CHAR **szArgv)
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

static INT32 _AudCmpUopBBE3DGain(INT32 i4Argc, const CHAR **szArgv)
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
#if 0
static INT32 _AudCmpUopSAPVolume(INT32 i4Argc,const CHAR** szArgv)
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
        VERIFY(BSP_FreeAlignedDmaMemory((UINT8*)pMemPlayBuf));
        Printf( "Free memory 0x%x \n",pMemPlayBuf);    
        pMemPlayBuf = 0;
        return 0;
    }
    else
    {
        VERIFY(BSP_FreeAlignedDmaMemory((UINT8*)pMemPlayBuf));
        Printf( "Free memory 0x%x \n",pMemPlayBuf);   
    }
    
    Printf( "Allocate memory size 0x%x for memory play...\n",u4Size);
    pMemPlayBuf = (UINT8 *)BSP_AllocAlignedDmaMemory(u4Size, 32);    
    if (pMemPlayBuf == NULL)
    {
        printf( "Memory request for memory play fail !!\n");
        return -1;
    }

    Printf( "Allocate memory from 0x%x to 0x%x Success, len = 0x%x\n",pMemPlayBuf,pMemPlayBuf+u4Size,u4Size);
    Printf( "data.load.binary FILENAME 0x%x--0x%x /ny /swap \n",pMemPlayBuf,pMemPlayBuf+u4Size);

    return 0;
   
}

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
    "FMRDO_STEREO"
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
    UNUSED(i4Argc);
    UNUSED(szArgv);
    AUD_FMT_T eAudDecFormat;
    UINT8 u1DecId;
  
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
    "NICAM_DUAL1_DUAL2"
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
        Printf("   10  :  mono to stereo threshold     64~1024     %d     \n",AUD_DspGetA2Threshold(AUD_FACTORY_A2_MONO2STEREO));   
        Printf("   11  :  stereo to mono threshold      0~512        %d      \n",AUD_DspGetA2Threshold(AUD_FACTORY_A2_STEREO2MONO)); 
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
 
    Printf(" Current audio format:  %s  \n",_aszAudFormat[rAudInfo.e_aud_fmt]);
    Printf(" Sampling rate:         %d Hz  \n",rAudInfo.ui4_sample_rate);
    Printf(" Data rate:             %d bps\n",rAudInfo.ui4_data_rate);
    Printf(" Audio decoding mode:   %s\n",_aszAudACMode[rAudInfo.e_aud_type]);
        
    return 1 ;
}

static INT32 _AudCmdDtvPTSDelay(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 uDelayFactor;
  
    if (i4Argc < 2)
    {
        Printf("Usage: PTSD [factor]\n");             
        Printf("[factor] 0 ~ 255 \n\n");
        Printf("Audio start decoding PTS would increased (PTS_AC3_CNT * factor)\n");
        return -1;       
    }

    if (i4Argc >= 2)
    {	
        uDelayFactor = StrToInt(szArgv[1]);        
        AUD_AudioPTSDelayEnable(AUD_DEC_MAIN, uDelayFactor);  
	    Printf(" Set DelayFactor to %d\n", uDelayFactor);
    }    
        
    return 1;
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
    /* Andrew mark for MT5387 Build FAIL*/
#if 0
    UINT16 u2DecId;
    UINT16 u2Mode;


    if (i4Argc < 3)
    {
        Printf("Usage: dualmono [decoder id] [dual mono mode]\n");
        Printf("[decoder id]     0:FIRST                     1:SECOND \n");
        Printf("[dual mono mode] 0:STEREO (Disable)      1:L MONO \n");
        Printf("                 2:R MONO                3:MIX MONO\n");
        return -1;
    }
    u2DecId = (UINT16)StrToInt(szArgv[1]);
    u2Mode = (UINT16)StrToInt(szArgv[2]);


    if(u2Mode<5)
    {
        AUD_DspDualMono(u2Mode, u2DecId);
    }    
   
    return 1;
#else 
    Printf("Please Fix this command in MT5387 aud_cmd.c !!!!!!\n");
    return -1;
#endif
    
};

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
    u1DecId = (AUD_GetTargetCountry() == COUNTRY_EU) ? AUD_DEC_AUX : AUD_DEC_MAIN;
        
    if (i4Argc < 2)
    {
        Printf("  \n");     
        Printf("Current mode : %d\n",AUD_DspGetAC3CompMode(u1DecId));
        Printf("  \n");     
        Printf("Usage: ac3cm [compression mode]\n");
        Printf("[compression mode] 0:custom mode 0         1:custom mode 1\n");
        Printf("                   2:LINE mode             3:RF mode\n");
        return -1;    
    }
    u2Mode = (UINT16)StrToInt(szArgv[1]);

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
    u1DecId = (AUD_GetTargetCountry() == COUNTRY_EU) ? AUD_DEC_AUX : AUD_DEC_MAIN;

    if (i4Argc < 2)
    {
        Printf("  \n");     
        Printf("Current level : (%d*0.125) dB\n",AUD_DspGetAC3DRCRange(u1DecId));
        Printf("  \n");     
        Printf("Usage: ac3dr [range]\n");
        Printf("[range]          High cut and Low boost value     \n");
        Printf("                 0:0.0                   8:1.0    \n");
        Printf("                 0~8:0.0~1.0 step 0.125           \n");
        return -1;
    }
    u2Mode = (UINT16)StrToInt(szArgv[1]);

    if(u2Mode<=8)
    {
        AUD_DspAC3DRCRange(u2Mode, u1DecId);
    }   
    return 1;
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
    UINT32 u1IDbuf[8];
    	
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

    Aud_FlashAQWrite(u2Offset, &u1Data, 1);

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
        Aud_FlashAQRead(u2Offset+i, &u1Data, 1);
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
    Aud_FlashAQRead(FLASH_AQ_VER_OFFSET, (UINT8*)pTestVer, FLASH_AQ_VER_LENGTH);
    
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
        Aud_FlashAQRead(FLASH_AQ_VER_OFFSET, (UINT8*)pTestVer, FLASH_AQ_VER_LENGTH);
    }
    else
    {
        x_strcpy(pTestVer,szArgv[2]);
    }

    if (Aud_FlashAQCompVersion(pTestVer,pUppVersion))
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
    Aud_FlashAQ_SetDefault();

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
    
    Aud_FlashAQ_GetDscrpt(&eAudAQDscrpt);
    
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
    Aud_FlashAQRead(FLASH_AQ_VER_OFFSET, pBuffer, u4TotalSize);

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
    Aud_FlashAQRead(u4Offset, pBuffer, u4Temp);

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
    
    Aud_FlashAQ_Fill_Data(TRUE);
    
    return 0;
}
#endif /* CC_AUD_USE_FLASH_AQ */
//=================================================



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
    {"soundeffect",     "s",    _AudCmdSoundEffect, NULL,       "sound effect on/off", CLI_GUEST},
    {"agc",        "a", _AudCmdAgcFlag, NULL,       "agc on/off", CLI_GUEST},
    {"bypass",  "b",    _AudCmdBypassPostProcessing, NULL,      "bypass post processing", CLI_GUEST},
#ifdef __MODEL_slt__
    {"SltSorting",  "slt",  _AudCmdSltSorting, NULL,    "slt sorting", CLI_SUPERVISOR},
    {"DtvSlt",  "dtvslt",   _AudCmdDtvSltSorting, NULL, "Dtv slt sorting", CLI_SUPERVISOR},
    {"AtvSlt",  "atvslt",   _AudCmdAtvSltSorting, NULL, "Atv slt sorting", CLI_SUPERVISOR},
    {"IntAdSlt",  "iadslt",   _AudCmdIntAdSltSorting, NULL, "Internal AD slt sorting", CLI_SUPERVISOR},
    {"SltFlashing", "flash",    _AudCmdSltFlashing, NULL,   "burning slt pattern to flash", CLI_SUPERVISOR},
    {"SltROM", "romslt",    _AudCmdROMSltSorting, NULL,   "ROM test", CLI_SUPERVISOR},
#endif
    {"Bagt",     "bagt",   _AudCmdBagt, NULL,         "buffer agent", CLI_GUEST},
    {"testdemux2",  "t2d", _AudCmdTestDmxToDec2, NULL,   "Test demux send data to decoder 1", CLI_GUEST },
    {"dd_banner",   "banner",	_AudCmdDDbanner, NULL,	"DD banner turn on for DD Test", CLI_GUEST },
    {"memalloc",   "mal",	_AudCmdMemAlloc, NULL,	"Allocate memory for test", CLI_GUEST },
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
    {"iec_set",     "iecs",  _SpdifIecSet, NULL,        "IEC setting [decoder id] [flag] [enable]", CLI_GUEST},
    {"iec_ch",      "iecc",  _SpdifIecChannelSet, NULL, "IEC Channel setting [decoder id] [channel]", CLI_SUPERVISOR},
        
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudAdacCmdTbl[] = 
{
    {"format",      "f",    _AdacCmdDataFormat, NULL,   "Data format ex. format 0 1", CLI_GUEST},   
    {"datainvert",  "d",    _AdacCmdDataInvert, NULL,   "Data invert ex.  datainvert 0 1", CLI_GUEST},  
    {"samplefreq",  "sf",   _AdacCmdSampleFreq, NULL,   "DAC Sample Frequence ex.sf 0 1", CLI_GUEST},   
    {"SelectFs",    "s",    _AdacCmdSelectFs, NULL,     "Select Fs ex. SelectFs 0x01", CLI_SUPERVISOR},
    {"drvcurrent",  "drv",  _AdacCmdDrvCur, NULL,       "Select MCLK BCLK driving current ex. drv 0 8 8", CLI_GUEST},   
    {"volume",      "v",    _AdacCmdWM8776VolCtl, NULL, "WM8776 headphone volume control", CLI_GUEST},  
   {"CodecRead",        "cr",   _AdacCmdCodecRead, NULL,    "WM8776/MT8291 register read", CLI_GUEST},  
   {"CodecWrite",       "cw",   _AdacCmdCodecWrite, NULL,   "WM8776/MT8291 register write", CLI_GUEST}, 
   {"CodecChSel",       "ch",   _AdacCmdCodecChlSel, NULL,  "WM8776/MT8291 register write", CLI_GUEST}, 
   {"AmpRead",      "ar",   _AdacCmdAmpRead, NULL,  "Amplifier register read", CLI_GUEST},  
   {"AmpWrite",     "aw",   _AdacCmdAmpWrite, NULL, "Amplifier register write", CLI_GUEST}, 
    {"AmpRamRead",   "arr",  _AdacCmdAmpRamRead, NULL, "Amplifier ram read", CLI_GUEST},
    {"AmpRamWrite",  "awr",  _AdacCmdAmpRamWrite, NULL, "Amplifier ram write", CLI_GUEST},
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
#ifdef CC_MT5391_AUD_SUPPORT
    {"PGA gain",    "pg",   _InterDacPGAGain,    NULL,  "Internal DAC PGA gain adjust", CLI_GUEST},	
    {"Headphone PGA gain",    "hpg",  _InterHeadphonePGAGain,    NULL,  "Internal Headphone PGA gain adjust", CLI_GUEST},	
#endif 
    // last cli command record, NULL
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#if 1 // PROBE_RELATED_API
static CLI_EXEC_T _arAudProbeCmdTbl[] =
{
    {"pause",       "p",    _AudCmdProbePause,  NULL,       "Probe: Pause", CLI_SUPERVISOR},
    {"step",        "st",    _AudCmdProbeStep,  NULL,       "Probe: Step",  CLI_SUPERVISOR},
    {"go",          "g",    _AudCmdProbeGo,     NULL,       "Probe: Go",    CLI_SUPERVISOR},
    {"read",        "r",    _AudCmdProbeRead,   NULL,       "Probe: Read",  CLI_SUPERVISOR},
    {"write",       "w",    _AudCmdProbeWrite,   NULL,      "Probe: Write",  CLI_SUPERVISOR},
    {"ShowRegs",    "sr",    _AudCmdShowRegs,   NULL,       "Probe: Show Registers",    CLI_SUPERVISOR},
    {"SetBPts",     "stb",    _AudCmdSetBPts,   NULL,       "Probe: Setup BreakPoints", CLI_SUPERVISOR},
    {"ShowBPts",    "sb",    _AudCmdShowBPts,   NULL,       "Probe: Show BreakPoints",  CLI_SUPERVISOR},
    {"StopOnWrite", "sw",    _AudCmdStopOnWrite, NULL,       "Probe: Stop on Write",   CLI_SUPERVISOR},
    
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
    {"pcm pre-buffer",       "pb",    _AudPreBuffer, NULL,        "pcm pre-buffer", CLI_GUEST},
    {"cache",       "cache",    _AudCacheHitRate, NULL,        "DSP cache hit rate", CLI_GUEST},
    {"ROMID",       "id",    _AdspDSPROMID, NULL,        "ROM ID", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#ifdef __MODEL_slt__
static CLI_EXEC_T _arAudDSPSLTCmdTbl[] =
{
    {"init",        "i",    _ADSPSLTInit,   NULL,       "adsp SLT init. once",  CLI_SUPERVISOR},
    {"test",        "t",    _ADSPSLTTest,   NULL,       "adsp SLT testing",     CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

static CLI_EXEC_T _arAudUopEqTbl[] =
{
    {"flag",        "flag",    _AudCmpUopEqFlag , NULL,   "EQ function on/off", CLI_GUEST},
    {"mode",        "m",     _AudCmpUopEqMode, NULL, "EQ mode configuration", CLI_GUEST},
    {"eqTableSet", "set", _AudCmpUopEqTableSet, NULL, "Set EQ Table", CLI_GUEST},
    {"eqTableQry", "q", _AudCmpUopEqTableQry, NULL, "Query EQ Table", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudUopSbassTbl[] =
{
    {"flag",        "flag", _AudCmpUopSbassFlag , NULL,   "sbass function on/off", CLI_GUEST},
    {"bassboost",   "bb",     _AudCmpUopBassBoostGain, NULL, "Bass boost gain configuration", CLI_GUEST},
    {"clearboost",  "cb",     _AudCmpUopClearBoostGain, NULL, "Clear boost gain configuration", CLI_GUEST},
    {"query",       "q", _AudCmpUopSbassTableQry, NULL, "query sbass Table", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudUopLimiterTbl[] =
{
    {"mode",        "m", _AudCmpUopLimiterMode , NULL,   "limiter mode", CLI_GUEST},
    {"flag",   "flag",     _AudCmpUopLimiterFlag, NULL, "individual limiter flag", CLI_GUEST},
    {"threshold",  "thre",     _AudCmpUopLimiterThre, NULL, "limiter fixed mode threshold", CLI_GUEST},
#ifdef CC_RATIO_LIMITER
    {"ratio",  "ratio",     _AudCmpUopLimiterGainRatio, NULL, "limiter gain ratio", CLI_GUEST},
#endif
    {"query",       "q", _AudCmpUopLimiterQuery, NULL, "query limiter configuration", CLI_GUEST},
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
    {"OnOff",       "onoff", _AudCmpUopSRSWOW,       NULL,  "SRS WOW On/Off",   CLI_SUPERVISOR},
    {"wspk",        "wspk",  _AudCmpUopSRSWOWSpkSz,  NULL,  "Speaker Size",     CLI_SUPERVISOR},
    {"wbrt",        "wbrt",  _AudCmpUopSRSWOWBright, NULL,  "Brightness",       CLI_SUPERVISOR},
    {"wtb",         "wtb",   _AudCmpUopSRSWOWTBLvl,  NULL,  "TruBass Level",    CLI_SUPERVISOR},
    {"wwid",        "wwid",  _AudCmpUopSRSWOWWidth,  NULL,  "Width",            CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif
#ifdef DSP_SUPPORT_SRSTSXT
static CLI_EXEC_T _arAudUopSRSTSXTTbl[] =
{
    {"tspk",        "tspk",  _AudCmpUopSRSTSXTSpkSz,  NULL,  "Speaker Size",     CLI_SUPERVISOR},
    {"tsmode",      "tsmd",  _AudCmpUopSRSTSXTMode,  NULL,  "Mode",     CLI_SUPERVISOR}, 
    {"telv",        "telv",  _AudCmpUopSRSTSXTElev,   NULL,  "Elevation",        CLI_SUPERVISOR},
    {"ing",        "ing",  _AudCmpUopSRSTSXTinputgain,   NULL,  "input gain",        CLI_SUPERVISOR},
    {"ttb",         "ttb",   _AudCmpUopSRSTSXTTBLvl,  NULL,  "TruBass Level",    CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

#ifdef DSP_SUPPORT_SRSTSHD
//sunman_tshd 
static CLI_EXEC_T _arAudUopSRSTSHDTbl[] =
{
	{"tinit", "tinit",		_AudCmpUopSRSTSHDSetInit,	NULL,	"Initial settings ",        	CLI_SUPERVISOR},	    
	{"tinpg",	"tinpg",	_AudCmpUopSRSTSHDInpGain,	NULL,	"Input gain",	       	CLI_SUPERVISOR},
       {"toutg",	"toutg",	_AudCmpUopSRSTSHDOutGain,	NULL,	"Output gain",		       CLI_SUPERVISOR},
     	{"tsrlv",  	"tsrlv",	_AudCmpUopSRSTSHDSurrLvl,	NULL,	"Surround Level ",        	CLI_SUPERVISOR},
 	{"tspk",	"tspk",	_AudCmpUopSRSTSHDSpkSz,	NULL,	"Speaker Size",		CLI_SUPERVISOR},
	{"ttb",	"ttb",	_AudCmpUopSRSTSHDTBLvl,	NULL,	"TruBass Level",    		CLI_SUPERVISOR},
	{"tdef",	"tdef",	_AudCmpUopSRSTSHDDef,   	NULL,	"Definition Control",	CLI_SUPERVISOR},
	{"telv",	"telv",	_AudCmpUopSRSTSHDElev,		NULL,	"FOCUS Elevation",       CLI_SUPERVISOR},
       {"tmode","tmode",	_AudCmpUopSRSTSHDMode,	NULL,	"TSHD Mode",			CLI_SUPERVISOR},
	//{"tflag",	"tflag",	_AudCmpUopSRSTSHDSetFlag,	NULL,	"TSHD Flag",			CLI_SUPERVISOR},
	//{"tmode","tmode",	_AudCmpUopSRSTSHDSetLtRt_PL2M,	NULL,	"LtRt or PL2Music",	CLI_SUPERVISOR},
	//{"tsspk",	"tsspk",	_AudCmpUopSRSTSHDSubSpkSz,NULL,	"Sub Speaker Size",	CLI_SUPERVISOR},
	//{"tcofq",	"tcofq", 	_AudCmpUopSRSTSHDCOFREQ,	NULL,	"CrossOver Freq",		CLI_SUPERVISOR},
	//{"tts",	"tts",	_AudCmpUopSRSTSHDSetTSCtrl,   	NULL,	"TruSurround Control",	CLI_SUPERVISOR},
	//{"tlim",  	"tlim",	_AudCmpUopSRSTSHDLimiter,	NULL,	"Limiter Control",        	CLI_SUPERVISOR},
	//{"timod", "timod",	_AudCmpUopSRSTSHDInpuMode,	NULL,	"Set Input Mode ",        	CLI_SUPERVISOR},
	//{"tomod", "tomod",	_AudCmpUopSRSTSHDOutputMode,	NULL,	"Set Output Mode ",        	CLI_SUPERVISOR},
	//{"tbypass", "tbypass",		_AudCmpUopSRSTSHDSetBypass,	NULL,	"Set Bypass Mode ",        	CLI_SUPERVISOR},
	//{"tout", "tout",	 _AudCmpUopSRSTSHDSetSubToL,	NULL,	"Set Sub, Center to L, R ",        	CLI_SUPERVISOR},	
     {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif


#ifdef CC_AUD_BBE_SUPPORT      
static CLI_EXEC_T _arAudUopBBETbl[] =
{
    {"query",    "q", _AudCmpUopBBEQuery,  NULL, "Query BBE Config", CLI_GUEST},
    {"setmode", "mode",   _AudCmpUopBBEMode, NULL, "Set BBE Mode", CLI_GUEST},
    {"setlevel", "level", _AudCmpUopBBELevel,  NULL, "Set BBE Level", CLI_GUEST},
    {"sethigh", "high",  _AudCmpUopBBEHighGain, NULL, "Set BBE High Gain", CLI_GUEST},
    {"setlow", "low", _AudCmpUopBBELowGain,  NULL, "Query BBE Low Gain", CLI_GUEST},
    {"set3d" , "3d", _AudCmpUopBBE3DGain, NULL, "Query BBE Config", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR} 
};
#endif

static CLI_EXEC_T _arAudUopSpkCfgTbl[] =
{
    {"spkcfg",     "spkcfg",    _AudCmdUopSpeakerConfig , NULL, "speaker config steup", CLI_GUEST},
    {"delaycfg",   "delaycfg",  _AudCmpUopChannelDelay , NULL,  "channel delay steup", CLI_GUEST},
    {"largesmall", "ls",        _AudCmdUopSpeakerLargeSmallConfig , NULL,   "speaker large/small config", CLI_GUEST},
    {"cutoff_freq","cf",        _AudCmpUopCutFreq , NULL,   "set cutoff frequency", CLI_GUEST},
    {"channel",    "ch",        _AudCmdUopSpeakerOutputConfig , NULL,   "speaker config", CLI_GUEST},
    {"subwoofer",  "sw",        _AudCmdUopSpeakerSubwooferEnable , NULL,    "subwoofer on/off", CLI_GUEST},
    {"bass",       "bs",        _AudCmdUopBassMngEnable , NULL, "bass management on/off", CLI_GUEST},
    {"autodm",     "dm",        _AudCmpUopAutoDmx , NULL,   "auto downmix on/off", CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
static CLI_EXEC_T _arAudUopCmdTbl[] = 
{
    {"fine volume", "fv",    _AudCmpUopFineVolume, NULL, "Audio Master Volume fine control", CLI_GUEST},    
    {"volume",      "v",     _AudCmpUopVolume, NULL, "Audio Volume control", CLI_GUEST},    
    {"vol_curve","vc",  _AudCmpUopVolumeCurve, NULL, "Audio Volume Curve Set", CLI_GUEST},
    {"vol_curve_query","vcq", _AudCmpUopQueryVolumeCurve, NULL, "Audio Volume Curve Query", CLI_GUEST},
#ifdef CC_SET_VOLUME
    {"set volume table", "svt",    _AudCmpUopSetVolTable, NULL, "Audio Volume table set ", CLI_GUEST},    
#endif
#ifdef CC_AUD_NEW_CV_TABLE
    {"set CHVOl table", "scvt", _AudCmpUopSetChVolTable, NULL, "Audio Channle Volume table set ", CLI_GUEST},    
    {"ch_vol_curve_query","cvcq", _AudCmpUopQueryChannelVolumeCurve, NULL, "Channel Audio Volume Curve Query", CLI_GUEST},
#endif
    {"ch volume",   "cv",     _AudCmpUopChVolume, NULL, "Audio Channel Volume control", CLI_GUEST},         
    {"ch vol gain", "cvg",    _AudCmpUopChVolumeGain, NULL, "Audio Channel Volume Gain control", CLI_GUEST},
#ifdef CC_AUD_VOLUME_OFFSET
    {"volume offset", "vo",	  _AudCmpUopVolumeOffset, NULL, "Audio Channel Volume offset", CLI_GUEST},
#endif

#ifdef CC_AUD_LINEOUT_VOL_OFFSET
    {"linout vol offset",	"lovo",	  _AudCmpUopLineOutVolOffset, NULL, "Audio Line out Volume offset", CLI_GUEST}, 		
#endif
#ifdef CC_AUD_LINEOUT_VOL_MODE
    {"linout vol", "lov",	  _AudCmpUopLinOutVolume, NULL, "Audio Line out Volume control", CLI_GUEST},
#endif
#ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
    {"src linout vol", "slov",	  _AudCmdUopSrcLinOutVolume, NULL, "Audio Source Line out Volume set", CLI_GUEST},
#endif
    {"src volume",  "sv",  _AudCmdUopSrcVolume, NULL, "Audio Source Volume control", CLI_GUEST},
    {"src volume query", "svq", _AudCmdUopGetSrcVolume, NULL, "Query Audio Source Volume", CLI_GUEST},
    {"equalizer",   "eq",     NULL, _arAudUopEqTbl, "EQ configuration", CLI_GUEST},
    {"sbass",   "sbass",     NULL, _arAudUopSbassTbl, "Bass/treble configuration", CLI_GUEST},
    {"reverb",      "rb",     _AudCmpUopReverbMode, NULL, "Reverb configuration", CLI_GUEST},
    {"limiter",   "limiter",     NULL, _arAudUopLimiterTbl, "Limiter configuration", CLI_GUEST},
    {"pl2cfg",      "pl2cfg", NULL, _arAudUopPL2Tbl,"Prologic 2 related configuration", CLI_GUEST},     
    {"spkuop",      "spkuop", NULL, _arAudUopSpkCfgTbl,"Speaker related configuration", CLI_GUEST},
    {"AVC",      "a",     _AudCmpUopAVC, NULL, "Automatic Volume Control", CLI_GUEST},  
    {"MonoDownmix", "md",     _AudCmpUopMonoDownmix, NULL, "Downmix to mono", CLI_SUPERVISOR},
    {"Karaoke mix ratio", "kr",     _AudCmpUopKaraokeMixRatio, NULL, "Karaoke mix ratio", CLI_SUPERVISOR},    
#ifdef FRAC_SPEED_SUPPORT
    {"playSpeed", "fspeed",  _AudCmpUopPlaySpeed, NULL, "Set fractional play speed", CLI_GUEST},  //sunman for play speed
#endif
#if defined(__linux__)
    {"balance",      "bl",     _AudCmpUopBalance, NULL, "Set L/R balance", CLI_GUEST},  
#endif
#ifdef DSP_SUPPORT_SRSWOW  
    {"srswow",      "wow", NULL, _arAudUopSRSWOWTbl, "Set SRS WOW", CLI_GUEST},  
#endif
#ifdef DSP_SUPPORT_SRSTSXT
    {"srstsxt",     "tsxt", NULL, _arAudUopSRSTSXTTbl, "Set SRS TSXT", CLI_GUEST},  
#endif    
    {"virtualsurround",   "vs",     _AudCmpUopVirtualSurround, NULL, "Virtual Surround flag", CLI_GUEST},   
    {"vsurround config",   "vscfg",     _AudCmpUopVSurrCfg, NULL, "Virtual Surround Config", CLI_GUEST},
#ifdef DSP_SUPPORT_SRSTSHD
    {"srstshd",     "tshd", NULL, _arAudUopSRSTSHDTbl, "Set SRS TSHD", CLI_GUEST},  //sunman_tshd
#endif
#ifdef CC_AUD_SOUND_MODE
    {"post sound mode",   "psm",     _AudCmpUopPostSoundMode, NULL, "Sound mode change", CLI_GUEST},   
#endif
    //{"surround mode",   "vsm",     _AudCmpUopVSMode, NULL, "surround mode change", CLI_GUEST},   
#ifdef CC_AUD_BBE_SUPPORT      
    {"bbe",         "bbe", NULL, _arAudUopBBETbl, "Set BBE Function", CLI_GUEST},
#endif
    {"hpf",         "hpf",      _AudCmpUopHpfEnable, NULL,  "Set HPF enable",    CLI_GUEST},
    {"hpf_fc",      "fc",       _AudCmpUopHpfFc, NULL,      "Set HPF Fc",        CLI_GUEST},
    {"clipper",     "clip",     _AudCmpUopClipper, NULL,    "Set clipper enable",CLI_GUEST},
    {"clip_val",    "clipval",  _AudCmpUopClipperVal, NULL, "Set clipper value", CLI_GUEST},
#ifdef CC_NEW_POST_SUPPORT    
    {"peq",         "peq",      _AudCmpUopPEQ, NULL,        "Set PEQ",           CLI_GUEST},
    {"peqc",        "peqc",     _AudCmpUopPEQCfg, NULL,     "Set PEQ Configuration", CLI_GUEST},
    {"peqc2",       "peqc2",    _AudCmpUopPEQCfg2, NULL,    "Set PEQ Configuration", CLI_GUEST},
#endif
#ifdef CC_AUD_SPEAKER_HEIGHT
    {"sph", 		"sph",		_AudCmpUopSPH, NULL,		"Set Speaker Height", CLI_GUEST},
#endif
    {"FF SPEED",    "speed",    _AudCmdDspSpeed , NULL,     "DSP Speed", CLI_GUEST}, 
    {"DMixPos",     "DMP",      _AudCmdUopDownmixPosition , NULL, "Downmix channel position", CLI_GUEST}, 
    {"MultiPair",   "MultiPair",_AudCmdUopMultiPairOutput , NULL, "Multi-Pair output", CLI_GUEST}, 
    {"LR DownMix",  "LRDMix",   _AudCmdUopLRDownMix, NULL, "LR DownMix", CLI_GUEST}, 
    {"dump",        "dump",     _AudCmdDspUopDump , NULL, "DSP Dump", CLI_GUEST}, 
    {"DDCO",        "DDCO",     _AudCmdUopDDCO , NULL, "DDCO setting", CLI_GUEST}, 
    {"DDCO AGC",        "DDCOAgc",     _AudCmdUopDDCOAgc, NULL, "DDCO AGC setting", CLI_GUEST},     
    {"DDCO LFE LFP",        "DDCOLFE",     _AudCmdUopDDCOLFE, NULL, "DDCO AGC setting", CLI_GUEST}, 
    {"dualmono",    "dm",    _AudCmdDtvDualMono , NULL,    "dual-mono steup", CLI_SUPERVISOR},
 
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudCfgCmdTbl[] = 
{
    {"AvInSwitch",    "isw",     _AudCfgAvInSwitch,   NULL, "Av input component select", CLI_GUEST},    
    {"ChlOutCfg",     "osw",     _AudCfgChlOutSwitch, NULL, "Out channel switch", CLI_GUEST},       
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAud8292CmdTbl[] =
{
    {"PGA gain",    "pg",     _Aud8292PgaGain,   NULL, "PGA gain", CLI_GUEST},
    {"HP gain",    "hpg",     _Aud8292HpGain,   NULL, "Headphone gain", CLI_GUEST},    
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudAtvCmdTbl[] = 
{
    {"query",     "q",    _AudCmdAtvquery, NULL, "query ATV audio informantion", CLI_SUPERVISOR},
    {"Tvsys",     "sys",    _AudCmdAtvTvSys, NULL, "Tv channel system: B/G, D/K, I, L, M", CLI_SUPERVISOR},
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
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudDtvCmdTbl[] = 
{
    {"query DTV",     "q",    _AudCmdDtvQuery, NULL, "Query DTV decoder informantion", CLI_SUPERVISOR},
    {"ptsdly",      "ptsd",	_AudCmdDtvPTSDelay,     NULL,  "Delay audio startup by increasing PTS", CLI_GUEST},
    {"Set AC3",     "ac3",    NULL, _arAudDtvAC3Tbl, "Set AC3 decoder informantion", CLI_SUPERVISOR},

    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudFMRadioCmdTbl[] =
{
    {"detect FM",     "det",    _AudCmdFMRadioDetect, NULL, "Detect FM radio channel", CLI_SUPERVISOR},
    {"play FM",       "play",   _AudCmdFMRadioPlay,   NULL, "Play FM radio channel", CLI_GUEST},
    {"FM stereo",     "fm2",   _AudCmdFMRadioSoundMode,   NULL, "Setup FM radio channel", CLI_GUEST},

    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudIoCmdTbl[] = 
{
    {"spdif",       NULL,   NULL, _arAudSpdifCmdTbl,    "spdif command", CLI_GUEST },
    {"adac",        NULL,   NULL, _arAudAdacCmdTbl,     "adac command", CLI_GUEST },
    {"interdac",    NULL,   NULL, _arAudInterDacCmdTbl, "inter dac command", CLI_GUEST },
    {"cfg",         NULL,   NULL, _arAudCfgCmdTbl,      "configuration", CLI_GUEST },
    {"8292",         NULL,   NULL, _arAud8292CmdTbl,      "8292 configuration", CLI_GUEST },
    
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
        
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
/******************************************************************************
* Variable      : cli "aud" command table
******************************************************************************/

static CLI_EXEC_T _arAudCmdTbl[] =
{
    {"init",        "i",    _AudCmdInit, NULL,          "Aud init", CLI_SUPERVISOR},
    CLIMOD_DEBUG_CLIENTRY(AUD),
    //CLIMOD_REGTST_CLIENTRY(AUD),
    {"setdec",      "s",    _AudCmdSetDecType,   NULL,  "Aud set decoding type", CLI_SUPERVISOR},
    {"resume",      NULL,   _AudCmdResume,       NULL,  "Aud resume", CLI_SUPERVISOR},
    {"pause",       NULL,   _AudCmdPause,        NULL,  "Aud pause", CLI_SUPERVISOR},
    {"play",        NULL,   _AudCmdPlay,         NULL,  "Aud play", CLI_SUPERVISOR},
    {"stop",        NULL,   _AudCmdStop,         NULL,  "Aud stop", CLI_SUPERVISOR},
    {"dualset",     "ds",   _AudCmdDualDecSetDecType, NULL,  "dualset", CLI_SUPERVISOR},
    {"dualplay",    "dplay", _AudCmdDualDecPlay, NULL,  "dualplay", CLI_SUPERVISOR},
    {"dualstop",    "dstop", _AudCmdDualDecStop, NULL,  "dualstop", CLI_SUPERVISOR},
    {"dual",    "dual", _AudCmdDualDecMode, NULL,  "dual", CLI_SUPERVISOR},
#ifdef CC_MT5391_AUD_3_DECODER
    {"tri",    "tridec", _AudCmdTriDecMode, NULL,  "tri_dec", CLI_SUPERVISOR},
#endif
    {"sync",        NULL,   _AudCmdSyncMode,     NULL,  "Aud sync mode setting", CLI_SUPERVISOR},
    {"stream",      "str",  _AudCmdSetStream,    NULL,  "Playback stream src", CLI_SUPERVISOR},
    {"query",       "q",    _AudCmpQuery,        NULL,  "Audio status query", CLI_SUPERVISOR},
#ifdef SUPPORT_MIXSOUND
    {"mixsound",    "m",    _AudCmdMixsound,     NULL,  "Audio Mixsound Playback", CLI_SUPERVISOR},
#endif    
    {"dsp",         NULL,   NULL, _arAudDspCmdTbl,      "dsp command", CLI_GUEST },
    {"uop",         NULL,   NULL, _arAudUopCmdTbl,      "audio uop", CLI_GUEST },
    {"t",           NULL,   NULL, _arAudTestCmdTbl,     "test", CLI_GUEST },
    {"Clip",        NULL,   NULL, _arAudClipCmdTbl,     "Aud Clip", CLI_GUEST },
#ifdef __MODEL_slt__  
    {"dspslt",      NULL,   NULL, _arAudDSPSLTCmdTbl,   "DSP SLT", CLI_SUPERVISOR },
#endif
#if defined(__linux__)
    {"isplay",  "ip",   _AudCmdIsDecoderPlay, NULL,     "Check if audio decoder is playing", CLI_SUPERVISOR},
#endif
    {"atv",  NULL,  NULL,  _arAudAtvCmdTbl,  "ATV command", CLI_SUPERVISOR},
    {"dtv",  NULL,  NULL,  _arAudDtvCmdTbl,  "DTV command", CLI_SUPERVISOR},
    {"fmrdo", NULL, NULL, _arAudFMRadioCmdTbl, "FM Radio command", CLI_SUPERVISOR},
    {"io",  NULL,  NULL,  _arAudIoCmdTbl,  "I/O command", CLI_GUEST},
    {"nvm",      "nvm",   NULL, _arAudNvmCmdTbl,  "Nvm Read/Wrtie", CLI_GUEST },
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
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

