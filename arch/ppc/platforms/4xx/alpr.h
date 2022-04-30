/*
 * ALPR board definitions
 *
 * Copyright (c) 2006 DENX Software Engineering
 * Heiko Schocher <hs@denx.de>
 * Stefan Roese <sr@denx.de>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#ifdef __KERNEL__
#ifndef __ASM_ALPR_H__
#define __ASM_ALPR_H__

#include <platforms/4xx/ibm440gx.h>

#ifndef __ASSEMBLY__
enum ppc_sys_devices {
        FPGA_UART0,
        FPGA_UART1,
        FPGA_UART2,
/*
        PD_NDFC,
        PD_NDFC_CHIP0,
        PD_NDFC_CHIP1,
        PD_NDFC_CHIP2,
        PD_NDFC_CHIP3,
*/
        NUM_PPC_SYS_DEVS,
}; // test-only

int alpr_map_ibuf_fpga(void __iomem **vbar0, void __iomem **vbar1, void __iomem **vbar2,
		       phys_addr_t *pbar0, phys_addr_t *pbar1, phys_addr_t *pbar2,
		       int *irq);
int alpr_unmap_ibuf_fpga(void __iomem *vbar0, void __iomem *vbar1, void __iomem *vbar2);
#endif

/* F/W TLB mapping used in bootloader glue to reset EMAC */
#define PPC44x_EMAC0_MR0	0xe0000800

/* Flash */
#define ALPR_FLASH_ADDR		0x00000001ffe00000ULL

/* FPGA_REG_3 (Ethernet Groups) */
#define OCOTEA_FPGA_REG_3	0x0000000148300003ULL

/* NAND */
#define	ALPR_NAND_FLASH_REG_ADDR 0x00000001f0000000ULL

/*
 * Serial port defines
 */
#define RS_TABLE_SIZE	2

/* head_44x.S created UART mapping, used before early_serial_setup.
 * We cannot use default OpenBIOS UART mappings because they
 * don't work for configurations with more than 512M RAM.    --ebs
 */
#define UART0_IO_BASE	0xF0000200
#define UART1_IO_BASE	0xF0000300

#define BASE_BAUD	11059200/16
#define STD_UART_OP(num)					\
	{ 0, BASE_BAUD, 0, UART##num##_INT,			\
		(ASYNC_BOOT_AUTOCONF | ASYNC_SKIP_TEST),	\
		iomem_base: (void*)UART##num##_IO_BASE,		\
		io_type: SERIAL_IO_MEM},

#define SERIAL_PORT_DFNS	\
	STD_UART_OP(0)		\
	STD_UART_OP(1)

/* PCI support */
#define ALPR_PCI_LOWER_IO	0x00000000
#define ALPR_PCI_UPPER_IO	0x0000ffff
#define ALPR_PCI_LOWER_MEM	0x80000000
#define ALPR_PCI_UPPER_MEM	0xffffefff

#define ALPR_PCI_CFGREGS_BASE	0x000000020ec00000ULL
#define ALPR_PCI_CFGA_PLB32	0x0ec00000
#define ALPR_PCI_CFGD_PLB32	0x0ec00004

#define ALPR_PCI_IO_BASE	0x0000000208000000ULL
#define ALPR_PCI_IO_SIZE	0x00010000
#define ALPR_PCI_MEM_OFFSET	0x00000000

/* Offsets in IBUF FPGA BAR2 */
#define IBUF_UART0_CTRL		0x0200
#define IBUF_UART0_DATA		0x0400
#define IBUF_UART0_EOI		0x07f8

#define IBUF_UART1_CTRL		0x0a00
#define IBUF_UART1_DATA		0x0c00
#define IBUF_UART1_EOI		0x0ff8

#define IBUF_UART2_CTRL		0x1200
#define IBUF_UART2_DATA		0x1400
#define IBUF_UART2_EOI		0x17f8

#define IBUF_INT_FLAG		0x2000
#define IBUF_INT_ENABLE		0x2008
#define IBUF_INT_STATUS		0x2010

#define IBUF_TIMER_US		0x2100
#define IBUF_TIMER_S		0x2108

#define IBUF_MISC		0x2300

#define IBUF_TS_FIFO		0x2400
#define IBUF_TS_FIFO_COUNT	0x2408

#define IBUF_READ_FIFO_ADDR	0x2500
#define IBUF_WRITE_FIFO_ADDR	0x2508
#define IBUF_SEG_PREFILTER_ADDR	0x2510

#define IBUF_INTENSITY_SUM_ADDR	0x2600
#define IBUF_INTENSITY_SUM_SIZE	0x2608

#define IBUF_IMAGE_LOCATIONS	0x3000
#define IBUF_PREFIL_LOCATIONS	0x3398
#define IBUF_IMAGE_COUNT	0x33f8

#define IBUF_ALT_ASMI_ADDR	0x3800
#define IBUF_ALT_ASMI_READ	0x3808
#define IBUF_ALT_ASMI_WRITE	0x3810

#define IBUF_IMG_WRITE_PTR	0x3c00
#define IBUF_IMG_READ_PTR	0x3c08
#define IBUF_IMG_OVERRUN	0x3c10

/* Misc definitions */
#define MISC_FLASH_SEL_A	0x00000001
#define MISC_FLASH_SEL_B	0x00000002
#define MISC_FLASH_SEL_BOTH	0x00000003
#define MISC_FLASH_SEL_MASK	0x00000003
#define MISC_TRIG_EN		0x00000004
#define MISC_MU_TRIG_STAT	0x00000008
#define MISC_MU_WIN_STAT	0x00000010
#define MISC_MONO_EN		0x00000020
#define MISC_SW_TRIG		0x00000040
#define MISC_RESET_FIFO		0x00000080
#define MISC_IBUF_EN		0x00000100

/* Interrupt definitions */
#define INT_UART0_TX		0x00000001
#define INT_UART0_RX		0x00000002
#define INT_UART0_MASK		0x00000003
#define INT_UART1_TX		0x00000004
#define INT_UART1_RX		0x00000008
#define INT_UART1_MASK		0x0000000c
#define INT_UART2_TX		0x00000010
#define INT_UART2_RX		0x00000020
#define INT_UART2_MASK		0x00000030
#define INT_TS			0x00000040
#define INT_TS_OVERRUN		0x00000080
#define INT_TS_MASK		0x000000c0
#define INT_IMAGE_RAW		0x00000100
#define INT_IMAGE_RESIZE	0x00000200
#define INT_IMAGE_OVERRUN	0x00004000
#define INT_IMAGE_MASK		0x00004300
#define INT_GLOBAL		0x80000000

#endif				/* __ASM_ALPR_H__ */
#endif				/* __KERNEL__ */
