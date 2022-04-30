/*
 * linux/include/asm/arch/sp_sflash.h
 *
 *  Copyright (C) ST MICROELECTRONICS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General PGublic License as published by
 * the Free Software Foundation; either version 2 of the License , or
 * ( at your option )any later version.
 *
 * This program is distributed in the hope that it will be useful , 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not , write to the Free Software
 * Foundation , Inc. , 59 Temple Place , Suite 330 , Boston , MA  02111-1307 USA
 * AUTHOR :  Siva Borra <siva.borra@st.com> */


#ifndef SP_SFLASH_H
#define SP_SFLASH_H

#include <asm/arch/hardware.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include "flash_chars.h"

#define M25P64
#define MAX_NUM_FLASH_CHIP		4
/* Max possible slots in the Spearhead */

/* These should move to mtd.h later    */

#define MTD_CAP_SFLASH             (MTD_ERASEABLE)

#define SFLASH_M25P10_PAGE_SHIFT   0x7

#define SFLASH_M25P64_PAGE_SHIFT   0x8 

#define SFLASH_GET_DEVID() (SFLASH_DEVID_M25P64) 

#define SMI_BASE      IO_ADDRESS(0xFC000000)

#define FLASH_START_ADDRESS IO_ADDRESS(0xf8000000)

/*  4 MBytes will be reserved for the MTD */
#define MTD_PARTITION_OFFSET	0x00400000 

#define SMIBANK0_BASE (FLASH_START_ADDRESS + 0x00000000)

#define SMIBANK1_BASE (FLASH_START_ADDRESS + 0x01000000)

#define SMIBANK2_BASE (FLASH_START_ADDRESS + 0x02000000)

#define SMIBANK3_BASE (FLASH_START_ADDRESS + 0x03000000)

#define FLASH_BANK_SIZE 0x00800000

#define CHECK_TFF 0x00000100

#define CHECK_WCF 0x00000200

#define BANK0 0
#define BANK1 1
#define BANK2 2
#define BANK3 3

typedef struct SMIController
{

  unsigned int SMI_CR1;	/* 1000F000 */
  unsigned int SMI_CR2;	/* 1000F004 */
  unsigned int SMI_SR; 	/* 1000F008 */
  unsigned int SMI_TR; 	/* 1000F00C */
  unsigned int SMI_RR; 	/* 1000F010 */

} SMIController;

static struct mtd_info *stwsf_mtd[MAX_NUM_FLASH_CHIP];

#define SMICntl ((volatile struct SMIController*) (SMI_BASE))

#define ST_M25P64     0x00172020

/* STATUS_REG */
#define INT_WCF_CLR   0xFFFFFDFF /* clear: WCF clear */
#define INT_TFF_CLR   0xFFFFFEFF /* clear: TFF clear */
#define WIP_BIT       0x00000001 /* WIP Bit of SPI SR on SMI SR */
#define WEL_BIT       0x00000002 /* WEL Bit of SPI SR on SMI SR */
#define RSR           0x00000005 /* Read Status regiser */
#define TFF           0x00000100 /* Transfer Finished FLag */
#define WCF           0x00000200 /* Transfer Finished FLag */
#define ERF1          0x00000400 /* Error Flag 1 */
#define ERF2          0x00000800 /* Error Flag 2 */
#define WM0           0x00001000 /* WM Bank 0 */
#define WM1           0x00002000 /* WM Bank 1 */
#define WM2           0x00004000 /* WM Bank 2 */
#define WM3           0x00008000 /* WM Bank 3 */

/* CONTROL REG 1 */
#define BANK_EN       0x0000000F /* enables all banks */
#define DSEL_TIME     0x00000050 /* Deselect time 5+1 SMI_CK periods */
#define PRESCAL5      0x00000500 /* AHB_CK prescaling value */
#define PRESCALA      0x00000A00 /* AHB_CK prescaling value */
#define SW_MODE       0x10000000 /* enables SW Mode */
#define WB_MODE       0x20000000 /* Write Burst Mode */
#define FAST_MODE     0x00008000 /* Fast Mode */

/* CONTROL REG 2 */
#define RD_STATUS_REG 0x00000400 /* reads status reg */
#define WE            0x00000800 /* Write Enable */
#define BANK0_SEL     0x00000000 /* Select Bank0 */
#define BANK1_SEL     0x00001000 /* Select Bank1 */
#define BANK2_SEL     0x00002000 /* Select Bank2 */
#define BANK3_SEL     0x00003000 /* Select Bank3 */
#define SEND          0x00000080 /* Send data */
#define TX_LEN_1      0x00000001 /* data length = 1 byte */
#define TX_LEN_2      0x00000002 /* data length = 2 byte */
#define TX_LEN_3      0x00000003 /* data length = 3 byte */
#define TX_LEN_4      0x00000004 /* data length = 4 byte */
#define RX_LEN_1      0x00000010 /* data length = 1 byte */
#define RX_LEN_2      0x00000020 /* data length = 2 byte */
#define RX_LEN_3      0x00000030 /* data length = 3 byte */
#define RX_LEN_4      0x00000040 /* data length = 4 byte */
#define TFIE          0x00000100 /* Transmission Flag Interrupt Enable */
#define WCIE          0x00000200 /* Write Complete Interrupt Enable */

/* TR REG */

#define READ_ID       0x0000009F 

/* Read Identification for ST, Spansion Macronix and Atmel*/
int memory_indices[4]; /* For storing memory indices */

unsigned int read_ids[]={0x00000090, 0x00000015,READ_ID}; /* three groups */
/* SST and Atmel some part */

#define BULK_ERASE    0x000000C7   /* BULK erase */
#define SECTOR_ERASE  0x000000D8   /* SECTOR erase */
#define BANK_SIZE     64 * 1024    /* Specific Bank Size = 64KBytes */
#define WRITE_ENABLE  0x00000006   /* Wenable command to FLASH */

typedef enum {
    SF_READY,
    SF_READI,
    SF_WRITI,
    SF_ERASI,
    SF_SYNCI
} sf_state_t;

struct sflash_chip {
    spinlock_t chip_lock;
    wait_queue_head_t wq;
    sf_state_t state;
    unsigned int chip_shift;
    unsigned int page_shift;
    u_char *data_buf;
};


/* prototypes of the STW serial flash driver */

/************ Highlevel routines**************/

static int   
sflash_erase( struct mtd_info *,
	      struct erase_info *
	    );

static int   
sflash_read( struct mtd_info *,
	     loff_t ,
	     size_t ,
	     size_t *,
	     u_char *
           );

static int   
sflash_write( struct mtd_info *,
	      loff_t ,
	      size_t ,
	      size_t *,
	      const u_char *
	    );

static void  sflash_sync ( struct mtd_info * );

//static void  sflash_cleanup( void );

static int   sflash_init( void );

//static int   stwsf_chip_init( void );

static int   stwsf_chip_probe( struct mtd_info *, unsigned int );

static int   stwsf_get_chip_index(unsigned int );

//static int   stwsf_get_block_size( unsigned int );

//static int   stwsf_get_chip_size( unsigned int  );

static int   stwsf_chip_erase( void );

static void  stwsf_read_enable_sequence( void );

unsigned int getSectorID(unsigned int , int );


/************ Low level SMI routines **********************/

extern void smi_init(unsigned int);

extern void smi_prog(void);

extern void 
 smi_read(unsigned char*, unsigned char*, unsigned int, unsigned int);

extern int  
 smi_write(unsigned char*, unsigned char*, unsigned int, unsigned int);

extern int  smi_bank_erase(unsigned int);

extern int  smi_sector_erase(unsigned int, unsigned int);

extern void smi_test(void);

extern unsigned int smi_read_id(unsigned int bank);

extern unsigned int smi_read_sr(unsigned int bank);

unsigned int get_bulk_erase_cmd(unsigned int memId);

unsigned int get_sector_erase_cmd(unsigned int memId);

void smi_int_handler(void);

int smi_write_hacked ( unsigned int* src_addr,
                       unsigned int* dst_addr,
                       unsigned int length,
                       unsigned int bank
                 );


/******************** SMI routines end ****************************************/

#endif /* STW_SFLASH_H */

/* End of file - sp_sflash.h */

