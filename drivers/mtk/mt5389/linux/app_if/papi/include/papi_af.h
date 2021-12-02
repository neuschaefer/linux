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
/* (c) 2008 Koninklijke Philips Electronics N.V., All rights reserved
 * 
 * This source code and any compilation or derivative thereof is the
 * proprietary information of Koninklijke Philips Electronics N.V. and is
 * confidential in nature.
 * Under no circumstances is this software to be exposed to or placed under an
 * Open Source License of any type without the expressed written permission of
 * Koninklijke Philips Electronics N.V.
 */

#include "papi_types.h"

/**
 *   @file papi_af.h
 */
/** @defgroup papi_af  */

/*
 *  interface aclip of type IPlfApiAudioClip
 */

/** @defgroup papi_af_aclip papi_af_aclip: AudioClip
  * @ingroup papi_af */

/** @defgroup papi_af_aclip_Functions Functions 
  * @ingroup papi_af_aclip */

/**  @ingroup papi_af_aclip_Functions */
/**  Hand over an audio clip data buffer to the platform for decoding. */ 
/**  @param id Buffer id with audio clip data. */
/**  @param size Size of the buffer. */
/**  @param Data Memory pointer to the data buffer. */
/**  @param endOfData True if this is the last data buffer for the audio clip, false if not. */
/**  @param retval Standard return value. */
extern FResult papi_af_aclip_AddData( Nat32 id, Nat32 size, Address Data, Bool endOfData, Nat32 * retval );

/**  @ingroup papi_af_aclip_Functions */
/**  Return the number of data buffers for audio clip playback that are still available. */ 
/**  @param AvailableNrBuffers Return value with the number of data buffers for audio clip playback that are still available. */
/**  @param retval Standard return value. */
extern FResult papi_af_aclip_GetAvailableNrBuffers( Nat32 * AvailableNrBuffers, Nat32 * retval );

/**  @ingroup papi_af_aclip_Functions */
/**  Enable/disable playback of an audio clip. */ 
/**  @param enable True to enable playback of an audio clip, false to disable it. */
/**  @param retval Standard return value. */
extern FResult papi_af_aclip_EnableClipPlay( Bool enable, Nat32 * retval );

/**  @ingroup papi_af_aclip_Functions */
/**  Return whether play back of an audio clip is enabled. */ 
/**  @param Enable Return value which indicates whether play back of an audio clip is enabled. */
/**  @param retval Standard return value. */
extern FResult papi_af_aclip_GetClipPlayEnabled( Bool * Enable, Nat32 * retval );

/**  @ingroup papi_af_aclip_Functions */
/**  Resume decoding of audio clip data. */ 
/**  @param retval Standard return value. */
extern FResult papi_af_aclip_Continue( Nat32 * retval );

/**  @ingroup papi_af_aclip_Functions */
/**  Return the maximum number of data buffers for audio clip playback. */ 
/**  @param MaxNrBuffers Return value with the the maximum number of data buffer for audio clip playback. */
/**  @param retval Standard return value. */
extern FResult papi_af_aclip_GetMaxNrBuffers( Nat32 * MaxNrBuffers, Nat32 * retval );

/**  @ingroup papi_af_aclip_Functions */
/**  Pause decoding of audio clip data. */ 
/**  @param retval Standard return value. */
extern FResult papi_af_aclip_Pause( Nat32 * retval );

/**  @ingroup papi_af_aclip_Functions */
/**  Return whether pausing of audio clip decoding is supported. */ 
/**  @param Supported True if pausing is supported, false if not. */
/**  @param retval Standard return value. */
extern FResult papi_af_aclip_GetPauseSupp( Bool * Supported, Nat32 * retval );

/**  @ingroup papi_af_aclip_Functions */
/**  Start decoding of audio clip data buffers. */ 
/**  @param retval Standard return value. */
extern FResult papi_af_aclip_Start( Nat32 * retval );

/**  @ingroup papi_af_aclip_Functions */
/**  Return the current state for playing back an audio clip. */ 
/**  @param State Return value with the current state for playing back an audio clip. */
/**  @param retval Standard return value. */
extern FResult papi_af_aclip_GetState( tmDigAdec_ClipDecodingState_t * State, Nat32 * retval );

/**  @ingroup papi_af_aclip_Functions */
/**  Stop decoding of audio clip data buffers */ 
/**  @param retval Standard return value. */
extern FResult papi_af_aclip_Stop( Nat32 * retval );



/*
 *  interface hsoo of type IPlfApiSoundOutput
 */

/** @defgroup papi_af_hsoo papi_af_hsoo: SoundOutput
  * There are multiple instances of this interface:
- ssoo is for speaker
- hsoo is for headphone
.
It is not uncommon that the functionality on the headphone is limited, because of limitations in the platform. At least volume is expected to be supported on the headphone interface. Individual Supported() calls indicate which functionality is and is not supported.

  * @ingroup papi_af */

/** @defgroup papi_af_hsoo_Datatypes Datatypes 
  * @ingroup papi_af_hsoo */
/**
 *   @defgroup papi_af_hsoo_EqualizerBand Equalizer Band
 *   @ingroup papi_af_hsoo_Datatypes
 *   These are the equalizer band definitions.
 *   @{ */
#define papi_af_hsoo_MinEqualizerBand 0
#define papi_af_hsoo_MaxEqualizerBand 4
/*   @} */

/**
 *   @defgroup papi_af_hsoo_Volume Volume
 *   @ingroup papi_af_hsoo_Datatypes
 *   These are the volume definitions.
 *   @{ */
#define papi_af_hsoo_MinVolume 0
#define papi_af_hsoo_MaxVolume 255
/*   @} */

/**
 *   @defgroup papi_af_hsoo_Treble Treble
 *   @ingroup papi_af_hsoo_Datatypes
 *   These are the treble definitions.
 *   @{ */
#define papi_af_hsoo_MinTreble 0
#define papi_af_hsoo_MedTreble 128
#define papi_af_hsoo_MaxTreble 255
/*   @} */

/**
 *   @defgroup papi_af_hsoo_Balance Balance
 *   @ingroup papi_af_hsoo_Datatypes
 *   These are the balance definitions.
 *   @{ */
#define papi_af_hsoo_MinBalance -255
#define papi_af_hsoo_MedBalance 0
#define papi_af_hsoo_MaxBalance 255
/*   @} */

/**
 *   @defgroup papi_af_hsoo_DeltaVolume Delta Volume
 *   @ingroup papi_af_hsoo_Datatypes
 *   These are the delta volume definitions.
 *   @{ */
#define papi_af_hsoo_MinDeltaVolume -255
#define papi_af_hsoo_MedDeltaVolume 0
#define papi_af_hsoo_MaxDeltaVolume 255
/*   @} */

/**
 *   @defgroup papi_af_hsoo_Bass Bass
 *   @ingroup papi_af_hsoo_Datatypes
 *   These are the bass definitions.
 *   @{ */
#define papi_af_hsoo_MinBass 0
#define papi_af_hsoo_MedBass 128
#define papi_af_hsoo_MaxBass 255
/*   @} */

/** @defgroup papi_af_hsoo_Functions Functions 
  * @ingroup papi_af_hsoo */

/**  @ingroup papi_af_hsoo_Functions */
/**  Get the current balance. */ 
/**  @param retval Return value with the current balance. */
extern FResult papi_af_hsoo_GetBalance( int * retval );

/**  @ingroup papi_af_hsoo_Functions */
/**  Set the current balance. */ 
/**  @param balance Balance to be set. */
extern FResult papi_af_hsoo_SetBalance( int balance );

/**  @ingroup papi_af_hsoo_Functions */
/**  Return if balance is supported. */ 
/**  @param retval True, if balance is supported. False, if not. */
extern FResult papi_af_hsoo_BalanceSupported( Bool * retval );

/**  @ingroup papi_af_hsoo_Functions */
/**  Get bass. */ 
/**  @param retval Return value with the current bass. */
extern FResult papi_af_hsoo_GetBass( int * retval );

/**  @ingroup papi_af_hsoo_Functions */
/**  Set bass. */ 
/**  @param bass Bass to be set. */
extern FResult papi_af_hsoo_SetBass( int bass );

/**  @ingroup papi_af_hsoo_Functions */
/**  Return whether bass is supported. */ 
/**  @param retval True, if bass is supported. False, if not. */
extern FResult papi_af_hsoo_BassSupported( Bool * retval );

/**  @ingroup papi_af_hsoo_Functions */
/**  Get the current delta volume. */ 
/**  @param retval Return value with the current delta volume. */
extern FResult papi_af_hsoo_GetDeltaVolume( int * retval );

/**  @ingroup papi_af_hsoo_Functions */
/**  Set the current delta volume. */ 
/**  @param delta Delta volume to be set. */
extern FResult papi_af_hsoo_SetDeltaVolume( int delta );

/**  @ingroup papi_af_hsoo_Functions */
/**  Return whether delta volume is supported. */ 
/**  @param retval True, if delta volume is supported. False, if not. */
extern FResult papi_af_hsoo_DeltaVolumeSupported( Bool * retval );

/**  @ingroup papi_af_hsoo_Functions */
/**  Get equalizer value for given band. */ 
/**  @param band Equalizer band. */
/**  @param retval Return value indicating the equalizer value. */
extern FResult papi_af_hsoo_GetEqualizerBandValue( int band, int * retval );

/**  @ingroup papi_af_hsoo_Functions */
/**  Set equalizer value for given band. */ 
/**  @param band Band to be set. */
/**  @param value Equalizer value to be set. */
extern FResult papi_af_hsoo_SetEqualizerBandValue( int band, int value );

/**  @ingroup papi_af_hsoo_Functions */
/**  Return whether equalizer is supported. */ 
/**  @param retval True, if equalizer is supported. False, if not. */
extern FResult papi_af_hsoo_EqualizerSupported( Bool * retval );

/**  @ingroup papi_af_hsoo_Functions */
/**  Get equalizer maximum value for given band. */ 
/**  @param band Equalizer band. */
/**  @param retval Return value indicating the equalizer value. */
extern FResult papi_af_hsoo_MaxEqualizerBandValue( int band, int * retval );

/**  @ingroup papi_af_hsoo_Functions */
/**  Get equalizer medium value for given band. */ 
/**  @param band Equalizer band. */
/**  @param retval Return value indicating the equalizer value. */
extern FResult papi_af_hsoo_MedEqualizerBandValue( int band, int * retval );

/**  @ingroup papi_af_hsoo_Functions */
/**  Get equalizer minimum value for given band. */ 
/**  @param band Equalizer band. */
/**  @param retval Return value indicating the equalizer value. */
extern FResult papi_af_hsoo_MinEqualizerBandValue( int band, int * retval );

/**  @ingroup papi_af_hsoo_Functions */
/**  Enable/disable auto mute. */ 
/**  @param enable True, to enable auto mute. False, to disable it. */
extern FResult papi_af_hsoo_EnableMute( Bool enable );

/**  @ingroup papi_af_hsoo_Functions */
/**  Get mute. */ 
/**  @param retval Return value with the current the mute. */
extern FResult papi_af_hsoo_GetMute( Bool * retval );

/**  @ingroup papi_af_hsoo_Functions */
/**  Set Mute. */ 
/**  @param mute Mute to be set. */
extern FResult papi_af_hsoo_SetMute( Bool mute );

/**  @ingroup papi_af_hsoo_Functions */
/**  Returns whether mute is supported. */ 
/**  @param retval True, if mute is supported. False, if not. */
extern FResult papi_af_hsoo_MuteSupported( Bool * retval );

/**  @ingroup papi_af_hsoo_Functions */
/**  Get treble. */ 
/**  @param retval Return value with the current treble value. */
extern FResult papi_af_hsoo_GetTreble( int * retval );

/**  @ingroup papi_af_hsoo_Functions */
/**  Set treble. */ 
/**  @param treble Treble to be set. */
extern FResult papi_af_hsoo_SetTreble( int treble );

/**  @ingroup papi_af_hsoo_Functions */
/**  Return whether Treble is supported. */ 
/**  @param retval True, if treble is supported. False, if not. */
extern FResult papi_af_hsoo_TrebleSupported( Bool * retval );

/**  @ingroup papi_af_hsoo_Functions */
/**  Get the current volume. */ 
/**  @param retval Return value with the current volume. */
extern FResult papi_af_hsoo_GetVolume( int * retval );

/**  @ingroup papi_af_hsoo_Functions */
/**  Set the current volume. */ 
/**  @param volume Volume to be set. */
extern FResult papi_af_hsoo_SetVolume( int volume );

/**  @ingroup papi_af_hsoo_Functions */
/**  Return whether volume is supported. */ 
/**  @param retval True, if volume is supported. False, if not. */
extern FResult papi_af_hsoo_VolumeSupported( Bool * retval );



/*
 *  interface sad of type IPlfApiSoundAudioDescription
 */

/** @defgroup papi_af_sad papi_af_sad: SoundAudioDescription
  * Audio description (A.D.) is an ancillary component associated with a TV service, which delivers a verbal description of the visual scene as an aid to understanding and enjoyment particularly, but not exclusively, for viewers who have visual impairments.<br>
The IPlfApiSoundAudioDescription interface can be used to control the A.D. routing for the main speakers, for the headphones, for the monitor output (variable level output) and for the SPDIF/SCART (fixed level) output. Each of those outputs can contain either the main audio steam, or a mix of the main audio stream and the A.D. stream.<br>
This interface can also be used to control the volume of the A.D. stream. The main audio stream is not affected by this setting.<br>
Notice that in order to actually render the A.D. stream, it is needed that the A.D. stream is setup. For this it is needed to allocate and start a PID filter for the A.D. stream.

  * @ingroup papi_af */

/** @defgroup papi_af_sad_Datatypes Datatypes 
  * @ingroup papi_af_sad */
/** @defgroup papi_af_sad_Functions Functions 
  * @ingroup papi_af_sad */

/**  @ingroup papi_af_sad_Functions */
/**  Get the output routing for main-speakers, headphone, monitor-out and spdif-out. */ 
/**  @param retval Returns on which outputs the Audio Description stream will be mixed with the main audio stream. A logical AND operation with the constants MainSpeakers, HeadPhones, MonitorOut and SpdifOut can be done to obtain the result for a specific output. See also SetADRouting. */
extern FResult papi_af_sad_GetADRouting( int * retval );

/**  @ingroup papi_af_sad_Functions */
/**  Set the output routing for main-speakers, headphone, monitor-out and spdif-out. */ 
/**  @param outputs The parameter outputs specifies on which outputs the Audio Description stream is to be mixed with the main audio stream. For this purpose, zero or more of the constants MainSpeakers, HeadPhones, MonitorOut (variable output level) and SpdifOut (comprises SPDIF, SCART and line-out (fixed output level)) can be OR-ed. The included outputs (constants) will have the Audio Description (once it is available), whereas the outputs that are not included will not have the Audio Description. In the case that routing for Audio Descriptor is no longer needed, then the constant NoADRouting shall be used as a parameter */
extern FResult papi_af_sad_SetADRouting( int outputs );

/**  @ingroup papi_af_sad_Functions */
/**  Determines whether Audio Description routing is allowed within the current use-case. */ 
/**  @param retval Returns TRUE in the case that Audio Description routing is allowed within the current use-case. FALSE is returned in case Audio Description routing is not allowed within the current use-case */
extern FResult papi_af_sad_IsADRoutingAllowed( Bool * retval );

/**  @ingroup papi_af_sad_Functions */
/**  Determines whether Audio Description routing is allowed in the platform. */ 
/**  @param retval Returns TRUE in the case that Audio Description routing is allowed within the current platform instance. FALSE is returned in case Audio Description routing is not allowed within the current platform instance */
extern FResult papi_af_sad_IsADRoutingSupported( Bool * retval );

/**  @ingroup papi_af_sad_Functions */
/**  Determines whether Audio Description (user) volume control is allowed within the current use-case. */ 
/**  @param retval Returns TRUE in the case that Audio Description (user) volume control is allowed within the current use case. FALSE is returned in case Audio Description (user) volume control is not allowed within the current use-case */
extern FResult papi_af_sad_IsVolumeControlAllowed( Bool * retval );

/**  @ingroup papi_af_sad_Functions */
/**  Determines whether Audio Description (user) volume control is allowed within the current platform instance. */ 
/**  @param retval Returns TRUE in the case that Audio Description (user) volume control is allowed within the current platform instance. FALSE is returned in case Audio Description (user) volume control is not allowed within the current platform instance */
extern FResult papi_af_sad_IsVolumeControlSupported( Bool * retval );

/**  @ingroup papi_af_sad_Functions */
/**  Gets the actual volume level of the Audio Description stream. */ 
/**  @param retval The actual volume level */
extern FResult papi_af_sad_GetVolume( int * retval );

/**  @ingroup papi_af_sad_Functions */
/**  Returns the maximal allowed volume. Any following SetVolume call should be smaller then this value */ 
/**  @param retval The maximum volume */
extern FResult papi_af_sad_GetVolumeMax( int * retval );

/**  @ingroup papi_af_sad_Functions */
/**  Returns the minimal allowed volume. Any following SetVolume call should be greater then this value */ 
/**  @param retval The minimum volume */
extern FResult papi_af_sad_GetVolumeMin( int * retval );

/**  @ingroup papi_af_sad_Functions */
/**  Sets the volume level of the Audio Description stream.<br>
<ul>
<li>	Notice that this volume setting affects the volume of the Audio Description stream only. This volume setting is applied on the Audio Description stream before being mixed with the main audio stream. "Normal" volume control of the various outputs (such as "main" and "headphone") is done after mixing.</li>
<li>	Notice that this volume setting is linear.</li>
<li>	This function shall only be called in the context of a use-case where the IsVolumeControlAllowed API function would return TRUE</li>
</ul>
 */ 
/**  @param retval The parameter volume specifies the volume level of the Audio Description stream before being mixed with the main audio stream. The value of the volume parameter must be in between GetVolumeMin and GetVolumeMax */
extern FResult papi_af_sad_SetVolume( int volume );

#define papi_af_sad_NoADRouting 0
#define papi_af_sad_MainSpeakers 1
#define papi_af_sad_HeadPhones 2
#define papi_af_sad_MonitorOut 4
#define papi_af_sad_SpdifOut 8


/*
 *  interface sip of type IPlfApiSoundImprovement
 */

/** @defgroup papi_af_sip papi_af_sip: SoundImprovement
  * The human ear listening curves (Fletcher-Munson loudness contours) show, that the ears of a human are less sensitive for low and high frequencies at low sound pressure level (volume level). This interface provides control over the available features that can improve volume impression or the "sound colour", for example by embroidering the low and or high frequencies. <br>
<b>Loudness</b><br>
In general a loudness function can be used to compensate the human ear sensitivity loss at low volume levels. At the lower volume range the loudness gain - e.g. low and high frequency gain levels - varies with the total gain value of the volume stage.  At high volume the resulting loudness curve is flat, because there is no need to boost high and low frequencies at a high sound pressure level.<br>
<br><b>Sound improvement modes</b><br>
Next to loudness feature other sound improvement features exist. They maybe using similar but different and sometimes more advanced algorithms, depending on the kind of hardware or vendor supplying the algorithm.<br>
<ul>
<li><b> Dynamic Bass Enhancement (DBE)</b><br>
This feature produces a level depending bass boost. The dynamic behaviour allows a strong bass amplification for small volume signals, but only small bass amplification for high volume signals.</li>
<li><b>(Dynamic) Ultra Bass (DUB, UB)</b><br>
This feature gives the impression of deep bass reproduction although the fundamentals are missing, caused by producing harmonics of the low frequency content.</li>
<li><b>Barcus Berry Electronics (BBE)</b>
The BBE system addresses the problem of loss of fidelity - or subtle distortions - caused by the inherent characteristics of the loudspeaker by compensating for phase and amplitude distortions. Varying either the amplitude or the phase of the transients and harmonics within signal causes the desired distortion of the sound's characteristics needed to compensate for the flaws in the speaker characteristics.</li>
<li><b>Micronas Perfect Bass (MPB, MB)</b>
The Micronas MPB feature is a combination of dynamic bass enhancement and adding harmonics, exploiting the psychoacoustic phenomenon of the 'missing fundamental'.</li>
</ul>
<br>Combinations of sound modes are not always supported.<br>
<br>This interface supplies functions to check which features (and combination of features) are supported. Autonomous changes in availability of these features can be checked with 'allowed' functions and are notified as such.<br>

  * @ingroup papi_af */

/** @defgroup papi_af_sip_Datatypes Datatypes 
  * @ingroup papi_af_sip */
/**
 *   @defgroup papi_af_sip_Improvement Sound Improvement
 *   @ingroup papi_af_sip_Datatypes
 *   Sound improvement modes.
 *   @{ */
#define papi_af_sip_ImprovementNone 0
#define papi_af_sip_ImprovementDbe 1
#define papi_af_sip_ImprovementUltraBass 2
#define papi_af_sip_ImprovementBbe 4
#define papi_af_sip_ImprovementMpb 8
/*   @} */

/** @defgroup papi_af_sip_Functions Functions 
  * @ingroup papi_af_sip */

/**  @ingroup papi_af_sip_Functions */
/**  Return the current sound improvement mode. */ 
/**  @param retval The sound improvement mode. See datatype \ref papi_af_sip_Improvement. */
extern FResult papi_af_sip_GetImprovementMode( int * retval );

/**  @ingroup papi_af_sip_Functions */
/**  Set the current sound improvement mode. */ 
/**  @param improvement The sound improvement mode. See datatype \ref papi_af_sip_Improvement. */
extern FResult papi_af_sip_SetImprovementMode( int improvement );

/**  @ingroup papi_af_sip_Functions */
/**  Return if the specified sound improvement mode is allowed. */ 
/**  @param improvement The sound improvement mode. See datatype \ref papi_af_sip_Improvement. */
/**  @param retval True if the sound improvement mode is allowed, false if not. */
extern FResult papi_af_sip_IsImprovementModeAllowed( int improvement, Bool * retval );

/**  @ingroup papi_af_sip_Functions */
/**  Generic function to detect if a specific sound improvement mode is supported. */ 
/**  @param improvement The sound improvement mode. See datatype \ref papi_af_sip_Improvement. */
/**  @param retval True if the sound improvement mode is supported, false if not. */
extern FResult papi_af_sip_IsImprovementModeSupported( int improvement, Bool * retval );

/**  @ingroup papi_af_sip_Functions */
/**  Return if loudness is allowed. */ 
/**  @param retval True if loudness is allowed, false if not. */
extern FResult papi_af_sip_IsLoudnessAllowed( Bool * retval );

/**  @ingroup papi_af_sip_Functions */
/**  Return if loudness is supported. */ 
/**  @param retval True if loudness is supported, false if not. */
extern FResult papi_af_sip_IsLoudnessSupported( Bool * retval );

/**  @ingroup papi_af_sip_Functions */
/**  Return whether loudness is set. */ 
/**  @param retval True if loudness is set, false if it is switched off. */
extern FResult papi_af_sip_GetLoudness( Bool * retval );

/**  @ingroup papi_af_sip_Functions */
/**  Set loudness. */ 
/**  @param loudness True to set loudness, false to switch off loudness. */
extern FResult papi_af_sip_SetLoudness( Bool loudness );



/*
 *  interface sla of type IPlfApiSoundLevelAdjust
 */

/** @defgroup papi_af_sla papi_af_sla: SoundLevelAdjust
  * <b>Concepts</b><br><br>
<b>AVL</b><br>
The Automatic Volume Level (AVL) feature is a feature that constantly measures the input signal level and dynamically corrects the signal level (attenuation) to keep the output signal on a predefined average level. This avoids annoying volume jumps whenever the broadcaster changes to a new program, e.g. from a movie to commercials and back.<br>
The AVL feature may need to be switched off in certain cases where the feature conflicts with other features in the sound signal path. In case the AVL is autonomously switched on or off a notification can be expected (see notification interface).<br>
<br><b>DRC</b><br>
The Dynamic Range Compression (DRC) is a typical feature within digital audio streams (e.g. AC-3). Such digital data streams can have additional DRC information encoded that can be used to adapt the sound output level in a predefined way.<br>
The DRC feature may need to be switched off in certain cases, e.g. when no DRC information is encoded in the current audio data stream. In case DRC is autonomously switched on or off a notification can be expected (see notification interface).<br>
The main difference between the DRC and AVL is in the relation with the actual signal level. DRC adapts based on additional information that is provided about the actual signal while AVL adapts based on the measurements concerning the actual signal level.<br>
<br><b>Night mode compression</b><br>
Night mode compression compresses the dynamic range of a signal in a similar way as DRC, but does not use meta-data. It lowers high levels, and it raises low levels (AVL only lowers high levels). Night mode compression must not be used simultaneously with AVL or DRC.<br><br><br>
About user preferences and actual settings<br><br>
The Set functions on this interface set a user preference, which the platform does not change autonomously. The actual settings (which are returned by the Get functions on this interface) determine whether a feature is currently applied to the signal. The actual settings can change autonomously. The platform implementation determines the actual settings based on the user preferences, taking the following into account:<br>
<ul>
<li>	The platform may only apply a feature (like AVL) to the signal, if the user preference indicates it; The platform will not always match the actual setting to the user preference though (see bullets below)</li>
<li>	The platform will apply at most one of the DRC, AVL and night mode compression features at any given moment.</li>
<li>	DRC can only be applied to a signal if the signal carries meta-data for it;</li>
<li>	If DRC can be applied to a signal (and the user preference wants it enabled), it takes precedence over both AVL and night mode compression.</li>
<li>	The user may not indicate a preference for AVL and night mode compression at the same time.</li>
<li>	The platform can have platform-specific constraints. One such constraint may be that AVL may not be active when Dolby Pro Logic decoding is active. A platform-specific document should describe such constraints, if they exist.</li>
</ul>

  * @ingroup papi_af */

/** @defgroup papi_af_sla_Datatypes Datatypes 
  * @ingroup papi_af_sla */
/**
 *   @defgroup papi_af_sla_Drc Drc Modes
 *   @ingroup papi_af_sla_Datatypes
 *   Dynamic range compression modes.
 *   @{ */
#define papi_af_sla_DrcModeUnknown 0
#define papi_af_sla_DrcModeDynamic 1
#define papi_af_sla_DrcModeStandard 2
#define papi_af_sla_DrcModeCompressed 3
/*   @} */

/** @defgroup papi_af_sla_Functions Functions 
  * @ingroup papi_af_sla */

/**  @ingroup papi_af_sla_Functions */
/**  Switch auto volume leveling on and off. */ 
/**  @param avl True, to switch auto volume leveling on, false to switch off. */
extern FResult papi_af_sla_SetAutoVolumeLevel( Bool avl );

/**  @ingroup papi_af_sla_Functions */
/**  Return the current dynamic range compression mode. */ 
/**  @param retval Return value with the current dynamic range compression mode. See datatype \ref papi_af_sla_Drc. */
extern FResult papi_af_sla_GetDynamicRangeCompression( int * retval );

/**  @ingroup papi_af_sla_Functions */
/**  Set the dynamic range compression mode. */ 
/**  @param drc Specific dynamic range compression mode. See datatype \ref papi_af_sla_Drc. */
extern FResult papi_af_sla_SetDynamicRangeCompression( int drc );

/**  @ingroup papi_af_sla_Functions */
/**  Return if auto volume leveling is allowed. */ 
/**  @param retval True, if auto volume leveling is allowed, false if not. */
extern FResult papi_af_sla_IsAutoVolumeLevelAllowed( Bool * retval );

/**  @ingroup papi_af_sla_Functions */
/**  Return if auto volume leveling is supported. */ 
/**  @param retval True, if auto volume leveling is supported, false if not. */
extern FResult papi_af_sla_IsAutoVolumeLevelSupported( Bool * retval );

/**  @ingroup papi_af_sla_Functions */
/**  Return if specified dynamic range compression mode is allowed. */ 
/**  @param drc Specific dynamic range compression mode. See datatype \ref papi_af_sla_Drc. */
/**  @param retval True, if the dynamic range compression mode is allowed, false if not. */
extern FResult papi_af_sla_IsDynamicRangeCompressionAllowed( int drc, Bool * retval );

/**  @ingroup papi_af_sla_Functions */
/**  Return if dynamic range compression (in a digital audio stream like AC3) is supported. */ 
/**  @param drc Specific digital range compression mode. See datatype \ref papi_af_sla_Drc. */
/**  @param retval Return value indicating whether the specified dynamic range compression mode is supported. */
extern FResult papi_af_sla_IsDynamicRangeCompressionSupported( int drc, Bool * retval );

/**  @ingroup papi_af_sla_Functions */
/**  Return if night mode compression is allowed. */ 
/**  @param retval True, if night mode compression is allowed, false if not. */
extern FResult papi_af_sla_IsNightModeAllowed( Bool * retval );

/**  @ingroup papi_af_sla_Functions */
/**  Return if night mode compression is supported. Night mode compression compresses the dynamic range of a signal in a similar way as dynamic range compression, but does not use meta-data. It lowers high levels, and it raises low levels (automatic volume leveling only lowers high levels). Night mode compression must not be used simultaneously with automatic volume leveling or dynamic range compression. */ 
/**  @param retval True if Night Mode Compression is supported, false if not. */
extern FResult papi_af_sla_IsNightModeSupported( Bool * retval );

/**  @ingroup papi_af_sla_Functions */
/**  Return whether night mode compression is currently switched on or off. */ 
/**  @param retval True, if night mode compression is currently switched on, false if it is currently switched off. */
extern FResult papi_af_sla_GetNightMode( Bool * retval );

/**  @ingroup papi_af_sla_Functions */
/**  Switch night mode compression on and off. */ 
/**  @param nmc True, to switch night mode compression on, false to switch off. */
extern FResult papi_af_sla_SetNightMode( Bool nmc );

/**  @ingroup papi_af_sla_Functions */
/**  Return whether auto volume leveling is switch on or off. */ 
/**  @param retval True, if auto volume leveling is switched on, false if it is switched off. */
extern FResult papi_af_sla_GetAutoVolumeLevel( Bool * retval );



/*
 *  interface smt of type IPlfApiSoundMute
 */

/** @defgroup papi_af_smt papi_af_smt: SoundMute
  * @ingroup papi_af */

/** @defgroup papi_af_smt_Datatypes Datatypes 
  * @ingroup papi_af_smt */
/**
 *   @defgroup papi_af_smt_SoundOutput Sound Output
 *   @ingroup papi_af_smt_Datatypes
 *   Sound outputs.
 *   @{ */
#define papi_af_smt_None 0
#define papi_af_smt_Headphone 1
#define papi_af_smt_Speaker 2
#define papi_af_smt_Monitor 4
#define papi_af_smt_Spdif 8
#define papi_af_smt_Line1 16
#define papi_af_smt_Line2 32
#define papi_af_smt_All 63
/*   @} */

/** @defgroup papi_af_smt_Functions Functions 
  * @ingroup papi_af_smt */

/**  @ingroup papi_af_smt_Functions */
/**  Mute the sound outputs that are specified. */ 
/**  @param outputs Parameters with the different sound outputs OR-ed. See datatype \ref papi_af_smt_SoundOutput. */
extern FResult papi_af_smt_MuteOutputs( int outputs );

/**  @ingroup papi_af_smt_Functions */
/**  Return the mute status of the different sound outputs. */ 
/**  @param retval Return value with the different sound outputs OR-ed. See datatype \ref papi_af_smt_SoundOutput. */
extern FResult papi_af_smt_GetActualMuteStatus( int * retval );



/*
 *  interface som of type IPlfApiSurroundMode
 */

/** @defgroup papi_af_som papi_af_som: SurroundMode
  * Setting a surround mode can be defined as selecting a specific surround algorithm. It is able to either generate additional (surround) sound signals or reduce the number of sound signals (via virtualising algorithms).
The surround modes can be roughly divided into 6 levels:<br>
<ul>
<li>	Off: No surround processing (Off).</li>
<li>	Enhanced: Stereo widening (Spatial, Incredible Mono, Incredible Stereo, Incredible Surround, SrsWow, Srs3DSound).</li>
<li>	Front 3: Centre signal separation, providing left, right and centre signals (Front 3 Stereo).</li>
<li>	Virtual: Virtualising the surround signal(s) into the front signals left, right and/or centre: Incredible 3D Surround, Digital3D, VirtualDolby422, VirtualDolby423, VirtualDolby521, VirtualDolby522, VirtualDolbyDigital, SrsTruSurround, SrsTruSurroundWithDPLII, SrsTruSurroundXT, SrsTruSurroundXTWithDPLII.</li>
<li>	Hall: Surround signal generation, providing left, right, and surround signals (Hall, Stereo4).</li>
<li>	Maximum: Providing separate left, right, centre, and surround signals (Matrix, Dolby Pro Logic, Dolby Pro Logic Phantom, Dolby Pro Logic II, Dolby Digital, or MPEG MultiChannel).</li>
</ul><br><br>
Dolby Digital and MPEG MultiChannel signals can contain various numbers of sound channels, for Dolby Digital there are specific Dolby requirements  specifying the allowed surround modes for each sound channel configuration. Normally MPEG MultiChannel signals are treated the same way.<br>
Selection of a surround mode does not always result in sound output on all sound signals related to this surround mode. An individual sound output will produce an output signal only in case the input signal contains a properly encoded sound signal for this output.
  * @ingroup papi_af */

/** @defgroup papi_af_som_Datatypes Datatypes 
  * @ingroup papi_af_som */
/**
 *   @defgroup papi_af_som_SurroundLevel Surround Level
 *   @ingroup papi_af_som_Datatypes
 *   Surround Level Definitions.
 *   @{ */
#define papi_af_som_CurrentLevel 0
#define papi_af_som_LevelOff 1
#define papi_af_som_LevelEnhanced 2
#define papi_af_som_LevelFront3 3
#define papi_af_som_LevelVirtual 4
#define papi_af_som_LevelHall 5
#define papi_af_som_LevelMaximum 6

#define papi_af_som_NumOfLevel 7
/*   @} */

/**
 *   @defgroup papi_af_som_SurroundMode Surround Mode
 *   @ingroup papi_af_som_Datatypes
 *   Surround Mode Definitions.
 *   @{ */
#define papi_af_som_CurrentMode 0
#define papi_af_som_Off 1
#define papi_af_som_Spatial 2
#define papi_af_som_Hall 3
#define papi_af_som_IncredibleSurround 4
#define papi_af_som_SrsWow 5
#define papi_af_som_Srs3DSound 6
#define papi_af_som_Incredible3DSurround 7
#define papi_af_som_Front3Stereo 8
#define papi_af_som_Stereo4 9
#define papi_af_som_DolbyProLogic 10
#define papi_af_som_IncredibleMono 11
#define papi_af_som_IncredibleStereo 12
#define papi_af_som_VirtualDolby422 13
#define papi_af_som_VirtualDolby423 14
#define papi_af_som_VirtualDolby521 15
#define papi_af_som_VirtualDolby522 16
#define papi_af_som_SrsTruSurround 17
#define papi_af_som_SrsTruSurroundWithDPLII 18
#define papi_af_som_SrsTruSurroundXT 19
#define papi_af_som_SrsTruSurroundXTWithDPLII 20
#define papi_af_som_Matrix 21
#define papi_af_som_DolbyDigital 22
#define papi_af_som_MPEGMultiChannel 23
#define papi_af_som_DolbyProLogicII 24
#define papi_af_som_VirtualDolbyDigital 25
#define papi_af_som_Digital3D 26
#define papi_af_som_DolbyProLogicPhantom 27

#define papi_af_som_NumOfMode 28
/*   @} */

/**
 *   @defgroup papi_af_som_Effect Effect Strength
 *   @ingroup papi_af_som_Datatypes
 *   Effect Strength Definitions.
 *   @{ */
#define papi_af_som_NoEffect 0
#define papi_af_som_MinEffectStrength 1
#define papi_af_som_MedEffectStrength 2
#define papi_af_som_MaxEffectStrength 3
/*   @} */

/**
 *   @defgroup papi_af_som_AutoSurround Auto Surround
 *   @ingroup papi_af_som_Datatypes
 *   Auto Surround Mode Definitions.
 *   @{ */
#define papi_af_som_useNone 0
#define papi_af_som_useWssDpl 1
#define papi_af_som_useP50Dpl 2
#define papi_af_som_useSPDIFDpl 4
#define papi_af_som_useSpeakerConfiguration 8
#define papi_af_som_useMonoStereo 16
/*   @} */

/** @defgroup papi_af_som_Functions Functions 
  * @ingroup papi_af_som */

/**  @ingroup papi_af_som_Functions */
/**  Returns if AutoSurroundMode is supported. */ 
/**  @param autosurroundmode Input parameter Auto Surround Mode. See datatype \ref papi_af_som_AutoSurround. */
/**  @param retval Returns if supported. */
extern FResult papi_af_som_AutoSurroundModeSupported( int autosurroundmode, int * retval );

/**  @ingroup papi_af_som_Functions */
/**  This function will take into account the information that the algorithm is allowed to react upon. The most appropriate surround mode is then selected based on the available information, e.g., when asked to react on WSS (useWssDpl) and the WSS bit is active, then Dolby Pro Logic will be selected when this surround mode is supported (in case Dolby Pro Logic is not supported 3Dsurround is selected).<br>
<ul>
<li>	It also takes into account the speaker location if the user has requested this (useSpeakerConfiguration). In this case Dolby Pro Logic will not be selected when there are no rear speakers or centre speaker, but the most enhanced and appropriate mode that is supported will be selected.</li>
<li>	The priority of the various kinds of information to select a surround mode is component specific (see component datasheet).</li>
<li>	This function should only be called with a value that remains unchanged by a call to AutoSurroundModeSupported(autosurroundmode).</li>
</ul> */ 
/**  @param autosurroundmode Input parameter Auto Surround Mode. See datatype \ref papi_af_som_AutoSurround. */
extern FResult papi_af_som_SetAutoSurround( int autosurroundmode );

/**  @ingroup papi_af_som_Functions */
/**  Gets the AutoSurroundMode. */ 
/**  @param retval Returns auto surround mode. See datatype \ref papi_af_som_AutoSurround. */
extern FResult papi_af_som_GetAutoSurround( int * retval );

/**  @ingroup papi_af_som_Functions */
/**  Returns if given SurroundLevel is allowed. */ 
/**  @param surroundlevel Input parameter Surround Level. See datatype \ref papi_af_som_SurroundLevel. */
/**  @param retval Returns if allowed. */
extern FResult papi_af_som_SurroundLevelAllowed( int surroundlevel, Bool * retval );

/**  @ingroup papi_af_som_Functions */
/**  Gets the Effect Strength for given Surround Level. */ 
/**  @param surroundlevel Input parameter surround level. See datatype \ref papi_af_som_SurroundLevel. */
/**  @param retval Returns surround level effect strength. See datatype \ref papi_af_som_Effect. */
extern FResult papi_af_som_GetSurroundLevelEffectStrength( int surroundlevel, int * retval );

/**  @ingroup papi_af_som_Functions */
/**  Sets the Effect Strength for given Surround Level. */ 
/**  @param strength Input parameter surround effect strength. See datatype \ref papi_af_som_Effect. */
/**  @param surroundlevel Input parameter surround level. See datatype \ref papi_af_som_SurroundLevel. */
extern FResult papi_af_som_SetSurroundLevelEffectStrength( int strength, int surroundlevel );

/**  @ingroup papi_af_som_Functions */
/**  Returns if given SurroundMode supports Effect Strength. */ 
/**  @param surroundlevel Input parameter Surround Level. See datatype \ref papi_af_som_SurroundLevel. */
/**  @param retval Returns if supported. */
extern FResult papi_af_som_SurroundLevelEffectStrengthSupported( int surroundlevel, Bool * retval );

/**  @ingroup papi_af_som_Functions */
/**  This function returns the surround level in which the specified surround mode resides. */ 
/**  @param surroundmode Input parameter Surround Mode. See datatype \ref papi_af_som_SurroundMode. */
/**  @param retval Returns surround level. See datatype \ref papi_af_som_SurroundLevel. */
extern FResult papi_af_som_GetSurroundLevel( int surroundmode, int * retval );

/**  @ingroup papi_af_som_Functions */
/**  Sets the SurroundLevel. */ 
/**  @param surroundlevel Input parameter Surround Level. See datatype \ref papi_af_som_SurroundLevel. */
extern FResult papi_af_som_SetSurroundLevel( int surroundlevel );

/**  @ingroup papi_af_som_Functions */
/**  Returns if given SurroundLevel is supported. */ 
/**  @param surroundlevel Input parameter Surround Level. See datatype \ref papi_af_som_SurroundLevel. */
/**  @param retval Returns if supported. */
extern FResult papi_af_som_SurroundLevelSupported( int surroundlevel, Bool * retval );

/**  @ingroup papi_af_som_Functions */
/**  Returns if given SurroundMode is allowed. */ 
/**  @param surroundmode Input parameter Surround Mode. See datatype \ref papi_af_som_SurroundMode. */
/**  @param retval Returns if allowed. */
extern FResult papi_af_som_SurroundModeAllowed( int surroundmode, Bool * retval );

/**  @ingroup papi_af_som_Functions */
/**  Gets the Effect Strength for given SurroundMode. */ 
/**  @param surroundmode Input parameter surround mode. See datatype \ref papi_af_som_SurroundMode. */
/**  @param retval Returns surround mode effect strength. See datatype \ref papi_af_som_Effect. */
extern FResult papi_af_som_GetSurroundModeEffectStrength( int surroundmode, int * retval );

/**  @ingroup papi_af_som_Functions */
/**  Sets the Effect Strength for given SurroundMode. Not all surround modes support effect strength. See SurroundModeEffectStrengthSupported() for more information.<br>
<ul>
<li> The strength can be NoEffect or varied from MinEffectStrength to MaxEffectStrength.
<li> Any out of range value will be clipped.
</ul><br>
 */ 
/**  @param strength Input parameter surround effect strength. See datatype \ref papi_af_som_Effect. */
/**  @param surroundmode Input parameter surround mode. See datatype \ref papi_af_som_SurroundMode. */
extern FResult papi_af_som_SetSurroundModeEffectStrength( int strength, int surroundmode );

/**  @ingroup papi_af_som_Functions */
/**  Returns if Effect Strength is supported with given SurroundMode. */ 
/**  @param surroundmode Input parameter Surround Mode. See datatype \ref papi_af_som_SurroundMode. */
/**  @param retval Returns if supported. */
extern FResult papi_af_som_SurroundModeEffectStrengthSupported( int surroundmode, Bool * retval );

/**  @ingroup papi_af_som_Functions */
/**  This function returns the surround mode that would be selected if the reproduction was being done in the level described by the surroundlevel parameter. */ 
/**  @param surroundlevel Input parameter Surround Level. See datatype \ref papi_af_som_SurroundLevel. */
/**  @param retval Returns surround mode. See datatype \ref papi_af_som_SurroundMode. */
extern FResult papi_af_som_GetSurroundMode( int surroundlevel, int * retval );

/**  @ingroup papi_af_som_Functions */
/**  Sets the current SurroundMode. */ 
/**  @param surroundmode Input parameter Surround Mode. See datatype \ref papi_af_som_SurroundMode. */
extern FResult papi_af_som_SetSurroundMode( int surroundmode );

/**  @ingroup papi_af_som_Functions */
/**  Returns if given SurroundMode is supported. */ 
/**  @param surroundmode Input parameter Surround Mode. See datatype \ref papi_af_som_SurroundMode. */
/**  @param retval Returns if supported. */
extern FResult papi_af_som_SurroundModeSupported( int surroundmode, Bool * retval );



/*
 *  interface ssoc of type IPlfApiSoundChannel
 */

/** @defgroup papi_af_ssoc papi_af_ssoc: SoundChannel
  * @ingroup papi_af */

/** @defgroup papi_af_ssoc_Datatypes Datatypes 
  * @ingroup papi_af_ssoc */
/**
 *   @defgroup papi_af_ssoc_Channel Sound Channels
 *   @ingroup papi_af_ssoc_Datatypes
 *   These are the Sound Channel definitions.
 *   @{ */
#define papi_af_ssoc_Channel1 1
#define papi_af_ssoc_Channel2 2
#define papi_af_ssoc_Channel3 3
/*   @} */

/** @defgroup papi_af_ssoc_Functions Functions 
  * @ingroup papi_af_ssoc */

/**  @ingroup papi_af_ssoc_Functions */
/**  Returns the currently selected logical audio channel. */ 
/**  @param retval The currently selected logical channel. See datatype \ref papi_af_ssoc_Channel. */
extern FResult papi_af_ssoc_GetChannel( int * retval );

/**  @ingroup papi_af_ssoc_Functions */
/**  This function is used to select a logical audio channel.  */ 
/**  @param channel The logical channel that has to be selected. See datatype \ref papi_af_ssoc_Channel. */
extern FResult papi_af_ssoc_SetChannel( int channel );

/**  @ingroup papi_af_ssoc_Functions */
/**  Returns the number of front speaker channels for the specified logical channel. */ 
/**  @param channel The logical channel for which the number of front speaker channels is requested. */
/**  @param retval The number of front speaker channels. */
extern FResult papi_af_ssoc_FrontChannels( int channel, int * retval );

/**  @ingroup papi_af_ssoc_Functions */
/**  This function states whether the logical channel contains digital audio (including NICAM). */ 
/**  @param channel The logical channel for which information is to be obtained. */
/**  @param retval TRUE if the logical channel is digital, FALSE otherwise. */
extern FResult papi_af_ssoc_IsDigital( int channel, Bool * retval );

/**  @ingroup papi_af_ssoc_Functions */
/**  This function states whether the logical channel has Dolby Pro Logic information. */ 
/**  @param channel The logical channel for which information is to be obtained. */
/**  @param retval TRUE if the logical channel contains Dolby Pro Logic information, FALSE otherwise. */
extern FResult papi_af_ssoc_IsDolbyProLogic( int channel, Bool * retval );

/**  @ingroup papi_af_ssoc_Functions */
/**  This function evaluates whether the logical channel is a mono audio channel. */ 
/**  @param channel The logical channel for which information is to be obtained. */
/**  @param retval TRUE if the logical channel is a mono channel, FALSE otherwise. */
extern FResult papi_af_ssoc_IsMono( int channel, Bool * retval );

/**  @ingroup papi_af_ssoc_Functions */
/**  This function states whether the logical channel is related to the video signal. */ 
/**  @param channel The logical channel for which information is to be obtained. */
/**  @param retval TRUE if the logical channel is related, FALSE otherwise. */
extern FResult papi_af_ssoc_IsRelated( int channel, Bool * retval );

/**  @ingroup papi_af_ssoc_Functions */
/**  This function states whether the logical channel contains a second language signal. */ 
/**  @param channel The logical channel for which information is to be obtained. */
/**  @param retval TRUE if the logical channel contains a second language, FALSE otherwise. */
extern FResult papi_af_ssoc_IsSecondLanguage( int channel, Bool * retval );

/**  @ingroup papi_af_ssoc_Functions */
/**  This function evaluates whether the logical channel is a stereo audio channel. */ 
/**  @param channel The logical channel for which information is to be obtained. */
/**  @param retval TRUE if the logical channel is a stereo channel, FALSE otherwise. */
extern FResult papi_af_ssoc_IsStereo( int channel, Bool * retval );

/**  @ingroup papi_af_ssoc_Functions */
/**  Returns the availability of an LFE subwoofer channel for the specified logical channel. */ 
/**  @param channel The logical channel for which the availability of an LFE subwoofer channel is requested. */
/**  @param retval TRUE if an LFE subwoofer channel is available, FALSE otherwise. */
extern FResult papi_af_ssoc_LfeChannels( int channel, int * retval );

/**  @ingroup papi_af_ssoc_Functions */
/**  Returns the number of rear speaker channels for the specified logical channel. */ 
/**  @param channel The logical channel for which the number of rear speaker channels is requested. */
/**  @param retval The number of rear speaker channels. */
extern FResult papi_af_ssoc_RearChannels( int channel, int * retval );



/*
 *  interface ssoo of type IPlfApiSoundOutput
 */

/** @defgroup papi_af_ssoo papi_af_ssoo: SoundOutput
  * There are multiple instances of this interface:
- ssoo is for speaker
- hsoo is for headphone
.
It is not uncommon that the functionality on the headphone is limited, because of limitations in the platform. At least volume is expected to be supported on the headphone interface. Individual Supported() calls indicate which functionality is and is not supported.

  * @ingroup papi_af */

/** @defgroup papi_af_ssoo_Datatypes Datatypes 
  * @ingroup papi_af_ssoo */
/**
 *   @defgroup papi_af_ssoo_EqualizerBand Equalizer Band
 *   @ingroup papi_af_ssoo_Datatypes
 *   These are the equalizer band definitions.
 *   @{ */
#define papi_af_ssoo_MinEqualizerBand 0
#define papi_af_ssoo_MaxEqualizerBand 4
/*   @} */

/**
 *   @defgroup papi_af_ssoo_Volume Volume
 *   @ingroup papi_af_ssoo_Datatypes
 *   These are the volume definitions.
 *   @{ */
#define papi_af_ssoo_MinVolume 0
#define papi_af_ssoo_MaxVolume 255
/*   @} */

/**
 *   @defgroup papi_af_ssoo_Treble Treble
 *   @ingroup papi_af_ssoo_Datatypes
 *   These are the treble definitions.
 *   @{ */
#define papi_af_ssoo_MinTreble 0
#define papi_af_ssoo_MedTreble 128
#define papi_af_ssoo_MaxTreble 255
/*   @} */

/**
 *   @defgroup papi_af_ssoo_Balance Balance
 *   @ingroup papi_af_ssoo_Datatypes
 *   These are the balance definitions.
 *   @{ */
#define papi_af_ssoo_MinBalance -255
#define papi_af_ssoo_MedBalance 0
#define papi_af_ssoo_MaxBalance 255
/*   @} */

/**
 *   @defgroup papi_af_ssoo_DeltaVolume Delta Volume
 *   @ingroup papi_af_ssoo_Datatypes
 *   These are the delta volume definitions.
 *   @{ */
#define papi_af_ssoo_MinDeltaVolume -255
#define papi_af_ssoo_MedDeltaVolume 0
#define papi_af_ssoo_MaxDeltaVolume 255
/*   @} */

/**
 *   @defgroup papi_af_ssoo_Bass Bass
 *   @ingroup papi_af_ssoo_Datatypes
 *   These are the bass definitions.
 *   @{ */
#define papi_af_ssoo_MinBass 0
#define papi_af_ssoo_MedBass 128
#define papi_af_ssoo_MaxBass 255
/*   @} */

/** @defgroup papi_af_ssoo_Functions Functions 
  * @ingroup papi_af_ssoo */

/**  @ingroup papi_af_ssoo_Functions */
/**  Get the current balance. */ 
/**  @param retval Return value with the current balance. */
extern FResult papi_af_ssoo_GetBalance( int * retval );

/**  @ingroup papi_af_ssoo_Functions */
/**  Set the current balance. */ 
/**  @param balance Balance to be set. */
extern FResult papi_af_ssoo_SetBalance( int balance );

/**  @ingroup papi_af_ssoo_Functions */
/**  Return if balance is supported. */ 
/**  @param retval True, if balance is supported. False, if not. */
extern FResult papi_af_ssoo_BalanceSupported( Bool * retval );

/**  @ingroup papi_af_ssoo_Functions */
/**  Get bass. */ 
/**  @param retval Return value with the current bass. */
extern FResult papi_af_ssoo_GetBass( int * retval );

/**  @ingroup papi_af_ssoo_Functions */
/**  Set bass. */ 
/**  @param bass Bass to be set. */
extern FResult papi_af_ssoo_SetBass( int bass );

/**  @ingroup papi_af_ssoo_Functions */
/**  Return whether bass is supported. */ 
/**  @param retval True, if bass is supported. False, if not. */
extern FResult papi_af_ssoo_BassSupported( Bool * retval );

/**  @ingroup papi_af_ssoo_Functions */
/**  Get the current delta volume. */ 
/**  @param retval Return value with the current delta volume. */
extern FResult papi_af_ssoo_GetDeltaVolume( int * retval );

/**  @ingroup papi_af_ssoo_Functions */
/**  Set the current delta volume. */ 
/**  @param delta Delta volume to be set. */
extern FResult papi_af_ssoo_SetDeltaVolume( int delta );

/**  @ingroup papi_af_ssoo_Functions */
/**  Return whether delta volume is supported. */ 
/**  @param retval True, if delta volume is supported. False, if not. */
extern FResult papi_af_ssoo_DeltaVolumeSupported( Bool * retval );

/**  @ingroup papi_af_ssoo_Functions */
/**  Get equalizer value for given band. */ 
/**  @param band Equalizer band. */
/**  @param retval Return value indicating the equalizer value. */
extern FResult papi_af_ssoo_GetEqualizerBandValue( int band, int * retval );

/**  @ingroup papi_af_ssoo_Functions */
/**  Set equalizer value for given band. */ 
/**  @param band Band to be set. */
/**  @param value Equalizer value to be set. */
extern FResult papi_af_ssoo_SetEqualizerBandValue( int band, int value );

/**  @ingroup papi_af_ssoo_Functions */
/**  Return whether equalizer is supported. */ 
/**  @param retval True, if equalizer is supported. False, if not. */
extern FResult papi_af_ssoo_EqualizerSupported( Bool * retval );

/**  @ingroup papi_af_ssoo_Functions */
/**  Get equalizer maximum value for given band. */ 
/**  @param band Equalizer band. */
/**  @param retval Return value indicating the equalizer value. */
extern FResult papi_af_ssoo_MaxEqualizerBandValue( int band, int * retval );

/**  @ingroup papi_af_ssoo_Functions */
/**  Get equalizer medium value for given band. */ 
/**  @param band Equalizer band. */
/**  @param retval Return value indicating the equalizer value. */
extern FResult papi_af_ssoo_MedEqualizerBandValue( int band, int * retval );

/**  @ingroup papi_af_ssoo_Functions */
/**  Get equalizer minimum value for given band. */ 
/**  @param band Equalizer band. */
/**  @param retval Return value indicating the equalizer value. */
extern FResult papi_af_ssoo_MinEqualizerBandValue( int band, int * retval );

/**  @ingroup papi_af_ssoo_Functions */
/**  Enable/disable auto mute. */ 
/**  @param enable True, to enable auto mute. False, to disable it. */
extern FResult papi_af_ssoo_EnableMute( Bool enable );

/**  @ingroup papi_af_ssoo_Functions */
/**  Get mute. */ 
/**  @param retval Return value with the current the mute. */
extern FResult papi_af_ssoo_GetMute( Bool * retval );

/**  @ingroup papi_af_ssoo_Functions */
/**  Set Mute. */ 
/**  @param mute Mute to be set. */
extern FResult papi_af_ssoo_SetMute( Bool mute );

/**  @ingroup papi_af_ssoo_Functions */
/**  Returns whether mute is supported. */ 
/**  @param retval True, if mute is supported. False, if not. */
extern FResult papi_af_ssoo_MuteSupported( Bool * retval );

/**  @ingroup papi_af_ssoo_Functions */
/**  Get treble. */ 
/**  @param retval Return value with the current treble value. */
extern FResult papi_af_ssoo_GetTreble( int * retval );

/**  @ingroup papi_af_ssoo_Functions */
/**  Set treble. */ 
/**  @param treble Treble to be set. */
extern FResult papi_af_ssoo_SetTreble( int treble );

/**  @ingroup papi_af_ssoo_Functions */
/**  Return whether Treble is supported. */ 
/**  @param retval True, if treble is supported. False, if not. */
extern FResult papi_af_ssoo_TrebleSupported( Bool * retval );

/**  @ingroup papi_af_ssoo_Functions */
/**  Get the current volume. */ 
/**  @param retval Return value with the current volume. */
extern FResult papi_af_ssoo_GetVolume( int * retval );

/**  @ingroup papi_af_ssoo_Functions */
/**  Set the current volume. */ 
/**  @param volume Volume to be set. */
extern FResult papi_af_ssoo_SetVolume( int volume );

/**  @ingroup papi_af_ssoo_Functions */
/**  Return whether volume is supported. */ 
/**  @param retval True, if volume is supported. False, if not. */
extern FResult papi_af_ssoo_VolumeSupported( Bool * retval );



/*
 *  interface aclipN of type IPlfApiAudioClipNotify
 */

/** @defgroup papi_af_aclip papi_af_aclip: AudioClip
  * @ingroup papi_af */

/** @defgroup papi_af_aclipN_Notifications Notifications 
  * @ingroup papi_af_aclip */

/**  @ingroup papi_af_aclipN_Notifications */
/**  This notification function is called when the specified audio clip data buffer has been decoded. */ 
/**  @param id Identifier of the audio clip data buffer. */
extern void papi_af_aclipN_OnBufferAvailable( Nat32 id );

/**  @ingroup papi_af_aclipN_Notifications */
/**  This notification function is called when the audio clip data that was labelled endOfData has been decoded. */ 
extern void papi_af_aclipN_OnEndOfData( void );

/**  @ingroup papi_af_aclipN_Notifications */
/**  This notification function is called when audio clip decoding is paused. */ 
extern void papi_af_aclipN_OnPaused( void );

/**  @ingroup papi_af_aclipN_Notifications */
/**  This notification function is called when audio clip decoding has started. */ 
extern void papi_af_aclipN_OnStarted( void );

/**  @ingroup papi_af_aclipN_Notifications */
/**  This notification function is called when audio clip decoding is stopped. */ 
extern void papi_af_aclipN_OnStopped( void );



/*
 *  interface hsooN of type IPlfApiSoundOutputNotify
 */

/** @defgroup papi_af_hsoo papi_af_hsoo: SoundOutput
  * Notifications are only raised when the value changed due to another reason then a call to the corresponding Set function. E.g. if SetMute is called, there should be no <i>OnMuted</i> notification called.
  * @ingroup papi_af */

/** @defgroup papi_af_hsooN_Notifications Notifications 
  * @ingroup papi_af_hsoo */

/**  @ingroup papi_af_hsooN_Notifications */
/**  Notification that current bass value has changed. */ 
extern void papi_af_hsooN_OnBassChanged( void );

/**  @ingroup papi_af_hsooN_Notifications */
/**  Notification that current equalizer band value has changed. */ 
/**  @param band The Band number. */
extern void papi_af_hsooN_OnEqualizerBandValueChanged( int band );

/**  @ingroup papi_af_hsooN_Notifications */
/**  Notification that current mute value has changed. */ 
extern void papi_af_hsooN_OnMuted( void );

/**  @ingroup papi_af_hsooN_Notifications */
/**  Notification that current treble value has changed. */ 
extern void papi_af_hsooN_OnTrebleChanged( void );

/**  @ingroup papi_af_hsooN_Notifications */
/**  Notification that current volume value has changed. */ 
extern void papi_af_hsooN_OnVolumeChanged( void );



/*
 *  interface sipN of type IPlfApiSoundImprovementNotify
 */

/** @defgroup papi_af_sip papi_af_sip: SoundImprovement
  * @ingroup papi_af */

/** @defgroup papi_af_sipN_Notifications Notifications 
  * @ingroup papi_af_sip */

/**  @ingroup papi_af_sipN_Notifications */
/**  Notification if the allowed sound improvement mode has changed. */ 
extern void papi_af_sipN_OnImprovementModeAllowedChanged( void );

/**  @ingroup papi_af_sipN_Notifications */
/**  Notification if the current sound improvement mode has changed. */ 
extern void papi_af_sipN_OnImprovementModeChanged( void );

/**  @ingroup papi_af_sipN_Notifications */
/**  Notification if the allowed loudness mode has changed. */ 
extern void papi_af_sipN_OnLoudnessModeAllowedChanged( void );

/**  @ingroup papi_af_sipN_Notifications */
/**  Notification if the current loudness mode has changed. */ 
extern void papi_af_sipN_OnLoudnessModeChanged( void );



/*
 *  interface slaN of type IPlfApiSoundLevelAdjustNotify
 */

/** @defgroup papi_af_sla papi_af_sla: SoundLevelAdjust
  * @ingroup papi_af */

/** @defgroup papi_af_slaN_Notifications Notifications 
  * @ingroup papi_af_sla */

/**  @ingroup papi_af_slaN_Notifications */
/**  Notification that the allowed auto volume level has changed. */ 
extern void papi_af_slaN_OnAutoVolumeLevelAllowedChanged( void );

/**  @ingroup papi_af_slaN_Notifications */
/**  Notification that the current auto volume level has changed. */ 
extern void papi_af_slaN_OnAutoVolumeLevelChanged( void );

/**  @ingroup papi_af_slaN_Notifications */
/**  Notification that the allowed dynamic range compression has changed. */ 
extern void papi_af_slaN_OnDynamicRangeCompressionAllowedChanged( void );

/**  @ingroup papi_af_slaN_Notifications */
/**  Notification that the current dynamic range compression has changed. */ 
extern void papi_af_slaN_OnDynamicRangeCompressionChanged( void );

/**  @ingroup papi_af_slaN_Notifications */
/**  Notification that the allowed night mode compression has changed. */ 
extern void papi_af_slaN_OnNightModeAllowedChanged( void );

/**  @ingroup papi_af_slaN_Notifications */
/**  Notification that the current night mode compression has changed. */ 
extern void papi_af_slaN_OnNightModeChanged( void );



/*
 *  interface somN of type IPlfApiSurroundModeNotify
 */

/** @defgroup papi_af_som papi_af_som: SurroundMode
  * @ingroup papi_af */

/** @defgroup papi_af_somN_Notifications Notifications 
  * @ingroup papi_af_som */

/**  @ingroup papi_af_somN_Notifications */
/**  Notification that AutoSurroundMode is active. */ 
extern void papi_af_somN_AutoSurroundModeActiveNotify( void );

/**  @ingroup papi_af_somN_Notifications */
/**  Notification that the allowed SurroundLevel has changed. */ 
extern void papi_af_somN_OnSurroundLevelAllowedChanged( void );

/**  @ingroup papi_af_somN_Notifications */
/**  Notification that the current SurroundLevel has changed. */ 
extern void papi_af_somN_OnSurroundLevelChanged( void );

/**  @ingroup papi_af_somN_Notifications */
/**  Notification that the allowed SurroundMode has changed. */ 
extern void papi_af_somN_OnSurroundModeAllowedChanged( void );

/**  @ingroup papi_af_somN_Notifications */
/**  Notification that the current SurroundMode has changed. */ 
extern void papi_af_somN_OnSurroundModeChanged( void );



/*
 *  interface ssocN of type IPlfApiSoundChannelNotify
 */

/** @defgroup papi_af_ssoc papi_af_ssoc: SoundChannel
  * @ingroup papi_af */

/** @defgroup papi_af_ssocN_Notifications Notifications 
  * @ingroup papi_af_ssoc */

/**  @ingroup papi_af_ssocN_Notifications */
/**  This function is called when the currently selected logical channel has been changed autonomously. */ 
extern void papi_af_ssocN_OnChannelChanged( void );

/**  @ingroup papi_af_ssocN_Notifications */
/**  This function is called when one or more properties (as defined in the ITvSoundChannel datasheet) of a logical sound channel changes. */ 
/**  @param channel Input parameter indicating the Channel number. */
extern void papi_af_ssocN_OnSoundPropertiesChanged( int channel );



/*
 *  interface ssooN of type IPlfApiSoundOutputNotify
 */

/** @defgroup papi_af_ssoo papi_af_ssoo: SoundOutput
  * Notifications are only raised when the value changed due to another reason then a call to the corresponding Set function. E.g. if SetMute is called, there should be no <i>OnMuted</i> notification called.
  * @ingroup papi_af */

/** @defgroup papi_af_ssooN_Notifications Notifications 
  * @ingroup papi_af_ssoo */

/**  @ingroup papi_af_ssooN_Notifications */
/**  Notification that current bass value has changed. */ 
extern void papi_af_ssooN_OnBassChanged( void );

/**  @ingroup papi_af_ssooN_Notifications */
/**  Notification that current equalizer band value has changed. */ 
/**  @param band The Band number. */
extern void papi_af_ssooN_OnEqualizerBandValueChanged( int band );

/**  @ingroup papi_af_ssooN_Notifications */
/**  Notification that current mute value has changed. */ 
extern void papi_af_ssooN_OnMuted( void );

/**  @ingroup papi_af_ssooN_Notifications */
/**  Notification that current treble value has changed. */ 
extern void papi_af_ssooN_OnTrebleChanged( void );

/**  @ingroup papi_af_ssooN_Notifications */
/**  Notification that current volume value has changed. */ 
extern void papi_af_ssooN_OnVolumeChanged( void );


