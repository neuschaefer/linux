/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_40/AdpLocalLib/DrmAdpKeyStoreLocal.h#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

#ifndef DRMADPKEYSTORELOCAL_H_INCLUDE
#define DRMADPKEYSTORELOCAL_H_INCLUDE

#ifndef __KERNEL__

#include "divx_drm_adp_common.h"
#include "AdpLib/DrmAdpTypes.h"

#else

#include "../../divx_drm_adp_common.h"
#include "../AdpLib/DrmAdpTypes.h"

#endif

/* Sizes. */
#define ADP_HARDWARE_SECRET_BYTES	32
#define ADP_HARDWARE_KEY_BYTES      16
#define ADP_HARDWARE_KEY_BITS       128

/* Returns. */
#define ADP_NO_LOCAL_HARDWARE_KEY    60
#define ADP_NO_LOCAL_PUBLIC_KEY      61

#ifdef __cplusplus
extern "C" {
#endif

void localGetHardwareSecret(uint8_t *hardwareKey);
#ifdef SUPPORT_SIGNED_ACTIVATION
uint8_t localGetPublicKeyFromId(const uint32_t id, char nHex[ADP_RSA_HEX_KEY_SIZE_BYTES], char eHex[ADP_RSA_HEX_E_EXP_SIZE_BYTES]);
#endif

#ifdef __cplusplus
};
#endif

#endif
/* DRMADPKEYSTORELOCAL_H_INCLUDE */
