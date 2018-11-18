/* 2012-04-12: File added and changed by Sony Corporation */
/*
 * Copyright (C) 2010 NVIDIA, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307, USA
 */


#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/device.h>

#include <mach/clk.h>
#include <mach/iomap.h>
#include <mach/irqs.h>
#include <mach/pinmux.h>
#include <mach/iomap.h>
#include <mach/io.h>
#include <mach/kbc.h>

#include <asm/mach-types.h>
#include <asm/mach/arch.h>

#include "devices.h"

#define NBX02_ROW_COUNT		3
#define NBX02_COL_COUNT		6

static const u32 nbx02_keymap[] = {
	KEY(0, 4, KEY_VOLUMEUP),
	KEY(0, 5, KEY_VOLUMEDOWN),
};

static const struct matrix_keymap_data nbx02_keymap_data = {
	.keymap = nbx02_keymap,
	.keymap_size = ARRAY_SIZE(nbx02_keymap),
};

static struct tegra_kbc_platform_data nbx02_kbc_platform_data = {
	.debounce_cnt = 20,
	.repeat_cnt = 50 * 32,
	.keymap_data = &nbx02_keymap_data,
	.use_fn_map = false,
	.wakeup = false,
};


static struct resource nbx02_kbc_resources[] = {
	[0] = {
		.start = TEGRA_KBC_BASE,
		.end   = TEGRA_KBC_BASE + TEGRA_KBC_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = INT_KBC,
		.end   = INT_KBC,
		.flags = IORESOURCE_IRQ,
	},
};


#include <linux/delay.h>
#include <linux/io.h>

static void tegra_assert_kbc_reset(int r)
{

	void __iomem *reset = IO_ADDRESS(TEGRA_PMC_BASE + 0x00);
	u32 reg;

	reg = readl(reset);

	if ( r ) {
		reg |= 0x08;
	} else {
		reg &= ~0x08;
	}
	writel(reg, reset);
}

struct platform_device nbx02_kbc_device = {
	.name = "tegra-kbc",
	.id = -1,
	.dev = {
		.platform_data = &nbx02_kbc_platform_data,
	},
	.resource = nbx02_kbc_resources,
	.num_resources = ARRAY_SIZE(nbx02_kbc_resources),
};

int __init nbx02_kbc_init(void)
{
	struct tegra_kbc_platform_data *data = &nbx02_kbc_platform_data;
	int i;

	pr_info("KBC: nbx02_kbc_init\n");

	tegra_assert_kbc_reset(1);
	udelay(100);
	tegra_assert_kbc_reset(0);

	for (i = 0; i < NBX02_ROW_COUNT; i++) {
		data->pin_cfg[i].num = i;
		data->pin_cfg[i].is_row = true;
		data->pin_cfg[i].en = true;
	}
	for (i = 0; i < NBX02_COL_COUNT; i++) {
		data->pin_cfg[i + KBC_MAX_ROW].num = i;
		data->pin_cfg[i + KBC_MAX_ROW].en = true;
	}

	platform_device_register(&nbx02_kbc_device);
	return 0;
}



