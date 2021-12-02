/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/crypto/DivXPortableAPI.c#1 $
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
#ifndef EXTERNAL_DIVX_PORTABLE_INITIALIZATION

#include "DivXPortableAPI.h"



DECLARE_DIVX_PORTABLE_MODULE

/*
 *
 */
int32_t DivXPortableInitialize ( void )
{

    int32_t i = 0;
    int32_t j = 0;
    int32_t retVal = 0;

    /*
     * For all the modules that are added go ahead and 
     * initialize them
     *
     */
    for ( i = 0; i < NUMBER_OF_PORTABLE_MODULES; i++ )
    {
        /* Check initialize pointer and terminate it NULL marker is found. */         
        if ( PORTABLE_MODULES_ARRAY_NAME[i].initializeFPtr == NULL )
        {
            break;
        }

        retVal = PORTABLE_MODULES_ARRAY_NAME[i].initializeFPtr( &(PORTABLE_MODULES_ARRAY_NAME[i].handle) );
        if ( retVal != 0 )
        {
            /* Finalize the modules that did successfully get initialized */
            for ( j = 0; j < i; i++ )
            {
                PORTABLE_MODULES_ARRAY_NAME[i].finalizeFPtr( &(PORTABLE_MODULES_ARRAY_NAME[i].handle) );
            }
            break;
        }
    }

    return retVal;

}


/*
 *
 */
int32_t DivXPortableFinalize ( void )
{
    int32_t i = 0;
    int32_t retVal = 0;

    /*
     * For all the modules that are added go ahead and 
     * finalize them.  If a module fails to finalize,
     * don't error out, but do report the failure.
     *
     */
    for ( i = 0; i < NUMBER_OF_PORTABLE_MODULES; i++ )
    {
        if ( PORTABLE_MODULES_ARRAY_NAME[i].finalizeFPtr == NULL )
        {
            break;
        }
        retVal |= PORTABLE_MODULES_ARRAY_NAME[i].finalizeFPtr( &(PORTABLE_MODULES_ARRAY_NAME[i].handle) );
    }

    return retVal;
}

#endif
