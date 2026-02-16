/*
 * linux/include/asm-arm/arch-avalanche/vmalloc.h
 */

/* Copyright 2008, Texas Instruments Incorporated
 *
 * This program has been modified from its original operation by Texas Instruments
 * to do the following:
 * Explanation of modification.
 * avalanche changes  
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

#include <mach/hardware.h>

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
#define VMALLOC_END       (DEVICE_FREE_START_1)
