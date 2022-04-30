/*
 * drivers/mtd/maps/tqm82xx.c
 *
 * MTD mapping driver for TQM82xx boards
 *
 * Copyright 2006 Heiko Schocher, DENX Software Engineering, <hs@denx.de>.
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2.  This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <asm/ppcboot.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>

#define FLASH_BANK_MAX	1

extern unsigned char __res[];

/* trivial struct to describe partition information */
struct mtd_part_def
{
	int nums;
	unsigned char *type;
	struct mtd_partition* mtd_part;
};

static struct mtd_info* mtd_banks[FLASH_BANK_MAX];
static struct map_info* map_banks[FLASH_BANK_MAX];
static struct mtd_part_def part_banks[FLASH_BANK_MAX];

static unsigned long num_banks;
static unsigned long start_scan_addr;

#ifdef CONFIG_MTD_PARTITIONS
/*
 * The following defines the partition layout of TQM82xx boards.
 *
 * See include/linux/mtd/partitions.h for definition of the
 * mtd_partition structure.
 *
 * Assume minimal initial size of 4 MiB per bank, will be updated
 * later in init_tqm82xx_mtd() routine.
 */

#define RW_PART0_OF	0
#define RW_PART0_SZ	0x40000	/* U-Boot */
#define RW_PART1_SZ	0x20000	/* Environment */
#define RW_PART2_SZ	0x20000	/* Environment redundant */
#define RW_PART3_SZ	0x40000	/* Kernel */
#define RW_PART4_SZ	0x40000	/* Ramdisk */

/* Partition definition for the first flash bank which is always present. */
static struct mtd_partition tqm82xx_partitions_bank1[] = {
	{
		.name = "u-boot",
		.offset = RW_PART0_OF,
		.size = RW_PART0_SZ
	},
	{
		.name = "env",
		.offset = MTDPART_OFS_APPEND,
		.size = RW_PART1_SZ,
	},
	{
		.name = "red_env",
		.offset = MTDPART_OFS_APPEND,
		.size = RW_PART2_SZ,
	},
	{
		.name = "kernel",
		.offset = MTDPART_OFS_APPEND,
		.size = RW_PART3_SZ,
	},
	{
		.name = "root",
		.offset = MTDPART_OFS_APPEND,
		.size = RW_PART4_SZ,
	},
	{
		.name = "user",
		.offset = MTDPART_OFS_APPEND,
		/* NOTE: this partion size is re-calcated in		*/
		/* init_tqm82xx_mtd() to cover actual device size.	*/
	}
};

#endif	/* CONFIG_MTD_PARTITIONS */

static int __init init_tqm82xx_mtd(void)
{
	int idx = 0, ret = 0;
	unsigned long flash_addr, flash_size, mtd_size = 0;

	/* pointer to TQM82xx board info data */
	bd_t *bd = (bd_t *)__res;
#ifdef CONFIG_MTD_CMDLINE_PARTS
	int n;
	char mtdid[4];
	const char *part_probes[] = { "cmdlinepart", NULL };
#endif

	flash_addr = bd->bi_flashstart;
	flash_size = bd->bi_flashsize;

	/* request maximum flash size address space */
	start_scan_addr = (unsigned long)ioremap(flash_addr, flash_size);
	if (!start_scan_addr) {
		printk(KERN_NOTICE "%s: Failed to ioremap address: 0x%lx\n",
		       __FUNCTION__, flash_addr);
		return -EIO;
	}

	for(idx = 0 ; idx < FLASH_BANK_MAX ; idx++) {
		if (mtd_size >= flash_size)
			break;

		pr_debug("%s: chip probing count %d\n", __FUNCTION__, idx);

		map_banks[idx] = (struct map_info *)kmalloc(sizeof(struct map_info),
							    GFP_KERNEL);
		if (map_banks[idx] == NULL) {
			ret = -ENOMEM;
			goto error_mem;
		}
		memset((void *)map_banks[idx], 0, sizeof(struct map_info));
		map_banks[idx]->name = (char *)kmalloc(16, GFP_KERNEL);
		if (map_banks[idx]->name == NULL) {
			ret = -ENOMEM;
			goto error_mem;
		}
		memset((void *)map_banks[idx]->name, 0, 16);

		sprintf(map_banks[idx]->name, "TQM82xx-%d", idx);
		map_banks[idx]->size = flash_size;
		map_banks[idx]->bankwidth = 4;

		simple_map_init(map_banks[idx]);

		map_banks[idx]->virt = (void __iomem *)
			(start_scan_addr + ((idx > 0) ?
					    (mtd_banks[idx-1] ?
					     mtd_banks[idx-1]->size : 0) : 0));
		map_banks[idx]->phys =
			flash_addr + ((idx > 0) ?
				      (mtd_banks[idx-1] ?
				       mtd_banks[idx-1]->size : 0) : 0);

		/* start to probe flash chips */
		mtd_banks[idx] = do_map_probe("cfi_probe", map_banks[idx]);
		if (mtd_banks[idx]) {
			mtd_banks[idx]->owner = THIS_MODULE;
			mtd_size += mtd_banks[idx]->size;
			num_banks++;
			pr_debug("%s: bank %ld, name: %s, size: %d bytes \n",
				 __FUNCTION__, num_banks,
				 mtd_banks[idx]->name, mtd_banks[idx]->size);
		}
	}

	/* no supported flash chips found */
	if (!num_banks) {
		printk(KERN_NOTICE "TQM82xx: No supported flash chips found!\n");
		ret = -ENXIO;
		goto error_mem;
	}

#ifdef CONFIG_MTD_PARTITIONS
	/*
	 * Select static partition definitions
	 */
	n = ARRAY_SIZE(tqm82xx_partitions_bank1);
	part_banks[0].mtd_part	= tqm82xx_partitions_bank1;
	part_banks[0].type	= "static image bank1";
	part_banks[0].nums	= n;

	/*
	 * Adjust partitions to flash size
	 */
	tqm82xx_partitions_bank1[5].size = flash_size -
		RW_PART0_SZ - RW_PART1_SZ - RW_PART2_SZ - RW_PART3_SZ \
		- RW_PART4_SZ;

	for(idx = 0; idx < num_banks ; idx++) {
#ifdef CONFIG_MTD_CMDLINE_PARTS
		sprintf(mtdid, "%d", idx);
		n = parse_mtd_partitions(mtd_banks[idx],
					 part_probes,
					 &part_banks[idx].mtd_part,
					 0);
		pr_debug("%s: %d command line partitions on bank %s\n",
			 __FUNCTION__, n, mtdid);
		if (n > 0) {
			part_banks[idx].type = "command line";
			part_banks[idx].nums = n;
		}
#endif	/* CONFIG_MTD_CMDLINE_PARTS */
		if (part_banks[idx].nums == 0) {
			printk(KERN_NOTICE
			       "TQM82xx flash bank %d: no partition info "
			       "available, registering whole device\n", idx);
			add_mtd_device(mtd_banks[idx]);
		} else {
			printk(KERN_NOTICE
			       "TQM82xx flash bank %d: Using %s partition "
			       "definition\n", idx, part_banks[idx].type);
			add_mtd_partitions(mtd_banks[idx],
					   part_banks[idx].mtd_part,
					   part_banks[idx].nums);
		}
	}
#else	/* ! CONFIG_MTD_PARTITIONS */
	printk(KERN_NOTICE "TQM82xx flash: registering %d flash banks "
	       "at once\n", num_banks);

	for(idx = 0 ; idx < num_banks ; idx++)
		add_mtd_device(mtd_banks[idx]);

#endif	/* CONFIG_MTD_PARTITIONS */

	return 0;
error_mem:
	for (idx = 0 ; idx < FLASH_BANK_MAX ; idx++) {
		if (map_banks[idx] != NULL) {
			if (map_banks[idx]->name != NULL) {
				kfree(map_banks[idx]->name);
				map_banks[idx]->name = NULL;
			}
			kfree(map_banks[idx]);
			map_banks[idx] = NULL;
		}
	}

	iounmap((void *)start_scan_addr);

	return ret;
}

static void __exit cleanup_tqm82xx_mtd(void)
{
	unsigned int idx = 0;
	for(idx = 0 ; idx < num_banks ; idx++) {
		/* destroy mtd_info previously allocated */
		if (mtd_banks[idx]) {
			del_mtd_partitions(mtd_banks[idx]);
			map_destroy(mtd_banks[idx]);
		}

		/* release map_info not used anymore */
		kfree(map_banks[idx]->name);
		kfree(map_banks[idx]);
	}

	if (start_scan_addr) {
		iounmap((void *)start_scan_addr);
		start_scan_addr = 0;
	}
}

module_init(init_tqm82xx_mtd);
module_exit(cleanup_tqm82xx_mtd);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Heiko Schocher <hs@denx.de>");
MODULE_DESCRIPTION("MTD map driver for TQM82xx boards");
