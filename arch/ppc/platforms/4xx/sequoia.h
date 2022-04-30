/*
 * arch/ppc/platforms/4xx/sequoia.h
 *
 * Sequoia board definitions
 *
 * Wade Farnsworth <wfarnsworth@mvista.com>
 *
 * Copyright 2004 MontaVista Software Inc.
 * Copyright 2006 AMCC
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#ifdef __KERNEL__
#ifndef __ASM_SEQUOIA_H__
#define __ASM_SEQUOIA_H__

#include <platforms/4xx/ppc440epx.h>

/* Default clock rate */
#define SEQUOIA_TMRCLK     50000000
#define SEQUOIA_SYSCLK     33333333

/* Board Control and Status Registers */
#define SEQUOIA_BCSR_BASE_ADDR	0x1c0000000ULL

#ifndef __ASSEMBLY__
struct sequoia_bcsr {
	unsigned char board_id;		/* board revision */
	unsigned char cpld_version;	/* CPLD version */
	unsigned char user_dip_switch;	/* LEDs and user DIP switch */
	unsigned char conf_dip_switch;	/* configuration DIP switch */
	unsigned char tmrclk_control;	/* TMRCLK source and modifier */
	unsigned char pci_and_status;	/* PCI speed 33/66 */
	unsigned char reset_ctrl;	/* reset control */
	unsigned char memory_ctrl;	/* FLASH, EEPROM write protect, STTM addr */
	unsigned char eth_ctrl;		/* Ethernet PHY resets */
	unsigned char usb_ctrl;		/* USB control */
	unsigned char perf_timer0;	/* performance timer */
	unsigned char perf_timer1;
	unsigned char perf_timer2;
	unsigned char perf_timer3;
};
#endif /* __ASSEMBLY__ */

#define SEQUOIA_NAND_FLASH_REG_ADDR	0x1D0000000ULL
#define SEQUOIA_NAND_FLASH_REG_SIZE	0x2000

/*
 * Serial port defines
 */
#define RS_TABLE_SIZE			4
/* UART mappings used before early_serial_setup so should be coherent with U-Boot */
#define UART0_IO_BASE			0xEF600300
#define UART1_IO_BASE			0xEF600400
#define UART2_IO_BASE			0xEF600500
#define UART3_IO_BASE			0xEF600600

#define BASE_BAUD			33177600/3/16

#define STD_UART_OP(num)						\
	{ 0, BASE_BAUD, 0, UART##num##_INT,				\
			(ASYNC_BOOT_AUTOCONF | ASYNC_SKIP_TEST),	\
			iomem_base: (void*)UART##num##_IO_BASE,		\
			io_type: SERIAL_IO_MEM},

#define SERIAL_PORT_DFNS			\
	STD_UART_OP(0)				\
	STD_UART_OP(1)				\
	STD_UART_OP(2)				\
	STD_UART_OP(3)

/* PCI support */
#define SEQUOIA_PCI_CFGREGS_BASE	0x00000001eec00000ULL
#define SEQUOIA_PCI_CFGA_PLB32		0xeec00000
#define SEQUOIA_PCI_CFGD_PLB32		0xeec00004
#define SEQUOIA_PCI_CFGA_OFFSET		0
#define SEQUOIA_PCI_CFGD_OFFSET		0x4

#define SEQUOIA_PCI_IO_BASE		0x00000001e8000000ULL
#define SEQUOIA_PCI_IO_SIZE		0x00010000
#define SEQUOIA_PCI_MEM_OFFSET	  	0x00000000
#define SEQUOIA_PCI_PHY_MEM_BASE	0x000000080000000ULL
/* PLB base address base as seen by the core, implemented on PLB3 */
/* PLB base address as seen by the SOC : 0x000000180000000ULL    */

#define SEQUOIA_PCI_LOWER_IO		0x00000000
#define SEQUOIA_PCI_UPPER_IO		0x0000ffff
#define SEQUOIA_PCI_LOWER_MEM		0x80000000
#define SEQUOIA_PCI_UPPER_MEM		0x8fffffff	/* to be checked with AS & TR should be bfffffff (test-only????????????) */
#define SEQUOIA_PCI_MEM_BASE		0x80000000

#define SEQUOIA_PCIL0_BASE		0x00000001ef400000ULL
#define SEQUOIA_PCIL0_SIZE		0x40

#define SEQUOIA_PCIL0_PMM0LA		0x000
#define SEQUOIA_PCIL0_PMM0MA		0x004
#define SEQUOIA_PCIL0_PMM0PCILA		0x008
#define SEQUOIA_PCIL0_PMM0PCIHA		0x00C
#define SEQUOIA_PCIL0_PMM1LA		0x010
#define SEQUOIA_PCIL0_PMM1MA		0x014
#define SEQUOIA_PCIL0_PMM1PCILA		0x018
#define SEQUOIA_PCIL0_PMM1PCIHA		0x01C
#define SEQUOIA_PCIL0_PMM2LA		0x020
#define SEQUOIA_PCIL0_PMM2MA		0x024
#define SEQUOIA_PCIL0_PMM2PCILA		0x028
#define SEQUOIA_PCIL0_PMM2PCIHA		0x02C
#define SEQUOIA_PCIL0_PTM1MS		0x030
#define SEQUOIA_PCIL0_PTM1LA		0x034
#define SEQUOIA_PCIL0_PTM2MS		0x038
#define SEQUOIA_PCIL0_PTM2LA		0x03C

#endif                          /* __ASM_SEQUOIA_H__ */
#endif                          /* __KERNEL__ */
