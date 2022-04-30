/*
 *
 *  Copyright (C)2008  ST MICROELECTRONICS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * AUTHOR :  Deepak Sikri <deepak.sikri@st.com> */
#include <linux/init.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/sysdev.h>
#include <linux/interrupt.h>

#include <asm/system.h>
#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/leds.h>
#include <asm/mach-types.h>
#include <asm/hardware/icst307.h>

#include <asm/mach/arch.h>
#include <asm/mach/flash.h>
#include <asm/mach/irq.h>
#include <asm/mach/time.h>
#include <asm/mach/map.h>
#include <asm/mach/mmc.h>

#include "core.h"
#include "clock.h"

/*
 * Setup a VA for the spearbasic Vectored Interrupt Controller.
 */
#define VA_VIC1_BASE		 IO_ADDRESS(SPEARBASIC_VIC1_BASE)
//#define VA_VIC2_BASE		 IO_ADDRESS(SPEARBASIC_VIC2_BASE)

volatile unsigned char *TDM_CLEAR_REGISTER;
unsigned int TDM_DEBUG_MSG_LEVEL;
EXPORT_SYMBOL(TDM_CLEAR_REGISTER);
EXPORT_SYMBOL(TDM_DEBUG_MSG_LEVEL);

void vic_mask_irq(unsigned int irq)
{
	if(irq == 28){
		if(TDM_DEBUG_MSG_LEVEL > 0){
			printk("Mask IRQ: %d STATUS: %X\n", irq, *((unsigned int *)(VA_VIC1_BASE + VIC_IRQ_STATUS)) );
			printk("Mask VIC_IRQ_ENABLE: %X\n", *((unsigned int *)(VA_VIC1_BASE + VIC_IRQ_ENABLE)) );
		}
		return;
	}
	if(irq < 32 )
	{
		irq -= IRQ_VIC_START;
		writel(1 << irq, VA_VIC1_BASE + VIC_IRQ_ENABLE_CLEAR);
	}	
}

void vic_unmask_irq(unsigned int irq)
{
	
	if(irq == 28){
		if(TDM_DEBUG_MSG_LEVEL > 0){
			printk("UnMask IRQ: %d STATUS: %X\n", irq, *((unsigned int *)(VA_VIC1_BASE + VIC_IRQ_STATUS)) );
			printk("UnMask VIC_IRQ_ENABLE: %X\n", *((unsigned int *)(VA_VIC1_BASE + VIC_IRQ_ENABLE)) );
		}
	}
	if(irq < 32 )
	{
		irq -= IRQ_VIC_START;
		writel(1 << irq, VA_VIC1_BASE + VIC_IRQ_ENABLE);
	}
	
	if(irq == 28){
		if(TDM_DEBUG_MSG_LEVEL > 0){
			printk("After UnMask IRQ: %d STATUS: %X\n", irq, *((unsigned int *)(VA_VIC1_BASE + VIC_IRQ_STATUS)) );
			printk("After UnMask VIC_IRQ_ENABLE: %X\n", *((unsigned int *)(VA_VIC1_BASE + VIC_IRQ_ENABLE)) );
		}
	}
	
//	else
//	{
//		irq -= IRQ_VIC_START;
//		writel(1 << (irq - 32), VA_VIC2_BASE + VIC_IRQ_ENABLE);
//	}

}

EXPORT_SYMBOL(vic_mask_irq);
EXPORT_SYMBOL(vic_unmask_irq);


static struct irqchip vic_chip = {
	.ack	= vic_mask_irq,
	.mask	= vic_mask_irq,
	.unmask	= vic_unmask_irq,
};

static void do_init_irq(unsigned char vic)
{
	unsigned long VA_VIC_BASE;
	unsigned int irq;
	unsigned int i, value;

//	if(vic == 1 )
		VA_VIC_BASE = VA_VIC1_BASE;
//	else
//		VA_VIC_BASE = VA_VIC2_BASE;

	/* Disable all interrupts initially. */

	writel(0,  VA_VIC_BASE + VIC_INT_SELECT);
	writel(0,  VA_VIC_BASE + VIC_IRQ_ENABLE);
	writel(~0, VA_VIC_BASE + VIC_IRQ_ENABLE_CLEAR);
	writel(0,  VA_VIC_BASE + VIC_IRQ_STATUS);
	writel(0,  VA_VIC_BASE + VIC_ITCR);
	writel(~0, VA_VIC_BASE + VIC_IRQ_SOFT_CLEAR);

	/*
	 * Make sure we clear all existing interrupts
	 */
	writel(0, VA_VIC_BASE + VIC_VECT_ADDR);

	for (i = 0; i < 19; i++) {
		value = readl(VA_VIC_BASE + VIC_VECT_ADDR);
		writel(value, VA_VIC_BASE + VIC_VECT_ADDR);
	}

	for(irq = 0; irq < 16; irq++)
	{
		value = readl(VA_VIC_BASE + VIC_VECT_CNTL0 + (irq * 4) );
		writel(value | VICVectCntl_Enable | irq, VA_VIC_BASE + VIC_VECT_CNTL0 + (irq * 4) );
	}


	writel(32, VA_VIC_BASE + VIC_DEF_VECT_ADDR);

}

void __init spearbasic_init_irq(void)
{
	unsigned int i;

	/* for VIC1 */
	do_init_irq(1);

	/* for VIC2 */
//	do_init_irq(2);

	for (i = IRQ_VIC_START; i <= IRQ_VIC_END; i++) {
		set_irq_chip(i, &vic_chip);
		set_irq_handler(i, do_level_IRQ);
		set_irq_flags(i, IRQF_VALID | IRQF_PROBE);
	}
}
