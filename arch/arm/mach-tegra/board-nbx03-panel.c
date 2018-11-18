/* 2012-04-12: File added and changed by Sony Corporation */
/*
 * arch/arm/mach-tegra/board-nbx03-panel.c
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
#include <linux/nvhost.h>
#include <mach/nvmap.h>
#include <mach/irqs.h>
#include <mach/iomap.h>
#include <mach/dc.h>
#include <mach/fb.h>
#include <mach/tegra_odm_fuses.h>

#include "devices.h"
#include "gpio-names.h"
#include "board.h"

//#define USE_EARLYSUSPEND
#define USE_EARLYSUSPEND_BLANK

static const int nbx03_BlOff   = TEGRA_GPIO_PM4;
static const int nbx03_LcdEn   = TEGRA_GPIO_PM5;
static const int nbx03_PnlID   = TEGRA_GPIO_PK7;

static struct platform_pwm_backlight_data nbx03_backlight_data = {
	.pwm_id		= 0,
	.max_brightness	= 255,
	.dft_brightness	= 197,
	.pwm_period_ns	= 4760000,
};

static struct platform_device nbx03_backlight_device = {
	.name	= "pwm-backlight",
	.id	= -1,
	.dev	= {
		.platform_data = &nbx03_backlight_data,
	},
};

#define MODELID_EVT 0x1c
#define MODELID_DVT 0x18
#define MODELID_PVT 0x14
#define MODELID_MP  0x10
static int get_model_id(void)
{
    int ret=0;
    u32 odm_rsvd[8];
    int model_id;

    ret = tegra_fuse_read(ODM_RSVD, odm_rsvd, sizeof(odm_rsvd));
    if(ret!=0)return 0;
    model_id = odm_rsvd[0]&0x7f;
/*{
    int i;
    for(i=0; i<8; i++){pr_info("%08x ", odm_rsvd[i]);}
    pr_info("\n[%s] 0x%x\n", __FUNCTION__, model_id);
}*/
    return model_id;
};

static int nbx03_panel_enable(void)
{
	int model_id;
	gpio_set_value(nbx03_LcdEn, 0x1);
	msleep(10);

	model_id = get_model_id();
	if((model_id==MODELID_EVT) || !model_id) {
		gpio_set_value(nbx03_BlOff, 0x0);
		msleep(10);
	}
	return 0;
}

static int nbx03_panel_disable(void)
{
	int model_id;
	model_id = get_model_id();
	if((model_id==MODELID_EVT) || !model_id) {
		gpio_set_value(nbx03_BlOff, 0x1);
		msleep(100);
	}
	gpio_set_value(nbx03_LcdEn, 0x0);
	msleep(10);
	return 0;
}

static void nbx03_panel_resume(void)
{
/* Nothing to do */
}

static void nbx03_panel_suspend(void)
{
/* Nothing to do */
}

static struct resource nbx03_disp1_resources[] = {
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

static struct tegra_dc_mode nbx03_panel_modes[] = {
	{
		.pclk = 71980800,
		.h_ref_to_sync = 0,
		.v_ref_to_sync = 1,
		.h_sync_width = 32,
		.v_sync_width = 3,
		.h_back_porch = 70,
		.v_back_porch = 9,
		.h_active = 1280,
		.v_active = 800,
		.h_front_porch = 90,
		.v_front_porch = 3,
		.flags = TEGRA_DC_MODE_FLAG_NEG_V_SYNC | TEGRA_DC_MODE_FLAG_NEG_H_SYNC,
	},
};

static struct tegra_fb_data nbx03_fb_data = {
	.win		= 0,
	.xres		= 1280,
	.yres		= 800,
	.bits_per_pixel	= 32,
};

static struct tegra_dc_out_pin nbx03_panel_out_pins[] = {
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
		.pol  = TEGRA_DC_OUT_PIN_POL_LOW,
	},
};

static struct tegra_dc_out nbx03_disp1_out = {
	.type		= TEGRA_DC_OUT_RGB,

	.align		= TEGRA_DC_ALIGN_MSB,
	.order		= TEGRA_DC_ORDER_RED_BLUE,

	.depth		= 18,
	//.dither		= TEGRA_DC_DISABLE_DITHER,
	.dither		= TEGRA_DC_ORDERED_DITHER,
	//.dither		= TEGRA_DC_ERRDIFF_DITHER,

	.modes	 	= nbx03_panel_modes,
	.n_modes	= ARRAY_SIZE(nbx03_panel_modes),

	.enable		= nbx03_panel_enable,
	.disable	= nbx03_panel_disable,

	.out_pins	= nbx03_panel_out_pins,
	.n_out_pins	= ARRAY_SIZE(nbx03_panel_out_pins),
};

static struct tegra_dc_platform_data nbx03_disp1_pdata = {
	.flags		= TEGRA_DC_FLAG_ENABLED,
	.default_out	= &nbx03_disp1_out,
	.fb		= &nbx03_fb_data,
};

static struct nvhost_device nbx03_disp1_device = {
	.name		= "tegradc",
	.id		= 0,
	.resource	= nbx03_disp1_resources,
	.num_resources	= ARRAY_SIZE(nbx03_disp1_resources),
	.dev = {
		.platform_data = &nbx03_disp1_pdata,
	},
};

static struct nvmap_platform_carveout nbx03_carveouts[] = {
	[0] = NVMAP_HEAP_CARVEOUT_IRAM_INIT,
	[1] = {
		.name		= "generic-0",
		.usage_mask	= NVMAP_HEAP_CARVEOUT_GENERIC,
		.buddy_size	= SZ_32K,
	},
};

static struct nvmap_platform_data nbx03_nvmap_data = {
	.carveouts	= nbx03_carveouts,
	.nr_carveouts	= ARRAY_SIZE(nbx03_carveouts),
};

static struct platform_device nbx03_nvmap_device = {
	.name	= "tegra-nvmap",
	.id	= -1,
	.dev	= {
		.platform_data = &nbx03_nvmap_data,
	},
};

static struct platform_device *nbx03_gfx_devices[] __initdata = {
	&nbx03_nvmap_device,
#ifdef CONFIG_TEGRA_GRHOST
	&tegra_grhost_device,
#endif
	&tegra_pwfm0_device,
	&nbx03_backlight_device,
};

#ifdef CONFIG_HAS_EARLYSUSPEND
/* put early_suspend/late_resume handlers here for the display in order
 * to keep the code out of the display driver, keeping it closer to upstream
 */
struct early_suspend nbx03_panel_early_suspender;

static void nbx03_panel_early_suspend(struct early_suspend *h)
{
	/*pr_info("#[%s] pre\n", __FUNCTION__);*/
#ifdef USE_EARLYSUSPEND
	nbx03_panel_suspend();
#endif
	if (num_registered_fb > 0)
		fb_blank(registered_fb[0], FB_BLANK_POWERDOWN);
	/*pr_info("#[%s] post\n", __FUNCTION__);*/
}

static void nbx03_panel_late_resume(struct early_suspend *h)
{
	/*pr_info("#[%s] pre\n", __FUNCTION__);*/
	if (num_registered_fb > 0)
		fb_blank(registered_fb[0], FB_BLANK_UNBLANK);
#ifdef USE_EARLYSUSPEND
	nbx03_panel_resume();
#endif
	/*pr_info("#[%s] post\n", __FUNCTION__);*/
}

static struct early_suspend nbx03_lcd_early_suspender;

static void nbx03_lcd_early_suspend(struct early_suspend *h)
{
	/* must be called before screen blank*/
#ifdef USE_EARLYSUSPEND_BLANK
	nbx03_panel_suspend();
#endif
}

static void nbx03_lcd_late_resume(struct early_suspend *h)
{
	/* must be called after screen unblank*/
#ifdef USE_EARLYSUSPEND_BLANK
	nbx03_panel_resume();
#endif
}
#endif

static int nbx03_gpio_request(int gpio_num, const char *gpio_name, int value)
{
	int ret;

	ret = gpio_request(gpio_num, gpio_name);
	if (ret < 0) {
		pr_err("[%s]gpio req err: %d,%s\n", __FUNCTION__, gpio_num, gpio_name);
		return ret;
	}
	ret = gpio_direction_output(gpio_num, value);
	if (ret < 0) {
		pr_err("[%s]gpio dir err: %d,%s\n", __FUNCTION__, gpio_num, gpio_name);
		gpio_free(gpio_num);
		return ret;
	}

	tegra_gpio_enable(gpio_num);
	pr_info("[%s]gpio req: %d as %s\n", __FUNCTION__, gpio_num, gpio_name);

	return 0;
}

static int nbx03_lcd_init(void)
{
	int ret, model_id;

	model_id = get_model_id();
	if((model_id==MODELID_EVT) || !model_id) {
	ret = nbx03_gpio_request(nbx03_BlOff, "BlOff", 1);
	if (ret < 0) return ret;
	}
	ret = nbx03_gpio_request(nbx03_LcdEn, "LcdEn", 1);
	if (ret < 0) return ret;
	return ret;
};

int __init nbx03_panel_init(void)
{
	int err;
	struct resource *res;

	nbx03_lcd_init();

#ifdef CONFIG_HAS_EARLYSUSPEND
	nbx03_panel_early_suspender.suspend = nbx03_panel_early_suspend;
	nbx03_panel_early_suspender.resume = nbx03_panel_late_resume;
	nbx03_panel_early_suspender.level = EARLY_SUSPEND_LEVEL_DISABLE_FB;
	register_early_suspend(&nbx03_panel_early_suspender);

	nbx03_lcd_early_suspender.suspend = nbx03_lcd_early_suspend;
	nbx03_lcd_early_suspender.resume = nbx03_lcd_late_resume;
	nbx03_lcd_early_suspender.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN;
	register_early_suspend(&nbx03_lcd_early_suspender);
#endif

	nbx03_carveouts[1].base = tegra_carveout_start;
	nbx03_carveouts[1].size = tegra_carveout_size;

	err = platform_add_devices(nbx03_gfx_devices,
				   ARRAY_SIZE(nbx03_gfx_devices));

	res = nvhost_get_resource_byname(&nbx03_disp1_device,
		IORESOURCE_MEM, "fbmem");
	res->start = tegra_fb_start;
	res->end = tegra_fb_start + tegra_fb_size - 1;

	if (!err)
		err = nvhost_device_register(&nbx03_disp1_device);

	return err;
}

