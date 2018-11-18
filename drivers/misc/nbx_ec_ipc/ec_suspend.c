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
/* EC SUSPEND */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/suspend.h>
#include <linux/reboot.h>
#include <linux/err.h>

#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif
 
#include <linux/nbx_ec_ipc.h>

#define EC_POWERSTATE_S5 0
#define EC_POWERSTATE_S3 1
#define EC_POWERSTATE_ES 2
#define EC_POWERSTATE_S0 3

static void ec_suspend_send_powerstate_change_response(const uint8_t* buf, int size, void* private_data)
{
	if((buf == NULL) || (size <= 0)) {
		pr_err("ec_suspend: ec_ipc_send_request_async(POWERSTATE_CHANGE) failed. %d\n", size);
	}
}
static int ec_suspend_send_powerstate_change(uint8_t next_state, int async)
{
	int ret;

#define EC_IPC_CID_POWERSTATE_CHANGE 0x91

	enum {
		EC_POWERSTATE_CHANGE_NEXT = 0,
		NOOF_EC_POWERSTATE_CHANGE_REQ,
	};
	uint8_t req_buf[NOOF_EC_POWERSTATE_CHANGE_REQ];

	enum {
		EC_POWERSTATE_CHANGE_RESULT = 0,
		NOOF_EC_POWERSTATE_CHANGE_RES,
	};
	uint8_t res_buf[NOOF_EC_POWERSTATE_CHANGE_RES];
#define EC_POWERSTATE_CHANGE_RESULT_OK 0
#define EC_POWERSTATE_CHANGE_RESULT_NG 1

	req_buf[EC_POWERSTATE_CHANGE_NEXT] = next_state;

	if(async == 0) {
		ret = ec_ipc_send_request(EC_IPC_PID_POWERSTATE, EC_IPC_CID_POWERSTATE_CHANGE,
					req_buf, sizeof(req_buf),
					res_buf, sizeof(res_buf));
		if(ret < (int)sizeof(res_buf)) {
			pr_err("ec_suspend: ec_ipc_send_request(POWERSTATE_CHANGE) failed. %d\n", ret);
			return -EIO;
		}

		if(res_buf[EC_POWERSTATE_CHANGE_RESULT] != EC_POWERSTATE_CHANGE_RESULT_OK) {
			pr_err("ec_suspend: ec_suspend_send_powerstate_change(%d) failed. %d\n", next_state, ret);
			return -EIO;
		}
	}
	else {
		ec_ipc_send_request_async(EC_IPC_PID_POWERSTATE, EC_IPC_CID_POWERSTATE_CHANGE,
					req_buf, sizeof(req_buf),
					&ec_suspend_send_powerstate_change_response, NULL);
		ret = 0;
	}

	return ret;
}

static int ec_suspend_send_reserve_reboot(int enable)
{
	int ret;

#define EC_IPC_CID_RESERVE_REBOOT 0x93

	enum {
		RESERVE_REBOOT_ENABLE = 0,
		NOOF_RESERVE_REBOOT_REQ,
	};
	uint8_t req_buf[NOOF_RESERVE_REBOOT_REQ];

	enum {
		RESERVE_REBOOT_RESULT = 0,
		NOOF_RESERVE_REBOOT_RES,
	};
	uint8_t res_buf[NOOF_RESERVE_REBOOT_RES];
#define RESERVE_REBOOT_RESULT_OK 0
#define RESERVE_REBOOT_RESULT_NG 1

	req_buf[RESERVE_REBOOT_ENABLE] = (enable == 0) ? 0 : 1;

	ret = ec_ipc_send_request(EC_IPC_PID_POWERSTATE, EC_IPC_CID_RESERVE_REBOOT,
					req_buf, sizeof(req_buf),
					res_buf, sizeof(res_buf));

	if(ret < (int)sizeof(res_buf)) {
		pr_err("ec_suspend: ec_ipc_send_request(RESERVE_REBOOT) failed. %d\n", ret);
		return -EIO;
	}

	if(res_buf[RESERVE_REBOOT_RESULT] != RESERVE_REBOOT_RESULT_OK) {
		pr_err("ec_suspend: ec_suspend_send_reserve_reboot(%d) failed. %d\n", enable, ret);
		return -EIO;
	}

	return ret;
}

static atomic_t ec_suspend_earlysuspending;

#ifdef CONFIG_HAS_EARLYSUSPEND
static void ec_suspend_pre_earlysuspend(struct early_suspend *h)
{
	atomic_set(&ec_suspend_earlysuspending, 1);
	ec_suspend_send_powerstate_change(EC_POWERSTATE_ES, 0);
}
static void ec_suspend_post_earlysuspend(struct early_suspend *h)
{
	ec_suspend_send_powerstate_change(EC_POWERSTATE_S0, 0);
	atomic_set(&ec_suspend_earlysuspending, 0);
}
static struct early_suspend ec_suspend_earlysuspend =
{
	.suspend = ec_suspend_pre_earlysuspend,
	.resume = ec_suspend_post_earlysuspend,
	.level = EARLY_SUSPEND_LEVEL_DISABLE_FB + 1,
};
#endif

int nbx_ec_suspend_suspend(void)
{
	ec_suspend_send_powerstate_change(EC_POWERSTATE_S3, 0);
	return 0;
}
EXPORT_SYMBOL(nbx_ec_suspend_suspend);

int nbx_ec_suspend_resume(void)
{
	/* async */
	ec_suspend_send_powerstate_change((atomic_read(&ec_suspend_earlysuspending) != 0) ? EC_POWERSTATE_ES : EC_POWERSTATE_S0, 1);
	return 0;
}
EXPORT_SYMBOL(nbx_ec_suspend_resume);

static int ec_suspend_reboot_event(struct notifier_block* this, unsigned long event, void* ptr)
{
	int ret = NOTIFY_DONE;

	switch (event) {
	case SYS_RESTART:

		ec_suspend_send_reserve_reboot(1);

		ret = NOTIFY_OK;
		break;
	case SYS_HALT:
	case SYS_POWER_OFF:

		ec_suspend_send_powerstate_change(EC_POWERSTATE_S5, 0);

		ret = NOTIFY_OK;
		break;
	default:
		break;
	}

	return ret;
}

static struct notifier_block ec_suspend_reboot_notifier = {
	.notifier_call = ec_suspend_reboot_event,
	.priority = 0,
};

static int __init ec_suspend_probe(struct nbx_ec_ipc_device* edev)
{
	atomic_set(&ec_suspend_earlysuspending, 0);

#ifdef CONFIG_HAS_EARLYSUSPEND
	register_early_suspend(&ec_suspend_earlysuspend);
#endif

	register_reboot_notifier(&ec_suspend_reboot_notifier);

	return 0;
}
static int ec_suspend_remove(struct nbx_ec_ipc_device* edev)
{
	unregister_reboot_notifier(&ec_suspend_reboot_notifier);

#ifdef CONFIG_HAS_EARLYSUSPEND
	unregister_early_suspend(&ec_suspend_earlysuspend);
#endif

	return 0;
}

static struct nbx_ec_ipc_driver ec_ipc_suspend_driver = {
	.probe   = ec_suspend_probe,
	.remove  = ec_suspend_remove,
	.drv     = {
		.name = "ec_ipc_suspend",
	},
};

static int __init ec_suspend_init(void)
{
	int ret;

	ret = nbx_ec_ipc_driver_register(&ec_ipc_suspend_driver);
	if (ret < 0) {
		pr_err("%s:nbx_ec_ipc_driver_register() failed, %d\n", __func__, ret);
		return ret;
	}

	return 0;
}
static void __exit ec_suspend_exit(void)
{
	nbx_ec_ipc_driver_unregister(&ec_ipc_suspend_driver);
}

module_init(ec_suspend_init);
module_exit(ec_suspend_exit);

MODULE_LICENSE("GPL");
