/*
 * arch/ppc/platforms/4xx/p3p440.c
 *
 * P3P440 board specific routines
 *
 * Copyright (c) 2005 DENX Software Engineering
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

#include <asm/system.h>
#include <asm/pgtable.h>
#include <asm/page.h>
#include <asm/dma.h>
#include <asm/io.h>
#include <asm/machdep.h>
#include <asm/ocp.h>
#include <asm/pci-bridge.h>
#include <asm/time.h>
#include <asm/bootinfo.h>
#include <asm/ppc4xx_pic.h>
#include <asm/ppcboot.h>
#include <asm/tlbflush.h>

#include <syslib/gen550.h>
#include <syslib/ibm440gp_common.h>

#include <platforms/4xx/ibm440gp.h>

extern bd_t __res;

static struct ibm44x_clocks clocks __initdata;

/*
 * P3P440 external IRQ triggering/polarity settings
 */
unsigned char ppc4xx_uic_ext_irq_cfg[] __initdata = {
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE),	/* IRQ0: PCI_INTA */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE),	/* IRQ1: PCI_INTB */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE),	/* IRQ2: PCI_INTC */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE),	/* IRQ3: PCI_INTD */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE),	/* IRQ4: ETH INT */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE),	/* IRQ5: USB */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE),	/*  */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE),	/*  */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE),	/*  */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE),	/*  */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE),	/*  */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE),	/*  */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE),	/*  */
};

static void __init
p3p440_calibrate_decr(void)
{
	unsigned int freq;

	freq = clocks.cpu;

	ibm44x_calibrate_decr(freq);
}

static int
p3p440_show_cpuinfo(struct seq_file *m)
{
	seq_printf(m, "vendor\t\t: Prodrive\n");
	seq_printf(m, "machine\t\t: P3P440\n");

	return 0;
}

#if defined(CONFIG_I2C_IBM_IIC) && defined(CONFIG_SENSORS_MAX6900)
extern ulong max6900_get_rtc_time(void);
extern int max6900_set_rtc_time(unsigned long nowtime);

static int __init
p3p440_rtc_hookup(void)
{
	struct timespec	tv;

        ppc_md.set_rtc_time = max6900_set_rtc_time;
        ppc_md.get_rtc_time = max6900_get_rtc_time;

	tv.tv_nsec = 0;
	tv.tv_sec = (ppc_md.get_rtc_time)();
	do_settimeofday(&tv);

	return 0;
}
late_initcall(p3p440_rtc_hookup);
#endif

static inline int
p3p440_map_irq(struct pci_dev *dev, unsigned char idsel, unsigned char pin)
{
	static char pci_irq_table[][4] =
		/*
		 *	PCI IDSEL/INTPIN->INTLINE
		 * 	   A   B   C   D
		 */
		{
			/*
			 * Note: PMC Module A has device id 0! This could
			 * be a problem, since the PPC440 pci bridge is located
			 * on this device. -sr
			 */
			{PCI_INTB, PCI_INTC, PCI_INTD, PCI_INTA}, /* device 2 */
			{PCI_INTC, PCI_INTD, PCI_INTA, PCI_INTB}, /* device 3 */
			{PCI_INTD, PCI_INTA, PCI_INTB, PCI_INTC}, /* device 4 */
		};

	const long min_idsel = 2, max_idsel = 4, irqs_per_slot = 4;
	return PCI_IRQ_TABLE_LOOKUP;
}

#define PCIX_WRITEL(value, offset) \
	(writel(value, pcix_reg_base + offset))

/*
 * FIXME: This is only here to "make it work".  This will move
 * to a ibm_pcix.c which will contain a generic IBM PCIX bridge
 * configuration library. -Matt
 */
static void __init
p3p440_setup_pcix(void)
{
	void __iomem *pcix_reg_base;

	pcix_reg_base = ioremap64(PCIX0_REG_BASE, PCIX_REG_SIZE);

	/* Disable all windows */
	PCIX_WRITEL(0, PCIX0_POM0SA);
	PCIX_WRITEL(0, PCIX0_POM1SA);
	PCIX_WRITEL(0, PCIX0_POM2SA);
	PCIX_WRITEL(0, PCIX0_PIM0SA);
	PCIX_WRITEL(0, PCIX0_PIM1SA);
	PCIX_WRITEL(0, PCIX0_PIM2SA);

	/* Setup 2GB PLB->PCI outbound mem window (3_8000_0000->0_8000_0000) */
	PCIX_WRITEL(0x00000003, PCIX0_POM0LAH);
	PCIX_WRITEL(0x80000000, PCIX0_POM0LAL);
	PCIX_WRITEL(0x00000000, PCIX0_POM0PCIAH);
	PCIX_WRITEL(0x80000000, PCIX0_POM0PCIAL);
	PCIX_WRITEL(0x80000001, PCIX0_POM0SA);

	/* Setup 2GB PCI->PLB inbound memory window at 0, enable MSIs */
	PCIX_WRITEL(0x00000000, PCIX0_PIM0LAH);
	PCIX_WRITEL(0x00000000, PCIX0_PIM0LAL);
	PCIX_WRITEL(0x80000007, PCIX0_PIM0SA);

	eieio();
}

static int is_monarch(void)
{
	void __iomem *gpio_base;
	ulong val;

	gpio_base = ioremap64(GPIO_BASE, 0x1000);
	val = in_be32((void *)(gpio_base+0x1c));
	iounmap((void *)gpio_base);

	if (val  & CFG_MONARCH_IO)
		return 0;
	else
		return 1;
}

static void __init
p3p440_setup_hose(void)
{
	struct pci_controller *hose;

	if (!is_monarch()) {
		/*
		 * REMARK: This Non-Monarch mode need perhaps some changes.
		 * It's not tested at all, because of lack of hardware. --sr
		 */
		printk("P3P440-PCI: Non-Monarch detected, skipping PCI init!\n");
		return;
	}

	/* Configure windows on the PCI-X host bridge */
	p3p440_setup_pcix();

	hose = pcibios_alloc_controller();

	if (!hose)
		return;

	hose->first_busno = 0;
	hose->last_busno = 0xff;

	hose->pci_mem_offset = P3P440_PCI_MEM_OFFSET;

	pci_init_resource(&hose->io_resource,
			P3P440_PCI_LOWER_IO,
			P3P440_PCI_UPPER_IO,
			IORESOURCE_IO,
			"PCI host bridge");

	pci_init_resource(&hose->mem_resources[0],
			P3P440_PCI_LOWER_MEM,
			P3P440_PCI_UPPER_MEM,
			IORESOURCE_MEM,
			"PCI host bridge");

	hose->io_space.start = P3P440_PCI_LOWER_IO;
	hose->io_space.end = P3P440_PCI_UPPER_IO;
	hose->mem_space.start = P3P440_PCI_LOWER_MEM;
	hose->mem_space.end = P3P440_PCI_UPPER_MEM;
	hose->io_base_virt = ioremap64(P3P440_PCI_IO_BASE, P3P440_PCI_IO_SIZE);
	isa_io_base = (unsigned long)hose->io_base_virt;

	setup_indirect_pci(hose,
			P3P440_PCI_CFGA_PLB32,
			P3P440_PCI_CFGD_PLB32);
	hose->set_cfg_type = 1;

	hose->last_busno = pciauto_bus_scan(hose, hose->first_busno);

	ppc_md.pci_swizzle = common_swizzle;
	ppc_md.pci_map_irq = p3p440_map_irq;
}

static void __init
p3p440_early_serial_map(void)
{
	struct uart_port port;

	/* Setup ioremapped serial port access */
	memset(&port, 0, sizeof(port));
	port.membase = ioremap64(PPC440GP_UART0_ADDR, 8);
	port.irq = 0;
	port.uartclk = clocks.uart0;
	port.regshift = 0;
	port.iotype = SERIAL_IO_MEM;
	port.flags = ASYNC_BOOT_AUTOCONF | ASYNC_SKIP_TEST;
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

	port.membase = ioremap64(PPC440GP_UART1_ADDR, 8);
	port.irq = 1;
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

static void __init
p3p440_setup_arch(void)
{
	struct ocp_def *def;
	struct ocp_func_emac_data *emacdata;

	/* Set mac_addr for each EMAC */
	def = ocp_get_one_device(OCP_VENDOR_IBM, OCP_FUNC_EMAC, 0);
	emacdata = def->additions;
	emacdata->phy_map = 0x00000001;	/* Skip 0x00 */
	emacdata->phy_mode = PHY_MODE_RMII;
	memcpy(emacdata->mac_addr, __res.bi_enetaddr, 6);

	def = ocp_get_one_device(OCP_VENDOR_IBM, OCP_FUNC_EMAC, 1);
	emacdata = def->additions;
	emacdata->phy_map = 0x00000001;	/* Skip 0x00 */
	emacdata->phy_mode = PHY_MODE_RMII;
	memcpy(emacdata->mac_addr, __res.bi_enet1addr, 6);

	/*
	 * Determine various clocks.
	 * To be completely correct we should get SysClk
	 * from FPGA, because it can be changed by on-board switches
	 * --ebs
	 */
	ibm440gp_get_clocks(&clocks, 33333333, 0);
	ocp_sys_info.opb_bus_freq = clocks.opb;

	/* init to some ~sane value until calibrate_delay() runs */
        loops_per_jiffy = 50000000/HZ;

	/* Setup PCI host bridge */
	p3p440_setup_hose();

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

	p3p440_early_serial_map();

	/* Identify the system */
	printk("Prodrive P3P440 port (DENX Software Engineering (sr@denx.de))\n");
}

void __init platform_init(unsigned long r3, unsigned long r4,
		unsigned long r5, unsigned long r6, unsigned long r7)
{
	ibm44x_platform_init(r3, r4, r5, r6, r7);

	ppc_md.setup_arch = p3p440_setup_arch;
	ppc_md.show_cpuinfo = p3p440_show_cpuinfo;
	ppc_md.get_irq = NULL;		/* Set in ppc4xx_pic_init() */

	ppc_md.calibrate_decr = p3p440_calibrate_decr;
	ppc_md.time_init = NULL;
	ppc_md.set_rtc_time = NULL;
	ppc_md.get_rtc_time = NULL;

#ifdef CONFIG_KGDB
	ppc_md.early_serial_map = p3p440_early_serial_map;
#endif
}

