// SPDX-License-Identifier: GPL-2.0-only
// Copyright 2019 Jonathan Neuschäfer
//
// MFD driver for the MSP430-based embedded controller used in certain Netronix
// ebook reader board designs

#include <asm/unaligned.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/i2c.h>
#include <linux/mfd/ntx_msp430.h>
#include <linux/of_platform.h>
#include <linux/pm.h>
#include <linux/reboot.h>
#include <linux/types.h>

/* Register access */

int ntx_msp430_read16(struct ntx_msp430 *mfd, u8 addr)
{
	u8 request[1] = { addr };
	u8 response[2];
	int res;

	struct i2c_msg msgs[] = {
		{
			.addr = mfd->client->addr,
			.flags = mfd->client->flags,
			.len = sizeof(request),
			.buf = request
		}, {
			.addr = mfd->client->addr,
			.flags = mfd->client->flags | I2C_M_RD,
			.len = sizeof(response),
			.buf = response
		}
	};

	res = i2c_transfer(mfd->client->adapter, msgs, ARRAY_SIZE(msgs));
	if (res < 0)
		return res;
	if (res != ARRAY_SIZE(msgs))
		return -EIO;

	return get_unaligned_be16(response);
}
EXPORT_SYMBOL(ntx_msp430_read16);

int ntx_msp430_write16(struct ntx_msp430 *mfd, u8 addr, u16 value)
{
	u8 request[3] = { addr, };
	int res;

	put_unaligned_be16(value, request + 1);

	res = i2c_transfer_buffer_flags(mfd->client, request, sizeof(request),
					mfd->client->flags);
	if (res < 0)
		return res;

	return 0;
}
EXPORT_SYMBOL(ntx_msp430_write16);

int ntx_msp430_read8(struct ntx_msp430 *mfd, u8 addr)
{
	int res = ntx_msp430_read16(mfd, addr);

	if (res < 0)
		return res;

	return (res >> 8) & 0xff;
}
EXPORT_SYMBOL(ntx_msp430_read8);

int ntx_msp430_write8(struct ntx_msp430 *mfd, u8 addr, u8 value)
{
	return ntx_msp430_write16(mfd, addr, value << 8);
}
EXPORT_SYMBOL(ntx_msp430_write8);


/* Reboot/poweroff handling */

static struct ntx_msp430 *poweroff_restart_instance;

static void ntx_msp430_poweroff(void)
{
	ntx_msp430_write8(poweroff_restart_instance, 0x50, 0x01);
	msleep(5000);
}

static int ntx_msp430_restart(struct notifier_block *nb,
		unsigned long action, void *data)
{
	/* FIXME: The I2C driver sleeps, but restart handlers may not sleep */
	ntx_msp430_write8(poweroff_restart_instance, 0x90, 0xff);
	/* TODO: delay? */
	return NOTIFY_DONE;
}

static struct notifier_block ntx_msp430_restart_handler = {
	.notifier_call = ntx_msp430_restart,
	.priority = 128
};


/* Driver setup */

static int ntx_msp430_probe(struct i2c_client *client,
			    const struct i2c_device_id *ids)
{
	struct ntx_msp430 *mfd;
	int res;

	mfd = devm_kmalloc(&client->dev, sizeof(*mfd), GFP_KERNEL);
	if (!mfd)
		return -ENOMEM;

	mfd->dev = &client->dev;
	mfd->client = client;

	/* Determine the firmware version */
	res = ntx_msp430_read16(mfd, 0);
	if (res < 0) {
		dev_dbg(mfd->dev, "Failed to read firmware version number\n");
		return res;
	}
	mfd->version = res;

	dev_info(mfd->dev,
		 "Netronix MSP430 embedded controller version %04x detected.\n",
		 mfd->version);

	/* For now, we don't support the new register layout. */
	if (is_new_layout(mfd))
		return -ENOTSUPP;

	if (of_device_is_system_power_controller(mfd->dev->of_node)) {
		/*
		 * Set the 'powerkeep' bit. This is necessary on some boards
		 * in order to keep the system running.
		 */
		res = ntx_msp430_write8(mfd, 0x70, 0x08);
		if (res < 0)
			return res;

		/* Install poweroff handler */
		WARN_ON(poweroff_restart_instance);
		poweroff_restart_instance = mfd;
		if (pm_power_off != NULL)
			/* TODO: Refactor among all poweroff drivers */
			dev_err(mfd->dev, "pm_power_off already assigned\n");
		else
			pm_power_off = ntx_msp430_poweroff;

		/* Install board reset handler */
		res = register_restart_handler(&ntx_msp430_restart_handler);
		if (res < 0)
			dev_err(mfd->dev,
				"Failed to register restart handler: %d\n", res);
	}

	i2c_set_clientdata(client, mfd);

	return devm_of_platform_populate(mfd->dev);
}

static const struct of_device_id of_ntx_msp430_match_table[] = {
	{ .compatible = "netronix,msp430-ec", },
	{}
};

static struct i2c_driver ntx_msp430_driver = {
	.driver	= {
		.name	= "ntx_msp430",
		.of_match_table = of_ntx_msp430_match_table,
	},
	.probe		= ntx_msp430_probe,
};
builtin_i2c_driver(ntx_msp430_driver);
