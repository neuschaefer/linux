/*
 *
 * spi_ticodec.c
 * Description:
 * codec spi master driver 
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
/*
Includes Intel Corporation's changes/modifications dated: 2014.
Changed/modified portions - Copyright © 2014, Intel Corporation.
*/


/*
 * TI Avalanche Codec SPI controller driver.
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
#include <linux/spi/spi_ticodec.h>
#include <linux/spi/avalanche_spi_bitbang.h>


//#define DEBUG_TI(fmt,arg...)  printk(KERN_EMERG fmt , ##arg);
#define DEBUG_TI(fmt,arg...) 

#define ENTER DEBUG_TI("TI CODEC SPI [%s->line %d] \n", __FUNCTION__, __LINE__);
#define EXIT  DEBUG_TI("TI CODEC SPI [%s->line %d] \n", __FUNCTION__, __LINE__);


/************************************************
* CONTROLLER REGISTER BITS 
************************************************/
#define SPI_DEF_FREQ 							(5000000) 
#define SPI_CLK_EN								(1 << 24)
#define SPI_BPW_TO_CMD(word_len)				((word_len & 0x1F) << 16)
#define SPI_CMD_TO_BPW(cmd) 					((cmd >> 16) & 0x1F)

#if (CONFIG_SPI_TI_CODEC_ZSI_MODE)
#define SPI_CMD_READ 							(0x01 << 23)
#else
#define SPI_CMD_READ 							(0x01 << 21)
#endif

#define SPI_CMD_WRITE 							(0x01 << 22)
#define SPI_INT_ENABLE 							(0x01 << 25)

#if (CONFIG_SPI_TI_CODEC_ZSI_MODE)
#define SPI_DOUT_DIR							(0x00000000) // ZSI mode - meaning always 4 wire mode
#elif (CONFIG_SPI_TI_CODEC_NO_ZSI_3_WIRE_MODE)
#define SPI_DOUT_DIR							(0x80000000) // No ZSI mode and 3 wire mode
#else
#define SPI_DOUT_DIR							(0x00000000) // No ZSI mode and 4 wire mode
#endif


/**********************************************
* SPI write data register
**********************************************/
#define SPI_COMPLETE								(0x80000000)

/* operating mode selection from kconfig */
#ifdef CONFIG_SPI_INTERRUPT_MODE
#define SPI_INTERRUPT_MODE 							(1)
#endif
/* worst case 2 seconds delay */
#define AVALANCHE_TI_CODEC_SPI_DELAY				(msecs_to_jiffies(2000))
#ifdef CONFIG_SPI_TASKLET_MODE
#define SPI_INTERRUPT_MODE 							(1)
#define SPI_INT_TASKLET 							(1)
#endif

#ifdef SPI_POLLING_MODE
#undef SPI_INTERRUPT_MODE
#undef SPI_INT_TASKLET
#endif

#define BIT_CLEAR_TIME_OUT						(0x0FFFFF)
#define SPI_RW_CMD_MASK							(~0xE00000)

/* SPI Controller registers */
struct ti_codec_spi_reg {		
	volatile u32 spcr1;
	volatile u32 spdr;
	volatile u32 spbr;
};

struct ti_codec_spi_slave {
	u32 cmd_to_write;
    u32 bytes_per_word;
    u32 frame_len;
    u8 active_cs;
};

/* SPI Controller driver's private data. */
struct ti_codec_spi {
	/* bitbang has to be first */
	struct spi_bitbang bitbang;
	struct completion done;
	struct ti_codec_spi_reg __iomem *base;
	/* rx & tx bufs from the spi_transfer */
	const void *tx;
	void *rx;
	/* functions to deal with different sized buffers */
	void (*get_rx) (u32 rx_data, struct ti_codec_spi *);
	u32(*get_tx) (struct ti_codec_spi *);
	int count;
	u32 irq;
	u32 sysclk;
	u32 ref_freq;
	/* chip select activation deactivation */
	int	(*activate_cs)(u8 cs, u8 polarity,struct ti_ctlr_cs_sel_t *ctlr_cs_sel);
	int	(*deactivate_cs)(u8 cs, u8 polarity,struct ti_ctlr_cs_sel_t *ctlr_cs_sel);
	struct ti_codec_spi_slave slave[ AVALANCHE_SPI_MAX_CHIPSELECT ];
	struct spi_device *spi;
#ifdef SPI_INT_TASKLET
	struct tasklet_struct tasklet;
	u8 have_interrupt;
#endif
};

#define ti_codec_SPI_RX_BUF(type) 	  \
static inline void ti_codec_spi_rx_buf_##type(u32 data, \
                                        struct ti_codec_spi *ti_codec_spi) \
{									  \
	type * rx = ti_codec_spi->rx;	  \
	*rx++ = (type)data;				  \
	ti_codec_spi->rx = rx;			  \
}

#define ti_codec_SPI_TX_BUF(type)	 \
static inline u32 ti_codec_spi_tx_buf_##type(\
                                        struct ti_codec_spi *ti_codec_spi)	\
{								           \
	u32 data;				        	   \
	const type * tx = ti_codec_spi->tx;   \
	data = *tx++;						   \
	ti_codec_spi->tx = tx;				   \
	return data;						   \
}

ti_codec_SPI_RX_BUF(u8)
ti_codec_SPI_RX_BUF(u16)
ti_codec_SPI_RX_BUF(u32)
ti_codec_SPI_TX_BUF(u8)
ti_codec_SPI_TX_BUF(u16)
ti_codec_SPI_TX_BUF(u32)

#ifdef DEBUG
static int ti_codec_spi_reg_dump(unsigned long base)
{
	volatile struct ti_codec_spi_reg *regs = (volatile struct ti_codec_spi_reg *)base;
	printk("Spi register dump\n");
	printk("spcr1 addr = 0x%08x  spcr1 = 0x%08x\n", &(regs->spcr1), regs->spcr1);
	printk("spdr  addr = 0x%08x  spdr  = 0x%08x\n", &(regs->spdr), regs->spdr);
	printk("spbr  addr = 0x%08x  spbr  = 0x%08x\n", &(regs->spbr), regs->spbr);
    return 0;
};
#endif

/* 
 * Bitbang layer uses this interface to set the framelength 
 * for the slave device for current transfer in progress
 */
void ti_codec_spi_set_flen(struct spi_device *spi, u32 flen)
{
	struct ti_codec_spi *ti_codec_spi;
	ENTER
	ti_codec_spi = spi_master_get_devdata(spi->master);
	ti_codec_spi->slave[spi->chip_select].frame_len = (flen-1);
	DEBUG_TI("flen = %d, frame_len = %d\n", flen, 
		  ti_codec_spi->slave[spi->chip_select].frame_len);
	EXIT
}

/*
 * Interface to control the chip select signal
 */
static void ti_codec_spi_chipselect(struct spi_device *spi, int value)
{
	struct ti_codec_spi *ti_codec_spi;
    /* determine - active high or active low */
	u8 pol = spi->mode & SPI_CS_HIGH ? 1 : 0; 
	struct ti_ctlr_cs_sel_t ctlr_cs_sel;
	u8 cs;
	ENTER
	ti_codec_spi = spi_master_get_devdata(spi->master);
    ctlr_cs_sel.cs = spi->chip_select;
	ti_codec_spi->slave[spi->chip_select].active_cs = ctlr_cs_sel.cs;
	ctlr_cs_sel.pol = pol;

	/* board specific chip select logic decides the polarity and cs line for 
     * the controller 
     */
	if ( value == BITBANG_CS_ACTIVE) {
		if (ti_codec_spi->activate_cs) 
            /* Activate the chip select signal in the board */
			ti_codec_spi->activate_cs(spi->chip_select, pol , &ctlr_cs_sel);
	}
	else {
		return;
	}
	pol = ctlr_cs_sel.pol;
	ti_codec_spi->slave[spi->chip_select].active_cs = cs = ctlr_cs_sel.cs;
	EXIT
}

/**
 * ti_codec_spi_setup_transfer - This functions will determine transfer method
 * @spi: spi device on which data transfer to be done
 * @t: spi transfer in which transfer info is filled
 *
 * This function determines data transfer method (8/16/32 bit transfer).
 * It will also set the SPI Clock Control register according to 
 * SPI slave device freq.
 */
static int ti_codec_spi_setup_transfer( struct spi_device *spi, 
                                         struct spi_transfer *t )
{
	struct ti_codec_spi *ti_codec_spi;
	struct ti_ctlr_cs_sel_t ctlr_cs_sel;
	u32 cmd_to_write = 0;
	u8 bits_per_word = 0;
	u32 hz = 0;
	u32 clk_div;
	ENTER
	ti_codec_spi = spi_master_get_devdata(spi->master);
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
		ti_codec_spi->get_rx = ti_codec_spi_rx_buf_u8;
		ti_codec_spi->get_tx = ti_codec_spi_tx_buf_u8;
		ti_codec_spi->slave[spi->chip_select].bytes_per_word = 1;
	} else if (bits_per_word <= 16) {
		ti_codec_spi->get_rx = ti_codec_spi_rx_buf_u16;
		ti_codec_spi->get_tx = ti_codec_spi_tx_buf_u16;
		ti_codec_spi->slave[spi->chip_select].bytes_per_word = 2;
	} else if (bits_per_word <= 32) {
		ti_codec_spi->get_rx = ti_codec_spi_rx_buf_u32;
		ti_codec_spi->get_tx = ti_codec_spi_tx_buf_u32;
		ti_codec_spi->slave[spi->chip_select].bytes_per_word = 4;
	}else {
		return -1;
	}
	/* Current transfer not interested in overriding the existing frequency */
	if(t && !hz) {
		goto no_freq;
	}
	if(!hz)  {
		hz = spi->max_speed_hz;
		if(!hz)  {
			hz = SPI_DEF_FREQ; /* defaulting to 2Mhz*/
			printk("[SPI] -> Slave device speed not set correctly.\
					 Trying with %dHz\n", hz);
		}
	}
	/* requested freq can't be more than ref_clk */
	if(hz > ti_codec_spi->ref_freq)
		hz = ti_codec_spi->ref_freq;
	clk_div = (ti_codec_spi->ref_freq/hz) - 1;
	cmd_to_write =	SPI_CLK_EN	| (clk_div & 0xFFFF);	
/**************************************************************************/
no_freq:
	cmd_to_write |= SPI_BPW_TO_CMD((bits_per_word));
	/* Determine the slave device to operate on */
 	if (ti_codec_spi->activate_cs) 
		/* Activate the chip select signal in the board */
		ti_codec_spi->activate_cs(spi->chip_select, 
							((spi->mode & SPI_CS_HIGH) ? 1 : 0), &ctlr_cs_sel);
#ifdef SPI_INTERRUPT_MODE
	 /* enable word complete interrupt */
	cmd_to_write |= SPI_INT_ENABLE; 
#endif
	if(spi->mode & SPI_3_WIRE)
		cmd_to_write |= SPI_DOUT_DIR;
	ti_codec_spi->slave[spi->chip_select].cmd_to_write = cmd_to_write;
	DEBUG_TI("[C2W=%x]\n", ti_codec_spi->slave[spi->chip_select].cmd_to_write);
	EXIT
	return 0;
}


/**
 * ti_codec_spi_setup - This functions will set default transfer method
 * @spi: spi device on which data transfer to be done
 *
 * This functions sets the default transfer method.
 */

static int ti_codec_spi_setup(struct spi_device *spi)
{
	int retval =0;
	struct ti_codec_spi *ti_codec_spi;
	ENTER
	ti_codec_spi = spi_master_get_devdata(spi->master);
	/*if bits per word length is zero then set it default 8*/
	if (!spi->bits_per_word)
		spi->bits_per_word = 8;
	ti_codec_spi->slave[spi->chip_select].cmd_to_write = 0;
	retval = ti_codec_spi_setup_transfer(spi, NULL);
	EXIT
	return retval;
}


/**
 * ti_codec_spi_transfer - functions which will handle transfer data
 * @spi: spi device on which data transfer to be done
 * @t: spi transfer in which transfer info is filled
 *
 * This function will put data to be transferred into data register
 * of SPI controller and then wait untill the completion will be marked
 * by the IRQ Handler.
 */
static int ti_codec_spi_transfer(struct spi_device *spi,struct spi_transfer *t)
{
	struct ti_codec_spi *ti_codec_spi;
	u8 conv = 1, bits_per_word = 0;
	u32 word = 0, cmd_to_write = 0;
#ifndef SPI_INTERRUPT_MODE
	unsigned long timeout;
	volatile u32 event = 0x0;
	u32 rx_data=0;
#endif
	ENTER

	ti_codec_spi = spi_master_get_devdata(spi->master);
	ti_codec_spi->tx = t->tx_buf;
	ti_codec_spi->rx = t->rx_buf;
	/* convert len to words bbased on bits_per_word */
	bits_per_word = 
			SPI_CMD_TO_BPW(ti_codec_spi->slave[spi->chip_select].cmd_to_write);
	if(t)
		bits_per_word = t->bits_per_word;
	/* if bits_per_word is not set then set it default */
	if (!bits_per_word) 
		bits_per_word = spi->bits_per_word;
	/* Assign function pointer to appropriate transfer method 8bit/16bit or 
     * 32bit transfer 
     */
	if( bits_per_word <= 8 ){
		ti_codec_spi->slave[spi->chip_select].bytes_per_word = 1;
	}else if( bits_per_word <= 16 ){
		ti_codec_spi->slave[spi->chip_select].bytes_per_word = 2;
	}else if( bits_per_word <= 32 ){
		ti_codec_spi->slave[spi->chip_select].bytes_per_word = 4;
	} else {
		return -1;
    }
	conv  = ti_codec_spi->slave[spi->chip_select].bytes_per_word;
	ti_codec_spi->count = t->len / conv;
	DEBUG_TI("chipselect=%d, bytes_per_word=%d, t->len=%d, conv=%d\n", 
				spi->chip_select, 
				ti_codec_spi->slave[spi->chip_select].bytes_per_word,
				t->len, conv);
	INIT_COMPLETION(ti_codec_spi->done);
	/* Determine the command to execute READ or WRITE */
	if( t->tx_buf ){	
		cmd_to_write = SPI_CMD_WRITE;
		DEBUG_TI("Settng WRITE cmd\n");
	}
	else{
		cmd_to_write = SPI_CMD_READ;
		DEBUG_TI("Settng READ cmd\n");
	}
#ifdef SPI_INTERRUPT_MODE 
	/* clean the r/w cmd bits */	
	ti_codec_spi->slave[spi->chip_select].cmd_to_write &= SPI_RW_CMD_MASK; 

	/* update command */
	ti_codec_spi->slave[spi->chip_select].cmd_to_write |= cmd_to_write;
	DEBUG_TI("ti_codec_spi->count=%d \n",ti_codec_spi->count);
	/* The ISR needs the information about current slave device whose 
     * transmission is in progress 
     */
	ti_codec_spi->spi = spi;
	/* Are we going to write data to the slave device? */
	if (ti_codec_spi->tx)
	{
		word = ti_codec_spi->get_tx(ti_codec_spi);
		#ifdef AVALANCHE_TI_CODEC_SPI_IP_FIX
		word <<= (32- bits_per_word);
		#endif
		ti_codec_spi->base->spdr = word;
	}
	/* Start the transfer */
	ti_codec_spi->base->spcr1 = 
		((ti_codec_spi->slave[spi->chip_select].cmd_to_write) | SPI_DOUT_DIR);
	enable_irq(LNXINTNUM(ti_codec_spi->irq)); 
	DEBUG_TI("[C_WRITTEN=%x]\n",
		ti_codec_spi->slave[spi->chip_select].cmd_to_write);
	DEBUG_TI("B4 wait_for_completion\n");
	/* wait for completion signal from ISR */
	wait_for_completion(&ti_codec_spi->done);
	/* If we are working in polling mode */
#else	
	/* clean the bits */	
	ti_codec_spi->slave[spi->chip_select].cmd_to_write &= SPI_RW_CMD_MASK; 
	/* update command */
	ti_codec_spi->slave[spi->chip_select].cmd_to_write |= cmd_to_write;	
	DEBUG_TI("reg_val = %x, frm_len=%d, c2w=%x\n", cmd_to_write, 
		ti_codec_spi->slave[spi->chip_select].frame_len, 
		ti_codec_spi->slave[spi->chip_select].cmd_to_write);
	DEBUG_TI("ti_codec_spi->count=%d \n",ti_codec_spi->count);
	/* start dispatching the words */
 	while(ti_codec_spi->count > 0)
	{
		if (ti_codec_spi->tx)
       	{
			/* write the word to be transmitted into the data register */
			word = ti_codec_spi->get_tx(ti_codec_spi);
			#ifdef AVALANCHE_TI_CODEC_SPI_IP_FIX
			word <<= (32- bits_per_word);
			#endif
			ti_codec_spi->base->spdr = word;
			DEBUG_TI("[D=%x]\n", word);
		}
		/* issue the command */
		DEBUG_TI("[C=%x]\n",ti_codec_spi->slave[spi->chip_select].cmd_to_write);
		ti_codec_spi->base->spcr1 = 
                          ((ti_codec_spi->slave[spi->chip_select].cmd_to_write)  | SPI_DOUT_DIR);
		/* Wait for busy bit/word complete to clear */
		timeout = jiffies + AVALANCHE_TI_CODEC_SPI_DELAY;
		do 
		{   
			event = 0;
			/* tx */
			if(ti_codec_spi->tx)
			{
				event = ti_codec_spi->base->spdr;
				if(!(event & SPI_COMPLETE))
					break;
			}
			/* rx */
			else
			{
				event = ti_codec_spi->base->spbr;
				if(event & SPI_COMPLETE)
					break;
			}
			if(time_after(jiffies, timeout))
    	        printk("[SPI] -> Word complete not set for a long time even after busy bit is cleared, status=%x\n", event);
		}while(time_after(timeout, jiffies));
		udelay(1);
		DEBUG_TI("SPI status_reg_val=%x\n", event);
		/* do we have something to read from the slave device */
		if (ti_codec_spi->rx)
		{
           	rx_data = ti_codec_spi->base->spbr & ~(SPI_COMPLETE);
			DEBUG_TI("data read %x \n",rx_data);
			/* place the read data in the driver buffer */
			ti_codec_spi->get_rx(rx_data, ti_codec_spi);
		}
		ti_codec_spi->count--;
	}
#endif
	/* SPI Framework maintains the count only in bytes so convert 
     * back to bytes 
     */
	ti_codec_spi->count *= conv;
	EXIT
	return t->len;
}

#ifdef SPI_INTERRUPT_MODE

/**
 * ti_codec_spi_irq - probe function for SPI Master Controller
 * @irq: IRQ number for this SPI Master
 * @context_data: structure for SPI Master controller ti_codec_spi
 * @ptregs: 
 *
 * ISR will determine that interrupt arrives either for READ or WRITE command.
 * According to command it will do the appropriate action. It will check 
 * transfer length and if it is not zero then dispatch transfer command again.
 * If transfer length is zero then it will indicate the COMPLETION so that
 * ti_codec_spi_transfer function can go ahead.
 */
irqreturn_t ti_codec_spi_irq( s32 irq, void *context_data,
							   struct pt_regs * ptregs)
{

	struct ti_codec_spi *ti_codec_spi = context_data;
#ifdef SPI_INTERRUPT_MODE	
	volatile u32 event = 0x1,word_complete, cmd;
	u32 bits_per_word;
	u32 rx_data;
	unsigned long timeout;
	irqreturn_t ret = IRQ_NONE;
	struct spi_device *spi = ti_codec_spi->spi;
#endif
#ifdef SPI_INT_TASKLET
	DEBUG_TI("IN_IRQ\n");
	ti_codec_spi->have_interrupt = 1;
	disable_irq(LNXINTNUM(ti_codec_spi->irq));	
	DEBUG_TI("Further SPI interrupts disabled\n");	
	/* invoke the tasklet */
	tasklet_hi_schedule(&(ti_codec_spi->tasklet));
	DEBUG_TI("SPI tasklet re-scheduled\n");
	return IRQ_HANDLED;
#else /* SPI_INT_TASKLET */
	DEBUG_TI("IN_IRQ\n");
    /* Busy bit should not be set */
	timeout = jiffies + AVALANCHE_TI_CODEC_SPI_DELAY;
	word_complete = 0;
	do 
	{  
		event = 0;
		/* tx */
		if(ti_codec_spi->tx)
		{
			event = ti_codec_spi->base->spdr;
			if(!(event & SPI_COMPLETE))
			{
				word_complete = 1;
				break;
			}
		}
		/* rx */
		else
		{
			event = ti_codec_spi->base->spbr;
			if(event & SPI_COMPLETE)
			{
				word_complete = 1;
				break;
			}
		}

		if(time_after(jiffies, timeout)){
   	        printk("[SPI] ->Busy bit not cleared for a long time, status=%x\n", event);
        }
	}while(time_after(timeout, jiffies));
	ret = IRQ_HANDLED; 
	/* IRQ_HANDLED = 1 defined in linux_interrupt.h */
	if( word_complete )
	{
		/* what command invoked this spi interrupt ? */
		cmd = ti_codec_spi->base->spcr1;
		bits_per_word = SPI_CMD_TO_BPW(cmd) + 1;
		/* is this interrupt for any data from the slave device? */
		if( ti_codec_spi->rx ){
			/* Read the received data */
			rx_data = ti_codec_spi->base->spbr & (~SPI_COMPLETE);
			DEBUG_TI("data received %x\n",rx_data);
			/* put the read data in the driver buffer */
			ti_codec_spi->get_rx(rx_data, ti_codec_spi);
		}
		ti_codec_spi->count -= 1;
		/* If the number of words to be transmitted is not zero 
    	 * then dispatch transfer command again 
         */
		if( ti_codec_spi->count > 0 )
		{
			if( ti_codec_spi->tx )
			{
				u32 word = ti_codec_spi->get_tx(ti_codec_spi);
				#ifdef AVALANCHE_TI_CODEC_SPI_IP_FIX	
				word <<= (32- bits_per_word);
				#endif
				ti_codec_spi->base->spdr = word;
				DEBUG_TI("DIRQ=%x\n", word);
			}
			DEBUG_TI("CONT_IRQ = %x\n", 
                  ti_codec_spi->slave[spi->chip_select].cmd_to_write);
			/* issue the continue command */	
			ti_codec_spi->base->spcr1 = 
                   ((ti_codec_spi->slave[spi->chip_select].cmd_to_write) | SPI_DOUT_DIR); 
		}
		else
		{
			/* disab?le interrupt after frame completion */
			disable_irq(LNXINTNUM(ti_codec_spi->irq));		
			/* Notify the completion. ti_codec_spi_transfer can now go ahead*/
			complete (&ti_codec_spi->done); 
		}
	}
	return ret;
#endif
}


#endif /* SPI_INTERRUPT_MODE */

#ifdef SPI_INT_TASKLET
void ti_codec_spi_tasklet_func(unsigned long data)
{
	struct ti_codec_spi *ti_codec_spi = (struct ti_codec_spi *)(data);
	volatile u32 event = 0x1, cmd;
	u32 bits_per_word, word_complete;
	u32 rx_data;
	struct spi_device *spi = ti_codec_spi->spi;
	static int tasklet_timeout = BIT_CLEAR_TIME_OUT;
	ENTER
	if( !(ti_codec_spi->have_interrupt) ){
		DEBUG_TI("SPI Tasklet need not be invoked, since there is no interrupt \
               from SPI\n");
		return;
	}
spi_tasklet_soft_schedule:
    /* Busy bit should not be set */
    /* Get interrupt events (tx/rx) and clear the interrupt bit
       Word Complete or Frame Complete*/
	word_complete = 0;
	event = 0;
	/* tx */
	if(ti_codec_spi->tx)
	{
		event = ti_codec_spi->base->spdr;
		if(!(event & SPI_COMPLETE))
		{
			word_complete = 1;
			break;
		}
	}
	/* rx */
	else
	{
		event = ti_codec_spi->base->spbr;
		if(event & SPI_COMPLETE)
		{
			word_complete = 1;
			break;
		}
	}

	/* check for word complete */
	if(!word_complete) {
        DEBUG_TI("{SPI] Word complete not set (status=%x]\n", event);
		if( !tasklet_timeout-- ){
			printk("{SPI] Word complete not set for a long time (status=%x]\n",
                     event);
            ti_codec_spi->have_interrupt = 0;
			tasklet_timeout = 0;
            complete (&ti_codec_spi->done);
            EXIT
            return;
		}
        /* don't waste any time in checking for busy, instead reschedule 
		 * ourself 
         */
        tasklet_hi_schedule(&(ti_codec_spi->tasklet));
        EXIT
        return;
	}
	tasklet_timeout = BIT_CLEAR_TIME_OUT;
	/* what command invoked this spi interrupt ? */
	cmd = ti_codec_spi->base->spcr1;
	bits_per_word = SPI_CMD_TO_BPW(cmd) + 1;
	/* is this interrupt for any data from the slave device? */
	if( ti_codec_spi->rx ){
		/* Read the received data */
		rx_data = ti_codec_spi->base->spbr & (~SPI_COMPLETE);
		DEBUG_TI("data received %x\n",rx_data);
		/* put the read data in the driver buffer */
		ti_codec_spi->get_rx(rx_data, ti_codec_spi);
	}
	ti_codec_spi->count -= 1;
	/* If the number of words to be transmitted is not zero  then dispatch
     * transfer command again 
     */
	if (ti_codec_spi->count > 0)
	{
		if (ti_codec_spi->tx)
		{
			u32 word = ti_codec_spi->get_tx(ti_codec_spi);
			#ifdef AVALANCHE_TI_CODEC_SPI_IP_FIX	
			word <<= (32- bits_per_word);
			#endif
			ti_codec_spi->base->spdr = word;
			DEBUG_TI("DIRQ=%x\n", word);
		}
		DEBUG_TI("CONT_IRQ = %x\n", 
               ti_codec_spi->slave[spi->chip_select].cmd_to_write);
		/* issue the continue command */	
		ti_codec_spi->base->spcr1 = 
                ((ti_codec_spi->slave[spi->chip_select].cmd_to_write) | SPI_DOUT_DIR);
		goto spi_tasklet_soft_schedule;
	}
	else
	{
		ti_codec_spi->have_interrupt = 0;
		DEBUG_TI("Sending Completion notification to spi_bufs\n");
		/* Notify the completion. ti_codec_spi_transfer can now go ahead */
		complete (&ti_codec_spi->done); 
	}
	EXIT
	return;
}

#endif /* SPI_INT_TASKLET */


/**
 * ti_codec_spi_probe - probe function for SPI Master Controller
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

static int __init ti_codec_spi_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct spi_master *master;
	struct ti_codec_spi *ti_codec_spi;
	struct ti_codec_spi_platform_data *pdata;
	struct resource *r;
	int ret = 0;
	ENTER
	/* Get resources(memory, IRQ) associated with the device */
	master = spi_alloc_master(dev, sizeof(struct ti_codec_spi));
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
	ti_codec_spi = spi_master_get_devdata(master);
	/*Call function to get VBUS freq on Yamuna and decide CLK_DIV value*/
	ti_codec_spi->ref_freq = PAL_sysClkcGetFreq(AVALANCHE_CODEC_SPI_REF_CLOCK);
#ifdef SPI_INT_TASKLET
	ti_codec_spi->have_interrupt = 0;
	/* tasklet for transferring data */
	tasklet_init( &(ti_codec_spi->tasklet), ti_codec_spi_tasklet_func,
                  (unsigned long)ti_codec_spi);
#endif
	ti_codec_spi->bitbang.master         = spi_master_get(master);
	ti_codec_spi->bitbang.chipselect     = ti_codec_spi_chipselect;
	ti_codec_spi->bitbang.set_flen       = ti_codec_spi_set_flen;
	ti_codec_spi->bitbang.setup_transfer = ti_codec_spi_setup_transfer;
	ti_codec_spi->bitbang.txrx_bufs      = ti_codec_spi_transfer;
	ti_codec_spi->sysclk                 = pdata->sysclk;
	ti_codec_spi->activate_cs            = pdata->activate_cs;
	ti_codec_spi->deactivate_cs          = pdata->deactivate_cs;
	ti_codec_spi->get_rx                 = ti_codec_spi_rx_buf_u8;
	ti_codec_spi->get_tx                 = ti_codec_spi_tx_buf_u8;
	if (ti_codec_spi->bitbang.master)
            ti_codec_spi->bitbang.master->setup  = ti_codec_spi_setup;
	init_completion(&ti_codec_spi->done);
	ti_codec_spi->base = (struct ti_codec_spi_reg __iomem *)(r->start);
	if (ti_codec_spi->base == NULL) {
		ret = -ENOMEM;
		goto put_master;
	}
#ifdef SPI_INTERRUPT_MODE
	ti_codec_spi->irq = platform_get_irq(pdev, 0);
	if( ti_codec_spi->irq < 0 ){
		ret = -ENXIO;
		goto unmap_io;
	}
	ret = avalanche_intc_set_interrupt_type(LNXINTNUM(ti_codec_spi->irq), 0);
    if( ret != 0 )
    {
        DEBUG_TI("irq type set fails\n");
        goto unmap_io;
    }
	/* Register for SPI Interrupt */
	ret = request_irq(LNXINTNUM( ti_codec_spi->irq), ti_codec_spi_irq,
								 SA_INTERRUPT , "ti_codec_spi", ti_codec_spi );
	if( ret != 0 )
	{
		DEBUG_TI("request_irq fails\n");
		goto unmap_io;
	}
	disable_irq(LNXINTNUM(ti_codec_spi->irq));		
#endif /* SPI_INTERRUPT_MODE */
	master->bus_num = pdata->bus_num;
	master->num_chipselect = pdata->max_chipselect;
	ENTER

	/* SPI controller initializations */
	ti_codec_spi->base->spcr1 	= TI_CODEC_SPI_DEF_SPCR1; /* Bi-Directional, Data Clock Enable, Datalen = 8 bits, Div clock =1*/

	/* Put the controller in Core SPI mode by default */
	ret = avalanche_spi_bitbang_start(&ti_codec_spi->bitbang);
	if (ret != 0)
		goto free_irq;
	printk(KERN_INFO "%s: TI Codec SPI Controller driver at 0x%p \
          (irq = %d)\n",dev_name(ti_codec_spi->bitbang.master->dev.parent), ti_codec_spi->base, ti_codec_spi->irq);
	return ret;
free_irq:
#ifdef SPI_INTERRUPT_MODE
	free_irq(ti_codec_spi->irq, ti_codec_spi);
unmap_io:
#endif
put_master:
	spi_master_put(master);
free_master:
	kfree(master);
err:
	return ret;
}



/**
 * ti_codec_spi_remove - remove function for SPI Master Controller
 * @dev: platform_device structure which contains plateform specific data
 *
 * This function will do the reverse action of ti_codec_spi_probe function
 * It will free the IRQ and SPI controller's memory region. 
 * It will also call spi_bitbang_stop to destroy the work queue which was
 * created by spi_bitbang_start.
 */
static int ti_codec_spi_remove(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct ti_codec_spi *ti_codec_spi;
	struct spi_master *master;
	ENTER
	master = dev_get_drvdata(dev);
	ti_codec_spi = spi_master_get_devdata(master);
	avalanche_spi_bitbang_stop(&ti_codec_spi->bitbang);
#ifdef SPI_INTERRUPT_MODE
	free_irq(ti_codec_spi->irq, ti_codec_spi);
#endif
	spi_master_put(ti_codec_spi->bitbang.master);
	EXIT
	return 0;
}

static struct platform_driver ti_codec_spi_driver = {
	.driver = {
		.name  = "ti_codec_spi",
		.bus   = &platform_bus_type,
		.owner = THIS_MODULE,
	},
	.probe     = ti_codec_spi_probe,
	.remove    = ti_codec_spi_remove,
	.shutdown  = NULL,
	.suspend   = NULL,
	.resume    = NULL,
};
   

static int __init ti_codec_spi_init(void)
{
	ENTER
	return platform_driver_register(&ti_codec_spi_driver);
	EXIT
}

static void __exit ti_codec_spi_exit(void)
{
	platform_driver_unregister(&ti_codec_spi_driver);
}

module_init(ti_codec_spi_init);
module_exit(ti_codec_spi_exit);

MODULE_AUTHOR("Texas Instruments");
MODULE_DESCRIPTION("TI CODEC SPI Master Driver");
MODULE_LICENSE("GPL");
