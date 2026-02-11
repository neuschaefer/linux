/*****************************************************************************
*  Copyright 2001 - 2012 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
*  http://www.gnu.org/licenses/old-license/gpl-2.0.html (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a
* license other than the GPL, without Broadcom's express prior written
* consent.
*
*****************************************************************************/

/*
 * Frameworks:
 *
 *    - SMP:          Locks with a semaphore around all critical sections.
 *    - GPIO:         No GPIOs used.
 *    - MMU:          ioremap and platform data used correctly.
 *    - Dynamic /dev: Fully supported.
 *    - Suspend:      Not implemented.
 *    - Clocks:       Not implemented.
 *    - Power:        Not implemented.
 *
 */

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
#include <linux/broadcom/otp.h>                               
#include <chal/chal_otp.h>
#include <mach/rdb/brcm_rdb_sysmap.h>

#undef OTP_DBG
#define OTP_DBG 1

#if (OTP_DBG == 1)
#define OTP_DEBUG(fmt, args...) printk(KERN_NOTICE "OTP: " fmt, ## args)
#else
#define OTP_DEBUG(fmt, args...) 
#endif

struct bcm_otp_cfg {
    struct class*  dev_class;
    struct device* dev;
	void __iomem *base;
};
struct bcm_otp_cfg otpCfg;

struct semaphore otp_lock;

/* Handle to the OTP driver */
CHAL_OTP_HANDLE_t otp_handle;

static int otp_write_row(struct otp_data *data)
{
   int rc = 0;

   if (data->row > otpHw_MAX_ROW)
      return -EINVAL;

   down(&otp_lock);

   if (data->row >= 16 && data->row <= 19) {
      int bit;
      for (bit=0;bit<32;bit++)
         if (data->data & (1<<bit))
            if (chal_otp_lockData(&otp_handle, data->row, bit ))
               printk("LockData Failed (0x%08x) - row: %03d, bit: %02d\n", chal_otp_getStatusCode(&otp_handle), data->row, bit);
   } else {
   rc = chal_otp_writeData(&otp_handle, data->row, data->data);
	if ((rc == 0) && (data->enable_ecc)) {
      rc = chal_otp_calculateEcc(&otp_handle, data->row);   
   }
   }

   up(&otp_lock);
   
	if (rc != 0) {
      rc = -EIO;
   }
	
   return rc;
}

static int otp_fail_row(struct otp_data *data)
{
   int rc = 0;

   if (data->row > otpHw_MAX_ROW)
      return -EINVAL;

   down(&otp_lock);

   rc = chal_otp_writeRawData(&otp_handle, data->row, 0, OTP_WDATA_1_OTP_WDATA_FAIL_MASK);

   up(&otp_lock);
   
   if (rc != 0)
   {
      rc = -EIO;
   }
	
   return rc;
}

static int otp_read_row(struct otp_data *data)
{
   if (data->row > otpHw_MAX_ROW)
      return -EINVAL;

	down(&otp_lock);

   if (data->enable_ecc)
      data->data = chal_otp_readData(&otp_handle, data->row);
   else
      data->data = chal_otp_readRawData(&otp_handle, data->row);

   up(&otp_lock);
   
   return 0;
}

static int otp_read_fail(struct otp_data *data)
{
   if (data->row > otpHw_MAX_ROW)
      return -EINVAL;

	down(&otp_lock);

   data->data = chal_otp_readRawDataUpper(&otp_handle, data->row) & OTP_WDATA_1_OTP_WDATA_FAIL_MASK;

   up(&otp_lock);
   
   return 0;
}

static int otp_open(struct inode *inode, struct file *filp)
{
	//OTP_DEBUG("OTP device opened\n");
	return 0;
}

static int otp_release(struct inode *inode, struct file *filp)
{
	//OTP_DEBUG("OTP device closed\n");
	return 0;
}

    static long otp_ioctl(
                     struct file *filp, 
			     unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
		case OTP_IOCTL_READ_ROW:
		{
			int ret;
			struct otp_data data;

			ret =
			    copy_from_user(&data, (struct otp_data *)arg,
					   sizeof(data));
			if (ret)
				return -EFAULT;
			
			ret = otp_read_row(&data);
			if (ret < 0)
				return ret;

			ret = copy_to_user((void *)arg, &data, sizeof(data));
			if (ret)
				return -EFAULT;
		}
		break;
		
		case OTP_IOCTL_READ_FAIL:
		{
			int ret;
			struct otp_data data;

			ret = copy_from_user(&data, (struct otp_data *)arg, sizeof(data));
			if (ret)
				return -EFAULT;
			
			ret = otp_read_fail(&data);
			if (ret < 0)
				return ret;

			ret = copy_to_user((void *)arg, &data, sizeof(data));
			if (ret)
				return -EFAULT;
		}
		break;
		
      case OTP_IOCTL_WRITE_ROW:
      {
			int ret;
			struct otp_data data;

			ret =
			    copy_from_user(&data, (struct otp_data *)arg,
					   sizeof(data));
			if (ret)
				return -EFAULT;
			
			ret = otp_write_row(&data);
			if (ret < 0)
				return ret;

			ret = copy_to_user((void *)arg, &data, sizeof(data));
			if (ret)
				return -EFAULT;
      }
      break;

		case OTP_IOCTL_FAIL_ROW:
		{
			int ret;
			struct otp_data data;

			ret = copy_from_user(&data, (struct otp_data *)arg, sizeof(data));
			if (ret)
				return -EFAULT;
			
			ret = otp_fail_row(&data);
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

struct file_operations otp_fops = {
	owner: THIS_MODULE,
	open: otp_open,
	release: otp_release,
   unlocked_ioctl: otp_ioctl,
};

static int __init kona_otp_probe(struct platform_device *dev)
{
	struct resource *res;
    int rval;
    uint32_t status;
    
	int ret;

	/* We only accept one device, and it must have an id of -1 */
	if (dev->id != -1)
		return -ENODEV;

	res = platform_get_resource(dev, IORESOURCE_MEM, 0);
	if (!res) {
		ret = -ENODEV;
		goto err_out;
	}

	otpCfg.base = ioremap(res->start, resource_size(res));
	if (!otpCfg.base) {
		ret = -ENOMEM;
		goto err_clock;
	}
    
	rval = register_chrdev(BCM_OTP_MAJOR, BCM_OTP_DEV_NAME, &otp_fops);
	if (rval < 0) {
		printk("OTP: register_chrdev failed for major %d\n",
		       BCM_OTP_MAJOR);
		return rval;
	} else {
		OTP_DEBUG("OTP: class create succeeded, major: %d\n",
			  BCM_OTP_MAJOR);
    }

    /* Create the device */
    otpCfg.dev_class = class_create(THIS_MODULE, BCM_OTP_DEV_NAME);
	if (IS_ERR(otpCfg.dev_class)) {
        rval = PTR_ERR(otpCfg.dev_class);
        printk("class create failed: %d\n", rval);
        goto init_err_class_create;
	} else {
        OTP_DEBUG("OTP: class create succeeded\n");
    }

	otpCfg.dev =
	    device_create(otpCfg.dev_class, NULL, MKDEV(BCM_OTP_MAJOR, 0), NULL,
			  BCM_OTP_DEV_NAME);
	if (IS_ERR(otpCfg.dev)) {
        rval = PTR_ERR(otpCfg.dev);
        printk("device create failed: %d\n", rval);
        goto init_err_device_create;
	} else {
        OTP_DEBUG("otp char device created\n"); 
    }

	rval = chal_otp_init(&otp_handle, (uint32_t) otpCfg.base);
	if (rval != 0) {
        printk("chal_otp init failed\n");
        goto init_err;
    }
    
    chal_otp_hw_init(&otp_handle);
    status = chal_otp_getStatusCode(&otp_handle);
	if (status != 0) {
        printk("chal_otp_hw_init failed\n");
        rval = -status;
        goto init_err;
	} else {
        OTP_DEBUG("OTP h/w initialized successfully\n");
    }

        sema_init(&otp_lock, 1);

    return rval; 
    
init_err:
    device_destroy(otpCfg.dev_class, MKDEV(BCM_OTP_MAJOR, 0));

init_err_device_create:
    class_destroy(otpCfg.dev_class);

init_err_class_create:
    unregister_chrdev(BCM_OTP_MAJOR, BCM_OTP_DEV_NAME);
 err_clock:
	iounmap(otpCfg.base);

 err_out:
	return -ENODEV;
}

static int __devexit kona_otp_remove(struct platform_device *dev)
{
   /* Destroy device */
   device_destroy(otpCfg.dev_class, MKDEV(BCM_OTP_MAJOR, 0));
   class_destroy(otpCfg.dev_class);
   unregister_chrdev(BCM_OTP_MAJOR, BCM_OTP_DEV_NAME);
	iounmap(otpCfg.base);
	return 0;
}

#define kona_otp_suspend    NULL
#define kona_otp_resume     NULL

/* work with hotplug and coldplug */
MODULE_ALIAS("platform:kona_otp");

static struct platform_driver kona_otp_driver = {
	.remove = __devexit_p(kona_otp_remove),
	.suspend = kona_otp_suspend,
	.resume = kona_otp_resume,
	.driver = {
		   .owner = THIS_MODULE,
		   .name = "kona_otp",
		   },
};

static char banner[] __initdata = KERN_INFO "Broadcom OTP Driver\n";

static int __init kona_otp_init(void)
{
	printk(banner);

	return platform_driver_probe(&kona_otp_driver, kona_otp_probe);
}

static void __exit kona_otp_exit(void)
{
	platform_driver_unregister(&kona_otp_driver);
}

module_init(kona_otp_init);
module_exit(kona_otp_exit);

MODULE_AUTHOR("Broadcom");
MODULE_DESCRIPTION("OTP Driver");
MODULE_LICENSE("GPL");
