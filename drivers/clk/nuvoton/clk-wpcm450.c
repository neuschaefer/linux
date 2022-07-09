// SPDX-License-Identifier: GPL-2.0-only
/*
 * Nuvoton WPCM450 clock and reset controller driver.
 *
 * Copyright (C) 2024 Jonathan Neuschäfer <j.neuschaefer@gmx.net>
 */

#include <linux/bitfield.h>
#include <linux/clk-provider.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/reset-controller.h>
#include <linux/reset/reset-simple.h>
#include <linux/slab.h>
#include <linux/platform_device.h>

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
wpcm450_clk_register_pll(struct device *dev, void __iomem *pllcon, const char *name,
			 const struct clk_parent_data *parent, unsigned long flags)
{
	struct wpcm450_clk_pll *pll;
	struct clk_init_data init = {};
	int ret;

	pll = devm_kzalloc(dev, sizeof(*pll), GFP_KERNEL);
	if (!pll)
		return ERR_PTR(-ENOMEM);

	init.name = name;
	init.ops = &wpcm450_clk_pll_ops;
	init.parent_data = parent;
	init.num_parents = 1;
	init.flags = flags;

	pll->pllcon = pllcon;
	pll->hw.init = &init;

	ret = devm_clk_hw_register(dev, &pll->hw);
	if (ret) {
		devm_kfree(dev, pll);
		return ERR_PTR(ret);
	}

	return &pll->hw;
}

// Additional clock indexes for internal use.
enum {
	WPCM450_CLK_REF = -1,
	WPCM450_CLK_REFDIV2 = WPCM450_NUM_CLKS,
	WPCM450_CLK_PLL0,
	WPCM450_CLK_PLL1,
	WPCM450_CLK_CPUSEL,
	WPCM450_CLK_CLKOUT,
	WPCM450_CLK_UARTSEL,
	WPCM450_CLK_HUARTSEL,
	WPCM450_CLK_CPU,
	WPCM450_CLK_ADCDIV,
	WPCM450_CLK_APB,
	WPCM450_CLK_AHB,
	WPCM450_CLK_AHB3,
	WPCM450_CLK_UART,
	WPCM450_NUM_CLKS_TOTAL
};

static struct clk_parent_data index_to_parent_data(struct clk_hw **hws, int index)
{
	struct clk_parent_data parent_data = {};

	if (index == WPCM450_CLK_REF)
		parent_data.fw_name = "ref";
	else
		parent_data.hw = hws[index];

	return parent_data;
}

static size_t index_to_parent_data_array(struct clk_hw **hws,
					 const int *indexes, size_t num_indexes,
					 struct clk_parent_data *parent_data,
					 size_t num_parent_data)
{
	size_t i;

	for (i = 0; i < min(num_parent_data, num_indexes); i++)
		parent_data[i] = index_to_parent_data(hws, indexes[i]);

	return i;
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
	int index;
	int parent;
	unsigned int reg;
	unsigned long flags;
};

static const struct wpcm450_pll_data pll_data[] = {
	{ "pll0", WPCM450_CLK_PLL0, WPCM450_CLK_REF, REG_PLLCON0, 0 },
	{ "pll1", WPCM450_CLK_PLL1, WPCM450_CLK_REF, REG_PLLCON1, 0 },
};

struct wpcm450_clksel_data {
	const char *name;
	int index;
	const int *parents;
	unsigned int num_parents;
	const u32 *table;
	int shift;
	int width;
	unsigned long flags;
};

static const u32 parent_table[] = { 0, 1, 2 };

static const int default_parents[] = {
	WPCM450_CLK_PLL0,
	WPCM450_CLK_PLL1,
	WPCM450_CLK_REF,
};

static const int huart_parents[] = {
	WPCM450_CLK_REF,
	WPCM450_CLK_REFDIV2,
};

static const struct wpcm450_clksel_data clksel_data[] = {
	{ "cpusel", WPCM450_CLK_CPUSEL, default_parents, ARRAY_SIZE(default_parents),
		parent_table, 0, 2, CLK_IS_CRITICAL },
	{ "clkout", WPCM450_CLK_CLKOUT, default_parents, ARRAY_SIZE(default_parents),
		parent_table, 2, 2, 0 },
	{ "usbphy", WPCM450_CLK_USBPHY, default_parents, ARRAY_SIZE(default_parents),
		parent_table, 6, 2, 0 },
	{ "uartsel", WPCM450_CLK_UARTSEL, default_parents, ARRAY_SIZE(default_parents),
		parent_table, 8, 2, 0 },
	{ "huartsel", WPCM450_CLK_HUARTSEL, huart_parents, ARRAY_SIZE(huart_parents),
		parent_table, 10, 1, 0 },
};

static const struct clk_div_table div_fixed2[] = {
	{ .val = 0, .div = 2 },
	{ }
};

struct wpcm450_clkdiv_data {
	const char *name;
	int index;
	int parent;
	int div_flags;
	const struct clk_div_table *table;
	int shift;
	int width;
	unsigned long flags;
};

static struct wpcm450_clkdiv_data clkdiv_data_early[] = {
	{ "ref/2", WPCM450_CLK_REFDIV2, WPCM450_CLK_REF, 0, div_fixed2, 0, 0 },
};

static const struct wpcm450_clkdiv_data clkdiv_data[] = {
	{ "cpu", WPCM450_CLK_CPU, WPCM450_CLK_CPUSEL, 0, div_fixed2, 0, 0, CLK_IS_CRITICAL },
	{ "adcdiv", WPCM450_CLK_ADCDIV, WPCM450_CLK_REF, CLK_DIVIDER_POWER_OF_TWO, NULL, 28, 2, 0 },
	{ "ahb", WPCM450_CLK_AHB, WPCM450_CLK_CPU, CLK_DIVIDER_POWER_OF_TWO, NULL, 24, 2, 0 },
	{ "apb", WPCM450_CLK_APB, WPCM450_CLK_AHB, CLK_DIVIDER_POWER_OF_TWO, NULL, 26, 2, 0 },
	{ "uart", WPCM450_CLK_UART, WPCM450_CLK_UARTSEL, 0, NULL, 16, 4, 0 },
	{ "ahb3", WPCM450_CLK_AHB3, WPCM450_CLK_AHB, CLK_DIVIDER_POWER_OF_TWO, NULL, 8, 2, 0 },
};

struct wpcm450_clken_data {
	const char *name;
	int index;
	int parent;
	unsigned long flags;
};

static const struct wpcm450_clken_data clken_data[] = {
	{ "fiu", WPCM450_CLK_FIU, WPCM450_CLK_AHB3, 0 },
	{ "xbus", WPCM450_CLK_XBUS, WPCM450_CLK_AHB3, 0 },
	{ "kcs", WPCM450_CLK_KCS, WPCM450_CLK_APB, 0 },
	{ "shm", WPCM450_CLK_SHM, WPCM450_CLK_AHB3, 0 },
	{ "usb1", WPCM450_CLK_USB1, WPCM450_CLK_AHB, 0 },
	{ "emc0", WPCM450_CLK_EMC0, WPCM450_CLK_AHB, 0 },
	{ "emc1", WPCM450_CLK_EMC1, WPCM450_CLK_AHB, 0 },
	{ "usb0", WPCM450_CLK_USB0, WPCM450_CLK_AHB, 0 },
	{ "peci", WPCM450_CLK_PECI, WPCM450_CLK_APB, 0 },
	{ "aes", WPCM450_CLK_AES, WPCM450_CLK_APB, 0 },
	{ "uart0", WPCM450_CLK_UART0, WPCM450_CLK_UART, 0 },
	{ "uart1", WPCM450_CLK_UART1, WPCM450_CLK_UART, 0 },
	{ "smb2", WPCM450_CLK_SMB2, WPCM450_CLK_APB, 0 },
	{ "smb3", WPCM450_CLK_SMB3, WPCM450_CLK_APB, 0 },
	{ "smb4", WPCM450_CLK_SMB4, WPCM450_CLK_APB, 0 },
	{ "smb5", WPCM450_CLK_SMB5, WPCM450_CLK_APB, 0 },
	{ "huart", WPCM450_CLK_HUART, WPCM450_CLK_HUARTSEL, 0 },
	{ "pwm", WPCM450_CLK_PWM, WPCM450_CLK_APB, 0 },
	{ "timer0", WPCM450_CLK_TIMER0, WPCM450_CLK_REFDIV2, CLK_IS_CRITICAL },
	{ "timer1", WPCM450_CLK_TIMER1, WPCM450_CLK_REFDIV2, CLK_IS_CRITICAL },
	{ "timer2", WPCM450_CLK_TIMER2, WPCM450_CLK_REFDIV2, 0 },
	{ "timer3", WPCM450_CLK_TIMER3, WPCM450_CLK_REFDIV2, 0 },
	{ "timer4", WPCM450_CLK_TIMER4, WPCM450_CLK_REFDIV2, 0 },
	{ "mft0", WPCM450_CLK_MFT0, WPCM450_CLK_APB, 0 },
	{ "mft1", WPCM450_CLK_MFT1, WPCM450_CLK_APB, 0 },
	{ "wdt", WPCM450_CLK_WDT, WPCM450_CLK_REFDIV2, 0 },
	{ "adc", WPCM450_CLK_ADC, WPCM450_CLK_ADCDIV, 0 },
	{ "sdio", WPCM450_CLK_SDIO, WPCM450_CLK_AHB, 0 },
	{ "sspi", WPCM450_CLK_SSPI, WPCM450_CLK_APB, 0 },
	{ "smb0", WPCM450_CLK_SMB0, WPCM450_CLK_APB, 0 },
	{ "smb1", WPCM450_CLK_SMB1, WPCM450_CLK_APB, 0 },
};

static DEFINE_SPINLOCK(wpcm450_clk_lock);

static int wpcm450_reset_probe(struct platform_device *pdev, void __iomem *clk_base)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	struct reset_simple_data *reset;

	reset = devm_kzalloc(dev, sizeof(*reset), GFP_KERNEL);
	if (!reset)
		return -ENOMEM;

	reset->rcdev.owner = THIS_MODULE;
	reset->rcdev.nr_resets = WPCM450_NUM_RESETS;
	reset->rcdev.ops = &reset_simple_ops;
	reset->rcdev.of_node = np;
	reset->membase = clk_base + REG_IPSRST;
	return devm_reset_controller_register(dev, &reset->rcdev);
}

static int wpcm450_clk_probe(struct platform_device *pdev)
{
	struct clk_hw_onecell_data *clk_data;
	struct clk_hw **hws;
	struct clk_hw *hw;
	void __iomem *clk_base;
	int i, ret;

	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	resource_size_t map_size;

	clk_base = devm_of_iomap(dev, np, 0, &map_size);
	if (IS_ERR(clk_base)) {
		dev_err(dev, "failed to map registers\n");
		return PTR_ERR(clk_base);
	}

	clk_data = devm_kzalloc(dev, struct_size(clk_data, hws, WPCM450_NUM_CLKS_TOTAL),
				GFP_KERNEL);
	if (!clk_data)
		return -ENOMEM;

	clk_data->num = WPCM450_NUM_CLKS;
	hws = clk_data->hws;

	for (i = 0; i < WPCM450_NUM_CLKS_TOTAL; i++)
		hws[i] = ERR_PTR(-ENOENT);

	/* PLLs */
	for (i = 0; i < ARRAY_SIZE(pll_data); i++) {
		const struct wpcm450_pll_data *data = &pll_data[i];
		struct clk_parent_data parent = index_to_parent_data(hws, data->parent);

		hw = wpcm450_clk_register_pll(dev, clk_base + data->reg, data->name,
					      &parent, data->flags);
		if (IS_ERR(hw)) {
			dev_err(dev, "Failed to register PLL: %pe\n", hw);
			return PTR_ERR(hw);
		}
		clk_data->hws[data->index] = hw;
	}

	/* Early divisors (REF/2) */
	for (i = 0; i < ARRAY_SIZE(clkdiv_data_early); i++) {
		const struct wpcm450_clkdiv_data *data = &clkdiv_data_early[i];
		struct clk_parent_data parent = index_to_parent_data(hws, data->parent);

		hw = devm_clk_hw_register_divider_table_parent_data(dev, data->name, &parent,
								    data->flags,
								    clk_base + REG_CLKDIV,
								    data->shift, data->width,
								    data->div_flags, data->table,
								    &wpcm450_clk_lock);

		if (IS_ERR(hw)) {
			dev_err(dev, "Failed to register div table: %pe\n", hw);
			return PTR_ERR(hw);
		}
		clk_data->hws[data->index] = hw;
	}

	/* Selects/muxes */
	for (i = 0; i < ARRAY_SIZE(clksel_data); i++) {
		const struct wpcm450_clksel_data *data = &clksel_data[i];
		struct clk_parent_data parents[4];
		size_t num_parents = index_to_parent_data_array(hws,
				data->parents, data->num_parents, parents,
				ARRAY_SIZE(parents));

		hw = devm_clk_hw_register_mux_parent_data_table(dev, data->name, parents,
								num_parents, data->flags,
								clk_base + REG_CLKSEL, data->shift,
								data->width, 0, NULL,
								&wpcm450_clk_lock);

		if (IS_ERR(hw)) {
			dev_err(dev, "Failed to register mux: %pe\n", hw);
			return PTR_ERR(hw);
		}
		clk_data->hws[data->index] = hw;
	}

	/* Divisors */
	for (i = 0; i < ARRAY_SIZE(clkdiv_data); i++) {
		const struct wpcm450_clkdiv_data *data = &clkdiv_data[i];
		struct clk_parent_data parent = index_to_parent_data(hws, data->parent);

		hw = devm_clk_hw_register_divider_table_parent_data(dev, data->name, &parent,
								    data->flags,
								    clk_base + REG_CLKDIV,
								    data->shift, data->width,
								    data->div_flags, data->table,
								    &wpcm450_clk_lock);

		if (IS_ERR(hw)) {
			dev_err(dev, "Failed to register divider: %pe\n", hw);
			return PTR_ERR(hw);
		}
		clk_data->hws[data->index] = hw;
	}

	/* Enables/gates */
	for (i = 0; i < ARRAY_SIZE(clken_data); i++) {
		const struct wpcm450_clken_data *data = &clken_data[i];
		struct clk_parent_data parent = index_to_parent_data(hws, data->parent);

		hw = devm_clk_hw_register_gate_parent_data(dev, data->name, &parent, data->flags,
							   clk_base + REG_CLKEN, data->index,
							   data->flags, &wpcm450_clk_lock);
		if (IS_ERR(hw)) {
			dev_err(dev, "Failed to register gate: %pe\n", hw);
			return PTR_ERR(hw);
		}
		clk_data->hws[data->index] = hw;
	}

	ret = devm_of_clk_add_hw_provider(dev, of_clk_hw_onecell_get, clk_data);
	if (ret)
		dev_err(dev, "Failed to add DT provider: %pe\n", ERR_PTR(ret));

	ret = wpcm450_reset_probe(pdev, clk_base);
	if (ret)
		dev_err(dev, "Failed to register reset controller: %pe\n", ERR_PTR(ret));

	return 0;
}

static const struct of_device_id wpcm450_of_match[] = {
	{ .compatible = "nuvoton,wpcm450-clk" },
	{}
};
MODULE_DEVICE_TABLE(of, wpcm450_of_match);

static struct platform_driver wpcm450_clk_driver = {
	.driver = {
		.name = "wpcm450-clk",
		.of_match_table = wpcm450_of_match,
	},
	.probe = wpcm450_clk_probe,
};
module_platform_driver(wpcm450_clk_driver);

MODULE_AUTHOR("Jonathan Neuschäfer <j.neuschaefer@gmx.net>");
MODULE_DESCRIPTION("Nuvoton WPCM450 clock and reset controller driver");
MODULE_LICENSE("GPL");
