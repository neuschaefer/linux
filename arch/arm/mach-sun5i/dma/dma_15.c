/*
 * arch\arm\mach-sun5i\dma\dma_15.c
 * (C) Copyright 2007-2011
 * Reuuimlla Technology Co., Ltd. <www.reuuimllatech.com>
 * huangxin <huangxin@Reuuimllatech.com>
 *
 * some simple description for this code
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sysdev.h>
#include <linux/serial_core.h>
#include <linux/platform_device.h>


#include <mach/dma.h>
#include <mach/system.h>
#include "dma_regs.h"
#include <mach/platform.h>
#include <linux/pm.h>

static struct sw_dma_map __initdata sw_dma_mappings[DMACH_MAX] = {
	[DMACH_NSPI0_RX] = {
		.name		= "spi0_rx",
		.channels = {DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID,
				0,0,0,0,0,0,0,0,},
	},
	[DMACH_NSPI0_TX] = {
		.name		= "spi0_tx",
		.channels = {DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID,
				0,0,0,0,0,0,0,0,},
	},
	[DMACH_NSPI1_RX] = {
		.name		= "spi1_rx",
		.channels = {DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID,
				0,0,0,0,0,0,0,0,},
	},
	[DMACH_NSPI1_TX] = {
		.name		= "spi1_tx",
		.channels = {DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID,
				0,0,0,0,0,0,0,0,},
	},
	[DMACH_NSPI2_RX] = {
		.name		= "spi2_rx",
		.channels = {DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID,
				0,0,0,0,0,0,0,0,},
	},
	[DMACH_NSPI2_TX] = {
		.name		= "spi2_tx",
		.channels = {DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID,
				0,0,0,0,0,0,0,0,},
	},
	[DMACH_NUART0] = {
		.name		= "uart0",
		.channels = {DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID,
			0,0,0,0,0,0,0,0,},
	},
	[DMACH_NUART1] = {
		.name		= "uart1",
		.channels = {DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID,
			0,0,0,0,0,0,0,0,},
	},
	[DMACH_NUART2] = {
		.name		= "uart2",
		.channels = {DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID,
			0,0,0,0,0,0,0,0,},
	},
	[DMACH_NUART3] = {
		.name		= "uart3",
		.channels = {DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID,
			0,0,0,0,0,0,0,0,},
	},
	[DMACH_NSRAM] = {
		.name		= "nsram",
		.channels = {DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,0,0,0,0,0,0,0,0},
	},
	[DMACH_NSDRAM] = {
		.name		= "nsdram",
		.channels = {DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,0,0,0,0,0,0,0,0},
	},
	[DMACH_NTPAD] = {
		.name		= "tpadc",
		.channels = {DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,0,0,0,0,0,0,0,0},
	},
	[DMACH_NADDA_PLAY] = {
		.name		= "adda_play",
		.channels = {DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,
				0,0,0,0,0,0,0,0,},
	},
	[DMACH_NADDA_CAPTURE] = {
		.name		= "adda_capture",
		.channels = {DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,
				0,0,0,0,0,0,0,0,},
	},
	[DMACH_NIIS_PLAY] = {
		.name		= "iis",
		.channels = {DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,
				0,0,0,0,0,0,0,0,},
	},
	[DMACH_NIIS_CAPTURE] = {
		.name		= "iis_capture",
		.channels = {DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,
				0,0,0,0,0,0,0,0,},
	},
	[DMACH_NIR] = {
		.name		= "ir",
		.channels = {DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID,
			0,0,0,0,0,0,0,0,},
	},
	[DMACH_NSPDIF] = {
		.name		= "spdif",
		.channels = {DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,
				0,0,0,0,0,0,0,0,},
	},
	[DMACH_NHDMI] = {
		.name		= "hdmi",
		.channels = {DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,
				0,0,0,0,0,0,0,0,},
	},
	[DMACH_NUSBEP1] = {
		.name		= "usbep1",
		.channels = {DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,
				0,0,0,0,0,0,0,0,},
	},
	[DMACH_NUSBEP2] = {
		.name		= "usbep2",
		.channels = {DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,
				0,0,0,0,0,0,0,0,},
	},
	[DMACH_NUSBEP3] = {
		.name		= "usbep3",
		.channels = {DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,
				0,0,0,0,0,0,0,0,},
	},
	[DMACH_NUSBEP4] = {
		.name		= "usbep4",
		.channels = {DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,
				0,0,0,0,0,0,0,0,},
	},
	[DMACH_NUSBEP5] = {
		.name		= "usbep5",
		.channels = {DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,DMA_CH_VALID,
				0,0,0,0,0,0,0,0,},
	},
	[DMACH_DSRAM] = {
		.name		= "dsram",
		.channels = {0,0,0,0,0,0,0,0,
			DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID},
		},
	[DMACH_DSDRAM] = {
		.name		= "dsdram",
		.channels = {0,0,0,0,0,0,0,0,
			DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID},
		},
	[DMACH_DNAND] = {
		.name		= "dnand",
		.channels = {0,0,0,0,0,0,0,0,
			DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID},
		},
	[DMACH_DUSB0] = {
		.name		= "usb0",
		.channels = {0,0,0,0,0,0,0,0,
			DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID},
		},
	[DMACH_DEMACR] = {
		.name		= "EMACRX_DMA",
		.channels = {0,0,0,0,0,0,0,0,
			DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID},
	},
	[DMACH_DEMACT] = {
		.name		= "EMACTX_DMA",
		.channels = {0,0,0,0,0,0,0,0,
			DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID},
	},
	[DMACH_DSPI1_RX] = {
		.name		= "dspi1_rx",
		.channels = {0,0,0,0,0,0,0,0,
			DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID},
	},
	[DMACH_DSPI1_TX] = {
		.name		= "dspi1_tx",
		.channels = {0,0,0,0,0,0,0,0,
			DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID},
	},
	[DMACH_DSSR] = {
		.name		= "dssr",
		.channels = {0,0,0,0,0,0,0,0,
			DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID},
	},
	[DMACH_DSST] = {
		.name		= "dsst",
		.channels = {0,0,0,0,0,0,0,0,
			DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID},
	},
	[DMACH_TCON0] = {
		.name		= "tcon0",
		.channels = {0,0,0,0,0,0,0,0,
			DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID},
	},
	[DMACH_HDMIAUDIO] = {
		.name		= "hdmiaudio",
		.channels = {0,0,0,0,0,0,0,0,
			DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID},
	},
	[DMACH_DMS] = {
		.name		= "dms",
		.channels = {0,0,0,0,0,0,0,0,
			DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID},
	},
	[DMACH_DSPI0_RX] = {
		.name		= "dspi0_rx",
		.channels = {0,0,0,0,0,0,0,0,
			DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID},
	},
	[DMACH_DSPI0_TX] = {
		.name		= "dspi0_tx",
		.channels = {0,0,0,0,0,0,0,0,
			DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID},
	},
	[DMACH_DSPI2_RX] = {
		.name		= "dspi2_rx",
		.channels = {0,0,0,0,0,0,0,0,
			DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID},
	},
	[DMACH_DSPI2_TX] = {
		.name		= "dspi2_tx",
		.channels = {0,0,0,0,0,0,0,0,
			DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID, DMA_CH_VALID},
	},
};

static struct sw_dma_selection __initdata sw_dma_sel = {
	.dcon_mask	= 0xffffffff,
	.map		= sw_dma_mappings,
	.map_size	= ARRAY_SIZE(sw_dma_mappings),
};

static int __devinit sw_dmac_probe(struct platform_device *dev)
{
	int ret;

	sw15_dma_init();

	ret = sw_dma_init_map(&sw_dma_sel);

	if (ret) {
		printk("DMAC: failed to init map\n");
	} else {
		pr_info("Initialize DMAC OK\n");
	}

	return ret;
}
static int __devexit sw_dmac_remove(struct platform_device *dev)
{
        printk("[%s] enter\n", __FUNCTION__);
        return 0;
}
#ifdef CONFIG_PM

static void __iomem *dma_base;
static int regsave[2];
static struct sw_dma_chan cp[16];
extern struct sw_dma_chan sw_chans[SW_DMA_CHANNELS];
static int sw_dmac_suspend(struct platform_device *dev, pm_message_t state)
{
	int channel;
	
	dma_base = (void __iomem *)SW_VA_DMAC_IO_BASE;
	
	/*process for normal standby*/
	if (NORMAL_STANDBY == standby_type) {		
	/*process for super standby*/	
	} else if(SUPER_STANDBY == standby_type) {
		for (channel = 0; channel < SW_DMA_CHANNELS;  channel++) {
			cp[channel] = sw_chans[channel];
			
			if ((channel & 0xff) < 8) {
				cp[channel].regs   = dma_base + 0x100 + (channel * 0x20);
			} else {
				cp[channel].regs   = dma_base + 0x300 + ((channel - 8) * 0x20);
			}
			
			if (cp[channel].state == SW_DMA_RUNNING) {
				sw_dma_ctrl(cp[channel].number, SW_DMAOP_STOP);
				cp[channel].suspend_state = SW_DMA_SUSPEND_RUNNING;				
			}
			
			/*while enter the suspend,save the dma register*/
			cp[channel].regsave_ctrl 	= readl(cp[channel].regs);
			cp[channel].regsave_source 	= readl(cp[channel].regs + 0x4);
			cp[channel].regsave_des 	= readl(cp[channel].regs + 0x8);
			cp[channel].regsave_byte 	= readl(cp[channel].regs + 0xc);

		}
		
		/*save the dma enable register*/
		regsave[0] = readl(dma_base + SW_DMA_DIRQEN);
		/*save the dma pending status register*/
		regsave[1] = readl(dma_base + SW_DMA_DIRQPD);		
	}
	return 0;
}

static int sw_dmac_resume(struct platform_device *dev)
{
	int channel;	
	dma_base = (void __iomem *)SW_VA_DMAC_IO_BASE;

	if (NORMAL_STANDBY == standby_type) {		
		//process for normal standby
	} else if(SUPER_STANDBY == standby_type) {
		/*restore the dma enable register*/
		writel(regsave[0], dma_base + SW_DMA_DIRQEN);
		/*restore the dma pending status register*/	
		writel(regsave[1], dma_base + SW_DMA_DIRQPD);		
			
		for (channel = 0; channel < SW_DMA_CHANNELS;  channel++) {			
			if ((channel & 0xff) < 8) {
				cp[channel].regs   = dma_base + 0x100 + (channel * 0x20);
			} else {
				cp[channel].regs   = dma_base + 0x300 + ((channel - 8) * 0x20);
			}
		
			/*while enter the suspend,save the dma register*/
			writel(cp[channel].regsave_ctrl, cp[channel].regs);
			writel(cp[channel].regsave_source, cp[channel].regs + 0x4);
			writel(cp[channel].regsave_des, cp[channel].regs + 0x8);
			writel(cp[channel].regsave_byte, cp[channel].regs + 0xc);
			
			if (cp[channel].suspend_state == SW_DMA_SUSPEND_RUNNING) {
				sw_dma_ctrl(cp[channel].number, SW_DMAOP_START);			
			}	
		}
	}
	    
    return 0;
}
#else
	#define sw_dmac_suspend NULL
	#define sw_dmac_resume  NULL
#endif /* CONFIG_PM */

static struct platform_driver sw_dmac_driver = {
        .probe          = sw_dmac_probe,
        .remove         = __devexit_p(sw_dmac_remove),
        .suspend        = sw_dmac_suspend,
        .resume         = sw_dmac_resume,
        .driver         = {
                .name   = "sw_dmac",
                .owner  = THIS_MODULE,
        },
};

static int __init sw_dma_drvinit(void)
{
        platform_driver_register(&sw_dmac_driver);
	return 0;
}

arch_initcall(sw_dma_drvinit);


