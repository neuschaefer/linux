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
 *   @file papi_src.h
 */
/** @defgroup papi_src  */

/*
 *  interface brk of type IPlfApiBreakin
 */

/** @defgroup papi_src_brk papi_src_brk: Breakin
  * @ingroup papi_src */

/** @defgroup papi_src_brk_Functions Functions 
  * @ingroup papi_src_brk */

/**  @ingroup papi_src_brk_Functions */
/**  Verify whether a source is from extension breakin. */ 
/**  @param source Input parameter source. */
/**  @param Status Output parameter whether the source is from extension breakin. */
extern FResult papi_src_brk_ExtensionStatus( tmPlfInstPreset_Source_t source, Bool * Status );



/*
 *  interface conf of type IPlfApiConnectionConfiguration
 */

/** @defgroup papi_src_conf papi_src_conf: ConnectionConfiguration
  * @ingroup papi_src */

/** @defgroup papi_src_conf_Datatypes Datatypes 
  * @ingroup papi_src_conf */
/**
 *   @defgroup papi_src_conf_Medium Medium Mode
 *   @ingroup papi_src_conf_Datatypes
 *   Tuner Input Medium Mode.
 *   @{ */
#define papi_src_conf_MediumCable 1
#define papi_src_conf_MediumTerrestrial 2
/*   @} */

/** @defgroup papi_src_conf_Functions Functions 
  * @ingroup papi_src_conf */

/**  @ingroup papi_src_conf_Functions */
/**  Get Tuner Medium. */ 
/**  @param medium Output parameter medium. */
extern FResult papi_src_conf_GetTunerMedium( int * medium );

/**  @ingroup papi_src_conf_Functions */
/**  Set Tuner Medium. */ 
/**  @param medium Input parameter medium. */
extern FResult papi_src_conf_SetTunerMedium( int medium );



/*
 *  interface dsgid of type IPlfApiDesignatorId
 */

/** @defgroup papi_src_dsgid papi_src_dsgid: DesignatorId
  * For a number of source inputs, the designator is evident from the source selected and does not need to be specified. In some cases however, the designator has to be explicitly specified by the client of the platform, because it is not clear from the source that is selected. For instance, if tuner is selected as a source, the designator indicates whether an analog or digital use case is selected.
  * @ingroup papi_src */

/** @defgroup papi_src_dsgid_Datatypes Datatypes 
  * @ingroup papi_src_dsgid */
/**
 *   @defgroup papi_src_src_Designator Designator ID's
 *   @ingroup papi_src_dsgid_Datatypes
 *   List of designator ID.
 *   @{ */
#define papi_src_dsgid_DesignatorNone 0
#define papi_src_dsgid_DesignatorAll -1
#define papi_src_dsgid_DesignatorAnalog 1
#define papi_src_dsgid_DesignatorDigital 2
#define papi_src_dsgid_DesignatorMpeg2 4
#define papi_src_dsgid_DesignatorMpeg4 8
#define papi_src_dsgid_DesignatorJpg 16
#define papi_src_dsgid_DesignatorMp3 32
#define papi_src_dsgid_DesignatorWm 64
#define papi_src_dsgid_DesignatorDivX 128
/*   @} */



/*
 *  interface src of type IPlfApiSourceSetup
 */

/** @defgroup papi_src_src papi_src_src: SourceSetup
  * @ingroup papi_src */

/** @defgroup papi_src_src_Functions Functions 
  * @ingroup papi_src_src */

/**  @ingroup papi_src_src_Functions */
/**  This functions commits the new audio and video source settings to the platform, in order to let the platform make the transition from the current source settings to the newly defined source settings. */ 
/**  @param retval True, if the transition has completed. False, if the transition is still in progress and completion will be notified via a corresponding notification function <i>OnSourceSettingsApplied</i>. */
extern FResult papi_src_src_ApplySourceSettings( Bool * retval );

/**  @ingroup papi_src_src_Functions */
/**  Get current audio source settings. */ 
/**  @param audionodeid Ignore this parameter. */
/**  @param source Output parameter source. See datatype \ref papi_src_src_Src. */
/**  @param designator Output parameter designator. See datatype \ref papi_src_src_Designator. */
extern FResult papi_src_src_GetCurrentAudioSelection( int audionodeid, int * source, Nat32 * designator );

/**  @ingroup papi_src_src_Functions */
/**  Get current video source settings. */ 
/**  @param source Output parameter source. See datatype \ref papi_src_src_Src. */
/**  @param designator Output parameter designator. See datatype \ref papi_src_src_Designator. */
extern FResult papi_src_src_GetCurrentVideoSelection( int * source, Nat32 * designator );

/**  @ingroup papi_src_src_Functions */
/**  Verify whether it is allowed to select the specified audio source with the specified designator. */ 
/**  @param source Input parameter source. See datatype \ref papi_src_src_Src. */
/**  @param designator Input parameter designator. See datatype \ref papi_src_src_Designator. */
/**  @param retval TRUE if allowed, FALSE if not. */
extern FResult papi_src_src_IsAudioSelectionAllowed( int audionodeid, int source, Nat32 designator, Bool * retval );

/**  @ingroup papi_src_src_Functions */
/**  Verify whether it is allowed to select the specified video source with the specified designator. */ 
/**  @param source Input parameter source. See datatype \ref papi_src_src_Src. */
/**  @param designator Input parameter designator. See datatype \ref papi_src_src_Designator. */
/**  @param retval TRUE if allowed, FALSE if not. */
extern FResult papi_src_src_IsVideoSelectionAllowed( int source, Nat32 designator, Bool * retval );

/**  @ingroup papi_src_src_Functions */
/**  Select the input source for the audio. The platform will not yet make the transition to the newly selected audio source. The platform will make the transition only after all changes have been committed via the ApplySourceSettings() function. */ 
/**  @param source Input parameter source. See datatype \ref papi_src_src_Src. */
/**  @param designator Input parameter designator. See datatype \ref papi_src_src_Designator. */
/**  @param retval TRUE if synchronous, if FALSE wait for notification. */
extern FResult papi_src_src_SelectAudioSource(int audionodeid, int source, Nat32 designator, Bool * retval );

/**  @ingroup papi_src_src_Functions */
/**  Select the input source for the video. The platform will not yet make the transition to the newly selected video source. The platform will make the transition only after all changes have been committed via the ApplySourceSettings() function. */ 
/**  @param source Input parameter source. See datatype \ref papi_src_src_Src. */
/**  @param designator Input parameter designator. See datatype \ref papi_src_src_Designator. */
/**  @param retval TRUE if synchronous, if FALSE wait for notification. */
extern FResult papi_src_src_SelectVideoSource( int source, Nat32 designator, Bool * retval );



/*
 *  interface srcid of type IPlfApiSourceId
 */

/** @defgroup papi_src_srcid papi_src_srcid: SourceId
  * @ingroup papi_src */

/** @defgroup papi_src_srcid_Datatypes Datatypes 
  * @ingroup papi_src_srcid */
/**
 *   @defgroup papi_src_src_Src Source ID's
 *   @ingroup papi_src_srcid_Datatypes
 *   List of source identifiers.
 *   @{ */
#define papi_src_srcid_SrcInvalid -1
#define papi_src_srcid_SrcNone 0
#define papi_src_srcid_SrcTuner 1
 /**  This is the front or side connector on a TV, e.g. to connect a video camera to the TV. */
#define papi_src_srcid_SrcFront 2
#define papi_src_srcid_SrcAv1 3
#define papi_src_srcid_SrcAv2 4
#define papi_src_srcid_SrcAv3 5
#define papi_src_srcid_SrcAv4 6
#define papi_src_srcid_SrcAv5 7
#define papi_src_srcid_SrcAv6 8
#define papi_src_srcid_SrcAv7 9
#define papi_src_srcid_SrcAv8 10
#define papi_src_srcid_SrcAv9 11
#define papi_src_srcid_SrcAv10 12
#define papi_src_srcid_SrcHdmi1 13
#define papi_src_srcid_SrcHdmi2 14
#define papi_src_srcid_SrcHdmi3 15
#define papi_src_srcid_SrcHdmi4 16
#define papi_src_srcid_SrcHdmi5 17
#define papi_src_srcid_SrcUrl 18
#define papi_src_srcid_SrcBolton 19
#define papi_src_srcid_SrcVga 20
/*   @} */



/*
 *  interface brkN of type IPlfApiBreakinNotify
 */

/** @defgroup papi_src_brk papi_src_brk: Breakin
  * @ingroup papi_src */

/** @defgroup papi_src_brkN_Notifications Notifications 
  * @ingroup papi_src_brk */

/**  @ingroup papi_src_brkN_Notifications */
/**  Notification if extension status has changed. */ 
/**  @param source Input parameter source. */
/**  @param status Input parameter setting notification. */
extern void papi_src_brkN_OnExtensionStatusChanged( tmPlfInstPreset_Source_t source, Bool status );



/*
 *  interface srcN of type IPlfApiSourceSetupNotify
 */

/** @defgroup papi_src_src papi_src_src: SourceSetup
  * @ingroup papi_src */

/** @defgroup papi_src_srcN_Notifications Notifications 
  * @ingroup papi_src_src */

/**  @ingroup papi_src_srcN_Notifications */
/**  Notification raised when given audio node is physically selected. */ 
/**  @param source Input parameter video source. See datatype \ref papi_src_src_Src. */
extern void papi_src_srcN_OnAudioSourceSelected( int audionodeid, int source );

/**  @ingroup papi_src_srcN_Notifications */
/**  This notification is raised when the transition to the new source settings has completed. */ 
extern void papi_src_srcN_OnSourceSettingsApplied( void );

/**  @ingroup papi_src_srcN_Notifications */
/**  Notification raised when given video source is physically selected. */ 
/**  @param source Input parameter video source. See datatype \ref papi_src_src_Src. */
extern void papi_src_srcN_OnVideoSourceSelected( int source );


