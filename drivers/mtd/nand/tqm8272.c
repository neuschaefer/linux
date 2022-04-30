/*
 * drivers/mtd/nand/tqm8272.c
 *
 * Copyright (C) 2006 DENX Software Engineering Heiko Schocher <hs@denx.de>
 *
 * Copyright (C) 2004 Technologic Systems (support@embeddedARM.com)
 *
 * Derived from drivers/mtd/nand/edb7312.c
 *   Copyright (C) 2004 Marius Gröger (mag@sysgo.de)
 *
 * Derived from drivers/mtd/nand/autcpu12.c
 *   Copyright (c) 2001 Thomas Gleixner (gleixner@autronix.de)
 *
 * $Id: ts7250.c,v 1.4 2004/12/30 22:02:07 joff Exp $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Overview:
 *   This is a device driver for the NAND flash device found on the
 *   TQM8272 Board.
 */

#include <linux/slab.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/mpc8260.h>
#include <asm/cpm2.h>

/*
 * MTD structure for TQM8272 board
 */
static struct mtd_info *tqm8272_mtd = NULL;

#ifdef CONFIG_MTD_PARTITIONS
static const char *part_probes[] = { "cmdlinepart", NULL };

#define NUM_PARTITIONS 3

/*
 * Define static partitions for flash device
 */
static struct mtd_partition partition_info128[] = {
	{
		.name		= "U-Boot",
		.offset		= 0x00000000,
		.size		= 0x000040000,
	}, {
		.name		= "Env",
		.offset		= 0x000040000,
		.size		= 0x000040000,
	}, {
		.name		= "Linux",
		.offset		= 0x000080000,
		.size		= 0x000200000,
	},
};
#endif

#define CS_OFF	0x80
static int	cs = 0;

static void tqm8272_select_chip(struct mtd_info *mtd, int chipnr)
{
	struct nand_chip *chip = mtd->priv;
	cs = chipnr;
}

static void tqm8272_hwcontrol(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	struct nand_chip *chip = mtd->priv;

	if (ctrl & NAND_ALE) {
		writeb (cmd, (unsigned char *)((chip->IO_ADDR_W + cs * CS_OFF) + 0x40));
	} else if (ctrl & NAND_CLE) {
		writeb (cmd, (unsigned char *)((chip->IO_ADDR_W + cs * CS_OFF) + 0x20));
	} else {
	}
}

/*
 *	read device ready pin
 */
static int tqm8272_device_ready(struct mtd_info *mtd)
{
	udelay(500);
	return 1;
}

static uint8_t tqm8272_read_byte(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd->priv;

	return readb((unsigned char *)(chip->IO_ADDR_R + cs * CS_OFF));
}

static void tqm8272_write_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	struct nand_chip *chip = mtd->priv;
	int	i;

	for (i = 0; i < len; i++) {
		writeb(buf[i], (unsigned char *)(chip->IO_ADDR_W + cs * CS_OFF));
	}

}

static void tqm8272_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
	struct nand_chip *chip = mtd->priv;
	int	i;

	for (i = 0; i < len; i++)
		buf[i] = readb((unsigned char *)(chip->IO_ADDR_R + cs * CS_OFF));
}

static int tqm8272_verify_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	struct nand_chip *chip = mtd->priv;
	int	i;
	unsigned char tmp;

	for (i = 0; i < len; i++) {
		if (buf[i] != readb((unsigned char *)(chip->IO_ADDR_R + cs * CS_OFF)))
			return -EFAULT;
	}
	return 0;
}
/*
 * Main initialization routine
 */
static int __init tqm8272_init(void)
{
	struct nand_chip *this;
	const char *part_type = 0;
	int mtd_parts_nb = 0;
	struct mtd_partition *mtd_parts = 0;
	struct  mem_ctlr	*memctl = &cpm2_immr->im_memctl;

	/* are there NANDs ? */
	if ((memctl->memc_br3 & 0x01) != 0x01) {
		printk("No NANDs on TQM8272\n");
		return -ENOMEM;
	}
	/* Allocate memory for MTD device structure and private data */
	tqm8272_mtd = kmalloc(sizeof(struct mtd_info) + sizeof(struct nand_chip), GFP_KERNEL);
	if (!tqm8272_mtd) {
		printk("Unable to allocate TQM8272 NAND MTD device structure.\n");
		return -ENOMEM;
	}

	/* Get pointer to private data */
	this = (struct nand_chip *)(&tqm8272_mtd[1]);

	/* Initialize structures */
	memset(tqm8272_mtd, 0, sizeof(struct mtd_info));
	memset(this, 0, sizeof(struct nand_chip));

	/* Link the private data with the MTD structure */
	tqm8272_mtd->priv = this;
	tqm8272_mtd->owner = THIS_MODULE;

	/* insert callbacks */
	this->IO_ADDR_R = (void *) ioremap (CFG_NAND_BASE, 0x1000);	/* ioremap */
	this->IO_ADDR_W = this->IO_ADDR_R;
	this->cmd_ctrl = tqm8272_hwcontrol;
	this->read_byte = tqm8272_read_byte;
	this->dev_ready = tqm8272_device_ready;
	this->select_chip = tqm8272_select_chip;
	this->read_buf = tqm8272_read_buf;
	this->write_buf = tqm8272_write_buf;
	this->verify_buf = tqm8272_verify_buf;
	this->chip_delay = 50;
	this->ecc.mode = NAND_ECC_SOFT;

	printk("Searching for NAND flash...\n");

	/* Scan to find existence of the device */
	if (nand_scan(tqm8272_mtd, 4)) {
		kfree(tqm8272_mtd);
		return -ENXIO;
	}
	printk("... found %d chip(s) size: %x\n", this->numchips, tqm8272_mtd->size);
#ifdef CONFIG_MTD_PARTITIONS
	tqm8272_mtd->name = "tqm8272-nand";
	mtd_parts_nb = parse_mtd_partitions(tqm8272_mtd, part_probes, &mtd_parts, 0);
	if (mtd_parts_nb > 0)
		part_type = "command line";
	else
		mtd_parts_nb = 0;
#endif
	if (mtd_parts_nb == 0) {
		if (tqm8272_mtd->size >= (128 * 0x100000))
			mtd_parts = partition_info128;
		mtd_parts_nb = NUM_PARTITIONS;
		part_type = "static";
	}

	/* Register the partitions */
	printk(KERN_NOTICE "Using %s partition definition\n", part_type);
	add_mtd_partitions(tqm8272_mtd, mtd_parts, mtd_parts_nb);

	return 0;
}

module_init(tqm8272_init);

/*
 * Clean up routine
 */
static void __exit tqm8272_cleanup(void)
{
	iounmap ((void *)CFG_NAND_BASE);
	/* Unregister the device */
	del_mtd_device(tqm8272_mtd);

	/* Free the MTD device structure */
	kfree(tqm8272_mtd);
}

module_exit(tqm8272_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Heiko Schocher <hs@denx.de>");
MODULE_DESCRIPTION("MTD map driver for TQM8272 board");
