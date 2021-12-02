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
#define VMALLOC_START	  ((((unsigned long)high_memory < (PAGE_OFFSET + 512*1024*1024) ? (PAGE_OFFSET + 512*1024*1024) : (unsigned long)high_memory) + VMALLOC_OFFSET) & ~(VMALLOC_OFFSET-1))

#define VMALLOC_END       ((unsigned long)(0xF0000000 - VMALLOC_OFFSET))

/* Extra area that could be used by drivers. */
#define EXTRA_VMALLOC_START 0xF4800000
#define EXTRA_VMALLC_END    ((unsigned long)(0xFF000000 - VMALLOC_OFFSET))
