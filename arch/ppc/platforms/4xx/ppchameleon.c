/*
 * Support for DAVE PPC 405EP evaluation board (PPChameleon).
 *
 * Author: Wolfgang Denk, DENX Software Engineering, <wd@denx.de>
 *	   derived from "Octobus hppc405" board by haag.cc
 *	   (see http://www.haag.cc/downloads/files/octobus-hppc405-2.6.12.2-V0.1)
 *	   which in turn was obviously based on evb405ep.
 *         Maintained by Wolfgang Denk, <wd@denx.de>
 *
 * 2005 (c) Wolfgang Denk, DENX Software Engineering.  This file is
 * licensed under the terms of the GNU General Public License version 2.
 * This program is licensed "as is" without any warranty of any kind,
 * whether express or implied.
 */

#include <linux/init.h>
#include <linux/pci.h>
#include <linux/serial_core.h>
#include <linux/serial.h>

#include <asm/system.h>
#include <asm/pci-bridge.h>
#include <asm/machdep.h>
#include <asm/page.h>
#include <asm/time.h>
#include <asm/io.h>
#include <asm/ocp.h>
#include <platforms/4xx/ibm405ep.h>

#ifdef CONFIG_GEN_RTC
#include <asm/todc.h>
#endif

#include <asm/kgdb.h>

#undef DEBUG

#ifdef DEBUG
#define DBG(x...) printk(x)
#else
#define DBG(x...)
#endif


void *ppchameleon_rtc_base;

extern void gen550_init(int, struct uart_port *);
extern int early_serial_setup(struct uart_port *port);

/* Some IRQs unique to the board
 * Used by the generic 405 PCI setup functions in ppc4xx_pci.c
 */
int __init
ppc405_map_irq(struct pci_dev *dev, unsigned char idsel, unsigned char pin)
{
	static char pci_irq_table[][4] =
	    /*
	     *      PCI IDSEL/INTPIN->INTLINE
	     *      A       B       C       D
	     */
	{
		{28, 28, 28, 28},	/* IDSEL 1 - PCI slot 1 */
		{29, 29, 29, 29},	/* IDSEL 2 - PCI slot 2 */
		{30, 30, 30, 30},	/* IDSEL 3 - PCI slot 3 */
		{31, 31, 31, 31},	/* IDSEL 4 - PCI slot 4 */
	};

	const long min_idsel = 1, max_idsel = 4, irqs_per_slot = 4;
	return PCI_IRQ_TABLE_LOOKUP;
};


/* The serial clock for the chip is an internal clock determined by
 * different clock speeds/dividers.
 * Calculate the proper input baud rate and setup the serial driver.
 */

static void __init
ppchameleon_early_serial_map (void)
{
	u32 uart_div;
	struct uart_port port;
	int serial_baud_405ep;

	int size = sizeof (port);
	bd_t tmp = (bd_t) __res;

	/* bd_t *bip = &tmp; */
	uart_div = (mfdcr (DCRN_CPC0_UCR_BASE) & DCRN_CPC0_UCR_U0DIV);

	serial_baud_405ep = tmp.bi_intfreq / uart_div;
	/* serial_baud_405ep=__res.bi_baudrate * uart_div; */

	/* Setup ioremapped serial port access */
	memset (&port, 0, size);
	port.membase = (void *) ACTING_UART0_IO_BASE;
	port.irq = UART0_INT;
	port.uartclk = serial_baud_405ep;
	port.regshift = 0;
	port.iotype = SERIAL_IO_MEM;
	port.flags = ASYNC_BOOT_AUTOCONF | ASYNC_SKIP_TEST;
	port.line = 0;

	if (early_serial_setup (&port) != 0) {
		printk ("Early serial init of port 0 failed\n");
	}
}


void __init board_setup_arch (void)
{

	ppc4xx_setup_arch ();

	ibm_ocp_set_emac(0, 1);

	ppchameleon_early_serial_map ();

	printk ("DAVE PPchameleonEVB Board (Wolfgang Denk wd@denx.de)\n");
}

#if defined(CONFIG_BIOS_FIXUP)
void __init bios_fixup (struct pci_controller *hose, struct pcil0_regs *pcip)
{

	unsigned int bar_response, bar;

	/*
	 * Expected PCI mapping:
	 *
	 *  PLB addr             PCI memory addr
	 *  ---------------------       ---------------------
	 *  0000'0000 - 7fff'ffff <---  0000'0000 - 7fff'ffff
	 *  8000'0000 - Bfff'ffff --->  8000'0000 - Bfff'ffff
	 *
	 *  PLB addr             PCI io addr
	 *  ---------------------       ---------------------
	 *  e800'0000 - e800'ffff --->  0000'0000 - 0001'0000
	 *
	 * The following code is simplified by assuming that the bootrom
	 * has been well behaved in following this mapping.
	 */

#ifdef DEBUG
	int i;

	printk ("ioremap PCLIO_BASE = 0x%x\n", pcip);
	printk ("PCI bridge regs before fixup \n");
	for (i = 0; i <= 3; i++) {
		printk (" pmm%dma\t0x%x\n", i, in_le32 (&(pcip->pmm[i].ma)));
		printk (" pmm%dma\t0x%x\n", i, in_le32 (&(pcip->pmm[i].la)));
		printk (" pmm%dma\t0x%x\n", i,
			in_le32 (&(pcip->pmm[i].pcila)));
		printk (" pmm%dma\t0x%x\n", i,
			in_le32 (&(pcip->pmm[i].pciha)));
	}
	printk (" ptm1ms\t0x%x\n", in_le32 (&(pcip->ptm1ms)));
	printk (" ptm1la\t0x%x\n", in_le32 (&(pcip->ptm1la)));
	printk (" ptm2ms\t0x%x\n", in_le32 (&(pcip->ptm2ms)));
	printk (" ptm2la\t0x%x\n", in_le32 (&(pcip->ptm2la)));

#endif

	/* added for IBM boot rom version 1.15 bios bar changes  -AK */

	/* Disable region first */
	out_le32 ((void *) &(pcip->pmm[0].ma), 0x00000000);
	/* PLB starting addr, PCI: 0x80000000 */
	out_le32 ((void *) &(pcip->pmm[0].la), 0x80000000);
	/* PCI start addr, 0x80000000 */
	out_le32 ((void *) &(pcip->pmm[0].pcila), PPC405_PCI_MEM_BASE);
	/* 512MB range of PLB to PCI */
	out_le32 ((void *) &(pcip->pmm[0].pciha), 0x00000000);
	/* Enable no pre-fetch, enable region */
	out_le32 ((void *) &(pcip->pmm[0].ma), ((0xffffffff -
						 (PPC405_PCI_UPPER_MEM -
						  PPC405_PCI_MEM_BASE)) |
						0x01));

	/* Disable region one */
	out_le32 ((void *) &(pcip->pmm[1].ma), 0x00000000);
	out_le32 ((void *) &(pcip->pmm[1].la), 0x00000000);
	out_le32 ((void *) &(pcip->pmm[1].pcila), 0x00000000);
	out_le32 ((void *) &(pcip->pmm[1].pciha), 0x00000000);
	out_le32 ((void *) &(pcip->pmm[1].ma), 0x00000000);
	out_le32 ((void *) &(pcip->ptm1ms), 0x00000001);

	/* Disable region two */
	out_le32 ((void *) &(pcip->pmm[2].ma), 0x00000000);
	out_le32 ((void *) &(pcip->pmm[2].la), 0x00000000);
	out_le32 ((void *) &(pcip->pmm[2].pcila), 0x00000000);
	out_le32 ((void *) &(pcip->pmm[2].pciha), 0x00000000);
	out_le32 ((void *) &(pcip->pmm[2].ma), 0x00000000);
	out_le32 ((void *) &(pcip->ptm2ms), 0x00000000);
	out_le32 ((void *) &(pcip->ptm2la), 0x00000000);

	/* Zero config bars */
	for (bar = PCI_BASE_ADDRESS_1; bar <= PCI_BASE_ADDRESS_2; bar += 4) {
		early_write_config_dword (hose, hose->first_busno,
					  PCI_FUNC (hose->first_busno), bar,
					  0x00000000);
		early_read_config_dword (hose, hose->first_busno,
					 PCI_FUNC (hose->first_busno), bar,
					 &bar_response);
		DBG ("BUS %d, device %d, Function %d bar 0x%8.8x is 0x%8.8x\n", hose->first_busno, PCI_SLOT (hose->first_busno), PCI_FUNC (hose->first_busno), bar, bar_response);
	}
	/* end work arround */
#ifdef DEBUG

	printk ("PCI bridge regs after fixup \n");
	for (i = 0; i <= 3; i++) {
		printk (" pmm%dma\t0x%x\n", i, in_le32 (&(pcip->pmm[i].ma)));
		printk (" pmm%dma\t0x%x\n", i, in_le32 (&(pcip->pmm[i].la)));
		printk (" pmm%dma\t0x%x\n", i,
			in_le32 (&(pcip->pmm[i].pcila)));
		printk (" pmm%dma\t0x%x\n", i,
			in_le32 (&(pcip->pmm[i].pciha)));
	}
	printk (" ptm1ms\t0x%x\n", in_le32 (&(pcip->ptm1ms)));
	printk (" ptm1la\t0x%x\n", in_le32 (&(pcip->ptm1la)));
	printk (" ptm2ms\t0x%x\n", in_le32 (&(pcip->ptm2ms)));
	printk (" ptm2la\t0x%x\n", in_le32 (&(pcip->ptm2la)));

#endif
}
#endif

void __init board_io_mapping (void)
{
	ppc4xx_map_io ();
}

void __init board_setup_irq (void)
{
}

void __init
platform_init (unsigned long r3, unsigned long r4, unsigned long r5,
	       unsigned long r6, unsigned long r7)
{
	ppc4xx_init (r3, r4, r5, r6, r7);

	ppc_md.setup_arch = board_setup_arch;
	ppc_md.setup_io_mappings = board_io_mapping;

#ifdef CONFIG_GEN_RTC
	ppc_md.set_rtc_time = NULL;	/* will be set by driver init code */
	ppc_md.get_rtc_time = NULL;
#endif
#ifdef CONFIG_KGDB
	ppc_md.early_serial_map = ppchameleon_early_serial_map;
#endif
}
