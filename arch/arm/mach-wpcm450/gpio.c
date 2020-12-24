/* linux/arch/arm/mach-wpcm450/gpio.c
 *
 * Copyright (c) 2006 Winbond Electronics
 * All rights reserved.
 *
 * shirley yu (clyu2@winbond.com)
 *
 * WPCM450 GPIO support
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
 * Changelog
 *	22-08-2006  clyu  Created file
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

void wpcm450_gpio_cfgpin(unsigned int pin, unsigned int function)
{
#if 0
	void __iomem *base = WPCM450_GPIO_BASE(pin);
	unsigned long mask;
	unsigned long con;
	unsigned long flags;

	if (pin < WPCM450_GPIO_BANKB) {
		mask = 1 << WPCM450_GPIO_OFFSET(pin);
	} else {
		mask = 3 << WPCM450_GPIO_OFFSET(pin)*2;
	}

	switch (function) {
	case WPCM450_GPIO_LEAVE:
		mask = 0;
		function = 0;
		break;

	case WPCM450_GPIO_INPUT:
	case WPCM450_GPIO_OUTPUT:
	case WPCM450_GPIO_SFN2:
	case WPCM450_GPIO_SFN3:
		if (pin < WPCM450_GPIO_BANKB) {
			function &= 1;
			function <<= WPCM450_GPIO_OFFSET(pin);
		} else {
			function &= 3;
			function <<= WPCM450_GPIO_OFFSET(pin)*2;
		}
	}

	/* modify the specified register wwith IRQs off */

	local_irq_save(flags);

	con  = __raw_readl(base + 0x00);
	con &= ~mask;
	con |= function;

	__raw_writel(con, base + 0x00);

	local_irq_restore(flags);
#endif
}

EXPORT_SYMBOL(wpcm450_gpio_cfgpin);

unsigned int wpcm450_gpio_getcfg(unsigned int pin)
{
#if 0
	void __iomem *base = WPCM450_GPIO_BASE(pin);
	unsigned long mask;
	if (pin < WPCM450_GPIO_BANKB) {
		mask = 1 << WPCM450_GPIO_OFFSET(pin);
	} else {
		mask = 3 << WPCM450_GPIO_OFFSET(pin)*2;
	}

	return __raw_readl(base) & mask;
#else
	return 0;
#endif

}

EXPORT_SYMBOL(wpcm450_gpio_getcfg);

void wpcm450_gpio_pullup(unsigned int pin, unsigned int to)
{
#if 0
	void __iomem *base = WPCM450_GPIO_BASE(pin);
	unsigned long offs = WPCM450_GPIO_OFFSET(pin);
	unsigned long flags;
	unsigned long up;

	if (pin < WPCM450_GPIO_BANKB)
		return;

	local_irq_save(flags);

	up = __raw_readl(base + 0x08);
	up &= ~(1L << offs);
	up |= to << offs;
	__raw_writel(up, base + 0x08);

	local_irq_restore(flags);
#endif
}

EXPORT_SYMBOL(wpcm450_gpio_pullup);

void wpcm450_gpio_setpin(unsigned int pin, unsigned int to)
{
#if 0
	void __iomem *base = WPCM450_GPIO_BASE(pin);
	unsigned long offs = WPCM450_GPIO_OFFSET(pin);
	unsigned long flags;
	unsigned long dat;

	local_irq_save(flags);

	dat = __raw_readl(base + 0x04);
	dat &= ~(1 << offs);
	dat |= to << offs;
	__raw_writel(dat, base + 0x04);

	local_irq_restore(flags);
#endif
}

EXPORT_SYMBOL(wpcm450_gpio_setpin);

unsigned int wpcm450_gpio_getpin(unsigned int pin)
{
#if 0
	void __iomem *base = WPCM450_GPIO_BASE(pin);
	unsigned long offs = WPCM450_GPIO_OFFSET(pin);

	return __raw_readl(base + 0x04) & (1<< offs);
#else
	return 0;
#endif
}

EXPORT_SYMBOL(wpcm450_gpio_getpin);

unsigned int wpcm450_modify_misccr(unsigned int clear, unsigned int change)
{
#if 0
	unsigned long flags;
	unsigned long misccr;

	local_irq_save(flags);
	misccr = __raw_readl(WPCM450_MISCCR);
	misccr &= ~clear;
	misccr ^= change;
	__raw_writel(misccr, W90P94XX_MISCCR);
	local_irq_restore(flags);

	return misccr;
#else
	return 0;
#endif

}

EXPORT_SYMBOL(wpcm450_modify_misccr);
