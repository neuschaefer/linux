/*
 * Copyright (C) 2011-2012 Sony Corporation
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
 * published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
/*
 * linux/drivers/video/backlight/nbx02_bl.c
 *
 * NBX02 backlight control, board code has to setup
 * 1) pin configuration so PWM waveforms can output
 * 2) platform_data being correctly configured
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/fb.h>
#include <linux/backlight.h>
#include <linux/err.h>
#include <linux/pwm.h>
#include <linux/pwm_backlight.h>
#include <linux/nbx02_backlight.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

struct nbx02_bl_data {
	struct {
		struct pwm_device	*pwm;
		int flags;
		int off_keeper;
		int duty;

#define BL_FLAGS_KEEP_TIMER	(1 << 0)
#define BL_FLAGS_PWM_ENABLE	(1 << 1)

		struct timer_list timer;
	} panel[2];
	int expire_time;
	struct device		*dev;
	unsigned int		period;
	int			(*notify)(struct device *,
					  int brightness);
	struct miscdevice miscdev;
};

static struct backlight_device *bl_dev;

/*
** conversion table
** from brightness(0-255) to backlight pwm duty ratio(0-1000)
*/
static const int bl_tbl[256] = {
   28,   28,   28,   29,   29,   30,   30,   31,   31,   31,
   32,   32,   33,   33,   34,   34,   35,   35,   36,   36,
   37,   37,   38,   38,   39,   39,   40,   41,   41,   42,
   42,   43,   44,   44,   45,   45,   46,   47,   47,   48,
   49,   49,   50,   51,   52,   52,   53,   54,   55,   55,
   56,   57,   58,   59,   59,   60,   61,   62,   63,   64,
   65,   66,   67,   67,   68,   69,   70,   71,   72,   73,
   74,   76,   77,   78,   79,   80,   81,   82,   83,   85,
   86,   87,   88,   89,   91,   92,   93,   95,   96,   97,
   99,  100,  102,  103,  104,  106,  107,  109,  110,  112,
  114,  115,  117,  119,  120,  122,  124,  125,  127,  129,
  131,  133,  135,  136,  138,  140,  142,  144,  146,  148,
  151,  153,  155,  157,  159,  161,  164,  166,  168,  171,
  173,  176,  178,  181,  183,  186,  188,  191,  194,  197,
  199,  202,  205,  208,  211,  214,  217,  220,  223,  226,
  229,  233,  236,  239,  243,  246,  250,  253,  257,  260,
  264,  268,  271,  275,  279,  283,  287,  291,  295,  299,
  304,  308,  312,  317,  321,  326,  330,  335,  340,  345,
  349,  354,  359,  364,  370,  375,  380,  385,  391,  396,
  402,  408,  413,  419,  425,  431,  437,  443,  450,  456,
  462,  469,  476,  482,  489,  496,  503,  510,  517,  525,
  532,  540,  547,  555,  563,  571,  579,  587,  595,  604,
  612,  621,  629,  638,  647,  656,  666,  675,  685,  694,
  704,  714,  724,  734,  745,  755,  766,  777,  788,  799,
  810,  821,  833,  845,  857,  869,  881,  894,  906,  919,
  932,  945,  958,  972,  986, 1000
};

static int nbx02_backlight_update_status(struct backlight_device *bl)
{
	struct nbx02_bl_data *pb = dev_get_drvdata(&bl->dev);
	int brightness = bl->props.brightness;
	int index;
	int duty;

	if (bl->props.power != FB_BLANK_UNBLANK)
		brightness = 0;

	if (bl->props.fb_blank != FB_BLANK_UNBLANK)
		brightness = 0;

	if (pb->notify)
		brightness = pb->notify(pb->dev, brightness);

	if (brightness == 0) {
		for (index = 0; index < 2; index++) {
			if (pb->panel[index].flags & BL_FLAGS_PWM_ENABLE) {
				pwm_config(pb->panel[index].pwm, 0, pb->period);
				pwm_disable(pb->panel[index].pwm);
				pb->panel[index].flags &= ~BL_FLAGS_PWM_ENABLE;
				pb->panel[index].duty = 0;
			}
		}
	} else {
		for (index = 0; index < 2; index++) {
			if (pb->panel[index].off_keeper) {
				duty = pb->period * bl_tbl[brightness] / 1000;
				if (!(pb->panel[index].flags & BL_FLAGS_PWM_ENABLE) ||
						(pb->panel[index].duty != duty)) {
					pwm_config(pb->panel[index].pwm, duty, pb->period);
					pb->panel[index].duty = duty;
				}
				if (!(pb->panel[index].flags & BL_FLAGS_PWM_ENABLE)) {
					if (!pwm_enable(pb->panel[index].pwm)) {
						pb->panel[index].flags |= BL_FLAGS_PWM_ENABLE;
					}
				}
			}
			else {
				if (pb->panel[index].flags & BL_FLAGS_PWM_ENABLE) {
					pwm_config(pb->panel[index].pwm, 0, pb->period);
					pwm_disable(pb->panel[index].pwm);
					pb->panel[index].flags &= ~BL_FLAGS_PWM_ENABLE;
					pb->panel[index].duty = 0;
				}
			}
		}
	}
	return 0;
}

static int nbx02_backlight_get_brightness(struct backlight_device *bl)
{
	return bl->props.brightness;
}

static const struct backlight_ops nbx02_backlight_ops = {
	.update_status	= nbx02_backlight_update_status,
	.get_brightness	= nbx02_backlight_get_brightness,
};

static int store_expire_time(unsigned long expire_time)
{
	struct nbx02_bl_data *pb = bl_dev ? dev_get_drvdata(&bl_dev->dev) : 0;
	int rc = 0;

	if ( !pb )
		return -EFAULT;

	mutex_lock(&bl_dev->ops_lock);
	if (bl_dev->ops) {
		if ((expire_time != 0) && (expire_time < 50 || expire_time > INT_MAX))
			rc = -EINVAL;
		else {
			pr_debug("nbx02_backlight: set expire time to %lu\n", expire_time);
			pb->expire_time = expire_time;
		}
	}
	else {
		rc = -ENXIO;
	}
	mutex_unlock(&bl_dev->ops_lock);

	return rc;
}

void off_keep_timer_stop(struct backlight_device *bl)
{
	struct nbx02_bl_data *pb = dev_get_drvdata(&bl->dev);
	int index;

	for (index = 0; index < 2; index++) {
		if (pb->panel[index].flags & BL_FLAGS_KEEP_TIMER) {
			pr_debug("nbx02_backlight: off keep timer stop(%d)", index);
			del_timer_sync(&pb->panel[index].timer);
			pb->panel[index].off_keeper = 1;
			backlight_update_status(bl);
			pb->panel[index].flags &= ~BL_FLAGS_KEEP_TIMER;
		}
	}
}

static void off_keep_timer_handler(unsigned long info_addr)
{
	struct nbx02_bl_data *pb = bl_dev ? dev_get_drvdata(&bl_dev->dev) : 0;
	int index = (int)info_addr;

	if ( !pb ) 
		return;

	if (pb->panel[index].flags & BL_FLAGS_KEEP_TIMER) {
		pr_debug("nbx02_backlight: off keep timeup(%d)", index);
		pb->panel[index].off_keeper = 1;
		backlight_update_status(bl_dev);
		pb->panel[index].flags &= ~BL_FLAGS_KEEP_TIMER;
	}
}

static ssize_t nbx02_backlight_store_off_keeper(unsigned long off_keeper,
		int index)
{
	struct nbx02_bl_data *pb = bl_dev ? dev_get_drvdata(&bl_dev->dev) : 0;
	int rc = 0;
	unsigned long timeout;

	if ( !pb )
		return -EFAULT;

	mutex_lock(&bl_dev->ops_lock);
	if (bl_dev->ops) {
		pr_debug("nbx02_backlight: set off keeper%d to %lu\n", index, off_keeper);
		if (pb->panel[index].off_keeper != off_keeper) {
			if (off_keeper) {
				if (pb->panel[index].flags & BL_FLAGS_KEEP_TIMER) {
					pr_debug("nbx02_backlight: off keep timer stop(%d)", index);
					del_timer_sync(&pb->panel[index].timer);
					pb->panel[index].flags &= ~BL_FLAGS_KEEP_TIMER;
				}
			}
			else if (pb->expire_time != 0) {
				if (!(pb->panel[index].flags & BL_FLAGS_KEEP_TIMER)) {

					init_timer(&pb->panel[index].timer);
					timeout = pb->expire_time / (1000 / HZ);
					pb->panel[index].timer.function = off_keep_timer_handler;
					pb->panel[index].timer.expires = jiffies + timeout;
					pb->panel[index].timer.data = (unsigned long)index;
					pr_debug("nbx02_backlight: off keep timer start(%d)=%lu",
									 index, timeout);
					add_timer(&pb->panel[index].timer);
					pb->panel[index].flags |= BL_FLAGS_KEEP_TIMER;
				}
			}
			pb->panel[index].off_keeper = off_keeper;
			backlight_update_status(bl_dev);
		}
		else if (!off_keeper) {
			if (pb->expire_time != 0) {
				if (pb->panel[index].flags & BL_FLAGS_KEEP_TIMER) {

					timeout = pb->expire_time / (1000 / HZ);
					pr_debug("nbx02_backlight: off keep timer restart(%d)=%lu",
									 index, timeout);
					mod_timer(&pb->panel[index].timer, jiffies + timeout);
				}
			}
			else {
				pr_debug("nbx02_backlight: off keep timer stop(%d)", index);
				del_timer_sync(&pb->panel[index].timer);
			}
			pb->panel[index].off_keeper = off_keeper;
			backlight_update_status(bl_dev);
		}
	}
	else {
		rc = -ENXIO;
	}
	mutex_unlock(&bl_dev->ops_lock);

	return rc;
}

static int nbx02_backlight_open(struct inode *inode, struct file *filp)
{
	struct nbx02_bl_data *pb = bl_dev ? dev_get_drvdata(&bl_dev->dev) : 0;
	filp->private_data = pb;
	if ( pb ) {
		pr_debug("open(%s)", dev_name(pb->dev));
	}
	return 0;
}

static int nbx02_backlight_release(struct inode *inode, struct file *filp)
{
	struct nbx02_bl_data *pb = bl_dev ? dev_get_drvdata(&bl_dev->dev) : 0;
	if ( pb ) {
		pr_debug("release(%s)", dev_name(pb->dev));
	}
	return 0;
}

static long nbx02_backlight_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	//struct nbx02_bl_data *pb = bl_dev ? dev_get_drvdata(&bl_dev->dev) : 0;
	struct nbx02_bl_data *pb = (struct nbx02_bl_data*)(filp->private_data);
	int err = 0;
	int32_t data1;
	nbx_ioc_light data2;
	void __user *uarg = (void __user *)arg;

	if ( !pb )
		return -EFAULT;

	if (_IOC_TYPE(cmd) != NBX_BACKLIGHT_IOC_MAGIC)
		return -ENOTTY;

	if (_IOC_NR(cmd) > NBX_BACKLIGHT_IOC_MAXNR)
		return -ENOTTY;

	if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, uarg, _IOC_SIZE(cmd));
	if (_IOC_DIR(cmd) & _IOC_WRITE)
		err = !access_ok(VERIFY_READ, uarg, _IOC_SIZE(cmd));

	if (err)
		return -EFAULT;

	switch (cmd) {
	case NBX_BACKLIGHT_IOC_SET_EXPIRE_TIME_MS:
		if (copy_from_user(&data1, uarg, sizeof(data1)))
			return -EFAULT;

		pr_debug("ioctl(%s,NBX_BACKLIGHT_IOC_SET_EXPIRE_TIME_MS,%d)",
						 dev_name(pb->dev), data1);
		err = store_expire_time((unsigned long)data1);
		break;

	case NBX_BACKLIGHT_IOC_GET_EXPIRE_TIME_MS:
		data1 = (int)pb->expire_time;
		if (copy_to_user(uarg, &data1, sizeof(data1)))
			return -EFAULT;

		pr_debug("ioctl(%s,NBX_BACKLIGHT_IOC_GET_EXPIRE_TIME_MS)=%d",
						 dev_name(pb->dev), data1);
		break;

	case NBX_BACKLIGHT_IOC_SET_LIGHT:
		if (copy_from_user(data2, uarg, sizeof(data2)))
			return -EFAULT;

		pr_debug("ioctl(%s,NBX_BACKLIGHT_IOC_SET_LIGHT,%d,%d)",
						 dev_name(pb->dev), data2[0], data2[1]);
		err = nbx02_backlight_store_off_keeper((unsigned long)data2[1], data2[0]);
		break;

	case NBX_BACKLIGHT_IOC_GET_LIGHT:
		if (copy_from_user(data2, uarg, sizeof(data2)))
			return -EFAULT;

		if ((data2[0] < 0) || (data2[0] > 1))
			return -EINVAL;

		data2[1] = pb->panel[data2[0]].off_keeper;
		if (copy_to_user(uarg, data2, sizeof(data2)))
			return -EFAULT;

		pr_debug("ioctl(%s,NBX_BACKLIGHT_IOC_GET_LIGHT,%d)=%d",
						 dev_name(pb->dev), data2[0], data2[1]);
		break;

	default:
		return -ENOTTY;
	}
	return err;
}

static const struct file_operations nbx02_backlight_fops = {
	.owner		= THIS_MODULE,
	.open		= nbx02_backlight_open,
	.release	= nbx02_backlight_release,
	.unlocked_ioctl	= nbx02_backlight_ioctl
};

static int nbx02_backlight_probe(struct platform_device *pdev)
{
	struct backlight_properties props;
	struct platform_nbx02_backlight_data *data = pdev->dev.platform_data;
	struct backlight_device *bl;
	struct nbx02_bl_data *pb;
	int index;
	int ret;

	if (!data) {
		dev_err(&pdev->dev, "failed to find platform data\n");
		return -EINVAL;
	}

	if (data->pwm[0].init) {
		ret = data->pwm[0].init(&pdev->dev);
		if (ret < 0)
			return ret;
	}

	pb = kzalloc(sizeof(*pb), GFP_KERNEL);
	if (!pb) {
		dev_err(&pdev->dev, "no memory for state\n");
		ret = -ENOMEM;
		goto err_alloc;
	}

	pb->period = data->pwm[0].pwm_period_ns;
	pb->notify = data->pwm[0].notify;
	pb->dev = &pdev->dev;

	pb->panel[0].pwm = pwm_request(data->pwm[0].pwm_id, "backlight");
	if (IS_ERR(pb->panel[0].pwm)) {
		dev_err(&pdev->dev, "unable to request nbx02 for upper backlight\n");
		ret = PTR_ERR(pb->panel[0].pwm);
		goto err_pwm0;
	} else
		dev_dbg(&pdev->dev, "got nbx02 for upper backlight\n");

	pb->panel[1].pwm = pwm_request(data->pwm[1].pwm_id, "backlight");
	if (IS_ERR(pb->panel[1].pwm)) {
		dev_err(&pdev->dev, "unable to request nbx02 for lower backlight\n");
		ret = PTR_ERR(pb->panel[1].pwm);
		goto err_pwm1;
	} else
		dev_dbg(&pdev->dev, "got nbx02 for lower backlight\n");

	memset(&props, 0, sizeof(props));
	props.type = BACKLIGHT_PLATFORM;
	props.max_brightness = data->pwm[0].max_brightness;
	bl = backlight_device_register(dev_name(&pdev->dev), &pdev->dev, pb,
				       &nbx02_backlight_ops, &props);
	if (IS_ERR(bl)) {
		dev_err(&pdev->dev, "failed to register backlight\n");
		ret = PTR_ERR(bl);
		goto err_bl;
	}

	pb->miscdev.minor = MISC_DYNAMIC_MINOR;
	pb->miscdev.name = "nbx_backlight";
	pb->miscdev.fops = &nbx02_backlight_fops;

	ret = misc_register(&pb->miscdev);
	if (ret) {
		dev_err(&pdev->dev, "unable to register miscdevice %s\n", pb->miscdev.name);
		goto err_misc;
	}

	bl->props.brightness = data->pwm[0].dft_brightness;

	for (index = 0; index < 2; index++) {
		pb->panel[index].off_keeper = 1;
		pb->panel[index].flags = 0;
		pb->panel[index].duty = 0;
	}
	pb->expire_time = 1000;

	nbx02_backlight_update_status(bl);

	platform_set_drvdata(pdev, bl);

	bl_dev = bl;

	return 0;

err_misc:
	backlight_device_unregister(bl);
err_bl:
	pwm_free(pb->panel[1].pwm);
err_pwm1:
	pwm_free(pb->panel[0].pwm);
err_pwm0:
	kfree(pb);
err_alloc:
	if (data->pwm[0].exit)
		data->pwm[0].exit(&pdev->dev);
	return ret;
}

static int nbx02_backlight_remove(struct platform_device *pdev)
{
	struct platform_nbx02_backlight_data *data = pdev->dev.platform_data;
	struct backlight_device *bl = platform_get_drvdata(pdev);
	struct nbx02_bl_data *pb = dev_get_drvdata(&bl->dev);
	int index;

	off_keep_timer_stop(bl);

	bl_dev = NULL;

	misc_deregister(&pb->miscdev);

	backlight_device_unregister(bl);
	for (index = 0; index < 2; index++) {
		if (pb->panel[index].flags & BL_FLAGS_PWM_ENABLE) {
			pwm_config(pb->panel[index].pwm, 0, pb->period);
			pwm_disable(pb->panel[index].pwm);
			pb->panel[index].flags &= ~BL_FLAGS_PWM_ENABLE;
			pb->panel[index].duty = 0;
		}
		pwm_free(pb->panel[index].pwm);
	}
	kfree(pb);
	if (data->pwm[0].exit)
		data->pwm[0].exit(&pdev->dev);
	return 0;
}

#ifdef CONFIG_PM
static int nbx02_backlight_suspend(struct platform_device *pdev,
				 pm_message_t state)
{
	struct backlight_device *bl = platform_get_drvdata(pdev);
	struct nbx02_bl_data *pb = dev_get_drvdata(&bl->dev);
	int index;

	if (pb->notify)
		pb->notify(pb->dev, 0);

	off_keep_timer_stop(bl);

	for (index = 0; index < 2; index++) {
		if (pb->panel[index].flags & BL_FLAGS_PWM_ENABLE) {
			pwm_config(pb->panel[index].pwm, 0, pb->period);
			pwm_disable(pb->panel[index].pwm);
			pb->panel[index].flags &= ~BL_FLAGS_PWM_ENABLE;
			pb->panel[index].duty = 0;
		}
	}
	return 0;
}

static int nbx02_backlight_resume(struct platform_device *pdev)
{
	struct backlight_device *bl = platform_get_drvdata(pdev);

	nbx02_backlight_update_status(bl);
	return 0;
}
#else
#define nbx02_backlight_suspend	NULL
#define nbx02_backlight_resume	NULL
#endif

static struct platform_driver nbx02_backlight_driver = {
	.driver		= {
		.name	= "nbx02_backlight",
		.owner	= THIS_MODULE,
	},
	.probe		= nbx02_backlight_probe,
	.remove		= nbx02_backlight_remove,
	.suspend	= nbx02_backlight_suspend,
	.resume		= nbx02_backlight_resume,
};

static int __init nbx02_backlight_init(void)
{
	return platform_driver_register(&nbx02_backlight_driver);
}
module_init(nbx02_backlight_init);

static void __exit nbx02_backlight_exit(void)
{
	platform_driver_unregister(&nbx02_backlight_driver);
}
module_exit(nbx02_backlight_exit);

MODULE_DESCRIPTION("nbx02 Backlight Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:nbx02_backlight");

