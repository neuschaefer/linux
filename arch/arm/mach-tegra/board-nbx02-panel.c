/* 2012-04-12: File added and changed by Sony Corporation */
/*
 * arch/arm/mach-tegra/board-nbx02-panel.c
 *
 * Copyright (c) 2010, NVIDIA Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/regulator/consumer.h>
#include <linux/resource.h>
#include <asm/mach-types.h>
#include <linux/platform_device.h>
#include <linux/earlysuspend.h>
#include <linux/pwm_backlight.h>
#include <linux/nbx02_backlight.h>
#include <linux/nvhost.h>
#include <mach/nvmap.h>
#include <mach/irqs.h>
#include <mach/iomap.h>
#include <mach/dc.h>
#include <mach/fb.h>
#include <linux/reboot.h>

#include "devices.h"
#include "gpio-names.h"
#include "board.h"


//#define NBX02_LCD_ENABLE_POWEROFF
//#define USE_EARLYSUSPEND
#define USE_EARLYSUSPEND_BLANK

void off_keep_timer_stop(struct backlight_device *);

static struct platform_nbx02_backlight_data nbx02_backlight_data = {
	.pwm[0] = {
		.pwm_id		= 0,
		.max_brightness	= 255,
		.dft_brightness	= 255,
		.pwm_period_ns	= 2777777,
	},
	.pwm[1] = {
		.pwm_id		= 1,
		.max_brightness	= 255,
		.dft_brightness	= 255,
		.pwm_period_ns	= 2777777,
	},
};

static struct platform_device nbx02_backlight_device = {
	.name	= "nbx02_backlight",
	.id	= -1,
	.dev	= {
		.platform_data = &nbx02_backlight_data,
	},
};

static void nbx02_lcd_i2c_write(struct i2c_adapter *adap, u16 addr, u8 *buf, u16 len)
{
	struct i2c_msg i2c_msg;

	i2c_msg.addr	= addr;
	i2c_msg.flags	= 0;
	i2c_msg.len	= len;
	i2c_msg.buf	= buf;

	i2c_transfer(adap, &i2c_msg, 1);
}

static const int nbx02_V18LcdEn   = TEGRA_GPIO_PM4;
static const int nbx02_V33LcdEn   = TEGRA_GPIO_PM5;
static const int nbx02_Lcd0ResetN = TEGRA_GPIO_PM6;
static const int nbx02_Lcd1ResetN = TEGRA_GPIO_PM7;
#define LCD_I2C_ADDR_UPPER 0x3d
#define LCD_I2C_ADDR_LOWER 0x3f

static void nbx02_panel_on(void)
{
	u8 values[2];
	struct i2c_adapter *adap = i2c_get_adapter(1);

	//pr_info("%s\n", __FUNCTION__);
#ifdef NBX02_LCD_ENABLE_POWEROFF
	gpio_set_value(nbx02_V18LcdEn, 0x1);
	gpio_set_value(nbx02_V33LcdEn, 0x1);
	msleep(10); /* min. 1ms */

	gpio_set_value(nbx02_Lcd0ResetN, 0x1);
	gpio_set_value(nbx02_Lcd1ResetN, 0x1);
	msleep(20);
#endif

	values[0] = 0x36;
	values[1] = 0x00;
	nbx02_lcd_i2c_write(adap, LCD_I2C_ADDR_UPPER, values, 2);
	nbx02_lcd_i2c_write(adap, LCD_I2C_ADDR_LOWER, values, 2);
	values[0] = 0x3a;
	values[1] = 0x60;
	nbx02_lcd_i2c_write(adap, LCD_I2C_ADDR_UPPER, values, 2);
	nbx02_lcd_i2c_write(adap, LCD_I2C_ADDR_LOWER, values, 2);
	values[0] = 0xc6;
	values[1] = 0x11;
	nbx02_lcd_i2c_write(adap, LCD_I2C_ADDR_UPPER, values, 2);
	values[0] = 0xc6;
	values[1] = 0x21;
	nbx02_lcd_i2c_write(adap, LCD_I2C_ADDR_LOWER, values, 2);
}

static void nbx02_panel_off(void)
{
	//pr_info("%s\n", __FUNCTION__);
#ifdef NBX02_LCD_ENABLE_POWEROFF
	gpio_set_value(nbx02_Lcd0ResetN, 0x0);
	gpio_set_value(nbx02_Lcd1ResetN, 0x0);
	gpio_set_value(nbx02_V33LcdEn, 0x0);
	gpio_set_value(nbx02_V18LcdEn, 0x0);
#endif
}

static void nbx02_panel_resume(void)
{
	u8 values[5];
	struct i2c_adapter *adap = i2c_get_adapter(1);

	gpio_set_value(nbx02_Lcd0ResetN, 0x0);
	gpio_set_value(nbx02_Lcd1ResetN, 0x0);
	msleep(10);
	gpio_set_value(nbx02_Lcd0ResetN, 0x1);
	gpio_set_value(nbx02_Lcd1ResetN, 0x1);
	msleep(20);

	values[0] = 0xB0;
	values[1] = 0x04;
	nbx02_lcd_i2c_write(adap, LCD_I2C_ADDR_UPPER, values, 2);
	nbx02_lcd_i2c_write(adap, LCD_I2C_ADDR_LOWER, values, 2);
	values[0] = 0xD1;
	values[1] = 0x10;
	values[2] = 0x14;
	values[3] = 0x53;
	values[4] = 0xE3;
	nbx02_lcd_i2c_write(adap, LCD_I2C_ADDR_UPPER, values, 5);
	nbx02_lcd_i2c_write(adap, LCD_I2C_ADDR_LOWER, values, 5);
	values[0] = 0xB0;
	values[1] = 0x03;
	nbx02_lcd_i2c_write(adap, LCD_I2C_ADDR_UPPER, values, 2);
	nbx02_lcd_i2c_write(adap, LCD_I2C_ADDR_LOWER, values, 2);

	values[0] = 0x36;
	values[1] = 0x00;
	nbx02_lcd_i2c_write(adap, LCD_I2C_ADDR_UPPER, values, 2);
	nbx02_lcd_i2c_write(adap, LCD_I2C_ADDR_LOWER, values, 2);
	values[0] = 0x3a;
	values[1] = 0x60;
	nbx02_lcd_i2c_write(adap, LCD_I2C_ADDR_UPPER, values, 2);
	nbx02_lcd_i2c_write(adap, LCD_I2C_ADDR_LOWER, values, 2);
	values[0] = 0xc6;
	values[1] = 0x11;
	nbx02_lcd_i2c_write(adap, LCD_I2C_ADDR_UPPER, values, 2);
	values[0] = 0xc6;
	values[1] = 0x21;
	nbx02_lcd_i2c_write(adap, LCD_I2C_ADDR_LOWER, values, 2);

	//pr_info("%s\n", __FUNCTION__);
	values[0] = 0x29;
	nbx02_lcd_i2c_write(adap, LCD_I2C_ADDR_UPPER, values, 1);
	nbx02_lcd_i2c_write(adap, LCD_I2C_ADDR_LOWER, values, 1);
	values[0] = 0x11;
	nbx02_lcd_i2c_write(adap, LCD_I2C_ADDR_UPPER, values, 1);
	nbx02_lcd_i2c_write(adap, LCD_I2C_ADDR_LOWER, values, 1);
}

static void nbx02_panel_suspend(void)
{
	u8 values[2];
	struct i2c_adapter *adap = i2c_get_adapter(1);
	struct backlight_device *bl = platform_get_drvdata(&nbx02_backlight_device);

	//pr_info("%s\n", __FUNCTION__);
	values[0] = 0x10;
	nbx02_lcd_i2c_write(adap, LCD_I2C_ADDR_UPPER, values, 1);
	nbx02_lcd_i2c_write(adap, LCD_I2C_ADDR_LOWER, values, 1);
	msleep(130); /* LCD controller needs 130ms to suspend itself */

	off_keep_timer_stop(bl);
}

static int nbx02_panel_enable(void)
{
	//pr_info("%s\n", __FUNCTION__);
#if !defined(USE_EARLYSUSPEND) && !defined(USE_EARLYSUSPEND_BLANK)
	nbx02_panel_resume();
#endif
	return 0;
}

static int nbx02_panel_disable(void)
{
	//pr_info("%s\n", __FUNCTION__);
#if !defined(USE_EARLYSUSPEND) && !defined(USE_EARLYSUSPEND_BLANK)
	nbx02_panel_suspend();
#endif
	return 0;
}

static struct resource nbx02_disp1_resources[] = {
	{
		.name	= "irq",
		.start	= INT_DISPLAY_GENERAL,
		.end	= INT_DISPLAY_GENERAL,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name	= "regs",
		.start	= TEGRA_DISPLAY_BASE,
		.end	= TEGRA_DISPLAY_BASE + TEGRA_DISPLAY_SIZE-1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "fbmem",
		.flags	= IORESOURCE_MEM,
	},
};

static struct tegra_dc_mode nbx02_panel_modes[] = {
	{
		.pclk = 48000000, /* Vsync freq. is 46HZ */
		.h_ref_to_sync = 1,
		.v_ref_to_sync = 1,
		.h_sync_width = 8,
		.v_sync_width = 1,
		.h_back_porch = 16,
		.v_back_porch = 0,
		.h_active = 1024,
		.v_active = 960,
		.h_front_porch = 23,
		.v_front_porch = 11,
		.flags = TEGRA_DC_MODE_FLAG_NEG_V_SYNC | TEGRA_DC_MODE_FLAG_NEG_H_SYNC,
	},
};

static struct tegra_fb_data nbx02_fb_data = {
	.win		= 0,
	.xres		= 1024,
	.yres		= 960,
	.bits_per_pixel	= 32,
};

static struct tegra_dc_out_pin nbx02_panel_out_pins[] = {
	{
		.name = TEGRA_DC_OUT_PIN_H_SYNC,
		.pol  = TEGRA_DC_OUT_PIN_POL_LOW,
	},
	{
		.name = TEGRA_DC_OUT_PIN_V_SYNC,
		.pol  = TEGRA_DC_OUT_PIN_POL_LOW,
	},
	{
		.name = TEGRA_DC_OUT_PIN_PIXEL_CLOCK,
		.pol  = TEGRA_DC_OUT_PIN_POL_HIGH,
	},
};

static struct tegra_dc_out nbx02_disp1_out = {
	.type		= TEGRA_DC_OUT_RGB,

	.align		= TEGRA_DC_ALIGN_MSB,
	.order		= TEGRA_DC_ORDER_RED_BLUE,

	.depth		= 18,
	//.dither		= TEGRA_DC_DISABLE_DITHER,
	.dither		= TEGRA_DC_ORDERED_DITHER,
	//.dither		= TEGRA_DC_ERRDIFF_DITHER,

	.modes	 	= nbx02_panel_modes,
	.n_modes 	= ARRAY_SIZE(nbx02_panel_modes),

	.enable		= nbx02_panel_enable,
	.disable	= nbx02_panel_disable,

	.out_pins	= nbx02_panel_out_pins,
	.n_out_pins	= ARRAY_SIZE(nbx02_panel_out_pins),
};

static struct tegra_dc_platform_data nbx02_disp1_pdata = {
	.flags		= TEGRA_DC_FLAG_ENABLED,
	.default_out	= &nbx02_disp1_out,
	.fb		= &nbx02_fb_data,
};

static struct nvhost_device nbx02_disp1_device = {
	.name		= "tegradc",
	.id		= 0,
	.resource	= nbx02_disp1_resources,
	.num_resources	= ARRAY_SIZE(nbx02_disp1_resources),
	.dev = {
		.platform_data = &nbx02_disp1_pdata,
	},
};

static struct nvmap_platform_carveout nbx02_carveouts[] = {
	[0] = NVMAP_HEAP_CARVEOUT_IRAM_INIT,
	[1] = {
		.name		= "generic-0",
		.usage_mask	= NVMAP_HEAP_CARVEOUT_GENERIC,
		.buddy_size	= SZ_32K,
	},
};

static struct nvmap_platform_data nbx02_nvmap_data = {
	.carveouts	= nbx02_carveouts,
	.nr_carveouts	= ARRAY_SIZE(nbx02_carveouts),
};

static struct platform_device nbx02_nvmap_device = {
	.name	= "tegra-nvmap",
	.id	= -1,
	.dev	= {
		.platform_data = &nbx02_nvmap_data,
	},
};

static struct platform_device *nbx02_gfx_devices[] __initdata = {
	&nbx02_nvmap_device,
#ifdef CONFIG_TEGRA_GRHOST
	&tegra_grhost_device,
#endif
	&tegra_pwfm0_device,
	&tegra_pwfm1_device,
	&nbx02_backlight_device,
};

#ifdef CONFIG_HAS_EARLYSUSPEND
/* put early_suspend/late_resume handlers here for the display in order
 * to keep the code out of the display driver, keeping it closer to upstream
 */
static struct early_suspend nbx02_panel_early_suspender;

static void nbx02_panel_early_suspend(struct early_suspend *h)
{
	/* must be called before screen blank*/
#ifdef USE_EARLYSUSPEND
	nbx02_panel_suspend();
#endif
	//pr_info("%s: pre\n", __FUNCTION__);
	if (num_registered_fb > 0)
		fb_blank(registered_fb[0], FB_BLANK_POWERDOWN);
	//pr_info("%s: post\n", __FUNCTION__);
}

static void nbx02_panel_late_resume(struct early_suspend *h)
{
	//pr_info("%s: pre\n", __FUNCTION__);
	if (num_registered_fb > 0)
		fb_blank(registered_fb[0], FB_BLANK_UNBLANK);
	//pr_info("%s: post\n", __FUNCTION__);
	
	/* must be called after screen unblank*/
#ifdef USE_EARLYSUSPEND
	nbx02_panel_resume();
#endif
}

static struct early_suspend nbx02_lcd_early_suspender;

static void nbx02_lcd_early_suspend(struct early_suspend *h)
{
	//pr_info("%s: pre\n", __FUNCTION__);
	/* must be called before screen blank*/
#ifdef USE_EARLYSUSPEND_BLANK
	nbx02_panel_suspend();
#endif
	//pr_info("%s: post\n", __FUNCTION__);
}

static void nbx02_lcd_late_resume(struct early_suspend *h)
{
	//pr_info("%s: pre\n", __FUNCTION__);
	msleep(100);
	/* must be called after screen unblank*/
#ifdef USE_EARLYSUSPEND_BLANK
	nbx02_panel_resume();
#endif
	//pr_info("%s: post\n", __FUNCTION__);
}
#endif

#if 0
static struct i2c_board_info __initdata nbx02_lcd_i2c_info[] = {
	{
		I2C_BOARD_INFO("lcd_upper", LCD_I2C_ADDR_UPPER),
	},
	{
		I2C_BOARD_INFO("lcd_lower", LCD_I2C_ADDR_LOWER),
	},
};

static int nbx02_lcd_probe(struct i2c_client *client, const struct i2c_device_id *dev_id)
{
	if (client->addr == LCD_I2C_ADDR_UPPER) {
		pr_info("%s: upper\n", __FUNCTION__);
	} else if (client->addr == LCD_I2C_ADDR_LOWER) {
		pr_info("%s: lower\n", __FUNCTION__);
	} else {
		pr_info("%s: unknown\n", __FUNCTION__);
	}

	return 0;
}

static int __devexit nbx02_lcd_remove(struct i2c_client *client)
{
	return 0;
}

static const struct i2c_device_id nbx02_lcd_id[] = {
	{ "lcd_upper", 0},
	{ "lcd_lower", 1},
	{},
};

static struct i2c_driver nbx02_lcd = {
	.driver = {
		.name = "nbx02_lcd",
	},
	.probe    = nbx02_lcd_probe,
	.remove   = nbx02_lcd_remove,
	.id_table = nbx02_lcd_id,
};
#endif

static int nbx02_gpio_request(int gpio_num, const char *gpio_name, int value)
{
	int ret;

	ret = gpio_request(gpio_num, gpio_name);
	if (ret < 0) {
		pr_err("gpio req err: %d,%s\n", gpio_num, gpio_name);
		return ret;
	}
	ret = gpio_direction_output(gpio_num, value);
	if (ret < 0) {
		pr_err("gpio dir err: %d,%s\n", gpio_num, gpio_name);
		gpio_free(gpio_num);
		return ret;
	}

	tegra_gpio_enable(gpio_num);
	pr_info("gpio requested: %d as %s\n", gpio_num, gpio_name);

	return 0;
}

static int nbx02_lcd_reboot_event(struct notifier_block* this, unsigned long event, void* ptr)
{
	int ret = NOTIFY_DONE;

	switch (event) {
	case SYS_RESTART:
	case SYS_HALT:
	case SYS_POWER_OFF:
		nbx02_panel_suspend();
		ret = NOTIFY_OK;
		break;
	default:
		break;
	}

	return ret;
}

static struct notifier_block nbx02_lcd_reboot_notifier = {
	.notifier_call = nbx02_lcd_reboot_event,
};

static int nbx02_lcd_init(void)
{
	int ret;

	ret = nbx02_gpio_request(nbx02_V18LcdEn, "V18LcdEn", 1);
	if (ret < 0) return ret;
	ret = nbx02_gpio_request(nbx02_V33LcdEn, "V33LcdEn", 1);
	if (ret < 0) return ret;
	ret = nbx02_gpio_request(nbx02_Lcd0ResetN, "Lcd0ResetN", 1);
	if (ret < 0) return ret;
	ret = nbx02_gpio_request(nbx02_Lcd1ResetN, "Lcd1ResetN", 1);
	if (ret < 0) return ret;
#if 0
	i2c_register_board_info(1, nbx02_lcd_i2c_info, ARRAY_SIZE(nbx02_lcd_i2c_info));
	i2c_add_driver(&nbx02_lcd);
#endif

	register_reboot_notifier(&nbx02_lcd_reboot_notifier);

	return ret;
};

int __init nbx02_panel_init(void)
{
	int err;
	struct resource *res;

	nbx02_lcd_init();

#ifdef CONFIG_HAS_EARLYSUSPEND
	nbx02_panel_early_suspender.suspend = nbx02_panel_early_suspend;
	nbx02_panel_early_suspender.resume = nbx02_panel_late_resume;
	nbx02_panel_early_suspender.level = EARLY_SUSPEND_LEVEL_DISABLE_FB;
	register_early_suspend(&nbx02_panel_early_suspender);

	nbx02_lcd_early_suspender.suspend = nbx02_lcd_early_suspend;
	nbx02_lcd_early_suspender.resume = nbx02_lcd_late_resume;
	nbx02_lcd_early_suspender.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN;
	register_early_suspend(&nbx02_lcd_early_suspender);
#endif

	nbx02_carveouts[1].base = tegra_carveout_start;
	nbx02_carveouts[1].size = tegra_carveout_size;

	err = platform_add_devices(nbx02_gfx_devices,
				   ARRAY_SIZE(nbx02_gfx_devices));

	res = nvhost_get_resource_byname(&nbx02_disp1_device,
		IORESOURCE_MEM, "fbmem");
	res->start = tegra_fb_start;
	res->end = tegra_fb_start + tegra_fb_size - 1;

	if (!err)
		err = nvhost_device_register(&nbx02_disp1_device);

	return err;
}
