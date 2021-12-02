/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/crypto/reference/DivXRsaDecrypt.c#1 $
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
 *  FUNCTION DEFINITIONS
 *
 *-----------------------------------------------------------------------------------------*/


int32_t DivXRsaPrivateDecrypt( rsaPrivateKeyHandle keyHandle,
                               const uint8_t * encryptedData,
                               int32_t encryptedDataLength,
                               uint8_t * message,
                               int32_t * messageLength )
{


    // Based of bn (big number) arithmitic. Functions extracted from openssl bn library

    XBIGNUM *c;
    XBIGNUM *t;
    int byteLength;
    int i;
    int padSize;

    DivXRSA *rsaKey = (DivXRSA *) keyHandle;
    c = XBN_new();
    t = XBN_new();

    // Get ciphered into bn.
    XBN_bin2bn( encryptedData,
                encryptedDataLength,
                c );

    // Decrypt.
    XBN_mod_exp( t,
                 c,
                 rsaKey->d,
                 rsaKey->n,
                 rsaKey->xbnContext );

    /* length of plain text should be length of n. If not, then need to pad with leading zero */
    byteLength = XBN_num_bytes( t );
    *messageLength = XBN_num_bytes( rsaKey->n );
    padSize = *messageLength - byteLength;

    /* convert unencrypted datato binary */
    if ( 0 == XBN_bn2bin( t,
                          &message[padSize] ) )
    {
        return -1;
    }
    for ( i = 0; i < padSize; i++ )
    {
        message[i] = 0;
    }

    // Clean up.
    XBN_free(c);
    XBN_free(t);

    return 0;
}


int32_t DivXRsaPublicDecrypt( rsaPublicKeyHandle keyHandle,
                              const uint8_t * encryptedData,
                              int32_t encryptedDataLength,
                              uint8_t * message,
                              int32_t * messageLength )
{


    // Based of bn (big number) arithmitic. Functions extracted from openssl bn library

    XBIGNUM *c;
    XBIGNUM *t;
    int byteLength;
    int i;
    int padSize;

    DivXRSA *rsaKey = (DivXRSA *) keyHandle;
    c = XBN_new();
    t = XBN_new();

    // Get ciphered into bn.
    XBN_bin2bn( encryptedData,
                encryptedDataLength,
                c );

    // Decrypt.
    XBN_mod_exp( t,
                 c,
                 rsaKey->e,
                 rsaKey->n,
                 rsaKey->xbnContext );

    /* length of plain text should be length of n. If not, then need to pad with leading zero */
    byteLength = XBN_num_bytes( t );
    *messageLength = XBN_num_bytes( rsaKey->n );
    padSize = *messageLength - byteLength;

    /* convert unencrypted datato binary */
    if ( 0 == XBN_bn2bin( t,
                          &message[padSize] ) )
    {
        return -1;
    }
    for ( i = 0; i < padSize; i++ )
    {
        message[i] = 0;
    }

    // Clean up.
    XBN_free(c);
    XBN_free(t);

    return 0;
}

