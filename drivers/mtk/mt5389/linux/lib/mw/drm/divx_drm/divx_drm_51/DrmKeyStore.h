/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/DrmKeyStore.h#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

#ifndef __KEYSTORE_H
#define __KEYSTORE_H

#ifndef __KERNEL__

#ifdef _MTK_BUILD_
#include "divx_drm_adp_common.h"
#else
#include "DivXInt.h"
#endif

#else

#include "../divx_drm_adp_common.h"

#endif

// Sizes.
#define SIZEOF_BASE_KEY_SIZE_BYTES			32
#define SIZEOF_BASE_KEY_SIZE_BITS			256
#define SIZEOF_BASE_KEY_ID_SIZE_BYTES		44
#define SIZEOF_MESSAGE_KEY_SIZE_BYTES		20
#define HEX_OCTET_PER_BYTE               2

#define KEY_STORE_ERROR_GENERAL               -1
#define KEY_STORE_ERROR_ALREADY_ACTIVATED     -2

#ifdef __cplusplus
extern "C" {
#endif

int32_t keyStoreGetMessageKey( uint8_t protectionFormatFlag,
                               uint8_t *messageKey );

int32_t keyStoreGetProtectionFormat( uint8_t *protectionFormatFlag,
                                     uint8_t* message );

#ifdef __cplusplus
};
#endif

#endif
