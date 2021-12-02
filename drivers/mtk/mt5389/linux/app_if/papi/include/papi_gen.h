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
 *   @file papi_gen.h
 */
/** @defgroup papi_gen  */

/*
 *  interface aln of type IPlfApiAlign
 */

/** @defgroup papi_gen_aln papi_gen_aln: Align
  * @ingroup papi_gen */

/** @defgroup papi_gen_aln_Functions Functions 
  * @ingroup papi_gen_aln */



/**  @ingroup papi_gen_aln_Functions */
/**  Returns a default value for the alignment. See also the Min function. */ 
/**  @param alignment The alignment for which the default value must be returned. */
/**  @param retval The default value for the alignment. */
extern FResult papi_gen_aln_Default( int alignment, int * retval );

/**  @ingroup papi_gen_aln_Functions */
/**  Returns the value of the specified alignment. */ 
/**  @param alignment The alignment for which the value is to be returned. */
/**  @param retval The current value of the specified alignment. */
extern FResult papi_gen_aln_Get( int alignment, int * retval );

/**  @ingroup papi_gen_aln_Functions */
/**  States whether this alignment is supported by the platform. */ 
/**  @param alignment The alignment that is or is not supported. */
/**  @param retval TRUE if the alignment is supported, FALSE otherwise. */
extern FResult papi_gen_aln_IsSupported( int alignment, Bool * retval );

/**  @ingroup papi_gen_aln_Functions */
/**  Returns the maximal value for the alignment. See also the Min function. */ 
/**  @param alignment The alignment for which the maximal value must be returned. */
/**  @param retval The maximum value for the alignment. */
extern FResult papi_gen_aln_Max( int alignment, int * retval );

/**  @ingroup papi_gen_aln_Functions */
/**  Returns the minimal value for the alignment. Note that this  value is chassis dependent, therefore it is not defined as part of the data type. The value is defined by the component that implements this interface. */ 
/**  @param alignment The alignment for which the minimal value must be returned. */
/**  @param retval The minimum value for the alignment. */
extern FResult papi_gen_aln_Min( int alignment, int * retval );

/**  @ingroup papi_gen_aln_Functions */
/**  Sets the alignment to the specified value. This is a general alignment function to set an alignment to a specific value. Alignments refers to the collection of (register) settings that needs to be fine-tuned per product instance. This is typically done in the factory or by a service engineer. It needs to be separately documented which alignments can be set for a specific product, the corresponding alignment identifier that has to be used in these functions, and what the allowed values are. */ 
/**  @param alignment The alignment to be set. */
/**  @param value The new value of the alignment. */
extern FResult papi_gen_aln_Set( int alignment, int value );



/*
 *  interface hwerr of type IPlfApiHwError
 */

/** @defgroup papi_gen_hwerr papi_gen_hwerr: HwError
  * This interface allows a client of the platform to query the error which was identified first during startup of the system.
  * @ingroup papi_gen */

/** @defgroup papi_gen_hwerr_Functions Functions 
  * @ingroup papi_gen_hwerr */

/**  @ingroup papi_gen_hwerr_Functions */
/**  Returns the first detected error */ 
/**  @param errid identifier of the error */
extern FResult papi_gen_hwerr_GetFirstError( int * errid );



/*
 *  interface sbp of type IPlfApiSoundBeeper
 */

/** @defgroup papi_gen_sbp papi_gen_sbp: SoundBeeper
  * This interface can be used to control the sound beeper. The beeper can generate a tone with a certain frequency and volume level. Be aware that hardware devices are not always very flexible in setting frequencies or volumes. In these cases the desired setting could be clipped to the nearest possible value
  * @ingroup papi_gen */

/** @defgroup papi_gen_sbp_Functions Functions 
  * @ingroup papi_gen_sbp */

/**  @ingroup papi_gen_sbp_Functions */
/**  Return maximum frequency of the sound beeper. */ 
/**  @param retval Maximum frequency. */
extern FResult papi_gen_sbp_GetMaxFrequency( int * retval );

/**  @ingroup papi_gen_sbp_Functions */
/**  Return minimum frequency of the sound beeper. */ 
/**  @param retval Minimum frequency. */
extern FResult papi_gen_sbp_GetMinFrequency( int * retval );

/**  @ingroup papi_gen_sbp_Functions */
/**  Return the actual frequency of the sound beeper (which is not necessarily the same as the one which was set). */ 
/**  @param retval Return value with the current frequency. */
extern FResult papi_gen_sbp_GetFrequency( int * retval );

/**  @ingroup papi_gen_sbp_Functions */
/**  Set frequency of the sound beeper. */ 
/**  @param frequency Input parameter with the frequency that has to be set. */
extern FResult papi_gen_sbp_SetFrequency( int frequency );

/**  @ingroup papi_gen_sbp_Functions */
/**  Return whether the sound beeper is muted or not. */ 
/**  @param retval True if sound beeper is muted, false if unmuted. */
extern FResult papi_gen_sbp_GetMute( Bool * retval );

/**  @ingroup papi_gen_sbp_Functions */
/**  Mute or unmute the sound beeper. */ 
/**  @param mute True to mute the sound beeper, false to unmute. */
extern FResult papi_gen_sbp_SetMute( Bool mute );

/**  @ingroup papi_gen_sbp_Functions */
/**  Return maximum volume level. */ 
/**  @param retval Maximum volume level. */
extern FResult papi_gen_sbp_GetMaxVolume( int * retval );

/**  @ingroup papi_gen_sbp_Functions */
/**  Return minimum volume level. */ 
/**  @param retval Minimum volume level. */
extern FResult papi_gen_sbp_GetMinVolume( int * retval );

/**  @ingroup papi_gen_sbp_Functions */
/**  Return the current volume of the sound beeper. */ 
/**  @param retval Return value with the current volume. */
extern FResult papi_gen_sbp_GetVolume( int * retval );

/**  @ingroup papi_gen_sbp_Functions */
/**  Set the volume of the sound beeper. */ 
/**  @param volume Volume to be set. */
extern FResult papi_gen_sbp_SetVolume( int volume );



/*
 *  interface vgtp of type IPlfApiVideoGeneratorTestPattern
 */

/** @defgroup papi_gen_vgtp papi_gen_vgtp: VideoGeneratorTestPattern
  * This interface relates to the capability of individual hardware components inside the platform to put a video test pattern onto the video path. Which hardware components are actually capable of generating such a test pattern is platform specific. Test patterns are typically used by service to troubleshoot in case there is no video signal. Depending on the hardware, there can be multiple hardware components inside the platform that can generate a test pattern. In addition, a single component may be capable of generating different test patterns. With a particular platform implementation should come a list that states the components (= video generator IDs) and test patterns (= pattern IDs) that can be selected per component using this interface.
  * @ingroup papi_gen */

/** @defgroup papi_gen_vgtp_Functions Functions 
  * @ingroup papi_gen_vgtp */

/**  @ingroup papi_gen_vgtp_Functions */
/**  Check whether the specified video generator is allowed to generate the specified test pattern at this moment. It is platform specific under which conditions this is not allowed. */ 
/**  @param generatorid Select a particular video generator. */
/**  @param patternid Select a particular test pattern. */
/**  @param retval TRUE if the video generator is allowed to generate the test pattern, FALSE otherwise. */
extern FResult papi_gen_vgtp_IsTestPatternAllowed( int generatorid, int patternid, Bool * retval );

/**  @ingroup papi_gen_vgtp_Functions */
/**  Returns the current test pattern that is active for a particular video generator. It will be typically 'None' after the system has started and once SetTestPattern() is called, it is the last test pattern that was set. */ 
/**  @param generatorid Select a particular video generator. */
/**  @param retval Return value with the current test pattern. */
extern FResult papi_gen_vgtp_GetTestPattern( int generatorid, int * retval );

/**  @ingroup papi_gen_vgtp_Functions */
/**  This function is used to activate a test pattern inside the platform. It is not intended that a call to this function changes the video signal path that was already selected, e.g. change the source input. The test pattern will only be visible if a signal path was already selected in which the hardware component that generates the test pattern is also a part of. For instance, if an analogue use case is active and the MPEG decoder is instructed to generate a test pattern, the test pattern will typically not be visible. Disabling the test pattern is typically done by selecting a patterid that represents 'None' on the video generator. It should be defined with the platform implementation which video generator IDs are supported and which patterns IDs can be selected per video generator (including 'None'). */ 
/**  @param generatorid Select a particular video generator. */
/**  @param patternid Select a particular test pattern. */
extern FResult papi_gen_vgtp_SetTestPattern( int generatorid, int patternid );



/*
 *  interface hwerrN of type IPlfApiHwErrorNotify
 */

/** @defgroup papi_gen_hwerr papi_gen_hwerr: HwError
  * This interface is used to notify any HW malfunctioning in the system.
  * @ingroup papi_gen */

/** @defgroup papi_gen_hwerrN_Notifications Notifications 
  * @ingroup papi_gen_hwerr */

/**  @ingroup papi_gen_hwerrN_Notifications */
/**  Notifies that an error has occured, e.g. while doing an I2C write to a device, or when a status register of a device indicates an error, or ... */ 
/**  @param errid An identifier for the error. Typically, these id's are HW specific so they are not specified as part of this interface */
extern void papi_gen_hwerrN_OnError( int errid );


