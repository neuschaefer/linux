// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (C) J. Neuschäfer <j.ne@posteo.net>
/*
 * Wii extension bus.
 *
 * Different devices can be attached to the bus (but only one at a time), and
 * they have different input report layouts and functions. They do however have
 * an initialization sequence and identification mechanism in common.
 */

// TODO: GPIO-based presence detection

#include <linux/module.h>
#include <linux/of.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/input/wiiext.h>
#include <linux/unaligned.h>

/* common registers */
#define WIIEXT_REG_INIT1	0xf0
#define WIIEXT_INIT1_VALUE	0x55
#define WIIEXT_REG_INIT2	0xfb
#define WIIEXT_INIT2_VALUE	0x00
#define WIIEXT_REG_ID		0xfa
#define WIIEXT_ID_LENGTH	6

static void wiiext_id_from_bytes(struct wiiext_id *id, const u8 *buf)
{
	id->field[0] = get_unaligned_be16(&buf[0]);
	id->field[1] = get_unaligned_be16(&buf[2]);
	id->field[2] = get_unaligned_be16(&buf[4]);
}

static const struct wiiext_id wiiext_id_uninitialized = WIIEXT_ID(0xffff, 0xffff, 0xffff);

int wiiext_read(struct wiiext *wiiext, u8 reg, u8 *data, size_t length)
{
	struct i2c_client *client = wiiext->client;
	int ret = 0;

	mutex_lock(&wiiext->mutex);

	ret = i2c_master_send(client, &reg, sizeof(reg));
	if (ret != sizeof(reg)) {
		if (ret != -ENXIO)
			dev_info(&client->dev, "send(%02x, %d) = %d\n", reg, length, ret);

		ret = ret < 0 ? ret : -EIO;
		goto out;
	}

	/*
	 * Wiimotes make a 200 µs delay between the send/receive stages of a
	 * register read, and original extensions will fail if the delay is
	 * shorter.
	 */
	usleep_range(200, 500);

	ret = i2c_master_recv(client, data, length);
	if (ret != length) {
		if (ret != -ENXIO)
			dev_info(&client->dev, "recv(%02x, %d) = %d\n", reg, length, ret);

		ret = ret < 0 ? ret : -EIO;
		goto out;
	}

	ret = 0;
out:
	/* Let some time pass before allowing another interaction with the device */
	usleep_range(200, 500);

	mutex_unlock(&wiiext->mutex);
	return ret;
}
EXPORT_SYMBOL_GPL(wiiext_read);

int wiiext_write(struct wiiext *wiiext, const u8 *data, size_t length)
{
	int ret = 0;

	mutex_lock(&wiiext->mutex);

	ret = i2c_master_send(wiiext->client, data, length);
	if (ret != length)
		ret = ret < 0 ? ret : -EIO;

	mutex_unlock(&wiiext->mutex);

	return ret;
}
EXPORT_SYMBOL_GPL(wiiext_write);

int wiiext_write_one(struct wiiext *wiiext, u8 reg, u8 value)
{
	u8 buf[2] = { reg, value };

	return wiiext_write(wiiext, buf, 2);
}
EXPORT_SYMBOL_GPL(wiiext_write_one);

static int wiiext_read_id(struct wiiext *wiiext)
{
	u8 buf[WIIEXT_ID_LENGTH];
	int ret;

	ret = wiiext_read(wiiext, WIIEXT_REG_ID, buf, sizeof(buf));

	if (ret == -ENXIO)
		return ret; /* no response at all */

	if (ret == -EIO)
		return ret;

	wiiext_id_from_bytes(&wiiext->id, buf);

	return 0;
}

static int wiiext_discover(struct wiiext *wiiext)
{
	struct i2c_client *client = wiiext->client;
	int ret;

	ret = wiiext_write_one(wiiext, WIIEXT_REG_INIT1, WIIEXT_INIT1_VALUE);
	if (ret < 0)
		return ret;

	msleep(20);

	ret = wiiext_write_one(wiiext, WIIEXT_REG_INIT2, WIIEXT_INIT2_VALUE);
	if (ret < 0)
		return ret;

	msleep(20);

	ret = wiiext_read_id(wiiext);
	if (ret < 0)
		return ret;

	if (wiiext_id_eq(&wiiext->id, &wiiext_id_uninitialized))
		return -EIO;

	dev_info(&client->dev, "Found device ID %04X %04X %04X\n",
		 wiiext->id.field[0], wiiext->id.field[1], wiiext->id.field[2]);

	snprintf(wiiext->subdev_name, sizeof(wiiext->subdev_name), "wiiext-%04x-%04x-%04x",
		 wiiext->id.field[0], wiiext->id.field[1], wiiext->id.field[2]);
	wiiext->subdev.name = wiiext->subdev_name;

	ret = devm_mfd_add_devices(&client->dev, PLATFORM_DEVID_NONE,
				   &wiiext->subdev, 1, NULL, 0, NULL);
	if (ret < 0) {
		dev_info(&client->dev, "Failed to add subdev: %d\n", ret);
		return ret;
	}

	return 0;
}

static void wiiext_presence_queue_work(struct wiiext *wiiext)
{
	unsigned long delay;

	delay = msecs_to_jiffies(wiiext->presence_poll_interval);
	if (delay >= HZ)
		delay = round_jiffies_relative(delay);

	queue_delayed_work(system_freezable_wq,
			   &wiiext->presence_work, delay);
}

static void wiiext_presence_work(struct work_struct *work)
{
	struct wiiext *wiiext = container_of(work, struct wiiext, presence_work.work);
	struct i2c_client *client = wiiext->client;
	int ret;
	u8 dummy;

	if (wiiext->present) {
		/* Check if it's still there */
		ret = wiiext_read(wiiext, 0, &dummy, 1);
		if (ret < 0) {
			wiiext->present = false;
			if (ret == -ENXIO)
				dev_info(&client->dev, "Device disconnected\n");
			else
				dev_info(&client->dev, "Device error: %d\n", ret);

			mfd_remove_devices(&client->dev);
		}
	} else {
		/* Check if something new appeared */
		ret = wiiext_discover(wiiext);
		if (ret == 0)
			wiiext->present = true;
	}

	wiiext_presence_queue_work(wiiext);
}

static int wiiext_probe(struct i2c_client *client)
{
	struct wiiext *wiiext;

	wiiext = devm_kzalloc(&client->dev, sizeof(*wiiext), GFP_KERNEL);
	if (!wiiext)
		return -ENOMEM;

	wiiext->client = client;
	i2c_set_clientdata(client, wiiext);

	wiiext->present = false;
	wiiext->presence_poll_interval = 250;
	wiiext->input_poll_interval = 10;
	mutex_init(&wiiext->mutex);

	INIT_DELAYED_WORK(&wiiext->presence_work, wiiext_presence_work);
	wiiext_presence_queue_work(wiiext);

	return 0;
}

static void wiiext_remove(struct i2c_client *client)
{
	struct wiiext *wiiext = i2c_get_clientdata(client);

	cancel_delayed_work_sync(&wiiext->presence_work);
}


#ifdef CONFIG_OF
static const struct of_device_id wiiext_of_match[] = {
	{ .compatible = "nintendo,wiiext" },
	{ }
};
MODULE_DEVICE_TABLE(of, wiiext_of_match);
#endif

static struct i2c_driver wiiext_driver = {
	.probe = wiiext_probe,
	.remove = wiiext_remove,
	.driver = {
		.name = "wiiext",
		.of_match_table = of_match_ptr(wiiext_of_match),
	},
};
module_i2c_driver(wiiext_driver);

MODULE_AUTHOR("J. Neuschäfer <j.ne@posteo.net>");
MODULE_DESCRIPTION("Wii extension bus");
MODULE_LICENSE("GPL");
