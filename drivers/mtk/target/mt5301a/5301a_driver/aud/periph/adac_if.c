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
 * $RCSfile: adac_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file adac_if.c
 *  Audio interface setting, including codec and speaker mute/unmute, headphone detect mute control
 *  and codec/amp gpio control.
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN

#include "x_typedef.h"
#include "x_os.h"
#include "x_assert.h"
#include "adac_if.h"
#include "aud_if.h"
#include "adac_hw.h"
#include "aud_debug.h"
#include "drvcust_if.h"
#include "x_pinmux.h"
#include "codec_wm8776.h"
#include "codec_mt8291.h"
#include "codec_mt8292.h"
#include "codec_mt5387.h"
#ifdef CC_MT5391_AUD_SUPPORT
#include "codec_mt8297.h"
#else
#include "codec_mt536xinternal.h"
#endif
#include "adac_apogee.h"
#include "digiamp_others.h"
#include "codec_ad8356.h"

#ifdef CC_AUD_USE_EXTRA_PERIPHERAL_IC
#include "periph_if.h"
#endif

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

CODEC_T* _prCodecTbl = NULL;
CODEC_T* _prCodecTblNUM2 = NULL;

AMP_T*   _prAmpTbl = NULL;
//ADAC_T*  _prDacTbl = NULL;
//CODEC_T*  _prAdcTbl = NULL;


// Semaphore and message queue
static HANDLE_T _hSemaCodecSetting = NULL;
static HANDLE_T _hSemaAmpSetting = NULL;

static BOOL _fgCodecSystemMute = FALSE;
static BOOL _fgCodecMWMute = FALSE;
static BOOL _fgCodecHpDetectMute = FALSE;
static BOOL _fgCodecClkChangeMute = FALSE;
static BOOL _fgCodecHdmiUnstableMute = FALSE;

static BOOL _fgSpkHpDetectSpkEnableFlag = TRUE;
static BOOL _fgSpkDcDetectSpkEnableFlag = TRUE;
static BOOL _fgVolumeSpkEnableFlag = TRUE;
static BOOL _fgSpkGpioEnableFlag = FALSE;
static BOOL _fgSpkSystemEnableFlag = TRUE;
static BOOL _fgSpkHdmiUnstableEnableFlag = TRUE;
static BOOL _fgSpkEnable = FALSE;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

static BOOL _IsUseWM8776(void)
{
    AUD_ADAC_USAGE_T* prAdacUsage;
    UNUSED(DRVCUST_OptQuery(eAudioDacUsage, (UINT32 *)(void *)&prAdacUsage));

    if (prAdacUsage == NULL)
    {
        return FALSE;
    }

    if ((prAdacUsage->eCodec1 == AUD_CODEC_WM8776))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static BOOL _IsUseMT8291(void)
{
    AUD_ADAC_USAGE_T* prAdacUsage;
    UNUSED(DRVCUST_OptQuery(eAudioDacUsage, (UINT32 *)(void *)&prAdacUsage));

    if (prAdacUsage == NULL)
    {
        return FALSE;
    }

    if ((prAdacUsage->eCodec1 == AUD_CODEC_MT8291))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#ifdef CC_MT5391_AUD_SUPPORT
static BOOL _IsUseMT538xInternal(void)
{
    AUD_ADAC_USAGE_T* prAdacUsage;
    UNUSED(DRVCUST_OptQuery(eAudioDacUsage, (UINT32 *)(void *)&prAdacUsage));

    if (prAdacUsage == NULL)
    {
        return FALSE;
    }

    if ((prAdacUsage->eCodec1 == AUD_CODEC_MT538xInternal))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
static BOOL _IsUseMT539xInternal(void)
{
    AUD_ADAC_USAGE_T* prAdacUsage;
    UNUSED(DRVCUST_OptQuery(eAudioDacUsage, (UINT32 *)(void *)&prAdacUsage));

    if (prAdacUsage == NULL)
    {
        return FALSE;
    }

    if ((prAdacUsage->eCodec1 == AUD_CODEC_MT539xInternal))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#else

static BOOL _IsUseMT536xInternal(void)
{
    AUD_ADAC_USAGE_T* prAdacUsage;
    UNUSED(DRVCUST_OptQuery(eAudioDacUsage, (UINT32 *)(void *)&prAdacUsage));

    if (prAdacUsage == NULL)
    {
        return FALSE;
    }

    if ((prAdacUsage->eCodec1 == AUD_CODEC_MT536xInternal))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif

BOOL _IsUseMT8292(void)
{
    AUD_ADAC_USAGE_T* prAdacUsage;
    UNUSED(DRVCUST_OptQuery(eAudioDacUsage, (UINT32 *)(void *)&prAdacUsage));

    if (prAdacUsage == NULL)
    {
        return FALSE;
    }

    if ((prAdacUsage->eCodec2 == AUD_CODEC_MT8292))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static BOOL _IsUseMT5387(void)
{
    AUD_ADAC_USAGE_T* prAdacUsage;
    UNUSED(DRVCUST_OptQuery(eAudioDacUsage, (UINT32 *)(void *)&prAdacUsage));

    if (prAdacUsage == NULL)
    {
        return FALSE;
    }

    if ((prAdacUsage->eCodec1 == AUD_CODEC_MT5387))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static BOOL _IsUseDigiAmp(void)
{
    AUD_ADAC_USAGE_T* prAdacUsage;
    UNUSED(DRVCUST_OptQuery(eAudioDacUsage, (UINT32 *)(void *)&prAdacUsage));

    if (prAdacUsage == NULL)
    {
        return FALSE;
    }

    if (prAdacUsage->eAudAmp1 == AUD_AMP_APOGEE_DDX_2051) 
    {
        return TRUE;
    }
    else if (prAdacUsage->eAudAmp1 == AUD_AMP_AD82581) 
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static BOOL _IsUseDigiAmpOthers(void)
{
    AUD_ADAC_USAGE_T* prAdacUsage;
    UNUSED(DRVCUST_OptQuery(eAudioDacUsage, (UINT32 *)(void *)&prAdacUsage));

    if (prAdacUsage == NULL)
    {
        return FALSE;
    }

    if (prAdacUsage->eAudAmp2 == AUD_AMP_OTHERS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static BOOL _IsUseAD8356(void)
{
    AUD_ADAC_USAGE_T* prAdacUsage;
    UNUSED(DRVCUST_OptQuery(eAudioDacUsage, (UINT32 *)(void *)&prAdacUsage));

    if (prAdacUsage == NULL)
    {
        LOG(0,"_IsUseAD8356 null.\n");
        return FALSE;
    }

    if (prAdacUsage->eAudAmp2 == AUD_AMP_AD8356)
    {
        LOG(0,"_IsUseAD8356 true.\n");
        return TRUE;
    }
    else
    {
        LOG(0,"_IsUseAD8356 false.\n");
        return FALSE;
    }
}

static void _LoadCodecTble(void)
{
    if (_IsUseWM8776())
    {
        _prCodecTbl = _CODEC_GetWM8776Tbl();
    }
    else if (_IsUseMT8291())
    {
        _prCodecTbl = _CODEC_GetMT8291Tbl();
    }
#ifdef CC_MT5391_AUD_SUPPORT    
    else if (_IsUseMT538xInternal())
    {
        _prCodecTbl = _CODEC_GetMT8297Tbl();
        //_prCodecTbl = _CODEC_GetMT538xInternalTbl();
    }
#else    
    else if (_IsUseMT536xInternal())
    {
        _prCodecTbl = _CODEC_GetMT536xInternalTbl();
    }
    else if (_IsUseMT5387())
    {
        Printf("[_LoadCodecTble] - MT5387\n");
        _prCodecTbl = _CODEC_GetMT5387Tbl();
    }
#endif    

    if (_IsUseMT8292())
    {
        _prCodecTblNUM2= _CODEC_GetMT8292Tbl();
    }

    
}

static void _LoadAmpTble(void)
{
    if (_IsUseDigiAmp())
    {
        //DEBUG_CODE
        Printf("[_LoadAmpTble] - DigiAmp\n");
        //DEBUG_CODE            
        //_prAmpTbl = _AMP_GetDigiAmpFunTbl(); //xiaoyi for build warning
    } 
    if(_IsUseAD8356())
    {
        _prAmpTbl = _AMP_GetAmpAD8356FunTbl();
    }
    if (_IsUseDigiAmpOthers())
    {
        _prAmpTbl = _AMP_GetAmpOthersFunTbl();
    }
}

static void _AdacSpeakerChangeState(void)
{
    static BOOL fgInit = FALSE;
    BOOL fgEnableFlag;
    
    fgEnableFlag = _fgSpkHpDetectSpkEnableFlag && _fgSpkGpioEnableFlag &&
                   _fgVolumeSpkEnableFlag && _fgSpkSystemEnableFlag && 
                   _fgSpkDcDetectSpkEnableFlag && _fgSpkHdmiUnstableEnableFlag;

    if ((fgEnableFlag != _fgSpkEnable) || (!fgInit))
    {
        if (fgEnableFlag) //unmute amp
        {           
        	  //1. hardware unmute first
            //ADAC_GpioAmpEnable(fgEnableFlag);
        		//2. software unmute
        		if (_prAmpTbl != NULL)
        		{
            	  if (_prAmpTbl->prAmpFunc->pfnMute != NULL)
            	  {
                    _prAmpTbl->prAmpFunc->pfnMute(!fgEnableFlag);
            	  }
                }
        		else
        		{
                    ADAC_GpioAmpEnable(fgEnableFlag);
        		}
        }
        else //mute amp
        {
        		//1. software mute first
        		if (_prAmpTbl != NULL)
        		{
                if (_prAmpTbl->prAmpFunc->pfnMute != NULL)
                {
                    _prAmpTbl->prAmpFunc->pfnMute(!fgEnableFlag);
                }
            }
        	else
        	{
                ADAC_GpioAmpEnable(fgEnableFlag);
        	}
            //2. hardware mute
            //ADAC_GpioAmpEnable(fgEnableFlag);
        }
        LOG(8, "**** _AdacSpeakerChangeState = %x , 1: Unmute, 0: Mute\n", fgEnableFlag);
    }
    
    fgInit = TRUE;
    _fgSpkEnable = fgEnableFlag;
}

static void _AdacCodecChangeState(void)
{
    BOOL fgMuteFlag;

    fgMuteFlag = _fgCodecSystemMute || _fgCodecMWMute || _fgCodecHpDetectMute || _fgCodecClkChangeMute || _fgCodecHdmiUnstableMute;
    
    if (_prCodecTbl != NULL)
    {
        if (_prCodecTbl->prCodecFuncTbl->pfnMute != NULL)
        {
            _prCodecTbl->prCodecFuncTbl->pfnMute(3,fgMuteFlag);
        }
    }    
    if (_prCodecTblNUM2 != NULL)
    {
        if (_prCodecTblNUM2->prCodecFuncTbl->pfnMute != NULL)
        {
            _prCodecTblNUM2->prCodecFuncTbl->pfnMute(3,fgMuteFlag);
        }
    }    
    
   //ADAC_GpioCodecMute(fgMuteFlag);
   LOG(8, "**** _AdacCodecChangeState = %x , 1: Mute, 0: Unmute\n", fgMuteFlag);
}

static void _AdacSwInit(void)
{
    if (_hSemaCodecSetting == NULL)
    {
        VERIFY(x_sema_create(&_hSemaCodecSetting, X_SEMA_TYPE_BINARY,
                                        X_SEMA_STATE_UNLOCK) == OSR_OK);
    }

    if (_hSemaAmpSetting == NULL)
    {
        VERIFY(x_sema_create(&_hSemaAmpSetting, X_SEMA_TYPE_BINARY,
                                        X_SEMA_STATE_UNLOCK) == OSR_OK);
    }
}

static void _AdacTableInit(void)
{
    static BOOL fgInit = FALSE;
    if (!fgInit)
    {
        _LoadCodecTble(); 
        _LoadAmpTble();
        _AdacSwInit();
        fgInit = TRUE;
    }
}

//---------------------------------------------------------------------------
// Global functions
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ADAC_Init
//
/** Initial audio codec and digital amp.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_Init(void)
{  
    _AdacTableInit();

    if (_prAmpTbl != NULL)
    {
        if (_prAmpTbl->prAmpFunc->pfnInit != NULL)
        {
            _prAmpTbl->prAmpFunc->pfnInit();
        }
    }
    if (_prCodecTbl != NULL)
    {
        if (_prCodecTbl->prCodecFuncTbl->pfnInit != NULL)
        {
            _prCodecTbl->prCodecFuncTbl->pfnInit();
        }
    }   
    if (_prCodecTblNUM2 != NULL)
    {
        if (_prCodecTblNUM2->prCodecFuncTbl->pfnInit != NULL)
        {
            _prCodecTblNUM2->prCodecFuncTbl->pfnInit();
        }
    } 

#ifdef CC_AUD_USE_EXTRA_PERIPHERAL_IC

    PERIPH_Init();    

#endif
    
}

//-----------------------------------------------------------------------------
// ADAC_Enable
//
/** Audio codec and amp function table init.
 *
 *  @param[in]     fgEnable   TRUE for initial.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_Enable(BOOL fgEnable)
{
    UNUSED(fgEnable);
    _AdacTableInit();
    // TODO !!
/*
    if (_prDacTbl != NULL)
    {
        if (_prDacTbl->prDacFuncTbl->pfnEnable != NULL)
        {
            _prDacTbl->prDacFuncTbl->pfnEnable(fgEnable);
        }
    }
*/
}

//-----------------------------------------------------------------------------
// ADAC_Mute
//
/** Audio codec and speaker mute.
 *
 *  @param[in]     fgEnable   "TRUE" for mute,"FALSE" for unmute.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_Mute(BOOL fgEnable)
{
    _AdacTableInit();
    
    // Driver will mute codec for internal parameter setting
    VERIFY(x_sema_lock(_hSemaCodecSetting, X_SEMA_OPTION_WAIT) == OSR_OK);
    _fgCodecSystemMute = fgEnable;
    _AdacCodecChangeState();
    VERIFY(x_sema_unlock(_hSemaCodecSetting) == OSR_OK);
  
    // Driver will mute Amp (speaker) for internal parameter setting
    VERIFY(x_sema_lock(_hSemaAmpSetting, X_SEMA_OPTION_WAIT) == OSR_OK);
    _fgSpkSystemEnableFlag = !fgEnable;
    #if 0//Darren Weng(because amplify mute funetion always has pop noise, so we don't suggest use amplfiy mue when change sample rate,let's just mute DAC PGA mute funtion.
    _AdacSpeakerChangeState();
    #endif
    VERIFY(x_sema_unlock(_hSemaAmpSetting) == OSR_OK);

}

//-----------------------------------------------------------------------------
// ADAC_DacFormatCfg
//
/** Setting DAC Data Format.
 *
 *  @param[in]     u1DecId   Decoder ID.
 *  @param[in]     eFormat   Audio data format,ex:LJ,RJ,I2S.
 *  @param[in]     eMclk   MCLK frequency.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_DacFormatCfg(UINT8 u1DecId, DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)  
{
    UNUSED(u1DecId);
    _AdacTableInit();
    
    if (_prCodecTbl != NULL)
    {
        if (_prCodecTbl->prCodecFuncTbl->pfnSetDacInFmt != NULL)
        {
            _prCodecTbl->prCodecFuncTbl->pfnSetDacInFmt(eFormat, eMclk);
        }
    }
    if (_prAmpTbl != NULL)
    {
        if (_prAmpTbl->prAmpFunc->pfnSetDacInFmt != NULL)
        {
            _prAmpTbl->prAmpFunc->pfnSetDacInFmt(eFormat, eMclk);
        }
    }
}

//-----------------------------------------------------------------------------
// ADAC_AdcFormatCfg
//
/** Setting Codec Adc Data Format.
 *
 *  @param[in]     u1DecId   Decoder ID.
 *  @param[in]     eFormat   Audio data format,ex:LJ,RJ,I2S.
 *  @param[in]     eMclk   MCLK frequency.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_AdcFormatCfg(UINT8 u1DecId, DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)  
{
    UNUSED(u1DecId);
    _AdacTableInit();
    
    if (_prCodecTbl != NULL)
    {
        if (_prCodecTbl->prCodecFuncTbl->pfnSetAdcOutFmt != NULL)
        {
            _prCodecTbl->prCodecFuncTbl->pfnSetAdcOutFmt(eFormat, eMclk);
        }
    }
}

//-----------------------------------------------------------------------------
// ADAC_HeadphoneVolCtl
//
/** Setting Codec Adc Data Format.
 *
 *  @param[in]     i2VolInHalfDb   volume value.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_HeadphoneVolCtl(INT16 i2VolInHalfDb)
{
    _AdacTableInit();
    if (_prCodecTbl != NULL)
    {
        if (_prCodecTbl->prCodecFuncTbl->pfnSetHpGain != NULL)
        {
            _prCodecTbl->prCodecFuncTbl->pfnSetHpGain(0, i2VolInHalfDb);
        }
    }
}

//-----------------------------------------------------------------------------
// ADAC_GetHeadphoneVolMaxMin
/**  Setting the max and min value of headphone volume.
*
*  @param[in]     pi2Max   volume max value.
*  @param[in]     pi2Min   volume min value.
*
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_GetHeadphoneVolMaxMin(INT16* pi2Max,INT16* pi2Min)
{
    _AdacTableInit();
     //lint -e{613}
    if (_prCodecTbl != NULL)
    {
        *pi2Max = _prCodecTbl->i2MaxHpGain;
        *pi2Max = _prCodecTbl->i2MinHpGain;
    }
    else
    {
        *pi2Max = 0xff;
        *pi2Min = 0; 
    }
}


//-----------------------------------------------------------------------------
// ADAC_VolumeMuteSpeakerEnable
//
/** Mute speaker when volume value equal to zero.
 *
 *  @param[in]     fgFlag   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_VolumeMuteSpeakerEnable(BOOL fgFlag)
{
    static BOOL fgInit = FALSE;
    //BOOL fgSpkEnable;
    
    _AdacTableInit();
    
    //fgSpkEnable = !fgFlag;
    if ((_fgVolumeSpkEnableFlag != fgFlag) || (!fgInit))
    {
        _fgVolumeSpkEnableFlag = fgFlag;
        _AdacSpeakerChangeState(); 
    }
    
    fgInit = TRUE;
}

//-----------------------------------------------------------------------------
// ADAC_DcDetectSpkEnable
//
/** Detect the DC and mute speaker.
 *
 *  @param[in]     fgFlag   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_DcDetectSpkEnable(BOOL fgFlag)
{
    static BOOL fgInit = FALSE;
    
    _AdacTableInit();

    VERIFY(x_sema_lock(_hSemaAmpSetting, X_SEMA_OPTION_WAIT) == OSR_OK);

    if ((_fgSpkDcDetectSpkEnableFlag != fgFlag) || (!fgInit))
    {
        _fgSpkDcDetectSpkEnableFlag = fgFlag;
        _AdacSpeakerChangeState(); 
    }
   
    VERIFY(x_sema_unlock(_hSemaAmpSetting) == OSR_OK);
    
    fgInit = TRUE;
}

//-----------------------------------------------------------------------------
// ADAC_HdmiUnstableSpkEnable
//
/** HDMI signal unstable and mute speaker.
 *
 *  @param[in]     fgFlag   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_HdmiUnstableSpkEnable(BOOL fgFlag)
{
    static BOOL fgInit = FALSE;
    
    _AdacTableInit();

    VERIFY(x_sema_lock(_hSemaAmpSetting, X_SEMA_OPTION_WAIT) == OSR_OK);

    if ((_fgSpkHdmiUnstableEnableFlag != fgFlag) || (!fgInit))
    {
        _fgSpkHdmiUnstableEnableFlag = fgFlag;
        _AdacSpeakerChangeState(); 
    }
   
    VERIFY(x_sema_unlock(_hSemaAmpSetting) == OSR_OK);
    
    fgInit = TRUE;
}

//-----------------------------------------------------------------------------
// ADAC_HpDetectSpkEnable
//
/** Detect the headphone plug in and mute speaker.
 *
 *  @param[in]     fgFlag   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_HpDetectSpkEnable(BOOL fgFlag)
{
    static BOOL fgInit = FALSE;
    
    _AdacTableInit();

    VERIFY(x_sema_lock(_hSemaAmpSetting, X_SEMA_OPTION_WAIT) == OSR_OK);

    if ((_fgSpkHpDetectSpkEnableFlag != fgFlag) || (!fgInit))
    {
        _fgSpkHpDetectSpkEnableFlag = fgFlag;
        _AdacSpeakerChangeState(); 
    }
   
    VERIFY(x_sema_unlock(_hSemaAmpSetting) == OSR_OK);
    
    fgInit = TRUE;
}

//-----------------------------------------------------------------------------
// ADAC_HpDetectCodecMute
//
/** Detect the headphone plug in and mute codec.
 *
 *  @param[in]     fgMute   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_HpDetectCodecMute(BOOL fgMute)
{
    _AdacTableInit();

    VERIFY(x_sema_lock(_hSemaCodecSetting, X_SEMA_OPTION_WAIT) == OSR_OK);

    _fgCodecHpDetectMute = fgMute;
    _AdacCodecChangeState();

    VERIFY(x_sema_unlock(_hSemaCodecSetting) == OSR_OK);
}

//-----------------------------------------------------------------------------
// ADAC_HpDetectDacMute
//
/** Detect the headphone plug in and mute internaldac.
 *
 *  @param[in]     fgMute   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_HpDetectDacMute(BOOL fgMute)
{
   UNUSED(fgMute);
    _AdacTableInit();

//    VERIFY(x_sema_lock(_hSemaDacSetting, X_SEMA_OPTION_WAIT) == OSR_OK);

//    _fgDacHpDetectMute = fgMute;
//    _AdacDacChangeState();

//    VERIFY(x_sema_unlock(_hSemaDacSetting) == OSR_OK);

//    VERIFY(x_sema_lock(_hSemaAdcSetting, X_SEMA_OPTION_WAIT) == OSR_OK);

//    _fgAdcHpDetectMute = fgMute;
//    _AadcAdcChangeState();

//    VERIFY(x_sema_unlock(_hSemaAdcSetting) == OSR_OK);
}

//-----------------------------------------------------------------------------
// ADAC_ClkChangeCodecMute
//
/** Some codec have to mute before clock source change 
 *
 *  @param[in]     fgMute   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_ClkChangeCodecMute(BOOL fgMute)
{
    _AdacTableInit();

    VERIFY(x_sema_lock(_hSemaCodecSetting, X_SEMA_OPTION_WAIT) == OSR_OK);

    // Mute first ..
    _fgCodecClkChangeMute = fgMute;
    _AdacCodecChangeState();

    // Check if need apll reset in MT8291 ..
    if (fgMute && _IsUseMT8291())
    {
        _MT8291_PLLReset();    
    }
    VERIFY(x_sema_unlock(_hSemaCodecSetting) == OSR_OK);
}

//-----------------------------------------------------------------------------
// ADAC_SpeakerEnable
//
/** Speaker enable routine.
 *
 *  @param[in]     fgEnable   "TRUE" for enable, "FALSE" for mute speaker.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_SpeakerEnable(BOOL fgEnable)
{
    static BOOL fgInit = FALSE;

    _AdacTableInit();   
    
    VERIFY(x_sema_lock(_hSemaAmpSetting, X_SEMA_OPTION_WAIT) == OSR_OK);
    
    if ((_fgSpkGpioEnableFlag != fgEnable) || (!fgInit))
    {
        _fgSpkGpioEnableFlag = fgEnable;
        _AdacSpeakerChangeState();   
    }

    VERIFY(x_sema_unlock(_hSemaAmpSetting) == OSR_OK);
    
    fgInit = TRUE;
}

//-----------------------------------------------------------------------------
// ADAC_CodecMute
//
/** Codec enable routine.
 *
 *  @param[in]     fgMute   "TRUE" for enable, "FALSE" for mute codec.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_CodecMute(BOOL fgMute)
{
    _AdacTableInit();

    VERIFY(x_sema_lock(_hSemaCodecSetting, X_SEMA_OPTION_WAIT) == OSR_OK);
    
    _fgCodecMWMute = fgMute;
    _AdacCodecChangeState();
    
    VERIFY(x_sema_unlock(_hSemaCodecSetting) == OSR_OK);
}

//-----------------------------------------------------------------------------
// ADAC_CodecHdmiUnstableMute
//
/** Codec mute when hdmi signal unstable.
 *
 *  @param[in]     fgMute   "TRUE" for enable, "FALSE" for mute codec.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_CodecHdmiUnstableMute(BOOL fgMute)
{
    _AdacTableInit();

    VERIFY(x_sema_lock(_hSemaCodecSetting, X_SEMA_OPTION_WAIT) == OSR_OK);
    
    _fgCodecHdmiUnstableMute = fgMute;
    _AdacCodecChangeState();
    
    VERIFY(x_sema_unlock(_hSemaCodecSetting) == OSR_OK);
}



//-----------------------------------------------------------------------------
// ADAC_GpioCodecMute
//
/** Codec GPIO control for mute/unmute.
 *
 *  @param[in]     fgEnable   "TRUE" for enable, "FALSE" for mute codec from GPIO.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_GpioCodecMute(BOOL fgEnable)
{
    INT32 i4RegAoutSwitch;
    UINT32 u4GpioMute;
    UINT32 u4OpctrlGpio = UNUSED_GPIO;
    AUD_MUTE_GPIO_POLARITY_T eMutePolariy;
    AUD_OPCTRL_MUTE_GPIO_POLARITY_T eOpctlMutePolariy;

    eMutePolariy = (AUD_MUTE_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioMuteGpioPolarity);
    if (eMutePolariy == AUD_MUTE_GPIO_HIGH_ENALBE)
    {
        i4RegAoutSwitch = fgEnable ? 1 : 0;
    }
    else
    {
        i4RegAoutSwitch = fgEnable ? 0 : 1;
    }
    u4GpioMute = DRVCUST_OptGet(eAudioMuteGpioNum);
    if (u4GpioMute != 0xffffffff)
    {  
        UNUSED(GPIO_Output((INT32)u4GpioMute, (INT32 *)&i4RegAoutSwitch));
    }

    u4OpctrlGpio = DRVCUST_OptGet(eAudioOpctrlMuteGpioNum);
    if (u4OpctrlGpio != 0xffffffff)
    {
        eOpctlMutePolariy = (AUD_OPCTRL_MUTE_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioOpctrlMuteGpioPolarity);
        if (eOpctlMutePolariy == AUD_OPCTRL_MUTE_GPIO_HIGH_ENALBE)
        {
            i4RegAoutSwitch = fgEnable ? 1 : 0;
        }
        else
        {
            i4RegAoutSwitch = fgEnable ? 0 : 1;
        }
        UNUSED(GPIO_Output((INT32)u4OpctrlGpio, (INT32 *)&i4RegAoutSwitch));
    }

    //UNUSED(GPIO_Output(AUD_GPIO_DAC_MUTE_CTL, (const INT32 *)&i4RegAoutSwitch));
}

//-----------------------------------------------------------------------------
// ADAC_GpioAmpEnable
//
/** Speaker GPIO control for mute/unmute.
 *
 *  @param[in]     fgEnable   "TRUE" for enable, "FALSE" for mute speaker from GPIO.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_GpioAmpEnable(BOOL fgEnable)
{
    INT32 i4RegAoutSwitch;
    UINT32 u4GpioAmp;
    AUD_AMP_GPIO_POLARITY_T eAmpPolariy;

    u4GpioAmp = DRVCUST_OptGet(eAudioAmpEnableGpioNum);
    if (u4GpioAmp != 0xffffffff)
    {
        eAmpPolariy = (AUD_AMP_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioAmpGpioPolarity);
        if (eAmpPolariy == AUD_AMP_GPIO_HIGH_ENALBE)
        {
            i4RegAoutSwitch = fgEnable ? 1 : 0;
        }
        else
        {
            i4RegAoutSwitch = fgEnable ? 0 : 1;
        }
        UNUSED(GPIO_Output((INT32)u4GpioAmp, (INT32 *)&i4RegAoutSwitch));
    }
}

//-----------------------------------------------------------------------------
// ADAC_CodecAdChlSel
//
/** Codec ad channel select.
 *
 *  @param[in]     u1Chl   Input data channel.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_CodecAdChlSel(UINT8 u1Chl)
{
    _AdacTableInit();

    if (_prCodecTbl != NULL)
	{
    	if (_prCodecTbl->prCodecFuncTbl->pfnSetAdcMuxSel != NULL)
    	{
        		_prCodecTbl->prCodecFuncTbl->pfnSetAdcMuxSel(u1Chl);
    	}
	}
    
/*
    if (_IsUseMT8292())
    {
    	if (_prCodecTblNUM2 != NULL)
    	{
        	if (_prCodecTblNUM2->prCodecFuncTbl->pfnSetAdcMuxSel != NULL)
        	{
            		_prCodecTblNUM2->prCodecFuncTbl->pfnSetAdcMuxSel(u1Chl);
        	}
    	}

#ifdef CC_AUD_USE_8292_AD_MUX
    	if (_prCodecTbl != NULL)
    	{
        	if (_prCodecTbl->prCodecFuncTbl->pfnSetAdcMuxSel != NULL)
        	{
            		_prCodecTbl->prCodecFuncTbl->pfnSetAdcMuxSel(0);	//USE 8292
        	}
    	}
#else
        if (_prCodecTbl != NULL)
    	{
        	if (_prCodecTbl->prCodecFuncTbl->pfnSetAdcMuxSel != NULL)
        	{
            		_prCodecTbl->prCodecFuncTbl->pfnSetAdcMuxSel(u1Chl);	//USE 5381
        	}
    	}
#endif
    }
    else
    {
        if (_prCodecTbl != NULL)
    	{
        	if (_prCodecTbl->prCodecFuncTbl->pfnSetAdcMuxSel != NULL)
        	{
            		_prCodecTbl->prCodecFuncTbl->pfnSetAdcMuxSel(u1Chl);	//USE 5381
        	}
    	}
    }
*/

}

//-----------------------------------------------------------------------------
// ADAC_CodecRegRead
//
/** Read register of external codec.
 *
 *  @param[in]     u1Offset   Address.
 *
 *  @retval 	UINT8	Data.
 */
//-----------------------------------------------------------------------------
UINT8 ADAC_CodecRegRead(UINT8 u1Offset)
{
    _AdacTableInit();
    if ((_prCodecTbl != NULL) &&
        (_prCodecTbl->prCodecFuncTbl->pfnRegRead != NULL))
    {
        return _prCodecTbl->prCodecFuncTbl->pfnRegRead(u1Offset);
    }
    else
    {
        return 0xff;
    }
}

//-----------------------------------------------------------------------------
// ADAC_CodecRegWrite
//
/** Write register of external codec.
 *
 *  @param[in]     u1Offset   	Address.
 *  @param[in]     u1Data   	Data.
 *
 *  @retval 	void.
 */
//-----------------------------------------------------------------------------
void ADAC_CodecRegWrite(UINT8 u1Offset, UINT8 u1Data)
{
    _AdacTableInit();
    
    if (_prCodecTbl != NULL)
    {
        if (_prCodecTbl->prCodecFuncTbl->pfnRegWrite != NULL)
        {
            _prCodecTbl->prCodecFuncTbl->pfnRegWrite(u1Offset, u1Data);
        }
    }
}

//-----------------------------------------------------------------------------
// ADAC_AmpRegRead
//
/** Read register of external digital amp.
 *
 *  @param[in]     u1Offset   Address.
 *
 *  @retval 	UINT8	Data.
 */
//-----------------------------------------------------------------------------
UINT8 ADAC_AmpRegRead(UINT8 u1Offset)
{
    _AdacTableInit();
    
    if ((_prAmpTbl != NULL) && (_prAmpTbl->prAmpFunc->pfnRegRead != NULL))
    {
        return _prAmpTbl->prAmpFunc->pfnRegRead(u1Offset);
    }
    else
    {
        return 0xff;
    }
}

//-----------------------------------------------------------------------------
// ADAC_AmpRegWrite
//
/** Write register of external digital amplify.
 *
 *  @param[in]     u1Offset   	Address.
 *  @param[in]     u1Data   	Data.
 *
 *  @retval 	void.
 */
//-----------------------------------------------------------------------------
void ADAC_AmpRegWrite(UINT8 u1Offset, UINT8 u1Data)
{
    _AdacTableInit();
    
    if (_prAmpTbl != NULL)
    {
        if (_prAmpTbl->prAmpFunc->pfnRegWrite != NULL)
        {
            _prAmpTbl->prAmpFunc->pfnRegWrite(u1Offset, u1Data);
        }
    }
}

