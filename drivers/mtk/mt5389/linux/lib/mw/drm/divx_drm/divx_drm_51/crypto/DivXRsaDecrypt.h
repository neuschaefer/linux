/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/crypto/DivXRsaDecrypt.h#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/
#ifndef _DIVXRSADECRYPT_H
#define _DIVXRSADECRYPT_H

#include "DivXRsa.h"


/*------------------------------------------------------------------------------------------
 *
 *  FUNCTION DECLARATIONS
 *
 *-----------------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif


int32_t DivXRsaPrivateDecrypt( rsaPrivateKeyHandle keyHandle,
                               const uint8_t* cryptText,
                               int32_t cryptTextLength,
                               uint8_t* plainText,
                               int32_t* plainTextLength );

int32_t DivXRsaPublicDecrypt( rsaPublicKeyHandle keyHandle,
                              const uint8_t* cryptText,
                              int32_t cryptTextLength,
                              uint8_t* plainText,
                              int32_t* plainTextLength );

#ifdef __cplusplus
};
#endif

#endif
