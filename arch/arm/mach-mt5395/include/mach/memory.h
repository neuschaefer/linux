/*
 *  linux/include/asm-arm/arch-mt5391/memory.h
 *
 * Licensed under the GPL
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html#TOC1
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile:  $
 * $Revision: #1 $
 *
 */
#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

/*
 * Physical DRAM offset.
 */
#define PHYS_OFFSET	(0x00000000)
#define BUS_OFFSET	(0x00000000)

#define PCIE_MMIO_BASE                         (0xF0200000)                 /* MMIO: memory mapped io base address for PCI-express. */
#define PCIE_MMIO_PHYSICAL_BASE     (0xF0200000)                  /* AHB-PCI window base */
#define PCIE_MMIO_LENGTH                    (0x00200000)                 /* AHB-PCI window size = 2Mbytes for MMIO. */
#define PCIE_BRIDGE_LENGTH                 (0x00200000)                 /* 2Mbytes Bridge size for PCI-express. */

#endif
