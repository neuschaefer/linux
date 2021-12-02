/**
  Copyright (C) 2008 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmsysOM3869SInstance.c
 *                %version: 2 %
 *
 * \date          %date_modified%
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *
 * Detailed description may be added here.
 *
 * \section info Change Information
 *
 * \verbatim
   Date          Modified by CRPRNr  TASKNr  Maintenance description
   -------------|-----------|-------|-------|-----------------------------------
   26-Nov-2007  | V.VRIGNAUD|       |       | COMPATIBILITY WITH NXPFE
                | C.CAZETTES|       |       | 
   -------------|-----------|-------|-------|-----------------------------------
                |           |       |       |
   -------------|-----------|-------|-------|-----------------------------------
   \endverbatim
 *
*/

#include "tmNxTypes.h"
#include "tmCompId.h"
#include "tmbslFrontEndCfgItem.h"
#include "tmFrontEnd.h"
#include "tmUnitParams.h"

#include "tmbslFrontEndTypes.h"
#include "tmsysFrontEndTypes.h"


#include "tmsysOM3869S.h"
#include "tmsysOM3869Slocal.h"
#include "tmsysOM3869SInstance.h"


//-----------------------------------------------------------------------------
// Global data:
//-----------------------------------------------------------------------------
//
//
tmOM3869SObject_t gOM3869SInstance[] =
{
    {
        (tmUnitSelect_t)(-1),                                       /* OM3869S unit number */
        (tmUnitSelect_t)(-1),                                       /* OM3869S unit number temporary */
        (tmUnitSelect_t)(-1),                                       /* TDA182I2 unit number */
        Null,                                                       /* pMutex */
        False,                                                      /* init */
        {                                                           /* sRWFunc */
            Null,
            Null
        },
        {                                                           /* sTime */
            Null,
            Null
        },
        {                                                           /* sDebug */
            Null
        },
        {                                                           /* sMutex */
            Null,
            Null,
            Null,
            Null
        },
        tmPowerStandby,                                             /* Power State */
        False,                                                      /* resetDone */
        {                                                           /* uHwAddress */
            0x00,                                                   /* tmOM3869SUnitDeviceTypeUnknown */
            0xC0,                                                   /* tmOM3869SUnitDeviceTypeTDA182I2 */
        },
    },
    {
        (tmUnitSelect_t)(-1),                                       /* OM3869S unit number */
        (tmUnitSelect_t)(-1),                                       /* OM3869S unit number temporary */
        (tmUnitSelect_t)(-1),                                       /* TDA182I2 unit number */
        Null,                                                       /* pMutex */
        False,                                                      /* init */
        {                                                           /* sRWFunc */
            Null,
            Null
        },
        {                                                           /* sTime */
            Null,
            Null
        },
        {                                                           /* sDebug */
            Null
        },
        {                                                           /* sMutex */
            Null,
            Null,
            Null,
            Null
        },
        tmPowerStandby,                                             /* Power State */
        False,                                                      /* resetDone */
        {                                                           /* uHwAddress */
            0x00,                                                   /* tmOM3869SUnitDeviceTypeUnknown */
            0xC6,                                                   /* tmOM3869SUnitDeviceTypeTDA182I2 */
        },
    }
};


//-----------------------------------------------------------------------------
// FUNCTION:    OM3869SAllocInstance:
//
// DESCRIPTION: allocate new instance
//
// RETURN:      
//
// NOTES:       
//-----------------------------------------------------------------------------
//
tmErrorCode_t
OM3869SAllocInstance
(
    tmUnitSelect_t      tUnit,      /* I: Unit number */
    pptmOM3869SObject_t  ppDrvObject /* I: Device Object */
)
{ 
    tmErrorCode_t       err = OM3869S_ERR_BAD_UNIT_NUMBER;
    ptmOM3869SObject_t   pObj = Null;
    UInt32              uLoopCounter = 0;    

    /* Find a free instance */
    for(uLoopCounter = 0; uLoopCounter<OM3869S_MAX_UNITS; uLoopCounter++)
    {
        pObj = &gOM3869SInstance[uLoopCounter];
        if(pObj->init == False)
        {
            pObj->tUnit = tUnit;
            pObj->tUnitW = tUnit;

            *ppDrvObject = pObj;
            err = TM_OK;
            break;
        }
    }

    /* return value */
    return err;
}

//-----------------------------------------------------------------------------
// FUNCTION:    OM3869SDeAllocInstance:
//
// DESCRIPTION: deallocate instance
//
// RETURN:      always TM_OK
//
// NOTES:       
//-----------------------------------------------------------------------------
//
tmErrorCode_t
OM3869SDeAllocInstance
(
    tmUnitSelect_t  tUnit   /* I: Unit number */
)
{     
    tmErrorCode_t       err = OM3869S_ERR_NOT_INITIALIZED;
    ptmOM3869SObject_t   pObj = Null;

    /* check input parameters */
    err = OM3869SGetInstance(tUnit, &pObj);

    /* check driver state */
    if (err == TM_OK)
    {
        if (pObj == Null || pObj->init == False)
        {
            err = OM3869S_ERR_NOT_INITIALIZED;
        }
    }

    if ((err == TM_OK) && (pObj != Null)) 
    {
        pObj->init = False;
    }

    /* return value */
    return err;
}

//-----------------------------------------------------------------------------
// FUNCTION:    OM3869SGetInstance:
//
// DESCRIPTION: get the instance
//
// RETURN:      always True
//
// NOTES:       
//-----------------------------------------------------------------------------
//
tmErrorCode_t
OM3869SGetInstance
(
    tmUnitSelect_t      tUnit,      /* I: Unit number */
    pptmOM3869SObject_t  ppDrvObject /* I: Device Object */
)
{     
    tmErrorCode_t       err = OM3869S_ERR_NOT_INITIALIZED;
    ptmOM3869SObject_t   pObj = Null;
    UInt32              uLoopCounter = 0;    

    /* get instance */
    for(uLoopCounter = 0; uLoopCounter<OM3869S_MAX_UNITS; uLoopCounter++)
    {
        pObj = &gOM3869SInstance[uLoopCounter];

		mcSHOW_DBG_MSG(("OM3869SGetInstance:tUnit(0x%08X) .\n", pObj->tUnit));
		mcSHOW_DBG_MSG(("OM3869SGetInstance:tUnitW(0x%08X) .\n", pObj->tUnitW));
		mcSHOW_DBG_MSG(("OM3869SGetInstance:init(0x%08X) .\n", pObj->init));
		if ((pObj->sRWFunc.Read !=Null) &&(pObj->sRWFunc.Write!= Null))
			{

             mcSHOW_DBG_MSG(("OM3869SGetInstance:sRWFuncis not null.\n"));
		    }
		else
			{
              mcSHOW_DBG_MSG(("OM3869SGetInstance:sRWFunc is  null.\n"));

		    }
		
        if(pObj->init == True && pObj->tUnit == GET_INDEX_TYPE_TUNIT(tUnit))
        {
            pObj->tUnitW = tUnit;

            *ppDrvObject = pObj;
            err = TM_OK;
            break;
        }
    }

    /* return value */
    return err;
}

