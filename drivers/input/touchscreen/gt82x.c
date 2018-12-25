/*---------------------------------------------------------------------------------------------------------
 * driver/input/touchscreen/goodix_touch.c
 *
 * Copyright(c) 2010 Goodix Technology Corp.     
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Change Date: 
 *		2010.11.11, add point_queue's definiens.     
 *                             
 * 		2011.03.09, rewrite point_queue's definiens.  
 *   
 * 		2011.05.12, delete point_queue for Android 2.2/Android 2.3 and so on.
 *                                                                                              
 *---------------------------------------------------------------------------------------------------------*/
#include <linux/i2c.h>
#include <linux/input.h>
#include "gt82x_touch.h"
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/suspend.h>
#include <asm/irq.h>
#include <asm/io.h>

#include <mach/irqs.h>
#include <mach/system.h>
#include <mach/hardware.h>
#include <mach/sys_config.h>
#include "ctp_platform_ops.h"

#if defined(CONFIG_HAS_EARLYSUSPEND) || defined(CONFIG_PM)
    #include <linux/pm.h>
    #include <linux/earlysuspend.h>
    #include <linux/power/aw_pm.h>
#endif

#define FOR_TSLIB_TEST
#define PRINT_INT_INFO
#define PRINT_POINT_INFO
#define PRINT_INITPANEL_INFO
#define PRINT_SUSPEND_INFO
#define TEST_I2C_TRANSFER
//#undef CONFIG_HAS_EARLYSUSPEND

#ifndef GUITAR_GT80X
#error The code does not match the hardware version.
#endif

struct goodix_ts_data {
	int retry;
	int panel_type;
	uint8_t bad_data;
	char phys[32];		
	struct i2c_client *client;
	struct input_dev *input_dev;
	uint8_t use_irq;
	uint8_t use_shutdown;
	uint32_t gpio_shutdown;
	uint32_t gpio_irq;
	uint32_t screen_width;
	uint32_t screen_height;
	struct ts_event	event;
	struct hrtimer timer;
	struct work_struct  work;
	int (*power)(struct goodix_ts_data * ts, int on);
#ifdef CONFIG_HAS_EARLYSUSPEND	
    struct early_suspend early_suspend;
#endif
};

const char *f3x_ts_name = "gt80x";
static struct workqueue_struct *goodix_wq;
#define X_DIFF (800)

#ifdef PRINT_POINT_INFO 
#define print_point_info(fmt, args...)   \
        do{                              \
                printk(fmt, ##args);     \
        }while(0)
#else
#define print_point_info(fmt, args...)   //
#endif

#ifdef PRINT_INT_INFO 
#define print_int_info(fmt, args...)     \
        do{                              \
                printk(fmt, ##args);     \
        }while(0)
#else
#define print_int_info(fmt, args...)   //
#endif

#ifdef PRINT_INITPANEL_INFO
#define print_initpanel_info(fmt, args...)     \
        do{                              \
                printk(fmt, ##args);     \
        }while(0)
#else
#define print_initpanel_info(fmt, args...)   //
#endif
///////////////////////////////////////////////
//specific tp related macro: need be configured for specific tp
#define CTP_IRQ_NO			(gpio_int_info[0].port_num)
#define CTP_IRQ_MODE			(POSITIVE_EDGE)
#define CTP_NAME			GOODIX_I2C_NAME
#define TS_RESET_LOW_PERIOD		(15)
#define TS_INITIAL_HIGH_PERIOD		(15)
#define TS_WAKEUP_LOW_PERIOD	(100)
#define TS_WAKEUP_HIGH_PERIOD	(100)
#define TS_POLL_DELAY			(10)	/* ms delay between samples */
#define TS_POLL_PERIOD			(10)	/* ms delay between samples */
#define SCREEN_MAX_HEIGHT		(screen_max_x)
#define SCREEN_MAX_WIDTH		(screen_max_y)
#define PRESS_MAX			(255)


#define READ_TOUCH_ADDR_H   0x0F
#define READ_TOUCH_ADDR_L   0x40

static void* __iomem gpio_addr = NULL;
static int gpio_int_hdle = 0;
static int gpio_wakeup_hdle = 0;
static int gpio_reset_hdle = 0;
static int gpio_wakeup_enable = 1;
static int gpio_reset_enable = 1;
static user_gpio_set_t  gpio_int_info[1];
static int aa = 0;
static int bb =0;

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
}u_i2c_addr = {{0x00},};

/*
 * ctp_get_pendown_state  : get the int_line data state, 
 * 
 * return value:
 *             return PRESS_DOWN: if down
 *             return FREE_UP: if up,
 *             return 0: do not need process, equal free up.
 */
static int ctp_get_pendown_state(void)
{
	unsigned int reg_val;
	static int state = FREE_UP;


	reg_val = readl(gpio_addr + PIOH_DATA);

	if(!(reg_val & (1<<CTP_IRQ_NO))){
		state = PRESS_DOWN;
		print_int_info("pen down. \n");
	}else{ //touch panel is free up
		state = FREE_UP;
		print_int_info("free up. \n");
	}
	return state;
}

/**
 * ctp_clear_penirq - clear int pending
 *
 */
static void ctp_clear_penirq(void)
{
	int reg_val;

	reg_val = readl(gpio_addr + PIO_INT_STAT_OFFSET);

	if((reg_val = (reg_val&(1<<(CTP_IRQ_NO))))){
		print_int_info("==CTP_IRQ_NO:%d=\n",CTP_IRQ_NO);              
		writel(reg_val,gpio_addr + PIO_INT_STAT_OFFSET);
	}
	return;
}

/**
 * ctp_set_irq_mode - according sysconfig's subkey "ctp_int_port" to config int port.
 * 
 * return value: 
 *              0:      success;
 *              others: fail; 
 */
static int ctp_set_irq_mode(char *major_key , char *subkey, ext_int_mode int_mode)
{
	int ret = 0;
	__u32 reg_num = 0;
	__u32 reg_addr = 0;
	__u32 reg_val = 0;
	//config gpio to int mode
	printk("%s: config gpio to int mode. \n", __func__);
#ifndef SYSCONFIG_GPIO_ENABLE
#else
	if(gpio_int_hdle){
		gpio_release(gpio_int_hdle, 2);
	}
	gpio_int_hdle = gpio_request_ex(major_key, subkey);
	if(!gpio_int_hdle){
		printk("request tp_int_port failed. \n");
		ret = -1;
		goto request_tp_int_port_failed;
	}
	gpio_get_one_pin_status(gpio_int_hdle, gpio_int_info, subkey, 1);
	pr_info("%s, %d: gpio_int_info, port = %d, port_num = %d. \n", __func__, __LINE__, \
		gpio_int_info[0].port, gpio_int_info[0].port_num);
#endif

#ifdef AW_GPIO_INT_API_ENABLE
#else
	pr_info(" INTERRUPT CONFIG\n");
	reg_num = (gpio_int_info[0].port_num)%8;
	reg_addr = (gpio_int_info[0].port_num)/8;
	reg_val = readl(gpio_addr + int_cfg_addr[reg_addr]);
	reg_val &= (~(7 << (reg_num * 4)));
	reg_val |= (int_mode << (reg_num * 4));
	writel(reg_val,gpio_addr+int_cfg_addr[reg_addr]);
                                                               
	ctp_clear_penirq();
                                                               
	reg_val = readl(gpio_addr+PIO_INT_CTRL_OFFSET); 
	reg_val |= (1 << (gpio_int_info[0].port_num));
	writel(reg_val,gpio_addr+PIO_INT_CTRL_OFFSET);

	udelay(1);
#endif

request_tp_int_port_failed:
	return ret;  
}

/**
 * ctp_set_gpio_mode - according sysconfig's subkey "ctp_io_port" to config io port.
 *
 * return value: 
 *              0:      success;
 *              others: fail; 
 */
static int ctp_set_gpio_mode(void)
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
	gpio_int_hdle = gpio_request_ex("ctp_para", "ctp_io_port");
	if(!gpio_int_hdle){
		printk("request ctp_io_port failed. \n");
		ret = -1;
		goto request_tp_io_port_failed;
	}
#endif
	return ret;

request_tp_io_port_failed:
	return ret;
}

/**
 * ctp_judge_int_occur - whether interrupt occur.
 *
 * return value: 
 *              0:      int occur;
 *              others: no int occur; 
 */
static int ctp_judge_int_occur(void)
{
	//int reg_val[3];
	int reg_val;
	int ret = -1;

	reg_val = readl(gpio_addr + PIO_INT_STAT_OFFSET);
	if(reg_val&(1<<(CTP_IRQ_NO))){
		ret = 0;
	}
	return ret; 	
}

/**
 * ctp_free_platform_resource - corresponding with ctp_init_platform_resource
 *
 */
static void ctp_free_platform_resource(void)
{
	printk("=======%s=========.\n", __func__);
	if(gpio_addr){
		iounmap(gpio_addr);
	}
	
	if(gpio_int_hdle){
		gpio_release(gpio_int_hdle, 2);
	}
	
	if(gpio_wakeup_hdle){
		gpio_release(gpio_wakeup_hdle, 2);
	}
	
	if(gpio_reset_hdle){
		gpio_release(gpio_reset_hdle, 2);
	}

	return;
}


/**
 * ctp_init_platform_resource - initialize platform related resource
 * return value: 0 : success
 *               -EIO :  i/o err.
 *
 */
static int ctp_init_platform_resource(void)
{
	int ret = 0;

	gpio_addr = ioremap(PIO_BASE_ADDRESS, PIO_RANGE_SIZE);
	//printk("%s, gpio_addr = 0x%x. \n", __func__, gpio_addr);
	if(!gpio_addr) {
		ret = -EIO;
		goto exit_ioremap_failed;	
	}
	//    gpio_wakeup_enable = 1;
	gpio_wakeup_hdle = gpio_request_ex("ctp_para", "ctp_wakeup");
	if(!gpio_wakeup_hdle) {
		pr_warning("%s: tp_wakeup request gpio fail!\n", __func__);
		gpio_wakeup_enable = 0;
	}

	gpio_reset_hdle = gpio_request_ex("ctp_para", "ctp_reset");
	if(!gpio_reset_hdle) {
		pr_warning("%s: tp_reset request gpio fail!\n", __func__);
		gpio_reset_enable = 0;
	}

	return ret;

exit_ioremap_failed:
	ctp_free_platform_resource();
	return ret;
}


/**
 * ctp_fetch_sysconfig_para - get config info from sysconfig.fex file.
 * return value:  
 *                    = 0; success;
 *                    < 0; err
 */
static int ctp_fetch_sysconfig_para(void)
{
	int ret = -1;
	int ctp_used = -1;
	char name[I2C_NAME_SIZE];
	script_parser_value_type_t type = SCIRPT_PARSER_VALUE_TYPE_STRING;

	printk("%s. \n", __func__);

	if(SCRIPT_PARSER_OK != script_parser_fetch("ctp_para", "ctp_used", &ctp_used, 1)){
		pr_err("%s: script_parser_fetch err. \n", __func__);
		goto script_parser_fetch_err;
	}
	if(1 != ctp_used){
		pr_err("%s: ctp_unused. \n",  __func__);
		//ret = 1;
		return ret;
	}

	if(SCRIPT_PARSER_OK != script_parser_fetch_ex("ctp_para", "ctp_name", (int *)(&name), &type, sizeof(name)/sizeof(int))){
		pr_err("%s: script_parser_fetch err. \n", __func__);
		goto script_parser_fetch_err;
	}
	if(strcmp(CTP_NAME, name)){
		pr_err("%s: name %s does not match CTP_NAME. \n", __func__, name);
		pr_err(CTP_NAME);
		//ret = 1;
		return ret;
	}

	if(SCRIPT_PARSER_OK != script_parser_fetch("ctp_para", "ctp_twi_addr", &twi_addr, sizeof(twi_addr)/sizeof(__u32))){
		pr_err("%s: script_parser_fetch err. \n", name);
		goto script_parser_fetch_err;
	}
	//big-endian or small-endian?
	//printk("%s: before: ctp_twi_addr is 0x%x, dirty_addr_buf: 0x%hx. dirty_addr_buf[1]: 0x%hx \n", __func__, twi_addr, u_i2c_addr.dirty_addr_buf[0], u_i2c_addr.dirty_addr_buf[1]);
	u_i2c_addr.dirty_addr_buf[0] = twi_addr;
	u_i2c_addr.dirty_addr_buf[1] = I2C_CLIENT_END;
	printk("%s: after: ctp_twi_addr is 0x%x, dirty_addr_buf: 0x%hx. dirty_addr_buf[1]: 0x%hx \n", __func__, twi_addr, u_i2c_addr.dirty_addr_buf[0], u_i2c_addr.dirty_addr_buf[1]);
	//printk("%s: after: ctp_twi_addr is 0x%x, u32_dirty_addr_buf: 0x%hx. u32_dirty_addr_buf[1]: 0x%hx \n", __func__, twi_addr, u32_dirty_addr_buf[0],u32_dirty_addr_buf[1]);

	if(SCRIPT_PARSER_OK != script_parser_fetch("ctp_para", "ctp_twi_id", &twi_id, sizeof(twi_id)/sizeof(__u32))){
		pr_err("%s: script_parser_fetch err. \n", name);
		goto script_parser_fetch_err;
	}
	printk("%s: ctp_twi_id is %d. \n", __func__, twi_id);
	
	if(SCRIPT_PARSER_OK != script_parser_fetch("ctp_para", "ctp_screen_max_x", &screen_max_x, 1)){
		pr_err("%s: script_parser_fetch err. \n", __func__);
		goto script_parser_fetch_err;
	}
	pr_info("%s: screen_max_x = %d. \n", __func__, screen_max_x);

	if(SCRIPT_PARSER_OK != script_parser_fetch("ctp_para", "ctp_screen_max_y", &screen_max_y, 1)){
		pr_err("%s: script_parser_fetch err. \n", __func__);
		goto script_parser_fetch_err;
	}
	pr_info("%s: screen_max_y = %d. \n", __func__, screen_max_y);

	if(SCRIPT_PARSER_OK != script_parser_fetch("ctp_para", "ctp_revert_x_flag", &revert_x_flag, 1)){
		pr_err("%s: script_parser_fetch err. \n", __func__);
		goto script_parser_fetch_err;
	}
	pr_info("%s: revert_x_flag = %d. \n", __func__, revert_x_flag);

	if(SCRIPT_PARSER_OK != script_parser_fetch("ctp_para", "ctp_revert_y_flag", &revert_y_flag, 1)){
		pr_err("%s: script_parser_fetch err. \n", __func__);
		goto script_parser_fetch_err;
	}
	pr_info("%s: revert_y_flag = %d. \n", __func__, revert_y_flag);

	if(SCRIPT_PARSER_OK != script_parser_fetch("ctp_para", "ctp_exchange_x_y_flag", &exchange_x_y_flag, 1)){
		pr_err("ft5x_ts: script_parser_fetch err. \n");
		goto script_parser_fetch_err;
	}
	pr_info("%s: exchange_x_y_flag = %d. \n", __func__, exchange_x_y_flag);

	return 0;

script_parser_fetch_err:
	pr_notice("=========script_parser_fetch_err============\n");
	return ret;
}

/**
 * ctp_reset - function
 *
 */
static void ctp_reset(void)
{
	printk("%s. \n", __func__);
	if(gpio_reset_enable){
		if(EGPIO_SUCCESS != gpio_write_one_pin_value(gpio_reset_hdle, 0, "ctp_reset")){
			printk("%s: err when operate gpio. \n", __func__);
		}
		mdelay(TS_RESET_LOW_PERIOD);
		if(EGPIO_SUCCESS != gpio_write_one_pin_value(gpio_reset_hdle, 1, "ctp_reset")){
			printk("%s: err when operate gpio. \n", __func__);
		}
		mdelay(TS_INITIAL_HIGH_PERIOD);
	}
}

/**
 * ctp_wakeup - function
 *
 */
static void ctp_wakeup(void)
{
	printk("%s. \n", __func__);
	if(1 == gpio_wakeup_enable){  
		if(EGPIO_SUCCESS != gpio_write_one_pin_value(gpio_wakeup_hdle, 0, "ctp_wakeup")){
			printk("%s: err when operate gpio. \n", __func__);
		}
		mdelay(TS_WAKEUP_LOW_PERIOD);
		if(EGPIO_SUCCESS != gpio_write_one_pin_value(gpio_wakeup_hdle, 1, "ctp_wakeup")){
			printk("%s: err when operate gpio. \n", __func__);
		}
		mdelay(TS_WAKEUP_HIGH_PERIOD);

	}
	return;
}
/**
 * ctp_detect - Device detection callback for automatic device creation
 * return value:  
 *                    = 0; success;
 *                    < 0; err
 */
int ctp_detect(struct i2c_client *client, struct i2c_board_info *info)
{
	struct i2c_adapter *adapter = client->adapter;

	if(twi_id == adapter->nr)
	{
		pr_info("%s: Detected chip %s at adapter %d, address 0x%02x\n",
			 __func__, CTP_NAME, i2c_adapter_id(adapter), client->addr);

		strlcpy(info->type, CTP_NAME, I2C_NAME_SIZE);
		return 0;
	}else{
		return -ENODEV;
	}
}
////////////////////////////////////////////////////////////////

static struct ctp_platform_ops ctp_ops = {
	.get_pendown_state = ctp_get_pendown_state,
	.clear_penirq	   = ctp_clear_penirq,
	.set_irq_mode      = ctp_set_irq_mode,
	.set_gpio_mode     = ctp_set_gpio_mode,	
	.judge_int_occur   = ctp_judge_int_occur,
	.init_platform_resource = ctp_init_platform_resource,
	.free_platform_resource = ctp_free_platform_resource,
	.fetch_sysconfig_para = ctp_fetch_sysconfig_para,
	.ts_reset =          ctp_reset,
	.ts_wakeup =         ctp_wakeup,
	.ts_detect = ctp_detect,
};


//ͣ���豸
#ifdef CONFIG_HAS_EARLYSUSPEND
static void goodix_ts_suspend(struct early_suspend *h)
{
    int ret;
    struct goodix_ts_data *ts = container_of(h, struct goodix_ts_data, early_suspend);
    struct i2c_client * client = ts->client;
#ifdef PRINT_SUSPEND_INFO
    printk("enter earlysuspend: goodix_ts_suspend. \n");
#endif    
        
    if (g_suspend_state != PM_SUSPEND_PARTIAL)
    {
        disable_irq(ts->gpio_irq);
        ret = cancel_work_sync(&ts->work);  
            
        if (ts->power) {
            ret = ts->power(ts,0);
            if (ret < 0)
                dev_dbg(&client->dev, "%s power off failed\n", f3x_ts_name);
        }
    }
    else
    {
        standby_wakeup_event |= SUSPEND_WAKEUP_SRC_PIO;
    }
    return ;
}

//���»���
static void goodix_ts_resume(struct early_suspend *h)
{
	int ret;
	struct goodix_ts_data *ts = container_of(h, struct goodix_ts_data, early_suspend);
    struct i2c_client * client = ts->client;
    
#ifdef PRINT_SUSPEND_INFO
    printk("enter laterresume: goodix_ts_resume. \n");
#endif 

    if (g_suspend_state != PM_SUSPEND_PARTIAL)
    {
        if (ts->power) {
            ret = ts->power(ts, 1);
            if (ret < 0)
                dev_dbg(&client->dev, "%s power on failed\n", f3x_ts_name);
        }
        
        enable_irq(ts->gpio_irq);
    }
	return ;
}
#else
#ifdef CONFIG_PM
//ͣ���豸
static int goodix_ts_suspend(struct i2c_client *client, pm_message_t mesg)
{
	int ret;
	struct goodix_ts_data *ts = i2c_get_clientdata(client);

#ifdef PRINT_SUSPEND_INFO
	printk("enter: goodix_ts_suspend. msg=%i\n",mesg.event);
#endif         
	//disable_irq(ts->gpio_irq);
	ret = cancel_work_sync(&ts->work);	
	//set wakeup event type
	standby_wakeup_event |= SUSPEND_WAKEUP_SRC_PIO;
	//don't power off touch
//	if (ts->power) {
//		ret = ts->power(ts,0);
//		if (ret < 0)
//			dev_dbg(&client->dev, "%s power off failed\n", f3x_ts_name);
//	}
	return 0;
}

//���»���
static int goodix_ts_resume(struct i2c_client *client)
{
	int ret;
	struct goodix_ts_data *ts = i2c_get_clientdata(client);

#ifdef PRINT_SUSPEND_INFO
	printk("enter: goodix_ts_resume. \n");
#endif
	//tp already power on

//	if (ts->power) {
//		ret = ts->power(ts, 1);
//		if (ret < 0)
//			dev_dbg(&client->dev, "%s power on failed\n", f3x_ts_name);
//	}

	//enable_irq(ts->gpio_irq);
	return 0;
}
#endif

#endif


/*used by GT80X-IAP module */
struct i2c_client * i2c_connect_client = NULL;
EXPORT_SYMBOL(i2c_connect_client);
/**********************************************************************	
��������I2Cͨ�ŷ�ʽΪ��
	7bit�ӻ��ַ���дλ + buf����ݵ�ַ+��д��ݣ�
	 --------------------------------------------------------------------
	��  �ӻ��ַ   �� buf[0](��ݵ�ַ) | buf[1]~buf[MAX-1](д����ȡ�������)  |
	 --------------------------------------------------------------------
	��ֲǰ�����������ظ�ʽ�޸ģ���
***********************************************************************/

//Function as i2c_master_receive, and return 2 if operation is successful.
static int i2c_read_bytes(struct i2c_client *client, uint8_t *buf, uint16_t len)
{
	struct i2c_msg msgs[2];
	int ret=-1;
	//����д��ַ
	msgs[0].flags = !I2C_M_RD;
	msgs[0].addr = client->addr;
	msgs[0].len = 2;		//data address
	msgs[0].buf = buf;
	//�������
	msgs[1].flags = I2C_M_RD;//����Ϣ
	msgs[1].addr = client->addr;
	msgs[1].len = len-2;
	msgs[1].buf = buf+2;
	
	ret=i2c_transfer(client->adapter, msgs, 2);
	return ret;
}

//Function as i2c_master_send, and return 1 if operation is successful. 
static int i2c_write_bytes(struct i2c_client *client, uint8_t *data, uint16_t len)
{
	struct i2c_msg msg;
	int ret=-1;
	
	msg.flags = !I2C_M_RD;//д��Ϣ
	msg.addr = client->addr;
	msg.len = len;
	msg.buf = data;		
	
	ret=i2c_transfer(client->adapter, &msg,1);
	return ret;
}

/*******************************************************
���ܣ�
	����ǰ׺����
	
	ts:	client˽����ݽṹ��
return��
    �ɹ�����1
*******************************************************/
static s32 i2c_pre_cmd(struct goodix_ts_data *ts)
{
    s32 ret;
    u8 pre_cmd_data[2]={0x0f, 0xff};

    ret=i2c_write_bytes(ts->client,pre_cmd_data,2);
    return ret;//*/
}

/*******************************************************
���ܣ�
	���ͺ�׺����
	
	ts:	client˽����ݽṹ��
return��
    �ɹ�����1
*******************************************************/
static s32 i2c_end_cmd(struct goodix_ts_data *ts)
{
    s32 ret;
    u8 end_cmd_data[2]={0x80, 0x00};    

    ret=i2c_write_bytes(ts->client,end_cmd_data,2);
    return ret;//*/
}

/*******************************************************
���ܣ�
	GT82X��ʼ���������ڷ���������Ϣ
����
	ts:	struct goodix_ts_data
return��
	ִ�н���룬0��ʾ��ִ��
*******************************************************/

static int goodix_init_panel(struct goodix_ts_data *ts)
{
	int ret=-1;
	int i = 0;
    uint8_t config_info1[114];
    uint8_t config_info[114];
    print_initpanel_info("init panel\n");
	 
	if((800 == screen_max_x) && (600 == screen_max_y))	{			//828
	uint8_t data_info[] = {/*
                            0x0F,0x80,
                            0x1D,0x0E,0x1C,0x0D,0x1B,0x0C,0x1A,0x0B,
                            0x19,0x0A,0x18,0x09,0x17,0x08,0x16,0x07,
                            0x15,0x06,0x14,0x05,0x13,0x04,0x12,0x03,
                            0x11,0x02,0x10,0x01,0x0F,0x00,0x13,0x09,
                            0x12,0x08,0x11,0x07,0x10,0x06,0x0F,0x05,
                            0x0E,0x04,0x0D,0x03,0x0C,0x02,0x0B,0x01,
                            0x0A,0x00,0x0F,0x03,0x88,0x88,0x88,0x28,
                            0x00,0x00,0x07,0x00,0x00,0x0E,0x44,0x35,
                            0x59,0x03,0x00,0x05,0x00,0x02,0x58,0x04,
                            0x00,0x51,0x44,0x4C,0x40,0x26,0x00,0x03,
                            0x14,0x05,0x14,0x10,0x00,0x01,0x00,0x00,
                            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01
                            */
                0x0F,0x80,            
							 	0x02,0x11,0x03,0x12,0x04,0x13,0x05,0x14,
							 	0x06,0x15,0x07,0x16,0x08,0x17,0x09,0xFF,
							 	0xFF,0xFF,0xFF,0xFF,0xFF,0x14,0x15,0x16,
							 	0x17,0x18,0x19,0x1A,0x1B,0x1C,0x11,0x07,
							 	0x10,0x06,0x0F,0x05,0x0E,0x04,0x0D,0x03,
							 	0xFF,0xFF,0xFF,0xFF,0xFF,0x0E,0x0F,0x10,
							 	0x11,0x12,0x0F,0x03,0x00,0x10,0x10,0x1C,
							 	0x00,0x00,0x03,0x00,0x00,0x02,0x45,0x36,
							 	0x14,0x03,0x00,0x05,0x00,0x02,0x58,0x03,
							 	0x20,0x34,0x37,0x37,0x39,0x29,0x00,0x26,
							 	0x19,0x02,0x16,0x14,0x00,0x00,0x00,0x00,
							 	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
							 	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
							 	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01                            
                            };
             for(i =0; i < 114; i++){
            config_info[i] = data_info[i];
        }
	}
	
	ret=i2c_write_bytes(ts->client,config_info, 114);
	i2c_end_cmd(ts);
    msleep(10);
	print_initpanel_info("init panel ret = %d\n",ret);
	if (ret < 0) 
		return ret;
	msleep(100);
	
	#ifdef PRINT_INITPANEL_INFO
	config_info1[0] = 0x0F;
	config_info1[1] = 0x80;
	ret=i2c_read_bytes(ts->client,config_info1,114);	
	for( i = 0;i<114;i++)
	{
	    print_initpanel_info("i = %d config_info1[i] = %x \n",i,config_info1[i]);
	    
	    }
	msleep(10);
	#endif
	
	return 1;

} 

/*��ȡGT80X�İ汾�Ų���ӡ*/
//static int  goodix_read_version(struct goodix_ts_data *ts)
//{
//#define GT80X_VERSION_LENGTH	40	
//	int ret;
//	uint8_t version[2] = {0x69,0xff};			//command of reading Guitar's version 
//	uint8_t version_data[GT80X_VERSION_LENGTH+1]={0x6A};	//store touchscreen version infomation
//	memset(version_data+1, 0, GT80X_VERSION_LENGTH);
//	ret = i2c_write_bytes(ts->client,version,2);
//	if (ret != 1) 
//		return ret;
//	msleep(50);						
//	ret = i2c_read_bytes(ts->client,version_data, GT80X_VERSION_LENGTH);
//	if (ret != 2) 
//		strncpy(version_data+1, "NULL", 4);
//	dev_info(&ts->client->dev,"GT80X Version: %s\n", version_data+1);
//	version[1] = 0x00;				//cancel the command
//	i2c_write_bytes(ts->client, version, 2);
//	return 0;	
//}
static s32 goodix_ts_version(struct goodix_ts_data *ts)

{

    u8 buf[8];

 

    buf[0] = 0x0f;

    buf[1] = 0x7d;

    

    i2c_read_bytes(ts->client, buf, 5);

    i2c_end_cmd(ts);

 

    printk("PID:%02x, VID:%02x%02x\n", buf[2], buf[3], buf[4]);

 

    return 1;

}

static s32 touch_num(u8 value, s32 max)
{
    s32 tmp = 0;

    while((tmp < max) && value)
    {
        if ((value & 0x01) == 1)
        {
            tmp++;
        }
        value = value >> 1;
    }

    return tmp;
}

/*******************************************************	
���ܣ�
	��������������
	���жϴ���������1�������ݣ�У����ٷ������
����
	ts:	client˽����ݽṹ��
return��
    void
********************************************************/
static void goodix_ts_work_func(struct work_struct *work)
{
    u8 finger = 0;
    u8 chk_sum = 0;
    u8 key = 0;
    static u8 last_key = 0;
    u16 X_value;
    u16 Y_value;
    u32 count = 0;
    u32 position = 0;
    s32 ret = -1;
    s32 tmp = 0;
    s32 i = 0;
    s32 j = 0;
    u8 *coor_point;
    u8 tmp_data[2 + 2 + 5*MAX_FINGER_NUM + 1] = {READ_TOUCH_ADDR_H,READ_TOUCH_ADDR_L,0, 0};
    u8 *touch_data = NULL;
    static u8 finger_last[MAX_FINGER_NUM+1]={0};        //�ϴδ����������ָ����
    u8 finger_current[MAX_FINGER_NUM+1] = {0};        //��ǰ�����������ָ����
    struct goodix_ts_data *ts = container_of(work, struct goodix_ts_data, work);
    
#ifndef INT_PORT
COORDINATE_POLL:
#endif
    if( tmp > 9)
    {
        dev_info(&(ts->client->dev), "Because of transfer error,touchscreen stop working.\n");
        goto XFER_ERROR ;
    }

//jim add
#if defined(CONFIG_HAS_EARLYSUSPEND)
    if (standby_output.pio_data_cnt != 0)
    {
        touch_data = standby_output.pio_buffer;
    } 
    standby_output.pio_data_cnt = 0;
#endif
	//disable_irq(ts->gpio_irq);
    if (touch_data == NULL)
    {
        touch_data = tmp_data;
        //���齫���һ���Զ�ȡ��
        ret=i2c_read_bytes(ts->client, tmp_data,sizeof(tmp_data)/sizeof(tmp_data[0])); 
        i2c_end_cmd(ts);
        if(ret <= 0) 
        {
            dev_err(&(ts->client->dev),"line:%d,I2C transfer error. Number:%d\n ",__LINE__, ret);
            ts->bad_data = 1;
            tmp ++;
        }
        
        if(ts->bad_data)
        {
            //TODO:Is sending config once again (to reset the chip) useful?    
            ts->bad_data = 0;
            msleep(20);
        }
    }

//    if((touch_data[2]&0xC0)!=0x80)
//    {
//        printk("COORDINATE_POLL!\n");
//        goto COORDINATE_POLL;  
//        j++;
//        if(j == 5) {
//            j = 0;
//        goto DATA_NO_READY;
//        }     
//        
//    }

    key = touch_data[3]&0x0f; // 1, 2, 4, 8
    if (key == 0x0f)
    {
        if (goodix_init_panel(ts))
        {
/**/        print_point_info("Reload config failed!\n");
            goto XFER_ERROR;
        }
        else
        {   
            print_point_info("Reload config successfully!\n");
        }
        
    }

    finger = (u8)touch_num(touch_data[2]&0x1f, MAX_FINGER_NUM);

/**/print_point_info("touch num:%x\n", finger);

    for (i = 1;i < MAX_FINGER_NUM + 1; i++)        
    {
        finger_current[i] = !!(touch_data[2] & (0x01<<(i-1)));
    }

#ifndef DEBUG_COORD
/**/for (i = 0; i < (2 + 2 + 5*MAX_FINGER_NUM + 1); i++)
/**/{  
/**/    print_point_info("%5x", touch_data[i]);
/**/}
/**/print_point_info("\n");
#endif 

    //����У���    
    coor_point = &touch_data[4];
    chk_sum = 0;
    for ( i = 0; i < 5*finger; i++)
    {
        chk_sum += coor_point[i];
/**/    print_point_info("%5x", coor_point[i]);
    }
/**/print_point_info("\ncheck sum:%x\n", chk_sum);
/**/print_point_info("check sum byte:%x\n", coor_point[5*finger]);
    if (chk_sum != coor_point[5*finger])
    {
        goto XFER_ERROR;
    }

    //�������//
    if (finger)
    {
        for(i = 0, position=1;position < MAX_FINGER_NUM+1; position++)
        {
            if(finger_current[position])
            {     
                X_value = coor_point[i] << 8;
                X_value = X_value | coor_point[i + 1];

                Y_value = coor_point[i + 2] << 8;
                Y_value = Y_value | coor_point[i + 3];
                
               if(1 == revert_x_flag){
			     X_value= SCREEN_MAX_WIDTH - X_value;
			      }
		       if(1 == revert_y_flag){
			     Y_value = SCREEN_MAX_HEIGHT - Y_value;
			     }
			   if(1 == exchange_x_y_flag) {
			    
			      swap(X_value, Y_value);
			    }
		        
                input_report_key(ts->input_dev, BTN_TOUCH, 1);
                input_report_abs(ts->input_dev, ABS_MT_TRACKING_ID, position - 1);
                input_report_abs(ts->input_dev, ABS_MT_POSITION_X,Y_value);  
                input_report_abs(ts->input_dev, ABS_MT_POSITION_Y,X_value);
                input_report_abs(ts->input_dev, ABS_MT_TOUCH_MAJOR,15);
                input_mt_sync(ts->input_dev);
                i += 5;

    /*        printk("X:%d\n", (s32)X_value);*/
    /*        printk("Y:%d\n", (s32)Y_value);*/
            }
        }
    }
    else
    {
        input_report_key(ts->input_dev, BTN_TOUCH, 0);
        input_report_abs(ts->input_dev, ABS_MT_TOUCH_MAJOR, 0);
        input_mt_sync(ts->input_dev);
    }

#ifdef HAVE_TOUCH_KEY
#ifdef DEBUG_COORD
/**/for (i = 0; i < 4; i++)
/**/{
/**/    print_point_info("key:%4x   ", !!(key&(0x01<<i)));
/**/}
/**/print_point_info("\n");
#endif

    if((last_key != 0) || (key != 0))
    {
        for(count = 0; count < 4; count++)
        {
            input_report_key(ts->input_dev, touch_key_array[count], !!(key&(0x01<<count)));    
        }
    }
    last_key = key;
#endif

    input_sync(ts->input_dev);
    
    for(position=1;position<MAX_FINGER_NUM+1; position++)
    {
        finger_last[position] = finger_current[position];
    }

//DATA_NO_READY:
//    printk("DATA_NO_READY!\n");
XFER_ERROR:
    print_point_info("XFER_ERROR!\n");
//     aa++;
//   print_point_info("aa = %d\n",aa);
    //enable_irq(ts->gpio_irq);
}


/*******************************************************	
���ܣ�
	�ж���Ӧ����
	���жϴ��������ȴ��������?������
********************************************************/
static irqreturn_t goodix_ts_irq_handler(int irq, void *dev_id)
{
	struct goodix_ts_data *ts = dev_id;	
	int reg_val;	
	print_int_info("==========------TS Interrupt-----============\n"); 

	//clear the IRQ_EINT21 interrupt pending
	reg_val = readl(gpio_addr + PIO_INT_STAT_OFFSET);

	if(reg_val&(1<<(CTP_IRQ_NO)))
	{	
		print_int_info("==IRQ_EINT%d=\n",CTP_IRQ_NO);
		writel(reg_val&(1<<(CTP_IRQ_NO)),gpio_addr + PIO_INT_STAT_OFFSET);
		queue_work(goodix_wq, &ts->work);
//		bb++;
//	   print_point_info("bb = %d\n",bb);
	}
	else
	{
	    print_int_info("Other Interrupt\n");
	    return IRQ_NONE;
	}
	return IRQ_HANDLED;
}

/*******************************************************	
���ܣ�
	GT80X�ĵ�Դ����
����
	on:����GT80X����ģʽ��0Ϊ����Sleepģʽ
return��
	�Ƿ����óɹ���С��0��ʾ����ʧ��
********************************************************/
//static int goodix_ts_power(struct goodix_ts_data * ts, int on)
//{
//	int ret = 0;
//	
//	switch(on) 
//	{
//		case 0:
//			gpio_write_one_pin_value(gpio_wakeup_hdle, 1, "ctp_wakeup");
//			ret = 1;
//			break;
//		case 1:
//			gpio_write_one_pin_value(gpio_wakeup_hdle, 0, "ctp_wakeup");
//			ret = 1;
//			break;	
//	}
//	dev_dbg(&ts->client->dev, "Set Guitar's Shutdown %s. Ret:%d.\n", on?"LOW":"HIGH", ret);
//	return ret;
//}
static int goodix_ts_power(struct goodix_ts_data * ts, int on)
{
	  s32 ret = -1;
       s32 success = 1;
    u8 i2c_control_buf1[3] = {0x0F,0xF2,0xc0};        //suspend cmd
    u8 i2c_control_buf2[3] = {0x0F,0xF2,0x00};

    switch(on)
    {
    case 0:
        ret = i2c_write_bytes(ts->client, i2c_control_buf1, 3);
        i2c_end_cmd(ts);
        return ret;

    case 1:
//        GPIO_DIRECTION_OUTPUT(INT_PORT, 0);
//        GPIO_SET_VALUE(INT_PORT, 0);
//        msleep(1);
//        GPIO_SET_VALUE(INT_PORT, 1);
//        msleep(1);
//        GPIO_DIRECTION_INPUT(INT_PORT);
//        GPIO_PULL_UPDOWN(INT_PORT, 0);
//
    	  gpio_set_one_pin_io_status(gpio_int_hdle, 1, "ctp_int_port");
    	  gpio_write_one_pin_value(gpio_int_hdle, 0, "ctp_int_port");
    	  msleep(100);
    	  gpio_set_one_pin_io_status(gpio_int_hdle, 1, "ctp_int_port");
		  gpio_write_one_pin_value(gpio_int_hdle, 1, "ctp_int_port");
		  msleep(100);
		  gpio_set_one_pin_io_status(gpio_int_hdle, 1, "ctp_int_port");
		  gpio_write_one_pin_value(gpio_int_hdle, 0, "ctp_int_port"); 
		  
//		  gpio_set_one_pin_io_status(gpio_int_hdle, 0, "ctp_int_port");
		  gpio_set_one_pin_pull(gpio_int_hdle, 0, "ctp_int_port");
		  
		  ret = ctp_ops.set_irq_mode("ctp_para", "ctp_int_port", CTP_IRQ_MODE);
			if(0 != ret){
				printk("%s:ctp_ops.set_irq_mode err. \n", __func__);
				return ret;
			}

//        ret = i2c_write_bytes(ts->client, i2c_control_buf2, 3);
//        //i2c_end_cmd(ts);
//         msleep(10);
          return ret;

         default:
          printk(KERN_DEBUG "%s: Cant't support this command.",f3x_ts_name );
          return -EINVAL;
        
    } 
	
}

//Test i2c to check device. Before it SHUTDOWN port Must be low state 30ms or more.
static bool goodix_i2c_test(struct i2c_client * client)
{
	int ret, retry;
	uint8_t test_data[1] = { 0 };	//only write a data address.
	
	for(retry=0; retry < 5; retry++)
	{
		ret =i2c_write_bytes(client, test_data, 1);	//Test i2c.
		if (ret == 1)
			break;
		msleep(5);
	}
	
	return ret==1 ? true : false;
}

/*******************************************************	
���ܣ�
	������̽�⺯��
	��ע����ʱ���ã�Ҫ����ڶ�Ӧ��client����
	����IO,�жϵ���Դ���룻�豸ע�᣻��������ʼ���ȹ���
����
	client?�������豸�ṹ�?
	id���豸ID
return��
	ִ�н���룬0��ʾ��ִ��
********************************************************/
static int goodix_ts_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct goodix_ts_data *ts;
	int ret = 0;
	int err;
    aa = 0;
    bb = 0;
	//struct goodix_i2c_platform_data *pdata;
	//dev_dbg(&client->dev,"Install touchscreen driver for guitar.\n");
	pr_info("===============================GT801 Probe===========================\n");
    pr_info("==========================================================\n");
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)){
		dev_err(&client->dev, "System need I2C function.\n");
		ret = -ENODEV;
		goto err_check_functionality_failed;
	}
	
	ts = kzalloc(sizeof(*ts), GFP_KERNEL);
	if (ts == NULL) {
		ret = -ENOMEM;
		goto err_alloc_data_failed;
	}
		
	ts->gpio_irq = SW_INT_IRQNO_PIO;
	i2c_connect_client = client;				//used by Guitar Updating.

#ifdef TEST_I2C_TRANSFER
	//TODO: used to set speed of i2c transfer. Should be change as your paltform.
	pr_info("Begin goodix i2c test\n");
	ret = goodix_i2c_test(client);
	if(!ret){
		pr_info("Warnning: I2C connection might be something wrong!\n");
		goto err_i2c_failed;
	}
	pr_info("===== goodix i2c test ok=======\n");
#endif
	
	INIT_WORK(&ts->work, goodix_ts_work_func);
	ts->client = client;
	i2c_set_clientdata(client, ts);
	
	ts->input_dev = input_allocate_device();
	if (ts->input_dev == NULL) 
	{
		ret = -ENOMEM;
		dev_dbg(&client->dev,"Failed to allocate input device\n");
		goto err_input_dev_alloc_failed;
	}

	ts->input_dev->evbit[0] = BIT_MASK(EV_SYN) | BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS) ;
	
#ifndef GOODIX_MULTI_TOUCH	
	ts->input_dev->keybit[BIT_WORD(BTN_TOUCH)] = BIT_MASK(BTN_TOUCH);
	ts->input_dev->absbit[0] = BIT(ABS_X) | BIT(ABS_Y) | BIT(ABS_PRESSURE);
	input_set_abs_params(ts->input_dev, ABS_X, 0, SCREEN_MAX_HEIGHT, 0, 0);
	input_set_abs_params(ts->input_dev, ABS_Y, 0, SCREEN_MAX_WIDTH, 0, 0);
	input_set_abs_params(ts->input_dev, ABS_PRESSURE, 0, 255, 0, 0);	
	
#else
	ts->input_dev->absbit[0] = BIT_MASK(ABS_MT_TRACKING_ID) |
		BIT_MASK(ABS_MT_TOUCH_MAJOR)| BIT_MASK(ABS_MT_WIDTH_MAJOR) |
  		BIT_MASK(ABS_MT_POSITION_X) | BIT_MASK(ABS_MT_POSITION_Y); 	// for android
	input_set_abs_params(ts->input_dev, ABS_MT_WIDTH_MAJOR, 0, 255, 0, 0);
	input_set_abs_params(ts->input_dev, ABS_MT_TOUCH_MAJOR, 0, 255, 0, 0);
	input_set_abs_params(ts->input_dev, ABS_MT_POSITION_X, 0, SCREEN_MAX_HEIGHT, 0, 0);
	input_set_abs_params(ts->input_dev, ABS_MT_POSITION_Y, 0, SCREEN_MAX_WIDTH, 0, 0);	
	input_set_abs_params(ts->input_dev, ABS_MT_TRACKING_ID, 0, MAX_FINGER_NUM, 0, 0);	
#endif	

#ifdef FOR_TSLIB_TEST
	set_bit(BTN_TOUCH, ts->input_dev->keybit);
#endif

	sprintf(ts->phys, "input/goodix-ts");
	ts->input_dev->name = f3x_ts_name;
	ts->input_dev->phys = ts->phys;
	ts->input_dev->id.bustype = BUS_I2C;
	ts->input_dev->id.vendor = 0xDEAD;
	ts->input_dev->id.product = 0xBEEF;
	ts->input_dev->id.version = 0x1105;	

	ret = input_register_device(ts->input_dev);
	if (ret) {
		dev_err(&client->dev,"Unable to register %s input device\n", ts->input_dev->name);
		goto err_input_register_device_failed;
	}
	
	goodix_wq = create_singlethread_workqueue("goodix_wq");
	if (!goodix_wq) {
		printk(KERN_ALERT "Creat %s workqueue failed.\n", f3x_ts_name);
		return -ENOMEM;
		
	}
	flush_workqueue(goodix_wq);	
	ts->power = goodix_ts_power;
	gpio_write_one_pin_value(gpio_wakeup_hdle, 0, "ctp_wakeup");
    msleep(100);
    gpio_write_one_pin_value(gpio_wakeup_hdle, 1, "ctp_wakeup");
    msleep(100);
		

	ret = goodix_init_panel(ts);
	if(!ret) {
		goto err_init_godix_ts;
	}else {
	   printk("init panel succeed!\n");	
    }
#ifdef CONFIG_HAS_EARLYSUSPEND	
	printk("==register_early_suspend =\n");	
	ts->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;	
	ts->early_suspend.suspend = goodix_ts_suspend;
	ts->early_suspend.resume	= goodix_ts_resume;	
	register_early_suspend(&ts->early_suspend);
#endif

	err = ctp_ops.set_irq_mode("ctp_para", "ctp_int_port",CTP_IRQ_MODE);
	if(0 != err){
		printk("%s:ctp_ops.set_irq_mode err. \n", __func__);
		goto exit_set_irq_mode;
	}
	
	err =  request_irq(SW_INT_IRQNO_PIO, goodix_ts_irq_handler, IRQF_TRIGGER_RISING | IRQF_SHARED, client->name, ts);
	if (err < 0) {
		pr_info( "goodix_probe: request irq failed\n");
		goto exit_irq_request_failed;
	}

	pr_info("Read Goodix version\n");
	goodix_ts_version(ts);
	//msleep(260);	

	dev_dbg(&client->dev,"Start  %s in %s mode\n", 
		ts->input_dev->name, ts->use_irq ? "Interrupt" : "Polling");
		
	pr_info("========Probe Ok================\n");
	return 0;

exit_set_irq_mode:
	printk("fail1\n");	
exit_irq_request_failed:
	printk("fail2\n");	
err_init_godix_ts:
	printk("fail3\n");	
err_input_register_device_failed:
	printk("fail4\n");	
	input_free_device(ts->input_dev);
err_input_dev_alloc_failed:
	printk("fail5\n");	
	i2c_set_clientdata(client, NULL);
err_i2c_failed:
	printk("fail6\n");	
err_alloc_data_failed:
	printk("fail7\n");	
err_check_functionality_failed:
	return ret;
}


/*******************************************************	
���ܣ�
	����Դ�ͷ�
����
	client���豸�ṹ��
return��
	ִ�н���룬0��ʾ��ִ��
********************************************************/
static int goodix_ts_remove(struct i2c_client *client)
{
	struct goodix_ts_data *ts = i2c_get_clientdata(client);
	dev_notice(&client->dev,"The driver is removing...\n");
	
	free_irq(SW_INT_IRQNO_PIO, ts);
	#ifdef CONFIG_HAS_EARLYSUSPEND	
		unregister_early_suspend(&ts->early_suspend);
	#endif
	flush_workqueue(goodix_wq);
	if (goodix_wq)
		destroy_workqueue(goodix_wq);
	input_unregister_device(ts->input_dev);
	input_free_device(ts->input_dev);
	i2c_set_clientdata(ts->client, NULL);
	kfree(ts);
	return 0;
}

//�����ڸ���� �豸���豸ID �б�
//only one client
static const struct i2c_device_id goodix_ts_id[] = {
	{ GOODIX_I2C_NAME, 0 },
	{ }
};

//�豸��ṹ��
static struct i2c_driver goodix_ts_driver = {
	.class = I2C_CLASS_HWMON,
	.probe		= goodix_ts_probe,
	.remove		= goodix_ts_remove,
#ifdef CONFIG_HAS_EARLYSUSPEND
#else
#ifdef CONFIG_PM
	.suspend	= goodix_ts_suspend,
	.resume		= goodix_ts_resume,
#endif
#endif
	.id_table	= goodix_ts_id,
	.driver = {
		.name	= GOODIX_I2C_NAME,
		.owner = THIS_MODULE,
	},
	.address_list	= u_i2c_addr.normal_i2c,
};


//����غ���
static int __devinit goodix_ts_init(void)
{
	int ret = -1;
	int err = -1;

	printk("===========================%s=====================\n", __func__);

	if (ctp_ops.fetch_sysconfig_para)
	{
		if(ctp_ops.fetch_sysconfig_para()){
			printk("%s: err.\n", __func__);
			return -1;
		}
	}
	printk("%s: after fetch_sysconfig_para:  normal_i2c: 0x%hx. normal_i2c[1]: 0x%hx \n", \
	__func__, u_i2c_addr.normal_i2c[0], u_i2c_addr.normal_i2c[1]);

	err = ctp_ops.init_platform_resource();
	if(0 != err){
		printk("%s:ctp_ops.init_platform_resource err. \n", __func__);    
	}

	//reset
	ctp_ops.ts_reset();
	//wakeup
	ctp_ops.ts_wakeup();
	
	goodix_ts_driver.detect = ctp_ops.ts_detect;

	ret = i2c_add_driver(&goodix_ts_driver);

	return ret;
}

//��ж�غ���
static void __exit goodix_ts_exit(void)
{
	i2c_del_driver(&goodix_ts_driver);
	ctp_ops.free_platform_resource();
	
	return;
}

late_initcall(goodix_ts_init);
module_exit(goodix_ts_exit);

MODULE_DESCRIPTION("Goodix Touchscreen Driver");
MODULE_LICENSE("GPL v2");
