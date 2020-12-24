/* linux/arch/arm/mach-wpcm450/gpio.c
 *
 * Copyright (c) 2006 winbond Electronics
 *	shirley yu <clyu2@winbond.com>
 *
 * wpcm450 GPIO support
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
 *  Changelog:
 *    22-08-2006     clyu     Created file
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>

#include <asm/hardware.h>
#include <asm/irq.h>
#include <asm/io.h>

#include <asm/arch/regs-gpio.h>

int wpcm450_gpio_irqfilter(unsigned int pin, unsigned int on,
			   unsigned int config)
{
	void __iomem *reg = WPCM450_EINFLT0;
	unsigned long flags;
	unsigned long val;

	if (pin < WPCM450_GPG8 || pin > WPCM450_GPG15)
		return -1;

	config &= 0xff;

	pin -= WPCM450_GPG8_EINT16;
	reg += pin & ~3;

	local_irq_save(flags);

	/* update filter width and clock source */

	val = __raw_readl(reg);
	val &= ~(0xff << ((pin & 3) * 8));
	val |= config << ((pin & 3) * 8);
	__raw_writel(val, reg);

	/* update filter enable */

	val = __raw_readl(WPCM450_EXTINT2);
	val &= ~(1 << ((pin * 4) + 3));
	val |= on << ((pin * 4) + 3);
	__raw_writel(val, WPCM450_EXTINT2);

	local_irq_restore(flags);

	return 0;
}

EXPORT_SYMBOL(wpcm450_gpio_irqfilter);

int wpcm450_gpio_getirq(unsigned int pin)
{
	if (pin < WPCM450_GPF0 || pin > WPCM450_GPG15_EINT23)
		return -1;	/* not valid interrupts */

	if (pin < WPCM450_GPG0 && pin > WPCM450_GPF7)
		return -1;	/* not valid pin */

	if (pin < WPCM450_GPF4)
		return (pin - WPCM450_GPF0) + IRQ_EINT0;

	if (pin < WPCM450_GPG0)
		return (pin - WPCM450_GPF4) + IRQ_EINT4;

	return (pin - WPCM450_GPG0) + IRQ_EINT8;
}

EXPORT_SYMBOL(wpcm450_gpio_getirq);
