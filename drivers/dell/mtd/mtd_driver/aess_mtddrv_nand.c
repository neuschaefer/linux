/*
 *  aess_mtddrv_nand.c
 *
 *  Copyright (c) 2007 Avocent Corp.
 *
 *  Derived from drivers/mtd/nand/autcpu12.c
 *       Copyright (c) 2001 Thomas Gleixner (gleixner@autronix.de)
 *
 * $Id$
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Overview:
 *   This is a device driver for the NAND flash device found on the
 *   winbond wpcm450 board. It supports 64MB nand flash.
 */

#include <linux/slab.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <asm/io.h>
#include <asm/sizes.h>
#include <asm/arch/map.h>

/* Use SYSTEM_NAME defined in environment.dat to determine 
platform.
(chtsai)
//#define CONFIG_WPCM450_NAND_EVB
//#define CONFIG_WPCM450_NAND_WHOVILLE
*/

#ifdef CONFIG_MTD_PARTITIONS
#define	mtd_has_partitions()    (1)
#else
#define	mtd_has_partitions()    (0)
#endif

#define WPCM450_MFSEL1          (WPCM450_VA_GCR+0x00C)
#define WPCM450_XBCR            (WPCM450_VA_GCR+0x04C)
#define WPCM450_MFSEL1_XCS2SEL  (1<<28)

#ifdef CONFIG_MTD_NAND_EVB
/*
 * Define partitions for flash devices
 */
static struct mtd_partition wpcm450_nand_partition_info[] = {
	{ .name		= "WPCM450 EVB NAND Flash Partition 4 (BBT, Bad Block Table)",
	  .offset	= 0x000000,
	  .size		= 1 * SZ_1M },
	{ .name		= "WPCM450 EVB NAND Flash Partition 5 (Redundant Kernel)",
	  .offset	= 0x100000,    
	  .size		= 3 * SZ_1M },
	{ .name		= "WPCM450 EVB NAND Flash Partition 6 (Redundant Rootfs)",
	  .offset	= 0x400000,    
	  .size		= 60 * SZ_1M },
};

static struct nand_bbt_descr wpcm450_badblock_pattern = {
	.options = 0,
	.offs = 5,
	.len = 1,
};

#else
#ifdef CONFIG_MTD_NAND_WHOVILLE
/*
 * Define partitions for flash devices
 */
static struct mtd_partition wpcm450_nand_partition_info[] = {
	{ .name		= "Whoville NAND Flash Partition 2 (Bad Block Table)",
	  .offset	= 0x000000,
	  .size		= 1 * SZ_1M },
	{ .name		= "Whoville NAND Flash Partition 3 (Kernel)",
	  .offset	= 0x100000,    
	  .size		= 3 * SZ_1M },
	{ .name		= "Whoville NAND Flash Partition 4 (Rootfs)",
	  .offset	= 0x400000,    
	  .size		= 60 * SZ_1M },
};

static struct nand_bbt_descr wpcm450_badblock_pattern = {
	.options = 0,
	.offs = 0,
	.len = 2,
};

#endif
#endif


static uint8_t bbt_pattern[] = {'B', 'b', 't', '0' };
static uint8_t mirror_pattern[] = {'1', 't', 'b', 'B' };

static struct nand_bbt_descr wpcm450_bbt_main_descr = {
	.options = NAND_BBT_CREATE | NAND_BBT_WRITE
		| NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_PERCHIP,
	.offs =	8,
	.len = 4,
	.veroffs = 12,
	.maxblocks = 4,
	.pattern = bbt_pattern
};

static struct nand_bbt_descr wpcm450_bbt_mirror_descr = {
	.options = NAND_BBT_CREATE | NAND_BBT_WRITE
		| NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_PERCHIP,
	.offs =	8,
	.len = 4,
	.veroffs = 12,
	.maxblocks = 4,
	.pattern = mirror_pattern
};

/*
 * MTD structure for wpcm450 board
 */
static struct mtd_info *wpcm450_nand_mtd = NULL;


static inline void nand_cs_on(void)
{
}

static void nand_cs_off(void)
{
}

/*
 *	hardware specific access to control-lines
*/
static void wpcm450_hwcontrol(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	struct nand_chip* this = mtd->priv;
	unsigned int nandaddr = (unsigned int)this->IO_ADDR_W;
	
	if (ctrl & NAND_CTRL_CHANGE) {
		if ( ctrl & NAND_ALE )
		{
			nandaddr |=  (2);
		}
		else
		{
			nandaddr &= ~(2);
        }			
		if ( ctrl & NAND_CLE )
		{
			nandaddr |=  (1);
		}	
		else
		{
			nandaddr &= ~(1);
	    }		
		if ( ctrl & NAND_NCE )
		{
			nand_cs_on();
        }			
		else
		{
			nand_cs_off();
        }			
	}

	this->IO_ADDR_W = (void __iomem*)nandaddr;
	
	if (cmd != NAND_CMD_NONE)
	{
		writeb(cmd, this->IO_ADDR_W);		
    }		
}

static u_char wpcm450_read_byte(struct mtd_info *mtd)
{
	struct nand_chip *this = mtd->priv;

	return (readb(this->IO_ADDR_R));    
}

static void wpcm450_read_buf(struct mtd_info *mtd, u_char *buf, int len)
{
	int i;
	struct nand_chip *this = mtd->priv;
		    
	for (i=0; i<len; i++)
	{
		*buf++ = readb(this->IO_ADDR_R);    
    }		
}

static void wpcm450_write_buf(struct mtd_info *mtd, const u_char *buf, int len)
{
	int i;
	struct nand_chip *this = mtd->priv;
		    
	for (i=0; i<len; i++)
	{
		writeb(*buf++ , this->IO_ADDR_W);    
	}
}

static int wpcm450_verify_buf(struct mtd_info *mtd, const u_char *buf, int len)
{
	int i;
	struct nand_chip *this = mtd->priv;
		    
	for (i=0; i<len; i++)
	{
		if (buf[i] != readb(this->IO_ADDR_R))
		{
            printk(KERN_ERR  "WPCM450 NAND Flash Verify Buffer Fault!\n");		    
			return -EFAULT;
		}
	}
	
	return 0;    
}

/*
 * Main initialization routine
 */
int __init wpcm450_init (void)
{
    struct nand_chip *this;
    unsigned long tempreg = 0;
    int i;

    /* enable nXCS2 */
    tempreg = ioread32(WPCM450_MFSEL1);
    iowrite32((unsigned long) (tempreg | WPCM450_MFSEL1_XCS2SEL), WPCM450_MFSEL1);

    /* X-Bus timing: 0x7FF77FFF is lowest, 0 is highest */ 
#ifdef CONFIG_MTD_NAND_EVB
    iowrite32((unsigned long) 0, WPCM450_XBCR);
#else
#ifdef CONFIG_MTD_NAND_WHOVILLE
    iowrite32((unsigned long) (0x7FF77FFF), WPCM450_XBCR);
#endif
#endif

    /* Allocate memory for MTD device structure and private data */
    wpcm450_nand_mtd = kmalloc(sizeof(struct mtd_info) + sizeof (struct nand_chip),
                          GFP_KERNEL);
    if (!wpcm450_nand_mtd)
    {
        printk(KERN_ERR "Unable to allocate WPCM450 NAND MTD device structure.\n");
        return -ENOMEM;
    }

    /* Get pointer to private data */
    this = (struct nand_chip *) (&wpcm450_nand_mtd[1]);

    /* Initialize structures */
    memset((char *) wpcm450_nand_mtd, 0, sizeof(struct mtd_info));
    memset((char *) this, 0, sizeof(struct nand_chip));

    /* Link the private data with the MTD structure */
    wpcm450_nand_mtd->priv = this;
	wpcm450_nand_mtd->owner = THIS_MODULE;    

    /* Set address of NAND IO lines */
    this->IO_ADDR_R = WPCM450_VA_XBUS2;
    this->IO_ADDR_W = WPCM450_VA_XBUS2;
    this->cmd_ctrl = wpcm450_hwcontrol;
    //this->dev_ready = wpcm450_device_ready;

#ifdef CONFIG_MTD_NAND_EVB
    /* 12 us command delay time */
    this->chip_delay = 12;
#else
#ifdef CONFIG_MTD_NAND_WHOVILLE
    /* 25 us command delay time */
    this->chip_delay = 60;
#endif
#endif

    this->ecc.mode = NAND_ECC_SOFT;

    /* Enable the following for a flash based bad block table */
    this->options = NAND_USE_FLASH_BBT;
    this->bbt_td = &wpcm450_bbt_main_descr;
    this->bbt_md = &wpcm450_bbt_mirror_descr;
    this->badblock_pattern = &wpcm450_badblock_pattern;

	/* read/write functions */
	this->read_byte = wpcm450_read_byte;
	this->read_buf = wpcm450_read_buf;
	this->write_buf = wpcm450_write_buf;
	this->verify_buf = wpcm450_verify_buf;

    /* Scan to find existance of the device */
    if (nand_scan (wpcm450_nand_mtd, 1))
    {
		printk(KERN_ERR  "No NAND device\n");    
		kfree(wpcm450_nand_mtd);  
		  
        return -ENXIO;
    }

    /* partitions should match sector boundaries; and it may be good to
     * use readonly partitions for writeprotected sectors (BP2..BP0).
     */
    if (mtd_has_partitions())
    {
        struct mtd_partition	*parts = NULL;
        int nr_parts = 0;
      
        #ifdef CONFIG_MTD_CMDLINE_PARTS
        static const char *part_probes[] = { "cmdlinepart", NULL, };

        nr_parts = parse_mtd_partitions(wpcm450_nand_mtd, part_probes, &parts, 0);
        #endif

        if (nr_parts <= 0)
        {
            /* Use defaule Partition */
            parts = wpcm450_nand_partition_info;
            nr_parts = sizeof(wpcm450_nand_partition_info) / sizeof(struct mtd_partition);
        }

        if (nr_parts > 0)
        {

            for (i = 0; i < nr_parts; i++)
            {
                DEBUG(MTD_DEBUG_LEVEL0, "partitions[%d] = "
                       "{.name = %s, .offset = 0x%.8x, "
                       ".size = 0x%.8x (%uK) }\n",
                       i, parts[i].name,
                       parts[i].offset,
                       parts[i].size,
                       parts[i].size / 1024);
            }
            
            return add_mtd_partitions(wpcm450_nand_mtd, parts, nr_parts);
        }
    } 

    return add_mtd_device(wpcm450_nand_mtd) == 1 ? -ENODEV : 0;
}

module_init(wpcm450_init);

static void __exit wpcm450_cleanup (void)
{
	/* Release resources, unregister device */
	nand_release (wpcm450_nand_mtd);

	/* Free the MTD device structure */
	kfree (wpcm450_nand_mtd);
}
module_exit(wpcm450_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Timothy Huang <timothy.huang@avocent.com>");
MODULE_DESCRIPTION("NAND flash device support on wpcm450 evaluation broad");
