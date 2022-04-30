/*
 * $Id: $
 *
 * drivers/mtd/maps/p3p440.c
 *
 * Mapping for Prodrive P3P440 flash
 *
 * Copyright (c) 2005 DENX Software Engineering
 * Stefan Roese <sr@denx.de>
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

#define RW_PART0_OF	0
#define RW_PART0_SZ	0x180000
#define RW_PART1_OF	RW_PART0_OF + RW_PART0_SZ
#define RW_PART1_SZ	0x280000
#define RW_PART2_OF	RW_PART1_OF + RW_PART1_SZ
/* Partition 2 will be autosized dynamically... */
#define RW_PART3_SZ	0x40000
#define RW_PART4_SZ	0x40000

static struct mtd_partition p3p440_flash_partitions[] = {
	{
		.name = "kernel",
		.offset = RW_PART0_OF,
		.size = RW_PART0_SZ
	},
	{
		.name = "root",
		.offset = RW_PART1_OF,
		.size = RW_PART1_SZ,
	},
	{
		.name = "user",
		.offset = RW_PART2_OF,
/*		.size = RW_PART2_SZ */ /* will be adjusted dynamically */
	},
	{
		.name = "env",
/*		.offset = RW_PART3_OF, */ /* will be adjusted dynamically */
		.size = RW_PART3_SZ,
	},
	{
		.name = "u-boot",
/*		.offset = RW_PART4_OF, */ /* will be adjusted dynamically */
		.size = RW_PART4_SZ,
	}
};

struct map_info p3p440_flash_map = {
	.name = "p3p440-flash",
	.bankwidth = 2,
};

static struct mtd_info *p3p440_mtd;

int __init init_p3p440_flash(void)
{
	unsigned long long flash_base;
	unsigned long flash_size;

	flash_base = __res.bi_flashstart | 0x0000000100000000LL;
	flash_size = __res.bi_flashsize;

	p3p440_flash_map.size = flash_size;
	p3p440_flash_map.phys = flash_base;
	p3p440_flash_map.virt = ioremap64(flash_base,
					  p3p440_flash_map.size);


	if (!p3p440_flash_map.virt) {
		printk("init_p3p440_flash: failed to ioremap\n");
		return -EIO;
	}

	/*
	 * Adjust partitions to flash size
	 */
	p3p440_flash_partitions[2].size = p3p440_flash_map.size -
		RW_PART0_SZ - RW_PART1_SZ - RW_PART3_SZ - RW_PART4_SZ;
	p3p440_flash_partitions[3].offset = p3p440_flash_partitions[2].size +
		RW_PART2_OF;
	p3p440_flash_partitions[4].offset = p3p440_flash_partitions[3].size +
		p3p440_flash_partitions[3].offset;

	simple_map_init(&p3p440_flash_map);

	p3p440_mtd = do_map_probe("cfi_probe", &p3p440_flash_map);

	if (p3p440_mtd) {
		p3p440_mtd->owner = THIS_MODULE;
		return add_mtd_partitions(p3p440_mtd,
					  p3p440_flash_partitions,
					  ARRAY_SIZE(p3p440_flash_partitions));
	}

	return -ENXIO;
}

static void __exit cleanup_p3p440_flash(void)
{
	if (p3p440_mtd) {
		del_mtd_partitions(p3p440_mtd);
		/* moved iounmap after map_destroy - armin */
		map_destroy(p3p440_mtd);
		iounmap((void *)p3p440_flash_map.virt);
	}
}

module_init(init_p3p440_flash);
module_exit(cleanup_p3p440_flash);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stefan Roese <sr@denx.de>");
MODULE_DESCRIPTION("MTD map and partitions for Prodrive P3P440 board");
