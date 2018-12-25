/* 
 * drivers/input/touchscreen/ft5x0x_ts.c
 *
 * FocalTech ft5x TouchScreen driver. 
 *
 * Copyright (c) 2010  Focal tech Ltd.
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
 *
 *	note: only support mulititouch	Wenfs 2010-10-01
 *  for this touchscreen to work, it's slave addr must be set to 0x7e | 0x70
 */

#include <linux/i2c.h>
#include <linux/input.h>
#include "ft5x_ts_pb.h"

#if defined(CONFIG_PM)
	#include <linux/pm.h>
	#include <linux/suspend.h>
	#include <linux/power/aw_pm.h>
#endif

#include <linux/delay.h>
#include <linux/interrupt.h>
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
#include "ctp_platform_ops.h"
#include <linux/suspend.h>
#include <linux/proc_fs.h>
#include <mach/gpio.h>

#define FOR_TSLIB_TEST
//#define PRINT_INT_INFO
#define PRINT_POINT_INFO
//#define CONFIG_FT5X0X_SLIDER
//#define TOUCH_KEY_SUPPORT
#ifdef TOUCH_KEY_SUPPORT
//#define TOUCH_KEY_FOR_EVB13
//#define TOUCH_KEY_FOR_ANGDA
#ifdef TOUCH_KEY_FOR_ANGDA
#define TOUCH_KEY_X_LIMIT	(60000)
#define TOUCH_KEY_NUMBER	(4)
#endif
#ifdef TOUCH_KEY_FOR_EVB13
#define TOUCH_KEY_LOWER_X_LIMIT	(848)
#define TOUCH_KEY_HIGHER_X_LIMIT	(852)
#define TOUCH_KEY_NUMBER	(5)
#endif
#endif

struct i2c_dev{
struct list_head list;	
struct i2c_adapter *adap;
struct device *dev;
};

static struct class *i2c_dev_class;
static LIST_HEAD (i2c_dev_list);
static DEFINE_SPINLOCK(i2c_dev_list_lock);

#define FT5X_NAME	"ft5x_ts"//"synaptics_i2c_rmi"//"synaptics-rmi-ts"// 

static struct i2c_client *this_client;
#ifdef TOUCH_KEY_SUPPORT
static int key_tp  = 0;
static int key_val = 0;
#endif

#ifdef PRINT_POINT_INFO 
#define print_point_info(fmt, args...)   \
        do{                              \
                pr_info(fmt, ##args);     \
        }while(0)
#else
#define print_point_info(fmt, args...)   //
#endif

#ifdef PRINT_INT_INFO 
#define print_int_info(fmt, args...)     \
        do{                              \
                pr_info(fmt, ##args);     \
        }while(0)
#else
#define print_int_info(fmt, args...)   //
#endif
///////////////////////////////////////////////
//specific tp related macro: need be configured for specific tp
#define CTP_IRQ_NO			(gpio_int_info[0].port_num)

#define CTP_IRQ_MODE			(NEGATIVE_EDGE)
#define CTP_NAME			FT5X_NAME
#define TS_RESET_LOW_PERIOD		(10)
#define TS_INITIAL_HIGH_PERIOD		(30)
#define TS_WAKEUP_LOW_PERIOD	(20)
#define TS_WAKEUP_HIGH_PERIOD	(20)
#define TS_POLL_DELAY			(10)	/* ms delay between samples */
#define TS_POLL_PERIOD			(10)	/* ms delay between samples */
#define SCREEN_MAX_X			(screen_max_x)
#define SCREEN_MAX_Y			(screen_max_y)
#define PRESS_MAX			(255)


static void* __iomem gpio_addr = NULL;
static int gpio_int_hdle = 0;
static int gpio_wakeup_hdle = 0;
static int gpio_reset_hdle = 0;
static int gpio_wakeup_enable = 1;
static int gpio_reset_enable = 1;

static int gpio_power_hdle = 0;

static int screen_max_x = 0;
static int screen_max_y = 0;
static int revert_x_flag = 0;
static int revert_y_flag = 0;
static int exchange_x_y_flag = 0;
static int tp_no_swap_hack_flag = 0;

extern int ft5x02_Init_IC_Param(struct i2c_client * client);
extern int ft5x02_get_ic_param(struct i2c_client * client);

/* Addresses to scan */
static union{
	unsigned short dirty_addr_buf[2];
	const unsigned short normal_i2c[2];
}u_i2c_addr = {{0x00},};
static __u32 twi_id = 0;

//use sw gpio interrupt trigger

/*
 * ctp_get_pendown_state  : get the int_line data state,
 *
 * return value:
 *             return PRESS_DOWN: if down
 *             return FREE_UP: if up,
 *             return 0: do not need process, equal free up.
 */
static int ctp_get_pendown_state(void) {

	//not supported
	return 0;
}

/**
 * ctp_clear_penirq - clear int pending
 *
 */
static void ctp_clear_penirq(void) {

	sw_gpio_eint_clr_irqpd_sta(gpio_int_hdle);
	return;
}

/**
 * ctp_set_irq_mode - according sysconfig's subkey "ctp_int_port" to config int port.
 *
 * return value:
 *              0:      success;
 *              others: fail;
 */
static int ctp_set_irq_mode(char *major_key , char *subkey, ext_int_mode int_mode) {

	pr_info("%s: config gpio to int mode. \n", __func__);

	if(gpio_int_hdle){
		sw_gpio_irq_free(gpio_int_hdle);
	}

	gpio_int_hdle = sw_gpio_irq_request(major_key,subkey,int_mode);

	return gpio_int_hdle ? 0 : -EIO;
}

/**
 * ctp_set_gpio_mode - according sysconfig's subkey "ctp_io_port" to config io port.
 *
 * return value:
 *              0:      success;
 *              others: fail;
 */
static int ctp_set_gpio_mode(void) {

	//not supported
	return -1;
}

/**
 * ctp_judge_int_occur - whether interrupt occur.
 *
 * return value:
 *              0:      int occur;
 *              others: no int occur;
 */
static int ctp_judge_int_occur(void) {

	return (sw_gpio_eint_get_irqpd_sta(gpio_int_hdle) == 1) ? 0 : 1;
}

/**
 * ctp_free_platform_resource - corresponding with ctp_init_platform_resource
 *
 */
static void ctp_free_platform_resource(void)
{
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

	if(gpio_power_hdle){
		gpio_release(gpio_power_hdle, 2);
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
	//pr_info("%s, gpio_addr = 0x%x. \n", __func__, gpio_addr);
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

	gpio_power_hdle = gpio_request_ex("ctp_para", "ctp_powerpin");
	if(!gpio_reset_hdle) {
		pr_warning("%s: tp_power request gpio fail!\n", __func__);
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
	__u32 twi_addr = 0;
	//__u32 twi_id = 0;
	script_parser_value_type_t type = SCIRPT_PARSER_VALUE_TYPE_STRING;

	pr_info("%s. \n", __func__);

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
	//pr_info("%s: before: ctp_twi_addr is 0x%x, dirty_addr_buf: 0x%hx. dirty_addr_buf[1]: 0x%hx \n", __func__, twi_addr, u_i2c_addr.dirty_addr_buf[0], u_i2c_addr.dirty_addr_buf[1]);
	u_i2c_addr.dirty_addr_buf[0] = twi_addr;
	u_i2c_addr.dirty_addr_buf[1] = I2C_CLIENT_END;
	pr_info("%s: after: ctp_twi_addr is 0x%x, dirty_addr_buf: 0x%hx. dirty_addr_buf[1]: 0x%hx \n", __func__, twi_addr, u_i2c_addr.dirty_addr_buf[0], u_i2c_addr.dirty_addr_buf[1]);
	//pr_info("%s: after: ctp_twi_addr is 0x%x, u32_dirty_addr_buf: 0x%hx. u32_dirty_addr_buf[1]: 0x%hx \n", __func__, twi_addr, u32_dirty_addr_buf[0],u32_dirty_addr_buf[1]);

	if(SCRIPT_PARSER_OK != script_parser_fetch("ctp_para", "ctp_twi_id", &twi_id, sizeof(twi_id)/sizeof(__u32))){
		pr_err("%s: script_parser_fetch err. \n", name);
		goto script_parser_fetch_err;
	}
	pr_info("%s: ctp_twi_id is %d. \n", __func__, twi_id);
	
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

	if(SCRIPT_PARSER_OK != script_parser_fetch("ctp_para", "ctp_tp_no_swap_hack_flag", &tp_no_swap_hack_flag, 1)){
		pr_info("ft5x_ts: swap hack not disabled\n");
		tp_no_swap_hack_flag = 0;
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
	if(gpio_reset_enable){
		pr_info("%s. \n", __func__);
		if(EGPIO_SUCCESS != gpio_write_one_pin_value(gpio_reset_hdle, 0, "ctp_reset")){
			pr_info("%s: err when operate gpio. \n", __func__);
		}
		mdelay(TS_RESET_LOW_PERIOD);
		if(EGPIO_SUCCESS != gpio_write_one_pin_value(gpio_reset_hdle, 1, "ctp_reset")){
			pr_info("%s: err when operate gpio. \n", __func__);
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
	if(1 == gpio_wakeup_enable){  
		pr_info("%s. \n", __func__);
		if(EGPIO_SUCCESS != gpio_write_one_pin_value(gpio_wakeup_hdle, 0, "ctp_wakeup")){
			pr_info("%s: err when operate gpio. \n", __func__);
		}
		mdelay(TS_WAKEUP_LOW_PERIOD);
		if(EGPIO_SUCCESS != gpio_write_one_pin_value(gpio_wakeup_hdle, 1, "ctp_wakeup")){
			pr_info("%s: err when operate gpio. \n", __func__);
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
static int ctp_detect(struct i2c_client *client, struct i2c_board_info *info)
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

static struct i2c_dev *get_free_i2c_dev(struct i2c_adapter *adap) 
{
	struct i2c_dev *i2c_dev;

	if (adap->nr >= I2C_MINORS){
		pr_info("i2c-dev:out of device minors (%d) \n",adap->nr);
		return ERR_PTR (-ENODEV);
	}

	i2c_dev = kzalloc(sizeof(*i2c_dev), GFP_KERNEL);
	if (!i2c_dev){
		return ERR_PTR(-ENOMEM);
	}
	i2c_dev->adap = adap;

	spin_lock(&i2c_dev_list_lock);
	list_add_tail(&i2c_dev->list, &i2c_dev_list);
	spin_unlock(&i2c_dev_list_lock);
	
	return i2c_dev;
}


static int ft5x_i2c_rxdata(char *rxdata, int length);

struct ts_event {
	u16	x1;
	u16	y1;
	u16	x2;
	u16	y2;
	u16	x3;
	u16	y3;
	u16	x4;
	u16	y4;
	u16	x5;
	u16	y5;
	u16	pressure;
	s16 touch_ID1;
	s16 touch_ID2;
	s16 touch_ID3;
	s16 touch_ID4;
	s16 touch_ID5;
    u8  touch_point;
};
#ifdef CONFIG_FT5X0X_SLIDER
struct slider_event_s {
	int x;
	bool active;
};
#endif
struct ft5x_ts_data {
	struct input_dev	*input_dev;
#ifdef CONFIG_FT5X0X_SLIDER
	struct input_dev	*input_dev_slider;
	struct slider_event_s slider_event;
#endif
	struct ts_event		event;
	struct delayed_work 	pen_event_work;
	struct workqueue_struct *ts_workqueue;
	int in_suspend;
	bool defered_read;
	bool locked;
};


/* ---------------------------------------------------------------------
*
*   Focal Touch panel upgrade related driver
*
*
----------------------------------------------------------------------*/
typedef enum
{
	ERR_OK,
	ERR_MODE,
	ERR_READID,
	ERR_ERASE,
	ERR_STATUS,
	ERR_ECC,
	ERR_DL_ERASE_FAIL,
	ERR_DL_PROGRAM_FAIL,
	ERR_DL_VERIFY_FAIL
}E_UPGRADE_ERR_TYPE;

typedef unsigned char         FTS_BYTE;     //8 bit
typedef unsigned short        FTS_WORD;    //16 bit
typedef unsigned int          FTS_DWRD;    //16 bit
typedef unsigned char         FTS_BOOL;    //8 bit

#define FTS_NULL                0x0
#define FTS_TRUE                0x01
#define FTS_FALSE               0x0

#define I2C_CTPM_ADDRESS        (0x70>>1)
/*
[function]: 
    callback: read data from ctpm by i2c interface,implemented by special user;
[parameters]:
    bt_ctpm_addr[in]    :the address of the ctpm;
    pbt_buf[out]        :data buffer;
    dw_lenth[in]        :the length of the data buffer;
[return]:
    FTS_TRUE     :success;
    FTS_FALSE    :fail;
*/
int i2c_read_interface(u8 bt_ctpm_addr, u8* pbt_buf, u16 dw_lenth)
{
	int ret;

	ret=i2c_master_recv(this_client, pbt_buf, dw_lenth);

	if(ret != dw_lenth){
		pr_info("ret = %d. \n", ret);
		pr_info("i2c_read_interface error\n");
		return FTS_FALSE;
	}

	return FTS_TRUE;
}

/*
[function]: 
    callback: write data to ctpm by i2c interface,implemented by special user;
[parameters]:
    bt_ctpm_addr[in]    :the address of the ctpm;
    pbt_buf[in]        :data buffer;
    dw_lenth[in]        :the length of the data buffer;
[return]:
    FTS_TRUE     :success;
    FTS_FALSE    :fail;
*/
int i2c_write_interface(u8 bt_ctpm_addr, u8* pbt_buf, u16 dw_lenth)
{
	int ret;
	ret=i2c_master_send(this_client, pbt_buf, dw_lenth);
	if(ret != dw_lenth){
		pr_info("i2c_write_interface error\n");
		return FTS_FALSE;
	}

	return FTS_TRUE;
}


/***************************************************************************************/

/*
[function]: 
    read out the register value.
[parameters]:
    e_reg_name[in]    :register name;
    pbt_buf[out]    :the returned register value;
    bt_len[in]        :length of pbt_buf, should be set to 2;        
[return]:
    FTS_TRUE    :success;
    FTS_FALSE    :io fail;
*/
u8 fts_register_read(u8 e_reg_name, u8* pbt_buf, u8 bt_len)
{
	u8 read_cmd[3]= {0};
	u8 cmd_len     = 0;

	read_cmd[0] = e_reg_name;
	cmd_len = 1;    

	/*call the write callback function*/
	//    if(!i2c_write_interface(I2C_CTPM_ADDRESS, &read_cmd, cmd_len))
	//    {
	//        return FTS_FALSE;
	//    }


	if(!i2c_write_interface(I2C_CTPM_ADDRESS, read_cmd, cmd_len))	{//change by zhengdixu
		return FTS_FALSE;
	}

	/*call the read callback function to get the register value*/        
	if(!i2c_read_interface(I2C_CTPM_ADDRESS, pbt_buf, bt_len)){
		return FTS_FALSE;
	}
	return FTS_TRUE;
}

/*
*ft5x02_i2c_Read-read data and write data by i2c
*@client: handle of i2c
*@writebuf: Data that will be written to the slave
*@writelen: How many bytes to write
*@readbuf: Where to store data read from slave
*@readlen: How many bytes to read
*
*Returns negative errno, else the number of messages executed
*
*
*/
int ft5x02_i2c_Read(struct i2c_client *client,  char * writebuf, int writelen, 
							char *readbuf, int readlen)
{
	int ret;

	if(writelen > 0)
	{
		struct i2c_msg msgs[] = {
			{
				.addr	= client->addr,
				.flags	= 0,
				.len	= writelen,
				.buf	= writebuf,
			},
			{
				.addr	= client->addr,
				.flags	= I2C_M_RD,
				.len	= readlen,
				.buf	= readbuf,
			},
		};
		ret = i2c_transfer(client->adapter, msgs, 2);
		if (ret < 0)
			pr_err("function:%s. i2c read error: %d\n", __func__, ret);
	}
	else
	{
		struct i2c_msg msgs[] = {
			{
				.addr	= client->addr,
				.flags	= I2C_M_RD,
				.len	= readlen,
				.buf	= readbuf,
			},
		};
		ret = i2c_transfer(client->adapter, msgs, 1);
		if (ret < 0)
			pr_err("function:%s. i2c read error: %d\n", __func__, ret);
	}
	return ret;
}
/*
*write data by i2c 
*/
int ft5x02_i2c_Write(struct i2c_client *client, char *writebuf, int writelen)
{
	int ret;

	struct i2c_msg msg[] = {
		{
			.addr	= client->addr,
			.flags	= 0,
			.len	= writelen,
			.buf	= writebuf,
		},
	};

	ret = i2c_transfer(client->adapter, msg, 1);
	if (ret < 0)
		pr_err("%s i2c write error: %d\n", __func__, ret);

	return ret;
}


int ft5x02_write_reg(struct i2c_client * client, u8 regaddr, u8 regvalue)
{
	unsigned char buf[2] = {0};
	buf[0] = regaddr;
	buf[1] = regvalue;

	return ft5x02_i2c_Write(client, buf, sizeof(buf));
}

int ft5x02_read_reg(struct i2c_client * client, u8 regaddr, u8 * regvalue)
{
	return ft5x02_i2c_Read(client, &regaddr, 1, regvalue, 1);
}

/*
[function]: 
    write a value to register.
[parameters]:
    e_reg_name[in]    :register name;
    pbt_buf[in]        :the returned register value;
[return]:
    FTS_TRUE    :success;
    FTS_FALSE    :io fail;
*/
int fts_register_write(u8 e_reg_name, u8 bt_value)
{
	FTS_BYTE write_cmd[2] = {0};

	write_cmd[0] = e_reg_name;
	write_cmd[1] = bt_value;

	/*call the write callback function*/
	//return i2c_write_interface(I2C_CTPM_ADDRESS, &write_cmd, 2);
	return i2c_write_interface(I2C_CTPM_ADDRESS, write_cmd, 2); //change by zhengdixu
}

/*
[function]: 
    send a command to ctpm.
[parameters]:
    btcmd[in]        :command code;
    btPara1[in]    :parameter 1;    
    btPara2[in]    :parameter 2;    
    btPara3[in]    :parameter 3;    
    num[in]        :the valid input parameter numbers, if only command code needed and no parameters followed,then the num is 1;    
[return]:
    FTS_TRUE    :success;
    FTS_FALSE    :io fail;
*/
int cmd_write(u8 btcmd,u8 btPara1,u8 btPara2,u8 btPara3,u8 num)
{
	FTS_BYTE write_cmd[4] = {0};

	write_cmd[0] = btcmd;
	write_cmd[1] = btPara1;
	write_cmd[2] = btPara2;
	write_cmd[3] = btPara3;
	//return i2c_write_interface(I2C_CTPM_ADDRESS, &write_cmd, num);
	return i2c_write_interface(I2C_CTPM_ADDRESS, write_cmd, num);//change by zhengdixu
}

/*
[function]: 
    write data to ctpm , the destination address is 0.
[parameters]:
    pbt_buf[in]    :point to data buffer;
    bt_len[in]        :the data numbers;    
[return]:
    FTS_TRUE    :success;
    FTS_FALSE    :io fail;
*/
int byte_write(u8* pbt_buf, u16 dw_len)
{
	return i2c_write_interface(I2C_CTPM_ADDRESS, pbt_buf, dw_len);
}

/*
[function]: 
    read out data from ctpm,the destination address is 0.
[parameters]:
    pbt_buf[out]    :point to data buffer;
    bt_len[in]        :the data numbers;    
[return]:
    FTS_TRUE    :success;
    FTS_FALSE    :io fail;
*/
int byte_read(u8* pbt_buf, u8 bt_len)
{
	return i2c_read_interface(I2C_CTPM_ADDRESS, pbt_buf, bt_len);
	//ft5x_i2c_rxdata
}


/*
[function]: 
    burn the FW to ctpm.
[parameters]:(ref. SPEC)
    pbt_buf[in]    :point to Head+FW ;
    dw_lenth[in]:the length of the FW + 6(the Head length);    
    bt_ecc[in]    :the ECC of the FW
[return]:
    ERR_OK        :no error;
    ERR_MODE    :fail to switch to UPDATE mode;
    ERR_READID    :read id fail;
    ERR_ERASE    :erase chip fail;
    ERR_STATUS    :status error;
    ERR_ECC        :ecc error.
*/

static int ft5x_i2c_rxdata(char *rxdata, int length)
{
	int ret;

	struct i2c_msg msgs[] = {
		{
			.addr	= this_client->addr,
			.flags	= 0,
			.len	= 1,
			.buf	= rxdata,
		},
		{
			.addr	= this_client->addr,
			.flags	= I2C_M_RD,
			.len	= length,
			.buf	= rxdata,
		},
	};

        //pr_info("IIC add = %x\n",this_client->addr);
	ret = i2c_transfer(this_client->adapter, msgs, 2);
	if (ret < 0)
		pr_info("msg %s i2c read error: %d\n", __func__, ret);
	
	return ret;
}

static int ft5x_i2c_txdata(char *txdata, int length)
{
	int ret;

	struct i2c_msg msg[] = {
		{
			.addr	= this_client->addr,
			.flags	= 0,
			.len	= length,
			.buf	= txdata,
		},
	};

   	//msleep(1);
	ret = i2c_transfer(this_client->adapter, msg, 1);
	if (ret < 0)
		pr_err("%s i2c write error: %d\n", __func__, ret);

	return ret;
}

static int ft5x_set_reg(u8 addr, u8 para)
{
	u8 buf[3];
	int ret = -1;

	buf[0] = addr;
	buf[1] = para;
	ret = ft5x_i2c_txdata(buf, 2);
	if (ret < 0) {
		pr_err("write reg failed! %#x ret: %d", buf[0], ret);
		return -1;
	}

	return 0;
}

static void ft5x_ts_release(void)
{
	struct ft5x_ts_data *data = i2c_get_clientdata(this_client);
#ifdef CONFIG_FT5X0X_MULTITOUCH	
#ifdef TOUCH_KEY_SUPPORT
	if(1 == key_tp){
		input_report_key(data->input_dev, key_val, 0);
		print_point_info("Release Key = %d\n",key_val);		
	} else{
		input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, 0);
	}
#else
	input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, 0);
#endif

#else
	input_report_abs(data->input_dev, ABS_PRESSURE, 0);
	input_report_key(data->input_dev, BTN_TOUCH, 0);
#endif
	
	input_sync(data->input_dev);
	return;

}

#ifdef CONFIG_FT5X0X_SLIDER
//threshold between screen touch and slider touch points;
#define TP_MAX_X 1600

/*
 * We have one touch controller for touch screen and slider;
 * All points those do not hit to SCREEN_MAX_X x SCREEN_MAX_Y square consider as slider.
 * This function searches in event struct slider's point, fill its and delete from touch screen struct those points.
 */
static int check_slider_point(struct ts_event *ev) {
	struct ft5x_ts_data *data = i2c_get_clientdata(this_client);
	struct slider_event_s *sl_ev = &data->slider_event;
	int i;
	//check all points
	switch(ev->touch_point) {
		case 3:
		{
			if ((ev->x3) > TP_MAX_X) {
				sl_ev->x = ev->y3;
				sl_ev->active = true;

			}
		}
		case 2:
		{
			if ((ev->x2) > TP_MAX_X) {
				sl_ev->x = ev->y2;
				sl_ev->active = true;

			}
		}
		case 1:
		{
			if (ev->x1 > TP_MAX_X) {
				sl_ev->x = ev->y1;
				sl_ev->active = true;
			}
		}
		default:
			break;
	}
	u16 *p;
	//clean missed points to screen touch
	for (i = ev->touch_point; i > 0; i--) {
		p = ((u16 *) ev) + (i - 1) * 2;
		//printk("[%i] x = %i, y = %i :: ",i,(u16) *p,(u16) *(p+1));
		if ((u16) *p > TP_MAX_X) {
			memmove(p, p + 2, (ev->touch_point - (i - 1)) * 2 * sizeof(u16));
			--(ev->touch_point);
		}
	}
	return 0;
}

static int report_slider_point() {
	struct ft5x_ts_data *data = i2c_get_clientdata(this_client);
	struct slider_event_s *sl_ev = &data->slider_event;
	if (sl_ev->active) {
		input_report_abs(data->input_dev_slider, ABS_X, sl_ev->x);
		input_sync(data->input_dev_slider);
		sl_ev->active = false;
	}
	return 0;
}
#endif

static int ft5x_read_data(void)
{
	struct ft5x_ts_data *data = i2c_get_clientdata(this_client);
	struct ts_event *event = &data->event;
	unsigned char buf[32]={0};
	int ret = -1;
	bool swap_flag = false;

	if (data->in_suspend)
		return -EBUSY;

#ifdef CONFIG_FT5X0X_MULTITOUCH
	ret = ft5x_i2c_rxdata(buf, 31);
#else
	ret = ft5x_i2c_rxdata(buf, 31);
#endif
	if (ret < 0) {
		pr_info("%s read_data i2c_rxdata failed: %d\n", __func__, ret);
		return ret;
	}

//	int i = 0;
//	printk("----------buf--------\n");
//	for(i = 2;i<=10;i++)
//		printk("%x ",buf[i]);
//	printk("\n----------end--------\n");

	memset(event, 0, sizeof(struct ts_event));

	event->touch_point = buf[2] & 0x07;// 000 0111
	//print_point_info("touch point = %d\n",event->touch_point);

	if (event->touch_point == 0 && data->defered_read) {
		event->touch_point = 1;
		if (!tp_no_swap_hack_flag)
			swap_flag = true;
	}
	data->defered_read = false;

//	if (event->touch_point == 0) {
//		ft5x_ts_release();
//		return 1;
//	}

#ifdef CONFIG_FT5X0X_MULTITOUCH
	switch (event->touch_point) {
	case 5:
		event->x5 = (s16)(buf[0x1b] & 0x0F)<<8 | (s16)buf[0x1c];
		event->y5 = (s16)(buf[0x1d] & 0x0F)<<8 | (s16)buf[0x1e];
		if(1 == revert_x_flag){
			event->x5 = SCREEN_MAX_X - event->x5;
		}
		if(1 == revert_y_flag){
			event->y5 = SCREEN_MAX_Y - event->y5;
		}
		//pr_info("before swap: event->x5 = %d, event->y5 = %d. \n", event->x5, event->y5);
		if(1 == exchange_x_y_flag){
			swap(event->x5, event->y5);
		}
		//pr_info("after swap: event->x5 = %d, event->y5 = %d. \n", event->x5, event->y5);
		event->touch_ID5=(s16)(buf[0x1D] & 0xF0)>>4;
	case 4:
		event->x4 = (s16)(buf[0x15] & 0x0F)<<8 | (s16)buf[0x16];
		event->y4 = (s16)(buf[0x17] & 0x0F)<<8 | (s16)buf[0x18];
		if(1 == revert_x_flag){
			event->x4 = SCREEN_MAX_X - event->x4;
		}
		if(1 == revert_y_flag){
			event->y4 = SCREEN_MAX_Y - event->y4;
		}
		//pr_info("before swap: event->x4 = %d, event->y4 = %d. \n", event->x4, event->y4);
		if(1 == exchange_x_y_flag){
			swap(event->x4, event->y4);
		}
		//pr_info("after swap: event->x4 = %d, event->y4 = %d. \n", event->x4, event->y4);
		event->touch_ID4=(s16)(buf[0x17] & 0xF0)>>4;
	case 3:
		event->x3 = (s16)(buf[0x0f] & 0x0F)<<8 | (s16)buf[0x10];
		event->y3 = (s16)(buf[0x11] & 0x0F)<<8 | (s16)buf[0x12];
		if(1 == revert_x_flag){
			event->x3 = SCREEN_MAX_X - event->x3;
		}
		if(1 == revert_y_flag){
			event->y3 = SCREEN_MAX_Y - event->y3;
		}
		//pr_info("before swap: event->x3 = %d, event->y3 = %d. \n", event->x3, event->y3);
		if(1 == exchange_x_y_flag){
			swap(event->x3, event->y3);
		}
		//pr_info("after swap: event->x3 = %d, event->y3 = %d. \n", event->x3, event->y3);
		event->touch_ID3=(s16)(buf[0x11] & 0xF0)>>4;
	case 2:
		event->x2 = (s16)(buf[9] & 0x0F)<<8 | (s16)buf[10];
		event->y2 = (s16)(buf[11] & 0x0F)<<8 | (s16)buf[12];
		if(1 == revert_x_flag){
			event->x2 = SCREEN_MAX_X - event->x2;
		}
		if(1 == revert_y_flag){
			event->y2 = SCREEN_MAX_Y - event->y2;
		}
		//pr_info("before swap: event->x2 = %d, event->y2 = %d. \n", event->x2, event->y2);
		if(1 == exchange_x_y_flag){
			swap(event->x2, event->y2);
		}
		//pr_info("after swap: event->x2 = %d, event->y2 = %d. \n", event->x2, event->y2);
		event->touch_ID2=(s16)(buf[0x0b] & 0xF0)>>4;
	case 1:
		event->x1 = (s16)(buf[3] & 0x0F)<<8 | (s16)buf[4];
		event->y1 = (s16)(buf[5] & 0x0F)<<8 | (s16)buf[6];
#ifdef TOUCH_KEY_FOR_ANGDA
		if(event->x1 < TOUCH_KEY_X_LIMIT)
		{
			if(1 == revert_x_flag){
				event->x1 = SCREEN_MAX_X - event->x1;
			}
			if(1 == revert_y_flag){
				event->y1 = SCREEN_MAX_Y - event->y1;
			}
			//pr_info("before swap: event->x1 = %d, event->y1 = %d. \n", event->x1, event->y1);
			if(1 == exchange_x_y_flag){
				swap(event->x1, event->y1);
			}
		}
#elif defined(TOUCH_KEY_FOR_EVB13)
		if((event->x1 > TOUCH_KEY_LOWER_X_LIMIT)&&(event->x1<TOUCH_KEY_HIGHER_X_LIMIT))
		{
			if(1 == revert_x_flag){
				event->x1 = SCREEN_MAX_X - event->x1;
			}
			if(1 == revert_y_flag){
				event->y1 = SCREEN_MAX_Y - event->y1;
			}
			//pr_info("before swap: event->x1 = %d, event->y1 = %d. \n", event->x1, event->y1);
			if(1 == exchange_x_y_flag){
				swap(event->x1, event->y1);
			}
		}
#else
		if(1 == revert_x_flag){
			event->x1 = SCREEN_MAX_X - event->x1;
		}
		if(1 == revert_y_flag){
			event->y1 = SCREEN_MAX_Y - event->y1;
		}
		//pr_info("before swap: event->x1 = %d, event->y1 = %d. \n", event->x1, event->y1);
		if(1 == exchange_x_y_flag || swap_flag){
			swap(event->x1, event->y1);
		}
#endif

		//pr_info("after swap: event->x1 = %d, event->y1 = %d. \n", event->x1, event->y1);
		event->touch_ID1=(s16)(buf[0x05] & 0xF0)>>4;
		break;

	case 0:
		return 0;

	default:
		return -1;
	}
#else
	if (event->touch_point == 1) {
		event->x1 = (s16)(buf[3] & 0x0F)<<8 | (s16)buf[4];
		event->y1 = (s16)(buf[5] & 0x0F)<<8 | (s16)buf[6];
	}
#endif
	event->pressure = 200;

	dev_dbg(&this_client->dev, "%s: 1:%d %d 2:%d %d \n", __func__,
	event->x1, event->y1, event->x2, event->y2);

#ifdef CONFIG_FT5X0X_SLIDER
	check_slider_point(event);
#endif
    return 0;
}

static void ft5x_report_multitouch(void)
{
	struct ft5x_ts_data *data = i2c_get_clientdata(this_client);
	struct ts_event *event = &data->event;

#ifdef TOUCH_KEY_SUPPORT
	if(1 == key_tp){
		return;
	}
#endif

	switch(event->touch_point) {
	case 5:
		input_report_abs(data->input_dev, ABS_MT_TRACKING_ID, event->touch_ID5);	
		input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
		input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x5);
		input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y5);
		input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
		input_mt_sync(data->input_dev);
		//print_point_info("===x5 = %d,y5 = %d ====\n",event->x2,event->y2);
	case 4:
		input_report_abs(data->input_dev, ABS_MT_TRACKING_ID, event->touch_ID4);	
		input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
		input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x4);
		input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y4);
		input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
		input_mt_sync(data->input_dev);
		//print_point_info("===x4 = %d,y4 = %d ====\n",event->x2,event->y2);
	case 3:
		input_report_abs(data->input_dev, ABS_MT_TRACKING_ID, event->touch_ID3);	
		input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
		input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x3);
		input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y3);
		input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
		input_mt_sync(data->input_dev);
		//print_point_info("===x3 = %d,y3 = %d ====\n",event->x2,event->y2);
	case 2:
		input_report_abs(data->input_dev, ABS_MT_TRACKING_ID, event->touch_ID2);	
		input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
		input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x2);
		input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y2);
		input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
		input_mt_sync(data->input_dev);
		//print_point_info("===x2 = %d,y2 = %d ====\n",event->x2,event->y2);
	case 1:
		input_report_abs(data->input_dev, ABS_MT_TRACKING_ID, event->touch_ID1);	
		input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
		input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x1);
		input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y1);
		input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
		input_mt_sync(data->input_dev);
		//print_point_info("===x1 = %d,y1 = %d ====\n",event->x1,event->y1);
		break;

	case 0:
		return;

	default:
		//print_point_info("==touch_point default =\n");
		break;
	}
	
	input_sync(data->input_dev);
	dev_dbg(&this_client->dev, "%s: 1:%d %d 2:%d %d \n", __func__,
		event->x1, event->y1, event->x2, event->y2);
	return;
}

#ifndef CONFIG_FT5X0X_MULTITOUCH
static void ft5x_report_singletouch(void)
{
	struct ft5x_ts_data *data = i2c_get_clientdata(this_client);
	struct ts_event *event = &data->event;
	
	if (event->touch_point == 1) {
		input_report_abs(data->input_dev, ABS_X, event->x1);
		input_report_abs(data->input_dev, ABS_Y, event->y1);
		input_report_abs(data->input_dev, ABS_PRESSURE, event->pressure);
	}
	input_report_key(data->input_dev, BTN_TOUCH, 1);
	input_sync(data->input_dev);
	dev_dbg(&this_client->dev, "%s: 1:%d %d 2:%d %d \n", __func__,
		event->x1, event->y1, event->x2, event->y2);

	return;
}
#endif

#ifdef TOUCH_KEY_SUPPORT
static void ft5x_report_touchkey(void)
{
	struct ft5x_ts_data *data = i2c_get_clientdata(this_client);
	struct ts_event *event = &data->event;
	//print_point_info("x=%d===Y=%d\n",event->x1,event->y1);

#ifdef TOUCH_KEY_FOR_ANGDA
	if((1==event->touch_point)&&(event->x1 > TOUCH_KEY_X_LIMIT)){
		key_tp = 1;
		if(event->y1 < 40){
			key_val = 1;
			input_report_key(data->input_dev, key_val, 1);
			input_sync(data->input_dev);  
			print_point_info("===KEY 1====\n");
		}else if(event->y1 < 90){
			key_val = 2;
			input_report_key(data->input_dev, key_val, 1);
			input_sync(data->input_dev);     
			print_point_info("===KEY 2 ====\n");
		}else{
			key_val = 3;
			input_report_key(data->input_dev, key_val, 1);
			input_sync(data->input_dev);     
			print_point_info("===KEY 3====\n");	
		}
	} else{
		key_tp = 0;
	}
#endif
#ifdef TOUCH_KEY_FOR_EVB13
	if((1==event->touch_point)&&((event->x1 > TOUCH_KEY_LOWER_X_LIMIT)&&(event->x1<TOUCH_KEY_HIGHER_X_LIMIT))){
		key_tp = 1;
		if(event->y1 < 5){
			key_val = 1;
			input_report_key(data->input_dev, key_val, 1);
			input_sync(data->input_dev);  
			print_point_info("===KEY 1====\n");     
		}else if((event->y1 < 45)&&(event->y1>35)){
			key_val = 2;
			input_report_key(data->input_dev, key_val, 1);
			input_sync(data->input_dev);     
			print_point_info("===KEY 2 ====\n");
		}else if((event->y1 < 75)&&(event->y1>65)){
			key_val = 3;
			input_report_key(data->input_dev, key_val, 1);
			input_sync(data->input_dev);     
			print_point_info("===KEY 3====\n");
		}else if ((event->y1 < 105)&&(event->y1>95))	{
			key_val = 4;
			input_report_key(data->input_dev, key_val, 1);
			input_sync(data->input_dev);     
			print_point_info("===KEY 4====\n");	
		}
	}else{
		key_tp = 0;
	}
#endif

	return;
}
#endif

static void ft5x_report_value(void)
{

	//pr_info("==ft5x_report_value =\n");
#ifdef TOUCH_KEY_SUPPORT
	ft5x_report_touchkey();
#endif

#ifdef CONFIG_FT5X0X_MULTITOUCH
	ft5x_report_multitouch();
#else	/* CONFIG_FT5X0X_MULTITOUCH*/
	ft5x_report_singletouch();
#endif	/* CONFIG_FT5X0X_MULTITOUCH*/

#ifdef CONFIG_FT5X0X_SLIDER
	report_slider_point();
#endif

	return;
}	/*end ft5x_report_value*/


#define MAX_RETRY_READ 4

static void ft5x_ts_pen_irq_work(struct delayed_work *work)
{
	int ret = -1;
	static int retry_counter = 0;
	struct ft5x_ts_data *ft5x_ts = container_of(work,struct ft5x_ts_data,pen_event_work);

	//pr_info("==work 1=\n");
	ret = ft5x_read_data();
	if (ret == 0) {
		ft5x_report_value();
		if (ft5x_ts->event.touch_point == 0)
			ft5x_ts_release();
		retry_counter = 0;
	} else if (ret == -EBUSY || ret == -ENODEV) {
		//bus or device are not resumed still
		//delay work on a future
		printk("[%s] return error %i\n",__func__,ret);
		if (retry_counter > MAX_RETRY_READ) {
			printk("[%s] reading work was fail %i time, canceling\n", __func__, MAX_RETRY_READ);
			retry_counter = 0;
		} else {
			queue_delayed_work(ft5x_ts->ts_workqueue, &ft5x_ts->pen_event_work,msecs_to_jiffies(50));
			retry_counter ++;
		}
	}
	//enable_irq(SW_INT_IRQNO_PIO);

}

static irqreturn_t ft5x_ts_interrupt(int irq, void *dev_id)
{
	struct ft5x_ts_data *ft5x_ts = dev_id;
		
	print_int_info("==========------ft5x_ts TS Interrupt-----============\n"); 
	if(!ctp_ops.judge_int_occur()){
		print_int_info("==IRQ_EINT%d=\n",CTP_IRQ_NO);
		ctp_ops.clear_penirq();
		if (!delayed_work_pending(&ft5x_ts->pen_event_work))
		{
			print_int_info("Enter work %i\n",this_client->dev.power.is_suspended);
			if (this_client->dev.power.is_suspended)
				ft5x_ts->defered_read = true;
			queue_delayed_work(ft5x_ts->ts_workqueue, &ft5x_ts->pen_event_work,0);
		}
	}else{
		print_int_info("Other Interrupt\n");
		return IRQ_NONE;
	}

	return IRQ_HANDLED;
}

#if defined(CONFIG_PM)

extern suspend_state_t g_suspend_state;
extern __u32 standby_wakeup_event;

static int ft5x_ts_suspend(struct i2c_client *client, pm_message_t mesg) {

	struct ft5x_ts_data *ft5x_ts = i2c_get_clientdata(client);
	//printk("[%s] msg = %i\n",__func__,g_suspend_state);

	if (g_suspend_state == PM_SUSPEND_PARTIAL && !ft5x_ts->locked) {
		standby_wakeup_event |= SUSPEND_WAKEUP_SRC_PIO;
	} else {
		ft5x_set_reg(FT5X0X_REG_PMODE, PMODE_HIBERNATE);
		ft5x_ts->in_suspend |= SUSPEND_MODE;
		if (g_suspend_state == PM_SUSPEND_MEM)
			ft5x_ts->in_suspend |= POWEROFF_MODE;
	}
	return 0;
}

static int ft5x_ts_resume(struct i2c_client *client) {

	struct ft5x_ts_data *ft5x_ts = i2c_get_clientdata(client);
	//printk("[%s] in suspend = %i\n",__func__,ft5x_ts->in_suspend);
	if (ft5x_ts->in_suspend & POWEROFF_MODE) { //reconfigure touch chip
		ft5x02_Init_IC_Param(this_client);
		msleep(50);
		fts_register_write(0, 0x40);
		msleep(25);
		fts_register_write(0, 0);
		msleep(25);
	} else if (ft5x_ts->in_suspend & SUSPEND_MODE)	{
		ctp_ops.ts_reset();
	}
	ft5x_ts->in_suspend = 0;
	return 0;
}
#endif

static int proc_keylock_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	struct ft5x_ts_data *ft5x_ts = (struct ft5x_ts_data *)data;
	*eof = 1;
	if (ft5x_ts)
		return snprintf(page, PAGE_SIZE, "%u\n", ft5x_ts->locked ? 1 : 0);
	else
		return snprintf(page, PAGE_SIZE, "ERROR\n");
}

static int proc_keylock_write(struct file *file, const char __user *buffer, unsigned long count, void *data)
{
	char buf[16];
	struct ft5x_ts_data *ft5x_ts = (struct ft5x_ts_data *)data;

	if (count > sizeof(buf) -1 )
		return -EINVAL;

	if (!count)
		return 0;

	if (copy_from_user(buf, buffer, count))
		return -EFAULT;

	buf[count] = '\0';

	switch (buf[0]) {
		case '0':
			if (ft5x_ts)
				ft5x_ts->locked = false;
			break;
		default:
			if (ft5x_ts)
				ft5x_ts->locked = true;;
			break;
	}

	return count;
}

static int ft5x_ts_lock_init(struct ft5x_ts_data *ft5x_ts)
{
	struct proc_dir_entry *dir, *file;

	dir = proc_mkdir("keylock", NULL);
	if (!dir) {
		printk("could not create /proc/keylock\n");
		return -1;
	}

	file = create_proc_entry("lock", S_IRUGO | S_IWUGO, dir);
	if (!file) {
		printk("could not create /proc/keylock/lock\n");
		return -1;
	}

	file->data = ft5x_ts;
	file->read_proc = proc_keylock_read;
	file->write_proc = proc_keylock_write;

	return 0;
}

static int 
ft5x_ts_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct ft5x_ts_data *ft5x_ts;
	struct input_dev *input_dev, *input_dev_slider;
	struct device *dev;
	struct i2c_dev *i2c_dev;
	int err = 0;

#ifdef TOUCH_KEY_SUPPORT
	int i = 0;
#endif

	pr_info("====%s begin=====.  \n", __func__);
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		err = -ENODEV;
		goto exit_check_functionality_failed;
	}


	ft5x_ts = kzalloc(sizeof(*ft5x_ts), GFP_KERNEL);
	if (!ft5x_ts)	{
		err = -ENOMEM;
		goto exit_alloc_data_failed;
	}

	//pr_info("touch panel gpio addr: = 0x%x", gpio_addr);
	this_client = client;
	
	//pr_info("ft5x_ts_probe : client->addr = %d. \n", client->addr);
	this_client->addr = client->addr;
	//pr_info("ft5x_ts_probe : client->addr = %d. \n", client->addr);
	i2c_set_clientdata(client, ft5x_ts);

//	pr_info("==INIT_WORK=\n");
	INIT_DELAYED_WORK(&ft5x_ts->pen_event_work, ft5x_ts_pen_irq_work);
	ft5x_ts->ts_workqueue = create_singlethread_workqueue(dev_name(&client->dev));
	if (!ft5x_ts->ts_workqueue) {
		err = -ESRCH;
		goto exit_create_singlethread;
	}

	input_dev = input_allocate_device();
	if (!input_dev) {
		err = -ENOMEM;
		dev_err(&client->dev, "failed to allocate touch input device\n");
		goto exit_input_dev_alloc_failed;
	}
	
	ft5x_ts->input_dev = input_dev;

#ifdef CONFIG_FT5X0X_MULTITOUCH
	set_bit(ABS_MT_TOUCH_MAJOR, input_dev->absbit);
	set_bit(ABS_MT_POSITION_X, input_dev->absbit);
	set_bit(ABS_MT_POSITION_Y, input_dev->absbit);
	set_bit(ABS_MT_WIDTH_MAJOR, input_dev->absbit);	
#ifdef FOR_TSLIB_TEST
	set_bit(BTN_TOUCH, input_dev->keybit);
#endif
	input_set_abs_params(input_dev,
			     ABS_MT_POSITION_X, 0, SCREEN_MAX_X, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_MT_POSITION_Y, 0, SCREEN_MAX_Y, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_MT_TOUCH_MAJOR, 0, PRESS_MAX, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_MT_WIDTH_MAJOR, 0, 200, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_MT_TRACKING_ID, 0, 4, 0, 0);
#ifdef TOUCH_KEY_SUPPORT
	key_tp = 0;
	input_dev->evbit[0] = BIT_MASK(EV_KEY);
	for (i = 1; i < TOUCH_KEY_NUMBER; i++)
		set_bit(i, input_dev->keybit);
#endif
#else
	set_bit(ABS_X, input_dev->absbit);
	set_bit(ABS_Y, input_dev->absbit);
	set_bit(ABS_PRESSURE, input_dev->absbit);
	set_bit(BTN_TOUCH, input_dev->keybit);
	input_set_abs_params(input_dev, ABS_X, 0, SCREEN_MAX_X, 0, 0);
	input_set_abs_params(input_dev, ABS_Y, 0, SCREEN_MAX_Y, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_PRESSURE, 0, PRESS_MAX, 0 , 0);
#endif

	set_bit(EV_ABS, input_dev->evbit);
	set_bit(EV_KEY, input_dev->evbit);

	input_dev->name		= CTP_NAME;		//dev_name(&client->dev)
	err = input_register_device(input_dev);
	if (err) {
		dev_err(&client->dev,
		"ft5x_ts_probe: failed to register input device: %s\n",
		dev_name(&client->dev));
		goto exit_input_register_device_failed;
	}

#ifdef CONFIG_FT5X0X_MULTITOUCH
	pr_info("CONFIG_FT5X0X_MULTITOUCH is defined. \n");
#endif
#ifdef CONFIG_FT5X0X_SLIDER
	//allocate slider input device
	input_dev_slider = input_allocate_device();
	if (!input_dev_slider) {
		err = -ENOMEM;
		dev_err(&client->dev, "failed to allocate slider input device\n");
		goto exit_slider_input_dev_alloc_failed;
	}

	ft5x_ts->input_dev_slider = input_dev_slider;
	set_bit(EV_ABS, input_dev_slider->evbit);
	//set_bit(ABS_X, input_dev_slider->absbit);

	//touch screen is rotated for 90 degrees;
	//X and Y are swapped.
	input_set_abs_params(input_dev_slider, ABS_X, 0, SCREEN_MAX_Y, 0, 0);

	input_dev_slider->name = CTP_NAME"_slider";
	//input_dev_slider->phys = "input/slider0";

	err = input_register_device(input_dev_slider);
	if (err) {
		dev_err(&client->dev,"ft5x_ts_probe: failed to register slider input device: %s\n",
		dev_name(&client->dev));
		goto exit_slider_input_register_device_failed;
	}
	//finish slider device register
#endif

	err = ctp_ops.set_irq_mode("ctp_para", "ctp_int_port", CTP_IRQ_MODE);
	if(0 != err){
		pr_info("%s:ctp_ops.set_irq_mode err. \n", __func__);
		goto exit_set_irq_mode;
	}

	err = request_irq(SW_INT_IRQNO_PIO, ft5x_ts_interrupt, IRQF_SHARED, "ft5x_ts", ft5x_ts);
	if (err < 0) {
		dev_err(&client->dev, "ft5x_ts_probe: request irq failed\n");
		goto exit_irq_request_failed;
	}

	if (gpio_int_hdle) {
		//enable gpio eint trigger;
		sw_gpio_eint_set_enable(gpio_int_hdle,1);

		//clear trigger state
		sw_gpio_eint_clr_irqpd_sta(gpio_int_hdle);
	}

	i2c_dev = get_free_i2c_dev(client->adapter);
	if (IS_ERR(i2c_dev)){	
		err = PTR_ERR(i2c_dev);		
		return err;	
	}
	dev = device_create(i2c_dev_class, &client->adapter->dev, MKDEV(I2C_MAJOR,client->adapter->nr), NULL, "aw_i2c_ts%d", client->adapter->nr);	
	if (IS_ERR(dev))	{		
			err = PTR_ERR(dev);		
			return err;	
	}

	pr_info("[%s] start init chip\n", __func__);
	int ret = 0; uint8_t v = 0;
	ret = ft5x02_Init_IC_Param(this_client);
	//pr_info("[%s] init return = %i\n", __func__,ret);
	msleep(50);
#ifndef CONFIG_TOUCHSCREEN_FT5X_TS_PB_REDUCED_INIT
	ret = ft5x02_write_reg(this_client,0,0x40);
	//fts_register_read(0, &v,1);
	//pr_info("[%s] turn on factory mode; return = %i; read = %i\n", __func__,ret,v);
	msleep(150);
	ret = ft5x02_write_reg(this_client,0,0);
	//fts_register_read(0, &v,1);
	//pr_info("[%s] turn on work mode; return = %i; read = %i\n", __func__,ret,v);
	msleep(150);
#endif	

	//on some devices need to change state from work mode to factory and backward the two times
	//The ft5x02_get_ic_param() do this changing. Also it may be used as debug func.
	ft5x02_get_ic_param(this_client);
	pr_info("[%s] finish init chip\n", __func__);
	
	ft5x_ts_lock_init(ft5x_ts);

	return 0;

exit_irq_request_failed:
exit_set_irq_mode:
	cancel_delayed_work_sync(&ft5x_ts->pen_event_work);
	destroy_workqueue(ft5x_ts->ts_workqueue);
	enable_irq(SW_INT_IRQNO_PIO);
#ifdef CONFIG_FT5X0X_SLIDER
exit_slider_input_register_device_failed:
	input_free_device(input_dev_slider);
exit_slider_input_dev_alloc_failed:
#endif
exit_input_register_device_failed:
	input_free_device(input_dev);
exit_input_dev_alloc_failed:
	free_irq(SW_INT_IRQNO_PIO, ft5x_ts);
exit_create_singlethread:
	pr_info("==singlethread error =\n");
	i2c_set_clientdata(client, NULL);
	kfree(ft5x_ts);
exit_alloc_data_failed:
exit_check_functionality_failed:
	return err;
}

static int __devexit ft5x_ts_remove(struct i2c_client *client)
{

	struct ft5x_ts_data *ft5x_ts = i2c_get_clientdata(client);
	ft5x_set_reg(FT5X0X_REG_PMODE, PMODE_HIBERNATE);
	
	pr_info("==ft5x_ts_remove=\n");
	free_irq(SW_INT_IRQNO_PIO, ft5x_ts);
	input_unregister_device(ft5x_ts->input_dev);
	input_free_device(ft5x_ts->input_dev);
	cancel_delayed_work_sync(&ft5x_ts->pen_event_work);
	destroy_workqueue(ft5x_ts->ts_workqueue);
	kfree(ft5x_ts);
    
	i2c_set_clientdata(client, NULL);
	ctp_ops.free_platform_resource();

	return 0;

}

static const struct i2c_device_id ft5x_ts_id[] = {
	{ CTP_NAME, 0 },
	{}
};
MODULE_DEVICE_TABLE(i2c, ft5x_ts_id);

static struct i2c_driver ft5x_ts_driver = {
	.class = I2C_CLASS_HWMON,
	.probe		= ft5x_ts_probe,
	.remove		= __devexit_p(ft5x_ts_remove),
	.id_table	= ft5x_ts_id,
	.driver	= {
		.name	= CTP_NAME,
		.owner	= THIS_MODULE,
	},
	.address_list	= u_i2c_addr.normal_i2c,
#ifdef CONFIG_PM
	.suspend	= ft5x_ts_suspend,
	.resume		= ft5x_ts_resume,
#endif
};

//============================
static ssize_t ft5x_reg_show(struct class *dev, struct class_attribute *attr, char *buf);
static ssize_t ft5x_reg_store(struct class *dev, struct class_attribute *attr, const char *buf, size_t count);
//static DEVICE_ATTR(reg, S_IWUSR | S_IRUGO, ft5x_reg_show, ft5x_reg_store);
static const CLASS_ATTR(reg, S_IWUSR | S_IRUGO, ft5x_reg_show, ft5x_reg_store);

static uint8_t ft5x_reg_addr = 0;
static ssize_t ft5x_reg_show(struct class *dev,
		struct class_attribute *attr, char *buf)
{
    uint8_t val;
    fts_register_read(ft5x_reg_addr,&val,1);
	return sprintf(buf,"REG[%x]=%x\n",ft5x_reg_addr,val);
}

static ssize_t ft5x_reg_store(struct class *dev,
				struct class_attribute *attr, const char *buf, size_t count)
{
	int tmp = 0,l = 0;
	uint8_t val;
	l = strlen(buf);
	tmp = simple_strtoul(buf, NULL, 16);
	if( l < 4/*tmp < 256*/ )
		ft5x_reg_addr = tmp;
	else {
		val = tmp & 0x00FF;
		ft5x_reg_addr = (tmp >> 8) & 0x00FF;
		fts_register_write(ft5x_reg_addr,val);
	}
	return count;
}
//================================

static int __init ft5x_ts_init(void)
{ 
	int ret = -1;
	int err = -1;

	pr_info("===========================%s=====================\n", __func__);

	if (ctp_ops.fetch_sysconfig_para)
	{
		if(ctp_ops.fetch_sysconfig_para()){
			pr_info("%s: err.\n", __func__);
			return -1;
		}
	}
	pr_info("%s: after fetch_sysconfig_para:  normal_i2c: 0x%hx. normal_i2c[1]: 0x%hx \n", \
	__func__, u_i2c_addr.normal_i2c[0], u_i2c_addr.normal_i2c[1]);

	err = ctp_ops.init_platform_resource();
	if(0 != err){
	    pr_info("%s:ctp_ops.init_platform_resource err. \n", __func__);    
	}

	//reset
	ctp_ops.ts_reset();
	//wakeup
	ctp_ops.ts_wakeup();  
	
	ft5x_ts_driver.detect = ctp_ops.ts_detect;

	i2c_dev_class = class_create(THIS_MODULE,"aw_i2c_dev");
	if (IS_ERR(i2c_dev_class)) {		
		ret = PTR_ERR(i2c_dev_class);		
		class_destroy(i2c_dev_class);	
	}

	ret = class_create_file(i2c_dev_class,&class_attr_reg);
	if (ret < 0) {
		pr_info(KERN_ERR "%s:register reg_attr failed\n",__FILE__);
		class_remove_file(i2c_dev_class,&class_attr_reg);
	}

	ret = i2c_add_driver(&ft5x_ts_driver);

	return ret;
}

static void __exit ft5x_ts_exit(void)
{
	pr_info("==ft5x_ts_exit==\n");
	i2c_del_driver(&ft5x_ts_driver);
}

late_initcall(ft5x_ts_init);
module_exit(ft5x_ts_exit);

MODULE_AUTHOR("Dushes");
MODULE_DESCRIPTION("ft5202 TouchScreen driver");
MODULE_LICENSE("GPL");

