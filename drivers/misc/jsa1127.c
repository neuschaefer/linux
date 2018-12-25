/*  Digital Light Sensor driver for linux
 *  by XXX <XXX@newbietech.com>
 *
 *  Copyright (C) 2011 newbie Technology.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 */

/* #define DEBUG */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <mach/sys_config.h>

#define	jsa1127_SLAVE_ADDR	0x29
#define	JSA1127_NAME		"jsa1127"
#define	ABS_LUX			(ABS_MISC)
#define	MIN_LUX		(0)	/* TODO: to be modified */
#define	MAX_LUX		(32767)	/* TODO: to be modified */
#define	INPUT_FUZZ	(0)
#define	INPUT_FLAT	(0)

#define JSA1127_INIT_DELAY	60
#define JSA1127_INTERVAL_LONG	100
#define JSA1127_INTERVAL_SHORT	20

#define CONTINUOUS_MODE_CMD	0x0C
#define ONETIME_MODE_CMD	0x04
#define SHUTDOWN_MODE_CMD	0x8C

struct jsa1127_struct {
	struct delayed_work work;
	struct workqueue_struct *queue;
};

static struct kobject *jsa1127_kobj;

static unsigned int twi_addr = jsa1127_SLAVE_ADDR;
static unsigned int twi_id = 0xff;

static struct i2c_client *this_client;
static struct jsa1127_struct *jsa1127_data;

#define MEAS_SIZE 16
static int meas[MEAS_SIZE];
static size_t meas_cnt;

static int jsa1127_i2c_read(char *data, int lenth)
{
	int ret;
	u8 address = 0;
	struct i2c_msg msgs[] = {
		{
		 .addr = this_client->addr,
		 .flags = 0,
		 .len = 1,
		 .buf = &address,
		},
		{
		 .addr = this_client->addr,
		 .flags = 1,
		 .len = lenth,
		 .buf = data,
		},
	};

	ret = i2c_transfer(this_client->adapter, msgs, 2);
	if (ret < 0)
		pr_err("[%s] I2C data read failed %d\n", __func__, ret);

	return ret;
}

static int jsa1127_byte_write_reg(u8 value)
{
	int ret;

	ret = i2c_smbus_write_byte(this_client, value);
	if (0 != ret)
		pr_err(" %s i2c read error: %d\n", __func__, ret);

	return ret;
}

static int jsa1127_hw_enable(void)
{
	pr_debug("[%s]\n", __func__);
	jsa1127_byte_write_reg(CONTINUOUS_MODE_CMD);
	return 0;
}

static int jsa1127_hw_disable(void)
{
	pr_debug("[%s]\n", __func__);
	jsa1127_byte_write_reg(SHUTDOWN_MODE_CMD);
	return 0;
}

static int jsa1127_suspend(struct device *dev)
{
	pr_debug("[%s]\n", __func__);
	jsa1127_hw_disable();
	return 0;
}

static int jsa1127_resume(struct device *dev)
{
	pr_debug("[%s]\n", __func__);
	jsa1127_hw_enable();
	return 0;
}

static int jsa1127_get_val(void)
{
	int data = 0, ret;
	unsigned char buf[2] = {0};

	ret = jsa1127_i2c_read(buf, ARRAY_SIZE(buf));
	if (ret > 0) {
		pr_debug("[%s] buf [%02X, %02X]\n", __func__, buf[0], buf[1]);

		if ((buf[1] & 0x80) != 0x80)
			pr_warn("[%s] No valid conversion result is available\n", __func__);

		buf[1] &= ~0x80;
		data = (buf[1] << 8) + buf[0];

		pr_debug("[%s] Converted data: %d\n", __func__, data);
	}

	return data;
}

static void jsa1127_read_loop(struct work_struct *work)
{
	int i, n;
	char buf[256];

	meas[meas_cnt] = jsa1127_get_val();
	meas_cnt++;
	meas_cnt %= MEAS_SIZE;

	queue_delayed_work(jsa1127_data->queue, &jsa1127_data->work, msecs_to_jiffies(JSA1127_INTERVAL_LONG));

#if defined(DEBUG)
	for (i = n = 0; i < MEAS_SIZE; i++)
		n += snprintf(buf + n, ARRAY_SIZE(buf), " %d", meas[i]);

	pr_debug("[%s] meas[] = %s\n", __func__, buf);
#endif
}

static ssize_t jsa1127_show_lux(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
	int lux, i;

	for (i = 0, lux = 0; i < ARRAY_SIZE(meas); i++)
		lux += meas[i];

	lux = (lux * 100) / 67;

	return sprintf(buf, "%i\n", lux / MEAS_SIZE);
}

static DEVICE_ATTR(lux, S_IRUGO, jsa1127_show_lux, NULL);

static struct attribute *jsa1127_attributes[] = {
	&dev_attr_lux.attr,
	NULL
};

static const struct attribute_group jsa1127_attr_group = {
	.attrs = jsa1127_attributes,
};

static int jsa1127_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	int err = 0;

	pr_info("[%s] JSA-1127 ambient light sensor driver\n", __func__);

	jsa1127_data = kzalloc(sizeof(struct jsa1127_struct), GFP_KERNEL);
	if (!jsa1127_data) {
		err = -ENOMEM;
		goto do_fail;
	}

	this_client = client;
	jsa1127_hw_enable();

	msleep(200);

	INIT_DELAYED_WORK(&jsa1127_data->work, jsa1127_read_loop);

	jsa1127_data->queue = create_singlethread_workqueue(JSA1127_NAME);
	if (!jsa1127_data->queue) {
		pr_err("[%s] Failed to create workqueue\n", __func__);
		err = -ESRCH;
		goto do_fail_malloc;
	}

	jsa1127_kobj = kobject_create_and_add(JSA1127_NAME, kernel_kobj);
	if (!jsa1127_kobj)
		return -ENOMEM;

	err = sysfs_create_group(jsa1127_kobj, &jsa1127_attr_group);
	if (err)
		goto do_fail_malloc;

	queue_delayed_work(jsa1127_data->queue, &jsa1127_data->work,
			   JSA1127_INIT_DELAY);

	pr_debug("[%s] Driver was registered\n", __func__);

	return 0;

do_fail_malloc:
	kobject_put(jsa1127_kobj);
	i2c_set_clientdata(client, NULL);
	kfree(&jsa1127_data);

do_fail:
	return err;
}

static int __devexit jsa1127_remove(struct i2c_client *client)
{
	pr_debug("[%s] Removing driver\n", __func__);

	sysfs_remove_group(jsa1127_kobj, &jsa1127_attr_group);
	kobject_put(jsa1127_kobj);

	cancel_delayed_work_sync(&jsa1127_data->work);
	destroy_workqueue(jsa1127_data->queue);

	if (client == this_client)
		i2c_set_clientdata(client, NULL);
	else
		pr_err("remove jsa1127 with wrong i2c_client\n");

	kfree(jsa1127_data);

	return 0;
}

static int jsa1127_detect(struct i2c_client *client,
			  struct i2c_board_info *info)
{
	struct i2c_adapter *adapter = client->adapter;

	pr_debug("[%s]\n", __func__);

	if (adapter->nr == twi_id) {
		strlcpy(info->type, JSA1127_NAME, I2C_NAME_SIZE);
		return 0;
	} else {
		return -ENODEV;
	}
}

static const struct i2c_device_id jsa1127_id[] = {
	{JSA1127_NAME, 0},
	{}
};

MODULE_DEVICE_TABLE(i2c, jsa1127_id);

static const unsigned short normal_i2c[] = {jsa1127_SLAVE_ADDR, I2C_CLIENT_END};

static SIMPLE_DEV_PM_OPS(jsa1127_pm, jsa1127_suspend, jsa1127_resume);

static struct i2c_driver jsa1127_i2c_driver = {
	.probe = jsa1127_probe,
	.remove = __devexit_p(jsa1127_remove),
	.detect = jsa1127_detect,
	.id_table = jsa1127_id,
	.class = I2C_CLASS_HWMON,
	.driver = {
		.name = JSA1127_NAME,
		.owner = THIS_MODULE,
		.pm = &jsa1127_pm,
	},
	.address_list = normal_i2c,
};

static int jsa1127_fetch_sysconfig_para(void)
{
	int ret = -1;
	int jsa1127_used = -1;
	char name[I2C_NAME_SIZE];

	script_parser_value_type_t type = SCIRPT_PARSER_VALUE_TYPE_STRING;

	if (SCRIPT_PARSER_OK !=
	    script_parser_fetch("ls_para_1127", "ls_used", &jsa1127_used, 1)) {
		pr_err("%s: script_parser_fetch err.\n", __func__);
		goto script_parser_fetch_err;
	}
	if (1 != jsa1127_used) {
		pr_err("%s: jsa1127_unused.\n", __func__);
		return -1;
	}

	if (SCRIPT_PARSER_OK !=
	    script_parser_fetch_ex("ls_para_1127", "ls_name", (int *)(&name),
				   &type, I2C_NAME_SIZE * sizeof(char))) {
		pr_err("%s: script_parser_fetch err.\n", __func__);
		goto script_parser_fetch_err;
	}
	if (strcmp(JSA1127_NAME, name)) {
		pr_err("%s: name %s does not match JSA1127_NAME.\n", __func__,
		       name);
		pr_err(JSA1127_NAME);
		return -1;
	}

	if (SCRIPT_PARSER_OK !=
	    script_parser_fetch("ls_para_1127", "ls_twi_id", &twi_id,
				sizeof(twi_id))) {
		pr_err("%s: script_parser_fetch twi_id err.\n", name);
		goto script_parser_fetch_err;
	}

	pr_debug("[%s] Fetched I2C_BUS:I2C_ADDR - %02x:%02x\n", __func__,
			twi_id, twi_addr);

	return 0;

script_parser_fetch_err:
	pr_warn("[%s] Failed to fetch values from sysconfig\n", __func__);
	return ret;
}

static int __init jsa1127_init(void)
{
	int ret = 0;

	pr_debug("[%s]\n", __func__);

	ret = jsa1127_fetch_sysconfig_para();
	if (ret < 0) {
		pr_err("[%s] Failed to fetch from sysconfig!\n", __func__);
		return -1;
	}

	ret = i2c_add_driver(&jsa1127_i2c_driver);
	if (ret)
		return ret;

	pr_debug("[%s] I2C driver inited\n", __func__);

	return ret;
}

static void __exit jsa1127_exit(void)
{
	pr_debug("[%s]", __func__);
	jsa1127_hw_disable();

	i2c_del_driver(&jsa1127_i2c_driver);
}

module_init(jsa1127_init);
module_exit(jsa1127_exit);

MODULE_AUTHOR("zly <zenglingying@newbietech.com>");
MODULE_DESCRIPTION("AW Light Sensor Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.1");
