/*
 *
 * puma6_spi.c  
 * Description:
 * SPI Platform device data for Puma6 SoC.
 *
 *
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
#include <linux/spi/spi.h>
#include <linux/spi/avalanche_spi.h>
#include <linux/spi/spi_ticodec.h>
#include <puma6_hardware.h>
#include <puma6_spi.h>
#include <puma6_cru_ctrl.h>
#include <puma6_bootcfg_ctrl.h>


/***********************************************************************
 *       Puma6 SPI Slave devices section
 ***********************************************************************/


struct avalanche_sfi_dev_info_t flash0_info = {
	.mode = CONFIG_SFL_SFI,
	.sfi_base = MM_SPI_VIRT, /* form puma6_hardware.h */
    .initialized = 0,
};

struct avalanche_sfi_dev_info_t flash1_info = {
	.mode = CONFIG_SFL_SFI,
	.sfi_base = MM_SPI_VIRT, /* form puma6_hardware.h */
    .initialized = 0,
};

/***********************************************************************
 *       Puma6 SPI Controller section
 ***********************************************************************/

/* Platform specifc information, can be changed from platform to platform */
struct avalanche_spi_platform_data puma6_spi_platform_data = {
    .bus_num                = AVALANCHE_SPI_BUS_NUM,
    .max_chipselect         = AVALANCHE_SPI_MAX_CHIPSELECT,
    .addr_mode              = AVALANCHE_SPI_3_BYTE_ADDR_MODE,
	};



static struct resource puma6_spi_resources[] = {
        [0] = {
                .start          = AVALANCHE_FLASH_SPI_BASE,
                .end            = AVALANCHE_FLASH_SPI_BASE + AVALANCHE_SPI_SIZE,
                .flags          = IORESOURCE_MEM,
        },
};


/* Flash SPI Device */
static struct platform_device puma6_flash_spi_device = {
        .name           = "puma6_spi_device",    /* this name used to for driver matching */
        .id             = 0,                     /* the device instance number, or else "-1" to indicate there's only one. */
        .dev = {
               .platform_data = &puma6_spi_platform_data,
                /* Note: platform_device.dev.bus_id = 'puma6_spi.0'   (name.id) */
        }, 
        .num_resources  = ARRAY_SIZE(puma6_spi_resources),
        .resource       = puma6_spi_resources,
};



#ifdef CONFIG_SPI_TI_CODEC
/******************************************************************************
 *            CODEC SPI Controller and slave devices section
 ******************************************************************************/
int codec_spi_activate_cs( u8 cs, u8 pol, struct ti_ctlr_cs_sel_t *ctlr_cs_sel)
{
    int ret_val = 0;

    /* this is a system level register used to drive chip select to TIDs */
    if (cs)
    {
        PAL_sysBootCfgCtrl_ReadModifyWriteReg(BOOTCFG_GPCR ,0x40,0x40);
    }
    else
    {
        PAL_sysBootCfgCtrl_ReadModifyWriteReg(BOOTCFG_GPCR ,0x40,0x00);
    }

    ctlr_cs_sel->cs = cs;
    ctlr_cs_sel->pol = pol;
    return ret_val;
}

struct ti_codec_spi_platform_data puma6_codec_spi_platform_data = {
        .initial_spmode         = AVALANCHE_SPI_INIT_SPMODE,
        .bus_num                = AVALANCHE_CODEC_SPI_BUS_NUM,
        .max_chipselect         = AVALANCHE_CODEC_SPI_MAX_CHIPSELECT,
        .activate_cs            = codec_spi_activate_cs,
        .deactivate_cs          = NULL,
        /* four our controller cs is taken care by framelength */
        .sysclk                 = AVALANCHE_DEFAULT_SYS_CLK,
};

static struct resource puma6_codec_spi_resources[] = {
        [0] = {
                .start          = AVALANCHE_CODEC_SPI_BASE,
                .end            = AVALANCHE_SPI_1_BASE + AVALANCHE_SPI_SIZE,
                .flags          = IORESOURCE_MEM,
        },
        [1] = {
                .start          = AVALANCHE_CODEC_SPI_INT,
                .end            = AVALANCHE_CODEC_SPI_INT,
                .flags          = IORESOURCE_IRQ,
        },
};

static struct platform_device puma6_codec_spi_device = {
        .name           = "ti_codec_spi",
        .id             = 0,
        .dev = {
               .platform_data = &puma6_codec_spi_platform_data,
        },
        .num_resources  = ARRAY_SIZE(puma6_codec_spi_resources),
        .resource       = puma6_codec_spi_resources,
};
#endif /* CONFIG_SPI_TI_CODEC */


/*********************************************************************************
 *           Generic Section 
 *********************************************************************************/

/* board-specific information about each SPI device */
static struct spi_board_info puma6_spi_board_info[] = {
	{
 	/* This name is very important becoz the protocol or slave driver will probe
     * only using this name for hooking the driver to the corresponding device
	 */
        .modalias        = "spansion",                 /* Name of chip driver that handle this chip device*/
        .platform_data   = (void*)NULL,                /* platform_data goes to spi_device.dev.platform_data*/
		.controller_data = &flash0_info,               /* controller_data goes to spi_device.controller_data */ 
        .mode            = SPI_MODE_0,                 /* mode becomes spi_device.mode, */
        .irq             = 0,                          /* irq is copied too spi_device.*/
        .max_speed_hz    = CONFIG_AVALANCHE_SFL_CLK,   /* From ".config" TBD  now equal to 40000000*/
        .bus_num         = 0,
		.chip_select 	 = AVALANCHE_SPI_CS0,
	},
	{
 	/* This name is very important becoz the protocol or slave driver will probe
     * only using this name for hooking the driver to the corresponding device
	 */
        .modalias        = "spansion1",               /* Name of chip driver that handle this chip device*/
        .platform_data   = (void*)NULL,               /* platform_data goes to spi_device.dev.platform_data*/
		.controller_data = &flash1_info,              /* controller_data goes to spi_device.controller_data */
        .mode            = SPI_MODE_0,                /* mode becomes spi_device.mode, */
        .irq             = 0,                         /* irq is copied too spi_device.*/
        .max_speed_hz    = CONFIG_AVALANCHE_SFL_CLK,  /* From ".config" TBD */
        .bus_num         = 0,
		.chip_select 	 = AVALANCHE_SPI_CS1,
	},

#ifdef CONFIG_SPI_TI_CODEC
    {
    /* This name is very important becoz the protocol or slave driver will probe
     * only using this name for hooking the driver to the corresponding device
     */
        .modalias        = "tid_0",
        .platform_data   = (void*)NULL,
        .controller_data = (void*)NULL,
        .mode            = SPI_MODE_0,
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
        .mode            = SPI_MODE_0,
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
    &puma6_flash_spi_device,
#ifdef CONFIG_SPI_TI_CODEC
    &puma6_codec_spi_device,
#endif /* CONFIG_SPI_TI_CODEC */
};



static int puma6_spi_init(void)
{

    /* Register the slave devices present in the board with SPI subsytem */
    spi_register_board_info( puma6_spi_board_info, ARRAY_SIZE(puma6_spi_board_info));

	/* Register the master controllers with platform */
    platform_add_devices( spi_controller_devices, ARRAY_SIZE(spi_controller_devices));


	return 0;
}

subsys_initcall(puma6_spi_init);
