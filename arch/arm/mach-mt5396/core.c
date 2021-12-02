/*
 *  linux/arch/arm/mach-mt5396/core.c
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
#include <linux/bootmem.h>

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
#ifdef CONFIG_PCI
#include <plat/mtk_pci.h>
#endif

#ifdef CONFIG_ARM_GIC
#include <asm/hardware/gic.h>
static void (*gic_mask_irq)(unsigned int irq) = NULL;
static void (*gic_unmask_irq)(unsigned int irq) = NULL;
static void (*gic_ack_irq)(unsigned int irq) = NULL;
#endif

#include <linux/usb/android_composite.h>
#include <linux/platform_device.h>

#ifdef ANDROID
// PMEM related
#include <linux/platform_device.h>
#include <linux/bootmem.h>
#include <linux/android_pmem.h>
#define MT53XX_PMEM_SIZE 0x2F00000


int pmem_setup(struct android_pmem_platform_data *pdata,
	       long (*ioctl)(struct file *, unsigned int, unsigned long),
	       int (*release)(struct inode *, struct file *));
#endif

DEFINE_SPINLOCK(mt53xx_bim_lock);
EXPORT_SYMBOL(mt53xx_bim_lock);

static unsigned long mt53xx_reserve_start, mt53xx_reserve_size;

void mt53xx_get_reserved_area(unsigned long *start, unsigned long *size)
{
    *start = mt53xx_reserve_start;
    *size = mt53xx_reserve_size;
}
EXPORT_SYMBOL(mt53xx_get_reserved_area);

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

static inline u32 __pdwnc_readl(u32 regaddr32)
{
        return __raw_readl(PDWNC_VIRT + regaddr32);
}

static inline void __pdwnc_writel(u32 regval32, u32 regaddr32)
{
        __raw_writel(regval32, PDWNC_VIRT + regaddr32);
}

void __pdwnc_reboot_in_kernel(void)
{
        __pdwnc_writel(0x7fff0000, 0x104);
        __pdwnc_writel((__pdwnc_readl(0x100) & 0x00ffffff) | 0x01, 0x100);
}

static inline int _bim_is_vector_valid(u32 irq_num)
{
    return (irq_num < NR_IRQS);
}


static void mt53xx_mask_ack_irq(unsigned int irq)
{
    u32 regval32;
    unsigned long flags;

    if (!_bim_is_vector_valid(irq))
    {
        while (1)
            ;
        return;
    }

#ifdef CONFIG_ARM_GIC
    gic_mask_irq(irq);
    gic_ack_irq(irq);
#endif
    if (irq >= VECTOR_MISC2_BASE)
    {
        u32 misc_irq;
        misc_irq = _MISC2IRQ(irq);
        spin_lock_irqsave(&mt53xx_bim_lock, flags);
        regval32 = __bim_readl(REG_RW_M2INTEN);
        regval32 &= ~misc_irq;
        __bim_writel(regval32, REG_RW_M2INTEN);
        __bim_writel(misc_irq, REG_RW_M2INTCLR);
        spin_unlock_irqrestore(&mt53xx_bim_lock, flags);
    }
    else if (irq >= VECTOR_MISC_BASE)
    {
        u32 misc_irq;
        misc_irq = _MISCIRQ(irq);
        spin_lock_irqsave(&mt53xx_bim_lock, flags);
        regval32 = __bim_readl(REG_RW_MINTEN);
        regval32 &= ~misc_irq;
        __bim_writel(regval32, REG_RW_MINTEN);
        if (irq != VECTOR_DRAMC)
        {
            __bim_writel(misc_irq, REG_RW_MINTCLR);
        }
        spin_unlock_irqrestore(&mt53xx_bim_lock, flags);
    }
    else
    {
        spin_lock_irqsave(&mt53xx_bim_lock, flags);
        regval32 = __bim_readl(REG_RW_IRQEN);
        regval32 &= ~(1 << (irq-SPI_OFFSET));
        __bim_writel(regval32, REG_RW_IRQEN);
        if (irq != VECTOR_DRAMC)
        {
            __bim_writel((1 << (irq-SPI_OFFSET)), REG_RW_IRQCLR);
        }
        spin_unlock_irqrestore(&mt53xx_bim_lock, flags);
    }
}

static void mt53xx_mask_irq(unsigned int irq)
{
    u32 regval32;
    unsigned long flags;

    if (!_bim_is_vector_valid(irq))
    {
        while (1)
            ;
        return;
    }

#ifdef CONFIG_ARM_GIC
    gic_mask_irq(irq);
#endif
    if (irq >= VECTOR_MISC2_BASE)
    {
        u32 misc_irq;
        misc_irq = _MISC2IRQ(irq);
        spin_lock_irqsave(&mt53xx_bim_lock, flags);
        regval32 = __bim_readl(REG_RW_M2INTEN);
        regval32 &= ~misc_irq;
        __bim_writel(regval32, REG_RW_M2INTEN);
        spin_unlock_irqrestore(&mt53xx_bim_lock, flags);
    }
    else if (irq >= VECTOR_MISC_BASE)
    {
        u32 misc_irq;
        misc_irq = _MISCIRQ(irq);
        spin_lock_irqsave(&mt53xx_bim_lock, flags);
        regval32 = __bim_readl(REG_RW_MINTEN);
        regval32 &= ~misc_irq;
        __bim_writel(regval32, REG_RW_MINTEN);
        spin_unlock_irqrestore(&mt53xx_bim_lock, flags);
    }
    else
    {
        spin_lock_irqsave(&mt53xx_bim_lock, flags);
        regval32 = __bim_readl(REG_RW_IRQEN);
        regval32 &= ~(1 << (irq-SPI_OFFSET));
        __bim_writel(regval32, REG_RW_IRQEN);
        spin_unlock_irqrestore(&mt53xx_bim_lock, flags);
    }
}

static void mt53xx_unmask_irq(unsigned int irq)
{
    u32 regval32;
    unsigned long flags;

    if (irq >= VECTOR_MISC2_BASE)
    {
        u32 misc_irq;
        misc_irq = _MISC2IRQ(irq);
        spin_lock_irqsave(&mt53xx_bim_lock, flags);
        regval32 = __bim_readl(REG_RW_M2INTEN);
        regval32 |= (misc_irq);
        __bim_writel(regval32, REG_RW_M2INTEN);
        spin_unlock_irqrestore(&mt53xx_bim_lock, flags);
    }
    else if (irq >= VECTOR_MISC_BASE)
    {
        u32 misc_irq;
        misc_irq = _MISCIRQ(irq);
        spin_lock_irqsave(&mt53xx_bim_lock, flags);
        regval32 = __bim_readl(REG_RW_MINTEN);
        regval32 |= (misc_irq);
        __bim_writel(regval32, REG_RW_MINTEN);
        spin_unlock_irqrestore(&mt53xx_bim_lock, flags);
    }
    else
    {
        spin_lock_irqsave(&mt53xx_bim_lock, flags);
        regval32 = __bim_readl(REG_RW_IRQEN);
        regval32 |= (1 << (irq-SPI_OFFSET));
        __bim_writel(regval32, REG_RW_IRQEN);
        spin_unlock_irqrestore(&mt53xx_bim_lock, flags);
    }
#ifdef CONFIG_ARM_GIC
    gic_unmask_irq(irq);
#endif
}

static struct irq_chip mt53xx_irqchip =
{
    .mask_ack    = mt53xx_mask_ack_irq,
    .mask   = mt53xx_mask_irq,
    .unmask = mt53xx_unmask_irq,
};


static struct resource mt53xx_pmu_resource[] = {
    [0] = {
        .start  = VECTOR_PMU0,
        .end    = VECTOR_PMU0,
        .flags  = IORESOURCE_IRQ,
    },
#ifdef CONFIG_MACH_MT5396
    [1] = {
        .start  = VECTOR_PMU1,
        .end    = VECTOR_PMU1,
        .flags  = IORESOURCE_IRQ,
    },
#endif
};

static struct platform_device mt53xx_pmu_device = {
    .name       = "arm-pmu",
    .id     = ARM_PMU_DEVICE_CPU,
    .resource   = mt53xx_pmu_resource,
    .num_resources  = ARRAY_SIZE(mt53xx_pmu_resource),
};



static void __init mt53xx_init_machine_irq(void)
{
    unsigned long flags;
    unsigned int irq;
    u32 u4Reg;
    
#ifdef CONFIG_ARM_GIC
    struct irq_chip *gic;
    gic_dist_init(0, (void __iomem *)MPCORE_GIC_DIST_VIRT, 29);
    gic_cpu_init(0, (void __iomem *)MPCORE_GIC_CPU_VIRT);

    gic = get_irq_chip(29);
    gic_unmask_irq = gic->unmask;
    gic_mask_irq = gic->mask;
    gic_ack_irq = gic->ack; 
    mt53xx_irqchip.set_affinity=gic->set_affinity;
#endif

    spin_lock_irqsave(&mt53xx_bim_lock, flags);

    /* turn all irq off */
    __bim_writel(0, REG_RW_IRQEN);
    __bim_writel(0, REG_RW_MINTEN);
    __bim_writel(0, REG_RW_M2INTEN);

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
    __bim_writel(0xffffffff, REG_RW_M2INTCLR);
    __bim_writel(0xffffffff, REG_RW_M2INTCLR);
    __bim_writel(0xffffffff, REG_RW_M2INTCLR);
    __bim_writel(0xffffffff, REG_RW_M2INTCLR);
    __bim_writel(0xffffffff, REG_RW_M2INTCLR);


    for (irq = SPI_OFFSET; irq < NR_IRQS; irq++)
    {
        set_irq_chip(irq, &mt53xx_irqchip);
        set_irq_handler(irq, handle_level_irq);
        set_irq_flags(irq, IRQF_VALID | IRQF_PROBE);
    }
        
    /* turn misc and misc2 irq on */
    __bim_writel(0xc0000000, REG_RW_IRQEN);

    /* set pmu irq to level triggered */
    u4Reg = __bim_readl(REG_RW_MISC);
    u4Reg |= 0x100;
    __bim_writel(u4Reg, REG_RW_MISC);

    /* for some reason, people believe we have to do above */
    spin_unlock_irqrestore(&mt53xx_bim_lock, flags);
    
}

/* io map */

static struct map_desc mt53xx_io_desc[] __initdata =
{
    {IO_VIRT, __phys_to_pfn(IO_PHYS), IO_SIZE, MT_DEVICE},
    {0xF2000000, __phys_to_pfn(0xF2000000), 0x1000, MT_DEVICE},
    {0xF4000000, __phys_to_pfn(0xF4000000), 0x4000, MT_DEVICE},

    {DMX_SRAM_VIRT, __phys_to_pfn(0xFB005000), 0x1000, MT_DEVICE}, // ldr env temp copy
#ifdef CONFIG_CPU_V7
    {0xF2002000, __phys_to_pfn(0xF2002000), SZ_16K, MT_DEVICE},
#endif
#ifdef CONFIG_PCI    
    /* use for PCI express module */
    {PCIE_MMIO_BASE, __phys_to_pfn(PCIE_MMIO_PHYSICAL_BASE), PCIE_MMIO_LENGTH, MT_DEVICE},   
#endif /* CONFIG_PCI */

    {0, 0, 0, MT_MEMORY},  // Space holder for FBM mapping. !!!!! MUST STAY IN LAST ENTRY !!!!!
};

#ifdef ANDROID
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
#endif

static void __init mt53xx_map_io(void)
{
    int io_desc_size = ARRAY_SIZE(mt53xx_io_desc);
    unsigned long hole, b0end;

    // Check if we want to map FBM in place.
    // We prefer to map FBM in place, because it will use section mapping
    // (page size=1M). This could reduce TLB miss and save some RAM (pte space)
    // Also, it can save vmlloc space if there's a hole (2 membank)
    if (meminfo.nr_banks >= 2)
    {
        b0end = meminfo.bank[0].start + meminfo.bank[0].size;
        hole = meminfo.bank[1].start - b0end;
        if (meminfo.bank[1].start <= 512*1024*1024)
        {
            mt53xx_reserve_start = b0end;
            mt53xx_reserve_size = hole;
        }
    }
    else
    {
        // !!!!!FIXME!!!!! Need a better way to handle this.
        // Hueristic to guess how many memory we have and possible size of FBM.
        b0end = meminfo.bank[0].start + meminfo.bank[0].size;
        if (b0end <= 160*1024*1024)
        {
            mt53xx_reserve_start = b0end;
            mt53xx_reserve_size = 256*1024*1024 - b0end;
        }
        else if (b0end <= 512*1024*1024)
        {
            mt53xx_reserve_start = b0end;
            mt53xx_reserve_size = 512*1024*1024 - b0end;
        }
    }

    if (mt53xx_reserve_start)
    {
        struct map_desc *desc = &mt53xx_io_desc[io_desc_size-1];

        desc->virtual = (unsigned long)phys_to_virt(mt53xx_reserve_start);
        desc->pfn = __phys_to_pfn(mt53xx_reserve_start);
        desc->length = mt53xx_reserve_size;
    }
    else
        io_desc_size--;

    iotable_init(mt53xx_io_desc, io_desc_size);

#ifdef ANDROID
    mtk_pmem_allocate_memory_regions();
#endif
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
    .nr_controllers = 1,
    .swizzle    = pci_std_swizzle,
    .setup      = MTK_pci_sys_setup,
    .scan       = MTK_pci_sys_scan,
    .map_irq    = MTK_pci_map_irq
};

static int __init mtk_pci_init(void)
{
    pci_common_init(&mtk_pci);

    return 0;
}

subsys_initcall(mtk_pci_init);
#endif /* CONFIG_PCI */

/*****************************************************************************
 * RTC
 ****************************************************************************/

#ifdef CONFIG_RTC_DRV_MT53XX
static struct resource mt53xx_rtc_resource[] = {
    [0] = {
//        .start  = VECTOR_PDWNC,
//        .end    = VECTOR_PDWNC,
        .start  = -1,//TODO
        .end    = -1,
        .flags  = IORESOURCE_IRQ,
    },
};
static struct platform_device mt53xx_rtc_device = {
	.name		= "mt53xx_rtc",
	.id		= 0,
    .resource   = mt53xx_rtc_resource,
    .num_resources  = ARRAY_SIZE(mt53xx_rtc_resource),
};

static void __init mt53xx_add_device_rtc(void)
{
	platform_device_register(&mt53xx_rtc_device);
}
#else
static void __init mt53xx_add_device_rtc(void) {}
#endif

#ifdef ANDROID
static char *usb_functions_adb_ums[] = { "adb" };

static char *usb_functions_all[] = { "adb"};

struct android_usb_product usb_products[] = {
    {
    .product_id = 0x0d02, 
//    .product_id = 0x0005, 
    .num_functions = ARRAY_SIZE(usb_functions_adb_ums),
    .functions      = usb_functions_adb_ums,
    },
};

/* standard android USB platform data */
static struct android_usb_platform_data andusb_plat = {
        .vendor_id              = 0x18d1,
//        .vendor_id              = 0x0E8D,
        .product_id             = 0x0004,    
        .manufacturer_name      = "MediaTek Inc.",    
        .product_name           = "MTK_adb",    
        .serial_number          = "AndroidTV",   
        .num_products           = ARRAY_SIZE(usb_products),    
        .products               = usb_products,    
        .num_functions          = ARRAY_SIZE(usb_functions_all),    
        .functions              = usb_functions_all,    
};
        
static struct platform_device androidusb_device = {    
        .name           = "android_usb",    
        .id             = -1,    
        .dev            = {    
                .platform_data  = &andusb_plat,    
        },    
};
#endif
/*****************************************************************************
 * 53xx init
 ****************************************************************************/

extern void mt53xx_proc_htimer_init(void);
static void  __init mt53xx_init_machine(void)
{
    mt53xx_proc_htimer_init();

    platform_device_register(&mt53xx_pmu_device);
#ifdef ANDROID
    platform_device_register(&android_pmem_device);
    platform_device_register(&androidusb_device);
#endif
    mt53xx_add_device_rtc();
}

extern struct sys_timer mt53xx_timer;


#ifdef CONFIG_CACHE_L2X0
#include <asm/hardware/cache-l2x0.h>

#ifdef CONFIG_CPU_V7
static int __init mt53xx_l2_cache_init(void)
{
    register unsigned int reg;
/*
[30] Early BRESP enable 0 = Early BRESP disabled. This is the default. 1 = Early BRESP enabled.
[29] Instruction prefetch enable 0 = Instruction prefetching disabled. This is the default. 1 = Instruction prefetching enabled.
[28] Data prefetch enable 0 = Data prefetching disabled. This is the default. 1 = Data prefetching enabled.
[24:23] Force write allocate b00 = Use AWCACHE attributes for WA. This is the default. 
b01 = Force no allocate, set WA bit always 0.
b10 = Override AWCACHE attributes, set WA bit always 1, all cacheable write misses become write allocated.
b11 = Internally mapped to 00.
[12] Exclusive cache configuration 0 = Disabled. This is the default. 1 = Enabled
[0] Full Line of Zero Enable 0 = Full line of write zero behavior disabled. This is the default. 1 = Full line of write zero behavior
normal:0xf2000104=0x73041001
benchmark:0xf2000104=0x72040001
*/
    l2x0_init((void __iomem *)L2C_BASE, 0x71001001, 0x8e7feffe);
    __asm__ ("MRC     p15, 0, %0, c1, c0, 1" : "=r" (reg));
    __asm__ ("ORR     %0, %1, #0x8" : "=r" (reg) : "r" (reg)); //FOZ=0x8
    __asm__ ("MCR     p15, 0, %0, c1, c0, 1" : : "r" (reg));
    return 0;
}
#else
static int __init mt53xx_l2_cache_init(void)
{
    l2x0_init((void __iomem *)L2C_BASE, 0x31000000, 0xce7fffff);
    return 0;
}
#endif /* CONFIG_CPU_V7 */
early_initcall(mt53xx_l2_cache_init);
#endif


#ifdef CONFIG_ZONE_DMA
/*
 * Adjust the zones if there are restrictions for DMA access.
 */
void __init mt53xx_adjust_zones(unsigned long *size,
				  unsigned long *hole)
{
	unsigned long dma_size = SZ_512M >> PAGE_SHIFT;

	if (size[0] <= dma_size)
		return;

	size[ZONE_NORMAL] = size[0] - dma_size;
	size[ZONE_DMA] = dma_size;
}
#endif /* CONFIG_ZONE_DMA */

#ifdef CONFIG_DMABOUNCE
int dma_needs_bounce(struct device *dev, dma_addr_t dma_addr, size_t size)
{
	return ((dma_addr + size) >= SZ_512M);
}
#endif


#ifdef CONFIG_ARCH_MT5368
MACHINE_START(MT5368, "MT5368")
#elif defined(CONFIG_MACH_MT5396)
MACHINE_START(MT5396, "MT5396")
#elif defined(CONFIG_MACH_MT5369)
MACHINE_START(MT5396, "MT5369")
#elif defined(CONFIG_MACH_MT5389)
MACHINE_START(MT5389, "MT5389")
#else
#error Undefined mach/arch
#endif
    .phys_io        = MT53xx_PA_UART,
    .io_pg_offst    = (MT53xx_VA_UART >> 18) & 0xfffc,
    .boot_params    = MT53xx_SDRAM_PA + 0x100,
    .map_io         = mt53xx_map_io,
    .init_irq       = mt53xx_init_machine_irq,
    .timer          = &mt53xx_timer,
    .init_machine   = mt53xx_init_machine,
MACHINE_END
