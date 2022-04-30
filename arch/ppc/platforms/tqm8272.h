/*
 * TQM8260 board specific definitions
 *
 * Copyright (c) 2001 Wolfgang Denk (wd@denx.de)
 */

#ifndef __TQM8260_PLATFORM
#define __TQM8260_PLATFORM


#include <asm/ppcboot.h>

#define CPM_MAP_ADDR		((uint)0xFFF00000)
#define PHY_INTERRUPT		SIU_INT_IRQ7

/* For our show_cpuinfo hooks. */
#define CPUINFO_VENDOR		"TQ Systems"
#define CPUINFO_MACHINE		"TQM8272 PowerPC Port by DENX Software Engineering, www.denx.de"

/* PPC Sys identification */
#define BOARD_CHIP_NAME "8272"

#define BOOTROM_RESTART_ADDR	((uint)0x40000104)

#define CFG_NAND_BASE		0x50000000

#endif	/* __TQM8260_PLATFORM */
