/*
 * $Id: $
 *
 * drivers/mtd/maps/alpr.c
 *
 * Mapping for ALPR flash.
 *
 * Copyright (c) 2006 DENX Software Engineering
 * Heiko Schocher <hs@denx.de>
 *
 * Based on original work by
 *      Matt Porter <mporter@kernel.crashing.org>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>

#include <asm/io.h>
#include <asm/ibm4xx.h>
#include <asm/ppcboot.h>

extern bd_t __res;

#define RW_PART0_SZ	0x1a0000
#define RW_PART1_SZ	0x10000
#define RW_PART2_SZ	0x10000
#define RW_PART3_SZ	0x40000

static struct mtd_partition alpr_flash_partitions[] = {
	{
		.name   = "fpga",
		.offset = 0,
		.size   = RW_PART0_SZ
	},
	{
		.name   = "env0",
                .offset = MTDPART_OFS_NXTBLK,
		.size   = RW_PART1_SZ,
	},
	{
		.name   = "env1",
                .offset = MTDPART_OFS_NXTBLK,
		.size   = RW_PART2_SZ,
	},
	{
		.name   = "u-boot",
                .offset = MTDPART_OFS_NXTBLK,
                .size   = MTDPART_SIZ_FULL,
	}
};

struct map_info alpr_flash_map = {
	.name = "ALPR-flash",
	.bankwidth = 2,
};

static struct mtd_info *alpr_mtd;

int __init init_alpr_flash(void)
{
	unsigned long flash_base, flash_size;

	flash_base = __res.bi_flashstart;
	flash_size = __res.bi_flashsize;

	alpr_flash_map.size = flash_size;
	alpr_flash_map.phys = flash_base;
	alpr_flash_map.virt =
		(void __iomem *)ioremap64(ALPR_FLASH_ADDR, alpr_flash_map.size);

	if (!alpr_flash_map.virt) {
		printk("%s: failed to ioremap\n", __FUNCTION__);
		return -EIO;
	}

	simple_map_init(&alpr_flash_map);

	alpr_mtd = do_map_probe("cfi_probe", &alpr_flash_map);

	if (alpr_mtd) {
		alpr_mtd->owner = THIS_MODULE;
		return add_mtd_partitions(alpr_mtd,
				alpr_flash_partitions,
				ARRAY_SIZE(alpr_flash_partitions));
	}

	return -ENXIO;
}

static void __exit cleanup_alpr_flash(void)
{
	if (alpr_mtd) {
		del_mtd_partitions(alpr_mtd);
		/* moved iounmap after map_destroy - armin */
		map_destroy(alpr_mtd);
		iounmap((void *)alpr_flash_map.virt);
	}
}

module_init(init_alpr_flash);
module_exit(cleanup_alpr_flash);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Heiko Schocher <hs@denx.de>");
MODULE_DESCRIPTION("MTD map and partitions for the Prodrive ALPR board.");
