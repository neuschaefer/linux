/*
 * $RCSfile$
 * $Revision$
 * $Date$
 * $Author$
 *
 * AESS Hardware Watchdog Driver.
 *  
 * Copyright (C) 2006 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <asm/arch/map.h>

#include <asm/bitops.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include "aess_wdtdrv.h"


//#define VWDT_DEBUG
/* Debugging Message */
#ifdef VWDT_DEBUG
#define DEBUG_MSG(fmt, args...) printk( "WDT: " fmt, ## args)
#else
#define DEBUG_MSG(fmt, args...)
#endif //VWDT_DEBUG

/* driver name will passed by insmod command */
static char *driver_name = "aess_wdtdrv"; 
static int nowayout = WATCHDOG_NOWAYOUT;

unsigned char wdt_count = 0;

module_param(nowayout, int, 0);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started (default=CONFIG_WATCHDOG_NOWAYOUT)");

static struct vwdt_private vwdt_pri[VWDT_MAX_DEVS];


/* Linux device data structure */
dev_t wdt_dev;
struct cdev *wdt_cdev;


/*
 *	Allow only one person to hold it open
 */
static int aess_wdt_open(struct inode *inode, struct file *file)
{
	struct vwdt_private *lp;
	int minor;
	int wdt_base;

	minor = iminor(inode);

	if ((minor-WATCHDOG_MINOR) > VWDT_MAX_DEVS) {
		printk("Error minor number!\n");
		return -ENODEV;
	}

//	lp = &vwdt_pri[minor-WATCHDOG_MINOR];
	lp = &vwdt_pri[0];
	wdt_base = lp->base_addr;

	DEBUG_MSG("minor  = %d\n", minor);
	DEBUG_MSG("base  = 0x%08x\n", lp->base_addr);
	DEBUG_MSG("used  = %d\n", lp->used);
	DEBUG_MSG("cstat  = %d\n", lp->cstat);
	DEBUG_MSG("count = %d\n", lp->count);
	DEBUG_MSG("nowayout = %d\n", nowayout);

	nonseekable_open(inode, file);
	if (test_and_set_bit(0,&lp->used))
		return -EBUSY;

	if (nowayout)
		__module_get(THIS_MODULE);

	file->private_data = lp;

	/* Activate Winbond WPCM450 Watchdog timer */
	/* Watchdog Timer Interval Select */
	/* Watchdog Timer Clock input divided by 256 */
	test_and_set_bit(WDT_CLOCK_BIT10, (void *)wdt_base);
	switch(lp->count)
	{
		case WDT_017SEC_COUNT:
			test_and_clear_bit(WDT_INTERVAL_SELECT_BIT4, (void *)wdt_base);
			test_and_clear_bit(WDT_INTERVAL_SELECT_BIT5, (void *)wdt_base);
			break;
		case WDT_067SEC_COUNT:
			test_and_set_bit(WDT_INTERVAL_SELECT_BIT4, (void *)wdt_base);
			test_and_clear_bit(WDT_INTERVAL_SELECT_BIT5, (void *)wdt_base);
			break;
		case WDT_268SEC_COUNT:
			test_and_clear_bit(WDT_INTERVAL_SELECT_BIT4, (void *)wdt_base);
			test_and_set_bit(WDT_INTERVAL_SELECT_BIT5, (void *)wdt_base);
			break;
		case WDT_107SEC_COUNT:
		default:
			test_and_set_bit(WDT_INTERVAL_SELECT_BIT4, (void *)wdt_base);
			test_and_set_bit(WDT_INTERVAL_SELECT_BIT5, (void *)wdt_base);
			break;
					
	}

	/* start WDT */
	test_and_set_bit(WDT_INTERRUPT_ENABLE_BIT6, (void *)wdt_base);
	test_and_set_bit(WDT_RESET_ENABLE_BIT1, (void *)wdt_base);
	test_and_set_bit(WDT_TIMER_ENABLE_BIT7, (void *)wdt_base);
	test_and_set_bit(WDT_RESET_BIT0, (void *)wdt_base);

	DEBUG_MSG("\n reg = 0x%x \n", ioread32((void *)wdt_base));
	return 0;
}


static int aess_wdt_keepalive(struct file *file)
{
	struct vwdt_private *lp = file->private_data;
	int wdt_base = lp->base_addr;

	#ifdef VWDT_DEBUG
	if (ioread32((void *)wdt_base) & (1<< WDT_INTERRUPT_FLAG_BIT3)) {
		DEBUG_MSG("The HW WDT interrup has occurred, before update it.\n");
	}
	#endif

	/* reset WDT count register */
	test_and_set_bit(WDT_RESET_BIT0, (void *)wdt_base);
	wdt_count++;
	DEBUG_MSG("\n aess_wdt_keepalive %d\n", wdt_count);
	return 0;
}

static int aess_wdt_release(struct inode *inode, struct file *file)
{
	struct vwdt_private *lp = file->private_data;
	int wdt_base = lp->base_addr;

	clear_bit(0, &lp->used);

	if (WDT_CLOSE_ALLOW == lp->cstat) {
		/* close WDT */
		test_and_clear_bit(WDT_TIMER_ENABLE_BIT7, (void *)wdt_base);
		test_and_clear_bit(WDT_INTERRUPT_ENABLE_BIT6, (void *)wdt_base);
		test_and_clear_bit(WDT_RESET_ENABLE_BIT1, (void *)wdt_base);
	} else {
		printk(KERN_CRIT "WDT : Unexpected close, timer will not stop!\n");
		aess_wdt_keepalive(file);
	}

	DEBUG_MSG("base  = 0x%08x\n", lp->base_addr);
	DEBUG_MSG("used  = %d\n", lp->used);
	DEBUG_MSG("cstat = %d\n", lp->cstat);
	DEBUG_MSG("count = %d\n", lp->count);
	return 0;
}

static ssize_t aess_wdt_write(struct file *file, const char *buf,
				size_t len, loff_t *ppos)
{
	struct vwdt_private *lp = file->private_data;

	/* only updata WDT if len is ture */
	if(len) {
		/* Chelck 'no way out' feature */
		if (!nowayout) {
			size_t i;
			lp->cstat = WDT_CLOSE_NOT_ALLOW;
			for (i = 0; i != len; i++) {
				char c;
				if (get_user(c, buf + i))
					return -EFAULT;
				if ('V' == c)
					lp->cstat = WDT_CLOSE_ALLOW;
			}
		}
		/* Update the WDT to keep-alive */
		aess_wdt_keepalive(file);
	}
	return len;
}

static struct watchdog_info ident = {
	.options	= WDIOF_MAGICCLOSE | WDIOF_SETTIMEOUT | WDIOF_KEEPALIVEPING,
	.firmware_version = 100,
	.identity	= "AESS Watchdog",
};

static int aess_wdt_ioctl(struct inode *inode, struct file *file,
	unsigned int cmd, unsigned long arg)
{
	struct vwdt_private *lp = file->private_data;
	int ret = -ENOIOCTLCMD;
	int time;
	void __user *argp = (void __user *)arg;
	int __user *p = argp;

	switch (cmd) {
	case WDIOC_GETSUPPORT:
		ret = copy_to_user(argp, &ident, sizeof(ident));
		if (ret != 0)
			ret = -EFAULT;
		break;

	case WDIOC_GETSTATUS:
	case WDIOC_GETBOOTSTATUS:
		ret = put_user(0, p);
		break;

	case WDIOC_SETTIMEOUT:
		ret = get_user(time, p);
		if (ret)
			break;

		if (time < 0 || time > 3) {
			ret = -EINVAL;
			break;
		}
		/* set WDT count */
		lp->count = time;
		aess_wdt_keepalive(file);
		ret = put_user(time, p);
		break;

	case WDIOC_GETTIMEOUT:
		time = lp->count;
		ret = put_user(time, p);
		break;

	case WDIOC_KEEPALIVE:
		aess_wdt_keepalive(file);
		ret = 0;
		break;
	}
	return ret;
}

static struct file_operations aess_wdt_fops =
{
	.owner		= THIS_MODULE,
	.llseek		= no_llseek,
	.write		= aess_wdt_write,
	.ioctl		= aess_wdt_ioctl,
	.open		= aess_wdt_open,
	.release	= aess_wdt_release,
};

static struct miscdevice aess_wdt_miscdev[VWDT_MAX_DEVS] =
{
	[0] = {
		.minor		= WATCHDOG_MINOR,
		.name		= "aess_wdtdrv",
		.fops		= &aess_wdt_fops,
	},
};

#if 1
static int __init aess_wdt_init(void)
{
	int i,ret;
	struct vwdt_private *lp;

	if (ioread32((void *)WDT_REG_CONTROL_ADDR) & (1<< WDT_RESET_FLAG_BIT2)) {
		printk("****************************** \n");
		printk("The HW WDT reset has occurred.\n");
		printk("****************************** \n");
		test_and_clear_bit(WDT_RESET_FLAG_BIT2, (void *)WDT_REG_CONTROL_ADDR);
	}

	for(i=0 ; i<VWDT_MAX_DEVS ; i++) {
		/* init wdt private data */
		lp = &vwdt_pri[i];
		switch (i) {
			case 0:
				lp->base_addr = WDT_REG_CONTROL_ADDR;
				lp->cstat = WDT_CLOSE_NOT_ALLOW;
				lp->count = WDT_107SEC_COUNT;
				break;
			default:
				return -1;
		}

		/* WDT register */
		ret = misc_register(&aess_wdt_miscdev[i]);
		if (0 == ret)
			DEBUG_MSG("AESS Watchdog Timer %d: timer margin 10 sec\n",i);
		else
			return -1;
	}
	//printk("mknod /dev/aess_wdtdrv c %d 0\n", MINOR(WATCHDOG_MINOR));
	printk("mknod /dev/aess_wdtdrv c 10 %d\n", WATCHDOG_MINOR);

	return ret;
}

static void __exit aess_wdt_exit(void)
{
	int i;
	for (i=0; i<VWDT_MAX_DEVS; i++) {
		misc_deregister(&aess_wdt_miscdev[i]);
	}
}
#else
static int __init aess_wdt_init(void)
{
	int result;
	int i = 0;
	struct vwdt_private *lp;
	
	DEBUG_MSG(KERN_DEBUG "KN:init_aess_wdt_module\n");
	
		lp = &vwdt_pri[i];
		switch (i) {
			case 0:
				lp->base_addr = WDT_REG_CONTROL_ADDR;
				lp->cstat = WDT_CLOSE_NOT_ALLOW;
				lp->count = WDT_107SEC_COUNT;
				break;
			default:
				return -1;
		}
	wdt_cdev = cdev_alloc();
   
	wdt_cdev->ops = &aess_wdt_fops;
	result = alloc_chrdev_region(&wdt_dev, 0, 1, driver_name);

	printk("mknod /dev/aess_wdtdrv c %d 0\n", MAJOR(wdt_dev));

	if (result < 0) {
		DEBUG_MSG (KERN_ERR "KN:Registering the WDT character device failed with %d\n", MAJOR(wdt_dev));
		return result;
	}

	cdev_add(wdt_cdev, wdt_dev, 1);
	return (STATUS_OK);

}

static void __exit aess_wdt_exit(void)
{
	DEBUG_MSG("aess_wdtdrv_exit\n");
	cdev_del(wdt_cdev);
	unregister_chrdev_region(wdt_dev, 1);
}
#endif

module_init(aess_wdt_init);
module_exit(aess_wdt_exit);

MODULE_DESCRIPTION("AESS Hardware Watchdog Driver");
MODULE_AUTHOR("Ivan.Huang@avocent.com");
MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
module_param(driver_name, charp, S_IRUGO);

