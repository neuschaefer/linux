/*
 * ALPR board specific routines
 *
 * Copyright (c) 2006 DENX Software Engineering
 * Heiko Schocher <hs@denx.de>
 * Stefan Roese <sr@denx.de>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/stddef.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/reboot.h>
#include <linux/pci.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/major.h>
#include <linux/blkdev.h>
#include <linux/console.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/initrd.h>
#include <linux/seq_file.h>
#include <linux/root_dev.h>
#include <linux/tty.h>
#include <linux/serial.h>
#include <linux/serial_core.h>
#include <linux/resource.h>
#include <linux/platform_device.h>

#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>

#include <asm/system.h>
#include <asm/pgtable.h>
#include <asm/page.h>
#include <asm/dma.h>
#include <asm/io.h>
#include <asm/machdep.h>
#include <asm/ocp.h>
#include <asm/pci-bridge.h>
#include <asm/time.h>
#include <asm/todc.h>
#include <asm/bootinfo.h>
#include <asm/ppc4xx_pic.h>
#include <asm/ppcboot.h>
#include <asm/tlbflush.h>

#include <syslib/gen550.h>
#include <syslib/ibm440gx_common.h>

extern bd_t __res;

/*
 * External IRQ triggering/polarity settings
 */
unsigned char ppc4xx_uic_ext_irq_cfg[] __initdata = {
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE),	/* IRQ0: PCI slot 0 */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE),	/* IRQ1: PCI slot 1 */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE),	/* IRQ2: PCI slot 2 */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE),	/* IRQ3: PCI slot 3 */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE),	/* IRQ4: PHY0 */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE),	/* IRQ5: PHY1 */
	(IRQ_SENSE_EDGE  | IRQ_POLARITY_NEGATIVE),	/* IRQ6: SHUTDOWN */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE),	/* IRQ7: I_MU_TRIG */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE),	/* IRQ8: I_MU_WIN */
	(IRQ_SENSE_EDGE  | IRQ_POLARITY_NEGATIVE),	/* IRQ9: SSD_EMPTY */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE),	/* IRQ10: PMC_PRESENT */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE),	/* IRQ11: N/A */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE),	/* IRQ12: UART1_RX */
};

static struct ibm44x_clocks clocks __initdata;

static struct pci_dev *dev = NULL;
static int fpga_map_count = 0;

#define PCI_VENDOR_ID_IBUF_FPGA	0xfffe
#define PCI_DEVICE_ID_IBUF_FPGA	0xf05f

static struct resource alpr_ndfc = {
        .start = 0xf0000000,		/* the real addr is 0x1f0000000! */
        .end = 0xf0000fff,
        .flags = IORESOURCE_MEM,
};

static struct platform_device alpr_ndfc_device = {
        .name = "pd-ndfc-nand",
        .id = 0,
        .num_resources = 1,
        .resource = &alpr_ndfc,
};

static struct mtd_partition nand_parts0[] = {
        {
                .name   = "kernel1",
                .offset = 0,
                .size   = 2 << 20,
        },
        {
                .name   = "kernel2",
                .offset = MTDPART_OFS_NXTBLK,
                .size   = 2 << 20,
        },
        {
                .name   = "res-nand",
                .offset = MTDPART_OFS_NXTBLK,
                .size   = 5 << 20,
        },
        {
                .name   = "rootfs",
                .offset = MTDPART_OFS_NXTBLK,
                .size   = 128 << 20,
        },
        {
                .name   = "logfs",
                .offset = MTDPART_OFS_NXTBLK,
                .size   = 4 << 20,
        },
        {
                .name   = "data0",
                .offset = MTDPART_OFS_NXTBLK,
                .size   = MTDPART_SIZ_FULL,
        }
};

static struct mtd_partition nand_parts1[] = {
        {
                .name   = "data1",
                .offset = 0,
                .size   = MTDPART_SIZ_FULL,
        }
};

static struct mtd_partition nand_parts2[] = {
        {
                .name   = "data2",
                .offset = 0,
                .size   = MTDPART_SIZ_FULL,
        }
};

static struct mtd_partition nand_parts3[] = {
        {
                .name   = "data3",
                .offset = 0,
                .size   = MTDPART_SIZ_FULL,
        }
};

static struct platform_nand_chip alpr_nand_chip0 = {
        .nr_chips = 1,
        .chip_offset = 0,
        .nr_partitions = ARRAY_SIZE(nand_parts0),
        .partitions = nand_parts0,
        .chip_delay = 50,
};

static struct platform_nand_chip alpr_nand_chip1 = {
        .nr_chips = 1,
        .chip_offset = 1,
        .nr_partitions = ARRAY_SIZE(nand_parts1),
        .partitions = nand_parts1,
        .chip_delay = 50,
};

static struct platform_nand_chip alpr_nand_chip2 = {
        .nr_chips = 1,
        .chip_offset = 2,
        .nr_partitions = ARRAY_SIZE(nand_parts2),
        .partitions = nand_parts2,
        .chip_delay = 50,
};

static struct platform_nand_chip alpr_nand_chip3 = {
        .nr_chips = 1,
        .chip_offset = 3,
        .nr_partitions = ARRAY_SIZE(nand_parts3),
        .partitions = nand_parts3,
        .chip_delay = 50,
};

static struct platform_device alpr_nand_device[] = {
	{
		.name = "pd-ndfc-chip",
		.id = 0,
		.num_resources = 0,
		.dev = {
			.platform_data = &alpr_nand_chip0,
			.parent = &alpr_ndfc_device.dev,
		}
	},
	{
		.name = "pd-ndfc-chip",
		.id = 1,
		.num_resources = 0,
		.dev = {
			.platform_data = &alpr_nand_chip1,
			.parent = &alpr_ndfc_device.dev,
		}
	},
	{
		.name = "pd-ndfc-chip",
		.id = 2,
		.num_resources = 0,
		.dev = {
			.platform_data = &alpr_nand_chip2,
			.parent = &alpr_ndfc_device.dev,
		}
	},
	{
		.name = "pd-ndfc-chip",
		.id = 3,
		.num_resources = 0,
		.dev = {
			.platform_data = &alpr_nand_chip3,
			.parent = &alpr_ndfc_device.dev,
		}
	},
};

static struct platform_device alpr_fpga_uart_device[] = {
	{
		.name		= "fpga-uart",
		.id		= 0,
		.num_resources	= 2,
		.resource	= (struct resource[]) {
			{
				.start	= IBUF_UART0_CTRL,
				.end	= IBUF_UART0_CTRL + 0x5ff,
				.flags	= IORESOURCE_MEM,
			},
			{
				.start	= 24,
				.end	= 24,
				.flags	= IORESOURCE_IRQ,
			},
		},
	},
	{
		.name		= "fpga-uart",
		.id		= 1,
		.num_resources	= 2,
		.resource	= (struct resource[]) {
			{
				.start	= IBUF_UART1_CTRL,
				.end	= IBUF_UART1_CTRL + 0x5ff,
				.flags	= IORESOURCE_MEM,
			},
			{
				.start	= 24,
				.end	= 24,
				.flags	= IORESOURCE_IRQ,
			},
		},
	},
	{
		.name		= "fpga-uart",
		.id		= 2,
		.num_resources	= 2,
		.resource	= (struct resource[]) {
			{
				.start	= IBUF_UART2_CTRL,
				.end	= IBUF_UART2_CTRL + 0x5ff,
				.flags	= IORESOURCE_MEM,
			},
			{
				.start	= 24,
				.end	= 24,
				.flags	= IORESOURCE_IRQ,
			},
		},
	},
};

static void __init alpr_calibrate_decr(void)
{
	unsigned int freq;

	freq = clocks.cpu;

	ibm44x_calibrate_decr(freq);
}

static int alpr_show_cpuinfo(struct seq_file *m)
{
	seq_printf(m, "vendor\t\t: Prodrive\n");
	seq_printf(m, "machine\t\t: PPC440GX ALPR\n");
	ibm440gx_show_cpuinfo(m);
	return 0;
}

static inline int alpr_map_irq(struct pci_dev *dev, unsigned char idsel,
			       unsigned char pin)
{
	static char pci_irq_table[][4] =
		/*
		 *	PCI IDSEL/INTPIN->INTLINE
		 * 	   A   B   C   D
		 */
		{
			{ 25, 25, 25, 25 },	/* device 2 - J2K DSP	*/
			{ 24, 24, 24, 24 },	/* device 3 - IBUF FPGA	*/
			{ 23, 23, 23, 23 },	/* device 4 - USB	*/
		};

	const long min_idsel = 2, max_idsel = 4, irqs_per_slot = 4;
	return PCI_IRQ_TABLE_LOOKUP;
}

static void __init alpr_set_emacdata(void)
{
	struct ocp_def *def;
	struct ocp_func_emac_data *emacdata;
	int i;

	/*
	 * Note: Current rev. board only operates in Group 4a
	 * mode, so we always set EMAC0-1 for SMII and EMAC2-3
	 * for RGMII (though these could run in RTBI just the same).
	 *
	 * The FPGA reg 3 information isn't even suitable for
	 * determining the phy_mode, so if the board becomes
	 * usable in !4a, it will be necessary to parse an environment
	 * variable from the firmware or similar to properly configure
	 * the phy_map/phy_mode.
	 */
	/* Set phy_map, phy_mode, and mac_addr for each EMAC */
	for (i=0; i<4; i++) {
		def = ocp_get_one_device(OCP_VENDOR_IBM, OCP_FUNC_EMAC, i);
		emacdata = def->additions;
		if (i < 2) {
			emacdata->phy_map = 0xffffffff;	/* Skip all */
			emacdata->phy_mode = PHY_MODE_SMII;
		} else {
			emacdata->phy_map = 0x00000001; /* Skip 0x00 */
			emacdata->phy_mode = PHY_MODE_RGMII;
		}
		if (i == 0)
			memcpy(emacdata->mac_addr, __res.bi_enetaddr, 6);
		else if (i == 1)
			memcpy(emacdata->mac_addr, __res.bi_enet1addr, 6);
		else if (i == 2)
			memcpy(emacdata->mac_addr, __res.bi_enet2addr, 6);
		else if (i == 3)
			memcpy(emacdata->mac_addr, __res.bi_enet3addr, 6);
	}
}

#define PCIX_READW(offset)			\
	(readw(pcix_reg_base+offset))

#define PCIX_READL(offset)			\
	(readl(pcix_reg_base+offset))

#define PCIX_WRITEW(value, offset)		\
	(writew(value, pcix_reg_base+offset))

#define PCIX_WRITEL(value, offset)		\
	(writel(value, pcix_reg_base+offset))

/*
 * FIXME: This is only here to "make it work".  This will move
 * to a ibm_pcix.c which will contain a generic IBM PCIX bridge
 * configuration library. -Matt
 */
static void __init alpr_setup_pcix(void)
{
	void *pcix_reg_base;

	pcix_reg_base = ioremap64(PCIX0_REG_BASE, PCIX_REG_SIZE);

	/* Enable PCIX0 I/O, Mem, and Busmaster cycles */
	PCIX_WRITEW(PCIX_READW(PCIX0_COMMAND) | PCI_COMMAND_IO | PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER, PCIX0_COMMAND);

	/* Disable all windows */
	PCIX_WRITEL(0, PCIX0_POM0SA);
	PCIX_WRITEL(0, PCIX0_POM1SA);
	PCIX_WRITEL(0, PCIX0_POM2SA);
	PCIX_WRITEL(0, PCIX0_PIM0SA);
	PCIX_WRITEL(0, PCIX0_PIM0SAH);
	PCIX_WRITEL(0, PCIX0_PIM1SA);
	PCIX_WRITEL(0, PCIX0_PIM2SA);
	PCIX_WRITEL(0, PCIX0_PIM2SAH);

	/* Setup 2GB PLB->PCI outbound mem window (3_8000_0000->0_8000_0000) */
	PCIX_WRITEL(0x00000003, PCIX0_POM0LAH);
	PCIX_WRITEL(0x80000000, PCIX0_POM0LAL);
	PCIX_WRITEL(0x00000000, PCIX0_POM0PCIAH);
	PCIX_WRITEL(0x80000000, PCIX0_POM0PCIAL);
	PCIX_WRITEL(0x80000001, PCIX0_POM0SA);

#if 0 /* test-only */
	printk("POM0LAH: %x = %x\n", PCIX0_POM0LAH, PCIX_READL(PCIX0_POM0LAH));
	printk("POM0LAL: %x = %x\n", PCIX0_POM0LAL, PCIX_READL(PCIX0_POM0LAL));
	printk("POM0PCIAH: %x = %x\n", PCIX0_POM0PCIAH, PCIX_READL(PCIX0_POM0PCIAH));
	printk("POM0PCIAL: %x = %x\n", PCIX0_POM0PCIAL, PCIX_READL(PCIX0_POM0PCIAL));
	printk("POM0SA: %x = %x\n", PCIX0_POM0SA, PCIX_READW(PCIX0_POM0SA));
#endif

	/* Setup 2GB PCI->PLB inbound memory window at 0, enable MSIs */
	PCIX_WRITEL(0x00000000, PCIX0_PIM0LAH);
	PCIX_WRITEL(0x00000000, PCIX0_PIM0LAL);
	PCIX_WRITEL(0xe0000007, PCIX0_PIM0SA);

	eieio();
}

static void __init alpr_setup_hose(void)
{
	struct pci_controller *hose;

	/* Configure windows on the PCI-X host bridge */
	alpr_setup_pcix();

	hose = pcibios_alloc_controller();

	if (!hose)
		return;

	hose->first_busno = 0;
	hose->last_busno = 0xff;

	hose->pci_mem_offset = ALPR_PCI_MEM_OFFSET;

	pci_init_resource(&hose->io_resource,
			  ALPR_PCI_LOWER_IO,
			  ALPR_PCI_UPPER_IO,
			  IORESOURCE_IO,
			  "PCI host bridge");

	pci_init_resource(&hose->mem_resources[0],
			  ALPR_PCI_LOWER_MEM,
			  ALPR_PCI_UPPER_MEM,
			  IORESOURCE_MEM,
			  "PCI host bridge");

	hose->io_space.start = ALPR_PCI_LOWER_IO;
	hose->io_space.end = ALPR_PCI_UPPER_IO;
	hose->mem_space.start = ALPR_PCI_LOWER_MEM;
	hose->mem_space.end = ALPR_PCI_UPPER_MEM;
	hose->io_base_virt = ioremap64(ALPR_PCI_IO_BASE, ALPR_PCI_IO_SIZE);
	isa_io_base = (unsigned long) hose->io_base_virt;

	setup_indirect_pci(hose,
			   ALPR_PCI_CFGA_PLB32,
			   ALPR_PCI_CFGD_PLB32);
	hose->set_cfg_type = 1;

	hose->last_busno = pciauto_bus_scan(hose, hose->first_busno);

	ppc_md.pci_swizzle = common_swizzle;
	ppc_md.pci_map_irq = alpr_map_irq;
}

static void __init alpr_early_serial_map(void)
{
	struct uart_port port;

	/* Setup ioremapped serial port access */
	memset(&port, 0, sizeof(port));
	port.membase = ioremap64(PPC440GX_UART0_ADDR, 8);
	port.irq = UART0_INT;
	port.uartclk = clocks.uart0;
	port.regshift = 0;
	port.iotype = UPIO_MEM;
	port.flags = UPF_BOOT_AUTOCONF | UPF_SKIP_TEST;
	port.line = 0;

	if (early_serial_setup(&port) != 0) {
		printk("Early serial init of port 0 failed\n");
	}

#if defined(CONFIG_SERIAL_TEXT_DEBUG) || defined(CONFIG_KGDB)
	/* Configure debug serial access */
	gen550_init(0, &port);

	/* Purge TLB entry added in head_44x.S for early serial access */
	_tlbie(UART0_IO_BASE);
#endif

	port.membase = ioremap64(PPC440GX_UART1_ADDR, 8);
	port.irq = UART1_INT;
	port.uartclk = clocks.uart1;
	port.line = 1;

	if (early_serial_setup(&port) != 0) {
		printk("Early serial init of port 1 failed\n");
	}

#if defined(CONFIG_SERIAL_TEXT_DEBUG) || defined(CONFIG_KGDB)
	/* Configure debug serial access */
	gen550_init(1, &port);
#endif
}

static void __init alpr_setup_arch(void)
{
	alpr_set_emacdata();

	ibm440gx_tah_enable();

	ibm440gx_get_clocks(&clocks, 33333333, 0);
	ocp_sys_info.opb_bus_freq = clocks.opb;

	/* init to some ~sane value until calibrate_delay() runs */
        loops_per_jiffy = 50000000/HZ;

	/* Setup PCI host bridge */
	alpr_setup_hose();

#ifdef CONFIG_BLK_DEV_INITRD
	if (initrd_start)
		ROOT_DEV = Root_RAM0;
	else
#endif
#ifdef CONFIG_ROOT_NFS
		ROOT_DEV = Root_NFS;
#else
	ROOT_DEV = Root_HDA1;
#endif

	alpr_early_serial_map();

	/* Identify the system */
	printk("Prodrive ALPR port (DENX Software Engineering <sr@denx.de>)\n");
}

static void __init alpr_init(void)
{
	ibm440gx_l2c_setup(&clocks);
}

static void alpr_progress(char *buf, unsigned short val)
{
/*	printk("INF %s val: %d\n", buf, val);*/
}

static void alpr_restart(char *cmd)
{
	local_irq_disable();
	mtspr(SPRN_DBCR0, DBCR0_RST_CHIP);
}

void __init platform_init(unsigned long r3, unsigned long r4,
			  unsigned long r5, unsigned long r6, unsigned long r7)
{
	ibm440gx_platform_init(r3, r4, r5, r6, r7);

	ppc_md.progress = alpr_progress;
	ppc_md.setup_arch = alpr_setup_arch;
	ppc_md.show_cpuinfo = alpr_show_cpuinfo;
	ppc_md.get_irq = NULL;		/* Set in ppc4xx_pic_init() */

	ppc_md.calibrate_decr = alpr_calibrate_decr;
#ifdef CONFIG_KGDB
	ppc_md.early_serial_map = alpr_early_serial_map;
#endif
	ppc_md.init = alpr_init;
	ppc_md.restart = alpr_restart;
}

static int __init alpr_system_init(void)
{
	platform_device_register(&alpr_fpga_uart_device[0]);
	platform_device_register(&alpr_fpga_uart_device[1]);
	platform_device_register(&alpr_fpga_uart_device[2]);

	platform_device_register(&alpr_ndfc_device);
        platform_device_register(&alpr_nand_device[0]);
        platform_device_register(&alpr_nand_device[1]);
        platform_device_register(&alpr_nand_device[2]);
        platform_device_register(&alpr_nand_device[3]);

        return 0;
}

int alpr_map_ibuf_fpga(void __iomem **vbar0, void __iomem **vbar1, void __iomem **vbar2,
		       phys_addr_t *pbar0, phys_addr_t *pbar1, phys_addr_t *pbar2,
		       int *irq)
{
	int err;

	/*
	 * Locate FPGA on pci bus
	 */
	dev = pci_get_device(PCI_VENDOR_ID_IBUF_FPGA, PCI_DEVICE_ID_IBUF_FPGA, NULL);
	if (dev == NULL) {
		printk(KERN_ERR "ALPR: FPGA not found on PCI bus\n");
		return -ENODEV;
	}

	/*
	 * Enable pci device upon first call
	 */
	if (fpga_map_count == 0) {
		err = pci_enable_device(dev);
		if (err) {
			printk(KERN_ERR "ALPR: Cannot enable PCI device\n");
			return err;
		}
	}

	/*
	 * Map BAR0 & BAR1 spaces and return their virtual addresses
	 */
	if (vbar0 != NULL) {
#if 0 /* test-only */
		*vbar0 = ioremap_nocache(pci_resource_start(dev, 0), pci_resource_len(dev, 0));
#else
		*vbar0 = ioremap_nocache(pci_resource_start(dev, 0), (1 << 20)); /* test-only: map only 1MB */
#endif
		if (!*vbar0) {
			printk(KERN_ERR "ALPR: Cannot map BAR0\n");
			return -ENODEV;
		}
	}

	if (vbar1 != NULL) {
		*vbar1 = ioremap_nocache(pci_resource_start(dev, 2), pci_resource_len(dev, 2));
		if (!*vbar1) {
			printk(KERN_ERR "ALPR: Cannot map BAR2\n");
			return -ENODEV;
		}
	}

	if (vbar2 != NULL) {
		*vbar2 = ioremap_nocache(pci_resource_start(dev, 3), pci_resource_len(dev, 3));
		if (!*vbar2) {
			printk(KERN_ERR "ALPR: Cannot map BAR3\n");
			return -ENODEV;
		}
	}

	if (pbar0 != NULL)
		*pbar0 = pci_resource_start(dev, 0);
	if (pbar1 != NULL)
		*pbar1 = pci_resource_start(dev, 2);
	if (pbar2 != NULL)
		*pbar2 = pci_resource_start(dev, 3);

	/*
	 * Don't forget the irq
	 */
	if (irq)
		*irq = dev->irq;

	fpga_map_count++;

	return 0;
}

int alpr_unmap_ibuf_fpga(void __iomem *vbar0, void __iomem *vbar1, void __iomem *vbar2)
{
	/*
	 * Unmap BAR's
	 */
	if (vbar0)
		iounmap(vbar0);
	if (vbar1)
		iounmap(vbar1);
	if (vbar2)
		iounmap(vbar2);

	/*
	 * Disable pci device upon last call
	 */
	if (fpga_map_count == 1)
		pci_disable_device(dev);

	fpga_map_count--;

	return 0;
}

arch_initcall(alpr_system_init);

EXPORT_SYMBOL(alpr_map_ibuf_fpga);
EXPORT_SYMBOL(alpr_unmap_ibuf_fpga);
