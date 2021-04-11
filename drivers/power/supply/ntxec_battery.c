// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * The Netronix embedded controller is a microcontroller found in some
 * e-book readers designed by the original design manufacturer Netronix, Inc.
 * It contains RTC, battery monitoring, system power management, and PWM
 * functionality.
 *
 * This driver implements battery monitoring.
 *
 * Copyright 2021 Josua Mayer <josua.mayer@jm0.eu>
 */

#include <linux/mfd/ntxec.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/power_supply.h>
#include <linux/regmap.h>

static const enum power_supply_property ntxec_battery_properties[] = {
	POWER_SUPPLY_PROP_VOLTAGE_MIN_DESIGN,
	POWER_SUPPLY_PROP_VOLTAGE_MAX_DESIGN,
	POWER_SUPPLY_PROP_ENERGY_FULL_DESIGN,
	POWER_SUPPLY_PROP_CHARGE_FULL_DESIGN,
	POWER_SUPPLY_PROP_VOLTAGE_NOW,
};

struct ntxec_battery {
	struct ntxec *ec;
	struct power_supply *psy;
	struct power_supply_battery_info info;
};

#define NTXEC_REG_READ_BATTERY	0x41

static int ntxec_battery_get_property(struct power_supply *psy,
				     enum power_supply_property psp,
				     union power_supply_propval *val)
{
	struct ntxec_battery *bat = power_supply_get_drvdata(psy);
	int ret;
	unsigned int value;

	switch (psp) {
		case POWER_SUPPLY_PROP_VOLTAGE_MIN_DESIGN:
			val->intval = bat->info.voltage_min_design_uv;
			break;
		case POWER_SUPPLY_PROP_VOLTAGE_MAX_DESIGN:
			val->intval = bat->info.voltage_max_design_uv;
			break;
		case POWER_SUPPLY_PROP_CHARGE_FULL_DESIGN:
			val->intval = bat->info.charge_full_design_uah;
			break;
		case POWER_SUPPLY_PROP_ENERGY_FULL_DESIGN:
			val->intval = bat->info.energy_full_design_uwh;
			break;
		case POWER_SUPPLY_PROP_VOLTAGE_NOW:
			ret = regmap_read(bat->ec->regmap, NTXEC_REG_READ_BATTERY, &value);
			if (ret < 0)
				return ret;

			/* ec value to microvolt conversion:
			 * vendor kernel source suggests linear behaviour from 3V to 4.2V
			 * with readings 767 to 1023; each increment represents 4687,5uV.
			 * adjust 3V boundary slightly to report exactly 4.2V when full.
			 */
			val->intval = 2999872 + (value - 767) * 4688;
			break;
		default:
			dev_err(&psy->dev, "%s: invalid property %u\n", __func__, psp);
			return -EINVAL;
	}

	return 0;
}

static const struct power_supply_desc ntxec_battery_desc = {
	.name = "ec-battery",
	.type = POWER_SUPPLY_TYPE_BATTERY,
	.properties = ntxec_battery_properties,
	.get_property = ntxec_battery_get_property,
	.num_properties = ARRAY_SIZE(ntxec_battery_properties),
};

static int ntxec_battery_probe(struct platform_device *pdev)
{
	struct ntxec_battery *bat;
	struct power_supply_config psy_cfg = {};
	int ret;

	pdev->dev.of_node = pdev->dev.parent->of_node;

	bat = devm_kzalloc(&pdev->dev, sizeof(*bat), GFP_KERNEL);
	if (!bat)
		return -ENOMEM;
	platform_set_drvdata(pdev, bat);

	psy_cfg.of_node = pdev->dev.parent->of_node;
	psy_cfg.drv_data = bat;

	bat->ec = dev_get_drvdata(pdev->dev.parent);

	bat->psy = devm_power_supply_register_no_ws(&pdev->dev,
						    &ntxec_battery_desc,
						    &psy_cfg);
	if (IS_ERR(bat->psy))
		return dev_err_probe(&pdev->dev, PTR_ERR(bat->psy),
				     "failed to register battery\n");

	ret = power_supply_get_battery_info(bat->psy, &bat->info);
	if (ret) {
		dev_err_probe(&pdev->dev, ret, "failed to get battery info\n");
		return ret;
	}

	return 0;
}

static int ntxec_battery_remove(struct platform_device *pdev)
{
	struct ntxec_battery *bat = platform_get_drvdata(pdev);

	power_supply_unregister(bat->psy);

	return 0;
}

static struct platform_driver ntxec_battery_driver = {
	.driver = {
		.name = "ntxec-battery",
	},
	.probe = ntxec_battery_probe,
	.remove = ntxec_battery_remove,
};
module_platform_driver(ntxec_battery_driver);

MODULE_AUTHOR("Josua Mayer <josua.mayer@jm0.eu>");
MODULE_DESCRIPTION("Battery driver for Netronix EC");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:ntxec-battery");
