/*
 * Hibernation support specific for ARM
 *
 * Copyright (C) 2010 Nokia Corporation
 * Copyright (C) 2010 Texas Instruments, Inc.
 * Copyright (C) 2006 Rafael J. Wysocki <rjw <at> sisk.pl>
 *
 * Contact: Hiroshi DOYU <Hiroshi.DOYU <at> nokia.com>
 *
 * License terms: GNU General Public License (GPL) version 2
 */
#include <linux/module.h>
#include <linux/suspend.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/major.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <asm/delay.h>
#include <linux/power/aw_pm.h>
#include <linux/module.h>
#include <asm/io.h>
#include <asm/mach/map.h>
#include <asm/cacheflush.h>
#include <asm/tlbflush.h>
#include <asm/pgtable.h>
#include "pm_i.h"

/*
 * "Linux" PTE definitions.
 *
 * We keep two sets of PTEs - the hardware and the linux version.
 * This allows greater flexibility in the way we map the Linux bits
 * onto the hardware tables, and allows us to have YOUNG and DIRTY
 * bits.
 *
 * The PTE table pointer refers to the hardware entries; the "Linux"
 * entries are stored 1024 bytes below.
 */

#define L_PTE_WRITE		(1 << 7)
#define L_PTE_EXEC		(1 << 9)
#define PAGE_TBL_ADDR 		(0xc0004000)

struct saved_mmu_level_one {
	u32 vaddr;
	u32 entry_val;
};

/* Used in mem_context_asm.S */
#define SYS_CONTEXT_SIZE (2)
#define SVC_CONTEXT_SIZE (2)	//do not need backup r14? reason?
#define FIQ_CONTEXT_SIZE (7)
#define ABT_CONTEXT_SIZE (2 )
#define IRQ_CONTEXT_SIZE (2)
#define UND_CONTEXT_SIZE (2)
#define MON_CONTEXT_SIZE (2)

#define EMPTY_CONTEXT_SIZE (11 * sizeof(u32))


unsigned long saved_context_r13_sys[SYS_CONTEXT_SIZE];
unsigned long saved_cpsr_svc;
unsigned long saved_context_r12_svc[SVC_CONTEXT_SIZE];
unsigned long saved_spsr_svc;   
unsigned long saved_context_r13_fiq[FIQ_CONTEXT_SIZE];
unsigned long saved_spsr_fiq;
unsigned long saved_context_r13_abt[ABT_CONTEXT_SIZE];
unsigned long saved_spsr_abt;
unsigned long saved_context_r13_irq[IRQ_CONTEXT_SIZE];
unsigned long saved_spsr_irq;
unsigned long saved_context_r13_und[UND_CONTEXT_SIZE];
unsigned long saved_spsr_und;
unsigned long saved_context_r13_mon[MON_CONTEXT_SIZE];
unsigned long saved_spsr_mon;
unsigned long saved_empty_context_svc[EMPTY_CONTEXT_SIZE];

static struct saved_mmu_level_one backup_tbl[1];

/* References to section boundaries */
extern const void __nosave_begin, __nosave_end;

static inline pmd_t *pmd_off(pgd_t *pgd, unsigned long virt)
{
	return pmd_offset(pgd, virt);
}

static inline pmd_t *pmd_off_k(unsigned long virt)
{
	pgd_t *pgd;
	pgd = pgd_offset_k(virt);
	return pmd_off(pgd, virt);
}

struct mem_type {
	unsigned int prot_pte;
	unsigned int prot_l1;
	unsigned int prot_sect;
	unsigned int domain;
};


#define PROT_PTE_DEVICE		L_PTE_PRESENT|L_PTE_YOUNG|L_PTE_DIRTY|L_PTE_WRITE
#define PROT_SECT_DEVICE	PMD_TYPE_SECT|PMD_SECT_AP_WRITE

static struct mem_type mem_types[] = {
	[MT_DEVICE] = {		  /* Strongly ordered / ARMv6 shared device */
		.prot_pte	= PROT_PTE_DEVICE | L_PTE_MT_DEV_SHARED |
				  L_PTE_SHARED,
		.prot_l1	= PMD_TYPE_TABLE,
		.prot_sect	= PROT_SECT_DEVICE | PMD_SECT_S,
		.domain		= DOMAIN_IO,
	},
	[MT_DEVICE_NONSHARED] = { /* ARMv6 non-shared device */
		.prot_pte	= PROT_PTE_DEVICE | L_PTE_MT_DEV_NONSHARED,
		.prot_l1	= PMD_TYPE_TABLE,
		.prot_sect	= PROT_SECT_DEVICE,
		.domain		= DOMAIN_IO,
	},
	[MT_DEVICE_CACHED] = {	  /* ioremap_cached */
		.prot_pte	= PROT_PTE_DEVICE | L_PTE_MT_DEV_CACHED,
		.prot_l1	= PMD_TYPE_TABLE,
		.prot_sect	= PROT_SECT_DEVICE | PMD_SECT_WB,
		.domain		= DOMAIN_IO,
	},	
	[MT_DEVICE_WC] = {	/* ioremap_wc */
		.prot_pte	= PROT_PTE_DEVICE | L_PTE_MT_DEV_WC,
		.prot_l1	= PMD_TYPE_TABLE,
		.prot_sect	= PROT_SECT_DEVICE,
		.domain		= DOMAIN_IO,
	},
	[MT_UNCACHED] = {
		.prot_pte	= PROT_PTE_DEVICE,
		.prot_l1	= PMD_TYPE_TABLE,
		.prot_sect	= PMD_TYPE_SECT | PMD_SECT_XN,
		.domain		= DOMAIN_IO,
	},
	[MT_CACHECLEAN] = {
		.prot_sect = PMD_TYPE_SECT | PMD_SECT_XN,
		.domain    = DOMAIN_KERNEL,
	},
	[MT_MINICLEAN] = {
		.prot_sect = PMD_TYPE_SECT | PMD_SECT_XN | PMD_SECT_MINICACHE,
		.domain    = DOMAIN_KERNEL,
	},
	[MT_LOW_VECTORS] = {
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY |
				L_PTE_EXEC,
		.prot_l1   = PMD_TYPE_TABLE,
		.domain    = DOMAIN_USER,
	},
	[MT_HIGH_VECTORS] = {
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY |
				L_PTE_USER | L_PTE_EXEC,
		.prot_l1   = PMD_TYPE_TABLE,
		.domain    = DOMAIN_USER,
	},
	[MT_MEMORY] = {
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY |
				L_PTE_WRITE | L_PTE_EXEC,
		.prot_l1   = PMD_TYPE_TABLE,
		.prot_sect = PMD_TYPE_SECT | PMD_SECT_AP_WRITE,
		.domain    = DOMAIN_KERNEL,
	},
	[MT_ROM] = {
		.prot_sect = PMD_TYPE_SECT,
		.domain    = DOMAIN_KERNEL,
	},
	[MT_MEMORY_NONCACHED] = {
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY |
				L_PTE_WRITE | L_PTE_EXEC | L_PTE_MT_BUFFERABLE,
		.prot_l1   = PMD_TYPE_TABLE,
		.prot_sect = PMD_TYPE_SECT | PMD_SECT_AP_WRITE,
		.domain    = DOMAIN_KERNEL,
	},
	[MT_MEMORY_DTCM] = {
		.prot_pte	= L_PTE_PRESENT | L_PTE_YOUNG |
		                  L_PTE_DIRTY | L_PTE_WRITE,
		.prot_l1	= PMD_TYPE_TABLE,
		.prot_sect	= PMD_TYPE_SECT | PMD_SECT_XN,
		.domain		= DOMAIN_KERNEL,
	},
	[MT_MEMORY_ITCM] = {
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY |
				L_PTE_USER | L_PTE_EXEC,
		.prot_l1   = PMD_TYPE_TABLE,
		.domain    = DOMAIN_IO,
	},
};

struct saved_context saved_context;

/*
 * before enter suspend, need to clear mem flag, 
 * in case the flag is failed to clear by resume code 
 * 
*/
void clear_mem_flag(void)
{
	__u32 saved_flag = *(volatile __u32 *)(PERMANENT_REG);
	saved_flag &= 0xfffffffe;
	*(volatile __u32 *)(PERMANENT_REG) = saved_flag;
	return;
}


/*
 * Create the page directory entries and any necessary
 * page tables for the mapping specified by `md'.  We
 * are able to cope here with varying sizes and address
 * offsets, and we take full advantage of sections and
 * supersections.
 */
void create_mapping(struct map_desc *md)
{
	unsigned long phys, addr, length;
	const struct mem_type *type;

	//busy_waiting();
	type = &mem_types[md->type];

	addr = md->virtual & PAGE_MASK;
	phys = (unsigned long)__pfn_to_phys(md->pfn);
	length = PAGE_ALIGN(md->length + (md->virtual & ~PAGE_MASK));

	//busy_waiting();
	if (type->prot_l1 == 0 && ((addr | phys | length) & ~SECTION_MASK)) {
		printk(KERN_WARNING "BUG: map for 0x%08llx at 0x%08lx can not "
		       "be mapped using pages, ignoring.\n",
		       (long long)__pfn_to_phys(md->pfn), addr);
		return;
	}

	//busy_waiting();
	//__cpuc_flush_kern_all();
	//__cpuc_flush_user_all();
	*((volatile __u32 *)(PAGE_TBL_ADDR)) = 0xc4a;
	//clean cache
	__cpuc_coherent_user_range((long unsigned int)(PAGE_TBL_ADDR), (long unsigned int)(PAGE_TBL_ADDR + (sizeof(u32)) - 1));
	//actually, not need, just for test.
	//flush_tlb_all();

	return;
}

void save_mapping(unsigned long vaddr)
{
	unsigned long addr;
	pgd_t *pgd = 0;
	pmd_t *pmd = 0;

	//busy_waiting();
	addr = vaddr & PAGE_MASK;
	pgd = pgd_offset_k(addr);
	pmd = pmd_offset(pgd, addr);

	//__cpuc_flush_kern_all();
	backup_tbl[0].vaddr = addr;
	backup_tbl[0].entry_val = *((volatile __u32 *)(PAGE_TBL_ADDR));
	//flush_tlb_all();

	return;
}

void restore_mapping(unsigned long vaddr)
{
	unsigned long addr;
	pgd_t *pgd = 0;
	pmd_t *pmd = 0;
	
	addr = vaddr & PAGE_MASK;
	pgd = pgd_offset_k(addr);
	pmd = pmd_offset(pgd, addr);

	if(addr != backup_tbl[0].vaddr){
		while(1);
		return;
	}
	//__cpuc_flush_kern_all();
	*((volatile __u32 *)(PAGE_TBL_ADDR)) = backup_tbl[0].entry_val;
	//clean cache
	__cpuc_coherent_user_range((long unsigned int)(PAGE_TBL_ADDR), (long unsigned int)(PAGE_TBL_ADDR + (sizeof(u32)) - 1));
	flush_tlb_all();

	return;
}


void save_processor_state(void)
{
	preempt_disable();
	__save_processor_state(&saved_context);
}

void restore_processor_state(void)
{
	__restore_processor_state(&saved_context);
	preempt_enable();
}

void restore_processor_ttbr0(void)
{
	asm volatile ("mcr p15, 0, %0, c2, c0, 0" : : "r"(saved_context.ttb_0r));
	return;
}



