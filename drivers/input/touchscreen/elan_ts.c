#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <asm/irq.h>
#include <asm/io.h>

#include <mach/irqs.h>
#include <mach/system.h>
#include <mach/hardware.h>
#include <mach/sys_config.h>
#include "ctp_platform_ops.h"

#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/pm.h>
#include <linux/earlysuspend.h>
#endif

#define PRINT_SUSPEND_INFO

#include "gt813_ts.h"

#if GTP_ICS_SLOT_REPORT
#include <linux/input/mt.h>
#endif

//#define USE_TIMER
#undef  POLL_TIME
#define POLL_TIME  30

//#define ELAN_DEBUG
#ifdef ELAN_DEBUG
#define printk_d(fmt, args...)  printk("[elan_ts] " "%s(%d): " fmt "\n", __FUNCTION__, __LINE__, ##args)
#define printk_i(fmt, args...)  printk("[elan_ts] " "%s(%d): " fmt "\n", __FUNCTION__, __LINE__, ##args)
#else
#define printk_d(fmt, args...)
#define printk_i(fmt, args...)
#endif


//#define pr_info printk


struct elan_ts_data {
	struct i2c_client *client;
	struct input_dev *input_dev;

	struct hrtimer timer;
	struct work_struct  work;
	struct workqueue_struct *wq;

#ifdef CONFIG_HAS_EARLYSUSPEND	
    struct early_suspend early_suspend;
#endif
};

static struct elan_ts_data *g_elan_ts_data = NULL;
const char *elan_ts_name = "elan_ts";

#define ELAN_I2C_DEVICE_NAME    "elan_ts_d"
#define ELAN_INPUT_DEVICE_NAME  "gt813_ts_i"

#define ELAN_I2C_ADDR 		0x10
#define ELAN_TS_X_MAX 		800
#define ELAN_TS_Y_MAX 		600
#define IDX_PACKET_SIZE		8

enum {	
	hello_packet  = 0x55,
	idx_coordinate_packet 	= 0x5a,
	};
enum {idx_finger_state = 7,};



#define PRINT_POINT_INFO
#define PRINT_INT_INFO

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

///////////////////////////////////////////////
//specific tp related macro: need be configured for specific tp
#define CTP_IRQ_NO			(gpio_int_info[0].port_num)
#define CTP_IRQ_MODE			(NEGATIVE_EDGE)
#define CTP_NAME				ELAN_I2C_DEVICE_NAME
#define TS_RESET_LOW_PERIOD		(15)
#define TS_INITIAL_HIGH_PERIOD		(15)
#define TS_WAKEUP_LOW_PERIOD	(100)
#define TS_WAKEUP_HIGH_PERIOD	(100)
#define TS_POLL_DELAY			(10)	/* ms delay between samples */
#define TS_POLL_PERIOD			(10)	/* ms delay between samples */
#define SCREEN_MAX_HEIGHT		(screen_max_x)
#define SCREEN_MAX_WIDTH		(screen_max_y)
#define PRESS_MAX			(255)

static void* __iomem gpio_addr = NULL;
static int gpio_int_hdle = 0;
static int gpio_wakeup_hdle = 0;
static int gpio_reset_hdle = 0;
static int gpio_wakeup_enable = 1;
static int gpio_reset_enable = 1;
static user_gpio_set_t  gpio_int_info[1];

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


void eeprom_test(void);

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

	//get the input port state
	reg_val = readl(gpio_addr + PIOH_DATA);
	//printk("reg_val = %x\n",reg_val);
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
	//clear the IRQ_EINT29 interrupt pending
	//printk("clear pend irq pending\n");
	reg_val = readl(gpio_addr + PIO_INT_STAT_OFFSET);
	//writel(reg_val,gpio_addr + PIO_INT_STAT_OFFSET);
	//writel(reg_val&(1<<(IRQ_EINT21)),gpio_addr + PIO_INT_STAT_OFFSET);
	if((reg_val = (reg_val&(1<<(CTP_IRQ_NO))))){
		print_int_info("==CTP_IRQ_NO=\n");              
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
	pr_info("%s: config gpio to int mode. \n", __func__);
#ifndef SYSCONFIG_GPIO_ENABLE
#else
	if(gpio_int_hdle){
		gpio_release(gpio_int_hdle, 2);
	}
	gpio_int_hdle = gpio_request_ex(major_key, subkey);
	if(!gpio_int_hdle){
		pr_info("request tp_int_port failed. \n");
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

#if 0
	if(strcmp(CTP_NAME, name)){
		pr_err("%s: name %s does not match CTP_NAME.  %s \n", __func__, name, CTP_NAME);
		pr_err(CTP_NAME);
		//ret = 1;
		return ret;
	}
#endif

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
		if(EGPIO_SUCCESS != gpio_write_one_pin_value(gpio_wakeup_hdle, 1, "ctp_wakeup")){
			printk("%s: err when operate gpio. \n", __func__);
		}
		mdelay(TS_WAKEUP_LOW_PERIOD);
		if(EGPIO_SUCCESS != gpio_write_one_pin_value(gpio_wakeup_hdle, 0, "ctp_wakeup")){
			printk("%s: err when operate gpio. \n", __func__);
		}
		mdelay(TS_WAKEUP_HIGH_PERIOD);

	}
	return;
}

static s8 gtp_i2c_test(struct i2c_client *client);
/**
 * ctp_detect - Device detection callback for automatic device creation
 * return value:  
 *                    = 0; success;
 *                    < 0; err
 */
int ctp_detect(struct i2c_client *client, struct i2c_board_info *info)
{
	struct i2c_adapter *adapter = client->adapter;

	printk_i("here twi_id=%d address=0x%02x", adapter->nr, client->addr);

	if(twi_id == adapter->nr) {
		if (gtp_i2c_test(client) >= 0) {
			printk("%s: Detected chip %s at adapter %d, address 0x%02x\n",
				__func__, CTP_NAME, i2c_adapter_id(adapter), client->addr);

			strlcpy(info->type, CTP_NAME, I2C_NAME_SIZE);
			return 0;
		} else {
			return -ENODEV;
		}
	} else {
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



static void elan_ts_power(struct elan_ts_data * ts, int on)
{
	if (on)
		gpio_write_one_pin_value(gpio_wakeup_hdle, 0, "ctp_wakeup");
	else
		gpio_write_one_pin_value(gpio_wakeup_hdle, 1, "ctp_wakeup");
}

#ifdef CONFIG_HAS_EARLYSUSPEND
static void elan_ts_suspend(struct early_suspend *h)
{
	int ret;
	struct elan_ts_data *ts = container_of(h, struct elan_ts_data, early_suspend);

	printk_i("here");

	ret = cancel_work_sync(&ts->work);	
	elan_ts_power(ts, 0);
}

static void elan_ts_resume(struct early_suspend *h)
{
	struct elan_ts_data *ts = container_of(h, struct elan_ts_data, early_suspend);
    
	printk_i("here");

	elan_ts_power(ts, 1);
}
#else
#ifdef CONFIG_PM
static int elan_ts_suspend(struct i2c_client *client, pm_message_t mesg)
{
	int ret;
	struct elan_ts_data *ts = i2c_get_clientdata(client);
	
	printk_i("here");

	ret = cancel_work_sync(&ts->work);	
	elan_ts_power(ts, 0);

	return 0;
}


static int elan_ts_resume(struct i2c_client *client)
{
	int ret;
	struct elan_ts_data *ts = i2c_get_clientdata(client);
	
	printk_i("here");

	elan_ts_power(ts, 1);
	return 0;
}
#endif
#endif


s32 elan_i2c_read(struct i2c_client *client, u8 *buf, s32 len)
{
    struct i2c_msg msgs[1];
    s32 ret = -1;
   // s32 retries = 0;

    msgs[0].flags = I2C_M_RD;
    msgs[0].addr  = 0x10; //client->addr;
    msgs[0].len   = len;
    msgs[0].buf   = buf;

  //  while(retries < 5) {
        ret = i2c_transfer(client->adapter, msgs, 1);
  //      if (ret == 1)
  //			break;
  //      retries++;
  //  }
    return ret;
}



static int elan_touch_recv_data(struct i2c_client *client, uint8_t *buf)
{	
	int rc, bytes_to_recv = IDX_PACKET_SIZE;
	if (buf == NULL)
		return -EINVAL;	

	memset(buf, 0, bytes_to_recv);
	rc = elan_i2c_read(client, buf, bytes_to_recv);

//	printk_d("elan_i2c_read rc = %d buf[0] =0x%x buf[7] = 0x%x", rc, buf[0], buf[7]);

	if (rc < 0) {	
		return -EINVAL;	
	}	
	return rc;
}



static s8 gtp_i2c_test(struct i2c_client *client)
{
	uint8_t buf[IDX_PACKET_SIZE] = {0};	
    u8 retry = 0;
    s8 ret = -1;
  
    while(retry++ < 2)
    {
        ret = elan_touch_recv_data(client, buf);
        if (ret > 0)
        {
            return ret;
        }
        printk_i("i2c test failed time %d.",retry);
        msleep(10);
    }
    return ret;
}




static inline int elan_touch_parse_xy(uint8_t *data, uint16_t *x, uint16_t *y)
{	
	*x = (data[0] & 0xf0);
	*x <<= 4;
	*x |= data[1];
	*y = (data[0] & 0x0f);
	*y <<= 8;
	*y |= data[2];
	return 0;
}


static void elan_touch_report_data(struct elan_ts_data *ts, uint8_t *buf)
{	
	uint16_t x1, x2, y1, y2;		
	uint8_t finger_stat;	

//	printk_d("buf[0] = 0x%x", buf[0]);

	if (buf[0] != idx_coordinate_packet)
		return;

//	printk_d("buf[0]=0x%x buf[1]=0x%x buf[2]=0x%x buf[3]=0x%x", buf[0], buf[1], buf[2], buf[3]);
//	printk_d("buf[4]=0x%x buf[5]=0x%x buf[6]=0x%x buf[7]=0x%x", buf[4], buf[5], buf[6], buf[7]);

	finger_stat = (buf[idx_finger_state] & 0x06) >> 1;

	if (finger_stat == 0) {	
		if ((buf[idx_finger_state] == 1) && !(buf[1]|buf[2]|buf[3]|buf[4]|buf[5]|buf[6])) {
			input_report_key(ts->input_dev, BTN_TOUCH, 0);	
			input_report_key(ts->input_dev, BTN_2, 0);		
			input_sync(ts->input_dev);
		//	gtp_touch_up
			printk_d("touch up\n");	
		}
		return;
	} 
	
	if (finger_stat == 1) {			
		elan_touch_parse_xy(&buf[1], &x1, &y1);	
		x1 = x1 * 800 / 2048; 
		y1 = y1 * 600 / 2048; 	

		input_report_abs(ts->input_dev, ABS_X, x1);
		input_report_abs(ts->input_dev, ABS_Y, y1);
		input_report_key(ts->input_dev, BTN_TOUCH, 1);	
		input_report_key(ts->input_dev, BTN_2, 0);		
		input_sync(ts->input_dev);

		printk_d("x1=%d,y1=%d\n", x1, y1);	
		return;
	} 
	
	if (finger_stat == 2) {
		elan_touch_parse_xy(&buf[1], &x1, &y1);	
		x1 = x1 * 800 / 2048; 
		y1 = y1 * 600 / 2048; 	
		input_report_abs(ts->input_dev, ABS_X, x1);
		input_report_abs(ts->input_dev, ABS_Y, y1);
		input_report_key(ts->input_dev, BTN_TOUCH, 1);	
		elan_touch_parse_xy(&buf[4], &x2, &y2);	
		
		x2 = x2 * 800 / 2048; 
		y2 = y2 * 600 / 2048; 	
		input_report_abs(ts->input_dev, ABS_HAT0X, x2);	
		input_report_abs(ts->input_dev, ABS_HAT0Y, y2);
		input_report_key(ts->input_dev, BTN_2, 1);		
		input_sync(ts->input_dev);

		printk_d("x1=%d,y1=%d--x2=%d,y2=%d\n", x1, y1, x2, y2);	
		return;
	}		
}


static void elan_ts_work_func(struct work_struct *work)
{	
	int rc;	
	uint8_t buf[IDX_PACKET_SIZE] = {0};	
    struct elan_ts_data *ts = NULL;

//	printk_d("here");
	ts = container_of(work, struct elan_ts_data, work);

#ifdef USE_TIMER
	if (1) {
#else
	if (1) {
#endif
		rc = elan_touch_recv_data(ts->client, buf);
		if (rc < 0)
			return;	
		elan_touch_report_data(ts, buf);
	}
}


static enum hrtimer_restart elan_ts_timer_func(struct hrtimer *timer)
{
	struct elan_ts_data *ts = container_of(timer, struct elan_ts_data, timer);
	queue_work(ts->wq, &ts->work);
	hrtimer_start(&ts->timer, ktime_set(0, (POLL_TIME+6)*1000000), HRTIMER_MODE_REL);
	return HRTIMER_NORESTART;
}


static irqreturn_t goodix_ts_irq_handler(int irq, void *dev_id)
{
	struct elan_ts_data *ts = dev_id;	
	int reg_val;	

	printk_i("here");
	//clear the IRQ_EINT21 interrupt pending
	reg_val = readl(gpio_addr + PIO_INT_STAT_OFFSET);
	if (reg_val&(1<<(CTP_IRQ_NO))) {	
		printk_i("CTP_IRQ_NO");
		writel(reg_val&(1<<(CTP_IRQ_NO)),gpio_addr + PIO_INT_STAT_OFFSET);
		queue_work(ts->wq, &ts->work);
	} else {
	    return IRQ_NONE;
	}

	return IRQ_HANDLED;
}

void elan_ts_axp_irq(void)
{
	printk("elan_ts_axp_int \n");
	queue_work(g_elan_ts_data->wq, &g_elan_ts_data->work);
}

extern void axp_eanble_tp_irq(void (*tp_irq_proc)(void));

static int elan_ts_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct elan_ts_data *ts;
	int ret = 0;
	int err;

	printk_i("here");

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		dev_err(&client->dev, "System need I2C function.\n");
		ret = -ENODEV;
		goto err_check_functionality_failed;
	}
	
	ts = kzalloc(sizeof(*ts), GFP_KERNEL);
	if (ts == NULL) {
		ret = -ENOMEM;
		goto err_alloc_data_failed;
	}

	INIT_WORK(&ts->work, elan_ts_work_func);
	ts->client = client;
	i2c_set_clientdata(client, ts);
	
	ts->input_dev = input_allocate_device();
	if (ts->input_dev == NULL) {
		ret = -ENOMEM;
		dev_dbg(&client->dev,"Failed to allocate input device\n");
		goto err_input_dev_alloc_failed;
	}

	set_bit(EV_SYN, ts->input_dev->evbit);
	set_bit(EV_KEY, ts->input_dev->evbit);
	set_bit(BTN_TOUCH, ts->input_dev->keybit);	
	set_bit(BTN_2, ts->input_dev->keybit);		
	set_bit(EV_ABS, ts->input_dev->evbit);	
	set_bit(ABS_X, ts->input_dev->absbit);	
	set_bit(ABS_Y, ts->input_dev->absbit);	
	set_bit(ABS_HAT0X, ts->input_dev->absbit);	
	set_bit(ABS_HAT0Y, ts->input_dev->absbit);  
	input_set_abs_params(ts->input_dev, ABS_X, 0, ELAN_TS_X_MAX, 0, 0);	
	input_set_abs_params(ts->input_dev, ABS_Y, 0, ELAN_TS_Y_MAX, 0, 0);	
	input_set_abs_params(ts->input_dev, ABS_HAT0X, 0, ELAN_TS_X_MAX, 0, 0);
	input_set_abs_params(ts->input_dev, ABS_HAT0Y, 0, ELAN_TS_Y_MAX, 0, 0);	

	ts->input_dev->name = ELAN_INPUT_DEVICE_NAME; 
	ts->input_dev->id.bustype = BUS_I2C;
	ts->input_dev->id.vendor  = 0xDEAD;
	ts->input_dev->id.product = 0xBEEF;
	ts->input_dev->id.version = 0x1105;	

	ret = input_register_device(ts->input_dev);
	if (ret) {
		dev_err(&client->dev,"Unable to register %s input device\n", ts->input_dev->name);
		goto err_input_register_device_failed;
	}
	
	ts->wq = create_singlethread_workqueue("goodix_wq");
	if (!ts->wq) {
		printk(KERN_ALERT "Creat %s workqueue failed.\n", elan_ts_name);
		return -ENOMEM;
	}

	flush_workqueue(ts->wq);	
	msleep(30);	

#ifdef CONFIG_HAS_EARLYSUSPEND	
	printk_i("==register_early_suspend =\n");	
	ts->early_suspend.level   = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;	
	ts->early_suspend.suspend = elan_ts_suspend;
	ts->early_suspend.resume  = elan_ts_resume;	
	register_early_suspend(&ts->early_suspend);
#endif

	err = ctp_ops.set_irq_mode("ctp_para", "ctp_int_port",CTP_IRQ_MODE);
	if(0 != err){
		printk("%s:ctp_ops.set_irq_mode err. \n", __func__);
		goto exit_set_irq_mode;
	}
	
#ifndef USE_TIMER
	err =  request_irq(SW_INT_IRQNO_PIO, goodix_ts_irq_handler, IRQF_TRIGGER_RISING | IRQF_SHARED, client->name, ts);
	if (err < 0) {
		pr_info( "goodix_probe: request irq failed\n");
		goto exit_irq_request_failed;
	}
#endif

#ifdef USE_TIMER
//	hrtimer_init(&ts->timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
//	ts->timer.function = elan_ts_timer_func;
//	hrtimer_start(&ts->timer, ktime_set(1, 0), HRTIMER_MODE_REL);
#endif
	
	g_elan_ts_data = ts;
//	axp_eanble_tp_irq(elan_ts_axp_irq);
	printk_i("end");
	return 0;

exit_set_irq_mode:
#ifndef USE_TIMER
exit_irq_request_failed:
#endif
err_input_register_device_failed:
	input_free_device(ts->input_dev);
err_input_dev_alloc_failed:
	i2c_set_clientdata(client, NULL);
err_alloc_data_failed:
err_check_functionality_failed:
	return ret;
}


static int elan_ts_remove(struct i2c_client *client)
{
	struct elan_ts_data *ts = i2c_get_clientdata(client);

	dev_notice(&client->dev,"The driver is removing...\n");
	free_irq(SW_INT_IRQNO_PIO, ts);

	#ifdef CONFIG_HAS_EARLYSUSPEND	
		unregister_early_suspend(&ts->early_suspend);
	#endif
	
	flush_workqueue(ts->wq);
	if (ts->wq)
		destroy_workqueue(ts->wq);
	input_unregister_device(ts->input_dev);
	input_free_device(ts->input_dev);
	i2c_set_clientdata(ts->client, NULL);
	kfree(ts);

	return 0;
}


static const struct i2c_device_id elan_ts_id[] = {
	{ELAN_I2C_DEVICE_NAME, 0},
	{ }
};


static struct i2c_driver elan_ts_driver = {
	.class = I2C_CLASS_HWMON,
	.probe		= elan_ts_probe,
	.remove		= elan_ts_remove,
#ifdef CONFIG_HAS_EARLYSUSPEND
#else
#ifdef CONFIG_PM
	.suspend	= elan_ts_suspend,
	.resume		= elan_ts_resume,
#endif
#endif
	.id_table	= elan_ts_id,
	.driver = {
		.name  = ELAN_I2C_DEVICE_NAME,
		.owner = THIS_MODULE,
	},
	.address_list	= u_i2c_addr.normal_i2c,
};

//extern void eink_disp_display_logo()

//驱动加载函数
static int __devinit elan_ts_init(void)
{
	int ret = -1;
	int err = -1;

	printk_i("here");

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
	
	elan_ts_driver.detect = ctp_ops.ts_detect;

	ret = i2c_add_driver(&elan_ts_driver);

//	eink_disp_display_logo();

	printk_i("i2c_add_driver ret=%d", ret);
	return ret;
}

//驱动卸载函数
static void __exit elan_ts_exit(void)
{
	printk_i("here");

	i2c_del_driver(&elan_ts_driver);
	ctp_ops.free_platform_resource();
	
	return;
}

late_initcall(elan_ts_init);
module_exit(elan_ts_exit);

MODULE_DESCRIPTION("Elan Touchscreen Driver");
MODULE_LICENSE("GPL v2");


//i2c_driver  中名称要与 sys_config1.fex ctp_name = "gt813_ts_d" 一致
//ts->input_dev->name 名称要与 gt813_ts_i.idc 一致， 才不是鼠标 模式

//	sprintf(ts->phys, "input/gt813_ts_i");
//	ts->input_dev->name = ELAN_INPUT_DEVICE_NAME; //elan_ts_name;
