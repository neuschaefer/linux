/*
 * An hwmon driver for the Analog Devices AD7414
 *
 * Copyright 2006 Stefan Roese <sr@denx.de>, DENX Software Engineering
 *
 * Based on ad7418.c
 * Copyright 2006 Tower Technologies, Alessandro Zummo <a.zummo@towertech.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/i2c.h>
#include <linux/hwmon.h>
#include <linux/err.h>
#include <linux/mutex.h>

#define DRV_VERSION "0.1"

/* straight from the datasheet */
#define AD7414_TEMP_MIN (-55000)
#define AD7414_TEMP_MAX 125000

/* Addresses to scan */
static unsigned short normal_i2c[] = { 0x48, I2C_CLIENT_END };

/* Insmod parameters */
I2C_CLIENT_INSMOD;

/* AD7414 registers */
#define AD7414_REG_TEMP		0x00
#define AD7414_REG_CONF		0x01
#define AD7414_REG_T_HIGH	0x02
#define AD7414_REG_T_LOW	0x03

struct ad7414_data {
	struct i2c_client	client;
	struct class_device	*class_dev;
	struct mutex		lock;
	char			valid;		/* !=0 if following fields are valid */
	unsigned long		last_updated;	/* In jiffies */
	u16			temp_input;	/* Register values */
	u8			temp_max;
	u8			temp_min;
	u8			temp_alert;
	u8			temp_max_flag;
	u8			temp_min_flag;
};

static int ad7414_attach_adapter(struct i2c_adapter *adapter);
static int ad7414_detect(struct i2c_adapter *adapter, int address, int kind);
static int ad7414_detach_client(struct i2c_client *client);

static struct i2c_driver ad7414_driver = {
	.driver = {
		.name	= "ad7414",
	},
	.attach_adapter	= ad7414_attach_adapter,
	.detach_client	= ad7414_detach_client,
};

/*
 * TEMP: 0.001C/bit (-55C to +125C)
 * REG: (0.5C/bit, two's complement) << 7
 */
static inline int AD7414_TEMP_FROM_REG(u16 reg)
{
	/* use integer division instead of equivalent right shift to
	 * guarantee arithmetic shift and preserve the sign
	 */
	return ((s16)reg / 128) * 500;
}

/* All registers are word-sized, except for the configuration registers.
 * AD7414 uses a high-byte first convention, which is exactly opposite to
 * the usual practice.
 */
static int ad7414_read(struct i2c_client *client, u8 reg)
{
	if (reg == AD7414_REG_TEMP)
		return swab16(i2c_smbus_read_word_data(client, reg));
	else
		return i2c_smbus_read_byte_data(client, reg);
}

static int ad7414_write(struct i2c_client *client, u8 reg, u16 value)
{
	return i2c_smbus_write_byte_data(client, reg, value);
}

static void ad7414_init_client(struct i2c_client *client)
{
	/* TODO: anything to do here??? */
}

static struct ad7414_data *ad7414_update_device(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct ad7414_data *data = i2c_get_clientdata(client);

	mutex_lock(&data->lock);

	if (time_after(jiffies, data->last_updated + HZ + HZ / 2)
		|| !data->valid) {
		dev_dbg(&client->dev, "starting ad7414 update\n");

		data->temp_input = ad7414_read(client, AD7414_REG_TEMP);
		data->temp_alert = (data->temp_input >> 5) & 0x01;
		data->temp_max_flag = (data->temp_input >> 4) & 0x01;
		data->temp_min_flag = (data->temp_input >> 3) & 0x01;
		data->temp_max = ad7414_read(client, AD7414_REG_T_HIGH);
		data->temp_min = ad7414_read(client, AD7414_REG_T_LOW);

		data->last_updated = jiffies;
		data->valid = 1;
	}

	mutex_unlock(&data->lock);

	return data;
}

#define show(value) \
static ssize_t show_##value(struct device *dev, struct device_attribute *attr, char *buf)		\
{									\
	struct ad7414_data *data = ad7414_update_device(dev);		\
	return sprintf(buf, "%d\n", AD7414_TEMP_FROM_REG(data->value));	\
}
show(temp_input);

#define show_8(value)	\
static ssize_t show_##value(struct device *dev, struct device_attribute *attr, char *buf)		\
{								\
	struct ad7414_data *data = ad7414_update_device(dev);	\
	return sprintf(buf, "%d\n", data->value);		\
}
show_8(temp_max);
show_8(temp_min);
show_8(temp_alert);
show_8(temp_max_flag);
show_8(temp_min_flag);

#define set(value, reg)	\
static ssize_t set_##value(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)	\
{								\
	struct i2c_client *client = to_i2c_client(dev);		\
	struct ad7414_data *data = i2c_get_clientdata(client);	\
	int temp = simple_strtoul(buf, NULL, 10);		\
								\
	mutex_lock(&data->lock);				\
	data->value = temp;					\
	ad7414_write(client, reg, data->value);			\
	mutex_unlock(&data->lock);				\
	return count;						\
}
set(temp_max, AD7414_REG_T_HIGH);
set(temp_min, AD7414_REG_T_LOW);

static DEVICE_ATTR(temp1_max, S_IWUSR | S_IRUGO, show_temp_max, set_temp_max);
static DEVICE_ATTR(temp1_min, S_IWUSR | S_IRUGO, show_temp_min, set_temp_min);
static DEVICE_ATTR(temp1_input, S_IRUGO, show_temp_input, NULL);
static DEVICE_ATTR(temp1_alert, S_IRUGO, show_temp_alert, NULL);
static DEVICE_ATTR(temp1_max_flag, S_IRUGO, show_temp_max_flag, NULL);
static DEVICE_ATTR(temp1_min_flag, S_IRUGO, show_temp_min_flag, NULL);

static int ad7414_attach_adapter(struct i2c_adapter *adapter)
{
	if (!(adapter->class & I2C_CLASS_HWMON))
		return 0;
	return i2c_probe(adapter, &addr_data, ad7414_detect);
}

static struct attribute *ad7414_attributes[] = {
	&dev_attr_temp1_input.attr,
	&dev_attr_temp1_max.attr,
	&dev_attr_temp1_min.attr,
	&dev_attr_temp1_alert.attr,
	&dev_attr_temp1_max_flag.attr,
	&dev_attr_temp1_min_flag.attr,
	NULL
};

static const struct attribute_group ad7414_group = {
	.attrs = ad7414_attributes,
};

static int ad7414_detect(struct i2c_adapter *adapter, int address, int kind)
{
	struct i2c_client *client;
	struct ad7414_data *data;
	int err = 0;

	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA |
					I2C_FUNC_SMBUS_WORD_DATA))
		goto exit;

	if (!(data = kzalloc(sizeof(struct ad7414_data), GFP_KERNEL))) {
		err = -ENOMEM;
		goto exit;
	}

	client = &data->client;
	client->addr = address;
	client->adapter = adapter;
	client->driver = &ad7414_driver;
	client->flags = 0;

	i2c_set_clientdata(client, data);

	mutex_init(&data->lock);

	/* TODO: not testing for AD7414 done yet... */

	strlcpy(client->name, ad7414_driver.driver.name, I2C_NAME_SIZE);

	if ((err = i2c_attach_client(client)))
		goto exit_free;

	dev_info(&client->dev, "chip found, driver version " DRV_VERSION "\n");

	/* Initialize the AD7414 chip */
	ad7414_init_client(client);

	/* Register sysfs hooks */
	if ((err = sysfs_create_group(&client->dev.kobj, &ad7414_group)))
		goto exit_detach;

	data->class_dev = hwmon_device_register(&client->dev);
	if (IS_ERR(data->class_dev)) {
		err = PTR_ERR(data->class_dev);
		goto exit_remove;
	}

	return 0;

exit_remove:
	sysfs_remove_group(&client->dev.kobj, &ad7414_group);
exit_detach:
	i2c_detach_client(client);
exit_free:
	kfree(data);
exit:
	return err;
}

static int ad7414_detach_client(struct i2c_client *client)
{
	struct ad7414_data *data = i2c_get_clientdata(client);
	hwmon_device_unregister(data->class_dev);
	sysfs_remove_group(&client->dev.kobj, &ad7414_group);
	i2c_detach_client(client);
	kfree(data);
	return 0;
}

static int __init ad7414_init(void)
{
	return i2c_add_driver(&ad7414_driver);
}

static void __exit ad7414_exit(void)
{
	i2c_del_driver(&ad7414_driver);
}

MODULE_AUTHOR("Stefan Roese <sr@denx.de>");
MODULE_DESCRIPTION("AD7414 driver");
MODULE_LICENSE("GPL");
MODULE_VERSION(DRV_VERSION);

module_init(ad7414_init);
module_exit(ad7414_exit);
