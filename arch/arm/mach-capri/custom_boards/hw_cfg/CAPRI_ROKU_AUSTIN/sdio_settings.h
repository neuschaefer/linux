/*****************************************************************************
* Copyright 2010 - 2011 Broadcom Corporation.  All rights reserved.
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

#ifndef SDIO_SETTINGS_H
#define SDIO_SETTINGS_H

/*
 * Refer to mach/sdio_platform.h for details
 */


/*
 * #define HW_WLAN_GPIO_RESET_PIN  	
 * #define HW_WLAN_GPIO_HOST_WAKE_PIN	
 * #define HW_WLAN_GPIO_SHUTDOWN_PIN
 * #define HW_WLAN_GPIO_REG_PIN
 */


/*
 * HW_SDIO_PARAM defines the array of the struct sdio_platform_cfg data
 * structure, with each element in the array representing a SDIO device setting
 */

/* primary eMMC and NAND cannot coexist */
#define HW_SDIO_PARAM \
{ \
	{ /* SDIO4 */ \
		.id = 3, \
		.peri_clk_rate = 48000000, \
		.cd_gpio = 129, \
		.wp_gpio = -1, \
		.devtype = SDIO_DEV_TYPE_SDMMC, \
	}, \
}

#endif /* SDIO_SETTINGS_H */
