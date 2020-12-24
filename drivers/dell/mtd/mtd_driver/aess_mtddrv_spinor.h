/*
 * $RCSfile: aess_mtddrv_spinor.c,v $
 * $Revision:  $
 * $Date:  $
 * $Author: Watt Wu $
 *
 * WPCM450 On chip SPI NOR sensor driver.
 *  
 * Copyright (C) 2007 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */


#if !defined (WPCM450_SPINOR_H)
#define WPCM450_SPINOR_H
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>

#ifdef WPCM450_SPINOR_C

#include <asm/arch/map.h>

/* For temporary use, because no type.h now */
/* ============================================ */
#define    hw_char         *(volatile unsigned char *)
#define    hw_short        *(volatile unsigned short *)

#define VPlong(x)   			(*(volatile unsigned int *)(x))
#define VPshort(x) 			(*(volatile unsigned short *)(x))
#define VPchar(x)  			(*(volatile unsigned char *)(x))

/* type define */
typedef    unsigned long   uint32;
typedef    unsigned short  uint16;
typedef    unsigned char   uint8;

typedef unsigned char       UINT8;
typedef unsigned short int  UINT16;
typedef unsigned long int   UINT32;

typedef enum {
	FALSE = 0,
	TRUE  = 1
} BOOL;

#define STATUS_OK       0
#define STATUS_BUSY     1

#define ALIGN_128       0x7F
#define ALIGN_512       0x1FF
#define SHIFT_128       7
#define SHIFT_512       9
#define SHIFT_4K        12
#define BITS_7_0        0xFF
#define BITS_15_8       0xFF00
#define BITS_23_16      0xFF0000

#define SHIFT_256           8
#define SHIFT_512           9
#define SHIFT_1024          10

/*-------------------
 * BIT operation macros
 *-------------------*/
#define SET_BIT(reg, bit)       ((reg)|=(0x1<<(bit)))
#define READ_BIT(reg, bit)      ((reg)&(0x1<<(bit)))
#define CLEAR_BIT(reg, bit)     ((reg)&=(~(0x1<<(bit))))
#define IS_BIT_SET(reg, bit)    (((reg)&(0x1<<(bit)))!=0)

/*---------------------
 * FIELD operation macros
 *---------------------*/
// Get contents of "field" from "reg"
#define GET_FIELD(reg, field)   \
    GET_FIELD_SP(field##_S, field##_P,reg)
    
// Set contents of "field" in "reg" to value"
#define SET_FIELD(reg, field, value)    \
    SET_FIELD_SP(field##_S, field##_P,reg, value)
    
// Auxiliary macro: Get contents of field using size and position
#define GET_FIELD_SP(size, position, reg)    ((reg>>position)&((1<<size)-1))

// Auxiliary macro: Set contents of field using fields size and position
#define SET_FIELD_SP(size, position, reg ,value)    \
    (reg = (reg &(~((((uint32)1<<size)-1)<<position)))|((uint32)value<<position))

/*---------------------
 * MASK operation macros
 *---------------------*/
#define SET_MASK(reg, bit_mask)       ((reg)|=(bit_mask))
#define READ_MASK(reg, bit_mask)      ((reg)&(bit_mask))
#define CLEAR_MASK(reg, bit_mask)     ((reg)&=(~(bit_mask)))
#define IS_MASK_SET(reg, bit_mask)    (((reg)&(bit_mask))!=0)

/* ============================================= */

/*----- Flash interface unit (FIU) -----*/
//#define FLASH_BASE			0x40000000

/* GCR base address */
#define GCR_BA      WPCM450_VA_GCR
#define MFSEL1      VPlong(GCR_BA + 0x00C)

#define MF_SCS3SEL_BIT   5
#define MF_SCS2SEL_BIT   4
#define MF_SCS1SEL_BIT   3

/* FIU base address */
#define FIU_REGS_BASE   WPCM450_VA_FIU

#define FIU_CFG         hw_char((FIU_REGS_BASE + 0x0))  // FIU configuration                   
#define BURST_CFG       hw_char((FIU_REGS_BASE + 0x1))  // Burst configuration                 
#define RESP_CFG        hw_char((FIU_REGS_BASE + 0x2))  // FIU response configuration          
#define CFBB_PROT       hw_char((FIU_REGS_BASE + 0x3))  // Core firmware boot block protection 
#define FWIN1_LOW       hw_short((FIU_REGS_BASE+ 0x4))  // Flash Access Windows 1, low limit   
#define FWIN1_HIGH      hw_short((FIU_REGS_BASE+ 0x6))  // Flash Access Windows 1, high limit
#define FWIN2_LOW       hw_short((FIU_REGS_BASE+ 0x8))  // Flash Access Windows 2, low limit   
#define FWIN2_HIGH      hw_short((FIU_REGS_BASE+ 0xA))  // Flash Access Windows 2, high limit
#define FWIN3_LOW       hw_short((FIU_REGS_BASE+ 0xC))  // Flash Access Windows 3, low limit   
#define FWIN3_HIGH      hw_short((FIU_REGS_BASE+ 0xE))  // Flash Access Windows 3, high limit
#define PROT_LOCK       hw_char((FIU_REGS_BASE + 0x10)) // Protection lock                     
#define PROT_CLEAR      hw_char((FIU_REGS_BASE + 0x11)) // Protection and lock clear          

#define	SPI_FL_CFG      hw_char((FIU_REGS_BASE+0x14))   // SPI flash configuration
#define	UMA_CODE        hw_char((FIU_REGS_BASE+0x16))   // UMA code byte
#define	UMA_AB0         hw_char((FIU_REGS_BASE+0x17))   // UMA Address Byte 0
#define	UMA_AB1         hw_char((FIU_REGS_BASE+0x18))   // UMA Address Byte 1
#define	UMA_AB2         hw_char((FIU_REGS_BASE+0x19))   // UMA Address Byte 2
#define	UMA_DB0         hw_char((FIU_REGS_BASE+0x1A))   // UMA Data Byte 0
#define	UMA_DB1         hw_char((FIU_REGS_BASE+0x1B))   // UMA Data Byte 1
#define	UMA_DB2         hw_char((FIU_REGS_BASE+0x1C))   // UMA Data Byte 2
#define	UMA_DB3         hw_char((FIU_REGS_BASE+0x1D))   // UMA Data Byte 3
#define	UMA_CTS         hw_char((FIU_REGS_BASE+0x1E))   // UMA control and status
#define	UMA_ECTS         hw_char((FIU_REGS_BASE+0x1F))  // UMA extended control and status

#define WIN_LIMIT_4K_SHIFT  12

/* FIU & SPI registers fields */
/* FIU_CFG fields */
#define FIU_FL_SIZE_S        8
#define FIU_FL_SIZE_P        0

/* BURST_CFG fields */
#define FIU_W_BURST_S        2
#define FIU_W_BURST_P        4
#define FIU_R_BURST_S        2
#define FIU_R_BURST_P        0


/* RESP_CFG fields */
#define FIU_INT_EN_BIT       1
#define FIU_IAD_EN_BIT       0

#define FIU_WIN_LOW_S       13
#define FIU_WIN_LOW_P        0

#define FIU_WIN_HIGH_S      15
#define FIU_WIN_HIGH_P       0

/* PROT_LOCK register fields */
#define FIU_PRM_LK_BIT       4
#define FIU_FWIN3_LK_BIT     2
#define FIU_FWIN2_LK_BIT     1
#define FIU_FWIN1_LK_BIT     0


/* SPI_FL_CFG fields */
#define SPI_F_READ_BIT       6
#define SPI_DEV_SIZE_S       6
#define SPI_DEV_SIZE_P       0

/* UMA_CTS fields */
#define UMA_EXEC_DONE_BIT    7
#define UMA_DEV_NUM_S        2
#define UMA_DEV_NUM_P        5
#define UMA_A_SIZE_BIT       3
#define UMA_D_SIZE_S         3
#define UMA_D_SIZE_P         0
#define UMA_RD_WR_BIT        4

/* Status Register bits. */
#define	SR_WIP			1	/* Write in progress */
#define	SR_WEL			2	/* Write enable latch */
#define	SR_BP0			4	/* Block protect 0 */
#define	SR_BP1			8	/* Block protect 1 */
#define	SR_BP2			0x10	/* Block protect 2 */
#define	SR_SRWD			0x80	/* SR write protect */

struct spinor_chip {
	struct semaphore	lock;
};

typedef struct flash_t
{
    UINT32 mf_id;
    UINT32 capacity;
    char*  name;
    UINT32 chip_size;
    UINT32 block_size;
} flash_t;

/*******************************************************************
* Typedef Definitions
*******************************************************************/
typedef enum _spi_w_burst_t {
  SPI_ONE_BYTE_W_BURST = 0,
  SPI_FOUR_BYTE_W_BURST = 2
} SPI_w_burst_t;

typedef enum _spi_r_burst_t {
  SPI_ONE_BYTE_R_BURST = 0,
  SPI_SIXTEEN_BYTE_R_BURST = 3
} SPI_r_burst_t;

typedef enum _spi_w_protect_int_t {
  SPI_W_PROTECT_INT_DISABLE = 0,
  SPI_W_PROTECT_INT_ENABLE = 1
} SPI_w_protect_int_t;

typedef enum _spi_incorect_access_int_t {
  SPI_INCORECT_ACCESS_INT_DISABLE = 0,
  SPI_INCORECT_ACCESS_INT_ENABLE = 1
} SPI_incorect_access_int_t;

typedef enum _spi_fast_read_t {
  SPI_FAST_READ_DISABLE = 0,
  SPI_FAST_READ_ENABLE = 1
} SPI_fast_read_t;


#else 

extern int wpcm450_spinor_erase(struct mtd_info *mtd, struct erase_info *instr);
extern int wpcm450_spinor_write(struct mtd_info *mtd, loff_t to, size_t len,	size_t *retlen, const u_char *buf);
extern int wpcm450_spinor_read(struct mtd_info *mtd, loff_t from, size_t len,size_t *retlen, u_char *buf);
extern int aess_write_spi_nor_flash( unsigned char *data_addr , int len );


#endif      /* WPCM450_SPINOR_C */



#endif      /* WPCM450_SPINOR_H */
