/*
 * Copyright (C) 2010-2012 Sony Corporation
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
#include <linux/module.h>
#include <linux/init.h>
#include <linux/rfkill.h>
#include <linux/platform_device.h>
#include "wireless_power_control.h" 

static int nbx_rfkill_set_power(void *data, bool blocked)
{
	int ret;

	if (!blocked) {
		ret = wireless_power_control(WPC_MODULE_BLUETOOTH, 1);
		if (ret < 0) 
			pr_err("%s: power on failed\n", __func__);

	} else {
		ret = wireless_power_control(WPC_MODULE_BLUETOOTH, 0);
		if (ret < 0) 
			pr_err("%s: power off failed\n", __func__);
	}
	return ret;
}

static struct rfkill_ops nbx_rfkill_ops = {
	.set_block = nbx_rfkill_set_power,
};

static int nbx_rfkill_probe(struct platform_device *pdev)
{
	struct rfkill *rfkill;

	int rc;

	rfkill = rfkill_alloc("nbx-rfkill", &pdev->dev,
			RFKILL_TYPE_BLUETOOTH, &nbx_rfkill_ops, pdev);
	if (!rfkill) {
		pr_err("%s: rfkill allocation failed\n", __func__);
		rc = -ENOMEM;
		return rc;
	}
	platform_set_drvdata(pdev, rfkill);
	
	rc = rfkill_register(rfkill);
	if (rc < 0) {
		pr_err("%s: rfkill registration failed\n", __func__);
		rfkill_destroy(rfkill);
		return rc;
	}

	return 0;
}

static int nbx_rfkill_remove(struct platform_device *pdev)
{
	struct rfkill *rfkill = platform_get_drvdata(pdev);

	rfkill_unregister(rfkill);
	rfkill_destroy(rfkill);

	return 0;
	
}

static struct platform_driver nbx_rfkill_driver = {
	.probe = nbx_rfkill_probe,
	.remove = nbx_rfkill_remove,
	.driver = {
		.name = "nbx-rfkill",
		.owner = THIS_MODULE,
	},
};

static struct platform_device nbx_device = {
	.name = "nbx-rfkill",
};

static int __init nbx_rfkill_init(void)
{
	int ret;

	ret = platform_driver_register(&nbx_rfkill_driver);	
	if (ret < 0) {
		pr_err("%s: driver registration failed\n", __func__);
		return ret;
	}

	ret = platform_device_register(&nbx_device);
	if (ret < 0) {	
		pr_err("%s: device registration failed\n", __func__);
		platform_driver_unregister(&nbx_rfkill_driver);
	}

	return ret;
}

static void __exit nbx_rfkill_exit(void)
{
	platform_driver_unregister(&nbx_rfkill_driver);
}

module_init(nbx_rfkill_init);
module_exit(nbx_rfkill_exit);

MODULE_DESCRIPTION("NBX rfkill");
MODULE_AUTHOR("Sony");
MODULE_LICENSE("GPL");
