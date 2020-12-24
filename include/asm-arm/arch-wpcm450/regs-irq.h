/* linux/include/asm/arch-wpcm450/regs-irq.h
 *
 * Copyright (c) 2006 Winbond Electronics
 * All rights reserved.
 *
 * shirley yu (clyu2@winbond.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *
 *
 * Changelog:
 *  22-Aug-2006 clyu  Created file
 */


#ifndef ___ASM_ARCH_REGS_IRQ_H
#define ___ASM_ARCH_REGS_IRQ_H "$Id$"

/* interrupt controller */

#define WPCM450_IRQREG(x)   ((x) + WPCM450_VA_AIC)

/* ----- Define	the interrupt controller registers ----- */
#define WPCM450_AIC_SCR1	WPCM450_IRQREG(0x004)	/* Source control register 1  */
#define WPCM450_AIC_SCR6	WPCM450_IRQREG(0x018)	/* Source control register 6  */
#define WPCM450_AIC_SCR7	WPCM450_IRQREG(0x01C)	/* Source control register 7  */
#define WPCM450_AIC_SCR8	WPCM450_IRQREG(0x020)	/* Source control register 8  */
#define WPCM450_AIC_SCR12	WPCM450_IRQREG(0x030)	/* Source control register 12 */
#define WPCM450_AIC_SCR13	WPCM450_IRQREG(0x034)	/* Source control register 13 */
#define WPCM450_AIC_SCR14	WPCM450_IRQREG(0x038)	/* Source control register 14 */
#define WPCM450_AIC_SCR17	WPCM450_IRQREG(0x044)	/* Source control register 17 */
#define WPCM450_AIC_SCR18	WPCM450_IRQREG(0x048)	/* Source control register 18 */

#define WPCM450_AIC_GEN		WPCM450_IRQREG(0x084)   
#define WPCM450_AIC_GRSR	WPCM450_IRQREG(0x088)   
#define WPCM450_AIC_IRSR	WPCM450_IRQREG(0x100)   
#define WPCM450_AIC_IASR	WPCM450_IRQREG(0x104)   
                                                    
#define WPCM450_AIC_ISR		WPCM450_IRQREG(0x108)   /* Interrupt status register */
#define WPCM450_AIC_IPER	WPCM450_IRQREG(0x10C)   
#define WPCM450_AIC_ISNR	WPCM450_IRQREG(0x110)   
#define WPCM450_AIC_IMR		WPCM450_IRQREG(0x114)   /* Interrupt mask register */
#define WPCM450_AIC_OISR	WPCM450_IRQREG(0x118)   
                                                    
#define WPCM450_AIC_MECR	WPCM450_IRQREG(0x120)   /* Mask enable command register */
#define WPCM450_AIC_MDCR	WPCM450_IRQREG(0x124)   /* Mask disable command register */
#define WPCM450_AIC_SSCR	WPCM450_IRQREG(0x128)   /* Source set command register */
#define WPCM450_AIC_SCCR	WPCM450_IRQREG(0x12C)   

#define WPCM450_AIC_EOSCR	WPCM450_IRQREG(0x130)   

#endif /* ___ASM_ARCH_REGS_IRQ_H */
