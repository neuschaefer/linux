/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/crypto/DivXRng.h#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/
#ifndef _DIVXRNG_H
#define _DIVXRNG_H

/*------------------------------------------------------------------------------------------
 *
 *  LOCAL INCLUDES
 *
 *-----------------------------------------------------------------------------------------*/
#include "DivXPortableAPI.h"

#ifdef __cplusplus
extern "C" {
#endif

int32_t DivXRngGetRandomBytes ( uint8_t *buffer,
                                uint32_t bufferLength );
#ifdef __cplusplus
};
#endif

#endif
