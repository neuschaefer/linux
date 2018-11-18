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
#include <linux/delay.h>
#include <mach/hci-event.h>

#define OC_CHECK_NUM 		10
#define OC_CHECK_INTERVAL_MS 	100

/* sysfs interface for switch class (/sys/class/switch/hci-event/state) */
static ssize_t
print_switch_state(struct switch_dev *sdev, char *buf)
{
        struct hci_event_platform_data *pdata = 
	    container_of(sdev, struct hci_event_platform_data, sdev);

	buf[0] = '\0';

	if (pdata->hci_state & TEGRA_HCI_STATE_OC_MASK)
		strlcat(buf, "OC", UEVENT_BUFFER_SIZE);

	if (pdata->hci_state & TEGRA_HCI_STATE_OH_MASK) {
		if (strlen(buf) != 0)
			strlcat(buf, ",", UEVENT_BUFFER_SIZE);
		strlcat(buf, "OH", UEVENT_BUFFER_SIZE);
	}
	if (pdata->hci_state & TEGRA_HCI_STATE_NS_MASK) {
		if (strlen(buf) != 0)
			strlcat(buf, ",", UEVENT_BUFFER_SIZE);
		strlcat(buf, "NS", UEVENT_BUFFER_SIZE);
	}
	if (strlen(buf) == 0)
		strlcat(buf, "NM", UEVENT_BUFFER_SIZE);

        return strlen(buf);
}

/* 
 * sysfs interface for debug.(force setstate) 
 * (/sys/class/switch/hci-event/dbg) 
 */
static ssize_t
store_setstate(struct device *dev, struct device_attribute *attr, 
    const char *buf, size_t count)
{
	struct switch_dev *sdev = (struct switch_dev *)dev_get_drvdata(dev);
        struct hci_event_platform_data *pdata = 
	    container_of(sdev, struct hci_event_platform_data, sdev);

	if (!strncmp(buf, "!OC", strlen("!OC"))) {
		pdata->hci_state &= ~TEGRA_HCI_STATE_OC_MASK;	
	} else if (!strncmp(buf, "!OH", strlen("!OH"))) {
		pdata->hci_state &= ~TEGRA_HCI_STATE_OH_MASK;	
	} else if (!strncmp(buf, "!NS", strlen("!NS"))) {
		pdata->hci_state &= ~TEGRA_HCI_STATE_NS_MASK;
	} else if (!strncmp(buf, "OC", strlen("OC"))) {
		pdata->hci_state |= TEGRA_HCI_STATE_OC_MASK;	
	} else if (!strncmp(buf, "OH", strlen("OH"))) {
		pdata->hci_state |= TEGRA_HCI_STATE_OH_MASK;
	} else if (!strncmp(buf, "NS", strlen("NS"))) {
		pdata->hci_state |= TEGRA_HCI_STATE_NS_MASK;
	}
	switch_set_state(sdev, pdata->hci_state);
	return count;	
}
static DEVICE_ATTR(dbg, 0200, NULL, store_setstate);


static void
hci_event_ocdet_work(struct work_struct *work)
{
        struct hci_event_platform_data *pdata = 
	    container_of(work, struct hci_event_platform_data, work);
	int pin;
	int i;

	pin = gpio_get_value(pdata->ocdet_pin);
	for (i = 0; i < OC_CHECK_NUM; i++) {

		/* if vbus=off(not hostmode), ignore */
		if (!pin && !gpio_get_value(pdata->pwren_pin)) {
			pdata->hci_state &= ~TEGRA_HCI_STATE_OC_MASK;
			switch_set_state(&pdata->sdev, pdata->hci_state);
			return;
		}

		if (pin != gpio_get_value(pdata->ocdet_pin)) {
			printk("chattering detect\n");
			return;
		}
		msleep(OC_CHECK_INTERVAL_MS);
	}

	/* low active */
	if (pin) {
		printk("oc_det pin 1\n");
		pdata->hci_state &= ~TEGRA_HCI_STATE_OC_MASK;
	} else {
		printk("oc_det pin 0\n");
		pdata->hci_state |= TEGRA_HCI_STATE_OC_MASK;
	}

	switch_set_state(&pdata->sdev, pdata->hci_state);
}

/* Over current detect */
static irqreturn_t
overcurrent_intr(int irq, void *arg)
{
	struct hci_event_platform_data *pdata;
	pdata = (struct hci_event_platform_data *)arg;

	schedule_work(&pdata->work);

	return IRQ_HANDLED;
}


static int __init
hci_event_probe(struct platform_device *pdev)
{
	struct hci_event_platform_data *pdata;
	int ret;
	int irq;


	pdata = (struct hci_event_platform_data *)pdev->dev.platform_data;

	/* initialize work for sending event */
	INIT_WORK(&pdata->work, hci_event_ocdet_work);

	/* switch class register */
	pdata->sdev.name = "hci-event";
	pdata->sdev.print_name = NULL;
	pdata->sdev.print_state = print_switch_state;
	if ((ret = switch_dev_register(&pdata->sdev)) < 0)
		goto err0;
	/* add sysfs file for debug */
	if ((ret = device_create_file(pdata->sdev.dev, &dev_attr_dbg)) < 0)
		goto err1;

	tegra_gpio_enable(pdata->ocdet_pin);

	/* get gpio/irq for VBUS over current detection */
	if ((ret = gpio_request(pdata->ocdet_pin, "oc detect")) < 0) {
		goto err2;
	}

	gpio_direction_input(pdata->ocdet_pin);

	if ((irq = gpio_to_irq(pdata->ocdet_pin)) < 0) {
		ret = irq;
		goto err3;
	}
	if ((ret = request_irq(irq, overcurrent_intr, 
	    IRQF_SHARED | IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, 
	    "hci_event", pdata)) < 0) {
		goto err3;
	}

	/* check initial state */
	schedule_work(&pdata->work);
	
	return 0;
err3:
	gpio_free(pdata->ocdet_pin);
err2:
	device_remove_file(pdata->sdev.dev, &dev_attr_dbg);
err1:
	switch_dev_unregister(&pdata->sdev);
err0:
	return ret;
}

static int __exit
hci_event_remove(struct platform_device *pdev)
{
	struct hci_event_platform_data *pdata;
	int irq;

	pdata = (struct hci_event_platform_data *)pdev->dev.platform_data;

	cancel_work_sync(&pdata->work);

	switch_dev_unregister(&pdata->sdev);

	irq = gpio_to_irq(pdata->ocdet_pin);
	free_irq(irq, pdata);

	gpio_free(pdata->ocdet_pin);

	device_remove_file(pdata->sdev.dev, &dev_attr_dbg);


	return 0;
}

static struct platform_driver hci_event_driver = {
	.probe = hci_event_probe,
	.remove = __exit_p(hci_event_remove),
	.driver = {
		.name = "hci-event",
		.owner = THIS_MODULE
	}
};

static int __init hci_event_init(void)
{
	return platform_driver_register(&hci_event_driver);
}
module_init(hci_event_init);

static void __exit hci_event_exit(void)
{
	return platform_driver_unregister(&hci_event_driver);
}
module_exit(hci_event_exit);


