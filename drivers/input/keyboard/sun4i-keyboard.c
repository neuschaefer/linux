/*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
*
* Copyright (c) 2011
*
* ChangeLog
*
*
*/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/keyboard.h>
#include <linux/ioport.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/timer.h> 
#include <mach/sys_config.h>
#include <mach/gpio.h>
#include <linux/suspend.h>

#if defined(CONFIG_HAS_EARLYSUSPEND) || defined(CONFIG_PM)
#include <linux/pm.h>
#include <linux/earlysuspend.h>
#include <linux/power/aw_pm.h>
#endif
//#define  KEY_DEBUG
//#define  KEY_DEBUG_LEVEL2
//#define  PRINT_SUSPEND_INFO

#define INPUT_DEV_NAME	("sun4i-keyboard")

#define  KEY_MAX_CNT  		(13)
 
#define  KEY_BASSADDRESS	(0xf1c22800)
#define  LRADC_CTRL		(0x00)
#define  LRADC_INTC		(0x04)
#define  LRADC_INT_STA 		(0x08)
#define  LRADC_DATA0		(0x0c)
#define  LRADC_DATA1		(0x10)

#define  FIRST_CONCERT_DLY		(2<<24)
#define  CHAN				(0x3)
#define  ADC_CHAN_SELECT		(CHAN<<22)
#define  LRADC_KEY_MODE		(0)
#define  KEY_MODE_SELECT		(LRADC_KEY_MODE<<12)
#define  KEY_SINGLE_MODE_SELECT		(0x01<<12)
#define  LEVELB_VOL			(0<<4)

#define  LRADC_HOLD_EN		(1<<6)

#define  LRADC_SAMPLE_32HZ		(3<<2)
#define  LRADC_SAMPLE_62HZ		(2<<2)
#define  LRADC_SAMPLE_125HZ		(1<<2)
#define  LRADC_SAMPLE_250HZ		(0<<2)


#define  LRADC_EN			(1<<0)

#define  LRADC_ADC1_UP_EN		(1<<12)
#define  LRADC_ADC1_DOWN_EN		(1<<9)
#define  LRADC_ADC1_DATA_EN		(1<<8)

#define  LRADC_ADC0_UP_EN		(1<<4)
#define  LRADC_ADC0_DOWN_EN		(1<<1)
#define  LRADC_ADC0_DATA_EN		(1<<0)

#define  LRADC_ADC1_UPPEND		(1<<12)
#define  LRADC_ADC1_DOWNPEND	(1<<9)
#define  LRADC_ADC1_DATAPEND		(1<<8)


#define  LRADC_ADC0_UPPEND 		(1<<4)
#define  LRADC_ADC0_DOWNPEND	(1<<1)
#define  LRADC_ADC0_DATAPEND		(1<<0)

#define EVB
//#define CUSTUM
#define ONE_CHANNEL
#define MODE_0V2
//#define MODE_0V15
//#define TWO_CHANNEL
#ifdef MODE_0V2
//standard of key maping
//0.2V mode	 

#define REPORT_START_NUM			(2)
#define REPORT_KEY_LOW_LIMIT_COUNT		(1)
#define MAX_CYCLE_COUNTER			(100)
//#define REPORT_REPEAT_KEY_BY_INPUT_CORE
//#define REPORT_REPEAT_KEY_FROM_HW
#define INITIAL_VALUE				(0Xff)

static unsigned char keypad_mapindex[64] =
{
    0,0,0,0,0,               	   //key 0, 5¸ö£¬ 0-4
    1,1,1,1,1,1,1,1,1,1,           //key 1, 10¸ö£¬ 5-14
    2,2,2,2,2,2,2,2,2,2,           //key 2, 10¸ö£¬ 15-24
    3,3,3,                   	   //key 3, 3¸ö£¬ 25-27
    4,4,4,4,4,4,4,4,4,             //key 4, 8¸ö£¬ 28-36
    5,5,5,                   	   //key 5, 3¸ö£¬ 37-39
    6,6,6,6,6,6,6,          	   //key 6, 7¸ö£¬40-46
    7,7,7,7,7,7,7,7,7,7,7,7, 	   //key 7, 12¸ö£¬47-58
    8,8,8,8,8					   //key 8, 5¸ö£¬59-63
};
#endif
                        
#ifdef MODE_0V15
//0.15V mode
static unsigned char keypad_mapindex[64] =
{
	0,0,0,                      //key1
	1,1,1,1,1,                  //key2
	2,2,2,2,2,
	3,3,3,3,
	4,4,4,4,4,
	5,5,5,5,5,
	6,6,6,6,6,
	7,7,7,7,
	8,8,8,8,8,
	9,9,9,9,9,
	10,10,10,10,
	11,11,11,11,
	12,12,12,12,12,12,12,12,12,12 //key13
};
#endif

#ifdef EVB
static unsigned int sun4i_scankeycodes[KEY_MAX_CNT]=
{
	[0 ] = KEY_RESERVED,
	[1 ] = KEY_DOWN,
	[2 ] = KEY_UP,
	[3 ] = KEY_RESERVED,
	[4 ] = KEY_LEFT,
	[5 ] = KEY_RESERVED,
	[6 ] = KEY_ENTER,
	[7 ] = KEY_RIGHT,
	[8 ] = KEY_RESERVED,
	[9 ] = KEY_RESERVED,
	[10] = KEY_RESERVED,
	[11] = KEY_RESERVED,
	[12] = KEY_RESERVED,
};
#endif
#ifdef CONFIG_PM
struct dev_power_domain keyboard_pm_domain;
#else
#ifdef CONFIG_HAS_EARLYSUSPEND	
struct sun4i_keyboard_data {
    struct early_suspend early_suspend;
};
#endif
#endif

static volatile unsigned int key_val;
static struct input_dev *sun4ikbd_dev;
static unsigned char scancode;

static unsigned int key_reg;
static unsigned int key_int_reg;
static unsigned char suspend_flag = 0;
static unsigned char key_cnt = 0;
static unsigned char cycle_buffer[REPORT_START_NUM] = {0};
static unsigned char transfer_code = INITIAL_VALUE;

#ifdef CONFIG_PM
#else
#ifdef CONFIG_HAS_EARLYSUSPEND
static struct sun4i_keyboard_data *keyboard_data;
#endif
#endif

#if 1
//hxm add GPIO key

static int touchkey3=3;
static int touchkey4=4;

static u32 hdle2;
static u32 hdle3;
static bool gpio_press1 = false;
static bool gpio_press2 = false;

int gpioKey_to_Vomdebug = 0;
EXPORT_SYMBOL(gpioKey_to_Vomdebug);

int sw_gpio_read(u32 handle);

static irqreturn_t GPIOKEY_EINT_Handler2(int irq, void *dev_id)	
{
	int reg_val = 0;

	reg_val = sw_gpio_eint_get_irqpd_sta(hdle2);
	if (reg_val != 1)
		return IRQ_NONE;

	if (gpio_press1 == false) {
		sw_gpio_set_trigger(hdle2, TRIG_LEVL_HIGH);
		input_report_key(sun4ikbd_dev, KEY_BACK, 1);
		input_sync(sun4ikbd_dev);
		gpio_press1 = true;
	}
	else {
		sw_gpio_set_trigger(hdle2, TRIG_LEVL_LOW);
		input_report_key(sun4ikbd_dev, KEY_BACK, 0);
		input_sync(sun4ikbd_dev);
		gpio_press1 = false;
	}

	sw_gpio_eint_clr_irqpd_sta(hdle2);

	return IRQ_HANDLED;
}

static irqreturn_t GPIOKEY_EINT_Handler3(int irq, void *dev_id)
{
	int reg_val = 0;

   	reg_val = sw_gpio_eint_get_irqpd_sta(hdle3);
	if (reg_val != 1)
		return IRQ_NONE;

	if (gpio_press2 == false) {
		gpioKey_to_Vomdebug = 1;
		sw_gpio_set_trigger(hdle3, TRIG_LEVL_HIGH);
		input_report_key(sun4ikbd_dev, KEY_HOME, 1);
		input_sync(sun4ikbd_dev);
		gpio_press2 = true;
	}
	else {
		sw_gpio_set_trigger(hdle3, TRIG_LEVL_LOW);
		input_report_key(sun4ikbd_dev, KEY_HOME, 0);
		input_sync(sun4ikbd_dev);
		gpio_press2 = false;
	}

	sw_gpio_eint_clr_irqpd_sta(hdle3);

	return IRQ_HANDLED;
}

static int __init gpio_eint_test_init(void)
{
    int pending;

    //PG9
    hdle2 = sw_gpio_irq_request("GPIOKEY_para", "GPIO_KEY_THREE", TRIG_LEVL_LOW);
    if (!hdle2) {
        printk("request gpio irq failed\n");
        return -1;
    }
    
    msleep(1);

    sw_gpio_eint_set_enable(hdle2, 1);
    pending = sw_gpio_eint_get_irqpd_sta(hdle2);
    if (pending < 0) {
        printk("get irq pending failed\n");
    }
	sw_gpio_eint_clr_irqpd_sta(hdle2);


	 //PG10    
    hdle3 = sw_gpio_irq_request("GPIOKEY_para", "GPIO_KEY_FOUR", TRIG_LEVL_LOW);
    if (!hdle3) {
        printk("request gpio irq failed\n");
        return -1;
    }

    msleep(1);

    sw_gpio_eint_set_enable(hdle3, 1);
    pending = sw_gpio_eint_get_irqpd_sta(hdle3);
    if (pending < 0) {
        printk("get irq pending failed\n");
    }
	sw_gpio_eint_clr_irqpd_sta(hdle3);
	
    return 0;
}

static int __exit gpio_eint_test_exit(void)
{
	//printk("shy  exit [%s] called\n", __func__);

	sw_gpio_eint_set_enable(hdle2, 0);
	sw_gpio_irq_free(hdle2);

	sw_gpio_eint_set_enable(hdle3, 0);
	sw_gpio_irq_free(hdle3);

	return 0;
}

static int  GpioKey_probe()
{	
	       int err = -1;    
              int device_used = -1;		
		//printk("%s %s %d \n",__FILE__,__func__,__LINE__);
		if(SCRIPT_PARSER_OK != (err = script_parser_fetch("GPIOKEY_para", "GPIOKEY_used", &device_used, 1)))
		{	                
			pr_err("%s: hxm@@@ script_parser_fetch err.ret = %d. \n", __func__, err);
			return device_used;	
		}	
		if(1 == device_used){
			pr_err("%s: hxm GPIOKEY__used. \n",  __func__);	
		}	
		else{
			pr_err("%s: hxm GPIOKEY__unused. \n",  __func__); 
			return device_used ;	
		}	
//		elan_wq = create_singlethread_workqueue("elan_wq");
//		INIT_WORK(&Gpio_work, gpio_work_func);
		//

		gpio_eint_test_init();

		err = request_irq(SW_INT_IRQNO_PIO, GPIOKEY_EINT_Handler2, IRQF_SHARED  ,"gpiokey3", &touchkey3); 
		if (err < 0) {		
			printk( "GPIO KEY: request irq failed\n");
			return err;	
		}
		
		err = request_irq(SW_INT_IRQNO_PIO, GPIOKEY_EINT_Handler3, IRQF_SHARED  ,"gpiokey4", &touchkey4); 
		if (err < 0) {		
			printk( "GPIO KEY: request irq failed\n");
			return err;	
		}

		return 0;		
}
#endif

//end


#ifdef CONFIG_PM
static int sun4i_keyboard_suspend(struct device *dev)
{
#ifdef PRINT_SUSPEND_INFO
	printk("sun4i_keyboard_suspend enter\n");
#endif
	suspend_flag = 1;
	if (g_suspend_state == PM_SUSPEND_MEM)
		key_int_reg = readl(KEY_BASSADDRESS + LRADC_INTC);
	key_reg = readl(KEY_BASSADDRESS + LRADC_CTRL);
	writel(key_reg|KEY_SINGLE_MODE_SELECT,KEY_BASSADDRESS + LRADC_CTRL);

	if (g_suspend_state == PM_SUSPEND_PARTIAL) {
		standby_wakeup_event |= SUSPEND_WAKEUP_SRC_PIO | SUSPEND_WAKEUP_SRC_KEY;
	}

	return 0;
}

static int  sun4i_keyboard_resume(struct device *dev)
{
#ifdef PRINT_SUSPEND_INFO
	printk("sun4i_keyboard_resume enter\n");
#endif
	if (g_suspend_state == PM_SUSPEND_MEM)
		writel(key_int_reg, KEY_BASSADDRESS + LRADC_INTC);
	writel(key_reg, KEY_BASSADDRESS + LRADC_CTRL);

	suspend_flag = 0;

	return 0;
}

#else
//�����
#ifdef CONFIG_HAS_EARLYSUSPEND
static void sun4i_keyboard_suspend(struct early_suspend *h)
{
#ifdef PRINT_SUSPEND_INFO
	printk("sun4i_keyboard_suspend enter\n");
#endif
	suspend_flag = 1;
	key_reg = readl(KEY_BASSADDRESS + LRADC_CTRL);
	writel(key_reg|KEY_SINGLE_MODE_SELECT,KEY_BASSADDRESS + LRADC_CTRL);
	
	if (g_suspend_state == PM_SUSPEND_PARTIAL) {
		standby_wakeup_event |= SUSPEND_WAKEUP_SRC_KEY;
	}
	return ;
}

//�������
static void sun4i_keyboard_resume(struct early_suspend *h)
{
#ifdef PRINT_SUSPEND_INFO
	printk("sun4i_keyboard_resume enter\n");
#endif
	writel(key_reg,KEY_BASSADDRESS + LRADC_CTRL);

	return ; 
}
#endif
#endif

static bool key_pressed_in_suspend = false;

static irqreturn_t sun4i_isr_key(int irq, void *dummy)
{
	unsigned int  reg_val;
	int judge_flag = 0;
	int loop = 0;
	
	#ifdef KEY_DEBUG
	    printk("Key Interrupt\n");
  	#endif
	reg_val  = readl(KEY_BASSADDRESS + LRADC_INT_STA);
	//writel(reg_val,KEY_BASSADDRESS + LRADC_INT_STA);

	if(reg_val&LRADC_ADC0_DOWNPEND)
	{
		#ifdef KEY_DEBUG
		    printk("key down\n");
		#endif
	}
	
	if(reg_val&LRADC_ADC0_DATAPEND)
	{
		key_val = readl(KEY_BASSADDRESS+LRADC_DATA0);
		
		if(key_val < 0x3f)
		{

		cycle_buffer[key_cnt%REPORT_START_NUM] = key_val&0x3f;

		if((key_cnt + 1) < REPORT_START_NUM)
		{
			if(suspend_flag == 1){
#ifdef KEY_DEBUG_LEVEL2
				printk("-----report data: key_val :%8d scancode: %8d\n",\
				key_val, scancode);
#endif			
				scancode = keypad_mapindex[key_val&0x3f];
				input_report_key(sun4ikbd_dev, sun4i_scankeycodes[scancode], 1);
				input_sync(sun4ikbd_dev);
				key_pressed_in_suspend = true;
				suspend_flag = 0;
			}

			//do not report key message

		}else{
		   // printk("key_cnt = %d \n",key_cnt);
			//scancode = cycle_buffer[(key_cnt-2)%REPORT_START_NUM];
			if(cycle_buffer[(key_cnt - REPORT_START_NUM + 1)%REPORT_START_NUM] \
			== cycle_buffer[(key_cnt - REPORT_START_NUM + 2)%REPORT_START_NUM])
			{
			key_val = cycle_buffer[(key_cnt - REPORT_START_NUM + 1)%REPORT_START_NUM];
			scancode = keypad_mapindex[key_val&0x3f];
			judge_flag = 1;

			}  
#ifdef KEY_DEBUG
			printk("cycle_buffer[(key_cnt - 1)] = 0x%x,cycle_buffer[(key_cnt -  2)]) = 0x%x\n",cycle_buffer[(key_cnt -  1)],cycle_buffer[(key_cnt -  2)]);
#endif
			if((!judge_flag) && (cycle_buffer[(key_cnt - REPORT_START_NUM + 1)%REPORT_START_NUM] \
			== cycle_buffer[(key_cnt - REPORT_START_NUM + 2)%REPORT_START_NUM]))
			{

			key_val = cycle_buffer[(key_cnt - REPORT_START_NUM + 1)%REPORT_START_NUM];
			scancode = keypad_mapindex[key_val&0x3f];
			judge_flag = 1;
			                           
			} 
			if(1 == judge_flag)
			{
#ifdef KEY_DEBUG_LEVEL2
				printk("report data: key_val :%8d transfer_code: %8d , scancode: %8d\n",\
				key_val, transfer_code, scancode);
#endif

				if(transfer_code == scancode){
				//report repeat key value
#ifdef REPORT_REPEAT_KEY_FROM_HW
				input_report_key(sun4ikbd_dev, sun4i_scankeycodes[scancode], 0);
				input_sync(sun4ikbd_dev);
				input_report_key(sun4ikbd_dev, sun4i_scankeycodes[scancode], 1);
				input_sync(sun4ikbd_dev);
#else
				//do not report key value
#endif
				}else if(INITIAL_VALUE != transfer_code){                               
				//report previous key value up signal + report current key value down
				input_report_key(sun4ikbd_dev, sun4i_scankeycodes[transfer_code], 0);
				input_sync(sun4ikbd_dev);
				input_report_key(sun4ikbd_dev, sun4i_scankeycodes[scancode], 1);
				input_sync(sun4ikbd_dev);
				transfer_code = scancode;

				}else{
				//INITIAL_VALUE == transfer_code, first time to report key event
				input_report_key(sun4ikbd_dev, sun4i_scankeycodes[scancode], 1);
				input_sync(sun4ikbd_dev);
				transfer_code = scancode;
				}

			}

			}
			key_cnt++;
			if(key_cnt > 2 * MAX_CYCLE_COUNTER ){
			key_cnt -= MAX_CYCLE_COUNTER;
			}

		}
	}
        
	if(reg_val&LRADC_ADC0_UPPEND)
	{
		if(key_cnt > REPORT_START_NUM)
		{
			if(INITIAL_VALUE != transfer_code)
			{
#ifdef KEY_DEBUG_LEVEL2
			printk("report data: key_val :%8d transfer_code: %8d \n",key_val, transfer_code);
#endif
			input_report_key(sun4ikbd_dev, sun4i_scankeycodes[transfer_code], 0);
			input_sync(sun4ikbd_dev);
			}

		}else if( key_cnt >= REPORT_KEY_LOW_LIMIT_COUNT){   
			//rely on hardware first_delay work, need to be verified!
			if(cycle_buffer[0] == cycle_buffer[1]){
				key_val = cycle_buffer[0];
				scancode = keypad_mapindex[key_val&0x3f];
#ifdef KEY_DEBUG_LEVEL2
				printk("report data: key_val :%8d scancode: %8d \n",key_val, scancode);
#endif
				input_report_key(sun4ikbd_dev, sun4i_scankeycodes[scancode], 1);
				input_sync(sun4ikbd_dev);   
				input_report_key(sun4ikbd_dev, sun4i_scankeycodes[scancode], 0);
				input_sync(sun4ikbd_dev);  
			}

		}
		if (key_pressed_in_suspend) {
			input_report_key(sun4ikbd_dev, sun4i_scankeycodes[scancode], 0);
			input_sync(sun4ikbd_dev);
			key_pressed_in_suspend = false;
		}
#ifdef KEY_DEBUG
		printk("key up \n");
#endif

		key_cnt = 0;
		judge_flag = 0;
		transfer_code = INITIAL_VALUE;
		for(loop = 0; loop < REPORT_START_NUM; loop++)
		{
			cycle_buffer[loop] = 0; 
		}

	}
	
	writel(reg_val,KEY_BASSADDRESS + LRADC_INT_STA);
	return IRQ_HANDLED;
}


/*************************add led test**************************/
#include "../../video/sun5i/disp/OSAL/OSAL_Pin.h"
static int  Led_Init_Para()
{	
	 int err = -1;    
     int device_used = -1;		
	//printk("%s %s %d \n",__FILE__,__func__,__LINE__);
	if(SCRIPT_PARSER_OK != (err = script_parser_fetch("LED_para", "LED_used", &device_used, 1)))
	{	                
		pr_err("%s: hxm@@@ script_parser_fetch err.ret = %d. \n", __func__, err);
		return device_used;	
	}	
	if(1 == device_used){
		pr_err("%s: hxm GPIOKEY__used. \n",  __func__);	
	}	
	else{
		pr_err("%s: hxm GPIOKEY__unused. \n",  __func__); 
		return device_used ;	
	}	

	user_gpio_set_t gpio_info;
	__hdle gpio_hd;
    if(OSAL_Script_FetchParser_Data("LED_para", "GPIO_LED_PIN", (int *)&gpio_info,sizeof(user_gpio_set_t)/sizeof(int)) < 0)
    {
        __wrn("LED_para GPIO_LED_PIN fail\n");
        return -1;
    }
	gpio_hd = OSAL_GPIO_Request(&gpio_info,1);
	gpio_info.mul_sel = 1;
	gpio_info.pull = 1;
	gpio_info.drv_level = 1;
	gpio_info.data = 0;
	gpio_set_one_pin_status(gpio_hd,&gpio_info,"GPIO_LED_PIN",1);


	return gpio_hd;		
}

static int __init sun4ikbd_init(void)
{
	int i;
	int err =0;	

#ifdef KEY_DEBUG
	printk("sun4ikbd_init \n");
#endif
	//Led_Init_Para();

	sun4ikbd_dev = input_allocate_device();
	if (!sun4ikbd_dev) {
		printk(KERN_ERR "sun4ikbd: not enough memory for input device\n");
		err = -ENOMEM;
		goto fail1;
	}

	sun4ikbd_dev->name = INPUT_DEV_NAME;  
	sun4ikbd_dev->phys = "sun4ikbd/input0"; 
	sun4ikbd_dev->id.bustype = BUS_HOST;      
	sun4ikbd_dev->id.vendor = 0x0001;
	sun4ikbd_dev->id.product = 0x0001;
	sun4ikbd_dev->id.version = 0x0100;

#ifdef REPORT_REPEAT_KEY_BY_INPUT_CORE
	sun4ikbd_dev->evbit[0] = BIT_MASK(EV_KEY)|BIT_MASK(EV_REP);
	printk("REPORT_REPEAT_KEY_BY_INPUT_CORE is defined, support report repeat key value. \n");
#else
	sun4ikbd_dev->evbit[0] = BIT_MASK(EV_KEY);
#endif

	for (i = 0; i < KEY_MAX_CNT; i++)
		set_bit(sun4i_scankeycodes[i], sun4ikbd_dev->keybit);
	set_bit(KEY_BACK, sun4ikbd_dev->keybit);
	set_bit(KEY_HOME, sun4ikbd_dev->keybit);
	
#ifdef ONE_CHANNEL
	writel(LRADC_ADC0_DOWN_EN|LRADC_ADC0_UP_EN|LRADC_ADC0_DATA_EN,KEY_BASSADDRESS + LRADC_INTC);	
	writel(FIRST_CONCERT_DLY|LEVELB_VOL|KEY_MODE_SELECT|LRADC_HOLD_EN|ADC_CHAN_SELECT|LRADC_SAMPLE_125HZ|LRADC_EN,KEY_BASSADDRESS + LRADC_CTRL);
	//writel(FIRST_CONCERT_DLY|LEVELB_VOL|KEY_MODE_SELECT|ADC_CHAN_SELECT|LRADC_SAMPLE_62HZ|LRADC_EN,KEY_BASSADDRESS + LRADC_CTRL);

#else
#endif


	if (request_irq(SW_INT_IRQNO_LRADC, sun4i_isr_key, 0, "sun4ikbd", NULL)){
		err = -EBUSY;
		printk("request irq failure. \n");
		goto fail2;
	}

	err = input_register_device(sun4ikbd_dev);
	if (err)
		goto fail3;
#ifdef CONFIG_PM
	keyboard_pm_domain.ops.suspend = sun4i_keyboard_suspend;
	keyboard_pm_domain.ops.resume = sun4i_keyboard_resume;
	sun4ikbd_dev->dev.pwr_domain = &keyboard_pm_domain;
#else
#ifdef CONFIG_HAS_EARLYSUSPEND	
	printk("==register_early_suspend =\n");
	keyboard_data = kzalloc(sizeof(*keyboard_data), GFP_KERNEL);
	if (keyboard_data == NULL) {
		err = -ENOMEM;
		goto err_alloc_data_failed;
	}

	keyboard_data->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 3;	
	keyboard_data->early_suspend.suspend = sun4i_keyboard_suspend;
	keyboard_data->early_suspend.resume	= sun4i_keyboard_resume;
	register_early_suspend(&keyboard_data->early_suspend);
#endif
#endif
    GpioKey_probe();

	return 0;
#ifdef CONFIG_PM
#else
#ifdef CONFIG_HAS_EARLYSUSPEND
 err_alloc_data_failed:
#endif
#endif
 fail3:	
	free_irq(SW_INT_IRQNO_LRADC, sun4i_isr_key);
 fail2:	
	input_free_device(sun4ikbd_dev);
 fail1:
     ;
#ifdef KEY_DEBUG
	printk("sun4ikbd_init failed. \n");
#endif

 return err;
}

static void __exit sun4ikbd_exit(void)
{
#ifdef CONFIG_PM
#else
#ifdef CONFIG_HAS_EARLYSUSPEND	
	 unregister_early_suspend(&keyboard_data->early_suspend);	
#endif
#endif
	free_irq(SW_INT_IRQNO_LRADC, sun4i_isr_key);
	input_unregister_device(sun4ikbd_dev);
	gpio_eint_test_exit();
}

module_init(sun4ikbd_init);
module_exit(sun4ikbd_exit);


MODULE_AUTHOR(" <@>");
MODULE_DESCRIPTION("sun4i-keyboard driver");
MODULE_LICENSE("GPL");



