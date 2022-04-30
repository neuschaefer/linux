/*
===============================================================================
FILE:        stm8.c
VERSION:     2.0
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
- 08/23/09: complete rewrite

===============================================================================
*/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h> 
#include <linux/slab.h>	
#include <linux/mutex.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/stm8.h>
#include "stm8_priv.h"
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>
#include <linux/proc_fs.h>


#include <linux/net.h>
#include <net/sock.h>
#include <linux/udp.h>
#include <linux/in.h>
#include <asm/uaccess.h>
#include <linux/file.h>
#include <linux/socket.h>
#include <linux/smp_lock.h>
#include <linux/slab.h>

#include <linux/inet.h>

/******************************************************************************
                            GENERAL DEFINITIONS
******************************************************************************/
#define STM8_TRACE(x...) 
//#define STM8_TRACE(x...) printk(x)
//#define STM8_ERROR(x...)
#define STM8_ERROR(x...) printk(x)

#define IRQ_STM8 11
#define IRQ_TIMER_BST22 7

#define FORCE_HFREE_AMP_ON_INIT//on module init we switch on the handsfree amplifier

#define STM8_MAJOR_NO 241

#if 0
#define SIGNAL_REFRESH_LEDS 33
#define LCS_PID_FILENAME "/tmp/lcs_pid"
#endif

static const int stm8_I2C_devno = MKDEV(STM8_MAJOR_NO,0);
static struct STM8_device stm8_dev;
static u8 stm8_status;
static stm8_led_conf call_led_conf;
static stm8_led_conf msg_led_conf;

#define STM8INTERRUPT 1
#define STM8TIMERINTERRUPT 2
static wait_queue_head_t stm8queue;
static wait_queue_head_t stm8interruptqueue;
static unsigned char stm8sourceinterrupt;
static struct task_struct * stm8irqthread;

static int prevx, prevy;
static stm8_calibration_info cal_info;

static struct mutex i2cmutex;
#define I2CTIMEOUT msecs_to_jiffies(100)
static wait_queue_head_t i2cwq;
static unsigned int i2c_irqstat;
static unsigned int i2c_txabrt;
static unsigned int i2c_done;

/* mapped registers */
static unsigned int * mappedGPIO0; 
static unsigned int * mappedGPIO1; 
static unsigned int * mappedTIMERB22;

/******************************************************************************
                         SW FIFO EVENT MANAGEMENT ROUTINES
******************************************************************************/
#define FIFOQDEPTH 16 // Must be power of 2
static int FifoQW;
static int FifoQR;
static struct keyboard_touch_fifo FifoQ[FIFOQDEPTH];
static spinlock_t FifoLock = SPIN_LOCK_UNLOCKED;
static int FifoLocked;

static struct socket *send_udp_socket = NULL;
#if 0
static struct task_struct *lcs_task;

static int update_leds_signal2lcs2(void)
{
  int fd;
  char buf[10];
  int pid = 0;
  int ret = 0;
  struct siginfo info;

  mm_segment_t old_fs = get_fs();
  set_fs(KERNEL_DS);

  memset(buf, 0, 10);
  fd = sys_open(LCS_PID_FILENAME, O_RDONLY, 0);
  if (fd >= 0) {
    sys_read(fd, buf, 10);
    sys_close(fd);
    sscanf(buf, "%d", &pid);
    printk("lcs2 pid:%d\n", pid);

    rcu_read_lock();
    lcs_task = find_task_by_pid_type(PIDTYPE_PID, pid);
    if(lcs_task == NULL){
      printk("no such pid\n");
      rcu_read_unlock();
      return -ENODEV;
    }
    rcu_read_unlock();
    printk("found task via pid\n");
    /* send the signal */
    memset(&info, 0, sizeof(struct siginfo));
    info.si_signo = SIGNAL_REFRESH_LEDS;
    info.si_code = SI_QUEUE;
    info.si_int = 0xbeef;
    ret = send_sig_info(SIGNAL_REFRESH_LEDS, &info, lcs_task);
    if (ret < 0) {
      printk("error sending signal\n");
    }
  }
  set_fs(old_fs);
  return ret;
}
#endif


static int update_leds_signal2lcs2(void) {
  struct sockaddr_in to;
  struct msghdr msg;
  struct iovec iov;
  int len;
  mm_segment_t oldmm;
  static char data[1] = "x";


  /* socket to send data */
  if (!send_udp_socket) {
    if (sock_create(AF_INET, SOCK_DGRAM, IPPROTO_UDP, &send_udp_socket) < 0) {
      printk( KERN_ERR "server: Error creating send_udp_socket.n" );
      return -EIO;
    }
  }

  if (send_udp_socket) {
    /* generate answer message */
    memset(&to,0, sizeof(to));
    to.sin_family = AF_INET;
    to.sin_addr.s_addr = in_aton("127.0.0.1");  
    to.sin_port = htons( (unsigned short)1300);;

    msg.msg_name     = &to;
    msg.msg_namelen  = sizeof(to);
    msg.msg_iov      = &iov;
    msg.msg_iovlen   = 1;
    msg.msg_control  = NULL;
    msg.msg_controllen = 0;
    msg.msg_flags = MSG_CONFIRM;

    oldmm = get_fs(); set_fs(KERNEL_DS);

    msg.msg_iov->iov_len = 1;
    msg.msg_iov->iov_base = data;


    len = sock_sendmsg(send_udp_socket, &msg, 1);
    printk("led update:len:%d\n", len);

    set_fs(oldmm);
  }

#if 0
  if (send_udp_socket)
    sock_release(send_udp_socket);
#endif
  return 0;
}

static void InitFifoQ(void)
{
    spin_lock_bh(&FifoLock);
    FifoQW = 0;
    FifoQR = 0;
    FifoLocked = 1;
    spin_unlock_bh(&FifoLock);
}

static int QueueEventFifo(struct keyboard_touch_fifo * f)
{
    spin_lock_bh(&FifoLock);
    if(((FIFOQDEPTH + FifoQR - 1) & (FIFOQDEPTH-1)) == FifoQW)
    {
        // Fifo Full
        STM8_TRACE("Fifo full\n");
        spin_unlock_bh(&FifoLock);
        return -1;
    }
    memcpy(&FifoQ[FifoQW], f, sizeof(struct keyboard_touch_fifo));
    STM8_TRACE("FifoQ %d(%d,%x)\n",FifoQW,f->event_type,f->keyboard_code);
    FifoQW = (FifoQW + 1) & (FIFOQDEPTH-1);
    spin_unlock_bh(&FifoLock);
    return 0;
}

static void UnlockFifo(void)
{
    spin_lock_bh(&FifoLock);
    FifoLocked = 0;
    spin_unlock_bh(&FifoLock);
}


static struct keyboard_touch_fifo * GetNextFifoItem(void)
{
    struct keyboard_touch_fifo * f;
    
    spin_lock_bh(&FifoLock);
    if(FifoLocked)
    {
        spin_unlock_bh(&FifoLock);
        return NULL;
    }    

    if(FifoQR == FifoQW)
    {
        // Fifo Empty
        f = NULL;
    }
    else
    {
        f = &FifoQ[FifoQR];
        STM8_TRACE("FifoDQ %d(%d,%x)\n",FifoQR,f->event_type,f->keyboard_code);
        FifoQR = (FifoQR + 1) & (FIFOQDEPTH-1);
        if(FifoQR == FifoQW)
        {
            // Fifo Empty : lock fifo
            FifoLocked = 1;
        }
    }    
    spin_unlock_bh(&FifoLock);
    
    return f;
}

/******************************************************************************
    I2C CONTROLLER ACCESS ROUTINES - SPECIALIZED FOR IP PHONE APPLICATIONS
******************************************************************************/
static irqreturn_t spear_i2c_irq (int irq, void *_dev)
{
    unsigned int dummy;
    
    STM8_TRACE("I2CIRQ\n");
    //mask all interrupts & save raw status
    i2c_irqstat = *IC_INTR_STAT;
    i2c_txabrt = *IC_TXABRT_SOURCE;
    *IC_INTR_MASK = 0x0;
    
    //clear all interrupts
    dummy = *IC_CLR_INTR;
    i2c_done = 1;
	wake_up_interruptible(&i2cwq);

	return IRQ_HANDLED;
}


static int SPEAR_I2C_Init(void)
{   
  // init i2c mutex
  mutex_init(&i2cmutex);
	
  // init i2c waitqueue
  init_waitqueue_head(&i2cwq);
	
  // reinitialize I2C interface
  *IC_ENABLE = 0x0;
  *IC_CON = 0x63; // no slave, master mode, std speed, 7-bit addressing, restart enabled
  *IC_TX_TL = 0x0; //TX empty thrshold for irq
  *IC_SS_SCL_HCNT = 1 * (IC_CLK * MIN_SS_SCL_HIGHTIME) / NANO_TO_MICRO;
  *IC_SS_SCL_LCNT = 1 * (IC_CLK * MIN_SS_SCL_LOWTIME) / NANO_TO_MICRO;	
  *IC_TAR = 0x51; //STM8 Address by default
  *IC_INTR_MASK = 0x0; //no interrupt allowed
  // install IRQ 
  if(request_irq(IRQ_I2C,spear_i2c_irq,0, "I2C IRQ", spear_i2c_irq))
    {
      STM8_ERROR("I2C irq registration failure\n");
      return -1;
    }

   return 0;
}

static void SPEAR_I2C_Close(void)
{   
	*IC_ENABLE = 0x0;
	free_irq(IRQ_I2C, spear_i2c_irq);
}


void SPEAR_I2C_GetAccess(unsigned char Address)
{   
	// Get Mutex
	mutex_lock(&i2cmutex);
	
	// Check I2C cell is really idle.
	if(*IC_STATUS & 1)
	{
	    STM8_ERROR ("Error : I2C cell not idle\n");
	}
	
	*IC_TAR = Address; 
	*IC_ENABLE = 0x1;	
	STM8_TRACE("I2c getaccess");
}
EXPORT_SYMBOL(SPEAR_I2C_GetAccess);

void SPEAR_I2C_ReleaseAccess(void)
{   
	// Check I2C cell is really idle.
	if(*IC_STATUS & 1)
	{
	    STM8_ERROR ("Error : I2C cell not idle\n");
	    // TO DO: Wait then reset
	}
	
	//disable i2c
	*IC_ENABLE = 0x0;
    
    // Release Mutex
	mutex_unlock(&i2cmutex);
	STM8_TRACE("I2c releaseaccess");
}
EXPORT_SYMBOL(SPEAR_I2C_ReleaseAccess);


int SPEAR_I2C_Write(unsigned char addlen, unsigned int subaddress, unsigned char * value, unsigned char len)
{
    unsigned char i;
    STM8_TRACE("I2CW:%x,%x,%d:",subaddress,*value,len);
    while(1)
    {
        i2c_done = 0;
        *IC_TX_TL = 0x0; 
    	if(addlen == 16) 
    	{
    	    *IC_CMD_DATA = (subaddress & 0xFF00) >> 0x08;
    	}    	
        *IC_CMD_DATA = subaddress & 0x00FF;
        for(i=0;i<len;i++)
        {
            *IC_CMD_DATA = value[i];
        }        
        *IC_INTR_MASK = IC_R_TX_EMPTY | IC_R_TX_ABRT;
        
        if(wait_event_interruptible_timeout(i2cwq,(i2c_done == 1),I2CTIMEOUT) == 0)
        {
	  // timeout -> reset cell and restart
	  STM8_ERROR("I2Cwrite : timeout\n");
	  STM8_GPIO_RESET(GPIO_STM8_RST);
	  msleep(2); /* 2ms reset pulse */
	  STM8_GPIO_SET(GPIO_STM8_RST);
	  msleep(5); /* 5ms reset pulse */
	  *IC_ENABLE = 0x0;
	  *IC_ENABLE = 0x1;
	  STM8_ERROR("Reset of stm8 executed\n");
	  update_leds_signal2lcs2();
        }
        else
        {
            if(i2c_irqstat & IC_R_TX_ABRT)
            {
                // tx abort -> reset cell and restart
                *IC_ENABLE = 0x0;
                STM8_ERROR("I2Cwrite (%x): tx abort\n",i2c_txabrt);
                *IC_ENABLE = 0x1;
            }
            else
            {
                unsigned int loop = 100;
                while((*IC_STATUS & 1) && (loop!=0))
    	        {
    	            udelay(20); // active wait 
    	            loop--;
            	}
            	if(loop==0)
            	{
                    // active remains high -> reset cell and restart
                    *IC_ENABLE = 0x0;
                    STM8_ERROR("I2Cwrite : remains active\n");
                    *IC_ENABLE = 0x1;
            	}
            	else
            	{
                    //everything went fine : final checks
                    if(((i2c_irqstat & IC_R_TX_EMPTY) == 0) || (*IC_STATUS != 6))
                    {
                        STM8_ERROR("I2Cwrite (%x,%x): strange condition\n",i2c_irqstat,*IC_STATUS);
                    }
                    STM8_TRACE("...ok\n");
                    return 0;
                }                
            }
        }
    }    
}


int SPEAR_I2C_Read(unsigned char addlen, unsigned int subaddress, unsigned char * value, unsigned char len)
{
  unsigned char i;
  STM8_TRACE("I2CR:%x,%d:",subaddress,len);
#if 0
  static int count = 0;

  if (count == 300) {/* remove it */
    STM8_GPIO_RESET(GPIO_STM8_RST);
    msleep(2); /* 2ms reset pulse */
    STM8_GPIO_SET(GPIO_STM8_RST);
    msleep(5); /* 5ms reset pulse */
    STM8_ERROR("Resetsimul of stm8 executed\n");
    update_leds_signal2lcs2();
    count = 0;
  }
  count++;
#endif
  while(1)
    {
      i2c_done = 0;
      *IC_RX_TL = len-1; 
      if(addlen == 16) 
    	{
	  *IC_CMD_DATA = (subaddress & 0xFF00) >> 0x08;
    	}  
      *IC_CMD_DATA = subaddress & 0x00FF;
      for(i=0;i<len;i++)
        {
	  *IC_CMD_DATA = IC_CMD;
        }
      *IC_INTR_MASK = IC_R_RX_FULL | IC_R_TX_ABRT;
        
      if(wait_event_interruptible_timeout(i2cwq,(i2c_done == 1),I2CTIMEOUT) == 0)
        {
	  STM8_ERROR("I2Cread : timeout\n");
	  //	    	    printk("mutex count:%d\n", atomic_read(&i2cmutex.count));
	  STM8_GPIO_RESET(GPIO_STM8_RST);
	  msleep(2); /* 2ms reset pulse */
	  STM8_GPIO_SET(GPIO_STM8_RST);
	  msleep(5); /* 5ms reset pulse */
	  *IC_ENABLE = 0x0;
	  *IC_ENABLE = 0x1;
	  STM8_ERROR("Reset of stm8 executed\n");
	  update_leds_signal2lcs2();
        }
      else
        {
            if(i2c_irqstat & IC_R_TX_ABRT)
            {
                // tx abort -> reset cell and restart
                *IC_ENABLE = 0x0;
                STM8_ERROR("I2Cread (%x): tx abort\n",i2c_txabrt);
                *IC_ENABLE = 0x1;
            }
            else
            {
                unsigned int loop = 100;
                while((*IC_STATUS & 1) && (loop!=0))
    	        {
    	            udelay(20); // active wait 
    	            loop--;
            	}
            	if(loop==0)
            	{
                    // active remains high -> reset cell and restart
                    *IC_ENABLE = 0x0;
                    STM8_ERROR("I2Cread : remains active\n");
                    *IC_ENABLE = 0x1;
            	}
            	else
            	{
                    //everything went fine : final checks
                    if(((i2c_irqstat & IC_R_RX_FULL) == 0) || (*IC_STATUS != 0xE))
                    {
                        STM8_ERROR("I2Cread (%x,%x): strange condition\n",i2c_irqstat,*IC_STATUS);
                    }
                    i=0;while(*IC_STATUS & IC_STATUS_RFNE)
                    {
                        value[i] = *IC_CMD_DATA;
                        i++;
                    }
                    if(i!=len)
                    {
                        STM8_ERROR("I2Cread (%d,%d): len mismatch\n",i,len);
                    }
                    STM8_TRACE("...ok:%x\n",value[0]);
                    return 0;
                }                                        
            }
        }
    }        
}

int SPEAR_I2C_Write1(unsigned int subaddress, unsigned char value)
{
    return SPEAR_I2C_Write(16, subaddress, &value, 1);
}

int SPEAR_I2C_Read1(unsigned int subaddress)
{
    unsigned char value;
    SPEAR_I2C_Read(16, subaddress, &value, 1);
    return value;
}

int SPEAR_I2C_Write8(unsigned int subaddress, unsigned char value)
{
    return SPEAR_I2C_Write(8, subaddress, &value, 1);
}
EXPORT_SYMBOL(SPEAR_I2C_Write8);

int SPEAR_I2C_Read8(unsigned int subaddress)
{
    unsigned char value;
    SPEAR_I2C_Read(8, subaddress, &value, 1);
    return value;
}
EXPORT_SYMBOL(SPEAR_I2C_Read8);


/******************************************************************************
                            STM8 TIMER RELATED FUNCTIONS
******************************************************************************/

irqreturn_t stm8_timer_interrupt(int irq,void *dev_id,struct pt_regs *regs)
{
    TIMERB22_STATUS_INT_ACK = TIMER_COUNTER_IRQ_CLEAR_FLAG;
	if(stm8_drag_conf.status == DRAG_ON)
	{
        //disable irq source
        TIMERB22_CONTROL &= ~TIMER_CONTROL_FLAG_COMPARATOR;        
        stm8sourceinterrupt |= STM8TIMERINTERRUPT;
        wake_up_interruptible(&stm8interruptqueue);
    }
	return IRQ_HANDLED;

}

static void stm8handletimerinterrupt (void)    
{
	char buffer[4];
	unsigned int x,y;
	int delta;
	struct keyboard_touch_fifo new_event;

    //read stm8 drag X,Y
    SPEAR_I2C_GetAccess(0x51);		
	SPEAR_I2C_Read(16,TOUCH_KEYBOARD_FIFO_DRAG_FIELD, buffer, 4);    	
	SPEAR_I2C_ReleaseAccess();	
	x = (((unsigned int) buffer[0]) << 8) + ((unsigned int) buffer[1]);
	y = (((unsigned int) buffer[2]) << 8) + ((unsigned int) buffer[3]);;
	delta = abs(x - prevx) + abs(y - prevy);
	//if change > threshold
	if (delta >= stm8_drag_conf.pixel_threshold)
	{
        new_event.current_status = STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_TOUCHSCREEN & STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_PRESS;//event_status;
		new_event.event_type = TOUCH_DRAGGED;
		new_event.ts_x_m = buffer[0];
		new_event.ts_x_l = buffer[1];
		new_event.ts_y_m = buffer[2];
		new_event.ts_y_l = buffer[3];
		//update Drag&Drop data
		prevx = x;
		prevy = y;
		//post event
	    QueueEventFifo(&new_event);
	    //async communication
	    kill_fasync (&stm8_dev.stm8_async_queue, SIGIO, POLL_IN);
	    UnlockFifo();
	}	
	// reenable irq
	TIMERB22_CONTROL |= TIMER_CONTROL_FLAG_COMPARATOR;        
}

int stm8_timer_init(void)
{
	int rc = 0;

	TIMERB22_CONTROL = TIMER_CONTROL_DEFAULT;
	//default to 10 millisec (if prescaler division scale 64)
	//(note: real value 10000 * 962.41 ns = 9.62 millisec)
	// FRED : see if we can switch to 20ms
	TIMERB22_COMPARE = 10000;
	
	//register timer irq
	rc = request_irq(IRQ_TIMER_BST22, (void *)stm8_timer_interrupt,SA_INTERRUPT | SA_SHIRQ,"stm8 timer irq",stm8_timer_interrupt);
    if (rc == -EINVAL || rc == -EBUSY) 
    {
		printk(KERN_ERR "stm8: Failed to register timer irq, return value = %d\n",rc);
	}
	
	
	return rc;
}


void stm8_timer_conf(unsigned int counter)
{
	TIMERB22_COMPARE = (counter & 0xFFFF);
}

void stm8_timer_control( stm8_drag_states enable)
{
	if(enable == DRAG_ON)
	{
		TIMERB22_CONTROL = TIMER_CONTROL_DEFAULT | TIMER_CONTROL_FLAG_COMPARATOR | TIMER_CONTROL_ENABLE_FLAG;
	}
	else
	{
		TIMERB22_CONTROL = TIMER_CONTROL_DEFAULT;
	}
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

	stm8_ctrl ctrl;
	stm8_calibration_info calib;
	char buffer;

	struct keyboard_touch_fifo *ops;
	struct keyboard_touch_event event;

	switch (cmd) {
		case STM8_GET_NEXT_FIFO_EVENT: 
		    ops = GetNextFifoItem();
		    if(ops == NULL)
		    {
  		        return -EFAULT;
		    }
			event.current_status = ops->current_status;
			event.event_type = ops->event_type;
			event.keyboard_code = ops->keyboard_code;
			event.x = (((unsigned int) ops->ts_x_m) << 8) + ((unsigned int) ops->ts_x_l);
			event.y = (((unsigned int) ops->ts_y_m) << 8) + ((unsigned int) ops->ts_y_l);;
			return copy_to_user(argp, &event, sizeof(struct keyboard_touch_event));

		case STM8_SET:
			if( copy_from_user(&ctrl, argp, sizeof(stm8_ctrl)) )
			{
				return -EFAULT;
			}
			switch(ctrl.feature)
			{
				case CALL_LED:
                    SPEAR_I2C_GetAccess(0x51);	
					switch(ctrl.state){
						case ON:
							buffer = SPEAR_I2C_Read1(LED_CONTROL_CTL);
							buffer |= STM8_FLAG_LED_CONTROL_LED2_ON;
							buffer &= ~(STM8_FLAG_LED_CONTROL_LED2_BLINK);
							SPEAR_I2C_Write1(LED_CONTROL_CTL, buffer);
						break;
							
						case OFF:
							buffer = SPEAR_I2C_Read1(LED_CONTROL_CTL);
							buffer &= ~(STM8_FLAG_LED_CONTROL_LED2_ON);
							buffer &= ~(STM8_FLAG_LED_CONTROL_LED2_BLINK);
							SPEAR_I2C_Write1(LED_CONTROL_CTL, buffer);
						break;
							
						case BLINK:
							buffer = SPEAR_I2C_Read1(LED_CONTROL_CTL);
							buffer &= ~(STM8_FLAG_LED_CONTROL_LED2_ON);
							buffer |= STM8_FLAG_LED_CONTROL_LED2_BLINK;
							call_led_conf.duty_cycle = ctrl.duty_cycle & 0xFF;
							call_led_conf.frequency = ctrl.frequency & 0xFF;
							SPEAR_I2C_Write1(LED_CONTROL_DUTY_CALL, call_led_conf.duty_cycle);
							SPEAR_I2C_Write1(LED_CONTROL_FREQ_CALL, call_led_conf.frequency);
							SPEAR_I2C_Write1(LED_CONTROL_CTL, buffer);
						break;
							
						default:
							STM8_ERROR("Error, unknown CALL_LED state requested: %u\n", ctrl.state);
						break;
					}
					stm8_features_status.call_led = ctrl.state;					
					SPEAR_I2C_ReleaseAccess();
				break;
					
				case MSG_LED:
				    SPEAR_I2C_GetAccess(0x51);	
					switch(ctrl.state)
					{
						case ON:
							buffer = SPEAR_I2C_Read1(LED_CONTROL_CTL);
							buffer |= STM8_FLAG_LED_CONTROL_LED1_ON;
							buffer &= ~(STM8_FLAG_LED_CONTROL_LED1_BLINK);
							SPEAR_I2C_Write1(LED_CONTROL_CTL, buffer);
						break;
							
						case OFF:
							buffer = SPEAR_I2C_Read1(LED_CONTROL_CTL);
							buffer &= ~(STM8_FLAG_LED_CONTROL_LED1_ON);
							buffer &= ~(STM8_FLAG_LED_CONTROL_LED1_BLINK);
							SPEAR_I2C_Write1(LED_CONTROL_CTL, buffer);
						break;
							
						case BLINK:
							buffer = SPEAR_I2C_Read1(LED_CONTROL_CTL);
							buffer &= ~(STM8_FLAG_LED_CONTROL_LED1_ON);
							buffer |= STM8_FLAG_LED_CONTROL_LED1_BLINK;
							msg_led_conf.duty_cycle = ctrl.duty_cycle & 0xFF;
							msg_led_conf.frequency = ctrl.frequency & 0xFF;
							SPEAR_I2C_Write1(LED_CONTROL_DUTY_CALL, msg_led_conf.duty_cycle);
							SPEAR_I2C_Write1(LED_CONTROL_FREQ_CALL, msg_led_conf.frequency);
							SPEAR_I2C_Write1(LED_CONTROL_CTL, buffer);
						break;
					}
					stm8_features_status.msg_led = ctrl.state;
					SPEAR_I2C_ReleaseAccess();
				break;

				//SNOM870E (snom vision)
				case STM8_870E_LED_CTL:
				        SPEAR_I2C_GetAccess(0x51);
					SPEAR_I2C_Write1(LED_CONTROL_CMD, ctrl.led_870e & 0xFF);
					SPEAR_I2C_ReleaseAccess();
				break;

				//SNOM820
				case STM8_820_LED_CTL:
				        SPEAR_I2C_GetAccess(0x51);
					SPEAR_I2C_Write1(LED_CONTROL_CMD, ctrl.led_820 & 0xFF);
					SPEAR_I2C_ReleaseAccess();
				break;
					
				case PIN17_EHS:
				    SPEAR_I2C_GetAccess(0x51);	
					buffer = SPEAR_I2C_Read1(INPUT_SIGNAL_CONTROL_CONTROL);
					if(ctrl.state == ON)
					{
						buffer |= STM8_FLAG_INPUT_SIGNAL_CONTROL_PIN17_EHS;
					}
					else
					{
						buffer &= ~(STM8_FLAG_INPUT_SIGNAL_CONTROL_PIN17_EHS);
					}
					SPEAR_I2C_Write1(INPUT_SIGNAL_CONTROL_CONTROL, buffer);
					stm8_features_status.pin17_ehs = ctrl.state;
					SPEAR_I2C_ReleaseAccess();
				break;
				
				case PIN23_EHS:
					STM8_ERROR("PIN23_EHS: STM8_SET not supported\n");
				break;
					
				case PIN27_SPI_CK:
				    SPEAR_I2C_GetAccess(0x51);	
					buffer = SPEAR_I2C_Read1(INPUT_SIGNAL_CONTROL_CONTROL);
					if(ctrl.state == ON)
					{
						buffer |= STM8_FLAG_INPUT_SIGNAL_CONTROL_PIN27_SPI_CK;
					}
					else
					{
						buffer &= ~(STM8_FLAG_INPUT_SIGNAL_CONTROL_PIN27_SPI_CK);
					}
					SPEAR_I2C_Write1(INPUT_SIGNAL_CONTROL_CONTROL, buffer);
					stm8_features_status.pin27_spi_ck = ctrl.state;
					SPEAR_I2C_ReleaseAccess();
				break;
					
				case PIN32_AMP:
					SPEAR_I2C_GetAccess(0x51);	
					buffer = SPEAR_I2C_Read1(INPUT_SIGNAL_CONTROL_CONTROL);
					if(ctrl.state == ON)
					{
						buffer |= STM8_FLAG_INPUT_SIGNAL_CONTROL_PIN32_AMP;
					}
					else
					{
						buffer &= ~(STM8_FLAG_INPUT_SIGNAL_CONTROL_PIN32_AMP);
					}
					SPEAR_I2C_Write1(INPUT_SIGNAL_CONTROL_CONTROL, buffer);
					stm8_features_status.pin32_amp = ctrl.state;
					SPEAR_I2C_ReleaseAccess();
				break;
					
				case KEYPAD:
				    SPEAR_I2C_GetAccess(0x51);	
					stm8_features_status.keypad = ctrl.state;
					if(ctrl.state == ON)
					{
						SPEAR_I2C_Write1(KEYBOARD_SETTING_CTL, ~STM8_FLAG_KEYBOARD_SETTING_CONTROL_KEYBOARD_DIS);
					}
					else
					{
						SPEAR_I2C_Write1(KEYBOARD_SETTING_CTL, STM8_FLAG_KEYBOARD_SETTING_CONTROL_KEYBOARD_DIS);
					}
					SPEAR_I2C_ReleaseAccess();
				break;
					
				case TOUCHSCREEN:
					SPEAR_I2C_GetAccess(0x51);	
					buffer = SPEAR_I2C_Read1(TOUCH_CALIBRATION_SETTING_CTL);
					if(ctrl.state == ON)
					{
						buffer &= ~(STM8_FLAG_TOUCH_CALIBRATION_SETTING_CONTROL_TOUCH_DIS);
					}
					else
					{
						buffer |= STM8_FLAG_TOUCH_CALIBRATION_SETTING_CONTROL_TOUCH_DIS;
					}
					SPEAR_I2C_Write1(TOUCH_CALIBRATION_SETTING_CTL, buffer);
					stm8_features_status.touchscreen = ctrl.state;
					SPEAR_I2C_ReleaseAccess();
				break;
					
				case DRAGMODE:
					if(ctrl.state == ON)
					{
						stm8_timer_control(DRAG_OFF);
						stm8_drag_conf.status = DRAG_ON;
						stm8_drag_conf.time_resolution = ctrl.period;
						stm8_drag_conf.pixel_threshold = ctrl.pixels;
						stm8_features_status.dragmode = ON;
						stm8_timer_conf(stm8_drag_conf.time_resolution * 1000);//TODO change to a constant
						stm8_timer_control(stm8_drag_conf.status);
					}
					else
					{
						stm8_drag_conf.status = DRAG_OFF;
						stm8_features_status.dragmode = OFF;
						stm8_timer_control(stm8_drag_conf.status);
					}
				break;
				
				case MUTE_LED:
					if(ctrl.state == ON)
					{
						STM8_GPIO_SET(GPIO_LED_MUTE);
					}
					else
					{
						STM8_GPIO_RESET(GPIO_LED_MUTE);
					}
					stm8_features_status.mute_led = ctrl.state;
				break;
				
				case HEADSET_LED:
					if(ctrl.state == ON)
					{
						STM8_GPIO_SET(GPIO_LED_HEADSET);
					}
					else
					{
						STM8_GPIO_RESET(GPIO_LED_HEADSET);
					}
					stm8_features_status.headset_led = ctrl.state;
				break;
					
				case HANDSFREE_LED:
					if(ctrl.state == ON)
					{
						STM8_GPIO_SET(GPIO_LED_HANDSFREE);
					}
					else
					{
						STM8_GPIO_RESET(GPIO_LED_HANDSFREE);
					}
					stm8_features_status.handsfree_led = ctrl.state;
				break;
					
				default:
					return -EINVAL;
			}
			return copy_to_user(argp, &ctrl, sizeof( stm8_ctrl ));
		break;
			
		case STM8_GET:
			if( copy_from_user(&ctrl, argp, sizeof(stm8_ctrl)) )
			{
				return -EFAULT;
			}
			ctrl.pixels = 0;
			ctrl.period = 0;
			ctrl.duty_cycle = 0;
			ctrl.frequency = 0;
			switch(ctrl.feature)
			{				
				case CALL_LED:
					ctrl.state = stm8_features_status.call_led;
					ctrl.duty_cycle = call_led_conf.duty_cycle;
					ctrl.frequency = call_led_conf.frequency;
				break;
					
				case MSG_LED:
					ctrl.state = stm8_features_status.msg_led;
					ctrl.duty_cycle = msg_led_conf.duty_cycle;
					ctrl.frequency = msg_led_conf.frequency;
				break;
					
				case PIN17_EHS:
					ctrl.state = stm8_features_status.pin17_ehs;
				break;
				
				case PIN23_EHS:
					SPEAR_I2C_GetAccess(0x51);
					if ( SPEAR_I2C_Read1(INPUT_SIGNAL_CONTROL_STATUS) & STM8_FLAG_INPUT_SIGNAL_STATUS_PIN23_EHS )
					{
						ctrl.state = ON;
					}
					else
					{
						ctrl.state = OFF;
					}
					SPEAR_I2C_ReleaseAccess();
				break;
				
				case PIN27_SPI_CK:
					ctrl.state = stm8_features_status.pin27_spi_ck;
				break;
					
				case PIN32_AMP:
					ctrl.state = stm8_features_status.pin32_amp;
				break;
					
				case KEYPAD:
					ctrl.state = stm8_features_status.keypad;
				break;
					
				case TOUCHSCREEN:
					ctrl.state = stm8_features_status.touchscreen;
				break;
					
				case DRAGMODE:
					ctrl.state = stm8_features_status.dragmode;
					ctrl.pixels = stm8_drag_conf.pixel_threshold;
					ctrl.period = stm8_drag_conf.time_resolution;
				break;
				
				case MUTE_LED:
					ctrl.state = stm8_features_status.mute_led;
				break;
				
				case HEADSET_LED:
					ctrl.state = stm8_features_status.headset_led;
				break;
					
				case HANDSFREE_LED:
					ctrl.state = stm8_features_status.handsfree_led;
				break;
					
				default:
					return -EINVAL;
				break;
			}
			return copy_to_user(argp, &ctrl, sizeof( stm8_ctrl ));
		break;
		
		case STM8_TS_CALIBRATION_INIT:
			SPEAR_I2C_GetAccess(0x51);
			SPEAR_I2C_Write1(TOUCH_CALIBRATION_SETTING_CTL, STM8_FLAG_TOUCH_CALIBRATION_SETTING_CONTROL_CALIB_REQ);
			SPEAR_I2C_ReleaseAccess();
			cal_info.global_status = 0xFF;
			wait_event_interruptible(stm8queue, (cal_info.global_status != 0xFF));
			calib.global_status = cal_info.global_status;
			calib.touchscreen_status = cal_info.touchscreen_status;
			calib.calibration_cross_x = cal_info.calibration_cross_x;
			calib.calibration_cross_y = cal_info.calibration_cross_y;
			return copy_to_user(argp, &calib, sizeof( stm8_calibration_info ));			
		break;
		
		case STM8_TS_CALIBRATION_ACK:
		    SPEAR_I2C_GetAccess(0x51);
			SPEAR_I2C_Write1 ( TOUCH_CALIBRATION_SETTING_CTL, STM8_FLAG_TOUCH_CALIBRATION_SETTING_CONTROL_CROSS_ACK | STM8_FLAG_TOUCH_CALIBRATION_SETTING_CONTROL_CALIB_REQ );
			SPEAR_I2C_ReleaseAccess();
			cal_info.global_status = 0xFF;
			wait_event_interruptible(stm8queue, (cal_info.global_status != 0xFF));
			calib.global_status = cal_info.global_status;
			calib.touchscreen_status = cal_info.touchscreen_status;
			calib.calibration_cross_x = cal_info.calibration_cross_x;
			calib.calibration_cross_y = cal_info.calibration_cross_y;
			if(calib.touchscreen_status & 0x30)
			{
			    /* reset calib */
			    SPEAR_I2C_GetAccess(0x51);
			    SPEAR_I2C_Write1 ( TOUCH_CALIBRATION_SETTING_CTL, 0 );
			    SPEAR_I2C_ReleaseAccess();
			}
			return copy_to_user(argp, &calib, sizeof( stm8_calibration_info ));		
		break;
						
		default:
			STM8_ERROR("stm8_ioctl: invalid cmd\n");
			return -EINVAL;
	}

	return -EINVAL;	
}

/*
-------------------------------------------------------------------------------
PRIVATE: stm8_fasync

Used for async communication.
-------------------------------------------------------------------------------
*/

static int stm8_fasync (int fd, struct file *filp, int mode)
{
	return fasync_helper (fd, filp, mode, &stm8_dev.stm8_async_queue);
}


/*
-------------------------------------------------------------------------------
PRIVATE: stm8_release

Invoked when the logical device is closed.
-------------------------------------------------------------------------------
*/

static int stm8_release (struct inode *inode, struct file *file)
{
	if (file->f_flags & FASYNC) 
	{
		stm8_fasync(-1, file, 0);
	}
	return 0;
}

/*
-------------------------------------------------------------------------------
PRIVATE: stm8_interrupt

Interrupt routine
-------------------------------------------------------------------------------
 */
irqreturn_t stm8_interrupt(int irq,void *dev_id,struct pt_regs *regs)
{
    if ((GPIO1_MIS & 0x02) == 0)
    {
        return IRQ_NONE;
    }        
    GPIO1_IE &= 0xFD; // disable pio irq
    GPIO1_IC = 0x02;  //GPIO1_IC (WO) : clear interrupt
    
    stm8sourceinterrupt |= STM8INTERRUPT;
    wake_up_interruptible(&stm8interruptqueue);

	return IRQ_HANDLED;
}

static void stm8handleinterrupt(void)
{
	char read_buffer [4];
	struct keyboard_touch_fifo new_event;
	char eventqueued = 0;
	
    SPEAR_I2C_GetAccess(0x51);
    
    while ((GPIO1_DATA & 0x02) == 0)
    {
    
        stm8_status = SPEAR_I2C_Read1(STATUS_CONTROL_STATUS);
        STM8_TRACE("STATUS:%x\n",stm8_status);
    	if (stm8_status & STM8_FLAG_STATUS_CONTROL_GLOBAL_ERROR)
    	{
    		STM8_ERROR("Global Error (STM8 IRQ), status: 0x%02X flags: 0x%02X\n", stm8_status, SPEAR_I2C_Read1(STATUS_CONTROL_ERR_FLAGS));
    		SPEAR_I2C_Write1 (STATUS_CONTROL_ERR_FLAGS, 0x0);//clear all error flags
    	}
#ifdef 	STM8_MANAGE_WAKEUP_EVENTS
    	if (stm8_status & STM8_FLAG_STATUS_CONTROL_WAKEUP)
    	{
    		SPEAR_I2C_Write1 (POWER_DOWN_CONTROL_WAKEUP_SOURCE1, 0x0);//clear event
    	}
#endif
    		
    	if (stm8_status & STM8_FLAG_STATUS_CONTROL_TOUCH_CALIBRATION)
    	{
    		//read statuses
    		cal_info.global_status = SPEAR_I2C_Read1(STATUS_CONTROL_STATUS);
    		cal_info.touchscreen_status = SPEAR_I2C_Read1(TOUCH_CALIBRATION_SETTING_STATUS);
    		SPEAR_I2C_Read(16,TOUCH_CALIBRATION_SETTING_CALIBRATION_CROSS_X,read_buffer,4);		
    		cal_info.calibration_cross_x = (((unsigned int) read_buffer[0]) << 8) + (unsigned int) read_buffer[1];
    		cal_info.calibration_cross_y = (((unsigned int) read_buffer[2]) << 8) + (unsigned int) read_buffer[3];		
    		SPEAR_I2C_Write1 (TOUCH_CALIBRATION_SETTING_CTL, STM8_FLAG_TOUCH_CALIBRATION_SETTING_CONTROL_CALIB_REQ);//clear interrupt
    		wake_up_interruptible(&stm8queue);
    	}
    	
#ifdef STM8_MANAGE_HOOK_EVENTS
    	
    	if (stm8_status & STM8_FLAG_STATUS_EXTERNAL_EVENT)
    	{
    		new_event.current_status = STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_KEYBOARD;
    		if(SPEAR_I2C_Read1(INPUT_SIGNAL_CONTROL_STATUS) & STM8_FLAG_INPUT_SIGNAL_CONTROL_HOOK) 
    		{
    			new_event.current_status |= STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_PRESS;
    			new_event.event_type = KEY_PRESSED;
    		}
    		else
    		{
    			new_event.current_status |= STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_RELEASE;
    			new_event.event_type = KEY_RELEASED;
    		}
    		new_event.keyboard_code = STM8_HOOK_KEYCODE;
    		new_event.ts_x_m = 0x0;
    		new_event.ts_x_l = 0x0;
    		new_event.ts_y_m = 0x0;
    		new_event.ts_y_l = 0x0;
    		QueueEventFifo(&new_event);
        	eventqueued = 1;
    		SPEAR_I2C_Write1 (INPUT_SIGNAL_CONTROL_STATUS, 0x0);//clear hook event
    	}
#endif
    	
    	if (stm8_status & STM8_FLAG_STATUS_CONTROL_FIFO_NOT_EMPTY)
    	{
    		bool fifo_not_empty;
    		u8 event_status;			
    		do 
    		{
    		    event_status = SPEAR_I2C_Read1(TOUCH_KEYBOARD_FIFO_STATUS);
    			fifo_not_empty = (event_status & STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_NOT_EMPTY) ? true : false;    		
        		new_event.current_status = event_status;
            	//differentiate current(oldest) event between keyboard and touch screen, press and release
            	STM8_TRACE("EVENT:%x\n",event_status);
    			if(event_status & STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_KEYBOARD) 
    			{
    				if(event_status & STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_PRESS)
    				{
    					new_event.event_type = KEY_PRESSED;
    				}
    				else if(event_status & STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_RELEASE)
    				{
    					new_event.event_type = KEY_RELEASED;
    				}
    				else
    				{
    				    new_event.event_type = -1; 
    				}
    				
    				new_event.keyboard_code = SPEAR_I2C_Read1(TOUCH_KEYBOARD_FIFO_DATA_FIELD);
    				STM8_TRACE("KEYBOARD:%x\n",new_event.keyboard_code);
    				new_event.ts_x_m = 0;
    				new_event.ts_x_l = 0;
    				new_event.ts_y_m = 0;
    				new_event.ts_y_l = 0;
    			}	
    			else if(event_status & STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_TOUCHSCREEN) 
    			{
    				if(event_status & STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_PRESS)
    				{
    					new_event.event_type = TOUCH_PRESSED;
    				}
    				else if(event_status & STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_RELEASE)
    				{
    					new_event.event_type = TOUCH_RELEASED;
    				}
    				else
    				{
    				    new_event.event_type = -1; 
    				}
    			    SPEAR_I2C_Read(16,TOUCH_KEYBOARD_FIFO_DATA_FIELD, read_buffer, 4);    
    				new_event.ts_x_m = read_buffer[0];
    				new_event.ts_x_l = read_buffer[1];
    				new_event.ts_y_m = read_buffer[2];
    				new_event.ts_y_l = read_buffer[3];
    				if(event_status & STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_PRESS)
    				{
    					prevx = (((unsigned int) read_buffer[0]) << 8) + ((unsigned int) read_buffer[1]); //Fred for drag and drop ???
    					prevy = (((unsigned int) read_buffer[2]) << 8) + ((unsigned int) read_buffer[3]);
    				}
    			}
                else
                {
                    new_event.event_type = -1; 
                }
                if(new_event.event_type != -1)
                {
                    // post event
	                QueueEventFifo(&new_event);
                    eventqueued = 1;	            
                }                    
        		SPEAR_I2C_Write1(TOUCH_KEYBOARD_FIFO_CTL,STM8_FLAG_TOUCH_KEYBOARD_FIFO_CONTROL_RELEASE_FIFO);
    	    }
    	    while (fifo_not_empty);
    	}
    }

	if(eventqueued)
	{
	    kill_fasync (&stm8_dev.stm8_async_queue, SIGIO, POLL_IN);
	    UnlockFifo();
	}
    
    SPEAR_I2C_ReleaseAccess();
	
	GPIO1_IE |= 0x02; // enable pio irq
				
}







static int stm8_thread(void * arg)
{
    //allow_signal(SIGKILL);
    stm8sourceinterrupt = 0;
    while (!kthread_should_stop())
    {
        wait_event_interruptible(stm8interruptqueue, (stm8sourceinterrupt !=0 ) || kthread_should_stop());
        if(kthread_should_stop())
        {
            free_irq(IRQ_STM8, stm8_interrupt);
	        free_irq(IRQ_TIMER_BST22, stm8_timer_interrupt);
            return 0;
        }
        if(stm8sourceinterrupt & STM8INTERRUPT)
        {
            stm8sourceinterrupt &= ~STM8INTERRUPT;
            stm8handleinterrupt();            
        }
        if(stm8sourceinterrupt & STM8TIMERINTERRUPT)
        {
            stm8sourceinterrupt &= ~STM8TIMERINTERRUPT;
            stm8handletimerinterrupt();               
        }
    }
  	free_irq(IRQ_STM8, stm8_interrupt);
	free_irq(IRQ_TIMER_BST22, stm8_timer_interrupt);
    return 0;
}





/******************************************************************************
                                    MODULE
******************************************************************************/

static struct file_operations stm8_operations =
{	
	.owner 	= THIS_MODULE,
	.open   = stm8_open,
	.ioctl  = stm8_ioctl,
	.release = stm8_release,
	.fasync	 = stm8_fasync
};

#if 0
struct task_struct *t;

static ssize_t write_pid(struct file *file, const char __user *buf,
                                size_t count, loff_t *ppos)
{
	char mybuf[10];
	int pid = 0;
	int copied;
	/* read the value from user space */
	if(count > 10)
		return -EINVAL;
	copied = copy_from_user(mybuf, buf, count);
	sscanf(mybuf, "%d", &pid);
	printk("pid = %d\n", pid);

	rcu_read_lock();
	t = find_task_by_pid_type(PIDTYPE_PID, pid);  //find the task_struct associated with this pid
	if(t == NULL){
		printk("no such pid\n");
		rcu_read_unlock();
		return -ENODEV;
	}
	rcu_read_unlock();

	return count;
}

static const struct file_operations file_fops = {
  .write = write_pid,
};
#endif

/*
-------------------------------------------------------------------------------
PRIVATE: STM8_Init

Invoked when the module is dynamically loaded.
-------------------------------------------------------------------------------
*/
static int __init STM8_Init(void)
{
	int rc = 0;
	u8 temp = 0;
	unsigned int * mapreg32;

    STM8_TRACE("stm8 initialization: enter\n");

	init_waitqueue_head(&stm8queue);
	init_waitqueue_head(&stm8interruptqueue);
	InitFifoQ();

    /* Register char device driver */	
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
		printk("***unable to register character device***\n");
		goto out;
	}	

	// register remapping
	mappedGPIO0 = ioremap(SPEARBASIC_START_APB_GPIO0,0x800); 
	mappedGPIO1 = ioremap(SPEARBASIC_START_APB_GPIO1,0x800); 
	mappedTIMERB22 = ioremap(SPEARBASIC_START_BASIC_TMR1,0x400);
	
	//for SPEAr LED control
    GPIO0_DIR |= 0x23;
	
	//Interrupt management
	GPIO1_DIR &= 0xFD; //GPIO1_DIR : set pios 1 as input
	GPIO1_IS |= 0x02; //GPIO1_IS : leveltriggered interrupt
	GPIO1_IEV &= 0xFD; //GPIO1_IEV : low level triggered interrupt
	GPIO1_IE &= 0xFD; //GPIO1_IE : disable interrupt
    GPIO1_IC = 0x02;  //GPIO1_IC (WO) : clear interrupt

    stm8irqthread = kthread_run(stm8_thread, NULL, "stm8irqthread");    
    if(stm8irqthread == NULL)
    {
        printk(KERN_ERR "stm8: Failed to create kernel thread\n");
		goto out;
    }
    

	/* register interrupt for STM8 */
	rc = request_irq(IRQ_STM8, (void *)stm8_interrupt,SA_INTERRUPT | SA_SHIRQ,"stm8 irq",stm8_interrupt);
	if (rc == -EINVAL || rc == -EBUSY) 
	{
		printk(KERN_ERR "stm8: Failed to register irq, return value = %d\n",rc);
		goto out;
	}

	
    // RAS configuration
	mapreg32 = ioremap(SPEARBASIC_RAS1,4);*mapreg32 = 0x00002ff4;iounmap(mapreg32);
	mapreg32 = ioremap(SPEARBASIC_START_MISC_REG,4);temp = (*mapreg32 & 0x7fffffff ) | 0x40000000;
	*mapreg32 = temp;iounmap(mapreg32);
	
	////////////
	//drag&drop default status initialization
	////////////
	stm8_drag_conf.status = DRAG_OFF;
	stm8_drag_conf.time_resolution = 10000;//about 10 millisec
	stm8_drag_conf.pixel_threshold = 1;
			
	//init I2C, to clear events and flags
	SPEAR_I2C_Init();
	SPEAR_I2C_GetAccess(0x51);
	SPEAR_I2C_Write1(TOUCH_KEYBOARD_FIFO_CTL,STM8_FLAG_TOUCH_KEYBOARD_FIFO_CONTROL_CLEAR_FIFO);
	
	//clearing error flags
	SPEAR_I2C_Write1 (STATUS_CONTROL_ERR_FLAGS, 0x0);	
	temp = SPEAR_I2C_Read1(0x1E01);
	
	//clearing wakeup events
	SPEAR_I2C_Write1 (POWER_DOWN_CONTROL_WAKEUP_SOURCE1, 0x0);//clear event	
	temp = SPEAR_I2C_Read1(0x1E01);
	
#ifdef STM8_MANAGE_HOOK_EVENTS
	temp = SPEAR_I2C_Read1(INPUT_SIGNAL_CONTROL_STATUS);//read hook status?
	SPEAR_I2C_Write1 (INPUT_SIGNAL_CONTROL_STATUS, 0x0);//clear hook event
	temp = SPEAR_I2C_Read1(0x1E01);
#endif
			 
#ifdef FORCE_HFREE_AMP_ON_INIT
    temp = SPEAR_I2C_Read1(INPUT_SIGNAL_CONTROL_CONTROL);
	temp |= STM8_FLAG_INPUT_SIGNAL_CONTROL_PIN32_AMP;
	SPEAR_I2C_Write1(INPUT_SIGNAL_CONTROL_CONTROL, temp);
	stm8_features_status.pin32_amp = ON;
#endif

    SPEAR_I2C_ReleaseAccess();

	stm8_timer_init();	 
    GPIO1_IE |= 0x02; //GPIO1_IE : enable interrupt		 
	
	printk("stm8 module loaded, version %s\n", STM8_DRIVER_VERSION);

out:
	return rc;
}

/*
-------------------------------------------------------------------------------
PRIVATE: STM8_Exit

Invoked when the module is dynamically unloaded.
-------------------------------------------------------------------------------
*/
static void __exit STM8_Exit(void)
{
    kthread_stop(stm8irqthread);
    SPEAR_I2C_Close();    

	iounmap(mappedGPIO0); 
    iounmap(mappedGPIO1); 
	iounmap(mappedTIMERB22);
	
	cdev_del(&stm8_dev.i2c_cdev) ;
	unregister_chrdev_region(stm8_I2C_devno, 1);
}

/*
-------------------------------------------------------------------------------
Standard module declarations
-------------------------------------------------------------------------------
*/

module_init(STM8_Init);
module_exit(STM8_Exit);

MODULE_DESCRIPTION("SPEAr300/STM8 driver v2");
MODULE_AUTHOR("STMicroelectronics");
MODULE_LICENSE("GPL");                           

/******************************************************************************
                                      EOF
******************************************************************************/ 

