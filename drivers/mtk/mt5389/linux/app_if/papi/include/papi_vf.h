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
 *   @file papi_vf.h
 */
/** @defgroup papi_vf  */

/*
 *  interface vclip of type IPlfApiVideoClip
 */

/** @defgroup papi_vf_vclip papi_vf_vclip: VideoClip
  * @ingroup papi_vf */

/** @defgroup papi_vf_vclip_Functions Functions 
  * @ingroup papi_vf_vclip */

/**  @ingroup papi_vf_vclip_Functions */
/**  Queues a video clip buffer in front of the video decoder for video clip playback. This function can be called multiple times to queue multiple buffers. */ 
/**  @param id Buffer identifier. */
/**  @param size The size in bytes of the buffer. */
/**  @param Data Pointer to the buffer. */
/**  @param endOfData Indicates if this buffer also signals the end of the clip data. */
/**  @param retval Standard return value. */
extern FResult papi_vf_vclip_AddData( Nat32 id, Nat32 size, Address Data, Bool endOfData, Nat32 * retval );

/**  @ingroup papi_vf_vclip_Functions */
/**  Returns the remaining number of clip buffers that are available to offer video clip data to the platform. */ 
/**  @param AvailableNrBuffers Output parameter returning the remaining number of clip buffers. */
/**  @param retval Standard return value. */
extern FResult papi_vf_vclip_GetAvailableNrBuffers( Nat32 * AvailableNrBuffers, Nat32 * retval );

/**  @ingroup papi_vf_vclip_Functions */
/**  Enables or disables the video clip playback mode. If enabled, the video decoder no longer decodes the video stream that is offered on its streaming input, instead it awaits the arrival of video clip data that is offered via data buffers. If disabled, the video decoders switches back to decoding of the video stream that is offered on its streaming input. */ 
/**  @param enable Indicates whether clip play feature has to be enabled (True) or disabled (False). */
/**  @param retval Standard return value. */
extern FResult papi_vf_vclip_EnableClipPlay( Bool enable, Nat32 * retval );

/**  @ingroup papi_vf_vclip_Functions */
/**  Returns whether the video decoder is in video clip playback mode. */ 
/**  @param Enable Output parameter returning whether the video decoder is in video clip playback mode. */
/**  @param retval Standard return value. */
extern FResult papi_vf_vclip_GetClipPlayEnabled( Bool * Enable, Nat32 * retval );

/**  @ingroup papi_vf_vclip_Functions */
/**  Returns the maximum number of buffers that are available to offer video clip data to the platform. */ 
/**  @param MaxNrBuffers Output parameter returning the maximum number of clip buffers. */
/**  @param retval Standard return value. */
extern FResult papi_vf_vclip_GetMaxNrBuffers( Nat32 * MaxNrBuffers, Nat32 * retval );

/**  @ingroup papi_vf_vclip_Functions */
/**  This function sends a request to start the decoding process. The actual completion of this request is notified via a corresponding notification function. The actual starting depends on the availability of data that is offered via AddData(). This data can be added both before and after this function is called. */ 
/**  @param retval Standard return value. */
extern FResult papi_vf_vclip_Start( Nat32 * retval );

/**  @ingroup papi_vf_vclip_Functions */
/**  Returns the state of video decoder during video clip playback. */ 
/**  @param ClipDecodingState Output parameter the state of video decoder during video clip playback. */
/**  @param retval Standard return value. */
extern FResult papi_vf_vclip_GetState( tmDigVdec_ClipDecodingState_t * ClipDecodingState, Nat32 * retval );

/**  @ingroup papi_vf_vclip_Functions */
/**  This function sends a request to stop the decoding process. The actual completion of this request is notified via a corresponding notification function. This call also releases the pending buffers that have been offered to the platform via AddData(). */ 
/**  @param retval Standard return value. */
extern FResult papi_vf_vclip_Stop( Nat32 * retval );



/*
 *  interface vfr of type IPlfApiVideoFreeze
 */

/** @defgroup papi_vf_vfr papi_vf_vfr: VideoFreeze
  * @ingroup papi_vf */

/** @defgroup papi_vf_vfr_Functions Functions 
  * @ingroup papi_vf_vfr */

/**  @ingroup papi_vf_vfr_Functions */
/**  Indicates if a call to SetFreeze is allowed. */ 
/**  @param retval Returns True if the SetFreeze may be called */
extern FResult papi_vf_vfr_FreezeAllowed( Bool * retval );

/**  @ingroup papi_vf_vfr_Functions */
/**  This function poses a request to the platform to freeze or unfreeze the video content. The completion of the request is signalled via the corresponding notification function. When it is frozen, the video contents is no longer updated. Every SetFreeze call will result in a notification, even if a freeze request is made when the video content is already frozen. */ 
/**  @param mode boolean to freeze or unfreeze. */
extern FResult papi_vf_vfr_SetFreeze( Bool mode );

/**  @ingroup papi_vf_vfr_Functions */
/**  Indicates if the SetFreeze operation will have any effect. */ 
/**  @param retval Returns True when freeze is supported. */
extern FResult papi_vf_vfr_FreezeSupported( Bool * retval );

/**  @ingroup papi_vf_vfr_Functions */
/**  Indicates if the video is frozen. */ 
/**  @param retval a return value of True indicates the video is frozen. */
extern FResult papi_vf_vfr_IsFrozen( Bool * retval );



/*
 *  interface vim of type IPlfApiVideoImprovement
 */

/** @defgroup papi_vf_vim papi_vf_vim: VideoImprovement
  * @ingroup papi_vf */

/** @defgroup papi_vf_vim_Datatypes Datatypes 
  * @ingroup papi_vf_vim */
/**
 *   @defgroup papi_vf_vim_ColorEnhancement Color Enhancement Modes
 *   @ingroup papi_vf_vim_Datatypes
 *   Supported levels of Color Enhancement.
 *   @{ */
 /**  Value used when Color Enhancement is set to off */
#define papi_vf_vim_ModeOffColorEnhancement 0
 /**  Value used when Color Enhancement is set to the minimum level */
#define papi_vf_vim_ModeMinColorEnhancement 1
 /**  Value used when Color Enhancement is set to the medium level */
#define papi_vf_vim_ModeMedColorEnhancement 2
 /**  Value used when Color Enhancement is set to the maximum level */
#define papi_vf_vim_ModeMaxColorEnhancement 3
/*   @} */

/** @defgroup papi_vf_vim_Functions Functions 
  * @ingroup papi_vf_vim */

/**  @ingroup papi_vf_vim_Functions */
/**  Returns the currently set level of color enhancement. */ 
/**  @param retval Set to the current level of color enhancement. See datatype \ref papi_vf_vim_ColorEnhancement. */
extern FResult papi_vf_vim_GetColorEnhancement( int * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Sets color enhancement to the requested level. When this function returns, the platform has changed the color enhancement and will not sent a notification. */ 
/**  @param mode The requested level for color enhancement. See datatype \ref papi_vf_vim_ColorEnhancement. */
extern FResult papi_vf_vim_SetColorEnhancement( int mode );

/**  @ingroup papi_vf_vim_Functions */
/**  Indicates if color enhancement is supported. */ 
/**  @param mode The requested level for color enhancement. See datatype \ref papi_vf_vim_ColorEnhancement. */
/**  @param retval Is set to True when the requested level for color enhancement is supported. Is set to False otherwise. */
extern FResult papi_vf_vim_ColorEnhancementSupported( int mode, Bool * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Returns the value that indicates the maximum level of digital natural motion. */ 
/**  @param retval The value that indicates the maximumlevel of dynamic natural motion. */
extern FResult papi_vf_vim_GetMaxDigitalNaturalMotion( int * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Returns the value that indicates the minimum level of digital natural motion. */ 
/**  @param retval The value that indicates the minimum level of dynamic natural motion. */
extern FResult papi_vf_vim_GetMinDigitalNaturalMotion( int * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Returns the current level of digital natural motion. */ 
/**  @param retval Is set to the current level of digital natural motion. */
extern FResult papi_vf_vim_GetDigitalNaturalMotion( int * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Sets the level of digital natural motion. */ 
/**  @param mode The requested level of digital natural motion. Values between GetMinDigitalNaturalMotion and GetMaxDigitalNaturalMotion are supported. */
extern FResult papi_vf_vim_SetDigitalNaturalMotion( int mode );

/**  @ingroup papi_vf_vim_Functions */
/**  Returns the value that indicates a maximum level of DNR. */ 
/**  @param retval The value that indicates a maximum level of DNR */
extern FResult papi_vf_vim_GetMaxDnr( int * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Returns the value that indicates a medium level of DNR. */ 
/**  @param retval The value that indicates a medium level of DNR */
extern FResult papi_vf_vim_GetMedDnr( int * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Returns the value that indicates a minimum level of DNR. */ 
/**  @param retval The value that indicates a minimum level of DNR */
extern FResult papi_vf_vim_GetMinDnr( int * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Returns the currently set level of DNR. */ 
/**  @param retval The current level set for DNR. */
extern FResult papi_vf_vim_GetDnr( int * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Sets the level of DNR. */ 
/**  @param mode The requested level of DNR. The mode parameter should be GetOffDnr or between the GetMinDnr and GetMaxDnr. */
extern FResult papi_vf_vim_SetDnr( int mode );

/**  @ingroup papi_vf_vim_Functions */
/**  Indicates if DNR is supported. */ 
/**  @param mode The requested level of DNR. The mode parameter should be GetOffDnr or between the GetMinDnr and GetMaxDnr. */
/**  @param retval Is set to True when the requested level of DNR is supported. */
extern FResult papi_vf_vim_DnrSupported( int mode, Bool * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Returns the value that indicates the maximum level of dynamic contrast. */ 
/**  @param retval The value that indicates the maximum level of dynamic contrast. */
extern FResult papi_vf_vim_GetMaxDynamicContrast( int * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Returns the value that indicates the medium level of dynamic contrast. */ 
/**  @param retval The value that indicates the medium level of dynamic contrast. */
extern FResult papi_vf_vim_GetMedDynamicContrast( int * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Returns the value that indicates the minimum level of dynamic contrast. */ 
/**  @param retval The value that indicates the minimum level of dynamic contrast. */
extern FResult papi_vf_vim_GetMinDynamicContrast( int * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Returns the currently set level of dynamic contrast. */ 
/**  @param retval The current level of dynamic constrast. */
extern FResult papi_vf_vim_GetDynamicContrast( int * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Set the dynamic contrast to the requested level. */ 
/**  @param mode The requested level of dynamic constrast. The values between GetMinDynamicContrast en GetMaxDynamicContrast are allowed. */
extern FResult papi_vf_vim_SetDynamicContrast( int mode );

/**  @ingroup papi_vf_vim_Functions */
/**  Indicates if dynamic contrast is supported. */ 
/**  @param mode The requested level of dynamic constrast. The values between GetMinDynamicContrast en GetMaxDynamicContrast are allowed. */
/**  @param retval Is set to True when the requested level of dynamic contrast is supported. Set to False otherwise. */
extern FResult papi_vf_vim_DynamicContrastSupported( int mode, Bool * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Indicates if the specified digital natural motion level is allowed. The SetDigitalNaturalMotion may be called only if this function returns True. */ 
/**  @param mode The requested level of digital natural motion. Values between GetMinDigitalNaturalMotion and GetMaxDigitalNaturalMotion are allowed. */
/**  @param retval Is set to True when the specified level (mode parameter) is allowed. Is set to False otherwise. */
extern FResult papi_vf_vim_IsDigitalNaturalMotionAllowed( int mode, Bool * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Indicates if the specified digital natural motion level is supported. */ 
/**  @param mode The requested level of digital natural motion. Values between GetMinDigitalNaturalMotion and GetMaxDigitalNaturalMotion are allowed. */
/**  @param retval Is set to True when the specified level (mode parameter) is supported. Is set to False otherwise. */
extern FResult papi_vf_vim_IsDigitalNaturalMotionSupported( int mode, Bool * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Indicates if LTI is currently set on or off. */ 
/**  @param retval Is set to True when LTI is currently on. False otherwise. */
extern FResult papi_vf_vim_GetLti( Bool * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Turns the LTI on or off. */ 
/**  @param onoff Setting this to True turns LTI on. */
extern FResult papi_vf_vim_SetLti( Bool onoff );

/**  @ingroup papi_vf_vim_Functions */
/**  Indicates if LTI is supported. IF supported, calls to SetLti are allowed. */ 
/**  @param retval Returns True if LTI is supported. False otherwise. */
extern FResult papi_vf_vim_LtiSupported( Bool * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Indicates if MPEG noise reduction is enabled. */ 
/**  @param retval Is set to True when MPEG noise reduction is enabled. Is set to False otherwise. */
extern FResult papi_vf_vim_GetMpegNoiseReduction( Bool * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Set the MPEG nosie reduction on or off. */ 
/**  @param onoff When True is passed, MPEG noise reducsiton is enabled. */
extern FResult papi_vf_vim_SetMpegNoiseReduction( Bool onoff );

/**  @ingroup papi_vf_vim_Functions */
/**  Indicates if MPEG noise reduction is supported. */ 
/**  @param retval Is set to True when MPEG noise reduction is supported. */
extern FResult papi_vf_vim_MpegNoiseReductionSupported( Bool * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Returns the value that indicates digital natural motion is set to off. */ 
/**  @param retval The value that indicates dynamic natural motion is set to off. */
extern FResult papi_vf_vim_GetOffDigitalNaturalMotion( int * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Returns the value that indicates DNR is off. */ 
/**  @param retval The value that indicates DNR off. */
extern FResult papi_vf_vim_GetOffDnr( int * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Returns the value that indicates dynamic contrast is off. */ 
/**  @param retval The value that indicates dynamic contrast off. */
extern FResult papi_vf_vim_GetOffDynamicContrast( int * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Returns the value that indicates the maximum level of sharpness. */ 
/**  @param retval The value that indicates the maximumlevel of sharpness. */
extern FResult papi_vf_vim_GetMaxSharpness( int * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Returns the value that indicates the medium level of sharpness. */ 
/**  @param retval The value that indicates the medium level of sharpness. */
extern FResult papi_vf_vim_GetMedSharpness( int * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Returns the value that indicates the minimum level of sharpness. */ 
/**  @param retval The value that indicates the minimum level of sharpness. */
extern FResult papi_vf_vim_GetMinSharpness( int * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Returns the currently set level of sharpness. */ 
/**  @param retval The currently set level of sharpness. */
extern FResult papi_vf_vim_GetSharpness( int * retval );

/**  @ingroup papi_vf_vim_Functions */
/**  Sets sharpness to the specified level. When this function returns, the platform has changed the sharpness and it will NOT sent an OnSharpnessChanged notification. */ 
/**  @param mode The requested sharpness level. Values between GetMinSharpness and GetMaxSharpness are allowed. */
extern FResult papi_vf_vim_SetSharpness( int mode );

/**  @ingroup papi_vf_vim_Functions */
/**  Indicates if sharpness is supported. When supported, calls to SetSharpness will have an effect. */ 
/**  @param mode The requested sharpness level. Values betweeen GetMinSharpness and GetMaxSharpness are allowed. */
/**  @param retval When set to True indicates the requested sharpness level is supported. False if not supported. */
extern FResult papi_vf_vim_SharpnessSupported( int mode, Bool * retval );



/*
 *  interface vio of type IPlfApiVideoOutput
 */

/** @defgroup papi_vf_vio papi_vf_vio: VideoOutput
  * @ingroup papi_vf */

/** @defgroup papi_vf_vio_Datatypes Datatypes 
  * @ingroup papi_vf_vio */
/**
 *   @defgroup papi_vf_vio_BlankingColor Blanking Color
 *   @ingroup papi_vf_vio_Datatypes
 *   The supported blanking colors.
 *   @{ */
#define papi_vf_vio_MinBlankingColor 0
#define papi_vf_vio_BlankingColorBlue 0
#define papi_vf_vio_BlankingColorBlack 1
#define papi_vf_vio_BlankingColorGrey 2
#define papi_vf_vio_MaxBlankingColor 2
/*   @} */

/**
 *   @defgroup papi_vf_vio_Hue Hue
 *   @ingroup papi_vf_vio_Datatypes
 *   Range of allowed Hue settings.
 *   @{ */
#define papi_vf_vio_MinHue 0
#define papi_vf_vio_MaxHue 100
/*   @} */

/**
 *   @defgroup papi_vf_vio_Tint Tint
 *   @ingroup papi_vf_vio_Datatypes
 *   Supported levels of Tint.
 *   @{ */
#define papi_vf_vio_MinTint 0
#define papi_vf_vio_CoolTint 0
#define papi_vf_vio_NormalTint 1
#define papi_vf_vio_WarmTint 2
#define papi_vf_vio_CustomTint 3
#define papi_vf_vio_MaxTint 3
/*   @} */

/**
 *   @defgroup papi_vf_vio_Brightness Brightness
 *   @ingroup papi_vf_vio_Datatypes
 *   Range of allowed Brightness settings.
 *   @{ */
#define papi_vf_vio_MinBrightness 0
#define papi_vf_vio_MaxBrightness 100
/*   @} */

/**
 *   @defgroup papi_vf_vio_Color Color
 *   @ingroup papi_vf_vio_Datatypes
 *   Range of allowed Color settings.
 *   @{ */
#define papi_vf_vio_MinColor 0
#define papi_vf_vio_MaxColor 100
/*   @} */

/**
 *   @defgroup papi_vf_vio_Contrast Contrast
 *   @ingroup papi_vf_vio_Datatypes
 *   Range of allowed Contrast settings.
 *   @{ */
#define papi_vf_vio_MinContrast 0
#define papi_vf_vio_MaxContrast 100
/*   @} */

/** @defgroup papi_vf_vio_Functions Functions 
  * @ingroup papi_vf_vio */

/**  @ingroup papi_vf_vio_Functions */
/**  Blanks or unblanks the video mixer output (video and graphics planes). Even if unblank is indicated, the actual output may still be blanked. The blanking color is set via SetBlankingColor. */ 
/**  @param blanked The requested blanking status. */
extern FResult papi_vf_vio_SetBlanked( Bool blanked );

/**  @ingroup papi_vf_vio_Functions */
/**  Sets ths color to be used when blanking the video output. */ 
/**  @param color The color for blanking. See datatype \ref papi_vf_vio_BlankingColor. */
extern FResult papi_vf_vio_SetBlankingColor( int color );

/**  @ingroup papi_vf_vio_Functions */
/**  Enable the auto blanking behaviour in the platform. The platform will automatically blank the complete video mixer output (video and graphics), e.g. during a program change. */ 
/**  @param mode True to enable the auto blanking behaviour. False otherwise. */
extern FResult papi_vf_vio_EnableBlanking( Bool mode );

/**  @ingroup papi_vf_vio_Functions */
/**  Returns the set level of brightness. */ 
/**  @param retval  See datatype \ref papi_vf_vio_Brightness. */
extern FResult papi_vf_vio_GetBrightness( int * retval );

/**  @ingroup papi_vf_vio_Functions */
/**  Sets the brightness to the specified value. When this function returns, the platform has changed the brightness and it will NOT sent an OnBrightnessChanged notification. */ 
/**  @param brightness The brightness value. See datatype \ref papi_vf_vio_Brightness. */
extern FResult papi_vf_vio_SetBrightness( int brightness );

/**  @ingroup papi_vf_vio_Functions */
/**  Returns the set color. */ 
/**  @param retval  See datatype \ref papi_vf_vio_Color. */
extern FResult papi_vf_vio_GetColor( int * retval );

/**  @ingroup papi_vf_vio_Functions */
/**  Sets the color to the specified value. */ 
/**  @param color The color value. See datatype \ref papi_vf_vio_Color. */
extern FResult papi_vf_vio_SetColor( int color );

/**  @ingroup papi_vf_vio_Functions */
/**  Returns the set level of contrast. */ 
/**  @param retval  See datatype \ref papi_vf_vio_Contrast. */
extern FResult papi_vf_vio_GetContrast( int * retval );

/**  @ingroup papi_vf_vio_Functions */
/**  Sets the contrast to the specified value. When this function returns, the platform has changed the contrast and it will NOT sent an OnContrastChanged notification. */ 
/**  @param contrast The contrast value. See datatype \ref papi_vf_vio_Contrast. */
extern FResult papi_vf_vio_SetContrast( int contrast );

/**  @ingroup papi_vf_vio_Functions */
/**  Returns the field frequency of the input signal. */ 
/**  @param retval The field frequency (in Hz). */
extern FResult papi_vf_vio_GetFieldFrequency( int * retval );

/**  @ingroup papi_vf_vio_Functions */
/**  Indicates that a call to SetHue is allowed. */ 
/**  @param retval Is set to True when calling SetHue is allowed. Is set to False otherwise. */
extern FResult papi_vf_vio_HueAllowed( Bool * retval );

/**  @ingroup papi_vf_vio_Functions */
/**  Returns the set level of hue. */ 
/**  @param retval  See datatype \ref papi_vf_vio_Hue. */
extern FResult papi_vf_vio_GetHue( int * retval );

/**  @ingroup papi_vf_vio_Functions */
/**  Sets the hue to the specified value. */ 
/**  @param hue The hue value. See datatype \ref papi_vf_vio_Hue. */
extern FResult papi_vf_vio_SetHue( int hue );

/**  @ingroup papi_vf_vio_Functions */
/**  Indicates if setting hue is supported. */ 
/**  @param retval Is set to True when hue is supported. */
extern FResult papi_vf_vio_HueSupported( Bool * retval );

/**  @ingroup papi_vf_vio_Functions */
/**  Returns the previously set blanking status. Even if not blanked is returned, the actual output may still be blanked. */ 
/**  @param retval The previously set value via SetBlanked. */
extern FResult papi_vf_vio_IsBlanked( Bool * retval );

/**  @ingroup papi_vf_vio_Functions */
/**  Returns the previously set value for EnableBlanking. */ 
/**  @param retval The value set earlier in EnableBlanking. */
extern FResult papi_vf_vio_IsBlankingEnabled( Bool * retval );

/**  @ingroup papi_vf_vio_Functions */
/**  Indicates if the input signal is interlaced. */ 
/**  @param retval Is set to True when the input signal is interlaces. Is set to False when the input signal is progressive. */
extern FResult papi_vf_vio_IsInterlaced( Bool * retval );

/**  @ingroup papi_vf_vio_Functions */
extern FResult papi_vf_vio_NumberOfLines( int * retval );

/**  @ingroup papi_vf_vio_Functions */
/**  Returns the set level of tint. */ 
/**  @param retval  See datatype \ref papi_vf_vio_Tint. */
extern FResult papi_vf_vio_GetTint( int * retval );

/**  @ingroup papi_vf_vio_Functions */
/**  Sets the tint (whitepoint) to the specified value. */ 
/**  @param tint The tint value. See datatype \ref papi_vf_vio_Tint. */
extern FResult papi_vf_vio_SetTint( int tint );

/**  @ingroup papi_vf_vio_Functions */
/**  Indicates if the requested tint value can be set. */ 
/**  @param tint The requested tint value. See datatype \ref papi_vf_vio_Tint. */
/**  @param retval Is set to True when the requested tint value is supported. */
extern FResult papi_vf_vio_TintSupported( int tint, Bool * retval );

#define papi_vf_vio_MinTilt -255
#define papi_vf_vio_NoTilt 0
#define papi_vf_vio_MaxTilt 255


/*
 *  interface vmt of type IPlfApiVideoMute
 */

/** @defgroup papi_vf_vmt papi_vf_vmt: VideoMute
  * Video mute only applies to blanking of the video layer. The blanking functionality on the mixer interface on the other hand applies to both the video and graphics layers.
  * @ingroup papi_vf */

/** @defgroup papi_vf_vmt_Datatypes Datatypes 
  * @ingroup papi_vf_vmt */
/**
 *   @defgroup papi_vf_vmt_ColorRange Mute Color
 *   @ingroup papi_vf_vmt_Datatypes
 *   Range of possible color settings for mute.
 *   @{ */
#define papi_vf_vmt_MinColor 0
#define papi_vf_vmt_MaxColor 255
/*   @} */

/** @defgroup papi_vf_vmt_Functions Functions 
  * @ingroup papi_vf_vmt */

/**  @ingroup papi_vf_vmt_Functions */
/**  This enables and disables auto blanking inside the platform. When auto blanking is enabled, the platform will blank the video output in case of bad signal conditions (no video, zapping, ...) to avoid transient video being displayed to the user. Auto blanking is normally enabled. The option to disable it, is typically used for testing and servicing purposes. Note that this auto blanking behaviour applies ONLY to the video layer. EnableAutoBlank() on the mixer applies to both video and graphics layers. */ 
/**  @param mode True to enable auto blanking of the video layer, False to disable it. */
extern FResult papi_vf_vmt_SetAutoMute( Bool mode );

/**  @ingroup papi_vf_vmt_Functions */
/**  Indicates if auto mute is supported. */ 
/**  @param retval Is set to True when auto mute is supported. Is set to False otherwise. */
extern FResult papi_vf_vmt_IsAutoMuteSupported( Bool * retval );

/**  @ingroup papi_vf_vmt_Functions */
/**  Indicates if the specified mute color is supported. */ 
/**  @param red The red component of the mute color. See datatype \ref papi_vf_vmt_ColorRange. */
/**  @param green The green component of the mute color. See datatype \ref papi_vf_vmt_ColorRange. */
/**  @param blue The blue component of the mute color. See datatype \ref papi_vf_vmt_ColorRange. */
/**  @param retval Is set to True if the specified mute color is supported. */
extern FResult papi_vf_vmt_IsMuteColorSupported( int red, int green, int blue, Bool * retval );

/**  @ingroup papi_vf_vmt_Functions */
/**  Indicates if mute can be set. */ 
/**  @param retval Returns True if mute can be set. */
extern FResult papi_vf_vmt_IsMuteSupported( Bool * retval );

/**  @ingroup papi_vf_vmt_Functions */
/**  Returns the actual mute color. The platform may internally overrule the mute color that was set. It provides the actual mute color on the video layer. */ 
/**  @param red The red component of the mute color. See datatype \ref papi_vf_vmt_ColorRange. */
/**  @param green The green component of the mute color. See datatype \ref papi_vf_vmt_ColorRange. */
/**  @param blue The blue component of the mute color. See datatype \ref papi_vf_vmt_ColorRange. */
extern FResult papi_vf_vmt_GetActualMuteColor( int * red, int * green, int * blue );

/**  @ingroup papi_vf_vmt_Functions */
/**  Returns the color used when muting the video. */ 
/**  @param red Set to the red component of the mute color. See datatype \ref papi_vf_vmt_ColorRange. */
/**  @param green Set to the green component of the mute color. See datatype \ref papi_vf_vmt_ColorRange. */
/**  @param blue Set to the blue component of the mute color. See datatype \ref papi_vf_vmt_ColorRange. */
extern FResult papi_vf_vmt_GetMuteColor( int * red, int * green, int * blue );

/**  @ingroup papi_vf_vmt_Functions */
/**  Specifies the color to be used when muting the video. */ 
/**  @param red The red component of the mute color. See datatype \ref papi_vf_vmt_ColorRange. */
/**  @param green The green component of the mute color. See datatype \ref papi_vf_vmt_ColorRange. */
/**  @param blue The blue component of the mute color. See datatype \ref papi_vf_vmt_ColorRange. */
extern FResult papi_vf_vmt_SetMuteColor( int red, int green, int blue );

/**  @ingroup papi_vf_vmt_Functions */
/**  Indicates if the video output is actually muted. Either as a result of SetMute or by implementation defined behavior. It provides the actual blank status (on/off) on the video layer. */ 
/**  @param retval  */
extern FResult papi_vf_vmt_GetActualMute( Bool * retval );

/**  @ingroup papi_vf_vmt_Functions */
/**  Returns the previously set auto mute status. */ 
/**  @param retval The set auto mute status. */
extern FResult papi_vf_vmt_GetAutoMute( Bool * retval );

/**  @ingroup papi_vf_vmt_Functions */
/**  Returns if mute is set or not. */ 
/**  @param retval The mute value set. */
extern FResult papi_vf_vmt_GetMute( Bool * retval );

/**  @ingroup papi_vf_vmt_Functions */
/**  Mutes the video layer. Depending on the platform implementation and run-time conditions the request can be handled synchronously or asynchronously by the platform. */ 
/**  @param mode Mutes the video when set to True. Unmutes otherwise. */
/**  @param retval True is returned if the platform could handle the request synchronously, which means that the platform has muted or unmuted the video layer as requested when this function call returns. False is returned if the platform handles the request asynchronously, which means that the request has not yet been completed by the platform when this function call returns and the platform will call the corresponding notification function when it has completed the request. */
extern FResult papi_vf_vmt_SetMute( Bool mode, Bool * retval );



/*
 *  interface vvmt of type IPlfApiVideoMute
 */

/** @defgroup papi_vf_vvmt papi_vf_vvmt: VideoMute
  * Video mute only applies to blanking of the video layer. The blanking functionality on the mixer interface on the other hand applies to both the video and graphics layers.
  * @ingroup papi_vf */

/** @defgroup papi_vf_vvmt_Datatypes Datatypes 
  * @ingroup papi_vf_vvmt */
/**
 *   @defgroup papi_vf_vvmt_ColorRange Mute Color
 *   @ingroup papi_vf_vvmt_Datatypes
 *   Range of possible color settings for mute.
 *   @{ */
#define papi_vf_vvmt_MinColor 0
#define papi_vf_vvmt_MaxColor 255
/*   @} */

/** @defgroup papi_vf_vvmt_Functions Functions 
  * @ingroup papi_vf_vvmt */

/**  @ingroup papi_vf_vvmt_Functions */
/**  This enables and disables auto blanking inside the platform. When auto blanking is enabled, the platform will blank the video output in case of bad signal conditions (no video, zapping, ...) to avoid transient video being displayed to the user. Auto blanking is normally enabled. The option to disable it, is typically used for testing and servicing purposes. Note that this auto blanking behaviour applies ONLY to the video layer. EnableAutoBlank() on the mixer applies to both video and graphics layers. */ 
/**  @param mode True to enable auto blanking of the video layer, False to disable it. */
extern FResult papi_vf_vvmt_SetAutoMute( Bool mode );

/**  @ingroup papi_vf_vvmt_Functions */
/**  Indicates if auto mute is supported. */ 
/**  @param retval Is set to True when auto mute is supported. Is set to False otherwise. */
extern FResult papi_vf_vvmt_IsAutoMuteSupported( Bool * retval );

/**  @ingroup papi_vf_vvmt_Functions */
/**  Indicates if the specified mute color is supported. */ 
/**  @param red The red component of the mute color. See datatype \ref papi_vf_vvmt_ColorRange. */
/**  @param green The green component of the mute color. See datatype \ref papi_vf_vvmt_ColorRange. */
/**  @param blue The blue component of the mute color. See datatype \ref papi_vf_vvmt_ColorRange. */
/**  @param retval Is set to True if the specified mute color is supported. */
extern FResult papi_vf_vvmt_IsMuteColorSupported( int red, int green, int blue, Bool * retval );

/**  @ingroup papi_vf_vvmt_Functions */
/**  Indicates if mute can be set. */ 
/**  @param retval Returns True if mute can be set. */
extern FResult papi_vf_vvmt_IsMuteSupported( Bool * retval );

/**  @ingroup papi_vf_vvmt_Functions */
/**  Returns the actual mute color. The platform may internally overrule the mute color that was set. It provides the actual mute color on the video layer. */ 
/**  @param red The red component of the mute color. See datatype \ref papi_vf_vvmt_ColorRange. */
/**  @param green The green component of the mute color. See datatype \ref papi_vf_vvmt_ColorRange. */
/**  @param blue The blue component of the mute color. See datatype \ref papi_vf_vvmt_ColorRange. */
extern FResult papi_vf_vvmt_GetActualMuteColor( int * red, int * green, int * blue );

/**  @ingroup papi_vf_vvmt_Functions */
/**  Returns the color used when muting the video. */ 
/**  @param red Set to the red component of the mute color. See datatype \ref papi_vf_vvmt_ColorRange. */
/**  @param green Set to the green component of the mute color. See datatype \ref papi_vf_vvmt_ColorRange. */
/**  @param blue Set to the blue component of the mute color. See datatype \ref papi_vf_vvmt_ColorRange. */
extern FResult papi_vf_vvmt_GetMuteColor( int * red, int * green, int * blue );

/**  @ingroup papi_vf_vvmt_Functions */
/**  Specifies the color to be used when muting the video. */ 
/**  @param red The red component of the mute color. See datatype \ref papi_vf_vvmt_ColorRange. */
/**  @param green The green component of the mute color. See datatype \ref papi_vf_vvmt_ColorRange. */
/**  @param blue The blue component of the mute color. See datatype \ref papi_vf_vvmt_ColorRange. */
extern FResult papi_vf_vvmt_SetMuteColor( int red, int green, int blue );

/**  @ingroup papi_vf_vvmt_Functions */
/**  Indicates if the video output is actually muted. Either as a result of SetMute or by implementation defined behavior. It provides the actual blank status (on/off) on the video layer. */ 
/**  @param retval  */
extern FResult papi_vf_vvmt_GetActualMute( Bool * retval );

/**  @ingroup papi_vf_vvmt_Functions */
/**  Returns the previously set auto mute status. */ 
/**  @param retval The set auto mute status. */
extern FResult papi_vf_vvmt_GetAutoMute( Bool * retval );

/**  @ingroup papi_vf_vvmt_Functions */
/**  Returns if mute is set or not. */ 
/**  @param retval The mute value set. */
extern FResult papi_vf_vvmt_GetMute( Bool * retval );

/**  @ingroup papi_vf_vvmt_Functions */
/**  Mutes the video layer. Depending on the platform implementation and run-time conditions the request can be handled synchronously or asynchronously by the platform. */ 
/**  @param mode Mutes the video when set to True. Unmutes otherwise. */
/**  @param retval True is returned if the platform could handle the request synchronously, which means that the platform has muted or unmuted the video layer as requested when this function call returns. False is returned if the platform handles the request asynchronously, which means that the request has not yet been completed by the platform when this function call returns and the platform will call the corresponding notification function when it has completed the request. */
extern FResult papi_vf_vvmt_SetMute( Bool mode, Bool * retval );



/*
 *  interface vclipN of type IPlfApiVideoClipNotify
 */

/** @defgroup papi_vf_vclip papi_vf_vclip: VideoClip
  * @ingroup papi_vf */

/** @defgroup papi_vf_vclipN_Notifications Notifications 
  * @ingroup papi_vf_vclip */

/**  @ingroup papi_vf_vclipN_Notifications */
/**  Notification that the specified clip buffer has been decoded by the decoder. */ 
/**  @param id The id identifying the clip buffer for which the contained data has been decoded. */
extern void papi_vf_vclipN_OnBufferAvailable( Nat32 id );

/**  @ingroup papi_vf_vclipN_Notifications */
/**  Notification that the clip buffer with endOfData=True containing the last data of the clip has been decoded or if the size of the buffer was zero. This notification is coming from the decoder and does not signal that rendering of the clip has also finished. */ 
extern void papi_vf_vclipN_OnEndOfData( void );

/**  @ingroup papi_vf_vclipN_Notifications */
/**  Notification that the decoder has started after a start request. It signals that the decoder is active and ready to start the decoding process or, in case data was already offered to it, already decoding data. */ 
extern void papi_vf_vclipN_OnStarted( void );

/**  @ingroup papi_vf_vclipN_Notifications */
/**  Notification that the decoder has stopped after a stop request. It signals that the decoder is inactive. Also no other notifications will follow unless the decoder is started again. */ 
extern void papi_vf_vclipN_OnStopped( void );



/*
 *  interface vfrN of type IPlfApiVideoFreezeNotify
 */

/** @defgroup papi_vf_vfr papi_vf_vfr: VideoFreeze
  * @ingroup papi_vf */

/** @defgroup papi_vf_vfrN_Notifications Notifications 
  * @ingroup papi_vf_vfr */

/**  @ingroup papi_vf_vfrN_Notifications */
/**  This notification is raised when the allowedness of freeze changed. */ 
extern void papi_vf_vfrN_OnFreezeAllowedChanged( void );

/**  @ingroup papi_vf_vfrN_Notifications */
/**  This notification is raised when the (un)freeze is completed. */ 
/**  @param Frozen Is set to True when frozen. False otherwise. */
extern void papi_vf_vfrN_OnFreezeCompleted( Bool Frozen );



/*
 *  interface vimN of type IPlfApiVideoImprovementNotify
 */

/** @defgroup papi_vf_vim papi_vf_vim: VideoImprovement
  * @ingroup papi_vf */

/** @defgroup papi_vf_vimN_Notifications Notifications 
  * @ingroup papi_vf_vim */

/**  @ingroup papi_vf_vimN_Notifications */
/**  This notification is raised when the color enhancement level changed autonomously inside the platform. */ 
/**  @param mode The current color enhancement level. */
extern void papi_vf_vimN_OnColorEnhancementChanged( int mode );

/**  @ingroup papi_vf_vimN_Notifications */
/**  This notification is raised when there is a change in allowing calls to SetDigitalNaturalMotion. */ 
extern void papi_vf_vimN_OnDigitalNaturalMotionAllowednessChanged( void );

/**  @ingroup papi_vf_vimN_Notifications */
/**  This notification is raised when the digital natural motion level is changed. */ 
/**  @param mode The current level of digital natural motion. */
extern void papi_vf_vimN_OnDigitalNaturalMotionChanged( int mode );

/**  @ingroup papi_vf_vimN_Notifications */
/**  This notification is raised when the DNR level is changed. */ 
/**  @param mode The current DNR level. */
extern void papi_vf_vimN_OnDnrChanged( int mode );

/**  @ingroup papi_vf_vimN_Notifications */
/**  This notification is raised when the dynamic contrast level changed autonomously inside the platform. */ 
/**  @param mode The current dynamic constrast level. */
extern void papi_vf_vimN_OnDynamicContrastChanged( int mode );

/**  @ingroup papi_vf_vimN_Notifications */
/**  This notification is raised when LTI is turned on or off. */ 
/**  @param onoff The current state of LTI. */
extern void papi_vf_vimN_OnLtiChanged( Bool onoff );

/**  @ingroup papi_vf_vimN_Notifications */
/**  This notification is raised when the MPEG noise reduction is turned on or off. */ 
/**  @param onoff Is True when the MPEG noise reduction is enabled. False otherwise. */
extern void papi_vf_vimN_OnMpegNoiseReductionChanged( Bool onoff );

/**  @ingroup papi_vf_vimN_Notifications */
/**  This notification is raised when the sharpness level changed autonomously inside the platform. */ 
/**  @param mode The current level of sharpness. */
extern void papi_vf_vimN_OnSharpnessChanged( int mode );



/*
 *  interface vioN of type IPlfApiVideoOutputNotify
 */

/** @defgroup papi_vf_vio papi_vf_vio: VideoOutput
  * @ingroup papi_vf */

/** @defgroup papi_vf_vioN_Notifications Notifications 
  * @ingroup papi_vf_vio */

/**  @ingroup papi_vf_vioN_Notifications */
/**  This notification is raised when the blanking state changed autonomously in the platform. */ 
extern void papi_vf_vioN_OnBlankedChanged( void );

/**  @ingroup papi_vf_vioN_Notifications */
/**  This notification is raised when the brightness changes inside the platform. */ 
extern void papi_vf_vioN_OnBrightnessChanged( void );

/**  @ingroup papi_vf_vioN_Notifications */
/**  This notification is raised when the color changes inside the platform. */ 
extern void papi_vf_vioN_OnColorChanged( void );

/**  @ingroup papi_vf_vioN_Notifications */
/**  This notification is raised when the contrast changes inside the platform. */ 
extern void papi_vf_vioN_OnContrastChanged( void );

/**  @ingroup papi_vf_vioN_Notifications */
/**  This notification is raised when the enable blanking state changes autonomously in the platform. */ 
extern void papi_vf_vioN_OnEnableBlankingChanged( void );

/**  @ingroup papi_vf_vioN_Notifications */
/**  This notification is raised when the field frequency of the video signal changed. */ 
extern void papi_vf_vioN_OnFieldFrequencyChanged( void );

/**  @ingroup papi_vf_vioN_Notifications */
/**  This notification is raised when the allowednes for hue changed. */ 
extern void papi_vf_vioN_OnHueAllowedChanged( void );

/**  @ingroup papi_vf_vioN_Notifications */
/**  This notification is raised when the hue changed. */ 
extern void papi_vf_vioN_OnHueChanged( void );

/**  @ingroup papi_vf_vioN_Notifications */
/**  This notification is raised when the video signal changed from interlaced to progressive or the other way around. */ 
extern void papi_vf_vioN_OnInterlacedChanged( void );

/**  @ingroup papi_vf_vioN_Notifications */
/**  This notification is raised when the vertical resolution (number of lines) of the video signal changed. */ 
extern void papi_vf_vioN_OnNumberOfLinesChanged( void );

/**  @ingroup papi_vf_vioN_Notifications */
/**  This notification is raised when the tint (color temperatur) changed. */ 
extern void papi_vf_vioN_OnTintChanged( void );



/*
 *  interface vmtN of type IPlfApiVideoMuteNotify
 */

/** @defgroup papi_vf_vmt papi_vf_vmt: VideoMute
  * @ingroup papi_vf */

/** @defgroup papi_vf_vmtN_Notifications Notifications 
  * @ingroup papi_vf_vmt */

/**  @ingroup papi_vf_vmtN_Notifications */
/**  This notifies autonomous changes in mute status of the video output. */ 
extern void papi_vf_vmtN_OnActualMuteChanged( void );

/**  @ingroup papi_vf_vmtN_Notifications */
/**  This notifies autonomous changes in the mute color of the video output. */ 
extern void papi_vf_vmtN_OnActualMuteColorChanged( void );



/*
 *  interface vvmtN of type IPlfApiVideoMuteNotify
 */

/** @defgroup papi_vf_vvmt papi_vf_vvmt: VideoMute
  * @ingroup papi_vf */

/** @defgroup papi_vf_vvmtN_Notifications Notifications 
  * @ingroup papi_vf_vvmt */

/**  @ingroup papi_vf_vvmtN_Notifications */
/**  This notifies autonomous changes in mute status of the video output. */ 
extern void papi_vf_vvmtN_OnActualMuteChanged( void );

/**  @ingroup papi_vf_vvmtN_Notifications */
/**  This notifies autonomous changes in the mute color of the video output. */ 
extern void papi_vf_vvmtN_OnActualMuteColorChanged( void );


