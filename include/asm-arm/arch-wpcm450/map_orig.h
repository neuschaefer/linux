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

#ifndef __ASSEMBLY__
#define WPCM450_ADDR(x)	  ((void __iomem *)0xF0000000 + (x))
#else
#define WPCM450_ADDR(x)	  (0xF0000000 + (x))
#endif


#define AHB_IO_BASE		0xB0000000
#define APB_IO_BASE		0xB8000000
#define CLOCKPW_BASE	(AHB_IO_BASE+0x200)
#define AIC_IO_BASE		(APB_IO_BASE+0x2000)
#define TIMER_IO_BASE	(APB_IO_BASE+0x1000)

/* interrupt controller is the first thing we put in, to make
 * the assembly code for the irq detection easier
 */
#define WPCM450_VA_IRQ	   WPCM450_ADDR(0x00000000)
#define WPCM450_PA_IRQ	   (0xB8002000)
#define WPCM450_SZ_IRQ	   SZ_4K

/* Clock and Power management */
//#define WPCM450_VA_CLKPWR  WPCM450_ADDR(0x00000200)
//#define WPCM450_PA_CLKPWR  (0xB0000200)
//#define WPCM450_SZ_CLKPWR  SZ_4K

#define WPCM450_VA_GCR	   WPCM450_ADDR(0x00001000)
#define WPCM450_PA_GCR	   (0xB0000000)
#define WPCM450_SZ_GCR	   SZ_4K

/* Clock and Power management */
#define WPCM450_VA_CLKPWR  WPCM450_ADDR(0x00002000)
#define WPCM450_PA_CLKPWR  (0xB0000200)
#define WPCM450_SZ_CLKPWR  SZ_4K

/* External MAC control */
#define WPCM450_VA_EMC 	   WPCM450_ADDR(0x00003000)
#define WPCM450_PA_EMC 	   (0xB0002000)
#define WPCM450_SZ_EMC 	   SZ_8K

/* UARTs */
#define WPCM450_VA_UART	   WPCM450_ADDR(0x00005000)
#define WPCM450_PA_UART	   (0xB8000000)
#define WPCM450_SZ_UART	   SZ_4K

/* Timers */
#define WPCM450_VA_TIMER   WPCM450_ADDR(0x00006000)
#define WPCM450_PA_TIMER   (0xB8001000)
#define WPCM450_SZ_TIMER   SZ_4K//SZ_1M

/* GPIO ports */
#define WPCM450_VA_GPIO	   WPCM450_ADDR(0x00007000)
#define WPCM450_PA_GPIO	   (0xB8003000)
#define WPCM450_SZ_GPIO	   SZ_4K

/* GPIO ports */
#define WPCM450_VA_FIU	   WPCM450_ADDR(0x00008000)
#define WPCM450_PA_FIU	   (0xC8000000)
#define WPCM450_SZ_FIU	   SZ_4K

#define WPCM450_SDRAM_PA    (0x00000000)

#ifdef AVI_REMOVE 
/* EBI management */
#define WPCM450_VA_EBI  WPCM450_ADDR(0x00001000)
#define WPCM450_PA_EBI  (0xB0001000)
#define WPCM450_SZ_EBI  SZ_4K

/* PCI interface controller */
#define WPCM450_VA_PCI 	   WPCM450_ADDR(0x00002000)
#define WPCM450_PA_PCI 	   (0xB0002000)
#define WPCM450_SZ_PCI 	   SZ_4K

/* GDMA control */
#define WPCM450_VA_GDMA 	   WPCM450_ADDR(0x00004000)
#define WPCM450_PA_GDMA 	   (0xB0004000)
#define WPCM450_SZ_GDMA 	   SZ_4K

/* USB host controller */
#define WPCM450_VA_USBOHCIHOST WPCM450_ADDR(0x00005000)
#define WPCM450_PA_USBOHCIHOST (0xB0005000)
#define WPCM450_SZ_USBOHCIHOST SZ_4K

/* USB Device port */
#define WPCM450_VA_USBDEV  WPCM450_ADDR(0x00006000)
#define WPCM450_PA_USBDEV  (0xB0006000)
#define WPCM450_SZ_USBDEV  SZ_4K

/* DMA controller */
#define WPCM450_VA_DMA	   WPCM450_ADDR(0x00007000)
#define WPCM450_PA_DMA	   (0xB0007000)
#define WPCM450_SZ_DMA	   SZ_4K

/* FMI controller */
#define WPCM450_VA_FMI	   WPCM450_ADDR(0x0000d000)//zswan change it 20070301
#define WPCM450_PA_FMI	   (0xB000d000)
#define WPCM450_SZ_FMI	   SZ_4K

/* LCD controller */
#define WPCM450_VA_LCD	   WPCM450_ADDR(0x00008000)
#define WPCM450_PA_LCD	   (0xB0008000)
#define WPCM450_SZ_LCD	   SZ_4K

/* Audio Controller controller */
#define WPCM450_VA_ACTL	   WPCM450_ADDR(0x00009000)
#define WPCM450_PA_ACTL	   (0xB0009000)
#define WPCM450_SZ_ACTL	   SZ_4K

/* ATAPI */
#define WPCM450_VA_ATAPI WPCM450_ADDR(0x0000A000)
#define WPCM450_PA_ATAPI (0xB000A000)
#define WPCM450_SZ_ATAPI SZ_4K

/* 2D */
#define WPCM450_VA_GE WPCM450_ADDR(0x0000B000)
#define WPCM450_PA_GE (0xB000B000)
#define WPCM450_SZ_GE SZ_4K

/* RTC */
#define WPCM450_VA_RTC	   WPCM450_ADDR(0x08004000)
#define WPCM450_PA_RTC	   (0xB8004000)
#define WPCM450_SZ_RTC	   SZ_4K

/* Smart card host interface control register */
#define WPCM450_VA_SC	   WPCM450_ADDR(0x08005000)
#define WPCM450_PA_SC	   (0xB8005000)
#define WPCM450_SZ_SC	   SZ_4K

/* I2C hardware controller */
#define WPCM450_VA_I2C	   WPCM450_ADDR(0x08006000)
#define WPCM450_PA_I2C	   (0xB8006000)
#define WPCM450_SZ_I2C	   SZ_4K

/* Pulse Width Modulation(PWM) Control Registers */
#define WPCM450_VA_PWM	   WPCM450_ADDR(0x08007000)
#define WPCM450_PA_PWM	   (0xB8007000)
#define WPCM450_SZ_PWM	   SZ_4K

/* Keypad Interface Control Registers */
#define WPCM450_VA_KPI	   WPCM450_ADDR(0x08008000)
#define WPCM450_PA_KPI	   (0xB8008000)
#define WPCM450_SZ_KPI	   SZ_4K

/* PS2 Interface Control Registers */
#define WPCM450_VA_PS2	   WPCM450_ADDR(0x08009000)
#define WPCM450_PA_PS2	   (0xB8009000)
#define WPCM450_SZ_PS2	   SZ_4K

/* ADC */
#define WPCM450_VA_ADC	   WPCM450_ADDR(0x0800A000)
#define WPCM450_PA_ADC	   (0xB800A000)
#define WPCM450_SZ_ADC	   SZ_4K

#define VA_IIC_BASE	   (WPCM450_VA_IIC)
#endif // AVI_REMOVE


#endif /* __ASM_ARCH_MAP_H */
