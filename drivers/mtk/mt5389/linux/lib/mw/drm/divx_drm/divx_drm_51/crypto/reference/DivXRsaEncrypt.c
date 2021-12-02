/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/crypto/reference/DivXRsaEncrypt.c#1 $
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

#include "DivXRsa.h"
#include "DivXRsaLocal.h"
#include "bn.h"

#else

#include "../DivXRsa.h"
#include "DivXRsaLocal.h"
#include "bn.h"

#endif

/*------------------------------------------------------------------------------------------
 *
 *  FUNCTION DECLARATIONS
 *
 *-----------------------------------------------------------------------------------------*/

int32_t DivXRsaPrivateEncrypt( rsaPrivateKeyHandle keyHandle,
                               const uint8_t* message,
                               int32_t messageLength,
                               uint8_t* encryptedData,
                               int32_t* encryptedDataLength )
{

    // Based of bn (big number) arithmitic. Functions extracted from openssl bn library

    XBIGNUM *f, *ret;

    int byteLength;
    int i;
    int padSize;

    DivXRSA *rsaKey = (DivXRSA *) keyHandle;
    f = XBN_new();
    ret = XBN_new();

    // Get ciphered into bn.
    XBN_bin2bn( message,
                messageLength,
                f );

    // Encrypt.
    XBN_mod_exp( ret,
                 f,
                 rsaKey->d,
                 rsaKey->n,
                 rsaKey->xbnContext );

    /* length of plain text should be length of n. If not, then need to pad with leading zero */
    byteLength = XBN_num_bytes( ret );
    *encryptedDataLength = XBN_num_bytes(rsaKey->n);
    padSize = *encryptedDataLength - byteLength;

    /* convert unencrypted datato binary */
    if ( 0 == XBN_bn2bin( ret,
                          &encryptedData[padSize] ) )
    {
        return -1;
    }
    for ( i = 0; i < padSize; i++ )
    {
        encryptedData[i] = 0;
    }

    // Clean up.
    XBN_free( ret );
    XBN_free( f );

    return 0;


}



int32_t DivXRsaPublicEncrypt( rsaPublicKeyHandle keyHandle,
                              const uint8_t* message,
                              int32_t messageLength,
                              uint8_t* encryptedData,
                              int32_t* encryptedDataLength )
{

    // Based of bn (big number) arithmitic. Functions extracted from openssl bn library

    XBIGNUM *f, *ret;

    int byteLength;
    int i;
    int padSize;

    DivXRSA *rsaKey = (DivXRSA *) keyHandle;
    f = XBN_new();
    ret = XBN_new();

    // Get ciphered into bn.
    XBN_bin2bn( message,
                messageLength,
                f );

    // Encrypt.
    XBN_mod_exp( ret,
                 f,
                 rsaKey->e,
                 rsaKey->n,
                 rsaKey->xbnContext );

    /* length of plain text should be length of n. If not, then need to pad with leading zero */
    byteLength = XBN_num_bytes( ret );
    *encryptedDataLength = XBN_num_bytes(rsaKey->n);
    padSize = *encryptedDataLength - byteLength;

    /* convert unencrypted datato binary */
    if ( 0 == XBN_bn2bin( ret,
                          &encryptedData[padSize] ) )
    {
        return -1;
    }
    for ( i = 0; i < padSize; i++ )
    {
        encryptedData[i] = 0;
    }

    // Clean up.
    XBN_free( ret );
    XBN_free( f );

    return 0;


}
