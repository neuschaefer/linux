/*
 *
 * avalanche_spi_master_puma.c
 * Description:
 * spi controller master driver
 *
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2008-2014 Intel Corporation. All rights reserved.
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

/*
 * Avalanche puma SPI controller driver.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/completion.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <linux/device.h>
#include <linux/spinlock.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm-arm/arch-avalanche/generic/pal.h>
#include<linux/device.h>
#include<linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/spi/avalanche_spi.h>
#include <linux/spi/avalanche_spi_bitbang.h>
#if defined (CONFIG_HW_MUTEXES)
#include <asm-arm/arch-avalanche/generic/hw_mutex_ctrl.h>
#endif

#if defined(CONFIG_SPI_DEBUG)
#define DEBUG(fmt,arg...)  printk(KERN_EMERG fmt , ##arg);
#else
#define DEBUG(fmt,arg...)
#endif

#define ENTER DEBUG(">>>>> [ENTR %s-%d] \n", __FUNCTION__, __LINE__);
#define EXIT  DEBUG("<<<<< [EXIT %s-%d] \n", __FUNCTION__, __LINE__);



/************************************************
* CONTROLLER REG FLAGS *
************************************************/
/* MCR (Mode Control Register) settings masks */
#define MCR_CS1_ENABLE  0x00100000
#define MCR_CS0_ENABLE  0x00010000
#define MCR_CS1_DISABLE 0xFFCFFFFF
#define MCR_CS0_DISABLE 0xFFFCFFFF
#define MCR_ADDRESSING_MODE_MASK    0x0000F000
#define MCR_ADDRESSING_MODE_4_BYTE  0x00004000
#define MCR_ADDRESSING_MODE_3_BYTE  0x00003000

/* Command / Data Register settings masks */
#define CDR_NBYTES_NULL    0x00000000
#define CDR_NBYTES_1       0x01000000
#define CDR_NBYTES_2       0x02000000
#define CDR_NBYTES_3       0x03000000
#define CDR_CS_HOLD_ENABLE 0x04000000



/* SPI Controller registers */
struct puma_spi_reg {
	volatile u32 mode_ctrl;                          /* MODULE ADDRESS + 0x00 */
	volatile u32 addr_split;                         /* MODULE ADDRESS + 0x04 */
	volatile u32 current_addr;                       /* MODULE ADDRESS + 0x08 */
	volatile u32 cmd_data;                           /* MODULE ADDRESS + 0x0C */
	volatile u32 interface_config;                   /* MODULE ADDRESS + 0x10 */
	volatile u32 high_efficiency_cmd_data;           /* MODULE ADDRESS + 0x20 */
	volatile u32 high_efficiency_transaction_params; /* MODULE ADDRESS + 0x24 */ 
	volatile u32 high_efficiency_opcode;             /* MODULE ADDRESS + 0x28 */
};


/* SPI Controller driver's private data. */
struct puma_spi_data{
	/* bitbang has to be first */
	struct spi_bitbang bitbang;
	struct completion done;
    struct puma_spi_reg __iomem *base;
	struct spi_device *spi;

};

/*** 
 * Helper function for word endian swapping
 */
inline u32 endian_swap(u32 x) 
{ 
    int swp = x;
    return ( ((swp&0x000000FF)<<24) + ((swp&0x0000FF00)<<8 ) +
             ((swp&0x00FF0000)>>8 ) + ((swp&0xFF000000)>>24) );
} 

/***
 * Bitbang layer uses this interface to set the framelength
 * for the slave device for current transfer in progress 
 *  
 */
void puma_spi_set_flen(struct spi_device *spi, u32 flen)
{
    ENTER

    DEBUG("puma_spi_set_flen: spi device:%d, flen:%d\n",spi->chip_select,flen);

	EXIT
}

/***
 * Interface to control the chip select signal 
 *  
 * Usage: This API will be invoked by the SPI bitbang worker
 * queue to initiate and terminate the SPI transfer.
 *   
 */
static void puma_spi_chipselect(struct spi_device *spi, int value)
{
    struct puma_spi_data *puma_spi;
    u32 mode_ctrl_reg;
    u32 cmd_data_reg;

    ENTER

	DEBUG("puma_spi_chipselect: spi device:%d, value:%d\n",spi->chip_select,value);

    /* Get pointer to spi controller */
	puma_spi = spi_master_get_devdata(spi->master);

    /* Enable Chip Select */
    if (value == 1)
    {
        /* Read Mode Control Register */
        mode_ctrl_reg = endian_swap(puma_spi->base->mode_ctrl);       
        DEBUG(" %s - Read  Mode Ctrl Reg 0x%08X\n",__FUNCTION__,mode_ctrl_reg);
    
        /* Disable CS1: Clear bits 21:20 */
        mode_ctrl_reg = mode_ctrl_reg & MCR_CS1_DISABLE;      
    
        /* Disable CS0: Clear bits 17:16 */
        mode_ctrl_reg = mode_ctrl_reg & MCR_CS0_DISABLE;      
    
    
        /* Set Chip Select */
        if (spi->chip_select == AVALANCHE_SPI_CS1)
            mode_ctrl_reg = mode_ctrl_reg | MCR_CS1_ENABLE;   /* Enable CS1: set bit 20              */
        else
            mode_ctrl_reg = mode_ctrl_reg | MCR_CS0_ENABLE;   /* Enable CS0: set bit 16              */
    
    
        DEBUG(" %s - Write Mode Ctrl Reg 0x%08X\n",__FUNCTION__,mode_ctrl_reg);
        puma_spi->base->mode_ctrl = endian_swap(mode_ctrl_reg);       /* Write back Mode Control Register    */
    }

    /* Clear Chip Select HOLD*/
    cmd_data_reg = 0;
    DEBUG(" %s - Clear CS HOLD, Write CmdData Reg 0x%08X\n",__FUNCTION__,cmd_data_reg);
    puma_spi->base->cmd_data = cmd_data_reg;


    EXIT

    return;
}

/**
 * puma_spi_setup_transfer - This functions will determine 
 * transfer method 
 * @spi: spi device on which data transfer to be done
 * @t: spi transfer in which transfer info is filled
 *
 * This function determines data transfer method (8/16/32 bit transfer).
 * It will also set the SPI Clock Control register according to
 * SPI slave device freq. 
 *  
 * Usage: This API will be called by SPI bitbang worker queue to 
 * identify the type of the SPI transfer like frequency, chip 
 * select number of slave device, type of the command (Tx or Rx) 
 * WordLength for SPI transfer, interrupt mode or polling mode 
 * etc. By this info the command will be prepared to write into 
 * the SPI Command Register. 
 */
static int puma_spi_setup_transfer( struct spi_device *spi,struct spi_transfer *t )
{
	ENTER
   
    DEBUG("puma_spi_setup_transfer: spi device:%d\n",spi->chip_select);

	EXIT
	return 0;
}



/**
 * puma_spi_setup - This functions will set default transfer 
 * method 
 * @spi: spi device on which data transfer to be done
 *
 * This functions sets the default transfer method. 
 *  
 * Usage: When client driver invoke spi_setup from SPI core 
 * module, it will call puma_spi_setup API from Master 
 * Controller which will calculate and setup values to be 
 * written form SPI controller register. 
 */

static int puma_spi_setup(struct spi_device *spi)
{
	int retval =0;

	ENTER

    /*if bits per word length is zero then set it default 8*/
	if (!spi->bits_per_word)
		spi->bits_per_word = 8;

    DEBUG("puma_spi_setup: spi device:%d\n",spi->chip_select);

    EXIT

	return retval;
}



/**
 * puma_spi_bufs - functions which will handle transfer data
 * @spi: spi device on which data transfer to be done
 * @t: spi transfer in which transfer info is filled
 *
 * This function will put data to be transferred into data register
 * of SPI controller and then wait untill the completion will be marked
 * by the IRQ Handler. 
 *  
 * Usage: This API will be invoked by the SPI bitbang worker queue to 
 * initiate the transfer. This API will write the SPI command into the
 * SPI Command Register and data if transmit is to perform. In
 * polling mode it will poll the SPI status register and loop
 * till the number bytes in spi_transfer are completed. In 
 * interrupt mode when the word transfer completes the interrupt
 * will come and execution goes into ISR Handler puma_spi_irq. 
 * This ISR handler will clear the interrupt and read data from 
 * SPI Data Register if needed. It will write command again to 
 * SPI Command register if number of bytes in spi_transfer is 
 * not zero. 
 */
static int puma_spi_bufs(struct spi_device *spi,struct spi_transfer *t)
{
	struct puma_spi_data *puma_spi;
    struct avalanche_sfi_dev_info_t *sfi;
    //u32 cs;
    u32 cmd_data_reg = 0;
    u32 tx_bytes = 0;
    u32 rx_bytes = 0;
    char*  tx_buf = (char*)t->tx_buf;
    char*  rx_buf = (char*)t->rx_buf;
    int actual_length = 0;

    ENTER

    // Get pointer to spi controller
	puma_spi = spi_master_get_devdata(spi->master);

    // Get Pointer to SFI data structure 
	sfi = (struct avalanche_sfi_dev_info_t*)(spi->controller_data);
    
	DEBUG("In function %s sfi = 0x%x  mode=%d\n",__FUNCTION__,(unsigned)sfi,(sfi)?sfi->mode:-1);

    /* Check if it is SFI Read mode */
	if(sfi != NULL && sfi->mode == AVALANCHE_SFI_MODE)
	{
        DEBUG("IN CORE SFI MODE\n");

        if(t->rx_buf == NULL)
		{
			DEBUG("in function %s t->rx_buff  is NULL 0x%x\n", __FUNCTION__,(unsigned)t->rx_buf);
		}

		actual_length = sfi->sfi_transfer(spi,t);

		DEBUG("Actual Length in %s : %d\n", __FUNCTION__, actual_length);
		return actual_length;
	}

    /* Continue in SPI Mode */
    DEBUG("IN CORE SPI MODE\n");

    // Get Chip Select  (AVALANCHE_SPI_CS0 or AVALANCHE_SPI_CS1)
    // cs = spi->chip_select;
    
    INIT_COMPLETION(puma_spi->done);

    /* Determine the command to execute READ or WRITE */
    if( tx_buf )
    {
        tx_bytes = 0;
        while (tx_bytes < t->len)
        {
            cmd_data_reg = CDR_CS_HOLD_ENABLE | CDR_NBYTES_1 | (tx_buf[tx_bytes] << 16);
            DEBUG(" %s - TX Write CmdData Reg 0x%08X\n",__FUNCTION__,cmd_data_reg);
            puma_spi->base->cmd_data = endian_swap(cmd_data_reg);
            tx_bytes++;
        }
    }
    else
    {
        rx_bytes = 0;
        while (rx_bytes < t->len)
        {
            cmd_data_reg = CDR_CS_HOLD_ENABLE | CDR_NBYTES_1;  // Set to read 1 byte
            DEBUG(" %s - RX Write CmdData Reg 0x%08X\n",__FUNCTION__,cmd_data_reg);
            puma_spi->base->cmd_data = endian_swap(cmd_data_reg);

            cmd_data_reg = endian_swap(puma_spi->base->cmd_data);      // Read 1 byte
            DEBUG(" %s - RX Read  CmdData Reg 0x%08X\n",__FUNCTION__,cmd_data_reg);
            rx_buf[rx_bytes] = (u8)(cmd_data_reg & 0x000000FF);
            DEBUG(" %s - rx_buf[%d]-0x%02X\n",__FUNCTION__,rx_bytes,rx_buf[rx_bytes]);
            rx_bytes++;
        }
    }

    EXIT
    #ifdef SPI_PROFILE
    spi_end_profile();
    printk("Finished Profiling\n");
    #endif

    return t->len;
}



/**
 * puma_spi_probe - probe function for SPI Master Controller
 * @dev: platform_device structure which contains plateform specific data
 *
 * According to Linux Deviced Model this function will be invoked by Linux
 * with plateform_device struct which contains the device specific info
 * like bus_num, max_chipselect (how many slave devices can be connected),
 * clock freq. of SPI controller, SPI controller's memory range, IRQ number etc.
 * This info will be provided by board specific code which will reside in
 * linux-2.6.10/arch/mips/mips-boards/avalanche_avalanche/avalanche_yamuna code.
 * This function will map the SPI controller's memory, register IRQ,
 * Reset SPI controller and setting its registers to default value.
 * It will invoke spi_bitbang_start to create work queue so that client driver
 * can register transfer method to work queue.
 */

static int __init puma_spi_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;            // Pointer to Device
	struct spi_master *master;                  // Pointer to SPI Master device
	struct puma_spi_data *puma_spi;           // SPI Controller driver's private data. 
	struct avalanche_spi_platform_data *pdata;  // SPI Controller driver's platfrom data (can change form one platrom to another).
    
    struct resource *r;                         // Pointer to Resources
	int ret = 0;

	ENTER

    /* Allocate SPI Master device
        - Set pointer to the controller 'Device' structure
        - Allocat space to the controller private data structure */
	master = spi_alloc_master(dev, sizeof(struct puma_spi_data));

	if( master == NULL ){
		ret = -ENOMEM;
		goto err;
	}

    /* set pointer to SPI Master device structure in the controller 'Device' structure (driver_data)*/
	dev_set_drvdata(dev, (master));

	pdata = dev->platform_data;

	if( pdata == NULL ){
		ret = -ENODEV;
		goto free_master;
	}

	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if( r == NULL ){
		ret = -ENODEV;
		goto free_master;
	}

    /* Get pointer to controller private date structure */
	puma_spi = spi_master_get_devdata(master);


	puma_spi->bitbang.master         = spi_master_get(master);
	puma_spi->bitbang.chipselect     = puma_spi_chipselect;
	puma_spi->bitbang.set_flen       = puma_spi_set_flen;
	puma_spi->bitbang.setup_transfer = puma_spi_setup_transfer;
	puma_spi->bitbang.txrx_bufs      = puma_spi_bufs;
    puma_spi->bitbang.master->setup  = puma_spi_setup;
	

	init_completion(&puma_spi->done);  // like sema_init

	puma_spi->base = (struct puma_spi_reg __iomem *)(r->start);
	if (puma_spi->base == NULL) {
		ret = -ENOMEM;
		goto put_master;
	}
    master->bus_num = pdata->bus_num;
	master->num_chipselect = pdata->max_chipselect;

	/* SPI controller initializations */
#if defined (CONFIG_HW_MUTEXES)
    /* Lock the HW Mutex */
    if (hw_mutex_lock_interruptible(HW_MUTEX_NOR_SPI) != 0)
    {
        printk(KERN_EMERG,"puma_spi_probe failed - Can't lock HW mutex \n");
        ret = -EIO;
        goto unmap_io;
    }
#endif
    printk(KERN_INFO "puma SPI Controller: Control Reg        [0x%08X]: 0x%08X\n",(unsigned int)&puma_spi->base->mode_ctrl,endian_swap(puma_spi->base->mode_ctrl));
    printk(KERN_INFO "puma SPI Controller: Split Reg          [0x%08X]: 0x%08X\n",(unsigned int)&puma_spi->base->addr_split,endian_swap(puma_spi->base->addr_split));
    printk(KERN_INFO "puma SPI Controller: Current Address Reg[0x%08X]: 0x%08X\n",(unsigned int)&puma_spi->base->current_addr,endian_swap(puma_spi->base->current_addr));

    if ((endian_swap(puma_spi->base->mode_ctrl) & MCR_ADDRESSING_MODE_MASK) == MCR_ADDRESSING_MODE_4_BYTE)
        pdata->addr_mode = AVALANCHE_SPI_4_BYTE_ADDR_MODE;
    else
        pdata->addr_mode = AVALANCHE_SPI_3_BYTE_ADDR_MODE;

    printk(KERN_INFO "puma SPI Controller: Addressing mode set to: %d\n",pdata->addr_mode);

#if defined (CONFIG_HW_MUTEXES)
    /* Un-lock the HW Mutex */
    hw_mutex_unlock(HW_MUTEX_NOR_SPI);
#endif
    /* avalanche_spi_bitbang_start, will call to spi_register_master() */
	ret = avalanche_spi_bitbang_start(&puma_spi->bitbang);

	if (ret != 0)
		goto unmap_io;

	printk(KERN_INFO "puma SPI Controller driver at 0x%p bus_num = 0x%x \n",puma_spi->base, pdata->bus_num);

	return ret;

unmap_io:
	iounmap(puma_spi->base);
put_master:
	spi_master_put(master);
free_master:
	kfree(master);
err:
	return ret;
}


/**
 * puma_spi_remove - remove function for SPI Master Controller
 * @dev: platform_device structure which contains plateform specific data
 *
 * This function will do the reverse action of puma_spi_probe function
 * It will free the IRQ and SPI controller's memory region.
 * It will also call spi_bitbang_stop to destroy the work queue which was
 * created by spi_bitbang_start.
 */
static int puma_spi_remove(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct puma_spi_data *puma_spi;
	struct spi_master *master;
	ENTER
	master = dev_get_drvdata(dev);

	puma_spi = spi_master_get_devdata(master);
	avalanche_spi_bitbang_stop(&puma_spi->bitbang);

	iounmap(puma_spi->base);
	spi_master_put(puma_spi->bitbang.master);
	EXIT
	return 0;
}

static struct platform_driver spi_puma_driver = {
	.driver = {
		.name  = "puma_spi_device",
		.bus   = &platform_bus_type,
		.owner = THIS_MODULE,
	},
	.probe     = puma_spi_probe,
	.remove    = puma_spi_remove,
	.shutdown  = NULL,
	.suspend   = NULL,
	.resume    = NULL,
};


static int __init spi_puma_init(void)
{
	ENTER
	return platform_driver_register(&spi_puma_driver);
	EXIT
}

static void __exit spi_puma_exit(void)
{
	platform_driver_unregister(&spi_puma_driver);
}

module_init(spi_puma_init);
module_exit(spi_puma_exit);

MODULE_AUTHOR("Intel.");
MODULE_DESCRIPTION("puma SPI Master Controller Driver");
MODULE_LICENSE("GPL");
