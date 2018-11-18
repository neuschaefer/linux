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
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/list.h>
#include <linux/err.h>
#include <linux/semaphore.h>
#include <linux/suspend.h>
#include <linux/wakelock.h>

#include <linux/nbx_ec_ipc.h>
#include <linux/nbx_ec_ipc_acplug.h>

#define LOCKED_STATEMENT(mutex, statement)	\
	down(mutex);				\
	do {					\
		statement;			\
	} while(0);				\
	up(mutex);

static LIST_HEAD(callback_list);
static DEFINE_SEMAPHORE(callback_list_mutex);

struct callback_t {
	struct list_head list;
	void (*func)(void);
};

static int ec_ipc_acplug_last = -1; /* unknown */

#define EC_IPC_CID_AC_INFO_REQUEST 0x20
#define EC_IPC_CID_AC_INFO_EVENT 0x21

static struct workqueue_struct* ec_ipc_acplug_workqueue;
static struct work_struct ec_ipc_acplug_work;

static struct wake_lock ec_ipc_acplug_wake_lock;

static void ec_ipc_acplug_worker(struct work_struct* work)
{
	ssize_t ret;
	int acplug_state;

	enum {
		AC_INFO_RES_STATE = 0,
		NOOF_AC_INFO_RES,
	};
	uint8_t res_buf[NOOF_AC_INFO_RES];

#define AC_INFO_RES_STATE_UNPLUGGED 0
#define AC_INFO_RES_STATE_PLUGGED 1

	ret = ec_ipc_send_request(EC_IPC_PID_BATTERY, EC_IPC_CID_AC_INFO_REQUEST,
				NULL, 0,
				res_buf, sizeof(res_buf) );
	if(ret < (int)sizeof(res_buf)){
		pr_err("ec_ipc_acplug:ec_ipc_send_request failed. %d\n", ret);
		goto err_exit;
	}

	acplug_state = (res_buf[AC_INFO_RES_STATE] == AC_INFO_RES_STATE_UNPLUGGED)? 0: 1;

	if(ec_ipc_acplug_last != acplug_state) {
		struct callback_t* callback;

		ec_ipc_acplug_last = acplug_state;

		LOCKED_STATEMENT(&callback_list_mutex,
				list_for_each_entry(callback, &callback_list, list) {
					(callback->func)();
				}
			);
	}

err_exit:
	wake_unlock(&ec_ipc_acplug_wake_lock);
}

static void ec_ipc_acplug_event(const uint8_t* buf, int size)
{
	if(buf == NULL) return;
	if(ec_ipc_acplug_workqueue == NULL) return;

	queue_work(ec_ipc_acplug_workqueue, &ec_ipc_acplug_work);
}

int nbx_ec_ipc_acplug_get_state(void)
{
	return ec_ipc_acplug_last;
}

void nbx_ec_ipc_acplug_request_update(void)
{
	if(ec_ipc_acplug_workqueue == NULL) return;

	queue_work(ec_ipc_acplug_workqueue, &ec_ipc_acplug_work);
}

int nbx_ec_ipc_acplug_register_callback( void (*func)(void) )
{
	struct callback_t* callback;

	callback = kzalloc(sizeof(struct callback_t), GFP_KERNEL);
	if(callback == NULL) {
		return -ENOMEM;
	}

	callback->func = func;

	LOCKED_STATEMENT(&callback_list_mutex,
			INIT_LIST_HEAD(&(callback->list));
			list_add_tail(&(callback->list), &callback_list);
		);

	return 0;
}

void nbx_ec_ipc_acplug_unregister_callback( void (*func)(void) )
{
	struct callback_t* del_callback;
	struct callback_t* n_callback;

	LOCKED_STATEMENT(&callback_list_mutex,
			list_for_each_entry_safe(del_callback, n_callback, &callback_list, list) {
				if(del_callback->func == func) {
					list_del(&(del_callback->list));
					kfree(del_callback);
				}
			}
		);
}

#ifdef CONFIG_SUSPEND

static int ec_ipc_acplug_suspend(struct nbx_ec_ipc_device* edev)
{
	cancel_work_sync(&ec_ipc_acplug_work);

	return 0;
}
static int ec_ipc_acplug_resume(struct nbx_ec_ipc_device* edev)
{
	if(ec_ipc_acplug_workqueue != NULL) {
		wake_lock_timeout(&ec_ipc_acplug_wake_lock, 10 * HZ);
		queue_work(ec_ipc_acplug_workqueue, &ec_ipc_acplug_work);
	}

	return 0;
}

#else /* !CONFIG_SUSPEND */
#define ec_ipc_acplug_suspend NULL
#define ec_ipc_acplug_resume NULL
#endif /* CONFIG_SUSPEND */

static int ec_ipc_acplug_probe(struct nbx_ec_ipc_device* edev)
{
	int ret = 0;

	ec_ipc_acplug_workqueue = create_singlethread_workqueue("ec_ipc_acplug_workqueue");
	if(ec_ipc_acplug_workqueue == NULL) {
		pr_err("ec_ipc_acplug:create_singlethread_workqueue() failed.\n");
		ret = -ENOMEM;
		goto error_exit;
	}

	INIT_WORK(&ec_ipc_acplug_work, ec_ipc_acplug_worker);

	wake_lock_init(&ec_ipc_acplug_wake_lock, WAKE_LOCK_SUSPEND, "ec_ipc_acplug");

	ec_ipc_acplug_last = -1; /* unknown */
	ec_ipc_register_recv_event(EC_IPC_CID_AC_INFO_EVENT, ec_ipc_acplug_event);
	queue_work(ec_ipc_acplug_workqueue, &ec_ipc_acplug_work); /* 1st check */

	return 0;

error_exit:
	ec_ipc_unregister_recv_event(EC_IPC_CID_AC_INFO_EVENT);
	if(ec_ipc_acplug_workqueue != NULL) {
		destroy_workqueue(ec_ipc_acplug_workqueue);
	}

	ec_ipc_acplug_workqueue = NULL;

	return ret;
}

static int ec_ipc_acplug_remove(struct nbx_ec_ipc_device* edev)
{
	struct callback_t* del_callback;
	struct callback_t* n_callback;

	ec_ipc_unregister_recv_event(EC_IPC_CID_AC_INFO_EVENT);

	LOCKED_STATEMENT(&callback_list_mutex,
			list_for_each_entry_safe(del_callback, n_callback, &callback_list, list) {
				list_del(&(del_callback->list));
				kfree(del_callback);
			}
		);

	if(ec_ipc_acplug_workqueue != NULL) {
		cancel_work_sync(&ec_ipc_acplug_work);
		flush_workqueue(ec_ipc_acplug_workqueue);
		destroy_workqueue(ec_ipc_acplug_workqueue);
	}

	ec_ipc_acplug_workqueue = NULL;

	wake_lock_destroy(&ec_ipc_acplug_wake_lock);

	return 0;
}

static struct nbx_ec_ipc_driver ec_ipc_acplug_driver = {
	.probe   = ec_ipc_acplug_probe,
	.remove  = ec_ipc_acplug_remove,
	.suspend = ec_ipc_acplug_suspend,
	.resume  = ec_ipc_acplug_resume,
	.drv  = {
		.name = "ec_ipc_acplug",
	},
};

static int __init ec_ipc_acplug_init(void)
{
	int ret;

	ec_ipc_acplug_workqueue = NULL;

	ret = nbx_ec_ipc_driver_register(&ec_ipc_acplug_driver);
	if (ret < 0) {
		pr_err("%s:nbx_ec_ipc_driver_register() failed, %d\n", __func__, ret);
		return ret;
	}

	return 0;
}
static void  __exit ec_ipc_acplug_exit(void)
{
	nbx_ec_ipc_driver_unregister(&ec_ipc_acplug_driver);
}

module_init(ec_ipc_acplug_init);
module_exit(ec_ipc_acplug_exit);

MODULE_LICENSE("GPL");
