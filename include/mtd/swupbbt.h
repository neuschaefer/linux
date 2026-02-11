/*
 * Flash based bad block table for software upgrade 
 *
 * The BBT manages new worn bad blocks compliment to factory marked bad blocks.
 * It is triggerred when software upgrade block erase fails and recorded in the
 * last block along with update image in the upgrade partition. u-boot then
 * reads the BBT to remap block to locate kernel, and mtdblock_robbs driver
 * reads the BBT to remap root file system blocks.
 *
 * Note new bad blocks can't be managed by marking the OOB in the mtd driver
 * because it would have expanded partition boundary.
 *  
 * The BBT life cycle only lives through one software upgrade since it is
 * erased when software update to another version and is replaced by a new BBT.
 * The logic behind is new bad blocks might be recovered after complete erase
 * or false positive at first place, and the next block erase failure can always
 * reconstruct the BBT. 
 *
 * The BBT is located in upgrade partition's last block. It has relative offset
 * from the block instead of fixed absolute offset from device start because
 * partition last block floats depend on factory marked bad blocks. It is also
 * non-zero offset since it coexists with iblock to avoid overlap. Assume 48MB
 * of Active or Update partition and 128KB per block, the layout of the BBT
 * block is like:
 *
 * 0x00000 - 0x00260 iblock_struct
 * 0x01000 - 0x01064 software upgrade BBT, include 4 byte checksum in the end
 * 0x1ff80 - 0x20000 img_info_t
 *
 * The BBT format is an array of bytes with 2 bits per block, similar to
 * standard kernel nand_bbt.c:
 *
 * 11b: block is good
 * 10b: block is marked bad due to wear
 * 
 * It has 4 byte checksum in the end and uses inverse sum because erased flash
 * contains all 0xff.
 *
 * This header are shared by kernel, u-boot, and userland code so that it is 
 * consistent to format or parse BBT among them. To maintain only one copy of
 * this file but still share among different source trees, it is committed under
 * one source tree and then copied to other source trees by Makefile
 *
 * For Austin:
 * port/bcmarm/platform/bcm-11130/linux/include/mtd/swupbbt.h
 */

#ifndef __SWUPBBT_H
#define __SWUPBBT_H

#ifdef __KERNEL__
#include <linux/types.h>
#include <linux/string.h>
typedef long intptr_t;
#else
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#endif

/* BBT offset start from the last block */
#define SWUPBBT_BLK_OFF 0x1000

/* BBT table checksum field size */
#define SWUPBBT_CSUM_SIZE sizeof(uint32_t)

/* Number of blocks covered by each BBT bytes */
#define SWUPBBT_N_BLK_PER_BYTE 4

/*
 * Mark all block as good in the BBT buffer
 * bbt - the start of BBT buffer
 * len - the lengh of BBT buffer include 4 byte checksum field in the end
 *
 * All following function parameters have the same meaning
 */
static inline void swupbbt_clear_bbt(uint8_t *bbt, size_t len)
{
    memset(bbt, 0xff, len);
}

/* Verify if BBT is valid by verifing the checksum in its end */
static inline int swupbbt_verify_checksum(uint8_t *bbt, size_t len)
{
    uint32_t csum = 0;
    size_t i;
    for (i = 0; i < len - SWUPBBT_CSUM_SIZE; i++)
        csum += (uint8_t)~bbt[i];
    csum = ~csum;
    return memcmp(&bbt[len-SWUPBBT_CSUM_SIZE], &csum, SWUPBBT_CSUM_SIZE);
}

/* Generate checksum at the end of the BBT buffer */
static inline uint32_t swupbbt_gen_checksum(uint8_t *bbt, size_t len)
{
    uint32_t csum = 0;
    size_t i;
    for (i = 0; i < len - SWUPBBT_CSUM_SIZE; i++)
        csum += (uint8_t)~bbt[i];
    csum = ~csum;
    memcpy(&bbt[len-SWUPBBT_CSUM_SIZE], &csum, SWUPBBT_CSUM_SIZE);
    return csum;
}

/* Check if a block index in BBT buffer has bad block status */
static inline int swupbbt_block_isbad(uint8_t *bbt, unsigned blk)
{
    return ((bbt[blk >> 2] >> ((blk & 0x03) << 1)) & 0x03) != 0x03;
}

/* Mark one block index in BBT buffer as bad block status */
static inline void swupbbt_block_markbad(uint8_t *bbt, unsigned blk)
{
    bbt[blk >> 2] &= ~(0x01 << ((blk & 0x03) << 1));
}

/* Merge one BBT buffer with another and return if they differs */
static inline int swupbbt_merge(uint8_t *dst_bbt, uint8_t *src_bbt, size_t len)
{
    size_t i;
    int changed = 0;
    for (i = 0; i < len - SWUPBBT_CSUM_SIZE; i++) {
        changed |= src_bbt[i] - dst_bbt[i];
        dst_bbt[i] &= src_bbt[i];
    }
    return changed;
}

/* Check if there is at least one bad block in the BBT buffer */
static inline int swupbbt_has_badblock(uint8_t *bbt, size_t len)
{
    uint32_t csum;
    memcpy(&csum, &bbt[len-SWUPBBT_CSUM_SIZE], SWUPBBT_CSUM_SIZE);
    return csum != 0xffffffff;
}

/* Count number of bits set */
static inline int popcount(unsigned int x)
{
    //return __builtin_popcount(x);
    int count = 0;
    for (; x != 0; x &= x - 1)
        count++;
    return count;
}

/* Count total number of bad blocks in the BBT buffer */
static inline int swupbbt_total_badblock(uint8_t *bbt, size_t len)
{
    unsigned int headtail = (unsigned int)-1;
    int i, total = 0;
    /* aligned portion, assume sizeof(int) == 4 */
    len -= SWUPBBT_CSUM_SIZE;
    for (i = (intptr_t)(bbt+3) & ~0x03; i < ((intptr_t)(bbt+len) & ~0x03); i += 4)
        total += popcount(~*(unsigned int*)(intptr_t)i);

    /* unaligned leading and trailing portion, unlikely happen*/
    memcpy(&headtail, bbt, ((intptr_t)(bbt+3) & ~0x03) - (intptr_t)bbt);
    total += popcount(~headtail);
    headtail = (unsigned int)-1;
    memcpy(&headtail, (void*)(((intptr_t)(bbt+len)) & ~0x03),
           (intptr_t)bbt + len - ((intptr_t)(bbt+len) & ~0x03));
    total += popcount(~headtail);
    return total;
}

#endif // __SWUPBBT_H
