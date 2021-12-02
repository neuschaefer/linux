/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/crypto/reference/DivXAesEncrypt.c#1 $
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

#include "DivXRng.h"
#include "DivXAesEncrypt.h"
#include "rijndael-api-fst.h"

#ifdef _MTK_BUILD_
#include "libc/string.h"
#else
#include <string.h>
#endif

#else

#include "../DivXRng.h"
#include "../DivXAesEncrypt.h"
#include "rijndael-api-fst.h"

#endif

#define NUM_BITS_PER_BYTE 8

/*------------------------------------------------------------------------------------------
 *
 *  FUNCTION DEFINITIONS
 *
 *-----------------------------------------------------------------------------------------*/
int32_t DivXAesECBEncrypt ( aesKeyHandle keyHandle,
                            uint8_t* inBuffer,
                            uint32_t inBufferLengthInBytes,
                            uint8_t* outBuffer,
                            uint32_t* outBufferLengthInBytes )
{
    aesCipherInstance cipher;
    int32_t encryptedBlockSize;
    
    if ( TRUE == aesCipherInit( &cipher,
                                MODE_ECB,
                                NULL ) )
    {
        ((aesKeyInstance*)keyHandle)->direction = DIR_ENCRYPT;
        encryptedBlockSize = aesBlockEncrypt( &cipher,
                                              (aesKeyInstance*)keyHandle,
                                              inBuffer,
                                              inBufferLengthInBytes * NUM_BITS_PER_BYTE,
                                              outBuffer );
        *outBufferLengthInBytes = encryptedBlockSize / NUM_BITS_PER_BYTE;
        return 0;
    }
    return -1;
}
