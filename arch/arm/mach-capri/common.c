/************************************************************************************************/
/*																							  */
/*  Copyright 2010  Broadcom Corporation														*/
/*																							  */
/*	 Unless you and Broadcom execute a separate written software license agreement governing  */
/*	 use of this software, this software is licensed to you under the terms of the GNU		*/
/*	 General Public License version 2 (the GPL), available at								 */
/*																							  */
/*		  http://www.broadcom.com/licenses/GPLv2.php										  */
/*																							  */
/*	 with the following added to such license:												*/
/*																							  */
/*	 As a special exception, the copyright holders of this software give you permission to	*/
/*	 link this software with independent modules, and to copy and distribute the resulting	*/
/*	 executable under terms of your choice, provided that you also meet, for each linked	  */
/*	 independent module, the terms and conditions of the license of that module.			  */
/*	 An independent module is a module which is not derived from this software.  The special  */
/*	 exception does not apply to any modifications of the software.						   */
/*																							  */
/*	 Notwithstanding the above, under no circumstances may you combine this software in any   */
/*	 way with any other Broadcom software provided under a license other than the GPL,		*/
/*	 without Broadcom's express prior written consent.										*/
/*																							  */
/************************************************************************************************/
#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <mach/clock.h>
#include <linux/serial.h>
#include <linux/serial_core.h>
#include <linux/serial_reg.h>
#include <linux/serial_8250.h>
#include <linux/clk.h>
#include <linux/pwm_backlight.h>
#include <asm/mach-types.h>
#include <mach/hardware.h>
#include <mach/kona.h>
#include <mach/rdb/brcm_rdb_uartb.h>
#include <mach/irqs.h>
#include <plat/chal/chal_trace.h>
#include <trace/stm.h>
#include <asm/pmu.h>
#include <plat/pl330-pdata.h>
#include <linux/dma-mapping.h>
#include <linux/spi/spi.h>
#include <plat/spi_kona.h>
#include <plat/chal/chal_trace.h>
#include <trace/stm.h>
#ifdef CONFIG_KONA_AVS
#include <plat/kona_avs.h>
#include "capri_avs.h"
#endif


#if defined(CONFIG_KONA_CPU_FREQ_DRV)
#include <plat/kona_cpufreq_drv.h>
#include <linux/cpufreq.h>
#include <mach/clock.h>
#include <linux/clk.h>
#include <mach/pi_mgr.h>
#endif

#ifdef CONFIG_KONA_POWER_MGR
#include <plat/pwr_mgr.h>

#define VLT_LUT_SIZE 16
#endif

#ifdef CONFIG_SENSORS_KONA
#include <linux/broadcom/kona-thermal.h>
#include <linux/csapi_adc.h>
#include <linux/mfd/bcmpmu.h>
#endif

#ifdef CONFIG_WD_TAPPER
#include <linux/broadcom/wd-tapper.h>
#endif

#define KONA_UART0_PA   UARTB_BASE_ADDR
#define KONA_UART1_PA   UARTB2_BASE_ADDR
#define KONA_UART2_PA   UARTB3_BASE_ADDR
#define KONA_UART3_PA   UARTB4_BASE_ADDR

#define KONA_8250PORT(name, clk, freq, private_index) \
{ \
	.membase = (void __iomem *)(KONA_##name##_VA), \
	.mapbase = (resource_size_t)(KONA_##name##_PA), \
	.irq = BCM_INT_ID_##name, \
	.uartclk = freq, \
	.regshift = 2, \
	.iotype = UPIO_MEM32, \
	.type = PORT_KONA, \
	.flags = UPF_BOOT_AUTOCONF | UPF_FIXED_TYPE | UPF_SKIP_TEST, \
	.clk_name = clk, \
	.pm = capri_8250_pm, \
	.private_data = (void *) &capri_uart_private_data[private_index], \
	.serial_out = capri_serial_out, \
	.serial_in = capri_serial_in, \
	.handle_irq = capri_serial_handle_irq, \
}

struct capri_uart_data {
	int last_lcr;
	char *peri_clk;
};

static struct capri_uart_data capri_uart_private_data[] = {
	[0] = {
		.last_lcr = 0,
		.peri_clk = "uartb_clk",
	},
	[1] = {
		.last_lcr = 0,
		.peri_clk = "uartb2_clk",
	},
	[2] = {
		.last_lcr = 0,
		.peri_clk = "uartb3_clk",
	},
	[3] = {
		.last_lcr = 0,
		.peri_clk = "uartb4_clk",
	},
};

static void capri_8250_pm(struct uart_port *port, unsigned int state,
	unsigned int old_state)
{
	struct clk *clk;

	printk(KERN_INFO "In %s port = 0x%08X state = %d old_state = %d\n",
		__func__, (unsigned int) port, state, old_state);

	clk = clk_get(port->dev,
		((struct capri_uart_data *) port->private_data)->peri_clk);

	switch (state) {
	case 0:
		clk_enable(clk);
		serial8250_do_pm(port, state, old_state);
		break;
	case 3:
		clk_disable(clk);
		serial8250_do_pm(port, state, old_state);
		break;
	default:
		serial8250_do_pm(port, state, old_state);
		break;
	}

	clk_put(clk);
}


static void capri_serial_out(struct uart_port *p, int offset, int value)
{
	struct capri_uart_data *d = p->private_data;

	if (offset == UART_LCR)
		d->last_lcr = value;

	offset <<= p->regshift;
	writel(value, p->membase + offset);
}

static unsigned int capri_serial_in(struct uart_port *p, int offset)
{
	offset <<= p->regshift;

	return readl(p->membase + offset);
}

/* Offset for the DesignWare's UART Status Register. */
#define UART_USR	0x1f

static int capri_serial_handle_irq(struct uart_port *p)
{
	struct capri_uart_data *d = p->private_data;
	unsigned int iir = p->serial_in(p, UART_IIR);

	if (serial8250_handle_irq(p, iir)) {
		return 1;
	} else if ((iir & UART_IIR_BUSY) == UART_IIR_BUSY) {
		/* Clear the USR and write the LCR again. */
		(void)p->serial_in(p, UART_USR);
		p->serial_out(p, d->last_lcr, UART_LCR);

		return 1;
	}

	return 0;
}

static struct plat_serial8250_port uart_data[] = {
	KONA_8250PORT(UART0, "uartb_clk",  26000000, 0),
	KONA_8250PORT(UART1, "uartb2_clk", 26000000, 1),
#ifndef CONFIG_MACH_CAPRI_FPGA
	KONA_8250PORT(UART2, "uartb3_clk", 48000000, 2),
	KONA_8250PORT(UART3, "uartb4_clk", 26000000, 3),
#endif
	{ .flags = 0, },
};

static struct platform_device board_serial_device = {
	.name = "serial8250",
	.id = PLAT8250_DEV_PLATFORM,
	.dev = {
		.platform_data = uart_data,
	},
};

#ifdef CONFIG_STM_TRACE
static struct stm_platform_data stm_pdata = {
	.regs_phys_base	   = STM_BASE_ADDR,
	.channels_phys_base   = SWSTM_BASE_ADDR,
	.id_mask			  = 0x0,   /* Skip ID check/match */
	.final_funnel		  = CHAL_TRACE_FIN_FUNNEL,
};

struct platform_device kona_stm_device = {
	.name = "stm",
	.id = -1,
	.dev = {
			.platform_data = &stm_pdata,
	},
};
#endif

#if defined(CONFIG_HW_RANDOM_KONA)
static struct resource rng_device_resource[] = {
	[0] = {
		.start = SEC_RNG_BASE_ADDR,
		.end   = SEC_RNG_BASE_ADDR + 0x14,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = BCM_INT_ID_SECURE_TRAP1,
		.end   = BCM_INT_ID_SECURE_TRAP1,
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device rng_device = {
	.name		  = "kona_rng",
	.id			= -1,
	.resource	  = rng_device_resource,
	.num_resources = ARRAY_SIZE(rng_device_resource),
};
#endif

#if defined(CONFIG_KONA_LEDM)
static struct resource ledm_device_resource[] = {
    [0] = {
        .start = LEDM_BASE_ADDR,
        .end   = LEDM_BASE_ADDR + 0x20,
        .flags = IORESOURCE_MEM,
    },
};

static struct platform_device ledm_device =
{
    .name          = "kona_ledm",
    .id            = -1,
    .resource      = ledm_device_resource,
    .num_resources = ARRAY_SIZE(ledm_device_resource),
};
#endif

#if defined(CONFIG_KONA_PWMC)
static struct resource pwm_device_resource[] = {
	[0] = {
		.start = PWM_BASE_ADDR,
		.end   = PWM_BASE_ADDR + 0x10,
		.flags = IORESOURCE_MEM,
	},
};

static struct platform_device pwm_device = {
	.name		  = "kona_pwmc",
	.id			= -1,
	.resource	  = pwm_device_resource,
	.num_resources = ARRAY_SIZE(pwm_device_resource),
};
#endif

#if defined(CONFIG_BCM_OTP)
static struct resource otp_device_resource[] = {
	[0] = {
		.start = SEC_OTP_BASE_ADDR,
		.end   = SEC_OTP_BASE_ADDR + 0x30,
		.flags = IORESOURCE_MEM,
	},
};

static struct platform_device otp_device =
{
	.name          = "kona_otp",
	.id            = -1,
	.resource	  = otp_device_resource,
	.num_resources = ARRAY_SIZE(otp_device_resource),
};
#endif

#if defined(CONFIG_KONA_WATCHDOG)
static struct resource wdt_device_resource[] = {
    [0] = {
        .start = SECWD2_BASE_ADDR,
        .end   = SECWD2_BASE_ADDR + 0x34,
        .flags = IORESOURCE_MEM,
    },
};

static struct platform_device wdt_device =
{
   .name          = "kona_wdt",
   .id            = -1,
   .resource	  = wdt_device_resource,
   .num_resources = ARRAY_SIZE(wdt_device_resource),
   .dev = {
        .platform_data = SEC_WD_APB_BUS_CLK_NAME_STR,
    },
};
#endif

#if defined(CONFIG_BACKLIGHT_PWM)
static struct platform_pwm_backlight_data pwm_backlight_data = {
	.pwm_name	= "kona_pwmc:2",
	.max_brightness	= 255,
	.dft_brightness	= 255,
	.pwm_period_ns	= 55555,
	.polarity = 1,
   .enable_gpio = 7
};

static struct platform_device pwm_backlight_device = {
	.name	 = "pwm-backlight",
	.id	   = -1,
	.dev	  = {
		.platform_data = &pwm_backlight_data,
	},
};
#endif

#if defined(CONFIG_MPCORE_WATCHDOG)
static struct resource wdt_device_resource[] = {
	[0] = {
		.start = PTIM_BASE_ADDR,
		.end   = PTIM_BASE_ADDR + 0x34,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = BCM_INT_ID_PPI14,
		.end   = BCM_INT_ID_PPI14,
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device wdt_device = {
	.name		  = "mpcore_wdt",
	.id			= -1,
	.resource	  = wdt_device_resource,
	.num_resources = ARRAY_SIZE(wdt_device_resource),
	.dev = {
		.platform_data = "arm_periph_clk",
	},
};
#endif

#ifdef CONFIG_WD_TAPPER
static struct wd_tapper_platform_data wd_tapper_data = {
	/* Set the count to the time equivalent to the time-out in milliseconds
	 * required to pet the PMU watchdog to overcome the problem of reset in
	 * suspend*/
	.count = 32000,
	.ch_num = 1,
	.name = "aon-timer",
};

static struct platform_device wd_tapper = {
	.name = "wd_tapper",
	.id = 0,
	.dev = {
		.platform_data = &wd_tapper_data,
		}
};
#endif

#if defined(CONFIG_MTD_BCMNAND)
static struct platform_device nand_device = {
	.name		  = "bcmnand",
	.id			= -1,
};
#endif

#if defined(CONFIG_RTC_DRV_ISLAND)
static struct resource rtc_device_resource[] = {
	[0] = {
		.start = RTC_APB_BASE_ADDR,
		.end   = RTC_APB_BASE_ADDR + 0x24,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = BCM_INT_ID_BBL1,
		.end   = BCM_INT_ID_BBL1,
		.flags = IORESOURCE_IRQ,
	},
	[2] = {
		.start = BCM_INT_ID_BBL2,
		.end   = BCM_INT_ID_BBL2,
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device rtc_device = {
	.name		  = "island-rtc",
	.id			= -1,
	.resource	  = rtc_device_resource,
	.num_resources = ARRAY_SIZE(rtc_device_resource),
	.dev = {
		.platform_data = "bbl_apb_clk",
	},
};
#endif



/* ARM performance monitor unit */
static struct resource pmu_resource = {
		.start = BCM_INT_ID_PMU_IRQ0,
		.end = BCM_INT_ID_PMU_IRQ0,
		.flags = IORESOURCE_IRQ,
};

static struct platform_device pmu_device = {
		.name = "arm-pmu",
		.id   = -1,
		.resource = &pmu_resource,
		.num_resources = 1,
};
/* SPI configuration */
static struct resource kona_sspi_spi0_resource[] = {
	[0] = {
			.start = SSP0_BASE_ADDR,
			.end = SSP0_BASE_ADDR + SZ_4K - 1,
			.flags = IORESOURCE_MEM,
		   },
	[1] = {
			.start = BCM_INT_ID_SSP0,
			.end = BCM_INT_ID_SSP0,
			.flags = IORESOURCE_IRQ,
		   },
};

static struct spi_kona_platform_data sspi_spi0_info = {
	.enable_dma = 1,
	.cs_line = 1,
	.mode = SPI_LOOP | SPI_MODE_3 | SPI_3WIRE,
};

static struct platform_device kona_sspi_spi0_device = {
	.dev = {
		.platform_data = &sspi_spi0_info,
		},
	.name = "kona_sspi_spi",
	.id = 0,
	.resource = kona_sspi_spi0_resource,
	.num_resources = ARRAY_SIZE(kona_sspi_spi0_resource),
};

#ifdef CONFIG_USB_DWC_OTG
static struct resource kona_hsotgctrl_platform_resource[] = {
	[0] = {
		.start = HSOTG_CTRL_BASE_ADDR,
		.end = HSOTG_CTRL_BASE_ADDR + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = CHIPREGS_BASE_ADDR,
		.end = CHIPREGS_BASE_ADDR + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	[2] = {
		.start = HUB_CLK_BASE_ADDR,
		.end = HUB_CLK_BASE_ADDR + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
};

static struct platform_device board_kona_hsotgctrl_platform_device = {
	.name = "bcm_hsotgctrl",
	.id = -1,
	.resource = kona_hsotgctrl_platform_resource,
	.num_resources = ARRAY_SIZE(kona_hsotgctrl_platform_resource),
};

static struct resource kona_otg_platform_resource[] = {
	[0] = { /* Keep HSOTG_BASE_ADDR as first IORESOURCE_MEM to be compatible with legacy code */
		.start = HSOTG_BASE_ADDR,
		.end = HSOTG_BASE_ADDR + SZ_64K - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = BCM_INT_ID_USB_HSOTG,
		.end = BCM_INT_ID_USB_HSOTG,
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device board_kona_otg_platform_device = {
	.name = "dwc_otg",
	.id = -1,
	.resource = kona_otg_platform_resource,
	.num_resources = ARRAY_SIZE(kona_otg_platform_resource),
};
#endif

#ifdef CONFIG_SENSORS_KONA
static struct resource board_tmon_resource[] = {
	{	/* For Current Temperature */
		.start = TMON_BASE_ADDR,
		.end = TMON_BASE_ADDR + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	{	/* For Temperature IRQ */
		.start = BCM_INT_ID_TEMP_MON,
		.end = BCM_INT_ID_TEMP_MON,
		.flags = IORESOURCE_IRQ,
	},
};

struct platform_device tmon_device = {
	.name = "kona-tmon",
	.id = -1,
	.resource = board_tmon_resource,
	.num_resources = ARRAY_SIZE(board_tmon_resource),
};

static struct resource board_thermal_resource[] = {
	{	/* For Current Temperature */
		.start = TMON_BASE_ADDR,
		.end = TMON_BASE_ADDR + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	{	/* For Temperature IRQ */
		.start = BCM_INT_ID_TEMP_MON,
		.end = BCM_INT_ID_TEMP_MON,
		.flags = IORESOURCE_IRQ,
	},
};

static struct thermal_sensor_config sensor_data[] = {
	{			/*TMON sensor */
	 .thermal_id			 = 1,
		.thermal_name		   = "tmon",
		.thermal_type		   = SENSOR_BB_TMON,
		.thermal_mc			 = 0,
		.thermal_read		   = SENSOR_READ_DIRECT,
		.thermal_location	   = 1,
		.thermal_warning_lvl_1  = 100000,
		.thermal_warning_lvl_2  = 110000,
		.thermal_fatal_lvl	  = 120000,
		.thermal_lowest			= 200000,
		.thermal_highest		= 0,
		.thermal_warning_action = THERM_ACTION_NOTIFY,
		.thermal_fatal_action   = THERM_ACTION_NOTIFY_SHUTDOWN,
		.thermal_sensor_param   = 0,
		.thermal_control		= SENSOR_INTERRUPT,
		.convert_callback	   = NULL,
	},
	{			/*NTC (battery) sensor */
	 .thermal_id			 = 2,
		.thermal_name		   = "battery",
		.thermal_type		   = SENSOR_BATTERY,
		.thermal_mc			 = 0,
		.thermal_read		   = SENSOR_READ_PMU_I2C,
		.thermal_location	   = 2,
		.thermal_warning_lvl_1  = 105000,
		.thermal_warning_lvl_2  = 115000,
		.thermal_fatal_lvl	  = 125000,
		.thermal_lowest			= 200000,
		.thermal_highest		= 0,
		.thermal_warning_action = THERM_ACTION_NOTIFY,
		.thermal_fatal_action   = THERM_ACTION_NOTIFY_SHUTDOWN,
#ifdef CONFIG_MFD_BCM_PMU590XX
	 .thermal_sensor_param   = ADC_NTC_CHANNEL,
#else
	 .thermal_sensor_param = PMU_ADC_NTC,
#endif
	 .thermal_control		= SENSOR_PERIODIC_READ,
		.convert_callback	   = NULL,
	},
	{			/*32kHz crystal sensor */
	 .thermal_id			 = 3,
		.thermal_name		   = "32k",
		.thermal_type		   = SENSOR_CRYSTAL,
		.thermal_mc			 = 0,
		.thermal_read		   = SENSOR_READ_PMU_I2C,
		.thermal_location	   = 3,
		.thermal_warning_lvl_1  = 106000,
		.thermal_warning_lvl_2  = 116000,
		.thermal_fatal_lvl	  = 126000,
		.thermal_lowest			= 200000,
		.thermal_highest		= 0,
		.thermal_warning_action = THERM_ACTION_NOTIFY,
		.thermal_fatal_action   = THERM_ACTION_NOTIFY_SHUTDOWN,
#ifdef CONFIG_MFD_BCM_PMU590XX
	 .thermal_sensor_param   = ADC_32KTEMP_CHANNEL,
#else
	 .thermal_sensor_param = PMU_ADC_32KTEMP,
#endif
	 .thermal_control		= SENSOR_PERIODIC_READ,
		.convert_callback	   = NULL,
	},
	{			/*PA sensor */
	 .thermal_id			 = 4,
		.thermal_name		   = "PA",
		.thermal_type		   = SENSOR_PA,
		.thermal_mc			 = 0,
		.thermal_read		   = SENSOR_READ_PMU_I2C,
		.thermal_location	   = 4,
		.thermal_warning_lvl_1  = 107000,
		.thermal_warning_lvl_2  = 117000,
		.thermal_fatal_lvl	  = 127000,
		.thermal_lowest			= 200000,
		.thermal_highest		= 0,
		.thermal_warning_action = THERM_ACTION_NOTIFY,
		.thermal_fatal_action   = THERM_ACTION_NOTIFY_SHUTDOWN,
#ifdef CONFIG_MFD_BCM_PMU590XX
	 .thermal_sensor_param   = ADC_PATEMP_CHANNEL,
#else
	 .thermal_sensor_param = PMU_ADC_PATEMP,
#endif
	 .thermal_control		= SENSOR_PERIODIC_READ,
		.convert_callback	   = NULL,
	}
};


static struct therm_data thermal_pdata = {
	.flags = 0,
	.thermal_update_interval = 0,
	.num_sensors = 4,
	.sensors = sensor_data,
};

struct platform_device thermal_device = {
	.name = "kona-thermal",
	.id = -1,
	.resource = board_thermal_resource,
	.num_resources = ARRAY_SIZE(board_thermal_resource),
	.dev = {
		.platform_data = &thermal_pdata,
	},
};
#endif

#ifdef CONFIG_KONA_CPU_FREQ_DRV
struct kona_freq_tbl kona_freq_tbl[] = {
#ifdef CONFIG_CAPRI_156M
	FTBL_INIT(156000, PI_PROC_OPP_ECONOMY),
	FTBL_INIT(312000, PI_PROC_OPP_ECONOMY1),
	FTBL_INIT(600000, PI_PROC_OPP_NORMAL),
	FTBL_INIT(800000, PI_PROC_OPP_TURBO1),
	FTBL_INIT(1200000, PI_PROC_OPP_TURBO),
#else
#ifdef CONFIG_CAPRI_PM_A1
	FTBL_INIT(312000, PI_PROC_OPP_ECONOMY1),
	FTBL_INIT(600000, PI_PROC_OPP_NORMAL),
	FTBL_INIT(800000, PI_PROC_OPP_TURBO1),
	FTBL_INIT(1200000, PI_PROC_OPP_TURBO),
#else
	FTBL_INIT(312000, PI_PROC_OPP_ECONOMY1),
	FTBL_INIT(550000, PI_PROC_OPP_NORMAL),
	FTBL_INIT(733333, PI_PROC_OPP_TURBO1),
	FTBL_INIT(1100000, PI_PROC_OPP_TURBO),
#endif
#endif
};

void capri_cpufreq_init(void)
{
	struct clk *a9_pll_chnl0;
	struct clk *a9_pll_chnl1;
	a9_pll_chnl0 = clk_get(NULL, A9_PLL_CHNL0_CLK_NAME_STR);
	a9_pll_chnl1 = clk_get(NULL, A9_PLL_CHNL1_CLK_NAME_STR);

	BUG_ON(IS_ERR_OR_NULL(a9_pll_chnl0) ||
				IS_ERR_OR_NULL(a9_pll_chnl1));

	/*Update DVFS freq table based on PLL settings done by the loader*/
#ifdef CONFIG_CAPRI_156M
	kona_freq_tbl[2].cpu_freq = 3*clk_get_rate(a9_pll_chnl0)/1000/4;
	kona_freq_tbl[3].cpu_freq = clk_get_rate(a9_pll_chnl0)/1000;
	kona_freq_tbl[4].cpu_freq = clk_get_rate(a9_pll_chnl1)/1000;

	pr_info("%s a9_pll_chnl0 freq = %dKhz a9_pll_chnl1 freq = %dKhz\n",
		__func__, kona_freq_tbl[3].cpu_freq, kona_freq_tbl[4].cpu_freq);
#else
	kona_freq_tbl[2].cpu_freq = clk_get_rate(a9_pll_chnl0)/1000;
	kona_freq_tbl[3].cpu_freq = clk_get_rate(a9_pll_chnl1)/1000;

	pr_info("%s a9_pll_chnl0 freq = %dKhz a9_pll_chnl1 freq = %dKhz\n",
		__func__, kona_freq_tbl[2].cpu_freq, kona_freq_tbl[3].cpu_freq);
#endif
}

struct kona_cpufreq_drv_pdata kona_cpufreq_drv_pdata = {
	.flags = KONA_CPUFREQ_UPDATE_LPJ,
	.freq_tbl = kona_freq_tbl,
	.num_freqs = ARRAY_SIZE(kona_freq_tbl),
	/*FIX ME: To be changed according to the cpu latency*/
	.latency = 10*1000,
	.pi_id = PI_MGR_PI_ID_ARM_CORE,
	.cpufreq_init = capri_cpufreq_init,
};

static struct platform_device kona_cpufreq_device = {
	.name	= "kona-cpufreq-drv",
	.id	  = -1,
	.dev = {
		.platform_data		= &kona_cpufreq_drv_pdata,
	},
};
#endif /*CONFIG_KONA_CPU_FREQ_DRV*/

#ifdef CONFIG_KONA_AVS
#define SILICON_NUM_BIN  5

void avs_silicon_type_notify(u32 silicon_type, int freq_id)
{
	pr_info("%s : silicon type = %d freq = %d\n", __func__,
			silicon_type,
			freq_id);
	pm_init_pmu_sr_vlt_map_table(silicon_type, freq_id);
}

static u32 svt_pmos_bin[SILICON_NUM_BIN + 1] = { 125, 146, 171, 201, 220, 240 };
static u32 svt_nmos_bin[SILICON_NUM_BIN + 1] = { 75, 96, 126, 151, 200, 220 };

static u32 lvt_pmos_bin[SILICON_NUM_BIN + 1] = { 150, 181, 216, 220, 240, 251 };
static u32 lvt_nmos_bin[SILICON_NUM_BIN + 1] = { 90, 111, 146, 181, 200, 220 };

u32 svt_silicon_type_lut[SILICON_NUM_BIN * SILICON_NUM_BIN] = {
	SILICON_SS, SILICON_SS, SILICON_TS, SILICON_TS, SILICON_TT,
	SILICON_SS, SILICON_TS, SILICON_TS, SILICON_TT, SILICON_TT,
	SILICON_TT, SILICON_TF, SILICON_FF, SILICON_FF, SILICON_FF,
	SILICON_TT, SILICON_TF, SILICON_FF, SILICON_FF, SILICON_FF,
	SILICON_TT, SILICON_TF, SILICON_FF, SILICON_FF, SILICON_FF
};

u32 lvt_silicon_type_lut[SILICON_NUM_BIN * SILICON_NUM_BIN] = {
	SILICON_SS, SILICON_SS, SILICON_TS, SILICON_TS, SILICON_TT,
	SILICON_SS, SILICON_TS, SILICON_TS, SILICON_TT, SILICON_TT,
	SILICON_TT, SILICON_TF, SILICON_FF, SILICON_FF, SILICON_FF,
	SILICON_TT, SILICON_TF, SILICON_FF, SILICON_FF, SILICON_FF,
	SILICON_TT, SILICON_TF, SILICON_FF, SILICON_FF, SILICON_FF
};

/* index = ATE_AVS_BIN[3:0]*/
#if 0
static struct kona_ate_lut_entry ate_lut[] = {
	{ATE_FIELD_RESERVED , ATE_FIELD_RESERVED}, /* 0 */
	{A9_FREQ_850_MHZ, SILICON_FF},	/* 1 */
	{A9_FREQ_850_MHZ, SILICON_TT},/* 2 */
	{A9_FREQ_850_MHZ, SILICON_SS},   /* 3 */
	{A9_FREQ_1_GHZ, SILICON_FF},	/* 4 */
	{A9_FREQ_1_GHZ, SILICON_TT},  /* 5 */
	{ATE_FIELD_RESERVED, ATE_FIELD_RESERVED},/* 6 - Reserved */
	{ATE_FIELD_RESERVED, ATE_FIELD_RESERVED},/* 7 - Reserved */
	{ATE_FIELD_RESERVED, ATE_FIELD_RESERVED},/* 8 - Reserved */
	{ATE_FIELD_RESERVED, ATE_FIELD_RESERVED},/* 9 - Reserved */
	{A9_FREQ_850_MHZ, SILICON_TT}, /* 10 */
	{A9_FREQ_850_MHZ, SILICON_SS},	/* 11 */
	{ATE_FIELD_RESERVED, ATE_FIELD_RESERVED},/* 12 - reserved */
	{ATE_FIELD_RESERVED, ATE_FIELD_RESERVED},/* 13 - reserved */
	{A9_FREQ_1_GHZ, SILICON_TT},	/* 14 */
	{A9_FREQ_850_MHZ, SILICON_TT},/* 15 */
};

#else
static struct kona_ate_lut_entry ate_lut[] = {
	{ATE_FIELD_RESERVED , ATE_FIELD_RESERVED}, /* 0 */
	{A9_FREQ_1_GHZ, SILICON_FF},	/* 1 */
	{A9_FREQ_1_GHZ, SILICON_FF},/* 2 */
	{A9_FREQ_1_GHZ, SILICON_SS},   /* 3 */
	{A9_FREQ_1_GHZ, SILICON_FF},	/* 4 */
	{A9_FREQ_1_GHZ, SILICON_FF},  /* 5 */
	{ATE_FIELD_RESERVED, ATE_FIELD_RESERVED},/* 6 - Reserved */
	{ATE_FIELD_RESERVED, ATE_FIELD_RESERVED},/* 7 - Reserved */
	{ATE_FIELD_RESERVED, ATE_FIELD_RESERVED},/* 8 - Reserved */
	{ATE_FIELD_RESERVED, ATE_FIELD_RESERVED},/* 9 - Reserved */
	{A9_FREQ_1_GHZ, SILICON_FF}, /* 10 */
	{A9_FREQ_1_GHZ, SILICON_SS},	/* 11 */
	{ATE_FIELD_RESERVED, ATE_FIELD_RESERVED},/* 12 - reserved */
	{ATE_FIELD_RESERVED, ATE_FIELD_RESERVED},/* 13 - reserved */
	{A9_FREQ_1_GHZ, SILICON_FF},	/* 14 */
	{A9_FREQ_1_GHZ, SILICON_FF},/* 15 */
};
#endif
static struct kona_avs_pdata avs_pdata = {
	.flags = AVS_TYPE_OPEN | AVS_READ_FROM_MEM | AVS_ATE_FEATURE_ENABLE,
	.avs_mon_addr = 0x34053FA8,
	/* Mem addr where ATE value is copied by ABI */
	.avs_ate_addr = 0x34053FA0,
	.ate_default_silicon_type = SILICON_SS,
	.ate_default_cpu_freq = A9_FREQ_1_GHZ,
	.nmos_bin_size = SILICON_NUM_BIN,
	.pmos_bin_size = SILICON_NUM_BIN,

	.svt_pmos_bin = svt_pmos_bin,
	.svt_nmos_bin = svt_nmos_bin,

	.lvt_pmos_bin = lvt_pmos_bin,
	.lvt_nmos_bin = lvt_nmos_bin,

	.svt_silicon_type_lut = svt_silicon_type_lut,
	.lvt_silicon_type_lut = lvt_silicon_type_lut,
	.ate_lut = ate_lut,

	.silicon_type_notify = avs_silicon_type_notify,
};

struct platform_device kona_avs_device = {
	.name = "kona-avs",
	.id = -1,
	.dev = {
		.platform_data = &avs_pdata,
		},
};

#endif


#ifdef CONFIG_DMAC_PL330
static struct kona_pl330_data capri_pl330_pdata =	{
	/* Non Secure DMAC virtual base address */
	.dmac_ns_base = KONA_DMAC_NS_VA,
	/* Secure DMAC virtual base address */
	.dmac_s_base = KONA_DMAC_S_VA,
	/* # of PL330 dmac channels 'configurable' */
	.num_pl330_chans = 8,
	/* irq number to use */
	.irq_base = BCM_INT_ID_RESERVED184,
	/* # of PL330 Interrupt lines connected to GIC */
	.irq_line_count = 8,
};

static struct platform_device pl330_dmac_device = {
	.name = "kona-dmac-pl330",
	.id = 0,
	.dev = {
		.platform_data = &capri_pl330_pdata,
		.coherent_dma_mask  = DMA_BIT_MASK(64),
	},
};
#endif

#if defined(CONFIG_CRYPTO_DEV_BRCM_SPUM_HASH)
static struct resource board_spum_resource[] = {
	   [0] = {
				.start  =	   SEC_SPUM_NS_APB_BASE_ADDR,
				.end	=	   SEC_SPUM_NS_APB_BASE_ADDR + SZ_64K - 1,
				.flags  =	   IORESOURCE_MEM,
	   },
	   [1] = {
				.start  =	   SPUM_NS_BASE_ADDR,
				.end	=	   SPUM_NS_BASE_ADDR + SZ_64K - 1,
				.flags  =	   IORESOURCE_MEM,
	   }
};

static struct platform_device board_spum_device = {
		.name		   =	   "brcm-spum",
		.id			 =	   0,
		.resource	   =	   board_spum_resource,
		.num_resources  =	   ARRAY_SIZE(board_spum_resource),
};
#endif

#if defined(CONFIG_CRYPTO_DEV_BRCM_SPUM_AES)
static struct resource board_spum_aes_resource[] = {
	[0] = {
		.start  =	   SEC_SPUM_NS_APB_BASE_ADDR,
		.end	=	   SEC_SPUM_NS_APB_BASE_ADDR + SZ_64K - 1,
		.flags  =	   IORESOURCE_MEM,
	},
	[1] = {
		.start  =	   SPUM_NS_BASE_ADDR,
		.end	=	   SPUM_NS_BASE_ADDR + SZ_64K - 1,
		.flags  =	   IORESOURCE_MEM,
	}
};

static struct platform_device board_spum_aes_device = {
	.name		   =	   "brcm-spum-aes",
	.id			 =	   0,
	.resource	   =	   board_spum_aes_resource,
	.num_resources  =	   ARRAY_SIZE(board_spum_aes_resource),
};
#endif

#ifdef CONFIG_PM_WAKELOCK_HELPER
static struct platform_device kpwh_device = {
	.name = "kpwh",
	.id   = -1,
};
#endif

#ifdef CONFIG_BCM_VC_SYSMAN_REMOTE
static struct platform_device vc_sysman_remote_device = {
	.name = "vc-sysman-remote",
	.id   = -1,
};
#endif

#ifdef CONFIG_BCM_VC_DISPLAY
static struct platform_device vc_display_device = {
		.name = "vc-display",
		.id   = -1,
};
#endif


/* Common devices among all Island boards */
static struct platform_device *board_common_plat_devices[] __initdata = {
	&board_serial_device,
	&kona_sspi_spi0_device,
#if defined(CONFIG_BCM_OTP)
	&otp_device,
#endif
#if defined(CONFIG_MPCORE_WATCHDOG)
		&wdt_device,
#endif
#if defined(CONFIG_KONA_WATCHDOG)
		&wdt_device,
#endif
#if defined(CONFIG_HW_RANDOM_KONA)
		&rng_device,
#endif
#if defined(CONFIG_RTC_DRV_ISLAND)
		&rtc_device,
#endif
#if defined(CONFIG_MTD_BCMNAND)
		&nand_device,
#endif
#if defined(CONFIG_KONA_LEDM)
        $ledm_device,
#endif
#if defined(CONFIG_KONA_PWMC)
		&pwm_device,
#endif

#ifdef CONFIG_STM_TRACE
	&kona_stm_device,
#endif
	&pmu_device,

#ifdef CONFIG_USB_DWC_OTG
	&board_kona_hsotgctrl_platform_device,
	&board_kona_otg_platform_device,
#endif

#ifdef CONFIG_KONA_AVS
	&kona_avs_device,
#endif

#if defined(CONFIG_BACKLIGHT_PWM)
	&pwm_backlight_device,
#endif

#ifdef CONFIG_KONA_CPU_FREQ_DRV
	&kona_cpufreq_device,
#endif

#ifdef CONFIG_SENSORS_KONA
	&tmon_device,
	&thermal_device,
#endif

#ifdef CONFIG_DMAC_PL330
	&pl330_dmac_device,
#endif

#ifdef CONFIG_CRYPTO_DEV_BRCM_SPUM_HASH
	&board_spum_device,
#endif

#ifdef CONFIG_CRYPTO_DEV_BRCM_SPUM_AES
	&board_spum_aes_device,
#endif

#ifdef CONFIG_PM_WAKELOCK_HELPER
	&kpwh_device,
#endif
#ifdef CONFIG_BCM_VC_SYSMAN_REMOTE
	&vc_sysman_remote_device,
#endif

#ifdef CONFIG_BCM_VC_DISPLAY
	   &vc_display_device,
#endif

#if defined(CONFIG_WD_TAPPER)
	&wd_tapper,
#endif
};

void __init board_add_common_devices(void)
{
	platform_add_devices(board_common_plat_devices,
			ARRAY_SIZE(board_common_plat_devices));
}

