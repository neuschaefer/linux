/*
 * arch/arch/mach-sun5i/core.c
 * (C) Copyright 2010-2015
 * Reuuimlla Technology Co., Ltd. <www.reuuimllatech.com>
 * Benn Huang <benn@Reuuimllatech.com>
 *
 * SUN5I machine core implementations
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 */

#include <linux/init.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/sysdev.h>
#include <linux/interrupt.h>
#include <linux/amba/bus.h>
#include <linux/amba/clcd.h>
#include <linux/amba/pl061.h>
#include <linux/amba/mmci.h>
#include <linux/amba/pl022.h>
#include <linux/io.h>
#include <linux/gfp.h>
#include <linux/clockchips.h>
#include <linux/memblock.h>
#include <linux/bootmem.h>

#include <asm/clkdev.h>
#include <asm/system.h>
#include <asm/irq.h>
#include <asm/leds.h>
#include <asm/hardware/arm_timer.h>
#include <asm/hardware/icst.h>
#include <asm/hardware/vic.h>
#include <asm/mach-types.h>
#include <asm/setup.h>

#include <asm/mach/arch.h>
#include <asm/mach/flash.h>
#include <asm/mach/irq.h>
#include <asm/mach/time.h>
#include <asm/mach/map.h>
#include <mach/system.h>
#include <mach/timex.h>
#include <mach/sys_config.h>

/**
 * Machine Implementations
 *
 */

static struct map_desc sw_io_desc[] __initdata = {
	{ SW_VA_SRAM_BASE, __phys_to_pfn(SW_PA_SRAM_BASE),  (SZ_128K + SZ_64K), MT_MEMORY_ITCM  },
	{ SW_VA_IO_BASE,   __phys_to_pfn(SW_PA_IO_BASE),    (SZ_1M + SZ_2M),    MT_DEVICE       },
	{ SW_VA_BROM_BASE, __phys_to_pfn(SW_PA_BROM_BASE),  (SZ_64K),           MT_MEMORY_ITCM  },
};

void __init sw_core_map_io(void)
{
	iotable_init(sw_io_desc, ARRAY_SIZE(sw_io_desc));
}

static u32 DRAMC_get_dram_size(void)
{
	u32 reg_val;
	u32 dram_size;
	u32 chip_den;

	reg_val = readl(SW_DRAM_SDR_DCR);
	chip_den = (reg_val >> 3) & 0x7;
	if(chip_den == 0)
		dram_size = 32;
	else if(chip_den == 1)
		dram_size = 64;
	else if(chip_den == 2)
		dram_size = 128;
	else if(chip_den == 3)
		dram_size = 256;
	else if(chip_den == 4)
		dram_size = 512;
	else
		dram_size = 1024;

	if( ((reg_val>>1)&0x3) == 0x1)
		dram_size<<=1;
	if( ((reg_val>>6)&0x7) == 0x3)
		dram_size<<=1;
	if( ((reg_val>>10)&0x3) == 0x1)
		dram_size<<=1;

        return dram_size;
//	return 256;
}

static void __init sw_core_fixup(struct machine_desc *desc,
                  struct tag *tags, char **cmdline,
                  struct meminfo *mi)
{
	u32 size;

#ifdef CONFIG_SUN5I_FPGA
	size = 256;
	mi->nr_banks = 1;
	mi->bank[0].start = 0x40000000;
	mi->bank[0].size = SZ_1M * size;
#else
	size = DRAMC_get_dram_size();
	early_printk("DRAM: %d", size);

	mi->nr_banks = 2;
	mi->bank[0].start = 0x40000000;
	mi->bank[0].size = SW_BANK1_SIZE;
	mi->bank[1].start = 0x40000000 + SW_BANK2_OFFSET;
	mi->bank[1].size = SZ_1M * size - SW_BANK2_OFFSET;
//	mi->bank[1].size = SZ_1M * 256 - SW_BANK2_OFFSET;
#endif

	pr_info("Total Detected Memory: %uMB with %d banks\n", size, mi->nr_banks);
}

unsigned long fb_start = SW_FB_MEM_BASE;
unsigned long fb_size = SW_FB_MEM_SIZE;
EXPORT_SYMBOL(fb_start);
EXPORT_SYMBOL(fb_size);

unsigned long ve_start = SW_VE_MEM_BASE;
unsigned long ve_size = SW_VE_MEM_SIZE;
EXPORT_SYMBOL(ve_start);
EXPORT_SYMBOL(ve_size);

#ifndef CONFIG_SUN5I_A13
unsigned long g2d_start = (PLAT_PHYS_OFFSET + SZ_512M - SZ_128M);
unsigned long g2d_size = SZ_1M * 16;
EXPORT_SYMBOL(g2d_start);
EXPORT_SYMBOL(g2d_size);
#endif

static void __init sw_core_reserve(void)
{
	pr_info("Memory Reserved(in bytes):\n");
	memblock_reserve(SYS_CONFIG_MEMBASE, SYS_CONFIG_MEMSIZE);
	pr_info("\tSYS: 0x%08x, 0x%08x\n", (unsigned int)SYS_CONFIG_MEMBASE, (unsigned int)SYS_CONFIG_MEMSIZE);
#ifdef CONFIG_SUN5I_FPGA
#else

#ifdef CONFIG_LYCHEE_FB_SUN5I
	memblock_reserve(fb_start, fb_size);
	memblock_reserve(ve_start, ve_size);
	//memblock_reserve(ve_start + SZ_64M, SZ_16M);
	pr_info("\tLCD: 0x%08x, 0x%08x\n", (unsigned int)fb_start, (unsigned int)fb_size);
	pr_info("\tVE : 0x%08x, 0x%08x\n", (unsigned int)ve_start, (unsigned int)ve_size);
#endif

#endif
}

void sw_irq_ack(struct irq_data *irqd)
{
	unsigned int irq = irqd->irq;

	if (irq < 32){
		writel(readl(SW_INT_ENABLE_REG0) & ~(1<<irq), SW_INT_ENABLE_REG0);
		writel(readl(SW_INT_MASK_REG0) | (1 << irq), SW_INT_MASK_REG0);
		writel(readl(SW_INT_IRQ_PENDING_REG0) | (1<<irq), SW_INT_IRQ_PENDING_REG0);
	} else if(irq < 64){
		irq -= 32;
		writel(readl(SW_INT_ENABLE_REG1) & ~(1<<irq), SW_INT_ENABLE_REG1);
		writel(readl(SW_INT_MASK_REG1) | (1 << irq), SW_INT_MASK_REG1);
		writel(readl(SW_INT_IRQ_PENDING_REG1) | (1<<irq), SW_INT_IRQ_PENDING_REG1);
	} else if(irq < 96){
		irq -= 64;
		writel(readl(SW_INT_ENABLE_REG2) & ~(1<<irq), SW_INT_ENABLE_REG2);
		writel(readl(SW_INT_MASK_REG2) | (1 << irq), SW_INT_MASK_REG2);
		writel(readl(SW_INT_IRQ_PENDING_REG2) | (1<<irq), SW_INT_IRQ_PENDING_REG2);
	}
}

/* Mask an IRQ line, which means disabling the IRQ line */
static void sw_irq_mask(struct irq_data *irqd)
{
	unsigned int irq = irqd->irq;

	if(irq < 32){
		writel(readl(SW_INT_ENABLE_REG0) & ~(1<<irq), SW_INT_ENABLE_REG0);
		writel(readl(SW_INT_MASK_REG0) | (1 << irq), SW_INT_MASK_REG0);
	} else if(irq < 64){
		irq -= 32;
		writel(readl(SW_INT_ENABLE_REG1) & ~(1<<irq), SW_INT_ENABLE_REG1);
		writel(readl(SW_INT_MASK_REG1) | (1 << irq), SW_INT_MASK_REG1);
	} else if(irq < 96){
		irq -= 64;
		writel(readl(SW_INT_ENABLE_REG2) & ~(1<<irq), SW_INT_ENABLE_REG2);
		writel(readl(SW_INT_MASK_REG2) | (1 << irq), SW_INT_MASK_REG2);
	}
}

static void sw_irq_unmask(struct irq_data *irqd)
{
	unsigned int irq = irqd->irq;

	if(irq < 32){
		writel(readl(SW_INT_ENABLE_REG0) | (1<<irq), SW_INT_ENABLE_REG0);
		writel(readl(SW_INT_MASK_REG0) & ~(1 << irq), SW_INT_MASK_REG0);
		if(irq == SW_INT_IRQNO_ENMI) /* must clear pending bit when enabled */
			writel((1 << SW_INT_IRQNO_ENMI), SW_INT_IRQ_PENDING_REG0);
	} else if(irq < 64){
		irq -= 32;
		writel(readl(SW_INT_ENABLE_REG1) | (1<<irq), SW_INT_ENABLE_REG1);
		writel(readl(SW_INT_MASK_REG1) & ~(1 << irq), SW_INT_MASK_REG1);
	} else if(irq < 96){
		irq -= 64;
		writel(readl(SW_INT_ENABLE_REG2) | (1<<irq), SW_INT_ENABLE_REG2);
		writel(readl(SW_INT_MASK_REG2) & ~(1 << irq), SW_INT_MASK_REG2);
	}
}

static struct irq_chip sw_vic_chip = {
	.name       = "sw_vic",
	.irq_ack    = sw_irq_ack,
	.irq_mask   = sw_irq_mask,
	.irq_unmask = sw_irq_unmask,
};

void __init sw_core_init_irq(void)
{
	u32 i = 0;

	/* Disable & clear all interrupts */
	writel(0, SW_INT_ENABLE_REG0);
	writel(0, SW_INT_ENABLE_REG1);
	writel(0, SW_INT_ENABLE_REG2);

	writel(0xffffffff, SW_INT_MASK_REG0);
	writel(0xffffffff, SW_INT_MASK_REG1);
	writel(0xffffffff, SW_INT_MASK_REG2);

	writel(0xffffffff, SW_INT_IRQ_PENDING_REG0);
	writel(0xffffffff, SW_INT_IRQ_PENDING_REG1);
	writel(0xffffffff, SW_INT_IRQ_PENDING_REG2);
	writel(0xffffffff, SW_INT_FIQ_PENDING_REG0);
	writel(0xffffffff, SW_INT_FIQ_PENDING_REG1);
	writel(0xffffffff, SW_INT_FIQ_PENDING_REG2);

	/*enable protection mode*/
	writel(0x01, SW_INT_PROTECTION_REG);
	/*config the external interrupt source type*/
	writel(0x00, SW_INT_NMI_CTRL_REG);

	for (i = SW_INT_START; i < SW_INT_END; i++) {
		irq_set_chip(i, &sw_vic_chip);
		irq_set_handler(i, handle_level_irq);
		set_irq_flags(i, IRQF_VALID | IRQF_PROBE);
	}
}



/**
 * Global vars definitions
 *
 */
static void timer_set_mode(enum clock_event_mode mode, struct clock_event_device *clk)
{
	volatile u32 ctrl;

	switch (mode) {
	case CLOCK_EVT_MODE_PERIODIC:
		pr_info("timer0: Periodic Mode\n");
		writel(TMR_INTER_VAL, SW_TIMER0_INTVAL_REG); /* interval (999+1) */
		ctrl = readl(SW_TIMER0_CTL_REG);
		ctrl &= ~(1<<7);    /* Continuous mode */
		ctrl |= 1;  /* Enable this timer */
		break;

	case CLOCK_EVT_MODE_ONESHOT:
		pr_info("timer0: Oneshot Mode\n");
		ctrl = readl(SW_TIMER0_CTL_REG);
		ctrl |= (1<<7);     /* Single mode */
		break;
	case CLOCK_EVT_MODE_UNUSED:
	case CLOCK_EVT_MODE_SHUTDOWN:
	default:
		ctrl = readl(SW_TIMER0_CTL_REG);
		ctrl &= ~(1<<0);    /* Disable timer0 */
		break;
	}

	writel(ctrl, SW_TIMER0_CTL_REG);
}

/* Useless when periodic mode */
static int timer_set_next_event(unsigned long evt, struct clock_event_device *unused)
{
	volatile u32 ctrl;

	/* clear any pending before continue */
	ctrl = readl(SW_TIMER0_CTL_REG);
	writel(evt, SW_TIMER0_CNTVAL_REG);
	ctrl |= (1<<1);
	writel(ctrl, SW_TIMER0_CTL_REG);
	writel(ctrl | 0x1, SW_TIMER0_CTL_REG);

	return 0;
}

static struct clock_event_device timer0_clockevent = {
	.name = "timer0",
	.shift = 32,
	.rating = 100,
	.features = CLOCK_EVT_FEAT_PERIODIC | CLOCK_EVT_FEAT_ONESHOT,
	.set_mode = timer_set_mode,
	.set_next_event = timer_set_next_event,
};


static irqreturn_t sw_timer_interrupt(int irq, void *dev_id)
{
	struct clock_event_device *evt = (struct clock_event_device *)dev_id;

	writel(0x1, SW_TIMER_INT_STA_REG);
	/*
 	 * timer_set_next_event will be called only in ONESHOT mode
 	 */
	evt->event_handler(evt);

	return IRQ_HANDLED;
}

static struct irqaction sw_timer_irq = {
	.name = "timer0",
	.flags = IRQF_DISABLED | IRQF_TIMER | IRQF_IRQPOLL,
	.handler = sw_timer_interrupt,
	.dev_id = &timer0_clockevent,
	.irq = SW_INT_IRQNO_TIMER0,
};
extern int aw_clksrc_init(void);

static void __init sw_timer_init(void)
{
	int ret;
	volatile u32  val = 0;

	writel(TMR_INTER_VAL, SW_TIMER0_INTVAL_REG);
	/* set clock sourch to HOSC, 16 pre-division */
	val = readl(SW_TIMER0_CTL_REG);
	val &= ~(0x07<<4);
	val &= ~(0x03<<2);
	val |= (4<<4) | (1<<2);
	writel(val, SW_TIMER0_CTL_REG);
	/* set mode to auto reload */
	val = readl(SW_TIMER0_CTL_REG);
	val |= (1<<1);
	writel(val, SW_TIMER0_CTL_REG);

	ret = setup_irq(SW_INT_IRQNO_TIMER0, &sw_timer_irq);
	if (ret) {
		pr_warning("failed to setup irq %d\n", SW_INT_IRQNO_TIMER0);
	}

	/* Enable time0 interrupt */
	val = readl(SW_TIMER_INT_CTL_REG);
	val |= (1<<0);
	writel(val, SW_TIMER_INT_CTL_REG);

	timer0_clockevent.mult = div_sc(SYS_TIMER_CLKSRC/SYS_TIMER_SCAL, NSEC_PER_SEC, timer0_clockevent.shift);
	timer0_clockevent.max_delta_ns = clockevent_delta2ns(0xff, &timer0_clockevent);
	timer0_clockevent.min_delta_ns = clockevent_delta2ns(0x1, &timer0_clockevent);
	timer0_clockevent.cpumask = cpumask_of(0);
	timer0_clockevent.irq = sw_timer_irq.irq;
	clockevents_register_device(&timer0_clockevent);
    printk("%s,line:%d\n", __func__, __LINE__);
    aw_clksrc_init();
}

struct sys_timer sw_sys_timer = {
	.init = sw_timer_init,
};

extern void __init sw_pdev_init(void);
void __init sw_core_init(void)
{
	sw_pdev_init();
}


static enum sw_ic_ver version = MAGIC_VER_NULL;
enum sw_ic_ver sw_get_ic_ver(void)
{
    u32 val;

    if(version != MAGIC_VER_NULL) {
        return version;
    }

    val = readl(SW_VA_CCM_IO_BASE + 0x60);
    val |= 1<<5;
    writel(val, SW_VA_CCM_IO_BASE + 0x60);
    val = readl(SW_VA_CCM_IO_BASE + 0x9c);
    val |= 1<<31;
    writel(val, SW_VA_CCM_IO_BASE + 0x9c);

    val = readl(SW_VA_SSE_IO_BASE);
    switch((val>>16)&0x07)
    {
        case 0:
        {
            val = readl(SW_VA_SID_IO_BASE+0x08);
            val = (val>>12) & 0x0f;
            if((val == 0x3) || (val == 0)) {
                val = readl(SW_VA_SID_IO_BASE+0x00);
                val = (val>>8)&0xffffff;
                if((val == 0x162541) || (val == 0)) {
                    version = MAGIC_VER_A12A;
                } else if(val == 0x162542) {
                    version = MAGIC_VER_A12B;
                } else {
                    version = MAGIC_VER_UNKNOWN;
                }
            } else if(val == 0x07) {
                val = readl(SW_VA_SID_IO_BASE+0x00);
                val = (val>>8)&0xffffff;
                if((val == 0x162541) || (val == 0)) {
                    version = MAGIC_VER_A10SA;
                } else if(val == 0x162542) {
                    version = MAGIC_VER_A10SB;
                } else {
                    version = MAGIC_VER_UNKNOWN;
                }
            } else {
                version = MAGIC_VER_UNKNOWN;
            }
            break;
        }

        case 1:
        {
            val = readl(SW_VA_SID_IO_BASE+0x00);
            val = (val>>8)&0xffffff;
            if((val == 0x162541) || (val == 0x162565) || (val == 0)) {
                version = MAGIC_VER_A13A;
            } else if(val == 0x162542) {
                version = MAGIC_VER_A13B;
            } else {
                version = MAGIC_VER_UNKNOWN;
            }
            break;
        }

        default:
        {
            version = MAGIC_VER_UNKNOWN;
            break;
        }
    }

    return version;
}
EXPORT_SYMBOL(sw_get_ic_ver);


int sw_get_chip_id(struct sw_chip_id *chip_id)
{
    chip_id->sid_rkey0 = readl(SW_VA_SID_IO_BASE);
    chip_id->sid_rkey1 = readl(SW_VA_SID_IO_BASE+0x04);
    chip_id->sid_rkey2 = readl(SW_VA_SID_IO_BASE+0x08);
    chip_id->sid_rkey3 = readl(SW_VA_SID_IO_BASE+0x0C);

    return 0;
}
EXPORT_SYMBOL(sw_get_chip_id);

/**
 * Arch Required Implementations
 *
 */
//void arch_idle(void)
//{

//}

//void arch_reset(char mode, const char *cmd)
//{


//}


MACHINE_START(SUN5I, "sun5i")
	.boot_params    = PLAT_PHYS_OFFSET + 0x100,
	.timer          = &sw_sys_timer,
	.fixup          = sw_core_fixup,
	.map_io         = sw_core_map_io,
	.init_early     = NULL,
	.init_irq       = sw_core_init_irq,
	.init_machine   = sw_core_init,
	.reserve        = sw_core_reserve,
MACHINE_END

