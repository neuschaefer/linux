/*
 * Mapping for Bamboo user flash
 *
 * Wade Farnsworth <wfarnsworth@mvista.com>
 *
 * Copyright 2005 MontaVista Software Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>
#include <asm/io.h>
#include <asm/ibm44x.h>
#include <platforms/4xx/bamboo.h>

static struct mtd_info *small_flash, *large_flash, *sram;

static struct map_info bamboo_small_map = {
	.name = "Small-flash",
	.size = BAMBOO_SMALL_FLASH_SIZE,
	.bankwidth = 1,
};

static struct map_info bamboo_large_map = {
	.name = "Large-flash",
	.size = BAMBOO_LARGE_FLASH_SIZE,
	.bankwidth = 2,
};

static struct map_info bamboo_sram_map = {
	.name = "SRAM",
	.size = BAMBOO_SRAM_SIZE,
	.bankwidth = 2,
};

#ifdef CONFIG_MTD_UBOOT_PARTITIONS
static struct mtd_partition bamboo_small_partitions[] = {
	{
		.name =   "reserved",
		.offset = 0,
		.size =   0x60000,
	},
	{
		.name =   "env",
		.offset = 0x60000,
		.size =   0x20000,
	},
	{
		.name =   "u-boot",
		.offset = 0x80000,
		.size =   0x80000,
		/*.mask_flags = MTD_WRITEABLE, */ /* force read-only */
	}
};

static struct mtd_partition bamboo_large_partitions[] = {
	{
		.name =   "kernel",
		.offset = 0,
		.size =   0x180000,
	},
	{
		.name =   "root",
		.offset = 0x180000,
		.size =   0x280000,
	}
};
#else /* CONFIG_MTD_UBOOT_PARTITIONS */
static struct mtd_partition bamboo_small_partitions[] = {
	{
		.name = "pibs",
		.offset = 0x0,
		.size = 0x100000,
		.mask_flags = MTD_WRITEABLE,
	}
};

static struct mtd_partition bamboo_large_partitions[] = {
	{
	 	.name = "filesystem",
	 	.offset = 0x0,
	 	.size = 0x400000,
	}
};
#endif /* CONFIG_MTD_UBOOT_PARTITIONS */

static struct mtd_partition bamboo_sram_partitions[] = {
	{
	 	.name = "sram",
	 	.offset = 0x0,
	 	.size = 0x100000,
	}
};

int __init
init_bamboo(void)
{
	u8 setting_reg;
	u8 *setting_adr;
	unsigned long small_flash_base, large_flash_base, sram_base;
	unsigned long *gpio_base;

	setting_adr = ioremap64(BAMBOO_FPGA_SETTING_REG_ADDR, 8);
	if (!setting_adr)
		return -ENOMEM;
	setting_reg = readb(setting_adr);
	iounmap(setting_adr);

	/*
	 * Some versions of PIBS don't set up the GPIO controller
	 * for the devices on chip select 4 (large flash and sram).
	 */
	gpio_base = ioremap64(0x0EF600B00ULL, 0x80);
	if (!gpio_base) {
		printk("Failed to ioremap GPIO\n");
		return -ENOMEM;
	}
	* (gpio_base + 0x02) |= 0x00001000;
	* (gpio_base + 0x04) |= 0x00001000;
	iounmap((void *) gpio_base);

	/*
	 * Use the values in the FPGA Setting Register to determine where
	 * each flash bank is located.
	 */
	if (!BAMBOO_BOOT_NAND_FLASH(setting_reg)) {
		if (BAMBOO_BOOT_SMALL_FLASH(setting_reg)) {
			small_flash_base = BAMBOO_SMALL_FLASH_HIGH;
		} else {
			small_flash_base = BAMBOO_SMALL_FLASH_LOW;
		}

		bamboo_small_map.phys = small_flash_base;
		bamboo_small_map.virt =
			(ulong *) ioremap64(small_flash_base,
					    bamboo_small_map.size);
		if (!bamboo_small_map.virt) {
			printk("Failed to ioremap flash\n");
			return -EIO;
		}

		simple_map_init(&bamboo_small_map);

		small_flash = do_map_probe("jedec_probe", &bamboo_small_map);
		if (small_flash) {
			small_flash->owner = THIS_MODULE;
			add_mtd_partitions(small_flash, bamboo_small_partitions,
					   ARRAY_SIZE(bamboo_small_partitions));
		} else {
			printk(KERN_INFO
			       "small flash disabled: Probe failed due to probable hardware issue\n");
			iounmap((void *) bamboo_small_map.virt);
			bamboo_small_map.virt = 0;
		}
	} else
		bamboo_small_map.virt = 0;

	/*
	 * Wiring to the large flash on the Rev 0 Bamboo is incorrect, so
	 * this should fail.
	 *
	 * This has been fixed on the Rev 1.
	 */
	if (BAMBOO_BOOT_NAND_FLASH(setting_reg) || BAMBOO_BOOT_SMALL_FLASH(setting_reg))
		if (BAMBOO_LARGE_FLASH_EN(setting_reg))
			large_flash_base = BAMBOO_LARGE_FLASH_LOW1;
		else
			large_flash_base = BAMBOO_LARGE_FLASH_LOW2;
	else if (BAMBOO_LARGE_FLASH_EN(setting_reg))
		large_flash_base = BAMBOO_LARGE_FLASH_HIGH1;
	else
		large_flash_base = BAMBOO_LARGE_FLASH_HIGH2;
	bamboo_large_map.phys = large_flash_base;
	bamboo_large_map.virt = (ulong *) ioremap64(large_flash_base, bamboo_large_map.size);
	if (!bamboo_large_map.virt) {
		printk("Failed to ioremap flash\n");
		return -EIO;
	}

	simple_map_init(&bamboo_large_map);
	large_flash = do_map_probe("cfi_probe", &bamboo_large_map);
	if (large_flash) {
		large_flash->owner = THIS_MODULE;
		add_mtd_partitions(large_flash, bamboo_large_partitions,
				   ARRAY_SIZE(bamboo_large_partitions));
	} else {
		printk(KERN_INFO
		       "large flash disabled: Probe failed due to probable hardware issue\n");
		iounmap((void *) bamboo_large_map.virt);
		bamboo_large_map.virt = 0;
	}

	if (BAMBOO_BOOT_NAND_FLASH(setting_reg) || BAMBOO_BOOT_SMALL_FLASH(setting_reg))
		if (BAMBOO_LARGE_FLASH_EN(setting_reg))
			sram_base = BAMBOO_SRAM_LOW1;
		else
			sram_base = BAMBOO_SRAM_LOW2;
	else if (BAMBOO_LARGE_FLASH_EN(setting_reg))
		sram_base = BAMBOO_SRAM_HIGH2;
	else
		sram_base = BAMBOO_SRAM_HIGH1;

	bamboo_sram_map.phys = sram_base;
	bamboo_sram_map.virt = (ulong *) ioremap64(sram_base, bamboo_sram_map.size);
	if (!bamboo_sram_map.virt) {
		printk("Failed to ioremap flash \n");
		return -EIO;
	}

	simple_map_init(&bamboo_sram_map);

	sram = do_map_probe("map_ram", &bamboo_sram_map);
	if (sram) {
		sram->owner = THIS_MODULE;
		sram->erasesize = 0x10;
		add_mtd_partitions(sram, bamboo_sram_partitions,
				   ARRAY_SIZE(bamboo_sram_partitions));
	} else {
		printk(KERN_INFO
		       "sram disabled: Probe failed due to probable hardware issue\n");
		iounmap((void *) bamboo_sram_map.virt);
		bamboo_sram_map.virt = 0;
	}

	if (!(small_flash || large_flash || sram))
		return -ENXIO;

	return 0;
}

static void __exit
cleanup_bamboo(void)
{
	if (small_flash) {
		del_mtd_partitions(small_flash);
		map_destroy(small_flash);
	}

	if (large_flash) {
		del_mtd_partitions(large_flash);
		map_destroy(large_flash);
	}

	if (sram) {
		del_mtd_partitions(sram);
		map_destroy(sram);
	}

	if (bamboo_small_map.virt) {
		iounmap((void *) bamboo_small_map.virt);
		bamboo_small_map.virt = 0;
	}

	if (bamboo_large_map.virt) {
		iounmap((void *) bamboo_large_map.virt);
		bamboo_large_map.virt = 0;
	}

	if (bamboo_sram_map.virt) {
		iounmap((void *) bamboo_sram_map.virt);
		bamboo_sram_map.virt = 0;
	}
}

module_init(init_bamboo);
module_exit(cleanup_bamboo);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wade Farnsworth <wfarnsworth@mvista.com>");
MODULE_DESCRIPTION("MTD map and partitions for IBM 440EP Bamboo boards");
