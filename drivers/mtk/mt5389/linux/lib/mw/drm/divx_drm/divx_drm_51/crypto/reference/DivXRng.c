/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/crypto/reference/DivXRng.c#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/
#ifndef __KERNEL__

#include "DivXRng.h"
#include "DivXAesDecrypt.h"
#include "DivXAesEncrypt.h"

#ifdef _MTK_BUILD_
#include "libc/memory.h"
#include "libc/stdlib.h"
#include "libc/time.h"
#include "libc/string.h"
#else
#include <stdlib.h>
#include <time.h>
#include <string.h>
#endif

#else

#include "../DivXRng.h"
#include "../DivXAesDecrypt.h"
#include "../DivXAesEncrypt.h"

#endif

//#define PREDICTABLE

int32_t DivXRngGetRandomBytes ( uint8_t *buffer,
                                uint32_t bufferLength )
{

#ifdef PREDICTABLE

   memset( buffer,
           0xA5,
           bufferLength );
   return 0;

#else

    uint8_t inputBuffer[64];
    uint8_t outBuffer[64];
    uint32_t outBufferLength;
    uint32_t tempLength = bufferLength;
    uint8_t binaryKeyMaterial[32];
    aesKeyHandle keyHandle;
    int i = 0;

    while ( tempLength > 0 )
    {
        /* Prep the binary key material */
        for ( i = 0; i < sizeof(binaryKeyMaterial); i++ )
        {
            binaryKeyMaterial[i] = (uint8_t)( rand ( ) & 0xFF );
        }

        /* Prep the input buffer */
        for ( i = 0; i < sizeof(inputBuffer); i++ )
        {
            inputBuffer[i] = rand ( ) & 0xFF;
        }

        DivXAesCreateKey ( &keyHandle,
                           aesKeySize32Bytes,
                           binaryKeyMaterial );
        outBufferLength = sizeof ( outBuffer );
        DivXAesECBEncrypt ( keyHandle,
                            inputBuffer,
                            sizeof(inputBuffer),
                            outBuffer,
                            &outBufferLength );
        DivXAesDeleteKey ( keyHandle );
        if ( tempLength < outBufferLength )
        {
            memcpy ( buffer + (bufferLength - tempLength),
                     outBuffer,
                     tempLength );
            tempLength = 0;
        }
        else
        {
            memcpy ( buffer + (bufferLength - tempLength),
                     outBuffer,
                     outBufferLength );
            tempLength -= outBufferLength;
        }                

    }
    return 0;

#endif

}

