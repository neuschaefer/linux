/*------------------------------------------------------------------------------

 Copyright 2009-2010 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2010/06/04
 Revision: 1.0.0.0

------------------------------------------------------------------------------*/

#ifndef RE215_I2C_H
#define RE215_I2C_H

#include "re215_common.h"

#define RE215_I2C_START_EN   (0x01)  /* Output START (S) */
#define RE215_I2C_STOP_EN    (0x02)  /* Output STOP  (P) */

typedef struct re215_i2c_t {
	/*--------------------------------------------------------------------
	  Read/Write bytes from/to the I2C device.

	  re215_i2c_t*       pI2c          : Instance of I2C control struct
	  uint8_t           deviceAddress : The device address.
	  uint8_t           subAddress    : The sub address.
	  uint8_t           *pData        : The buffer to store the read from.
	  uint32_t          size          : The number of bytes to read from the I2C device.
	  uint8_t           mode          : The I2C mode (whether to output START/STOP conditions).
	--------------------------------------------------------------------*/
	re215_result_t (*Read)(struct re215_i2c_t* pI2c, uint8_t deviceAddress, uint8_t* pData, uint32_t size, uint8_t mode);
	re215_result_t (*Write)(struct re215_i2c_t* pI2c, uint8_t deviceAddress, const uint8_t *pData, uint32_t size, uint8_t mode);
	re215_result_t (*ReadRegister)(struct re215_i2c_t* pI2c, uint8_t deviceAddress, uint8_t subAddress, uint8_t* pData, uint32_t size);
	re215_result_t (*WriteRegister)(struct re215_i2c_t* pI2c, uint8_t deviceAddress, uint8_t subAddress, const uint8_t *pData, uint32_t size);
	/* Should be implemented using WriteRegister function */
	re215_result_t (*WriteOneRegister)(struct re215_i2c_t* pI2c, uint8_t deviceAddress, uint8_t subAddress, uint8_t data);

	uint8_t          gwAddress; /* (Only for gateway access) Gateway address (Demod device address) */
	uint8_t          gwSub;     /* (Only for gateway access) Gateway sub address */

	uint32_t         flags;     /* Flags that can be used by i2c code */
	void*            user;      /* User defined */
} re215_i2c_t;

/*------------------------------------------------------------------------------
  I2C help functions
------------------------------------------------------------------------------*/
/* Common ReadRegister/WriteRegister/WriteOneRegister function implementation using Read/Write functions */
re215_result_t re215_i2c_CommonReadRegister(re215_i2c_t* pI2c, uint8_t deviceAddress, uint8_t subAddress, uint8_t* pData, uint32_t size);
re215_result_t re215_i2c_CommonWriteRegister(re215_i2c_t* pI2c, uint8_t deviceAddress, uint8_t subAddress, const uint8_t* pData, uint32_t size);
re215_result_t re215_i2c_CommonWriteOneRegister(re215_i2c_t* pI2c, uint8_t deviceAddress, uint8_t subAddress, uint8_t data);

/* For Read-Modify-Write */
re215_result_t re215_i2c_SetRegisterBits(re215_i2c_t* pI2c, uint8_t deviceAddress, uint8_t subAddress, uint8_t data, uint8_t mask);

#endif /* RE215_I2C_H */
