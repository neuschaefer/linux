/*
 * Wacom Penabled Driver for I2C
 *
 * Copyright (c) 2011 - 2013 Tatsunosuke Tobita, Wacom.
 * <tobita.tatsunosuke@wacom.co.jp>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software
 * Foundation; either version of 2 of the License,
 * or (at your option) any later version.
 */
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/async.h>
#include <linux/hrtimer.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <mach/irqs.h>
#include <mach/system.h>
#include <mach/hardware.h>
#include <mach/sys_config.h>
#include <linux/suspend.h>
#include <asm/unaligned.h>
#include "ctp_platform_ops.h"

#include <mach/gpio.h>

#ifdef CONFIG_HAS_EARLYSUSPEND
    #include <linux/pm.h>
	#include <linux/power/aw_pm.h>
#endif

#ifdef CONFIG_AW_AXP
//#include <linux/axp-rw.h>
#include <linux/mfd/axp-mfd.h>
#endif

#define WACOM_CMD_QUERY0	0x04
#define WACOM_CMD_QUERY1	0x00
#define WACOM_CMD_QUERY2	0x33
#define WACOM_CMD_QUERY3	0x02
#define WACOM_CMD_THROW0	0x05
#define WACOM_CMD_THROW1	0x00
#define WACOM_QUERY_SIZE	       19

#define WACOM_CMD_WAKEUPSLEEP0	0x04
#define WACOM_CMD_WAKEUPSLEEP1	0x00
#define WACOM_CMD_WAKEUPSLEEP2	0x01
#define WACOM_CMD_WAKEUPSLEEP3	0x08

/*resolution definion according to touch screen */
#define ETP_NAME				"wacom_i2c"
#define ETS_RESET_LOW_PERIOD		(150)
#define ETS_INITIAL_HIGH_PERIOD		(150)
#define ETS_WAKEUP_LOW_PERIOD	(100)
#define ETS_WAKEUP_HIGH_PERIOD	(100)
#define ETS_POLL_DELAY			(10)	
/* ms delay between samples */
#define ETS_POLL_PERIOD			(10)	
/* ms delay between samples */
#define SCREEN_MAX_HEIGHT		1440//(screen_max_x)
#define SCREEN_MAX_WIDTH		1080//(screen_max_y)
#define PRESS_MAX				(255)

static void* __iomem gpio_addr = NULL;
static int gpio_int_hdle = 0;
static int gpio_reset_hdle = 0;
static int gpio_pwe_hdle = 0;
static int gpio_pdet_hdle = 0;

static int gpio_reset_enable = 1;
static int gpio_pwe_enable = 1;

static int screen_max_x = 0;
static int screen_max_y = 0;
static int revert_x_flag = 0;
static int revert_y_flag = 0;
static int exchange_x_y_flag = 0;

static __u32 twi_addr = 0;
static __u32 twi_id = 0;
static int	int_cfg_addr[]={PIO_INT_CFG0_OFFSET,PIO_INT_CFG1_OFFSET,
			PIO_INT_CFG2_OFFSET, PIO_INT_CFG3_OFFSET};
/* Addresses to scan */
union{
	unsigned short dirty_addr_buf[2];
	const unsigned short normal_i2c[2];
}u_i2c_addr_etp = {{0x00},};


struct wacom_features {
	int x_max;
	int y_max;
	int pressure_max;
	int fw_version;
};

struct wacom_i2c {
	struct i2c_client *client;
	struct input_dev *input;
	u8 data[WACOM_QUERY_SIZE];
	bool prox;
	int tool;
	struct work_struct    pen_event_work;
	struct workqueue_struct *ts_workqueue; 	struct delayed_work work;
};
static struct i2c_client *this_client;

static int wacom_query_device(struct i2c_client *client,
			      struct wacom_features *features)
{
	int ret;
	u8 cmd1[] = { WACOM_CMD_QUERY0, WACOM_CMD_QUERY1,WACOM_CMD_QUERY2, WACOM_CMD_QUERY3  };
	u8 cmd2[] = { WACOM_CMD_THROW0,WACOM_CMD_THROW1 };
	u8 data[WACOM_QUERY_SIZE];
	struct i2c_msg msgs[] = {
		{
			.addr = client->addr,
			.flags = 0,
			.len = sizeof(cmd1),
			.buf = cmd1,
		},
		{
			.addr = client->addr,
			.flags = 0,
			.len = sizeof(cmd2),
			.buf = cmd2,
		},
		{
			.addr = client->addr,
			.flags = I2C_M_RD,
			.len = sizeof(data),
			.buf = data,
		},
	};
	ret = i2c_transfer(client->adapter, msgs, ARRAY_SIZE(msgs));
	if (ret < 0)
		return ret;
	if (ret != ARRAY_SIZE(msgs))
		return -EIO;
	features->x_max = get_unaligned_le16(&data[3]);
	features->y_max = get_unaligned_le16(&data[5]);
	features->pressure_max = get_unaligned_le16(&data[11]);
	features->fw_version =data[14];
	features->fw_version =features->fw_version<<8|data[13];
	printk("x_max:%d, y_max:%d, pressure:%d, fw:%x\n",features->x_max, features->y_max,features->pressure_max, features->fw_version);//dev_dbg

	return 0;
}

static int wacom_i2c_open(struct input_dev *dev)
{
	struct wacom_i2c *wac_i2c = input_get_drvdata(dev);
	struct i2c_client *client = wac_i2c->client;

	enable_irq(client->irq);

	return 0;
}

static void wacom_i2c_close(struct input_dev *dev)
{
	struct wacom_i2c *wac_i2c = input_get_drvdata(dev);
	struct i2c_client *client = wac_i2c->client;

	disable_irq(client->irq);
}

/** * etp_set_gpio_mode - a
ccording sysconfig's subkey "etp_io_port" to config io port.
 *
 * return value:      0:      success;
 *                    others: fail; 
 */
static int etp_set_gpio_mode(void)
{
	//int reg_val;
	int ret = 0;
	//config gpio to io mode
	printk("%s: config gpio to io mode. \n", __func__);
#ifndef SYSCONFIG_GPIO_ENABLE
#else
	if(gpio_int_hdle){
		gpio_release(gpio_int_hdle, 2);
	}
	gpio_int_hdle = gpio_request_ex("etp_para", "etp_io_port");
	
if(!gpio_int_hdle){
		printk("request etp_io_port failed. \n");
		ret= -1;
		goto request_tp_io_port_failed;
	}
#endif
	return ret;

request_tp_io_port_failed:
	return ret;
}


/**
 * etp_free_platform_
resource - corresponding with etp_init_platform_resource *
 */
static void etp_free_platform_resource(void)
{
	printk("=======%s=========.\n", __func__);

	if(gpio_addr){		
		iounmap(gpio_addr);	
	}
	
	if(gpio_int_hdle){
		gpio_release(gpio_int_hdle, 2);	
    }

/*		
    if(gpio_wakeup_hdle){
		gpio_release(gpio_wakeup_hdle, 2);

	}*/
	if(gpio_reset_hdle){
		gpio_release(gpio_reset_hdle, 2);
	}
	
	if(gpio_pwe_hdle){
		gpio_release(gpio_pwe_hdle, 2);
	}	
	
	
	return;
}


/**
 * etp_init_platform_resource - initialize platform related resource
 * return value: 0 : success
 *               -EIO:  i/o err. 
 * 
*/
static int etp_init_platform_resource(void)
{
	int ret = 0;

	gpio_addr = ioremap(PIO_BASE_ADDRESS, PIO_RANGE_SIZE);
	//printk("%s, gpio_addr = 0x%x. \n", __func__, gpio_addr);
	if(!gpio_addr) {
		ret = -EIO;
		goto exit_ioremap_failed;	
	}


	gpio_reset_hdle = gpio_request_ex("etp_para", "etp_reset");
	if(!gpio_reset_hdle){
		pr_warning("%s: tp_reset request gpio fail!\n", __func__);
		gpio_reset_enable = 0;
	}
	
	gpio_pwe_hdle = gpio_request_ex("etp_para", "etp_pwe");
	if(!gpio_pwe_hdle){
		  pr_warning("%s: gpio_pwe request gpio fail!\n", __func__);
		  gpio_pwe_enable=0;
	}

	gpio_pdet_hdle = gpio_request_ex("etp_para", "etp_pdct");
	if(!gpio_pdet_hdle){
		  pr_warning("%s: etp_pdct request gpio fail!\n", __func__);
	}
	
	return ret;
exit_ioremap_failed:
	etp_free_platform_resource();
	return ret;
}
/**
 * etp_fetch_sysconfig_para - get config info from sysconfig.fex file.
 * return value:  
 *                = 0; success;
 *                < 0; err
 */
static int etp_fetch_sysconfig_para(void)
{
	int ret = -1;
	int etp_used = -1;
	char name[I2C_NAME_SIZE];	
	script_parser_value_type_t type = SCIRPT_PARSER_VALUE_TYPE_STRING;
	printk("%s. \n", __func__);
	if(SCRIPT_PARSER_OK != script_parser_fetch("etp_para", "etp_used", &etp_used, 1)){
		pr_err("%s: script_parser_fetch err. \n", __func__);
		goto script_parser_fetch_err;
	}

	if(1 != etp_used){
		pr_err("%s: etp_unused. \n", __func__);
		return ret;
	}

	if(SCRIPT_PARSER_OK != script_parser_fetch_ex("etp_para","etp_name", (int *)(&name), &type, sizeof(name)/sizeof(int)))
	{		
	    pr_err("%s: script_parser_fetch err. \n", __func__);
		goto script_parser_fetch_err;
	}
	if(strcmp(ETP_NAME, name)){
		pr_err("%s: name %s does not match etp_NAME. \n", __func__, name);
		pr_err(ETP_NAME);
		//ret = 1;
		return ret;
	}

	if(SCRIPT_PARSER_OK != script_parser_fetch("etp_para", "etp_twi_addr", &twi_addr, sizeof(twi_addr)/sizeof(__u32))){
		pr_err("%s: script_parser_fetch err. \n", name);
		goto script_parser_fetch_err;
	}
	//big-endian or small-endian?
	u_i2c_addr_etp.dirty_addr_buf[0] = twi_addr;
	u_i2c_addr_etp.dirty_addr_buf[1] = I2C_CLIENT_END;
	printk("%s: after: etp_twi_addr is 0x%x, dirty_addr_buf: 0x%hx. dirty_addr_buf[1]: 0x%hx \n", __func__, twi_addr, u_i2c_addr_etp.dirty_addr_buf[0], u_i2c_addr_etp.dirty_addr_buf[1]);
	//printk("%s: after: etp_twi_addr is 0x%x, u32_dirty_addr_buf: 0x%hx. u32_dirty_addr_buf[1]: 0x%hx \n", __func__, twi_addr, u32_dirty_addr_buf[0],u32_dirty_addr_buf[1]);

	if(SCRIPT_PARSER_OK != script_parser_fetch("etp_para", "etp_twi_id", &twi_id, sizeof(twi_id)/sizeof(__u32))){
		pr_err("%s: script_parser_fetch err. \n", name);
		goto script_parser_fetch_err;
	}
	printk("%s: etp_twi_id is %d. \n", __func__, twi_id);
	
	if(SCRIPT_PARSER_OK != script_parser_fetch("etp_para", "etp_screen_max_x", &screen_max_x, 1)){
		pr_err("%s: script_parser_fetch err. \n", __func__);
		goto script_parser_fetch_err;
	}

	pr_info("%s: screen_max_x = %d. \n", __func__, screen_max_x);

	if(SCRIPT_PARSER_OK != script_parser_fetch("etp_para", "etp_screen_max_y", &screen_max_y, 1)){
		pr_err("%s: script_parser_fetch err. \n", __func__);
		goto script_parser_fetch_err;
	}

	pr_info("%s: screen_max_y = %d. \n", __func__, screen_max_y);

	if(SCRIPT_PARSER_OK != script_parser_fetch("etp_para", "etp_revert_x_flag", &revert_x_flag, 1)){
		pr_err("%s: script_parser_fetch err. \n", __func__);
		goto script_parser_fetch_err;
	}
	
   pr_info("%s: revert_x_flag = %d. \n", __func__, revert_x_flag);

	if(SCRIPT_PARSER_OK != script_parser_fetch("etp_para", "etp_revert_y_flag", &revert_y_flag, 1)){
		pr_err("%s: script_parser_fetch err. \n", __func__);
		goto script_parser_fetch_err;
	}
	pr_info("%s: revert_y_flag = %d. \n", __func__, revert_y_flag);

	if(SCRIPT_PARSER_OK != script_parser_fetch("etp_para", "etp_exchange_x_y_flag", &exchange_x_y_flag, 1)){
		pr_err("inferpoint_ts: script_parser_fetch err. \n");
		goto script_parser_fetch_err;
	}
	pr_info("%s: exchange_x_y_flag = %d. \n", __func__, exchange_x_y_flag);

	return 0;

script_parser_fetch_err:	
	pr_notice("=========script_parser_fetch_err============\n");
	return ret;
}

/*** etp_reset - function**/

static void etp_reset(void)
{
	printk("%s. \n", __func__);

	if(gpio_reset_enable){
		mdelay(50);
		if(EGPIO_SUCCESS != gpio_write_one_pin_value(gpio_reset_hdle, 1, "etp_reset")){
			printk("%s: err when operate gpio. \n", __func__);
		}
		mdelay(50);
		if(EGPIO_SUCCESS != gpio_write_one_pin_value(gpio_reset_hdle, 0, "etp_reset")){
			printk("%s: err when operate gpio. \n", __func__);
		}
		mdelay(110);
		if(EGPIO_SUCCESS!= gpio_write_one_pin_value(gpio_reset_hdle, 1, "etp_reset")){
			printk("%s: err when operate gpio. \n", __func__);
		}
		mdelay(100);
	}
	
}

/**
 * etp_detect - Device detection callback for automatic device creation
 * return value:  
 *                   = 0; success;
 *                   < 0; err
 */
int etp_detect(struct i2c_client *client, struct i2c_board_info *info)
{
	struct i2c_adapter *adapter = client->adapter;

	if(twi_id == adapter->nr)
	{
		printk("%s: Detected chip %s at adapter %d, address 0x%02x\n",
			 __func__, ETP_NAME, i2c_adapter_id(adapter), client->addr);

		strlcpy(info->type, ETP_NAME, I2C_NAME_SIZE);
		return 0;
	}else{
		return -ENODEV;
	}
}


static struct ctp_platform_ops etp_ops = 
{	
	.set_gpio_mode     = etp_set_gpio_mode,		
	.init_platform_resource = etp_init_platform_resource,	
	.free_platform_resource = etp_free_platform_resource,	
	.fetch_sysconfig_para = etp_fetch_sysconfig_para,	
	.ts_reset =          etp_reset,	
	.ts_detect = 		etp_detect,
};
//////hxm add for test PE01
static u32 hdle0;
static int  gpio_eint_etp_init(void)
{
    int pending;
    hdle0 = sw_gpio_irq_request("etp_para", "etp_int_port", TRIG_LEVL_LOW);//TRIG_EDGE_POSITIVE);
    if (!hdle0) {
        printk("request gpio irq failed\n");
        return -1;
    }
 
    sw_gpio_eint_set_enable(hdle0, 1);
    pending = sw_gpio_eint_get_irqpd_sta(hdle0);
    if (pending < 0) {
        printk("get irq pending failed\n");
    }
    sw_gpio_eint_clr_irqpd_sta(hdle0);
}

static int  gpio_eint_etp_exit(void)
{

	sw_gpio_eint_set_enable(hdle0, 0);
	sw_gpio_irq_free(hdle0);
	return 0;
}

//end

static int wac_work_func(void)
{
    struct wacom_i2c *wac_i2c =i2c_get_clientdata(this_client);
	struct input_dev *input = wac_i2c->input;
	u8 *data = wac_i2c->data;
	unsigned int x, y, pressure,temp;
	unsigned char tsw, f1, f2, ers;
	int error;
       error = i2c_master_recv(wac_i2c->client,wac_i2c->data, sizeof(wac_i2c->data));
	if (error < 0)
		goto out;
	tsw = data[3] & 0x01;
	ers = data[3] & 0x04;
	f1 = data[3] & 0x02;
	f2 = data[3] & 0x10;
	x = le16_to_cpup((__le16 *)&data[4]);
	//printk("===========================%s,%d,x=%d =====================\n", __func__,__LINE__,x); 
	y = le16_to_cpup((__le16 *)&data[6]);
	//printk("===========================%s,%d,y=%d =====================\n", __func__,__LINE__,y); 
	pressure = le16_to_cpup((__le16 *)&data[8]);
	//printk("===========================%s,%d,pressure=%d =====================\n", __func__,__LINE__,pressure);
	if (!wac_i2c->prox)
		wac_i2c->tool = (data[3] & 0x0c) ?BTN_TOOL_RUBBER : BTN_TOOL_PEN;
	x = (x*1080)/10368;
	y =1440-(y*1440)/14024;//13824;
	temp=x;
	x=y;
	y=temp;
	printk("=%s,x=%d \n", __func__,x); 
	printk("=%s,y=%d \n", __func__,y); 
	printk("=%s,pressure=%d \n", __func__,pressure);
	
	wac_i2c->prox = data[3] & 0x20;

	input_report_key(input, BTN_TOUCH, tsw || ers);
	input_report_key(input, wac_i2c->tool, wac_i2c->prox);
	input_report_key(input, BTN_STYLUS, f1);
	input_report_key(input, BTN_STYLUS2, f2);
	input_report_abs(input, ABS_X, x);
	input_report_abs(input, ABS_Y, y);
	input_report_abs(input, ABS_PRESSURE, pressure);	
	input_sync(input);

out:	
	enable_irq(SW_INT_IRQNO_PIO);

}

static irqreturn_t wacom_i2c_irq(int irq, void *dev_id)
{     
	int reg_val = 0;
	 struct wacom_i2c *wac_i2c = dev_id;
	//printk("===========================%s =====================\n", __func__);
	reg_val=sw_gpio_eint_get_irqpd_sta(hdle0);
	if(1==reg_val)
	{	
	   disable_irq_nosync(SW_INT_IRQNO_PIO);
	   //disable_irq(SW_INT_IRQNO_PIO);

          sw_gpio_eint_clr_irqpd_sta(hdle0);//clear penirq

	   if (!work_pending(&wac_i2c->pen_event_work)) 
		{
			//printk("Enter work\n");
			queue_work(wac_i2c->ts_workqueue, &wac_i2c->pen_event_work);
		}
	}	    
	else		
	{
		//printk("Other Interrupt not wac_i2c \n");
		return IRQ_NONE;
	}
	return IRQ_HANDLED;		
}


static int wacom_i2c_probe(struct i2c_client *client,
				     const struct i2c_device_id *id)
{
	struct wacom_i2c *wac_i2c;
	struct input_dev *input;
	struct wacom_features features = { 0 };
	int error;
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		dev_err(&client->dev, "i2c_check_functionality error\n");
		return -EIO;
	}

#ifdef CONFIG_AW_AXP
	//turn on
	struct device *axp_dev = axp_get_dev();
	uint8_t val;

	axp_read(axp_dev, POWER20_GPIO0_CTL, &val);
	printk("val hxm READ POWER20_GPIO0_CTL=%x\n",val);
	val=val & 0xFB;
	axp_write(axp_dev,POWER20_GPIO0_CTL,val);
	printk("val hxm WRITE POWER20_GPIO0_CTL =%x\n",val);
	axp_read(axp_dev, POWER20_GPIO0_CTL, &val);
	printk("val hxm READ POWER20_GPIO0_CTL =%x\n",val);

	//hxm add  set ldo_io0 output voltage;

	axp_read(axp_dev, POWER20_GPIO0_VOL, &val);
	printk("val hxm READ ldo_io0=%x\n",val);
	val=val |0x50;
	axp_write(axp_dev,POWER20_GPIO0_VOL,val);
	printk("val hxm WRITE ldo_io0=%x\n",val);
	axp_read(axp_dev, POWER20_GPIO0_VOL, &val);
	printk("val hxm =%x\n",val);
#endif

	etp_ops.ts_reset();

	error = wacom_query_device(client, &features);
	printk("wacom_query_device error =%d \n",error);
	if (error)
		return error;
	wac_i2c = kzalloc(sizeof(*wac_i2c), GFP_KERNEL);
	input = input_allocate_device();
	if (!wac_i2c || !input) {
		error = -ENOMEM;
		goto err_free_mem;
	}
	wac_i2c->client = client;
	wac_i2c->input = input;

	input->name = "Wacom_I2C_Digitizer";
	input->id.bustype = BUS_I2C;
	input->id.vendor = 0x56a;
	input->id.version = features.fw_version;
	input->dev.parent = &client->dev;
	//input->open = wacom_i2c_open;
	//input->close = wacom_i2c_close;

	input->evbit[0] |= BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS);

	__set_bit(BTN_TOOL_PEN, input->keybit);
	__set_bit(BTN_TOOL_RUBBER, input->keybit);
	__set_bit(BTN_STYLUS, input->keybit);
	__set_bit(BTN_STYLUS2, input->keybit);
	__set_bit(BTN_TOUCH, input->keybit);

	//input_set_abs_params(input, ABS_X, 0, features.x_max, 0, 0);
	//input_set_abs_params(input, ABS_Y, 0, features.y_max, 0, 0);
	//input_set_abs_params(input, ABS_PRESSURE,0, features.pressure_max, 0, 0);

	input_set_abs_params(input, ABS_X, 0, 1440, 0, 0);
	input_set_abs_params(input, ABS_Y, 0, 1080, 0, 0);
	input_set_abs_params(input, ABS_PRESSURE,0, 1023, 0, 0);


	input_set_drvdata(input, wac_i2c);
	
	this_client = client;
	
	i2c_set_clientdata(client, wac_i2c);
	

	INIT_WORK(&wac_i2c->pen_event_work, wac_work_func);
	
	wac_i2c->ts_workqueue = create_singlethread_workqueue(dev_name(&client->dev));
	if (!wac_i2c->ts_workqueue) {
		error = -ESRCH;
		goto exit_create_singlethread;
	}
	
  
    error = request_irq(SW_INT_IRQNO_PIO, wacom_i2c_irq,/*IRQF_TRIGGER_FALLING |IRQF_TRIGGER_RISING | */IRQF_SHARED/*IRQF_TRIGGER_LOW| IRQF_TRIGGER_RISING|IRQF_SHARED*/  ,"wacom_i2c", wac_i2c);
	if (error < 0) {		
	printk( "wacom_i2c_irq : request irq failed\n");
	return error;	
	}
	
	gpio_eint_etp_init();
	/* Disable the IRQ, we'll enable it in wac_i2c_open() */
	//disable_irq(client->irq);

	error = input_register_device(wac_i2c->input);
	if (error) {
		dev_err(&client->dev,
			"Failed to register input device, error: %d\n", error);
		goto err_free_irq;
	}
	printk("===========================%s over=====================\n", __func__);
	return 0;

err_free_irq:
	free_irq(client->irq, wac_i2c);
	gpio_eint_etp_exit();
err_free_mem:
	input_free_device(input);
	kfree(wac_i2c);
exit_create_singlethread:
	i2c_set_clientdata(client, NULL);
	kfree(wac_i2c);	

	return error;
}

static int wacom_i2c_remove(struct i2c_client *client)
{
	struct wacom_i2c *wac_i2c = i2c_get_clientdata(client);

	free_irq(client->irq, wac_i2c);
	input_unregister_device(wac_i2c->input);
	kfree(wac_i2c);

	return 0;
}

static int wacom_i2c_set_sleep(struct i2c_client *client)
{
	int ret;
	u8 cmd1[] = { WACOM_CMD_WAKEUPSLEEP0, WACOM_CMD_WAKEUPSLEEP1,
			WACOM_CMD_WAKEUPSLEEP2, WACOM_CMD_WAKEUPSLEEP3 };
	struct i2c_msg msgs[] = {
		{
			.addr = client->addr,
			.flags = 0,
			.len = sizeof(cmd1),
			.buf = cmd1,
		},
	};
	ret = i2c_transfer(client->adapter, msgs, ARRAY_SIZE(msgs));
	if (ret < 0) {
		printk("wacom i2c write error = %d\n",ret);
		return ret;
	}
	if (ret != ARRAY_SIZE(msgs))
		return -EIO;
	
	return 0;
}

static int wacom_i2c_set_on(struct i2c_client *client)
{
	int ret;
	u8 cmd1[] = { WACOM_CMD_WAKEUPSLEEP0, WACOM_CMD_WAKEUPSLEEP1,
			WACOM_CMD_WAKEUPSLEEP1, WACOM_CMD_WAKEUPSLEEP3 };
	struct i2c_msg msgs[] = {
		{
			.addr = client->addr,
			.flags = 0,
			.len = sizeof(cmd1),
			.buf = cmd1,
		},
	};
	ret = i2c_transfer(client->adapter, msgs, ARRAY_SIZE(msgs));
	if (ret < 0) {
		printk("wacom i2c write error = %d\n",ret);
		return ret;
	}
	if (ret != ARRAY_SIZE(msgs))
		return -EIO;
	
	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int wacom_i2c_suspend(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	
       return wacom_i2c_set_sleep(client);
}

static int wacom_i2c_resume(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
      
	return wacom_i2c_set_on(client);
}
#endif

static SIMPLE_DEV_PM_OPS(wacom_i2c_pm, wacom_i2c_suspend, wacom_i2c_resume);

static const struct i2c_device_id wacom_i2c_id[] = {
	{ ETP_NAME, 0 },
	{ },
};
MODULE_DEVICE_TABLE(i2c, wacom_i2c_id);
	
static struct i2c_driver wacom_i2c_driver = {
    .class = I2C_CLASS_HWMON,
	.probe		= wacom_i2c_probe,
	.remove		= wacom_i2c_remove,
	.id_table	= wacom_i2c_id,
	
	.driver	= {
	.name	= ETP_NAME,
	.owner	= THIS_MODULE,
	#ifdef CONFIG_PM_SLEEP
	.pm	= &wacom_i2c_pm,
	#endif

	},
	.address_list	= u_i2c_addr_etp.normal_i2c,
};

static int __init wacom_i2c_init(void)
{
	int ret = -1;
	int err = -1;

	printk("===========================%s=====================\n", __func__);

	if (etp_ops.fetch_sysconfig_para)
	{
		if(etp_ops.fetch_sysconfig_para()){
			printk("%s: err.\n", __func__);
			return -1;
		}
	}
	printk("%s: after fetch_sysconfig_para:  normal_i2c: 0x%hx. normal_i2c[1]: 0x%hx \n", \
	__func__, u_i2c_addr_etp.normal_i2c[0], u_i2c_addr_etp.normal_i2c[1]);

	err = etp_ops.init_platform_resource();
	if(0 != err){
		printk("%s:etp_ops.init_platform_resource err. \n", __func__);    
	}

	if(gpio_pwe_enable){
		if(EGPIO_SUCCESS != gpio_write_one_pin_value(gpio_pwe_hdle, 0, "etp_pwe"))
		{
		printk("%s: err when operate gpio. \n", __func__);
		}
		printk("%s: etp_pwe to low . \n", __func__);
	}

    //etp_ops.ts_reset();

	wacom_i2c_driver.detect = etp_ops.ts_detect;

	ret = i2c_add_driver(&wacom_i2c_driver);
	
	return ret;
}

/*******************************************************	

********************************************************/
static void __exit wacom_ts_exit(void)
{
	i2c_del_driver(&wacom_i2c_driver);
	etp_ops.free_platform_resource();
	gpio_eint_etp_exit();
	return;
}
module_init(wacom_i2c_init);
module_exit(wacom_ts_exit);

MODULE_AUTHOR("Tatsunosuke Tobita <tobita.tatsunosuke@wacom.co.jp>");
MODULE_DESCRIPTION("WACOM EMR I2C Driver");
MODULE_LICENSE("GPL");
