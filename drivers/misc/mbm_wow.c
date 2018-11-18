/* 2012-04-12: File added and changed by Sony Corporation */
/*
 * drivers/misc/mbm_wow.c
 *
 * Keyboard class input driver for the NVIDIA Tegra SoC internal matrix
 * keyboard controller
 *
 * Copyright (c) 2009-2010, NVIDIA Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/* #define RM_SUPPORT */

#include <linux/module.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/clk.h>
#include <linux/irq.h>
#include <mach/clk.h>

extern pid_t s_ril_pid;

#ifdef CONFIG_PM
static int mbm_wow_suspend(struct platform_device *pdev, pm_message_t state)
{
    if(s_ril_pid) {
        int irq;

        irq = platform_get_irq(pdev, 0);
        if(irq < 0) {
            dev_err(&pdev->dev, "FAILED to get wake IRQ\n");
            return -ENXIO;
        }
        irq_set_irq_type(irq, IRQ_TYPE_LEVEL_LOW);
        enable_irq_wake(irq);
    }

    return 0;
}

static int mbm_wow_resume(struct platform_device *pdev)
{
    if(s_ril_pid) {
        int irq;

        irq = platform_get_irq(pdev, 0);
        if(irq < 0) {
            dev_err(&pdev->dev, "FAILED to get wake IRQ\n");
            return -ENXIO;
        }
        disable_irq_wake(irq);
    }

    return 0;
}
#endif

static struct platform_driver mbm_wow_driver = {
#ifdef CONFIG_PM
    .suspend = mbm_wow_suspend,
    .resume = mbm_wow_resume,
#endif
    .driver = {
        .name = "mbm_wow"
    }
};

static void __exit mbm_wow_exit(void)
{
	platform_driver_unregister(&mbm_wow_driver);
}

static int __devinit mbm_wow_init(void)
{
    printk("%s called\n", __func__);
    return platform_driver_register(&mbm_wow_driver);
}

module_exit(mbm_wow_exit);
module_init(mbm_wow_init);

MODULE_DESCRIPTION("mbm wake pin control driver");
