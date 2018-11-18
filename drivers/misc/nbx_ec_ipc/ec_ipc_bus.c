/*
 * Copyright (C) 2012 Sony Corporation
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
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/mutex.h>
#include <linux/completion.h>
#include <asm/uaccess.h>
#include <linux/semaphore.h>

#include <linux/nbx_ec_ipc.h>

#define to_nbx_ec_ipc_device(d) container_of((d), struct nbx_ec_ipc_device, dev)
#define to_nbx_ec_ipc_driver(d) container_of((d), struct nbx_ec_ipc_driver, drv)

static int nbx_ec_ipc_bus_match(struct device* dev, struct device_driver* drv)
{
	int ret;
	struct nbx_ec_ipc_device* edev = to_nbx_ec_ipc_device(dev);

	ret = (strcmp(edev->name, drv->name) == 0);

	return ret;
}

static int nbx_ec_ipc_bus_probe(struct device* dev)
{
	int ret = 0;
	struct nbx_ec_ipc_device* edev = to_nbx_ec_ipc_device(dev);
	struct nbx_ec_ipc_driver* edrv = to_nbx_ec_ipc_driver(dev->driver);

	if(edrv->probe != NULL) {
		ret = (edrv->probe)(edev);
	}

	return ret;
}

static int nbx_ec_ipc_bus_remove(struct device* dev)
{
	int ret = 0;
	struct nbx_ec_ipc_device* edev = to_nbx_ec_ipc_device(dev);
	struct nbx_ec_ipc_driver* edrv = to_nbx_ec_ipc_driver(dev->driver);

	if(edrv->remove != NULL) {
		ret = (edrv->remove)(edev);
	}

	return ret;
}

static void nbx_ec_ipc_bus_shutdown(struct device* dev)
{
	struct nbx_ec_ipc_device* edev = to_nbx_ec_ipc_device(dev);
	struct nbx_ec_ipc_driver* edrv = to_nbx_ec_ipc_driver(dev->driver);

	if(edrv->shutdown != NULL) {
		(edrv->shutdown)(edev);
	}
}

#define get_device_pm_ops(d) (((d) != NULL) ? (((d)->driver != NULL) ? ((d)->driver->pm) : NULL) : NULL)

#ifdef CONFIG_PM_SLEEP

static int nbx_ec_ipc_bus_pm_prepare(struct device* dev)
{
	int ret = 0;
	struct nbx_ec_ipc_device* edev = to_nbx_ec_ipc_device(dev);
	struct nbx_ec_ipc_driver* edrv = to_nbx_ec_ipc_driver(dev->driver);
	const struct dev_pm_ops* pm = get_device_pm_ops(dev);

	if( (pm != NULL) && (pm->prepare != NULL) ){
		ret = (pm->prepare)(dev);
	}
	else if(edrv->prepare_suspend != NULL) {
		ret = (edrv->prepare_suspend)(edev);
	}

	return ret;
}

static void nbx_ec_ipc_bus_pm_complete(struct device* dev)
{
	struct nbx_ec_ipc_device* edev = to_nbx_ec_ipc_device(dev);
	struct nbx_ec_ipc_driver* edrv = to_nbx_ec_ipc_driver(dev->driver);
	const struct dev_pm_ops* pm = get_device_pm_ops(dev);

	if( (pm != NULL) && (pm->complete != NULL) ){
		(pm->complete)(dev);
	}
	else if(edrv->complete_resume != NULL) {
		(edrv->complete_resume)(edev);
	}
}

#else /* !CONFIG_PM_SLEEP */

#define nbx_ec_ipc_bus_pm_prepare NULL
#define nbx_ec_ipc_bus_pm_complete NULL

#endif /* CONFIG_PM_SLEEP */

#ifdef CONFIG_SUSPEND

static int nbx_ec_ipc_bus_pm_suspend(struct device* dev)
{
	int ret = 0;
	struct nbx_ec_ipc_device* edev = to_nbx_ec_ipc_device(dev);
	struct nbx_ec_ipc_driver* edrv = to_nbx_ec_ipc_driver(dev->driver);
	const struct dev_pm_ops* pm = get_device_pm_ops(dev);

	if( (pm != NULL) && (pm->suspend != NULL) ){
		ret = (pm->suspend)(dev);
	}
	else if(edrv->suspend != NULL) {
		ret = (edrv->suspend)(edev);
	}

	return ret;
}

static int nbx_ec_ipc_bus_pm_suspend_noirq(struct device* dev)
{
	int ret = 0;
	struct nbx_ec_ipc_device* edev = to_nbx_ec_ipc_device(dev);
	struct nbx_ec_ipc_driver* edrv = to_nbx_ec_ipc_driver(dev->driver);
	const struct dev_pm_ops* pm = get_device_pm_ops(dev);

	if( (pm != NULL) && (pm->suspend_noirq != NULL) ){
		ret = (pm->suspend_noirq)(dev);
	}
	else if(edrv->suspend_noirq != NULL) {
		ret = (edrv->suspend_noirq)(edev);
	}

	return ret;
}

static int nbx_ec_ipc_bus_pm_resume_noirq(struct device* dev)
{
	int ret = 0;
	struct nbx_ec_ipc_device* edev = to_nbx_ec_ipc_device(dev);
	struct nbx_ec_ipc_driver* edrv = to_nbx_ec_ipc_driver(dev->driver);
	const struct dev_pm_ops* pm = get_device_pm_ops(dev);

	if( (pm != NULL) && (pm->resume_noirq != NULL) ){
		ret = (pm->resume_noirq)(dev);
	}
	else if(edrv->resume_noirq != NULL) {
		ret = (edrv->resume_noirq)(edev);
	}

	return ret;
}

static int nbx_ec_ipc_bus_pm_resume(struct device* dev)
{
	int ret = 0;
	struct nbx_ec_ipc_device* edev = to_nbx_ec_ipc_device(dev);
	struct nbx_ec_ipc_driver* edrv = to_nbx_ec_ipc_driver(dev->driver);
	const struct dev_pm_ops* pm = get_device_pm_ops(dev);

	if( (pm != NULL) && (pm->resume != NULL) ){
		ret = (pm->resume)(dev);
	}
	else if(edrv->resume != NULL) {
		ret = (edrv->resume)(edev);
	}

	return ret;
}

#else /* !CONFIG_SUSPEND */

#define nbx_ec_ipc_bus_pm_suspend NULL
#define nbx_ec_ipc_bus_pm_suspend_noirq NULL
#define nbx_ec_ipc_bus_pm_resume_noirq NULL
#define nbx_ec_ipc_bus_pm_resume NULL

#endif /* CONFIG_SUSPEND */

static const struct dev_pm_ops nbx_ec_ipc_bus_pm_ops = {
	.prepare = nbx_ec_ipc_bus_pm_prepare,
	.suspend = nbx_ec_ipc_bus_pm_suspend,
	.suspend_noirq = nbx_ec_ipc_bus_pm_suspend_noirq,
	.resume_noirq = nbx_ec_ipc_bus_pm_resume_noirq,
	.resume = nbx_ec_ipc_bus_pm_resume,
	.complete = nbx_ec_ipc_bus_pm_complete,
};

struct bus_type nbx_ec_ipc_bus_type = {
	.name		= "nbx_ec_ipc",
	.match		= nbx_ec_ipc_bus_match,
	.probe		= nbx_ec_ipc_bus_probe,
	.remove		= nbx_ec_ipc_bus_remove,
	.shutdown	= nbx_ec_ipc_bus_shutdown,
	.pm		= &nbx_ec_ipc_bus_pm_ops,
};
EXPORT_SYMBOL(nbx_ec_ipc_bus_type);

static struct device* ec_ipc_core_device = NULL;

int nbx_ec_ipc_core_device_register(struct nbx_ec_ipc_device* edev)
{
	int ret;

	ec_ipc_core_device = &(edev->dev);
	ret = nbx_ec_ipc_device_register(edev);

	return ret;
}
EXPORT_SYMBOL(nbx_ec_ipc_core_device_register);

void nbx_ec_ipc_core_device_unregister(struct nbx_ec_ipc_device* edev)
{
	nbx_ec_ipc_device_unregister(edev);
	ec_ipc_core_device = NULL;
}
EXPORT_SYMBOL(nbx_ec_ipc_core_device_unregister);

int nbx_ec_ipc_device_register(struct nbx_ec_ipc_device* edev)
{
	int ret;
	struct device* dev;

	if( (edev == NULL) || (edev->name[0] == 0) ){
		return -EINVAL;
	}
	if(ec_ipc_core_device == NULL) {
		return -EAGAIN;
	}

	dev = &(edev->dev);
	if((dev->parent == NULL) && (dev != ec_ipc_core_device) ){
		dev->parent = ec_ipc_core_device;
	}
	dev->bus = &nbx_ec_ipc_bus_type;
	dev_set_name(dev, "%s", edev->name);

	ret = device_register(dev);

	pr_debug("registerd nbx_ec_ipc_device:%s (%d)\n", dev_name(dev), ret);

	return ret;
}
EXPORT_SYMBOL(nbx_ec_ipc_device_register);

void nbx_ec_ipc_device_unregister(struct nbx_ec_ipc_device* edev)
{
	if(edev != NULL) {
		device_unregister(&(edev->dev));
	}
}
EXPORT_SYMBOL(nbx_ec_ipc_device_unregister);

int nbx_ec_ipc_driver_register(struct nbx_ec_ipc_driver* edrv)
{
	int ret;

	if(edrv == NULL) {
		return -EINVAL;
	}

	edrv->drv.bus = &nbx_ec_ipc_bus_type;

	ret = driver_register(&(edrv->drv));

	pr_debug("registerd nbx_ec_ipc_driver:%s (%d)\n", edrv->drv.name, ret);

	return ret;
}
EXPORT_SYMBOL(nbx_ec_ipc_driver_register);

void nbx_ec_ipc_driver_unregister(struct nbx_ec_ipc_driver* edrv)
{
	if(edrv != NULL) {
		driver_unregister(&(edrv->drv));
	}
}
EXPORT_SYMBOL(nbx_ec_ipc_driver_unregister);

static int __init nbx_ec_ipc_bus_init(void)
{
	int ret;

	ret = bus_register(&nbx_ec_ipc_bus_type);

	return ret;
}
static void __exit nbx_ec_ipc_bus_exit(void)
{
	bus_unregister(&nbx_ec_ipc_bus_type);
}

subsys_initcall(nbx_ec_ipc_bus_init);
module_exit(nbx_ec_ipc_bus_exit);

MODULE_LICENSE("GPL");
