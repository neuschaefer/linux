/*
===============================================================================
FILE:        stm8.c
VERSION:     0.1
DESCRIPTION: STM8 driver for IP phones based on SPEAr Basic SoC

                                   * * * * *

                Copyright (C) 2008-2009 by STMicroelectronics,
              Computer Systems Division, VoIP Competence Center.

     This program is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
      the Free Software Foundation, either version 3 of the License, or
                     (at your option) any later version.

       This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
                 GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
     along with this program. If not, see <http://www.gnu.org/licenses/>.

                                   * * * * *

Compliant to requirements of IP Phone applications, based on SPEAr Basic SoC,
STM8 Micro Controller.
Designed to be used as dynamically loaded kernel module (LKM).

Supported devices:
/dev/stm8

general Kconfig options:

Changes are required to default /Makefile for building.
The following line must be appended to the list of existing drivers:

obj-$(CONFIG_STM8)	+= stm8.o

Modification History:
- 11/11/08: created first skeleton
===============================================================================
*/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h> 
#include <linux/slab.h>	//kmalloc,kfree
#include <linux/mutex.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include <linux/list.h>

#include "stm8_870E.h"

/******************************************************************************
                            GENERAL DEFINITIONS
******************************************************************************/
#define IRQ_STM8 11//why 28? //11 is right, GPIO irq

//#define STM8_DEBUG_IRQLINE
//#define STM8_DEBUG_READ_COMMAND

//struct mutex stw5094a_mutex;

static unsigned int stm8_int_counter = 0;

static u8* SPEARBASIC_GPIO_IC = 0;
static u8* SPEARBASIC_GPIO_MIS = 0;

extern struct mutex i2c_mutex;

/******************************************************************************
                             PRIVATE VARIABLES
******************************************************************************/


/******************************************************************************
    I2C CONTROLLER ACCESS ROUTINES - SPECIALIZED FOR IP PHONE APPLICATIONS
******************************************************************************/

/*
-------------------------------------------------------------------------------
PRIVATE: STM8_I2C_SetStanderedFrequency

Sets transfer frequency
-------------------------------------------------------------------------------
*/

void STM8_I2C_SetStanderedFrequency(void)
{
	int HighCount; 
	int LowCount;
	
	HighCount = (IC_CLK * MIN_SS_SCL_HIGHTIME) / NANO_TO_MICRO;
	I2C_set_SS_SCL_HCNT(HighCount);
	
	LowCount = (IC_CLK * MIN_SS_SCL_LOWTIME) / NANO_TO_MICRO;
	I2C_set_SS_SCL_LCNT(LowCount);
}

/*
-------------------------------------------------------------------------------
PRIVATE: STM8_I2C_Init

Initializes the given I2C controller by default settings
-------------------------------------------------------------------------------
*/

void STM8_I2C_Init(void)
{   
	/*
        I2C_Disable();
	
	I2C_set_CON(IC_CON_SPL | IC_CON_MM); // 7bit address, set standard speed mode, set master mode

        I2C_set_RX_TL(FIFO_SIZE);
        I2C_set_TX_TL(FIFO_SIZE);
		//I2C_set_TX_TL(0);
	
	STM8_I2C_SetStanderedFrequency();	
	
	I2C_set_INTR_MASK(IC_M_STOP_DET);

	I2C_TAR(STM8_I2C_DEVICE_ADDR_WRITE);	// set I2c device address 51 

	I2C_Enable();
	*/
	
	// reinitialize I2C interface
	//based on stw5490a I2C routines
	*IC_ENABLE = 0x0;
	*IC_CON = 0x03;//IC_CON_SPL | IC_CON_MM; // master mode, std speed, 7-bit addressing
	//trying with restart bit enabled
	//*IC_CON = 0x23;//it seems the same
	*IC_RX_TL = 0x7; //FIFO_SIZE; // maybe not needed
	*IC_TX_TL = 0x7; //FIFO_SIZE; // maybe not needed
	*IC_SS_SCL_HCNT = 1 * (IC_CLK * MIN_SS_SCL_HIGHTIME) / NANO_TO_MICRO;
	*IC_SS_SCL_LCNT = 1 * (IC_CLK * MIN_SS_SCL_LOWTIME) / NANO_TO_MICRO;
	*IC_INTR_MASK = 0x200;
	//trying disabling stop intr mask
	//*IC_INTR_MASK = 0x000;//seems the same
	*IC_SAR = 0x02;
	*IC_TAR = 0x51;
	*IC_ENABLE = 0x1;
	
}

/*
-------------------------------------------------------------------------------
PRIVATE: STM8_I2C_CheckStop

Checks for the generation of stop condition
-------------------------------------------------------------------------------
*/

void STM8_I2C_Stop_Detect(void)
{
	int ic_clr_stop_detect;

	while(!(*IC_RAW_INTR_STAT & IC_R_STOP_DET));
	ic_clr_stop_detect = *IC_CLR_STOP_DET;  //clear stop detected bit  
}

/*
-------------------------------------------------------------------------------
PRIVATE: STM8_read_register

Reads one register of STM8 with I2C control interface
-------------------------------------------------------------------------------
*/

char STM8_read_register(u16 register_to_read, u32 num_of_bytes, char* read_buffer)
{
	mutex_lock(&i2c_mutex);
	//printk("STM8_read_register: enter\n");

	char data_read = 0;
	int receive_bytes = 0;

	I2C_Disable();	
	I2C_TAR(STM8_I2C_DEVICE_ADDR_WRITE);
	I2C_Enable();

	*IC_CMD_DATA = (register_to_read & 0xFF00) >> 0x08;
	while (!(*IC_STATUS & IC_STATUS_TFNF));

	*IC_CMD_DATA = register_to_read & 0x00FF;
	while (!(*IC_STATUS &IC_STATUS_TFE));
	//STM8_I2C_Stop_Detect(); 

	*IC_CMD_DATA = IC_CMD;

	while (receive_bytes < num_of_bytes) {	
		while (!(*IC_STATUS & IC_STATUS_RFNE));
		*read_buffer = (unsigned char)(*IC_CMD_DATA);
		read_buffer++;
		receive_bytes++;
///for speed-up test		printk("Received byte %d\n", receive_bytes);
	};

	STM8_I2C_Stop_Detect(); 

///for speed-up test	printk("stm8 read register %x, read %d bytes, value: %02X\n",register_to_read, num_of_bytes, *read_buffer);

///for speed-up test	printk("STM8_read_register: exit\n");
	mutex_unlock(&i2c_mutex);

	//return data_read;
	return receive_bytes;
}

/*
-------------------------------------------------------------------------------
PRIVATE: STM8_write_register

Writes one register of STw5094A with I2C control interface
-------------------------------------------------------------------------------
*/

void STM8_write_register(u16 register_to_write, char data_to_write)
{
	unsigned int dummy = 0;
	unsigned int data[3] = {0,0,0};
	/*
	//mutex_lock(&stw5094a_mutex);
	printk("STM8_write_register: enter\n");

	I2C_Disable();	
	I2C_TAR(STM8_I2C_DEVICE_ADDR_WRITE);
	I2C_Enable();
 
	printk("STM8_write_register: sending register number\n");
	
	//we must set bit 8 of IC_CMD_DATA to 0 for writes
	
	*IC_CMD_DATA = (register_to_write & 0xFF00) >> 0x08;
	//while (!(*IC_STATUS & IC_STATUS_TFNF));
	////while (!(*IC_STATUS & IC_STATUS_TFE));

	
	// detect stop bit and clear bit

	//if (*IC_RAW_INTR_STAT & 0x20) { dummy = *IC_CLR_STOP_DET; }

	
	
	printk("STM8_write_register: sending offset\n");
	
	*IC_CMD_DATA = register_to_write & 0x00FF;
	//while (!(*IC_STATUS & IC_STATUS_TFNF));
	////while (!(*IC_STATUS & IC_STATUS_TFE));

	printk("STM8_write_register: writing\n");
	
	*IC_CMD_DATA = data_to_write & 0xFF;
	//while (!(*IC_STATUS &IC_STATUS_TFE));
	
	//printk("STM8_write_register: Stop detection\n");
	//STM8_I2C_Stop_Detect(); 
	
	*/

	
	//based on stw5490a routines
	// reinitialize I2C interface
	
	
	data[0] = (unsigned int)(register_to_write & 0xFF00) >> 0x08;
	data[1] = (unsigned int)register_to_write & 0x00FF;
	data[2] = (unsigned int)data_to_write;
	
	mutex_lock(&i2c_mutex);
	
	STM8_I2C_Init();
	
	// send register index to be written
	//printk("STM8_write_register: register index\n");

	*IC_CMD_DATA = (register_to_write & 0xFF00) >> 0x08;
	//*IC_CMD_DATA = data[0];

	// detect stop bit and clear bit

	if (*IC_RAW_INTR_STAT & 0x20) { dummy = *IC_CLR_STOP_DET; }
		 
	//while (!(*IC_STATUS & IC_STATUS_TFE)); // wait until TX buffer empty
	//*IC_CMD_DATA = data[1];
	//while (!(*IC_STATUS & IC_STATUS_TFE)); // wait until TX buffer empty
		 
	//while (!(*IC_STATUS & IC_STATUS_TFE)); // wait until TX buffer empty
	//*IC_CMD_DATA = data[2];
	//while (!(*IC_STATUS & IC_STATUS_TFE)); // wait until TX buffer empty

	 
	//the offset (new)
	//printk("STM8_write_register: offset\n");
	*IC_CMD_DATA = register_to_write & 0x00FF;
	
	// detect stop bit and clear bit (repeated)

	//if (*IC_RAW_INTR_STAT & 0x20) { dummy = *IC_CLR_STOP_DET; }
	
	// send data
	//printk("STM8_write_register: writing\n");
	//while (!(*IC_STATUS & IC_STATUS_TFE)); // wait until TX buffer empty
	*IC_CMD_DATA = data_to_write;
	//while (!(*IC_STATUS & IC_STATUS_TFE)); // wait until TX buffer empty
	
	
	
///for speed-up test
		printk("stm8: write register %x, register value %x\n",register_to_write, data_to_write);	

	mutex_unlock(&i2c_mutex);
///for speed-up test
		printk("STM8_write_register: exit\n");
}

/******************************************************************************
                            STM8 LOGICAL DEVICE
******************************************************************************/

/*
-------------------------------------------------------------------------------
PRIVATE: stm8_open

Invoked when the logical device is opened.
-------------------------------------------------------------------------------
*/

static int stm8_open (struct inode *inode, struct file *file)
{	
	printk("stm8_open: enter\n");	
	u8 temp = 0;

	mutex_lock(&i2c_mutex);

	//Initializing I2C Controllers with the default settings.
	STM8_I2C_Init();
	 
	mutex_unlock(&i2c_mutex);

	//FIXME It is better to do in module init?

	// moved to module init
	/*
	//Interrupt management
	//GPIODIR: set the direction of GPIO[1] (on PAD A7) to be IN 
	u8* SPEARBASIC_GPIO_DIR = ioremap(SPEARBASIC_START_APB_GPIO1 + 0x400,1);
	temp = ioread8(SPEARBASIC_GPIO_DIR);
	iowrite8(temp & 0xFD, SPEARBASIC_GPIO_DIR);  //GPIO1 to be IN
	
	//GPIOIS: level on GPIO1 is detected	
	u8* SPEARBASIC_GPIO_IS = ioremap(SPEARBASIC_START_APB_GPIO1 + 0x404,1);
	temp = ioread8(SPEARBASIC_GPIO_IS);
	iowrite8(temp | 0x02, SPEARBASIC_GPIO_IS);  //level triggered interrupt			

	//GPIOIEV: level on GPIO1 is detected	
	u8* SPEARBASIC_GPIO_IEV = ioremap(SPEARBASIC_START_APB_GPIO1 + 0x40C,1);
	temp = ioread8(SPEARBASIC_GPIO_IEV);
	iowrite8(temp & 0xFD, SPEARBASIC_GPIO_IEV);  //low level triggered

	//GPIOIE: enable interrupt triggring on GPIO1
	u8* SPEARBASIC_GPIO_IE = ioremap(SPEARBASIC_START_APB_GPIO1 + 0x410,1);
	temp = ioread8(SPEARBASIC_GPIO_IE);
	iowrite8(temp | 0x02, SPEARBASIC_GPIO_IE);  //GPIO1 can trigger interrupt

	//following block ??? cut & paste from stw5094a driver?
	//commented out
	
	//u16* SPEARBASIC_IT_MASK  = ioremap(0x50000054,2);   	
	//u16 temp2 = readl(SPEARBASIC_IT_MASK);
	//writel(temp2 | 0x0100, SPEARBASIC_IT_MASK); //enable IRQ1 on legacy GPIO
 	
	
	//clearing
	SPEARBASIC_GPIO_IC = ioremap(SPEARBASIC_START_APB_GPIO1 + 0x41C,1);
	*SPEARBASIC_GPIO_IC &= 0x2;
	*/
	
	//disable_irq(IRQ_STM8);
	//enable_irq(IRQ_STM8);//FIXME

	printk("stm8_open: exit\n");

	return 0;
}

/*
-------------------------------------------------------------------------------
PRIVATE: stm8_ioctl

Execute specified "cmd" using "arg" if applicable.
-------------------------------------------------------------------------------
*/

static int stm8_ioctl (struct inode *inode, struct file *file, unsigned int cmd, u_long arg)
{
	void __user *argp = (void __user *)arg;
	int __user *p = argp;
	int val = 0;
	
	unsigned int *b = p;

	struct list_head * i_list;
	struct list_head * temp;
	struct keyboard_touch_fifo *ops;
	struct keyboard_touch_event event;

	//printk("stm8_ioctl: enter\n");

	switch (cmd) {
		case STM8_GET_STATUS:
///for speed-up test			printk("stm8_ioctl: cmd = STM8_GET_STATUS\n");
			val = stm8_status;		
			return put_user(val, p);	
		case STM8_SET_POWER_DOWM_MODE:	// only active mode is supported in current version 
///for speed-up test			printk("stm8_ioctl: cmd = STM8_SET_POWER_DOWM_MODE, ");
			if (get_user(val, p))
				return -EFAULT;
///for speed-up test			printk("arg = %d\n", val);
			if (val != STM8_FLAG_POWER_DOWN_CONTROL_ACTIVE) val = STM8_FLAG_POWER_DOWN_CONTROL_ACTIVE;
			stm8_power_down_mode = val;
			STM8_write_register(POWER_DOWN_CONTROL_CTL, val);
			return put_user(val, p);
		case STM8_GET_POWER_DOWN_WAKEUPSOURCE:
///for speed-up test			printk("stm8_ioctl: cmd = STM8_GET_POWER_DOWN_WAKEUPSOURCE\n");
			val = stm8_power_down_wakeupsource;		
			return put_user(val, p);	
		case STM8_GET_TOUCH_KEYBOARD_FIFO_STATUS:
///for speed-up test			printk("stm8_ioctl: cmd = STM8_GET_TOUCH_KEYBOARD_FIFO_STATUS\n");
			list_for_each(i_list, &keyboard_touch_list) {
				ops = list_entry(i_list, struct keyboard_touch_fifo, list);
				//ops->current_status			
			}
			return put_user(val, p);
			
			
		case STM8_GET_NEXT_FIFO_EVENT:
		//blocking
			//while (list_empty(&keyboard_touch_list));
			if (list_empty(&keyboard_touch_list)) return -EFAULT;
			//list_for_each(i_list, &keyboard_touch_list) {
			
				ops = list_entry(keyboard_touch_list.next, struct keyboard_touch_fifo, list);
				event.current_status = ops->current_status;
				event.keyboard_code = ops->keyboard_code;
				//event.touchscreen_xy = ops->touchscreen_xy;
				event.ts_x_m = ops->ts_x_m;
				event.ts_x_l = ops->ts_x_l;
				event.ts_y_m = ops->ts_y_m;
				event.ts_y_l = ops->ts_y_l;
				list_del(keyboard_touch_list.next);
				//break;//
			//}
			//list_del(ops);
			//LIST_FIRST(keyboard_touch_list);
			//return put_user(event,p);
			return copy_to_user(argp, &event, sizeof(struct keyboard_touch_event));
			
		case STM8_SET_LED_MUTE:
			if (get_user(val, p))
				return -EFAULT;
			stm8_led_mute = val;
			if(stm8_led_mute == 0){
				STM8_GPIO_RESET(GPIO_LED_MUTE);
				//printk("Data address: 0x%08X\n", SPEARBASIC_GPIO_DATA);
				//*(unsigned int*)(SPEARBASIC_GPIO_DATA) |= 0x20;
			}else{
				STM8_GPIO_SET(GPIO_LED_MUTE);
				//printk("Data address: 0x%08X\n", SPEARBASIC_GPIO_DATA);
				//*(unsigned int*)(SPEARBASIC_GPIO_DATA) &= ~(0x20);
			}
			return put_user(val, p);
		
		case STM8_SET_LED_HEADSET:
			if (get_user(val, p))
				return -EFAULT;
			stm8_led_headset = val;
			if(stm8_led_headset == 0){
				STM8_GPIO_RESET(GPIO_LED_HEADSET);
				//STM8_GPIO_RESET(0x0);
			}else{
				STM8_GPIO_SET(GPIO_LED_HEADSET);
				//STM8_GPIO_SET(0x0);
			}
			return put_user(val, p);
		
		case STM8_SET_LED_HANDSFREE:
			if (get_user(val, p))
				return -EFAULT;
			stm8_led_handsfree = val;
			if(stm8_led_handsfree == 0){
				STM8_GPIO_RESET(GPIO_LED_HANDSFREE);
				//STM8_GPIO_RESET(0x1);
			}else{
				STM8_GPIO_SET(GPIO_LED_HANDSFREE);
				//STM8_GPIO_SET(0x1);
			}
			return put_user(val, p);

		case STM8_SET_BACKLIGHT:
			if (get_user(val, p))
				return -EFAULT;
			stm8_backlight = val;
			if(stm8_backlight == 0){
				STM8_GPIO_RESET(GPIO_BACKLIGHT);
				//STM8_GPIO_RESET(0x1);
			}else{
				STM8_GPIO_SET(GPIO_BACKLIGHT);
				//STM8_GPIO_SET(0x1);
			}
			return put_user(val, p);
			
		case STM8_GET_KEY_CODE:
			list_for_each_safe(i_list,temp, &keyboard_touch_list) {
				ops = list_entry(i_list, struct keyboard_touch_fifo, list);
				//ops->keyboard_code
				p = ops->keyboard_code;
				list_del(i_list);
				kfree(ops);
			}
			//printk("stm8_ioctl: cmd = STM8_GET_KEY_CODE\n");	
			return put_user(val, p);	
		case STM8_GET_TOUCH_CORDINATION:
///for speed-up test			printk("stm8_ioctl: cmd = STM8_GET_TOUCH_CORDINATION\n");	
			list_for_each_safe(i_list,temp,&keyboard_touch_list) {
				ops = list_entry(i_list, struct keyboard_touch_fifo, list);
				//ops->touchscreen_xy
				list_del(i_list);
				kfree(ops);
			}
			return put_user(val, p);	
		case STM8_SET_KEYBOARD_SETTING:
///for speed-up test			printk("stm8_ioctl: cmd = STM8_SET_KEYBOARD_SETTING, ");	
			if (get_user(val, p))
				return -EFAULT;
			printk("arg = %d\n", val);
			stm8_keyboard_setting = val;
			STM8_write_register(KEYBOARD_SETTING_CTL, val);
			return put_user(val, p);
		case STM8_SET_LED:
///for speed-up test			printk("stm8_ioctl: cmd = STM8_SET_LED, ");	
			if (get_user(val, p))
				return -EFAULT;
			printk("arg = %d\n", val);
			stm8_led = val;
			STM8_write_register(LED_CONTROL_CTL, val);
#ifdef STM8_DEBUG_IRQLINE
			STM8_write_register(0x1F01, 0x0);
#endif STM8_DEBUG_IRQLINE
			return put_user(val, p);

		case STM8_870E_SET_LED:
			if (get_user(val, p))
				return -EFAULT;
			printk("arg = %d\n", val);
			stm8_led = val;
			STM8_write_register(LED_CONTROL_870E_ONOFF, val);
#ifdef STM8_DEBUG_IRQLINE
			STM8_write_register(0x1F01, 0x0);
#endif STM8_DEBUG_IRQLINE
			return put_user(val, p);
			
#ifdef STM8_DEBUG_READ_COMMAND
			case STM8_READ_DBG:
			printk("stm8_ioctl: cmd = STM8_READ_DBG, ");	
			if (get_user(val, p))
				return -EFAULT;
			printk("arg = %d\n", val);
			//STM8_read_register( (u16*) p, ((char*) p)+2  , val);
			STM8_read_register(0x1E00, 1, &val);
			printk("\n");
			return put_user(val, p);
#endif
			
#ifdef STM8_DEBUG_WRITE_COMMAND
			case STM8_WRITE_DBG:
			printk("stm8_ioctl: cmd = STM8_WRITE_DBG, ");	
			if (get_user(val, p))
				return -EFAULT;
			printk("arg = %d\n", val);
			STM8_write_register( b,  b+1);
			return put_user(val, p);
#endif
			
		default:
			printk("stm8_ioctl: invalid cmd\n");
			return -EINVAL;
	}

	printk("stm8_ioctl: exit\n");

	return -EINVAL;	
}



/*
-------------------------------------------------------------------------------
PRIVATE: stm8_release

Invoked when the logical device is closed.
-------------------------------------------------------------------------------
*/

static int stm8_fasync (int fd, struct file *filp, int mode)

{
	return fasync_helper (fd, filp, mode, &stm8_dev.stm8_async_queue);
}

static int stm8_release (struct inode *inode, struct file *file)
{
	if (file->f_flags & FASYNC) {
		stm8_fasync(-1, file, 0);
	}

	return 0;
}

irqreturn_t stm8_interrupt(int irq,void *dev_id,struct pt_regs *regs)
{
	//IRQ debug, null pointer deref. //FIXME
	//disable_irq(IRQ_STM8);//FIXME
	stm8_int_counter++;
	
	//check legacy gpio register : GPIOMIS
	
	//moved to init
	//FIXME u8* SPEARBASIC_GPIO_MIS = ioremap(SPEARBASIC_START_APB_GPIO1 + 0x418,1);
	
	if ((*SPEARBASIC_GPIO_MIS & 0x02) == 0) return IRQ_NONE;

	//FSM to handle the interrupt event reported by STM8 
	char read_buffer [4];
	while (STM8_read_register(STATUS_CONTROL_STATUS, 1,read_buffer) !=1);
	//stm8_status = read_buffer[0] & 0xF8;//last 3 bits are reserved
	stm8_status = read_buffer[0];//no need to mask now

	/* //switch not usable if multiple event are present at same time
	switch (stm8_status) {
		case STM8_FLAG_STATUS_CONTROL_WAKEUP :
			while (STM8_read_register(POWER_DOWN_CONTROL_WAKEUP_SOURCE1, 1,read_buffer) !=1);
			stm8_power_down_wakeupsource = read_buffer[0];
			
			switch (stm8_power_down_wakeupsource) {
				case STM8_FLAG_POWER_DOWN_WAKEUPSOURCE1_HOOK:
				break;
				case STM8_FLAG_POWER_DOWN_WAKEUPSOURCE1_TOUCHSCREEN:
				break;
				case STM8_FLAG_POWER_DOWN_WAKEUPSOURCE1_KEYBOARD:
				break;
				case STM8_FLAG_POWER_DOWN_WAKEUPSOURCE1_POWER_ON:
				break;
				case STM8_FLAG_POWER_DOWN_WAKEUPSOURCE1_USBPLUG:
				break;
 			}
		break;
		case STM8_FLAG_STATUS_CONTROL_TOUCH_CALIBRATION:
		//TBD//FIXME !!!
		break;
		case STM8_FLAG_STATUS_CONTROL_GLOBAL_ERROR:
		break;
		case STM8_FLAG_STATUS_CONTROL_FIFO_NOT_EMPTY: {
			bool fifo_not_empty;
			u8 event_status;			

			do {
				while (STM8_read_register(TOUCH_KEYBOARD_FIFO_STATUS, 1,read_buffer) !=1);
				event_status = read_buffer[0];
				
				if(event_status & STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_NOT_EMPTY)
					fifo_not_empty = true;	//FIFO Not Empty indicates another event, waiting in buffer
				else fifo_not_empty = false;

				struct keyboard_touch_fifo *new_item = kmalloc(sizeof(struct keyboard_touch_fifo), GFP_KERNEL);
				if(!new_item)  return NULL;
				INIT_LIST_HEAD(&new_item->list);
				new_item->current_status = event_status;

				//differentiate current(oldest) event between keyboard and touch screen, press and release
				if(event_status & STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_KEYBOARD) {
					while (STM8_read_register(TOUCH_KEYBOARD_FIFO_DATA_FIELD, 1,read_buffer) != 1);
					new_item->keyboard_code = read_buffer[0];
					
					if(event_status & STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_PRESS);
					else if(event_status & STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_RELEASE);
				}	
				else if(event_status & STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_TOUCHSCREEN) {
					while (STM8_read_register(TOUCH_KEYBOARD_FIFO_DATA_FIELD, 4,read_buffer)!=4);
					new_item->touchscreen_xy = (u32)(read_buffer);

					if(event_status & STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_PRESS);
					else if(event_status & STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_RELEASE);
				}
				list_add_tail(&new_item->list,&keyboard_touch_list); 

				//release one event 
 				STM8_write_register(TOUCH_KEYBOARD_FIFO_CTL,STM8_FLAG_TOUCH_KEYBOARD_FIFO_CONTROL_RELEASE_FIFO);
			} while (fifo_not_empty);

			//clear FIFO
			STM8_write_register(TOUCH_KEYBOARD_FIFO_CTL,STM8_FLAG_TOUCH_KEYBOARD_FIFO_CONTROL_CLEAR_FIFO);
		}
		break;

	}
	*///switch not usable
	if (stm8_status & STM8_FLAG_STATUS_CONTROL_GLOBAL_ERROR){
		//FIXME error handling TBD
		STM8_write_register (STATUS_CONTROL_ERR_FLAGS, 0x0);//clear all error flags
	}
#ifdef 	STM8_MANAGE_WAKEUP_EVENTS
	if (stm8_status & STM8_FLAG_STATUS_CONTROL_WAKEUP){
		//TODO to be implemented
		//TODO check if this is for hook management
		STM8_write_register (POWER_DOWN_CONTROL_WAKEUP_SOURCE1, 0x0);//clear event
	}
#endif
	
	if (stm8_status & STM8_FLAG_STATUS_CONTROL_TOUCH_CALIBRATION){
		//TODO to be implemented
	}
	
#ifdef STM8_MANAGE_HOOK_EVENTS
	
	if (stm8_status & STM8_FLAG_STATUS_EXTERNAL_EVENT){
		//hook and usb plug
		//only hook for now
		printk("External event\n");
		//read the status and clear the interrupts
		//with FW from Feb 27 it seems stuck here, commented out
		//works with FW 1b from March 2
		STM8_read_register(INPUT_SIGNAL_CONTROL_STATUS, 1, &read_buffer[0]);
		
		printk("Status hook/usb event: 0x%02X\n", read_buffer[0]);
		//0x80 = press (put down handset)
		//0x00 = release (get handset in hand)
		
		//press key event
		
		struct keyboard_touch_fifo *new_item = kmalloc(sizeof(struct keyboard_touch_fifo), GFP_KERNEL);
		if(!new_item)  return NULL;//FIXME error handling TBD
		
		INIT_LIST_HEAD(&new_item->list);//
		//new_item->current_status = read_buffer[0];//TODO Press/Release status!!!
		new_item->current_status = STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_KEYBOARD;
		if(read_buffer[0] & STM8_FLAG_INPUT_SIGNAL_CONTROL_HOOK) {
			new_item->current_status |= STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_PRESS;
		}else{
			new_item->current_status |= STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_RELEASE;
		}
		
		new_item->keyboard_code = STM8_HOOK_KEYCODE;
		new_item->ts_x_m = 0x0;
		new_item->ts_x_l = 0x0;
		new_item->ts_y_m = 0x0;
		new_item->ts_y_l = 0x0;
		list_add_tail(&new_item->list, &keyboard_touch_list);
		//event is not released with previous read
		//STM8_read_register(INPUT_SIGNAL_CONTROL_STATUS, 1,read_buffer);
		//printk("ISC_STATUS reg 0x%04X value 0x02%X\n", INPUT_SIGNAL_CONTROL_STATUS, read_buffer[0]);
		printk("Release hook event\n");
		STM8_write_register (INPUT_SIGNAL_CONTROL_STATUS, 0x0);//clear hook event
	}
#endif
	
	if (stm8_status & STM8_FLAG_STATUS_CONTROL_FIFO_NOT_EMPTY){
		bool fifo_not_empty;
		u8 event_status;			

		do {
			while (STM8_read_register(TOUCH_KEYBOARD_FIFO_STATUS, 1,read_buffer) !=1);
			event_status = read_buffer[0];
			
			if(event_status & STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_NOT_EMPTY)
				fifo_not_empty = true;	//FIFO Not Empty indicates another event, waiting in buffer
			else fifo_not_empty = false;

			struct keyboard_touch_fifo *new_item = kmalloc(sizeof(struct keyboard_touch_fifo), GFP_KERNEL);
			if(!new_item)  return NULL;//FIXME error handling TBD
			INIT_LIST_HEAD(&new_item->list);//FIXME why here?//Ok, I suppose
			new_item->current_status = event_status;

			//differentiate current(oldest) event between keyboard and touch screen, press and release
			if(event_status & STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_KEYBOARD) {
				while (STM8_read_register(TOUCH_KEYBOARD_FIFO_DATA_FIELD, 1,read_buffer) != 1);
				new_item->keyboard_code = read_buffer[0];
				printk("Keyboard code: %02X other bytes: %02X %02X %02X\n", read_buffer[0], read_buffer[1], read_buffer[2], read_buffer[3]);
				
				if(event_status & STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_PRESS);
				else if(event_status & STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_RELEASE);
			}	
			else if(event_status & STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_TOUCHSCREEN) {
				//while (STM8_read_register(TOUCH_KEYBOARD_FIFO_DATA_FIELD, 4,read_buffer)!=4);
				//TODO check endianess
				(STM8_read_register(TOUCH_KEYBOARD_FIFO_DATA_FIELD + 0, 1, &read_buffer[0])!=1);
				(STM8_read_register(TOUCH_KEYBOARD_FIFO_DATA_FIELD + 1, 1, &read_buffer[1])!=1);
				(STM8_read_register(TOUCH_KEYBOARD_FIFO_DATA_FIELD + 2, 1, &read_buffer[2])!=1);
				(STM8_read_register(TOUCH_KEYBOARD_FIFO_DATA_FIELD + 3, 1, &read_buffer[3])!=1);
				//new_item->touchscreen_xy = (u32)(read_buffer);
				new_item->ts_x_m = read_buffer[0];
				new_item->ts_x_l = read_buffer[1];
				new_item->ts_y_m = read_buffer[2];
				new_item->ts_y_l = read_buffer[3];
				printk("Touchscreen data: %02X %02X %02X %02X\n", read_buffer[0], read_buffer[1], read_buffer[2], read_buffer[3]);

				if(event_status & STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_PRESS);
				else if(event_status & STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_RELEASE);
			}
			//FIXME!!!
			list_add_tail(&new_item->list, &keyboard_touch_list); 
			//FIXME!!!

			//release one event
///for speed-up test			printk("releasing fifo event\n");
			STM8_write_register(TOUCH_KEYBOARD_FIFO_CTL,STM8_FLAG_TOUCH_KEYBOARD_FIFO_CONTROL_RELEASE_FIFO);
		} while (fifo_not_empty);

		//clear FIFO
		//FIXME needed???
		//if we released last FIFO event, then this is not needed
		//STM8_write_register(TOUCH_KEYBOARD_FIFO_CTL,STM8_FLAG_TOUCH_KEYBOARD_FIFO_CONTROL_CLEAR_FIFO);
	}

	kill_fasync (&stm8_dev.stm8_async_queue, SIGIO, POLL_IN);

	//Clearing the interrupt
	*SPEARBASIC_GPIO_IC &= 0x2;//clearing GPIO1 interrupt
	
	//IRQ debug//FIXME
	//enable_irq(IRQ_STM8);
	
	return IRQ_HANDLED;
}

/******************************************************************************
                                    MODULE
******************************************************************************/

/*
-------------------------------------------------------------------------------
PRIVATE: STM8_Init

Invoked when the module is dynamically loaded.
-------------------------------------------------------------------------------
*/


// block of pointers to MIXER routines above

static struct file_operations stm8_operations =
{	
	.owner 	= THIS_MODULE,
	.open   = stm8_open,
	.ioctl  = stm8_ioctl,
	.release = stm8_release,
	.fasync	 = stm8_fasync
};

static int __init STM8_Init(void)
{
	int rc = 0;
	u8 temp = 0;

	stm8_int_counter = 0;//irq debug
	
	INIT_LIST_HEAD(&keyboard_touch_list);//fifo list init
	
	printk("stm8 initialization: enter\n");
	
	//mutex_init(&stw5094a_mutex);//i2c lock
	
	rc = register_chrdev_region(stm8_I2C_devno, 1, "STM8");
	if (rc)
	{
		printk( "register_chrdev_region %x\n", rc);
		return rc;
	}
	cdev_init(&stm8_dev.i2c_cdev, &stm8_operations);
	stm8_dev.i2c_cdev.owner = THIS_MODULE;

	rc = cdev_add(&stm8_dev.i2c_cdev, stm8_I2C_devno, 1);
	if (rc)
	{
		//If Error occurs while adding a cdev instance then exit.
		printk("***unable to register character device***\n");
		goto out;
	}

	else {
		//register interrupt for STM8 
		rc = request_irq(IRQ_STM8, (void *)stm8_interrupt,SA_INTERRUPT | SA_SHIRQ,"stm8 irq",stm8_interrupt);
		if (rc == -EINVAL || rc == -EBUSY) {
			printk(KERN_ERR "stm8: Failed to register irq, return value = %d\n",rc);
			goto out;
		}
		printk("rc = %d\n",rc);
	}

	//
	disable_irq(IRQ_STM8);
	
	
	//for SPEAr LED control
	u32* SPEARBASIC_PL_GPIO_DIR = ioremap(0xA9000400, 4);
	//*SPEARBASIC_PL_GPIO_DIR |= 0xE0;//PL GPIO 5 6 7 as output
	*SPEARBASIC_PL_GPIO_DIR |= 0x23;//PL GPIO 5 0 1 as output
	SPEARBASIC_PL_GPIO_DATA = ioremap( (0xA9000400 - 4), 4);
	printk("GPIO Data address: 0x%08X\n", SPEARBASIC_PL_GPIO_DATA);
	//LED managed by SPEAr, GPIO have to be set as outputs
	//MUTE=GPIO5
	//HEADSET=GPIO6
	//HANDFREE=GPIO7
	
	//Interrupt management
	//GPIODIR: set the direction of GPIO[1] (on PAD A7) to be IN 
	u8* SPEARBASIC_GPIO_DIR = ioremap(SPEARBASIC_START_APB_GPIO1 + 0x400,1);
	temp = ioread8(SPEARBASIC_GPIO_DIR);
	iowrite8(temp & 0xFD, SPEARBASIC_GPIO_DIR);  //GPIO1 to be IN
	
	//GPIOIS: level on GPIO1 is detected	
	u8* SPEARBASIC_GPIO_IS = ioremap(SPEARBASIC_START_APB_GPIO1 + 0x404,1);
	temp = ioread8(SPEARBASIC_GPIO_IS);
	iowrite8(temp | 0x02, SPEARBASIC_GPIO_IS);  //level triggered interrupt			

	//GPIOIEV: level on GPIO1 is detected	
	u8* SPEARBASIC_GPIO_IEV = ioremap(SPEARBASIC_START_APB_GPIO1 + 0x40C,1);
	temp = ioread8(SPEARBASIC_GPIO_IEV);
	iowrite8(temp & 0xFD, SPEARBASIC_GPIO_IEV);  //low level triggered

	//GPIOIE: enable interrupt triggring on GPIO1
	u8* SPEARBASIC_GPIO_IE = ioremap(SPEARBASIC_START_APB_GPIO1 + 0x410,1);
	temp = ioread8(SPEARBASIC_GPIO_IE);
	iowrite8(temp | 0x02, SPEARBASIC_GPIO_IE);  //GPIO1 can trigger interrupt
	
	//clearing IRQ on SPEAr Basic side
	SPEARBASIC_GPIO_IC = ioremap(SPEARBASIC_START_APB_GPIO1 + 0x41C,1);
	*SPEARBASIC_GPIO_IC &= 0x2;
	
	SPEARBASIC_GPIO_MIS = ioremap(SPEARBASIC_START_APB_GPIO1 + 0x418,1);
	
	//init I2C, to clear events and flags
	STM8_I2C_Init();
	//clearing FIFO on STM8
	//STM8_write_register(0x2000, 0x1);
	STM8_write_register(TOUCH_KEYBOARD_FIFO_CTL,STM8_FLAG_TOUCH_KEYBOARD_FIFO_CONTROL_CLEAR_FIFO);
	
	//clearing error flags
	STM8_write_register (STATUS_CONTROL_ERR_FLAGS, 0x0);
	
	STM8_read_register(0x1E01, 1, &temp);
	printk("Status 0x1E01: %02X\n", temp);
	
	
	//clearing wakeup events
	STM8_write_register (POWER_DOWN_CONTROL_WAKEUP_SOURCE1, 0x0);//clear event
	
	STM8_read_register(0x1E01, 1, &temp);
	printk("Status 0x1E01: %02X\n", temp);
	
	
#ifdef STM8_MANAGE_HOOK_EVENTS
	//clearing hook events
	//FIXME seems to stops here
	STM8_read_register(INPUT_SIGNAL_CONTROL_STATUS, 1, &temp);//read hook status?
	printk("Read INPUT_SIGNAL_CONTROL_STATUS: 0x%02X\n", temp);
	//new firmware, we write 0 to clear
	STM8_write_register (INPUT_SIGNAL_CONTROL_STATUS, 0x0);//clear hook event
	STM8_read_register(0x1E01, 1, &temp);
	printk("Status 0x1E01: %02X\n", temp);
	
	//try: read 0x34 on address 0x1E01
	//STM8_write_register(0x1E01, 0x0);
	printk("**** Hook keycode: 0x%02X\n", STM8_HOOK_KEYCODE);
	
#endif

	//masking wakeup and extern events //I hope//not working
	//STM8_write_register (STATUS_CONTROL_MODULE + 0x2, 0x84);

	enable_irq(IRQ_STM8);
	
	printk("stm8 initialization: exit\n");

out:	return rc;
}

/*
-------------------------------------------------------------------------------
PRIVATE: STM8_Exit

Invoked when the module is dynamically unloaded.
-------------------------------------------------------------------------------
*/

static void __exit STM8_Exit(void)
{
	//mutex_destroy(&stw5094a_mutex);

	cdev_del(&stm8_dev.i2c_cdev) ;
	unregister_chrdev_region(stm8_I2C_devno, 1);

	disable_irq(IRQ_STM8);
	free_irq(IRQ_STM8,stm8_interrupt);
	
	iounmap(SPEARBASIC_PL_GPIO_DATA);
	
	//IRQ debug
	printk("Interrupt count: %u\n", stm8_int_counter );
}

/*
-------------------------------------------------------------------------------
Standard module declarations
-------------------------------------------------------------------------------
*/

module_init(STM8_Init);
module_exit(STM8_Exit);

MODULE_DESCRIPTION("STM8 driver for IP phones based on SPEAr Basic SoC");
MODULE_AUTHOR("STMicroelectronics");
MODULE_LICENSE("GPL");                           

/******************************************************************************
                                      EOF
******************************************************************************/ 

