/*
 *
 * avalanche_spi_bitbang.c
 * Description:
 * referred drivers/spi/spi_bitbang.c
 * avalanche_spi_bitbang.c - polling/bitbanging TI avalanche SPI master 
 *                           controller driver utilities
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

#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/spi/spi.h>
#include <linux/spi/avalanche_spi.h>
#include <linux/spi/avalanche_spi_bitbang.h>
#include <linux/module.h>

#define SLAB_KERNEL GFP_KERNEL

#if defined(CONFIG_SPI_DEBUG)
#define SPI_DEBUG(fmt,arg...)  printk(KERN_EMERG fmt , ##arg);
#else
#define SPI_DEBUG(fmt,arg...)
#endif

#define ENTER SPI_DEBUG("[Ei %s-%d] \n", __FUNCTION__, __LINE__);
#define EXIT  SPI_DEBUG("[Ex %s-%d] \n", __FUNCTION__, __LINE__);

#define AVALANCHE_SPI_MAX_FLEN 4096


/*
 * FIRST PART (OPTIONAL):  word-at-a-time spi_transfer support.
 * Use this for GPIO or shift-register level hardware APIs.
 *
 * spi_bitbang_cs is in spi_device->controller_state, which is unavailable
 * to glue code.  These bitbang setup() and cleanup() routines are always
 * used, though maybe they're called from controller-aware code.
 *
 * chipselect() and friends may use use spi_device->controller_data and
 * controller registers as appropriate.
 *
 *
 * NOTE:  SPI controller pins can often be used as GPIO pins instead,
 * which means you could use a bitbang driver either to get hardware
 * working quickly, or testing for differences that aren't speed related.
 */

struct spi_bitbang_cs {
	unsigned nsecs;	/* (clock cycle time)/2 */

	u32	(*txrx_word)(struct spi_device *spi, unsigned nsecs,u32 word, u8 bits);

	unsigned (*txrx_bufs)(struct spi_device *, 
	u32 (*txrx_word)(struct spi_device *spi,unsigned nsecs,u32 word, u8 bits),

	unsigned, 
    struct spi_transfer *);
};


static unsigned bitbang_txrx_8( struct spi_device *spi, 
	u32	(*txrx_word)(struct spi_device *spi,unsigned nsecs, u32 word, u8 bits),
	unsigned ns,
	struct spi_transfer	*t )
 
{
	unsigned bits  = spi->bits_per_word;
	unsigned count = t->len;
	const u8 *tx   = t->tx_buf;
	u8		 *rx   = t->rx_buf;

	while (likely(count > 0)) {
		u8		word = 0;

		if (tx)
			word = *tx++;
		word = txrx_word(spi, ns, word, bits);
		if (rx)
			*rx++ = word;
		count -= 1;
	}
	return t->len - count;
}

static unsigned bitbang_txrx_16(struct spi_device	*spi,
	u32 (*txrx_word)(struct spi_device *spi,unsigned nsecs,	u32 word, u8 bits),
	unsigned ns,
	struct spi_transfer	*t) 
{
	unsigned bits  = spi->bits_per_word;
	unsigned count = t->len;
	const u16 *tx  = t->tx_buf;
	u16		  *rx  = t->rx_buf;

	while (likely(count > 1)) {
		u16		word = 0;

		if (tx)
			word = *tx++;
		word = txrx_word(spi, ns, word, bits);
		if (rx)
			*rx++ = word;
		count -= 2;
	}
	return t->len - count;
}

static unsigned bitbang_txrx_32( struct spi_device	*spi,
	u32 (*txrx_word)(struct spi_device *spi,unsigned nsecs,	u32 word, u8 bits),
	unsigned ns,
	struct spi_transfer	*t) 
{
	unsigned bits  = spi->bits_per_word;
	unsigned count = t->len;
	const u32 *tx  = t->tx_buf;
	u32		 *rx   = t->rx_buf;

	while (likely(count > 3)) {
		u32		word = 0;

		if (tx)
			word = *tx++;
		word = txrx_word(spi, ns, word, bits);
		if (rx)
			*rx++ = word;
		count -= 4;
	}
	return t->len - count;
}

int avalanche_spi_bitbang_setup_transfer( struct spi_device *spi, 
										  struct spi_transfer *t )
{
	struct spi_bitbang_cs *cs = spi->controller_state;
	u8 bits_per_word;
	u32 hz;

	if (t) {
		bits_per_word = t->bits_per_word;
		hz = t->speed_hz;
	} else {
		bits_per_word = 0;
		hz = 0;
	}

	/* spi_transfer level calls that work per-word */
	if (!bits_per_word)
		bits_per_word = spi->bits_per_word;
	if (bits_per_word <= 8)
		cs->txrx_bufs = bitbang_txrx_8;
	else if (bits_per_word <= 16)
		cs->txrx_bufs = bitbang_txrx_16;
	else if (bits_per_word <= 32)
		cs->txrx_bufs = bitbang_txrx_32;
	else
		return -EINVAL;

	/* nsecs = (clock period)/2 */
	if (!hz)
		hz = spi->max_speed_hz;
	if (hz) {
		cs->nsecs = (1000000000/2) / hz;
		if (cs->nsecs > (MAX_UDELAY_MS * 1000 * 1000))
			return -EINVAL;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(avalanche_spi_bitbang_setup_transfer);

/**
 * avalanche_spi_bitbang_setup - default setup for per-word I/O loops
 */
int avalanche_spi_bitbang_setup(struct spi_device *spi)
{
	struct spi_bitbang_cs *cs = spi->controller_state;
	struct spi_bitbang *bitbang;
	int retval;

	bitbang = spi_master_get_devdata(spi->master);

	/* REVISIT: some systems will want to support devices using lsb-first
	 * bit encodings on the wire.  In pure software that would be trivial,
	 * just bitbang_txrx_le_cphaX() routines shifting the other way, and
	 * some hardware controllers also have this support.
	 */
	if ((spi->mode & SPI_LSB_FIRST) != 0)
		return -EINVAL;

	if (!cs) {
		cs = kzalloc(sizeof *cs, SLAB_KERNEL);
		if (!cs)
			return -ENOMEM;
		spi->controller_state = cs;
	}

	if (!spi->bits_per_word)
		spi->bits_per_word = 8;

	/* per-word shift register access, in hardware or bitbanging */
	cs->txrx_word = bitbang->txrx_word[spi->mode & (SPI_CPOL|SPI_CPHA)];
	if (!cs->txrx_word)
		return -EINVAL;

	retval = avalanche_spi_bitbang_setup_transfer(spi, NULL);
	if (retval < 0)
		return retval;

	dev_dbg(&spi->dev, "%s, mode %d, %u bits/w, %u nsec/bit\n",
			__FUNCTION__, spi->mode & (SPI_CPOL | SPI_CPHA),
			spi->bits_per_word, 2 * cs->nsecs);

	/* NOTE we _need_ to call chipselect() early, ideally with adapter
	 * setup, unless the hardware defaults cooperate to avoid confusion
	 * between normal (active low) and inverted chipselects.
	 */

	/* deselect chip (low or high) */
	bitbang->chipselect(spi, BITBANG_CS_INACTIVE);
	ndelay(cs->nsecs);

	return 0;
}
EXPORT_SYMBOL_GPL(avalanche_spi_bitbang_setup);

/**
 * avalanche_spi_bitbang_cleanup - default cleanup for per-word I/O loops
 */
void avalanche_spi_bitbang_cleanup(struct spi_device *spi)
{
	kfree(spi->controller_state);
}
EXPORT_SYMBOL_GPL(avalanche_spi_bitbang_cleanup);

static int avalanche_spi_bitbang_bufs( struct spi_device *spi, 
                                       struct spi_transfer *t )
{
	struct spi_bitbang_cs *cs = spi->controller_state;
	unsigned nsecs = cs->nsecs;

	return cs->txrx_bufs(spi, cs->txrx_word, nsecs, t);
}

/*----------------------------------------------------------------------*/

/*
 * SECOND PART ... simple transfer queue runner.
 *
 * This costs a task context per controller, running the queue by
 * performing each transfer in sequence.  Smarter hardware can queue
 * several DMA transfers at once, and process several controller queues
 * in parallel; this driver doesn't match such hardware very well.
 *
 * Drivers can provide word-at-a-time i/o primitives, or provide
 * transfer-at-a-time ones to leverage dma or fifo hardware.
 */
static int avalanche_spi_bitbang_transfer_one_message(struct spi_master *master,
						       struct spi_message *m)
{
	struct spi_bitbang	*bitbang = spi_master_get_devdata(master);
	struct spi_message	*new_m = NULL;
	struct spi_device	*spi = m->spi;
	struct spi_transfer	*t = NULL;
	struct spi_transfer	*new_t = NULL;
	struct spi_transfer	*temp_t = NULL, *prev_t = NULL;
	unsigned	major_flen, minor_flen;
	u8		bits_per_word;
	unsigned	len, cs_change;
	int		status;
	int (*setup_transfer)(struct spi_device *,struct spi_transfer *);
	struct avalanche_sfi_dev_info_t *sfi =
			(struct avalanche_sfi_dev_info_t*)(spi->controller_data);

	cs_change = 1;
	status = 0;
	setup_transfer = NULL;

	major_flen = 0;

	//list_entry((&m->transfers)->next, typeof(*t_start), transfer_list);

	// create a new message
	new_m = kmalloc(sizeof(struct spi_message), GFP_KERNEL);
	if(new_m == NULL) {
		panic("[Avalanche SPI] Not Enough Memory\n");
		return;
	}

	spi_message_init(new_m);

	list_for_each_entry (new_t, &m->transfers, transfer_list) {

		if (bitbang->shutdown) {
			status = -ESHUTDOWN;
			break;
		}

		len = new_t->len;

		/* calculate minor FLEN for this spi_transfer based on
		*   bits_per_word
		*/

		bits_per_word = spi->bits_per_word;

		if(new_t->bits_per_word) {
			bits_per_word = new_t->bits_per_word;
		}

		if (bits_per_word <= 8 || bits_per_word > 32)
			minor_flen = len;
		else if(bits_per_word <= 16)
			minor_flen = len/2;
		else
			minor_flen = len/4;

		major_flen += minor_flen;


		if((major_flen > AVALANCHE_SPI_MAX_FLEN)) {
			if(sfi == NULL || sfi->mode != AVALANCHE_SFI_MODE) {
				printk("SPI frame length supplied is more than supported \
		    length\n");
				break;
			}
		}

		/* add this spi_transfer to our spi_message my_m */
		temp_t = NULL;
		temp_t = kmalloc(sizeof(struct spi_transfer), GFP_KERNEL);
		if(!temp_t) {
			panic("[SPI Bitbang] Kmalloc failed");
		}

		memcpy(temp_t, new_t, sizeof(struct spi_transfer));

		spi_message_add_tail(temp_t, new_m);

		if(!new_t->cs_change &&
		   (new_t->transfer_list.next != &m->transfers)) {
			continue;
		}

		/* program the major_flen in the command register with dummy
		 * transfer command
		 */

		if(bitbang->set_flen)
			bitbang->set_flen(spi, major_flen);

		/* for first time cs has to be deactivated */
		cs_change = 1;

		t = NULL;

		/* now trasfer the frame created by us */
		list_for_each_entry (t, &(new_m->transfers), transfer_list) {
			if(prev_t) {
				spi_transfer_del(prev_t);
				kfree(prev_t);
				prev_t = NULL;
			}

			/* override or restore speed and wordsize */
			if (t->speed_hz || t->bits_per_word) {
				setup_transfer = bitbang->setup_transfer;
				if (!setup_transfer) {
					status = -ENOPROTOOPT;
					SPI_DEBUG("No setup transfer installed\n");
					break;
				}
			}
			if (setup_transfer) {
				status = setup_transfer(spi, t);
				if (status < 0){
					SPI_DEBUG("Setup transfer failed \n");
					break;
				}
			}

			/* set up default clock polarity, and activate chip;
			 * this implicitly updates clock and spi modes as
			 * previously recorded for this device via setup().
			 * (and also deselects any other chip that might be
			 * selected ...)
			 */
			if (cs_change) {
				bitbang->chipselect(spi, BITBANG_CS_ACTIVE);
			}
			cs_change = t->cs_change;
			if (!t->tx_buf && !t->rx_buf && t->len) {
				status = -EINVAL;
				SPI_DEBUG("is somebody playing with me?\n");
				break;
			}

			/* transfer data.  the lower level code handles any
			 * new dma mappings it needs. our caller always gave
			 * us dma-safe buffers.
			 */
			if (t->len) {
				/* REVISIT dma API still needs a designated
				 * DMA_ADDR_INVALID; ~0 might be better.
				 */
				if (!m->is_dma_mapped)
					t->rx_dma = t->tx_dma = 0;
				status = bitbang->txrx_bufs(spi, t);
			}
			if (status != t->len) {
				if (status > 0)
					status = -EMSGSIZE;
				SPI_DEBUG("bufs not successful in transmitting data\n");
				break;
			}
			m->actual_length += status;
			status = 0;

			/* protocol tweaks before next transfer */
			if (t->delay_usecs)
				udelay(t->delay_usecs);

			prev_t = t;

			if (!cs_change) {
				continue;
			}


			if (t->transfer_list.next == &new_m->transfers) {
				break;
			}

			/* sometimes a short mid-message deselect of the chip
			 * may be needed to terminate a mode or command
			 */
			bitbang->chipselect(spi, BITBANG_CS_INACTIVE);
			EXIT
		}

		if(prev_t) {
			spi_transfer_del(prev_t);
			kfree(prev_t);
			prev_t = NULL;
		}
		else if(t) {
			spi_transfer_del(t);
			kfree(t);
			t = NULL;
		}

		// Re-initialize our spi_message
		spi_message_init(new_m);

		if (new_t->transfer_list.next == &m->transfers)
		{
			break;
		}
	}


	// free newly created message
	spi_message_free(new_m);

	m->status = status;

	/* restore speed and wordsize */
	if (setup_transfer)
		setup_transfer(spi, NULL);

	/* normally deactivate chipselect ... unless no error and
	 * cs_change has hinted that the next message will probably
	 * be for this chip too.
	 */
	if (!(status == 0 && cs_change)) {
		bitbang->chipselect(spi, BITBANG_CS_INACTIVE);
	}

	spi_finalize_current_message(master);

	return 0;
}


/**
 * avalanche_spi_bitbang_transfer - default submit to transfer queue
 */
/*
int avalanche_spi_bitbang_transfer( struct spi_device *spi,
                                    struct spi_message *m )
{
	struct spi_bitbang	*bitbang;
	unsigned long		flags;
	int			status = 0;

	ENTER
	m->actual_length = 0;
	m->status = -EINPROGRESS;

	bitbang = spi_master_get_devdata(spi->master);
	if (bitbang->shutdown)
		return -ESHUTDOWN;

	spin_lock_irqsave(&bitbang->lock, flags);
	if (!spi->max_speed_hz)
		status = -ENETDOWN;
	else {
		list_add_tail(&m->queue, &bitbang->queue);
		wake_up_process(bitbang->task);
	}
	spin_unlock_irqrestore(&bitbang->lock, flags);

	EXIT
	return status;
}
EXPORT_SYMBOL_GPL(avalanche_spi_bitbang_transfer);
*/

/**                                                                                                
 * avalanche_spi_bitbang_thread                                                                    
 * @data: Pointer to structure spi_bitbang, passed as a void pointer.                              
 *                                                                                                 
 * This thread invokes avalanche_bitbang_work to process the queue                                 
 * of SPI transfers, and goes back to sleep until a new transfer is queued,                        
 * or when requested to stop.                                                                      
 */                                                                                                
/*static int avalanche_spi_bitbang_thread(void* data)
{                                                                                                  
	   struct spi_bitbang *bitbang = (struct spi_bitbang*)data;                                    
																								   
	   while (!kthread_should_stop())                                                              
	   {                                                                                           
			   unsigned long flags;                                                                
			   spin_lock_irqsave(&bitbang->lock, flags);                                           
			   if (list_empty(&bitbang->queue) && !kthread_should_stop())                          
			   {                                                                                   
					   set_current_state(TASK_INTERRUPTIBLE);                                      
					   spin_unlock_irqrestore(&bitbang->lock, flags);                              
					   schedule();                                                                 
					   spin_lock_irqsave(&bitbang->lock, flags);                                   
			   }                                                                                   
			   spin_unlock_irqrestore(&bitbang->lock, flags);                                      
			   avalanche_bitbang_work(bitbang);                                                    
	   }                                                                                           
	   return 0;                                                                                   
}*/
																								   
/*----------------------------------------------------------------------*/

/**
 * avalanche_spi_bitbang_start - start up a polled/bitbanging SPI master driver
 * @bitbang: driver handle
 *
 * Caller should have zero-initialized all parts of the structure, and then
 * provided callbacks for chip selection and I/O loops.  If the master has
 * a transfer method, its final step should call spi_bitbang_transfer; or,
 * that's the default if the transfer routine is not initialized.  It should
 * also set up the bus number and number of chipselects.
 *
 * For i/o loops, provide callbacks either per-word (for bitbanging, or for
 * hardware that basically exposes a shift register) or per-spi_transfer
 * (which takes better advantage of hardware like fifos or DMA engines).
 *
 * Drivers using per-word I/O loops should use (or call) spi_bitbang_setup and
 * spi_bitbang_cleanup to handle those spi master methods.  Those methods are
 * the defaults if the bitbang->txrx_bufs routine isn't initialized.
 *
 * This routine registers the spi_master, which will process requests in a
 * dedicated task, keeping IRQs unblocked most of the time.  To stop
 * processing those requests, call spi_bitbang_stop().
 */
int avalanche_spi_bitbang_start(struct spi_bitbang *bitbang)
{
	if (!bitbang->master || !bitbang->chipselect)
		return -EINVAL;

	if (!bitbang->master->transfer_one_message)
		bitbang->master->transfer_one_message = avalanche_spi_bitbang_transfer_one_message;
	if (!bitbang->txrx_bufs) {
		bitbang->use_dma = 0;
		bitbang->txrx_bufs = avalanche_spi_bitbang_bufs;
		if (!bitbang->master->setup) {
			if (!bitbang->setup_transfer)
				bitbang->setup_transfer =
					 avalanche_spi_bitbang_setup_transfer;
			bitbang->master->setup = avalanche_spi_bitbang_setup;
			bitbang->master->cleanup = avalanche_spi_bitbang_cleanup;
		}
	} else if (!bitbang->master->setup)
		return -EINVAL;

	/* this task is the only thing to touch the SPI bits */
	SPI_DEBUG("dev_name = %s\n", dev_name(bitbang->master->dev.parent));

	return spi_register_master(bitbang->master);
}
EXPORT_SYMBOL_GPL(avalanche_spi_bitbang_start);

/**
 * spi_bitbang_stop - stops the task providing spi communication
 */
int avalanche_spi_bitbang_stop(struct spi_bitbang *bitbang)
{
	bitbang->shutdown = 0;
	spi_unregister_master(bitbang->master);

	return 0;
}
EXPORT_SYMBOL_GPL(avalanche_spi_bitbang_stop);
MODULE_LICENSE("GPL");

