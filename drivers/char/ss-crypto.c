/*
 * Timer device implementation for SGI UV platform.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (c) 2009 Silicon Graphics, Inc.  All rights reserved.
 *
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/ioctl.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/vmalloc.h>
#include <linux/mutex.h>
#include <linux/mm.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>
#include <linux/time.h>
#include <linux/platform_device.h>
#include <asm/scatterlist.h>
#include <linux/crypto.h>
#include <crypto/if_alg.h>
#include <asm/cacheflush.h>
#include <asm/dma.h>
#include <mach/dma.h>

#include "ss-crypto.h"

#define SS_CRYPTO_NAME "ss-crypto"

#define ERR(s...) printk(KERN_ERR SS_CRYPTO_NAME ": " s)
#define INF(s...) printk(KERN_ERR SS_CRYPTO_NAME ": " s)
//#define DBG(s...) printk(KERN_ERR SS_CRYPTO_NAME ": " s)
#define DBG(s...)

#define USE_DIRECT_DMA

#define SS_CLK_RATE 150000000

#define SS_MODE_SW  0
#define SS_MODE_PIO 1
#define SS_MODE_DMA 2

#define SS_AES_128  0
#define SS_AES_192  1
#define SS_AES_256  2
#define SS_DES      3
#define SS_3DES     4

#define SS_ENCRYPT  0
#define SS_DECRYPT  1

#define IOCTL_SS_INIT     0x21000000

#define BLOCKSIZE 65536
#define NUMBLOCKS 16

#define FLUSHTAIL 1024

extern void sw_dma_set_direct_callback(int ch, void (*fn)(void));

static struct sw_dma_client ss_dma_tx_client = {
        .name = "SS DMA TX"
};

static struct sw_dma_client ss_dma_rx_client = {
        .name = "SS DMA RX"
};

static struct class *ss_crypto_class;

static DECLARE_WAIT_QUEUE_HEAD(write_wq);
static DECLARE_WAIT_QUEUE_HEAD(read_wq);

static DEFINE_MUTEX(dma_mutex);

static struct clk *busclk;
static struct clk *ssclk;
static struct clk *pll5p;
static struct clk *pll62;
static void *ssreg;
static void *dmareg;

static dma_addr_t buf_phys;
static void *buf_virt;

static struct crypto_blkcipher *tfm = NULL;

static int dma_rx_hdle;
static int dma_tx_hdle;

static int readptr, writeptr, handledptr;
static atomic_t usedblk, handledblk;

struct ss_crypt_param {

	int mode;
	int op;
	int algo;
	u32 key[8];
	u32 iv[4];

} cparam;

static char *hexdump(void *data, int len)
{
	static char buf[132];
	unsigned char *p = (unsigned char *) data;
	int i;

	if (len > 64) len = 64;
	for (i=0; i<len; i++) sprintf(buf+i*2, "%02x", *p++);
	return buf;
}

static char *blockdump(void *data, int len)
{
	static char buf[64];

	strcpy(buf, hexdump(data, 8));
	buf[16] = buf[17] = '.';
	strcpy(buf+18, hexdump(data+(len-8), 8));
	return buf;
}

static inline void *get_buffer_va(int ptr)
{
	return (void *)((u32)buf_virt + ptr * BLOCKSIZE);
}

static inline dma_addr_t get_buffer_pa(int ptr)
{
	return (dma_addr_t)((u32)buf_phys + ptr * BLOCKSIZE);
}

static int crypt_init(struct ss_crypt_param *param)
{
	u32 mask;
	char *algo;
	int keysize;

	if (! param) param = &cparam;

	if (param->mode != SS_MODE_SW && param->mode != SS_MODE_PIO && param->mode != SS_MODE_DMA) {
		ERR("unknown mode %d\n", param->mode);
		return -EINVAL;
	}

	if (param->op != SS_ENCRYPT && param->op != SS_DECRYPT) {
		ERR("unknown operation %d\n", param->op);
		return -EINVAL;
	}

	switch (param->algo) {
	  case SS_AES_128:
		mask = (SUNXI_OP_AES | SUNXI_AES_128BITS);
		algo = "cbc(aes)";
		keysize = 128/8;
		break;
	  case SS_AES_192:
		mask = (SUNXI_OP_AES | SUNXI_AES_192BITS);
		algo = "cbc(aes)";
		keysize = 192/8;
		break;
	  case SS_AES_256:
		mask = (SUNXI_OP_AES | SUNXI_AES_256BITS);
		algo = "cbc(aes)";
		keysize = 256/8;
		break;
	  case SS_DES:
		mask = SUNXI_OP_DES;
		algo = "cbc(des)";
		keysize = 64/8;
		break;
	  case SS_3DES:
		mask = SUNXI_OP_3DES;
		algo = "cbc(des3_ede)";
		keysize = 192/8;
		break;
	  default:
		printk(KERN_ERR "%s: unknown algorithm %d\n", SS_CRYPTO_NAME, param->algo);
		return -EINVAL;

	}
	mask |= (param->op == SS_ENCRYPT) ? SUNXI_SS_ENCRYPTION : SUNXI_SS_DECRYPTION;
	mask |= (SUNXI_SS_CBC | SUNXI_KEYSELECT_KEYN);

	SS_CTL_REG = 0;
	if (tfm) crypto_free_blkcipher(tfm);
	tfm = NULL;

	DBG("crypt_init: mode=%d algo=%d op=%d keysize=%d\n", param->mode, param->algo, param->op, keysize);
	DBG("crypt_init: key= %s\n", hexdump(param->key, keysize));
	DBG("crypt_init: iv=  %s\n", hexdump(param->iv, 16));

	if (param->mode == SS_MODE_SW) {
		tfm = crypto_alloc_blkcipher(algo, 0, 0 /*CRYPTO_ALG_ASYNC*/);
		if (! tfm) {
			ERR("crypto_alloc_blkcipher: error\n");
			return -EIO;
		}
		crypto_blkcipher_setkey(tfm, &(param->key), keysize);
		crypto_blkcipher_set_iv(tfm, &(param->iv), crypto_blkcipher_ivsize(tfm));
	} else {
		SS_KEY0_REG = param->key[0];
		SS_KEY1_REG = param->key[1];
		SS_KEY2_REG = param->key[2];
		SS_KEY3_REG = param->key[3];
		SS_KEY4_REG = param->key[4];
		SS_KEY5_REG = param->key[5];
		SS_KEY6_REG = param->key[6];
		SS_KEY7_REG = param->key[7];
		SS_IV0_REG  = param->iv[0];
		SS_IV1_REG  = param->iv[1];
		SS_IV2_REG  = param->iv[2];
		SS_IV3_REG  = param->iv[3];

		SS_CTL_REG = mask;
		SS_ICSR_REG = 0;
		SS_CTL_REG = mask | SUNXI_SS_ENABLED;
	}

	writeptr = handledptr = readptr = 0;
	atomic_set(&usedblk, 0);
	atomic_set(&handledblk, 0);

	if (param != &cparam) memcpy(&cparam, param, sizeof(struct ss_crypt_param));
	return 0;
}

static int crypt_process_sw(void)
{
	void *inptr;
	struct scatterlist sg_in[1];
        struct blkcipher_desc desc;

	while (handledptr != writeptr) {

		inptr = get_buffer_va(handledptr);
		DBG("crypt-sw: ptr=%d addr=%08x\n", handledptr, (int) inptr);
		DBG("IN:  %s\n", blockdump(inptr, BLOCKSIZE));
		sg_init_one(&sg_in[0], inptr, BLOCKSIZE);
		desc.tfm = tfm;
		desc.flags = 0;
		if (cparam.op == SS_ENCRYPT) {
			crypto_blkcipher_encrypt(&desc, sg_in, sg_in, BLOCKSIZE);
		} else {
			crypto_blkcipher_decrypt(&desc, sg_in, sg_in, BLOCKSIZE);
		}
		DBG("OUT: %s\n", blockdump(inptr, BLOCKSIZE));

		handledptr = (handledptr + 1) % NUMBLOCKS;
		atomic_inc(&handledblk);
		DBG("handledblk=%d\n", atomic_read(&handledblk));
		wake_up(&read_wq);

	}
	return 0;
}

static int crypt_process_pio(void)
{
	u32 *inptr, *outptr, *endptr;
	u32 fcsr;
	int nw, nr;

	while (handledptr != writeptr) {

		inptr = outptr = (u32 *) get_buffer_va(handledptr);
		endptr = (u32 *) ((u32)inptr + BLOCKSIZE);
		DBG("crypt-pio: ptr=%d addr=%08x\n", handledptr, (int) inptr);
		DBG("IN:  %s\n", blockdump(get_buffer_va(handledptr), BLOCKSIZE));

		while (outptr < endptr) {
			fcsr = SS_FCSR_REG;
			nw = SUNXI_RXFIFO_SPACES(fcsr);
			nr = SUNXI_TXFIFO_SPACES(fcsr);
			if(inptr < endptr && nw >= 8) {
				SS_RXFIFO_REG = *inptr++;
				SS_RXFIFO_REG = *inptr++;
				SS_RXFIFO_REG = *inptr++;
				SS_RXFIFO_REG = *inptr++;
				SS_RXFIFO_REG = *inptr++;
				SS_RXFIFO_REG = *inptr++;
				SS_RXFIFO_REG = *inptr++;
				SS_RXFIFO_REG = *inptr++;
			}
			if(outptr < inptr && nr >= 8) {
				*outptr++ = SS_TXFIFO_REG;
				*outptr++ = SS_TXFIFO_REG;
				*outptr++ = SS_TXFIFO_REG;
				*outptr++ = SS_TXFIFO_REG;
				*outptr++ = SS_TXFIFO_REG;
				*outptr++ = SS_TXFIFO_REG;
				*outptr++ = SS_TXFIFO_REG;
				*outptr++ = SS_TXFIFO_REG;
			}
		}

		DBG("OUT: %s\n", blockdump(get_buffer_va(handledptr), BLOCKSIZE));
		handledptr = (handledptr + 1) % NUMBLOCKS;
		atomic_inc(&handledblk);
		wake_up(&read_wq);

	}
	return 0;
}

static int crypt_process_dma(void)
{
	u32 *inptr;
	dma_addr_t inphys;

	if (handledptr == writeptr) {
		mutex_unlock(&dma_mutex);
		return 0;
	}
	
	inptr = (u32 *) get_buffer_va(handledptr);
	inphys = get_buffer_pa(handledptr);
	DBG("crypt: ptr=%d addr=%08x\n", handledptr, (int) inptr);
	DBG("IN:  %s\n", blockdump(inptr, BLOCKSIZE));

	__cpuc_flush_dcache_area(inptr, BLOCKSIZE+FLUSHTAIL);

#ifndef USE_DIRECT_DMA

	sw_dma_enqueue(dma_rx_hdle, NULL, inphys, BLOCKSIZE);
	sw_dma_enqueue(dma_tx_hdle, NULL, inphys, BLOCKSIZE);

	sw_dma_ctrl(dma_rx_hdle, SW_DMAOP_START);
	sw_dma_ctrl(dma_tx_hdle, SW_DMAOP_START);

	SS_ICSR_REG |= (1 << 4);

#else

	u32 dma_rx_base = SUNXI_DDMA_REG_BASE + ((dma_rx_hdle & 15) - 8) * 0x20;
	u32 dma_tx_base = SUNXI_DDMA_REG_BASE + ((dma_tx_hdle & 15) - 8) * 0x20;

	volatile u32 *dma_rx_cfg_reg  = (volatile u32 *)(dma_rx_base + 0x00);
	volatile u32 *dma_rx_src_reg  = (volatile u32 *)(dma_rx_base + 0x04);
	volatile u32 *dma_rx_dest_reg = (volatile u32 *)(dma_rx_base + 0x08);
	volatile u32 *dma_rx_bc_reg   = (volatile u32 *)(dma_rx_base + 0x0c);
	volatile u32 *dma_rx_para_reg = (volatile u32 *)(dma_rx_base + 0x18);

	volatile u32 *dma_tx_cfg_reg  = (volatile u32 *)(dma_tx_base + 0x00);
	volatile u32 *dma_tx_src_reg  = (volatile u32 *)(dma_tx_base + 0x04);
	volatile u32 *dma_tx_dest_reg = (volatile u32 *)(dma_tx_base + 0x08);
	volatile u32 *dma_tx_bc_reg   = (volatile u32 *)(dma_tx_base + 0x0c);
	volatile u32 *dma_tx_para_reg = (volatile u32 *)(dma_tx_base + 0x18);

	//                 conti=0    dwidth=32   dburst=8    dmode=IO     drq=SS
	*dma_tx_cfg_reg =  (0 << 29) | (2 << 25) | (1 << 23) | (1 << 21) | (0xa << 16);
	//                 bc=upd     swidth=32   sburst=8    smode=lin    drq=sdram
	*dma_tx_cfg_reg |= (1 << 15) | (2 << 9)  | (0 << 7)  | (0 << 5)  | (0x1 << 0);

	*dma_tx_src_reg = inphys;
	*dma_tx_dest_reg = (u32) &SS_RXFIFO_REG;
	*dma_tx_bc_reg = BLOCKSIZE;
	*dma_tx_para_reg = 0;

	//                 conti=0    dwidth=32   dburst=8    dmode=lin   drq=sdram
	*dma_rx_cfg_reg =  (0 << 29) | (2 << 25) | (0 << 23) | (0 << 21) | (0x1 << 16);
	//                 bc=upd     swidth=32   sburst=8    smode=IO    drq=SS
	*dma_rx_cfg_reg |= (1 << 15) | (2 << 9)  | (1 << 7)  | (1 << 5)  | (0xb << 0);

	*dma_rx_src_reg = (u32) &SS_TXFIFO_REG;
	*dma_rx_dest_reg = inphys;
	*dma_rx_bc_reg = BLOCKSIZE;
	*dma_rx_para_reg = 0;

	DMA_IRQ_EN_REG |= (1 << ((dma_rx_hdle & 15) * 2 + 1));
	DMA_IRQ_EN_REG |= (1 << ((dma_tx_hdle & 15) * 2 + 1));

	*dma_tx_cfg_reg |= (1 << 31);
	*dma_rx_cfg_reg |= (1 << 31);

	SS_ICSR_REG |= (1 << 4);

	DBG("DMA IRQEN: %08x DMA IRQPEND: %08x\n", DMA_IRQ_EN_REG, DMA_IRQ_PEND_REG);
	DBG("CH8: cfg=%08x src=%08x dest=%08x bc=%x\n", DDMA0_CFG_REG, DDMA0_SRC_ADDR_REG, DDMA0_DEST_ADDR_REG, DDMA0_BC_REG);
	DBG("CH9: cfg=%08x src=%08x dest=%08x bc=%x\n", DDMA1_CFG_REG, DDMA1_SRC_ADDR_REG, DDMA1_DEST_ADDR_REG, DDMA1_BC_REG);

#endif

	return 0;
}

static void dma_tx_callback(void)
{
	DBG("tx callback\n");
}

static void dma_rx_callback(void)
{
	if (SS_ICSR_REG == 0) return;

	void *inptr = get_buffer_va(handledptr);
	__cpuc_flush_dcache_area(inptr, BLOCKSIZE+FLUSHTAIL);

	DBG("OUT: %s\n", blockdump(inptr, BLOCKSIZE));

	SS_ICSR_REG = 0;

	DMA_IRQ_EN_REG &= ~(1 << ((dma_rx_hdle & 15) * 2 + 1));
	DMA_IRQ_EN_REG &= ~(1 << ((dma_tx_hdle & 15) * 2 + 1));

	handledptr = (handledptr + 1) % NUMBLOCKS;
	atomic_inc(&handledblk);
	wake_up(&read_wq);

	crypt_process_dma();
}

#ifndef USE_DIRECT_DMA

static void ss_crypto_dma_callback(struct sw_dma_chan * ch, void *buf, int size, enum sw_dma_buffresult result)
{
	dma_rx_callback();
}

#endif

static ssize_t ss_crypto_write(struct file * file, const char __user * buf,
                        size_t count, loff_t *ppos)
{
	void *vaddr;
	int tcount = count;
	if (count & (BLOCKSIZE - 1)) return -EINVAL;

	DBG("write %d @ %d\n", (int) count, (int) *ppos);

	while (tcount > 0) {

		DBG("usedblk=%d\n", atomic_read(&usedblk));

		if (wait_event_interruptible(write_wq, atomic_read(&usedblk) < NUMBLOCKS))
			return -EINTR;

		atomic_inc(&usedblk);
		vaddr = get_buffer_va(writeptr);
		DBG("writeptr=%d vaddr=%08x\n", writeptr, vaddr);
		writeptr = (writeptr + 1) % NUMBLOCKS;

		if (copy_from_user(vaddr, buf, BLOCKSIZE) != 0) {
			return -EFAULT;
		}

		switch (cparam.mode) {
		  case SS_MODE_SW:
			crypt_process_sw();
			break;
		  case SS_MODE_PIO:
			crypt_process_pio();
			break;
		  case SS_MODE_DMA:
			if (mutex_trylock(&dma_mutex)) {
				crypt_process_dma();
			}
			break;
		}

		buf += BLOCKSIZE;
		tcount -= BLOCKSIZE;
	}

	return count;
}

static ssize_t ss_crypto_read(struct file * file, char __user * buf,
                        size_t count, loff_t *ppos)
{
	void *vaddr;
	int tcount = count;
	if (count & (BLOCKSIZE - 1)) return -EINVAL;

	while (tcount > 0) {

		if (wait_event_interruptible(read_wq, atomic_read(&handledblk) > 0))
			return -EINTR;

		vaddr = get_buffer_va(readptr);
		if (copy_to_user(buf, vaddr, BLOCKSIZE) != 0)
			return -EFAULT;

		readptr = (readptr + 1) % NUMBLOCKS;
		atomic_dec(&handledblk);
		atomic_dec(&usedblk);
		wake_up(&write_wq);

		buf += BLOCKSIZE;
		tcount -= BLOCKSIZE;
	}
	return count;
}

static long ss_crypto_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct ss_crypt_param tparam;
	int ret = 0;

	switch (cmd) {

	  case IOCTL_SS_INIT:
		if (copy_from_user(&tparam, (void __user *) arg, sizeof(struct ss_crypt_param)))
			return -EFAULT;
		ret = crypt_init(&tparam);
		break;

	  default:
		ret = -ENOSYS;
		break;

	}

	return ret;
}

static ssize_t ss_crypto_op_show(struct class *cls, struct class_attribute *attr, char *buf)
{
	char *s = "unknown";
	switch (cparam.op) {
	  case SS_ENCRYPT: s = "encrypt"; break;
	  case SS_MODE_PIO: s = "decrypt"; break;
	}
	return sprintf(buf, "%s\n", s);
}

static ssize_t ss_crypto_op_store(struct class *cls, struct class_attribute *attr, const char *buf, size_t count)
{
	if (buf[0] == 'e' || buf[0] == 'E') {
		cparam.op = SS_ENCRYPT;
	} else if (buf[0] == 'd' || buf[0] == 'D') {
		cparam.op = SS_DECRYPT;
	} else {
		ERR("unknown operation '%s'\n", buf);
		return -EINVAL;
	}
	crypt_init(NULL);
	return count;
}
static ssize_t ss_crypto_mode_show(struct class *cls, struct class_attribute *attr, char *buf)
{
	char *s = "unknown";
	switch (cparam.mode) {
	  case SS_MODE_SW: s = "software"; break;
	  case SS_MODE_PIO: s = "pio"; break;
	  case SS_MODE_DMA: s = "dma"; break;
	}
	return sprintf(buf, "%s\n", s);
}

static ssize_t ss_crypto_mode_store(struct class *cls, struct class_attribute *attr, const char *buf, size_t count)
{
	if (buf[0] == 's' || buf[0] == 'S') {
		cparam.mode = SS_MODE_SW;
	} else if (buf[0] == 'p' || buf[0] == 'P') {
		cparam.mode = SS_MODE_PIO;
	} else if (buf[0] == 'd' || buf[0] == 'D') {
		cparam.mode = SS_MODE_DMA;
	} else {
		ERR("unknown mode '%s'\n", buf);
		return -EINVAL;
	}
	crypt_init(NULL);
	return count;
}

static ssize_t ss_crypto_algo_show(struct class *cls, struct class_attribute *attr, char *buf)
{
	char *s = "unknown";
	switch (cparam.algo) {
	  case SS_AES_128: s = "AES128"; break;
	  case SS_AES_192: s = "AES192"; break;
	  case SS_AES_256: s = "AES256"; break;
	  case SS_DES: s = "DES"; break;
	  case SS_3DES: s = "3DES"; break;
	}
	return sprintf(buf, "%s\n", s);
}

static ssize_t ss_crypto_algo_store(struct class *cls, struct class_attribute *attr, const char *buf, size_t count)
{
	if (strncasecmp(buf, "AES128", 6) == 0) {
		cparam.algo = SS_AES_128;
	} else if (strncasecmp(buf, "AES192", 6) == 0) {
		cparam.algo = SS_AES_128;
	} else if (strncasecmp(buf, "AES256", 6) == 0) {
		cparam.algo = SS_AES_256;
	} else if (strncasecmp(buf, "DES", 3) == 0) {
		cparam.algo = SS_DES;
	} else if (strncasecmp(buf, "3DES", 4) == 0) {
		cparam.algo = SS_3DES;
	} else {
		ERR("unknown method '%s'\n", buf);
		return -EINVAL;
	}
	crypt_init(NULL);
	return count;
}

static ssize_t ss_crypto_key_store(struct class *cls, struct class_attribute *attr, const char *buf, size_t count)
{
	if (count != 128/8 && count != 192/8 && count != 256/8) {
		ERR("wrong key size %d\n", count);
		return -EINVAL;
	}
	memcpy(cparam.key, (void *) buf, count);
	crypt_init(NULL);
	return count;
}

static ssize_t ss_crypto_iv_store(struct class *cls, struct class_attribute *attr, const char *buf, size_t count)
{
	if (count != 64/8 && count != 128/8) {
		ERR("wrong initialization vector size %d\n", count);
		return -EINVAL;
	}
	memcpy(cparam.iv, (void *) buf, count);
	crypt_init(NULL);
	return count;
}

static ssize_t ss_crypto_dump_store(struct class *cls, struct class_attribute *attr, const char *buf, size_t count)
{
	ERR("SS_CTL=%08x SS_FCSR=%08x SS_ICSR=%08x\n", SS_CTL_REG, SS_FCSR_REG, SS_ICSR_REG);
	ERR("DMA IRQEN: %08x DMA IRQPEND: %08x\n", DMA_IRQ_EN_REG, DMA_IRQ_PEND_REG);
	ERR("CH8: cfg=%08x src=%08x dest=%08x bc=%x\n", DDMA0_CFG_REG, DDMA0_SRC_ADDR_REG, DDMA0_DEST_ADDR_REG, DDMA0_BC_REG);
	ERR("CH9: cfg=%08x src=%08x dest=%08x bc=%x\n", DDMA1_CFG_REG, DDMA1_SRC_ADDR_REG, DDMA1_DEST_ADDR_REG, DDMA1_BC_REG);
	return count;
}

static CLASS_ATTR(mode, 0600, ss_crypto_mode_show, ss_crypto_mode_store);
static CLASS_ATTR(algo, 0600, ss_crypto_algo_show, ss_crypto_algo_store);
static CLASS_ATTR(operation, 0600, ss_crypto_op_show, ss_crypto_op_store);
static CLASS_ATTR(key, 0200, NULL, ss_crypto_key_store);
static CLASS_ATTR(iv, 0200, NULL, ss_crypto_iv_store);
static CLASS_ATTR(dump, 0200, NULL, ss_crypto_dump_store);

static const struct file_operations ss_crypto_fops = {
	.owner = THIS_MODULE,
	.read = ss_crypto_read,
	.write = ss_crypto_write,
	.unlocked_ioctl = ss_crypto_ioctl,
};

static struct miscdevice ss_crypto_miscdev = {
	237 /*MISC_DYNAMIC_MINOR*/,
	SS_CRYPTO_NAME,
	&ss_crypto_fops
};

static int __init ss_crypto_init(void)
{
	struct dma_hw_conf dma_config;
	int ret;

	ssreg = ioremap_nocache(SUNXI_SS_REG_BASE, 0x1000);
	if (IS_ERR(ssreg)) {
		ERR("could not map ss registers\n");
		return -ENOMEM;
	}

	dmareg = ioremap_nocache(SUNXI_DMA_REG_BASE, 0x1000);
	if (IS_ERR(ssreg)) {
		ERR("could not map dma registers\n");
		return -ENOMEM;
	}

	pll5p = clk_get(NULL, "sdram_pll_p");
	if (IS_ERR(pll5p)) {
		ERR("could not get pll5 clock\n");
		return -ENODEV;
	}
	INF("pll5: %d\n", (int) clk_get_rate(pll5p));
/*
	pll62 = clk_get(NULL, "sata_pll_2");
	if (IS_ERR(pll62)) {
		ERR("could not get pll6 clock\n");
		return -ENODEV;
	}
	INF("pll6: %d\n", clk_get_rate(pll62));
*/
	busclk = clk_get(NULL, "ahb_ss");
	if (IS_ERR(busclk)) {
		ERR("could not get ahb_ss clock\n");
		return -ENODEV;
	}
	clk_enable(busclk);

	ssclk = clk_get(NULL, "ss");
	if (IS_ERR(ssclk)) {
		ERR("could not get ss clock\n");
		return -ENODEV;
	}

	if (clk_set_parent(ssclk, pll5p) != 0) {
		ERR("could not set parent bus clock\n");
	}

	clk_set_rate(ssclk, 150000000);
	clk_enable(ssclk);
	INF("ssclk: %d\n", (int) clk_get_rate(ssclk));

	dma_rx_hdle = sw_dma_request(DMACH_DSSR, &ss_dma_rx_client, NULL);
	dma_tx_hdle = sw_dma_request(DMACH_DSST, &ss_dma_tx_client, NULL);
	if (dma_rx_hdle < 0 || dma_tx_hdle < 0) {
		ERR("failed to allocate dma channel\n");
		return -ENOMEM;
	}

#ifndef USE_DIRECT_DMA

	memset(&dma_config, 0, sizeof(struct dma_hw_conf));
	dma_config.drqsrc_type  = D_DRQSRC_SDRAM;
	dma_config.drqdst_type  = DRQ_TYPE_SS;
	dma_config.xfer_type    = DMAXFER_D_SWORD_S_SWORD;
	dma_config.address_type = DMAADDRT_D_IO_S_LN;
	dma_config.dir          = SW_DMA_WDEV;
	dma_config.reload       = 0;
	dma_config.from         = buf_phys;
	dma_config.to           = (u32) &SS_RXFIFO_REG;
	dma_config.hf_irq       = SW_DMA_IRQ_FULL;
	dma_config.cmbk         = 0; //0x07070707;

	sw_dma_config(dma_tx_hdle, &dma_config);

	memset(&dma_config, 0, sizeof(struct dma_hw_conf));
	dma_config.drqsrc_type  = DRQ_TYPE_SS;
	dma_config.drqdst_type  = D_DRQSRC_SDRAM;
	dma_config.xfer_type    = DMAXFER_D_SWORD_S_SWORD;
	dma_config.address_type = DMAADDRT_D_LN_S_IO;
	dma_config.dir          = SW_DMA_RDEV;
	dma_config.reload       = 0;
	dma_config.from         = (u32) &SS_TXFIFO_REG;
	dma_config.to           = buf_phys;
	dma_config.hf_irq       = SW_DMA_IRQ_FULL;
	dma_config.cmbk         = 0; //0x07070707;

	sw_dma_config(dma_rx_hdle, &dma_config);

	sw_dma_set_buffdone_fn(dma_tx_hdle, NULL);
	sw_dma_set_buffdone_fn(dma_rx_hdle, ss_crypto_dma_callback);

#else

	sw_dma_set_direct_callback(dma_rx_hdle & 15, dma_rx_callback);
	sw_dma_set_direct_callback(dma_tx_hdle & 15, dma_tx_callback);

#endif

	DBG("DMA tx: %x rx: %x\n", dma_tx_hdle, dma_rx_hdle);

	buf_virt = dma_alloc_coherent(NULL, NUMBLOCKS * BLOCKSIZE + FLUSHTAIL, &buf_phys, GFP_KERNEL);
	if (! buf_virt) {
		ERR("failed to allocate memory\n");
		return -ENOMEM;
	}

	cparam.mode= SS_MODE_DMA;
	cparam.op = SS_ENCRYPT;
	cparam.algo = SS_AES_256;

	if (misc_register(&ss_crypto_miscdev)) {
		ERR("failed to register device\n");
		return -ENODEV;
	}

	ss_crypto_class = class_create(THIS_MODULE, SS_CRYPTO_NAME);
	if (IS_ERR(ss_crypto_class)) {
		ERR("failed to create class\n");
		return -ENODEV;
	}

	ret =  class_create_file(ss_crypto_class, &class_attr_mode);
	ret |= class_create_file(ss_crypto_class, &class_attr_algo);
	ret |= class_create_file(ss_crypto_class, &class_attr_operation);
	ret |= class_create_file(ss_crypto_class, &class_attr_key);
	ret |= class_create_file(ss_crypto_class, &class_attr_iv);
	ret |= class_create_file(ss_crypto_class, &class_attr_dump);
	if (ret != 0) {
		ERR("failed to create sysfs files\n");
	}

	crypt_init(NULL);

	INF("registered\n");
	return 0;
}

module_init(ss_crypto_init);

