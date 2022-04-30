/*
 * PM82X board specific definitions
 * 
 * Copyright (c) 2005 Heiko Schocher (hs@denx.de)
 */

#ifndef __MACH_PM82X_H
#define __MACH_PM82X_H

#include <asm/ppcboot.h>

#if defined(CONFIG_PCI)
/* #include <platforms/m8260_pci.h> */
#endif

#define CPM_MAP_ADDR		((uint)0xF0000000)
#define	I2C_ADDR_RTC		0x51

/* For our show_cpuinfo hooks. */
#define CPUINFO_VENDOR          "Microsys"
#define CPUINFO_MACHINE         "PM82x PowerPC"

#if defined (CONFIG_MTD_NAND_DISKONCHIP)
#define BOOTROM_RESTART_ADDR    ((uint)0x40000100)
#else
#define BOOTROM_RESTART_ADDR    ((uint)0xFF800100)
#endif

/* PPC Sys identification */
#define BOARD_CHIP_NAME "8250"

#ifdef CONFIG_PCI
/* PCI interrupt controller */
#define PCI_INT_STAT_REG        0xF8200000
#define PCI_INT_MASK_REG        0xF8200004
#define PIRQA                   (NR_CPM_INTS + 0)
#define PIRQB                   (NR_CPM_INTS + 1)
#define PIRQC                   (NR_CPM_INTS + 2)
#define PIRQD                   (NR_CPM_INTS + 3)

#define PCI_INT_TO_SIU  SIU_INT_IRQ2

#endif /* CONFIG_PCI */


#endif	/* __MACH_PM82X_H */
