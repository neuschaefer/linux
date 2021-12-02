/*------------------------------------------------------------------------------

 Copyright 2011 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2011/01/11
 Revision: 1.0.0.0

------------------------------------------------------------------------------*/

#ifndef RB211_I2C_MTK_H
#define RB211_I2C_MTK_H

#include "rb211_tuner_i2c.h"

rb211_result_t rb211_tuner_i2c_MTK_CreateI2c(rb211_tuner_i2c_t* pI2c);
rb211_result_t rb211_tuner_i2c_MTK_CreateI2cGw(rb211_tuner_i2c_t* pI2c, UINT8 gwAddress, UINT8 gwSub);

#endif /* RB211_I2C_MTK_H */
