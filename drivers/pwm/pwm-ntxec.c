// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * The Netronix embedded controller is a microcontroller found in some
 * e-book readers designed by the ODM Netronix, Inc. It contains RTC,
 * battery monitoring, system power management, and PWM functionality.
 *
 * This driver implements PWM output.
 *
 * Copyright 2020 Jonathan Neuschäfer <j.neuschaefer@gmx.net>
 */

#include <linux/mfd/ntxec.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/pwm.h>
#include <linux/regmap.h>
#include <linux/types.h>

struct ntxec_pwm {
	struct device *dev;
	struct ntxec *ec;
	struct pwm_chip chip;
};

static struct ntxec_pwm *pwmchip_to_pwm(struct pwm_chip *chip)
{
	return container_of(chip, struct ntxec_pwm, chip);
}

#define NTXEC_REG_AUTO_OFF_HI	0xa1
#define NTXEC_REG_AUTO_OFF_LO	0xa2
#define NTXEC_REG_ENABLE	0xa3
#define NTXEC_REG_PERIOD_LOW	0xa4
#define NTXEC_REG_PERIOD_HIGH	0xa5
#define NTXEC_REG_DUTY_LOW	0xa6
#define NTXEC_REG_DUTY_HIGH	0xa7

/*
 * The time base used in the EC is 8MHz, or 125ns. Period and duty cycle are
 * measured in this unit.
 */
#define TIME_BASE_NS 125

/*
 * The maximum input value (in nanoseconds) is determined by the time base and
 * the range of the hardware registers that hold the converted value.
 * It fits into 32 bits, so we can do our calculations in 32 bits as well.
 */
#define MAX_PERIOD_NS (TIME_BASE_NS * 0x10000 - 1)

static int ntxec_pwm_apply(struct pwm_chip *chip, struct pwm_device *pwm_dev,
			   const struct pwm_state *state)
{
	struct ntxec_pwm *pwm = pwmchip_to_pwm(pwm_dev->chip);
	unsigned int duty = state->duty_cycle;
	unsigned int period = state->period;
	int res = 0;

	if (period > MAX_PERIOD_NS) {
		dev_warn(pwm->dev,
			 "Period is not representable in 16 bits after division by %u: %u\n",
			 TIME_BASE_NS, period);
		return -ERANGE;
	}

	period /= TIME_BASE_NS;
	duty /= TIME_BASE_NS;

	res = regmap_write(pwm->ec->regmap, NTXEC_REG_PERIOD_HIGH, ntxec_reg8(period >> 8));
	if (res)
		return res;

	res = regmap_write(pwm->ec->regmap, NTXEC_REG_PERIOD_LOW, ntxec_reg8(period));
	if (res)
		return res;

	res = regmap_write(pwm->ec->regmap, NTXEC_REG_DUTY_HIGH, ntxec_reg8(duty >> 8));
	if (res)
		return res;

	res = regmap_write(pwm->ec->regmap, NTXEC_REG_DUTY_LOW, ntxec_reg8(duty));
	if (res)
		return res;

	/*
	 * Writing a duty cycle of zone puts the device into a state where
	 * writing a higher duty cycle doesn't result in the brightness that it
	 * usually results in. This can be fixed by cycling the ENABLE register.
	 *
	 * As a workaround, write ENABLE=0 when the duty cycle is zero.
	 */
	if (state->enabled && duty != 0) {
		res = regmap_write(pwm->ec->regmap, NTXEC_REG_ENABLE, ntxec_reg8(1));
		if (res)
			return res;

		/* Disable the auto-off timer */
		res = regmap_write(pwm->ec->regmap, NTXEC_REG_AUTO_OFF_HI, ntxec_reg8(0xff));
		if (res)
			return res;

		return regmap_write(pwm->ec->regmap, NTXEC_REG_AUTO_OFF_LO, ntxec_reg8(0xff));
	} else {
		return regmap_write(pwm->ec->regmap, NTXEC_REG_ENABLE, ntxec_reg8(0));
	}
}

static struct pwm_ops ntxec_pwm_ops = {
	.apply = ntxec_pwm_apply,
	.owner = THIS_MODULE,
};

static int ntxec_pwm_probe(struct platform_device *pdev)
{
	struct ntxec *ec = dev_get_drvdata(pdev->dev.parent);
	struct ntxec_pwm *pwm;
	struct pwm_chip *chip;
	int res;

	pwm = devm_kzalloc(&pdev->dev, sizeof(*pwm), GFP_KERNEL);
	if (!pwm)
		return -ENOMEM;

	pwm->ec = ec;
	pwm->dev = &pdev->dev;

	chip = &pwm->chip;
	chip->dev = &pdev->dev;
	chip->ops = &ntxec_pwm_ops;
	chip->base = -1;
	chip->npwm = 1;

	res = pwmchip_add(chip);
	if (res < 0)
		return res;

	platform_set_drvdata(pdev, pwm);

	return 0;
}

static int ntxec_pwm_remove(struct platform_device *pdev)
{
	struct ntxec_pwm *pwm = platform_get_drvdata(pdev);
	struct pwm_chip *chip = &pwm->chip;

	return pwmchip_remove(chip);
}

static struct platform_driver ntxec_pwm_driver = {
	.driver = {
		.name = "ntxec-pwm",
	},
	.probe = ntxec_pwm_probe,
	.remove = ntxec_pwm_remove,
};
module_platform_driver(ntxec_pwm_driver);

MODULE_AUTHOR("Jonathan Neuschäfer <j.neuschaefer@gmx.net>");
MODULE_DESCRIPTION("PWM driver for Netronix EC");
MODULE_LICENSE("GPL");
