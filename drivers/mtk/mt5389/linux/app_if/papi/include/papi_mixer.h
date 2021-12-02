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
/* (c) 2007 Koninklijke Philips Electronics N.V., All rights reserved
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
 *   @file papi_mixer.h
 *   @brief Mixer
 */
/** @defgroup mixer Mixer */
/** @defgroup mixer_datatypes Datatypes
  * @ingroup mixer */
/** @defgroup mixer_functions Functions
  * @ingroup mixer */
/** @defgroup mixer_notifications Notifications
  * @ingroup mixer */

/*
 *  interface vmx of type tmIVmix
 */

/**
 *   @defgroup tmIVmix_FieldRate Mixer Output Field Rate
 *   @ingroup mixer_datatypes
 *   The field rate of the output of the mixer.
 *   @{ */
#define papi_mixer_vmx_FieldRateInvalid -2
#define papi_mixer_vmx_FieldRateUnknown -1
#define papi_mixer_vmx_FieldRate50Hz 0
#define papi_mixer_vmx_FieldRate60Hz 1
#define papi_mixer_vmx_FieldRate75Hz 2
#define papi_mixer_vmx_FieldRate100Hz 3
/*   @} */

/**
 *   @defgroup tmIVmix_ScanType Scan Type
 *   @ingroup mixer_datatypes
 *   The scan type of the signal: interlaced or progressive.
 *   @{ */
#define papi_mixer_vmx_ScanTypeInvalid -2
#define papi_mixer_vmx_ScanTypeUnknown -1
#define papi_mixer_vmx_ScanTypeInterlaced 0
#define papi_mixer_vmx_ScanTypeProgressive 1
/*   @} */

/**  @ingroup mixer_functions */
/**  @brief Get the color of the background layer */
/**  @param pBgColor Return value with the background color. */
extern void papi_mixer_vmx_GetBgColor( VMixColor * pBgColor );

/**  @ingroup mixer_functions */
/**  @brief Blanks or unblanks the video mixer output */
/**  @param blank True to blank the output, false to unblank the output. */
/**  @param blankColor If the output is blanked, the blanking color that needs to be shown. */
extern void papi_mixer_vmx_BlankOutput( Bool blank, VMixColor blankColor );

/**  @ingroup mixer_functions */
/**  @brief Returns the auto blanking settings in the platform. */
/**  @param pBlankOnOutputResolutionChangedEnabled True, if the output is blanked if the output resolution changes. */
/**  @param pBlankOnFieldRateChangedEnabled True, if the output is blanked if the field rate changes. */
/**  @param pBlankOnScanTypeChangedEnabled True, if the output is blanked if the scan type changes. */
/**  @param pBlankColor If the output is auto blanked, the blanking color that is shown. */
extern void papi_mixer_vmx_GetAutoBlankEnabled( Bool * pBlankOnOutputResolutionChangedEnabled, Bool * pBlankOnFieldRateChangedEnabled, Bool * pBlankOnScanTypeChangedEnabled, VMixColor * pBlankColor );

/**  @ingroup mixer_functions */
/**  @brief Returns the output properties of the video mixer. */
/**  @param pWidth Output parameter with the number of pixels per line. */
/**  @param pHeight Output parameter with the number of lines. */
/**  @param pFieldRate Output parameter with the field rate. See datatype \ref tmIVmix_FieldRate. */
/**  @param pScanType Output parameter with the scan type. See datatype \ref tmIVmix_ScanType. */
extern void papi_mixer_vmx_GetOutputProperties( Nat32 * pWidth, Nat32 * pHeight, int * pFieldRate, int * pScanType );

/**  @ingroup mixer_functions */
/**  @brief Set the color of the background layer */
/**  @param bgColor The background color that has to be set. */
extern void papi_mixer_vmx_SetBgColor( VMixColor bgColor );

/**  @ingroup mixer_functions */
/**  @brief Returns the number of video layers supported by the video mixer. */
/**  @param pNrLayers Output parameter with the number of video layers supported by the video mixer. */
extern void papi_mixer_vmx_GetSuppNrLayers( Nat32 * pNrLayers );

/**  @ingroup mixer_functions */
/**  @brief Enable the auto blanking behaviour in the platform. */
/**  @param enableOnOutputResolutionChanged True, to blank the output if the output resolution changes. */
/**  @param enableOnFieldRateChanged True, to blank the output if the field rate changes. */
/**  @param enableOnScanTypeChanged True, to blank the output if the scan type changes. */
/**  @param blankColor If the output is auto blanked, the blanking color that needs to be shown. */
extern void papi_mixer_vmx_EnableAutoBlank( Bool enableOnOutputResolutionChanged, Bool enableOnFieldRateChanged, Bool enableOnScanTypeChanged, VMixColor blankColor );

/**  @ingroup mixer_functions */
/**  @brief Returns whether the output is blanked or not. */
/**  @param pBlanked True if the output is blanked, false if not. */
/**  @param pBlankColor If the output is blanked, the blanking color that is shown. */
extern void papi_mixer_vmx_GetOutputBlanked( Bool * pBlanked, VMixColor * pBlankColor );

/**  @ingroup mixer_functions */
/**  @brief Deactivate auto blank. */
extern void papi_mixer_vmx_AutoBlankUnblank( void );



/*
 *  interface vmxvl of type tmIVmixVidLayer
 */

/**
 *   @defgroup tmIVmixVidLayer_ScaleMode Scale Mode
 *   @ingroup mixer_datatypes
 *   
 *   @{ */
#define papi_mixer_vmxvl_ScaleModeNoScaling 1
#define papi_mixer_vmxvl_ScaleModeLinear 2
#define papi_mixer_vmxvl_ScaleModePanorama 4
#define papi_mixer_vmxvl_ScaleModeHorizontal 8
/*   @} */

/**  @ingroup mixer_functions */
/**  @brief Hide or show this layer. */
/**  @param layerNr Video layer number. */
/**  @param hide True to hide this layer, false to show this layer. */
extern void papi_mixer_vmxvl_Hide( Nat32 layerNr, Bool hide );

/**  @ingroup mixer_functions */
/**  @brief Set the background color for this layer. */
/**  @param layerNr Video layer number. */
/**  @param bgColor The background color that has to be set. */
extern void papi_mixer_vmxvl_SetBgColor( Nat32 layerNr, VMixColor bgColor );

/**  @ingroup mixer_functions */
/**  @brief Sets the coordinates of the destination window on this layer. The video mixer commits these new settings after an ActiveNewWindowSettings(). */
/**  @param layerNr Video layer number. */
/**  @param dstRect Coordinates of the destination window relative to the layer window. */
extern void papi_mixer_vmxvl_SetDstWindow( Nat32 layerNr, VMixRect dstRect );

/**  @ingroup mixer_functions */
/**  @brief Returns the scale modes supported by this layer. */
/**  @param layerNr Video layer number. */
/**  @param pScaleModes Return value with the OR-ed supported scale modes. See datatype \ref tmIVmixVidLayer_ScaleMode. */
extern void papi_mixer_vmxvl_GetSuppScaleModes( Nat32 layerNr, int * pScaleModes );

/**  @ingroup mixer_functions */
/**  @brief Return the aspect ratio of the input of this layer in the format numerator:denominator. If there is no information about the aspect ratio then numerator=0 and denominator=1. */
/**  @param layerNr Video layer number. */
/**  @param pNumerator Output parameter with the aspect ratio of the input, numerator part. */
/**  @param pDenominator Output parameter with the aspect ratio of the input, denominator part. */
extern void papi_mixer_vmxvl_GetInputAspectRatio( Nat32 layerNr, Nat16 * pNumerator, Nat16 * pDenominator );

/**  @ingroup mixer_functions */
/**  @brief Return the minimum and maximum value for the mid scale ratio for panoramic scaling. */
/**  @param layerNr Video layer number. */
/**  @param pMinMidScaleRatio Minimum scale ratio in percentages. */
/**  @param pMaxMidScaleRatio Maximum scale ratio in percentages. */
extern void papi_mixer_vmxvl_GetPanoramaMidScaleRange( Nat32 layerNr, Nat32 * pMinMidScaleRatio, Nat32 * pMaxMidScaleRatio );

/**  @ingroup mixer_functions */
/**  @brief Return the coordinates of the destination window on this layer. */
/**  @param layerNr Video layer number. */
/**  @param pDstRect Return value with the coordinates of the destination window. */
extern void papi_mixer_vmxvl_GetDstWindow( Nat32 layerNr, VMixRect * pDstRect );

/**  @ingroup mixer_functions */
/**  @brief Returns whether this layer is hidden or shown. */
/**  @param layerNr Video layer number. */
/**  @param pHidden Return value is true if this layer is hidden, false if this layer is shown. */
extern void papi_mixer_vmxvl_GetHidden( Nat32 layerNr, Bool * pHidden );

/**  @ingroup mixer_functions */
/**  @brief Returns the maximum window width and height for this layer. */
/**  @param layerNr Video layer number. */
/**  @param pMaxWidth Returns the maximum window width. */
/**  @param pMaxHeight Returns the maximum window height. */
extern void papi_mixer_vmxvl_GetMaxLayerWindowSize( Nat32 layerNr, Nat32 * pMaxWidth, Nat32 * pMaxHeight );

/**  @ingroup mixer_functions */
/**  @brief Return the coordinates of the source window on this layer. */
/**  @param layerNr Video layer number. */
/**  @param pSrcRect Return value with the coordinates of the source window. */
extern void papi_mixer_vmxvl_GetSrcWindow( Nat32 layerNr, VMixRect * pSrcRect );

/**  @ingroup mixer_functions */
/**  @brief Return the current scale mode. */
/**  @param layerNr Video layer number. */
/**  @param pScaleMode Return value with the current scale mode. See datatype \ref tmIVmixVidLayer_ScaleMode. */
extern void papi_mixer_vmxvl_GetScaleMode( Nat32 layerNr, int * pScaleMode );

/**  @ingroup mixer_functions */
/**  @brief Return the active image area of the input of this layer (excluding black bars). If there is no information about the active image area, then (0,0,0,0) is returned. */
/**  @param layerNr Video layer number. */
/**  @param pActiveImageArea Output parameter with the active image area. */
extern void papi_mixer_vmxvl_GetInputActiveImageArea( Nat32 layerNr, VMixRect * pActiveImageArea );

/**  @ingroup mixer_functions */
/**  @brief Returns the minimum window width and height for this layer. */
/**  @param layerNr Video layer number. */
/**  @param pMinWidth Returns the minimum window width. */
/**  @param pMinHeight Returns the minimum window height. */
extern void papi_mixer_vmxvl_GetMinWindowSize( Nat32 layerNr, Nat32 * pMinWidth, Nat32 * pMinHeight );

/**  @ingroup mixer_functions */
/**  @brief Sets the coordinates of the source window on this layer. The video mixer commits these new settings after an ActiveNewWindowSettings(). Overscan will be controlled by the middleware and application layer by setting the src window. The platform should not clip any overscan itself without src window being set. */
/**  @param layerNr Video layer number. */
/**  @param srcRect Coordinates of the source window relative to the input window. */
extern void papi_mixer_vmxvl_SetSrcWindow( Nat32 layerNr, VMixRect srcRect );

/**  @ingroup mixer_functions */
/**  @brief Returns whether a background color is supported for this layer or not. */
/**  @param layerNr Video layer number. */
/**  @param pBgSupp True if background color is supported, false otherwise. */
extern void papi_mixer_vmxvl_GetBgSupp( Nat32 layerNr, Bool * pBgSupp );

/**  @ingroup mixer_functions */
/**  @brief Sets the coordinates of the layer window on this layer. The video mixer commits these new settings after an ActiveNewWindowSettings(). */
/**  @param layerNr Video layer number. */
/**  @param layerRect Coordinates of the layer window relative to the output window. */
extern void papi_mixer_vmxvl_SetLayerWindow( Nat32 layerNr, VMixRect layerRect );

/**  @ingroup mixer_functions */
/**  @brief Returns for a particular scaling mode the minimum and maximum scale factors in percentages. */
/**  @param layerNr Video layer number. */
/**  @param scaleMode The specified scaling mode. See datatype \ref tmIVmixVidLayer_ScaleMode. */
/**  @param pMinWidthPercentage The minimal horizontal scale factor. */
/**  @param pMinHeightPercentage The minimal vertical scale factor. */
/**  @param pMaxWidthPercentage The maximum horizontal scale factor. */
/**  @param pMaxHeightPercentage The maximum vertical scale factor. */
extern void papi_mixer_vmxvl_GetScaleRange( Nat32 layerNr, int scaleMode, Nat32 * pMinWidthPercentage, Nat32 * pMinHeightPercentage, Nat32 * pMaxWidthPercentage, Nat32 * pMaxHeightPercentage );

/**  @ingroup mixer_functions */
/**  @brief Return the coordinates of the layer window on this layer. */
/**  @param layerNr Video layer number. */
/**  @param pLayerRect Return value with the coordinates of the layer window. */
extern void papi_mixer_vmxvl_GetLayerWindow( Nat32 layerNr, VMixRect * pLayerRect );

/**  @ingroup mixer_functions */
/**  @brief Set the mid scale ratio for panoramic scaling. */
/**  @param layerNr Video layer number. */
/**  @param midScaleRatio Mid scale ratio for panoramic scaling. */
extern void papi_mixer_vmxvl_SetPanoramaMidScale( Nat32 layerNr, Nat32 midScaleRatio );

/**  @ingroup mixer_functions */
/**  @brief Returns the background color for this layer. */
/**  @param layerNr Video layer number. */
/**  @param pBgColor Return value with the background color for this layer. */
extern void papi_mixer_vmxvl_GetBgColor( Nat32 layerNr, VMixColor * pBgColor );

/**  @ingroup mixer_functions */
/**  @brief Select a particular scale mode */
/**  @param layerNr Video layer number. */
/**  @param scaleMode Scale mode that is set. See datatype \ref tmIVmixVidLayer_ScaleMode. */
extern void papi_mixer_vmxvl_SetScaleMode( Nat32 layerNr, int scaleMode );

/**  @ingroup mixer_functions */
/**  @brief Return the input resolution of this layer. */
/**  @param layerNr Video layer number. */
/**  @param pWidth Output parameter with the number of pixels per line. */
/**  @param pHeight Output parameter with the number of lines. */
extern void papi_mixer_vmxvl_GetInputResolution( Nat32 layerNr, Nat32 * pWidth, Nat32 * pHeight );

/**  @ingroup mixer_functions */
/**  @brief The changes specified by SetLayerWindow(), SetDstWindow(), SetSrcWindow(), SetScaleMode(), and SetPanoramaMidScale() are carried out smoothly in the specified amount of time. This is handled asynchronously and the completion of the operation is signalled via a notification function. */
/**  @param layerNr Video layer number. */
/**  @param syncWithNextInput True to wait for the next field/frame, false to start immediately. */
/**  @param nrMilliSeconds Number of milliseconds that the transition should take. */
extern void papi_mixer_vmxvl_ActivateNewWindowSettings( Nat32 layerNr, Bool syncWithNextInput, Nat32 nrMilliSeconds );

/**  @ingroup mixer_functions */
/**  @brief Return the current mid scale ratio for panoramic scaling. */
/**  @param layerNr Video layer number. */
/**  @param pMidScaleRatio Return value with the current mid scale ratio for panoramic scaling. */
extern void papi_mixer_vmxvl_GetPanoramaMidScale( Nat32 layerNr, Nat32 * pMidScaleRatio );



/*
 *  interface vmxN of type tmIVmixNtf
 */

/**  @ingroup mixer_notifications */
/**  @brief This notification function is called whenever the input properties of a video layer or the output properties of the video mixer changes. */
/**  @param cookie Cookie that will be returned for every notification. */
/**  @param outputResolutionChanged Indicates whether the output resolution has changed. */
/**  @param outputWidth The width of the output. */
/**  @param outputHeight The height of the output. */
/**  @param fieldRateChanged Indicates whether the field rate has changed. */
/**  @param fieldRate The field rate of the output See datatype \ref tmIVmix_FieldRate. */
/**  @param scanTypeChanged Indicates whether the scan type has changed. */
/**  @param scanType The scan type of output. See datatype \ref tmIVmix_ScanType. */
/**  @param nrVidLayers The number of video layers supported by the video mixer. */
/**  @param pVidLayerProps An array containing the properties per video layer indexed by the video layer number. */
extern void papi_mixer_vmxN_OnNewProperties( Nat32 cookie, Bool outputResolutionChanged, Nat32 outputWidth, Nat32 outputHeight, Bool fieldRateChanged, int fieldRate, Bool scanTypeChanged, int scanType, Nat32 nrVidLayers, VMixNewVidLayerProp * pVidLayerProps );



/*
 *  interface vmxvlN of type tmIVmixVidLayerNtf
 */

/**  @ingroup mixer_notifications */
/**  @brief This notification function is called when the smooth transition to the new window settings is completed. */
/**  @param layerNr Video layer number. */
/**  @param cookie Cookie that will be returned for every notification. */
extern void papi_mixer_vmxvlN_OnNewSettingsCompleted( Nat32 layerNr, Nat32 cookie );

/**  @ingroup mixer_notifications */
/**  @brief This notification function is called when the smooth transition to the new window settings is interrupted. */
/**  @param layerNr Video layer number. */
/**  @param cookie Cookie that will be returned for every notification. */
extern void papi_mixer_vmxvlN_OnNewSettingsInterrupted( Nat32 layerNr, Nat32 cookie );


