/*
 * linux/include/asm-arm/arch-puma/memory.h
 *
 * by Alexander Schulz
 *
 * derived from:
 * linux/include/asm-arm/arch-ebsa110/memory.h
 * Copyright (c) 1996-1999 Russell King.
 */

/* Copyright 2008, Texas Instruments Incorporated
 *
 * This program has been modified from its original operation by Texas Instruments
 * to do the following:
 * Explanation of modification.
 *  avalanche architecture changes
 *  
 *
 * THIS MODIFIED SOFTWARE AND DOCUMENTATION ARE PROVIDED
 * "AS IS," AND TEXAS INSTRUMENTS MAKES NO REPRESENTATIONS
 * OR WARRENTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO, WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
 * PARTICULAR PURPOSE OR THAT THE USE OF THE SOFTWARE OR
 * DOCUMENTATION WILL NOT INFRINGE ANY THIRD PARTY PATENTS,
 * COPYRIGHTS, TRADEMARKS OR OTHER RIGHTS.
 * See The GNU General Public License for more details.
 *
 * These changes are covered under version 2 of the GNU General Public License,
 * dated June 1991.
 */

/*
    Includes Intel Corporation's changes/modifications dated: December 20, 2013. 
    Changed/modified portions - Copyright © 2013, Intel Corporation. 
*/



#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

#include <linux/sizes.h>

/*
 * Physical DRAM offset.
 */
#define PLAT_PHYS_OFFSET UL(CONFIG_ARM_AVALANCHE_SDRAM_ADDRESS)
#ifndef CONFIG_ARM_PATCH_PHYS_VIRT
#define TASK_SIZE               (0xf000000)
#define TASK_UNMAPPED_BASE      (0x4000000)
#define PAGE_OFFSET PLAT_PHYS_OFFSET
#endif

#define PHYS_TO_NID(addr)  0
/*
 * Kernel direct-mapped RAM region start
 */
#if 0
	#if defined(CONFIG_MACH_PUMA5_VOLCANO)
	#define PAGE_OFFSET	(0x80000000)
	#else
	#define PAGE_OFFSET	(0xC0000000)
	#endif
#endif

#ifndef __ASSEMBLY__

#if 0
static inline void __arch_adjust_zones(int node, unsigned long *zone_size, unsigned long *zhole_size) 
{
  if (node != 0) return;
  /* Only the first 4 MB (=1024 Pages) are usable for DMA */
  zone_size[1] = zone_size[0] - 1024;
  zone_size[0] = 1024;
  zhole_size[1] = zhole_size[0];
  zhole_size[0] = 0;
}

#define arch_adjust_zones(node, size, holes) \
	__arch_adjust_zones(node, size, holes)

#define ISA_DMA_THRESHOLD	(PHYS_OFFSET + SZ_4M - 1)
#endif

#endif

/*#define __virt_to_bus(x)	__virt_to_phys(x)*/
/*#define __bus_to_virt(x)	__phys_to_virt(x)*/
/*#define __pfn_to_bus            __virt_to_bus*/
/*#define __bus_to_pfn            __bus_to_virt*/


#endif
