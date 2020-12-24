/* linux/arch/arm/mach-wpcm450/clock.c
 *
 * Copyright (c) 2006 Winbond Electronics
 * All rights reserved.
 *
 * shirley yu (clyu2@winbond.com)
 *
 * WPCM450 Clock control support
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
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/sysdev.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/clk.h>
#include <linux/mutex.h>
#include <linux/delay.h>

#include <asm/hardware.h>
#include <asm/irq.h>
#include <asm/io.h>

#include <asm/arch/regs-clock.h>
#include <asm/arch/regs-gpio.h>

#include "clock.h"
#include "cpu.h"

/* clock information */

static LIST_HEAD(clocks);

DEFINE_MUTEX(clocks_mutex);

/* old functions */

void inline wpcm450_clk_enable(unsigned int clocks, unsigned int enable)
{
#if 0
	unsigned long clkcon;

	clkcon = __raw_readl(WPCM450_CLKCON);

	if (enable)
		clkcon |= clocks;
	else
		clkcon &= ~clocks;

	/* ensure none of the special function bits set */
	clkcon &= ~(WPCM450_CLKCON_IDLE|WPCM450_CLKCON_POWER);

	__raw_writel(clkcon, WPCM450_CLKCON);
#endif
}

/* enable and disable calls for use with the clk struct */

#if 0 /* currently not used */
static int clk_null_enable(struct clk *clk, int enable)
{
	return 0;
}
#endif

int wpcm450_clkcon_enable(struct clk *clk, int enable)
{
	wpcm450_clk_enable(clk->ctrlbit, enable);
	return 0;
}

/* Clock API calls */

struct clk *clk_get(struct device *dev, const char *id)
{
	struct clk *p;
	struct clk *clk = ERR_PTR(-ENOENT);
	int idno;

	if (dev == NULL || dev->bus != &platform_bus_type)
		idno = -1;
	else
		idno = to_platform_device(dev)->id;

	mutex_lock(&clocks_mutex);

	list_for_each_entry(p, &clocks, list) {
		if (p->id == idno &&
		    strcmp(id, p->name) == 0 &&
		    try_module_get(p->owner)) {
			clk = p;
			break;
		}
	}

	/* check for the case where a device was supplied, but the
	 * clock that was being searched for is not device specific */

	if (IS_ERR(clk)) {
		list_for_each_entry(p, &clocks, list) {
			if (p->id == -1 && strcmp(id, p->name) == 0 &&
			    try_module_get(p->owner)) {
				clk = p;
				break;
			}
		}
	}

	mutex_unlock(&clocks_mutex);
	return clk;
}

void clk_put(struct clk *clk)
{
	module_put(clk->owner);
}

int clk_enable(struct clk *clk)
{
	if (IS_ERR(clk) || clk == NULL)
		return -EINVAL;

	clk_enable(clk->parent);

	mutex_lock(&clocks_mutex);

	if ((clk->usage++) == 0)
		(clk->enable)(clk, 1);

	mutex_unlock(&clocks_mutex);
	return 0;
}

void clk_disable(struct clk *clk)
{
	if (IS_ERR(clk) || clk == NULL)
		return;

	mutex_lock(&clocks_mutex);

	if ((--clk->usage) == 0)
		(clk->enable)(clk, 0);

	mutex_unlock(&clocks_mutex);
	clk_disable(clk->parent);
}


unsigned long clk_get_rate(struct clk *clk)
{
	if (IS_ERR(clk))
		return 0;

	if (clk->rate != 0)
		return clk->rate;

	while (clk->parent != NULL && clk->rate == 0)
		clk = clk->parent;

	return clk->rate;
}

long clk_round_rate(struct clk *clk, unsigned long rate)
{
	if (!IS_ERR(clk) && clk->round_rate)
		return (clk->round_rate)(clk, rate);

	return rate;
}

int clk_set_rate(struct clk *clk, unsigned long rate)
{
	int ret;

	if (IS_ERR(clk))
		return -EINVAL;

	mutex_lock(&clocks_mutex);
	ret = (clk->set_rate)(clk, rate);
	mutex_unlock(&clocks_mutex);

	return ret;
}

struct clk *clk_get_parent(struct clk *clk)
{
	return clk->parent;
}

int clk_set_parent(struct clk *clk, struct clk *parent)
{
	int ret = 0;

	if (IS_ERR(clk))
		return -EINVAL;

	mutex_lock(&clocks_mutex);

	if (clk->set_parent)
		ret = (clk->set_parent)(clk, parent);

	mutex_unlock(&clocks_mutex);

	return ret;
}

EXPORT_SYMBOL(clk_get);
EXPORT_SYMBOL(clk_put);
EXPORT_SYMBOL(clk_enable);
EXPORT_SYMBOL(clk_disable);
EXPORT_SYMBOL(clk_get_rate);
EXPORT_SYMBOL(clk_round_rate);
EXPORT_SYMBOL(clk_set_rate);
EXPORT_SYMBOL(clk_get_parent);
EXPORT_SYMBOL(clk_set_parent);

/* base clock enable */

#if 0 /* currently not used */
static int wpcm450_upll_enable(struct clk *clk, int enable)
{
#if 0
	unsigned long clkslow = __raw_readl(WPCM450_CLKSLOW);
	unsigned long orig = clkslow;

	if (enable)
		clkslow &= ~WPCM450_CLKSLOW_UCLK_OFF;
	else
		clkslow |= WPCM450_CLKSLOW_UCLK_OFF;

	__raw_writel(clkslow, WPCM450_CLKSLOW);

	/* if we started the UPLL, then allow to settle */

	if (enable && (orig & WPCM450_CLKSLOW_UCLK_OFF))
		udelay(200);
#endif
	return 0;
}
#endif

/* base clocks */
#if 0
static struct clk clk_xtal = {
	.name		= "xtal",
	.id		= -1,
	.rate		= 0,
	.parent		= NULL,
	.ctrlbit	= 0,
};

static struct clk clk_upll = {
	.name		= "upll",
	.id		= -1,
	.parent		= NULL,
	.enable		= s3c24xx_upll_enable,
	.ctrlbit	= 0,
};

static struct clk clk_f = {
	.name		= "fclk",
	.id		= -1,
	.rate		= 0,
	.parent		= NULL,
	.ctrlbit	= 0,
};

static struct clk clk_h = {
	.name		= "hclk",
	.id		= -1,
	.rate		= 0,
	.parent		= NULL,
	.ctrlbit	= 0,
};

static struct clk clk_p = {
	.name		= "pclk",
	.id		= -1,
	.rate		= 0,
	.parent		= NULL,
	.ctrlbit	= 0,
};

struct clk clk_usb_bus = {
	.name		= "usb-bus",
	.id		= -1,
	.rate		= 0,
	.parent		= &clk_upll,
};
#endif

/* clocks that could be registered by external code */

#if 0 /* currently not used */
static int wpcm450_dclk_enable(struct clk *clk, int enable)
{
#if 0
	unsigned long dclkcon = __raw_readl(WPCM450_DCLKCON);

	if (enable)
		dclkcon |= clk->ctrlbit;
	else
		dclkcon &= ~clk->ctrlbit;

	__raw_writel(dclkcon, WPCM450_DCLKCON);
#endif
	return 0;
}

static int wpcm450_dclk_setparent(struct clk *clk, struct clk *parent)
{
#if 0
	unsigned long dclkcon;
	unsigned int uclk;

	if (parent == &clk_upll)
		uclk = 1;
	else if (parent == &clk_p)
		uclk = 0;
	else
		return -EINVAL;

	clk->parent = parent;

	dclkcon = __raw_readl(WPCM450_DCLKCON);

	if (clk->ctrlbit == WPCM450_DCLKCON_DCLK0EN) {
		if (uclk)
			dclkcon |= WPCM450_DCLKCON_DCLK0_UCLK;
		else
			dclkcon &= ~WPCM450_DCLKCON_DCLK0_UCLK;
	} else {
		if (uclk)
			dclkcon |= WPCM450_DCLKCON_DCLK1_UCLK;
		else
			dclkcon &= ~WPCM450_DCLKCON_DCLK1_UCLK;
	}

	__raw_writel(dclkcon, WPCM450_DCLKCON);
#endif
	return 0;
}

static int wpcm450_clkout_setparent(struct clk *clk, struct clk *parent)
{
#if 0
	unsigned long mask;
	unsigned long source;

	/* calculate the MISCCR setting for the clock */

	if (parent == &clk_xtal)
		source = WPCM450_MISCCR_CLK0_MPLL;
	else if (parent == &clk_upll)
		source = WPCM450_MISCCR_CLK0_UPLL;
	else if (parent == &clk_f)
		source = WPCM450_MISCCR_CLK0_FCLK;
	else if (parent == &clk_h)
		source = WPCM450_MISCCR_CLK0_HCLK;
	else if (parent == &clk_p)
		source = WPCM450_MISCCR_CLK0_PCLK;
	else if (clk == &wpcm450_clkout0 && parent == &wpcm450_dclk0)
		source = WPCM450_MISCCR_CLK0_DCLK0;
	else if (clk == &wpcm450_clkout1 && parent == &wpcm450_dclk1)
		source = WPCM450_MISCCR_CLK0_DCLK0;
	else
		return -EINVAL;

	clk->parent = parent;

	if (clk == &wpcm450_dclk0)
		mask = WPCM450_MISCCR_CLK0_MASK;
	else {
		source <<= 4;
		mask = WPCM450_MISCCR_CLK1_MASK;
	}

	wpcm450_modify_misccr(mask, source);
#endif
	return 0;
}
#endif

/* external clock definitions */

#if 0
struct clk wpcm450_dclk0 = {
	.name		= "dclk0",
	.id		= -1,
	.ctrlbit	= WPCM450_DCLKCON_DCLK0EN,
	.enable	        = wpcm450_dclk_enable,
	.set_parent	= wpcm450_dclk_setparent,
};

struct clk wpcm450_dclk1 = {
	.name		= "dclk1",
	.id		= -1,
	.ctrlbit	= WPCM450_DCLKCON_DCLK0EN,
	.enable		= wpcm450_dclk_enable,
	.set_parent	= wpcm450_dclk_setparent,
};

struct clk wpcm450_clkout0 = {
	.name		= "clkout0",
	.id		= -1,
	.set_parent	= wpcm450_clkout_setparent,
};

struct clk wpcm450_clkout1 = {
	.name		= "clkout1",
	.id		= -1,
	.set_parent	= wpcm450_clkout_setparent,
};

struct clk wpcm450_uclk = {
	.name		= "uclk",
	.id		= -1,
};
#endif

/* standard clock definitions */
#if 0
static struct clk init_clocks[] = {
	{
		.name		= "nand",
		.id		= -1,
		.parent		= &clk_h,
		.enable		= wpcm450_clkcon_enable,
		.ctrlbit	= WPCM450_CLKCON_NAND,
	}, {
		.name		= "lcd",
		.id		= -1,
		.parent		= &clk_h,
		.enable		= wpcm450_clkcon_enable,
		.ctrlbit	= WPCM450_CLKCON_LCDC,
	}, {
		.name		= "usb-host",
		.id		= -1,
		.parent		= &clk_h,
		.enable		= wpcm450_clkcon_enable,
		.ctrlbit	= WPCM450_CLKCON_USBH,
	}, {
		.name		= "usb-device",
		.id		= -1,
		.parent		= &clk_h,
		.enable		= wpcm450_clkcon_enable,
		.ctrlbit	= WPCM450_CLKCON_USBD,
	}, {
		.name		= "timers",
		.id		= -1,
		.parent		= &clk_p,
		.enable		= wpcm450_clkcon_enable,
		.ctrlbit	= WPCM450_CLKCON_PWMT,
	}, {
		.name		= "sdi",
		.id		= -1,
		.parent		= &clk_p,
		.enable		= wpcm450_clkcon_enable,
		.ctrlbit	= WPCM450_CLKCON_SDI,
	}, {
		.name		= "uart",
		.id		= 0,
		.parent		= &clk_p,
		.enable		= wpcm450_clkcon_enable,
		.ctrlbit	= WPCM450_CLKCON_UART0,
	}, {
		.name		= "uart",
		.id		= 1,
		.parent		= &clk_p,
		.enable		= wpcm450_clkcon_enable,
		.ctrlbit	= WPCM4500_CLKCON_UART1,
	}, {
		.name		= "uart",
		.id		= 2,
		.parent		= &clk_p,
		.enable		= wpcm450_clkcon_enable,
		.ctrlbit	= WPCM450_CLKCON_UART2,
	}, {
		.name		= "gpio",
		.id		= -1,
		.parent		= &clk_p,
		.enable		= wpcm450_clkcon_enable,
		.ctrlbit	= WPCM450_CLKCON_GPIO,
	}, {
		.name		= "rtc",
		.id		= -1,
		.parent		= &clk_p,
		.enable		= wpcm450_clkcon_enable,
		.ctrlbit	= WPCM450_CLKCON_RTC,
	}, {
		.name		= "adc",
		.id		= -1,
		.parent		= &clk_p,
		.enable		= wpcm450_clkcon_enable,
		.ctrlbit	= WPCM450_CLKCON_ADC,
	}, {
		.name		= "i2c",
		.id		= -1,
		.parent		= &clk_p,
		.enable		= wpcm450_clkcon_enable,
		.ctrlbit	= WPCM450_CLKCON_IIC,
	}, {
		.name		= "iis",
		.id		= -1,
		.parent		= &clk_p,
		.enable		= wpcm450_clkcon_enable,
		.ctrlbit	= WPCM450_CLKCON_IIS,
	}, {
		.name		= "spi",
		.id		= -1,
		.parent		= &clk_p,
		.enable		= wpcm450_clkcon_enable,
		.ctrlbit	= WPCM450_CLKCON_SPI,
	}, {
		.name		= "watchdog",
		.id		= -1,
		.parent		= &clk_p,
		.ctrlbit	= 0,
	}
};
#endif

/* initialise the clock system */

int wpcm450_register_clock(struct clk *clk)
{
#if 0
	clk->owner = THIS_MODULE;

	if (clk->enable == NULL)
		clk->enable = clk_null_enable;

	/* if this is a standard clock, set the usage state */

	if (clk->ctrlbit && clk->enable == wpcm450_clkcon_enable) {
		unsigned long clkcon = __raw_readl(WPCM450_CLKCON);

		clk->usage = (clkcon & clk->ctrlbit) ? 1 : 0;
	}

	/* add to the list of available clocks */

	mutex_lock(&clocks_mutex);
	list_add(&clk->list, &clocks);
	mutex_unlock(&clocks_mutex);
#endif
	return 0;
}

/* initalise all the clocks */

int __init wpcm450_setup_clocks(unsigned long xtal,
				unsigned long fclk,
				unsigned long hclk,
				unsigned long pclk)
{
#if 0
	unsigned long upllcon = __raw_readl(WPCM450_UPLLCON);
	unsigned long clkslow = __raw_readl(WPCM450_CLKSLOW);
	struct clk *clkp = init_clocks;
	int ptr;
	int ret;

	printk(KERN_INFO "wpcm450 Clocks, (c) 2004 Simtec Electronics\n");

	/* initialise the main system clocks */

	clk_xtal.rate = xtal;
	clk_upll.rate = wpcm450_get_pll(upllcon, xtal);

	clk_h.rate = hclk;
	clk_p.rate = pclk;
	clk_f.rate = fclk;

	/* We must be careful disabling the clocks we are not intending to
	 * be using at boot time, as subsytems such as the LCD which do
	 * their own DMA requests to the bus can cause the system to lockup
	 * if they where in the middle of requesting bus access.
	 *
	 * Disabling the LCD clock if the LCD is active is very dangerous,
	 * and therefore the bootloader should be  careful to not enable
	 * the LCD clock if it is not needed.
	*/

	mutex_lock(&clocks_mutex);

	s3c24xx_clk_enable(WPCM450_CLKCON_NAND, 0);
	s3c24xx_clk_enable(WPCM450_CLKCON_USBH, 0);
	s3c24xx_clk_enable(WPCM450_CLKCON_USBD, 0);
	s3c24xx_clk_enable(WPCM450_CLKCON_ADC, 0);
	s3c24xx_clk_enable(WPCM450_CLKCON_IIC, 0);
	s3c24xx_clk_enable(WPCM450_CLKCON_SPI, 0);

	mutex_unlock(&clocks_mutex);

	/* assume uart clocks are correctly setup */

	/* register our clocks */

	if (wpcm450_register_clock(&clk_xtal) < 0)
		printk(KERN_ERR "failed to register master xtal\n");

	if (wpcm450_register_clock(&clk_upll) < 0)
		printk(KERN_ERR "failed to register upll clock\n");

	if (wpcm450_register_clock(&clk_f) < 0)
		printk(KERN_ERR "failed to register cpu fclk\n");

	if (wpcm450_register_clock(&clk_h) < 0)
		printk(KERN_ERR "failed to register cpu hclk\n");

	if (wpcm450_register_clock(&clk_p) < 0)
		printk(KERN_ERR "failed to register cpu pclk\n");


	if (wpcm450_register_clock(&clk_usb_bus) < 0)
		printk(KERN_ERR "failed to register usb bus clock\n");

	/* register clocks from clock array */

	for (ptr = 0; ptr < ARRAY_SIZE(init_clocks); ptr++, clkp++) {
		ret = wpcm450_register_clock(clkp);
		if (ret < 0) {
			printk(KERN_ERR "Failed to register clock %s (%d)\n",
			       clkp->name, ret);
		}
	}

	/* show the clock-slow value */

	printk("CLOCK: Slow mode (%ld.%ld MHz), %s, MPLL %s, UPLL %s\n",
	       print_mhz(xtal / ( 2 * WPCM450_CLKSLOW_GET_SLOWVAL(clkslow))),
	       (clkslow & WPCM450_CLKSLOW_SLOW) ? "slow" : "fast",
	       (clkslow & WPCM450_CLKSLOW_MPLL_OFF) ? "off" : "on",
	       (clkslow & WPCM450_CLKSLOW_UCLK_OFF) ? "off" : "on");
#endif

	return 0;
}
