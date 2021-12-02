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
 *   @file papi_setup.h
 */
/** @defgroup papi_setup  */

/*
 *  interface lgc of type IPlfApiEnable
 */

/** @defgroup papi_setup_lgc papi_setup_lgc: Enable
  * @ingroup papi_setup */

/** @defgroup papi_setup_lgc_Functions Functions 
  * @ingroup papi_setup_lgc */

/**  @ingroup papi_setup_lgc_Functions */
extern FResult papi_setup_lgc_Disable( void );

/**  @ingroup papi_setup_lgc_Functions */
extern FResult papi_setup_lgc_Enable( void );

/**  @ingroup papi_setup_lgc_Functions */
extern FResult papi_setup_lgc_IsEnabled( Bool * retval );



/*
 *  interface pow of type IPlfApiPower
 */

/** @defgroup papi_setup_pow papi_setup_pow: Power
  * @ingroup papi_setup */

/** @defgroup papi_setup_pow_Datatypes Datatypes 
  * @ingroup papi_setup_pow */
/**
 *   @defgroup papi_setup_pow_Application TV Application Modes
 *   @ingroup papi_setup_pow_Datatypes
 *   These are the TV Application Modes.
 *   @{ */
 /**  Parameter value for the Restart function indicating that the set should restart loading the normal TV application. */
#define papi_setup_pow_ApplicationTvCold 0
 /**  Parameter value for the Restart function indicating that the set should restart loading the end user SW download application. */
#define papi_setup_pow_ApplicationSwDownload 1
/*   @} */

/**
 *   @defgroup papi_setup_pow_Power TV Power Modes
 *   @ingroup papi_setup_pow_Datatypes
 *   These are the TV Power Modes.
 *   @{ */
 /**  The lowest possible power state. */
#define papi_setup_pow_PowerMin 0
 /**  The TV is in a state not represented by the other values. */
#define papi_setup_pow_PowerUnknown -1
 /**  The audio and video power is switched off. */
#define papi_setup_pow_PowerOff 0
#define papi_setup_pow_PowerStandby 0
 /**  The TV enables remote control of the Conditional Access Device. */
#define papi_setup_pow_PowerCadStandby 1
#define papi_setup_pow_PowerSemiPowerOff 1
 /**  The TV can process signals, but has no deflection yet. */
#define papi_setup_pow_PowerSemiStandby 2
 /**  The TV is fully operational. */
#define papi_setup_pow_PowerOn 3
 /**  The TV is capable of capturing VBI data. */
#define papi_setup_pow_PowerDataAcquisition 4
 /**  The maximum power state. */
#define papi_setup_pow_PowerMax 4
/*   @} */

/** @defgroup papi_setup_pow_Functions Functions 
  * @ingroup papi_setup_pow */

/**  @ingroup papi_setup_pow_Functions */
/**  Restart the set loading the given application. Used eg. by the download application to go back to the main TV application. */ 
/**  @param appl The application to restart into. See datatype \ref papi_setup_pow_Application. */
extern FResult papi_setup_pow_Restart( int appl );

/**  @ingroup papi_setup_pow_Functions */
/**  This function brings the TV platform into the specified power state. This may take a certain time, and the platform may go through a number of intermediate states before the required state is reached. <br><br>
pApi library only needs to support PowerOff and PowerOn power modes. If PowerOn is set, the hardware of all platform drivers is programmed and brought up.
 */ 
/**  @param mode The desired power mode. See datatype \ref papi_setup_pow_Power. */
extern FResult papi_setup_pow_SetTvPower( int mode );



/*
 *  interface powN of type IPlfApiPowerNotify
 */

/** @defgroup papi_setup_pow papi_setup_pow: Power
  * @ingroup papi_setup */

/** @defgroup papi_setup_powN_Notifications Notifications 
  * @ingroup papi_setup_pow */

/**  @ingroup papi_setup_powN_Notifications */
/**  This function is called if the power state requested by the last call of SetTvPower has been reached. */ 
extern void papi_setup_powN_OnTvPowerChanged( void );

/**  @ingroup papi_setup_powN_Notifications */
/**  This function is called when the power state specified with the function SetTvPower cannot be reached. */ 
extern void papi_setup_powN_OnTvPowerNotChanged( void );


