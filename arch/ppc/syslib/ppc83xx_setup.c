/*
 * MPC83XX common board code
 *
 * Maintainer: Kumar Gala <galak@kernel.crashing.org>
 *
 * Copyright 2005 Freescale Semiconductor Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the  GNU General Public License along
 * with this program; if not, write  to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Added PCI support -- Tony Li <tony.li@freescale.com>
 */

#include <linux/types.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/serial.h>
#include <linux/tty.h>	/* for linux/serial_core.h */
#include <linux/serial_core.h>
#include <linux/serial_8250.h>

#include <asm/time.h>
#include <asm/mpc83xx.h>
#include <asm/mmu.h>
#include <asm/ppc_sys.h>
#include <asm/kgdb.h>
#include <asm/delay.h>
#include <asm/machdep.h>

#include <syslib/ppc83xx_setup.h>
#if defined(CONFIG_PCI)
#include <asm/delay.h>
#include <syslib/ppc83xx_pci.h>
#endif

phys_addr_t immrbar;

/* Return the amount of memory */
unsigned long __init
mpc83xx_find_end_of_memory(void)
{
        bd_t *binfo;

        binfo = (bd_t *) __res;

        return binfo->bi_memsize;
}

long __init
mpc83xx_time_init(void)
{
	bd_t *binfo = (bd_t *)__res;
	u32 *spcr = ioremap(binfo->bi_immr_base + MPC83xx_SPCR, 4);

	*spcr |= MPC83xx_SPCR_TBEN;

	iounmap(spcr);

	return 0;
}

/* The decrementer counts at the system (internal) clock freq divided by 4 */
void __init
mpc83xx_calibrate_decr(void)
{
        bd_t *binfo = (bd_t *) __res;
        unsigned int freq, divisor;

	freq = binfo->bi_busfreq;
	divisor = 4;
	tb_ticks_per_jiffy = freq / HZ / divisor;
	tb_to_us = mulhwu_scale_factor(freq / divisor, 1000000);
}

#ifdef CONFIG_SERIAL_8250
void __init
mpc83xx_early_serial_map(void)
{
#if defined(CONFIG_SERIAL_TEXT_DEBUG) || defined(CONFIG_KGDB)
	struct uart_port serial_req;
#endif
	struct plat_serial8250_port *pdata;
	bd_t *binfo = (bd_t *) __res;
	pdata = (struct plat_serial8250_port *) ppc_sys_get_pdata(MPC83xx_DUART);

	/* Setup serial port access */
	pdata[0].uartclk = binfo->bi_busfreq;
	pdata[0].mapbase += binfo->bi_immr_base;
	pdata[0].membase = ioremap(pdata[0].mapbase, 0x100);

#if defined(CONFIG_SERIAL_TEXT_DEBUG) || defined(CONFIG_KGDB)
	memset(&serial_req, 0, sizeof (serial_req));
	serial_req.iotype = UPIO_MEM;
	serial_req.mapbase = pdata[0].mapbase;
	serial_req.membase = pdata[0].membase;
	serial_req.regshift = 0;

	gen550_init(0, &serial_req);
#endif

	pdata[1].uartclk = binfo->bi_busfreq;
	pdata[1].mapbase += binfo->bi_immr_base;
	pdata[1].membase = ioremap(pdata[1].mapbase, 0x100);

#if defined(CONFIG_SERIAL_TEXT_DEBUG) || defined(CONFIG_KGDB)
	/* Assume gen550_init() doesn't modify serial_req */
	serial_req.mapbase = pdata[1].mapbase;
	serial_req.membase = pdata[1].membase;

	gen550_init(1, &serial_req);
#endif
}
#endif

void
mpc83xx_restart(char *cmd)
{
	bd_t *binfo = (bd_t *)__res;

	u32 *rcr = ioremap(binfo->bi_immr_base + MPC83xx_RCR, 4);
	u32 *rpr = ioremap(binfo->bi_immr_base + MPC83xx_RPR, 4);

	/* apply reset protect unlock command to
	 * reset control protection  register */
	*rpr = MPC83xx_RPR_RSTE;

	/* apply software hard reset to
	 * reset control register*/
	*rcr = MPC83xx_RCR_SWHR;

	/* not reached, but... */
	iounmap(rcr);
	iounmap(rpr);
	for (;;) ;
}

void
mpc83xx_power_off(void)
{
	local_irq_disable();
	for(;;);
}

void
mpc83xx_halt(void)
{
	local_irq_disable();
	for(;;);
}

#if defined(CONFIG_PCI)
void __init
mpc83xx_setup_pci1(struct pci_controller *hose)
{
	u16 reg16;
	volatile immr_pcictrl_t * pci_ctrl;
	volatile immr_ios_t * ios;
	bd_t *binfo = (bd_t *) __res;

	pci_ctrl = ioremap(binfo->bi_immr_base + 0x8500, sizeof(immr_pcictrl_t));
	ios = ioremap(binfo->bi_immr_base + 0x8400, sizeof(immr_ios_t));

	/*
	 * Configure PCI Outbound Translation Windows
	 */
	ios->potar0 = (MPC83xx_PCI1_LOWER_MEM >> 12) & POTAR_TA_MASK;
	ios->pobar0 = (MPC83xx_PCI1_LOWER_MEM >> 12) & POBAR_BA_MASK;
	ios->pocmr0 = POCMR_EN |
		(((0xffffffff - (MPC83xx_PCI1_UPPER_MEM -
				MPC83xx_PCI1_LOWER_MEM)) >> 12) & POCMR_CM_MASK);

	/* mapped to PCI1 IO space */
	ios->potar1 = (MPC83xx_PCI1_LOWER_IO >> 12) & POTAR_TA_MASK;
	ios->pobar1 = (MPC83xx_PCI1_IO_BASE >> 12) & POBAR_BA_MASK;
	ios->pocmr1 = POCMR_EN | POCMR_IO |
		(((0xffffffff - (MPC83xx_PCI1_UPPER_IO -
				MPC83xx_PCI1_LOWER_IO)) >> 12) & POCMR_CM_MASK);

	/*
	 * Configure PCI Inbound Translation Windows
	 */
	pci_ctrl->pitar1 = 0x0;
	pci_ctrl->pibar1 = 0x0;
	pci_ctrl->piebar1 = 0x0;
	pci_ctrl->piwar1 = PIWAR_EN | PIWAR_PF | PIWAR_RTT_SNOOP | PIWAR_WTT_SNOOP | PIWAR_IWS_2G;

	/*
	 * Release PCI RST signal
	 */
	pci_ctrl->gcr = 0;
	udelay(2000);
	pci_ctrl->gcr = 1;
	udelay(2000);

	reg16 = 0xff;
	early_read_config_word(hose, hose->first_busno, 0, PCI_COMMAND, &reg16);
	reg16 |= PCI_COMMAND_SERR | PCI_COMMAND_MASTER | PCI_COMMAND_MEMORY;
	early_write_config_word(hose, hose->first_busno, 0, PCI_COMMAND, reg16);

	/*
	 * Clear non-reserved bits in status register.
	 */
	early_write_config_word(hose, hose->first_busno, 0, PCI_STATUS, 0xffff);
	early_write_config_byte(hose, hose->first_busno, 0, PCI_LATENCY_TIMER, 0x80);

	iounmap(pci_ctrl);
	iounmap(ios);
}

#ifdef CONFIG_MPC83xx_PCI2
void __init
mpc83xx_setup_pci2(struct pci_controller *hose)
{
	u16 reg16;
	volatile immr_pcictrl_t * pci_ctrl;
	volatile immr_ios_t * ios;
	bd_t *binfo = (bd_t *) __res;

	pci_ctrl = ioremap(binfo->bi_immr_base + 0x8600, sizeof(immr_pcictrl_t));
	ios = ioremap(binfo->bi_immr_base + 0x8400, sizeof(immr_ios_t));

	/*
	 * Configure PCI Outbound Translation Windows
	 */
	ios->potar3 = (MPC83xx_PCI2_LOWER_MEM >> 12) & POTAR_TA_MASK;
	ios->pobar3 = (MPC83xx_PCI2_LOWER_MEM >> 12) & POBAR_BA_MASK;
	ios->pocmr3 = POCMR_EN | POCMR_DST |
		(((0xffffffff - (MPC83xx_PCI2_UPPER_MEM -
				MPC83xx_PCI2_LOWER_MEM)) >> 12) & POCMR_CM_MASK);

	/* mapped to PCI2 IO space */
	ios->potar4 = (MPC83xx_PCI2_LOWER_IO >> 12) & POTAR_TA_MASK;
	ios->pobar4 = (MPC83xx_PCI2_IO_BASE >> 12) & POBAR_BA_MASK;
	ios->pocmr4 = POCMR_EN | POCMR_DST | POCMR_IO |
		(((0xffffffff - (MPC83xx_PCI2_UPPER_IO -
				MPC83xx_PCI2_LOWER_IO)) >> 12) & POCMR_CM_MASK);

	/*
	 * Configure PCI Inbound Translation Windows
	 */
	pci_ctrl->pitar1 = 0x0;
	pci_ctrl->pibar1 = 0x0;
	pci_ctrl->piebar1 = 0x0;
	pci_ctrl->piwar1 = PIWAR_EN | PIWAR_PF | PIWAR_RTT_SNOOP | PIWAR_WTT_SNOOP | PIWAR_IWS_2G;

	/*
	 * Release PCI RST signal
	 */
	pci_ctrl->gcr = 0;
	udelay(2000);
	pci_ctrl->gcr = 1;
	udelay(2000);

	reg16 = 0xff;
	early_read_config_word(hose, hose->first_busno, 0, PCI_COMMAND, &reg16);
	reg16 |= PCI_COMMAND_SERR | PCI_COMMAND_MASTER | PCI_COMMAND_MEMORY;
	early_write_config_word(hose, hose->first_busno, 0, PCI_COMMAND, reg16);

	/*
	 * Clear non-reserved bits in status register.
	 */
	early_write_config_word(hose, hose->first_busno, 0, PCI_STATUS, 0xffff);
	early_write_config_byte(hose, hose->first_busno, 0, PCI_LATENCY_TIMER, 0x80);

	iounmap(pci_ctrl);
	iounmap(ios);
}
#endif

/*
 * NOTICE for the MPC83xx SYS Freescale evaluation board:
 *
 * PCI buses can be enabled only if SYS board combinates with PIB
 * (Platform IO Board) board which provide 3 PCI slots. There is 2 PCI buses
 * and 3 PCI slots, so people must configure the routes between them before
 * enable PCI bus. This routes are under the control of PCA9555PW device which
 * can be accessed via I2C bus 2 and are configured by firmware. Refer to
 * Freescale to get more information about firmware configuration.
 */
void __init
mpc83xx_setup_hose(void)
{
	u32 val32;
	volatile immr_clk_t * clk;
	struct pci_controller * hose1;
#ifdef CONFIG_MPC83xx_PCI2
	struct pci_controller * hose2;
#endif
	bd_t * binfo = (bd_t *)__res;

	clk = ioremap(binfo->bi_immr_base + 0xA00,
			sizeof(immr_clk_t));

	/*
	 * Configure PCI_CLK_OUTPUT
	 */
	val32 = clk->occr;
	udelay(2000);
	
#ifdef CONFIG_TQM834x
	/*
	 * WARNING! only PCI_CLK_OUTPUT1 is enabled for the TQM834x as this is
	 * the one line actually used for clocking all external PCI devices in
	 * current setup of the STK85xx.  Enabling other PCI_CLK_OUTPUT lines
	 * may lead to board's hang for unknown reasons - particularly
	 * PCI_CLK_OUTPUT6 and PCI_CLK_OUTPUT7 are known to hang the board;
	 * this issue is under investigation (18 oct 05)
	 */
	val32 = OCCR_PCICOE1; 
#else
	/* enable all PCI_CLK_OUTs */
	val32 = (OCCR_PCICOE0 | OCCR_PCICOE1 | OCCR_PCICOE2 | OCCR_PCICOE3 \
		 | OCCR_PCICOE4 | OCCR_PCICOE5 | OCCR_PCICOE6 | OCCR_PCICOE7);
#endif
	/* set frequency of the PCI and clock outputs for external devicesc
	 * according to system clocking settings */
	if (clk->spmr & SPMR_CKID) {
		/* PCI Clock is 1/2 of CONFIG_83XX_CLKIN so need to set up OCCR
		 * fields accordingly */
		val32 |= (OCCR_PCI1CR | OCCR_PCI2CR);
		
		val32 |= (OCCR_PCICD0 | OCCR_PCICD1 | OCCR_PCICD2 \
			  | OCCR_PCICD3 | OCCR_PCICD4 | OCCR_PCICD5 \
			  | OCCR_PCICD6 | OCCR_PCICD7);
	}
	
	clk->occr = val32;
	udelay(2000);
	iounmap(clk);

	hose1 = pcibios_alloc_controller();
	if(!hose1)
		return;

	hose1->bus_offset = 0;
	hose1->first_busno = 0;
	hose1->last_busno = 0xff;

	setup_indirect_pci(hose1, binfo->bi_immr_base + PCI1_CFG_ADDR_OFFSET,
			binfo->bi_immr_base + PCI1_CFG_DATA_OFFSET);
	hose1->set_cfg_type = 1;

	mpc83xx_setup_pci1(hose1);

	hose1->pci_mem_offset = MPC83xx_PCI1_MEM_OFFSET;
	hose1->mem_space.start = MPC83xx_PCI1_LOWER_MEM;
	hose1->mem_space.end = MPC83xx_PCI1_UPPER_MEM;

	hose1->io_base_phys = MPC83xx_PCI1_IO_BASE;
	hose1->io_space.start = MPC83xx_PCI1_LOWER_IO;
	hose1->io_space.end = MPC83xx_PCI1_UPPER_IO;
#ifdef CONFIG_MPC83xx_PCI2
	isa_io_base = (unsigned long)ioremap(MPC83xx_PCI1_IO_BASE,
			MPC83xx_PCI1_IO_SIZE + MPC83xx_PCI2_IO_SIZE);
#else
	isa_io_base = (unsigned long)ioremap(MPC83xx_PCI1_IO_BASE,
			MPC83xx_PCI1_IO_SIZE);
#endif /* CONFIG_MPC83xx_PCI2 */
	hose1->io_base_virt = (void *)isa_io_base;
	/* setup resources */
	pci_init_resource(&hose1->io_resource,
			MPC83xx_PCI1_LOWER_IO,
			MPC83xx_PCI1_UPPER_IO,
			IORESOURCE_IO, "PCI host bridge 1");
	pci_init_resource(&hose1->mem_resources[0],
			MPC83xx_PCI1_LOWER_MEM,
			MPC83xx_PCI1_UPPER_MEM,
			IORESOURCE_MEM, "PCI host bridge 1");

	hose1->last_busno = pciauto_bus_scan(hose1, hose1->first_busno);

#ifdef CONFIG_MPC83xx_PCI2
	hose2 = pcibios_alloc_controller();
	if(!hose2)
		return;

	hose2->bus_offset = hose1->last_busno + 1;
	hose2->first_busno = hose1->last_busno + 1;
	hose2->last_busno = 0xff;
	setup_indirect_pci(hose2, binfo->bi_immr_base + PCI2_CFG_ADDR_OFFSET,
			binfo->bi_immr_base + PCI2_CFG_DATA_OFFSET);
	hose2->set_cfg_type = 1;

	mpc83xx_setup_pci2(hose2);

	hose2->pci_mem_offset = MPC83xx_PCI2_MEM_OFFSET;
	hose2->mem_space.start = MPC83xx_PCI2_LOWER_MEM;
	hose2->mem_space.end = MPC83xx_PCI2_UPPER_MEM;

	hose2->io_base_phys = MPC83xx_PCI2_IO_BASE;
	hose2->io_space.start = MPC83xx_PCI2_LOWER_IO;
	hose2->io_space.end = MPC83xx_PCI2_UPPER_IO;
	hose2->io_base_virt = (void *)(isa_io_base + MPC83xx_PCI1_IO_SIZE);
	/* setup resources */
	pci_init_resource(&hose2->io_resource,
			MPC83xx_PCI2_LOWER_IO,
			MPC83xx_PCI2_UPPER_IO,
			IORESOURCE_IO, "PCI host bridge 2");
	pci_init_resource(&hose2->mem_resources[0],
			MPC83xx_PCI2_LOWER_MEM,
			MPC83xx_PCI2_UPPER_MEM,
			IORESOURCE_MEM, "PCI host bridge 2");

	hose2->last_busno = pciauto_bus_scan(hose2, hose2->first_busno);
#endif /* CONFIG_MPC83xx_PCI2 */
}


void __init
mpc834x_pcibios_fixup(void)
{
	struct pci_dev *dev = NULL;
	unsigned int class;
	int i;
	
	if ((dev = pci_find_device(PCI_VENDOR_ID_FREESCALE,
				   PCI_DEVICE_ID_FREESCALE_MPC8349E, NULL))) {
		class = dev->class >> 8;
		if (class == PCI_CLASS_BRIDGE_OTHER) {
			/*
			 * at least rev 1.1 of the MPC8349E chip has the class
			 * wrongly set in the host/PCI bridge config space
			 * register; this leads to the host/PCI bridge being
			 * treated as a regular client device, trying to assign
			 * PCI resources to it etc.
			 *
			 */
			dev->class = (PCI_CLASS_BRIDGE_HOST << 8) & ~0xf;

			/* for some odd reasons the host/PCI bridge behaves
			 * like an agent device and would claim PCI mem/io/irq
			 * resources, so we make these resources inactive (i.e.
			 * the bridge will not claim resources from itself)
			 */
			for (i = 0; i < DEVICE_COUNT_RESOURCE; i++) 
				dev->resource[i].flags = IORESOURCE_UNSET;
		}
	}	
	
}
#endif /*CONFIG_PCI*/
