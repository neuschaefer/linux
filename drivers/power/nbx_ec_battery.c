/*
 * Copyright (C) 2011-2012 Sony Corporation
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
 * published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
/*
  NBX EC Battery
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/debugfs.h>
#include <linux/power_supply.h>
#include <linux/wakelock.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/suspend.h>
#include <linux/wakelock.h>

#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif

#include <linux/nbx_ec_ipc.h>
#include <linux/nbx_ec_ipc_acplug.h>
#include <linux/nbx_ec_battery.h>

#define EC_IPC_CID_BATTERY_CHARGE_MODE 0x14
#define EC_IPC_CID_BATTERY_INFO_REQUEST 0x15
#define EC_IPC_CID_BATTERY_INFO_EVENT 0x16

#define INITIAL_POLLING_INTERVAL 0 /* Initial disable */
static struct workqueue_struct* nbx_ec_battery_poll_workqueue;
static struct delayed_work nbx_ec_battery_poll_work;

#define DEFAULT_BATTERY_TEMP 200
#define DEFAULT_BATTERY_PERCENT 50
#define DEFAULT_BATTERY_VOLTAGE 3700

#define BATTERY_DEGRADATION_UNKNOWN 0
#define BATTERY_DEGRADATION_GOOD 1
#define BATTERY_DEGRADATION_WARNING 2
#define BATTERY_DEGRADATION_DEGRADED 3

static enum power_supply_property nbx_ec_battery_properties[] = {
	POWER_SUPPLY_PROP_STATUS,
	POWER_SUPPLY_PROP_HEALTH,
	POWER_SUPPLY_PROP_PRESENT,
	POWER_SUPPLY_PROP_TECHNOLOGY,
	POWER_SUPPLY_PROP_CAPACITY,
	POWER_SUPPLY_PROP_VOLTAGE_NOW,
	POWER_SUPPLY_PROP_TEMP
};

static enum power_supply_property nbx_ec_power_properties[] = {
	POWER_SUPPLY_PROP_ONLINE,
};

static char *supply_list[] = {
	"battery",
};

static int nbx_ec_power_get_property(struct power_supply *psy,
				enum power_supply_property psp,
				union power_supply_propval *val);

static int nbx_ec_battery_get_property(struct power_supply *psy,
				enum power_supply_property psp,
				union power_supply_propval *val);

static struct power_supply nbx_ec_supplies[] = {
	{
		.name = "battery",
		.type = POWER_SUPPLY_TYPE_BATTERY,
		.properties = nbx_ec_battery_properties,
		.num_properties = ARRAY_SIZE(nbx_ec_battery_properties),
		.get_property = nbx_ec_battery_get_property,
	},
	{
		.name = "ac",
		.type = POWER_SUPPLY_TYPE_MAINS,
		.supplied_to = supply_list,
		.num_supplicants = ARRAY_SIZE(supply_list),
		.properties = nbx_ec_power_properties,
		.num_properties = ARRAY_SIZE(nbx_ec_power_properties),
		.get_property = nbx_ec_power_get_property,
	},
};

struct nbx_ec_battery_dev_t {
	uint32_t batt_status;
	uint32_t batt_vol;
	int32_t  batt_temp;
	uint32_t batt_health;
	uint32_t BatteryLifePercent;
	uint32_t ACLineStatus;
	uint32_t batt_status_poll_period;
	int      batt_degrade;
	int      present;
};

static struct nbx_ec_battery_dev_t *batt_dev;
static DEFINE_SEMAPHORE(batt_dev_mutex);

#define LOCKED_STATEMENT(mutex, statement)	\
	down(mutex);				\
	do {					\
		statement;			\
	} while(0);				\
	up(mutex);

static void nbx_ec_battery_check_empty(void);

static ssize_t nbx_ec_battery_show_property(
	struct device *dev,
	struct device_attribute *attr,
	char *buf)
{
	uint32_t poll_period = 0;
	LOCKED_STATEMENT(&batt_dev_mutex,
			poll_period = batt_dev->batt_status_poll_period;
		);

	return snprintf(buf, PAGE_SIZE, "%d\n", poll_period);
}

static ssize_t nbx_ec_battery_store_property(
	struct device *dev,
	struct device_attribute *attr,
	const char *buf,
	size_t count)
{
	unsigned int value = 0;
	value = simple_strtoul(buf, NULL, 0);

	LOCKED_STATEMENT(&batt_dev_mutex,
			batt_dev->batt_status_poll_period = value;
		);

	if(0 < value) {
		if(nbx_ec_battery_poll_workqueue != NULL) {
			cancel_delayed_work(&nbx_ec_battery_poll_work);
			queue_delayed_work(nbx_ec_battery_poll_workqueue, &nbx_ec_battery_poll_work,
					msecs_to_jiffies(value));
		}
	}

	return count;
}

static struct device_attribute nbx_ec_battery_attr = {
	.attr = { .name = "status_poll_period", .mode = S_IRUGO | S_IWUSR },
	.show = nbx_ec_battery_show_property,
	.store = nbx_ec_battery_store_property,
};

static ssize_t nbx_ec_battery_show_degradation(
	struct device *dev,
	struct device_attribute *attr,
	char *buf)
{
	ssize_t ret;
	int batt_degrade = BATTERY_DEGRADATION_UNKNOWN;

	LOCKED_STATEMENT(&batt_dev_mutex,
			batt_degrade = batt_dev->batt_degrade;
		);

	if(batt_degrade == BATTERY_DEGRADATION_GOOD) {
		ret = snprintf(buf, PAGE_SIZE, "Good");
	}
	else if(batt_degrade == BATTERY_DEGRADATION_WARNING) {
		ret = snprintf(buf, PAGE_SIZE, "Warning");
	}
	else if(batt_degrade == BATTERY_DEGRADATION_DEGRADED) {
		ret = snprintf(buf, PAGE_SIZE, "Degraded");
	}
	else {
		ret = snprintf(buf, PAGE_SIZE, "Unknown");
	}

	return ret;
}

static struct device_attribute nbx_ec_battery_degradation_attr = {
	.attr = { .name = "degradation", .mode = S_IRUGO },
	.show = nbx_ec_battery_show_degradation,
	.store = NULL,
};

static void nbx_ec_acplug_changed(void)
{
	int i;
	int acplug_state;

	if(batt_dev == NULL) return;

	acplug_state = nbx_ec_ipc_acplug_get_state();

	if(0 <= acplug_state) {
		LOCKED_STATEMENT(&batt_dev_mutex,
				batt_dev->ACLineStatus = acplug_state;
			);

		for (i = 0; i < ARRAY_SIZE(nbx_ec_supplies); i++) {
			if(0 == strcmp(nbx_ec_supplies[i].name, "ac") ){
				power_supply_changed(&nbx_ec_supplies[i]);
			}
		}
	}

	nbx_ec_battery_check_empty();
}

static int nbx_ec_battery_info(void)
{
	ssize_t ret;

	enum {
		BATTERY_INFO_RES_INFO = 0,
		BATTERY_INFO_RES_STATUS,
		BATTERY_INFO_RES_EXT_STATUS,
		BATTERY_INFO_RES_REMAIN_L,
		BATTERY_INFO_RES_REMAIN_H,
		BATTERY_INFO_RES_VOLT_L,
		BATTERY_INFO_RES_VOLT_H,
		BATTERY_INFO_RES_TEMP_L,
		BATTERY_INFO_RES_TEMP_H,
		NOOF_BATTERY_INFO_RES,
	};
	uint8_t res_buf[NOOF_BATTERY_INFO_RES];

	enum {
		BATTERY_INFO_RES_INFO_UNKNOWN     = 0x00,
		BATTERY_INFO_RES_INFO_CHARGING    = 0x01,
		BATTERY_INFO_RES_INFO_DISCHARGING = 0x02,
		BATTERY_INFO_RES_INFO_NOTCHARGING = 0x03,
		BATTERY_INFO_RES_INFO_FULL        = 0x04,
		BATTERY_INFO_RES_INFO_MASK        = 0x07,
	};
	enum {
		BATTERY_INFO_RES_STATUS_DEGRADATION_UNKNOWN = 0x00,
		BATTERY_INFO_RES_STATUS_DEGRADATION_GOOD    = 0x10,
		BATTERY_INFO_RES_STATUS_DEGRADATION_WARNING = 0x20,
		BATTERY_INFO_RES_STATUS_DEGRADATION_DEGRADE = 0x30,
		BATTERY_INFO_RES_STATUS_DEGRADATION_MASK    = 0x30,
	};
	enum {
		BATTERY_INFO_RES_STATUS_HEALTH_UNKNOWN  = 0x00,
		BATTERY_INFO_RES_STATUS_HEALTH_GOOD     = 0x01,
		BATTERY_INFO_RES_STATUS_HEALTH_OVERHEAT = 0x02,
		BATTERY_INFO_RES_STATUS_HEALTH_DEAD     = 0x03,
		BATTERY_INFO_RES_STATUS_HEALTH_OVERVOLT = 0x04,
		BATTERY_INFO_RES_STATUS_HEALTH_UNSPEC   = 0x05,
		BATTERY_INFO_RES_STATUS_HEALTH_COLD     = 0x06,
		BATTERY_INFO_RES_STATUS_HEALTH_MASK     = 0x0F,
	};
	enum {
		BATTERY_INFO_RES_EXT_STATUS_CHARGE_FAULT = 0x01,
	};

	if(batt_dev == NULL) return -EFAULT;

	ret = ec_ipc_send_request(EC_IPC_PID_BATTERY,
				EC_IPC_CID_BATTERY_INFO_REQUEST,
				NULL, 0,
				res_buf, sizeof(res_buf));
	if(ret < (int)sizeof(res_buf)){
		pr_err("nbx_ec_battery:battery_info:ec_ipc_send_request failed. %d(%d)\n", ret, (int)sizeof(res_buf));
		return -EIO;
	}

	LOCKED_STATEMENT(&batt_dev_mutex,
			switch(res_buf[BATTERY_INFO_RES_INFO] & BATTERY_INFO_RES_INFO_MASK) {
			case BATTERY_INFO_RES_INFO_CHARGING:
				batt_dev->batt_status = POWER_SUPPLY_STATUS_CHARGING;
				batt_dev->present = 1;
				break;
			case BATTERY_INFO_RES_INFO_DISCHARGING:
				batt_dev->batt_status = POWER_SUPPLY_STATUS_DISCHARGING;
				batt_dev->present = 1;
				break;
			case BATTERY_INFO_RES_INFO_NOTCHARGING:
				batt_dev->batt_status = POWER_SUPPLY_STATUS_NOT_CHARGING;
				batt_dev->present = 1;
				break;
			case BATTERY_INFO_RES_INFO_FULL:
				batt_dev->batt_status = POWER_SUPPLY_STATUS_FULL;
				batt_dev->present = 1;
				break;
			default:
				batt_dev->batt_status = POWER_SUPPLY_STATUS_UNKNOWN;
				batt_dev->present = 0;
				break;
			}

			switch(res_buf[BATTERY_INFO_RES_STATUS] & BATTERY_INFO_RES_STATUS_DEGRADATION_MASK) {
			case BATTERY_INFO_RES_STATUS_DEGRADATION_GOOD:
				batt_dev->batt_degrade = BATTERY_DEGRADATION_GOOD;
				break;
			case BATTERY_INFO_RES_STATUS_DEGRADATION_WARNING:
				batt_dev->batt_degrade = BATTERY_DEGRADATION_WARNING;
				break;
			case BATTERY_INFO_RES_STATUS_DEGRADATION_DEGRADE:
				batt_dev->batt_degrade = BATTERY_DEGRADATION_DEGRADED;
				break;
			default:
				batt_dev->batt_degrade = BATTERY_DEGRADATION_UNKNOWN;
				break;
			}

			switch(res_buf[BATTERY_INFO_RES_STATUS] & BATTERY_INFO_RES_STATUS_HEALTH_MASK) {
			case BATTERY_INFO_RES_STATUS_HEALTH_GOOD:
				batt_dev->batt_health = POWER_SUPPLY_HEALTH_GOOD;
				break;
			case BATTERY_INFO_RES_STATUS_HEALTH_OVERHEAT:
				batt_dev->batt_health = POWER_SUPPLY_HEALTH_OVERHEAT;
				break;
			case BATTERY_INFO_RES_STATUS_HEALTH_DEAD:
				batt_dev->batt_health = POWER_SUPPLY_HEALTH_DEAD;
				break;
			case BATTERY_INFO_RES_STATUS_HEALTH_OVERVOLT:
				batt_dev->batt_health = POWER_SUPPLY_HEALTH_OVERVOLTAGE;
				break;
			case BATTERY_INFO_RES_STATUS_HEALTH_UNSPEC:
				batt_dev->batt_health = POWER_SUPPLY_HEALTH_UNSPEC_FAILURE;
				break;
			case BATTERY_INFO_RES_STATUS_HEALTH_COLD:
				batt_dev->batt_health = POWER_SUPPLY_HEALTH_COLD;
				break;
			default:
				batt_dev->batt_health = POWER_SUPPLY_HEALTH_UNKNOWN;
				break;
			}

			/* ignore BATTERY_INFO_RES_REMAIN_L */
			batt_dev->BatteryLifePercent = res_buf[BATTERY_INFO_RES_REMAIN_H];

			batt_dev->batt_vol =
			(((uint32_t)(res_buf[BATTERY_INFO_RES_VOLT_H])) << 8) +
			res_buf[BATTERY_INFO_RES_VOLT_L]; /* mV */

			batt_dev->batt_temp = (int16_t)((((uint16_t)(res_buf[BATTERY_INFO_RES_TEMP_H])) << 8) +
							res_buf[BATTERY_INFO_RES_TEMP_L]);
			batt_dev->batt_temp = (batt_dev->batt_temp * 10) / 256; /* 1/256C to 1/10C */
		);

	return 0;
}

int nbx_ec_battery_max_charge_mode(uint8_t mode)
{
	ssize_t ret;

	enum {
		BATTERY_CHARGE_MODE_REQ_MODE = 0,
		NOOF_BATTERY_CHARGE_MODE_REQ,
	};
	uint8_t req_buf[NOOF_BATTERY_CHARGE_MODE_REQ];

	enum {
		BATTERY_CHARGE_MODE_RES_RESULT = 0,
		NOOF_BATTERY_CHARGE_MODE_RES,
	};
	uint8_t res_buf[NOOF_BATTERY_CHARGE_MODE_RES];

#define BATTERY_CHARGE_MODE_RES_RESULT_OK 0
#define BATTERY_CHARGE_MODE_RES_RESULT_NG 1

	req_buf[BATTERY_CHARGE_MODE_REQ_MODE] = mode;

	ret = ec_ipc_send_request(EC_IPC_PID_BATTERY,
				EC_IPC_CID_BATTERY_CHARGE_MODE,
				req_buf, sizeof(req_buf),
				res_buf, sizeof(res_buf));

	if(ret < (int)sizeof(res_buf)){
		pr_err("nbx_ec_battery_max_charge_mode:ec_ipc_send_request failed. %d(%d)\n", ret, (int)sizeof(res_buf));
		return -EIO;
	}
	if(res_buf[BATTERY_CHARGE_MODE_RES_RESULT] != BATTERY_CHARGE_MODE_RES_RESULT_OK) {
		pr_err("nbx_ec_battery_max_charge_mode result NG. \n");
		return -EINVAL;
	}

	return 0;
}
EXPORT_SYMBOL(nbx_ec_battery_max_charge_mode);

static int nbx_ec_power_get_property(struct power_supply *psy,
				enum power_supply_property psp, union power_supply_propval *val)
{
	switch (psp) {

	case POWER_SUPPLY_PROP_ONLINE:
		if (psy->type == POWER_SUPPLY_TYPE_MAINS) {
			LOCKED_STATEMENT(&batt_dev_mutex,
					val->intval = batt_dev->ACLineStatus;
				);
		}
		else
			val->intval = 0;
		break;

	default:
		return -EINVAL;
	}
	return 0;
}

static int nbx_ec_battery_get_property(struct power_supply *psy,
				enum power_supply_property psp,
				union power_supply_propval *val)
{
	switch (psp) {

	case POWER_SUPPLY_PROP_STATUS:
		LOCKED_STATEMENT(&batt_dev_mutex,
				if (batt_dev->present)
					val->intval = batt_dev->batt_status;
				else
					val->intval = POWER_SUPPLY_STATUS_UNKNOWN;
			);
		break;

	case POWER_SUPPLY_PROP_PRESENT:
		LOCKED_STATEMENT(&batt_dev_mutex,
				val->intval = batt_dev->present;
			);
		break;

	case POWER_SUPPLY_PROP_TECHNOLOGY:
		LOCKED_STATEMENT(&batt_dev_mutex,
				if (batt_dev->present)
					val->intval = POWER_SUPPLY_TECHNOLOGY_LION; /* fixed */
				else
					val->intval = POWER_SUPPLY_TECHNOLOGY_UNKNOWN;
			);

		break;

	case POWER_SUPPLY_PROP_HEALTH:
		LOCKED_STATEMENT(&batt_dev_mutex,
				if (batt_dev->present)
					val->intval = batt_dev->batt_health;
				else
					val->intval = POWER_SUPPLY_HEALTH_UNKNOWN;
			);
		break;

	case POWER_SUPPLY_PROP_VOLTAGE_NOW:
		LOCKED_STATEMENT(&batt_dev_mutex,
				if (batt_dev->present)
					val->intval = batt_dev->batt_vol * 1000; /* microvolt */
				else
					val->intval = DEFAULT_BATTERY_VOLTAGE * 1000; /* microvolt */
			);
		break;

	case POWER_SUPPLY_PROP_CAPACITY:
		LOCKED_STATEMENT(&batt_dev_mutex,
				if (batt_dev->present)
					val->intval = batt_dev->BatteryLifePercent;
				else
					val->intval = DEFAULT_BATTERY_PERCENT;
			);
		break;

	case POWER_SUPPLY_PROP_TEMP:
		LOCKED_STATEMENT(&batt_dev_mutex,
				if (batt_dev->present) {
					val->intval = batt_dev->batt_temp;

					// FIXME : Provisional doesn't become 68.0C or more
					if(679 < val->intval) val->intval = 679;
				}
				else {
					val->intval = DEFAULT_BATTERY_TEMP;
				}
			);
		break;

	default:
		return -EINVAL;
	}

	return 0;
}

static void nbx_ec_battery_receive_event(const uint8_t* buf, int size)
{
	if(buf == NULL) return;
	if(nbx_ec_battery_poll_workqueue == NULL) return;

	/* exec work, now */
	cancel_delayed_work(&nbx_ec_battery_poll_work);
	queue_delayed_work(nbx_ec_battery_poll_workqueue, &nbx_ec_battery_poll_work, 0);
}

static struct wake_lock nbx_ec_battery_wake_lock;

static struct wake_lock nbx_ec_battery_empty_shutdown_wake_lock;

static void nbx_ec_battery_check_empty(void)
{
	int empty = 0;

	/* Shutdown when AC unplugged & Battery is empty. */
	LOCKED_STATEMENT(&batt_dev_mutex,
			if( (batt_dev->ACLineStatus == 0) && ((batt_dev->present) && (batt_dev->BatteryLifePercent <= 0)) ){
				empty = 1;
			}
		);

	if(empty) {
		wake_lock(&nbx_ec_battery_empty_shutdown_wake_lock);
	}
	else {
		wake_unlock(&nbx_ec_battery_empty_shutdown_wake_lock);
	}
}

static void nbx_ec_battery_poll_func(struct work_struct* work)
{
	int result;
	int i;
	uint32_t poll_period = INITIAL_POLLING_INTERVAL;

	result = nbx_ec_battery_info();
	if(0 <= result) {
		for (i = 0; i < ARRAY_SIZE(nbx_ec_supplies); i++) {
			if(0 == strcmp(nbx_ec_supplies[i].name, "battery") ){
				power_supply_changed(&nbx_ec_supplies[i]);
			}
		}

		nbx_ec_battery_check_empty();
	}

	nbx_ec_ipc_acplug_request_update();

	/* next */
	LOCKED_STATEMENT(&batt_dev_mutex,
			poll_period = batt_dev->batt_status_poll_period;
		);

	if(0 < poll_period) {
		queue_delayed_work(nbx_ec_battery_poll_workqueue, &nbx_ec_battery_poll_work,
				msecs_to_jiffies(poll_period));
	}

	wake_unlock(&nbx_ec_battery_wake_lock);
}

#ifdef CONFIG_SUSPEND

static int nbx_ec_battery_suspend(struct nbx_ec_ipc_device *edev)
{
	cancel_delayed_work_sync(&nbx_ec_battery_poll_work);

	return 0;
}
static int nbx_ec_battery_resume(struct nbx_ec_ipc_device *edev)
{
	if(nbx_ec_battery_poll_workqueue != NULL) {
		wake_lock_timeout(&nbx_ec_battery_wake_lock, 10 * HZ);
		queue_delayed_work(nbx_ec_battery_poll_workqueue, &nbx_ec_battery_poll_work, 0);
	}

	return 0;
}

#else /* !CONFIG_SUSPEND */
#define nbx_ec_battery_suspend NULL
#define nbx_ec_battery_resume NULL
#endif /* CONFIG_SUSPEND */

static int nbx_ec_battery_probe(struct nbx_ec_ipc_device *edev)
{
	int i, rc = 0;
	struct nbx_ec_battery_platform_data* batt_pdata =
		(struct nbx_ec_battery_platform_data*)(edev->dev.platform_data);

	nbx_ec_battery_poll_workqueue = NULL;

	LOCKED_STATEMENT(&batt_dev_mutex,
			batt_dev = kzalloc(sizeof(*batt_dev), GFP_KERNEL);
			if (!batt_dev) {
				rc = -ENOMEM;
				break;
			}
			memset(batt_dev, 0, sizeof(*batt_dev));

			/* Assume battery is present at start */
			batt_dev->present = 1;
			batt_dev->ACLineStatus = 1;
			batt_dev->BatteryLifePercent = DEFAULT_BATTERY_PERCENT;
			batt_dev->batt_vol = DEFAULT_BATTERY_VOLTAGE;
			batt_dev->batt_temp = DEFAULT_BATTERY_TEMP;
			batt_dev->batt_status = POWER_SUPPLY_STATUS_UNKNOWN;
			batt_dev->batt_health = POWER_SUPPLY_HEALTH_UNKNOWN;
			batt_dev->batt_status_poll_period = INITIAL_POLLING_INTERVAL;
			batt_dev->batt_degrade = BATTERY_DEGRADATION_UNKNOWN;
		);

	if(rc) goto error_exit;

	for (i = 0; i < ARRAY_SIZE(nbx_ec_supplies); i++) {
		rc = power_supply_register(&edev->dev, &nbx_ec_supplies[i]);
		if (rc) {
			pr_err("Failed to register power supply\n");
			goto error_exit;
		}
	}
	pr_info("%s: battery driver registered\n", edev->name);

	nbx_ec_battery_poll_workqueue = create_singlethread_workqueue("nbx_ec_battery_poll_workqueue");
	if(nbx_ec_battery_poll_workqueue == NULL) {
		pr_err("nbx_ec_battery:create_singlethread_workqueue() failed.\n");
		rc = -ENOMEM;
		goto error_exit;
	}

	rc = device_create_file(&edev->dev, &nbx_ec_battery_attr);
	if (rc) {
		pr_err("nbx_ec_battery_probe:device_create_file FAILED\n");
		goto error_exit;
	}

	if(batt_pdata->degradation) {
		rc = device_create_file(&edev->dev, &nbx_ec_battery_degradation_attr);
		if (rc) {
			pr_err("nbx_ec_battery_probe:device_create_file FAILED\n");
			goto error_exit;
		}
	}

	INIT_DELAYED_WORK(&nbx_ec_battery_poll_work, nbx_ec_battery_poll_func);

	wake_lock_init(&nbx_ec_battery_wake_lock, WAKE_LOCK_SUSPEND, "nbx_ec_battery");
	wake_lock_init(&nbx_ec_battery_empty_shutdown_wake_lock, WAKE_LOCK_SUSPEND, "nbx_ec_battery_empty_shutdown");

	/* Initial read */
	queue_delayed_work(nbx_ec_battery_poll_workqueue, &nbx_ec_battery_poll_work, 0);

	ec_ipc_register_recv_event(EC_IPC_CID_BATTERY_INFO_EVENT, nbx_ec_battery_receive_event);
	nbx_ec_ipc_acplug_register_callback(nbx_ec_acplug_changed);

	nbx_ec_acplug_changed(); /* initialize */

	return 0;

error_exit:
	ec_ipc_unregister_recv_event(EC_IPC_CID_BATTERY_INFO_EVENT);
	nbx_ec_ipc_acplug_unregister_callback(nbx_ec_acplug_changed);

	if(nbx_ec_battery_poll_workqueue != NULL) {
		destroy_workqueue(nbx_ec_battery_poll_workqueue);
	}
	nbx_ec_battery_poll_workqueue = NULL;

	for (i = 0; i < ARRAY_SIZE(nbx_ec_supplies); i++) {
		power_supply_unregister(&nbx_ec_supplies[i]);
	}

	LOCKED_STATEMENT(&batt_dev_mutex,
			kfree(batt_dev);
			batt_dev = NULL;
		);

	return rc;
}

static int nbx_ec_battery_remove(struct nbx_ec_ipc_device *edev)
{
	int i;
	struct nbx_ec_battery_platform_data* batt_pdata =
		(struct nbx_ec_battery_platform_data*)(edev->dev.platform_data);

	ec_ipc_unregister_recv_event(EC_IPC_CID_BATTERY_INFO_EVENT);
	nbx_ec_ipc_acplug_unregister_callback(nbx_ec_acplug_changed);

	if(nbx_ec_battery_poll_workqueue != NULL) {
		cancel_delayed_work_sync(&nbx_ec_battery_poll_work);
		flush_workqueue(nbx_ec_battery_poll_workqueue);
		destroy_workqueue(nbx_ec_battery_poll_workqueue);
	}
	nbx_ec_battery_poll_workqueue = NULL;

	for (i = 0; i < ARRAY_SIZE(nbx_ec_supplies); i++) {
		power_supply_unregister(&nbx_ec_supplies[i]);
	}

	if (batt_dev) {
		device_remove_file(&edev->dev, &nbx_ec_battery_attr);

		if(batt_pdata->degradation) {
			device_remove_file(&edev->dev, &nbx_ec_battery_degradation_attr);
		}

		LOCKED_STATEMENT(&batt_dev_mutex,
				kfree(batt_dev);
				batt_dev = NULL;
			);
	}

	wake_lock_destroy(&nbx_ec_battery_wake_lock);
	wake_lock_destroy(&nbx_ec_battery_empty_shutdown_wake_lock);

	return 0;
}

static struct nbx_ec_ipc_driver nbx_ec_battery_driver =
{
	.probe   = nbx_ec_battery_probe,
	.remove  = nbx_ec_battery_remove,
	.suspend = nbx_ec_battery_suspend,
	.resume  = nbx_ec_battery_resume,
	.drv     = {
		.name     = "nbx_battery",
	},
};

static int __init nbx_ec_battery_init(void)
{
	int ret;

	ret = nbx_ec_ipc_driver_register(&nbx_ec_battery_driver);
	if (ret < 0) {
		pr_err("%s:nbx_ec_ipc_driver_register() failed, %d\n", __func__, ret);
		return ret;
	}

	return 0;
}

static void __exit nbx_ec_battery_exit(void)
{
	nbx_ec_ipc_driver_unregister(&nbx_ec_battery_driver);
}

module_init(nbx_ec_battery_init);
module_exit(nbx_ec_battery_exit);

MODULE_LICENSE("GPL");
