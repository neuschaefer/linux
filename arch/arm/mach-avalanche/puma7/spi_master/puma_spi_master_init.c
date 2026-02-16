/*
 *
 * puma_spi_master_init.c
 * Description:
 * SPI Platform device data for Puma7 SoC.
 *
 *
 * GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2014-2017 Intel Corporation.
 *
 *  This program is free software; you can redistribute it and/or modify 
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 *  General Public License for more details.
 *
 *
 *  You should have received a copy of the GNU General Public License 
 *  along with this program; if not, write to the Free Software 
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution 
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *  Intel Corporation
 *  2200 Mission College Blvd.
 *  Santa Clara, CA  97052
 */

#include <asm-arm/arch-avalanche/puma7/spi_master/puma_spi_master.h>

#include <asm-arm/arch-avalanche/puma7/puma7.h>
#include <asm-arm/arch-avalanche/generic/pal.h>

#define PUMA7_SPI_IO_BASE (AVALANCHE_SPI_MASTER)
#define PUMA7_SPI_IO_SIZE (0x10C)


static struct resource puma_spi_master_resources = {
    .start = PUMA7_SPI_IO_BASE,
    .end   = PUMA7_SPI_IO_BASE + PUMA7_SPI_IO_SIZE,
    .flags = IORESOURCE_MEM
};

static struct platform_device puma_spi_master_device = {
    .name          = "puma_spi_master",
    .id            = -1,
    .num_resources = 1,
    .resource      = &puma_spi_master_resources
};


static int __init puma_spi_init(void)
{
    int ret;
    printk(KERN_INFO "SPI MASTER: Initializing Intel(R) SPI MASTER driver\n");

    /* Register SPI master controller */
    ret = puma_spi_master_init(&puma_spi_master_device);
    if (ret < 0) {
        goto out;
    }

    PAL_sysResetCtrl(CRU_NUM_SPI_MASTER, OUT_OF_RESET);

    /* Create SPI proxy driver for user-space access */
    ret = puma_spi_proxy_init();
    if (ret < 0) {
        goto out;
    }
    
out:
    return ret;
}

subsys_initcall(puma_spi_init);

/* Driver identification */
MODULE_DESCRIPTION("SPI MASTER Device Driver");
MODULE_LICENSE("GPL");
