#if defined(CONFIG_BCM_KF_MTD_BCMNAND)
/*
 *
 *  drivers/mtd/bcmspinand/bcm63xx-spinand.c
 *
    <:copyright-BRCM:2011:DUAL/GPL:standard
    
       Copyright (c) 2011 Broadcom Corporation
       All Rights Reserved
    
    Unless you and Broadcom execute a separate written software license
    agreement governing use of this software, this software is licensed
    to you under the terms of the GNU General Public License version 2
    (the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php,
    with the following added to such license:
    
       As a special exception, the copyright holders of this software give
       you permission to link this software with independent modules, and
       to copy and distribute the resulting executable under terms of your
       choice, provided that you also meet, for each linked independent
       module, the terms and conditions of the license of that module.
       An independent module is a module which is not derived from this
       software.  The special exception does not apply to any modifications
       of the software.
    
    Not withstanding the above, under no circumstances may you combine
    this software in any way with any other Broadcom software provided
    under a license other than the GPL, without Broadcom's express prior
    written consent.
    
    :> 


    File: bcm63xx-spinand.c

    Description: 
    This is a device driver for the Broadcom SPINAND flash for bcm63xxx boards.

 */
 
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <asm/io.h>
#include <asm/bootinfo.h>
#include <bcm_map_part.h>
#include <board.h>
#include <linux/slab.h> 
#include <linux/version.h>
#include <generated/autoconf.h>
#include <linux/kernel.h>
#include <linux/spi/spi.h>
#include <linux/types.h>
#include <linux/jffs2.h>
#include <linux/delay.h>

//#undef DEBUG_NAND
//#define DEBUG_NAND
#if defined(DEBUG_NAND)
#define DBG_PRINTF printk
#else
#define DBG_PRINTF(...)
#endif


#define STATUS_DEFAULT NAND_STATUS_TRUE_READY|NAND_STATUS_READY|NAND_STATUS_WP

/* Command codes for the flash_command routine */
#define FLASH_PROG          0x02    /* program load data to cache */
#define FLASH_READ          0x03    /* read data from cache */
#define FLASH_WRDI          0x04    /* reset write enable latch */
#define FLASH_WREN          0x06    /* set write enable latch */
#define FLASH_READ_FAST     0x0B    /* read data from cache */
#define FLASH_GFEAT         0x0F    /* get feature option */
#define FLASH_PEXEC         0x10    /* program cache data to memory array*/
#define FLASH_PREAD         0x13    /* read from memory array to cache */
#define FLASH_SFEAT         0x1F    /* set feature option */
#define FLASH_PROG_RAN      0x84    /* program load data to cache at offset*/
#define FLASH_BERASE        0xD8    /* erase one block in memory array */
#define FLASH_RDID          0x9F    /* read manufacturer and product id */
#define FLASH_RESET         0xFF    /* reset flash */

#define FEATURE_PROT_ADDR   0xA0
#define FEATURE_FEAT_ADDR   0xB0
#define FEATURE_STAT_ADDR   0xC0

/* Feature protectin bit defintion */
#define PROT_BRWD           0x80
#define PROT_BP_MASK        0x38
#define PROT_BP_SHIFT       0x3
#define PROT_BP_ALL         0x7
#define PROT_BP_NONE        0x0
/* giga device only */
#define PROT_INV            0x04
#define PROT_CMP            0x02

/* Feature feature bit defintion */
#define FEAT_OPT_EN         0x40
#define FEAT_ECC_EN         0x10
#define FEAT_DISABLE        0x0
/* giga device only */
#define FEAT_BBI            0x04
#define FEAT_QE             0x01

/* Feature status bit definition */
#define STAT_ECC_MASK         0x30
#define STAT_ECC_SHIFT        4
#define STAT_ECC_GOOD         0
#define STAT_ECC_CORR1        1     /* corrected, bit number 1 to 7*/
#define STAT_ECC_UNCORR       2     /* uncorrectable error*/
#define STAT_ECC_CORR2        3     /* reserved for micron, corrected bit 8 for gigadevice*/
#define STAT_PFAIL            0x8   /* program fail*/
#define STAT_EFAIL            0x4   /* erase fail*/
#define STAT_WEL              0x2   /* write enable latch*/
#define STAT_OIP              0x1   /* operation in progress*/

/* Return codes from flash_status */
#define STATUS_READY        0       /* ready for action */
#define STATUS_BUSY         1       /* operation in progress */
#define STATUS_TIMEOUT      2       /* operation timed out */
#define STATUS_ERROR        3       /* unclassified but unhappy status */

/* Micron manufacturer ID */
#define MICRONPART          0x2C
#define ID_MT29F1G01        0x12
#define ID_MT29F2G01        0x22
#define ID_MT29F4G01        0x32

/* Giga Device manufacturer ID */
#define GIGADEVPART         0xC8
#define ID_GD5F1GQ4         0xF1

// device information bytes required to identify device for SPI NAND
#define SPI_NAND_ID_LENGTH  2

// device information bytes required to identify device for Linux NAND
#define NAND_ID_LENGTH  4
/** Variables. **/

static struct nand_ecclayout spinand_oob_64_bch4 =
{
    .eccbytes = 17,
    .eccpos = { // for ease of use, call the bad block marker an ECC byte as well
        0, 12, 13, 14, 15, // these must be in numerical order
        28, 29, 30, 31,
        44, 45, 46, 47,
        60, 61, 62, 63
    },
    .oobavail = 12, // per 512 bytes? JFFS2 multiplies by 4 to find available OOB size
    .oobfree = {
        {.offset = 1,
         .length = 11},
        {.offset = 16,
         .length = 12},
        {.offset = 24,
         .length = 12},
        {.offset = 48,
         .length = 12}
    }
};

#define FLASH_API_OK                1
#define FLASH_API_OK_BLANK          2
#define FLASH_API_ERROR            -1
#define FLASH_API_CORR             -2

/* the controller will handle operations that are greater than the FIFO size
   code that relies on READ_BUF_LEN_MAX, READ_BUF_LEN_MIN or spi_max_op_len
   could be changed */
#define SPI_BUF_LEN        512    /* largest of the maximum transaction sizes for SPI */
/* this is the slave ID of the SPI flash for use with the SPI controller */
#define SPI_FLASH_SLAVE_DEV_ID    0
/* clock defines for the flash */
#define SPI_FLASH_DEF_CLOCK       781000
#define SPARE_MAX_SIZE          (27 * 16)
#define CTRLR_CACHE_SIZE        512
#define ECC_MASK_BIT(ECCMSK, OFS)   (ECCMSK[OFS / 8] & (1 << (OFS % 8)))

/* legacy and HS controllers can coexist - use bus num to differentiate */
#define LEG_SPI_BUS_NUM  0
#define HS_SPI_BUS_NUM   1

#define SPI_CONTROLLER_STATE_SET             (1 << 31)
#define SPI_CONTROLLER_STATE_CPHA_EXT        (1 << 30)
#define SPI_CONTROLLER_STATE_GATE_CLK_SSOFF  (1 << 29)
#define SPI_CONTROLLER_STATE_ASYNC_CLOCK     (1 << 28)

#define SPI_CONTROLLER_MAX_SYNC_CLOCK 30000000

/* set mode and controller state based on CHIP defaults
   these values do not apply to the legacy controller
   legacy controller uses SPI_MODE_3 and clock is not
   gated */

#if defined(_BCM96816_) || defined(CONFIG_BCM96816)
#define SPI_MODE_DEFAULT              SPI_MODE_1
#define SPI_CONTROLLER_STATE_DEFAULT  (SPI_CONTROLLER_STATE_GATE_CLK_SSOFF | SPI_CONTROLLER_STATE_CPHA_EXT)
#else
#define SPI_MODE_DEFAULT              SPI_MODE_0
#define SPI_CONTROLLER_STATE_DEFAULT  (SPI_CONTROLLER_STATE_GATE_CLK_SSOFF)
#endif


static unsigned int spi_max_op_len = SPI_BUF_LEN;
//static int spi_dummy_bytes         = 0;

/* default to legacy controller - updated later */
static int spi_flash_clock  = SPI_FLASH_DEF_CLOCK;
static int spi_flash_busnum = LEG_SPI_BUS_NUM;
spinlock_t chip_lock;

struct SpiNandChip
{
    unsigned char *chip_name;
    unsigned char chip_device_id[2];
    unsigned long chip_total_size;
    unsigned int chip_num_blocks;
    unsigned int chip_block_size;
    unsigned int chip_page_size;
    unsigned int chip_spare_size;
    unsigned int chip_spare_available;
    unsigned int chip_ecc_offset;
    struct nand_ecclayout *ecclayout;
    unsigned short chip_block_shift;
    unsigned short chip_page_shift;
    unsigned short chip_num_plane;
};

static struct SpiNandChip * pchip;


static struct SpiNandChip SpiDevInfo[] =
{
    {
        .chip_name = "GigaDevice GD5F1GQ4",
        .chip_device_id = {GIGADEVPART, ID_GD5F1GQ4},
        .chip_page_size = 2048,
        .chip_page_shift = 11,
        .chip_block_size = 64 * 2048,   // 64 pages per block x chip_page_size
        .chip_block_shift = 17,
        .chip_spare_size = 64,
        .chip_spare_available = 12, // JFFS2 uses this value x 4 to determine amount of OOB available
        .chip_ecc_offset = 0x840,   // location of ECC bytes
        .chip_num_blocks = 1024,       //1024 blocks total
        .chip_num_plane = 1,
        .chip_total_size = 64 * 2048 * 1024, // chip_block_size x chip_num_blocks
        .ecclayout = &spinand_oob_64_bch4,
    },
    {
        .chip_name = "Micron MT29F1G01",
        .chip_device_id = {MICRONPART, ID_MT29F1G01},
        .chip_page_size = 2048,
        .chip_page_shift = 11,
        .chip_block_size = 64 * 2048,   // 64 pages per block x chip_page_size
        .chip_block_shift = 17,
        .chip_spare_size = 64,
        .chip_spare_available = 12, // JFFS2 uses this value x 4 to determine amount of OOB available
        .chip_ecc_offset = 0x800,   // location of ECC bytes
        .chip_num_blocks = 1024,       //1024 blocks total
        .chip_num_plane = 1,
        .chip_total_size = 64 * 2048 * 1024, // chip_block_size x chip_num_blocks
        .ecclayout = &spinand_oob_64_bch4,
    },
    {
        .chip_name = "Micron MT29F2G01",
        .chip_device_id = {MICRONPART, ID_MT29F2G01},
        .chip_page_size = 2048,
        .chip_page_shift = 11,
        .chip_block_size = 64 * 2048,   // 64 pages per block x chip_page_size
        .chip_block_shift = 17,
        .chip_spare_size = 64,
        .chip_spare_available = 12, // JFFS2 uses this value x 4 to determine amount of OOB available
        .chip_ecc_offset = 0x800,   // location of ECC bytes
        .chip_num_blocks = 2048,       //1024 blocks total
        .chip_num_plane = 2,
        .chip_total_size = 64 * 2048 * 2048, // chip_block_size x chip_num_blocks
        .ecclayout = &spinand_oob_64_bch4,
    },
    {
        .chip_name = "Micron MT29F4G01",
        .chip_device_id = {MICRONPART, ID_MT29F4G01},
        .chip_page_size = 2048,
        .chip_page_shift = 11,
        .chip_block_size = 64 * 2048,   // 64 pages per block x chip_page_size
        .chip_block_shift = 17,
        .chip_spare_size = 64,
        .chip_spare_available = 12, // JFFS2 uses this value x 4 to determine amount of OOB available
        .chip_ecc_offset = 0x800,   // location of ECC bytes
        .chip_num_blocks = 4096,       //1024 blocks total
        .chip_num_plane = 2,
        .chip_total_size = 64 * 2048 * 4096, // chip_block_size x chip_num_blocks
        .ecclayout = &spinand_oob_64_bch4,
    },
    {
        .chip_name = "default",
        .chip_device_id = {0, 0},
        .chip_page_size = 2048,
        .chip_page_shift = 11,
        .chip_block_size = 64 * 2048,   // 64 pages per block x chip_page_size
        .chip_block_shift = 17,
        .chip_spare_size = 64,
        .chip_spare_available = 12, // JFFS2 uses this value x 4 to determine amount of OOB available
        .chip_ecc_offset = 0x800,   // location of ECC bytes
        .chip_num_blocks = 1024,       //1024 blocks total
        .chip_num_plane = 1,
        .chip_total_size = 64 * 2048 * 1024, // chip_block_size x chip_num_blocks
        .ecclayout = &spinand_oob_64_bch4,
    }
};

#ifdef CONFIG_CPU_LITTLE_ENDIAN
static const unsigned short jffs2_clean_marker[] = {JFFS2_MAGIC_BITMASK, JFFS2_NODETYPE_CLEANMARKER, 0x0008, 0x0000};
#else
static const unsigned short jffs2_clean_marker[] = {JFFS2_MAGIC_BITMASK, JFFS2_NODETYPE_CLEANMARKER, 0x0000, 0x0008};
#endif

static struct spi_device * pSpiDevice; // handle for SPI NAND device

static unsigned char * pageBuf;
static unsigned int pageBufI;
static int pageAddr, pageOffset;
static int status = STATUS_DEFAULT;
static bool SpiNandDeviceRegistered = FALSE;

/** Prototypes. **/
static int spi_nand_read_page(unsigned long page_addr, unsigned int page_offset, unsigned char *buffer, int len);
static int spi_nand_write_page(unsigned long page_addr, unsigned int page_offset, unsigned char *buffer, int len);
static int spi_nand_write_enable(void);
static int spi_nand_is_blk_bad(unsigned long addr);
static int spi_nand_mark_blk_bad(unsigned long addr);
static int spi_nand_write_enable(void);
static int spi_nand_write_disable(void);
static void spi_nand_row_addr(unsigned int page_addr, unsigned char* buf);
static void spi_nand_col_addr(unsigned int page_addr, unsigned int page_offset, unsigned char* buf);
static void spi_nand_get_device_id(unsigned char * buf, unsigned int len);
static int spi_nand_wel(void);

static int spiRead( struct spi_transfer *xfer );
static int spiWrite( unsigned char *msg_buf, int nbytes );
static void spi_nand_read_cfg(void);

static int spi_nand_device_reset(void);
static int spi_nand_status(void);
static int spi_nand_ready(void);
static int spi_nand_ecc(void);
static int spi_nand_sector_erase_int(unsigned long addr);

static int spi_nand_get_feat(unsigned char feat_addr);
static void spi_nand_set_feat(unsigned char feat_addr, unsigned char feat_val);

static void bcm63xx_cmd(struct mtd_info *mtd, unsigned int command, int column, int page);
static unsigned char bcm63xx_read_byte(struct mtd_info *mtd);
static void bcm63xx_read_buf(struct mtd_info *mtd, uint8_t *buf, int len);
static void bcm63xx_write(struct mtd_info *mtd, const uint8_t *buf, int len);
static int bcm63xx_status(struct mtd_info *mtd, struct nand_chip *chip);
static int bcm63xx_block_isbad(struct mtd_info *mtd, loff_t ofs, int getchip);
static int bcm63xx_block_markbad(struct mtd_info *mtd, loff_t ofs);
static void bcm63xx_select(struct mtd_info *mtd, int chip);
static int bcm63xx_scan_bbt(struct mtd_info *mtd);


static int spiRead(struct spi_transfer *xfer)
{
    if (FALSE == SpiNandDeviceRegistered)
    {
        printk("ERROR!! SPI NAND read without SPI NAND Linux device registration\n");
        return(0);
    }

    {
        struct spi_message  message;

        spi_message_init(&message);
        spi_message_add_tail(xfer, &message);

        /* the controller does not support asynchronous transfer,
           when spi_async returns the transfer will be complete
           don't use spi_sync (to avoid the call to schedule),
           scheduling will conflict with atomic operations
           such as writing image from Linux */
        return(spi_async(pSpiDevice, &message));
    }
}


static int spiWrite(unsigned char *msg_buf, int nbytes)
{
    if (FALSE == SpiNandDeviceRegistered)
    {
        printk("ERROR!! SPI NAND write without SPI NAND Linux device registration\n");
        return(0);
    }

    {
        struct spi_message  message;
        struct spi_transfer xfer;

        spi_message_init(&message);
        memset(&xfer, 0, (sizeof xfer));
        xfer.prepend_cnt = 0;
        xfer.len         = nbytes;
        xfer.speed_hz    = pSpiDevice->max_speed_hz;
        xfer.rx_buf      = NULL;
        xfer.tx_buf      = msg_buf;

        spi_message_add_tail(&xfer, &message);

        /* the controller does not support asynchronous transfer
           when spi_async returns the transfer will be complete
           don't use spi_sync to avoid the call to schedule */
        return(spi_async(pSpiDevice, &message));
    }
}

static void spi_nand_read_cfg(void)
{ // search through SPI NAND devices to find match
    unsigned char buf[SPI_NAND_ID_LENGTH];
    int i = 0;

    spi_nand_get_device_id(buf, SPI_NAND_ID_LENGTH);

    do
    {
        if (!memcmp(SpiDevInfo[i].chip_device_id, buf, SPI_NAND_ID_LENGTH))
            break;
        i++;
    } while(memcmp(SpiDevInfo[i].chip_name, "default", 7));

    pchip = &SpiDevInfo[i];
    if (!spin_is_locked(&chip_lock)) // show status only if initial reset since Linux NAND code resets chip during every block erase
        printk("Found SPI NAND device %s\n", pchip->chip_name);
}

/***********************************************************************/
/* reset SPI NAND device and get configuration information             */
/* some devices such as Micron MT29F1G01 require explicit reset before */
/* access to the device.                                               */
/***********************************************************************/
static int spi_nand_device_reset(void)
{
    unsigned char buf[4];
#if defined(CONFIG_BRCM_IKOS)
    unsigned int i;
    for( i = 0; i < 250; i++);
#else
    udelay(300);
#endif
    if (!spin_is_locked(&chip_lock)) // show status only if initial reset since Linux NAND code resets chip during every block erase
        printk("SPI NAND device reset\n");
    buf[0]        = FLASH_RESET;
    spiWrite(buf, 1);

#if defined(CONFIG_BRCM_IKOS)
    for( i = 0; i < 3000; i++);
#else
    /* device is availabe after 5ms */
    udelay(5000);
#endif
    while(!spi_nand_ready()); // do we need this here??

    spi_nand_set_feat(FEATURE_PROT_ADDR, FEAT_DISABLE); // disable block locking

    spi_nand_read_cfg();

    return(FLASH_API_OK);
}

/*********************************************************************/
/*  row address is 24 bit length. so buf must be at least 3 bytes. */
/*  For gigadevcie GD5F1GQ4 part(2K page size, 64 page per block and 1024 blocks) */
/*  Row Address. RA<5:0> selects a page inside a block, and RA<15:6> selects a block and */
/*  first byte is dummy byte */
/*********************************************************************/
static void spi_nand_row_addr(unsigned int page_addr, unsigned char* buf)
{
    buf[0] = (unsigned char)(page_addr>>(pchip->chip_page_shift+16)); //dummy byte
    buf[1] = (unsigned char)(page_addr>>(pchip->chip_page_shift+8));
    buf[2] = (unsigned char)(page_addr>>(pchip->chip_page_shift));

    return;
}

/*********************************************************************/
/*  column address select the offset within the page. For gigadevcie GD5F1GQ4 part(2K page size and 2112 with spare) */
/*  is 12 bit length. so buf must be at least 2 bytes. The 12 bit address is capable of address from 0 to 4095 bytes */
/*  however only byte 0 to 2111 are valid. */
/*********************************************************************/

static void spi_nand_col_addr(unsigned int page_addr, unsigned int page_offset, unsigned char* buf)
{
    page_offset = page_offset&((1<<(pchip->chip_page_shift+1))-1);  /* page size + spare area size */
    /* the upper 4 bits of buf[0] is either wrap bits for gigadevice or dummy bit[3:1] + plane select bit[0] for micron
     */
    if(*pchip->chip_device_id == MICRONPART)
    {
        /* setup plane bit if more than one plane. otherwise that bit is always 0 */
        if( pchip->chip_num_plane > 1 )
            buf[0] = (unsigned char)(((page_offset>>8)&0xf)|((page_addr>>pchip->chip_block_shift)&0x1)<<4); //plane bit is the first bit of the block number RowAddr[6]
        else
            buf[0] = (unsigned char)((page_offset>>8)&0xf);
    }
    else
    {
        /* use default wrap option 0, wrap length 2112 */
        buf[0] = (unsigned char)((page_offset>>8)&0xf);
    }
    buf[1] = (unsigned char)(page_offset);

    return;
}

/***************************************************************************
 * Function Name: spi_nand_read_page
 * Description  : Reads a NAND page into the specified buffer.
 * Returns      : FLASH_API_OK or FLASH_API_ERROR
 ***************************************************************************/
static int spi_nand_read_page(unsigned long page_addr, unsigned int page_offset, unsigned char *buffer, int len)
{
    unsigned char buf[spi_max_op_len];
    int status = FLASH_API_OK;
    int maxread;
    struct spi_transfer xfer;

    if (((page_offset + len) > (pchip->chip_page_size + pchip->chip_spare_size)) && // check to see if reading withing page/OOB boundary
            (page_offset != 0x840) && (len != 0x40)) // check to allow reading of Gigadevice hidden spare area
    {
        printk("spi_nand_read_page(): Attempt to read past page boundary, offset 0x%x, length 0x%x, into page address 0x%x\n", page_offset, len, (unsigned int)page_addr);

        return (FLASH_API_ERROR);
    }

    spi_nand_set_feat(FEATURE_FEAT_ADDR, FEAT_ECC_EN); // reading from page, enable ECC, turn on ECC anyway even if there's a failure should still fill buffer

    /* The PAGE READ (13h) command transfers the data from the NAND Flash array to the
     * cache register.  The PAGE READ command requires a 24-bit address consisting of
     * 8 dummy bits followed by a 16-bit block/page address.
     */
    buf[0] = FLASH_PREAD;
    spi_nand_row_addr(page_addr, buf+1);
    DBG_PRINTF("spi_nand_read_page - spi cmd 0x%x, 0x%x, 0x%x, 0x%x\n", buf[0], buf[1], buf[2], buf[3]);
    spiWrite(buf, 4);

    /* GET FEATURES (0Fh)  command to read the status */
    while(!spi_nand_ready());

    if (page_offset < pchip->chip_page_size)
        status = spi_nand_ecc();

    if (!len)
        return(status);

    while (len > 0)
    { // break up NAND buffer read into SPI buffer sized chunks
       /* Random data read (0Bh or 03h) command to read the page data from the cache
          The RANDOM DATA READ command requires 4 dummy bits, followed by a 12-bit column
          address for the starting byte address and a dummy byte for waiting data.
          This is only for 2K page size, the format will change for other page size.
       */

        maxread = (len > spi_max_op_len) ? spi_max_op_len : len;
        buf[0] = FLASH_READ;
        spi_nand_col_addr(page_addr, page_offset, buf+1);
        buf[3] = 0; //dummy byte

        DBG_PRINTF("spi_nand_read_page - spi cmd 0x%x, 0x%x, 0x%x, 0x%x\n", buf[0],buf[1],buf[2],buf[3]);
        DBG_PRINTF("spi_nand_read_page - spi read len 0x%x, offset 0x%x, remaining 0x%x\n", maxread, offset, nbytes-maxread);

        memset(&xfer, 0, sizeof(struct spi_transfer));
        xfer.tx_buf      = buf;
        xfer.rx_buf      = buffer;
        xfer.len         = maxread;
        xfer.speed_hz    = spi_flash_clock;
        xfer.prepend_cnt = 4;
        xfer.addr_len    = 3; // length of address field (max 4 bytes)
        xfer.addr_offset = 1; // offset of first addr byte in header
        xfer.hdr_len     = 4; // length of header
        xfer.unit_size   = 1; // data for each transfer will be divided into multiples of unit_size
        spiRead(&xfer);
        while (!spi_nand_ready());

        buffer += maxread;
        len -= maxread;
        page_offset += maxread;
    }

    /* check ecc from status bits if we are reading from page area */
    if (status == FLASH_API_ERROR)
    { // check to see if ECC is set to all FF's to verify as blank page (don't check page area as there may be bad bits which would not look like a blank page)
        spi_nand_set_feat(FEATURE_FEAT_ADDR, FEAT_DISABLE); // disable ECC

        buf[0] = FLASH_READ;
        spi_nand_col_addr(page_addr, pchip->chip_ecc_offset, buf+1);
        buf[3] = 0; //dummy byte

        memset(&xfer, 0, sizeof(struct spi_transfer));
        xfer.tx_buf      = buf;
        xfer.rx_buf      = buf;
        xfer.len         = 64;
        xfer.speed_hz    = spi_flash_clock;
        xfer.prepend_cnt = 4;
        xfer.addr_len    = 3;
        xfer.addr_offset = 1;
        xfer.hdr_len     = 4;
        xfer.unit_size   = 1;
        spiRead(&xfer);
        while (!spi_nand_ready());

        { // check to see if all ECC bytes are 0xFF to verify page is empty
            int i;

            for (i = 0; i < pchip->ecclayout->eccbytes; i++)
                if (buf[pchip->ecclayout->eccpos[i]] != 0xFF)
                {
                    printk("Uncorrectable ECC ERROR!! Address 0x%x, block 0x%x, page 0x%x is not empty!!\n", (unsigned int)page_addr, (unsigned int)page_addr>>pchip->chip_block_shift, (unsigned int)(page_addr%pchip->chip_block_size)>>pchip->chip_page_shift);
                    return(FLASH_API_ERROR);
                }

            memset(buffer, 0xFF, len); // fill buffer with 0xFF just in case there were any bad bits read

            return(FLASH_API_OK);
        }
    }

    return(status);
}

/*********************************************************************/
/* Flash_status return the feature status byte                       */
/*********************************************************************/
static int spi_nand_status(void)
{
    return spi_nand_get_feat(FEATURE_STAT_ADDR);
}

/* check device ready bit */
static int spi_nand_ready(void)
{
  return (spi_nand_status()&STAT_OIP) ? 0 : 1;
}

/*********************************************************************/
/*  spi_nand_get_feat return the feature byte at feat_addr            */
/*********************************************************************/
static int spi_nand_get_feat(unsigned char feat_addr)
{
    unsigned char buf[4];
    struct spi_transfer xfer;

    /* check device is ready */
    memset(&xfer, 0, sizeof(struct spi_transfer));
    buf[0]           = FLASH_GFEAT;
    buf[1]           = feat_addr;
    xfer.tx_buf      = buf;
    xfer.rx_buf      = buf;
    xfer.len         = 1;
    xfer.speed_hz    = spi_flash_clock;
    xfer.prepend_cnt = 2;
    spiRead(&xfer);

    DBG_PRINTF("spi_nand_get_feat at 0x%x 0x%x\n", feat_addr, buf[0]);

    return buf[0];
}

/*********************************************************************/
/*  spi_nand_set_feat set the feature byte at feat_addr              */
/*********************************************************************/
static void spi_nand_set_feat(unsigned char feat_addr, unsigned char feat_val)
{
    unsigned char buf[3];

    /* check device is ready */
    buf[0]           = FLASH_SFEAT;
    buf[1]           = feat_addr;
    buf[2]           = feat_val;
    spiWrite(buf, 3);

    while(!spi_nand_ready());

    return;
}

static int spi_nand_ecc(void)
{
    int status;

    status = spi_nand_status();
    status = (status & STAT_ECC_MASK) >> STAT_ECC_SHIFT;
    if( status == STAT_ECC_GOOD )
        return(FLASH_API_OK);

    if( status == STAT_ECC_UNCORR ) // uncorrectable ECC error, however page may be empty, next layer up checks for this
        return(FLASH_API_ERROR);

    return(FLASH_API_CORR); // correctable errors
}

/*********************************************************************/
/* Flash_sector__int() wait until the erase is completed before      */
/* returning control to the calling function.  This can be used in   */
/* cases which require the program to hold until a sector is erased, */
/* without adding the wait check external to this function.          */
/*********************************************************************/
static int spi_nand_sector_erase_int(unsigned long addr)
{
    unsigned char buf[11];
    int status;
    unsigned char buffer[pchip->chip_spare_size];
    int dirty = 0;

    addr &= ~(pchip->chip_block_size - 1);

    DBG_PRINTF("spi_nand_sector_erase_int block at address 0x%lx\n", addr);

    if (spi_nand_is_blk_bad(addr))
    {
        printk("spi_nand_sector_erase_int(): Attempt to erase failed due to bad block 0x%lx (address 0x%lx)\n", addr >> pchip->chip_block_shift, addr);
        return (FLASH_API_ERROR);
    }

    { // check if block is empty, may not have to erase
        loff_t page_addr;
        int i;

        for (page_addr = addr; !dirty && (page_addr < (addr + pchip->chip_block_size)); page_addr += pchip->chip_page_size)
        { // check to see that ECC is empty to determine if page is erased
            if (spi_nand_read_page(page_addr, pchip->chip_ecc_offset, buffer, pchip->chip_spare_size) == FLASH_API_ERROR)
                dirty = 1;

            for (i = 0; !dirty && (i < pchip->chip_spare_size); i++)
                if (buffer[i] != 0xFF)
                    dirty = 1;
        }
    }

    if (dirty)
    { // erase dirty block
        spi_nand_write_enable();
        buf[0] = FLASH_BERASE;
        spi_nand_row_addr(addr, buf+1);
        spiWrite(buf, 4);
        while(!spi_nand_ready()) ;

        status = spi_nand_status();
        if( status & STAT_EFAIL )
        {
            printk("spi_nand_sector_erase_int(): Erase block 0x%lx failed, sts 0x%x\n",  addr >> pchip->chip_block_shift, status);
            return(FLASH_API_ERROR);
        }

        spi_nand_write_disable();
    }

    return (FLASH_API_OK);
}

/**********************************************************************/
/* flash_write_enable() must be called before any change to the       */
/* device such as write, erase. It also unlock the blocks if they are */
/* previouly locked.                                                  */
/**********************************************************************/
static int spi_nand_write_enable(void)
{
    unsigned char buf[4], prot;

    /* make sure it is not locked first */
    prot = spi_nand_get_feat(FEATURE_PROT_ADDR);
    if( prot != 0 )
    {
        prot = 0;
        spi_nand_set_feat(FEATURE_PROT_ADDR, prot);
    }

    /* send write enable cmd and check feature status WEL latch bit */
    buf[0] = FLASH_WREN;
    spiWrite(buf, 1);
    while(!spi_nand_ready());
    while(!spi_nand_wel());

    return(FLASH_API_OK);
}

static int spi_nand_write_disable(void)
{
    unsigned char buf[4];

    buf[0] = FLASH_WRDI;
    spiWrite(buf, 1);
    while(!spi_nand_ready());
    while(spi_nand_wel());

    return(FLASH_API_OK);
}

/***************************************************************************
 * Function Name: spi_nand_write_page
 * Description  : Writes up to a NAND block of pages from the specified buffer.
 * Returns      : FLASH_API_OK or FLASH_API_ERROR
 ***************************************************************************/
static int spi_nand_write_page(unsigned long page_addr, unsigned int page_offset, unsigned char *buffer, int len)
{
    unsigned char spi_buf[512];  /* HS_SPI_BUFFER_LEN SPI controller fifo size is currently 512 bytes*/
    unsigned char xfer_buf[pchip->chip_page_size + pchip->chip_spare_size];
    int maxwrite, status;
    int verify;

    if (!len)
    {
        printk("spi_nand_write_page(): Not writing any data to page addr 0x%x, page_offset 0x%x, len 0x%x\n", (unsigned int)page_addr, page_offset, len);
        return (FLASH_API_OK);
    }

    if ((page_offset + len) > (pchip->chip_page_size + pchip->chip_spare_size))
    {
        printk("spi_nand_write_page(): Attempt to write past page boundary, offset 0x%x, length 0x%x, into page address 0x%x\n", page_offset, len, (unsigned int)page_addr);
        return (FLASH_API_ERROR);
    }

    { // check if write is blank
        unsigned int i;

        DBG_PRINTF("spi_nand_write_page - page addr 0x%x, offset 0x%x, size 0x%x, len 0x%x\n", (unsigned int)page_addr, offset, size, len);

        status = FLASH_API_OK_BLANK;
        for( i = 0; i < len; i++ )
        { // do this on a byte basis because buffer may not be aligned/whole page
            if( *(buffer+i) != 0xff )
            {
                status = FLASH_API_OK;
                break;
            }
        }
    }

    if (status == FLASH_API_OK_BLANK)
        return(FLASH_API_OK); // don't write to page if data is all FF's

    if (page_offset < pchip->chip_page_size)
    { // writing into page area, read page first to fill write buffer, must do this because we don't know if JFFS2 clean marker is there or not and this is included in ECC calculation
        spi_nand_set_feat(FEATURE_FEAT_ADDR, FEAT_ECC_EN); // enable ECC if writing to page
        verify = 1;
    }
    else
    { // not writing into page area
        if ((page_offset != pchip->chip_page_size) || (len != 1))
            return(FLASH_API_OK); // only other allowed write is the bad block marker; return if not that

        spi_nand_set_feat(FEATURE_FEAT_ADDR, FEAT_DISABLE); // else don't write ECC
        verify = 0;
    }

    memset(xfer_buf, 0xff, sizeof(xfer_buf));
    memcpy(xfer_buf + page_offset, buffer, len);
    len = pchip->chip_page_size + pchip->chip_spare_size;
    page_offset = 0;

    while (len > 0)
    {
        /* Send Program Load Random Data Command (0x84) to load data to cache register.
         * PROGRAM LOAD consists of an 8-bit Op code, followed by 4 bit dummy and a
         * 12-bit column address, then the data bytes to be programmed. */
        spi_buf[0] = FLASH_PROG_RAN;
        spi_nand_col_addr(page_addr, page_offset, spi_buf + 1);

        maxwrite = (len > (spi_max_op_len - 5)) ? (spi_max_op_len - 5) : len;

        memcpy(&spi_buf[3], xfer_buf + page_offset, maxwrite);
        DBG_PRINTF("spi_nand_write_page - spi cmd 0x%x, 0x%x, 0x%x\n", spi_buf[0], spi_buf[1], spi_buf[2]);
        DBG_PRINTF("spi_nand_write_page - spi write len 0x%x, offset 0x%x, remaining 0x%x\n", maxwrite, offset, len-maxwrite);

        spiWrite(spi_buf, maxwrite + 3);

        len -= maxwrite;
        page_offset += maxwrite;

        while(!spi_nand_ready()); // do we need this here??
    }

    /* Send Program Execute command (0x10) to write cache data to memory array
     * Send address (24bit): 8 bit dummy + 16 bit address (page/Block)
     */
    /* Send Write enable Command (0x06) */
    spi_nand_write_enable();

    spi_buf[0] = FLASH_PEXEC;
    spi_nand_row_addr(page_addr, spi_buf + 1);
    DBG_PRINTF("spi_nand_write_page - spi cmd 0x%x, 0x%x, 0x%x, 0x%x\n", spi_buf[0], spi_buf[1], spi_buf[2], spi_buf[3]);
    spiWrite(spi_buf, 4);
    while(!spi_nand_ready());

    status = spi_nand_status();
    spi_nand_write_disable();

    if(status & STAT_PFAIL)
    {
        printk("Page program failed at address 0x%x, sts 0x%x\n", (unsigned int)page_addr, status);
        return(FLASH_API_ERROR);
    }

    if (verify)
    {
        unsigned char buf[pchip->chip_page_size];

        status = spi_nand_read_page(page_addr, 0, buf, pchip->chip_page_size);

        if (status == FLASH_API_ERROR)
        {
            printk("Write verify failed reading back page at address 0x%lx\n", page_addr);
            return(FLASH_API_ERROR);
        }

        if (memcmp(xfer_buf, buf, pchip->chip_page_size))
        {
            printk("Write data did not match read data at address 0x%lx\n", page_addr);
            return(FLASH_API_ERROR);
        }

        if (status == FLASH_API_CORR)
        {
            printk("Write verify correctable errors at address 0x%lx\n", page_addr);
            return(FLASH_API_CORR);
        }
    }

    return (FLASH_API_OK);
}

/* check device write enable latch bit */
static int spi_nand_wel(void)
{
  return (spi_nand_status() & STAT_WEL) ? 1 : 0;
}

/*********************************************************************/
/* flash_get_device_id() return the device id of the component.      */
/*********************************************************************/

static void spi_nand_get_device_id(unsigned char * buf, unsigned int len)
{
    unsigned char buffer[2];
    struct spi_transfer xfer;

    memset(&xfer, 0, sizeof(struct spi_transfer));
    buffer[0]        = FLASH_RDID;
    buffer[1]        = 0;
    xfer.tx_buf      = buffer;
    xfer.rx_buf      = buf;
    xfer.len         = len;
    xfer.speed_hz    = spi_flash_clock;
    xfer.prepend_cnt = 2;
    spiRead(&xfer);
    while(!spi_nand_ready());

    DBG_PRINTF("spi_nand_get_device_id 0x%x 0x%x\n", buf[0], buf[1]);
}

static int spi_nand_is_blk_bad(unsigned long addr)
{
    unsigned char buf;

    if (addr < pchip->chip_block_size)
        return 0; // always return good for block 0, because if it's a bad chip quite possibly the board is useless

    addr &= ~(pchip->chip_block_size - 1);

    spi_nand_read_page(addr, pchip->chip_page_size, &buf, 1);

    if (0xFF != buf)
    {
        printk("Bad Block 0x%lx found (address 0x%lx)\n", addr >> pchip->chip_block_shift, addr);
        return(1);
    }

    return(0);
}

static int spi_nand_mark_blk_bad(unsigned long addr)
{
    int ret;

    addr &= ~(pchip->chip_block_size - 1);

    printk("Marking block 0x%lx bad (address 0x%lx)\n", addr >> pchip->chip_block_shift, addr);

    ret = spi_nand_write_page(addr, pchip->chip_page_size, "\0", 1);

    if (ret != FLASH_API_OK)
        printk("Unable to mark block 0x%lx bad\n", addr >> pchip->chip_block_shift);

    return(ret);
}

static void bcm63xx_cmd(struct mtd_info *mtd, unsigned int command, int column, int page)
{
    unsigned long addr = page * mtd->writesize;

    spin_lock(&chip_lock);

    switch(command)
    {
        case NAND_CMD_READ0:
        case NAND_CMD_READ1: // step 1/2 for read, execute SPI NAND read command and transfer SPI NAND data to local buffer

            status = STATUS_DEFAULT;

            if (addr > mtd->size)
            {
                printk("SPI NAND ERROR!! Trying to read past end of chip\n");
                status |= NAND_STATUS_FAIL;
            }
            else
            {
                if (FLASH_API_ERROR == spi_nand_read_page(page * mtd->writesize, column, pageBuf, mtd->writesize + mtd->oobsize))
                {
                    printk("SPI NAND ERROR Reading page!!\n");
                    status |= NAND_STATUS_FAIL;
                }

                pageBufI = 0;
            }
            break;

        case NAND_CMD_READOOB: // step 1/2 for read, execute SPI NAND read command and transfer SPI NAND data to local buffer
            status = STATUS_DEFAULT;

            if (addr > mtd->size)
            {
                printk("SPI NAND ERROR!! Trying to read past end of chip\n");
                status |= NAND_STATUS_FAIL;
            }
            else
            {
                if (FLASH_API_ERROR == spi_nand_read_page(page * mtd->writesize, mtd->writesize, pageBuf + mtd->writesize, mtd->oobsize))
                {
                    printk("SPI NAND ERROR Reading page OOB!!\n");
                    status |= NAND_STATUS_FAIL;
                }
                pageBufI = mtd->writesize;
            }
            break;

        case NAND_CMD_RESET:
            status = STATUS_DEFAULT;

            if (FLASH_API_ERROR == spi_nand_device_reset())
            {
                printk("ERROR resetting SPI NAND device!!\n");
                status |= NAND_STATUS_FAIL;
            }
            break;

        case NAND_CMD_READID:
            status = STATUS_DEFAULT;

            spi_nand_get_device_id(pageBuf, NAND_ID_LENGTH);

            if (*pageBuf == GIGADEVPART)
                *(pageBuf+2) = 0x80; *(pageBuf+3) = 0x1D; // provide the rest of the ID bytes that Gigadevice omits

            pageBufI = 0;
            break;

        case NAND_CMD_STATUS: // NAND infrastructure only uses this to determine if write protect is set
            *(pageBuf + mtd->writesize + mtd->oobsize - 1) = status;
            pageBufI = mtd->writesize + mtd->oobsize - 1; // set pointer to end of buffer so we have a limit to the amount of data read
            break;

        case NAND_CMD_SEQIN: // step 1/3 for write, capture address
            status = STATUS_DEFAULT;

            if (addr > mtd->size)
            {
                printk("ERROR!! Trying to program past end of chip\n");
                status |= NAND_STATUS_FAIL;
            }
            else
            {
                pageAddr = addr;
                pageOffset = column;
                pageBufI = 0;
            }
            break;

        case NAND_CMD_PAGEPROG: // step 3/3 for write, transfer local buffer to SPI NAND device and execute SPI NAND write command
        {
            int error = 0;

            addr = pageAddr & ~(mtd->erasesize - 1); // block address

            if ((status = spi_nand_write_page(pageAddr, pageOffset, pageBuf, pageBufI)) == FLASH_API_ERROR)
                error = 1;

            if (!error && (status == FLASH_API_CORR) && (pageAddr >= mtd->erasesize))
            { // read/erase/write block to see if we can get rid of the bit errors, but only if not block zero
                int offset;
                unsigned char * buffer;

                printk("Correctible errors, SPI NAND Rewriting block\n");

                buffer = kmalloc(mtd->erasesize, GFP_ATOMIC);
                if (!buffer)
                { // unfortunately can't attempt to fix block in this case
                    printk("Error allocating buffer!!\n");
                    error = 1;
                }

                // read block
                for (offset = 0; !error && (offset < mtd->erasesize); offset += mtd->writesize)
                {
                    status = spi_nand_read_page(addr + offset, 0, buffer + offset, mtd->writesize);
                    if (status == FLASH_API_ERROR)
                        error = 1;
                }

                // erase block
                if (!error)
                {
                    status = spi_nand_sector_erase_int(addr);
                    if (status == FLASH_API_ERROR)
                        error = 1;
                }

                // write block
                if (!error)
                {
                    for (offset = 0; offset < mtd->erasesize; offset += mtd->writesize)
                    {
                        status = spi_nand_write_page(addr + offset, 0, buffer + offset, mtd->writesize);
                        if (status != FLASH_API_OK)
                            error = 1; // essentially failed, but finish writing out all the data anyway to hopefully be recovered later
                    }
                }

                if(buffer)
                    kfree(buffer);
            }

            status = STATUS_DEFAULT;

            if (error)
            {
                printk("SPI NAND ERROR Writing page!!\n");
                status |= NAND_STATUS_FAIL;
                spi_nand_mark_blk_bad(addr); // JFFS2 will do this automatically
            }

            break;
        }

        case NAND_CMD_ERASE1:
            status = STATUS_DEFAULT;

            if (addr >= mtd->size)
            {
                printk("ERROR!! Trying to erase past end of chip\n");
                status |= NAND_STATUS_FAIL;
            }
            else if (FLASH_API_ERROR == spi_nand_sector_erase_int(addr))
            {
                printk("SPI NAND ERROR Erasing block!!\n");
                status |= NAND_STATUS_FAIL;
            }
        case NAND_CMD_ERASE2:
            break;

            default:
                printk("ERROR!! Unkonwn NAND command 0x%x\n", command);
                status |= NAND_STATUS_FAIL;
        }

    spin_unlock(&chip_lock);
}

static unsigned char bcm63xx_read_byte(struct mtd_info *mtd)
{
    unsigned char ret;

    spin_lock(&chip_lock);

    ret = *(pageBuf + pageBufI++);

    spin_unlock(&chip_lock);

    return(ret);
}

static void bcm63xx_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{ // step 2/2 for read, read data from local buffer
    spin_lock(&chip_lock);

    if ((pageBufI + len) > (mtd->writesize + mtd->oobsize))
        printk("ERROR!! Trying to read past end of buffer\n");
    else
    {
        memcpy(buf, pageBuf+pageBufI, len);
        pageBufI += len;
    }

    spin_unlock(&chip_lock);
}

// step 2/3 for write, fill local buffer
static void bcm63xx_write(struct mtd_info *mtd, const uint8_t *buf, int len)
{ // write to buffer
    spin_lock(&chip_lock);

    if ((pageBufI + len) > (mtd->writesize + mtd->oobsize))
        printk("ERROR!! Trying to write past end of buffer\n");
    else
    {
        memcpy(pageBuf+pageBufI, buf, len);
        pageBufI += len;
    }

    spin_unlock(&chip_lock);
}

static int bcm63xx_status(struct mtd_info *mtd, struct nand_chip *chip)
{ // NAND infrastructure used this to not only determine when a command has finished (spinlocks will take care of that)
    // but also to return the status

    spin_lock(&chip_lock);

    spin_unlock(&chip_lock);

    return(status);
}

static int bcm63xx_block_isbad(struct mtd_info *mtd, loff_t ofs, int getchip)
{
    int ret;

    spin_lock(&chip_lock);

    ret = spi_nand_is_blk_bad(ofs);

    spin_unlock(&chip_lock);

    return(ret);
}

static int bcm63xx_block_markbad(struct mtd_info *mtd, loff_t ofs)
{
    int ret;

    spin_lock(&chip_lock);

    ret = spi_nand_mark_blk_bad(ofs);

    spin_unlock(&chip_lock);

    return(ret);
}

static void bcm63xx_select(struct mtd_info *mtd, int chip)
{ // dummy function, chip is always selected as far as the NAND infrastructure is concerned
}

static int bcm63xx_scan_bbt(struct mtd_info *mtd)
{ // dummy function
    return(1); // this will ultimately be the return value for nand_scan
}

static struct spi_board_info bcmSpiDevInfo =
{
    .modalias      = "bcm_SpiDev",
    .chip_select   = 0,
    .max_speed_hz  = 781000,
    .bus_num       = LEG_SPI_BUS_NUM,
    .mode          = SPI_MODE_3,
};

static struct spi_driver bcmSpiDevDrv =
{
    .driver =
        {
        .name     = "bcm_SpiDev",
        .bus      = &spi_bus_type,
        .owner    = THIS_MODULE,
        },
};


void bcmspinand_probe(struct mtd_info * mtd)
{
    struct nand_chip * nand = mtd->priv;
    struct spi_master * pSpiMaster;
    int spiCtrlState;

    printk("SPI NAND Device Linux Registration\n");

    /* micron MT29F1G01 only support up to 50MHz, update to 50Mhz if it is more than that */
#if defined(CONFIG_BCM96318)
    spi_flash_busnum = HS_SPI_BUS_NUM;
    spi_flash_clock = 50000000;

#elif defined(CONFIG_BCM96838)
    spi_flash_busnum = HS_SPI_BUS_NUM;
    spi_flash_clock = 20000000; // reset value

#elif defined(CONFIG_BCM963381)
    spi_flash_busnum = HS_SPI_BUS_NUM;
    //FIXME choose the right clock
    spi_flash_clock = 20000000; // reset value
#endif

    /* retrieve the maximum read/write transaction length from the SPI controller */
    spi_max_op_len = SPI_BUF_LEN;

    /* set the controller state, spi_mode_0 */
    spiCtrlState = SPI_CONTROLLER_STATE_DEFAULT;

    if ( spi_flash_clock > SPI_CONTROLLER_MAX_SYNC_CLOCK )
       spiCtrlState |= SPI_CONTROLLER_STATE_ASYNC_CLOCK;

    bcmSpiDevInfo.max_speed_hz    = spi_flash_clock;
    bcmSpiDevInfo.controller_data = (void *)spiCtrlState;
    bcmSpiDevInfo.mode            = SPI_MODE_DEFAULT;
    bcmSpiDevInfo.chip_select     = SPI_FLASH_SLAVE_DEV_ID;
    bcmSpiDevInfo.bus_num         = spi_flash_busnum;

    pSpiMaster = spi_busnum_to_master( spi_flash_busnum );
    pSpiDevice = spi_new_device(pSpiMaster, &bcmSpiDevInfo);

    /* register as SPI device */
    spi_register_driver(&bcmSpiDevDrv);

    SpiNandDeviceRegistered = TRUE;

    printk("SPI NAND Linux Registration\n");

    spin_lock_init(&chip_lock);

    spi_nand_device_reset(); // reset and set configuration information

    nand->ecc.size = pchip->chip_page_size;
    nand->ecc.bytes = 0;
    nand->ecc.strength = 0;
    nand->ecc.layout = pchip->ecclayout;
    nand->page_shift = pchip->chip_page_shift;
    nand->phys_erase_shift = pchip->chip_block_shift;
    nand->chipsize = pchip->chip_total_size;

    pageBuf = kmalloc(pchip->chip_page_size + pchip->chip_spare_size, GFP_KERNEL);

    nand->options = NAND_NO_AUTOINCR | NAND_NO_READRDY | NAND_NO_SUBPAGE_WRITE;

    nand->chip_delay = 0;
    nand->read_byte = bcm63xx_read_byte;
    nand->read_buf = bcm63xx_read_buf;
    nand->ecc.mode = NAND_ECC_NONE;

    nand->select_chip = bcm63xx_select;
    nand->write_buf  = bcm63xx_write;
    nand->scan_bbt = bcm63xx_scan_bbt;
    nand->block_bad = bcm63xx_block_isbad;
    nand->block_markbad = bcm63xx_block_markbad;
    nand->cmdfunc = bcm63xx_cmd;
    nand->waitfunc = bcm63xx_status;
}

#endif //CONFIG_BCM_KF_MTD_BCMNAND
