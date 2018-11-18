/* 2012-04-12: File added and changed by Sony Corporation */
/*
 * arch/arm/mach-tegra/board-nbx03.c
 *
 * Copyright (c) 2010 - 2011, NVIDIA Corporation.
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

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/ctype.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/serial_8250.h>
#include <linux/i2c.h>
#include <linux/dma-mapping.h>
#include <linux/delay.h>
#include <linux/i2c-tegra.h>
#include <linux/gpio.h>
#include <linux/input.h>
#include <linux/platform_data/tegra_usb.h>
#include <linux/mfd/tps6586x.h>
#include <linux/memblock.h>
#include <linux/tegra_uart.h>
#include <linux/nbx_ec_ipc.h>
#include <linux/nbx_ec_battery.h>
#include <linux/nbx_dock.h>

#include <sound/wm8903.h>

#include <mach/clk.h>
#include <mach/iomap.h>
#include <mach/irqs.h>
#include <mach/pinmux.h>
#include <mach/iomap.h>
#include <mach/io.h>
#include <mach/i2s.h>
#include <mach/tegra_wm8903_pdata.h>

#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <mach/usb_phy.h>
#include <mach/hci-event.h>
#include <mach/a1026.h>

#include "board.h"
#include "clock.h"
#include "board-nbx03.h"
#include "devices.h"
#include "gpio-names.h"
#include "fuse.h"
#include "board-nbx-common.h"
#include "wireless_power_control.h"
#include "pm.h"
#include "pm-irq.h"

#if defined(CONFIG_TOUCHSCREEN_CYTTSP_I2C) || defined(CONFIG_TOUCHSCREEN_CYTTSP_I2C_MODULE)
#define TOUCHSCREEN_CYTTSP_I2C
#endif

#ifdef TOUCHSCREEN_CYTTSP_I2C
#include <linux/input/cyttsp.h>
/* default bootloader keys */
u8 dflt_bl_keys[] = {
	0, 1, 2, 3, 4, 5, 6, 7
};
enum cyttsp_gest {
	CY_GEST_GRP_NONE = 0x0F,
	CY_GEST_GRP1 =	0x10,
	CY_GEST_GRP2 = 0x20,
	CY_GEST_GRP3 = 0x40,
	CY_GEST_GRP4 = 0x80,
};
#endif

static struct tegra_utmip_config utmi_phy_config[] = {
	[0] = {
			.hssync_start_delay = 9,
			.idle_wait_delay = 17,
			.elastic_limit = 16,
			.term_range_adj = 6,
			.xcvr_setup = 15,
			.xcvr_setup_offset = 0,
			.xcvr_use_fuses = 1,
			.xcvr_lsfslew = 2,
			.xcvr_lsrslew = 2,
	},
	[1] = {
			.hssync_start_delay = 9,
			.idle_wait_delay = 17,
			.elastic_limit = 16,
			.term_range_adj = 6,
			.xcvr_setup = 8,
			.xcvr_setup_offset = 0,
			.xcvr_use_fuses = 1,
			.xcvr_lsfslew = 2,
			.xcvr_lsrslew = 2,
	},
};

//static struct tegra_ulpi_config ulpi_phy_config = {
//	.reset_gpio = TEGRA_GPIO_PG2,
//	.clk = "clk_dev2",
//	.inf_type = TEGRA_USB_LINK_ULPI,
//};

#ifdef CONFIG_BT_BLUESLEEP
static noinline void __init tegra_setup_bluesleep(void)
{
	struct platform_device *pdev = NULL;
	struct resource *res;

	pdev = platform_device_alloc("bluesleep", 0);
	if (!pdev) {
		pr_err("unable to allocate platform device for bluesleep");
		return;
	}

	res = kzalloc(sizeof(struct resource) * 2, GFP_KERNEL);
	if (!res) {
		pr_err("unable to allocate resource for bluesleep\n");
		goto err_free_dev;
	}

	res[0].name   = "gpio_host_wake";
	res[0].start  = TEGRA_GPIO_PS2;
	res[0].end    = TEGRA_GPIO_PS2;
	res[0].flags  = IORESOURCE_IO;

	res[1].name   = "host_wake";
	res[1].start  = gpio_to_irq(TEGRA_GPIO_PS2);
	res[1].end    = gpio_to_irq(TEGRA_GPIO_PS2);
	res[1].flags  = IORESOURCE_IRQ | IORESOURCE_IRQ_HIGHEDGE;

	if (platform_device_add_resources(pdev, res, 2)) {
		pr_err("unable to add resources to bluesleep device\n");
		goto err_free_res;
	}

	if (platform_device_add(pdev)) {
		pr_err("unable to add bluesleep device\n");
		goto err_free_res;
	}

	tegra_gpio_enable(TEGRA_GPIO_PS2);

	return;

err_free_res:
	kfree(res);
err_free_dev:
	platform_device_put(pdev);
	return;
}
#else
static inline void tegra_setup_bluesleep(void) { }
#endif

static __initdata struct tegra_clk_init_table nbx03_clk_init_table[] = {
	/* name		parent		rate		enabled */
	{ "blink",	"clk_32k",	32768,		false},
	{ "pll_p_out4",	"pll_p",	24000000,	true },
        { "pwm",        "clk_m",        13000000,       false},
	{ "pll_a",	NULL,		56448000,	true},
	{ "pll_a_out0",	NULL,		11289600,	true},
	{ "i2s1",	"pll_a_out0",	11289600,	true},
	{ "i2s2",	"pll_a_out0",	11289600,	true},
	{ "audio",	"pll_a_out0",	11289600,	true},
	{ "audio_2x",	"audio",	22579200,	true},
        { "disp1",      "pll_p",        216000000,      true},
	{ "spdif_out",	"pll_a_out0",	5644800,	false},
	{ "kbc",	"clk_32k",	32768,		true},
	{ NULL,		NULL,		0,		0},
};

//static struct tegra_ulpi_config nbx03_ehci2_ulpi_phy_config = {
//	.reset_gpio = TEGRA_GPIO_PV1,
//	.clk = "clk_dev2",
//};

//static struct tegra_ehci_platform_data nbx03_ehci2_ulpi_platform_data = {
//	.operating_mode = TEGRA_USB_HOST,
//	.power_down_on_bus_suspend = 0,
//	.phy_config = &nbx03_ehci2_ulpi_phy_config,
//};

static const struct tegra_i2c_clk_table nbx03_i2c1_clk_table[] = {
	{
		.addr = 0x3e,
		.clk_rate = 100000,
	},
};

static struct tegra_i2c_platform_data nbx03_i2c1_platform_data = {
	.adapter_nr	= 0,
	.bus_count	= 1,
	.bus_clk_rate	= { 400000, 0 },
	.slave_addr = 0x00FC,
	.clk_table = nbx03_i2c1_clk_table,
	.table_size = ARRAY_SIZE(nbx03_i2c1_clk_table),
};

static struct tegra_i2c_platform_data nbx03_i2c2_platform_data = {
	.adapter_nr	= 1,
	.bus_count	= 1,
	.bus_clk_rate	= { 400000, 0 },
	.slave_addr = 0x00FC,
};

static struct tegra_i2c_platform_data nbx03_i2c3_platform_data = {
	.adapter_nr	= 2,
	.bus_count	= 1,
	.bus_clk_rate	= { 400000, 0 },
	.slave_addr = 0x00FC,
};

static struct tegra_i2c_platform_data nbx03_dvc_platform_data = {
	.adapter_nr	= 3,
	.bus_count	= 1,
	.bus_clk_rate	= { 400000, 0 },
	.is_dvc		= true,
};

#ifdef TOUCHSCREEN_CYTTSP_I2C
static int cyttsp_wakeup(void)
{
	int ret;

	ret = gpio_direction_output(TEGRA_GPIO_PA4, 1);
	if (ret) {
		printk(KERN_ERR "%s: Failed to request gpio_direction_output\n",
		__func__);
                return ret;
	}
	gpio_set_value(TEGRA_GPIO_PA4, 1);
	msleep(300);
	return 0;
}

static int cyttsp_poweroff(void)
{
	int ret;

	ret = gpio_direction_output(TEGRA_GPIO_PA4, 0);
	if (ret) {
		printk(KERN_ERR "%s: Failed to request gpio_direction_output\n",
		__func__);
                return ret;
	}        
	gpio_set_value(TEGRA_GPIO_PA4, 0);
	/*msleep(3); */
	return 0;
}

static int cyttsp_init(int on)
{
	int ret;
	if (on) {
#if 1                
		ret = gpio_request(TEGRA_GPIO_PG0, "CYTTSP IRQ GPIO");
		if (ret) {
			printk(KERN_ERR "%s: Failed to request GPIO %d\n",
			       __func__, TEGRA_GPIO_PG0);
			return ret;
		}
#endif
		tegra_gpio_enable(TEGRA_GPIO_PG0);
		gpio_direction_input(TEGRA_GPIO_PG0);

#if 1
        	ret = gpio_request(TEGRA_GPIO_PA4, "CYTTSP POWER GPIO");
        	if (ret) {
        		printk(KERN_ERR "%s: Failed to request GPIO %d\n",
        		       __func__, TEGRA_GPIO_PA4);
        		return ret;
        	}
#endif
		tegra_gpio_enable(TEGRA_GPIO_PA4);
		gpio_direction_output(TEGRA_GPIO_PA4, 1);

	} else {
		gpio_free(TEGRA_GPIO_PG0);
		gpio_free(TEGRA_GPIO_PA4);
	}
	return 0;
}


static struct cyttsp_platform_data cypress_i2c_touch_data = {
	.wakeup = cyttsp_wakeup,
	.poweroff = cyttsp_poweroff,
	.init = cyttsp_init,
//	.mt_sync = input_mt_sync,
	/* TODO: maxx and maxy values should be retrieved from the firmware */
	.maxx = 1279,
	.maxy = 799,
	.flags = 0x04,
//	.gen = CY_GEN4,
//	.use_st = 0,
//	.use_mt = 1,
//	.use_trk_id = 0,
	.use_hndshk = 0,
//	.use_timer = 0,
	.use_sleep = 1,
//	.use_gestures = 0,
//	.use_load_file = 0,
//	.use_force_fw_update = 0,
//	.use_virtual_keys = 0,
	/* activate up to 4 groups
	 * and set active distance
	 */
//	.gest_set = CY_GEST_GRP_NONE | CY_ACT_DIST,
	/* change act_intrvl to customize the Active power state
	 * scanning/processing refresh interval for Operating mode
	 */
	/* activate up to 4 groups
	 * and set active distance
	 */
	.act_dist = CY_GEST_GRP_NONE & CY_ACT_DIST_DFLT,
	.act_intrvl = CY_ACT_INTRVL_DFLT,
	/* change tch_tmout to customize the touch timeout for the
	 * Active power state for Operating mode
	 */
	.tch_tmout = CY_TCH_TMOUT_DFLT,
	/* change lp_intrvl to customize the Low Power power state
	 * scanning/processing refresh interval for Operating mode
	 */
	.lp_intrvl = CY_LP_INTRVL_DFLT,
	.name = CY_I2C_NAME,
	.irq_gpio = TEGRA_GPIO_PG0,
	.bl_keys = dflt_bl_keys,
};

#endif /* TOUCHSCREEN_CYTTSP_I2C */

#ifdef TOUCHSCREEN_CYTTSP_I2C
static const struct i2c_board_info nbx03_i2c_bus1_touch_info[] = {
	{
		I2C_BOARD_INFO(CY_I2C_NAME, 0x67),
		.platform_data = &cypress_i2c_touch_data,
	},
};

static int __init nbx03_touch_init(void)
{
	i2c_register_board_info(1, nbx03_i2c_bus1_touch_info, ARRAY_SIZE(nbx03_i2c_bus1_touch_info));

	return 0;
}
#endif

static struct wm8903_platform_data nbx03_wm8903_pdata = {
	.irq_active_low = 0,
	.micdet_cfg = 0x83,
	.micdet_delay = 0,
	.gpio_base = NBX03_GPIO_WM8903(0),
	.gpio_cfg = {
		(WM8903_GPn_FN_MICBIAS_SHORT_DETECT << WM8903_GP1_FN_SHIFT),
		WM8903_GPIO_NO_CONFIG,
		0,                     /* as output pin */
		(WM8903_GPn_FN_MICBIAS_CURRENT_DETECT << WM8903_GP4_FN_SHIFT),
		WM8903_GPIO_NO_CONFIG,
	},
};

static struct i2c_board_info __initdata wm8903_board_info = {
	I2C_BOARD_INFO("wm8903", 0x1a),
	.platform_data = &nbx03_wm8903_pdata,
};

static void nbx03_i2c_init(void)
{
	tegra_i2c_device1.dev.platform_data = &nbx03_i2c1_platform_data;
	tegra_i2c_device2.dev.platform_data = &nbx03_i2c2_platform_data;
	tegra_i2c_device3.dev.platform_data = &nbx03_i2c3_platform_data;
	tegra_i2c_device4.dev.platform_data = &nbx03_dvc_platform_data;

	platform_device_register(&tegra_i2c_device1);
	platform_device_register(&tegra_i2c_device2);
	platform_device_register(&tegra_i2c_device3);
	platform_device_register(&tegra_i2c_device4);

	i2c_register_board_info(0, &wm8903_board_info, 1);
}

static struct platform_device *nbx03_uart_devices[] __initdata = {
	&tegra_uarta_device,
	&tegra_uartb_device,
	&tegra_uartc_device,
	&tegra_uartd_device,
};

static struct uart_clk_parent uart_parent_clk_default[] = {
	[0] = {.name = "clk_m"},
};

static struct uart_clk_parent uart_parent_clk_bluetooth[] = {
	[0] = {.name = "pll_m"},
};

static struct tegra_uart_platform_data nbx03_uart_pdata_default;
static struct tegra_uart_platform_data nbx03_uart_pdata_bluetooth;

static void __init uart_debug_init(void)
{
	unsigned long rate;
	struct clk *c;

	/* UARTA is the debug port. */
	pr_info("Selecting UARTA as the debug console\n");
	nbx03_uart_devices[0] = &debug_uarta_device;
	debug_uart_port_base = ((struct plat_serial8250_port *)(
			debug_uarta_device.dev.platform_data))->mapbase;
	debug_uart_clk = clk_get_sys("serial8250.0", "uarta");

	/* Clock enable for the debug channel */
	if (!IS_ERR_OR_NULL(debug_uart_clk)) {
		rate = ((struct plat_serial8250_port *)(
			debug_uarta_device.dev.platform_data))->uartclk;
		pr_info("The debug console clock name is %s\n",
						debug_uart_clk->name);
		c = tegra_get_clock_by_name("pll_p");
		if (IS_ERR_OR_NULL(c))
			pr_err("Not getting the parent clock pll_p\n");
		else
			clk_set_parent(debug_uart_clk, c);

		clk_enable(debug_uart_clk);
		clk_set_rate(debug_uart_clk, rate);
	} else {
		pr_err("Not getting the clock %s for debug console\n",
					debug_uart_clk->name);
	}
}

static void __init nbx03_uart_init_fixclocklist(struct uart_clk_parent* clklist, int num)
{
	int i;
	struct clk *c;

	for (i = 0; i < num; ++i) {
		c = tegra_get_clock_by_name(clklist[i].name);
		if (IS_ERR_OR_NULL(c)) {
			pr_err("Not able to get the clock for %s\n",
				clklist[i].name);
			continue;
		}
		clklist[i].parent_clk = c;
		clklist[i].fixed_clk_rate = clk_get_rate(c);
	}
}

static void __init nbx03_uart_init(void)
{
	nbx03_uart_init_fixclocklist(uart_parent_clk_default, ARRAY_SIZE(uart_parent_clk_default));
	nbx03_uart_pdata_default.parent_clk_list = uart_parent_clk_default;
	nbx03_uart_pdata_default.parent_clk_count = ARRAY_SIZE(uart_parent_clk_default);
	tegra_uarta_device.dev.platform_data = &nbx03_uart_pdata_default;
	tegra_uartb_device.dev.platform_data = &nbx03_uart_pdata_default;
	tegra_uartd_device.dev.platform_data = &nbx03_uart_pdata_default;

	nbx03_uart_init_fixclocklist(uart_parent_clk_bluetooth, ARRAY_SIZE(uart_parent_clk_bluetooth));
	nbx03_uart_pdata_bluetooth.parent_clk_list = uart_parent_clk_bluetooth;
	nbx03_uart_pdata_bluetooth.parent_clk_count = ARRAY_SIZE(uart_parent_clk_bluetooth);
	tegra_uartc_device.dev.platform_data = &nbx03_uart_pdata_bluetooth;

	/* Register low speed only if it is selected */
	if (!is_tegra_debug_uartport_hs())
		uart_debug_init();

	platform_add_devices(nbx03_uart_devices,
				ARRAY_SIZE(nbx03_uart_devices));
}

static struct platform_device tegra_camera = {
	.name = "tegra_camera",
	.id = -1,
};

static struct tegra_wm8903_platform_data nbx03_audio_pdata = {
	.gpio_spkr_en		= TEGRA_GPIO_SPKR_EN,
	.gpio_hp_det		= TEGRA_GPIO_HP_DET,
	.gpio_cdc_irq		= TEGRA_GPIO_CDC_IRQ,
	.gpio_cdc_shrt		= TEGRA_GPIO_CDC_SHRT,
	.gpio_hp_mute		= -1,
	.gpio_int_mic_en	= -1, //TEGRA_GPIO_INT_MIC_EN,
	.gpio_ext_mic_en	= -1, //TEGRA_GPIO_EXT_MIC_EN,
	.echo_canceler_sleep    = a1026_suspend_command,
	.echo_canceler_wake     = a1026_resume_command,
};

static struct platform_device nbx03_audio_device = {
	.name	= "tegra-snd-wm8903",
	.id	= 0,
	.dev	= {
		.platform_data  = &nbx03_audio_pdata,
	},
};

static struct hci_event_platform_data hci_event_platform = {
	.ocdet_pin = TEGRA_GPIO_PO1, /* USB_OC */
	.pwren_pin = TEGRA_GPIO_PD0, /* PWR_EN */
};
static struct platform_device hci_event_device = {
	.name = "hci-event",
	.dev = {
	.platform_data = &hci_event_platform,
	},
};

static struct nbx_dock_platform_data nbx_dock_platform = {
	.dockdet_pin = TEGRA_GPIO_PU6, /* DOCK_DET */
};
static struct platform_device nbx_dock_device = {
	.name = "nbx_dock",
	.dev = {
	.platform_data = &nbx_dock_platform,
	},
};

static struct platform_device *nbx03_devices[] __initdata = {
	&tegra_pmu_device,
	&tegra_gart_device,
	&tegra_aes_device,
	&tegra_wdt_device,
	&tegra_avp_device,
	&tegra_camera,
	&tegra_i2s_device1,
	&tegra_i2s_device2,
	&tegra_spdif_device,
	&tegra_das_device,
	&spdif_dit_device,
	&bluetooth_dit_device,
	&tegra_pcm_device,
	&nbx03_audio_device,
	&hci_event_device,
	&nbx_dock_device,
};


static struct usb_phy_plat_data tegra_usb_phy_pdata[] = {
	[0] = {
			.instance = 0,
			.vbus_irq = TPS6586X_INT_BASE + TPS6586X_INT_USB_DET,
			.vbus_gpio = TEGRA_GPIO_PD0,
	},
	[1] = {
			.instance = 1,
			.vbus_gpio = -1,
	},
	[2] = {
			.instance = 2,
			.vbus_gpio = TEGRA_GPIO_PD3,
	},
};


static struct tegra_ehci_platform_data tegra_ehci_pdata[] = {
	[0] = {
			.phy_config = &utmi_phy_config[0],
			.operating_mode = TEGRA_USB_HOST,
			.power_down_on_bus_suspend = 0,
			.default_enable = true,
	},
#if 0
	[1] = {
			.phy_config = &ulpi_phy_config,
			.operating_mode = TEGRA_USB_HOST,
			.power_down_on_bus_suspend = 1,
	},
#endif
	[2] = {
			.phy_config = &utmi_phy_config[1],
			.operating_mode = TEGRA_USB_HOST,
			.power_down_on_bus_suspend = 0,
			.default_enable = true,
	},
};

static struct tegra_otg_platform_data tegra_otg_pdata = {
	.ehci_device = &tegra_ehci1_device,
	.ehci_pdata = &tegra_ehci_pdata[0],
};

//static int __init nbx03_gps_init(void)
//{
//	struct clk *clk32 = clk_get_sys(NULL, "blink");
//	if (!IS_ERR(clk32)) {
//		clk_set_rate(clk32,clk32->parent->rate);
//		clk_enable(clk32);
//	}
//
//	tegra_gpio_enable(TEGRA_GPIO_PZ3);
//	return 0;
//}

static void nbx03_power_off(void)
{
	int ret;

	ret = tps6586x_power_off();
	if (ret)
		pr_err("nbx03: failed to power off\n");

	while(1);
}

static void __init nbx03_power_off_init(void)
{
	pm_power_off = nbx03_power_off;
}

static void nbx03_usb_init(void)
{
	char *src = NULL;

	tegra_usb_phy_init(tegra_usb_phy_pdata, ARRAY_SIZE(tegra_usb_phy_pdata));
	/* OTG should be the first to be registered */
	tegra_otg_device.dev.platform_data = &tegra_otg_pdata;
	platform_device_register(&tegra_otg_device);

	platform_device_register(&tegra_udc_device);
	//platform_device_register(&tegra_ehci2_device);

	tegra_ehci3_device.dev.platform_data=&tegra_ehci_pdata[2];
	platform_device_register(&tegra_ehci3_device);
}

static struct resource mbm_wow_resources[] = {
    {
        .start = TEGRA_GPIO_TO_IRQ(TEGRA_GPIO_PN7),
        .end   = TEGRA_GPIO_TO_IRQ(TEGRA_GPIO_PN7),
        .flags = IORESOURCE_IRQ,
    },
};

static struct platform_device mbm_wow_device = {
    .name          = "mbm_wow",
    .id            = -1,
    .resource      = mbm_wow_resources,
    .num_resources = ARRAY_SIZE(mbm_wow_resources),
};

static void __init nbx03_wwan_init(void)
{
	wireless_power_control(WPC_MODULE_WWAN, 1);
	msleep(10);
	wireless_power_control(WPC_MODULE_WWAN_RF, 1);

	platform_device_register(&mbm_wow_device);
}

static struct a1026_platform_data a1026_pdata = {
	.reset_pin = TEGRA_GPIO_PU1,
	.wake_pin = TEGRA_GPIO_PU0,
	.clk = "cdev2",
	.is_awake = 0,
};

static const struct i2c_board_info i2c_a1026_info[] = {
	{
		I2C_BOARD_INFO("a1026", 0x3e),
		.platform_data = &a1026_pdata,
	},
};

struct tap_delay_initdata {
	const char* clkname;
	int tap_delay;
};

static const struct tap_delay_initdata nbx03_tap_delay[] = {
	/* wifi */
	{
		.clkname = "sdhci-tegra.0",
		.tap_delay = 8,
	},
	/* sdcard */
	{
		.clkname = "sdhci-tegra.2",
		.tap_delay = 4,
	},
	/* emmc */
	{
		.clkname = "sdhci-tegra.3",
		.tap_delay = 6,
	},
};

static void nbx03_set_tap_delay(void)
{
	int i;

	for ( i = 0 ; i < ARRAY_SIZE(nbx03_tap_delay) ; ++i ) {
		const char *clkname = nbx03_tap_delay[i].clkname;
		struct clk *clk;
		clk = clk_get_sys(clkname, NULL);
		if (!IS_ERR(clk)) {
			tegra_sdmmc_tap_delay(clk, nbx03_tap_delay[i].tap_delay);
			clk_put(clk);
		} else {
			pr_err("Failed to set sdhci tap delay for %s\n", clkname);
		}
	}
}

static void __init tegra_nbx03_init(void)
{
	pr_info("board-nbx03: odmdata=%08x\n", nbx_get_odmdata());

	nbx03_set_tap_delay();
	tegra_clk_init_from_table(nbx03_clk_init_table);
	nbx03_pinmux_init();
	nbx03_i2c_init();
	nbx03_uart_init();
	//tegra_ehci2_device.dev.platform_data
	//	= &nbx03_ehci2_ulpi_platform_data;
	platform_add_devices(nbx03_devices, ARRAY_SIZE(nbx03_devices));

	nbx03_sdhci_init();
	//nbx03_charge_init();
	nbx03_regulator_init();
	//nbx03_charger_init();

#ifdef TOUCHSCREEN_CYTTSP_I2C
	pr_info("Initializing cypress touch driver\n");
	nbx03_touch_init();
#endif

	nbx03_kbc_init();

	nbx03_usb_init();
	//nbx03_gps_init();
	nbx03_panel_init();
	nbx03_sensors_init();
	//nbx03_bt_rfkill();
	nbx03_power_off_init();
	nbx03_emc_init();
#ifdef CONFIG_BT_BLUESLEEP
        tegra_setup_bluesleep();
#endif
	nbx03_wwan_init();

	i2c_register_board_info(0, i2c_a1026_info, 
				ARRAY_SIZE(i2c_a1026_info));

	nbx_ecipc_irq_setup(TEGRA_GPIO_PJ7, IRQF_TRIGGER_LOW);
}

int __init tegra_nbx03_protected_aperture_init(void)
{
	tegra_protected_aperture_init(tegra_grhost_aperture);
	return 0;
}
late_initcall(tegra_nbx03_protected_aperture_init);

#ifdef CONFIG_NBX_EC_IPC

static struct nbx_ec_ipc_device nbx03_ec_ipc_core_device = {
	.name = "nbx_ec_ipc",
};
static struct nbx_ec_ipc_device nbx03_ec_ipc_suspend_device = {
	.name = "ec_ipc_suspend",
};
static struct nbx_ec_ipc_device nbx03_ec_ipc_acplug_device = {
	.name = "ec_ipc_acplug",
};
static struct nbx_ec_ipc_device nbx03_powerkey_device = {
	.name = "nbx_powerkey",
};
static struct nbx_ec_ipc_device nbx03_led_device = {
	.name = "nbx_led",
};

static struct nbx_ec_battery_platform_data nbx_ec_battery_pdata = {
	.degradation = 1,
};
static struct nbx_ec_ipc_device nbx03_battery_device = {
	.name = "nbx_battery",
	.dev = {
		.platform_data = &nbx_ec_battery_pdata,
	},
};
static struct nbx_ec_ipc_device nbx03_ec_ipc_light_device = {
	.name = "bh1690",
};

int __init nbx03_ec_devices_init(void)
{
	int ret;

	/* must later than register tegra_uartd_device/driver */
	nbx03_ec_ipc_core_device.dev.parent = &(tegra_uartd_device.dev);
	ret = nbx_ec_ipc_core_device_register(&nbx03_ec_ipc_core_device);
	if (ret < 0) {
		return ret;
	}
	ret = nbx_ec_ipc_device_register(&nbx03_ec_ipc_suspend_device);
	if (ret < 0) {
		return ret;
	}

	ret = nbx_ec_ipc_device_register(&nbx03_ec_ipc_acplug_device);
	if (ret < 0) {
		return ret;
	}

	ret = nbx_ec_ipc_device_register(&nbx03_powerkey_device);
	if (ret < 0) {
		return ret;
	}

	ret = nbx_ec_ipc_device_register(&nbx03_led_device);
	if (ret < 0) {
		return ret;
	}

	ret = nbx_ec_ipc_device_register(&nbx03_battery_device);
	if (ret < 0) {
		return ret;
	}

	ret = nbx_ec_ipc_device_register(&nbx03_ec_ipc_light_device);
	if (ret < 0) {
		return ret;
	}

	return 0;
}
late_initcall(nbx03_ec_devices_init);

#endif /* CONFIG_NBX_EC_IPC */

#define ODMDATA_MEMSIZE_MASK 0x000000c0
void __init tegra_nbx03_reserve(void)
{
	unsigned long cmdline_memsize, odmdata_memsize;

	if (memblock_reserve(0x0, 4096) < 0)
		pr_warn("Cannot reserve first 4K of memory for safety\n");

	cmdline_memsize = memblock_end_of_DRAM();
	odmdata_memsize = (nbx_get_odmdata() & ODMDATA_MEMSIZE_MASK) ? SZ_1G : SZ_512M;
	if (cmdline_memsize != odmdata_memsize) {
		pr_warn("Memory size mismatch between cmdline and odmdata\n");
		pr_warn(" cmdline: 0x%08lx, odmdata: 0x%08lx\n", cmdline_memsize, odmdata_memsize);
	}

	if (memblock_remove(SZ_512M - SZ_2M, SZ_2M) < 0)
		pr_warn("Cannot reserve memory\n");

	/* FIXME: carveout_size, fb_size, fb2_size(for 2nd display)
	 * carveout_size: minimum=128M, with flash=160M, recommendation=256M
	 * refer to NvBug#786465 */
	if (min(cmdline_memsize, odmdata_memsize) == SZ_512M)
		tegra_reserve(SZ_128M, SZ_8M + SZ_1M, 0);
	else
		tegra_reserve(SZ_256M, SZ_8M + SZ_1M, 0);
}

MACHINE_START(NBX03, "nbx03")
	.boot_params    = 0x00000100,
	.map_io         = tegra_map_common_io,
	.reserve        = tegra_nbx03_reserve,
	.init_early	= tegra_init_early,
	.init_irq       = tegra_init_irq,
	.timer          = &tegra_timer,
	.init_machine   = tegra_nbx03_init,
MACHINE_END
