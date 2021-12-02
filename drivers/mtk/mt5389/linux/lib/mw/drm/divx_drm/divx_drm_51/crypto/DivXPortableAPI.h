/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/crypto/DivXPortableAPI.h#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/
#ifndef _DIVXPORTABLEAPI_H
#define _DIVXPORTABLEAPI_H

/*------------------------------------------------------------------------------------------
 *
 *  LOCAL INCLUDES
 *
 *-----------------------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "DivXPortableCryptoAPI.h"
#ifdef _MTK_BUILD_
#include "divx_drm_adp_common.h"
#else
#include "DivXInt.h"
#include "DivXNULL.h"
#endif

#else

#include "DivXPortableCryptoAPI.h"

#include "../../divx_drm_adp_common.h"

#endif


/*------------------------------------------------------------------------------------------
 *
 *  TYPE DECLARATIONS
 *
 *-----------------------------------------------------------------------------------------*/
typedef int32_t (*portableFPtr) ( uint32_t *handle );
struct moduleDescriptor_t
{
    portableFPtr initializeFPtr;
    portableFPtr finalizeFPtr;
    uint32_t handle;
};
typedef struct moduleDescriptor_t moduleDescriptor;


/*------------------------------------------------------------------------------------------
 *
 *  MACROS
 *
 *-----------------------------------------------------------------------------------------*/

#define DIVX_MAX_MODULES 10

/*
 *  USER MACROS:  Macros users should use to utilize this API.
 */
#define DIVX_PORTABLE_ADD_MODULE(name) { DivXPortable##name##Initialize, DivXPortable##name##Finalize, 0 },
#define DIVX_PORTABLE_MODULES_BEGIN moduleDescriptor PORTABLE_MODULES_ARRAY_NAME[DIVX_MAX_MODULES] = {
#define DIVX_PORTABLE_MODULES_END { NULL, NULL, 0 } };

/*
 *  UTILITY MACROS:  Macros users can use to help navigate the modules.
 */
#define PORTABLE_MODULES_ARRAY_NAME g_divxPortableModules
#define NUMBER_OF_PORTABLE_MODULES (sizeof(PORTABLE_MODULES_ARRAY_NAME)/sizeof(moduleDescriptor))

#define DECLARE_DIVX_PORTABLE_MODULE extern moduleDescriptor PORTABLE_MODULES_ARRAY_NAME[DIVX_MAX_MODULES];


/*------------------------------------------------------------------------------------------
 *
 *  FUNCTION DECLARATIONS
 *
 *-----------------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

int32_t DivXPortableInitialize ( void );
int32_t DivXPortableFinalize ( void );

#ifdef __cplusplus
};
#endif

#endif

