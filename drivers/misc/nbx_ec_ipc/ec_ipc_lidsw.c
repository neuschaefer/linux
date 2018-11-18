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
#include <linux/nbx_ec_ipc_lidsw.h>

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

static int ec_ipc_lidsw_last = -1; /* unknown */

#define EC_IPC_CID_LID_REQUEST 0x30
#define EC_IPC_CID_LID_EVENT 0x31

static struct workqueue_struct* ec_ipc_lidsw_workqueue;
static struct work_struct ec_ipc_lidsw_work;

static struct wake_lock ec_ipc_lidsw_wake_lock;

static void ec_ipc_lidsw_worker(struct work_struct* work)
{
	ssize_t ret;
	int lidsw_state;

	enum {
		LID_RES_RESULT = 0,
		NOOF_LID_RES,
	};
	uint8_t res_buf[NOOF_LID_RES];

#define LID_RES_RESULT_OPEN 0
#define LID_RES_RESULT_CLOSE 1

	ret = ec_ipc_send_request(EC_IPC_PID_LID, EC_IPC_CID_LID_REQUEST,
				NULL, 0,
				res_buf, sizeof(res_buf) );
	if(ret < (int)sizeof(res_buf)){
		pr_err("ec_ipc_lidsw:ec_ipc_send_request failed. %d\n", ret);
		goto err_exit;
	}

	lidsw_state = (res_buf[LID_RES_RESULT] == LID_RES_RESULT_OPEN)? 0: 1;

	if(ec_ipc_lidsw_last != lidsw_state) {
		struct callback_t* callback;

		ec_ipc_lidsw_last = lidsw_state;

		LOCKED_STATEMENT(&callback_list_mutex,
				list_for_each_entry(callback, &callback_list, list) {
					(callback->func)();
				}
			);
	}

err_exit:
	wake_unlock(&ec_ipc_lidsw_wake_lock);
}

static void ec_ipc_lidsw_event(const uint8_t* buf, int size)
{
	if(buf == NULL) return;
	if(ec_ipc_lidsw_workqueue == NULL) return;

	queue_work(ec_ipc_lidsw_workqueue, &ec_ipc_lidsw_work);
}

int nbx_ec_ipc_lidsw_get_state(void)
{
	return ec_ipc_lidsw_last;
}

int nbx_ec_ipc_lidsw_register_callback( void (*func)(void) )
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

void nbx_ec_ipc_lidsw_unregister_callback( void (*func)(void) )
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

static int ec_ipc_lidsw_suspend(struct nbx_ec_ipc_device* edev)
{
	cancel_work_sync(&ec_ipc_lidsw_work);
	ec_ipc_lidsw_last = -1; /* unknown */

	return 0;
}
static int ec_ipc_lidsw_resume(struct nbx_ec_ipc_device* edev)
{
	if(ec_ipc_lidsw_workqueue != NULL) {
		wake_lock_timeout(&ec_ipc_lidsw_wake_lock, 10 * HZ);
		queue_work(ec_ipc_lidsw_workqueue, &ec_ipc_lidsw_work);
	}

	return 0;
}

#else /* !CONFIG_SUSPEND */
#define ec_ipc_lidsw_suspend NULL
#define ec_ipc_lidsw_resume NULL
#endif /* CONFIG_SUSPEND */

static int ec_ipc_lidsw_probe(struct nbx_ec_ipc_device* edev)
{
	int ret = 0;

	ec_ipc_lidsw_workqueue = create_singlethread_workqueue("ec_ipc_lidsw_workqueue");
	if(ec_ipc_lidsw_workqueue == NULL) {
		pr_err("ec_ipc_lidsw:create_singlethread_workqueue() failed.\n");
		ret = -ENOMEM;
		goto error_exit;
	}

	INIT_WORK(&ec_ipc_lidsw_work, ec_ipc_lidsw_worker);

	wake_lock_init(&ec_ipc_lidsw_wake_lock, WAKE_LOCK_SUSPEND, "ec_ipc_lidsw");

	ec_ipc_lidsw_last = -1; /* unknown */
	ec_ipc_register_recv_event(EC_IPC_CID_LID_EVENT, ec_ipc_lidsw_event);
	queue_work(ec_ipc_lidsw_workqueue, &ec_ipc_lidsw_work); /* 1st check */

	return 0;

error_exit:
	ec_ipc_unregister_recv_event(EC_IPC_CID_LID_EVENT);
	if(ec_ipc_lidsw_workqueue != NULL) {
		destroy_workqueue(ec_ipc_lidsw_workqueue);
	}

	ec_ipc_lidsw_workqueue = NULL;

	return ret;
}

static int ec_ipc_lidsw_remove(struct nbx_ec_ipc_device* edev)
{
	struct callback_t* del_callback;
	struct callback_t* n_callback;

	ec_ipc_unregister_recv_event(EC_IPC_CID_LID_EVENT);

	LOCKED_STATEMENT(&callback_list_mutex,
			list_for_each_entry_safe(del_callback, n_callback, &callback_list, list) {
				list_del(&(del_callback->list));
				kfree(del_callback);
			}
		);

	if(ec_ipc_lidsw_workqueue != NULL) {
		cancel_work_sync(&ec_ipc_lidsw_work);
		flush_workqueue(ec_ipc_lidsw_workqueue);
		destroy_workqueue(ec_ipc_lidsw_workqueue);
	}

	ec_ipc_lidsw_workqueue = NULL;

	wake_lock_destroy(&ec_ipc_lidsw_wake_lock);

	return 0;
}

static struct nbx_ec_ipc_driver ec_ipc_lidsw_driver = {
	.probe   = ec_ipc_lidsw_probe,
	.remove  = ec_ipc_lidsw_remove,
	.suspend = ec_ipc_lidsw_suspend,
	.resume  = ec_ipc_lidsw_resume,
	.drv = {
		.name = "ec_ipc_lidsw",
	},
};

static int __init ec_ipc_lidsw_init(void)
{
	int ret;

	ec_ipc_lidsw_workqueue = NULL;

	ret = nbx_ec_ipc_driver_register(&ec_ipc_lidsw_driver);
	if (ret < 0) {
		pr_err("%s:nbx_ec_ipc_driver_register() failed, %d\n", __func__, ret);
		return ret;
	}

	return 0;
}
static void  __exit ec_ipc_lidsw_exit(void)
{
	nbx_ec_ipc_driver_unregister(&ec_ipc_lidsw_driver);
}

module_init(ec_ipc_lidsw_init);
module_exit(ec_ipc_lidsw_exit);

MODULE_LICENSE("GPL");
