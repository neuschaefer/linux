/*
 * include/asm-arm/arch-mt5391/entry-macro.S
 *
 * Licensed under the GPL
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html#TOC1
 *
 * $Author:  $
 * $Date:  $
 * $RCSfile:  $
 * $Revision:  $
 *
 * Low-level IRQ helper macros for MT5391 platforms
 *
 */
#include <mach/hardware.h>
#include <mach/irqs.h>

/* leave disable_fiq as an empty macro */
    .macro disable_fiq
    .endm
    .macro  get_irqnr_preamble, base, tmp
    .endm

    .macro  arch_ret_to_user, tmp1, tmp2
    .endm

/* 
 * returns highest prio irq in irqnr,
 * all other regs are used as tmp regs
 */
    .macro get_irqnr_and_base, irqnr, irqstat, base, tmp

    /* set bim base register */
    ldr \base, =BIM_VIRT
    /* load irq enable bit mask */
    ldr \tmp, [\base, #REG_RW_MINTEN]
    /* load current irq bits */
    ldr \irqstat, [\base, #REG_RW_MINTST]
    /* ignore disabled irq bits */
    and \irqstat, \irqstat, \tmp
    /* check whether DRAMC interrupt happens */
    mov \irqnr, #32
    and \tmp, \irqstat, #1
    teq \tmp, #0
    bne 100f

    /* load irq enable bit mask */
    ldr \tmp, [\base, #REG_IRQEN]
    /* load current irq bits */
    ldr \irqstat, [\base, #REG_IRQST]
    /* ignore disabled irq bits */
    and \irqstat, \irqstat, \tmp
    /* check whether vdoin interrupt happens */
    mov \irqnr, #20
    and \tmp, \irqstat, #0x00100000
    teq \tmp, #0
    bne 100f

    /* ignore misc interrupt bit(31) */
    and \irqstat, \irqstat, #0x7FFFFFFF
    /* get irqnr (highest irq number) */
    clz \irqnr, \irqstat
    rsb \irqnr, \irqnr, #31
    /* test if no irq */
    teq \irqstat, #0
	bne 100f

    /* load irq enable bit mask */
    ldr \tmp, [\base, #REG_RW_MINTEN]
    /* load current irq bits */
    ldr \irqstat, [\base, #REG_RW_MINTST]
    /* ignore disabled irq bits */
    and \irqstat, \irqstat, \tmp

    /* get irqnr (highest irq number) */
    clz \irqnr, \irqstat
    rsb \irqnr, \irqnr, #63

100:	
    /* test if no irq */
    teq \irqstat, #0
    /* if irqstat == 0, means no irq currently */
    /* else irqnr contains the highest irq number */
    .endm
