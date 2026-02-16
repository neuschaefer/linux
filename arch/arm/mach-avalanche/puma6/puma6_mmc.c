/*
 * puma6_mmc.c
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2012 Intel Corporation. All rights reserved.
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
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *    Intel Corporation
 *    2200 Mission College Blvd.
 *    Santa Clara, CA  97052
 *
 */


#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/resource.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <puma6.h>
#include <puma6_hardware.h>



/***********************************************************************
 *       Puma6 SD Host Controller section (eMMC Controller)
 ***********************************************************************/
struct avalanche_mmc_platform_data {
	/* board specific information */
	u16 	temp; /* for future usage */
};

/* Platform specifc information, can be changed from platform to platform */
struct avalanche_mmc_platform_data puma6_mmc_platform_data = {
    .temp = 0, /* for future usage */
	};



static struct resource puma6_mmc_resources[] = {
        [0] = {
                .start          = (AVALANCHE_EMMC_HOST_MBAR_BASE),
                .end            = (AVALANCHE_EMMC_HOST_MBAR_BASE) + 0x1FF,
                .flags          = IORESOURCE_MEM,
        },
        [1] = {
                .start          = (AVALANCHE_AEP_EMMC_HOST_BASE),
                .end            = (AVALANCHE_AEP_EMMC_HOST_BASE) + 0x1FF,
                .flags          = IORESOURCE_MEM,
        },
        [2] = {
                .start          = AVALANCHE_EMMC_INT, 
                .end            = AVALANCHE_EMMC_INT,
                .flags          = IORESOURCE_IRQ,
        },
};


/* Flash eMMC Device */
static struct platform_device puma6_mmc_device = {
        .name           = "sdhci-puma6",         /* this name used to for driver matching */
        .id             = 0,                     /* the device instance number, or else "-1" to indicate there's only one. */
        .dev = {
               .platform_data = &puma6_mmc_platform_data,
        }, 
        .num_resources  = ARRAY_SIZE(puma6_mmc_resources),
        .resource       = puma6_mmc_resources,
};


static struct platform_device *mmc_controller_devices[] __initdata = {
    &puma6_mmc_device,
};



static int puma6_mmc_init(void)
{

    /* Register the mmc controller with platform bus*/
    platform_add_devices( mmc_controller_devices, ARRAY_SIZE(mmc_controller_devices));


	return 0;
}
subsys_initcall(puma6_mmc_init); 

