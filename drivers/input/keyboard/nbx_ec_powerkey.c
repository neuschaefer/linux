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
#include <linux/init.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/suspend.h>
#include <linux/wakelock.h>

#include <linux/nbx_ec_ipc.h>

static int nbx_ec_powerkey_state;
static int nbx_ec_powerkey_state_reported;
static struct input_dev* nbx_ec_powerkey_dev;

static DEFINE_SEMAPHORE(powerkey_mutex);

#define LOCKED_STATEMENT(mutex, statement)	\
	down(mutex);				\
	do {					\
		statement;			\
	} while(0);				\
	up(mutex);

#define EC_IPC_CID_POWERKEY_REQUEST 0x34
#define EC_IPC_CID_POWERKEY_EVENT 0x35

static struct workqueue_struct* nbx_ec_powerkey_workqueue;
static struct work_struct nbx_ec_powerkey_work;

static struct wake_lock nbx_ec_powerkey_wake_lock;

static void nbx_ec_powerkey_worker(struct work_struct* work)
{
	ssize_t ret;
	int powerkey_state = -1;

	enum {
		POWERKEY_RES_RESULT = 0,
		NOOF_POWERKEY_RES,
	};

#define POWERKEY_RES_RESULT_UP 0
#define POWERKEY_RES_RESULT_DOWN 1

	uint8_t res_buf[NOOF_POWERKEY_RES];

	if(nbx_ec_powerkey_dev == NULL) return;

	LOCKED_STATEMENT(&powerkey_mutex,
			powerkey_state = nbx_ec_powerkey_state;
		);

	if(powerkey_state < 0) {
		ret = ec_ipc_send_request(EC_IPC_PID_POWERKEY, EC_IPC_CID_POWERKEY_REQUEST,
					NULL, 0,
					res_buf, sizeof(res_buf) );
		if(ret < NOOF_POWERKEY_RES){
			pr_err("nbx_ec_powerkey:ec_ipc_send_request failed. %d\n", ret);
			goto err_exit;
		}

		powerkey_state = (res_buf[POWERKEY_RES_RESULT] == POWERKEY_RES_RESULT_UP)? 0: 1;

		if(powerkey_state == 0 ) {
			if( ec_ipc_get_ec_int_factor(EC_INT_POWERKEY) ){
				/* if powerkey off & ecint powerkey then on -> off */
				input_report_key(nbx_ec_powerkey_dev, KEY_POWER, 1);
				input_sync(nbx_ec_powerkey_dev);
				nbx_ec_powerkey_state_reported = 1;
			}

		}
	}

	if(nbx_ec_powerkey_state_reported != powerkey_state) {
		input_report_key(nbx_ec_powerkey_dev, KEY_POWER, powerkey_state);
		input_sync(nbx_ec_powerkey_dev);
	}
	nbx_ec_powerkey_state_reported = powerkey_state;

err_exit:
	wake_unlock(&nbx_ec_powerkey_wake_lock);
}

static void nbx_ec_powerkey_event(const uint8_t* buf, int size)
{
	enum {
		POWERKEY_EVENT_STATE = 0,
		NOOF_POWERKEY_EVENT,
	};
#define POWERKEY_EVENT_STATE_UP 0
#define POWERKEY_EVENT_STATE_DOWN 1

	if(buf == NULL) return;
	if(nbx_ec_powerkey_workqueue == NULL) return;
	if(size < NOOF_POWERKEY_EVENT) return;

	LOCKED_STATEMENT(&powerkey_mutex,
			nbx_ec_powerkey_state = (buf[POWERKEY_EVENT_STATE] == POWERKEY_EVENT_STATE_UP) ? 0 : 1;
		);

	queue_work(nbx_ec_powerkey_workqueue, &nbx_ec_powerkey_work);
}

#ifdef CONFIG_SUSPEND

static int nbx_ec_powerkey_suspend(struct nbx_ec_ipc_device* edev)
{
	cancel_work_sync(&nbx_ec_powerkey_work);

	return 0;
}
static int nbx_ec_powerkey_resume(struct nbx_ec_ipc_device* edev)
{
	LOCKED_STATEMENT(&powerkey_mutex,
			nbx_ec_powerkey_state = -1;
		);

	if(nbx_ec_powerkey_workqueue != NULL) {
		wake_lock_timeout(&nbx_ec_powerkey_wake_lock, 10 * HZ);
		queue_work(nbx_ec_powerkey_workqueue, &nbx_ec_powerkey_work);
	}

	return 0;
}

#else /* !CONFIG_SUSPEND */
#define nbx_ec_powerkey_suspend NULL
#define nbx_ec_powerkey_resume NULL
#endif /* CONFIG_SUSPEND */

static int nbx_ec_powerkey_probe(struct nbx_ec_ipc_device* edev)
{
	int ret = 0;

	nbx_ec_powerkey_dev = input_allocate_device();
	if(nbx_ec_powerkey_dev == NULL) {
		pr_err("nbx_ec_powerkey:input_allocate_device() failed.\n");
		ret = -ENOMEM;
		goto error_exit;
	}

	nbx_ec_powerkey_dev->name = "nbx_powerkey";
	set_bit(EV_KEY, nbx_ec_powerkey_dev->evbit);
	set_bit(KEY_POWER, nbx_ec_powerkey_dev->keybit);

	ret = input_register_device(nbx_ec_powerkey_dev);
	if(ret != 0) {
		pr_err("nbx_ec_powerkey:input_register_device() failed. %d\n", ret);
		goto error_exit;
	}

	nbx_ec_powerkey_workqueue = create_singlethread_workqueue("nbx_ec_powerkey_workqueue");
	if(nbx_ec_powerkey_workqueue == NULL) {
		pr_err("nbx_ec_powerkey:create_singlethread_workqueue() failed.\n");
		ret = -ENOMEM;
		goto error_exit;
	}

	INIT_WORK(&nbx_ec_powerkey_work, nbx_ec_powerkey_worker);

	wake_lock_init(&nbx_ec_powerkey_wake_lock, WAKE_LOCK_SUSPEND, "nbx_ec_powerkey");

	LOCKED_STATEMENT(&powerkey_mutex,
			nbx_ec_powerkey_state = -1;
		);
	nbx_ec_powerkey_state_reported = -1;
	ec_ipc_register_recv_event(EC_IPC_CID_POWERKEY_EVENT, nbx_ec_powerkey_event);
	queue_work(nbx_ec_powerkey_workqueue, &nbx_ec_powerkey_work); /* 1st check */

	return 0;

error_exit:
	ec_ipc_unregister_recv_event(EC_IPC_CID_POWERKEY_EVENT);
	if(nbx_ec_powerkey_workqueue != NULL) {
		destroy_workqueue(nbx_ec_powerkey_workqueue);
	}
	input_unregister_device(nbx_ec_powerkey_dev);
	input_free_device(nbx_ec_powerkey_dev);
	nbx_ec_powerkey_dev = NULL;
	nbx_ec_powerkey_workqueue = NULL;

	return ret;
}

static int nbx_ec_powerkey_remove(struct nbx_ec_ipc_device* edev)
{
	ec_ipc_unregister_recv_event(EC_IPC_CID_POWERKEY_EVENT);

	if(nbx_ec_powerkey_workqueue != NULL) {
		cancel_work_sync(&nbx_ec_powerkey_work);
		flush_workqueue(nbx_ec_powerkey_workqueue);
		destroy_workqueue(nbx_ec_powerkey_workqueue);
	}

	input_unregister_device(nbx_ec_powerkey_dev);
	input_free_device(nbx_ec_powerkey_dev);
	nbx_ec_powerkey_dev = NULL;
	nbx_ec_powerkey_workqueue = NULL;

	wake_lock_destroy(&nbx_ec_powerkey_wake_lock);

	return 0;
}

static struct nbx_ec_ipc_driver nbx_ec_powerkey_driver = {
	.probe   = nbx_ec_powerkey_probe,
	.remove  = nbx_ec_powerkey_remove,
	.suspend = nbx_ec_powerkey_suspend,
	.resume  = nbx_ec_powerkey_resume,
	.drv = {
		.name = "nbx_powerkey",
	},
};

static int __init nbx_ec_powerkey_init(void)
{
	int ret;

	nbx_ec_powerkey_dev = NULL;
	nbx_ec_powerkey_workqueue = NULL;

	ret = nbx_ec_ipc_driver_register(&nbx_ec_powerkey_driver);
	if (ret < 0) {
		pr_err("%s:nbx_ec_ipc_driver_register() failed, %d\n", __func__, ret);
		return ret;
	}

	return 0;
}
static void  __exit nbx_ec_powerkey_exit(void)
{
	nbx_ec_ipc_driver_unregister(&nbx_ec_powerkey_driver);
}

module_init(nbx_ec_powerkey_init);
module_exit(nbx_ec_powerkey_exit);

MODULE_LICENSE("GPL");
