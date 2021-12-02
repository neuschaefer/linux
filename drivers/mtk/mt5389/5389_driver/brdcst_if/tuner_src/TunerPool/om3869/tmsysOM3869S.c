/**
Copyright (C) 2008 NXP B.V., All Rights Reserved.
This source code and any compilation or derivative thereof is the proprietary
information of NXP B.V. and is confidential in nature. Under no circumstances
is this software to be  exposed to or placed under an Open Source License of
any type without the expressed written permission of NXP B.V.
*
* \file          tmsysOM3869S_63.c
*                %version: 11 %
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
9-JULY-2007  | A.TANT    |       |       | CREATION OF TDA10048 ARCHITECTURE 2.0.0
-------------|-----------|-------|-------|-----------------------------------
|           |       |       |
-------------|-----------|-------|-------|-----------------------------------
\endverbatim
*
*/


/*============================================================================*/
/*                   STANDARD INCLUDE FILES                                   */
/*============================================================================*/


/*============================================================================*/
/*                   PROJECT INCLUDE FILES                                    */
/*============================================================================*/
#include "tmNxTypes.h"
#include "tmCompId.h"
#include "tmFrontEnd.h"
#include "tmbslFrontEndTypes.h"
#include "tmsysFrontEndTypes.h"
#include "tmUnitParams.h"

#include "tmbslTDA18272.h"
#include "tmddTDA182I2.h"

#include "tmsysOM3869S.h"
#include "tmsysOM3869Slocal.h"
#include "tmsysOM3869SInstance.h"
#include "PD_Def.h"  //for debug printf

/*============================================================================*/
/*                   MACRO DEFINITION                                         */
/*============================================================================*/

#ifndef SIZEOF_ARRAY
#define SIZEOF_ARRAY(ar)        (sizeof(ar)/sizeof((ar)[0]))
#endif // !defined(SIZEOF_ARRAY)

/*============================================================================*/
/*                   TYPE DEFINITION                                          */
/*============================================================================*/



/*============================================================================*/
/*                   PUBLIC VARIABLES DEFINITION                              */
/*============================================================================*/

/*============================================================================*/
/*                   STATIC VARIABLES DECLARATIONS                            */
/*============================================================================*/

/*============================================================================*/
/*                       EXTERN FUNCTION PROTOTYPES                           */
/*============================================================================*/


/*============================================================================*/
/*                   STATIC FUNCTIONS DECLARATIONS                            */
/*============================================================================*/


/*============================================================================*/
/*                   PUBLIC FUNCTIONS DEFINITIONS                             */
/*============================================================================*/



/*============================================================================*/
/*                   PROJECT INCLUDE FILES                                    */
/*============================================================================*/


/*============================================================================*/
/*                   TYPE DEFINITION                                          */
/*============================================================================*/


/*============================================================================*/
/*                   STATIC VARIABLES DECLARATIONS                            */
/*============================================================================*/


/*============================================================================*/
/*                   PUBLIC FUNCTIONS DECLARATIONS                            */
/*============================================================================*/



/*============================================================================*/
/*                   STATIC FUNCTIONS DECLARATIONS                            */
/*============================================================================*/

/*============================================================================*/
/*                   PUBLIC FUNCTIONS DEFINITIONS                             */
/*============================================================================*/


/*============================================================================*/
/*============================================================================*/
/* tmsysOM3869SGetSWVersion                                                 */
/*============================================================================*/
tmErrorCode_t
tmsysOM3869SGetSWVersion
(
    ptmsysSWVersion_t   pSWVersion  /* O: Receives SW Version  */
)
{
    tmErrorCode_t   err = TM_OK;
    static char     OM3869SName[] = "tmsysOM3869S\0";
    static char     TDA18272Name[] = "tmbslTDA18272\0";

    pSWVersion->arrayItemsNumber = 2;
    pSWVersion->swVersionArray[0].pName = (char *)OM3869SName;
    pSWVersion->swVersionArray[0].nameBufSize = sizeof(OM3869SName);
    
    pSWVersion->swVersionArray[0].swVersion.compatibilityNr = OM3869S_SYS_COMP_NUM;
    pSWVersion->swVersionArray[0].swVersion.majorVersionNr = OM3869S_SYS_MAJOR_VER;
    pSWVersion->swVersionArray[0].swVersion.minorVersionNr = OM3869S_SYS_MINOR_VER;
    
    pSWVersion->swVersionArray[1].pName = (char *)TDA18272Name;
    pSWVersion->swVersionArray[1].nameBufSize = sizeof(TDA18272Name);
    err = tmbslTDA182I2GetSWVersion(&pSWVersion->swVersionArray[1].swVersion);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA182I2GetSWVersion failed."));

    return err;
}

/*============================================================================*/
/* tmsysOM3869SInit                                                         */
/*============================================================================*/
tmErrorCode_t
tmsysOM3869SInit
(
    tmUnitSelect_t              tUnit,      /* I: FrontEnd unit number */
    tmbslFrontEndDependency_t*  psSrvFunc   /*  I: setup parameters */
)
{
    ptmOM3869SObject_t    pObj = Null;
    tmErrorCode_t           err = TM_OK;

    // pObj initialization
    err = OM3869SGetInstance(tUnit, &pObj);
	   if (err != TM_OK)
		  {
            mcSHOW_USER_MSG(("tmsysOM3869Init:Need AllocInstance!\n"));
		  }
	    if (err == TM_OK)
		  {
            mcSHOW_USER_MSG(("tmsysOM3869Init:OM3869GetInstance success!\n"));
		  }

    /* check driver state */
    if (err == TM_OK || err == OM3869S_ERR_NOT_INITIALIZED)
    {
        if (pObj != Null && pObj->init == True)
        {
            err = OM3869S_ERR_NOT_INITIALIZED;
        }
        else 
        {
            /* initialize the Object */
            if (pObj == Null)
            {
                mcSHOW_USER_MSG(("tmsysOM3869Init:Start AllocInstance !\n"));
				err = OM3869SAllocInstance(tUnit, &pObj);
				if (err != TM_OK)
					{
                      mcSHOW_USER_MSG(("tmsysOM3869Init:OM3869AllocInstance fail!\n"));
				    }
                if (err != TM_OK || pObj == Null)
                {
                    err = OM3869S_ERR_NOT_INITIALIZED;        
                }
            }

            if (err == TM_OK)
            {

               mcSHOW_USER_MSG(("tmsysOM3869Init:AllocInstance success !\n"));
				// initialize the Object by default values
                pObj->sRWFunc = psSrvFunc->sIo;
                pObj->sTime = psSrvFunc->sTime;
                pObj->sDebug = psSrvFunc->sDebug;

                if(  psSrvFunc->sMutex.Init != Null
                    && psSrvFunc->sMutex.DeInit != Null
                    && psSrvFunc->sMutex.Acquire != Null
                    && psSrvFunc->sMutex.Release != Null)
                {
                    pObj->sMutex = psSrvFunc->sMutex;

                    err = pObj->sMutex.Init(&pObj->pMutex);
                }

                pObj->tUnitTDA182I2 = GET_INDEX_TUNIT(tUnit)|UNIT_PATH_TYPE_VAL(tmOM3869SUnitDeviceTypeTDA182I2);

                pObj->init = True;
                err = TM_OK;

                err = OM3869SInit(tUnit);
				if (err != TM_OK)
					{

					 mcSHOW_USER_MSG(("OM3869Init fail!\n"));
					 
				    }
				else
					{

                         mcSHOW_USER_MSG(("OM3869Init success!\n"));
				    }
           }
        }
    }

    return err;
}

/*============================================================================*/
/* tmsysOM3869SDeInit                                                          */
/*============================================================================*/
tmErrorCode_t
tmsysOM3869SDeInit
(
    tmUnitSelect_t  tUnit   /* I: FrontEnd unit number */
)
{
    ptmOM3869SObject_t    pObj = Null;
    tmErrorCode_t           err = TM_OK;

    err = OM3869SGetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "OM3869SGetInstance(0x%08X) failed.", tUnit));

    /************************************************************************/
    /* DeInitialize TDA182I2 low layer setup                                */
    /************************************************************************/
    if(err == TM_OK)
    {
        if(pObj->sMutex.DeInit != Null)
        {
            if(pObj->pMutex != Null)
            {
                err = pObj->sMutex.DeInit(pObj->pMutex);
            }

            pObj->sMutex.Init = Null;
            pObj->sMutex.DeInit = Null;
            pObj->sMutex.Acquire = Null;
            pObj->sMutex.Release = Null;

            pObj->pMutex = Null;
        }
        
        err = tmbslTDA182I2DeInit(pObj->tUnitTDA182I2);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA182I2DeInit(0x%08X) failed.", pObj->tUnitTDA182I2));
    }

 
    err = OM3869SDeAllocInstance(tUnit);

    return err;
}

/*============================================================================*/
/* tmsysOM3869SReset                                                        */
/*============================================================================*/
tmErrorCode_t
tmsysOM3869SReset
(
    tmUnitSelect_t  tUnit   /* I: FrontEnd unit number */
)
{
    ptmOM3869SObject_t    pObj = Null;
    tmErrorCode_t           err = OM3869S_ERR_NOT_INITIALIZED;

    /* check input parameters */
    err = OM3869SGetInstance(tUnit, &pObj);
	if (err != TM_OK)
		{
         mcSHOW_USER_MSG(("OM3869SGetInstance fail!\n"));
	    }
	 else
	 	{
          mcSHOW_USER_MSG(("OM3869SGetInstance success!\n"));
	    }
		
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "OM3869SGetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        err = OM3869SMutexAcquire(pObj, OM3869S_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        err = OM3869SReset(tUnit);
		if (err != TM_OK)
		{
         mcSHOW_USER_MSG(("OM3869SReset fail!\n"));
	    }
		else
	 	{
          mcSHOW_USER_MSG(("OM3869SReset success!\n"));
	    }
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "OM3869SReset(0x%08X) failed.", tUnit));

        (void)OM3869SMutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* tmsysOM3869SSetPowerState                                                */
/*============================================================================*/
tmErrorCode_t
tmsysOM3869SSetPowerState
(
    tmUnitSelect_t  tUnit,      /* I: FrontEnd unit number */
    tmPowerState_t  ePowerState /* I: Power state of the device */
)
{
    ptmOM3869SObject_t    pObj = Null;
    tmErrorCode_t           err = TM_OK;

    err = OM3869SGetInstance(tUnit, &pObj);
	 if (err != TM_OK)
	 	{
           mcSHOW_DBG_MSG(("tmsysOM3869SetPowerState:OM3869GetInstance failed.\n"));	
	    }
   

    if(err == TM_OK)
    {
        err = OM3869SMutexAcquire(pObj, OM3869S_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        switch(ePowerState)
        {
        case tmPowerOn:
            if (err == TM_OK)
            {
                pObj->powerState = ePowerState;

                /* Set TDA182I2 power state to Normal Mode */
                err = tmbslTDA182I2SetPowerState(pObj->tUnitTDA182I2, tmTDA182I2_PowerNormalMode);
				if (err != TM_OK)
	 	         {
                    mcSHOW_DBG_MSG(("tmsysOM3869SetPowerState:tmbslTDA182I2SetPowerState failed.\n"));	
	             }
                
            }
            break;

        case tmPowerStandby:
            if (err == TM_OK)
            {
                pObj->powerState = ePowerState;

                /* Set TDA182I2 power state to standby with Xtal and Loop Through*/
                err = tmbslTDA182I2SetPowerState(pObj->tUnitTDA182I2, tmTDA182I2_PowerStandbyWithLNAOnAndWithXtalOn);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA182I2SetPowerState(0x%08X, tmPowerStandby) failed.", pObj->tUnitTDA182I2));
            }
            break;

        case tmPowerSuspend:
        case tmPowerOff:
        default:
            err = OM3869S_ERR_NOT_SUPPORTED;
            break;
        }

        if (err == TM_OK)
        {
            pObj->lastTuneReqType = TRT_UNKNOWN;
        }

        (void)OM3869SMutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* tmsysOM3869SSetPowerState                                                */
/*============================================================================*/
tmErrorCode_t
tmsysOM3869SGetPowerState
(
    tmUnitSelect_t  tUnit,      /* I: FrontEnd unit number */
    ptmPowerState_t pPowerState /* O: Power state of the device */
)
{
    ptmOM3869SObject_t    pObj = Null;
    tmErrorCode_t           err = TM_OK;

    err = OM3869SGetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "OM3869SGetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        err = OM3869SMutexAcquire(pObj, OM3869S_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        *pPowerState = pObj->powerState;

        (void)OM3869SMutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* tmsysOM3869SSendRequest                                                  */
/*============================================================================*/
tmErrorCode_t
tmsysOM3869SSendRequest
(
    tmUnitSelect_t  tUnit,              /* I: FrontEnd unit number */
    pVoid           pTuneRequest,       /* I/O: Tune Request Structure pointer */
    UInt32          dwTuneRequestSize,  /* I: Tune Request Structure size */
    tmTuneReqType_t tTuneReqType,        /* I: Tune Request Type */
    tmTDA182I2StandardMode_t  sysMode     //add by rui.mei
)
{
    ptmOM3869SObject_t    pObj = Null;
    tmErrorCode_t           err = TM_OK;

    if(err == TM_OK)
    {
        err = OM3869SGetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "OM3869SGetInstance(0x%08X) failed.", tUnit));
    }

    // check pointer
    if( !pTuneRequest )
    {
        tmDBGPRINTEx(DEBUGLVL_ERROR, "Error: Invalid Pointer!");
        mcSHOW_DBG_MSG(("Error: Invalid pTuneRequest Pointer!\n"));
        err = OM3869S_ERR_BAD_PARAMETER;
    }

    //if(err == TM_OK  && !pObj->resetDone)
    //{
    //    err = OM3869S_ERR_NOT_INITIALIZED;
    //}

    if(err == TM_OK)
    {
        err = OM3869SMutexAcquire(pObj, OM3869S_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        switch(tTuneReqType)
        {
        case TRT_TUNER_ONLY:
            if( dwTuneRequestSize != sizeof(tmTunerOnlyRequest_t) )
            {
                err = TM_ERROR;
            }
            else
            {
                if (err == TM_OK)
                {
                 
                    err = OM3869SSetFrequency(tUnit, pTuneRequest,TRT_TUNER_ONLY, sysMode);
                    if (err != TM_OK)
						{
						   
						   mcSHOW_DBG_MSG(( "Error: OM3869SetFrequency failed.\n"));
					    }    
                }
            }
            break;
			
		case TRT_DVBT:
            if( dwTuneRequestSize != sizeof(tmDVBTRequest_t) )
            {
                tmDBGPRINTEx(DEBUGLVL_ERROR, "Error: Bad parameter");
                err = OM3869S_ERR_BAD_PARAMETER;
            }
            else
            {
                if (err == TM_OK)
                {
                    
                    err = OM3869SSetFrequency(tUnit,pTuneRequest,TRT_DVBT, sysMode);
                    if (err != TM_OK)
						{
						   
						   mcSHOW_DBG_MSG(( "Error: OM3869SetFrequency failed.\n"));
					    }    
                }
            }
            break;
          case  TRT_ANALOG_TV:
            if( dwTuneRequestSize != sizeof(tmAnalogTVRequest_t) )
            {
                tmDBGPRINTEx(DEBUGLVL_ERROR, "Error: Bad parameter");
                err = TM_ERROR;
            }
            else
            {
                if (err == TM_OK)
                {
                    //ptmAnalogTVRequest_t pTunerAnalogRequest = (ptmAnalogTVRequest_t)(pTuneRequest);
                    err = OM3869SSetFrequency(tUnit, pTuneRequest,TRT_ANALOG_TV, sysMode);
					if (err != TM_OK)
						{
						   
						   mcSHOW_DBG_MSG(( "Error: OM3869SetFrequency failed.\n"));
					    }                 
                }
            }
            break;
        default:
            tmDBGPRINTEx(DEBUGLVL_ERROR, "Error: Unsupported tune request");
			mcSHOW_DBG_MSG(("Error: Unsupported tune request!:\n "));
            err = OM3869S_ERR_NOT_SUPPORTED;
            break;
        }

        (void)OM3869SMutexRelease(pObj);
    }
    if (err == TM_OK )
    	{

		mcSHOW_DBG_MSG(("callfuction tmsysOM3869SendRequest success!:\n "));
    	}
	else
		{
		mcSHOW_DBG_MSG(("callfuction tmsysOM3869SendRequest fail!:\n "));
		}
    return err;
}

/*============================================================================*/
/* tmsysOM3869SSetI2CSwitchState                                               */
/*============================================================================*/
tmErrorCode_t
tmsysOM3869SSetI2CSwitchState
(
    tmUnitSelect_t                  tUnit,  /* I: FrontEnd unit number */
    tmsysFrontEndI2CSwitchState_t   eState  /* I: I2C switch state */
)
{
    return OM3869S_ERR_NOT_SUPPORTED;
}

/*============================================================================*/
/* tmsysOM3869SGetI2CSwitchState                                               */
/*============================================================================*/
tmErrorCode_t
tmsysOM3869SGetI2CSwitchState
(
    tmUnitSelect_t                  tUnit,
    tmsysFrontEndI2CSwitchState_t*  peState,
    UInt32*                         puI2CSwitchCounter
)
{
    return OM3869S_ERR_NOT_SUPPORTED;
}

/*============================================================================*/
/* tmsysOM3869SGetLockStatus                                                */
/*============================================================================*/
tmErrorCode_t
tmsysOM3869SGetLockStatus
(
    tmUnitSelect_t          tUnit,      /* I: FrontEnd unit number */
    tmsysFrontEndState_t*   pLockStatus /* O: Lock status */
)
{
    ptmOM3869SObject_t    pObj = Null;
    tmErrorCode_t           err = TM_OK;
    tmbslFrontEndState_t    lockStatus = tmbslFrontEndStateUnknown;

    err = OM3869SGetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "OM3869SGetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        err = OM3869SMutexAcquire(pObj, OM3869S_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        /* Get TDA182I2 PLL Lock status */
        err = tmbslTDA182I2GetLockStatus(pObj->tUnitTDA182I2, &lockStatus);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA182I2GetLockStatus(0x%08X) failed.", pObj->tUnitTDA182I2));

        if(err == TM_OK)
        {
           *pLockStatus = (tmsysFrontEndState_t)lockStatus;
        }

        (void)OM3869SMutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* tmsysOM3869SGetSignalStrength                                               */
/*============================================================================*/
tmErrorCode_t
tmsysOM3869SGetSignalStrength
(
    tmUnitSelect_t  tUnit,      /* I: FrontEnd unit number */
    UInt32          *pStrength  /* I/O: Signal Strength pointer */
)
{
    return OM3869S_ERR_NOT_SUPPORTED;
}

/*============================================================================*/
/* tmsysOM3869SGetSignalQuality                                                */
/*============================================================================*/
tmErrorCode_t
tmsysOM3869SGetSignalQuality
(
    tmUnitSelect_t  tUnit,      /* I: FrontEnd unit number */
    UInt32          *pQuality   /* I/O: Signal Quality pointer */
)
{
   return OM3869S_ERR_NOT_SUPPORTED;
}

/*============================================================================*/
/* tmsysOM3869SGetDeviceUnit                                                */
/*============================================================================*/
tmErrorCode_t
tmsysOM3869SGetDeviceUnit
(
    tmUnitSelect_t              tUnit,      /* I: FrontEnd unit number */
    tmOM3869SUnitDeviceType_t deviceType, /* I: Device Type  */
    ptmUnitSelect_t             ptUnit      /* O: Device unit number */
)
{
    ptmOM3869SObject_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    err = OM3869SGetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "OM3869SGetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        err = OM3869SMutexAcquire(pObj, OM3869S_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        if(ptUnit!=Null)
        {
            switch(deviceType)
            {
            default:
            case tmOM3869SUnitDeviceTypeUnknown:
                err = OM3869S_ERR_BAD_PARAMETER;
                break;

            case tmOM3869SUnitDeviceTypeTDA182I2:
                *ptUnit = pObj->tUnitTDA182I2;
                break;
            }
        }
        
        (void)OM3869SMutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* tmsysOM3869SSetHwAddress                                                    */
/*============================================================================*/
tmErrorCode_t
tmsysOM3869SSetHwAddress
(
    tmUnitSelect_t              tUnit,      /* I: FrontEnd unit number */
    tmOM3869SUnitDeviceType_t    deviceType, /* I: Device Type  */
    UInt32                      uHwAddress  /* I: Hardware Address */
)
{
    ptmOM3869SObject_t           pObj = Null;
    tmErrorCode_t               err = TM_OK;

    err = OM3869SGetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "OM3869SGetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        err = OM3869SMutexAcquire(pObj, OM3869S_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        if(deviceType>tmOM3869SUnitDeviceTypeUnknown && deviceType<tmOM3869SUnitDeviceTypeMax)
        {
            pObj->uHwAddress[deviceType] = uHwAddress;
        }
        else
        {
            err = OM3869S_ERR_BAD_PARAMETER;
        }

        (void)OM3869SMutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* tmsysOM3869SGetHwAddress                                                    */
/*============================================================================*/
extern tmErrorCode_t
tmsysOM3869SGetHwAddress
(
    tmUnitSelect_t              tUnit,      /* I: FrontEnd unit number */
    tmOM3869SUnitDeviceType_t    deviceType, /* I: Device Type  */
    UInt32*                     puHwAddress /* O: Hardware Address */
)
{
    ptmOM3869SObject_t           pObj = Null;
    tmErrorCode_t               err = TM_OK;

    err = OM3869SGetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "OM3869SGetInstance(0x%08X) failed.", tUnit));

    /* Check incoming pointer */
    if(err == TM_OK && !puHwAddress)
    {
        tmDBGPRINTEx(DEBUGLVL_TERSE,("Error: Invalid Pointer!"));
        err = OM3869S_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        err = OM3869SMutexAcquire(pObj, OM3869S_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        if(deviceType>tmOM3869SUnitDeviceTypeUnknown && deviceType<tmOM3869SUnitDeviceTypeMax)
        {
            *puHwAddress = pObj->uHwAddress[deviceType];
        }
        else
        {
            err = OM3869S_ERR_BAD_PARAMETER;
        }

        (void)OM3869SMutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/*                   STATIC FUNCTIONS DEFINITIONS                             */
/*============================================================================*/

/*============================================================================*/
/* OM3869SInit                                                                 */
/*============================================================================*/
static tmErrorCode_t
OM3869SInit
(
    tmUnitSelect_t  tUnit   /* I: FrontEnd unit number */
)
{
    ptmOM3869SObject_t        pObj = Null;
    tmErrorCode_t               err = TM_OK;
    tmbslFrontEndDependency_t   sSrvFunc;   /* setup parameters */

    err = OM3869SGetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "OM3869SGetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        /* Fill function pointers structure */
        sSrvFunc.sIo                    = pObj->sRWFunc;
        sSrvFunc.sTime                  = pObj->sTime;
        sSrvFunc.sDebug                 = pObj->sDebug;
        sSrvFunc.sMutex                 = pObj->sMutex;
        sSrvFunc.dwAdditionalDataSize   = 0;
        sSrvFunc.pAdditionalData        = Null;

        /************************************************************************/
        /* TDA182I2 low layer setup                                             */
        /************************************************************************/

        err = tmbslTDA182I2Init(pObj->tUnitTDA182I2, &sSrvFunc);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA182I2Init(0x%08X) failed.", pObj->tUnitTDA182I2));
    }

    return err;
}

/*============================================================================*/
/* OM3869SDeInit                                                               */
/*============================================================================*/
/*
static tmErrorCode_t
OM3869SDeInit
(
    tmUnitSelect_t  tUnit   // I: FrontEnd unit number 
)
{
    ptmOM3869SObject_t           pObj = Null;
    tmErrorCode_t               err = TM_OK;

    err = OM3869SGetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "OM3869SGetInstance(0x%08X) failed.", tUnit));
 
    if(err == TM_OK)
    {
        err = tmbslTDA182I2DeInit(pObj->tUnitTDA182I2);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA182I2DeInit(0x%08X) failed.", pObj->tUnitTDA182I2));
    }

    return err;
}*/

/*============================================================================*/
/* OM3869SReset                                                                */
/*============================================================================*/
static tmErrorCode_t
OM3869SReset
(
    tmUnitSelect_t  tUnit   /* I: FrontEnd unit number */
)
{
    ptmOM3869SObject_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    err = OM3869SGetInstance(tUnit, &pObj);
	
		
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "OM3869SGetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        err = tmsysOM3869SSetPowerState(pObj->tUnit, tmPowerStandby);
		if (err != TM_OK)
		{
         mcSHOW_USER_MSG(("tmsysOM3869SSetPowerState fail!\n"));
	    }
	 else
	 	{
          mcSHOW_USER_MSG(("tmsysOM3869SSetPowerState success!\n"));
	    }
    }

    if(err == TM_OK)
    {
        pObj->resetDone = False;

        /************************************************************************/
        /* TDA182I2 initialization                                              */
        /************************************************************************/
        /* Determine if we need to wait in Reset function */
        err = tmbslTDA182I2SetIRQWait(pObj->tUnitTDA182I2, True);

        if(err == TM_OK)
        {
            err = tmbslTDA182I2SetPowerState(pObj->tUnitTDA182I2, tmTDA182I2_PowerNormalMode);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA182I2SetPowerState(0x%08X, tmPowerOn) failed.", pObj->tUnitTDA182I2));
        }

        if(err == TM_OK)
        {
            err = tmbslTDA182I2Reset(pObj->tUnitTDA182I2);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA182I2Reset(0x%08X) failed.", pObj->tUnitTDA182I2));
        }
        if (err == TM_OK)
        {
            err = tmbslTDA182I2SetPowerState(pObj->tUnitTDA182I2, tmTDA182I2_PowerStandbyWithLNAOnAndWithXtalOn);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA182I2SetPowerState(0x%08X, tmPowerStandby) failed.", pObj->tUnitTDA182I2));
        }

        if (err != TM_OK)
        {
            tmDBGPRINTEx(DEBUGLVL_ERROR, "tmbslTDA182I2Reset(0x%08X) failed.", pObj->tUnitTDA182I2);
            /* Open I²C switch to stop Tuner access */
            (void)tmsysOM3869SSetI2CSwitchState(pObj->tUnit, tmsysFrontEndI2CSwitchStateReset);
        }
    }
    return err;
}

/*============================================================================*/
/* OM3869SSetFrequency                                                         */
/*============================================================================*/
static tmErrorCode_t
OM3869SSetFrequency
(
    tmUnitSelect_t          tUnit,          /* I: FrontEnd unit number */
    pVoid    pTuneRequest,    /* I/O: Tuner Tune Request Structure pointer */
    tmTuneReqType_t tTuneReqType,
    tmTDA182I2StandardMode_t  sysMode
)
{
    ptmOM3869SObject_t           pObj = Null;
    tmErrorCode_t               err = TM_OK;
    tmbslFrontEndState_t        eTunerPLLLock = tmbslFrontEndStateUnknown;

    err = OM3869SGetInstance(tUnit, &pObj);
	if (err != TM_OK)
		{
         mcSHOW_DBG_MSG(("\n OM3869GetInstance(0x%08X) failed!", tUnit));
	    }
	else
		{
         mcSHOW_DBG_MSG(("\n OM3869GetInstance(0x%08X) success!", tUnit));
		 mcSHOW_DBG_MSG(("\n\n==========================================================================="));
		 mcSHOW_DBG_MSG(("OM3869SetFrequency(0x%X) is called with following parameters:\n",pObj->tUnit));   
		 mcSHOW_DBG_MSG(( "     SysMode:           %d \n", sysMode));
		 mcSHOW_DBG_MSG(( "     TunerRegType:           %d \n", tTuneReqType));
		 mcSHOW_DBG_MSG(("\n\n==========================================================================="));
	    }

    //if(err == TM_OK && pObj->lastTuneReqType != TRT_TUNER_ONLY)
	if(err == TM_OK && pObj->lastTuneReqType != tTuneReqType)
    {
        mcSHOW_DBG_MSG2(("\n Mode change"));

        if(err == TM_OK)
        {
            err = tmbslTDA182I2SetPowerState(pObj->tUnitTDA182I2, tmTDA182I2_PowerNormalMode);
			if (err != TM_OK)
				{
				mcSHOW_DBG_MSG(("tmbslTDA182I2SetPowerState(0x%08X, PowerOn) failed\n", pObj->tUnitTDA182I2));

			    }
        }
        if(err == TM_OK)
        {
            pObj->powerState = tmPowerOn;
           // pObj->lastTuneReqType = TRT_TUNER_ONLY;
			pObj->lastTuneReqType = tTuneReqType;
        }
    }

    /************************************************************************/
    /* Program Tuner                                                        */
    /************************************************************************/

    if(err == TM_OK)
    {
        /* Set Tuner Standard mode */
        //err = tmbslTDA182I2SetStandardMode(pObj->tUnitTDA182I2, pTuneRequest->dwStandard);
        mcSHOW_DBG_MSG(("Sart Set SysMode\n"));
		err = tmbslTDA182I2SetStandardMode(pObj->tUnitTDA182I2, sysMode);
        if (err != TM_OK)
        {
            //tmDBGPRINTEx(DEBUGLVL_ERROR, "tmbslTDA182I2SetStandardMode(0x%08X, %d) failed.", pObj->tUnitTDA182I2, pTuneRequest->dwStandard);
			//tmDBGPRINTEx(DEBUGLVL_ERROR, "tmbslTDA182I2SetStandardMode(0x%08X, %d) failed.", pObj->tUnitTDA182I2, sysMode);
			mcSHOW_DBG_MSG(( "tmbslTDA182I2SetStandardMode %d failed. \n", sysMode));
        }
    }

    if(err == TM_OK)
    {
        /* Set Tuner RF */
		 mcSHOW_DBG_MSG(("Start Set RF frequence\n"));
		if (tTuneReqType == TRT_DVBT)
			{
               err = tmbslTDA182I2SetRf(pObj->tUnitTDA182I2, ((ptmDVBTRequest_t)pTuneRequest)->dwFrequency);
			   mcSHOW_DBG_MSG(( "   [DVBT] Set Frequency:     %d Hz\n", ((ptmDVBTRequest_t)pTuneRequest)->dwFrequency));
			}
		else if (tTuneReqType == TRT_ANALOG_TV)
			{
		       err = tmbslTDA182I2SetRf(pObj->tUnitTDA182I2, ((ptmAnalogTVRequest_t)pTuneRequest)->dwFrequency);
			   mcSHOW_DBG_MSG(( "   [Analog] Set Frequency:     %d Hz\n", ((ptmAnalogTVRequest_t)pTuneRequest)->dwFrequency));
			}
		else 
			{
		       err = tmbslTDA182I2SetRf(pObj->tUnitTDA182I2, ((ptmTunerOnlyRequest_t)pTuneRequest)->dwFrequency);
			   mcSHOW_DBG_MSG(( "   [TunerOnly] Set Frequency:     %d Hz\n", ((ptmTunerOnlyRequest_t)pTuneRequest)->dwFrequency));
			}
        if (err != TM_OK)
        {
            tmDBGPRINTEx(DEBUGLVL_ERROR, "tmbslTDA182I2SetRf(0x%08X, %d) failed.", pObj->tUnitTDA182I2, pTuneRequest->dwFrequency);
			mcSHOW_DBG_MSG(( "tmbslTDA182I2SetRf failed.\n"));
        }
    }
    
    if(err == TM_OK)
    {
        /* Get Tuner PLL Lock status */
        err = tmbslTDA182I2GetLockStatus(pObj->tUnitTDA182I2, &eTunerPLLLock);
        if (err != TM_OK)
        {
            tmDBGPRINTEx(DEBUGLVL_ERROR, "tmbslTDA182I2GetLockStatus(0x%08X) failed.", pObj->tUnitTDA182I2);
			mcSHOW_DBG_MSG(( "tmbslTDA182I2GetLockStatus(0x%08X) failed.\n", pObj->tUnitTDA182I2));
        }
    }

    if(err == TM_OK)
    {
        if (tTuneReqType == TRT_DVBT)
         ((ptmDVBTRequest_t)pTuneRequest)->eFrontEndLock = (tmsysFrontEndState_t)eTunerPLLLock;
		else if (tTuneReqType == TRT_ANALOG_TV)
		 ((ptmAnalogTVRequest_t)pTuneRequest)->eFrontEndHLock = (tmsysFrontEndState_t)eTunerPLLLock;
		else 
	     ((ptmTunerOnlyRequest_t)pTuneRequest)->eTunerLock = (tmsysFrontEndState_t)eTunerPLLLock;
    }

    /* Print the result of the Manager function */
    switch(eTunerPLLLock)
    {
    case tmbslFrontEndStateLocked:
        if(err == TM_OK)
        {
            mcSHOW_DBG_MSG(( "  OM3869SetFrequency FrontEndState:  Locked. (0x%08X)! \n", pObj->tUnit));
        }
        if(err == TM_OK)
        {
            tmDBGPRINTEx(DEBUGLVL_TERSE, "===========================================================================");
            tmDBGPRINTEx(DEBUGLVL_TERSE, " OM3869SSetFrequency(0x%08X) found following parameters:", pObj->tUnit);
            tmDBGPRINTEx(DEBUGLVL_TERSE, "===========================================================================");
            tmDBGPRINTEx(DEBUGLVL_TERSE, "     Frequency:      %d Hz", pTuneRequest->dwFrequency);
            tmDBGPRINTEx(DEBUGLVL_TERSE, "===========================================================================");
        }

        break;

    case tmbslFrontEndStateNotLocked:
        if(err == TM_OK)
        {
            mcSHOW_DBG_MSG(( "OM3869SetFrequency FrontEndState: UnLocked!!! (0x%08X) \n", pObj->tUnit));
        }
        break;

    case tmbslFrontEndStateSearching:
    default:
        if(err == TM_OK)
        {
            mcSHOW_DBG_MSG(( "OM3869SetFrequency FrontEndState: Locking TimeOut.... (0x%08X)! \n", pObj->tUnit));
        }
        break;
    }

    return err;
}

/*============================================================================*/
/* OM3869SMutexAcquire                                                      */
/*============================================================================*/
extern tmErrorCode_t
OM3869SMutexAcquire
(
    ptmOM3869SObject_t    pObj,
    UInt32                  timeOut
)
{
    tmErrorCode_t   err = TM_OK;

    if(pObj->sMutex.Acquire != Null && pObj->pMutex != Null)
    {
        err = pObj->sMutex.Acquire(pObj->pMutex, timeOut);
    }

    return err;
}

/*============================================================================*/
/* OM3869SMutexRelease                                                      */
/*============================================================================*/
extern tmErrorCode_t
OM3869SMutexRelease
(
    ptmOM3869SObject_t    pObj
)
{
    tmErrorCode_t   err = TM_OK;

    if(pObj->sMutex.Release != Null && pObj->pMutex != Null)
    {
        err = pObj->sMutex.Release(pObj->pMutex);
    }

    return err;
}

/*============================================================================*/
/* tmsysOM3869SWriteRegister                                                  */
/*============================================================================*/

tmErrorCode_t
tmsysOM3869SWriteRegister
(
    tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uSubAddress,    /* I: sub address */
    UInt8           uValue  /* I: Item value */
)
{
    ptmOM3869SObject_t    pObj = Null;
    tmErrorCode_t           err = TM_OK;

    if(err == TM_OK)
    {
        err = OM3869SGetInstance(tUnit, &pObj);
		if (err != TM_OK)
			{
             mcSHOW_DBG_MSG(("OM3869GetInstance(0x%08X) failed.\n", tUnit));
		    }
		else
			{

             mcSHOW_DBG_MSG(("OM3869GetInstance(0x%08X) success.\n", tUnit));
		    }
        
    }
    if(err == TM_OK)
    {
        err = OM3869SMutexAcquire(pObj, OM3869S_MUTEX_TIMEOUT);
    }
     err = tmbslTDA182I2WriteTuner_Register(tUnit, uSubAddress, uValue);
	 if(err != TM_OK)
	 	{
         mcSHOW_DBG_MSG(("tmbslTDA182I2WriteTuner_Register(0x%08X) failed.\n", uSubAddress));
	    }

	 (void)OM3869SMutexRelease(pObj);
	 return err;
}

/*============================================================================*/
/* tmsysOM3869SReadRegister                                                   */
/*============================================================================*/

tmErrorCode_t
tmsysOM3869SReadRegister
(
    tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uSubAddress,    /* I: sub address */
    UInt8*           puValue  /* I: Item value */
)
{
	   ptmOM3869SObject_t	 pObj = Null;
	   tmErrorCode_t		   err = TM_OK;
	
	   if(err == TM_OK)
	   {
		   err = OM3869SGetInstance(tUnit, &pObj);
		   if (err != TM_OK)
			   {
				mcSHOW_DBG_MSG(("OM3869GetInstance(0x%08X) failed.\n", tUnit));
			   }
		   else
			  {

                mcSHOW_DBG_MSG(("OM3869GetInstance(0x%08X) success.\n", tUnit));
		      }
	   }
	   if(err == TM_OK)
	   {
		   err = OM3869SMutexAcquire(pObj, OM3869S_MUTEX_TIMEOUT);
	   }
		err = tmbslTDA182I2ReadTuner_Register(tUnit, uSubAddress, puValue);
		if(err != TM_OK)
		   {
			mcSHOW_DBG_MSG(("tmbslTDA182I2ReadTuner_Register(0x%08X) failed.\n", uSubAddress));
		   }
	
		(void)OM3869SMutexRelease(pObj);

      return err;
}

