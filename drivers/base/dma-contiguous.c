/*
 * Contiguous Memory Allocator for DMA mapping framework
 * Copyright (c) 2010-2011 by Samsung Electronics.
 * Written by:
 *	Marek Szyprowski <m.szyprowski@samsung.com>
 *	Michal Nazarewicz <mina86@mina86.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License or (at your optional) any later version of the license.
 */

/* for using pr_info */
#define pr_fmt(fmt) "\033[31mFunction = %s, Line = %d, cma: \033[m" fmt , __PRETTY_FUNCTION__, __LINE__

#ifdef CONFIG_CMA_DEBUG
#ifndef DEBUG
#  define DEBUG
#endif
#endif

#include <asm/page.h>
#include <asm/dma-contiguous.h>

#include <linux/memblock.h>
#include <linux/err.h>
#include <linux/mm.h>
#include <linux/mutex.h>
#include <linux/page-isolation.h>
#include <linux/sizes.h>
#include <linux/slab.h>
#include <linux/swap.h>
#include <linux/mm_types.h>
#include <linux/dma-contiguous.h>
#include "mdrv_types.h"
#include "mdrv_system.h"

#ifdef CONFIG_MP_ION_PATCH_MSTAR
	//move to dma-contiguous.h
#else
struct cma {
	unsigned long	base_pfn;
	unsigned long	count;
	unsigned long	*bitmap;
	struct mutex    lock;
};
#endif

#ifdef CONFIG_MP_CMA_PATCH_CMA_MSTAR_DRIVER_BUFFER
#define CMA_HEAP_MIUOFFSET_NOCARE (-1UL)
#endif

struct cma *dma_contiguous_default_area;

#ifdef CONFIG_CMA_SIZE_MBYTES
#define CMA_SIZE_MBYTES CONFIG_CMA_SIZE_MBYTES
#else
#define CMA_SIZE_MBYTES 0
#endif

/*
 * Default global CMA area size can be defined in kernel's .config.
 * This is usefull mainly for distro maintainers to create a kernel
 * that works correctly for most supported systems.
 * The size can be set in bytes or as a percentage of the total memory
 * in the system.
 *
 * Users, who want to set the size of global CMA area for their system
 * should use cma= kernel parameter.
 */
static const phys_addr_t size_bytes = CMA_SIZE_MBYTES * SZ_1M;
static phys_addr_t size_cmdline = -1;

#ifdef CONFIG_MP_CMA_PATCH_CMA_MSTAR_DRIVER_BUFFER
/* To store every mstar driver cma_buffer size, including default cma_buffer with index is 0 */
struct CMA_BootArgs_Config cma_config[MAX_CMA_AREAS];
struct device mstar_cma_device[MAX_CMA_AREAS];
int mstar_driver_boot_cma_buffer_num = 0;


EXPORT_SYMBOL(cma_config);
EXPORT_SYMBOL(mstar_cma_device);
EXPORT_SYMBOL(mstar_driver_boot_cma_buffer_num);

/*
  * if  start address has been specified, only convert it to cpu bus address;
  * else, find start address
  */
static bool GetReservedPhysicalAddr(unsigned char miu, unsigned long *start, 
    unsigned long *size)

{
    unsigned long alignstart = 0, alignfactor = pageblock_nr_pages*PAGE_SIZE;

    if(miu < 0 || miu > 3 || *size == 0)
        goto GetReservedPhysicalAddr_Fail;
    
    if(*start != CMA_HEAP_MIUOFFSET_NOCARE)
    {
        if(!IS_ALIGNED(*start, alignfactor))
            goto GetReservedPhysicalAddr_Fail;
    }

    if(!IS_ALIGNED(*size, alignfactor))
        goto GetReservedPhysicalAddr_Fail;
    
    if(*start == CMA_HEAP_MIUOFFSET_NOCARE)
        return true;
    
    switch (miu)
    {
        case 0:
        {            
            alignstart = *start + ARM_MIU0_BUS_BASE;          
            break;
        }
        case 1:
        {            
            alignstart = *start + ARM_MIU1_BUS_BASE;           
            break;
        }
        case 2:
        {            
            alignstart = *start + ARM_MIU2_BUS_BASE;         
            break;
        }
        case 3:
        {            
            alignstart = *start + ARM_MIU3_BUS_BASE;           
            break;
        }
        default:
            goto GetReservedPhysicalAddr_Fail;
    }    

    *start = alignstart;
    return true;

GetReservedPhysicalAddr_Fail:
    printk(KERN_ERR "error: invalid parameters\n");
    *start = 0;
    *size = 0;
    return false;
}

extern phys_addr_t arm_lowmem_limit;
static unsigned long _find_in_range(phys_addr_t start, phys_addr_t end, phys_addr_t size, phys_addr_t alignfactor)
{
    phys_addr_t ret = 0;

    if((arm_lowmem_limit > start) && (arm_lowmem_limit < end))
    {
        ret = memblock_find_in_range(start, arm_lowmem_limit, size, alignfactor);
        if(ret > 0)
            return ret;

        ret = memblock_find_in_range(arm_lowmem_limit, end, size, alignfactor);
        if(ret > 0)
            return ret;      
    }
    else if(end > start)
    {
        ret = memblock_find_in_range(start, end, size, alignfactor);
        if(ret > 0)
            return ret;
    }

    return ret;
}

static unsigned long find_start_addr(unsigned char miu, unsigned long size)
{
    unsigned long ret = 0;
    unsigned long alignfactor = pageblock_nr_pages*PAGE_SIZE;

    if(miu < 0 || miu > 3)
        return 0;
	
    switch (miu)
    {
        case 0:
        {
            ret = _find_in_range(ARM_MIU0_BUS_BASE, ARM_MIU1_BUS_BASE, size, alignfactor);		 
            break;
        }
        case 1:
        {
            ret = _find_in_range(ARM_MIU1_BUS_BASE, ARM_MIU2_BUS_BASE, size, alignfactor);        
            break;
        }
        case 2:
        {
            ret = _find_in_range(ARM_MIU2_BUS_BASE, ARM_MIU3_BUS_BASE, size, alignfactor);       
            break;
        }
        case 3:
        {
            ret = _find_in_range(ARM_MIU3_BUS_BASE, CMA_HEAP_MIUOFFSET_NOCARE, size, alignfactor);        
            break;
        }
        default:
            return 0;
    }  

    return ret;
}

static bool parse_heap_config(char *cmdline, struct CMA_BootArgs_Config * heapconfig)
{
    char * option;
    int leng = 0;
    bool has_start = false;

    if(cmdline == NULL)
        goto INVALID_HEAP_CONFIG;

    //printk(KERN_ERR "cmdline: %s\n", cmdline);

    option = strstr(cmdline, ",");
    leng = (int)(option - cmdline);
    if(leng > (CMA_HEAP_NAME_LENG-1))
        leng = CMA_HEAP_NAME_LENG -1;

    strncpy(heapconfig->name, cmdline, leng);
    heapconfig->name[leng] = '\0';

    option = strstr(cmdline, "st=");
    if(option != NULL)
        has_start = true;

    option = strstr(cmdline, "sz=");
    if(option == NULL)
        goto INVALID_HEAP_CONFIG;

    option = strstr(cmdline, "hid=");
    if(option == NULL)
        goto INVALID_HEAP_CONFIG;


    option = strstr(cmdline, "miu=");
    if(option == NULL)
        goto INVALID_HEAP_CONFIG;

    if(has_start)
    {
        sscanf(option, "miu=%d,hid=%d,sz=%lx,st=%lx", &heapconfig->miu, 
        &heapconfig->pool_id, &heapconfig->size, &heapconfig->start);    
    }
    else
    {
        sscanf(option, "miu=%d,hid=%d,sz=%lx", &heapconfig->miu, 
        &heapconfig->pool_id, &heapconfig->size);

        heapconfig->start = CMA_HEAP_MIUOFFSET_NOCARE;  
    }

    if(!GetReservedPhysicalAddr(heapconfig->miu, &heapconfig->start, &heapconfig->size))
        goto INVALID_HEAP_CONFIG;

    //printk(KERN_ERR "miu %d, pool id %d, size %lu, start %lu\n", heapconfig->miu, 
    //heapconfig->pool_id, heapconfig->size, heapconfig->miu_offset);

    return true;

INVALID_HEAP_CONFIG:
	heapconfig->size = 0;
	return false;
}

int __init setup_cma0_info(char *cmdline)
{
    if(!parse_heap_config(cmdline, &cma_config[mstar_driver_boot_cma_buffer_num]))        
        printk(KERN_ERR "error: cma0 args invalid\n");    
    else
        mstar_driver_boot_cma_buffer_num++;

    return 0;
}

int __init setup_cma1_info(char *cmdline)
{
    if(!parse_heap_config(cmdline, &cma_config[mstar_driver_boot_cma_buffer_num]))        
        printk(KERN_ERR "error: cma1 args invalid\n");    
    else
        mstar_driver_boot_cma_buffer_num++;

    return 0;
}

int __init setup_cma2_info(char *cmdline)
{
    if(!parse_heap_config(cmdline, &cma_config[mstar_driver_boot_cma_buffer_num]))        
        printk(KERN_ERR "error: cma2 args invalid\n");    
    else
        mstar_driver_boot_cma_buffer_num++;

    return 0;
}

int __init setup_cma3_info(char *cmdline)
{
    if(!parse_heap_config(cmdline, &cma_config[mstar_driver_boot_cma_buffer_num]))        
        printk(KERN_ERR "error: cma3 args invalid\n");    
    else
        mstar_driver_boot_cma_buffer_num++;

    return 0;
}

int __init setup_cma4_info(char *cmdline)
{
    if(!parse_heap_config(cmdline, &cma_config[mstar_driver_boot_cma_buffer_num]))        
        printk(KERN_ERR "error: cma4 args invalid\n");    
    else
        mstar_driver_boot_cma_buffer_num++;

    return 0;
}

int __init setup_cma5_info(char *cmdline)
{
    if(!parse_heap_config(cmdline, &cma_config[mstar_driver_boot_cma_buffer_num]))        
        printk(KERN_ERR "error: cma5 args invalid\n");    
    else
        mstar_driver_boot_cma_buffer_num++;

    return 0;
}

early_param("CMA0", setup_cma0_info);
early_param("CMA1", setup_cma1_info);
early_param("CMA2", setup_cma2_info);
early_param("CMA3", setup_cma3_info);
early_param("CMA4", setup_cma4_info);
early_param("CMA5", setup_cma5_info);
#endif

static int __init early_cma(char *p)
{
    pr_debug("%s(%s)\n", __func__, p);
    size_cmdline = memparse(p, &p);
    return 0;
}
early_param("cma", early_cma);

#ifdef CONFIG_CMA_SIZE_PERCENTAGE

static phys_addr_t __init __maybe_unused cma_early_percent_memory(void)
{
	struct memblock_region *reg;
	unsigned long total_pages = 0;

	/*
	 * We cannot use memblock_phys_mem_size() here, because
	 * memblock_analyze() has not been called yet.
	 */
	for_each_memblock(memory, reg)
		total_pages += memblock_region_memory_end_pfn(reg) -
			       memblock_region_memory_base_pfn(reg);

	return (total_pages * CONFIG_CMA_SIZE_PERCENTAGE / 100) << PAGE_SHIFT;
}

#else

static inline __maybe_unused phys_addr_t cma_early_percent_memory(void)
{
	return 0;
}

#endif

static void count_cma_area_free_page_num(struct cma *counted_cma)
{
	int count_cma_free_page_count       = 0;
	int count_cma_free_start			= 0;
	int count_cma_bitmap_start_zero    	= 0;
	int count_cma_bitmap_end_zero    	= 0;

	int debug = 0;
#if 0
	//for debug usage
	if(counted_cma->base_pfn == 0xa8400){
	//if(0){//(counted_cma->base_pfn == 0x26000){
		debug = 1;
		int i = 0;
		int *ptr = counted_cma->bitmap;
		printk("ptr = %p \n",ptr);
		for(i = 0 ; i < 2048/4 ; i ++){
			printk(" %x -",*(ptr + i));
			if(i % 16 == 0)
				printk("\n");
		}

	}
#endif

	printk("\033[32mcma_area having \033[m");
	for(;;)
	{
		count_cma_bitmap_start_zero = find_next_zero_bit(counted_cma->bitmap, counted_cma->count, count_cma_free_start);

		if(count_cma_bitmap_start_zero >= counted_cma->count)
			break;
		if(debug)
			printk("######### count_cma_bitmap_start_zero=%x \n",count_cma_bitmap_start_zero);
		count_cma_free_start = count_cma_bitmap_start_zero + 1;
		
		count_cma_bitmap_end_zero = find_next_bit(counted_cma->bitmap, counted_cma->count, count_cma_free_start);

		if(debug)
			printk("######### count_cma_bitmap_end_zero=%x \n",count_cma_bitmap_end_zero);

		if(count_cma_bitmap_end_zero >= counted_cma->count)
		{
			count_cma_free_page_count += (counted_cma->count - count_cma_bitmap_start_zero);
			break;
		}

		count_cma_free_page_count += (count_cma_bitmap_end_zero - count_cma_bitmap_start_zero);

		count_cma_free_start = count_cma_bitmap_end_zero + 1;
		
		if(count_cma_free_start >= counted_cma->count)
			break;
	}
	printk("\033[32m%d free pages\033[m\n", count_cma_free_page_count);

	return;
}

static struct cma_reserved {
	phys_addr_t start;
	unsigned long size;
	struct device *dev;
} cma_reserved[MAX_CMA_AREAS] __initdata;
static unsigned cma_reserved_count __initdata;

/**
 * dma_contiguous_reserve() - reserve area for contiguous memory handling
 * @limit: End address of the reserved memory (optional, 0 for any).
 *
 * This function reserves memory from early allocator. It should be
 * called by arch specific code once the early allocator (memblock or bootmem)
 * has been activated and all other subsystems have already allocated/reserved
 * memory.
 */
void __init dma_contiguous_reserve(phys_addr_t limit)
{
	phys_addr_t selected_size = 0;
    int ret = 0;

#ifdef CONFIG_MP_CMA_PATCH_CMA_MSTAR_DRIVER_BUFFER
	int dma_declare_index = 0;

    /*
     *  mstar_cma_device store the device with successfully parsed cma buffer info
     */
	struct device *declare_mstar_cma_device = &mstar_cma_device[0];
#endif

	pr_debug("%s(limit %08lx)\n", __func__, (unsigned long)limit);

	if(size_cmdline != -1)
	{
		selected_size = size_cmdline;
	}
	else
	{
#ifdef CONFIG_CMA_SIZE_SEL_MBYTES
		selected_size = size_bytes;	// set in .config
#elif defined(CONFIG_CMA_SIZE_SEL_PERCENTAGE)
		selected_size = cma_early_percent_memory();	// set in .config
#elif defined(CONFIG_CMA_SIZE_SEL_MIN)
		selected_size = min(size_bytes, cma_early_percent_memory());	// both CONFIG_CMA_SIZE_MBYTES and CONFIG_CMA_SIZE_PERCENTAGE can be set in .config
#elif defined(CONFIG_CMA_SIZE_SEL_MAX)
		selected_size = max(size_bytes, cma_early_percent_memory());
#endif
	}

	if(selected_size)
	{
		pr_info("reserving %ld MiB for global area\n", (unsigned long)selected_size / SZ_1M);
		ret = dma_declare_contiguous(NULL, selected_size, 0, limit);
       
#ifdef CONFIG_MP_CMA_PATCH_CMA_MSTAR_DRIVER_BUFFER
        if(ret)
        {
            printk(KERN_ERR "error: declare default cma config fail\n");
            BUG_ON(ret);
        }
#endif
    }

#ifdef CONFIG_MP_CMA_PATCH_CMA_MSTAR_DRIVER_BUFFER
	/*
	 * add cma buffer from bootargs, and assigne it to the specific device
	 * cma buffer is not limited in low memory, also can locate in high memory 
	 */
    
	while(dma_declare_index < mstar_driver_boot_cma_buffer_num)
	{
		pr_info("reserving %ld MiB for mstar_driver\n", cma_config[dma_declare_index].size / SZ_1M);       

        BUG_ON(cma_config[dma_declare_index].size == 0);
        if(CMA_HEAP_MIUOFFSET_NOCARE == cma_config[dma_declare_index].start)
        {
			cma_config[dma_declare_index].start = find_start_addr(cma_config[dma_declare_index].miu, cma_config[dma_declare_index].size);

			BUG_ON(cma_config[dma_declare_index].start == 0);
        }
         
#if 0
        printk(KERN_ERR "index %d, name %s, miu %d, start %lu size %lu, pool id %d\n",dma_declare_index,
            cma_config[dma_declare_index].name, (int)cma_config[dma_declare_index].miu, 
            cma_config[dma_declare_index].start, cma_config[dma_declare_index].size,
            cma_config[dma_declare_index].pool_id);
#endif
        //check if the reserved memory allocated across 2 memory zones: a part of it in normal zone, the other in high memory
        if(cma_config[dma_declare_index].start > 0)
        {
            if((cma_config[dma_declare_index].start < arm_lowmem_limit)
                && (cma_config[dma_declare_index].start + cma_config[dma_declare_index].size > arm_lowmem_limit))
            {
                printk(KERN_ERR "Warning: reserved memory allocated across 2 memory zones!!!=========\n");				
            }
        }

        ret = dma_declare_contiguous(declare_mstar_cma_device, cma_config[dma_declare_index].size,
        cma_config[dma_declare_index].start, limit);
		declare_mstar_cma_device->coherent_dma_mask = ~0;	// not sure, this mask will be used in __dma_alloc while doing cma_alloc, 0xFFFFFFFF is for NULL device

        if(ret)
        {
            printk(KERN_ERR "error: reserve memory fail, start %lu size %lu\n", 
            cma_config[dma_declare_index].start, cma_config[dma_declare_index].size);
            BUG_ON(ret);
        }

		declare_mstar_cma_device->init_name = cma_config[dma_declare_index].name;
		dma_declare_index++;
        declare_mstar_cma_device++;
	}    
#endif
};

static DEFINE_MUTEX(cma_mutex);

static __init int cma_activate_area(struct cma *cma)
{
	unsigned long base_pfn = cma->base_pfn, pfn = base_pfn;
	unsigned i = cma->count >> pageblock_order;	// one pageblock having 1024 pages(4MB), i means having how much pageblock
	struct zone *zone;

	WARN_ON_ONCE(!pfn_valid(pfn));
	zone = page_zone(pfn_to_page(pfn));
	printk("\033[35mFunction = %s, pfn: 0x%X is in zone %s\033[m\n", __PRETTY_FUNCTION__, (unsigned int)pfn, zone->name);

	do {
		unsigned j;
		base_pfn = pfn;

		/* check if all pages in this pageblock are vaild and in the same zone i
		 * pageblock_nr_pages is 1024 pages
		 */
		for (j = pageblock_nr_pages; j; --j, pfn++) {
			WARN_ON_ONCE(!pfn_valid(pfn));
			if (page_zone(pfn_to_page(pfn)) != zone)
				return -EINVAL;
		}
		//printk("\033[35mFunction = %s, Line = %d, pfn is 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, (unsigned int)pfn);
		//printk("\033[35mFunction = %s, Line = %d, base_pfn is 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, (unsigned int)base_pfn);
		init_cma_reserved_pageblock(pfn_to_page(base_pfn));
	} while (--i);

#ifdef CONFIG_MP_CMA_PATCH_CMA_AGGRESSIVE_ALLOC
	adjust_managed_cma_page_count(zone, cma->count); 
#endif
	
	mutex_init(&cma->lock);
	return 0;
}

static void clear_cma_bitmap(struct cma *cma, unsigned long pfn, int count)
{
       mutex_lock(&cma->lock);
       bitmap_clear(cma->bitmap, pfn - cma->base_pfn, count);
       mutex_unlock(&cma->lock);
}

static __init struct cma *cma_create_area(unsigned long base_pfn,
				     unsigned long count)
{
	int bitmap_size = BITS_TO_LONGS(count) * sizeof(long);	// count is page_num, BITS_TO_LONGS(count) coverts count to ~ long size. bitmap_size is ~ byte the bitmap needs
	struct cma *cma;
	int ret = -ENOMEM;

	pr_debug("%s(base %08lx, count %lx, bitmap_size %d(bytes))\n", __func__, base_pfn, count, bitmap_size);

	cma = kmalloc(sizeof *cma, GFP_KERNEL);
	if (!cma)
		return ERR_PTR(-ENOMEM);

	cma->base_pfn = base_pfn;
	cma->count = count;
	cma->bitmap = kzalloc(bitmap_size, GFP_KERNEL);

	if (!cma->bitmap)
		goto no_mem;

	ret = cma_activate_area(cma);
	if (ret)        
		goto error;

	pr_debug("%s: returned %p\n", __func__, (void *)cma);
	return cma;

error:
	kfree(cma->bitmap);
no_mem:
	kfree(cma);
	return ERR_PTR(ret);
}

// cma_reserved_count may not be 0, this means some cma buffer is pre-reserved, cma_init_reserved_areas will clear these pre-reserved areas, and mark as reserved cma_buffer

static int __init cma_init_reserved_areas(void)
{
	struct cma_reserved *r = cma_reserved;
	unsigned i = cma_reserved_count;

	pr_debug("%s()\n", __func__);

	for (; i; --i, ++r) {
		struct cma *cma;
		pr_info("init No-%d cma_buffer\n", (cma_reserved_count - i + 1));
		cma = cma_create_area(PFN_DOWN(r->start),
				      r->size >> PAGE_SHIFT);
		if (!IS_ERR(cma))
			dev_set_cma_area(r->dev, cma);
		else
		{
		    pr_info("cma_create_area No-%d cma struct fail\n", (cma_reserved_count - i + 1));
		    BUG_ON(1);
		}
	}
	return 0;
}
core_initcall(cma_init_reserved_areas);

/**
 * dma_declare_contiguous() - reserve area for contiguous memory handling
 *			      for particular device
 * @dev:   Pointer to device structure.
 * @size:  Size of the reserved memory.
 * @base:  Start address of the reserved memory (optional, 0 for any).
 * @limit: End address of the reserved memory (optional, 0 for any).
 *
 * This function reserves memory for specified device. It should be
 * called by board specific code when early allocator (memblock or bootmem)
 * is still activate.
 */
int __init dma_declare_contiguous(struct device *dev, phys_addr_t size,
				  phys_addr_t base, phys_addr_t limit)
{
	struct cma_reserved *r = &cma_reserved[cma_reserved_count];
	phys_addr_t alignment;
	phys_addr_t addr;

	pr_debug("%s(size %lx, base %08lx, limit %08lx)\n", __func__,
		 (unsigned long)size, (unsigned long)base,
		 (unsigned long)limit);

	pr_info("cma_reserved_count is %d\n", cma_reserved_count);
	pr_info("base is 0x%X\n", (unsigned int)base);
	pr_info("limit is 0x%X\n", (unsigned int)limit);

	/* Sanity checks */
	if (cma_reserved_count == ARRAY_SIZE(cma_reserved)) {
		pr_err("Not enough slots for CMA reserved regions!\n");
		return -ENOSPC;
	}

	if (!size)
		return -EINVAL;

	/* Sanitise input arguments */
	alignment = PAGE_SIZE << max(MAX_ORDER - 1, pageblock_order);	// 4MB alignment
	base = ALIGN(base, alignment);	// to next 4MB address, ex: 0x20200000 -> 0x20400000
	size = ALIGN(size, alignment);	// 							  0x200000 ->   0x400000
	limit &= ~(alignment - 1);

	/* Reserve memory */
	if(base)
	{
		/* here means you are really want to reserve a memory region 
		 * from "base" to "base + size", so we will not search a memory region here.
		 * Instead, we directly reserve the memory from "base" to "base + size"
		 */

		/* first check if already in reserved region, 
		 * then add it to reserved region. if the memory region
		 * from "base" to "base + size" is already in a reserved region
		 * then declare will be failed
		 */
		if (memblock_is_region_reserved(base, size) || memblock_reserve(base, size) < 0)
		{
			printk("\033[35mFunction = %s, declare cma_buffer with base, but it can not be reserved\033[m\n", __PRETTY_FUNCTION__);
			base = -EBUSY;
			goto err;
		}
	}
	else
	{
		/*
		 * Use __memblock_alloc_base() since
		 * memblock_alloc_base() panic()s.
		 */

		/* Walks over free (memory && !reserved) areas of memblock in reverse to find memory and reserve it*/
		addr = __memblock_alloc_base(size, alignment, limit);
		if (!addr) {
			base = -ENOMEM;
			goto err;
		} else {
			base = addr;
		}
	}

	/*
	 * Each reserved area must be initialised later, when more kernel
	 * subsystems (like slab allocator) are available.
	 */
	r->start = base;
	r->size = size;
	r->dev = dev;
	cma_reserved_count++;
	pr_info("successfully reserved %ld MiB at %08lx\n", (unsigned long)size / SZ_1M,
		(unsigned long)base);

	/* Architecture specific contiguous memory fixup. */
#if defined(CONFIG_MP_CMA_PATCH_CMA_64_BIT_TEMP_MODIFICATION)
#ifndef CONFIG_ARM64
	/* cause we do not find the corresponding remap action in paging_init for 64_bit kernel */
	dma_contiguous_early_fixup(base, size);
#endif
#endif
	return 0;
err:
	pr_err("CMA: failed to reserve %ld MiB\n", (unsigned long)size / SZ_1M);
	return base;
}

/**
 * dma_alloc_from_contiguous() - allocate pages from contiguous area
 * @dev:   Pointer to device for which the allocation is performed.
 * @count: Requested number of pages.
 * @align: Requested alignment of pages (in PAGE_SIZE order).	// request 2^align pages
 *
 * This function allocates memory buffer for specified device. It uses
 * device specific contiguous memory area if available or the default
 * global one. Requires architecture specific get_dev_cma_area() helper
 * function.
 */
struct page *dma_alloc_from_contiguous(struct device *dev, int count,
				       unsigned int align)
{
	unsigned long mask, pfn, pageno, start = 0;
	struct cma *cma = dev_get_cma_area(dev);
	struct page *page = NULL;
	int ret;

	if (!cma || !cma->count)
		return NULL;

	if (align > CONFIG_CMA_ALIGNMENT)
	{
		// if size > 2^8 pages ==> align will be > 8
		align = CONFIG_CMA_ALIGNMENT;
	}

	//printk("\033[31m%s(alloc %d pages, align %d)\033[m\n", __func__, count, align);

	if (!count)
		return NULL;

	mask = (1 << align) - 1;

	printk("\033[32m[%s] Before dma_alloc_from_contiguous \033[m", current->comm);
	count_cma_area_free_page_num(cma);

	for (;;) {
		 mutex_lock(&cma->lock);
		 
		// bitmap_find_next_zero_area will return an index which points a zero-bit in bitmap, whose following "count" bits are all zero-bit(from pageno to pageno+count are all free pages)
		pageno = bitmap_find_next_zero_area(cma->bitmap, cma->count,
						    start, count, mask);
		if (pageno >= cma->count)	// no such continuous area
		{
			mutex_unlock(&cma->lock);
			break;
		}
		bitmap_set(cma->bitmap, pageno, count);
		/*
		* It's safe to drop the lock here. We've marked this region for
		* our exclusive use. If the migration fails we will take the
		* lock again and unmark it.
		*/
		mutex_unlock(&cma->lock);

		pfn = cma->base_pfn + pageno;
		mutex_lock(&cma_mutex);
		ret = alloc_contig_range(pfn, pfn + count, MIGRATE_CMA);
		mutex_unlock(&cma_mutex);
		if (ret == 0) {
			page = pfn_to_page(pfn);
#ifdef CONFIG_MP_CMA_PATCH_CMA_AGGRESSIVE_ALLOC			
			adjust_managed_cma_page_count(page_zone(page), -count); 
#endif
			break;
		} else if (ret != -EBUSY) {
			clear_cma_bitmap(cma, pfn, count);
			break;
		}
		clear_cma_bitmap(cma, pfn, count);
		pr_debug("%s(): memory range at %p is busy, retrying\n",
			 __func__, pfn_to_page(pfn));
		/* try again with a bit different memory target */
		start = pageno + mask + 1;
	}

	//printk("\033[32mAlloc Bus_Addr at 0x%X\033[m\n", (unsigned int)(pfn << PAGE_SHIFT)); // joe.liu
	//printk("\033[32m[%s] After dma_alloc_from_contiguous \033[m", current->comm);
	//count_cma_area_free_page_num(cma);
	
	return page;
}

/**
 * dma_release_from_contiguous() - release allocated pages
 * @dev:   Pointer to device for which the pages were allocated.
 * @pages: Allocated pages.	// start pfn
 * @count: Number of allocated pages.
 *
 * This function releases memory allocated by dma_alloc_from_contiguous().
 * It returns false when provided pages do not belong to contiguous area and
 * true otherwise.
 */
bool dma_release_from_contiguous(struct device *dev, struct page *pages,
				 int count)
{
	struct cma *cma = dev_get_cma_area(dev);
	unsigned long pfn;

	if (!cma || !pages)
		return false;

	pr_debug("%s(page %p)\n", __func__, (void *)pages);

	pfn = page_to_pfn(pages);

	if (pfn < cma->base_pfn || pfn >= cma->base_pfn + cma->count)
		return false;

	VM_BUG_ON(pfn + count > cma->base_pfn + cma->count);

	printk("\033[32m[%s] Before dma_release_from_contiguous \033[m", current->comm);
	count_cma_area_free_page_num(cma);

	free_contig_range(pfn, count);
	clear_cma_bitmap(cma, pfn, count);

#ifdef CONFIG_MP_CMA_PATCH_CMA_AGGRESSIVE_ALLOC
	adjust_managed_cma_page_count(page_zone(pages), count); 
#endif

	printk("\033[32m[%s] After dma_release_from_contiguous \033[m", current->comm);
	count_cma_area_free_page_num(cma);

	return true;
}


#ifdef CONFIG_MP_ION_PATCH_MSTAR
/**
 * dma_alloc_from_contiguous_addr() - allocate pages from contiguous area from specified start address
 * @dev:   Pointer to device for which the allocation is performed.
 * @start: Start bus address to allocate.
 * @count: Requested number of pages.
 * @align: Requested alignment of pages (in PAGE_SIZE order).	// request 2^align pages
 *
 * This function allocates memory buffer for specified device. It uses
 * device specific contiguous memory area if available or the default
 * global one. Requires architecture specific get_dev_cma_area() helper
 * function.
 */
void *dma_alloc_from_contiguous_addr(struct device *dev, unsigned long start,
																int count, unsigned int align)
{
	unsigned long mask;
	struct cma *cma = dev_get_cma_area(dev);
	struct page *page = NULL;
	unsigned long start_pfn = start >> PAGE_SHIFT;
	int ret,next_bit;

	if (!cma || !cma->count)
		return NULL;
	
	if (!count)
		return NULL;

	//count = count >> PAGE_SHIFT;
	if(start_pfn < cma->base_pfn || start_pfn >= cma->base_pfn + cma->count || start_pfn + count > cma->base_pfn + cma->count){
		printk("invalide start address, start_pfn = 0x%X, count = 0x%X, cma[0x%X, 0x%X] \n", (unsigned int)start_pfn, (unsigned int)count, (unsigned int)cma->base_pfn, (unsigned int)cma->count);
		return NULL;
	}

	if (align > CONFIG_CMA_ALIGNMENT)
	{
		// if size > 2^8 pages ==> align will be > 8
		align = CONFIG_CMA_ALIGNMENT;
	}

	mask = (1 << align) - 1;
	start_pfn = (start_pfn + mask - 1) & ~mask;
	printk("\033[31m%s(alloc %d pages, start 0x%X, start pfn 0x%X)\033[m\n", __func__, count, (unsigned int)start, (unsigned int)start_pfn);

	printk("\033[32m[%s] Before dma_alloc_from_contiguous \033[m", current->comm);
	count_cma_area_free_page_num(cma);

	mutex_lock(&cma->lock);
	next_bit = find_next_bit(cma->bitmap, cma->count, start_pfn - cma->base_pfn);
	if(next_bit - (start_pfn - cma->base_pfn) < count)	//can not allocate [start_pfn, start_pfn + count)
	{
		mutex_unlock(&cma->lock);
		printk("Cannot allocte [0x%X, 0x%X] due to offset 0x%X is occupied! \n",
			(unsigned int)start_pfn, (unsigned int)(start_pfn + count), (unsigned int)next_bit);
		goto RET;
	}
	bitmap_set(cma->bitmap, start_pfn - cma->base_pfn, count);
	mutex_unlock(&cma->lock);

	mutex_lock(&cma_mutex);
	ret = alloc_contig_range(start_pfn, start_pfn + count, MIGRATE_CMA);
	mutex_unlock(&cma_mutex);
	if (ret == 0) {
		page = pfn_to_page(start_pfn);
#ifdef CONFIG_MP_CMA_PATCH_CMA_AGGRESSIVE_ALLOC			
		adjust_managed_cma_page_count(page_zone(page), -count); 
#endif		
	} else if (ret != -EBUSY) {
		clear_cma_bitmap(cma, start_pfn, count);
		printk("allocte [%lx, %lx] failed \n",start_pfn , start_pfn + count);
		goto RET;
	}
	else{
		printk("######################## ret = %d \n",ret);	
		clear_cma_bitmap(cma, start_pfn, count);
		WARN_ON(1);
	}
	
	printk("\033[32mAlloc Bus_Addr at 0x%X\033[m\n", (unsigned int)(start_pfn << PAGE_SHIFT)); // joe.liu
	printk("\033[32m[%s] After dma_alloc_from_contiguous \033[m", current->comm);
	count_cma_area_free_page_num(cma);
	
RET:
	return page;
}
#endif

