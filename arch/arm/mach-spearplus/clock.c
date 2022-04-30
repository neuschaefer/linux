/*
 *
 *  Copyright (C)2006  ST MICROELECTRONICS
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
 * AUTHOR :  Arif Khan <arif.khan@st.com> */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/err.h>

#include <asm/io.h>
#include <asm/semaphore.h>

#include <asm/arch/timex.h>

#include "clock.h"

static LIST_HEAD(clocks);
static DECLARE_MUTEX(clocks_sem);

struct clk *clk_get(struct device *dev, const char *id)
{
	struct clk *p, *clk = ERR_PTR(-ENOENT);

	down(&clocks_sem);
	list_for_each_entry(p, &clocks, node) {
		if (strcmp(id, p->name) == 0 && try_module_get(p->owner)) {
			clk = p;
			break;
		}
	}
	up(&clocks_sem);

	return clk;
}
EXPORT_SYMBOL(clk_get);

void clk_put(struct clk *clk)
{
	module_put(clk->owner);
}
EXPORT_SYMBOL(clk_put);

int clk_enable(struct clk *clk)
{
	return 0;
}
EXPORT_SYMBOL(clk_enable);

void clk_disable(struct clk *clk)
{
}
EXPORT_SYMBOL(clk_disable);

int clk_use(struct clk *clk)
{
	return 0;
}
EXPORT_SYMBOL(clk_use);

void clk_unuse(struct clk *clk)
{
}
EXPORT_SYMBOL(clk_unuse);

unsigned long clk_get_rate(struct clk *clk)
{
	return clk->rate;
}
EXPORT_SYMBOL(clk_get_rate);

long clk_round_rate(struct clk *clk, unsigned long rate)
{
	return rate;
}
EXPORT_SYMBOL(clk_round_rate);

int clk_set_rate(struct clk *clk, unsigned long rate)
{
	int ret = 0;


	/*
	 * Set the LCD clock rate (default is 48MHz).
	 */
	
	clk->rate = CLOCK_TICK_RATE;

	return ret;
}
EXPORT_SYMBOL(clk_set_rate);

/*
 * These are fixed clocks.
 */
static struct clk kmi_clk = {
	.name	= "KMIREFCLK",
	.rate	= 24000000,
};

static struct clk uart_clk = {
	.name	= "UARTCLK",
	.rate	= 24000000,
};

static struct clk clcd_clk = {
	.name	= "CLCDCLK",
	.rate	= 48000000,
};

static struct clk mmci_clk = {
	.name	= "MCLK",
	.rate	= 33000000,
};

int clk_register(struct clk *clk)
{
	down(&clocks_sem);
	list_add(&clk->node, &clocks);
	up(&clocks_sem);
	return 0;
}
EXPORT_SYMBOL(clk_register);

void clk_unregister(struct clk *clk)
{
	down(&clocks_sem);
	list_del(&clk->node);
	up(&clocks_sem);
}
EXPORT_SYMBOL(clk_unregister);

static int __init clk_init(void)
{
	clk_register(&kmi_clk);
	clk_register(&uart_clk);
	clk_register(&mmci_clk);
	clk_register(&clcd_clk);
	return 0;
}
arch_initcall(clk_init);
