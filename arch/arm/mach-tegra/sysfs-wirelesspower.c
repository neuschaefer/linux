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
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include "wireless_power_control.h"

//#define SYSFS_WIRELESSPOWER_DEBUG_PRINTS 1

#ifdef SYSFS_WIRELESSPOWER_DEBUG_PRINTS
#define PRINT_WIRELESSPOWER(x) pr_debug x
#else
#define PRINT_WIRELESSPOWER(x)
#endif

static ssize_t sysfswirelesspower_show(struct kobject *kobj,
				struct kobj_attribute *attr, char *buf);

static ssize_t sysfswirelesspower_store(struct kobject *kobj,
					struct kobj_attribute *attr, const char *buf, size_t count);


static struct kobj_attribute wirelesspower_gps_attr =
	__ATTR(gps, 0400, sysfswirelesspower_show, sysfswirelesspower_store);

// return the module type based on the module name.
static int GetModuleType(const char *str)
{
	WPCModuleType type;
	
	if (!strcmp(str, "gps")) {
		type = WPC_MODULE_GPS;
	} else {
		type = WPC_MODULE_NONE;
		PRINT_WIRELESSPOWER(("\r\n Invalid Module type:%s... Find out reason\r",str));
	}
	return type;
}

static ssize_t sysfswirelesspower_show(struct kobject *kobj,
				struct kobj_attribute *attr, char *buf)
{
	int on_off;
	int err;
	WPCModuleType ModuleType;
	
	ModuleType = GetModuleType(attr->attr.name);
	
	err = wireless_power_status(ModuleType, &on_off);
	if (err < 0) {
		PRINT_WIRELESSPOWER(("wireless_power_status failed err=%d ModuleType=%d\n", err, ModuleType));
		return err;
	}
	PRINT_WIRELESSPOWER(("on_off=%d\n", on_off));
	snprintf(buf,PAGE_SIZE,"%d\n",on_off);
	return (strlen(buf));
}

static ssize_t sysfswirelesspower_store(struct kobject *kobj,
					struct kobj_attribute *attr, const char *buf, size_t count)
{
	WPCModuleType ModuleType;
	int err;
	int on_off;
	
	ModuleType = GetModuleType(attr->attr.name);
	
	if (buf==NULL || count == 0) {
		PRINT_WIRELESSPOWER(("Invalid Argument\n"));
		return -EINVAL;
	}
	
	if (strncmp(buf, "0", 1) == 0) {
		on_off = 0;
	} else if (strncmp(buf, "1", 1) == 0) {
		on_off = 1;
	} else {
		PRINT_WIRELESSPOWER(("Invalid data in ssize_t sysfswirelesspower_store_1\n"));
		return -EINVAL;
	} 
	
	err = wireless_power_control(ModuleType, on_off);
	
	if (err < 0) {
		PRINT_WIRELESSPOWER(("wireless_power_control failed %d\n", err));
		return err;
	}
	
	return count;
}

static struct attribute *wirelesspower_attrs[] = {
	&wirelesspower_gps_attr.attr,
	NULL
};

static const struct attribute_group wirelesspower_sysfs_files = {
	.attrs	= wirelesspower_attrs,
};

static int wirelesspower_probe(struct platform_device *pdev)
{
	int err;
	PRINT_WIRELESSPOWER(("in wirelesspower_probbe\n"));
	err = sysfs_create_group(&pdev->dev.kobj, &wirelesspower_sysfs_files);
	if (err)
		goto fail_no_sysfs;
	return 0;
fail_no_sysfs:
	return err;
}

static int wirelesspower_remove(struct platform_device *pdev)
{
	PRINT_WIRELESSPOWER(("in wirelesspower_remove\n"));
	sysfs_remove_group(&pdev->dev.kobj, &wirelesspower_sysfs_files);
	return 0;
}

static struct platform_driver wirelesspower_driver = {
	.probe  = wirelesspower_probe,
	.remove = wirelesspower_remove,
	.driver = {
		.name  = "wirelesspower",
		.owner = THIS_MODULE,
	},
};

static struct platform_device* wirelesspower_dev;

static int __init wirelesspower_init(void)
{
	int ret;
	PRINT_WIRELESSPOWER(("in wirelesspower_init\n"));
	ret = platform_driver_register(&wirelesspower_driver);
	if (ret < 0)
		return ret;
	
	wirelesspower_dev = platform_device_register_simple("wirelesspower", -1, NULL, 0);
	if (IS_ERR(wirelesspower_dev)) {
		ret = PTR_ERR(wirelesspower_dev);
		platform_driver_unregister(&wirelesspower_driver);
	}
	return ret;
}

static void __exit wirelesspower_exit(void)
{
	PRINT_WIRELESSPOWER(("in wirelesspower_exit\n"));
	if ( wirelesspower_dev ) {
		platform_device_unregister(wirelesspower_dev);
		wirelesspower_dev = NULL;
	}
	platform_driver_unregister(&wirelesspower_driver);
}

module_init(wirelesspower_init);
module_exit(wirelesspower_exit);
MODULE_LICENSE("GPL");

