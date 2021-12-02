/*
 *  linux/include/asm-arm/arch-mt5396/memory.h
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

#ifdef CC_MT53XX_SUPPORT_2G_DRAM
#define MAX_PHYSMEM_BITS    31
#else
#define MAX_PHYSMEM_BITS    30
#endif
#define SECTION_SIZE_BITS   27
/*#define NODE_MEM_SIZE_BITS  29*/

#if !defined(__ASSEMBLY__) && defined(CONFIG_ZONE_DMA)
extern void mt53xx_adjust_zones(unsigned long *size,
				  unsigned long *hole);
#define arch_adjust_zones(size, hole) \
	mt53xx_adjust_zones(size, hole)

#define ISA_DMA_THRESHOLD       (PHYS_OFFSET + SZ_512M - 1)
#define MAX_DMA_ADDRESS         (PAGE_OFFSET + SZ_512M)
#endif
#define ARCH_VECTOR_PAGE_PHYS    0xFB004000
#define DMX_SRAM_VIRT            0xF3000000            // Will mapping DMX SRAM (0xFB00500) to here

#endif
