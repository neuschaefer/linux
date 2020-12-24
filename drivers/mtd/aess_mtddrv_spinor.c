/*
 *  aess_mtddrv_spinor.c
 *
 *  Copyright (c) 2007 Avocent Corp.
 *
 *  Derived from drivers/mtd/nand/autcpu12.c
 *       Copyright (c) 2001 Thomas Gleixner (gleixner@autronix.de)
 *
 * $Id$
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Overview:
 *   This is a device driver for the NAND flash device found on the
 *   winbond wpcm450 board. It supports 64MB nand flash.
 */ 
#define IDRACLITE 1

#define WPCM450_SPINOR_C

#include <linux/slab.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <asm/sizes.h>
#include <linux/vmalloc.h>
#include <asm/arch/regs-gpio.h>


#include "aess_mtddrv_spinor.h"

/* Use SYSTEM_NAME defined in environment.dat to determine platform. 
(chtsai)
//#define CONFIG_WPCM450_SPINOR_EVB
//#define CONFIG_WPCM450_SPINOR_WHOVILLE
*/
#ifndef CONFIG_WPCM450_SPINOR_WHOVILLE
#define CONFIG_WPCM450_SPINOR_WHOVILLE
#endif

//#define WPCM450_MTD_SPINOR_DEBUG
#ifdef WPCM450_MTD_SPINOR_DEBUG
    #define DUMP_MSG(format, args...)       dump_msg(format, ## args)
#else
    #define DUMP_MSG(format, args...)
#endif

#ifdef CONFIG_MTD_PARTITIONS
#define	mtd_has_partitions()	(1)
#else
#define	mtd_has_partitions()	(0)
#endif


#define GPIO_REG_PORT2_CFG0_ADDR        WPCM450_GPIOREG(0x3c)
#define GPIO_REG_PORT2_CFG1_ADDR        WPCM450_GPIOREG(0x40)
#define GPIO_REG_PORT2_CFG2_ADDR        WPCM450_GPIOREG(0x44)
#define GPIO_REG_PORT2_DATAOUT_ADDR     WPCM450_GPIOREG(0x48)
#define GPIO_REG_PORT2_DATAIN_ADDR      WPCM450_GPIOREG(0x4c)
#define GPIO_GPIO40						(8)


#define WINBOND_MF_ID        0xEF
#define W25P32_CAPACITY_ID   0x16

#define ATMEL_MF_ID          0x1F
#define AT26DF_DEV_ID        0x40
#define AT26DF08_CAPACITY_ID 0x05

#define SPI_READ_JEDEC_ID_CMD    0x9F
#define SPI_WRITE_ENABLE_CMD     0x06
#define SPI_WRITE_DISABLE_CMD    0x04 
#define SPI_READ_STATUS_REG_CMD  0x05
#define SPI_WRITE_STATUS_REG_CMD 0x01
#define SPI_READ_DATA_CMD        0x03
#define SPI_PAGE_PRGM_CMD        0x02
#define SPI_SECTOR_ERASE_CMD     0xD8
#define SPI_READ_PID_CMD         0x90
#define SPI_SEQU_PRGM_CMD        0xAD

/*
 * MTD structure for wpcm450 board
 */
static struct mtd_info *wpcm450_spinor_mtd = NULL;

#ifdef CONFIG_WPCM450_SPINOR_EVB
    #define MAX_FLASH_BANKS	4
#else
    #ifdef CONFIG_WPCM450_SPINOR_WHOVILLE
        #define MAX_FLASH_BANKS	1
    #endif
#endif

#define FLASH_SIZE  SZ_64M
#define MAX_READY_WAIT_COUNT	1000000

#define CAP_ID_1Mbits       0x11
#define CAP_ID_2Mbits       0x12
#define CAP_ID_4Mbits       0x13
#define CAP_ID_8Mbits       0x14
#define CAP_ID_16Mbits      0x15
#define CAP_ID_32Mbits      0x16
#define CAP_ID_64Mbits      0x17
#define CAP_ID_128Mbits     0x18

#define MF_ID_ATMEL         0x1F
#define MF_ID_WINBOND       0xEF
#define MF_ID_MACRONIX      0xC2
#define MF_ID_SPANSION      0x01
#define MF_ID_STM           0x20
#define MF_ID_SST           0xBF

// Atmel ICs
#define AT26DF08_DID        0x4500  /* low byte is product version; ignore it */
#define AT26DF16_DID        0x4600  /* low byte is product version; ignore it */
// WINBOND IC's
#define W25P16_DID          0x2015
#define W25P32_DID          0x2016
// MACRONIX IC's
#define MX25L16_DID         0x2015
#define MX25L128_DID        0x2018
// SPANSION IC's
#define S25FL016_DID        0x0214      // ?
#define S25FL128_DID        0x2018
#define S25FL128_CAPACITY_ID     0x17

// ST ICs
#define M25P16_DID          0x2015
#define M25P32_DID          0x2016
//SST ICs
#define SST25VF016_DID      0x2541

enum SPI_FLASH_DEV_TYPE
{
    WINDBOND_W25P16 = 0,
    WINDBOND_W25P32,
    ATMEL_AT26DF08,
    ATMEL_AT26DF16,
    MACRONIX_MX25L16,
    MACRONIX_MX25L128,
    PANSION_S25FL016,
    PANSION_S25FL128,
    ST_M25P16,
    ST_M25P32, 
    SST_SST25VF016,
    SPINOR_DEV_NUM,    /* count of the supported device */
};


static int spi_flash_dev_type ;
static int spi_total_size ;

typedef struct _WPC450_SPI_ST
{
    struct flash_t spinor_flash ;
    UINT16 DevID ;    
}SPINOR_ST;


/* default SPI flash */
#define defualt_spi_dev ATMEL_AT26DF16


static SPINOR_ST spinor_list[]=
{
/* MF_ID,          Capacity ID,          Chip name,   Chip Size, Sector(Block) Size */
{{ MF_ID_WINBOND,  CAP_ID_16Mbits,        "W25P16",      SZ_2M, SZ_64K }, W25P16_DID    },
{{ MF_ID_WINBOND,  CAP_ID_32Mbits,        "W25P32",      SZ_4M, SZ_64K }, W25P32_DID    },
{{ MF_ID_ATMEL,    CAP_ID_8Mbits,         "AT26DF08xx", SZ_1M, SZ_64K }, AT26DF08_DID  },
{{ MF_ID_ATMEL,    CAP_ID_16Mbits,        "AT26DF16",    SZ_2M, SZ_64K }, AT26DF16_DID  },
{{ MF_ID_MACRONIX, CAP_ID_16Mbits,        "MX25L16",     SZ_2M, SZ_64K }, MX25L16_DID   },
{{ MF_ID_MACRONIX, CAP_ID_128Mbits,       "MX25L128",    SZ_16M,SZ_64K }, MX25L128_DID  },
{{ MF_ID_SPANSION, CAP_ID_16Mbits,        "S25FL016_",   SZ_2M, SZ_64K }, S25FL016_DID  },
{{ MF_ID_SPANSION, S25FL128_CAPACITY_ID,  "S25FL128PIF", SZ_16M,SZ_64K }, S25FL128_DID  },
{{ MF_ID_STM,      CAP_ID_16Mbits,        "M25P16",      SZ_2M, SZ_64K }, M25P16_DID    },
{{ MF_ID_STM,      CAP_ID_32Mbits,        "M25P32",      SZ_4M, SZ_64K }, M25P32_DID    },
{{ MF_ID_SST,      CAP_ID_16Mbits,        "SST25VF016",  SZ_2M, SZ_64K }, SST25VF016_DID},
};
static flash_t wpcm450_spinor_flash ; 

#ifndef IDRACLITE
static struct mtd_partition wpcm450_spinor_16M_partition_info[] = {
{ .name		= "WPCM450 EVB SPI NOR Flash Partition 0 (U-BOOT)",
  .offset	= 0x000000,
  .size		= 256 * SZ_1K },
{ .name		= "WPCM450 EVB SPI NOR Flash Partition 1 (Private Storage)",
  .offset	= 0x040000,      /* Start at 256K */
  .size		= 768 * SZ_1K },
{ .name		= "WPCM450 EVB SPI NOR Flash Partition 2 (Kernel)",
  .offset	= 0x100000,    
  .size		= 2 * SZ_1M },
{ .name		= "WPCM450 EVB SPI NOR Flash Partition 3 ",
  .offset	= 0x300000,
  .size		= 13 * SZ_1M },
};
#else
static struct mtd_partition wpcm450_spinor_16M_partition_info[] = {
{ .name		= "WPCM450 EVB SPI NOR Flash Partition 0 (U-BOOT)",
  .offset	= 0x000000,
  .size		= 256 * SZ_1K },
{ .name		= "WPCM450 EVB SPI NOR Flash Partition 1 (Private Storage)",
  .offset	= 0x040000,      /* Start at 256K */
  .size		= 768 * SZ_1K },
{ .name		= "WPCM450 EVB SPI NOR Flash Partition 2 (Life Cycle Log)",
  .offset	= 0x100000,      /* Start at 1MB */
  .size		= 1 * SZ_1M },
{ .name		= "WPCM450 EVB SPI NOR Flash Partition 3 (Kernel)",
  .offset	= 0x200000,    
  .size		= (1 * SZ_1M) + (512 * SZ_1K) },
{ .name		= "WPCM450 EVB SPI NOR Flash Partition 4 (Rootfs)",
  .offset	= 0x380000,
  .size		= (5 * SZ_1M) + (512 * SZ_1K) },
{ .name		= "WPCM450 EVB SPI NOR Flash Partition 5 (DellFS)",
  .offset	= 0x900000,
  .size		= 7 * SZ_1M },
};
#endif
// for debug 
static struct mtd_partition wpcm450_spinor_8M_partition_info[] = {
{ .name		= "Whoville SPI NOR Flash Partition 0 (U-BOOT)",
  .offset	= 0x000000,
  .size		= 256 * SZ_1K },
{ .name		= "Whoville SPI NOR Flash Partition 1 (Private Storage)",
  .offset	= 0x040000,      /* Start at 256K */
  .size		= 768 * SZ_1K },
{ .name		= "Whoville SPI NOR Flash Partition 2  (Kernel)",
  .offset	= 0x100000,      /* Start at 256K */
  .size		= 1 * SZ_1M },
{ .name		= "Whoville SPI NOR Flash Partition 3 ",
  .offset	= 0x200000,      /* Start at 256K */
  .size		= 2 * SZ_1M },
{ .name		= "Whoville SPI NOR Flash Partition 4 ",
  .offset	= 0x400000,      /* Start at 256K */
  .size		= 2 * SZ_1M },
{ .name		= "Whoville SPI NOR Flash Partition 5 ",
  .offset	= 0x600000,      /* Start at 256K */
  .size		= 2 * SZ_1M },
};

static struct mtd_partition wpcm450_spinor_2M_partition_info[] = {
{ .name		= "Whoville SPI NOR Flash Partition 0 (U-BOOT)",
  .offset	= 0x000000,
  .size		= 256 * SZ_1K },
{ .name		= "Whoville SPI NOR Flash Partition 1 (Private Storage)",
  .offset	= 0x040000,      /* Start at 256K */
  .size		= 768 * SZ_1K },
{ .name		= "Whoville SPI NOR Flash Partition 2 ",
  .offset	= 0x100000,      /* Start at 256K */
  .size		= 1 * SZ_1M },
};

static struct mtd_partition wpcm450_spinor_1M_partition_info[] = {
{ .name		= "Whoville SPI NOR Flash Partition 0 (U-BOOT)",
  .offset	= 0x000000,
  .size		= 256 * SZ_1K },
{ .name		= "Whoville SPI NOR Flash Partition 1 (Private Storage)",
  .offset	= 0x040000,      /* Start at 256K */
  .size		= 768 * SZ_1K },
};

#ifdef dump_msg_supported
static void dump_msg(const char *label, const unsigned char *buf, unsigned int length)
{
	unsigned int	start, num, i;
	char		line[52], *p;

	if (length > 32)
	{
		length = 32;
	}
    
	printk("MTD_SPI: %s, length %u:\n", label, length);

	start = 0;
	while (length > 0) {
		num = min(length, 16u);
		p = line;
		for (i = 0; i < num; ++i) {
			if (i == 8)
				*p++ = ' ';
			sprintf(p, " %02x", buf[i]);
			p += 3;
		}
		*p = 0;
		printk("%6x: %s\n", start, line);
		buf += num;
		start += num;
		length -= num;
	}
}
#endif


/* control the signal of WP# of SPI flash */
void fiu_write_protect(int dev, UINT8 enable)
{
    if (enable)
    {
#ifdef CONFIG_WPCM450_SPINOR_WHOVILLE
        /* pull low GPIO40, external resister is pulling low */
        clear_bit(GPIO_GPIO40, (void *) (GPIO_REG_PORT2_DATAOUT_ADDR));
        clear_bit(GPIO_GPIO40, (void *) (GPIO_REG_PORT2_CFG0_ADDR));
#endif
    }
    else
    {
#ifdef CONFIG_WPCM450_SPINOR_WHOVILLE
        /* pull high GPIO40 */
        set_bit(GPIO_GPIO40, (void *) (GPIO_REG_PORT2_DATAOUT_ADDR));
        set_bit(GPIO_GPIO40, (void *) (GPIO_REG_PORT2_CFG0_ADDR));
#endif
    }
    
    return;
}


/*------------------------------------------------------------------------------
 * Function:
 *    fiu_unlock_write_protection
 *
 * Description:
 *    Calling this function will do the following: 
 *        1. unlock the size and position of the 3 "write windows". Note that it 
 *           doesn't change the existing size & position.
 *        2. Remove all write protection from the Firmware Boot Block
 *                     
 * Parameters:  
 *    None
 *
 * Return Value:
 *    None
 *----------------------------------------------------------------------------*/
void fiu_unlock_write_protection(void) 
{
    /* open lock sequence - 0x87, 0x61, 0x63 */
    PROT_CLEAR = 0x87;
    PROT_CLEAR = 0x61;
    PROT_CLEAR = 0x63;
}

/*------------------------------------------------------------------------------
* Function:
*    fiu_set_write_window
*
* Description:
*    All access to the flash is write protected by default. Write access is only 
*    allowed inside a "write window". Use this function to open a "write window"
*    The FIU support up to 3 programmable write windows. Each window can be located 
*    on any address boundary of 4Kbytes and it size can vary from 4Kbytes to 4Mbytes
*
* Parameters:  
*    win_num    - "write window" number. Legal values are 1, 2 or 3
*    low_limit  - Address of the window's low limit in the flash address space.
*                 Writing 0 to this parameter will disable the window.
*    high_limit - Address of the window's high limit in the flash address space.
*                 Writing 0 to this parameter will disable the window.
*    lock       - Set to TRUE to lock the window position & size, otherwise set to
*                 FALSE. Once the window is locked it can be unlocked either by
*                 reset, or by calling FIU_unlock_write_protection().
*
* Return Value:    
*    TRUE  - write window was set as requested.
*    FALSE - Error: The selected window is locked and can not be changed.
*----------------------------------------------------------------------------*/
BOOL fiu_set_write_window(int win_num,
                          UINT32 low_limit,
                          UINT32 high_limit,
                          BOOL lock) 
{
    // divide the window limits by 4K
    low_limit = low_limit >> WIN_LIMIT_4K_SHIFT;
    high_limit = high_limit >> WIN_LIMIT_4K_SHIFT;

    // check that high_limit is indeed higher than low_limit 
    if (!(high_limit > low_limit))
    {
        return FALSE;
    }

    switch (win_num)
    {
        case 1:
            // check that WIN1 is not locked
            if (IS_BIT_SET(PROT_LOCK, FIU_FWIN1_LK_BIT))
            {
                return FALSE;
            }
            
            // set WIN1
            SET_FIELD(FWIN1_LOW, FIU_WIN_LOW, low_limit);
            SET_FIELD(FWIN1_HIGH, FIU_WIN_HIGH, high_limit);

            // if lock - lock WIN1
            if (lock)
            {
                SET_BIT(PROT_LOCK, FIU_FWIN1_LK_BIT);
            }
            break;

        case 2:
            // check that WIN2 is not locked
            if (IS_BIT_SET(PROT_LOCK, FIU_FWIN2_LK_BIT))
            {
                return FALSE;
            }
            
            // set WIN2
            SET_FIELD(FWIN2_LOW, FIU_WIN_LOW, low_limit);
            SET_FIELD(FWIN2_HIGH, FIU_WIN_HIGH, high_limit);
            
            // if lock - lock WIN2
            if (lock)
            {
                SET_BIT(PROT_LOCK, FIU_FWIN2_LK_BIT);
            }
            break;

        case 3:
            // check that WIN3 is not locked
            if (IS_BIT_SET(PROT_LOCK, FIU_FWIN3_LK_BIT))
            {
                return FALSE;
            }
            
            // set WIN3
            SET_FIELD(FWIN3_LOW, FIU_WIN_LOW, low_limit);
            SET_FIELD(FWIN3_HIGH, FIU_WIN_HIGH, high_limit);
            
            // if lock - lock WIN3
            if (lock)
            {
                SET_BIT(PROT_LOCK, FIU_FWIN3_LK_BIT);
            }
            break;

        default:
            // illegal device
            return FALSE;
    }

    return TRUE;
}

/*------------------------------------------------------------------------------
* Function:
*    fiu_uma_read
*                
* Description:
*    Read up to 4 bytes from the flash. using the FIU User Mode Access (UMA).
*
* Parameters:
*    device           - Select the flash device (0 or 1) to be accessed 
*    transaction_code - Specify the SPI UMA transaction code
*    address          - Location on the flash , in the flash address space
*    address_size     - if TRUE, 3 bytes address, to be placed in UMA_AB0-2
*                       else (FALSE), no address for this SPI UMA transaction 
*    data             - a pointer to a data buffer to hold the read data.
*    data_size        - buffer size. Legal sizes are 1,2,3,4 
*                       
*
* Return Value:
*    None
*----------------------------------------------------------------------------*/
void fiu_uma_read(int device,
                  UINT8 transaction_code,        
                  UINT32 address,
                  BOOL address_size,
                  UINT8 * data,
                  int data_size) 
{
    UINT8 uma_ab;
    UINT8 uma_cts = 0x0;

    // set device number - DEV_NUM in UMA_CTS
    // legal device numbers are 0,1,2,3 
    switch(device)
    {
        case 0 :
        case 1 :
        case 2 :
        case 3 :
            SET_FIELD(uma_cts, UMA_DEV_NUM, device);
            break;
            
        default:
            return;
    }

    SET_FIELD(uma_cts, UMA_DEV_NUM, device);

    // set transaction code in UMA_CODE
    UMA_CODE = transaction_code;

    if (address_size)
    {
        // set address size bit
        SET_BIT(uma_cts, UMA_A_SIZE_BIT);

        // set the UMA address registers - UMA_AB0-2
        uma_ab = address & 0xFF;
        UMA_AB0 = uma_ab;

        uma_ab = (address >> 8) & 0xFF;
        UMA_AB1 = uma_ab;

        uma_ab = (address >> 16) & 0xFF;
        UMA_AB2 = uma_ab;
    }
    else
    {
        // clear address size bit
        CLEAR_BIT(uma_cts, UMA_A_SIZE_BIT);
    }

    // check legal data size
    switch (data_size)
    {
        case 4:
            UMA_DB3 = 0x0;
        case 3:
            UMA_DB2 = 0x0;
        case 2:
            UMA_DB1 = 0x0;
        case 1:
            UMA_DB0 = 0x0;
            break;       
            
        default:
            // illegal size
            return;
    }

    // set data size - D_SIZE in UMA_CTS
    SET_FIELD(uma_cts, UMA_D_SIZE, data_size);

    // read select in UMA_CTS
    CLEAR_BIT(uma_cts, UMA_RD_WR_BIT);

    UMA_CTS = uma_cts;   

    // initiate the read
    SET_BIT(UMA_CTS, UMA_EXEC_DONE_BIT);

    // wait for indication that transaction has terminated
    while (IS_BIT_SET(UMA_CTS, UMA_EXEC_DONE_BIT));

    // copy read data from UMA_DB0-3 regs to data buffer    
    switch (data_size)
    {
        case 4:
            *(data+3) = UMA_DB3;
        case 3:
            *(data+2) = UMA_DB2;
        case 2:
            *(data+1) = UMA_DB1;
        case 1:
            *data     = UMA_DB0;
            break;  
    }
}
/********************************************************************
name : 

description : Waits until the flash is not busy

parameters :

return :

*********************************************************************/
static int  fiu_wait_for_ready(UINT8 device) 
{
    int count;
    UINT8 sr;

    for (count = 0; count < MAX_READY_WAIT_COUNT; count++)
    {
        fiu_uma_read(device,
                     SPI_READ_STATUS_REG_CMD,        
                     0, //NULL,
                     FALSE,
                     &sr,
                     1);
        
        if (!(sr & SR_WIP))
        {
            return (STATUS_OK);
        }
    }

    return (STATUS_BUSY);
}

/*------------------------------------------------------------------------------
* Function:    
*    FIU_uma_write
*
* Description:
*    Write up to 4 bytes to the flash using the FIU User Mode Access (UMA) which 
*    allows the core an indirect access to the flash, bypassing FIU flash write
*    protection.
*                  
* Parameters:  
*    device           - Select the flash device (0 or 1) to be accessed
*    transaction_code - Specify the SPI UMA transaction code
*    address          - Location on the flash, in the flash address space
*    address_size     - if TRUE, 3 bytes address, to be placed in UMA_AB0-2
*                       else (FALSE), no address for this SPI UMA transaction 
*    data             - a pointer to a data buffer (buffer of bytes)
*    data_size        - data buffer size in bytes. Legal sizes are 0,1,2,3,4 
*
* Return Value:
*    None
*----------------------------------------------------------------------------*/
void fiu_uma_write(
           int device,
           UINT8 transaction_code,
           UINT32 address,
           BOOL address_size,
           UINT8 * data,
           int data_size) 
{
    UINT8 uma_ab = 0x0;
    UINT8 uma_cts = 0x0;


    // set device number - DEV_NUM in UMA_CTS
    // legal device numbers are 0,1,2,3 
    switch(device)
    {
        case 0 :
        case 1 :
        case 2 :
        case 3 :
            SET_FIELD(uma_cts, UMA_DEV_NUM, device);
            break;
        default:
            return;
    }

    // set transaction code in UMA_CODE
    UMA_CODE = transaction_code;

    if (address_size)
    {
        // set address size bit
        SET_BIT(uma_cts, UMA_A_SIZE_BIT);

        // set the UMA address registers - UMA_AB0-2
        uma_ab = address & BITS_7_0;
        UMA_AB0 = uma_ab;

        uma_ab = (address & BITS_15_8) >> 8;
        UMA_AB1 = uma_ab;

        uma_ab = (address & BITS_23_16) >> 16;
        UMA_AB2 = uma_ab;
    } 
    else
    {
        // clear address size bit
        CLEAR_BIT(uma_cts, UMA_A_SIZE_BIT);
    }

    // set the UMA data registers - UMA_DB0-3
    switch (data_size)
    {
        case 4:
            UMA_DB3 = *(data+3);
        case 3:
            UMA_DB2 = *(data+2);
        case 2:
            UMA_DB1 = *(data+1);
        case 1:
            UMA_DB0 = *data;
            break;
        case 0:
            // no data to write (a control transaction)
            break;
        default:
            // illegal data_size
            return;
    }

    // set data size
    SET_FIELD(uma_cts,UMA_D_SIZE, data_size);

    // write select
    SET_BIT(uma_cts, UMA_RD_WR_BIT);

    UMA_CTS = uma_cts;  

    // initiate the write
    SET_BIT(UMA_CTS, UMA_EXEC_DONE_BIT);

    // wait for indication that transaction has terminated
    while (IS_BIT_SET(UMA_CTS, UMA_EXEC_DONE_BIT));
}

/* page write 
   - data_size should be even and between 4 to 256
   - bit 0 of address should be zero, 
   - (address + data_size) can't exceed one page*/
void fiu_uma_page_write(
                        int device,
                        UINT32 address,
                        UINT8 * data,
                        int data_size
                       )
{
    UINT8 uma_ab = 0x00;
    UINT8 uma_cts = 0x00;
    int cur_offset = 0;
    int rest_data = 0;
    
    /* set transaction code in UMA_CODE */
    UMA_CODE = SPI_PAGE_PRGM_CMD;
    
    /* set the UMA address registers - UMA_AB0-2 */
    uma_ab = address & BITS_7_0;
    UMA_AB0 = uma_ab;

    uma_ab = (address & BITS_15_8) >> 8;
    UMA_AB1 = uma_ab;

    uma_ab = (address & BITS_23_16) >> 16;
    UMA_AB2 = uma_ab;
    
    /* set the UMA data registers - UMA_DB0-3 */
    UMA_DB0 = *(data + cur_offset++);
    UMA_DB1 = *(data + cur_offset++);
    UMA_DB2 = *(data + cur_offset++);
    UMA_DB3 = *(data + cur_offset++);
    
    /* set device number - DEV_NUM in UMA_CTS
       legal device numbers are 0,1,2,3 */
    switch(device)
    {
        case 0 :
        case 1 :
        case 2 :
        case 3 :
            SET_FIELD(uma_cts, UMA_DEV_NUM, device);
            break;
        default:
        return;
    }

    /* set address size bit */
    SET_BIT(uma_cts, UMA_A_SIZE_BIT);
    
    /* set data size */
    SET_FIELD(uma_cts,UMA_D_SIZE, 4);

    /* write select */
    SET_BIT(uma_cts, UMA_RD_WR_BIT);

    /* software control chip select */
    UMA_ECTS = 0x0F & ~(0x01 << device);

    UMA_CTS = uma_cts;

    /* initiate the write */
    SET_BIT(UMA_CTS, UMA_EXEC_DONE_BIT);

    /* wait for indication that transaction has terminated */
    while (IS_BIT_SET(UMA_CTS, UMA_EXEC_DONE_BIT));
    
    cur_offset = 4;
    
    if (data_size <= cur_offset)
    {
        /* software release chip select */
        UMA_ECTS = 0x0F;
        
        return;
    }
    
    for(;cur_offset < data_size;)
    {
        rest_data = data_size - cur_offset;
        
        if (rest_data >= 8)
        {
            /* 8 bytes */
            UMA_CODE = *(data + cur_offset++);
            UMA_AB2 = *(data + cur_offset++);
            
            UMA_AB1 = *(data + cur_offset++);
            UMA_AB0 = *(data + cur_offset++);
            
            UMA_DB0 = *(data + cur_offset++);
            UMA_DB1 = *(data + cur_offset++);
            
            UMA_DB2 = *(data + cur_offset++);
            UMA_DB3 = *(data + cur_offset++);
        }
        else if (rest_data == 6)
        {
            /* 6 bytes */
            UMA_CODE = *(data + cur_offset++);
            UMA_AB2 = *(data + cur_offset++);
            
            UMA_AB1 = *(data + cur_offset++);
            UMA_AB0 = *(data + cur_offset++);
            
            UMA_DB0 = *(data + cur_offset++);
            UMA_DB1 = *(data + cur_offset++);
            
            /* set data field as 2 */
            uma_cts = ((uma_cts & 0xF8) | 0x02);
            
        }
        else if (rest_data == 4)
        {
            /* 4 bytes */
            UMA_CODE = *(data + cur_offset++);
            UMA_AB2 = *(data + cur_offset++);
            
            UMA_AB1 = *(data + cur_offset++);
            UMA_AB0 = *(data + cur_offset++);
            
            /* set data field as 0 */
            uma_cts = (uma_cts & 0xF8);
            
            
        }
        /* if (rest_data == 2) */
        else if (rest_data == 2)
        {
            /* 2 bytes */
            UMA_CODE = *(data + cur_offset++);
            UMA_DB0 = *(data + cur_offset++);
            
            /* set data field as 1 */
            uma_cts = ((uma_cts & 0xF0) | 0x01);
            
        }
        else
        {
            DEBUG(MTD_DEBUG_LEVEL0, "data_size error\n");
            break;
            
        }
        
        UMA_CTS = uma_cts;
        
        /* initiate the write */
        SET_BIT(UMA_CTS, UMA_EXEC_DONE_BIT);
        
        /* wait for indication that transaction has terminated */
        while (IS_BIT_SET(UMA_CTS, UMA_EXEC_DONE_BIT));
    }
    
    /* software release chip select */
    UMA_ECTS = 0x0F;
    
    return;
}

/* page write 
   - data_size should be even and between 4 to 256
   - bit 0 of address should be zero, 
   - (address + data_size) can't exceed one page*/
static void fiu_uma_sequential_write(
                        UINT8 device,
                        UINT32 address,
                        UINT8 * data,
                        int data_size
                       )
{
    
    UINT8 uma_ab = 0x00;
    UINT8 uma_cts = 0x00;
    UINT32 cur_offset = 0;
    
    // set transaction code in UMA_CODE 
    UMA_CODE = SPI_SEQU_PRGM_CMD;
    
    // set the UMA address registers - UMA_AB0-2 
    uma_ab = address & BITS_7_0;
    UMA_AB0 = uma_ab;

    uma_ab = (address & BITS_15_8) >> 8;
    UMA_AB1 = uma_ab;

    uma_ab = (address & BITS_23_16) >> 16;
    UMA_AB2 = uma_ab;
    
    // set the UMA data registers - UMA_DB0-3 
    UMA_DB0 = *(data + cur_offset++);
    UMA_DB1 = *(data + cur_offset++);
    
    // set device number - DEV_NUM in UMA_CTS
    //   legal device numbers are 0,1,2,3 
    switch (device)
    {
        case 0:
        case 1:
        case 2:
        case 3:
            SET_FIELD(uma_cts, UMA_DEV_NUM, device);
            break;
            
        default:
            return;
    }

    // set address size bit 
    SET_BIT(uma_cts, UMA_A_SIZE_BIT);
    
    // set data size 
    SET_FIELD(uma_cts, UMA_D_SIZE, 2 );
    
    // write select 
    SET_BIT(uma_cts, UMA_RD_WR_BIT);
    
    // set to hardware register 
    UMA_CTS = uma_cts;
    
    // initiate the write 
    SET_BIT(UMA_CTS, UMA_EXEC_DONE_BIT);
    
    // wait for indication that transaction has terminated 
    while (IS_BIT_SET(UMA_CTS, UMA_EXEC_DONE_BIT));
    
    // check if flash is busy 
    fiu_wait_for_ready(device);
    if (data_size <= cur_offset)
    {
        // issue write disable command to terminate the sequential program 
        fiu_uma_write(
                       device,                  // only one flash device
                       SPI_WRITE_DISABLE_CMD,   // write enable transaction code
                       0,                       // address irrelevant
                       FALSE,                   // no address for transaction 
                       NULL,                    // no write data
                       0);                      // no data
        
        // check if flash is busy 
        fiu_wait_for_ready(device);
        
        return;
    }
    // clear address size bit 
    CLEAR_BIT(uma_cts, UMA_A_SIZE_BIT);
    
    while (cur_offset < data_size)
    {
        // set transaction code in UMA_CODE 
        UMA_CODE = SPI_SEQU_PRGM_CMD;
        
        UMA_DB0 = *(data + cur_offset++);
        UMA_DB1 = *(data + cur_offset++);
        
        // set to hardware register 
        UMA_CTS = uma_cts;
        
        // initiate the write 
        SET_BIT(UMA_CTS, UMA_EXEC_DONE_BIT);
        
        // wait for indication that transaction has terminated 
        while (IS_BIT_SET(UMA_CTS, UMA_EXEC_DONE_BIT));
        
        // check if flash is busy 
        fiu_wait_for_ready(device);
    }
    
    // issue write disable command to terminate the sequential program 
    fiu_uma_write(
                   device,                  // only one flash device
                   SPI_WRITE_DISABLE_CMD,   // write enable transaction code
                   0,                       // address irrelevant
                   FALSE,                   // no address for transaction 
                   NULL,                    // no write data
                   0);                      // no data
    
    // check if flash is busy 
    fiu_wait_for_ready(device);
    return;
}

void fiu_config(int max_chip_size, int total_flash_size) 
{
    UINT8 fl_blk_cnt = 0x00;
    SPI_fast_read_t fast_read = SPI_FAST_READ_DISABLE;
    SPI_w_protect_int_t wpa_trap = SPI_W_PROTECT_INT_DISABLE;
    SPI_incorect_access_int_t iad_trap = SPI_INCORECT_ACCESS_INT_DISABLE; 
    SPI_w_burst_t write_burst_size = SPI_ONE_BYTE_W_BURST;
    SPI_r_burst_t read_burst_size = SPI_ONE_BYTE_R_BURST;

    // config total flash size - FIU_CFG.FL_SIZE_P1
    // if not 512 (KB) align, add 512 (KB)
    if (total_flash_size & (SZ_512K - 1))
    {
        total_flash_size = total_flash_size + SZ_512K; 
    }
    
    // convert from KBs to 512KBs
    fl_blk_cnt = total_flash_size >> SHIFT_512;

    SET_FIELD(FIU_CFG, FIU_FL_SIZE, fl_blk_cnt);
   
    // config larger flash size 
    SET_FIELD(SPI_FL_CFG, SPI_DEV_SIZE, max_chip_size >> SHIFT_512); 
    
    // config fast read
    if (fast_read)
    {
        SET_BIT(SPI_FL_CFG, SPI_F_READ_BIT);
    }
    else
    {
        CLEAR_BIT(SPI_FL_CFG, SPI_F_READ_BIT);
    }
    
    // config IAD trap
    if (iad_trap)
    {
        SET_BIT(RESP_CFG, FIU_IAD_EN_BIT);
    }
    else
    {
        CLEAR_BIT(RESP_CFG, FIU_IAD_EN_BIT);
    }
    
    // config IAD trap
    if (wpa_trap)
    {
      SET_BIT(RESP_CFG, FIU_INT_EN_BIT);
    }
    else
    {
      CLEAR_BIT(RESP_CFG, FIU_INT_EN_BIT);
    }
   
    // config write burst size
    SET_FIELD(BURST_CFG, FIU_W_BURST, write_burst_size);

    // config read burst size
    SET_FIELD(BURST_CFG, FIU_R_BURST, read_burst_size);
}

static int wpcm450_spinor_read(struct mtd_info *mtd, loff_t from, size_t len,
	size_t *retlen, u_char *buf)
{
    struct spinor_chip *flash = mtd->priv;
    struct flash_t *info = &wpcm450_spinor_flash;
    int dev;
    int i, readlen, currlen;
    UINT32 addr = (UINT32) from;

    DEBUG(MTD_DEBUG_LEVEL2, "%s %s 0x%08x, len %zd\n", __FUNCTION__, "from", (UINT32)from, len);
    
    /* sanity checks */
    if (!len)
    {
        return 0;
    }
    
    if (from + len > mtd->size)
    {
        return -EINVAL;
    }
    
    /* Byte count starts at zero. */
    if (retlen)
    {
        *retlen = 0;
    }
    
    down(&flash->lock);

    /* get flash bank information */
    dev = addr / info->chip_size;

    /* Wait till previous write/erase is done. */
    if (fiu_wait_for_ready(dev))
    {
        /* REVISIT status return?? */
        //up(&flash->lock);
        *retlen = 0;

        return 1;
    }

    /* NOTE:  OPCODE_FAST_READ (if available) is faster... */
    i = 0;
    currlen = (int) len;
    do
    {
        addr = ((UINT32) from + i);
        
        /* get flash bank information */
        dev = addr / info->chip_size;
        addr -= (dev * info->chip_size);
        
        if (currlen < 4)
        {
            readlen = currlen;
        }
        else
        {
            readlen = 4;
        }

        DEBUG(MTD_DEBUG_LEVEL3, "fiu_uma_read: dev=%d, ori_addr=0x%lx, addr=0x%lx ", dev, ((UINT32) from + i), addr);

        fiu_uma_read(dev,                   // only one flash device
                     SPI_READ_DATA_CMD,     // read transaction code
                     addr,
                     TRUE,                  // transaction has address 
                     (UINT8 *)(buf + i),    // buffer to store read data
                     readlen);              // read data size

        DEBUG(MTD_DEBUG_LEVEL3, "buf=0x%lx\n", *(UINT32 *)(buf + i));
        
        i += readlen;
        currlen -= 4;	 

    }
    while (currlen > 0);

    DUMP_MSG("MTD_READ", buf, i);
    
    *retlen = i;

    up(&flash->lock);

    return 0;
}

static int wpcm450_spinor_write(struct mtd_info *mtd, loff_t to, size_t len,
                            	size_t *retlen, const u_char *buf)
{
    UINT32 addr = (UINT32) to;
    UINT32 cnt = (UINT32) len;
    struct spinor_chip *flash = mtd->priv;
    struct flash_t *info = &wpcm450_spinor_flash;
    int dev;
    int align_size;
    int rest_size;
    UINT32 w_addr;
    UINT32 w_bytes = 0;
    UINT32 chip_addr;
    
    DEBUG(MTD_DEBUG_LEVEL2, "%s %s 0x%08x, len %zd\n", __FUNCTION__, "to", (UINT32)to, len);

    if (retlen)
    {
        *retlen = 0;
    }
    
    /* sanity checks */
    if (!len)
    {
        return(0);
    }
    
    if (to + len > mtd->size)
    {
        return -EINVAL;
    }
    
    down(&flash->lock);

    /* get flash bank information */
    dev = addr / info->chip_size;

    /* check if flash is busy */
    if (fiu_wait_for_ready(dev))
    {
        /* REVISIT status return?? */
        up(&flash->lock);
        *retlen = 0;

        return 1;
    }

    /* 1: Process even address */
    if (addr & 0x01)
    {
        DEBUG(MTD_DEBUG_LEVEL3, "1: Process even address: addr=0x%lx\n", addr);
        
        fiu_uma_write(
                      dev,                   // only one flash device
                      SPI_WRITE_ENABLE_CMD,  // write enable transaction code
                      0,                     // address irrelevant            
                      FALSE,                 // no address for transaction    
                      NULL,                  // no write data                 
                      0);                    // no data                       
        
        if (fiu_wait_for_ready(dev))
        {
            /* REVISIT status return?? */
            up(&flash->lock);
            *retlen = 0;
            
            return 1;
        }

        fiu_uma_write(dev,               // only one flash device
                      SPI_PAGE_PRGM_CMD,            // write transaction code
                      addr,
                      TRUE,            // transaction has address 
                      (UINT8 *) buf,      // write data
                      1);              // data size

        //*((volatile UINT8*)(addr))=*buf;
        
        buf++;
        addr++;
        cnt--;  
        
        if (fiu_wait_for_ready(dev))
        {
            /* REVISIT status return?? */
            up(&flash->lock);
            *retlen = 0;
            
            return 1;
        }
    }
    
    /* set up destination of writing address */
    w_addr = addr;
    
    /* get flash bank information */
    dev = w_addr / info->chip_size;

    align_size = cnt & (~0x03); /* amount whole 32-bit words */
    rest_size  = cnt & 0x03;    /* amount non 32-bit rest bytes */

    
    if (align_size != 0)
    {
        if ((addr & 0x000000FF) != 0)
        {
            fiu_uma_write(
                          dev,                   // flash device num
                          SPI_WRITE_ENABLE_CMD,  // write enable transaction code
                          0,                     // address irrelevant
                          FALSE,                 // no address for transaction 
                          NULL,                  // no write data
                          0);                    // no data
            
            w_bytes = 0x00000100 - (addr & 0x000000FF);
            
            if (w_bytes > align_size)
            {
                w_bytes = align_size;
            }
            
            if (fiu_wait_for_ready(dev))
            {
                /* REVISIT status return?? */
                up(&flash->lock);
                *retlen = 0;
                
                return 1;
            }
            
            /* 2. Process Partial Page */
            DEBUG(MTD_DEBUG_LEVEL3, "2. Process Partial Page from addr=0x%lx len=0x%lx\n", w_addr, w_bytes);

            chip_addr = w_addr - (dev * info->chip_size);
         
            if( spi_flash_dev_type == SST_SST25VF016 )  
                fiu_uma_sequential_write(dev, chip_addr, (UINT8*)buf, (int) w_bytes);
            else
                fiu_uma_page_write(dev, chip_addr, (UINT8*)buf, (int) w_bytes);
                            
            DEBUG(MTD_DEBUG_LEVEL1, "Writing Flash: 0x%lx     \r", w_addr);
            
            buf += w_bytes;
            w_addr += w_bytes;
            align_size -= w_bytes;
            
            if (fiu_wait_for_ready(dev))
            {
                /* REVISIT status return?? */
                up(&flash->lock);
                *retlen = 0;
                
                return 1;
            }
        }

        /* 3. Process Whole Page */
        DEBUG(MTD_DEBUG_LEVEL3, "3. Process Whole Page from addr=0x%lx len=0x%lx\n", w_addr, align_size);
        
        while (align_size > 0)
        {
            /* get flash bank information */
            dev = w_addr / info->chip_size;
            
            fiu_uma_write(
                           dev,                   // flash device num
                           SPI_WRITE_ENABLE_CMD,  // write enable transaction code
                           0,                     // address irrelevant
                           FALSE,                 // no address for transaction 
                           NULL,                  // no write data
                           0);                    // no data
            
            /* page size is 256 bytes */
            if (align_size >= 0x100)
            {
                w_bytes = 0x100;
            }
            else
            {
                /* 4. Process the Finial aligned byte of Partial Page */
                DEBUG(MTD_DEBUG_LEVEL3, "4. Process the Finial aligned byte of Partial Page from addr=0x%lx len=0x%lx\n", w_addr, align_size);
                w_bytes = align_size;
            }
            
            if (fiu_wait_for_ready(dev))
            {
                /* REVISIT status return?? */
                up(&flash->lock);
                *retlen = 0;
                
                return 1;
            }

            chip_addr = w_addr - (dev * info->chip_size);
            if( spi_flash_dev_type == SST_SST25VF016 )  
                fiu_uma_sequential_write(dev, w_addr, (UINT8*)buf, (int) w_bytes);
            else
                fiu_uma_page_write(dev, w_addr, (UINT8*)buf, (int) w_bytes);
            //DEBUG(MTD_DEBUG_LEVEL1, "Writing Bank %d: 0x%lx     \r", dev, chip_addr);
            DEBUG(MTD_DEBUG_LEVEL1, "Writing Flash: 0x%lx     \r", w_addr);
            
            buf += w_bytes;
            w_addr += w_bytes;
            align_size -= w_bytes;
            
            if (fiu_wait_for_ready(dev))
            {
                /* REVISIT status return?? */
                up(&flash->lock);
                *retlen = 0;
                
                return 1;
            }
        }
    }

    /* 5. Process the Finial non-aligned byte of Partial Page */
    if(rest_size != 0)
    {
        DEBUG(MTD_DEBUG_LEVEL3, "5. Process the Finial non-aligned byte of Partial Page from addr=0x%lx len=0x%lx\n", w_addr, rest_size);
        
        for(; w_addr < (addr+cnt); w_addr++)
        {
            /* get flash bank information */
            dev = w_addr / info->chip_size;
            
            fiu_uma_write(
                          dev,                   // only one flash device
                          SPI_WRITE_ENABLE_CMD,  // write enable transaction code
                          0,                     // address irrelevant
                          FALSE,                 // no address for transaction 
                          NULL,                  // no write data
                          0);                    // no data
            
            if (fiu_wait_for_ready(dev))
            {
                /* REVISIT status return?? */
                up(&flash->lock);
                *retlen = 0;
                
                return 1;
            }

            chip_addr = w_addr - (dev * info->chip_size);
            fiu_uma_write(dev,               // only one flash device
                          SPI_PAGE_PRGM_CMD,            // write transaction code
                          chip_addr,
                          TRUE,            // transaction has address 
                          (UINT8 *) buf,      // write data
                          1);              // data size
            
            //*((volatile UINT8*)(w_addr))=*buf;
            
            buf++;
            
            if (fiu_wait_for_ready(dev))
            {
                /* REVISIT status return?? */
                up(&flash->lock);
                *retlen = 0;
                
                return 1;
            }
        }
    }
    
    *retlen = len;

    up(&flash->lock);
    
    return 0;
}


static int wpcm450_spinor_erase(struct mtd_info *mtd, struct erase_info *instr)
{
    struct spinor_chip *flash = mtd->priv;
    struct flash_t *info = &wpcm450_spinor_flash;
    u32 addr;
    int len;
    int dev;
    UINT32 chip_addr;

    DEBUG(MTD_DEBUG_LEVEL2, "%s %s 0x%08x, len %d\n", __FUNCTION__, "at", (u32)instr->addr, instr->len);

    /* sanity checks */
    if (instr->addr + instr->len > mtd->size)
    {
        return -EINVAL;
    }
    
    if ((instr->addr % mtd->erasesize) != 0 || (instr->len % mtd->erasesize) != 0)
    {
        return -EINVAL;
    }

    addr = instr->addr;
    len = instr->len;

    down(&flash->lock);

    /* now erase those sectors */
    while (len)
    {
        dev = addr / info->chip_size;

        fiu_uma_write(
                       dev,                     // only one flash device
                       SPI_WRITE_ENABLE_CMD,    // write enable transaction code
                       0,                       // address irrelevant
                       FALSE,                   // no address for transaction 
                       NULL,                    // no write data
                       0);                      // no data

        if(fiu_wait_for_ready(dev))
        {
            instr->state = MTD_ERASE_FAILED;
            up(&flash->lock);
            return -EIO;
        }     
        
        chip_addr = addr - (dev * info->chip_size);

        fiu_uma_write(
                       dev,                     // only one flash device
                       SPI_SECTOR_ERASE_CMD,    // sector erase transaction code
                       chip_addr,                    // address relevant
                       TRUE,                    // address for transaction 
                       NULL,                    // no write data
                       0);                      // no data
        //DEBUG(MTD_DEBUG_LEVEL1, "Erasing Bank %d: 0x%lx \r", dev, chip_addr);
        DEBUG(MTD_DEBUG_LEVEL1, "Erasing Flash: 0x%lx     \r", addr);
        
        if(fiu_wait_for_ready(dev))
        {
            instr->state = MTD_ERASE_FAILED;
            up(&flash->lock);
            return -EIO;
        }     

        addr += mtd->erasesize;
        len -= mtd->erasesize;
    }

    up(&flash->lock);

    instr->state = MTD_ERASE_DONE;
    mtd_erase_callback(instr);

    return 0;
}
#define	ADDRESS_OF_MAC	0x40034000
#define	ADDRESS_OF_MEM	0x40030000
#define ADDRESS_IN_SPI  0x00030000
#define	Addr_of_MAC_in_SPI	(0x4000+16)
/*
struct 
{
    UINT8 magic_number[4];      // 0x1B8, 0x41 0x56 0x43 0x54 (AVCT) 
    UINT8 ps_ver;               // version, 0x01 
    UINT8 reserved[3];          // 0x00, 0x00 0x00 
    UINT8 mac0[8];              // mac address for emc 0    
    UINT8 mac1[8];              // mac address for emc 1 
    UINT8 uboot_ver[8];         // 0-2: major 4-6: minor, in BCD 
}mac_info;
*/
#ifndef SZ_64K
#define SZ_64K (64*SZ_1K)
#endif

int aess_write_spi_nor_flash(unsigned char *data_addr , int len )
{
    struct spinor_chip *flash = (struct spinor_chip *) (&wpcm450_spinor_mtd[1]);
    //struct flash_t *info = &wpcm450_spinor_flash;
    int i ;
    //UINT32 addr ;
    void *pBuf_64k = NULL ;
    struct resource		*spi_mem;
    void __iomem 		*mem_ioaddr;

    down(&flash->lock);
    // get flash bank information 
  
    pBuf_64k = vmalloc( SZ_64K );
    if( pBuf_64k == NULL )
    {
        printk(KERN_ERR "\n Debug vmalloc pBuf fail" );
        goto aess_Write_err_unlock ;
    }
    spi_mem = request_mem_region( ADDRESS_OF_MEM , SZ_64K ,"READ_SPI_BUF");
    if (!spi_mem) 
    {
        printk(KERN_ERR "%s: failed to request io memory region.\n", __FUNCTION__);
        goto aess_Write_err_unmalloc ;
    }
    mem_ioaddr = ioremap( ADDRESS_OF_MEM , SZ_64K );

    if (0 == mem_ioaddr)
    {
        printk(KERN_ERR "%s: failed to ioremap() io memory region.\n", __FUNCTION__);
        goto aess_Write_err_unmap ;
    }

    memcpy_fromio( pBuf_64k , mem_ioaddr , SZ_64K );
    
    release_mem_region( ADDRESS_OF_MEM , SZ_64K );
    
    memcpy( (pBuf_64k + Addr_of_MAC_in_SPI) , data_addr , len );
    //printk( "\n debug %x:%x:%x:%x:%x:%x:",data_addr[0],data_addr[1],data_addr[2],data_addr[3],data_addr[4],data_addr[5]);
    
    if(fiu_wait_for_ready(0))
        goto aess_Write_err_unlock ;

    fiu_uma_write(
                   0 ,//                    // only one flash device
                   SPI_WRITE_ENABLE_CMD,    // write enable transaction code
                   0,                       // address irrelevant
                   FALSE,                   // no address for transaction 
                   NULL,                    // no write data
                   0);                      // no data

    if(fiu_wait_for_ready(0))
        goto aess_Write_err_unmalloc ;
        
    fiu_uma_write(
                   0,                       // only one flash device
                   SPI_SECTOR_ERASE_CMD,    // sector erase transaction code
                   ADDRESS_IN_SPI,      // address relevant
                   TRUE,                    // address for transaction 
                   NULL,                    // no write data
                   0);                      // no data
    
    if(fiu_wait_for_ready(0))
        goto aess_Write_err_unmalloc ;

    for( i = 0 ; i < SZ_64K ; i+=256 )
    {
         fiu_uma_write(
                       0,                     // only one flash device
                       SPI_WRITE_ENABLE_CMD,    // write enable transaction code
                       0,                       // address irrelevant
                       FALSE,                   // no address for transaction 
                       NULL,                    // no write data
                       0);                      // no data
    
        if(fiu_wait_for_ready( 0 ))
            goto aess_Write_err_unmalloc ;

        if( spi_flash_dev_type == SST_SST25VF016 )  
            fiu_uma_sequential_write( 0, ADDRESS_IN_SPI+i, pBuf_64k+i, 256);
        else
            fiu_uma_page_write( 0, ADDRESS_IN_SPI+i , pBuf_64k+i, 256);
        
        if(fiu_wait_for_ready( 0 ))
            goto aess_Write_err_unmalloc ;
    }
    
    vfree( pBuf_64k );
    up(&flash->lock);
    
    return 0 ;

aess_Write_err_unmap:
	release_mem_region(ADDRESS_OF_MEM, SZ_64K );

aess_Write_err_unmalloc:
    vfree( pBuf_64k );
        
aess_Write_err_unlock:
    up(&flash->lock);
//aess_Write_err_end:
    return 1 ;
        
}


static int SPINOR_device_identify( void )
{ 
    UINT8  IDs[3];
    UINT16 DevID = 0  ; 
    int dev_num , i ;
    UINT32 total_size = 0 ;
    
    spi_flash_dev_type = SPINOR_DEV_NUM ;
    
    for( dev_num = 0 ; dev_num < MAX_FLASH_BANKS ; dev_num++ )
    {
        DevID = 0;
        fiu_uma_read( dev_num , SPI_READ_JEDEC_ID_CMD, 0, false , &IDs[0] , 3 );
        DevID = ((DevID | IDs[1]) << 8) | IDs[2];
        
        if (IDs[0] == MF_ID_ATMEL)
        {
            /* ignore the byte of product version code */
            DevID &= 0xFF00;
        }
        
        for (i = 0; i < SPINOR_DEV_NUM; i++)
        {
            if ((spinor_list[i].spinor_flash.mf_id == IDs[0])
                && (spinor_list[i].DevID == DevID))
            {
                DEBUG(MTD_DEBUG_LEVEL0 ,"\nSPI[%d],ManufactureID=%02xh ,DeviceID=%02xh",dev_num,IDs[0],DevID ) ;        
                if( dev_num ==0 )
                {
                    spi_flash_dev_type = i ;
                    memcpy( &wpcm450_spinor_flash ,&spinor_list[i].spinor_flash , sizeof( flash_t) );
                }
                total_size += spinor_list[i].spinor_flash.chip_size ;
            }
        }
        
        if( spi_flash_dev_type == SPINOR_DEV_NUM && dev_num==0 )
        {
            printk("SPI flash isn't identified; use default setting\n");
            
            memcpy( &wpcm450_spinor_flash ,&spinor_list[defualt_spi_dev].spinor_flash , sizeof( flash_t) );
            total_size += spinor_list[defualt_spi_dev].spinor_flash.chip_size ;
            return total_size ;
        }
    }
    
    return total_size ;
}

static void spi_flash_unlock_protection( void )
{
    int dev_idx;
    UINT8 status_reg_val;
    
    for (dev_idx = 0; dev_idx < MAX_FLASH_BANKS; dev_idx++)
    {
        /* disable write protect signal, WP# */
        fiu_write_protect(dev_idx, 0);
        
        /* set to 0 bits 2,3,4,5 that move all chip to unprotected mode */
        /* status_reg_val &= 0xC3; */
        
        /* clear bit 7, 5, 4, 3 and 2 to disable write protections \
           bit 6, 1 and 0 are don't care */
        status_reg_val = 0;
        
        fiu_uma_write(dev_idx, SPI_WRITE_ENABLE_CMD, 0, FALSE, NULL, 0);
        fiu_wait_for_ready(dev_idx);
         
        fiu_uma_write(dev_idx, SPI_WRITE_STATUS_REG_CMD, 0, FALSE, &status_reg_val, sizeof(UINT8));
        fiu_wait_for_ready(dev_idx);
        
        /* enable write protect signal, WP# */
        fiu_write_protect(dev_idx, 1);
    } 
}


void wpcm450_fiu_init(struct spinor_chip *this)

{
    int chip_size, total_flash_size;
    
#ifdef CONFIG_WPCM450_SPINOR_EVB    
    /* enable flashes 1-3 */
    CLEAR_BIT(MFSEL1, MF_SCS3SEL_BIT);
    SET_BIT(MFSEL1, MF_SCS2SEL_BIT);
    SET_BIT(MFSEL1, MF_SCS1SEL_BIT);
#else
    /* disable flashes 1-3 */
    SET_BIT(MFSEL1, MF_SCS3SEL_BIT);
    CLEAR_BIT(MFSEL1, MF_SCS2SEL_BIT);
    CLEAR_BIT(MFSEL1, MF_SCS1SEL_BIT); 
#endif

    /* Initialize the semaphore */
    init_MUTEX(&this->lock);

    chip_size = wpcm450_spinor_flash.chip_size;

    //total_flash_size = wpcm450_spinor_flash.chip_size * MAX_FLASH_BANKS;
    total_flash_size = SPINOR_device_identify();
    
    spi_total_size = total_flash_size ;
    
    fiu_unlock_write_protection();
    
    fiu_config(chip_size >> SHIFT_1024, total_flash_size >> SHIFT_1024);	

    spi_flash_unlock_protection() ;

    if(fiu_set_write_window(1, 0x0, total_flash_size, FALSE) != TRUE )
    {
        return;     // ERROR;
    }

    if(fiu_set_write_window(2, 0x0, 0x0, FALSE) != TRUE )
    {
        return;     // ERROR;
    }

    if(fiu_set_write_window(3, 0x0, 0x0, FALSE) != TRUE )
    {
        return;     // ERROR;
    }
}


int wpcm450_spinor_probe (struct mtd_info *mtd, int maxchips)
{
    struct flash_t *info = &wpcm450_spinor_flash;
    int i;

    mtd->name = info->name;

    mtd->type = MTD_NORFLASH;
    mtd->flags = MTD_CAP_NORFLASH;
    //mtd->size = info->chip_size * MAX_FLASH_BANKS;
    mtd->size = spi_total_size ;
    mtd->erasesize = info->block_size;
    mtd->erase = wpcm450_spinor_erase;
    mtd->read = wpcm450_spinor_read;
    mtd->write = wpcm450_spinor_write;
	mtd->writesize = 1;    

    /* partitions should match sector boundaries; and it may be good to
     * use readonly partitions for writeprotected sectors (BP2..BP0).
     */
    if (mtd_has_partitions())
    {
        struct mtd_partition	*parts = NULL;
        int nr_parts = 0;

        #ifdef CONFIG_MTD_CMDLINE_PARTS
        static const char *part_probes[] = { "cmdlinepart", NULL, };

        nr_parts = parse_mtd_partitions(mtd, part_probes, &parts, 0);
        #endif

        if (nr_parts <= 0)
        {
            // configure partition by spi flash size
            if( spi_total_size >= SZ_16M )
            {
                parts = wpcm450_spinor_16M_partition_info;
                nr_parts = sizeof(wpcm450_spinor_16M_partition_info) / sizeof(struct mtd_partition);
            }
            else if( spi_total_size >= SZ_8M )
            {
                parts = wpcm450_spinor_8M_partition_info;
                nr_parts = sizeof(wpcm450_spinor_8M_partition_info) / sizeof(struct mtd_partition);
            }
            else if( spi_total_size >= SZ_2M )
            {
                parts = wpcm450_spinor_2M_partition_info;
                nr_parts = sizeof(wpcm450_spinor_2M_partition_info) / sizeof(struct mtd_partition);
            }
            else if( spi_total_size >= SZ_1M )
            {
                parts = wpcm450_spinor_1M_partition_info;
                nr_parts = sizeof(wpcm450_spinor_1M_partition_info) / sizeof(struct mtd_partition);
            }
            DEBUG(MTD_DEBUG_LEVEL0,"\nMTD spi partition_count=%d ; total_size=%d  ",nr_parts ,spi_total_size  );
        }

        if (nr_parts > 0)
        {
            for (i = 0; i < nr_parts; i++)
            {
                DEBUG(MTD_DEBUG_LEVEL0, "partitions[%d] = "
                       "{.name = %s, .offset = 0x%.8x, "
                       ".size = 0x%.8x (%uK) }\n",
                       i, parts[i].name,
                       parts[i].offset,
                       parts[i].size,
                       parts[i].size / 1024);
            }
            
            return add_mtd_partitions(mtd, parts, nr_parts);
        }
    } 

    return add_mtd_device(mtd) == 1 ? -ENODEV : 0;
}

/*
 * Main initialization routine
 */
int __init wpcm450_spinor_init (void)
{
    struct spinor_chip *this;
    int err = 0;

    /* Allocate/initialize memory for MTD device structure and private data */
    wpcm450_spinor_mtd = kzalloc(sizeof(struct mtd_info) + sizeof (struct spinor_chip), GFP_KERNEL);
    
    if (!wpcm450_spinor_mtd)
    {
        DEBUG(MTD_DEBUG_LEVEL0, "Unable to allocate WPCM450 NAND MTD device structure.\n");
        return -ENOMEM;
    }
    /* WPCM450 FIU Initialization */

    /* Get pointer to private data */
    this = (struct spinor_chip *) (&wpcm450_spinor_mtd[1]);

    /* Link the private data with the MTD structure */
    wpcm450_spinor_mtd->priv = this;

    wpcm450_fiu_init(this);

    if (wpcm450_spinor_probe(wpcm450_spinor_mtd, 1))
    {
        return -ENXIO;
    }

    return err;
}

static void __exit wpcm450_spinor_cleanup (void)
{
#ifdef CONFIG_MTD_PARTITIONS
	/* Deregister partitions */
	del_mtd_partitions(wpcm450_spinor_mtd);
#endif
	/* Deregister the device */
	del_mtd_device (wpcm450_spinor_mtd);

	/* Free the MTD device structure */
	kfree (wpcm450_spinor_mtd);
}



module_init(wpcm450_spinor_init);
module_exit(wpcm450_spinor_cleanup);

#ifndef WPCM450_SPINOR_EXPORTED_APIS
#define WPCM450_SPINOR_EXPORTED_APIS
EXPORT_SYMBOL(wpcm450_spinor_erase);
EXPORT_SYMBOL(wpcm450_spinor_write);
EXPORT_SYMBOL(wpcm450_spinor_read);
EXPORT_SYMBOL(aess_write_spi_nor_flash);
#endif

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Taiyi Kuo <taiyi.kuo@avocent.com>");
MODULE_DESCRIPTION("SPI NOR flash device support on wpcm450 evaluation broad");


