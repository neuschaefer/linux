/**
Copyright (C) 2008 NXP B.V., All Rights Reserved.
This source code and any compilation or derivative thereof is the proprietary
information of NXP B.V. and is confidential in nature. Under no circumstances
is this software to be  exposed to or placed under an Open Source License of
any type without the expressed written permission of NXP B.V.
*
* \file          tmbslTDA182I2local.h
*                %version: 7 %
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
|            |           |       |
-------------|-----------|-------|-------|-----------------------------------
|            |           |       |
-------------|-----------|-------|-------|-----------------------------------
\endverbatim
*
*/

#ifndef _TMBSL_TDA182I2LOCAL_H 
#define _TMBSL_TDA182I2LOCAL_H

#ifndef TMBSL_TDA18272
#define TMBSL_TDA18272
#endif
/*------------------------------------------------------------------------------*/
/* Standard include files:                                                      */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/* Project include files:                                                       */
/*------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C"
{
#endif

/*------------------------------------------------------------------------------*/
/* Types and defines:                                                           */
/*------------------------------------------------------------------------------*/

#define TDA182I2_MUTEX_TIMEOUT  TMBSL_FRONTEND_MUTEX_TIMEOUT_INFINITE

#ifdef TMBSL_TDA18272
 #define TMBSL_TDA182I2_COMPONENT_NAME_STR "TDA18272"
#else /* TMBSL_TDA18272 */
 #define TMBSL_TDA182I2_COMPONENT_NAME_STR "TDA18212"
#endif /* TMBSL_TDA18272 */

#define _SYSTEMFUNC (pObj->SystemFunc)
#define POBJ_SRVFUNC_SIO pObj->sRWFunc
#define POBJ_SRVFUNC_STIME pObj->sTime
#define P_DBGPRINTEx pObj->sDebug.Print
#define P_DBGPRINTVALID ((pObj != Null) && (pObj->sDebug.Print != Null))


/*-------------*/
/* ERROR CODES */
/*-------------*/

#define TDA182I2_MAX_UNITS                          2

    typedef struct _tmTDA182I2Object_t
    {
        tmUnitSelect_t              tUnit;
        tmUnitSelect_t              tUnitW;
        ptmbslFrontEndMutexHandle   pMutex;
        Bool                        init;
        tmbslFrontEndIoFunc_t       sRWFunc;
        tmbslFrontEndTimeFunc_t     sTime;
        tmbslFrontEndDebugFunc_t    sDebug;
        tmbslFrontEndMutexFunc_t    sMutex;
        tmTDA182I2PowerState_t      curPowerState;
        UInt32                      uRF;
        tmTDA182I2StandardMode_t    StandardMode;
        tmTDA182I2StdCoefficients   Std_Array[tmTDA182I2_StandardMode_Max];
    } tmTDA182I2Object_t, *ptmTDA182I2Object_t, **pptmTDA182I2Object_t;


 tmErrorCode_t TDA182I2Init(tmUnitSelect_t tUnit);
// tmErrorCode_t TDA182I2Wait(ptmTDA182I2Object_t pObj, UInt32 Time);

extern tmErrorCode_t TDA182I2MutexAcquire(ptmTDA182I2Object_t   pObj, UInt32 timeOut);
extern tmErrorCode_t TDA182I2MutexRelease(ptmTDA182I2Object_t   pObj);

#ifdef __cplusplus
}
#endif

#endif /* _TMBSL_TDA182I2LOCAL_H */
