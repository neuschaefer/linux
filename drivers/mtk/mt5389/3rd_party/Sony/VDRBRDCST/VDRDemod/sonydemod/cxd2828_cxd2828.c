/*------------------------------------------------------------------------------

 Copyright 2010 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2010/08/20
 Revision: 1.1.0.0

------------------------------------------------------------------------------*/

#include "cxd2828_cxd2828.h"

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
                                  UINT8 i2cAddress_T)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_ENTER("cxd2828_cxd2828_Create");

    /* Null check */
    if(!pTDemod || !pI2C_T || !pI2C_T->Read || !pI2C_T->ReadRegister || !pI2C_T->Write ||
        !pI2C_T->WriteOneRegister || !pI2C_T->WriteRegister ){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    /* ===== initialize cxd2828_isdb_t_demod_t ===== */
    pTDemod->pI2c = pI2C_T;
    pTDemod->i2cAddress = i2cAddress_T;
    pTDemod->product = CXD2828_ISDB_T_DEMOD_PRODUCT_CXD2828;
    pTDemod->state = CXD2828_ISDB_T_DEMOD_STATE_PRE_INIT;
    pTDemod->OTSDT_HIZ = 0x7F;

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  Initialize CXD2828R

  All registers of CXD2828R are initialized.
  The software state of ISDB-T change to "Init".
  The hardware state of ISDB-T part change to "Sleep-T".

  pTDemod : Address of ISDB-T part instance.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_cxd2828_Initialize(cxd2828_isdb_t_demod_t* pTDemod)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_ENTER("cxd2828_cxd2828_Initialize");

    /* Null check */
    if(!pTDemod){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    /* Initialize internal prarameters */
    pTDemod->OTSDT_HIZ = 0x7F;

    /* Initialize all register */
    result = pTDemod->pI2c->WriteOneRegister(pTDemod->pI2c, pTDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
    result = pTDemod->pI2c->WriteOneRegister(pTDemod->pI2c, pTDemod->i2cAddress, 0x02, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    /* Set state to PreInit */
    pTDemod->state = CXD2828_ISDB_T_DEMOD_STATE_PRE_INIT;

    /* Initialize ISDB-T demod */
    result = cxd2828_isdb_t_demod_Initialize(pTDemod);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    CXD2828_TRACE_RETURN(result);
}

/*--------------------------------------------------------------------
  TS output setting

  Change the setting of TS output pattern.

  pTDemod   : Address of ISDB-T part instance.
  tsOut     : TS output pattern.
  clockRate : Clock rate of ISDB-T at serial output (0:Low rate, 1:High rate)
              It is ignored in case of parallel output.
--------------------------------------------------------------------*/
cxd2828_result_t cxd2828_cxd2828_TSOutputSetting(cxd2828_isdb_t_demod_t* pTDemod, cxd2828_cxd2828_tsout_t tsOut, UINT8 clockRate)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;
    UINT8 data;
    UINT8 oparallel = 0;
    UINT8 oserial2p = 0;
    UINT8 oserxchg = 0;
    UINT8 osatout = 0;

    CXD2828_TRACE_ENTER("cxd2828_cxd2828_TSOutputSetting");

    /* Null check */
    if(!pTDemod || !pTDemod->pI2c){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    /* Parameter check */
    if(clockRate >= 2){
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }
    
    /* Settings for SLV-I */
    switch(tsOut)
    {
    case CXD2828_CXD2828_TSOUT_PARALLEL_T:
        oparallel = 1;
        oserial2p = 0;
        oserxchg = 0;
        osatout = 0;
        break;
    case CXD2828_CXD2828_TSOUT_SERIAL_T_N:
        oparallel = 0;
        oserial2p = 0;
        oserxchg = 0;
        osatout = 0;
        break;
    case CXD2828_CXD2828_TSOUT_SERIAL_T2_N:
        oparallel = 0;
        oserial2p = 0;
        oserxchg = 0;
        osatout = 0;
        break;
    default:
        CXD2828_TRACE_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    result = pTDemod->pI2c->WriteOneRegister(pTDemod->pI2c, pTDemod->i2cAddress, 0x00, 0x00);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    /* data[1] : oparallel */
    /* data[0] : oserial2p */
    /* mask    : 0000 0011 (0x03) */
    data = ((oparallel & 0x01) << 1) | (oserial2p & 0x01);
    result = cxd2828_i2c_SetRegisterBits(pTDemod->pI2c, pTDemod->i2cAddress, 0xCD, data, 0x03);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    /* data[7] : oserxchg  */
    /* data[5] : osatout   */
    /* mask    : 1010 0000 */
    data = ((oserxchg & 0x01) << 7) | ((osatout & 0x01) << 5);
    result = cxd2828_i2c_SetRegisterBits(pTDemod->pI2c, pTDemod->i2cAddress, 0xCE, data, 0xA0);
    if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }

    if(tsOut == CXD2828_CXD2828_TSOUT_PARALLEL_T){
        /* data[1] : sckdirect = 0 */
        /* data[0] : oserialen = 0 */
        /* mask    : 0000 0011 (0x03) */
        result = cxd2828_i2c_SetRegisterBits(pTDemod->pI2c, pTDemod->i2cAddress, 0x19, 0x00, 0x03);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        pTDemod->OTSDT_HIZ = 0x00;
        /* check hardware state */
        result = pTDemod->pI2c->ReadRegister(pTDemod->pI2c, pTDemod->i2cAddress, 0xFC, &data, 1);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        if(data == 0x83){
            /* If the hardware state is Normal-T, set pTDemod->OTSDT_HIZ to register immediately. */
            result = pTDemod->pI2c->WriteOneRegister(pTDemod->pI2c, pTDemod->i2cAddress, 0x1E, pTDemod->OTSDT_HIZ);
            if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        }
    } else {
        /* data[1] : sckdirect = clockRate */
        /* data[0] : oserialen = 1 */
        /* mask    : 0000 0011 (0x03) */
        data = ((clockRate & 0x01) << 1 | 0x01);
        result = cxd2828_i2c_SetRegisterBits(pTDemod->pI2c, pTDemod->i2cAddress, 0x19, data, 0x03);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        /* oserexchgb7 */
        if(tsOut == CXD2828_CXD2828_TSOUT_SERIAL_T2_N){
            data = 0x01;
            pTDemod->OTSDT_HIZ = 0x7F;
        } else {
            data = 0x00;
            pTDemod->OTSDT_HIZ = 0xFE;
        }
        result = cxd2828_i2c_SetRegisterBits(pTDemod->pI2c, pTDemod->i2cAddress, 0x1A, data, 0x01);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        /* check hardware state */
        result = pTDemod->pI2c->ReadRegister(pTDemod->pI2c, pTDemod->i2cAddress, 0xFC, &data, 1);
        if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        if(data == 0x83){
            /* If the hardware state is Normal-T, set pTDemod->OTSDT_HIZ to register immediately. */
            result = pTDemod->pI2c->WriteOneRegister(pTDemod->pI2c, pTDemod->i2cAddress, 0x1E, pTDemod->OTSDT_HIZ);
            if(result != CXD2828_RESULT_OK){ CXD2828_TRACE_RETURN(result); }
        }
    }
    CXD2828_TRACE_RETURN(result);
}
