/*
 * Direct MTD block device access, readonly with NAND bad block skipping
 *
 * (C) 2007 Andre McCurdy, NXP Semiconductors
 * (C) 2000-2003 Nicolas Pitre <nico@cam.org>
 * (C) 1999-2003 David Woodhouse <dwmw2@infradead.org>
 */

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/vmalloc.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/blktrans.h>
#include <linux/mutex.h>
#include <linux/iblockstruct.h>

#define AIMAGE_DECRYPTION
///////#define DEBUG_FSKEY "abcdefghijklmnop" // DO NOT CHECK-IN WITH THIS DEFINED!

#ifdef AIMAGE_DECRYPTION

#ifdef CONFIG_PNX8335_AES
#include <linux/pnx8335_aes.h>
#else
#include <linux/crypto.h>
#include <linux/scatterlist.h>
extern int decrypt_16_with_private_key(unsigned char* src, unsigned char* dst);

#endif
#include <linux/random.h>

#endif /* AIMAGE_DECRYPTION */

#include <mtd/swupbbt.h>

#define DEV_BLOCKSIZE		(512)			/* fixed by MTD block driver infrastructure */
#define CACHE_SIZE		(4 * 1024)		/* must be a multiple of DEV_BLOCKSIZE and a fraction of NAND erase block size */
#define CACHE_BASE_INVALID	((unsigned long) -1)

#if defined(CONFIG_ARCH_BCM2708) || defined(CONFIG_ARCH_CAPRI)
#define CRAMFS_OFFSET (512*1024)
#else
#define CRAMFS_OFFSET (0)
#endif

/*
   Pick a major device number to 'borrow'... whichever one we pick, the
   driver which really owns that number MUST NOT ALSO BE INCLUDE IN THE KERNEL !!

   31 : The offical mtdblock major number. Picking this allows the driver to be
	tested as a drop in (readonly) replacement for the normal mtdblock driver.

   44 : The mtd 'ftl.c' major number. Note that the bootloader, fstab, etc,
	etc may need tweaking to use an unexpected device...
*/

#if 0
#define ROBBS_NAME	"mtdblock"
#define ROBBS_MAJOR	31
#else
#define ROBBS_NAME	"mtdblock_robbs"
#define ROBBS_MAJOR	44
#define ROBBS_RAW_NAME	"mtdblock_robbs_raw"
#define ROBBS_RAW_MAJOR	48
#define ROBBS_FULL_NAME "mtdblock_robbs_full"
#define ROBBS_FULL_MAJOR 45
#endif

#if defined (AIMAGE_DECRYPTION)
#define IMG_MAGIC                       0x41676d69
#define IMG_MAGIC2                      0x43634d52
#define IMG_TYPE_INITFS_CRAMFS          (0x0A)
#define IMG_TYPE_APP_CRAMFS             (0x0D)
#define IMG_FLAG_ENCMODE_NONE           (0 << 6)
#define IMG_FLAG_ENCMODE_CBC_ONEPASS    (1 << 6)
#define IMG_FLAG_ENCMODE_CBC_4KBLOCKS   (2 << 6)
#define IMG_FLAG_ENCMODE_CTR            (3 << 6)
#define IMG_FLAG_ENCMODE_MASK           (7 << 6)

typedef struct
{
	unsigned int _d0[2];
	unsigned int magic;
	unsigned int magic2;
	unsigned int _d1[2];
	unsigned int type;
	unsigned int length;
	unsigned int data_length;
	unsigned int _d2[3];
	unsigned int flags;
	unsigned int build_time;
	unsigned int _d3[33];
	unsigned int hash[5];
	unsigned int boot_version;
	unsigned int reserved[7];
	unsigned int iv[4];
}
aimage_v1_header_t;

/* We use the last erase block in an encrypted partition to keep a copy of 
 * the iblock which among other things contains the fskey for the partition.
 * The img_info_t at the end of the block verifies the block's contents.
 */
typedef struct
{
	unsigned int magic;
	unsigned int build_time;
	unsigned int hash[5];
	unsigned int iv[4];
	unsigned int _pad[21];
} img_info_t;

#define IBLOCK_HEADER_MAGIC  0xE76A38EB

#endif

#if defined (AIMAGE_DECRYPTION)
struct encrypted_region {
	struct encrypted_region* next;
	unsigned long start_pos;
	unsigned long block_pos;
	unsigned long length;
	unsigned char iv_prefix[12];
};
#endif

struct mtdblk_dev {
	struct mtd_blktrans_dev mbd;
	int count;
	unsigned int bblist_count;
	unsigned long *bblist_data;
	unsigned long cache_base;
	unsigned long cramfs_offset;
	struct mutex cache_mutex;
#if defined (AIMAGE_DECRYPTION)
	struct encrypted_region* encrypted_regions;
	unsigned char key[16];
#ifndef CONFIG_PNX8335_AES
	struct crypto_blkcipher* tfm_decrypt;
#endif
#endif
	unsigned char cache_buffer[CACHE_SIZE];
};

static struct mutex mtdblks_lock;
static unsigned char* iblock_from_mtd4 = NULL;

#ifdef AIMAGE_DECRYPTION

#ifndef CONFIG_PNX8335_AES
static int init_crypt(struct crypto_blkcipher* tfm, const u8* key, unsigned int key_size, const u8* iv, unsigned int iv_size)
{
	u8* crypt_iv = crypto_blkcipher_crt(tfm)->iv;
	unsigned int crypt_iv_size = crypto_blkcipher_ivsize(tfm);
	if (crypt_iv_size != iv_size) {
		printk("*** IV size %d != %d\n", crypt_iv_size, iv_size);
		return -1;
	}
	memcpy(crypt_iv, iv, iv_size);
	crypto_blkcipher_setkey(tfm, key, key_size);
	return 0;
}
#endif

static int decrypt(struct mtdblk_dev *mtdblk, const u8* data, unsigned int data_size, const u8* key, unsigned int key_size, const u8* iv, unsigned int iv_size)
{
#ifdef CONFIG_PNX8335_AES
	struct aes_op op;
	op.src = data;
	op.dst = op.src;
	op.blocks = data_size / 16;
	op.cmd = AESCMD_DECRYPT_CBC_KEY_USER;
	memcpy(op.key, key, 16);
	memcpy(op.iv, iv, 16);
	if (aes_process_op_in_kernel (&op) != 0) {
		printk ("aes operation failed !!\n");
		return -EIO;
	}
#else
	struct blkcipher_desc desc;
	struct scatterlist sg;
	int result;

	if (init_crypt(mtdblk->tfm_decrypt, key, key_size, iv, iv_size) < 0)
		return -EIO;
	desc.tfm = mtdblk->tfm_decrypt;
	desc.flags = 0;
	sg_init_one(&sg, data, data_size);
	result = crypto_blkcipher_decrypt(&desc, &sg, &sg, data_size);
	if (result < 0) {
		printk("decrypt error %d\n", result);
		return result;
	}
#endif
	return 0;
}

#endif

static unsigned long get_img_info_offset(struct mtdblk_dev *mtdblk)
{
	struct mtd_info *mtd = mtdblk->mbd.mtd;
	unsigned long offset = mtd->size - mtd->erasesize - mtdblk->cramfs_offset;
	offset -= mtdblk->bblist_count * mtd->erasesize;
	//printk("img_info_offset %ld - %ld - %ld - (%ld*128k)\n", (long)mtd->size, (long)mtd->erasesize, (long)mtdblk->cramfs_offset, (long) mtdblk->bblist_count);
	return offset;
}

static struct encrypted_region* get_encrypted_region(struct mtdblk_dev* mtdblk, unsigned long pos, unsigned long len)
{
	struct encrypted_region* region;

	for (region = mtdblk->encrypted_regions;  region != NULL;  region = region->next) {
		if (pos + len > region->start_pos && pos < region->start_pos + region->length)
			return region;
	}
	return NULL;
}

static int mtdblock_robbs_cache_load (struct mtdblk_dev *mtdblk, unsigned long pos_phys, unsigned long pos_virt)
{
	struct mtd_info *mtd = mtdblk->mbd.mtd;
	size_t retlen;
	int ret;

//	printk ("%s: %s at 0x%08lx\n", __FUNCTION__, mtd->name, pos);

	if (pos_phys & (CACHE_SIZE - 1)) {
		printk ("%s: pos_phys (0x%08lx) not CACHE_SIZE aligned ?!?\n", __FUNCTION__, pos_phys);
		return -EIO;
	}

	mutex_lock (&mtdblk->cache_mutex);

	mtdblk->cache_base = CACHE_BASE_INVALID;
	ret = TIMED_CALL(mtd, mtd->read (mtd, pos_phys, CACHE_SIZE, &retlen, mtdblk->cache_buffer, 0), read, &retlen);
	if (ret == 0 || ret == -EUCLEAN) {
		mtdblk->cache_base = pos_phys;

#if defined (AIMAGE_DECRYPTION)
{
	struct encrypted_region* region = get_encrypted_region(mtdblk, pos_virt, CACHE_SIZE);
	if (region != NULL) {
		/* Some part (maybe all) of this data is encrypted.
		 * Decrypt the encrypted part. */
		unsigned int enc_start_block;
		unsigned char* src;
		unsigned int size;
		unsigned char iv[16];

		src = mtdblk->cache_buffer;
		size = CACHE_SIZE;
		BUG_ON(region->block_pos > pos_virt);
		enc_start_block = (pos_virt - region->block_pos) / 16;
		if (region->start_pos > pos_virt) {
			/* There are unencrypted bytes at the beginning of the buffer. */
			unsigned long unenc = region->start_pos - pos_virt;
			BUG_ON(unenc >= CACHE_SIZE);
			src += unenc;
			size -= unenc;
			enc_start_block += unenc / 16;
		}
		if (pos_virt + CACHE_SIZE > region->start_pos + region->length) {
			/* There are unencrypted bytes at the end of the buffer. */
			size -= (pos_virt + CACHE_SIZE) - (region->start_pos + region->length);
		}
		memcpy(iv, region->iv_prefix, 12);
		iv[12] = (enc_start_block >> 24) & 0xFF;
		iv[13] = (enc_start_block >> 16) & 0xFF;
		iv[14] = (enc_start_block >> 8) & 0xFF;
		iv[15] = (enc_start_block >> 0) & 0xFF;
		ret = decrypt(mtdblk, src, size, mtdblk->key, sizeof(mtdblk->key), iv, sizeof(iv));
	}
}
#endif
	}

	mutex_unlock (&mtdblk->cache_mutex);

	if (ret && ret != -EUCLEAN)
		return ret;
	if (retlen != CACHE_SIZE)
		return -EIO;

	return 0;
}

static unsigned long robbs_offset(struct mtd_blktrans_dev *dev, unsigned long pos_base)
{
	struct mtdblk_dev *mtdblk = container_of(dev, struct mtdblk_dev, mbd);
	struct mtd_info *mtd = mtdblk->mbd.mtd;
	int i;

	if (mtdblk->bblist_count != 0) {
		for (i = 0; i < mtdblk->bblist_count; i++) {
			if (pos_base < mtdblk->bblist_data[i])
				break;
			pos_base += mtd->erasesize;
			if (pos_base >= mtd->size) {
				printk ("%s: %s 0x%08lx slipped out of range (0x%08llx)\n",
					__FUNCTION__, mtd->name, pos_base, mtd->size);
				return -1;
			}
		}
	}
	return pos_base;
}

static int mtdblock_robbs_readsect (struct mtd_blktrans_dev *dev, unsigned long lba, char *buf)
{
	struct mtdblk_dev *mtdblk = container_of(dev, struct mtdblk_dev, mbd);
	struct mtd_info *mtd = mtdblk->mbd.mtd;
	unsigned long pos_abs = mtdblk->cramfs_offset + lba * DEV_BLOCKSIZE;
	unsigned long pos_base = (pos_abs / CACHE_SIZE) * CACHE_SIZE;
	unsigned long pos_offset = pos_abs - pos_base;
	unsigned long pos_base_original = pos_base - mtdblk->cramfs_offset;
	unsigned long pos_boot_version = offsetof(aimage_v1_header_t, boot_version);
	int result;

//	printk ("%s: %s at 0x%08lx (0x%08lx + %4ld)\n", __FUNCTION__, mtd->name, pos_abs, pos_base, pos_offset);

	pos_base = robbs_offset(dev, pos_base);
	if (pos_base == (unsigned long)-1)
		return -EIO;

//	printk("robbs readsect %lx: base %lx (cached %lx, bad %d), orig %lx, offset %ld\n", lba, pos_base, mtdblk->cache_base, mtdblk->bblist_count, pos_base_original, pos_offset);
	if (pos_base != mtdblk->cache_base)
		if ((result = mtdblock_robbs_cache_load (mtdblk, pos_base, pos_base_original)) != 0)
			return result;

	memcpy (buf, mtdblk->cache_buffer + pos_offset, DEV_BLOCKSIZE);
	if (mtd->index == 1 && pos_abs == 0) {
		// Force boot_version in aimage hdr to zero.
		// This is how it was when the Merkle tree was generated, 
		// but it may have been changed by the boot code since then.
		memset(buf + pos_boot_version, 0, sizeof(unsigned long));
	}

	return 0;
}

extern
int erase_write (struct mtd_info *mtd, unsigned long pos,
			int len, const char *buf);

#if defined (AIMAGE_DECRYPTION)
static int decrypt_img_key(unsigned char* key)
{
#ifdef CONFIG_PNX8335_AES
	unsigned char result[16];
	struct aes_op op;

	op.src = key;
	op.blocks = 1;
	op.dst = result;
	op.cmd = AESCMD_DECRYPT_ECB_KEY_KC_BYPASS;
	if (aes_process_op_in_kernel (&op) != 0) {
		printk ("decrypt_img_key: aes operation failed !!\n");
		return -EIO;
	}
	memcpy(key,result,16);
	return 0;
#else
	return decrypt_16_with_private_key(key, key);
#endif
}
#endif

#if defined (AIMAGE_DECRYPTION)
/* Read the last block of an encrypted partition and get the fs key from it.
 */
static int get_img_info(struct mtd_blktrans_dev *mbd, aimage_v1_header_t *ahdr)
{
	struct mtdblk_dev *mtdblk = container_of(mbd, struct mtdblk_dev, mbd);
	struct mtd_info *mtd = mbd->mtd;
	unsigned char *img_info = NULL;
	img_info_t* ihdr;
	struct iblock_struct* iblock;
	unsigned long img_info_offset;
	size_t retlen;
	int ret = 0;

	if (mtd->index == 1) {
		/* Read last block of the partition */
		mtdblk->cache_base = CACHE_BASE_INVALID;
		img_info_offset = robbs_offset(mbd, get_img_info_offset(mtdblk) + mtdblk->cramfs_offset);
		if (img_info_offset < 0)
			return -EIO;
		img_info = vmalloc(mtd->erasesize);
		if (!img_info)
			return -ENOMEM;
		ihdr = (img_info_t*) (img_info + mtd->erasesize - sizeof(img_info_t));
		ret = TIMED_CALL(mtd, mtd->read(mtd, img_info_offset, mtd->erasesize, &retlen, img_info, 0), read, &retlen);
		if (ret == -EUCLEAN)
			ret = 0;
		if (ret != 0 || retlen != mtd->erasesize) {
			printk("Cannot read img info: error %d, len %d\n", ret, retlen);
			ihdr->magic = 0; /* mark invalid */
		}

		/* Make sure img_info is valid and matches the aimage. */
		if (ihdr->magic != IBLOCK_HEADER_MAGIC ||
		    ihdr->build_time != ahdr->build_time ||
		    memcmp(ihdr->hash, ahdr->hash, sizeof(ihdr->hash)) ||
		    memcmp(ihdr->iv, ahdr->iv, sizeof(ihdr->iv))) {

			/* Invalid img_info: copy valid iblock from mtd4. */
			printk("Copy iblock from mtd4 to %ld in %s (magic %x)\n", 
				img_info_offset, mtd->name, ihdr->magic);
			if (!iblock_from_mtd4) {
				printk("no iblock in mtd1 or mtd4: giving up\n");
				ret = -EIO;
				goto out;
			}
			memset(ihdr, 0, sizeof(img_info_t));
			ihdr->magic = IBLOCK_HEADER_MAGIC;
			ihdr->build_time = ahdr->build_time;
			memcpy(ihdr->hash, ahdr->hash, sizeof(ihdr->hash));
			memcpy(ihdr->iv, ahdr->iv, sizeof(ihdr->iv));
			memcpy(img_info, iblock_from_mtd4, sizeof(struct iblock_struct));
			ret = erase_write(mtd, img_info_offset, mtd->erasesize, img_info);
			if (ret)
				goto out;
		} else {
			printk("Found iblock at %ld in %s\n", img_info_offset, mtd->name);
		}
		iblock = (struct iblock_struct*) img_info;

	} else {
		if (!iblock_from_mtd4) {
			printk("no iblock in mtd4: giving up\n");
			ret = -EIO;
			goto out;
		}
		iblock = (struct iblock_struct*) iblock_from_mtd4;
	}

	/* We don't yet know whether there are any encrypted aimages on this partition.
	 * So get the key unconditionally just in case. */
	if (1) {
		/* Decrypt encrypted key to get the real key. */
		memcpy(mtdblk->key, iblock->fskey, sizeof(mtdblk->key));
		//printk("%s: Encrypted FS detected, Encrypted Key: %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n", 
		//	mtd->name,
		//	mtdblk->key[0], mtdblk->key[1], mtdblk->key[ 2], mtdblk->key[ 3], mtdblk->key[ 4], mtdblk->key[ 5], mtdblk->key[ 6], mtdblk->key[ 7],
		//	mtdblk->key[8], mtdblk->key[9], mtdblk->key[10], mtdblk->key[11], mtdblk->key[12], mtdblk->key[13], mtdblk->key[14], mtdblk->key[15]);

		/* Decrypt to get the real key. */
		ret = decrypt_img_key(mtdblk->key);
		#ifdef DEBUG_FSKEY
		 memcpy(mtdblk->key, DEBUG_FSKEY, sizeof(mtdblk->key));
		#endif
	}
 out:
 	if (img_info) vfree(img_info);
	return ret;
}
#endif

static bool is_dev_unit;
static int __init is_dev_unit_setup(char *s)
{
	if (s[0] == '1' && s[1] == 0)
		is_dev_unit = 1;
	return 1;
}
__setup("dev=", is_dev_unit_setup);

static uint8_t *bbt_cache = NULL;

/**
 * mtdblock_load_swupbbt - Read softare upgrade BBT from active partition 
 *
 * The BBT will be loaded in order to map out bad blocks in activee partition
 * so that cramfs rootfs can be correctly mounted. If error occurs, ignore
 * BBT and assume no new bad blocks by filling cache copy with 0xff.
 */
static int mtdblock_load_swupbbt(struct mtd_info *mtd)
{
	loff_t bbt_off;
	size_t bbt_len, retlen;
	int ret = 0;

	/* Already initialized */
	if (bbt_cache)
		return 0;

	/* Allocate BBT cache */
	/* Force 32 bit division since 64 bit div doesn't work and heavy,
	   this also means limiting mtd->size to 4GB which is enough */
	bbt_len = (uint32_t)mtd->size / mtd->erasesize / SWUPBBT_N_BLK_PER_BYTE
		+ SWUPBBT_CSUM_SIZE;
	if (bbt_len <= SWUPBBT_CSUM_SIZE) {
		printk(KERN_WARNING "%s: Invalid BBT length %d\n",
		       __func__, bbt_len);
		return -EINVAL;
	}
	bbt_cache = kmalloc(bbt_len, GFP_KERNEL);
	if (!bbt_cache)
		return -ENOMEM;
	swupbbt_clear_bbt(bbt_cache, bbt_len);

	/* Read BBT in the last block of the partition, same block as iblock */
	bbt_off = mtd->size - mtd->erasesize + SWUPBBT_BLK_OFF;
	ret = TIMED_CALL(mtd, mtd->read(mtd, bbt_off, bbt_len, &retlen, bbt_cache, 0), read, &retlen);
	if (ret < 0) {
		printk(KERN_WARNING "%s: Error reading BBT 0x%08llx\n",
		       __func__, bbt_off);
		goto read_err;
	}

	/* Verify checksum of BBT content */
	if (swupbbt_verify_checksum(bbt_cache, bbt_len)) {
		printk(KERN_INFO "%s: Ignore BBT with bad checksum\n",
		       __func__);
		ret = -EINVAL;
		goto read_err;
	}

	printk(KERN_INFO "%s: Load BBT length %d total %d bad blocks\n",
	       __func__, bbt_len, swupbbt_total_badblock(bbt_cache, bbt_len));
	return 0;

read_err:
	swupbbt_clear_bbt(bbt_cache, bbt_len);

	return ret;
}

static int mtdblock_robbs_open (struct mtd_blktrans_dev *mbd)
{
	struct mtdblk_dev *mtdblk = container_of(mbd, struct mtdblk_dev, mbd);
	struct mtd_info *mtd = mbd->mtd;
	unsigned long pos;
	unsigned int count;
	unsigned int blk;
	int ret = 0;

	/*
	   Fixme: does mtdblock core code provide locking around calls to this function ?!?
	   If not, things look broken for SMP or preempt...
	*/

	mutex_lock(&mtdblks_lock);
	if (mtdblk->count) {
		mtdblk->count++;
		printk ("mtdblock_open: %s, count: %d\n", mtd->name, mtdblk->count);
		mutex_unlock(&mtdblks_lock);
		return 0;
	}

	//printk ("%s: %s, erasesize: %d, sizeof(struct mtdblk_dev): %d\n",
	//	__FUNCTION__, mtd->name, mtd->erasesize, sizeof(struct mtdblk_dev));

	mtdblk->cramfs_offset = (mbd->tr->major == ROBBS_FULL_MAJOR) ? 0 : CRAMFS_OFFSET;
	mtdblock_load_swupbbt(mtd);

	mtdblk->bblist_count = 0;
	blk = 0;
	for (pos = 0; pos < mtd->size; pos += mtd->erasesize) {
		if (mtd->block_isbad (mtd, pos)
		    || (bbt_cache && swupbbt_block_isbad(bbt_cache, blk)))
		{
			mtdblk->bblist_count++;
			printk ("%s: badpos: 0x%08lx (count %d)\n",
				__FUNCTION__, pos, mtdblk->bblist_count);
		}
		blk++;
	}

	if (mtdblk->bblist_count != 0) {
		mtdblk->bblist_data = kmalloc (mtdblk->bblist_count * sizeof (*mtdblk->bblist_data), GFP_KERNEL);
		if (! mtdblk->bblist_data) {
			kfree (mtdblk);
			ret = -ENOMEM;
			goto out;
		}
		count = 0;
		blk = 0;
		for (pos = 0; pos < mtd->size; pos += mtd->erasesize) {
			if (mtd->block_isbad (mtd, pos)
			    || (bbt_cache && swupbbt_block_isbad(bbt_cache, blk)))
			{
				count++;
//				printk ("%s: badpos: 0x%08lx (count %d)\n", __FUNCTION__, pos, count);
				if (count > mtdblk->bblist_count) {
					printk ("%s: increase in bad block count since first pass !?!\n", __FUNCTION__);
					break;
				}
				mtdblk->bblist_data[count - 1] = pos;
			}
			blk++;
		}
	}

	mtdblk->cache_base = CACHE_BASE_INVALID;
	mutex_init (&mtdblk->cache_mutex);

#if defined (AIMAGE_DECRYPTION)
#ifndef CONFIG_PNX8335_AES
	if (mtdblk->tfm_decrypt == NULL) {
		mtdblk->tfm_decrypt = crypto_alloc_blkcipher("cbc(aes)", 0, CRYPTO_ALG_ASYNC);
		if (IS_ERR(mtdblk->tfm_decrypt)) {
			goto out;
			ret = PTR_ERR(mtdblk->tfm_decrypt);
		}
	}
#endif
	if (mbd->tr->major == ROBBS_MAJOR) { // no decryption in RAW or FULL
		unsigned char* sector_buf = kmalloc(DEV_BLOCKSIZE, GFP_KERNEL);
		aimage_v1_header_t *header = (aimage_v1_header_t *) sector_buf;
		int first_hdr = 1;
		unsigned long pos;

		mtdblk->encrypted_regions = NULL;
		if (!sector_buf) {
			ret = -ENOMEM;
			goto out;
		}

		// Walk through the aimage headers and look for encrypted ones.
		for (pos = 0; ; ) {
			if (pos % DEV_BLOCKSIZE) {
				printk("position %ld in %s not block aligned\n", (long) pos, mtd->name);
				ret = -EINVAL;
				goto out;
			}
			if ((ret = mtdblock_robbs_readsect (mbd, pos / DEV_BLOCKSIZE, sector_buf)) != 0) {
				kfree(sector_buf);
				goto out;
			}
			if (le32_to_cpu(header->magic) != IMG_MAGIC || 
			    le32_to_cpu(header->magic2) != IMG_MAGIC2)
				break;
			if (first_hdr && (ret = get_img_info(mbd, header)) != 0) {
				kfree(sector_buf);
				goto out;
			}
			if ((le32_to_cpu(header->type) == IMG_TYPE_INITFS_CRAMFS ||
			     le32_to_cpu(header->type) == IMG_TYPE_APP_CRAMFS) &&
			    (le32_to_cpu (header->flags) & IMG_FLAG_ENCMODE_MASK) == IMG_FLAG_ENCMODE_CBC_4KBLOCKS)
			{
				struct encrypted_region* region = (struct encrypted_region*)
					kmalloc(sizeof(struct encrypted_region), GFP_KERNEL);
				region->block_pos = pos;
				region->start_pos = pos + 256;
				region->length = le32_to_cpu(header->length) - 256;
				memcpy(region->iv_prefix, header->iv, sizeof(region->iv_prefix));
				printk("MTD encrypted region on %s at 0x%lx, length 0x%lx\n",
					mtd->name, region->start_pos, region->length);
				mutex_lock(&mtdblk->cache_mutex);
				region->next = mtdblk->encrypted_regions;
				mtdblk->encrypted_regions = region;
				/* Any cached data may be invalid since it may have been read without decryption. */
				mtdblk->cache_base = CACHE_BASE_INVALID;
				mutex_unlock(&mtdblk->cache_mutex);
			} else if (first_hdr && !is_dev_unit) {
				/* Non-dev units must use encrypted images */
				ret = -EPERM;
				kfree(sector_buf);
				goto out;
			}
			pos += le32_to_cpu(header->length);
			if (first_hdr && pos < CRAMFS_OFFSET)
				pos = CRAMFS_OFFSET;
			first_hdr = 0;
		}
		kfree(sector_buf);
	}
#endif /* AIMAGE_DECRYPTION */

out:
	if (0 == ret) mtdblk->count = 1;
	mutex_unlock(&mtdblks_lock);
	return ret;
}

static int mtdblock_robbs_release (struct mtd_blktrans_dev *mbd)
{
	struct mtdblk_dev *mtdblk = container_of(mbd, struct mtdblk_dev, mbd);
	//struct mtd_info *mtd = mtdblk->mbd.mtd;

	mutex_lock(&mtdblks_lock);
	if (--mtdblk->count == 0) {
#if defined(AIMAGE_DECRYPTION) && !defined(CONFIG_PNX8335_AES)
		crypto_free_blkcipher(mtdblk->tfm_decrypt);
		mtdblk->tfm_decrypt = NULL;
#endif
		kfree (mtdblk->bblist_data);
		while (mtdblk->encrypted_regions != NULL) {
			struct encrypted_region* region = mtdblk->encrypted_regions;
			mtdblk->encrypted_regions = region->next;
			kfree(region);
		}
	}
	mutex_unlock(&mtdblks_lock);
	return 0;
}

static void mtdblock_robbs_add_mtd(struct mtd_blktrans_ops *tr, struct mtd_info *mtd)
{
	struct mtdblk_dev *dev = kzalloc(sizeof(*dev), GFP_KERNEL);

	if (!dev)
		return;

	dev->mbd.mtd = mtd;
	dev->mbd.devnum = mtd->index;

	dev->mbd.size = mtd->size >> 9;
	dev->mbd.tr = tr;

	dev->mbd.readonly = 1;

	if (add_mtd_blktrans_dev(&dev->mbd))
		kfree(dev);

	if (mtd->index == 4) {
		/* Read the iblock from mtd4.  
		 * We might need it later if we open an an encrypted partition
		 * which has not yet had the img_info written to it.
		 */
		int ret;
		size_t retlen;
		iblock_from_mtd4 = kmalloc(sizeof(struct iblock_struct), GFP_KERNEL); /* leaked */
		ret = TIMED_CALL(mtd, mtd->read(mtd, 0, sizeof(struct iblock_struct), &retlen, iblock_from_mtd4, 0), read, &retlen);
		if (ret && ret != -EUCLEAN) {
			kfree(iblock_from_mtd4);
			iblock_from_mtd4 = NULL;
		}
	}
}

static void mtdblock_robbs_remove_dev(struct mtd_blktrans_dev *dev)
{
	del_mtd_blktrans_dev(dev);
}

static struct mtd_blktrans_ops mtdblock_robbs_tr =
{
	.name		= ROBBS_NAME,
	.major		= ROBBS_MAJOR,
	.part_bits	= 0,
	.blksize 	= 512,
	.open		= mtdblock_robbs_open,
//	.flush		= mtdblock_robbs_flush,
	.release	= mtdblock_robbs_release,
	.readsect	= mtdblock_robbs_readsect,
	.add_mtd	= mtdblock_robbs_add_mtd,
	.remove_dev	= mtdblock_robbs_remove_dev,
	.owner		= THIS_MODULE,
};

static struct mtd_blktrans_ops mtdblock_robbs_raw_tr =
{
	.name		= ROBBS_RAW_NAME,
	.major		= ROBBS_RAW_MAJOR,
	.part_bits	= 0,
	.blksize 	= 512,
	.open		= mtdblock_robbs_open,
//	.flush		= mtdblock_robbs_flush,
	.release	= mtdblock_robbs_release,
	.readsect	= mtdblock_robbs_readsect,
	.add_mtd	= mtdblock_robbs_add_mtd,
	.remove_dev	= mtdblock_robbs_remove_dev,
	.owner		= THIS_MODULE,
};

static struct mtd_blktrans_ops mtdblock_robbs_full_tr =
{
	.name		= ROBBS_FULL_NAME,
	.major		= ROBBS_FULL_MAJOR,
	.part_bits	= 0,
	.blksize 	= 512,
	.open		= mtdblock_robbs_open,
//	.flush		= mtdblock_robbs_flush,
	.release	= mtdblock_robbs_release,
	.readsect	= mtdblock_robbs_readsect,
	.add_mtd	= mtdblock_robbs_add_mtd,
	.remove_dev	= mtdblock_robbs_remove_dev,
	.owner		= THIS_MODULE,
};

static int __init init_mtdblock_robbs (void)
{
	int ret = register_mtd_blktrans (&mtdblock_robbs_tr);
	mutex_init(&mtdblks_lock);
	if (ret != 0) return ret;
	ret = register_mtd_blktrans(&mtdblock_robbs_raw_tr);
	if (ret != 0) return ret;
	ret = register_mtd_blktrans(&mtdblock_robbs_full_tr);
	return ret;
}

static void __exit cleanup_mtdblock_robbs (void)
{
	deregister_mtd_blktrans (&mtdblock_robbs_tr);
	deregister_mtd_blktrans (&mtdblock_robbs_raw_tr);
	deregister_mtd_blktrans (&mtdblock_robbs_full_tr);
}

module_init(init_mtdblock_robbs);
module_exit(cleanup_mtdblock_robbs);

MODULE_LICENSE("GPL");
