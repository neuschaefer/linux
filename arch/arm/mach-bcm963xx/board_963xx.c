#if defined(CONFIG_BCM_KF_ARM_BCM963XX)
/*
<:copyright-BRCM:2013:GPL/GPL:standard

   Copyright (c) 2013 Broadcom Corporation
   All Rights Reserved

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation (the "GPL").

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.

:>
*/

/*
 * Generic board routine for Broadcom 963xx ARM boards
 */
#include <linux/types.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/clkdev.h>
#include <linux/module.h>

#include <asm/setup.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>
#include <asm/clkdev.h>

#include <mach/hardware.h>
#include <mach/memory.h>
#include <mach/smp.h>

#include <plat/bsp.h>
#if defined(CONFIG_BCM963138)
#include <plat/ca9mpcore.h>
#elif defined(CONFIG_BCM963148)
#include <plat/b15core.h>
#endif

#include <bcm_map_part.h>
#include <board.h>

#ifndef CONFIG_BRCM_IKOS
#if defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148)
#define BCM963XX_RESERVE_MEM_ADSL
#define BCM963XX_RESERVE_MEM_RDP
#endif
#endif

#if defined(BCM963XX_RESERVE_MEM_ADSL) || defined(BCM963XX_RESERVE_MEM_RDP)
#include <asm/mach/map.h>
#include <linux/memblock.h>
#endif

#if defined(BCM963XX_RESERVE_MEM_ADSL)
#include "softdsl/AdslCoreDefs.h"
#endif

#if defined(BCM963XX_RESERVE_MEM_RDP)
unsigned long tm_size = 0, mc_size = 0;
#endif

unsigned char g_blparms_buf[1024];
unsigned long memsize = SZ_16M;
bool is_rootfs_set = false;
bool is_memory_reserved = false;

#define MB_ALIGNED(__val)	(((__val & ~SZ_1M) ? SZ_1M : 0) + (__val & SZ_1M))
		

/***************************************************************************
 * C++ New and delete operator functions
 ***************************************************************************/

/* void *operator new(unsigned int sz) */
void *_Znwj(unsigned int sz)
{
	return( kmalloc(sz, GFP_KERNEL) );
}

/* void *operator new[](unsigned int sz)*/
void *_Znaj(unsigned int sz)
{
	return( kmalloc(sz, GFP_KERNEL) );
}

/* placement new operator */
/* void *operator new (unsigned int size, void *ptr) */
void *ZnwjPv(unsigned int size, void *ptr)
{
	return ptr;
}

/* void operator delete(void *m) */
void _ZdlPv(void *m)
{
	kfree(m);
}

/* void operator delete[](void *m) */
void _ZdaPv(void *m)
{
	kfree(m);
}
EXPORT_SYMBOL(_Znwj);
EXPORT_SYMBOL(_Znaj);
EXPORT_SYMBOL(ZnwjPv);
EXPORT_SYMBOL(_ZdlPv);
EXPORT_SYMBOL(_ZdaPv);

unsigned long getMemorySize(void)
{
	return memsize;
}

/* Pointers to memory buffers allocated for the DSP module */
void *dsp_core;
void *dsp_init;
EXPORT_SYMBOL(dsp_core);
EXPORT_SYMBOL(dsp_init);

/*
*****************************************************************************
** FUNCTION:   allocDspModBuffers
**
** PURPOSE:    Allocates buffers for the init and core sections of the DSP
**             module. This module is special since it has to be allocated
**             in the 0x800.. memory range which is not mapped by the TLB.
**
** PARAMETERS: None
** RETURNS:    Nothing
*****************************************************************************
*/
void __init allocDspModBuffers(void)
{
}

#if defined(BCM963XX_RESERVE_MEM_ADSL)
/* Reserve memory for DSL */
#define ADSL_SDRAM_RESERVE_SIZE		MB_ALIGNED(ADSL_SDRAM_IMAGE_SIZE)

/***************************************************************************
 * Function Name: kerSysGetDslPhyMemory
 * Description  : return the start address of the reserved DSL SDRAM. The memory
 * 		  is reserved in the arch dependent setup.c
 * Returns      : physical address of the reserved DSL SDRAM
 ***************************************************************************/
void *kerSysGetDslPhyMemory(void)
{
	if (is_memory_reserved == true) {
		if (getMemorySize() <= SZ_256M)
			return (void *)(getMemorySize() - ADSL_SDRAM_RESERVE_SIZE);
		else
			return (void *)(SZ_256M - ADSL_SDRAM_RESERVE_SIZE);
	} else
		return NULL;
}

EXPORT_SYMBOL(kerSysGetDslPhyMemory);
#endif

#if defined(BCM963XX_RESERVE_MEM_RDP)
/* Reserve memory for RDPA */
#define RDPA_RESERVE_MEM_NUM		2
static struct {
	char name[32];
	uint32_t phys_addr;
	uint32_t size;
} rdpa_reserve_mem[RDPA_RESERVE_MEM_NUM];

int BcmMemReserveGetByName(char *name, void **addr, unsigned int *size)
{
	int i;

	*addr = NULL;
	*size = 0;

	if (is_memory_reserved == false)
		return -1;

	for (i = 0; i < RDPA_RESERVE_MEM_NUM; i++) {
		if (strcmp(name, rdpa_reserve_mem[i].name) == 0) {
			*addr = phys_to_virt(rdpa_reserve_mem[i].phys_addr);
			*size = rdpa_reserve_mem[i].size;
			return 0;
		}
	}
	return -1;
}
EXPORT_SYMBOL(BcmMemReserveGetByName);
#endif

bool kerSysIsRootfsSet(void)
{
	return is_rootfs_set;
}
EXPORT_SYMBOL(kerSysIsRootfsSet);

void __init board_map_io(void)
{
#if defined(BCM963XX_RESERVE_MEM_ADSL) || defined(BCM963XX_RESERVE_MEM_RDP)
	struct map_desc desc[3];
	int i = 0, j;
#endif
	/* Map SoC specific I/O */
	soc_map_io();

#if defined(BCM963XX_RESERVE_MEM_ADSL)
	/* create a noncacheable memory device mapping for DSL driver to
	 * access the reserved memory */
	desc[i].virtual = (unsigned long)phys_to_virt(
			(unsigned long)kerSysGetDslPhyMemory());
	desc[i].pfn = __phys_to_pfn((unsigned long)kerSysGetDslPhyMemory());
	desc[i].length = ADSL_SDRAM_RESERVE_SIZE;
	desc[i].type = MT_MEMORY_NONCACHED;
	printk("creatiing a MT_MEMORY_NONCACHED device at physical address of "
			"0x%08lx to virtual address at "
			"0x%08lx with size of 0x%lx byte for DSL\n",
			(unsigned long)kerSysGetDslPhyMemory(),
			desc[i].virtual, desc[i].length);
	i++;
#endif

#if defined(BCM963XX_RESERVE_MEM_RDP)
	for (j = 0; j < RDPA_RESERVE_MEM_NUM; j++) {
		desc[i].virtual = (unsigned long)phys_to_virt(
				rdpa_reserve_mem[j].phys_addr);
		desc[i].pfn = __phys_to_pfn(rdpa_reserve_mem[j].phys_addr);
		desc[i].length = rdpa_reserve_mem[j].size;
		desc[i].type = MT_MEMORY_NONCACHED;
		printk("creatiing a MT_MEMORY_NONCACHED device at physical "
				"address of 0x%08lx to virtual address at "
				"0x%08lx with size of 0x%lx byte for RDPA "
				"%s\n",
				(unsigned long)rdpa_reserve_mem[j].phys_addr,
				desc[i].virtual, desc[i].length,
				rdpa_reserve_mem[j].name);
		i++;
	}
#endif

#if defined(BCM963XX_RESERVE_MEM_ADSL) || defined(BCM963XX_RESERVE_MEM_RDP)
	iotable_init(desc, i);
#endif

	if (getMemorySize() <= SZ_32M)
		printk("WARNING! System is with 0x%0lx memory, might not "
				"boot successfully.\n"
				"\tcheck ATAG or CMDLINE\n", getMemorySize());

	soc_init_clock();
}

void __init board_init_early(void)
{
	soc_init_early();
}


void __init board_init_irq(void)
{
	soc_init_irq();
	
	/* serial_setup(sih); */
}

void __init board_init_timer(void)
{
	soc_init_timer();
}

static void __init bcm_setup(void)
{
#if !defined(CONFIG_BCM_KF_IKOS) || !defined(CONFIG_BRCM_IKOS)
	kerSysEarlyFlashInit();
	kerSysFlashInit();
#endif
}

void __init board_init_machine(void)
{
	/*
	 * Add common platform devices that do not have board dependent HW
	 * configurations
	 */
	soc_add_devices();

	bcm_setup();

	return;
}

static void __init set_memsize_from_cmdline(char *cmdline)
{
	char *cmd_ptr, *end_ptr;

	cmd_ptr = strstr(cmdline, "mem=");
	if (cmd_ptr != NULL) {
		cmd_ptr += 4;
		memsize = (unsigned long)memparse(cmd_ptr, &end_ptr);
	}
}

static void __init check_if_rootfs_is_set(char *cmdline)
{
	char *cmd_ptr;

	cmd_ptr = strstr(cmdline, "root=");
	if (cmd_ptr != NULL)
		is_rootfs_set = true;
}

/* in ARM, there are two ways of passing in memory size.
 * one is by setting it in ATAG_MEM, and the other one is by setting the
 * size in CMDLINE.  The first appearance of mem=nn[KMG] in CMDLINE is the
 * value that has the highest priority. And if there is no memory size set
 * in CMDLINE, then it will use the value in ATAG_MEM.  If there is no ATAG
 * given from boot loader, then a default ATAG with memory size set to 16MB
 * will be taken effect.
 * Assuming CONFIG_CMDLINE_EXTEND is set. The logic doesn't work if
 * CONFIG_CMDLINE_FROM_BOOTLOADER is set. */
static void __init board_fixup(struct tag *t, char **cmdline, struct meminfo *mi)
{
	soc_fixup();

	/* obtaining info passing down from boot loader */
	for (; t->hdr.size; t = tag_next(t)) {
		if ((t->hdr.tag == ATAG_CORE) && (t->u.core.rootdev != 0xff))
			is_rootfs_set = true;

		if (t->hdr.tag == ATAG_MEM)
			memsize = t->u.mem.size;

#if defined(BCM963XX_RESERVE_MEM_RDP)
		if (t->hdr.tag == ATAG_RDPSIZE) {
			tm_size = t->u.rdpsize.tm_size * SZ_1M;
			mc_size = t->u.rdpsize.mc_size * SZ_1M;
		}
#endif

		if (t->hdr.tag == ATAG_BLPARM)
			strlcpy(g_blparms_buf, t->u.blparm.blparm, 1024);

		if (t->hdr.tag == ATAG_CMDLINE) {
			set_memsize_from_cmdline(t->u.cmdline.cmdline);
			check_if_rootfs_is_set(t->u.cmdline.cmdline);
		}
	}

	set_memsize_from_cmdline(*cmdline);
	check_if_rootfs_is_set(*cmdline);
}

static void __init board_reserve(void)
{
	/* used for reserve mem blocks */
#if defined(BCM963XX_RESERVE_MEM_ADSL) || defined(BCM963XX_RESERVE_MEM_RDP)
	unsigned long mem_end = getMemorySize();
	unsigned long rsrv_mem_required = SZ_8M;

	/* both reserved memory for RDP and DSL have to be within first
	 * 256MB */
	if (mem_end > SZ_256M)
		mem_end = SZ_256M;
#endif

#if defined(BCM963XX_RESERVE_MEM_RDP)
	/* Make sure the input values are larger than minimum required */
	if (tm_size < TM_DEF_DDR_SIZE)
		tm_size = TM_DEF_DDR_SIZE;

	if (mc_size < TM_MC_DEF_DDR_SIZE)
		mc_size = TM_MC_DEF_DDR_SIZE;

	/* both TM and MC reserved memory size has to be multiple of 2MB */
	if (tm_size & SZ_1M)
		tm_size += SZ_1M;
	if (mc_size & SZ_1M)
		mc_size += SZ_1M;

	rsrv_mem_required += tm_size + mc_size;
#endif

#if defined(BCM963XX_RESERVE_MEM_ADSL)
	rsrv_mem_required += ADSL_SDRAM_RESERVE_SIZE;
#endif

#if defined(BCM963XX_RESERVE_MEM_ADSL) || defined(BCM963XX_RESERVE_MEM_RDP)
	/* check if those configured memory sizes are over what
	 * system has */
	if (getMemorySize() < rsrv_mem_required) {
#if defined(BCM963XX_RESERVE_MEM_RDP)
		/* If RDP is enabled, try to use the default
		 * TM and MC reserved memory size and try again */
		rsrv_mem_required -= tm_size + mc_size;
		tm_size = TM_DEF_DDR_SIZE;
		mc_size = TM_MC_DEF_DDR_SIZE;
		rsrv_mem_required += tm_size + mc_size;
#endif
		if (getMemorySize() < rsrv_mem_required)
			return;
	}
#endif

#if defined(BCM963XX_RESERVE_MEM_ADSL)
	/* reserve memory for DSL.  We use memblock_remove + IO_MAP the removed
	 * memory block to MT_MEMORY_NONCACHED here because ADSL driver code
	 * will need to access the memory.  Another option is to use
	 * memblock_reserve where the kernel still sees the memory, but I could
	 * not find a function to make the reserved memory noncacheable. */
	mem_end -= ADSL_SDRAM_RESERVE_SIZE;
	memblock_remove(mem_end, ADSL_SDRAM_RESERVE_SIZE);
#endif

#if defined(BCM963XX_RESERVE_MEM_RDP)
	mem_end -= tm_size;
	/* TM reserved memory has to be 2MB-aligned */
	if (mem_end & SZ_1M)
		mem_end -= SZ_1M;
	memblock_remove(mem_end, tm_size);
	strcpy(rdpa_reserve_mem[0].name, TM_BASE_ADDR_STR);
	rdpa_reserve_mem[0].phys_addr = (uint32_t)mem_end;
	rdpa_reserve_mem[0].size = tm_size;

	mem_end -= mc_size;
	/* MC reserved memory has to be 2MB-aligned */
	if (unlikely(mem_end & SZ_1M))
		mem_end -= SZ_1M;
	memblock_remove(mem_end, mc_size);
	strcpy(rdpa_reserve_mem[1].name, TM_MC_BASE_ADDR_STR);
	rdpa_reserve_mem[1].phys_addr = (uint32_t)mem_end;
	rdpa_reserve_mem[1].size = mc_size;
#endif

#if defined(BCM963XX_RESERVE_MEM_ADSL) || defined(BCM963XX_RESERVE_MEM_RDP)
	is_memory_reserved = true;
#endif
}

static struct sys_timer board_timer = {
	.init = board_init_timer,
};

static void board_restart(char mode, const char *cmd)
{
#ifndef CONFIG_BRCM_IKOS
	kerSysMipsSoftReset();
#endif
}

#if defined(CONFIG_BCM963138)
MACHINE_START(BCM963138, "BCM963138")
	/* Maintainer: Broadcom */
	.fixup		= board_fixup,
	.reserve	= board_reserve,
	.map_io		= board_map_io,	
	.init_early	= board_init_early,
	.init_irq	= board_init_irq,
	.timer		= &board_timer,
	.init_machine	= board_init_machine,
#ifdef CONFIG_MULTI_IRQ_HANDLER
	.handle_irq	= gic_handle_irq,
#endif
#ifdef CONFIG_ZONE_DMA
	/* If enable CONFIG_ZONE_DMA, it will reserve the given size of
	 * memory from SDRAM and use it exclusively for DMA purpose.
	 * This ensures the device driver can allocate enough memory. */
	.dma_zone_size	= SZ_16M,	/* must be multiple of 2MB */
#endif
	.restart	= board_restart,
MACHINE_END
#endif

#if defined(CONFIG_BCM963148)
MACHINE_START(BCM963148, "BCM963148")
	/* Maintainer: Broadcom */
	.fixup		= board_fixup,
	.reserve	= board_reserve,
	.map_io		= board_map_io,	
	.init_early	= board_init_early,
	.init_irq	= board_init_irq,
	.timer		= &board_timer,
	.init_machine	= board_init_machine,
#ifdef CONFIG_MULTI_IRQ_HANDLER
	.handle_irq	= gic_handle_irq,
#endif
#ifdef CONFIG_ZONE_DMA
	/* If enable CONFIG_ZONE_DMA, it will reserve the given size of
	 * memory from SDRAM and use it exclusively for DMA purpose.
	 * This ensures the device driver can allocate enough memory. */
	.dma_zone_size	= SZ_16M,	/* must be multiple of 2MB and within 16MB for DSL PHY */
#endif
	.restart	= board_restart,
MACHINE_END
#endif

#endif /* CONFIG_BCM_KF_ARM_BCM963XX */
