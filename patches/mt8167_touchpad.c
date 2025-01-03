/*
 * Driver for the I2C 4-keys MT8167 Touchpad
 *
 * Copyright 2017 SoftAtHome
 *
 * Licensed under the GPL-2 or later.
 */

#include <linux/module.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/of_irq.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>


#define DRV_NAME "mt8167-touchpad"
#define KEY_NUM 4


struct touchpad_data {
	unsigned int btncode[KEY_NUM];
	unsigned int debounce_interval;
	int irq_touchpad;
	int irq_reset;
	int gpio_reset;
	struct delayed_work work_touchpad;
	struct delayed_work work_reset;
	struct input_dev *idev;
	struct i2c_client *client;
	char name[64];
	char phys[32];
	unsigned int laststate;
	unsigned short reset_state;
};

static void mt8167_reset_report_event(struct work_struct *work)
{
	struct touchpad_data *lp = container_of(work, struct touchpad_data, work_reset.work);
	int value = !gpio_get_value(lp->gpio_reset);
	if (lp->reset_state != value) {
		input_report_key(lp->idev, 116, value);
		input_sync(lp->idev);

		lp->reset_state = value;
	}
}

static void mt8167_touchpad_report_event(struct work_struct *work)
{
	struct touchpad_data *lp = container_of(work, struct touchpad_data, work_touchpad.work);
	unsigned int nextstate = i2c_smbus_read_byte_data(lp->client, 0x01);
	unsigned short key_down;
	int i;

	//printk(KERN_INFO "[mt8167_touchpad] mt8167_touchpad_report_event.\n");

	if (lp->laststate != nextstate) {
		for (i = 0; i < KEY_NUM; ++i) {
			if (((lp->laststate >> i) & 1) != ((nextstate >> i) & 1)) {
				key_down = (unsigned short)((nextstate >> i) & 1);
				input_report_key(lp->idev, lp->btncode[i], key_down);
			}
		}

		input_sync(lp->idev);

		lp->laststate = nextstate;
	}
}

static irqreturn_t mt8167_reset_irq_handler(int irq, void *dev_id)
{
	struct touchpad_data *lp = dev_id;

	mod_delayed_work(system_wq,
			 &lp->work_reset,
			 msecs_to_jiffies(lp->debounce_interval));

	return IRQ_HANDLED;
}

static irqreturn_t mt8167_touchpad_irq_handler(int irq, void *dev_id)
{
	struct touchpad_data *lp = dev_id;

	mod_delayed_work(system_wq,
			 &lp->work_touchpad,
			 msecs_to_jiffies(lp->debounce_interval));

	return IRQ_HANDLED;
}


static int mt8167_touchpad_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int i, ret;
	struct input_dev *idev;
	struct touchpad_data *lp;

	lp = kzalloc(sizeof(*lp), GFP_KERNEL);
	if (!lp)
		return -ENOMEM;

	idev = input_allocate_device();
	if (!idev) {
		dev_err(&client->dev, "[mt8167_touchpad] Can't allocate input device\n");
		ret = -ENOMEM;
		goto fail_allocate;
	}

	lp->idev = idev;
	lp->client = client;


	if (of_property_read_u32(client->dev.of_node, "debounce_interval", &(lp->debounce_interval))) {
		dev_err(&client->dev, "[mt8167_touchpad] Debounce interval is not provided, default to 0\n");
		lp->debounce_interval = 0;
	}

	ret = of_property_read_u32_array(client->dev.of_node, "sah,linux-keycodes", lp->btncode, KEY_NUM);	
	if (ret) {
		printk(KERN_INFO "[mt8167_touchpad] No key codes detected.\n");
		goto fail_free_device;
	}

	for (i = 0; i < KEY_NUM; i++) {
		input_set_capability(idev, EV_KEY, lp->btncode[i]);
	}
	// Reset button capa
	input_set_capability(idev, EV_KEY, 116);

	sprintf(lp->name, DRV_NAME);
	sprintf(lp->phys, "touchpad_data/input0");

	idev->name = lp->name;
	idev->phys = lp->phys;
	idev->id.bustype = BUS_HOST;
	idev->id.vendor = 0x0001;
	idev->id.product = 0x0001;
	idev->id.version = 0x0100;

	lp->laststate = i2c_smbus_read_byte_data(client, 0);

	// IRQ for touchpad
	lp->irq_touchpad = irq_of_parse_and_map(client->dev.of_node, 0);
	if (lp->irq_touchpad < 0) {
		ret = lp->irq_touchpad;
		dev_err(&client->dev,"[mt8167_touchpad] Unable to parse irq 0, error %d\n",
			ret);
		goto fail_free_device;
	}

	// Delayed work for debounce simulation
	INIT_DELAYED_WORK(&lp->work_touchpad, mt8167_touchpad_report_event);

	ret = request_any_context_irq(lp->irq_touchpad, mt8167_touchpad_irq_handler, 
		IRQF_SHARED | IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "mt8167_touchpad", lp);

	if (ret) {
		dev_err(&client->dev, "[mt8167_touchpad] IRQ %d is not free\n", lp->irq_touchpad);
		goto fail_free_device;
	}

	// IRQ for reset button
	lp->gpio_reset = of_get_named_gpio(client->dev.of_node, "gpio-reset", 0);
	if (lp->gpio_reset < 0) {
		ret = lp->gpio_reset;
		dev_err(&client->dev,"[mt8167_touchpad] Unable to get parse reset GPIO, error %d\n",
			ret);
		goto fail_free_irq_touchpad;
	}
	
	gpio_request(lp->gpio_reset, "gpio-reset");

	lp->irq_reset = gpio_to_irq(lp->gpio_reset);
	if (lp->irq_reset < 0) {
		ret = lp->irq_reset;
		dev_err(&client->dev,"[mt8167_touchpad] Unable to get irq from GPIO %d, error %d\n", lp->gpio_reset,
			ret);
		goto fail_free_gpio;
	}

	// Delayed work for debounce simulation
	INIT_DELAYED_WORK(&lp->work_reset, mt8167_reset_report_event);

	ret = request_any_context_irq(lp->irq_reset, mt8167_reset_irq_handler, 
		IRQF_SHARED | IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "mt8167_reset", lp);

	if (ret) {
		dev_err(&client->dev, "[mt8167_touchpad] IRQ %d is not free\n", lp->irq_reset);
		goto fail_free_gpio;
	}

	lp->reset_state = 0;

	// Register input device
	ret = input_register_device(idev);
	if (ret) {
		dev_err(&client->dev, "[mt8167_touchpad] input_register_device() failed\n");
		goto fail_free_irq_reset;
	}

	i2c_set_clientdata(client, lp);

	return 0;

 fail_free_irq_reset:
	free_irq(lp->irq_reset, lp);
 fail_free_gpio:
 	gpio_free(lp->gpio_reset);
 fail_free_irq_touchpad:
	free_irq(lp->irq_touchpad, lp);
 fail_free_device:
	input_free_device(idev);
 fail_allocate:
	kfree(lp);

	return ret;
}

static int mt8167_touchpad_remove(struct i2c_client *client)
{
	struct touchpad_data *lp = i2c_get_clientdata(client);

	free_irq(lp->irq_touchpad, lp);
	free_irq(lp->irq_reset, lp);

	gpio_free(lp->gpio_reset);

	input_unregister_device(lp->idev);
	kfree(lp);

	return 0;
}

#ifdef CONFIG_PM
static int mt8167_touchpad_resume(struct device *dev)
{
	struct touchpad_data *lp = i2c_get_clientdata(to_i2c_client(dev));

	enable_irq(lp->irq_touchpad);

	return 0;
}

static int mt8167_touchpad_suspend(struct device *dev)
{
	struct touchpad_data *lp = i2c_get_clientdata(to_i2c_client(dev));

	disable_irq(lp->irq_touchpad);

	return 0;
}

static const struct dev_pm_ops mt8167_touchpad_pm_ops = {
	.suspend	= mt8167_touchpad_suspend,
	.resume		= mt8167_touchpad_resume,
};

#else
# define mt8167_touchpad_resume  NULL
# define mt8167_touchpad_suspend NULL
#endif

static const struct i2c_device_id mt8167_touchpad_id[] = {
	{ DRV_NAME, 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, mt8167_touchpad_id);

static struct i2c_driver mt8167_touchpad_driver = {
	.driver = {
		.name  = DRV_NAME,
#ifdef CONFIG_PM
		.pm = &mt8167_touchpad_pm_ops,
#endif
	},
	.probe    = mt8167_touchpad_probe,
	.remove   = mt8167_touchpad_remove,
	.id_table = mt8167_touchpad_id,
};

module_i2c_driver(mt8167_touchpad_driver);

MODULE_AUTHOR("Sacha TUBIANA");
MODULE_DESCRIPTION("Keypad input driver for Mediatek touchpad");
MODULE_LICENSE("GPL");
