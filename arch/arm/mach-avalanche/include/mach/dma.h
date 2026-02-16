/*
 * linux/include/asm-arm/arch-shark/dma.h
 *
 * by Alexander Schulz
 */

/* Copyright 2008, Texas Instruments Incorporated
 *
 * This program has been modified from its original operation by Texas Instruments
 * to do the following:
 * Explanation of modification.
 *  No change, keeping it for future avalanche SOC use.
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

#ifndef __ASM_ARCH_DMA_H
#define __ASM_ARCH_DMA_H

/* Use only the lowest 4MB, nothing else works.
 * The rest is not DMAable. See dev /  .properties
 * in OpenFirmware.
 */
#define MAX_DMA_ADDRESS		0xC0400000
#define MAX_DMA_CHANNELS	8
#define DMA_ISA_CASCADE         4

#endif /* _ASM_ARCH_DMA_H */

