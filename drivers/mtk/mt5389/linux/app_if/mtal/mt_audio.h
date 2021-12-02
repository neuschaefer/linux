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
 * $Author: hongjun.chu $
 * $Date: 2012/04/28 $
 * $RCSfile: mt_audio.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file mt_audio.h
 *  MTAL kernel driver implementation: Audio decoder
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#ifndef	MT_AUDIO_H
#define MT_AUDIO_H

#include "x_linux.h"
#include "aud_if.h"

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

/**Initialize Auio Decider module.
*@param void
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_Init (void);


/**Get audio decoder type and stream source configuration.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param peStreamFrom               Stream source.
*@param prDecType                  Decoder format.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_GetDecType (UINT8 u1DecId,
                                       MTAUDDEC_DEC_STREAM_FROM_T *peStreamFrom,
                                       MTAUDDEC_FMT_T *prDecType);

/**Set audio decoder type and stream source.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eStreamFrom                Stream source.
*@param eDecFmt                    Decoder format.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetDecType (UINT8 u1DecId,
                                       MTAUDDEC_DEC_STREAM_FROM_T eStreamFrom,
                                       MTAUDDEC_FMT_T eDecFmt);

/**Audio decoder playback.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_Play (UINT8 u1DecId);


/**Resume audio decoder.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_Resume (UINT8 u1DecId);


/**Pause audio decoder.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_Pause (UINT8 u1DecId);


/**Stop audio decoder.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_Stop (UINT8 u1DecId);

/**Audio decoder call back function register.
*@param eFuncType                  The call back function type.
*@param u4FuncPtr                  The call back function pointer.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_RegCb (MTAUDDEC_CB_FUNC_ID_T eFuncType, UINT32 u4FuncPtr,
                                  UINT32 u4Arg1, UINT32 u4Arg2);


/**Get the information of audio decoder.
*  Since in some case, even application issue play to decoder, but if the
*  bitstream is not feed to decoder, there is no audio output yet.
*  This function indicates the status that if the decoding is correctly getting
*  bistream and successfully decoded.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param pfgPlay                    Is decoder real play
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_GetDecOnPlayingStatus (UINT8 u1DecId,
        BOOL* pfgOnPlay);


/**Set AV synchronization mode.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eSynMode                   AV sync mode or just free run in DTV playback.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetAvSynMode (UINT8 u1DecId,
        MTAV_SYNC_MODE_T eSynMode);

/**Setup AC3 compression mode.
* US/Austria model specific function
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eCompMode                  Compression mode.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetAc3CompMode (UINT8 u1DecId,
        MTAUDDEC_AC3_COMP_MODE_T eCompMode);


/**Setup AC3 downmix mode.
* US/Austria model specific function
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eDownmixMode               Downmix mode.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetAc3DownmixMode (UINT8 u1DecId,
        MTAUDDEC_AC3_DOWNMIX_MODE eDownmixMode);


/**Get AC3 downmix mode.
* US/Austria model specific function
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eDownmixMode               Downmix mode.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_GetAc3DownmixMode (UINT8 u1DecId,
        MTAUDDEC_AC3_DOWNMIX_MODE * peDownmixMode);

/**Get AC3 audio mode.
* US/Austria model specific function
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param peMode                     AC3 channel information.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_GetAc3Mode (UINT8 u1DecId,
                                       MTAUDDEC_MODE_T * peMode);

/**Get AC3 ES info.
* US/Austria model specific function
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param peAC3ESInfo             AC3 ES information.
*@retval MTR_OK                     Success.
*@retval MTR_NOT_OK              Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_GetAC3ESInfo (UINT8 u1DecId, MTAUDDEC_AC3_ES_INFO_T *peAC3ESInfo);

/**Get audio decoder mpeg esinfo.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param pAC3ESInfo            audio es info.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T _MTAUD_GetMPEGESInfo(UINT8 u1DecId, MTAUDDEC_MPEG_ES_INFO_T *peMPEGESInfo);

/**Get audio decoder mpeg esinfo.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param pAC3ESInfo            audio es info.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTAUDDEC_GetMPEGESInfo(UINT8 u1DecId, MTAUDDEC_MPEG_ES_INFO_T *pMPEGESInfo);

/**Get HEAAC ES info.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param peHEAACESInfo         HEAAC ES information.
*@retval MTR_OK                     Success.
*@retval MTR_NOT_OK              Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_GetHEAACESInfo (UINT8 u1DecId, MTAUDDEC_HEAAC_ES_INFO_T *peMPEGESInfo);


/**Get audio sampling rate.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param pSmpRate                   Sampling rate.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_GetSamplingRate (UINT8 u1DecId,
        MTAUDDEC_SAMPLE_FREQ_T * pSmpRate);

/**Setup range for dynamix range compression.
* US/Austria model specific function
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1DrcLevel                 DRC range.
*                                  0 ~ 8 (step size is 0.125)
*                                  0 -> 0.0
*                                  1 -> 0.125
*                                  8 -> 1.0
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetAc3DrcRange (UINT8 u1DecId, UINT8 u1DrcLevel);

/**Setup 0~100 range for dynamix range compression.
* US/Austria model specific function
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1DrcLevel                 DRC range 0~100.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetAc3DrcRange100 (UINT8 u1DecId, UINT8 u1DrcLevel);



/**Setup AC3 dual mono mode.
* US/Austria model specific function
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eDualMonoMode              Dual mono mode
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetAc3DualMono (UINT8 u1DecId,
        MTAUDDEC_AC3_DUALMONO_MODE eDualMonoMode);


/**Setup AC3 dual mono mode.
* US/Austria model specific function
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eDualMonoMode              Dual mono mode
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_GetAc3DualMono (UINT8 u1DecId,
        MTAUDDEC_AC3_DUALMONO_MODE * peDualMonoMode);


/**Get the analogy TV sound system of current channel.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param peATVSoundSys              Sound system
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_GetAtvSoundSys (UINT8 u1DecId,
        MTAUDDEC_SIF_STANDARD_T *peATVSoundSys);


/**Get the analogy TV stereo mode of current channel.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param peATVMode                  Detected mode
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_GetAtvDetectMode (UINT8 u1DecId,
        MTAUDDEC_ATV_DETECT_MODE_T *peATVMode);


/**Setup analog TV output mode.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eSoundMode                 UNKNOWN = 0,
*                                  MONO,
*                                  STEREO,
*                                  SAP,
*                                  DUAL1,
*                                  DUAL2,
*                                  NICAM_MONO,
*                                  NICAM_STEREO,
*                                  NICAM_DUAL1,
*                                  NICAM_DUAL2,
*                                  A2_DUAL1,
*                                  A2_DUAL2,
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetAtvOutputMode (UINT8 u1DecId,
        MTAUDDEC_SOUND_MODE_T eSoundMode);

/**Setup SIF input mode
* DVB EU model specific function
* Should set to audio decoder before issue playback at EU/KOREA/EIAJ model.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eSIFMode                   SIF Mode
*                                  NICAM BG,
*                                  FM BG,
*                                  A2 BG,
*                                  NICAM I,
*                                  FM I,
*                                  NICAM DK,
*                                  FM DK,
*                                  A2 DK1,
*                                  A2 DK3,
*                                  NICAM L,
*                                  AM L,
*                                  A2 KOREA
*                                  BTSC
*                                  EIAJ
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetSIFMode (UINT8 u1DecId,
                                       MTAUDDEC_SIF_STANDARD_T eSIFMode);

/**Get detected SIF mode
* DVB EU model specific function
* Issue decoder to do SIF mode detection. It may take some time for the detection.
* Notes that application should stop decoder before invoking this function.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eSIFMode                   FM DK or AM L
*                                  Since audio decoder cannot distinguish from
*                                  FM DK and AM L, so application should set this
*                                  information to audio decoder at first before
*                                  issue decoder to do detection.
*@param peSIFMode                  Detection result
*                                  MODE_DETECT = 0,
*                                  NICAM BG,
*                                  FM BG,
*                                  A2 BG,
*                                  NICAM I,
*                                  FM I,
*                                  NICAM DK,
*                                  FM DK,
*                                  A2 DK1,
*                                  A2 DK3,
*                                  NICAM L,
*                                  AM L,
*                                  A2 KOREA
*                                  BTSC
*                                  EIAJ
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/

EXTERN MT_RESULT_T  _MTAUD_GetDetectSIFMode (UINT8 u1DecId,
        MTAUDDEC_SIF_STANDARD_T eSIFMode,
        MTAUDDEC_SIF_STANDARD_T* peSIFMode);

/**Setup PCM information.
* Used in audio clip playback.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param prPcmSetting               PCM setting
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetPcmInfo (UINT8 u1DecId,
                                       MTAUDDEC_PCM_SETTING_T* prPcmSetting);


/**Setup individual channel delay
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eChIndex                   Individual channel.
*@param u2Delay                    Delay (0~300 ms with 1 ms/step).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetChannelDelay (UINT8 u1DecId, MTAUDDEC_CH_T eChIndex,
        UINT16 u2Delay);


/**Setup individual channel delay no uop
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eChIndex                   Individual channel.
*@param u2Delay                    Delay (0~300 ms with 1 ms/step).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetChannelDelayNoUop (UINT8 u1DecId, MTAUDDEC_CH_T eChIndex,
        UINT16 u2Delay);

/**Setup individual channel volume.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eChIndex                   Individual channel.
*@param u1Value                    0~100.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetChannelVolume (UINT8 u1DecId, MTAUDDEC_CH_T eChannel,
        UINT8 u1Value);

/**Setup audio description mode.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1Mode                     0x02 0x08 0xa.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetADMode(UINT8 u1DecId,  UINT8 u1Mode);


/**Setup audio description enable.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1Mode                     enable/disable.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetADEnable(UINT8 u1DecId,  UINT8 u1Mode);

#if 1
/**_MTAUD_LoadAudioClip.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1BufferSize              u1BufferSize
*@param ClipBufferPointer        ClipBufferPointer
*@param u1DecFmt                  DecFmt
*@param u1RepeatNumber        RepeatNumber
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_LoadAudioClip (UINT8 u1DecId, UINT32 u1BufferSize, UINT32 ClipBufferPointer, MTAUDDEC_FMT_T u1DecFmt, UINT32 u1RepeatNumber);
#else
/**_MTAUD_LoadAudioClip.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1BufferSize              u1BufferSize
*@param ClipBufferPointer        ClipBufferPointer
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_LoadAudioClip (UINT8 u1DecId, UINT32 u1BufferSize, UINT32 ClipBufferPointer);
#endif


/**_MTAUD_PlayAudioClip.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param ReaptNumber             clip ReaptNumber
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T _MTAUD_PlayAudioClip(UINT8 u1DecId,  UINT32 ReaptNumber );

/**Stop audio clip.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_StopAudioClip (UINT8 u1DecId);

/**_MTAUD_DeleteAudioClip.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_DeleteAudioClip (void);
/**Mute individual channel
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eChannel                   Individual channel.
*@param fgMute                     Mute or not.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetChannelMute (UINT8 u1DecId, MTAUDDEC_CH_T eChannel,
        BOOL fgMute);

/**Setup extra gain for individual channel.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eChannel                   Individual channel.
*@param i4Value                    -256~96. (-128 ~ 48 dB with 0.5 dB/step)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetChannelVolGain (UINT8 u1DecId, MTAUDDEC_CH_T eChannel,
        INT32 i4Value);

/**Setup volume balance.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1Balance                  Balance value (0~100).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetBalance (UINT8 u1DecId, UINT8 u1Balance);


/**Setup master volume.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1Volume                   Volume value (0~100).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetMasterVolume (UINT8 u1DecId, UINT8 u1Volume);

/**Setup input source volume.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetSourceVolume (UINT8 u1DecId);

/**Setup Auto volume control (AVC).
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetAvc(UINT8 u1DecId, BOOL fgEnable);

/**Setup 2 band AVC parameters
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1Band                     Band number (0: Low band, 1: High band)
*@param eAVCCfgType                Parameter type
*@param i4Parameter                Parameter value
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T _MTAUD_Set2BandAvcCfg(UINT8 u1DecId, UINT8 u1Band,
        MTAUDDEC_AVC_CFG_TYPE_T eAVCCfgType, INT32 i4Parameter);

/**Setup BBE
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetBBE(UINT8 u1DecId, BOOL fgEnable);


/**Setup SRS
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetSRS(UINT8 u1DecId, BOOL fgEnable);

/**Setup virtural surround
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetVirtualSurround(UINT8 u1DecId, BOOL fgEnable);

/**Setup bass boost.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetBassBoost (UINT8 u1DecId, BOOL fgEnable);

/**Setup bass
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param i2Bass                     bass value  ( +-14 steps, with 0.5 dB/step)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetBass (UINT8 u1DecId, INT16 i2Bass);


/**Setup treble boost.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u2CompMode                 Compression mode.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetTrebleBoost(UINT8 u1DecId, BOOL fgEnable);

/**Setup Treble
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param i2Treble                   Treble value  ( +-14 steps, with 0.5 dB/step)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetTreble (UINT8 u1DecId, INT16 i2Treble);

/**Setup EQ.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetEqualizer(UINT8 u1DecId, BOOL fgEnable);

/**Setup EQ band configuration.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@param i2Band0                    band 0 setup ( +-28, with 0.5 dB/step)
*@param i2Band1                    band 1 setup ( +-28, with 0.5 dB/step)
*@param i2Band2                    band 2 setup ( +-28, with 0.5 dB/step)
*@param i2Band3                    band 3 setup ( +-28, with 0.5 dB/step)
*@param i2Band4                    band 4 setup ( +-28, with 0.5 dB/step)
*@param i2Band5                    band 5 setup ( +-28, with 0.5 dB/step)
*@param i2Band6                    band 6 setup ( +-28, with 0.5 dB/step)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetEqualizerBandConfig(UINT8 u1DecId, INT16 i2Band0,
        INT16 i2Band1, INT16 i2Band2, INT16 i2Band3,
        INT16 i2Band4, INT16 i2Band5, INT16 i2Band6);

/**Setup line-in input mux
*@param  eInputId                  Input mux id.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetAvInputMuxSel (MTAUDDEC_INPUT_ID_T eInputId);

/**Setup line-in input mux by AD
*@param  eADNum                  Input mux ADC id.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetAvInputMuxSelByAD(UINT8 eADNum);

/**Setup Dual mode configuration.
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetDualMode (BOOL fgEnable);

/**Setup High Deveation mode configuration.
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetHDevMode (BOOL fgEnable);


/**Setup IEC configuration.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eIecCfg                    IEC format.
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetIecConfig (UINT8 u1DecId, MTAUDDEC_IEC_T eIecCfg,
        BOOL fgEnable);

/**Setup IEC refer to which channel.
*@param eIecChannel                Channel number.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetIecChannel (MTAUDDEC_PCM_CH_T eChannel);

/**Setup IEC copyright.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetIecCopyright(UINT8 u1DecId, BOOL fgEnable);

/**Setup IEC copyright.
*@param spdif_ctrl                  SPDIF control
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetSpdifSCMS(UINT8 spdif_ctrl);

/**Get IEC copyright.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param pfgEnable                  on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_GetIecCopyright(UINT8 u1DecId, BOOL * pfgEnable);

/**Setup IEC category code
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1CategoryCode             Category code.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetIecCategoryCode(UINT8 u1DecId, UINT8 u1CategoryCode);

/**Get IEC category code
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param pu1CategoryCode            Category code.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_GetIecCategoryCode(UINT8 u1DecId, UINT8 * pu1CategoryCode);

/**Setup IEC category code
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1WordLength               Word length.(in bits, range: 16~24)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetIecWordLength(UINT8 u1DecId, UINT8 u1WordLength);

/**Get IEC category code
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param pu1WordLength              Word length.(in bits, range: 16~24)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_GetIecWordLength(UINT8 u1DecId, UINT8 * pu1WordLength);

/**Setup Speaker output mode.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eOutputMode                L+R = 0,
*                                  L+L,
*                                  R+R,
*                                  MIX
*
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetSpeakerOutMode (UINT8 u1DecId,
        MTAUDDEC_SKP_OUTPUT_MODE_T eOutputMode);

/**_MTAUD_GetAudioPTS.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param Pts                            pts
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T _MTAUD_GetAudioPTS(UINT8 u1DecId, UINT32 *Pts);

/**Setup individual channel volume.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eChIndex                   Individual channel.
*@param i2Value                    Volume value in 0.25dB/step.
*                                  Range: 80 ~ -512 (+20dB ~ -128dB)
*                                  80   ->  +20   dB
*                                  79   ->  +19.75dB
*                                  78   ->  +19.5 dB
*                                  77   ->  +19.25dB
*                                  76   ->  +19   dB
*                                   :        :
*                                  0    ->  0     dB
*                                  -1   ->  -0.25 dB
*                                  -2   ->  -0.5  dB
*                                  -3   ->  -0.75 dB
*                                  -4   ->  -1    dB
*                                   :       :
*                                  -512 ->  -128  dB
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetChannelVolumeInDB (UINT8 u1DecId,
        MTAUDDEC_CH_T eChannel, INT16 i2Value);

/**Setup balance value in dB.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param i2LeftDB                   Volume value in 0.25dB/step.
*                                  Range: 0 ~ -512
*                                  0    ->  0     dB
*                                  -1   ->  -0.25 dB
*                                  -2   ->  -0.5  dB
*                                  -3   ->  -0.75 dB
*                                  -4   ->  -1    dB
*                                   :       :
*                                  -512 ->  -128  dB
*@param i2LeftDB                   Volume value in 0.25dB/step.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetBalanceInDB (UINT8 u1DecId, INT16 i2LeftDB,
        INT16 i2RightDB);



/**Setup master volume in dB.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param i2Volume                   Volume value in 0.25dB/step.
*                                  Range: 0 ~ -512
*                                  0    ->  0     dB
*                                  -1   ->  -0.25 dB
*                                  -2   ->  -0.5  dB
*                                  -3   ->  -0.75 dB
*                                  -4   ->  -1    dB
*                                   :       :
*                                  -512 ->  -128  dB
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetMasterVolumeInDB (UINT8 u1DecId, INT16 i2Volume);

/**Setup master volume for bypass channel in dB.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param i2Volume                   Volume value in 0.25dB/step.
*                                  Range: 0 ~ -512
*                                  0    ->  0     dB
*                                  -1   ->  -0.25 dB
*                                  -2   ->  -0.5  dB
*                                  -3   ->  -0.75 dB
*                                  -4   ->  -1    dB
*                                   :       :
*                                  -512 ->  -128  dB
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetMasterVolumeForBypassChannelInDB (UINT8 u1DecId,
        INT16 i2Volume);

/**Setup clipper threshold in dB.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eChIndex                   Individual channel.
*@param i2Value                    Volume value in 0.25dB/step.
*                                  Range: 0 ~ -512 (0dB ~ -128dB)
*                                  0    ->  0     dB
*                                  -1   ->  -0.25 dB
*                                  -2   ->  -0.5  dB
*                                  -3   ->  -0.75 dB
*                                  -4   ->  -1    dB
*                                   :       :
*                                  -512 ->  -128  dB
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetClipperThresholdInDB (UINT8 u1DecId,
        MTAUDDEC_CH_T eChannel, INT16 i2Value);

/**Setup pre-gain in dB.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param i2Value                    Volume value in 0.25dB/step.
*                                  Range: 0 ~ -512 (0dB ~ -128dB)
*                                  0    ->  0     dB
*                                  -1   ->  -0.25 dB
*                                  -2   ->  -0.5  dB
*                                  -3   ->  -0.75 dB
*                                  -4   ->  -1    dB
*                                   :       :
*                                  -512 ->  -128  dB
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetPreGainInDB (UINT8 u1DecId, INT16 i2Value);

/**Setup AVC parameters
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eAVCCfgType                Parameter type
*@param i4Parameter                Parameter value
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T _MTAUD_SetAvcCfg(UINT8 u1DecId,
                                    MTAUDDEC_AVC_CFG_TYPE_T eAVCCfgType, INT32 i4Parameter);


/**Setup virtural surround parameters
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eVSCfgType                 Parameter type
*@param u4Parameter                Parameter value
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T _MTAUD_SetVirtualSurroundCfg(UINT8 u1DecId,
        MTAUDDEC_VSURR_CFG_TYPE_T eVSCfgType, UINT32 u4Parameter);



/**Setup bass enhancer switch
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetBassEnhancer (UINT8 u1DecId, BOOL fgEnable);

/**Setup bass enhancer parameers
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eBassEnhancerType          Parameter type
*@param u4Parameter                Parameter value
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T _MTAUD_SetBassEnhancerCfg(UINT8 u1DecId,
        MTAUDDEC_BE_PARAM_TYPE_T eBassEnhancerType, UINT32 u4Parameter);


/**Setup PEQ switch on/off
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   Enable
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T _MTAUD_SetPEQ(UINT8 u1DecId, BOOL fgEnable);


/**Setup PEQ parameters
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1Set                      PEQ set (0 ~ 7)
*@param u4Frequency                Frequency
*@param eQValue                    Q
*@param i2Gain                     Gain (+-12dB) with 0.5dB/step
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T _MTAUD_SetPEQCfg(UINT8 u1DecId, UINT8 u1Set,
                                    UINT32 u4Frequency, MTAUDDEC_PEQ_Q_TYPE_T eQValue, INT16 i2Gain);

/**Setup Limiter switch on/off
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   Enable
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T _MTAUD_SetLimiter(UINT8 u1DecId, BOOL fgEnable);


/**Setup Limiter configuration
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1Mode                     limiter mode
*                                  (0: off, 1: adaptive mode, 2: fixed mode).
*@param u1Flag                     limiter flag (0: off, 1: on)
*                                  bit 0: vol
*                                  bit 1: vsurr
*                                  bit 2: sbass
*                                  bit 3: EQ
*                                  bit 4: reverb
*                                  bit 5: virtual bass
*@param  u4Thresh                  limiter threshold (0x0 ~ 0x7fffff)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T _MTAUD_SetLimiterCfg(UINT8 u1DecId, UINT8 u1Mode,
                                 UINT16 u2Flag, UINT32 u4Thresh);

/**Setup HPF Fc switch
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   On/Off
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T _MTAUD_SetHPF(UINT8 u1DecId, BOOL fgEnable);


/**Setup HPF Fc
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u2Fc                       Frequency (50 ~ 300 Hz)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T _MTAUD_SetHPFFc(UINT8 u1DecId, UINT16 u2Fc);

#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
/**Mixound Play
*@param u4addr
*@param u4len
*@param u4gain                     0x20000 means 0dB
*@param u2config                   bit  0:3  samplerate  0: 48k   1: 44.1k 2: 32k 3: 24k 4:16k 5:12k
*                                            bit  4: source mode : 0->mono, 1->stereo
*                                            bit  5: 0: 8bit, 1 16bits
*                                            bit  6: 0: bigendian  1:littleendian
*                                            bit  7: 0: no repeat   1:repeat
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T _MTAUD_MixSoundPlay(UINT32 u4addr, UINT32 u4len, UINT32 u4gain, UINT16 u2config);

/**Mixound Stop
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T _MTAUD_MixSoundStop(void);

/**Mixound Pause
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T _MTAUD_MixSoundPause(void);

/**Mixound Resume
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T _MTAUD_MixSoundResume(void);

#endif

/**set DSP speed
*@param u4Mode                   step is 0.1, 10: normal speed; 5: 0.5X speed ; 20: 2X speed
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T _MTAUD_SetDSPSpeedMode(UINT32 u4Mode );

EXTERN MT_RESULT_T _MTAUD_RecordPCM (UINT32 u4ClipBufferPointer, UINT32 u4BufferSize);
EXTERN MT_RESULT_T _MTAUD_DownloadDram (UINT32 u4Ptr, UINT32 u4Addr, UINT32 u4Size);
EXTERN MT_RESULT_T _MTAUD_UploadDram (UINT32 u4Ptr, UINT32 u4Addr, UINT32 u4Size);
/**Set the S/PDIF Serial Copying Management System (SCMS) output bits.
*@param c_bit                   bit2
*@param l_bit                    bit15
*@retval MTR_OK               Success.
*@retval MTR_NOT_OK       Fail.
*/
EXTERN MT_RESULT_T  _MTAUD_SetSCMS(BOOL c_bit,BOOL l_bit);

#endif  //MT_AUDIO_H

