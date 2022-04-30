/*
 * arch/ppc/platforms/p3p440.h
 *
 * P3P440 board definitions
 *
 * Copyright (c) 2005 DENX Software Engineering
 * Stefan Roese <sr@denx.de>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#ifdef __KERNEL__
#ifndef __ASM_P3P440_H__
#define __ASM_P3P440_H__

#include <platforms/4xx/ibm440gp.h>

/*
 * Serial port defines
 */

/* head_44x.S created UART mapping, used before early_serial_setup.
 * We cannot use default OpenBIOS UART mappings because they
 * don't work for configurations with more than 512M RAM.    --ebs
 */
#define UART0_IO_BASE	0xF0000200
#define UART1_IO_BASE	0xF0000300

/* no external clock used, will be filled dynamically */
#define BASE_BAUD	691200

#define STD_UART_OP(num)					\
	{ 0, BASE_BAUD, 0, UART##num##_INT,			\
		(ASYNC_BOOT_AUTOCONF | ASYNC_SKIP_TEST),	\
		iomem_base: (void*)UART##num##_IO_BASE,		\
		io_type: SERIAL_IO_MEM},

#define SERIAL_PORT_DFNS	\
	STD_UART_OP(0)		\
	STD_UART_OP(1)

/* PCI support */
#define P3P440_PCI_LOWER_IO	0x00000000
#define P3P440_PCI_UPPER_IO	0x0000ffff
#define P3P440_PCI_LOWER_MEM	0x80002000
#define P3P440_PCI_UPPER_MEM	0xffffefff

#define P3P440_PCI_CFGREGS_BASE	0x000000020ec00000
#define P3P440_PCI_CFGA_PLB32	0x0ec00000
#define P3P440_PCI_CFGD_PLB32	0x0ec00004

#define P3P440_PCI_IO_BASE	0x0000000208000000ULL
#define P3P440_PCI_IO_SIZE	0x00010000
#define P3P440_PCI_MEM_OFFSET	0x00000000

#define PCI_INTA		23
#define PCI_INTB		24
#define PCI_INTC		25
#define PCI_INTD		26

#define CFG_GPIO_RDY		(0x80000000 >> 11)
#define CFG_MONARCH_IO		(0x80000000 >> 18)
#define CFG_EREADY_IO		(0x80000000 >> 20)
#define CFG_LED_GREEN		(0x80000000 >> 21)
#define CFG_LED_RED		(0x80000000 >> 22)

#endif				/* __ASM_P3P440_H__ */
#endif				/* __KERNEL__ */
