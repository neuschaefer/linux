/*
 *
 * puma5_spi.c  
 * Description:
 * SPI Platform device data for Puma5 SoC.
 *
 *
 * Copyright (C) 2008, Texas Instruments, Incorporated
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
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
#include <asm-arm/arch-avalanche/generic/pal.h>
#include <linux/spi/spi.h>
#include <linux/spi/avalanche_spi.h>
#include <linux/spi/spi_ticodec.h>
#include <asm-arm/arch-avalanche/puma5/puma5_hardware.h>
#include <asm-arm/arch-avalanche/puma5/puma5_spi.h>

#define MAX_PUMA5_SFI_CLIENTS 	4

/***********************************************************************
 *       MMSPI Controller and Slave devices section
 ***********************************************************************/

int spi_activate_cs( u8 cs, u8 pol, struct ctlr_cs_sel_t *ctlr_cs_sel)
{
	int ret_val = 0;
	ctlr_cs_sel->cs = cs;
	ctlr_cs_sel->pol = pol;
	return ret_val;
}

struct avalanche_sfi_dev_info_t spansion_sfi_info = {
	.mode = CONFIG_SFL_SFI,
	.sfi_base = MM_SPI_0_VIRT, /* form puma5_hardware.h */
    .initialized = 0,
};

struct avalanche_sfi_dev_info_t spansion1_sfi_info = {
	.mode = CONFIG_SFL_SFI,
	.sfi_base = MM_SPI_1_VIRT, /* form puma5_hardware.h */
    .initialized = 0,
};

struct avalanche_sfi_dev_info_t winbond_sfi_info = {
	.mode = CONFIG_SFL_SFI,
	.sfi_base = MM_SPI_0_VIRT, /* Got form puma5_hardware.h */
    .initialized = 0,
};


struct avalanche_spi_platform_data puma5_spi_platform_data = {

        .initial_spmode         = AVALANCHE_SPI_INIT_SPMODE,
        .bus_num                = AVALANCHE_SPI_BUS_NUM,
        .max_chipselect         = AVALANCHE_SPI_MAX_CHIPSELECT,        
        .activate_cs            = spi_activate_cs,
        .deactivate_cs          = NULL, 
		/* four our controller cs is taken care by framelength */
        .sysclk                 = AVALANCHE_DEFAULT_SYS_CLK,
        .addr_mode              = AVALANCHE_SPI_3_BYTE_ADDR_MODE,
};

static struct resource puma5_spi_resources[] = {
        [0] = {
                .start          = AVALANCHE_SPI_BASE,
                .end            = AVALANCHE_SPI_BASE + AVALANCHE_SPI_SIZE,
                .flags          = IORESOURCE_MEM,
        },
        [1] = {
                .start          = AVALANCHE_SPI_INT,
                .end            = AVALANCHE_SPI_INT,
                .flags          = IORESOURCE_IRQ,
        },
};


static struct platform_device puma5_spi_device = {
        .name           = "ti_spi",
        .id             = 0,
        .dev = {
               .platform_data = &puma5_spi_platform_data,
        },
        .num_resources  = ARRAY_SIZE(puma5_spi_resources),
        .resource       = puma5_spi_resources,
};


#ifdef CONFIG_SPI_TI_CODEC
/******************************************************************************
 *            CODEC SPI Controller and slave devices section
 ******************************************************************************/
int codec_spi_activate_cs( u8 cs, u8 pol, struct ti_ctlr_cs_sel_t *ctlr_cs_sel)
{
    int ret_val = 0;

	/* this is a system level register used to drive chip select to TIDs */
	*(CODEC_SPI_CS_SEL_REG) =  cs;

    ctlr_cs_sel->cs = cs;
    ctlr_cs_sel->pol = pol;
    return ret_val;
}

struct ti_codec_spi_platform_data puma5_codec_spi_platform_data = {
        .initial_spmode         = AVALANCHE_SPI_INIT_SPMODE,
        .bus_num                = AVALANCHE_CODEC_SPI_BUS_NUM,
        .max_chipselect         = AVALANCHE_CODEC_SPI_MAX_CHIPSELECT,
        .activate_cs            = codec_spi_activate_cs,
        .deactivate_cs          = NULL,
        /* four our controller cs is taken care by framelength */
        .sysclk                 = AVALANCHE_DEFAULT_SYS_CLK,
};

static struct resource puma5_codec_spi_resources[] = {
        [0] = {
                .start          = AVALANCHE_CODEC_SPI_BASE,
                .end            = AVALANCHE_CODEC_SPI_BASE + AVALANCHE_SPI_SIZE,
                .flags          = IORESOURCE_MEM,
        },
        [1] = {
                .start          = AVALANCHE_CODEC_SPI_INT,
                .end            = AVALANCHE_CODEC_SPI_INT,
                .flags          = IORESOURCE_IRQ,
        },
};

static struct platform_device puma5_codec_spi_device = {
        .name           = "ti_codec_spi",
        .id             = 0,
        .dev = {
               .platform_data = &puma5_codec_spi_platform_data,
        },
        .num_resources  = ARRAY_SIZE(puma5_spi_resources),
        .resource       = puma5_codec_spi_resources,
};
#endif /* CONFIG_SPI_TI_CODEC */


/*********************************************************************************
 *           Generic Section 
 *********************************************************************************/

/*Put slave specific information */
static struct spi_board_info puma5_spi_board_info[] = {
#ifdef CONFIG_MTD_WINBOND
	{
 	/* This name is very important becoz the protocol or slave driver will probe
     * only using this name for hooking the driver to the corresponding device
	 */
        .modalias        = "winbond",
        .platform_data   = (void*)NULL,
		.controller_data = &winbond_sfi_info, 
        .mode            = SPI_MODE_0,
        .irq             = 0, 
        .max_speed_hz    = 25*1000*1000,  
        .bus_num         = 0,
		.chip_select 	 = AVALANCHE_SPI_CS0,
	},
#else
	{
 	/* This name is very important becoz the protocol or slave driver will probe
     * only using this name for hooking the driver to the corresponding device
	 */
        .modalias        = "spansion",
        .platform_data   = (void*)NULL,
		.controller_data = &spansion_sfi_info, 
        .mode            = SPI_MODE_0,
        .irq             = 0, 
        .max_speed_hz    = CONFIG_AVALANCHE_SFL_CLK,  
        .bus_num         = 0,
		.chip_select 	 = AVALANCHE_SPI_CS0,
	},
	{
 	/* This name is very important becoz the protocol or slave driver will probe
     * only using this name for hooking the driver to the corresponding device
	 */
        .modalias        = "spansion1",
        .platform_data   = (void*)NULL,
		.controller_data = &spansion1_sfi_info, 
        .mode            = SPI_MODE_0,
        .irq             = 0, 
        .max_speed_hz    = CONFIG_AVALANCHE_SFL_CLK,  
        .bus_num         = 0,
		.chip_select 	 = AVALANCHE_SPI_CS1,
	},
#endif

#ifdef CONFIG_SPI_TI_CODEC
    {
    /* This name is very important becoz the protocol or slave driver will probe
     * only using this name for hooking the driver to the corresponding device
     */
        .modalias        = "tid_0",
        .platform_data   = (void*)NULL,
        .controller_data = (void*)NULL,
        .mode            = SPI_MODE_0 ,//| SPI_3_WIRE,
        .irq             = 0,
        .max_speed_hz    = (5*1000*1000),
        .bus_num         = AVALANCHE_CODEC_SPI_BUS_NUM,
        .chip_select     = AVALANCHE_SPI_CS0,
    },
    {
    /* This name is very important becoz the protocol or slave driver will probe
     * only using this name for hooking the driver to the corresponding device
     */
        .modalias        = "tid_1",
        .platform_data   = (void*)NULL,
        .controller_data = (void*)NULL,
        .mode            = SPI_MODE_0 ,//| SPI_3_WIRE,
        .irq             = 0,
        .max_speed_hz    = (5*1000*1000),
        .bus_num         = AVALANCHE_CODEC_SPI_BUS_NUM,
        .chip_select     = AVALANCHE_SPI_CS1,
    }
#endif /* CONFIG_SPI_TI_CODEC */
};

/* This function registers the spi mastere device with the platform and the spi
 *slave devices with the spi bus 
 */

static struct platform_device *spi_controller_devices[] __initdata = {
    &puma5_spi_device,
#ifdef CONFIG_SPI_TI_CODEC
    &puma5_codec_spi_device,
#endif /* CONFIG_SPI_TI_CODEC */
};



static int puma5_spi_init(void)
{

#ifdef CONFIG_SPI_TI_CODEC
	/* if already out of reset then this state change */
	/* would not happen */
    PAL_sysResetCtrl((INT32)PSC_TDM, OUT_OF_RESET);
#endif

    /* Register the slave devices present in the board with SPI subsytem */
    spi_register_board_info( puma5_spi_board_info,
							 ARRAY_SIZE(puma5_spi_board_info));

	/* Register the master controller with platform */
    platform_add_devices( spi_controller_devices, 
						  ARRAY_SIZE(spi_controller_devices));


	return 0;
}
subsys_initcall(puma5_spi_init);
