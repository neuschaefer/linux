/*****************************************************************************
* Copyright 2011 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a
* license other than the GPL, without Broadcom's express prior written
* consent.
*****************************************************************************/

#ifndef LSM330_I2C_SETTINGS_H
#define LSM330_I2C_SETTINGS_H

#define LSM330_I2C_BUS_ID 0
#define LSM330_I2C_ACC_ADDR 0x18
#define LSM330_I2C_GYRO_ADDR 0x6A

#define LSM330_ACC_DATA \
{ \
	.g_range = LSM330DLC_ACC_G_4G, \
 \
	.axis_map_x = 0, \
	.axis_map_y = 1, \
	.axis_map_z = 2, \
 \
	.negate_x = 1, \
	.negate_y = 1, \
	.negate_z = 0, \
 \
	.gpio_int1 = -1, \
	.gpio_int2 = -1, \
}

#define LSM330_GYRO_DATA \
{ \
	.fs_range = LSM330DLC_GYR_FS_2000DPS, \
 \
	.axis_map_x = 0, \
	.axis_map_y = 1, \
	.axis_map_z = 2, \
 \
	.negate_x = 1, \
	.negate_y = 1, \
	.negate_z = 0, \
 \
	.gpio_int1 = -1, \
	.gpio_int2 = -1, \
}

#endif /* LSM330_I2C_SETTINGS_H */
