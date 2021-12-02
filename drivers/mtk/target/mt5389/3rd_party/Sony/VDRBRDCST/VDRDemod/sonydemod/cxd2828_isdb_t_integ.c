/*------------------------------------------------------------------------------

 Copyright 2010 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2010/07/23
 Revision: 1.1.1.0

------------------------------------------------------------------------------*/

#include "cxd2828_isdb_t_integ.h"

/*------------------------------------------------------------------------------
 Internal defines
------------------------------------------------------------------------------*/

#define CXD2828_ISDB_T_INTEG_TUNE_TIMEOUT              2000

/* Time in ms between each read of the demod status to determine demod lock condition. */
#define CXD2828_ISDB_T_INTEG_TUNE_LOCK_CHECK_PERIOD    100

/*------------------------------------------------------------------------------
 Prototypes of static functions
------------------------------------------------------------------------------*/
static cxd2828_result_t Tune(cxd2828_isdb_t_integ_t* pInteg, UINT8 isScan);

/*------------------------------------------------------------------------------
 Functions
------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------
  Tune sequence

  This function control ISDB-T demodulator and ISDB-T tuner.
  To use this function, it is necessary to set following parameters in pInteg.
    - pDemod    : Address of ISDB-T demod part instance.
    - pTuner    : Address of ISDB-T tuner part instance.
    - frequency : Frequency to tune (KHz).
    - isPreset  : User preset information or not(0:Don't use, 1:Use)
    - presetInfo: If isPreset == 1, it must be set this parameter.
    - isEWS     : Tuning type(0:Normal tune, 1:EWS tune)

  pInteg     : Address of ISDB-T integration part instance.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_integ_Tune(cxd2828_isdb_t_integ_t* pInteg)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;
    CXD2828_TRACE_ENTER("cxd2828_isdb_t_integ_Tune");

    /* Clear cancel flag */
    pInteg->cancelFlag = 0;

    result = Tune(pInteg, 0);
    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  Scan sequence

  This function is a sample to scan sequence.
  This function control ISDB-T demodulator and ISDB-T tuner.
  To use this function, it is necessary to set following parameters in pInteg.
    - pDemod    : Address of ISDB-T demod part instance.
    - pTuner    : Address of ISDB-T tuner part instance.
    - frequency : Start frequency to scan (KHz).
  This function continue to tune each frequency and check lock.
  If it locked, call callback function at "Lock event".
  If it didn't lock, call callback function at "Unlock event".
  You can confirm eventId that is member of argument of callback function.
  This function is simple implementation for sample.
  If you need, following implementation is necessary.
  - In callback information
    - Get preset information and store it.(For shorten tuning time)
    - Get carrier frequency offset and revice the frequency found by scan.

  Start : frequency
  Step  : scanStepFrequency
  End   : scanEndFrequency

  1.         2.         3.         4... Y          Z(last)
  +----------------------------------------------------------------
  |<--Step-->|<--Step-->|<--Step-->|... |<--Step-->|
  Start                                            End

  pInteg           : Address of ISDB-T integration part instance.
  scanEndFrequency : The max frequency for scan (KHz)
  scanStepFrequency: The step frequency for scan (KHz)
  pCallback        : Callback function pointer.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_integ_Scan(cxd2828_isdb_t_integ_t* pInteg,
                                     UINT32 scanEndFrequency,
                                     UINT32 scanStepFrequency,
                                     void (*pCallback)(cxd2828_isdb_t_integ_t*))
{
    cxd2828_result_t result = CXD2828_RESULT_OK;
    UINT32 freqCurrent = 0;

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_integ_Scan");

    /* Null check */
    if(!pInteg || !pInteg->pDemod || !pInteg->pDemod->pI2c || !pInteg->pTuner || !pInteg->pTuner->pI2c || !pCallback){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    /* Frequency check */
    if((pInteg->frequency > scanEndFrequency) || (scanStepFrequency == 0)){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    /* Clear cancel flag */
    pInteg->cancelFlag = 0;

    pInteg->isEWS = 0;
    pInteg->isPreset = 0;

    /* Set first frequency */
    freqCurrent = pInteg->frequency;

    /* Scan loop */
    while(freqCurrent <= scanEndFrequency){
        /* Check cancel flag */
        if(pInteg->cancelFlag){
            CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_CANCEL);
        }

        pInteg->frequency = freqCurrent;

        result = Tune(pInteg, 1);
        switch(result)
        {
        case CXD2828_RESULT_OK:
            /* Lock */
            pInteg->eventId = CXD2828_ISDB_T_INTEG_EVENT_ID_LOCK;
            (*pCallback)(pInteg);
            break;
        case CXD2828_RESULT_ERROR_UNLOCK:
        case CXD2828_RESULT_ERROR_TIMEOUT:
            /* Unlock */
            pInteg->eventId = CXD2828_ISDB_T_INTEG_EVENT_ID_UNLOCK;
            (*pCallback)(pInteg);
            break;
        default:
            return result; /* Error return */
        }
        freqCurrent += scanStepFrequency;
    }
    result = CXD2828_RESULT_OK;

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  Cancel sequence

  This function stop the sequence cxd2828_isdb_t_integ_Tune or
  cxd2828_isdb_t_integ_Scan by calling from other thread.

  pInteg     : Address of ISDB-T integration part instance.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_integ_Cancel(cxd2828_isdb_t_integ_t* pInteg)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_ENTER("cxd2828_isdb_t_integ_Cancel");

    /* Null check */
    if(!pInteg || !pInteg->pDemod || !pInteg->pDemod->pI2c || !pInteg->pTuner || !pInteg->pTuner->pI2c){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    pInteg->cancelFlag = 1;

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  Tuning sequence

  This is used by cxd2828_isdb_t_integ_Tune and cxd2828_isdb_t_integ_Scan.

  pInteg : Address of ISDB-T integration part instance.
  isScan : Scan or Tune (0:Tune, 1:Scan)
--------------------------------------------------------------------*/
static cxd2828_result_t Tune(cxd2828_isdb_t_integ_t* pInteg, UINT8 isScan)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;
    int index = 0;
    int inv_seq = 0;
    UINT8 isFalseUnlock = 0;
    UINT8 data = 0;
    cxd2828_isdb_t_demod_lock_status_t lockStatus;

    CXD2828_TRACE_ENTER("Tune");

    /* Null check */
    if(!pInteg || !pInteg->pDemod || !pInteg->pDemod->pI2c || !pInteg->pTuner || !pInteg->pTuner->pI2c){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    /* Tune RF */
    result = pInteg->pTuner->Tune(pInteg->pTuner, pInteg->frequency);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    /* Tune Demod (Preset) */
    if(pInteg->isPreset){
        result = cxd2828_isdb_t_demod_SetPreset(pInteg->pDemod, &(pInteg->presetInfo));
    } else {
        result = cxd2828_isdb_t_demod_SetPreset(pInteg->pDemod, NULL);
    }
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    /* Tune Demod (Tune) */
    if(pInteg->isEWS){
        result = cxd2828_isdb_t_demod_EWSTune(pInteg->pDemod);
    } else {
        result = cxd2828_isdb_t_demod_Tune(pInteg->pDemod);
    }
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    retry_label:
    for(index = 0; index < (CXD2828_ISDB_T_INTEG_TUNE_TIMEOUT/CXD2828_ISDB_T_INTEG_TUNE_LOCK_CHECK_PERIOD); index++){
        /* Check cancel flag */
        if(pInteg->cancelFlag){
            CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_CANCEL);
        }

        /* Get lock status */
        if(isScan){
            /* Scan */
            result = cxd2828_isdb_t_demod_CheckDemodLock(pInteg->pDemod, &lockStatus);
            if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        } else {
            /* Tune */
            if(pInteg->isEWS){
                /* In the case of EWS tune, TS lock flag is 0 all time. */
                /* So it is necessary to check demod lock status.       */
                result = cxd2828_isdb_t_demod_CheckDemodLock(pInteg->pDemod, &lockStatus);
                if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
            } else {
                result = cxd2828_isdb_t_demod_CheckTSLock(pInteg->pDemod, &lockStatus);
                if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
            }
        }

        /* Check lock status */
        switch(lockStatus){
        case CXD2828_ISDB_T_DEMOD_LOCK_STATUS_LOCK:
            CXD2828_TRACE_RETURN(CXD2828_RESULT_OK);
        case CXD2828_ISDB_T_DEMOD_LOCK_STATUS_UNLOCK:
            result = pInteg->pDemod->pI2c->ReadRegister(pInteg->pDemod->pI2c, pInteg->pDemod->i2cAddress, 0x90, &data, 1);
            if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
            if(((data & 0x01) == 0x01) && (inv_seq == 0)){
                /* auto spectrum inversion ON & 1st unlock judge in this Tune sequence */
                inv_seq++;
                result = cxd2828_isdb_t_demod_CheckFalseUnlock(pInteg->pDemod, &isFalseUnlock);
                if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
                if(isFalseUnlock == 0){
                    CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_UNLOCK);
                } else {
                    /* retry wait lock sequence */
                    goto retry_label;
                }
            } else {
                CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_UNLOCK);
            }
        case CXD2828_ISDB_T_DEMOD_LOCK_STATUS_NOT_DETECT:
            /* continue waiting... */
            break;
        default:
            /* error occurred */
            CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_OTHER);
        }
        CXD2828_SLEEP(CXD2828_ISDB_T_INTEG_TUNE_LOCK_CHECK_PERIOD);
    }

    CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_TIMEOUT);
}
