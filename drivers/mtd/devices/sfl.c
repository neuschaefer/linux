/*
 *
 * sfl.c
 * Description:
 * serial flash mtd driver. This driver currently handles all flash devices.
 * TODO :
 *   1. It is difficult to provide a common protection layer for all serial
 *      flashes and hence driver currently does not support locking/unlocking
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
 * Change Log:
 * May-2007     -   srinath                     : First version for spansion
 * Oct-2007     -   mansoor.ahamed@ti.com       : Spansion SFI cleanup
 * Nov-2007     -   mansoor.ahamed@ti.com       : Multi manufacturer support
 * Apr-2008     -   mansoor.ahamed@ti.com       : Cleanup for multi flash probe
 */
/*
Includes Intel Corporation's changes/modifications dated: 2014.
Changed/modified portions - Copyright © 2014, Intel Corporation.
*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/interrupt.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>
#include <linux/spi/avalanche_spi.h>
#include <linux/semaphore.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <asm/signal.h>
#include <mach/semaphore.h>

#include "../mtdcore.h"

#if defined (CONFIG_HW_MUTEXES)
#include <asm-arm/arch-avalanche/generic/hw_mutex_ctrl.h>
#endif

extern int
kill_proc(pid_t pid, int sig, int priv);

/********************** Debug helpers ******************/
#define __MODULE__ "sfl"

#if defined(CONFIG_SPI_DEBUG)
#define DEBUG_SFL(fmt,arg...)  printk(KERN_DEBUG "%s:%s():line %d : " fmt , __MODULE__, __FUNCTION__, __LINE__, ##arg);
#else
#define DEBUG_SFL(fmt,arg...) \
	do { } while (0)
#endif

#define PRINTK(fmt,arg...)  printk(KERN_EMERG "%s:%s():line %d : " fmt , __MODULE__, __FUNCTION__, __LINE__, ##arg);

#define ENTER() DEBUG_SFL("Enter\n");
#define EXIT()  DEBUG_SFL("Exit\n");



/****************** Module param defaults ***************/
/* 4 seconds max delay */
#define SFL_DELAY                       (msecs_to_jiffies(4000))
#define SFL_FAST_READ                   1 /* NOT same as Dual Read */
#define SFL_NORMAL_READ                 0
#define SFL_DUAL_FAST_READ              2
#define SFL_NUM_ADDR_BYTES              3
#define FAST_READ_NUM_DUMMY_BYTES       1
#define NORMAL_READ_NUM_DUMMY_BYTES     0
#define SFL_CMD_FRAME_SIZE_4            4
#define SFL_CMD_FRAME_SIZE_5            5

#ifdef CONFIG_SFL_EXTENDED_FLASH
#define MONOLITHIC_32MB_FLASH              /* Enable extended flash support (32MB monolithic flashes) */
#endif
/*****************************************************
*   Status Register bits.
*****************************************************/
#define SR_WIP                          0x01    /* Write in progress */
#define SR_WEL                          0x02    /* Write enable latch */
#define SR_BP0                          0x04    /* Block protect 0 */
#define SR_BP1                          0x08    /* Block protect 1 */
#define SR_BP2                          0x10    /* Block protect 2 */
#define SR_BP3                          0x20    /* Block protect 2 */
#define SR_SRWD                         0x80    /* SR write Disable */

struct sfl {
    unsigned char       command[6];
    struct spi_device   *spi;
    struct semaphore    lock;
    struct mtd_info     mtd;
    unsigned int index;
};

/* serial flash command table */
struct sflash_cmd_table {
    unsigned char r; /* read */
    unsigned char fr; /* fast read */
    unsigned char dfr; /* dual fast read */
    unsigned char rdid; /* read device id */
    unsigned char wren; /* write enable */
    unsigned char wrdi; /* write disable */
    unsigned char se; /* sector erase */
    unsigned char be; /* block erase */
    unsigned char ce; /* chip erase */
    unsigned char pp; /* page program */
    unsigned char cp; /* continuously program whole chip */
    unsigned char rdsr; /* read status reg */
    unsigned char wrsr; /* write status reg */
    unsigned char dp; /* deep power down */
    unsigned char rd; /* release deep power down */
    unsigned char res; /* read electronic device id */
};
/* default serial flash command table */
static const struct sflash_cmd_table def_cmd_table =
{
    .r  = 0x03,
    .fr = 0x0b,
    .dfr = 0x0b,    /* No dual read support */
    .rdid   = 0x9f,
    .wren   = 0x06,
    .wrdi   = 0x04,
    .se = 0xd8,
    .be = 0xd8,
    .ce = 0xc7,
    .pp = 0x02,
    .rdsr   = 0x05,
    .wrsr   = 0x01,
    .dp = 0xb9,
    .rd = 0xab,
    .res    = 0xab
};

/* serial flash command tables */
/* macronix cmd set */
static const struct sflash_cmd_table mx25lxxxx_cmd_table =
{
    .r  = 0x03,
    .fr = 0x0b,
    .dfr = 0x0b, /* No dual read support */
    .rdid   = 0x9f,
    .wren   = 0x06,
    .wrdi   = 0x04,
    .se = 0x20,
    .be = 0xd8,
    .ce = 0xc7,
    .pp = 0x02,
    .rdsr   = 0x05,
    .wrsr   = 0x01,
    .dp = 0xb9,
    .rd = 0xab,
    .res    = 0xab
};


/* Winbond cmd set */
static const struct sflash_cmd_table w25qxxxx_cmd_table =
{
    .r  = 0x03,
    .fr = 0x0b,
    .dfr = 0x3b,
    .rdid   = 0x9f,
    .wren   = 0x06,
    .wrdi   = 0x04,
    .se = 0x20,
    .be = 0xd8,
    .ce = 0xc7,
    .pp = 0x02,
    .rdsr   = 0x05,
    .wrsr   = 0x01,
    .dp = 0xb9,
    .rd = 0xab,
    .res    = 0xab
};


/* Numonyx Q series cmd set */
static const struct sflash_cmd_table n25qxxxx_cmd_table =
{
    .r  = 0x03,
    .fr = 0x0b,
    .dfr = 0x3b,
    .rdid   = 0x9f,
    .wren   = 0x06,
    .wrdi   = 0x04,
    .se = 0xd8,     /* No use for top/bottom 4K sectors */
    .be = 0xd8,     /* Note: according to spec 'be' (Bulk Erase) is 0xC7, 0xD8 is 'se' (Sector Erase)  */
    .ce = 0xc7,     /* Note: 'ce' no such command use same as 'be' (Bulk Erase) 0xC7 */
    .pp = 0x02,
    .rdsr   = 0x05,
    .wrsr   = 0x01,
    .dp = 0xb9,     /* Note: no such command is the spec. */
    .rd = 0xab,     /* Note: no such command is the spec. */
    .res = 0xab     /* Note: no such command is the spec. */
};

/* serial flash meta data structure */
struct sfl_flash_info {
    char    *name;
    int     (*probe)(struct spi_device   *spi, int index);
    unsigned int    id;
    unsigned int    sector_size;
    unsigned int    n_sectors;
    int             page_size;
    unsigned int    lock_all;
    unsigned int    n_regions;
    unsigned int    prot_ratio;
    #define SFL_MAX_REGIONS (5)
    struct mtd_erase_region_info *regions;
    const struct sflash_cmd_table *cmd;
    unsigned int sectors_per_block;
};
#if defined (CONFIG_MACH_PUMA5)
/* A 64-byte length data type for SFI burst reads */
typedef struct
{
    char buffer[ 64 ];

} sfi_read_buf_t;
#else /* CONFIG_MACH_PUMA6  For Puma-6 SoC */
/* A 8-byte length data type for SFI burst reads
   This is the maximum size, that is allowed in Puma6 */
typedef struct
{
    char buffer[8];

} sfi_read_buf_t;
#endif

/* sector size in different flashes */
#define S25FLXXXX_SECT_SIZE     (64*1024)
#define S25FL128P00_SECT_SIZE   (256*1024)
#define S25FL128P01_SECT_SIZE   (64*1024)
#define M25FL128P_SECT_SIZE     (256*1024)
#define M25FL64P_SECT_SIZE      (64*1024)
#define MX25LXXXX_SECT_SIZE     (4*1024)
#define W25QXXXBV_SECT_SIZE     (4*1024)
#define N25Q128_SECT_SIZE       (64*1024)
#define S25VF064_SECT_SIZE      (4*1024)
#define N25Q256_SECT_SIZE       (64*1024)

/* sectors per block */
#define S25FLXXXX_SECT_PER_BLOCK    (1)
#define M25FLXXXX_SECT_PER_BLOCK    (1)
#define MX25LXXXX_SECT_PER_BLOCK    (16)
#define W25QXXXBV_SECT_PER_BLOCK    (16)
#define N25QXXXXX_SECT_PER_BLOCK    (1)
#define S25VFXXXX_SECT_PER_BLOCK    (16)

/* page size */
#define S25FLXXXX_PAGE_SIZE     (256)
#define M25FLXXXX_PAGE_SIZE     (256)
#define MX25LXXXX_PAGE_SIZE     (256)
#define W25QXXXBV_PAGE_SIZE     (256)
#define N25QXXXXX_PAGE_SIZE     (256)
#define S25VFXXXX_PAGE_SIZE     (256)


/* different Manufacturer ids */
#define VENDOR_SPANSION_ID          (0x01)
#define VENDOR_NUMONYX_ID           (0x20)
#define VENDOR_MACRONIX_ID          (0xC2)
#define VENDOR_WINBOND_ID           (0xEF)
#define VENDOR_SST_ID               (0xBF)

/* different device ids */
#define S25FL004A_ID            (0x0212)
#define S25FL008A_ID            (0x0213)
#define S25FL016A_ID            (0x0214)
#define S25FL032A_ID            (0x0215)
#define S25FL064A_ID            (0x0216)
#define S25FL128P_ID            (0x2018)
#define S25FL256S_ID            (0x0219)
#define M25FL128P_ID            (0x2018)
#define M25FL64P_ID             (0x2017)
#define MX25L1605_ID            (0x2015)
#define MX25L3205_ID            (0x2016)
#define MX25L6405_ID            (0x2017)
#define MX25L12805_ID           (0x2018)
#define W25Q64BV_ID             (0x4017)
#define W25Q128BV_ID            (0x4018)
#define N25Q128_ID              (0xBA18)
#define N25Q256_ID              (0xBA19)
#define S25VF064_ID             (0x254B)

/* sector numbers in different flashes */
#define N_S25FL004A_SECTORS     (  8)
#define N_S25FL008A_SECTORS     ( 16)
#define N_S25FL016A_SECTORS     ( 32)
#define N_S25FL032A_SECTORS     ( 64)
#define N_S25FL064A_SECTORS     (128)
#define N_S25FL256S_SECTORS     (512)
#define N_S25FL128P00_SECTORS   ( 64)
#define N_S25FL128P01_SECTORS   (256)
#define N_M25FL128P_SECTORS     ( 64)
#define N_M25FL64P_SECTORS      (128)
#define N_MX25L1605_SECTORS     (512)
#define N_MX25L3205_SECTORS     (1024)
#define N_MX25L6405_SECTORS     (2048)
#define N_MX25L12805_SECTORS    (4096)
#define N_W25Q64BV_SECTORS      (2048)
#define N_W25Q128BV_SECTORS     (4096)
#define N_N25Q128_SECTORS       ( 256)
#define N_N25Q256_SECTORS       ( 512)
#define N_S25VF064_SECTORS      (2048)

/* protection ratio start */
/* TODO : protection is not used currently */
#define S25FL004A_PROT_RATIO            (  8)
#define S25FL008A_PROT_RATIO            ( 16)
#define S25FL016A_PROT_RATIO            ( 32)
#define S25FL032A_PROT_RATIO            ( 64)
#define S25FL064A_PROT_RATIO            ( 64)
#define S25FL128P00_PROT_RATIO          ( 64)
#define S25FL128P01_PROT_RATIO          (128)
#define S25FL256S_PROT_RATIO            (0xFFFFFFFF)
#define M25FL128P_PROT_RATIO            (0xFFFFFFFF)
#define MX25LXXXX_PROT_RATIO            (0xFFFFFFFF)
#define W25QXXXBV_PROT_RATIO            (0xFFFFFFFF)
#define N25Q128_PROT_RATIO              (0xFFFFFFFF)
#define N25Q256_PROT_RATIO              (0xFFFFFFFF)
#define S25VF064_PROT_RATIO             (0xFFFFFFFF)

/* different flashes region info */
/* spansion flashes */
#if defined (CONFIG_MACH_PUMA5)
static struct mtd_erase_region_info s25fl004a_regions =
    {0, S25FLXXXX_SECT_SIZE, N_S25FL004A_SECTORS};

static struct mtd_erase_region_info s25fl008a_regions =
    {0, S25FLXXXX_SECT_SIZE, N_S25FL008A_SECTORS};

static struct mtd_erase_region_info s25fl016a_regions =
    {0, S25FLXXXX_SECT_SIZE, N_S25FL016A_SECTORS};

static struct mtd_erase_region_info s25fl032a_regions =
    {0, S25FLXXXX_SECT_SIZE, N_S25FL032A_SECTORS};

static struct mtd_erase_region_info s25fl064a_regions =
    {0, S25FLXXXX_SECT_SIZE, N_S25FL064A_SECTORS};

static struct mtd_erase_region_info s25fl128p_regions =
    {0, S25FLXXXX_SECT_SIZE, N_S25FL128P01_SECTORS};

static struct mtd_erase_region_info s25fl256s_regions =
    {0, S25FLXXXX_SECT_SIZE, N_S25FL256S_SECTORS};

/* numonyix flashes */
static struct mtd_erase_region_info m25fl128p_regions =
    {0, M25FL128P_SECT_SIZE, N_M25FL128P_SECTORS};

static struct mtd_erase_region_info m25fl64p_regions =
    {0, M25FL64P_SECT_SIZE, N_M25FL64P_SECTORS};

static struct mtd_erase_region_info n25q128_regions =
    {0, N25Q128_SECT_SIZE, N_N25Q128_SECTORS};

static struct mtd_erase_region_info n25q256a_regions =
    {0, N25Q256_SECT_SIZE, N_N25Q256_SECTORS};

/* macronix flashes */
static struct mtd_erase_region_info mx25l1605_regions =
    {0, MX25LXXXX_SECT_SIZE, N_MX25L1605_SECTORS};

static struct mtd_erase_region_info mx25l3205_regions =
    {0, MX25LXXXX_SECT_SIZE, N_MX25L3205_SECTORS};

static struct mtd_erase_region_info mx25l6405_regions =
    {0, MX25LXXXX_SECT_SIZE, N_MX25L6405_SECTORS};

static struct mtd_erase_region_info mx25l12805_regions =
    {0, MX25LXXXX_SECT_SIZE, N_MX25L12805_SECTORS};

/* winbond flashes */
static struct mtd_erase_region_info w25q64bv_regions =
    {0, W25QXXXBV_SECT_SIZE, N_W25Q64BV_SECTORS};

static struct mtd_erase_region_info w25q128bv_regions =
    {0, W25QXXXBV_SECT_SIZE, N_W25Q128BV_SECTORS};

/* sst flashes */
static struct mtd_erase_region_info s25vf064_regions =
    {0, S25VF064_SECT_SIZE, N_S25VF064_SECTORS};
#else /* CONFIG_MACH_PUMA6  For Puma-6 SoC */
static struct mtd_erase_region_info n25q256_regions =
    {0, N25Q256_SECT_SIZE, N_N25Q256_SECTORS};
#endif /* CONFIG_MACH_PUMA5*/


#if defined (CONFIG_MACH_PUMA5)
/* returns 1 on detection */
static int m25flxxxx_probe(struct spi_device *spi, int index);
static int s25fl128p_probe(struct spi_device *spi, int index);
static int s25fl0xxx_probe(struct spi_device *spi, int index);
static int mx25lxxxx_probe(struct spi_device *spi, int index);
static int w25qxxxx_probe(struct spi_device *spi, int index);
static int s25vfxxxx_probe(struct spi_device *spi, int index);
static int s25fl256s_probe(struct spi_device *spi, int index);
static int n25qxxxx_probe(struct spi_device *spi, int index);
#else /* CONFIG_MACH_PUMA6  For Puma-6 SoC */
static int n25qxxxx_probe(struct spi_device   *spi, int index);
#endif /* CONFIG_MACH_PUMA5 */

/* empty fields will be filled by probe func */
static struct sfl_flash_info sfl_data [] = {
#if defined (CONFIG_MACH_PUMA5)
    { "s25fl004a", s25fl0xxx_probe, S25FL004A_ID, S25FLXXXX_SECT_SIZE,
            N_S25FL004A_SECTORS, S25FLXXXX_PAGE_SIZE, 0x1C, 1,
            S25FL004A_PROT_RATIO, &s25fl004a_regions, &def_cmd_table, S25FLXXXX_SECT_PER_BLOCK },

    { "s25fl008a", s25fl0xxx_probe, S25FL008A_ID, S25FLXXXX_SECT_SIZE,
            N_S25FL008A_SECTORS, S25FLXXXX_PAGE_SIZE, 0x1C, 1,
            S25FL008A_PROT_RATIO, &s25fl008a_regions, &def_cmd_table, S25FLXXXX_SECT_PER_BLOCK },

    { "s25fl016a", s25fl0xxx_probe, S25FL016A_ID, S25FLXXXX_SECT_SIZE,
            N_S25FL016A_SECTORS, S25FLXXXX_PAGE_SIZE, 0x1C, 1,
            S25FL016A_PROT_RATIO, &s25fl016a_regions, &def_cmd_table, S25FLXXXX_SECT_PER_BLOCK },

    { "s25fl032a", s25fl0xxx_probe, S25FL032A_ID, S25FLXXXX_SECT_SIZE,
            N_S25FL032A_SECTORS, S25FLXXXX_PAGE_SIZE, 0x1C, 1,
            S25FL032A_PROT_RATIO, &s25fl032a_regions, &def_cmd_table, S25FLXXXX_SECT_PER_BLOCK },

    { "s25fl064a", s25fl0xxx_probe, S25FL064A_ID, S25FLXXXX_SECT_SIZE,
            N_S25FL064A_SECTORS, S25FLXXXX_PAGE_SIZE, 0x1C, 1,
            S25FL064A_PROT_RATIO, &s25fl064a_regions, &def_cmd_table, S25FLXXXX_SECT_PER_BLOCK },

    { "s25fl128p", s25fl128p_probe, S25FL128P_ID, 0,
            0, S25FLXXXX_PAGE_SIZE, 0x3C, 1,
            0, &s25fl128p_regions, &def_cmd_table, S25FLXXXX_SECT_PER_BLOCK },

    { "m25fl128p", m25flxxxx_probe, M25FL128P_ID, M25FL128P_SECT_SIZE,
            N_M25FL128P_SECTORS, M25FLXXXX_PAGE_SIZE, 0x3C, 1,
            M25FL128P_PROT_RATIO, &m25fl128p_regions, &def_cmd_table, M25FLXXXX_SECT_PER_BLOCK },

    { "m25fl64p", m25flxxxx_probe, M25FL64P_ID, M25FL64P_SECT_SIZE,
            N_M25FL64P_SECTORS, M25FLXXXX_PAGE_SIZE, 0x3C, 1,
            M25FL128P_PROT_RATIO, &m25fl64p_regions, &def_cmd_table, M25FLXXXX_SECT_PER_BLOCK },

    { "n25q128", m25flxxxx_probe, N25Q128_ID, N25Q128_SECT_SIZE,
            N_N25Q128_SECTORS, N25QXXXXX_PAGE_SIZE, 0x3C, 1,
            N25Q128_PROT_RATIO, &n25q128_regions, &n25qxxxx_cmd_table, N25QXXXXX_SECT_PER_BLOCK },

    { "mx25l1605", mx25lxxxx_probe, MX25L1605_ID, MX25LXXXX_SECT_SIZE,
            N_MX25L1605_SECTORS, MX25LXXXX_PAGE_SIZE, 0x3C, 1,
            MX25LXXXX_PROT_RATIO, &mx25l1605_regions, &mx25lxxxx_cmd_table, MX25LXXXX_SECT_PER_BLOCK },

    { "mx25l3205", mx25lxxxx_probe, MX25L3205_ID, MX25LXXXX_SECT_SIZE,
            N_MX25L3205_SECTORS, MX25LXXXX_PAGE_SIZE, 0x3C, 1,
            MX25LXXXX_PROT_RATIO, &mx25l3205_regions, &mx25lxxxx_cmd_table, MX25LXXXX_SECT_PER_BLOCK },

    { "mx25l6405", mx25lxxxx_probe, MX25L6405_ID, MX25LXXXX_SECT_SIZE,
            N_MX25L6405_SECTORS, MX25LXXXX_PAGE_SIZE, 0x3C, 1,
            MX25LXXXX_PROT_RATIO, &mx25l6405_regions, &mx25lxxxx_cmd_table, MX25LXXXX_SECT_PER_BLOCK },

    { "mx25l12805", mx25lxxxx_probe, MX25L12805_ID, MX25LXXXX_SECT_SIZE,
            N_MX25L12805_SECTORS, MX25LXXXX_PAGE_SIZE, 0x3C, 1,
            MX25LXXXX_PROT_RATIO, &mx25l12805_regions, &mx25lxxxx_cmd_table, MX25LXXXX_SECT_PER_BLOCK },

    { "w25q64bv", w25qxxxx_probe, W25Q64BV_ID, W25QXXXBV_SECT_SIZE,
            N_W25Q64BV_SECTORS, W25QXXXBV_PAGE_SIZE, 0x3C, 1,
            W25QXXXBV_PROT_RATIO, &w25q64bv_regions, &w25qxxxx_cmd_table, W25QXXXBV_SECT_PER_BLOCK },

    { "w25q128bv", w25qxxxx_probe, W25Q128BV_ID, W25QXXXBV_SECT_SIZE,
            N_W25Q128BV_SECTORS, W25QXXXBV_PAGE_SIZE, 0x3C, 1,
            W25QXXXBV_PROT_RATIO, &w25q128bv_regions, &w25qxxxx_cmd_table, W25QXXXBV_SECT_PER_BLOCK },

    { "s25vf064", s25vfxxxx_probe, S25VF064_ID, S25VF064_SECT_SIZE,
            N_S25VF064_SECTORS, S25VFXXXX_PAGE_SIZE, 0x3C, 1,
            S25VF064_PROT_RATIO, &s25vf064_regions, &w25qxxxx_cmd_table, S25VFXXXX_SECT_PER_BLOCK },

    { "s25fl256s", s25fl256s_probe, S25FL256S_ID, S25FLXXXX_SECT_SIZE,
            N_S25FL256S_SECTORS, S25VFXXXX_PAGE_SIZE, 0x3C, 1,
            S25FL256S_PROT_RATIO, &s25fl256s_regions, &def_cmd_table, S25FLXXXX_SECT_PER_BLOCK },

    { "n25q256a", n25qxxxx_probe, N25Q256_ID, N25Q256_SECT_SIZE,
            N_N25Q256_SECTORS, N25QXXXXX_PAGE_SIZE, 0x3C, 1,
            N25Q256_PROT_RATIO, &n25q256a_regions, &n25qxxxx_cmd_table, N25QXXXXX_SECT_PER_BLOCK },
#else /* CONFIG_MACH_PUMA6  For Puma-6 SoC */
    { "n25q256", n25qxxxx_probe, N25Q256_ID, N25Q256_SECT_SIZE,
            N_N25Q256_SECTORS, N25QXXXXX_PAGE_SIZE, 0x3C, 1,
            N25Q256_PROT_RATIO, &n25q256_regions, &n25qxxxx_cmd_table, N25QXXXXX_SECT_PER_BLOCK },
#endif
};

/* Helper macros */
#define UNLOCK_ALL_SECTORS              (0x0)
#define DUMMY_ADDRESS                   (0x0000)

#define NUM_REGIONS(i)                  (sfl_data[i].n_regions)
#define NUM_SECTORS(i)                  (sfl_data[i].n_sectors)
#define SFL_PAGESIZE(i)                 (sfl_data[i].page_size)
#define LOCK_ALL_SECTORS(i)             (sfl_data[i].lock_all)
#define PROTECT_BITS_MASK(i)            (LOCK_ALL_SECTORS(i))
#define SFL_SECTOR_SIZE(i)              (sfl_data[i].sector_size)
#define SFL_SIZE(i)                     (SFL_SECTOR_SIZE(i) * NUM_SECTORS(i))
#define SFL_PROT_RATIO(i)               (sfl_data[i].prot_ratio)
#define ERASE_REGION(i)                 (sfl_data[i].regions)
#define SFL_SECT_PER_BLOCK(i)           (sfl_data[i].sectors_per_block)

/* Command Table Access Macros  */
/* Read */
#define OPCODE_READ(i)                  (sfl_data[i].cmd->r)
#define OPCODE_FAST_READ(i)             (sfl_data[i].cmd->fr)
#define OPCODE_DUAL_FAST_READ(i)        (sfl_data[i].cmd->dfr)
#define OPCODE_READ_DEV_ID(i)           (sfl_data[i].cmd->rdid)
/* Write Control */
#define OPCODE_WRITE_ENABLE(i)          (sfl_data[i].cmd->wren)
#define OPCODE_WRITE_DISABLE(i)         (sfl_data[i].cmd->wrdi)
/* Erase */
#define OPCODE_SECTOR_ERASE(i)          (sfl_data[i].cmd->se)
#define OPCODE_CHIP_ERASE(i)            (sfl_data[i].cmd->ce)
#define OPCODE_BLOCK_ERASE(i)           (sfl_data[i].cmd->be)

/* Program */
#define OPCODE_PAGE_PROGRAM(i)          (sfl_data[i].cmd->pp)
/* Status Regs */
#define OPCODE_READ_STATUS_REG(i)       (sfl_data[i].cmd->rdsr)
#define OPCODE_WRITE_STATUS_REG(i)      (sfl_data[i].cmd->wrsr)
/* Power Saving */
#define OPCODE_DEEP_POWER_DOWN(i)       (sfl_data[i].cmd->dp)
#define OPCODE_RELEASE_POWER_DOWN(i)    (sfl_data[i].cmd->rd)
#define OPCODE_READ_JEDEC_ID(i)         (sfl_data[i].cmd->res)


/* this is specific to sfl (may be for all spi flash devices) */
struct sfl_sfi_transfer_hdr_t
{
    unsigned int len;
    unsigned int rx_addr;
    void *rx_buff;
};


/* Global variables */
static s32 sfi_mode = -1;
static s32 fast_read = -1;
static u16 sfl_sfi_mode =  CONFIG_SFL_SFI;
static u16 sfl_fast_read = CONFIG_SFL_FAST_READ;
static struct sfl   flash_info[2]={0};
static u16 sfl_addr_mode = AVALANCHE_SPI_3_BYTE_ADDR_MODE;
/* Function definitions */

static inline struct sfl *mtd_to_sfl(struct mtd_info *mtd)
{
    return container_of(mtd, struct sfl, mtd);
}

#if 0 /* might require in future for locking/unlocking support */
/*
 * get_protect_bits_val return the value for the address range to lock the
 * flash.
 * Returns protection bit value on success.
 */
static unsigned char get_protect_bits_val(struct sfl *flash,
                        unsigned int start_addr, unsigned int end_addr)
{
    ENTER();
    if( end_addr > SFL_SIZE(flash->index) )
    {
        EXIT();
        return 0;
    }
    return 0;
/* TODO */
#if 0
    if(start_addr >= 0x7E0000){
        return 0x04;
    }else if( start_addr >= 0x7C0000 ){
        return 0x08;
    }else if( start_addr >= 0x780000 ){
        return 0x0C;
    }else if( start_addr >= 0x700000 ){
        return 0x10;
    }else if( start_addr >= 0x600000 ){
        return 0x14;
    }else if( start_addr >= 0x400000 ){
        return 0x18;
    }else if ( start_addr >= 0x000000){
        return 0x1C;
    }else{
        return 0;
    }
#endif
}
#endif

/*
 * write_status_reg writes the vlaue to status reg.
 * Returns status on success or negetive valueif error.
 * NOTE:  write enable should be called before calling this function.
 */
static int write_status_reg(struct sfl *flash,unsigned char  val)
{
    s32 ret = 0;
    unsigned char write_val[2] = {OPCODE_WRITE_STATUS_REG(flash->index), val};
    DEBUG_SFL();
    /* Setup */
    spi_setup(flash->spi);

    /* Send command */
    if((ret = spi_write(flash->spi, write_val, 2)) < 0)
        PRINTK("failed\n");

    return 0;
}

/*
 * read_status_reg return the status of the flash
 * Returns status on success or negetive valueif error.
 */
static int read_status_reg(struct sfl *flash, unsigned char *val)
{
    ssize_t ret;
    unsigned char code = OPCODE_READ_STATUS_REG(flash->index);
    ENTER();
    *val = 0;  /* Initialize return value */

    /* Setup */
    spi_setup(flash->spi);

    /* Send command */
    ret = spi_write_then_read(flash->spi, &code, 1, val, 1);
    if( ret < 0 )
    {
        dev_err(&flash->spi->dev, "error %d reading SR\n",(int) ret);
        PRINTK("error %d reading SR\n",(int) ret);
    }
    EXIT();
    return ret;
}

/*
 * write_enable perpares the flash state to enable write/erase/lock/unlock
 * Returns 0 on success or negetive valueif error.
 */
static inline int write_enable(struct sfl *flash)
{
    ssize_t ret;
    unsigned char   code = OPCODE_WRITE_ENABLE(flash->index);

    ENTER();
    /* Setup */
    spi_setup(flash->spi);

    /* Send command */
    ret = spi_write(flash->spi, &code,1 );

    return ret;
}

/*
 * wait_till_erase_ready to read status register until ready, or timeout occurs.
 * Returns non-zero if error.
 */
static int wait_till_erase_ready(struct sfl *flash)
{
    unsigned char sr;
    unsigned long timeout = jiffies + SFL_DELAY;

    ENTER();
    do{
        msleep(1);
        if(read_status_reg(flash, &sr)) {
            continue;
        }else if (!(sr & SR_WIP)){
            return 0;
        }

    }while(time_after(timeout, jiffies));
    EXIT();
    PRINTK( "*****************************************************************\n" );
    PRINTK( "*** Serial flash is stuck on busy state, rebooting the system! **\n" );
    PRINTK( "*****************************************************************\n" );

    /* Kill init */
    kill_proc( 1, SIGTERM, 1 );

    return 1;
}

/*
 * wait_till_ready to read status register until ready, or timeout occurs.
 * Returns non-zero if error.
 */
static int wait_till_ready(struct sfl *flash)
{
    unsigned char sr;
    unsigned long timeout = jiffies + SFL_DELAY;

    ENTER();
    do{
        mdelay(1);
        if(read_status_reg(flash, &sr))
        {
            continue;
        }
        else if (!(sr & SR_WIP))
        {
            return 0;  /* Normal exit */
        }

    }while(time_after(timeout, jiffies));
    EXIT();
    PRINTK( "*****************************************************************\n" );
    PRINTK( "*** Serial flash is stuck on busy state, rebooting the system! **\n" );
    PRINTK( "*****************************************************************\n" );

    /* Kill init */
    kill_proc( 1, SIGTERM, 1 );

    return 1;
}

#ifdef MONOLITHIC_32MB_FLASH

#define FLASH_BORDER_OFFSET_16MB  0x1000000   /*16MB BORDER ADDRESS*/

#define EMM_LOW_ADDRESS         0
#define EMM_HIGH_ADDRESS        1

#define EMM_OK      0
#define EMM_ERR     1
#define EMM_LOW     2
#define EMM_HIGH    3
#define EMM_NOT_FL  5
/*array of flash modes, array dimension is equal to number of chip select (CS)*/
static int ext_mem_mode[2] = {-1};  /* Initialise to "no set" */

/* Spansion */

#define SPANSION_MAN_ID 0x01

#define SPANSION_BRAC               0xB9    /* Bank Register access */
#define SPANSION_WRTR               0x01    /* Write register       */
#define SPANSION_BRRD               0x16    /* Bank Register read   */
#define SPANSION_BRWR               0x17    /* Bank Register write  */

int switch_spansion_bank(struct sfl *flash, unsigned char mode)
{
    int ret = 0;
    unsigned char cmds[] = {SPANSION_BRAC, SPANSION_WRTR, mode, SPANSION_BRRD};
    
    DEBUG_SFL("%s %d bank=%d mode=%d\n", __FUNCTION__, __LINE__, 
                                flash->spi->chip_select, mode);

    if (wait_till_ready(flash) )
    {
        PRINTK("wait_till_ready failed\n");
        return -EIO;
    }

    /* Write new Bank Register value: phase 1*/
    ret = spi_write(flash->spi, &cmds[0], sizeof(cmds));
    if (wait_till_ready(flash) )
    {
        PRINTK("wait_till_ready failed\n");
        return -EIO;
    }	
    if( ret < 0 )
	{
		dev_err(&flash->spi->dev, "error %d writing BRAC (1)\n",(int) ret);
		PRINTK("error %d writing BRAC (1)\n",(int) ret);
	}

    /* Write new Bank Register value: phase 2*/
    cmds[0] = SPANSION_BRWR;
    cmds[1] = mode;  
    ret = spi_write(flash->spi, &cmds[0], 2);

    if (wait_till_ready(flash) )
    {
        PRINTK("wait_till_ready failed\n");
        return -EIO;
    }
    if( ret < 0 )
	{
		dev_err(&flash->spi->dev, "error %d riting BRAC (2)\n",(int) ret);
		PRINTK("error %d writing BRAC (2)\n",(int) ret);
	}

    DEBUG_SFL("Spansion flash switched to %s area\n", mode ? "high" : "low");

    return EMM_OK;
}

/* Numonyx */

#define NUMONYX_WRNVCR          0xB1 /* Write non-volatile configuration reg. */
#define NUMONYX_RDNVCR          0xB5 /* Read non-volatile configuration reg. */
#define NUMONYX_RDEAR           0xC8 /* Read extended address register */
#define NUMONYX_WREAR           0xC5 /* Write extended address register */

int switch_numonyx_bank(struct sfl* flash, unsigned char mode)
{
    int ret = 0;
    u8 rdear_cmd = NUMONYX_RDEAR, rdear_val[] = {0, 0};
    /*2nd byte (mode value) in wrear cmd will be fixed after reading EAR*/
    u8 wrear_cmd[2] = {NUMONYX_WREAR, mode};
    
    if (wait_till_ready(flash) )
    {
        PRINTK("wait_till_ready failed\n");
        return -EIO;
    }	
    DEBUG_SFL("%s %d bank=%d mode=%d\n", __FUNCTION__, __LINE__, 
                                flash->spi->chip_select, mode);
   
    ret = spi_write_then_read(flash->spi, &rdear_cmd, 1, &rdear_val, 2);
	if( ret < 0 )
	{
		dev_err(&flash->spi->dev, "error %d reading EAR (1)\n",(int) ret);
		PRINTK("error %d reading EAR (1)\n",(int) ret);
	}

    DEBUG_SFL("Current flash bank is %s (EAR: 0x%x)\n",
        (rdear_val[0] & 0x01) ? "high" : "low", rdear_val[0]);

    if ((rdear_val[0] & 0x01) == (u8)mode)
    {
        DEBUG_SFL("Current flash bank is %s (EAR: 0x%x)\n",
            (rdear_val[0] & 0x01) ? "high" : "low", rdear_val[0]);

        if (wait_till_ready(flash) )
        {
            PRINTK("wait_till_ready failed\n");
            return -EIO;
        }
    }
   
    /*set bit in EAR for access to high bank and clear for low */
    wrear_cmd[1] = (mode ? (rdear_val[0] | 1) : rdear_val[0] & (~1));

    ret = write_enable(flash);
    if (wait_till_ready(flash) )
    {
        PRINTK("wait_till_ready failed\n");
        return -EIO;
    }
    ret = spi_write(flash->spi, wrear_cmd, sizeof(wrear_cmd));

    if (wait_till_ready(flash) )
    {
        PRINTK("wait_till_ready failed\n");
        return -EIO;
    }

    DEBUG_SFL("Numonyx flash switched to %s area\n", mode ? "high" : "low");
    

    return EMM_OK;
}

int test_flash_bank(struct sfl *flash, loff_t addr, size_t len, size_t *proc_len)
{
    /* If entire address space above 16MB border */
    if (addr >= FLASH_BORDER_OFFSET_16MB)
    {
        if (proc_len)
            *proc_len = len;

        return EMM_HIGH;        
    }

    /* If entire address space below 16MB border */
    else if (addr + len <= FLASH_BORDER_OFFSET_16MB)
    {
        if (proc_len)
            *proc_len = len;

        return EMM_LOW;
    }

    /* If address space begins below 16MB border and ends above it */
    else if ((addr < FLASH_BORDER_OFFSET_16MB) && 
            (addr + len > FLASH_BORDER_OFFSET_16MB))
    {
        if (proc_len)
            *proc_len = FLASH_BORDER_OFFSET_16MB - addr;

        return EMM_LOW;               
    }
    else
    {
        PRINTK("error: %s %d test flash error! addr=0x%llx len=%zd\n", 
                                            __FUNCTION__, __LINE__, addr, len);
        BUG();
    }
    return -EMM_ERR;
}

int switch_flash_bank(struct sfl *flash, u8 mode)
{
    int res;
    int idx = flash->index;
    unsigned char old_mode, temp;

    DEBUG_SFL("%s %d switch to mode=%d flash id %d, manuf id 0x%x\n", 
                __FUNCTION__, __LINE__, mode, idx, sfl_data[idx].id);

/* Change spi mode to AVALANCHE_CORE_SPI_MODE, in order to use switch command */
    SFI_SET_MODE(flash->spi,AVALANCHE_CORE_SPI_MODE, old_mode);

    switch (sfl_data[idx].id)
    {
        case S25FL256S_ID: 
            res = switch_spansion_bank(flash, mode);  
            break;

        case N25Q256_ID: 
            res = switch_numonyx_bank(flash, mode);
            break;

        default:
            SFI_SET_MODE(flash->spi, old_mode, temp);
            PRINTK("error: Undefined flash\n");
            return -EMM_ERR;
    }
    DEBUG_SFL("%s %d switch to %d res=%d\n", __FUNCTION__, __LINE__, mode, res);

    SFI_SET_MODE(flash->spi, old_mode, temp);
    return res;
}

#endif /* MONOLITHIC_32MB_FLASH */

/*
 * Erase an address range on the flash chip.  The address range may extend
 * one or more erase sectors.  Return an error is there is a problem erasing.
 */
static int sfl_erase_slice(struct mtd_info *mtd, struct erase_info *instr)
{
    struct sfl *flash = mtd_to_sfl(mtd);
    unsigned int addr;
    int len;
    unsigned char status;
    int ret = 0;
    int cmd_len = 0;
#ifdef MONOLITHIC_32MB_FLASH
    int fl_tst_res = 0; 
    size_t fl_retlen = 0;
#endif

    down(&flash->lock);

    DEBUG_SFL( "Erasing address 0x%x, length 0x%x\n",  instr->addr, instr->len );

    instr->state =  MTD_ERASING;
    addr = instr->addr;
    len = instr->len;

    /* now erase these sectors */
    while( len > 0)
    {
        unsigned int block_erase = 0;

#if defined (CONFIG_HW_MUTEXES)
        /* Lock the HW Mutex */
        if (hw_mutex_lock_interruptible(HW_MUTEX_NOR_SPI) != 0)
        {
            PRINTK("Flash Erase failed - Can't lock HW mutex \n");
            instr->state = MTD_ERASE_FAILED;
            ret = -EIO;
            goto erase_hwmutex_err;
        }
#endif
        /* read Status Register */
        if(wait_till_ready(flash) )
        {
            PRINTK("wait_till_ready failed\n");
            instr->state = MTD_ERASE_FAILED;
            ret = -EBUSY;
            len = 0; /* set loop exit condition */
            goto erase_loop_exit;
        }

        if(read_status_reg(flash, &status) < 0) {
            instr->state = MTD_ERASE_FAILED;
            ret = -EIO;
            len = 0; /* set loop exit condition */
            goto erase_loop_exit;
        }
        status &= PROTECT_BITS_MASK(flash->index);

        /* Check if flash is locked */
        if(status)
        {
            /* sfl unlock layout fix */
            status = UNLOCK_ALL_SECTORS;

            /* Write new Status (unlock) to Status Register */
            write_enable(flash);
            if (wait_till_ready(flash) )
            {
                PRINTK("wait_till_ready failed\n");
                instr->state = MTD_ERASE_FAILED;
                ret = -EBUSY;
                len = 0; /* set loop exit condition */
                goto erase_loop_exit;
            }

            if((ret = write_status_reg(flash,status))){
                PRINTK("write_status_reg failed\n");
            }

            /* read Status Register again, to verify*/
            if(wait_till_ready(flash) )
            {
                PRINTK("wait_till_ready failed\n");
                instr->state = MTD_ERASE_FAILED;
                ret = -EBUSY;
                len = 0; /* set loop exit condition */
                goto erase_loop_exit;
            }

            if(read_status_reg(flash, &status) < 0) {
                instr->state = MTD_ERASE_FAILED;
                ret = -EIO;
                len = 0; /* set loop exit condition */
                goto erase_loop_exit;
            }
            status &= PROTECT_BITS_MASK(flash->index);

            if(status)
            {
                PRINTK("ERROR: Erasing in locked region BPx = 0x%0x\n",status);
                instr->state = MTD_ERASE_FAILED;
                ret = -EROFS;
                len = 0; /* set loop exit condition */
                goto erase_loop_exit;
            }
        }

        /* For 4KB sectors flash devices, erase blocks when possible for faster performance */
        if( SFL_SECT_PER_BLOCK(flash->index) > 1 )
        {
            /* Check if we are in block boundry, and if erasing a block will not overflow */
            if ( ( ( addr & ( ( mtd->erasesize*SFL_SECT_PER_BLOCK(flash->index) ) - 1 ) ) == 0) && ( len >= ( mtd->erasesize*SFL_SECT_PER_BLOCK(flash->index) ) ) )
            {
               block_erase = 1;
               DEBUG_SFL( "Block erase, addr 0x%x\n", addr );
            }
        }

#ifdef MONOLITHIC_32MB_FLASH
        /* Current 32MB monolithic flashes do not support block erasing (only 1 sect per block) */
        fl_tst_res = test_flash_bank(flash, addr, len, &fl_retlen);
		DEBUG_SFL("%s %s 0x%08x, len %d fl_tst_res=%d\n ", __FUNCTION__,"from", addr, len, fl_tst_res);
        if (fl_tst_res == EMM_HIGH)
        {
            addr -= FLASH_BORDER_OFFSET_16MB;
            /*Switch if we currently not in high area yet*/
            if (ext_mem_mode[flash->spi->chip_select] != EMM_HIGH)
            {
                ext_mem_mode[flash->spi->chip_select] = EMM_HIGH;    
                switch_flash_bank(flash, EMM_HIGH_ADDRESS);
            }
        }
        else if (fl_tst_res == EMM_LOW)
        {
            /*Switch if we currently not in high area yet*/
            if (ext_mem_mode[flash->spi->chip_select] != EMM_LOW)
            {
                ext_mem_mode[flash->spi->chip_select] = EMM_LOW;    
                switch_flash_bank(flash, EMM_LOW_ADDRESS);
            }
            if (fl_retlen != len)
            {
                /*This should never happen, because we erase full sector
                within one area low or high*/
                BUG();
            }
        }
        else
            PRINTK("Error: flash test bank ret %d\n", fl_tst_res);
#endif

        if (sfl_addr_mode  == AVALANCHE_SPI_3_BYTE_ADDR_MODE)
        {
            /* Set up command buffer. */
            flash->command[0] = block_erase ? OPCODE_BLOCK_ERASE(flash->index) : OPCODE_SECTOR_ERASE(flash->index);
            flash->command[1] = addr >> 16;
            flash->command[2] = addr >> 8;
            flash->command[3] = addr;
            cmd_len = SFL_CMD_FRAME_SIZE_4;
        }
        else
        {
            /* Set up command buffer. */
            flash->command[0] = block_erase ? OPCODE_BLOCK_ERASE(flash->index) : OPCODE_SECTOR_ERASE(flash->index);
            flash->command[1] = addr >> 24;
            flash->command[2] = addr >> 16;
            flash->command[3] = addr >> 8;
            flash->command[4] = addr;
            cmd_len = SFL_CMD_FRAME_SIZE_5;
        }

        /* Send write enable, then erase commands. */
        write_enable(flash);
        if(wait_till_ready(flash) )
        {
            PRINTK("wait_till_ready failed\n");
            instr->state = MTD_ERASE_FAILED;
            ret = -EBUSY;
            len = 0; /* set loop exit condition */
            goto erase_loop_exit;
        }

        spi_write(flash->spi, flash->command,cmd_len);

        if(wait_till_erase_ready(flash) )
        {
            PRINTK("wait_till_erase_ready failed\n");
            instr->state = MTD_ERASE_FAILED;
            ret = -EBUSY;
            len = 0; /* set loop exit condition */
            goto erase_loop_exit;
        }

#ifdef MONOLITHIC_32MB_FLASH
        if (fl_tst_res == EMM_HIGH)
        {
            /*restore the address for correct usage in next iterations, if any*/
            addr += FLASH_BORDER_OFFSET_16MB;
            /*flash still in high mode here*/
        }
#endif

        if( !block_erase )
        {
            addr += mtd->erasesize;
            len -= mtd->erasesize;
        }
        else
        {
            addr += ( mtd->erasesize * SFL_SECT_PER_BLOCK(flash->index) );
            len -= ( mtd->erasesize * SFL_SECT_PER_BLOCK(flash->index) );
        }
erase_loop_exit: ;
#if defined (CONFIG_HW_MUTEXES)
        /* Un-lock the HW Mutex */
        hw_mutex_unlock(HW_MUTEX_NOR_SPI);
#endif
    }
    if (instr->state == MTD_ERASING)
        instr->state = MTD_ERASE_DONE;
#if defined (CONFIG_HW_MUTEXES)
erase_hwmutex_err:
#endif
    up(&flash->lock);
    mtd_erase_callback(instr);

    return ret;
}

/*
 * Erase an address range on the flash chip.  The address range may extend
 * one or more erase sectors.  Return an error is there is a problem erasing.
 */
static int sfl_erase(struct mtd_info *mtd, struct erase_info *instr)
{
    struct sfl *flash = mtd_to_sfl(mtd);
    int len, size;
    unsigned int addr;
    int ret = 0;

    DEBUG_SFL( "Erasing address 0x%x, length 0x%x\n",  (unsigned int)instr->addr, (unsigned int)instr->len );

    /* sanity checks */
    if( ( instr->addr + instr->len ) > flash->mtd.size)
    {
        PRINTK("invalid address (or) length\n");
        return -EINVAL;
    }
    if ( (( instr->addr & ( mtd->erasesize - 1 ) ) != 0 )   ||
         ( ( instr->len & ( mtd->erasesize - 1 ) ) != 0 )  )
    {
        PRINTK("invalid address (or) length, exceeds partition size\n");
        return -EINVAL;
    }

    len = instr->len;
    addr = instr->addr;

    while( len > 0)
    {
        unsigned int block_erase = 0;

        /* For 4KB sectors flash devices, erase blocks when possible for faster performance */
        if( SFL_SECT_PER_BLOCK(flash->index) > 1 )
        {
            /* Check if we are in block boundry, and if erasing a block will not overflow */
            if ( ( ( addr & ( ( mtd->erasesize*SFL_SECT_PER_BLOCK(flash->index) ) - 1 ) ) == 0) && ( len >= ( mtd->erasesize*SFL_SECT_PER_BLOCK(flash->index) ) ) )
            {
               block_erase = 1;
               DEBUG_SFL( "Block erase, addr 0x%x\n", addr );
            }
        }

        if( !block_erase )
        {
            size = mtd->erasesize;
        }
        else
        {
            size = ( mtd->erasesize * SFL_SECT_PER_BLOCK(flash->index) );
        }

        instr->len = size;
        instr->addr = addr;

        ret = sfl_erase_slice(mtd, instr);

        if (ret != 0)
        {
            return ret;
        }

        addr += size;
        len -= size;
    }

    return ret;
}

/*
 * Read an address range from the flash chip.  The address range
 * may be any size provided it is within the physical boundaries.
 */

static int sfl_read( struct mtd_info *mtd, loff_t from, size_t len,
                          size_t *retlen, u_char *buf)
{
    struct sfl *flash = mtd_to_sfl(mtd);
    struct spi_transfer t[2];
    struct spi_message m;
    struct sfl_sfi_transfer_hdr_t sfi_hdr;
    unsigned int page_offset, page_size,i;
    unsigned short index = 0;
    unsigned char old_mode,temp, read_cmd;
    int ret = 0;
#ifdef MONOLITHIC_32MB_FLASH
    int fl_tst_res = 0; 
    size_t fl_retlen = 0;
#endif
    DEBUG_SFL("Read from 0x%08x, len %zd\n",(u32)from,len);

    if( !len )
    {
        PRINTK("Invalid length\n");
        return 0;
    }
    if( ((from + len ) > flash->mtd.size) || (retlen == NULL))
    {
        PRINTK("Argument exceeds partitions size \n");
        return -EINVAL;
    }

    /* Semaphore */
    down(&flash->lock);

    spi_message_init(&m);
    memset(t, 0, sizeof(t));

    /* if we are working in SFI mode then do the whole read and return immediately */
    if(likely(sfl_sfi_mode))
    {
        DEBUG_SFL("Initializing sfi_hdr index = %d \n",index);

        sfi_hdr.len     = len;
        sfi_hdr.rx_buff = buf;
        sfi_hdr.rx_addr = from;
        t[index].rx_buf = &sfi_hdr;
        t[index].len = sizeof(sfi_hdr);

        DEBUG_SFL("sfl sfi header = 0x%0x, len=%d rx_addr=0x%0x\n",
               (unsigned)t[index].rx_buf, sfi_hdr.len, sfi_hdr.rx_addr);

        spi_message_add_tail(&t[index], &m);

        /* Switch mode to SFI */
        SFI_SET_MODE(flash->spi,AVALANCHE_SFI_MODE,old_mode);

#if defined (CONFIG_HW_MUTEXES)
        /* Lock the HW Mutex */
        if (hw_mutex_lock_interruptible(HW_MUTEX_NOR_SPI) != 0)
        {
            PRINTK("Flash Read failed - Can't lock HW mutex \n");

            /* Switch back to old mode*/
            SFI_SET_MODE(flash->spi,old_mode,temp);
            *retlen = 0;
            ret = -EIO;
            goto read_err;
        }
#endif
#ifdef MONOLITHIC_32MB_FLASH
        fl_tst_res = test_flash_bank(flash, from, len, &fl_retlen);
        if (fl_tst_res == EMM_HIGH)
        {
            sfi_hdr.rx_addr -= FLASH_BORDER_OFFSET_16MB;
            if (ext_mem_mode[flash->spi->chip_select] != EMM_HIGH ) 
            {
                ext_mem_mode[flash->spi->chip_select] = EMM_HIGH; 
                switch_flash_bank(flash, EMM_HIGH_ADDRESS);  
            }
            spi_sync(flash->spi, &m);
        }
        else if (fl_tst_res == EMM_LOW)
        {
            if (ext_mem_mode[flash->spi->chip_select] != EMM_LOW ) 
            {
                ext_mem_mode[flash->spi->chip_select] = EMM_LOW; 
                switch_flash_bank(flash, EMM_LOW_ADDRESS);  
            }

            /*If we start to read in low area and continue in the high one*/
            if (fl_retlen != len)
            {
                /*Set length from low area for sfi ftransfer*/
                sfi_hdr.len = fl_retlen;
                /*Get data from low area*/
                spi_sync(flash->spi, &m);
                /*Set new rx addr (should be 0x0 - as 0 byte in high bank)*/
                sfi_hdr.rx_addr = from + fl_retlen - FLASH_BORDER_OFFSET_16MB;
                if (sfi_hdr.rx_addr)
                    BUG();
                /*Set length of rest of data in high area and new buffer pointer*/
                sfi_hdr.len = len - fl_retlen;    
                sfi_hdr.rx_buff = buf + fl_retlen;
                /*Switch flash to high area*/
                ext_mem_mode[flash->spi->chip_select] = EMM_HIGH; 
                ret = switch_flash_bank(flash, EMM_HIGH_ADDRESS);
 
                fl_tst_res = EMM_HIGH;
                /*Get data from high area*/
                spi_sync(flash->spi, &m);
                /*Set total length of data*/
                sfi_hdr.len = len;
            }
            else
            {
                spi_sync(flash->spi, &m);
            }
        }
#else
        spi_sync(flash->spi, &m);
#endif


#if defined (CONFIG_HW_MUTEXES)
        /* Un-lock the HW Mutex */
        hw_mutex_unlock(HW_MUTEX_NOR_SPI);
#endif
        /* Switch back to old mode*/
        SFI_SET_MODE(flash->spi,old_mode,temp);

        *retlen = sfi_hdr.len;

        /* Semaphore */
        up(&flash->lock);
        return 0;
    }
    else
    {
        /* Read in slow SPI mode */

        /* we need the flash command packet only if we are operating in core SPI
         * mode
         */

        t[index].tx_buf = flash->command;
        /* only fast read needs dummy byte address */
        if(sfl_fast_read == SFL_NORMAL_READ)
        {
            t[index].len = SFL_CMD_FRAME_SIZE_4;
        }
        else
        {
            /* In Fast/Dual fast read mode ADDRESS + DUMMY BYTE */
            t[index].len = (SFL_CMD_FRAME_SIZE_4 + 1);
        }
        if (sfl_addr_mode == AVALANCHE_SPI_4_BYTE_ADDR_MODE)
        {
            /* Increase len by one */
            t[index].len++;
        }

        DEBUG_SFL("length of command is %d\n",t[index].len);
        spi_message_add_tail(&t[index], &m);
        index++;
        t[index].rx_buf = buf;

        spi_message_add_tail(&t[index], &m);
        *retlen = 0;

        /* sfl read in core spi mode */
        if(sfl_fast_read == SFL_NORMAL_READ)
        {
            DEBUG_SFL("initializing the NORMAL READ CMD \n");
            read_cmd = OPCODE_READ(flash->index);
        }
        else if(sfl_fast_read == SFL_FAST_READ)
        {
            DEBUG_SFL("initializing the FAST READ CMD \n");
            read_cmd = OPCODE_FAST_READ(flash->index);
        }
        else
        {
            DEBUG_SFL("initializing the DUAL FAST READ CMD \n");
            read_cmd = OPCODE_DUAL_FAST_READ(flash->index);
        }
        if (sfl_addr_mode == AVALANCHE_SPI_3_BYTE_ADDR_MODE)
        {
            flash->command[0] = read_cmd;
            flash->command[1] = from >> 16;
            flash->command[2] = from >> 8;
            flash->command[3] = from;
            flash->command[4] = DUMMY_ADDRESS;
        }
        else
        {
            flash->command[0] = read_cmd;
            flash->command[1] = from >> 24;
            flash->command[2] = from >> 16;
            flash->command[3] = from >> 8;
            flash->command[4] = from;
            flash->command[5] = DUMMY_ADDRESS;

        }

        /* what page do we start with? */
        page_offset = from & ( SFL_PAGESIZE(flash->index) - 1 );

#if defined (CONFIG_HW_MUTEXES)
        /* Lock the HW Mutex */
        if (hw_mutex_lock_interruptible(HW_MUTEX_NOR_SPI) != 0)
        {
            PRINTK("Flash Read failed - Can't lock HW mutex \n");
            ret = -EIO;
            goto read_err;
        }
#endif

        /* do all the bytes fit onto one page? */
        if (page_offset + len <= SFL_PAGESIZE(flash->index))
        {
            DEBUG_SFL("Read with in a page \n");
            t[index].len = len;
            spi_sync(flash->spi, &m);
            DEBUG_SFL("returned = %d index = %d, cmd size = %d\n",
                   m.actual_length, index, t[0].len);
            *retlen = m.actual_length - t[0].len;
        }
        else
        {
            DEBUG_SFL("Read Multiple pages \n");
            /* the size of data remaining on the first page */
            page_size = SFL_PAGESIZE(flash->index) - page_offset;
            t[index].len = page_size;
            spi_sync(flash->spi, &m);
            *retlen = m.actual_length - t[0].len;

            /* write everything in PAGESIZE chunks */
            for (i = page_size; i < len; i += page_size)
            {
                page_size = len - i;

                if (page_size > SFL_PAGESIZE(flash->index))
                {
                    page_size = SFL_PAGESIZE(flash->index);
                }

                if (sfl_addr_mode == AVALANCHE_SPI_3_BYTE_ADDR_MODE)
                {
                    flash->command[0] = read_cmd;
                    flash->command[1] = (from + i) >> 16;
                    flash->command[2] = (from + i) >> 8;
                    flash->command[3] = (from + i);
                    flash->command[4] = DUMMY_ADDRESS;
                }
                else
                {
                    flash->command[0] = read_cmd;
                    flash->command[1] = (from + i) >> 24;
                    flash->command[2] = (from + i) >> 16;
                    flash->command[3] = (from + i) >> 8;
                    flash->command[4] = (from + i);
                    flash->command[5] = DUMMY_ADDRESS;
                }

                t[index].rx_buf = buf + i;
                t[index].len = page_size;

                spi_sync(flash->spi, &m);

                if (retlen){
                    DEBUG_SFL("returned = %d index = %d, cmd size = %d\n",
                        m.actual_length, index,t[0].len);
                    *retlen += m.actual_length - (t[0].len);
                }
            }
        }

        DEBUG_SFL("Actual Length : %d\n", *retlen);
#if defined (CONFIG_HW_MUTEXES)
        /* Un-lock the HW Mutex */
        hw_mutex_unlock(HW_MUTEX_NOR_SPI);
#endif
    }
#if defined (CONFIG_HW_MUTEXES)
read_err:
#endif
    /* Semaphore */
    up(&flash->lock);

    return ret;

}

static int sfl_write_page( struct mtd_info *mtd, loff_t to, size_t len, size_t *retlen, const u_char *buf)
{
    struct sfl *flash = mtd_to_sfl(mtd);
    unsigned int page_offset;
    struct spi_transfer t[2];
    struct spi_message m;
    unsigned char status = 0;
    int ret = 0;

    ENTER();
    DEBUG_SFL("Writing page to 0x%08x, len %zd\n",(unsigned int)to, len);

    /* initialize return value */
    if( retlen )
    {
        *retlen = 0;
    }

    /* sanity checks */
    if( !len ){
        PRINTK("sfl_write_page failed - invalid length\n");
        return -EINVAL;
    }

    /* Get 'Page Offset' from 'to' address */
    page_offset = to & ( SFL_PAGESIZE(flash->index) - 1 );
    if (page_offset + len > SFL_PAGESIZE(flash->index))
    {
        PRINTK("sfl_write_page failed - length exceeds page size(%d) : (psge offset = 0x%p + len=%d)\n",SFL_PAGESIZE(flash->index),(void *)page_offset, len );
        return -EINVAL;
    }

    /* Wait until finished previous write command. */
    if(wait_till_ready(flash) )
    {
        return -EBUSY;
    }

    /* Read Status Register */
    if(read_status_reg(flash, &status) < 0)
    {
        return  -EIO;;
    }
    status &= PROTECT_BITS_MASK(flash->index);

    /* Check if flash is locked */
    if(status)
    {
        DEBUG_SFL("Warning: Writing in locked region BPx = 0x%0x, trying to unlock\n",status);

        /* sfl unlock layout fix */
        status = UNLOCK_ALL_SECTORS;

        /* Set Write Enable */
        write_enable(flash);

        /* Write new status (unlocked) to Status Register */
        if(wait_till_ready(flash) )
        {
            return -EBUSY;
        }

        if((ret = write_status_reg(flash,status)))
        {
            PRINTK("sfl_write_page failed - write_status_reg failed\n");
            return  -EIO;
        }

        /* Wait until finished previous write command. */
        if(wait_till_ready(flash) )
        {
            return -EBUSY;
        }

        /* Read Status Register */
        if(read_status_reg(flash, &status) < 0) {
            return -EIO;
        }
        status &= PROTECT_BITS_MASK(flash->index);

        if(status)
        {
            PRINTK("ERROR: Writing in locked region BPx = 0x%0x\n",status);
            return -EROFS;
        }
    }
    if (sfl_addr_mode == AVALANCHE_SPI_3_BYTE_ADDR_MODE)
    {
        /* Set up the opcode in the write buffer. */
        flash->command[0] = OPCODE_PAGE_PROGRAM(flash->index);
        flash->command[1] = to >> 16;
        flash->command[2] = to >> 8;
        flash->command[3] = to;
    }
    else
    {
        /* Set up the opcode in the write buffer. */
        flash->command[0] = OPCODE_PAGE_PROGRAM(flash->index);
        flash->command[1] = to >> 24;
        flash->command[2] = to >> 16;
        flash->command[3] = to >> 8;
        flash->command[4] = to;
    }

    /* Initialize messages structures */
    memset(t, 0, (sizeof t));
    spi_message_init(&m);

    /* Set first message (Command)*/
    t[0].tx_buf = flash->command;
    if (sfl_addr_mode == AVALANCHE_SPI_4_BYTE_ADDR_MODE)
    {
        t[0].len = SFL_CMD_FRAME_SIZE_5;
    }
    else
    {
        t[0].len = SFL_CMD_FRAME_SIZE_4;
    }
    spi_message_add_tail(&t[0], &m);

    /* Set first message (Data)*/
    t[1].tx_buf = buf;
    t[1].len = len;
    spi_message_add_tail(&t[1], &m);


    write_enable(flash);
    if(wait_till_ready(flash) )
    {
        return -EBUSY;
    }

    spi_sync(flash->spi, &m);
    if(wait_till_ready(flash) )
    {
        return -EBUSY;
    }

    *retlen = m.actual_length - t[0].len;


    return ret;
}

/*****************************************************************************
     Currently Write is not supported in SFI mode so use SPI Mode writes
*****************************************************************************/
static int sfl_write( struct mtd_info *mtd, loff_t to, size_t len,
                               size_t *retlen, const u_char *buf)
{
    struct sfl *flash = mtd_to_sfl(mtd);
    unsigned int page_offset;
    unsigned char old_mode;
    int ret = 0;
    size_t retlen_page;
    size_t len_page;
    size_t bytes_written;
#ifdef MONOLITHIC_32MB_FLASH
    int fl_test_res = 0; 
    size_t fl_retlen = 0;
#endif


    ENTER();
    DEBUG_SFL("Writing to 0x%08x, len %zd\n",(unsigned int)to, len);

    /* initialize return value */
    if( retlen )
    {
        *retlen = 0;
    }

    /* sanity checks */
    if( !len ){
        PRINTK("invalid length\n");
        return -EINVAL;
    }

    if( ( to + len ) > flash->mtd.size){
        PRINTK("length exceeds partition size\n");
        return -EINVAL;
    }

    /* Semaphore */
    down(&flash->lock);

#if defined (CONFIG_HW_MUTEXES)
    /* Lock the HW Mutex */
    if (hw_mutex_lock_interruptible(HW_MUTEX_NOR_SPI) != 0)
    {
        PRINTK("sfl_write failed - Can't lock HW mutex\n");
        ret = -EIO;
        goto write_hwmutex_err;
    }
#endif

    /* Writes is only supported in CORE_SPI mode so make sure to be
     * in CORE_SPI mode
     */
    if(sfl_sfi_mode)
        SFI_SET_MODE(flash->spi, AVALANCHE_CORE_SPI_MODE, old_mode);

    /* Get first page offset */
    page_offset = to & ( SFL_PAGESIZE(flash->index) - 1 );

    /* Calculate first chunk size */
    if (page_offset + len <= SFL_PAGESIZE(flash->index))
    {
        len_page = len;
    }
    else
    {
        len_page = SFL_PAGESIZE(flash->index) - page_offset;
    }

    bytes_written = 0;
    while (bytes_written < len)
    {
#ifdef MONOLITHIC_32MB_FLASH
        fl_test_res = test_flash_bank(flash, to+bytes_written, len_page, &fl_retlen);
        if (fl_test_res == EMM_HIGH)
        {
            to -= FLASH_BORDER_OFFSET_16MB;
            /*Switch if we currently not in high area yet*/
            if (ext_mem_mode[flash->spi->chip_select] != EMM_HIGH)
            {
                ext_mem_mode[flash->spi->chip_select] = EMM_HIGH; 
                switch_flash_bank(flash, EMM_HIGH_ADDRESS);
            }
        }
        else
        {
            if (ext_mem_mode[flash->spi->chip_select] != EMM_LOW ) 
            {
                ext_mem_mode[flash->spi->chip_select] = EMM_LOW; 
                switch_flash_bank(flash, EMM_LOW_ADDRESS);  
            }
        }
#endif
        /* Write one page */
        if ((ret = sfl_write_page(mtd,to+bytes_written,len_page, &retlen_page, buf+bytes_written)) < 0)
        {
            PRINTK("sfl_write_page: failed\n");
            goto write_err;
        }

        /* update total written bytes */
        bytes_written += retlen_page;


        /* Calulate next chunk size - if there is less than one page size then reduce the chunk size*/
        if (len - bytes_written > SFL_PAGESIZE(flash->index))
            len_page = SFL_PAGESIZE(flash->index); /* normal page size*/
        else
            len_page = len - bytes_written;              /* last page chunk */

#ifdef MONOLITHIC_32MB_FLASH
            /* restore the address */
            if (fl_test_res == EMM_HIGH)
            {
                to += FLASH_BORDER_OFFSET_16MB;    
            }
#endif
    }

    *retlen = bytes_written;

write_err:
#if defined (CONFIG_HW_MUTEXES)
    /* Un-lock the HW Mutex */
    hw_mutex_unlock(HW_MUTEX_NOR_SPI);

write_hwmutex_err:
#endif
    /* Semaphore */
    up(&flash->lock);
    return ret;
}


static int sfl_unlockall(struct mtd_info *mtd)
{
    struct sfl *flash = mtd_to_sfl(mtd);
    unsigned char  status;
    s32 ret=0;

    /* Semaphore */
    down(&flash->lock);

#if defined (CONFIG_HW_MUTEXES)
    /* Lock the HW Mutex */
    if (hw_mutex_lock_interruptible(HW_MUTEX_NOR_SPI) != 0)
    {
        PRINTK("sfl_unlockall failed - Can't lock HW mutex\n");
        ret = -EIO;
        goto unlockall_hwmutex_err;
    }
#endif

    if(wait_till_ready(flash) )
    {
        ret = -EBUSY;
        goto unlockall_err;
    }

    /* sfl unlock layout fix */
    status = UNLOCK_ALL_SECTORS;

    write_enable(flash);
    if(wait_till_ready(flash) )
    {
        ret = -EBUSY;
        goto unlockall_err;
    }

    if((ret = write_status_reg(flash,status))){
        PRINTK("write_status_reg failed\n");
    }else{
        DEBUG_SFL("Whole flash unlocked\n");
    }

    if(wait_till_ready(flash) )
    {
        ret = -EBUSY;
        goto unlockall_err;
    }

    DEBUG_SFL(" 1 Status reg = 0x%x", read_status_reg(flash,&ret));

unlockall_err:
#if defined (CONFIG_HW_MUTEXES)
    /* Un-lock the HW Mutex */
    hw_mutex_unlock(HW_MUTEX_NOR_SPI);

unlockall_hwmutex_err:
#endif
    /* Semaphore */
    up(&flash->lock);
    return ret;
}


/* Lock and Unlock functions */
static  int sfl_lock (struct mtd_info *mtd, loff_t ofs, uint64_t len)
{
    s32 ret = 0;
/* TODO */
#if 0
    struct sfl *flash = mtd_to_sfl(mtd);
    unsigned char status = 0;

    ENTER();
    /* sanity checks */
    if( ( ofs + len ) > flash->mtd.size)
    {
        PRINTK("Argument exceeds partition size\n");
        return -EINVAL;
    }
    down(&flash->lock);
    if(wait_till_ready(flash) )
    {
        PRINTK("wait_till_ready failed\n");
        up(&lock);
        return -EBUSY;
    }
    status = get_protect_bits_val(flash, ofs,(ofs + len));
    DEBUG_SFL("Status reg = 0x%x\n",(unsigned)status);

    /* sfl lock layout fix */
    status = LOCK_ALL_SECTORS(flash->index);

    if ( status <= (unsigned char) LOCK_ALL_SECTORS(flash->index) )
    {
        write_enable(flash);
        if(wait_till_ready(flash) )
        {
            PRINTK("wait_till_ready failed\n");
            up(&lock);
            return -EBUSY;
        }

        if((ret = write_status_reg(flash,status)) != 0)
        {
            PRINTK("write_status_reg failed\n");
        }else{
            DEBUG_SFL("Sector(s) corrosponding to Address  0x%8x to  0x%8x is \
                     locked\n",(unsigned)ofs,(unsigned) (ofs +len) );
        }
    }
    up(&flash->lock);
#endif
    return ret;
}

/*
 *sfl_unlock unlocks the Faash sector form the  given address  to
 * MAx flash address.
 * returns 0 on success , negetive value otherwise
 */
static int sfl_unlock (struct mtd_info *mtd, loff_t ofs, uint64_t len)
{
    s32 ret=0;
/* TODO */
#if 0
    struct sfl *flash = mtd_to_sfl(mtd);
    unsigned char  status;
    unsigned char  temp;

    DEBUG_SFL("addr = 0x%8x and len = %d\n",(unsigned)ofs,(unsigned)len);

    /* sanity checks */
    if( ( ofs + len ) > flash->mtd.size)
    {
        PRINTK("Argument exceeds partition size\n");
        return -EINVAL;
    }
    down(&flash->lock);
    if(wait_till_ready(flash) )
    {
        PRINTK("wait_till_ready failed\n");
        up(&lock);
        return -EBUSY;
    }

    status = read_status_reg(flash);
    status &= PROTECT_BITS_MASK(flash->index);

    /* unlock the whole flash */
    if((len + ofs) >= (SFL_SIZE(flash->index) - (SFL_SECTOR_SIZE(flash->index) *2)))
    {
        DEBUG_SFL("Unlocking the whole flash \n");
        status = 0;
    }
    else
    {
        temp = get_protect_bits_val(flash, (ofs + len), SFL_SIZE(flash->index));
        if(status > temp)
            status = temp;
    }

    DEBUG_SFL("0 Status reg = 0x%x", read_status_reg(flash));

    /* sfl unlock layout fix */
    status = UNLOCK_ALL_SECTORS;

    if ( status <= (unsigned char) LOCK_ALL_SECTORS(flash->index) )
    {
        write_enable(flash);
        if(wait_till_ready(flash) )
        {
            PRINTK("wait_till_ready failed\n");
            up(&lock);
            return -EBUSY;
        }
        if((ret = write_status_reg(flash,status))){
            PRINTK("write_status_reg failed\n");
        }else{
            DEBUG_SFL("Sector(s) corrosponding to Address 0x%x to  0x%x is unlocked\n",
                    (unsigned)ofs, (unsigned )(ofs +len));
        }
    }
    DEBUG_SFL(" 1 Status reg = 0x%x", read_status_reg(flash));
    up(&flash->lock);
#endif
    return ret;
}

/* Power Management functions */
static int sfl_suspend (struct mtd_info *mtd)
{
    struct sfl *flash = mtd_to_sfl(mtd);
    struct spi_transfer t[2];
    struct spi_message m;
    s32 ret=0;

    memset(t, 0, (sizeof t));

    spi_message_init(&m);

    t[0].tx_buf = flash->command;
    /* operating in Core SPI Mode CMD length needed is 4 bytes no
     *  dummy byte required
     */
    if (sfl_addr_mode == AVALANCHE_SPI_4_BYTE_ADDR_MODE)
    {
        t[0].len = SFL_CMD_FRAME_SIZE_5;
    }
    else
    {
        t[0].len = SFL_CMD_FRAME_SIZE_4;
    }
    spi_message_add_tail(&t[0], &m);

     ENTER();

    /* Semaphore */
    down(&flash->lock);

#if defined (CONFIG_HW_MUTEXES)
    /* Lock the HW Mutex */
    if (hw_mutex_lock_interruptible(HW_MUTEX_NOR_SPI) != 0)
    {
        PRINTK("Flash Suspend failed - Can't lock HW mutex\n");
        ret = -EIO;
        goto suspend_hwmutex_err;
    }
#endif

    if(wait_till_ready(flash) )
    {
        ret = -EBUSY;
        goto suspend_err;
    }

    write_enable(flash);
    if(wait_till_ready(flash) )
    {
        ret = -EBUSY;
        goto suspend_err;
    }

    if (sfl_addr_mode == AVALANCHE_SPI_3_BYTE_ADDR_MODE)
    {
        /* Set up the opcode in the write buffer. */
        flash->command[0] = OPCODE_DEEP_POWER_DOWN(flash->index);
        flash->command[1] = DUMMY_ADDRESS >> 16;
        flash->command[2] = DUMMY_ADDRESS >> 8;
        flash->command[3] = DUMMY_ADDRESS;
    }
    else
    {
        /* Set up the opcode in the write buffer. */
        flash->command[0] = OPCODE_DEEP_POWER_DOWN(flash->index);
        flash->command[1] = DUMMY_ADDRESS >> 24;
        flash->command[2] = DUMMY_ADDRESS >> 16;
        flash->command[3] = DUMMY_ADDRESS >> 8;
        flash->command[4] = DUMMY_ADDRESS;
    }

    DEBUG_SFL("Going to Deep Sleep ..............Bye\n");
    spi_sync(flash->spi, &m);

    if(wait_till_ready(flash) )
    {
        ret = -EBUSY;
        goto suspend_err;
    }

suspend_err:
#if defined (CONFIG_HW_MUTEXES)
    /* Un-lock the HW Mutex */
    hw_mutex_unlock(HW_MUTEX_NOR_SPI);

suspend_hwmutex_err:
#endif
    /* Semaphore */
    up(&flash->lock);
    return ret;

}

static void sfl_resume (struct mtd_info *mtd)
{
    struct sfl *flash = mtd_to_sfl(mtd);
    struct spi_transfer t[2];
    struct spi_message m;

    memset(t, 0, (sizeof t));

    spi_message_init(&m);

    t[0].tx_buf = flash->command;
    /* operating in Core SPI Mode CMD length needed is 4 bytes no
     *  dummy byte required
     */
    if (sfl_addr_mode == AVALANCHE_SPI_4_BYTE_ADDR_MODE)
    {
        t[0].len = SFL_CMD_FRAME_SIZE_5;
    }
    else
    {
        t[0].len = SFL_CMD_FRAME_SIZE_4;
    }
    spi_message_add_tail(&t[0], &m);

     ENTER();
    /* No other command works just wake up First */
    /* Semaphore */
    down(&flash->lock);

#if defined (CONFIG_HW_MUTEXES)
    /* Lock the HW Mutex */
    if (hw_mutex_lock_interruptible(HW_MUTEX_NOR_SPI) != 0)
    {
        PRINTK("Flash Resunme failed - Can't lock HW mutex\n");
        goto resume_hwmutex_err;
    }
#endif

    if(wait_till_ready(flash) )
    {
        goto resume_err;
    }
    write_enable(flash);
    /* write enable requires wait till ready */

    if(wait_till_ready(flash) )
    {
        goto resume_err;
    }

    if (sfl_addr_mode == AVALANCHE_SPI_3_BYTE_ADDR_MODE)
    {
        /* Set up the opcode in the write buffer. */
        flash->command[0] = OPCODE_RELEASE_POWER_DOWN(flash->index);
        flash->command[1] = DUMMY_ADDRESS >> 16;
        flash->command[2] = DUMMY_ADDRESS >> 8;
        flash->command[3] = DUMMY_ADDRESS;
    }
    else
    {
        flash->command[0] = OPCODE_RELEASE_POWER_DOWN(flash->index);
        flash->command[1] = DUMMY_ADDRESS >> 24;
        flash->command[2] = DUMMY_ADDRESS >> 16;
        flash->command[3] = DUMMY_ADDRESS >> 8;
        flash->command[4] = DUMMY_ADDRESS;
    }

    DEBUG_SFL("waking Up form ........ Deep Sleep Hi :)\n");
    spi_sync(flash->spi, &m);

    if(wait_till_ready(flash) )
    {
        goto resume_err;
    }


resume_err:
#if defined (CONFIG_HW_MUTEXES)
    /* Un-lock the HW Mutex */
    hw_mutex_unlock(HW_MUTEX_NOR_SPI);

resume_hwmutex_err:
#endif
    /* Semaphore */
    up(&flash->lock);
    return;
}

/*
 * sfl_sfi_transfer Client transfer function used in SFI mode of
 * operation.
 * On success returns number of bytes transfered otherwise 0.
 */
int sfl_sfi_transfer(struct spi_device *spi, struct spi_transfer *t)
{
    struct sfl_sfi_transfer_hdr_t *sfi_hdr = NULL;

    struct avalanche_sfi_dev_info_t *sfi =
        (struct avalanche_sfi_dev_info_t *)spi->controller_data;

    ENTER();
    if(t->rx_buf != NULL )
    {
       DEBUG_SFL("t->rx_buf (sfi header) is 0x%x\n",(unsigned)t->rx_buf );
       sfi_hdr = (struct sfl_sfi_transfer_hdr_t*)(t->rx_buf);
    }
    else
    {
        DEBUG_SFL("t->rx_buf is NULL 0x%x\n",(unsigned)t->rx_buf );
        return 0;
    }

    if(likely(sfi_hdr->rx_buff))
    {
        unsigned len = sfi_hdr->len;
        unsigned i = 0;
        volatile sfi_read_buf_t *d_ptr = (volatile sfi_read_buf_t*)(sfi_hdr->rx_buff);
        volatile sfi_read_buf_t *s_ptr = (volatile sfi_read_buf_t*)(sfi->sfi_base + (unsigned)(sfi_hdr->rx_addr));

        DEBUG_SFL("Doing RX \n");

        DEBUG_SFL("Copyng the data to rx_bf [d_ptr=0x%0x s_ptr=0x%0x] \n",(unsigned) d_ptr,(unsigned) s_ptr);

        /* Check alignment of requested address */
        if((((unsigned)s_ptr) & 0x03) == 0)
        {
            DEBUG_SFL("Source address is 4 byte aligned, read in %d-byte chunks (until last, left over bytes)\n",sizeof(sfi_read_buf_t));
            while(len>=sizeof(sfi_read_buf_t))
            {
                d_ptr[i] = s_ptr[i];
                i++;
                len-=sizeof(sfi_read_buf_t);
                DEBUG_SFL("Read %d bytes (left %d)\n",sizeof(sfi_read_buf_t),len);
            }
        }

        /* Read the rest of data in 4 byte chunks */
        if(len)
        {
            /* Convert to unsigned pointers */
            volatile unsigned *s_ptr_i = (volatile unsigned *)&s_ptr[i];
            volatile unsigned *d_ptr_i = (volatile unsigned *)&d_ptr[i];

            /* Check alignment of requested address */
            if((((unsigned)s_ptr) & 0x03) == 0)
            {
                DEBUG_SFL("Source address is 4 byte aligned, read in 4-byte chunks (%d bytes)\n",len);

                i=0;
                /* Read the rest of data in 4 byte chunks */
                while(len>=sizeof(unsigned))
                {
                    d_ptr_i[i] = s_ptr_i[i];

                    i++;
                    len-=sizeof(unsigned);
                    DEBUG_SFL("Read 4 bytes (left %d)\n",len);
                }
            }

            if(len)
            {
                volatile unsigned char *s_ptr_c = (volatile unsigned char *)&s_ptr_i[i];
                volatile unsigned char *d_ptr_c = (volatile unsigned char *)&d_ptr_i[i];

                DEBUG_SFL("Read the rest of data in 1 byte chunks (%d bytes)\n", len);

                i=0;

                /* Read the left-overs */
                do
                {
                    d_ptr_c[i]=s_ptr_c[i];
                    i++;
                    len--;
                    DEBUG_SFL("Read 1 byte (left %d)\n",len);
                } while(len);

            }
        }
        DEBUG_SFL("\nlength: %d\n", sfi_hdr->len);
        return (int)(sfi_hdr->len);
    }

    return 0;
}

/**************** Helper functions used by probe ****************/
/* function to read device id */
static void sfl_read_id(struct spi_device *spi, int index, unsigned int id_size, unsigned char* id)
{
    unsigned char old_mode = 0;
    unsigned char cmd = OPCODE_READ_DEV_ID(index);

    if(sfl_sfi_mode)
        SFI_SET_MODE(spi, AVALANCHE_CORE_SPI_MODE, old_mode);

#if defined (CONFIG_HW_MUTEXES)
    /* Lock the HW Mutex */
    if (hw_mutex_lock_interruptible(HW_MUTEX_NOR_SPI) != 0)
    {
        PRINTK("Flash Read ID failed - Can't lock HW mutex\n");
        return;
    }
#endif

    spi_write_then_read(spi, &cmd, 1, id, id_size);

#if defined (CONFIG_HW_MUTEXES)
    /* Un-lock the HW Mutex */
    hw_mutex_unlock(HW_MUTEX_NOR_SPI);
#endif

    return;
}

#if defined (CONFIG_MACH_PUMA5)

/* returns 1 on successful s25f0xxx detection */
static int s25fl0xxx_probe(struct spi_device   *spi, int index)
{
    unsigned char old_mode = 0;
    unsigned char  id[3];

    if(sfl_sfi_mode)
        SFI_SET_MODE(spi, AVALANCHE_CORE_SPI_MODE, old_mode);

    sfl_read_id(spi, index, 3, id);

    /* check manufacturer */
    if(id[0] != VENDOR_SPANSION_ID)
        return 0;

    if(id[2] != (sfl_data[index].id & 0xFF))
        return 0;

    return 1;
}

/* returns 1 on successful m25flxxxx detection */
static int m25flxxxx_probe(struct spi_device   *spi, int index)
{
    unsigned char old_mode = 0;
    unsigned char  id[3];

    if(sfl_sfi_mode)
        SFI_SET_MODE(spi, AVALANCHE_CORE_SPI_MODE, old_mode);

    sfl_read_id(spi, index, 3, id);

    /* check manufacturer */
    if(id[0] != VENDOR_NUMONYX_ID)
        return 0;

    if(id[1] != ((sfl_data[index].id >> 8) & 0xFF))
        return 0;

    if(id[2] != (sfl_data[index].id & 0xFF))
        return 0;

    return 1;
}

/* returns 1 on successful mx25lxxxx  detection */
static int mx25lxxxx_probe(struct spi_device   *spi, int index)
{
    unsigned char old_mode = 0;
    unsigned char  id[3];

    if(sfl_sfi_mode)
        SFI_SET_MODE(spi, AVALANCHE_CORE_SPI_MODE, old_mode);

    sfl_read_id(spi, index, 3, id);

    /* check manufacturer */
    if(id[0] != VENDOR_MACRONIX_ID)
        return 0;

    if(id[2] != (sfl_data[index].id & 0xFF))
        return 0;

    return 1;
}

/* returns 1 on successful w25qxxxx detection */
static int w25qxxxx_probe(struct spi_device   *spi, int index)
{
    unsigned char old_mode = 0;
    unsigned char  id[3];

    if(sfl_sfi_mode)
        SFI_SET_MODE(spi, AVALANCHE_CORE_SPI_MODE, old_mode);

    sfl_read_id(spi, index, 3, id);

    /* check manufacturer */
    if(id[0] != VENDOR_WINBOND_ID)
        return 0;

    if(id[2] != (sfl_data[index].id & 0xFF))
        return 0;

    return 1;
}

/* returns 1 on successful s25vfxxxx detection */
static int s25vfxxxx_probe(struct spi_device   *spi, int index)
{
    unsigned char old_mode = 0;
    unsigned char  id[3];

    if(sfl_sfi_mode)
        SFI_SET_MODE(spi, AVALANCHE_CORE_SPI_MODE, old_mode);

    sfl_read_id(spi, index, 3, id);

    /* check manufacturer */
    if(id[0] != VENDOR_SST_ID)
        return 0;

    if(id[2] != (sfl_data[index].id & 0xFF))
        return 0;

    return 1;
}

/* returns 1 on successful s25fl128p detection */
static int s25fl128p_probe(struct spi_device   *spi, int index)
{
    unsigned char old_mode = 0;
    unsigned char  id[5];

    if(sfl_sfi_mode)
        SFI_SET_MODE(spi, AVALANCHE_CORE_SPI_MODE, old_mode);

    sfl_read_id(spi, index, 5, id);

    /* check manufacturer */
    if(id[0] != VENDOR_SPANSION_ID)
        return 0;

    /* check if it is s25fl128p */
    if(id[2] != (sfl_data[index].id & 0xFF))
        return 0;

    /* based on extended device id fill sector and n_sector info */
    switch(id[4])
    {
        case 0:
            sfl_data[index].sector_size     = S25FL128P00_SECT_SIZE;
            sfl_data[index].n_sectors       = N_S25FL128P00_SECTORS;
            sfl_data[index].prot_ratio  = S25FL128P00_PROT_RATIO;
            break;

        case 1:
        default:
            sfl_data[index].sector_size     = S25FL128P01_SECT_SIZE;
            sfl_data[index].n_sectors       = N_S25FL128P01_SECTORS;
            sfl_data[index].prot_ratio  = S25FL128P01_PROT_RATIO;
            break;
    }

    sfl_data[index].regions[0].offset       =   0;
    sfl_data[index].regions[0].erasesize    = sfl_data[index].sector_size;
    sfl_data[index].regions[0].numblocks    = sfl_data[index].n_sectors;

    return 1;
}

/* returns 1 on successful s25fl256s detection */
static int s25fl256s_probe(struct spi_device   *spi, int index)
{
    unsigned char old_mode = 0;
    unsigned char  id[5];

	if(sfl_sfi_mode)
	    SFI_SET_MODE(spi, AVALANCHE_CORE_SPI_MODE, old_mode);

	sfl_read_id(spi, index, 5, id);

	/* check manufacturer */
	if(id[0] != VENDOR_SPANSION_ID)
		return 0;

	/* check if it is s25fl256s */
    if(id[2] != (sfl_data[index].id & 0xFF))
        return 0;
    
    return 1;
}

/* returns 1 on successful n25qxxxx detection */
static int n25qxxxx_probe(struct spi_device   *spi, int index)
{
    unsigned char old_mode = 0;
    unsigned char  id[5];

	if(sfl_sfi_mode)
	    SFI_SET_MODE(spi, AVALANCHE_CORE_SPI_MODE, old_mode);

	sfl_read_id(spi, index, 5, id);

	/* check manufacturer */
	if(id[0] != VENDOR_NUMONYX_ID)
		return 0;

	/* check if it is n25q256 */
    if(id[2] != (sfl_data[index].id & 0xFF))
        return 0;     

    return 1;
}
#else /* CONFIG_MACH_PUMA6  For Puma-6 SoC */
/* returns 1 on successful m25flxxxx detection */
static int n25qxxxx_probe(struct spi_device   *spi, int index)
{
    unsigned char old_mode = 0;
    unsigned char  id[5];

    if(sfl_sfi_mode)
        SFI_SET_MODE(spi, AVALANCHE_CORE_SPI_MODE, old_mode);

    sfl_read_id(spi, index, 5, id);

    /* check manufacturer */
    if(id[0] != VENDOR_NUMONYX_ID)
        return 0;

    if(id[1] != ((sfl_data[index].id >> 8) & 0xFF))
        return 0;

    if(id[2] != (sfl_data[index].id & 0xFF))
        return 0;

    return 1;
}

#endif

static int sfl_probe(struct spi_device *spi)
{
    unsigned int i, index;
    struct sfl  *flash;
    struct flash_platform_data  *data;
    int flash_found = 0, ret;
    struct avalanche_sfi_dev_info_t *sfi = NULL;
    struct avalanche_spi_platform_data *pdata = NULL;
    ENTER();
    data = spi->dev.platform_data;


    for(i = 0; i < ARRAY_SIZE(sfl_data); i++)
    {
        if(sfl_data[i].probe)
        {
            if(sfl_data[i].probe(spi,i))
            {
                printk("Serial Flash [Bus:%1d CS:%1d] : ", spi->master->bus_num, spi->chip_select);
                printk("%s %dKB, %3d sectors each %3dKB \n",
                    sfl_data[i].name,
                    (sfl_data[i].sector_size*sfl_data[i].n_sectors/1024),
                    sfl_data[i].n_sectors, (sfl_data[i].sector_size/1024));
                flash_found     = 1;
                index           = i;
                break;
            }
        }
    }

    if(!flash_found)
    {
        printk("Serial Flash [Bus:%1d CS:%1d] : ", spi->master->bus_num, spi->chip_select);
        printk("No device found\n");
        return -ENODEV;
    }
    flash           = &(flash_info[spi->chip_select]);
    flash->index    = index;
    flash->spi      = spi;

    init_MUTEX(&flash->lock);
    dev_set_drvdata(&spi->dev, flash);
    flash->mtd.name         = (char*)(spi->modalias);
    flash->mtd.type         = MTD_NORFLASH;
    flash->mtd.writesize    = SFL_PAGESIZE(flash->index);
    flash->mtd.flags        = MTD_CAP_NORFLASH;
    flash->mtd.size         = SFL_SIZE(flash->index);
    flash->mtd.erasesize    = SFL_SECTOR_SIZE(flash->index);
    flash->mtd.numeraseregions = NUM_REGIONS(flash->index);
    flash->mtd.eraseregions = ERASE_REGION(flash->index);
    flash->mtd._erase        = sfl_erase;
    flash->mtd._read         = sfl_read;
    flash->mtd._write        = sfl_write;
    flash->mtd._lock         = sfl_lock;
    flash->mtd._unlock       = sfl_unlock;
    flash->mtd._suspend      = sfl_suspend;
    flash->mtd._resume       = sfl_resume;


    DEBUG_SFL("mtd .name = %s, .size = 0x%.8x (%uM) "
          ".erasesize = 0x%.8x (%uK) .numeraseregions = %d\n",
          sfl_data[flash->index].name,  flash->mtd.size,
          flash->mtd.size / (1024*1024),flash->mtd.erasesize,
          flash->mtd.erasesize / 1024,  flash->mtd.numeraseregions);

    /* Set the Addrssing mode. 3 or 4 byte addressing mode */
    pdata = spi->master->dev.parent->platform_data;
    sfl_addr_mode = pdata->addr_mode;
    DEBUG_SFL("Serial Flash Addressing mode = %d bytes.\n",sfl_addr_mode);

    /*
     * Initialise the controller data  set Read/Fast_read, write ,
     * read mode(Noraml/Fast) for SFI transfer
     */
    sfi =( struct avalanche_sfi_dev_info_t *)spi->controller_data;
    if(  sfi != NULL )
    {
        sfi->initialized = 0;
        sfi->mode       = AVALANCHE_CORE_SPI_MODE;
        sfi->write_cmd  = OPCODE_PAGE_PROGRAM(flash->index);
        if(sfl_fast_read == SFL_NORMAL_READ)        /* set from pre compile in .config */
        {
            sfi->dual_read          = SFL_NORMAL_READ;
            sfi->num_dummy_bytes    = NORMAL_READ_NUM_DUMMY_BYTES;
            /* Num adddrss byte count starts form zero i,e -1  */
            sfi->num_addr_bytes     = (SFL_NUM_ADDR_BYTES - 1);
            sfi->read_cmd           = OPCODE_READ(flash->index);
        }
        else if(sfl_fast_read == SFL_FAST_READ || ( OPCODE_DUAL_FAST_READ(flash->index) == OPCODE_FAST_READ(flash->index) ) )
        {
            /* Fast Read CMD values */
            sfi->dual_read          = SFL_NORMAL_READ;
            sfi->num_dummy_bytes    = FAST_READ_NUM_DUMMY_BYTES;
            /* Num adddrss byte count starts form zero i, e -1 */
            sfi->num_addr_bytes     = (SFL_NUM_ADDR_BYTES -1);
            sfi->read_cmd           = OPCODE_FAST_READ(flash->index);
        }
        else
        {
            /* Dual Fast Read CMD values */
            sfi->dual_read          = 1;
            sfi->num_dummy_bytes    = FAST_READ_NUM_DUMMY_BYTES;
            /* Num adddrss byte count starts form zero i, e -1 */
            sfi->num_addr_bytes     = (SFL_NUM_ADDR_BYTES -1);
            sfi->read_cmd           = OPCODE_DUAL_FAST_READ(flash->index);
        }
        sfi->sfi_transfer = sfl_sfi_transfer;
        DEBUG_SFL("SFI = 0x%0x  Operating in SFI mode with write_cmd = 0x%x, \
               read_cmd = 0x%x dual_read = %d num_dummy_bytes = %d \
               num_addr_bytes %d\n",(unsigned int)sfi,
               (unsigned int)sfi->write_cmd,(unsigned int)sfi->read_cmd,
               sfi->dual_read, sfi->num_dummy_bytes,sfi->num_addr_bytes);
    }
    else
    {
        /* FIXME: is this required ??? */
        sfl_sfi_mode = AVALANCHE_CORE_SPI_MODE;
        DEBUG_SFL(" Operating in Core SPI mode\n");
    }


    /* partitions should match sector boundaries; and it may be good to
     * use readonly partitions for writeprotected sectors (BP2..BP0).
     */
    ret = mtd_device_parse_register(&flash->mtd, NULL, 0,
				    data ? data->parts : NULL,
				    data ? data->nr_parts : 0);
    return ret ? (-ENODEV) : sfl_unlockall(&flash->mtd);
}

static int sfl_remove(struct spi_device *spi)
{
    struct  sfl *flash = dev_get_drvdata(&spi->dev);
    int     status;
    ENTER();
    /* Clean up MTD stuff. */
    status = mtd_device_unregister(&flash->mtd);

    if (status == 0) {
        kfree(flash);
    }
    return 0;
}


static struct spi_driver sfl_mtd_driver = {
    .driver = {
        .name       = "spansion",
        .bus        = &spi_bus_type,
        .owner      = THIS_MODULE,
    },
    .probe   = sfl_probe,
    .remove  = sfl_remove,
};

static struct spi_driver sfl1_mtd_driver = {
    .driver = {
        .name       = "spansion1",
        .bus        = &spi_bus_type,
        .owner      = THIS_MODULE,
    },
    .probe   = sfl_probe,
    .remove  = sfl_remove,
};

static int sfl_init( void )
{
    int ret0, ret1;
    ENTER();
#ifdef CONFIG_MTD_SFL_MODULE
    if( sfi_mode == AVALANCHE_CORE_SPI_MODE ||
         sfi_mode == AVALANCHE_SFI_MODE ) {
        sfl_sfi_mode = sfi_mode;
    }else{
        DEBUG_SFL("setting sfl_sfi_mode to default SFI MODE\n");
        sfl_sfi_mode = AVALANCHE_SFI_MODE;
    }
    DEBUG_SFL("sfl_sfi_mode = %d\n",sfl_sfi_mode);

    if( fast_read == SFL_NORMAL_READ ||
        fast_read == SFL_FAST_READ ||
        fast_read == SFL_DUAL_FAST_READ ){
        sfl_fast_read = fast_read;
    }else{
        DEBUG_SFL("setting sfl_fast_read to default Fast read Mode\n");
        sfl_fast_read = SFL_FAST_READ ;
    }

    DEBUG_SFL("sfl_fast_read = %d\n",sfl_fast_read);
#endif
    ret0 = spi_register_driver(&sfl_mtd_driver);
    ret1 = spi_register_driver(&sfl1_mtd_driver);

    return (ret0 == 0) ? ret1 : ret0;
}


static void sfl_exit( void )
{
    ENTER();
    spi_unregister_driver(&sfl_mtd_driver);
    spi_unregister_driver(&sfl1_mtd_driver);
}


module_init(sfl_init);
module_exit(sfl_exit)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("R.Srinath/Mansoor Ahmed/Hai Shalom - Texas Instruments Inc.");
MODULE_DESCRIPTION("MTD SPI driver for Serial flashes");
module_param(sfi_mode,int, S_IRUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(sfi_mode,"\n\t\t Set sfi_mode = 1 for SFI mode, sfi_mode = 0 for CORE SPI mode<Range:0/1> Default is SFI mode");
module_param(fast_read,int, S_IRUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(fast_read,"\n\t\t Set fast_read = 0 for NORMAL READ, fast_read  = 1 for FAST READ <Range:0/1> Default is FAST READ");

