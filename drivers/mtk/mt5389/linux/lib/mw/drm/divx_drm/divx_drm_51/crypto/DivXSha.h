/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/crypto/DivXSha.h#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/
#ifndef _DIVXSHA_H
#define _DIVXSHA_H

#include "DivXPortableAPI.h"

#define SHA256_BYTES 32

#ifdef __cplusplus
extern "C" {
#endif

typedef void* sha256Handle_t;

/* This function will do a single, non-accumulating pass */
int32_t DivXSha256( uint8_t *inBuffer,
                    uint32_t inBufferLength,
                    uint8_t *outBuffer,
                    uint32_t *outBufferLength );

/* These two functions work as a pair */
int32_t DivXSha256Init( sha256Handle_t* handle );
int32_t DivXSha256Update( sha256Handle_t handle,
                          uint8_t *inBuffer,
                          uint32_t inBufferLength );

int32_t DivXSha256Final( sha256Handle_t handle,
                         uint8_t *outBuffer,
                         uint32_t *outBufferLength );
                     
int32_t DivXSha1( uint8_t *inBuffer,
                  uint32_t inBufferLength,
                  uint8_t *outBuffer,
                  uint32_t *outBufferLength );
                                          
#ifdef __cplusplus
};
#endif

#endif
