/*
 * drivers/i2c/chips/max6900.c
 *
 * Driver for Maxim MAX6900 RTC
 *
 * Copyright (c) 2005-2006 DENX Software Engineering
 * Stefan Roese <sr@denx.de>
 *
 * Based on original work by
 *	Copyright (C) 2002-2004 Stefan Eletzhofer
 *	Copyright (C) 2000 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/rtc.h>		/* get the user-level API */
#include <linux/init.h>
#include <linux/list.h>

#include <asm/time.h>
#include <asm/machdep.h>

#include "max6900.h"

#define DRV_VERSION "1.0"

/*
 * Internal variables
 */
static LIST_HEAD(max6900_clients);

static struct i2c_client *myclient = NULL;

static int debug = 0;
module_param(debug, int, S_IRUGO | S_IWUSR);

static struct i2c_driver max6900_driver;

/* save/restore old machdep pointers */
int (*save_set_rtc_time)(unsigned long);
unsigned long (*save_get_rtc_time)(void);

extern spinlock_t rtc_lock;

static int max6900_read(struct i2c_client *client, unsigned char adr,
			unsigned char *buf, unsigned char len)
{
	int ret = -EIO;
	unsigned char addr[1] = { adr };
	struct i2c_msg msgs[2] = {
		{client->addr, 0, 1, addr},
		{client->addr, I2C_M_RD, len, buf}
	};

	_DBG(1, "client=%p, adr=%d, buf=%p, len=%d", client, adr, buf, len);

	if (!buf) {
		ret = -EINVAL;
		goto done;
	}

	ret = i2c_transfer(client->adapter, msgs, 2);
	if (ret == 2) {
		ret = 0;
	}

done:
	return ret;
}

static int max6900_write(struct i2c_client *client, unsigned char adr,
			 unsigned char *data, unsigned char len)
{
	int ret = 0;
	unsigned char _data[16];
	struct i2c_msg wr;
	int i;

	if (!data || len > 15) {
		ret = -EINVAL;
		goto done;
	}

	_DBG(1, "client=%p, adr=%d, buf=%p, len=%d", client, adr, data, len);

	_data[0] = adr;
	for (i = 0; i < len; i++) {
		_data[i + 1] = data[i];
		_DBG(5, "data[%d] = 0x%02x (%d)", i, data[i], data[i]);
	}

	wr.addr = client->addr;
	wr.flags = 0;
	wr.len = len + 1;
	wr.buf = _data;

	ret = i2c_transfer(client->adapter, &wr, 1);
	if (ret == 1) {
		ret = 0;
	}

done:
	return ret;
}

static int max6900_attach(struct i2c_adapter *adap, int addr, int kind)
{
	int ret;
	struct i2c_client *new_client;
	struct max6900_data *d;
	unsigned char val = 0;

	d = kmalloc(sizeof(struct max6900_data), GFP_KERNEL);
	if (!d) {
		ret = -ENOMEM;
		goto done;
	}
	memset(d, 0, sizeof(struct max6900_data));
	INIT_LIST_HEAD(&d->list);

	new_client = &d->client;

	strlcpy(new_client->name, "MAX6900", I2C_NAME_SIZE);
	i2c_set_clientdata(new_client, d);
	new_client->addr = addr;
	new_client->adapter = adap;
	new_client->driver = &max6900_driver;

	_DBG(1, "client=%p", new_client);

	/* Verify the chip is really an MAX6900 */
	max6900_read(new_client, 0x96, &val, 1);
	if (val != 0x07) {
		ret = -ENODEV;
		goto done;
	}

	ret = i2c_attach_client(new_client);

	/* Add client to local list */
	list_add(&d->list, &max6900_clients);

	dev_info(&new_client->dev, "chip found, driver version " DRV_VERSION "\n");

done:
	if (ret) {
		kfree(d);
	} else {
		myclient = new_client;
	}

	return ret;
}

static int max6900_probe(struct i2c_adapter *adap)
{
	max6900_attach(adap, 0x50, 0);
	return 0;
}

static int max6900_detach(struct i2c_client *client)
{
	struct max6900_data *data = i2c_get_clientdata(client);

	i2c_detach_client(client);
	kfree(i2c_get_clientdata(client));
	list_del(&data->list);
	return 0;
}

static int max6900_get_datetime(struct i2c_client *client, struct rtc_time *dt)
{
	int ret = -EIO;
	unsigned char sec, sec2, min, hour, mday, wday, mon, cent, year;

	_DBG(1, "client=%p, dt=%p", client, dt);

	if (!dt)
		return -EINVAL;

	do {
		ret = max6900_read(client, MAX6900_REG_SEC, &sec, 1);
		ret |= max6900_read(client, MAX6900_REG_MIN, &min, 1);
		ret |= max6900_read(client, MAX6900_REG_HOUR, &hour, 1);
		ret |= max6900_read(client, MAX6900_REG_MDAY, &mday, 1);
		ret |= max6900_read(client, MAX6900_REG_MON, &mon, 1);
		ret |= max6900_read(client, MAX6900_REG_WDAY, &wday, 1);
		ret |= max6900_read(client, MAX6900_REG_YEAR, &year, 1);
		ret |= max6900_read(client, MAX6900_REG_CENT, &cent, 1);
		if (ret)
			return ret;

		/*
		 * Check for seconds rollover
		 */
		ret = max6900_read(client, MAX6900_REG_SEC, &sec2, 1);
		if ((sec != 59) || (sec2 == sec)){
			break;
		}
	} while (1);

	dt->tm_year = BCD_TO_BIN(year) + BCD_TO_BIN(cent) * 100;
	dt->tm_mday = BCD_TO_BIN(mday & 0x3f);
	dt->tm_wday = BCD_TO_BIN(wday & 7);
	dt->tm_mon = BCD_TO_BIN(mon & 0x1f);

	dt->tm_sec = BCD_TO_BIN(sec & 0x7f);
	dt->tm_min = BCD_TO_BIN(min & 0x7f);
	dt->tm_hour = BCD_TO_BIN(hour & 0x3f);

	_DBGRTCTM(2, *dt);
	return 0;
}

static int
max6900_set_datetime(struct i2c_client *client, struct rtc_time *dt, int datetoo)
{
	int ret;
	unsigned char val;
	unsigned char sec, min, hour, mday, wday, mon, cent, year;

	_DBG(1, "client=%p, dt=%p", client, dt);

	if (!dt)
		return -EINVAL;

	val = 0x00;
	ret = max6900_write(client, 0x9e, &val, 1);

	/* Clear seconds to ensure no rollover */
	ret |= max6900_write(client, MAX6900_REG_SEC, &val, 1);

	min = BIN_TO_BCD(dt->tm_min);
	ret |= max6900_write(client, MAX6900_REG_MIN, &min, 1);
	hour = BIN_TO_BCD(dt->tm_hour);
	ret |= max6900_write(client, MAX6900_REG_HOUR, &hour, 1);

	if (datetoo) {
		mday = BIN_TO_BCD(dt->tm_mday);
		ret |= max6900_write(client, MAX6900_REG_MDAY, &mday, 1);
		wday = BIN_TO_BCD(dt->tm_wday);
		ret |= max6900_write(client, MAX6900_REG_WDAY, &wday, 1);
		mon = BIN_TO_BCD(dt->tm_mon);
		ret |= max6900_write(client, MAX6900_REG_MON, &mon, 1);
		year = BIN_TO_BCD(dt->tm_year % 100);
		ret |= max6900_write(client, MAX6900_REG_YEAR, &year, 1);
		cent = BIN_TO_BCD(dt->tm_year / 100);
		ret |= max6900_write(client, MAX6900_REG_CENT, &cent, 1);
	}

	sec = BIN_TO_BCD(dt->tm_sec);
	ret |= max6900_write(client, MAX6900_REG_SEC, &sec, 1);

	if (ret) {
		_DBG(1, "error writing data! %d", ret);
	}

	return ret;
}

static int max6900_get_ctrl(struct i2c_client *client, unsigned int *ctrl)
{
	struct max6900_data *data = i2c_get_clientdata(client);

	if (!ctrl)
		return -1;

	*ctrl = data->ctrl;
	return 0;
}

static int max6900_set_ctrl(struct i2c_client *client, unsigned int *ctrl)
{
	struct max6900_data *data = i2c_get_clientdata(client);
	unsigned char buf[2];

	if (!ctrl)
		return -1;

	buf[0] = *ctrl & 0xff;
	buf[1] = (*ctrl & 0xff00) >> 8;
	data->ctrl = *ctrl;

	return max6900_write(client, 0, buf, 2);
}

static int max6900_read_mem(struct i2c_client *client, struct mem *mem)
{

	if (!mem)
		return -EINVAL;

	return max6900_read(client, mem->loc, mem->data, mem->nr);
}

static int max6900_write_mem(struct i2c_client *client, struct mem *mem)
{

	if (!mem)
		return -EINVAL;

	return max6900_write(client, mem->loc, mem->data, mem->nr);
}

static int
max6900_command(struct i2c_client *client, unsigned int cmd, void *arg)
{

	_DBG(1, "cmd=%d", cmd);

	switch (cmd) {
	case RTC_GETDATETIME:
		return max6900_get_datetime(client, arg);

	case RTC_SETTIME:
		return max6900_set_datetime(client, arg, 0);

	case RTC_SETDATETIME:
		return max6900_set_datetime(client, arg, 1);

	case RTC_GETCTRL:
		return max6900_get_ctrl(client, arg);

	case RTC_SETCTRL:
		return max6900_set_ctrl(client, arg);

	case MEM_READ:
		return max6900_read_mem(client, arg);

	case MEM_WRITE:
		return max6900_write_mem(client, arg);

	default:
		return -EINVAL;
	}
}

/*
 * Public API for access to specific device. Useful for low-level
 * RTC access from kernel code.
 */
int max6900_do_command(int bus, int cmd, void *arg)
{
	struct list_head *walk;
	struct list_head *tmp;
	struct max6900_data *data;

	list_for_each_safe(walk, tmp, &max6900_clients) {
		data = list_entry(walk, struct max6900_data, list);
		if (data->client.adapter->nr == bus) {
			return max6900_command(&data->client, cmd, arg);
		}
	}

	return -ENODEV;
}

/***************************************************************************
 *
 * get RTC time:
 */
unsigned long max6900_get_rtc_time(void)
{
	struct rtc_time tm;
	int result;

	spin_lock(&rtc_lock);
	result = max6900_do_command(0, RTC_GETDATETIME, &tm);
	spin_unlock(&rtc_lock);

	if (result == 0)
		result = mktime(tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	return result;
}

/***************************************************************************
 *
 * set RTC time:
 */
int max6900_set_rtc_time(unsigned long now)
{
        struct rtc_time tm;
        unsigned char century, year, mon, wday, mday, hour, min, sec;

        to_tm (now, &tm);

        _DBG (2, "Set RTC [dec] year=%d mon=%d day=%d hour=%d min=%d sec=%d\n",
                tm.tm_year, tm.tm_mon, tm.tm_mday,
                tm.tm_hour, tm.tm_min, tm.tm_sec);

        century = (tm.tm_year >= 2000) ? 0x80 : 0;
        year = BIN_TO_BCD (tm.tm_year % 100);
        mon  = BIN_TO_BCD (tm.tm_mon) | century;
        wday = BIN_TO_BCD (tm.tm_wday);
        mday = BIN_TO_BCD (tm.tm_mday);
        hour = BIN_TO_BCD (tm.tm_hour);
        min  = BIN_TO_BCD (tm.tm_min);
        sec  = BIN_TO_BCD (tm.tm_sec);

        _DBG (2, "Set RTC [bcd] year=%X mon=%X day=%X "
                "hour=%X min=%X sec=%X wday=%X\n",
                year, mon, mday, hour, min, sec, wday);

	max6900_set_datetime(myclient, &tm, 1);

        return (0);
}

static struct i2c_driver max6900_driver = {
	.driver = {
		.name	= "MAX6900",
	},
	.id		= I2C_DRIVERID_MAX6900,
	.attach_adapter = max6900_probe,
	.detach_client	= max6900_detach,
	.command	= max6900_command
};

static __init int max6900_init(void)
{
	return i2c_add_driver(&max6900_driver);
}

static __exit void max6900_exit(void)
{
        ppc_md.set_rtc_time = save_set_rtc_time;
        ppc_md.get_rtc_time = save_get_rtc_time;

	i2c_del_driver(&max6900_driver);
}

MODULE_AUTHOR("Stefan Roese <sr@denx.de>");
MODULE_DESCRIPTION("Maxim/Dallas MAX6900 RTC I2C Client Driver");
MODULE_LICENSE("GPL");

module_init(max6900_init);
module_exit(max6900_exit);
