/*
 *
 * avalanche_spi_master.c
 * Description:
 * spi controller master driver
 *
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
 * TI Avalanche SPI controller driver.
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

//#define CONFIG_SPI_DEBUG 1

#if defined(CONFIG_SPI_DEBUG)
#define DEBUG(fmt,arg...)  printk(KERN_EMERG fmt , ##arg);
#else
#define DEBUG(fmt,arg...)
#endif

#define ENTER DEBUG(">>>>> [ENTR %s-%d] \n", __FUNCTION__, __LINE__);
#define EXIT  DEBUG("<<<<< [EXIT %s-%d] \n", __FUNCTION__, __LINE__);


#define FLEN_MASK       0x00000FFF
#define SPI_BITS_PER_WORD(cmd) ((cmd &0x00F80000) >> 19)
#define ERROR			-1

/************************************************
* CONTROLLER REG FLAGS *
************************************************/
/* SPI clk Control Reg FLags */
#define SPI_FREQ 				(2000000)
/* Default 2 MHz according to "UR8 EVM Spec 0.1.pdf". */
#define SPI_CLK_EN				(1 << 31)

/* SPI Device control Reg Flags */
#define SET_DD0 				 0x00
#define SET_DD1 				 0x00
#define SET_DD2 				 0x00
#define SET_DD3 				 0x00
#define SET_CKPOL_SLAVE_0 	    (0x01)
#define SET_CKPHA_SLAVE_0 		(0x01 << 2)
#define SET_CKPOL_SLAVE_1 		(0x01 << 8)
#define SET_CKPHA_SLAVE_1 		(0x01 << 10)
#define SET_CKPOL_SLAVE_2 		(0x01 << 16)
#define SET_CKPHA_SLAVE_2 		(0x01 << 18)
#define SET_CKPOL_SLAVE_3 		(0x01 << 24)
#define SET_CKPHA_SLAVE_3 		(0x01 << 26)

/* SPI Command Reg Flags */
/*configure bits from 19 to 23 of SPI command register*/
#define SPMODE_LEN(word_len)	((word_len) << 19)
#define CMD_TO_BPS(cmd) 		((cmd >> 19) & 0x1F)

/*16th and 17th bits of SPI command register*/
#define SPI_CMD_READ 			(0x01 << 16)
#define SPI_CMD_WRITE 			(0x02 << 16)
#define SPI_CMD_DUAL_READ 		(0x03 << 16)

#define SPI_INT_ENABLE 			(0x03 << 14)
#define WC_INT					0x2
/*Enable Word Complete  Interrupt*/

/* MM SPI set up Reg Flags */
#define MMSPI_READ_CMD_POS			0
#define NUM_MMSPI_REGS				4
#define MMSPI_NUM_ADDR_BYTES_POS	8
#define MMSPI_NUM_DUMMY_BYTES_POS	10
#define MMSPI_DUAL_READ_POS			12
#define MMSPI_WRITE_CMD_POS			16

/*MM SPI Switch Reg Flags */
#define SET_CORE_SPI_MODE			(0x00 << 0)
#define SET_CORE_SFI_MODE			(0x01 << 0)

/* operating mode selection from kconfig */
#ifdef CONFIG_SPI_INTERRUPT_MODE
#define SPI_INTERRUPT_MODE 			1
#endif
/* 2 seconds worst case delay */
#define AVALANCHE_SPI_DELAY			(msecs_to_jiffies(2000))
#ifdef CONFIG_SPI_TASKLET_MODE
#define SPI_INTERRUPT_MODE 			1
#define SPI_INT_TASKLET 			1
#endif

#ifdef CONFIG_SPI_POLLING_MODE
#undef SPI_INTERRUPT_MODE
#undef SPI_INT_TASKLET
#endif

/* SPI Controller registers */
struct avalanche_spi_reg {
	volatile u32 rev_id;    /* MODULE ADDRESS + 0x00 */
	volatile u32 clk_ctrl;	/* MODULE ADDRESS + 0x04 */
	volatile u32 dev_cfg;	/* MODULE ADDRESS + 0x08 */
	volatile u32 cmd;		/* MODULE ADDRESS + 0x0C */
	volatile u32 status;	/* MODULE ADDRESS + 0x10 */
	volatile u32 data;		/* MODULE ADDRESS + 0x14 */
	volatile u32 mm_spi_setup[NUM_MMSPI_REGS];
							/* MODULE ADDRESS + 0x18,0x1c,0x20,x024 */
	volatile u32 mm_spi_switch; /* MODULE ADDRESS + 0x28 */
};

struct avalanche_spi_slave {
	u32 cmd_to_write;
	u32 clk_ctrl_to_write;
    u32 bytes_per_word;
    u32 frame_len;
    u8 active_cs;
};

/* SPI Controller driver's private data. */
struct avalanche_spi {
	/* bitbang has to be first */
	struct spi_bitbang bitbang;
	struct completion done;
	struct avalanche_spi_reg __iomem *base;
	/* rx & tx bufs from the spi_transfer */
	const void *tx;
	void *rx;
	/* functions to deal with different sized buffers */
	void (*get_rx) (u32 rx_data, struct avalanche_spi *);
	u32(*get_tx) (struct avalanche_spi *);
	int count;
	u32 irq;
	u32 nsecs;		/* (clock cycle time)/2 */
	u32 sysclk;
	u32 ref_freq;
	/* chip select activation deactivation */
	int	(*activate_cs)(u8 cs, u8 polarity,struct ctlr_cs_sel_t *ctlr_cs_sel);
	int	(*deactivate_cs)(u8 cs, u8 polarity,struct ctlr_cs_sel_t *ctlr_cs_sel);
	struct avalanche_spi_slave slave[ AVALANCHE_SPI_MAX_CHIPSELECT ];
	struct spi_device *spi;
#ifdef SPI_INT_TASKLET
	struct tasklet_struct tasklet;
	u8 have_interrupt;
#endif
};

#define avalanche_SPI_RX_BUF(type) 	  \
static inline void avalanche_spi_rx_buf_##type(u32 data, \
                                        struct avalanche_spi *avalanche_spi) \
{									  \
	type * rx = avalanche_spi->rx;	  \
	*rx++ = (type)data;				  \
	avalanche_spi->rx = rx;			  \
}

#define avalanche_SPI_TX_BUF(type)	 \
static inline u32 avalanche_spi_tx_buf_##type(\
                                        struct avalanche_spi *avalanche_spi)	\
{								           \
	u32 data;				        	   \
	const type * tx = avalanche_spi->tx;   \
	data = *tx++;						   \
	avalanche_spi->tx = tx;				   \
	return data;						   \
}

avalanche_SPI_RX_BUF(u8)
avalanche_SPI_RX_BUF(u16)
avalanche_SPI_RX_BUF(u32)
avalanche_SPI_TX_BUF(u8)
avalanche_SPI_TX_BUF(u16)
avalanche_SPI_TX_BUF(u32)

#ifdef CONFIG_SPI_DEBUG
static int spi_master_reg_dump(unsigned long spi_master)
{
    printk("\n SPI Master Reg Contents\n");
    printk("The rev_id   = 0x%x\n",*(volatile unsigned int*)
                                    (spi_master + 0x00));
    printk("The clk_ctrl = 0x%x\n",*(volatile unsigned int*)
                                    (spi_master + 0x04));
    printk("The dev_cfg  = 0x%x\n",*(volatile unsigned int*)
                                    (spi_master + 0x08));
    printk("The cmd      = 0x%x\n",*(volatile unsigned int*)
                                    (spi_master + 0x0C));
    printk("The status   = 0x%x\n",*(volatile unsigned int*)
                                    (spi_master + 0x10));
    printk("The data     = 0x%x\n",*(volatile unsigned int*)
                                    (spi_master + 0x14));
    printk("The mm_spi_setup_reg0 = 0x%x\n",*(volatile unsigned int*)
                                             (spi_master + 0x18));
    printk("The mm_spi_setup_reg1 = 0x%x\n",*(volatile unsigned int*)
                                             (spi_master + 0x1C));
    printk("The mm_spi_setup_reg2 = 0x%x\n",*(volatile unsigned int*)
                                             (spi_master + 0x20));
    printk("The mm_spi_setup_reg3 = 0x%x\n",*(volatile unsigned int*)
                                             (spi_master + 0x24));
    printk("The mm_spi_switch = 0x%x\n",*(volatile unsigned int*)
                                         (spi_master + 0x28));
    return 0;
};
#endif

/*
 * Bitbang layer uses this interface to set the framelength
 * for the slave device for current transfer in progress
 */
void avalanche_spi_set_flen(struct spi_device *spi, u32 flen)
{
	struct avalanche_spi *avalanche_spi;
	ENTER
	avalanche_spi = spi_master_get_devdata(spi->master);

    /* Wait for SPISR[0] to be cleared */
    while( avalanche_spi->base->status & 0x1 ) {
        udelay(10);
    }

    avalanche_spi->slave[spi->chip_select].frame_len = ((flen-1) & FLEN_MASK);
	DEBUG("flen = %d, frame_len = %d\n", flen,
		  avalanche_spi->slave[spi->chip_select].frame_len);
	EXIT
}

/*
 * Interface to control the chip select signal
 */
static void avalanche_spi_chipselect(struct spi_device *spi, int value)
{
	struct avalanche_spi *avalanche_spi;
    /* determine - active high or active low */
	u8 pol = spi->mode & SPI_CS_HIGH ? 1 : 0;
	u32 regval = 0;
	struct ctlr_cs_sel_t ctlr_cs_sel;
	u8 cs;

	ENTER
	avalanche_spi = spi_master_get_devdata(spi->master);

    /* Wait for SPISR[0] to be cleared */
    while( avalanche_spi->base->status & 0x1 ) {
        udelay(10);
    }
    
    ctlr_cs_sel.cs = spi->chip_select;
	avalanche_spi->slave[spi->chip_select].active_cs = ctlr_cs_sel.cs;
	ctlr_cs_sel.pol = pol;

	/* board specific chip select logic decides the polarity and cs line for
     * the controller
     */
	if ( value == BITBANG_CS_ACTIVE) {
		if (avalanche_spi->activate_cs)
            /* Activate the chip select signal in the board */
			avalanche_spi->activate_cs(spi->chip_select, pol , &ctlr_cs_sel);
	}
	else {

        EXIT

		return;
	}

	pol = ctlr_cs_sel.pol;
	avalanche_spi->slave[spi->chip_select].active_cs = cs = ctlr_cs_sel.cs;

	/*Read the value from device configuration (SPIDC) register*/
	regval = avalanche_spi->base->dev_cfg;

	/* Mask out bits in regval which we are going to set.
	 * (bits 0 to 2, 8 to 10, 16 to 18 and 24 to 26).
	 * These bits will determine the chip select polarity, clock polarity and
	 * clock phase for 1st, 2nd, 3rd and 4th slave devices.
     */
	switch(cs)
	{

	    case 0: /* for Slave device 0 */
			if (spi->mode & SPI_CPHA)
				regval |= SET_CKPHA_SLAVE_0;
			if (spi->mode & SPI_CPOL)
				regval |= SET_CKPOL_SLAVE_0;

			if(pol)
				regval |= (1<<1);

			regval |= SET_DD0;
            /* only required if operating at higher freq than SPI controller */
			break;

	    case 1: /* for Slave device 1 */
			if (spi->mode & SPI_CPHA)
				regval |= SET_CKPHA_SLAVE_1;
			if (spi->mode & SPI_CPOL)
				regval |= SET_CKPOL_SLAVE_1;

			 if(pol)
						regval |= (1<<9);


			regval |= SET_DD1;

			break;

	    case 2: /* for Slave device 2 */
			if (spi->mode & SPI_CPHA)
				regval |= SET_CKPHA_SLAVE_2;
			if (spi->mode & SPI_CPOL)
				regval |= SET_CKPOL_SLAVE_2;

			 if(pol)
				regval |= (1<<17);


			regval |= SET_DD2;

			break;

		case 3: /* for Slave device 3 */
			if (spi->mode & SPI_CPHA)
				regval |= SET_CKPHA_SLAVE_3;

			if (spi->mode & SPI_CPOL)
				regval |= SET_CKPOL_SLAVE_3;

			 if(pol)
				regval |= (1<<25);

			regval |= SET_DD3;

			break;

		default:
			break;
	}
	/* Write the configuration parameter to the device config register (SPIDC)
     */
	avalanche_spi->base->dev_cfg = regval;
	DEBUG("[DC=%x , CS=%x, pol=%x\n] ",regval, cs, pol);

	EXIT

}

/**
 * avalanche_spi_setup_transfer - This functions will determine transfer method
 * @spi: spi device on which data transfer to be done
 * @t: spi transfer in which transfer info is filled
 *
 * This function determines data transfer method (8/16/32 bit transfer).
 * It will also set the SPI Clock Control register according to
 * SPI slave device freq.
 */
static int avalanche_spi_setup_transfer( struct spi_device *spi,
                                         struct spi_transfer *t )
{
	struct avalanche_spi *avalanche_spi;
	struct ctlr_cs_sel_t ctlr_cs_sel;
	u32 regval = 0;
	u8 bits_per_word = 0;
	u32 hz = 0;
	u32 clk_div;

	ENTER
	avalanche_spi = spi_master_get_devdata(spi->master);

	if (t) {
		bits_per_word = t->bits_per_word;
		hz = t->speed_hz;
	}

	/* if bits_per_word is not set then set it default */
	if (!bits_per_word)
		bits_per_word = spi->bits_per_word;

	/* Assign function pointer to appropriate transfer method 8bit/16bit or
     * 32bit transfer
     */
	if (bits_per_word <= 8)
	{
		avalanche_spi->get_rx = avalanche_spi_rx_buf_u8;
		avalanche_spi->get_tx = avalanche_spi_tx_buf_u8;
		avalanche_spi->slave[spi->chip_select].bytes_per_word = 1;
	} else if (bits_per_word <= 16) {
		avalanche_spi->get_rx = avalanche_spi_rx_buf_u16;
		avalanche_spi->get_tx = avalanche_spi_tx_buf_u16;
		avalanche_spi->slave[spi->chip_select].bytes_per_word = 2;
	} else if (bits_per_word <= 32) {
		avalanche_spi->get_rx = avalanche_spi_rx_buf_u32;
		avalanche_spi->get_tx = avalanche_spi_tx_buf_u32;
		avalanche_spi->slave[spi->chip_select].bytes_per_word = 4;
	}else {
		return ERROR;
	}

	/* Current transfer not interested in overriding the existing frequency */
	if(t && !hz) {
		goto no_freq;
	}

	if(!hz)  {
		hz = spi->max_speed_hz;
		if(!hz)  {
			hz = SPI_FREQ; /* defaulting to 2Mhz*/
			printk("[SPI] -> Slave device speed not set correctly.\
					 Trying with %dHz\n", hz);
		}
	}
	/* requested freq can't be more than ref_clk */
	if(hz > avalanche_spi->ref_freq)
		hz = avalanche_spi->ref_freq;

	clk_div = (avalanche_spi->ref_freq/hz) - 1;
	regval = SPI_CLK_EN | clk_div;
	/* write value to SPI clock controll register */
	avalanche_spi->slave[spi->chip_select].clk_ctrl_to_write = regval;
	DEBUG("[clk_ctrl=%x]\n", regval);

/**************************************************************************/
no_freq:
	regval = 0;
	/*SPMOD_LEN macro will shift the bits_per_word value to bits number
	19 to 23 of SPI command register.*/
	regval |= SPMODE_LEN(bits_per_word-1);
	/* Determine the slave device to operate on */
 	if (avalanche_spi->activate_cs)
		/* Activate the chip select signal in the board */
		avalanche_spi->activate_cs(spi->chip_select,
							((spi->mode & SPI_CS_HIGH) ? 1 : 0), &ctlr_cs_sel);

 	regval |= (ctlr_cs_sel.cs << 28);

#ifdef SPI_INTERRUPT_MODE
	/* We are using the interrupt mode
	 * enable interrupt for WC and FC
	 */
	regval |= SPI_INT_ENABLE;
#endif

	/* write command to the SPI command register (SPICR)
	 * We can't write partial command to SPI command register bcoz if we do
     * that then CS will be low and command will be executed. So we need to
     * store this command into the SPI controller structure and write into
	 * SPI command register in avalanche_spi_bufs function.
     */

	avalanche_spi->slave[spi->chip_select].cmd_to_write = regval;
	DEBUG("[C2W=%x]\n", avalanche_spi->slave[spi->chip_select].cmd_to_write);

	EXIT
	return 0;
}


/**
 * avalanche_spi_setup - This functions will set default transfer method
 * @spi: spi device on which data transfer to be done
 *
 * This functions sets the default transfer method.
 */

static int avalanche_spi_setup(struct spi_device *spi)
{
	int retval =0;
	struct avalanche_spi *avalanche_spi;

	ENTER

	avalanche_spi = spi_master_get_devdata(spi->master);

    /* Wait for SPISR[0] to be cleared */
    while( avalanche_spi->base->status & 0x1 ) {
        udelay(10);
    }

    /*if bits per word length is zero then set it default 8*/
	if (!spi->bits_per_word)
		spi->bits_per_word = 8;

	avalanche_spi->slave[spi->chip_select].cmd_to_write = 0;
	retval = avalanche_spi_setup_transfer(spi, NULL);

	EXIT

	return retval;
}


/**
 * avalanche_spi_bufs - functions which will handle transfer data
 * @spi: spi device on which data transfer to be done
 * @t: spi transfer in which transfer info is filled
 *
 * This function will put data to be transferred into data register
 * of SPI controller and then wait untill the completion will be marked
 * by the IRQ Handler.
 */
#define BIT_CLEAR_TIME_OUT	0x0FFFFF

/* #define SPI_PROFILE 1 */

#ifdef SPI_PROFILE

static unsigned int spi_prof_start;
static unsigned int spi_prof_end;
static unsigned int spi_avg_latency;

#define rdtscl(dest) \
    __asm__ __volatile__("mfc0 %0,$9; nop" : "=r" (dest))

static void calculate_spi_latency(void)
{
    unsigned int diff = spi_prof_end - spi_prof_start;

    if(diff < 0)
    {
        printk("Count register wraparound not supported.\n");
    } else {
        if(spi_avg_latency == 0)
        {
            spi_avg_latency = diff;
        } else {
            spi_avg_latency += diff;
            spi_avg_latency /= 2;
        }
    }
}

void spi_start_profile(void)
{
    rdtscl(spi_prof_start);
}

void spi_end_profile(void)
{
    rdtscl(spi_prof_end);
    calculate_spi_latency();
	printk("[SPI-PROFILE] Throughput = %d\n", spi_avg_latency);
}

unsigned int spi_get_avg_latency()
{
    return spi_avg_latency;
}
#endif /* SPI_PROFILE */

static int avalanche_spi_bufs(struct spi_device *spi,struct spi_transfer *t)
{
	struct avalanche_spi *avalanche_spi;
	u32 word, regval = 0;
	u32 conv, bits_per_word = 0;
#ifndef SPI_INTERRUPT_MODE
	unsigned long timeout;
	volatile u32 event = 0x1;
	u32 rx_data=0;
#endif
	int old_mode = 0;
	int actual_length = 0;

/*------------------------- SFI START  -------------------------------------*/
	struct avalanche_sfi_dev_info_t *sfi =
			(struct avalanche_sfi_dev_info_t*)(spi->controller_data);
	avalanche_spi = spi_master_get_devdata(spi->master);

	old_mode = avalanche_spi->base->mm_spi_switch;

	DEBUG("In function %s sfi = 0x%x  mode=%d\n",__FUNCTION__,(unsigned)sfi,
		   (sfi)?sfi->mode:-1);

	if(sfi != NULL && sfi->mode == AVALANCHE_SFI_MODE)
	{
		old_mode = avalanche_spi->base->mm_spi_switch;
		DEBUG("Initializing the SFI mode Registers \n");
		/* fill mm spi setup registers */
		if( sfi->initialized == 0 )
		{
            avalanche_spi->base->mm_spi_setup[spi->chip_select] =
#ifdef SPI_ENABLE_MMSPI_WRITE
                sfi->write_cmd << MMSPI_WRITE_CMD_POS;
#else
                0 << MMSPI_WRITE_CMD_POS;
#endif
			avalanche_spi->base->mm_spi_setup[spi->chip_select] |=
							sfi->dual_read<< MMSPI_DUAL_READ_POS;
			avalanche_spi->base->mm_spi_setup[spi->chip_select] |=
							sfi->num_dummy_bytes << MMSPI_NUM_DUMMY_BYTES_POS;
			avalanche_spi->base->mm_spi_setup[spi->chip_select] |=
						sfi->num_addr_bytes << MMSPI_NUM_ADDR_BYTES_POS;
			avalanche_spi->base->mm_spi_setup[spi->chip_select] |=
						sfi->read_cmd << MMSPI_READ_CMD_POS;

			DEBUG("MMSPI REG %d contains spi->chip_select0x%x\n",
					spi->chip_select,(unsigned int)
					avalanche_spi->base->mm_spi_setup[spi->chip_select]);
			sfi->initialized = 1;
		}
		avalanche_spi->base->mm_spi_switch = AVALANCHE_SFI_MODE;
		DEBUG("Before calling Client Transfer Function in \
			  avalanche_spi_bufs \n");
		if(t->rx_buf == NULL)
		{
			DEBUG("in function %s t->rx_buff  is NULL 0x%x\n", __FUNCTION__,
				   (unsigned)t->rx_buf);
		}
		DEBUG("Before calling Client Transfer in func %s t->rx_buff is 0x%x\n",
				__FUNCTION__, (unsigned)t->rx_buf);

		actual_length = sfi->sfi_transfer(spi,t);
		DEBUG("After calling Client Transfer Fun in avalanche_spi_bufs\n");
		avalanche_spi->base->mm_spi_switch = old_mode;
		/* Do we need to revert the sfi->mode here ? */
		DEBUG("Actual Length in %s : %d\n", __FUNCTION__, actual_length);
		return actual_length;
	}
	else
	{
		DEBUG(" IN CORE SPI MODE\n");
		avalanche_spi->base->mm_spi_switch = AVALANCHE_CORE_SPI_MODE;

	}
/*------------------------- SFI END  -------------------------------------*/
	#ifdef SPI_PROFILE
	printk("Starting to Profile\n");
	spi_start_profile();
	#endif

	ENTER

	avalanche_spi->tx = t->tx_buf;
	avalanche_spi->rx = t->rx_buf;

	/* convert len to words bbased on bits_per_word */
	bits_per_word = CMD_TO_BPS(
                    avalanche_spi->slave[spi->chip_select].cmd_to_write) + 1;

	if( t ) {
		bits_per_word = t->bits_per_word;
	}
	/* if bits_per_word is not set then set it default */
	if (!bits_per_word)
		bits_per_word = spi->bits_per_word;

	/* Assign function pointer to appropriate transfer method 8bit/16bit or
     * 32bit transfer
     */
	if( bits_per_word <= 8 ){
		avalanche_spi->slave[spi->chip_select].bytes_per_word = 1;
        conv = 0;
	}else if( bits_per_word <= 16 ){
		avalanche_spi->slave[spi->chip_select].bytes_per_word = 2;
        conv = 1;
	}else if( bits_per_word <= 32 ){
		avalanche_spi->slave[spi->chip_select].bytes_per_word = 4;
        conv = 2;
	} else {
		return ERROR;
    }
	
	/*
	 * update spi frequency - this is done for every transer becoz previous
	 * might have been for some other slave device and it would have operated
	 * in a different frequency
	 */
#if (0)
#ifndef CONFIG_MACH_PUMA5
#ifndef CONFIG_MACH_PUMA6
	avalanche_spi->base->clk_ctrl =
		avalanche_spi->slave[spi->chip_select].clk_ctrl_to_write;
#endif
#endif
#endif

	avalanche_spi->count = t->len >> conv;
	DEBUG("chipselect=%d, bytes_per_word=%d, t->len=%d, conv=%d\n",
	spi->chip_select, avalanche_spi->slave[spi->chip_select].bytes_per_word,
	t->len, conv);

	INIT_COMPLETION(avalanche_spi->done);

	/* Determine the command to execute READ or WRITE */
	if( t->tx_buf ){
		regval = SPI_CMD_WRITE;
		DEBUG("Settng WRITE cmd\n");
	}
	else{
		regval = SPI_CMD_READ;
		/* regval = SPI_CMD_DUAL_READ; need to fix this */
		DEBUG("Settng READ cmd\n");
	}

#ifdef SPI_INTERRUPT_MODE
	/* clean the bits */
	avalanche_spi->slave[spi->chip_select].cmd_to_write &= ~(0x30FFF);

	/* update command to write with frame length and the read/write command */
	avalanche_spi->slave[spi->chip_select].cmd_to_write |= regval |
						(avalanche_spi->slave[spi->chip_select].frame_len);

	DEBUG("avalanche_spi->count=%d \n",avalanche_spi->count);

	/* The ISR needs the information about current slave device whose
     * transmission is in progress
     */
	avalanche_spi->spi = spi;
	/* Are we going to write data to the slave device? */
	if (avalanche_spi->tx)
	{
		word = avalanche_spi->get_tx(avalanche_spi);
		#ifdef AVALANCHE_SPI_IP_FIX
		word <<= (32- bits_per_word);
		#endif
		avalanche_spi->base->data = word;
	}

	/* Start the transfer */
	avalanche_spi->base->cmd =
		avalanche_spi->slave[spi->chip_select].cmd_to_write;
	enable_irq(LNXINTNUM(avalanche_spi->irq));

	DEBUG("[C_WRITTEN=%x]\n",
		avalanche_spi->slave[spi->chip_select].cmd_to_write);
	DEBUG("B4 wait_for_completion\n");

	/* wait for completion signal from ISR */
	wait_for_completion(&avalanche_spi->done);

	/* If we are working in polling mode */
#else
	/* clean the bits */
	avalanche_spi->slave[spi->chip_select].cmd_to_write &= ~(0x30FFF);
	/* update command to write with frame length and the read/write command */
	avalanche_spi->slave[spi->chip_select].cmd_to_write |=
		regval | avalanche_spi->slave[spi->chip_select].frame_len;

	DEBUG("reg_val = %x, frm_len=%d, c2w=%x\n", regval, 
	avalanche_spi->slave[spi->chip_select].frame_len,
	avalanche_spi->slave[spi->chip_select].cmd_to_write);

	DEBUG("avalanche_spi->count=%d \n",avalanche_spi->count);

	/* start dispatching the words */
 	while(avalanche_spi->count > 0)
	{
        if (avalanche_spi->tx)
       	{
			/* write the word to be transmitted into the data register */
			word = avalanche_spi->get_tx(avalanche_spi);
			#ifdef AVALANCHE_SPI_IP_FIX
			word <<= (32- bits_per_word);
			#endif
			avalanche_spi->base->data = word;
			DEBUG("[D=%x]\n", word);
		}

		/* issue the command */
		DEBUG("[C=%x]\n",avalanche_spi->slave[spi->chip_select].cmd_to_write);
		avalanche_spi->base->cmd =
                          avalanche_spi->slave[spi->chip_select].cmd_to_write;


		/* Wait for busy bit/word complete to clear */
		timeout = jiffies + AVALANCHE_SPI_DELAY;
		do
		{
			event = avalanche_spi->base->status;
			/* Mask off all bits except word complete */
			if(((event & 0x3) == 0x2)){
        	    break;
            }          
            if(time_after(jiffies, timeout))
    	        printk(KERN_ERR "[SPI] -> Word complete not set for a long time even \
						after busy bit is cleared, status=%x\n", event);
		}while(time_after(timeout, jiffies));

		DEBUG("SPI status_reg_val=%x\n", event);
    	/* do we have something to read from the slave device */
		if (avalanche_spi->rx)
		{
           	rx_data = avalanche_spi->base->data;
			DEBUG("data read %x \n",rx_data);
			/* place the read data in the driver buffer */
			avalanche_spi->get_rx(rx_data, avalanche_spi);
		}
		avalanche_spi->count--;
	}
#endif

	/* SPI Framework maintains the count only in bytes so convert
     * back to bytes
     */

	avalanche_spi->count <<= conv;

	EXIT
	#ifdef SPI_PROFILE
	spi_end_profile();
	printk("Finished Profiling\n");
	printk("Bytes Transfered = %d, Bytes Pending = %d\n",
           (t->len-avalanche_spi->count), avalanche_spi->count);
	#endif
	avalanche_spi->base->mm_spi_switch = old_mode;
	return t->len;
}

#ifdef SPI_INTERRUPT_MODE

/**
 * avalanche_spi_irq - probe function for SPI Master Controller
 * @irq: IRQ number for this SPI Master
 * @context_data: structure for SPI Master controller avalanche_spi
 * @ptregs:
 *
 * ISR will determine that interrupt arrives either for READ or WRITE command.
 * According to command it will do the appropriate action. It will check
 * transfer length and if it is not zero then dispatch transfer command again.
 * If transfer length is zero then it will indicate the COMPLETION so that
 * avalanche_spi_bufs function can go ahead.
 */
irqreturn_t avalanche_spi_irq( s32 irq, void *context_data,
							   struct pt_regs * ptregs)
{

	struct avalanche_spi *avalanche_spi = context_data;
#ifdef SPI_INTERRUPT_MODE
	volatile u32 event = 0x1,word_complete, cmd;
	u32 bits_per_word;
	u32 rx_data;
	unsigned long timeout;
	irqreturn_t ret = IRQ_NONE;
	struct spi_device *spi = avalanche_spi->spi;
#endif


#ifdef SPI_INT_TASKLET
	DEBUG("IN_IRQ\n");
	avalanche_spi->have_interrupt = 1;
	disable_irq(LNXINTNUM(avalanche_spi->irq));

	DEBUG("Further SPI interrupts disabled\n");
	/* invoke the tasklet */
	tasklet_hi_schedule(&(avalanche_spi->tasklet));
	DEBUG("SPI tasklet re-scheduled\n");

	return IRQ_HANDLED;
#else /* SPI_INT_TASKLET */

	DEBUG("IN_IRQ\n");

    /* Busy bit should not be set */
	timeout = jiffies + AVALANCHE_SPI_DELAY;
	do
	{
		event = avalanche_spi->base->status;
		/* Mask off all other bits except busy bit */
		if(!( (event & 0x01) & 0x1) ){
       	    break;
		}
		if(time_after(jiffies, timeout)){
   	        printk("[SPI] ->Busy bit not cleared for long time, status=%x\n", event);
        }
		event = 0;
	}while(time_after(timeout, jiffies));

	word_complete = event & WC_INT;

	ret = IRQ_HANDLED;
	/* IRQ_HANDLED = 1 defined in linux_interrupt.h */

	if( word_complete )
	{
		/* what command invoked this spi interrupt ? */
		cmd = avalanche_spi->base->cmd;
		bits_per_word = SPI_BITS_PER_WORD(cmd) + 1;

		/* is this interrupt for any data from the slave device? */
		if( avalanche_spi->rx ){
			/* Read the received data */
			rx_data = avalanche_spi->base->data;
			DEBUG("data received %x\n",rx_data);
			/* put the read data in the driver buffer */
			avalanche_spi->get_rx(rx_data, avalanche_spi);
		}
		avalanche_spi->count -= 1;

		/* If the number of words to be transmitted is not zero
    	 * then dispatch transfer command again
         */
		if( avalanche_spi->count > 0 )
		{
			if( avalanche_spi->tx )
			{
				u32 word = avalanche_spi->get_tx(avalanche_spi);

				#ifdef AVALANCHE_SPI_IP_FIX
				word <<= (32- bits_per_word);
				#endif
				avalanche_spi->base->data = word;

				DEBUG("DIRQ=%x\n", word);

			}
			DEBUG("CONT_IRQ = %x\n",
                  avalanche_spi->slave[spi->chip_select].cmd_to_write);

			/* issue the continue command */
			avalanche_spi->base->cmd =
                   avalanche_spi->slave[spi->chip_select].cmd_to_write;
		}
		else
		{
			/* disab?le interrupt after frame completion */
			disable_irq(LNXINTNUM(avalanche_spi->irq));
			/* Notify the completion. avalanche_spi_bufs can now go ahead*/
			complete (&avalanche_spi->done);
		}
	}

	return ret;
#endif
}


#endif /* SPI_INTERRUPT_MODE */

#ifdef SPI_INT_TASKLET
void avalanche_spi_tasklet_func(unsigned long data)
{
	struct avalanche_spi *avalanche_spi = (struct avalanche_spi *)(data);
	volatile u32 event = 0x1, cmd;
	u32 bits_per_word;
	u32 rx_data;
	struct spi_device *spi = avalanche_spi->spi;
	static int tasklet_timeout = BIT_CLEAR_TIME_OUT;

	ENTER
	if( !(avalanche_spi->have_interrupt) ){
		DEBUG("SPI Tasklet need not be invoked, since there is no interrupt \
               from SPI\n");
		return;
	}

spi_tasklet_soft_schedule:
    /* Busy bit should not be set */
    /* Get interrupt events (tx/rx) and clear the interrupt bit
       Word Complete or Frame Complete*/
	event = avalanche_spi->base->status;

	/* check for word complete */
	if((event & 0x3) != 0x2 ) {
        DEBUG("{SPI] Word complete not set (status=%x]\n", event);
		if( !tasklet_timeout-- ){
			printk("{SPI] Word complete not set for a long time (status=%x]\n",
                     event);
            avalanche_spi->have_interrupt = 0;
			tasklet_timeout = 0;
            complete (&avalanche_spi->done);
            EXIT
            return;
		}
        /* don't waste any time in checking for busy, instead reschedule
		 * ourself
         */
        tasklet_hi_schedule(&(avalanche_spi->tasklet));
        EXIT
        return;
	}

	tasklet_timeout = BIT_CLEAR_TIME_OUT;

	/* what command invoked this spi interrupt ? */
	cmd = avalanche_spi->base->cmd;
	bits_per_word = SPI_BITS_PER_WORD(cmd) + 1;

	/* is this interrupt for any data from the slave device? */
	if( avalanche_spi->rx ){
		/* Read the received data */
		rx_data = avalanche_spi->base->data;
		DEBUG("data received %x\n",rx_data);
		/* put the read data in the driver buffer */
		avalanche_spi->get_rx(rx_data, avalanche_spi);
	}

	avalanche_spi->count -= 1;
	/* If the number of words to be transmitted is not zero  then dispatch
     * transfer command again
     */
	if (avalanche_spi->count > 0)
	{
		if (avalanche_spi->tx)
		{
			u32 word = avalanche_spi->get_tx(avalanche_spi);

			#ifdef AVALANCHE_SPI_IP_FIX
			word <<= (32- bits_per_word);
			#endif
			avalanche_spi->base->data = word;

			DEBUG("DIRQ=%x\n", word);

		}

		DEBUG("CONT_IRQ = %x\n",
               avalanche_spi->slave[spi->chip_select].cmd_to_write);

		/* issue the continue command */
		avalanche_spi->base->cmd =
                avalanche_spi->slave[spi->chip_select].cmd_to_write;
		goto spi_tasklet_soft_schedule;
	}
	else
	{
		avalanche_spi->have_interrupt = 0;
		DEBUG("Sending Completion notification to spi_bufs\n");
		/* Notify the completion. avalanche_spi_bufs can now go ahead */
		complete (&avalanche_spi->done);
	}
	EXIT
	return;
}

#endif /* SPI_INT_TASKLET */


/**
 * avalanche_spi_probe - probe function for SPI Master Controller
 * @dev: platform_device structure which contains plateform specific data
 *
 * According to Linux Deviced Model this function will be invoked by Linux
 * with plateform_device struct which contains the device specific info
 * like bus_num, max_chipselect (how many slave devices can be connected),
 * clock freq. of SPI controller, SPI controller's memory range, IRQ number etc.
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

static int __init avalanche_spi_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct spi_master *master;
	struct avalanche_spi *avalanche_spi;
	struct avalanche_spi_platform_data *pdata;
	struct resource *r;
	int ret = 0;

	ENTER

	/* Get resources(memory, IRQ) associated with the device */
	master = spi_alloc_master(dev, sizeof(struct avalanche_spi));

	if( master == NULL ){
		ret = -ENOMEM;
		goto err;
	}

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

	avalanche_spi = spi_master_get_devdata(master);

	/*Call function to get VBUS freq on Yamuna and decide CLK_DIV value*/
	avalanche_spi->ref_freq = PAL_sysClkcGetFreq(AVALANCHE_SPI_REF_CLOCK);

#ifdef SPI_INT_TASKLET
	avalanche_spi->have_interrupt = 0;
	/* tasklet for transferring data */
	tasklet_init( &(avalanche_spi->tasklet), avalanche_spi_tasklet_func,
                  (unsigned long)avalanche_spi);
#endif

	avalanche_spi->bitbang.master         = spi_master_get(master);
	avalanche_spi->bitbang.chipselect     = avalanche_spi_chipselect;
	avalanche_spi->bitbang.set_flen       = avalanche_spi_set_flen;
	avalanche_spi->bitbang.setup_transfer = avalanche_spi_setup_transfer;
	avalanche_spi->bitbang.txrx_bufs      = avalanche_spi_bufs;
	avalanche_spi->sysclk                 = pdata->sysclk;
	avalanche_spi->activate_cs            = pdata->activate_cs;
	avalanche_spi->deactivate_cs          = pdata->deactivate_cs;
	avalanche_spi->get_rx                 = avalanche_spi_rx_buf_u8;
	avalanche_spi->get_tx                 = avalanche_spi_tx_buf_u8;
	avalanche_spi->bitbang.master->setup  = avalanche_spi_setup;

	init_completion(&avalanche_spi->done);

	avalanche_spi->base = (struct avalanche_spi_reg __iomem *)(r->start);
	if (avalanche_spi->base == NULL) {
		ret = -ENOMEM;
		goto put_master;
	}
	avalanche_spi->base->mm_spi_switch = SET_CORE_SPI_MODE;

#ifdef SPI_INTERRUPT_MODE
	avalanche_spi->irq = platform_get_irq(pdev, 0);

	if( avalanche_spi->irq < 0 ){
		ret = -ENXIO;
		goto unmap_io;
	}
	ret = avalanche_intc_set_interrupt_type(LNXINTNUM(avalanche_spi->irq), 0);

    if( ret != 0 )
    {
        DEBUG("irq type set fails\n");
        goto unmap_io;
    }
	/* Register for SPI Interrupt */
	ret = request_irq(LNXINTNUM( avalanche_spi->irq), avalanche_spi_irq,
								 SA_INTERRUPT , "ti_spi", avalanche_spi );

	if( ret != 0 )
	{
		DEBUG("request_irq fails\n");
		goto unmap_io;
	}

	disable_irq(LNXINTNUM(avalanche_spi->irq));
#endif /* SPI_INTERRUPT_MODE */

	master->bus_num = pdata->bus_num;
	master->num_chipselect = pdata->max_chipselect;

	/* SPI controller initializations */
#if (0)
#ifndef CONFIG_MACH_PUMA5
#ifndef CONFIG_MACH_PUMA6
	avalanche_spi->base->clk_ctrl 	= 0;
#endif
#endif
#endif
	avalanche_spi->base->dev_cfg 	= 0;
	avalanche_spi->base->data		= 0;
	/* Put the controller in Core SPI mode by default */

	ret = avalanche_spi_bitbang_start(&avalanche_spi->bitbang);

	if (ret != 0)
		goto free_irq;
	printk(KERN_INFO "%s: AVALANCHE SPI Controller driver at 0x%p \
          (irq = %d)\n",dev_name(avalanche_spi->bitbang.master->dev.parent), avalanche_spi->base, avalanche_spi->irq);

	return ret;

free_irq:
#ifdef SPI_INTERRUPT_MODE
	free_irq(avalanche_spi->irq, avalanche_spi);

unmap_io:
#endif

	iounmap(avalanche_spi->base);
put_master:
	spi_master_put(master);
free_master:
	kfree(master);
err:
	return ret;
}

/* Workaround for flash endless busy state */
void avalanche_spi_unlock_flash_busy_state( struct spi_device *spi )
{
	struct avalanche_spi *avalanche_spi;

    avalanche_spi = spi_master_get_devdata(spi->master);

    /* Clearing the SPICR register */
    avalanche_spi->base->cmd = 0;
}

/**
 * avalanche_spi_remove - remove function for SPI Master Controller
 * @dev: platform_device structure which contains plateform specific data
 *
 * This function will do the reverse action of avalanche_spi_probe function
 * It will free the IRQ and SPI controller's memory region.
 * It will also call spi_bitbang_stop to destroy the work queue which was
 * created by spi_bitbang_start.
 */
static int avalanche_spi_remove(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct avalanche_spi *avalanche_spi;
	struct spi_master *master;
	ENTER
	master = dev_get_drvdata(dev);

	avalanche_spi = spi_master_get_devdata(master);
	avalanche_spi_bitbang_stop(&avalanche_spi->bitbang);
#ifdef SPI_INTERRUPT_MODE
	free_irq(avalanche_spi->irq, avalanche_spi);
#endif

	iounmap(avalanche_spi->base);
	spi_master_put(avalanche_spi->bitbang.master);
	EXIT
	return 0;
}

static struct platform_driver avalanche_spi_driver = {
	.driver = {
		.name  = "ti_spi",
		.bus   = &platform_bus_type,
		.owner = THIS_MODULE,
	},
	.probe     = avalanche_spi_probe,
	.remove    = avalanche_spi_remove,
	.shutdown  = NULL,
	.suspend   = NULL,
	.resume    = NULL,
};


static int __init avalanche_spi_init(void)
{
	ENTER
	return platform_driver_register(&avalanche_spi_driver);
	EXIT
}

static void __exit avalanche_spi_exit(void)
{
	platform_driver_unregister(&avalanche_spi_driver);
}

module_init(avalanche_spi_init);
module_exit(avalanche_spi_exit);

MODULE_AUTHOR("Texas Instruments Inda Pvt LTD.");
MODULE_DESCRIPTION("AVALANCHE SPI/SFI Master Controller Driver");
MODULE_LICENSE("GPL");
