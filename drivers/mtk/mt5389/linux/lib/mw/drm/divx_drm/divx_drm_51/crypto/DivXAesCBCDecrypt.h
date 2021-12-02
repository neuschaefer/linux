/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/crypto/DivXAesCBCDecrypt.h#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

#ifndef _DIVXAESCBCDECRYPT_H
#define _DIVXAESCBCDECRYPT_H

/*------------------------------------------------------------------------------------------
 *
 *  LOCAL INCLUDES
 *
 *-----------------------------------------------------------------------------------------*/
#include "DivXPortableAPI.h"
#include "DivXAesDecrypt.h"

/*------------------------------------------------------------------------------------------
 *
 *  TYPE DECLARATIONS
 *
 *-----------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------
 *
 *  FUNCTION DECLARATIONS
 *
 *-----------------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

int32_t DivXAesCBCDecrypt ( aesKeyHandle keyHandle,
                            uint8_t* inBuffer,
                            uint32_t inBufferLengthInBytes,
                            uint8_t* outBuffer,
                            uint32_t* outBufferLengthInBytes );

#ifdef __cplusplus
};
#endif

#endif

