/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/crypto/DivXRsaEncrypt.h#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/
#ifndef _DIVXRSAENCRYPT_H
#define _DIVXRSAENCRYPT_H

/*------------------------------------------------------------------------------------------
 *
 *  LOCAL INCLUDES
 *
 *-----------------------------------------------------------------------------------------*/
#include "DivXRsa.h"

/*------------------------------------------------------------------------------------------
 *
 *  FUNCTION DECLARATIONS
 *
 *-----------------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif


int32_t DivXRsaPrivateEncrypt( rsaPrivateKeyHandle keyHandle,
                               const uint8_t* plainText,
                               int32_t plainTextLength,
                               uint8_t* cryptText,
                               int32_t* cryptTextLength );

int32_t DivXRsaPublicEncrypt( rsaPublicKeyHandle keyHandle,
                              const uint8_t* plainText,
                              int32_t plainTextLength,
                              uint8_t* cryptText,
                              int32_t* cryptTextLength );

#ifdef __cplusplus
};
#endif

#endif
