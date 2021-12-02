/*------------------------------------------------------------------------------

 Copyright 2011 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2011/01/11
 Revision: 1.0.0.0

------------------------------------------------------------------------------*/

#include "cxd2828_i2c_MTK.h"
#include "i2c_api.h"

#define BURST_WRITE_MAX 128

static cxd2828_result_t cxd2828_i2c_MTK_Read(cxd2828_i2c_t* pI2c, UINT8 deviceAddress, UINT8* pData,
                  UINT32 size, UINT8 mode)
{
    CXD2828_TRACE_I2C_ENTER("cxd2828_i2c_MTK_Read");

    CXD2828_ARG_UNUSED(mode);

    if((!pI2c) || (!pData) || (size == 0)) CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_ERROR_ARG);

    if(ucI2cReadOnly(deviceAddress, (UINT8*)pData, (UINT16)size) == 0){
        CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_OK);
    }else{
        CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_ERROR_I2C);
    }
}

static cxd2828_result_t cxd2828_i2c_MTK_Write(cxd2828_i2c_t* pI2c, UINT8 deviceAddress, const UINT8 * pData,
                  UINT32 size, UINT8 mode)
{
    CXD2828_TRACE_I2C_ENTER("cxd2828_i2c_MTK_Write");

    CXD2828_ARG_UNUSED(mode);

    if((!pI2c) || (!pData) || (size == 0)) CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_ERROR_ARG);

    if(ucI2cWriteOnly(deviceAddress, (UINT8*)pData, (UINT16)size) == 0){
        CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_OK);
    }else{
        CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_ERROR_I2C);
    }
}

static cxd2828_result_t cxd2828_i2c_MTK_ReadGw(cxd2828_i2c_t* pI2c, UINT8 deviceAddress, UINT8* pData,
                  UINT32 size, UINT8 mode)
{
    /* NOT USED NOW */
    CXD2828_TRACE_I2C_ENTER("cxd2828_i2c_MTK_ReadGw");

    CXD2828_ARG_UNUSED(mode);

    if((!pI2c) || (!pData) || (size == 0)) CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_ERROR_ARG);

    CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_OK);
}

static cxd2828_result_t cxd2828_i2c_MTK_WriteGw(cxd2828_i2c_t* pI2c, UINT8 deviceAddress, const UINT8 * pData,
                  UINT32 size, UINT8 mode)
{
    UINT8 buffer[BURST_WRITE_MAX + 1];

    CXD2828_TRACE_I2C_ENTER("cxd2828_i2c_MTK_WriteGw");

    CXD2828_ARG_UNUSED(mode);

    if((!pI2c) || (!pData) || (size == 0)) CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_ERROR_ARG);

    buffer[0] = deviceAddress;
    x_memcpy(&(buffer[1]), pData, size);

    if(ucI2cWrite(pI2c->gwAddress, pI2c->gwSub, buffer, (UINT16)(size + 1)) == 0){
        CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_OK);
    }else{
        CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_ERROR_I2C);
    }
}

static cxd2828_result_t cxd2828_i2c_MTK_ReadRegister(cxd2828_i2c_t* pI2c, UINT8 deviceAddress, UINT8 subAddress,
                  UINT8* pData, UINT32 size)
{
    CXD2828_TRACE_I2C_ENTER("cxd2828_i2c_MTK_ReadRegister");

    if((!pI2c) || (!pData) || (size == 0)) CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_ERROR_ARG);

    if(ucI2cRead(deviceAddress, subAddress, (UINT8*)pData, (UINT16)size) == 0){
        CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_OK);
    }else{
        CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_ERROR_I2C);
    }
}

static cxd2828_result_t cxd2828_i2c_MTK_WriteRegister(cxd2828_i2c_t* pI2c, UINT8 deviceAddress, UINT8 subAddress,
                  const UINT8* pData, UINT32 size)
{
    CXD2828_TRACE_I2C_ENTER("cxd2828_i2c_MTK_WriteRegister");

    if((!pI2c) || (!pData) || (size == 0)) CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_ERROR_ARG);

    if(ucI2cWrite(deviceAddress, subAddress, (UINT8*)pData, (UINT16)size) == 0){
        CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_OK);
    }else{
        CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_ERROR_I2C);
    }
}

static cxd2828_result_t cxd2828_i2c_MTK_WriteRegisterGw(cxd2828_i2c_t* pI2c, UINT8 deviceAddress, UINT8 subAddress,
                  const UINT8* pData, UINT32 size)
{
    UINT8 buffer[BURST_WRITE_MAX + 2];

    CXD2828_TRACE_I2C_ENTER("cxd2828_i2c_MTK_WriteRegisterGw");

    if((!pI2c) || (!pData) || (size == 0)) CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_ERROR_ARG);

    buffer[0] = deviceAddress;
    buffer[1] = subAddress;
    x_memcpy(&(buffer[2]), pData, size);

    if(ucI2cWrite(pI2c->gwAddress, pI2c->gwSub, buffer, (UINT16)(size + 2)) == 0){
        CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_OK);
    }else{
        CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_ERROR_I2C);
    }
}

static cxd2828_result_t cxd2828_i2c_MTK_ReadRegisterGw(cxd2828_i2c_t* pI2c, UINT8 deviceAddress, UINT8 subAddress,
                  UINT8* pData, UINT32 size)
{
    UINT32 wordAddr = 0;
    cxd2828_result_t cxd2828_result = CXD2828_RESULT_OK;
    
    CXD2828_TRACE_I2C_ENTER("cxd2828_i2c_MTK_ReadRegisterGw");

    if((!pI2c) || (!pData) || (size == 0)) CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_ERROR_ARG);

    {
        UINT8 data = subAddress;
        cxd2828_result = cxd2828_i2c_MTK_WriteGw(pI2c, deviceAddress, &data, 1, CXD2828_I2C_START_EN);
        if(cxd2828_result != CXD2828_RESULT_OK){
            CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_ERROR_I2C);
        }
    }

    // I2C address in MTK function should be in 8bit form
    wordAddr = (pI2c->gwSub << 8) | (deviceAddress | 0x01); // I2C Read

    if(ucI2cReadOnly_Gateway(pI2c->gwAddress, wordAddr, (UINT8 *)pData, size) == 0){
        CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_OK);
    }else{
        CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_ERROR_I2C);
    }
}

cxd2828_result_t cxd2828_i2c_MTK_CreateI2c(cxd2828_i2c_t* pI2c)
{
    CXD2828_TRACE_I2C_ENTER("cxd2828_i2c_MTK_CreateI2c");

    if(!pI2c){
        CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    pI2c->Read = cxd2828_i2c_MTK_Read;
    pI2c->Write = cxd2828_i2c_MTK_Write;
    pI2c->ReadRegister = cxd2828_i2c_MTK_ReadRegister;
    pI2c->WriteRegister = cxd2828_i2c_MTK_WriteRegister;
    pI2c->WriteOneRegister = cxd2828_i2c_CommonWriteOneRegister;
    pI2c->gwAddress = 0;
    pI2c->gwSub = 0;
    pI2c->user = NULL;

    CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_OK);
}

cxd2828_result_t cxd2828_i2c_MTK_CreateI2cGw(cxd2828_i2c_t* pI2c, UINT8 gwAddress, UINT8 gwSub)
{
    CXD2828_TRACE_I2C_ENTER("cxd2828_i2c_MTK_CreateI2cGw");

    if(!pI2c){
        CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_ERROR_ARG);
    }

    pI2c->Read = cxd2828_i2c_MTK_ReadGw; // NOTE: This is not used.
    pI2c->Write = cxd2828_i2c_MTK_WriteGw;
    pI2c->ReadRegister = cxd2828_i2c_MTK_ReadRegisterGw;
    pI2c->WriteRegister = cxd2828_i2c_MTK_WriteRegisterGw;
    pI2c->WriteOneRegister = cxd2828_i2c_CommonWriteOneRegister;
    pI2c->gwAddress = gwAddress;
    pI2c->gwSub = gwSub;
    pI2c->user = NULL;

    CXD2828_TRACE_I2C_RETURN(CXD2828_RESULT_OK);
}

