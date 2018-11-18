/* 2012-04-12: File added and changed by Sony Corporation */
/*
 * arch/arm/mach-tegra/board-nbx02.c
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
#include <mach/a1026.h>
#include <mach/tegra_odm_fuses.h>

#include "board.h"
#include "clock.h"
#include "board-nbx02.h"
#include "devices.h"
#include "gpio-names.h"
#include "fuse.h"
#include "board-nbx-common.h"
#include "wireless_power_control.h"
#include "pm.h"
#include "pm-irq.h"

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

static __initdata struct tegra_clk_init_table nbx02_clk_init_table[] = {
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
	{ "spdif_out",	"pll_a_out0",	5644800,	false},
	{ "kbc",	"clk_32k",	32768,		true},
	{ NULL,		NULL,		0,		0},
};

//static struct tegra_ulpi_config ventana_ehci2_ulpi_phy_config = {
//	.reset_gpio = TEGRA_GPIO_PV1,
//	.clk = "cdev2",
//};

//static struct tegra_ehci_platform_data ventana_ehci2_ulpi_platform_data = {
//	.operating_mode = TEGRA_USB_HOST,
//	.power_down_on_bus_suspend = 1,
//	.phy_config = &ventana_ehci2_ulpi_phy_config,
//	.phy_type = TEGRA_USB_PHY_TYPE_LINK_ULPI,
 //};

static const struct tegra_i2c_clk_table nbx02_i2c1_clk_table[] = {
	{
		.addr = 0x3e,
		.clk_rate = 100000,
	},
};

static struct tegra_i2c_platform_data nbx02_i2c1_platform_data = {
	.adapter_nr	= 0,
	.bus_count	= 1,
	.bus_clk_rate	= { 400000, 0 },
	.slave_addr = 0x00FC,
	.clk_table = nbx02_i2c1_clk_table,
	.table_size = ARRAY_SIZE(nbx02_i2c1_clk_table),
};

static struct tegra_i2c_platform_data nbx02_i2c2_platform_data = {
	.adapter_nr	= 1,
	.bus_count	= 1,
	.bus_clk_rate	= { 400000, 0 },
	.slave_addr = 0x00FC,
};

static struct tegra_i2c_platform_data nbx02_i2c3_platform_data = {
	.adapter_nr	= 2,
	.bus_count	= 1,
	.bus_clk_rate	= { 400000, 0 },
	.slave_addr = 0x00FC,
};

static struct tegra_i2c_platform_data nbx02_dvc_platform_data = {
	.adapter_nr	= 3,
	.bus_count	= 1,
	.bus_clk_rate	= { 400000, 0 },
	.is_dvc		= true,
};

static struct wm8903_platform_data nbx02_wm8903_pdata = {
	.irq_active_low = 0,
	.micdet_cfg = 0x83,
	.micdet_delay = 0,
	.gpio_base = NBX02_GPIO_WM8903(0),
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
	.platform_data = &nbx02_wm8903_pdata,
};

static void nbx02_i2c_init(void)
{
	tegra_i2c_device1.dev.platform_data = &nbx02_i2c1_platform_data;
	tegra_i2c_device2.dev.platform_data = &nbx02_i2c2_platform_data;
	tegra_i2c_device3.dev.platform_data = &nbx02_i2c3_platform_data;
	tegra_i2c_device4.dev.platform_data = &nbx02_dvc_platform_data;

	platform_device_register(&tegra_i2c_device1);
	platform_device_register(&tegra_i2c_device2);
	platform_device_register(&tegra_i2c_device3);
	platform_device_register(&tegra_i2c_device4);

	i2c_register_board_info(0, &wm8903_board_info, 1);
}

static struct platform_device *nbx02_uart_devices[] __initdata = {
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

static struct tegra_uart_platform_data nbx02_uart_pdata_default;
static struct tegra_uart_platform_data nbx02_uart_pdata_bluetooth;

static void __init uart_debug_init(void)
{
	unsigned long rate;
	struct clk *c;

	/* UARTA is the debug port. */
	pr_info("Selecting UARTA as the debug console\n");
	nbx02_uart_devices[0] = &debug_uarta_device;
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

static void __init nbx02_uart_init_fixclocklist(struct uart_clk_parent* clklist, int num)
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

static void __init nbx02_uart_init(void)
{
	nbx02_uart_init_fixclocklist(uart_parent_clk_default, ARRAY_SIZE(uart_parent_clk_default));
	nbx02_uart_pdata_default.parent_clk_list = uart_parent_clk_default;
	nbx02_uart_pdata_default.parent_clk_count = ARRAY_SIZE(uart_parent_clk_default);
	tegra_uarta_device.dev.platform_data = &nbx02_uart_pdata_default;
	tegra_uartb_device.dev.platform_data = &nbx02_uart_pdata_default;
	tegra_uartd_device.dev.platform_data = &nbx02_uart_pdata_default;

	nbx02_uart_init_fixclocklist(uart_parent_clk_bluetooth, ARRAY_SIZE(uart_parent_clk_bluetooth));
	nbx02_uart_pdata_bluetooth.parent_clk_list = uart_parent_clk_bluetooth;
	nbx02_uart_pdata_bluetooth.parent_clk_count = ARRAY_SIZE(uart_parent_clk_bluetooth);
	tegra_uartc_device.dev.platform_data = &nbx02_uart_pdata_bluetooth;

	/* Register low speed only if it is selected */
	if (!is_tegra_debug_uartport_hs())
		uart_debug_init();

	platform_add_devices(nbx02_uart_devices,
				ARRAY_SIZE(nbx02_uart_devices));
}

static struct platform_device tegra_camera = {
	.name = "tegra_camera",
	.id = -1,
};

static struct tegra_wm8903_platform_data nbx02_audio_pdata = {
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

static struct platform_device nbx02_audio_device = {
	.name	= "tegra-snd-wm8903",
	.id	= 0,
	.dev	= {
		.platform_data  = &nbx02_audio_pdata,
	},
};

static struct platform_device *nbx02_devices[] __initdata = {
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
	&nbx02_audio_device,
};

#if defined (CONFIG_TOUCHSCREEN_TMA300) || (CONFIG_TOUCHSCREEN_TMA300_MODULE)
#include <linux/input/nbx_touch.h>

static void touch_gpio_enable(int gpio_num){
	tegra_gpio_enable(gpio_num);
	return;
}

static void touch_pinmux_set_pullupdown(int group, int pullupdown){
	tegra_pinmux_set_pullupdown(group, pullupdown);
	return;
}

static struct nbx_touch_platform_data nbx02_i2c_touch_info = {

	.gpio_enable = touch_gpio_enable,
	.pinmux_set_pullupdown = touch_pinmux_set_pullupdown,
};

static const struct i2c_board_info nbx02_i2c_bus1_touch_info[] = {
	{
		I2C_BOARD_INFO("i2c_nbx_touch", 0x1b),
		.platform_data = &nbx02_i2c_touch_info,
	},
	{
		I2C_BOARD_INFO("i2c_nbx_touch", 0x3b),
		.platform_data = &nbx02_i2c_touch_info,
	},
};

static int __init nbx02_touch_init_tma300(void)
{
	i2c_register_board_info(1, nbx02_i2c_bus1_touch_info, ARRAY_SIZE(nbx02_i2c_bus1_touch_info));

	return 0;
}
#endif

static struct usb_phy_plat_data tegra_usb_phy_pdata[] = {
	[0] = {
			.instance = 0,
			.vbus_gpio = -1,
	},
	[1] = {
			.instance = 1,
			.vbus_gpio = -1,
	},
	[2] = {
			.instance = 2,
			.vbus_gpio = -1,
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

//static int __init nbx02_gps_init(void)
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

static void nbx02_power_off(void)
{
	int ret;

	ret = tps6586x_power_off();
	if (ret)
		pr_err("nbx02: failed to power off\n");

	while(1);
}

static void __init nbx02_power_off_init(void)
{
	pm_power_off = nbx02_power_off;
}

static void nbx02_usb_init(void)
{
	tegra_usb_phy_init(tegra_usb_phy_pdata, ARRAY_SIZE(tegra_usb_phy_pdata));
	/* OTG should be the first to be registered */
	tegra_otg_device.dev.platform_data = &tegra_otg_pdata;
	platform_device_register(&tegra_otg_device);
	tegra_pm_irq_set_wake_type(INT_USB, IRQF_TRIGGER_RISING);

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

static void __init nbx02_wwan_init(void)
{
	int ret = 0;
	u32 odm_rsvd[8];
	int disable_wwan;
	int DISABLE_WWAN_BIT = 28;
	
	ret = tegra_fuse_read(ODM_RSVD, odm_rsvd, sizeof(odm_rsvd));
	
	if(ret == 0) {
		disable_wwan = (odm_rsvd[7] >> DISABLE_WWAN_BIT) & 0x01;
		
		if(disable_wwan == 0) {
			wireless_power_control(WPC_MODULE_WWAN, 1);
			msleep(10);
			wireless_power_control(WPC_MODULE_WWAN_RF, 1);
			
			platform_device_register(&mbm_wow_device);
		}
	}
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

static const struct tap_delay_initdata nbx02_tap_delay[] = {
	/* wifi */
	{
		.clkname = "sdhci-tegra.0",
		.tap_delay = 8,
	},
	/* sdcard */
	{
		.clkname = "sdhci-tegra.2",
		.tap_delay = 5,
	},
	/* emmc */
	{
		.clkname = "sdhci-tegra.3",
		.tap_delay = 5,
	},
};

static void nbx02_set_tap_delay(void)
{
	int i;

	for ( i = 0 ; i < ARRAY_SIZE(nbx02_tap_delay) ; ++i ) {
		const char *clkname = nbx02_tap_delay[i].clkname;
		struct clk *clk;
		clk = clk_get_sys(clkname, NULL);
		if (!IS_ERR(clk)) {
			tegra_sdmmc_tap_delay(clk, nbx02_tap_delay[i].tap_delay);
			clk_put(clk);
		} else {
			pr_err("Failed to set sdhci tap delay for %s\n", clkname);
		}
	}
}

static void __init tegra_nbx02_init(void)
{
	pr_info("board-nbx02.c: nbx_odmdata=%08x\n", nbx_get_odmdata());

	nbx02_set_tap_delay();
	tegra_clk_init_from_table(nbx02_clk_init_table);
	nbx02_pinmux_init();
	nbx02_i2c_init();
	nbx02_uart_init();
	//tegra_ehci2_device.dev.platform_data
	//	= &nbx02_ehci2_ulpi_platform_data;
	platform_add_devices(nbx02_devices, ARRAY_SIZE(nbx02_devices));

	nbx02_sdhci_init();
	//nbx02_charge_init();
	nbx02_regulator_init();
	//nbx02_charger_init();

#if defined (CONFIG_TOUCHSCREEN_TMA300) || (CONFIG_TOUCHSCREEN_TMA300_MODULE)
	pr_info("Initializing TMA300 touch driver\n");
	nbx02_touch_init_tma300();
#endif

	nbx02_kbc_init();

	nbx02_usb_init();
	//nbx02_gps_init();
	nbx02_panel_init();
	nbx02_sensors_init();
	//nbx02_bt_rfkill();
	nbx02_power_off_init();
	nbx02_emc_init();
#ifdef CONFIG_BT_BLUESLEEP
        tegra_setup_bluesleep();
#endif
	nbx02_wwan_init();

	i2c_register_board_info(0, i2c_a1026_info, 
				ARRAY_SIZE(i2c_a1026_info));

	nbx_ecipc_irq_setup(TEGRA_GPIO_PJ7, IRQF_TRIGGER_LOW);
}

int __init tegra_nbx02_protected_aperture_init(void)
{
	tegra_protected_aperture_init(tegra_grhost_aperture);
	return 0;
}
late_initcall(tegra_nbx02_protected_aperture_init);

#ifdef CONFIG_NBX_EC_IPC

static struct nbx_ec_ipc_device nbx02_ec_ipc_core_device = {
	.name = "nbx_ec_ipc",
};
static struct nbx_ec_ipc_device nbx02_ec_ipc_suspend_device = {
	.name = "ec_ipc_suspend",
};
static struct nbx_ec_ipc_device nbx02_ec_ipc_acplug_device = {
	.name = "ec_ipc_acplug",
};
static struct nbx_ec_ipc_device nbx02_ec_ipc_lidsw_device = {
	.name = "ec_ipc_lidsw",
};
static struct nbx_ec_ipc_device nbx02_lidsw_device = {
	.name = "nbx_lidsw",
};
static struct nbx_ec_ipc_device nbx02_powerkey_device = {
	.name = "nbx_powerkey",
};
static struct nbx_ec_ipc_device nbx02_led_device = {
	.name = "nbx_led",
};

static struct nbx_ec_battery_platform_data nbx_ec_battery_pdata = {
	.degradation = 0,
};
static struct nbx_ec_ipc_device nbx02_battery_device = {
	.name = "nbx_battery",
	.dev = {
		.platform_data = &nbx_ec_battery_pdata,
	},
};
static struct nbx_ec_ipc_device nbx02_ec_ipc_light_device = {
	.name = "bh1690",
};

int __init nbx02_ec_devices_init(void)
{
	int ret;

	/* must later than register tegra_uartd_device/driver */
	nbx02_ec_ipc_core_device.dev.parent = &(tegra_uartd_device.dev);
	ret = nbx_ec_ipc_core_device_register(&nbx02_ec_ipc_core_device);
	if (ret < 0) {
		return ret;
	}
	ret = nbx_ec_ipc_device_register(&nbx02_ec_ipc_suspend_device);
	if (ret < 0) {
		return ret;
	}

	ret = nbx_ec_ipc_device_register(&nbx02_ec_ipc_acplug_device);
	if (ret < 0) {
		return ret;
	}

	ret = nbx_ec_ipc_device_register(&nbx02_ec_ipc_lidsw_device);
	if (ret < 0) {
		return ret;
	}
	ret = nbx_ec_ipc_device_register(&nbx02_lidsw_device);
	if (ret < 0) {
		return ret;
	}

	ret = nbx_ec_ipc_device_register(&nbx02_powerkey_device);
	if (ret < 0) {
		return ret;
	}

	ret = nbx_ec_ipc_device_register(&nbx02_led_device);
	if (ret < 0) {
		return ret;
	}

	ret = nbx_ec_ipc_device_register(&nbx02_battery_device);
	if (ret < 0) {
		return ret;
	}

	ret = nbx_ec_ipc_device_register(&nbx02_ec_ipc_light_device);
	if (ret < 0) {
		return ret;
	}

	return 0;
}
late_initcall(nbx02_ec_devices_init);

#endif /* CONFIG_NBX_EC_IPC */

#define ODMDATA_MEMSIZE_MASK 0x000000c0
void __init tegra_nbx02_reserve(void)
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

MACHINE_START(NBX02, "nbx02")
	.boot_params    = 0x00000100,
	.map_io         = tegra_map_common_io,
	.reserve        = tegra_nbx02_reserve,
	.init_early	= tegra_init_early,
	.init_irq       = tegra_init_irq,
	.timer          = &tegra_timer,
	.init_machine   = tegra_nbx02_init,
MACHINE_END
