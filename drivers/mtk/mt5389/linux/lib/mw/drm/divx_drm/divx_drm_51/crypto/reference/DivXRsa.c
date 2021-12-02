/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/crypto/reference/DivXRsa.c#1 $
// Copyright (c) 2007 DivX, Inc. http://www.divx.com/corporate
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

#ifdef _MTK_BUILD_
#include "libc/stdlib.h"
#else
#include <stdlib.h>
#endif

#else

#include "../DivXRsa.h"
#include "DivXRsaLocal.h"
#include "bn.h"

#endif

/*------------------------------------------------------------------------------------------
 *
 *  LOCAL FUNCTION DECLARATIONS
 *
 *-----------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------
 *
 *  FUNCTION DEFINITIONS
 *
 *-----------------------------------------------------------------------------------------*/
int32_t DivXRsaCreatePublicKey( rsaPublicKeyHandle * keyHandle,
                                const uint8_t * n,
                                int32_t nLength,
                                const uint8_t * e,
                                int32_t eLength )
{
    int32_t retval = -1;
    DivXRSA *rsaKey;

    rsaKey = DivXRSA_new();

    rsaKey->n = XBN_bin2bn(n, nLength, rsaKey->n);
    rsaKey->e = XBN_bin2bn(e, eLength, rsaKey->e);
    if ( rsaKey->n && rsaKey->e )
    {
        retval = 0;
    }

    *keyHandle = (rsaPublicKeyHandle) rsaKey;
    return retval;
}

int32_t DivXRsaDeletePublicKey( rsaPublicKeyHandle keyHandle )
{
    DivXRSA_free((DivXRSA *)keyHandle);
    return 0;
}

int32_t DivXRsaCreatePrivateKey( rsaPrivateKeyHandle * keyHandle,
                                 const uint8_t * n,
                                 int32_t nLength,
                                 const uint8_t * e,
                                 int32_t eLength,
                                 const uint8_t * d,
                                 int32_t dLength )
{

    int32_t retval = -1;
    DivXRSA *rsaKey;

    rsaKey = DivXRSA_new( );

    rsaKey->n = XBN_bin2bn(n, nLength, rsaKey->n);
    rsaKey->e = XBN_bin2bn(e, eLength, rsaKey->e);
    rsaKey->d = XBN_bin2bn(d, dLength, rsaKey->d);
    if ( rsaKey->n && rsaKey->e && rsaKey->d )
    {
        retval = 0;
    }

    *keyHandle = (rsaPrivateKeyHandle) rsaKey;
    return retval;

}

int32_t DivXRsaDeletePrivateKey( rsaPrivateKeyHandle keyHandle )
{
    DivXRSA_free((DivXRSA *)keyHandle);
    return 0;
}



DivXRSA *DivXRSA_new( void )
{
    /* create structure memory */
    DivXRSA *divXRSA = (DivXRSA *) malloc(sizeof(DivXRSA));

    /* create new context */
    divXRSA->xbnContext = XBN_CTX_new();

    /* create XBIGNUM variables */
    divXRSA->n = XBN_new();
    divXRSA->e = XBN_new();
    divXRSA->d = XBN_new();

    return divXRSA;
}

void DivXRSA_free(DivXRSA *divXRSA)
{

    /* free XBIGNUM variables */
    XBN_free(divXRSA->n);
    XBN_free(divXRSA->e);
    XBN_free(divXRSA->d);

    /* free context */
    XBN_CTX_free(divXRSA->xbnContext);

    /* free structure memory */
    free(divXRSA);


}
