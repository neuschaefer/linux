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
 
#define IO_BASE 0x20000000

#ifndef __ASSEMBLY__
#define WPCM450_ADDR(x)	  ((void __iomem *)IO_BASE + (x))
#else
#define WPCM450_ADDR(x)	  (IO_BASE + (x))
#endif

/* System Global Control Registers (GCR_BA) */
#define WPCM450_PA_GCR	(0xB0000000)
#define WPCM450_VA_GCR	WPCM450_ADDR(WPCM450_PA_GCR)
#define WPCM450_SZ_GCR	SZ_512B

/* Clock Control Registers (CLK_BA) */
#define WPCM450_PA_CLKPWR	(0xB0000200)
#define WPCM450_VA_CLKPWR	WPCM450_ADDR(WPCM450_PA_CLKPWR)
#define WPCM450_SZ_CLKPWR	SZ_256B

/* Clock Control Registers (CLK_BA) */
#define WPCM450_PA_CLK	(0xB0000200)
#define WPCM450_VA_CLK	WPCM450_ADDR(WPCM450_PA_CLK)
#define WPCM450_SZ_CLK	SZ_256B

/* Memory Controller Control Register (MC_BA) */
#define WPCM450_PA_MC	(0xB0001000)
#define WPCM450_VA_MC	WPCM450_ADDR(WPCM450_PA_MC)
#define WPCM450_SZ_MC	SZ_4K

/* External MAC control */
#define WPCM450_PA_EMC	(0xB0002000)
#define WPCM450_VA_EMC	WPCM450_ADDR(WPCM450_PA_EMC)
#define WPCM450_SZ_EMC	SZ_4K

/* Ethernet MAC 1 Control Register (EMC1_BA) */
#define WPCM450_PA_EMC1	(0xB0002000)
#define WPCM450_VA_EMC1	WPCM450_ADDR(WPCM450_PA_EMC1)
#define WPCM450_SZ_EMC1	SZ_4K

/* Ethernet MAC 2 Control Register (EMC2_BA) */
#define WPCM450_PA_EMC2	(0xB0003000)
#define WPCM450_VA_EMC2	WPCM450_ADDR(WPCM450_PA_EMC2)
#define WPCM450_SZ_EMC2	SZ_4K

/* USB Device 2.0 Control Register(USBD2_BA) */
#define WPCM450_PA_USBD2	(0xB0005000)
#define WPCM450_VA_USBD2	WPCM450_ADDR(WPCM450_PA_USBD2)
#define WPCM450_SZ_USBD2	SZ_4K

/* USB Device 1.0 Control Register(USBD1_BA) */
#define WPCM450_PA_USBD1	(0xB0006000)
#define WPCM450_VA_USBD1	WPCM450_ADDR(WPCM450_PA_USBD1)
#define WPCM450_SZ_USBD1	SZ_4K

/* VCD Control Register (VCD_BA) */
#define WPCM450_PA_VCD	(0xB0010000)
#define WPCM450_VA_VCD	WPCM450_ADDR(WPCM450_PA_VCD)
#define WPCM450_SZ_VCD	SZ_64K

/* UART Control Register (UART_BA) */
#define WPCM450_PA_UART	(0xB8000000)
#define WPCM450_VA_UART	WPCM450_ADDR(WPCM450_PA_UART)
#define WPCM450_SZ_UART	SZ_256B

/* UART 0 Control Register (UART0_BA) */
#define WPCM450_PA_UART0	(0xB8000000)
#define WPCM450_VA_UART0	WPCM450_ADDR(WPCM450_PA_UART0)
#define WPCM450_SZ_UART0	SZ_256B

/* UART 1 Control Register (UART1_BA) */
#define WPCM450_PA_UART1	(0xB8000100)
#define WPCM450_VA_UART1	WPCM450_ADDR(WPCM450_PA_UART1)
#define WPCM450_SZ_UART1	SZ_256B

/* PECI Interface Register (PECI_BA) */
#define WPCM450_PA_PECI	(0xB8000200)
#define WPCM450_VA_PECI	WPCM450_ADDR(WPCM450_PA_PECI)
#define WPCM450_SZ_PECI	SZ_256B

/* Graphics Core Information R/O Register (GFXI_BA) */
#define WPCM450_PA_GFXI	(0xB8000300)
#define WPCM450_VA_GFXI	WPCM450_ADDR(WPCM450_PA_GFXI)
#define WPCM450_SZ_GFXI	SZ_256B

/* Timer Control Register (TMR_BA) */
#define WPCM450_PA_TIMER	(0xB8001000)
#define WPCM450_VA_TIMER	WPCM450_ADDR(WPCM450_PA_TIMER)
#define WPCM450_SZ_TIMER	SZ_4K

/* Timer Control Register (TMR_BA) */
#define WPCM450_PA_TMR	(0xB8001000)
#define WPCM450_VA_TMR	WPCM450_ADDR(WPCM450_PA_TMR)
#define WPCM450_SZ_TMR	SZ_4K

/* Interrupt Control Register (AIC_BA) */
#define WPCM450_PA_IRQ	(0xB8002000)
#define WPCM450_VA_IRQ	(0xF8000000)
//#define WPCM450_VA_IRQ	WPCM450_ADDR(WPCM450_PA_IRQ)
#define WPCM450_SZ_IRQ	SZ_4K

/* Interrupt Control Register (AIC_BA) */
#define WPCM450_PA_AIC	(0xB8002000)
#define WPCM450_VA_AIC	WPCM450_ADDR(WPCM450_PA_AIC)
#define WPCM450_SZ_AIC	SZ_4K

/* GPIO Control Register (GPIO_BA) */
#define WPCM450_PA_GPIO	(0xB8003000)
#define WPCM450_VA_GPIO	WPCM450_ADDR(WPCM450_PA_GPIO)
#define WPCM450_SZ_GPIO	SZ_4K

/* TACH Timer 0 Control Register (MFT0_BA) */
#define WPCM450_PA_MFT0	(0xB8004000)
#define WPCM450_VA_MFT0	WPCM450_ADDR(WPCM450_PA_MFT0)
#define WPCM450_SZ_MFT0	SZ_4K

/* TACH Timer 0 Control Register (MFT1_BA) */
#define WPCM450_PA_MFT1	(0xB8005000)
#define WPCM450_VA_MFT1	WPCM450_ADDR(WPCM450_PA_MFT1)
#define WPCM450_SZ_MFT1	SZ_4K

/* SMBus 0 Control Register (SMB0_BA) */
#define WPCM450_PA_SMB0	(0xB8006000)
#define WPCM450_VA_SMB0	WPCM450_ADDR(WPCM450_PA_SMB0)
#define WPCM450_SZ_SMB0	SZ_256B

/* SMBus 1 Control Register (SMB1_BA) */
#define WPCM450_PA_SMB1	(0xB8006100)
#define WPCM450_VA_SMB1	WPCM450_ADDR(WPCM450_PA_SMB1)
#define WPCM450_SZ_SMB1	SZ_256B

/* SMBus 2 Control Register (SMB2_BA) */
#define WPCM450_PA_SMB2	(0xB8006200)
#define WPCM450_VA_SMB2	WPCM450_ADDR(WPCM450_PA_SMB2)
#define WPCM450_SZ_SMB2	SZ_256B

/* SMBus 3 Control Register (SMB3_BA) */
#define WPCM450_PA_SMB3	(0xB80060300)
#define WPCM450_VA_SMB3	WPCM450_ADDR(WPCM450_PA_SMB3)
#define WPCM450_SZ_SMB3	SZ_256B

/* SMBus 4 Control Register (SMB4_BA) */
#define WPCM450_PA_SMB4	(0xB8006400)
#define WPCM450_VA_SMB4	WPCM450_ADDR(WPCM450_PA_SMB4)
#define WPCM450_SZ_SMB4	SZ_256B

/* SMBus 5 Control Register (SMB5_BA) */
#define WPCM450_PA_SMB5	(0xB8006500)
#define WPCM450_VA_SMB5	WPCM450_ADDR(WPCM450_PA_SMB5)
#define WPCM450_SZ_SMB5	SZ_256B

/* PWM Control Register (PWM_BA) */
#define WPCM450_PA_PWM	(0xB8007000)
#define WPCM450_VA_PWM	WPCM450_ADDR(WPCM450_PA_PWM)
#define WPCM450_SZ_PWM	SZ_4K

/* KCS Interface Control Register (KCS_BA) */
#define WPCM450_PA_KCS	(0xB8008000)
#define WPCM450_VA_KCS	WPCM450_ADDR(WPCM450_PA_KCS)
#define WPCM450_SZ_KCS	SZ_4K

/* ADC Control Register (ADC_BA) */
#define WPCM450_PA_ADC	(0xB8009000)
#define WPCM450_VA_ADC	WPCM450_ADDR(WPCM450_PA_ADC)
#define WPCM450_SZ_ADC	SZ_4K

/* FIU Register (FIU_BA) */
#define WPCM450_PA_FIU	(0xC8000000)
#define WPCM450_VA_FIU	WPCM450_ADDR(WPCM450_PA_FIU)
#define WPCM450_SZ_FIU	SZ_4K

/* SHM Register (SHM_BA) */
#define WPCM450_PA_SHM	(0xC8001000)
#define WPCM450_VA_SHM	WPCM450_ADDR(WPCM450_PA_SHM)
#define WPCM450_SZ_SHM	SZ_4K

#define WPCM450_SDRAM_PA    (0x00000000)

#endif /* __ASM_ARCH_MAP_H */
