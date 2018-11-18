/* 2012-04-12: File added and changed by Sony Corporation */
/*
 * linux/drivers/video/backlight/pwm_bl.c
 *
 * simple PWM based backlight control, board code has to setup
 * 1) pin configuration so PWM waveforms can output
 * 2) platform_data being correctly configured
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/fb.h>
#include <linux/backlight.h>
#include <linux/err.h>
#include <linux/pwm.h>
#include <linux/pwm_backlight.h>
#include <linux/slab.h>

struct pwm_bl_data {
	struct pwm_device	*pwm;
	struct device		*dev;
	unsigned int		period;
	unsigned int		lth_brightness;
	int			(*notify)(struct device *,
					  int brightness);
	int			(*check_fb)(struct device *, struct fb_info *);
};

#ifdef CONFIG_MACH_NBX03
/*
** conversion table
** from brightness(0-255) to backlight pwm(0-100)
** 10^(( 0.006768 * b ) + 0.274071 ) * 100
** except for b=10->3%, b=20->3%
*/
static const int bl_tbl[256] = {
/*
   188,  191,  194,  197,  200,  203,  206,  210,  213,  216, 
   220,  223,  227,  230,  234,  237,  241,  245,  249,  253, 
   257,  261,  265,  269,  273,  278,  282,  286,  291,  295, 
*/
   188,  191,  194,  197,  200,  203,  206,  210,  213,  216, 
   300,  223,  227,  230,  234,  237,  241,  245,  249,  253, 
   300,  261,  265,  269,  273,  278,  282,  286,  291,  295, 
   300,  305,  309,  314,  319,  324,  329,  335,  340,  345, 
   351,  356,  362,  367,  373,  379,  385,  391,  397,  403, 
   410,  416,  423,  429,  436,  443,  450,  457,  464,  471, 
   479,  486,  494,  502,  510,  518,  526,  534,  542,  551, 
   560,  568,  577,  586,  596,  605,  614,  624,  634,  644, 
   654,  664,  675,  685,  696,  707,  718,  729,  741,  752, 
   764,  776,  788,  801,  813,  826,  839,  852,  866,  879, 
   893,  907,  921,  936,  950,  965,  981,  996, 1012, 1028, 
  1044, 1060, 1077, 1094, 1111, 1128, 1146, 1164, 1182, 1201, 
  1220, 1239, 1258, 1278, 1298, 1318, 1339, 1360, 1382, 1403, 
  1425, 1448, 1470, 1494, 1517, 1541, 1565, 1590, 1615, 1640, 
  1666, 1692, 1718, 1745, 1773, 1801, 1829, 1858, 1887, 1916, 
  1947, 1977, 2008, 2040, 2072, 2104, 2137, 2171, 2205, 2240, 
  2275, 2311, 2347, 2384, 2421, 2459, 2498, 2537, 2577, 2617, 
  2658, 2700, 2743, 2786, 2829, 2874, 2919, 2965, 3011, 3059, 
  3107, 3156, 3205, 3255, 3307, 3359, 3411, 3465, 3519, 3575, 
  3631, 3688, 3746, 3804, 3864, 3925, 3987, 4049, 4113, 4177, 
  4243, 4310, 4377, 4446, 4516, 4587, 4659, 4732, 4806, 4882, 
  4959, 5036, 5115, 5196, 5277, 5360, 5445, 5530, 5617, 5705, 
  5795, 5886, 5978, 6072, 6167, 6264, 6363, 6463, 6564, 6667, 
  6772, 6878, 6986, 7096, 7207, 7321, 7436, 7552, 7671, 7792, 
  7914, 8038, 8164, 8293, 8423, 8555, 8690, 8826, 8965, 9105, 
  9248, 9394, 9541, 9691, 9843, 9998, 
};
#endif
static int pwm_backlight_update_status(struct backlight_device *bl)
{
	struct pwm_bl_data *pb = dev_get_drvdata(&bl->dev);
	int brightness = bl->props.brightness;
#ifndef CONFIG_MACH_NBX03
	int max = bl->props.max_brightness;
#endif

	if (bl->props.power != FB_BLANK_UNBLANK)
		brightness = 0;

	if (bl->props.fb_blank != FB_BLANK_UNBLANK)
		brightness = 0;

	if (pb->notify)
		brightness = pb->notify(pb->dev, brightness);

	if (brightness == 0) {
		pwm_config(pb->pwm, 0, pb->period);
		pwm_disable(pb->pwm);
	} else {
#ifdef CONFIG_MACH_NBX03
		brightness = pb->period / 10000 * bl_tbl[brightness];
#else
		brightness = pb->lth_brightness +
			(brightness * (pb->period - pb->lth_brightness) / max);
#endif
		pwm_config(pb->pwm, brightness, pb->period);
		pwm_enable(pb->pwm);
	}
	return 0;
}

static int pwm_backlight_get_brightness(struct backlight_device *bl)
{
	return bl->props.brightness;
}

static int pwm_backlight_check_fb(struct backlight_device *bl,
				  struct fb_info *info)
{
	struct pwm_bl_data *pb = dev_get_drvdata(&bl->dev);

	return !pb->check_fb || pb->check_fb(pb->dev, info);
}

static const struct backlight_ops pwm_backlight_ops = {
	.update_status	= pwm_backlight_update_status,
	.get_brightness	= pwm_backlight_get_brightness,
	.check_fb	= pwm_backlight_check_fb,
};

static int pwm_backlight_probe(struct platform_device *pdev)
{
	struct backlight_properties props;
	struct platform_pwm_backlight_data *data = pdev->dev.platform_data;
	struct backlight_device *bl;
	struct pwm_bl_data *pb;
	int ret;

	if (!data) {
		dev_err(&pdev->dev, "failed to find platform data\n");
		return -EINVAL;
	}

	if (data->init) {
		ret = data->init(&pdev->dev);
		if (ret < 0)
			return ret;
	}

	pb = kzalloc(sizeof(*pb), GFP_KERNEL);
	if (!pb) {
		dev_err(&pdev->dev, "no memory for state\n");
		ret = -ENOMEM;
		goto err_alloc;
	}

	pb->period = data->pwm_period_ns;
	pb->notify = data->notify;
	pb->check_fb = data->check_fb;
	pb->lth_brightness = data->lth_brightness *
		(data->pwm_period_ns / data->max_brightness);
	pb->dev = &pdev->dev;

	pb->pwm = pwm_request(data->pwm_id, "backlight");
	if (IS_ERR(pb->pwm)) {
		dev_err(&pdev->dev, "unable to request PWM for backlight\n");
		ret = PTR_ERR(pb->pwm);
		goto err_pwm;
	} else
		dev_dbg(&pdev->dev, "got pwm for backlight\n");

	memset(&props, 0, sizeof(struct backlight_properties));
	props.type = BACKLIGHT_RAW;
	props.max_brightness = data->max_brightness;
	bl = backlight_device_register(dev_name(&pdev->dev), &pdev->dev, pb,
				       &pwm_backlight_ops, &props);
	if (IS_ERR(bl)) {
		dev_err(&pdev->dev, "failed to register backlight\n");
		ret = PTR_ERR(bl);
		goto err_bl;
	}

	bl->props.brightness = data->dft_brightness;
	backlight_update_status(bl);

	platform_set_drvdata(pdev, bl);
	return 0;

err_bl:
	pwm_free(pb->pwm);
err_pwm:
	kfree(pb);
err_alloc:
	if (data->exit)
		data->exit(&pdev->dev);
	return ret;
}

static int pwm_backlight_remove(struct platform_device *pdev)
{
	struct platform_pwm_backlight_data *data = pdev->dev.platform_data;
	struct backlight_device *bl = platform_get_drvdata(pdev);
	struct pwm_bl_data *pb = dev_get_drvdata(&bl->dev);

	backlight_device_unregister(bl);
	pwm_config(pb->pwm, 0, pb->period);
	pwm_disable(pb->pwm);
	pwm_free(pb->pwm);
	kfree(pb);
	if (data->exit)
		data->exit(&pdev->dev);
	return 0;
}

#ifdef CONFIG_PM
static int pwm_backlight_suspend(struct platform_device *pdev,
				 pm_message_t state)
{
	struct backlight_device *bl = platform_get_drvdata(pdev);
	struct pwm_bl_data *pb = dev_get_drvdata(&bl->dev);

	if (pb->notify)
		pb->notify(pb->dev, 0);
	pwm_config(pb->pwm, 0, pb->period);
	pwm_disable(pb->pwm);
	return 0;
}

static int pwm_backlight_resume(struct platform_device *pdev)
{
	struct backlight_device *bl = platform_get_drvdata(pdev);

	backlight_update_status(bl);
	return 0;
}
#else
#define pwm_backlight_suspend	NULL
#define pwm_backlight_resume	NULL
#endif

static struct platform_driver pwm_backlight_driver = {
	.driver		= {
		.name	= "pwm-backlight",
		.owner	= THIS_MODULE,
	},
	.probe		= pwm_backlight_probe,
	.remove		= pwm_backlight_remove,
	.suspend	= pwm_backlight_suspend,
	.resume		= pwm_backlight_resume,
};

static int __init pwm_backlight_init(void)
{
	return platform_driver_register(&pwm_backlight_driver);
}
module_init(pwm_backlight_init);

static void __exit pwm_backlight_exit(void)
{
	platform_driver_unregister(&pwm_backlight_driver);
}
module_exit(pwm_backlight_exit);

MODULE_DESCRIPTION("PWM based Backlight Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:pwm-backlight");

