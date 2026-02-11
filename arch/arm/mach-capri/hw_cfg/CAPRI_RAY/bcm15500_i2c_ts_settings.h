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

#ifndef BCM15500_I2C_TS_SETTINGS_H
#define BCM15500_I2C_TS_SETTINGS_H

/*
 * Refer to include/linux/i2c/egalax_i2c_ts.h for details
 */

#ifdef CONFIG_CAPRI_11130
#define HW_BCM15500_I2C_BUS_ID      0
#else
#define HW_BCM15500_I2C_BUS_ID      1
#endif

/* to signal the host processor for touch events */
#define HW_BCM15500_GPIO_INTERRUPT  1

/* reset pin */
#define HW_BCM15500_GPIO_RESET    -1

#endif /* EGALAX_I2C_TS_SETTINGS_H */
