#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/i2c-gpio.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <linux/sii9234.h>
/*#include "midas.h"*/

#ifdef CONFIG_SAMSUNG_MHL

#define GPIO_MHL_SDA_1_8V 66
#define GPIO_MHL_SCL_1_8V 65
#define GPIO_MHL_WAKE_UP  124
#define GPIO_MHL_INT      127
#define GPIO_HDMI_EN      123
#define GPIO_MHL_RST      128
#define GPIO_MHL_SEL      17
#define GPIO_HDMI_HPD     137
#define I2C_BUS_ID_MHL	  15

static void sii9234_cfg_gpio(void)
{
	int gpio, rc;
	
	printk(KERN_INFO "%s()\n", __func__);
	
	/* GPH1(6) XEINT 14 */

	/*
	irq_set_irq_type(MHL_WAKEUP_IRQ, IRQ_TYPE_EDGE_RISING);
       */
	gpio = GPIO_MHL_WAKE_UP;
	rc = gpio_request(gpio, "MHL_WAKEUP");
	if (rc < 0) {
		printk(KERN_ERR "unable to request GPIO pin %d\n", gpio);
		return;
	}
	gpio_direction_input(gpio);
	
	gpio = GPIO_MHL_INT;
	rc = gpio_request(gpio, "MHL_INT");
	if (rc < 0) {
		printk(KERN_ERR "unable to request GPIO pin %d\n", gpio);
		return;
	}
	gpio_direction_input(gpio);
	/*
	irq_set_irq_type(MHL_INT_IRQ, IRQ_TYPE_EDGE_RISING);
	*/
	
	gpio = GPIO_HDMI_EN;
	rc = gpio_request(gpio, "HDMI_EN");
	if (rc < 0) {
		printk(KERN_ERR "unable to request GPIO pin %d\n", gpio);
		return;
	}
	gpio_direction_output(gpio, 0);

	gpio = GPIO_MHL_RST;
	rc = gpio_request(gpio, "MHL_RST");
	if (rc < 0) {
		printk(KERN_ERR "unable to request GPIO pin %d\n", gpio);
		return;
	}
	gpio_direction_output(gpio, 0);
	

	gpio = GPIO_MHL_SEL;
	rc = gpio_request(gpio, "MHL_SEL");
	if (rc < 0) {
		printk(KERN_ERR "unable to request GPIO pin %d\n", gpio);
		return;
	}
	gpio_direction_output(gpio, 0);

}

static void sii9234_power_onoff(bool on)
{
	printk(KERN_INFO "%s(%d)\n", __func__, on);

	if (on) {
		/* To avoid floating state of the HPD pin *
		 * in the absence of external pull-up     */
		/*s3c_gpio_setpull(GPIO_HDMI_HPD, S3C_GPIO_PULL_NONE);*/
		gpio_set_value(GPIO_HDMI_EN, 1);

		/* todo: configure pull for i2c pad*/
		/*
		s3c_gpio_setpull(GPIO_MHL_SCL_1_8V, S3C_GPIO_PULL_DOWN);
		s3c_gpio_setpull(GPIO_MHL_SCL_1_8V, S3C_GPIO_PULL_NONE);
		*/
		
		/* sii9234_unmaks_interrupt(); // - need to add */
		/* VCC_SUB_2.0V is always on */
	} else {
		gpio_set_value(GPIO_MHL_RST, 0);
		usleep_range(10000, 20000);
		gpio_set_value(GPIO_MHL_RST, 1);

		/* To avoid floating state of the HPD pin *
		 * in the absence of external pull-up     */
		/*
		s3c_gpio_setpull(GPIO_HDMI_HPD, S3C_GPIO_PULL_DOWN);
		*/
		gpio_set_value(GPIO_HDMI_EN, 0);

		gpio_set_value(GPIO_MHL_RST, 1);
	}
}

static void sii9234_reset(void)
{
	printk(KERN_INFO "%s()\n", __func__);

	gpio_set_value(GPIO_MHL_RST, 0);
	usleep_range(10000, 20000);
	gpio_set_value(GPIO_MHL_RST, 1);
}

static void mhl_usb_switch_control(bool on)
{
	printk(KERN_INFO "%s() [MHL] USB path change : %s\n",
	       __func__, on ? "MHL" : "USB");
	if (on == 1) {
		if (gpio_get_value(GPIO_MHL_SEL))
			printk(KERN_INFO "[MHL] GPIO_MHL_SEL : already 1\n");
		else
			gpio_set_value(GPIO_MHL_SEL, 1);
	} else {
		if (!gpio_get_value(GPIO_MHL_SEL))
			printk(KERN_INFO "[MHL] GPIO_MHL_SEL : already 0\n");
		else
			gpio_set_value(GPIO_MHL_SEL, 0);
	}
}


static struct sii9234_platform_data sii9234_pdata = {
	.gpio_mhl_int = GPIO_MHL_INT,
	.init = sii9234_cfg_gpio,
	.mhl_sel = mhl_usb_switch_control,
	.hw_onoff = sii9234_power_onoff,
	.hw_reset = sii9234_reset,
	.enable_vbus = NULL,
	.vbus_present = NULL,
};

static struct i2c_board_info __initdata i2c_devs_sii9234[] = {
	{
		I2C_BOARD_INFO("sii9234_mhl_tx", 0x72>>1),
		.platform_data = &sii9234_pdata,
	},
	{
		I2C_BOARD_INFO("sii9234_tpi", 0x7A>>1),
		.platform_data = &sii9234_pdata,
	},
	{
		I2C_BOARD_INFO("sii9234_hdmi_rx", 0x92>>1),
		.platform_data = &sii9234_pdata,
	},
	{
		I2C_BOARD_INFO("sii9234_cbus", 0xC8>>1),
		.platform_data = &sii9234_pdata,
	},
};

static struct i2c_board_info i2c_dev_hdmi_ddc __initdata = {
	I2C_BOARD_INFO("s5p_ddc", (0x74 >> 1)),
};

static void __init board_mhl_init(void)
{
	int ret;

	ret = i2c_register_board_info(I2C_BUS_ID_MHL, i2c_devs_sii9234,
			ARRAY_SIZE(i2c_devs_sii9234));

	if (ret < 0) {
		printk(KERN_ERR "[MHL] adding i2c fail - nodevice\n");
		return;
	}
#if defined(CONFIG_MACH_S2PLUS) || defined(CONFIG_MACH_P4NOTE)
	sii9234_pdata.ddc_i2c_num = 5;
#else
	sii9234_pdata.ddc_i2c_num = (system_rev == 3 ? 16 : 5);
#endif

#ifdef CONFIG_MACH_SLP_PQ_LTE
	sii9234_pdata.ddc_i2c_num = 16;
#endif
	ret = i2c_register_board_info(sii9234_pdata.ddc_i2c_num, &i2c_dev_hdmi_ddc, 1);
	if (ret < 0) {
		printk(KERN_ERR "[MHL] adding ddc fail - nodevice\n");
		return;
	}

	return;
}

#endif
