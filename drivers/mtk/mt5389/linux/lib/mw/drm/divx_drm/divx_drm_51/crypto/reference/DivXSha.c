/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/crypto/reference/DivXSha.c#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/
#ifndef __KERNEL__

#include "DivXSha.h"
#include "sha256.h"
#ifdef _MTK_BUILD_
#include "libc/memory.h"
#include "libc/string.h"
#else
#include "string.h"
#endif

#else

#include "../DivXSha.h"
#include "sha256.h"

#include "x_os.h" /* malloc/free */
#define malloc      x_mem_alloc
#define free        x_mem_free

#endif

int32_t DivXSha256 ( uint8_t *inBuffer,
                     uint32_t inBufferLength,
                     uint8_t *outBuffer,
                     uint32_t *outBufferLength )
{

    if ( *outBufferLength < 32 )
    {
        *outBufferLength = 0;
        return -1;
    }
    sha256( inBuffer,
            inBufferLength,
            outBuffer );
    *outBufferLength = 32;
    return 0;

}

/* These two functions work as a pair */
int32_t DivXSha256Init( sha256Handle_t* handle )
{
	if ( *handle == 0 )
	{
	    *handle = malloc( sizeof(SHA256_CONTEXT) );
	    if ( *handle == 0 )
	    {
	        return -1;
	    }
    }
	sha256_init( (SHA256_CONTEXT*)(*handle) );
	return 0;
}

int32_t DivXSha256Update( sha256Handle_t handle,
                          uint8_t *inBuffer,
                          uint32_t inBufferLength )
{
	sha256_write( (SHA256_CONTEXT*)handle,
	              inBuffer,
	              inBufferLength );
    return 0;
}

int32_t DivXSha256Final( sha256Handle_t handle,
                         uint8_t *outBuffer,
                         uint32_t *outBufferLength )
{
	sha256_final( (SHA256_CONTEXT*)handle );
	memcpy( outBuffer,
	        sha256_read( (SHA256_CONTEXT*)handle ),
	        32 );
    *outBufferLength = 32;
	if ( handle == 0 )
	{
	    free( handle );
	}
    return 0;
}

int32_t DivXSha1 ( uint8_t *inBuffer,
                   uint32_t inBufferLength,
                   uint8_t *outBuffer,
                   uint32_t *outBufferLength )
{
    return -1;
}
