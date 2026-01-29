// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (C) J. Neuschäfer <j.ne@posteo.net>
/*
 * The Wii Nunchuk has two buttons, a two-axis joystick, and a three-axis accelerometer.
 */

#include <linux/module.h>
#include <linux/input.h>
#include <linux/input/wiiext.h>
#include <linux/platform_device.h>

struct nunchuk_data {
	u8 sx, sy;
	u16 ax, ay, az;
	bool bc, bz;
};

struct nunchuk {
	struct device *dev;
	struct wiiext *wiiext;
	struct input_dev *input;
	struct input_dev *accel;

	/* Home positions according to calibration data */
	struct nunchuk_data home;
};

unsigned short nunchuk_keymap[] = {
	BTN_TOP,	// "C"
	BTN_TOP2,	// "Z"
};

static int nunchuk_read_report(struct nunchuk *nunchuk, u8 reg, struct nunchuk_data *data)
{
	u8 report[6];
	int ret;

	ret = wiiext_read(nunchuk->wiiext, reg, report, 6);
	if (ret)
		return ret;

	/* buttons */
	data->bc = !(report[5] & 2);
	data->bz = !(report[5] & 1);

	/* stick */
	data->sx = report[0];
	data->sy = report[1];

	/* accelerometer */
	data->ax = report[2] << 2 | ((report[5] >> 2) & 3);
	data->ay = report[3] << 2 | ((report[5] >> 4) & 3);
	data->az = report[4] << 2 | ((report[5] >> 6) & 3);

	return 0;
}

static void nunchuk_poll(struct input_dev *input)
{
	struct nunchuk *nunchuk = input_get_drvdata(input);
	struct nunchuk_data data;

	if (nunchuk_read_report(nunchuk, 0, &data))
		return;

	/* buttons */
	input_event(input, EV_KEY, nunchuk_keymap[0], data.bc);
	input_event(input, EV_KEY, nunchuk_keymap[1], data.bz);

	/* stick */
	input_report_abs(input, ABS_X, data.sx - nunchuk->home.sx);
	input_report_abs(input, ABS_Y, -(data.sy - nunchuk->home.sy));

	input_sync(input);
}

static void nunchuk_accel_poll(struct input_dev *input)
{
	struct nunchuk *nunchuk = input_get_drvdata(input);
	struct nunchuk_data data;

	if (nunchuk_read_report(nunchuk, 0, &data))
		return;

	/* accelerometer */
	input_report_abs(nunchuk->accel, ABS_X, data.ax - nunchuk->home.ax);
	input_report_abs(nunchuk->accel, ABS_Y, data.ay - nunchuk->home.ay);
	input_report_abs(nunchuk->accel, ABS_Z, data.az - nunchuk->home.az);
	input_sync(nunchuk->accel);

	input_sync(input);
}

static struct input_dev *nunchuk_allocate_accelerometer(struct nunchuk *nunchuk)
{
	struct input_dev *input;

	input = devm_input_allocate_device(nunchuk->dev);
	if (!input)
		return ERR_PTR(-ENOMEM);

	input->id.bustype = BUS_I2C;

	__set_bit(INPUT_PROP_ACCELEROMETER, input->propbit);
	input_set_capability(input, EV_ABS, ABS_X);
	input_set_capability(input, EV_ABS, ABS_Y);
	input_set_capability(input, EV_ABS, ABS_Z);

	input->name = "Wii Nunchuk accelerometer";
	input_set_drvdata(input, nunchuk);

	input_setup_polling(input, nunchuk_accel_poll);
	input_set_poll_interval(input, nunchuk->wiiext->input_poll_interval);

	return input;
}

static void nunchuk_get_calibration(struct nunchuk *nunchuk)
{
	int ret;

	ret = nunchuk_read_report(nunchuk, 0x20, &nunchuk->home);
	if (ret) {
		dev_err(nunchuk->dev, "Failed to read calibration data: %d\n", ret);
	}
}

static int nunchuk_probe(struct platform_device *pdev)
{
	struct wiiext *wiiext = dev_get_drvdata(pdev->dev.parent);
	struct input_dev *input;
	struct nunchuk *nunchuk;
	int ret;

	device_set_of_node_from_dev(&pdev->dev, pdev->dev.parent);

	nunchuk = devm_kzalloc(&pdev->dev, sizeof(*nunchuk), GFP_KERNEL);
	if (!nunchuk)
		return -ENOMEM;

	input = devm_input_allocate_device(&pdev->dev);
	if (!input)
		return -ENOMEM;

	nunchuk->dev = &pdev->dev;
	nunchuk->wiiext = wiiext;
	nunchuk->input = input;

	nunchuk_get_calibration(nunchuk);

	input_set_drvdata(input, nunchuk);

	input->name = "Wii Nunchuk";
	//input->phys = "wiiext-nunchuk/input0"; //?
	input->phys = "wiiext-0000-a420-0000/input0"; //???
	input->dev.parent = &pdev->dev;
	input->keycode = nunchuk_keymap;
	input->keycodesize = sizeof(unsigned short);
	input->keycodemax = ARRAY_SIZE(nunchuk_keymap);
	input->id.bustype = BUS_HOST; //?

	input_setup_polling(input, nunchuk_poll);
	input_set_poll_interval(input, wiiext->input_poll_interval);

	for (size_t i = 0; i < ARRAY_SIZE(nunchuk_keymap); i++)
		input_set_capability(input, EV_KEY, nunchuk_keymap[i]);

	input_set_capability(input, EV_ABS, ABS_X);
	input_set_capability(input, EV_ABS, ABS_Y);

	input_set_abs_params(input, ABS_X, -150, +150, 0, 0);
	input_set_abs_params(input, ABS_Y, -150, +150, 0, 0);

	ret = input_register_device(input);
	if (ret) {
		dev_err(&pdev->dev, "Failed to register input device: %d\n", ret);
		return ret;
	}

	nunchuk->accel = nunchuk_allocate_accelerometer(nunchuk);
	if (IS_ERR(nunchuk->accel))
		return PTR_ERR(nunchuk->accel);

	ret = input_register_device(nunchuk->accel);
	if (ret) {
		dev_err(&pdev->dev, "Failed to register input device: %d\n", ret);
		return ret;
	}

	return 0;
}

static struct platform_driver nunchuk_driver = {
	.driver = {
		.name = "wiiext-0000-a420-0000",
	},
	.probe = nunchuk_probe,
};
module_platform_driver(nunchuk_driver);

MODULE_AUTHOR("J. Neuschäfer <j.ne@posteo.net>");
MODULE_DESCRIPTION("Wii Nunchuk driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:wiiext-0000-a420-0000");
