#if defined(CONFIG_BCM_KF_MTD_BCMNAND)
#include <linux/module.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <../drivers/mtd/mtdcore.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>
#include <linux/platform_device.h>
#include <asm/bootinfo.h>
#include <bcm_hwdefs.h>
#include <board.h>
#include <bcm_map_part.h>

#include "flash_api.h"

#define PRINTK(...)
//#define PRINTK printk

bool SpiNandRegistered = FALSE;

extern void bcmspinand_probe(struct mtd_info * mtd);


static struct mtd_partition bcm63XX_nand_parts[] =
{
    {name: "rootfs",		offset: 0, size: 0},
    {name: "rootfs_update",	offset: 0, size: 0},
    {name: "data",			offset: 0, size: 0},
    {name: "nvram",			offset: 0, size: 0},
    {name: "image",			offset: 0, size: 0},
    {name: "image_update",	offset: 0, size: 0},
    {name: NULL,			offset: 0, size: 0},
    {name: NULL,			offset: 0, size: 0},
    {name: NULL,			offset: 0, size: 0}
};

static int __init 
is_split_partition (struct mtd_info* mtd, unsigned long offset, unsigned long size, unsigned long *split_offset)
{
    uint8_t buf[0x100];
    size_t retlen;
    int split_found = 0;

    /* Search RootFS partion for split marker.
     * Marker is located in the last 0x100 bytes of the last BootFS Erase Block
     * If marker is found, we have separate Boot and Root Partitions.
     */
    for (*split_offset = offset + mtd->erasesize; *split_offset <= offset + size; *split_offset += mtd->erasesize)
    {
        if (mtd->_block_isbad(mtd, *split_offset - mtd->erasesize)) {
            continue;
        }
        mtd->_read(mtd, *split_offset - 0x100, 0x100, &retlen, buf);

        if (!strncmp (BCM_BCMFS_TAG, buf, strlen (BCM_BCMFS_TAG))) {
            if (!strncmp (BCM_BCMFS_TYPE_UBIFS, &buf[strlen (BCM_BCMFS_TAG)], strlen (BCM_BCMFS_TYPE_UBIFS)))
            {
                printk("***** Found UBIFS Marker at 0x%08lx\n", *split_offset - 0x100);
                split_found = 1;
                break;
            }
        }
    }

    return split_found;
}

static int __init mtd_init(void)
{
    struct mtd_info * mtd;
    struct nand_chip * nand;

    /* If SPI NAND FLASH is present then register the device. Otherwise do nothing */
    if (FLASH_IFC_SPINAND != flash_get_flash_type())
        return -ENODEV;

    if (((mtd = kmalloc(sizeof(struct mtd_info), GFP_KERNEL)) == NULL) ||
        ((nand = kmalloc(sizeof(struct nand_chip), GFP_KERNEL)) == NULL))
    {
        printk("Unable to allocate SPI NAND dev structure.\n");
        return -ENOMEM;
    }

    memset(mtd, 0, sizeof(struct mtd_info));
    memset(nand, 0, sizeof(struct nand_chip));

    mtd->priv = nand;

    bcmspinand_probe(mtd);

    /* Scan to check existence of the nand device */
    if(nand_scan(mtd, 1))
    {
        static NVRAM_DATA nvram;
        unsigned long rootfs_ofs;
        int nr_parts;
        int rootfs, rootfs_update;
        unsigned long split_offset;

        /* Root FS.  The CFE RAM boot loader saved the rootfs offset that the
         * Linux image was loaded from.
         */
        kerSysBlParmsGetInt(NAND_RFS_OFS_NAME, (int *) &rootfs_ofs);

        kerSysNvRamLoad(mtd);
        kerSysNvRamGet((char *)&nvram, sizeof(nvram), 0);
        nr_parts = 6;

        if( rootfs_ofs == nvram.ulNandPartOfsKb[NP_ROOTFS_1] )
        {
            rootfs = NP_ROOTFS_1;
            rootfs_update = NP_ROOTFS_2;
        }
        else if( rootfs_ofs == nvram.ulNandPartOfsKb[NP_ROOTFS_2] )
        {
            rootfs = NP_ROOTFS_2;
            rootfs_update = NP_ROOTFS_1;
        }
        else
        {
            /* Backward compatibility with old cferam. */
            extern unsigned char _text;
            unsigned long rootfs_ofs = *(unsigned long *) (&_text - 4);

            if( rootfs_ofs == nvram.ulNandPartOfsKb[NP_ROOTFS_1] )
            {
                rootfs = NP_ROOTFS_1;
                rootfs_update = NP_ROOTFS_2;
            }
            else
            {
                rootfs = NP_ROOTFS_2;
                rootfs_update = NP_ROOTFS_1;
            }
        }

        /* RootFS partition */
        bcm63XX_nand_parts[0].offset = nvram.ulNandPartOfsKb[rootfs]*1024;
        bcm63XX_nand_parts[0].size = nvram.ulNandPartSizeKb[rootfs]*1024;
        bcm63XX_nand_parts[0].ecclayout = nand->ecclayout;

        /* This partition is used for flashing images */
        bcm63XX_nand_parts[4].offset = bcm63XX_nand_parts[0].offset;
        bcm63XX_nand_parts[4].size = bcm63XX_nand_parts[0].size;
        bcm63XX_nand_parts[4].ecclayout = nand->ecclayout;

        if (is_split_partition (mtd, bcm63XX_nand_parts[0].offset, bcm63XX_nand_parts[0].size, &split_offset))
        {
            /* RootFS partition */
            bcm63XX_nand_parts[0].offset = split_offset;
            bcm63XX_nand_parts[0].size -= (split_offset - nvram.ulNandPartOfsKb[rootfs]*1024);

            /* BootFS partition */
            bcm63XX_nand_parts[nr_parts].name = "bootfs";
            bcm63XX_nand_parts[nr_parts].offset = nvram.ulNandPartOfsKb[rootfs]*1024;
            bcm63XX_nand_parts[nr_parts].size = split_offset - nvram.ulNandPartOfsKb[rootfs]*1024;
            bcm63XX_nand_parts[nr_parts].ecclayout = nand->ecclayout;
            nr_parts++;

            if (strncmp(arcs_cmdline, "root=", 5)) {
                kerSysSetBootParm("ubi.mtd", "0");
                kerSysSetBootParm("root=", "ubi:rootfs_ubifs");
                kerSysSetBootParm("rootfstype=", "ubifs");
            }
        }
        else {
            if (strncmp(arcs_cmdline, "root=", 5)) {
                kerSysSetBootParm("root=", "mtd:rootfs");
                kerSysSetBootParm("rootfstype=", "jffs2");
            }
        }

        /* RootFS_update partition */
        bcm63XX_nand_parts[1].offset = nvram.ulNandPartOfsKb[rootfs_update]*1024;
        bcm63XX_nand_parts[1].size = nvram.ulNandPartSizeKb[rootfs_update]*1024;
        bcm63XX_nand_parts[1].ecclayout = nand->ecclayout;

        /* This partition is used for flashing images */
        bcm63XX_nand_parts[5].offset = bcm63XX_nand_parts[1].offset;
        bcm63XX_nand_parts[5].size = bcm63XX_nand_parts[1].size;
        bcm63XX_nand_parts[5].ecclayout = nand->ecclayout;

        if (is_split_partition (mtd, bcm63XX_nand_parts[1].offset, bcm63XX_nand_parts[1].size, &split_offset))
        {
            /* rootfs_update partition */
            bcm63XX_nand_parts[1].offset = split_offset;
            bcm63XX_nand_parts[1].size -= (split_offset - nvram.ulNandPartOfsKb[rootfs_update]*1024);

            /* bootfs_update partition */
            bcm63XX_nand_parts[nr_parts].name = "bootfs_update";
            bcm63XX_nand_parts[nr_parts].offset = nvram.ulNandPartOfsKb[rootfs_update]*1024;
            bcm63XX_nand_parts[nr_parts].size = split_offset - nvram.ulNandPartOfsKb[rootfs_update]*1024;
            bcm63XX_nand_parts[nr_parts].ecclayout = nand->ecclayout;
            nr_parts++;
        }

        /* Data (psi, scratch pad) */
        bcm63XX_nand_parts[2].offset = nvram.ulNandPartOfsKb[NP_DATA] * 1024;
        bcm63XX_nand_parts[2].size = nvram.ulNandPartSizeKb[NP_DATA] * 1024;
        bcm63XX_nand_parts[2].ecclayout = nand->ecclayout;

        /* Boot and NVRAM data */
        bcm63XX_nand_parts[3].offset = nvram.ulNandPartOfsKb[NP_BOOT] * 1024;
        bcm63XX_nand_parts[3].size = nvram.ulNandPartSizeKb[NP_BOOT] * 1024;
        bcm63XX_nand_parts[3].ecclayout = nand->ecclayout;

        PRINTK("Part[0] name=%s, size=%llx, ofs=%llx\n", bcm63XX_nand_parts[0].name,
            bcm63XX_nand_parts[0].size, bcm63XX_nand_parts[0].offset);
        PRINTK("Part[1] name=%s, size=%llx, ofs=%llx\n", bcm63XX_nand_parts[1].name,
            bcm63XX_nand_parts[1].size, bcm63XX_nand_parts[1].offset);
        PRINTK("Part[2] name=%s, size=%llx, ofs=%llx\n", bcm63XX_nand_parts[2].name,
            bcm63XX_nand_parts[2].size, bcm63XX_nand_parts[2].offset);
        PRINTK("Part[3] name=%s, size=%llx, ofs=%llx\n", bcm63XX_nand_parts[3].name,
            bcm63XX_nand_parts[3].size, bcm63XX_nand_parts[3].offset);

        mtd_device_register(mtd, bcm63XX_nand_parts, nr_parts);

        SpiNandRegistered = TRUE;
    }

    return 0;
}

module_init(mtd_init);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("David Regan");
MODULE_DESCRIPTION("MTD map and partitions SPI NAND");

#endif /* CONFIG_BCM_KF_MTD_BCMNAND */
