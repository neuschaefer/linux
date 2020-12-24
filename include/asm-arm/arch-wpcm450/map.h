/* linux/include/asm-arm/arch-wpcm450/map.h
 *
 * (c) 2006 Winbond Electronics
 *  shirley yu <clyu2@winbond.com>
 *
 * WPCM450 - Memory map definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Changelog:
 *  22-Aug-2006 clyu   Created file
*/

#ifndef __ASM_ARCH_MAP_H
#define __ASM_ARCH_MAP_H

#include <linux/compiler.h>

/* we have a bit of a tight squeeze to fit all our registers from
 * 0xF00000000 upwards, since we use all of the nGCS space in some
 * capacity, and also need to fit the WPCM450 registers in as well...
 *
 * we try to ensure stuff like the IRQ registers are available for
 * an single MOVS instruction (ie, only 8 bits of set data)
 *
 * Note, we are trying to remove some of these from the implementation
 * as they are only useful to certain drivers...
 */

#ifndef __ASSEMBLY__
#define WPCM450_ADDR(x)	  ((void __iomem *)0xF0000000 + (x))
#else
#define WPCM450_ADDR(x)	  (0xF0000000 + (x))
#endif

/* interrupt controller is the first thing we put in, to make
 * the assembly code for the irq detection easier
 */
#define WPCM450_PA_AIC	   (0xB8002000)
#define WPCM450_VA_AIC	   WPCM450_ADDR(0x00000000)
#define WPCM450_SZ_AIC	   SZ_4K

/* System Global Control Registers (GCR_BA) */
#define WPCM450_PA_GCR	   (0xB0000000)
#define WPCM450_VA_GCR	   WPCM450_ADDR(0x00001000)
#define WPCM450_SZ_GCR	   SZ_4K

/* Clock Control Registers (CLK_BA) */
#define WPCM450_PA_CLK  (0xB0000200)
#define WPCM450_VA_CLK  WPCM450_ADDR(0x00002000)
#define WPCM450_SZ_CLK  SZ_4K

/* Ethernet MAC 1 Control Register (EMC1_BA) */
/* Ethernet MAC 2 Control Register (EMC2_BA) */
/* External MAC control */
#define WPCM450_PA_EMC 	   (0xB0002000)
#define WPCM450_VA_EMC 	   WPCM450_ADDR(0x00003000)
#define WPCM450_SZ_EMC 	   SZ_8K

/* UART Control Register (UART_BA) */
/* UARTs */
#define WPCM450_PA_UART	   (0xB8000000)
#define WPCM450_VA_UART	   WPCM450_ADDR(0x00005000)
#define WPCM450_SZ_UART	   SZ_4K

/* Timer Control Register (TMR_BA) */
#define WPCM450_PA_TMR   (0xB8001000)
#define WPCM450_VA_TMR   WPCM450_ADDR(0x00006000)
#define WPCM450_SZ_TMR   SZ_4K//SZ_1M

/* GPIO Control Register (GPIO_BA) */
/* GPIO ports */
#define WPCM450_PA_GPIO	   (0xB8003000)
#define WPCM450_VA_GPIO	   WPCM450_ADDR(0x00007000)
#define WPCM450_SZ_GPIO	   SZ_4K

/* Memory Controller Control Register (MC_BA) */
#define WPCM450_PA_MC	(0xB0001000)
#define WPCM450_VA_MC	WPCM450_ADDR(0x00008000)
#define WPCM450_SZ_MC	SZ_4K

/* USB Device 2.0 Control Register(USBD2_BA) */
#define WPCM450_PA_USBD2	(0xB0005000)
#define WPCM450_VA_USBD2	WPCM450_ADDR(0x00009000)
#define WPCM450_SZ_USBD2	SZ_4K

/* USB Device 1.0 Control Register(USBD1_BA) */
#define WPCM450_PA_USBD1	(0xB0006000)
#define WPCM450_VA_USBD1	WPCM450_ADDR(0x0000A000)
#define WPCM450_SZ_USBD1	SZ_4K

/* VCD Control Register (VCD_BA) */
#define WPCM450_PA_VCD	(0xB0010000)
#define WPCM450_VA_VCD	WPCM450_ADDR(0x0000B000)
#define WPCM450_SZ_VCD	SZ_4K

/* PECI Interface Register (PECI_BA) */
#define WPCM450_PA_PECI	(0xB8000200)
#define WPCM450_VA_PECI	WPCM450_ADDR(0x0000C000)
#define WPCM450_SZ_PECI	SZ_4K

/* Graphics Core Information R/O Register (GFXI_BA) */
#define WPCM450_PA_GFXI	(0xB8000300)
#define WPCM450_VA_GFXI	WPCM450_ADDR(0x0000D000)
#define WPCM450_SZ_GFXI	SZ_4K

/* TACH Timer 0 Control Register (MFT0_BA) */
#define WPCM450_PA_MFT0	(0xB8004000)
#define WPCM450_VA_MFT0	WPCM450_ADDR(0x0000E000)
#define WPCM450_SZ_MFT0	SZ_4K

/* TACH Timer 0 Control Register (MFT1_BA) */
#define WPCM450_PA_MFT1	(0xB8005000)
#define WPCM450_VA_MFT1	WPCM450_ADDR(0x0000F000)
#define WPCM450_SZ_MFT1	SZ_4K

/* SMBus 0 Control Register (SMB0_BA) */
#define WPCM450_PA_SMB0	(0xB8006000)
#define WPCM450_VA_SMB0	WPCM450_ADDR(0x00011000)
#define WPCM450_SZ_SMB0	SZ_4K

/* PWM Control Register (PWM_BA) */
#define WPCM450_PA_PWM	(0xB8007000)
#define WPCM450_VA_PWM	WPCM450_ADDR(0x00012000)
#define WPCM450_SZ_PWM	SZ_4K

/* KCS Interface Control Register (KCS_BA) */
#define WPCM450_PA_KCS	(0xB8008000)
#define WPCM450_VA_KCS	WPCM450_ADDR(0x00013000)
#define WPCM450_SZ_KCS	SZ_4K

/* ADC Control Register (ADC_BA) */
#define WPCM450_PA_ADC	(0xB8009000)
#define WPCM450_VA_ADC	WPCM450_ADDR(0x00014000)
#define WPCM450_SZ_ADC	SZ_4K

/* AES Control Register (ADC_BA) */
#define WPCM450_PA_AES	(0xB800B000)
#define WPCM450_VA_AES	WPCM450_ADDR(0x00015000)
#define WPCM450_SZ_AES	SZ_4K

/* FIU Register (FIU_BA) */
#define WPCM450_PA_FIU	(0xC8000000)
#define WPCM450_VA_FIU	WPCM450_ADDR(0x00016000)
#define WPCM450_SZ_FIU	SZ_4K

/* SHM Register (SHM_BA) */
#define WPCM450_PA_SHM	(0xC8001000)
#define WPCM450_VA_SHM	WPCM450_ADDR(0x00017000)
#define WPCM450_SZ_SHM	SZ_4K

/* XBus CS0 Register (XBUS0_BA) */
#define WPCM450_PA_XBUS0	(0x44000000)
#define WPCM450_VA_XBUS0	WPCM450_ADDR(0x00018000)
#define WPCM450_SZ_XBUS0	SZ_4K

/* XBus CS1 Register (XBUS1_BA) */
#define WPCM450_PA_XBUS1	(0x44800000)
#define WPCM450_VA_XBUS1	WPCM450_ADDR(0x00019000)
#define WPCM450_SZ_XBUS1	SZ_4K

/* XBus CS2 Register (XBUS2_BA) */
#define WPCM450_PA_XBUS2	(0x45000000)
#define WPCM450_VA_XBUS2	WPCM450_ADDR(0x0001A000)
#define WPCM450_SZ_XBUS2	SZ_4K

#define WPCM450_SDRAM_PA    (0x00000000)

#endif /* __ASM_ARCH_MAP_H */
