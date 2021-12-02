/*------------------------------------------------------------------------------

 Copyright 2011 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2011/01/11
 Revision: 1.0.0.0

------------------------------------------------------------------------------*/

#ifndef SONY_I2C_MTK_H
#define SONY_I2C_MTK_H

#include "sony_i2c.h"

sony_result_t sony_i2c_MTK_CreateI2c(sony_i2c_t* pI2c);
sony_result_t sony_i2c_MTK_CreateI2cGw(sony_i2c_t* pI2c, uint8_t gwAddress, uint8_t gwSub);

#endif /* SONY_I2C_MTK_H */
