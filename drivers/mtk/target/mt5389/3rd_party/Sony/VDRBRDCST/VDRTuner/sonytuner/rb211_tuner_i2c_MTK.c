/*------------------------------------------------------------------------------

 Copyright 2011 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2011/01/11
 Revision: 1.0.0.0

------------------------------------------------------------------------------*/

#include "rb211_tuner_i2c_MTK.h"
#include "i2c_api.h"

#define BURST_WRITE_MAX 128

static rb211_result_t rb211_tuner_i2c_MTK_Read(rb211_tuner_i2c_t* pI2c, UINT8 deviceAddress, UINT8* pData,
                  UINT32 size, UINT8 mode)
{
    RB211_TRACE_I2C_ENTER("rb211_tuner_i2c_MTK_Read");

    RB211_ARG_UNUSED(mode);

    if((!pI2c) || (!pData) || (size == 0)) RB211_TRACE_I2C_RETURN(RB211_RESULT_ERROR_ARG);

    if(ucI2cReadOnly(deviceAddress, (UINT8*)pData, (UINT16)size) == 0){
        RB211_TRACE_I2C_RETURN(RB211_RESULT_OK);
    }else{
        RB211_TRACE_I2C_RETURN(RB211_RESULT_ERROR_I2C);
    }
}

static rb211_result_t rb211_tuner_i2c_MTK_Write(rb211_tuner_i2c_t* pI2c, UINT8 deviceAddress, const UINT8 * pData,
                  UINT32 size, UINT8 mode)
{
    RB211_TRACE_I2C_ENTER("rb211_tuner_i2c_MTK_Write");

    RB211_ARG_UNUSED(mode);

    if((!pI2c) || (!pData) || (size == 0)) RB211_TRACE_I2C_RETURN(RB211_RESULT_ERROR_ARG);

    if(ucI2cWriteOnly(deviceAddress, (UINT8*)pData, (UINT16)size) == 0){
        RB211_TRACE_I2C_RETURN(RB211_RESULT_OK);
    }else{
        RB211_TRACE_I2C_RETURN(RB211_RESULT_ERROR_I2C);
    }
}

static rb211_result_t rb211_tuner_i2c_MTK_ReadGw(rb211_tuner_i2c_t* pI2c, UINT8 deviceAddress, UINT8* pData,
                  UINT32 size, UINT8 mode)
{
    /* NOT Used for now */
    RB211_TRACE_I2C_ENTER("rb211_tuner_i2c_MTK_ReadGw");

    RB211_ARG_UNUSED(mode);

    if((!pI2c) || (!pData) || (size == 0)) RB211_TRACE_I2C_RETURN(RB211_RESULT_ERROR_ARG);

    RB211_TRACE_I2C_RETURN(RB211_RESULT_OK);
}

static rb211_result_t rb211_tuner_i2c_MTK_WriteGw(rb211_tuner_i2c_t* pI2c, UINT8 deviceAddress, const UINT8 * pData,
                  UINT32 size, UINT8 mode)
{
    UINT8 buffer[BURST_WRITE_MAX + 1];

    RB211_TRACE_I2C_ENTER("rb211_tuner_i2c_MTK_WriteGw");

    RB211_ARG_UNUSED(mode);

    if((!pI2c) || (!pData) || (size == 0)) RB211_TRACE_I2C_RETURN(RB211_RESULT_ERROR_ARG);

    buffer[0] = deviceAddress;
    x_memcpy(&(buffer[1]), pData, size);

    if(ucI2cWrite(pI2c->gwAddress, pI2c->gwSub, buffer, (UINT16)(size + 1)) == 0){
        RB211_TRACE_I2C_RETURN(RB211_RESULT_OK);
    }else{
        RB211_TRACE_I2C_RETURN(RB211_RESULT_ERROR_I2C);
    }
}

static rb211_result_t rb211_tuner_i2c_MTK_ReadRegister(rb211_tuner_i2c_t* pI2c, UINT8 deviceAddress, UINT8 subAddress,
                  UINT8* pData, UINT32 size)
{
    RB211_TRACE_I2C_ENTER("rb211_tuner_i2c_MTK_ReadRegister");

    if((!pI2c) || (!pData) || (size == 0)) RB211_TRACE_I2C_RETURN(RB211_RESULT_ERROR_ARG);

    if(ucI2cRead(deviceAddress, subAddress, (UINT8*)pData, (UINT16)size) == 0){
        RB211_TRACE_I2C_RETURN(RB211_RESULT_OK);
    }else{
        RB211_TRACE_I2C_RETURN(RB211_RESULT_ERROR_I2C);
    }
}

static rb211_result_t rb211_tuner_i2c_MTK_WriteRegister(rb211_tuner_i2c_t* pI2c, UINT8 deviceAddress, UINT8 subAddress,
                  const UINT8* pData, UINT32 size)
{
    RB211_TRACE_I2C_ENTER("rb211_tuner_i2c_MTK_WriteRegister");

    if((!pI2c) || (!pData) || (size == 0)) RB211_TRACE_I2C_RETURN(RB211_RESULT_ERROR_ARG);

    if(ucI2cWrite(deviceAddress, subAddress, (UINT8*)pData, (UINT16)size) == 0){
        RB211_TRACE_I2C_RETURN(RB211_RESULT_OK);
    }else{
        RB211_TRACE_I2C_RETURN(RB211_RESULT_ERROR_I2C);
    }
}

static rb211_result_t rb211_tuner_i2c_MTK_WriteRegisterGw(rb211_tuner_i2c_t* pI2c, UINT8 deviceAddress, UINT8 subAddress,
                  const UINT8* pData, UINT32 size)
{
    UINT8 buffer[BURST_WRITE_MAX + 2];

    RB211_TRACE_I2C_ENTER("rb211_tuner_i2c_MTK_WriteRegisterGw");

    if((!pI2c) || (!pData) || (size == 0)) RB211_TRACE_I2C_RETURN(RB211_RESULT_ERROR_ARG);

    buffer[0] = deviceAddress;
    buffer[1] = subAddress;
    x_memcpy(&(buffer[2]), pData, size);

    if(ucI2cWrite(pI2c->gwAddress, pI2c->gwSub, buffer, (UINT16)(size + 2)) == 0){
        RB211_TRACE_I2C_RETURN(RB211_RESULT_OK);
    }else{
        RB211_TRACE_I2C_RETURN(RB211_RESULT_ERROR_I2C);
    }
}

static rb211_result_t rb211_tuner_i2c_MTK_ReadRegisterGw(rb211_tuner_i2c_t* pI2c, UINT8 deviceAddress, UINT8 subAddress,
                  UINT8* pData, UINT32 size)
{
    UINT32 wordAddr = 0;
    rb211_result_t rb211_result = RB211_RESULT_OK;

    RB211_TRACE_I2C_ENTER("rb211_tuner_i2c_MTK_ReadRegisterGw");

    if((!pI2c) || (!pData) || (size == 0)) RB211_TRACE_I2C_RETURN(RB211_RESULT_ERROR_ARG);

    {
        UINT8 data = subAddress;
        rb211_result = rb211_tuner_i2c_MTK_WriteGw(pI2c, deviceAddress, &data, 1, RB211_TUNER_I2C_START_EN);
        if(rb211_result != RB211_RESULT_OK){
            RB211_TRACE_I2C_RETURN(RB211_RESULT_ERROR_I2C);
        }
    }

    // I2C address in MTK function should be in 8bit form
    wordAddr = (pI2c->gwSub << 8) | (deviceAddress | 0x01); // I2C Read

    if(ucI2cReadOnly_Gateway(pI2c->gwAddress, wordAddr, (UINT8 *)pData, size) == 0){
        RB211_TRACE_I2C_RETURN(RB211_RESULT_OK);
    }else{
        RB211_TRACE_I2C_RETURN(RB211_RESULT_ERROR_I2C);
    }
}

rb211_result_t rb211_tuner_i2c_MTK_CreateI2c(rb211_tuner_i2c_t* pI2c)
{
    RB211_TRACE_I2C_ENTER("rb211_tuner_i2c_MTK_CreateI2c");

    if(!pI2c){
        RB211_TRACE_I2C_RETURN(RB211_RESULT_ERROR_ARG);
    }

    pI2c->Read = rb211_tuner_i2c_MTK_Read;
    pI2c->Write = rb211_tuner_i2c_MTK_Write;
    pI2c->ReadRegister = rb211_tuner_i2c_MTK_ReadRegister;
    pI2c->WriteRegister = rb211_tuner_i2c_MTK_WriteRegister;
    pI2c->WriteOneRegister = rb211_tuner_i2c_CommonWriteOneRegister;
    pI2c->gwAddress = 0;
    pI2c->gwSub = 0;
    pI2c->user = NULL;

    RB211_TRACE_I2C_RETURN(RB211_RESULT_OK);
}

rb211_result_t rb211_tuner_i2c_MTK_CreateI2cGw(rb211_tuner_i2c_t* pI2c, UINT8 gwAddress, UINT8 gwSub)
{
    RB211_TRACE_I2C_ENTER("rb211_tuner_i2c_MTK_CreateI2cGw");

    if(!pI2c){
        RB211_TRACE_I2C_RETURN(RB211_RESULT_ERROR_ARG);
    }

    pI2c->Read = rb211_tuner_i2c_MTK_ReadGw;// NOTE: This is not used.
    pI2c->Write = rb211_tuner_i2c_MTK_WriteGw;
    pI2c->ReadRegister = rb211_tuner_i2c_MTK_ReadRegisterGw;
    pI2c->WriteRegister = rb211_tuner_i2c_MTK_WriteRegisterGw;
    pI2c->WriteOneRegister = rb211_tuner_i2c_CommonWriteOneRegister;
    pI2c->gwAddress = gwAddress;
    pI2c->gwSub = gwSub;
    pI2c->user = NULL;

    RB211_TRACE_I2C_RETURN(RB211_RESULT_OK);
}

