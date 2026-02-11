/*****************************************************************************
* Copyright 2004 - 2011 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a
* license other than the GPL, without Broadcom's express prior written
* consent.
*****************************************************************************/
/**
 * @file    nandbch.c
 * @brief   NAND low level API
 * @note
 **/
#include <linux/dma-mapping.h>
#include <linux/delay.h>
#include <chal/chal_nandbch_uc.h>	/* micro-code */
#include <chal/chal_nandbch.h>
#include "nandbch.h"

#define NANDBCH_BCH_POLL_MAX_COUNT        100000

/* timeout values are in micro seconds */

/* reset timeout (> tRST) */
#define NANDBCH_TIMEOUT_RESET             2000

/* read timeout (> tR) */
#define NANDBCH_TIMEOUT_READ              500

/* program timeout (> tPROG) */
#define NANDBCH_TIMEOUT_WRITE             10000

/* block erase timeout (> tBERS) */
#define NANDBCH_TIMEOUT_ERASE             50000

/* DMA transfer timeout (> page size * max(tRC,tWC) ) */
#define NANDBCH_TIMEOUT_DMA               10000

/* feature timeout (> tFEAT) */
#define NANDBCH_TIMEOUT_FEATURE           200

#define NANDBCH_TIMEOUT_STATUS_GET        200
#define NANDBCH_TIMEOUT_ID_GET            200

#define NANDBCH_IRQ_ALL(bank) \
(NAND_IRQSTATUS_BCH_ERR_RDY_MASK | \
NAND_IRQSTATUS_BCH_WR_ECC_VALID_MASK | \
NAND_IRQSTATUS_BCH_RD_ECC_VALID_MASK | \
NAND_IRQSTATUS_DMA_CMPL_IRQ_MASK | \
NAND_IRQSTATUS_DMA_ERROR_ENABLE_MASK | \
(0x1 << (NAND_IRQSTATUS_BANK_CMPL_IRQ_SHIFT + (bank))) | \
(0x1 << (NAND_IRQSTATUS_BANK_ERR_IRQ_SHIFT + (bank))) | \
(0x1 << (NAND_IRQSTATUS_RB_IRQ_SHIFT + (bank))))

#define NANDBCH_IRQ_ERR(bank) \
(NAND_IRQSTATUS_DMA_ERROR_ENABLE_MASK | \
(0x1 << (NAND_IRQSTATUS_BANK_ERR_IRQ_SHIFT + (bank))))

/* Interrupt status masks indicating the completion of various operation */
#define NANDBCH_IRQ_BANK_RB(bank) \
((0x1 << (NAND_IRQSTATUS_BANK_CMPL_IRQ_SHIFT + (bank))) | \
(0x1 << (NAND_IRQSTATUS_RB_IRQ_SHIFT + (bank))))

#define NANDBCH_IRQ_BANK_DMA(bank) \
((0x1 << (NAND_IRQSTATUS_BANK_CMPL_IRQ_SHIFT + (bank))) | \
NAND_IRQSTATUS_DMA_CMPL_IRQ_MASK)

#define NANDBCH_IRQ_BANK(bank) \
(0x1 << (NAND_IRQSTATUS_BANK_CMPL_IRQ_SHIFT + (bank)))

#define NANDBCH_IRQ_RB(bank) \
(0x1 << (NAND_IRQSTATUS_RB_IRQ_SHIFT + (bank)))

/* bad block marker */
#define NAND_BADBLOCK_MARKER_POS    (0x0)
#define NAND_BADBLOCK_MARKER_GOOD   (0xFF)
#define NAND_BADBLOCK_MARKER_BAD    (0x0)

#define	TIMER_TICK_ADJUSTED_DELAY(usec)     (usec)
/* Configuration table version */
#define NANDBCH_CFG_VER                0
/* Configuration data ECC definitions */
#define NANDBCH_CFG_ECC_T              40
#define NANDBCH_CFG_ECC_N              4656
#define NANDBCH_CFG_ECC_K              4096
#define NANDBCH_CFG_ECC_SIZE           72
#define NANDBCH_CFG_ECC_BYTES          70
/* Conversion functions between integers and unaligned bytes in low endian order */
static inline uint16_t get_unaligned_le16(uint8_t *p)
{
	return p[0] | p[1] << 8;
}
static inline uint32_t get_unaligned_le32(uint8_t *p)
{
	return p[0] | p[1] << 8 | p[2] << 16 | p[3] << 24;
}
static inline void put_unaligned_le16(uint16_t val, uint8_t *p)
{
	*p++ = val;
	*p = val >> 8;
}
static inline void put_unaligned_le32(uint32_t val, uint8_t *p)
{
	*p++ = val;
	*p++ = val >> 8;
	*p++ = val >> 16;
	*p = val >> 24;
}

static void nand_reg_dump(
	void);
static uint32_t shift_of(
	uint32_t i);
static uint32_t wait_for_ucode_completion(
	uint32_t bank,
	uint32_t irq_mask,
	int32_t usec);
static uint32_t ucode_completion_err(
	uint32_t bank,
	const char *func,
	uint32_t cmd);
static uint32_t nand_bch_err_count(
	int32_t * err_count);
static uint32_t nand_bch_err_fix(
	int32_t err_count,
	uint8_t * buf,
	uint32_t len);

static uint32_t nand_bch_ecc_copy(
	uint8_t * buf,
	uint32_t len,
	uint32_t total_len);
static uint32_t nand_config_onfi(
	nandbch_info_t * ni);
static uint32_t crc16_valid(
	uint8_t * datap,
	uint32_t len,
	uint8_t * crc16p);
static uint32_t nand_param_read_pre(
	nandbch_info_t * ni,
	uint8_t bank);
static uint32_t nand_param_read(
	nandbch_info_t * ni,
	uint8_t bank,
	uint8_t * buf);
static uint32_t nand_param_read_random(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t offset,
	uint32_t size,
	uint8_t * buf);

/* global variables */
static uint8_t nand_id_buf[NANDBCH_ID_BUF_SIZE]
__attribute__ ((aligned(NAND_DMA_ALIGN)));
static uint8_t nand_aux_buf[NANDBCH_AUX_MAX_SIZE]
__attribute__ ((aligned(NAND_DMA_ALIGN)));

/*****************************************************************************
* Private functions definitions
******************************************************************************/

#define nandbch_reset_device              nandbch_reset

static uint32_t shift_of(
	uint32_t i)
{
	uint32_t s = 0;

	i >>= 1;
	while (i) {
		s++;
		i >>= 1;
	}
	return s;
}

/*
 * Wait for the specified interrupts.
 * Returns 1 for timeout, 0 for success
 * */
static uint32_t wait_for_ucode_completion(
	uint32_t bank,
	uint32_t irq_mask,
	int32_t usec)
{
	int32_t d = TIMER_TICK_ADJUSTED_DELAY(1);	/* polling period */
	uint32_t rc = 0;

	while (usec > 0) {
		if ((irq_mask & chal_nand_irq_status()) == irq_mask) {
			break;
		}
		udelay(d);
		usec -= d;
	}

	if (usec <= 0) {
		rc = ((irq_mask & chal_nand_irq_status()) != irq_mask);
	}

	if (irq_mask & NAND_IRQSTATUS_DMA_CMPL_IRQ_MASK) {
		chal_nand_dma_stop();
	}

	if (rc == 0) {
		chal_nand_irq_clear(irq_mask & NANDBCH_IRQ_ERR(bank));
	}

	return rc;
}

static inline void bch_config_aux(
	nandbch_info_t * ni)
{
	chal_nand_bch_config(NANDBCH_BUS_WIDTH(ni), NANDBCH_AUX_ECC_N(ni),
			     NANDBCH_AUX_ECC_K(ni), NANDBCH_AUX_ECC_T(ni));
}

static inline void bch_config_data(
	nandbch_info_t * ni)
{
	chal_nand_bch_config(NANDBCH_BUS_WIDTH(ni), NANDBCH_ECC_N(ni),
			     NANDBCH_ECC_K(ni), NANDBCH_ECC_T(ni));
}

static inline int wait_ucode_irq_rb(
	uint32_t bank,
	int32_t usec)
{
	return wait_for_ucode_completion(bank, NANDBCH_IRQ_RB(bank), usec);
}

static inline int wait_ucode_bank_dma(
	uint32_t bank,
	int32_t usec)
{
	return wait_for_ucode_completion(bank,
					 NANDBCH_IRQ_BANK_DMA(bank), usec);
}

static inline int wait_ucode_bank_rb(
	uint32_t bank,
	int32_t usec)
{
	return wait_for_ucode_completion(bank, NANDBCH_IRQ_BANK_RB(bank), usec);
}

static inline int wait_ucode_bank(
	uint32_t bank,
	int32_t usec)
{
	return wait_for_ucode_completion(bank, NANDBCH_IRQ_BANK(bank), usec);
}

static uint32_t ucode_completion_err(
	uint32_t bank,
	const char *func,
	uint32_t cmd)
{
	uint32_t irqstatus = chal_nand_irq_status();
	uint32_t rc = NANDBCH_RC_TOUT;

	dprint
	("Completion error: %s cmd %d, bank %d irqstatus 0x%x\n",
	 func, cmd, bank, irqstatus);
	nand_reg_dump();

	if (irqstatus & (0x1 & (NAND_IRQSTATUS_BANK_ERR_IRQ_SHIFT + bank))) {
		rc = NANDBCH_RC_BANK_ERROR;
	}
	if (irqstatus & NAND_IRQSTATUS_DMA_ERROR_ENABLE_MASK) {
		rc = NANDBCH_RC_DMA_ERROR;
	}

	chal_nand_irq_clear(NANDBCH_IRQ_ALL(bank));

	return rc;
}

static void nand_reg_dump(
	void)
{
#if NANDBCH_DEBUG == 1
	uint32_t *addr;
	chal_nand_prd_entry_t *prd_entry;
	uint32_t uc_instr;

	/* dump registers */
	dprint("\nregister dump:\n");
	dprint("\tcommand\t\t%08X\n",
	       *((volatile unsigned int *)(NAND_REG_COMMAND)));
	dprint("\taddress\t\t%08X\n",
	       *((volatile unsigned int *)(NAND_REG_ADDRESS)));
	dprint("\tattri0\t\t%08X\n",
	       *((volatile unsigned int *)(NAND_REG_ATTR0)));
	dprint("\tattri1\t\t%08X\n",
	       *((volatile unsigned int *)(NAND_REG_ATTR1)));
	dprint("\tbank\t\t%08X\n", *((volatile unsigned int *)(NAND_REG_BANK)));
	dprint("\tcontrol\t\t%08X\n",
	       *((volatile unsigned int *)(NAND_REG_CONTROL)));
	dprint("\tconfig0\t\t%08X\n",
	       *((volatile unsigned int *)(NAND_REG_CONFIG0)));
	dprint("\tconfig1\t\t%08X\n",
	       *((volatile unsigned int *)(NAND_REG_CONFIG1)));
	dprint("\tconfig2\t\t%08X\n",
	       *((volatile unsigned int *)(NAND_REG_CONFIG2)));
	dprint("\tstatus\t\t%08X\n",
	       *((volatile unsigned int *)(NAND_REG_STATUS)));
	dprint("\tirqctrl\t\t%08X\n",
	       *((volatile unsigned int *)(NAND_REG_IRQCTRL)));
	dprint("\tirqstatus\t%08X\n",
	       *((volatile unsigned int *)(NAND_REG_IRQSTATUS)));
	prd_entry =
		(chal_nand_prd_entry_t
		 *) (*((volatile unsigned int *)(NAND_REG_PRDBASE)));
	dprint("\tprdbase\t\t%p\n", prd_entry);
	dprint("\tdmaint\t\t%08X %08X %08X %08X\n",
	       *((volatile unsigned int *)(NAND_REG_DMAINT0)),
	       *((volatile unsigned int *)(NAND_REG_DMAINT1)),
	       *((volatile unsigned int *)(NAND_REG_DMAINT2)),
	       *((volatile unsigned int *)(NAND_REG_DMAINT3)));
	dprint("\n\terrlog:\n" "\t\t\t%08X %08X %08X %08X\n"
	       "\t\t\t%08X %08X %08X %08X\n",
	       *((volatile unsigned int *)(NAND_REG_ERRLOG0)),
	       *((volatile unsigned int *)(NAND_REG_ERRLOG1)),
	       *((volatile unsigned int *)(NAND_REG_ERRLOG2)),
	       *((volatile unsigned int *)(NAND_REG_ERRLOG3)),
	       *((volatile unsigned int *)(NAND_REG_ERRLOG4)),
	       *((volatile unsigned int *)(NAND_REG_ERRLOG5)),
	       *((volatile unsigned int *)(NAND_REG_ERRLOG6)),
	       *((volatile unsigned int *)(NAND_REG_ERRLOG7)));
	dprint("\n\tminstr\t\t%08X\n",
	       *((volatile unsigned int *)(NAND_REG_MINSTR)));
	dprint("\tmaddr0\t\t%08X\n",
	       *((volatile unsigned int *)(NAND_REG_MADDR0)));
	dprint("\tmaddr1\t\t%08X\n",
	       *((volatile unsigned int *)(NAND_REG_MADDR1)));
	dprint("\tmresp\t\t%08X\n",
	       *((volatile unsigned int *)(NAND_REG_MRESP)));
	dprint("\n\tbch_ctrl\t%08X\n",
	       *((volatile unsigned int *)(NAND_REG_BCH_CTRL)));
	dprint("\tbch_status\t%08X\n",
	       *((volatile unsigned int *)(NAND_REG_BCH_STATUS)));
	dprint("\tbch_nk\t\t%08X\n",
	       *((volatile unsigned int *)(NAND_REG_BCH_NK)));
	dprint("\tbch_t\t\t%08X\n",
	       *((volatile unsigned int *)(NAND_REG_BCH_T)));
	dprint("\tbch_err_loc\t%08X\n",
	       *((volatile unsigned int *)(NAND_REG_BCH_ERR_LOC)));
	dprint("\tbch_wr_ecc:\n" "\t\t\t%08X %08X %08X %08X\n"
	       "\t\t\t%08X %08X %08X %08X\n"
	       "\t\t\t%08X %08X %08X %08X\n"
	       "\t\t\t%08X %08X %08X %08X\n" "\t\t\t%08X %08X\n",
	       *((volatile unsigned int *)(NAND_REG_BCH_WR_ECC0)),
	       *((volatile unsigned int *)(NAND_REG_BCH_WR_ECC1)),
	       *((volatile unsigned int *)(NAND_REG_BCH_WR_ECC2)),
	       *((volatile unsigned int *)(NAND_REG_BCH_WR_ECC3)),
	       *((volatile unsigned int *)(NAND_REG_BCH_WR_ECC4)),
	       *((volatile unsigned int *)(NAND_REG_BCH_WR_ECC5)),
	       *((volatile unsigned int *)(NAND_REG_BCH_WR_ECC6)),
	       *((volatile unsigned int *)(NAND_REG_BCH_WR_ECC7)),
	       *((volatile unsigned int *)(NAND_REG_BCH_WR_ECC8)),
	       *((volatile unsigned int *)(NAND_REG_BCH_WR_ECC9)),
	       *((volatile unsigned int *)(NAND_REG_BCH_WR_ECC10)),
	       *((volatile unsigned int *)(NAND_REG_BCH_WR_ECC11)),
	       *((volatile unsigned int *)(NAND_REG_BCH_WR_ECC12)),
	       *((volatile unsigned int *)(NAND_REG_BCH_WR_ECC13)),
	       *((volatile unsigned int *)(NAND_REG_BCH_WR_ECC14)),
	       *((volatile unsigned int *)(NAND_REG_BCH_WR_ECC15)),
	       *((volatile unsigned int *)(NAND_REG_BCH_WR_ECC16)),
	       *((volatile unsigned int *)(NAND_REG_BCH_WR_ECC17)));

	/* dump prd table */
	if (prd_entry) {
		dprint("\nprd table:\n");
		while (1) {
			dprint("\t%08X\n", prd_entry->phys_addr);
			dprint("\t%08X\n", prd_entry->desc);
			if (prd_entry->desc & 0x80000000)
				break;
			prd_entry++;
		}
		dprint("\n");
	}

	/* dump ucode */
	dprint("\nucode:\n");
	addr = (uint32_t *) NAND_REG_UCODE_START;
	addr +=
		(*((volatile unsigned int *)(NAND_REG_COMMAND)) &
		 NAND_COMMAND_OPER_CODE_MASK);
	while (addr < (uint32_t *) NAND_REG_UCODE_END) {
		uc_instr = *((volatile unsigned int *)(addr));
		dprint("\t%08X\n", uc_instr);
		addr++;
		if ((uc_instr & NAND_COMMAND_OPER_CODE_MASK) == 0x1FFF) {
			break;
		}
	}
	dprint("\n");
#endif
}

static uint32_t nand_bch_err_count(
	int32_t * err_count)
{
	uint32_t cnt = NANDBCH_BCH_POLL_MAX_COUNT;

	while (cnt) {
		if (chal_nand_bch_rd_ecc_valid_irq_status())
			break;
		cnt--;
	}

	if (cnt == 0) {
		dprint("Read ECC valid IRQ timeout\n");
		nand_reg_dump();
		return NANDBCH_RC_ECC_TOUT;
	}

	*err_count = chal_nand_bch_err_count();

	chal_nand_bch_rd_ecc_valid_irq_clear();

	return NANDBCH_RC_SUCCESS;
}

static uint32_t nand_bch_err_fix(
	int32_t err_count,
	uint8_t * buf,
	uint32_t len)
{
	uint32_t cnt = NANDBCH_BCH_POLL_MAX_COUNT;

	while (cnt) {
		if (chal_nand_bch_err_rdy_irq_status())
			break;
		cnt--;
	}

	if (cnt == 0) {
		dprint("ECC error ready IRQ timeout\n");
		nand_reg_dump();
		return NANDBCH_RC_ECC_TOUT;
	}

	chal_nand_bch_err_fix(err_count, buf, len);

	chal_nand_bch_err_rdy_irq_clear();

	return NANDBCH_RC_SUCCESS;
}

static uint32_t nand_bch_ecc_copy(
	uint8_t * buf,
	uint32_t len,
	uint32_t total_len)
{
	uint32_t cnt = NANDBCH_BCH_POLL_MAX_COUNT;
	uint8_t *p;

	while (cnt) {
		if (chal_nand_bch_wr_ecc_valid_irq_status())
			break;
		cnt--;
	}

	if (cnt == 0) {
		dprint("Write ECC valid IRQ timeout\n");
		nand_reg_dump();
		return NANDBCH_RC_ECC_TOUT;
	}

	chal_nand_bch_ecc_copy(buf, len);

	/* zero the padding bytes */
	for (p = buf + len; p < buf + total_len; p++) {
		*p = 0;
	}

	chal_nand_bch_wr_ecc_valid_irq_clear();

	return NANDBCH_RC_SUCCESS;
}

/*
 * CRC16 verification
 *
 * Argunents:
 *    datap    pointer to byte stream
 *    len      byte stream length
 *    crc16p   pointer to CRC16
 * Return:
 *    1     crc correct
 *    0     crc incorrect
 */
static uint32_t crc16_valid(
	uint8_t * datap,
	uint32_t len,
	uint8_t * crc16p)
{
	/* Bit by bit algorithm without augmented zero bytes */
	const uint32_t crcinit = 0x4F4E;	/* Initial CRC value in the shift register */
	const int32_t order = 16;	/* Order of the CRC-16 */
	const uint32_t polynom = 0x8005;	/* Polynomial */

	uint32_t i, j, c, bit;
	uint32_t crc;
	uint32_t crcmask, crchighbit;

	crc = crcinit;		/* Initialize the shift register with 0x4F4E */
	crcmask = ((((uint32_t) 1 << (order - 1)) - 1) << 1) | 1;
	crchighbit = (uint32_t) 1 << (order - 1);

	/* process byte stream, one byte at a time, bits processed from MSB to LSB */
	for (i = 0; i < len; i++) {
		c = (uint32_t) (*datap);
		datap++;
		for (j = 0x80; j; j >>= 1) {
			bit = crc & crchighbit;
			crc <<= 1;
			if (c & j)
				bit ^= crchighbit;
			if (bit)
				crc ^= polynom;
		}
		crc &= crcmask;
	}

	c = ((uint32_t) (*(crc16p + 1)) << 8) | (uint32_t) (*crc16p);

	return (c == crc);
}

/*******************************************************************************
* nand_param_read_pre(ni, bank) - Send ONFI read parameter page command to
*                                 device
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
*******************************************************************************/
static uint32_t nand_param_read_pre(
	nandbch_info_t * ni,
	uint8_t bank)
{
	dprint("%s\n", __func__);
	/* disable ECC */
	chal_nand_bch_disable();

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_PARAM_READ_PRE], 0,	/* addr */
			     0,	/* attr0 */
			     0,	/* attr1 */
			     NULL,	/* prdbase */
			     NAND_DIRECTION_FROM_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_rb(bank, NANDBCH_TIMEOUT_READ)) {
		return ucode_completion_err(bank, __func__,
					    CHAL_NAND_UC_PARAM_READ_PRE);
	}

	return NANDBCH_RC_SUCCESS;
}

typedef enum { for_device, for_cpu
	     } for_dma_t;

/* Setup descriptor */
static inline void setup_desc(
	nandbch_info_t * ni,
	dma_addr_t addr,
	uint8_t bank,
	uint32_t size)
{
	//dprint("%s line %d addr=0x%x size=0x%x\n", __func__, __LINE__, addr, size);
	chal_nand_prd_desc_set(&ni->desc.desc[0], (void *)addr, bank, size, 1);
}

/* Setup descriptor pair */
static inline void setup_desc2(
	nandbch_info_t * ni,
	dma_addr_t addr,
	uint8_t bank1,
	uint32_t size,
	dma_addr_t addr2,
	uint8_t bank2,
	uint32_t size2)
{
	//dprint("%s line %d addr=0x%x size=0x%x addr2=0x%x size2=0x%x\n", __func__, __LINE__, addr, size, addr2, size2);
	chal_nand_prd_desc_set(&ni->desc.desc[0], (void *)addr, bank1, size, 0);
	chal_nand_prd_desc_set(&ni->desc.desc[1], (void *)addr2, bank2, size2,
			       1);
}

/* Cache maintenance for descriptor */
static inline void cache_desc(
	for_dma_t owner,
	nandbch_info_t * ni)
{
	if (owner == for_device) {
		//dprint("%s line %d dma_sync_single_for_device: phys_addr=0x%x size=0x%x (DMA_TO_DEVICE)\n",
		//	__func__, __LINE__, ni->desc.phys_addr, sizeof(ni->desc.desc));
		dma_sync_single_for_device(ni->dev, ni->desc.phys_addr,
					   sizeof(ni->desc.desc),
					   DMA_TO_DEVICE);
	} else {
		//dprint("%s line %d dma_sync_single_for_cpu: phys_addr=0x%x size=0x%x (DMA_TO_DEVICE)\n",
		//	__func__, __LINE__, ni->desc.phys_addr, sizeof(ni->desc.desc));
		dma_sync_single_for_cpu(ni->dev, ni->desc.phys_addr,
					sizeof(ni->desc.desc), DMA_TO_DEVICE);
	}
}

/* Cache maintenance for buffer */
static inline void cache_buf(
	for_dma_t owner,
	nandbch_info_t * ni,
	dma_addr_t addr,
	uint32_t size,
	enum dma_data_direction dir)
{
	if (owner == for_device) {
		//dprint("%s line %d dma_sync_single_for_device: addr=0x%x size=0x%x dir=%d (%s)\n",
		//	__func__, __LINE__, addr, size, dir, (dir==1) ? "DMA_TO_DEVICE" : "DMA_FROM_DEVICE");
		dma_sync_single_for_device(ni->dev, addr, size, dir);
	} else {
		//dprint("%s line %d dma_sync_single_for_cpu: addr=0x%x size=0x%x dir=%d (%s)\n",
		//	__func__, __LINE__, addr, size, dir, (dir==1) ? "DMA_TO_DEVICE" : "DMA_FROM_DEVICE");
		dma_sync_single_for_cpu(ni->dev, addr, size, dir);
	}
}

/* Setup descriptor and do cache maintenance on it */
static inline void cache_desc_for_device(
	nandbch_info_t * ni,
	dma_addr_t addr,
	uint8_t bank,
	uint32_t size)
{
	//dprint("%s line %d addr=0x%x size=0x%x\n", __func__, __LINE__, addr, size);
	setup_desc(ni, addr, bank, size);
	cache_desc(for_device, ni);
}

/* Setup descriptor pair and do cache maintenance on it */
static inline void cache_desc2_for_device(
	nandbch_info_t * ni,
	dma_addr_t addr,
	uint8_t bank1,
	uint32_t size,
	dma_addr_t addr2,
	uint8_t bank2,
	uint32_t size2)
{
	//dprint("%s line %d addr=0x%x size=0x%x, addr2=0x%x size2=0x%x\n", __func__, __LINE__, addr, size, addr2, size2);
	setup_desc2(ni, addr, bank1, size, addr2, bank2, size2);
	cache_desc(for_device, ni);
}

/* Setup descriptor pair and do cache maintenance on it and on the buffer */
static inline void cache_desc2_buf_for_device(
	nandbch_info_t * ni,
	dma_addr_t addr,
	uint8_t bank1,
	uint32_t size,
	dma_addr_t addr2,
	uint8_t bank2,
	uint32_t size2,
	enum dma_data_direction dir)
{
	//dprint("%s line %d addr=0x%x size=0x%x addr2=0x%x size2=0x%x\n", __func__, __LINE__, addr, size, addr2, size2);
	cache_desc2_for_device(ni, addr, bank1, size, addr2, bank2, size2);
	cache_buf(for_device, ni, addr, size, dir);	/* First buffer is the main data buffer */
}

/* Setup descriptor and do cache maintenance on it and on the buffer */
static inline void cache_desc_buf_for_device(
	nandbch_info_t * ni,
	dma_addr_t addr,
	uint8_t bank,
	uint32_t size,
	enum dma_data_direction dir)
{
	//dprint("%s line %d addr=0x%x size=0x%x\n", __func__, __LINE__, addr, size);
	cache_desc_for_device(ni, addr, bank, size);
	cache_buf(for_device, ni, addr, size, dir);
}

/* Do cache maintenance on the descriptor and the buffer */
static inline void cache_desc_buf_for_cpu(
	nandbch_info_t * ni,
	dma_addr_t addr,
	uint8_t bank,
	uint32_t size,
	enum dma_data_direction dir)
{
	(void)bank;		/* Keep prototypes the same for ease of use */
	//dprint("%s line %d addr=0x%x size=0x%x dir=%d\n", __func__, __LINE__, addr, size, dir);
	cache_desc(for_cpu, ni);
	cache_buf(for_cpu, ni, addr, size, dir);
}

/*******************************************************************************
* nand_param_read(ni, bank, buf) - Read ONFI parameter page (x 2)
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @buf:	   [out] buffer
*******************************************************************************/
static uint32_t nand_param_read(
	nandbch_info_t * ni,
	uint8_t bank,
	uint8_t * buf)
{
	int bufsize = 512;
	dma_addr_t addr = ni->buf.phys_addr;
	uint32_t size = bufsize;
	enum dma_data_direction dir = DMA_FROM_DEVICE;
	chal_nand_dmaint_set(bank, bufsize, 0);
	cache_desc_buf_for_device(ni, addr, bank, size, dir);

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_READ], 0,
			     /* addr */
			     bufsize,	/* attr0 */
			     0,	/* attr1 */
			     (void *)ni->desc.phys_addr,	/* prdbase */
			     NAND_DIRECTION_FROM_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
		cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
		return ucode_completion_err(bank, __func__, CHAL_NAND_UC_READ);
	}
	cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
	memcpy(buf, ni->buf.buf, bufsize);
	return NANDBCH_RC_SUCCESS;
}

/*******************************************************************************
* nand_param_read_random(ni, bank, buf) - Read random from parameter page
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @offset:	[in] byte offset
* @size:	   [in] data size
* @buf:	   [out] buffer
*******************************************************************************/
static uint32_t nand_param_read_random(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t offset,
	uint32_t size,
	uint8_t * buf)
{
	dma_addr_t addr = ni->buf.phys_addr;
	enum dma_data_direction dir = DMA_FROM_DEVICE;

	/* round up size to multiple of 512 bytes (HW limitation: HWCAPRI-385) */
	size = ((size + 511) / 512) * 512;
	chal_nand_dmaint_set(bank, size, 0);
	cache_desc_buf_for_device(ni, addr, bank, size, dir);

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)
			     [CHAL_NAND_UC_READ_RANDOM], 0,	/* addr */
			     (offset << 16) | size,	/* attr0 */
			     0,	/* attr1 */
			     (void *)ni->desc.phys_addr,	/* prdbase */
			     NAND_DIRECTION_FROM_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
		cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
		return ucode_completion_err(bank, __func__,
					    CHAL_NAND_UC_READ_RANDOM);
	}
	cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
	memcpy(buf, ni->buf.buf, size);
	return NANDBCH_RC_SUCCESS;
}

/**
 * nand_config_onfi - configures the geometry based on the ONFI parameter page
 */
static uint32_t nand_config_onfi(
	nandbch_info_t * ni)
{
	static uint32_t buf[1024];
	uint8_t *ext_param = (uint8_t *) buf;
	uint8_t *param_page = NULL;
	uint32_t i, j;
	uint32_t rc;
	uint32_t page_size;
	uint32_t pages_per_block;
	uint32_t blocks_per_lun;
	uint32_t lun_per_chip_enable;
	uint32_t valid_param_page;
	uint32_t param_page_cpy;
	uint32_t ext_param_offset;
	uint32_t ext_param_size;
	uint32_t ecc_section_offset;
	uint32_t timing_mode;
	uint8_t ecc;

	dprint("%s\n", __func__);
	if ((rc =
		     nandbch_id_get(ni, 0, NANDBCH_ONFI_ID_ADDR,
				    NANDBCH_ONFI_ID_SIZE,
				    nand_id_buf)) != NANDBCH_RC_SUCCESS) {
		return rc;
	}

	if ((nand_id_buf[0] != 'O') || (nand_id_buf[1] != 'N')
	    || (nand_id_buf[2] != 'F') || (nand_id_buf[3] != 'I')) {
		return NANDBCH_RC_NON_ONFI;
	}

	if ((rc = nand_param_read_pre(ni, 0)) != NANDBCH_RC_SUCCESS) {
		return rc;
	}

	i = 0;
	valid_param_page = 0;
	while (!valid_param_page && (i < 4)) {
		i++;
		/* get 512 bytes in one read due to HW limitation (HWCAPRI-385) */
		if ((rc =
			     nand_param_read(ni, 0,
					     ni->param_buf.buf)) != NANDBCH_RC_SUCCESS) {
			return rc;
		}

		for (j = 0; j < 2; j++) {
			param_page = ni->param_buf.buf + 256 * j;

			/* validate ONFI parameter page signature */
			if ((param_page[0] != 'O')
			    || (param_page[1] != 'N')
			    || (param_page[2] != 'F')
			    || (param_page[3] != 'I')) {
				continue;
			}
			/* validated CRC */
			if (crc16_valid(param_page, 254, param_page + 254)) {
				valid_param_page = 1;
				break;
			}
		}
	}

	/* return if no valid parameter page has been found */
	if (valid_param_page == 0) {
		return NANDBCH_RC_PARAM_ERR;
	}

	/* configure geometry parameters */
	page_size = get_unaligned_le32(&param_page[80]);
	pages_per_block = get_unaligned_le32(&param_page[92]);
	blocks_per_lun = get_unaligned_le32(&param_page[96]);
	lun_per_chip_enable = param_page[100];
	timing_mode = get_unaligned_le16(&param_page[129]);
	NANDBCH_BUS_WIDTH(ni) = (param_page[6] & 0x1) ? 16 : 8;
	if (param_page[101] == 0x22) {
		NANDBCH_ADDR_CYCLES(ni) = 4;
	} else if (param_page[101] == 0x23) {
		NANDBCH_ADDR_CYCLES(ni) = 5;
	} else {
		/* number of address cycles not supported */
		NANDBCH_ADDR_CYCLES(ni) = 0;
	}
	NANDBCH_PAGE_SHIFT(ni) = shift_of(page_size);
	NANDBCH_BLOCK_SHIFT(ni) =
		shift_of(pages_per_block) + NANDBCH_PAGE_SHIFT(ni);
	NANDBCH_BANK_SHIFT(ni) =
		shift_of(blocks_per_lun) + shift_of(lun_per_chip_enable) +
		NANDBCH_BLOCK_SHIFT(ni);
	NANDBCH_OOB_SIZE(ni) = get_unaligned_le16(&param_page[84]);

	NANDBCH_FEATURE(ni) = get_unaligned_le16(&param_page[6]);
	NANDBCH_OPT_CMD(ni) = get_unaligned_le16(&param_page[8]);

	if ((NANDBCH_FLAGS(ni) & NANDBCH_FLAG_TIMING) == 0) {

		/* set timing mode to the maximum supported by the device */
		NANDBCH_TIMING_MODE(ni) = 5;
		while (NANDBCH_TIMING_MODE(ni) > 0) {
			if (timing_mode & (1UL << NANDBCH_TIMING_MODE(ni)))
				break;
			NANDBCH_TIMING_MODE(ni)--;
		}
		NANDBCH_TIMING_SELECT(ni) = NANDBCH_TIMING_SELECT_TMODE_ONLY;
		if (NANDBCH_OPT_CMD(ni) & NANDBCH_OPT_CMD_GET_SET_FEATURE) {
			NANDBCH_TIMING_SELECT(ni) =
				NANDBCH_TIMING_SELECT_TMODE_SET_FEATURE;
		}

		NANDBCH_FLAGS(ni) |= NANDBCH_FLAG_TIMING;
	}
	if ((NANDBCH_FLAGS(ni) & NANDBCH_FLAG_BCH_CONFIG) == 0) {
		/* configure ECC */
		ecc = param_page[112];
		if (ecc != 0xff) {
			NANDBCH_SECTOR_SIZE(ni) = 512;
			NANDBCH_ECC_T(ni) = ecc;
		} else {	/* configure ECC from info in the extended paremeter table */

			/* extended parameter page not supported */
			if ((NANDBCH_FEATURE(ni) &
			     NANDBCH_FEATURE_EXT_PARAM_PAGE) == 0) {
				return NANDBCH_RC_EXTPARAM_ERR;
			}

			param_page_cpy = param_page[14];
			ext_param_offset = 256 * param_page_cpy;
			ext_param_size = get_unaligned_le16(&param_page[12]) << 4;

			if (ext_param_size > sizeof(buf)) {
				return NANDBCH_RC_EXTPARAM_ERR;
			}

			for (i = 0; i < param_page_cpy; i++) {
				if ((rc =
					     nand_param_read_random(ni, 0,
								    ext_param_offset,
								    ext_param_size,
								    ext_param))
				    != NANDBCH_RC_SUCCESS) {
					return rc;
				}

				ext_param_offset += ext_param_size;

				/* validate extended parameter page signature */
				if ((ext_param[2] != 'E')
				    || (ext_param[3] != 'P')
				    || (ext_param[4] != 'P')
				    || (ext_param[5] != 'S')) {
					continue;
				}
				/* validated CRC */
				if (crc16_valid
				    (ext_param + 2,
				     ext_param_size - 2, ext_param)) {
					ecc_section_offset = 32;
					if (ext_param[16] != 2) {
						ecc_section_offset +=
							(ext_param[17] << 4);
						if (ext_param[18] != 2) {
							return
								NANDBCH_RC_EXTPARAM_ERR;
						}
					}
					NANDBCH_ECC_T(ni) =
						ext_param[ecc_section_offset];
					NANDBCH_SECTOR_SIZE(ni) =
						0x1 <<
						ext_param[ecc_section_offset + 1];
					break;
				}
			}
			/* return if no valid extended parameter page has been found */
			if (i == param_page_cpy) {
				return NANDBCH_RC_EXTPARAM_ERR;
			}
		}
	}

	return NANDBCH_RC_SUCCESS;
}

/*******************************************************************************
* nand_config(ni, bank, addr_cycles, page, incr, iter) - Configure nand
* interface based on parameters stored on the NAND flash
* @ni:            [out] nand info structure
* @bank:	         [in] bank number
* @addr_cycles:	[in] address cycles
* @page:          [in/out] page number
* @incr:	         [in] page increment
* @iter:	         [in] mamimum number of iterations
*
* Attempts to retrive NAND configuration parameters from the specified bank
* and page. If unsuccessfull, it repeats the attempt up to the maximum number
* specified in the <iter> argument adding <incr> to the page number at each
* attempt. When it finds a valid config page, it updates the page number.
*
* The configuration page has the following information:
* (LSB at lower offset)
*
* Bytes        Content
* 0-3          "NAND" signature
* 4-7          Version number
* 8-11         Page size
* 12-15        ECC offset
* 16-17        Sector size
* 18           BCH ECC correction requirement (bits/sector)
*
* Additional parameters not used by BOOTROM:
*
* Bytes        Content
* 19           Auxiliary data BCH ECC correction requirement
* 20-23        Block size
* 24-27        Bank size (in MB)
* 28           Timing config selection
*                 0x00    default (ONFI timing mode 0)
*                 0x01    use ONFI timing mode from byte 29 and send ONFI
*                         SET_FEATURE command to device to change timing mode
*                 0x02    use ONFI timing mode from byte 29 only
*                 0x03    use NAND controller timing config register values
*                         (Capri encoding scheme)
* 29           ONFI timing mode [0..5]
* 30-45        NAND controller timing config register values
* 46-47        OOB size
*
* 48-511       Reserved
*
******************************************************************************/
uint32_t nandbch_config(
	nandbch_info_t * ni,
	uint8_t bank,
	uint8_t addr_cycles,
	uint32_t * page,
	uint32_t incr,
	uint32_t iter)
{
	uint32_t rc;
	uint32_t i, p;
	uint32_t uc_offset;
	uint32_t page_size, sect_size, ecc_offs;
	int32_t err_count;
	uint8_t ecc_t;
	uint32_t block_size, bank_size, aux_type_size, aux_ecc_bytes, oob_size;
	uint8_t aux_ecc_t;

	dprint("%s: bank%u cycles %u page 0x%x incr %u, iter %u\n",
	       __func__, bank, addr_cycles, *page, incr, iter);

	if ((ni == NULL) || (page == NULL)) {
		return NANDBCH_RC_FAILURE;
	}

	if ((addr_cycles != 4) && (addr_cycles != 5)) {
		return NANDBCH_RC_FAILURE;
	}

	NANDBCH_BUS_WIDTH(ni) = 8;
	NANDBCH_ADDR_CYCLES(ni) = addr_cycles;

	/* reset the NAND controller and set normal config (DMA mode; ECC off) */
	chal_nand_reset();

	/* setup bus width to 8 bit and aux data type to 4 byte */
	chal_nand_set_config(((NAND_CONFIG0_AUX_DATA_TYPE_4B <<
			       NAND_CONFIG0_AUX_DATA_TYPE_SHIFT) &
			      NAND_CONFIG0_AUX_DATA_TYPE_MASK)
			     |
			     ((NAND_CONFIG0_DATA_WIDTH_8 <<
			       NAND_CONFIG0_DATA_WIDTH_SHIFT) &
			      NAND_CONFIG0_DATA_WIDTH_MASK));

	/* configure ONFI timing mode 0 */
	chal_nand_set_timers(NAND_CONFIG1_TMODE0, NAND_CONFIG2_TMODE0);

	/* load ucode programs */
	uc_offset = 0;
	NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_RESET] =
		chal_nand_ucode_load(&uc_offset, uc_RESET, sizeof(uc_RESET));
	NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_READ_PRE] =
		chal_nand_ucode_load(&uc_offset, uc_READ_PRE, sizeof(uc_READ_PRE));
	NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_READ] =
		chal_nand_ucode_load(&uc_offset, uc_READ, sizeof(uc_READ));

	/* check for ucode memory overflow */
	if (chal_nand_ucode_buf_overflow(uc_offset)) {
		return NANDBCH_RC_NOMEM;
	}

	if ((rc = nandbch_reset_device(ni, bank)) != NANDBCH_RC_SUCCESS) {
		return rc;
	}

	/* Configure ECC to maximum supported */
	chal_nand_bch_config(8, NANDBCH_CFG_ECC_N, NANDBCH_CFG_ECC_K,
			     NANDBCH_CFG_ECC_T);

	/* Search for configuration page */
	p = *page;
	for (i = 0; i < iter; i++, p += incr) {
		dma_addr_t addr = ni->param_buf.phys_addr;
		uint32_t size = 512;
		dma_addr_t addr2 = ni->ecc_buf.phys_addr;
		uint32_t size2 = NANDBCH_CFG_ECC_SIZE;
		enum dma_data_direction dir = DMA_FROM_DEVICE;

		/* disable ECC */
		chal_nand_bch_disable();

		/* Execute ucode */
		chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_READ_PRE], p,	/* addr */
				     0,	/* attr0 */
				     NANDBCH_ADDR_CYCLES(ni),	/* attr1 */
				     NULL,	/* prdbase */
				     NAND_DIRECTION_FROM_NAND);

		/* Wait for completion interrupts */
		if (wait_ucode_bank_rb(bank, NANDBCH_TIMEOUT_READ)) {
			return ucode_completion_err(bank, __func__,
						    CHAL_NAND_UC_READ_PRE);
		}
		chal_nand_dmaint_set(bank, size, size2);
		cache_desc2_buf_for_device(ni, addr, bank, size, addr2, bank,
					   size2, dir);

		/* Enable ECC for read */
		chal_nand_bch_read_enable();

		/* Execute ucode */
		chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_READ], 0,	/* addr */
				     512 + NANDBCH_CFG_ECC_SIZE,	/* attr0 */
				     0,	/* attr1 */
				     (void *)ni->desc.phys_addr,	/* prdbase */
				     NAND_DIRECTION_FROM_NAND);

		/* Wait for completion interrupts */
		if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
			cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
			return ucode_completion_err(bank, __func__,
						    CHAL_NAND_UC_READ);
		}
		cache_desc_buf_for_cpu(ni, addr, bank, size, dir);

		/* disable ECC */
		chal_nand_bch_disable();

		/* Check for BCH ECC errors */
		rc = nand_bch_err_count(&err_count);
		if (rc != NANDBCH_RC_SUCCESS) {
			return rc;
		}

		/* Uncorrectable BCH ECC errors */
		if (err_count == -1) {
			/* not a configuration page */
			dprint("NAND table not found: page %d\n", p);
			continue;
		}

		/* Fix all correctable BCH ECC errors */
		if (err_count > 0) {
			rc = nand_bch_err_fix(err_count, ni->param_buf.buf,
					      512);
			if (rc != NANDBCH_RC_SUCCESS) {
				return rc;
			}
		}
		if ((ni->param_buf.buf[0] == 'N')
		    && (ni->param_buf.buf[1] == 'A')
		    && (ni->param_buf.buf[2] == 'N')
		    && (ni->param_buf.buf[3] == 'D')) {

			/* configuration page found */
			*page = p;
			break;
		}
	}

	if (i == iter) {
		/* config data not found */
		return NANDBCH_RC_NOCONFIG;
	}

	/* use configuration parameters */

	page_size = get_unaligned_le32(&ni->param_buf.buf[8]);
	ecc_offs = get_unaligned_le32(&ni->param_buf.buf[12]);
	sect_size = get_unaligned_le16(&ni->param_buf.buf[16]);
	ecc_t = ni->param_buf.buf[18];

	NANDBCH_PAGE_SHIFT(ni) = shift_of(page_size);
	NANDBCH_SECTOR_SIZE(ni) = sect_size;
	NANDBCH_ECC_T(ni) = ecc_t;
	NANDBCH_AUX_SIZE(ni) = ecc_offs - page_size;
	NANDBCH_AUX_ECC_T(ni) = 0;

	aux_ecc_t = ni->param_buf.buf[19];
	block_size = get_unaligned_le32(&ni->param_buf.buf[20]);
	bank_size = get_unaligned_le32(&ni->param_buf.buf[24]);
	oob_size = get_unaligned_le16(&ni->param_buf.buf[46]);
	if ((block_size > 0) && (block_size != 0xFFFFFFFF)) {
		NANDBCH_BLOCK_SHIFT(ni) = shift_of(block_size);
	}
	if ((bank_size > 0) && (bank_size != 0xFFFFFFFF)) {
		NANDBCH_BANK_SHIFT(ni) = shift_of(bank_size) + 20;
	}
	if ((oob_size > 0) && (oob_size != 0xFFFF)) {
		NANDBCH_OOB_SIZE(ni) = oob_size;
	}
	if (ni->param_buf.buf[28] != 0xFF) {
		NANDBCH_TIMING_SELECT(ni) = ni->param_buf.buf[28];
		switch (NANDBCH_TIMING_SELECT(ni)) {
		case NANDBCH_TIMING_SELECT_DEFAULT:

			/* nothing to do */
			break;
		case NANDBCH_TIMING_SELECT_TMODE_SET_FEATURE:
		case NANDBCH_TIMING_SELECT_TMODE_ONLY:
			NANDBCH_FLAGS(ni) |= NANDBCH_FLAG_TIMING;
			NANDBCH_TIMING_MODE(ni) = ni->param_buf.buf[29];
			break;
		case NANDBCH_TIMING_SELECT_REG_CAPRI:
			NANDBCH_TIMING_CONF1(ni) = get_unaligned_le32(&ni->param_buf.buf[30]);
			NANDBCH_TIMING_CONF2(ni) = get_unaligned_le32(&ni->param_buf.buf[34]);
			break;
		default:
			dprint
			("NAND config: ignoring invalid timing selection %d\n",
			 NANDBCH_TIMING_SELECT(ni));
			NANDBCH_TIMING_SELECT(ni) =
				NANDBCH_TIMING_SELECT_DEFAULT;
		}
	}
	if ((aux_ecc_t > 0) && (aux_ecc_t != 0xFF)) {

		/*
		 * The aux data size in DMAINT register has only 5 bits.
		 * By default, the aux type is set to 2 bytes, so max aux data size or ecc size with this config is 31*2
		 */
		aux_type_size = 2;

		/* Check the main data ECC size */
		if ((14 * ecc_t + 7) / 8 > 62) {

			/* set auxiliary data type to 4 bytes */
			aux_type_size = 4;
		}

		/* Calculate the AUX data ECC bytes */
		aux_ecc_bytes = (14 * aux_ecc_t + 7) / 8;

		/* round up aux ecc bytes to multiple of aux data type size */
		aux_ecc_bytes =
			((aux_ecc_bytes + aux_type_size -
			  1) / aux_type_size) * aux_type_size;
		if ((NANDBCH_AUX_SIZE(ni) % aux_type_size == 0)
		    && (NANDBCH_AUX_SIZE(ni) > aux_ecc_bytes)) {
			NANDBCH_AUX_ECC_T(ni) = aux_ecc_t;
			NANDBCH_AUX_SIZE(ni) -= aux_ecc_bytes;
		} else {
			dprint
			("NAND config: ignoring invalid auxiliary data ecc configuration\n");
		}
	}
	dprint("NAND table found: page %d addr 0x%08X\n", p,
	       p << NANDBCH_PAGE_SHIFT(ni));

	return NANDBCH_RC_SUCCESS;
}

/*******************************************************************************
* nandbch_init(ni, flags) - Initialize NAND interface
* @ni:            [in/out] nand info structure
* @flags:         [in] options
*
* By default it configures parameters in nandbch_info_t based on ONFI parameter
* page information.
* Default values for the aux_size is 2 bytes / 512 bytes of main data and no
* ECC correction is enabled for auxiliary data in the OOB.
* Calling nandbch_init with the NANDBCH_FLAG_GEOMETRY flag will force the use of the
* parameters passed in the geometry structure (minimum the bus_width and
* page_shift need to be set for NAND read only access).
* Calling nandbch_init with the NANDBCH_FLAG_BCH_CONFIG flag will force the use of
* the ECC parameters passed in the ecc structure (sector data_size and ecc_t
* need to be set).
* Calling nandbch_init with the NANDBCH_FLAG_AUX_CONFIG flag will force the use of
* the auxiliary data parameters passed in the aux structure (aux data_size and
* ecc_t need to be set).
* Calling nandbch_init with the NANDBCH_FLAG_TIMING flag will force the use of the
* timing parameters passed in the timing structure. By default timing values
* are set as follows:
*     Hold_tH		         Hold time:		                     20ns
*     Setup_tS		         Setup time:		                     70ns
*     wr_cycle_tWH	 	   Write high time:	                  45ns
*     wr_pulse_width_tWP	Write pulse width:	               55ns
*     rd_cycle_tREH	      Read high time:		               45ns
*     rd_pulse_width_tRP	Read pulse width:	                  55ns
*     rTHZ		            RE high to out imped time		      75ns
*     tCEA-tREA	         diff between CE and RE access time	60ns
*     tOE:                 Time adj for output enable to
*                          control the sampling time:          10ns
*******************************************************************************/
uint32_t nandbch_init(
	nandbch_info_t * ni,
	uint8_t flags)
{
	uint32_t uc_offset;
	uint32_t bank;
	uint32_t rc;
	uint32_t i;
	uint32_t aux_type_size;

	uint32_t config1, config2;
	uint32_t b;
	uint32_t buf;
	uint8_t *feat = (uint8_t *) & buf;

	dprint("%s\n", __func__);
	if (ni == NULL) {
		return NANDBCH_RC_FAILURE;
	}

	NANDBCH_FLAGS(ni) = flags;

	/* reset the NAND controller and set normal config (DMA mode; ECC off) */
	chal_nand_reset();

	/* setup bus width to 8 bit for ID stage */
	chal_nand_set_config(NAND_CONFIG0_NORMAL);

	/* configure ONFI timing mode 0 for ID stage */
	chal_nand_set_timers(NAND_CONFIG1_TMODE0, NAND_CONFIG2_TMODE0);

	/* load ucode programs */
	uc_offset = 0;
	NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_RESET] =
		chal_nand_ucode_load(&uc_offset, uc_RESET, sizeof(uc_RESET));
	NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_ID_GET] =
		chal_nand_ucode_load(&uc_offset, uc_ID_GET, sizeof(uc_ID_GET));
	NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_PARAM_READ_PRE] =
		chal_nand_ucode_load(&uc_offset, uc_PARAM_READ_PRE,
				     sizeof(uc_PARAM_READ_PRE));
	NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_GET_FEATURE_PRE] =
		chal_nand_ucode_load(&uc_offset, uc_GET_FEATURE_PRE,
				     sizeof(uc_GET_FEATURE_PRE));
	NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_SET_FEATURE] =
		chal_nand_ucode_load(&uc_offset, uc_SET_FEATURE,
				     sizeof(uc_SET_FEATURE));
	NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_READ_PRE] =
		chal_nand_ucode_load(&uc_offset, uc_READ_PRE, sizeof(uc_READ_PRE));
	NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_READ] =
		chal_nand_ucode_load(&uc_offset, uc_READ, sizeof(uc_READ));
	NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_READ_RANDOM] =
		chal_nand_ucode_load(&uc_offset, uc_READ_RANDOM,
				     sizeof(uc_READ_RANDOM));
	NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_READ_ECC] =
		chal_nand_ucode_load(&uc_offset, uc_READ_ECC, sizeof(uc_READ_ECC));
	NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_READ_RANDOM_ECC] =
		chal_nand_ucode_load(&uc_offset, uc_READ_RANDOM_ECC,
				     sizeof(uc_READ_RANDOM_ECC));
	NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_STATUS_GET] =
		chal_nand_ucode_load(&uc_offset, uc_STATUS_GET,
				     sizeof(uc_STATUS_GET));
	NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_BLOCK_ERASE] =
		chal_nand_ucode_load(&uc_offset, uc_BLOCK_ERASE,
				     sizeof(uc_BLOCK_ERASE));
	NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_PRE] =
		chal_nand_ucode_load(&uc_offset, uc_WRITE_PRE,
				     sizeof(uc_WRITE_PRE));
	NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_RANDOM] =
		chal_nand_ucode_load(&uc_offset, uc_WRITE_RANDOM,
				     sizeof(uc_WRITE_RANDOM));
	NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE] =
		chal_nand_ucode_load(&uc_offset, uc_WRITE, sizeof(uc_WRITE));
	NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_COMPLETE] =
		chal_nand_ucode_load(&uc_offset, uc_WRITE_COMPLETE,
				     sizeof(uc_WRITE_COMPLETE));

	/* check for ucode memory overflow */
	if (chal_nand_ucode_buf_overflow(uc_offset)) {
		return NANDBCH_RC_NOMEM;
	}

	/* discover bank and configuration */
	for (bank = 0; bank < NANDBCH_BANKS_MAX; bank++) {
		if ((rc = nandbch_reset_device(ni, bank)) != NANDBCH_RC_SUCCESS) {
			break;
		}
		if ((rc =
			     nandbch_id_get(ni, bank, NANDBCH_JEDEC_ID_ADDR,
					    NANDBCH_JEDEC_ID_SIZE,
					    nand_id_buf)) != NANDBCH_RC_SUCCESS) {
			break;
		}
		/* save ID of the first bank --- they all MUST be the same */
		if (bank == 0) {
			for (i = 0; i < NANDBCH_JEDEC_ID_SIZE; i++) {
				NANDBCH_ID(ni)[i] = nand_id_buf[i];
			}
		} else {
			/* all NAND chips must be the same */
			for (i = 0; i < NANDBCH_JEDEC_ID_SIZE; i++) {
				if ((NANDBCH_ID(ni))[i] != nand_id_buf[i]) {
					return NANDBCH_RC_BANK_CFG_ERR;
				}
			}
		}
	}

	if (bank == 0) {
		return rc;
	}

	NANDBCH_BANKS(ni) = bank;

	if ((NANDBCH_FLAGS(ni) & NANDBCH_FLAG_GEOMETRY) == 0) {
		/* Configure NAND geometry for ONFI devices */
		if ((rc = nand_config_onfi(ni)) != NANDBCH_RC_SUCCESS) {
#if 0
			/* For non ONFI device use the ID to configure geometry */
			nand_config_geometry(ni);
#else
			return rc;
#endif
		}
	}

	/* Configure timing parameters */
	if (NANDBCH_FLAGS(ni) & NANDBCH_FLAG_TIMING) {
		switch (NANDBCH_TIMING_SELECT(ni)) {
		case NANDBCH_TIMING_SELECT_DEFAULT:

			/* nothing to do */
			break;
		case NANDBCH_TIMING_SELECT_TMODE_SET_FEATURE:

			/* Send SET_FEATURE command to change timing mode on the NAND device */
			feat[0] = NANDBCH_TIMING_MODE(ni);
			feat[1] = 0;
			feat[2] = 0;
			feat[3] = 0;
			for (b = 0; b < NANDBCH_BANKS(ni); b++) {
				if ((rc =
					     nandbch_feature_set(ni, b, 0x01,
								 feat)) !=
				    NANDBCH_RC_SUCCESS) {
					dprint
					("Fail to set ONFI timing mode %d on bank %d\n",
					 NANDBCH_TIMING_MODE(ni), b);
					return rc;
				}
			}

			/* fall through */
		case NANDBCH_TIMING_SELECT_TMODE_ONLY:
			switch (NANDBCH_TIMING_MODE(ni)) {
			case 1:
				config1 = NAND_CONFIG1_TMODE1;
				config2 = NAND_CONFIG2_TMODE1;
				break;
			case 2:
				config1 = NAND_CONFIG1_TMODE2;
				config2 = NAND_CONFIG2_TMODE2;
				break;
			case 3:
				config1 = NAND_CONFIG1_TMODE3;
				config2 = NAND_CONFIG2_TMODE3;
				break;
			case 4:
				config1 = NAND_CONFIG1_TMODE4;
				config2 = NAND_CONFIG2_TMODE4;
				break;
			case 5:
				config1 = NAND_CONFIG1_TMODE5;
				config2 = NAND_CONFIG2_TMODE5;
				break;
			default:
				config1 = NAND_CONFIG1_TMODE0;
				config2 = NAND_CONFIG2_TMODE0;
			}
			chal_nand_set_timers(config1, config2);
			break;
		case NANDBCH_TIMING_SELECT_REG_CAPRI:

			/* set timing parameters */
			chal_nand_set_timers(NANDBCH_TIMING_CONF1(ni),
					     NANDBCH_TIMING_CONF2(ni));
			break;
		default:
			dprint("NAND init: ignore invalid timing select %d\n",
			       NANDBCH_TIMING_SELECT(ni));
			NANDBCH_TIMING_SELECT(ni) = 0;
		}
	}
	/* configure timing mode */

	if (NANDBCH_BUS_WIDTH(ni) == 16) {
		/* 16 bit NAND not supported */
		return NANDBCH_RC_IF_ERR;
	}

	if ((NANDBCH_ADDR_CYCLES(ni) != 4)
	    && (NANDBCH_ADDR_CYCLES(ni) != 5)) {
		/* only 4 and 5 address cycles supported */
		return NANDBCH_RC_IF_ERR;
	}

	if (NANDBCH_ECC_T(ni) == 0) {
		/* disable ECC if not required */
		NANDBCH_FLAGS(ni) &= ~NANDBCH_FLAG_ECC;
	}

	if ((NANDBCH_FLAGS(ni) & NANDBCH_FLAG_ECC) == 0) {
		/* ECC disabled */
		NANDBCH_SECTOR_SIZE(ni) = 0;
		NANDBCH_ECC_BYTES(ni) = 0;
		NANDBCH_ECC_SIZE(ni) = 0;
		NANDBCH_ECC_T(ni) = 0;
		NANDBCH_ECC_N(ni) = 0;
		NANDBCH_ECC_K(ni) = 0;
	} else {
		if (((NANDBCH_SECTOR_SIZE(ni) != 512)
		     && (NANDBCH_SECTOR_SIZE(ni) != 1024))
		    || (NANDBCH_ECC_T(ni) > 40)) {
			/* Unsupported BCH ECC configuration */
			return NANDBCH_RC_ECC_CFG_ERR;
		}

		/* calculate the other ecc parameters from sector_size and ecc_t */

		NANDBCH_ECC_BYTES(ni) = (14 * NANDBCH_ECC_T(ni) + 7) / 8;

		/*
		 * The aux data size in DMAINT register has only 5 bits.
		 * By default the aux type set to 2 bytes, so max aux data size with this config is 31*2
		 */
		if (NANDBCH_ECC_BYTES(ni) > 62) {
			/* set auxiliary data type to 4 bytes */
			chal_nand_set_config
			(chal_nand_set_aux_data_type
			 (chal_nand_get_config(),
			  NAND_CONFIG0_AUX_DATA_TYPE_4B));
		}

		/* round up ecc bytes to multiple of aux data type size */
		aux_type_size = chal_nand_get_aux_data_type_size();
		NANDBCH_ECC_SIZE(ni) =
			((NANDBCH_ECC_BYTES(ni) + aux_type_size -
			  1) / aux_type_size) * aux_type_size;

		NANDBCH_ECC_N(ni) =
			8 * (NANDBCH_SECTOR_SIZE(ni) + NANDBCH_ECC_BYTES(ni));
		NANDBCH_ECC_K(ni) = NANDBCH_ECC_N(ni) - 14 * NANDBCH_ECC_T(ni);
	}

	/* AUX data configuration */
	if ((NANDBCH_FLAGS(ni) & NANDBCH_FLAG_AUX_CONFIG) == 0) {
		/* default aux size: 2 bytes for every 512 byte of data and ECC disabled */
		NANDBCH_AUX_SIZE(ni) = 2 * (NANDBCH_PAGE_SIZE(ni) >> 9);
		NANDBCH_AUX_ECC_BYTES(ni) = 0;
		NANDBCH_AUX_ECC_SIZE(ni) = 0;
		NANDBCH_AUX_ECC_T(ni) = 0;
		NANDBCH_AUX_ECC_N(ni) = 0;
		NANDBCH_AUX_ECC_K(ni) = 0;
	} else {		/* custom aux configuration */

		aux_type_size = chal_nand_get_aux_data_type_size();

		/* check that AUX data size is multiple of aux data type size */
		if (NANDBCH_AUX_SIZE(ni) % aux_type_size) {
			/* Invalid AUX data size */
			return NANDBCH_RC_AUX_CFG_ERR;
		}

		if (((NANDBCH_FLAGS(ni) & NANDBCH_FLAG_ECC) == 0)
		    || (NANDBCH_AUX_ECC_T(ni) == 0)) {
			NANDBCH_AUX_ECC_BYTES(ni) = 0;
			NANDBCH_AUX_ECC_SIZE(ni) = 0;
			NANDBCH_AUX_ECC_T(ni) = 0;
			NANDBCH_AUX_ECC_N(ni) = 0;
			NANDBCH_AUX_ECC_K(ni) = 0;
		} else {	/* with ECC on AUX data */

			if (NANDBCH_ECC_T(ni) > 40) {
				/* Unsupported BCH ECC configuration */
				return NANDBCH_RC_ECC_CFG_ERR;
			}

			/* calculate the other parameters from aux_size and aux_ecc_t */
			NANDBCH_AUX_ECC_BYTES(ni) =
				(14 * NANDBCH_AUX_ECC_T(ni) + 7) / 8;

			/* round up aux ecc bytes to multiple of aux data type size */
			NANDBCH_AUX_ECC_SIZE(ni) =
				((NANDBCH_AUX_ECC_BYTES(ni) +
				  aux_type_size -
				  1) / aux_type_size) * aux_type_size;

			/* Auxiliary data size is on 5 bit in DMAINT register (HW limitation: HWCAPRI-385) */
			if ((NANDBCH_AUX_SIZE(ni) +
			     NANDBCH_AUX_ECC_SIZE(ni)) >
			    (0x1F * aux_type_size)) {
				/* Unsupported BCH ECC configuration */
				return NANDBCH_RC_ECC_CFG_ERR;
			}

			NANDBCH_AUX_ECC_N(ni) =
				8 * (NANDBCH_AUX_SIZE(ni) +
				     NANDBCH_AUX_ECC_BYTES(ni));
			NANDBCH_AUX_ECC_K(ni) =
				NANDBCH_AUX_ECC_N(ni) - 14 * NANDBCH_AUX_ECC_T(ni);
		}
	}

	/* if OOB size is known, verify that it can accomodate the configured AUX data plus AUX and main data ECC bytes */
	if (NANDBCH_OOB_SIZE(ni)) {
		if (NANDBCH_OOB_SIZE(ni) <
		    (NANDBCH_AUX_SIZE(ni) + NANDBCH_AUX_ECC_SIZE(ni) +
		     NANDBCH_SECTORS(ni) * NANDBCH_ECC_SIZE(ni))) {

			/* Not enough OOB bytes to support this configuration */
			return NANDBCH_RC_OOB_SIZE_ERR;
		}
	}
	return NANDBCH_RC_SUCCESS;
}

/*******************************************************************************
* nandbch_reset(ni, bank, buf) - Send reset command to the nand device
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
*******************************************************************************/
uint32_t nandbch_reset(
	nandbch_info_t * ni,
	uint8_t bank)
{
	/* disable ECC */
	chal_nand_bch_disable();

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_RESET], 0,	/* addr */
			     0,	/* attr0 */
			     0,	/* attr1 */
			     NULL,	/* prdbase */
			     NAND_DIRECTION_FROM_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_rb(bank, NANDBCH_TIMEOUT_RESET)) {
		return ucode_completion_err(bank, __func__, CHAL_NAND_UC_RESET);
	}

	return NANDBCH_RC_SUCCESS;
}

/*******************************************************************************
* nandbch_id_get(ni, bank, buf) - Get nand id
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @addr:	   [in] address (0x0 or 0x20)
* @len:	   [in] length (8 or 4)
* @buf:	   [out] buffer to get the id
*******************************************************************************/
uint32_t nandbch_id_get(
	nandbch_info_t * ni,
	uint8_t bank,
	uint8_t addr,
	uint8_t len,
	uint8_t * buf)
{
	dma_addr_t bufaddr = ni->buf.phys_addr;
	uint32_t size = len;
	enum dma_data_direction dir = DMA_FROM_DEVICE;

	dprint("%s\n", __func__);

	chal_nand_dmaint_set(bank, 0, len);
	cache_desc_buf_for_device(ni, bufaddr, bank, size, dir);

	/* disable ECC */
	chal_nand_bch_disable();

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_ID_GET], addr,	/* addr */
			     len,	/* attr0 */
			     0,	/* attr1 */
			     (void *)ni->desc.phys_addr,	/* prdbase */
			     NAND_DIRECTION_FROM_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_ID_GET)) {
		return ucode_completion_err(bank, __func__,
					    CHAL_NAND_UC_ID_GET);
	}
	cache_desc_buf_for_cpu(ni, bufaddr, bank, size, dir);
	memcpy(buf, ni->buf.buf, len);
	return NANDBCH_RC_SUCCESS;
}

/*******************************************************************************
* nandbch_page_read(ni, bank, page, buf, stats) - Reads one page from nand into buf
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @page:    [in] page number
* @buf:     [out] target buffer
* @stats:   [out] ECC stats (number of bad bits per sector)
*
* Note: If the caller wants ECC correction stats it needs to provide a pointer
* to a nandbch_eccstats_t structure. The number of bits corrected
* by ECC in every sector will be returned in the corresponding array element.
* If stats are not needed caller should provide a NULL pointer argument.
*******************************************************************************/
uint32_t nandbch_page_read(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf,
	nandbch_eccstats_t * stats)
{
	int rc;
	int dataerrs;
	int i;

	dprint("%s: bank%u page 0x%x buf=%p\n", __func__, bank, page, buf);

	if (NANDBCH_FLAGS(ni) & NANDBCH_FLAG_ECC) {
		rc = nandbch_page_read_ecc(ni, bank, page, buf, stats);
		if (rc == NANDBCH_RC_ECC_ERROR) {
			/* Allow reading unprogrammed pages */
			return nandbch_page_read_raw(ni, bank, page, buf);
		}
		dataerrs = 0;
		for (i = 0; i <stats->len; i++)  {
			dataerrs += stats->errs[i];
		}
		if (dataerrs > 40) {	// FIXME remove magic
			/* Allow reading unprogrammed pages */
			/* Clear bogus stats by setting returned length to 0 */
			stats->len = 0;
			dprint("%s: rc=%d dataerrs=%u, reading raw pages instead\n", __func__, rc, dataerrs);
			return nandbch_page_read_raw(ni, bank, page, buf);
		}
		return rc;
	}
	return nandbch_page_read_raw(ni, bank, page, buf);
}

/*******************************************************************************
* nandbch_page_read_ecc(ni, bank, page, buf, stats) - Reads one page from nand into
*                                                  buf with BCH-ECC correction
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @page:    [in] page number
* @buf:     [out] target buffer
* @stats:   [out] ECC stats (number of bad bits per sector)
*
* Note: If the caller wants ECC correction stats it needs to provide a pointer
* to a nandbch_eccstats_t structure. The number of bits corrected
* by ECC in every sector will be returned in the corresponding array element.
* If stats are not needed caller should provide a NULL pointer argument.
*******************************************************************************/
uint32_t nandbch_page_read_ecc(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf,
	nandbch_eccstats_t * stats)
{
	uint32_t sector;
	uint32_t sect_offs;
	uint32_t ecc_offs;
	int32_t err_count;
	dma_addr_t sect_buf;
	uint32_t cmd;
	uint32_t rc;

	dprint("%s: bank%u page 0x%x buf=%p\n", __func__, bank, page, buf);

	/* disable ECC */
	chal_nand_bch_disable();

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_READ_PRE], page,	/* addr */
			     0,	/* attr0 */
			     NANDBCH_ADDR_CYCLES(ni),	/* attr1 */
			     NULL,	/* prdbase */
			     NAND_DIRECTION_FROM_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_rb(bank, NANDBCH_TIMEOUT_READ)) {
		return ucode_completion_err(bank, __func__,
					    CHAL_NAND_UC_READ_PRE);
	}

	/* Configure ECC for main data */
	bch_config_data(ni);

	for (sector = 0; sector < NANDBCH_SECTORS(ni); sector++) {
		dma_addr_t addr;
		uint32_t size = NANDBCH_SECTOR_SIZE(ni);
		dma_addr_t addr2 = ni->ecc_buf.phys_addr;
		uint32_t size2 = NANDBCH_ECC_SIZE(ni);
		enum dma_data_direction dir = DMA_FROM_DEVICE;
		sect_offs = sector * NANDBCH_SECTOR_SIZE(ni);
		sect_buf = ni->buf.phys_addr + sect_offs;
		ecc_offs =
			NANDBCH_PAGE_SIZE(ni) + NANDBCH_AUX_SIZE(ni) +
			NANDBCH_AUX_ECC_SIZE(ni) + sector * NANDBCH_ECC_SIZE(ni);
		addr = sect_buf;
		chal_nand_dmaint_set(bank, size, size2);
		cache_desc2_buf_for_device(ni, addr, bank, size, addr2, bank,
					   size2, dir);

		/* Enable ECC for read */
		chal_nand_bch_read_enable();

		cmd =
			(sector ==
			 0) ? CHAL_NAND_UC_READ_ECC : CHAL_NAND_UC_READ_RANDOM_ECC;
		/* Execute ucode */
		chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[cmd], 0,	/* addr */
				     (sect_offs << 16) | NANDBCH_SECTOR_SIZE(ni),	/* attr0 */
				     (ecc_offs << 16) | NANDBCH_ECC_SIZE(ni),	/* attr1 */
				     (void *)ni->desc.phys_addr,	/* prdbase */
				     NAND_DIRECTION_FROM_NAND);

		/* Wait for completion interrupts */
		if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
			cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
			return ucode_completion_err(bank, __func__, cmd);
		}
		cache_desc_buf_for_cpu(ni, addr, bank, size, dir);

		/* Disable ECC */
		chal_nand_bch_disable();

		/* Check for BCH ECC errors */
		rc = nand_bch_err_count(&err_count);
		if (rc != NANDBCH_RC_SUCCESS) {
			return rc;
		}

		/* Uncorrectable BCH ECC errors */
		if (err_count == -1) {
			return NANDBCH_RC_ECC_ERROR;
		}

		/* Fix all correctable BCH ECC errors */
		if (err_count > 0) {
			rc = nand_bch_err_fix(err_count,
					      (uint8_t *)(ni->buf.buf + sect_offs),
					      NANDBCH_SECTOR_SIZE(ni));
			if (rc != NANDBCH_RC_SUCCESS) {
				return rc;
			}
		}

		/* Set the ECC stats for the sector */
		if (stats) {
			stats->errs[sector] = err_count;
		}
	}
	if (stats) {
		stats->len = NANDBCH_SECTORS(ni);
	}
	memcpy(buf, ni->buf.buf, NANDBCH_SECTOR_SIZE(ni) * NANDBCH_SECTORS(ni));
	return NANDBCH_RC_SUCCESS;
}

/*******************************************************************************
* nandbch_page_read_raw(ni, bank, page, buf) - Reads one page from nand into buf
*                                           witout ECC corection
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @page:    [in] page number
* @buf:     [out] target buffer
*******************************************************************************/
uint32_t nandbch_page_read_raw(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf)
{
	dma_addr_t addr = ni->buf.phys_addr;
	uint32_t size = NANDBCH_PAGE_SIZE(ni);
	enum dma_data_direction dir = DMA_FROM_DEVICE;

	dprint("%s: bank%u page 0x%x buf=%p\n", __func__, bank, page, buf);

	/* disable ECC */
	chal_nand_bch_disable();

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_READ_PRE], page,	/* addr */
			     0,	/* attr0 */
			     NANDBCH_ADDR_CYCLES(ni),	/* attr1 */
			     NULL,	/* prdbase */
			     NAND_DIRECTION_FROM_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_rb(bank, NANDBCH_TIMEOUT_READ)) {
		return ucode_completion_err(bank, __func__,
					    CHAL_NAND_UC_READ_PRE);
	}
	chal_nand_dmaint_set(bank, size, 0);
	cache_desc_buf_for_device(ni, addr, bank, size, dir);

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_READ], 0,	/* addr */
			     NANDBCH_PAGE_SIZE(ni),	/* attr0 */
			     0,	/* attr1 */
			     (void *)ni->desc.phys_addr,	/* prdbase */
			     NAND_DIRECTION_FROM_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
		cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
		return ucode_completion_err(bank, __func__, CHAL_NAND_UC_READ);
	}
	cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
	memcpy(buf, ni->buf.buf, NANDBCH_PAGE_SIZE(ni));
	return NANDBCH_RC_SUCCESS;
}

/*******************************************************************************
* nandbch_aux_read_raw(ni, bank, page, buf) - Reads the auxiliary data from oob
*                                          without ECC correction
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @page:    [in] page number
* @buf:     [out] target buffer
*******************************************************************************/
uint32_t nandbch_aux_read_raw(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf)
{
	dma_addr_t addr = ni->buf.phys_addr;
	uint32_t size = NANDBCH_AUX_SIZE(ni);
	enum dma_data_direction dir = DMA_FROM_DEVICE;

	//dprint("%s: bank%u page 0x%x buf=%p\n", __func__, bank, page, buf);

	/* disable ECC */
	chal_nand_bch_disable();

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_READ_PRE], page,	/* addr */
			     NANDBCH_PAGE_SIZE(ni) << 16,	/* attr0 */
			     NANDBCH_ADDR_CYCLES(ni),	/* attr1 */
			     NULL,	/* prdbase */
			     NAND_DIRECTION_FROM_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_rb(bank, NANDBCH_TIMEOUT_READ)) {
		return ucode_completion_err(bank, __func__,
					    CHAL_NAND_UC_READ_PRE);
	}
	chal_nand_dmaint_set(bank, 0, size);
	cache_desc_buf_for_device(ni, addr, bank, size, dir);

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_READ], 0,	/* addr */
			     NANDBCH_AUX_SIZE(ni),	/* attr0 */
			     0,	/* attr1 */
			     (void *)ni->desc.phys_addr,	/* prdbase */
			     NAND_DIRECTION_FROM_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
		cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
		return ucode_completion_err(bank, __func__, CHAL_NAND_UC_READ);
	}
	cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
	memcpy(buf, ni->buf.buf, NANDBCH_AUX_SIZE(ni));
	return NANDBCH_RC_SUCCESS;
}

/*******************************************************************************
* nandbch_aux_read(ni, bank, page, buf, stats) - Reads the auxiliary data from oob
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @page:    [in] page number
* @buf:     [out] target buffer
* @stats:   [out] ECC stats (number of bad bits)
*
* Note: If ECC correction stats are not needed caller should provide a NULL
* pointer argument.
*******************************************************************************/
uint32_t nandbch_aux_read(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf,
	uint8_t * stats)
{
	int rc;

	dprint("%s: bank%u page 0x%x buf=%p\n", __func__, bank, page, buf);

	if ((NANDBCH_FLAGS(ni) & NANDBCH_FLAG_ECC)
	    && NANDBCH_AUX_ECC_T(ni)) {
		rc = nandbch_aux_read_ecc(ni, bank, page, buf, stats);
		if (rc == NANDBCH_RC_ECC_ERROR) {
			/* Allow reading unprogrammed aux data */
			dprint("%s: rc=%d uncorrectable ecc, reading raw aux data instead\n", __func__, rc);
			return nandbch_aux_read_raw(ni, bank, page, buf);
		}
		if (*stats > 40) { // FIXME remove magic
			/* Allow reading unprogrammed aux data */
			dprint("%s: rc=%d auxerrs = %d, reading raw aux data instead\n", __func__, rc, *stats);
			*stats = 0;
			return nandbch_aux_read_raw(ni, bank, page, buf);
		}
		return rc;
	}
	return nandbch_aux_read_raw(ni, bank, page, buf);
}

/*******************************************************************************
* nandbch_aux_read_ecc(ni, bank, page, buf, stats) - Reads the auxiliary data from
*                                                 oob with BCH-ECC correction
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @page:    [in] page number
* @buf:     [out] target buffer
* @stats:   [out] ECC stats (number of bad bits)
*
* Note: If ECC correction stats are not needed caller should provide a NULL
* pointer argument.
*******************************************************************************/
uint32_t nandbch_aux_read_ecc(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf,
	uint8_t * stats)
{
	int32_t err_count;
	uint32_t rc;
	dma_addr_t addr = ni->buf.phys_addr;
	uint32_t size = NANDBCH_AUX_SIZE(ni);
	dma_addr_t addr2 = ni->ecc_buf.phys_addr;
	uint32_t size2 = NANDBCH_AUX_ECC_SIZE(ni);
	enum dma_data_direction dir = DMA_FROM_DEVICE;

	dprint("%s: bank%u page 0x%x buf=%p\n", __func__, bank, page, buf);

	/* disable ECC */
	chal_nand_bch_disable();

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_READ_PRE], page,	/* addr */
			     NANDBCH_PAGE_SIZE(ni) << 16,	/* attr0 */
			     NANDBCH_ADDR_CYCLES(ni),	/* attr1 */
			     NULL,	/* prdbase */
			     NAND_DIRECTION_FROM_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_rb(bank, NANDBCH_TIMEOUT_READ)) {
		return ucode_completion_err(bank, __func__,
					    CHAL_NAND_UC_READ_PRE);
	}
	chal_nand_dmaint_set(bank, 0, size + size2);
	cache_desc2_buf_for_device(ni, addr, bank, size, addr2, bank, size2,
				   dir);

	/* Configure ECC for auxiliary data */
	bch_config_aux(ni);

	/* Enable ECC for read */
	chal_nand_bch_read_enable();

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_READ], 0,	/* addr */
			     NANDBCH_AUX_SIZE(ni) + NANDBCH_AUX_ECC_SIZE(ni),	/* attr0 */
			     0,	/* attr1 */
			     (void *)ni->desc.phys_addr,	/* prdbase */
			     NAND_DIRECTION_FROM_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
		cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
		return ucode_completion_err(bank, __func__, CHAL_NAND_UC_READ);
	}
	cache_desc_buf_for_cpu(ni, addr, bank, size, dir);

	/* disable ECC */
	chal_nand_bch_disable();

	/* check for BCH ECC errors */
	rc = nand_bch_err_count(&err_count);
	if (rc != NANDBCH_RC_SUCCESS) {
		return rc;
	}

	/* uncorrectable BCH ECC errors */
	if (err_count == -1) {
		return NANDBCH_RC_ECC_ERROR;
	}

	/* fix all correctable BCH ECC errors */
	if (err_count > 0) {
		rc = nand_bch_err_fix(err_count, (uint8_t *) ni->buf.buf,
				      NANDBCH_AUX_SIZE(ni));
		if (rc != NANDBCH_RC_SUCCESS) {
			return rc;
		}
	}

	/* set the ECC stats */
	if (stats) {
		*stats = err_count;
	}
	memcpy(buf, ni->buf.buf, NANDBCH_AUX_SIZE(ni));
	return NANDBCH_RC_SUCCESS;
}

/*******************************************************************************
* nandbch_block_isbad(ni, bank, block, is_bad) - Checks if block is marked bad
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @block:   [in] block number
* @is_bad:  [out] 1 if block is marked bad, 0 if good
*******************************************************************************/
uint32_t nandbch_block_isbad(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t block,
	uint8_t * is_bad)
{
	uint32_t rc, i, page;
	uint32_t off[3];

	/* check first second and last page of the block */
	off[0] = 0;
	off[1] = 1;
	off[2] = (1 << (NANDBCH_BLOCK_SHIFT(ni) - NANDBCH_PAGE_SHIFT(ni)))
		 - 1;

	*is_bad = 0;

	page = block << (NANDBCH_BLOCK_SHIFT(ni) - NANDBCH_PAGE_SHIFT(ni));

	for (i = 0; i < 3; i++) {

		rc = nandbch_aux_read_raw(ni, bank, page + off[i],
					  nand_aux_buf);

		if (rc != NANDBCH_RC_SUCCESS) {
			*is_bad = 1;
			break;
		}

		if (nand_aux_buf[NAND_BADBLOCK_MARKER_POS] !=
		    NAND_BADBLOCK_MARKER_GOOD) {
			*is_bad = 1;
			break;
		}
	}
	if (*is_bad) {
		dprint("%s: bank%u block 0x%x isbad=%d\n", __func__, bank,
		       block, *is_bad);
	}

	return rc;
}

/*******************************************************************************
 * nand_status_get - gets the status for an individual bank
 * @ni:     [in] nand info structure
 * @bank:	[in] bank to get status
 * @status:	[out] nand status
*******************************************************************************/
uint32_t nandbch_status_get(
	nandbch_info_t * ni,
	uint8_t bank,
	uint8_t * status)
{
	dprint("%s\n", __func__);
	/* disable ECC */
	chal_nand_bch_disable();

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_STATUS_GET], 0,	/* addr */
			     0,	/* attr0 */
			     0,	/* attr1 */
			     NULL,	/* prdbase */
			     NAND_DIRECTION_FROM_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank(bank, NANDBCH_TIMEOUT_STATUS_GET)) {
		*status = ~0;
		return ucode_completion_err(bank, __func__,
					    CHAL_NAND_UC_STATUS_GET);
	}

	*status = chal_nand_status_result();

	dprint("%s: bank%u status=0x%x\n", __func__, bank, *status);

	return NANDBCH_RC_SUCCESS;
}

extern int initrd_spec;

int is_block_protected(uint32_t block)
{
	if (block <= 8 && strncmp("dev=1",saved_command_line,5) != 0 && 
	    !initrd_spec) 
		return 1;
	return 0;
}

int is_page_protected(nandbch_info_t* ni, uint32_t page)
{
	return is_block_protected(page >> ((NANDBCH_BLOCK_SHIFT(ni) - NANDBCH_PAGE_SHIFT(ni))));
}

/*******************************************************************************
* nandbch_block_force_erase(ni, bank, block) - Erase block ignoring bad block marks
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @block:   [in] block number
*******************************************************************************/
uint32_t nandbch_block_force_erase(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t block)
{
	uint32_t rc;
	uint8_t status;

	dprint("%s: bank%u block=0x%x\n", __func__, bank, block);

	if (is_block_protected(block))
		return NANDBCH_RC_FAIL_STATUS;

	/* disable ECC */
	chal_nand_bch_disable();

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_BLOCK_ERASE], block << (NANDBCH_BLOCK_SHIFT(ni) - NANDBCH_PAGE_SHIFT(ni)),	/* addr */
			     0,	/* attr0 */
			     NANDBCH_ADDR_CYCLES(ni) - 2,	/* attr1 */
			     NULL,	/* prdbase */
			     NAND_DIRECTION_FROM_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_rb(bank, NANDBCH_TIMEOUT_ERASE)) {
		return ucode_completion_err(bank, __func__,
					    CHAL_NAND_UC_BLOCK_ERASE);
	}

	do {
		rc = nandbch_status_get(ni, bank, &status);
		if (rc != NANDBCH_RC_SUCCESS)
			return rc;
	} while (!(status & NAND_STATUS_READY));

	return ((status & NAND_STATUS_FAIL) ? NANDBCH_RC_FAIL_STATUS :
		NANDBCH_RC_SUCCESS);
}

/*******************************************************************************
* nandbch_block_erase(ni, bank, block)  - Erase a block
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @block:   [in] block number
*******************************************************************************/
uint32_t nandbch_block_erase(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t block)
{
	uint32_t rc;
	uint8_t isbad;

	dprint("%s: bank%u block=0x%x\n", __func__, bank, block);

	rc = nandbch_block_isbad(ni, bank, block, &isbad);
	if (rc != NANDBCH_RC_SUCCESS)
		return rc;
	if (isbad) {
		return NANDBCH_RC_BB_NOERASE;
	}

	return nandbch_block_force_erase(ni, bank, block);
}

/*******************************************************************************
* nandbch_block_markbad(ni, bank, block) - Erase block and write the bad block
*                                       marker in oob
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @block:   [in] block number
*******************************************************************************/
uint32_t nandbch_block_markbad(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t block)
{
	uint8_t isbad;
	uint32_t i, rc, page;
	uint32_t off[3];

	dprint("%s: bank%u block=0x%x\n", __func__, bank, block);

	/* erase block first */
	nandbch_block_force_erase(ni, bank, block);

	/* mark first second and last page of the block */
	off[0] = 0;
	off[1] = 1;
	off[2] = (1 << (NANDBCH_BLOCK_SHIFT(ni) - NANDBCH_PAGE_SHIFT(ni)))
		 - 1;

	for (i = 0; i < NANDBCH_AUX_SIZE(ni); i++) {
		nand_aux_buf[i] = 0xFF;
	}
	nand_aux_buf[NAND_BADBLOCK_MARKER_POS] = NAND_BADBLOCK_MARKER_BAD;

	page = block << (NANDBCH_BLOCK_SHIFT(ni) - NANDBCH_PAGE_SHIFT(ni));

	for (i = 0; i < 3; i++) {
		nandbch_aux_write_raw(ni, bank, page + off[i], nand_aux_buf);
	}

	rc = nandbch_block_isbad(ni, bank, block, &isbad);
	if ((rc != NANDBCH_RC_SUCCESS) || !isbad) {
		return NANDBCH_RC_FAILURE;
	}

	return NANDBCH_RC_SUCCESS;
}

/*******************************************************************************
* nandbch_page_write(ni, bank, page, buf) - Writes page from buf to the nand device
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @page:    [in] page number
* @buf:     [in] source buffer
*******************************************************************************/
uint32_t nandbch_page_write(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf, 
	int raw)
{
	dprint("%s: bank%u page 0x%x, buf=%p\n", __func__, bank, page, buf);

	if ((NANDBCH_FLAGS(ni) & NANDBCH_FLAG_ECC) && !raw) {
		return nandbch_page_write_ecc(ni, bank, page, buf);
	}
	return nandbch_page_write_raw(ni, bank, page, buf);
}

/******************************************************************************
* nandbch_aux_write (ni, bank, page, buf) - Writes the auxiliary data to oob
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @page:    [in] page number
* @buf:     [in] source buffer
*******************************************************************************/
uint32_t nandbch_aux_write(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf)
{
	dprint("%s: bank%u page 0x%x, buf=%p\n", __func__, bank, page, buf);

	if ((NANDBCH_FLAGS(ni) & NANDBCH_FLAG_ECC)
	    && NANDBCH_AUX_ECC_T(ni)) {
		return nandbch_aux_write_ecc(ni, bank, page, buf);
	}
	return nandbch_aux_write_raw(ni, bank, page, buf);
}

/*******************************************************************************
* nandbch_page_aux_write(ni, bank, page, buf) - Write page and aux data from buf to
*                                            nand device
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @page:    [in] page number
* @buf:     [in] source buffer
*******************************************************************************/
uint32_t nandbch_page_aux_write(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf)
{
	dprint("%s: bank%u page 0x%x, buf=%p\n", __func__, bank, page, buf);

	if (NANDBCH_FLAGS(ni) & NANDBCH_FLAG_ECC) {
		return nandbch_page_aux_write_ecc(ni, bank, page, buf);
	}
	return nandbch_page_aux_write_raw(ni, bank, page, buf);
}

/*******************************************************************************
* nandbch_page_write_raw(ni, bank, page, buf) - Writes page from buf to the nand
*                                            device, without ECC
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @page:    [in] page number
* @buf:     [in] source buffer
*******************************************************************************/
uint32_t nandbch_page_write_raw(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf)
{
	uint32_t cmd;
	uint32_t rc;
	uint32_t sector;
	uint32_t sect_offs;
	dma_addr_t sect_buf;
	uint8_t status;
	dma_addr_t addr0 = ni->buf.phys_addr;
	uint32_t size0 = NANDBCH_PAGE_SIZE(ni);
	enum dma_data_direction dir = DMA_TO_DEVICE;

	if (is_page_protected(ni, page))
		return NANDBCH_RC_FAIL_STATUS;

	memcpy(ni->buf.buf, buf, NANDBCH_PAGE_SIZE(ni));

	dprint("%s: bank%u page 0x%x, buf=%p\n", __func__, bank, page, buf);

	/* disable ECC */
	chal_nand_bch_disable();

	/* Perform cache management for page buffer */
	cache_buf(for_device, ni, addr0, size0, dir);
	for (sector = 0; sector < NANDBCH_SECTORS(ni); sector++) {
		dma_addr_t addr;
		uint32_t size = NANDBCH_SECTOR_SIZE(ni);
		enum dma_data_direction dir = DMA_TO_DEVICE;
		sect_buf = ni->buf.phys_addr + sector * NANDBCH_SECTOR_SIZE(ni);
		sect_offs = sector * NANDBCH_SECTOR_SIZE(ni);
		addr = sect_buf;
		chal_nand_dmaint_set(bank, size, 0);
		cache_desc_for_device(ni, addr, bank, size);

		cmd =
		    (sector ==
		     0) ? CHAL_NAND_UC_WRITE_PRE : CHAL_NAND_UC_WRITE_RANDOM;

		/* Execute ucode */
		chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[cmd], page,	/* addr */
				     (sect_offs << 16) | NANDBCH_SECTOR_SIZE(ni),	/* attr0 */
				     NANDBCH_ADDR_CYCLES(ni),	/* attr1 */
				     (void *)ni->desc.phys_addr,	/* prdbase */
				     NAND_DIRECTION_TO_NAND);

		/* Wait for completion interrupts */
		if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
			cache_desc(for_cpu, ni);
			return ucode_completion_err(bank, __func__, cmd);
		}
		cache_desc(for_cpu, ni);
		cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
	}			/* for each sector of the page */
	cache_buf(for_cpu, ni, addr0, size0, dir);

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_COMPLETE], 0,	/* addr */
			     0,	/* attr0 */
			     0,	/* attr1 */
			     NULL,	/* prdbase */
			     NAND_DIRECTION_TO_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_rb(bank, NANDBCH_TIMEOUT_WRITE)) {
		return ucode_completion_err(bank, __func__,
					    CHAL_NAND_UC_WRITE_COMPLETE);
	}

	do {
		rc = nandbch_status_get(ni, bank, &status);
		if (rc != NANDBCH_RC_SUCCESS)
			return rc;
	} while (!(status & NAND_STATUS_READY));

	return ((status & NAND_STATUS_FAIL) ? NANDBCH_RC_FAIL_STATUS :
		NANDBCH_RC_SUCCESS);
}

/******************************************************************************
* nandbch_aux_write_raw (ni, bank, page, buf) - Writes the auxiliary data to oob,
*                                            without ECC
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @page:    [in] page number
* @buf:     [in] source buffer
*******************************************************************************/
uint32_t nandbch_aux_write_raw(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf)
{
	uint32_t rc;
	uint8_t status;
	dma_addr_t addr = ni->buf.phys_addr;
	uint32_t size = NANDBCH_AUX_SIZE(ni);
	enum dma_data_direction dir = DMA_TO_DEVICE;

	if (is_page_protected(ni, page))
		return NANDBCH_RC_FAIL_STATUS;

	memcpy(ni->buf.buf, buf, NANDBCH_AUX_SIZE(ni));

	dprint("%s: bank%u page 0x%x, buf=%p\n", __func__, bank, page, buf);

	/* disable ECC */
	chal_nand_bch_disable();
	chal_nand_dmaint_set(bank, 0, size);
	cache_desc_buf_for_device(ni, addr, bank, size, dir);

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_PRE], page,	/* addr */
			     (NANDBCH_PAGE_SIZE(ni) << 16) | NANDBCH_AUX_SIZE(ni),	/* attr0 */
			     NANDBCH_ADDR_CYCLES(ni),	/* attr1 */
			     (void *)ni->desc.phys_addr,	/* prdbase */
			     NAND_DIRECTION_TO_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
		cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
		return ucode_completion_err(bank, __func__,
					    CHAL_NAND_UC_WRITE_PRE);
	}
	cache_desc_buf_for_cpu(ni, addr, bank, size, dir);

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_COMPLETE], 0,	/* addr */
			     0,	/* attr0 */
			     0,	/* attr1 */
			     NULL,	/* prdbase */
			     NAND_DIRECTION_TO_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_rb(bank, NANDBCH_TIMEOUT_WRITE)) {
		return ucode_completion_err(bank, __func__,
					    CHAL_NAND_UC_WRITE_COMPLETE);
	}

	do {
		rc = nandbch_status_get(ni, bank, &status);
		if (rc != NANDBCH_RC_SUCCESS)
			return rc;
	} while (!(status & NAND_STATUS_READY));

	return ((status & NAND_STATUS_FAIL) ? NANDBCH_RC_FAIL_STATUS :
		NANDBCH_RC_SUCCESS);
}

/*******************************************************************************
* nandbch_page_aux_write_raw(ni, bank, page, buf) - Write page and aux data from
*                                                buf to nand device, without ECC
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @page:    [in] page number
* @buf:     [in] source buffer
*******************************************************************************/
uint32_t nandbch_page_aux_write_raw(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf)
{
	uint32_t rc;
	uint8_t status;
	dma_addr_t addr = ni->buf.phys_addr;
	uint32_t size = NANDBCH_PAGE_SIZE(ni) + NANDBCH_AUX_SIZE(ni);
	enum dma_data_direction dir = DMA_TO_DEVICE;

	if (is_page_protected(ni, page))
		return NANDBCH_RC_FAIL_STATUS;

	memcpy(ni->buf.buf, buf, NANDBCH_PAGE_SIZE(ni) + NANDBCH_AUX_SIZE(ni));

	dprint("%s: bank%u page 0x%x, buf=%p\n", __func__, bank, page, buf);

	/* disable ECC */
	chal_nand_bch_disable();
	chal_nand_dmaint_set(bank, NANDBCH_PAGE_SIZE(ni), NANDBCH_AUX_SIZE(ni));
	cache_desc_buf_for_device(ni, addr, bank, size, dir);

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_PRE], page,	/* addr */
			     NANDBCH_PAGE_SIZE(ni) + NANDBCH_AUX_SIZE(ni),	/* attr0 */
			     NANDBCH_ADDR_CYCLES(ni),	/* attr1 */
			     (void *)ni->desc.phys_addr,	/* prdbase */
			     NAND_DIRECTION_TO_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
		cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
		return ucode_completion_err(bank, __func__,
					    CHAL_NAND_UC_WRITE_PRE);
	}
	cache_desc_buf_for_cpu(ni, addr, bank, size, dir);

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_COMPLETE], 0,	/* addr */
			     0,	/* attr0 */
			     0,	/* attr1 */
			     NULL,	/* prdbase */
			     NAND_DIRECTION_TO_NAND);
	/* Wait for completion interrupts */
	if (wait_ucode_bank_rb(bank, NANDBCH_TIMEOUT_WRITE)) {
		return ucode_completion_err(bank, __func__,
					    CHAL_NAND_UC_WRITE_COMPLETE);
	}

	do {
		rc = nandbch_status_get(ni, bank, &status);
		if (rc != NANDBCH_RC_SUCCESS)
			return rc;
	} while (!(status & NAND_STATUS_READY));

	return ((status & NAND_STATUS_FAIL) ? NANDBCH_RC_FAIL_STATUS :
		NANDBCH_RC_SUCCESS);
}

/*******************************************************************************
* nandbch_page_write_ecc(ni, bank, page, buf) - Writes page from buf to the nand
*                                            device with BCH_ECC
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @page:    [in] page number
* @buf:     [in] source buffer
*******************************************************************************/
uint32_t nandbch_page_write_ecc(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf)
{
	uint32_t cmd;
	uint32_t rc;
	uint32_t sector;
	uint32_t sect_offs;
	uint32_t ecc_offs;
	dma_addr_t sect_buf;
	uint8_t status;
	dma_addr_t addr0 = ni->buf.phys_addr;
	uint32_t size0 = NANDBCH_PAGE_SIZE(ni);
	enum dma_data_direction dir = DMA_TO_DEVICE;

	if (is_page_protected(ni, page))
		return NANDBCH_RC_FAIL_STATUS;

	memcpy(ni->buf.buf, buf, NANDBCH_PAGE_SIZE(ni));

	dprint("%s: bank%u page 0x%x, buf=%p\n", __func__, bank, page, buf);

	/* disable ECC */
	chal_nand_bch_disable();

	/* Configure ECC for main data */
	bch_config_data(ni);

	/* Perform cache management for page buffer */
	cache_buf(for_device, ni, addr0, size0, dir);
	for (sector = 0; sector < NANDBCH_SECTORS(ni); sector++) {
		dma_addr_t addr;
		uint32_t size = NANDBCH_SECTOR_SIZE(ni);
		enum dma_data_direction dir = DMA_TO_DEVICE;
		sect_buf = ni->buf.phys_addr + sector * NANDBCH_SECTOR_SIZE(ni);
		sect_offs = sector * NANDBCH_SECTOR_SIZE(ni);
		ecc_offs =
			NANDBCH_PAGE_SIZE(ni) + NANDBCH_AUX_SIZE(ni) +
			NANDBCH_AUX_ECC_SIZE(ni) + sector * NANDBCH_ECC_SIZE(ni);
		addr = sect_buf;
		chal_nand_dmaint_set(bank, size, 0);
		cache_desc_for_device(ni, addr, bank, size);

		/* Enable ECC for write */
		chal_nand_bch_write_enable();

		cmd =
			(sector ==
			 0) ? CHAL_NAND_UC_WRITE_PRE : CHAL_NAND_UC_WRITE_RANDOM;

		/* Execute ucode */
		chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[cmd], page,	/* addr */
				     (sect_offs << 16) | NANDBCH_SECTOR_SIZE(ni),	/* attr0 */
				     NANDBCH_ADDR_CYCLES(ni),	/* attr1 */
				     (void *)ni->desc.phys_addr,	/* prdbase */
				     NAND_DIRECTION_TO_NAND);

		/* Wait for completion interrupts */
		if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
			cache_desc(for_cpu, ni);
			return ucode_completion_err(bank, __func__, cmd);
		}
		cache_desc(for_cpu, ni);

		/* disable ECC */
		chal_nand_bch_disable();

		/* retrieve ECC bytes */
		rc = nand_bch_ecc_copy(ni->ecc_buf.buf, NANDBCH_ECC_BYTES(ni),
				       NANDBCH_ECC_SIZE(ni));
		if (rc != NANDBCH_RC_SUCCESS) {
			return rc;
		}
		addr = ni->ecc_buf.phys_addr;
		size = NANDBCH_ECC_SIZE(ni);
		dir = DMA_TO_DEVICE;
		chal_nand_dmaint_set(bank, 0, size);
		cache_desc_buf_for_device(ni, addr, bank, size, dir);

		/* Execute ucode */
		chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_RANDOM], 0,	/* addr */
				     (ecc_offs << 16) | NANDBCH_ECC_SIZE(ni),	/* attr0 */
				     0,	/* attr1 */
				     (void *)ni->desc.phys_addr,	/* prdbase */
				     NAND_DIRECTION_TO_NAND);

		/* Wait for completion interrupts */
		if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
			cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
			return ucode_completion_err(bank, __func__,
						    CHAL_NAND_UC_WRITE_RANDOM);
		}
		cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
	}			/* for each sector of the page */
	cache_buf(for_cpu, ni, addr0, size0, dir);

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_COMPLETE], 0,	/* addr */
			     0,	/* attr0 */
			     0,	/* attr1 */
			     NULL,	/* prdbase */
			     NAND_DIRECTION_TO_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_rb(bank, NANDBCH_TIMEOUT_WRITE)) {
		return ucode_completion_err(bank, __func__,
					    CHAL_NAND_UC_WRITE_COMPLETE);
	}

	do {
		rc = nandbch_status_get(ni, bank, &status);
		if (rc != NANDBCH_RC_SUCCESS)
			return rc;
	} while (!(status & NAND_STATUS_READY));

	return ((status & NAND_STATUS_FAIL) ? NANDBCH_RC_FAIL_STATUS :
		NANDBCH_RC_SUCCESS);
}

/*******************************************************************************
* nandbch_page_aux_write_ecc(ni, bank, page, buf) - Writes page and auxiliary data
*                                                from buf to the nand device
*                                                with BCH_ECC
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @page:    [in] page number
* @buf:     [in] source buffer
*******************************************************************************/
uint32_t nandbch_page_aux_write_ecc(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf)
{
	uint32_t cmd;
	uint32_t rc;
	uint32_t sector;
	uint32_t sect_offs;
	uint32_t ecc_offs;
	uint32_t aux_offs;
	dma_addr_t sect_buf;
	dma_addr_t aux_buf;
	uint8_t status;
	dma_addr_t addr0 = ni->buf.phys_addr;
	uint32_t size0 = NANDBCH_PAGE_SIZE(ni) + NANDBCH_AUX_SIZE(ni);
	enum dma_data_direction dir = DMA_TO_DEVICE;
	dma_addr_t addr;
	uint32_t size;

	if (is_page_protected(ni, page))
		return NANDBCH_RC_FAIL_STATUS;

	memcpy(ni->buf.buf, buf, NANDBCH_PAGE_SIZE(ni) + NANDBCH_AUX_SIZE(ni));

	dprint("%s: bank%u page 0x%x, buf=%p\n", __func__, bank, page, buf);

	/* disable ECC */
	chal_nand_bch_disable();

	/* Configure ECC for main data */
	bch_config_data(ni);

	/* Perform cache management for page buffer */
	cache_buf(for_device, ni, addr0, size0, dir);
	for (sector = 0; sector < NANDBCH_SECTORS(ni); sector++) {
		size = NANDBCH_SECTOR_SIZE(ni);
		sect_buf = ni->buf.phys_addr + sector * NANDBCH_SECTOR_SIZE(ni);
		sect_offs = sector * NANDBCH_SECTOR_SIZE(ni);
		ecc_offs =
			NANDBCH_PAGE_SIZE(ni) + NANDBCH_AUX_SIZE(ni) +
			NANDBCH_AUX_ECC_SIZE(ni) + sector * NANDBCH_ECC_SIZE(ni);
		addr = sect_buf;
		cache_desc_for_device(ni, addr, bank, size);

		/* Enable ECC for write */
		chal_nand_bch_write_enable();

		cmd =
			(sector ==
			 0) ? CHAL_NAND_UC_WRITE_PRE : CHAL_NAND_UC_WRITE_RANDOM;

		/* Execute ucode */
		chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[cmd], page,	/* addr */
				     (sect_offs << 16) | NANDBCH_SECTOR_SIZE(ni),	/* attr0 */
				     NANDBCH_ADDR_CYCLES(ni),	/* attr1 */
				     (void *)ni->desc.phys_addr,	/* prdbase */
				     NAND_DIRECTION_TO_NAND);

		/* Wait for completion interrupts */
		if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
			cache_desc(for_cpu, ni);
			return ucode_completion_err(bank, __func__, cmd);
		}
		cache_desc(for_cpu, ni);

		/* disable ECC */
		chal_nand_bch_disable();

		/* retrieve ECC bytes */
		rc = nand_bch_ecc_copy(ni->ecc_buf.buf, NANDBCH_ECC_BYTES(ni),
				       NANDBCH_ECC_SIZE(ni));
		if (rc != NANDBCH_RC_SUCCESS) {

			// FIXME sync_single_for_cpu for page/aux buffer? Note this is a NOP for a write
			return rc;
		}
		addr = ni->ecc_buf.phys_addr;
		size = NANDBCH_ECC_SIZE(ni);
		dir = DMA_TO_DEVICE;
		chal_nand_dmaint_set(bank, 0, size);
		cache_desc_buf_for_device(ni, addr, bank, size, dir);

		/* Execute ucode */
		chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_RANDOM], 0,	/* addr */
				     (ecc_offs << 16) | NANDBCH_ECC_SIZE(ni),	/* attr0 */
				     0,	/* attr1 */
				     (void *)ni->desc.phys_addr,	/* prdbase */
				     NAND_DIRECTION_TO_NAND);

		/* Wait for completion interrupts */
		if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
			cache_desc_buf_for_cpu(ni, addr, bank, size, dir);

			// FIXME sync_single_for_cpu for page/aux buffer? Note this is a NOP for a write
			return ucode_completion_err(bank, __func__,
						    CHAL_NAND_UC_WRITE_RANDOM);
		}
		cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
	}			/* for each sector of the page */

	/* write auxiliary data to the OOB */
	aux_buf = ni->buf.phys_addr + NANDBCH_PAGE_SIZE(ni);
	aux_offs = NANDBCH_PAGE_SIZE(ni);
	addr = aux_buf;
	size = NANDBCH_AUX_SIZE(ni);
	chal_nand_dmaint_set(bank, 0, size);
	cache_desc_for_device(ni, addr, bank, size);

	/* aux_buf already cleaned above - part of page buffer */
	if (NANDBCH_AUX_ECC_T(ni)) {	/* Write AUX data with ECC */
		/* Configure ECC for auxiliary data */
		bch_config_aux(ni);

		/* Enable ECC for write */
		chal_nand_bch_write_enable();

		/* Execute ucode */
		chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_RANDOM], 0,	/* addr */
				     (aux_offs << 16) | NANDBCH_AUX_SIZE(ni),	/* attr0 */
				     0,	/* attr1 */
				     (void *)ni->desc.phys_addr,	/* prdbase */
				     NAND_DIRECTION_TO_NAND);

		/* Wait for completion interrupts */
		if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
			return ucode_completion_err(bank, __func__,
						    CHAL_NAND_UC_WRITE_RANDOM);
		}

		/* disable ECC */
		chal_nand_bch_disable();

		/* retrieve ECC bytes */
		rc = nand_bch_ecc_copy(ni->ecc_buf.buf,
				       NANDBCH_AUX_ECC_BYTES(ni),
				       NANDBCH_AUX_ECC_SIZE(ni));
		if (rc != NANDBCH_RC_SUCCESS) {
			return rc;
		}
		addr = ni->ecc_buf.phys_addr;
		size = NANDBCH_AUX_ECC_SIZE(ni);
		chal_nand_dmaint_set(bank, 0, size);
		cache_desc_buf_for_device(ni, addr, bank, size, dir);

		/* Execute ucode */
		chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE], 0,	/* addr */
				     NANDBCH_AUX_ECC_SIZE(ni),	/* attr0 */
				     0,	/* attr1 */
				     (void *)ni->desc.phys_addr,	/* prdbase */
				     NAND_DIRECTION_TO_NAND);

		/* Wait for completion interrupts */
		if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
			cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
			return ucode_completion_err(bank, __func__,
						    CHAL_NAND_UC_WRITE);
		}
		cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
	} else {		/* Write AUX data without ECC */

		/* disable ECC */
		chal_nand_bch_disable();

		/* Execute ucode */
		chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_RANDOM], 0,	/* addr */
				     (aux_offs << 16) | NANDBCH_AUX_SIZE(ni),	/* attr0 */
				     0,	/* attr1 */
				     (void *)ni->desc.phys_addr,	/* prdbase */
				     NAND_DIRECTION_TO_NAND);

		/* Wait for completion interrupts */
		if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
			return ucode_completion_err(bank, __func__,
						    CHAL_NAND_UC_WRITE_RANDOM);
		}
	}
	cache_buf(for_cpu, ni, addr0, size0, dir);

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_COMPLETE], 0,	/* addr */
			     0,	/* attr0 */
			     0,	/* attr1 */
			     NULL,	/* prdbase */
			     NAND_DIRECTION_TO_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_rb(bank, NANDBCH_TIMEOUT_WRITE)) {
		return ucode_completion_err(bank, __func__,
					    CHAL_NAND_UC_WRITE_COMPLETE);
	}

	do {
		rc = nandbch_status_get(ni, bank, &status);
		if (rc != NANDBCH_RC_SUCCESS)
			return rc;
	} while (!(status & NAND_STATUS_READY));

	return ((status & NAND_STATUS_FAIL) ? NANDBCH_RC_FAIL_STATUS :
		NANDBCH_RC_SUCCESS);
}

/*******************************************************************************
* nandbch_aux_write_ecc(ni, bank, page, buf) - Writes auxiliary data from buf to
*                                           to the nand device with BCH_ECC
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @page:    [in] page number
* @buf:     [in] source buffer
*******************************************************************************/
uint32_t nandbch_aux_write_ecc(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf)
{
	uint32_t aux_offs;
	uint32_t rc;
	uint8_t status;
	dma_addr_t addr = ni->buf.phys_addr;
	uint32_t size = NANDBCH_AUX_SIZE(ni);
	enum dma_data_direction dir = DMA_TO_DEVICE;

	if (is_page_protected(ni, page))
		return NANDBCH_RC_FAIL_STATUS;

	memcpy(ni->buf.buf, buf, size);

	dprint("%s: bank%u page 0x%x, buf=%p\n", __func__, bank, page, buf);

	/* disable ECC */
	chal_nand_bch_disable();

	aux_offs = NANDBCH_PAGE_SIZE(ni);
	chal_nand_dmaint_set(bank, 0, size);
	cache_desc_buf_for_device(ni, addr, bank, size, dir);

	/* Configure ECC for auxiliary data */
	bch_config_aux(ni);

	/* Enable ECC for write */
	chal_nand_bch_write_enable();

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_PRE], page,	/* addr */
			     (aux_offs << 16) | NANDBCH_AUX_SIZE(ni),	/* attr0 */
			     NANDBCH_ADDR_CYCLES(ni),	/* attr1 */
			     (void *)ni->desc.phys_addr,	/* prdbase */
			     NAND_DIRECTION_TO_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
		cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
		return ucode_completion_err(bank, __func__,
					    CHAL_NAND_UC_WRITE_PRE);
	}
	cache_desc_buf_for_cpu(ni, addr, bank, size, dir);

	/* disable ECC */
	chal_nand_bch_disable();

	/* retrieve ECC bytes */
	rc = nand_bch_ecc_copy(ni->ecc_buf.buf, NANDBCH_AUX_ECC_BYTES(ni),
			       NANDBCH_AUX_ECC_SIZE(ni));
	if (rc != NANDBCH_RC_SUCCESS) {
		return rc;
	}
	addr = ni->ecc_buf.phys_addr;
	size = NANDBCH_AUX_ECC_SIZE(ni);
	dir = DMA_TO_DEVICE;
	chal_nand_dmaint_set(bank, 0, size);
	cache_desc_buf_for_device(ni, addr, bank, size, dir);

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE], 0,	/* addr */
			     NANDBCH_AUX_ECC_SIZE(ni),	/* attr0 */
			     0,	/* attr1 */
			     (void *)ni->desc.phys_addr,	/* prdbase */
			     NAND_DIRECTION_TO_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
		cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
		return ucode_completion_err(bank, __func__, CHAL_NAND_UC_WRITE);
	}
	cache_desc_buf_for_cpu(ni, addr, bank, size, dir);

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_COMPLETE], 0,	/* addr */
			     0,	/* attr0 */
			     0,	/* attr1 */
			     NULL,	/* prdbase */
			     NAND_DIRECTION_TO_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_rb(bank, NANDBCH_TIMEOUT_WRITE)) {
		return ucode_completion_err(bank, __func__,
					    CHAL_NAND_UC_WRITE_COMPLETE);
	}

	do {
		rc = nandbch_status_get(ni, bank, &status);
		if (rc != NANDBCH_RC_SUCCESS)
			return rc;
	} while (!(status & NAND_STATUS_READY));

	return ((status & NAND_STATUS_FAIL) ? NANDBCH_RC_FAIL_STATUS :
		NANDBCH_RC_SUCCESS);
}

/*******************************************************************************
* nandbch_feature_get(ni, bank, feature, buf) - Get ONFI feature
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @feature:	[in] feature address
* @buf:	   [out] buffer to get the feature value (4 bytes)
*******************************************************************************/
uint32_t nandbch_feature_get(
	nandbch_info_t * ni,
	uint8_t bank,
	uint8_t feature,
	uint8_t * buf)
{
	dma_addr_t addr = ni->buf.phys_addr;
	uint32_t size = NANDBCH_FEATURE_SIZE;
	enum dma_data_direction dir = DMA_FROM_DEVICE;

	dprint("%s: bank%u feature 0x%x, buf=%p\n", __func__, bank, feature,
	       buf);

	/* disable ECC */
	chal_nand_bch_disable();

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_GET_FEATURE_PRE], feature,	/* addr */
			     0,	/* attr0 */
			     0,	/* attr1 */
			     0,	/* prdbase */
			     NAND_DIRECTION_FROM_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_rb(bank, NANDBCH_TIMEOUT_FEATURE)) {
		return ucode_completion_err(bank, __func__,
					    CHAL_NAND_UC_GET_FEATURE_PRE);
	}
	chal_nand_dmaint_set(bank, 0, size);
	cache_desc_buf_for_device(ni, addr, bank, size, dir);

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_READ], 0,	/* addr */
			     NANDBCH_FEATURE_SIZE,	/* attr0 */
			     0,	/* attr1 */
			     (void *)ni->desc.phys_addr,	/* prdbase */
			     NAND_DIRECTION_FROM_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
		cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
		return ucode_completion_err(bank, __func__, CHAL_NAND_UC_READ);
	}
	cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
	memcpy(buf, ni->buf.buf, NANDBCH_FEATURE_SIZE);
	return NANDBCH_RC_SUCCESS;
}

/*******************************************************************************
* nandbch_feature_set(ni, bank, feature, buf) - Set ONFI feature
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @feature:	[in] feature address
* @buf:	   [out] buffer containing the feature value (4 bytes)
*******************************************************************************/
uint32_t nandbch_feature_set(
	nandbch_info_t * ni,
	uint8_t bank,
	uint8_t feature,
	uint8_t * buf)
{
	dma_addr_t addr = ni->buf.phys_addr;
	uint32_t size = NANDBCH_FEATURE_SIZE;
	enum dma_data_direction dir = DMA_TO_DEVICE;
	memcpy(ni->buf.buf, buf, NANDBCH_FEATURE_SIZE);
	chal_nand_dmaint_set(bank, 0, size);
	cache_desc_buf_for_device(ni, addr, bank, size, dir);

	dprint("%s: bank%u feature 0x%x, buf=%p\n", __func__, bank, feature,
	       buf);

	/* disable ECC */
	chal_nand_bch_disable();

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_SET_FEATURE], feature,	/* addr */
			     0,	/* attr0 */
			     0,	/* attr1 */
			     (void *)ni->desc.phys_addr,	/* prdbase */
			     NAND_DIRECTION_TO_NAND);

	/* Wait for completion interrupts */
	if (wait_for_ucode_completion(bank,
				      (NAND_IRQSTATUS_DMA_CMPL_IRQ_MASK |
				       NANDBCH_IRQ_BANK_RB(bank)),
				      NANDBCH_TIMEOUT_FEATURE)) {
		cache_desc_buf_for_device(ni, addr, bank, size, dir);
		return ucode_completion_err(bank, __func__,
					    CHAL_NAND_UC_SET_FEATURE);
	}
	cache_desc_buf_for_device(ni, addr, bank, size, dir);
	return NANDBCH_RC_SUCCESS;
}

/*******************************************************************************
* nandbch_oob_read(ni, bank, page, buf) - Reads used oob bytes (aux data, aux data
*                                      ECC bytes and main data ECC bytes) from
*                                      nand into buf without ECC correction
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @page:    [in] page number
* @buf:     [out] target buffer
*******************************************************************************/
uint32_t nandbch_oob_read(nandbch_info_t * ni, uint8_t bank,
			  uint32_t page, uint8_t * buf)
{
	dma_addr_t addr = ni->buf.phys_addr;
	uint32_t size = 
		NANDBCH_AUX_SIZE(ni) + NANDBCH_AUX_ECC_SIZE(ni) +
		NANDBCH_SECTORS(ni) * NANDBCH_ECC_SIZE(ni);
	uint32_t offs = 0;
	enum dma_data_direction dir = DMA_FROM_DEVICE;

	uint32_t len, chunk, aux_type_size;

	/* disable ECC */
	chal_nand_bch_disable();

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_READ_PRE], page,	/* addr */
			     0,	/* attr0 */
			     NANDBCH_ADDR_CYCLES(ni),	/* attr1 */
			     NULL,	/* prdbase */
			     NAND_DIRECTION_FROM_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_rb(bank, NANDBCH_TIMEOUT_READ)) {
		return ucode_completion_err(bank, __func__,
					    CHAL_NAND_UC_READ_PRE);
	}

	/* 
	 * Read OOB bytes in several chunks because of the size limitation
	 * imposed by DMAINT register (HWCAPRI-385)
	 */

	/* round up size to multiple of auxiliary data type */
	aux_type_size = chal_nand_get_aux_data_type_size();
	size = ((size + aux_type_size - 1) / aux_type_size) * aux_type_size;
	chunk = 0x10 * aux_type_size;
	while (size) {
		if (size > chunk) {
			len = chunk;
		}

		else {
			len = size;
		}

		/* Setup DMA descriptor */
		chal_nand_dmaint_set(bank, 0, len);
		cache_desc_buf_for_device(ni, addr, bank, len, dir);
		size -= len;

		/* Execute ucode */
		chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)
				     [CHAL_NAND_UC_READ_RANDOM], 0,	/* addr */
				     ((offs+NANDBCH_PAGE_SIZE(ni)) << 16) | len,	/* attr0 */
				     0,	/* attr1 */
				     (void *)ni->desc.phys_addr,	/* prdbase */
				     NAND_DIRECTION_FROM_NAND);

		/* Wait for completion interrupts */
		if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
			cache_desc_buf_for_cpu(ni, addr, bank, len, dir);
			return ucode_completion_err(bank, __func__,
						    CHAL_NAND_UC_READ);
		}
		cache_desc_buf_for_cpu(ni, addr, bank, len, dir);
		memcpy(buf + offs, ni->buf.buf, len);
		offs += len;
	}
	return NANDBCH_RC_SUCCESS;
}

/*******************************************************************************
* nandbch_oob_write(ni, bank, page, buf) - Writes oob bytes (aux data, aux data
*                                      ECC bytes and main data ECC bytes) to
*                                      nand from buf without ECC correction
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @page:    [in] page number
* @buf:     [in] target buffer
*******************************************************************************/
uint32_t nandbch_oob_write(nandbch_info_t * ni, uint8_t bank,
			  uint32_t page, uint8_t * buf)
{
	uint32_t rc;
	uint8_t status;
	dma_addr_t addr = ni->buf.phys_addr;
	uint32_t size = 
	    NANDBCH_AUX_SIZE(ni) + NANDBCH_AUX_ECC_SIZE(ni) +
	    NANDBCH_SECTORS(ni) * NANDBCH_ECC_SIZE(ni);
	uint32_t offs = 0;
	enum dma_data_direction dir = DMA_TO_DEVICE;

	uint32_t cmd, len, chunk, aux_type_size;

	if (is_page_protected(ni, page))
		return NANDBCH_RC_FAIL_STATUS;

	/* disable ECC */
	chal_nand_bch_disable();

	/* 
	 * Write OOB bytes in several chunks because of the size limitation
	 * imposed by DMAINT register (HWCAPRI-385)
	 */

	/* round up size to multiple of auxiliary data type */
	aux_type_size = chal_nand_get_aux_data_type_size();
	size = ((size + aux_type_size - 1) / aux_type_size) * aux_type_size;
	chunk = 0x10 * aux_type_size;
	while (size) {
		if (size > chunk) {
			len = chunk;
		}

		else {
			len = size;
		}

		/* Setup DMA descriptor */
		chal_nand_dmaint_set(bank, 0, len);
		memcpy(ni->buf.buf, buf + offs, len);
		cache_desc_buf_for_device(ni, addr, bank, len, dir);
		size -= len;

		cmd = (offs == 0) ? CHAL_NAND_UC_WRITE_PRE : CHAL_NAND_UC_WRITE_RANDOM;

		/* Execute ucode */
		chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[cmd], page,	/* addr */
				     ((offs+NANDBCH_PAGE_SIZE(ni)) << 16) | len,	/* attr0 */
			             NANDBCH_ADDR_CYCLES(ni),	/* attr1 */
				     (void *)ni->desc.phys_addr,	/* prdbase */
				     NAND_DIRECTION_TO_NAND);

		/* Wait for completion interrupts */
		if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
			cache_desc_buf_for_cpu(ni, addr, bank, len, dir);
			return ucode_completion_err(bank, __func__, cmd);
		}
		cache_desc_buf_for_cpu(ni, addr, bank, len, dir);
		offs += len;
	}

	/* Execute ucode */
	chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_COMPLETE], 0,	/* addr */
			     0,	/* attr0 */
			     0,	/* attr1 */
			     NULL,	/* prdbase */
			     NAND_DIRECTION_TO_NAND);

	/* Wait for completion interrupts */
	if (wait_ucode_bank_rb(bank, NANDBCH_TIMEOUT_WRITE)) {
		return ucode_completion_err(bank, __func__,
					    CHAL_NAND_UC_WRITE_COMPLETE);
	}

	do {
		rc = nandbch_status_get(ni, bank, &status);
		if (rc != NANDBCH_RC_SUCCESS)
			return rc;
	} while (!(status & NAND_STATUS_READY));

	return ((status & NAND_STATUS_FAIL) ? NANDBCH_RC_FAIL_STATUS :
		NANDBCH_RC_SUCCESS);
	return NANDBCH_RC_SUCCESS;
}
/*******************************************************************************
*
*     CS INTERLEAVED MODE FUNCTIONS
*
*******************************************************************************/

/*******************************************************************************
* nandbch_page_read_ileaved(ni, page0, page1, buf0, buf1, stats0, stats1) - Reads
* one page from each bank (CS iterleaved mode)
* @ni:      [in] nand info structure
* @page0:   [in] page number bank 0
* @page1:   [in] page number bank 1
* @buf0:    [out] target page buffer bank 0
* @buf1:    [out] target page buffer bank 1
* @stats0:  [out] ECC stats bank 0 (number of bad bits per sector)
* @stats0:  [out] ECC stats bank 1 (number of bad bits per sector)
*
* Note: If the caller wants ECC correction stats it needs to provide a pointer
* to a nandbch_eccstats_t structure. The number of bits corrected
* by ECC in every sector will be returned in the corresponding array element.
* If stats are not needed caller should provide a NULL pointer argument.
*******************************************************************************/
uint32_t nandbch_page_read_ileaved(
	nandbch_info_t * ni,
	uint32_t page0,
	uint32_t page1,
	uint8_t * buf0,
	uint8_t * buf1,
	nandbch_eccstats_t * stats0,
	nandbch_eccstats_t * stats1)
{
	if (NANDBCH_FLAGS(ni) & NANDBCH_FLAG_ECC) {
		return nandbch_page_read_ileaved_ecc(ni, page0, page1, buf0,
						     buf1, stats0, stats1);
	}
	return nandbch_page_read_ileaved_raw(ni, page0, page1, buf0, buf1);
}

/*******************************************************************************
* nandbch_page_read_ileaved_ecc(ni, page0, page1, buf0, buf1, stats0, stats1) - Reads
* one page from each bank with ECC correction (CS iterleaved mode)
* @ni:      [in] nand info structure
* @page0:   [in] page number bank 0
* @page1:   [in] page number bank 1
* @buf0:    [out] target page buffer bank 0
* @buf1:    [out] target page buffer bank 1
* @stats0:  [out] ECC stats bank 0 (number of bad bits per sector)
* @stats0:  [out] ECC stats bank 1 (number of bad bits per sector)
*
* Note: If the caller wants ECC correction stats it needs to provide a pointer
* to a nandbch_eccstats_t structure. The number of bits corrected
* by ECC in every sector will be returned in the corresponding array element.
* If stats are not needed caller should provide a NULL pointer argument.
*******************************************************************************/
uint32_t nandbch_page_read_ileaved_ecc(
	nandbch_info_t * ni,
	uint32_t page0,
	uint32_t page1,
	uint8_t * buf0,
	uint8_t * buf1,
	nandbch_eccstats_t * stats0,
	nandbch_eccstats_t * stats1)
{
	uint8_t bank;
	uint8_t *buf;
	nandbch_eccstats_t *stats;
	uint32_t sector;
	uint32_t sect_offs;
	uint32_t ecc_offs;
	int32_t err_count;
	dma_addr_t sect_buf;
	uint32_t cmd;
	uint32_t rc;

	/* disable ECC */
	chal_nand_bch_disable();

	for (bank = 0; bank < 2; bank++) {
		/* Execute ucode */
		chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_READ_PRE], bank ? page1 : page0,	/* addr */
				     0,	/* attr0 */
				     NANDBCH_ADDR_CYCLES(ni),	/* attr1 */
				     NULL,	/* prdbase */
				     NAND_DIRECTION_FROM_NAND);

		/* Wait for bank completion interrupt */
		if (wait_ucode_bank(bank, NANDBCH_TIMEOUT_READ)) {
			return ucode_completion_err(bank, __func__,
						    CHAL_NAND_UC_READ_PRE);
		}
	}			/* for each bank */

	/* Configure ECC for main data */
	bch_config_data(ni);

	for (bank = 0; bank < 2; bank++) {

		buf = (bank ? buf1 : buf0);
		stats = (bank ? stats1 : stats0);

		/* Wait for bank ready interrupt */
		if (wait_ucode_irq_rb(bank, NANDBCH_TIMEOUT_READ)) {
			return ucode_completion_err(bank, __func__,
						    CHAL_NAND_UC_READ_PRE);
		}

		for (sector = 0; sector < NANDBCH_SECTORS(ni); sector++) {
			dma_addr_t addr;
			uint32_t size = NANDBCH_SECTOR_SIZE(ni);
			dma_addr_t addr2 = ni->ecc_buf.phys_addr;
			uint32_t size2 = NANDBCH_ECC_SIZE(ni);
			enum dma_data_direction dir = DMA_FROM_DEVICE;
			sect_offs = sector * NANDBCH_SECTOR_SIZE(ni);
			sect_buf = ni->buf.phys_addr + sect_offs;
			ecc_offs =
				NANDBCH_PAGE_SIZE(ni) + NANDBCH_AUX_SIZE(ni) +
				NANDBCH_AUX_ECC_SIZE(ni) +
				sector * NANDBCH_ECC_SIZE(ni);
			addr = sect_buf;
			chal_nand_dmaint_set(bank, size, size2);
			cache_desc2_buf_for_device(ni, addr, bank, size, addr2,
						   bank, size2, dir);

			/* Enable ECC for read */
			chal_nand_bch_read_enable();

			cmd =
				(sector ==
				 0) ? CHAL_NAND_UC_READ_ECC :
				CHAL_NAND_UC_READ_RANDOM_ECC;
			/* Execute ucode */
			chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[cmd], 0,	/* addr */
					     (sect_offs << 16) | NANDBCH_SECTOR_SIZE(ni),	/* attr0 */
					     (ecc_offs << 16) | NANDBCH_ECC_SIZE(ni),	/* attr1 */
					     (void *)ni->desc.phys_addr,	/* prdbase */
					     NAND_DIRECTION_FROM_NAND);

			/* Wait for completion interrupts */
			if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
				cache_desc_buf_for_device(ni, addr, bank, size,
							  dir);
				return ucode_completion_err(bank,
							    __func__, cmd);
			}
			cache_desc_buf_for_device(ni, addr, bank, size, dir);

			/* Disable ECC */
			chal_nand_bch_disable();

			/* Check for BCH ECC errors */
			rc = nand_bch_err_count(&err_count);
			if (rc != NANDBCH_RC_SUCCESS) {
				return rc;
			}

			/* Uncorrectable BCH ECC errors */
			if (err_count == -1) {
				return NANDBCH_RC_ECC_ERROR;
			}

			/* Fix all correctable BCH ECC errors */
			if (err_count > 0) {
				rc = nand_bch_err_fix(err_count, (uint8_t *)
						      (uint8_t *)(ni->buf.buf + sect_offs),
						      NANDBCH_SECTOR_SIZE(ni));
				if (rc != NANDBCH_RC_SUCCESS) {
					return rc;
				}
			}

			/* Set the ECC stats for the sector */
			if (stats) {
				stats->errs[sector] = err_count;
			}

		}		/* for each sector */
		if (stats) {
			stats->len = NANDBCH_SECTORS(ni);
		}
		memcpy(buf, ni->buf.buf, NANDBCH_PAGE_SIZE(ni));
	}			/* for each bank */

	return NANDBCH_RC_SUCCESS;
}

/*******************************************************************************
* nandbch_page_read_ileaved_raw(ni, page0, page1, buf0, buf1 - Reads one page from
* each bank without ECC correction (CS iterleaved mode)
* @ni:      [in] nand info structure
* @page0:   [in] page number bank 0
* @page1:   [in] page number bank 1
* @buf0:    [out] target page buffer bank 0
* @buf1:    [out] target page buffer bank 1
*******************************************************************************/
uint32_t nandbch_page_read_ileaved_raw(
	nandbch_info_t * ni,
	uint32_t page0,
	uint32_t page1,
	uint8_t * buf0,
	uint8_t * buf1)
{
	uint8_t bank;
	uint8_t *buf;

	/* disable ECC */
	chal_nand_bch_disable();

	for (bank = 0; bank < 2; bank++) {
		/* Execute ucode */
		chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_READ_PRE], bank ? page1 : page0,	/* addr */
				     0,	/* attr0 */
				     NANDBCH_ADDR_CYCLES(ni),	/* attr1 */
				     NULL,	/* prdbase */
				     NAND_DIRECTION_FROM_NAND);

		/* Wait for bank completion interrupt */
		if (wait_ucode_bank(bank, NANDBCH_TIMEOUT_READ)) {
			return ucode_completion_err(bank, __func__,
						    CHAL_NAND_UC_READ_PRE);
		}

	}			/* for each bank */

	for (bank = 0; bank < 2; bank++) {
		dma_addr_t addr = ni->buf.phys_addr;
		uint32_t size = NANDBCH_PAGE_SIZE(ni);
		enum dma_data_direction dir = DMA_FROM_DEVICE;
		buf = (bank ? buf1 : buf0);

		/* Wait for bank ready interrupt */
		if (wait_ucode_irq_rb(bank, NANDBCH_TIMEOUT_READ)) {
			return ucode_completion_err(bank, __func__,
						    CHAL_NAND_UC_READ_PRE);
		}
		chal_nand_dmaint_set(bank, size, 0);
		cache_desc_buf_for_device(ni, addr, bank, size, dir);

		/* Execute ucode */
		chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_READ], 0,	/* addr */
				     NANDBCH_PAGE_SIZE(ni),	/* attr0 */
				     0,	/* attr1 */
				     (void *)ni->desc.phys_addr,	/* prdbase */
				     NAND_DIRECTION_FROM_NAND);

		/* Wait for completion interrupts */
		if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
			cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
			return ucode_completion_err(bank, __func__,
						    CHAL_NAND_UC_READ);
		}
		cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
		memcpy(buf, ni->buf.buf, size);
	}			/* for each bank */

	return NANDBCH_RC_SUCCESS;
}

/*******************************************************************************
* nandbch_block_force_erase_ileaved(ni, block0, block1) - Erase one block from each
* bank ignoring bad block marks (CS interleaved mode)
* @ni:      [in] nand info structure
* @block0:	[in] block number from bank 0
* @block1:  [in] block number from bank 1
*******************************************************************************/
uint32_t nandbch_block_force_erase_ileaved(
	nandbch_info_t * ni,
	uint32_t block0,
	uint32_t block1)
{
	uint8_t bank;
	uint32_t block;
	uint32_t rc;
	uint8_t *status;
	uint8_t status0;
	uint8_t status1;

	if (is_block_protected(block0) || is_block_protected(block1))
		return NANDBCH_RC_FAIL_STATUS;
	/* disable ECC */
	chal_nand_bch_disable();

	for (bank = 0; bank < 2; bank++) {
		block = bank ? block1 : block0;

		/* Execute ucode */
		chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_BLOCK_ERASE], block << (NANDBCH_BLOCK_SHIFT(ni) - NANDBCH_PAGE_SHIFT(ni)),	/* addr */
				     0,	/* attr0 */
				     NANDBCH_ADDR_CYCLES(ni) - 2,	/* attr1 */
				     NULL,	/* prdbase */
				     NAND_DIRECTION_FROM_NAND);

		/* Wait for completion interrupts */
		if (wait_ucode_bank(bank, NANDBCH_TIMEOUT_ERASE)) {
			return ucode_completion_err(bank, __func__,
						    CHAL_NAND_UC_BLOCK_ERASE);
		}

	}			/* for each bank */

	for (bank = 0; bank < 2; bank++) {
		/* Wait for bank ready interrupt */
		if (wait_ucode_irq_rb(bank, NANDBCH_TIMEOUT_ERASE)) {
			return ucode_completion_err(bank, __func__,
						    CHAL_NAND_UC_BLOCK_ERASE);
		}

		status = bank ? &status1 : &status0;
		do {
			rc = nandbch_status_get(ni, bank, status);
			if (rc != NANDBCH_RC_SUCCESS)
				return rc;
		} while (!(*status & NAND_STATUS_READY));

	}			/* for each bank */

	return (((status0 & NAND_STATUS_FAIL)
		 || (status1 & NAND_STATUS_FAIL)) ?
		NANDBCH_RC_FAIL_STATUS : NANDBCH_RC_SUCCESS);
}

/*******************************************************************************
* nandbch_block_erase_ileaved(ni, block0, block1)  - Erase a block from each bank
* (CS interleaved mode)
* @ni:      [in] nand info structure
* @block0:	[in] block number from bank 0
* @block1:  [in] block number from bank 1
*******************************************************************************/
uint32_t nandbch_block_erase_ileaved(
	nandbch_info_t * ni,
	uint32_t block0,
	uint32_t block1)
{
	uint8_t bank;
	uint32_t block;
	uint32_t rc;
	uint8_t isbad;

	if (is_block_protected(block0) || is_block_protected(block1))
		return NANDBCH_RC_FAIL_STATUS;

	for (bank = 0; bank < 2; bank++) {
		block = bank ? block1 : block0;

		rc = nandbch_block_isbad(ni, bank, block, &isbad);
		if (rc != NANDBCH_RC_SUCCESS)
			return rc;
		if (isbad) {
			return NANDBCH_RC_BB_NOERASE;
		}

		rc = nandbch_block_force_erase(ni, bank, block);
		if (rc != NANDBCH_RC_SUCCESS)
			return rc;

	}			/* for each bank */

	return NANDBCH_RC_SUCCESS;
}

/*******************************************************************************
* nandbch_page_write_ileaved(ni, page0, page1, buf0, buf1) - Writes one page to
* each bank (CS interleaved mode)
* @ni:      [in] nand info structure
* @page0:   [in] page number bank 0
* @page1:   [in] page number bank 1
* @buf0:    [out] source page buffer bank 0
* @buf1:    [out] source page buffer bank 1
*******************************************************************************/
uint32_t nandbch_page_write_ileaved(
	nandbch_info_t * ni,
	uint32_t page0,
	uint32_t page1,
	uint8_t * buf0,
	uint8_t * buf1)
{
	if (NANDBCH_FLAGS(ni) & NANDBCH_FLAG_ECC) {
		return nandbch_page_write_ileaved_ecc(ni, page0,
						      page1, buf0, buf1);
	}
	return nandbch_page_write_ileaved_raw(ni, page0, page1, buf0, buf1);
}

/*******************************************************************************
* nandbch_page_write_ileaved_raw(ni, page0, page1, buf0, buf1) - Writes one page to
* each bank without ECC (CS interleaved mode)
* @ni:      [in] nand info structure
* @page0:   [in] page number bank 0
* @page1:   [in] page number bank 1
* @buf0:    [out] source page buffer bank 0
* @buf1:    [out] source page buffer bank 1
*******************************************************************************/
uint32_t nandbch_page_write_ileaved_raw(
	nandbch_info_t * ni,
	uint32_t page0,
	uint32_t page1,
	uint8_t * buf0,
	uint8_t * buf1)
{
	uint32_t rc;
	uint8_t bank;
	uint8_t *buf;
	uint8_t *status;
	uint8_t status0;
	uint8_t status1;

	if (is_page_protected(ni, page0) || is_page_protected(ni, page1))
		return NANDBCH_RC_FAIL_STATUS;

	/* disable ECC */
	chal_nand_bch_disable();

	for (bank = 0; bank < 2; bank++) {
		dma_addr_t addr = ni->buf.phys_addr;
		uint32_t size = NANDBCH_PAGE_SIZE(ni);
		enum dma_data_direction dir = DMA_TO_DEVICE;
		buf = (bank ? buf1 : buf0);
		memcpy(ni->buf.buf, buf, size);
		chal_nand_dmaint_set(bank, size, 0);
		cache_desc_buf_for_device(ni, addr, bank, size, dir);

		/* Execute ucode */
		chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_PRE], bank ? page1 : page0,	/* addr */
				     NANDBCH_PAGE_SIZE(ni),	/* attr0 */
				     NANDBCH_ADDR_CYCLES(ni),	/* attr1 */
				     (void *)ni->desc.phys_addr,	/* prdbase */
				     NAND_DIRECTION_TO_NAND);

		/* Wait for completion interrupts */
		if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
			cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
			return ucode_completion_err(bank, __func__,
						    CHAL_NAND_UC_WRITE_PRE);
		}
		cache_desc_buf_for_cpu(ni, addr, bank, size, dir);

		/* Execute ucode */
		chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_COMPLETE], 0,	/* addr */
				     0,	/* attr0 */
				     0,	/* attr1 */
				     NULL,	/* prdbase */
				     NAND_DIRECTION_TO_NAND);
		/* Wait for completion interrupts */
		if (wait_ucode_bank(bank, NANDBCH_TIMEOUT_WRITE)) {
			return ucode_completion_err(bank, __func__,
						    CHAL_NAND_UC_WRITE_COMPLETE);
		}

	}			/* for each bank */

	for (bank = 0; bank < 2; bank++) {
		/* Wait for bank ready interrupt */
		if (wait_ucode_irq_rb(bank, NANDBCH_TIMEOUT_WRITE)) {
			return ucode_completion_err(bank, __func__,
						    CHAL_NAND_UC_WRITE_COMPLETE);
		}

		status = bank ? &status1 : &status0;
		do {
			rc = nandbch_status_get(ni, bank, status);
			if (rc != NANDBCH_RC_SUCCESS)
				return rc;
		} while (!(*status & NAND_STATUS_READY));

	}			/* for each bank */

	return (((status0 & NAND_STATUS_FAIL)
		 || (status1 & NAND_STATUS_FAIL)) ?
		NANDBCH_RC_FAIL_STATUS : NANDBCH_RC_SUCCESS);
}

/*******************************************************************************
* nandbch_page_write_ileaved_ecc(ni, page0, page1, buf0, buf1) - Writes one page to
* each bank with ECC (CS interleaved mode)
* @ni:      [in] nand info structure
* @page0:   [in] page number bank 0
* @page1:   [in] page number bank 1
* @buf0:    [out] source page buffer bank 0
* @buf1:    [out] source page buffer bank 1
*******************************************************************************/
uint32_t nandbch_page_write_ileaved_ecc(
	nandbch_info_t * ni,
	uint32_t page0,
	uint32_t page1,
	uint8_t * buf0,
	uint8_t * buf1)
{
	uint32_t cmd;
	uint32_t rc;
	uint32_t sector;
	uint32_t sect_offs;
	uint32_t ecc_offs;
	dma_addr_t sect_buf;
	uint8_t bank;
	uint8_t *buf;
	uint8_t *status;
	uint8_t status0;
	uint8_t status1;

	if (is_page_protected(ni, page0) || is_page_protected(ni, page1))
		return NANDBCH_RC_FAIL_STATUS;
	/* disable ECC */
	chal_nand_bch_disable();

	/* Configure ECC for main data */
	bch_config_data(ni);

	for (bank = 0; bank < 2; bank++) {
		dma_addr_t addr0 = ni->buf.phys_addr;
		uint32_t size0 = NANDBCH_PAGE_SIZE(ni);
		enum dma_data_direction dir = DMA_TO_DEVICE;
		buf = (bank ? buf1 : buf0);
		memcpy(ni->buf.buf, buf, size0);

		/* Perform cache management for page buffer */
		cache_buf(for_device, ni, addr0, size0, dir);
		for (sector = 0; sector < NANDBCH_SECTORS(ni); sector++) {
			dma_addr_t addr;
			uint32_t size = NANDBCH_SECTOR_SIZE(ni);
			sect_buf =
				ni->buf.phys_addr +
				sector * NANDBCH_SECTOR_SIZE(ni);
			sect_offs = sector * NANDBCH_SECTOR_SIZE(ni);
			ecc_offs =
				NANDBCH_PAGE_SIZE(ni) + NANDBCH_AUX_SIZE(ni) +
				NANDBCH_AUX_ECC_SIZE(ni) +
				sector * NANDBCH_ECC_SIZE(ni);
			addr = sect_buf;
			chal_nand_dmaint_set(bank, size, 0);
			cache_desc_for_device(ni, addr, bank, size);

			/* Enable ECC for write */
			chal_nand_bch_write_enable();

			cmd =
				(sector ==
				 0) ? CHAL_NAND_UC_WRITE_PRE :
				CHAL_NAND_UC_WRITE_RANDOM;

			/* Execute ucode */
			chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[cmd], bank ? page1 : page0,	/* addr */
					     (sect_offs << 16) | NANDBCH_SECTOR_SIZE(ni),	/* attr0 */
					     NANDBCH_ADDR_CYCLES(ni),	/* attr1 */
					     (void *)ni->desc.phys_addr,	/* prdbase */
					     NAND_DIRECTION_TO_NAND);

			/* Wait for completion interrupts */
			if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
				cache_desc(for_cpu, ni);
				return ucode_completion_err(bank,
							    __func__, cmd);
			}
			cache_desc(for_cpu, ni);

			/* disable ECC */
			chal_nand_bch_disable();

			/* retrieve ECC bytes */
			rc = nand_bch_ecc_copy(ni->ecc_buf.buf,
					       NANDBCH_ECC_BYTES(ni),
					       NANDBCH_ECC_SIZE(ni));
			if (rc != NANDBCH_RC_SUCCESS) {
				return rc;
			}
			addr = ni->ecc_buf.phys_addr;
			size = NANDBCH_ECC_SIZE(ni);
			chal_nand_dmaint_set(bank, 0, size);
			cache_desc_buf_for_device(ni, addr, bank, size, dir);

			/* Execute ucode */
			chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_RANDOM], 0,	/* addr */
					     (ecc_offs << 16) | NANDBCH_ECC_SIZE(ni),	/* attr0 */
					     0,	/* attr1 */
					     (void *)ni->desc.phys_addr,	/* prdbase */
					     NAND_DIRECTION_TO_NAND);

			/* Wait for completion interrupts */
			if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
				cache_desc_buf_for_cpu(ni, addr, bank, size,
						       dir);
				return ucode_completion_err(bank,
							    __func__,
							    CHAL_NAND_UC_WRITE_RANDOM);
			}
			cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
		}		/* for each sector of the page */
		cache_buf(for_cpu, ni, addr0, size0, dir);

		/* Execute ucode */
		chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_COMPLETE], 0,	/* addr */
				     0,	/* attr0 */
				     0,	/* attr1 */
				     NULL,	/* prdbase */
				     NAND_DIRECTION_TO_NAND);

		/* Wait for completion interrupt */
		if (wait_ucode_bank(bank, NANDBCH_TIMEOUT_WRITE)) {
			return ucode_completion_err(bank, __func__,
						    CHAL_NAND_UC_WRITE_COMPLETE);
		}

	}			/* for each bank */

	for (bank = 0; bank < 2; bank++) {
		/* Wait for ready interrupt */
		if (wait_ucode_irq_rb(bank, NANDBCH_TIMEOUT_WRITE)) {
			return ucode_completion_err(bank, __func__,
						    CHAL_NAND_UC_WRITE_COMPLETE);
		}

		status = bank ? &status1 : &status0;
		do {
			rc = nandbch_status_get(ni, bank, status);
			if (rc != NANDBCH_RC_SUCCESS)
				return rc;
		} while (!(*status & NAND_STATUS_READY));
	}			/* for each bank */

	return (((status0 & NAND_STATUS_FAIL)
		 || (status1 & NAND_STATUS_FAIL)) ?
		NANDBCH_RC_FAIL_STATUS : NANDBCH_RC_SUCCESS);
}

/*******************************************************************************
* nandbch_page_aux_write_ileaved(ni, page0, page1, buf0, buf1) - Writes one page
* and aux data to each bank (CS interleaved mode)
* @ni:      [in] nand info structure
* @page0:   [in] page number bank 0
* @page1:   [in] page number bank 1
* @buf0:    [out] source page buffer bank 0
* @buf1:    [out] source page buffer bank 1
*******************************************************************************/
uint32_t nandbch_page_aux_write_ileaved(
	nandbch_info_t * ni,
	uint32_t page0,
	uint32_t page1,
	uint8_t * buf0,
	uint8_t * buf1)
{
	if (NANDBCH_FLAGS(ni) & NANDBCH_FLAG_ECC) {
		return nandbch_page_aux_write_ileaved_ecc(ni, page0,
				page1, buf0, buf1);
	}
	return nandbch_page_aux_write_ileaved_raw(ni, page0, page1, buf0, buf1);
}

/*******************************************************************************
* nandbch_page_aux_write_ileaved_raw(ni, page0, page1, buf0, buf1) - Writes one
* page and aux data to each bank without ECC (CS interleaved mode)
* @ni:      [in] nand info structure
* @page0:   [in] page number bank 0
* @page1:   [in] page number bank 1
* @buf0:    [out] source page buffer bank 0
* @buf1:    [out] source page buffer bank 1
*******************************************************************************/
uint32_t nandbch_page_aux_write_ileaved_raw(
	nandbch_info_t * ni,
	uint32_t page0,
	uint32_t page1,
	uint8_t * buf0,
	uint8_t * buf1)
{
	uint32_t rc;
	uint8_t bank;
	uint8_t *buf;
	uint8_t *status;
	uint8_t status0;
	uint8_t status1;

	if (is_page_protected(ni, page0) || is_page_protected(ni, page1))
		return NANDBCH_RC_FAIL_STATUS;

	/* disable ECC */
	chal_nand_bch_disable();
	for (bank = 0; bank < 2; bank++) {
		dma_addr_t addr = ni->buf.phys_addr;
		uint32_t size = NANDBCH_PAGE_SIZE(ni) + NANDBCH_AUX_SIZE(ni);
		enum dma_data_direction dir = DMA_TO_DEVICE;
		buf = (bank ? buf1 : buf0);
		memcpy(ni->buf.buf, buf, size);
		chal_nand_dmaint_set(bank, NANDBCH_PAGE_SIZE(ni),
				     NANDBCH_AUX_SIZE(ni));
		cache_desc_buf_for_device(ni, addr, bank, size, dir);

		/* Execute ucode */
		chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_PRE], bank ? page1 : page0,	/* addr */
				     NANDBCH_PAGE_SIZE(ni) + NANDBCH_AUX_SIZE(ni),	/* attr0 */
				     NANDBCH_ADDR_CYCLES(ni),	/* attr1 */
				     (void *)ni->desc.phys_addr,	/* prdbase */
				     NAND_DIRECTION_TO_NAND);

		/* Wait for completion interrupts */
		if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
			cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
			return ucode_completion_err(bank, __func__,
						    CHAL_NAND_UC_WRITE_PRE);
		}
		cache_desc_buf_for_cpu(ni, addr, bank, size, dir);

		/* Execute ucode */
		chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_COMPLETE], 0,	/* addr */
				     0,	/* attr0 */
				     0,	/* attr1 */
				     NULL,	/* prdbase */
				     NAND_DIRECTION_TO_NAND);

		/* Wait for completion interrupts */
		if (wait_ucode_bank(bank, NANDBCH_TIMEOUT_WRITE)) {
			return ucode_completion_err(bank, __func__,
						    CHAL_NAND_UC_WRITE_COMPLETE);
		}

	}			/* for each bank */

	for (bank = 0; bank < 2; bank++) {
		/* Wait for bank ready interrupt */
		if (wait_ucode_irq_rb(bank, NANDBCH_TIMEOUT_WRITE)) {
			return ucode_completion_err(bank, __func__,
						    CHAL_NAND_UC_WRITE_COMPLETE);
		}

		status = bank ? &status1 : &status0;
		do {
			rc = nandbch_status_get(ni, bank, status);
			if (rc != NANDBCH_RC_SUCCESS)
				return rc;
		} while (!(*status & NAND_STATUS_READY));

	}			/* for each bank */

	return (((status0 & NAND_STATUS_FAIL)
		 || (status1 & NAND_STATUS_FAIL)) ?
		NANDBCH_RC_FAIL_STATUS : NANDBCH_RC_SUCCESS);
}

/*******************************************************************************
* nandbch_page_aux_write_ileaved_ecc(ni, page0, page1, buf0, buf1) - Writes one
* page and aux data to each bank with ECC (CS interleaved mode)
* @ni:      [in] nand info structure
* @page0:   [in] page number bank 0
* @page1:   [in] page number bank 1
* @buf0:    [out] source page buffer bank 0
* @buf1:    [out] source page buffer bank 1
*******************************************************************************/
uint32_t nandbch_page_aux_write_ileaved_ecc(
	nandbch_info_t * ni,
	uint32_t page0,
	uint32_t page1,
	uint8_t * buf0,
	uint8_t * buf1)
{
	uint32_t cmd;
	uint32_t rc;
	uint32_t sector;
	uint32_t sect_offs;
	uint32_t ecc_offs;
	uint32_t aux_offs;
	dma_addr_t sect_buf;
	dma_addr_t aux_buf;
	uint8_t bank;
	uint8_t *buf;
	uint8_t *status;
	uint8_t status0;
	uint8_t status1;

	if (is_page_protected(ni, page0) || is_page_protected(ni, page1))
		return NANDBCH_RC_FAIL_STATUS;

	/* disable ECC */
	chal_nand_bch_disable();

	/* Configure ECC for main data */
	bch_config_data(ni);

	for (bank = 0; bank < 2; bank++) {
		dma_addr_t addr0 = ni->buf.phys_addr;
		uint32_t size0 = NANDBCH_PAGE_SIZE(ni) + NANDBCH_AUX_SIZE(ni);
		enum dma_data_direction dir = DMA_TO_DEVICE;
		buf = (bank ? buf1 : buf0);
		memcpy(ni->buf.buf, buf, size0);

		/* Perform cache management for page buffer */
		cache_buf(for_device, ni, addr0, size0, dir);
		for (sector = 0; sector < NANDBCH_SECTORS(ni); sector++) {
			dma_addr_t addr;
			uint32_t size = NANDBCH_SECTOR_SIZE(ni);
			sect_buf =
				ni->buf.phys_addr +
				sector * NANDBCH_SECTOR_SIZE(ni);
			sect_offs = sector * NANDBCH_SECTOR_SIZE(ni);
			ecc_offs =
				NANDBCH_PAGE_SIZE(ni) + NANDBCH_AUX_SIZE(ni) +
				NANDBCH_AUX_ECC_SIZE(ni) +
				sector * NANDBCH_ECC_SIZE(ni);
			addr = sect_buf;
			chal_nand_dmaint_set(bank, size, 0);
			cache_desc_for_device(ni, addr, bank, size);

			/* Enable ECC for write */
			chal_nand_bch_write_enable();

			cmd =
				(sector ==
				 0) ? CHAL_NAND_UC_WRITE_PRE :
				CHAL_NAND_UC_WRITE_RANDOM;

			/* Execute ucode */
			chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[cmd], bank ? page1 : page0,	/* addr */
					     (sect_offs << 16) | NANDBCH_SECTOR_SIZE(ni),	/* attr0 */
					     NANDBCH_ADDR_CYCLES(ni),	/* attr1 */
					     (void *)ni->desc.phys_addr,	/* prdbase */
					     NAND_DIRECTION_TO_NAND);

			/* Wait for completion interrupts */
			if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
				return ucode_completion_err(bank,
							    __func__, cmd);
			}
			cache_desc(for_cpu, ni);

			/* disable ECC */
			chal_nand_bch_disable();

			/* retrieve ECC bytes */
			rc = nand_bch_ecc_copy(ni->ecc_buf.buf,
					       NANDBCH_ECC_BYTES(ni),
					       NANDBCH_ECC_SIZE(ni));
			if (rc != NANDBCH_RC_SUCCESS) {
				return rc;
			}
			addr = ni->ecc_buf.phys_addr;
			size = NANDBCH_ECC_SIZE(ni);
			chal_nand_dmaint_set(bank, 0, size);
			cache_desc_buf_for_device(ni, addr, bank, size, dir);

			/* Execute ucode */
			chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_RANDOM], 0,	/* addr */
					     (ecc_offs << 16) | NANDBCH_ECC_SIZE(ni),	/* attr0 */
					     0,	/* attr1 */
					     (void *)ni->desc.phys_addr,	/* prdbase */
					     NAND_DIRECTION_TO_NAND);

			/* Wait for completion interrupts */
			if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
				return ucode_completion_err(bank, __func__,
							    CHAL_NAND_UC_WRITE_RANDOM);
			}
			cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
		}		/* for each sector of the page */
		cache_buf(for_cpu, ni, addr0, size0, dir);

		/* write auxiliary data to the OOB */
		aux_buf = ni->buf.phys_addr + NANDBCH_PAGE_SIZE(ni);
		aux_offs = NANDBCH_PAGE_SIZE(ni);
		chal_nand_dmaint_set(bank, 0, NANDBCH_AUX_SIZE(ni));
		setup_desc(ni, aux_buf, bank, NANDBCH_ECC_SIZE(ni));
		if (NANDBCH_AUX_ECC_T(ni)) {	/* Write AUX data with ECC */
			dma_addr_t addr;
			uint32_t size;

			/* Perform cache management for descriptor */
			cache_desc(for_device, ni);

			/* Configure ECC for auxiliary data */
			bch_config_aux(ni);

			/* Enable ECC for write */
			chal_nand_bch_write_enable();

			/* Execute ucode */
			chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_RANDOM], 0,	/* addr */
					     (aux_offs << 16) | NANDBCH_AUX_SIZE(ni),	/* attr0 */
					     0,	/* attr1 */
					     (void *)ni->desc.phys_addr,	/* prdbase */
					     NAND_DIRECTION_TO_NAND);

			/* Wait for completion interrupts */
			if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
				cache_desc(for_cpu, ni);
				return ucode_completion_err(bank,
							    __func__,
							    CHAL_NAND_UC_WRITE_RANDOM);
			}
			cache_desc(for_cpu, ni);

			/* disable ECC */
			chal_nand_bch_disable();

			/* retrieve ECC bytes */
			rc = nand_bch_ecc_copy(ni->ecc_buf.buf,
					       NANDBCH_AUX_ECC_BYTES(ni),
					       NANDBCH_AUX_ECC_SIZE(ni));
			if (rc != NANDBCH_RC_SUCCESS) {
				return rc;
			}
			addr = ni->ecc_buf.phys_addr;
			size = NANDBCH_AUX_ECC_SIZE(ni);
			chal_nand_dmaint_set(bank, 0, size);
			cache_desc_buf_for_device(ni, addr, bank, size, dir);

			/* Execute ucode */
			chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE], 0,	/* addr */
					     NANDBCH_AUX_ECC_SIZE(ni),	/* attr0 */
					     0,	/* attr1 */
					     (void *)ni->desc.phys_addr,	/* prdbase */
					     NAND_DIRECTION_TO_NAND);

			/* Wait for completion interrupts */
			if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
				cache_desc_buf_for_cpu(ni, addr, bank, size,
						       dir);
				return ucode_completion_err(bank,
							    __func__,
							    CHAL_NAND_UC_WRITE);
			}
			cache_desc_buf_for_cpu(ni, addr, bank, size, dir);
		} else {	/* Write AUX data without ECC */

			/* Perform cache management for descriptor */
			cache_desc(for_device, ni);

			/* disable ECC */
			chal_nand_bch_disable();

			/* Execute ucode */
			chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_RANDOM], 0,	/* addr */
					     (aux_offs << 16) | NANDBCH_AUX_SIZE(ni),	/* attr0 */
					     0,	/* attr1 */
					     (void *)ni->desc.phys_addr,	/* prdbase */
					     NAND_DIRECTION_TO_NAND);

			/* Wait for completion interrupts */
			if (wait_ucode_bank_dma(bank, NANDBCH_TIMEOUT_DMA)) {
				cache_desc(for_cpu, ni);
				return ucode_completion_err(bank,
							    __func__,
							    CHAL_NAND_UC_WRITE_RANDOM);
			}
			cache_desc(for_cpu, ni);
		}

		/* Execute ucode */
		chal_nand_ucode_exec(bank, NANDBCH_UC_OFFSET(ni)[CHAL_NAND_UC_WRITE_COMPLETE], 0,	/* addr */
				     0,	/* attr0 */
				     0,	/* attr1 */
				     NULL,	/* prdbase */
				     NAND_DIRECTION_TO_NAND);

		/* Wait for completion interrupt */
		if (wait_ucode_bank(bank, NANDBCH_TIMEOUT_WRITE)) {
			return ucode_completion_err(bank, __func__,
						    CHAL_NAND_UC_WRITE_COMPLETE);
		}

	}			/* for each bank */

	for (bank = 0; bank < 2; bank++) {
		/* Wait for ready interrupt */
		if (wait_ucode_irq_rb(bank, NANDBCH_TIMEOUT_WRITE)) {
			return ucode_completion_err(bank, __func__,
						    CHAL_NAND_UC_WRITE_COMPLETE);
		}

		status = bank ? &status1 : &status0;
		do {
			rc = nandbch_status_get(ni, bank, status);
			if (rc != NANDBCH_RC_SUCCESS)
				return rc;
		} while (!(*status & NAND_STATUS_READY));
	}			/* for each bank */

	return (((status0 & NAND_STATUS_FAIL)
		 || (status1 & NAND_STATUS_FAIL)) ?
		NANDBCH_RC_FAIL_STATUS : NANDBCH_RC_SUCCESS);
}
