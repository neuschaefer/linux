#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/leds.h>
#include <linux/gpio.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include "leds-arc_common.h"
#include "leds-arc_custo.h"

void led_i2c_set(struct i2c_client *client, int set_address ,int set_value)
{
    char set_buf[2]; //first byte is address to write. others are bytes to be written
	set_buf[0]=set_address;
	set_buf[1]=set_value;
	i2c_master_send(client, set_buf, 2); //reposition file pointer to register 0x29
}

void led_i2c_get(struct i2c_client *client, int get_address ,char* result)
{
	char get_buf[1]; //first byte is address to write. others are bytes to be written
	get_buf[0]=get_address;
    i2c_master_send(client, get_buf, 1); //reposition file pointer to register 0x29
	i2c_master_recv(client, result, 1); //read register 0x29 and save to rbuf
}



static void reg_i2c_device(void)
{
	adap = i2c_get_adapter(I2C_BUS_ID);
	reg_client = i2c_new_device(adap, &arc_led_i2c_info);
}

static void unreg_i2c_device(void)
{
	i2c_put_adapter(adap);
	i2c_unregister_device(reg_client);
}

/************RED************/
static void arc_red_led0_color_set(struct led_classdev *led_cdev,
			    enum led_brightness value)
{
	led_i2c_set(reg_client, LED0_COLOR_R, value);
}
static void arc_red_led1_color_set(struct led_classdev *led_cdev,
			    enum led_brightness value)
{
	led_i2c_set(reg_client, LED1_COLOR_R, value);
}
static void arc_red_led2_color_set(struct led_classdev *led_cdev,
			    enum led_brightness value)
{
	led_i2c_set(reg_client, LED2_COLOR_R, value);
}
static void arc_red_led3_color_set(struct led_classdev *led_cdev,
			    enum led_brightness value)
{
	led_i2c_set(reg_client, LED3_COLOR_R, value);
}

static enum led_brightness arc_red_led0_color_get(struct led_classdev * led_dev)
{
	char r_buf[10];

	led_i2c_get(reg_client, LED0_COLOR_R, r_buf);

	return (int)r_buf[0];
}
static enum led_brightness arc_red_led1_color_get(struct led_classdev * led_dev)
{
	char r_buf[10];

	led_i2c_get(reg_client, LED1_COLOR_R, r_buf);

	return (int)r_buf[0];
}
static enum led_brightness arc_red_led2_color_get(struct led_classdev * led_dev)
{
	char r_buf[10];

	led_i2c_get(reg_client, LED2_COLOR_R, r_buf);

	return (int)r_buf[0];
}
static enum led_brightness arc_red_led3_color_get(struct led_classdev * led_dev)
{
	char r_buf[10];

	led_i2c_get(reg_client, LED3_COLOR_R, r_buf);

	return (int)r_buf[0];
}
/************RED************/

/************GREEN************/
static void arc_green_led0_color_set(struct led_classdev *led_cdev,
			    enum led_brightness value)
{
	led_i2c_set(reg_client, LED0_COLOR_G, value);
}
static void arc_green_led1_color_set(struct led_classdev *led_cdev,
			    enum led_brightness value)
{
	led_i2c_set(reg_client, LED1_COLOR_G, value);
}
static void arc_green_led2_color_set(struct led_classdev *led_cdev,
			    enum led_brightness value)
{
	led_i2c_set(reg_client, LED2_COLOR_G, value);
}
static void arc_green_led3_color_set(struct led_classdev *led_cdev,
			    enum led_brightness value)
{
	led_i2c_set(reg_client, LED3_COLOR_G, value);
}

static enum led_brightness arc_green_led0_color_get(struct led_classdev * led_dev)
{
	char r_buf[10];

	led_i2c_get(reg_client, LED0_COLOR_G, r_buf);

	return (int)r_buf[0];
}
static enum led_brightness arc_green_led1_color_get(struct led_classdev * led_dev)
{
	char r_buf[10];

	led_i2c_get(reg_client, LED1_COLOR_G, r_buf);

	return (int)r_buf[0];
}
static enum led_brightness arc_green_led2_color_get(struct led_classdev * led_dev)
{
	char r_buf[10];

	led_i2c_get(reg_client, LED2_COLOR_G, r_buf);

	return (int)r_buf[0];
}
static enum led_brightness arc_green_led3_color_get(struct led_classdev * led_dev)
{
	char r_buf[10];

	led_i2c_get(reg_client, LED3_COLOR_G, r_buf);

	return (int)r_buf[0];
}
/************GREEN************/

/************BLUE************/
static void arc_blue_led0_color_set(struct led_classdev *led_cdev,
			    enum led_brightness value)
{
	led_i2c_set(reg_client, LED0_COLOR_B, value);
}
static void arc_blue_led1_color_set(struct led_classdev *led_cdev,
			    enum led_brightness value)
{
	led_i2c_set(reg_client, LED1_COLOR_B, value);
}
static void arc_blue_led2_color_set(struct led_classdev *led_cdev,
			    enum led_brightness value)
{
	led_i2c_set(reg_client, LED2_COLOR_B, value);
}
static void arc_blue_led3_color_set(struct led_classdev *led_cdev,
			    enum led_brightness value)
{
	led_i2c_set(reg_client, LED3_COLOR_B, value);
}

static enum led_brightness arc_blue_led0_color_get(struct led_classdev * led_dev)
{
	char r_buf[10];

	led_i2c_get(reg_client, LED0_COLOR_B, r_buf);

	return (int)r_buf[0];
}
static enum led_brightness arc_blue_led1_color_get(struct led_classdev * led_dev)
{
	char r_buf[10];

	led_i2c_get(reg_client, LED1_COLOR_B, r_buf);

	return (int)r_buf[0];
}
static enum led_brightness arc_blue_led2_color_get(struct led_classdev * led_dev)
{
	char r_buf[10];

	led_i2c_get(reg_client, LED2_COLOR_B, r_buf);

	return (int)r_buf[0];
}
static enum led_brightness arc_blue_led3_color_get(struct led_classdev * led_dev)
{
	char r_buf[10];

	led_i2c_get(reg_client, LED3_COLOR_B, r_buf);

	return (int)r_buf[0];
}
/************BLUE************/

/************BRIGHT************/
static void arc_led0_bright_set(struct led_classdev *led_cdev,
			    enum led_brightness value)
{
	led_i2c_set(reg_client, LED0_BRIGHT, value);
}
static void arc_led1_bright_set(struct led_classdev *led_cdev,
			    enum led_brightness value)
{
	led_i2c_set(reg_client, LED1_BRIGHT, value);
}
static void arc_led2_bright_set(struct led_classdev *led_cdev,
			    enum led_brightness value)
{
	led_i2c_set(reg_client, LED2_BRIGHT, value);
}
static void arc_led3_bright_set(struct led_classdev *led_cdev,
			    enum led_brightness value)
{
	led_i2c_set(reg_client, LED3_BRIGHT, value);
}

static enum led_brightness arc_led0_bright_get(struct led_classdev * led_dev)
{
	char r_buf[10];

	led_i2c_get(reg_client, LED0_BRIGHT, r_buf);

	return (int)r_buf[0];
}
static enum led_brightness arc_led1_bright_get(struct led_classdev * led_dev)
{
	char r_buf[10];

	led_i2c_get(reg_client, LED1_BRIGHT, r_buf);

	return (int)r_buf[0];
}
static enum led_brightness arc_led2_bright_get(struct led_classdev * led_dev)
{
	char r_buf[10];

	led_i2c_get(reg_client, LED2_BRIGHT, r_buf);

	return (int)r_buf[0];
}
static enum led_brightness arc_led3_bright_get(struct led_classdev * led_dev)
{
	char r_buf[10];

	led_i2c_get(reg_client, LED3_BRIGHT, r_buf);

	return (int)r_buf[0];
}
/************BRIGHT************/

static ssize_t startup_anim_stop_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	unsigned long state;
	int ret;

	ret = kstrtoul(buf, 0, &state);
	if (state) {
		if (startup_thread) {
    		kthread_stop(startup_thread);
			startup_thread = NULL;
    	}
	}

	return size;
}
static DEVICE_ATTR_WO(startup_anim_stop);
static int arc_led_probe(struct platform_device *pdev)
{
	int ret;
	int i;
	int gpio;
		
	printk(KERN_INFO"%s(#%d)\n", __func__, __LINE__);
	
        reg_i2c_device();
	 
	for(i=0; i<LED_NUM*LED_COLOR; i++)
        {
		/* register our new led device */
		ret = led_classdev_register(&pdev->dev, &led_reg_cdev[i]); 
		if (ret < 0) 
		{
			dev_err(&pdev->dev, "led_classdev_register failed\n");
			return ret;
		}
	}

	// Enable leds by default
	ret = gpio_request_one(LED_GPIO, GPIOF_OUT_INIT_HIGH, "led_gpio");
	gpio_free(LED_GPIO);

	// Need a first write to i2c to initiate transfer
	led_i2c_set(reg_client, 0x00, 0x00);
	// Enable leds
	led_i2c_set(reg_client, 0x00, 0x40);
	led_i2c_set(reg_client, 0x01, 0x14);

	startup_thread = kthread_run(startup_anim, NULL, "startup_animation");

	for(i=0; i<LED_NUM*LED_COLOR; i++)
	{
		/* register device to stop startup animation */
		ret = device_create_file(led_reg_cdev[i].dev, &dev_attr_startup_anim_stop);
	}

    return 0;
}

static int arc_led_remove(struct platform_device *pdev)
{
	int i;
    printk(KERN_INFO"%s(#%d)\n", __func__, __LINE__);
    for(i=0; i<LED_NUM*LED_COLOR; i++)
    {
        device_remove_file(led_reg_cdev[i].dev, &dev_attr_startup_anim_stop);
        /* unregister our new led device */
        led_classdev_unregister(&led_reg_cdev[i]); 
    }
    unreg_i2c_device();
    return 0;
}

static struct platform_driver arc_led_driver = {
    .driver = {
        .name  = DEVNAME,
    },
    .probe  = arc_led_probe,
    .remove = arc_led_remove,
};

static int __init arc_led_init(void)
{
    int err;
    printk(KERN_INFO"%s(#%d)\n", __func__, __LINE__);

    err = platform_device_register(&arc_led_device);
    if (err) {
        pr_err("%s(#%d): platform_device_register failed(%d)\n",
                __func__, __LINE__, err);
        return err;
    }

    err = platform_driver_register(&arc_led_driver);
    if (err) {
        dev_err(&(arc_led_device.dev), "%s(#%d): platform_driver_register fail(%d)\n",
                __func__, __LINE__, err);
        goto dev_reg_failed;
    }

    return err;

dev_reg_failed:
    platform_device_unregister(&arc_led_device);

    return err;
}
module_init(arc_led_init);

static void __exit arc_led_exit(void)
{
    dev_info(&(arc_led_device.dev), "%s(#%d)\n", __func__, __LINE__);
    platform_device_unregister(&arc_led_device);
    platform_driver_unregister(&arc_led_driver);

    if (startup_thread) {
    	kthread_stop(startup_thread);
		startup_thread = NULL;
    }
}
module_exit(arc_led_exit);


MODULE_AUTHOR("Aaron Chung <aaron_chung@arcadyan.com>");
MODULE_DESCRIPTION("Kernel module for led");
MODULE_LICENSE("GPLV2");
