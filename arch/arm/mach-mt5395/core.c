/*
 *  linux/arch/arm/mach-mt5395/core.c
 *
 * Licensed under the GPL
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html#TOC1
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile:  $
 * $Revision: #1 $
 *
 * CPU core init - irq, time, baseio
 */

/* system header files */
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

#include <mach/hardware.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/setup.h>
#include <asm/mach-types.h>
#include <asm/pgtable.h>
#include <asm/page.h>
#include <asm/system.h>
#include <asm/uaccess.h>

#include <asm/mach/arch.h>
#include <asm/mach/irq.h>
#include <asm/mach/map.h>

#include <asm/mach/time.h>

#include <asm/pmu.h>
#include <linux/platform_device.h>

// NDC patch start
#include <linux/random.h>
#include <linux/net.h>
#include <asm/uaccess.h>
// NDC patch stop

// PCI related header file.
#include <linux/pci.h>
#include <asm/mach/pci.h>
#include "mtk_pci.h"


// PMEM related
#include <linux/platform_device.h>
#include <linux/bootmem.h>
#include <linux/android_pmem.h>
#define MT53XX_PMEM_SIZE 0x2F00000


int pmem_setup(struct android_pmem_platform_data *pdata,
	       long (*ioctl)(struct file *, unsigned int, unsigned long),
	       int (*release)(struct inode *, struct file *));

DEFINE_SPINLOCK(mt53xx_bim_lock);
EXPORT_SYMBOL(mt53xx_bim_lock);

/*======================================================================
 * irq
 */

static inline u32 __bim_readl(u32 regaddr32)
{
    return __raw_readl(BIM_VIRT + regaddr32);
}

static inline void __bim_writel(u32 regval32, u32 regaddr32)
{
    __raw_writel(regval32, BIM_VIRT + regaddr32);
}

static inline int _bim_is_vector_valid(u32 irq_num)
{
    return (irq_num < NR_IRQS);
}

static void mt5395_mask_ack_irq(unsigned int irq)
{
    u32 regval32;
    unsigned long flags;

    if (!_bim_is_vector_valid(irq))
    {
        while (1)
            ;
        return;
    }

    if (irq > VECTOR_MISC)
    {
        u32 misc_irq;
        misc_irq = _MISCIRQ(irq);
        local_irq_save(flags);
        regval32 = __bim_readl(REG_RW_MINTEN);
        regval32 &= ~misc_irq;
        __bim_writel(regval32, REG_RW_MINTEN);
        if ((irq != VECTOR_VDEC) && (irq != VECTOR_DRAMC))
        {
            __bim_writel(misc_irq, REG_RW_MINTCLR);
        }
        local_irq_restore(flags);
    }
    else
    {
        local_irq_save(flags);
        regval32 = __bim_readl(REG_RW_IRQEN);
        regval32 &= ~(1 << irq);
        __bim_writel(regval32, REG_RW_IRQEN);
        if ((irq != VECTOR_VDEC) && (irq != VECTOR_DRAMC))
        {
            __bim_writel((1 << irq), REG_RW_IRQCLR);
        }
        local_irq_restore(flags);
    }
}

static void mt5395_mask_irq(unsigned int irq)
{
    u32 regval32;
    unsigned long flags;

    if (!_bim_is_vector_valid(irq))
    {
        while (1)
            ;
        return;
    }

    if (irq > VECTOR_MISC)
    {
        u32 misc_irq;
        misc_irq = _MISCIRQ(irq);
        local_irq_save(flags);
        regval32 = __bim_readl(REG_RW_MINTEN);
        regval32 &= ~misc_irq;
        __bim_writel(regval32, REG_RW_MINTEN);
        local_irq_restore(flags);
    }
    else
    {
        local_irq_save(flags);
        regval32 = __bim_readl(REG_RW_IRQEN);
        regval32 &= ~(1 << irq);
        __bim_writel(regval32, REG_RW_IRQEN);
        local_irq_restore(flags);
    }
}

static void mt5395_unmask_irq(unsigned int irq)
{
    u32 regval32;
    unsigned long flags;

    if (irq > VECTOR_MISC)
    {
        u32 misc_irq;
        misc_irq = _MISCIRQ(irq);
        local_irq_save(flags);
        regval32 = __bim_readl(REG_RW_MINTEN);
        regval32 |= (misc_irq);
        __bim_writel(regval32, REG_RW_MINTEN);
        local_irq_restore(flags);
    }
    else
    {
        local_irq_save(flags);
        regval32 = __bim_readl(REG_RW_IRQEN);
        regval32 |= (1 << irq);
        __bim_writel(regval32, REG_RW_IRQEN);
        local_irq_restore(flags);
    }
}

static struct irq_chip mt5395_irqchip =
{
    .ack    = mt5395_mask_ack_irq,
    .mask   = mt5395_mask_irq,
    .unmask = mt5395_unmask_irq,
};

static void __init mt5395_init_irq(void)
{
    unsigned long flags;
    unsigned int irq;
    u32 u4Reg;

    local_irq_save(flags);

    /* turn all irq off */
    __bim_writel(0, REG_RW_IRQEN);
    __bim_writel(0, REG_RW_MINTEN);

    /* clear all pending irq */
    __bim_writel(0xffffffff, REG_RW_IRQCLR);
    __bim_writel(0xffffffff, REG_RW_IRQCLR);
    __bim_writel(0xffffffff, REG_RW_IRQCLR);
    __bim_writel(0xffffffff, REG_RW_IRQCLR);
    __bim_writel(0xffffffff, REG_RW_IRQCLR);
    __bim_writel(0xffffffff, REG_RW_MINTCLR);
    __bim_writel(0xffffffff, REG_RW_MINTCLR);
    __bim_writel(0xffffffff, REG_RW_MINTCLR);
    __bim_writel(0xffffffff, REG_RW_MINTCLR);
    __bim_writel(0xffffffff, REG_RW_MINTCLR);

    for (irq = 0; irq < NR_IRQS; irq++)
    {
        //dynamic_irq_init(irq);
        set_irq_chip(irq, &mt5395_irqchip);
        set_irq_handler(irq, handle_level_irq);
        set_irq_flags(irq, IRQF_VALID | IRQF_PROBE);
    }

    /* turn misc irq on */
    __bim_writel(0x80000000, REG_RW_IRQEN);

    /* set pmu irq to level triggered */
    u4Reg = __bim_readl(REG_RW_MISC);
    u4Reg |= 0x100;
    __bim_writel(u4Reg, REG_RW_MISC);

    /* for some reason, people believe we have to do above */
    local_irq_restore(flags);
}

#define TICKS_PER_SECOND HZ
#define BIM_BASE					(IO_VIRT + 0x08000)
#define CKGEN_BASE					(IO_VIRT + 0x0d000)
#define HAL_READ32(_reg_)           (*((volatile unsigned long*)(_reg_)))
#define IO_READ32(base, offset)     HAL_READ32((base) + (offset))
#define BIM_REG32(offset)           IO_REG32(BIM_BASE, offset)
#define BIM_READ32(off)             IO_READ32(BIM_BASE, (off))
#define CKGEN_READ32(offset)        IO_READ32(CKGEN_BASE, (offset))
#define BIM_WRITE32(off, val)       IO_WRITE32(BIM_BASE, (off), (val))

#define REG_IRQCL                   0x0038      // RISC IRQ Clear Register
#define VECTOR_T0                   3           // Timer 0
#define TIMER_ID                    2
#define TIMER_VECTOR                (VECTOR_T0 + TIMER_ID)
#define TIMER_IRQ                   (1 << TIMER_VECTOR)
#define TIMER_LOW_REG               (REG_RW_TIMER0_LOW + (TIMER_ID * 8))
#define TIMER_HIGH_REG              (REG_RW_TIMER0_HIGH + (TIMER_ID * 8))
#define TIMER_LIMIT_LOW_REG         (REG_RW_TIMER0_LLMT + (TIMER_ID * 8))
#define TIMER_LIMIT_HIGH_REG        (REG_RW_TIMER0_HLMT + (TIMER_ID * 8))
#define TCTL_TIMER_EN               (TMR_CNTDWN_EN(TIMER_ID))
#define TCTL_TIMER_AL               (TMR_AUTOLD_EN(TIMER_ID))
#define MAX_TIMER_INTERVAL          (0xffffffff)

// XTAL config
#define CLK_27MHZ               27000000    // 27 MHz
#define CLK_48MHZ               48000000    // 48 MHz
#define CLK_54MHZ               54000000    // 54 MHz
#define CLK_60MHZ               60000000    // 60 MHz

#define XTAL_STRAP_MODE         (CKGEN_READ32(0) & 0x6000)
#define XTAL_STRAP_MODE_SHIFT   13
#define SET_XTAL_27MHZ          (0U << XTAL_STRAP_MODE_SHIFT)
#define SET_XTAL_60MHZ          (1U << XTAL_STRAP_MODE_SHIFT)
#define SET_XTAL_54MHZ          (2U << XTAL_STRAP_MODE_SHIFT)
#define SET_XTAL_27MHZ_A        (3U << XTAL_STRAP_MODE_SHIFT)     // 27Mhz + MT8225 mode
#define IS_XTAL_54MHZ()         (XTAL_STRAP_MODE == SET_XTAL_54MHZ)
#define IS_XTAL_60MHZ()         (XTAL_STRAP_MODE == SET_XTAL_60MHZ)
#define IS_XTAL_27MHZ()         ((XTAL_STRAP_MODE == SET_XTAL_27MHZ) ||(XTAL_STRAP_MODE == SET_XTAL_27MHZ_A))
#define IS_XTAL_30MHZ()         0
#define GET_XTAL_CLK()          (IS_XTAL_54MHZ() ? (CLK_54MHZ) :    \
                                (IS_XTAL_60MHZ() ? (CLK_60MHZ) :    \
                                (CLK_27MHZ) ))

/*======================================================================
 * timer interrupt
 */
static unsigned long _timer_tick_count;

static u32 _CKGEN_GetXtalClock(void)
{
#ifdef CONFIG_MT53_FPGA
    return CLK_27MHZ;
#else
    return GET_XTAL_CLK();
#endif /* CONFIG_FPGA */
}

static irqreturn_t
mt5395_timer_interrupt(int irq, void *dev_id)
{
#ifdef CONFIG_GENERIC_CLOCKEVENTS
//#error need to implement clocksource/clockevent  
#else
    timer_tick();
#endif
    return IRQ_HANDLED;
}

static struct irqaction mt5395_timer_irq =
{
    .name       = "Mt5395 Timer Tick",
    .flags      = IRQF_DISABLED | IRQF_TIMER,
    .handler    = mt5395_timer_interrupt,
};

static struct resource mt53xx_pmu_resource[] = {
    [0] = {
        .start  = VECTOR_PMU,
        .end    = VECTOR_PMU,
        .flags  = IORESOURCE_IRQ,
    },
};

static struct platform_device mt53xx_pmu_device = {
    .name       = "arm-pmu",
    .id     = ARM_PMU_DEVICE_CPU,
    .resource   = mt53xx_pmu_resource,
    .num_resources  = ARRAY_SIZE(mt53xx_pmu_resource),
};


/* Valid TIMER_ID: 0, 1, 2 */
#define SYS_TIMER_ID        0
#define HP_TIMER_ID         2

#define SYS_TOFST           (SYS_TIMER_ID * 8)
#define HP_TOFST            (HP_TIMER_ID * 8)

#define SYS_TIMER_IRQ       (IRQ_TIMER0 +  SYS_TIMER_ID)
#define SYS_TIMER_REG_L     (REG_RW_TIMER0_LOW + SYS_TOFST)
#define SYS_TIMER_REG_H     (REG_RW_TIMER0_HIGH + SYS_TOFST)
#define SYS_TIMER_LIMIT_REG_L (REG_RW_TIMER0_LLMT + SYS_TOFST)
#define SYS_TIMER_LIMIT_REG_H (REG_RW_TIMER0_HLMT + SYS_TOFST)
#define SYS_TIMER_EN        (TCTL_T0EN << SYS_TOFST)
#define SYS_TIMER_AL        (TCTL_T0AL << SYS_TOFST)

#define HP_TIMER_IRQ       (IRQ_TIMER0 +  HP_TIMER_ID)
#define HP_TIMER_REG_L     (REG_RW_TIMER0_LOW + HP_TOFST)
#define HP_TIMER_REG_H     (REG_RW_TIMER0_HIGH + HP_TOFST)
#define HP_TIMER_LIMIT_REG_L (REG_RW_TIMER0_LLMT + HP_TOFST)
#define HP_TIMER_LIMIT_REG_H (REG_RW_TIMER0_HLMT + HP_TOFST)
#define HP_TIMER_EN        (TCTL_T0EN << HP_TOFST)
#define HP_TIMER_AL        (TCTL_T0AL << HP_TOFST)

/* bus ticks at 135MHz */
#define SYSTEM_BUS_CLOCK            135000000UL

/* we tick kernel at HZ */
#define SYS_TIMER_INTERVAL          (SYSTEM_BUS_CLOCK / HZ)


static void __init mt5395_timer_init(void)
{
    u32 regval32;
    u32 _u4SysClock;

    /* disable system clock */
    regval32 = __bim_readl(REG_RW_TIMER_CTRL);
    regval32 &= ~(SYS_TIMER_EN | SYS_TIMER_AL);
    __bim_writel(regval32, REG_RW_TIMER_CTRL);

    /* setup timer interval */
    _u4SysClock = _CKGEN_GetXtalClock();
    __bim_writel(_u4SysClock / HZ, SYS_TIMER_LIMIT_REG_L);
    __bim_writel(0, SYS_TIMER_LIMIT_REG_H);

    /* reset timer */
    __bim_writel(_u4SysClock / HZ, SYS_TIMER_REG_L);
    __bim_writel(0, SYS_TIMER_REG_H);

    mt5395_mask_irq(SYS_TIMER_IRQ);
    _timer_tick_count = 0;

    /* set system irq */
    setup_irq(SYS_TIMER_IRQ, &mt5395_timer_irq);

    /* enable timer with auto-load */
    regval32 |= (SYS_TIMER_EN | SYS_TIMER_AL);
    __bim_writel(regval32, REG_RW_TIMER_CTRL);
}

static unsigned long mt5395_gettimeoffset(void)
{
    u32 limit = __bim_readl(SYS_TIMER_LIMIT_REG_L);
    u32 timer = __bim_readl(SYS_TIMER_REG_L);

    return (limit-timer) * (1000000 / HZ) / limit;
}

static struct sys_timer mt5395_timer =
{
    .init       = mt5395_timer_init,
    .offset     = mt5395_gettimeoffset,
};

unsigned long mt5395_timer_read(int nr)
{
    return __bim_readl((REG_RW_TIMER0_LOW + (nr * 8)));
}
EXPORT_SYMBOL(mt5395_timer_read);

unsigned long long mt5395_timer_read_ll(int nr)
{
    unsigned long long utime;
    unsigned long time_lo1, time_lo2;
    unsigned long time_hi1, time_hi2;

    do
    {
        time_lo1 = __bim_readl(REG_RW_TIMER0_LOW + (nr * 8));
        time_hi1 = __bim_readl(REG_RW_TIMER0_HIGH + (nr * 8));
        time_lo2 = __bim_readl(REG_RW_TIMER0_LOW + (nr * 8));
        time_hi2 = __bim_readl(REG_RW_TIMER0_HIGH + (nr * 8));
    } while (time_hi1 == time_hi2);

    utime = ((unsigned long long)time_hi1 << 32) + (unsigned long long)time_lo1;

    return utime;
}
EXPORT_SYMBOL(mt5395_timer_read_ll);


/* /proc/htimer */

static ssize_t _htimer_ticks_per_sec_read(struct file *file, char __user *buf,
                                          size_t count, loff_t *ppos)
{
    u32 ticks_per_sec;
    unsigned char tbuf[8];

    if (count != 8)
    {
        return 0;
    }

    memset(tbuf, 0, 8);
    ticks_per_sec = _CKGEN_GetXtalClock();
    tbuf[0] = (unsigned char) (ticks_per_sec & 0x000000FF);
    tbuf[1] = (unsigned char) ((ticks_per_sec & 0x0000FF00) >> 8);
    tbuf[2] = (unsigned char) ((ticks_per_sec & 0x00FF0000) >> 16);
    tbuf[3] = (unsigned char) ((ticks_per_sec & 0xFF000000) >> 24);

    if (copy_to_user(buf, (void*)(tbuf), 8))
    {
        return -EFAULT;
    }

    return 8;
}

static ssize_t _htimer_current_tick_read(struct file *file, char __user *buf,
                                         size_t count, loff_t *ppos)
{
    u32 u4TickH, u4TickL;
    unsigned char tbuf[8];

    if (count != 8)
    {
        return 0;
    }

    memset(tbuf, 0, 8);

    u4TickH = *((volatile u32 *) (0xF0008194));
    u4TickL = *((volatile u32 *) (0xF0008074));

    tbuf[0] = (unsigned char) (u4TickL & 0x000000FF);
    tbuf[1] = (unsigned char) ((u4TickL & 0x0000FF00) >> 8);
    tbuf[2] = (unsigned char) ((u4TickL & 0x00FF0000) >> 16);
    tbuf[3] = (unsigned char) ((u4TickL & 0xFF000000) >> 24);
    tbuf[4] = (unsigned char) (u4TickH & 0x000000FF);
    tbuf[5] = (unsigned char) ((u4TickH & 0x0000FF00) >> 8);
    tbuf[6] = (unsigned char) ((u4TickH & 0x00FF0000) >> 16);
    tbuf[7] = (unsigned char) ((u4TickH & 0xFF000000) >> 24);

    if (copy_to_user(buf, (void*)(tbuf), 8))
    {
        return -EFAULT;
    }

    return 8;
}

static struct file_operations _htimer_ticks_per_sec_operations =
{
    .read = _htimer_ticks_per_sec_read,
};

static struct file_operations _htimer_current_tick_operations =
{
    .read = _htimer_current_tick_read,
};

/* hack from ./fs/proc/internal.h:14*/
extern struct proc_dir_entry proc_root;

static void _proc_htimer_init(void)
{
    struct proc_dir_entry * proc_file;

    proc_file = create_proc_entry("htimer_ticks_per_sec", S_IFREG | S_IWUSR, &proc_root);
    if (proc_file)
    {
        proc_file->proc_fops = &_htimer_ticks_per_sec_operations;
    }

    proc_file = create_proc_entry("htimer_current_tick", S_IFREG | S_IWUSR, &proc_root);
    if (proc_file)
    {
        proc_file->proc_fops = &_htimer_current_tick_operations;
    }
}


/* io map */

static struct map_desc mt5395_io_desc[] __initdata =
{
    {IO_VIRT, __phys_to_pfn(IO_PHYS), IO_SIZE, MT_DEVICE},
    {0xF2000000, __phys_to_pfn(0xF2000000), 0x1000, MT_DEVICE},
    {0xF4000000, __phys_to_pfn(0xF4000000), 0x4000, MT_DEVICE},

#ifdef CONFIG_PCI    
    /* use for PCI express module */
    {PCIE_MMIO_BASE, __phys_to_pfn(PCIE_MMIO_PHYSICAL_BASE), PCIE_MMIO_LENGTH, MT_DEVICE},   
#endif /* CONFIG_PCI */

#ifdef CONFIG_FAST_COLD_REBOOT
#define RESERVED_MEMORY_SIZE (8 * 1024 * 1024)
#ifdef CONFIG_PHYSICAL_DRAM_SIZE_64M
    {0xc0000000 + 64*1024*1024 - RESERVED_MEMORY_SIZE,  __phys_to_pfn(64*1024*1024 - RESERVED_MEMORY_SIZE),  RESERVED_MEMORY_SIZE, MT_MEMORY},
#else
#ifdef CONFIG_PHYSICAL_DRAM_SIZE_128M
    {0xc0000000 + 128*1024*1024 - RESERVED_MEMORY_SIZE, __phys_to_pfn(128*1024*1024 - RESERVED_MEMORY_SIZE), RESERVED_MEMORY_SIZE, MT_MEMORY},
#else
#ifdef CONFIG_PHYSICAL_DRAM_SIZE_192M
    {0xc0000000 + 192*1024*1024 - RESERVED_MEMORY_SIZE, __phys_to_pfn(192*1024*1024 - RESERVED_MEMORY_SIZE), RESERVED_MEMORY_SIZE, MT_MEMORY},
#else
#ifdef CONFIG_PHYSICAL_DRAM_SIZE_256M
    {0xc0000000 + 256*1024*1024 - RESERVED_MEMORY_SIZE, __phys_to_pfn(256*1024*1024 - RESERVED_MEMORY_SIZE), RESERVED_MEMORY_SIZE, MT_MEMORY},
#endif
#endif
#endif
#endif
#endif // CONFIG_FAST_COLD_REBOOT
};

static struct android_pmem_platform_data android_pmem_pdata = {
    .name = "pmem",
    .cached = 1,
};

static struct platform_device android_pmem_device = {
    .name = "android_pmem",
    .id = 0,
    .dev = {.platform_data = &android_pmem_pdata},
};

static unsigned pmem_size = MT53XX_PMEM_SIZE;

static void __init mtk_pmem_allocate_memory_regions(void)
{
    void *addr;
    unsigned long size;
    size = pmem_size;

	printk(KERN_INFO "====> PMEM BOOTMEM ALLOC <=====\n");
    if (size) {
#if 0
        addr = alloc_bootmem(size);
        android_pmem_pdata.start = __pa(addr);
        android_pmem_pdata.size = size;
#endif
        pr_info("allocating %lu bytes at %p (%lx physical) for"
            " pmem arena\n", size, addr, __pa(addr));
    }
}

void mtk_pmem_set_memory_regions(u32 start, u32 size)
{
        int i;
        android_pmem_pdata.start = start;
        android_pmem_pdata.size = size;
        i = pmem_setup(&android_pmem_pdata, NULL, NULL);
        printk(KERN_INFO "====> PMEM callback <=====\n");
        printk(KERN_INFO "pmem: start: 0x%08X, size: 0x%08X\n", start, size);
}
EXPORT_SYMBOL(mtk_pmem_set_memory_regions);

static void __init mt5395_map_io(void)
{
    iotable_init(mt5395_io_desc, ARRAY_SIZE(mt5395_io_desc));
	mtk_pmem_allocate_memory_regions();
}

extern char _text, _etext;
int mt53xx_get_rodata_addr(unsigned int *start, unsigned int *end)
{
    if (start)
    {
        *start = (unsigned int) (&_text);;
    }

    if (end)
    {
        *end = (unsigned int) (&_etext);
    }

    return 0;
}
EXPORT_SYMBOL(mt53xx_get_rodata_addr);

extern char _data, _end;
int mt53xx_get_rwdata_addr(unsigned int *start, unsigned int *end)
{
    if (start)
    {
        *start = (unsigned int) (&_data);
    }

    if (end)
    {
        *end = (unsigned int) (&_end);
    }

    return 0;
}
EXPORT_SYMBOL(mt53xx_get_rwdata_addr);

/*****************************************************************************
 * PCI
 ****************************************************************************/
#ifdef CONFIG_PCI
static struct hw_pci mtk_pci __initdata = {
    .nr_controllers	= 1,
    .swizzle	= pci_std_swizzle,
    .setup		= MTK_pci_sys_setup,
    .scan		= MTK_pci_sys_scan,
    .map_irq	= MTK_pci_map_irq
};

static int __init mtk_pci_init(void)
{
    pci_common_init(&mtk_pci);

    return 0;
}

subsys_initcall(mtk_pci_init);
#endif /* CONFIG_PCI */

/*****************************************************************************
 * 53xx init
 ****************************************************************************/
#ifdef CONFIG_CACHE_L2X0
#include <asm/hardware/cache-l2x0.h>
static int __init mt53xx_l2_cache_init(void)
{
    l2x0_init((void __iomem *)0xF2000000, 0x33020000, 0);
    return 0;
}
early_initcall(mt53xx_l2_cache_init);
#endif

static void  __init mt5395_init(void)
{
    _proc_htimer_init();
    platform_device_register(&mt53xx_pmu_device);
    platform_device_register(&android_pmem_device);
}

MACHINE_START(MT5395, "MT5395")
    .phys_io        = MT5395_PA_UART,
    .io_pg_offst    = (MT5395_VA_UART >> 18) & 0xfffc,
    .boot_params    = MT5395_SDRAM_PA + 0x100,
    .map_io         = mt5395_map_io,
    .init_irq       = mt5395_init_irq,
    .timer          = &mt5395_timer,
    .init_machine   = mt5395_init,
MACHINE_END

