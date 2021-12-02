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

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_assert.h"
#include "x_os.h"
#include "x_printf.h"
#include "aud_if.h"
LINT_EXT_HEADER_END
#include "x_drv_if.h"
#include "aud_common.h"
#include "dmx.h"
#include "pcr.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define AUD_TV_DECT_THREAD_STACK_SIZE   2048
#define AUD_DRV_THREAD_PRIORITY         100

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static HANDLE_T _hTvDetectThread;
static HANDLE_T _hTvDetectDoneSema;
static CRIT_STATE_T _rNotifyLock;

static BOOL _fTvSysDetected;
static BOOL _fIsAudTvDetected;
static DRV_AUD_PARAM_T _rAudioParam;        // Real paramter set in audio driver
static DRV_AUD_PARAM_T _rInterAudioParam;   // paramter buffer if audio in play
static DRV_TV_SYS_T _rAudioTvSys;
static DRV_SOUND_SYS_T _rAudioSoundSys;
static DRV_AUD_INTERNAL_T _rAudioInternal;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
//#define CC_AUD_DUAL_DEC_SUPPORT

#ifdef CC_AUD_DUAL_DEC_SUPPORT
#define AUD_SET_DEC_TYPE(eFrom, eType)  AUD_DSPCmdDualDecSetDecType(eFrom, \
                                                                    eType)
#define AUD_SET_PLAY(fBlock)            AUD_DSPCmdDualDecPlay(fBlock)
#define AUD_SET_STOP(fBlock)            AUD_DSPCmdDualDecStop(fBlock)

#else
#define AUD_SET_DEC_TYPE(eFrom, eType)  AUD_SetDecType(AUD_DEC_MAIN, \
                                                       eFrom, eType)
#define AUD_SET_PLAY(fBlock)            AUD_DSPCmdPlay(AUD_DEC_MAIN)
#define AUD_SET_STOP(fBlock)            AUD_DSPCmdStop(AUD_DEC_MAIN)
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

extern void AUD_SetAvSynMode(UINT8 u1DecId, AV_SYNC_MODE_T eSynMode);

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

void _SetSpdif(DRV_AUD_SPDIF_TYPE_T rSpdif)
{
    switch (rSpdif)
    {
    case DRV_SPDIF_OFF:
        AUD_DspIECConfig(AUD_DEC_MAIN, AUD_IEC_CFG_PCM, FALSE);
        break;
        
    case DRV_SPDIF_DOLBY_DIGITAL:
        AUD_DspIECConfig(AUD_DEC_MAIN, AUD_IEC_CFG_RAW, TRUE);
        break;
        
    case DRV_SPDIF_PCM:
        AUD_DspIECConfig(AUD_DEC_MAIN, AUD_IEC_CFG_PCM, TRUE);
        break;
        
    default:
        break;
    }
}

AUD_DEC_STREAM_FROM_T _AVInputSourceSelect(DRV_INPUT_SRC_T rInputSrc)
{
    AUD_INPUT_ID_T rAudInputSel;
    AUD_DEC_STREAM_FROM_T rAudDecStreamFrom = AUD_STREAM_FROM_LINE_IN;
    BOOL fInputSrcSwitch = TRUE;
    
    // Set audio input source
    switch (rInputSrc)
    {
    case DRV_IS_CVBS_1:
        rAudInputSel = AUD_INPUT_ID_COMP_VID_0;
        break;
    case DRV_IS_CVBS_2:
        rAudInputSel = AUD_INPUT_ID_COMP_VID_1;
        break;
    case DRV_IS_CVBS_3:
        rAudInputSel = AUD_INPUT_ID_COMP_VID_2;
        break;
    case DRV_IS_SVIDEO_1:
        rAudInputSel = AUD_INPUT_ID_S_VID_0;
        break;
    case DRV_IS_SVIDEO_2:
        rAudInputSel = AUD_INPUT_ID_S_VID_1;
        break;
    case DRV_IS_SVIDEO_3:
        rAudInputSel = AUD_INPUT_ID_S_VID_2;
        break;
    case DRV_IS_YPBPR_1:
        rAudInputSel = AUD_INPUT_ID_YPBPR_0;
        break;
    case DRV_IS_YPBPR_2:
        rAudInputSel = AUD_INPUT_ID_YPBPR_1;
        break;
    case DRV_IS_YPBPR_3:
        rAudInputSel = AUD_INPUT_ID_YPBPR_2;
        break;
    case DRV_IS_VGA_1:
        rAudInputSel = AUD_INPUT_ID_VGA_0;
        break;
        
    case DRV_IS_HDMI_1:
    case DRV_IS_DVI_1:
        rAudInputSel = AUD_INPUT_ID_HDMI_0;
        rAudDecStreamFrom = AUD_STREAM_FROM_HDMI;
        break;
        
    case DRV_IS_HDMI_2:
    case DRV_IS_DVI_2:
        rAudInputSel = AUD_INPUT_ID_HDMI_1;
        rAudDecStreamFrom = AUD_STREAM_FROM_HDMI;
        break;
        
    case DRV_IS_HDMI_3:
    case DRV_IS_DVI_3:
        rAudInputSel = AUD_INPUT_ID_HDMI_2;
        rAudDecStreamFrom = AUD_STREAM_FROM_HDMI;
        break;
        
    case DRV_IS_SCART_1:
        rAudInputSel = AUD_INPUT_ID_SCART_0;
        rAudDecStreamFrom = AUD_STREAM_FROM_LINE_IN;
        break;
        
    case DRV_IS_SCART_2:
        rAudInputSel = AUD_INPUT_ID_SCART_1;
        rAudDecStreamFrom = AUD_STREAM_FROM_LINE_IN;
        break;

    case DRV_IS_ATUNER_1:
        fInputSrcSwitch = FALSE;
        rAudDecStreamFrom = AUD_STREAM_FROM_ANALOG_TUNER;
        break;
        
    case DRV_IS_MPEG_1:
        fInputSrcSwitch = FALSE;
        rAudDecStreamFrom = AUD_STREAM_FROM_DIGITAL_TUNER;
        break;
        
   default:
        fInputSrcSwitch = FALSE;
        rAudDecStreamFrom = AUD_STREAM_FROM_OTHERS;
        break;
    }

    if (fInputSrcSwitch)
    {
        AUD_AvInputMuxSel(rAudInputSel);
    }
    
    return rAudDecStreamFrom;
}

DRV_TV_SYS_T _UpdateTvSys(UINT32 u4TvSysMask)
{

    if ((u4TvSysMask & AUD_TV_SYS_MASK_B) && (u4TvSysMask & AUD_TV_SYS_MASK_G))
    {
        return DRV_TV_SYS_B_G;
    }
    else if ((u4TvSysMask & AUD_TV_SYS_MASK_D) &&
             (u4TvSysMask & AUD_TV_SYS_MASK_K))
    {
        return DRV_TV_SYS_D_K;
    }
    else if (u4TvSysMask & AUD_TV_SYS_MASK_I)
    {
        return DRV_TV_SYS_I;
    }
    else if (u4TvSysMask & AUD_TV_SYS_MASK_L)
    {
        return DRV_TV_SYS_L;
    }
    else if (u4TvSysMask & AUD_TV_SYS_MASK_L_PRIME)
    {
        return DRV_TV_SYS_L1;
    }
    else if ((u4TvSysMask & AUD_TV_SYS_MASK_M) &&
             (u4TvSysMask & AUD_TV_SYS_MASK_N))
    {
        return DRV_TV_SYS_M_N;
    }
    return DRV_TV_SYS_UNKNOWN;
}

DRV_SOUND_SYS_T _UpdateSoundSys(UINT32 u4AudSysMask)
{
    switch (u4AudSysMask)
    {
    case AUD_SYS_AM:
        return DRV_SOUND_SYS_AM;

    case AUD_SYS_FM_MONO:
        return DRV_SOUND_SYS_FM_MONO;

    case AUD_SYS_MASK_FM_EIA_J:
        return DRV_SOUND_SYS_FM_EIA_J;

    case AUD_SYS_MASK_FM_A2:
        return DRV_SOUND_SYS_FM_A2;

    case AUD_SYS_MASK_FM_A2_DK1:
        return DRV_SOUND_SYS_FM_A2_DK1;

    case AUD_SYS_MASK_FM_A2_DK2:
        return DRV_SOUND_SYS_FM_A2_DK2;

    case AUD_SYS_FM_RADIO:
        return DRV_SOUND_SYS_FM_RADIO;

    case (AUD_SYS_MASK_FM_MONO | AUD_SYS_MASK_NICAM):
        return DRV_SOUND_SYS_FM_MONO_NICAM;

    case AUD_SYS_MASK_BTSC:
        return DRV_SOUND_SYS_BTSC;
    }
    
    return DRV_SOUND_SYS_BTSC;
}

TV_AUD_SYS_T _TransferTvMode(DRV_TV_SYS_T rTvSys, DRV_SOUND_SYS_T rSoundSys)
{
    switch (rTvSys)
    {
    case DRV_TV_SYS_B_G:
        if (rSoundSys == DRV_SOUND_SYS_FM_A2)
        {
            return SV_A2_BG;
        }
        else if (rSoundSys == DRV_SOUND_SYS_FM_MONO_NICAM)
        {
            return SV_PAL_BG;
        }
        break;
        
    case DRV_TV_SYS_D_K:
        if (rSoundSys == DRV_SOUND_SYS_FM_A2_DK1)
        {
            return SV_A2_DK1;
        }
        else if (rSoundSys == DRV_SOUND_SYS_FM_A2_DK2)
        {
            return SV_A2_DK2;
        }
        else if (rSoundSys == DRV_SOUND_SYS_FM_A2)
        {
            return SV_A2_DK3;
        }
        else if (rSoundSys == DRV_SOUND_SYS_FM_MONO_NICAM)
        {
            return SV_PAL_DK;
        }
        break;
        
    case DRV_TV_SYS_M_N:
        if (rSoundSys == DRV_SOUND_SYS_BTSC)
        {
            return SV_MTS;
        }
        else if (rSoundSys == DRV_SOUND_SYS_FM_EIA_J)
        {
            return SV_FM_FM;
        }
        else if (rSoundSys == DRV_SOUND_SYS_FM_A2)
        {
            return SV_NTSC_M;
        }
        break;
        
    case DRV_TV_SYS_I:
        if (rSoundSys == DRV_SOUND_SYS_FM_MONO_NICAM)
        {
            return SV_PAL_I;
        }
        break;
        
    case DRV_TV_SYS_L:
        if (rSoundSys == DRV_SOUND_SYS_FM_MONO_NICAM)
        {
            return SV_SECAM_L;
        }
        break;
    case DRV_TV_SYS_L1:
        if (rSoundSys == DRV_SOUND_SYS_FM_MONO_NICAM)
        {
            return SV_SECAM_L_PLUM;
        }
        break;
    }

    return SV_NONE_DETECTED;
}

void _DRV_AUD_TvDetectDone(void)
{
    VERIFY(x_sema_unlock(_hTvDetectDoneSema) == OSR_OK);
}

void _DRV_AUD_WaitTvDetect(void)
{
    VERIFY(x_sema_lock(_hTvDetectDoneSema, X_SEMA_OPTION_WAIT) == OSR_OK);
}

void _WaitTvDetectThread(void *pvArg)
{
    while (1)
    {
        // Wait TV system detection done
        _DRV_AUD_WaitTvDetect();
    
        // Notify the application the TV system type is detected
        if (_rAudioParam.pfnDetectDone != NULL)
        {
            _rAudioParam.pfnDetectDone(AUD_OK);
        }
        
        if (_fIsAudTvDetected)
        {
            VERIFY(_DRV_AUD_Stop() == DRV_E_OK);
        }
    }
}

//-------------------------------------------------------------------------
/** _DRV_AUD_TvSysDectNfy  
 *  TV audio system detection notification
 *  @param  
 *  @retval  NONE
 */
//-------------------------------------------------------------------------
void _DRV_AUD_Notify(void *pvNfyTag, AUD_COND_NFY_T eNfyCond,
                     UINT32 u4Data1, UINT32 u4Data2)
{
    switch (eNfyCond) {
    case AUD_COND_TV_SYS_DETECTED:
        _fTvSysDetected = TRUE;
        _rAudioTvSys = _UpdateTvSys(u4Data1);
        _rAudioSoundSys = _UpdateSoundSys(u4Data2);
        _DRV_AUD_TvDetectDone();
        break;

    case AUD_COND_CTRL_DONE:
        if (u4Data1 == AUD_CMD_PLAY)
        {
            // Notify the application the audio play command is done
            if (_rAudioParam.pfnPlayDone!= NULL)
            {
                _rAudioParam.pfnPlayDone(AUD_OK);
            }
        }
        else if (u4Data1 == AUD_CMD_STOP)
        {
            // Notify the application the audio stop command is done
            if (_rAudioParam.pfnStopDone!= NULL)
            {
                _rAudioParam.pfnStopDone(AUD_OK);
            }
        }
        break;
    }
}

//-----------------------------------------------------------------------------
// Exported functionss
//-----------------------------------------------------------------------------

DRV_ERR_CODE_T _DRV_AUD_Init(void)
{
    INT32 i4InitResult;
    UINT32 u4Handler = NULL;
        
    _rAudioInternal.u1State = DRV_AUD_STATE_STOP;
    _rAudioInternal.u1Pidx = 0;

    x_memset(&_rAudioInternal, 0, sizeof(DRV_AUD_INTERNAL_T));
    x_memset(&_rInterAudioParam, 0, sizeof(DRV_AUD_PARAM_T));
    x_memset(&_rAudioParam, 0, sizeof(DRV_AUD_PARAM_T));
    _rAudioParam.u2Pid= 0;
    _rAudioParam.u2PcrPid = 0;
    _rAudioParam.rInputSrc = DRV_IS_MPEG_1;
    _rAudioParam.rCodingType = DRV_AUD_CT_AUTO;
    _rAudioParam.pfnDetectDone = NULL;
    _rAudioParam.pfnPlayDone = NULL;
    _rAudioParam.pfnStopDone = NULL;
    _rAudioTvSys = DRV_TV_SYS_UNKNOWN;
    _rAudioSoundSys = DRV_SOUND_SYS_FM_MONO;
    _fIsAudTvDetected = FALSE;
    
    VERIFY(x_sema_create(&_hTvDetectDoneSema, X_SEMA_TYPE_BINARY,
                         X_SEMA_STATE_LOCK) == OSR_OK);
    VERIFY(x_thread_create(&_hTvDetectThread, "AudTvDect",
                AUD_TV_DECT_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
                _WaitTvDetectThread, sizeof(UINT32), NULL) == OSR_OK);

    // Register Notify function
    AUD_RegNotifyFunc(&u4Handler, _DRV_AUD_Notify);

    i4InitResult = AUD_Init();
    ADAC_SpeakerEnable(FALSE);
    AUD_DspBassBoostEnable(AUD_DEC_MAIN, TRUE);
    AUD_DspClearBoostEnable(AUD_DEC_MAIN, TRUE);
        
    // Set audio to stereo mode
    AUD_DspSpeakerOutputConfig(AUD_DEC_MAIN, 2); // 2/0 LtRt
    AUD_DspMonoDownMix(AUD_DEC_MAIN, FALSE);
        
    return (i4InitResult == AUD_OK) ? DRV_E_OK : DRV_E_FAIL;
}

DRV_ERR_CODE_T _DRV_AUD_SetParameter(DRV_AUD_PARAM_T* prParam)
{
    VERIFY(prParam != NULL);
    
    x_memcpy(&_rInterAudioParam, prParam, sizeof(DRV_AUD_PARAM_T));
    
    return DRV_E_OK;    
}

DRV_ERR_CODE_T _DRV_AUD_GetInfo(DRV_AUD_INFO_T* prInfo)
{
    VERIFY(prInfo != NULL);
    
    prInfo->rCodingType = _rAudioParam.rCodingType;
    prInfo->rInputSrc   = _rAudioParam.rInputSrc;
    prInfo->rTvSys      = _rAudioTvSys;
    prInfo->rSoundSys   = _rAudioSoundSys;
    
    return DRV_E_OK;    
}

DRV_ERR_CODE_T _DRV_AUD_Play(DRV_AUD_PARAM_T* prParam, BOOL fIsTvDetected)
{
    AUD_DEC_STREAM_FROM_T rDecStreamFrom;
    DRV_AUD_PARAM_T *prPlayParam;
    
    if (_rAudioInternal.u1State == DRV_AUD_STATE_PLAY)
    {
        // Audio is in the play state already 
        return DRV_E_FAIL;
    }

    x_memcpy(&_rAudioParam, &_rInterAudioParam, sizeof(DRV_AUD_PARAM_T));
    
    prPlayParam = (prParam == NULL) ? &_rAudioParam : prParam;
    _fIsAudTvDetected = fIsTvDetected;

    // Reset status of Tv system detection
    _rNotifyLock = x_crit_start();
    _fTvSysDetected = FALSE;
    x_crit_end(_rNotifyLock);
    
    AUD_SetAvSynMode(AUD_DEC_MAIN, AV_SYNC_SLAVE);
    rDecStreamFrom = _AVInputSourceSelect(prPlayParam->rInputSrc);
    if (_fIsAudTvDetected)
    {
        if (AUD_OK != AUD_SetDecType(AUD_DEC_MAIN, rDecStreamFrom,
                                     (AUD_FMT_T)prPlayParam->rCodingType))
        {
            return DRV_E_FAIL;
        }
        
        AUD_DSPCmdPlayAsyn(AUD_DEC_MAIN);
    }
    else
    {
        if (AUD_OK != AUD_SET_DEC_TYPE(rDecStreamFrom, 
                                       (AUD_FMT_T)prPlayParam->rCodingType))
        {
            return DRV_E_FAIL;
        }
        
        AUD_SET_PLAY(TRUE);
    }
    
    if ((prPlayParam->rInputSrc == DRV_IS_MPEG_1) ||
        (prPlayParam->rInputSrc == DRV_IS_MPEG_2))
    {
        UINT32 au4BufStart[2], au4BufEnd[2];
        DMX_PID_T rPid;
        
        VERIFY(AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0], 
                              &au4BufStart[1], &au4BufEnd[1]) == AUD_OK);

        rPid.u1TsIndex = 0;
        rPid.u1DeviceId = 0;
        rPid.u1KeyIndex = 0;
        rPid.fgEnable = TRUE;
        rPid.u1SteerMode = DMX_STEER_TO_FTUP;
        rPid.ePcrMode = DMX_PCR_MODE_NONE;
        rPid.eDescMode = DMX_DESC_MODE_NONE;
        rPid.pfnNotify = NULL;
        rPid.pvNotifyTag = NULL;
        rPid.ePidType = DMX_PID_TYPE_ES_AUDIO;
        rPid.u4BufAddr = au4BufStart[rPid.u1DeviceId];
        rPid.u4BufSize = 
            au4BufEnd[rPid.u1DeviceId] - au4BufStart[rPid.u1DeviceId];
        rPid.fgAllocateBuffer = FALSE;
        rPid.u2Pid = prPlayParam->u2Pid;
        
        if (!_DMX_AllocatePid(&_rAudioInternal.u1Pidx))
        {
            if (_fIsAudTvDetected)
            {
                VERIFY(AUD_DSPCmdStopAsyn(AUD_DEC_MAIN) == AUD_OK);
            }
            else
            {
                
                VERIFY(AUD_SET_STOP(TRUE) == AUD_OK);
            }
            return DRV_E_FAIL;
        }

        if(!DMX_SetPid(_rAudioInternal.u1Pidx, DMX_PID_FLAG_ALL, &rPid))
        {
            if (_fIsAudTvDetected)
            {
                VERIFY(AUD_DSPCmdStopAsyn(AUD_DEC_MAIN) == AUD_OK);
            }
            else
            {
                
                VERIFY(AUD_SET_STOP(TRUE) == AUD_OK);
            }
            return DRV_E_FAIL;
        }

        _DRV_PCR_TrySyncAudio();
    }

    _rAudioInternal.u1State = DRV_AUD_STATE_PLAY;
    return DRV_E_OK;    
}

DRV_ERR_CODE_T _DRV_AUD_Stop(void)
{
    x_memcpy(&_rAudioParam, &_rInterAudioParam, sizeof(DRV_AUD_PARAM_T));

    if (_fIsAudTvDetected) 
    {
        VERIFY(AUD_DSPCmdStop(AUD_DEC_MAIN) == AUD_OK);
    }
    else
    {
        VERIFY(AUD_SET_STOP(TRUE) == AUD_OK);
    }
    
    if ((_rAudioParam.rInputSrc == DRV_IS_MPEG_1) ||
        (_rAudioParam.rInputSrc == DRV_IS_MPEG_2))
    {
        DMX_PID_T rPid;
        
        rPid.fgEnable = FALSE;
        VERIFY(DMX_SetPid(_rAudioInternal.u1Pidx, DMX_PID_FLAG_VALID, &rPid));
        
        _DRV_PCR_Unsync();

        VERIFY(DMX_FreePid(_rAudioInternal.u1Pidx));
        _DMX_ReleasePid(_rAudioInternal.u1Pidx);          
    }
    
    _fIsAudTvDetected = FALSE;
    _rAudioInternal.u1State = DRV_AUD_STATE_STOP;            
    return DRV_E_OK;    
}

DRV_ERR_CODE_T _DRV_AUD_Mute(BOOL fEnable)
{
    static BOOL fgInit = FALSE;
    if (fEnable)
    {
        AUD_Mute(AUD_DEC_MAIN);
    }
    else
    {      
        AUD_UnMute(AUD_DEC_MAIN);
        if(!fgInit)
        {
            fgInit = TRUE;
            ADAC_SpeakerEnable(TRUE);
    	    ADAC_Mute(FALSE);          
        }
    }
    
    return DRV_E_OK;    
}

DRV_ERR_CODE_T _DRV_AUD_DetectTvSys(void)
{
    DRV_AUD_PARAM_T rTvDetectParam;
    DRV_ERR_CODE_T rRetVal = DRV_E_OK;

    // Keep the original parameter setting
    x_memcpy(&rTvDetectParam, &_rInterAudioParam, sizeof(DRV_AUD_PARAM_T));
    rTvDetectParam.rCodingType = (DRV_AUD_CODING_TYPE_T)AUD_FMT_DETECTOR;

    if (_rAudioTvSys == DRV_TV_SYS_L)
    {
        AUD_DspSetDetectDefaultMode(4); // SECAM-L
    }
    else
    {
        AUD_DspSetDetectDefaultMode(0); // PAL-DK
    }

    rRetVal = _DRV_AUD_Play(&rTvDetectParam, TRUE);
    return rRetVal;
}

DRV_ERR_CODE_T _DRV_AUD_SetTvSys(DRV_TV_SYS_T rTvSys, DRV_SOUND_SYS_T rSoundSys)
{
    TV_AUD_SYS_T rAudSys;
    
    rAudSys = _TransferTvMode(rTvSys, rSoundSys);
    if (rAudSys != SV_NONE_DETECTED)
    {
        AUD_DspSetTvSrcMode(rAudSys);
        return DRV_E_OK;
    }
    else
    {
        return DRV_E_FAIL;
    }
}

DRV_ERR_CODE_T _DRV_AUD_GetTvSys(DRV_TV_SYS_T* prTvSys,
                                 DRV_SOUND_SYS_T* prSoundSys)
{
    BOOL fTvDetectDone;
    DRV_ERR_CODE_T rRetVal = DRV_E_OK;

    _rNotifyLock = x_crit_start();
    fTvDetectDone = _fTvSysDetected;
    x_crit_end(_rNotifyLock);
        
    if (fTvDetectDone)
    {
        MW_TV_AUD_SYS_T rTvAudSys;
        
        AUD_DspGetAtvTvSys(&rTvAudSys);
        *prTvSys = _UpdateTvSys(rTvAudSys.u4TvSysMask);
        *prSoundSys = _UpdateSoundSys(rTvAudSys.u4AudSysMask);
    }
    else
    {
        rRetVal = DRV_E_FAIL;
    }
    
    return rRetVal;
}

DRV_ERR_CODE_T _DRV_AUD_SetQuality(UINT32 u4Mask, DRV_AUD_QUALITY_T* prQuality)
{
    VERIFY(prQuality != NULL);

    if (u4Mask & DRV_AUD_QTYMSK_VOLUME)
    {
        AUD_DspMasterVolume(AUD_DEC_MAIN, prQuality->u1Volume);
    }

    if (u4Mask & DRV_AUD_QTYMSK_BASS)
    {
        //AUD_DspBassBoostGain(AUD_DEC_MAIN, GET_BASS_MAP_VAL(prQuality->u1Bass));
        AUD_DspBassBoostGain(AUD_DEC_MAIN, prQuality->u1Bass);
    }
    
    if (u4Mask & DRV_AUD_QTYMSK_TREBLE)
    {
        //AUD_DspClearBoostGain(AUD_DEC_MAIN,
        //                      GET_TREBLE_MAP_VAL(prQuality->u1Treble));
        AUD_DspClearBoostGain(AUD_DEC_MAIN,
                              prQuality->u1Treble);
        
    }
    
    if (u4Mask & DRV_AUD_QTYMSK_BALANCE)
    {
        AUD_DspBalance(AUD_DEC_MAIN, prQuality->u1Balance);
    }
    
    if (u4Mask & DRV_AUD_QTYMSK_REVERB)
    {
        AUD_DspReverbMode(AUD_DEC_MAIN, prQuality->u1Reverb);
    }
    
    if (u4Mask & DRV_AUD_QTYMSK_EQUALIZER)
    {
        AUD_DspChEqLoadPresetChgMode(AUD_DEC_MAIN,
                                     (AUD_EQ_TYPE_T)prQuality->u1Equalizer);
    }
    
    if (u4Mask & DRV_AUD_QTYMSK_SURROUND)
    {
        AUD_DspVirtualSurroundEnalbe(AUD_DEC_MAIN,
                                     (BOOL)prQuality->u1Surround);
    }
    
    if (u4Mask & DRV_AUD_QTYMSK_AUTO_VOL_CTRL)
    {
        AUD_DspAVCEnable(AUD_DEC_MAIN, (BOOL)prQuality->u1AutoVolumeCtrl);
    }
    
    if (u4Mask & DRV_AUD_QTYMSK_SPEAKER)
    {
        ADAC_SpeakerEnable((BOOL)prQuality->u1SpeakerOn);
    }
    
    if (u4Mask & DRV_AUD_QTYMSK_SPDIF)
    {
        _SetSpdif((DRV_AUD_SPDIF_TYPE_T)prQuality->u1SpdifType);
    }
    
    return DRV_E_OK;
}

DRV_ERR_CODE_T _DRV_AUD_GetParameter(DRV_AUD_PARAM_T** prParam)
{
    *prParam = &_rAudioParam;
    return DRV_E_OK;
}

DRV_ERR_CODE_T _DRV_AUD_GetInternal(DRV_AUD_INTERNAL_T** prInternal)
{
    *prInternal = &_rAudioInternal;
    return DRV_E_OK;
}

//-----------------------------------------------------------------------------
/** _DRV_AUD_GetState
 *  Get audio API state
 *
 *  @param  void
 *
 *  @retval DRV_E_OK	        Succeed
 *  @retval DRV_E_FAIL	        Fail
 */
//-----------------------------------------------------------------------------
UINT32 _DRV_AUD_GetState(UINT8 u1VideoIndex)
{
    return _rAudioInternal.u1State;
}
