/************************************************************************************************/
/*                                                                                              */
/*  Copyright 2011  Broadcom Corporation                                                        */
/*                                                                                              */
/*     Unless you and Broadcom execute a separate written software license agreement governing  */
/*     use of this software, this software is licensed to you under the terms of the GNU        */
/*     General Public License version 2 (the GPL), available at                                 */
/*                                                                                              */
/*          http://www.broadcom.com/licenses/GPLv2.php                                          */
/*                                                                                              */
/*     with the following added to such license:                                                */
/*                                                                                              */
/*     As a special exception, the copyright holders of this software give you permission to    */
/*     link this software with independent modules, and to copy and distribute the resulting    */
/*     executable under terms of your choice, provided that you also meet, for each linked      */
/*     independent module, the terms and conditions of the license of that module.              */
/*     An independent module is a module which is not derived from this software.  The special  */
/*     exception does not apply to any modifications of the software.                           */
/*                                                                                              */
/*     Notwithstanding the above, under no circumstances may you combine this software in any   */
/*     way with any other Broadcom software provided under a license other than the GPL,        */
/*     without Broadcom's express prior written consent.                                        */
/*                                                                                              */
/************************************************************************************************/

/*
 *  drivers/mtd/bcmnand/bcmnand.c
 *
 *  Overview:
 *   This is the generic MTD driver for BCMNAND flash devices. 
 *   It borrows some code from nand_base.c, but since the generic
 *   nand code is too low level for this microcode interface,
 *   a new driver was created here.
 *
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/platform_device.h>
#include <asm/sizes.h>
#include <asm/io.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/flashchip.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/bcmnand.h>
#include <linux/mtd/partitions.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <chal/chal_nandbch.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include "nandbch.h"

#define BCMNAND_DEBUG 0

#ifndef BCMNAND_DEBUG
#define BCMNAND_DEBUG 0
#endif

#undef dprint
#if BCMNAND_DEBUG == 0
#define dprint(fmt, args...)
#else
#define dprint(fmt, args...) printk(fmt, ##args)
#endif

#define NAND_ENCRYPTION 1

#ifdef NAND_ENCRYPTION

#include <linux/crypto.h>
#include <linux/scatterlist.h>

#define CRYPT_BLOCK_SIZE  16

DEFINE_MUTEX(nand_crypto_mutex);
static struct crypto_blkcipher* nand_cipher = NULL;
static struct crypto_blkcipher* iv_cipher = NULL;

#endif /* NAND_ENCRYPTION */

static int nand_get_device(struct bcmnand_chip *chip, struct mtd_info *mtd,
			   int new_state);

static int nand_do_write_oob(struct mtd_info *mtd, loff_t to,
			     struct mtd_oob_ops *ops);

static int nand_block_isbad(struct mtd_info *mtd, loff_t ofs);

static struct clk *clk_peri;
static struct clk *clk_ahb;

const char *part_probes[] = { "cmdlinepart", NULL };

static void nandPrintError(uint32_t rc)
{
   char *msg;

   switch (rc) {
      case NANDBCH_RC_SUCCESS:         msg = "Success"; break;
      case NANDBCH_RC_FAILURE:         msg = "Generic failure"; break;
      case NANDBCH_RC_NOMEM:           msg = "UC memory not available"; break;
      case NANDBCH_RC_TOUT:            msg = "UC completion timeout"; break;
      case NANDBCH_RC_BANK_ERROR:      msg = "UC execution error"; break;
      case NANDBCH_RC_DMA_ERROR:       msg = "DMA error"; break;
      case NANDBCH_RC_ECC_CFG_ERR:     msg = "Unsupported BCH ECC configuration"; break;
      case NANDBCH_RC_ECC_ERROR:       msg = "Uncorrectable BCH ECC error"; break;
      case NANDBCH_RC_ECC_TOUT:        msg = "BCH ECC irq timeout"; break;
      case NANDBCH_RC_BANK_CFG_ERR:    msg = "Unsupported bank configuration"; break;
      case NANDBCH_RC_BB_NOERASE:      msg = "Erase bad block not alowed"; break;
      case NANDBCH_RC_NON_ONFI:        msg = "NAND not ONFI compliant"; break;
      case NANDBCH_RC_PARAM_ERR:       msg = "ONFI parameter page error"; break;
      case NANDBCH_RC_EXTPARAM_ERR:    msg = "ONFI extended parameter page error"; break;
      case NANDBCH_RC_PINMUX_ERR:      msg = "PINMUX error"; break;
      case NANDBCH_RC_NOCONFIG:        msg = "Configuration data not found"; break;
      case NANDBCH_RC_IF_ERR:          msg = "Interface configuration error"; break;
      case NANDBCH_RC_FAIL_STATUS:     msg = "NAND status fail"; break;
      case NANDBCH_RC_AUX_CFG_ERR:     msg = "Invalid AUX data configuration"; break;
      case NANDBCH_RC_OOB_SIZE_ERR:    msg = "Insufficient OOB bytes for current configuration"; break;

      default:                      msg = "Unknown return code";
   };

   printk(KERN_ERR "rc=%d %s\n", rc, msg);
}

static struct nand_ecclayout nand_hw_eccoob;

static int check_offs_len(struct mtd_info *mtd, loff_t ofs, uint64_t len)
{
	struct bcmnand_chip *chip = mtd->priv;
	int ret = 0;

	/* Start address must align on block boundary */
	if (ofs & ((1 << chip->phys_erase_shift) - 1)) {
		DEBUG(MTD_DEBUG_LEVEL0, "%s: Unaligned address\n", __func__);
		ret = -EINVAL;
	}

	/* Length must align on block boundary */
	if (len & ((1 << chip->phys_erase_shift) - 1)) {
		DEBUG(MTD_DEBUG_LEVEL0, "%s: Length not block aligned\n",
					__func__);
		ret = -EINVAL;
	}

	/* Do not allow past end of device */
	if (ofs + len > mtd->size) {
		DEBUG(MTD_DEBUG_LEVEL0, "%s: Past end of device\n",
					__func__);
		ret = -EINVAL;
	}

	return ret;
}

/**
 * nand_release_device - [GENERIC] release chip
 * @mtd:	MTD device structure
 *
 * Deselect, release chip lock and wake up anyone waiting on the device
 */
static void nand_release_device(struct mtd_info *mtd)
{
	struct bcmnand_chip *chip = mtd->priv;

	/* Release the controller and the chip */
	spin_lock(&chip->controller->lock);
	chip->controller->active = NULL;
	chip->state = FL_READY;
	wake_up(&chip->controller->wq);
	spin_unlock(&chip->controller->lock);
}

/**
 * nand_block_bad - [DEFAULT] Read bad block marker from the chip
 * @mtd:	MTD device structure
 * @ofs:	offset from device start
 * @getchip:	0, if the chip is already selected
 *
 * Check, if the block is bad.
 */
static int nand_block_bad(struct mtd_info *mtd, loff_t ofs, int getchip)
{
	int chipnr, ret;
	struct bcmnand_chip *chip = mtd->priv;
	int block;
	uint8_t is_bad = 0;

	dprint("%s ofs=0x%llx getchip=%d\n", __func__, ofs, getchip);

	/* Check for invalid offset */
	if (ofs > mtd->size)
		return -EINVAL;

	chipnr = (int)(ofs >> chip->chip_shift);
	block = (int)(ofs >> chip->phys_erase_shift);

	if (getchip) {
		nand_get_device(chip, mtd, FL_READING);
	}

	ret = nandbch_block_isbad(chip->priv, chipnr, block, &is_bad);

	if (getchip)
		nand_release_device(mtd);

	if (ret != NANDBCH_RC_SUCCESS) {
		printk(KERN_INFO "%s ofs=0x%llx is_bad=%d\n", __func__, ofs,
		       is_bad);
		nandPrintError(ret);
		/* If low level function fails, block may be good or bad - assume good */
	}
	return is_bad == 1;
}

/**
 * nand_check_wp - [GENERIC] check if the chip is write protected
 * @mtd:	MTD device structure
 * Check, if the device is write protected
 *
 * The function expects, that the device is already selected
 */
static int nand_check_wp(struct mtd_info *mtd)
{
#if 0				// Enable for Capri - not supported on BI
	struct bcmnand_chip *chip = mtd->priv;

	/* broken xD cards report WP despite being writable */
	if (chip->options & NAND_BROKEN_XD)
		return 0;

	/* Check the WP bit */
	chip->cmdfunc(mtd, NAND_CMD_STATUS, -1, -1);
	return (chip->read_byte(mtd) & NAND_STATUS_WP) ? 0 : 1;
#endif
	return 0;
}

/**
 * nand_block_checkbad - [GENERIC] Check if a block is marked bad
 * @mtd:        MTD device structure
 * @ofs:        offset from device start
 * @getchip:    0, if the chip is already selected
 * @allowbbt:   1, if its allowed to access the bbt area
 *
 * Check, if the block is bad. Either by reading the bad block table or
 * calling of the scan function.
 */
static int nand_block_checkbad(struct mtd_info *mtd, loff_t ofs, int getchip,
			       int allowbbt)
{
	(void)allowbbt;
	dprint("%s ofs=0x%llx getchip=%d allowbbt=%d\n", __func__, ofs, getchip, allowbbt);
	return nand_block_bad(mtd, ofs, getchip);
}

/**
 * panic_nand_get_device - [GENERIC] Get chip for selected access
 * @chip:	the nand chip descriptor
 * @mtd:	MTD device structure
 * @new_state:	the state which is requested
 *
 * Used when in panic, no locks are taken.
 */
static void panic_nand_get_device(struct bcmnand_chip *chip,
				  struct mtd_info *mtd, int new_state)
{
	/* Hardware controller shared among independend devices */
	chip->controller->active = chip;
	chip->state = new_state;
}

/**
 * nand_get_device - [GENERIC] Get chip for selected access
 * @chip:	the nand chip descriptor
 * @mtd:	MTD device structure
 * @new_state:	the state which is requested
 *
 * Get the device and lock it for exclusive access
 */
static int
nand_get_device(struct bcmnand_chip *chip, struct mtd_info *mtd, int new_state)
{
	spinlock_t *lock = &chip->controller->lock;
	wait_queue_head_t *wq = &chip->controller->wq;
	DECLARE_WAITQUEUE(wait, current);
retry:
	spin_lock(lock);

	/* Hardware controller shared among independent devices */
	if (!chip->controller->active)
		chip->controller->active = chip;

	if (chip->controller->active == chip && chip->state == FL_READY) {
		chip->state = new_state;
		spin_unlock(lock);
		return 0;
	}
	if (new_state == FL_PM_SUSPENDED) {
		if (chip->controller->active->state == FL_PM_SUSPENDED) {
			chip->state = FL_PM_SUSPENDED;
			spin_unlock(lock);
			return 0;
		}
	}
	set_current_state(TASK_UNINTERRUPTIBLE);
	add_wait_queue(wq, &wait);
	spin_unlock(lock);
	schedule();
	remove_wait_queue(wq, &wait);
	goto retry;
}


#ifdef NAND_ENCRYPTION

extern int decrypt_16_with_private_key(unsigned char* src, unsigned char* dst);

/*
 * Encrypt data with the internal box key.
 */
static int encrypt_with_box_key(const uint8_t* from, uint8_t* to, size_t len)
{
    BUG_ON(len != 16);
    return decrypt_16_with_private_key((unsigned char*) from, (unsigned char*) to);
}

/*
 * Get the AES key to use for NAND encryption/decryption.
 */
static uint8_t* get_nand_key(void)
{
	static uint8_t nand_key[CRYPT_BLOCK_SIZE];
	static int have_nand_key = 0;
	if (!have_nand_key) {
		/* NAND key is the box-encryption of this seed value. */
		static const uint8_t seed[CRYPT_BLOCK_SIZE] =
			{ 0x9f,0xb9,0x37,0xa2,0x2b,0x91,0xd7,0x6d,0x97,0xa5,0x09,0xcd,0x38,0xe4,0x20,0x4b };
		if (encrypt_with_box_key(seed, nand_key, CRYPT_BLOCK_SIZE) < 0)
			return NULL;
		//printk("mtd: nand_key: %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n", nand_key[0], nand_key[1], nand_key[2], nand_key[3], nand_key[4], nand_key[5], nand_key[6], nand_key[7], nand_key[8], nand_key[9], nand_key[10], nand_key[11], nand_key[12], nand_key[13], nand_key[14], nand_key[15]);
		have_nand_key = 1;
	}
	return nand_key;
}

/*
 * Get the MD5 hash of the NAND key.
 */
static uint8_t* get_nand_key_hash(void)
{
	static uint8_t nand_key_hash[CRYPT_BLOCK_SIZE];
	static int have_nand_key_hash = 0;
	if (!have_nand_key_hash) {
		uint8_t* nand_key = get_nand_key();
		struct hash_desc desc;
		struct scatterlist nand_key_sg;
		struct crypto_hash* hash;

		if (nand_key == NULL)
			return NULL;
		sg_init_one(&nand_key_sg, nand_key, CRYPT_BLOCK_SIZE);
		hash = crypto_alloc_hash("md5", 0, CRYPTO_ALG_ASYNC);
		if (IS_ERR(hash))
			return NULL;
		BUG_ON(crypto_hash_digestsize(hash) != CRYPT_BLOCK_SIZE);
		desc.tfm = hash;
		desc.flags = 0;
		crypto_hash_init(&desc);
		crypto_hash_update(&desc, &nand_key_sg, CRYPT_BLOCK_SIZE);
		crypto_hash_final(&desc, nand_key_hash);
		crypto_free_hash(hash);
		//printk("mtd: nand_key_hash_hash: %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n", nand_key_hash[0], nand_key_hash[1], nand_key_hash[2], nand_key_hash[3], nand_key_hash[4], nand_key_hash[5], nand_key_hash[6], nand_key_hash[7], nand_key_hash[8], nand_key_hash[9], nand_key_hash[10], nand_key_hash[11], nand_key_hash[12], nand_key_hash[13], nand_key_hash[14], nand_key_hash[15]);
		have_nand_key_hash = 1;
	}
	return nand_key_hash;
}

/*
 * Allocate and initialize a cipher.
 */
int get_cipher(struct crypto_blkcipher** p_cipher, const uint8_t* key)
{
	int result;

	if (*p_cipher != NULL)
		/* Already allocated. */
		return 0;
	if (key == NULL)
		return -1;
	*p_cipher = crypto_alloc_blkcipher("cbc(aes)", 0, CRYPTO_ALG_ASYNC);
	if (IS_ERR(*p_cipher)) {
		result = PTR_ERR(*p_cipher);
		*p_cipher = NULL;
		return result;
	}
	return crypto_blkcipher_setkey(*p_cipher, key, CRYPT_BLOCK_SIZE);
}

/*
 * Get the IV for a page.
 * Use ESSIV -- the IV is the encryption of the page number,
 * using the hash of the NAND key as the encryption key.
 */
static int get_iv(int page, uint8_t* iv)
{
	struct blkcipher_desc desc;
	struct scatterlist iv_sg;
	int result;

	/* First set iv = page number. */
	iv[0] = page & 0xFF;
	iv[1] = (page >> 8) & 0xFF;
	iv[2] = (page >> 16) & 0xFF;
	iv[3] = (page >> 24) & 0xFF;
	memset(&iv[4], 0, CRYPT_BLOCK_SIZE - 4);

	/* Encrypt iv using the iv_cipher. */
	/* assert(mutex_is_locked(&nand_crypto_mutex); */
	result = get_cipher(&iv_cipher, get_nand_key_hash());
	if (result < 0)
		return result;
	memset(crypto_blkcipher_crt(iv_cipher)->iv, 0, CRYPT_BLOCK_SIZE);
	desc.tfm = iv_cipher;
	desc.flags = 0;
	sg_init_one(&iv_sg, iv, CRYPT_BLOCK_SIZE);
	result = crypto_blkcipher_encrypt(&desc, &iv_sg, &iv_sg, CRYPT_BLOCK_SIZE);
	if (result < 0) {
		printk("NAND IV encrypt error %d\n", result);
		return result;
	}
	//printk("mtd: iv for page %d: %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n", page, iv[0], iv[1], iv[2], iv[3], iv[4], iv[5], iv[6], iv[7], iv[8], iv[9], iv[10], iv[11], iv[12], iv[13], iv[14], iv[15]);
	return 0;
}

typedef enum { ENCRYPT, DECRYPT } CryptType;

static int nand_crypt(CryptType crypt_type, const uint8_t* from, uint8_t* to, size_t len, int page)
{
	struct blkcipher_desc desc;
	struct scatterlist from_sg;
	struct scatterlist to_sg;
	int result;

	int const sector_size = 512;
	int const num_sectors = len / sector_size;
	int sector = page * num_sectors;

	BUG_ON(sector_size * num_sectors != len); /* len must be a multiple of sector_size. */

	mutex_lock(&nand_crypto_mutex);
	while (len > 0) {
		BUG_ON(len < sector_size);
		result = get_cipher(&nand_cipher, get_nand_key());
		if (result < 0) {
			mutex_unlock(&nand_crypto_mutex);
			return result;
		}
		result = get_iv(sector, crypto_blkcipher_crt(nand_cipher)->iv);
		if (result < 0) {
			mutex_unlock(&nand_crypto_mutex);
			return result;
		}
		desc.tfm = nand_cipher;
		desc.flags = 0;
		sg_init_one(&from_sg, from, sector_size);
		sg_init_one(&to_sg, to, sector_size);
		result = (crypt_type == ENCRYPT) ?
				crypto_blkcipher_encrypt(&desc, &to_sg, &from_sg, sector_size) : 
				crypto_blkcipher_decrypt(&desc, &to_sg, &from_sg, sector_size);
		if (result < 0) {
			printk("NAND %s error %d\n", (crypt_type == ENCRYPT) ? "encrypt" : "decrypt", result);
			return result;
		}
		from += sector_size;
		to += sector_size;
		len -= sector_size;
		sector += 1;
	}
	mutex_unlock(&nand_crypto_mutex);
	return 0;
}

/*
 * Determine if a block of data is completely erased.
 */
static int all_erased(const uint8_t* buf, size_t len)
{
	int i;
	for (i = 0;  i < len;  i++) {
		if (buf[i] != 0xFF)
			return 0;
	}
	return 1;
}

/*
 * Decrypt a page of data, in place.
 */
static inline void nand_decrypt_page(struct mtd_info* mtd, uint8_t* buf, int page)
{
	if (all_erased(buf, mtd->writesize))
		/* Don't decrypt completely erased data; let the caller see the FFs. */
		return;
	nand_crypt(DECRYPT, buf, buf, mtd->writesize, page);
}

/*
 * Encrypt a page of data and return the encrypted data.
 */
static inline uint8_t* nand_encrypt_page(struct mtd_info* mtd, const uint8_t* buf, int page)
{
	uint8_t* new_buf = kmalloc(mtd->writesize, GFP_KERNEL);
	nand_crypt(ENCRYPT, buf, new_buf, mtd->writesize, page);
	return new_buf;
}

#endif

/**
 * panic_nand_wait - [GENERIC]  wait until the command is done
 * @mtd:	MTD device structure
 * @chip:	NAND chip structure
 * @timeo:	Timeout
 *
 * Wait for command done. This is a helper function for nand_wait used when
 * we are in interrupt context. May happen when in panic and trying to write
 * an oops through mtdoops.
 */
static void panic_nand_wait(struct mtd_info *mtd, struct bcmnand_chip *chip,
			    unsigned long timeo)
{
#if 0				// maybe can do this in Capri
	int i;
	for (i = 0; i < timeo; i++) {
		if (chip->dev_ready) {
			if (chip->dev_ready(mtd))
				break;
		} else {
			if (chip->read_byte(mtd) & NAND_STATUS_READY)
				break;
		}
		mdelay(1);
	}
#endif
}

/**
 * nand_transfer_oob - [Internal] Transfer oob to client buffer
 * @chip:	nand chip structure
 * @oob:	oob destination address
 * @ops:	oob ops structure
 * @len:	size of oob to transfer
 */
static uint8_t *nand_transfer_oob(struct bcmnand_chip *chip, uint8_t *oob,
				  struct mtd_oob_ops *ops, size_t len)
{
	dprint("%s oob=0x%p ops->mode=%d ops->ooboffs=0x%x len=%d\n", __func__, oob, ops->ooboffs, ops->mode, len);
	dprint("%s chip->ecc.layout=%p chip->oob_poi=%p chip->ecc.layout->oobfree=%p\n", __func__, 
		chip->ecc.layout, chip->oob_poi, chip->ecc.layout->oobfree);

	switch (ops->mode) {

	case MTD_OOB_PLACE:
	case MTD_OOB_RAW:
		memcpy(oob, chip->oob_poi + ops->ooboffs, len);
		return oob + len;

	case MTD_OOB_AUTO: {
		struct nand_oobfree *free = chip->ecc.layout->oobfree;
		uint32_t boffs = 0, roffs = ops->ooboffs;
		size_t bytes = 0;

		for (; free->length && len; free++, len -= bytes) {
			/* Read request not from offset 0 ? */
			if (unlikely(roffs)) {
				if (roffs >= free->length) {
					roffs -= free->length;
					continue;
				}
				boffs = free->offset + roffs;
				bytes = min_t(size_t, len,
					      (free->length - roffs));
				roffs = 0;
			} else {
				bytes = min_t(size_t, len, free->length);
				boffs = free->offset;
			}
			memcpy(oob, chip->oob_poi + boffs, bytes);
			oob += bytes;
		}
		return oob;
	}
	default:
		BUG();
	}
	return NULL;
}

/**
 * nand_do_read_ops - [Internal] Read data with ECC
 *
 * @mtd:	MTD device structure
 * @from:	offset to read from
 * @ops:	oob ops structure
 *
 * Internal function. Called with chip held.
 */
static int nand_do_read_ops(struct mtd_info *mtd, loff_t from,
			    struct mtd_oob_ops *ops, int encrypted)
{
	int chipnr, page, realpage, col, bytes, aligned;
	struct bcmnand_chip *chip = mtd->priv;
	struct mtd_ecc_stats stats;
	int ret = 0;
	uint32_t readlen = ops->len;
	uint32_t oobreadlen = ops->ooblen;
	uint32_t max_oobsize = ops->mode == MTD_OOB_AUTO ?
		mtd->oobavail : mtd->oobsize;

	uint8_t *bufpoi, *oob, *buf;

	uint8_t dataerrs;
	uint8_t auxerrs;
	int i;

	dprint("%s from=0x%llx buf=%p oobbuf=%p readlen=0x%x oobreadlen=0x%x mode=%d\n", 
		__func__, from, ops->datbuf, ops->oobbuf, ops->len, ops->ooblen, ops->mode);

	stats = mtd->ecc_stats;

	chipnr = (int)(from >> chip->chip_shift);
	realpage = (int)(from >> chip->page_shift);
	page = realpage & chip->pagemask;

	col = (int)(from & (mtd->writesize - 1));

	buf = ops->datbuf;
	oob = ops->oobbuf;

	while (1) {
		bytes = min(mtd->writesize - col, readlen);
		aligned = (bytes == mtd->writesize);

		/* Is the current page in the buffer ? */
		if (realpage != chip->pagebuf || oob) {
			nandbch_eccstats_t eccstats;
			bufpoi = aligned ? buf : chip->buffers->databuf;

			eccstats.len = 0;
			/* Now read the page into the buffer */
			ret =
			    nandbch_page_read(chip->priv, chipnr, page,
						bufpoi, &eccstats);
			if (ret != NANDBCH_RC_SUCCESS) {
				nandPrintError(ret);
				mtd->ecc_stats.failed++;
			}
			if (encrypted)
				nand_decrypt_page(mtd, buf, page);
			dataerrs = 0;
			for (i = 0; i <eccstats.len; i++)  {
				dataerrs += eccstats.errs[i];
			}
			if (dataerrs)
			{
				mtd->ecc_stats.corrected += dataerrs;
				printk(KERN_DEBUG "%s: ret=%d %u errors on read of page %u\n", __func__, ret, dataerrs, page);
			}

			if (unlikely(oob)) {
				int toread;

				auxerrs = 0;
				ret =
			    	nandbch_oob_read(chip->priv, chipnr, page,
					       	bufpoi + mtd->writesize);
				if (ret != NANDBCH_RC_SUCCESS) {
					nandPrintError(ret);
				}
				if (auxerrs)
				{
					mtd->ecc_stats.corrected += auxerrs;
					printk(KERN_DEBUG "%s: ret=%d %u errors on read of aux data at page %u\n", __func__, ret, auxerrs, page);
				}
	
				toread = min(oobreadlen, max_oobsize);
				if (toread) {
					oob = nand_transfer_oob(chip,
						oob, ops, toread);
					oobreadlen -= toread;
				}
			}
			/* Transfer not aligned data */
			if (!aligned) {
				if (!NAND_SUBPAGE_READ(chip) && !oob &&
			    	!(mtd->ecc_stats.failed - stats.failed))
					chip->pagebuf = realpage;
				memcpy(buf, chip->buffers->databuf + col, bytes);
			}
			buf += bytes;
		} else {
			memcpy(buf, chip->buffers->databuf + col, bytes);
			buf += bytes;
		}

		readlen -= bytes;

		if (!readlen)
			break;

		/* For subsequent reads align to page boundary. */
		col = 0;
		/* Increment page address */
		realpage++;

		page = realpage & chip->pagemask;
		/* Check, if we cross a chip boundary */
		if (!page) {
			chipnr++;
		}
	}

	ops->retlen = ops->len - (size_t) readlen;
	if (oob)
		ops->oobretlen = ops->ooblen - oobreadlen;

	if (ret)
		return ret;

	if (mtd->ecc_stats.failed - stats.failed)
		return -EBADMSG;

	return  mtd->ecc_stats.corrected - stats.corrected ? -EUCLEAN : 0;
}

/**
 * nand_read - [MTD Interface] MTD compatibility function for nand_do_read_ecc
 * @mtd:	MTD device structure
 * @from:	offset to read from
 * @len:	number of bytes to read
 * @retlen:	pointer to variable to store the number of read bytes
 * @buf:	the databuffer to put data
 *
 * Get hold of the chip and call nand_do_read
 */
static int nand_read(struct mtd_info *mtd, loff_t from, size_t len,
		     size_t *retlen, uint8_t *buf, int encrypted)
{
	struct bcmnand_chip *chip = mtd->priv;
	int ret;

	dprint("%s from=0x%llx len=0x%x buf=%p\n", __func__, from, len, buf);

	/* Do not allow reads past end of device */
	if ((from + len) > mtd->size)
		return -EINVAL;
	if (!len)
		return 0;

	nand_get_device(chip, mtd, FL_READING);

	chip->ops.len = len;
	chip->ops.datbuf = buf;
	chip->ops.oobbuf = NULL;

	ret = nand_do_read_ops(mtd, from, &chip->ops, encrypted);

	*retlen = chip->ops.retlen;

	nand_release_device(mtd);

	return ret;
}

/**
 * nand_do_read_oob - [Intern] NAND read out-of-band
 * @mtd:	MTD device structure
 * @from:	offset to read from
 * @ops:	oob operations description structure
 *
 * NAND read out-of-band data from the spare area
 */
static int nand_do_read_oob(struct mtd_info *mtd, loff_t from,
			    struct mtd_oob_ops *ops)
{
	int page, realpage, chipnr;
	struct bcmnand_chip *chip = mtd->priv;
	int readlen = ops->ooblen;
	int len;
	uint8_t *buf = ops->oobbuf;

	DEBUG(MTD_DEBUG_LEVEL3, "%s: from = 0x%08Lx, len = %i\n",
			__func__, (unsigned long long)from, readlen);

	if (ops->mode == MTD_OOB_AUTO)
		len = chip->ecc.layout->oobavail;
	else
		len = mtd->oobsize;

	dprint("%s from=0x%llx readlen=0x%x len=0x%x ops->mode=%d buf=0x%p\n", __func__, from, readlen, len, ops->mode, buf);

	if (unlikely(ops->ooboffs >= len)) {
		DEBUG(MTD_DEBUG_LEVEL0, "%s: Attempt to start read "
					"outside oob\n", __func__);
		return -EINVAL;
	}

	/* Do not allow reads past end of device */
	if (unlikely(from >= mtd->size ||
		     ops->ooboffs + readlen > ((mtd->size >> chip->page_shift) -
					(from >> chip->page_shift)) * len)) {
		DEBUG(MTD_DEBUG_LEVEL0, "%s: Attempt read beyond end "
					"of device\n", __func__);
		return -EINVAL;
	}

	chipnr = (int)(from >> chip->chip_shift);

	/* Shift to get page */
	realpage = (int)(from >> chip->page_shift);
	page = realpage & chip->pagemask;

	while (1) {
		int ret;
		uint8_t auxerrs;

		auxerrs = 0;
		ret = nandbch_oob_read(chip->priv, chipnr, page, chip->oob_poi);
		if (ret != NANDBCH_RC_SUCCESS) {
			nandPrintError(ret);
		}
		if (auxerrs)
		{
			printk(KERN_DEBUG "%s: ret=%d %u errors on read of aux data at page %u\n", __func__, ret, auxerrs, page);
		}

		len = min(len, readlen);
		buf = nand_transfer_oob(chip, buf, ops, len);

		readlen -= len;
		if (readlen <= 0) {
			readlen = 0;
			break;
		}

		/* Increment page address */
		realpage++;

		page = realpage & chip->pagemask;
		/* Check, if we cross a chip boundary */
		if (!page) {
			chipnr++;
		}
	}

	ops->oobretlen = ops->ooblen;
	return 0;
}

/**
 * nand_read_oob - [MTD Interface] NAND read data and/or out-of-band
 * @mtd:	MTD device structure
 * @from:	offset to read from
 * @ops:	oob operation description structure
 *
 * NAND read data and/or out-of-band data
 */
static int nand_read_oob(struct mtd_info *mtd, loff_t from,
			 struct mtd_oob_ops *ops, int encrypted)
{
	struct bcmnand_chip *chip = mtd->priv;
	int ret = -ENOTSUPP;

	dprint("%s from=0x%llx len=0x%x ops->mode=%d\n", __func__, from, ops->len, ops->mode);

	ops->retlen = 0;

	/* Do not allow reads past end of device */
	if (ops->datbuf && (from + ops->len) > mtd->size) {
		DEBUG(MTD_DEBUG_LEVEL0, "%s: Attempt read "
				"beyond end of device\n", __func__);
		return -EINVAL;
	}

	nand_get_device(chip, mtd, FL_READING);

	switch (ops->mode) {
	case MTD_OOB_PLACE:
	case MTD_OOB_AUTO:
	case MTD_OOB_RAW:
		break;

	default:
		goto out;
	}

	if (!ops->datbuf)
		ret = nand_do_read_oob(mtd, from, ops);
	else
		ret = nand_do_read_ops(mtd, from, ops, encrypted);

out:
	nand_release_device(mtd);
	return ret;
}

/**
 * nand_fill_oob - [Internal] Transfer client buffer to oob
 * @chip:	nand chip structure
 * @oob:	oob data buffer
 * @len:	oob data write length
 * @ops:	oob ops structure
 */
static uint8_t *nand_fill_oob(struct bcmnand_chip *chip, uint8_t *oob, size_t len,
						struct mtd_oob_ops *ops)
{
	dprint("%s len=0x%x ops->mode=%d\n", __func__, len, ops->mode);

	switch (ops->mode) {

	case MTD_OOB_PLACE:
	case MTD_OOB_RAW:
		memcpy(chip->oob_poi + ops->ooboffs, oob, len);
		return oob + len;

	case MTD_OOB_AUTO: {
		struct nand_oobfree *free = chip->ecc.layout->oobfree;
		uint32_t boffs = 0, woffs = ops->ooboffs;
		size_t bytes = 0;

		for (; free->length && len; free++, len -= bytes) {
			/* Write request not from offset 0 ? */
			if (unlikely(woffs)) {
				if (woffs >= free->length) {
					woffs -= free->length;
					continue;
				}
				boffs = free->offset + woffs;
				bytes = min_t(size_t, len,
					      (free->length - woffs));
				woffs = 0;
			} else {
				bytes = min_t(size_t, len, free->length);
				boffs = free->offset;
			}
			memcpy(chip->oob_poi + boffs, oob, bytes);
			oob += bytes;
		}
		return oob;
	}
	default:
		BUG();
	}
	return NULL;
}
#ifdef CONFIG_MTD_BCMNAND_VERIFY_WRITE
#define LINE_WIDTH  16
static void DumpMem( const char *prefix, unsigned address, const void *inData, unsigned numBytes )
{
    const uint8_t  *data = (const uint8_t *)inData;
    unsigned        byteOffset;
    char            lineStr[( LINE_WIDTH * 4 ) + 20 ];
    char            str[ 10 ];

    if ( numBytes == 0 )
    {
        printk( "%s: No data\n", prefix );
        return;
    }

    for ( byteOffset = 0; byteOffset < numBytes; byteOffset += LINE_WIDTH ) 
    {
        unsigned    i;

        snprintf( lineStr, sizeof( lineStr ), "%s: %04x: ", prefix, address + byteOffset );

        for ( i = 0; i < LINE_WIDTH; i++ ) 
        {
            if (( byteOffset + i ) < numBytes )
            {
                snprintf( str, sizeof( str ), "%2.2x ", data[ byteOffset + i ] );
                strlcat( lineStr, str, sizeof( lineStr ));
            }
            else
            {
                strlcat( lineStr, "   ", sizeof( lineStr ));
            }
        }
        for ( i = 0; i < LINE_WIDTH; i++ ) 
        {
            if (( byteOffset + i ) < numBytes )
            {
                unsigned char ch = data[ byteOffset + i ];
                if (( ch < ' ' ) || ( ch > '~' ))
                {
                    strlcat( lineStr, ".", sizeof( lineStr ));
                }
                else
                {
                    str[0] = ch;
                    str[1] = '\0';
                    strlcat( lineStr, str, sizeof( lineStr ));
                }
            }
            else
            {
                break;
            }
        }
        printk( "%s\n", lineStr );
    }
}
#endif

#define NOTALIGNED(x)	((x & (chip->subpagesize - 1)) != 0)

/**
 * nand_do_write_ops - [Internal] NAND write with ECC
 * @mtd:	MTD device structure
 * @to:		offset to write to
 * @ops:	oob operations description structure
 *
 * NAND write with ECC
 */
static int nand_do_write_ops(struct mtd_info *mtd, loff_t to,
			     struct mtd_oob_ops *ops, int encrypted)
{
	int chipnr, realpage, page, blockmask, column;
	struct bcmnand_chip *chip = mtd->priv;
	uint32_t writelen = ops->len;

	uint32_t oobwritelen = ops->ooblen;
	uint32_t oobmaxlen = ops->mode == MTD_OOB_AUTO ?
				mtd->oobavail : mtd->oobsize;

	uint8_t *oob = ops->oobbuf;
	uint8_t *buf = ops->datbuf;
	uint8_t *obuf;
	int ret, subpage;

#ifdef CONFIG_MTD_BCMNAND_VERIFY_WRITE
	nandbch_eccstats_t eccstats;
#endif

	dprint("%s to=0x%llx oobwritelen=0x%x oobmaxlen=0x%x, oob=0x%p, buf=0x%p\n", __func__, 
		to, oobwritelen, oobmaxlen, oob, buf);

	ops->retlen = 0;
	if (!writelen)
		return 0;

	/* reject writes, which are not page aligned */
	if (NOTALIGNED(to) || NOTALIGNED(ops->len)) {
		printk(KERN_NOTICE "%s: Attempt to write not "
				"page aligned data\n", __func__);
		return -EINVAL;
	}

	column = to & (mtd->writesize - 1);
	subpage = column || (writelen & (mtd->writesize - 1));

	if (subpage && oob)
		return -EINVAL;

	chipnr = (int)(to >> chip->chip_shift);

	/* Check, if it is write protected */
	if (nand_check_wp(mtd))
		return -EIO;

	realpage = (int)(to >> chip->page_shift);
	page = realpage & chip->pagemask;
	blockmask = (1 << (chip->phys_erase_shift - chip->page_shift)) - 1;

	/* Invalidate the page cache, when we write to the cached page */
	if (to <= (chip->pagebuf << chip->page_shift) &&
	    (chip->pagebuf << chip->page_shift) < (to + ops->len))
		chip->pagebuf = -1;

	/* If we're not given explicit OOB data, let it be 0xFF */
	if (likely(!oob))
		memset(chip->oob_poi, 0xff, mtd->oobsize);

	/* Don't allow multipage oob writes with offset */
	if (oob && ops->ooboffs && (ops->ooboffs + ops->ooblen > oobmaxlen))
		return -EINVAL;

	while (1) {
		int bytes = mtd->writesize;
		int cached = writelen > bytes && page != blockmask;
		uint8_t *wbuf = buf;

		/* Partial page write ? */
		if (unlikely(column || writelen < (mtd->writesize - 1))) {
			cached = 0;
			bytes = min_t(int, bytes - column, (int) writelen);
			chip->pagebuf = -1;
			memset(chip->buffers->databuf, 0xff, mtd->writesize);
			memcpy(&chip->buffers->databuf[column], buf, bytes);
			wbuf = chip->buffers->databuf;
		}

		if (unlikely(oob)) {
			size_t len = min(oobwritelen, oobmaxlen);
			memset(chip->oob_poi, 0xff, mtd->oobsize);
			oob = nand_fill_oob(chip, oob, len, ops);
			oobwritelen -= len;
		}


		if (encrypted) {
			obuf = buf;
			buf = nand_encrypt_page(mtd, buf, page);
		}
		ret = nandbch_page_write(chip->priv, chipnr, page,
					   (uint8_t *) buf, (ops->mode == MTD_OOB_RAW));
		if (encrypted) {
			kfree(buf);
			buf = obuf;
		}
		if (ret != NANDBCH_RC_SUCCESS) {
			nandPrintError(ret);
			ret = -EIO;
			break;
		}

		if (unlikely(oob)) {
			ret =
			    nandbch_aux_write(chip->priv, chipnr, page,
						chip->oob_poi);
			if (ret != NANDBCH_RC_SUCCESS) {
				nandPrintError(ret);
				ret = -EIO;
				break;
			}
		}

#ifdef CONFIG_MTD_BCMNAND_VERIFY_WRITE
		ret = nandbch_page_read(chip->priv, chipnr, page,
					  (uint8_t *) chip->verifybuf, &eccstats);
		if (ret != NANDBCH_RC_SUCCESS) {
			nandPrintError(ret);
			ret = -EIO;
			break;
		}
		if (encrypted)
			nand_decrypt_page(mtd, chip->verifybuf, page);
		if (memcmp(buf, chip->verifybuf, mtd->writesize)) {
			printk(KERN_NOTICE "%s: verify failed at address 0x%x\n", __func__, page*mtd->writesize);
			DumpMem( "Orig", page*0x800, buf, mtd->writesize);
			DumpMem( "Rdbk", page*0x800, chip->verifybuf, mtd->writesize);
			ret = -EFAULT;
			break;
		}
		//printk(KERN_NOTICE "%s: verify succeeded at address 0x%x\n", __func__, page*mtd->writesize);
#endif

		writelen -= bytes;
		if (!writelen)
			break;

		column = 0;
		buf += bytes;
		realpage++;

		page = realpage & chip->pagemask;
		/* Check, if we cross a chip boundary */
		if (!page) {
			chipnr++;
		}
	}

	ops->retlen = ops->len - writelen;
	if (unlikely(oob))
		ops->oobretlen = ops->ooblen;
	return ret;
}

/**
 * panic_nand_write - [MTD Interface] NAND write with ECC
 * @mtd:	MTD device structure
 * @to:		offset to write to
 * @len:	number of bytes to write
 * @retlen:	pointer to variable to store the number of written bytes
 * @buf:	the data to write
 *
 * NAND write with ECC. Used when performing writes in interrupt context, this
 * may for example be called by mtdoops when writing an oops while in panic.
 */
static int panic_nand_write(struct mtd_info *mtd, loff_t to, size_t len,
			    size_t *retlen, const uint8_t *buf, int encrypted)
{
	struct bcmnand_chip *chip = mtd->priv;
	int ret;

	/* Do not allow reads past end of device */
	if ((to + len) > mtd->size)
		return -EINVAL;
	if (!len)
		return 0;

	/* Wait for the device to get ready.  */
	panic_nand_wait(mtd, chip, 400);

	/* Grab the device.  */
	panic_nand_get_device(chip, mtd, FL_WRITING);

	chip->ops.len = len;
	chip->ops.datbuf = (uint8_t *)buf;
	chip->ops.oobbuf = NULL;

	ret = nand_do_write_ops(mtd, to, &chip->ops, encrypted);

	*retlen = chip->ops.retlen;
	return ret;
}

/**
 * nand_write - [MTD Interface] NAND write with ECC
 * @mtd:	MTD device structure
 * @to:		offset to write to
 * @len:	number of bytes to write
 * @retlen:	pointer to variable to store the number of written bytes
 * @buf:	the data to write
 *
 * NAND write with ECC
 * subpages not supported.
 */
static int nand_write(struct mtd_info *mtd, loff_t to, size_t len,
		      size_t * retlen, const uint8_t * buf, int encrypted)
{
	struct bcmnand_chip *chip = mtd->priv;
	int ret;

	dprint("%s to=0x%llx len=0x%x buf=0x%p\n", __func__, 
		to, len, buf);

	/* Do not allow reads past end of device */
	if ((to + len) > mtd->size)
		return -EINVAL;
	if (!len)
		return 0;

	nand_get_device(chip, mtd, FL_WRITING);

	chip->ops.len = len;
	chip->ops.datbuf = (uint8_t *)buf;
	chip->ops.oobbuf = NULL;

	ret = nand_do_write_ops(mtd, to, &chip->ops, encrypted);

	*retlen = chip->ops.retlen;

	nand_release_device(mtd);

	return ret;
}

/**
 * nand_do_write_oob - [MTD Interface] NAND write out-of-band
 * @mtd:	MTD device structure
 * @to:		offset to write to
 * @ops:	oob operation description structure
 *
 * NAND write out-of-band
 */
static int nand_do_write_oob(struct mtd_info *mtd, loff_t to,
			     struct mtd_oob_ops *ops)
{
	int chipnr, page, status, len;
	struct bcmnand_chip *chip = mtd->priv;

	dprint("%s to=0x%llx ooblen=0x%x\n", __func__, 
		to, ops->ooblen);

	DEBUG(MTD_DEBUG_LEVEL3, "%s: to = 0x%08x, len = %i\n",
			 __func__, (unsigned int)to, (int)ops->ooblen);

	if (ops->mode == MTD_OOB_AUTO)
		len = chip->ecc.layout->oobavail;
	else
		len = mtd->oobsize;

	/* Do not allow write past end of page */
	if ((ops->ooboffs + ops->ooblen) > len) {
		DEBUG(MTD_DEBUG_LEVEL0, "%s: Attempt to write "
				"past end of page\n", __func__);
		return -EINVAL;
	}

	if (unlikely(ops->ooboffs >= len)) {
		DEBUG(MTD_DEBUG_LEVEL0, "%s: Attempt to start "
				"write outside oob\n", __func__);
		return -EINVAL;
	}

	/* Do not allow write past end of device */
	if (unlikely(to >= mtd->size ||
		     ops->ooboffs + ops->ooblen >
			((mtd->size >> chip->page_shift) -
			 (to >> chip->page_shift)) * len)) {
		DEBUG(MTD_DEBUG_LEVEL0, "%s: Attempt write beyond "
				"end of device\n", __func__);
		return -EINVAL;
	}

	chipnr = (int)(to >> chip->chip_shift);

	/* Shift to get page */
	page = (int)(to >> chip->page_shift);

#if 0
	/*
	 * Reset the chip. Some chips (like the Toshiba TC5832DC found in one
	 * of my DiskOnChip 2000 test units) will clear the whole data page too
	 * if we don't do this. I have no clue why, but I seem to have 'fixed'
	 * it in the doc2000 driver in August 1999.  dwmw2.
	 */
	chip->cmdfunc(mtd, NAND_CMD_RESET, -1, -1);
#endif

	/* Check, if it is write protected */
	if (nand_check_wp(mtd))
		return -EROFS;

	/* Invalidate the page cache, if we write to the cached page */
	if (page == chip->pagebuf)
		chip->pagebuf = -1;

	memset(chip->oob_poi, 0xff, mtd->oobsize);
	nand_fill_oob(chip, ops->oobbuf, ops->ooblen, ops);

	if (ops->mode == MTD_OOB_AUTO) {
		status = nandbch_aux_write(chip->priv, chipnr, page, chip->oob_poi);
	} else {
		status = nandbch_oob_write(chip->priv, chipnr, page, chip->oob_poi);
	}

	if (status) {
		nandPrintError(status);
		return status;
	}

	ops->oobretlen = ops->ooblen;

	return 0;
}

/**
 * nand_write_oob - [MTD Interface] NAND write data and/or out-of-band
 * @mtd:	MTD device structure
 * @to:		offset to write to
 * @ops:	oob operation description structure
 */
static int nand_write_oob(struct mtd_info *mtd, loff_t to,
			  struct mtd_oob_ops *ops, int encrypted)
{
	struct bcmnand_chip *chip = mtd->priv;
	int ret = -ENOTSUPP;

	dprint("%s to=0x%llx\n", __func__, to);

	ops->retlen = 0;

	/* Do not allow writes past end of device */
	if (ops->datbuf && (to + ops->len) > mtd->size) {
		DEBUG(MTD_DEBUG_LEVEL0, "%s: Attempt write beyond "
				"end of device\n", __func__);
		return -EINVAL;
	}

	nand_get_device(chip, mtd, FL_WRITING);

	switch (ops->mode) {
	case MTD_OOB_PLACE:
	case MTD_OOB_AUTO:
	case MTD_OOB_RAW:
		break;

	default:
		goto out;
	}

	if (!ops->datbuf)
		ret = nand_do_write_oob(mtd, to, ops);
	else
		ret = nand_do_write_ops(mtd, to, ops, encrypted);

out:
	nand_release_device(mtd);
	return ret;
}

/**
 * nand_erase - [MTD Interface] erase block(s)
 * @mtd:	MTD device structure
 * @instr:	erase instruction
 *
 * Erase one ore more blocks
 */
static int nand_erase(struct mtd_info *mtd, struct erase_info *instr)
{
	int page, pages_per_block, ret, chipnr;
	//int status;
	struct bcmnand_chip *chip = mtd->priv;
	loff_t len;

	dprint("%s: start = 0x%012llx, len = %llu\n",
			__func__, (unsigned long long)instr->addr,
			(unsigned long long)instr->len);

	DEBUG(MTD_DEBUG_LEVEL3, "%s: start = 0x%012llx, len = %llu\n",
				__func__, (unsigned long long)instr->addr,
				(unsigned long long)instr->len);

	if (check_offs_len(mtd, instr->addr, instr->len))
		return -EINVAL;

	instr->fail_addr = MTD_FAIL_ADDR_UNKNOWN;

	/* Grab the lock and see if the device is available */
	nand_get_device(chip, mtd, FL_ERASING);

	/* Shift to get first page */
	page = (int)(instr->addr >> chip->page_shift);
	chipnr = (int)(instr->addr >> chip->chip_shift);

	/* Calculate pages in each block */
	pages_per_block = 1 << (chip->phys_erase_shift - chip->page_shift);

	/* Check, if it is write protected */
	if (nand_check_wp(mtd)) {
		DEBUG(MTD_DEBUG_LEVEL0, "%s: Device is write protected!!!\n",
					__func__);
		instr->state = MTD_ERASE_FAILED;
		goto erase_exit;
	}

	/* Loop through the pages */
	len = instr->len;

	instr->state = MTD_ERASING;

	while (len) {
		int block, chipnr;
		loff_t addr = ((loff_t) page) << chip->page_shift;
		/*
		 * heck if we have a bad block, we do not erase bad blocks !
		 */
		if (nand_block_checkbad(mtd, addr, 0, 0)) {
			printk(KERN_WARNING "%s: attempt to erase a bad block "
			       "at page 0x%08x\n", __func__, page);
			instr->state = MTD_ERASE_FAILED;
			goto erase_exit;
		}

		/*
		 * Invalidate the page cache, if we erase the block which
		 * contains the current cached page
		 */
		if (page <= chip->pagebuf && chip->pagebuf <
		    (page + pages_per_block))
			chip->pagebuf = -1;

		block = (int)(addr >> chip->phys_erase_shift);
		chipnr = (int)(addr >> chip->chip_shift);

		ret = nandbch_block_erase(chip->priv, chipnr, block);
		if (ret != NANDBCH_RC_SUCCESS) {
			nandPrintError(ret);
			instr->state = MTD_ERASE_FAILED;
			instr->fail_addr = ((loff_t) page << chip->page_shift);
			goto erase_exit;
		}

		/* Increment page address and decrement length */
		len -= (1 << chip->phys_erase_shift);
		page += pages_per_block;
	}
	instr->state = MTD_ERASE_DONE;

erase_exit:

	ret = instr->state == MTD_ERASE_DONE ? 0 : -EIO;

	/* Deselect and wake up anyone waiting on the device */
	nand_release_device(mtd);

	/* Do call back function */
	if (!ret)
		mtd_erase_callback(instr);

	/* Return more or less happy */
	return ret;
}

/**
 * nand_sync - [MTD Interface] sync
 * @mtd:	MTD device structure
 *
 * Sync is actually a wait for chip ready function
 */
static void nand_sync(struct mtd_info *mtd)
{
	struct bcmnand_chip *chip = mtd->priv;

	DEBUG(MTD_DEBUG_LEVEL3, "%s: called\n", __func__);

	/* Grab the lock and see if the device is available */
	nand_get_device(chip, mtd, FL_SYNCING);
	/* Release it and go back */
	nand_release_device(mtd);
}

/**
 * nand_block_isbad - [MTD Interface] Check if block at offset is bad
 * @mtd:	MTD device structure
 * @offs:	offset relative to mtd start
 */
static int nand_block_isbad(struct mtd_info *mtd, loff_t offs)
{
	/* Check for invalid offset */
	dprint("%s: offs = 0x%llx\n", __func__, offs);

	if (offs > mtd->size)
		return -EINVAL;

	return nand_block_checkbad(mtd, offs, 1, 0);
}

/**
 * nand_block_markbad - [MTD Interface] Mark block at the given offset as bad
 * @mtd:	MTD device structure
 * @ofs:	offset relative to mtd start
 */
static int nand_block_markbad(struct mtd_info *mtd, loff_t ofs)
{
	struct bcmnand_chip *chip = mtd->priv;
	int chipnr = (int)(ofs >> chip->chip_shift);
	int block = (int)(ofs >> chip->phys_erase_shift);
	int ret;

	dprint("%s: offs = 0x%llx\n", __func__, ofs);

	ret = nand_block_isbad(mtd, ofs);
	if (ret) {
		/* If it was bad already, return success and do nothing. */
		if (ret > 0)
			return 0;
		return ret;
	}

	nand_get_device(chip, mtd, FL_WRITING);
	ret = nandbch_block_markbad(chip->priv, chipnr, block);
	nand_release_device(mtd);

	if (ret != NANDBCH_RC_SUCCESS) {
		nandPrintError(ret);
		ret = -EIO;
	}
	return ret;
}

/**
 * nand_suspend - [MTD Interface] Suspend the NAND flash
 * @mtd:	MTD device structure
 */
static int nand_suspend(struct mtd_info *mtd)
{
	struct bcmnand_chip *chip = mtd->priv;

	clk_disable(clk_peri);
	clk_disable(clk_ahb);

	return nand_get_device(chip, mtd, FL_PM_SUSPENDED);
}

/**
 * nand_resume - [MTD Interface] Resume the NAND flash
 * @mtd:	MTD device structure
 */
static void nand_resume(struct mtd_info *mtd)
{
	struct bcmnand_chip *chip = mtd->priv;

	clk_enable(clk_peri);
	clk_enable(clk_ahb);

	if (chip->state == FL_PM_SUSPENDED)
		nand_release_device(mtd);
	else
		printk(KERN_ERR "%s called for a chip which is not "
		       "in suspended state\n", __func__);
}

struct bcmnand_info {
	/* mtd and nand framework related */
	struct mtd_info mtd;
	struct bcmnand_chip chip;
	struct bcmnand_hw_control controller;
};


static int bcmnand_init_chip(struct bcmnand_info *info)
{
	uint32_t rc;
	uint8_t flags;
	uint32_t page;
	nandbch_info_t *pni = info->chip.priv;

	dprint("%s line %d info=%p pni=0x%p\n", __func__, __LINE__, info, pni);

	/* Initialize hardware. */

	/* Note - it is assumed that the pinmux and clocks are already setup by the 
	 * platform initialization code. */

	/* Don't have to clear pni structure because it was allocated and zeroed by caller */

	flags = NANDBCH_FLAG_ECC;

	/* 
 	 * Linux will have a valid geometry page setup by u-boot. Read this 
	 * page and fill in the geometry now. 
	 * Try 5 then 4 address cycles, every 64th page, 8 checks total.
	 * FIXME: We could also read the boot mode straps to determine 4 or 5 cycles 
	 */
	page = 0;
	rc = nandbch_config (pni, 0, 5, &page, 64, 2);
	if (rc != NANDBCH_RC_SUCCESS)
	{
		page = 0;
		rc = nandbch_config (pni, 0, 4, &page, 64, 2);
		if (rc != NANDBCH_RC_SUCCESS)
		{
			nandPrintError(rc);
			printk(KERN_ERR "Failed to find nand geometry page.\n");
			return -EINVAL;
		}
	}
	flags |= NANDBCH_FLAG_GEOMETRY; /* Tell the init not to use ONFI */

	if (NANDBCH_FLAGS(pni) & NANDBCH_FLAG_TIMING) {
		/* Config function found timing settings in parameter table */
		flags |= NANDBCH_FLAG_TIMING;
	}

	if ((rc = nandbch_init(pni, flags)) != NANDBCH_RC_SUCCESS) {
		nandPrintError(rc);
		printk(KERN_ERR "Failed to initialize.\n");
		return -EINVAL;
	}

	printk(KERN_INFO "\nNAND %d bit, ECC %s\n"
	       "ID %02X%02X%02X%02X%02X%02X%02X%02X\n"
	       "banks %d\n"
	       "bank size %d MB\n"
	       "page size %d KB\n"
	       "block size %d KB\n"
	       "aux data size %d bytes\n"
	       "used oob bytes %d",
	       NANDBCH_BUS_WIDTH(pni),
	       (NANDBCH_FLAGS(pni) & NANDBCH_FLAG_ECC) ? "enabled" : "disabled",
	       (NANDBCH_ID(pni))[0],
	       (NANDBCH_ID(pni))[1],
	       (NANDBCH_ID(pni))[2],
	       (NANDBCH_ID(pni))[3],
	       (NANDBCH_ID(pni))[4],
	       (NANDBCH_ID(pni))[5],
	       (NANDBCH_ID(pni))[6],
	       (NANDBCH_ID(pni))[7],
	       NANDBCH_BANKS(pni),
	       0x1 << (NANDBCH_BANK_SHIFT(pni) - 20),
	       NANDBCH_PAGE_SIZE(pni) >> 10,
	       NANDBCH_BLOCK_SIZE(pni) >> 10,
	       NANDBCH_AUX_SIZE(pni),
	       (NANDBCH_FLAGS(pni) & NANDBCH_FLAG_ECC) ? (NANDBCH_AUX_SIZE(pni)
							  +
							  NANDBCH_AUX_ECC_SIZE
							  (pni) +
							  NANDBCH_SECTORS(pni) *
							  NANDBCH_ECC_SIZE(pni))
	       : NANDBCH_AUX_SIZE(pni));

	if (NANDBCH_OOB_SIZE(pni)) {
		printk("/%d", NANDBCH_OOB_SIZE(pni));
	}

	printk("\n");

	if (NANDBCH_FLAGS(pni) & NANDBCH_FLAG_ECC) {
		printk("main data ecc %d/%d\n"
		       "aux data ecc %d/%d\n",
		       NANDBCH_ECC_T(pni), NANDBCH_SECTOR_SIZE(pni),
		       NANDBCH_AUX_ECC_T(pni), NANDBCH_AUX_SIZE(pni));
	}
	if (NANDBCH_FLAGS(pni) & NANDBCH_FLAG_TIMING) {
		printk("timing_select %d\n"
		       "timing mode %d\n",
		       NANDBCH_TIMING_SELECT(pni), 
		       NANDBCH_TIMING_MODE(pni));
	}

	return 0;
}

static nandbch_info_t nandbch;

static int __devinit bcmnand_probe(struct platform_device *pdev)
{
	int ret = -ENOENT;
	struct bcmnand_info *info;
	int rc;

#if 0				// FIXME - add resources later if appropriate
	struct bcmnand_platform_data *pdata = pdev->dev.platform_data;
	struct resource *res = pdev->resource;
	unsigned long size = resource_size(res);
#endif
	struct mtd_info *mtd;
	struct bcmnand_chip *chip;
	nandbch_info_t *pni;
	int rate;

	clk_peri = clk_get(&pdev->dev, "nand_clk");
	if (IS_ERR_OR_NULL(clk_peri)) {
		printk(KERN_ERR "%s clk_get of nand clk_peri failed\n", __func__);
		return -EINVAL;	
	}
	clk_ahb = clk_get(&pdev->dev, "nand_ahb_clk");
	if (IS_ERR_OR_NULL(clk_ahb)) {
		printk(KERN_ERR "%s clk_get of nand clk_ahb failed\n", __func__);
		return -EINVAL;	
	}

	rc = clk_set_rate(clk_peri, 208000000);  /* 208 MHz */
	if (rc) {
		printk(KERN_ERR "Couldn't set nand clk_peri clock rate, rc=%d\n", rc);
		return rc;
	}

	clk_enable(clk_peri);
	clk_enable(clk_ahb);

	rate = clk_get_rate(clk_peri);
	printk(KERN_INFO "NAND clock running at %u MHz\n", rate/1000000);

	info = kzalloc(sizeof(*info), GFP_KERNEL);
	if (info == NULL) {
		dev_err(&pdev->dev, "no memory for flash info\n");
		return -ENOMEM;
	}

	/* The bcmcapri data structure is private to the bcmnand world */
	info->chip.priv = &nandbch;
	nandbch.dev = &pdev->dev;

	spin_lock_init(&info->controller.lock);
	init_waitqueue_head(&info->controller.wq);

#if 0				// FIXME - add resources later if appropriate
	if (!request_mem_region(res->start, size, dev_name(&pdev->dev))) {
		ret = -EBUSY;
		goto out_free_info;
	}
#endif
	mtd = &info->mtd;
	chip = &info->chip;
	pni = info->chip.priv;

#if 0				// FIXME - add resources later if appropriate
	/* Since current capri nandbch has a msleep polling 
	 * architecture, we don't need to use irqs at this time.
	 * When we phase this out and switch to KONA, we may 
	 * then have to use and irq to signal hardware complete
	 * events.
	 */
	info->irq = platform_get_irq(pdev, 0);
#endif
	/* Descriptor mapping */
	pni->desc.phys_addr = dma_map_single(&pdev->dev, pni->desc.desc,
					     sizeof(pni->desc.desc),
					     DMA_BIDIRECTIONAL);
	if (dma_mapping_error(&pdev->dev, pni->desc.phys_addr)) {
		dev_err(&pdev->dev, "BCMNAND: failed to map DMA descriptors\n");
		goto out_free_info;
	}
	dprint("%s line %d  desc.phys_addr = 0x%x virtaddr=0x%p size=0x%x\n", 
		__func__, __LINE__, pni->desc.phys_addr, pni->desc.desc, sizeof(pni->desc.desc));

	/* Transfer buffer mapping */
	pni->buf.phys_addr = dma_map_single(&pdev->dev, pni->buf.buf,
					     BCMCAPRI_BUF_SIZE,
					     DMA_BIDIRECTIONAL);
	if (dma_mapping_error(&pdev->dev, pni->buf.phys_addr)) {
		dev_err(&pdev->dev, "BCMNAND: failed to map DMA transfer buffer\n");
		goto out_free_info;
	}
	dprint("%s line %d  buf.phys_addr = 0x%x virtaddr=0x%p size=0x%x\n", 
		__func__, __LINE__, pni->buf.phys_addr, pni->buf.buf, BCMCAPRI_BUF_SIZE);

	/* Param buffer mapping */
	pni->param_buf.phys_addr = dma_map_single(&pdev->dev, pni->param_buf.buf,
					     512,
					     DMA_BIDIRECTIONAL);
	if (dma_mapping_error(&pdev->dev, pni->param_buf.phys_addr)) {
		dev_err(&pdev->dev, "BCMNAND: failed to map DMA param buffer\n");
		goto out_free_info;
	}
	dprint("%s line %d  param_buf.phys_addr = 0x%x virtaddr=0x%p size=0x%x\n", 
		__func__, __LINE__, pni->param_buf.phys_addr, pni->param_buf.buf, 512);

	/* ECC buffer mapping */
	pni->ecc_buf.phys_addr = dma_map_single(&pdev->dev, pni->ecc_buf.buf,
					     NANDBCH_ECC_MAX_SIZE,
					     DMA_BIDIRECTIONAL);
	if (dma_mapping_error(&pdev->dev, pni->ecc_buf.phys_addr)) {
		dev_err(&pdev->dev, "BCMNAND: failed to map DMA ecc buffer\n");
		goto out_free_info;
	}
	dprint("%s line %d  ecc_buf.phys_addr = 0x%x virtaddr=0x%p size=0x%x\n", 
		__func__, __LINE__, pni->ecc_buf.phys_addr, pni->ecc_buf.buf, NANDBCH_ECC_MAX_SIZE);

	if (bcmnand_init_chip(info)) {
		printk(KERN_ERR "NAND init failed\n");
		ret = -EINVAL;
		goto out_free_info;
	}

	chip->chipsize = 0x1 << (NANDBCH_BANK_SHIFT(pni));
	chip->numchips = NANDBCH_BANKS(pni);
	mtd->size = chip->numchips * chip->chipsize;

	mtd->type = MTD_NANDFLASH;
	mtd->flags = MTD_CAP_NANDFLASH;
	mtd->erasesize = NANDBCH_BLOCK_SIZE(pni);
	mtd->writesize = NANDBCH_PAGE_SIZE(pni);
	mtd->oobsize = NANDBCH_AUX_SIZE(pni) + NANDBCH_AUX_ECC_SIZE(pni) +
	               NANDBCH_SECTORS(pni) * NANDBCH_ECC_SIZE(pni);

	mtd->oobavail = NANDBCH_AUX_SIZE(pni) - 1;

	mtd->name = "Capri NAND";
	mtd->priv = pni;

	mtd->erase = nand_erase;
	mtd->point = NULL;
	mtd->unpoint = NULL;
	mtd->read = nand_read;
	mtd->write = nand_write;
	mtd->panic_write = panic_nand_write;
	mtd->read_oob = nand_read_oob;
	mtd->write_oob = nand_write_oob;
	mtd->sync = nand_sync;
	mtd->lock = NULL;
	mtd->unlock = NULL;
	mtd->suspend = nand_suspend;
	mtd->resume = nand_resume;
	mtd->block_isbad = nand_block_isbad;
	mtd->block_markbad = nand_block_markbad;
	mtd->writebufsize = mtd->writesize;

	chip->ecc.layout = &nand_hw_eccoob;
	chip->ecc.layout->oobavail = mtd->oobavail;
	chip->ecc.layout->oobfree[0].offset = 1; 	/* Skip bad block marker */
	chip->ecc.layout->oobfree[0].length = mtd->oobavail;

	/* propagate ecc.layout to mtd_info */
	mtd->ecclayout = chip->ecc.layout;

	mtd->subpage_sft = 0;	/* subpages not supported */

	chip->buffers = kmalloc(sizeof(*chip->buffers), GFP_KERNEL);
	if (!chip->buffers)
		return -ENOMEM;

	/* Set the internal oob buffer location, just after the page data */
	chip->oob_poi = chip->buffers->databuf + mtd->writesize;

#ifdef CONFIG_MTD_BCMNAND_VERIFY_WRITE
	chip->verifybuf = kmalloc(mtd->writesize, GFP_KERNEL);
	if (!chip->verifybuf)
		return -ENOMEM;
#endif

	chip->subpagesize = mtd->writesize >> mtd->subpage_sft;

	/* Initialize state */
	chip->state = FL_READY;

	/* Invalidate the pagebuffer reference */
	chip->pagebuf = -1;

	/* Calculate the address shift from the page size */
	chip->page_shift = ffs(mtd->writesize) - 1;
	/* Convert chipsize to number of pages per chip -1. */
	chip->pagemask = (chip->chipsize >> chip->page_shift) - 1;

	chip->phys_erase_shift = ffs(mtd->erasesize) - 1;
	if (chip->chipsize & 0xffffffff)
		chip->chip_shift = ffs((unsigned)chip->chipsize) - 1;
	else
		chip->chip_shift =
		    ffs((unsigned)(chip->chipsize >> 32)) + 32 - 1;

	chip->controller = &info->controller;
	info->mtd.name = dev_name(&pdev->dev);
	mtd->priv = &info->chip;
	mtd->owner = THIS_MODULE;

	/* Register the partitions */
	{
		int nr_partitions;
		struct mtd_partition *partition_info;

		mtd->name = "bcmnand";
		nr_partitions =
		    parse_mtd_partitions(mtd, part_probes, &partition_info, 0);

		if (nr_partitions <= 0) {
			printk(KERN_ERR "BCMNAND: Too few partitions - %d\n",
			       nr_partitions);
			ret = -EIO;
			goto out_free_info;
		}
		mtd_device_register(mtd, partition_info, nr_partitions);
	}

	platform_set_drvdata(pdev, info);

	return 0;

 out_free_info:
	kfree(info);

	return ret;
}

static int __devexit bcmnand_remove(struct platform_device *pdev)
{
	struct bcmnand_info *info = platform_get_drvdata(pdev);
	struct bcmnand_chip *chip = &info->chip;
	nandbch_info_t *pni = chip->priv;

#if 0				// FIXME - maybe add resources later if appropriate
	struct resource *res = pdev->resource;
	unsigned long size = resource_size(res);
#endif
	dma_unmap_single(&pdev->dev, pni->desc.phys_addr, sizeof(pni->desc.desc), DMA_BIDIRECTIONAL);
	dma_unmap_single(&pdev->dev, pni->buf.phys_addr, BCMCAPRI_BUF_SIZE, DMA_BIDIRECTIONAL);
	dma_unmap_single(&pdev->dev, pni->param_buf.phys_addr, 512, DMA_BIDIRECTIONAL);
	dma_unmap_single(&pdev->dev, pni->ecc_buf.phys_addr, NANDBCH_ECC_MAX_SIZE, DMA_BIDIRECTIONAL);

	platform_set_drvdata(pdev, NULL);

#if 0				// FIXME - maybe add resources later if appropriate
	release_mem_region(res->start, size);
#endif
	kfree(info->chip.buffers);
	kfree(info);
	clk_disable(clk_peri);
	clk_put(clk_peri);
	clk_disable(clk_ahb);
	clk_put(clk_ahb);
	return 0;
}

static struct platform_driver bcmnand_driver = {
	.probe = bcmnand_probe,
	.remove = __devexit_p(bcmnand_remove),
	/* suspend/resume functions are not here - called from mtd parent */
	.driver = {
		   .name = "bcmnand",
		   .owner = THIS_MODULE,
		   },

};

static int __init bcmnand_init(void)
{
	printk(KERN_INFO "BCMNAND Driver,(C) 2011 Broadcom Corp\n");
	return platform_driver_register(&bcmnand_driver);
}

static void __exit bcmnand_exit(void)
{
	platform_driver_unregister(&bcmnand_driver);
}

module_init(bcmnand_init);
module_exit(bcmnand_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Broadcom");
MODULE_DESCRIPTION("Broadcom NAND flash driver code");
