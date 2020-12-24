/*
 * $RCSfile: aess_eventhandlerdrv.c,v $
 * $Revision: 1.2 $
 * $Date: 2008/12/17 11:19:20 $
 * $Author: solowu $
 *
 * Aspeed Kernel Event handler driver.
 *  
 * Copyright (C) 2006 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */
#define AESSEVENTHANDLER_C
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/spinlock.h>
#include <asm/uaccess.h>
#include "aess_eventhandlerdrv.h"
 
/* Debugging Message */
#ifdef EVT_DEBUG
#define DEBUG_MSG(fmt, args...) printk( "EVT: " fmt, ## args)
#else
#define DEBUG_MSG(fmt, args...)
#endif

/* Linux device data structure */ 
dev_t evt_dev;
struct cdev *eventhandler_cdev; 

/* driver name will passed by insmod command */
static char *driver_name="aess_eventhandlerdrv";
 
/* Init flag */
static UINT8 S_u8InitFlag = INIT_FAIL;


static DECLARE_WAIT_QUEUE_HEAD(wq);

/* Used for protect write command */
static UINT8 S_u8TotalEvent = 0;
static sEventData S_EventBuf[MAX_EVENT_NUMBER];
static sEventData *S_pHead;
static sEventData *S_pTail;
static spinlock_t S_spinlock = SPIN_LOCK_UNLOCKED;

static int aess_eventhandler_open(struct inode *inode, struct file *filp)
{
	UINT16 u16i = 0;
	if (S_u8InitFlag == INIT_FAIL)
	{
		printk(KERN_DEBUG "KN:start init aess_eventhandler_driver\n");          
		for (u16i = 0; u16i < MAX_EVENT_NUMBER; u16i++)
		{
			/* Set the initial value for data structure */
			S_EventBuf[u16i].u16DriverID = 0xffff;
			S_EventBuf[u16i].u32EventID = 0xffffffff;
		}
		
		/* Initial ring buffer, head and tail pointer */
		S_pHead = &S_EventBuf[1];
		S_pTail = &S_EventBuf[0];
		
		S_u8InitFlag = INIT_OK;
		printk(KERN_DEBUG "KN:finish init aess_eventhandler_driver!\n");      
	}
	else
	{
		/* fullfw reset, open node again */
		for (u16i = 0; u16i < MAX_EVENT_NUMBER; u16i++)
		{
			/* Set the initial value for data structure */
			S_EventBuf[u16i].u16DriverID = 0xffff;
			S_EventBuf[u16i].u32EventID = 0xffffffff;
		}
		
		/* Initial ring buffer, head and tail pointer */
		S_pHead = &S_EventBuf[1];
		S_pTail = &S_EventBuf[0];        
		
		/* Clear the total event count */
		S_u8TotalEvent = 0;
	}
	return (STATUS_OK);      
}

static int aess_eventhandler_close(struct inode* inode, struct file *flip)
{
	DEBUG_MSG("aess_eventhandler_close exit!\n");
	return 0;
}

int aess_generate_driver_event(UINT16 u16DriverID, UINT32 u32EventID)
{
	unsigned long u32Flag = 0;
	/* If nobody open node */
	if (S_u8InitFlag == INIT_FAIL)
	{
		return -STATUS_FAIL;
	}
	
	if (S_u8TotalEvent == MAX_EVENT_NUMBER)
	{
		/* Event buffer full */
		return -STATUS_FAIL;
	}    
	
	/* Use spin lock method, need to make sure there aren't too many 
	   things need to do in this area */
	spin_lock_irqsave(&S_spinlock, u32Flag);
	
	S_pHead->u16DriverID = u16DriverID;
	S_pHead->u32EventID = u32EventID;
	
	/* If the head doesn't arrive the end of buffer then move to next buffer */
	if (S_pHead != &S_EventBuf[MAX_EVENT_NUMBER - 1])
	{
		S_pHead++;    
	}
	/* If the head arrives the end of buffer then move to buffer start address */
	else
	{
		S_pHead = &S_EventBuf[0];
	}
	S_u8TotalEvent++;
	
                
	wake_up_interruptible(&wq);
	
	spin_unlock_irqrestore(&S_spinlock, u32Flag);
	return (STATUS_OK);
}

static int aess_eventhandler_read(struct file *filp, char *buf, 
								  size_t count, loff_t *ppos)
{
  unsigned long u32Flag = 0;
  INT32 s32RetVal =  STATUS_OK;
	
	/* If no data (S_u8TotalEvent = 0), go to sleep */
	wait_event_interruptible(wq, S_u8TotalEvent > 0);

	/* If the head doesn't arrive the end of buffer then move to next buffer */
	if (S_pTail != &S_EventBuf[MAX_EVENT_NUMBER - 1])
	{
		S_pTail++;
	}
	/* If the head arrives the end of buffer then move to buffer start address */
	else
	{
		S_pTail = &S_EventBuf[0];
	}       
	
	/* Copy data to user mode */
	if(copy_to_user(buf ,(char *) S_pTail, sizeof(sEventData)))
	{ 
	  s32RetVal = -EIO;
	}

	spin_lock_irqsave(&S_spinlock, u32Flag);
	S_u8TotalEvent--;        
	spin_unlock_irqrestore(&S_spinlock, u32Flag);
	return s32RetVal;

}



struct file_operations aess_eventhandler_fops = {
	.open = aess_eventhandler_open, 
	.read = aess_eventhandler_read,
	.release = aess_eventhandler_close,        
};

int __init aess_eventhandler_init(void)
{
	int result;
	
	printk(KERN_DEBUG "KN:init_aess_mem_module\n");
	
	eventhandler_cdev = cdev_alloc();

	eventhandler_cdev->ops = &aess_eventhandler_fops;
	result = alloc_chrdev_region(&evt_dev, 0, 1, driver_name);

	printk("mknod /dev/aess_eventhandlerdrv c %d 0\n", MAJOR(evt_dev));

	if (result < 0) {
		printk (KERN_ERR "KN:Registering the event handler device failed with %d\n", MAJOR(evt_dev));
		return result;
	}

	cdev_add(eventhandler_cdev, evt_dev, 1);
	return (STATUS_OK);
}

static void __exit aess_eventhandler_exit(void)
{
	DEBUG_MSG("aess_eventhandler_exit\n");
	cdev_del(eventhandler_cdev);
	unregister_chrdev_region(evt_dev, 1);
}



MODULE_DESCRIPTION("AESS Event Handler Driver");
MODULE_AUTHOR("Kopi Hsu <kopi.hsu@avocent.com>");
MODULE_LICENSE("GPL");
module_param(driver_name, charp, S_IRUGO);
module_init(aess_eventhandler_init);
module_exit(aess_eventhandler_exit);
EXPORT_SYMBOL(aess_generate_driver_event);
