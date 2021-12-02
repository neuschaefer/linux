/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/crypto/DivXRsa.h#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/
#ifndef _DIVXRSA_H
#define _DIVXRSA_H

#include "DivXPortableAPI.h"

/*------------------------------------------------------------------------------------------
 *
 *  TYPE DECLARATIONS
 *
 *-----------------------------------------------------------------------------------------*/
typedef uint32_t rsaPublicKeyHandle;
typedef uint32_t rsaPrivateKeyHandle;

#ifdef __cplusplus
extern "C" {
#endif

int32_t DivXRsaCreatePublicKey(rsaPublicKeyHandle * keyHandle,
			       const uint8_t * n,
			       int32_t nLength,
			       const uint8_t * e, int32_t eLength);
                               
int32_t DivXRsaDeletePublicKey(rsaPublicKeyHandle keyHandle);
                               
int32_t DivXRsaCreatePrivateKey( rsaPrivateKeyHandle* keyHandle,
                                 const uint8_t* n,
                                 int32_t nLength,
                                 const uint8_t* e,
                                 int32_t eLength,
                                 const uint8_t* d,
                                 int32_t dLength );

int32_t DivXRsaDeletePrivateKey( rsaPrivateKeyHandle keyHandle );

#ifdef __cplusplus
}
#endif

#endif 
//_DIVXRSA_H

