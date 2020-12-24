/**
 * $Id: aess_sdhcdrv.c,v 1.19 2008/12/12 03:40:23 christsai Exp $
 *
 *<center>
 *               Avocent Corporation. Proprietary Information.
 * \n<em>
 *      This software is supplied under the terms of a license agreement or
 *      nondisclosure agreement with Avocent Corporation, or its subsidiary, and
 *      may not be copied, disseminated, or distributed except in accordance
 *      with the terms of that agreement.
 *
 *        		Copyright &copy; 2007-2008 Avocent Corporation.
 *
 * 		This program is free software; you can redistribute it and/or modify
 * 		it under the terms of the GNU General Public License as published by
 * 		the Free Software Foundation; either version 2 of the License, or (at
 * 		your option) any later version.
 *
 *</em> </center>
 *----------------------------------------------------------------------------\n
 *  MODULES     WPCM450 Secure Digital Host Controller Interface Driver\n
 *----------------------------------------------------------------------------\n
 *  @file   aess_sdhcdrv.c
 *  @brief  This is the c file where all SDHC H/W related routines are
 *          implemented.
 *
 *  @internal
 *  $RCSfile: aess_sdhcdrv.c,v $
 *  $Revision: 1.19 $
 *  $Date: 2008/12/12 03:40:23 $
 *  $Author: christsai $
 *  $Source: /home/cvsroot/core/puti/platform/wpcm450/evb/src/kernel/linux-2.6.23.1/drivers/mmc/host/aess_sdhcdrv.c,v $
 *  $Name:  $
 *----------------------------------------------------------------------------*/

#include <linux/delay.h>
#include <linux/highmem.h>
#include <linux/dma-mapping.h>

#include <linux/mmc/host.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/semaphore.h>

#include <asm/scatterlist.h>

#include <asm/arch/regs-gcr.h>
#include <asm/arch/regs-clock.h>
#include <asm/arch/regs-gpio.h>


#include "aess_sdhcdrv.h"

/* To work around timeout issue occurred when multi-block write command 
 *	for Z2 chip only. (chtsai) */
#define FIX_TIMEOUT_PROBLEM

/* To work around "Never release inhibit bit(s)". (chtsai) */
#define FIX_WAIT_FOR_INHIBIT_BITS

/* Define this to work around the data timeout problem for eMMC,
 * whose data timeout for write would be over 3 seconds. (chtsai) 
 */
#define IGNORE_DATA_TIMEOUT_TOO_LARGE

#if 0
/* Define this to work around write protect signal problem. (chtsai) 
 * - WPCM450 Z21 and A1 return different values for SDWP.
 * - AMEA X01 inverts the WP signal and X00 does not.
 */
#define IGNORE_SDWP_SIGNAL
#endif

/* SD Host Control Register(SDHC_BA) */
#define WPCM450_PA_SDHC		(0xB0007000)
#define WPCM450_SZ_SDHC		SDHCI_REGSET_SIZE

#define WPCM450_GCR_MFSEL2  WPCM450_GCR(0x10)

/* Bits for Global Control Register */
#define WPCM450_HSP1SEL					(10)
#define WPCM450_SDIOSEL					(30)

#define WPCM450_H6GSEL					(30)

#define WPCM450_PDID					WPCM450_GCR(0x00)
#define WPCM450_PDID_Z1					(0x00926450)
#define WPCM450_PDID_Z2					(0x04926450)
#define WPCM450_PDID_A1					(0x08926450)
#define WPCM450_INTCR2					WPCM450_GCR(0x60)
#define WPCM450_SDSD					(0)

/* For Clock Control Registers */
#define WPCM450_CLKEN_SDIO				(28)
#define WPCM450_IPSRST_SDIO				(28)

#define GPIO_REG_PORT2_CFG0_ADDR        WPCM450_GPIOREG(0x3c)
#define GPIO_REG_PORT2_CFG1_ADDR        WPCM450_GPIOREG(0x40)
#define GPIO_REG_PORT2_CFG2_ADDR        WPCM450_GPIOREG(0x44)
#define GPIO_REG_PORT2_DATAOUT_ADDR     WPCM450_GPIOREG(0x48)
#define GPIO_REG_PORT2_DATAIN_ADDR      WPCM450_GPIOREG(0x4c)
#define GPIO_GPIO38						(6)

#define GPIO_REG_PORT3_CFG0_ADDR        WPCM450_GPIOREG(0x50)
#define GPIO_REG_PORT3_CFG1_ADDR        WPCM450_GPIOREG(0x54)
#define GPIO_REG_PORT3_CFG2_ADDR        WPCM450_GPIOREG(0x58)
#define GPIO_REG_PORT3_DATAOUT_ADDR     WPCM450_GPIOREG(0x5c)
#define GPIO_REG_PORT3_DATAIN_ADDR      WPCM450_GPIOREG(0x60)
#define GPIO_GPIO59						(11)
#define GPIO_GPO63						(15)

#define GPIO_REG_PORT6_DATAIN_ADDR      WPCM450_GPIOREG(0x8c)
#define GPIO_GPI111						(15)

#define GPIO_REG_PORT7_CFG0_ADDR        WPCM450_GPIOREG(0x90)
#define GPIO_REG_PORT7_CFG1_ADDR        WPCM450_GPIOREG(0x94)
#define GPIO_REG_PORT7_CFG2_ADDR        WPCM450_GPIOREG(0x98)
#define GPIO_REG_PORT7_DATAOUT_ADDR     WPCM450_GPIOREG(0x9c)
#define GPIO_REG_PORT7_DATAIN_ADDR      WPCM450_GPIOREG(0xa0)
#define GPIO_GPIO124					(10)

#define GPIO_REG_EVENT_TYPE_ADDR    	WPCM450_GPIOREG(0x0)
#define GPIO_REG_EVENT_POLARITY_ADDR    WPCM450_GPIOREG(0x4)
#define GPIO_REG_EVENT_DEBOUNCE_ADDR    WPCM450_GPIOREG(0x8)
#define GPIO_REG_EVENT_ENABLE_ADDR      WPCM450_GPIOREG(0xc)
#define GPIO_REG_EVENT_STATUS_ADDR      WPCM450_GPIOREG(0x10)
#define GPIO_GPIOE7						(7)

#define RESSIZE(ressource) (((ressource)->end - (ressource)->start)+1)

#define DRIVER_NAME "aess_sdhci"

#define DRIVER_VERSION_NO		"0.23.1.17"

#if defined(CONFIG_MMC_WINBOND_EVB)
# define DRIVER_VERSION_POSTFIX	"e"
#elif defined(CONFIG_MMC_WHOVILLE)
# define DRIVER_VERSION_POSTFIX	"w"
#endif

#define DRIVER_VERSION			DRIVER_VERSION_NO DRIVER_VERSION_POSTFIX

#define DRIVER_DESCRIPTION "Secure Digital Host Controller Interface driver"

enum debug_level_e {
	DEBUG_LEVEL_MESSAGE_DISABLE,
	DEBUG_LEVEL_MESSAGE_ENABLE,
	DEBUG_LEVEL_MESSAGE_VERBOSE
};

static char *version = DRIVER_VERSION;

static unsigned int debug_level = DEBUG_LEVEL_MESSAGE_DISABLE;

# define DBG(f, x...) \
	if (debug_level >= DEBUG_LEVEL_MESSAGE_ENABLE) \
		printk(DRIVER_NAME " [%s()]: " f, __func__,## x)

# define DBG_V(f, x...) \
	if (debug_level >= DEBUG_LEVEL_MESSAGE_VERBOSE) \
		printk(DRIVER_NAME " [%s()]: " f, __func__,## x)

static unsigned int debug_quirks = 0;

#define SDHCI_QUIRK_CLOCK_BEFORE_RESET			(1<<0)
#define SDHCI_QUIRK_FORCE_DMA					(1<<1)
/* Controller doesn't like some resets when there is no card inserted. */
#define SDHCI_QUIRK_NO_CARD_NO_RESET			(1<<2)
#define SDHCI_QUIRK_SINGLE_POWER_WRITE			(1<<3)
#define SDHCI_QUIRK_RESET_CMD_DATA_ON_IOS		(1<<4)
#define SDHCI_QUIRK_MUXED_SYSTEM				(1<<5)

#define SET_MUXED_SYSTEM(c)	(c->quirks |= SDHCI_QUIRK_MUXED_SYSTEM)
#define IS_MUXED_SYSTEM(c)	(c->quirks & SDHCI_QUIRK_MUXED_SYSTEM)


/***************************************************************************** 
 *                                                                           *
 * Macros to control SDHC and Mux                                            *
 *                                                                           *
 *****************************************************************************/

#if defined(CONFIG_MMC_WINBOND_EVB)

# define SD_MUX_INIT()			\
	/** Enable GPO63 */ \
	clear_bit(WPCM450_HSP1SEL,(void *)(WPCM450_GCR_MFSEL1)); \
	/* output buffer enable */ \
	set_bit(GPIO_GPO63,(void *)(GPIO_REG_PORT3_CFG0_ADDR))

# define SELECT_MMC_SLOT_0		set_bit(GPIO_GPO63,(void *)(GPIO_REG_PORT3_DATAOUT_ADDR))
# define SELECT_MMC_SLOT_1		clear_bit(GPIO_GPO63,(void *)(GPIO_REG_PORT3_DATAOUT_ADDR))

#elif defined(CONFIG_MMC_WHOVILLE)	

# define MASER_DC_PRESENT()		(!test_bit(GPIO_GPI111,(void *)(GPIO_REG_PORT6_DATAIN_ADDR)))

# define MUX_OE()			\
	/* output buffer enable */ \
	set_bit(GPIO_GPIO124,(void *)(GPIO_REG_PORT7_CFG0_ADDR)); \
	/** Enable MUX output */ \
	clear_bit(GPIO_GPIO124,(void *)(GPIO_REG_PORT7_DATAOUT_ADDR))

# define EN_AMEA_SD_POWER_CTRL()			\
	/** Enable GPO63 */ \
	clear_bit(WPCM450_HSP1SEL,(void *)(WPCM450_GCR_MFSEL1)); \
	/* output buffer enable */ \
	set_bit(GPIO_GPO63,(void *)(GPIO_REG_PORT3_CFG0_ADDR))

# define POWER_AMEA_SD_ON()			\
	/** Enable AMEA SD card power */ \
	set_bit(GPIO_GPO63,(void *)(GPIO_REG_PORT3_DATAOUT_ADDR))

# define POWER_AMEA_SD_OFF()			\
	/** Disable AMEA SD card power */ \
	clear_bit(GPIO_GPO63,(void *)(GPIO_REG_PORT3_DATAOUT_ADDR))

# define POWER_MASER()			\
	/** Enable GPIO59 */ \
	clear_bit(WPCM450_H6GSEL,(void *)(WPCM450_GCR_MFSEL2)); \
	/* output buffer enable */ \
	set_bit(GPIO_GPIO59,(void *)(GPIO_REG_PORT3_CFG0_ADDR)); \
	/** Enable MASER power */ \
	clear_bit(GPIO_GPIO59,(void *)(GPIO_REG_PORT3_DATAOUT_ADDR))

# define SELECT_MMC_SLOT_0		set_bit(GPIO_GPIO38,(void *)(GPIO_REG_PORT2_DATAOUT_ADDR))
# define SELECT_MMC_SLOT_1		clear_bit(GPIO_GPIO38,(void *)(GPIO_REG_PORT2_DATAOUT_ADDR))

#endif

#define SELECT_MMC_SLOT(x)		SELECT_MMC_SLOT_##x


static void sdhci_prepare_data(struct sdhci_host *, struct mmc_data *);
static void sdhci_finish_data(struct sdhci_host *);

static void sdhci_send_command(struct sdhci_host *, struct mmc_command *);
static void sdhci_finish_command(struct sdhci_host *);

static void sdhci_reset(struct sdhci_host *host, u8 mask);
static void sdhci_set_clock(struct sdhci_host *host, unsigned int clock);

#ifdef CONFIG_MMC_WHOVILLE
static void sdhci_switch_slot(struct sdhci_host *host);
static int sdhci_card_detect(struct mmc_host *mmc);

int CardChg = 0;
#endif

/******************************************************************************
*   FUNCTION        :   sdhci_dumpregs
******************************************************************************/
/**
 *  @brief      This API is to dump the registers for debugging
 *
 *  @return     None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Internal function\n
 *
 *****************************************************************************/
static void sdhci_dumpregs(
							/** A pointer to the host */
						    struct sdhci_host *host
						   )
{
	/** - Read registers and print them out. */
	printk(KERN_DEBUG DRIVER_NAME ": ============== REGISTER DUMP ==============\n");

	printk(KERN_DEBUG DRIVER_NAME ": Sys addr: 0x%08x | Version:  0x%08x\n",
		readl(host->ioaddr + SDHCI_DMA_ADDRESS),
		readw(host->ioaddr + SDHCI_HOST_VERSION));
	printk(KERN_DEBUG DRIVER_NAME ": Blk size: 0x%08x | Blk cnt:  0x%08x\n",
		readw(host->ioaddr + SDHCI_BLOCK_SIZE),
		readw(host->ioaddr + SDHCI_BLOCK_COUNT));
	printk(KERN_DEBUG DRIVER_NAME ": Command:  0x%08x | SW Reset: 0x%08x\n",
		readw(host->ioaddr + SDHCI_COMMAND),
		readb(host->ioaddr + SDHCI_SOFTWARE_RESET));
	printk(KERN_DEBUG DRIVER_NAME ": Argument: 0x%08x | Trn mode: 0x%08x\n",
		readl(host->ioaddr + SDHCI_ARGUMENT),
		readw(host->ioaddr + SDHCI_TRANSFER_MODE));
	printk(KERN_DEBUG DRIVER_NAME ": Present:  0x%08x | Host ctl: 0x%08x\n",
		readl(host->ioaddr + SDHCI_PRESENT_STATE),
		readb(host->ioaddr + SDHCI_HOST_CONTROL));
	printk(KERN_DEBUG DRIVER_NAME ": Power:    0x%08x | Blk gap:  0x%08x\n",
		readb(host->ioaddr + SDHCI_POWER_CONTROL),
		readb(host->ioaddr + SDHCI_BLOCK_GAP_CONTROL));
	printk(KERN_DEBUG DRIVER_NAME ": Wake-up:  0x%08x | Clock:    0x%08x\n",
		readb(host->ioaddr + SDHCI_WAKE_UP_CONTROL),
		readw(host->ioaddr + SDHCI_CLOCK_CONTROL));
	printk(KERN_DEBUG DRIVER_NAME ": Timeout:  0x%08x | Int stat: 0x%08x\n",
		readb(host->ioaddr + SDHCI_TIMEOUT_CONTROL),
		readl(host->ioaddr + SDHCI_INT_STATUS));
	printk(KERN_DEBUG DRIVER_NAME ": Int enab: 0x%08x | Sig enab: 0x%08x\n",
		readl(host->ioaddr + SDHCI_INT_ENABLE),
		readl(host->ioaddr + SDHCI_SIGNAL_ENABLE));
	printk(KERN_DEBUG DRIVER_NAME ": AC12 err: 0x%08x | Slot int: 0x%08x\n",
		readw(host->ioaddr + SDHCI_ACMD12_ERR),
		readw(host->ioaddr + SDHCI_SLOT_INT_STATUS));
	printk(KERN_DEBUG DRIVER_NAME ": Caps:     0x%08x | Max curr: 0x%08x\n",
		readl(host->ioaddr + SDHCI_CAPABILITIES),
		readl(host->ioaddr + SDHCI_MAX_CURRENT));

	printk(KERN_DEBUG DRIVER_NAME ": ===========================================\n");
}

/*****************************************************************************
 *                                                                           *
 * Low level functions                                                       *
 *                                                                           *
 *****************************************************************************/

#ifdef CONFIG_MMC_WHOVILLE
/******************************************************************************
*   FUNCTION        :   sdhci_switch_slot
******************************************************************************/
/**
 *  @brief      This API is to switch the slot
 *
 *  @return     None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Internal function\n
 *
 *****************************************************************************/
static void sdhci_switch_slot(
							   /** A pointer to the host */
						       struct sdhci_host *host
						      )
{
	u32 u32RegIntEn;
	u32 u32RegSigEn;
	int	i;
	
	/** - If the selected slot is current, just return */
	if (host->chip->cur_slot == host->slot)
	{
		return;
	}
	
	DBG("host->slot => %d\n", host->slot);

	/** - Save interrupt registers */
	u32RegIntEn = readl(host->ioaddr + SDHCI_INT_ENABLE);
	u32RegSigEn = readl(host->ioaddr + SDHCI_SIGNAL_ENABLE);
	
	/** - Disable interrupts */
	writel(0, host->ioaddr + SDHCI_INT_ENABLE);
	writel(0, host->ioaddr + SDHCI_SIGNAL_ENABLE);
	
	/** - Save registers for current host */
	if (host->chip->hosts[host->chip->cur_slot])
	{
		for (i=0; i<0x30; i+=4)
		{
			*(unsigned int *)(host->chip->hosts[host->chip->cur_slot]->reg_set+i) = 
					readl(host->chip->hosts[host->chip->cur_slot]->ioaddr + i);
		}
		*(u16 *)(host->chip->hosts[host->chip->cur_slot]->reg_set+SDHCI_HOST_VERSION) = 
			readw(host->chip->hosts[host->chip->cur_slot]->ioaddr + SDHCI_HOST_VERSION);
	}
	
#if 1	// Cause EVB system hang! (OK in Thidwitck?!)
	/** - Stop clock */
	writew(0, host->ioaddr + SDHCI_CLOCK_CONTROL);
#endif

	if (0 == host->slot)
	{
		SELECT_MMC_SLOT(0);
	}
	else
	{
		SELECT_MMC_SLOT(1);
	}
	
	/** - For high speed MMC, it needs to set INTCR2.SDSD to 0;
		otherwise, set SDSD to 1. */
	if (host->hs_mmc)
	{
		/* Disable SD sampling delay for HS MMC. */
		clear_bit(WPCM450_SDSD,(void *)(WPCM450_INTCR2));
	}
	else
	{
		/* Enable SD sampling delay for SD standard timing. */
		set_bit(WPCM450_SDSD,(void *)(WPCM450_INTCR2));
	}

	/** - Restore registers for the targeted host if they have been saved. */
	if (host->reg_set[SDHCI_HOST_VERSION] != 0)
	{
		for (i=0; i<0x30; i+=4)
		{
			/* Do not restore Command Register, 
				or it will cause invalid command. */
			if (i == SDHCI_TRANSFER_MODE)
				continue;
				
			writel(*(unsigned int *)(host->reg_set+i), host->ioaddr + i);
		}
	}
	
	host->chip->cur_slot = host->slot;
	
#if 0	//chtsai
	{ /* Cause error!? (chtsai) */
		unsigned int clock;

		DBG_V("Force a clock update\n");
		clock = host->clock;
		host->clock = 0;
		sdhci_set_clock(host, clock);
	}
#else
	/* Delay to wait for clock stable. (chtsai) */
	mdelay(5);
#endif

	/** - Reset lines to ensure later commands work. */
	sdhci_reset(host, SDHCI_RESET_CMD | SDHCI_RESET_DATA);
	
	/** - Restore interrupt registers */
	writel(u32RegIntEn, host->ioaddr + SDHCI_INT_ENABLE);
	writel(u32RegSigEn, host->ioaddr + SDHCI_SIGNAL_ENABLE);
}

#endif


/******************************************************************************
*   FUNCTION        :   sdhci_reset
******************************************************************************/
/**
 *  @brief      This API is to set the host
 *
 *  @return     None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Internal function\n
 *
 *****************************************************************************/
static void sdhci_reset(
						 /** A pointer to the host */
						 struct sdhci_host *host,
						 
						 /** Desired reset types */
						 u8 mask
						)
{
	unsigned long timeout;

	DBG_V("slot = %d, mask=%d\n", host->slot, mask);

	if (host->chip->quirks & SDHCI_QUIRK_NO_CARD_NO_RESET) {
#ifdef CONFIG_MMC_WHOVILLE
		/* The CARD_PRESENT is only for AMEA. (chtsai) 
		 * If MASER DC is present, it must be booted from it.
		 */
		if ((!MASER_DC_PRESENT() || host->slot != 0) && 
			!(readl(host->ioaddr + SDHCI_PRESENT_STATE) & SDHCI_CARD_PRESENT))
#else
		if (!(readl(host->ioaddr + SDHCI_PRESENT_STATE) & SDHCI_CARD_PRESENT))
#endif
		{
			printk(KERN_WARNING DRIVER_NAME " Card not present!\n");
			return;
		}
	}

	/** - Reset the host per requested type(s) */
	writeb(mask, host->ioaddr + SDHCI_SOFTWARE_RESET);

	/** - If it is reset all, it must delay a while. */
	if (mask & SDHCI_RESET_ALL)
	{
		host->clock = 0;
		DBG_V("reset host->clock = 0 for RESET_ALL\n");

		/* Without the delay, the driver hangs after second reset called after
			 registering IRQ, when the driver is built in kernel! (chtsai) */
		mdelay(1);
	}

	/* Wait max 100 ms */
	timeout = 100;

	/** - Wait until reset is done, or timeout, 100ms. */
	while (readb(host->ioaddr + SDHCI_SOFTWARE_RESET) & mask) {
		if (timeout == 0) {
			printk(KERN_ERR DRIVER_NAME " %s: Reset 0x%x never completed.\n",
				mmc_hostname(host->mmc), (int)mask);
			sdhci_dumpregs(host);
			return;
		}
		timeout--;
		mdelay(1);
	}
	
}

/******************************************************************************
*   FUNCTION        :   sdhci_init
******************************************************************************/
/**
 *  @brief      This API is to initialize the host
 *
 *  @return     None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Internal function\n
 *
 *****************************************************************************/
static void sdhci_init(
						 /** A pointer to the host */
						 struct sdhci_host *host
						)
{
	u32 intmask;

	DBG_V("slot=%d\n", host->slot);
	
	/** - Reset the host. */
	sdhci_reset(host, SDHCI_RESET_ALL);

	intmask = SDHCI_INT_BUS_POWER | SDHCI_INT_DATA_END_BIT |
		SDHCI_INT_DATA_CRC | SDHCI_INT_DATA_TIMEOUT | SDHCI_INT_INDEX |
		SDHCI_INT_END_BIT | SDHCI_INT_CRC | SDHCI_INT_TIMEOUT |
		SDHCI_INT_CARD_REMOVE | SDHCI_INT_CARD_INSERT |
		SDHCI_INT_DATA_AVAIL | SDHCI_INT_SPACE_AVAIL |
		SDHCI_INT_DMA_END | SDHCI_INT_DATA_END | SDHCI_INT_RESPONSE;

	/** - Enable interrupts. */
	writel(intmask, host->ioaddr + SDHCI_INT_ENABLE);
	writel(intmask, host->ioaddr + SDHCI_SIGNAL_ENABLE);
}

static void sdhci_activate_led(struct sdhci_host *host)
{
	u8 ctrl;

	ctrl = readb(host->ioaddr + SDHCI_HOST_CONTROL);
	ctrl |= SDHCI_CTRL_LED;
	writeb(ctrl, host->ioaddr + SDHCI_HOST_CONTROL);
}

static void sdhci_deactivate_led(struct sdhci_host *host)
{
	u8 ctrl;

	ctrl = readb(host->ioaddr + SDHCI_HOST_CONTROL);
	ctrl &= ~SDHCI_CTRL_LED;
	writeb(ctrl, host->ioaddr + SDHCI_HOST_CONTROL);
}

/*****************************************************************************
 *                                                                           *
 * Core functions                                                            *
 *                                                                           *
 *****************************************************************************/

static inline char* sdhci_sg_to_buffer(struct sdhci_host* host)
{
	return page_address(host->cur_sg->page) + host->cur_sg->offset;
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

/******************************************************************************
*   FUNCTION        :   sdhci_read_block_pio
******************************************************************************/
/**
 *  @brief      This API is to read a data block from the host
 *
 *  @return     None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Internal function\n
 *
 *****************************************************************************/
static void sdhci_read_block_pio(
								  /** A pointer to the host */
								  struct sdhci_host *host
								 )
{
	int blksize, chunk_remain;
	u32 data;
	char *buffer;
	int size;

	blksize = host->data->blksz;
	chunk_remain = 0;
	data = 0;

	buffer = sdhci_sg_to_buffer(host) + host->offset;

	/** - Read a block of data from the host, or until run out of buffer. */
	while (blksize) {
		if (chunk_remain == 0) {
			data = readl(host->ioaddr + SDHCI_BUFFER);
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

/******************************************************************************
*   FUNCTION        :   sdhci_write_block_pio
******************************************************************************/
/**
 *  @brief      This API is to write a data block to the host
 *
 *  @return     None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Internal function\n
 *
 *****************************************************************************/
static void sdhci_write_block_pio(
								   /** A pointer to the host */
								   struct sdhci_host *host
								  )
{
	int blksize, chunk_remain;
	u32 data;
	char *buffer;
	int bytes, size;

	blksize = host->data->blksz;
	chunk_remain = 4;
	data = 0;

	bytes = 0;
	buffer = sdhci_sg_to_buffer(host) + host->offset;

	/** - Write a block of data to the host, or until run out of data. */
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
			writel(data, host->ioaddr + SDHCI_BUFFER);
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

/******************************************************************************
*   FUNCTION        :   sdhci_transfer_pio
******************************************************************************/
/**
 *  @brief      This API is to read/write a data block from/to the host
 *
 *  @return     None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Internal function\n
 *
 *****************************************************************************/
static void sdhci_transfer_pio(
							    /** A pointer to the host */
								struct sdhci_host *host
							   )
{
	u32 mask;

	BUG_ON(!host->data);

	if (host->num_sg == 0)
		return;

	if (host->data->flags & MMC_DATA_READ)
		mask = SDHCI_DATA_AVAILABLE;
	else
		mask = SDHCI_SPACE_AVAILABLE;

	/** - According to data flag, 
			read block(s) of data from the host if data ready, or
			write block(s) of data to the host if space ready. */
	while (readl(host->ioaddr + SDHCI_PRESENT_STATE) & mask) {
		if (host->data->flags & MMC_DATA_READ)
			sdhci_read_block_pio(host);
		else
			sdhci_write_block_pio(host);

		if (host->num_sg == 0)
			break;
	}

	DBG_V("PIO transfer complete.\n");
}

/******************************************************************************
*   FUNCTION        :   sdhci_prepare_data
******************************************************************************/
/**
 *  @brief      This API is to set up registers for data to be transferred
 *
 *  @return     None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Internal function\n
 *
 *****************************************************************************/
static void sdhci_prepare_data(
							    /** A pointer to the host */
								struct sdhci_host *host,
							    
							    /** A pointer to the data */
								struct mmc_data *data
							   )
{
	unsigned target_timeout, current_timeout;
	int count;

	WARN_ON(host->data);

	/** - If there is no data for the request, just return */
	if (data == NULL)
		return;

	DBG_V("slot = %d\n", host->slot);
	
	/* Sanity checks */
	BUG_ON(data->blksz * data->blocks > 524288);
	BUG_ON(data->blksz > host->mmc->max_blk_size);
	BUG_ON(data->blocks > host->mmc->max_blk_count);

	host->data = data;
	host->data_early = 0;

	/** - Calculate the data timeout in us */
	target_timeout = data->timeout_us +
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
	DBG_V("host->timeout_clk=%d, current_timeout=%d, target_timeout=%d\n", 
			host->timeout_clk, current_timeout, target_timeout);
	
	while (current_timeout < target_timeout) {
		count++;
		current_timeout <<= 1;
		if (count >= 0xF)
			break;
	}

	if (count >= 0xF) {
#ifndef IGNORE_DATA_TIMEOUT_TOO_LARGE
		printk(KERN_WARNING DRIVER_NAME " %s: Too large timeout requested!\n",
			mmc_hostname(host->mmc));
#endif
		count = 0xE;
	}

	/** - Set up for the calculated data timeout */
	writeb(count, host->ioaddr + SDHCI_TIMEOUT_CONTROL);

	if (host->flags & SDHCI_USE_DMA) {

		BUG_ON(count != 1);

		writel(sg_dma_address(data->sg), host->ioaddr + SDHCI_DMA_ADDRESS);
	} else {
		host->cur_sg = data->sg;
		host->num_sg = data->sg_len;

		host->offset = 0;
		host->remain = host->cur_sg->length;
	}

	/** - Set up block size and count of the data */
	/* We do not handle DMA boundaries, so set it to max (512 KiB) */
	writew(SDHCI_MAKE_BLKSZ(7, data->blksz),
		host->ioaddr + SDHCI_BLOCK_SIZE);
	writew(data->blocks, host->ioaddr + SDHCI_BLOCK_COUNT);
}

/******************************************************************************
*   FUNCTION        :   sdhci_set_transfer_mode
******************************************************************************/
/**
 *  @brief      This API is to set up transfer mode
 *
 *  @return     None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Internal function\n
 *
 *****************************************************************************/
static void sdhci_set_transfer_mode(
									 /** A pointer to the host */
									 struct sdhci_host *host,
							    
							    	 /** A pointer to the data */
									 struct mmc_data *data
									)	
{
	u16 mode;

	/** - If there is no data for the request, just return */
	if (data == NULL)
		return;

	WARN_ON(!host->data);

	mode = SDHCI_TRNS_BLK_CNT_EN;
	if (data->blocks > 1)
		mode |= SDHCI_TRNS_MULTI;
	if (data->flags & MMC_DATA_READ)
		mode |= SDHCI_TRNS_READ;
	if (host->flags & SDHCI_USE_DMA)
		mode |= SDHCI_TRNS_DMA;

	/** - Set up transfer mode, including read/write, single/multi-block,
			and use DMA or not. */
	DBG_V("mode = 0x%x\n", mode);
	writew(mode, host->ioaddr + SDHCI_TRANSFER_MODE);
}

/******************************************************************************
*   FUNCTION        :   sdhci_finish_data
******************************************************************************/
/**
 *  @brief      This API is to finish the process for data transferred
 *
 *  @return     None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Internal function\n
 *
 *****************************************************************************/
static void sdhci_finish_data(
							   /** A pointer to the host */
							   struct sdhci_host *host
							  )
{
	struct mmc_data *data;
	u16 blocks;

	BUG_ON(!host->data);

	DBG_V("slot=%d\n", host->slot);

	data = host->data;
	host->data = NULL;

	if (host->flags & SDHCI_USE_DMA) {
	}
	
	/** - Collect information for transferred data bytes. */
	/*
	 * Controller doesn't count down when in single block mode.
	 */
	if (data->blocks == 1)
		blocks = (data->error == MMC_ERR_NONE) ? 0 : 1;
	else
		blocks = readw(host->ioaddr + SDHCI_BLOCK_COUNT);
	data->bytes_xfered = data->blksz * (data->blocks - blocks);

	if ((data->error == MMC_ERR_NONE) && blocks) {
		printk(KERN_ERR DRIVER_NAME " %s: Controller signalled completion even "
			"though there were blocks left.\n",
			mmc_hostname(host->mmc));
		data->error = MMC_ERR_FAILED;
	}

	/** - If there is stop command for the data, 
			- reset CMD and DAT lines if error, and
			- send the stop command. */
	/** - Otherwise, finishd the request. */
	if (data->stop) {
		/*
		 * The controller needs a reset of internal state machines
		 * upon error conditions.
		 */
		if (data->error != MMC_ERR_NONE) {
			sdhci_reset(host, SDHCI_RESET_CMD);
			sdhci_reset(host, SDHCI_RESET_DATA);
		}

		sdhci_send_command(host, data->stop);
	} else
		tasklet_schedule(&host->finish_tasklet);
}

/******************************************************************************
*   FUNCTION        :   sdhci_send_command
******************************************************************************/
/**
 *  @brief      This API is to send the command requested
 *
 *  @return     None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Internal function\n
 *
 *****************************************************************************/
static void sdhci_send_command(
							    /** A pointer to the host */
								struct sdhci_host *host,
								
							    /** A pointer to the command */
								struct mmc_command *cmd
							   )
{
	int flags;
	u32 mask;
	unsigned long timeout;

	WARN_ON(host->cmd);

	DBG("slot = %d, cmd->opcode = 0x%x\n", host->slot, cmd->opcode);

	/* Wait max 10 ms */
	timeout = 10;

#ifdef FIX_WAIT_FOR_INHIBIT_BITS
	/* Workaround: (chtsai)
	 * Since the SDHC would set Inhibit Bit (CMD) when the card is busy, 
	 *	DAT 0 is low, it stops sending any further command.
	 * Ignore the bit if it is busy.
	 */
	if (!(readl(host->ioaddr + SDHCI_PRESENT_STATE) & 0x100000)) /* DAT0=0 */
	{
		mask = 0; 						/* Ignore the bit */
	}
	else
#endif

	mask = SDHCI_CMD_INHIBIT;
	if ((cmd->data != NULL) || (cmd->flags & MMC_RSP_BUSY))
		mask |= SDHCI_DATA_INHIBIT;

	/* We shouldn't wait for data inihibit for stop commands, even
	   though they might use busy signaling */
	if (host->mrq->data && (cmd == host->mrq->data->stop))
		mask &= ~SDHCI_DATA_INHIBIT;
		
	/** - Wait for Inhibit Bit(s) released, or timeout. */
	while (readl(host->ioaddr + SDHCI_PRESENT_STATE) & mask) {
		if (timeout == 0) {
			printk(KERN_ERR DRIVER_NAME " %s: Controller never released "
					"inhibit bit(s). cmd = 0x%x, mask = 0x%x\n", 
					mmc_hostname(host->mmc), cmd->opcode, mask);
			sdhci_dumpregs(host);
			cmd->error = MMC_ERR_FAILED;
			tasklet_schedule(&host->finish_tasklet);
			return;
		}
		timeout--;
		mdelay(1);
	}

	/** - Set up a timer, 10 seconds, for the command. */
	DBG_V("jiffies = 0x%lx, HZ = %d\n", jiffies, HZ);
	mod_timer(&host->timer, jiffies + 10 * HZ);

	host->cmd = cmd;

	/** - Set up for data if any. */
	sdhci_prepare_data(host, cmd->data);
	
	/** - Set up argument for the command. */
	DBG_V("cmd->arg = 0x%x\n", cmd->arg);
	writel(cmd->arg, host->ioaddr + SDHCI_ARGUMENT);

	/** - Set up transfer mode for data if any. */
	sdhci_set_transfer_mode(host, cmd->data);

	/** - Verify response mode of the command. */
	if ((cmd->flags & MMC_RSP_136) && (cmd->flags & MMC_RSP_BUSY)) {
		printk(KERN_ERR DRIVER_NAME " %s: Unsupported response type!\n",
			mmc_hostname(host->mmc));
		cmd->error = MMC_ERR_INVALID;
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

	DBG_V("Write to command reg, flags=0x%x\n", flags);
	
	/** - Send the command, including response mode and flags. */
	writew(SDHCI_MAKE_CMD(cmd->opcode, flags),
		host->ioaddr + SDHCI_COMMAND);

}

/******************************************************************************
*   FUNCTION        :   sdhci_finish_command
******************************************************************************/
/**
 *  @brief      This API is to finish the command requested
 *
 *  @return     None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Internal function\n
 *
 *****************************************************************************/
static void sdhci_finish_command(
								  /** A pointer to the host */
								  struct sdhci_host *host
								 )
{
	int i;

	BUG_ON(host->cmd == NULL);

	/** - Update command response from registers. */
	if (host->cmd->flags & MMC_RSP_PRESENT) {
		if (host->cmd->flags & MMC_RSP_136) {
			/* CRC is stripped so we need to do some shifting. */
			for (i = 0;i < 4;i++) {
				host->cmd->resp[i] = readl(host->ioaddr +
					SDHCI_RESPONSE + (3-i)*4) << 8;
				if (i != 3)
					host->cmd->resp[i] |=
						readb(host->ioaddr +
						SDHCI_RESPONSE + (3-i)*4-1);
			}
		} else {
			host->cmd->resp[0] = readl(host->ioaddr + SDHCI_RESPONSE);
		}
	}

	host->cmd->error = MMC_ERR_NONE;

	if (host->data && host->data_early)
		sdhci_finish_data(host);

	DBG("Ending cmd (%x)\n", host->cmd->opcode);

	/** - If there is no data for the command, finish the request. */
	if (!host->cmd->data)
		tasklet_schedule(&host->finish_tasklet);

	host->cmd = NULL;
}

/******************************************************************************
*   FUNCTION        :   sdhci_set_clock
******************************************************************************/
/**
 *  @brief      This API is to set up clock of SDHC for accessed card
 *
 *  @return     None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Internal function\n
 *
 *****************************************************************************/
static void sdhci_set_clock(
							 /** A pointer to the host */
							 struct sdhci_host *host,
							 
							 /** The new clock/frequency setting */
							 unsigned int clock
							)
{
	int div=0;
	u16 clk=0;
	unsigned long timeout;

	DBG_V("slot = %d, host->max_clk = %d, host->clock = %d, clock = %d\n", 
			host->slot, host->max_clk, host->clock, clock);

    /** - If the desired clock equals to the clock set in host, just return. */
	if (clock == host->clock)
		return;

	/** - If it is MMC, must disable SD sampling delay. */
	if (clock == 26000000 || clock == 20000000)
	{
		clear_bit(WPCM450_SDSD,(void *)(WPCM450_INTCR2));
		host->hs_mmc = 1;
	}
	
#ifdef CONFIG_MMC_WHOVILLE
	/** - If it is SD, must lower its clock to 12MHz due to delay in MUX. */
	if (clock == 25000000)
	{
		clock = 12000000;
	}
#endif
	
	/** - Set clock to 0 first. */
	writew(0, host->ioaddr + SDHCI_CLOCK_CONTROL);

	/** - Enable SD sampling delay for SD standard timing if clock=0 */
	if (clock == 0)
	{
		set_bit(WPCM450_SDSD,(void *)(WPCM450_INTCR2));
		host->hs_mmc = 0;
		
		goto out;
	}
	
	for (div = 1;div < 256;div *= 2) {
		if ((host->max_clk / div) <= clock)
			break;
	}
	
	/* Update the real clock, 
		meant to correct the calculation for any timing. (chtsai) */
	if (div)
	{
		clock = host->max_clk / div;
	}

	div >>= 1;

	clk = div << SDHCI_DIVIDER_SHIFT;
	clk |= SDHCI_CLOCK_INT_EN;
	
	/** - Set up register for the clock. */
	writew(clk, host->ioaddr + SDHCI_CLOCK_CONTROL);

	/** - Wait until the clock is stable, or max 10 ms */
	timeout = 10;
	while (!((clk = readw(host->ioaddr + SDHCI_CLOCK_CONTROL))
		& SDHCI_CLOCK_INT_STABLE)) {
		if (timeout == 0) {
			printk(KERN_ERR DRIVER_NAME " %s: Internal clock never "
				"stabilised.\n", mmc_hostname(host->mmc));
			sdhci_dumpregs(host);
			return;
		}
		timeout--;
		mdelay(1);
	}

	/** - Enable the clock. */
	clk |= SDHCI_CLOCK_CARD_EN;
	writew(clk, host->ioaddr + SDHCI_CLOCK_CONTROL);

out:
	host->clock = clock;
	
	DBG_V("div = 0x%x, clk = 0x%x, host->clock = %d\n", div, clk, host->clock);
}

/******************************************************************************
*   FUNCTION        :   sdhci_set_power
******************************************************************************/
/**
 *  @brief      This API is to set up power of SDHC for accessed card
 *
 *  @return     None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Internal function\n
 *
 *****************************************************************************/
static void sdhci_set_power(
							 /** A pointer to the host */
							 struct sdhci_host *host,
							 
							 /** The new power setting */
							 unsigned short power
							)
{
	u8 pwr;
	
	DBG_V("host->power = 0x%x, power = 0x%x\n", host->power, power);
	
	/** - If the desired power equals to the power set in host, just return. */
	if (host->power == power)
		return;

	/** - Turn off the power if setting is -1. */
	if (power == (unsigned short)-1) {
		writeb(0, host->ioaddr + SDHCI_POWER_CONTROL);
		goto out;
	}

	/*
	 * Spec says that we should clear the power reg before setting
	 * a new value. Some controllers don't seem to like this though.
	 */
	if (!(host->chip->quirks & SDHCI_QUIRK_SINGLE_POWER_WRITE))
		writeb(0, host->ioaddr + SDHCI_POWER_CONTROL);

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

	/** - Turn on the power with the voltage set. */
	writeb(pwr, host->ioaddr + SDHCI_POWER_CONTROL);

out:
	host->power = power;
}

/*****************************************************************************
 *                                                                           *
 * MMC callbacks                                                             *
 *                                                                           *
 *****************************************************************************/

/******************************************************************************
*   FUNCTION        :   sdhci_request
******************************************************************************/
/**
 *  @brief      This API is to initiate a request to the host
 *
 *  @return     None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Global function\n
 *
 *****************************************************************************/
static void sdhci_request(
						   /** A pointer to the MMC host */
						   struct mmc_host *mmc,
						   
						   /** A pointer to the content of the request */
						   struct mmc_request *mrq
						  )
{
	struct sdhci_host *host;
	unsigned long flags;
	int ret;

	host = mmc_priv(mmc);

	DBG("slot = %d\n", host->slot);

#ifdef CONFIG_MMC_WHOVILLE
	/**	- Get semaphore to access the SDHC exclusively */
	if ((ret=down_interruptible(&host->chip->sem)))
	{
		printk(KERN_ERR " [%s()]: ret = 0x%0x", __func__, ret);
		return;
	}
	
	/**	- Switch to the slot for the host requested */
	sdhci_switch_slot(host);
#endif

	spin_lock_irqsave(&host->lock, flags);

	WARN_ON(host->mrq != NULL);

	sdhci_activate_led(host);

	host->mrq = mrq;

	/** - If the card is not present, return the request with timeout error.
		Otherwise, start the command. */
#ifdef CONFIG_MMC_WHOVILLE
	/* The CARD_PRESENT is only for AMEA. (chtsai) 
	 * If MASER DC is present, it must be booted from it.
	 */
	if ((!MASER_DC_PRESENT() || host->slot != 0) && 
		!(readl(host->ioaddr + SDHCI_PRESENT_STATE) & SDHCI_CARD_PRESENT))
	{
#else
	if (!(readl(host->ioaddr + SDHCI_PRESENT_STATE) & SDHCI_CARD_PRESENT)) {
#endif
		host->mrq->cmd->error = MMC_ERR_TIMEOUT;
		tasklet_schedule(&host->finish_tasklet);
	} else
		sdhci_send_command(host, mrq->cmd);

	mmiowb();
	spin_unlock_irqrestore(&host->lock, flags);
}


/******************************************************************************
*   FUNCTION        :   sdhci_set_ios
******************************************************************************/
/**
 *  @brief      This API is to set up I/O of the host
 *
 *  @return     None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Global function\n
 *
 *****************************************************************************/
static void sdhci_set_ios(
						   /** A pointer to the MMC host */
						   struct mmc_host *mmc,
						   
						   /** A pointer to the data for I/O setup */
						   struct mmc_ios *ios
						  )
{
	struct sdhci_host *host;
	unsigned long flags;
	u8 ctrl;
	int ret;

	host = mmc_priv(mmc);
	
	DBG_V("slot = %d, ios->power_mode=%d ios->clock=%d ios->bus_width=%d\n",
		host->slot, ios->power_mode, ios->clock, ios->bus_width);

#ifdef CONFIG_MMC_WHOVILLE	
	/**	- Get semaphore to access the SDHC exclusively */
	if ((ret=down_interruptible(&host->chip->sem)))
	{
		printk(KERN_ERR " [%s()]: ret = 0x%0x", __func__, ret);
		return;
	}
	
	/**	- Switch to the slot for the host requested */
	sdhci_switch_slot(host);
#endif

	spin_lock_irqsave(&host->lock, flags);

	/** - Reset the chip on each power off. 
		Should clear out any weird states. */
	if (ios->power_mode == MMC_POWER_OFF) {
		writel(0, host->ioaddr + SDHCI_SIGNAL_ENABLE);
		sdhci_init(host);
	}

#if 1 //chtsai
	/** - Set up clock. */
	sdhci_set_clock(host, ios->clock);

	/* Update the real clock back to host, 
		meant to correct the calculation for any timing. (chtsai) */
	ios->clock = host->clock;

#else
	/* For SD MUX, do not turn off clock. (chtsai) */
	if (0 == host->slot || ios->power_mode != MMC_POWER_OFF 
		|| 0 != ios->clock || 0 == host->clock) {
		DBG_V("ios->power_mode=%d ios->clock=%d host->clock=%d\n",
				ios->power_mode, ios->clock, host->clock);
		sdhci_set_clock(host, ios->clock);

		/* Update the real clock back to host, 
			meant to correct the calculation for any timing. (chtsai) */
		ios->clock = host->clock;

	}
#endif
	/** - Set up power. */
	if (ios->power_mode == MMC_POWER_OFF)
		sdhci_set_power(host, -1);
	else
		sdhci_set_power(host, ios->vdd);

	ctrl = readb(host->ioaddr + SDHCI_HOST_CONTROL);

	if (ios->bus_width == MMC_BUS_WIDTH_4)
		ctrl |= SDHCI_CTRL_4BITBUS;
	else
		ctrl &= ~SDHCI_CTRL_4BITBUS;

	/* Workaournd: (chtsai)
	 * The SDHC does not  support High Speed mode!
	 * Since SD runs in 25MHz and MMC in 26MHz, both of them would be in 24MHz.
	 * Thus, no need to enable HS mode.
	 */
#if 0
	if (ios->timing == MMC_TIMING_SD_HS || ios->timing == MMC_TIMING_MMC_HS)
		ctrl |= SDHCI_CTRL_HISPD;
	else
#endif
		ctrl &= ~SDHCI_CTRL_HISPD;

	/** - Set up bus width and speed mode. */
	writeb(ctrl, host->ioaddr + SDHCI_HOST_CONTROL);

	/*
	 * Some (ENE) controllers go apeshit on some ios operation,
	 * signalling timeout and CRC errors even on CMD0. Resetting
	 * it on each ios seems to solve the problem.
	 */
	if(host->chip->quirks & SDHCI_QUIRK_RESET_CMD_DATA_ON_IOS)
		sdhci_reset(host, SDHCI_RESET_CMD | SDHCI_RESET_DATA);

	mmiowb();
	spin_unlock_irqrestore(&host->lock, flags);

#ifdef CONFIG_MMC_WHOVILLE
	/** - Release semaphore */
	up(&host->chip->sem);
#endif
	
	DBG_V("ctrl = 0x%x\n", ctrl);
}

/******************************************************************************
*   FUNCTION        :   sdhci_get_ro
******************************************************************************/
/**
 *  @brief      This API is to get the access status of the card
 *
 *  @return     1: read-only,	0: writable
 *
 *  @warning    None
 *
 *  @note       For Z2, SDIOWP is high when locked. For A1 and after, it's low.
 *
 *  @internal   Function Type: Global function\n
 *
 *****************************************************************************/
static int sdhci_get_ro(
						 /** A pointer to the MMC host */
						 struct mmc_host *mmc
						)
{
	struct sdhci_host *host;
	unsigned long flags;
	int present;
	int ret;

	host = mmc_priv(mmc);

	DBG("slot = %d\n", host->slot);

#ifdef CONFIG_MMC_WHOVILLE
#if 1
	/** - It would always return writable for base MASER. */
	if (host->slot == 0)
#else
	/** - It would return writable for MASER DC. */
	if (IS_MUXED_SYSTEM(host->chip) && (host->slot == 0))
#endif
	{
		return 0;
	}

	/**	- Get semaphore to access the SDHC exclusively */
	if ((ret=down_interruptible(&host->chip->sem)))
	{
		printk(KERN_ERR " [%s()]: ret = 0x%0x", __func__, ret);
		return 1;
	}
	
	/**	- Switch to the slot for the host requested */
	sdhci_switch_slot(host);
#endif

	spin_lock_irqsave(&host->lock, flags);

	/**	- Read the state from the register */
	present = readl(host->ioaddr + SDHCI_PRESENT_STATE);

	spin_unlock_irqrestore(&host->lock, flags);

	DBG_V("present = 0x%x\n", present);
	
#ifdef CONFIG_MMC_WHOVILLE
	/** - Release semaphore */
	up(&host->chip->sem);
#endif
	
	/** - Return access status of the card */
	/* Workaround: the value read from SDIOWP is high when locked!!!
		Thus, invert the value temporarily.
		This is a Hermon bug to be fixed in A1 silicon.
		Also, AMEA card seems to have an extra inverter on the
		write-protect signal. Thus double-invert for Dell platforms.
		For now, ignore write-protect. Always return writable.
	   Also, there is no WP signal for base MASER! (chtsai)
	*/
#ifdef IGNORE_SDWP_SIGNAL
	return (0x00000000);
#else
# ifdef CONFIG_MMC_WINBOND_EVB
	if (host->chip->pdid == WPCM450_PDID_Z2)
	{
		return (present & SDHCI_WRITE_PROTECT);
	}
	else
	{
		return !(present & SDHCI_WRITE_PROTECT);
	}
# else
	/* For AMEA X01 */
	if (host->chip->pdid == WPCM450_PDID_Z2)
	{
		return !(present & SDHCI_WRITE_PROTECT);
	}
	else
	{
		return (present & SDHCI_WRITE_PROTECT);
	}
# endif
#endif
	/* End Workaround */
}

#ifdef CONFIG_MMC_WHOVILLE
/******************************************************************************
*   FUNCTION        :   sdhci_card_detect
******************************************************************************/
/**
 *  @brief      This API is to get card detect status of the card
 *
 *  @return     0: no change,	2: inserted,		3: removed
 *
 *  @warning    None
 *
 *  @note       The card detect signal is only for SD on AMEA card.
 *
 *  @internal   Function Type: Global function\n
 *
 *****************************************************************************/
static int sdhci_card_detect(
							  /** A pointer to the MMC host */
							  struct mmc_host *mmc
							 )
{
	struct sdhci_host *host;
	int ret=0;

	host = mmc_priv(mmc);

	DBG_V("slot = %d\n", host->slot);

	/** - If detect card changes in slot 1, 
			call back for the change by mmc_detect_change(). */
	if (host->chip->hosts[1] && CardChg)
	{
		struct sdhci_host *new_host = host->chip->hosts[1];
		
		DBG_V("-> mmc_detect_change()\n");
		mmc_detect_change(new_host->mmc, msecs_to_jiffies(500));
		ret = CardChg;
		CardChg = 0;
	}
	
	/** - Return the card detect status */
	return ret;
}
#endif


static const struct mmc_host_ops sdhci_ops = {
	.request	= sdhci_request,
	.set_ios	= sdhci_set_ios,
	.get_ro		= sdhci_get_ro,
#ifdef CONFIG_MMC_WHOVILLE
	.card_detect= sdhci_card_detect,
#endif
};

/*****************************************************************************
 *                                                                           *
 * Tasklets                                                                  *
 *                                                                           *
 *****************************************************************************/

/******************************************************************************
*   FUNCTION        :   sdhci_tasklet_card
******************************************************************************/
/**
 *  @brief      This API is the bottom half for handling card detect
 *
 *  @return     None
 *
 *  @warning    None
 *
 *  @note       
 *				- It works for SD card in socket only.
 *				- This is only for EVB.
 *
 *  @internal   Function Type: Internal function\n
 *
 *****************************************************************************/
static void sdhci_tasklet_card(
								/** The parameter for the tasklet */
								unsigned long param
							   )
{
	struct sdhci_host *host;
	unsigned long flags;

	/** - Cast the parameter to sdhci_host for the tasklet. */
	host = (struct sdhci_host*)param;

	spin_lock_irqsave(&host->lock, flags);

	/** - If card is not present, but having a request,
			- reset CMD and DAT lines,
			- set up error code,
			- schedule to finish the request. */
#ifdef CONFIG_MMC_WHOVILLE
	/* The CARD_PRESENT is only for AMEA. (chtsai) 
	 * If MASER DC is present, it must be booted from it.
	 */
	if ((!MASER_DC_PRESENT() || host->slot != 0) && 
		!(readl(host->ioaddr + SDHCI_PRESENT_STATE) & SDHCI_CARD_PRESENT))
	{
#else
	if (!(readl(host->ioaddr + SDHCI_PRESENT_STATE) & SDHCI_CARD_PRESENT)) {
#endif
		if (host->mrq) {
			printk(KERN_ERR DRIVER_NAME " %s: Card removed during transfer!\n",
				mmc_hostname(host->mmc));
			printk(KERN_ERR DRIVER_NAME " %s: Resetting controller.\n",
				mmc_hostname(host->mmc));

			sdhci_reset(host, SDHCI_RESET_CMD);
			sdhci_reset(host, SDHCI_RESET_DATA);

			host->mrq->cmd->error = MMC_ERR_FAILED;
			tasklet_schedule(&host->finish_tasklet);
		}
	}

	spin_unlock_irqrestore(&host->lock, flags);

	/** - Call back for the change by mmc_detect_change(). */
	mmc_detect_change(host->mmc, msecs_to_jiffies(500));
}

/******************************************************************************
*   FUNCTION        :   sdhci_tasklet_finish
******************************************************************************/
/**
 *  @brief      This API is the bottom half for finishing a request
 *
 *  @return     None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Internal function\n
 *
 *****************************************************************************/
static void sdhci_tasklet_finish(
								  /** The parameter for the tasklet */
								  unsigned long param
							     )
{
	struct sdhci_host *host;
	unsigned long flags;
	struct mmc_request *mrq;

	/** - Cast the parameter to sdhci_host for the tasklet. */
	host = (struct sdhci_host*)param;

	DBG_V("slot = %d\n", host->slot);
	
	spin_lock_irqsave(&host->lock, flags);

	/** - Delete the time set up for the command. */
	del_timer(&host->timer);

	mrq = host->mrq;

	/** - Reset CMD and DAT lines if any error. */
	/*
	 * The controller needs a reset of internal state machines
	 * upon error conditions.
	 */
	if ((mrq->cmd->error != MMC_ERR_NONE) ||
		(mrq->data && ((mrq->data->error != MMC_ERR_NONE) ||
		(mrq->data->stop && (mrq->data->stop->error != MMC_ERR_NONE))))) {

		/* Some controllers need this kick or reset won't work here */
		if (host->chip->quirks & SDHCI_QUIRK_CLOCK_BEFORE_RESET) {
			unsigned int clock;

			/* This is to force an update */
			DBG_V("Force a clock update\n");
			clock = host->clock;
			host->clock = 0;
			sdhci_set_clock(host, clock);
		}

		/* Spec says we should do both at the same time, but Ricoh
		   controllers do not like that. */
		sdhci_reset(host, SDHCI_RESET_CMD);
		sdhci_reset(host, SDHCI_RESET_DATA);
	}

	/** - Clear pointers for the request. */
	host->mrq = NULL;
	host->cmd = NULL;
	host->data = NULL;

	sdhci_deactivate_led(host);

	mmiowb();
	spin_unlock_irqrestore(&host->lock, flags);

#ifdef CONFIG_MMC_WHOVILLE
	/** - Release semaphore */
	/* This MUST be done before mmc_request_done() since there might be
		other request coming immediately, or it might cause kernel panic. */
	up(&host->chip->sem);
#endif

	/** - Inform MMC core the request is done by mmc_request_done(). */
	mmc_request_done(host->mmc, mrq);
}

/******************************************************************************
*   FUNCTION        :   sdhci_timeout_timer
******************************************************************************/
/**
 *  @brief      This API is the bottom half for finishing a request
 *
 *  @return     None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Internal function\n
 *
 *****************************************************************************/
static void sdhci_timeout_timer(
								 /** The data for the timer */
								 unsigned long data
								)
{
	struct sdhci_host *host;
	unsigned long flags;

	/** - Cast the data to sdhci_host for the timer. */
	host = (struct sdhci_host*)data;

	DBG_V("slot = %d\n", host->slot);

	spin_lock_irqsave(&host->lock, flags);

	/** - If there is a request pending,
			- dump registers for debugging
			- set up error code to be timeout
			- if there is data, finish the process for data transfer; 
				otherwise, schedule to finish the request. */
	if (host->mrq) {
		printk(KERN_ERR DRIVER_NAME " %s: Timeout waiting for hardware "
			"interrupt.\n", mmc_hostname(host->mmc));
		
		sdhci_dumpregs(host);

		if (host->data) {
			host->data->error = MMC_ERR_TIMEOUT;
			sdhci_finish_data(host);
		} else {
			if (host->cmd)
				host->cmd->error = MMC_ERR_TIMEOUT;
			else
				host->mrq->cmd->error = MMC_ERR_TIMEOUT;

			tasklet_schedule(&host->finish_tasklet);
		}
	}

	mmiowb();
	spin_unlock_irqrestore(&host->lock, flags);
}

/*****************************************************************************
 *                                                                           *
 * Interrupt handling                                                        *
 *                                                                           *
 *****************************************************************************/

/******************************************************************************
*   FUNCTION        :   sdhci_cmd_irq
******************************************************************************/
/**
 *  @brief      This API is to handle interrupts for command
 *
 *  @return     None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Internal function\n
 *
 *****************************************************************************/
static void sdhci_cmd_irq(
						   /** A pointer to the host */
						   struct sdhci_host *host,
						
						   /** Interrupt(s) to be processed */
						   u32 intmask
						  )
{
	BUG_ON(intmask == 0);

	/** - If no command, 
			- show error message,
			- dump registers,
			- reset CMD line, and
			- return. */
	if (!host->cmd) {
		printk(KERN_ERR DRIVER_NAME " %s: Got command interrupt 0x%08x even "
			"though no command operation was in progress.\n",
			mmc_hostname(host->mmc), (unsigned)intmask);
		sdhci_dumpregs(host);

		/* Reset CMD line to clear inhibit bits for later commands. (chtsai)*/
		sdhci_reset(host, SDHCI_RESET_CMD);

		return;
	}

	/** - Set up error code if any error interrupt. */
	if (intmask & SDHCI_INT_TIMEOUT)
		host->cmd->error = MMC_ERR_TIMEOUT;
	else if (intmask & SDHCI_INT_CRC)
		host->cmd->error = MMC_ERR_BADCRC;
	else if (intmask & (SDHCI_INT_END_BIT | SDHCI_INT_INDEX))
		host->cmd->error = MMC_ERR_FAILED;

	/** - If there is error for command, schedule to finish the request. */
	if (host->cmd->error != MMC_ERR_NONE)
		tasklet_schedule(&host->finish_tasklet);
	else if (intmask & SDHCI_INT_RESPONSE)
		/** - Otherwise, finish the command if Command Complete interrupt. */
		sdhci_finish_command(host);
}

/******************************************************************************
*   FUNCTION        :   sdhci_data_irq
******************************************************************************/
/**
 *  @brief      This API is to handle interrupts for data
 *
 *  @return     None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Internal function\n
 *
 *****************************************************************************/
static void sdhci_data_irq(
							/** A pointer to the host */
							struct sdhci_host *host,
							
							/** Interrupt(s) to be processed */
							u32 intmask
						   )
{
	BUG_ON(intmask == 0);

	/** - If there is no data, but no Transfer Complete interrupt,
	 		- show error message,
	 		- dump registers,
	 		- reset DAT line, and
	 		- return */
	if (!host->data) {
		/*
		 * A data end interrupt is sent together with the response
		 * for the stop command.
		 */
		if (intmask & SDHCI_INT_DATA_END)
#if 1 //chtsai
			return;
#else
		{
			/* Reset DAT & CMD for stop command. (chtsai)*/
			/* Would cause timeout for CMD13! (chtsai) */
			sdhci_reset(host, SDHCI_RESET_DATA | SDHCI_RESET_CMD);
			return;
		}
#endif

		printk(KERN_ERR DRIVER_NAME " %s: Got data interrupt 0x%08x even "
			"though no data operation was in progress.\n",
			mmc_hostname(host->mmc), (unsigned)intmask);
		sdhci_dumpregs(host);
#if 1 //chtsai
		DBG_V("Buffer Data Port Register: 0x%08x\n", readl(host->ioaddr + SDHCI_BUFFER));
		/* Reset DAT line to clear inhibit bits for later commands. (chtsai)*/
		sdhci_reset(host, SDHCI_RESET_DATA);
#endif
		return;
	}

	/** - Set up error code if any error interrupt. */
	if (intmask & SDHCI_INT_DATA_TIMEOUT)
		host->data->error = MMC_ERR_TIMEOUT;
	else if (intmask & SDHCI_INT_DATA_CRC)
		host->data->error = MMC_ERR_BADCRC;
	else if (intmask & SDHCI_INT_DATA_END_BIT)
		host->data->error = MMC_ERR_FAILED;

	/** - If there is error during data transferring, finish the data. */
	if (host->data->error != MMC_ERR_NONE)
		sdhci_finish_data(host);
	else {
		/** - Otherwise, transfer the data if data ready or space ready. */
		if (intmask & (SDHCI_INT_DATA_AVAIL | SDHCI_INT_SPACE_AVAIL))
			sdhci_transfer_pio(host);

		/*
		 * We currently don't do anything fancy with DMA
		 * boundaries, but as we can't disable the feature
		 * we need to at least restart the transfer.
		 */
		if (intmask & SDHCI_INT_DMA_END)
			writel(readl(host->ioaddr + SDHCI_DMA_ADDRESS),
				host->ioaddr + SDHCI_DMA_ADDRESS);

		/** - If Transfer Complete interrupt, finish the data. */
		if (intmask & SDHCI_INT_DATA_END) {
			if (host->cmd) {
				/*
				 * Data managed to finish before the
				 * command completed. Make sure we do
				 * things in the proper order.
				 */
				host->data_early = 1;
			} else {
				sdhci_finish_data(host);
			}
		}
	}
}

/******************************************************************************
*   FUNCTION        :   sdhci_irq
******************************************************************************/
/**
 *  @brief      This API is the ISR registered for the SDHC
 *
 *  @return     IRQ_HANDLED: OK,	IRQ_NONE: not processed.
 *
 *  @warning    None
 *
 *  @note       Since there is only one SDHC, 
 					the target host depends on the current slot.
 *
 *  @internal   Function Type: Global function\n
 *
 *****************************************************************************/
static irqreturn_t sdhci_irq(
							  /** IRQ number of the ISR */
							  int irq,
							  
							  /** Parameter for the ISR */
							  void *dev_id
							 )
{
	irqreturn_t result = IRQ_NONE;
	struct sdhci_chip 	*chip=(struct sdhci_chip*) dev_id;
	struct sdhci_host	*host;
	u32 intmask;

	host=chip->hosts[chip->cur_slot];
	spin_lock(&host->lock);

	/** - If the interrupt is not for SDHC, ignore it. */
	if (!test_bit(GPIO_GPIOE7,(void *)(GPIO_REG_EVENT_STATUS_ADDR))) {
		goto out;
	}
	
	intmask = readl(host->ioaddr + SDHCI_INT_STATUS);

	/** - If no interrupt or invalid status, ignore it. */
	if (!intmask || intmask == 0xffffffff) {
		/* Clear the interrupt status */
		set_bit(GPIO_GPIOE7,(void *)(GPIO_REG_EVENT_STATUS_ADDR));

		goto out;
	}

	DBG("*** %s got interrupt: 0x%08x\n", host->slot_descr, intmask);

	/** - For interrupt(s) for card detect,
			- go to bottom half for EVB,
			- for Whoville,
				- control the power of the card,
				- record the state. */
	if (intmask & (SDHCI_INT_CARD_INSERT | SDHCI_INT_CARD_REMOVE)) {
		writel(intmask & (SDHCI_INT_CARD_INSERT | SDHCI_INT_CARD_REMOVE),
			host->ioaddr + SDHCI_INT_STATUS);
		
#ifdef CONFIG_MMC_WINBOND_EVB
		tasklet_schedule(&host->card_tasklet);
#elif defined(CONFIG_MMC_WHOVILLE)
		if (intmask & SDHCI_INT_CARD_INSERT)
		{
			POWER_AMEA_SD_ON();
			CardChg = 2;
		}
		else if (intmask & SDHCI_INT_CARD_REMOVE)
		{
			POWER_AMEA_SD_OFF();
			CardChg = 3;
		}
#endif

	}

	intmask &= ~(SDHCI_INT_CARD_INSERT | SDHCI_INT_CARD_REMOVE);

	/** - For interrupt(s) for command, run sdhci_cmd_irq() */
	if (intmask & SDHCI_INT_CMD_MASK) {
		writel(intmask & SDHCI_INT_CMD_MASK,
			host->ioaddr + SDHCI_INT_STATUS);
		sdhci_cmd_irq(host, intmask & SDHCI_INT_CMD_MASK);
	}

	/** - For interrupt(s) for data, run sdhci_data_irq() */
	if (intmask & SDHCI_INT_DATA_MASK) {
		writel(intmask & SDHCI_INT_DATA_MASK,
			host->ioaddr + SDHCI_INT_STATUS);
		sdhci_data_irq(host, intmask & SDHCI_INT_DATA_MASK);
	}

	intmask &= ~(SDHCI_INT_CMD_MASK | SDHCI_INT_DATA_MASK);

	intmask &= ~SDHCI_INT_ERROR;

	/** - Show error message for the other interrupt(s). */
	if (intmask & SDHCI_INT_BUS_POWER) {
		printk(KERN_ERR DRIVER_NAME " %s: Card is consuming too much power!\n",
			mmc_hostname(host->mmc));
		writel(SDHCI_INT_BUS_POWER, host->ioaddr + SDHCI_INT_STATUS);
	}

	intmask &= ~SDHCI_INT_BUS_POWER;

	if (intmask) {
		printk(KERN_ERR DRIVER_NAME " %s: Unexpected interrupt 0x%08x.\n",
			mmc_hostname(host->mmc), intmask);
		sdhci_dumpregs(host);

		writel(intmask, host->ioaddr + SDHCI_INT_STATUS);
	}

	result = IRQ_HANDLED;
	
	/** - Clear the interrupt status */
	set_bit(GPIO_GPIOE7,(void *)(GPIO_REG_EVENT_STATUS_ADDR));


	mmiowb();

out:
	spin_unlock(&host->lock);

	/** - Return the result */
	return result;
}

/*****************************************************************************
 *                                                                           *
 * Suspend/resume                                                            *
 *                                                                           *
 *****************************************************************************/

#ifdef CONFIG_PM

/******************************************************************************
*   FUNCTION        :   sdhci_suspend
******************************************************************************/
/**
 *  @brief      This API is to suspend the platform device
 *
 *  @return     0: OK,	otherwise: failed.
 *
 *  @warning    None
 *
 *  @note       Since there is only one SDHC, 
 					some H/W related processes would be done once only.
 *
 *  @internal   Function Type: Global function\n
 *
 *****************************************************************************/
static int sdhci_suspend(
						  /** A pointer to the platform device */
						  struct platform_device *pdev,
						  
						  /** Suspend state */
						  pm_message_t state
						 )
{
	struct sdhci_chip *chip;
	int i, ret;

	/** - Get the chip from the device's driver data. */
	chip = platform_get_drvdata(pdev);
	if (!chip)
		return 0;

	DBG("Suspending...\n");

	/** - For each slot of the chip, 
			- suspend the MMC host in the slot,
			- restore all have done if error */
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

	/** - Return result */
	return 0;
}

/******************************************************************************
*   FUNCTION        :   sdhci_resume
******************************************************************************/
/**
 *  @brief      This API is to resume the platform device
 *
 *  @return     0: OK,	otherwise: failed.
 *
 *  @warning    None
 *
 *  @note       Since there is only one SDHC, 
 					some H/W related processes would be done once only.
 *
 *  @internal   Function Type: Global function\n
 *
 *****************************************************************************/
static int sdhci_resume(
						 /** A pointer to the platform device */
						 struct platform_device *pdev
						)
{
	struct sdhci_chip *chip;
	int i, ret;

	/** - Get the chip from the device's driver data. */
	chip = platform_get_drvdata(pdev);
	if (!chip)
		return 0;

	DBG("Resuming...\n");

	/** - For each slot of the chip, 
			- reset the SDHC
			- resume the MMC host in the slot. */
	for (i = 0;i < chip->num_slots;i++) {
		if (!chip->hosts[i])
			continue;
		if (chip->hosts[i]->flags & SDHCI_USE_DMA)
			;

		if (0 == i)
		{
			sdhci_init(chip->hosts[i]);
		}

		mmiowb();
		ret = mmc_resume_host(chip->hosts[i]->mmc);
		if (ret)
			return ret;
	}

	/** - Return result */
	return 0;
}

#else /* CONFIG_PM */

#define sdhci_suspend NULL
#define sdhci_resume NULL

#endif /* CONFIG_PM */

/*****************************************************************************
 *                                                                           *
 * Device probing/removal                                                    *
 *                                                                           *
 *****************************************************************************/

/******************************************************************************
*   FUNCTION        :   sdhci_probe_slot
******************************************************************************/
/**
 *  @brief      This API is to probe a slot
 *
 *  @return     0: OK,	otherwise: failed.
 *
 *  @warning    None
 *
 *  @note       Since there is only one SDHC, 
 					some H/W related processes would be done once only.
 *
 *  @internal   Function Type: Internal function\n
 *
 *****************************************************************************/
static int __devinit sdhci_probe_slot(
							   		   /** A pointer to the platform device */
							   		   struct platform_device *pdev,
							   
							   		   /** Index of the slot */
							   		   int slot
									  )
{
	int ret;
	unsigned int version;
	struct sdhci_chip *chip;
	struct mmc_host *mmc;
	struct sdhci_host *host;

	unsigned int caps=0;

	DBG("slot=%d\n", slot);

	/** - Get the chip from the device's driver data. */
	chip = platform_get_drvdata(pdev);
	BUG_ON(!chip);

	/** - Allocate a MMC host for the device. */
	mmc = mmc_alloc_host(sizeof(struct sdhci_host), &pdev->dev);
	if (!mmc)
	{
		ret = -ENOMEM;
		printk(KERN_ERR DRIVER_NAME " mmc_alloc_host error\n");
		return ret;
	}
	
	host = mmc_priv(mmc);
	host->slot = slot;
	host->mmc = mmc;
	snprintf(host->slot_descr, 20, "sdhci:slot%d", slot);
	
	host->chip = chip;
	chip->hosts[slot] = host;

	if (0 == slot)
	{
		/** - Get memory resource of the device, request and remap it. */
		host->mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
		if (!host->mem) {
			printk(KERN_ERR DRIVER_NAME " failed to get io memory region resouce.\n");
			ret = -ENOENT;
			goto free;
		}

		host->mem = request_mem_region(host->mem->start,
									   RESSIZE(host->mem), pdev->name);
		if (!host->mem) {
			printk(KERN_ERR DRIVER_NAME " failed to request io memory region.\n");
			ret = -ENOENT;
			goto free;
		}
		
		host->ioaddr = ioremap_nocache(host->mem->start, RESSIZE(host->mem));
		if (host->ioaddr == 0) {
			printk(KERN_ERR DRIVER_NAME " failed to ioremap_nocache() io memory region.\n");
			ret = -EINVAL;
			goto release;
		}
		
		host->irq = platform_get_irq(pdev, 0);
		if (host->irq == 0) {
			printk(KERN_ERR DRIVER_NAME " failed to get interrupt resouce.\n");
			ret = -EINVAL;
			goto unmap;
		}
		
		/** - Reset the SDHC. */
		sdhci_reset(host, SDHCI_RESET_ALL);

		/** - Read version and capabilities of the SDHC. */
		version = readw(host->ioaddr + SDHCI_HOST_VERSION);
		version = (version & SDHCI_SPEC_VER_MASK) >> SDHCI_SPEC_VER_SHIFT;
		DBG_V("version = %d\n", version);
		if (version > 1) {
			printk(KERN_ERR DRIVER_NAME " %s: Unknown controller version (%d). "
				"You may experience problems.\n", host->slot_descr,
				version);
		}
		caps = readl(host->ioaddr + SDHCI_CAPABILITIES);
//		if ((caps & SDHCI_CAN_DO_DMA) && ((pdev->class & 0x0000FF) == 0x01))
//			host->flags |= SDHCI_USE_DMA;
	
		if (host->flags & SDHCI_USE_DMA) {
			/* Do something! */
		}

		host->max_clk = (caps & SDHCI_CLOCK_BASE_MASK)>>SDHCI_CLOCK_BASE_SHIFT;
		if (0 == host->max_clk) {
			printk(KERN_ERR DRIVER_NAME " %s: Hardware doesn't specify base clock "
				"frequency.\n", host->slot_descr);
			ret = -ENODEV;
			goto unmap;
		}
		host->max_clk *= 1000000;

		host->timeout_clk =
			(caps & SDHCI_TIMEOUT_CLK_MASK) >> SDHCI_TIMEOUT_CLK_SHIFT;
		if (0 == host->timeout_clk) {
			printk(KERN_ERR DRIVER_NAME " %s: Hardware doesn't specify timeout clock "
				"frequency.\n", host->slot_descr);
			ret = -ENODEV;
			goto unmap;
		}
		if (caps & SDHCI_TIMEOUT_CLK_UNIT)
			host->timeout_clk *= 1000;
		
		DBG_V("caps = 0x%x, host->max_clk = %d, host->timeout_clk = %d\n", 
			caps, host->max_clk, host->timeout_clk);

		/** - Set MMC host parameters according to device's capabilities. */
		mmc->ops = &sdhci_ops;
		mmc->f_min = host->max_clk / 256;
		mmc->f_max = host->max_clk;
		
		/* The SDHC can work up to 24MHz only, so no SD high speed mode,
		 *	but MMC still could work in high speed mode, i.e. >20MHz. 
		 *	MMC_CAP_BYTEBLOCK has obsoleted. (chtsai) */
#if 0
		mmc->caps = MMC_CAP_4_BIT_DATA | MMC_CAP_MULTIWRITE | MMC_CAP_BYTEBLOCK;
		if (caps & SDHCI_CAN_DO_HISPD)
			mmc->caps |= MMC_CAP_SD_HIGHSPEED;
#else
		mmc->caps = MMC_CAP_4_BIT_DATA | MMC_CAP_MULTIWRITE;
		if (caps & SDHCI_CAN_DO_HISPD)
			mmc->caps |= MMC_CAP_MMC_HIGHSPEED;
#endif
		DBG("mmc->caps = 0x%lx\n", mmc->caps);
		
		mmc->ocr_avail = 0;
		if (caps & SDHCI_CAN_VDD_330)
			mmc->ocr_avail |= MMC_VDD_32_33|MMC_VDD_33_34;
		if (caps & SDHCI_CAN_VDD_300)
			mmc->ocr_avail |= MMC_VDD_29_30|MMC_VDD_30_31;
		if (caps & SDHCI_CAN_VDD_180)
			mmc->ocr_avail |= MMC_VDD_165_195;
	
		if (mmc->ocr_avail == 0)
		{
			printk(KERN_ERR DRIVER_NAME " %s: Hardware doesn't report any "
				"support voltages.\n", host->slot_descr);
			ret = -ENODEV;
			goto unmap;
		}
		
#ifdef CONFIG_MMC_WHOVILLE
		/** - Control the power of SD card on the AMEA card. */
		if (readl(host->ioaddr + SDHCI_PRESENT_STATE) & SDHCI_CARD_PRESENT)
		{
			POWER_AMEA_SD_ON();
		}
		else
		{
			POWER_AMEA_SD_OFF();
		}
#endif		
	} /* if (0 == slot) */
	else
	{
		DBG_V("Copy data for 2nd host\n");
		/* Copy the data from host 0 */
		host->mem = chip->hosts[0]->mem;
		host->ioaddr = chip->hosts[0]->ioaddr;
		host->irq = chip->hosts[0]->irq;
		host->max_clk = chip->hosts[0]->max_clk;
		host->timeout_clk = chip->hosts[0]->timeout_clk;
		mmc->ops = chip->hosts[0]->mmc->ops;
		mmc->f_min = chip->hosts[0]->mmc->f_min;
		mmc->f_max = chip->hosts[0]->mmc->f_max;
		mmc->ocr_avail = chip->hosts[0]->mmc->ocr_avail;
		mmc->caps = chip->hosts[0]->mmc->caps;
	}
	
	if (host->flags & SDHCI_USE_DMA) {
		/* To be implemented */
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
		printk(KERN_WARNING DRIVER_NAME " %s: Invalid maximum block size, assuming 512\n",
			host->slot_descr);
		mmc->max_blk_size = 512;
	} else
		mmc->max_blk_size = 512 << mmc->max_blk_size;

	/*
	 * Maximum block count.
	 */
#ifdef FIX_TIMEOUT_PROBLEM
	/* To work around for WPCM450 Z2 timeout issue, 
	 *	limit the block count to 6 as suggested in errata. (chtsai) */
	if (chip->pdid == WPCM450_PDID_Z2)
		mmc->max_blk_count = 6;
	else
#endif
#ifndef FIX_WAIT_FOR_INHIBIT_BITS
	mmc->max_blk_count = 65535;
#else
	/* To work around WPCM450 Inhibit Bit (CMD) issue, 
	 *	limit the block count to make sure the time needed to wait for 
	 *	the card's busy for next command. (chtsai) */
	mmc->max_blk_count = 400;
#endif

	/** - Init tasklets. */
	tasklet_init(&host->card_tasklet,
		sdhci_tasklet_card, (unsigned long)host);
	tasklet_init(&host->finish_tasklet,
		sdhci_tasklet_finish, (unsigned long)host);

	/** - Set up a timer. */
	setup_timer(&host->timer, sdhci_timeout_timer, (unsigned long)host);

	/** - Request the IRQ and initialize the SDHC. */
	if (0 == slot)
	{
		DBG("Request irq %d\n", host->irq);
		ret = request_irq(host->irq, sdhci_irq, IRQF_SHARED|IRQF_SAMPLE_RANDOM, DRIVER_NAME, chip);
		if (ret)
			goto untasklet;
	
		sdhci_init(host);
		
#ifdef CONFIG_MMC_DEBUG
		sdhci_dumpregs(host);
#endif
	} /* if (0 == slot) */

	mmiowb();

	/** - Add the MMC host. */
	if ((ret = mmc_add_host(mmc)))
	{
		printk(KERN_ERR DRIVER_NAME " failed to add mmc host. ret=0x%0x\n", ret);
		goto untasklet;
	}
	
	printk(KERN_INFO "%s: SDHCI at slot %d irq %d %s\n", mmc_hostname(mmc),
		host->slot, host->irq,
		(host->flags & SDHCI_USE_DMA)?"DMA":"PIO");

	/** - Return 0 if no error. */
	return 0;

untasklet:
	/** - If any error, restore all have done for the slot. */
	tasklet_kill(&host->card_tasklet);
	tasklet_kill(&host->finish_tasklet);
unmap:
	iounmap(host->ioaddr);
release:
	release_mem_region(host->mem->start, RESSIZE(host->mem));
free:
	host->chip = 0;
	chip->hosts[slot] = 0;
	mmc_free_host(mmc);

	/** - Return error code. */
	return ret;
}

/******************************************************************************
*   FUNCTION        :   sdhci_remove_slot
******************************************************************************/
/**
 *  @brief      This API is to remote a slot
 *
 *  @return     None
 *
 *  @warning    None
 *
 *  @note       Since there is only one SDHC, 
 					some H/W related processes would be done once only.
 *
 *  @internal   Function Type: Internal function\n
 *
 *****************************************************************************/
static void sdhci_remove_slot( 
							   /** A pointer to the platform device */
							   struct platform_device *pdev,
							   
							   /** Index of the slot */
							   int slot
							  )
{
	struct sdhci_chip *chip;
	struct mmc_host *mmc;
	struct sdhci_host *host;

	DBG("slot=%d\n", slot);
	
	/** - Get information from the device's driver data. */
	chip = platform_get_drvdata(pdev);
	host = chip->hosts[slot];
	mmc = host->mmc;

	/** - Clear the pointer for the host of the slot. */
	chip->hosts[slot] = NULL;

	/** - Remove the MMC host. */
	mmc_remove_host(mmc);

	/** - Reset the host and free IRQ. */
	if (0 == slot)
	{
		sdhci_reset(host, SDHCI_RESET_ALL);

		free_irq(host->irq, chip);
	}

	/** - Delete the timer. */
	del_timer_sync(&host->timer);

	/** - Delete the tasklets. */
	tasklet_kill(&host->card_tasklet);
	tasklet_kill(&host->finish_tasklet);

	/** - Unmap and release memory region allocated. */
	if (0 == slot)
	{
		iounmap(host->ioaddr);

		release_mem_region(host->mem->start, RESSIZE(host->mem));
	}

	/** - Free the MMC host. */
	mmc_free_host(mmc);
}

#ifdef CONFIG_MMC_WHOVILLE
/******************************************************************************
*   FUNCTION        :   sdhci_muxed_system
******************************************************************************/
/**
 *  @brief      This API is to get the status of muxing
 *
 *  @return     0: non muxed system, 	1, muxed system (default)
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Internal function\n
 *
 *****************************************************************************/
static int sdhci_muxed_system(void)
{
#define	ADDRESS_OF_LOADER_VARS	0x40030000
#define SIZE_OF_LOADER_VARS		0x4000
#define	BOOT_VAR_NAME			"fwudev="
#define	BOOT_DEV_VAR			BOOT_VAR_NAME "amea"
#define	LEN_OF_BOOT_VAR_NAME	strlen(BOOT_VAR_NAME)
#define	LEN_OF_BOOT_DEV_VAR		strlen(BOOT_DEV_VAR)

	int	ret=1;
	struct resource		*tmp_mem;
	void __iomem 		*tmp_ioaddr;
	unsigned char 		MuxedVar[LEN_OF_BOOT_DEV_VAR+1]=BOOT_DEV_VAR;
	int	bufOffset=0;
	int	iVarIndex=0;

	/** - Request the memory region for AMEA present */
	tmp_mem = request_mem_region(ADDRESS_OF_LOADER_VARS, 
								 SIZE_OF_LOADER_VARS, DRIVER_NAME);
	if (!tmp_mem) {
		printk(KERN_ERR "%s: failed to request io memory region 0x%x.\n", 
				__FUNCTION__, ADDRESS_OF_LOADER_VARS);
		return ret;
	}

	/** - Remap the memory region */
	tmp_ioaddr = ioremap_nocache(ADDRESS_OF_LOADER_VARS, SIZE_OF_LOADER_VARS);
	if (0 == tmp_ioaddr) {
		release_mem_region(ADDRESS_OF_LOADER_VARS, SIZE_OF_LOADER_VARS);
		printk(KERN_ERR "%s: failed to ioremap_nocache() io memory region.\n", 
				__FUNCTION__);
		return ret;
	}

	/** - Search for the variable defined for non-muxed system. */
	while (bufOffset < SIZE_OF_LOADER_VARS)
	{
		if (iVarIndex < LEN_OF_BOOT_VAR_NAME)
		{
			/* Search for the variable name first. */
			if (ioread8(tmp_ioaddr+bufOffset) == MuxedVar[iVarIndex])
				iVarIndex++;
			else
				iVarIndex = 0;
		}
		else if (ioread8(tmp_ioaddr+bufOffset) == MuxedVar[iVarIndex])
		{
			if (++iVarIndex == LEN_OF_BOOT_DEV_VAR)
			{
				/* Found the variable, so it is not muxed. */
				ret = 0;
				break;
			}
		}
		else
		{
			/* Device not match, just return. */
			break;
		}
		
		bufOffset++;
	}
	
	/** - Unmap the memory region */
	iounmap(tmp_ioaddr);
	
	/** - Release the memory region */
	release_mem_region(ADDRESS_OF_LOADER_VARS, SIZE_OF_LOADER_VARS);

	/** - Return the status */
	return ret;
}


/******************************************************************************
*   FUNCTION        :   sdhci_AMEA_present
******************************************************************************/
/**
 *  @brief      This API is to get the status of AMEA present
 *
 *  @return     0: not present, 	1, present
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Internal function\n
 *
 *****************************************************************************/
static int sdhci_AMEA_present(void)
{
#define	ADDRESS_OF_AMEA_PREN_N	0xC4000012
#define	BIT_OF_AMEA_PREN_N		(1<<1)
#define LEN_OF_AMEA_PREN_N		1

	int	ret=0;
	struct resource		*tmp_mem;
	void __iomem 		*tmp_ioaddr;

	/** - Request the memory region for AMEA present */
	tmp_mem = request_mem_region(ADDRESS_OF_AMEA_PREN_N, 
								 LEN_OF_AMEA_PREN_N, DRIVER_NAME);
	if (!tmp_mem) {
		printk(KERN_ERR "%s: failed to request io memory region 0x%x.\n", 
				__FUNCTION__, ADDRESS_OF_AMEA_PREN_N);
		return 0;
	}

	/** - Remap the memory region */
	tmp_ioaddr = ioremap_nocache(ADDRESS_OF_AMEA_PREN_N, LEN_OF_AMEA_PREN_N);
	if (0 == tmp_ioaddr) {
		release_mem_region(ADDRESS_OF_AMEA_PREN_N, LEN_OF_AMEA_PREN_N);
		printk(KERN_ERR "%s: failed to ioremap_nocache() io memory region.\n", 
				__FUNCTION__);
		return 0;
	}
	
	DBG_V("AMEA_PRSN_N: 0x%x\n", ioread8(tmp_ioaddr) & BIT_OF_AMEA_PREN_N);
	/** - Read the status */
	if (!(ioread8(tmp_ioaddr) & BIT_OF_AMEA_PREN_N))
	{
		ret = 1;
	}

	/** - Unmap the memory region */
	iounmap(tmp_ioaddr);
	
	/** - Release the memory region */
	release_mem_region(ADDRESS_OF_AMEA_PREN_N, LEN_OF_AMEA_PREN_N);

	/** - Return the status */
	return ret;
}
#endif


/******************************************************************************
*   FUNCTION        :   sdhci_probe
******************************************************************************/
/**
 *  @brief      This API is to probe slots of the platform device
 *
 *  @return     0: OK, 	otherwise, failed
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Global function\n
 *
 *****************************************************************************/
static int __devinit sdhci_probe(
								  /** A pointer to the platform device */
								  struct platform_device *pdev
								 )
{
	int ret=0;
	int i;
	struct sdhci_chip *chip;

	BUG_ON(pdev == NULL);

	DBG_V("enter\n");
	
	/** - Allocate memory for the chip */
	chip = kzalloc(sizeof(struct sdhci_chip) + sizeof(struct sdhci_host*) * 2, 
				   GFP_KERNEL);
	if (!chip) {
		printk(KERN_ERR DRIVER_NAME " kzalloc error\n");
		ret = -ENOMEM;
		goto err;
	}

	/** - Set up data for the chip */
	chip->pdev = pdev;
	chip->quirks = 0;
	chip->pdid = readl(WPCM450_PDID);

#ifdef CONFIG_MMC_WHOVILLE
	/** - Initialize MUX */
	init_MUTEX(&chip->sem);
	
	if (sdhci_muxed_system())
	{
		SET_MUXED_SYSTEM(chip);
	}
	
	if (IS_MUXED_SYSTEM(chip) && sdhci_AMEA_present())
	{
		chip->num_slots = 2;
		EN_AMEA_SD_POWER_CTRL();
	}
	else
#endif	
	{
		chip->num_slots = 1;
	}
	chip->cur_slot = 0;

	if (debug_quirks)
		chip->quirks = debug_quirks;

	/** - Set up the chip as the driver data of the device */
	platform_set_drvdata(pdev, chip);

	/** - For each slot of the chip, probe host in the slot.
			- If there is any error, restore all have done.
		 	- Sleep 3 seconds after probing. */
	for (i = 0; i < chip->num_slots; i++)
	{
		ret = sdhci_probe_slot(pdev, i);
		if (ret)
		{
			printk(KERN_ERR DRIVER_NAME " sdhci_probe_slot error, slot %d\n", i);
			for (i--;i >= 0;i--)
			{
				sdhci_remove_slot(pdev, i);
			}
			goto free;
		}
		
		msleep(3000);
	}

	/** - Return result */
	return ret;

free:
	platform_set_drvdata(pdev, NULL);
	kfree(chip);

err:
	return ret;
}

/******************************************************************************
*   FUNCTION        :   sdhci_remove
******************************************************************************/
/**
 *  @brief      This API is to remove slots of the platform device
 *
 *  @return     0: OK, 	otherwise, failed
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Global function\n
 *
 *****************************************************************************/
static int __devexit sdhci_remove(
								   /** A pointer to the platform device */
								   struct platform_device *pdev
								  )
{
	int i;
	struct sdhci_chip *chip;

	DBG_V("enter\n");
	
	/** - Get the chip from the device's driver data. */
	chip = platform_get_drvdata(pdev);

	if (chip)
	{
		/** - For each slot of the chip, remove host in the slot. */
		for (i = (chip->num_slots-1); i>=0 ; i--)
		{
			sdhci_remove_slot(pdev, i);
		}

		/** - Clear driver data of the device. */
		platform_set_drvdata(pdev, NULL);

		/** - Free the chip. */
		kfree(chip);
	}
	
	/** - Return OK. */
	return 0;
}


/*****************************************************************************
 *                                                                           *
 * Hardware init                                                             *
 *                                                                           *
 *****************************************************************************/

/******************************************************************************
*   FUNCTION        :   sdhci_module_init
******************************************************************************/
/**
 *  @brief      This API is to initialize registers to enable SDHC
 *
 *  @return     None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Internal function\n
 *
 *****************************************************************************/
static void sdhci_module_init(void)
{
	DBG_V("enter\n");
	
	/** - Enable SDIO */
	set_bit(WPCM450_SDIOSEL,(void *)(WPCM450_GCR_MFSEL1));

	/** - Enable SD sampling delay for SD standard timing */
	set_bit(WPCM450_SDSD,(void *)(WPCM450_INTCR2));

	/** - Enable SDIO clock, CLKEN/SDIO=1 */
	set_bit(WPCM450_CLKEN_SDIO,(void *)(WPCM450_CLKEN));

#ifdef CONFIG_MMC_WINBOND_EVB
	/** Init MUX */
	SD_MUX_INIT();
			
	SELECT_MMC_SLOT(1);
	
#endif

	/** - SD Card Interface - Set/clear Software Reset Control Bit */
	set_bit(WPCM450_IPSRST_SDIO,(void *)(WPCM450_CLKIPSRST));
	msleep(1);
	clear_bit(WPCM450_IPSRST_SDIO,(void *)(WPCM450_CLKIPSRST));
			
	/** - Set IRQ type */
	clear_bit(GPIO_GPIOE7,(void *)(GPIO_REG_EVENT_ENABLE_ADDR));
	set_bit(GPIO_GPIOE7,(void *)(GPIO_REG_EVENT_TYPE_ADDR));
	set_bit(GPIO_GPIOE7,(void *)(GPIO_REG_EVENT_POLARITY_ADDR));

	/** - Clear interrupt status */
	set_bit(GPIO_GPIOE7,(void *)(GPIO_REG_EVENT_STATUS_ADDR));
	
	/** - Enable interrupt */
	set_bit(GPIO_GPIOE7,(void *)(GPIO_REG_EVENT_ENABLE_ADDR));

	DBG_V("@GPIO_REG_EVENT_STATUS_ADDR = 0x%0x\n", 
			readl(GPIO_REG_EVENT_STATUS_ADDR));
}


/*****************************************************************************
 *                                                                           *
 * Device Declaration                                                        *
 *                                                                           *
 *****************************************************************************/

static void mmc_device_release(struct device *dev)
{
	/* normally not freed */
}

static struct resource wpcm450_mmc_resources[] = {
	[0] = {
		.start	= WPCM450_PA_SDHC,
		.end	= WPCM450_PA_SDHC + WPCM450_SZ_SDHC - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_GROUP1,
		.end	= IRQ_GROUP1,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device wpcm450_mmc_device = {
	.name		= DRIVER_NAME,
	.id			= -1,
	.dev		= {
//				.dma_mask		= &mmc_dmamask,
				.coherent_dma_mask	= 0xffffffff,
//				.platform_data		= &mmc_data,
				.release			= mmc_device_release,
	},
	.resource	= wpcm450_mmc_resources,
	.num_resources	= ARRAY_SIZE(wpcm450_mmc_resources),
};


/*****************************************************************************
 *                                                                           *
 * Driver Declaration                                                        *
 *                                                                           *
 *****************************************************************************/

static struct platform_driver wpcm450_sdhci_driver =
{
	.driver		= {
        .name	= DRIVER_NAME,
		.owner	= THIS_MODULE,
	},
    .probe  	= sdhci_probe,
	.remove 	= sdhci_remove,
	.suspend	= sdhci_suspend,
	.resume		= sdhci_resume,
};


/******************************************************************************
*   FUNCTION        :   aess_sdhci_drv_init
******************************************************************************/
/**
 *  @brief      This API is to initialize SDHC driver
 *
 *  @return     The status of initialization (0: OK, otherwise: failed)
 *
 *  @warning    None
 *
 *  @note       This must be done after MMC core and MMC block initialized.
 *
 *  @internal   Function Type: Global function\n
 *
 *****************************************************************************/
static int __init aess_sdhci_drv_init(void)
{
	int result=0;
	int pdid;

	printk(KERN_INFO DRIVER_NAME ": " DRIVER_DESCRIPTION " " DRIVER_VERSION "\n");

	/** - Verify H/W version. If it is Z1, it does not support SDHC. */
	pdid = readl(WPCM450_PDID);
	DBG_V("PDID=0x%x\n", pdid);
	if (pdid == WPCM450_PDID_Z1)
	{
		printk(KERN_INFO DRIVER_NAME ": H/W is not supported!!! PDID=0x%x\n", pdid);
		return EIO;
	}
	
	/** - Initialize registers for SDHC */
	sdhci_module_init();
	
	/** - Register the platform device driver. */
	if ((result=platform_driver_register(&wpcm450_sdhci_driver)))
	{
		printk(KERN_ERR DRIVER_NAME " platform_driver_register error, rc=%d\n", result);
		return result;
	}
	
	/* When move this ahead, segamentation fault! */
	/** - Register the platform device. */
	if ((result=platform_device_register(&wpcm450_mmc_device)))
	{
		printk(KERN_ERR DRIVER_NAME " platform_device_register error, rc=%d\n", result);
		platform_device_put(&wpcm450_mmc_device);
		platform_driver_unregister(&wpcm450_sdhci_driver);
		return result;
	}
	
	return result;
}

/******************************************************************************
*   FUNCTION        :   aess_sdhci_drv_exit
******************************************************************************/
/**
 *  @brief      This API is to terminate the SDHC driver
 *
 *  @return     None
 *
 *  @warning    None
 *
 *  @note       This must be done before MMC core and block's unregistration.
 *
 *  @internal   Function Type: Global function\n
 *
 *****************************************************************************/
static void __exit aess_sdhci_drv_exit(void)
{
	DBG_V("enter\n");

	/** - Disable interrupt */
	clear_bit(GPIO_GPIOE7,(void *)(GPIO_REG_EVENT_ENABLE_ADDR));

	/** - Un-register the platform device and driver */
	platform_device_unregister(&wpcm450_mmc_device);
	platform_driver_unregister(&wpcm450_sdhci_driver);
	
	/** - Disable SDIO */
	clear_bit(WPCM450_SDIOSEL,(void *)(WPCM450_GCR_MFSEL1));

	/** - Clear interrupt status */
	set_bit(GPIO_GPIOE7,(void *)(GPIO_REG_EVENT_STATUS_ADDR));
	
}

module_init(aess_sdhci_drv_init);
module_exit(aess_sdhci_drv_exit);

module_param(debug_quirks, uint, 0444);	/* read only */
module_param(debug_level, uint, 0644);	/* owner: read-write, others: read */
module_param(version, charp, 0444);		/* read only */

/* The following declarations work only for modules. */

MODULE_PARM_DESC(debug_quirks, "Force certain quirks.");
MODULE_PARM_DESC(debug_level, "Debug message level.");
MODULE_PARM_DESC(version, "Driver version.");

MODULE_AUTHOR("Pierre Ossman <drzeus@drzeus.cx>");
MODULE_DESCRIPTION(DRIVER_DESCRIPTION);
MODULE_LICENSE("GPL");
MODULE_VERSION(DRIVER_VERSION);

