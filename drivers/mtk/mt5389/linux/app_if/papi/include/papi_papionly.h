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

#ifndef PAPI_PAPIONLY_H
#define PAPI_PAPIONLY_H

#include "papi_types.h"


typedef struct
{
	CecOpcode opcode;
	int length;
	Nat8 buf[16];
} IOCTL_CEC_WAKEUP_BUFFER_T;

/**
 *   @file papi_papionly.h
 */
/** @defgroup papi_papionly  */

/*
 *  interface key of type IPlfApiEnable
 */

/** @defgroup papi_papionly_key papi_papionly_key: Enable
  * @ingroup papi_papionly */

/** @defgroup papi_papionly_key_Functions Functions 
  * @ingroup papi_papionly_key */

/**  @ingroup papi_papionly_key_Functions */
extern FResult papi_papionly_key_Disable( void );

/**  @ingroup papi_papionly_key_Functions */
extern FResult papi_papionly_key_Enable( void );

/**  @ingroup papi_papionly_key_Functions */
extern FResult papi_papionly_key_IsEnabled( Bool * retval );



/*
 *  interface lkb of type IPlfApiEnable
 */

/** @defgroup papi_papionly_lkb papi_papionly_lkb: Enable
  * @ingroup papi_papionly */

/** @defgroup papi_papionly_lkb_Functions Functions 
  * @ingroup papi_papionly_lkb */

/**  @ingroup papi_papionly_lkb_Functions */
extern FResult papi_papionly_lkb_Disable( void );

/**  @ingroup papi_papionly_lkb_Functions */
extern FResult papi_papionly_lkb_Enable( void );

/**  @ingroup papi_papionly_lkb_Functions */
extern FResult papi_papionly_lkb_IsEnabled( Bool * retval );



/*
 *  interface lvds of type IPlfApiEnable
 */

/** @defgroup papi_papionly_lvds papi_papionly_lvds: Enable
  * @ingroup papi_papionly */

/** @defgroup papi_papionly_lvds_Functions Functions 
  * @ingroup papi_papionly_lvds */

/**  @ingroup papi_papionly_lvds_Functions */
extern FResult papi_papionly_lvds_Disable( void );

/**  @ingroup papi_papionly_lvds_Functions */
extern FResult papi_papionly_lvds_Enable( void );

/**  @ingroup papi_papionly_lvds_Functions */
extern FResult papi_papionly_lvds_IsEnabled( Bool * retval );



/*
 *  interface vmix of type tmIVmix
 */

/** @defgroup papi_papionly_vmix papi_papionly_vmix: tmIVmix
  * The Vmix interface is used to control the video mixer. The video mixer combines multiple video and graphics layers into a single video output stream. The video inputs towards the video mixer are associated with individual video layers of the mixer. Individual video layers are controlled via the VmixVidLayer interface.
  * @ingroup papi_papionly */

/** @defgroup papi_papionly_vmix_Datatypes Datatypes 
  * @ingroup papi_papionly_vmix */
/**
 *   @defgroup papi_papionly_vmix_ScanType Scan Type
 *   @ingroup papi_papionly_vmix_Datatypes
 *   The scan type of the signal: interlaced or progressive.
 *   @{ */
#define papi_papionly_vmix_ScanTypeInvalid -2
#define papi_papionly_vmix_ScanTypeUnknown -1
#define papi_papionly_vmix_ScanTypeInterlaced 0
#define papi_papionly_vmix_ScanTypeProgressive 1
/*   @} */

/**
 *   @defgroup papi_papionly_vmix_FieldRate Mixer Output Field Rate
 *   @ingroup papi_papionly_vmix_Datatypes
 *   The field rate of the output of the mixer.
 *   @{ */
#define papi_papionly_vmix_FieldRateInvalid -2
#define papi_papionly_vmix_FieldRateUnknown -1
#define papi_papionly_vmix_FieldRate50Hz 0
#define papi_papionly_vmix_FieldRate60Hz 1
#define papi_papionly_vmix_FieldRate75Hz 2
#define papi_papionly_vmix_FieldRate100Hz 3
/*   @} */

/** @defgroup papi_papionly_vmix_Functions Functions 
  * @ingroup papi_papionly_vmix */

/**  @ingroup papi_papionly_vmix_Functions */
/**  Enable the auto blanking behaviour in the platform. */ 
/**  @param enableOnOutputResolutionChanged True, to blank the output if the output resolution changes. */
/**  @param enableOnFieldRateChanged True, to blank the output if the field rate changes. */
/**  @param enableOnScanTypeChanged True, to blank the output if the scan type changes. */
/**  @param blankColor If the output is auto blanked, the blanking color that needs to be shown. */
extern tmErrorCode_t papi_papionly_vmix_EnableAutoBlank( Bool enableOnOutputResolutionChanged, Bool enableOnFieldRateChanged, Bool enableOnScanTypeChanged, VMixColor blankColor );

/**  @ingroup papi_papionly_vmix_Functions */
/**  Deactivate auto blank. */ 
extern tmErrorCode_t papi_papionly_vmix_AutoBlankUnblank( void );

/**  @ingroup papi_papionly_vmix_Functions */
/**  Get the color of the background layer */ 
/**  @param pBgColor Return value with the background color. */
extern tmErrorCode_t papi_papionly_vmix_GetBgColor( VMixColor * pBgColor );

/**  @ingroup papi_papionly_vmix_Functions */
/**  Set the color of the background layer */ 
/**  @param bgColor The background color that has to be set. */
extern tmErrorCode_t papi_papionly_vmix_SetBgColor( VMixColor bgColor );

/**  @ingroup papi_papionly_vmix_Functions */
/**  Returns the auto blanking settings in the platform. */ 
/**  @param pBlankOnOutputResolutionChangedEnabled True, if the output is blanked if the output resolution changes. */
/**  @param pBlankOnFieldRateChangedEnabled True, if the output is blanked if the field rate changes. */
/**  @param pBlankOnScanTypeChangedEnabled True, if the output is blanked if the scan type changes. */
/**  @param pBlankColor If the output is auto blanked, the blanking color that is shown. */
extern tmErrorCode_t papi_papionly_vmix_GetAutoBlankEnabled( Bool * pBlankOnOutputResolutionChangedEnabled, Bool * pBlankOnFieldRateChangedEnabled, Bool * pBlankOnScanTypeChangedEnabled, VMixColor * pBlankColor );

/**  @ingroup papi_papionly_vmix_Functions */
/**  Blanks or unblanks the video mixer output. The result of blanking the output is that all video and graphics layers are hidden and that the requested blanking color is shown. */ 
/**  @param blank True to blank the output, false to unblank the output. */
/**  @param blankColor If the output is blanked, the blanking color that needs to be shown. */
extern tmErrorCode_t papi_papionly_vmix_BlankOutput( Bool blank, VMixColor blankColor );

/**  @ingroup papi_papionly_vmix_Functions */
/**  Returns the number of video layers supported by the video mixer. */ 
/**  @param pNrLayers Output parameter with the number of video layers supported by the video mixer. */
extern tmErrorCode_t papi_papionly_vmix_GetSuppNrLayers( Nat32 * pNrLayers );

/**  @ingroup papi_papionly_vmix_Functions */
/**  Returns whether the output is blanked or not. */ 
/**  @param pBlanked True if the output is blanked, false if not. */
/**  @param pBlankColor If the output is blanked, the blanking color that is shown. */
extern tmErrorCode_t papi_papionly_vmix_GetOutputBlanked( Bool * pBlanked, VMixColor * pBlankColor );

/**  @ingroup papi_papionly_vmix_Functions */
/**  Returns the output properties of the video mixer. */ 
/**  @param pWidth Output parameter with the number of pixels per line. */
/**  @param pHeight Output parameter with the number of lines. */
/**  @param pFieldRate Output parameter with the field rate. See datatype \ref papi_papionly_vmix_FieldRate. */
/**  @param pScanType Output parameter with the scan type. See datatype \ref papi_papionly_vmix_ScanType. */
extern tmErrorCode_t papi_papionly_vmix_GetOutputProperties( Nat32 * pWidth, Nat32 * pHeight, int * pFieldRate, int * pScanType );



/*
 *  interface vmixvid of type tmIVmixVidLayer
 */

/** @defgroup papi_papionly_vmixvid papi_papionly_vmixvid: tmIVmixVidLayer
  * This interface is used to control the individual video layers of the video mixer.
  * @ingroup papi_papionly */

/** @defgroup papi_papionly_vmixvid_Datatypes Datatypes 
  * @ingroup papi_papionly_vmixvid */
/**
 *   @defgroup papi_papionly_vmixvid_ScaleMode Scale Mode
 *   @ingroup papi_papionly_vmixvid_Datatypes
 *   
 *   @{ */
#define papi_papionly_vmixvid_ScaleModeNoScaling 1
#define papi_papionly_vmixvid_ScaleModeLinear 2
#define papi_papionly_vmixvid_ScaleModePanorama 4
#define papi_papionly_vmixvid_ScaleModeHorizontal 8
/*   @} */

/** @defgroup papi_papionly_vmixvid_Functions Functions 
  * @ingroup papi_papionly_vmixvid */

/**  @ingroup papi_papionly_vmixvid_Functions */
/**  The changes specified by SetLayerWindow(), SetDstWindow(), SetSrcWindow(), SetScaleMode(), and SetPanoramaMidScale() are carried out smoothly in the specified amount of time. This is handled asynchronously and the completion of the operation is signalled via a notification function. */ 
/**  @param layerNr Video layer number. */
/**  @param syncWithNextInput True to wait for the next field/frame, false to start immediately. */
/**  @param nrMilliSeconds Number of milliseconds that the transition should take. */
extern tmErrorCode_t papi_papionly_vmixvid_ActivateNewWindowSettings( Nat32 layerNr, Bool syncWithNextInput, Nat32 nrMilliSeconds );

/**  @ingroup papi_papionly_vmixvid_Functions */
/**  Returns the background color for this layer. */ 
/**  @param layerNr Video layer number. */
/**  @param pBgColor Return value with the background color for this layer. */
extern tmErrorCode_t papi_papionly_vmixvid_GetBgColor( Nat32 layerNr, VMixColor * pBgColor );

/**  @ingroup papi_papionly_vmixvid_Functions */
/**  Set the background color for this layer. */ 
/**  @param layerNr Video layer number. */
/**  @param bgColor The background color that has to be set. */
extern tmErrorCode_t papi_papionly_vmixvid_SetBgColor( Nat32 layerNr, VMixColor bgColor );

/**  @ingroup papi_papionly_vmixvid_Functions */
/**  Returns whether a background color is supported for this layer or not. */ 
/**  @param layerNr Video layer number. */
/**  @param pBgSupp True if background color is supported, false otherwise. */
extern tmErrorCode_t papi_papionly_vmixvid_GetBgSupp( Nat32 layerNr, Bool * pBgSupp );

/**  @ingroup papi_papionly_vmixvid_Functions */
/**  Return the coordinates of the destination window on this layer. */ 
/**  @param layerNr Video layer number. */
/**  @param pDstRect Return value with the coordinates of the destination window. */
extern tmErrorCode_t papi_papionly_vmixvid_GetDstWindow( Nat32 layerNr, VMixRect * pDstRect );

/**  @ingroup papi_papionly_vmixvid_Functions */
/**  Sets the coordinates of the destination window on this layer. The video mixer commits these new settings after an ActiveNewWindowSettings(). */ 
/**  @param layerNr Video layer number. */
/**  @param dstRect Coordinates of the destination window relative to the layer window. */
extern tmErrorCode_t papi_papionly_vmixvid_SetDstWindow( Nat32 layerNr, VMixRect dstRect );

/**  @ingroup papi_papionly_vmixvid_Functions */
/**  Returns whether this layer is hidden or shown. */ 
/**  @param layerNr Video layer number. */
/**  @param pHidden Return value is true if this layer is hidden, false if this layer is shown. */
extern tmErrorCode_t papi_papionly_vmixvid_GetHidden( Nat32 layerNr, Bool * pHidden );

/**  @ingroup papi_papionly_vmixvid_Functions */
/**  Hide or show this layer. */ 
/**  @param layerNr Video layer number. */
/**  @param hide True to hide this layer, false to show this layer. */
extern tmErrorCode_t papi_papionly_vmixvid_Hide( Nat32 layerNr, Bool hide );

/**  @ingroup papi_papionly_vmixvid_Functions */
/**  Return the active image area of the input of this layer (excluding black bars). If there is no information about the active image area, then (0,0,0,0) is returned. */ 
/**  @param layerNr Video layer number. */
/**  @param pActiveImageArea Output parameter with the active image area. */
extern tmErrorCode_t papi_papionly_vmixvid_GetInputActiveImageArea( Nat32 layerNr, VMixRect * pActiveImageArea );

/**  @ingroup papi_papionly_vmixvid_Functions */
/**  Return the aspect ratio of the input of this layer in the format numerator:denominator. If there is no information about the aspect ratio then numerator=0 and denominator=1. */ 
/**  @param layerNr Video layer number. */
/**  @param pNumerator Output parameter with the aspect ratio of the input, numerator part. */
/**  @param pDenominator Output parameter with the aspect ratio of the input, denominator part. */
extern tmErrorCode_t papi_papionly_vmixvid_GetInputAspectRatio( Nat32 layerNr, Nat16 * pNumerator, Nat16 * pDenominator );

/**  @ingroup papi_papionly_vmixvid_Functions */
/**  Returns the input resolution of the video that enters the specified video layer of the mixer. Note that the VideoProperties interfaces returns the video format returned by the video decoder (before the mixer). It is mostly the same, except that usually the input for the video plane of the mixer is already deinterlaced. For this reason, this function only has a width and height return value. */ 
/**  @param layerNr Video layer number. */
/**  @param pWidth Output parameter with the number of pixels per line. */
/**  @param pHeight Output parameter with the number of lines. */
extern tmErrorCode_t papi_papionly_vmixvid_GetInputResolution( Nat32 layerNr, Nat32 * pWidth, Nat32 * pHeight );

/**  @ingroup papi_papionly_vmixvid_Functions */
/**  Return the coordinates of the layer window on this layer. */ 
/**  @param layerNr Video layer number. */
/**  @param pLayerRect Return value with the coordinates of the layer window. */
extern tmErrorCode_t papi_papionly_vmixvid_GetLayerWindow( Nat32 layerNr, VMixRect * pLayerRect );

/**  @ingroup papi_papionly_vmixvid_Functions */
/**  Sets the coordinates of the layer window on this layer. The video mixer commits these new settings after an ActiveNewWindowSettings(). */ 
/**  @param layerNr Video layer number. */
/**  @param layerRect Coordinates of the layer window relative to the output window. */
extern tmErrorCode_t papi_papionly_vmixvid_SetLayerWindow( Nat32 layerNr, VMixRect layerRect );

/**  @ingroup papi_papionly_vmixvid_Functions */
/**  Returns the maximum window width and height for this layer. */ 
/**  @param layerNr Video layer number. */
/**  @param pMaxWidth Returns the maximum window width. */
/**  @param pMaxHeight Returns the maximum window height. */
extern tmErrorCode_t papi_papionly_vmixvid_GetMaxLayerWindowSize( Nat32 layerNr, Nat32 * pMaxWidth, Nat32 * pMaxHeight );

/**  @ingroup papi_papionly_vmixvid_Functions */
/**  Return the minimum and maximum value for the mid scale ratio for panoramic scaling. */ 
/**  @param layerNr Video layer number. */
/**  @param pMinMidScaleRatio Minimum scale ratio in percentages. */
/**  @param pMaxMidScaleRatio Maximum scale ratio in percentages. */
extern tmErrorCode_t papi_papionly_vmixvid_GetPanoramaMidScaleRange( Nat32 layerNr, Nat32 * pMinMidScaleRatio, Nat32 * pMaxMidScaleRatio );

/**  @ingroup papi_papionly_vmixvid_Functions */
/**  Return the current mid scale ratio for panoramic scaling. */ 
/**  @param layerNr Video layer number. */
/**  @param pMidScaleRatio Return value with the current mid scale ratio for panoramic scaling. */
extern tmErrorCode_t papi_papionly_vmixvid_GetPanoramaMidScale( Nat32 layerNr, Nat32 * pMidScaleRatio );

/**  @ingroup papi_papionly_vmixvid_Functions */
/**  Set the mid scale ratio for panoramic scaling. */ 
/**  @param layerNr Video layer number. */
/**  @param midScaleRatio Mid scale ratio for panoramic scaling. */
extern tmErrorCode_t papi_papionly_vmixvid_SetPanoramaMidScale( Nat32 layerNr, Nat32 midScaleRatio );

/**  @ingroup papi_papionly_vmixvid_Functions */
/**  Returns for a particular scaling mode the minimum and maximum scale factors in percentages. */ 
/**  @param layerNr Video layer number. */
/**  @param scaleMode The specified scaling mode. See datatype \ref papi_papionly_vmixvid_ScaleMode. */
/**  @param pMinWidthPercentage The minimal horizontal scale factor. */
/**  @param pMinHeightPercentage The minimal vertical scale factor. */
/**  @param pMaxWidthPercentage The maximum horizontal scale factor. */
/**  @param pMaxHeightPercentage The maximum vertical scale factor. */
extern tmErrorCode_t papi_papionly_vmixvid_GetScaleRange( Nat32 layerNr, int scaleMode, Nat32 * pMinWidthPercentage, Nat32 * pMinHeightPercentage, Nat32 * pMaxWidthPercentage, Nat32 * pMaxHeightPercentage );

/**  @ingroup papi_papionly_vmixvid_Functions */
/**  Return the current scale mode. */ 
/**  @param layerNr Video layer number. */
/**  @param pScaleMode Return value with the current scale mode. See datatype \ref papi_papionly_vmixvid_ScaleMode. */
extern tmErrorCode_t papi_papionly_vmixvid_GetScaleMode( Nat32 layerNr, int * pScaleMode );

/**  @ingroup papi_papionly_vmixvid_Functions */
/**  Select a particular scale mode */ 
/**  @param layerNr Video layer number. */
/**  @param scaleMode Scale mode that is set. See datatype \ref papi_papionly_vmixvid_ScaleMode. */
extern tmErrorCode_t papi_papionly_vmixvid_SetScaleMode( Nat32 layerNr, int scaleMode );

/**  @ingroup papi_papionly_vmixvid_Functions */
/**  Returns the scale modes supported by this layer. */ 
/**  @param layerNr Video layer number. */
/**  @param pScaleModes Return value with the OR-ed supported scale modes. See datatype \ref papi_papionly_vmixvid_ScaleMode. */
extern tmErrorCode_t papi_papionly_vmixvid_GetSuppScaleModes( Nat32 layerNr, int * pScaleModes );

/**  @ingroup papi_papionly_vmixvid_Functions */
/**  Return the coordinates of the source window on this layer. */ 
/**  @param layerNr Video layer number. */
/**  @param pSrcRect Return value with the coordinates of the source window. */
extern tmErrorCode_t papi_papionly_vmixvid_GetSrcWindow( Nat32 layerNr, VMixRect * pSrcRect );

/**  @ingroup papi_papionly_vmixvid_Functions */
/**  Sets the coordinates of the source window on this layer. The video mixer commits these new settings after an ActiveNewWindowSettings(). Overscan will be controlled by the middleware and application layer by setting the src window. The platform should not clip any overscan itself without src window being set. */ 
/**  @param layerNr Video layer number. */
/**  @param srcRect Coordinates of the source window relative to the input window. */
extern tmErrorCode_t papi_papionly_vmixvid_SetSrcWindow( Nat32 layerNr, VMixRect srcRect );

/**  @ingroup papi_papionly_vmixvid_Functions */
/**  Returns the minimum window width and height for this layer. */ 
/**  @param layerNr Video layer number. */
/**  @param pMinWidth Returns the minimum window width. */
/**  @param pMinHeight Returns the minimum window height. */
extern tmErrorCode_t papi_papionly_vmixvid_GetMinWindowSize( Nat32 layerNr, Nat32 * pMinWidth, Nat32 * pMinHeight );



/*
 *  interface wakeup of type IPapiWakeup
 */

/** @defgroup papi_papionly_wakeup papi_papionly_wakeup: IPapiWakeup
  * @ingroup papi_papionly */

/** @defgroup papi_papionly_wakeup_Datatypes Datatypes 
  * @ingroup papi_papionly_wakeup */
/**
 *   @defgroup papi_papionly_wakeup_WakeupReason WakeupReasons
 *   @ingroup papi_papionly_wakeup_Datatypes
 *   Possible reasons for waking up
 *   @{ */
#define papi_papionly_wakeup_WakeupReasonAlarm 1
#define papi_papionly_wakeup_WakeupReasonRc 2
#define papi_papionly_wakeup_WakeupReasonLkb 4
#define papi_papionly_wakeup_WakeupReasonCec 8
#define papi_papionly_wakeup_WakeupReasonPin 16
#define papi_papionly_wakeup_WakeupReasonBoot 32
/*   @} */

/** @defgroup papi_papionly_wakeup_Functions Functions 
  * @ingroup papi_papionly_wakeup */

/**  @ingroup papi_papionly_wakeup_Functions */
/**  Indicates the CEC opcodes upon which the set should wakeup. Only has effect if the WakeupReasonCec is programmed with the <i>ProgramWakeupReasons</i> function */ 
/**  @param nbrcodes The number of opcodes contained in the following array */
/**  @param wkp_code Array containing the CEC opcodes, of size <i>nbrcodes</i> */
extern void papi_papionly_wakeup_ProgramCecWakeupCodes( int nbrcodes, CecOpcode * wkp_code );

/**  @ingroup papi_papionly_wakeup_Functions */
/**  Returns the programmed wakeup reasons (not to be confused with <i>GetActualWakeupReason</i>). It can be an OR'ing of multiple reasons */ 
/**  @param wakeupreasons The programmed wakeup reasons */
extern void papi_papionly_wakeup_GetProgrammedWakeupReasons( int * wakeupreasons );

/**  @ingroup papi_papionly_wakeup_Functions */
/**  Indicates the RC keys upon which the set should wakeup. Only has effect if the WakeupReasonRc is programmed with the <i>ProgramWakeupReasons</i> function */ 
/**  @param nbrkeys The number of keys contained in the following arrays */
/**  @param src Array containing the RC source type, of size <i>nbrkeys</i> */
/**  @param sys Array containing the RC system, of size <i>nbrkeys</i> */
/**  @param cmd Array containing the RC command, of size <i>nbrkeys</i> */
extern void papi_papionly_wakeup_ProgramWakeupKeysRc( int nbrkeys, int * src, int * sys, int * cmd );

/**  @ingroup papi_papionly_wakeup_Functions */
/**  Indicates the LKB command upon which the set should wakeup. Only has effect if the WakeupReasonLkb is programmed with the <i>ProgramWakeupReasons</i> function */ 
/**  @param cmd The LKB command */
extern void papi_papionly_wakeup_ProgramWakeupLocalKeyboard( int cmd );

/**  @ingroup papi_papionly_wakeup_Functions */
/**  Programs the required wakeup reasons. It can be an OR'ing of multiple reasons */ 
/**  @param wakeupreasons Indication of what the set should wake up from */
extern void papi_papionly_wakeup_ProgramWakeupReasons( int wakeupreasons );

/**  @ingroup papi_papionly_wakeup_Functions */
/**  Indicates the CEC message upon which the set has woken up. Only contains relevant data in case <i>GetActualWakeupReason</i> contained <i>WakeupReasonCec</i> */ 
/**  @param opcode [out] parameter containing the opcode */
/**  @param length [in/out] parameter containing the length of the message. The provider of this function should not write more data then this, and overwrite the value to the actual length of the message */
/**  @param buf [out] array containing the complete message. The caller should ensure it is big enough to contain <i>length</i> bytes */
extern void papi_papionly_wakeup_GetWakeupDetailsCec( CecOpcode * opcode, int * length, Nat8 * buf );

/**  @ingroup papi_papionly_wakeup_Functions */
/**  Indicates the LKB command upon which the set has woken up. Only contains relevant data in case <i>GetActualWakeupReason</i> contained <i>WakeupReasonLkb</i> */ 
/**  @param cmd [out] parameter containing the LKB command */
extern void papi_papionly_wakeup_GetWakeupDetailsLkb( int * cmd );

/**  @ingroup papi_papionly_wakeup_Functions */
/**  Indicates the RC key upon which the set has woken up. Only contains relevant data in case <i>GetActualWakeupReason</i> contained <i>WakeupReasonRc</i> */ 
/**  @param src [out] parameter containing the RC source type, see IPapiKeyNotify */
/**  @param sys [out] parameter containing the RC system */
/**  @param cmd [out] parameter containing the RC command */
extern void papi_papionly_wakeup_GetWakeupDetailsRc( int * src, int * sys, int * cmd );

/**  @ingroup papi_papionly_wakeup_Functions */
/**  Returns the reason of wakeup. It can be only one of the WakeupReasons group (so not OR'ed) */ 
/**  @param wakeupreason The reason of the wakeup */
extern void papi_papionly_wakeup_GetActualWakeupReason( int * wakeupreason );



/*
 *  interface gamutntf of type IPapiHdmiGamutNotify
 */

/** @defgroup papi_papionly_gamut papi_papionly_gamut: IPapiHdmiGamut
  * This interface notifies the Gamut Metadata packet of the HDMI input (if present infoframe)
  * @ingroup papi_papionly */

/** @defgroup papi_papionly_gamutntf_Notifications Notifications 
  * @ingroup papi_papionly_gamut */

/**  @ingroup papi_papionly_gamutntf_Notifications */
/**  Called when the Gamut metadata infoframe is received, and HDMI input is active */ 
/**  @param len [in/out] parameter containing initially the maximum length to be written in <i>buf</i>, to be overwritten with the actual number of bytes written in <i>buf</i> */
/**  @param buf [out] array to which a maximum of <i>len</i> bytes will be written */
extern void papi_papionly_gamutntf_OnGamutDataChanged( int * len, Nat8 * buf );



/*
 *  interface keyntf of type IPapiKeyNotify
 */

/** @defgroup papi_papionly_key papi_papionly_key: IPapiKey
  * This interface notifies remote control events
  * @ingroup papi_papionly */

/** @defgroup papi_papionly_key_Datatypes Datatypes 
  * @ingroup papi_papionly_key */
/**
 *   @defgroup papi_papionly_key_SourceRc Type of RC
 *   @ingroup papi_papionly_key_Datatypes
 *   The type of remote control
 *   @{ */
#define papi_papionly_keyntf_SourceRc5 0
#define papi_papionly_keyntf_SourceRc6 1
#define papi_papionly_keyntf_SourceRcUnknown -1
/*   @} */

/** @defgroup papi_papionly_keyntf_Notifications Notifications 
  * @ingroup papi_papionly_key */

/**  @ingroup papi_papionly_keyntf_Notifications */
/**  Called when the key on the remote is pressed or released */ 
/**  @param source the source of the received remote control data (e.g. RC5, RC6). See datatype \ref papi_papionly_key_SourceRc. */
/**  @param toggle the toggle bit field of the received remote control data, if different from the previous value it indicates the presence of new information. See datatype \ref papi_papionly_key_SourceRc. */
/**  @param system the system field of the received remote control data */
/**  @param command the command field of the received remote control data */
/**  @param time the system time (in msecs) at end of the remote control data reception */
extern void papi_papionly_keyntf_OnKey( int source, Bool toggle, int system, int command, int time );



/*
 *  interface lkbntf of type IPapiLocalKeyboardNotify
 */

/** @defgroup papi_papionly_lkb papi_papionly_lkb: IPapiLocalKeyboard
  * This interface notifies activities on the local keyboard
  * @ingroup papi_papionly */

/** @defgroup papi_papionly_lkbntf_Notifications Notifications 
  * @ingroup papi_papionly_lkb */

/**  @ingroup papi_papionly_lkbntf_Notifications */
/**  Notifies that a button on the local keyboard has been pressed */ 
/**  @param command A number corresponding to a voltage level; the mapping of voltage range to command id's is board specific and configurede through diversity */
extern Bool papi_papionly_lkbntf_OnDown( int command );

/**  @ingroup papi_papionly_lkbntf_Notifications */
/**  Notifies that a button on the local keyboard has been released */ 
/**  @param command A number corresponding to a voltage level; the mapping of voltage range to command id's is board specific and configurede through diversity */
extern Bool papi_papionly_lkbntf_OnUp( int command );



/*
 *  interface vmixntf of type tmIVmixNtf
 */

/** @defgroup papi_papionly_vmix papi_papionly_vmix: tmIVmix
  * @ingroup papi_papionly */

/** @defgroup papi_papionly_vmixntf_Notifications Notifications 
  * @ingroup papi_papionly_vmix */

/**  @ingroup papi_papionly_vmixntf_Notifications */
/**  This notification function is called whenever the input properties of a video layer or the output properties of the video mixer changes. */ 
/**  @param cookie Cookie that will be returned for every notification. */
/**  @param outputResolutionChanged Indicates whether the output resolution has changed. */
/**  @param outputWidth The width of the output. */
/**  @param outputHeight The height of the output. */
/**  @param fieldRateChanged Indicates whether the field rate has changed. */
/**  @param fieldRate The field rate of the output See datatype \ref papi_papionly_vmix_FieldRate. */
/**  @param scanTypeChanged Indicates whether the scan type has changed. */
/**  @param scanType The scan type of output. See datatype \ref papi_papionly_vmix_ScanType. */
/**  @param nrVidLayers The number of video layers supported by the video mixer. */
/**  @param pVidLayerProps An array containing the properties per video layer indexed by the video layer number. */
extern void papi_papionly_vmixntf_OnNewProperties( Nat32 cookie, Bool outputResolutionChanged, Nat32 outputWidth, Nat32 outputHeight, Bool fieldRateChanged, int fieldRate, Bool scanTypeChanged, int scanType, Nat32 nrVidLayers, VMixNewVidLayerProp * pVidLayerProps );



/*
 *  interface vmixvidntf of type tmIVmixVidLayerNtf
 */

/** @defgroup papi_papionly_vmixvid papi_papionly_vmixvid: tmIVmixVidLayer
  * @ingroup papi_papionly */

/** @defgroup papi_papionly_vmixvidntf_Notifications Notifications 
  * @ingroup papi_papionly_vmixvid */

/**  @ingroup papi_papionly_vmixvidntf_Notifications */
/**  This notification function is called when the smooth transition to the new window settings is completed. */ 
/**  @param layerNr Video layer number. */
/**  @param cookie Cookie that will be returned for every notification. */
extern void papi_papionly_vmixvidntf_OnNewSettingsCompleted( Nat32 layerNr, Nat32 cookie );

/**  @ingroup papi_papionly_vmixvidntf_Notifications */
/**  This notification function is called when the smooth transition to the new window settings is interrupted. */ 
/**  @param layerNr Video layer number. */
/**  @param cookie Cookie that will be returned for every notification. */
extern void papi_papionly_vmixvidntf_OnNewSettingsInterrupted( Nat32 layerNr, Nat32 cookie );

#endif

