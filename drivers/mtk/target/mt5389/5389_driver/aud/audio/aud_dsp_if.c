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
 * $Author: xiongfeng.wen $
 * $Date: 2012/12/19 $
 * $RCSfile: aud_dsp_if.c,v $
 * $Revision: #4 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_dsp_interface.c
 *  Primitive APIs for audio and DSP configuration.
 */

#define AUD_EXTERN_FILE

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "aud_dsp_cfg.h"
#include "aud_debug.h"
#include "dsp_common.h"

LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
/*
static void DSP_IecOff(void)
{
	_DSP_IecOff();
}

static UINT32 DbToShm(INT32 i4Level)
{
	return _DbToShm(i4Level);
}

static UINT8 BalancedVolume(AUD_CH_T eChannel, UINT8 u1Balance, UINT8 u1Volume)
{
	return _BalancedVolume( eChannel,  u1Balance,  u1Volume);
}

static void VolumeCtrl(UINT8 u1DecId, AUD_CH_T eChannel, UINT32 u4Value)
{
	_VolumeCtrl( u1DecId,  eChannel,  u4Value);
}

static void AudDspChannelVolChange(UINT8 u1DecId, AUD_CH_T eChl)
{
	_AudDspChannelVolChange( u1DecId,  eChl);
}

static void AudDspMasterVolChange(UINT8 u1DecId)
{
	_AudDspMasterVolChange( u1DecId);
}


static void AudDspSrcVolChange(UINT8 u1DecId, UINT32 u4Value)
{
	_AudDspSrcVolChange( u1DecId,  u4Value);
}

static void AudDspAgcEnalbe(UINT8 u1DecId, BOOL fgEnable)
{
	_AudDspAgcEnalbe( u1DecId,  fgEnable);
}

static void AudDspSetIec(UCHAR ucDecId, AUD_IEC_T eIecCfg, BOOL fgEnable)
{
	_AudDspSetIec( ucDecId,  eIecCfg,  fgEnable);
}
*/

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

void AUD_DspCfgSetDecType(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt)
{
	_AUD_DspCfgSetDecType( u1DecId,  eStreamFrom,  eDecFmt);
}


//-----------------------------------------------------------------------------
/** AUD_DspFineTuneVolume
 *  Fine tune master volume/trim, and set it to DSP
 *  @param  u1DecId          Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  u4ShmValue       Volume (0x20000 is equal to 0 dBFs, max value: 0x7fffff)
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void AUD_DspFineTuneVolume(UINT8 u1DecId, AUD_CH_T eChannel, UINT32 u4ShmValue)
{
	_AUD_DspFineTuneVolume( u1DecId,  eChannel,  u4ShmValue);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetFineTuneVolume
 *  Fine tune master volume/trim, and set it to DSP
 *  @param  u1DecId          Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  u4ShmValue       Volume (0x20000 is equal to 0 dBFs, max value: 0x7fffff)
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
UINT32 AUD_DspGetFineTuneVolume(UINT8 u1DecId, AUD_CH_T eChannel)
{
    return _AUD_DspGetFineTuneVolume(u1DecId, eChannel);
}

//-----------------------------------------------------------------------------
/** AUD_DspVolumeInit
 *  Initialize all variables for volume control
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void AUD_DspVolumeInit(void)
{
	_AUD_DspVolumeInit();
}


//-----------------------------------------------------------------------------
/** AUD_DspEqCfgInit
 *  Initialize variables for equalizer preset configuration
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void AUD_DspEqCfgInit(void)
{
	_AUD_DspEqCfgInit();
}

//-----------------------------------------------------------------------------
/** AUD_DspChDelayInit
 *  Initialize table for channel delay configuration
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void AUD_DspChDelayInit(void)
{
    _AUD_DspChDelayInit();
}


//-----------------------------------------------------------------------------
/** AUD_DspSpdifInit
 *  Initialize SPDIF configuration
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void AUD_DspSpdifInit(void)
{
    _AUD_DspSpdifInit();
}

//-----------------------------------------------------------------------------
/** AUD_DspAsrcInit
 *  Initialize ASRC configuration
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void AUD_DspAsrcInit(void)
{
    _AUD_DspAsrcInit();
}



// *********************************************************************
// Function : void AUD_DspReverbEnable(UINT8 u1DecId, BOOL fgEnable)
// Description : turn on / off reverb control
// Parameter :  TRUE / FALSE
// Return    :  None.
// *********************************************************************
void AUD_DspReverbEnable(UINT8 u1DecId, BOOL fgEnable)
{
	_AUD_DspReverbEnable(u1DecId,fgEnable);
}

//-----------------------------------------------------------------------------
/** AUD_DspReverbMode
 *  load the predefined reverberation parameters sets.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  bMode From mode 0 to mode 7. Mode 0 will turn the reverberation effect off.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspReverbMode(UINT8 u1DecId, UINT8 u1Mode)
{
	_AUD_DspReverbMode( u1DecId,  u1Mode);
}

// *********************************************************************
// Function :  void AUD_DspChannelEqEnable(UINT8 u1DecId, BOOL fgEnable)
// Description : turn on /off the bass boost control
// Parameter : bMode - ADSP_ON / ADSP_OFF
// Return    : None.
// *********************************************************************
void AUD_DspChannelEqEnable(UINT8 u1DecId, BOOL fgEnable)
{
	_AUD_DspChannelEqEnable( u1DecId,  fgEnable);
}

BOOL AUD_DspGetChannelEqEnable(UINT8 u1DecId)
{
    return _AUD_DspGetChannelEqEnable(u1DecId);
}

// *********************************************************************
// Function :  void AUD_DspSpkSizeCfg(UINT8 u1DecId, UINT32 u4SpkSize)
// Description : Set the bass management cut-off frequency
// Parameter : u2SpkSize in xx Hz
// Return    : None.
// *********************************************************************
void AUD_DspSpkSizeCfg(UINT8 u1DecId, UINT32 u4SpkSize)
{
	_AUD_DspSpkSizeCfg( u1DecId,  u4SpkSize);
}

// *********************************************************************
// Function :  UINT32 AUD_DspSpkSizeQry(UINT8 u1DecId)
// Description : Query the bass management cut-off frequency
// Parameter :
// Return    : None.
// *********************************************************************
UINT32 AUD_DspSpkSizeQry(UINT8 u1DecId)
{
    return _AUD_DspSpkSizeQry(u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspBassBoostEnable
 *  turn on /off the bass boost control.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  fgEnable ADSP_ON / ADSP_OFF.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspBassBoostEnable(UINT8 u1DecId, BOOL fgEnable)
{
	_AUD_DspBassBoostEnable( u1DecId,  fgEnable) ;
}

BOOL AUD_DspGetBassBoostEnable(UINT8 u1DecId)
{
    return _AUD_DspGetBassBoostEnable(u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspBassBoostGain
 *   set the bass boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  u1Level the setting value is from 0 to 28, level 14 means 0db. A large
 *                         number will boost the bass and a smaller number will attenuate the bass.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspBassBoostGain(UINT8 u1DecId, UINT8 u1Level)
{
	_AUD_DspBassBoostGain( u1DecId,  u1Level);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetBassBoostGain
 *   get the bass boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *
 *  @retval bass gain value
 */
//-----------------------------------------------------------------------------
UINT32 AUD_DspGetBassBoostGain(UINT8 u1DecId)
{
    return _AUD_DspGetBassBoostGain(u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspTrebleBoostCtrl
 *   turn on / off treble boost control.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  fgEnable  ADSP_ON / ADSP_OFF.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSuperBassEnable(UINT8 u1DecId, BOOL fgEnable)
{
	_AUD_DspSuperBassEnable( u1DecId,  fgEnable);
}

// *********************************************************************
// Function : void AUD_DspTrebleBoostCtrl(BYTE bMode)
// Description : turn on / off treble boost control
// Parameter : ADSP_ON / ADSP_OFF
// Return    : None.
// *********************************************************************
void AUD_DspClearBoostEnable(UINT8 u1DecId, BOOL fgEnable)
{
	_AUD_DspClearBoostEnable( u1DecId,  fgEnable);
}

BOOL AUD_DspGetClearBoostEnable(UINT8 u1DecId)
{
    return _AUD_DspGetClearBoostEnable(u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspClearBoostGain
 *   set the clear boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  u1Level  range from 0 to 28, level 14 means 0db.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspClearBoostGain(UINT8 u1DecId, UINT8 u1Level)
{
	_AUD_DspClearBoostGain( u1DecId,  u1Level);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetClearBoostGain
 *   get the treble boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *
 *  @retval treble gain value
 */
//-----------------------------------------------------------------------------
UINT32 AUD_DspGetClearBoostGain(UINT8 u1DecId)
{
    return _AUD_DspGetClearBoostGain(u1DecId);
}

// *********************************************************************
// Function : void AUD_DspEqEnable(UINT8 u1DecId, BOOL fgEnable)
// Description : turn on / off equalizer control
// Parameter :  TRUE / FALSE
// Return    :  None.
// *********************************************************************
void AUD_DspEqEnable(UINT8 u1DecId, BOOL fgEnable)
{
	_AUD_DspEqEnable( u1DecId,  fgEnable);
}

// *********************************************************************
// Function : AUD_DspChEqBand(strucDspChBand rChBand, BYTE bLevel, BYTE fgNotify)
// Description : used to adjusting the equalizer's parameter for each channel
// Parameter : u1BandIdx -- EQ band index
//             bLevel  -- range is from 0 to 28
//             fgNotify -- to indicate changing the parameter immediately or not
// Return    : None.
// *********************************************************************
void AUD_DspChEqBand(UINT8 u1DecId, UINT8 u1BandIdx, UINT8 u1Level, BOOL fgNotify)
{
	_AUD_DspChEqBand( u1DecId,  u1BandIdx,  u1Level,  fgNotify);
}

//-----------------------------------------------------------------------------
/** AUD_DspChEqLoadPreset
 *   load the predefined channel equalizer setting.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  eEqType  the valid type number is from 0 to type 7.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspChEqLoadPreset(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType)
{
	_AUD_DspChEqLoadPreset( u1DecId,  eEqType);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetEqType
 *   get the type of current equalizer.
 *
 *  @param  u1DecId  Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  eEqType
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspGetEqType(UINT8 u1DecId, AUD_EQ_TYPE_T* eEqType)
{
    _AUD_DspGetEqType(u1DecId, eEqType);
}

//-----------------------------------------------------------------------------
/** AUD_DspChSetEqTable
 *   set the band gain of a specific equalizer.
 *
 *  @param  u1DecId  Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  eEqType  the valid type number is from 0 to type 7.
 *  @param  eqValue  band gain, valid value from -20 ~ 20
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspChSetEqTable(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, INT8 eqValue[])
{
    _AUD_DspChSetEqTable(u1DecId, eEqType, eqValue);
}

//-----------------------------------------------------------------------------
/** AUD_DspChQryEqTable
 *   query the band gain of a specific equalizer.
 *
 *  @param  u1DecId  Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  eEqType  the valid type number is from 0 to type 7.
 *  @param  eqValue  band gain, valid value from -20 ~ 20
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspChQryEqTable(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, INT8 eqValue[])
{
    _AUD_DspChQryEqTable(u1DecId, eEqType, eqValue);
}

// *********************************************************************
// Function : AUD_DspChEqBandMode(strucDspChBand rChBand, BYTE bLevel, BYTE fgNotify)
// Description : used to adjusting the equalizer's parameter for each channel and
// Parameter : u1BandIdx -- EQ band index
//             bLevel  -- range is from 0 to 28
//             fgNotify -- to indicate changing the parameter immediately or not
// Return    : None.
// *********************************************************************
void AUD_DspChEqBandMode(UINT8 u1DecId, UINT8 u1BandIdx, UINT8 u1Level, BOOL fgNotify)
{
	_AUD_DspChEqBandMode( u1DecId,  u1BandIdx,  u1Level,  fgNotify);
}

//-----------------------------------------------------------------------------
/** AUD_DspChEqLoadPresetChgMode
 *   load the predefined channel equalizer setting for mode changing.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  eEqType  the valid type number is from 0 to type 7.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspChEqLoadPresetChgMode(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType)
{
	_AUD_DspChEqLoadPresetChgMode( u1DecId,  eEqType);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetLimiterMode
 *   set limiter mode
 *
 *  @param  u1Mode  limiter mode (0: off, 1: adaptive mode, 2: fixed mode).
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetLimiterMode(UINT8 u1Mode)
{
    _AUD_DspSetLimiterMode(u1Mode);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetLimiterFlag
 *   set limiter flag
 *
 *  @param  u1Flag  limiter flag (0: off, 1: on)
 *                         bit 0: vol
 *                         bit 1: vsurr
 *                         bit 2: sbass
 *                         bit 3: EQ
 *                         bit 4: reverb
 *                         bit 5: speaker
 *                         bit 6: voice
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetLimiterFlag(UINT8 u1Flag)
{
    _AUD_DspSetLimiterFlag(u1Flag);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetLimiterThreshold
 *   set limiter threshold
 *
 *  @param  u4Thre  limiter threshold (0x0 ~ 0x7fffff)
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetLimiterThreshold(UINT32 u4Thre)
{
    _AUD_DspSetLimiterThreshold(u4Thre);
}

#ifdef CC_RATIO_LIMITER
//-----------------------------------------------------------------------------
/** AUD_DspSetLimiterGainRatio
 *   set limiter gain ratio
 *
 *  @param  u4Ratio  limiter gain ratio (0x0 ~ 0x7fffff)
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetLimiterGainRatio(UINT32 u4Ratio)
{
    _AUD_DspSetLimiterGainRatio(u4Ratio);
}
#endif

//-----------------------------------------------------------------------------
/** AUD_DspGetLimiterConfig
 *   get limiter configuration
 *
 *  @param  *pu1Mode  mode
 *  @param  *pu2Flag  flag
 *  @param  *pu4Thre  threshold
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspGetLimiterConfig(UINT8 *pu1Mode, UINT16 *pu2Flag, UINT32 *pu4Thre)
{
    _AUD_DspGetLimiterConfig(pu1Mode, pu2Flag, pu4Thre);
}

//-----------------------------------------------------------------------------
/** AUD_DspPL2Config
 *  main processing routine for Prologic 2.
 *
 *  @param  uCtrl control
                         PL2CTRL_SWITCH          0.
                         PL2CTRL_MODE            1.
                         PL2CTRL_PANORAMA        2.
                         PL2CTRL_DIMENSION       3.
                         PL2CTRL_C_WIDTH         4.
 *  @param  uMode corresponding parameters for PL2 control (uCtrl).
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspPL2Config(UINT8 uCtrl, UINT8 uMode)
{
	_AUD_DspPL2Config( uCtrl,  uMode);
}
/******************************************************************************
* Function      : AUD_DspPL2ConfigInit
* Description   : initialization for PL2 configuration
* Parameter     : u2PL2cfg--> W_PLIICONFIG,u2PL2Mode-->W_PLIIMODE
* Return        : None
******************************************************************************/
void AUD_DspPL2ConfigInit(UINT16 u2PL2cfg,UINT16 u2PL2Mode)
{
	_AUD_DspPL2ConfigInit( u2PL2cfg, u2PL2Mode);
}
/******************************************************************************
* Function      : AUD_DspAC3KaraMode
* Description   : setup for AC3 karaoke mode
* Parameter     : u2KaraMode: Karaoke mode,uDecIndx: 0: first decoder 1: seconder decoder
* Return        : None
******************************************************************************/
void AUD_DspAC3KaraMode(UINT16 u2KaraMode,UINT8 uDecIndx)
{
	_AUD_DspAC3KaraMode( u2KaraMode, uDecIndx);
}
/******************************************************************************
* Function      : AUD_DspAC3DualMono
* Description   : setup for AC3 dual mono mode
* Parameter     : u2DMMode: dual mono mode,uDecIndx: 0: first decoder 1: seconder decoder
* Return        : None
******************************************************************************/
void AUD_DspAC3DualMono(UINT16 u2DMMode,UINT8 uDecIndx)
{
	_AUD_DspAC3DualMono( u2DMMode, uDecIndx);
}

//-----------------------------------------------------------------------------
/** AUD_DspAC3CompMode
 *  setup for compression mode.
 *
 *  @param  u2CompMode compression mode.
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspAC3CompMode(UINT16 u2CompMode,UINT8 uDecIndx)
{
	_AUD_DspAC3CompMode( u2CompMode, uDecIndx) ;
    // DolbyGainTB11
    LOG(5, "@@@@AUD_DspAC3CompMode@@@@\n");
    _AUD_DspDolbyGainTB11(uDecIndx);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetAC3CompMode
 *  setup for compression mode.
 *
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval UINT16 u2CompMode
 */
//-----------------------------------------------------------------------------
UINT16 AUD_DspGetAC3CompMode(UINT8 uDecIndx)
{
	return _AUD_DspGetAC3CompMode(uDecIndx) ;
}

//-----------------------------------------------------------------------------
/** AUD_DspAC3DRCRange
 *  setup range for dynamix range compression.
 *
 *  @param  uDRCLevel  DRC range.
 *                               0x00000000-->0.0
 *                               0x007FFFFF-->1.0
 *                               0.0 ~ 1.0 step 0.125
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspAC3DRCRange(UINT8 uDRCLevel,UINT8 u1DecId)
{
	_AUD_DspAC3DRCRange( uDRCLevel, u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspAC3DRCRange100
 *  setup range for dynamix range compression.
 *
 *  @param  uDRCLevel  DRC range 0~100.
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspAC3DRCRange100(UINT8 uDRCLevel,UINT8 u1DecId)
{
	_AUD_DspAC3DRCRange100( uDRCLevel, u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetAC3DRCRange
 *  setup range for dynamix range compression.
 *
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval UINT8  uDRCLevel  DRC range.
 *                               0x00000000-->0.0
 *                               0x007FFFFF-->1.0
 *                               0.0 ~ 1.0 step 0.125
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetAC3DRCRange(UINT8 u1DecId)
{
	return _AUD_DspGetAC3DRCRange(u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetAC3DRCRange100
 *  setup range for dynamix range compression.
 *
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval UINT8  uDRCLevel  DRC range.
 *                               0x00000000-->0.0
 *                               0x007FFFFF-->1.0
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetAC3DRCRange100(UINT8 u1DecId)
{
	return _AUD_DspGetAC3DRCRange100(u1DecId);
}

#ifdef CC_AUD_SUPPORT_MS10
//-----------------------------------------------------------------------------
/** AUD_DspSetDDCStrmId
 *  setup range for associated stream id.
 *
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetDDCControl (UINT16 u2Control, UINT8 u1DecId)
{
	_AUD_DspSetDDCControl (u2Control, u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetDDCStrmId
 *  setup range for associated stream id.
 *
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetDDCStrmId (UINT8 u1StrmId, UINT8 u1DecId)
{
	_AUD_DspSetDDCStrmId (u1StrmId, u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetDDCAdEnable
 *  setup for DDC AD.
 *
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetDDCAdEnable (UINT8 u1DecId, BOOL fgEnable)
{
	_AUD_DspSetDDCAdEnable (u1DecId, fgEnable);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetDDCMixer
 *  setup for DDC mixer.
 *
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetDDCMixer (UINT8 u1Control, UINT8 u1DecId)
{
	_AUD_DspSetDDCMixer (u1Control, u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspDdtDmxMode
 *  setup for DDT dmx mode.
 *
 *  @param  u1Mode: dmxmode/RF mode, u1DecId 0: first decoder 1: seconder decoder. 2: third decoder, bSetDmx 0: set dmx, 1: set RF
 *  @retval void
 *  u1Mode:
 *  00b: Line, Lt/Rt
 *  01b: Line, Lo/Ro
 *  10b: RF, Lt/Rt
 *  11b: RF, Lo/Ro
 */
//-----------------------------------------------------------------------------
void AUD_DspDdtDmxRfMode (UINT8 u1Mode, UINT8 u1DecId, BOOL bSetDmx)
{
	_AUD_DspDdtDmxRfMode (u1Mode, u1DecId, bSetDmx);
    // DolbyGainTB11
    LOG(5, "@@@@AUD_DspDdtDmxRfMode@@@@\n");
    _AUD_DspDolbyGainTB11(u1DecId);	
}

//-----------------------------------------------------------------------------
/** AUD_DspDdtDualMode
 *  setup for DDT dual mode.
 *
 *  @param  u1Mode: dual mode, u1DecId 0: first decoder 1: seconder decoder. 2: third decoder
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspDdtDualMode (UINT8 u1Mode, UINT8 u1DecId)
{
	_AUD_DspDdtDualMode (u1Mode, u1DecId);
}


//-----------------------------------------------------------------------------
/** AUD_DspDdtCompVal
 *  setup for DDT DRC cut/boost factor.
 *
 *  @param  u1Mode: cut/boost factor 0~100, u1DecId 0: first decoder 1: seconder decoder. 2: third decoder
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspDdtCompVal (UINT8 u1Mode, UINT8 u1DecId)
{
	_AUD_DspDdtCompValRange (u1Mode, u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetDdtDmxRfMode
 *  setup for DDT dmx mode.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder. 2: third decoder, bGetDmx 0: get dmx, 1: get RF
 *  @retval void
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetDdtDmxRfMode (UINT8 u1DecId,BOOL bGetDmx)
{
	return _AUD_DspGetDdtDmxRfMode (u1DecId,bGetDmx);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetDdtDualMode
 *  setup for DDT dual mode.
 *
 *  @param  u1Mode: dual mode, u1DecId 0: first decoder 1: seconder decoder. 2: third decoder
 *  @retval void
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetDdtDualMode (UINT8 u1DecId)
{
	return _AUD_DspGetDdtDualMode (u1DecId);
}


//-----------------------------------------------------------------------------
/** AUD_DspGetDdtDrcRange
 *  setup for DDT DRC cut/boost factor.
 *
 *  @param  u1Mode: cut/boost factor 0~100, u1DecId 0: first decoder 1: seconder decoder. 2: third decoder
 *  @retval void
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetDdtDrcRange (UINT8 u1DecId)
{
	return _AUD_DspGetDdtDrcRange (u1DecId);
}

#endif  //CC_AUD_SUPPORT_MS10


/******************************************************************************
* Function      : AdspSpeakerConfig
* Description   : speaker configuration setup
* Parameter     : u4SpkCfg: Speaker config,uDecIndx: 0: first decoder 1: seconder decoder
* Return        : None
******************************************************************************/
/*
  D_SPKCFG,D_SPKCFG_DEC2 (currently second decoder only support 2/0,1/0,3/0 configuration)
  length: 24 bits
  description is the following
  bit 0 ~ 2:
    b000: 2/0 (LT/RT downmix: prologic compatible)
    b001: 1/0
    b010: 2/0 (LO/RO)
    b011: 3/0
    b100: 2/1
    b101: 3/1
    b110: 2/2
    b111: 3/2
  bit 5: Subwoofer Channel present(1)/absent(0)
  bit 12: Center Channel large(1)/small(0)
  bit 13: Left Channel large(1)/small(0)
  bit 14: Right Channel large(1)/small(0)
  bit 15: Left Surround Channel large(1)/small(0)
  bit 16: Right Surround Channel large(1)/small(0)
  bit 19: Downmix CH9  large(1)/small(0)
  bit 20: Downmix CH10 large(1)/small(0)
*/
void AUD_DspSpeakerConfig(UINT32 u4SpkCfg,UINT8 uDecIndex)
{
	_AUD_DspSpeakerConfig( u4SpkCfg, uDecIndex);
}

UINT32 AUD_DspGetSpeakerConfig(UINT8 uDecIndex)
{
    return _AUD_DspGetSpeakerConfig(uDecIndex);
}

/******************************************************************************
* Function      : AUD_DspSpeakerSizeConfig
* Description   : speaker configuration setup
* Parameter     : u4SpkCfg: Speaker config,uDecIndx: 0: first decoder 1: seconder decoder
* Return        : None
******************************************************************************/
/*
  D_SPKCFG,D_SPKCFG_DEC2 (currently second decoder only support 2/0,1/0,3/0 configuration)
  length: 24 bits
  description is the following
  bit 0 ~ 2:
    b000: 2/0 (LT/RT downmix: prologic compatible)
    b001: 1/0
    b010: 2/0 (LO/RO)
    b011: 3/0
    b100: 2/1
    b101: 3/1
    b110: 2/2
    b111: 3/2
  bit 5: Subwoofer Channel present(1)/absent(0)
  bit 12: Center Channel large(1)/small(0)
  bit 13: Left Channel large(1)/small(0)
  bit 14: Right Channel large(1)/small(0)
  bit 15: Left Surround Channel large(1)/small(0)
  bit 16: Right Surround Channel large(1)/small(0)
*/
void AUD_DspSpeakerSizeConfig(UINT8 uDecIndex, BOOL fgFrontLarge, BOOL fgSurroundLarge, BOOL fgCenterLarge)
{
	_AUD_DspSpeakerSizeConfig( uDecIndex,  fgFrontLarge,  fgSurroundLarge,  fgCenterLarge);
}

//-----------------------------------------------------------------------------
/** AUD_DspSpeakerLargeSmallConfig
 *  Speaker size configuration of each channel pair.
 *  @param  u1DecIndex          Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  u1FrontPair          Speaker size configuration for front left and right channels. (1: large, 0: small)
 *  @param  u2Center             Speaker size configuration for center channel. (1: large, 0: small)
 *  @param  u1Surround          Speaker size configuration for left and right surround channels. (1: large, 0: small)
 *  @param  u1CH910              Speaker size configuration for CH9/10 channels. (1: large, 0: small)

 *  @return void
 */
//-----------------------------------------------------------------------------
// large(1)/small(0)
void AUD_DspSpeakerLargeSmallConfig(UINT8 u1DecIndex, UINT8 u1FrontPair, UINT8 u2Center, UINT8 u1Surround, UINT8 u1CH910)
{
	_AUD_DspSpeakerLargeSmallConfig(u1DecIndex, u1FrontPair,u2Center,u1Surround, u1CH910);
}

void AUD_DspGetSpeakerLargeSmallConfig(UINT8 u1DecIndex, UINT8* u1FrontPair, UINT8* u1Center, UINT8* u1Surround, UINT8* u1CH910)
{
    _AUD_DspGetSpeakerLargeSmallConfig(u1DecIndex,u1FrontPair,u1Center,u1Surround,u1CH910);
}

//-----------------------------------------------------------------------------
/** AUD_DspSpeakerLsConfig
 *  setup speaker size of each channel.
 *
 *  @param  u1DecIndex 0: first decoder 1: seconder decoder.
 *  @param  u1L  large(1)/small(0).
 *  @param  u1R  large(1)/small(0).
 *  @param  u1Ls   large(1)/small(0).
 *  @param  u1Rs   large(1)/small(0).
 *  @param  u1C  large(1)/small(0).
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSpeakerLsConfig(UINT8 u1DecIndex, UINT8 u1L, UINT8 u1R, UINT8 u1Ls, UINT8 u1Rs, UINT8 u1C)
{
	_AUD_DspSpeakerLsConfig( u1DecIndex,  u1L,  u1R,  u1Ls,  u1Rs,  u1C);
}

//static UINT32 _au4SpeakerOutCfg[AUD_DEC_NUM]= {7,7};
//#define SPK_CFG_MASK (0x07)

//-----------------------------------------------------------------------------
/** AUD_DspSpeakerOutputConfig
 *  set speaker output config.
 *
 *  @param  u1DecIndex 0: first decoder 1: seconder decoder.
 *  @param  u1SpkCfg.
 *                              bit 0 ~ 2:.
 *                              b000: 2/0 (LT/RT downmix: prologic compatible).
 *                              b001: 1/0.
 *                              b010: 2/0 (LO/RO).
 *                              b011: 3/0.
 *                              b100: 2/1.
 *                              b101: 3/1.
 *                              b110: 2/2.
 *                              b111: 3/2.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSpeakerOutputConfig(UINT8 u1DecIndex, UINT8 u1SpkCfg)
{
	_AUD_DspSpeakerOutputConfig( u1DecIndex,  u1SpkCfg);
}

UINT8 AUD_DspGetSpeakerOutputConfig(UINT8 u1DecIndex)
{
    return _AUD_DspGetSpeakerOutputConfig(u1DecIndex);
}

//-----------------------------------------------------------------------------
/** AUD_DspSpeakerSubwooferEnable
 *  Turn On/Off subwoofer
 *  @param  u1DecIndex          Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  fgEnable              Subwoofer channel is turned on or off. (1: on, 0: off)
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
// bit 5: Subwoofer Channel present(1)/absent(0)
void AUD_DspSpeakerSubwooferEnable(UINT8 u1DecIndex, BOOL fgEnable)
{
	_AUD_DspSpeakerSubwooferEnable(u1DecIndex, fgEnable);
}

BOOL AUD_DspGetSpeakerSubwooferEnable(UINT8 u1DecIndex)
{
    return _AUD_DspGetSpeakerSubwooferEnable(u1DecIndex);
}

//-----------------------------------------------------------------------------
/** AUD_DspChannelDelay
 *  setup channel delay for individual channel.
 *
 *  @param  u2Delay 0.05 m/unit.
*  @param  eChIndex 0(L), 1(R), 2(LS), 3(RS), 4(C), 5(SUB), 6(Bypass L), 7(Bypass R)
 *                   8(Downmix L), 9(Downmix R), 10(AUD L), 11(AUD R), 13(All), 14(LFE)
 *  @param  u1DecIndex 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspChannelDelay(UINT16 u2Delay, AUD_CH_T eChIndex, UINT8 uDecIndex)
{
    _AUD_DspChannelDelay(u2Delay, eChIndex, uDecIndex);
}

/** AUD_DspChannelDelayNoUop
 *  setup channel delay for individual channel.
 *
 *  @param  u2Delay 0.05 m/unit.
*  @param  eChIndex 0(L), 1(R), 2(LS), 3(RS), 4(C), 5(SUB), 6(Bypass L), 7(Bypass R)
 *                   8(Downmix L), 9(Downmix R), 10(AUD L), 11(AUD R), 13(All), 14(LFE)
 *  @param  u1DecIndex 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspChannelDelayNoUop(UINT16 u2Delay, AUD_CH_T eChIndex, UINT8 uDecIndex)
{
    _AUD_DspChannelDelayNoUop(u2Delay, eChIndex, uDecIndex);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetChannelDelay
 *  Get channel delay for individual channel.
 *
 *  @param  eChIndex 0(L), 1(R), 2(LS), 3(RS), 4(C), 5(SUB), 6(Bypass L), 7(Bypass R)
 *                   8(Downmix L), 9(Downmix R), 10(AUD L), 11(AUD R), 13(All), 14(LFE)
 *  @retval channel delay
 */
//-----------------------------------------------------------------------------
UINT16 AUD_DspGetChannelDelay(AUD_CH_T eChIndex)
{
    return _AUD_DspGetChannelDelay(eChIndex);
}

#ifdef CC_ENABLE_AV_SYNC
//-----------------------------------------------------------------------------
/** AUD_DspChannelDelay_initial
 *  setup channel delay by input source.
 *
 *  @param  eStreamFrom
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspChannelDelay_initial(UINT16 u2Delay, AUD_CH_T eChIndex, UINT8 uDecIndex)
{
    _AUD_DspChannelDelay_initial(u2Delay, eChIndex, uDecIndex);
}
#endif

//-----------------------------------------------------------------------------
/** AUD_DspInputChannelDelay
 *  setup channel delay by input source.
 *
 *  @param  eStreamFrom
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspInputChannelDelay(AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    _AUD_DspInputChannelDelay(eStreamFrom);
}

//-----------------------------------------------------------------------------
/** AUD_DspChannelVolume
 *  Get channel volume of individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetChannelVolume(UINT8 u1DecId, AUD_CH_T eChannel)
{
	return _AUD_DspGetChannelVolume( u1DecId,  eChannel);
}

//-----------------------------------------------------------------------------
/** _AUD_DspChannelVolShmRawValue
 *  Set master volume use share memory raw data .
 *
 *  Note that AUD_DspChannelVolume & _AUD_DspChannelVolShmRawValue can only select
 *  one to use
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u4VolShm : 0 ~ 0x20000
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspChannelVolShmRawValue(UINT8 u1DecId, AUD_CH_T eChannel, UINT32 u4VolShm)
{
    _AUD_DspChannelVolShmRawValue(u1DecId, eChannel, u4VolShm);
}

//-----------------------------------------------------------------------------
/** AUD_DspChannelVolume
 *  setup channel volume for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  u1Value 0~100.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspChannelVolume(UINT8 u1DecId, AUD_CH_T eChannel, UINT8 u1Value)
{
	_AUD_DspChannelVolume( u1DecId,  eChannel,  u1Value);
}

//-----------------------------------------------------------------------------
/** AUD_DspPanFadePesUpdateEn
 *  setup ad pan fade pes update enable
 *
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspPanFadePesUpdateEn(BOOL bEnable)
{
	_AUD_DspPanFadePesUpdateEn(bEnable);
}

//-----------------------------------------------------------------------------
/** AUD_DspAdPanFadeEnable
 *  setup ad fade volume for individual channel.
 *
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspAdPanFadeEnable(BOOL bEnable)
{
	_AUD_DspAdPanFadeEnable(bEnable);
}

UINT8 AUD_DspGetAdPanFadeSetting(void)
{
    return _AUD_DspGetAdPanFadeSetting();
}

//-----------------------------------------------------------------------------
/** AUD_DspAdFadeVolumeSet
 *  setup ad fade volume for individual channel.
 *
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspAdFadeVolumeSet(UINT8 u1Value)
{
	_AUD_DspAdFadeVolumeSet(u1Value);
}

//-----------------------------------------------------------------------------
/** AUD_DspAdFadeVolumeUpdate
 *  setup ad fade volume for individual channel.
 *
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspAdFadeVolumeUpdate(UINT8 u1DecId, AUD_CH_T eChannel)
{
	_AUD_DspAdFadeVolumeUpdate(u1DecId, eChannel);
}

//-----------------------------------------------------------------------------
/** AUD_DspAdPanVolumeSet
 *  setup ad pan volume for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  u1Value 0~100.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspAdPanVolumeSet(UINT8 u1DecId, AUD_CH_T eChannel, UINT8 u1Value)
{
	_AUD_DspAdPanVolumeSet(u1DecId, eChannel, u1Value);
}

//-----------------------------------------------------------------------------
/** AUD_DspAdPanVolumeUpdate
 *  setup ad pan volume for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  u1Value 0~100.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspAdPanVolumeUpdate(UINT8 u1DecId, AUD_CH_T eChannel)
{
	_AUD_DspAdPanVolumeUpdate(u1DecId, eChannel);
}

#ifdef CC_AUD_LINEOUT_VOL_OFFSET
void AUD_DspLineOutVolOffset(UINT8 u1DecId,UINT8 u1Value)
{
	_AUD_DspLineOutVolOffset( u1DecId, u1Value);
}
#endif
//-----------------------------------------------------------------------------
/** AUD_DspWMAHeader
 *  setup WMA information
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  prWmaInfo  : pointer of WMA header information
 *  @retval void
 */
//-----------------------------------------------------------------------------
#ifdef DSP_WMA_MWIF
void AUD_DspWMAHeader(UINT8 u1DecId, AUD_WMA_CFG_T* prWmaInfo)
{
//#ifdef DATA_DISC_WMA_SUPPORT
	_AUD_DspWMAHeader(u1DecId,prWmaInfo); //_AUD_DspWMAHeader(UINT8 u1DecId,AUD_WMA_CFG_T* prWmaInfo)
//#endif
}
#endif

void AUD_DspWMAProHeader(UINT8 u1DecId, AUD_WMA_CFG_T* prWmaInfo)
{
	_AUD_DspWMAProHeader(u1DecId,prWmaInfo); //_AUD_DspWMAHeader(UINT8 u1DecId,AUD_WMA_CFG_T* prWmaInfo)
}

//-----------------------------------------------------------------------------
/** AUD_DspChannelMute
 *  setup mute for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  fgMute TRUE, FALSE
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspChannelMute(UINT8 u1DecId, AUD_CH_T eChannel, BOOL fgMute)
{
    _AUD_DspChannelMute(u1DecId, eChannel, fgMute);
}

#ifdef CC_AUD_VOLUME_OFFSET
void AUD_DspVolumeOffset(UINT8 u1DecId,UINT8 u1Index)
{
    _AUD_DspVolumeOffset(u1DecId,u1Index);
}
#endif

void AUD_DspClearSoundEnable(UINT8 u1DecId, BOOL fgEnable)
{
    _AUD_DspClearSoundEnable(u1DecId, fgEnable);
}

void AUD_DspClearSoundCfg(UINT8 u1UserQIdx, UINT16 u2UserFc)
{
    _AUD_DspClearSoundCfg(u1UserQIdx, u2UserFc);
}

//-----------------------------------------------------------------------------
/** AUD_DspChannelVolGain
 *  setup channel volume extra gain for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  i4Value     -256~96.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspChannelVolGain(UINT8 u1DecId, AUD_CH_T eChannel, INT32 i4Value)
{
    _AUD_DspChannelVolGain( u1DecId,  eChannel,  i4Value);
}

//-----------------------------------------------------------------------------
/** AUD_DspChannelVolDolbyGainTB11
 *  setup channel volume Dolby gain TB11 for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  i4Value     -256~96.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspChannelVolDolbyGainTB11(UINT8 u1DecId, AUD_CH_T eChannel, INT32 i4Value)
{
    _AUD_DspChannelVolDolbyGainTB11( u1DecId,  eChannel,  i4Value);
}

/***************************************************************************
     Function : AUD_DspDolbyGainTB11
     Description: Dolby Technical Bulletin 11.5 (addtional 3dB attenuation for EU)
     @param	u1DecId
     			AUD_DEC_MAIN: 1st dec
     			AUD_DEC_AUX: 2nd dec
     @retval	void
***************************************************************************/
void AUD_DspDolbyGainTB11(UINT8 u1DecId)
{
    _AUD_DspDolbyGainTB11(u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetChannelVolGain
 *  Get channel volume extra gain for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *
 *  @retval extra gain (-256~96)
 */
//-----------------------------------------------------------------------------
INT32 AUD_DspGetChannelVolGain(UINT8 u1DecId, AUD_CH_T eChannel)
{
    return _AUD_DspGetChannelVolGain(u1DecId, eChannel);
}

//-----------------------------------------------------------------------------
/** AUD_DspChannelOutPort
 *  setup output port for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  fgEnable TRUE, FALSE
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspChannelOutPort(UINT8 u1DecId, AUD_CH_T eChannel, BOOL fgEnable)
{
    _AUD_DspChannelOutPort(u1DecId, eChannel, fgEnable);
}

//-----------------------------------------------------------------------------
/** AUD_DspBalance
 *  setup balance.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Balance  : balance value (0~100).
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspBalance(UINT8 u1DecId, UINT8 u1Balance)
{
	_AUD_DspBalance( u1DecId,  u1Balance);
}

//-----------------------------------------------------------------------------
/** AUD_DspMasterVolumeFineTune
 *  setup master volume to DSP directly.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u4Volume  : volume value (0~0x7fffff).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
// 2006/08/28, created for tuning volume table
void AUD_DspMasterVolumeFineTune(UINT8 u1DecId, UINT32 u4Volume)
{
	_AUD_DspMasterVolumeFineTune( u1DecId,  u4Volume);
}

//-----------------------------------------------------------------------------
/** AUD_DspMasterVolume
 *  set master volume.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Volume  : volume value (0~100).
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspMasterVolume(UINT8 u1DecId, UINT8 u1Volume)
{
	_AUD_DspMasterVolume( u1DecId,  u1Volume);
}

UINT8 AUD_DspGetMasterVolume(UINT8 u1DecId)
{
    return _AUD_DspGetMasterVolume(u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspMasterVolume
 *  set master volume.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Volume  : volume value (0~100).
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspMixSndVolume(UINT8 u1DecId, UINT8 u1Volume)
{
	_AUD_DspMixSndVolume(u1DecId, u1Volume);
}

//-----------------------------------------------------------------------------
/** AUD_DspMasterVolShmRawValue
 *  Set master volume use share memory raw data .
 *
 *  Note that AUD_DspMasterVolume & _AUD_DspMasterVolShmRawValue can only select
 *  one to use
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u4VolShm : 0 ~ 0x20000
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspMasterVolShmRawValue(UINT8 u1DecId, UINT32 u4VolShm)
{
    _AUD_DspMasterVolShmRawValue(u1DecId, u4VolShm);
}

//-----------------------------------------------------------------------------
/** AUD_DspAVCEnable
 *  enable AVC.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgEnable  : on/off.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspAVCEnable(UINT8 u1DecId, BOOL fgEnable)
{
	_AUD_DspAVCEnable( u1DecId,  fgEnable);
}

//-----------------------------------------------------------------------------
/** AUD_DspChangeAVCPara
 *  setup automatic volume control parameters.
 *
 *  @param  u2Type     parameter selection
 *                             0: AVC on/off (u2Value = 0: AVC off, 1: AVC on)
 *                             1: AVC target level (dB) (u2Value = -6 ~ -24)
 *                             3: Max gain up (dB) (u2Value = 0 ~ 8)
 *                             5: Adjust rate (dB/128ms)
 *                             6: Adjust rate when AVC is turned on in UI (dB/128ms)
 *  @param  u2Value    parameter value
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspChangeAVCPara(UINT16 u2Type, INT16 u2Value, UINT8 u1DecId)
{
	_AUD_DspChangeAVCPara( u2Type,  (UINT16)u2Value,  u1DecId);
}


void AUD_DspGetAVCPara(UINT16 u2Type, UINT16* u2Value, UINT8 u1DecId)
{
    _AUD_DspGetAVCPara(u2Type, u2Value, u1DecId);
}
//-----------------------------------------------------------------------------
/** AUD_DspChange2BandAVCPara
 *  setup automatic volume control parameters.
 *
 *  @param  u2Type     parameter selection
 *                             0: AVC on/off (u2Value = 0: AVC off, 1: AVC on)
 *                             1: AVC target level (dB) (u2Value = -6 ~ -24)
 *                             3: Max gain up (dB) (u2Value = 0 ~ 8)
 *                             5: Adjust rate (dB/128ms)
 *                             6: Adjust rate when AVC is turned on in UI (dB/128ms)
 *  @param  u2Value    parameter value
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspChange2BandAVCPara(UINT8 u1Band, UINT16 u2Type, UINT16 u2Value, UINT8 u1DecId)
{
    _AUD_DspChange2BandAVCPara(u1Band, u2Type, u2Value, u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspInputChangeAVCPara
 *  setup automatic volume control parameters by input source.
 *
 *  @param  eStreamFrom
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspInputChangeAVCPara(AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    _AUD_DspInputChangeAVCPara(eStreamFrom);
}

//-----------------------------------------------------------------------------
/** AUD_DspMuteEnable
 *  audio mute control.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Mute/unmute. (1: mute audio, 0: unmute audio)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspMuteEnable(UINT8 u1DecId, BOOL fgEnable)
{
	_AUD_DspMuteEnable( u1DecId,  fgEnable);
}

//-----------------------------------------------------------------------------
/** AUD_DspMuteAllEnable
 *  audio mute control.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Mute/unmute. (1: mute audio, 0: unmute audio)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspMuteAllEnable(UINT8 u1DecId, BOOL fgEnable)
{
    _AUD_DspMuteAllEnable(u1DecId, fgEnable);
}

//-----------------------------------------------------------------------------
/** AUD_DspHdmiModeMuteEnable
 *  audio mute control in HDMI mode.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Mute/unmute. (1: mute audio, 0: unmute audio)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspHdmiModeMuteEnable(UINT8 u1DecId, BOOL fgEnable)
{
	_AUD_DspHdmiModeMuteEnable( u1DecId,  fgEnable);
}

//-----------------------------------------------------------------------------
/** AUD_DspBassMngEnalbe
 *  Bass management control.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Bass management enable. (1: enable, 0: disable)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspBassMngEnalbe(UINT8 u1DecId, BOOL fgEnable)
{
	_AUD_DspBassMngEnalbe( u1DecId,  fgEnable);
}

BOOL AUD_DspGetBassMngEnable(UINT8 u1DecId)
{
    return _AUD_DspGetBassMngEnable(u1DecId);
}

BOOL AUD_DspGetChannelVolumeEnable(UINT8 u1DecId)
{
    return _AUD_DspGetChannelVolumeEnable(u1DecId);
}
BOOL AUD_DspGetDelayEnable(UINT8 u1DecId)
{
    return _AUD_DspGetDelayEnable(u1DecId);
}

BOOL AUD_DspGetPostProcessingEnable(UINT8 u1DecId)
{
    return _AUD_DspGetPostProcessingEnable(u1DecId);
}

void AUD_DspProcMode(UINT8 u1DecId, UINT16 u2Mode)
{
	_AUD_DspProcMode( u1DecId,  u2Mode);
}

//-----------------------------------------------------------------------------
/** AUD_DspVirtualSurroundEnalbe
 *  enable virtual surround.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgEnable  : on/off.
 *  @retval void
 */
//-----------------------------------------------------------------------------
#ifndef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
void AUD_DspVirtualSurroundEnalbe(UINT8 u1DecId, BOOL fgEnable)
{
	_AUD_DspVirtualSurroundEnalbe( u1DecId,  fgEnable);
}
#else
void AUD_DspVirtualSurroundEnalbe(UINT8 u1DecId, UINT8 u1ModeIdx)
{
	_AUD_DspVirtualSurroundEnalbe( u1DecId,  u1ModeIdx);
}
void AUD_DspVSModeChange(UINT8 u1DecId,UINT8 u1ModeIdx)
{
	_AUD_DspVSModeChange(u1DecId, u1ModeIdx);
}
#endif //CC_AUD_NEW_POST_MULTI_VS_SUPPORT

BOOL AUD_DspGetVirtualSurroundEnalbe(UINT8 u1DecId)
{
    return _AUD_DspGetVirtualSurroundEnalbe(u1DecId);
}

#ifdef CC_AUD_SOUND_MODE
void AUD_DspPostSoundModeChange(UINT8 u1DecId,UINT8 u1ModeIdx)
{
	_AUD_DspPostSoundModeChange(u1DecId, u1ModeIdx);
}

void AUD_DspPSMCfgInit(void)
{
	_AUD_DspPSMCfgInit();
}
#endif

#ifdef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
void AUD_DspVSMCfgInit(void)
{
	_AUD_DspVSMCfgInit();
}
#endif

void AUD_DspPostSoundMode(UINT8 u1DecId,UINT8 u1ModeIdx)
{
	_AUD_DspPostSoundMode(u1DecId, u1ModeIdx, TRUE);
}

//-----------------------------------------------------------------------------
/** AUD_DspPostSoundModeStart
 *  Indicate sound mode start/end
 *
 *  @param  u1DecId          Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  fgStart          start(TRUE), end(FALSE).
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void AUD_DspPostSoundModeStart(UINT8 u1DecId, BOOL fgStart)
{
    _AUD_DspPostSoundModeStart(u1DecId, fgStart);
}

void AUD_DspADModeSet(UINT8 u1DecId, UINT16 u2Value)
{
    //Audio Description
    _AUD_DspADModeSet(u1DecId,  u2Value);
}


void AUD_DspADEnable(UINT8 u1DecId, BOOL fgEnable)
{
    //Audio Description
    _AUD_DspADEnable(u1DecId,  fgEnable);
}

#ifdef CC_AUD_SKYPE_SUPPORT
void AUD_DspSkypeRingMix(BOOL fgEnable)
{
    _AUD_DspSkypeRingMix(fgEnable);
}
void AUD_DspAuxMixToMain(BOOL fgEnable)
{
    _AUD_DspAuxMixToMain(fgEnable);
}
#endif

#ifdef CC_AUD_PCM_LINE_IN_LPF
void AUD_DSPSetPCMLineInLPF(BOOL fgEnable)
{
    _AUD_DSPSetPCMLineInLPF(fgEnable);
    return;
}
#endif

//-----------------------------------------------------------------------------
/** AUD_DspMonoDownMix
 *  enable mono downmix.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgEnable  : on/off.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspMonoDownMix(UINT8 u1DecId, BOOL fgEnable)
{
	_AUD_DspMonoDownMix( u1DecId,  fgEnable);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetMatrixOutMode
 *  setup for matrix output mode.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eMode 0: stereo (disable), 1: L mono, 2: R mono, 3: mix mono
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetMatrixOutMode(UINT8 u1DecId, AUD_MATRIX_OUT_T eMode)
{
	_AUD_DspSetMatrixOutMode(u1DecId, eMode);
}

UINT8 AUD_DspGetMatrixOutMode(UINT8 u1DecId)
{
    return _AUD_DspGetMatrixOutMode(u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspAutoDmx
 *  Setup auto downmix mode for AC3
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgEnable  : 0: disable AC3 auto downmix, 1: enable AC3 auto downmix
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspAutoDmx(UINT8 u1DecId, BOOL fgEnable)
{
	_AUD_DspAutoDmx( u1DecId,  fgEnable);
}

//-----------------------------------------------------------------------------
/** AUD_GetDspAutoDmx
 *  Get auto downmix mode for AC3
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgEnable  : 0: disable AC3 auto downmix, 1: enable AC3 auto downmix
 *  @retval void
 */
//-----------------------------------------------------------------------------
BOOL AUD_GetDspAutoDmx(UINT8 u1DecId)
{
	return (_AUD_GetDspAutoDmx(u1DecId));
}

//-----------------------------------------------------------------------------
/** AUD_DspBypassPostProcess
 *  Bypass audio post-processing.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u2BypassMode  : 1: bypass post-processing.
 *                          0x8000: bypass delay
 *                          0x4000: bypass trim
 *                          0x2000: bypass post-processing
 *                          0x1000: bypass bass management
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspBypassPostProcess(UINT8 u1DecId, UINT16 u2BypassMode)
{
	_AUD_DspBypassPostProcess(u1DecId,  u2BypassMode);
}

//-----------------------------------------------------------------------------
/** AUD_DspSoundEffectFlag
 *  Enable all audio sound effect. (EQ, Bass/Treble, Reverb, Surround, AVC)
 *
 *  @param  u1DecId  0: first decoder 1: seconder decoder.
 *  @param  fgFlag     0: disable, 1: enable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSoundEffectFlag(UINT8 u1DecId, BOOL fgFlag)
{
	_AUD_DspSoundEffectFlag( u1DecId,  fgFlag);
}

//-----------------------------------------------------------------------------
/** AUD_DspIECConfig
 *  Routine handling IEC Configuration.
 *
 *  @param  eIecCfg  0: PCM; 1:RAW.
 *  @param  fgEnable  on/off.
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
BOOL AUD_DspIECConfig(AUD_IEC_T eIecCfg, BOOL fgEnable)
{
	return _AUD_DspIECConfig(eIecCfg,  fgEnable);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetIecRawFlag
 *  Enable/disable sync IEC RAW delay to channel delay automatically.
 *
 *  @param  u1Flag
 *  @retval  N/A
 */
//-----------------------------------------------------------------------------
BOOL AUD_DspSetIecRawFlag(UINT8 u1Flag)
{
	_AUD_DspSetIecRawFlag(u1Flag);

	return TRUE;
}

//-----------------------------------------------------------------------------
/** AUD_DspSetIecRawDelay
 *  Set IEC RAW delay.
 *
 *  @param  i2Delay (ms)
 *  @retval  N/A
 */
//-----------------------------------------------------------------------------
BOOL AUD_DspSetIecRawDelay(INT16 i2Delay)
{
	_AUD_DspSetIecRawDelay(i2Delay);

	return TRUE;
}

//-----------------------------------------------------------------------------
/** AUD_DspIecChannel
 *  Routine handling IEC Configuration.
 *
 *  @param  eIecChannel  select audio channel,ex L/R.
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
BOOL AUD_DspIecChannel(AUD_IEC_CH_T eIecChannel)
{
	return _AUD_DspIecChannel(eIecChannel);
}

//-----------------------------------------------------------------------------
/** AUD_DspIecCopyright
 *  Routine handling IEC Configuration.
 *
 *  @param  u1Enable 0: disable 1: enable
 *  @param  u1CategoryCode
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
BOOL AUD_DspIecCopyright(UINT8 u1Enable, UINT8 u1CategoryCode)
{
   _AUD_DspSpdifSetInfo(SPDIF_REG_TYPE_DEFAULT, AUD_STREAM_FROM_DIGITAL_TUNER, u1Enable, u1CategoryCode);

   return TRUE;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetIecCopyright
 *  Routine handling IEC Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  copyright value
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetIecCopyright(AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    return _AUD_DspGetSpdifCopyright(SPDIF_REG_TYPE_DEFAULT, eStreamFrom);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetIecCategoryCode
 *  Routine handling IEC Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  category code value
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetIecCategoryCode(AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    return _AUD_DspGetSpdifCategoryCode(SPDIF_REG_TYPE_DEFAULT, eStreamFrom);
}

//-----------------------------------------------------------------------------
/** AUD_DspResetSpdifReg
 *   reset SPDIF register.
 *
 *  @param  void
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspResetSpdifReg (AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    _AUD_DspResetSpdifReg(SPDIF_REG_TYPE_USER, eStreamFrom);
}

//-----------------------------------------------------------------------------
/** AUD_SpdifInBypass
 *   SPDIF In Pin Select
 *
 *  @param  Pin number by IC.
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_SpdifInBypass(SPDIF_IN_PIN_NUM u1PIN)
{
    _AUD_SpdifInBypass(u1PIN);
}

#ifndef CC_AUD_SUPPORT_SPDIF_V20
//-----------------------------------------------------------------------------
/** AUD_DspSetSpdifRegType
 *   get SPDIF register type.
 *
 *  @param  SPIDF_REG_TYPE_T
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetSpdifRegType (SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    _AUD_DspSetSpdifRegType(type, eStreamFrom);
}
#endif

//-----------------------------------------------------------------------------
/** AUD_DspSetSpdifCopyright
 *  Routine handling SPDIF Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  copyright value
 */
//-----------------------------------------------------------------------------
#ifdef CC_AUD_SUPPORT_SPDIF_V20
void AUD_DspSetSpdifCopyright(AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 copyright)
{
    _AUD_DspSetSpdifCopyright(SPDIF_REG_TYPE_USER, eStreamFrom, copyright);
}
#else
void AUD_DspSetSpdifCopyright(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 copyright)
{
    UNUSED(type); // for backward compatible

    _AUD_DspSetSpdifCopyright(SPDIF_REG_TYPE_USER, eStreamFrom, copyright);
}
#endif


//-----------------------------------------------------------------------------
/** AUD_DspSetSpdifCategoryCode
 *  Routine handling SPDIF Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  copyright value
 */
//-----------------------------------------------------------------------------
void AUD_DspSetSpdifCategoryCode(AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 categoryCode)
{
    _AUD_DspSetSpdifCategoryCode(SPDIF_REG_TYPE_USER, eStreamFrom, categoryCode);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetSpdifWordLength
 *  Routine handling SPDIF Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  copyright value
 */
//-----------------------------------------------------------------------------
void AUD_DspSetSpdifWordLength(AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 wordLength)
{
    _AUD_DspSetSpdifWordLength(SPDIF_REG_TYPE_USER, eStreamFrom, wordLength);
}

//-----------------------------------------------------------------------------
/** AUD_DspSpdifSetInfo
 *  Routine handling IEC Configuration.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Enable 0: disable 1: enable
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
BOOL AUD_DspSpdifSetInfo(AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 u1CopyRight, UINT8 u1CategoryCode)
{
    _AUD_DspSpdifSetInfo(SPDIF_REG_TYPE_USER, eStreamFrom, u1CopyRight, u1CategoryCode);

    return TRUE;
}

//-----------------------------------------------------------------------------
/** AUD_DspSpdifSetCopyProtect
 *  Routine handling IEC Configuration.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Enable 0: disable 1: enable
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
BOOL AUD_DspSpdifSetCopyProtect(AUD_DEC_STREAM_FROM_T eStreamFrom, BOOL bCpBit, BOOL bLBit)
{
    _AUD_DspSpdifSetCopyProtect(SPDIF_REG_TYPE_USER, eStreamFrom, bCpBit, bLBit);

    return TRUE;
}

//-----------------------------------------------------------------------------
/** AUD_DspSetSpdifUpdateMode
 *  Routine handling SPDIF Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  copyright value
 */
//-----------------------------------------------------------------------------
void AUD_DspSetSpdifUpdateMode(AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 eUpdate)
{
    _AUD_DspSetSpdifUpdateMode(SPDIF_REG_TYPE_USER, eStreamFrom, eUpdate);
}


//-----------------------------------------------------------------------------
/** AUD_DspGetSpdifCopyright
 *  Routine handling IEC Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  copyright value
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetSpdifCopyright(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    return _AUD_DspGetSpdifCopyright(type, eStreamFrom);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetSpdifCategoryCode
 *  Routine handling IEC Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  category code value
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetSpdifCategoryCode(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    return _AUD_DspGetSpdifCategoryCode(type, eStreamFrom);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetSpdifWordLength
 *  Routine handling IEC Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  word length value
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetSpdifWordLength(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    return _AUD_DspGetSpdifWordLength(type, eStreamFrom);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetSpdifUpdateMode
 *  Routine handling IEC Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  by source? value
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetSpdifUpdateMode(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    return _AUD_DspGetSpdifupdateMode(type, eStreamFrom);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetSpdifOutputMode
 *  Routine handling IEC Configuration.
 *
 *  @param  u1DecId     decoder ID
 *
 *  @retval  value
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetSpdifOutputMode(UINT8 u1DecId)
{
    return _AUD_DspGetSpdifOutputMode(u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetSpdifUpdateMode
 *  Routine handling IEC Configuration.
 *
 *  @param
 *
 *  @retval
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetSpdifChannelStatus(SPDIF_CHANNEL_STATUS_T *rInfo)
{
    return _AUD_DspGetSpdifChannelStatus(rInfo);
}

#ifdef CC_AUD_MIXSOUND_SUPPORT
//-----------------------------------------------------------------------------
/** AUD_DspMixsoundEnable
 *  Mix sound enable.
 *
 *  @param  fgEnable TRUE(enable) FALSE(disable).
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspMixSndEnable(BOOL fgEnable)
{
	_AUD_DspMixSndEnable(fgEnable);
}

//-----------------------------------------------------------------------------
/** AUD_DspMixSndMute
 *  Mix sound mute.
 *
 *  @param  fgMutre TRUE(mute) FALSE(un-mute).
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspMixSndMute(BOOL fgMute)
{
	_AUD_DspMixSndMute(fgMute);
}

//-----------------------------------------------------------------------------
/** AUD_DspMixsoundControl
 *  Mix sound control.
 *
 *  @param  u2Mode play/stop.
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspMixSndControl(UINT16 u2Mode)
{
	_AUD_DspMixSndControl(u2Mode);
}

#ifdef NEW_MIXSOUND
void AUD_DspMixSndControl2(UINT16 u2Mode)
{
	_AUD_DspMixSndControl2(u2Mode);
}
#endif

//-----------------------------------------------------------------------------
/** AUD_DspMixSndClip
 *  Select mix sound data source.
 *
 *  @param  u1ClipIdx 0~3
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspMixSndClip(UINT8 u1ClipIdx)
{
	_AUD_DspMixSndClip(u1ClipIdx);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetMixSndEnable
 *  Get mix sound enable.
 *
 *  @param  void.
 *  @retval  TRUE(enable) FALSE(disable).
 */
//-----------------------------------------------------------------------------
BOOL AUD_DspGetMixSndEnable(void)
{
	return _AUD_DspGetMixSndEnable();
}

//-----------------------------------------------------------------------------
/** AUD_DspGetMixSndClipIdx
 *  Get mix sound clip index.
 *
 *  @param   void.
 *  @retval  clip index.
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetMixSndClipIdx(void)
{
	return _AUD_DspGetMixSndClipIdx();
}
#endif


//-----------------------------------------------------------------------------
/** AUD_GetVideoScrambleStatus
 *  Get VideoScrambleStatus
 *
 *  @param  void.
 *  @retval  TRUE(enable) FALSE(disable).
 */
//-----------------------------------------------------------------------------
BOOL AUD_GetVideoScrambleStatus(void)
{
	return _AUD_GetVideoScrambleStatus();
}

//-----------------------------------------------------------------------------
/** AUD_DspSetDemodOutputMode
 *  Set MTS output mode.
 *
 *  @param  u1Output    0: Output (Mono,Mono)
 *                               1: If stereo exists, Output (L,R) otherwise (Mono,Mono)
 *                               2: If SAP exists, Output (S,S) otherwise (L,R) otherwise (Mono,Mono)
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetDemodOutputMode(UINT8 u1Output)
{
	_AUD_DspSetDemodOutputMode( u1Output);
}

#ifdef DSP_SUPPORT_SRSTSXT //gmh add 20101028
void AUD_DspSetSRSTSXTSpkSz(UINT8 u1Mode)
{
	_AUD_DspSetSRSTSXTSpkSz(u1Mode);
}

void AUD_DspSetSRSTSXTSetMode(UINT8 u1Mode)
{
    _AUD_DspSetSRSTSXTSetMode(u1Mode);
}

void AUD_DspSetSRSTSXTElev(UINT8 u1Mode)
{
	_AUD_DspSetSRSTSXTElev(u1Mode);
}
void AUD_DspSetSRSTSXTinputgain(UINT32 u4Mode)
{
    _AUD_DspSetSRSTSXTinputgain(u4Mode);
}
void AUD_DspSetSRSTSXTTBLvl(UINT8 u1Mode)
{
	_AUD_DspSetSRSTSXTTBLvl(u1Mode);
}
#endif

//-----------------------------------------------------------------------------
/** AUD_DspSetDetectDefaultMode
 *   call this function before issue detection command.
 *
 *  @param  u1Mode 0: PAL_DK, 4: SECAM-L.
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetDetectDefaultMode(UINT8 u1Mode)
{
	_AUD_DspSetDetectDefaultMode( u1Mode);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetUserDefinedEqCfg
 *   setup user difined EQ config.
 *
 *  @param  u1DecId  0:first decoder 1: seconder decoder.
 *  @param  eEqType  0: off   others: 1~12.
 *  @param  u1BandIdx  0~4.
 *  @param  i1Value -100 ~ +100
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetUserDefinedEqCfg(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, UINT8 u1BandIdx, INT8 i1Value)
{
	_AUD_DspSetUserDefinedEqCfg( u1DecId,  eEqType,  u1BandIdx,  i1Value);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetUserDefinedEqCfg
 *   Get user difined EQ config.
 *
 *  @param  eEqType  0: off   others: 1~12.
 *  @param  u1BandIdx  0~4.
 *  @param  i1Value (-20 ~ +20)
 *
 *  @retval  TRUE: success
 *              FALSE: failed
 */
//-----------------------------------------------------------------------------
BOOL AUD_DspGetUserDefineEqCfg(AUD_EQ_TYPE_T eEqType, UINT8 u1BandIdx, INT8* pi1Value)
{
	return _AUD_DspGetUserDefineEqCfg( eEqType,  u1BandIdx, pi1Value);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetUserEqBandNum
 *   Get user defined band number.
 *
 *  @retval  CH_EQ_BAND_NO = 5.
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetUserEqBandNum(void)
{
	return _AUD_DspGetUserEqBandNum();
}

//-----------------------------------------------------------------------------
/** AUD_DspGetEqBandInfo
 *   Get user defined band info.
 *
 *  @param  prEqBandInfo  structure of eq band info (AUD_EQ_BAND_INFO_T).
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspGetEqBandInfo(AUD_EQ_BAND_INFO_T* prEqBandInfo)
{
	_AUD_DspGetEqBandInfo( prEqBandInfo);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetSrcVol
 *   get source volume.
 *
 *  @param  u1DecId  0:first decoder 1: seconder decoder.
 *  @param  eStrmSrc      AUD_STREAM_FROM_OTHERS = 0,         // ex. Pink Noise Generator.
                                   AUD_STREAM_FROM_DIGITAL_TUNER,      // Digital tuner.
                                   AUD_STREAM_FROM_ANALOG_TUNER,       // ex. Analog tuner.
                                   AUD_STREAM_FROM_SPDIF,              // SPDIF-in.
                                   AUD_STREAM_FROM_LINE_IN,            // Line in.
                                   AUD_STREAM_FROM_HDMI,               // HDMI in.
                                   AUD_STREAM_FROM_MEMORY,             // Memory.
                                   AUD_STREAM_FROM_MULTI_MEDIA,             // Multi-media

 *  @retval  input source volume
 */
//-----------------------------------------------------------------------------
INT16 AUD_DspGetSrcVol(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc)
{
	return _AUD_DspGetSrcVol( u1DecId,  eStrmSrc);
}

#ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
void AUD_DspSrcLinoutVol(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc, INT16 i2Vol)
{
	_AUD_DspSrcLinoutVol( u1DecId, eStrmSrc, i2Vol );
}
#endif

//-----------------------------------------------------------------------------
/** AUD_DspSetSrcVol
 *   set source volume.
 *
 *  @param  u1DecId  0:first decoder 1: seconder decoder.
 *  @param  eStrmSrc      AUD_STREAM_FROM_OTHERS = 0,         // ex. Pink Noise Generator.
                                   AUD_STREAM_FROM_DIGITAL_TUNER,      // Digital tuner.
                                   AUD_STREAM_FROM_ANALOG_TUNER,       // ex. Analog tuner.
                                   AUD_STREAM_FROM_SPDIF,              // SPDIF-in.
                                   AUD_STREAM_FROM_LINE_IN,            // Line in.
                                   AUD_STREAM_FROM_HDMI,               // HDMI in.
                                   AUD_STREAM_FROM_MEMORY,             // Memory.
                                   AUD_STREAM_FROM_MULTI_MEDIA,             // Multi-media

 *  @param  i2Vol  -128 ~ 96
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetSrcVol(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc, INT16 i2Vol)
{
	_AUD_DspSetSrcVol( u1DecId,  eStrmSrc,  i2Vol);
}

//-----------------------------------------------------------------------------
/** AUD_DspVolumeChange
 *   Setup master volume, or input source gain, or channel trim to DSP.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  eAudChl  : 0: front left channel.
 *                              1: front right channel.
 *                              2: rear left channel.
 *                              3: rear right channel.
 *                              4: center channel.
 *                              5: sub woofer channel.
 *                              6: bypass left channel.
 *                              7: bypass right channel.
 *                              8: downmix left channel.
 *                              9: downmix right channel.
 *                            10: aux front left channel.
 *                            11: aux front right channel.
 *                            12: input source gain.
 *                            13: all channels    ( For master volume control ).
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspVolumeChange(UINT8 u1DecId, AUD_CH_T eAudChl)
{
	_AUD_DspVolumeChange( u1DecId,  eAudChl);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetAvSrcVol
 *   set AV source volume.
 *
 *  @param  eInputSrcId         AUD_INPUT_ID_COMP_VID_0 = 0,
                                          AUD_INPUT_ID_COMP_VID_1,
                                          AUD_INPUT_ID_COMP_VID_2,
                                          AUD_INPUT_ID_COMP_VID_3,
                                          AUD_INPUT_ID_S_VID_0,
                                          AUD_INPUT_ID_S_VID_1,
                                          AUD_INPUT_ID_S_VID_2,
                                          AUD_INPUT_ID_YPBPR_0,
                                          AUD_INPUT_ID_YPBPR_1,
                                          AUD_INPUT_ID_YPBPR_2,
                                          AUD_INPUT_ID_VGA_0,
                                          AUD_INPUT_ID_VGA_1,
                                          AUD_INPUT_ID_HDMI_0,
                                          AUD_INPUT_ID_HDMI_1,
                                          AUD_INPUT_ID_HDMI_2,
                                          AUD_INPUT_ID_HDMI_3,
                                          AUD_INPUT_ID_DVI_0,
                                          AUD_INPUT_ID_DVI_1,
                                          AUD_INPUT_ID_DVI_2,
                                          AUD_INPUT_ID_DVI_3,
                                          AUD_INPUT_ID_SCART_0,
                                          AUD_INPUT_ID_SCART_1,
                                          AUD_INPUT_ID_SCART_2,
                                          AUD_INPUT_ID_SCART_3,
                                          AUD_INPUT_ID_AUXIN_0,
                                          AUD_INPUT_ID_AUXIN_1,
 *  @param  i2Vol  -256 ~ 256.
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetAvSrcVol(AUD_INPUT_ID_T eInputSrcId , INT16 i2Vol)
{
	_AUD_DspSetAvSrcVol( eInputSrcId ,  i2Vol);
}

AUD_INPUT_ID_T AUD_DspGetAvInputSrcId(void)
{
	return _AUD_DspGetAvInputSrcId();
}

//-----------------------------------------------------------------------------
/** AUD_DspGetAvInputSrcDvdId
 *
 *
 *  @retval
 */
//-----------------------------------------------------------------------------
AUD_INPUT_ID_T AUD_DspGetAvInputSrcDvdId(void)
{
	return _AUD_DspGetAvInputSrcDvdId();
}

//-----------------------------------------------------------------------------
/** AUD_DspGetAtvTvSys
 *
 *  @param  prTvAudSys  tv audio system structure.
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspGetAtvTvSys(MW_TV_AUD_SYS_T * prTvAudSys)
{
	_AUD_DspGetAtvTvSys(  prTvAudSys);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetAtvTvSystem
 *
 *  @param  void
 *  @retval  TV_AUD_SYS_T
 */
//-----------------------------------------------------------------------------
TV_AUD_SYS_T AUD_DspGetAtvTvSystem(void)
{
    return _AUD_DspGetAtvTvSystem();
}

/** AUD_DspGetAtvTvSystemStrength
 *
 *  @param  TV_AUD_SYS_T
 *  @retval  UINT32
 */
//-----------------------------------------------------------------------------
UINT32 AUD_DspGetAtvTvSystemStrength(TV_AUD_SYS_T eAudSys)
{
    return _AUD_DspGetAtvTvSystemStrength(eAudSys);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetTvSrcMode
 *   set tv source mode.
 *
 *  @param  eAudSys      SV_NONE_DETECTED = 0x0,
                                   SV_MTS           = 0x1,
                                   SV_FM_FM         = 0x2,
                                   SV_NTSC_M        = 0x3,
                                   SV_A2_BG         = 0x4,
                                   SV_A2_DK1        = 0x5,
                                   SV_A2_DK2        = 0x6,
                                   SV_A2_DK3        = 0x7,
                                   SV_PAL_I         = 0x8,
                                   SV_PAL_BG        = 0x9,
                                   SV_PAL_DK        = 0xa,
                                   SV_SECAM_L       = 0xb,
                                   SV_SECAM_L_PLUM  = 0xc,
                                   SV_SECAM_BG      = 0xd,
                                   SV_SECAM_DK      = 0xe
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetTvSrcMode(TV_AUD_SYS_T eAudSys)
{
	_AUD_DspSetTvSrcMode( eAudSys);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetTvSrcMode
 *   get tv source mode.
 *
 *  @param  void
 *  @retval  eAudSys
 */
//-----------------------------------------------------------------------------
TV_AUD_SYS_T AUD_DspGetTvSrcMode(void)
{
    return _AUD_DspGetTvSrcMode();
}

#ifdef CC_AUD_TV_SYS_FINE_TUNE_VOL
void AUD_SetTVSysFineTuneVol(TV_AUD_SYS_T eAudSys)
{
    _AUD_SetTVSysFineTuneVol(eAudSys);
}
#endif
//-----------------------------------------------------------------------------
/** AUD_DspSetAtvOutputMode
 *   set tv source mode.
 *
 *  @param  u1DecId 0:first decoder 1: seconder decoder.
 *  @param  eSoundMode         UNKNOWN = 0,
                                           MONO,
                                           STEREO,
                                           SUB_LANG,
                                           DUAL1,
                                           DUAL2,
                                           NICAM_MONO,
                                           NICAM_STEREO,
                                           NICAM_DUAL1,
                                           NICAM_DUAL2,
                                           A2_DUAL1,
                                           A2_DUAL2,
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetAtvOutputMode(UINT8 u1DecId, AUD_SOUND_MODE_T eSoundMode)
{
	_AUD_DspSetAtvOutputMode( u1DecId,  eSoundMode);
}

//-----------------------------------------------------------------------------
/** AUD_DspHdmiSpdifOutEnable
 *   SPDIF output control in HDMI mode.
 *
 *  @param  fgEnable    0: disable SPDIF output, 1: enable SPDIF output
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspHdmiSpdifOutEnable(BOOL fgEnable)
{
	_AUD_DspHdmiSpdifOutEnable( fgEnable);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetSpectrumInfo
 *   Get audio spectrum information.
 *
 *  @param  u1DecId 0:first decoder 1: seconder decoder.
 *  @param  u1BandNum      1~15 .
 *  @retval  u4Spectrum
 */
//-----------------------------------------------------------------------------
UINT32 AUD_DspGetSpectrumInfo(UINT8 u1DecId, UINT8 u1BandNum)
{
	return _AUD_DspGetSpectrumInfo( u1DecId,  u1BandNum);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetSoundMode
 *   Get ATV sound mode
 *
 *  @param  u1DecId 0:first decoder 1: seconder decoder.
 *
 *  @retval AUD_SOUND_MODE_T
 */
//-----------------------------------------------------------------------------
AUD_SOUND_MODE_T AUD_DspGetSoundMode(UINT8 u1DecId)
{
    return _AUD_DspGetSoundMode(u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspSpectrumEnable
 *   Enable audio spectrum calculation.
 *
 *  @param  u1DecId    0:first decoder 1: seconder decoder.
 *  @param  fgEnable    0: disable calculation, 1: enable calculation.
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSpectrumEnable(UINT8 u1DecId, BOOL fgEnable)
{
	_AUD_DspSpectrumEnable( u1DecId,  fgEnable);
}


//-----------------------------------------------------------------------------
/** AUD_AudioSetStcDiffBound
 *   Set STC PTS difference bound. Bound would be (x MS * (1+uStcDiff)).
 *
 *  @param  u1DecId      0:first decoder 1: seconder decoder.
 *  @param  uStcDiffLo    for low boundary of stc diff (0~255) .
 *  @param  uStcDiffHi    for high boundary of stc diff (0~255) .
 *  @param  uStcDiffWs   for worst boundary of stc diff (0~255) .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_AudioSetStcDiffBound(UINT8 u1DecId, UINT8 uStcDiffLo, UINT8 uStcDiffHi, UINT8 uStcDiffWs)
{
	_AUD_AudioSetStcDiffBound( u1DecId,  uStcDiffLo,  uStcDiffHi,  uStcDiffWs);
}

//-----------------------------------------------------------------------------
/** AUD_DDBannerEnable
 *   Turn on this funtion to close some effect and post processing,  for DD test only.
 *
 *  @param  u1DecId      0:first decoder 1: seconder decoder.
 *  @param  fgEnable     0: disable, 1: enable .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DDBannerEnable(UINT8 u1DecId, BOOL fgEnable)
{
	_AUD_DDBannerEnable( u1DecId,  fgEnable);
}

//-----------------------------------------------------------------------------
/** AUD_GetDDBannerEnable
 *   Get Dolby banner information.
 *
 *  @param  u1DecId      0:first decoder 1: seconder decoder.
 *  @param  fgEnable     0: disable, 1: enable .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
BOOL AUD_GetDDBannerEnable (void)
{
	return (_AUD_GetDDBannerEnable ());
}

//-----------------------------------------------------------------------------
/** AUD_VideoScrambled
 *   Set Video Scramble Status
 *
 *  @param  fgEnable     0: Normal Video, 1: Video scrambled .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_VideoScrambled(BOOL fgEnable)
{
    _AUD_VideoScrambled(fgEnable);

}
// *********************************************************************
// Function : AUD_DspSetPalDetection
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
void AUD_DspSetPalDetection ( AUD_FACTORY_PAL_DETECTION_ITEM_T eChangedItem,
                                                                                        UINT16 u2Value)
{
	_AUD_DspSetPalDetection (  eChangedItem,  u2Value);
}

// *********************************************************************
// Function : AUD_DspGetPalDetection
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
UINT16 AUD_DspGetPalDetection ( AUD_FACTORY_PAL_DETECTION_ITEM_T eItem)
{
	return _AUD_DspGetPalDetection (  eItem);
}

// *********************************************************************
// Function : AUD_DspSetA2Detection
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
void AUD_DspSetA2Detection ( AUD_FACTORY_A2_DETECTION_ITEM_T eChangedItem,
                                                                                        UINT16 u2Value)
{
	_AUD_DspSetA2Detection (  eChangedItem, u2Value);
}

// *********************************************************************
// Function : AUD_DspGetA2Detection
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
UINT16 AUD_DspGetA2Detection ( AUD_FACTORY_A2_DETECTION_ITEM_T eItem)
{
	return _AUD_DspGetA2Detection (  eItem);
}

// *********************************************************************
// Function : AUD_DspSetA2Threshold
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
void AUD_DspSetA2Threshold ( AUD_FACTORY_A2_THRESHOLD_ITEM_T eChangedItem,
                                                                                        UINT16 u2Value)
{
	_AUD_DspSetA2Threshold (  eChangedItem, u2Value);
}

// *********************************************************************
// Function : AUD_DspGetA2Threshold
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
UINT16 AUD_DspGetA2Threshold ( AUD_FACTORY_A2_THRESHOLD_ITEM_T eItem)
{
	return _AUD_DspGetA2Threshold (  eItem);
}

/******************************************************************************
* Function      : AUD_DspSetMtsDetection
* Description   : Set NPTV MTS Detection Configurations
* Parameter     : u2NumCheck, u2StereoMid, u2StereoConMid, u2Pilot3Mid,
*                 u2Pilot3ConMid, u2SapMid, u2SapConMid
* Return        : None
******************************************************************************/
void AUD_DspSetMtsDetection ( AUD_FACTORY_MTS_DETECTION_ITEM_T eChangedItem,
                                                                                        UINT16 u2Value)
{
	_AUD_DspSetMtsDetection (  eChangedItem,  u2Value) ;
}

/******************************************************************************
* Function      : AUD_DspGetMtsDetection
* Description   : Set NPTV MTS Detection Configurations
* Parameter     : u2NumCheck, u2StereoMid, u2StereoConMid, u2Pilot3Mid,
*                 u2Pilot3ConMid, u2SapMid, u2SapConMid
* Return        : None
******************************************************************************/
UINT16 AUD_DspGetMtsDetection ( AUD_FACTORY_MTS_DETECTION_ITEM_T eItem)
{
	return _AUD_DspGetMtsDetection (  eItem);
}

// *********************************************************************
// Function : AUD_DspSetFmfmDetection
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
void AUD_DspSetFmfmDetection ( AUD_FACTORY_FMFM_DETECTION_ITEM_T eChangedItem,
                                                                                        UINT16 u2Value)
{
	_AUD_DspSetFmfmDetection (  eChangedItem, u2Value) ;
}

// *********************************************************************
// Function : AUD_DspGetFmfmDetection
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
UINT16 AUD_DspGetFmfmDetection ( AUD_FACTORY_FMFM_DETECTION_ITEM_T eItem)
{
	return _AUD_DspGetFmfmDetection (  eItem);
}

// *********************************************************************
// Function : AUD_DspSetHdevMode
// Description : set high deveation mode on/off
// Parameter : bit0 : PAL
//                  bit1 : A2
//                  bit2 : MTS
//                  bit3 : FMFM
// Return    :
// *********************************************************************
void AUD_DspSetHdevMode ( AUD_ATV_DECODER_TYPE_T eDecType, BOOL fgEnable)
{
	_AUD_DspSetHdevMode (  eDecType,  fgEnable);
}

// *********************************************************************
// Function : AUD_DspGetHdevMode
// Description : set high deveation mode on/off
// Parameter : bit0 : PAL
//                  bit1 : A2
//                  bit2 : MTS
//                  bit3 : FMFM
// Return    :
// *********************************************************************
BOOL AUD_DspGetHdevMode (AUD_ATV_DECODER_TYPE_T eDecType)
{
	return _AUD_DspGetHdevMode ( eDecType);
}

// *********************************************************************
// Function : AUD_DspSetAmMute
// Description : set AM mute mode on/off and thresholds
// Parameter :
// Return    :
// *********************************************************************
void AUD_DspSetAmMute( AUD_FACTORY_PAL_AM_MUTE_ITEM_T eChangedItem,
                                                                                        UINT8 u1Value)
{
	_AUD_DspSetAmMute(  eChangedItem, u1Value);
}

// *********************************************************************
// Function : AUD_DspGetAmMute
// Description : set AM mute mode on/off and thresholds
// Parameter :
// Return    :
// *********************************************************************
UINT8 AUD_DspGetAmMute( AUD_FACTORY_PAL_AM_MUTE_ITEM_T eItem)
{
	return _AUD_DspGetAmMute(  eItem);
}

// *********************************************************************
// Function : Aud_DspSetFmMute
// Description : set FM mute mode on/off and thresholds
// Parameter : bit0 : PAL
//                  bit1 : A2
//                  bit2 : MTS
//                  bit3 : FMFM
// Return    :
// *********************************************************************
void AUD_DspSetFmMute ( AUD_ATV_DECODER_TYPE_T eDecType,
                                                            AUD_FACTORY_FM_MUTE_ITEM_T eChangedItem,
                                                                  UINT8 u1Value)
{
	_AUD_DspSetFmMute (  eDecType,  eChangedItem, u1Value);
}

// *********************************************************************
// Function : Aud_DspGetFmMute
// Description : set FM mute mode on/off and thresholds
// Parameter : bit0 : PAL
//                  bit1 : A2
//                  bit2 : MTS
//                  bit3 : FMFM
// Return    :
// *********************************************************************
UINT8 AUD_DspGetFmMute ( AUD_ATV_DECODER_TYPE_T eDecType,AUD_FACTORY_FM_MUTE_ITEM_T eItem)
{
	return _AUD_DspGetFmMute (  eDecType, eItem);
}

// *********************************************************************
// Function : AUD_DspSetCarrierShiftMode
// Description :set carrier shift mode on/off
// Parameter : bit0 : PAL
//                  bit1 : A2
//                  bit2 : MTS
//                  bit3 : FMFM
// Return    :
// *********************************************************************
void AUD_DspSetCarrierShiftMode ( AUD_ATV_DECODER_TYPE_T eDecType, BOOL fgEnable)
{
	_AUD_DspSetCarrierShiftMode (  eDecType,  fgEnable);
}

// *********************************************************************
// Function : AUD_DspGetCarrierShiftMode
// Description : set high deveation mode on/off
// Parameter : bit0 : PAL
//                  bit1 : A2
//                  bit2 : MTS
//                  bit3 : FMFM
// Return    :
// *********************************************************************
BOOL AUD_DspGetCarrierShiftMode (AUD_ATV_DECODER_TYPE_T eDecType)
{
	return _AUD_DspGetCarrierShiftMode ( eDecType);
}

// *********************************************************************
// Function : AUD_DspSetNonEUMode
// Description : set Non-EU mode on/off
// Parameter :
// Return    :
// *********************************************************************
void AUD_DspSetNonEUMode (BOOL fgEnable)
{
	_AUD_DspSetNonEUMode ( fgEnable);
}

// *********************************************************************
// Function : AUD_DspGetNonEUMode
// Description : set Non-EU mode on/off
// Parameter :
// Return    :
// *********************************************************************
BOOL AUD_DspGetNonEUMode (void)
{
	return _AUD_DspGetNonEUMode ();
}

// *********************************************************************
// Function : void AUD_DspSetFMSatuMuteMode() large
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
void AUD_DspSetFMSatuMuteMode ( AUD_ATV_DECODER_TYPE_T eDecType, BOOL fgEnable)
{
	_AUD_DspSetFMSatuMuteMode (eDecType, fgEnable);
}

// *********************************************************************
// Function : void AUD_DspGetFMSatuMuteMode() large
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
BOOL AUD_DspGetFMSatuMuteMode (AUD_ATV_DECODER_TYPE_T eDecType)
{
	return _AUD_DspGetFMSatuMuteMode (eDecType);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetPALFineVolume
 *   Set fine tune volume for PAL.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetPALFineVolume (UINT8 u1Index)
{
	_AUD_DspSetPALFineVolume ( u1Index);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetPALFineVolume
 *   Get fine tune volume for PAL.
 *
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetPALFineVolume (void)
{
	return _AUD_DspGetPALFineVolume ();
}

//-----------------------------------------------------------------------------
/** AUD_DspSetNicamFineVolume
 *   Set fine tune volume for NICAM.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetNicamFineVolume (UINT8 u1Index)
{
	_AUD_DspSetNicamFineVolume ( u1Index);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetNicamFineVolume
 *   Get fine tune volume for NICAM.
 *
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetNicamFineVolume (void)
{
	return _AUD_DspGetNicamFineVolume ();
}

//-----------------------------------------------------------------------------
/** AUD_DspSetAMFineVolume
 *   Set fine tune volume for AM.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetAMFineVolume (UINT8 u1Index)
{
	_AUD_DspSetAMFineVolume ( u1Index);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetAMFineVolume
 *   Get fine tune volume for AM.
 *
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetAMFineVolume (void)
{
	return _AUD_DspGetAMFineVolume ();
}

//-----------------------------------------------------------------------------
/** AUD_DspSetA2FineVolume
 *   Set fine tune volume for A2.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetA2FineVolume (UINT8 u1Index)
{
	_AUD_DspSetA2FineVolume ( u1Index);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetA2FineVolume
 *   Get fine tune volume for A2.
 *
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetA2FineVolume (void)
{
	return _AUD_DspGetA2FineVolume ();
}

//-----------------------------------------------------------------------------
/** AUD_DspSetMtsMonoFineVolume
 *   Set fine tune volume for MTS mono.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetMtsMonoFineVolume (UINT8 u1Index)
{
	_AUD_DspSetMtsMonoFineVolume ( u1Index);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetMtsMonoFineVolume
 *   Get fine tune volume for MTS mono.
 *
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetMtsMonoFineVolume (void)
{
	return _AUD_DspGetMtsMonoFineVolume ();
}

//-----------------------------------------------------------------------------
/** AUD_DspSetSAPFineVolume
 *   Set fine tune volume for SAP.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetSAPFineVolume(UINT8 u1Index)
{
	_AUD_DspSetSAPFineVolume( u1Index);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetSapFineVolume
 *   Get fine tune volume for SAP.
 *
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetSapFineVolume (void)
{
	return _AUD_DspGetSapFineVolume () ;
}

//-----------------------------------------------------------------------------
/** AUD_DspSetFmRadioFineVolume
 *   Set fine tune volume for FM radio.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetFmRadioFineVolume (UINT8 u1Index)
{
	_AUD_DspSetFmRadioFineVolume ( u1Index);
}

#ifdef CC_SET_VOLUME
void AUD_DspSetVolTable(UINT8 u1Idx, UINT32  u4Volume)
{
	 _AUD_DspSetVolTable(u1Idx, u4Volume) ;
}
#endif

#ifdef CC_AUD_NEW_CV_TABLE
void AUD_DspSetCVTable(UINT8 u1Idx, UINT32  u4Volume)
{
	 _AUD_DspSetCVTable(u1Idx, u4Volume) ;
}
UINT32 AUD_DspGetCVTable(UINT8 u1VolTabIdx)
{
    return _AUD_DspGetCVTable(u1VolTabIdx);
}

#endif

//-----------------------------------------------------------------------------
/** AUD_DspSetFmfmMonoFineVolume
 *   Set fine tune volume for FMFM mono.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetFmfmMonoFineVolume (UINT8 u1Index)
{
	_AUD_DspSetFmfmMonoFineVolume ( u1Index);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetFmfmMonoFineVolume
 *   Get fine tune volume for FMFM mono.
 *
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetFmfmMonoFineVolume (void)
{
	return _AUD_DspGetFmfmMonoFineVolume ();
}

//-----------------------------------------------------------------------------
/** AUD_DspSetFmfmDualFineVolume
 *   Set fine tune volume for FMFM dual.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetFmfmDualFineVolume(UINT8 u1Index)
{
	_AUD_DspSetFmfmDualFineVolume( u1Index);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetFmfmDualFineVolume
 *   Get fine tune volume for FMFM dual.
 *
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetFmfmDualFineVolume (void)
{
	return _AUD_DspGetFmfmDualFineVolume ();
}

//-----------------------------------------------------------------------------
/** AUD_DspSetMtsPilotDetection
 *   set MTS pilot offset detection on/off.
 *  @param  fgEnable	TRUE/FALSE
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetMtsPilotDetection (BOOL fgEnable)
{
	_AUD_DspSetMtsPilotDetection ( fgEnable);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetMtsPilotDetection
 *   get MTS pilot offset detection on/off.
 *  @param  void
 *  @retval  BOOL
 */
//-----------------------------------------------------------------------------
BOOL AUD_DspGetMtsPilotDetection (void)
{
	return _AUD_DspGetMtsPilotDetection ();
}

//-----------------------------------------------------------------------------
/** AUD_DspSetSapMute
 *   set SAP noise mute thresholds
 *  @param  eChangedItem	lower threshold or higher threshold
 *  @param  u1Value
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetSapMute( AUD_FACTORY_SAP_MUTE_ITEM_T eChangedItem,
                                                                                        UINT8 u1Value)
{
    _AUD_DspSetSapMute(eChangedItem, u1Value);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetSapMute
 *   get SAP noise mute thresholds
 *  @param  eItem lower threshold or higher threshold
 *  @retval  UINT8
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetSapMute( AUD_FACTORY_SAP_MUTE_ITEM_T eItem)
{
    return _AUD_DspGetSapMute(eItem);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetFMSatuMuteMode
 *   Set saturation mute thresholds
 *  @param  eItem lower threshold or higher threshold
 *  @retval  UINT8
 */
//-----------------------------------------------------------------------------
void AUD_DspSetFMSatuMuteTh ( AUD_FACTORY_SATU_MUTE_ITEM_T eChangedItem,
                                                                                        UINT8 u1Value)
{
	_AUD_DspSetFMSatuMuteTh(eChangedItem, u1Value);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetFMSatuMuteTh
 *   get saturation mute thresholds
 *  @param  eItem lower threshold or higher threshold
 *  @retval  UINT8
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetFMSatuMuteTh( AUD_FACTORY_SATU_MUTE_ITEM_T eItem)
{
    return _AUD_DspGetFMSatuMuteTh(eItem);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetVolumeTable
 *   Set volume table.
 *
 *  @param  u4VolTabChkPnt[]
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetVolumeTable(UINT32* u4VolTabChkPnt)  //Andrew Wen 07/8/20
{
    _AUD_DspSetVolumeTable(u4VolTabChkPnt);
}


//-----------------------------------------------------------------------------
/** AUD_DspGetVolumeTable
 *   Query volume table.
 *
 *  @param  u1VolTabIdx
 *
 *  @retval  volume gain
 */
//-----------------------------------------------------------------------------
UINT32 AUD_DspGetVolumeTable(UINT8 u1VolTabIdx)
{
    return _AUD_DspGetVolumeTable(u1VolTabIdx);
}


//-----------------------------------------------------------------------------
/** AUD_DspGetVolumeBaseLevel
 *   Get volume base level (0dB level).
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetVolumeBaseLevel(void)
{
    return _AUD_DspGetVolumeBaseLevel();
}

//-----------------------------------------------------------------------------
/** AUD_Surround_Get_Config
 *   Set new mtk vsurr config
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_Surround_Get_Config(AUD_VSURR_CFG_T* prVsurrCfg)
{
    _AUD_Surround_Get_Config(prVsurrCfg);
}

//-----------------------------------------------------------------------------
/** AUD_Surround_Set_Config
 *   Set new mtk vsurr config
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_Surround_Set_Config(AUD_VSURR_CFG_TYPE_T eVSCfgType, UINT32 u4VSParameter)
{
    _AUD_Surround_Set_Config(eVSCfgType, u4VSParameter);
}

 //audio depths links with video depths, by gallen/sammy, 20110526
#ifdef CC_3DTV_AUDIO_LINK_SUPPORT
void AUD_3D_AVLINK_Config(UINT32 u4DepthField)
{
    _AUD_3D_AVLINK_Config(u4DepthField);
}
#endif

void AUD_DspSetPcmPreBuffer(UINT8 u1DecId, UINT16 u2Length)
{
    _AUD_DspSetPcmPreBufferLength(u1DecId, u2Length);
}

UINT16 AUD_DspGetPcmPreBuffer(UINT8 u1DecId)
{
    return _AUD_DspGetPcmPreBufferLength(u1DecId);
}

void AUD_DspSetPCMChannelNumber(UINT8 u1DecId, UINT32 u4ChannelNo)
{
    _AUD_DspSetPCMChannelNumber(u1DecId, (UINT16)u4ChannelNo);
}

BOOL AUD_GetNicamStatus(void)
{
    return _AUD_GetNicamStatus();
}

#ifdef SUPPORT_PHILIPS_LOUDNESS
AUD_TYPE_T AUD_GetAudInfo(UINT8 u1DecId)
{
    return (_AUD_GetAudInfo(u1DecId));
}
#endif

void AUD_GetDtvAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo)
{
    _AUD_GetDtvAudInfo( u1DecId, prAudInfo);
}


#ifdef CC_AUD_LINEOUT_VOL_MODE
void AUD_DspSetLineOutVolumeMode(UINT8 u1DecId, UINT8 fgMode)
{
	_AUD_SetLineOutVolumeMode(u1DecId, fgMode);
}
UINT8 AUD_DspGetLineOutFixVol(void)
{
	return _AUD_DspGetLineOutFixVol();
}
#endif

//-----------------------------------------------------------------------------
/** AUD_DspSpeed
 *  Set dsp speed (non-fractional)
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Speed  : 0:normal play
 *                            : 2:FF 2-times
 *                            : 4:FF 4-times
 *                            : 8:FF 8-times
 *                            : 16:FF 16-times
 *                            : 32:FF 32-times
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSpeed(UINT8 u1DecId, UINT16 u2Speed)
{
	_AUD_DspSpeed(u1DecId, u2Speed);
}

//-----------------------------------------------------------------------------
/** AUD_DspDownmixPosition
 *  Set Downmix Channel (CH9/10) Position
 *
 *  @param  u1DMPosition   : 0: do ALL post processing
 *                         : 1: do downmix after input source gain and before AVC
 *                         : 2: Skip post-proc & do manual output matrix
 *                         : 3: Skip post-proc & skip manual output matrix
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspDownmixPosition(UINT8 u1DMPosition)
{
	_AUD_DspDownmixPosition(u1DMPosition);
}

UINT8 AUD_DspGetDownmixPosition(void)
{
    return _AUD_DspGetDownmixPosition();
}


//-----------------------------------------------------------------------------
/** AUD_DspMultiPairOutput
 *  Copy CH9/10 to other channel(LS/RS/C/SUB)
 *
 *  @param  u1DecId 0       : first decoder 1: seconder decoder.
 *  @param  u1MultiPair   : 0: OFF
 *                                   : 1: ON

 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspMultiPairOutput(UINT8 u1DecId, UINT16 u1MultiPair)
{
    _AUD_DspMultiPairOutput(u1DecId, u1MultiPair);
}

//-----------------------------------------------------------------------------
/** _AUD_DspLRDownmix
 *  Downmix to L/R channel after Reverbation and before BassManagement
 *
 *  @param  u1DecId 0       : first decoder 1: seconder decoder.
 *  @param  u1MultiPair   : 0: OFF
 *                                   : 1: ON

 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspLRDownmix(UINT8 u1DecId, BOOL fgEnable)
{
    _AUD_DspLRDownmix(u1DecId, fgEnable);
}

UINT16 AUD_DspGetSystemSetup(UINT8 u1DecId)
{
    return _AUD_DspGetSystemSetup(u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetDualDecMode
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetDualDecMode(BOOL fgEnable)
{
    _AUD_DspSetDualDecMode(fgEnable);
}

#ifdef CC_AUD_HPF_SUPPORT
//-----------------------------------------------------------------------------
/** AUD_DspSetHPFEnable
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetHPFEnable(BOOL fgEnable)
{
    _AUD_DspSetHPFEnable(fgEnable);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetHPFFc
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetHPFFc(UINT16 u2Fc)
{
    _AUD_DspSetHPFFc(u2Fc);
}
void AUD_DspSetHPF2Fc(UINT16 u2Fc)
{
    _AUD_DspSetHPF2Fc(u2Fc);
}

#endif

#ifdef CC_AUD_CLIPPER_SUPPORT
//-----------------------------------------------------------------------------
/** _AUD_DspSetClipperEnable
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetClipperEnable(BOOL fgEnable)
{
    _AUD_DspSetClipperEnable(fgEnable);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetClipperValue
 *
 *  @param  value
 *  @retval void
 */
//-----------------------------------------------------------------------------

void AUD_DspSetClipperValue(AUD_CH_T eChannel, UINT32 u4Value)
{
    _AUD_DspSetClipperValue(eChannel, u4Value);
}
#endif

#ifdef CC_AUD_SPEAKER_HEIGHT
#ifndef CC_AUD_SPEAKER_HEIGHT_MODE
//-----------------------------------------------------------------------------
/** AUD_DspSetSPHEnable
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetSPHEnable(BOOL fgEnable)
{
    _AUD_DspSetSPHEnable(fgEnable);
}
#else
//-----------------------------------------------------------------------------
/** AUD_DspSetSPHMode
 *
 *  @param  0:off 1:2DB 2:4DB 3:6DB
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetSPHMode(UINT8 u1Mode)
{
    _AUD_DspSetSPHMode(u1Mode);
}
#endif
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspSetPEQEnable
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetPEQEnable(BOOL fgEnable)
{
    _AUD_DspSetPEQEnable(fgEnable);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetPEQCfg
 *
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param u1Set                      PEQ set (0 ~ 7)
 *  @param u4Frequency                Frequency (20 ~ 20K)
 *  @param u2BW                       Bandwidth (xx Hz)
 *  @param i2Gain                     Gain (+-12dB) with 0.5dB/step
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetPEQCfg(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 u4Frequency,
                            UINT16 u2BW, INT16 i2Gain)
{
    _AUD_DspSetPEQCfg(u1DecId, u1PEQSet, u4Frequency, u2BW, i2Gain);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetPEQCfg2
 *
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param u1Set                      PEQ set (0 ~ 7)
 *  @param u4Frequency                Frequency (20 ~ 20K)
 *  @param eQValue                    Q
 *  @param i2Gain                     Gain (+-12dB) with 0.5dB/step
 *  @retval void
 */
//-----------------------------------------------------------------------------
#if 0 //sharp Q setting
void AUD_DspSetPEQCfg2(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 u4Frequency,
                            AUD_PEQ_Q_TYPE_T eQValue, INT16 i2Gain)
{
    _AUD_DspSetPEQCfg2(u1DecId, u1PEQSet, u4Frequency, eQValue, i2Gain);
}
#else
void AUD_DspSetPEQCfg2(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 u4Frequency,
                            UINT32 eQValue, INT16 i2Gain)
{
    _AUD_DspSetPEQCfg2(u1DecId, u1PEQSet, u4Frequency, eQValue, i2Gain);
}
#endif
#ifdef CC_AUD_VBASS_SUPPORT
//-----------------------------------------------------------------------------
/** AUD_DspSetVirBassEnable
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetVirBassEnable(BOOL fgEnable)
{
   _AUD_DspSetVirBassEnable(fgEnable);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetVirBassCfg
 *
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param u4Frequency                Frequency (20 ~ xx)
 *  @param u4Gain
 *  @param u4Gain2
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetVirBassCfg(UINT8 u1DecId, UINT32 u4Frequency, UINT32 u4Gain,
                        UINT32 u4Gain2)
{
    _AUD_DspSetVirBassCfg(u1DecId, u4Frequency, u4Gain, u4Gain2);
}

void AUD_DspGetVirBassCfg(UINT8 u1DecId, BOOL *fgEnable, UINT32 *u4Gain, UINT32 *u4Gain2)
{
    UNUSED(u1DecId);
    UNUSED(AUD_DspGetVirBassCfg);

    _AUD_DspGetVirBassCfg(u1DecId, fgEnable, u4Gain, u4Gain2);
}
#else
// In Linux even not define CC_AUD_VBASS_SUPPORT, it still need this 2 SYMBOLs ??
// prepare a UNUSED FUNNCTION for it.
void AUD_DspSetVirBassEnable(BOOL fgEnable)
{
    UNUSED(AUD_DspSetVirBassEnable);
    return;
}
void AUD_DspSetVirBassCfg(UINT8 u1DecId, UINT32 u4Frequency, UINT32 u4Gain, UINT32 u4Gain2)
{
    UNUSED(AUD_DspSetVirBassCfg);
    return;
}
#endif

#ifdef CC_AUD_FIR_SUPPORT
void AUD_DspSetFIREnable(UINT8 u1DecId, UINT8 u1ConeqMode)
{
   _AUD_DspSetFIREnable(u1ConeqMode);
}
#endif


//-----------------------------------------------------------------------------
/** AUD_NVM_Get_Profile
 *   Query Audio Profile Start Address
 *
 *  @param  pAudProfileStart
 *  @param  pAudProfileEnd
 *  @param  u1Profile
 *
 *  @retval
 */
//-----------------------------------------------------------------------------
UINT32 AUD_NVM_Get_Profile(UINT32* pAudProfileStart, UINT32* pAudProfileEnd, UINT8* u1Profile)
{
#ifndef CC_AUD_USE_NVM
    return 1;
#else
    return _AUD_NVM_Get_Profile(pAudProfileStart, pAudProfileEnd, u1Profile);
#endif
}

//-----------------------------------------------------------------------------
/** AUD_NVM_Read
 *   Read Audio NVM Data from EEPROM
 *
 *  @param
 *
 *  @retval
 */
//-----------------------------------------------------------------------------
UINT32 AUD_NVM_Read(void)
{
    // If not define CC_AUD_USE_NVM, this function do nothing.
    return _AUD_NVM_Read();
}
#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
//-----------------------------------------------------------------------------
/** AUD_DspADVEnable
 *
 *  @param  disable,light/medium/heavy compressiong
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspADVEnable(UINT8 fgEnable)
{
   _AUD_DspSetADVEnable(fgEnable);
}

//-----------------------------------------------------------------------------
/** AUD_DspADVStatus
 *
 *  @
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspADVStatus(void)
{
   _AudShowADVStatus();
}

//-----------------------------------------------------------------------------
/** AUD_DspADVSetChGain
 *
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspADVSetChGain(UINT8 ch,UINT32 value)
{
   _AUD_DspADVSetChGain(ch,value);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetABXEnable
 *
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetABXEnable(UINT8 fgEnable)
{
    _AUD_DspABXConfig(0, fgEnable);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetAEQEnable
 *
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetAEQEnable(BOOL fgEnable)
{
    _AUD_DspAEQEnable(0, fgEnable);
}

// ************************************************************
// Function : void AUD_DspAEQEnable(UINT8 u1DecId, BOOL fgEnable)
// Description : turn on / off the AEQ control
// Parameter : bMode - ADSP_ON / ADSP_OFF
// Return : None.
// ************************************************************
void AUD_DspAEQEnable(UINT8 u1DecId, BOOL fgEnable)
{
    _AUD_DspAEQEnable(u1DecId, fgEnable);
}

void AUD_DspAEQQuery(void)
{
    _AUD_DspAEQQuery();
}

void AUD_DspABXQuery(void)
{
    _AUD_DspABXQuery();
}

void AUD_DspABXConfig(UINT8 item, UINT8 val)
{
    _AUD_DspABXConfig(item, val);
}
#endif

//-----------------------------------------------------------------------------
/** AUD_DspDDCOEnable
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspDDCOEnable(BOOL fgEnable)
{
   _AUD_DspSetDDCOEnable(fgEnable);
}

void AUD_DspAtvMuteEnable(UINT8 u1DecId, BOOL fgEnable)
{
   _AUD_DspAtvMuteEnable(u1DecId,fgEnable);
}

//-----------------------------------------------------------------------------
/** AUD_DspDDCOAgcEnable
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspDDCOAgcEnable(BOOL fgEnable)
{
   _AUD_DspSetDDCOAgcEnable(fgEnable);
}

//-----------------------------------------------------------------------------
/** AUD_DspDDCOLFEEnable
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspDDCOLFEEnable(BOOL fgEnable)
{
   _AUD_DspSetDDCOLFEEnable(fgEnable);
}

//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
extern void _AUD_DspSetUploadDataEnable (void);
extern void _AUD_DspSetUploadMode(UINT8 u1Mode);
extern void _AUD_DataUpload_Init (void);
extern void _AUD_DataUpload_start (void);
extern void _AUD_GetUploadInfo(UINT32* u4Pts,UINT32* u4Addr, UINT32* u4Size);
extern void _AUD_GetYWUploadInfo(UINT32* u4base, UINT32* u4Size);
extern void _AUD_GetUploadPCMInfo(UINT32* uPCMBits, UINT32* uPCMChannels, UINT32* uPCMSampleRate);
extern BOOL _AUD_Dec3_Is_Mp3Enc(void);
//-----------------------------------------------------------------------------
/** AUD_DspUploadDataEnable
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspUploadDataEnable(void)
{
   _AUD_DspSetUploadDataEnable();
}

//-----------------------------------------------------------------------------
/** AUD_DspSetUploadMode
 *
 *  @param  0: off, 1: PCM, 2: MP3, 3: SBC Encoder, 4: ALSA, 5: WiFi
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetUploadMode(UINT8 u1Mode)
{
    _AUD_DspSetUploadMode(u1Mode);
}
//-----------------------------------------------------------------------------
/** AUD_DataUpload_Init
 */
//-----------------------------------------------------------------------------
void AUD_DataUpload_Init (void)
{
    _AUD_DataUpload_Init();
}
//-----------------------------------------------------------------------------
/** AUD_DataUpload_start
 */
//-----------------------------------------------------------------------------
void AUD_DataUpload_start (void)
{
    _AUD_DataUpload_start();
}

void AUD_GetUploadInfo(UINT32* u4Pts,UINT32* u4Addr, UINT32* u4Size)
{
    _AUD_GetUploadInfo(u4Pts, u4Addr, u4Size);
}
void AUD_GetYWUploadInfo(UINT32* u4base, UINT32* u4Size)
{
    _AUD_GetYWUploadInfo(u4base, u4Size);
}

void AUD_GetUploadPCMInfo(UINT32* uPCMBits, UINT32* uPCMChannels, UINT32* uPCMSampleRate)
{
    _AUD_GetUploadPCMInfo(uPCMBits, uPCMChannels, uPCMSampleRate);
}
BOOL AUD_Dec3_Is_Mp3Enc(void)
{
    return _AUD_Dec3_Is_Mp3Enc();
}
//#endif

#ifdef SUPPORT_PHILIPS_LOUDNESS
/** AUD_DspSetLoudness
 *
 *  @param  u1DecId, enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetLoudness(UINT8 u1DecId, BOOL fgEnable)
{
   _AUD_DspSetLoudness(u1DecId, fgEnable);
}

BOOL AUD_DspGetLoudness(UINT8 u1DecId)
{
   return _AUD_DspGetLoudness(u1DecId);
}
#endif

//-----------------------------------------------------------------------------
/** AUD_DspGetMpegAudInfo
 *
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param *prAudInfo                audio info structure pointer
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspGetMpegAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo)
{
    _AUD_DspGetMpegAudInfo(u1DecId, prAudInfo);
}

/** AUD_GetAacVersion
 *
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @retval version
 */
//-----------------------------------------------------------------------------
UINT8 AUD_GetAacVersion(UINT8 u1DecId)
{
    return _AUD_GetAacVersion(u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_GetAacTSFormat
 *
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @retval TSFormat
 */
//-----------------------------------------------------------------------------
UINT8 AUD_GetAacTSFormat(UINT8 u1DecId)
{
    return _AUD_GetAacTSFormat(u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_GetAC3_EAC3_Info
 *
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @retval version
 */
//-----------------------------------------------------------------------------
UINT8 AUD_GetAC3_EAC3_Info(UINT8 u1DecId)
{
    return _AUD_GetAC3_EAC3_Info(u1DecId);
}


//-----------------------------------------------------------------------------
/** AUD_DspGetMpegAudInfoWithLayer
 *
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param *prAudInfo                audio info structure pointer
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspGetMpegAudInfowithLayer(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo, UINT8 *prMPEGLayer)
{
    _AUD_DspGetMpegAudInfoWithLayer(u1DecId, prAudInfo, prMPEGLayer);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetMp3PreparsingMode
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetMp3PreparsingMode(BOOL fgEnable)
{
    _AUD_DspSetMp3PreparsingMode(fgEnable);
}

//-----------------------------------------------------------------------------
/** AUD_DspIsMp3Vbr
 *
 *  @param  void
 *  @retval true (VBR)  false(CBR)
 */
//-----------------------------------------------------------------------------
BOOL AUD_DspIsMp3Vbr(void)
{
    return _AUD_DspIsMp3Vbr();
}

//-----------------------------------------------------------------------------
/** AUD_DspCOOKParameter
 *  setup COOK information
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  prCookInfo  : pointer of COOK header information
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspCOOKParameter(UINT8 u1DecId, AUD_COOK_CFG_T* prCookInfo)
{
	_AUD_DspCOOKParameter(u1DecId, prCookInfo);
}

#ifdef CC_AUD_SETUP_MELODY
void AUD_DspSetMelodyVol(UINT8 u1Vol)
{
	_AUD_DspSetMelodyVol(u1Vol);
}
#endif

#ifdef CC_AUD_MIXSOUND_SUPPORT
void AUD_DspSetMixSndLoop(UINT32 u4Loop)
{
    _AUD_DspSetMixSndLoop(u4Loop);
}

void AUD_DspSetMixSndVol(UINT32 u4Vol)
{
    _AUD_DspSetMixSndVol(u4Vol);
}

UINT32 AUD_DspGetMixSndLoop(void)
{
    return _AUD_DspGetMixSndLoop();
}

UINT32 AUD_DspGetMixSndVol(void)
{
    return _AUD_DspGetMixSndVol();
}
#endif

#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
void AUD_DspSetMixSndLength(UINT32 u4Len)
{
    _AUD_DspSetMixSndLength(u4Len);
}

void AUD_DspSetMixSndGain(UINT32 u4Gain)
{
    _AUD_DspSetMixSndGain(u4Gain);
}

UINT32 AUD_DspGetMixSndGain(void)
{
    return _AUD_DspGetMixSndGain();
}

void AUD_DspSetMixSndStereo(UINT8 bOnOff)
{
    _AUD_DspSetMixSndStereo(bOnOff);
}

BOOL AUD_DspGetMixSndStereo(void)
{
    return _AUD_DspGetMixSndStereo();
}

void AUD_DspSetMixSndUpSampleRatio(UINT8 bRatio)
{
    _AUD_DspSetMixSndUpSampleRatio(bRatio);
}

#ifndef NEW_MIXSOUND
void AUD_DspSetMixSndMemPlay(UINT8 bOnOff)
{
    _AUD_DspSetMixSndMemPlay(bOnOff);
}
#endif

BOOL AUD_DspIsMixSndPlay(void)
{
    return _AUD_DspIsMixSndPlay();
}

// 0x0100: Mix to L/R
// 0x0200: Mix to SL/SR
// 0x0400: Mix to Center
// 0x0800: Mix to CH7/CH8
// 0x1000: Mix to CH9/CH10
void AUD_DspSetMixSndChannel(UINT16 wCh)
{
    _AUD_DspSetMixSndChannel(wCh);
}

UINT32 AUD_DspGetMixSndReadPtr(UINT8 u1StreamID)
{
    return _AUD_DspGetMixSndReadPtr(u1StreamID);
}

void AUD_DspSetMixSndWritePtr(UINT8 u1StreamID, UINT32 u4WritePtr)
{
    _AUD_DspSetMixSndWritePtr(u1StreamID, u4WritePtr);
}

void AUD_DspShowMixSndStatus(UINT8 u1StreamID)
{
    _AUD_DspShowMixSndStatus(u1StreamID);
}
#endif

//-----------------------------------------------------------------------------
/** AUD_DspSetWithVideo
 *   Set is with video
 *
 *  @param  u1DecId         Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  fgIsWithVideo   TRUE (with video). FALSE (no video)
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetWithVideo(UINT8 u1DecId, BOOL fgIsWithVideo)
{
    _AUD_DspSetWithVideo(u1DecId, fgIsWithVideo);
}

//-----------------------------------------------------------------------------
/** AUD_DspIsWithVideo
 *   Is with video
 *
 *  @param  u1DecId         Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
BOOL AUD_DspIsWithVideo(UINT8 u1DecId)
{
    return _AUD_DspIsWithVideo(u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetCapability
 *   Set Capability
 *
 *  @param      void
 *  @retval     void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetCapability(void)
{
    _AUD_DspSetCapability();
}

//-----------------------------------------------------------------------------
/** AUD_DspGetCapability
 *   Get Capability
 *
 *  @param      void
 *  @retval     format capability
 */
//-----------------------------------------------------------------------------
UINT32 AUD_DspGetCapability(UINT8 u1Idx)
{
    return _AUD_DspGetCapability(u1Idx);
}

//-----------------------------------------------------------------------------
/** AUD_DspIsFormatValid
 *  Is format valid
 *
 *  @param      u4FormatInfo: format index
 *  @retval     TRUE(valid). FALSE(invalid)
 */
//-----------------------------------------------------------------------------
BOOL AUD_DspIsFormatValid(UINT32 u4FormatInfo)
{
    return _AUD_DspIsFormatValid(u4FormatInfo);
}

BOOL AUD_DspIsSupportAC3(void)
{
    return _AUD_DspIsSupportAC3();
}

BOOL AUD_DspIsSupportEAC3(void)
{
    return _AUD_DspIsSupportEAC3();
}

BOOL AUD_DspIsSupportDDCO(void)
{
    return _AUD_DspIsSupportDDCO();
}


//-----------------------------------------------------------------------------
/** AUD_DspQueryChannelDelay
 *
 *
 *  @param      u1ChIdx
 *  @retval     Delay in ms
 */
//-----------------------------------------------------------------------------
UINT32 AUD_DspQueryChannelDelay(UINT8 u1ChIdx) //QUERY_DELAY
{
    return _AUD_DspQueryChannelDelay(u1ChIdx);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetStcId
 *  Set Stc ID for audio / DSP
 *
 *  @param           
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetStcId(UINT8 u1DecId, UINT16 u2StcId) // 2_STC_Task
{
    _AUD_DspSetStcId(u1DecId, u2StcId);
}

void AUD_DspGetStcId(UINT8 u1DecId, UINT16 *u2StcId) // 2_STC_Task
{
    _AUD_DspGetStcId(u1DecId, u2StcId);

}

