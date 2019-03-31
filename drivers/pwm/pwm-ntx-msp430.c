// SPDX-License-Identifier: GPL-2.0-only
// Copyright 2019 Jonathan Neuschäfer

#include <linux/mfd/ntx_msp430.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/pwm.h>
#include <linux/of_device.h>
#include <linux/types.h>

struct ntx_msp430_pwm {
	struct device *dev;
	struct ntx_msp430 *mfd;
	struct pwm_chip chip;
};

static struct ntx_msp430_pwm *pwmchip_to_pwm(struct pwm_chip *chip)
{
	return container_of(chip, struct ntx_msp430_pwm, chip);
}

#define MSP430_UNK_A		0xa1
#define MSP430_UNK_B		0xa2
#define MSP430_ENABLE		0xa3
#define MSP430_PERIOD_LOW	0xa4
#define MSP430_PERIOD_HIGH	0xa5
#define MSP430_DUTY_LOW		0xa6
#define MSP430_DUTY_HIGH	0xa7

/*
 * The time base used in the EC is 8MHz, or 125ns. Period and duty cycle are
 * measured in this unit.
 */
static int ntx_msp430_pwm_config(struct pwm_chip *chip, struct pwm_device *pwm,
				 int duty_ns, int period_ns)
{
	struct ntx_msp430_pwm *ec_pwm = pwmchip_to_pwm(chip);
	uint64_t duty = duty_ns;
	uint64_t period = period_ns;

	do_div(period, 125);
	if (period > 0xffff) {
		dev_warn(ec_pwm->dev,
			 "Period is not representable in 16 bits: %llu\n", period);
		return -ERANGE;
	}

	do_div(duty, 125);
	if (duty > 0xffff) {
		dev_warn(ec_pwm->dev, "Duty cycle is not representable in 16 bits: %llu\n",
			duty);
		return -ERANGE;
	}

	ntx_msp430_write8(ec_pwm->mfd, MSP430_PERIOD_HIGH, period >> 8);
	ntx_msp430_write8(ec_pwm->mfd, MSP430_PERIOD_LOW, period);
	ntx_msp430_write8(ec_pwm->mfd, MSP430_DUTY_HIGH, duty >> 8);
	ntx_msp430_write8(ec_pwm->mfd, MSP430_DUTY_LOW, duty);

	return 0;
}

static int ntx_msp430_pwm_enable(struct pwm_chip *chip,
				 struct pwm_device *pwm)
{
	struct ntx_msp430_pwm *ec_pwm = pwmchip_to_pwm(chip);

	return ntx_msp430_write8(ec_pwm->mfd, MSP430_ENABLE, 1);
}

static void ntx_msp430_pwm_disable(struct pwm_chip *chip,
				   struct pwm_device *pwm)
{
	struct ntx_msp430_pwm *ec_pwm = pwmchip_to_pwm(chip);

	ntx_msp430_write8(ec_pwm->mfd, MSP430_ENABLE, 0);
}

static struct pwm_ops ntx_msp430_pwm_ops = {
	.config		= ntx_msp430_pwm_config,
	.enable		= ntx_msp430_pwm_enable,
	.disable	= ntx_msp430_pwm_disable,
	.owner		= THIS_MODULE,
};

static int ntx_msp430_pwm_probe(struct platform_device *pdev)
{
	struct ntx_msp430 *mfd = dev_get_drvdata(pdev->dev.parent);
	struct ntx_msp430_pwm *pwm;
	struct pwm_chip *chip;
	int res;

	pwm = devm_kzalloc(&pdev->dev, sizeof(*pwm), GFP_KERNEL);
	if (!pwm)
		return -ENOMEM;

	pwm->mfd = mfd;
	pwm->dev = &pdev->dev;

	chip = &pwm->chip;
	chip->dev = &pdev->dev;
	chip->ops = &ntx_msp430_pwm_ops;
	chip->base = -1;
	chip->npwm = 1;

	res = pwmchip_add(chip);
	if (res < 0)
		return res;

	platform_set_drvdata(pdev, pwm);

	ntx_msp430_write8(pwm->mfd, MSP430_ENABLE, 0);
	ntx_msp430_write8(pwm->mfd, MSP430_UNK_A, 0xff);
	ntx_msp430_write8(pwm->mfd, MSP430_UNK_B, 0xff);

	return 0;
}

static int ntx_msp430_pwm_remove(struct platform_device *pdev)
{
	struct ntx_msp430_pwm *pwm = platform_get_drvdata(pdev);
	struct pwm_chip *chip = &pwm->chip;

	return pwmchip_remove(chip);
}

static const struct of_device_id ntx_msp430_pwm_of_match[] = {
	{ .compatible = "netronix,msp430-ec-pwm" },
	{ },
};
MODULE_DEVICE_TABLE(of, ntx_msp430_pwm_of_match);

static struct platform_driver ntx_msp430_pwm_driver = {
	.driver = {
		.name = "ntx-msp430-pwm",
		.of_match_table = ntx_msp430_pwm_of_match,
	},
	.probe = ntx_msp430_pwm_probe,
	.remove = ntx_msp430_pwm_remove,
};
module_platform_driver(ntx_msp430_pwm_driver);

MODULE_AUTHOR("Jonathan Neuschäfer <j.neuschaefer@gmx.net>");
MODULE_DESCRIPTION("PWM driver for Netronix MSP430 EC");
MODULE_LICENSE("GPL");
