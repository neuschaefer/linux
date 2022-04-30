/*
 * arch/ppc/platforms/4xx/ppc440epx.h
 *
 * PPC440EPX definitions
 *
 * Copyright 2002 Roland Dreier
 * Copyright 2004 MontaVista Software, Inc.
 * Copyright 2006 AMCC.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#ifdef __KERNEL__
#ifndef __PPC_PLATFORMS_PPC440EPX_H
#define __PPC_PLATFORMS_PPC440EPX_H

#include <asm/ibm44x.h>

/* Interrupt Assignements - used in OCP table definition */
#define IIC0_INT			2
#define IIC1_INT			7
#define EMAC0_INT			24
#define EMAC1_INT			25

/* UART */
#define PPC440EPX_UART0_ADDR		0x00000001EF600300ULL
#define PPC440EPX_UART1_ADDR		0x00000001EF600400ULL
#define PPC440EPX_UART2_ADDR		0x00000001EF600500ULL
#define PPC440EPX_UART3_ADDR		0x00000001EF600600ULL
#define UART0_INT			0
#define UART1_INT			1
#define UART2_INT			35
#define UART3_INT			36

/* IIC Bootstrap Registers */
#define SDR0_CFGADDR		0x00E		/* System DCR Address Register */
#define SDR0_CFGDATA		0x00F		/* System DCR Data Register */
#define SDR0_SDSTP1		0x021		/* Serial Device Strap Register 1 */

/* Clock and Power Management CPM0 */
#define IBM_CPM_IIC0		0x80000000	/* IIC interface */
#define IBM_CPM_IIC1		0x40000000	/* IIC interface */
#define IBM_CPM_PCI		0x20000000	/* PCI bridge */
#define IBM_CPM_USB1H		0x08000000	/* USB 1.1 Host */
#define IBM_CPM_FPU		0x04000000	/* floating point unit */
#define IBM_CPM_CPU		0x02000000	/* processor core */
#define IBM_CPM_DMA		0x01000000	/* DMA controller */
#define IBM_CPM_BGO		0x00800000	/* PLB3 to OPB bus arbiter */
#define IBM_CPM_EBC		0x00200000	/* External Bus Controller */
#define IBM_CPM_RGMII		0x00100000	/* Reduced Gigabit MII Bridge */
#define IBM_CPM_DMC		0x00080000	/* SDRAM peripheral controller */
#define IBM_CPM_PLB4		0x00040000	/* PLB4 bus arbiter */
#define IBM_CPM_PLB4x3		0x00020000	/* PLB4 to PLB3 bridge controller */
#define IBM_CPM_PLB3x4		0x00010000	/* PLB3 to PLB4 bridge controller */
#define IBM_CPM_PLB3		0x00008000	/* PLB3 bus arbiter */
#define IBM_CPM_NDFC 		0x00004000      /* NAND Flash Controller */
#define IBM_CPM_UIC1		0x00001000	/* Universal Interrupt Controller */
#define IBM_CPM_GPIO0		0x00000800	/* General Purpose IO (??) */
#define IBM_CPM_GPT		0x00000400	/* General Purpose Timers  */
#define IBM_CPM_UART0		0x00000200	/* serial port 0 */
#define IBM_CPM_UART1		0x00000100	/* serial port 1 */
#define IBM_CPM_UIC0		0x00000080	/* Universal Interrupt Controller */
#define IBM_CPM_TMRCLK		0x00000040	/* CPU timers */
#define IBM_CPM_EMAC0		0x00000020	/* ethernet port 0 */
#define IBM_CPM_UART2		0x00000010	/* serial port 2 */
#define IBM_CPM_UART3		0x00000008	/* serial port 3 */
#define IBM_CPM_EMAC1		0x00000004	/* ethernet port 1 */
#define IBM_CPM_P42OPB1		0x00000002	/* USB 2.0 Host*/
#define IBM_CPM_OPB2P4		0x00000001	/* USB 2.0 Host */

/* Clock and Power Management CPM1*/
#define IBM_CPM_UIC2		0x80000000	/* Universal Interrupt Controller 2 */
#define IBM_CPM_SRAM0		0x40000000	/* Internal SRAM Controller */
#define IBM_CPM_MAL0		0x20000000	/* Memory Access Layer */
#define IBM_CPM_USB2D0		0x10000000	/* USB2.0 Device */
#define IBM_CPM_USB2H		0x08000000	/* USB 2.0 HOST */
#define IBM_CPM_CRYP0		0x04000000	/* Security Engine */
#define IBM_CPM_KASU0		0x02000000	/* Kasumi Engine */

#endif /* __PPC_PLATFORMS_PPC440EPX_H */
#endif /* __KERNEL__ */
