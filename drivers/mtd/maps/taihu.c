/*
 *
 * drivers/mtd/maps/taihu.c
 *
 * FLASH map for the AMCC Taihu boards.
 *
 * 2005 UDTech, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>

#include <asm/io.h>

#define BOOTWINDOW_ADDR 0xffe00000
#define BOOTWINDOW_SIZE 0x00200000

#define APPWINDOW_ADDR 	0xfc000000
#define APPWINDOW_SIZE 	0x02000000


static struct mtd_partition taihu_bootflash_partitions[] = {
        {
		.name = "kozio diags",
		.offset = 0,
		.size = 0x001a0000,
		.mask_flags = MTD_WRITEABLE      /* force read-only */
	},
	{
		.name = "u-boot env",
		.offset = 0x001a0000,
		.size = 0x00020000
	},
	{
		.name = "u-boot",
		.offset = 0x001c0000,
		.size = 0x00040000,
		.mask_flags = MTD_WRITEABLE     /* force read-only */
	}
};

struct map_info taihu_bootflash_map = {
	.name = "AMCC Taihu Boot Flash",
	.size = BOOTWINDOW_SIZE,
	.bankwidth = 2,
	.phys = BOOTWINDOW_ADDR,
};

static struct mtd_partition taihu_appflash_partitions[] = {
	{
		.name = "kernel",
		.offset = 0,
		.size = 0x00300000,
		.mask_flags = MTD_WRITEABLE	/* force read-only */
	},
	{
		.name = "initrd",
		.offset = 0x00300000,
		.size = 0x01a00000,
		.mask_flags = MTD_WRITEABLE    /* force read-only */
	},
	{
		.name = "jffs2",
		.offset = 0x01D00000,
		.size = 0x00300000
	}
};


struct map_info taihu_appflash_map = {
	.name = "AMCC Taihu Application Flash",
	.size = APPWINDOW_SIZE,
	.bankwidth = 2,
	.phys = APPWINDOW_ADDR,
};


#define NUM_TAIHU_FLASH_PARTITIONS(parts)	\
	(sizeof(parts)/sizeof(parts[0]))

static struct mtd_info *taihu_mtd;

int __init init_taihu_flash(void)
{

	printk(KERN_NOTICE "taihu: bootflash mapping: %x at %x\n",
	       BOOTWINDOW_SIZE, BOOTWINDOW_ADDR);
	taihu_bootflash_map.virt = ioremap(BOOTWINDOW_ADDR, BOOTWINDOW_SIZE);
	if (!taihu_bootflash_map.virt) {
		printk("init_taihu_flash: failed to ioremap for bootflash\n");
		return -EIO;
	}
	simple_map_init(&taihu_bootflash_map);
	taihu_mtd = do_map_probe("cfi_probe", &taihu_bootflash_map);
	if (taihu_mtd) {
		taihu_mtd->owner = THIS_MODULE;
		add_mtd_partitions(taihu_mtd,
				   taihu_bootflash_partitions,
				   ARRAY_SIZE(taihu_bootflash_partitions));
	} else {
		printk("map probe failed (bootflash)\n");
		return -ENXIO;
	}

	printk(KERN_NOTICE "taihu: appflash mapping: %x at %x\n",
	       APPWINDOW_SIZE, APPWINDOW_ADDR);
	taihu_appflash_map.virt = ioremap(APPWINDOW_ADDR, APPWINDOW_SIZE);
	if (!taihu_appflash_map.virt) {
		printk("init_taihu_flash: failed to ioremap for appflash\n");
		return -EIO;
	}
	simple_map_init(&taihu_appflash_map);
	taihu_mtd = do_map_probe("cfi_probe", &taihu_appflash_map);
	if (taihu_mtd) {
		taihu_mtd->owner = THIS_MODULE;
		add_mtd_partitions(taihu_mtd,
				   taihu_appflash_partitions,
				   ARRAY_SIZE(taihu_appflash_partitions));
	} else {
		printk("map probe failed (appflash)\n");
		return -ENXIO;
	}

	return 0;
}

static void __exit cleanup_taihu_flash(void)
{
	if (taihu_mtd) {
		del_mtd_partitions(taihu_mtd);
		/* moved iounmap after map_destroy - armin */
		map_destroy(taihu_mtd);
	}

	if (taihu_bootflash_map.virt)
		iounmap((void *)taihu_bootflash_map.virt);
	if (taihu_appflash_map.virt)
		iounmap((void *)taihu_appflash_map.virt);
}

module_init(init_taihu_flash);
module_exit(cleanup_taihu_flash);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MTD map driver for the AMCC Taihu board");
