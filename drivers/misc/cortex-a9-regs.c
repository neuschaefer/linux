/*
 * Copyright (C) 2011 Broadcom Corporation
 *          Alamy Liu <alamy.liu@broadcom.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/io.h>                   // ioremap() / iounmap()
#include <linux/cpumask.h>              // num_online_cpus()
#include <linux/sched.h>                // set_cpus_allowed()
//#include <linux/preempt.h>


typedef struct __CORTEX_A9_REGS_DATA
{
    struct dentry *root_dir;

    struct dentry *base_addr_file;
    u64     base_addr;                  // Base address of every peripheral/region

    struct dentry *gic_dir, *gic_dist_file;
    u64     gic_dist;

    u32     peri_base_PA;               // Physical Address of PERIPHBASE (Peripheral Base)
    u32     gic_intrif_base;            // Interrupt Interface (Virtual address)
    u32     gic_dist_base;              // GIC Distributor base address (Virtual address)

}   CORTEX_A9_REGS_DATA;

CORTEX_A9_REGS_DATA regs_data;

#if 0
static struct platform_driver cortex_a9_regs_driver =
{
    .driver = {
        .name = "cortex_a9_regs",
    },
    .probe  = cortex_a9_regs_probe,
    .remove = __devexit_p(
};
#endif


static int debugfs_base_addr_get(void *data, u64 *val)
{
    printk(KERN_INFO "CBAR (PA)                 = 0x%08x\n", regs_data.peri_base_PA);
    printk(KERN_INFO "GIC Interrupt Interface   = 0x%08x\n", regs_data.gic_intrif_base);
    printk(KERN_INFO "GIC Distributor           = 0x%08x\n", regs_data.gic_dist_base);

//    regs_data.gic_intrif_base   = ioremap(regs_data.peri_base_PA + 0x0100, 0x0100); // 0x0100..0x01FF

    return 0;
}
DEFINE_SIMPLE_ATTRIBUTE(fops_base_addr, debugfs_base_addr_get, NULL, "%llu\n");

static int dump_gic_dist_registers(void)
{
    void __iomem    *gic_dist_base_va;
    u32             dump_addr;

    gic_dist_base_va = ioremap(regs_data.peri_base_PA + 0x1000, 0x1000);
    if (gic_dist_base_va == NULL)
    {
        printk(KERN_WARNING "Fail to map GIC Distributor base address\n");
        return -ENOMEM;
    }

    printk(KERN_INFO "----- CPU %d GIC Distributor registers -----\n", raw_smp_processor_id());
//    for (dump_addr = 0x0; dump_addr < 0x1000; dump_addr += 0x10)
    for (dump_addr = 0x0100; dump_addr < 0x300; dump_addr += 0x10)  // 0x100 & 0x180
    {
        printk(KERN_INFO "0x%08X    0x%08x 0x%08x  0x%08x 0x%08x\n"
            , dump_addr
            , *(u32 *)(gic_dist_base_va + dump_addr + 0x00)
            , *(u32 *)(gic_dist_base_va + dump_addr + 0x04)
            , *(u32 *)(gic_dist_base_va + dump_addr + 0x08)
            , *(u32 *)(gic_dist_base_va + dump_addr + 0x0C)
        );
    }
    printk(KERN_INFO "\n");

    iounmap( gic_dist_base_va );

    return 0;
}

static int debugfs_gic_dist_get(void *data, u64 *val)
{
    int cpu;

    *val = *(u64 *)data;
//    printk(KERN_INFO "get value is %llu, gic_dist = %llu\n", *val, regs_data.gic_dist);

    for_each_online_cpu(cpu)
    {
        // Bind to the specified CPU.
        // When this call returns, we should be running on the right CPU
        set_cpus_allowed(current, cpumask_of_cpu(cpu));

        dump_gic_dist_registers();
    }

    return 0;
}

static int debugfs_gic_dist_set(void *data, u64 val)
{
    if (val > num_online_cpus()) {
        val = num_online_cpus() - 1;
    }
    *(u64 *)data = val;
//    printk(KERN_INFO "set value is %llu, gic_dist = %llu\n", val, regs_data.gic_dist);

    // Bind to the specified CPU.
    // When this call returns, we should be running on the right CPU
    set_cpus_allowed(current, cpumask_of_cpu(val));

    dump_gic_dist_registers();

    return 0;
}
DEFINE_SIMPLE_ATTRIBUTE(fops_gic_dist, debugfs_gic_dist_get, debugfs_gic_dist_set, "%llu\n");

static int __init cortex_a9_regs_init(void)
{
    // Set pointers to NULL.
    memset(&regs_data, 0x00, sizeof(regs_data));

    // Cortex-A9 specification
    // MRC p15, 4, <Rt>, c15, c0, 0; Read Configuration Base Address Register
    asm("mrc p15, 4, %0, c15, c0, 0" : "=r" (regs_data.peri_base_PA));
    regs_data.peri_base_PA &= ~0x000F;  // Ignore the bit [7:0] for we are 32-bit system


    // Path: /sys/kernel/debug/cortex_a9_regs/
    if ( !(regs_data.root_dir   = debugfs_create_dir("cortex_a9_regs", NULL)) )
        goto debugfs_err;

    if ( !(regs_data.base_addr_file = debugfs_create_file("base_addr", (S_IWUSR | S_IRUGO), regs_data.root_dir, &regs_data.base_addr, &fops_base_addr)) )
        goto debugfs_err;

    // Generic Interrupt Controller (GIC) (path: cortex_a9_regs/gic/)
    if ( !(regs_data.gic_dir    = debugfs_create_dir("gic", regs_data.root_dir)) )
        goto debugfs_err;

    if ( !(regs_data.gic_dist_file  = debugfs_create_file("dist", (S_IWUSR | S_IRUGO), regs_data.gic_dir, &regs_data.gic_dist, &fops_gic_dist)) )
        goto debugfs_err;

    printk(KERN_INFO "Cortex-A9 registers module installed.\n");
    return 0;


debugfs_err:
    printk(KERN_INFO "Cortex-A9 registers module installation failed!\n");

    debugfs_remove(regs_data.gic_dist_file);
    debugfs_remove(regs_data.gic_dir);
    debugfs_remove(regs_data.root_dir);

    return -1;

//    return platform_driver_register( &cortex_a9_regs_driver );
}

static void __exit cortex_a9_regs_exit(void)
{
    debugfs_remove(regs_data.gic_dist_file);
    debugfs_remove(regs_data.gic_dir);
    debugfs_remove(regs_data.root_dir);

    printk(KERN_INFO "Cortex-A9 registers module released.\n");

//    platform_driver_unregister( &cortex_a9_regs_driver );
}


#if defined(MODULE)
module_init(cortex_a9_regs_init);
module_exit(cortex_a9_regs_exit);

MODULE_AUTHOR("Alamy Liu");
MODULE_DESCRIPTION("Eavesdrop Cortex-A9 registers");
MODULE_LICENSE("GPL");

#else
    subsys_initcall(cortex_a9_regs_init);

#endif

