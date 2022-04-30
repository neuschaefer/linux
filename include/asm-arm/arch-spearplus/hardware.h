/*
 *  linux/include/asm/arch/hardware.h
 *
 *  This file contains the hardware definitions of the spearplus boards.
 *
 *  Copyright(C)2006 ST MICROELECTRONICS
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __ASM_ARCH_HARDWARE_H
#define __ASM_ARCH_HARDWARE_H

#include <asm/sizes.h>
#include <asm/arch/platform.h>


//Clock Frequency of APB in SPEArPlus
#define SPEAR_APB_CLK		83250000

/*
 * Similar to above, but for PCI addresses (memory, IO, Config and the
 * V3 chip itself).  WARNING: this has to mirror definitions in platform.h
 */


#define SPEARPLUS_ICM1_ICM2_BASE				0xD0000000
#define SPEARPLUS_ICM1_ICM2_SIZE				0x10000000

#define SPEARPLUS_ICM4_BASE					0xE0000000
#define SPEARPLUS_ICM4_SIZE					0x08000000

#define SPEARPLUS_ICM9_BASE					0xC0000000
#define SPEARPLUS_ICM9_SIZE					0x10000000

#define SPEARPLUS_ML1_2_ICM3_BASE				0xF0000000
//#define SPEARPLUS_ML1_2_ICM3_SIZE				0x08000000
#define SPEARPLUS_ML1_2_ICM3_SIZE				0x0f000000

#define IO_ADDRESS(x)						(x)

// Ashwini - Adding IPC Shared Memory
#define IPC_MEMORY_BASE                             0xC1F00000
#define IPC_MEMORY_ADDRESS(x)                       (x | IPC_MEMORY_BASE)

#define IPC_MEMORY_SIZE                             0x100000
#define IPC_MEMORY_START                            0x1F00000


#endif
