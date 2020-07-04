/*
<:copyright-BRCM:2013:DUAL/GPL:standard

   Copyright (c) 2013 Broadcom Corporation
   All Rights Reserved

Unless you and Broadcom execute a separate written software license
agreement governing use of this software, this software is licensed
to you under the terms of the GNU General Public License version 2
(the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php,
with the following added to such license:

   As a special exception, the copyright holders of this software give
   you permission to link this software with independent modules, and
   to copy and distribute the resulting executable under terms of your
   choice, provided that you also meet, for each linked independent
   module, the terms and conditions of the license of that module.
   An independent module is a module which is not derived from this
   software.  The special exception does not apply to any modifications
   of the software.

Not withstanding the above, under no circumstances may you combine
this software in any way with any other Broadcom software provided
under a license other than the GPL, without Broadcom's express prior
written consent.

:>
*/

/*
 ****************************************************************************
 * File Name  : bcm63xx_sata.c
 *
 * Description: This file contains the initilzation and registration routines
 * to enable sata controller on bcm63xxx boards.
 *
 *
 ***************************************************************************/

#include <linux/types.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/clkdev.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/bug.h>
#include <linux/ahci_platform.h>

#include <bcm_map_part.h>
#include <bcm_intr.h>
#include <pmc_drv.h>
#include <BPCM.h>

extern int pmc_sata_power_up(void);
extern int pmc_sata_power_down(void);

/* macros to read write, to registers, with memory barriers to avoid reordering */
#define BDEV_RD(x)      (*((volatile unsigned *)(x))); mb()
#define BDEV_WR(x, y)   do { *((volatile unsigned *)(x)) = (y); mb(); } while (0)

/*TODO move these reg definitions to map_part.h */

#define SATA_HBA_BASE_ADDR          SATA_BASE

#define SATA_TOP_CTRL               (SATA_HBA_BASE_ADDR+0x0040)
#define SATA_PORT0_PCB              (SATA_HBA_BASE_ADDR+0x0100)
#define SATA_AHCI_BASE              (SATA_HBA_BASE_ADDR+0x2000)
#define SATA_AHCI_GHC               (SATA_HBA_BASE_ADDR+0x2000)
#define SATA_AHCI_PORT0_S1          (SATA_HBA_BASE_ADDR+0x2100)

#define SATA_AHCI_GHC_PHYS          (SATA_PHYS_BASE+0x2000)

#define SATA_MEM_SIZE               0x00002000

/* SATA_TOP_CTRL regsiters */
#define SATA_TOP_CTRL_BUS_CTRL      (SATA_TOP_CTRL+0x04)

/* SATA_PORT0_AHCI_S1 registers */
#define SATA_PORT0_AHCI_S1_PXIS     (SATA_AHCI_PORT0_S1+0x10)
#define SATA_PORT0_AHCI_S1_PXIE     (SATA_AHCI_PORT0_S1+0x14)
#define SATA_PORT0_AHCI_S1_PXCMD    (SATA_AHCI_PORT0_S1+0x18)

/* GHC regs */
#define GHC_HBA_CAP                 (SATA_AHCI_GHC+0x00) /* host capabilities */
#define GHC_GLOBAL_HBA_CONTROL      (SATA_AHCI_GHC+0x04) /* global host control */
#define GHC_INTERRUPT_STATUS        (SATA_AHCI_GHC+0x08) /* interrupt status */
#define GHC_PORTS_IMPLEMENTED       (SATA_AHCI_GHC+0x0c) /* bitmap of implemented ports */
#define GHC_HOST_VERSION            (SATA_AHCI_GHC+0x10) /* AHCI spec. version compliancy */

/* Phy reg */
#define PORT0_SATA3_PCB_REG0        (SATA_PORT0_PCB+0x0200)
#define PORT0_SATA3_PCB_REG1        (SATA_PORT0_PCB+0x0204)
#define PORT0_SATA3_PCB_REG2        (SATA_PORT0_PCB+0x0208)
#define PORT0_SATA3_PCB_REG3        (SATA_PORT0_PCB+0x020c)
#define PORT0_SATA3_PCB_REG4        (SATA_PORT0_PCB+0x0210)
#define PORT0_SATA3_PCB_REG5        (SATA_PORT0_PCB+0x0214)
#define PORT0_SATA3_PCB_REG6        (SATA_PORT0_PCB+0x0218)
#define PORT0_SATA3_PCB_REG7        (SATA_PORT0_PCB+0x021c)
#define PORT0_SATA3_PCB_REG8        (SATA_PORT0_PCB+0x0220)
#define PORT0_SATA3_PCB_BLOCK_ADDR  (SATA_PORT0_PCB+0x023C)


static __init void sata_hw_init(void)
{
    WriteBPCMRegister(PMB_ADDR_SATA, BPCMRegOffset(misc_control), 0);
    WriteBPCMRegister(PMB_ADDR_SATA, BPCMRegOffset(sr_control), ~0);
    WriteBPCMRegister(PMB_ADDR_SATA, BPCMRegOffset(sr_control), 0);
}

/*TODO: modify this function once HW issue is root caused for now we use delays */
static __init void write_2_pcb_block(unsigned addr, unsigned value, unsigned pcb_block)
{
    *(volatile unsigned *)(PORT0_SATA3_PCB_BLOCK_ADDR) = pcb_block;
    wmb();
    mdelay(1);
    *(volatile unsigned *)addr = value;
    wmb();
#if 0
    /*write again */
    *(volatile unsigned *)addr = value;
    wmb();
#endif
    mdelay(1);
    /* to print read twice due to HW issue */
    //printk("++++ addr = <%p>, data=%lx\n",addr, *addr);
    //printk("++++ addr = <%p>, data=%lx\n",addr, *addr);
}

static __init void GetFreqLock( void )
{
    uint32_t regData;
    int i = 10;

    printk("writing PORT0_SATA3_PCB_BLOCK_ADDR\n");

    write_2_pcb_block(PORT0_SATA3_PCB_REG7, 0x873, 0x60);

    write_2_pcb_block(PORT0_SATA3_PCB_REG6, 0xc000, 0x60);

    write_2_pcb_block(PORT0_SATA3_PCB_REG1, 0x3089, 0x50);
    udelay(100);
    write_2_pcb_block(PORT0_SATA3_PCB_REG1, 0x3088, 0x50);
    udelay(1000);
    //// Done with PLL ratio change and re-tunning

    write_2_pcb_block(PORT0_SATA3_PCB_REG2, 0x3000, 0xE0);
    write_2_pcb_block(PORT0_SATA3_PCB_REG6, 0x3000, 0xE0);

    udelay(1000);
    write_2_pcb_block(PORT0_SATA3_PCB_REG3, 0x32, 0x50);

    write_2_pcb_block(PORT0_SATA3_PCB_REG4, 0xA, 0x50);

    write_2_pcb_block(PORT0_SATA3_PCB_REG6, 0x64, 0x50);

    udelay(1000);
    BDEV_WR(PORT0_SATA3_PCB_BLOCK_ADDR, 0x00);
    wmb();

    regData = BDEV_RD(PORT0_SATA3_PCB_REG1);

    while (i && ((regData & 0x1000) == 0))
    {
        regData = BDEV_RD(PORT0_SATA3_PCB_REG1);
        udelay(1000);
        i--;
    }
    printk("INFO: PLL lock for port0 detected %0x...\n", regData);
}

static __init void sata_sim_init(void)
{
    BDEV_WR(GHC_GLOBAL_HBA_CONTROL, 0x80000001);
    mdelay(1);
    BDEV_WR(GHC_GLOBAL_HBA_CONTROL, 0x80000000);
    mdelay(10);

    BDEV_WR(SATA_PORT0_AHCI_S1_PXIS, 0x7fffffff);
    BDEV_WR(GHC_INTERRUPT_STATUS, 0x7fffffff);
    BDEV_WR(SATA_PORT0_AHCI_S1_PXIE, 0x7fffffff);

    BDEV_WR(SATA_PORT0_AHCI_S1_PXCMD, 0x00000010);
    /* setup endianess */
    BDEV_WR(SATA_TOP_CTRL_BUS_CTRL, 0x00000000);
}

static void bcm_dev_release(struct device *dev)
{
    put_device(dev->parent);
}

static struct resource bcm_ahci_resource[] = {
    [0] = {
        .start  = SATA_AHCI_GHC_PHYS,
        .end    = SATA_AHCI_GHC_PHYS + SATA_MEM_SIZE - 1,
        .flags  = IORESOURCE_MEM,
    },
    [1] = {
        .start  = INTERRUPT_ID_SATAC,
        .end    = INTERRUPT_ID_SATAC,
        .flags  = IORESOURCE_IRQ,
    },
};

static u64 bcm_ahci_dmamask = DMA_BIT_MASK(32);

static struct platform_device bcm_ahci_pdev = {
    .name       = "strict-ahci",
    .id         = 0,
    .resource   = bcm_ahci_resource,
    .num_resources      = ARRAY_SIZE(bcm_ahci_resource),
    .dev                = {
        .dma_mask               = &bcm_ahci_dmamask,
        .coherent_dma_mask      = DMA_BIT_MASK(32),
        .release                = bcm_dev_release,
    },
};

static __init int bcm_add_sata(void)
{
    printk("++++ Powering up SATA block\n");

    pmc_sata_power_up();
    mdelay(1);

    sata_hw_init();
    mdelay(1);

    GetFreqLock();
    mdelay(1);

    sata_sim_init();
    mdelay(1);

    if(platform_device_register(&bcm_ahci_pdev))
    {
        printk(KERN_ERR "++++ Failed to add platform device for SATA \n");
    }
    return 0;
}

#if defined CONFIG_SATA_AHCI_MODULE
static void bcm_mod_cleanup(void)
{
    platform_device_del(&bcm_ahci_pdev);
    pmc_sata_power_down();
    mdelay(1);
}

module_init(bcm_add_sata);
module_exit(bcm_mod_cleanup);

MODULE_LICENSE("GPL");
#else
arch_initcall(bcm_add_sata);
#endif

MODULE_LICENSE("GPL");
