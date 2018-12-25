/* include/linux/i2c/elan_ektf2k.h - ELAN EKTF2136 touchscreen driver
 *
 * Copyright (C) 2011 Elan Microelectronics Corporation.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#define SCREEN_MAX_HEIGHT 576
#define SCREEN_MAX_WIDTH  960

#define ELAN_X_MAX 320//1186	

#define ELAN_Y_MAX 640//1792


#ifndef _LINUX_ELAN_KTF2K_H
#define _LINUX_ELAN_KTF2K_H

#define ELAN_KTF2K_NAME "elan_ts_d"

struct elan_ktf2k_i2c_platform_data {
	uint16_t version;
	int abs_x_min;
	int abs_x_max;
	int abs_y_min;
	int abs_y_max;
	int intr_gpio;
	//int (*power)(int on);
	//int rst_gpio;
};

#endif /* _LINUX_ELAN_KTF2K_H */

