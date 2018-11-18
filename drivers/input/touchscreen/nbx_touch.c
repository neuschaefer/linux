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
#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

#include <linux/delay.h>
#include <linux/earlysuspend.h>

#include <linux/slab.h>

#include <mach/pinmux.h>
#include <../gpio-names.h>
#include <linux/input/nbx_touch.h>
#include <linux/nbx_ec_ipc_lidsw.h>
#include <linux/nbx_ec_ipc_acplug.h>

#ifdef DEBUG_INFO_TP
#include <linux/io.h>

#define TP_FLAG_CLEAR     0
#define TP_FLAG_CHANGE    1
static int tp_flag_upper;
static int tp_flag_lower;
static int debug_use = 0;

struct s_debug_func{

	int (*debug_001)(u16 *x, u16 *y, int i);
	void (*debug_002)(void);
	void (*debug_003)(int j);
        void (*debug_021)(int tp_num, u8* finger_id, u16* x, u16* y, u8* z, u8* finger_num);
	void (*debug_022)(int tp_num);
	int (*debug_023)(void);
};

static struct s_debug_func* p_func_set = NULL;
static struct mutex* p_mutex_nbx02 = NULL;

#define TP_01 (1)
#define TP_02 (2)

#endif /* DEBUG_INFO_TP */

#define I2C_DEV_ADDER_UPPER  (0x1B)
#define I2C_DEV_ADDER_LOWER  (0x3B)

#define GPIO_UPPER_TP_IRQ TEGRA_GPIO_PG0
#define GPIO_LOWER_TP_IRQ TEGRA_GPIO_PG1

#define X_MIN                (0)
#define X_MAX                (1023)
#define Y_MIN                (0)
#define Y_HALF               (479)
#define Y_MAX                (959)

#define Y_OFFSET             (480)

#define MAX_FINGER_NUM       (5)
#define MAX_PRESSURE         (255)
#define MAX_WIDTH            (255)

#define READ_BUFF_LEN_TMA300 (31)

#define ADDR_VBS             (0x7)
#define ADDR_MOVE_THRESH     (0xA)
#define ADDR_MOVE_STEP       (0xB)
#define ADDR_FW_VER          (0x14)
#define ADDR_FW_DATA         (0x20)

#define VBS_UPPER_VAL_1      (0xF)
#define VBS_UPPER_VAL_2      (0xB)
#define VBS_LOWER_VAL_1      (0x5)
#define VBS_LOWER_VAL_2      (0x9)

#define EARLY_SUSPEND        (0x1)
#define UPPER_SUSPEND        (0x2)
#define LOWER_SUSPEND        (0x4)

#define EVENT                (1)
#define EVENT_DONE           (0)

#define ADDR_AC_NOISE        (0xF)
#define AC_NOISE_FILTER_OFF  (0x0)
#define AC_NOISE_FILTER_ON   (0x1)

#define TP_POWER_ON       1
#define TP_POWER_OFF      0
static int tp_power_status = 0;

#define AC_NOISE_FILTER_AC_CONFIRM  (0)
#define AC_NOISE_FILTER_DEFAULT_ON  (1)
#define AC_NOISE_FILTER_DEFAULT_OFF (2)
static int ac_filter_change = AC_NOISE_FILTER_AC_CONFIRM;

#define LID_OPEN             (0)
#define LID_CLOSE            (1)
static int lid_state_flag = LID_OPEN;

#ifdef CONFIG_HAS_EARLYSUSPEND
struct early_suspend early_suspend;
#endif /* CONFIG_HAS_EARLYSUSPEND */
static u8 early_suspend_done = 0;

static bool debug_print = 0;

static int upper_tp_version = 0;
static int lower_tp_version = 0;
static int event_stop = 0;
static int init_done = 0;

struct nbx_touch_data {
	struct input_dev* input_dev;
	struct i2c_client* client;
	struct workqueue_struct* wq;
	struct work_struct work;
	int finger_down_upper;
	int finger_down_lower;
};

static struct input_dev* s_input_dev;
static struct i2c_client* upper_client = NULL;
static struct i2c_client* lower_client = NULL;

static const unsigned short normal_i2c[] = {
	I2C_DEV_ADDER_UPPER,
	I2C_DEV_ADDER_LOWER,
	I2C_CLIENT_END
};

static void ac_filter_changed(void);

#ifdef CONFIG_HAS_EARLYSUSPEND
static void nbx_touch_early_suspend(struct early_suspend* h);
static void nbx_touch_late_resume(struct early_suspend* h);
#endif /* CONFIG_HAS_EARLYSUSPEND */

#ifdef CONFIG_PM
static int nbx_touch_suspend(struct i2c_client* client, pm_message_t mesg);
static int nbx_touch_resume(struct i2c_client* client);
#endif /* CONFIG_PM */

static ssize_t
lpe_wait_show(struct device* dev, struct device_attribute* attr, char* buf){
	int ret_val = 0;

	if(0 == memcmp(attr->attr.name, "debug_print", sizeof("debug_print"))){
		ret_val = snprintf(buf, 4, "%d\n", debug_print);
	}else if(0 == memcmp(attr->attr.name, "ac_filter_change", sizeof("ac_filter_change"))){
		ret_val = snprintf(buf, 4, "%d\n", ac_filter_change);
	}else{
		printk(KERN_ALERT "No such file\n");
	}

	return ret_val;
}

static ssize_t
lpe_wait_store(struct device* dev, struct device_attribute* attr,
	       const char* buf, size_t count){
	int input_val = 0;

	input_val = simple_strtol(buf, NULL, 10);

	if(input_val < 0){
		return 0;
	}

	if(0 == memcmp(attr->attr.name, "debug_print", sizeof("debug_print"))){
		debug_print = input_val;
	}else if(0 == memcmp(attr->attr.name, "ac_filter_change", sizeof("ac_filter_change"))){
		switch ( input_val ) {
		case AC_NOISE_FILTER_AC_CONFIRM:
		case AC_NOISE_FILTER_DEFAULT_ON:
		case AC_NOISE_FILTER_DEFAULT_OFF:
			ac_filter_change = input_val;
			ac_filter_changed();
			break;
		default:
			break;
		}
	}else{
		printk(KERN_ALERT "No such file\n");
		return 0;
	}

	return (count);
}

static DEVICE_ATTR(debug_print, 0644, lpe_wait_show, lpe_wait_store);
static DEVICE_ATTR(ac_filter_change, 0644, lpe_wait_show, lpe_wait_store);


static int nbx_device_file_cleate(struct i2c_client* client){

	int err = 0;

	err = device_create_file(&client->dev, &dev_attr_debug_print);
        if(err){
		return err;
        }

	err = device_create_file(&client->dev, &dev_attr_ac_filter_change);
        if(err){
		return err;
        }

	return 0;
}


static int nbx_touch_read(struct i2c_client* client, u8 size, u8* value){
	int err;
	struct i2c_msg msg[2];
	u8 data_buf[size];

	if (!client->adapter){
		return -ENODEV;
	}

	msg[0].addr = client->addr;
	msg[0].flags = 1;
	msg[0].len = size;
	msg[0].buf = data_buf;

	err = i2c_transfer(client->adapter, msg, 1);
	if (err < 0){
		return err;
	}

	memcpy(value, data_buf, size);

	return err;
}

static int nbx_touch_read_from_addr(struct i2c_client* client, u8 addr, u8 size, u8* value){
	int err;
	struct i2c_msg msg[2];

	if (!client->adapter){
		return -ENODEV;
	}

	msg[0].addr = client->addr;
	msg[0].flags = 0;
	msg[0].len = 1;
	msg[0].buf = &addr;
	msg[1].addr = client->addr;
	msg[1].flags = 1;
	msg[1].len = size;
	msg[1].buf = value;

	err = i2c_transfer(client->adapter, msg, 2);
	if (err < 0){
		return err;
	}

	return err;
}

static int nbx_touch_write(struct i2c_client* client, u8 size, u8* value){
	int err;
	struct i2c_msg msg[1];
	unsigned char data[2];

	if (size == 1){
		data[0] = (value[0] & 0xff);
	}else if (size == 2){
		data[0] = (value[0] & 0xff);
		data[1] = (value[1] & 0xff);
	}else{
		return -EINVAL;
	}

	if (!client->adapter){
		return -ENODEV;
	}

	msg->addr = client->addr;
	msg->flags = 0;
	msg->len = size;
	msg->buf = data;

	err = i2c_transfer(client->adapter, msg, 1);
	if (err > 0){
		return 0;
	}else if(err == 0){
		return -EIO;
	}

	return err;
}

static int v_sync_blank_scan(struct i2c_client* client){

	u8 value[2];
	int err = 0;
	value[0] = ADDR_VBS;

	if(client->addr == I2C_DEV_ADDER_UPPER){

		value[1] = VBS_UPPER_VAL_1;
		/* dummy write for wake up */
		err = nbx_touch_write(client, 1, &(value[0]));
		if(err < 0){
			return err;
		}
		msleep(1);
		err = nbx_touch_write(client, 2, value);
		if(err < 0){
			return err;
		}
		msleep(40);

		value[1] = VBS_UPPER_VAL_2;
		/* dummy write for wake up */
		err = nbx_touch_write(client, 1, &(value[0]));
		if(err < 0){
			return err;
		}
		msleep(1);
		err = nbx_touch_write(client, 2, value);
		if(err < 0){
			return err;
		}

	}else if(client->addr == I2C_DEV_ADDER_LOWER){

		value[1] = VBS_LOWER_VAL_1;
		/* dummy write for wake up */
		err = nbx_touch_write(client, 1, &(value[0]));
		if(err < 0){
			return err;
		}
		msleep(1);
		err = nbx_touch_write(client, 2, value);
		if(err < 0){
			return err;
		}
		msleep(40);

		value[1] = VBS_LOWER_VAL_2;
		/* dummy write for wake up */
		err = nbx_touch_write(client, 1, &(value[0]));
		if(err < 0){
			return err;
		}
		msleep(1);
		err = nbx_touch_write(client, 2, value);
		if(err < 0){
			return err;
		}
	}

	return 0;
}

int nbx_touch_v_sync_blank_scan(void){

	u8 value[2];
	int err = 0;
	value[0] = ADDR_VBS;

	if(upper_tp_version < 0x60){
		return 0;
	}

	value[1] = VBS_UPPER_VAL_1;
	/* dummy write for wake up */
	err = nbx_touch_write(upper_client, 1, &(value[0]));
	if(err < 0){
		return err;
	}
	msleep(1);
	err = nbx_touch_write(upper_client, 2, value);
	if(err < 0){
		return err;
	}
	msleep(40);

	value[1] = VBS_UPPER_VAL_2;
	/* dummy write for wake up */
	err = nbx_touch_write(upper_client, 1, &(value[0]));
	if(err < 0){
		return err;
	}
	msleep(1);
	err = nbx_touch_write(upper_client, 2, value);
	if(err < 0){
		return err;
	}


	value[1] = VBS_LOWER_VAL_1;
	/* dummy write for wake up */
	err = nbx_touch_write(lower_client, 1, &(value[0]));
	if(err < 0){
		return err;
	}
	msleep(1);
	err = nbx_touch_write(lower_client, 2, value);
	if(err < 0){
		return err;
	}
	msleep(40);

	value[1] = VBS_LOWER_VAL_2;
	/* dummy write for wake up */
	err = nbx_touch_write(lower_client, 1, &(value[0]));
	if(err < 0){
		return err;
	}
	msleep(1);
	err = nbx_touch_write(lower_client, 2, value);
	if(err < 0){
		return err;
	}

	return 0;
}

#ifdef DEBUG_INFO_TP

static int set_move_threshold(struct i2c_client* client, int thr1, int thr2){

	u8 move_thresh[2];
	int err = 0;
	int i;

	move_thresh[0] = ADDR_MOVE_THRESH;
	move_thresh[1] = thr1;
	for(i = 0; i < 5; i++){
		err = nbx_touch_write(client, 2, move_thresh);
		if(err == 0){
			break;
		}
	}
	if(err < 0){
		return err;
	}

	move_thresh[0] = ADDR_MOVE_STEP;
	move_thresh[1] = thr2;
	for(i = 0; i < 5; i++){
		err = nbx_touch_write(client, 2, move_thresh);
		if(err == 0){
			break;
		}
	}
	if(err < 0){
		return err;
	}

	return 0;
}
#endif /* DEBUG_INFO_TP */

static int touch_power_on(struct i2c_client* client){

	int ret_val = 0;
	struct nbx_touch_platform_data* platform_data;

	platform_data = client->dev.platform_data;

	platform_data->gpio_enable(TEGRA_GPIO_PH0);

	ret_val = gpio_request(TEGRA_GPIO_PH0, "NBX02 TP0_UPDATE");
	if(ret_val){
		return ret_val;
	}
	/* Pin setting high for preventing i2c leak current */
	ret_val = gpio_direction_output(TEGRA_GPIO_PH0, 1);
	if(ret_val){
		return ret_val;
	}

	ret_val = gpio_get_value(TEGRA_GPIO_PH0);
	if(ret_val == 0){
		return -ENODEV;
	}

	platform_data->gpio_enable(TEGRA_GPIO_PH1);

	ret_val = gpio_request(TEGRA_GPIO_PH1, "NBX02 TP1_UPDATE");
	if(ret_val){
		return ret_val;
	}
	/* Pin setting high for preventing i2c leak current */
	ret_val = gpio_direction_output(TEGRA_GPIO_PH1, 1);
	if(ret_val){
		return ret_val;
	}

	ret_val = gpio_get_value(TEGRA_GPIO_PH1);
	if(ret_val == 0){
		return -ENODEV;
	}

	platform_data->gpio_enable(TEGRA_GPIO_PA4);

	ret_val = gpio_request(TEGRA_GPIO_PA4, "NBX02 3.3V");
	if(ret_val){
		return ret_val;
	}
	ret_val = gpio_direction_output(TEGRA_GPIO_PA4, 1);
	if(ret_val){
		return ret_val;
	}


	ret_val = gpio_get_value(TEGRA_GPIO_PA4);
	if(ret_val == 0){
		return -ENODEV;
	}

	platform_data->gpio_enable(TEGRA_GPIO_PA2);

	ret_val = gpio_request(TEGRA_GPIO_PA2, "NBX02 1.8V UPPER");
	if(ret_val){
		return ret_val;
	}
	ret_val = gpio_direction_output(TEGRA_GPIO_PA2, 1);
	if(ret_val){
		return ret_val;
	}

	ret_val = gpio_get_value(TEGRA_GPIO_PA2);
	if(ret_val == 0){
		return -ENODEV;
	}

	/* Pin setting low for changing from update gpio to i2c */
	gpio_set_value(TEGRA_GPIO_PH0, 0);

	platform_data->gpio_enable(TEGRA_GPIO_PA5);

	ret_val = gpio_request(TEGRA_GPIO_PA5, "NBX02 1.8V LOWER");
	if(ret_val){
		return ret_val;
	}
	ret_val = gpio_direction_output(TEGRA_GPIO_PA5, 1);
	if(ret_val){
		return ret_val;
	}

	ret_val = gpio_get_value(TEGRA_GPIO_PA5);
	if(ret_val == 0){
		return -ENODEV;
	}

	/* Pin setting low for changing from update gpio to i2c */
	gpio_set_value(TEGRA_GPIO_PH1, 0);

	/* Disable internal pull-up for ATD pingroup */
	platform_data->pinmux_set_pullupdown(TEGRA_PINGROUP_ATD, TEGRA_PUPD_NORMAL);

	msleep(100);

	tp_power_status = TP_POWER_ON;

	return 0;
}


static int nbx_touch_wakeup(void){

	/* 3.3V */
	gpio_set_value(TEGRA_GPIO_PA4, 1);

	/* 1.8V upper */
	gpio_set_value(TEGRA_GPIO_PA2, 1);

	/* Pin setting low for changing from update gpio to i2c */
	gpio_set_value(TEGRA_GPIO_PH0, 0);

	/* 1.8V lower */
	gpio_set_value(TEGRA_GPIO_PA5, 1);

	/* Pin setting low for changing from update gpio to i2c */
	gpio_set_value(TEGRA_GPIO_PH1, 0);

	msleep(100);

	tp_power_status = TP_POWER_ON;

	return 0;
}

static int nbx_touch_sleep(void){

	/* Pin setting high for preventing i2c leak current */
	gpio_set_value(TEGRA_GPIO_PH0, 1);

	/* 1.8V upper */
	gpio_set_value(TEGRA_GPIO_PA2, 0);

	/* Pin setting high for preventing i2c leak current */
	gpio_set_value(TEGRA_GPIO_PH1, 1);

	/* 1.8V lower */
	gpio_set_value(TEGRA_GPIO_PA5, 0);

	/* 3.3V */
	gpio_set_value(TEGRA_GPIO_PA4, 0);

	tp_power_status = TP_POWER_OFF;

	return 0;
}


static int nbx_ac_plugged(void){

	u8 value[2];
	int err = 0;

	value[0] = ADDR_AC_NOISE;
	value[1] = AC_NOISE_FILTER_ON;

	/* dummy write for wake up */
	err = nbx_touch_write(upper_client, 1, &(value[0]));
	if(err < 0){
		return err;
	}
	msleep(1);
	err = nbx_touch_write(upper_client, 2, value);
	if(err < 0){
		return err;
	}

	/* dummy write for wake up */
	err = nbx_touch_write(lower_client, 1, &(value[0]));
	if(err < 0){
		return err;
	}
	msleep(1);
	err = nbx_touch_write(lower_client, 2, value);
	if(err < 0){
		return err;
	}

	return 0;
}


static int nbx_ac_unplugged(void){

	u8 value[2];
	int err = 0;

	value[0] = ADDR_AC_NOISE;
	value[1] = AC_NOISE_FILTER_OFF;

	/* dummy write for wake up */
	err = nbx_touch_write(upper_client, 1, &(value[0]));
	if(err < 0){
		return err;
	}
	msleep(1);
	err = nbx_touch_write(upper_client, 2, value);
	if(err < 0){
		return err;
	}

	/* dummy write for wake up */
	err = nbx_touch_write(lower_client, 1, &(value[0]));
	if(err < 0){
		return err;
	}
	msleep(1);
	err = nbx_touch_write(lower_client, 2, value);
	if(err < 0){
		return err;
	}

	return 0;
}


static void ac_filter_changed(void){

	int acplug_state = -1;

	if(tp_power_status == TP_POWER_OFF){
		return;
	}

	if(init_done == 0){
		return;
	}

	switch ( ac_filter_change ) {
	case AC_NOISE_FILTER_AC_CONFIRM:
		acplug_state = nbx_ec_ipc_acplug_get_state();
		break;
	case AC_NOISE_FILTER_DEFAULT_ON:
		acplug_state = 1;
		break;
	case AC_NOISE_FILTER_DEFAULT_OFF:
		acplug_state = 0;
		break;
	default:
		acplug_state = -1;
		break;
	}

	switch (acplug_state) {
	case 0:
		nbx_ac_unplugged();
		break;
	case 1:
		nbx_ac_plugged();
		break;
	default:
		break;
	}

	return;
}

static void nbx_ac_changed(void){

	ac_filter_changed();
	return;
}


static int nbx_lid_open(void){
	int ret_val = 0;
	if ( s_input_dev ) {
		ret_val = nbx_touch_wakeup();
		v_sync_blank_scan(upper_client);
		v_sync_blank_scan(lower_client);
#ifdef DEBUG_INFO_TP
		set_move_threshold(upper_client, 0, 0);
		set_move_threshold(lower_client, 0, 0);
#endif /* DEBUG_INFO_TP */

		ac_filter_changed();

	}
	return ret_val;
}


static int nbx_lid_close(void){
	int ret_val = 0;
	if ( s_input_dev ) {
		ret_val = nbx_touch_sleep();
	}
	return ret_val;
}


static void nbx_lid_changed(void){

	int lidsw_state;
	lidsw_state = nbx_ec_ipc_lidsw_get_state();

	if(lidsw_state == 0){
		lid_state_flag = LID_OPEN;
	}else if(lidsw_state == 1){
		lid_state_flag = LID_CLOSE;
	}
	return;
}


int nbx_touch_event_stop(int state){

	event_stop = state;

	return 0;
}


static void tma300_multi_touch_break(struct i2c_client* client){

	struct nbx_touch_data* drv_data = i2c_get_clientdata(client);

	if(lid_state_flag == LID_CLOSE){
		return;
	}

	if(event_stop == 1){
		return;
	}
#ifdef DEBUG_INFO_TP
	if(debug_use){
		mutex_lock(p_mutex_nbx02);

		if(p_func_set->debug_002 != NULL){
			p_func_set->debug_002();
		}
		mutex_unlock(p_mutex_nbx02);
	}
#endif /* DEBUG_INFO_TP */

	if(debug_print == 1){
		if(client->addr == I2C_DEV_ADDER_UPPER){
			printk(KERN_ALERT "upper off\n");
		}else if(client->addr == I2C_DEV_ADDER_LOWER){
			printk(KERN_ALERT "lower off\n");
		}
	}

	input_mt_sync(drv_data->input_dev);
	input_sync(drv_data->input_dev);
}

static irqreturn_t nbx_touch_irq_handler(int irq, void* data){

        struct nbx_touch_data* drv_data = (struct nbx_touch_data*)data;

	queue_work(drv_data->wq, &(drv_data->work));
	return (IRQ_HANDLED);
}

static void multi_touch_work_5point(struct work_struct* work){

#ifdef DEBUG_INFO_TP
	int ret[MAX_FINGER_NUM] = {1};
#endif /* DEBUG_INFO_TP */

	int i = 0;
	int err;
	int pressure = MAX_PRESSURE;

	u16 x[MAX_FINGER_NUM];
	u16 y[MAX_FINGER_NUM];
	u8 z[MAX_FINGER_NUM];
	u8 finger_id[MAX_FINGER_NUM];

	u8 setting_addr = 0;
	u8 value[READ_BUFF_LEN_TMA300];
	u8 finger_num = 0;
	int call_break = 0;
	int tp_num = 0;
	u8 all_finger_num = 0;

	const char* which_tp = "";

	struct nbx_touch_data* drv_data = 
		container_of(work, struct nbx_touch_data, work);
	struct input_dev* input = drv_data->input_dev;
	struct i2c_client* client = drv_data->client;

#ifdef DEBUG_INFO_TP

	if(debug_use){
		mutex_lock(p_mutex_nbx02);
		/* set distance */
		if(client->addr == I2C_DEV_ADDER_UPPER){

			tp_num = TP_01;

			if(p_func_set->debug_001 != NULL &&
			   tp_flag_upper == TP_FLAG_CLEAR){

				tp_flag_upper = TP_FLAG_CHANGE;
				set_move_threshold(client, 0, 0);

			}else if(p_func_set->debug_001 == NULL &&
				 tp_flag_upper == TP_FLAG_CHANGE){

				tp_flag_upper = TP_FLAG_CLEAR;
				set_move_threshold(client, 25, 15);
			}
		}else if(client->addr == I2C_DEV_ADDER_LOWER){

			tp_num = TP_02;

			if(p_func_set->debug_001 != NULL &&
			   tp_flag_lower == TP_FLAG_CLEAR){

				tp_flag_lower = TP_FLAG_CHANGE;
				set_move_threshold(client, 0, 0);
			}else if(p_func_set->debug_001 == NULL &&
				 tp_flag_lower == TP_FLAG_CHANGE){

				tp_flag_lower = TP_FLAG_CLEAR;
				set_move_threshold(client, 25, 15);
			}
		}
		mutex_unlock(p_mutex_nbx02);
	}
#endif /* DEBUG_INFO_TP */

	if(client->addr == I2C_DEV_ADDER_UPPER){
		which_tp = "upper";
	}else if(client->addr == I2C_DEV_ADDER_LOWER){
		which_tp = "lower";
	}

	memset(x, 0xffff, sizeof(u16)*MAX_FINGER_NUM);
	memset(y, 0xffff, sizeof(u16)*MAX_FINGER_NUM);
	memset(z, 0xffff, sizeof(u8)*MAX_FINGER_NUM);
	memset(finger_id, 0, sizeof(u8)*MAX_FINGER_NUM);

	err = nbx_touch_read_from_addr(client, ADDR_FW_DATA, READ_BUFF_LEN_TMA300, value);
	if ( err < 0 ) {
		/* ignore err */
		goto exit;
	}

	if(lid_state_flag == LID_CLOSE){
		goto exit;
	}

	if(event_stop == 1){
		goto exit;
	}

	finger_num = 0x0f & value[0];

	if(finger_num > MAX_FINGER_NUM){
		finger_num = MAX_FINGER_NUM;
	}
	all_finger_num = finger_num;


	for(i = 0; i < finger_num; i++){

		finger_id[i] = value[i*6+1];

		x[i] = (value[i*6+2] << 8) + value[i*6+3];
		y[i] = (value[i*6+4] << 8) + value[i*6+5];
		z[i] = value[i*6+6];
	}

	/* break judgement */
	if((x[0] == 0xffff) && (y[0] == 0xffff)){
	
		if(client->addr == I2C_DEV_ADDER_UPPER){
			/* double break event eliminate */
			if(drv_data->finger_down_upper == EVENT){
				drv_data->finger_down_upper = EVENT_DONE;
			}else{
				goto exit;
			}
		}else if(client->addr == I2C_DEV_ADDER_LOWER){
			/* double break event eliminate */
			if(drv_data->finger_down_lower == EVENT){
				drv_data->finger_down_lower = EVENT_DONE;
			}else{
				goto exit;
			}
		}
#ifdef DEBUG_INFO_TP
		if(debug_use){
			mutex_lock(p_mutex_nbx02);
			if(p_func_set->debug_022 != NULL){
				p_func_set->debug_022(tp_num);
			}

			if(p_func_set->debug_023 != NULL){
				all_finger_num = p_func_set->debug_023();
			}
			mutex_unlock(p_mutex_nbx02);
		}
#endif /* DEBUG_INFO_TP */
		if(all_finger_num == 0){
			call_break = 1;
		}else{
			goto exit;
		}

	}

	if(call_break == 1){

		tma300_multi_touch_break(client);

		goto exit;
	}

	for(i = 0; i < finger_num; i++){

		if(z[i] > MAX_WIDTH){
			z[i] = MAX_WIDTH;
		}

		if((x[i] > X_MAX) || (y[i] > Y_HALF)){
#ifdef DEBUG_INFO_TP
			if(debug_use){
				mutex_lock(p_mutex_nbx02);
				if(p_func_set->debug_022 != NULL){
					p_func_set->debug_022(TP_01);
					p_func_set->debug_022(TP_02);
				}
				mutex_unlock(p_mutex_nbx02);
			}
#endif /* DEBUG_INFO_TP */
			printk(KERN_ALERT "touch screen value is size over finger=%d, (0x%04x, 0x%04x)\n", i, x[i], y[i]);
			goto exit;
		}
	}

	if(client->addr == I2C_DEV_ADDER_UPPER){
		drv_data->finger_down_upper = EVENT;
	}else if(client->addr == I2C_DEV_ADDER_LOWER){
		drv_data->finger_down_lower = EVENT;
	}

	if(client->addr == I2C_DEV_ADDER_UPPER){
	}else if(client->addr == I2C_DEV_ADDER_LOWER){

		for(i = 0; i < finger_num; i++){
#ifdef CONFIG_TOUCHSCREEN_TMA300_ROTATE_180
			x[i] = X_MAX  - x[i];
			y[i] = Y_HALF - y[i] + Y_OFFSET;
#else
			x[i] = x[i];
			y[i] = y[i] + Y_OFFSET;
#endif
		}
	}

#ifdef DEBUG_INFO_TP
	if(debug_use){
		mutex_lock(p_mutex_nbx02);

		if(p_func_set->debug_021 != NULL){
			p_func_set->debug_021(tp_num, finger_id, x, y, z, &all_finger_num);
		}

		if(p_func_set->debug_001 != NULL){
			for(i = 0; i < 5; ++i){

				ret[i] = p_func_set->debug_001(&x[i] ,&y[i], i);
				if(ret[i] < 0){
					mutex_unlock(p_mutex_nbx02);
					goto exit;
				}
			}
			if(all_finger_num && (!ret[0] && !ret[1] && !ret[2] && !ret[3] && !ret[4])){
				mutex_unlock(p_mutex_nbx02);
				goto exit;
			}
		}
		mutex_unlock(p_mutex_nbx02);
	}
#endif /* DEBUG_INFO_TP */

	if ( all_finger_num > 0 ) {
		input_report_key(input, BTN_TOUCH, 1);
	}

	for(i = 0; i < all_finger_num; i++){

		input_report_abs(input, ABS_MT_TOUCH_MAJOR, z[i]);
		input_report_abs(input, ABS_MT_POSITION_X, x[i]);
		input_report_abs(input, ABS_MT_POSITION_Y, y[i]);

		input_mt_sync(input);

	}
	if ( all_finger_num == 0 ) {
		input_mt_sync(input);
	}
	input_sync(input);

	if(debug_print == 1){
		printk(KERN_ALERT "finger=%d (%d, %d) (%d, %d) (%d, %d) (%d, %d) (%d, %d)\n", all_finger_num, x[0], y[0], x[1], y[1], x[2], y[2], x[3], y[3], x[4], y[4]);
	}

exit:
	return;

}


static int tma300_init(struct i2c_client* client,
		       const struct i2c_device_id* id, u8 version){

	struct nbx_touch_data* data;
	const char* name = "";
	int err = 0;

	int gpio_num = 0;
	int irq_num = 0;

	struct nbx_touch_platform_data* platform_data;

	platform_data = client->dev.platform_data;

	if(client->addr == I2C_DEV_ADDER_UPPER){
		gpio_num = GPIO_UPPER_TP_IRQ;
		upper_client = client;
	}else if(client->addr == I2C_DEV_ADDER_LOWER){
		gpio_num = GPIO_LOWER_TP_IRQ;
		lower_client = client;
	}

	/* input setting */
	data = kzalloc(sizeof(struct nbx_touch_data), GFP_KERNEL);
	if(data == NULL){
		err = -ENOMEM;
		goto exit;
	}

	if(!init_done){

		s_input_dev = input_allocate_device();
		if(s_input_dev == NULL){
			err = -ENOMEM;
			goto exit_kfree;
		}
		s_input_dev->phys = "I2C-2";
	}

	data->input_dev = s_input_dev;
	data->client = client;

	i2c_set_clientdata(client, data);

	name = "nbx-tma300touch";
	s_input_dev->name = name;

	if(!init_done){

		s_input_dev->dev.parent = &(client->dev);
		s_input_dev->id.version = version;

		__set_bit(EV_ABS, s_input_dev->evbit);
		__set_bit(EV_KEY, s_input_dev->evbit);
		__set_bit(BTN_TOUCH, s_input_dev->keybit);

		input_set_abs_params(s_input_dev, ABS_X,
				X_MIN, X_MAX, 0, 0);
		input_set_abs_params(s_input_dev, ABS_Y,
				Y_MIN, Y_MAX, 0, 0);

		input_set_abs_params(s_input_dev, ABS_MT_TOUCH_MAJOR,
				     0,
				     MAX_WIDTH, /*MAX_PRESSURE,*/
				     0, 0);

		input_set_abs_params(s_input_dev, ABS_MT_POSITION_X,
				     X_MIN,
				     X_MAX,
				     0, 0);

		input_set_abs_params(s_input_dev, ABS_MT_POSITION_Y,
				     Y_MIN,
				     Y_MAX,
				     0, 0);

		err = input_register_device(s_input_dev);
		if(err){
			printk(KERN_ALERT "touch screen input register device err=%d\n", err);
			goto exit_input_free;
		}
	}

	v_sync_blank_scan(client);

	data->wq = create_singlethread_workqueue(name);
	if(!data->wq){
		err = -ENOMEM;
		goto exit_input_free;
	}

	INIT_WORK(&(data->work), multi_touch_work_5point);

	platform_data->gpio_enable(gpio_num);

	err = gpio_request(gpio_num, "NBX02 interrupt");
	if(err){
                goto exit_input_free;
	}
	err = gpio_direction_input(gpio_num);
	if(err){
                goto exit_input_free;
	}

	irq_num = gpio_to_irq(gpio_num);
        if(irq_num < 0){
                goto exit_input_free;
        }

	err = request_irq(irq_num, (irq_handler_t)nbx_touch_irq_handler, IRQF_TRIGGER_FALLING, "nbx_tma300", data);

	if(err){
		printk(KERN_ALERT "touch screen request irq error=%d\n", err);
		goto exit_input_free;
	}

	err = nbx_device_file_cleate(client);
	if(err){
		printk(KERN_ALERT "touch screen device file cleate error=%d\n", err);
		goto exit_input_free;
	}

#ifdef CONFIG_HAS_EARLYSUSPEND
	if(!init_done){
		early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;
		early_suspend.suspend = nbx_touch_early_suspend;
		early_suspend.resume = nbx_touch_late_resume;
		register_early_suspend(&early_suspend);
	}

#endif /* CONFIG_HAS_EARLYSUSPEND */

#ifdef DEBUG_INFO_TP
	if(debug_use){
		tp_flag_upper = TP_FLAG_CLEAR;
		tp_flag_lower = TP_FLAG_CLEAR;

		mutex_lock(p_mutex_nbx02);

		if(p_func_set->debug_002 != NULL){
			p_func_set->debug_002();
		}

		mutex_unlock(p_mutex_nbx02);
	}
#endif /* DEBUG_INFO_TP */

	return 0;

exit_input_free:

#ifdef CONFIG_HAS_EARLYSUSPEND
	unregister_early_suspend(&early_suspend);
#endif /* CONFIG_HAS_EARLYSUSPEND */

	input_free_device(s_input_dev);
	s_input_dev = NULL;
exit_kfree:

	kfree(data);
exit:

	return err;
}


#ifdef CONFIG_HAS_EARLYSUSPEND

static void nbx_touch_early_suspend(struct early_suspend* h){

	int ret_val = 0;
	int irq_num = 0;
	int gpio_num = 0;
	struct nbx_touch_data* data;

	ret_val = nbx_touch_sleep();
	if(ret_val == 0){
		early_suspend_done = EARLY_SUSPEND;
	}
	gpio_num = GPIO_UPPER_TP_IRQ;
	irq_num = gpio_to_irq(gpio_num);
	disable_irq(irq_num);
	gpio_num = GPIO_LOWER_TP_IRQ;
	irq_num = gpio_to_irq(gpio_num);
	disable_irq(irq_num);

	data = i2c_get_clientdata(upper_client);
	cancel_work_sync(&(data->work));
	data = i2c_get_clientdata(lower_client);
	cancel_work_sync(&(data->work));
}

static void nbx_touch_late_resume(struct early_suspend* h){

	int ret_val = 0;
        int irq_num = 0;
        int gpio_num = 0;

	ret_val = nbx_touch_wakeup();

	v_sync_blank_scan(upper_client);
	v_sync_blank_scan(lower_client);
#ifdef DEBUG_INFO_TP
	set_move_threshold(upper_client, 0, 0);
	set_move_threshold(lower_client, 0, 0);
#endif /* DEBUG_INFO_TP */
	gpio_num = GPIO_UPPER_TP_IRQ;
	irq_num = gpio_to_irq(gpio_num);
	enable_irq(irq_num);
	gpio_num = GPIO_LOWER_TP_IRQ;
	irq_num = gpio_to_irq(gpio_num);
	enable_irq(irq_num);

	if(early_suspend_done != 0){
		early_suspend_done = 0;
	}

	ac_filter_changed();

}

#endif /* CONFIG_HAS_EARLYSUSPEND */

#ifdef CONFIG_PM

static int nbx_touch_suspend(struct i2c_client* client, pm_message_t mesg){

	int ret_val = 0;
	int irq_num = 0;
	int gpio_num = 0;
	struct nbx_touch_data* data;

	if(early_suspend_done == EARLY_SUSPEND){
		return 0;
	}else if(early_suspend_done == 0){
		/*
		 * UPPER_SUSPEND bit 0 and LOWER_SUSPEND bit is 0
		 * in this case, this function is called first.
		 * TP power is on.
		 *
		 * UPPER_SUSPEND bit 1 and LOWER_SUSPEND bit is 0
		 * UPPER_SUSPEND bit 0 and LOWER_SUSPEND bit is 1
		 * in this case, this function was already done.
		 * TP power is off.
		 */
		ret_val = nbx_touch_sleep();
	}

	if(client->addr == I2C_DEV_ADDER_UPPER){
		gpio_num = GPIO_UPPER_TP_IRQ;
		early_suspend_done |= UPPER_SUSPEND;
	}else if(client->addr == I2C_DEV_ADDER_LOWER){
		gpio_num = GPIO_LOWER_TP_IRQ;
		early_suspend_done |= LOWER_SUSPEND;
	}

	irq_num = gpio_to_irq(gpio_num);
	disable_irq(irq_num);

	data = i2c_get_clientdata(client);
	cancel_work_sync(&(data->work));

	return ret_val;
}

static int nbx_touch_resume(struct i2c_client* client){

	if(client->addr == I2C_DEV_ADDER_UPPER){
	}else if(client->addr == I2C_DEV_ADDER_LOWER){
	}

	return 0;
}
#endif /* CONFIG_PM */

static int nbx_touch_probe(struct i2c_client* client,
			const struct i2c_device_id* id){

	struct i2c_adapter* adapter = client->adapter;
	int err = 0;
	u8 setting_addr = 0;
	u8 version;

#ifdef DEBUG_INFO_TP
	int* p_debug = NULL;
#endif /* DEBUG_INFO_TP */

	if(!init_done){
		err = touch_power_on(client);
		if(err){
			printk(KERN_ALERT "touch screen power on error\n");
			goto exit;
		}
	}


	if (!i2c_check_functionality(adapter, I2C_FUNC_I2C)){
		printk(KERN_ALERT "touch screen i2c_check_functionality error\n");
		return -ENODEV;
	}

	setting_addr = ADDR_FW_VER;
	/* dummy write for wake up */
	err = nbx_touch_write(client, 1, &setting_addr);
	if(err < 0){

		printk(KERN_ALERT "touch screen i2c write error=%d\n", err);
		goto exit;
	}

	msleep(1);
	err = nbx_touch_read_from_addr(client, setting_addr, 1, &version);
	if(err < 0){
		printk(KERN_ALERT "touch screen i2c read error=%d\n", err);
		goto exit;
	}

	if(client->addr == I2C_DEV_ADDER_UPPER){
		upper_tp_version = version;
		printk(KERN_ALERT "TP FW upper version=0x%x\n", version);
	}else if(client->addr == I2C_DEV_ADDER_LOWER){
		lower_tp_version = version;
		printk(KERN_ALERT "TP FW lower version=0x%x\n", version);
	}

	if(upper_tp_version != 0 && lower_tp_version != 0 &&
	   upper_tp_version != lower_tp_version){
		printk(KERN_ALERT "different Firmware version error upper=0x%x, lower=0x%x\n", upper_tp_version, lower_tp_version);
		err = -ENODEV;
		goto exit;
	}

#ifdef DEBUG_INFO_TP
	if(!init_done){

		p_debug = ioremap(SZ_512M - SZ_2M, 4096);
		if(!p_debug){
			debug_use = 0;
		}else{
			debug_use = 1;
			p_func_set = kzalloc(sizeof(struct s_debug_func), GFP_KERNEL);
			p_mutex_nbx02 = kzalloc(sizeof(struct mutex), GFP_KERNEL);

			*p_debug = (int)p_func_set;
			*(p_debug + 1) = (int)p_mutex_nbx02;
			*(p_debug + 2) = 0;
			*(p_debug + 3) = 0;

			mutex_init(p_mutex_nbx02);
		}
	}
#endif /* DEBUG_INFO_TP */

	err = tma300_init(client, id, version);
	if(err < 0){
		printk(KERN_ALERT "touch screen tma300 init error\n");
		goto exit;
	}


	if(!init_done){

		err = nbx_ec_ipc_lidsw_register_callback(nbx_lid_changed);
		if(err != 0){
			goto exit;
		}

		err = nbx_ec_ipc_acplug_register_callback(nbx_ac_changed);
		if(err != 0){
			nbx_ec_ipc_lidsw_unregister_callback(nbx_lid_changed);
			goto exit;
		}

	}
	init_done = 1;

	ac_filter_changed();
exit:

	return err;

}


static const struct i2c_device_id nbx_touch_id[] = {
	{ "i2c_nbx_touch", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, nbx_touch_id);

static struct i2c_driver nbx_touch_driver = {
	.driver = {
		.name	= "i2c_nbx_touch",
	},
	.probe		= nbx_touch_probe,
	.id_table	= nbx_touch_id,

	.class		= I2C_CLASS_HWMON,
#ifdef CONFIG_PM
	.suspend  = nbx_touch_suspend,
	.resume   = nbx_touch_resume,
#endif /* CONFIG_PM */
};

static int __init nbx_touch_init(void){
	int ret_val = 0;

	ret_val = i2c_add_driver(&nbx_touch_driver);

	return ret_val;
}

static void __exit nbx_touch_exit(void){

	int irq_num = 0;
	int gpio_num = 0;
	struct nbx_touch_data* data;

#ifdef CONFIG_HAS_EARLYSUSPEND
	unregister_early_suspend(&early_suspend);
#endif /* CONFIG_HAS_EARLYSUSPEND */

	nbx_ec_ipc_lidsw_unregister_callback(nbx_lid_changed);

	nbx_ec_ipc_acplug_unregister_callback(nbx_ac_changed);

	/* interrupt upper */
	data = i2c_get_clientdata(upper_client);
	gpio_num = GPIO_UPPER_TP_IRQ;
	irq_num = gpio_to_irq(gpio_num);
	disable_irq(irq_num);
	free_irq(irq_num, data);
	gpio_free(GPIO_UPPER_TP_IRQ);

	/* interrupt lower */
	data = i2c_get_clientdata(lower_client);
	gpio_num = GPIO_LOWER_TP_IRQ;
	irq_num = gpio_to_irq(gpio_num);
	disable_irq(irq_num);
	free_irq(irq_num, data);
	gpio_free(GPIO_LOWER_TP_IRQ);

	msleep(80);

	/* power off */
	nbx_touch_sleep();

	gpio_free(TEGRA_GPIO_PA2);
	gpio_free(TEGRA_GPIO_PA5);
	gpio_free(TEGRA_GPIO_PA4);
	gpio_free(TEGRA_GPIO_PH0);
	gpio_free(TEGRA_GPIO_PH1);

	/* upper */
	data = i2c_get_clientdata(upper_client);

	cancel_work_sync(&(data->work));
	destroy_workqueue(data->wq);

	input_unregister_device(data->input_dev);

	kfree(data);

	/* lower */
	data = i2c_get_clientdata(lower_client);

	cancel_work_sync(&(data->work));
	destroy_workqueue(data->wq);

	/*
	input_unregister_device(data->input_dev);
	*/
	kfree(data);


	input_free_device(s_input_dev);
	s_input_dev = NULL;

	i2c_del_driver(&nbx_touch_driver);

	return;
}


MODULE_DESCRIPTION("I2C nbx_touch driver");
MODULE_LICENSE("GPL");

module_init(nbx_touch_init);
module_exit(nbx_touch_exit);
