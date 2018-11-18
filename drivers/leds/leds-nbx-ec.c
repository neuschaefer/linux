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
 * NBX EC LED support
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/leds.h>
#include <linux/err.h>
#include <linux/workqueue.h>
#include <linux/spinlock.h>
#include <linux/suspend.h>
#include <linux/wakelock.h>

#include <linux/nbx_ec_ipc.h>

enum {
	NBX_EC_LED_ID_INFO = 0,
	NOOF_NBX_EC_LED_ID,
};
enum {
	NBX_EC_LED_COLOR_R,
	NBX_EC_LED_COLOR_G,
	NBX_EC_LED_COLOR_B,
};

static struct workqueue_struct* nbx_ec_led_workqueue;
static struct delayed_work nbx_ec_led_work;
static int nbx_ec_led_work_block;

struct nbx_ec_led_setting_t {
	spinlock_t lock;
	struct wake_lock wake_lock;
	char wake_lock_name[20];
	int update;
	int color_r;
	int color_g;
	int color_b;
	int on;
	int off;
};

static struct nbx_ec_led_setting_t nbx_ec_led_setting[NOOF_NBX_EC_LED_ID];

static void nbx_ec_led_brightness_set(int id, int color,
				enum led_brightness value);
static int nbx_ec_led_blink_set(int id,
				unsigned long* delay_on,
				unsigned long* delay_off);

static void nbx_ec_led_info_r(struct led_classdev *led_cdev,
			enum led_brightness value)
{
	nbx_ec_led_brightness_set(NBX_EC_LED_ID_INFO, NBX_EC_LED_COLOR_R, value);
}

static void nbx_ec_led_info_g(struct led_classdev *led_cdev,
			enum led_brightness value)
{
	nbx_ec_led_brightness_set(NBX_EC_LED_ID_INFO, NBX_EC_LED_COLOR_G, value);
}

static void nbx_ec_led_info_b(struct led_classdev *led_cdev,
			enum led_brightness value)
{
	nbx_ec_led_brightness_set(NBX_EC_LED_ID_INFO, NBX_EC_LED_COLOR_B, value);
}

static void nbx_ec_led_info_blink_brightness(struct led_classdev *led_cdev,
					enum led_brightness value)
{
	/* nothing to do. */
}
static int nbx_ec_led_info_blink(struct led_classdev *led_cdev,
				unsigned long *delay_on,
				unsigned long *delay_off)
{
	return nbx_ec_led_blink_set(NBX_EC_LED_ID_INFO, delay_on, delay_off);
}

static struct led_classdev nbx_ec_led_classdevs[] = {
	{
		.name           = "info:red",
		.brightness_set = nbx_ec_led_info_r,
		.blink_set      = nbx_ec_led_info_blink,
	},
	{
		.name           = "info:green",
		.brightness_set = nbx_ec_led_info_g,
		.blink_set      = nbx_ec_led_info_blink,
	},
	{
		.name           = "info:blue",
		.brightness_set = nbx_ec_led_info_b,
		.blink_set      = nbx_ec_led_info_blink,
	},
	{
		.name           = "info:blink",
		.brightness_set = nbx_ec_led_info_blink_brightness,
		.blink_set      = nbx_ec_led_info_blink,
		.default_trigger = "timer",
	},

};

static void nbx_ec_led_set(struct work_struct* work)
{
	/* send command to ec */
	int id;
	ssize_t ret;

#define EC_IPC_CID_INFO_LED 0x40
	unsigned char cid;

	unsigned long flags;

	enum {
		LED_COLOR_R = 0,
		LED_COLOR_G,
		LED_COLOR_B,
		LED_BLINK_ON_LOW,
		LED_BLINK_ON_HIGH,
		LED_BLINK_OFF_LOW,
		LED_BLINK_OFF_HIGH,
		NOOF_LED_REQ,
	};
	unsigned char req_buf[NOOF_LED_REQ];

	enum {
		LED_RES_RESULT = 0,
		NOOF_LED_RES,
	};
	unsigned char res_buf[NOOF_LED_RES];

#define LED_RES_RESULT_OK 0
#define LED_RES_RESULT_NG 1

	for(id = 0; id < NOOF_NBX_EC_LED_ID; id++) {
		spinlock_t* plock = &(nbx_ec_led_setting[id].lock);

		spin_lock_irqsave(plock, flags);
		do {
			if(nbx_ec_led_setting[id].update == 0) {
				wake_unlock(&(nbx_ec_led_setting[id].wake_lock));
				break;
			}
			nbx_ec_led_setting[id].update = 0;

			if(id == NBX_EC_LED_ID_INFO)
				cid = EC_IPC_CID_INFO_LED;
			else
				break;

			req_buf[LED_COLOR_R] = nbx_ec_led_setting[id].color_r;
			req_buf[LED_COLOR_G] = nbx_ec_led_setting[id].color_g;
			req_buf[LED_COLOR_B] = nbx_ec_led_setting[id].color_b;
			req_buf[LED_BLINK_ON_LOW] = nbx_ec_led_setting[id].on & 0xFF;
			req_buf[LED_BLINK_ON_HIGH] = (nbx_ec_led_setting[id].on >> 8) & 0xFF;
			req_buf[LED_BLINK_OFF_LOW] = nbx_ec_led_setting[id].off & 0xFF;
			req_buf[LED_BLINK_OFF_HIGH] = (nbx_ec_led_setting[id].off >> 8) & 0xFF;

			spin_unlock_irqrestore(plock, flags);
			{
				ret = ec_ipc_send_request(EC_IPC_PID_LED, cid,
							req_buf, sizeof(req_buf),
							res_buf, sizeof(res_buf));
			}
			spin_lock_irqsave(plock, flags);

			if((ret < (int)sizeof(res_buf)) || (res_buf[LED_RES_RESULT] != LED_RES_RESULT_OK) ){
				if(ret < (int)sizeof(res_buf))
					pr_err("leds-nbx-ec:ec_ipc_send_request failed %d\n", ret);
				else if(res_buf[LED_RES_RESULT] != LED_RES_RESULT_OK)
					pr_err("leds-nbx-ec:ec_ipc_send_request result NG\n");

				/* retry delayed 1sec */
				nbx_ec_led_setting[id].update = 1;
				if(nbx_ec_led_work_block == 0) {
					queue_delayed_work(nbx_ec_led_workqueue, &nbx_ec_led_work, msecs_to_jiffies(1000));
				}

				break;
			}

			if(nbx_ec_led_setting[id].update == 0) {
				wake_unlock(&(nbx_ec_led_setting[id].wake_lock));
			}
			else {
				/* update value in sending. send again. */
				id--;
			}

		} while(0);
		spin_unlock_irqrestore(plock, flags);
	}
}

void nbx_ec_led_brightness_set(int id, int color,
			enum led_brightness value)
{
	unsigned long flags;

	spin_lock_irqsave(&(nbx_ec_led_setting[id].lock), flags);
	{
		switch(color){
		case NBX_EC_LED_COLOR_R:
			nbx_ec_led_setting[id].color_r = value;
			break;
		case NBX_EC_LED_COLOR_G:
			nbx_ec_led_setting[id].color_g = value;
			break;
		case NBX_EC_LED_COLOR_B:
			nbx_ec_led_setting[id].color_b = value;
			break;
		default:
			break;
		}
		nbx_ec_led_setting[id].update = 1;

		wake_lock(&(nbx_ec_led_setting[id].wake_lock));
	}
	spin_unlock_irqrestore(&(nbx_ec_led_setting[id].lock), flags);

	if(nbx_ec_led_work_block == 0) {
		if(nbx_ec_led_workqueue != NULL) {
			cancel_delayed_work(&nbx_ec_led_work);
			queue_delayed_work(nbx_ec_led_workqueue, &nbx_ec_led_work, 0);
		}
	}
}

int nbx_ec_led_blink_set(int id,
			unsigned long* delay_on,
			unsigned long* delay_off)
{
	unsigned long flags;

	spin_lock_irqsave(&(nbx_ec_led_setting[id].lock), flags);
	{
		nbx_ec_led_setting[id].on = *delay_on;
		nbx_ec_led_setting[id].off = *delay_off;
		nbx_ec_led_setting[id].update = 1;

		wake_lock(&(nbx_ec_led_setting[id].wake_lock));
	}
	spin_unlock_irqrestore(&(nbx_ec_led_setting[id].lock), flags);

	if(nbx_ec_led_work_block == 0) {
		if(nbx_ec_led_workqueue != NULL) {
			cancel_delayed_work(&nbx_ec_led_work);
			queue_delayed_work(nbx_ec_led_workqueue, &nbx_ec_led_work, 0);
		}
	}

	return 0;
}

#ifdef CONFIG_SUSPEND

static int nbx_ec_led_suspend(struct nbx_ec_ipc_device *edev)
{
	nbx_ec_led_work_block = 1;
	flush_delayed_work(&nbx_ec_led_work);

	return 0;
}
static int nbx_ec_led_resume(struct nbx_ec_ipc_device *edev)
{
	/* restart when send not completed. */
	nbx_ec_led_work_block = 0;
	if(nbx_ec_led_workqueue != NULL) {
		queue_delayed_work(nbx_ec_led_workqueue, &nbx_ec_led_work, 0);
	}

	return 0;
}

#else /* !CONFIG_SUSPEND */
#define nbx_ec_led_suspend NULL
#define nbx_ec_led_resume NULL
#endif /* CONFIG_SUSPEND */

static int nbx_ec_led_remove(struct nbx_ec_ipc_device *edev);

static int nbx_ec_led_probe(struct nbx_ec_ipc_device *edev)
{
	int i;
	int ret = 0;
	unsigned long flags;

	for(i = 0; i < ARRAY_SIZE(nbx_ec_led_setting); i++) {
		spin_lock_init(&(nbx_ec_led_setting[i].lock));
		spin_lock_irqsave(&(nbx_ec_led_setting[i].lock), flags);
		{
			snprintf(nbx_ec_led_setting[i].wake_lock_name,
				sizeof(nbx_ec_led_setting[i].wake_lock_name),
				"nbx-ec-led-%d", i);
			wake_lock_init(&(nbx_ec_led_setting[i].wake_lock), WAKE_LOCK_SUSPEND,
				nbx_ec_led_setting[i].wake_lock_name);
			nbx_ec_led_setting[i].update = 0;
			nbx_ec_led_setting[i].color_r = 0;
			nbx_ec_led_setting[i].color_g = 0;
			nbx_ec_led_setting[i].color_b = 0;
			nbx_ec_led_setting[i].on = 0;
			nbx_ec_led_setting[i].off = 0;
		}
		spin_unlock_irqrestore(&(nbx_ec_led_setting[i].lock), flags);
	}

	nbx_ec_led_workqueue = create_singlethread_workqueue("nbx_ec_led_workqueue");
	if(nbx_ec_led_workqueue == NULL) {
		pr_err("nbx_ec_led:create_singlethread_workqueue() failed.\n");
		ret = -ENOMEM;
		goto error_exit;
	}
	INIT_DELAYED_WORK(&nbx_ec_led_work, nbx_ec_led_set);
	nbx_ec_led_work_block = 0;

	for (i = 0; i < ARRAY_SIZE(nbx_ec_led_classdevs); i++) {
		ret = led_classdev_register(&edev->dev, &(nbx_ec_led_classdevs[i]));
		if (ret < 0) {
			goto error_exit;
		}
	}

	return 0;

error_exit:
	nbx_ec_led_remove(edev);

	return ret;
}

int nbx_ec_led_remove(struct nbx_ec_ipc_device *edev)
{
	int i;

	for(i = 0; i < ARRAY_SIZE(nbx_ec_led_classdevs); i++) {
		led_classdev_unregister(&(nbx_ec_led_classdevs[i]));
	}

	for(i = 0; i < ARRAY_SIZE(nbx_ec_led_setting); i++) {
		wake_lock_destroy(&(nbx_ec_led_setting[i].wake_lock));
	}

	if(nbx_ec_led_workqueue != NULL) {
		cancel_delayed_work_sync(&nbx_ec_led_work);
		flush_workqueue(nbx_ec_led_workqueue);
		destroy_workqueue(nbx_ec_led_workqueue);
	}
	nbx_ec_led_workqueue = NULL;

	return 0;
}

static struct nbx_ec_ipc_driver nbx_ec_led_driver = {
	.probe   = nbx_ec_led_probe,
	.remove  = nbx_ec_led_remove,
	.suspend = nbx_ec_led_suspend,
	.resume  = nbx_ec_led_resume,
	.drv = {
		.name    = "nbx_led",
	},
};

static int __init nbx_ec_led_init(void)
{
	int ret;

	ret = nbx_ec_ipc_driver_register(&nbx_ec_led_driver);
	if (ret < 0) {
		pr_err("%s:nbx_ec_ipc_driver_register() failed, %d\n", __func__, ret);
		return ret;
	}

	return 0;
}

static void __exit nbx_ec_led_exit(void)
{
	nbx_ec_ipc_driver_unregister(&nbx_ec_led_driver);
}

module_init(nbx_ec_led_init);
module_exit(nbx_ec_led_exit);

MODULE_LICENSE("GPL");
