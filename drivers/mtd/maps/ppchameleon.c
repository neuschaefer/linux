/*
 * drivers/mtd/maps/ppchameleon.c
 *
 * Mapping for DAVE PPChamelonEVB flash
 *
 * Maintained by Wolfgang Denk, <wd@denx.de>
 *
 * Copyright (c) 2005 DENX Software Engineering
 * Wolfgang Denk <wd@denx.de>
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
#include <linux/version.h>
#include <asm/io.h>
#include <asm/ibm4xx.h>
#include <asm/ppcboot.h>

extern bd_t __res;

static struct mtd_info *flash;

static struct map_info ppchameleon_map = {
	.name =		"PPChameleon",
	.bankwidth =	2,
};

static struct mtd_partition ppchameleon_partitions[] = {
    {
	name:	"linux",			/* Linux kernel image */
	offset:	0x00000000,
	size:	0x00180000,
/*	mask_flags: MTD_WRITEABLE,		/ * force read-only */
    },
    {
	name:	"user",				/* unassigned */
	offset:	0x00180000,
	size:	0x00240000,
    },
    {
	name:	"u-boot",			/* U-Boot Firmware */
	offset:	0x003C0000,
	size:	0x00040000,			/* 256 KB */
/*	mask_flags: MTD_WRITEABLE,		/ * force read-only */
    },
};

int __init init_ppchameleon(void)
{
	unsigned long flash_base, flash_size;

	flash_base = __res.bi_flashstart;
	flash_size = __res.bi_flashsize;

	ppchameleon_map.size = flash_size;
	ppchameleon_map.phys = flash_base;
	ppchameleon_map.virt =
		(void __iomem *)ioremap(flash_base, ppchameleon_map.size);

	if (!ppchameleon_map.virt) {
		printk("Failed to ioremap flash.\n");
		return -EIO;
	}

	simple_map_init(&ppchameleon_map);

	flash = do_map_probe("cfi_probe", &ppchameleon_map);
	if (flash) {
		flash->owner = THIS_MODULE;
		add_mtd_partitions(flash, ppchameleon_partitions,
					ARRAY_SIZE(ppchameleon_partitions));
	} else {
		printk("map probe failed for flash\n");
		return -ENXIO;
	}

	return 0;
}

static void __exit cleanup_ppchameleon(void)
{
	if (flash) {
		del_mtd_partitions(flash);
		map_destroy(flash);
	}

	if (ppchameleon_map.virt) {
		iounmap((void *)ppchameleon_map.virt);
		ppchameleon_map.virt = 0;
	}
}

module_init(init_ppchameleon);
module_exit(cleanup_ppchameleon);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wolfgang Denk <wd@denx.de>");
MODULE_DESCRIPTION("MTD map and partitions for DAVE PPChameleonEVB boards");
