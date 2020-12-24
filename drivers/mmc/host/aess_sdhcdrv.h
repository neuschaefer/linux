/*
 * linux/drivers/mmc/host/aess_sdhcdrv.h - 
 *
 *	Secure Digital Host Controller Interface driver for WPCM450
 *
 * Copyright &copy; 2007-2008 Avocent Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */


/*
 * Controller registers
 */

#define SDHCI_DMA_ADDRESS	0x00

#define SDHCI_BLOCK_SIZE	0x04
#define  SDHCI_MAKE_BLKSZ(dma, blksz) (((dma & 0x7) << 12) | (blksz & 0xFFF))

#define SDHCI_BLOCK_COUNT	0x06

#define SDHCI_ARGUMENT		0x08

#define SDHCI_TRANSFER_MODE	0x0C
#define  SDHCI_TRNS_DMA		0x01
#define  SDHCI_TRNS_BLK_CNT_EN	0x02
#define  SDHCI_TRNS_ACMD12	0x04
#define  SDHCI_TRNS_READ	0x10
#define  SDHCI_TRNS_MULTI	0x20

#define SDHCI_COMMAND		0x0E
#define  SDHCI_CMD_RESP_MASK	0x03
#define  SDHCI_CMD_CRC		0x08
#define  SDHCI_CMD_INDEX	0x10
#define  SDHCI_CMD_DATA		0x20

#define  SDHCI_CMD_RESP_NONE	0x00
#define  SDHCI_CMD_RESP_LONG	0x01
#define  SDHCI_CMD_RESP_SHORT	0x02
#define  SDHCI_CMD_RESP_SHORT_BUSY 0x03

#define SDHCI_MAKE_CMD(c, f) (((c & 0xff) << 8) | (f & 0xff))

#define SDHCI_RESPONSE		0x10

#define SDHCI_BUFFER		0x20

#define SDHCI_PRESENT_STATE	0x24
#define  SDHCI_CMD_INHIBIT	0x00000001
#define  SDHCI_DATA_INHIBIT	0x00000002
#define  SDHCI_DOING_WRITE	0x00000100
#define  SDHCI_DOING_READ	0x00000200
#define  SDHCI_SPACE_AVAILABLE	0x00000400
#define  SDHCI_DATA_AVAILABLE	0x00000800
#define  SDHCI_CARD_PRESENT	0x00010000
#define  SDHCI_WRITE_PROTECT	0x00080000

#define SDHCI_HOST_CONTROL 	0x28
#define  SDHCI_CTRL_LED		0x01
#define  SDHCI_CTRL_4BITBUS	0x02
#define  SDHCI_CTRL_HISPD	0x04
#define  SDHCI_TEST_LEVEL_CARD_INSERTED	(1<<6)
#define  SDHCI_SELECT_CD_TEST_LEVEL		(1<<7)

#define SDHCI_POWER_CONTROL	0x29
#define  SDHCI_POWER_ON		0x01
#define  SDHCI_POWER_180	0x0A
#define  SDHCI_POWER_300	0x0C
#define  SDHCI_POWER_330	0x0E

#define SDHCI_BLOCK_GAP_CONTROL	0x2A

#define SDHCI_WAKE_UP_CONTROL	0x2B

#define SDHCI_CLOCK_CONTROL	0x2C
#define  SDHCI_DIVIDER_SHIFT	8
#define  SDHCI_CLOCK_CARD_EN	0x0004
#define  SDHCI_CLOCK_INT_STABLE	0x0002
#define  SDHCI_CLOCK_INT_EN	0x0001

#define SDHCI_TIMEOUT_CONTROL	0x2E

#define SDHCI_SOFTWARE_RESET	0x2F
#define  SDHCI_RESET_ALL	0x01
#define  SDHCI_RESET_CMD	0x02
#define  SDHCI_RESET_DATA	0x04

#define SDHCI_INT_STATUS	0x30
#define SDHCI_INT_ENABLE	0x34
#define SDHCI_SIGNAL_ENABLE	0x38
#define  SDHCI_INT_RESPONSE	0x00000001
#define  SDHCI_INT_DATA_END	0x00000002
#define  SDHCI_INT_DMA_END	0x00000008
#define  SDHCI_INT_SPACE_AVAIL	0x00000010
#define  SDHCI_INT_DATA_AVAIL	0x00000020
#define  SDHCI_INT_CARD_INSERT	0x00000040
#define  SDHCI_INT_CARD_REMOVE	0x00000080
#define  SDHCI_INT_CARD_INT	0x00000100
#define  SDHCI_INT_ERROR	0x00008000
#define  SDHCI_INT_TIMEOUT	0x00010000
#define  SDHCI_INT_CRC		0x00020000
#define  SDHCI_INT_END_BIT	0x00040000
#define  SDHCI_INT_INDEX	0x00080000
#define  SDHCI_INT_DATA_TIMEOUT	0x00100000
#define  SDHCI_INT_DATA_CRC	0x00200000
#define  SDHCI_INT_DATA_END_BIT	0x00400000
#define  SDHCI_INT_BUS_POWER	0x00800000
#define  SDHCI_INT_ACMD12ERR	0x01000000

#define  SDHCI_INT_NORMAL_MASK	0x00007FFF
#define  SDHCI_INT_ERROR_MASK	0xFFFF8000

#define  SDHCI_INT_CMD_MASK	(SDHCI_INT_RESPONSE | SDHCI_INT_TIMEOUT | \
		SDHCI_INT_CRC | SDHCI_INT_END_BIT | SDHCI_INT_INDEX)
#define  SDHCI_INT_DATA_MASK	(SDHCI_INT_DATA_END | SDHCI_INT_DMA_END | \
		SDHCI_INT_DATA_AVAIL | SDHCI_INT_SPACE_AVAIL | \
		SDHCI_INT_DATA_TIMEOUT | SDHCI_INT_DATA_CRC | \
		SDHCI_INT_DATA_END_BIT)

#define SDHCI_ACMD12_ERR	0x3C

/* 3E-3F reserved */

#define SDHCI_CAPABILITIES	0x40
#define  SDHCI_TIMEOUT_CLK_MASK	0x0000003F
#define  SDHCI_TIMEOUT_CLK_SHIFT 0
#define  SDHCI_TIMEOUT_CLK_UNIT	0x00000080
#define  SDHCI_CLOCK_BASE_MASK	0x00003F00
#define  SDHCI_CLOCK_BASE_SHIFT	8
#define  SDHCI_MAX_BLOCK_MASK	0x00030000
#define  SDHCI_MAX_BLOCK_SHIFT  16
#define  SDHCI_CAN_DO_HISPD	0x00200000
#define  SDHCI_CAN_DO_DMA	0x00400000
#define  SDHCI_CAN_VDD_330	0x01000000
#define  SDHCI_CAN_VDD_300	0x02000000
#define  SDHCI_CAN_VDD_180	0x04000000

/* 44-47 reserved for more caps */

#define SDHCI_MAX_CURRENT	0x48

/* 4C-4F reserved for more max current */

/* 50-FB reserved */

#define SDHCI_SLOT_INT_STATUS	0xFC

#define SDHCI_HOST_VERSION	0xFE
#define  SDHCI_VENDOR_VER_MASK	0xFF00
#define  SDHCI_VENDOR_VER_SHIFT	8
#define  SDHCI_SPEC_VER_MASK	0x00FF
#define  SDHCI_SPEC_VER_SHIFT	0

/* The size the standard SDHC register set. */
#define SDHCI_REGSET_SIZE		0x100

struct sdhci_chip;


/******************************************************************************
*   STRUCT      :   sdhci_host
******************************************************************************/
/**
 *  @brief   structure of SDHC host, one host per slot
 *
 *****************************************************************************/
struct sdhci_host {
	/** Pointer to the chip of the host */
	struct sdhci_chip	*chip;
	
	/** Pinter to the MMC host associated with the slot */
	struct mmc_host		*mmc;

	/** Mutual exclusion semaphore for accessing the host/slot */
	spinlock_t			lock;

	/** Host attributes */
	int					flags;
#define SDHCI_USE_DMA		(1<<0)

	/** Max possible frequency (Hz) */
	unsigned int		max_clk;
	
	/** Timeout frequency used to calculate data timeout */	
	unsigned int		timeout_clk;

	/** Current clock (Hz) */
	unsigned int		clock;
	
	/** Current voltage */
	unsigned short		power;

	/** Current request */
	struct mmc_request	*mrq;

	/** Current command */	
	struct mmc_command	*cmd;
	
	/* Current data request */	
	struct mmc_data		*data;
	
	/** Data finished before command */
	int					data_early:1;
		
	/** Current sg to be transferred */
	struct scatterlist	*cur_sg;
	
	/** Entries left for transfer */	
	int					num_sg;
	
	/** Offset into current sg */	
	int					offset;
	
	/** Bytes left in current sg */	
	int					remain;
	
	/* Name of the slot for reservations */
	char				slot_descr[20];
	
	/** Device IRQ */
	int					irq;
	
	/** Slot index */
	int					slot;
	
	/* Flag to identify it's high speed MMC */
	int					hs_mmc;
	
	/** Memory address */
	struct resource		*mem;
	
#ifdef CONFIG_MMC_WHOVILLE
	/** Backed up values of Registers for the host */
	unsigned char		reg_set[SDHCI_REGSET_SIZE];
#endif
	
	/** Mapped address */
	void __iomem *		ioaddr;
	
	/** Tasklet for handling card detect */
	struct tasklet_struct	card_tasklet;
	
	/** Tasklet to finsih a request */
	struct tasklet_struct	finish_tasklet;
	
	/** Timer for request timeout */
	struct timer_list	timer;
};


/******************************************************************************
*   STRUCT      :   sdhci_chip
******************************************************************************/
/**
 *  @brief   structure of SDHC interface chip
 *
 *****************************************************************************/
struct sdhci_chip {
	/** Pointer to the related platform device */
	struct platform_device	*pdev;

	/** Special quirks for the chip to have different behavior */
	unsigned long			quirks;

	/** Number of slots on the chip */
	int						num_slots;
	
	/** Current slot being accessed */
	int						cur_slot;
	
	/** PDID of the chip */
	int 					pdid;
	
#ifdef CONFIG_MMC_WHOVILLE
	/** Mutual exclusion semaphore for accessing the chip */
	struct semaphore		sem;
#endif
	
	/** Pointers to hosts. This MUST be the last item in the structure! */
	struct sdhci_host		*hosts[0];
};
