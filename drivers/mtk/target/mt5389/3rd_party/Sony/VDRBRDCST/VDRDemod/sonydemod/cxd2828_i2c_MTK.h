/*------------------------------------------------------------------------------

 Copyright 2011 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2011/01/11
 Revision: 1.0.0.0

------------------------------------------------------------------------------*/

#ifndef CXD2828_I2C_MTK_H
#define CXD2828_I2C_MTK_H

#include "cxd2828_i2c.h"

cxd2828_result_t cxd2828_i2c_MTK_CreateI2c(cxd2828_i2c_t* pI2c);
cxd2828_result_t cxd2828_i2c_MTK_CreateI2cGw(cxd2828_i2c_t* pI2c, UINT8 gwAddress, UINT8 gwSub);

#endif /* CXD2828_I2C_MTK_H */
