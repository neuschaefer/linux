/*****************************************************************************
* Copyright 2005 - 20012 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL"). 
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a
* license other than the GPL, without Broadcom's express prior written
* consent.
*****************************************************************************/

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

#include <asm/uaccess.h>

#include <linux/broadcom/bcm_major.h>
#include <linux/broadcom/secretkey.h>                               
#include <mach/io_map.h>

static char banner[] = KERN_INFO "Secret Key Driver\n";

struct secretkey_cfg
{
    struct class*  dev_class;
    struct device* dev;
};
struct secretkey_cfg secretkeyCfg;

struct semaphore secretkey_lock;

extern void secretkey_service_call(uint32_t mode, uint32_t* data);

static int secretkey_op(struct secretkey_data *data)
{
   int i;
   cpumask_var_t old_mask;

   /* all secure monitor calls must occur on CPU 0 */
   if (!alloc_cpumask_var(&old_mask, GFP_KERNEL))
	return -ENOMEM;

   down(&secretkey_lock);

   cpumask_copy(old_mask, &current->cpus_allowed);
   set_cpus_allowed_ptr(current, cpumask_of(0));
   BUG_ON(smp_processor_id() != 0);

   for (i=0;i<4;i++)
      data->data[i] = swab32(data->data[i]);
   secretkey_service_call(data->mode, data->data);
   for (i=0;i<4;i++)
      data->data[i] = swab32(data->data[i]);

   set_cpus_allowed_ptr(current, old_mask);

   up(&secretkey_lock);

   free_cpumask_var(old_mask);
   return 0;
}

int decrypt_16_with_private_key(unsigned char* src, unsigned char* dst)
{
   struct secretkey_data data;
   if (*(uint32_t*)KONA_SRAM_VA != 0xBADDCAFE) {
      printk("secretkey driver can't operate in secure mode\n");
      return -1;
   }
   data.mode = 1;
   memcpy(data.data, src, 16);
   secretkey_op(&data);
   memcpy(dst, data.data, 16);
   return 0;
}

static int secretkey_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static int secretkey_release(struct inode *inode, struct file *filp)
{
	return 0;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static int secretkey_ioctl(struct inode *inode,
#else
static long secretkey_ioctl(
#endif 
                            struct file *filp, 
                            unsigned int cmd,
                            unsigned long arg)
{
	switch (cmd) {
		case SECRETKEY_IOCTL_OP:
		{
			int ret;
			struct secretkey_data data;

			ret = copy_from_user(&data, (struct secretkey_data *)arg, sizeof(data));
			if (ret)
				return -EFAULT;
			
			ret = secretkey_op(&data);
			if (ret < 0)
				return ret;

			ret = copy_to_user((void *)arg, &data, sizeof(data));
			if (ret)
				return -EFAULT;
		}
		break;

		default:
		return -EINVAL;
	}
	return 0;
}

struct file_operations secretkey_fops =
{
	owner: THIS_MODULE,
	open: secretkey_open,
	release: secretkey_release,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
	ioctl: secretkey_ioctl
#else
	unlocked_ioctl: secretkey_ioctl,
#endif
};

void test_op(void)
{
	struct secretkey_data data;
	data.mode = 2;
	data.data[0] = 0;
	data.data[1] = 0x11111111;
	data.data[2] = 0x22222222;
	data.data[3] = 0x33333333;
	secretkey_op(&data);
	printk("Test Result:\n%08x\n%08x\n%08x\n%08x\n",data.data[0],data.data[1],data.data[2],data.data[3]);
}

static int __init secretkey_init(void)
{
	int rval;
    
	printk(banner);
    
	rval = register_chrdev(SECRETKEY_MAJOR, SECRETKEY_DEV_NAME, &secretkey_fops);
	if (rval < 0) {
		printk("SECRETKEY: register_chrdev failed for major %d\n", SECRETKEY_MAJOR);
		return rval;
	}

	/* Create the device */
	secretkeyCfg.dev_class = class_create(THIS_MODULE, SECRETKEY_DEV_NAME);
	if (IS_ERR(secretkeyCfg.dev_class)) 
	{
		rval = PTR_ERR(secretkeyCfg.dev_class);
		printk("class create failed: %d\n", rval);
		goto init_err_class_create;
	}

	secretkeyCfg.dev = device_create(secretkeyCfg.dev_class, NULL, MKDEV(SECRETKEY_MAJOR, 0), 
	                                 NULL, SECRETKEY_DEV_NAME);
	if (IS_ERR(secretkeyCfg.dev)) 
	{
		rval = PTR_ERR(secretkeyCfg.dev);
		printk("device create failed: %d\n", rval);
		goto init_err_device_create;
	} 

	if (*(uint32_t*)KONA_SRAM_VA != 0xBADDCAFE) {
        	printk("secretkey driver can't operate in secure mode\n");
		rval = -EINVAL;
        	goto init_err;
	}
    
        printk("secretkey initialized successfully\n");

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,37)
	init_MUTEX(&secretkey_lock); /* unlocked */
#else
	sema_init(&secretkey_lock, 1);
#endif
	test_op();
    return rval; 
    
init_err:
    device_destroy(secretkeyCfg.dev_class, MKDEV(SECRETKEY_MAJOR, 0));

init_err_device_create:
    class_destroy(secretkeyCfg.dev_class);

init_err_class_create:
    unregister_chrdev(SECRETKEY_MAJOR, SECRETKEY_DEV_NAME);

    return rval;   
}

static void __exit secretkey_exit(void)
{
   /* Destroy device */
   device_destroy(secretkeyCfg.dev_class, MKDEV(SECRETKEY_MAJOR, 0));
   class_destroy(secretkeyCfg.dev_class);
   unregister_chrdev(SECRETKEY_MAJOR, SECRETKEY_DEV_NAME);
}

module_init(secretkey_init);
module_exit(secretkey_exit);

MODULE_DESCRIPTION("SECRETKEY Driver");
MODULE_LICENSE("GPL");
