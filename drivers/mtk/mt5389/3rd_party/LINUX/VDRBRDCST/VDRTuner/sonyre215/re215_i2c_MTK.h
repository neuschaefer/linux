/*------------------------------------------------------------------------------

 Copyright 2011 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2011/01/11
 Revision: 1.0.0.0

------------------------------------------------------------------------------*/

#ifndef RE215_I2C_MTK_H
#define RE215_I2C_MTK_H

#include "re215_i2c.h"

re215_result_t re215_i2c_MTK_CreateI2c(re215_i2c_t* pI2c);
re215_result_t re215_i2c_MTK_CreateI2cGw(re215_i2c_t* pI2c, uint8_t gwAddress, uint8_t gwSub);

#endif /* RE215_I2C_MTK_H */
