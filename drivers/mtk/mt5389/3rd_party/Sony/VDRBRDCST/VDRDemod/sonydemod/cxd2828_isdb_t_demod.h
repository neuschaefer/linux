/*------------------------------------------------------------------------------

 Copyright 2010, 2011 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2011/01/28
 Revision: 1.2.1.0

------------------------------------------------------------------------------*/
#ifndef CXD2828_ISDB_T_DEMOD_H
#define CXD2828_ISDB_T_DEMOD_H

#include "cxd2828_common.h"
#include "cxd2828_i2c.h"
#include "cxd2828_isdb_t.h"

/*------------------------------------------------------------------------------
  I2C address default (8bit form)
------------------------------------------------------------------------------*/
#define CXD2828_ISDB_T_DEMOD_ADDRESS           0xD8

/*------------------------------------------------------------------------------
  Preset information size
------------------------------------------------------------------------------*/
#define CXD2828_ISDB_T_DEMOD_PRESET_INFO_BYTES   13

/*------------------------------------------------------------------------------
  Enums
------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------
  Product
--------------------------------------------------------------------*/
typedef enum cxd2828_isdb_t_demod_product_t{
    CXD2828_ISDB_T_DEMOD_PRODUCT_CXD2826, /* CXD2826R */
    CXD2828_ISDB_T_DEMOD_PRODUCT_CXD2827, /* CXD2827R */
    CXD2828_ISDB_T_DEMOD_PRODUCT_CXD2828, /* CXD2828R */
    CXD2828_ISDB_T_DEMOD_PRODUCT_UNKNOWN
} cxd2828_isdb_t_demod_product_t;

/*--------------------------------------------------------------------
  Software state
--------------------------------------------------------------------*/
typedef enum cxd2828_isdb_t_demod_state_t{
    CXD2828_ISDB_T_DEMOD_STATE_UNKNOWN,
    CXD2828_ISDB_T_DEMOD_STATE_SHUTDOWN,
    CXD2828_ISDB_T_DEMOD_STATE_PRE_INIT, /* Hardware state is unknown.    */
    CXD2828_ISDB_T_DEMOD_STATE_INIT,     /* Hardware state is "Sleep".    */
    CXD2828_ISDB_T_DEMOD_STATE_NORMAL_T, /* Hardware state is "Normal-T". */
    CXD2828_ISDB_T_DEMOD_STATE_EWS,      /* Hardware state is "EWS".      */
    CXD2828_ISDB_T_DEMOD_STATE_INVALID
}cxd2828_isdb_t_demod_state_t;

/*--------------------------------------------------------------------
  Lock status
--------------------------------------------------------------------*/
typedef enum cxd2828_isdb_t_demod_lock_status_t{
    CXD2828_ISDB_T_DEMOD_LOCK_STATUS_UNLOCK,     /* Unlock */
    CXD2828_ISDB_T_DEMOD_LOCK_STATUS_NOT_DETECT, /* Not detect */
    CXD2828_ISDB_T_DEMOD_LOCK_STATUS_LOCK,       /* Lock */
    CXD2828_ISDB_T_DEMOD_LOCK_STATUS_UNKNOWN     /* Error occurred */
} cxd2828_isdb_t_demod_lock_status_t;

/*--------------------------------------------------------------------
  Config ID
--------------------------------------------------------------------*/
typedef enum cxd2828_isdb_t_demod_config_t{
    CXD2828_ISDB_T_DEMOD_CONFIG_OWFMT_CKINV,              /* TS clock edge (0:Falling edge, 1:Rising edge(default)) */
    CXD2828_ISDB_T_DEMOD_CONFIG_OCTL_IFAGCNEG,            /* The polarity of the tuner IFAGC (0:Do not invert, 1:Invert(default)) */
    CXD2828_ISDB_T_DEMOD_CONFIG_OCTL_ITBINVERT,           /* Input spectrum (0:Do not invert(default), 1:Invert) */
    CXD2828_ISDB_T_DEMOD_CONFIG_OREG_SYR_JPOG,            /* Limit modulation types (0:Full ISDB-T support, 1:Limit operation to the Japanese Operational Guidelines(default)) */
    CXD2828_ISDB_T_DEMOD_CONFIG_OREG_CCOD_SINV_SEARCH_ON, /* Detection and automatic correction of spectrum inversion.(0:Disable(default), 1:Enable) */
    CXD2828_ISDB_T_DEMOD_CONFIG_OTDA_DNCNV_FREQ,          /* Frequency Offset Setting for the Input Down Converter. Please set by CXD2828_ISDB_T_DEMOD_MAKE_IFFREQ_CONFIG() */
    CXD2828_ISDB_T_DEMOD_CONFIG_OADCGAIN,                 /* ADC full scale range(0:0.7Vpp, 1:1.0Vpp, 2:1.4Vpp(default)) */
} cxd2828_isdb_t_demod_config_t;

/* This macro is used to calculate IF frequency setting register values.         */
/* This macro includes floating point values but will be calculated by compiler! */
#define CXD2828_ISDB_T_DEMOD_MAKE_IFFREQ_CONFIG(freq) (UINT32)((freq * 409.2004) + 0.5)

/*------------------------------------------------------------------------------
  Structs
------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------
  Preset information
--------------------------------------------------------------------*/
typedef struct cxd2828_isdb_t_demod_preset_info_t{
    UINT8 data[CXD2828_ISDB_T_DEMOD_PRESET_INFO_BYTES];
} cxd2828_isdb_t_demod_preset_info_t;

/*--------------------------------------------------------------------
  ISDB-T demod part control data
--------------------------------------------------------------------*/
typedef struct cxd2828_isdb_t_demod_t{
    cxd2828_isdb_t_demod_state_t   state;      /* state of this software */
    cxd2828_i2c_t*                 pI2c;       /* I2C instance for ISDB-T control */
    UINT8                     i2cAddress; /* I2C slave address for ISDB-T control */
    cxd2828_isdb_t_demod_product_t product;    /* Product ID */
    UINT8                     OTSDT_HIZ;  /* OTSDT_HIZ */
    void*                       user;       /* User defined void* pointer */
} cxd2828_isdb_t_demod_t;

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
cxd2828_result_t cxd2828_isdb_t_demod_Initialize(cxd2828_isdb_t_demod_t* pDemod);

/*--------------------------------------------------------------------
  ISDB-T tuning

  Set tuning parameter to ISDB-T demod part.
  Before calling this function, please call cxd2828_isdb_t_demod_SetPreset
  and set preset information.

  pDemod : Address of ISDB-T part instance.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_Tune(cxd2828_isdb_t_demod_t* pDemod);

/*--------------------------------------------------------------------
  ISDB-T EWS tuning

  Set EWS tuning parameter to ISDB-T demod part.
  Before calling this function, please call cxd2828_isdb_t_demod_SetPreset
  and set preset information.

  pDemod : Address of ISDB-T part instance.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_EWSTune(cxd2828_isdb_t_demod_t* pDemod);

/*--------------------------------------------------------------------
  ISDB-T sleep

  Change hardware state to "Sleep".

  pDemod : Address of ISDB-T part instance.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_Sleep(cxd2828_isdb_t_demod_t* pDemod);

/*--------------------------------------------------------------------
  Set preset information

  Set preset information to shorten tuning time.
  It must call this function before tuning sequence.

  pDemod     : Address of ISDB-T part instance.
  presetInfo : Address of preset information.
               If it set "Null" preset informaiton is disable.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_SetPreset(cxd2828_isdb_t_demod_t* pDemod,
                                          cxd2828_isdb_t_demod_preset_info_t* presetInfo);

/*--------------------------------------------------------------------
  Soft reset

  Soft reset to ISDB-T part.

  pDemod     : Address of ISDB-T part instance.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_SoftReset(cxd2828_isdb_t_demod_t* pDemod);

/*--------------------------------------------------------------------
  Check demod lock status of ISDB-T demod part

  pDemod      : Address of ISDB-T part instance.
  pLockStatus : lock status of ISDB-T demod part.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_CheckDemodLock(cxd2828_isdb_t_demod_t* pDemod,
                                               cxd2828_isdb_t_demod_lock_status_t* pLockStatus);

/*--------------------------------------------------------------------
  Check TS lock status of ISDB-T demod part

  Return the result of TS lock flag.

  pDemod      : Address of ISDB-T part instance.
  pLockStatus : lock status of ISDB-T demod part.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_CheckTSLock(cxd2828_isdb_t_demod_t* pDemod,
                                            cxd2828_isdb_t_demod_lock_status_t* pLockStatus);


/*--------------------------------------------------------------------
  Check false unlock and retry if it is false unlock.

  pDemod         : Address of ISDB-T part instance.
  pIsFalseUnlock : 0:Correct unlock, 1:False unlock, please retry lock wait sequence.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_CheckFalseUnlock(cxd2828_isdb_t_demod_t* pDemod, UINT8* pIsFalseUnlock);

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
                                            UINT8 isWrite);

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
                                         UINT8* pValue);

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
                                          UINT8 value);

/*--------------------------------------------------------------------
  Set parameters for ISDB-T demod part

  Set each parameters for ISDB-T demod part.

  pDemod : Address of ISDB-T part instance.
  config : Config parmeters type.
  value  : Setting value.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_isdb_t_demod_SetConfig(cxd2828_isdb_t_demod_t* pDemod,
                                          cxd2828_isdb_t_demod_config_t config,
                                          UINT32 value);

#endif /* CXD2828_ISDB_T_DEMOD_H */
