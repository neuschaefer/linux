/*------------------------------------------------------------------------------

 Copyright 2009, 2010 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2010/03/19
 Revision: 1.0.0.0

------------------------------------------------------------------------------*/

#include "cxd2828_i2c.h"

#include "cxd2828_stdlib.h" /* for memcpy */

#define BURST_WRITE_MAX 128 /* Max length of burst write */

cxd2828_result_t cxd2828_i2c_CommonReadRegister(cxd2828_i2c_t* pI2c, UINT8 deviceAddress, UINT8 subAddress, UINT8* pData, UINT32 size)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_I2C_ENTER("cxd2828_i2c_CommonReadRegister");

    if(!pI2c){
        CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    result = pI2c->Write(pI2c, deviceAddress, &subAddress, 1, CXD2828_I2C_START_EN);
    if(result == CXD2828_RESULT_OK){
        result = pI2c->Read(pI2c, deviceAddress, pData, size, CXD2828_I2C_START_EN | CXD2828_I2C_STOP_EN);
    }

    CXD2828_TRACE_I2C_RETURN(result);
}

cxd2828_result_t cxd2828_i2c_CommonWriteRegister(cxd2828_i2c_t* pI2c, UINT8 deviceAddress, UINT8 subAddress, const UINT8* pData, UINT32 size)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;
    UINT8 buffer[BURST_WRITE_MAX + 1];

    CXD2828_TRACE_I2C_ENTER("cxd2828_i2c_CommonWriteRegister");

    if(!pI2c){
        CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_ERROR_ARG);
    }
    if(size > BURST_WRITE_MAX){
        /* Buffer is too small... */
        CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    buffer[0] = subAddress;
    cxd2828_memcpy(&(buffer[1]), pData, size);

    /* send the new buffer */
    result = pI2c->Write(pI2c, deviceAddress, buffer, size+1, CXD2828_I2C_START_EN | CXD2828_I2C_STOP_EN);
    CXD2828_TRACE_I2C_RETURN(result);
}

cxd2828_result_t cxd2828_i2c_CommonWriteOneRegister(cxd2828_i2c_t* pI2c, UINT8 deviceAddress, UINT8 subAddress, UINT8 data)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_I2C_ENTER("cxd2828_i2c_CommonWriteOneRegister");

    if(!pI2c){
        CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_ERROR_ARG);
    }
    result = pI2c->WriteRegister(pI2c, deviceAddress, subAddress, &data, 1);
    CXD2828_TRACE_I2C_RETURN(result);
}

/* For Read-Modify-Write */
cxd2828_result_t cxd2828_i2c_SetRegisterBits(cxd2828_i2c_t* pI2c, UINT8 deviceAddress, UINT8 subAddress, UINT8 data, UINT8 mask)
{
    cxd2828_result_t result = CXD2828_RESULT_OK;

    CXD2828_TRACE_I2C_ENTER("cxd2828_i2c_SetRegisterBits");

    if(!pI2c){
        CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_ERROR_ARG);
    }
    if(mask == 0x00){
        /* Nothing to do */
        CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_OK);
    }

    if(mask != 0xFF){
        UINT8 rdata = 0x00;
        result = pI2c->ReadRegister(pI2c, deviceAddress, subAddress, &rdata, 1);
        if(result != CXD2828_RESULT_OK) CXD2828_TRACE_I2C_RETURN(result);
        data = (data & mask) | (rdata & (mask ^ 0xFF));
    }

    result = pI2c->WriteOneRegister(pI2c, deviceAddress, subAddress, data);
    CXD2828_TRACE_I2C_RETURN(result);
}

