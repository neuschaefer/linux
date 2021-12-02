/*------------------------------------------------------------------------------

 Copyright 2009, 2010 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2010/03/19
 Revision: 1.0.0.0

------------------------------------------------------------------------------*/

#ifndef CXD2828_I2C_H
#define CXD2828_I2C_H

#include "cxd2828_common.h"

#define CXD2828_I2C_START_EN   (0x01)  /* Output START (S) */
#define CXD2828_I2C_STOP_EN    (0x02)  /* Output STOP  (P) */

typedef struct cxd2828_i2c_t {
    /*--------------------------------------------------------------------
      Read/Write bytes from/to the I2C device.

      cxd2828_i2c_t*       pI2c          : Instance of I2C control struct
      UINT8           deviceAddress : The device address.
      UINT8           subAddress    : The sub address.
      UINT8           *pData        : The buffer to store the read from.
      UINT32          size          : The number of bytes to read from the I2C device.
      UINT8           mode          : The I2C mode (whether to output START/STOP conditions).
    --------------------------------------------------------------------*/
    cxd2828_result_t (*Read)(struct cxd2828_i2c_t* pI2c, UINT8 deviceAddress, UINT8* pData, UINT32 size, UINT8 mode);
    cxd2828_result_t (*Write)(struct cxd2828_i2c_t* pI2c, UINT8 deviceAddress, const UINT8 *pData, UINT32 size, UINT8 mode);
    cxd2828_result_t (*ReadRegister)(struct cxd2828_i2c_t* pI2c, UINT8 deviceAddress, UINT8 subAddress, UINT8* pData, UINT32 size);
    cxd2828_result_t (*WriteRegister)(struct cxd2828_i2c_t* pI2c, UINT8 deviceAddress, UINT8 subAddress, const UINT8 *pData, UINT32 size);
    /* Should be implemented using WriteRegister function */
    cxd2828_result_t (*WriteOneRegister)(struct cxd2828_i2c_t* pI2c, UINT8 deviceAddress, UINT8 subAddress, UINT8 data);

    UINT8          gwAddress; /* (Only for gateway access) Gateway address (Demod device address) */
    UINT8          gwSub;     /* (Only for gateway access) Gateway sub address */

    UINT32         flags;     /* Flags that can be used by i2c code */
    void*            user;      /* User defined */
} cxd2828_i2c_t;

/*------------------------------------------------------------------------------
  I2C help functions
------------------------------------------------------------------------------*/
/* Common ReadRegister/WriteRegister/WriteOneRegister function implementation using Read/Write functions */
cxd2828_result_t cxd2828_i2c_CommonReadRegister(cxd2828_i2c_t* pI2c, UINT8 deviceAddress, UINT8 subAddress, UINT8* pData, UINT32 size);
cxd2828_result_t cxd2828_i2c_CommonWriteRegister(cxd2828_i2c_t* pI2c, UINT8 deviceAddress, UINT8 subAddress, const UINT8* pData, UINT32 size);
cxd2828_result_t cxd2828_i2c_CommonWriteOneRegister(cxd2828_i2c_t* pI2c, UINT8 deviceAddress, UINT8 subAddress, UINT8 data);

/* For Read-Modify-Write */
cxd2828_result_t cxd2828_i2c_SetRegisterBits(cxd2828_i2c_t* pI2c, UINT8 deviceAddress, UINT8 subAddress, UINT8 data, UINT8 mask);

#endif /* CXD2828_I2C_H */
