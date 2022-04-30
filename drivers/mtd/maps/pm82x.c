/*
 * Copyright(C) 2005
 * Wolfgang Denk, DENX Software Engineering, <wd@denx.de>
 *
 * Copyright(C) 2005
 * Josef Wagner, MicroSys GmbH <wagner@microsys.de>
 *
 * This code is GPLed
 *
 */

/*
 * PM825/PM826 uses 4 x Intel 28F160C3B (16 Mbit)
 * 4 x Intel 28F160C3 (16 Mbit) in one bank (64 bit bankwidth)
 * for a total of 8MB flash
 *
 * PM827/PM828 uses 4 x Intel 28F640C3 (64 Mbit)
 * in one bank (64 bit bankwidth)
 * for a total of 32MB flash

 * Thus we have to chose:
 * - Support 64-bit bankwidth => CONFIG_MTD_CFI_B8
 * - Support 4-chip flash interleave => CONFIG_MTD_CFI_I4
*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/ppcboot.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>

#if 0	/* Debugging turned off */
# define debugk(fmt,args...)	printk(fmt ,##args)
#else
# define debugk(fmt,args...)
#endif

#define FLASH_BANK_MAX 1

/* The "residual" data board information structure the boot loader
 * hands to us.
 */
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

static map_word pm82x_read64(struct map_info *map, unsigned long ofs)
{
	map_word val;
	val.x[0] = *((volatile __u32 *)(map->map_priv_1 + ofs));
	val.x[1] = *(((volatile __u32 *)(map->map_priv_1 + ofs)) + 1);
	return val;
}

static void pm82x_copy_from(struct map_info *map,
		void *to, unsigned long from, ssize_t len)
{
	memcpy_fromio(to, (void *)(map->map_priv_1 + from), len);
}

static void pm82x_write64 (struct map_info *map, map_word map_d,
		unsigned long adr)
{
	ulong addr = map->map_priv_1 + adr;
	__u64 d = ((__u64)map_d.x[0] << 32) + map_d.x[1];
	__u64 * data = &d;
	ulong flags;
	volatile ulong msr;
	ulong saved_msr;
	volatile long saved_fr[2];

	local_irq_save (flags);

	__asm__ __volatile__ ("mfmsr %0" : "=r" (msr):);
	saved_msr = msr;
	msr |= MSR_FP;
	msr &= ~(MSR_FE0 | MSR_FE1);

	__asm__ __volatile__ (
		"mtmsr %0\n"
		"isync\n"
		:
		: "r" (msr));

	__asm__ __volatile__ (
		"stfd 1, 0(%2)\n"
		"lfd  1, 0(%0)\n"
		"stfd 1, 0(%1)\n"
		"lfd  1, 0(%2)\n"
		 :
		 : "r" (data), "r" (addr), "b" (saved_fr)
	);

	__asm__ __volatile__ (
		"mtmsr %0\n"
		"isync\n"
		:
		: "r" (saved_msr));

	local_irq_restore (flags);
}

static void pm82x_copy_to(struct map_info *map,
		unsigned long to, const void *from, ssize_t len)
{
	memcpy_toio((void *)(map->map_priv_1 + to), from, len);
}

/*
 * The following defines the partition layout of PM825/PM826 boards.
 *
 * See include/linux/mtd/partitions.h for definition of the
 * mtd_partition structure.
 *
 */

#ifdef CONFIG_MTD_PARTITIONS
/* partition definition for first (and only) flash bank
 * also ref. to "drivers/char/flash_config.c"
 */
static struct mtd_partition pm82x_partitions_8M[] = {
	{
		name:		"U-Boot",		/* U-Boot image		*/
		offset:		0x00000000,
		size:		0x00040000,		/* 256 KB		*/
	},
	{
		name:		"kernel",		/* Linux kernel image	*/
		offset:		0x00040000,
		size:		0x000C0000,		/* 768 KB		*/
	},
	{
		name:		"ramdisk",		/* Ramdisk image	*/
		offset:		0x00100000,
		size:		0x00300000,		/* 3 MB			*/
	},
	{
		name:		"user",			/* User file system	*/
		offset:		0x00400000,
		size:		0x00400000,		/* 4 MB			*/
	},
};

static struct mtd_partition pm82x_partitions_32M[] = {
	{
		name:		"U-Boot",		/* U-Boot image		*/
		offset:		0x00000000,
		size:		0x00040000,		/* 256 KB		*/
	},
	{
		name:		"kernel",		/* Linux kernel image	*/
		offset:		0x00040000,
		size:		0x000C0000,		/* 768 KB		*/
	},
	{
		name:		"ramdisk",		/* Ramdisk image	*/
		offset:		0x00100000,
		size:		0x00300000,		/* 3 MB			*/
	},
	{
		name:		"user",			/* User file system	*/
		offset:		0x00400000,
		size:		0x01C00000,		/* 28 MB		*/
	},
};

#endif	/* CONFIG_MTD_PARTITIONS */

#define NB_OF(x) (sizeof (x) / sizeof (x[0]))

int __init init_pm82x_mtd(void)
{
	int idx = 0, ret = 0;
	unsigned long flash_addr, flash_size, mtd_size = 0;

	/* pointer to PM82x board info data */
	bd_t *bd = (bd_t *)__res;

#ifdef CONFIG_MTD_CMDLINE_PARTS
	int n;
	char mtdid[10];
	const char *part_probes[] = { "cmdlinepart", NULL };
#endif

	flash_addr = bd->bi_flashstart;
	flash_size = bd->bi_flashsize;

	/* request maximum flash size address space */
	start_scan_addr = (unsigned long)ioremap(flash_addr, flash_size);
	if (!start_scan_addr) {
		printk("%s: Failed to ioremap address: 0x%lx\n",
			__FUNCTION__, flash_addr);
		return -EIO;
	}

	for(idx = 0 ; idx < FLASH_BANK_MAX ; idx++) {
		if (mtd_size >= flash_size)
			break;

		debugk ("%s: chip probing count %d\n", __FUNCTION__, idx);

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

		sprintf(map_banks[idx]->name, "PM82x-%d", idx);
		map_banks[idx]->size	  = flash_size;
		map_banks[idx]->bankwidth = 8;
		map_banks[idx]->read	  = pm82x_read64;
		map_banks[idx]->copy_from = pm82x_copy_from;
		map_banks[idx]->write	  = pm82x_write64;
		map_banks[idx]->copy_to	  = pm82x_copy_to;
		map_banks[idx]->map_priv_1=
			start_scan_addr + ((idx > 0) ?
			(mtd_banks[idx-1] ? mtd_banks[idx-1]->size : 0) : 0);

		/* start to probe flash chips */
		mtd_banks[idx] = do_map_probe("jedec_probe", map_banks[idx]);
		if (mtd_banks[idx]) {
			mtd_banks[idx]->owner = THIS_MODULE;
			mtd_size += mtd_banks[idx]->size;
			num_banks++;
			debugk ("%s: bank %ld, name: %s, size: %d bytes \n",
				__FUNCTION__,
				num_banks,
				mtd_banks[idx]->name,
				mtd_banks[idx]->size);
		}
	}

	/* no supported flash chips found */
	if (!num_banks) {
		printk("PM82x: No supported flash chips found!\n");
		ret = -ENXIO;
		goto error_mem;
	}

#ifdef CONFIG_MTD_PARTITIONS
	/*
	 * Select static partition definitions
	 */
	if (flash_size == 0x800000) {
		part_banks[0].mtd_part	= pm82x_partitions_8M;
		part_banks[0].nums	= NB_OF(pm82x_partitions_8M);
	} else {
		part_banks[0].mtd_part	= pm82x_partitions_32M;
		part_banks[0].nums	= NB_OF(pm82x_partitions_32M);
	}
	part_banks[0].type	= "static image";

	for(idx = 0; idx < num_banks ; idx++) {
#ifdef CONFIG_MTD_CMDLINE_PARTS
		sprintf(mtdid, "%d", idx);
		n = parse_mtd_partitions(mtd_banks[idx],
						part_probes,
						&part_banks[idx].mtd_part,
						0);
		debugk ("%s: %d command line partitions on bank %s\n",
			__FUNCTION__, n, mtdid);
		if (n > 0) {
			part_banks[idx].type = "command line";
			part_banks[idx].nums = n;
		}
#endif	/* CONFIG_MTD_CMDLINE_PARTS */

		if (part_banks[idx].nums == 0) {
			printk (KERN_NOTICE
					"PM82x flash bank %d: no partition info "
					"available, registering whole device\n", idx);
			add_mtd_device(mtd_banks[idx]);
		} else {
			printk (KERN_NOTICE
					"PM82x flash bank %d: Using %s partition "
					"definition\n", idx, part_banks[idx].type);
			add_mtd_partitions (mtd_banks[idx],
					part_banks[idx].mtd_part,
					part_banks[idx].nums);
		}
	}
#else	/* ! CONFIG_MTD_PARTITIONS */
	printk (KERN_NOTICE "PM82x flash: registering %d flash banks "
			"at once\n", num_banks);

	for(idx = 0 ; idx < num_banks ; idx++) {
		add_mtd_device(mtd_banks[idx]);
	}
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

static void __exit cleanup_pm82x_mtd(void)
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

module_init(init_pm82x_mtd);
module_exit(cleanup_pm82x_mtd);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wolfgang Denk <wd@denx.de>");
MODULE_DESCRIPTION("MTD map driver for PM82x boards");
