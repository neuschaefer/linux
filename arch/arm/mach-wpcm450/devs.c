/* linux/arch/arm/mach-wpcm450/devs.c
 *
 * Copyright (c) 2006 Winbond Electronics
 * All rights reserved.
 *
 * shirley yu (clyu2@winbond.com)
 *
 * Base WPCM450 platform device definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Changelog:
 *
 *   21-Aug-2006  clyu created file
 *
*/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/platform_device.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>
#include <asm/arch/fb.h>
#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/irq.h>

#include <asm/arch/regs-serial.h>

#include "devs.h"
/****************************************/
/*for wpcm450 nand by ns24 zswan 2007*/
#include <linux/mtd/mtd.h> 
#include <linux/mtd/nand.h> 
#include <linux/mtd/nand_ecc.h> 
#include <linux/mtd/partitions.h> 
#include <asm/arch/nand.h> 
/****************************************/

/* Serial port registrations */



#ifdef AVI_REMOVE
/* USB Host Controller */

static struct resource wpcm_usb_resource[] = {
	[0] = {
		.start = WPCM450_PA_USBOHCIHOST,
		.end   = WPCM450_PA_USBOHCIHOST + WPCM450_SZ_USBOHCIHOST - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_USBH,
		.end   = IRQ_USBH,
		.flags = IORESOURCE_IRQ,
	}
};

static u64 wpcm_device_usb_dmamask = 0xffffffffUL;

struct platform_device wpcm_device_usb = {
	.name		  = "wpcm450-ehci",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(wpcm_usb_resource),
	.resource	  = wpcm_usb_resource,
	.dev              = {
		.dma_mask = &wpcm_device_usb_dmamask,
		.coherent_dma_mask = 0xffffffffUL
	}
};

EXPORT_SYMBOL(wpcm_device_usb);

/* LCD Controller */

static struct resource wpcm_lcd_resource[] = {
	[0] = {
		.start = WPCM450_PA_LCD,
		.end   = WPCM450_PA_LCD + WPCM450_SZ_LCD - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_LCD,
		.end   = IRQ_LCD,
		.flags = IORESOURCE_IRQ,
	}

};

static u64 wpcm_device_lcd_dmamask = 0xffffffffUL;

struct platform_device wpcm_device_lcd = {
	.name		  = "WPCM450-lcd",//ZSWAN
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(wpcm_lcd_resource),
	.resource	  = wpcm_lcd_resource,
	.dev              = {
		.dma_mask		= &wpcm_device_lcd_dmamask,
		.coherent_dma_mask	= 0xffffffffUL
	}
};

EXPORT_SYMBOL(wpcm_device_lcd);

void  wpcm450_fb_set_platdata(struct WPCM450fb_mach_info *pd)
{
	struct WPCM450fb_mach_info *npd;

	npd = kmalloc(sizeof(*npd), GFP_KERNEL);
	if (npd) {
		memcpy(npd, pd, sizeof(*npd));
		wpcm_device_lcd.dev.platform_data = npd;
	} else {
		printk(KERN_ERR "no memory for LCD platform data\n");
	}
}

/* -FMI Controller */

	/*--fmi-wpcm450-NAND Flash by ns24 zswan 20070228--*/
/*************************START*************************************/
struct mtd_partition wpcm450_nand_part[] = { 
	[0] = { 
		.name = "WPCM450 NANDFLASH Partition 1 (16M)", 
		.offset = 0x00000000, 
		.size = 0x01000000, 
		}, 
	[1] = { 
		.name = "WPCM450 NANDFLASH Partition 2 (16M)", 
		.offset = 0x01000000, 
		.size = 0x01000000, 
		}, 
}; 

static struct wpcm450_nand_set wpcm450_nand_sets[] = { 
	[0] = { 
		.nr_partitions = ARRAY_SIZE(wpcm450_nand_part), 
		.partitions = wpcm450_nand_part 
		} 
}; 

static struct wpcm450_platform_nand wpcm450_nand_info = { 
		.tacls = 0, 
		.twrph0 = 30, 
		.twrph1 = 0, 
		.nr_sets = ARRAY_SIZE(wpcm450_nand_sets), 
		.sets = wpcm450_nand_sets, 
}; 
/*************************END*************************************/
static struct resource wpcm_fmi_resource[] = {
	[0] = {
		.start = WPCM450_PA_FMI,
		.end   = WPCM450_PA_FMI + 0x200 - 1,
		.flags = IORESOURCE_MEM,
	}
};

struct platform_device wpcm_device_fmi = {
	.name		  = "wpcm450-fmi",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(wpcm_fmi_resource),
	.resource	  = wpcm_fmi_resource,
	.dev= {
	.platform_data = &wpcm450_nand_info
	}
};

EXPORT_SYMBOL(wpcm_device_fmi);

/* USB Device (Gadget)*/

static struct resource wpcm_usbgadget_resource[] = {
	[0] = {
		.start = WPCM450_PA_USBDEV,
		.end   = WPCM450_PA_USBDEV + WPCM450_SZ_USBDEV - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_USBD,
		.end   = IRQ_USBD,
		.flags = IORESOURCE_IRQ,
	}

};

struct platform_device wpcm_device_usbgadget = {
	.name		  = "wpcm450-usbgadget",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(wpcm_usbgadget_resource),
	.resource	  = wpcm_usbgadget_resource,
};

EXPORT_SYMBOL(wpcm_device_usbgadget);

#if 0//clyu
/* Watchdog */

static struct resource wpcm_wdt_resource[] = {
	[0] = {
		.start = WPCM450_PA_WATCHDOG,
		.end   = WPCM450_PA_WATCHDOG + WPCM450_SZ_WATCHDOG - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_WDT,
		.end   = IRQ_WDT,
		.flags = IORESOURCE_IRQ,
	}

};

struct platform_device wpcm_device_wdt = {
	.name		  = "wpcm450-wdt",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(wpcm_wdt_resource),
	.resource	  = wpcm_wdt_resource,
};

EXPORT_SYMBOL(wpcm_device_wdt);

#endif

/* I2C */

static struct resource wpcm_i2c_resource[] = {
	[0] = {
		.start = WPCM450_PA_I2C,
		.end   = WPCM450_PA_I2C + WPCM450_SZ_I2C - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_SMBGROUP,//IRQ_I2CGROUP,
		.end   = IRQ_SMBGROUP,//IRQ_I2CGROUP,
		.flags = IORESOURCE_IRQ,
	}

};

struct platform_device wpcm_device_i2c = {
	.name		  = "wpcm450-i2c",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(wpcm_i2c_resource),
	.resource	  = wpcm_i2c_resource,
};

EXPORT_SYMBOL(wpcm_device_i2c);

#if 0//clyu
/* IIS */
static struct resource wpcm_iis_resource[] = {
	[0] = {
		.start = WPCM450_PA_IIS,
		.end   = WPCM450_PA_IIS + WPCM450_SZ_IIS -1,
		.flags = IORESOURCE_MEM,
	}
};

static u64 wpcm_device_iis_dmamask = 0xffffffffUL;

struct platform_device wpcm_device_iis = {
	.name		  = "wpcm450-iis",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(wpcm_iis_resource),
	.resource	  = wpcm_iis_resource,
	.dev              = {
		.dma_mask = &wpcm_device_iis_dmamask,
		.coherent_dma_mask = 0xffffffffUL
	}
};

EXPORT_SYMBOL(wpcm_device_iis);
#endif
/* RTC */

static struct resource wpcm_rtc_resource[] = {
	[0] = {
		.start = WPCM450_PA_RTC,
		.end   = WPCM450_PA_RTC + 0xff,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_RTC,
		.end   = IRQ_RTC,
		.flags = IORESOURCE_IRQ,
	},
};

struct platform_device wpcm_device_rtc = {
	.name		  = "wpcm450-rtc",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(wpcm_rtc_resource),
	.resource	  = wpcm_rtc_resource,
};

EXPORT_SYMBOL(wpcm_device_rtc);

/* Smart card zswan add it */

static struct resource wpcm_sc_resource[] = {
	[0] = {
		.start = WPCM450_PA_SC,
		.end   = WPCM450_PA_SC + WPCM450_SZ_SC-1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_SCGROUP,
		.end   = IRQ_SCGROUP,
		.flags = IORESOURCE_IRQ,
	},
};

struct platform_device wpcm_device_sc = {
	.name		  = "wpcm450-sc",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(wpcm_sc_resource),
	.resource	  = wpcm_sc_resource,
};

EXPORT_SYMBOL(wpcm_device_sc);

/* ADC */

static struct resource wpcm_adc_resource[] = {
	[0] = {
		.start = WPCM450_PA_ADC,
		.end   = WPCM450_PA_ADC + WPCM450_SZ_ADC - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_ADC,
		.end   = IRQ_ADC,
		.flags = IORESOURCE_IRQ,
	}

};

struct platform_device wpcm_device_adc = {
	.name		  = "wpcm450-adc",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(wpcm_adc_resource),
	.resource	  = wpcm_adc_resource,
};

/* KPI */

static struct resource wpcm_kpi_resource[] = {
	[0] = {
		.start = WPCM450_PA_KPI,
		.end   = WPCM450_PA_KPI + WPCM450_SZ_KPI - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_KPI,
		.end   = IRQ_KPI,
		.flags = IORESOURCE_IRQ,
	}

};

struct platform_device wpcm_device_sdi = {
	.name		  = "wpcm450-kpi",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(wpcm_kpi_resource),
	.resource	  = wpcm_kpi_resource,
};

EXPORT_SYMBOL(wpcm_device_sdi);

/* PS2*/

static struct resource wpcm_ps2_resource[] = {
	[0] = {
		.start = WPCM450_PA_PS2,
		.end   = WPCM450_PA_PS2 + WPCM450_SZ_PS2 - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_P2SGROUP,
		.end   = IRQ_P2SGROUP,
		.flags = IORESOURCE_IRQ,
	}

};

static u64 wpcm_device_ps2_dmamask = 0xffffffffUL;

struct platform_device wpcm_device_ps2 = {
	.name		  = "wpcm450-ps2",
	.id		  = 0,
	.num_resources	  = ARRAY_SIZE(wpcm_ps2_resource),
	.resource	  = wpcm_ps2_resource,
        .dev              = {
                .dma_mask = &wpcm_device_ps2_dmamask,
                .coherent_dma_mask = 0xffffffffUL
        }
};

EXPORT_SYMBOL(wpcm_device_ps2);
#endif /* AVI_REMOVE */

/* pwm timer blocks */

static struct resource wpcm_clock_resource[] = {
	[0] = {
		.start = (resource_size_t) WPCM450_VA_CLK,
		.end   = (resource_size_t) WPCM450_VA_CLK + WPCM450_SZ_CLK - 0x200 - 1,
		.flags = IORESOURCE_MEM,
	},

};

struct platform_device wpcm_device_clock = {
	.name		  = "wpcm450-clock",
	.id		  = 0,
	.num_resources	  = ARRAY_SIZE(wpcm_clock_resource),
	.resource	  = wpcm_clock_resource,
};

EXPORT_SYMBOL(wpcm_device_clock);


