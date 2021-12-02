/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_40/AdpLocalLib/DrmAdpKeyStoreLocal.c#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

#ifndef __KERNEL__

#include "divx_drm_glue.h"
#include "DrmAdpKeyStoreLocal.h"
#include "DrmAdpLocal.h"

#ifdef _MTK_BUILD_
#include "libc/stdio.h"
#include "libc/string.h"
#include "libc/memory.h"
#else
#include <stdio.h>
#include <string.h>
#endif

#else

#include "../../divx_drm_glue.h"
#include "DrmAdpKeyStoreLocal.h"
#include "DrmAdpLocal.h"

#endif

void localGetHardwareSecret(uint8_t *hardwareSecret)
{
    divx_drm_hw_secret_key_glue((UINT8 *)hardwareSecret);
}

#ifdef SUPPORT_SIGNED_ACTIVATION
uint8_t localGetPublicKeyFromId(const uint32_t id,
								char nHex[ADP_RSA_HEX_KEY_SIZE_BYTES],
								char eHex[ADP_RSA_HEX_E_EXP_SIZE_BYTES])
{
	uint8_t result = ADP_NO_LOCAL_PUBLIC_KEY;

#include "DrmAdpLocalKeyStoreData.h"  /* If includeId is '1' it is a test public key. Production products must not contain this key. */

	if (includeId == id) 
	{
		strncpy(nHex, nT0001, ADP_RSA_HEX_KEY_SIZE_BYTES);   /* Variable name nT0001 = test key only. Replace with issued key for production. */
		strncpy(eHex, eT0001, ADP_RSA_HEX_E_EXP_SIZE_BYTES); /* Variable name eT0001 = test key only. Replace with issued key for production. */
		result = ADP_LOCAL_SUCCESS;
	}

	return result;
}
#endif

