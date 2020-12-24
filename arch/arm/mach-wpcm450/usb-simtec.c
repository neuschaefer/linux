/* linux/arch/arm/mach-wpcm450/usb-simtec.c
 *
 * Copyright (c) 2004,2005 Simtec Electronics
 *   Ben Dooks <ben@simtec.co.uk>
 *
 * http://www.simtec.co.uk/products/EB2410ITX/
 *
 * Simtec BAST and Thorcom VR1000 USB port support functions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *	14-Sep-2004 BJD  Created
 *	18-Oct-2004 BJD  Cleanups, and added code to report OC cleared
 *	09-Aug-2005 BJD  Renamed wpcm450_report_oc to wpcm450_usb_report_oc
 *	09-Aug-2005 BJD  Ports powered only if both are enabled
*/

#define DEBUG

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/device.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include <asm/arch/bast-map.h>
#include <asm/arch/bast-irq.h>
#include <asm/arch/usb-control.h>
#include <asm/arch/regs-gpio.h>

#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/irq.h>

#include "devs.h"
#include "usb-simtec.h"

/* control power and monitor over-current events on various Simtec
 * designed boards.
*/

static unsigned int power_state[2];

static void
usb_simtec_powercontrol(int port, int to)
{
	pr_debug("usb_simtec_powercontrol(%d,%d)\n", port, to);

	power_state[port] = to;

	if (power_state[0] && power_state[1])
		wpcm450_gpio_setpin(wpcm450_GPB4, 0);
	else
		wpcm450_gpio_setpin(wpcm450_GPB4, 1);
}

static irqreturn_t
usb_simtec_ocirq(int irq, void *pw, struct pt_regs *regs)
{
	struct wpcm450_hcd_info *info = (struct wpcm450_hcd_info *)pw;

	if (wpcm450_gpio_getpin(wpcm450_GPG10) == 0) {
		pr_debug("usb_simtec: over-current irq (oc detected)\n");
		wpcm450_usb_report_oc(info, 3);
	} else {
		pr_debug("usb_simtec: over-current irq (oc cleared)\n");
		wpcm450_usb_report_oc(info, 0);
	}

	return IRQ_HANDLED;
}

static void usb_simtec_enableoc(struct wpcm450_hcd_info *info, int on)
{
	int ret;

	if (on) {
		ret = request_irq(IRQ_USBOC, usb_simtec_ocirq,
				  SA_INTERRUPT | SA_TRIGGER_RISING |
				   SA_TRIGGER_FALLING,
				  "USB Over-current", info);
		if (ret != 0) {
			printk(KERN_ERR "failed to request usb oc irq\n");
		}
	} else {
		free_irq(IRQ_USBOC, info);
	}
}

static struct wpcm450_hcd_info usb_simtec_info = {
	.port[0]	= {
		.flags	= S3C_HCDFLG_USED
	},
	.port[1]	= {
		.flags	= S3C_HCDFLG_USED
	},

	.power_control	= usb_simtec_powercontrol,
	.enable_oc	= usb_simtec_enableoc,
};


int usb_simtec_init(void)
{
	printk("USB Power Control, (c) 2004 Simtec Electronics\n");
	s3c_device_usb.dev.platform_data = &usb_simtec_info;

	wpcm450_gpio_cfgpin(wpcm450_GPB4, wpcm450_GPB4_OUTP);
	wpcm450_gpio_setpin(wpcm450_GPB4, 1);
	return 0;
}
