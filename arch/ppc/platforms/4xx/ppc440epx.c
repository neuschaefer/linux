/*
 * arch/ppc/platforms/4xx/ppc440epx.c
 *
 * PPC440EPx I/O descriptions
 *
 * Wade Farnsworth <wfarnsworth@mvista.com>
 * Copyright 2004 MontaVista Software Inc.
 * Copyright 2006 AMCC
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <platforms/4xx/ppc440epx.h>
#include <asm/ocp.h>
#include <asm/ppc4xx_pic.h>

/* todo: include or remove CONFIG_INTR_COALESCE */
static struct ocp_func_emac_data ppc440epx_emac0_def = {
	.rgmii_idx	= 0,            /* RGMII device index */
	.rgmii_mux	= 0,
	.zmii_idx       = 0,            /* ZMII device index */
	.zmii_mux       = 0,            /* ZMII input of this EMAC */
	.mal_idx        = 0,            /* MAL device index */
	.mal_rx_chan    = 0,            /* MAL rx channel number */
	.mal_tx_chan    = 0,            /* MAL tx channel number */
	.wol_irq        = 61,		/* WOL interrupt number - same as 440EP */
	.mdio_idx       = -1,           /* No shared MDIO but always via ZMII bridge */
	.tah_idx	= -1,           /* No TAH */
#ifdef	CONFIG_INTR_COALESCE
	.txcoal_irq 	= 70,  		/* Interrupt coalescence TX IRQ */
	.rxcoal_irq 	= 72,  		/* Interrupt coalescence RX IRQ */
#endif
};

static struct ocp_func_emac_data ppc440epx_emac1_def = {
	.rgmii_idx	= 0,            /* RGMII */
	.rgmii_mux	= 1,            /* RGMII */
	.zmii_idx       = 0,            /* ZMII device index */
	.zmii_mux       = 1,            /* ZMII input of this EMAC */
	.mal_idx        = 0,            /* MAL device index */
	.mal_rx_chan    = 1,            /* MAL rx channel number */
	.mal_tx_chan    = 1,            /* MAL tx channel number */
	.wol_irq        = 63,  		/* WOL interrupt number _- same as 440EP */
	.mdio_idx       = -1,           /* no shared MDIO but always via ZMII bridge */
	.tah_idx	= -1,           /* No TAH */
#ifdef	CONFIG_INTR_COALESCE
	.txcoal_irq 	= 71,  		/* Interrupt coalescence TX IRQ */
	.rxcoal_irq 	= 73,  		/* Interrupt coalescence RX IRQ */
#endif
};
OCP_SYSFS_EMAC_DATA()

static struct ocp_func_mal_data ppc440epx_mal0_def = {
	.num_tx_chans   = 2,  		/* Number of TX channels */
	.num_rx_chans   = 2,    	/* Number of RX channels */
	.txeob_irq	= 10,		/* TX End Of Buffer IRQ  - same as 440EP */
	.rxeob_irq	= 11,		/* RX End Of Buffer IRQ  - same as 440EP*/
	.txde_irq	  = 33,		/* TX Descriptor Error IRQ - same as 440EP */
	.rxde_irq	  = 34,		/* RX Descriptor Error IRQ - same as 440EP*/
	.serr_irq	  = 32,		/* MAL System Error IRQ  - same as 440EP   */
	.dcr_base	= DCRN_MAL_BASE /* MAL0_CFG DCR number */
};
OCP_SYSFS_MAL_DATA()

static struct ocp_func_iic_data ppc440epx_iic0_def = {
	.fast_mode	= 0,		/* Use standad mode (100Khz) */
};

static struct ocp_func_iic_data ppc440epx_iic1_def = {
	.fast_mode	= 0,		/* Use standad mode (100Khz) */
};
OCP_SYSFS_IIC_DATA()

struct ocp_def core_ocp[] = {
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_OPB,
	  .index	= 0,
	  .paddr	= 0x1EF600000ULL,
	  .irq		= OCP_IRQ_NA,
	  .pm		= OCP_CPM_NA,
	},
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_16550,
	  .index	= 0,
	  .paddr	= PPC440EPX_UART0_ADDR,
	  .irq		= UART0_INT,
	  .pm		= IBM_CPM_UART0,
	},
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_16550,
	  .index	= 1,
	  .paddr	= PPC440EPX_UART1_ADDR,
	  .irq		= UART1_INT,
	  .pm		= IBM_CPM_UART1,
	},
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_16550,
	  .index	= 2,
	  .paddr	= PPC440EPX_UART2_ADDR,
	  .irq		= UART2_INT,
	  .pm		= IBM_CPM_UART2,
	},
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_16550,
	  .index	= 3,
	  .paddr	= PPC440EPX_UART3_ADDR,
	  .irq		= UART3_INT,
	  .pm		= IBM_CPM_UART3,
	},
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_IIC,
	  .index	= 0,
	  .paddr	= 0x1EF600700ULL,
	  .irq		= IIC0_INT,
	  .pm		= IBM_CPM_IIC0,
	  .additions	= &ppc440epx_iic0_def,
	  .show		= &ocp_show_iic_data
	},
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_IIC,
	  .index	= 1,
	  .paddr	= 0x1EF600800ULL,
	  .irq		= IIC1_INT,
	  .pm		= IBM_CPM_IIC1,
	  .additions	= &ppc440epx_iic1_def,
	  .show		= &ocp_show_iic_data
	},
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_GPIO,
	  .index	= 0,
	  .paddr	= 0x1EF600B00ULL,
	  .irq		= OCP_IRQ_NA,
	  .pm		= IBM_CPM_GPIO0,
	},
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_GPIO,
	  .index	= 1,
	  .paddr	= 0x1EF600C00ULL,
	  .irq		= OCP_IRQ_NA,
	  .pm		= OCP_CPM_NA,
	},
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_MAL,
	  .paddr	= OCP_PADDR_NA,
	  .irq		= OCP_IRQ_NA,
	  .pm		= OCP_CPM_NA,
	  .additions	= &ppc440epx_mal0_def,
	  .show		= &ocp_show_mal_data,
	},
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_EMAC,
	  .index	= 0,
	  .paddr	= 0x1EF600E00ULL,
	  .irq		= EMAC0_INT,
	  .pm		= OCP_CPM_NA,
	  .additions	= &ppc440epx_emac0_def,
	  .show		= &ocp_show_emac_data,
	},
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_EMAC,
	  .index	= 1,
	  .paddr	= 0x1EF600F00ULL,
	  .irq		= EMAC1_INT,
	  .pm		= OCP_CPM_NA,
	  .additions	= &ppc440epx_emac1_def,
	  .show		= &ocp_show_emac_data,
	},
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_ZMII,
	  .paddr	= 0x1EF600D00ULL,
	  .irq		= OCP_IRQ_NA,
	  .pm		= OCP_CPM_NA,
	},
	{ .vendor	= OCP_VENDOR_IBM,
	  .function	= OCP_FUNC_RGMII,
	  .paddr	= 0x1EF601000ULL,
	  .irq		= OCP_IRQ_NA,
	  .pm		= OCP_CPM_NA,
	},
	{ .vendor	= OCP_VENDOR_INVALID
	}
};

/* Polarity and triggering settings for internal interrupt sources */
/* 3 UIC */
struct ppc4xx_uic_settings ppc4xx_core_uic_cfg[] __initdata = {
	{ .polarity	= 0xffbff1ef,
	  .triggering   = 0x00000800,
	  .ext_irq_mask = 0x000000010,	/* IRQ4 */
	},
	{ .polarity	= 0xffffe7A5,
	  .triggering	= 0x06000040,
	  .ext_irq_mask = 0x0000380A,	/* IRQ7-IRQ8-IRQ9-IRQ0-IRQ1 */
	},
	{ .polarity	= 0x27ffffff,
	  .triggering	= 0x03e00000,
	  .ext_irq_mask = 0xd8000000,	/* IRQ5-IRQ6-IRQ2-IRQ3*/
	},
};

static struct resource usb_gadget_resources[] = {
	[0] = {
		.start	= 0x0E0000100ULL,
		.end 	= 0x0E000017FULL,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.name	= "usb_device_irq",
		.start	= 20,
		.end	= 20,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct resource ohci_usb_resources[] = {
	[0] = {
		.start	= 0x0E0000400,
		.end	= 0x0E00004FF,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= 21,
		.end	= 21,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct resource ehci_usb_resources[] = {
	[0] = {
		.start	= 0x0E0000300,
		.end	= 0x0E00003FF,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= 26,
		.end	= 26,
		.flags	= IORESOURCE_IRQ,
	},
};

static u64 dma_mask = 0xffffffffULL;

static struct platform_device ohci_usb_device = {
	.name		= "ppc-soc-ohci",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(ohci_usb_resources),
	.resource	= ohci_usb_resources,
	.dev		= {
		.dma_mask = &dma_mask,
		.coherent_dma_mask = 0xffffffffULL,
	}
};

static struct platform_device usb_gadget_device = {
	.name		= "musbhsfc_udc",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(usb_gadget_resources),
	.resource       = usb_gadget_resources,
	.dev		= {
		.dma_mask = &dma_mask,
		.coherent_dma_mask = 0xffffffffULL,
	}
};

static struct platform_device ehci_usb_device = {
	.name		= "amcc-ehci",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(ehci_usb_resources),
	.resource	= ehci_usb_resources,
	.dev		= {
		.dma_mask = &dma_mask,
		.coherent_dma_mask = 0xffffffffULL,
	}
};

static struct platform_device *ppc440epx_devs[] __initdata = {
	&ohci_usb_device,
	&ehci_usb_device,
	&usb_gadget_device,
};

static int __init ppc440epx_platform_add_devices(void)
{
	return platform_add_devices(ppc440epx_devs, ARRAY_SIZE(ppc440epx_devs));
}
arch_initcall(ppc440epx_platform_add_devices);
