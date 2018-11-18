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
#include <linux/switch.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/nbx_dock.h>

#include <linux/nbx_ec_ipc_acplug.h>

static struct delayed_work *gwork = NULL;


/* 
 * sysfs interface for debug.(force setstate) 
 * (/sys/class/switch/dock/dbg) 
 */
static ssize_t
store_setstate(struct device *dev, struct device_attribute *attr, 
    const char *buf, size_t count)
{
	struct switch_dev *sdev = (struct switch_dev *)dev_get_drvdata(dev);
	unsigned int state;
	if (count < 1 || (state = buf[0] - '0') >= 2) {
		return -EINVAL;
	}
	switch_set_state(sdev, state);
	return count;	
}
static DEVICE_ATTR(dbg, 0200, NULL, store_setstate);

static void dock_setstate_work(struct delayed_work *work)
{
        struct nbx_dock_platform_data *pdata = 
	    container_of(work, struct nbx_dock_platform_data, work);
	int dock_state;

	if (!pdata->ac_state)
		dock_state = 0; /* open */	
	else if (gpio_get_value(pdata->dockdet_pin))
		dock_state = 1; /* dock */	
	else
		dock_state = 0; /* open */	

	switch_set_state(&pdata->sdev, dock_state);
}

static void
nbx_dock_ac_state_changed(void)
{
	int ac_state;

	ac_state = nbx_ec_ipc_acplug_get_state();
	if(ac_state < 0) return;

	if (gwork) {
		struct nbx_dock_platform_data *pdata = 
		    container_of(gwork, struct nbx_dock_platform_data, work);
		if (pdata->ac_state != ac_state) {
			cancel_delayed_work_sync(gwork);
			pdata->ac_state = ac_state;
			schedule_delayed_work(gwork, HZ / 10);
		}
	}

}


static int __init
dock_probe(struct platform_device *pdev)
{
	struct nbx_dock_platform_data *pdata;
	int ret;

	pdata = (struct nbx_dock_platform_data *)pdev->dev.platform_data;

	/* initialize workqueue for sending event */
	INIT_DELAYED_WORK(&pdata->work, dock_setstate_work);

	/* switch class register */
	pdata->sdev.name = "dock";
	pdata->sdev.print_name = NULL;
	pdata->sdev.print_state = NULL;
	if ((ret = switch_dev_register(&pdata->sdev)) < 0)
		goto err0;
	/* add sysfs file for debug */
	if ((ret = device_create_file(pdata->sdev.dev, &dev_attr_dbg)) < 0)
		goto err1;

	tegra_gpio_enable(pdata->dockdet_pin);

	/* get gpio for dock detection */
	if ((ret = gpio_request(pdata->dockdet_pin, "dock det")) < 0) {
		goto err2;
	}

	gpio_direction_input(pdata->dockdet_pin);

	gwork = &pdata->work;

	nbx_ec_ipc_acplug_register_callback(nbx_dock_ac_state_changed);
	nbx_dock_ac_state_changed();

	return 0;

err2:
	device_remove_file(pdata->sdev.dev, &dev_attr_dbg);
err1:
	switch_dev_unregister(&pdata->sdev);
err0:
	return ret;
}

static int __exit
dock_remove(struct platform_device *pdev)
{
	struct nbx_dock_platform_data *pdata;

	pdata = (struct nbx_dock_platform_data *)pdev->dev.platform_data;

	cancel_delayed_work_sync(gwork);
	gwork = NULL;

	switch_dev_unregister(&pdata->sdev);

	gpio_free(pdata->dockdet_pin);

	device_remove_file(pdata->sdev.dev, &dev_attr_dbg);

	nbx_ec_ipc_acplug_unregister_callback(nbx_dock_ac_state_changed);

	return 0;
}

static struct platform_driver dock_driver = {
	.probe = dock_probe,
	.remove = __exit_p(dock_remove),
	.driver = {
		.name = "nbx_dock",
		.owner = THIS_MODULE
	}
};

static int __init dock_init(void)
{
	return platform_driver_register(&dock_driver);
}
module_init(dock_init);

static void __exit dock_exit(void)
{
	return platform_driver_unregister(&dock_driver);
}
module_exit(dock_exit);


