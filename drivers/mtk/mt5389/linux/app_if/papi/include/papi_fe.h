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
 *   @file papi_fe.h
 */
/** @defgroup papi_fe  */

/*
 *  interface assm of type IPlfApiSigStrengthMeas
 */

/** @defgroup papi_fe_assm papi_fe_assm: SigStrengthMeas
  * This interface is used to measure the strength of a signal. How and where in the signal path this measurement is performed is platform specific. There can be multiple instances of this interface to do different measurements at different positions in the signal path. The strength measurements that are reported via this interface is a logical representation of this measurement: the higher the value the stronger the signal. How the physical measurement is translated into the logical representation that is reported via this interface is up to the platform.<br>
Multiple instances of this interface have been defined:
<ul>
<li> ssm: signal strength measurement at the tuner level, used for both analog and digital</li>
<li> assm: signal strength measurement at the level of the channel decoder, used only for digital</li>
<li> qssm: signal strength measurement at the level of the channel decoder, used only for digital</li>
</ul>

  * @ingroup papi_fe */

/** @defgroup papi_fe_assm_Functions Functions 
  * @ingroup papi_fe_assm */

/**  @ingroup papi_fe_assm_Functions */
/**  This function returns if the current signal strength measurement is valid. */ 
/**  @param Valid Output parameter returning if the current measurement is valid. */
/**  @param retval standard return value. */
extern FResult papi_fe_assm_GetMeasValid( Bool * Valid, Nat32 * retval );

/**  @ingroup papi_fe_assm_Functions */
/**  Returns the minimum and maximum signal strength. */ 
/**  @param MinStrength Output parameter returning the minimum signal strength. */
/**  @param MaxStrength Output parameter returning the maximum signal strength. */
/**  @param retval standard return value. */
extern FResult papi_fe_assm_GetSigStrengthRange( Nat32 * MinStrength, Nat32 * MaxStrength, Nat32 * retval );

/**  @ingroup papi_fe_assm_Functions */
/**  This function returns the last measured signal strength. */ 
/**  @param Strength Output parameter returning the last measured signal strength. */
/**  @param retval standard return value. */
extern FResult papi_fe_assm_GetSigStrength( Nat32 * Strength, Nat32 * retval );

/**  @ingroup papi_fe_assm_Functions */
/**  This function starts the signal strength measurement. */ 
/**  @param Avail True, if the measurement is immediately available after this function returns. False, if availability of the measurement will be signalled asynchronously via a notification. */
/**  @param retval standard return value. */
extern FResult papi_fe_assm_Start( Bool * Avail, Nat32 * retval );

/**  @ingroup papi_fe_assm_Functions */
/**  This function stops the signal strength measurement. */ 
/**  @param Stopped True, if the measurement is indeed stopped after this function returns. False, if the measurement will be stopped asynchronously and signalled via a notification. */
/**  @param retval standard return value. */
extern FResult papi_fe_assm_Stop( Bool * Stopped, Nat32 * retval );



/*
 *  interface chantab of type IPlfApiChanTable
 */

/** @defgroup papi_fe_chantab papi_fe_chantab: ChanTable
  * @ingroup papi_fe */

/** @defgroup papi_fe_chantab_Functions Functions 
  * @ingroup papi_fe_chantab */

/**  @ingroup papi_fe_chantab_Functions */
/**  This function returns the carrier frequency. Note that the carrier frequency is not equal to the middle frequency of a channel. A typical offset to the lower boundary is 1.25 MHz. This value is not the same for every channel in a channel table */ 
/**  @param chan Input parameter indicating the channel number for which the frequency must be obtained. */
/**  @param table Input parameter indicating the channel table in which the channel must be found. */
/**  @param Freq Output parameter returning the carrier frequency in units of 1/16th MHz. <i>NoFrequency</i> is returned if the channel does not exist */
/**  @param retval Standard Return Value. */
extern FResult papi_fe_chantab_GetCarrierFreq( Nat32 chan, tmFe_ChanTable_t table, Nat32 * Freq, Nat32 * retval );

/**  @ingroup papi_fe_chantab_Functions */
/**  This function returns the first channel in a given channel table. */ 
/**  @param table Input parameter indicating the channel table in which the channel must be found. */
/**  @param Chan Output parameter returning the number of the first channel. <i>NoChannel</i> is returned if the channel table is unknown */
/**  @param retval Standard Return Value. */
extern FResult papi_fe_chantab_GetFirstChan( tmFe_ChanTable_t table, Nat32 * Chan, Nat32 * retval );

/**  @ingroup papi_fe_chantab_Functions */
/**  This function returns the first channel table. Note this function does not select the first channel table, use <i>SetChannelTable()</i> to apply a selection */ 
/**  @param Table Output parameter returning the first defined channel table. */
/**  @param retval Standard Return Value. */
extern FResult papi_fe_chantab_GetFirstTable( tmFe_ChanTable_t * table, Nat32 * retval );

/**  @ingroup papi_fe_chantab_Functions */
/**  This function returns the channel number that belongs to the given frequency. */ 
/**  @param freq Input parameter indicating the frequency (in units of 1/16th MHz) for which the channel must be obtained. */
/**  @param table Input parameter indicating the channel table in which the frequency must be found. */
/**  @param Chan Output parameter returning the channel number that corresponds to the given frequency. <i>NoChannel</i> is returned if the frequency does not belong to one of the channels in the channel table */
/**  @param retval Standard Return Value. */
extern FResult papi_fe_chantab_Freq2Chan( Nat32 freq, tmFe_ChanTable_t table, Nat32 * Chan, Nat32 * retval );

/**  @ingroup papi_fe_chantab_Functions */
/**  States whether the channel is defined in the channel table. It does not say anything about physical presence */ 
/**  @param chan Input parameter indicating the channel number to investigate. */
/**  @param table Input parameter indicating the channel table in which the channel must be found. */
/**  @param IsChan Output parameter returning the state indicating if the channel exists in the channel table. */
/**  @param retval Standard Return Value. */
extern FResult papi_fe_chantab_GetIsChan( Nat32 chan, tmFe_ChanTable_t table, Bool * IsChan, Nat32 * retval );

/**  @ingroup papi_fe_chantab_Functions */
/**  This function returns the last channel in a given channel table. */ 
/**  @param table Input parameter indicating the channel table in which the channel must be found. */
/**  @param Chan Output parameter returning the number of the last channel.<i>NoChannel</i> is returned if the channel table is unknown */
/**  @param retval Standard Return Value. */
extern FResult papi_fe_chantab_GetLastChan( tmFe_ChanTable_t table, Nat32 * Chan, Nat32 * retval );

/**  @ingroup papi_fe_chantab_Functions */
/**  This function returns the lower frequency. */ 
/**  @param chan Input parameter indicating the channel number for which the frequency must be obtained. */
/**  @param table Input parameter indicating the channel table in which the channel must be found. */
/**  @param Freq Output parameter returning the lower frequency in units of 1/16th MHz. */
/**  @param retval Standard Return Value. */
extern FResult papi_fe_chantab_GetLowerFreq( Nat32 chan, tmFe_ChanTable_t table, Nat32 * Freq, Nat32 * retval );

/**  @ingroup papi_fe_chantab_Functions */
/**  Given a channel number, returns the next defined channel number in the channel table. The behaviour is cyclic: the next of the last channel number returns the first channel number */ 
/**  @param chan Input parameter indicating the channel for which the next channel must be found. */
/**  @param table Input parameter indicating the channel table in which the channel must be found. */
/**  @param Chan Output parameter returning the number of the next defined channel. <i>NoChannel</i> is returned if channel is not defined in the channel table */
/**  @param retval Standard Return Value. */
extern FResult papi_fe_chantab_GetNextChan( Nat32 chan, tmFe_ChanTable_t table, Nat32 * Chan, Nat32 * retval );

/**  @ingroup papi_fe_chantab_Functions */
/**  Provides the identifier of the channel table that is defined as next to the current selected channel table. The behaviour is cyclic: the next of the last channel table returns the first defined channel table. Note this function does not select the next channel table, use <i>SetChannelTable()</i> to apply a selection */ 
/**  @param curtable Input parameter indicating the channel table for which the next channel table must be found. */
/**  @param Table Output parameter returning the next defined channel table. */
/**  @param retval Standard Return Value. */
extern FResult papi_fe_chantab_GetNextTable( tmFe_ChanTable_t curtable, tmFe_ChanTable_t * table, Nat32 * retval );

/**  @ingroup papi_fe_chantab_Functions */
/**  This function returns the next valid channel starting from frequency upwards  without wrapping. In case the frequency parameter defines a valid channel, the return value equals Frequency2Channel(frequency, table). Otherwise, the channel number with carrier frequency being nearest to but higher than the value of the frequency parameter is returned */ 
/**  @param freq Input parameter indicating the frequency (in units of 1/16th MHz) to start from upwards. */
/**  @param table Input parameter indicating the channel table in which the channel must be found. */
/**  @param Chan Output parameter returning the number of the next valid channel. <i>NoChannel</i> is returned if no channel can be found, this occurs when frequency is above the upper-frequency of the highest channel in the spectrum defined by the given channel table */
/**  @param retval Standard Return Value. */
extern FResult papi_fe_chantab_GetNextValidChan( Nat32 freq, tmFe_ChanTable_t table, Nat32 * Chan, Nat32 * retval );

/**  @ingroup papi_fe_chantab_Functions */
/**  Given a channel number, returns the previous defined channel number in the channel table. The behaviour is cyclic: the previous of the first channel number returns the last channel number */ 
/**  @param chan Input parameter indicating the channel for which the previous channel must be found. */
/**  @param table Input parameter indicating the channel table in which the channel must be found. */
/**  @param Chan Output parameter returning the number of the previous defined channel. */
/**  @param retval Standard Return Value. */
extern FResult papi_fe_chantab_GetPrevChan( Nat32 chan, tmFe_ChanTable_t table, Nat32 * Chan, Nat32 * retval );

/**  @ingroup papi_fe_chantab_Functions */
/**  This function returns the previous valid channel starting from frequency downwards  without wrapping. In case the frequency parameter defines a valid channel, the return value equals Frequency2Channel(frequency, table). Otherwise, the channel number with carrier frequency being nearest to but lower than the value of the frequency parameter is returned */ 
/**  @param freq Input parameter indicating the frequency (in units of 1/16th MHz) to start from downwards. */
/**  @param table Input parameter indicating the channel table in which the channel must be found. */
/**  @param Chan Output parameter returning the number of the previous valid channel. */
/**  @param retval Standard Return Value. */
extern FResult papi_fe_chantab_GetPrevValidChan( Nat32 freq, tmFe_ChanTable_t table, Nat32 * Chan, Nat32 * retval );

/**  @ingroup papi_fe_chantab_Functions */
/**  This function returns the upper and lower boundary frequencies of the segment in which the specified channel is located in the channel table. A segment is a closed band area in which all channels have the same bandwidth and same offset, e.g. in the EuropeC table channels 21-68 are all located side by side and all have the same bandwidth and same offset */ 
/**  @param chan Input parameter indicating the channel number for which the frequencies must be obtained. */
/**  @param table Input parameter indicating the channel table in which the channel must be found. */
/**  @param LowerBound Output parameter returning the lower bound frequency in units of 1/16th MHz. */
/**  @param UpperBound Output parameter returning the upper bound frequency in units of 1/16th MHz. */
/**  @param retval Standard Return Value. */
extern FResult papi_fe_chantab_GetSegmentBoundaries( Nat32 chan, tmFe_ChanTable_t table, Nat32 * LowerBound, Nat32 * UpperBound, Nat32 * retval );

/**  @ingroup papi_fe_chantab_Functions */
/**  This function returns the next channel table. */ 
/**  @param table Input parameter indicating the channel table to be checked if it is supported. */
/**  @param Supp Output parameter returning the state indicating if the channel table is supported. */
/**  @param retval Standard Return Value. */
extern FResult papi_fe_chantab_GetTableSupp( tmFe_ChanTable_t table, Bool * Supp, Nat32 * retval );

/**  @ingroup papi_fe_chantab_Functions */
/**  This function returns the Channel Table. */ 
/**  @param table Output parameter returning the currently selected channel table. */
/**  @param retval Standard Return Value. */
extern FResult papi_fe_chantab_GetTable( tmFe_ChanTable_t * table, Nat32 * retval );

/**  @ingroup papi_fe_chantab_Functions */
/**  This function sets the Channel Table. The selection of the channel table does not result in an autonomous adjustment of the tuner frequency to adhere to a possible difference in the channel definition of the newly selected channel table */ 
/**  @param table Input parameter indicating the channel table that should be selected. */
/**  @param retval Standard Return Value. */
extern FResult papi_fe_chantab_SetTable( tmFe_ChanTable_t table, Nat32 * retval );

/**  @ingroup papi_fe_chantab_Functions */
/**  This function returns the upper frequency. */ 
/**  @param chan Input parameter indicating the channel number for which the frequency must be obtained. */
/**  @param table Input parameter indicating the channel table in which the channel must be found. */
/**  @param Freq Output parameter returning the upper frequency in units of 1/16th MHz. */
/**  @param retval Standard Return Value. */
extern FResult papi_fe_chantab_GetUpperFreq( Nat32 chan, tmFe_ChanTable_t table, Nat32 * Freq, Nat32 * retval );



/*
 *  interface chdec of type IPlfApiChanDec
 */

/** @defgroup papi_fe_chdec papi_fe_chdec: ChanDec
  * The channel decoder controls digital demodulation and channel decoding. For additional information, see also the UHAPI Specification on Channel Decoding.
  * @ingroup papi_fe */

/** @defgroup papi_fe_chdec_Functions Functions 
  * @ingroup papi_fe_chdec */

/**  @ingroup papi_fe_chdec_Functions */
/**  Returns the actual BER value in units of 10-8. For BER values less than or equal to 10-8, the platform returns a value of 1. If 0 (zero) is returned, the BER is invalid. */ 
/**  @param Ber Output parameter returning 0, 1, or the actual BER value. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec_GetBer( Nat32 * Ber, Nat32 * retval );

/**  @ingroup papi_fe_chdec_Functions */
/**  This function returns the current BER (Bit Error Rate) threshold range. This is the post viterbi bit error rate (before RS decoding). */ 
/**  @param MinThreshold Output parameter returning the BER min. threshold value */
/**  @param MaxThreshold Output parameter returning the BER max. threshold value */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec_GetBerThresholdRange( Nat32 * MinThreshold, Nat32 * MaxThreshold, Nat32 * retval );

/**  @ingroup papi_fe_chdec_Functions */
/**  Returns the BER threshold value in units of 10-8. */ 
/**  @param Threshold Output parameter returning the BER threshold value in units of 10-8. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec_GetBerThreshold( Nat32 * Threshold, Nat32 * retval );

/**  @ingroup papi_fe_chdec_Functions */
/**  Set the threshold BER value. If the signalquality (as measured with the BER) goes below or above the threshold a notification is given by the platform. */ 
/**  @param threshold The threshold value in units of 10-8. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec_SetBerThreshold( Nat32 threshold, Nat32 * retval );

/**  @ingroup papi_fe_chdec_Functions */
/**  Returns the presence of a carrier. (Demodulator has been able to lock onto valid data). */ 
/**  @param Present Output parameter returns presence (True) or absence (False) of the carrier. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec_GetCarrierPresent( Bool * Present, Nat32 * retval );

/**  @ingroup papi_fe_chdec_Functions */
/**  Returns the actual constellation. */ 
/**  @param pConstellation Output parameter returning the actual constellation. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec_GetActualConstellation( tmFe_Constellation_t * pConstellation, Nat32 * retval );

/**  @ingroup papi_fe_chdec_Functions */
/**  Returns the constellation. */ 
/**  @param pConstellation Output parameter returning the constellation. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec_GetConstellation( tmFe_Constellation_t * pConstellation, Nat32 * retval );

/**  @ingroup papi_fe_chdec_Functions */
/**  Sets the constellation. */ 
/**  @param constellation The constellation. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec_SetConstellation( tmFe_Constellation_t constellation, Nat32 * retval );

/**  @ingroup papi_fe_chdec_Functions */
/**  Returns the supported constellations. */ 
/**  @param pConstellations Output parameter returning the supported constellations. The value is a bit-mask that is constructed by OR-ing the values of enum #_tmFe_Constellation_t. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec_GetSuppConstellations( tmFe_ConstellationSet_t * pConstellations, Nat32 * retval );

/**  @ingroup papi_fe_chdec_Functions */
/**  This function reports whether getting and setting the frequency error is supported. */ 
/**  @param FreqErrorSupp Output parameter indicating whether getting and setting the frequency error is supported. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec_GetFrequencyErrorSupp( Bool * FreqErrorSupp, Nat32 * retval );

/**  @ingroup papi_fe_chdec_Functions */
/**  Returns the frequency error due to the tuner and local oscillator as retrieved by the frequency error correction algorithm in channel decoder. This can be read during installation and reapplied during zapping. This will accelerate the lock time of zapping. */ 
/**  @param Error Output Parameter returning the Frequency error. (unit: Hz) */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec_GetFrequencyError( Int32 * Error, Nat32 * retval );

/**  @ingroup papi_fe_chdec_Functions */
/**  Sets the frequency error due to the tuner and local oscillator to accelerate the lock time of zapping. Note that the provided value is typically acquired during installation time using GetFrequencyError. Optional: HW specific! */ 
/**  @param error Input parameter indicating the Frequency error to be set (unit: Hz). */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec_SetFrequencyError( Int32 error, Nat32 * retval );

/**  @ingroup papi_fe_chdec_Functions */
/**  This function returns the currently active IQ mode. */ 
/**  @param pIqMode Output parameter returning the actual IQ mode. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec_GetActualIqMode( tmFe_IqMode_t * pIqMode, Nat32 * retval );

/**  @ingroup papi_fe_chdec_Functions */
/**  Returns the spectrum type. */ 
/**  @param pIqMode Output parameter returning the spectrum type. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec_GetIqMode( tmFe_IqMode_t * pIqMode, Nat32 * retval );

/**  @ingroup papi_fe_chdec_Functions */
/**  Sets the spectrum type. */ 
/**  @param iqMode The spectrum type. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec_SetIqMode( tmFe_IqMode_t iqMode, Nat32 * retval );

/**  @ingroup papi_fe_chdec_Functions */
/**  Returns the supported spectrum types. (This relates to the orientation of the q and I vector. They are perpendicular, but can be ( Q, I) or (Q, -I). The first one is referred  to as normal, the latter one as inverse. As far as I  know, needed for DVB-T. I think normal is the most comon one.) */ 
/**  @param pIqModes Output parameter returning the supported spectrum types. The value is a bit-mask that is constructed by OR-ing the values of enum #_tmFe_IqMode_t. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec_GetSuppIqModes( tmFe_IqModeSet_t * pIqModes, Nat32 * retval );

/**  @ingroup papi_fe_chdec_Functions */
/**  This function reports whether getting and setting the symbol error is supported. (The symbol error is indeed after RS decoding.)) */ 
/**  @param SymbolErrorSupp Output parameter indicating whether getting and setting the symbol error is supported. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec_GetSymbolErrorSupp( Bool * SymbolErrorSupp, Nat32 * retval );

/**  @ingroup papi_fe_chdec_Functions */
/**  Returns the symbol error. This can be read during installation and reapplied during zapping. This will accelerate the lock time of zapping. */ 
/**  @param Error Output Parameter returning the symbol error. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec_GetSymbolError( Int32 * Error, Nat32 * retval );

/**  @ingroup papi_fe_chdec_Functions */
/**  Sets the symbol error to accelerate the lock time of zapping. Note that the provided value is typically acquired during installation time using GetSymbolError. Optional: HW specific! */ 
/**  @param error Input parameter indicating the symbol error to be set. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec_SetSymbolError( Int32 error, Nat32 * retval );

/**  @ingroup papi_fe_chdec_Functions */
/**  This function reports whether getting and setting the time error is supported. (This may indeed be a confusing name. This is a measure for the error in the sample frequency in the channel decoder (after a tuned signal). The unit is ppm.) */ 
/**  @param TimeErrorSupp Output parameter indicating whether getting and setting the time error is supported. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec_GetTimeErrorSupp( Bool * TimeErrorSupp, Nat32 * retval );

/**  @ingroup papi_fe_chdec_Functions */
/**  Returns the error on the sampling frequency due to the local oscillator as retrieved by the sampling error correction in channel decoder. During scan mode (Installation) this error can be read and stored by middleware in order to be re-applied later on in zapping mode. This will accelerate the lock time of zapping. (The offset is the distance of the actual carrier frequency to the grid frequency. This is measured in 166 KHz steps (1/6 MHz). Within DVB-T it is required +/- 3 steps are supported. The frequency error is in Hz and is the difference between the local oscillator and the tuner.) */ 
/**  @param Error Output Parameter returning the Time error (unit: ppm). */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec_GetTimeError( Int32 * Error, Nat32 * retval );

/**  @ingroup papi_fe_chdec_Functions */
/**  Sets the error on the sampling frequency to accelerate the lock time of zapping. Note that the provided value is typically acquired during installation time using GetTimeError. Optional: HW specific! */ 
/**  @param error Input Parameter indicating the sampling frequency error to be set (unit: ppm). */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec_SetTimeError( Int32 error, Nat32 * retval );



/*
 *  interface chdec2 of type IPlfApiChanDec
 */

/** @defgroup papi_fe_chdec2 papi_fe_chdec2: ChanDec
  * The channel decoder controls digital demodulation and channel decoding. For additional information, see also the UHAPI Specification on Channel Decoding.
  * @ingroup papi_fe */

/** @defgroup papi_fe_chdec2_Functions Functions 
  * @ingroup papi_fe_chdec2 */

/**  @ingroup papi_fe_chdec2_Functions */
/**  Returns the actual BER value in units of 10-8. For BER values less than or equal to 10-8, the platform returns a value of 1. If 0 (zero) is returned, the BER is invalid. */ 
/**  @param Ber Output parameter returning 0, 1, or the actual BER value. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec2_GetBer( Nat32 * Ber, Nat32 * retval );

/**  @ingroup papi_fe_chdec2_Functions */
/**  This function returns the current BER (Bit Error Rate) threshold range. This is the post viterbi bit error rate (before RS decoding). */ 
/**  @param MinThreshold Output parameter returning the BER min. threshold value */
/**  @param MaxThreshold Output parameter returning the BER max. threshold value */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec2_GetBerThresholdRange( Nat32 * MinThreshold, Nat32 * MaxThreshold, Nat32 * retval );

/**  @ingroup papi_fe_chdec2_Functions */
/**  Returns the BER threshold value in units of 10-8. */ 
/**  @param Threshold Output parameter returning the BER threshold value in units of 10-8. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec2_GetBerThreshold( Nat32 * Threshold, Nat32 * retval );

/**  @ingroup papi_fe_chdec2_Functions */
/**  Set the threshold BER value. If the signalquality (as measured with the BER) goes below or above the threshold a notification is given by the platform. */ 
/**  @param threshold The threshold value in units of 10-8. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec2_SetBerThreshold( Nat32 threshold, Nat32 * retval );

/**  @ingroup papi_fe_chdec2_Functions */
/**  Returns the presence of a carrier. (Demodulator has been able to lock onto valid data). */ 
/**  @param Present Output parameter returns presence (True) or absence (False) of the carrier. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec2_GetCarrierPresent( Bool * Present, Nat32 * retval );

/**  @ingroup papi_fe_chdec2_Functions */
/**  Returns the actual constellation. */ 
/**  @param pConstellation Output parameter returning the actual constellation. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec2_GetActualConstellation( tmFe_Constellation_t * pConstellation, Nat32 * retval );

/**  @ingroup papi_fe_chdec2_Functions */
/**  Returns the constellation. */ 
/**  @param pConstellation Output parameter returning the constellation. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec2_GetConstellation( tmFe_Constellation_t * pConstellation, Nat32 * retval );

/**  @ingroup papi_fe_chdec2_Functions */
/**  Sets the constellation. */ 
/**  @param constellation The constellation. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec2_SetConstellation( tmFe_Constellation_t constellation, Nat32 * retval );

/**  @ingroup papi_fe_chdec2_Functions */
/**  Returns the supported constellations. */ 
/**  @param pConstellations Output parameter returning the supported constellations. The value is a bit-mask that is constructed by OR-ing the values of enum #_tmFe_Constellation_t. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec2_GetSuppConstellations( tmFe_ConstellationSet_t * pConstellations, Nat32 * retval );

/**  @ingroup papi_fe_chdec2_Functions */
/**  This function reports whether getting and setting the frequency error is supported. */ 
/**  @param FreqErrorSupp Output parameter indicating whether getting and setting the frequency error is supported. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec2_GetFrequencyErrorSupp( Bool * FreqErrorSupp, Nat32 * retval );

/**  @ingroup papi_fe_chdec2_Functions */
/**  Returns the frequency error due to the tuner and local oscillator as retrieved by the frequency error correction algorithm in channel decoder. This can be read during installation and reapplied during zapping. This will accelerate the lock time of zapping. */ 
/**  @param Error Output Parameter returning the Frequency error. (unit: Hz) */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec2_GetFrequencyError( Int32 * Error, Nat32 * retval );

/**  @ingroup papi_fe_chdec2_Functions */
/**  Sets the frequency error due to the tuner and local oscillator to accelerate the lock time of zapping. Note that the provided value is typically acquired during installation time using GetFrequencyError. Optional: HW specific! */ 
/**  @param error Input parameter indicating the Frequency error to be set (unit: Hz). */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec2_SetFrequencyError( Int32 error, Nat32 * retval );

/**  @ingroup papi_fe_chdec2_Functions */
/**  This function returns the currently active IQ mode. */ 
/**  @param pIqMode Output parameter returning the actual IQ mode. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec2_GetActualIqMode( tmFe_IqMode_t * pIqMode, Nat32 * retval );

/**  @ingroup papi_fe_chdec2_Functions */
/**  Returns the spectrum type. */ 
/**  @param pIqMode Output parameter returning the spectrum type. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec2_GetIqMode( tmFe_IqMode_t * pIqMode, Nat32 * retval );

/**  @ingroup papi_fe_chdec2_Functions */
/**  Sets the spectrum type. */ 
/**  @param iqMode The spectrum type. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec2_SetIqMode( tmFe_IqMode_t iqMode, Nat32 * retval );

/**  @ingroup papi_fe_chdec2_Functions */
/**  Returns the supported spectrum types. (This relates to the orientation of the q and I vector. They are perpendicular, but can be ( Q, I) or (Q, -I). The first one is referred  to as normal, the latter one as inverse. As far as I  know, needed for DVB-T. I think normal is the most comon one.) */ 
/**  @param pIqModes Output parameter returning the supported spectrum types. The value is a bit-mask that is constructed by OR-ing the values of enum #_tmFe_IqMode_t. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec2_GetSuppIqModes( tmFe_IqModeSet_t * pIqModes, Nat32 * retval );

/**  @ingroup papi_fe_chdec2_Functions */
/**  This function reports whether getting and setting the symbol error is supported. (The symbol error is indeed after RS decoding.)) */ 
/**  @param SymbolErrorSupp Output parameter indicating whether getting and setting the symbol error is supported. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec2_GetSymbolErrorSupp( Bool * SymbolErrorSupp, Nat32 * retval );

/**  @ingroup papi_fe_chdec2_Functions */
/**  Returns the symbol error. This can be read during installation and reapplied during zapping. This will accelerate the lock time of zapping. */ 
/**  @param Error Output Parameter returning the symbol error. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec2_GetSymbolError( Int32 * Error, Nat32 * retval );

/**  @ingroup papi_fe_chdec2_Functions */
/**  Sets the symbol error to accelerate the lock time of zapping. Note that the provided value is typically acquired during installation time using GetSymbolError. Optional: HW specific! */ 
/**  @param error Input parameter indicating the symbol error to be set. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec2_SetSymbolError( Int32 error, Nat32 * retval );

/**  @ingroup papi_fe_chdec2_Functions */
/**  This function reports whether getting and setting the time error is supported. (This may indeed be a confusing name. This is a measure for the error in the sample frequency in the channel decoder (after a tuned signal). The unit is ppm.) */ 
/**  @param TimeErrorSupp Output parameter indicating whether getting and setting the time error is supported. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec2_GetTimeErrorSupp( Bool * TimeErrorSupp, Nat32 * retval );

/**  @ingroup papi_fe_chdec2_Functions */
/**  Returns the error on the sampling frequency due to the local oscillator as retrieved by the sampling error correction in channel decoder. During scan mode (Installation) this error can be read and stored by middleware in order to be re-applied later on in zapping mode. This will accelerate the lock time of zapping. (The offset is the distance of the actual carrier frequency to the grid frequency. This is measured in 166 KHz steps (1/6 MHz). Within DVB-T it is required +/- 3 steps are supported. The frequency error is in Hz and is the difference between the local oscillator and the tuner.) */ 
/**  @param Error Output Parameter returning the Time error (unit: ppm). */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec2_GetTimeError( Int32 * Error, Nat32 * retval );

/**  @ingroup papi_fe_chdec2_Functions */
/**  Sets the error on the sampling frequency to accelerate the lock time of zapping. Note that the provided value is typically acquired during installation time using GetTimeError. Optional: HW specific! */ 
/**  @param error Input Parameter indicating the sampling frequency error to be set (unit: ppm). */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdec2_SetTimeError( Int32 error, Nat32 * retval );



/*
 *  interface chdecdvbt of type IPlfApiChanDecDvbT
 */

/** @defgroup papi_fe_chdecdvbt papi_fe_chdecdvbt: ChanDecDvbT
  * @ingroup papi_fe */

/** @defgroup papi_fe_chdecdvbt_Functions Functions 
  * @ingroup papi_fe_chdecdvbt */

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  This function returns the actual channel bandwidth (one of the channel bandwidth parameters of this interface). If automatic detection is supported and enabled this function will return if the detection is busy and if the automatic detection was unsuccessful or has been aborted. */ 
/**  @param Bandwidth Output Parameter returning the actual Bandwidth. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_GetActualChanBandwidth( tmFe_ChanBandwidth_t * Bandwidth, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  This function returns the Channel Bandwidth as set via the SetChanBandwidth function. */ 
/**  @param Bandwidth Output Parameter returning the Bandwidth that was set via SetChanBandwidth function. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_GetChanBandwidth( tmFe_ChanBandwidth_t * Bandwidth, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  This function sets the Channel Bandwidth. Only 6, 7 and 8MHz are supported. There is no auto detection of the channel bandwidth. */ 
/**  @param bandwidth Input Parameter indicating the new channel Bandwidth to be set. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_SetChanBandwidth( tmFe_ChanBandwidth_t bandwidth, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  Returns the Channel Bandwidths that are supported. */ 
/**  @param Bandwidths Output Parameter returning the supported channel Bandwidths. The value is a bit-mask that is constructed by OR-ing the values of enum #_tmFe_ChanBandwidth_t. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_GetSuppChanBandwidths( tmFe_ChanBandwidthSet_t * Bandwidths, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  Some channel decoders provide some settings that can be used to improve the zap speed. As all information with respect to the physical channels is owned and stored in the middleware, the platform needed an interface to the middleware. GetChannelInformation() is called during installation to collect this information and SetChannelInformation() is called during zapping to provide this information to speed up zapping. The information that is retrieved is not interpreted by the middleware and provided as-is to the platform. */ 
/**  @param info Output parameter returning channel information of the transport stream. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_GetChannelInformation( Nat16 * info, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  This function sets channel information. See also GetChannelInformation(). */ 
/**  @param info Input parameter with the channel information to be set. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_SetChannelInformation( Nat16 info, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  This function returns the actual code rate (one of the code rate parameters of this interface) for the selected priority bit stream (one of the priority parameters of this interface). If automatic detection is supported and enabled this function will return tmFe_CodeRateInvalid if the detection is busy and tmFe_CodeRateUnknown if the automatic detection was unsuccessful or has been aborted. */ 
/**  @param HighPrioRate Output parameter returning coderate for high priority stream. */
/**  @param LowPrioRate Output Parameter returning coderate for low priority stream. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_GetActualCodeRates( tmFe_CodeRate_t * HighPrioRate, tmFe_CodeRate_t * LowPrioRate, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  Returns the Code Rates that are supported. */ 
/**  @param Rates Output parameter returning coderates that are supported. The value is a bit-mask that is constructed by OR-ing the values of enum #_tmFe_CodeRate_t. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_GetSuppCodeRates( tmFe_CodeRateSet_t * Rates, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  Returns the coderates for the received High and Low Priority streams as they were set via the SetCodeRates function. */ 
/**  @param HighPrioRate Output parameter returning the set coderate for high priority stream. */
/**  @param LowPrioRate Output Parameter returning the set coderate for low priority stream. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_GetCodeRates( tmFe_CodeRate_t * HighPrioRate, tmFe_CodeRate_t * LowPrioRate, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  This function sets the code rate (one of the code rate parameters of this interface) for a selected priority bit streams (one of the priority parameters of this interface). tmFe_CodeRateInvalid, tmFe_CodeRateUnknown and tmFe_CodeRateNone cannot be set. */ 
/**  @param highPrioRate Input parameter indicating coderate for high priority stream. */
/**  @param lowPrioRate Input Parameter indicating coderate for low priority stream. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_SetCodeRates( tmFe_CodeRate_t highPrioRate, tmFe_CodeRate_t lowPrioRate, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  This function returns the actual frequency offset that is set in the hardware. */ 
/**  @param Offset Output Parameter returning the actual Frequency Offset of the transport stream. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_GetActualFrequencyOffset( tmFe_FrequencyOffset_t * Offset, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  This function returns the frequency offset. */ 
/**  @param Offset Output Parameter returning the Frequency Offset of the transport stream. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_GetFrequencyOffset( tmFe_FrequencyOffset_t * Offset, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  Sets the Frequency Offset. */ 
/**  @param offset Input parameter indicating the frequency offset to be set. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_SetFrequencyOffset( tmFe_FrequencyOffset_t offset, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  This function returns the supported frequency offsets. (The offset is the distance of the actual carrier frequency to the grid frequency. This is measured in 166 KHz steps (1/6 MHz). Within DVB-T it is required +/- 3 steps are supported. The frequency error is in Hz and is the difference between the local oscillator and the tuner.) */ 
/**  @param Offsets Output Parameter returning the supported Frequency Offsets. The value is a bit-mask that is constructed by OR-ing the values of enum #_tmFe_FrequencyOffset_t */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_GetSuppFrequencyOffsets( tmFe_FrequencyOffsetSet_t * Offsets, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  This function returns the actual guard interval (one of the guard interval parameters of this interface). If automatic detection is supported and enabled this function will return tmFe_GuardIntervalInvalid if the detection is busy and tmFe_GuardIntervalUnknown if the automatic detection was unsuccessful or has been aborted. */ 
/**  @param Interval Output parameter returning the actual Guard Interval. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_GetActualGuardInterval( tmFe_GuardInterval_t * Interval, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  Returns the GuardInterval as set via SetGuardInterval function. */ 
/**  @param Interval Output parameter returning the set Guard Interval. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_GetGuardInterval( tmFe_GuardInterval_t * Interval, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  This function sets the guard interval (one of the guard interval parameters of this interface) if it is supported. tmFe_GuardIntervalUnknown and tmFe_GuardIntervalInvalid cannot be set. */ 
/**  @param interval Input parameter indicating the new Guard Interval. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_SetGuardInterval( tmFe_GuardInterval_t interval, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  Returns the Guard Intervals supported. */ 
/**  @param Intervals Output parameter returning the Guard Intervals supported. The value is a bit-mask that is constructed by OR-ing the values of enum #_tmFe_GuardInterval_t. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_GetSuppGuardIntervals( tmFe_GuardIntervalSet_t * Intervals, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  This function returns the actual hierarchy mode (one of the hierarchy mode parameters of this interface). If automatic detection is supported and enabled this function will return tmFe_HierarchyModeInvalid if the detection is busy and tmFe_HierarchyModeUnknown if the automatic detection was unsuccessful or has been aborted. */ 
/**  @param Mode Output parameter returning the actual HierarchyMode. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_GetActualHierarchyMode( tmFe_HierarchyMode_t * Mode, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  Returns the HierarchyMode that was set via SetHierarchyMode function. */ 
/**  @param Mode Output parameter returning the HierarchyMode that was set via SetHierarchyMode. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_GetHierarchyMode( tmFe_HierarchyMode_t * Mode, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  This function sets the hierarchy mode (one of the hierarchy mode parameters of this interface) if it is supported. tmFe_HierarchyModeInvalid cannot be set. tmFe_HierarchyModeNone has to be used to select a non Hierarchial mode. */ 
/**  @param mode Input parameter indicating the HierarchyMode. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_SetHierarchyMode( tmFe_HierarchyMode_t mode, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  Returns the supported HierarchyModes. */ 
/**  @param Modes Output parameter returning supported HierarchyModes. The value is a bit-mask that is constructed by OR-ing the values of enum #_tmFe_HierarchyMode_t. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_GetSuppHierarchyModes( tmFe_HierarchyModeSet_t * Modes, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  This function returns the Stream priority of the transport stream. */ 
/**  @param streamPriority Output Parameter returning the Stream Priority of the transport stream. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_GetStreamPriority( tmFe_StreamPriority_t * streamPriority, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  Selects the decoded stream based on the requested priority. StreamPriority can take values  tmFe_StreamPriorityHigh and tmFe_StreamPriorityLow. */ 
/**  @param streamPriority Input Parameter indicating the Stream Priority to be set. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_SetStreamPriority( tmFe_StreamPriority_t streamPriority, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  This function returns the Transport Stream cell id. */ 
/**  @param cellId Output Parameter returning the transport stream cellid. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_GetTpsCellid( Nat16 * cellId, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  This function returns the actual transmission mode (one of the tx mode parameters of this interface). If automatic detection is supported and enabled this function will return tmFe_TxModeInvalid if the detection is busy and tmFe_TxModeUnknown if the automatic detection was unsuccessful or has been aborted. */ 
/**  @param Mode Output Parameter returning the actual TxMode. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_GetActualTxMode( tmFe_TxMode_t * Mode, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  Returns the transmission mode as set via the SetTxMode function. */ 
/**  @param Mode Output Parameter returning the set TxMode. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_GetTxMode( tmFe_TxMode_t * Mode, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  This function sets the transmission mode (one of the tx mode parameters of this interface) if it is supported. tmFe_TxModeInvalid and tmFe_TxModeUnknown cannot be set. */ 
/**  @param mode Input Parameter indicating the new Tx Mode. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_SetTxMode( tmFe_TxMode_t mode, Nat32 * retval );

/**  @ingroup papi_fe_chdecdvbt_Functions */
/**  Returns the supported Transmission Modes. */ 
/**  @param Modes Output parameter returning the supported Transmission Modes. The value is a bit-mask that is constructed by OR-ing the values of enum #_tmFe_TxMode_t. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecdvbt_GetSuppTxModes( tmFe_TxModeSet_t * Modes, Nat32 * retval );



/*
 *  interface chdecisdbt of type IPlfApiChanDecISDBT
 */

/** @defgroup papi_fe_chdecisdbt papi_fe_chdecisdbt: ChanDecISDBT
  * @ingroup papi_fe */

/** @defgroup papi_fe_chdecisdbt_Functions Functions 
  * @ingroup papi_fe_chdecisdbt */

/**  @ingroup papi_fe_chdecisdbt_Functions */
/**  Some channel decoders provide some settings that can be used to improve the zap speed. As all information with respect to the physical channels is owned and stored in the middleware, the platform needed an interface to the middleware. GetChannelInformation() is called during installation to collect this information and SetChannelInformation() is called during zapping to provide this information to speed up zapping. The information that is retrieved is not interpreted by the middleware and provided as-is to the platform. */ 
/**  @param info Output parameter returning channel information of the transport stream. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecisdbt_GetChannelInformation( Nat16 * info, Nat32 * retval );

/**  @ingroup papi_fe_chdecisdbt_Functions */
/**  This function sets channel information. See also GetChannelInformation(). */ 
/**  @param info Input parameter with the channel information to be set. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecisdbt_SetChannelInformation( Nat16 info, Nat32 * retval );

/**  @ingroup papi_fe_chdecisdbt_Functions */
/**  This function returns the actual code rate of indicated layer. If automatic detection is supported and enabled this function will return tmFe_CodeRateInvalid if the detection is busy and tmFe_CodeRateUnknown if the automatic detection was unsuccessful or has been aborted. */ 
/**  @param coderate Output parameter returning the actual coderate for selected layer. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecisdbt_GetActualCodeRates( int layer, tmFe_CodeRate_t * coderate, Nat32 * retval );

/**  @ingroup papi_fe_chdecisdbt_Functions */
/**  Returns the Code Rates that are supported. */ 
/**  @param Rates Output parameter returning coderates that are supported. The value is a bit-mask that is constructed by OR-ing the values of enum #_tmFe_CodeRate_t. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecisdbt_GetSuppCodeRates( tmFe_CodeRateSet_t * Rates, Nat32 * retval );

/**  @ingroup papi_fe_chdecisdbt_Functions */
/**  Returns the coderates for the indicated layer as they were set via the SetCodeRates function. */ 
/**  @param coderate Output parameter returning the set coderate for selected layer. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecisdbt_GetCodeRates( int layer, tmFe_CodeRate_t * coderate, Nat32 * retval );

/**  @ingroup papi_fe_chdecisdbt_Functions */
/**  This function sets the code rate (one of the code rate parameters of this interface) for a certain layer (one of the layer parameters of this interface). tmFe_CodeRateInvalid, tmFe_CodeRateUnknown and tmFe_CodeRateNone cannot be set. */ 
/**  @param layer The layer to which this function needs to be applied. */
/**  @param coderate Input parameter indicating coderate for selected layer. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecisdbt_SetCodeRates( int layer, tmFe_CodeRate_t coderate, Nat32 * retval );

/**  @ingroup papi_fe_chdecisdbt_Functions */
/**  Returns the actual constellation. */ 
/**  @param layer The layer to which this function needs to be applied. */
/**  @param pConstellation Output parameter returning the actual constellation. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecisdbt_GetActualConstellation( int layer, tmFe_Constellation_t * pConstellation, Nat32 * retval );

/**  @ingroup papi_fe_chdecisdbt_Functions */
/**  Returns the constellation. */ 
/**  @param layer The layer to which this function needs to be applied. */
/**  @param pConstellation Output parameter returning the constellation. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecisdbt_GetConstellation( int layer, tmFe_Constellation_t * pConstellation, Nat32 * retval );

/**  @ingroup papi_fe_chdecisdbt_Functions */
/**  Sets the constellation. */ 
/**  @param layer The layer to which this function needs to be applied. */
/**  @param constellation The constellation. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecisdbt_SetConstellation( int layer, tmFe_Constellation_t constellation, Nat32 * retval );

/**  @ingroup papi_fe_chdecisdbt_Functions */
/**  Returns the supported constellations. */ 
/**  @param pConstellations Output parameter returning the supported constellations. The value is a bit-mask that is constructed by OR-ing the values of enum #_tmFe_Constellation_t. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecisdbt_GetSuppConstellations( tmFe_ConstellationSet_t * pConstellations, Nat32 * retval );

/**  @ingroup papi_fe_chdecisdbt_Functions */
/**  This function returns the actual guard interval (one of the guard interval parameters of this interface). If automatic detection is supported and enabled this function will return tmFe_GuardIntervalInvalid if the detection is busy and tmFe_GuardIntervalUnknown if the automatic detection was unsuccessful or has been aborted. */ 
/**  @param Interval Output parameter returning the actual Guard Interval. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecisdbt_GetActualGuardInterval( tmFe_GuardInterval_t * Interval, Nat32 * retval );

/**  @ingroup papi_fe_chdecisdbt_Functions */
/**  Returns the GuardInterval as set via SetGuardInterval function. */ 
/**  @param Interval Output parameter returning the set Guard Interval. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecisdbt_GetGuardInterval( tmFe_GuardInterval_t * Interval, Nat32 * retval );

/**  @ingroup papi_fe_chdecisdbt_Functions */
/**  This function sets the guard interval (one of the guard interval parameters of this interface) if it is supported. tmFe_GuardIntervalUnknown and tmFe_GuardIntervalInvalid cannot be set. */ 
/**  @param interval Input parameter indicating the new Guard Interval. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecisdbt_SetGuardInterval( tmFe_GuardInterval_t interval, Nat32 * retval );

/**  @ingroup papi_fe_chdecisdbt_Functions */
/**  Returns the Guard Intervals supported. */ 
/**  @param Intervals Output parameter returning the Guard Intervals supported. The value is a bit-mask that is constructed by OR-ing the values of enum #_tmFe_GuardInterval_t. */
/**  @param retval Standard return value. */
extern FResult papi_fe_chdecisdbt_GetSuppGuardIntervals( tmFe_GuardIntervalSet_t * Intervals, Nat32 * retval );

/**  @ingroup papi_fe_chdecisdbt_Functions */
/**  Get the selected layer. */ 
/**  @param layer [out] parameter indicating the selected layer. */
extern FResult papi_fe_chdecisdbt_GetLayer( int * layer );

/**  @ingroup papi_fe_chdecisdbt_Functions */
/**  Select the layer. */ 
/**  @param layer Indicates the layer to be selected. */
extern FResult papi_fe_chdecisdbt_SetLayer( int layer );

/**  @ingroup papi_fe_chdecisdbt_Functions */
/**  Returns the supported layers. */ 
/**  @param layers OR'ing of supported layers. */
extern FResult papi_fe_chdecisdbt_GetSupportedLayers( int * layers );

#define papi_fe_chdecisdbt_LayerA 1
#define papi_fe_chdecisdbt_LayerB 2
#define papi_fe_chdecisdbt_LayerC 4


/*
 *  interface col of type IPlfApiColorDecode
 */

/** @defgroup papi_fe_col papi_fe_col: ColorDecode
  * In an analogue terrestrial television broadcast, RGB information is converted into YUV, and then coded into luminance (Y) and chrominance (C) information. These two are combined (together with synchronisation information) into a CVBS signal. A colour decoder separates CVBS into Y and C, and decodes the C signal into U and V colour difference signals. A matrix converts YUV to RGB. There are various standards for transmitting the colour information. The colour decoder can be forced to use a certain standard, or it can detect the standard automatically. In the latter case, the user of the interface can suggest a certain standard, to speed up the detection process
  * @ingroup papi_fe */

/** @defgroup papi_fe_col_Datatypes Datatypes 
  * @ingroup papi_fe_col */
/**
 *   @defgroup papi_fe_col_ColorSystem Color System Modes
 *   @ingroup papi_fe_col_Datatypes
 *   Supported levels of Color System Modes.
 *   @{ */
 /**  Color system unknown. */
#define papi_fe_col_UnknownColorSystem -1
 /**  Color system value during a transient (search or connect). This means that the colour decoder is not trying to detect the colour system, because the signal is not stable. */
#define papi_fe_col_InvalidColorSystem -2
#define papi_fe_col_Pal 0
#define papi_fe_col_Secam 1
#define papi_fe_col_Ntsc 2
/*   @} */

/**
 *   @defgroup papi_fe_col_Freq Color System Frequency
 *   @ingroup papi_fe_col_Datatypes
 *   Supported levels of Color System Frequencies.
 *   @{ */
 /**  Carrier frequency unknown. */
#define papi_fe_col_FreqUnknown -1
 /**  Carrier frequency value during a transient (search or connect). This means that the colour decoder is not trying to detect the colour system, because the signal is not stable. */
#define papi_fe_col_FreqInvalid -2
 /**  Carrier frequency of 4.433619 MHz */
#define papi_fe_col_Freq4pt433619 0
 /**  Carrier frequency of 3.582056 MHz */
#define papi_fe_col_Freq3pt582056 1
 /**  Carrier frequency of 3.575611 MHz */
#define papi_fe_col_Freq3pt575611 2
 /**  Carrier frequency of 3.579545 MHz */
#define papi_fe_col_Freq3pt579545 3
/*   @} */

/** @defgroup papi_fe_col_Functions Functions 
  * @ingroup papi_fe_col */

/**  @ingroup papi_fe_col_Functions */
/**  This function returns the actual carrier frequency being used. */ 
/**  @param retval The carrier frequency. See datatype \ref papi_fe_col_Freq. */
extern FResult papi_fe_col_GetCarrierFrequency( int * retval );

/**  @ingroup papi_fe_col_Functions */
/**  This function returns the actual colour system being decoded. */ 
/**  @param retval The colour system. See datatype \ref papi_fe_col_ColorSystem. */
extern FResult papi_fe_col_GetColorSystem( int * retval );

/**  @ingroup papi_fe_col_Functions */
/**  Informs an application whether the colour decoder supports a certain colour system in combination with a carrier frequency.<br>
Normally, a component data sheet will define which colour systems are supported. Applications that must run on different platforms can use this function to obtain this information at run time.<br>
It is not allowed to specify InvalidColorSystem or FreqInvalid as parameter.<br>
Specifying UnknownColorSystem is to be interpreted as checking for the supportedness of all valid colour systems in combination with the specified carrier frequency. Specifying FreqUnknown is to be interpreted similarly.<br>
So for example:<br>
<i>ColorSystemSupportedEx( UnknownColorSystem, Freq4pt433619 )</i><br>
is equivalent to<br>
<i>ColorSystemSupportedEx( Pal, Freq4pt433619 )<br>
|| ColorSystemSupportedEx( Secam, Freq4pt433619 )<br>
|| ColorSystemSupportedEx( Ntsc, Freq4pt433619 )</i><br>
 */ 
/**  @param colorsystem The colour system that may or may not be supported. See datatype \ref papi_fe_col_ColorSystem. */
/**  @param carrierfrequency The carrier frequency that may or may not be supported. See datatype \ref papi_fe_col_Freq. */
/**  @param retval TRUE if the colorsystem in combination with the carrier frequency is supported, FALSE otherwise. */
extern FResult papi_fe_col_ColorSystemSupportedEx( int colorsystem, int carrierfrequency, Bool * retval );

/**  @ingroup papi_fe_col_Functions */
/**  Tells whether the comb filter has been enabled and is active. */ 
/**  @param retval TRUE if the comb filter has been enabled and is active, FALSE otherwise. */
extern FResult papi_fe_col_CombFilterActive( Bool * retval );

/**  @ingroup papi_fe_col_Functions */
/**  This function enables or disables the use of the comb filter. If the comb filter is enabled but cannot be used on the current input signal, then the comb filter will be switched off. */ 
/**  @param mode TRUE to enable the comb filter, FALSE to disable its use. */
extern FResult papi_fe_col_EnableCombFilter( Bool mode );

/**  @ingroup papi_fe_col_Functions */
/**  Tells whether the comb filter has been enabled (see EnableCombFilter). */ 
/**  @param retval TRUE if the comb filter has been enabled, FALSE otherwise. */
extern FResult papi_fe_col_CombFilterEnabled( Bool * retval );

/**  @ingroup papi_fe_col_Functions */
/**  This function returns whether the comb filter is supported. */ 
/**  @param retval TRUE if comb filter is supported, FALSE otherwise. */
extern FResult papi_fe_col_CombFilterSupported( Bool * retval );

/**  @ingroup papi_fe_col_Functions */
/**  This function can be used to force the colour decoder to use the specified colour system in combination with a carrier frequency (even though the colour decoder may be able to detect by itself which colour system is being transmitted). */ 
/**  @param colorsystem The colour system to be used by the colour decoder. See datatype \ref papi_fe_col_ColorSystem. */
/**  @param carrierfrequency The carrier frequency to be used by the colour decoder. See datatype \ref papi_fe_col_Freq. */
extern FResult papi_fe_col_ForceColorSystemEx( int colorsystem, int carrierfrequency );

/**  @ingroup papi_fe_col_Functions */
/**  This function returns the carrier frequency that was previously set through function ForceColorSystemEx. */ 
/**  @param retval The forced carrier frequency.  See datatype \ref papi_fe_col_Freq. */
extern FResult papi_fe_col_GetForcedCarrierFrequency( int * retval );

/**  @ingroup papi_fe_col_Functions */
/**  This function returns the colour system that was previously set through function ForceColorSystem or ForceColorSystemEx. */ 
/**  @param retval The forced colour system. See datatype \ref papi_fe_col_ColorSystem. */
extern FResult papi_fe_col_GetForcedColorSystem( int * retval );

/**  @ingroup papi_fe_col_Functions */
/**  This function is used to suggest to the color decoder which colorsystem and carrier frequency to try first when it does automatic detection of the color system. It is only a suggestion: the color decoder may choose to ignore it.<br>
The function is typically invoked when zapping from one analog preset to another, to allow the platform to zap more quickly.<br>
<ul>
<li> It is not allowed to specify InvalidColorSystem or FreqInvalid as parameter.</li>
<li> UnknownColorSystem can be specified. The colour decoder will then try to determine the colour system by itself. </li>
<li> FreqUnknown can be specified. The colour decoder will then try to determine the carrier frequency by itself.</li>
<li> There is no guarantee that the color decoder will use the information: it may choose to ignore it.</li>
<li> Notify functions that report a change of colour system will not be called in those cases where the suggested colour system corresponds to the actual colour system.</li>
</ul>
. */ 
/**  @param colorsystem The colour system to be used by the colour decoder. See datatype \ref papi_fe_col_ColorSystem. */
/**  @param carrierfrequency The carrier frequency to be used by the colour decoder. See datatype \ref papi_fe_col_Freq. */
extern FResult papi_fe_col_SuggestColorSystemEx( int colorsystem, int carrierfrequency );

/**  @ingroup papi_fe_col_Functions */
/**  This function returns the carrier frequency that was previously set through function SuggestColorSystemEx. */ 
/**  @param retval The suggested carrier frequency. See datatype \ref papi_fe_col_Freq. */
extern FResult papi_fe_col_GetSuggestedCarrierFrequency( int * retval );

/**  @ingroup papi_fe_col_Functions */
/**  This function returns the colour system that was previously set through function SuggestColorSystem or SuggestColorSystemEx. */ 
/**  @param retval The suggested colour system. See datatype \ref papi_fe_col_ColorSystem. */
extern FResult papi_fe_col_GetSuggestedColorSystem( int * retval );



/*
 *  interface hsys of type IPlfApiSoundSystem
 */

/** @defgroup papi_fe_hsys papi_fe_hsys: SoundSystem
  * There are multiple instances of this interface:<br>
<ul>
<li> ssys is for speaker </li>
<li> hsys is for headphone </li>
</ul><br>
<b> Sound system modes </b></br></br>
The sound system modes are grouped:<br>
<ul>
<li> SoundPresent: Indicates whether or not a valid sound system is present.</li>
<li> IsAnalog, IsDigital, IsBaseband: Indicates if the decoded sound comes from an analogue, a digital or a base band source. It reflects the kind of audio received and does not specify whether the contents have been encoded analogue or digital. Baseband could for instance occur when the audio signal from a SCART input is being processed.</li>
<li> IsAM, IsFM: Indicates whether the decoded sound system comes from an AM or FM source.</li>
<li> IsNicam, Is2CS, IsBTSC, IsEIAJ, IsPCM, IsAC3, IsMPEG, IsDTS: Defines the various sound (encoding) systems. 2CS is "2 channel stereo". So two seperate FM modulated frequencies are used.</li>
<li> ContainsData, ContainsDual, ContainsUnrelated, ContainsDolbyPrologic, ContainsDynamicRangeCompression, ContainsKaraoke: This defines the properties applicable to the content that is being received. ContainsData, ContainsDual, ContainsUnrelated are NICAM specific and indicate the contents of the 2nd channel. It can contain 'Data', or contain a 2nd audio stream related to the first audio stream, e.g. a 2nd language ('Dual'), or it can even contain audio not related to the 1st audio stream like a voice over ('Unrelated').</li>
</ul>	.

  * @ingroup papi_fe */

/** @defgroup papi_fe_hsys_Datatypes Datatypes 
  * @ingroup papi_fe_hsys */
/**
 *   @defgroup papi_fe_hsys_SoundSystem Sound System Modes
 *   @ingroup papi_fe_hsys_Datatypes
 *   Supported levels of sound system modes. See also the front page for this interface for more information.
 *   @{ */
#define papi_fe_hsys_SoundPresent 1
#define papi_fe_hsys_IsAnalog 2
#define papi_fe_hsys_IsDigital 4
#define papi_fe_hsys_IsBaseband 8
#define papi_fe_hsys_IsAM 16
#define papi_fe_hsys_IsFM 32
#define papi_fe_hsys_IsNicam 64
#define papi_fe_hsys_Is2CS 128
#define papi_fe_hsys_IsBTSC 256
#define papi_fe_hsys_IsEIAJ 512
#define papi_fe_hsys_IsPCM 1024
#define papi_fe_hsys_IsAC3 2048
#define papi_fe_hsys_IsMPEG 4096
#define papi_fe_hsys_IsDTS 8192
#define papi_fe_hsys_ContainsData 16384
#define papi_fe_hsys_ContainsDual 32768
#define papi_fe_hsys_ContainsUnrelated 65536
#define papi_fe_hsys_ContainsDolbyProLogic 131072
#define papi_fe_hsys_ContainsDynamicRangeCompression 262144
#define papi_fe_hsys_ContainsKaraoke 524288
#define papi_fe_hsys_IsAC3Plus 1048576
#define papi_fe_hsys_IsHEAAC 2097152
/*   @} */

/** @defgroup papi_fe_hsys_Functions Functions 
  * @ingroup papi_fe_hsys */

/**  @ingroup papi_fe_hsys_Functions */
/**  This function returns the sound system mode as a logical OR of a number of defined constants. The bits in the return value only have a meaning if SoundPresent is set (0x00000001 is set). If SoundPresent is not set, then the sound system is absent / invalid. */ 
/**  @param retval Returns the sound system mode as a logical OR of a number of predefined constants. See datatype \ref papi_fe_hsys_SoundSystem. */
extern FResult papi_fe_hsys_GetSoundSystem( Nat32 * retval );

#define papi_fe_hsys_IsWMA 4194304


/*
 *  interface qssm of type IPlfApiSigStrengthMeas
 */

/** @defgroup papi_fe_qssm papi_fe_qssm: SigStrengthMeas
  * This interface is used to measure the strength of a signal. How and where in the signal path this measurement is performed is platform specific. There can be multiple instances of this interface to do different measurements at different positions in the signal path. The strength measurements that are reported via this interface is a logical representation of this measurement: the higher the value the stronger the signal. How the physical measurement is translated into the logical representation that is reported via this interface is up to the platform.<br>
Multiple instances of this interface have been defined:
<ul>
<li> ssm: signal strength measurement at the tuner level, used for both analog and digital</li>
<li> assm: signal strength measurement at the level of the channel decoder, used only for digital</li>
<li> qssm: signal strength measurement at the level of the channel decoder, used only for digital</li>
</ul>

  * @ingroup papi_fe */

/** @defgroup papi_fe_qssm_Functions Functions 
  * @ingroup papi_fe_qssm */

/**  @ingroup papi_fe_qssm_Functions */
/**  This function returns if the current signal strength measurement is valid. */ 
/**  @param Valid Output parameter returning if the current measurement is valid. */
/**  @param retval standard return value. */
extern FResult papi_fe_qssm_GetMeasValid( Bool * valid, Nat32 * retval );

/**  @ingroup papi_fe_qssm_Functions */
/**  Returns the minimum and maximum signal strength. */ 
/**  @param MinStrength Output parameter returning the minimum signal strength. */
/**  @param MaxStrength Output parameter returning the maximum signal strength. */
/**  @param retval standard return value. */
extern FResult papi_fe_qssm_GetSigStrengthRange( Nat32 * MinStrength, Nat32 * MaxStrength, Nat32 * retval );

/**  @ingroup papi_fe_qssm_Functions */
/**  This function returns the last measured signal strength. */ 
/**  @param Strength Output parameter returning the last measured signal strength. */
/**  @param retval standard return value. */
extern FResult papi_fe_qssm_GetSigStrength( Nat32 * Strength, Nat32 * retval );

/**  @ingroup papi_fe_qssm_Functions */
/**  This function starts the signal strength measurement. */ 
/**  @param Avail True, if the measurement is immediately available after this function returns. False, if availability of the measurement will be signalled asynchronously via a notification. */
/**  @param retval standard return value. */
extern FResult papi_fe_qssm_Start( Bool * Avail, Nat32 * retval );

/**  @ingroup papi_fe_qssm_Functions */
/**  This function stops the signal strength measurement. */ 
/**  @param Stopped True, if the measurement is indeed stopped after this function returns. False, if the measurement will be stopped asynchronously and signalled via a notification. */
/**  @param retval standard return value. */
extern FResult papi_fe_qssm_Stop( Bool * Stopped, Nat32 * retval );



/*
 *  interface snd of type IPlfApiSoundDecode
 */

/** @defgroup papi_fe_snd papi_fe_snd: SoundDecode
  * <b>Terrestrial Sound</b><br>
In an analogue terrestrial television broadcast, sound information can be coded in a variety of ways. Typical standards for coding sound are:
<ul>
<li> AM mono (as used in TV system L and L').</li>
<li> FM mono (as used in TV system BG and others).</li>
<li> 2CS (2 carrier sound), FM modulated signals which has two carriers. The second carrier contains either stereo information or a second (dual) sound channel.</li>
<li> NICAM, a digital sound standard, which can contain mono sound, stereo sound, dual sound or data, which is either related or unrelated to the image. NICAM is accompanied by an AM or FM mono signal.</li>
<li> BTSC, the US standard for sound, which can be mono or stereo, with an optional SAP (second audio program, always mono).</li>
</ul>
<br>The sound system should not be confused with the TV system. The latter deals with the location of the sound carriers, the former with the modulation and coding techniques. The sound and TV systems are more or less independent, though in practice certain combinations are fixed (such as AM with system L).
<br><br><b>Multichannel Sound</b><br>
Three methods are popular for encoding sound digitally. They are as follows:
<ul>
<li> MPEG x/y.z: ISO's Moving Picture Experts Group came up with this method of encoding audio digitally.</li>
<li> Dolby Digital DD x/y.z: The Dolby Laboratories of USA designed this method of sound encoding.</li>
<li>Pulse Code Modulation PCM: It consists of 2 front channels only.</li>
</ul><br>
In the above, x is the number of front channels (left, right, center, middle left/right, middle left/right can be supported by MPEG only), y is the number of surround channels (maximum two), z denotes the low frequency signal

  * @ingroup papi_fe */

/** @defgroup papi_fe_snd_Datatypes Datatypes 
  * @ingroup papi_fe_snd */
/**
 *   @defgroup papi_fe_snd_Allow Sound System
 *   @ingroup papi_fe_snd_Datatypes
 *   Bitmask for the various sound systems.
 *   @{ */
#define papi_fe_snd_AllowAnalog 1
#define papi_fe_snd_AllowDigital 2
#define papi_fe_snd_AllowNicam 4
#define papi_fe_snd_Allow2CS 8
#define papi_fe_snd_AllowBTSC 16
#define papi_fe_snd_AllowEIAJ 32
#define papi_fe_snd_AllowPCM 64
#define papi_fe_snd_AllowAC3 128
#define papi_fe_snd_AllowMPEG 256
#define papi_fe_snd_AllowDTS 512
/*   @} */

/** @defgroup papi_fe_snd_Functions Functions 
  * @ingroup papi_fe_snd */

/**  @ingroup papi_fe_snd_Functions */
/**  Returns the set of sound standards supported by the decoder. */ 
/**  @param retval Return value with the set of sound standards supported by the decoder OR-ed. See datatype \ref papi_fe_snd_Allow. */
extern FResult papi_fe_snd_GetAllowedSoundSystem( int * retval );

/**  @ingroup papi_fe_snd_Functions */
/**  Sets the set of sound standards that the decoder is allowed to detect. */ 
/**  @param system The set of sound standards that the decoder is allowed to detect OR-ed. See datatype \ref papi_fe_snd_Allow. */
extern FResult papi_fe_snd_AllowSoundSystem( int system );



/*
 *  interface ssm of type IPlfApiSigStrengthMeas
 */

/** @defgroup papi_fe_ssm papi_fe_ssm: SigStrengthMeas
  * This interface is used to measure the strength of a signal. How and where in the signal path this measurement is performed is platform specific. There can be multiple instances of this interface to do different measurements at different positions in the signal path. The strength measurements that are reported via this interface is a logical representation of this measurement: the higher the value the stronger the signal. How the physical measurement is translated into the logical representation that is reported via this interface is up to the platform.<br>
Multiple instances of this interface have been defined:
<ul>
<li> ssm: signal strength measurement at the tuner level, used for both analog and digital</li>
<li> assm: signal strength measurement at the level of the channel decoder, used only for digital</li>
<li> qssm: signal strength measurement at the level of the channel decoder, used only for digital</li>
</ul>

  * @ingroup papi_fe */

/** @defgroup papi_fe_ssm_Functions Functions 
  * @ingroup papi_fe_ssm */

/**  @ingroup papi_fe_ssm_Functions */
/**  This function returns if the current signal strength measurement is valid. */ 
/**  @param Valid Output parameter returning if the current measurement is valid. */
/**  @param retval standard return value. */
extern FResult papi_fe_ssm_GetMeasValid( Bool * Valid, Nat32 * retval );

/**  @ingroup papi_fe_ssm_Functions */
/**  Returns the minimum and maximum signal strength. */ 
/**  @param MinStrength Output parameter returning the minimum signal strength. */
/**  @param MaxStrength Output parameter returning the maximum signal strength. */
/**  @param retval standard return value. */
extern FResult papi_fe_ssm_GetSigStrengthRange( Nat32 * MinStrength, Nat32 * MaxStrength, Nat32 * retval );

/**  @ingroup papi_fe_ssm_Functions */
/**  This function returns the last measured signal strength. */ 
/**  @param Strength Output parameter returning the last measured signal strength. */
/**  @param retval standard return value. */
extern FResult papi_fe_ssm_GetSigStrength( Nat32 * Strength, Nat32 * retval );

/**  @ingroup papi_fe_ssm_Functions */
/**  This function starts the signal strength measurement. */ 
/**  @param Avail True, if the measurement is immediately available after this function returns. False, if availability of the measurement will be signalled asynchronously via a notification. */
/**  @param retval standard return value. */
extern FResult papi_fe_ssm_Start( Bool * Avail, Nat32 * retval );

/**  @ingroup papi_fe_ssm_Functions */
/**  This function stops the signal strength measurement. */ 
/**  @param Stopped True, if the measurement is indeed stopped after this function returns. False, if the measurement will be stopped asynchronously and signalled via a notification. */
/**  @param retval standard return value. */
extern FResult papi_fe_ssm_Stop( Bool * Stopped, Nat32 * retval );



/*
 *  interface ssys of type IPlfApiSoundSystem
 */

/** @defgroup papi_fe_ssys papi_fe_ssys: SoundSystem
  * There are multiple instances of this interface:<br>
<ul>
<li> ssys is for speaker </li>
<li> hsys is for headphone </li>
</ul><br>
<b> Sound system modes </b></br></br>
The sound system modes are grouped:<br>
<ul>
<li> SoundPresent: Indicates whether or not a valid sound system is present.</li>
<li> IsAnalog, IsDigital, IsBaseband: Indicates if the decoded sound comes from an analogue, a digital or a base band source. It reflects the kind of audio received and does not specify whether the contents have been encoded analogue or digital. Baseband could for instance occur when the audio signal from a SCART input is being processed.</li>
<li> IsAM, IsFM: Indicates whether the decoded sound system comes from an AM or FM source.</li>
<li> IsNicam, Is2CS, IsBTSC, IsEIAJ, IsPCM, IsAC3, IsMPEG, IsDTS: Defines the various sound (encoding) systems. 2CS is "2 channel stereo". So two seperate FM modulated frequencies are used.</li>
<li> ContainsData, ContainsDual, ContainsUnrelated, ContainsDolbyPrologic, ContainsDynamicRangeCompression, ContainsKaraoke: This defines the properties applicable to the content that is being received. ContainsData, ContainsDual, ContainsUnrelated are NICAM specific and indicate the contents of the 2nd channel. It can contain 'Data', or contain a 2nd audio stream related to the first audio stream, e.g. a 2nd language ('Dual'), or it can even contain audio not related to the 1st audio stream like a voice over ('Unrelated').</li>
</ul>	.

  * @ingroup papi_fe */

/** @defgroup papi_fe_ssys_Datatypes Datatypes 
  * @ingroup papi_fe_ssys */
/**
 *   @defgroup papi_fe_ssys_SoundSystem Sound System Modes
 *   @ingroup papi_fe_ssys_Datatypes
 *   Supported levels of sound system modes. See also the front page for this interface for more information.
 *   @{ */
#define papi_fe_ssys_SoundPresent 1
#define papi_fe_ssys_IsAnalog 2
#define papi_fe_ssys_IsDigital 4
#define papi_fe_ssys_IsBaseband 8
#define papi_fe_ssys_IsAM 16
#define papi_fe_ssys_IsFM 32
#define papi_fe_ssys_IsNicam 64
#define papi_fe_ssys_Is2CS 128
#define papi_fe_ssys_IsBTSC 256
#define papi_fe_ssys_IsEIAJ 512
#define papi_fe_ssys_IsPCM 1024
#define papi_fe_ssys_IsAC3 2048
#define papi_fe_ssys_IsMPEG 4096
#define papi_fe_ssys_IsDTS 8192
#define papi_fe_ssys_ContainsData 16384
#define papi_fe_ssys_ContainsDual 32768
#define papi_fe_ssys_ContainsUnrelated 65536
#define papi_fe_ssys_ContainsDolbyProLogic 131072
#define papi_fe_ssys_ContainsDynamicRangeCompression 262144
#define papi_fe_ssys_ContainsKaraoke 524288
#define papi_fe_ssys_IsAC3Plus 1048576
#define papi_fe_ssys_IsHEAAC 2097152
/*   @} */

/** @defgroup papi_fe_ssys_Functions Functions 
  * @ingroup papi_fe_ssys */

/**  @ingroup papi_fe_ssys_Functions */
/**  This function returns the sound system mode as a logical OR of a number of defined constants. The bits in the return value only have a meaning if SoundPresent is set (0x00000001 is set). If SoundPresent is not set, then the sound system is absent / invalid. */ 
/**  @param retval Returns the sound system mode as a logical OR of a number of predefined constants. See datatype \ref papi_fe_ssys_SoundSystem. */
extern FResult papi_fe_ssys_GetSoundSystem( Nat32 * retval );

#define papi_fe_ssys_IsWMA 4194304


/*
 *  interface stcdec of type IPlfApiStcDec
 */

/** @defgroup papi_fe_stcdec papi_fe_stcdec: StcDec
  * The STC decoder maintains the System Time Clock which is derived from the PCR values inside a digital MPEG stream.
  * @ingroup papi_fe */

/** @defgroup papi_fe_stcdec_Functions Functions 
  * @ingroup papi_fe_stcdec */

/**  @ingroup papi_fe_stcdec_Functions */
/**  Return the current value of the STC. This clock is a 33 bit value running at 90KHz based on the incoming PCRs. */ 
/**  @param Value Output parameter returning the current value of the clock. */
/**  @param retval Standard Return Value. */
extern FResult papi_fe_stcdec_GetTime( tmTimeStamp_t * Value, Nat32 * retval );



/*
 *  interface tsdmx of type IPlfApiTsDmxAbstract
 */

/** @defgroup papi_fe_tsdmx papi_fe_tsdmx: TsDmxAbstract
  * This document defines the Transport Stream Demultiplexing logical component. An instance of this logical component is referred to as a TS Demux and represents functionality to filter specific content (e.g. PES packets, sections) from a transport stream (TS). The functionality can be controlled by means of the interfaces specified in this document.
A TS Demux consists of:
<ul>
<li>	A number of PCR Filters that filter Program Clock Reference (PCR) data from the input transport stream and route it to a System Time Clock recovery mechanism.</li>
<li>	A number of Descramblers that descramble streams whose content has been protected by the content provider through scrambling.</li>
<li>	A number of Streams that model stream(s) of data of a particular type. Each Stream is either a Client Stream or a Platform Stream. A Client Stream is a Stream for which the data is routed to a client while a Platform Stream is a Stream for which the data is routed to an appropriate (with respect to the type of data, e.g. audio decoder for audio data) decoder or a remultiplexer.</li>
<li>	A TS Demultiplexer, this is where PCR Filters, Descramblers and Streams can be allocated.</li>
<li>	A number of PID Filters that extract transport packets with a given PID from a transport stream.</li>
<li>	A number of TPH Filters that extract transport packets from a transport stream. Besides filtering on a PID, it allows to filter on other fields in transport packet headers.</li>
<li>	A number of Section Filters that filter sections from the output of PID Filter objects they are associated with.</li>
<li>	A number of PES Filters that filter PES packets from the output of PID or TPH Filter objects they are associated with.</li>
<li>	A number of PES Notifiers that signal the reception of PES packets from the output of PID or TPH Filter objects they are associated with. A PES Notifier does not provide filter criteria, it just acts as a notifier of PES packets to clients.</li>
</ul><br><br>
For more information on demultiplexing see also the UHAPI documentation on Transport Stream Demultiplexing.<br>
  * @ingroup papi_fe */

/** @defgroup papi_fe_tsdmx_Datatypes Datatypes 
  * @ingroup papi_fe_tsdmx */
/**
 *   @defgroup papi_fe_tsdmx_MediaType Media Types
 *   @ingroup papi_fe_tsdmx_Datatypes
 *   Supported levels of Media Types.
 *   @{ */
#define papi_fe_tsdmx_MediaTypeAudio 1
#define papi_fe_tsdmx_MediaTypeVideo 2
#define papi_fe_tsdmx_MediaTypeAudioDesc 3
/*   @} */

/**
 *   @defgroup papi_fe_tsdmx_Filter Filter
 *   @ingroup papi_fe_tsdmx_Datatypes
 *   Filter Constants.
 *   @{ */
#define papi_fe_tsdmx_FilterDepth 16
#define papi_fe_tsdmx_InvalidSubscrId -1
/*   @} */

/**
 *   @defgroup papi_fe_tsdmx_Scrambling Descrambler Modes
 *   @ingroup papi_fe_tsdmx_Datatypes
 *   Supported levels of Descrambler Modes.
 *   @{ */
#define papi_fe_tsdmx_ScramblingUnknown 1
#define papi_fe_tsdmx_NotScrambled 2
#define papi_fe_tsdmx_Scrambled 3
/*   @} */

/**
 *   @defgroup papi_fe_tsdmx_StreamType Stream Types
 *   @ingroup papi_fe_tsdmx_Datatypes
 *   Supported stream Types.
 *   @{ */
#define papi_fe_tsdmx_StreamTypeInvalid 0
#define papi_fe_tsdmx_StreamTypemp1v 1
#define papi_fe_tsdmx_StreamTypemp2v 2
#define papi_fe_tsdmx_StreamTypemp4v 3
#define papi_fe_tsdmx_StreamTypemp1a 4
#define papi_fe_tsdmx_StreamTypemp2a 5
#define papi_fe_tsdmx_StreamTypemp4a 6
#define papi_fe_tsdmx_StreamTypettxt 7
#define papi_fe_tsdmx_StreamTypesubt 8
#define papi_fe_tsdmx_StreamTypepcr 9
#define papi_fe_tsdmx_StreamTypeac3 10
#define papi_fe_tsdmx_StreamTypeh264 11
#define papi_fe_tsdmx_StreamTypedivx 12
#define papi_fe_tsdmx_StreamTypevc1 13
#define papi_fe_tsdmx_StreamTypeaac 14
#define papi_fe_tsdmx_StreamTypeheaac 15
#define papi_fe_tsdmx_StreamTypewma 16
#define papi_fe_tsdmx_StreamTypeddplus 17
#define papi_fe_tsdmx_StreamTypedts 18
#define papi_fe_tsdmx_StreamTypemmw 19
#define papi_fe_tsdmx_StreamTypemma 20
#define papi_fe_tsdmx_StreamTypeOther 21
/*   @} */

/**
 *   @defgroup papi_fe_tsdmx_CrcMode CRC Modes
 *   @ingroup papi_fe_tsdmx_Datatypes
 *   Supported levels of CRC Modes.
 *   @{ */
#define papi_fe_tsdmx_CrcModeSkipBadSections 1
#define papi_fe_tsdmx_CrcModeAcceptAllSections 2
#define papi_fe_tsdmx_CrcModeDontCheckCrc 3
/*   @} */

/** @defgroup papi_fe_tsdmx_Functions Functions 
  * @ingroup papi_fe_tsdmx */

/**  @ingroup papi_fe_tsdmx_Functions */
/**  Returns the Audio Descriptive packet ID that was set. */ 
/**  @param pid Output parameter returning packet ID. */
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_GetAudioDescriptivePid( Nat16 * pid, Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  Sets Audio Descriptive packet ID. It refers to a descriptive audio stream (extra audio PID). The stream is mixed with the regular audio stream. This also starts the pid filter. */ 
/**  @param pid Input parameter containing packet ID. */
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_SetAudioDescriptivePid( Nat16 pid, Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  Asynchronous request to stop the audio descriptive pid filter. Completion is signalled via a corresponding notification function. */ 
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_StopAudioDescriptivePid( Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  Returns the Audio packet ID that was set. */ 
/**  @param pid Output parameter returning packet ID. */
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_GetAudioPid( Nat16 * pid, Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  This sets the Audio packet ID that are filtered by the demultiplexer and routed inside the platform towards the MPEG/AC3 audio decoder. Setting the audio pid, starts audio pid filtering and audio decoding. */ 
/**  @param pid Input parameter containing packet ID. */
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_SetAudioPid( Nat16 pid, Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  Asynchronous request to stop the audio pid filter. Completion is signalled via a corresponding notification function. */ 
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_StopAudioPid( Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  Returns the audio stream type. */ 
/**  @param streamtype Output parameter returning the audio stream type */
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_GetAudioStreamType( int * streamtype, Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  Sets the audio stream type. */ 
/**  @param streamtype The audio stream type */
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_SetAudioStreamType( int streamtype, Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  CancelFilter() will:
<ul>
<li> stop and release the section filter. </li>
<li> stop and release the corresponding PID filter, if it is not used otherwise. </li>
</ul><br><br>
. */ 
/**  @param subscrId The subscription ID which was returned by RequestFilter(). */
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_CancelFilter( Nat32 subscrId, Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  CancelListFilter() will:
<ul>
<li> stop and release the section filter. </li>
<li> stop and release the corresponding PID filter, if it is not used otherwise.</li>
</ul><br><br>
. */ 
/**  @param subscrId The subscription ID which was returned by RequestFilter(). */
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_CancelListFilter( Nat32 subscrId, Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  CancelPes() will:
<ul>
<li> stop and release the PES filter.</li>
<li> stop and release the corresponding PID filter, if it is not used otherwise.</li>
</ul><br><br>
. */ 
/**  @param subscrId The subscription ID which was returned by RequestPes(). */
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_CancelPes( Nat32 subscrId, Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  Frees up the memory for the section list buffer. It means the client has finished processing the data */ 
/**  @param subsrId Identification of the section. */
/**  @param listcookie Cookie that was passed in the notification. */
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_FreeListBuffer( Nat32 subscrId, Nat32 listcookie, Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  Frees up the memory for the PES buffer. It means the client has finished processing the data */ 
/**  @param subsrId Identification of the PES filter. */
/**  @param datacookie Cookie that was passed in the notification. */
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_FreePesBuffer( Nat32 subscrId, Nat32 datacookie, Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  Frees up the memory for the section buffer. It means the client has finished processing the data */ 
/**  @param subsrId Identification of the section. */
/**  @param datacookie Cookie that was passed in the notification. */
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_FreeSectionBuffer( Nat32 subscrId, Nat32 datacookie, Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  Indicates wether the audio stream type is supported. */ 
/**  @param streamtype The audio stream type */
/**  @param supported Output parameter returning the supportedness. */
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_IsAudioStreamTypeSupported( int streamtype, Bool * supported, Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  Indicates wether the pcr stream type is supported. */ 
/**  @param streamtype The pcr stream type */
/**  @param supported Output parameter returning the supportedness. */
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_IsPcrStreamTypeSupported( int streamtype, Bool * supported, Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  This function is used to detected whether requests can be made to the demux. */ 
/**  @param retval Indicates whether requests can be made to the demux. */
extern FResult papi_fe_tsdmx_IsRequestAllowed( Bool * retval );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  Indicates wether the video stream type is supported. */ 
/**  @param streamtype The video stream type */
/**  @param supported Output parameter returning the supportedness. */
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_IsVideoStreamTypeSupported( int streamtype, Bool * supported, Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  Gets PCR (Program Clock Reference) packet ID. */ 
/**  @param pid Output parameter returning packet ID. */
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_GetPcrPid( Nat16 * pid, Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  Sets PCR packet ID. It also starts the pid filter. */ 
/**  @param pid Input parameter containing packet ID. */
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_SetPcrPid( Nat16 pid, Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  Asynchronous request to stop the PCR pid filter. Completion is signalled via a corresponding notification function. */ 
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_StopPcrPid( Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  Returns the pcr stream type. */ 
/**  @param streamtype Output parameter returning the pcr stream type */
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_GetPcrStreamType( int * streamtype, Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  Sets the pcr stream type. */ 
/**  @param streamtype The pcr stream type */
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_SetPcrStreamType( int streamtype, Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  RequestFilter() will:
<ul>
<li> allocate and start a PID filter for the specified PID (if no filter has been created on this PID yet).</li>
<li> allocate a section filter on this PID filter, including setting of the crc mode, filter mode (continuous or one-shot), and setting of the mode value, match value, and mask value of the section filter.</li>
<li> start the section filter.</li>
</ul><br><br>
In one-shot filter mode, the section filter will automatically stop, if one section has been filtered and notified to the client. In continuous mode, it has to be stopped explicitly by the client, otherwise it will continue notifying the arrival of new sections. */ 
/**  @param pid PID of the transport streams packets that carry the required section. */
/**  @param BufferSize Input parameter representing buffer size. */
/**  @param CrcMode Input parameter representing CRC mode. */
/**  @param cookie Cookie that will be returned on every notification of a section. */
/**  @param ContinuousMode Start the filter in continuous mode (TRUE) or in one-shot mode (FALSE). */
/**  @param FilterDepth Input parameter representing filter depth. */
/**  @param ModeArray The mode value for the section filter. It indicates per bit whether positive filtering ('0') or negative filter ('1') should be applied to the corresponding bit in the data. */
/**  @param MatchArray The match value for the section filter. */
/**  @param MaskArray The mask value for the section filter. */
/**  @param error Output parameter returning the error code. */
/**  @param retval Returns the subscription ID corresponding to this request. This ID is used for updating, parsing and canceling this request. */
extern FResult papi_fe_tsdmx_RequestFilter( int pid, int BufferSize, int CrcMode, Bool ContinuousMode, Nat32 cookie, int FilterDepth, Nat8 * ModeArray, Nat8 * MatchArray, Nat8 * MaskArray, Nat32 * error, Nat32 * retval );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  RequestListFilter() will:
<ul>
<li> allocate and start a PID filter for the specified PID (if no filter has been created on this PID yet).</li>
<li> allocate a section filter on this PID filter, including setting of the crc mode, filter mode (continuous or one-shot), and setting of the mode value, match value, and mask value of the section filter.</li>
<li> start the section filter.</li>
</ul><br><br>
In one-shot filter mode, the section filter will automatically stop, if one section has been filtered and notified to the client. In continuous mode, it has to be stopped explicitly by the client, otherwise it will continue notifying the arrival of new sections. */ 
/**  @param pid PID of the transport streams packets that carry the required section. */
/**  @param BufferSize Input parameter representing buffer size. */
/**  @param CrcMode Input parameter representing CRC mode. */
/**  @param cookie Cookie that will be returned on every notification of a section. */
/**  @param ContinuousMode Start the filter in continuous mode (TRUE) or in one-shot mode (FALSE). */
/**  @param timeout Defines the timeout after which any received section data will be notified, in case nothing is received anymore */
/**  @param listLength The maximum number of sections in the list */
/**  @param FilterDepth Input parameter representing filter depth. */
/**  @param ModeArray The mode value for the section filter. It indicates per bit whether positive filtering ('0') or negative filter ('1') should be applied to the corresponding bit in the data. */
/**  @param MatchArray The match value for the section filter. */
/**  @param MaskArray The mask value for the section filter. */
/**  @param error Output parameter returning the error code. */
/**  @param retval Returns the subscription ID corresponding to this request. This ID is used for updating, parsing and canceling this request. */
extern FResult papi_fe_tsdmx_RequestListFilter( int pid, int BufferSize, int CrcMode, Bool ContinuousMode, Nat32 timeout, Nat32 listLength, Nat32 cookie, int FilterDepth, Nat8 * ModeArray, Nat8 * MatchArray, Nat8 * MaskArray, Nat32 * error, Nat32 * retval );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  RequestPes() will:
<ul>
<li> allocate and start a PID filter for the specified PID (if no filter has been created on this PID yet).</li>
<li> allocate a PES (Packetized Elementary Stream) filter subscription on the requested PID with the specified match and mask values.</li>
<li> start the PES filter.</li>
</ul><br><br>
. */ 
/**  @param pid The PID of the TS packets that carry the required section. */
/**  @param BufferSize Input parameter representing buffer size. */
/**  @param cookie Cookie that will be returned on every notification of a PES. */
/**  @param FilterDepth Input parameter representing filter depth. */
/**  @param MatchArray The match value for the PES filter. */
/**  @param MaskArray The mask value for the PES filter. */
/**  @param error Output parameter returning error code. */
/**  @param retval Returns the subscription ID corresponding to this request. This ID is used for updating, parsing and canceling this request. */
extern FResult papi_fe_tsdmx_RequestPes( int pid, int BufferSize, Nat32 cookie, int FilterDepth, Nat8 * MatchArray, Nat8 * MaskArray, Nat32 * error, Nat32 * retval );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  Returns whether the current stream - filtered by the PID or TPH Filter object on which this interface operates - is scrambled. */ 
/**  @param MediaType Input parameter media type. */
/**  @param error Output parameter returning error code. */
/**  @param retval Output parameter returning scrambling status. */
extern FResult papi_fe_tsdmx_GetScramblingStatus( int MediaType, Nat32 * error, int * retval );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  This function is called to update a section filter that has been created via RequestFilter(). It will:
<ul>
<li> Set the mode value, match value, and mask value of the section filter.</li>
<li> Start the section filter.</li>
</ul><br><br>
A filter running in continuous mode will be updated and continue to run. A filter that was started in one-shot mode and which has stopped after notifying one section, will be updated and started anew in one-shot mode. */ 
/**  @param subscrId The subscription ID which was returned by RequestFilter(). */
/**  @param FilterDepth Input parameter representing filter depth. */
/**  @param ModeArray The mode value for the section filter. */
/**  @param MatchArray The match value for the section filter. */
/**  @param MaskArray The mask value for the section filter. */
/**  @param error Output parameter returning the error code. */
/**  @param retval Returns TRUE if the section filter parameters were successfully updated and FALSE otherwise. */
extern FResult papi_fe_tsdmx_UpdateFilter( Nat32 subscrId, int FilterDepth, Nat8 * ModeArray, Nat8 * MatchArray, Nat8 * MaskArray, Nat32 * error, Bool * retval );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  This function is called to update a section list filter that has been created via RequestListFilter(). It will:
<ul>
<li> Set the mode value, match value, and mask value of the section filter.</li>
<li> Start the section filter.</li>
</ul><br><br>
A filter running in continuous mode will be updated and continue to run. A filter that was started in one-shot mode and which has stopped after notifying one section, will be updated and started anew in one-shot mode. */ 
/**  @param subscrId The subscription ID which was returned by RequestFilter(). */
/**  @param FilterDepth Input parameter representing filter depth. */
/**  @param ModeArray The mode value for the section filter. */
/**  @param MatchArray The match value for the section filter. */
/**  @param MaskArray The mask value for the section filter. */
/**  @param error Output parameter returning the error code. */
/**  @param retval Returns TRUE if the section filter parameters were successfully updated and FALSE otherwise. */
extern FResult papi_fe_tsdmx_UpdateListFilter( Nat32 subscrId, int FilterDepth, Nat8 * ModeArray, Nat8 * MatchArray, Nat8 * MaskArray, Nat32 * error, Bool * retval );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  Returns the Video packet ID that was set. */ 
/**  @param pid Output parameter returning packet ID. */
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_GetVideoPid( Nat16 * pid, Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  This set the video packet ID that are filtered by the demultiplexer and routed inside the platform towards the video decoder. Setting the video pid, starts video pid filtering and video decoding. */ 
/**  @param pid Input parameter containing packet ID. */
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_SetVideoPid( Nat16 pid, Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  Asynchronous request to stop the video pid filter. Completion is signalled via a corresponding notification function. */ 
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_StopVideoPid( Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  Returns the video stream type. */ 
/**  @param streamtype Output parameter returning the video stream type */
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_GetVideoStreamType( int * streamtype, Nat32 * error );

/**  @ingroup papi_fe_tsdmx_Functions */
/**  Sets the video stream type. */ 
/**  @param streamtype The video stream type */
/**  @param error Output parameter returning error code. */
extern FResult papi_fe_tsdmx_SetVideoStreamType( int streamtype, Nat32 * error );



/*
 *  interface tun of type IPlfApiTuning
 */

/** @defgroup papi_fe_tun papi_fe_tun: Tuning
  * <b> Analogue and Digital </b><br>
The tuning interface is used both for analogue and digital broadcast reception. Before using this interface, the client of the platform will have selected the tuner as a video and/or audio source using the function SelectVideoSource()/SelectAudioSource() in the Source module. With the designator parameter in the same function, the client also indicates whether an analogue or digital use case is selected. If a client of the platform calls SelectVideoSource()/SelectAudioSource() anew in the Source module, the use of the tuning interface for either analogue or digital broadcast reception can also change anew. <br>
<b> Frequencies and channel numbers </b> <br>
The 'frequency' denotes an exact position in a frequency band. A more abstract positioning is based upon channel numbers, where a 'channel' denotes a particular frequency including predefined lower and upper bound frequencies. The mapping of channel numbers to frequencies can be different per region or country. The channel table interface papi_fe_chantab provides means to select the regional or country specific channel table. <br>
Frequencies can be freely selected between minimum and maximum frequency, whereas channel tables provide a non-consecutive list (containing gaps) of possible channel numbers between minimum and maximum channel. So the frequency boundary limits are mainly depending on the tuner capabilities, whereas the selected channel table defines the channel boundary limits. <br>
<b> Searching </b> <br>
The tuner is also able to search for stations. The search process typically includes:
<ul>
<li> a 'coarse search' for a station (sweeping over frequencies or channels) </li>
<li> a 'fine tune' action on the station (around a frequency or within a channel) </li>
<li> a search for the 'TV system' </li>
</ul>
.
The search and tune operations result in a notification, notifying whether a station could or could not be found. The TV system is notified separately. <br>
<b> Automatic Frequency Control - AFC </b><br>
Through AFC, the tuner continuously updates its frequency to track a possibly drifting transmitter (such as a VCR). AFC takes place in a predefined window, which is a configurable setting of the platform. Notifications are raised when the frequency changes significantly and if the limits of this window are reached. Note that no station found/not found notifications are raised. <br>
<b> TV system detection </b><br>
The platform stays with the selected TV system unless the client of the platform sets a different TV system explicitly or triggers automatic TV system detection. Automatic TV system detection can be triggered in two ways:
<ul>
<li> if StartSearch() is called with TvSystemIncluded set, or </li>
<li> if SetTvSystem() is called with parameter tmFe_TvSystemNone. </li>
</ul>
.
TV system detection is always single shot: after a notification of the TV system that is detected, a client of the platform has to do a new request to start TV system detection anew. Without an explicit request to start TV detection, any change in TV system will not be detected by the platform as it assumes that the TV system remains unchanged. <br>
Any need for an initial or default (country-specific) TV system will be handled by an explict call to pApi. <br>
For additional information, see also the UHAPI Specification on Tuning.
  * @ingroup papi_fe */

/** @defgroup papi_fe_tun_Functions Functions 
  * @ingroup papi_fe_tun */

/**  @ingroup papi_fe_tun_Functions */
/**  This function enables or disables automatic frequency control (AFC). AFC is used by the tuner to track a drifting transmitter in a window around a set point frequency. AFC cannot be enabled while a search is in progress. During AFC, 'AFC frequency notification' notifications are raised if the actual frequency changes because of AFC, e.g. per minimal step of 1/16 MHz. The size of the window is platform implementation specific. If the AFC window is reached, an 'AFC limit reached' notification is called, but AFC continues to track if the carrier returns inside the AFC window again. */ 
/**  @param enable True to enable AFC. False to disable AFC. */
/**  @param retval Standard return value. */
extern FResult papi_fe_tun_EnableAfc( Bool enable, Nat32 * retval );

/**  @ingroup papi_fe_tun_Functions */
/**  This function returns if AFC (Automatic Frequency Control) is enabled. */ 
/**  @param Enabled True, if AFC is enabled. False, if not. */
/**  @param retval Standard return value. */
extern FResult papi_fe_tun_GetAfcEnabled( Bool * Enabled, Nat32 * retval );

/**  @ingroup papi_fe_tun_Functions */
/**  This function returns if AFC (Automatic Frequency Control) is supported. */ 
/**  @param Supp True, if AFC is supported. False, if not. */
/**  @param retval Standard return value. */
extern FResult papi_fe_tun_GetAfcSupp( Bool * Supp, Nat32 * retval );

/**  @ingroup papi_fe_tun_Functions */
/**  This function returns the position of the currently tuned frequency with regard to the IF frequency. */ 
/**  @param Value Returns AFC value. It is a value of enum #_tmFe_AfcValue_t. */
/**  @param retval Standard return value. */
extern FResult papi_fe_tun_GetAfcValue( tmFe_AfcValue_t * Value, Nat32 * retval );

/**  @ingroup papi_fe_tun_Functions */
/**  This function is used to obtain the actual channel to which the tuner is tuned. This channel can be the result of a SetFreq(), a SetChan(), or search action. */ 
/**  @param Chan Returns the actual channel. */
/**  @param retval Standard return value. */
extern FResult papi_fe_tun_GetChan( Nat32 * Chan, Nat32 * retval );

/**  @ingroup papi_fe_tun_Functions */
/**  Returns the minimum and maximum channel offset. */ 
/**  @param MinOffset Returns the minimum channel offset. */
/**  @param MaxOffset Returns the maximum channel offset. */
/**  @param retval Standard return value. */
extern FResult papi_fe_tun_GetChanOffsetRange( Nat32 * MinOffset, Nat32 * MaxOffset, Nat32 * retval );

/**  @ingroup papi_fe_tun_Functions */
/**  This function returns the channel offset that was set using SetChan(). If it was invalidated by an SetFreq(), StartSearch(), or EnableAfc() call, then #tmFe_NoOffset is returned. */ 
/**  @param Offset Returns channel offset. */
/**  @param retval Standard return value. */
extern FResult papi_fe_tun_GetChanOffset( Nat32 * Offset, Nat32 * retval );

/**  @ingroup papi_fe_tun_Functions */
/**  This function tunes to the specified channel, defined by the selected channel table, with a certain offset or fine-tune action.
- The offset will be converted to frequency steps. The step size is defined in units of 1/16th MHz.
- SetChan() will generate an 'on station found' or an 'on station not found' notification depending on whether a station is detected or not (within a predefined amount of time determined by the platform).
- SetChan() will stop any channel search in progress with no further notification.
- SetChan() will stop any AFC process with the proper notification.
- SetChan() will set the tuning operation to 'channel mode' instead of 'frequency mode', which also influences the search and AFC calls that follow.
- if fine-tuning is set, then the tuner performs a micro search around the frequency - defined by the channel - for a station within a (platform defined) window.
.
. */ 
/**  @param chan The channel to which the tuner is to be tuned. */
/**  @param offset Indicates whether an offset or fine-tuning action has to be executed. A value between the minimum and maximum channel offset indicates an offset. #tmFe_NoOffset indicates no fine-tuning. #tmFe_AutoOffset indicates fine-tuning. */
/**  @param retval Standard return value. */
extern FResult papi_fe_tun_SetChan( Nat32 chan, Nat32 offset, Nat32 * retval );

/**  @ingroup papi_fe_tun_Functions */
/**  This function returns the minimum and maximum frequency in units of 1/16th MHz that can be handled by the tuner. */ 
/**  @param MinFreq Returns the minimum frequency in units of 1/16th MHz. */
/**  @param MaxFreq Returns the maximum frequency in units of 1/16th MHz. */
/**  @param retval Standard return value. */
extern FResult papi_fe_tun_GetFreqRange( Nat32 * MinFreq, Nat32 * MaxFreq, Nat32 * retval );

/**  @ingroup papi_fe_tun_Functions */
/**  This function is used to obtain the actual tuner frequency to which the tuner is tuned. This frequency can be the result of a SetFreq(), a SetChan(), or search action. */ 
/**  @param Freq Returns actual frequency in units of 1/16th MHz. */
/**  @param retval Standard return value. */
extern FResult papi_fe_tun_GetFreq( Nat32 * Freq, Nat32 * retval );

/**  @ingroup papi_fe_tun_Functions */
/**  This function sets the tuner frequency in units of 1/16 MHz. If finetune is set, the set frequency is followed by a fine tune action.
- SetFreq() will generate an 'on station found' or an 'on station not found' notification depending on whether a station is detected or not (within a predefined amount of time determined by the platform).
- SetFreq() will stop any channel search in progress with no further notification.
- SetFreq() will stop any AFC process with the proper notification.
- SetFreq() will set the tuning operation to 'frequency mode' instead of 'channel mode', which also influences the search and AFC calls that follow.
.
Regarding fine tuning:
- For analogue, if finetune is set, then the tuner performs a micro search around the frequency for a station within a (platform defined) window. This is also default behaviour in StartSearch().
- For analogue, if finetune is not set, the tuner is set precisely to the frequency specified.
- For digital, if finetune is set, frequency offset detection takes place for DVB-T. This is also default behaviour in StartSearch().
- For digital, if finetune is not set, no frequency offset detection takes place for DVB-T.
.
. */ 
/**  @param freq The frequency to which the tuner is to be tuned in units of 1/16 MHz. */
/**  @param finetune Indicates whether a fine tune action needs to be executed. */
/**  @param retval Standard return value. */
extern FResult papi_fe_tun_SetFreq( Nat32 freq, Bool finetune, Nat32 * retval );

/**  @ingroup papi_fe_tun_Functions */
/**  This function returns whether the specified search mode is supported. */ 
/**  @param mode Input parameter with the specified search mode. The value is a bit-mask that is constructed by OR-ing the values of enum #_tmFe_SearchMode_t. */
/**  @param Supp True, if this particular search mode is supported. False, if not. */
/**  @param retval Standard return value. */
extern FResult papi_fe_tun_GetSearchModeSupp( tmFe_SearchMode_t mode, Bool * Supp, Nat32 * retval );

/**  @ingroup papi_fe_tun_Functions */
/**  This function returns the tuner search mode. */ 
/**  @param Mode Returns the search mode. The value is a bit-mask that is constructed by OR-ing the values of enum #_tmFe_SearchMode_t. */
/**  @param retval Standard return value. */
extern FResult papi_fe_tun_GetSearchMode( tmFe_SearchMode_t * Mode, Nat32 * retval );

/**  @ingroup papi_fe_tun_Functions */
/**  This function starts the tuner search starting from the current frequency or channel. The search mode is set using the mode parameter. The value is a bit-mask that is constructed by OR-ing the values of enum #_tmFe_SearchMode_t. <br>
The default search mode is: a frequency search, only for negative modulated signals, skip the transmission that is on the current frequency or channel, search towards the maximum frequency, no TV system decoding required, no colour system decoding required, no sound system decoding required. By setting additional bits in the mode parameter the behaviour can be changed, see enum #_tmFe_SearchMode_t. <br>
Part of the default search mode behaviour is also that fine tuning takes place when a transmission is found during analog search and frequency offset detection takes place for DVB-T.
- StartSearch() will generate an 'on station found' or an 'on station not found' notification depending on whether a station is detected or not, unless it is cancelled by a call to SetFreq(), SetChan(), or StopSearch().
- StartSearch() will stop any AFC process with the proper notification.
- It is the client responsibility to select only supported modes and to ensure that a search is stopped before it is called anew.
.
. */ 
/**  @param mode Input parameter with the specified search mode. The value is a bit-mask that is constructed by OR-ing the values of enum #_tmFe_SearchMode_t. */
/**  @param endFreq The frequency or channel after which the search should stop. If the search is upwards and the endFreq is below the current frequency or channel, then the search will wrap around. Similarly, if the search is downwards and the endFreq is higher than the current frequency or channel, then the search will also wrap around. */
/**  @param retval Standard return value. */
extern FResult papi_fe_tun_StartSearch( tmFe_SearchMode_t mode, Nat32 endFreq, Nat32 * retval );

/**  @ingroup papi_fe_tun_Functions */
/**  This function returns the tuner search state. */ 
/**  @param State Returns the search state. The value is a bit-mask that is constructed by OR-ing the values of enum #_tmFe_SearchState_t. */
/**  @param retval Standard return value. */
extern FResult papi_fe_tun_GetSearchState( tmFe_SearchState_t * State, Nat32 * retval );

/**  @ingroup papi_fe_tun_Functions */
/**  The search will be aborted without an 'on station found' or 'on station not found' notification. The tuner stays tuned to the last frequency or channel set by the search process. */ 
/**  @param retval Standard return value. */
extern FResult papi_fe_tun_StopSearch( Nat32 * retval );

/**  @ingroup papi_fe_tun_Functions */
/**  This function returns if the tuner is tuned to a valid station. */ 
/**  @param StationPresent True, if it is tuned to a valid station. False, if not. */
/**  @param retval Standard return value. */
extern FResult papi_fe_tun_GetStationPresent( Bool * StationPresent, Nat32 * retval );

/**  @ingroup papi_fe_tun_Functions */
/**  This function returns whether the specified TV system is supported. */ 
/**  @param tvSystem Input parameter with the specified TV system. */
/**  @param Supp True, if supported. False, if not. */
/**  @param retval Standard return value. */
extern FResult papi_fe_tun_GetTvSystemSupp( tmFe_TvSystem_t tvSystem, Bool * Supp, Nat32 * retval );

/**  @ingroup papi_fe_tun_Functions */
/**  This function gets the current TV system. */ 
/**  @param TvSystem Returns the current TV system. */
/**  @param retval Standard return value. */
extern FResult papi_fe_tun_GetTvSystem( tmFe_TvSystem_t * TvSystem, Nat32 * retval );

/**  @ingroup papi_fe_tun_Functions */
/**  This function sets the current TV system. If the system is called with #tmFe_TvSystemNone, then the tuner will start a TV system detection followed by a 'TV system detected' notification. Any tuner search in progress is stopped by this call. */ 
/**  @param TvSystem Input parameter current tv system. It is a value of enum #_tmFe_TvSystem_t. */
/**  @param retval Standard return value. */
extern FResult papi_fe_tun_SetTvSystem( tmFe_TvSystem_t TvSystem, Nat32 * retval );



/*
 *  interface vbisl of type IPlfApiVbiSlice
 */

/** @defgroup papi_fe_vbisl papi_fe_vbisl: VbiSlice
  * The VBI Slicer extracts VBI data from VBI lines. There are basically two ways of extracting VBI data:
- A client of the platform can explicitly set the VBI lines for which it is interested in using SetVbiLines() in which case the slicer needs to extract data only from those lines.
- The other option is Auto Slicing, wherein every VBI data type is looked for (sliced for) by the VBI slicer and the platform sends notification (OnWstData, OnWssData, etc.) if any of the data is in the transmission. On enabling Auto slicing, the client does not need to explicitly specify which VBI lines it is interested in. <br><br><br>
VBI usage:
- PAL/SECAM: Wide screen signalling (Wss), World standard teletext (Wst), and Video programming system (Vps)
- NTSC: Closed Captioning (Cc) and Copy Generation Management System (CGMS)
- HD (720/1080): Copy Generation Management System (CGMS)
.
For additional information, see also the UHAPI Specification on VBI Slicing.
  * @ingroup papi_fe */

/** @defgroup papi_fe_vbisl_Datatypes Datatypes 
  * @ingroup papi_fe_vbisl */
/**
 *   @defgroup papi_fe_vbisl_VbiSliceError Error codes VbiSlice
 *   @ingroup papi_fe_vbisl_Datatypes
 *   Error codes for VbiSlice.
 *   @{ */
#define papi_fe_vbisl_VbiSliceLinesNotSet 2049
/*   @} */

/**
 *   @defgroup papi_fe_vbisl_VbiSlice VBI slicer modes
 *   @ingroup papi_fe_vbisl_Datatypes
 *   Supported levels of VBI slicer modes.
 *   @{ */
 /**  Closed caption */
#define papi_fe_vbisl_Cc 1
 /**  Video programming system */
#define papi_fe_vbisl_Vps 2
 /**  Wide screen signalling */
#define papi_fe_vbisl_Wss 4
#define papi_fe_vbisl_Gemstar 8
 /**  Copy Generation Management System - Analog (CGMS-A), see also: http://en.wikipedia.org/wiki/CGMS-A */
#define papi_fe_vbisl_Cgms 16
 /**  World standard teletext */
#define papi_fe_vbisl_Wst 32
/*   @} */

/** @defgroup papi_fe_vbisl_Functions Functions 
  * @ingroup papi_fe_vbisl */

/**  @ingroup papi_fe_vbisl_Functions */
/**  Enable or disable auto slicing. When auto slicing is enabled, the platform will scan all VBI lines and give corresponding notifications (OnWstData, OnVpsData, ...) for all data that is detected. */ 
/**  @param enable True, to enable auto slicing. False, to disable auto slicing. */
/**  @param retval Standard return value. */
extern FResult papi_fe_vbisl_EnableAutoSlicing( Bool enable, Nat32 * retval );

/**  @ingroup papi_fe_vbisl_Functions */
/**  This function has not effect on the VBI slicer itself. There is an additional mask on top of the notifications that results from SetVbiLines() which can be set via this interface. Only the notifications for the data types that have also been set via this function will indeed be signalled towards the platform. It has no influence on the notifications resulting from auto slicing. */ 
/**  @param subscriptions Input parameter indicating the datatypes for which the notifications should be passed. The value is a bit-mask that is constructed by OR-ing the VBI data type values. See datatype \ref papi_fe_vbisl_VbiSlice. */
/**  @param retval Standard return value. */
extern FResult papi_fe_vbisl_SetSlicermode( Nat32 subscriptions, Nat32 * retval );

/**  @ingroup papi_fe_vbisl_Functions */
/**  Returns whether auto slicing is enabled. */ 
/**  @param Enabled True, if auto slicing is enabled. False, if not. */
/**  @param retval Standard return value. */
extern FResult papi_fe_vbisl_GetAutoSlicingEnabled( Bool * Enabled, Nat32 * retval );

/**  @ingroup papi_fe_vbisl_Functions */
/**  Corresponding Get-function to retrieve the data that has been set towards the platform using the SetVbiLines() function. */ 
/**  @param size Input parameter indicating the maximum number of entries that can be stored in the Lines and pDataTypes arrays, i.e. the amount of data has been allocated by the client for arrays pointed to by Lines and pDataTypes. */
/**  @param ActNrVbiLines Output parameter indicating the actual number of relevant elements stored by the platform in the arrays Lines and pDataTypes, which are in the range from 0 to ActNrVbiLines-1. */
/**  @param Lines Pointer to the first element of an array with the VBI lines to be sliced. Each element in the array specifies a particular VBI line. The corresponding element in the pDataTypes array specifies the VBI data types that had to be sliced on this particular VBI line. */
/**  @param pDataTypes Pointer to the first element of an array with the VBI data types to be sliced in. pDataTypes[i] contains the VBI data types to be sliced for the VBI line that is defined in Lines[i]. The value is a bit-mask that is constructed by OR-ing the values of enum #_tmVbiSlice2_VbiData_t. */
/**  @param retval Standard return value. */
extern FResult papi_fe_vbisl_GetVbiLines( Nat16 size, Nat32 * ActNrVbiLines, Nat32 * Lines, tmVbiSlice2_VbiDataSet_t * pDataTypes, Nat32 * retval );

/**  @ingroup papi_fe_vbisl_Functions */
/**  This function is used to set explicitly which VBI data types should be sliced from which VBI line. Auto slicing has to be disabled for this function to have effect. The input for this function is basically a list with two columns, pLines and DataTypes and 'nrVbiLines' entries long. For each entry i, pLines[i] defines a particular VBI line and DataTypes[i] defines the VBI datatypes that have to be sliced on this particular VBI line. */ 
/**  @param nrVbiLines The length of the list, in other words the number of elements in the arrays pLines and DataTypes. */
/**  @param pLines Pointer to the first element of an array which defines the VBI lines that need to be sliced. Each element in the array specifies a particular VBI line. The corresponding element in the DataTypes array specifies the VBI data types that have to be sliced on this particular VBI line. */
/**  @param DataTypes Pointer to the first element of an array with the VBI data types to be sliced in. DataTypes[i] contains the VBI data types to be sliced for the VBI line that is defined in pLines[i]. The value is a bit-mask that is constructed by OR-ing the values of enum #_tmVbiSlice2_VbiData_t. */
/**  @param retval Return value Ok or VBI Lines Not Set See datatype \ref papi_fe_vbisl_VbiSliceError. */
extern FResult papi_fe_vbisl_SetVbiLines( Nat16 nrVbiLines, Nat32 * pLines, tmVbiSlice2_VbiDataSet_t * DataTypes, Nat32 * retval );



/*
 *  interface vip of type IPlfApiVideoProperties
 */

/** @defgroup papi_fe_vip papi_fe_vip: VideoProperties
  * The video properties interfaces provides information on the incoming video signal. After video presence detected, additional properties can be detected about the type of signal.
  * @ingroup papi_fe */

/** @defgroup papi_fe_vip_Datatypes Datatypes 
  * @ingroup papi_fe_vip */
/**
 *   @defgroup papi_fe_vip_FieldFrequency Field frequency detected
 *   @ingroup papi_fe_vip_Datatypes
 *   Indicates if there is a valid value for the field frequency.
 *   @{ */
 /**  Field frequency is not detected yet, field frequency detection is in progress. */
#define papi_fe_vip_FieldFrequencyInvalid -1
 /**  Field frequency detection is finished, but field frequency could not be determined. */
#define papi_fe_vip_FieldFrequencyUnknown 0
/*   @} */

/**
 *   @defgroup papi_fe_vip_SignalPresence Signal presence detected
 *   @ingroup papi_fe_vip_Datatypes
 *   Values used to indicate if there is a signal present; which may or may not carry video.
 *   @{ */
#define papi_fe_vip_SignalPresenceInvalid -1
#define papi_fe_vip_SignalPresenceUnknown 0
#define papi_fe_vip_SignalPresenceDetected 1
#define papi_fe_vip_SignalPresenceNotDetected 2
/*   @} */

/**
 *   @defgroup papi_fe_vip_VideoPresence Video presence detected
 *   @ingroup papi_fe_vip_Datatypes
 *   Values used to indicate if there is video present. When there is video present there is also a signal present.
 *   @{ */
 /**  Video presence is not detected yet, presence detecion is in progress. */
#define papi_fe_vip_VideoPresenceInvalid -1
 /**  Video presence detection is finished, but presence could not be determined, e.g. unstable reception conditions. */
#define papi_fe_vip_VideoPresenceUnknown 0
 /**  Video presence detected, there is a valid video signal. */
#define papi_fe_vip_VideoPresenceDetected 1
 /**  No video presence detected, there is no valid video signal. */
#define papi_fe_vip_VideoPresenceNotDetected 2
/*   @} */

/**
 *   @defgroup papi_fe_vip_NumberOfLines Number of lines detected
 *   @ingroup papi_fe_vip_Datatypes
 *   Indicates if there is a valid value for the number of lines; i.e. the vertical resolution.
 *   @{ */
 /**  Number of lines is not detected yet, number of lines detection is in progress. */
#define papi_fe_vip_NumberOfLinesInvalid -1
 /**  Number of lines detection is finished, but number of lines could not be determined. */
#define papi_fe_vip_NumberOfLinesUnknown 0
/*   @} */

/**
 *   @defgroup papi_fe_vip_VideoCoding Video coding detected
 *   @ingroup papi_fe_vip_Datatypes
 *   The possible video codings.
 *   @{ */
 /**  Video coding is not detected yet, video coding detection is in progress. */
#define papi_fe_vip_VideoCodingInvalid -1
 /**  Video coding detection is finished, but video coding could not be determined. */
#define papi_fe_vip_VideoCodingUnknown 0
 /**  Video coding detected as CVBS. */
#define papi_fe_vip_VideoCodingCvbs 1
 /**  Video coding detected as YC. */
#define papi_fe_vip_VideoCodingYc 2
 /**  Video coding detected as YUV with 1fH horizontal scan frequency. */
#define papi_fe_vip_VideoCodingYuv 3
 /**  Video coding detected as YUV with 2fH horizontal scan frequency. */
#define papi_fe_vip_VideoCodingYuv2FH 4
 /**  Video coding detected as YUV with 3fH horizontal scan frequency. */
#define papi_fe_vip_VideoCodingYuv3FH 5
 /**  Video coding detected as RGB with 1fH horizontal scan frequency. */
#define papi_fe_vip_VideoCodingRgb 6
 /**  Video coding detected as RGB with 2fH horizontal scan frequency. */
#define papi_fe_vip_VideoCodingRgb2FH 7
 /**  Video coding detected as RGB with 3fH horizontal scan frequency. */
#define papi_fe_vip_VideoCodingRgb3FH 8
 /**  Video coding detected as YPbPr with 1fH horizontal scan frequency. */
#define papi_fe_vip_VideoCodingYPbPr 9
 /**  Video coding detected as YPbPr with 2fH horizontal scan frequency. */
#define papi_fe_vip_VideoCodingYPbPr2FH 10
 /**  Video coding detected as YPbPr with 3fH horizontal scan frequency. */
#define papi_fe_vip_VideoCodingYPbPr3FH 11
/*   @} */

/**
 *   @defgroup papi_fe_vip_ImageFormat Image format detected
 *   @ingroup papi_fe_vip_Datatypes
 *   The various image formats that can be detected.
 *   @{ */
 /**  Image format is not detected yet, image format detection is in progress. */
#define papi_fe_vip_ImageFormatInvalid -1
 /**  Image format detection is finished, but image format could not be determined. */
#define papi_fe_vip_ImageFormatUnknown 0
 /**  Image format detected as 4:3 aspect ratio, no black bars. */
#define papi_fe_vip_ImageFormat4by3FullFormat 1
 /**  Image format detected as 4:3 aspect ratio, the lower 2/14th part of the image is black. */
#define papi_fe_vip_ImageFormat14by9Top 2
 /**  Image format detected as 4:3 aspect ratio, the lower and upper 1/14th part of the image is black. */
#define papi_fe_vip_ImageFormat14by9Center 3
 /**  Image format detected as 4:3 aspect ratio, the lower 4/16th part of the image is black. */
#define papi_fe_vip_ImageFormat16by9Top 4
 /**  Image format detected as 4:3 aspect ratio, the lower and upper 2/16th part of the image is black. */
#define papi_fe_vip_ImageFormat16by9Center 5
 /**  Image format detected as 4:3 aspect ratio, no black bars, but a 14:9 centre window should contain all relevant picture content. */
#define papi_fe_vip_ImageFormat4by3Use14by9 6
 /**  Image format detected as 16:9 aspect ratio, no black bars, but a 14:9 centre window should contain all relevant picture content. */
#define papi_fe_vip_ImageFormat16by9Use14by9 7
 /**  Image format detected as 16:9 aspect ratio, no black bars, but a 4:3 centre window should contain all relevant picture content. */
#define papi_fe_vip_ImageFormat16by9Use4by3 8
 /**  Image format detected as 16:9 aspect ratio, no black bars. */
#define papi_fe_vip_ImageFormat16by9FullFormat 9
 /**  Image format detected as 4:3 aspect ratio, the lower and upper part (> 2/16th) of the image is black. */
#define papi_fe_vip_ImageFormatMoreThan16by9Center 10
/*   @} */

/**
 *   @defgroup papi_fe_vip_VideoProtection Video protection type detected
 *   @ingroup papi_fe_vip_Datatypes
 *   Possible video protection schemes that may be applied to an incoming signal.
 *   @{ */
 /**  Video protection type is not detected yet, video protection type detection is in progress. */
#define papi_fe_vip_VideoProtectionInvalid -1
 /**  Video protection type detection is finished, but video protection type could not be determined. */
#define papi_fe_vip_VideoProtectionUnknown 0
 /**  Copy-protected signal has been detected. It may be playable, but not be provided for recording on any of the outputs. */
#define papi_fe_vip_VideoProtectionCopyProtected 1
 /**  Scramble protection signal has been detected. The signal may need to be descrambled via an (external) decoder before it is playable. */
#define papi_fe_vip_VideoProtectionScrambled 2
 /**  Macrovision (TM) copy protection has been detected. */
#define papi_fe_vip_VideoProtectionMacroVision 3
/*   @} */

/**
 *   @defgroup papi_fe_vip_MacroVision Macrovision protection detected
 *   @ingroup papi_fe_vip_Datatypes
 *   The possible protection modes with Macrovision (TM).
 *   @{ */
#define papi_fe_vip_MacroVisionInvalid -1
#define papi_fe_vip_MacroVisionUnknown 0
#define papi_fe_vip_MacroVisionAgc 1
#define papi_fe_vip_MacroVisionColorstripeType2 2
#define papi_fe_vip_MacroVisionColorstripeType3 3
/*   @} */

/**
 *   @defgroup papi_fe_vip_Interlace Interlace mode detected
 *   @ingroup papi_fe_vip_Datatypes
 *   The possible interlace modes.
 *   @{ */
 /**  Interlace is not detected yet, interlace detection is in progress. */
#define papi_fe_vip_InterlaceInvalid -1
 /**  Interlace detection is finished, but interlace could not be determined. */
#define papi_fe_vip_InterlaceUnknown 0
 /**  Interlace one-one has been detected, also known as progressive scan. */
#define papi_fe_vip_InterlaceOneOne 1
 /**  Interlace two-one has been detected, also known as interlaced scan. The starting position of the first line in a field alternates every 'one' time in a series of 'two' fields. It alternates between the top left and the top middle position every field. */
#define papi_fe_vip_InterlaceTwoOne 2
 /**  Interlace four-two has been detected, also known as interlace scan. The starting position of the first line alternates every 'two' times in a series of 'four' fields. It alternates between the top left and the top middle position every two fields. */
#define papi_fe_vip_InterlaceFourTwo 3
#define papi_fe_vip_ScanTypeInterlaced 1
/*   @} */

/** @defgroup papi_fe_vip_Functions Functions 
  * @ingroup papi_fe_vip */

/**  @ingroup papi_fe_vip_Functions */
/**  Returns the field frequency in Hz. */ 
/**  @param retval Output parameter returning field frequency */
extern FResult papi_fe_vip_GetFieldFrequency( int * retval );

/**  @ingroup papi_fe_vip_Functions */
/**  This function returns the image format. */ 
/**  @param retval Output parameter returning the image format. See datatype \ref papi_fe_vip_ImageFormat. */
extern FResult papi_fe_vip_GetImageFormat( int * retval );

/**  @ingroup papi_fe_vip_Functions */
/**  Returns if the signal is interlaced. */ 
/**  @param retval Scantype of the signal. See datatype \ref papi_fe_vip_Interlace. */
extern FResult papi_fe_vip_GetInterlace( int * retval );

/**  @ingroup papi_fe_vip_Functions */
/**  This function returns if the signal is film. */ 
/**  @param retval Output parameter returning if the signal is film or not. */
extern FResult papi_fe_vip_IsFilmMode( Bool * retval );

/**  @ingroup papi_fe_vip_Functions */
/**  This function returns if the signal from a VCR. */ 
/**  @param retval Output parameter returning if the signal is from a VCR or not. */
extern FResult papi_fe_vip_IsFromVcr( Bool * retval );

/**  @ingroup papi_fe_vip_Functions */
/**  This function indicates wether the current signal has a PC resolution. */ 
/**  @param retval Output parameter returning if the signal has PC resolution or not. */
extern FResult papi_fe_vip_IsPCFormat( int pixels, int lines, int scantype, Bool * retval );

/**  @ingroup papi_fe_vip_Functions */
/**  This function returns the macro vision type. */ 
/**  @param retval Output parameter returning the Macrovision protection type. See datatype \ref papi_fe_vip_MacroVision. */
extern FResult papi_fe_vip_GetMacroVisionType( int * retval );

/**  @ingroup papi_fe_vip_Functions */
/**  Returns the total number of lines per frame. */ 
/**  @param retval Output parameter returning the total number of lines per frame. */
extern FResult papi_fe_vip_GetNumberOfFrameLines( int * retval );

/**  @ingroup papi_fe_vip_Functions */
/**  Returns the visible (active) number of lines per frame. */ 
/**  @param retval The visible (active) number of lines per frame. */
extern FResult papi_fe_vip_GetNumberOfVisibleLines( int * retval );

/**  @ingroup papi_fe_vip_Functions */
/**  Returns the visible (active) number of pixels per line. */ 
/**  @param retval The visible (active) number of pixels per line. */
extern FResult papi_fe_vip_GetNumberOfVisiblePixels( int * retval );

/**  @ingroup papi_fe_vip_Functions */
/**  Returns whether a video signal is present. */ 
/**  @param retval Output parameter returning video signal. See datatype \ref papi_fe_vip_SignalPresence. */
extern FResult papi_fe_vip_GetSignalPresent( int * retval );

/**  @ingroup papi_fe_vip_Functions */
/**  This function returns the video coding. */ 
/**  @param retval Output parameter returning the video coding. See datatype \ref papi_fe_vip_VideoCoding. */
extern FResult papi_fe_vip_GetVideoCoding( int * retval );

/**  @ingroup papi_fe_vip_Functions */
/**  Returns whether video is present. */ 
/**  @param retval Output parameter returning video presence. See datatype \ref papi_fe_vip_VideoPresence. */
extern FResult papi_fe_vip_GetVideoPresent( int * retval );

/**  @ingroup papi_fe_vip_Functions */
/**  This function returns the video protection. It is mainly Macrovision related, so only for analog video signals. Digital protection is handled via information from the demux in the middleware. */ 
/**  @param retval Output parameter returning the video protection.  See datatype \ref papi_fe_vip_VideoProtection. */
extern FResult papi_fe_vip_GetVideoProtection( int * retval );

#define papi_fe_vip_ScanTypeUnknown 0
#define papi_fe_vip_ScanTypeProgressive 2


/*
 *  interface assmN of type IPlfApiSigStrengthMeasNotify
 */

/** @defgroup papi_fe_assm papi_fe_assm: SigStrengthMeas
  * @ingroup papi_fe */

/** @defgroup papi_fe_assmN_Notifications Notifications 
  * @ingroup papi_fe_assm */

/**  @ingroup papi_fe_assmN_Notifications */
/**  This notification function is called when a signal strength measurement is stopped. */ 
extern void papi_fe_assmN_OnMeasAborted( void );

/**  @ingroup papi_fe_assmN_Notifications */
/**  This notification function is called when a signal strength measurement is ready. */ 
/**  @param strength The signal strength. */
extern void papi_fe_assmN_OnMeasReady( Nat32 strength );



/*
 *  interface chdec2N of type IPlfApiChanDecNotify
 */

/** @defgroup papi_fe_chdec2 papi_fe_chdec2: ChanDec
  * @ingroup papi_fe */

/** @defgroup papi_fe_chdec2N_Notifications Notifications 
  * @ingroup papi_fe_chdec2 */

/**  @ingroup papi_fe_chdec2N_Notifications */
/**  Notifies subscribed clients that the current BER crossed a Ber Threshold. */ 
/**  @param ber Input parameter returning the BER in units of 10-8 */
/**  @param berThreshold Input parameter returning the Ber threshold that was crossed in units of 10-8 */
extern void papi_fe_chdec2N_OnBerThresholdCrossed( Nat32 ber, Nat32 berThreshold );

/**  @ingroup papi_fe_chdec2N_Notifications */
/**  This notification is given by the platform whenever there is a change in the presence of a carrier in the channel decoder/digital demodulator. Presence of a carrier means that the demodulator has been able to lock onto valid data. */ 
/**  @param carrier True, if carrier presence has been detected. False, if carrier presence is lost. */
extern void papi_fe_chdec2N_OnCarrierPresentChanged( Bool carrier );

/**  @ingroup papi_fe_chdec2N_Notifications */
/**  Notifies subscribed clients that the Channel Bandwidth has been determined. */ 
/**  @param bandwith Input parameter indicating the detected channel bandwidth. */
extern void papi_fe_chdec2N_OnChanBandwidthDetected( tmFe_ChanBandwidth_t bandwith );

/**  @ingroup papi_fe_chdec2N_Notifications */
/**  Notifies subscribed clients that a CodeRate has been determined. In case only one bit stream is received, this is considered the high priority stream and the coderate for the low priority stream is set to tmFe_CodeRateNone. */ 
/**  @param highPrioRate Input parameter indicating the detected High Priority coderate. */
/**  @param lowPrioRate Input parameter indicating the detected Low Priority coderate. */
extern void papi_fe_chdec2N_OnCodeRateDetected( tmFe_CodeRate_t highPrioRate, tmFe_CodeRate_t lowPrioRate );

/**  @ingroup papi_fe_chdec2N_Notifications */
/**  Notifies subscribed clients when a constellation is detected. */ 
/**  @param constellation Input parameter indicating the detected constellation. */
extern void papi_fe_chdec2N_OnConstellationDetected( tmFe_Constellation_t constellation );

/**  @ingroup papi_fe_chdec2N_Notifications */
/**  Notifies subscribed clients that the Guard Interval has been determined. */ 
/**  @param interval Input parameter indicating the detected GuardInterval. */
extern void papi_fe_chdec2N_OnGuardIntervalDetected( tmFe_GuardInterval_t interval );

/**  @ingroup papi_fe_chdec2N_Notifications */
/**  Notifies subscribed clients that the HierarchyMode has been determined. */ 
/**  @param mode Input parameter indicating the detected Hierarchy Mode. */
extern void papi_fe_chdec2N_OnHierarchyModeDetected( tmFe_HierarchyMode_t mode );

/**  @ingroup papi_fe_chdec2N_Notifications */
/**  Notifies subscribed clients that a IqMode has been determined. */ 
/**  @param mode Input parameter indicating the detected IqMode. */
extern void papi_fe_chdec2N_OnIqModeDetected( tmFe_IqMode_t mode );

/**  @ingroup papi_fe_chdec2N_Notifications */
/**  Notifies subscribed clients that the TxMode has been determined. */ 
/**  @param mode Input parameter indicating the detected TxMode. */
extern void papi_fe_chdec2N_OnTxModeDetected( tmFe_TxMode_t mode );



/*
 *  interface chdecN of type IPlfApiChanDecNotify
 */

/** @defgroup papi_fe_chdec papi_fe_chdec: ChanDec
  * @ingroup papi_fe */

/** @defgroup papi_fe_chdecN_Notifications Notifications 
  * @ingroup papi_fe_chdec */

/**  @ingroup papi_fe_chdecN_Notifications */
/**  Notifies subscribed clients that the current BER crossed a Ber Threshold. */ 
/**  @param ber Input parameter returning the BER in units of 10-8 */
/**  @param berThreshold Input parameter returning the Ber threshold that was crossed in units of 10-8 */
extern void papi_fe_chdecN_OnBerThresholdCrossed( Nat32 ber, Nat32 berThreshold );

/**  @ingroup papi_fe_chdecN_Notifications */
/**  This notification is given by the platform whenever there is a change in the presence of a carrier in the channel decoder/digital demodulator. Presence of a carrier means that the demodulator has been able to lock onto valid data. */ 
/**  @param carrier True, if carrier presence has been detected. False, if carrier presence is lost. */
extern void papi_fe_chdecN_OnCarrierPresentChanged( Bool carrier );

/**  @ingroup papi_fe_chdecN_Notifications */
/**  Notifies subscribed clients that the Channel Bandwidth has been determined. */ 
/**  @param bandwith Input parameter indicating the detected channel bandwidth. */
extern void papi_fe_chdecN_OnChanBandwidthDetected( tmFe_ChanBandwidth_t bandwith );

/**  @ingroup papi_fe_chdecN_Notifications */
/**  Notifies subscribed clients that a CodeRate has been determined. In case only one bit stream is received, this is considered the high priority stream and the coderate for the low priority stream is set to tmFe_CodeRateNone. */ 
/**  @param highPrioRate Input parameter indicating the detected High Priority coderate. */
/**  @param lowPrioRate Input parameter indicating the detected Low Priority coderate. */
extern void papi_fe_chdecN_OnCodeRateDetected( tmFe_CodeRate_t highPrioRate, tmFe_CodeRate_t lowPrioRate );

/**  @ingroup papi_fe_chdecN_Notifications */
/**  Notifies subscribed clients when a constellation is detected. */ 
/**  @param constellation Input parameter indicating the detected constellation. */
extern void papi_fe_chdecN_OnConstellationDetected( tmFe_Constellation_t constellation );

/**  @ingroup papi_fe_chdecN_Notifications */
/**  Notifies subscribed clients that the Guard Interval has been determined. */ 
/**  @param interval Input parameter indicating the detected GuardInterval. */
extern void papi_fe_chdecN_OnGuardIntervalDetected( tmFe_GuardInterval_t interval );

/**  @ingroup papi_fe_chdecN_Notifications */
/**  Notifies subscribed clients that the HierarchyMode has been determined. */ 
/**  @param mode Input parameter indicating the detected Hierarchy Mode. */
extern void papi_fe_chdecN_OnHierarchyModeDetected( tmFe_HierarchyMode_t mode );

/**  @ingroup papi_fe_chdecN_Notifications */
/**  Notifies subscribed clients that a IqMode has been determined. */ 
/**  @param mode Input parameter indicating the detected IqMode. */
extern void papi_fe_chdecN_OnIqModeDetected( tmFe_IqMode_t mode );

/**  @ingroup papi_fe_chdecN_Notifications */
/**  Notifies subscribed clients that the TxMode has been determined. */ 
/**  @param mode Input parameter indicating the detected TxMode. */
extern void papi_fe_chdecN_OnTxModeDetected( tmFe_TxMode_t mode );



/*
 *  interface colN of type IPlfApiColorDecodeNotify
 */

/** @defgroup papi_fe_col papi_fe_col: ColorDecode
  * @ingroup papi_fe */

/** @defgroup papi_fe_colN_Notifications Notifications 
  * @ingroup papi_fe_col */

/**  @ingroup papi_fe_colN_Notifications */
/**  Notification when a change in the color system has occurred. */ 
extern void papi_fe_colN_OnColorSystemChanged( void );

/**  @ingroup papi_fe_colN_Notifications */
/**  Notification when a change in the comb filter system has occurred. */ 
extern void papi_fe_colN_OnCombFilterActiveChanged( void );



/*
 *  interface hsysN of type IPlfApiSoundSystemNotify
 */

/** @defgroup papi_fe_hsys papi_fe_hsys: SoundSystem
  * <b>Sound system detection</b><br><br>
Sound system detection by the audio decoder inside the platform is always active and uses this interface to signal changes. If StartSearch() is called on the tuner, as soon as the signal disappears, the audio decoder will notify an OnSoundSystemChanged as due to the signal loss it will go to SoundSystemInvalid. When the signal is found (and stable) it raises again a notification to signal the change to the detected sound system. Thus any change in the audio signal, also caused by SetFreq(), has to be notified.
  * @ingroup papi_fe */

/** @defgroup papi_fe_hsysN_Notifications Notifications 
  * @ingroup papi_fe_hsys */

/**  @ingroup papi_fe_hsysN_Notifications */
/**  Notification that the sound system has changed. The contents of the sound system can be retrieved via the SoundSystem interface. Also a loss of sound system is notified. */ 
extern void papi_fe_hsysN_OnSoundSystemChanged( void );



/*
 *  interface qssmN of type IPlfApiSigStrengthMeasNotify
 */

/** @defgroup papi_fe_qssm papi_fe_qssm: SigStrengthMeas
  * @ingroup papi_fe */

/** @defgroup papi_fe_qssmN_Notifications Notifications 
  * @ingroup papi_fe_qssm */

/**  @ingroup papi_fe_qssmN_Notifications */
/**  This notification function is called when a signal strength measurement is stopped. */ 
extern void papi_fe_qssmN_OnMeasAborted( void );

/**  @ingroup papi_fe_qssmN_Notifications */
/**  This notification function is called when a signal strength measurement is ready. */ 
/**  @param strength The signal strength. */
extern void papi_fe_qssmN_OnMeasReady( Nat32 strength );



/*
 *  interface ssmN of type IPlfApiSigStrengthMeasNotify
 */

/** @defgroup papi_fe_ssm papi_fe_ssm: SigStrengthMeas
  * @ingroup papi_fe */

/** @defgroup papi_fe_ssmN_Notifications Notifications 
  * @ingroup papi_fe_ssm */

/**  @ingroup papi_fe_ssmN_Notifications */
/**  This notification function is called when a signal strength measurement is stopped. */ 
extern void papi_fe_ssmN_OnMeasAborted( void );

/**  @ingroup papi_fe_ssmN_Notifications */
/**  This notification function is called when a signal strength measurement is ready. */ 
/**  @param strength The signal strength. */
extern void papi_fe_ssmN_OnMeasReady( Nat32 strength );



/*
 *  interface ssysN of type IPlfApiSoundSystemNotify
 */

/** @defgroup papi_fe_ssys papi_fe_ssys: SoundSystem
  * <b>Sound system detection</b><br><br>
Sound system detection by the audio decoder inside the platform is always active and uses this interface to signal changes. If StartSearch() is called on the tuner, as soon as the signal disappears, the audio decoder will notify an OnSoundSystemChanged as due to the signal loss it will go to SoundSystemInvalid. When the signal is found (and stable) it raises again a notification to signal the change to the detected sound system. Thus any change in the audio signal, also caused by SetFreq(), has to be notified.
  * @ingroup papi_fe */

/** @defgroup papi_fe_ssysN_Notifications Notifications 
  * @ingroup papi_fe_ssys */

/**  @ingroup papi_fe_ssysN_Notifications */
/**  Notification that the sound system has changed. The contents of the sound system can be retrieved via the SoundSystem interface. Also a loss of sound system is notified. */ 
extern void papi_fe_ssysN_OnSoundSystemChanged( void );



/*
 *  interface tsdmxN of type IPlfApiTsDmxAbstractNotify
 */

/** @defgroup papi_fe_tsdmx papi_fe_tsdmx: TsDmxAbstract
  * @ingroup papi_fe */

/** @defgroup papi_fe_tsdmx_Datatypes Datatypes 
  * @ingroup papi_fe_tsdmx */
/**
 *   @defgroup papi_fe_tsdmx_CrcStatus CRC Status
 *   @ingroup papi_fe_tsdmx_Datatypes
 *   Supported levels of CRC Status.
 *   @{ */
#define papi_fe_tsdmxN_CrcStatusOk 1
#define papi_fe_tsdmxN_CrcStatusNotOk 2
#define papi_fe_tsdmxN_CrcStatusNotChecked 3
#define papi_fe_tsdmxN_CrcStatusNoCrc 4
/*   @} */

/** @defgroup papi_fe_tsdmxN_Notifications Notifications 
  * @ingroup papi_fe_tsdmx */

/**  @ingroup papi_fe_tsdmxN_Notifications */
/**  Generated when the audio descriptive pid filter has stopped after been requested to do so via a call to the corresponding stop function. */ 
/**  @param pid Input parameter packet ID. */
extern void papi_fe_tsdmxN_OnAudioDescriptivePidStopped( Nat16 pid );

/**  @ingroup papi_fe_tsdmxN_Notifications */
/**  Generated when the audio pid filter has stopped after been requested to do so via a call to the corresponding stop function. */ 
/**  @param pid Input parameter packet ID. */
extern void papi_fe_tsdmxN_OnAudioPidStopped( Nat16 pid );

/**  @ingroup papi_fe_tsdmxN_Notifications */
/**  Generated when the transport stream demultiplexer cannot synchronize on the incoming transport stream due to its high bit rate. */ 
extern void papi_fe_tsdmxN_OnBufferOverflow( void );

/**  @ingroup papi_fe_tsdmxN_Notifications */
/**  Generated when the PRC pid filter has stopped after been requested to do so via a call to the corresponding stop function. */ 
/**  @param pid Input parameter packet ID. */
extern void papi_fe_tsdmxN_OnPcrPidStopped( Nat16 pid );

/**  @ingroup papi_fe_tsdmxN_Notifications */
/**  Generated whenever section filter request is cancelled (either by a client of the interface who requested it or by removal of the demux logical component in the signal path due to switching of the use case). */ 
/**  @param subscrId The identifier for a subscription request. Identifiers can be obtained from a separate interface. */
extern void papi_fe_tsdmxN_OnPesCancelled( Nat32 subscrId );

/**  @ingroup papi_fe_tsdmxN_Notifications */
/**  Generated when the Demux logical component is available (when selecting digital usecase). */ 
extern void papi_fe_tsdmxN_OnRequestAllowed( void );

/**  @ingroup papi_fe_tsdmxN_Notifications */
/**  Generated whenever section filter request is cancelled (either by client who requested it or by removal of demux logical component due to use case switch). */ 
/**  @param subscrId The identifier for a subscription request. Identifiers can be obtained from a separate interface. */
extern void papi_fe_tsdmxN_OnRequestCancelled( Nat32 subscrId );

/**  @ingroup papi_fe_tsdmxN_Notifications */
/**  Generated whenever section list filter request is cancelled (either by client who requested it or by removal of demux logical component due to use case switch). */ 
/**  @param subscrId The identifier for a subscription request. Identifiers can be obtained from a separate interface. */
extern void papi_fe_tsdmxN_OnRequestListCancelled( Nat32 subscrId );

/**  @ingroup papi_fe_tsdmxN_Notifications */
/**  Generated when the Demux logical component is removed (when selecting analog usecase). */ 
extern void papi_fe_tsdmxN_OnRequestNotAllowed( void );

/**  @ingroup papi_fe_tsdmxN_Notifications */
/**  Generated when the scrambling status of the transport stream signal has changed. */ 
/**  @param MediaType Input parameter media type. */
/**  @param status Indicates whether the current stream that is filtered by the TPH Filter or PID Filter is scrambled. */
extern void papi_fe_tsdmxN_OnScramblingStatusChanged( int MediaType, int status );

/**  @ingroup papi_fe_tsdmxN_Notifications */
/**  Notifies the arrival of demux sections. It is called when a complete MPEG section has been filtered and written to a buffer. Complete means starting from and including the `table_id' (see [5]) to the last byte of this section including the CRC field, i.e. `section_length' + 3 bytes of this section, if present */ 
/**  @param subscrId The identifier for a subscription request. Identifiers can be obtained from a separate interface. */
/**  @param Cookie Cookie that will be returned to user for every notification of section. */
/**  @param CrcStatus Result of the CRC check on the notified section. */
/**  @param PSize Size of the available data, in bytes (excluding any buffer wrap-around) */
/**  @param PAddr Memory pointer pointing at the start of the available data. */
/**  @param SSize Size of the available data in the second part, in bytes, in case of a buffer wrap-around. Zero, otherwise. */
/**  @param SAddr Memory pointer pointing at the start of the available data in the second part, in case of a buffer wrap-around. */
extern void papi_fe_tsdmxN_OnSectionArrived( Nat32 subscrId, int Cookie, int CrcStatus, Nat32 PSize, Address PAddr, Nat32 SSize, Address SAddr, Nat32 DataCookie );

/**  @ingroup papi_fe_tsdmxN_Notifications */
/**  Notifies the arrival of a section list, which is nothing more then a combination of OnSectionArrived notifications */ 
/**  @param subscrId The identifier for a subscription request. Identifiers can be obtained from a separate interface. */
/**  @param Cookie Cookie that will be returned to user for every notification of section. */
/**  @param ListCookie Cookie that will be returned to user for every notification of section. */
/**  @param nSections Number of individual sections present in the sections parameter. */
/**  @param sections Array of sections, size nSections. */
extern void papi_fe_tsdmxN_OnSectionListArrived( Nat32 subscrId, int Cookie, Nat32 ListCookie, Nat32 nSections, tmTsDmx_Section_t * sections );

/**  @ingroup papi_fe_tsdmxN_Notifications */
/**  Generated when the transport stream demultiplexer encounters the synchronization byte on several consecutive transport packets of the incoming transport stream after previously being out of sync. */ 
extern void papi_fe_tsdmxN_OnSyncLock( void );

/**  @ingroup papi_fe_tsdmxN_Notifications */
/**  Generated when the synchronization byte on several consecutive transport packets of the incoming transport stream is not found. */ 
extern void papi_fe_tsdmxN_OnSyncLost( void );

/**  @ingroup papi_fe_tsdmxN_Notifications */
/**  Generated when the video pid filter has stopped after been requested to do so via a call to the corresponding stop function. */ 
/**  @param pid Input parameter packet ID. */
extern void papi_fe_tsdmxN_OnVideoPidStopped( Nat16 pid );



/*
 *  interface tunN of type IPlfApiTuningNotify
 */

/** @defgroup papi_fe_tun papi_fe_tun: Tuning
  * @ingroup papi_fe */

/** @defgroup papi_fe_tunN_Notifications Notifications 
  * @ingroup papi_fe_tun */

/**  @ingroup papi_fe_tunN_Notifications */
/**  This notification is raised if the actual frequency changes because of AFC, e.g. per minimal step of 1/16 MHz. */ 
/**  @param freq The current frequency value. */
extern void papi_fe_tunN_OnAfcFreqChanged( Nat32 freq );

/**  @ingroup papi_fe_tunN_Notifications */
/**  This function is called when AFC reaches the limits of the AFC window. */ 
/**  @param freq The current frequency value. */
extern void papi_fe_tunN_OnAfcLimitReached( Nat32 freq );

/**  @ingroup papi_fe_tunN_Notifications */
/**  This function is called when the AFC mode changes. It is not called when AFC is enabled or disabled explicitly by the client of the platform. It will typically be called if AFC is enabled and SetFreq(), SetChan(), or StartSearch() is called. These calls will stop the AFC process, which has to be signalled via this notification. */ 
extern void papi_fe_tunN_OnAfcModeChanged( void );

/**  @ingroup papi_fe_tunN_Notifications */
/**  This notification is typically used by the middleware to present a progress bar to the user in line with the progress of the tuning algorithm. A notification is expected once per MHz. If the tuning algorithm is fine tuning and making small steps, then a notification is expected only when the smaller steps have added up to at least a MHz. Similarly, if the tuning algorithm is making a big step and in one step it increase the frequency say 8 MHz (for whatever reason) only one notification is expected and not a separate notification for every MHz that it increases. */ 
/**  @param state Indicates the current state of the tuning algorithm, see enum #_tmFe_SearchState_t. It is sufficient if the platform simply returns tmFe_SsSearching. */
extern void papi_fe_tunN_OnSearchInProgress( tmFe_SearchState_t state );

/**  @ingroup papi_fe_tunN_Notifications */
/**  This function is called when search state is changed. */ 
/**  @param state Indicates the current state of the tuning algorithm, see enum #_tmFe_SearchState_t. It is sufficient if the platform sends a notification immediately after StartSearch() is called indicating a transition to the state tmFe_SsSearching. If searching has finished, the platform has to notify the transition to tmFe_SsNone. */
extern void papi_fe_tunN_OnSearchStateChanged( tmFe_SearchState_t state );

/**  @ingroup papi_fe_tunN_Notifications */
/**  This function is called when a station is found. It is used both for analog and digital tuning. For digital, it means that the channel decoder has locked to the signal. (It does NOT indicate whether the demultiplexer was capable of extracting a valid audio video stream from the transport stream.). \n \n
The difference with OnCarrierPresentChanged() in the channel decoder is that an OnStationFound() (or an OnStationNotFound()) notification results from an explicit call to the platform, i.e. because SetFreq(), SetChan() or StartSearch() was called. OnCarrierPresentChanged() is reported whenever there is a change in the presence of a carrier in the channel decoder/digital demodulator, e.g. because the input signal was lost.
 */ 
extern void papi_fe_tunN_OnStationFound( void );

/**  @ingroup papi_fe_tunN_Notifications */
/**  This function is called when a station is not found. It is used both for analog and digital tuning. */ 
extern void papi_fe_tunN_OnStationNotFound( void );

/**  @ingroup papi_fe_tunN_Notifications */
/**  This function is called when a TV system is detected. */ 
/**  @param tvSystem Subscription TV system value. */
extern void papi_fe_tunN_OnTvSystemDetected( tmFe_TvSystem_t tvSystem );



/*
 *  interface vbislN of type IPlfApiVbiSliceNotify
 */

/** @defgroup papi_fe_vbisl papi_fe_vbisl: VbiSlice
  * <b>OnXNoData</b><br><br>
The notification interface contains a number of functions related to specific data not being detected for the last x consecutive frames, e.g. OnCcNoData, OnCgmsNoData, OnWssNoData, etc. The value x should be a configurable parameter of the platform. The same value x is used for all functions.
  * @ingroup papi_fe */

/** @defgroup papi_fe_vbislN_Notifications Notifications 
  * @ingroup papi_fe_vbisl */

/**  @ingroup papi_fe_vbislN_Notifications */
/**  Notification function by the platform to return closed captioning information which has been captured by the VBI slicer. The closed captioning information in the odd and even field is returned via separate parameters. */ 
/**  @param cctype The type off CC data that has been captured. It is a value of enum #_tmVbiSlice2_Cc_t. */
/**  @param oLine The line number for which CC data from the odd field has been sliced. If oLine is 0, the content of oField is irrelevant. */
/**  @param eLine The line number for which CC data from the even field has been sliced. If eLine is 0, the content of eField is irrelevant. */
/**  @param oField The CC data from the odd field. */
/**  @param eField The CC data from the even field. */
extern void papi_fe_vbislN_OnCcData( tmVbiSlice2_Cc_t cctype, Nat32 oLine, Nat32 eLine, Nat16 oField, Nat16 eField );

/**  @ingroup papi_fe_vbislN_Notifications */
/**  Notification function that the stream of CC data in either the odd or even field has ended. It is signalled, if CC data was previously detected, but has not been detected for last x consecutive frames (where x is a configurable parameter of the platform.). */ 
/**  @param field True if CC data stream in the even fields has ended. False if the CC data stream in the odd fields has ended. */
extern void papi_fe_vbislN_OnCcNoData( Bool field );

/**  @ingroup papi_fe_vbislN_Notifications */
/**  Notification function by the platform to return CGMS (Copy Generation Management System) data which has been captured by the VBI slicer. */ 
/**  @param cgmsPresentOddField Indicates whether CGMS data from the odd field of the current frame has been extracted. */
/**  @param cgmsPresentEvenField Indicates whether CGMS data from the even field of the current frame has been extracted. */
/**  @param dataValidOddField If True, it indicates that the CRC check on the data in dataOddField is okay, else the content of dataValidOddField is irrelevant. */
/**  @param dataValidEvenField If True, it indicates that the CRC check on the data in dataEvenField is okay, else the content of dataValidEvenField is irrelevant. */
/**  @param dataOddField If cgmsPresentOddField is True, dataOddField contains the CGMS data of the odd field of the current frame. The CGMS bits are in bits 0..7. */
/**  @param dataEvenField If cgmsPresentEvenField is True, dataEvenField contains the CGMS data of the even field of the current frame. The CGMS bits are in bits 0..7. */
extern void papi_fe_vbislN_OnCgmsData( Bool cgmsPresentOddField, Bool cgmsPresentEvenField, Bool dataValidOddField, Bool dataValidEvenField, Nat8 dataOddField, Nat8 dataEvenField );

/**  @ingroup papi_fe_vbislN_Notifications */
/**  Notification function that the stream of CGMS data has ended. It is signalled, if CGMS data was previously detected, but has not been detected for last x consecutive frames (where x is a configurable parameter of the platform.). */ 
extern void papi_fe_vbislN_OnCgmsNoData( void );

/**  @ingroup papi_fe_vbislN_Notifications */
/**  Notification function by the platform to return Gemstar data which has been captured by the VBI slicer. The pointer Buffer points to the Gemstar data that has been captured by the VBI slicer on a frame. The platform owns the storage space occupied by the data. The storage spaces, and therefore the Gemstar data, are guaranteed to stay unmodified until this function returns. */ 
/**  @param nrLines The number of lines of the video frame for which Gemstar data has been sliced. */
/**  @param pLines Pointer to a buffer containing the line numbers for which Gemstar data has been sliced. pLines[i] contains the VBI line sliced with Gemstar data type Types[i]. The size of pLines buffer is defined by nrLines. */
/**  @param Types Pointer to a buffer containing the Gemstar type for each line for which Gemstar data has been sliced. Its values are of type enum #_tmVbiSlice2_Gemstar_t. The size of the Types buffer is defined by nrLines. */
/**  @param size The size, in bytes, of valid Gemstar data to which Buffer points. If nrLines=0, this is also 0. */
/**  @param Buffer Pointer to a 4-byte aligned buffer with valid Gemstar data. Buffer points to the first element of this data. The size of the buffer is given by the size parameter. */
extern void papi_fe_vbislN_OnGemstarData( Nat32 nrLines, Nat32 * pLines, tmVbiSlice2_Gemstar_t * Types, Nat32 size, Nat8 * Buffer );

/**  @ingroup papi_fe_vbislN_Notifications */
/**  Notification function that the stream of Gemstar data has ended. It is signalled, if Gemstar data was previously detected, but has not been detected for last x consecutive frames (where x is a configurable parameter of the platform.). */ 
/**  @param gemstarType Indicates the Gemstar type of data that has ended. It is a value of type enum #_tmVbiSlice2_Gemstar_t. */
extern void papi_fe_vbislN_OnGemstarNoData( tmVbiSlice2_Gemstar_t gemstarType );

/**  @ingroup papi_fe_vbislN_Notifications */
/**  Notification function by the platform to return Video programming system data which has been captured by the VBI slicer. The pointer Buffer points to the captured VPS data. The VBI Slicer owns the storage space occupied by the data. The storage space, and therefore the VPS data, is guaranteed to stay unmodified until this function returns. */ 
/**  @param size The size, in bytes, of valid VPS data. */
/**  @param Buffer Pointer to a buffer of valid VPS data. */
extern void papi_fe_vbislN_OnVpsData( Nat8 size, Nat8 * Buffer );

/**  @ingroup papi_fe_vbislN_Notifications */
/**  Notification function that the stream of VPS data has ended. It is signalled, if VPS data was previously detected, but has not been detected for last x consecutive frames (where x is a configurable parameter of the platform.). */ 
extern void papi_fe_vbislN_OnVpsNoData( void );

/**  @ingroup papi_fe_vbislN_Notifications */
/**  Notification function by the platform to return wide-screen signalling (WSS) information which has been captured by the VBI slicer. The WSS information in the odd and even field is returned via separate parameters. */ 
/**  @param wsstype The type off WSS data that has been captured. It is a value of type enum #_tmVbiSlice2_Wss_t. */
/**  @param wssPresentOddField Indicates whether WSS data from the odd field of the current frame has been extracted. */
/**  @param wssPresentEvenField Indicates whether WSS data from the even field of the current frame has been extracted. */
/**  @param dataOddField If wssPresentOddField is True, dataOddField contains the WSS data of the odd field of the current frame. The WSS bits are in bits 0..13. If wssPresentOddField is False, the content of dataOddField is irrelevant. */
/**  @param dataEvenField If wssPresentEvenField is True, dataEvenField contains the WSS data of the even field of the current frame. The WSS bits are in bits 0..13. If wssPresentEvenField is False, the content of dataEvenField is irrelevant. */
/**  @param errorBitsOddField If wssPresentOddField is True, errorBitsOddField indicates whether there are errors in the WSS data of the odd field of the current frame. <br>
When the type of the WSS data is #tmVbiSlice2_EuroWss, errorBitsOddField is 0 (EuroWss has no CRC data). <br>
When the type of the WSS data is #tmVbiSlice2_UsWss, if errorBitsOddField is not equal to 0, it indicates that the CRC check on the data in dataOddField failed. <br>
If wssPresentOddField is False, the content of errorBitsOddField is irrelevant. */
/**  @param errorBitsEvenField If wssPresentEvenField is True, errorBitsEvenField indicates whether there are errors in the WSS data of the even field of the current frame. <br>
When the type of the WSS data is #tmVbiSlice2_EuroWss, errorBitsEvenField is 0 (EuroWss has no CRC data). <br>
When the type of the WSS data is #tmVbiSlice2_UsWss, if errorBitsEvenField is not equal to 0, it indicates that the CRC check on the data in dataEvenField failed. <br>
If wssPresentEvenField is False, the content of errorBitsEvenField is irrelevant. */
extern void papi_fe_vbislN_OnWssData( tmVbiSlice2_Wss_t wsstype, Bool wssPresentOddField, Bool wssPresentEvenField, Nat16 dataOddField, Nat16 dataEvenField, Nat16 errorBitsOddField, Nat16 errorBitsEvenField );

/**  @ingroup papi_fe_vbislN_Notifications */
/**  Notification function that the stream of wide-screen signalling (WSS) data has ended. It is signalled, if WSS data was previously detected, but has not been detected for last x consecutive frames (where x is a configurable parameter of the platform.). */ 
extern void papi_fe_vbislN_OnWssNoData( void );

/**  @ingroup papi_fe_vbislN_Notifications */
/**  Notification function by the platform to return world standard teletext (WST) information which has been captured by the VBI slicer for a frame (odd and even fields). The WST information that has been captured for the individual fields is returned in separate buffers in consecutive order: first field in the FirstBuffer, second field in the SecondBuffer. The VBI Slicer owns the storage space occupied by the data. The storage spaces, and therefore the WST data, are guaranteed to stay unmodified until this function returns. */ 
/**  @param wsttype The type off WST data that has been captured. It is a value of type enum #_tmVbiSlice2_Wst_t. */
/**  @param nrLines The number of lines of the video frame for which WST data has been sliced, which is the sum of the lines that have been sliced in each individual field. */
/**  @param pLines Pointer to a buffer containing the line numbers for which WST data has been sliced. This buffer contains first the line numbers that have been sliced on the first field, followed by the line numbers that have been sliced on the second field. The size of the buffer equals nrLines. */
/**  @param sizeFirstBuffer The size, in bytes, of valid WST data to which FirstBuffer points. If nrLines=0, this is also 0. */
/**  @param sizeSecondBuffer The size, in bytes, of valid WST data to which SecondBuffer points. If nrLines=0, this is also 0. */
/**  @param FirstBuffer Pointer to a buffer with valid WST data which has been retrieved from the first field. The size of the buffer is sizeFirstBuffer. */
/**  @param SecondBuffer Pointer to a buffer with valid WST data which has been retrieved from the second field. The size of the buffer is sizeSecondBuffer. */
extern void papi_fe_vbislN_OnWstData( tmVbiSlice2_Wst_t wsttype, Nat32 nrLines, Nat32 * pLines, Nat32 sizeFirstBuffer, Nat32 sizeSecondBuffer, Nat8 * FirstBuffer, Nat8 * SecondBuffer );

/**  @ingroup papi_fe_vbislN_Notifications */
/**  Notification function that the stream of world standard teletext (WST) data has ended. It is signalled, if WST data was previously detected, but has not been detected for last x consecutive frames (where x is a configurable parameter of the platform.). */ 
extern void papi_fe_vbislN_OnWstNoData( void );



/*
 *  interface vipN of type IPlfApiVideoPropertiesNotify
 */

/** @defgroup papi_fe_vip papi_fe_vip: VideoProperties
  * There is a difference between the notifications from the video properties interface in the Frontend and the notifications from the video output interface in Video Featuring. The video properties interface indicates the properties at the input, as detected by the decoder(s) (analog, digital, ...). The video output interfaces indicates properties at the output of the mixer that goes to the display. Typically, the video output properties do not change as the format to the display remains the same. The video input properties on the other hand follow the signal properties at the input.
  * @ingroup papi_fe */

/** @defgroup papi_fe_vipN_Notifications Notifications 
  * @ingroup papi_fe_vip */

/**  @ingroup papi_fe_vipN_Notifications */
/**  Notification when a change in field frequecy of the video signal has ocurred. */ 
extern void papi_fe_vipN_OnFieldFrequencyChanged( void );

/**  @ingroup papi_fe_vipN_Notifications */
/**  Notification when a change in film mode of the video signal has ocurred. */ 
extern void papi_fe_vipN_OnFilmModeChanged( void );

/**  @ingroup papi_fe_vipN_Notifications */
/**  Notification when a change in VCR signal has ocurred. */ 
extern void papi_fe_vipN_OnFromVcrChanged( void );

/**  @ingroup papi_fe_vipN_Notifications */
/**  Notification when a change in image format of the video signal has ocurred. */ 
extern void papi_fe_vipN_OnImageFormatChanged( void );

/**  @ingroup papi_fe_vipN_Notifications */
/**  Notification when a change in interlaced mode of the video signal has ocurred. */ 
extern void papi_fe_vipN_OnInterlacedChanged( void );

/**  @ingroup papi_fe_vipN_Notifications */
/**  Notification when a change in number of lines in a frame of the video signal has ocurred. */ 
extern void papi_fe_vipN_OnNumberOfFrameLinesChanged( void );

/**  @ingroup papi_fe_vipN_Notifications */
/**  Notification when a change in number of visible lines of the video signal has ocurred. */ 
extern void papi_fe_vipN_OnNumberOfVisibleLinesChanged( void );

/**  @ingroup papi_fe_vipN_Notifications */
/**  Notification when a change in number of visible pixels of the video signal has ocurred. */ 
extern void papi_fe_vipN_OnNumberOfVisiblePixelsChanged( void );

/**  @ingroup papi_fe_vipN_Notifications */
/**  Notification when a change in signal presence has occured. */ 
extern void papi_fe_vipN_OnSignalPresentChanged( void );

/**  @ingroup papi_fe_vipN_Notifications */
/**  Notification when a change in the video coding has occured. */ 
extern void papi_fe_vipN_OnVideoCodingChanged( void );

/**  @ingroup papi_fe_vipN_Notifications */
/**  Notification when a change in video presence has occured. */ 
extern void papi_fe_vipN_OnVideoPresentChanged( void );

/**  @ingroup papi_fe_vipN_Notifications */
/**  Notification when a change in the protection mechanism of the video signal has occured. */ 
extern void papi_fe_vipN_OnVideoProtectionChanged( void );


