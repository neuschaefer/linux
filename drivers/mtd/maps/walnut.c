/*
 * $Id: walnut.c,v 1.3 2005/11/07 11:14:29 gleixner Exp $
 *
 * drivers/mtd/maps/walnut.c
 *
 * Mapping for Walnut (405GP), Sycamore (405GPr) and Bubinga (405EP) flash
 *
 * Copyright (c) 2005 DENX Software Engineering
 * Stefan Roese <sr@denx.de>
 *
 * Based on original work by
 * 	Heikki Lindholm <holindho@infradead.org>
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

static struct mtd_info *flash;

static struct map_info walnut_map = {
	.name =		"PPC40x-flash",
	.size =		PPC40x_FLASH_SIZE,
	.bankwidth =	1,
};

#ifdef CONFIG_MTD_UBOOT_PARTITIONS
static struct mtd_partition walnut_partitions[] = {
	{
		.name =   "reserved",
		.offset = 0,
		.size =   0x20000,
	},
	{
		.name =   "env",
		.offset = 0x20000,
		.size =   0x20000,
	},
	{
		.name =   "u-boot",
		.offset = 0x40000,
		.size =   0x40000,
		/*.mask_flags = MTD_WRITEABLE, */ /* force read-only */
	}
};
#else /* CONFIG_MTD_UBOOT_PARTITIONS */
/* Actually, OpenBIOS is the last 128 KiB of the flash - better
 * partitioning could be made */
static struct mtd_partition walnut_partitions[] = {
	{
		.name =   "OpenBIOS",
		.offset = 0x0,
		.size =   PPC40x_FLASH_SIZE,
		/*.mask_flags = MTD_WRITEABLE, */ /* force read-only */
	}
};
#endif /* CONFIG_MTD_UBOOT_PARTITIONS */

int __init init_walnut(void)
{
	u8 fpga_brds1;
	void *fpga_brds1_adr;
	void *fpga_status_adr;
	unsigned long flash_base;

	/* this should already be mapped (platform/4xx/walnut.c) */
	fpga_status_adr = ioremap(PPC40x_FPGA_BASE, 8);
	if (!fpga_status_adr)
		return -ENOMEM;

	fpga_brds1_adr = fpga_status_adr + PPC40x_FPGA_REG_OFFS;
	fpga_brds1 = readb(fpga_brds1_adr);
	/* iounmap(fpga_status_adr); */

	if (PPC40x_FLASH_ONBD_N(fpga_brds1)) {
		printk("The on-board flash is disabled (U79 sw 5)!");
		iounmap(fpga_status_adr);
		return -EIO;
	}
	if (PPC40x_FLASH_SRAM_SEL(fpga_brds1))
		flash_base = PPC40x_FLASH_LOW;
	else
		flash_base = PPC40x_FLASH_HIGH;

	walnut_map.phys = flash_base;
	walnut_map.virt =
		(void __iomem *)ioremap(flash_base, walnut_map.size);

	if (!walnut_map.virt) {
		printk("Failed to ioremap flash.\n");
		iounmap(fpga_status_adr);
		return -EIO;
	}

	simple_map_init(&walnut_map);

	flash = do_map_probe("jedec_probe", &walnut_map);
	if (flash) {
		flash->owner = THIS_MODULE;
		add_mtd_partitions(flash, walnut_partitions,
					ARRAY_SIZE(walnut_partitions));
	} else {
		printk("map probe failed for flash\n");
		iounmap(fpga_status_adr);
		return -ENXIO;
	}

	iounmap(fpga_status_adr);
	return 0;
}

static void __exit cleanup_walnut(void)
{
	if (flash) {
		del_mtd_partitions(flash);
		map_destroy(flash);
	}

	if (walnut_map.virt) {
		iounmap((void *)walnut_map.virt);
		walnut_map.virt = 0;
	}
}

module_init(init_walnut);
module_exit(cleanup_walnut);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stefan Roese <sr@denx.de>");
MODULE_DESCRIPTION("MTD map and partitions for IBM 405GP/r/EP boards");
