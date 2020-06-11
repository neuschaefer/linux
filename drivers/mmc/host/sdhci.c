/*
 *  linux/drivers/mmc/host/sdhci.c - Secure Digital Host Controller Interface driver
 *
 *  Copyright (C) 2005-2007 Pierre Ossman, All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * Thanks to the following companies for their support:
 *
 *     - JMicron (hardware and technical support)
 */

#include <linux/delay.h>
#include <linux/highmem.h>
#include <linux/pci.h>
#include <linux/dma-mapping.h>
#include <linux/scatterlist.h>
#include <linux/platform_device.h>

#include <linux/mmc/host.h>

#include "sdhci.h"

#define DRIVER_NAME "sdhci"

#define DBG(f, x...) \
	pr_debug(DRIVER_NAME " [%s()]: " f, __func__,## x)

static unsigned int debug_quirks = 0;

/*
 * Different quirks to handle when the hardware deviates from a strict
 * interpretation of the SDHCI specification.
 */

/* Controller doesn't honor resets unless we touch the clock register */
#define SDHCI_QUIRK_CLOCK_BEFORE_RESET			(1<<0)
/* Controller has bad caps bits, but really supports DMA */
#define SDHCI_QUIRK_FORCE_DMA				(1<<1)
/* Controller doesn't like some resets when there is no card inserted. */
#define SDHCI_QUIRK_NO_CARD_NO_RESET			(1<<2)
/* Controller doesn't like clearing the power reg before a change */
#define SDHCI_QUIRK_SINGLE_POWER_WRITE			(1<<3)
/* Controller has flaky internal state so reset it on each ios change */
#define SDHCI_QUIRK_RESET_CMD_DATA_ON_IOS		(1<<4)
/* Controller has an unusable DMA engine */
#define SDHCI_QUIRK_BROKEN_DMA				(1<<5)
/* Controller can only DMA from 32-bit aligned addresses */
#define SDHCI_QUIRK_32BIT_DMA_ADDR			(1<<6)
/* Controller can only DMA chunk sizes that are a multiple of 32 bits */
#define SDHCI_QUIRK_32BIT_DMA_SIZE			(1<<7)
/* Controller needs to be reset after each request to stay stable */
#define SDHCI_QUIRK_RESET_AFTER_REQUEST			(1<<8)

static const struct pci_device_id pci_ids[] __devinitdata = {
	{
		.vendor		= PCI_VENDOR_ID_RICOH,
		.device		= PCI_DEVICE_ID_RICOH_R5C822,
		.subvendor	= PCI_VENDOR_ID_IBM,
		.subdevice	= PCI_ANY_ID,
		.driver_data	= SDHCI_QUIRK_CLOCK_BEFORE_RESET |
				  SDHCI_QUIRK_FORCE_DMA,
	},

	{
		.vendor		= PCI_VENDOR_ID_RICOH,
		.device		= PCI_DEVICE_ID_RICOH_R5C822,
		.subvendor	= PCI_ANY_ID,
		.subdevice	= PCI_ANY_ID,
		.driver_data	= SDHCI_QUIRK_FORCE_DMA |
				  SDHCI_QUIRK_NO_CARD_NO_RESET,
	},

	{
		.vendor		= PCI_VENDOR_ID_TI,
		.device		= PCI_DEVICE_ID_TI_XX21_XX11_SD,
		.subvendor	= PCI_ANY_ID,
		.subdevice	= PCI_ANY_ID,
		.driver_data	= SDHCI_QUIRK_FORCE_DMA,
	},

	{
		.vendor		= PCI_VENDOR_ID_ENE,
		.device		= PCI_DEVICE_ID_ENE_CB712_SD,
		.subvendor	= PCI_ANY_ID,
		.subdevice	= PCI_ANY_ID,
		.driver_data	= SDHCI_QUIRK_SINGLE_POWER_WRITE |
				  SDHCI_QUIRK_BROKEN_DMA,
	},

	{
		.vendor		= PCI_VENDOR_ID_ENE,
		.device		= PCI_DEVICE_ID_ENE_CB712_SD_2,
		.subvendor	= PCI_ANY_ID,
		.subdevice	= PCI_ANY_ID,
		.driver_data	= SDHCI_QUIRK_SINGLE_POWER_WRITE |
				  SDHCI_QUIRK_BROKEN_DMA,
	},

	{
		.vendor         = PCI_VENDOR_ID_ENE,
		.device         = PCI_DEVICE_ID_ENE_CB714_SD,
		.subvendor      = PCI_ANY_ID,
		.subdevice      = PCI_ANY_ID,
		.driver_data    = SDHCI_QUIRK_SINGLE_POWER_WRITE |
				  SDHCI_QUIRK_RESET_CMD_DATA_ON_IOS,
	},

	{
		.vendor         = PCI_VENDOR_ID_ENE,
		.device         = PCI_DEVICE_ID_ENE_CB714_SD_2,
		.subvendor      = PCI_ANY_ID,
		.subdevice      = PCI_ANY_ID,
		.driver_data    = SDHCI_QUIRK_SINGLE_POWER_WRITE |
				  SDHCI_QUIRK_RESET_CMD_DATA_ON_IOS,
	},

	{
		.vendor         = PCI_VENDOR_ID_JMICRON,
		.device         = PCI_DEVICE_ID_JMICRON_JMB38X_SD,
		.subvendor      = PCI_ANY_ID,
		.subdevice      = PCI_ANY_ID,
		.driver_data    = SDHCI_QUIRK_32BIT_DMA_ADDR |
				  SDHCI_QUIRK_32BIT_DMA_SIZE |
				  SDHCI_QUIRK_RESET_AFTER_REQUEST,
	},

	{	/* Generic SD host controller */
		PCI_DEVICE_CLASS((PCI_CLASS_SYSTEM_SDHCI << 8), 0xFFFF00)
	},

	{ /* end: all zeroes */ },
};

MODULE_DEVICE_TABLE(pci, pci_ids);

static void sdhci_prepare_data(struct sdhci_host *, struct mmc_data *);
static void sdhci_finish_data(struct sdhci_host *);

static void sdhci_send_command(struct sdhci_host *, struct mmc_command *);
static void sdhci_finish_command(struct sdhci_host *);

static void sdhci_dumpregs(struct sdhci_host *host)
{
	printk(KERN_DEBUG DRIVER_NAME ": ============== REGISTER DUMP ==============\n");

	printk(KERN_DEBUG DRIVER_NAME ": Sys addr: 0x%08x | Version:  0x%08x\n",
		ioread32(host->ioaddr + SDHCI_DMA_ADDRESS),
		ioread16(host->ioaddr + SDHCI_HOST_VERSION));
	printk(KERN_DEBUG DRIVER_NAME ": Blk size: 0x%08x | Blk cnt:  0x%08x\n",
		ioread16(host->ioaddr + SDHCI_BLOCK_SIZE),
		ioread16(host->ioaddr + SDHCI_BLOCK_COUNT));
	printk(KERN_DEBUG DRIVER_NAME ": Argument: 0x%08x | Trn mode: 0x%08x\n",
		ioread32(host->ioaddr + SDHCI_ARGUMENT),
		ioread16(host->ioaddr + SDHCI_TRANSFER_MODE));
	printk(KERN_DEBUG DRIVER_NAME ": Present:  0x%08x | Host ctl: 0x%08x\n",
		ioread32(host->ioaddr + SDHCI_PRESENT_STATE),
		ioread8(host->ioaddr + SDHCI_HOST_CONTROL));
	printk(KERN_DEBUG DRIVER_NAME ": Power:    0x%08x | Blk gap:  0x%08x\n",
		ioread8(host->ioaddr + SDHCI_POWER_CONTROL),
		ioread8(host->ioaddr + SDHCI_BLOCK_GAP_CONTROL));
	printk(KERN_DEBUG DRIVER_NAME ": Wake-up:  0x%08x | Clock:    0x%08x\n",
		ioread8(host->ioaddr + SDHCI_WAKE_UP_CONTROL),
		ioread16(host->ioaddr + SDHCI_CLOCK_CONTROL));
	printk(KERN_DEBUG DRIVER_NAME ": Timeout:  0x%08x | Int stat: 0x%08x\n",
		ioread8(host->ioaddr + SDHCI_TIMEOUT_CONTROL),
		ioread32(host->ioaddr + SDHCI_INT_STATUS));
	printk(KERN_DEBUG DRIVER_NAME ": Int enab: 0x%08x | Sig enab: 0x%08x\n",
		ioread32(host->ioaddr + SDHCI_INT_ENABLE),
		ioread32(host->ioaddr + SDHCI_SIGNAL_ENABLE));
	printk(KERN_DEBUG DRIVER_NAME ": AC12 err: 0x%08x | Slot int: 0x%08x\n",
		ioread16(host->ioaddr + SDHCI_ACMD12_ERR),
		ioread16(host->ioaddr + SDHCI_SLOT_INT_STATUS));
	printk(KERN_DEBUG DRIVER_NAME ": Caps:     0x%08x | Max curr: 0x%08x\n",
		ioread32(host->ioaddr + SDHCI_CAPABILITIES),
		ioread32(host->ioaddr + SDHCI_MAX_CURRENT));

	printk(KERN_DEBUG DRIVER_NAME ": ===========================================\n");
}


/*****************************************************************************\
 *                                                                           *
 * Low level functions                                                       *
 *                                                                           *
\*****************************************************************************/
static void sdhci_reset(struct sdhci_host *host, u8 mask)
{
	unsigned long timeout;
	unsigned char val;

    
	if (host->chip->quirks & SDHCI_QUIRK_NO_CARD_NO_RESET) {
		printk("bleah\n");
		if (!(ioread32(host->ioaddr + SDHCI_PRESENT_STATE) &
			SDHCI_CARD_PRESENT))
			return;
	}

	iowrite8(mask, host->ioaddr + SDHCI_SOFTWARE_RESET);

	if (mask & SDHCI_RESET_ALL)
		host->clock = 0;

    
	/* Wait max 100 ms */
	timeout = 100;

	//AMI comment: note without this we get frequent lockup as soon as the write happens!!
	mdelay(1);


	/* hw clears the bit when it's done */


	val = ioread8((host->ioaddr +SDHCI_SOFTWARE_RESET));
	while (val & mask) {
		printk("not set\n");
		if (timeout == 0) {
			printk(KERN_ERR "%s: Reset 0x%x never completed.\n",
				mmc_hostname(host->mmc), (int)mask);
			sdhci_dumpregs(host);
			return;
		}
		timeout--;
		mdelay(1);
	}
}



static void sdhci_init(struct sdhci_host *host)
{
	u32 intmask;

    
	sdhci_reset(host, SDHCI_RESET_ALL);

	intmask = SDHCI_INT_BUS_POWER | SDHCI_INT_DATA_END_BIT |
		SDHCI_INT_DATA_CRC | SDHCI_INT_DATA_TIMEOUT | SDHCI_INT_INDEX |
		SDHCI_INT_END_BIT | SDHCI_INT_CRC | SDHCI_INT_TIMEOUT |
		SDHCI_INT_CARD_REMOVE | SDHCI_INT_CARD_INSERT |
		SDHCI_INT_DATA_AVAIL | SDHCI_INT_SPACE_AVAIL |
		SDHCI_INT_DMA_END | SDHCI_INT_DATA_END | SDHCI_INT_RESPONSE;

	iowrite32(intmask, host->ioaddr + SDHCI_INT_ENABLE);
	iowrite32(intmask, host->ioaddr + SDHCI_SIGNAL_ENABLE);
}


static void sdhci_activate_led(struct sdhci_host *host)
{
	u8 ctrl;

    
	ctrl = ioread8(host->ioaddr + SDHCI_HOST_CONTROL);
	ctrl |= SDHCI_CTRL_LED;
	iowrite8(ctrl, host->ioaddr + SDHCI_HOST_CONTROL);
}

static void sdhci_deactivate_led(struct sdhci_host *host)
{
	u8 ctrl;

    
	ctrl = ioread8(host->ioaddr + SDHCI_HOST_CONTROL);
	ctrl &= ~SDHCI_CTRL_LED;
	iowrite8(ctrl, host->ioaddr + SDHCI_HOST_CONTROL);
}

/*****************************************************************************\
 *                                                                           *
 * Core functions                                                            *
 *                                                                           *
\*****************************************************************************/

static inline char* sdhci_sg_to_buffer(struct sdhci_host* host)
{

	return sg_virt(host->cur_sg);
}

static inline int sdhci_next_sg(struct sdhci_host* host)
{
	/*
	 * Skip to next SG entry.
	 */
	host->cur_sg++;
	host->num_sg--;

	/*
	 * Any entries left?
	 */
	if (host->num_sg > 0) {
		host->offset = 0;
		host->remain = host->cur_sg->length;
	}

	return host->num_sg;
}

static void sdhci_read_block_pio(struct sdhci_host *host)
{
	int blksize, chunk_remain;
	u32 data;
	char *buffer;
	int size;

    
	DBG("PIO reading\n");

	blksize = host->data->blksz;
	chunk_remain = 0;
	data = 0;

	buffer = sdhci_sg_to_buffer(host) + host->offset;

	while (blksize) {
		if (chunk_remain == 0) {
			data = ioread32(host->ioaddr + SDHCI_BUFFER);
			chunk_remain = min(blksize, 4);
		}

		size = min(host->remain, chunk_remain);

		chunk_remain -= size;
		blksize -= size;
		host->offset += size;
		host->remain -= size;

		while (size) {
			*buffer = data & 0xFF;
			buffer++;
			data >>= 8;
			size--;
		}

		if (host->remain == 0) {
			if (sdhci_next_sg(host) == 0) {
				BUG_ON(blksize != 0);
				return;
			}
			buffer = sdhci_sg_to_buffer(host);
		}
	}
}

static void sdhci_write_block_pio(struct sdhci_host *host)
{
	int blksize, chunk_remain;
	u32 data;
	char *buffer;
	int bytes, size;
	int wrcount = 0;

	DBG("PIO writing\n");
	

	blksize = host->data->blksz;
	chunk_remain = 4;
	data = 0;

	bytes = 0;
	buffer = sdhci_sg_to_buffer(host) + host->offset;

	while (blksize) {
		size = min(host->remain, chunk_remain);

		chunk_remain -= size;
		blksize -= size;
		host->offset += size;
		host->remain -= size;

		while (size) {
			data >>= 8;
			data |= (u32)*buffer << 24;
			buffer++;
			size--;
		}

		if (chunk_remain == 0) {
			iowrite32(data, host->ioaddr + SDHCI_BUFFER);
			wrcount+=4;
			chunk_remain = min(blksize, 4);
		}

		if (host->remain == 0) {
			if (sdhci_next_sg(host) == 0) {
				BUG_ON(blksize != 0);
				return;
			}
			buffer = sdhci_sg_to_buffer(host);
		}
	}

    
}

static void sdhci_transfer_pio(struct sdhci_host *host)
{
	u32 mask;
	
	static int data_avail_wait = 0;
	static int last_blk_prob_ct = 0;

	BUG_ON(!host->data);
	

	if (host->num_sg == 0)
		return;

	if (host->data->flags & MMC_DATA_READ)
		mask = SDHCI_DATA_AVAILABLE;
	else
		mask = SDHCI_SPACE_AVAILABLE;

//	DBG("prst:0x%x\n",ioread32(host->ioaddr + SDHCI_PRESENT_STATE));
	while (ioread32(host->ioaddr + SDHCI_PRESENT_STATE) & mask) {
		
		if (host->data->flags & MMC_DATA_READ)
			sdhci_read_block_pio(host);
		else
			sdhci_write_block_pio(host);

		DBG("num sg is %x\n",host->num_sg);
		if (host->num_sg == 0)
		{
			DBG("all done\n");
			break;
        }
	}

	if(host->num_sg != 0 && !(host->data->flags & MMC_DATA_READ) )
	{
		int retryct = 100;
		DBG("Got a write case where num sg is not yet 0 (it is %d) but buffer space indicates not available\n",host->num_sg);
		DBG("doing 100 retries to see if buffer space clears\n");
		while(retryct > 0)
		{
			if(ioread32(host->ioaddr + SDHCI_PRESENT_STATE) & mask)
			{
				DBG("Cleared!!!\n");
				break;
			}
			retryct--;
		}
		if(retryct == 0)
		{
			DBG("never cleared!!\n");
		}
	}

	//DBG("PIO transfer says complete and num sg is %x. remaining is %d\n",host->num_sg,host->remain);
	#if 0
	if(host->num_sg == 1 && host->remain == 512 && !(host->data->flags & MMC_DATA_READ))
	{
		//we got transfer complete eventhough present state says buffer write enable is 0
		DBG("exactly one block left but host doesnt want to set space avail!!\n");
		last_blk_prob_ct++;
		if(last_blk_prob_ct == 2)
		{
			int retryct=100;
			DBG("ok we waited for the flag to clear..and we got two interrupts for the last block..so now we write\n");
			printk("Waiting for space avail to clear for 100 iterations\n");
			//THIS IS WRONG!!
			while (ioread32(host->ioaddr + SDHCI_PRESENT_STATE) & mask) 
			{
				retryct--;
				if(retryct == 0) break;
			}
			if(retryct == 0)
			{
				printk("bleah!!! flag did not clear\n");
			}
			else
			{
				printk("ah it did clear retry count is %d\n",retryct);
				sdhci_write_block_pio(host);

			}

			//sdhci_write_block_pio(host);
			last_blk_prob_ct = 0;

		}
      
		
		
	}
	#endif
}



static void sdhci_prepare_data(struct sdhci_host *host, struct mmc_data *data)
{
	u8 count;
	unsigned target_timeout, current_timeout;

    
	WARN_ON(host->data);

	if (data == NULL)
	{
		
		return;
	}
		

	/* Sanity checks */
	BUG_ON(data->blksz * data->blocks > 524288);
	BUG_ON(data->blksz > host->mmc->max_blk_size);
	BUG_ON(data->blocks > 65535);

	host->data = data;
	host->data_early = 0;

	DBG("prepare data block size is %d and block count is %d\n",data->blksz,data->blocks);

	/* timeout in us */
	target_timeout = data->timeout_ns / 1000 +
		data->timeout_clks / host->clock;

	/*
	 * Figure out needed cycles.
	 * We do this in steps in order to fit inside a 32 bit int.
	 * The first step is the minimum timeout, which will have a
	 * minimum resolution of 6 bits:
	 * (1) 2^13*1000 > 2^22,
	 * (2) host->timeout_clk < 2^16
	 *     =>
	 *     (1) / (2) > 2^6
	 */
	count = 0;
	current_timeout = (1 << 13) * 1000 / host->timeout_clk;
	while (current_timeout < target_timeout) {
		count++;
		current_timeout <<= 1;
		if (count >= 0xF)
			break;
	}

	if (count >= 0xF) {
		printk(KERN_WARNING "%s: Too large timeout requested!\n",
			mmc_hostname(host->mmc));
		count = 0xE;
	}

	iowrite8(count, host->ioaddr + SDHCI_TIMEOUT_CONTROL);

	if (host->flags & SDHCI_USE_DMA)
		host->flags |= SDHCI_REQ_USE_DMA;

	if (unlikely((host->flags & SDHCI_REQ_USE_DMA) &&
		(host->chip->quirks & SDHCI_QUIRK_32BIT_DMA_SIZE) &&
		((data->blksz * data->blocks) & 0x3))) {
		DBG("Reverting to PIO because of transfer size (%d)\n",
			data->blksz * data->blocks);
		host->flags &= ~SDHCI_REQ_USE_DMA;
	}

	/*
	 * The assumption here being that alignment is the same after
	 * translation to device address space.
	 */
	if (unlikely((host->flags & SDHCI_REQ_USE_DMA) &&
		(host->chip->quirks & SDHCI_QUIRK_32BIT_DMA_ADDR) &&
		(data->sg->offset & 0x3))) {
		DBG("Reverting to PIO because of bad alignment\n");
		host->flags &= ~SDHCI_REQ_USE_DMA;
	}

	if (host->flags & SDHCI_REQ_USE_DMA) {
		int count;

		count = dma_map_sg(host->chip->pdev, data->sg, data->sg_len,
			(data->flags & MMC_DATA_READ)?DMA_FROM_DEVICE:DMA_TO_DEVICE);
		BUG_ON(count != 1);

		iowrite32(sg_dma_address(data->sg), host->ioaddr + SDHCI_DMA_ADDRESS);
	} else {
		host->cur_sg = data->sg;
		host->num_sg = data->sg_len;

		host->offset = 0;
		host->remain = host->cur_sg->length;
	}

	/* We do not handle DMA boundaries, so set it to max (512 KiB) */
	iowrite16(SDHCI_MAKE_BLKSZ(7, data->blksz),
		host->ioaddr + SDHCI_BLOCK_SIZE);
	iowrite16(data->blocks, host->ioaddr + SDHCI_BLOCK_COUNT);

	
}

static void sdhci_set_transfer_mode(struct sdhci_host *host,
	struct mmc_data *data)
{
	u16 mode;

    
	if (data == NULL)
		return;

	WARN_ON(!host->data);

	mode = SDHCI_TRNS_BLK_CNT_EN;
	if (data->blocks > 1)
		mode |= SDHCI_TRNS_MULTI;
	if (data->flags & MMC_DATA_READ)
		mode |= SDHCI_TRNS_READ;
	if (host->flags & SDHCI_REQ_USE_DMA)
		mode |= SDHCI_TRNS_DMA;

	iowrite16(mode, host->ioaddr + SDHCI_TRANSFER_MODE);
}

static void sdhci_finish_data(struct sdhci_host *host)
{
	struct mmc_data *data;
	u16 blocks;

    
	BUG_ON(!host->data);

	data = host->data;
	host->data = NULL;

	if (host->flags & SDHCI_REQ_USE_DMA) {
		dma_unmap_sg(host->chip->pdev, data->sg, data->sg_len,
			(data->flags & MMC_DATA_READ)?DMA_FROM_DEVICE:DMA_TO_DEVICE);
	}

	/*
	 * Controller doesn't count down when in single block mode.
	 */
	if (data->blocks == 1)
		blocks = (data->error == 0) ? 0 : 1;
	else
		blocks = ioread16(host->ioaddr + SDHCI_BLOCK_COUNT);
	data->bytes_xfered = data->blksz * (data->blocks - blocks);

	if (!data->error && blocks) {
		printk(KERN_ERR "%s: Controller signalled completion even "
			"though there were blocks left.\n",
			mmc_hostname(host->mmc));
		data->error = -EIO;
	}

	if (data->stop) {
		/*
		 * The controller needs a reset of internal state machines
		 * upon error conditions.
		 */
		if (data->error) {
			sdhci_reset(host, SDHCI_RESET_CMD);
			sdhci_reset(host, SDHCI_RESET_DATA);
		}

		sdhci_send_command(host, data->stop);
	} else
		tasklet_schedule(&host->finish_tasklet);
}

static void sdhci_send_command(struct sdhci_host *host, struct mmc_command *cmd)
{
	int flags;
	u32 mask;
	unsigned long timeout;

	
	WARN_ON(host->cmd);

	/* Wait max 250 ms */
	timeout = 250; //250 ms seems to do the magic here instead of the original 10ms for waiting for inhibit bits

	mask = SDHCI_CMD_INHIBIT;
	if ((cmd->data != NULL) || (cmd->flags & MMC_RSP_BUSY))
		mask |= SDHCI_DATA_INHIBIT;

	/* We shouldn't wait for data inihibit for stop commands, even
	   though they might use busy signaling */
	if (host->mrq->data && (cmd == host->mrq->data->stop))
		mask &= ~SDHCI_DATA_INHIBIT;

	while (ioread32(host->ioaddr + SDHCI_PRESENT_STATE) & mask) {
		if (timeout == 0) {
			printk(KERN_ERR "%s: Controller never released "
				"inhibit bit(s).\n", mmc_hostname(host->mmc));
			sdhci_dumpregs(host);
			cmd->error = -EIO;
			tasklet_schedule(&host->finish_tasklet);
			return;
		}
		timeout--;
		mdelay(1);
	}

	mod_timer(&host->timer, jiffies + 10 * HZ);

	host->cmd = cmd;

	sdhci_prepare_data(host, cmd->data);

	iowrite32(cmd->arg, host->ioaddr + SDHCI_ARGUMENT);

	sdhci_set_transfer_mode(host, cmd->data);

	if ((cmd->flags & MMC_RSP_136) && (cmd->flags & MMC_RSP_BUSY)) {
		printk(KERN_ERR "%s: Unsupported response type!\n",
			mmc_hostname(host->mmc));
		cmd->error = -EINVAL;
		tasklet_schedule(&host->finish_tasklet);
		return;
	}

	if (!(cmd->flags & MMC_RSP_PRESENT))
		flags = SDHCI_CMD_RESP_NONE;
	else if (cmd->flags & MMC_RSP_136)
		flags = SDHCI_CMD_RESP_LONG;
	else if (cmd->flags & MMC_RSP_BUSY)
		flags = SDHCI_CMD_RESP_SHORT_BUSY;
	else
		flags = SDHCI_CMD_RESP_SHORT;

	if (cmd->flags & MMC_RSP_CRC)
		flags |= SDHCI_CMD_CRC;
	if (cmd->flags & MMC_RSP_OPCODE)
		flags |= SDHCI_CMD_INDEX;
	if (cmd->data)
		flags |= SDHCI_CMD_DATA;

	iowrite16(SDHCI_MAKE_CMD(cmd->opcode, flags),
		host->ioaddr + SDHCI_COMMAND);
}


static void sdhci_finish_command(struct sdhci_host *host)
{
	int i;

	BUG_ON(host->cmd == NULL);

    
	if (host->cmd->flags & MMC_RSP_PRESENT) {
		if (host->cmd->flags & MMC_RSP_136) {
			/* CRC is stripped so we need to do some shifting. */
			for (i = 0;i < 4;i++) {
				host->cmd->resp[i] = ioread32(host->ioaddr +
					SDHCI_RESPONSE + (3-i)*4) << 8;
				if (i != 3)
					host->cmd->resp[i] |=
						ioread8(host->ioaddr +
						SDHCI_RESPONSE + (3-i)*4-1);
			}
		} else {
			host->cmd->resp[0] = ioread32(host->ioaddr + SDHCI_RESPONSE);
		}
	}

	host->cmd->error = 0;

	if (host->data && host->data_early)
		sdhci_finish_data(host);

	if (!host->cmd->data)
		tasklet_schedule(&host->finish_tasklet);

	host->cmd = NULL;
}

static void sdhci_set_clock(struct sdhci_host *host, unsigned int clock)
{
	int div;
	u16 clk;
	unsigned long timeout;

	if (clock == host->clock)
		return;

	iowrite16(0, host->ioaddr + SDHCI_CLOCK_CONTROL);

	if (clock == 0)
		goto out;

	for (div = 1;div < 256;div *= 2) {
		if ((host->max_clk / div) <= clock)
			break;
	}
	div >>= 1;

	clk = div << SDHCI_DIVIDER_SHIFT;
	clk |= SDHCI_CLOCK_INT_EN;
	iowrite16(clk, host->ioaddr + SDHCI_CLOCK_CONTROL);

	/* Wait max 10 ms */
	timeout = 10;
	while (!((clk = ioread16(host->ioaddr + SDHCI_CLOCK_CONTROL))
		& SDHCI_CLOCK_INT_STABLE)) {
		if (timeout == 0) {
			printk(KERN_ERR "%s: Internal clock never "
				"stabilised.\n", mmc_hostname(host->mmc));
			sdhci_dumpregs(host);
			return;
		}
		timeout--;
		mdelay(1);
	}

	clk |= SDHCI_CLOCK_CARD_EN;
	iowrite16(clk, host->ioaddr + SDHCI_CLOCK_CONTROL);

out:
	host->clock = clock;
}

static void sdhci_set_power(struct sdhci_host *host, unsigned short power)
{
	u8 pwr;

    
	if (host->power == power)
		return;

	if (power == (unsigned short)-1) {
		iowrite8(0, host->ioaddr + SDHCI_POWER_CONTROL);
		goto out;
	}

	/*
	 * Spec says that we should clear the power reg before setting
	 * a new value. Some controllers don't seem to like this though.
	 */
	if (!(host->chip->quirks & SDHCI_QUIRK_SINGLE_POWER_WRITE))
		iowrite8(0, host->ioaddr + SDHCI_POWER_CONTROL);

	pwr = SDHCI_POWER_ON;

	switch (1 << power) {
	case MMC_VDD_165_195:
		pwr |= SDHCI_POWER_180;
		break;
	case MMC_VDD_29_30:
	case MMC_VDD_30_31:
		pwr |= SDHCI_POWER_300;
		break;
	case MMC_VDD_32_33:
	case MMC_VDD_33_34:
		pwr |= SDHCI_POWER_330;
		break;
	default:
		BUG();
	}

	iowrite8(pwr, host->ioaddr + SDHCI_POWER_CONTROL);

out:
	host->power = power;
}

/*****************************************************************************\
 *                                                                           *
 * MMC callbacks                                                             *
 *                                                                           *
\*****************************************************************************/

static void sdhci_request(struct mmc_host *mmc, struct mmc_request *mrq)
{
	struct sdhci_host *host;
	unsigned long flags;

	
	host = mmc_priv(mmc);

	spin_lock_irqsave(&host->lock, flags);

	WARN_ON(host->mrq != NULL);

	sdhci_activate_led(host);

	host->mrq = mrq;

	if (!(ioread32(host->ioaddr + SDHCI_PRESENT_STATE) & SDHCI_CARD_PRESENT)) {
		host->mrq->cmd->error = -ENOMEDIUM;
		tasklet_schedule(&host->finish_tasklet);
	} else
		sdhci_send_command(host, mrq->cmd);

	mmiowb();
	spin_unlock_irqrestore(&host->lock, flags);
}

static void sdhci_set_ios(struct mmc_host *mmc, struct mmc_ios *ios)
{
	struct sdhci_host *host;
	unsigned long flags;
	u8 ctrl;

    
	host = mmc_priv(mmc);

	spin_lock_irqsave(&host->lock, flags);

	/*
	 * Reset the chip on each power off.
	 * Should clear out any weird states.
	 */
	if (ios->power_mode == MMC_POWER_OFF) {
		iowrite32(0, host->ioaddr + SDHCI_SIGNAL_ENABLE);
		sdhci_init(host);
	}

	sdhci_set_clock(host, ios->clock);

	if (ios->power_mode == MMC_POWER_OFF)
		sdhci_set_power(host, -1);
	else
		sdhci_set_power(host, ios->vdd);

	ctrl = ioread8(host->ioaddr + SDHCI_HOST_CONTROL);

	if (ios->bus_width == MMC_BUS_WIDTH_4)
		ctrl |= SDHCI_CTRL_4BITBUS;
	else
		ctrl &= ~SDHCI_CTRL_4BITBUS;

	if (ios->timing == MMC_TIMING_SD_HS)
		ctrl |= SDHCI_CTRL_HISPD;
	else
		ctrl &= ~SDHCI_CTRL_HISPD;

	iowrite8(ctrl, host->ioaddr + SDHCI_HOST_CONTROL);

	/*
	 * Some (ENE) controllers go apeshit on some ios operation,
	 * signalling timeout and CRC errors even on CMD0. Resetting
	 * it on each ios seems to solve the problem.
	 */
	if(host->chip->quirks & SDHCI_QUIRK_RESET_CMD_DATA_ON_IOS)
		sdhci_reset(host, SDHCI_RESET_CMD | SDHCI_RESET_DATA);

	mmiowb();
	spin_unlock_irqrestore(&host->lock, flags);
}



static int sdhci_get_ro(struct mmc_host *mmc)
{
	struct sdhci_host *host;
	unsigned long flags;
	int present;


	host = mmc_priv(mmc);

	spin_lock_irqsave(&host->lock, flags);

	present = ioread32(host->ioaddr + SDHCI_PRESENT_STATE);

	spin_unlock_irqrestore(&host->lock, flags);


	return !(present & SDHCI_WRITE_PROTECT);
}

static void sdhci_enable_sdio_irq(struct mmc_host *mmc, int enable)
{
	struct sdhci_host *host;
	unsigned long flags;
	u32 ier;

	

	host = mmc_priv(mmc);

	spin_lock_irqsave(&host->lock, flags);

	ier = ioread32(host->ioaddr + SDHCI_INT_ENABLE);

	ier &= ~SDHCI_INT_CARD_INT;
	if (enable)
		ier |= SDHCI_INT_CARD_INT;

	iowrite32(ier, host->ioaddr + SDHCI_INT_ENABLE);
	iowrite32(ier, host->ioaddr + SDHCI_SIGNAL_ENABLE);

	mmiowb();

	spin_unlock_irqrestore(&host->lock, flags);
}


static const struct mmc_host_ops sdhci_ops = {
	.request	= sdhci_request,
	.set_ios	= sdhci_set_ios,
	.get_ro		= sdhci_get_ro,
	.enable_sdio_irq = sdhci_enable_sdio_irq,
};

/*****************************************************************************\
 *                                                                           *
 * Tasklets                                                                  *
 *                                                                           *
\*****************************************************************************/

static void sdhci_tasklet_card(unsigned long param)
{
	struct sdhci_host *host;
	unsigned long flags;

    host = (struct sdhci_host*)param;

	spin_lock_irqsave(&host->lock, flags);

	if (!(ioread32(host->ioaddr + SDHCI_PRESENT_STATE) & SDHCI_CARD_PRESENT)) {
		if (host->mrq) {
			printk(KERN_ERR "%s: Card removed during transfer!\n",
				mmc_hostname(host->mmc));
			printk(KERN_ERR "%s: Resetting controller.\n",
				mmc_hostname(host->mmc));

			sdhci_reset(host, SDHCI_RESET_CMD);
			sdhci_reset(host, SDHCI_RESET_DATA);

			host->mrq->cmd->error = -ENOMEDIUM;
			tasklet_schedule(&host->finish_tasklet);
		}
	}

	spin_unlock_irqrestore(&host->lock, flags);

	mmc_detect_change(host->mmc, msecs_to_jiffies(500));
}

static void sdhci_tasklet_finish(unsigned long param)
{
	struct sdhci_host *host;
	unsigned long flags;
	struct mmc_request *mrq;

    
	host = (struct sdhci_host*)param;

	spin_lock_irqsave(&host->lock, flags);

	del_timer(&host->timer);

	mrq = host->mrq;

	/*
	 * The controller needs a reset of internal state machines
	 * upon error conditions.
	 */
	if (mrq->cmd->error ||
		(mrq->data && (mrq->data->error ||
		(mrq->data->stop && mrq->data->stop->error))) ||
		(host->chip->quirks & SDHCI_QUIRK_RESET_AFTER_REQUEST)) {

		/* Some controllers need this kick or reset won't work here */
		if (host->chip->quirks & SDHCI_QUIRK_CLOCK_BEFORE_RESET) {
			unsigned int clock;

			/* This is to force an update */
			clock = host->clock;
			host->clock = 0;
			sdhci_set_clock(host, clock);
		}

		/* Spec says we should do both at the same time, but Ricoh
		   controllers do not like that. */
		sdhci_reset(host, SDHCI_RESET_CMD);
		sdhci_reset(host, SDHCI_RESET_DATA);
	}

	host->mrq = NULL;
	host->cmd = NULL;
	host->data = NULL;

	sdhci_deactivate_led(host);

	mmiowb();
	spin_unlock_irqrestore(&host->lock, flags);

	mmc_request_done(host->mmc, mrq);
}

static void sdhci_timeout_timer(unsigned long data)
{
	struct sdhci_host *host;
	unsigned long flags;

	

	host = (struct sdhci_host*)data;

	spin_lock_irqsave(&host->lock, flags);

	if (host->mrq) {
		printk(KERN_ERR "%s: Timeout waiting for hardware "
			"interrupt.\n", mmc_hostname(host->mmc));
		sdhci_dumpregs(host);

		if (host->data) {
			host->data->error = -ETIMEDOUT;
			sdhci_finish_data(host);
		} else {
			if (host->cmd)
				host->cmd->error = -ETIMEDOUT;
			else
				host->mrq->cmd->error = -ETIMEDOUT;

			tasklet_schedule(&host->finish_tasklet);
		}
	}

	mmiowb();
	spin_unlock_irqrestore(&host->lock, flags);
}

/*****************************************************************************\
 *                                                                           *
 * Interrupt handling                                                        *
 *                                                                           *
\*****************************************************************************/

static void sdhci_cmd_irq(struct sdhci_host *host, u32 intmask)
{
	BUG_ON(intmask == 0);

	

	if (!host->cmd) {
		printk(KERN_ERR "%s: Got command interrupt 0x%08x even "
			"though no command operation was in progress.\n",
			mmc_hostname(host->mmc), (unsigned)intmask);
		sdhci_dumpregs(host);
		return;
	}

	if (intmask & SDHCI_INT_TIMEOUT)
		host->cmd->error = -ETIMEDOUT;
	else if (intmask & (SDHCI_INT_CRC | SDHCI_INT_END_BIT |
			SDHCI_INT_INDEX))
		host->cmd->error = -EILSEQ;

	if (host->cmd->error)
		tasklet_schedule(&host->finish_tasklet);
	else if (intmask & SDHCI_INT_RESPONSE)
		sdhci_finish_command(host);
}

static void sdhci_data_irq(struct sdhci_host *host, u32 intmask)
{
	BUG_ON(intmask == 0);

	

	if (!host->data) {
		/*
		 * A data end interrupt is sent together with the response
		 * for the stop command.
		 */
		if (intmask & SDHCI_INT_DATA_END)
			return;

		printk(KERN_ERR "%s: Got data interrupt 0x%08x even "
			"though no data operation was in progress.\n",
			mmc_hostname(host->mmc), (unsigned)intmask);
		sdhci_dumpregs(host);

		return;
	}

	if (intmask & SDHCI_INT_DATA_TIMEOUT)
		host->data->error = -ETIMEDOUT;
	else if (intmask & (SDHCI_INT_DATA_CRC | SDHCI_INT_DATA_END_BIT))
		host->data->error = -EILSEQ;

	if (host->data->error)
		sdhci_finish_data(host);
	else {
		if (intmask & (SDHCI_INT_DATA_AVAIL | SDHCI_INT_SPACE_AVAIL))
			sdhci_transfer_pio(host);

		/*
		 * We currently don't do anything fancy with DMA
		 * boundaries, but as we can't disable the feature
		 * we need to at least restart the transfer.
		 */
		if (intmask & SDHCI_INT_DMA_END)
			iowrite32(ioread32(host->ioaddr + SDHCI_DMA_ADDRESS),
				host->ioaddr + SDHCI_DMA_ADDRESS);

		if (intmask & SDHCI_INT_DATA_END) {
			if (host->cmd) {
				/*
				 * Data managed to finish before the
				 * command completed. Make sure we do
				 * things in the proper order.
				 */
				host->data_early = 1;
				printk("EARLy DATA!!\n");
			} else {
				sdhci_finish_data(host);
			}
		}
	}
}

static irqreturn_t sdhci_irq(int irq, void *dev_id)
{
	irqreturn_t result;
	struct sdhci_host* host = dev_id;
	u32 intmask;
	int cardint = 0;
	unsigned int gpio7stat;

    

	spin_lock(&host->lock);

	
	intmask = ioread32(host->ioaddr + SDHCI_INT_STATUS);
	while(intmask != 0)
	{
		
		
	
		//if (!intmask || intmask == 0xffffffff) {
			//result = IRQ_NONE;
			//printk("got stupid intmask:0x%x\n",intmask);
			//goto out;
		//}
	
		DBG("*** %s got interrupt: 0x%08x\n", host->slot_descr, intmask);
	
		if (intmask & (SDHCI_INT_CARD_INSERT | SDHCI_INT_CARD_REMOVE)) {
			iowrite32(intmask & (SDHCI_INT_CARD_INSERT | SDHCI_INT_CARD_REMOVE),
				host->ioaddr + SDHCI_INT_STATUS);
			tasklet_schedule(&host->card_tasklet);
		}
	
		intmask &= ~(SDHCI_INT_CARD_INSERT | SDHCI_INT_CARD_REMOVE);
	
		if (intmask & SDHCI_INT_CMD_MASK) {
			iowrite32(intmask & SDHCI_INT_CMD_MASK,
				host->ioaddr + SDHCI_INT_STATUS);
			sdhci_cmd_irq(host, intmask & SDHCI_INT_CMD_MASK);
		}
	
		if (intmask & SDHCI_INT_DATA_MASK) {
			iowrite32(intmask & SDHCI_INT_DATA_MASK,
				host->ioaddr + SDHCI_INT_STATUS);
			sdhci_data_irq(host, intmask & SDHCI_INT_DATA_MASK);
		}
	
		intmask &= ~(SDHCI_INT_CMD_MASK | SDHCI_INT_DATA_MASK);
	
		intmask &= ~SDHCI_INT_ERROR;
	
		if (intmask & SDHCI_INT_BUS_POWER) {
			printk(KERN_ERR "%s: Card is consuming too much power!\n",
				mmc_hostname(host->mmc));
			iowrite32(SDHCI_INT_BUS_POWER, host->ioaddr + SDHCI_INT_STATUS);
		}
	
		intmask &= ~SDHCI_INT_BUS_POWER;
	
		if (intmask & SDHCI_INT_CARD_INT)
			cardint = 1;
	
		intmask &= ~SDHCI_INT_CARD_INT;
	
		if (intmask) {
			printk(KERN_ERR "%s: Unexpected interrupt 0x%08x.\n",
				mmc_hostname(host->mmc), intmask);
			sdhci_dumpregs(host);
	
			iowrite32(intmask, host->ioaddr + SDHCI_INT_STATUS);
		}

        
		intmask = ioread32(host->ioaddr + SDHCI_INT_STATUS);
		
	}

	spin_unlock(&host->lock);
out:
	result = IRQ_HANDLED;
	
		mmiowb();

		

	//we have to clear the GPIO event status bit here
	gpio7stat  = ioread32(host->chip->gpiobase+0x10);
	gpio7stat  |= ((unsigned int)1 << 7);
	iowrite32(gpio7stat ,host->chip->gpiobase+0x10);


	

	/*
	 * We have to delay this as it calls back into the driver.
	 */
	if (cardint)
		mmc_signal_sdio_irq(host->mmc);

	

	return result;
}

/*****************************************************************************\
 *                                                                           *
 * Suspend/resume                                                            *
 *                                                                           *
\*****************************************************************************/

#ifdef CONFIG_PM

static int sdhci_suspend (struct pci_dev *pdev, pm_message_t state)
{
	struct sdhci_chip *chip;
	int i, ret;

	printk(KERN_WARN "***** Suspend not changed from PCI ..why are we getting this? %s\n",__FUNCTION__);
	chip = pci_get_drvdata(pdev);
	if (!chip)
		return 0;

	DBG("Suspending...\n");

	for (i = 0;i < chip->num_slots;i++) {
		if (!chip->hosts[i])
			continue;
		ret = mmc_suspend_host(chip->hosts[i]->mmc, state);
		if (ret) {
			for (i--;i >= 0;i--)
				mmc_resume_host(chip->hosts[i]->mmc);
			return ret;
		}
	}

	pci_save_state(pdev);
	pci_enable_wake(pdev, pci_choose_state(pdev, state), 0);

	for (i = 0;i < chip->num_slots;i++) {
		if (!chip->hosts[i])
			continue;
		free_irq(chip->hosts[i]->irq, chip->hosts[i]);
	}

	pci_disable_device(pdev);
	pci_set_power_state(pdev, pci_choose_state(pdev, state));

	return 0;
}

static int sdhci_resume (struct pci_dev *pdev)
{
	struct sdhci_chip *chip;
	int i, ret;

	printk(KERN_WARN "***** Resume not changed from PCI ..why are we getting this? %s\n",__FUNCTION__);

	chip = pci_get_drvdata(pdev);
	if (!chip)
		return 0;

	DBG("Resuming...\n");

	pci_set_power_state(pdev, PCI_D0);
	pci_restore_state(pdev);
	ret = pci_enable_device(pdev);
	if (ret)
		return ret;

	for (i = 0;i < chip->num_slots;i++) {
		if (!chip->hosts[i])
			continue;
		if (chip->hosts[i]->flags & SDHCI_USE_DMA)
			pci_set_master(pdev);
		ret = request_irq(chip->hosts[i]->irq, sdhci_irq,
			IRQF_SHARED, chip->hosts[i]->slot_descr,
			chip->hosts[i]);
		if (ret)
			return ret;
		sdhci_init(chip->hosts[i]);
		mmiowb();
		ret = mmc_resume_host(chip->hosts[i]->mmc);
		if (ret)
			return ret;
	}

	return 0;
}

#else /* CONFIG_PM */

#define sdhci_suspend NULL
#define sdhci_resume NULL

#endif /* CONFIG_PM */


static int __init sdhci_probe(struct platform_device *pdev)
{

	//allocate chip and set it
	int ret;
	struct sdhci_chip* chip;
	unsigned int version;
	struct mmc_host* mmc;
	struct sdhci_host* host;
	unsigned int caps;
	unsigned int mf;
	int i;


	void __iomem *		clkbase;
	void __iomem *		intbase;



	printk( KERN_INFO "Probe called\n");
	//printk("*****%s\n",__FUNCTION__);	


	//we allocate the chip which in turn can hold multiple slots..
	//well in this case we have only one slot
	chip = kzalloc(sizeof(struct sdhci_chip) +
		sizeof(struct sdhci_host*) * 1, GFP_KERNEL);

	if (!chip) 
	{
		//ret = -ENOMEM;
		//goto err;
		return -ENOMEM;
	}

	chip->pdev = (struct pci_dev*)pdev;
    chip->num_slots = 1;

	//set it as our dirver data in plat device
	platform_set_drvdata(pdev,chip);


    
	//now do resource alloc stuff that was previously done for each slot
	//we already have chip
	//get a mmc host
	mmc = mmc_alloc_host(sizeof(struct sdhci_host), &pdev->dev);
	if (!mmc)
		return -ENOMEM;

	//we remember the host as mmc in our sdhci_host structure
	host = mmc_priv(mmc);
	host->mmc = mmc;

	//in our sdhci_host we remember our chip
	host->chip = chip;
	//and in our chip we point back to the host..yeooogh
	//dont ask me , please.
    chip->hosts[0] = host;

	host->addr = 0xb0007000;
	host->irq = 3;

	if(request_mem_region(0xb0007000,0xff,"SDHCI") == NULL)
	{
		printk("Yeeks!!\n");
	}

	host->ioaddr = ioremap(host->addr,0xff);
	if(!host->ioaddr)
	{
		return -ENOMEM;
	}

	printk("trying multiple resets before doing any clock stuff\n");
	for(i = 0; i< 10;i++)
	{
		//sdhci_reset(host,SDHCI_RESET_ALL);

	}
	printk("done\n");

	clkbase = ioremap_nocache(0xb0000000,0x4000);

	printk("after ioremap of clock base is %p!!\n",clkbase);

	//let us set SD multifunction
	mf = ioread32(clkbase+0x0c);
	mf |= 1<<30;
	iowrite32(mf,clkbase+0x0c);

	//also enable sd clock
	mf = ioread32(clkbase + 0x0200);
	mf |= 1<<28;
	iowrite32(mf,clkbase + 0x0200);

	//so the software ip reset bit
	mf = ioread32(clkbase + 0x0200+0x20);
	mf |= 1 << 28;
	iowrite32(mf,clkbase + 0x0200+0x20);
	mdelay(1000);
	mf = ioread32(clkbase + 0x0200+0x20);
	mf &= ~(1 << 28);
	iowrite32(mf,clkbase + 0x0200+0x20);

	iounmap(clkbase);


	//set gpio for configuring IRQ properly
	chip->gpiobase = ioremap_nocache(0xb8003000,0x4000);
	mf = ioread32(chip->gpiobase+ 0x0c);
	mf &= ~((unsigned int)0x0ff0);
	iowrite32(mf,chip->gpiobase+0x0c);

	mf = ioread32(chip->gpiobase+0x0);
	mf |= ((unsigned int)1 << 7);
	iowrite32(mf,chip->gpiobase+0x0);

	mf = ioread32(chip->gpiobase+0x04);
	mf |= ((unsigned int)1 << 7);
	iowrite32(mf,chip->gpiobase+0x04);

	mf = ioread32(chip->gpiobase+0x10);
	mf |= ((unsigned int)1 << 7);
	iowrite32(mf,chip->gpiobase+0x10);

    



	printk("before reset\n");


	sdhci_reset(host,SDHCI_RESET_ALL);

	printk("after reset\n");

	printk("trying multiple resets before version\n");
	for(i = 0; i< 10;i++)
	{
		sdhci_reset(host,SDHCI_RESET_ALL);

	}
	printk("done\n");

	version = ioread16(host->ioaddr + SDHCI_HOST_VERSION);
	printk("Vesion is %x\n",version);
	version = (version & SDHCI_SPEC_VER_MASK) >> SDHCI_SPEC_VER_SHIFT;
	if (version > 1) 
	{
		printk(KERN_ERR "%s: Unknown controller version (%d). "
			"You may experience problems.\n", host->slot_descr,
			version);
	}

	printk("trying multiple resets after version\n");
	for(i = 0; i< 10;i++)
	{
		sdhci_reset(host,SDHCI_RESET_ALL);

	}
	printk("done\n");


	

	caps = ioread32(host->ioaddr + SDHCI_CAPABILITIES);

	//let us do PIO for now
	host->flags &= ~SDHCI_USE_DMA;
	//host->flags |= SDHCI_USE_DMA;

	host->max_clk =
		(caps & SDHCI_CLOCK_BASE_MASK) >> SDHCI_CLOCK_BASE_SHIFT;
	if (host->max_clk == 0) 
	{
		printk(KERN_ERR "%s: Hardware doesn't specify base clock "
			"frequency.\n", host->slot_descr);
		ret = -ENODEV;
		goto unmap;
	}
	else
	{
		printk("Hardware base clock freq ok\n");
	}
	host->max_clk *= 1000000;

	host->timeout_clk =
		(caps & SDHCI_TIMEOUT_CLK_MASK) >> SDHCI_TIMEOUT_CLK_SHIFT;
	if (host->timeout_clk == 0) {
		printk(KERN_ERR "%s: Hardware doesn't specify timeout clock "
			"frequency.\n", host->slot_descr);
		ret = -ENODEV;
		goto unmap;
	}


	if (caps & SDHCI_TIMEOUT_CLK_UNIT)
		host->timeout_clk *= 1000;


	/*
	 * Set host parameters.
	 */
	mmc->ops = &sdhci_ops;
	mmc->f_min = host->max_clk / 256;
	mmc->f_max = host->max_clk;
	mmc->caps = MMC_CAP_4_BIT_DATA | MMC_CAP_MULTIWRITE | MMC_CAP_SDIO_IRQ;

	if (caps & SDHCI_CAN_DO_HISPD)
		mmc->caps |= MMC_CAP_SD_HIGHSPEED;

	mmc->ocr_avail = 0;
	if (caps & SDHCI_CAN_VDD_330)
		mmc->ocr_avail |= MMC_VDD_32_33|MMC_VDD_33_34;
	if (caps & SDHCI_CAN_VDD_300)
		mmc->ocr_avail |= MMC_VDD_29_30|MMC_VDD_30_31;
	if (caps & SDHCI_CAN_VDD_180)
		mmc->ocr_avail |= MMC_VDD_165_195;

	if (mmc->ocr_avail == 0) {
		printk(KERN_ERR "%s: Hardware doesn't report any "
			"support voltages.\n", host->slot_descr);
		ret = -ENODEV;
		goto unmap;
	}

	spin_lock_init(&host->lock);

	/*
	 * Maximum number of segments. Hardware cannot do scatter lists.
	 */
	if (host->flags & SDHCI_USE_DMA)
		mmc->max_hw_segs = 1;
	else
		mmc->max_hw_segs = 16;
	mmc->max_phys_segs = 16;

	/*
	 * Maximum number of sectors in one transfer. Limited by DMA boundary
	 * size (512KiB).
	 */
	mmc->max_req_size = 524288;

	/*
	 * Maximum segment size. Could be one segment with the maximum number
	 * of bytes.
	 */
	mmc->max_seg_size = mmc->max_req_size;

	/*
	 * Maximum block size. This varies from controller to controller and
	 * is specified in the capabilities register.
	 */
	mmc->max_blk_size = (caps & SDHCI_MAX_BLOCK_MASK) >> SDHCI_MAX_BLOCK_SHIFT;
	if (mmc->max_blk_size >= 3) {
		printk(KERN_WARNING "%s: Invalid maximum block size, assuming 512\n",
			host->slot_descr);
		mmc->max_blk_size = 512;
	} else
		mmc->max_blk_size = 512 << mmc->max_blk_size;

	/*
	 * Maximum block count.
	 */
	//mmc->max_blk_count = 65535;
	mmc->max_blk_count = 65535;


	/*
	 * Init tasklets.
	 */
	tasklet_init(&host->card_tasklet,
		sdhci_tasklet_card, (unsigned long)host);
	tasklet_init(&host->finish_tasklet,
		sdhci_tasklet_finish, (unsigned long)host);

	setup_timer(&host->timer, sdhci_timeout_timer, (unsigned long)host);

	printk("trying multiple resets before req irq\n");
	for(i = 0; i< 10;i++)
	{
		sdhci_reset(host,SDHCI_RESET_ALL);

	}
	printk("done\n");

	ret = request_irq(host->irq, sdhci_irq, IRQF_DISABLED,
		"SD", host);
    if (ret)
		goto untasklet;


	printk("trying multiple resets after req irq\n");
	for(i = 0; i< 10;i++)
	{
		sdhci_reset(host,SDHCI_RESET_ALL);

	}
	printk("done\n");

	//now enalbe the interrupt in teh gpio
	mf = ioread32(chip->gpiobase+ 0x0c);
	mf |= ((unsigned int)1<<7);
	printk("mf is now %x\n",mf);
	iowrite32(mf,chip->gpiobase+0x0c);

	


	sdhci_init(host);

#ifdef CONFIG_MMC_DEBUG
	sdhci_dumpregs(host);
#endif

	mmiowb();

	mmc_add_host(mmc);

	printk(KERN_INFO "%s: SDHCI at 0x%08lx irq %d %s\n", mmc_hostname(mmc),
		host->addr, host->irq,
		(host->flags & SDHCI_USE_DMA)?"DMA":"PIO");

	return 0;

untasklet:
	tasklet_kill(&host->card_tasklet);
	tasklet_kill(&host->finish_tasklet);
unmap:
	iounmap(host->ioaddr);
release:
	//pci_release_region(pdev, host->bar);
//free:
	mmc_free_host(mmc);

	return ret;
	



	return 0;

	/*int ret, i;
	u8 slots, rev;
	struct sdhci_chip *chip;

	BUG_ON(pdev == NULL);
	BUG_ON(ent == NULL);

	printk("Probe called!!\n");

	pci_read_config_byte(pdev, PCI_CLASS_REVISION, &rev);

	printk(KERN_INFO DRIVER_NAME
		": SDHCI controller found at %s [%04x:%04x] (rev %x)\n",
		pci_name(pdev), (int)pdev->vendor, (int)pdev->device,
		(int)rev);

	ret = pci_read_config_byte(pdev, PCI_SLOT_INFO, &slots);
	if (ret)
		return ret;

	slots = PCI_SLOT_INFO_SLOTS(slots) + 1;
	DBG("found %d slot(s)\n", slots);
	if (slots == 0)
		return -ENODEV;

	ret = pci_enable_device(pdev);
	if (ret)
		return ret;

	chip = kzalloc(sizeof(struct sdhci_chip) +
		sizeof(struct sdhci_host*) * slots, GFP_KERNEL);
	if (!chip) {
		ret = -ENOMEM;
		goto err;
	}

	chip->pdev = pdev;
	chip->quirks = ent->driver_data;

	if (debug_quirks)
		chip->quirks = debug_quirks;

	chip->num_slots = slots;
	pci_set_drvdata(pdev, chip);

	for (i = 0;i < slots;i++) {
		ret = sdhci_probe_slot(pdev, i);
		if (ret) {
			for (i--;i >= 0;i--)
				sdhci_remove_slot(pdev, i);
			goto free;
		}
	}

	return 0;

free:
	pci_set_drvdata(pdev, NULL);
	kfree(chip);

err:
	pci_disable_device(pdev);
	return ret;*/



}

static int __exit sdhci_remove(struct platform_device *pdev)
{

	/*chip = pci_get_drvdata(pdev);

	if (chip) {
		for (i = 0;i < chip->num_slots;i++)
			sdhci_remove_slot(pdev, i);

		pci_set_drvdata(pdev, NULL);

		kfree(chip);
	}

	pci_disable_device(pdev);*/



	struct mmc_host* mmc;
	struct sdhci_chip* chip;
	struct sdhci_host* host;

		//printk("*****%s\n",__FUNCTION__);

	printk("In remove\n");

	chip = platform_get_drvdata(pdev);
	host = chip->hosts[0];
	mmc = host->mmc;

	chip->hosts[0] = NULL;

	mmc_remove_host(mmc);

	sdhci_reset(host, SDHCI_RESET_ALL);

	free_irq(host->irq, host);

	del_timer_sync(&host->timer);

	tasklet_kill(&host->card_tasklet);
	tasklet_kill(&host->finish_tasklet);

	iounmap(host->ioaddr);
	iounmap(chip->gpiobase);

	release_mem_region(host->addr,0xff);

//	pci_release_region(pdev, host->bar);

	mmc_free_host(mmc);

    
	return 0;
}

/*static struct pci_driver sdhci_driver_pci = {
	.name = 	DRIVER_NAME,
	.id_table =	pci_ids,
	.probe = 	sdhci_probe,
	.remove =	__devexit_p(sdhci_remove),
	.suspend =	sdhci_suspend,
	.resume	=	sdhci_resume,
};*/

static struct platform_driver sdhci_driver = {
	.remove		= __exit_p(sdhci_remove),
	.suspend	= sdhci_suspend,
	.resume		= sdhci_resume,
	.probe 		= sdhci_probe,
	.driver		= {
		.name	= DRIVER_NAME,
		.owner	= THIS_MODULE,
	},
};

static struct platform_device *sdhci_platdev;	

/*****************************************************************************\
 *                                                                           *
 * Driver init/exit                                                          *
 *                                                                           *
\*****************************************************************************/

static int __init sdhci_drv_init(void)
{

	int err;
	struct resource *res;

	res = kmalloc(sizeof(*res),GFP_KERNEL);
	if(!res)
	{
		return -ENOMEM;
	}

	//fill in the resources

	
	printk(KERN_INFO DRIVER_NAME
		": Secure Digital Host Controller Interface driver\n");
	printk(KERN_INFO DRIVER_NAME ": Copyright(c) Pierre Ossman\n");

    
	err = platform_driver_register(&sdhci_driver);
	if(err)
		return err;

	sdhci_platdev = platform_device_register_simple(DRIVER_NAME, -1, NULL, 0);
	if (IS_ERR(sdhci_platdev)) 
	{
		err = PTR_ERR(sdhci_platdev);
		goto unreg_platform_driver;
	}

	return 0;


unreg_platform_driver:
	platform_driver_unregister(&sdhci_driver);
	return err;
   
}

static void __exit sdhci_drv_exit(void)
{
	DBG("Exiting\n");

	platform_device_unregister(sdhci_platdev);
	platform_driver_unregister(&sdhci_driver);
}

module_init(sdhci_drv_init);
module_exit(sdhci_drv_exit);

module_param(debug_quirks, uint, 0444);

MODULE_AUTHOR("Pierre Ossman <drzeus@drzeus.cx>");
MODULE_DESCRIPTION("Secure Digital Host Controller Interface driver");
MODULE_LICENSE("GPL");

MODULE_PARM_DESC(debug_quirks, "Force certain quirks.");
