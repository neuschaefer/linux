/*------------------------------------------------------------------------------

 Copyright 2009, 2010 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2010/03/19
 Revision: 1.0.0.0

------------------------------------------------------------------------------*/

#ifndef CXD2828_ISDB_T_TUNER_H
#define CXD2828_ISDB_T_TUNER_H

#include "cxd2828_isdb_t.h"

/* Software state of this driver. */
typedef enum {
    CXD2828_ISDB_T_TUNER_STATE_UNKNOWN,
    CXD2828_ISDB_T_TUNER_STATE_SHUTDOWN,       /* Chip is in Shutdown state */
    CXD2828_ISDB_T_TUNER_STATE_PRE_INIT,       /* Chip is in PreInitialize state */
    CXD2828_ISDB_T_TUNER_STATE_INIT,           /* Chip is in Initialized state */
    CXD2828_ISDB_T_TUNER_STATE_SLEEP,          /* Chip is in Sleep state */
    CXD2828_ISDB_T_TUNER_STATE_TUNE,           /* Chip is in Tuning state */
    CXD2828_ISDB_T_TUNER_STATE_INVALID
} cxd2828_isdb_t_tuner_state_t;

/*------------------------------------------------------------------------------
  Structs
------------------------------------------------------------------------------*/

typedef struct cxd2828_isdb_t_tuner_t {
    cxd2828_result_t (*Initialize)(struct cxd2828_isdb_t_tuner_t* pTuner); /* Initialize the tuner */
    cxd2828_result_t (*Finalize)(struct cxd2828_isdb_t_tuner_t* pTuner);   /* Finalize the tuner */
    /*--------------------------------------------------------------------
      Tune a frequency with a certain bandwidth
      Tuner driver can block while waiting for PLL locked indication (if required).

      cxd2828_isdb_t_tuner_t* pTuner      Instance of tuner control struct
      UINT32          frequency      RF frequency(kHz)
    --------------------------------------------------------------------*/
    cxd2828_result_t (*Tune)(struct cxd2828_isdb_t_tuner_t* pTuner, UINT32 frequency);
    cxd2828_result_t (*Sleep)(struct cxd2828_isdb_t_tuner_t* pTuner);      /* Sleep the tuner */

    UINT32          frequency;  /* RF frequency(kHz) to tune */
    UINT32          xtalFreq;   /* Xtal frequency (MHz) */

    cxd2828_i2c_t*       pI2c;       /* I2C control instance */
    UINT8           i2cAddress; /* I2C address */

    UINT32          flags;      /* Flags that can be used by tuner drivers */
    void*             user;       /* User defined data */

    cxd2828_isdb_t_tuner_state_t state; /* Software state */
    /* This state information need not be used according to chip because it is unnecessary. */

} cxd2828_isdb_t_tuner_t;

#endif /* CXD2828_ISDB_T_TUNER_H */
