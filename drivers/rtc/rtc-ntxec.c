// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * The Netronix embedded controller is a microcontroller found in some
 * e-book readers designed by the ODM Netronix, Inc. It contains RTC,
 * battery monitoring, system power management, and PWM functionality.
 *
 * This driver implements access to the RTC time and date.
 *
 * Copyright 2020 Jonathan Neuschäfer <j.neuschaefer@gmx.net>
 */

#include <linux/mfd/ntxec.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/rtc.h>
#include <linux/types.h>

struct ntxec_rtc {
	struct device *dev;
	struct ntxec *ec;
};

#define NTXEC_REG_WRITE_YEAR	0x10
#define NTXEC_REG_WRITE_MONTH	0x11
#define NTXEC_REG_WRITE_DAY	0x12
#define NTXEC_REG_WRITE_HOUR	0x13
#define NTXEC_REG_WRITE_MINUTE	0x14
#define NTXEC_REG_WRITE_SECOND	0x15

#define NTXEC_REG_READ_YM	0x20
#define NTXEC_REG_READ_DH	0x21
#define NTXEC_REG_READ_MS	0x23

static int ntxec_read_time(struct device *dev, struct rtc_time *tm)
{
	struct ntxec_rtc *rtc = dev_get_drvdata(dev);
	unsigned int value;
	int res;

	res = regmap_read(rtc->ec->regmap, NTXEC_REG_READ_YM, &value);
	if (res < 0)
		return res;

	tm->tm_year = (value >> 8) + 100;
	tm->tm_mon = (value & 0xff) - 1;

	res = regmap_read(rtc->ec->regmap, NTXEC_REG_READ_DH, &value);
	if (res < 0)
		return res;

	tm->tm_mday = value >> 8;
	tm->tm_hour = value & 0xff;

	res = regmap_read(rtc->ec->regmap, NTXEC_REG_READ_MS, &value);
	if (res < 0)
		return res;

	tm->tm_min = value >> 8;
	tm->tm_sec = value & 0xff;

	return 0;
}

static int ntxec_set_time(struct device *dev, struct rtc_time *tm)
{
	struct ntxec_rtc *rtc = dev_get_drvdata(dev);
	int res = 0;

	res = regmap_write(rtc->ec->regmap, NTXEC_REG_WRITE_YEAR, ntxec_reg8(tm->tm_year - 100));
	if (res)
		return res;

	res = regmap_write(rtc->ec->regmap, NTXEC_REG_WRITE_MONTH, ntxec_reg8(tm->tm_mon + 1));
	if (res)
		return res;

	res = regmap_write(rtc->ec->regmap, NTXEC_REG_WRITE_DAY, ntxec_reg8(tm->tm_mday));
	if (res)
		return res;

	res = regmap_write(rtc->ec->regmap, NTXEC_REG_WRITE_HOUR, ntxec_reg8(tm->tm_hour));
	if (res)
		return res;

	res = regmap_write(rtc->ec->regmap, NTXEC_REG_WRITE_MINUTE, ntxec_reg8(tm->tm_min));
	if (res)
		return res;

	return regmap_write(rtc->ec->regmap, NTXEC_REG_WRITE_SECOND, ntxec_reg8(tm->tm_sec));
}

static const struct rtc_class_ops ntxec_rtc_ops = {
	.read_time = ntxec_read_time,
	.set_time = ntxec_set_time,
};

static int ntxec_rtc_probe(struct platform_device *pdev)
{
	struct rtc_device *dev;
	struct ntxec_rtc *rtc;

	rtc = devm_kzalloc(&pdev->dev, sizeof(*rtc), GFP_KERNEL);
	if (!rtc)
		return -ENOMEM;

	rtc->dev = &pdev->dev;
	rtc->ec = dev_get_drvdata(pdev->dev.parent);
	platform_set_drvdata(pdev, rtc);

	dev = devm_rtc_allocate_device(&pdev->dev);
	if (IS_ERR(dev))
		return PTR_ERR(dev);

	dev->ops = &ntxec_rtc_ops;
	dev->range_min = RTC_TIMESTAMP_BEGIN_2000;
	dev->range_max = 9025257599LL; /* 2255-12-31 23:59:59 */

	return rtc_register_device(dev);
}

static struct platform_driver ntxec_rtc_driver = {
	.driver = {
		.name = "ntxec-rtc",
	},
	.probe = ntxec_rtc_probe,
};
module_platform_driver(ntxec_rtc_driver);

MODULE_AUTHOR("Jonathan Neuschäfer <j.neuschaefer@gmx.net>");
MODULE_DESCRIPTION("RTC driver for Netronix EC");
MODULE_LICENSE("GPL");
