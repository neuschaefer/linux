/*
 * arch/ppc/platforms/4xx/yosemite.c
 *
 * Yosemite and Yellowstone board specific routines
 *
 * Wade Farnsworth <wfarnsworth@mvista.com>
 * Copyright 2004 MontaVista Software Inc.
 *
 * John Otken <jotken@softadvances.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/stddef.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/initrd.h>
#include <linux/irq.h>
#include <linux/root_dev.h>
#include <linux/tty.h>
#include <linux/serial.h>
#include <linux/serial_core.h>

#include <asm/machdep.h>
#include <asm/ocp.h>
#include <asm/bootinfo.h>
#include <asm/ppcboot.h>

#include <syslib/ibm440gx_common.h>

/*
 * The AMCC Yosemite and Yellowstones boards share the same PC board.
 * The Yosemite has a 440EP and USB support.  The Yellowstone has a
 * 440GR and no USB hardware.  The 440EP and 440GR have the same PVR.
 */
#ifdef CONFIG_YELLOWSTONE
#define BOARDNAME  "440GR Yellowstone"
#else
#define BOARDNAME  "440EP Yosemite"
#endif


extern bd_t __res;

static struct ibm44x_clocks clocks __initdata;


/*
 * Yosemite external IRQ triggering/polarity settings
 */
unsigned char ppc4xx_uic_ext_irq_cfg[] __initdata = {
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE), /* IRQ0: ETH0 */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE), /* IRQ1: ETH1 */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE), /* IRQ2: PCI_INTA */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE), /* IRQ3: STTM_ALERT */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE), /* IRQ4: GPIO44 */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE), /* IRQ5: GND */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE), /* IRQ6: GPIO45 */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE), /* IRQ7: GPIO46 */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE), /* IRQ8: GPIO47 */
	(IRQ_SENSE_LEVEL | IRQ_POLARITY_NEGATIVE), /* IRQ9: GPIO48 */
};


static void __init
yosemite_calibrate_decr(void)
{
	unsigned int freq;

	if (mfspr(SPRN_CCR1) & CCR1_TCS)
		freq = YOSEMITE_TMRCLK;
	else
		freq = clocks.cpu;

	ibm44x_calibrate_decr(freq);
}

static int
yosemite_show_cpuinfo(struct seq_file *m)
{
	seq_printf(m, "vendor\t\t: AMCC\n");
	seq_printf(m, "machine\t\t: PPC" BOARDNAME "\n");

	return 0;
}

static inline int
yosemite_map_irq(struct pci_dev *dev, unsigned char idsel, unsigned char pin)
{
	static char pci_irq_table[][4] =
		/*
		 *      PCI IDSEL/INTPIN->INTLINE
		 *         A   B   C   D
		 */
		{
			{ 25, 25, 25, 25 },     /* IDSEL 1 - PCI Slot 0 */
		};

	const long min_idsel = 12, max_idsel = 12, irqs_per_slot = 4;
	return PCI_IRQ_TABLE_LOOKUP;
}

static void __init yosemite_set_emacdata(void)
{
	struct ocp_def *def;
	struct ocp_func_emac_data *emacdata;

	/* Set mac_addr and phy mode for each EMAC */

	def = ocp_get_one_device(OCP_VENDOR_IBM, OCP_FUNC_EMAC, 0);
	emacdata = def->additions;
	memcpy(emacdata->mac_addr, __res.bi_enetaddr, 6);
	emacdata->phy_mode = PHY_MODE_RMII;

	def = ocp_get_one_device(OCP_VENDOR_IBM, OCP_FUNC_EMAC, 1);
	emacdata = def->additions;
	memcpy(emacdata->mac_addr, __res.bi_enet1addr, 6);
	emacdata->phy_mode = PHY_MODE_RMII;
}

static int
yosemite_exclude_device(unsigned char bus, unsigned char devfn)
{
	return (bus == 0 && devfn == 0);
}

#define PCI_READW(offset)				\
        (readw((void *)((u32)pci_reg_base+offset)))

#define PCI_WRITEW(value, offset)				\
	(writew(value, (void *)((u32)pci_reg_base+offset)))

#define PCI_WRITEL(value, offset)				\
	(writel(value, (void *)((u32)pci_reg_base+offset)))

static void __init
yosemite_setup_pci(void)
{
	void *pci_reg_base;
	unsigned long memory_size;
	memory_size = ppc_md.find_end_of_memory();

	pci_reg_base = ioremap64(YOSEMITE_PCIL0_BASE, YOSEMITE_PCIL0_SIZE);

	/* Enable PCI I/O, Mem, and Busmaster cycles */
	PCI_WRITEW(PCI_READW(PCI_COMMAND) |
		   PCI_COMMAND_MEMORY |
		   PCI_COMMAND_MASTER, PCI_COMMAND);

	/* Disable region first */
	PCI_WRITEL(0, YOSEMITE_PCIL0_PMM0MA);

	/* PLB starting addr: 0x00000000A0000000 */
	PCI_WRITEL(YOSEMITE_PCI_PHY_MEM_BASE, YOSEMITE_PCIL0_PMM0LA);

	/* PCI start addr, 0xA0000000 (PCI Address) */
	PCI_WRITEL(YOSEMITE_PCI_MEM_BASE, YOSEMITE_PCIL0_PMM0PCILA);
	PCI_WRITEL(0, YOSEMITE_PCIL0_PMM0PCIHA);

	/* Enable no pre-fetch, enable region */
	PCI_WRITEL(((0xffffffff -
		     (YOSEMITE_PCI_UPPER_MEM - YOSEMITE_PCI_MEM_BASE)) | 0x01),
		   YOSEMITE_PCIL0_PMM0MA);

	/* Disable region one */
	PCI_WRITEL(0, YOSEMITE_PCIL0_PMM1MA);
	PCI_WRITEL(0, YOSEMITE_PCIL0_PMM1LA);
	PCI_WRITEL(0, YOSEMITE_PCIL0_PMM1PCILA);
	PCI_WRITEL(0, YOSEMITE_PCIL0_PMM1PCIHA);
	PCI_WRITEL(0, YOSEMITE_PCIL0_PMM1MA);

	/* Disable region two */
	PCI_WRITEL(0, YOSEMITE_PCIL0_PMM2MA);
	PCI_WRITEL(0, YOSEMITE_PCIL0_PMM2LA);
	PCI_WRITEL(0, YOSEMITE_PCIL0_PMM2PCILA);
	PCI_WRITEL(0, YOSEMITE_PCIL0_PMM2PCIHA);
	PCI_WRITEL(0, YOSEMITE_PCIL0_PMM2MA);

	/* Now configure the PCI->PLB windows, we only use PTM1
	 *
	 * For Inbound flow, set the window size to all available memory
	 * This is required because if size is smaller,
	 * then Eth/PCI DD would fail as PCI card not able to access
	 * the memory allocated by DD.
	 */

	PCI_WRITEL(0, YOSEMITE_PCIL0_PTM1MS);   /* disabled region 1 */
	PCI_WRITEL(0, YOSEMITE_PCIL0_PTM1LA);   /* begin of address map */

	memory_size = 1 << fls(memory_size - 1);

	/* Size low + Enabled */
	PCI_WRITEL((0xffffffff - (memory_size - 1)) | 0x1, YOSEMITE_PCIL0_PTM1MS);

	eieio();
	iounmap(pci_reg_base);
}

static void __init
yosemite_setup_hose(void)
{
	unsigned int bar_response, bar;
	struct pci_controller *hose;

	yosemite_setup_pci();

	hose = pcibios_alloc_controller();

	if (!hose)
		return;

	hose->first_busno = 0;
	hose->last_busno = 0xff;

	hose->pci_mem_offset = YOSEMITE_PCI_MEM_OFFSET;

	pci_init_resource(&hose->io_resource,
			  YOSEMITE_PCI_LOWER_IO,
			  YOSEMITE_PCI_UPPER_IO,
			  IORESOURCE_IO,
			  "PCI host bridge");

	pci_init_resource(&hose->mem_resources[0],
			  YOSEMITE_PCI_LOWER_MEM,
			  YOSEMITE_PCI_UPPER_MEM,
			  IORESOURCE_MEM,
			  "PCI host bridge");

	ppc_md.pci_exclude_device = yosemite_exclude_device;

	hose->io_space.start = YOSEMITE_PCI_LOWER_IO;
	hose->io_space.end = YOSEMITE_PCI_UPPER_IO;
	hose->mem_space.start = YOSEMITE_PCI_LOWER_MEM;
	hose->mem_space.end = YOSEMITE_PCI_UPPER_MEM;
	isa_io_base =
		(unsigned long)ioremap64(YOSEMITE_PCI_IO_BASE, YOSEMITE_PCI_IO_SIZE);
	hose->io_base_virt = (void *)isa_io_base;

	setup_indirect_pci(hose,
			   YOSEMITE_PCI_CFGA_PLB32,
			   YOSEMITE_PCI_CFGD_PLB32);
	hose->set_cfg_type = 1;

	/* Zero config bars */
	for (bar = PCI_BASE_ADDRESS_1; bar <= PCI_BASE_ADDRESS_2; bar += 4) {
		early_write_config_dword(hose, hose->first_busno,
					 PCI_FUNC(hose->first_busno), bar,
					 0x00000000);
		early_read_config_dword(hose, hose->first_busno,
					PCI_FUNC(hose->first_busno), bar,
					&bar_response);
	}

	hose->last_busno = pciauto_bus_scan(hose, hose->first_busno);

	ppc_md.pci_swizzle = common_swizzle;
	ppc_md.pci_map_irq = yosemite_map_irq;
}

static void __init
yosemite_early_serial_map(void)
{
	struct uart_port port;

	/* Setup ioremapped serial port access */
	memset(&port, 0, sizeof(port));
	port.membase = ioremap64(PPC440EP_UART0_ADDR, 8);
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
#endif

	port.membase = ioremap64(PPC440EP_UART1_ADDR, 8);
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
yosemite_setup_arch(void)
{
	yosemite_set_emacdata();

	ibm440gx_get_clocks(&clocks, YOSEMITE_SYSCLK, 6 * 1843200);
	ocp_sys_info.opb_bus_freq = clocks.opb;

	/* init to some ~sane value until calibrate_delay() runs */
        loops_per_jiffy = 50000000/HZ;

	/* Setup PCI host bridge */
	yosemite_setup_hose();

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

	yosemite_early_serial_map();

	/* Identify the system */
	printk( "AMCC PowerPC " BOARDNAME " Platform\n" );
}

void __init platform_init(unsigned long r3, unsigned long r4,
			  unsigned long r5, unsigned long r6, unsigned long r7)
{
	ibm44x_platform_init(r3, r4, r5, r6, r7);

	ppc_md.setup_arch = yosemite_setup_arch;
	ppc_md.show_cpuinfo = yosemite_show_cpuinfo;
	ppc_md.get_irq = NULL;          /* Set in ppc4xx_pic_init() */

	ppc_md.calibrate_decr = yosemite_calibrate_decr;

#ifdef CONFIG_KGDB
	ppc_md.early_serial_map = yosemite_early_serial_map;
#endif
}
