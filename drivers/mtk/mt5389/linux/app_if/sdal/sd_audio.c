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
 * $RCSfile: sd_audio.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file sd_audio.c
 *  SDAL kernel driver implementation: Sound
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

//#include "x_typedef.h"
#include "x_util.h"
#include "aud_if.h"
#include "SdType.h"
#include "SdSound.h"
#include "SdAVDec.h"
#include "SdAuxOut.h"
#include "sd_audio.h"
#include "x_assert.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//#define CONNECT_SEMA_PROTECT
#define APP_EXT_IN_WORKAROUND
#define CONNECT_LOCK_CHECK

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define AMP_GAIN_MAX 100
#define SS_VOL_MAX   8192
#define VOL_MAX      100
#define EQ_LVL_MAX   20

#define PLAY_AUI_PROTECT_TIME_OUT  5000
#define SYS_DETECT_TIME_OUT 1000
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

static SdSound_Settings_t _arSoundSetInfo[AUD_DEC_NUM];

#define AUD_POINTER_VALIDATE(pPara)          \
    if (pPara == NULL)                       \
    {                                        \   
        return SD_ERR_INV;                   \   
    }                                        \   

#define AUD_SOUND_ID_VALIDATE(eSoundId)      \
    if (eSoundId >= SD_SOUND_MAX)            \
    {                                        \   
        return SD_ERR_INV;                   \
    }                                        \   

#define AUD_SPEAKER_ID_VALIDATE(eSpkId)      \
    if (eSpkId >= SD_SPEAKER_MAX)            \
    {                                        \   
        return SD_ERR_INV;                   \
    }                                        \ 

#define AUD_BACK_END_ID_VALIDATE(eBackEndId)   \
    if (eBackEndId >= SD_BE_IN_MAX)            \
    {                                          \   
        return SD_ERR_INV;                     \
    }                                          \   

#define BOOL_VALIDATE(fgBool)                   \
    if ((fgBool != TRUE) && (fgBool != FALSE))  \
    {                                           \
        return SD_ERR_INV;                      \
    }                                           \

#define SOUND_ID_TO_DEC_ID(SdSoundID_k)                         \
    (SdSoundID_k == SD_SOUND1) ? AUD_DEC_AUX : AUD_DEC_MAIN     \


#define AV_DEC_TO_DEC_ID(eAVDecId)                           \
    (eAVDecId == SD_AVDEC1) ? AUD_DEC_AUX : AUD_DEC_MAIN     \


#define IS_SET_VOL(u1DecId, pSettings)                      \
    (pSettings->volume != _arSoundSetInfo[u1DecId].volume)  \
    ? TRUE : FALSE                                          \
    
#define IS_SET_BALANCE(u1DecId, pSettings)                   \
    (pSettings->balance != _arSoundSetInfo[u1DecId].balance)  \
    ? TRUE : FALSE                                           \

#define IS_SET_SPK_DELAY(u1DecId, pSettings)                            \
    (pSettings->speakerDelay != _arSoundSetInfo[u1DecId].speakerDelay)  \
    ? TRUE : FALSE                                                      \

#define IS_SET_HEADPHONE_DELAY(u1DecId, pSettings)                      \
    (pSettings->headsetDelay != _arSoundSetInfo[u1DecId].headsetDelay)  \
    ? TRUE : FALSE                                                      \

#define IS_SET_AUX_ANALOG_DELAY(u1DecId, pSettings)                         \
    (pSettings->auxAnalogDelay != _arSoundSetInfo[u1DecId].auxAnalogDelay)  \
    ? TRUE : FALSE                                                          \

#define IS_SET_AGC(u1DecId, pSettings)                                  \
    (pSettings->bAutoVolume != _arSoundSetInfo[u1DecId].bAutoVolume)    \
    ? TRUE : FALSE                                                      \

#define IS_SET_BBE(u1DecId, pSettings)                  \
    (pSettings->bBBE != _arSoundSetInfo[u1DecId].bBBE)  \
    ? TRUE : FALSE                                      \

#define IS_SET_DNSE_MODE(u1DecId, pSettings)                            \
    (pSettings->eDNSeMode != _arSoundSetInfo[u1DecId].eDNSeMode)        \
    ? TRUE : FALSE                                                      \

#define IS_SET_MTS_MODE(u1DecId, pSettings)                             \
    (pSettings->eOutputAnalogMts != _arSoundSetInfo[u1DecId].eOutputAnalogMts)  \
    ? TRUE : FALSE                                                      \

#define IS_SET_TSXT_MODE(u1DecId, pSettings)                            \
    (pSettings->eTsxtMode != _arSoundSetInfo[u1DecId].eTsxtMode)  \
    ? TRUE : FALSE                                                      \

#define IS_SET_AMP_GAIN(u1DecId, pSettings)                             \
    (pSettings->eAmpGainLevel != _arSoundSetInfo[u1DecId].eAmpGainLevel)  \
    ? TRUE : FALSE                                                      \



#define SD_AUD_SEMA_LOCK()      VERIFY(x_sema_lock(_hSdAudSema,\
                                                   X_SEMA_OPTION_WAIT) == OSR_OK)
#define SD_AUD_SEMA_UNLOCK()    VERIFY(x_sema_unlock(_hSdAudSema) == OSR_OK)
#define SD_AUD_SEMA_INIT()      VERIFY(x_sema_create(&_hSdAudSema,\
                                            X_SEMA_TYPE_MUTEX,\
                                            X_SEMA_STATE_UNLOCK) == OSR_OK)

#ifdef CONNECT_SEMA_PROTECT

#define SD_AUD_CONNECT_SEMA_LOCK(u1DecId)      VERIFY(x_sema_lock(_ahSdAudConnectSema[u1DecId],\
                                                   X_SEMA_OPTION_WAIT) == OSR_OK)
#define SD_AUD_CONNECT_SEMA_UNLOCK(u1DecId)    VERIFY(x_sema_unlock(_ahSdAudConnectSema[u1DecId]) == OSR_OK)
#define SD_AUD_CONNECT_SEMA_INIT(u1DecId)      VERIFY(x_sema_create(&_ahSdAudConnectSema[u1DecId],\
                                                      X_SEMA_TYPE_BINARY,\
                                                      X_SEMA_STATE_UNLOCK) == OSR_OK)
#endif

// This semaphore is used to protect SdSound_PlayAUI and DTV/ATV/Ext-In play
#define SD_AUD_ON_PLAY_SEMA_LOCK()      VERIFY(x_sema_lock(_hSdAudOnPlaySema,\
                                                   X_SEMA_OPTION_WAIT) == OSR_OK)
#define SD_AUD_ON_PLAY_SEMA_UNLOCK()    VERIFY(x_sema_unlock(_hSdAudOnPlaySema) == OSR_OK)
#define SD_AUD_ON_PLAY_SEMA_INIT()      VERIFY(x_sema_create(&_hSdAudOnPlaySema,\
                                                      X_SEMA_TYPE_BINARY,\
                                                      X_SEMA_STATE_UNLOCK) == OSR_OK)
                                                      
// This semaphore is used to check if sound system detection done
//#define SD_SYS_SEMA_LOCK()      VERIFY(x_sema_lock(_hSdSysSema,X_SEMA_OPTION_NOWAIT) == OSR_OK)
#define SD_SYS_SEMA_UNLOCK()    VERIFY(x_sema_unlock(_hSdSysSema) == OSR_OK)
#define SD_SYS_SEMA_INIT()      VERIFY(x_sema_create(&_hSdSysSema,\
                                                      X_SEMA_TYPE_BINARY,\
                                                      X_SEMA_STATE_UNLOCK) == OSR_OK)
    
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static AUD_DEC_INFOR_T  _arAudDec[AUD_DEC_NUM];
static HANDLE_T         _hSdAudSema;
static AUD_IEC_T        _aeSpdifType[AUD_DEC_NUM];
static BOOL             _fgSpdifOutEnable = TRUE;

#ifdef CONNECT_SEMA_PROTECT
static HANDLE_T         _ahSdAudConnectSema[AUD_DEC_NUM];
#endif

static HANDLE_T         _hSdAudOnPlaySema = NULL;
static HANDLE_T         _hSdSysSema;

static BOOL             _fgAUIDone = FALSE;
static BOOL             _fgAUIOnPlay = TRUE;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

extern AUD_TYPE_T DSP_GetAudChlInfo(UINT8 u1DecId);
extern void _MT8297_ChlSel(UINT8 u1Indx);
extern void vCloseIec (void);

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

static INT8 EQ_LVL_TRANSFER(UINT32 u4EqLvl)
{
    INT8 i1Level;

    if (u4EqLvl > EQ_LVL_MAX)
    {
        u4EqLvl = EQ_LVL_MAX;
    }
    // 1dB -> 0.5 dB per step
    i1Level = ((INT8)((INT8)u4EqLvl - 10)) * 2;

    return i1Level;
}

static UINT8 VOLUME_LVL_TRANSFER(UINT32 u4VolLvl)
{
#if 0
    UINT8  u1VolLvl;
    UINT32 u4VolTmp;
    
    if (u4VolLvl == 0)  
    {
        u1VolLvl = 0;
    }
    else if (u4VolLvl == SS_VOL_MAX)
    {
        u1VolLvl = VOL_MAX;
    }
    else
    {
        u4VolTmp = u4VolLvl/82;
        u1VolLvl = (UINT8)u4VolTmp;
    }

    return u1VolLvl;
#else

    return (UINT8)u4VolLvl;

#endif
}

static AUD_FMT_T _FmtTransfer(SdAudioFormat_k eSoundFmt)
{
    AUD_FMT_T eAudFmt = AUD_FMT_UNKNOWN;
    
    switch (eSoundFmt)
    {
    case SD_AUDIO_FORMAT_PCM:
        eAudFmt = AUD_FMT_PCM;
        break;
    case SD_AUDIO_FORMAT_MPEG1:
    case SD_AUDIO_FORMAT_MPEG2:    
        eAudFmt = AUD_FMT_MPEG;
        break;
    case SD_AUDIO_FORMAT_AC3:
        eAudFmt = AUD_FMT_AC3;
        break;
    case SD_AUDIO_FORMAT_MPEG1_L3:
        eAudFmt = AUD_FMT_MP3;
        break;
    case SD_AUDIO_FORMAT_AAC:
    case SD_AUDIO_FORMAT_DTS:
    case SD_AUDIO_FORMAT_VORBIS:
        eAudFmt = AUD_FMT_AC3;
        printk("Not support format\n");
        break;
    default:
        printk("Error audio format(%d) setup\n", (INT32)eSoundFmt);
    }

    return eAudFmt;
}

static UINT16 _DelayFromMs(UINT32 u4Delay)
{
    return (UINT16)(u4Delay*7);
}

static void _AudAvInputMuxSel(SdSource_k eSource)
{
    switch(eSource)                                                                                   
    {                                                                                                           
    case SD_SOURCE_AV1:
        AUD_AvInputMuxSel(AUD_INPUT_ID_COMP_VID_0);
        break;
    case SD_SOURCE_AV2:                                                                           
        AUD_AvInputMuxSel(AUD_INPUT_ID_COMP_VID_1);
        break;
    case SD_SOURCE_AV3: 
        AUD_AvInputMuxSel(AUD_INPUT_ID_COMP_VID_2);
        break;
    case SD_SOURCE_AV4:                                                                           
        AUD_AvInputMuxSel(AUD_INPUT_ID_COMP_VID_3);
        break;
    case SD_SOURCE_SVIDEO1:                    
        AUD_AvInputMuxSel(AUD_INPUT_ID_S_VID_0);
        break;
    case SD_SOURCE_SVIDEO2:                                                                       
        AUD_AvInputMuxSel(AUD_INPUT_ID_S_VID_1);
        break;    
    case SD_SOURCE_SVIDEO3:                  
        AUD_AvInputMuxSel(AUD_INPUT_ID_S_VID_2);
        break;    
    case SD_SOURCE_COMP1:                        
        AUD_AvInputMuxSel(AUD_INPUT_ID_YPBPR_0);
        break;    
    case SD_SOURCE_COMP2:                        
        AUD_AvInputMuxSel(AUD_INPUT_ID_YPBPR_1);
        break;    
    case SD_SOURCE_COMP3:                       
        AUD_AvInputMuxSel(AUD_INPUT_ID_YPBPR_2);
        break;
    case SD_SOURCE_PC1:                           
        AUD_AvInputMuxSel(AUD_INPUT_ID_VGA_0);
        break;        
    case SD_SOURCE_PC2:                           
        AUD_AvInputMuxSel(AUD_INPUT_ID_VGA_1);
        break;                                                                                                       
    case SD_SOURCE_DVI1:
        AUD_AvInputMuxSel(AUD_INPUT_ID_DVI_0);
        break; 
    case SD_SOURCE_DVI2:
        AUD_AvInputMuxSel(AUD_INPUT_ID_DVI_1);
        break; 
    case SD_SOURCE_DVI3:
        AUD_AvInputMuxSel(AUD_INPUT_ID_DVI_2);
        break;                                                                                   
    case SD_SOURCE_HDMI1:
        AUD_AvInputMuxSel(AUD_INPUT_ID_HDMI_0);
        break;                                                                                   
    case SD_SOURCE_HDMI2:
        AUD_AvInputMuxSel(AUD_INPUT_ID_HDMI_1);
        break;                                                                                   
    case SD_SOURCE_HDMI3:
        AUD_AvInputMuxSel(AUD_INPUT_ID_HDMI_2);
        break;
    case SD_SOURCE_SCART1:
        AUD_AvInputMuxSel(AUD_INPUT_ID_SCART_0);
        break;
    case SD_SOURCE_SCART2:
        AUD_AvInputMuxSel(AUD_INPUT_ID_SCART_1);
        break;
    case SD_SOURCE_SCART3:
        AUD_AvInputMuxSel(AUD_INPUT_ID_SCART_2);
        break;
    case SD_SOURCE_SCART4:
        AUD_AvInputMuxSel(AUD_INPUT_ID_SCART_3);
        break;                                                                               
    case SD_SOURCE_COMP4:                        
    case SD_SOURCE_SVIDEO4:                                                                       
    case SD_SOURCE_PC3:   
    case SD_SOURCE_PC4:   
    case SD_SOURCE_DVI4:  
    case SD_SOURCE_HDMI4:    
    default:
        // Not support
        printk("Not support AvInputMux\n");                                                                                                   
    }
}    

static void _AudMiscAvInputMuxSel(SoundInputPort_k eSoundInput)
{
    switch (eSoundInput)                                                                                   
    {                                                                                                           
    case SIP_AIN0:
        _MT8297_ChlSel(0);
        break;
    case SIP_AIN1:                                                                           
        _MT8297_ChlSel(1);
        break;
    case SIP_AIN2: 
        _MT8297_ChlSel(2);
        break;
    case SIP_AIN3:                                                                           
        _MT8297_ChlSel(3);
        break;
    case SIP_AIN4:                    
        _MT8297_ChlSel(4);
        break;
    case SIP_AIN5:                                                                       
        _MT8297_ChlSel(5);
        break;    
    case SIP_AIN6:                  
        _MT8297_ChlSel(6);
        break;     
    default:
        // Not support
        _MT8297_ChlSel(0);
        //printk("Not support AvInputMux\n");                                                                                                   
    }
}    

static void _AudSetEqLevel(UINT8 u1DecId, SdSound_Settings_t* pSettings)
{
    UINT8 u1BandChgNum = 0;
    AUD_POINTER_VALIDATE(pSettings);

    // Check just individual band change or a set of band change
    if (pSettings->uEQ.sEqLevel5.eqBand100Hz != _arSoundSetInfo[u1DecId].uEQ.sEqLevel5.eqBand100Hz)  
    {
        u1BandChgNum++;
    }
    if (pSettings->uEQ.sEqLevel5.eqBand300Hz != _arSoundSetInfo[u1DecId].uEQ.sEqLevel5.eqBand300Hz)  
    {
        u1BandChgNum++;
    }
    if (pSettings->uEQ.sEqLevel5.eqBand1KHz != _arSoundSetInfo[u1DecId].uEQ.sEqLevel5.eqBand1KHz)  
    {
        u1BandChgNum++;
    }
    if (pSettings->uEQ.sEqLevel5.eqBand3KHz != _arSoundSetInfo[u1DecId].uEQ.sEqLevel5.eqBand3KHz)  
    {
        u1BandChgNum++;
    }
    if (pSettings->uEQ.sEqLevel5.eqBand10KHz != _arSoundSetInfo[u1DecId].uEQ.sEqLevel5.eqBand10KHz)  
    {
        u1BandChgNum++;
    }    
    
    if (u1BandChgNum == 1)  // Only one band change
    {
        if (pSettings->uEQ.sEqLevel5.eqBand100Hz != _arSoundSetInfo[u1DecId].uEQ.sEqLevel5.eqBand100Hz)  
        {
            AUD_DspSetUserDefinedEqCfg(u1DecId, AUD_EQ_CUST1, 0, EQ_LVL_TRANSFER(pSettings->uEQ.sEqLevel5.eqBand100Hz));        
        }
        if (pSettings->uEQ.sEqLevel5.eqBand300Hz != _arSoundSetInfo[u1DecId].uEQ.sEqLevel5.eqBand300Hz)  
        {
            AUD_DspSetUserDefinedEqCfg(u1DecId, AUD_EQ_CUST1, 1, EQ_LVL_TRANSFER(pSettings->uEQ.sEqLevel5.eqBand300Hz));        
        }
        if (pSettings->uEQ.sEqLevel5.eqBand1KHz != _arSoundSetInfo[u1DecId].uEQ.sEqLevel5.eqBand1KHz)  
        {
            AUD_DspSetUserDefinedEqCfg(u1DecId, AUD_EQ_CUST1, 2, EQ_LVL_TRANSFER(pSettings->uEQ.sEqLevel5.eqBand1KHz));        
        }
        if (pSettings->uEQ.sEqLevel5.eqBand3KHz != _arSoundSetInfo[u1DecId].uEQ.sEqLevel5.eqBand3KHz)  
        {
            AUD_DspSetUserDefinedEqCfg(u1DecId, AUD_EQ_CUST1, 3, EQ_LVL_TRANSFER(pSettings->uEQ.sEqLevel5.eqBand3KHz));        
        }
        if (pSettings->uEQ.sEqLevel5.eqBand10KHz != _arSoundSetInfo[u1DecId].uEQ.sEqLevel5.eqBand10KHz)  
        {
            AUD_DspSetUserDefinedEqCfg(u1DecId, AUD_EQ_CUST1, 4, EQ_LVL_TRANSFER(pSettings->uEQ.sEqLevel5.eqBand10KHz));        
        }
    }    
    else if (u1BandChgNum > 1) // A set of band change
    {
        AUD_DspSetUserDefinedSuitEqCfg(u1DecId, AUD_EQ_CUST1, EQ_LVL_TRANSFER(pSettings->uEQ.sEqLevel5.eqBand100Hz), 
                                                              EQ_LVL_TRANSFER(pSettings->uEQ.sEqLevel5.eqBand300Hz),
                                                              EQ_LVL_TRANSFER(pSettings->uEQ.sEqLevel5.eqBand1KHz),
                                                              EQ_LVL_TRANSFER(pSettings->uEQ.sEqLevel5.eqBand3KHz),
                                                              EQ_LVL_TRANSFER(pSettings->uEQ.sEqLevel5.eqBand10KHz));

    }
    
    // Save to local variables
    _arSoundSetInfo[u1DecId].uEQ.sEqLevel5.eqBand100Hz = pSettings->uEQ.sEqLevel5.eqBand100Hz;
    _arSoundSetInfo[u1DecId].uEQ.sEqLevel5.eqBand300Hz = pSettings->uEQ.sEqLevel5.eqBand300Hz;
    _arSoundSetInfo[u1DecId].uEQ.sEqLevel5.eqBand1KHz = pSettings->uEQ.sEqLevel5.eqBand1KHz;
    _arSoundSetInfo[u1DecId].uEQ.sEqLevel5.eqBand3KHz = pSettings->uEQ.sEqLevel5.eqBand3KHz;
    _arSoundSetInfo[u1DecId].uEQ.sEqLevel5.eqBand10KHz = pSettings->uEQ.sEqLevel5.eqBand10KHz;
       
}

// FIXME 
static UINT8 _AudBalance(UINT32 u4Balance)
{
    UINT8 u1Balance;
    u1Balance = (UINT8)u4Balance;
    return u1Balance;
}

static INT16 _AudAmpGain(SdSound_AmpGainLevel_k eAmpGain)
{
    INT16 i2AmpGain = AMP_GAIN_MAX;

    switch (eAmpGain)
    {
    case SD_AMP_GAIN_0:
        i2AmpGain = AMP_GAIN_MAX/5;
        break;
    case SD_AMP_GAIN_1:
        i2AmpGain = AMP_GAIN_MAX*2/5;
        break;
    case SD_AMP_GAIN_2:
        i2AmpGain = AMP_GAIN_MAX*3/5;
        break;    
    case SD_AMP_GAIN_3:
        i2AmpGain = AMP_GAIN_MAX*4/5;
        break;
    case SD_AMP_GAIN_MAX:
        i2AmpGain = AMP_GAIN_MAX;
        break;    
    default:
        break;
    }
   
    return i2AmpGain;
}

static void _AudSetMtsMode(UINT8 u1DecId, SdSound_MtsMode_k eMtsMode)
{
    AUD_SOUND_MODE_T eSoundMode = AUD_SOUND_MODE_MONO;  
    switch (eMtsMode)
    {
	case SD_MTS_MONO:
	    eSoundMode = AUD_SOUND_MODE_MONO;
	    break;
	case SD_MTS_STEREO:
	    eSoundMode = AUD_SOUND_MODE_STEREO;
	    break;
/*	    
	case SD_MTS_FOREIGN: // FIXME
	    eSoundMode = AUD_SOUND_MODE_STEREO;
	    break;
*/	    
	case SD_MTS_SAP:
	    eSoundMode = AUD_SOUND_MODE_SUB_LANG;
	    break;
/*	    
	case SD_MTS_DUAL:  // A2 dual ? FIXME
	    eSoundMode = AUD_SOUND_MODE_STEREO;
	    break;
*/	    
	case SD_MTS_STEREO_SAP: // FIXME
	    eSoundMode = AUD_SOUND_MODE_STEREO;
	    break;
/*	    
	case SD_MTS_NICAM_STEREO:
	    eSoundMode = AUD_SOUND_MODE_NICAM_STEREO;
	    break;
*/
/*
	case SD_MTS_NICAM_DUAL: // FIXME
	    eSoundMode = AUD_SOUND_MODE_NICAM_DUAL1;
	    break;
*/	
/*
	case SD_MTS_NICAM_DUAL1:
	    eSoundMode = AUD_SOUND_MODE_NICAM_DUAL1;
	    break;
*/	    
	case SD_MTS_NICAM_DUAL2:
	    eSoundMode = AUD_SOUND_MODE_NICAM_DUAL2;
	    break;
    default:
        break;
    }

    AUD_DspSetAtvOutputMode(u1DecId, eSoundMode);

}

static void _AudSetTsxtMode(UINT8 u1DecId, SdSound_TsxtMode_k eTsxtMode)
{
    UINT8 u1Mode = 2;  
    switch (eTsxtMode)
    {
	case SD_TSXT_OFF:
	    u1Mode = 0;
	    break;
	case SD_TSXT_STEREO:
	    u1Mode = 2;
	    break;
    case SD_TSXT_3D_MONO:
	    u1Mode = 1;
	    break; 
	case SD_TSXT_MAX:
	    u1Mode = 2;
	    break;
	default:
	    u1Mode = 2;
    }
    AUD_DspSetSRSTSXTSetMode(u1Mode);
}
	    
static SdSound_MtsMode_k _GetAudType(UINT8 u1DecId)
{
    AUD_TYPE_T         eAudType;
    SdSound_MtsMode_k  eSdMtsMode = SD_MTS_STEREO;

    eAudType = DSP_GetAudChlInfo(u1DecId);
    
    switch(eAudType)
    {
    case AUD_TYPE_UNKNOWN:
        break;
    case AUD_TYPE_MONO:
        eSdMtsMode = SD_MTS_MONO;
        break;
    case AUD_TYPE_MONO_SUB:
        eSdMtsMode = SD_MTS_SAP;
        break;
    case AUD_TYPE_STEREO:
        eSdMtsMode = SD_MTS_STEREO;
        break;
    case AUD_TYPE_STEREO_SUB:
        eSdMtsMode = SD_MTS_STEREO_SAP;
        break;        
    case AUD_TYPE_FM_MONO_NICAM_STEREO:
        eSdMtsMode = SD_MTS_NICAM_STEREO;
        break;
    case AUD_TYPE_FM_MONO_NICAM_DUAL: 
        eSdMtsMode = SD_MTS_NICAM_MONO;
        break;
    case AUD_TYPE_DUAL_MONO:    
    case AUD_TYPE_OTHERS:
    case AUD_TYPE_STEREO_DOLBY_SURROUND:
    case AUD_TYPE_SURROUND_2CH:
    case AUD_TYPE_SURROUND:
    case AUD_TYPE_3_0:
    case AUD_TYPE_4_0:
    case AUD_TYPE_5_1:
    case AUD_TYPE_7_1:
    case AUD_TYPE_FM_MONO_NICAM_MONO:    
        break;
    default:
        break;
    }        

    return eSdMtsMode;
}

static SdSound_SoundSys_k _AudGetSoundSys (UINT8 u1DecId, SdSound_SoundSys_k eDefaultSys )
{
    SdSound_SoundSys_k eSoundSys = SD_AUDIO_ATV_BG;
    UINT8 u1Mode; //0: PAL_DK, 4: SECAM-L.
    INT32 i4Ret;

    if (_arAudDec[u1DecId].fgOnPlay)
    {
        AUD_DSPCmdStop(u1DecId);
        _arAudDec[u1DecId].fgOnPlay = FALSE;
        printk("Invalid _SD_AUD_GetSoundSys. Decoder already on play\n");
    }

    u1Mode = (eDefaultSys == SD_AUDIO_ATV_L)? 4: 0;
    AUD_DspSetDetectDefaultMode(u1Mode);
    _arAudDec[u1DecId].eAudFmt = AUD_FMT_DETECTOR;
    AUD_SetDecType(u1DecId, AUD_STREAM_FROM_ANALOG_TUNER, AUD_FMT_DETECTOR); 

    if (!_arAudDec[u1DecId].fgOnPlay)
    {
        i4Ret = x_sema_lock(_hSdSysSema,X_SEMA_OPTION_NOWAIT);
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));

        AUD_DSPCmdPlay(u1DecId);
        _arAudDec[u1DecId].fgOnPlay = TRUE;
        
       i4Ret = x_sema_lock_timeout(_hSdSysSema, SYS_DETECT_TIME_OUT);
       VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));
       if (i4Ret == OSR_TIMEOUT)
       {
          printk("SdSound System detection timeout \n");    
       }
       
       _SD_AUD_AtvStop(u1DecId);
    }
   
   eSoundSys = (SdSound_SoundSys_k) AUD_DspGetAtvTvSystem();     
   return  eSoundSys;
    
}

static void _AudSdalNfyFunc(void *          pvNfyTag,
                          AUD_COND_NFY_T  eNfyCond,
                          UINT32          u4Data1,
                          UINT32          u4Data2) 
{
    switch (eNfyCond)
    {
    case AUD_COND_TV_SYS_DETECTED:
        SD_SYS_SEMA_UNLOCK();
    	break;
    case AUD_COND_CTRL_DONE:
    case AUD_COND_AUD_CLIP_DONE:
    case AUD_COND_MEM_BUFFER_DONE:
    case AUD_COND_FLUSH_DONE:
    case AUD_COND_FEED_ME_ON:
    case AUD_COND_FEED_ME_OFF:
    case AUD_COND_AUD_INFO_CHG:
    /* do nothing */
    	break;	
    default:
    	break;	
    }

}

void _FeederPcmSetting(UINT8 u1DecId)
{
    AUD_PCM_SETTING_T rPcmSetting;

    // Load pcm setting ..
    switch (_arAudDec[u1DecId].rPcmSetting.eFsRate)
    {
    case SD_AUDIO_FS_NONE:
        rPcmSetting.eSampleFreq = FS_44K;
        break;
    case SD_AUDIO_FS_44KHZ:
        rPcmSetting.eSampleFreq = FS_44K;
        break;
    case SD_AUDIO_FS_48KHZ:
        rPcmSetting.eSampleFreq = FS_48K;
        break;
    case SD_AUDIO_FS_32KHZ:
        rPcmSetting.eSampleFreq = FS_32K;
        break;
    case SD_AUDIO_FS_22KHZ:
        rPcmSetting.eSampleFreq = FS_22K;
        break;
    case SD_AUDIO_FS_24KHZ:
        rPcmSetting.eSampleFreq = FS_24K;
        break;    
    case SD_AUDIO_FS_88KHZ:
        rPcmSetting.eSampleFreq = FS_88K;
        break;       
    case SD_AUDIO_FS_96KHZ:
        rPcmSetting.eSampleFreq = FS_96K;
        break;   
    case SD_AUDIO_FS_192KHZ:
        rPcmSetting.eSampleFreq = FS_192K;
        break;  
    case SD_AUDIO_FS_176KHZ:
        rPcmSetting.eSampleFreq = FS_176K;
        break;     
    default:
        rPcmSetting.eSampleFreq = FS_44K;
    }

    switch(_arAudDec[u1DecId].rPcmSetting.bitWidth)
    {
    case 16:
        if (_arAudDec[u1DecId].rPcmSetting.bLittleEndian)
        {
            rPcmSetting.ePcmDataInfo = PCM_16_BITS_LITTLE_ENDIAN;
        }
        else
        {
            rPcmSetting.ePcmDataInfo = PCM_16_BITS_BIG_ENDIAN;
        }
        break;
    case 24:
        rPcmSetting.ePcmDataInfo = PCM_24_BITS;
        break;
    default:
        rPcmSetting.ePcmDataInfo = PCM_24_BITS;
    }

    AUD_PcmSetting(u1DecId, &rPcmSetting);
    
}


static void _AudSetSubwoofer(UINT8 u1DecId, SdSound_WooferSet_t * prWooferSetup)
{
    AUD_POINTER_VALIDATE(prWooferSetup);

    if (prWooferSetup->cornerFreq != _arSoundSetInfo[u1DecId].sWooferSet.cornerFreq)
    {
        // Decoder not support dynamic corner frequency change yet ..
    }
    if (prWooferSetup->volume != _arSoundSetInfo[u1DecId].sWooferSet.volume)
    {
        UINT32 u4Volume = prWooferSetup->volume;

        AUD_DspChannelVolume(u1DecId, AUD_CH_SUB_WOOFER, VOLUME_LVL_TRANSFER(u4Volume));
    }
    if (prWooferSetup->eFilterType != _arSoundSetInfo[u1DecId].sWooferSet.eFilterType)
    {
        // Decoder not support dynamic filter type change yet ..
    }
}

static INT32 _AudClipNotifyHandler(UINT8 u1DecId, UINT32 u1Datat, UINT32 u4Datat2)
{
    printk(" >>>>>>>>>> _AudClipNotifyHandler >>>>>>>>>>\n");
    // Un-Lock audio resource at the end of play AUI
    SD_AUD_ON_PLAY_SEMA_UNLOCK();
    _fgAUIDone = TRUE;

    //Put event to Callback module
    UNUSED(_CB_PutEvent(CB_SDAL_SOUNDAUI_TRIGGER, 0, 0));    
    printk(" >>>>>>>>>> _CB_PutEvent >>>>>>>>>>\n");
}

static void _SD_AUD_AvTypePlay(UINT8 u1DecId, BOOL fgIsHdmi)
{
#ifdef APP_EXT_IN_WORKAROUND
    // already on play when connect
#else
    INT32 i4Ret;

    if (_fgAUIOnPlay)
    {   
        i4Ret = x_sema_lock_timeout(_hSdAudOnPlaySema, PLAY_AUI_PROTECT_TIME_OUT);
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));
        if (i4Ret == OSR_TIMEOUT)
        {
            printk("SdSound PlayAUI still on play\n");    
        }
    }    
#endif        
    if (_arAudDec[u1DecId].fgOnPlay)
    {
        AUD_DSPCmdStop(u1DecId);
        _arAudDec[u1DecId].fgOnPlay = FALSE;
        printk("Invalid _SD_AUD_AvTypePlay\n");
    }

    _arAudDec[u1DecId].eAudFmt = AUD_FMT_PCM;
    if (fgIsHdmi)
    {
        AUD_SetDecType(u1DecId, AUD_STREAM_FROM_HDMI, AUD_FMT_PCM); 
    }
    else
    {
        AUD_SetDecType(u1DecId, AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM); 
    }
    
    _arAudDec[u1DecId].fgOnPlay = TRUE;
    //default: stereo PCM
    AUD_DspSetPCMChannelNumber(u1DecId,0);
    AUD_DSPCmdPlay(u1DecId);
}

static void _SD_AUD_AvTypeStop(UINT8 u1DecId)
{   
    _arAudDec[u1DecId].fgOnPlay = FALSE;
    AUD_DSPCmdStopAsyn(u1DecId);

    _fgAUIOnPlay = FALSE;

    SD_AUD_ON_PLAY_SEMA_UNLOCK();
}

SdResult _SD_AUD_SetAudFmt(SdSoundID_k eSoundId, SdAudioFormat_k eSoundFmt, SdAVDec_Settings_t* pSettings)
{
    UINT8 u1DecId;
    AUD_SOUND_ID_VALIDATE(eSoundId);
    u1DecId = SOUND_ID_TO_DEC_ID(eSoundId);

    SD_AUD_SEMA_LOCK();

    AUD_FMT_T eAudFmt = AUD_FMT_UNKNOWN;
    switch (eSoundFmt)
    {
    case SD_AUDIO_FORMAT_PCM:
        eAudFmt = AUD_FMT_PCM;
        break;
    case SD_AUDIO_FORMAT_MPEG1:
    case SD_AUDIO_FORMAT_MPEG2:    
        eAudFmt = AUD_FMT_MPEG;
        break;
    case SD_AUDIO_FORMAT_AC3:
        eAudFmt = AUD_FMT_AC3;
        break;
    case SD_AUDIO_FORMAT_MPEG1_L3:
        eAudFmt = AUD_FMT_MP3;
        break;
    case SD_AUDIO_FORMAT_AAC:
        eAudFmt = AUD_FMT_AAC;
        break;
    case SD_AUDIO_FORMAT_WMA:
        eAudFmt = AUD_FMT_WMA;
        break;        
    case SD_AUDIO_FORMAT_DTS:
    case SD_AUDIO_FORMAT_VORBIS:
        eAudFmt = AUD_FMT_AC3;
        printk("Not support format\n");
        break;
    default:
        printk("Error audio format(%d) setup\n", (INT32)eSoundFmt);
    }

    _arAudDec[u1DecId].eAudFmt = eAudFmt;

    if (eSoundFmt == SD_AUDIO_FORMAT_PCM)
    {
        _arAudDec[u1DecId].rPcmSetting.bitWidth = pSettings->sPCMSettings.bitWidth;
        _arAudDec[u1DecId].rPcmSetting.channels = pSettings->sPCMSettings.channels;
        _arAudDec[u1DecId].rPcmSetting.eFsRate = pSettings->sPCMSettings.eFsRate;
    }    

    // Setup SPDIF output format
    if (pSettings->audio.auxpid == -1)
    {
        _aeSpdifType[u1DecId] = AUD_IEC_CFG_PCM;
        // For SPDIF output timing fine tune issue
        //AUD_DspIECConfig(u1DecId, _aeSpdifType[u1DecId], _fgSpdifOutEnable);
    }
    else
    {
        _aeSpdifType[u1DecId] = AUD_IEC_CFG_RAW;
        // For SPDIF output timing fine tune issue 
        //AUD_DspIECConfig(u1DecId, _aeSpdifType[u1DecId], _fgSpdifOutEnable);
    }

    SD_AUD_SEMA_UNLOCK();
    
    return SD_OK;
}

void _SD_AUD_DtvPlay(UINT8 u1DecId)
{
    INT32 i4Ret;
 
    SD_AUD_SEMA_LOCK();

    if (_fgAUIOnPlay)
    {
        i4Ret = x_sema_lock_timeout(_hSdAudOnPlaySema, PLAY_AUI_PROTECT_TIME_OUT);
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));
        if (i4Ret == OSR_TIMEOUT)
        {
            printk("SdSound PlayAUI still on play\n");    
        }
    }
    
    if (_arAudDec[u1DecId].fgOnPlay)
    {
        AUD_DSPCmdStop(u1DecId);
        _arAudDec[u1DecId].fgOnPlay = FALSE;
        printk("Invalid _SD_AUD_DtvPlay. Decoder already on play\n");
    }
    
#if 0
    if ((_arAudDec[u1DecId].eAudFmt != AUD_FMT_AC3) && 
        (_arAudDec[u1DecId].eAudFmt != AUD_FMT_MPEG))
    {
        _arAudDec[u1DecId].eAudFmt = AUD_FMT_AC3;
        AUD_SetDecType(u1DecId, AUD_STREAM_FROM_DIGITAL_TUNER, AUD_FMT_AC3); 
    }
#endif

    AUD_SetDecType(u1DecId, _arAudDec[u1DecId].eStrSrc, _arAudDec[u1DecId].eAudFmt); 

    _arAudDec[u1DecId].fgOnPlay = TRUE;

    //default: stereo PCM
    AUD_DspSetPCMChannelNumber(u1DecId,0);
    
    if (_arAudDec[u1DecId].eStrSrc == AUD_STREAM_FROM_FEEDER)
    {
        _FeederPcmSetting(u1DecId);
        //for feeder mono PCM, set special channel number 9
        if (_arAudDec[u1DecId].rPcmSetting.channels == 1)
        {
            AUD_DspSetPCMChannelNumber(u1DecId,9);
        }
    }
    
    AUD_DSPCmdPlay(u1DecId);

    SD_AUD_SEMA_UNLOCK();
}

void _SD_AUD_DtvStop(UINT8 u1DecId)
{
    SD_AUD_SEMA_LOCK();
    
    _arAudDec[u1DecId].fgOnPlay = FALSE;
    AUD_DSPCmdStop(u1DecId);

    SD_AUD_ON_PLAY_SEMA_UNLOCK();

    _fgAUIOnPlay = FALSE;
    
    SD_AUD_SEMA_UNLOCK();
}

void _SD_AUD_AtvPlay(UINT8 u1DecId)
{
    INT32 i4Ret;
    SD_AUD_SEMA_LOCK();

    if (_fgAUIOnPlay)
    {    
        i4Ret = x_sema_lock_timeout(_hSdAudOnPlaySema, PLAY_AUI_PROTECT_TIME_OUT);
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));
        if (i4Ret == OSR_TIMEOUT)
        {
            printk("SdSound PlayAUI still on play\n");    
        }
    }
    
    if ((!_arAudDec[u1DecId].fgConnected))
    {
        printk("Invalid _SD_AUD_AtvPlay. Not connected\n");
        SD_AUD_SEMA_UNLOCK();
        return ;
    }

    if (_arAudDec[u1DecId].fgOnPlay)
    {
        AUD_DSPCmdStop(u1DecId);
        _arAudDec[u1DecId].fgOnPlay = FALSE;
        printk("Invalid _SD_AUD_AtvPlay. Decoder already on play\n");
    }

    //Add new API to set ATV decoder type SdSound_SetDecType
    //_arAudDec[u1DecId].eAudFmt = AUD_FMT_MTS;
    //AUD_SetDecType(u1DecId, AUD_STREAM_FROM_ANALOG_TUNER, AUD_FMT_MTS); 
    
    if (!_arAudDec[u1DecId].fgOnPlay)
    {
        AUD_DSPCmdPlay(u1DecId);
    }

    _arAudDec[u1DecId].fgOnPlay = TRUE;
    
    SD_AUD_SEMA_UNLOCK();
}

void _SD_AUD_AtvStop(UINT8 u1DecId)
{
    SD_AUD_SEMA_LOCK();
    
    _arAudDec[u1DecId].fgOnPlay = FALSE;
    AUD_DSPCmdStopAsyn(u1DecId);
    //AUD_DSPCmdStop(u1DecId);

    SD_AUD_ON_PLAY_SEMA_UNLOCK();

    _fgAUIOnPlay = FALSE;
    
    SD_AUD_SEMA_UNLOCK();
}

void _SD_AUD_ExtInPlay(UINT8 u1DecId)
{
    if (_arAudDec[u1DecId].fgUseNormalSoundConnect)
    {
        switch (_arAudDec[u1DecId].eSourcComp)
        {
        case SD_SOURCE_ATV1:
        case SD_SOURCE_ATV2:    
            _SD_AUD_AtvPlay(u1DecId);
            break;
        case SD_SOURCE_AV1:
        case SD_SOURCE_AV2:                                                                           
        case SD_SOURCE_AV3:                                                                           
        case SD_SOURCE_AV4:                                                                                                                                                                              
        case SD_SOURCE_SVIDEO1:                                                                       
        case SD_SOURCE_SVIDEO2:                                                                       
        case SD_SOURCE_SVIDEO3:                                                                       
        case SD_SOURCE_SVIDEO4:                                                                                                                                                                          
        case SD_SOURCE_COMP1:                        
        case SD_SOURCE_COMP2:                        
        case SD_SOURCE_COMP3:                        
        case SD_SOURCE_COMP4:                                                                                                                           
        case SD_SOURCE_PC1:                           
        case SD_SOURCE_PC2:                           
        case SD_SOURCE_PC3:                           
        case SD_SOURCE_PC4:                                                                                                                              
        case SD_SOURCE_DVI1:
        case SD_SOURCE_DVI2:
        case SD_SOURCE_DVI3:
        case SD_SOURCE_DVI4:
        case SD_SOURCE_PCM:    
        case SD_SOURCE_SCART1:
        case SD_SOURCE_SCART2:
        case SD_SOURCE_SCART3:
        case SD_SOURCE_SCART4:
            _SD_AUD_AvTypePlay(u1DecId, FALSE);
            break;
        case SD_SOURCE_HDMI1:
        case SD_SOURCE_HDMI2:
        case SD_SOURCE_HDMI3:
        case SD_SOURCE_HDMI4:
            _SD_AUD_AvTypePlay(u1DecId, TRUE);
            break;
        default:
            printk("Invalid _SD_AUD_ExtInPlay\n");
        }    
    }
    else  // use MISC sound connect
    {
        switch (_arAudDec[u1DecId].eSoundInput)
        {
        case SIP_SIF:
            _SD_AUD_AtvPlay(u1DecId);
            break;
        case SIP_HDMI:
            _SD_AUD_AvTypePlay(u1DecId, TRUE);
            break;
        case SIP_AIN0:
        case SIP_AIN1:
        case SIP_AIN2:
        case SIP_AIN3:
        case SIP_AIN4:
        case SIP_AIN5:
        case SIP_AIN6:    
            _SD_AUD_AvTypePlay(u1DecId, FALSE);
            break;
        default:
            printk("Invalid _SD_AUD_ExtInPlay\n");
        }    
    }

}

void _SD_AUD_ExtInStop(UINT8 u1DecId)
{
    if (_arAudDec[u1DecId].fgUseNormalSoundConnect)
    {
        switch (_arAudDec[u1DecId].eSourcComp)
        {
        case SD_SOURCE_ATV1:
        case SD_SOURCE_ATV2:    
            _SD_AUD_AtvStop(u1DecId);
            break;
        case SD_SOURCE_AV1:
        case SD_SOURCE_AV2:                                                                           
        case SD_SOURCE_AV3:                                                                           
        case SD_SOURCE_AV4:                                                                                                                                                                              
        case SD_SOURCE_SVIDEO1:                                                                       
        case SD_SOURCE_SVIDEO2:                                                                       
        case SD_SOURCE_SVIDEO3:                                                                       
        case SD_SOURCE_SVIDEO4:                                                                                                                                                                          
        case SD_SOURCE_COMP1:                        
        case SD_SOURCE_COMP2:                        
        case SD_SOURCE_COMP3:                        
        case SD_SOURCE_COMP4:                                                                                                                           
        case SD_SOURCE_PC1:                           
        case SD_SOURCE_PC2:                           
        case SD_SOURCE_PC3:                           
        case SD_SOURCE_PC4:                                                                                                                              
        case SD_SOURCE_DVI1:
        case SD_SOURCE_DVI2:
        case SD_SOURCE_DVI3:
        case SD_SOURCE_DVI4:
        case SD_SOURCE_HDMI1:
        case SD_SOURCE_HDMI2:
        case SD_SOURCE_HDMI3:
        case SD_SOURCE_HDMI4:
        case SD_SOURCE_SCART1:
        case SD_SOURCE_SCART2:
        case SD_SOURCE_SCART3:
        case SD_SOURCE_SCART4:
            _SD_AUD_AvTypeStop(u1DecId);
            break;
        default:
            printk("Invalid _SD_AUD_AvTypeStop\n");
        }    
    }
    else  // use MISC sound connect
    {
        switch (_arAudDec[u1DecId].eSoundInput)
        {
        case SIP_SIF:
            _SD_AUD_AtvStop(u1DecId);
            break;
        case SIP_HDMI:
        case SIP_AIN0:
        case SIP_AIN1:
        case SIP_AIN2:
        case SIP_AIN3:
        case SIP_AIN4:
        case SIP_AIN5:
        case SIP_AIN6:    
            _SD_AUD_AvTypeStop(u1DecId);
            break;
        default:
            printk("Invalid _SD_AUD_AvTypeStop\n");
        }    
    }

}

SdResult _SD_AUD_AuxOutMute(SdAuxOutID_k eAuxOutId, SdBool_t bMuteOn, SdAuxOut_Audio_k eAuxAudio)
{
    UINT8 u1DecId;
    BOOL_VALIDATE(bMuteOn);
    AUD_CH_T eChannel;
    	     
    switch (eAuxAudio)
    {
    case SD_AUXAUDIO_ANALOG:
        if (eAuxOutId == SD_AUXOUT0)
        {
            u1DecId = AUD_DEC_AUX;
            eChannel = AUD_CH_FRONT_LEFT;
        }
        else
        {
            u1DecId = AUD_DEC_MAIN;
            eChannel = AUD_CH_REAR_LEFT;
        }
        
        // Use channel volume to implement the mute function
        AUD_DspChannelMute(u1DecId, eChannel, bMuteOn);
        AUD_DspChannelMute(u1DecId, (AUD_CH_T)(eChannel+1), bMuteOn);
        break;
    case SD_AUXAUDIO_SPDIF:
        u1DecId = AUD_DEC_MAIN;
        eChannel = AUD_CH_BYPASS_LEFT;
        AUD_DspChannelMute(u1DecId, eChannel, bMuteOn);
        AUD_DspChannelMute(u1DecId, (AUD_CH_T)(eChannel+1), bMuteOn);

        // For SPDIF output timing fine tune issue
        // AUD_DspIECConfig(u1DecId, _aeSpdifType[u1DecId], !bMuteOn);
        _fgSpdifOutEnable = !bMuteOn;
        break;
    case SD_AUXAUDIO_ALL:
        // For SPDIF output timing fine tune issue
        // AUD_DspIECConfig(u1DecId, _aeSpdifType[u1DecId], !bMuteOn);
        _fgSpdifOutEnable = !bMuteOn;
        u1DecId = AUD_DEC_MAIN;
        eChannel = AUD_CH_BYPASS_LEFT;

        AUD_DspChannelMute(u1DecId, eChannel, bMuteOn);
        AUD_DspChannelMute(u1DecId,  (AUD_CH_T)(eChannel+1), bMuteOn);

        if (eAuxOutId == SD_AUXOUT0)
        {
            u1DecId = AUD_DEC_AUX;
            eChannel = AUD_CH_FRONT_LEFT;
        }
        else
        {
            u1DecId = AUD_DEC_MAIN;
            eChannel = AUD_CH_REAR_LEFT;
        }
        
        // Use channel volume to implement the mute function
        AUD_DspChannelMute(u1DecId, eChannel, bMuteOn);
        AUD_DspChannelMute(u1DecId, (AUD_CH_T)(eChannel+1), bMuteOn);
       
        break;
    default:
        break;
    }

    return SD_OK;
}

SdResult _SD_AUD_Init(SdSound_Step_k step, SdPowerState_k ePowerState)
{
    static BOOL fgInit = FALSE;
    INT32 i4Ret = SD_OK;
    INT32 i;
    UINT32 u4Handler = NULL;

    UNUSED(step);
    UNUSED(ePowerState);

    if (ePowerState == SD_PWROFF_TO_STANDBY)
    {
        // TODO !!
        // Close speaker .. or ampifier gpio down ..
        return SD_OK;
    }

    if (!fgInit)
    {   
        fgInit = TRUE;
    
        SD_AUD_SEMA_INIT();
        SD_AUD_SEMA_LOCK();
        SD_AUD_ON_PLAY_SEMA_INIT();
        SD_SYS_SEMA_INIT();
#ifdef CONNECT_SEMA_PROTECT
        SD_AUD_CONNECT_SEMA_INIT(AUD_DEC_MAIN);
        SD_AUD_CONNECT_SEMA_INIT(AUD_DEC_AUX);
#endif
        
        for (i=0; i<AUD_DEC_NUM; i++)
        {
        // Clear decoder info ..
        x_memset((void *)&(_arAudDec[i]), 0, sizeof(AUD_DEC_INFO_T));

        // Setup SoundSetInfo
        _arAudDec[i].prSoundSetting = &_arSoundSetInfo[i];

        // Initial value
        _arAudDec[i].fgConnected = FALSE;
        _arAudDec[i].fgOnPlay = FALSE;
        _arAudDec[i].eAudFmt = AUD_FMT_AC3;
        _arAudDec[i].eAudAtvFmt = AUD_FMT_MTS;
        _arAudDec[i].eStrSrc = AUD_STREAM_FROM_LINE_IN;

#if 0        
        // HDMI initial setting
        _arAudDec[i].rHdmiSetting.bIsPcm = TRUE;
        _arAudDec[i].rHdmiSetting.eInputFsRate = SD_HDMI_AUDIO_48KHZ; // FIXME
#endif

        // Sound status initial value
        _arAudDec[i].rSoundStatus.eAmpGainLevel = SD_AMP_GAIN_MAX;
        _arAudDec[i].rSoundStatus.eInputAnalogMts = SD_MTS_MONO;

        // Sound PCM setting 
        _arAudDec[i].rPcmSetting.bitWidth = 24;
        _arAudDec[i].rPcmSetting.channels = 2;
        _arAudDec[i].rPcmSetting.eFsRate = SD_AUDIO_FS_44KHZ;

        // Sound Info initial value    
        _arSoundSetInfo[i].volume = 0;
        _arSoundSetInfo[i].balance = 50;  // FIXME
        _arSoundSetInfo[i].adVolume = 100;
        _arSoundSetInfo[i].speakerDelay = 0;
        _arSoundSetInfo[i].headsetDelay = 0;
        _arSoundSetInfo[i].auxAnalogDelay = 0;
        _arSoundSetInfo[i].auxSpdifDelay = 0;
        _arSoundSetInfo[i].bAutoVolume = TRUE;
        _arSoundSetInfo[i].bBBE = FALSE;

        _arSoundSetInfo[i].eDNSeMode = SD_DNSE_NONE;
        _arSoundSetInfo[i].eOutputAnalogMts = SD_MTS_MONO;
        _arSoundSetInfo[i].eTsxtMode = SD_TSXT_OFF;
        _arSoundSetInfo[i].bSupportEQ7 = FALSE;
        _arSoundSetInfo[i].uEQ.sEqLevel5.eqBand100Hz = 0; 
        _arSoundSetInfo[i].uEQ.sEqLevel5.eqBand300Hz = 0; 
        _arSoundSetInfo[i].uEQ.sEqLevel5.eqBand1KHz = 0;
        _arSoundSetInfo[i].uEQ.sEqLevel5.eqBand3KHz = 0;
        _arSoundSetInfo[i].uEQ.sEqLevel5.eqBand10KHz = 0;
 
        _arSoundSetInfo[i].eAmpGainLevel = FALSE;
        _arSoundSetInfo[i].sWooferSet.volume = 100;
        _arSoundSetInfo[i].sWooferSet.cornerFreq = 100;
        _arSoundSetInfo[i].sWooferSet.eFilterType = SD_SUBWOOFER_LOUD_CHANNEL;

        _aeSpdifType[i] = AUD_IEC_CFG_RAW;

        }
        
        /* Register SDAL notify function */
        AUD_RegNotifyFunc(&u4Handler, _AudSdalNfyFunc);
        	
        AUD_DspChEqLoadPresetChgMode(AUD_DEC_MAIN, AUD_EQ_CUST1);
        i4Ret = AUD_Init(); 

        // Open audio output ..
        // ADAC_Mute(FALSE);

        // Defualt load pcm decoder ..    
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM);    
        //AUD_SetDecType(AUD_DEC_AUX, AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM);    

        // Default set to Lt/Rt downmix
        AUD_DspSpeakerOutputConfig(AUD_DEC_MAIN, 0);

        // Default set volume to 0
        _AUD_DspMasterVolShmRawValue(AUD_DEC_MAIN, 0);

        // Setup channel volume
        _AUD_DspChannelVolume(AUD_DEC_MAIN, AUD_CH_FRONT_LEFT,   100);
        _AUD_DspChannelVolume(AUD_DEC_MAIN, AUD_CH_FRONT_RIGHT,  100);
        _AUD_DspChannelVolume(AUD_DEC_MAIN, AUD_CH_BYPASS_LEFT,  100);
        _AUD_DspChannelVolume(AUD_DEC_MAIN, AUD_CH_BYPASS_RIGHT, 100);
        _AUD_DspChannelVolume(AUD_DEC_MAIN, AUD_CH_DMX_LEFT,     100);
        _AUD_DspChannelVolume(AUD_DEC_MAIN, AUD_CH_DMX_RIGHT,    100);

        // Setup mute status
        // Mute speaker output
        AUD_DspChannelMute(AUD_DEC_MAIN, AUD_CH_FRONT_LEFT, TRUE);
        AUD_DspChannelMute(AUD_DEC_MAIN, AUD_CH_FRONT_RIGHT, TRUE);
        // Mute monitor output
        AUD_DspChannelMute(AUD_DEC_MAIN, AUD_CH_BYPASS_LEFT, TRUE);
        AUD_DspChannelMute(AUD_DEC_MAIN, AUD_CH_BYPASS_RIGHT, TRUE);
        // Mute headphone output
        AUD_DspChannelMute(AUD_DEC_MAIN, AUD_CH_DMX_LEFT, TRUE);
        AUD_DspChannelMute(AUD_DEC_MAIN, AUD_CH_DMX_RIGHT, TRUE);

        // Set AC3 data rate config to default value
        AUD_SetAC3DataRateCfg(AUD_DEC_MAIN, TRUE);

        // Setup channel delay to 0
        AUD_DspChannelDelay(0, AUD_CH_ALL, AUD_DEC_MAIN);

        // Defualt set to AC3 full compression 
        AUD_DspAC3DRCRange(8, AUD_DEC_MAIN);    

        // Default set SPDIF to AC3 raw data
        AUD_DspIECConfig(AUD_DEC_MAIN, AUD_IEC_CFG_RAW, TRUE);
        
        // Default set SRSTsxt speaker size as 150 Hz
        AUD_DspSetSRSTSXTSpkSz(3);

        // Default set SRSTsxt bass boost as 20%
        DSP_SRSTSXTTBLvlRawData(0x199999);

        SD_AUD_SEMA_UNLOCK();
    }
   
    return (i4Ret == AUD_OK) ? SD_OK : SD_NOT_OK;
}

SdResult _SD_AUD_MiscConnect(SdSoundID_k eSoundId, SoundInputPort_k eSoundInput, SdBackEndInID_k eInputId)
{  
    UINT8 u1DecId;
    SdResult  eRet = SD_OK;
    AUD_SOUND_ID_VALIDATE(eSoundId);
    AUD_BACK_END_ID_VALIDATE(eInputId);
    
    u1DecId = SOUND_ID_TO_DEC_ID(eSoundId);

#ifdef CONNECT_SEMA_PROTECT
    SD_AUD_CONNECT_SEMA_LOCK(u1DecId);
#endif

#ifdef CONNECT_LOCK_CHECK
    if (_arAudDec[u1DecId].fgConnected == TRUE)
    {
        eRet = SD_ERR_LOCKED;
    }
#endif

    SD_AUD_SEMA_LOCK();

    _arAudDec[u1DecId].fgConnected = TRUE;
    _arAudDec[u1DecId].eSoundInput = eSoundInput;
    _arAudDec[u1DecId].fgUseNormalSoundConnect = FALSE;

    switch (eSoundInput)                                                                                   
    {                                                                             
    case SIP_SIF:         // Use Analog TV of Main Tuner as Source Path.  
        _arAudDec[u1DecId].eStrSrc = AUD_STREAM_FROM_ANALOG_TUNER;
        break;                                                                                     
    case SIP_ADEC:         // Use Digital TV of Main Tuner as Source Path.  
        _arAudDec[u1DecId].eStrSrc = AUD_STREAM_FROM_DIGITAL_TUNER;
        break;
    case SIP_AIN0:
    case SIP_AIN1:                                                                           
    case SIP_AIN2:                                                                           
    case SIP_AIN3:                                                                                                                                                                              
    case SIP_AIN4:                                                                       
    case SIP_AIN5:                                                                       
    case SIP_AIN6:
        _arAudDec[u1DecId].eStrSrc = AUD_STREAM_FROM_LINE_IN;
        _AudMiscAvInputMuxSel(eSoundInput);
#ifdef APP_EXT_IN_WORKAROUND            
        _SD_AUD_AvTypePlay(u1DecId, FALSE);
#endif
        break;                                                                                     
    case SIP_HDMI:
        _arAudDec[u1DecId].eStrSrc = AUD_STREAM_FROM_HDMI;
        _AudMiscAvInputMuxSel(eSoundInput);
#ifdef APP_EXT_IN_WORKAROUND            
        _SD_AUD_AvTypePlay(u1DecId, TRUE);
#endif
        break;  
    case SIP_FEEDER:      //!< Media Source, DNet, WiseLink, PVR    
        _arAudDec[u1DecId].eStrSrc = AUD_STREAM_FROM_FEEDER;
        break;
    default:
        printk("Error SdSource_k \n");
                                                                     
    }

    if (eSoundInput == SIP_ADEC)
    {
        _aeSpdifType[u1DecId] = AUD_IEC_CFG_RAW;
    }
    else
    {
        _aeSpdifType[u1DecId] = AUD_IEC_CFG_PCM;
    }

    SD_AUD_SEMA_UNLOCK();
    return eRet;
}

SdResult _SD_AUD_Connect(SdSoundID_k eSoundId, SdSource_k eSrc, SdBackEndInID_k eInputId)
{                                        
    UINT8 u1DecId;
    SdResult  eRet = SD_OK;
    AUD_SOUND_ID_VALIDATE(eSoundId);
    AUD_BACK_END_ID_VALIDATE(eInputId);

    u1DecId = SOUND_ID_TO_DEC_ID(eSoundId);

#ifdef CONNECT_SEMA_PROTECT
    SD_AUD_CONNECT_SEMA_LOCK(u1DecId);
#endif

#ifdef CONNECT_LOCK_CHECK
    if (_arAudDec[u1DecId].fgConnected == TRUE)
    {
#if 1
        // Check if on play -> stop
        // Disconnect
        // New connect

    if (_arAudDec[u1DecId].fgOnPlay)
    {
        AUD_DSPCmdStop(u1DecId);
        _arAudDec[u1DecId].fgOnPlay = FALSE;
    }

    _SD_AUD_Disconnect(eSoundId);

#else    
        eRet = SD_ERR_LOCKED;
#endif
    }
#endif

    SD_AUD_SEMA_LOCK();

    _arAudDec[u1DecId].fgConnected = TRUE;
    _arAudDec[u1DecId].eSourcComp = eSrc;
    _arAudDec[u1DecId].fgUseNormalSoundConnect = TRUE;
                                                                                                   
    switch (eSrc)                                                                                   
    {                                                                                              
    case SD_SOURCE_NONE:     
        _arAudDec[u1DecId].eStrSrc = AUD_STREAM_FROM_LINE_IN;
        break;                                                                                     
    case SD_SOURCE_ATV1:         // Use Analog TV of Main Tuner as Source Path.                                                                           
    case SD_SOURCE_ATV2:         // Use Analog TV of PIP Tuner as Source Path.                                                                           
        _arAudDec[u1DecId].eStrSrc = AUD_STREAM_FROM_ANALOG_TUNER;
        //_SD_AUD_ExtInPlay(u1DecId); //Laihui: seems need to add here??       
        break;                                                                                     
    case SD_SOURCE_DTV1:         // Use Digital TV of Main Tuner as Source Path.                 
    case SD_SOURCE_DTV2:         // Use Digital TV of PIP Tuner as Source Path.                  
        _arAudDec[u1DecId].eStrSrc = AUD_STREAM_FROM_DIGITAL_TUNER;
        break;
    case SD_SOURCE_AV1:
    case SD_SOURCE_AV2:                                                                           
    case SD_SOURCE_AV3:                                                                           
    case SD_SOURCE_AV4:                                                                                                                                                                              
    case SD_SOURCE_SVIDEO1:                                                                       
    case SD_SOURCE_SVIDEO2:                                                                       
    case SD_SOURCE_SVIDEO3:                                                                       
    case SD_SOURCE_SVIDEO4:                                                                                                                                                                          
    case SD_SOURCE_COMP1:                        
    case SD_SOURCE_COMP2:                        
    case SD_SOURCE_COMP3:                        
    case SD_SOURCE_COMP4:                                                                                                                           
    case SD_SOURCE_PC1:                           
    case SD_SOURCE_PC2:                           
    case SD_SOURCE_PC3:                           
    case SD_SOURCE_PC4:                                                                                                                              
    case SD_SOURCE_DVI1:
    case SD_SOURCE_DVI2:
    case SD_SOURCE_DVI3:
    case SD_SOURCE_DVI4:
    case SD_SOURCE_SCART1:
    case SD_SOURCE_SCART2:
    case SD_SOURCE_SCART3:
    case SD_SOURCE_SCART4:
        _arAudDec[u1DecId].eStrSrc = AUD_STREAM_FROM_LINE_IN;
        _AudAvInputMuxSel(eSrc);
#ifdef APP_EXT_IN_WORKAROUND        
        _SD_AUD_ExtInPlay(u1DecId);
#endif
        break;                                                                                     
    case  SD_SOURCE_HDMI1:
    case  SD_SOURCE_HDMI2:
    case  SD_SOURCE_HDMI3:
    case  SD_SOURCE_HDMI4:
        _arAudDec[u1DecId].eStrSrc = AUD_STREAM_FROM_HDMI;
        _AudAvInputMuxSel(eSrc);
#ifdef APP_EXT_IN_WORKAROUND        
        _SD_AUD_ExtInPlay(u1DecId);
#endif
        break;  
    case  SD_SOURCE_MEDIA:      //!< Media Source, DNet, WiseLink, PVR    
        _arAudDec[u1DecId].eStrSrc = AUD_STREAM_FROM_FEEDER;
        break;
    case  SD_SOURCE_DVR:
    case  SD_SOURCE_DLINK:                       
        _arAudDec[u1DecId].eStrSrc = AUD_STREAM_FROM_LINE_IN;
        printk("Not supported SdSource_k \n");
        break;
    case  SD_SOURCE_PCM:        //!< Use PCM as source.                                    
        _arAudDec[u1DecId].eStrSrc = AUD_STREAM_FROM_LINE_IN;
        //_SD_AUD_ExtInPlay(u1DecId);
        printk("Internal PCM \n");
        break;
    default:
        printk("Error SdSource_k \n");
                                                                     
    }

    if ((eSrc == SD_SOURCE_DTV1) ||
        (eSrc == SD_SOURCE_DTV2))
    {
        _aeSpdifType[u1DecId] = AUD_IEC_CFG_RAW;
    }
    else
    {
        _aeSpdifType[u1DecId] = AUD_IEC_CFG_PCM;
    }

    SD_AUD_SEMA_UNLOCK();
    return eRet;
}                                     

SdResult _SD_AUD_Disconnect(SdSoundID_k eSoundId)
{
    UINT8 u1DecId;
    AUD_SOUND_ID_VALIDATE(eSoundId);
    u1DecId = SOUND_ID_TO_DEC_ID(eSoundId);
    
    SD_AUD_SEMA_LOCK();

#ifdef CONNECT_SEMA_PROTECT
    SD_AUD_CONNECT_SEMA_UNLOCK(u1DecId);
#endif

    _arAudDec[u1DecId].fgConnected = FALSE;

    if (_arAudDec[u1DecId].fgOnPlay)
    {
        AUD_DSPCmdStop(u1DecId);
        _arAudDec[u1DecId].fgOnPlay = FALSE;
    }

    if (u1DecId == AUD_DEC_AUX)
    {
        AUD_DspSetDualDecMode(FALSE);
        AUD_DrvClkCfgDualDecMode(FALSE);
    }
    
    SD_AUD_SEMA_UNLOCK();
    return SD_OK;
}

SdResult _SD_AUD_SpeakerConnect(SdSpeakerID_k eSpeakerId, SdSoundID_k eSoundId)
{
    AUD_SOUND_ID_VALIDATE(eSoundId);
    AUD_SPEAKER_ID_VALIDATE(eSpeakerId);

    SD_AUD_SEMA_LOCK();
    // TODO !!
    SD_AUD_SEMA_UNLOCK();
    
    return SD_OK;
}

SdResult _SD_AUD_SpeakerDisconnect(SdSpeakerID_k eSpeakerId)    
{
    AUD_SPEAKER_ID_VALIDATE(eSpeakerId);

    SD_AUD_SEMA_LOCK();
    // Suppose there is nothing to do ..
    SD_AUD_SEMA_UNLOCK();
    return SD_OK;
}

SdResult _SD_AUD_GetInfo(SdSoundID_k eSoundId, SdSound_Settings_t *pSettings)
{
    AUD_SOUND_ID_VALIDATE(eSoundId);
    AUD_POINTER_VALIDATE(pSettings);

    SD_AUD_SEMA_LOCK();

    memcpy((VOID *)pSettings, (const VOID *)_arSoundSetInfo, sizeof(SdSound_Settings_t));        

    SD_AUD_SEMA_UNLOCK();
    return SD_OK;
}

SdResult _SD_AUD_SetInfo(SdSoundID_k eSoundId, SdSound_Settings_t *pSettings)
{
    UINT8 u1DecId;
    AUD_SOUND_ID_VALIDATE(eSoundId);
    AUD_POINTER_VALIDATE(pSettings);
    u1DecId = SOUND_ID_TO_DEC_ID(eSoundId);

    SD_AUD_SEMA_LOCK();
    
    if (IS_SET_VOL(u1DecId, pSettings))
    {
        _AUD_DspMasterVolShmRawValue(u1DecId, pSettings->volume);
    }
    if (IS_SET_BALANCE(u1DecId, pSettings))
    {
        AUD_DspBalance(u1DecId, _AudBalance(pSettings->balance));
    }
    if (IS_SET_SPK_DELAY(u1DecId, pSettings))
    {
        AUD_DspChannelDelay(_DelayFromMs(pSettings->speakerDelay), AUD_CH_FRONT_LEFT, u1DecId);
        AUD_DspChannelDelay(_DelayFromMs(pSettings->speakerDelay), AUD_CH_FRONT_RIGHT, u1DecId);
    }
    if (IS_SET_HEADPHONE_DELAY(u1DecId, pSettings))
    {
        AUD_DspChannelDelay(_DelayFromMs(pSettings->headsetDelay), AUD_CH_DMX_LEFT, u1DecId);
        AUD_DspChannelDelay(_DelayFromMs(pSettings->headsetDelay), AUD_CH_DMX_RIGHT, u1DecId);
    }
    if (IS_SET_AUX_ANALOG_DELAY(u1DecId, pSettings))
    {
        AUD_DspChannelDelay(_DelayFromMs(pSettings->auxAnalogDelay), AUD_CH_BYPASS_LEFT, u1DecId);
        AUD_DspChannelDelay(_DelayFromMs(pSettings->auxAnalogDelay), AUD_CH_BYPASS_RIGHT, u1DecId);
    }
    if (IS_SET_AGC(u1DecId, pSettings))
    {
        AUD_DspAVCEnable(u1DecId, pSettings->bAutoVolume);
    }
    if (IS_SET_BBE(u1DecId, pSettings))
    {
        // TODO
    }
    if (IS_SET_TSXT_MODE(u1DecId, pSettings))
    {
        _AudSetTsxtMode(u1DecId, pSettings->eTsxtMode);
    }
    if (IS_SET_DNSE_MODE(u1DecId, pSettings))
    {
        // TODO
    }
    if (IS_SET_MTS_MODE(u1DecId, pSettings))
    {
        _AudSetMtsMode(u1DecId, pSettings->eOutputAnalogMts);
    }

    // Check EQ
    _AudSetEqLevel(u1DecId, pSettings);

    if (IS_SET_AMP_GAIN(u1DecId, pSettings))
    {
        // TODO
        //ADAC_AmpGain(u1DecId, _AudAmpGain(pSettings->eAmpGainLevel));
    }

    // Check subwoofer setup
    _AudSetSubwoofer(u1DecId, &(pSettings->sWooferSet));

    // Save to local 
    memcpy((void *)&_arSoundSetInfo[u1DecId], (const void *) pSettings, 
             sizeof(SdSound_Settings_t));

    SD_AUD_SEMA_UNLOCK();
    
    return SD_OK;
}

SdResult _SD_AUD_GetStatus(SdSoundID_k eSoundId, SdMask_t mask, 
                           SdSound_Status_t *pState, uint32_t timeout)
{
    AUD_DEC_STREAM_FROM_T eStrmSrc;
    SdSound_MtsMode_k     eMtsMode = SD_MTS_STEREO;
    UINT8                 u1DecId;
        
    AUD_SOUND_ID_VALIDATE(eSoundId);
    AUD_POINTER_VALIDATE(pState);
    u1DecId = SOUND_ID_TO_DEC_ID(eSoundId);
    UNUSED(timeout);

    SD_AUD_SEMA_LOCK();
    
    eStrmSrc = _arAudDec[u1DecId].eStrSrc;

    switch (eStrmSrc)
    {
    case AUD_STREAM_FROM_LINE_IN:
    case AUD_STREAM_FROM_HDMI:    
        eMtsMode = SD_MTS_STEREO;
        break;
    case AUD_STREAM_FROM_ANALOG_TUNER:
        eMtsMode = _GetAudType(eSoundId);
        break;
    case AUD_STREAM_FROM_DIGITAL_TUNER:
        // FIXME !!
        eMtsMode = SD_MTS_STEREO;
        break;
    default:
        break;    
    }
    
    pState->eInputAnalogMts = eMtsMode;
    pState->eAmpGainLevel = _arAudDec[u1DecId].rSoundStatus.eAmpGainLevel;

    if (_SD_AUD_GetAUIStatus())
    {
        pState->eAUIState = SD_PLAY_AUI_DONE;
    }
    else
    {
        pState->eAUIState = SD_PLAY_AUI_ING;
    }

    SD_AUD_SEMA_UNLOCK();
    
    return SD_OK;
}

SdResult _SD_AUD_AmpMute(SdSpeakerID_k eSpeakerId, SdBool_t bMuteOn)  
{
    AUD_SPEAKER_ID_VALIDATE(eSpeakerId);
    UNUSED(eSpeakerId);

    SD_AUD_SEMA_LOCK();
    
    SD_AUD_SEMA_UNLOCK();
    return SD_OK;
}

SdResult _SD_AUD_SpeakerMute(SdSpeakerID_k eSpeakerId, SdBool_t bMuteOn)
{
    UINT8 u1DecId = AUD_DEC_MAIN;
    AUD_SPEAKER_ID_VALIDATE(eSpeakerId);
    UNUSED(eSpeakerId);

    SD_AUD_SEMA_LOCK();

    if (eSpeakerId == SD_SPEAKER_MAIN)
    {
        // Use channel mute to implement the mute function
        AUD_DspChannelMute(u1DecId, AUD_CH_FRONT_LEFT, bMuteOn);
        AUD_DspChannelMute(u1DecId, AUD_CH_FRONT_RIGHT, bMuteOn);
    }
    else if (eSpeakerId == SD_SPEAKER_SUB0)  // Headphone ..
    {
        // Use channel mute to implement the mute function
        AUD_DspChannelMute(u1DecId, AUD_CH_CENTER, bMuteOn);
        AUD_DspChannelMute(u1DecId, AUD_CH_SUB_WOOFER, bMuteOn);
    }
    
    //ADAC_SpeakerEnable(!bMuteOn);

    SD_AUD_SEMA_UNLOCK();
    return SD_OK;
}

SdResult _SD_AUD_PlayBeeper(SdSoundID_k eSoundId, SdSound_MelodyMode_k eMelodyMode)    
{
    AUD_CLIP_TYPE_T      rAudClip;
    MEM_BUFFER_INFO_T    rMemBuf;
    UINT8                u1DecId = AUD_DEC_MAIN;
    
    AUD_SOUND_ID_VALIDATE(eSoundId);
    SD_AUD_SEMA_LOCK();

    memset(&rAudClip, 0, sizeof(AUD_CLIP_TYPE_T));

    _AUD_GetStrInfo(eMelodyMode, &rMemBuf);

    rAudClip.fgPlayTestTone = FALSE;
    rAudClip.eAudFmt = AUD_FMT_PCM;
    rAudClip.eMemBuf.pData    = rMemBuf.pData;
    rAudClip.eMemBuf.u4Length = rMemBuf.u4Length;
    rAudClip.eMemBuf.u4LoopCount = 5;
    rAudClip.u4RepeatCount = 5;
    rAudClip.u4OutputChlSelect = 0xffffffff;

    AUD_SetAudClip(u1DecId, &rAudClip);
    AUD_PlayAudClip(u1DecId, rAudClip.u4OutputChlSelect,
                    rAudClip.fgPlayTestTone);
    
    SD_AUD_SEMA_UNLOCK();
    return SD_OK;
}

SdResult _SD_AUD_PlayAUI(SdSoundID_k eSoundId, SdAudioFormat_k eFormat, 
                         uint8_t* pBuffer, uint32_t u4Size)
{
    AUD_CLIP_TYPE_T      rAudClip;
    UINT8                u1DecId = AUD_DEC_MAIN;   

    AUD_SOUND_ID_VALIDATE(eSoundId);
    AUD_POINTER_VALIDATE(pBuffer);
    u1DecId = SOUND_ID_TO_DEC_ID(eSoundId);

    _fgAUIDone = FALSE;
    
    SD_AUD_SEMA_LOCK();

    // Lock audio resource when AUI is on play
    SD_AUD_ON_PLAY_SEMA_LOCK();

    rAudClip.fgPlayTestTone = FALSE;
    rAudClip.eAudFmt = _FmtTransfer(eFormat);
    rAudClip.eMemBuf.pData    = pBuffer;
    rAudClip.eMemBuf.u4Length = u4Size;
    rAudClip.eMemBuf.u4LoopCount = 1;
    rAudClip.u4RepeatCount = 1;
    rAudClip.u4OutputChlSelect = AUD_CH_ALL;

    AUD_SetAudClip(u1DecId, &rAudClip);
    AUD_ClipSetNotifyFunc(u1DecId, _AudClipNotifyHandler);
    AUD_PlayAudClip(u1DecId, rAudClip.u4OutputChlSelect,
                    rAudClip.fgPlayTestTone);
    
    SD_AUD_SEMA_UNLOCK();
    return SD_OK;
}


SdResult _SD_AUD_SetSifType(SdSoundID_k eSoundId, 
                            SdSound_SoundSys_k  eSoundTyp)
{
    UINT8                u1DecId = AUD_DEC_MAIN;   

    AUD_SOUND_ID_VALIDATE(eSoundId);
    u1DecId = SOUND_ID_TO_DEC_ID(eSoundId);

    SD_AUD_SEMA_LOCK();

    switch (eSoundTyp)
    {
        case SD_AUDIO_ATV_BG:
        case SD_AUDIO_ATV_I:
        case SD_AUDIO_ATV_DK:
        case SD_AUDIO_ATV_L:
            _arAudDec[u1DecId].eAudFmt = AUD_FMT_PAL;
            break;
        case SD_AUDIO_ATV_BG_A2:
        case SD_AUDIO_ATV_DK1_A2:
        case SD_AUDIO_ATV_DK2_A2:
        case SD_AUDIO_ATV_DK3_A2:
        case SD_AUDIO_ATV_A2_KOREA:
            _arAudDec[u1DecId].eAudFmt = AUD_FMT_A2;            
            break;
        case SD_AUDIO_ATV_BTSC:
            _arAudDec[u1DecId].eAudFmt = AUD_FMT_MTS;
            break;
        case SD_AUDIO_ATV_EIAJ:
            _arAudDec[u1DecId].eAudFmt = AUD_FMT_UNKNOWN;
            break;
    }

    if (GetTargetCountry() == COUNTRY_EU)
    {
        AUD_DspSetTvSrcMode((TV_AUD_SYS_T)eSoundTyp);

        AUD_DspSetDualDecMode(TRUE);
        AUD_DrvClkCfgDualDecMode(TRUE);
    }
    
    AUD_SetDecType(u1DecId, _arAudDec[u1DecId].eStrSrc , _arAudDec[u1DecId].eAudFmt); 

    SD_AUD_SEMA_UNLOCK();
    return SD_OK;
}


BOOL _SD_AUD_GetAUIStatus(void)
{
    return _fgAUIDone;
}

void _SD_AUD_Debug(void)
{

}


SdResult _SD_AUD_GetSoundSys(SdSoundID_k eSoundId, SdSound_SoundSys_k eDefaultSys, SdSound_SoundSys_k *peSoundSys)
{
    UINT8                 u1DecId;
    AUD_SOUND_ID_VALIDATE(eSoundId);
    AUD_POINTER_VALIDATE(peSoundSys);
    u1DecId = SOUND_ID_TO_DEC_ID(eSoundId);

    if (GetTargetCountry() == COUNTRY_EU)
    {
        *peSoundSys = _AudGetSoundSys(u1DecId, eDefaultSys);          
    }
    else
    {
        *peSoundSys = SD_AUDIO_ATV_BTSC;
    }
    return SD_OK;
}


