/**
Copyright (C) 2008 NXP B.V., All Rights Reserved.
This source code and any compilation or derivative thereof is the proprietary
information of NXP B.V. and is confidential in nature. Under no circumstances
is this software to be  exposed to or placed under an Open Source License of
any type without the expressed written permission of NXP B.V.
*
* \file          tmsysOM3869Slocal.h
*                %version: 8 %
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
9-JULY-2007  | A.TANT    |       |       | CREATION OF OM3869S ARCHITECTURE 2.0.0
-------------|-----------|-------|-------|-----------------------------------
|           |       |       |
-------------|-----------|-------|-------|-----------------------------------
\endverbatim
*
*/


#ifndef TMSYSOM3869SLOCAL_H
#define TMSYSOM3869SLOCAL_H

/*============================================================================*/
/*                       INCLUDE FILES                                        */
/*============================================================================*/


#ifdef __cplusplus
extern "C" {
#endif


    /*============================================================================*/
    /*                       MACRO DEFINITION                                     */
    /*============================================================================*/

#define OM3869S_SYS_COMP_NUM         1
#define OM3869S_SYS_MAJOR_VER        3
#define OM3869S_SYS_MINOR_VER        2

#define OM3869S_MUTEX_TIMEOUT       TMBSL_FRONTEND_MUTEX_TIMEOUT_INFINITE

#define POBJ_SRVFUNC_SIO pObj->sRWFunc
#define POBJ_SRVFUNC_STIME pObj->sTime
#define P_DBGPRINTEx tmsysOM3869SPrint
#define P_DBGPRINTVALID (True)

/*-------------*/
/* ERROR CODES */
/*-------------*/

/*---------*/
/* CHANNEL */
/*---------*/

//#define OM3869S_MAX_UNITS            2
#define OM3869S_MAX_UNITS            1



    /*-----------------------------------------------*/
    /*    DEFAULT VALUES FOR CONFIGURATION MANAGEMENT  */
    /*-----------------------------------------------*/

    /*---------*/
    /*  INDEX  */
    /*---------*/


    /*------------------*/
    /*special config    */
    /*------------------*/

    /*------------------*/
    /*  DEFAULT VALUES*/
    /*------------------*/

    /*----------------*/
    /*  DEFINE MASKS*/
    /*----------------*/


    /*---------------*/
    /*  DEFINE BITS  */
    /*---------------*/
    /*Special Config*/

    /*============================================================================*/
    /*                       ENUM OR TYPE DEFINITION                              */
    /*============================================================================*/

    typedef struct _tmOM3869SObject_t
    {
        tmUnitSelect_t              tUnit;
        tmUnitSelect_t              tUnitW;
        tmUnitSelect_t              tUnitTDA182I2;
        ptmbslFrontEndMutexHandle   pMutex;
        Bool                        init;
        tmbslFrontEndIoFunc_t       sRWFunc;
        tmbslFrontEndTimeFunc_t     sTime;
        tmbslFrontEndDebugFunc_t    sDebug;
        tmbslFrontEndMutexFunc_t    sMutex;
        tmPowerState_t              powerState;
        Bool                        resetDone;
        UInt32                      uHwAddress[tmOM3869SUnitDeviceTypeMax];
        tmTuneReqType_t             lastTuneReqType;
    } tmOM3869SObject_t, *ptmOM3869SObject_t,**pptmOM3869SObject_t;



    /*============================================================================*/
    /*                       EXTERN DATA DEFINITION                               */
    /*============================================================================*/

    /*============================================================================*/
    /*                       EXTERN FUNCTION PROTOTYPES                           */
    /*============================================================================*/
     tmErrorCode_t OM3869SInit(tmUnitSelect_t tUnit);
     tmErrorCode_t OM3869SReset(tmUnitSelect_t tUnit);
     tmErrorCode_t OM3869SSetFrequency(tmUnitSelect_t   tUnit, pVoid    pTuneRequest,  tmTuneReqType_t tTuneReqType,
    tmTDA182I2StandardMode_t  sysMode);

    extern tmErrorCode_t OM3869SMutexAcquire(ptmOM3869SObject_t   pObj, UInt32 timeOut);
    extern tmErrorCode_t OM3869SMutexRelease(ptmOM3869SObject_t   pObj);

#ifdef __cplusplus
}
#endif

#endif /* TMSYSOM3869SLOCAL_H */
/*============================================================================*/
/*                            END OF FILE                                     */
/*============================================================================*/

