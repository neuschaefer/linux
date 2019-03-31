// SPDX-License-Identifier: GPL-2.0-only
// Copyright 2019 Jonathan Neuschäfer

#include <linux/rtc.h>
#include <linux/mfd/ntx_msp430.h>
#include <linux/platform_device.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/of_device.h>

struct ntx_msp430_rtc {
	struct device *dev;
	struct ntx_msp430 *mfd;
};

#define MSP430_WRITE_YEAR	0x10
#define MSP430_WRITE_MONTH	0x11
#define MSP430_WRITE_DAY	0x12
#define MSP430_WRITE_HOUR	0x13
#define MSP430_WRITE_MINUTE	0x14
#define MSP430_WRITE_SECOND	0x15

#define MSP430_READ_YM		0x20
#define MSP430_READ_DH		0x21
#define MSP430_READ_MS		0x22


static int ntx_msp430_read_time(struct device *dev, struct rtc_time *tm)
{
	struct ntx_msp430_rtc *rtc = dev_get_drvdata(dev);
	int res;

	res = ntx_msp430_read16(rtc->mfd, MSP430_READ_YM);
	if (res < 0)
		return res;

	tm->tm_year = (res >> 8) + 100;
	tm->tm_mon = (res & 0xff) - 1;

	res = ntx_msp430_read16(rtc->mfd, MSP430_READ_DH);
	if (res < 0)
		return res;

	tm->tm_mday = res >> 8;
	tm->tm_hour = res & 0xff;

	res = ntx_msp430_read16(rtc->mfd, MSP430_READ_MS);
	if (res < 0)
		return res;

	tm->tm_min = res >> 8;
	tm->tm_sec = res & 0xff;

	return 0;
}

static int ntx_msp430_set_time(struct device *dev, struct rtc_time *tm)
{
	struct ntx_msp430_rtc *rtc = dev_get_drvdata(dev);

	ntx_msp430_write8(rtc->mfd, MSP430_WRITE_YEAR, tm->tm_year - 100);
	ntx_msp430_write8(rtc->mfd, MSP430_WRITE_MONTH, tm->tm_mon + 1);
	ntx_msp430_write8(rtc->mfd, MSP430_WRITE_DAY, tm->tm_mday);
	ntx_msp430_write8(rtc->mfd, MSP430_WRITE_HOUR, tm->tm_hour);
	ntx_msp430_write8(rtc->mfd, MSP430_WRITE_MINUTE, tm->tm_min);
	ntx_msp430_write8(rtc->mfd, MSP430_WRITE_SECOND, tm->tm_sec);

	return 0;
}

static const struct rtc_class_ops ntx_msp430_rtc_ops = {
	.read_time		= ntx_msp430_read_time,
	.set_time		= ntx_msp430_set_time,
};

static int ntx_msp430_rtc_probe(struct platform_device *pdev)
{
	struct rtc_device *rtcdev;
	struct ntx_msp430_rtc *rtc;

	rtc = devm_kzalloc(&pdev->dev, sizeof(*rtc), GFP_KERNEL);
	if (!rtc)
		return -ENOMEM;

	rtc->dev = &pdev->dev;
	rtc->mfd = dev_get_drvdata(pdev->dev.parent);
	platform_set_drvdata(pdev, rtc);

	rtcdev = devm_rtc_device_register(&pdev->dev, "ntx-msp430-rtc",
					  &ntx_msp430_rtc_ops, THIS_MODULE);
	if (IS_ERR(rtcdev))
		return PTR_ERR(rtc);

	return 0;
}

static const struct of_device_id ntx_msp430_rtc_of_match[] = {
	{ .compatible = "netronix,msp430-ec-rtc" },
	{ },
};
MODULE_DEVICE_TABLE(of, ntx_msp430_rtc_of_match);

static struct platform_driver ntx_msp430_rtc_driver = {
	.driver = {
		.name = "ntx-msp430-rtc",
		.of_match_table = ntx_msp430_rtc_of_match,
	},
	.probe = ntx_msp430_rtc_probe,
};
module_platform_driver(ntx_msp430_rtc_driver);

MODULE_AUTHOR("Jonathan Neuschäfer <j.neuschaefer@gmx.net>");
MODULE_DESCRIPTION("RTC driver for Netronix MSP430 EC");
MODULE_LICENSE("GPL");
