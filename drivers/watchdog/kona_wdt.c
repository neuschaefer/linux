/*
 * Watchdog driver for the KONA architecture
 *
 * Copyright (C) 2011 Broadcom Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation
 */

/*
 * Frameworks:
 *
 *    - SMP:          Fully supported.    Locking is in place where necessary.
 *    - GPIO:         Fully supported.    No GPIOs are used.
 *    - MMU:          Fully supported.    Platform model with ioremap used.
 *    - Dynamic /dev: Fully supported.    Registers itself as /dev/watchdog with misc device as standard.
 *    - Suspend:      Fully supported.    Suspend/resume disable/enable watchdog and clock.
 *    - Clocks:       Fully supported.
 *    - Power:        Fully supported.    Clocks only enabled when watchdog is running.
 *
 */
#include <linux/oom.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/timer.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
#include <linux/init.h>
#include <linux/bitops.h>
#include <linux/uaccess.h>
#include <mach/hardware.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/clk.h>
#include <mach/rdb/brcm_rdb_secwatchdog.h>
#include <mach/rdb/brcm_rdb_root_rst_mgr_reg.h>

extern int printk_time;
extern int is_roku_application(struct task_struct *t);
extern unsigned ltcore_dump_cnt;

struct kona_wdt {
	struct device *dev;
	struct cdev cdev;
	dev_t number;
	struct class *class;
	struct clk *clock;
	unsigned long status;
	spinlock_t lock;
	void __iomem *base;
};

static struct platform_device *kona_wdt_dev;
static unsigned reset_status = 0;

#define WARNING_PRETIMEOUT      6               /* warning 6 secs before reboot */

#define TIMER_MARGIN	60
static int nowayout = WATCHDOG_NOWAYOUT;
static unsigned int heartbeat = TIMER_MARGIN;   /* (secs) Default is 1 minute */

#define	WDT_IN_USE		0
#define	WDT_OK_TO_CLOSE		1
#define	WDT_CLK_RESOLUTION	4	/* 62.5ms */
#define	WDT_TICK_RATE			16	/* 16 ticks per sec */

/* Some macros to make register manipulation code a little less noisy. */
#define WDT_FIELD(name,val)   mk_field((val), SECWATCHDOG_SDOGCR_##name##_SHIFT, SECWATCHDOG_SDOGCR_##name##_MASK)
#define WDT_READL(name)       readl(wdt->base + SECWATCHDOG_##name##_OFFSET)
#define WDT_WRITEL(val,name)  writel((val), wdt->base + SECWATCHDOG_##name##_OFFSET)

struct kona_wdt *wdt;


static unsigned long kona_wdt_timeleft(void)
{
	unsigned long val;

	val = WDT_READL(SDOGR);
	val &= SECWATCHDOG_SDOGR_CNT_MASK;
	return val;
}

#ifndef WARNING_PRETIMEOUT

#define warning_set()
#define warning_disable()

#else

#include <linux/hrtimer.h>
#include <linux/sysrq.h>
#include <linux/sched.h>

static struct semaphore warning_sem;

static void wake_wdog_listener(void)
{
	up(&warning_sem);
}

static unsigned int pretimeout = WARNING_PRETIMEOUT; 

static void dump_tasks(void)
{
	struct task_struct *p;
	struct task_struct *task;

	read_lock(&tasklist_lock);
	printk("[ pid ]   uid  tgid total_vm      rss name\n");
	for_each_process(p) {
		task = find_lock_task_mm(p);
		if (!task) {
			continue;
		}

		printk("[%5d] %5d %5d %8lu %8lu %s\n",
			task->pid, task_uid(task),
			task->tgid, task->mm->total_vm, get_mm_rss(task->mm),
			task->comm);
		task_unlock(task);
	}
	read_unlock(&tasklist_lock);
}

/* Callback for when the pretimeout expires. */
static void warning_trigger(unsigned long arg)
{
	struct task_struct *p;
	int app_found = 0;
	int save_printk_time = printk_time;
	int save_loglevel = console_loglevel;
	unsigned long timeleft = kona_wdt_timeleft();

	printk("Watchdog Warning with %ld + %ld/%d sec on timer\n",
		timeleft / WDT_TICK_RATE, timeleft % WDT_TICK_RATE, WDT_TICK_RATE);
	console_loglevel = 0; // Disable printing to the console to allow this to complete
	printk_time = 0;
	handle_sysrq('t'); /* dump scheduler state */
	handle_sysrq('m'); /* dump memory stats */
	print_modules();
	dump_tasks();
	dump_stack();
	wake_wdog_listener();
	printk_time = save_printk_time;
	console_loglevel = save_loglevel;

#if 1 /* ifdef CONFIG_SPECIAL_APPLICATION */
	read_lock(&tasklist_lock);
	for_each_process(p) {
		if (is_roku_application(p)) {
			app_found = 1;
			printk("sending SIGKILL to %s [%d]\n", p->comm, p->pid);
			if (do_send_sig_info(SIGKILL, SEND_SIG_FORCED, p, false))
				printk("signal failed\n");
			break;
		}
	}
	read_unlock(&tasklist_lock);
	if (!app_found) {
		printk("Application not running.  ltcore size: %u\n", ltcore_dump_cnt);
	}
#endif
	timeleft = kona_wdt_timeleft();
	printk("Watchdog Warning done with %ld + %ld/%d sec on timer\n",
		timeleft / WDT_TICK_RATE, timeleft % WDT_TICK_RATE, WDT_TICK_RATE);
}

DEFINE_MUTEX(warning_mutex);
DEFINE_TIMER(warning_timer, warning_trigger, 0, 0);

/* Set pretimeout expiration to pretimeout secs before timeout. */
static void warning_set(void)
{
	int incr_sec;
	unsigned long expire;

	mutex_lock(&warning_mutex);
	incr_sec = (heartbeat <= pretimeout) ? 1 : heartbeat - pretimeout;
	expire = jiffies + msecs_to_jiffies(incr_sec * 1000);
	mod_timer(&warning_timer, expire);
	mutex_unlock(&warning_mutex);
}

/* Disable pretimeout. */
static void warning_disable(void)
{
	mutex_lock(&warning_mutex);
	del_timer_sync(&warning_timer);
	mutex_unlock(&warning_mutex);
}

#endif /* WARNING_PRETIMEOUT */

static inline unsigned long mk_field(unsigned long val, unsigned shift, unsigned long mask)
{
	val <<= shift;
	BUG_ON(val & ~mask);
	return val;
}

static inline void wait_load(void)
{
	while (WDT_READL(SDOGCR) & SECWATCHDOG_SDOGCR_WD_LOAD_FLAG_MASK)
		continue;
}

static int kona_wdt_is_enabled(void)
{
	return !!(SECWATCHDOG_SDOGCR_EN_MASK & WDT_READL(SDOGCR));
}

static void kona_wdt_enable(void)
{
	if (!kona_wdt_is_enabled())
	{
		unsigned long val;
		printk("kona_wdt: enable (%d sec)\n", heartbeat);
		warning_set();
		clk_enable(wdt->clock);
		spin_lock(&wdt->lock);
		wait_load();
		/* Sequence to enable the watchdog */
		val = WDT_READL(SDOGCR);
		val &= ~(SECWATCHDOG_SDOGCR_LD_MASK | 
			SECWATCHDOG_SDOGCR_CLKS_MASK);
		val |= WDT_FIELD(LD, heartbeat * WDT_TICK_RATE) |
			WDT_FIELD(CLKS, WDT_CLK_RESOLUTION) |
			SECWATCHDOG_SDOGCR_EN_MASK | 
			SECWATCHDOG_SDOGCR_SRSTEN_MASK;
		WDT_WRITEL(val, SDOGCR);
		wait_load();
		spin_unlock(&wdt->lock);
	}
}

static void kona_wdt_disable(void)
{
	if (kona_wdt_is_enabled())
	{
		unsigned long val;

		printk("kona_wdt: disable\n");
		warning_disable();
		spin_lock(&wdt->lock);
		wait_load();
		/* Sequence to disable the watchdog */
		val = WDT_READL(SDOGCR);
		val &= ~(SECWATCHDOG_SDOGCR_EN_MASK | 
			SECWATCHDOG_SDOGCR_SRSTEN_MASK);
		WDT_WRITEL(val, SDOGCR);
		wait_load();
		spin_unlock(&wdt->lock);
		clk_disable(wdt->clock);
	}
}

static void kona_wdt_keepalive(void)
{
	if (kona_wdt_is_enabled())
	{
		unsigned long val;

		warning_set();
		spin_lock(&wdt->lock);
		wait_load();
		val = WDT_READL(SDOGCR);
		val &= ~(SECWATCHDOG_SDOGCR_LD_MASK);
		val |= WDT_FIELD(LD, heartbeat * WDT_TICK_RATE);
		WDT_WRITEL(val, SDOGCR);
		wait_load();
		spin_unlock(&wdt->lock);
	}
}

static int kona_wdt_set_heartbeat(int t)
{
	if (t < 0x0000 || t > 0xFFFF)
		return -EINVAL;
	if (t == heartbeat) {
		/* Don't disable/reenable if we don't have to. */
		if (t == 0 && !kona_wdt_is_enabled())
			return 0;
		if (t > 0 && kona_wdt_is_enabled()) {
			kona_wdt_keepalive();
			return 0;
		}
	}
	kona_wdt_disable();
	heartbeat = t;
	if (t > 0)
		kona_wdt_enable();
	return 0;
}

static int kona_wdt_set_pretimeout(int t)
{
	if (t < 0)
		return -EINVAL;
	pretimeout = t;
	return 0;
}

static int kona_wdt_open(struct inode *inode, struct file *file)
{
	if ((file->f_flags & O_ACCMODE) != O_RDONLY) {
		if (test_and_set_bit(WDT_IN_USE, &wdt->status))
			return -EBUSY;
		clear_bit(WDT_OK_TO_CLOSE, &wdt->status);
		kona_wdt_set_heartbeat(heartbeat);
	}

	return nonseekable_open(inode, file);
}

static ssize_t kona_wdt_write(struct file *file, const char *data,
			      size_t len, loff_t * ppos)
{
	if ((file->f_flags & O_ACCMODE) == O_RDONLY)
		return -EACCES;
	if (len) {
		if (!nowayout) {
			size_t i;

			clear_bit(WDT_OK_TO_CLOSE, &wdt->status);

			for (i = 0; i != len; i++) {
				char c;

				if (get_user(c, data + i))
					return -EFAULT;
				if (c == 'V')
					set_bit(WDT_OK_TO_CLOSE, &wdt->status);
			}
		}
		kona_wdt_keepalive();
	}

	return len;
}

static const struct watchdog_info ident = {
	.options = WDIOF_MAGICCLOSE | WDIOF_SETTIMEOUT | WDIOF_PRETIMEOUT | WDIOF_KEEPALIVEPING,
	.identity = "KONA Watchdog",
	.firmware_version = 0,
};

static long kona_wdt_ioctl(struct file *file, unsigned int cmd,
			   unsigned long arg)
{
	int ret = -ENOTTY;
	int time;
	int status;

	switch (cmd) {
	case WDIOC_GETSUPPORT:
		ret = copy_to_user((struct watchdog_info *)arg, &ident,
				   sizeof(ident)) ? -EFAULT : 0;
		break;

	case WDIOC_GETSTATUS:
		ret = put_user(0, (int *)arg);
		break;

	case WDIOC_GETBOOTSTATUS:
		ret = put_user(reset_status, (int *)arg);
		break;

	case WDIOC_KEEPALIVE:
		if ((file->f_flags & O_ACCMODE) == O_RDONLY)
			return -EACCES;
		kona_wdt_keepalive();
		ret = 0;
		break;

	case WDIOC_SETTIMEOUT:
		if ((file->f_flags & O_ACCMODE) == O_RDONLY)
			return -EACCES;
		ret = get_user(time, (int *)arg);
		if (ret)
			break;
		ret = kona_wdt_set_heartbeat(time);
		if (ret)
			break;
		/* Fall through */
	case WDIOC_GETTIMEOUT:
		status = kona_wdt_is_enabled() ? heartbeat : 0;
		ret = put_user(status, (int *)arg);
		break;

	case WDIOC_GETTIMELEFT:
		time = kona_wdt_is_enabled() ? kona_wdt_timeleft() / WDT_TICK_RATE : 0;
		ret = put_user(time, (int *)arg);
		break;

	case WDIOC_SETPRETIMEOUT:
		if ((file->f_flags & O_ACCMODE) == O_RDONLY)
			return -EACCES;
		ret = get_user(time, (int *)arg);
		if (ret)
			break;
		ret = kona_wdt_set_pretimeout(time);
		if (ret)
			break;
		kona_wdt_keepalive();
		/* Fall through */
	case WDIOC_GETPRETIMEOUT:
		time = kona_wdt_is_enabled() ? pretimeout : 0;
		ret = put_user(time, (int *)arg);
		break;
	}

	return ret;
}

static int kona_wdt_release(struct inode *inode, struct file *file)
{
	if ((file->f_flags & O_ACCMODE) == O_RDONLY)
		return 0;
	if (test_bit(WDT_OK_TO_CLOSE, &wdt->status))
		kona_wdt_disable();
#if 0 /* sometimes we WANT to leave wdog running after the program that opened it exits */
	else
		 printk(KERN_CRIT "WATCHDOG: Device closed unexpectedly - "
			 "timer will not stop\n")
#endif
	clear_bit(WDT_IN_USE, &wdt->status);
	clear_bit(WDT_OK_TO_CLOSE, &wdt->status);

	return 0;
}

/*
 *	System shutdown handler.  Turn off the watchdog if we're
 *	restarting or halting the system.
 */
static void kona_wdt_shutdown(struct platform_device *dev)
{
	if (system_state == SYSTEM_RESTART || system_state == SYSTEM_HALT)
		kona_wdt_disable();
}

static const struct file_operations kona_wdt_fops = {
	.owner = THIS_MODULE,
	.llseek = no_llseek,
	.write = kona_wdt_write,
	.unlocked_ioctl = kona_wdt_ioctl,
	.open = kona_wdt_open,
	.release = kona_wdt_release,
};

static int __devinit kona_wdt_probe(struct platform_device *dev)
{
	struct resource *res;
	int ret;

	/* We only accept one device, and it must have an id of -1 */
	if (dev->id != -1)
		return -ENODEV;

	res = platform_get_resource(dev, IORESOURCE_MEM, 0);
	if (!res) {
		ret = -ENODEV;
		goto err_out;
	}

	wdt = devm_kzalloc(&dev->dev, sizeof(struct kona_wdt), GFP_KERNEL);
	if (!wdt) {
		ret = -ENOMEM;
		goto err_out;
	}

	wdt->dev = &dev->dev;

	spin_lock_init(&wdt->lock);

	wdt->clock = clk_get(wdt->dev, wdt->dev->platform_data);
	if (wdt->clock < 0) {
		ret = -ENXIO;
		goto err_out;
	}

	wdt->base = ioremap(res->start, resource_size(res));
	if (!wdt->base) {
		ret = -ENOMEM;
		goto err_clock;
	}

	ret = alloc_chrdev_region(&wdt->number, 0, 1, "kona_wdt");
	if (ret < 0) {
		dev_printk(KERN_ERR, wdt->dev,
			   "cannot register with dynamic device number (err=%d)\n",
			   -ret);
		goto err_misc;
	}
	dev_printk(KERN_INFO, wdt->dev, "using device number %d, %d\n",
		   MAJOR(wdt->number), MINOR(wdt->number));

	cdev_init(&wdt->cdev, &kona_wdt_fops);

	kona_wdt_disable();
	platform_set_drvdata(dev, wdt);
	kona_wdt_dev = dev;

	ret = cdev_add(&wdt->cdev, wdt->number, 1);
	if (ret < 0) {
		dev_printk(KERN_ERR, wdt->dev, "To add device\n");
		goto err_cdev_add;
	}

	/* create /dev/watchdog
	 * we use udev to make the file
	 */
	wdt->class = class_create(THIS_MODULE, "watchdog");
	(void)device_create(wdt->class, wdt->dev, wdt->number, NULL,
			    "watchdog");

	return 0;

 err_cdev_add:
	unregister_chrdev_region(wdt->number, 1);
 err_misc:
	iounmap(wdt->base);
 err_clock:
	clk_put(wdt->clock);
 err_out:
	return ret;
}

static int __devexit kona_wdt_remove(struct platform_device *dev)
{
	struct kona_wdt *wdt = platform_get_drvdata(dev);

	device_destroy(wdt->class, wdt->number);
	class_unregister(wdt->class);
	class_destroy(wdt->class);

	cdev_del(&wdt->cdev);

	platform_set_drvdata(dev, NULL);

	unregister_chrdev_region(wdt->number, 1);

	kona_wdt_dev = NULL;

	clk_put(wdt->clock);
	iounmap(wdt->base);
	return 0;
}

#ifdef CONFIG_PM
static int enabled;

static int kona_wdt_suspend(struct platform_device *dev, pm_message_t msg)
{
	enabled = kona_wdt_is_enabled();
	if (enabled) {
		kona_wdt_disable();	/* Turn the WDT off */
	}
	return 0;
}

static int kona_wdt_resume(struct platform_device *dev)
{
	/* re-activate timer */
	if (enabled) {
		kona_wdt_enable();
	}
	return 0;
}
#else
#define kona_wdt_suspend    NULL
#define kona_wdt_resume     NULL
#endif

/* work with hotplug and coldplug */
MODULE_ALIAS("platform:kona_wdt");

static struct platform_driver kona_wdt_driver = {
	.remove = __devexit_p(kona_wdt_remove),
	.suspend = kona_wdt_suspend,
	.resume = kona_wdt_resume,
	.shutdown = kona_wdt_shutdown,
	.driver = {
		   .owner = THIS_MODULE,
		   .name = "kona_wdt",
		   },
};

static char banner[] __initdata = KERN_INFO "Kona Watchdog Timer: 0.1. "
    "heartbeat=%d sec (nowayout= %d)\n";

static int wdog_open(struct inode *inode, struct file *filp)
{
    return 0;
}

static int wdog_release(struct inode *inode, struct file *filp)
{
    return 0;
}

/* Read on /dev/wdog_warning will return when the watchdog
 * warning is about to fire.
 */
static ssize_t wdog_warning_read(struct file *filp, char *buff, size_t count, loff_t *fpos)
{
	if (down_interruptible(&warning_sem))
		return -ERESTARTSYS;
	return 0;
}

static const struct file_operations wdog_warning_fops = {
    .owner      = THIS_MODULE,
    .llseek     = no_llseek,
    .open       = wdog_open,
    .release    = wdog_release,
    .read 	= wdog_warning_read,
};

static struct miscdevice wdog_warning_dev = {
    .minor        = MISC_DYNAMIC_MINOR,
    .name        = "wdog_warning",
    .fops        = &wdog_warning_fops,
};

static int __init kona_wdt_init(void)
{
	int ret;

	printk(banner, heartbeat, nowayout);

	/*
	 * Check that the margin value is within it's range;
	 * if not reset to the default
	 */
	if (heartbeat < 0x0001 || heartbeat > 0xFFFF) {
		heartbeat = TIMER_MARGIN;
		printk(KERN_INFO
		       "heartbeat value must be 0 < heartbeat < 65536, using %d\n",
		       heartbeat);
	}

	ret = platform_driver_probe(&kona_wdt_driver, kona_wdt_probe);

	sema_init(&warning_sem, 0);
	if (!ret) ret = misc_register(&wdog_warning_dev);
	
	if (ret < 0) {
	        pr_err("kona_wdt failed to register device\n");
	}
	return ret;
}

static void __exit kona_wdt_exit(void)
{
	platform_driver_unregister(&kona_wdt_driver);
	misc_deregister(&wdog_warning_dev);
}

static int setup_reset(char *str)
{
	if (strchr(str, 'w'))
		reset_status |= WDIOF_CARDRESET; /* watchdog reset */
	if (strchr(str, 'p'))
		reset_status |= WDIOF_POWERUNDER; /* power on */
	if (strchr(str, 's'))
		reset_status |= WDIOF_EXTERN1; /* software reset */
	if (strchr(str, 'b'))
		reset_status |= WDIOF_EXTERN2; /* reset button */
    return 1;
}
__setup("reset=", setup_reset);

module_init(kona_wdt_init);
module_exit(kona_wdt_exit);

MODULE_AUTHOR("Broadcom Inc.");
MODULE_DESCRIPTION("KONA Architecture Watchdog");

module_param(heartbeat, int, 0);
MODULE_PARM_DESC(heartbeat, "Watchdog heartbeat in seconds (default 60s)");

module_param(nowayout, int, 0);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started");

MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
