/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/crypto/reference/DivXRsaLocal.h#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/
#ifndef DIVXRSALOCAL_H
#define DIVXRSALOCAL_H

#ifndef __KERNEL__

#include "DivXPortableAPI.h"
#include "bn.h"

#else

#include "../DivXPortableAPI.h"
#include "bn.h"

#endif

/*------------------------------------------------------------------------------------------
 *
 *  TYPE DECLARATIONS
 *
 *-----------------------------------------------------------------------------------------*/

typedef struct _DivXRSA {

    XBN_CTX *xbnContext;
    XBIGNUM *n;
    XBIGNUM *e;
    XBIGNUM *d;

} DivXRSA;


/*------------------------------------------------------------------------------------------
 *
 *  FUNCTION DECLARATIONS
 *
 *-----------------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

DivXRSA *DivXRSA_new( void );
void DivXRSA_free(DivXRSA *divXRSA);
                               
#ifdef __cplusplus
}
#endif
                               

#endif
//DIVXRSALOCAL_H
