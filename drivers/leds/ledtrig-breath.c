/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/*
 * LED Kernel Breath Trigger
 *
 * Copyright 2005-2006 Openedhand Ltd.
 *
 * Author: Richard Purdie <rpurdie@openedhand.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/ctype.h>
#include <linux/leds.h>
#include "leds.h"

int breath_enable = 0;
EXPORT_SYMBOL(breath_enable);
int breath_ledn = 0;
EXPORT_SYMBOL(breath_ledn);
int breath_on_delay = 10000;
EXPORT_SYMBOL(breath_on_delay);
int breath_off_delay = 20000;
EXPORT_SYMBOL(breath_off_delay);
int kDutyMaxThresold = 120;
EXPORT_SYMBOL(kDutyMaxThresold);

struct gpio_led_trace
{
    struct led_classdev cdev;
    unsigned gpio;
};

static ssize_t led_breath_on_delay_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);

	return 0;
}

static ssize_t led_breath_on_delay_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	int ret = -EINVAL;
	char *after;
	unsigned long state = simple_strtoul(buf, &after, 10);
	size_t count = after - buf;

	if (isspace(*after))
		count++;

	if (count == size) {
 		breath_on_delay = state;
		ret = count;
	}

	return ret;
}

static ssize_t led_breath_off_delay_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);

	return 0;
}


static ssize_t led_breath_off_delay_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	int ret = -EINVAL;
	char *after;
	unsigned long state = simple_strtoul(buf, &after, 10);
	size_t count = after - buf;

	if (isspace(*after))
		count++;

	if (count == size) {
		breath_off_delay = state;
		ret = count;
	}

	return ret;
}

static ssize_t led_breath_enable_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);

	return 0;
}

static ssize_t led_breath_enable_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	int ret = -EINVAL;
	char *after;
	unsigned long state = simple_strtoul(buf, &after, 10);
	size_t count = after - buf;

	if (isspace(*after))
		count++;

	if (count == size) {
 		breath_enable = state;
		ret = count;
	}

	return ret;
}

static DEVICE_ATTR(breath_on_delay, 0644, led_breath_on_delay_show, led_breath_on_delay_store);
static DEVICE_ATTR(breath_off_delay, 0644, led_breath_off_delay_show, led_breath_off_delay_store);
static DEVICE_ATTR(breath_enable, 0644, led_breath_enable_show, led_breath_enable_store);

extern void  hi_kernel_breath_timer_enable(int enable);

static void breath_trig_activate(struct led_classdev *led_cdev)
{
	struct gpio_led_trace *led_dat =
                    container_of(led_cdev, struct gpio_led_trace, cdev);
	int rc;	

	led_cdev->trigger_data = NULL;

	rc = device_create_file(led_cdev->dev, &dev_attr_breath_on_delay);
	if (rc)
		return;
	rc = device_create_file(led_cdev->dev, &dev_attr_breath_off_delay);
	if (rc)
		return;
	rc = device_create_file(led_cdev->dev, &dev_attr_breath_enable);
	if (rc)
		goto err_out_delayon;

	breath_ledn = led_dat->gpio;
	kDutyMaxThresold = led_cdev->breath_speed;
	hi_kernel_breath_timer_enable(1);

	led_cdev->trigger_data = (void *)1;

	return;

err_out_delayon:
	device_remove_file(led_cdev->dev, &dev_attr_breath_on_delay);
	device_remove_file(led_cdev->dev, &dev_attr_breath_off_delay);
}

static void breath_trig_deactivate(struct led_classdev *led_cdev)
{
	if (led_cdev->trigger_data) {
		device_remove_file(led_cdev->dev, &dev_attr_breath_on_delay);
		device_remove_file(led_cdev->dev, &dev_attr_breath_off_delay);
		device_remove_file(led_cdev->dev, &dev_attr_breath_enable);
	}

	/* Stop breathing */
	//breath_enable = 0;	
	hi_kernel_breath_timer_enable(0);	
	led_brightness_set(led_cdev, LED_OFF);
}


static struct led_trigger breath_led_trigger = {
	.name     = "breath",
	.activate = breath_trig_activate,
	.deactivate = breath_trig_deactivate,
};

static int __init breath_trig_init(void)
{
	printk("breath_trig_init\n");
	return led_trigger_register(&breath_led_trigger);
}

static void __exit breath_trig_exit(void)
{

	led_trigger_unregister(&breath_led_trigger);
}

module_init(breath_trig_init);
module_exit(breath_trig_exit);

MODULE_AUTHOR("Richard Purdie <rpurdie@openedhand.com>");
MODULE_DESCRIPTION("breath LED trigger");
MODULE_LICENSE("GPL");
