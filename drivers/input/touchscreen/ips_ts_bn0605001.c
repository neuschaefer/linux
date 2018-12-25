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
#include <linux/suspend.h>
#ifdef CONFIG_HAS_EARLYSUSPEND    
#include <linux/pm.h>    
#include <linux/earlysuspend.h>
#include <linux/power/aw_pm.h>
#endif

//#define inferpoint_debug
/*resolution definion according to touch screen */
#define CTP_IRQ_NO			(gpio_int_info[0].port_num)
#define CTP_IRQ_MODE			(NEGATIVE_EDGE)
#define CTP_NAME				DEVICE_NAME
#define TS_RESET_LOW_PERIOD		(15)
#define TS_INITIAL_HIGH_PERIOD		(15)
#define TS_WAKEUP_LOW_PERIOD	(100)
#define TS_WAKEUP_HIGH_PERIOD	(100)
#define TS_POLL_DELAY			(10)	
/* ms delay between samples */
#define TS_POLL_PERIOD			(10)	
/* ms delay between samples */
#define SCREEN_MAX_HEIGHT		800//(screen_max_x)
#define SCREEN_MAX_WIDTH		600//(screen_max_y)
#define PRESS_MAX				(255)

static void* __iomem gpio_addr = NULL;
static int gpio_int_hdle = 0;
//static int gpio_wakeup_hdle = 0;
//static int gpio_reset_hdle = 0;
static int gpio_wakeup_enable = 1;
static int gpio_reset_enable = 1;
static user_gpio_set_t  gpio_int_info[1];

static int screen_max_x = 0;
static int screen_max_y = 0;
static int revert_x_flag = 0;
static int revert_y_flag = 0;
static int exchange_x_y_flag = 0;
//static struct workqueue_struct *goodix_wq;

extern bool eink_update_flag;

static u16 up_pos_x = 0;
static u16 up_pos_y = 0;

static __u32 twi_addr = 0;
static __u32 twi_id = 0;
static int	int_cfg_addr[]={PIO_INT_CFG0_OFFSET,PIO_INT_CFG1_OFFSET,
			PIO_INT_CFG2_OFFSET, PIO_INT_CFG3_OFFSET};
/* Addresses to scan */
union{
	unsigned short dirty_addr_buf[2];
	const unsigned short normal_i2c[2];
}u_i2c_addr = {{0x00},};
static int reg_val;

#define TP_MAX_X 2200    //2047//    
#define TP_MAX_Y 1600	 //2047//  
#define DEVICE_NAME    "Goodix-TS"

//#define TIMER_READ_DATA

#ifdef TIMER_READ_DATA
static void touch_timer_handler (unsigned long irq);
#define SAMPLE_PERIOD  jiffies + HZ/10
static struct timer_list touch_timer =
		TIMER_INITIALIZER(touch_timer_handler, 1, 0);
#endif

static void ctp_clear_penirq(void);
static int ctp_judge_int_occur(void);

struct ts_data {
	u16 x1;
	u16 y1;
	u16 x2;
	u16 y2;	
};

struct inferpoint_ts_priv {
	struct ts_data data_to_xy;
	struct i2c_client *client;
	struct input_dev *input;
	struct delayed_work work;
	int irq;
};
unsigned short ss;
struct inferpoint_ts_priv *priv;

static int ctp_set_irq_mode(char *major_key , char *subkey, ext_int_mode int_mode);


/*******************************************************	

*********************************************************/
static int inferpoint_i2c_recv(struct i2c_client *client,char *dataBuffer,int data_lenth)
{
	int ret;
	u8 address = 0;
	struct i2c_msg msgs[] = {
	{
		.addr = client->addr,
		.flags = 0,
		.len = 1,
		.buf = &address,

	},
	{
		.addr =client->addr,
		.flags = 1,
		.len = data_lenth,
		.buf = dataBuffer,
	},
	};

	ret = i2c_transfer(client->adapter,msgs,2);
	if(ret<0)	
		pr_err("msg %s i2c read error: %d\n",__func__,ret);
			
	return ret;
}


//add 12.10

/*******************************************************	
???ܣ?
	   I2Cд??????
??????
	txdata: д????????
	para:	???ݳ???
	buf??	??????
return??
	ִ????Ϣ
*********************************************************/
static int inferpoint_i2c_txdata(struct i2c_client *client, char *txdata, int length)
{
	int ret;

	struct i2c_msg msg[] = {
		{
			.addr	= client->addr,
			.flags	= 0,
			.len	= length,
			.buf	= txdata,
		},
	};

	ret = i2c_transfer(client->adapter, msg, 1);
	
	if (ret < 0)
		pr_err("%s i2c write error: %d\n", __func__, ret);

	return ret;
}
/*******************************************************	
???ܣ?
	   дTP?Ĵ?????????
??????
	addr: ???ƼĴ?????ַ
	para:	??????????
	buf??	??????
return??
	ִ????Ϣ
*********************************************************/
static int inferpoint_set_reg(u8 addr, u8 para)
{
    u8 reg_buf[3];
    int ret = -1;

    reg_buf[0] = addr;
    reg_buf[1] = para;
    ret = inferpoint_i2c_txdata(priv->client, reg_buf, 2);
    if (ret < 0) {
        pr_err("write reg failed! %#x ret: %d", reg_buf[0], ret);
        return -1;
    }
    
    return 0;
}

//end
/*****************************************************

********************************************************/
static unsigned long transform_to_screen_x(unsigned long x)
{
	if(1==revert_x_flag)
		return ((TP_MAX_X-x)*SCREEN_MAX_HEIGHT)/TP_MAX_X;		
	else
		return (x*SCREEN_MAX_HEIGHT)/TP_MAX_X;
	
}
static unsigned long transform_to_screen_y(unsigned long y)
{
	if(1== revert_y_flag)
	return ((TP_MAX_Y-y)*SCREEN_MAX_WIDTH)/TP_MAX_Y;
	else
	return (y*SCREEN_MAX_WIDTH)/TP_MAX_Y;
	
}

/*******************************************************	
??
********************************************************/

static void inferpoint_ts_work(struct work_struct *work)
{
  //struct inferpoint_ts_priv *priv = container_of(work, struct inferpoint_ts_priv, work.work);
	//unsigned short x1pos, y1pos, x2pos, y2pos,x3pos,y3pos,x4pos,y4pos,x5pos,y5pos;
	struct ts_data *data = &priv->data_to_xy;
	unsigned char buf[9];
	unsigned char i;
	int ret;

	#ifdef inferpoint_debug
	printk("start inferpoint touchscreen 1.\n");

	#endif	

	memset(buf, 0, sizeof(buf));
   // printk("start inferpoint touchscreen 2.\n");
	ret = inferpoint_i2c_recv(priv->client,buf,sizeof(buf));
    //printk("start inferpoint touchscreen 3.\n");
	if (ret<0) {
	#ifdef inferpoint_debug
		printk(KERN_INFO"--------inferpoint_i2c_recv-------%d---.\n");
		dev_err(&priv->client->dev, "Unable to read i2c page\n");
	#endif	
		goto out;
	}
//	for(i=0;i<9;i++)
//	{
//		printk("the %dth  number is%d\n",i,buf[i]);
//	}
	
	if((buf[0]&0x1F) != 0)
	{
	//	printk("start inferpoint touchscreen 4.\n");
    //hxm add
   //   if(3!=g_pm_state)
	     {


			if((buf[0]&0x01) != 0)
			{
	//		   printk("start inferpoint touchscreen 5.\n");
				
				data->x1 = ((buf[1]&0x0f) << 8 | buf[2]);
				data->y1 = ((buf[3]&0x0f)  << 8 | buf[4]);
					
//				printk("TPx1=%d\n",data->x1);
//				printk("TPy1=%d\n",data->y1);
				
				data->x1 = transform_to_screen_x(data->x1);
				data->y1 = transform_to_screen_y(data->y1);
//				printk("data->x1=%d\n",data->x1);
//				printk("data->y1=%d\n",data->y1);
				input_report_abs(priv->input, ABS_MT_TOUCH_MAJOR, 8);//max(data->x1,data->y1));
				input_report_abs(priv->input, ABS_MT_TRACKING_ID,1);
				input_report_abs(priv->input, ABS_MT_POSITION_X, data->x1);
				input_report_abs(priv->input, ABS_MT_POSITION_Y, data->y1);			
				input_mt_sync(priv->input);
				
			}
			if((buf[0]&0x02) != 0)
			{
			
				data->x2 = ((buf[5]&0x0f) << 8 | buf[6]);
				data->y2 = ((buf[7]&0x0f)  << 8 | buf[8]);
	           	//printk("TPx2=%d\n",data->x2);
			//	printk("TPy2=%d\n",data->y2);
			   
				
					data->x2 = transform_to_screen_x(data->x2);
					data->y2 = transform_to_screen_y(data->y2);
			//		printk("data->x2=%d\n",data->x2);
			//	    printk("data->y2=%d\n",data->y2);
					input_report_abs(priv->input, ABS_MT_TOUCH_MAJOR, 8);//max(data->x2,data->y2));
					input_report_abs(priv->input, ABS_MT_TRACKING_ID,2);


					input_report_abs(priv->input, ABS_MT_POSITION_X, data->x2);

					input_report_abs(priv->input, ABS_MT_POSITION_Y, data->y2);
				
				
					input_mt_sync(priv->input);
				
			}
			input_sync(priv->input);

			
	    }

	
	}
	else
	{
//		printk("start inferpoint touchscreen 9.\n");
		input_report_abs(priv->input, ABS_MT_TOUCH_MAJOR, 0);
			//input_mt_slot(priv->input, 0);
			//input_mt_report_slot_state(priv->input, MT_TOOL_FINGER, false);
			//input_mt_slot(priv->input, 1);
			//input_mt_report_slot_state(priv->input, MT_TOOL_FINGER, false);
		input_mt_sync(priv->input);

		input_sync(priv->input);
	}


 out:
	//enable_irq(priv->irq);
	;
}





#ifdef TIMER_READ_DATA
static void touch_timer_handler (unsigned long irq) 
{  
//	printk("Jim_dream touch_timer_handler\n");
//	queue_work(goodix_wq, &priv->work);	
	schedule_delayed_work(&priv->work,0);
}
#endif

/*******************************************************	

********************************************************/
static irqreturn_t inferpoint_ts_isr(int irq, void *dev_id)
{
	
	if(!ctp_judge_int_occur()){
	   ctp_clear_penirq();	
//	int reg_val;	
//	reg_val = readl(gpio_addr + PIO_INT_STAT_OFFSET);    
//	if(reg_val&(1<<(CTP_IRQ_NO)))
//	{	
//		printk("==IRQ_EINT%d=\n",CTP_IRQ_NO);
//		writel(reg_val&(1<<(CTP_IRQ_NO)),gpio_addr + PIO_INT_STAT_OFFSET);
//      disable_irq_nosync(SW_INT_IRQNO_PIO);
//    }
//hxm add
         //if(3!=g_pm_state)
		    //{
		    //printk("inferpoint_ts_isr  g_pm_state=%d\n",g_pm_state);
    		schedule_delayed_work(&priv->work,0);
		    //}
			//else
			//printk("inferpoint_ts_isr  g_pm_state=%d\n",g_pm_state);
//end

	}
	else
	{
	    printk("Other Interrupt\n");
	    return IRQ_NONE;
	}
	
	return IRQ_HANDLED;
}

/*******************************************************	

********************************************************/
static int inferpoint_ts_remove(struct i2c_client *client)
{
	free_irq(priv->irq, priv);
	
	//flush_workqueue(goodix_wq);
	//if (goodix_wq)
		//destroy_workqueue(goodix_wq);

	input_unregister_device(priv->input);
	kfree(priv);
	dev_set_drvdata(&client->dev, NULL);

	return 0;
}
/*******************************************************	

********************************************************/

static int inferpoint_ts_suspend(struct i2c_client *client, pm_message_t mesg)
{
/*
    printk(KERN_INFO "start tttssss inferpoint_ts_suspend.\n");
	if (device_may_wakeup(&client->dev))
		enable_irq_wake(priv->irq);
*/	
	if (g_suspend_state != PM_SUSPEND_PARTIAL ){
		printk("go to  inferpoint_ts_suspend g_suspend_state=%d.\n",g_suspend_state);
	    inferpoint_set_reg(0x0d, 0x00);	  //turn off TP
		msleep(20); 
	}
	else{
	standby_wakeup_event |= SUSPEND_WAKEUP_SRC_PIO;
	}
		return 0;

}



/*******************************************************	

********************************************************/
static int inferpoint_ts_resume(struct i2c_client *client)
{
	int error;
/*
 printk(KERN_INFO "start tttsss inferpoint_ts_resume.\n");
	if (device_may_wakeup(&client->dev))
		disable_irq_wake(priv->irq);

*/	
 if (g_suspend_state != PM_SUSPEND_PARTIAL ) {	
 	printk("go to  inferpoint_ts_resume g_suspend_state=%d.\n",g_suspend_state);
	inferpoint_set_reg(0x0d, 0x80);	  //turn on TP
	msleep(20);
    	error = ctp_set_irq_mode("ctp_para", "ctp_int_port",CTP_IRQ_MODE);
	if(0 != error){
		printk(KERN_INFO "ctp_ops.set_irq_mode err.\n");
	}
 } else
	printk("not to  inferpoint_ts_resume.\n");
 
	return 0;

}



//MODULE_DEVICE_TABLE(i2c, inferpoint_ts);


//hxm add
/* * ctp_get_pendown_state  :get the int_line data state,  *  * return value: 
*             return PRESS_DOWN: if down 
*             return FREE_UP: if up, 
*             return 0: do not need process, equal free up. 
*/
static int ctp_get_pendown_state(void)
{
	unsigned int reg_val;
	static int state = FREE_UP;//get the input port state
	reg_val = readl(gpio_addr + PIOH_DATA);
	//printk("reg_val = %x\n",reg_val);
	if(!(reg_val & (1<<CTP_IRQ_NO))){
		state = PRESS_DOWN;
		printk("pen down. \n");
	}else{ //touch panelisfree up
	state =FREE_UP;
	printk("free up. \n");
	
}	
return state;
}

/*** ctp_clear_penirq - clear int pending**/
static void ctp_clear_penirq(void)
{
	int reg_val;
	//clear the IRQ_EINT29 interrupt pending
	//printk("clear pend irq pending\n");
	reg_val = readl(gpio_addr + PIO_INT_STAT_OFFSET);
	//writel(reg_val,gpio_addr + PIO_INT_STAT_OFFSET);
	//writel(reg_val&(1<<(IRQ_EINT21)),gpio_addr + PIO_INT_STAT_OFFSET);
	if((reg_val = (reg_val&(1<<(CTP_IRQ_NO))))){
//		printk("==CTP_IRQ_NO=\n");  	
    	writel(reg_val,gpio_addr + PIO_INT_STAT_OFFSET);
	}
	return;
}

/*** ctp_set_irq_mode - according sysconfig's subkey "ctp_int_port" to config int port.
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

/** * ctp_set_gpio_mode - a
ccording sysconfig's subkey "ctp_io_port" to config io port.
 *
 * return value:      0:      success;
 *                    others: fail; 
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
		ret= -1;
		goto request_tp_io_port_failed;
	}
#endif
	return ret;

request_tp_io_port_failed:
	return ret;
}

/**
 * ctp_judge_int_occur - whether interrupt
 occur.
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
 * ctp_free_platform_
resource - corresponding with ctp_init_platform_resource *
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

/*		
    if(gpio_wakeup_hdle){
		gpio_release(gpio_wakeup_hdle, 2);

	}
	if(gpio_reset_hdle){
		gpio_release(gpio_reset_hdle, 2);
	}
*/	
	return;
}


/**
 * ctp_init_platform_resource - initialize platform related resource
 * return value: 0 : success
 *               -EIO:  i/o err. 
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
/*
	gpio_wakeup_hdle = gpio_request_ex("ctp_para", "ctp_wakeup");
	if(!gpio_wakeup_hdle) {
		pr_warning("%s: tp_wakeuprequest gpio fail!\n", __func__);
		gpio_wakeup_enable = 0;
	}

	gpio_reset_hdle = gpio_request_ex("ctp_para", "ctp_reset");
	if(!gpio_reset_hdle){
		pr_warning("%s: tp_reset request gpio fail!\n", __func__);
		gpio_reset_enable = 0;
	}
*/
	return ret;
exit_ioremap_failed:
	ctp_free_platform_resource();
	return ret;
}


/**
 * ctp_fetch_sysconfig_para - get config info from sysconfig.fex file.
 * return value:  
 *                = 0; success;
 *                < 0; err
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
		pr_err("%s: ctp_unused. \n", __func__);
		return ret;
	}

	if(SCRIPT_PARSER_OK != script_parser_fetch_ex("ctp_para","ctp_name", (int *)(&name), &type, sizeof(name)/sizeof(int)))
	{		
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
		pr_err("inferpoint_ts: script_parser_fetch err. \n");
		goto script_parser_fetch_err;
	}
	pr_info("%s: exchange_x_y_flag = %d. \n", __func__, exchange_x_y_flag);

	return 0;

script_parser_fetch_err:	
	pr_notice("=========script_parser_fetch_err============\n");
	return ret;
}

/*** ctp_reset - function**/

static void ctp_reset(void)
{
	printk("%s. \n", __func__);
/*	
	if(gpio_reset_enable){
		
		if(EGPIO_SUCCESS != gpio_write_one_pin_value(gpio_reset_hdle, 0, "ctp_reset")){
			printk("%s: err when operate gpio. \n", __func__);
		}
		
		mdelay(TS_RESET_LOW_PERIOD);
		
		if(EGPIO_SUCCESS!= gpio_write_one_pin_value(gpio_reset_hdle, 1, "ctp_reset")){
			printk("%s: err when operate gpio. \n", __func__);
		}
		
		mdelay(TS_INITIAL_HIGH_PERIOD);
	}
*/	
}

/*** ctp_wakeup - function**/
static void ctp_wakeup(void)

{
	printk("%s. \n", __func__);
/*	
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
*/	
	return;
}

/**
 * ctp_detect - Device detection callback for automatic device creation
 * return value:  
 *                   = 0; success;
 *                   < 0; err
 */
int ctp_detect(struct i2c_client *client, struct i2c_board_info *info)
{
	struct i2c_adapter *adapter = client->adapter;

	if(twi_id == adapter->nr)
	{
		printk("%s: Detected chip %s at adapter %d, address 0x%02x\n",
			 __func__, CTP_NAME, i2c_adapter_id(adapter), client->addr);

		strlcpy(info->type, CTP_NAME, I2C_NAME_SIZE);
		return 0;
	}else{
		return -ENODEV;
	}
}

static struct ctp_platform_ops ctp_ops = 
{	.get_pendown_state = ctp_get_pendown_state,
	.clear_penirq	   = ctp_clear_penirq,
	.set_irq_mode      = ctp_set_irq_mode,	
	.set_gpio_mode     = ctp_set_gpio_mode,		
	.judge_int_occur   = ctp_judge_int_occur,	
	.init_platform_resource = ctp_init_platform_resource,	
	.free_platform_resource = ctp_free_platform_resource,	
	.fetch_sysconfig_para = ctp_fetch_sysconfig_para,	
//	.ts_reset =          ctp_reset,	
//	.ts_wakeup =         ctp_wakeup,	
	.ts_detect = 		ctp_detect,
};

/*******************************************************	

********************************************************/
static int inferpoint_ts_probe(struct i2c_client *client, const struct i2c_device_id *idp)
{
	struct input_dev *input;
	int error;
	
	#ifdef inferpoint_debug
	printk("start inferpoint touchscreen probe.\n");
	#endif

	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv) 
	{
		dev_err(&client->dev, "failed to allocate driver data\n");
		error = -ENOMEM;
		goto err0;
	}

	dev_set_drvdata(&client->dev, priv);

	input = input_allocate_device();
	if (!input) 
	{		dev_err(&client->dev, "Failed to allocate input device.\n");
		error = -ENOMEM;
		goto err1;
	}

	input->evbit[0] = BIT_MASK(EV_SYN) | BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS) ;

#if 0 //ndef GOODIX_MULTI_TOUCH	
	input->keybit[BIT_WORD(BTN_TOUCH)] = BIT_MASK(BTN_TOUCH);
	input->absbit[0] = BIT(ABS_X) | BIT(ABS_Y) | BIT(ABS_PRESSURE);
	input_set_abs_params(input, ABS_X, 0, SCREEN_MAX_HEIGHT, 0, 0);
	input_set_abs_params(input, ABS_Y, 0, SCREEN_MAX_WIDTH, 0, 0);
	input_set_abs_params(input, ABS_PRESSURE, 0, 255, 0, 0);	
	
#else
	set_bit(EV_ABS, input->evbit);

	input_set_abs_params(input, ABS_MT_TRACKING_ID, 0, 2, 0, 0);

	input_set_abs_params(input, ABS_MT_POSITION_X, 0, SCREEN_MAX_HEIGHT, 0, 0);

	input_set_abs_params(input, ABS_MT_POSITION_Y, 0, SCREEN_MAX_WIDTH, 0, 0);

	input_set_abs_params(input, ABS_MT_TOUCH_MAJOR, 0, 255, 0, 0);

	input_set_abs_params(input, ABS_MT_WIDTH_MAJOR, 0, 15, 0, 0);


   /*
	input->absbit[0] = BIT_MASK(ABS_MT_TRACKING_ID) |
			BIT_MASK(ABS_MT_TOUCH_MAJOR)| BIT_MASK(ABS_MT_WIDTH_MAJOR) |
  			BIT_MASK(ABS_MT_POSITION_X) | BIT_MASK(ABS_MT_POSITION_Y); 	// for android
	input_set_abs_params(input, ABS_MT_WIDTH_MAJOR, 0, 255, 0, 0);
	input_set_abs_params(input, ABS_MT_TOUCH_MAJOR, 0, 255, 0, 0);
	input_set_abs_params(input, ABS_MT_POSITION_X, 0, SCREEN_MAX_HEIGHT, 0, 0);
	input_set_abs_params(input, ABS_MT_POSITION_Y, 0, SCREEN_MAX_WIDTH, 0, 0);	
	input_set_abs_params(input, ABS_MT_TRACKING_ID, 0, 2, 0, 0);
   */
#endif
	
	set_bit(BTN_TOUCH, input->keybit);

	input->name = "gt80x";
	//input->phys = "input/goodix-ts";
	input->id.bustype = BUS_I2C;
	input->dev.parent = &client->dev;
	//input->name = client->name;
	printk("client->name=%s.\n",client->name);

	input_set_drvdata(input, priv);

	priv->client = client;
	priv->input  = input;

	INIT_DELAYED_WORK(&priv->work, inferpoint_ts_work);
	//priv->irq = client->irq;
	priv->irq  = SW_INT_IRQNO_PIO;
/*
	goodix_wq = create_singlethread_workqueue("goodix_wq");
	if (!goodix_wq) {
		printk(KERN_ALERT "Creat gt80x workqueue failed.\n");
		return -ENOMEM;
		
	}
	flush_workqueue(goodix_wq);	
*/
	error = input_register_device(input);
	if (error)
		goto err1;

    error = ctp_ops.set_irq_mode("ctp_para", "ctp_int_port",CTP_IRQ_MODE);
	if(0 != error){
		printk(KERN_INFO "ctp_ops.set_irq_mode err.\n");
		goto err2;
	}
	
	//reg_val = readl(gpio_addr + PIO_INT_CTRL_OFFSET);
    //reg_val &=~(1<<CTP_IRQ_NO);
    //writel(reg_val,gpio_addr + PIO_INT_CTRL_OFFSET);
//end

#ifdef TIMER_READ_DATA	

#else
	error = request_irq(SW_INT_IRQNO_PIO, inferpoint_ts_isr,IRQF_TRIGGER_FALLING|IRQF_TRIGGER_RISING | IRQF_SHARED, client->name, priv);//IRQF_TRIGGER_LOW,IRQF_TRIGGER_FALLING
	if (error) {
		dev_err(&client->dev, "Unable to request touchscreen IRQ.\n");
		goto err2;
	}
#endif

	device_init_wakeup(&client->dev, 1);

#ifdef TIMER_READ_DATA	
	mod_timer(&touch_timer,SAMPLE_PERIOD);
#endif

	return 0;

 err2:
	input_unregister_device(input);
	input = NULL; /* so we dont try to free it below */
 err1:
	input_free_device(input);
	kfree(priv);
 err0:
	dev_set_drvdata(&client->dev, NULL);
	return error;
}

//end

static const struct i2c_device_id inferpoint_ts_id[] = {
	{DEVICE_NAME, 0},	
    { }
};

static struct i2c_driver inferpoint_ts_driver = {
		.class = I2C_CLASS_HWMON,
		.probe		= inferpoint_ts_probe,
		.remove		= inferpoint_ts_remove,
	#ifndef CONFIG_HAS_EARLYSUSPEND
	#else
		.suspend	= inferpoint_ts_suspend,
		.resume		= inferpoint_ts_resume,
	#endif
		.id_table	= inferpoint_ts_id,
		.driver = {
			.name	= DEVICE_NAME,
			.owner  = THIS_MODULE,
		},
		.address_list	= u_i2c_addr.normal_i2c,
	};
/*******************************************************	

********************************************************/
static int __init inferpoint_ts_init(void)
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
//	ctp_ops.ts_reset();
	//wakeup
//	ctp_ops.ts_wakeup();
	
	inferpoint_ts_driver.detect = ctp_ops.ts_detect;

	ret = i2c_add_driver(&inferpoint_ts_driver);
	
	return ret;
}

/*******************************************************	

********************************************************/
static void __exit inferpoint_ts_exit(void)
{
	i2c_del_driver(&inferpoint_ts_driver);
	ctp_ops.free_platform_resource();
	return;
}

MODULE_DESCRIPTION("inferpoint Touchscreen driver");
MODULE_AUTHOR("devin <devin@inferpoint.com>");
MODULE_LICENSE("GPL");

module_init(inferpoint_ts_init);
module_exit(inferpoint_ts_exit);


