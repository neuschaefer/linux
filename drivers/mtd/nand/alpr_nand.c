/*
 *  drivers/mtd/alpr_nand.c
 *
 *  Overview:
 *   Driver for Prodrive NAND Flash Controller
 *
 * (C) Copyright 2006
 * Stefan Roese, DENX Software Engineering, sr@denx.de.
 *
 * Based on original work by
 *	Thomas Gleixner
 *	Copyright 2006 IBM
 *
 *  This program is free software; you can redistribute	 it and/or modify it
 *  under  the terms of	 the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the	License, or (at your
 *  option) any later version.
 *
 */
#include <linux/module.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/mtd.h>
#include <linux/platform_device.h>

#include <asm/io.h>
#include <asm/ibm44x.h>

/* PD_NDFC Register definitions */
#define PD_NDFC_CMD0		0x00
#define PD_NDFC_CMD1		0x01
#define PD_NDFC_CMD2		0x02
#define PD_NDFC_CMD3		0x03
#define PD_NDFC_ADDR_WAIT	0x04
#define PD_NDFC_TERM		0x05
#define PD_NDFC_DATA		0x08

#define PD_NDFC_MAX_BANKS	4

struct pd_ndfc_nand_mtd {
	struct mtd_info			mtd;
	struct nand_chip		chip;
	struct platform_nand_chip	*pl_chip;
};

static struct pd_ndfc_nand_mtd pd_ndfc_mtd[PD_NDFC_MAX_BANKS];

struct pd_ndfc_controller {
	void __iomem		*ndfcbase;
	struct nand_hw_control	pd_ndfc_control;
	atomic_t		childs_active;
};

static struct pd_ndfc_controller pd_ndfc_ctrl;
static int selected_chip;

static void pd_ndfc_select_chip0(struct mtd_info *mtd, int chip)
{
	selected_chip = 0;
}

static void pd_ndfc_select_chip1(struct mtd_info *mtd, int chip)
{
	selected_chip = 1;
}

static void pd_ndfc_select_chip2(struct mtd_info *mtd, int chip)
{
	selected_chip = 2;
}

static void pd_ndfc_select_chip3(struct mtd_info *mtd, int chip)
{
	selected_chip = 3;
}

static void pd_ndfc_hwcontrol(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	struct pd_ndfc_controller *pd_ndfc = &pd_ndfc_ctrl;
	u32 cmd_offs[4] = {PD_NDFC_CMD0, PD_NDFC_CMD1, PD_NDFC_CMD2, PD_NDFC_CMD3};

	if (cmd == NAND_CMD_NONE)
		return;

	if (ctrl & NAND_CLE)
		out_8(pd_ndfc->ndfcbase + cmd_offs[selected_chip], cmd & 0xff);
	else
		out_8(pd_ndfc->ndfcbase + PD_NDFC_ADDR_WAIT, cmd & 0xff);
}

static int pd_ndfc_ready(struct mtd_info *mtd)
{
	struct pd_ndfc_controller *pd_ndfc = &pd_ndfc_ctrl;

	/*
	 * blocking read until ready
	 */
	in_8(pd_ndfc->ndfcbase + PD_NDFC_ADDR_WAIT);

	return 1;
}

static void pd_ndfc_write_byte(struct mtd_info *mtd, u_char byte)
{
	struct pd_ndfc_controller *pd_ndfc = &pd_ndfc_ctrl;

	out_8(pd_ndfc->ndfcbase + PD_NDFC_DATA, byte);
}

static u_char pd_ndfc_read_byte(struct mtd_info *mtd)
{
	struct pd_ndfc_controller *pd_ndfc = &pd_ndfc_ctrl;
	u_char retval;

	retval = (u_char)in_8(pd_ndfc->ndfcbase + PD_NDFC_DATA);

	return retval;
}

static void pd_ndfc_write_buf(struct mtd_info *mtd, const u_char *buf, int len)
{
	int i;

	for (i=0; i<len; i++)
		pd_ndfc_write_byte(mtd, buf[i]);
}

static void pd_ndfc_read_buf(struct mtd_info *mtd, u_char *buf, int len)
{
	int i;

	for (i=0; i<len; i++)
		buf[i] = pd_ndfc_read_byte(mtd);
}

static int pd_ndfc_verify_buf(struct mtd_info *mtd, const u_char *buf, int len)
{
	int i;

	for (i=0; i<len; i++)
		if (buf[i] != pd_ndfc_read_byte(mtd))
			return -EFAULT;

	return 0;
}

/*
 * Initialize chip structure
 */
static void pd_ndfc_chip_init(struct pd_ndfc_nand_mtd *mtd, int chip_nr)
{
	struct pd_ndfc_controller *pd_ndfc = &pd_ndfc_ctrl;
	struct nand_chip *chip = &mtd->chip;

	chip->IO_ADDR_R = pd_ndfc->ndfcbase + PD_NDFC_DATA;
	chip->IO_ADDR_W = pd_ndfc->ndfcbase + PD_NDFC_DATA;
	chip->cmd_ctrl = pd_ndfc_hwcontrol;
	chip->dev_ready = pd_ndfc_ready;
	chip->read_byte = pd_ndfc_read_byte;
	chip->write_buf = pd_ndfc_write_buf;
	chip->read_buf = pd_ndfc_read_buf;
	chip->verify_buf = pd_ndfc_verify_buf;
	switch (chip_nr) {
	case 0:
		chip->select_chip = pd_ndfc_select_chip0;
		break;
	case 1:
		chip->select_chip = pd_ndfc_select_chip1;
		break;
	case 2:
		chip->select_chip = pd_ndfc_select_chip2;
		break;
	case 3:
		chip->select_chip = pd_ndfc_select_chip3;
		break;
	}
	chip->chip_delay = 50;
	chip->priv = mtd;
	chip->options = mtd->pl_chip->options;
	chip->controller = &pd_ndfc->pd_ndfc_control;
	chip->ecc.mode = NAND_ECC_SOFT;
	mtd->mtd.priv = chip;
	mtd->mtd.owner = THIS_MODULE;
}

static int pd_ndfc_chip_probe(struct platform_device *pdev)
{
	struct platform_nand_chip *nc = pdev->dev.platform_data;
	struct pd_ndfc_controller *pd_ndfc = &pd_ndfc_ctrl;
	struct pd_ndfc_nand_mtd *nandmtd;

	if (nc->chip_offset >= PD_NDFC_MAX_BANKS || nc->nr_chips > PD_NDFC_MAX_BANKS)
		return -EINVAL;

	nandmtd = &pd_ndfc_mtd[nc->chip_offset];
	if (nandmtd->pl_chip)
		return -EBUSY;

	nandmtd->pl_chip = nc;
	pd_ndfc_chip_init(nandmtd, nc->chip_offset);

	/* Scan for chips */
	if (nand_scan(&nandmtd->mtd, 1)) {
		nandmtd->pl_chip = NULL;
		return -ENODEV;
	}

#ifdef CONFIG_MTD_PARTITIONS
	printk("Number of partitions %d\n", nc->nr_partitions);
	if (nc->nr_partitions) {
		add_mtd_partitions(&nandmtd->mtd, nc->partitions,
				   nc->nr_partitions);

	} else
#else
		add_mtd_device(&nandmtd->mtd);
#endif

	atomic_inc(&pd_ndfc->childs_active);

	return 0;
}

static int pd_ndfc_chip_remove(struct platform_device *pdev)
{
	return 0;
}

static int pd_ndfc_nand_probe(struct platform_device *pdev)
{
	struct resource *res = pdev->resource;
	struct pd_ndfc_controller *pd_ndfc = &pd_ndfc_ctrl;
	unsigned long long phys = 0x100000000LL | res->start;

	pd_ndfc->ndfcbase = ioremap64(phys, res->end - res->start + 1);
	if (!pd_ndfc->ndfcbase) {
		printk(KERN_ERR "PD_NDFC: ioremap failed\n");
		return -EIO;
	}

	spin_lock_init(&pd_ndfc->pd_ndfc_control.lock);
	init_waitqueue_head(&pd_ndfc->pd_ndfc_control.wq);

	platform_set_drvdata(pdev, pd_ndfc);

	printk("PD_NDFC NAND Driver initialized.\n");

	return 0;
}

static int pd_ndfc_nand_remove(struct platform_device *pdev)
{
	struct pd_ndfc_controller *pd_ndfc = platform_get_drvdata(pdev);

	if (atomic_read(&pd_ndfc->childs_active))
		return -EBUSY;

	if (pd_ndfc) {
		platform_set_drvdata(pdev, NULL);
		iounmap(pd_ndfc_ctrl.ndfcbase);
		pd_ndfc_ctrl.ndfcbase = NULL;
	}
	return 0;
}

/* driver device registration */

static struct platform_driver pd_ndfc_chip_driver = {
	.probe		= pd_ndfc_chip_probe,
	.remove		= pd_ndfc_chip_remove,
	.driver		= {
		.name	= "pd-ndfc-chip",
		.owner	= THIS_MODULE,
	},
};

static struct platform_driver pd_ndfc_nand_driver = {
	.probe		= pd_ndfc_nand_probe,
	.remove		= pd_ndfc_nand_remove,
	.driver		= {
		.name	= "pd-ndfc-nand",
		.owner	= THIS_MODULE,
	},
};

static int __init pd_ndfc_nand_init(void)
{
	int ret;

	spin_lock_init(&pd_ndfc_ctrl.pd_ndfc_control.lock);
	init_waitqueue_head(&pd_ndfc_ctrl.pd_ndfc_control.wq);

	ret = platform_driver_register(&pd_ndfc_nand_driver);
	if (!ret)
		ret = platform_driver_register(&pd_ndfc_chip_driver);
	return ret;
}

static void __exit pd_ndfc_nand_exit(void)
{
	platform_driver_unregister(&pd_ndfc_chip_driver);
	platform_driver_unregister(&pd_ndfc_nand_driver);
}

module_init(pd_ndfc_nand_init);
module_exit(pd_ndfc_nand_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stefan Roese <sr@denx.de>");
MODULE_DESCRIPTION("Platform driver for Prodrive NAND controller");
