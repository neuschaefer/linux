/*
 *  linux/include/asm-arm/arch-mt5391/vmalloc.h
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

/*
 * Just any arbitrary offset to the start of the vmalloc VM area: the
 * current 8MB value just means that there will be a 8MB "hole" after the
 * physical memory until the kernel virtual memory starts.  That means that
 * any out-of-bounds memory accesses will hopefully be caught.
 * The vmalloc() routines leaves a hole of 4kB between each vmalloced
 * area for the same reason. ;)
 */

#define VMALLOC_OFFSET	  (8*1024*1024)
#define VMALLOC_START	  (((unsigned long)high_memory + VMALLOC_OFFSET) & ~(VMALLOC_OFFSET-1))
#ifdef CONFIG_FAST_COLD_REBOOT
#ifdef CONFIG_PHYSICAL_DRAM_SIZE_256M
#undef VMALLOC_START
#define VMALLOC_START	  ((unsigned long) (PAGE_OFFSET + 0x10000000 + VMALLOC_OFFSET))
#endif
#endif // CONFIG_FAST_COLD_REBOOT

#define VMALLOC_END       ((unsigned long)(0xF0000000 - VMALLOC_OFFSET))


