/*------------------------------------------------------------------------------

 Copyright 2010 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2010/08/20
 Revision: 1.1.0.0

------------------------------------------------------------------------------*/
#ifndef CXD2828_CXD2828_H
#define CXD2828_CXD2828_H

#include "cxd2828_common.h"
#include "cxd2828_i2c.h"
#include "cxd2828_isdb_t.h"
#include "cxd2828_isdb_t_demod.h"

/*------------------------------------------------------------------------------
  Enums
------------------------------------------------------------------------------*/
/* TS output setting */
typedef enum cxd2828_cxd2828_tsout_t {
    CXD2828_CXD2828_TSOUT_PARALLEL_T,  /* Parallel ( ISDB-T ) */
    CXD2828_CXD2828_TSOUT_SERIAL_T_N,  /* Serial   ( ISDB-T / None   ) */
    CXD2828_CXD2828_TSOUT_SERIAL_T2_N  /* Serial   ( ISDB-T(CXD2817R,CXD2820R compatible) / None ) */
} cxd2828_cxd2828_tsout_t;

/*------------------------------------------------------------------------------
  Functions
------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------
  Create instance

  Initialize pTDemod.

  pTDemod      : Address of ISDB-T part instance.
  pI2C_T       : Address of I2C instance for pTDemod.
  i2cAddress_T : I2C slave address for ISDB-T part (default:0xD8)
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_cxd2828_Create(cxd2828_isdb_t_demod_t* pTDemod,
                                  cxd2828_i2c_t* pI2C_T,
                                  UINT8 i2cAddress_T);

/*--------------------------------------------------------------------
  Initialize CXD2828R

  All registers of CXD2828R are initialized.
  The software state of ISDB-T change to "Init".
  The hardware state of ISDB-T part change to "Sleep-T".

  pTDemod : Address of ISDB-T part instance.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_cxd2828_Initialize(cxd2828_isdb_t_demod_t* pTDemod);

/*--------------------------------------------------------------------
  TS output setting

  Change the setting of TS output pattern.

  pTDemod   : Address of ISDB-T part instance.
  tsOut     : TS output pattern.
  clockRate : Clock rate of ISDB-T at serial output (0:Low rate, 1:High rate)
              It is ignored in case of parallel output.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_cxd2828_TSOutputSetting(cxd2828_isdb_t_demod_t* pTDemod, cxd2828_cxd2828_tsout_t tsOut, UINT8 clockRate);

#endif /* CXD2828_CXD2828_H */
