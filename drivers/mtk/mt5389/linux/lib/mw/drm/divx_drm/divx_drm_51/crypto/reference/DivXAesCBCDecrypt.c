/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/crypto/reference/DivXAesCBCDecrypt.c#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

/*------------------------------------------------------------------------------------------
 *
 *  LOCAL INCLUDES
 *
 *-----------------------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "DivXAesCBCDecrypt.h"
#include "rijndael-api-fst.h"

#ifdef _MTK_BUILD_
#include "libc/stdlib.h"
#else
#include <stdlib.h>
#endif

#else

#include "../DivXAesCBCDecrypt.h"
#include "rijndael-api-fst.h"

#endif

#define NUM_BITS_PER_BYTE 8

/*------------------------------------------------------------------------------------------
 *
 *  FUNCTION DEFINITIONS
 *
 *-----------------------------------------------------------------------------------------*/
int32_t DivXAesCBCDecrypt ( aesKeyHandle keyHandle,
                            uint8_t* inBuffer,
                            uint32_t inBufferLengthInBytes,
                            uint8_t* outBuffer,
                            uint32_t* outBufferLengthInBytes )
{
    return -1;
}
