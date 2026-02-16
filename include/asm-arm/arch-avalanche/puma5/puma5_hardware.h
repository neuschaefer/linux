/* Copyright 2008, Texas Instruments Incorporated
 *
 * This program has been modified from its original operation by 
 * Texas Instruments to do the following:
 * Explanation of modification.
 * derived from:
 * linux/include/asm-arm/arch-ebsa110/hardware.h
 *  
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
Includes Intel Corporation's changes/modifications dated: 2014.
Changed/modified portions - Copyright © 2014, Intel Corporation.
*/


#ifndef __PUMA5_HARDWARE_H
#define __PUMA5_HARDWARE_H

#include <asm/arch/memory.h>
/*
 * Mapping areas
 */

#define DEVICE_REGION_1_SIZE	(0x10000000)

#define DEVICE_FREE_START_1	(0xd0000000)
#define DEVICE_FREE_START_2	(DEVICE_FREE_START_1 + DEVICE_REGION_1_SIZE)

#define PCIO_BASE		(0)
#define IO_RELOC_ADDR	(DEVICE_FREE_START_1)	
#define IO_OFFSET		(DEVICE_FREE_START_1) /* Virtual I/O - Phys I/O */

/* BootCFG, UART, WDT, etc., Region */
#define IO_PHY        	(0x0)
#define IO_START		(IO_PHY)
#define IO_SIZE         (DEVICE_REGION_1_SIZE)
#define IO_VIRT         (DEVICE_FREE_START_1 + IO_PHY)
#define IO_BASE			(IO_VIRT)
#define IO_END			(IO_VIRT + IO_SIZE - 1)


#if 0 /* SR, Vlynq and other devices fall under IO_START (given above). Information provided below (inside #if 0) is for understanding pupose */
/* Session Router (SR) Region */
#define SR_PHY			(0x03000000)
#define SR_SIZE			(0x00500000)
#define SR_VIRT			(DEVICE_FREE_REGION_START + SR_PHY)
#define SR_END			(SR_VIRT + SR_SIZE - 1)

/* BootCFG, UART, WDT, etc., Region */
#define IO_PHY        	(0x08600000)
#define IO_START		(IO_PHY)
#define IO_SIZE         (0x00100000)
#define IO_VIRT         (SR_END + 1)
#define IO_BASE			(IO_VIRT)
#define IO_END			(IO_VIRT + IO_SIZE - 1)

/* Vlynq Region */
#define VLYNQ_PHY		(0x0c000000)
#define VLYNQ_SIZE		(0x04000000)
#define VLYNQ_VIRT		(IO_END + 1)
#define VLYNQ_END		(VLYNQ_VIRT + VLYNQ_SIZE - 1)
#endif

/* EMIF 3e Configuratio Region */
#define EMIF3E_PHY		(0x20000000)
#define EMIF3E_SIZE		(0x01000000)
#define EMIF3E_VIRT		(IO_END + 1)
#define EMIF3E_END		(EMIF3E_VIRT + EMIF3E_SIZE - 1)

/* Async EMIF 0 (Parallel Flash 0 ) Region */
#define FLASH_0_PHY		(0x38000000)
#define FLASH_0_SIZE	(0x01000000)
#define FLASH_0_VIRT	(EMIF3E_END + 1)
#define FLASH_0_END		(FLASH_0_VIRT + FLASH_0_SIZE - 1)

/* Async EMIF 1 (Parallel Flash 1 ) Region */
#define FLASH_1_PHY		(0x39000000)
#define FLASH_1_SIZE	(0x01000000)
#define FLASH_1_VIRT	(FLASH_0_END + 1)
#define FLASH_1_END		(FLASH_1_VIRT + FLASH_1_SIZE - 1)

/* Memory Mapped serial flash 0 Region */
#define MM_SPI_0_PHY	(0x48000000)
#define MM_SPI_0_SIZE	(0x01000000)
#define MM_SPI_0_VIRT	(FLASH_1_END + 1)
#define MM_SPI_0_END	(MM_SPI_0_VIRT + MM_SPI_0_SIZE - 1)

/* Memory Mapped serial flash 1 Region */
#define MM_SPI_1_PHY	(0x4c000000)
#define MM_SPI_1_SIZE	(0x01000000)
#define MM_SPI_1_VIRT	(MM_SPI_0_END + 1)
#define MM_SPI_1_END	(MM_SPI_1_VIRT + MM_SPI_1_SIZE - 1)

/* Interrupt controller Region */
#define INTC_PHY		(0x50000000)
#define INTC_SIZE		(0x00100000)
#define INTC_VIRT		(MM_SPI_1_END + 1)
#define INTC_END		(INTC_VIRT + INTC_SIZE - 1)

#define NON_OS_RESERVED_PHY		(0)
#define NON_OS_RESERVED_SIZE	(0x01000000)
#define NON_OS_RESERVED_VIRT	(INTC_END + 1)
#define NON_OS_RESERVED_END		(NON_OS_RESERVED_VIRT + NON_OS_RESERVED_SIZE - 1)


#if defined(CONFIG_MACH_PUMA5_VOLCANO)
/* Volcano information Region */
#define VOLCANO_PHY		(0xFFF00000)
#define VOLCANO_SIZE	(0x00100000)
#define VOLCANO_VIRT	(INTC_END + 1)
#define VOLCANO_END		(VOLCANO_VIRT + VOLCANO_SIZE - 1)
#endif

/* TODO : NEED to take care of Region 2 */
#define IO_ADDRESS_ASM(pa)  ((pa) + IO_OFFSET)
#define IO_ADDRESS(pa)  	(((unsigned int)(pa) < DEVICE_REGION_1_SIZE) ? ((unsigned int)(pa) + IO_OFFSET) : 0)
#define IO_PHY2VIRT(pa)   	(IO_ADDRESS((unsigned int)(pa)))
#define IO_VIRT2PHY_ASM(va) ((va) - IO_OFFSET)
#define IO_VIRT2PHY(va) 	(((unsigned int)(va) < DEVICE_FREE_START_2) ? ((unsigned int)(va) - IO_OFFSET) : 0)

/* DDR CR for EMIF DDR ASYNC mode */
#define REG_DDR_CR              (0x08611B28)
#define EMIF_DDR_DUAL_MODE      (1 << 1)
#define IS_DDR_ASYNCH           ((*((volatile UINT32* )IO_PHY2VIRT(REG_DDR_CR))) & EMIF_DDR_DUAL_MODE)

/* defined here since required for low-level debug functions (asm code) */
/* UART Base address for Puma5 */
#define AVALANCHE_UART0_REGS_BASE        (IO_ADDRESS_ASM(0x08610E00))
#define AVALANCHE_UART1_REGS_BASE        (IO_ADDRESS_ASM(0x08610F00))

#endif /* __PUMA5_HARDWARE_H */

