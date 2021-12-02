/*------------------------------------------------------------------------------

 Copyright 2010, 2011 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2011/01/28
 Revision: 1.2.1.0

------------------------------------------------------------------------------*/

#include "cxd2828_isdb_t_demod.h"
#include "cxd2828_isdb_t_demod_monitor.h"

/*------------------------------------------------------------------------------
 Prototypes of static functions
------------------------------------------------------------------------------*/
static cxd2828_result_t setDemodParam(cxd2828_isdb_t_demod_t* pDemod);
static cxd2828_result_t SLT2EWS(cxd2828_isdb_t_demod_t* pDemod);
static cxd2828_result_t EWS2SLT(cxd2828_isdb_t_demod_t* pDemod);
static cxd2828_result_t SLT2NOT(cxd2828_isdb_t_demod_t* pDemod);
static cxd2828_result_t NOT2SLT(cxd2828_isdb_t_demod_t* pDemod);
static cxd2828_result_t NOT2EWS(cxd2828_isdb_t_demod_t* pDemod);
static cxd2828_result_t EWS2NOT(cxd2828_isdb_t_demod_t* pDemod);
static cxd2828_result_t T_tune(cxd2828_isdb_t_demod_t* pDemod);

/*------------------------------------------------------------------------------
 Functions
------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------
  Initialize ISDB-T part

  Initialize ISDB-T part.
  This function is called from upper layer in this driver.
  So user don't need to call this function.

  pDemod : Address of ISDB-T part instance.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_Initialize(cxd2828_isdb_t_demod_t* pDemod)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_Initialize");

    /* Null check */
    if(!pDemod){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    switch(pDemod->state)
    {
    case CXD2828_ISDB_T_DEMOD_STATE_UNKNOWN:
    case CXD2828_ISDB_T_DEMOD_STATE_SHUTDOWN:
    case CXD2828_ISDB_T_DEMOD_STATE_INVALID:
    default:
        result = CXD2828_RESULT_ERROR_SW_STATE;
        break;
    case CXD2828_ISDB_T_DEMOD_STATE_NORMAL_T:
    case CXD2828_ISDB_T_DEMOD_STATE_EWS:
        result = cxd2828_isdb_t_demod_Sleep(pDemod);
        break;
    case CXD2828_ISDB_T_DEMOD_STATE_PRE_INIT:
        /* For reset monitor registers */
        result = cxd2828_isdb_t_demod_SoftReset(pDemod);
        pDemod->state = CXD2828_ISDB_T_DEMOD_STATE_INIT;
        break;
    case CXD2828_ISDB_T_DEMOD_STATE_INIT:
        /* do nothing */
        break;
    }

    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  ISDB-T tuning

  Set tuning parameter to ISDB-T demod part.
  Before calling this function, please call cxd2828_isdb_t_demod_SetPreset
  and set preset information.

  pDemod : Address of ISDB-T part instance.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_Tune(cxd2828_isdb_t_demod_t* pDemod)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_Tune");

    /* Null check */
    if(!pDemod){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    switch(pDemod->state)
    {
    case CXD2828_ISDB_T_DEMOD_STATE_UNKNOWN:
    case CXD2828_ISDB_T_DEMOD_STATE_SHUTDOWN:
    case CXD2828_ISDB_T_DEMOD_STATE_PRE_INIT:
    case CXD2828_ISDB_T_DEMOD_STATE_INVALID:
    default:
        result = CXD2828_RESULT_ERROR_SW_STATE;
        break;
    case CXD2828_ISDB_T_DEMOD_STATE_INIT:
        result = SLT2NOT(pDemod);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        pDemod->state = CXD2828_ISDB_T_DEMOD_STATE_NORMAL_T;
        break;
    case CXD2828_ISDB_T_DEMOD_STATE_NORMAL_T:
        result = T_tune(pDemod);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        pDemod->state = CXD2828_ISDB_T_DEMOD_STATE_NORMAL_T;
        break;
    case CXD2828_ISDB_T_DEMOD_STATE_EWS:
        result = EWS2NOT(pDemod);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        pDemod->state = CXD2828_ISDB_T_DEMOD_STATE_NORMAL_T;
        break;
    }

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  ISDB-T EWS tuning

  Set EWS tuning parameter to ISDB-T demod part.
  Before calling this function, please call cxd2828_isdb_t_demod_SetPreset
  and set preset information.

  pDemod : Address of ISDB-T part instance.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_EWSTune(cxd2828_isdb_t_demod_t* pDemod)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_EWSTune");

    /* Null check */
    if(!pDemod){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    switch(pDemod->state)
    {
    case CXD2828_ISDB_T_DEMOD_STATE_UNKNOWN:
    case CXD2828_ISDB_T_DEMOD_STATE_SHUTDOWN:
    case CXD2828_ISDB_T_DEMOD_STATE_PRE_INIT:
    case CXD2828_ISDB_T_DEMOD_STATE_INVALID:
    default:
        result = CXD2828_RESULT_ERROR_SW_STATE;
        break;
    case CXD2828_ISDB_T_DEMOD_STATE_INIT:
        result = SLT2EWS(pDemod);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        pDemod->state = CXD2828_ISDB_T_DEMOD_STATE_EWS;
        break;
    case CXD2828_ISDB_T_DEMOD_STATE_NORMAL_T:
        result = NOT2EWS(pDemod);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        pDemod->state = CXD2828_ISDB_T_DEMOD_STATE_EWS;
        break;
    case CXD2828_ISDB_T_DEMOD_STATE_EWS:
        result = T_tune(pDemod);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        pDemod->state = CXD2828_ISDB_T_DEMOD_STATE_EWS;
        break;
    }

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  ISDB-T sleep

  Change hardware state to "Sleep".

  pDemod : Address of ISDB-T part instance.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_Sleep(cxd2828_isdb_t_demod_t* pDemod)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_Sleep");

    /* Null check */
    if(!pDemod){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    switch(pDemod->state)
    {
    case CXD2828_ISDB_T_DEMOD_STATE_UNKNOWN:
    case CXD2828_ISDB_T_DEMOD_STATE_SHUTDOWN:
    case CXD2828_ISDB_T_DEMOD_STATE_PRE_INIT:
    case CXD2828_ISDB_T_DEMOD_STATE_INVALID:
    default:
        result = CXD2828_RESULT_ERROR_SW_STATE;
        break;
    case CXD2828_ISDB_T_DEMOD_STATE_INIT:
        /* do nothing */
        break;
    case CXD2828_ISDB_T_DEMOD_STATE_NORMAL_T:
        result = NOT2SLT(pDemod);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        pDemod->state = CXD2828_ISDB_T_DEMOD_STATE_INIT;
        break;
    case CXD2828_ISDB_T_DEMOD_STATE_EWS:
        result = EWS2SLT(pDemod);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        pDemod->state = CXD2828_ISDB_T_DEMOD_STATE_INIT;
        break;
    }

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  Set preset information

  Set preset information to shorten tuning time.
  It must call this function before tuning sequence.

  pDemod     : Address of ISDB-T part instance.
  presetInfo : Address of preset information.
               If it set "Null" preset informaiton is disable.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_SetPreset(cxd2828_isdb_t_demod_t* pDemod,
                                          cxd2828_isdb_t_demod_preset_info_t* presetInfo)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_SetPreset");

    /* Null check */
    if(!pDemod || !pDemod->pI2c || !pDemod->pI2c->WriteOneRegister){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    if(presetInfo) {
        /* Preset On */
        result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0xCB, 0x01);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0xCC, 0x01);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        result = pDemod->pI2c->WriteRegister(pDemod->pI2c, pDemod->i2cAddress, 0xED, presetInfo->data, CXD2828_ISDB_T_DEMOD_PRESET_INFO_BYTES);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
    } else {
        /* Preset Off */
        result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0xCB, 0x00);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0xCC, 0x00);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
    }

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  Soft reset

  Soft reset to ISDB-T part.

  pDemod     : Address of ISDB-T part instance.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_SoftReset(cxd2828_isdb_t_demod_t* pDemod)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_SoftReset");

    /* Null check */
    if(!pDemod || !pDemod->pI2c || !pDemod->pI2c->WriteOneRegister){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0xFE, 0x01);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  Check demod lock status of ISDB-T demod part

  pDemod      : Address of ISDB-T part instance.
  pLockStatus : lock status of ISDB-T demod part.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_CheckDemodLock(cxd2828_isdb_t_demod_t* pDemod,
                                               cxd2828_isdb_t_demod_lock_status_t* pLockStatus)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;
    UINT8 dmdLock = 0;
    UINT8 tsLock = 0;
    UINT8 unlock = 0;

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_CheckDemodLock");

    /* Null check */
    if(!pDemod || !pDemod->pI2c || !pLockStatus){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    result = cxd2828_isdb_t_demod_MonitorSyncStat(pDemod, &dmdLock, &tsLock, &unlock);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    if(unlock){
        *pLockStatus = CXD2828_ISDB_T_DEMOD_LOCK_STATUS_UNLOCK;
    } else if(dmdLock) {
        *pLockStatus = CXD2828_ISDB_T_DEMOD_LOCK_STATUS_LOCK;
    } else {
        *pLockStatus = CXD2828_ISDB_T_DEMOD_LOCK_STATUS_NOT_DETECT;
    }

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  Check TS lock status of ISDB-T demod part

  Return the result of TS lock flag.

  pDemod      : Address of ISDB-T part instance.
  pLockStatus : lock status of ISDB-T demod part.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_CheckTSLock(cxd2828_isdb_t_demod_t* pDemod,
                                            cxd2828_isdb_t_demod_lock_status_t* pLockStatus)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;
    UINT8 dmdLock = 0;
    UINT8 tsLock = 0;
    UINT8 unlock = 0;

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_CheckTSLock");

    /* Null check */
    if(!pDemod || !pDemod->pI2c || !pLockStatus){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    result = cxd2828_isdb_t_demod_MonitorSyncStat(pDemod, &dmdLock, &tsLock, &unlock);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    if(unlock){
        *pLockStatus = CXD2828_ISDB_T_DEMOD_LOCK_STATUS_UNLOCK;
    } else if(tsLock) {
        *pLockStatus = CXD2828_ISDB_T_DEMOD_LOCK_STATUS_LOCK;
    } else {
        *pLockStatus = CXD2828_ISDB_T_DEMOD_LOCK_STATUS_NOT_DETECT;
    }

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  Check false unlock and retry if it is false unlock.

  pDemod         : Address of ISDB-T part instance.
  pIsFalseUnlock : 0:Correct unlock, 1:False unlock, please retry lock wait sequence.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_CheckFalseUnlock(cxd2828_isdb_t_demod_t* pDemod, UINT8* pIsFalseUnlock)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;
    UINT8 data[3];
    UINT8 mod;
    UINT8 gi;
    INT32 dft0;
    INT32 dft1;

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_CheckFalseUnlock");

    /* Null check */
    if(!pDemod || !pDemod->pI2c || !pIsFalseUnlock){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
    result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0x14, data, 1);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    mod = (UINT8)((data[0] >> 4) & 0x03);
    gi = (UINT8)(data[0] & 0x03);

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x0e);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
    result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0x39, data, 3);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
    dft0 = (INT32)(((UINT32)(data[0] & 0x03) << 16) | (UINT32)((data[1] & 0xFF) << 8) | (UINT32)(data[2] & 0xFF));
    result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0x51, data, 3);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
    dft1 = (INT32)(((UINT32)(data[0] & 0x03) << 16) | (UINT32)((data[1] & 0xFF) << 8) | (UINT32)(data[2] & 0xFF));

    if((mod == 3) && (gi == 1) && ((dft0 - dft1) < 163840)){
        result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0xbc, data, 1);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        data[0] = ~data[0];
        result = cxd2828_i2c_SetRegisterBits(pDemod->pI2c, pDemod->i2cAddress, 0xbc, data[0], 0x10);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        result = cxd2828_isdb_t_demod_SoftReset(pDemod);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        *pIsFalseUnlock = 1;
    } else {
        *pIsFalseUnlock = 0;
    }

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  GPIO setting to ISDB-T demod part

  Set each GPIO to "Enable/Disable" and "Read/Write".
  Read  : Read the data input from GPIO pin.
  Write : Output the data that set by register from GPIO pin.

  CXD2826R, CXD2827R
    It is necessary to select the terget GPIO to ISDB-T
    by cxd2828_isdb_cxd2827_GPIOSetting before use this function.

                          +------+
                    +-----|ISDB-T|
                    |     +------+
                 +----+
    GPIOx-pin----|    | <- It must be set to "ISDB-T"
                 +----+
                    |     +------+
                    +-----|ISDB-S|
                          +------+
  CXD2828R
    There is no restriction.

  pDemod   : Address of ISDB-T part instance.
  id       : GPIO ID (0, 1, 2)
  isEnable : 0: disable.
             1: enable.
  isWrite  : 0: use for GPO.
             1: use for GPI.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_GPIOSetting(cxd2828_isdb_t_demod_t* pDemod,
                                            UINT8 id,
                                            UINT8 isEnable,
                                            UINT8 isWrite)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;
    UINT8 data = 0;
    UINT8 mask = 0;

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_GPIOSetting");

    /* Null check */
    if(!pDemod || !pDemod->pI2c || !pDemod->pI2c->WriteOneRegister){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    if(id >= 3){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    if(isEnable){
        /* Enable */
        data = (UINT8)(1 << id);
        mask = (UINT8)(1 << id);
        result = cxd2828_i2c_SetRegisterBits(pDemod->pI2c, pDemod->i2cAddress, 0x21, data, mask);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        if(isWrite){
            /* GPO */
            data = 0x00;
            mask = (UINT8)(1 << id);
        } else {
            /* GPI */
            data = (UINT8)(1 << id);
            mask = (UINT8)(1 << id);
        }
        result = cxd2828_i2c_SetRegisterBits(pDemod->pI2c, pDemod->i2cAddress, 0x1C, data, mask);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
    } else {
        /* Disable */
        data = 0x00;
        mask = (UINT8)(1 << id);
        result = cxd2828_i2c_SetRegisterBits(pDemod->pI2c, pDemod->i2cAddress, 0x21, data, mask);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
    }

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  Read GPIO value

  Read the data input from GPIO pin.
  For reading GPIO data by this function, user must set to the target
  GPIO to "Enable" and "Read"(by cxd2828_isdb_t_demod_GPIOSetting).

  pDemod : Address of ISDB-T part instance.
  id     : GPIO ID (0, 1, 2)
  pValue : Address of the read result.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_GPIORead(cxd2828_isdb_t_demod_t* pDemod,
                                         UINT8 id,
                                         UINT8* pValue)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;
    UINT8 data = 0;

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_GPIORead");

    /* Null check */
    if(!pDemod || !pDemod->pI2c || !pDemod->pI2c->WriteOneRegister || !pValue){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    /* Parameter range check */
    if(id >= 3) {
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->ReadRegister(pDemod->pI2c, pDemod->i2cAddress, 0x1B, &data, 1);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    *pValue = (data >> id) & 0x01;

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  Write GPIO value

  Write the data output from GPIO pin.
  For writing GPIO data by this function, user must set to the target
  GPIO to "Enable" and "Write"(by cxd2828_isdb_t_demod_GPIOSetting).

  pDemod : Address of ISDB-T part instance.
  id     : GPIO ID (0, 1, 2)
  value  : The value of data output by GPIO pin.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_GPIOWrite(cxd2828_isdb_t_demod_t* pDemod,
                                          UINT8 id,
                                          UINT8 value)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;
    UINT8 data = 0;
    UINT8 mask = 0;

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_GPIOWrite");

    /* Null check */
    if(!pDemod || !pDemod->pI2c || !pDemod->pI2c->WriteOneRegister){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    /* Parameter range check */
    if(id >= 3) {
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }
    if(value > 1){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    data = (UINT8)((value & 0x01) << (id + 4));
    mask = (UINT8)(1 << (id + 4));

    result = cxd2828_i2c_SetRegisterBits(pDemod->pI2c, pDemod->i2cAddress, 0x1C, data, mask);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  Set parameters for ISDB-T demod part

  Set each parameters for ISDB-T demod part.

  pDemod : Address of ISDB-T part instance.
  config : Config parmeters type.
  value  : Setting value.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_SetConfig(cxd2828_isdb_t_demod_t* pDemod,
                                          cxd2828_isdb_t_demod_config_t config,
                                          UINT32 value)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;
    UINT8 data[3] = {0,0,0};

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_demod_SetConfig");

    /* Null check */
    if(!pDemod || !pDemod->pI2c || !pDemod->pI2c->WriteOneRegister){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    switch(config){
    case CXD2828_ISDB_T_DEMOD_CONFIG_OWFMT_CKINV:
        switch(value){
        case 0:
            /* Falling edge */
            data[0] = 0x00;
            break;
        case 1:
            /* Rising edge */
            data[0] = 0x02;
            break;
        default:
            CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
        }
        result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        result = cxd2828_i2c_SetRegisterBits(pDemod->pI2c, pDemod->i2cAddress, 0xA0, data[0], 0x02);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        break;
    case CXD2828_ISDB_T_DEMOD_CONFIG_OCTL_IFAGCNEG:
        switch(value){
        case 0:
            /* Positive */
            data[0] = 0x00;
            break;
        case 1:
            /* Negative */
            data[0] = 0x40;
            break;
        default:
            CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
        }
        result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        result = cxd2828_i2c_SetRegisterBits(pDemod->pI2c, pDemod->i2cAddress, 0xE6, data[0], 0x40);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        break;
    case CXD2828_ISDB_T_DEMOD_CONFIG_OCTL_ITBINVERT:
        switch(value){
        case 0:
            /* Do not invert input spectrum */
            data[0] = 0x00;
            break;
        case 1:
            /* Invert input spectrum */
            data[0] = 0x10;
            break;
        default:
            CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
        }
        result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        result = cxd2828_i2c_SetRegisterBits(pDemod->pI2c, pDemod->i2cAddress, 0xBC, data[0], 0x10);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        break;
    case CXD2828_ISDB_T_DEMOD_CONFIG_OREG_SYR_JPOG:
        switch(value){
        case 0:
            /* Full ISDB-T support */
            data[0] = 0x00;
            break;
        case 1:
            /* Japan mode */
            data[0] = 0x03;
            break;
        default:
            CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
        }
        result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        result = cxd2828_i2c_SetRegisterBits(pDemod->pI2c, pDemod->i2cAddress, 0x67, data[0], 0x03);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        break;
    case CXD2828_ISDB_T_DEMOD_CONFIG_OREG_CCOD_SINV_SEARCH_ON:
        switch(value){
        case 0:
            /* Disable spectrum detection and automatic correction */
            data[0] = 0x00;
            break;
        case 1:
            /* Enable spectrum detection and automatic correction */
            data[0] = 0x01;
            break;
        default:
            CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
        }
        result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        result = cxd2828_i2c_SetRegisterBits(pDemod->pI2c, pDemod->i2cAddress, 0x90, data[0], 0x01);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        break;
    case CXD2828_ISDB_T_DEMOD_CONFIG_OTDA_DNCNV_FREQ:
        if(value > 0xFFFFFF){
            CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
        }
        data[0] = (UINT8)((value >> 16) & 0xFF);
        data[1] = (UINT8)((value >>  8) & 0xFF);
        data[2] = (UINT8) (value        & 0xFF);
        result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        result = pDemod->pI2c->WriteRegister(pDemod->pI2c, pDemod->i2cAddress, 0xBD, data, 3);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        break;
    case CXD2828_ISDB_T_DEMOD_CONFIG_OADCGAIN:
        switch(value){
        case 0:
            /* 0.7Vpp */
            result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
            if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
            result = cxd2828_i2c_SetRegisterBits(pDemod->pI2c, pDemod->i2cAddress, 0xD7, 0x00, 0x60);
            if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
            break;
        case 1:
            /* 1.0Vpp */
            result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
            if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
            result = cxd2828_i2c_SetRegisterBits(pDemod->pI2c, pDemod->i2cAddress, 0xD7, 0x20, 0x60);
            if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
            break;
        case 2:
            /* 1.4Vpp */
            result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
            if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
            result = cxd2828_i2c_SetRegisterBits(pDemod->pI2c, pDemod->i2cAddress, 0xD7, 0x40, 0x60);
            if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
            break;
        default:
            CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
        }
        break;
    default:
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    CXD2828_TRACE_RETURN(result);
}

/*------------------------------------------------------------------------------
  static functions
------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------
  Set the parameters of demodulator

  pDemod   : Address of ISDB-T part instance.
--------------------------------------------------------------------*/
static cxd2828_result_t setDemodParam(cxd2828_isdb_t_demod_t* pDemod)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_ENTER("setDemodParam");

    /* Null check */
    if(!pDemod){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x27, 0x05);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0xBB, 0x1F);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0xDA, 0x04);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x26, 0x01);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x01);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x67, 0x09);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x68, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  SLS2EWS

  pDemod   : Address of ISDB-T part instance.
--------------------------------------------------------------------*/
static cxd2828_result_t SLT2EWS(cxd2828_isdb_t_demod_t* pDemod)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_ENTER("SLT2EWS");

    /* Null check */
    if(!pDemod || !pDemod->pI2c || !pDemod->pI2c->WriteOneRegister){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x1D, 0x47);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = setDemodParam(pDemod);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0xFC, 0x82);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = cxd2828_isdb_t_demod_SoftReset(pDemod);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  EWS2SLT

  pDemod   : Address of ISDB-T part instance.
--------------------------------------------------------------------*/
static cxd2828_result_t EWS2SLT(cxd2828_isdb_t_demod_t* pDemod)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_ENTER("EWS2SLT");

    /* Null check */
    if(!pDemod || !pDemod->pI2c || !pDemod->pI2c->WriteOneRegister){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0xFC, 0x81);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x1D, 0x7F);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  SLT2NOT

  pDemod   : Address of ISDB-T part instance.
--------------------------------------------------------------------*/
static cxd2828_result_t SLT2NOT(cxd2828_isdb_t_demod_t* pDemod)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_ENTER("SLT2NOT");

    /* Null check */
    if(!pDemod || !pDemod->pI2c || !pDemod->pI2c->WriteOneRegister){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x1D, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x1E, pDemod->OTSDT_HIZ);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = setDemodParam(pDemod);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0xFC, 0x83);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = cxd2828_isdb_t_demod_SoftReset(pDemod);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  NOT2SLT

  pDemod   : Address of ISDB-T part instance.
--------------------------------------------------------------------*/
static cxd2828_result_t NOT2SLT(cxd2828_isdb_t_demod_t* pDemod)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_ENTER("NOT2SLT");

    /* Null check */
    if(!pDemod || !pDemod->pI2c || !pDemod->pI2c->WriteOneRegister){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0xFC, 0x81);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x1D, 0x7F);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x1E, 0xFF);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  NOT2EWS

  pDemod   : Address of ISDB-T part instance.
--------------------------------------------------------------------*/
static cxd2828_result_t NOT2EWS(cxd2828_isdb_t_demod_t* pDemod)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_ENTER("NOT2EWS");

    /* Null check */
    if(!pDemod || !pDemod->pI2c || !pDemod->pI2c->WriteOneRegister){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0xFC, 0x82);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = cxd2828_isdb_t_demod_SoftReset(pDemod);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x1D, 0x47);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x1E, 0xFF);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  EWS2NOT

  pDemod   : Address of ISDB-T part instance.
--------------------------------------------------------------------*/
static cxd2828_result_t EWS2NOT(cxd2828_isdb_t_demod_t* pDemod)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_ENTER("EWS2NOT");

    /* Null check */
    if(!pDemod || !pDemod->pI2c || !pDemod->pI2c->WriteOneRegister){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x1D, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x1E, pDemod->OTSDT_HIZ);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = setDemodParam(pDemod);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = pDemod->pI2c->WriteOneRegister(pDemod->pI2c, pDemod->i2cAddress, 0xFC, 0x83);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = cxd2828_isdb_t_demod_SoftReset(pDemod);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  T_tune

  pDemod   : Address of ISDB-T part instance.
--------------------------------------------------------------------*/
static cxd2828_result_t T_tune(cxd2828_isdb_t_demod_t* pDemod)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_ENTER("T_tune");

    /* Null check */
    if(!pDemod || !pDemod->pI2c || !pDemod->pI2c->WriteOneRegister){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    result = setDemodParam(pDemod);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    result = cxd2828_isdb_t_demod_SoftReset(pDemod);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    CXD2828_TRACE_RETURN(result);
}
