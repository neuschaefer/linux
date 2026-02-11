/****************************************************************************
*
* Copyright 2010 --2012 Broadcom Corporation.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
*****************************************************************************/
#include <linux/init.h>
#include <linux/gfp.h>
#include <linux/smp.h>
#include <linux/cpu.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/platform_device.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/io.h>
#include <linux/semaphore.h>
#include <linux/spinlock.h>
#include <linux/broadcom/knllog.h>
#include <asm/uaccess.h>


#undef BLOCKER_DBG
#define BLOCKER_DBG 1

#if ((BLOCKER_DBG == 1) && defined(CONFIG_BCM_KNLLOG_SUPPORT))
#define BLOCKER_DEBUG(fmt, args...) \
	if (gKnllogIrqSchedEnable && KNLLOG_PROFILING) { \
		KNLLOG("BLOCKER: " fmt, ## args); \
	}
#else
#define BLOCKER_DEBUG(fmt, args...)
#endif

static atomic_t blocker_count = ATOMIC_INIT(0);

static void blocker(void *info)
{
	unsigned long flags;

	BLOCKER_DEBUG("Blocking cpu %d\n", smp_processor_id());
	local_irq_save(flags);
	atomic_inc(&blocker_count);
	while (atomic_read(&blocker_count))
		;
	local_irq_restore(flags);
	BLOCKER_DEBUG("Unblocking cpu %d\n", smp_processor_id());
}

typedef int (*pause_fn_t) (void *arg);

int pause_other_cpus(pause_fn_t fn, void *arg)
{
	int ret = 0;
	unsigned long flags;
	int online_cpus_cnt;
	unsigned long count;

	preempt_disable();
	BLOCKER_DEBUG("Running pause on cpu %d\n", smp_processor_id());
	online_cpus_cnt = num_online_cpus();
	if (online_cpus_cnt > 1) {
		count = jiffies + HZ * 2;
		smp_call_function(blocker, NULL, false);
		while (time_before(jiffies, count)) {
			if (atomic_read(&blocker_count) + 1 >= online_cpus_cnt)
				break;
		}

		if (!time_before(jiffies, count)) {
			pr_err("BLOCKER: Failed %s, online:%d, count:%d\n",
				__func__, online_cpus_cnt,
				(int)atomic_read(&blocker_count));
			atomic_set(&blocker_count, 0);
			ret = -1;
			goto error;
		}
	}
	local_irq_save(flags);
	BLOCKER_DEBUG("In critical section on cpu %d\n", smp_processor_id());
	if (fn)
		ret = fn(arg);
	atomic_set(&blocker_count, 0);
	local_irq_restore(flags);
error:
	BLOCKER_DEBUG("Finishing pause on cpu %d\n", smp_processor_id());
	preempt_enable();
	return ret;
}
EXPORT_SYMBOL(pause_other_cpus);

static int blocker_open(struct inode *inode, struct file *filp)
{
	BLOCKER_DEBUG("BLOCKER device opened\n");
	pause_other_cpus(NULL, NULL);
	return 0;
}

static int blocker_release(struct inode *inode, struct file *filp)
{
	BLOCKER_DEBUG("BLOCKER device closed\n");
	return 0;
}

static long blocker_ioctl(struct file *filp,
			  unsigned int cmd, unsigned long arg)
{
	BLOCKER_DEBUG("BLOCKER device IOCTL\n");
	return 0;
}

const struct file_operations blocker_fops = {
	.owner = THIS_MODULE,
	.open = blocker_open,
	.release = blocker_release,
	.unlocked_ioctl = blocker_ioctl,
};

static char banner[] __initdata = KERN_INFO "Broadcom BLOCKER Driver\n";

#define BCM_BLOCKER_MAJOR 200
#define BCM_BLOCKER_DEV_NAME "blocker"

struct class *dev_class;
struct device *dev;

static int __init kona_blocker_init(void)
{
	int rval;
	printk(banner);

	rval =
	    register_chrdev(BCM_BLOCKER_MAJOR, BCM_BLOCKER_DEV_NAME,
			    &blocker_fops);
	if (rval < 0) {
		printk("BLOCKER: register_chrdev failed for major %d\n",
		       BCM_BLOCKER_MAJOR);
		return rval;
	} else {
		BLOCKER_DEBUG("BLOCKER: class create succeeded, major: %d\n",
			      BCM_BLOCKER_MAJOR);
	}

	/* Create the device */
	dev_class = class_create(THIS_MODULE, BCM_BLOCKER_DEV_NAME);
	if (IS_ERR(dev_class)) {
		rval = PTR_ERR(dev_class);
		printk(KERN_ERR "class create failed: %d\n", rval);
	} else {
		BLOCKER_DEBUG("BLOCKER: class create succeeded\n");
	}

	dev =
	    device_create(dev_class, NULL, MKDEV(BCM_BLOCKER_MAJOR, 0), NULL,
			  BCM_BLOCKER_DEV_NAME);

	return 0;
}

static void __exit kona_blocker_exit(void)
{
	device_destroy(dev_class, MKDEV(BCM_BLOCKER_MAJOR, 0));
	class_destroy(dev_class);
	unregister_chrdev(BCM_BLOCKER_MAJOR, BCM_BLOCKER_DEV_NAME);

}

module_init(kona_blocker_init);
module_exit(kona_blocker_exit);

MODULE_AUTHOR("Broadcom");
MODULE_DESCRIPTION("BLOCKER Driver");
MODULE_LICENSE("GPL");
