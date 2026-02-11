/*****************************************************************************
/*****************************************************************************
* Copyright 2012 Broadcom Corporation.  All rights reserved.
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

#ifndef BCM915500_I2C_TS_H
#define BCM915500_I2C_TS_H

#define BCM15500_TSC_NAME "bcm15500"

/* ---------------------------------------------------------- */
/* - (Host) Platform Specific Definitions and Platform Data - */
/* ---------------------------------------------------------- */

/*
 * host I2C bus/adapter id that the BCM Touch Controller is on
 * - MUST BE SET
 */
#define BCMTCH_HW_I2C_BUS_ID            1

/*
 * host gpio pin used to reset the BCM Touch Controller
 * - MUST be set to -1 if not used
 */
#define BCMTCH_HW_GPIO_RESET_PIN        0

#define BCMTCH_HW_POLARITY_ACTIVE_HIGH  1
#define BCMTCH_HW_POLARITY_ACTIVE_LOW   0

#define BCMTCH_HW_GPIO_RESET_POLARITY   BCMTCH_HW_POLARITY_ACTIVE_LOW
#define BCMTCH_HW_GPIO_RESET_TIME_MS    100

/*
 * host gpio pin used to receive interrupts from the BCM Touch Controller
 * - MUST be set to -1 if not used
 */
#define BCMTCH_HW_GPIO_INTERRUPT_PIN        1
#define BCMTCH_HW_GPIO_INTERRUPT_TRIGGER    IRQF_TRIGGER_FALLING

/* I2C addresses of the BCM Touch Controller */
#define BCMTCH_HW_I2C_ADDR_SPM      0x66 /* SPM address on I2C bus */
#define BCMTCH_HW_I2C_ADDR_SYS      0x68 /* SYS/AHB address on I2C bus */

/* orientation of the LCD/touch panel to the BCM Touch Controller */
#define BCMTCH_HW_AXIS_SWAP_X_Y     0   /* swap x <-> y axes */
#define BCMTCH_HW_AXIS_SWAP_Y       0   /* swap x-axis */
#define BCMTCH_HW_AXIS_SWAP_X       0   /* swap y-axis */

/* structure for BCM Touch Controller platform hardware configuration */
struct bcmtch_platform_data {
	int i2c_bus_id;
	int i2c_addr_sys;
	int gpio_reset_pin;
	int gpio_interrupt_pin;
	int gpio_interrupt_trigger;
	int gpio_reset_polarity;
	int gpio_reset_time_ms;
};
#define bcmtch_platform_data_t struct bcmtch_platform_data

#endif
