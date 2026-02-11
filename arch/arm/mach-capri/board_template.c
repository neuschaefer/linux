/*****************************************************************************
* Copyright 2011 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a
* license other than the GPL, without Broadcom's express prior written
* consent.
*****************************************************************************/

/*
 * A board template for adding devices and pass their associated board
 * dependent configurations as platform_data into the drivers
 *
 * This file needs to be included by the board specific source code
 */

#include <linux/init.h>
#include <linux/dma-mapping.h>
#include <linux/serial_8250.h>
#include <linux/i2c.h>
#include <linux/i2c-kona.h>

#include <asm/memory.h>
#include <asm/sizes.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <asm/hardware/gic.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include <mach/clock.h>
#include <mach/hardware.h>
#include <mach/irqs.h>
#include <mach/kona.h>
#include <mach/dma_mmap.h>
#include <mach/sdma.h>

#ifdef CONFIG_I2C_GPIO
#include <linux/i2c-gpio.h>
#endif

#if defined(CONFIG_LITTLE_MEMC_QOS)
#include <mach/memc_qos.h>
#endif
#include <mach/sdio_platform.h>
#include <mach/usbh_cfg.h>

#include <sdio_settings.h>
#include <i2c_settings.h>
#include <usbh_settings.h>

#if defined(CONFIG_BCM_HALAUDIO)
#include <mach/halaudio_audioh_platform.h>
#include <mach/halaudio_pcm_platform.h>
#include <halaudio_settings.h>
#include <halaudio_audioh_settings.h>
#include <halaudio_pcm_settings.h>
#endif


#if defined(CONFIG_BCM_RFKILL) || defined(CONFIG_BCM_RFKILL_MODULE)
#include <linux/broadcom/bcmbt_rfkill.h>
#include <bcmbt_rfkill_settings.h>
#endif

#if defined(CONFIG_BCM_BT_LPM) || defined(CONFIG_BCM_BT_LPM_MODULE)
#include <linux/broadcom/bcmbt_lpm.h>
#include <bcmbt_lpm_settings.h>
#endif

#ifdef CONFIG_BCM_BZHW
#include <linux/broadcom/bcm_bzhw.h>
#endif

#ifdef CONFIG_USB_SWITCH_FSA9485
#include <linux/power_supply.h>
#include <linux/i2c/fsa9485.h>
#endif

#if defined(CONFIG_TOUCHSCREEN_EGALAX_I2C) || defined(CONFIG_TOUCHSCREEN_EGALAX_I2C_MODULE)
#include <linux/i2c/egalax_i2c_ts.h>
#include <egalax_i2c_ts_settings.h>
#endif

#if defined(CONFIG_TOUCHSCREEN_BCM915500) || defined(CONFIG_TOUCHSCREEN_BCM915500_MODULE)
#include <linux/i2c/bcm15500_i2c_ts.h>
#endif

#if defined(CONFIG_MPU_SENSORS_MPU6050B1) || defined(CONFIG_MPU_SENSORS_MPU6050B1_MODULE)
#include <linux/mpu.h>
#include <mpu6050_settings.h>
#endif

#if defined(CONFIG_BMP18X_I2C) || defined(CONFIG_BMP18X_I2C_MODULE)
#include <linux/bmp18x.h>
#include <bmp18x_i2c_settings.h>
#endif

#if defined(CONFIG_AL3006) || defined(CONFIG_AL3006_MODULE)
#include <linux/al3006.h>
#include <al3006_i2c_settings.h>
#endif

#if defined(CONFIG_AMI306) || defined(CONFIG_AMI306_MODULE)
#include <linux/ami306_def.h>
#include <linux/ami_sensor.h>
#include <ami306_settings.h>
#endif

#if defined(CONFIG_NET_ISLAND)
#include <mach/net_platform.h>
#include <net_settings.h>
#endif

#if defined(CONFIG_LEDS_GPIO) || defined(CONFIG_LEDS_GPIO_MODULE)
#include <leds_gpio_settings.h>
#include <linux/leds.h>
#endif

#if defined(CONFIG_KEYBOARD_GPIO) || defined(CONFIG_KEYBOARD_GPIO_MODULE)
#include <linux/input.h>
#include <linux/gpio_keys.h>
#include <gpio_keys_settings.h>
#endif

#if defined(CONFIG_KONA_VCHIQ) || defined(CONFIG_KONA_VCHIQ_MODULE)
#include <mach/io_map.h>
#include <mach/aram_layout.h>

#include <linux/broadcom/vchiq_platform_data_kona.h>
#include <linux/broadcom/vchiq_platform_data_memdrv_kona.h>
#include <camera_settings.h>
#endif

#if defined(CONFIG_KEYBOARD_KONA) || defined(CONFIG_KEYBOARD_KONA_MODULE)
#include <linux/kona_keypad.h>
#include <keymap_settings.h>
#endif

#if defined(CONFIG_BCM_GPS) || defined(CONFIG_BCM_GPS_MODULE)
#include <gps_settings.h>
#include <linux/broadcom/gps.h>
#endif

#if defined(CONFIG_BCM_HAPTICS) || defined(CONFIG_BCM_HAPTICS_MODULE)
#include <linux/broadcom/bcm_haptics.h>
#include <bcm_haptics_settings.h>
#endif

#if defined(CONFIG_BCM_HEADSET_SW)
#include <linux/broadcom/headset_cfg.h>
#include <headset_settings.h>
#endif

#if defined(CONFIG_KONA_HEADSET) || defined(CONFIG_KONA_HEADSET_MULTI_BUTTON)
#include <mach/kona_headset_pd.h>
#endif

#if defined(CONFIG_BCM_HDMI_DET) || defined(CONFIG_BCM_HDMI_DET_MODULE)
#include <linux/broadcom/hdmi_cfg.h>
#include <hdmi_settings.h>
#endif

#if defined(CONFIG_TFT_PANEL) || defined(CONFIG_TFT_PANEL_MODULE)
#include <linux/broadcom/tft_panel.h>
#include <tft_panel_settings.h>
#endif

#ifdef CONFIG_TOUCHSCREEN_TANGO
#include <linux/i2c/tango_ts.h>
#endif

#ifdef CONFIG_TOUCHSCREEN_QT602240
#include <linux/i2c/qt602240_ts.h>
#endif

#ifdef CONFIG_TOUCHSCREEN_GT818
#include <linux/i2c/gt818-ts.h>
#endif

#if defined(CONFIG_BCM2079x_NFC)
#include <linux/bcmi2cnfc.h>
#include <bcmi2cnfc_settings.h>
#endif

#ifdef CONFIG_KEYBOARD_BCM
#include <linux/input.h>
#include <mach/bcm_keypad.h>
#endif

#ifdef CONFIG_WD_TAPPER
#include <linux/broadcom/wd-tapper.h>
#endif

#ifdef CONFIG_BCM_VC_CMA
#include <linux/broadcom/vc_cma.h>
#endif

#include "mach.h"
#include "common.h"

#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif

#ifdef CONFIG_TOUCHSCREEN_ATMEL_MXT224 
#include "board-tsp-mxt224.c"
#endif

#ifdef CONFIG_SAMSUNG_MHL
#include "board_mhl_sii9234.c"
#endif

#include <linux/mfd/bcmpmu.h>

#ifndef CAPRI_BOARD_ID
#error CAPRI_BOARD_ID needs to be defined in board_xxx.c
#endif

#ifdef CONFIG_BRCM_UNIFIED_DHD_SUPPORT

#include "board-capri-wifi.h"
extern int capri_wifi_status_register(void (*callback) (int card_present, void *dev_id),
			  void *dev_id);

#endif

#define INT_SRAM_BASE             0x34040000

#if defined(CONFIG_LITTLE_MODE)
#define VC_EMI                    0x80000000 /* VC4 EMI */
#else
#define VC_EMI                    0x40000000 /* VC4 EMI */
#endif


/*
 * Since this board template is included by each board_xxx.c. We concatenate
 * CAPRI_BOARD_ID to help debugging when multiple boards are compiled into
 * a single image
 */
#define concatenate_again(a, b) a ## b
#define concatenate(a, b) concatenate_again(a, b)

/* The SDIO index starts from 1 in RDB. Remap to start numbering from 0 */
#define PHYS_ADDR_SDIO0        SDIO1_BASE_ADDR
#define PHYS_ADDR_SDIO1        SDIO2_BASE_ADDR
#define PHYS_ADDR_SDIO2        SDIO3_BASE_ADDR
#define PHYS_ADDR_SDIO3        SDIO4_BASE_ADDR
#define SDIO_CORE_REG_SIZE     0x10000


/* number of I2C adapters (hosts/masters) */
#define MAX_I2C_ADAPS    4

/* The BSC (I2C) index starts from 1 in RDB. Remap to start at 0 */
#define PHYS_ADDR_BSC0         BSC1_BASE_ADDR
#define PHYS_ADDR_BSC1         BSC2_BASE_ADDR
#define PHYS_ADDR_BSC2         PMU_BSC_BASE_ADDR
#define PHYS_ADDR_BSC3         BSC3_BASE_ADDR

#define BSC_CORE_REG_SIZE      0x100

#define USBH_EHCI_CORE_REG_SIZE    0x90
#define USBH_OHCI_CORE_REG_SIZE    0x1000
#define USBH_DWC_REG_OFFSET        USBH_EHCI_CORE_REG_SIZE
#define USBH_DWC_BASE_ADDR         (EHCI_BASE_ADDR + USBH_DWC_REG_OFFSET)
#define USBH_DWC_CORE_REG_SIZE     0x20
#define USBH_CTRL_CORE_REG_SIZE    0x20

#define USBH_HSIC2_EHCI_CORE_REG_SIZE	0x90
#define USBH_HSIC_PHY_CORE_REG_SIZE		0x40

#define OTG_CTRL_CORE_REG_SIZE     0x100

#if defined(CONFIG_MACH_CAPRI_RAY) || defined(CONFIG_MACH_CAPRI_STONE) || defined(CONFIG_MACH_CAPRI_SS)
#define TANGO_GPIO_RESET_PIN        0
#define TANGO_GPIO_IRQ_PIN          1
#define TANGO_I2C_BUS_ID            1
#endif

#ifdef CONFIG_TOUCHSCREEN_TANGO
#define IS_MULTI_TOUCH  1
#define MAX_NUM_FINGERS 2
#endif

#ifdef CONFIG_TOUCHSCREEN_GT818
#define GT818_GPIO_RESET_PIN        (10)
#define GT818_GPIO_IRQ_PIN          (11)
#define GT818_I2C_BUS_ID            (10)


#define CAPRI_SCL_GPIO  (8)
#define CAPRI_SDA_GPIO  (9)
#endif

#ifdef CONFIG_KEYBOARD_BCM
/* keypad map */
#define BCM_KEY_ROW_0  0
#define BCM_KEY_ROW_1  1
#define BCM_KEY_ROW_2  2
#define BCM_KEY_ROW_3  3
#define BCM_KEY_ROW_4  4
#define BCM_KEY_ROW_5  5
#define BCM_KEY_ROW_6  6
#define BCM_KEY_ROW_7  7

#define BCM_KEY_COL_0  0
#define BCM_KEY_COL_1  1
#define BCM_KEY_COL_2  2
#define BCM_KEY_COL_3  3
#define BCM_KEY_COL_4  4
#define BCM_KEY_COL_5  5
#define BCM_KEY_COL_6  6
#define BCM_KEY_COL_7  7
#endif

static struct resource sdio0_resource[] = {
	[0] = {
		.start = PHYS_ADDR_SDIO0,
		.end = PHYS_ADDR_SDIO0 + SDIO_CORE_REG_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = BCM_INT_ID_SDIO0,
		.end = BCM_INT_ID_SDIO0,
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource sdio1_resource[] = {
	[0] = {
		.start = PHYS_ADDR_SDIO1,
		.end = PHYS_ADDR_SDIO1 + SDIO_CORE_REG_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = BCM_INT_ID_SDIO1,
		.end = BCM_INT_ID_SDIO1,
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource sdio2_resource[] = {
        [0] = {
                .start = PHYS_ADDR_SDIO2,
                .end = PHYS_ADDR_SDIO2 + SDIO_CORE_REG_SIZE - 1,
                .flags = IORESOURCE_MEM,
        },
        [1] = {
                .start = BCM_INT_ID_SDIO_NAND,
                .end = BCM_INT_ID_SDIO_NAND,
                .flags = IORESOURCE_IRQ,
        },
};

static struct resource sdio3_resource[] = {
        [0] = {
                .start = PHYS_ADDR_SDIO3,
                .end = PHYS_ADDR_SDIO3 + SDIO_CORE_REG_SIZE - 1,
                .flags = IORESOURCE_MEM,
        },
        [1] = {
                .start = BCM_INT_ID_SDIO_MMC,
                .end = BCM_INT_ID_SDIO_MMC,
                .flags = IORESOURCE_IRQ,
        },
};


static struct sdio_platform_cfg sdio_param[] =
#ifdef HW_SDIO_PARAM
	HW_SDIO_PARAM;
#else
	{};
#endif

static struct platform_device sdio_devices[SDIO_MAX_NUM_DEVICES] =
{
	{ /* SDIO0 */
		.name = "sdhci",
		.id = 0,
		.resource = sdio0_resource,
		.num_resources	= ARRAY_SIZE(sdio0_resource),
	},
	{ /* SDIO1 */
		.name = "sdhci",
		.id = 1,
		.resource = sdio1_resource,
		.num_resources	= ARRAY_SIZE(sdio1_resource),
	},
	{/* SDIO2 */
		.name = "sdhci",
		.id = 2,
		.resource = sdio2_resource,
		.num_resources    = ARRAY_SIZE(sdio2_resource),
	},
	{/* SDIO3 */
                .name = "sdhci",
                .id = 3,
                .resource = sdio3_resource,
                .num_resources    = ARRAY_SIZE(sdio3_resource),
        },
};

#if defined(CONFIG_NET_ISLAND)
static struct island_net_hw_cfg net_data =
#ifdef HW_CFG_NET
	HW_CFG_NET;
#else
{
	.addrPhy0 = 0,
	.addrPhy1 = 1,
	.gpioPhy0 = -1,
	.gpioPhy1 = -1,
	.gpioPhyPwrEn = -1,
	.phy_regulator_name = NULL,
};
#endif

static struct platform_device net_device =
{
	.name = "island-net",
	.id = -1,
	.dev =
	{
		.platform_data = &net_data,
	},
};
#endif /* CONFIG_NET_ISLAND */

static struct bsc_adap_cfg i2c_adap_param[] =
#ifdef HW_I2C_ADAP_PARAM
	HW_I2C_ADAP_PARAM;
#else
	{};
#endif

static struct resource i2c0_resource[] = {
	[0] =
	{
		.start = PHYS_ADDR_BSC0,
		.end = PHYS_ADDR_BSC0 + BSC_CORE_REG_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] =
	{
		.start = BCM_INT_ID_I2C0,
		.end = BCM_INT_ID_I2C0,
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource i2c1_resource[] = {
	[0] =
	{
		.start = PHYS_ADDR_BSC1,
		.end = PHYS_ADDR_BSC1 + BSC_CORE_REG_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] =
	{
		.start = BCM_INT_ID_I2C1,
		.end = BCM_INT_ID_I2C1,
		.flags = IORESOURCE_IRQ,
	},
};
static struct resource i2c2_resource[] = {
	[0] =
	{
		.start = PHYS_ADDR_BSC2,
		.end = PHYS_ADDR_BSC2 + BSC_CORE_REG_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] =
	{
		.start = BCM_INT_ID_PM_I2C,
		.end = BCM_INT_ID_PM_I2C,
		.flags = IORESOURCE_IRQ,
	},
};
static struct resource i2c3_resource[] = {
	[0] =
	{
		.start = PHYS_ADDR_BSC3,
		.end = PHYS_ADDR_BSC3 + BSC_CORE_REG_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] =
	{
		.start = BCM_INT_ID_I2C2,
		.end = BCM_INT_ID_I2C2,
		.flags = IORESOURCE_IRQ,
	},
};



static struct platform_device i2c_adap_devices[MAX_I2C_ADAPS] =
{
	{  /* for BSC0 */
		.name = "bsc-i2c",
		.id = 0,
		.resource = i2c0_resource,
		.num_resources	= ARRAY_SIZE(i2c0_resource),
	},
	{  /* for BSC1 */
		.name = "bsc-i2c",
		.id = 1,
		.resource = i2c1_resource,
		.num_resources	= ARRAY_SIZE(i2c1_resource),
	},
	{  /* for BSC2 */
		.name = "bsc-i2c",
		.id = 2,
		.resource = i2c2_resource,
		.num_resources	= ARRAY_SIZE(i2c2_resource),
	},
	{  /* for PMU BSC */
		.name = "bsc-i2c",
		.id = 3,
		.resource = i2c3_resource,
		.num_resources	= ARRAY_SIZE(i2c3_resource),
	},
};

#ifdef CONFIG_BCM_HEADSET_SW

#define board_headsetdet_data concatenate(CAPRI_BOARD_ID, _headsetdet_data)
static struct headset_hw_cfg board_headsetdet_data =
#ifdef HW_CFG_HEADSET
	HW_CFG_HEADSET;
#else
{
	.gpio_headset_det = -1,
	.gpio_headset_active_low = 0,
	.gpio_mic_det = -1,
	.gpio_mic_active_low = 0,
};
#endif

#define board_headsetdet_device concatenate(CAPRI_BOARD_ID, _headsetdet_device)
static struct platform_device board_headsetdet_device =
{
	.name = "bcm-headset-det",
	.id = -1,
	.dev =
	{
		.platform_data = &board_headsetdet_data,
	},
};

#define board_add_headsetdet_device concatenate(CAPRI_BOARD_ID, _add_headsetdet_device)
static void __init board_add_headsetdet_device(void)
{
	platform_device_register(&board_headsetdet_device);
}

#endif /* CONFIG_BCM_HEADSET_SW */

#if defined(CONFIG_KONA_HEADSET) || defined(CONFIG_KONA_HEADSET_MULTI_BUTTON)
#define HS_IRQ		gpio_to_irq(12)
#define HSB_IRQ		BCM_INT_ID_AUXMIC_COMP2
#define HSB_REL_IRQ 	BCM_INT_ID_AUXMIC_COMP2_INV
#ifdef CONFIG_KONA_GPIO_HEADSET_SW_EN
#define GPIO_HEADSET_SW_EN	101 /* GPIO101 */
#else
#define GPIO_HEADSET_SW_EN	-1
#endif
static struct kona_headset_pd headset_data = {
	/* GPIO state read is 1 on HS insert and 0 for
	 * HS remove
	 */
#if defined(CONFIG_MACH_CAPRI_SS) || defined(CONFIG_MACH_CAPRI_M500)
	.hs_default_state = 1,
#else
	.hs_default_state = 0,
#endif
	/*
	 * Because of the presence of the resistor in the MIC_IN line.
	 * The actual ground is not 0, but a small offset is added to it.
	 * This needs to be subtracted from the measured voltage to determine the
	 * correct value. This will vary for different HW based on the resistor
	 * values used.
	 *
	 * What this means to Rhearay?
	 * From the schematics looks like there is no such resistor put on
	 * Rhearay. That means technically there is no need to subtract any extra load
	 * from the read Voltages. On other HW, if there is a resistor present
	 * on this line, please measure the load value and put it here.
	 */
#if defined(CONFIG_MACH_CAPRI_M500)
    .phone_ref_offset = 70,
#else
	.phone_ref_offset = 0,
#endif
	.gpio_for_accessory_detection = 1,
	.aci_clk_name = "aci_apb_clk",
	.gpio_headset_sw_en = GPIO_HEADSET_SW_EN,
};

static struct resource board_headset_resource[] = {
	{	/* For AUXMIC */
		.start = AUXMIC_BASE_ADDR,
		.end = AUXMIC_BASE_ADDR + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	{	/* For ACI */
		.start = ACI_BASE_ADDR,
		.end = ACI_BASE_ADDR + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	{	/* For Headset IRQ */
		.start = HS_IRQ,
		.end = HS_IRQ,
		.flags = IORESOURCE_IRQ,
	},
	{	/* For Headset button  press IRQ */
		.start = HSB_IRQ,
		.end = HSB_IRQ,
		.flags = IORESOURCE_IRQ,
	},
	{	/* For Headset button  release IRQ */
		.start = HSB_REL_IRQ,
		.end = HSB_REL_IRQ,
		.flags = IORESOURCE_IRQ,
	},
	{	/* COMP1 for type detection */
		.start = BCM_INT_ID_AUXMIC_COMP1,
		.end = HSB_REL_IRQ,
		.flags = IORESOURCE_IRQ,
	},
};

struct platform_device headset_device = {
	.name = "konaaciheadset",
	.id = -1,
	.resource = board_headset_resource,
	.num_resources	= ARRAY_SIZE(board_headset_resource),
	.dev	=	{
		.platform_data = &headset_data,
	},
};
#endif /* CONFIG_KONA_HEADSET || CONFIG_KONA_HEADSET_MULTI_BUTTON */

#if defined(CONFIG_BCM_HDMI_DET) || defined(CONFIG_BCM_HDMI_DET_MODULE)

#define board_hdmidet_data concatenate(CAPRI_BOARD_ID, _hdmidet_data)
static struct hdmi_hw_cfg board_hdmidet_data =
#ifdef HW_CFG_HDMI
	HW_CFG_HDMI;
#else
{
	.gpio_hdmi_det = -1,
};
#endif

#define board_hdmidet_device concatenate(CAPRI_BOARD_ID, _hdmidet_device)
static struct platform_device board_hdmidet_device =
{
	.name = "hdmi-detect",
	.id = -1,
	.dev =
	{
		.platform_data = &board_hdmidet_data,
	},
};

#define board_add_hdmidet_device concatenate(CAPRI_BOARD_ID, _add_hdmidet_device)
static void __init board_add_hdmidet_device(void)
{
	platform_device_register(&board_hdmidet_device);
}

#endif /* #if defined(CONFIG_BCM_HDMI_DET) || defined(CONFIG_BCM_HDMI_DET_MODULE) */

#if defined(CONFIG_TFT_PANEL) || defined(CONFIG_TFT_PANEL_MODULE)
#define board_tft_panel_data concatenate(CAPRI_BOARD_ID, _tft_panel_data)
static struct tft_panel_platform_data board_tft_panel_data = TFT_PANEL_SETTINGS;

#define board_tft_panel_device concatenate(CAPRI_BOARD_ID, _tft_panel_device)
static struct platform_device board_tft_panel_device =
{
	.name = TFT_PANEL_DRIVER_NAME,
	.id = -1,
	.dev =
	{
		.platform_data = &board_tft_panel_data,
	},
};

#define board_add_tft_panel_device concatenate(CAPRI_BOARD_ID, _add_tft_panel_device)
static void __init board_add_tft_panel_device(void)
{
	platform_device_register(&board_tft_panel_device);
}
#endif

#if defined(CONFIG_USB_BCM_CAPRI)
static struct usbh_cfg usbh_param = HW_USBH_PARAM;

static struct resource usbh_resource[] = {
	[0] = {
		.start = USBH_PHY_BASE_ADDR,
		.end = USBH_PHY_BASE_ADDR + USBH_CTRL_CORE_REG_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
};

static struct platform_device usbh_device =
{
	.name = "usbh",
	.resource = usbh_resource,
	.num_resources = ARRAY_SIZE(usbh_resource),
	.dev = {
		.platform_data = &usbh_param,
	},
};
#endif

#if defined(CONFIG_USB_BCM_CAPRI_HSIC)
static struct usbh_cfg usbh_hsic_param = HW_USBH_HSIC_PARAM;

static struct resource usbh_hsic_resource[] = {
	[0] = {
		.start = HSIC_PHY_BASE_ADDR,
		.end = HSIC_PHY_BASE_ADDR + USBH_HSIC_PHY_CORE_REG_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
};

static struct platform_device usbh_hsic_device = {
	.name = "usbhsic",
	.resource = usbh_hsic_resource,
	.num_resources = ARRAY_SIZE(usbh_hsic_resource),
	.dev = {
		.platform_data = &usbh_hsic_param,
	},
};
#endif

#if defined(CONFIG_USB_EHCI_BCM)
static u64 ehci_dmamask = DMA_BIT_MASK(32);

static struct resource usbh_ehci_resource[] = {
	[0] = {
		.start = EHCI_BASE_ADDR,
		.end = EHCI_BASE_ADDR + USBH_EHCI_CORE_REG_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = BCM_INT_ID_ULPI_EHCI,
		.end = BCM_INT_ID_ULPI_EHCI,
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device usbh_ehci_device =
{
	.name = "bcm-ehci",
	.id = 0,
	.resource = usbh_ehci_resource,
	.num_resources = ARRAY_SIZE(usbh_ehci_resource),
	.dev = {
		.dma_mask = &ehci_dmamask,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
};
#endif

#if defined(CONFIG_USB_EHCI_BCM) && defined(CONFIG_USB_BCM_CAPRI_HSIC)
static u64 hsic2_ehci_dmamask = DMA_BIT_MASK(32);

static struct resource usbh_hsic2_ehci_resource[] = {
	[0] = {
		.start = HSIC2_EHCI_BASE_ADDR,
		.end = HSIC2_EHCI_BASE_ADDR + USBH_HSIC2_EHCI_CORE_REG_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = BCM_INT_ID_HSIC2_EHCI,
		.end = BCM_INT_ID_HSIC2_EHCI,
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device usbh_hsic2_ehci_device = {
	.name = "bcm-ehci",
	.id = 1,
	.resource = usbh_hsic2_ehci_resource,
	.num_resources = ARRAY_SIZE(usbh_hsic2_ehci_resource),
	.dev = {
		.dma_mask = &hsic2_ehci_dmamask,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
};
#endif

#if defined(CONFIG_USB_OHCI_BCM)

static u64 ohci_dmamask = DMA_BIT_MASK(32);

static struct resource usbh_ohci_resource[] = {
	[0] = {
		.start = OHCI_BASE_ADDR,
		.end = OHCI_BASE_ADDR + USBH_OHCI_CORE_REG_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = BCM_INT_ID_ULPI_OHCI,
		.end = BCM_INT_ID_ULPI_OHCI,
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device usbh_ohci_device =
{
	.name = "bcm-ohci",
	.id = 0,
	.resource = usbh_ohci_resource,
	.num_resources = ARRAY_SIZE(usbh_ohci_resource),
	.dev = {
		.dma_mask = &ohci_dmamask,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
};
#endif

#if defined(CONFIG_TOUCHSCREEN_EGALAX_I2C) || defined(CONFIG_TOUCHSCREEN_EGALAX_I2C_MODULE)
static struct egalax_i2c_ts_cfg egalax_i2c_param =
{
	.id = -1,
	.gpio = {
		.reset = -1,
		.event = -1,
	},
   .reset_time = 0,
   .reset_level = 0,
};

static struct i2c_board_info egalax_i2c_boardinfo[] =
{
	{
		.type = "egalax_i2c",
		.addr = 0x04,
		.platform_data = &egalax_i2c_param,
	},
};
#endif

#ifdef CONFIG_USB_SWITCH_FSA9485

enum cable_type_t{
	CABLE_TYPE_USB,
	CABLE_TYPE_AC,
	CABLE_TYPE_NONE
};


#define FSA9485_I2C_BUS_ID 8
#define GPIO_FSA9485_I2C_SDA 131
#define GPIO_FSA9485_I2C_SCL 132
#define GPIO_FSA9485_INT 100

/* Values in this table need to be revisited */
static int chrgr_curr_lmt[PMU_CHRGR_TYPE_MAX] = {
	[PMU_CHRGR_TYPE_NONE] = 0,
	[PMU_CHRGR_TYPE_SDP] = 0,
	[PMU_CHRGR_TYPE_CDP] = 500,
	[PMU_CHRGR_TYPE_DCP] = 700,
	[PMU_CHRGR_TYPE_TYPE1] = 1200,
	[PMU_CHRGR_TYPE_TYPE2] = 1200,
	[PMU_CHRGR_TYPE_PS2] = 0,
	[PMU_CHRGR_TYPE_ACA] = 0,
};

void send_usb_insert_event(enum bcmpmu_event_t event, void *para);
void send_chrgr_insert_event(enum bcmpmu_event_t event, void *para);
static enum cable_type_t set_cable_status;

static void fsa9485_usb_cb(bool attached)
{
	enum bcmpmu_chrgr_type_t chrgr_type;
	enum bcmpmu_usb_type_t usb_type;

	pr_info("fsa9485_usb_cb attached %d\n", attached);

	set_cable_status = attached ? CABLE_TYPE_USB : CABLE_TYPE_NONE;

	switch (set_cable_status) {
	case CABLE_TYPE_USB:
		usb_type = PMU_USB_TYPE_SDP;
		chrgr_type = PMU_CHRGR_TYPE_SDP;
		pr_info("%s USB attached\n",__func__);
		break;
		
	case CABLE_TYPE_NONE:
		usb_type = PMU_USB_TYPE_NONE;
		chrgr_type = PMU_CHRGR_TYPE_NONE;		
		pr_info("%s USB removed\n",__func__);
		break;
	}

	send_usb_insert_event(BCMPMU_USB_EVENT_USB_DETECTION, &usb_type);
	send_chrgr_insert_event(BCMPMU_CHRGR_EVENT_CHGR_DETECTION,&chrgr_type);
	
}

static void fsa9485_charger_cb(bool attached)
{
	enum bcmpmu_chrgr_type_t chrgr_type;
	enum bcmpmu_usb_type_t usb_type;
	
	pr_info("fsa9480_charger_cb attached %d\n", attached);

	set_cable_status = attached ? CABLE_TYPE_AC : CABLE_TYPE_NONE;

	switch (set_cable_status) {
	case CABLE_TYPE_AC:
		usb_type = PMU_USB_TYPE_NONE;
		chrgr_type = PMU_CHRGR_TYPE_DCP;		
		pr_info("%s TA attached\n",__func__);
		break;
	case CABLE_TYPE_NONE:
		usb_type = PMU_USB_TYPE_NONE;
		chrgr_type = PMU_CHRGR_TYPE_NONE;				
		pr_info("%s TA removed\n",__func__);
		break;
	}
	send_chrgr_insert_event(BCMPMU_CHRGR_EVENT_CHGR_DETECTION,&chrgr_type);
	send_chrgr_insert_event(BCMPMU_CHRGR_EVENT_CHRG_CURR_LMT,&chrgr_curr_lmt[chrgr_type]);
}

static void fsa9485_jig_cb(bool attached)
{
	pr_info("fsa9480_jig_cb attached %d\n", attached);
}
static void fsa9485_uart_cb(bool attached)
{
	pr_info("fsa9485_uart_cb attached %d\n", attached);
}

static struct fsa9485_platform_data fsa9485_pdata = {
	.usb_cb = fsa9485_usb_cb,
	.charger_cb = fsa9485_charger_cb,
	.jig_cb = fsa9485_jig_cb,
	.uart_cb = fsa9485_uart_cb,
};

static struct i2c_board_info  __initdata micro_usb_i2c_devices_info[]  = {
	{
		I2C_BOARD_INFO("fsa9485", 0x4A >> 1),
		.platform_data = &fsa9485_pdata,
		.irq = gpio_to_irq(GPIO_FSA9485_INT),
	},
};

static struct i2c_gpio_platform_data fsa_i2c_gpio_data={
	.sda_pin = GPIO_FSA9485_I2C_SDA,
	.scl_pin = GPIO_FSA9485_I2C_SCL,
	.udelay	 = 2,
};

static struct platform_device fsa_i2c_gpio_device = {
	.name			= "i2c-gpio",
	.id			= FSA9485_I2C_BUS_ID,
	.dev			={
		.platform_data	= &fsa_i2c_gpio_data,
	},
};

static struct platform_device *mUSB_i2c_devices[] __initdata = {
	&fsa_i2c_gpio_device,
};


#endif
#if defined(CONFIG_TOUCHSCREEN_BCM915500) || defined(CONFIG_TOUCHSCREEN_BCM915500_MODULE)
static struct bcmtch_platform_data bcm915500_i2c_platform_data =
{
	.i2c_bus_id             = BCMTCH_HW_I2C_BUS_ID,
	.i2c_addr_sys           = BCMTCH_HW_I2C_ADDR_SYS,

	.gpio_interrupt_pin     = BCMTCH_HW_GPIO_INTERRUPT_PIN,
	.gpio_interrupt_trigger = BCMTCH_HW_GPIO_INTERRUPT_TRIGGER,
	.gpio_reset_pin         = BCMTCH_HW_GPIO_RESET_PIN,
	.gpio_reset_polarity    = BCMTCH_HW_GPIO_RESET_POLARITY,
	.gpio_reset_time_ms     = BCMTCH_HW_GPIO_RESET_TIME_MS,
};

static struct i2c_board_info __initdata bcm915500_i2c_boardinfo[] =
{
	{
		I2C_BOARD_INFO(BCM15500_TSC_NAME, BCMTCH_HW_I2C_ADDR_SPM),
		.platform_data  = &bcm915500_i2c_platform_data,
		.irq            = gpio_to_irq(BCMTCH_HW_GPIO_INTERRUPT_PIN),
	},
};
#endif


#if defined(CONFIG_I2C_GPIO)
#if defined(CONFIG_TOUCHSCREEN_GT818)
/* Adding the device here as this device is board specific */
struct i2c_gpio_platform_data capri_i2c_gpio_data = {
	.sda_pin               = CAPRI_SDA_GPIO,
	.scl_pin               = CAPRI_SCL_GPIO,
	.udelay = 1,
	.sda_is_open_drain     = 0,
	.scl_is_open_drain     = 0,
 /*       .scl_is_open_drain     = 1,*/
 };
struct platform_device capri_i2c_gpio_device =  {
	.name   = "i2c-gpio",
	.id     = 0x0a,
	.dev    = {
		.platform_data = &capri_i2c_gpio_data,
	},
};
#endif

#ifdef CONFIG_3_TOUCH
#define GPIO_3_TOUCH_SDA 0
#define GPIO_3_TOUCH_SCL 1
#define I2C_BUS_ID_3_TOUCH 14

static struct i2c_gpio_platform_data touch_i2c_gpio_data = {
        .sda_pin    = GPIO_3_TOUCH_SDA,
        .scl_pin    = GPIO_3_TOUCH_SCL,
		//.sda_is_open_drain = true,
		//.scl_is_open_drain = true,
        .udelay  = 3,  //// brian :3
        .timeout = 100,
};

static struct platform_device touch_i2c_gpio_device = {
        .name       = "i2c-gpio",
        .id     = I2C_BUS_ID_3_TOUCH
        .dev        = {
            .platform_data  = &touch_i2c_gpio_data,
        },
};
#endif

#ifdef CONFIG_SAMSUNG_MHL

static struct i2c_gpio_platform_data mhl_i2c_gpio_data = {
        .sda_pin    = GPIO_MHL_SDA_1_8V,
        .scl_pin    = GPIO_MHL_SCL_1_8V,
		//.sda_is_open_drain = true,
		//.scl_is_open_drain = true,
        .udelay  = 3, 
        .timeout = 100,
};

static struct platform_device mhl_i2c_gpio_device = {
        .name       = "i2c-gpio",
        .id     = I2C_BUS_ID_MHL,
        .dev        = {
            .platform_data  = &mhl_i2c_gpio_data,
        },
};
#endif

#ifdef CONFIG_NFC_I2C_GPIO

static struct i2c_gpio_platform_data nfc_i2c_gpio_data = {
	.sda_pin    = BCMI2CNFC_SDA,
	.scl_pin    = BCMI2CNFC_SCL,
	.udelay  = 3, 
	.timeout = 100,
};

static struct platform_device nfc_i2c_gpio_device = {
        .name       = "i2c-gpio",
        .id     = BCMI2CNFC_BUSID,
        .dev        = {
		.platform_data  = &nfc_i2c_gpio_data,
        },
};

#endif

static struct platform_device *gpio_i2c_devices[] __initdata = {
#ifdef CONFIG_3_TOUCH
	&touch_i2c_gpio_device,
#endif

#ifdef CONFIG_SAMSUNG_MHL
	&mhl_i2c_gpio_device,
#endif

#ifdef CONFIG_NFC_I2C_GPIO
	&nfc_i2c_gpio_device,
#endif

};
#endif

#ifdef CONFIG_TOUCHSCREEN_QT602240

#define QT602240_INT_GPIO_PIN      2 /* skip expander chip */

static int qt602240_platform_init_hw(void)
{
	int rc;
	rc = gpio_request(QT602240_INT_GPIO_PIN, "ts_qt602240");
	if (rc < 0)
	{
		printk(KERN_ERR "unable to request GPIO pin %d\n", QT602240_INT_GPIO_PIN);
		return rc;
	}
	gpio_direction_input(QT602240_INT_GPIO_PIN);

	return 0;
}

static void qt602240_platform_exit_hw(void)
{
	gpio_free(QT602240_INT_GPIO_PIN);
}

static struct qt602240_platform_data qt602240_platform_data = {
	.i2c_pdata = {
					.spd_magic = SLAVE_SPD_MAGIC_NUM,
					.i2c_speed = 2,
				 },
	.x_line		= 15,
	.y_line		= 11,
	.x_size		= 1023,
	.y_size		= 1023,
	.x_min		= 90,
	.y_min		= 90,
	.x_max		= 0x3ff,
	.y_max		= 0x3ff,
	.max_area	= 0xff,
	.blen		= 33,
	.threshold	= 70,
	.voltage	= 2700000,              /* 2.8V */
	.orient		= QT602240_DIAGONAL_COUNTER,
	.init_platform_hw = qt602240_platform_init_hw,
	.exit_platform_hw = qt602240_platform_exit_hw,
};

static struct i2c_board_info __initdata qt602240_info[] = {
	{
		I2C_BOARD_INFO("qt602240_ts", 0x4a),
		.platform_data = &qt602240_platform_data,
		.irq = gpio_to_irq(QT602240_INT_GPIO_PIN),
	},
};
#endif /* CONFIG_TOUCHSCREEN_QT602240 */


#ifdef CONFIG_TOUCHSCREEN_ATMEL_MXT
#include <linux/i2c/atmel_mxt_ts.h>

#define MXT224_INT_GPIO_PIN      2 /* skip expander chip */

static int mxt224_platform_init_hw(void)
{
	int rc;
	rc = gpio_request(MXT224_INT_GPIO_PIN, "ts_mxt224");
	if (rc < 0)
	{
		printk(KERN_ERR "unable to request GPIO pin %d\n", MXT224_INT_GPIO_PIN);
		return rc;
	}
	gpio_direction_input(MXT224_INT_GPIO_PIN);

	return 0;
}

static void mxt224_platform_exit_hw(void)
{
	gpio_free(MXT224_INT_GPIO_PIN);
}

static struct mxt_platform_data mxt224_platform_data = {
	.x_line		= 19,
	.y_line		= 11,
	.x_size		= 800,
	.y_size		= 480,
	.blen		= 11,
	.threshold	= 40,
	.voltage	= 2800000,              /* 2.8V */
	.orient		= MXT_DIAGONAL,
	//.init_platform_hw = mxt224_platform_init_hw,
	//.exit_platform_hw = mxt224_platform_exit_hw,
};


static struct i2c_board_info __initdata mxt224_info[] = {
	{
		I2C_BOARD_INFO("mXT224", 0x4a),
		.platform_data = &mxt224_platform_data,
		.irq = gpio_to_irq(MXT224_INT_GPIO_PIN),
	},
};


#endif


#ifdef CONFIG_TOUCHSCREEN_TANGO
static struct TANGO_I2C_TS_t tango_plat_data = {
        .gpio_irq_pin           = TANGO_GPIO_IRQ_PIN,
        .gpio_reset_pin         = TANGO_GPIO_RESET_PIN,
        .x_max_value            = 480,
        .y_max_value            = 800,
        .layout                 = TANGO_S32_LAYOUT,
        .is_multi_touch         = IS_MULTI_TOUCH,
        .is_resetable           = 1,
        .max_finger_val         = MAX_NUM_FINGERS,
        .panel_width             = 56,
};

static struct i2c_board_info __initdata tango_info[] =
{
        {       /* New touch screen i2c slave address. */
                I2C_BOARD_INFO(I2C_TS_DRIVER_NAME, TANGO_S32_SLAVE_ADDR),
                .platform_data = &tango_plat_data,
                .irq = gpio_to_irq(TANGO_GPIO_IRQ_PIN),
        },
};

#endif

#ifdef CONFIG_TOUCHSCREEN_GT818

static struct goodix_i2c_rmi_platform_data capri_gt818_pdata = {
        .scr_x_min = 0,
        .scr_x_max = 240,
        .scr_y_min = 0,
        .scr_y_max = 320,
        .int_port = GT818_GPIO_IRQ_PIN,
	.tp_rst   = GT818_GPIO_RESET_PIN,
      /*  .init_hw = gt818_init_platform_hw,*/
        
};
static struct i2c_board_info __initdata gt818_info[] =
{
        {       /* New touch screen i2c slave address. */
                I2C_BOARD_INFO("gt818-ts", 0x5D),
                .platform_data = &capri_gt818_pdata,
                .irq = gpio_to_irq(GT818_GPIO_IRQ_PIN),
        },
};
#endif



#if defined(CONFIG_LEDS_GPIO) || defined(CONFIG_LEDS_GPIO_MODULE)
#define board_gpio_leds concatenate(CAPRI_BOARD_ID, _board_gpio_leds)
static struct gpio_led board_gpio_leds[] = GPIO_LEDS_SETTINGS;

#define leds_gpio_data concatenate(CAPRI_BOARD_ID, _leds_gpio_data)
static struct gpio_led_platform_data leds_gpio_data =
{
	.num_leds = ARRAY_SIZE(board_gpio_leds),
	.leds = board_gpio_leds,
};

#define board_leds_gpio_device concatenate(CAPRI_BOARD_ID, _leds_gpio_device)
static struct platform_device board_leds_gpio_device = {
	.name = "leds-gpio",
	.id = -1,
	.dev = {
		.platform_data = &leds_gpio_data,
	},
};
#endif

#if defined(CONFIG_KEYBOARD_GPIO) || defined(CONFIG_KEYBOARD_GPIO_MODULE)
#define board_gpio_keys concatenate(CAPRI_BOARD_ID, _board_gpio_keys)
static struct gpio_keys_button board_gpio_keys[] = GPIO_KEYS_SETTINGS;

#define gpio_keys_data concatenate(CAPRI_BOARD_ID, _gpio_keys_data)
static struct gpio_keys_platform_data gpio_keys_data =
{
	.nbuttons = ARRAY_SIZE(board_gpio_keys),
	.buttons = board_gpio_keys,
};

#define board_gpio_keys_device concatenate(CAPRI_BOARD_ID, _gpio_keys_device)
static struct platform_device board_gpio_keys_device = {
	.name = "gpio-keys",
	.id = -1,
	.dev = {
		.platform_data = &gpio_keys_data,
	},
};
#endif

#if defined(CONFIG_KEYBOARD_KONA) || defined(CONFIG_KEYBOARD_KONA_MODULE)

#define board_keypad_keymap concatenate(CAPRI_BOARD_ID, _keypad_keymap)
static struct KEYMAP board_keypad_keymap[] = HW_DEFAULT_KEYMAP;

#define board_keypad_pwroff concatenate(CAPRI_BOARD_ID, _keypad_pwroff)
static unsigned int board_keypad_pwroff[] = HW_DEFAULT_POWEROFF;

#define board_keypad_param concatenate(CAPRI_BOARD_ID, _keypad_param)
static struct KEYPAD_DATA board_keypad_param =
{
	.active_mode = 0,
	.keymap      = board_keypad_keymap,
	.keymap_cnt  = ARRAY_SIZE(board_keypad_keymap),
	.pwroff      = board_keypad_pwroff,
	.pwroff_cnt  = ARRAY_SIZE(board_keypad_pwroff),
	.clock       = GPIOKP_APB_BUS_CLK_NAME_STR,
};

#define board_keypad_device_resource concatenate(CAPRI_BOARD_ID, _keypad_device_resource)
static struct resource board_keypad_device_resource[] = {
	[0] = {
		.start = KEYPAD_BASE_ADDR,
		.end   = KEYPAD_BASE_ADDR + 0xD0,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = BCM_INT_ID_KEYPAD,
		.end   = BCM_INT_ID_KEYPAD,
		.flags = IORESOURCE_IRQ,
	},
};

#define board_keypad_device concatenate(CAPRI_BOARD_ID, _keypad_device)
static struct platform_device board_keypad_device =
{
	.name          = "kona_keypad",
	.id            = -1,
	.resource      = board_keypad_device_resource,
	.num_resources = ARRAY_SIZE(board_keypad_device_resource),
	.dev = {
		.platform_data = &board_keypad_param,
	},
};
#endif

#ifdef CONFIG_KEYBOARD_BCM
/*!
 * The keyboard definition structure.
 */
struct platform_device bcm_kp_device = {
	.name = "bcm_keypad",
	.id = -1,
};

#if defined(CONFIG_MACH_CAPRI_RAY)
static struct bcm_keymap newKeymap[] = {
	{BCM_KEY_ROW_0, BCM_KEY_COL_0, "Search Key", KEY_SEARCH},
	{BCM_KEY_ROW_0, BCM_KEY_COL_1, "Menu-Key", KEY_MENU},
	{BCM_KEY_ROW_0, BCM_KEY_COL_2, "Home-Key", KEY_HOME},
	{BCM_KEY_ROW_0, BCM_KEY_COL_3, "Back Key", KEY_BACK},
	{BCM_KEY_ROW_0, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_0, "Volumeup Key", KEY_VOLUMEUP},
	{BCM_KEY_ROW_1, BCM_KEY_COL_1, "Volumedown Key", KEY_VOLUMEDOWN},
	{BCM_KEY_ROW_1, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_7, "unused", 0},
};

static struct bcm_keypad_platform_info bcm_keypad_data = {
	.row_num = 2,
	.col_num = 4,
	.keymap = newKeymap,
	.bcm_keypad_base = (void *)__iomem HW_IO_PHYS_TO_VIRT(KEYPAD_BASE_ADDR),
};
#elif defined(CONFIG_MACH_CAPRI_SS)
static struct bcm_keymap newKeymap[] = {
	{BCM_KEY_ROW_0, BCM_KEY_COL_0, "Home-Key", KEY_HOME},
	{BCM_KEY_ROW_0, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_0, "Volumeup Key", KEY_VOLUMEUP},
	{BCM_KEY_ROW_1, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_0, "Volumedn Key", KEY_VOLUMEDOWN},
	{BCM_KEY_ROW_2, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_7, "unused", 0},
};

static struct bcm_keypad_platform_info bcm_keypad_data = {
	.row_num = 3,
	.col_num = 1,
	.keymap = newKeymap,
	.bcm_keypad_base = (void *)__iomem HW_IO_PHYS_TO_VIRT(KEYPAD_BASE_ADDR),
};
#elif defined(CONFIG_MACH_CAPRI_M500)
static struct bcm_keymap newKeymap[] = {
	{BCM_KEY_ROW_0, BCM_KEY_COL_0, "Volumeup Key", KEY_VOLUMEUP},
	{BCM_KEY_ROW_0, BCM_KEY_COL_1, "Volumedown Key", KEY_VOLUMEDOWN},
	{BCM_KEY_ROW_0, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_7, "unused", 0},
};

static struct bcm_keypad_platform_info bcm_keypad_data = {
	.row_num = 1,
	.col_num = 2,
	.keymap = newKeymap,
	.bcm_keypad_base = (void *)__iomem HW_IO_PHYS_TO_VIRT(KEYPAD_BASE_ADDR),
};
#else 
static struct bcm_keymap newKeymap[] = {
	{BCM_KEY_ROW_0, BCM_KEY_COL_0, "Volumedown Key", KEY_VOLUMEDOWN},
	{BCM_KEY_ROW_0, BCM_KEY_COL_1, "Volumeup Key", KEY_VOLUMEUP},
	{BCM_KEY_ROW_0, BCM_KEY_COL_2, "Search Key", KEY_SEARCH},
	{BCM_KEY_ROW_0, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_0, "Home-Key", KEY_HOME},
	{BCM_KEY_ROW_1, BCM_KEY_COL_1, "Back Key", KEY_BACK},
	{BCM_KEY_ROW_1, BCM_KEY_COL_2, "Menu-Key", KEY_MENU},
	{BCM_KEY_ROW_1, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_7, "unused", 0},
};

static struct bcm_keypad_platform_info bcm_keypad_data = {
	.row_num = 2,
	.col_num = 3,
	.keymap = newKeymap,
	.bcm_keypad_base = (void *)__iomem HW_IO_PHYS_TO_VIRT(KEYPAD_BASE_ADDR),
};
#endif

#endif

#if defined(CONFIG_BCM_AAA) || defined(CONFIG_BCM_AAA_MODULE)
static struct platform_device board_bcm_aaa_device = {
	.name = "bcm-aaa",
	.id = -1,
};
#endif

#if defined(CONFIG_BCM_MEMC) || defined(CONFIG_BCM_MEMC_MODULE)
static struct platform_device board_bcm_memc_device = {
	.name = "bcm-memc",
	.id = -1,
};
#endif

#if defined(CONFIG_BCM_WFD) || defined(CONFIG_BCM_WFD_MODULE)
static struct platform_device board_bcm_wfd_device = {
	.name = "bcm-wfd",
	.id = -1,
};
#endif

#if defined(CONFIG_BCM_GPS) || defined(CONFIG_BCM_GPS_MODULE)
#define board_hana_gps_info concatenate(CAPRI_BOARD_ID, _board_hana_gps_info)
static struct gps_platform_data board_hana_gps_info = GPS_PLATFORM_DATA_SETTINGS;

#define platform_device_gps concatenate(CAPRI_BOARD_ID, _platform_device_gps)
static struct platform_device platform_device_gps =
{
	.name = "gps",
	.id = -1,
	.dev = {
		.platform_data = &board_hana_gps_info,
	},
};
#endif

#if defined(CONFIG_BCM_HAPTICS) || defined(CONFIG_BCM_HAPTICS_MODULE)
#define board_bcm_haptics_device concatenate(CAPRI_BOARD_ID, _bcm_haptics_device)

#define board_bcm_haptics_data concatenate(CAPRI_BOARD_ID, _board_bcm_haptics_data)
static struct bcm_haptics_data board_bcm_haptics_data = BCM_HAPTICS_SETTINGS;

static struct platform_device board_bcm_haptics_device = {
	.name = BCM_HAPTICS_DRIVER_NAME,
	.id = -1,
	.dev = {
		.platform_data = &board_bcm_haptics_data,
	},
};
#endif

#if defined(CONFIG_KONA_OTG_CP) || defined(CONFIG_KONA_OTG_CP_MODULE)
static struct resource otg_cp_resource[] = {
	[0] = {
		.start = HSOTG_CTRL_BASE_ADDR,
		.end = HSOTG_CTRL_BASE_ADDR + OTG_CTRL_CORE_REG_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = BCM_INT_ID_USB_OTG_DRV_VBUS,
		.end = BCM_INT_ID_USB_OTG_DRV_VBUS,
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device otg_cp_device =
{
	.name = "kona-otg-cp",
	.id = -1,
	.resource = otg_cp_resource,
	.num_resources = ARRAY_SIZE(otg_cp_resource),
};
#endif

#if defined(CONFIG_KONA_VCHIQ) || defined(CONFIG_KONA_VCHIQ_MODULE)

/****************************************************************************
*
*   VCHIQ display device
*
***************************************************************************/
#if defined( CONFIG_KONA_VCHIQ_MEMDRV ) || defined( CONFIG_KONA_VCHIQ_MEMDRV_MODULE )

/*
 * Internal videocore using the vchiq_arm stack
 */

#define  VCHIQ_DISPLAY_DEVICE

#define IPC_SHARED_CHANNEL_VIRT     ( KONA_INT_SRAM_BASE + BCMHANA_ARAM_VC_OFFSET )
#define IPC_SHARED_CHANNEL_PHYS     ( INT_SRAM_BASE + BCMHANA_ARAM_VC_OFFSET )

static VCHIQ_PLATFORM_MEMDRV_KONA_DATA_GPIO_T vchiq_display_data_memdrv_kona_gpio_config[] =
{
   HW_CFG_CAMERA_GPIO
};

static VCHIQ_PLATFORM_DATA_MEMDRV_KONA_T vchiq_display_data_memdrv_kona = {
    .memdrv = {
        .common = {
            .instance_name = "display",
            .dev_type      = VCHIQ_DEVICE_TYPE_SHARED_MEM,
        },
        .sharedMemVirt  = (void *)(IPC_SHARED_CHANNEL_VIRT),
        .sharedMemPhys  = IPC_SHARED_CHANNEL_PHYS,
    },
    .ipcIrq                =  BCM_INT_ID_IPC_OPEN,
    .num_gpio_configs      =  ARRAY_SIZE( vchiq_display_data_memdrv_kona_gpio_config ),
    .gpio_config           = vchiq_display_data_memdrv_kona_gpio_config,
};

static struct platform_device vchiq_display_device = {
    .name = "vchiq_memdrv_kona",
    .id = 0,
    .dev = {
        .platform_data = &vchiq_display_data_memdrv_kona,
    },
};

#elif defined( CONFIG_VC_VCHIQ_BUSDRV_SHAREDMEM ) || defined( CONFIG_VC_VCHIQ_BUSDRV_SHAREDMEM_MODULE )

/*
 * Internal videocore using the vchiq stack.
 */

#define  VCHIQ_DISPLAY_DEVICE

static VCHIQ_PLATFORM_DATA_KONA_T vchiq_display_data_shared_mem = {
    .common = {
        .instance_name  = "display",
        .dev_type       = VCHIQ_DEVICE_TYPE_HOST_PORT,
    },
};

static struct platform_device vchiq_display_device = {
    .name = "vchiq_busdrv_sharedmem",
    .id = 0,
    .dev = {
        .platform_data = &vchiq_display_data_shared_mem,
    },
};

#endif

struct platform_device * vchiq_devices[] __initdata =
{
#if defined( VCHIQ_DISPLAY_DEVICE )
	&vchiq_display_device,
#endif

#ifdef CONFIG_KEYBOARD_BCM
	&bcm_kp_device,
#endif
};

#endif  /* CONFIG_KONA_VCHIQ */

#if defined(CONFIG_MPU_SENSORS_MPU6050B1) || defined(CONFIG_MPU_SENSORS_MPU6050B1_MODULE)



#define mpu6050_platform_data concatenate(CAPRI_BOARD_ID, _mpu6050_data)

static struct bcm_mpu_platform_data bcm_mpu6050_platform_data =
{
	.base_data = {
		.int_config  = MPU6050_INIT_CFG,
		.level_shifter = 0,
		.orientation = MPU6050_DRIVER_ACCEL_GYRO_ORIENTATION,
	},
	.irq_gpio = MPU6050_IRQ_GPIO,
};


static struct i2c_board_info __initdata inv_mpu_i2c0_boardinfo[] =
{
	{
		I2C_BOARD_INFO("mpu6050", MPU6050_SLAVE_ADDR),
		.platform_data = &bcm_mpu6050_platform_data,
	},
};
#endif /* CONFIG_MPU_SENSORS_MPU6050B1 */

#if defined(CONFIG_BMP18X_I2C) || defined(CONFIG_BMP18X_I2C_MODULE)
static struct i2c_board_info __initdata i2c_bmp18x_info[] =
{
	{
		I2C_BOARD_INFO(BMP18X_NAME, BMP18X_I2C_ADDRESS),
	},
};
#endif

#if defined(CONFIG_AL3006) || defined(CONFIG_AL3006_MODULE)
static struct al3006_platform_data al3006_pdata = {
#ifdef AL3006_IRQ_GPIO
	.irq_gpio = AL3006_IRQ_GPIO,
#else
	.irq_gpio = -1,
#endif
};

static struct i2c_board_info __initdata i2c_al3006_info[] = {
	{
		I2C_BOARD_INFO("al3006", AL3006_I2C_ADDRESS),
		.platform_data = &al3006_pdata,
	},
};
#endif

#if defined(CONFIG_BCM2079x_NFC)

static struct bcmi2cnfc_i2c_platform_data bcmi2cnfc_pdata = {
	.irq_gpio = BCMI2CNFC_IRQ,
	.en_gpio = BCMI2CNFC_EN,
	.wake_gpio = BCMI2CNFC_WAKE,
//	.i2c_pdata	= {ADD_I2C_SLAVE_SPEED(BSC_BUS_SPEED_400K),},
};



static struct i2c_board_info __initdata bcmi2cnfc[] = {
	{
	 I2C_BOARD_INFO("bcmi2cnfc", BCMI2CNFC_ADDR),
	 .flags = I2C_CLIENT_TEN,
	 .platform_data = (void *)&bcmi2cnfc_pdata,
	 .irq = gpio_to_irq(BCMI2CNFC_IRQ),
	 },

};
#endif

#if defined(CONFIG_AMI306) || defined(CONFIG_AMI306_MODULE)
static struct ami306_platform_data ami306_data = AMI306_DATA;
static struct i2c_board_info __initdata i2c_ami306_info[] =
{
	{
		I2C_BOARD_INFO(AMI_DRV_NAME, AMI_I2C_ADDRESS),
		.platform_data = &ami306_data,
	},
};
#endif

#if defined(CONFIG_BCM_RFKILL) || defined(CONFIG_BCM_RFKILL_MODULE)
#define board_bcmbt_rfkill_cfg concatenate(CAPRI_BOARD_ID, _bcmbt_rfkill_cfg)
static struct bcmbt_rfkill_platform_data board_bcmbt_rfkill_cfg =
{
#if defined(BCMBT_VREG_GPIO)
	.vreg_gpio = BCMBT_VREG_GPIO,
#else
	.vreg_gpio = -1,
#endif
	.n_reset_gpio = -1,
	.aux0_gpio = -1,
	.aux1_gpio = -1
};
#define board_bcmbt_rfkill_device concatenate(CAPRI_BOARD_ID, _bcmbt_rfkill_device)
static struct platform_device board_bcmbt_rfkill_device =
{
	.name = "bcmbt-rfkill",
	.id = 1,
	.dev =
	{
		.platform_data = &board_bcmbt_rfkill_cfg,
	},
};

static void __init board_add_bcmbt_rfkill_device(void)
{
	platform_device_register(&board_bcmbt_rfkill_device);
}
#endif


#ifdef CONFIG_BCM_BZHW
#define GPIO_BT_WAKE 97
#define GPIO_HOST_WAKE 96
#define board_bcm_bzhw_data concatenate(CAPRI_BOARD_ID, _bcm_bzhw_data)
static struct bcm_bzhw_platform_data bcm_bzhw_data = {
	.gpio_bt_wake = GPIO_BT_WAKE,
	.gpio_host_wake = GPIO_HOST_WAKE,
};

#define board_bcm_bzhw_device concatenate(CAPRI_BOARD_ID, _bcm_bzhw_device)
static struct platform_device board_bcm_bzhw_device = {
	.name = "bcm_bzhw",
	.id = -1,
	.dev = {
		.platform_data = &bcm_bzhw_data,
		},
};

static void __init board_add_bcm_bzhw_device(void)
{
	platform_device_register(&board_bcm_bzhw_device);
}
#endif


#if defined(CONFIG_BCM_BT_LPM) || defined(CONFIG_BCM_BT_LPM_MODULE)
#define board_bcmbt_lpm_cfg concatenate(CAPRI_BOARD_ID, _bcmbt_lpm_cfg)
static struct bcm_bt_lpm_platform_data board_bcmbt_lpm_cfg =
{
	.gpio_bt_wake = GPIO_BT_WAKE,
	.gpio_host_wake = GPIO_HOST_WAKE,
};
#define board_bcmbt_lpm_device concatenate(CAPRI_BOARD_ID, _bcmbt_lpm_device)
static struct platform_device board_bcmbt_lpm_device =
{
	.name = "bcmbt-lpm",
	.id = -1,
	.dev =
	{
		.platform_data = &board_bcmbt_lpm_cfg,
	},
};

static void __init board_add_bcmbt_lpm_device(void)
{
	platform_device_register(&board_bcmbt_lpm_device);
}
#endif

static void __init add_sdio_device(void)
{
	unsigned int i, id, num_devices;

	num_devices = ARRAY_SIZE(sdio_param);
	if (num_devices > SDIO_MAX_NUM_DEVICES)
		num_devices = SDIO_MAX_NUM_DEVICES;

	/*
	 * Need to register eMMC as the first SDIO device so it grabs mmcblk0 when
	 * it's installed. This required for rootfs to be mounted properly
	 *
	 * Ask Darwin for why we need to do this
	 */
	for (i = 0; i < num_devices; i++)
	{
		id = sdio_param[i].id;
		if (id < SDIO_MAX_NUM_DEVICES)
		{
			if (sdio_param[i].devtype == SDIO_DEV_TYPE_EMMC)
			{
				sdio_devices[id].dev.platform_data = &sdio_param[i];
				platform_device_register(&sdio_devices[id]);
			}
		}
	}

	for (i = 0; i < num_devices; i++)
	{
		id = sdio_param[i].id;

		/* skip eMMC as it has been registered */
		if (sdio_param[i].devtype == SDIO_DEV_TYPE_EMMC)
			continue;

		if (id < SDIO_MAX_NUM_DEVICES)
		{
			if (sdio_param[i].devtype == SDIO_DEV_TYPE_WIFI)
			{
				struct sdio_wifi_gpio_cfg *wifi_gpio =
					&sdio_param[i].wifi_gpio;

#ifdef HW_WLAN_GPIO_RESET_PIN
				wifi_gpio->reset = HW_WLAN_GPIO_RESET_PIN;
#else
				wifi_gpio->reset = -1;
#endif
#ifdef HW_WLAN_GPIO_SHUTDOWN_PIN
				wifi_gpio->shutdown = HW_WLAN_GPIO_SHUTDOWN_PIN;
#else
				wifi_gpio->shutdown = -1;
#endif
#ifdef HW_WLAN_GPIO_REG_PIN
				wifi_gpio->reg = HW_WLAN_GPIO_REG_PIN;
#else
				wifi_gpio->reg = -1;
#endif
#ifdef HW_WLAN_GPIO_HOST_WAKE_PIN
				wifi_gpio->host_wake = HW_WLAN_GPIO_HOST_WAKE_PIN;
#else
				wifi_gpio->host_wake = -1;
#endif
			}
			sdio_devices[id].dev.platform_data = &sdio_param[i];
			platform_device_register(&sdio_devices[id]);
		}
	}
}

static void __init add_i2c_device(void)
{
	unsigned int i, num_devices;

	num_devices = ARRAY_SIZE(i2c_adap_param);
	if (num_devices == 0)
		return;
	if (num_devices > MAX_I2C_ADAPS)
		num_devices = MAX_I2C_ADAPS;

	for (i = 0; i < num_devices; i++) {
		/* DO NOT register the I2C device if it is disabled */
		if (i2c_adap_param[i].disable == 1)
			continue;

		i2c_adap_devices[i].dev.platform_data = &i2c_adap_param[i];
		platform_device_register(&i2c_adap_devices[i]);
	}

#if defined(CONFIG_TOUCHSCREEN_EGALAX_I2C) || defined(CONFIG_TOUCHSCREEN_EGALAX_I2C_MODULE)
#ifdef HW_EGALAX_I2C_BUS_ID
	egalax_i2c_param.id = HW_EGALAX_I2C_BUS_ID;
#endif

#ifdef HW_EGALAX_GPIO_RESET
	egalax_i2c_param.gpio.reset = HW_EGALAX_GPIO_RESET;
#endif

#ifdef HW_EGALAX_GPIO_EVENT
	egalax_i2c_param.gpio.event = HW_EGALAX_GPIO_EVENT;
#endif

#ifdef HW_EGALAX_RESET_MSECS
   egalax_i2c_param.reset_time = HW_EGALAX_RESET_MSECS;
#endif

#ifdef HW_EGALAX_RESET_LEVEL
   egalax_i2c_param.reset_level = HW_EGALAX_RESET_LEVEL;
#endif

	egalax_i2c_boardinfo[0].irq =
		gpio_to_irq(egalax_i2c_param.gpio.event);

	i2c_register_board_info(egalax_i2c_param.id, egalax_i2c_boardinfo,
			ARRAY_SIZE(egalax_i2c_boardinfo));
#endif

#if defined(CONFIG_TOUCHSCREEN_BCM915500) || defined(CONFIG_TOUCHSCREEN_BCM915500_MODULE)
	printk(KERN_INFO "PPTEST %s() bcm915500_i2c_boardinfo[0].irq: %d\n",
		__func__, bcm915500_i2c_boardinfo[0].irq);

	i2c_register_board_info(bcm915500_i2c_platform_data.i2c_bus_id,
				bcm915500_i2c_boardinfo,
				ARRAY_SIZE(bcm915500_i2c_boardinfo));
#endif

#ifdef CONFIG_TOUCHSCREEN_TANGO
	i2c_register_board_info(TANGO_I2C_BUS_ID,
		tango_info,
		ARRAY_SIZE(tango_info));
#endif

#ifdef CONFIG_TOUCHSCREEN_QT602240
	i2c_register_board_info(1,
			qt602240_info,
			ARRAY_SIZE(qt602240_info));
#endif

#ifdef CONFIG_USB_SWITCH_FSA9485
	pr_info("fsa9485\n");
	i2c_register_board_info(FSA9485_I2C_BUS_ID, micro_usb_i2c_devices_info,ARRAY_SIZE(micro_usb_i2c_devices_info));
#endif

#ifdef CONFIG_TOUCHSCREEN_GT818
        i2c_register_board_info(GT818_I2C_BUS_ID,
                gt818_info,
                ARRAY_SIZE(gt818_info));
#endif

#ifdef CONFIG_TOUCHSCREEN_ATMEL_MXT
i2c_register_board_info(1,
			mxt224_info,
			ARRAY_SIZE(mxt224_info));
#endif

#ifdef CONFIG_TOUCHSCREEN_ATMEL_MXT224
	board_tsp_init();
#endif

#ifdef CONFIG_SAMSUNG_MHL
	board_mhl_init();
#endif


#if defined(CONFIG_MPU_SENSORS_MPU6050B1) || defined(CONFIG_MPU_SENSORS_MPU6050B1_MODULE)
#if defined(MPU6050_IRQ_GPIO)
	inv_mpu_i2c0_boardinfo[0].irq = gpio_to_irq(MPU6050_IRQ_GPIO);
#endif
	i2c_register_board_info(MPU6050_I2C_BUS_ID,
		inv_mpu_i2c0_boardinfo, ARRAY_SIZE(inv_mpu_i2c0_boardinfo));
#endif

#if defined(CONFIG_BMP18X_I2C) || defined(CONFIG_BMP18X_I2C_MODULE)
	i2c_register_board_info(
#ifdef BMP18X_I2C_BUS_ID
			BMP18X_I2C_BUS_ID,
#else
			-1,
#endif
			i2c_bmp18x_info, ARRAY_SIZE(i2c_bmp18x_info));
#endif

#if defined(CONFIG_AL3006) || defined(CONFIG_AL3006_MODULE)
#ifdef AL3006_IRQ
	i2c_al3006_info[0].irq = gpio_to_irq(AL3006_IRQ_GPIO);
#else
	i2c_al3006_info[0].irq = -1;
#endif
	i2c_register_board_info(
#ifdef AL3006_I2C_BUS_ID
		AL3006_I2C_BUS_ID,
#else
		-1,
#endif
		i2c_al3006_info, ARRAY_SIZE(i2c_al3006_info));
#endif /* CONFIG_AL3006 */

#if defined(CONFIG_AMI306) || defined(CONFIG_AMI306_MODULE)
	i2c_register_board_info (
#ifdef AMI_I2C_BUS_NUM
		AMI_I2C_BUS_NUM,
#else
		-1,
#endif
		i2c_ami306_info, ARRAY_SIZE(i2c_ami306_info));
#endif /* CONFIG_AMI306 */

#if defined(CONFIG_BCM2079x_NFC)
	i2c_register_board_info(BCMI2CNFC_BUSID, bcmi2cnfc, ARRAY_SIZE(bcmi2cnfc));
#endif

}

#if defined(CONFIG_LEDS_GPIO) || defined(CONFIG_LEDS_GPIO_MODULE)
#define board_add_led_device concatenate(CAPRI_BOARD_ID, _add_led_device)
static void __init board_add_led_device(void)
{
	platform_device_register(&board_leds_gpio_device);
}
#endif

#if defined(CONFIG_KEYBOARD_GPIO) || defined(CONFIG_KEYBOARD_GPIO_MODULE)
#define board_add_keys_device concatenate(CAPRI_BOARD_ID, _add_keyboard_device)
static void __init board_add_keys_device(void)
{
	platform_device_register(&board_gpio_keys_device);
}
#endif

#if defined(CONFIG_KEYBOARD_KONA) || defined(CONFIG_KEYBOARD_KONA_MODULE)
#define board_add_keyboard_kona concatenate(CAPRI_BOARD_ID, _add_keyboard_kona)
static void __init board_add_keyboard_kona(void)
{
	platform_device_register(&board_keypad_device);
}
#endif


static void __init add_usbh_device(void)
{
	/*
	 * Always register the low level USB host device before EHCI/OHCI
	 * devices. Also, always add EHCI device before OHCI
	 */
#if defined(CONFIG_USB_BCM_CAPRI)
	platform_device_register(&usbh_device);
#endif
#if defined(CONFIG_USB_BCM_CAPRI_HSIC)
	platform_device_register(&usbh_hsic_device);
#endif
#if defined(CONFIG_USB_EHCI_BCM)
	platform_device_register(&usbh_ehci_device);
#endif
#if defined(CONFIG_USB_OHCI_BCM)
	platform_device_register(&usbh_ohci_device);
#endif
#if defined(CONFIG_USB_EHCI_BCM) && defined(CONFIG_USB_BCM_CAPRI_HSIC)
	platform_device_register(&usbh_hsic2_ehci_device);
#endif
}

static void __init add_usb_otg_device(void)
{
#if defined(CONFIG_KONA_OTG_CP) || defined(CONFIG_KONA_OTG_CP_MODULE)
	platform_device_register(&otg_cp_device);
#endif
}

#if defined(CONFIG_BCM_HALAUDIO)
#define board_halaudio_dev_list concatenate(CAPRI_BOARD_ID, _halaudio_dev_list)
static HALAUDIO_DEV_CFG board_halaudio_dev_list[] =
#ifdef HALAUDIO_DEV_LIST
	HALAUDIO_DEV_LIST;
#else
	NULL;
#endif

#define board_halaudio_cfg concatenate(CAPRI_BOARD_ID, _halaudio_cfg)
static HALAUDIO_CFG board_halaudio_cfg;

#define board_halaudio_device concatenate(CAPRI_BOARD_ID, _halaudio_device)
static struct platform_device board_halaudio_device =
{
	.name = "bcm-halaudio",
	.id = -1, /* to indicate there's only one such device */
	.dev =
	{
		.platform_data = &board_halaudio_cfg,
	},
};

#define board_halaudio_audio_info concatenate(CAPRI_BOARD_ID, _halaudio_audioh_info)
static HALAUDIO_AUDIOH_PLATFORM_INFO board_halaudio_audioh_info =
{
	.spk_en_gpio =
	{
#ifdef HALAUDIO_AUDIOH_SETTINGS_GPIO_HANDSFREE_LEFT_EN
		.handsfree_left_en = HALAUDIO_AUDIOH_SETTINGS_GPIO_HANDSFREE_LEFT_EN,
#else
		.handsfree_left_en = -1,
#endif
#ifdef HALAUDIO_AUDIOH_SETTINGS_GPIO_HANDSFREE_RIGHT_EN
		.handsfree_right_en = HALAUDIO_AUDIOH_SETTINGS_GPIO_HANDSFREE_RIGHT_EN,
#else
		.handsfree_right_en = -1,
#endif

#ifdef HALAUDIO_AUDIOH_SETTINGS_GPIO_HEADSET_EN
		.headset_en = HALAUDIO_AUDIOH_SETTINGS_GPIO_HEADSET_EN,
#else
		.headset_en = -1,
#endif
	},

#ifdef HALAUDIO_AUDIOH_SETTINGS_EARPIECE_SPARE_BIT_EN
	.earpiece_spare_bit_en = HALAUDIO_AUDIOH_SETTINGS_EARPIECE_SPARE_BIT_EN,
#else
	.earpiece_spare_bit_en = 1,
#endif

#ifdef HALAUDIO_AUDIOH_SETTINGS_MIC_DATA_LPBK_EN
   .mic_data_lpbk_en = HALAUDIO_AUDIOH_SETTINGS_MIC_DATA_LPBK_EN,
#else
   .mic_data_lpbk_en = 1,
#endif

};

#define board_halaudio_audioh_device concatenate(CAPRI_BOARD_ID, _halaudio_audioh_device)
static struct platform_device board_halaudio_audioh_device =
{
	.name = "bcm-halaudio-audioh",
	.id = -1, /* to indicate there's only one such device */
	.dev =
	{
		.platform_data = &board_halaudio_audioh_info,
	},
};

#define board_halaudio_pcm_info concatenate(CAPRI_BOARD_ID, _halaudio_pcm_info)
static HALAUDIO_PCM_PLATFORM_INFO board_halaudio_pcm_info =
{
#ifdef HALAUDIO_PCM_SETTINGS_CORE_ID_SELECT
	.core_id_select = HALAUDIO_PCM_SETTINGS_CORE_ID_SELECT,
#else
	.core_id_select = -1,
#endif
#ifdef HALAUDIO_PCM_SETTINGS_CHANS_SUPPORTED
	.channels = HALAUDIO_PCM_SETTINGS_CHANS_SUPPORTED,
#endif
#ifdef HALAUDIO_PCM_SETTINGS_CHAN_SELECT
	.channel_select = HALAUDIO_PCM_SETTINGS_CHAN_SELECT,
#endif
	.bt_gpio =
	{
#ifdef HALAUDIO_PCM_SETTINGS_GPIO_BT_RST_B
		.rst_b = HALAUDIO_PCM_SETTINGS_GPIO_BT_RST_B,
#else
		.rst_b = -1,
#endif
#ifdef HALAUDIO_PCM_SETTINGS_GPIO_BT_VREG_CTL
		.vreg_ctl = HALAUDIO_PCM_SETTINGS_GPIO_BT_VREG_CTL,
#else
		.vreg_ctl = -1,
#endif
#ifdef HALAUDIO_PCM_SETTINGS_GPIO_BT_WAKE
		.wake = HALAUDIO_PCM_SETTINGS_GPIO_BT_WAKE,
#else
		.wake = -1,
#endif
	},
#ifdef HALAUDIO_PCM_SETTINGS_BT_REQ_UART_GPIO_GROUP
	.bt_req_uart_gpio_group = HALAUDIO_PCM_SETTINGS_BT_REQ_UART_GPIO_GROUP,
#else
	.bt_req_uart_gpio_group = -1,
#endif
};

#ifndef HALAUDIO_PCM_SETTINGS_GPIO_BT_RST_B
#define BT_SUPPORT    0
#else
#define BT_SUPPORT    1
#endif

#define board_halaudio_pcm_device concatenate(CAPRI_BOARD_ID, _halaudio_pcm_device)
static struct platform_device board_halaudio_pcm_device =
{
	.name = "bcm-halaudio-pcm",
	.id = -1, /* to indicate there's only one such device */
	.dev =
	{
		.platform_data = &board_halaudio_pcm_info,
	},
};

#define board_add_halaudio_device concatenate(CAPRI_BOARD_ID, _add_halaudio_device)
static void __init board_add_halaudio_device(void)
{
	board_halaudio_cfg.numdev = ARRAY_SIZE(board_halaudio_dev_list);
	board_halaudio_cfg.devlist = board_halaudio_dev_list;
	platform_device_register(&board_halaudio_device);
	platform_device_register(&board_halaudio_audioh_device);
	platform_device_register(&board_halaudio_pcm_device);
}
#endif /* CONFIG_BCM_HALAUDIO */

#ifdef CONFIG_WD_TAPPER
static struct wd_tapper_platform_data wd_tapper_data = {
        /* Set the count to the time equivalent to the time-out in milliseconds
         * required to pet the PMU watchdog to overcome the problem of reset in
         * suspend*/
        .count = 120000,
        .ch_num = 1,
        .name = "aon-timer",
};

static struct platform_device wd_tapper = {
        .name = "wd_tapper",
        .id = 0,
        .dev = {
                .platform_data = &wd_tapper_data,
                },
};
#endif

static void __init add_devices(void)
{
#if defined(HW_I2C_ADAP_PARAM)
	add_i2c_device();
#endif

#if defined(HW_SDIO_PARAM)
	add_sdio_device();
#endif

#ifdef CONFIG_BRCM_UNIFIED_DHD_SUPPORT
	printk(KERN_ERR "Calling WLAN_INIT!\n");
	capri_wlan_init();
	printk(KERN_ERR "DONE WLAN_INIT!\n");
#endif

#if defined(CONFIG_LEDS_GPIO) || defined(CONFIG_LEDS_GPIO_MODULE)
	board_add_led_device();
#endif

#if defined(CONFIG_BCM_HEADSET_SW)
	board_add_headsetdet_device();
#endif

#if defined(CONFIG_KONA_HEADSET) || defined(CONFIG_KONA_HEADSET_MULTI_BUTTON)
	platform_device_register(&headset_device);
#endif

#if defined(CONFIG_KEYBOARD_GPIO) || defined(CONFIG_KEYBOARD_GPIO_MODULE)
	board_add_keys_device();
#endif

#if defined(CONFIG_KEYBOARD_KONA) || defined(CONFIG_KEYBOARD_KONA_MODULE)
	board_add_keyboard_kona();
#endif

#ifdef CONFIG_KEYBOARD_BCM
	bcm_kp_device.dev.platform_data = &bcm_keypad_data;
#endif

#if defined(CONFIG_BCM_RFKILL) || defined(CONFIG_BCM_RFKILL_MODULE)
        board_add_bcmbt_rfkill_device();
#endif

#ifdef CONFIG_BCM_BZHW
	board_add_bcm_bzhw_device();
#endif

#if defined(CONFIG_BCM_BT_LPM) || defined(CONFIG_BCM_BT_LPM_MODULE)
        board_add_bcmbt_lpm_device();
#endif

#if defined(CONFIG_BCM_HAPTICS) || defined(CONFIG_BCM_HAPTICS_MODULE)
	platform_device_register(&board_bcm_haptics_device);
#endif

	add_usbh_device();
	add_usb_otg_device();

#if defined(CONFIG_BCM_HDMI_DET) || defined(CONFIG_BCM_HDMI_DET_MODULE)
	board_add_hdmidet_device();
#endif

#if defined(CONFIG_TFT_PANEL) || defined(CONFIG_TFT_PANEL_MODULE)
	board_add_tft_panel_device();
#endif

#if defined(CONFIG_BCM_HALAUDIO)
	board_add_halaudio_device();
#endif

#if defined(CONFIG_I2C_GPIO)
#if defined(CONFIG_TOUCHSCREEN_GT818)
       platform_device_register(&capri_i2c_gpio_device);
#endif
	platform_add_devices(gpio_i2c_devices, ARRAY_SIZE(gpio_i2c_devices));
#endif

#ifdef CONFIG_USB_SWITCH_FSA9485
	pr_info("fsa9485 mUSB_i2c_devices\n");
	platform_add_devices(mUSB_i2c_devices, ARRAY_SIZE(mUSB_i2c_devices));
#endif

#if defined(CONFIG_NET_ISLAND)
	platform_device_register(&net_device);
#endif

#if defined(CONFIG_BCM_AAA) || defined(CONFIG_BCM_AAA_MODULE)
	platform_device_register(&board_bcm_aaa_device);
#endif

#if defined(CONFIG_BCM_GPS) || defined(CONFIG_BCM_GPS_MODULE)
	platform_device_register(&platform_device_gps);
#endif

#if defined(CONFIG_KONA_VCHIQ) || defined(CONFIG_KONA_VCHIQ_MODULE)
   platform_add_devices( vchiq_devices, ARRAY_SIZE( vchiq_devices ) );
#endif

#if defined(CONFIG_BCM_MEMC) || defined(CONFIG_BCM_MEMC_MODULE)
	platform_device_register(&board_bcm_memc_device);
#endif

#if defined(CONFIG_WD_TAPPER)
	platform_device_register(&wd_tapper);
#endif

#if defined(CONFIG_BCM_WFD) || defined(CONFIG_BCM_WFD_MODULE)
	platform_device_register(&board_bcm_wfd_device);
#endif
}

static void capri_poweroff(void)
{
#ifdef CONFIG_MFD_BCMPMU
        bcmpmu_client_power_off();
#endif

	while(1);
}

#if defined(CONFIG_LCD_LD9040) && defined(CONFIG_SPI_SSPI_KONA)
#include <linux/spi/spi.h>
#include <linux/lcd.h>
#include <linux/delay.h>

#define AMOLED_RESET    (18)

int ld9040_reset(struct lcd_device *ld)
{
	printk(KERN_INFO "%s:+\n", __func__);

	if (gpio_request(AMOLED_RESET, "AmoledRst")) {
		printk(KERN_ERR "%s: failed to get (AmoledRst) gpio\n",
		       __func__);
		return -1;
	}
	gpio_direction_output(AMOLED_RESET, 1);
	gpio_set_value(AMOLED_RESET, 1);
	msleep(1);
	gpio_set_value(AMOLED_RESET, 0);
	msleep(1);
	gpio_set_value(AMOLED_RESET, 1);
	msleep(5);
	gpio_free(AMOLED_RESET);
	
	return 0;
}

int ld9040_power_on(struct lcd_device *ld, int enable)
{
	printk(KERN_INFO "%s: enable[%d]\n", __func__, enable);
	
	if(0 == enable) {
		/* keep lcd in reset */
		if (gpio_request(AMOLED_RESET, "AmoledRst")) {
			printk(KERN_ERR "%s: failed to get (AmoledRst) gpio\n",
			       __func__);
			return -1;
		}
		gpio_direction_output(AMOLED_RESET, 0);
		gpio_set_value(AMOLED_RESET, 0);
		gpio_free(AMOLED_RESET);
	}
	return 0;
}

static struct lcd_platform_data ld9040_platform_data = {
	.reset			= ld9040_reset,
	.power_on		= ld9040_power_on,
	.lcd_enabled		= 0,
	.reset_delay		= 1,
	.power_on_delay		= 1,
	.power_off_delay	= 1,
	.pdata			= NULL,
};

static struct spi_board_info ld9040_spi_device[] __initdata = {
	{
		.modalias	= "ld9040",
		.max_speed_hz	= 1000000,
		.chip_select	= 0,
		.mode           = SPI_MODE_3 | SPI_3WIRE,
		.platform_data	= &ld9040_platform_data,
	},
};

static void __init oled_lcd_init(void)
{
	spi_register_board_info(ld9040_spi_device,
		ARRAY_SIZE(ld9040_spi_device));
}
#endif

static void __init board_init(void)
{
#if defined(CONFIG_MFD_BCMPMU)
	pm_power_off = capri_poweroff;
#endif
	
#if defined(CONFIG_MAP_DMA_MMAP)
	dma_mmap_init();
#endif
#if defined(CONFIG_MAP_SDMA)
	sdma_init();
#endif

#if defined(CONFIG_LITTLE_MEMC_QOS)
	memc_qos_init();
#endif

#if defined(CONFIG_BCM_VC_CMA)
	vc_cma_early_init();
#endif

	/*
	 * Add common platform devices that do not have board dependent HW
	 * configurations
	 */
	board_add_common_devices();

	/* add devices with board dependent HW configurations */
	add_devices();
#if defined(CONFIG_LCD_LD9040) && defined(CONFIG_SPI_SSPI_KONA)
	oled_lcd_init();
#endif
}

static void __init board_reserve(void)
{
#if defined(CONFIG_BCM_VC_CMA)
	vc_cma_reserve();
#endif
}

/*
 * Template used by board-xxx.c to create new board instance
 */
#ifdef CONFIG_ZONE_DMA
#define CREATE_BOARD_INSTANCE(id,name) \
MACHINE_START(id, name) \
	.map_io = capri_map_io, \
	.init_irq = kona_init_irq, \
	.timer  = &kona_timer, \
	.init_machine = board_init, \
	.reserve = board_reserve, \
	.dma_zone_size = SZ_128M, \
MACHINE_END
#else
#define CREATE_BOARD_INSTANCE(id,name) \
MACHINE_START(id, name) \
	.map_io = capri_map_io, \
	.init_irq = kona_init_irq, \
	.timer  = &kona_timer, \
	.init_machine = board_init, \
	.reserve = board_reserve, \
MACHINE_END
#endif
