/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/crypto/reference/DivXAesDecrypt.c#1 $
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

#include "DivXAesDecrypt.h"
#include "rijndael-api-fst.h"
#ifdef _MTK_BUILD_
#include "libc/stdlib.h"
#else
#include <stdlib.h>
#endif

#else

#include "../DivXAesDecrypt.h"
#include "rijndael-api-fst.h"

#include "x_os.h" /* malloc/free */
#define malloc      x_mem_alloc
#define free        x_mem_free

#endif

#define NUM_BITS_PER_BYTE 8
/*------------------------------------------------------------------------------------------
 *
 *  FUNCTION DEFINITIONS
 *
 *-----------------------------------------------------------------------------------------*/
int32_t DivXAesCreateKey ( aesKeyHandle* keyHandle,
                           aesKeySizesInBytes keySizeInBytes,
                           uint8_t *binaryKeyMaterial )
{
    int32_t retVal;

    aesKeyInstance *sessionKeyInstance = malloc( sizeof (aesKeyInstance) );
 
    if ( NULL == sessionKeyInstance )
    {
        return -1;
    }

    retVal = aesMakeKey( sessionKeyInstance,
                         DIR_ENCRYPT,
                         keySizeInBytes * NUM_BITS_PER_BYTE,
                         (char*)binaryKeyMaterial );

    /* makeKey returns < 0 for error and 1 for success */
    if ( retVal < 0 )
    {
        free( sessionKeyInstance );
        return -1;
    }
    else
    {
        *keyHandle = (uint32_t)sessionKeyInstance;
    }

    return 0;
}


int32_t DivXAesDeleteKey ( aesKeyHandle keyHandle )
{
    if ( keyHandle )
    {
        free ( (char*)(keyHandle) );
        return 0;
    }
    return -1;
}

int32_t DivXAesECBDecrypt ( aesKeyHandle keyHandle,
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
        ((aesKeyInstance*)keyHandle)->direction = DIR_DECRYPT;
        encryptedBlockSize = aesBlockDecrypt( &cipher,
                                              (aesKeyInstance*)keyHandle,
                                              inBuffer,
                                              inBufferLengthInBytes * NUM_BITS_PER_BYTE,
                                              outBuffer );
        *outBufferLengthInBytes = encryptedBlockSize / NUM_BITS_PER_BYTE;
        return 0;
    }
    return -1;
}
