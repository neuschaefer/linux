// SPDX-License-Identifier: GPL-2.0-only
/*
 * Nuvoton WPCM450 clock and reset controller driver.
 *
 * Copyright (C) 2022 Jonathan Neuschäfer <j.neuschaefer@gmx.net>
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/bitfield.h>
#include <linux/clk-provider.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/reset-controller.h>
#include <linux/reset/reset-simple.h>
#include <linux/slab.h>

#include <dt-bindings/clock/nuvoton,wpcm450-clk.h>

struct wpcm450_clk_pll {
	struct clk_hw hw;
	void __iomem *pllcon;
	u8 flags;
};

#define to_wpcm450_clk_pll(_hw) container_of(_hw, struct wpcm450_clk_pll, hw)

#define PLLCON_FBDV	GENMASK(24, 16)
#define PLLCON_PRST	BIT(13)
#define PLLCON_PWDEN	BIT(12)
#define PLLCON_OTDV	GENMASK(10, 8)
#define PLLCON_INDV	GENMASK(5, 0)

static unsigned long wpcm450_clk_pll_recalc_rate(struct clk_hw *hw,
						 unsigned long parent_rate)
{
	struct wpcm450_clk_pll *pll = to_wpcm450_clk_pll(hw);
	unsigned long fbdv, indv, otdv;
	u64 rate;
	u32 pllcon;

	if (parent_rate == 0)
		return 0;

	pllcon = readl_relaxed(pll->pllcon);

	indv = FIELD_GET(PLLCON_INDV, pllcon) + 1;
	fbdv = FIELD_GET(PLLCON_FBDV, pllcon) + 1;
	otdv = FIELD_GET(PLLCON_OTDV, pllcon) + 1;

	rate = (u64)parent_rate * fbdv;
	do_div(rate, indv * otdv);

	return rate;
}

static int wpcm450_clk_pll_is_enabled(struct clk_hw *hw)
{
	struct wpcm450_clk_pll *pll = to_wpcm450_clk_pll(hw);
	u32 pllcon;

	pllcon = readl_relaxed(pll->pllcon);

	return !(pllcon & PLLCON_PRST);
}

static void wpcm450_clk_pll_disable(struct clk_hw *hw)
{
	struct wpcm450_clk_pll *pll = to_wpcm450_clk_pll(hw);
	u32 pllcon;

	pllcon = readl_relaxed(pll->pllcon);
	pllcon |= PLLCON_PRST | PLLCON_PWDEN;
	writel(pllcon, pll->pllcon);
}

static const struct clk_ops wpcm450_clk_pll_ops = {
	.recalc_rate = wpcm450_clk_pll_recalc_rate,
	.is_enabled = wpcm450_clk_pll_is_enabled,
	.disable = wpcm450_clk_pll_disable
};

static struct clk_hw *
wpcm450_clk_register_pll(struct device_node *np, void __iomem *pllcon, const char *name,
			 const struct clk_parent_data *parent, unsigned long flags)
{
	struct wpcm450_clk_pll *pll;
	struct clk_init_data init = {};
	int ret;

	pll = kzalloc(sizeof(*pll), GFP_KERNEL);
	if (!pll)
		return ERR_PTR(-ENOMEM);

	init.name = name;
	init.ops = &wpcm450_clk_pll_ops;
	init.parent_data = parent;
	init.num_parents = 1;
	init.flags = flags;

	pll->pllcon = pllcon;
	pll->hw.init = &init;

	ret = of_clk_hw_register(np, &pll->hw);
	if (ret) {
		kfree(pll);
		return ERR_PTR(ret);
	}

	return &pll->hw;
}

#define REG_CLKEN	0x00
#define REG_CLKSEL	0x04
#define REG_CLKDIV	0x08
#define REG_PLLCON0	0x0c
#define REG_PLLCON1	0x10
#define REG_PMCON	0x14
#define REG_IRQWAKECON	0x18
#define REG_IRQWAKEFLAG	0x1c
#define REG_IPSRST	0x20

struct wpcm450_pll_data {
	const char *name;
	struct clk_parent_data parent;
	unsigned int reg;
	unsigned long flags;
};

static const struct wpcm450_pll_data pll_data[] = {
	{ "pll0", { .fw_name = "ref" }, REG_PLLCON0, 0 },
	{ "pll1", { .fw_name = "ref" }, REG_PLLCON1, 0 },
};

struct wpcm450_clksel_data {
	const char *name;
	const struct clk_parent_data *parents;
	unsigned int num_parents;
	const u32 *table;
	int shift;
	int width;
	int index;
	unsigned long flags;
};

static const u32 parent_table[] = { 0, 1, 2 };

static const struct clk_parent_data default_parents[] = {
	{ .name = "pll0" },
	{ .name = "pll1" },
	{ .name = "ref" },
};

static const struct clk_parent_data huart_parents[] = {
	{ .fw_name = "ref" },
	{ .name = "refdiv2" },
};

static const struct wpcm450_clksel_data clksel_data[] = {
	{ "cpusel", default_parents, ARRAY_SIZE(default_parents),
		parent_table, 0, 2, -1, CLK_IS_CRITICAL },
	{ "clkout", default_parents, ARRAY_SIZE(default_parents),
		parent_table, 2, 2, -1, 0 },
	{ "usbphy", default_parents, ARRAY_SIZE(default_parents),
		parent_table, 6, 2, -1, 0 },
	{ "uartsel", default_parents, ARRAY_SIZE(default_parents),
		parent_table, 8, 2, WPCM450_CLK_USBPHY, 0 },
	{ "huartsel", huart_parents, ARRAY_SIZE(huart_parents),
		parent_table, 10, 1, -1, 0 },
};

static const struct clk_div_table div_fixed2[] = {
	{ .val = 0, .div = 2 },
	{ }
};

struct wpcm450_clkdiv_data {
	const char *name;
	struct clk_parent_data parent;
	int div_flags;
	const struct clk_div_table *table;
	int shift;
	int width;
	unsigned long flags;
};

static struct wpcm450_clkdiv_data clkdiv_data_early[] = {
	{ "refdiv2", { .name = "ref" }, 0, div_fixed2, 0, 0 },
};

static const struct wpcm450_clkdiv_data clkdiv_data[] = {
	{ "cpu", { .name = "cpusel" }, 0, div_fixed2, 0, 0, CLK_IS_CRITICAL },
	{ "adcdiv", { .name = "ref" }, CLK_DIVIDER_POWER_OF_TWO, NULL, 28, 2, 0 },
	{ "apb", { .name = "ahb" }, CLK_DIVIDER_POWER_OF_TWO, NULL, 26, 2, 0 },
	{ "ahb", { .name = "cpu" }, CLK_DIVIDER_POWER_OF_TWO, NULL, 24, 2, 0 },
	{ "uart", { .name = "uartsel" }, 0, NULL, 16, 4, 0 },
	{ "ahb3", { .name = "ahb" }, CLK_DIVIDER_POWER_OF_TWO, NULL, 8, 2, 0 },
};

struct wpcm450_clken_data {
	const char *name;
	struct clk_parent_data parent;
	int bitnum;
	unsigned long flags;
};

static const struct wpcm450_clken_data clken_data[] = {
	{ "fiu", { .name = "ahb3" }, WPCM450_CLK_FIU, 0 },
	{ "xbus", { .name = "ahb3" }, WPCM450_CLK_XBUS, 0 },
	{ "kcs", { .name = "apb" }, WPCM450_CLK_KCS, 0 },
	{ "shm", { .name = "ahb3" }, WPCM450_CLK_SHM, 0 },
	{ "usb1", { .name = "ahb" }, WPCM450_CLK_USB1, 0 },
	{ "emc0", { .name = "ahb" }, WPCM450_CLK_EMC0, 0 },
	{ "emc1", { .name = "ahb" }, WPCM450_CLK_EMC1, 0 },
	{ "usb0", { .name = "ahb" }, WPCM450_CLK_USB0, 0 },
	{ "peci", { .name = "apb" }, WPCM450_CLK_PECI, 0 },
	{ "aes", { .name = "apb" }, WPCM450_CLK_AES, 0 },
	{ "uart0", { .name = "uart" }, WPCM450_CLK_UART0, 0 },
	{ "uart1", { .name = "uart" }, WPCM450_CLK_UART1, 0 },
	{ "smb2", { .name = "apb" }, WPCM450_CLK_SMB2, 0 },
	{ "smb3", { .name = "apb" }, WPCM450_CLK_SMB3, 0 },
	{ "smb4", { .name = "apb" }, WPCM450_CLK_SMB4, 0 },
	{ "smb5", { .name = "apb" }, WPCM450_CLK_SMB5, 0 },
	{ "huart", { .name = "huartsel" }, WPCM450_CLK_HUART, 0 },
	{ "pwm", { .name = "apb" }, WPCM450_CLK_PWM, 0 },
	{ "timer0", { .name = "refdiv2" }, WPCM450_CLK_TIMER0, 0 },
	{ "timer1", { .name = "refdiv2" }, WPCM450_CLK_TIMER1, 0 },
	{ "timer2", { .name = "refdiv2" }, WPCM450_CLK_TIMER2, 0 },
	{ "timer3", { .name = "refdiv2" }, WPCM450_CLK_TIMER3, 0 },
	{ "timer4", { .name = "refdiv2" }, WPCM450_CLK_TIMER4, 0 },
	{ "mft0", { .name = "apb" }, WPCM450_CLK_MFT0, 0 },
	{ "mft1", { .name = "apb" }, WPCM450_CLK_MFT1, 0 },
	{ "wdt", { .name = "refdiv2" }, WPCM450_CLK_WDT, 0 },
	{ "adc", { .name = "adcdiv" }, WPCM450_CLK_ADC, 0 },
	{ "sdio", { .name = "ahb" }, WPCM450_CLK_SDIO, 0 },
	{ "sspi", { .name = "apb" }, WPCM450_CLK_SSPI, 0 },
	{ "smb0", { .name = "apb" }, WPCM450_CLK_SMB0, 0 },
	{ "smb1", { .name = "apb" }, WPCM450_CLK_SMB1, 0 },
};

static DEFINE_SPINLOCK(wpcm450_clk_lock);

/*
 * NOTE: Error handling is very rudimentary here. If the clock driver initial-
 * ization fails, the system is probably in bigger trouble than what is caused
 * by a few leaked resources.
 */

static void __init wpcm450_clk_init(struct device_node *np)
{
	struct clk_hw_onecell_data *clk_data;
	static struct clk_hw **hws;
	static struct clk_hw *hw;
	void __iomem *clk_base;
	int i, ret;
	struct reset_simple_data *reset;

	clk_base = of_iomap(np, 0);
	if (!clk_base) {
		pr_err("%pOFP: failed to map registers\n", np);
		of_node_put(np);
		return;
	}
	of_node_put(np);

	clk_data = kzalloc(struct_size(clk_data, hws, WPCM450_NUM_CLKS), GFP_KERNEL);
	if (!clk_data)
		return;

	clk_data->num = WPCM450_NUM_CLKS;
	hws = clk_data->hws;

	for (i = 0; i < WPCM450_NUM_CLKS; i++)
		hws[i] = ERR_PTR(-ENOENT);

	/* PLLs */
	for (i = 0; i < ARRAY_SIZE(pll_data); i++) {
		const struct wpcm450_pll_data *data = &pll_data[i];

		hw = wpcm450_clk_register_pll(np, clk_base + data->reg, data->name,
					      &data->parent, data->flags);
		if (IS_ERR(hw)) {
			pr_info("Failed to register PLL: %pe\n", hw);
			return;
		}
	}

	/* Early divisors (REF/2) */
	for (i = 0; i < ARRAY_SIZE(clkdiv_data_early); i++) {
		const struct wpcm450_clkdiv_data *data = &clkdiv_data_early[i];

		hw = clk_hw_register_divider_table_parent_data(NULL, data->name, &data->parent,
							       data->flags, clk_base + REG_CLKDIV,
							       data->shift, data->width,
							       data->div_flags, data->table,
							       &wpcm450_clk_lock);
		if (IS_ERR(hw)) {
			pr_err("Failed to register div table: %pe\n", hw);
			return;
		}
	}

	/* Selects/muxes */
	for (i = 0; i < ARRAY_SIZE(clksel_data); i++) {
		const struct wpcm450_clksel_data *data = &clksel_data[i];

		hw = clk_hw_register_mux_parent_data(NULL, data->name, data->parents,
						     data->num_parents, data->flags,
						     clk_base + REG_CLKSEL, data->shift,
						     data->width, 0,
						     &wpcm450_clk_lock);
		if (IS_ERR(hw)) {
			pr_err("Failed to register mux: %pe\n", hw);
			return;
		}
		if (data->index >= 0)
			clk_data->hws[data->index] = hw;
	}

	/* Divisors */
	for (i = 0; i < ARRAY_SIZE(clkdiv_data); i++) {
		const struct wpcm450_clkdiv_data *data = &clkdiv_data[i];

		hw = clk_hw_register_divider_table_parent_data(NULL, data->name, &data->parent,
							       data->flags, clk_base + REG_CLKDIV,
							       data->shift, data->width,
							       data->div_flags, data->table,
							       &wpcm450_clk_lock);
		if (IS_ERR(hw)) {
			pr_err("Failed to register divider: %pe\n", hw);
			return;
		}
	}

	/* Enables/gates */
	for (i = 0; i < ARRAY_SIZE(clken_data); i++) {
		const struct wpcm450_clken_data *data = &clken_data[i];

		hw = clk_hw_register_gate_parent_data(NULL, data->name, &data->parent, data->flags,
						      clk_base + REG_CLKEN, data->bitnum,
						      data->flags, &wpcm450_clk_lock);
		if (IS_ERR(hw)) {
			pr_err("Failed to register gate: %pe\n", hw);
			return;
		}
		clk_data->hws[data->bitnum] = hw;
	}

	ret = of_clk_add_hw_provider(np, of_clk_hw_onecell_get, clk_data);
	if (ret)
		pr_err("Failed to add DT provider: %pe\n", ERR_PTR(ret));

	/* Reset controller */
	reset = kzalloc(sizeof(*reset), GFP_KERNEL);
	if (!reset)
		return;
	reset->rcdev.owner = THIS_MODULE;
	reset->rcdev.nr_resets = WPCM450_NUM_RESETS;
	reset->rcdev.ops = &reset_simple_ops;
	reset->rcdev.of_node = np;
	reset->membase = clk_base + REG_IPSRST;
	ret = reset_controller_register(&reset->rcdev);
	if (ret)
		pr_err("Failed to register reset controller: %pe\n", ERR_PTR(ret));

	of_node_put(np);
}

CLK_OF_DECLARE(wpcm450_clk_init, "nuvoton,wpcm450-clk", wpcm450_clk_init);
