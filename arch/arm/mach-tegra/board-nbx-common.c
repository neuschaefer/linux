/*
 * Copyright (C) 2011-2012 Sony Corporation
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
 * published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#include <linux/kernel.h>
#include <linux/init.h>

#include "board-nbx-common.h"

static u32 s_odmdata;

static int __init nbx_odmdata_arg(char *options)
{
	char *p = options;

	s_odmdata = (u32)simple_strtoul(p, NULL, 0);

	pr_info("odmdata=%08x\n", s_odmdata);

	return 0;
}
early_param("odmdata", nbx_odmdata_arg);

u32 nbx_get_odmdata()
{
	return s_odmdata;


}

// NBXTODO:should be cared by ec_ipc driver
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <mach/gpio.h>
#include "gpio-names.h"

static irqreturn_t ecipc_irq(int irq, void *dev_id)
{
	/* dummy */
	return IRQ_HANDLED;
}
void nbx_ecipc_irq_setup(int gpio_num, int irqf_trigger)
{
	int err;
	int irq_num = gpio_to_irq(gpio_num);
	tegra_gpio_enable(gpio_num);
	err = gpio_request(gpio_num, "ECIPC interrupt");
	if (err < 0) {
		pr_err("%s - gpio request ECIPC_IRQ failed\n", __func__);
		goto errout;
	}
	err = gpio_direction_input(gpio_num);
	if (err < 0) {
		pr_err("%s - gpio direction input ECIPC_IRQ failed\n", __func__);
		goto errout;
	}
	err = request_irq(irq_num,
			ecipc_irq,
			irqf_trigger,
			"ECIPC_IRQ",
			NULL);
	if (err < 0) {
		pr_err("%s - request irq ECIPC_IRQ failed\n", __func__);
		goto errout;
	}
	irq_set_irq_wake(irq_num, 1);

	//enable_irq(irq_num);
errout:
	;
}
