/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/DrmConfig.h#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

#ifndef DRMCONFIG_H_INCLUDE
#define DRMCONFIG_H_INCLUDE

#ifndef __KERNEL__

#ifdef _MTK_BUILD_
#include "divx_drm_adp_common.h"
#else
#include "DivXInt.h"
#include "DivXBool.h"
#endif

#else

#include "../divx_drm_adp_common.h"

#endif

/*************************************************************************
 *  CONFIGURATION FLAGS
 *
 *  For most builds, DivX recommends defining DRM_USE_COMPILER_PREPROCESSOR
 *  so your preprocessor definitions are set via command line or project settings
 *
 *  The following flags should be set during production builds:
 *   1)  DRM_GO_LIVE = 1
 *   2)  DRM_XXX_PLATFORM  (one of the platforms must be chosen)
 *   3)  DRM_DATALOAD_USE_LHS  ( DivX Partner Support will instruct you on this flag if
 *       it is necessary, by default it should remain 0 )
 *
 *   Do not manually set DRM_GO_LIVE_BASEKEYS.  This will be toggled based on the GO_LIVE
 *   flag.
 *
 *   Depending on the DRM_XXX_PLATFORM variant, the set of local functions in DrmLocal.c/h
 *   are required to be set will change.  Please refer to the DivX DRM documentation for the
 *   description of the local functions to port.
 */
#ifndef DRM_USE_COMPILER_PREPROCESSOR

/*********************************************
 * FLAG:         DRM_GO_LIVE 
 * DESCRIPTION:  This flag MUST be enabled before shipping firmware.
 *
 * DEFAULT:      OFF
 ********************************************/
#define DRM_GO_LIVE            (1)
#if DRM_GO_LIVE == 0
#ifndef DRM_HARDWARE_SIMULATED
#define DRM_HARDWARE_SIMULATED
#endif
#endif

/*********************************************
 * FLAG:         DRM_DATALOAD_USE_LHS 
 * DESCRIPTION:  This flag will replace the CTK with the LHS.
 *               LHS is preferred over the CTK.  The DataLoad.c
 *               must match one or the other.
 *
 * DEFAULT:      OFF
 ********************************************/
#define DRM_DATALOAD_USE_LHS  (1)


/*********************************************
 * FLAG:         DRM_BASE_ENCRYPTION_ONLY  
 * DESCRIPTION:  This flag will replace set the DRM
 *               API to only use base encryption.
 *
 *               Activation/purchase/rental files will
 *               not work.
 *
 * DEFAULT:      OFF
 ********************************************/
#define DRM_BASE_ENCRYPTION_ONLY  (0)

/*********************************************
 * FLAG:         DRM_GO_LIVE_BASEKEYS 
 * DESCRIPTION:  This flag MUST be enabled before shipping firmware.  If DRM_GO_LIVE is set
 *               this flag will automatically set this flag.
 *
 * DEFAULT:      OFF, AUTO TRIGGER
 ********************************************/
#if !defined DRM_GO_LIVE_BASEKEYS
#if DRM_GO_LIVE == 1
#define DRM_GO_LIVE_BASEKEYS   (1)
#else
#define DRM_GO_LIVE_BASEKEYS   (0) 
#endif
#endif

/*********************************************
 * FLAG:         DRM_XXX_PLATFORM
 * DESCRIPTION:  This flag will specify the current platform in use
 *
 * PREDICATES:   DRM_GO_LIVE must be enabled for this to be honored.
 * DEFAULT:      OFF
 ********************************************/
#if DRM_GO_LIVE == 1

/*********************************************
 * The following flags control the localization required for your target
 * platform.
 *
 * DRM_OTHER_SECURE_PLATFORM can only be used if the product has been designated
 * as secure.  Secure is defined as a product which has the following attributes:
 *     1) Location of DRM memory is difficult to access by the user
 *     2) Location of the DRM memory require priviledges beyond that of a user
 *     3) Location of the DRM memory requires the user to use a third-party tool
 *        to locate, modify, and/or alter the DRM memory.
 *  
 * For example, if the product is a CDMA phone running Windows Mobile 2005,
 * the flags would be set as:
 *    #define DRM_MOBILE_WINDOWS_PLATFORM   (1)
 *    #define DRM_CDMA_MOBILE_TYPE          (1)
 *
 * DEFINE ONE OF THE FOLLOWING:
 *     1) DRM_MOBILE_WINDOWS_PLATFORM
 *     2) DRM_MOBILE_SYMBIAN_PLATFORM
 *     3) DRM_DESKTOP_WINDOWS_PLATFORM
 *     4) DRM_DESKTOP_LINUX_PLATFORM
 *     5) DRM_OTHER_SECURE_PLATFORM
 *
 ********************************************/
#define DRM_MOBILE_WINDOWS_PLATFORM   (0)
#define DRM_MOBILE_SYMBIAN_PLATFORM   (0)
#define DRM_MOBILE_LINUX_PLATFORM   (0)
#define DRM_DESKTOP_WINDOWS_PLATFORM  (0)
#define DRM_DESKTOP_LINUX_PLATFORM    (0)
#define DRM_OTHER_SECURE_PLATFORM     (1)

#endif

#endif

#if DRM_GO_LIVE == 1

/* Configuration error checking is done here */

#if ( DRM_MOBILE_WINDOWS_PLATFORM + DRM_MOBILE_SYMBIAN_PLATFORM + DRM_DESKTOP_WINDOWS_PLATFORM + DRM_DESKTOP_LINUX_PLATFORM + DRM_OTHER_SECURE_PLATFORM) > 1
#error "You cannot specify more than one type of platform"
#endif

#if ( DRM_MOBILE_WINDOWS_PLATFORM + DRM_MOBILE_SYMBIAN_PLATFORM + DRM_DESKTOP_WINDOWS_PLATFORM + DRM_DESKTOP_LINUX_PLATFORM + DRM_OTHER_SECURE_PLATFORM) == 0
#error "You must specify one type of platform"
#endif

#endif

#endif
/* DRMCONFIG_H_INCLUDE */
