/*------------------------------------------------------------------------------

 Copyright 2010 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2010/03/19
 Revision: 1.0.0.0

------------------------------------------------------------------------------*/
#ifndef CXD2828_ISDB_T_INTEG_H
#define CXD2828_ISDB_T_INTEG_H

#include "cxd2828_common.h"
#include "cxd2828_i2c.h"
#include "cxd2828_isdb_t.h"
#include "cxd2828_isdb_t_demod.h"
#include "cxd2828_isdb_t_tuner.h"

/*------------------------------------------------------------------------------
  Enums
------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------
  Event ID for callback event
--------------------------------------------------------------------*/
typedef enum cxd2828_isdb_t_integ_event_id_t {
  CXD2828_ISDB_T_INTEG_EVENT_ID_LOCK,    /* Lock in scan sequence */
  CXD2828_ISDB_T_INTEG_EVENT_ID_UNLOCK,  /* Unlock in scan sequence */
  CXD2828_ISDB_T_INTEG_EVENT_ID_UNKNOWN  /* Unknown event */
} cxd2828_isdb_t_integ_event_id_t;

/*------------------------------------------------------------------------------
  Structs
------------------------------------------------------------------------------*/
typedef struct cxd2828_isdb_t_integ_t {
    cxd2828_isdb_t_demod_t*            pDemod;    /* ISDB-T demod part instance */
    cxd2828_isdb_t_tuner_t*            pTuner;    /* ISDB-T tuner part instance */
    UINT32                        frequency; /* frequency (kHz) */
    UINT8                         isPreset;  /* Use preset information (0:Don't use, 1:Use) */
    cxd2828_isdb_t_demod_preset_info_t presetInfo;/* Preset information */
    UINT8                         isEWS;     /* Tuning type (0:Normal, 1:EWS) */
    UINT8                         cancelFlag;/* Flag for stop tuning and scan sequence */
    cxd2828_isdb_t_integ_event_id_t    eventId;   /* Event ID for callback event */
    void*                           user;      /* For users */
} cxd2828_isdb_t_integ_t;

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
cxd2828_result_t cxd2828_isdb_t_integ_Tune(cxd2828_isdb_t_integ_t* pInteg);

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
  +--------------------------------------------------------------
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
                                     void (*pCallback)(cxd2828_isdb_t_integ_t*));

/*--------------------------------------------------------------------
  Cancel sequence

  This function stop the sequence cxd2828_isdb_t_integ_Tune or
  cxd2828_isdb_t_integ_Scan by calling from other thread.

  pInteg     : Address of ISDB-T integration part instance.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_integ_Cancel(cxd2828_isdb_t_integ_t* pInteg);

#endif /* CXD2828_ISDB_T_INTEG_H */
