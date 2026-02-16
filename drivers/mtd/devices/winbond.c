/*
 *
 * winbond.c
 * Description:
 * winbond flash mtd driver
 *
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
#include <asm/semaphore.h>
#include <asm/delay.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

#ifdef DEBUG
#undef DEBUG
#endif

#define CONFIG_SPI_DEBUG 
#if defined(CONFIG_SPI_DEBUG)
#define DEBUG(fmt,arg...)  printk(KERN_EMERG fmt , ##arg);
#else
#define DEBUG(fmt,arg...)
#endif

/* Winbond Flash Deatils */
#define NUM_REGIONS						1
#define NUM_SECTORS 				    512		
#define NUM_BLOCKS						32
#define FLASH_PAGESIZE					256
#define SECTOR_ERASE_SIZE				4*1024
#define BLOCK_ERASE_SIZE				64*1024
#define WINBOND_MAX_ADDRESS				0x200000

#define WINBOND_DELAY					1000 
#define LOCK_ALL_SECTORS				0x18
#define PROTECT_BITS_MASK				0x3C
#define UNLOCK_FLASH_MASK				0xC3
#define DUMMY_ADDRESS					0x0000

#define WINBOND_NORMAL_READ				0
#define WINBOND_FAST_READ				1 
#define WINBOND_DUAL_READ			    2	

#define DUAL_READ_ENABLE				1
#define DUAL_READ_DISABLE				0

#define WINBOND_NUM_ADDR_BYTES			3
#define NORMAL_READ_NUM_DUMMY_BYTES		0	
#define	FAST_READ_NUM_DUMMY_BYTES		1
#define	DUAL_READ_NUM_DUMMY_BYTES		1
#define WINBOND_CMD_FRAME_SIZE			4

/***************************************************** 
*	Command Table Winbond Serial Flash 
*****************************************************/

#define OPCODE_READ_DEV_ID             0x90
#define OPCODE_WRITE_ENABLE            0x06
#define OPCODE_WRITE_DISABLE           0x04
#define OPCODE_READ_STATUS_REG         0x05
#define OPCODE_WRITE_STATUS_REG        0x01
#define OPCODE_READ                    0x03
#define OPCODE_FAST_READ               0x0B
#define OPCODE_FAST_READ_DUAL_OP       0x3B
#define OPCODE_PAGE_PROGRAM            0x02
#define OPCODE_SECTOR_ERASE            0x20 /*4KB*/
#define OPCODE_BLOCK_ERASE             0xD8 /*64 KB */
#define OPCODE_CHIP_ERASE              0xC7
#define OPCODE_DEEP_POWER_DOWN 		   0xB9	
#define OPCODE_RELEASE_POWER_DOWN 	   0xAB	
#define OPCODE_READ_JEDEC_ID           0x9F

/*****************************************************
*	Status Register bits. 
*****************************************************/
#define	SR_WIP							0x01	/* Write in progress */
#define	SR_WEL							0x02	/* Write enable latch */
#define	SR_BP0							0x04	/* Block protect 0 */
#define	SR_BP1							0x08	/* Block protect 1 */
#define	SR_BP2							0x10	/* Block protect 2 */
#define SR_TB_WP						0x20	/* Top/Bottom Write protect */	
#define	SR_SRWD							0x80	/* SR write Disable */

/* Define max times to check status register before we give up. */
#define	MAX_READY_WAIT_COUNT			100000

#ifdef CONFIG_MTD_PARTITIONS
#define	mtd_has_partitions()			(1)
#else
#define	mtd_has_partitions()			(0)
#endif


struct winbond {
	unsigned char 	    command[5];
	unsigned int        partitioned;
	struct spi_device	*spi;
	struct semaphore	lock;
	struct mtd_info		mtd;
};


static struct mtd_erase_region_info winbond_erase_regions[NUM_REGIONS] = {
	{ 0, SECTOR_ERASE_SIZE, NUM_SECTORS}  
};	

/* this is specific to winbond (may be for all spi flash devices) */
struct spasnion_sfi_transfer_hdr_t
{
    u32 len;
    u32 tx_addr;
    u32 rx_addr;
 	void *tx_buff;
    void *rx_buff;
};


/* Global variables */
static s32 sfi_mode = -1;
static s32 read_mode = -1;
static u16 winbond_sfi_mode =  CONFIG_WINBOND_SFI;
static u16 winbond_read_mode = CONFIG_WINBOND_READ;

/* Function definitions */

static inline struct winbond *mtd_to_winbond(struct mtd_info *mtd)
{
	return container_of(mtd, struct winbond, mtd);
}

/*
 * get_protect_bits_val return the value for the address range to lock the 
 * flash.
 * Returns protection bit value on success.
 */
static unsigned char get_protect_bits_val(u32 start_addr, u32 end_addr)
{
	if( (end_addr > WINBOND_MAX_ADDRESS ) ||
		( end_addr < start_addr) )
	{
		printk("In function %s Invalid Address Range\n",__FUNCTION__);
		return 0;
	}

	if( start_addr > 0x00FFFF )
	{
		/* Above 1 MB locking upper region */
		if(start_addr == 0x100000 ){
			return 0x14;
		}else if( start_addr <= 0x180000 ){
			return 0x10;
		}else if( start_addr <= 0x1C0000 ){
			return 0x0C;
		}else if( start_addr <= 0x1E0000 ){
			return 0x08;
		}else if( start_addr <= 0x1F0000 ){
			return 0x04;
		}else{
			return 0;
		}
	}
	else
	{
		/* Below  1 MB locking Lower region */
		if(end_addr <= 0x00FFFF ){
			return 0x24;
		}else if( end_addr <= 0x01FFFF ){
			return 0x14;
		}else if( end_addr <= 0x03FFFF ){
			return 0x2C;
		}else if( end_addr <= 0x07FFFF ){
			return 0x30;
		}else if( end_addr <= 0x0FFFFF ){
			return 0x34;
		}else if( end_addr <= 0x1FFFFF ){
			return 0x18;
		}else{
			return 0;
		}
	}
}	

/*
 * write_status_reg writes the vlaue to status reg.
 * Returns status on success or negetive valueif error.
 * NOTE:  write enable should be called before calling this function.
 */
static int write_status_reg(struct winbond *flash,u8  val)
{
	ssize_t retval;
	u8 write_val[2] = {OPCODE_WRITE_STATUS_REG, val};
	DEBUG("Inside the function %s\n",__FUNCTION__);
	spi_setup(flash->spi);
	retval = spi_write(flash->spi, write_val, 2);
	if( retval < 0 ) 
	{
		dev_err(&flash->spi->dev, "error %d reading SR\n",(int) retval);
		printk("error %d Writing  SR\n",(int) retval);
		return retval;
	}
	return 0;
}

/*
 * read_status_reg return the status of the flash 
 * Returns status on success or negetive valueif error.
 */
static int read_status_reg(struct winbond *flash)
{
	ssize_t retval;
	u8 code = OPCODE_READ_STATUS_REG;
	u8 val;
	DEBUG("Inside the function %s\n",__FUNCTION__);
	spi_setup(flash->spi);
	retval = spi_write_then_read(flash->spi, &code, 1, &val, 1);
	if( retval < 0 ) 
	{
		dev_err(&flash->spi->dev, "error %d reading SR\n",(int) retval);
		printk("error %d reading SR\n",(int) retval);
		return retval;
	}
	return val;
}

/*
 * write_enable perpares the flash state to enable write/erase/lock/unlock 
 * Returns 0 on success or negetive valueif error.
 */
static inline int write_enable(struct winbond *flash)
{
	ssize_t retval;
	u8	code = OPCODE_WRITE_ENABLE;

	DEBUG("Inside the function %s\n",__FUNCTION__);
	spi_setup(flash->spi);
	retval = spi_write(flash->spi, &code,1 );
	return retval;
}


/*
 * wait_till_ready to read status register until ready, or timeout occurs.
 * Returns non-zero if error.
 */
static int wait_till_ready(struct winbond *flash)
{
	s32 sr;
	u64 start = 0;

	DEBUG("Inside the function %s\n",__FUNCTION__);
	start = jiffies;
	do{ 
		sr = read_status_reg(flash);
		if (sr < 0){
			break;
		}else if (!(sr & SR_WIP)){
			return 0;
		}
	}while( (jiffies - start) < WINBOND_DELAY );

	return 1;
}

/*
 * Erase one sector of flash memory at offset ``offset'' which is any
 * address within the sector which should be erased.
 */
static int erase_sector(struct winbond *flash, u32 offset)
{
	DEBUG("Inside the function %s at 0x%08x\n", __FUNCTION__, offset);
		
	/* Wait until finished previous write command. */
	if( wait_till_ready(flash) )
	{
		up(&flash->lock);
		printk("wait_till_ready failed returning from function  line %s %d\n",
				__FUNCTION__,__LINE__);
		return 1;
	}
	/* Send write enable, then erase commands. */
	write_enable(flash);

	/* Set up command buffer. */
	flash->command[0] = OPCODE_SECTOR_ERASE;
	flash->command[1] = offset >> 16;
	flash->command[2] = offset >> 8;
	flash->command[3] = offset;
	spi_write(flash->spi, flash->command,WINBOND_CMD_FRAME_SIZE);
	return 0;
}

/*
 * Erase an address range on the flash chip.  The address range may extend
 * one or more erase sectors.  Return an error is there is a problem erasing.
 */
static int winbond_erase(struct mtd_info *mtd, struct erase_info *instr)
{
	struct winbond *flash = mtd_to_winbond(mtd);
	u32 addr,len;
	u8 status;

	DEBUG("Inside the function %s %s 0x%08x, len %d\n", __FUNCTION__, "at",
   		  (u32)instr->addr, instr->len);

	/* sanity checks */
	if( ( instr->addr + instr->len ) > flash->mtd.size)
	{
		printk("In function %s invalid address/ length\n",__FUNCTION__);
		return -EINVAL;
	}
	if ( (( instr->addr % mtd->erasesize ) != 0 )	|| 
		 ( ( instr->len % mtd->erasesize ) != 0 )  )
	{
		printk("In function %s invalid address/ length \
				Address should be in sector boundry Sector eraseSize = %x\n",
				__FUNCTION__,SECTOR_ERASE_SIZE);
		return -EINVAL;
	}

// TBD need to implement FLASH LOCK check logic 
#if 0
	status = read_status_reg(flash);
	status &= PROTECT_BITS_MASK;

	if( status >= get_protect_bits_val((instr->addr + instr->len),
		WINBOND_MAX_ADDRESS )) 
	{
		printk("ERROR can not erase locked region\n");
		printk("status = 0x%0x, Protection bits = 0x%0x \n",
		(unsigned)status,
		(unsigned)get_protect_bits_val((instr->addr + instr->len),
		WINBOND_MAX_ADDRESS));

		return -EACCES;		
  	}
#endif
	addr = instr->addr;
	len = instr->len;
  	down(&flash->lock);
	/* now erase those sectors */
	while( len ) 
	{
		instr->state = 	MTD_ERASING;
		if( erase_sector(flash, addr) ) 
		{
			instr->state = MTD_ERASE_FAILED;
			up(&flash->lock);
			printk(" Erase Sector Failed \n");
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

/*
 * Read an address range from the flash chip.  The address range
 * may be any size provided it is within the physical boundaries.
 */

static int winbond_read( struct mtd_info *mtd, loff_t from, size_t len,
						  size_t *retlen, u_char *buf)
{
	struct winbond *flash = mtd_to_winbond(mtd);
	struct spi_transfer t[2];
	struct spi_message m;
	struct spasnion_sfi_transfer_hdr_t sfi_hdr;
	u32 page_offset, page_size,i;
	u16 index = 0;
	u8 old_mode,temp;
	s32 retval = 0;
	
	DEBUG("%s %s 0x%08x, len %zd\n ", __FUNCTION__,"from",(u32)from, len);

	if( !len ){
		printk("In function %s Invalid length\n",__FUNCTION__);
		return 0;
	}
	if( ((from + len ) > flash->mtd.size) || (retlen == NULL)){
		printk("In function %s Invalid address/length \n",__FUNCTION__);
		return -EINVAL;
	}

	spi_message_init(&m);
	memset(t, 0, sizeof(t));
	/* we need the flash command packet only if we are operating in core SFI 
	 * mode 
	 */
	if( !winbond_sfi_mode )
	{
		t[index].tx_buf = flash->command;
		/* only fast read needs dummy byte address */
		if(winbond_read_mode == WINBOND_NORMAL_READ){
			t[index].len = WINBOND_CMD_FRAME_SIZE;		
		}else {
			/* In Fast read mode ADDRESS + DUMMY BYTE */		
			t[index].len = (WINBOND_CMD_FRAME_SIZE + 1);
		}
		DEBUG("length of command is %d\n",t[index].len);
		spi_message_add_tail(&t[index], &m);
		index++;
    	t[index].rx_buf = buf;
	}
	else
	{    
		DEBUG(" In function %s initializing sfi_hdr index = %d \n",
				__FUNCTION__,index);

		memset(&sfi_hdr, 0, sizeof(sfi_hdr));
		sfi_hdr.len 	= len;
		sfi_hdr.rx_buff = buf;
		sfi_hdr.rx_addr = from;
		t[index].rx_buf = &sfi_hdr;
		t[index].len = sizeof(sfi_hdr);

		DEBUG("winbond sfi header = 0x%0x, len=%d rx_addr=0x%0x\n",
   			   (unsigned)t[index].rx_buf, sfi_hdr.len, sfi_hdr.rx_addr);
	}

	spi_message_add_tail(&t[index], &m);
	*retlen = 0;
	down(&flash->lock);

	/* Wait till previous write/erase is done. */
	if( wait_till_ready(flash) )
	{
		retval = 1;
		printk("Function %s Line %d rearm  failed\n", __FUNCTION__, __LINE__);
		goto WINBOND_READ_EXIT; 
	}

	/* if we are working in sfi mode then do the wholke read and 
	 *	return immediately 
 	 */
	if(winbond_sfi_mode)
	{
		DEBUG("Switching to SFI MODE\n");
		SFI_SET_MODE(flash->spi,AVALANCHE_SFI_MODE,old_mode);
		DEBUG("Calling SPI_SYNC in SFI mode \n");
		spi_sync(flash->spi, &m);
		DEBUG("Switching back to old mode \n");
		SFI_SET_MODE(flash->spi,old_mode,temp);
		*retlen = sfi_hdr.len;    
		goto WINBOND_READ_REARM;
	}
	/* Spansion read in core spi mode */
	if(winbond_read_mode == WINBOND_DUAL_READ)
	{
		DEBUG("In function %s line num %d initializing the DUAL READ CMD \n",
			  __FUNCTION__,__LINE__);
		flash->command[0] = OPCODE_FAST_READ_DUAL_OP;
	}
	else if (winbond_read_mode == WINBOND_FAST_READ)
	{
		DEBUG("In function %s line num %d initializing the FAST READ CMD \n",
			  __FUNCTION__,__LINE__);
		flash->command[0] = OPCODE_FAST_READ;
	}else {
		DEBUG("In function %s line num %d initializing the NORAML READ CMD \n",
				__FUNCTION__,__LINE__);
		flash->command[0] = OPCODE_READ;
	}

	flash->command[1] = from >> 16;
	flash->command[2] = from >> 8;
	flash->command[3] = from;
	flash->command[4] = DUMMY_ADDRESS;

	/* what page do we start with? */
    page_offset = from % FLASH_PAGESIZE;

	/* do all the bytes fit onto one page? */
    if (page_offset + len <= FLASH_PAGESIZE) {
		DEBUG("Read with in a page \n");
        t[index].len = len;
        spi_sync(flash->spi, &m);
		DEBUG("spi_sync line %d returned = %d index = %d, cmd size = %d\n",
			   __LINE__, m.actual_length, index, t[0].len);
        *retlen = m.actual_length - t[0].len;
    } 
	else 
	{
		DEBUG("Read Multiple pages \n");
        /* the size of data remaining on the first page */
	    page_size = FLASH_PAGESIZE - page_offset;
        t[index].len = page_size;
        spi_sync(flash->spi, &m);
        *retlen = m.actual_length - t[0].len;

        /* write everything in PAGESIZE chunks */
        for (i = page_size; i < len; i += page_size) 
		{
            page_size = len - i;

            if (page_size > FLASH_PAGESIZE)
			{
                page_size = FLASH_PAGESIZE;
			}
			if(winbond_read_mode == WINBOND_DUAL_READ)
			{
				DEBUG("In function %s line num %d initializing the DUAL READ CMD \n",
					  __FUNCTION__,__LINE__);
				flash->command[0] = OPCODE_FAST_READ_DUAL_OP;
			}
			else if (winbond_read_mode == WINBOND_FAST_READ)
			{
				DEBUG("In function %s line num %d initializing the FAST READ CMD \n",
					  __FUNCTION__,__LINE__);
				flash->command[0] = OPCODE_FAST_READ;
			}else {
				DEBUG("In function %s line num %d initializing the NORMAL READ CMD \n",
						__FUNCTION__,__LINE__);
					flash->command[0] = OPCODE_READ;
			}

			flash->command[1] = (from + i) >> 16;
        	flash->command[2] = (from + i) >> 8;
           	flash->command[3] = (from + i);
			flash->command[4] = DUMMY_ADDRESS;

            t[index].rx_buf = buf + i;
            t[index].len = page_size;

            if(wait_till_ready(flash))
			{
				retval = 1;
				DEBUG("Function %s Line %d rearm  failed\n",
						 __FUNCTION__, __LINE__);
				goto WINBOND_READ_EXIT;
			}

            spi_sync(flash->spi, &m);

            if (retlen){
				DEBUG("spi_sync line %d returned = %d index = %d, \
					   cmd size = %d\n",__LINE__, m.actual_length, index,\
					   t[0].len);
                *retlen += m.actual_length - (t[0].len);
			}
		}
    }

WINBOND_READ_REARM:
	if(wait_till_ready(flash))
	{
		retval = 1;	
		printk("wait_till_ready failed returning from function  line %s %d\n",
				__FUNCTION__,__LINE__);
	}
WINBOND_READ_EXIT:
  	up(&flash->lock);
	DEBUG("Actual Length in %s : %d\n", __FUNCTION__, *retlen);
	return retval;

}

static int winbond_write( struct mtd_info *mtd, loff_t to, size_t len,
							   size_t *retlen, const u_char *buf)
{

/*****************************************************************************
     Currently Write is not supported in SFI mode so use SPI Mode writes
*****************************************************************************/
	struct winbond *flash = mtd_to_winbond(mtd);
	u32 page_offset, page_size;
	struct spi_transfer t[2];
	struct spi_message m;
	u8 status = 0,old_mode;

	DEBUG("Inside the function %s:%d\n",__FUNCTION__, __LINE__);
	DEBUG("Write to  0x%08x, len %zd\n",(u32)to, len);

	if( retlen ){
		*retlen = 0;
	}

	/* sanity checks */
	if( !len ){
		printk("In function invalid argumnet len\n"); 
		return 0 ;
	}

	if( ( to + len ) > flash->mtd.size){
		printk("In function invalid argumnet to or len\n"); 
		return -EINVAL;
	}

// TBD need to implement FLASH LOCK check logic 
#if 0
	status = read_status_reg(flash);
	status &= PROTECT_BITS_MASK;

	if(status >= get_protect_bits_val((to+len),WINBOND_MAX_ADDRESS)) 
	{
		printk("ERROR : Writing in locked region status = 0x%0x, \
				Protection bits = 0x%0x\n",status, \
				get_protect_bits_val((to+len),WINBOND_MAX_ADDRESS ) );

		return -EACCES;		
  	}
#endif

	memset(t, 0, (sizeof t));
	spi_message_init(&m);
	t[0].tx_buf = flash->command;
	/* operating in Core SPI Mode CMD length needed is 4 bytes no 
	   dummy byte required 
	*/
	/* Take care while implementing SFI write */ 	
	t[0].len = WINBOND_CMD_FRAME_SIZE;
	spi_message_add_tail(&t[0], &m);

	t[1].tx_buf = buf;
	spi_message_add_tail(&t[1], &m);

  	down(&flash->lock);

	DEBUG("Inside the function %s:%d\n",__FUNCTION__, __LINE__);
	write_enable(flash);

	/* Wait until finished previous write command. */
	if( wait_till_ready(flash) )
    {
        up(&flash->lock);
        printk("wait_till_ready failed returning from function  line %s %d\n",
				__FUNCTION__,__LINE__);
        return 1;
    }
	/* Writes is only supported in CORE_SPI mode so make sure to be 
	 * in CORE_SPI mode 
	 */
    SFI_SET_MODE(flash->spi, AVALANCHE_CORE_SPI_MODE, old_mode);
	
	/* Set up the opcode in the write buffer. */
	flash->command[0] = OPCODE_PAGE_PROGRAM;
	flash->command[1] = to >> 16;
	flash->command[2] = to >> 8;
	flash->command[3] = to;

	/* what page do we start with? */
	page_offset = to % FLASH_PAGESIZE;

	/* do all the bytes fit onto one page? */
	if (page_offset + len <= FLASH_PAGESIZE) 
	{
		t[1].len = len;
		DEBUG("Within Single Page\n");
		spi_sync(flash->spi, &m);


		*retlen = m.actual_length - t[0].len;
	}
	else 
	{
		u32 i;
		DEBUG("Spawning multiple pages\n");

		/* the size of data remaining on the first page */
		page_size = FLASH_PAGESIZE - page_offset;

		t[1].len = page_size;
		spi_sync(flash->spi, &m);

	/* operating in Core SPI Mode CMD length needed is 4 bytes no dummy byte required */
		*retlen = m.actual_length - t[0].len;

		/* write everything in PAGESIZE chunks */
		for (i = page_size; i < len; i += page_size) 
		{
			page_size = len - i;
			if (page_size > FLASH_PAGESIZE)
				page_size = FLASH_PAGESIZE;

			/* write the next page to flash */
			flash->command[0] = OPCODE_PAGE_PROGRAM;
			flash->command[1] = (to + i) >> 16;
			flash->command[2] = (to + i) >> 8;
			flash->command[3] = (to + i);

			t[1].tx_buf = buf + i;
			t[1].len = page_size;

			write_enable(flash);
			
			if (wait_till_ready(flash))
		    {
        		up(&flash->lock);
		        printk("wait_till_ready failed returning from function  \
						line %s %d\n",__FUNCTION__,__LINE__);
        		return 1;
		    }

			spi_sync(flash->spi, &m);

			if( retlen )
			{
				/* operating in Core SPI Mode CMD length needed is 4 bytes no
				 * dummy byte required 
				 */
				*retlen += m.actual_length 	- t[0].len;
	        }
 		}
	}
	if (wait_till_ready(flash))
	{
   		up(&flash->lock);
        printk("wait_till_ready failed returning from function  line %s %d\n",
				__FUNCTION__,__LINE__);

   		return 1;
 	}
	up(&flash->lock);
	return 0;
}


/* Lock and Unlock functions */
static  int winbond_lock (struct mtd_info *mtd, loff_t ofs, size_t len)
{	
	struct winbond *flash = mtd_to_winbond(mtd);
	u8 status = 0;
	s32 retval = 0; 
	DEBUG(" In function %s\n",__FUNCTION__);

	/* sanity checks */
	if( ( ofs + len ) > flash->mtd.size)
	{
		DEBUG(" In function %s Invalid arguments\n",__FUNCTION__);
		return -EINVAL;
	}
/*
	down(&flash->lock);
	status = get_protect_bits_val(ofs,(ofs + len));
	DEBUG("Status reg = 0x%x\n",(unsigned)status);

	if ( status <= (unsigned char) LOCK_ALL_SECTORS )
	{
		write_enable(flash);
		retval = write_status_reg(flash,status);
		if(retval != 0)
		{
			printk("In function  %s write_status_reg failed\n",__FUNCTION__);		
		}else{
			printk("Sector(s) corrosponding to Address  0x%8x to  0x%8x is \
					 locked\n",(unsigned)ofs,(unsigned) (ofs +len) );
		}
	}
	up(&flash->lock);
*/
	return retval;
}	

/*
 *winbond_unlock unlocks the Faash sector form the  given address  to 
 * MAx flash address.
 * returns 0 on success , negetive value otherwise	 
 */  		
static int winbond_unlock (struct mtd_info *mtd, loff_t ofs, size_t len)
{
	struct winbond *flash = mtd_to_winbond(mtd);
	u8  status;
	u8  temp;
	s32 retval=0;

	DEBUG("Inside the function %s with addr = 0x%8x and len = %d\n",
			__FUNCTION__,(unsigned)ofs,(unsigned)len);
	
	/* sanity checks */
	if( ( ofs + len ) > flash->mtd.size)
	{
		printk(" Invalid arguments\n");
		return -EINVAL;
	}
	down(&flash->lock);
	write_enable(flash);
	status = read_status_reg(flash);
	status &= UNLOCK_FLASH_MASK;
	write_enable(flash);

	/* unlock the whole flash */
	if((len + ofs) >= (WINBOND_MAX_ADDRESS - SECTOR_ERASE_SIZE))
	{
		DEBUG("Unlocking the whole flash \n");	
		status = 0;
	}
	else 
	{
		temp = get_protect_bits_val((ofs + len), WINBOND_MAX_ADDRESS);
		if(status > temp)
			status = temp;
	}

	DEBUG("0 Status reg = 0x%x", read_status_reg(flash));

	if ( status <= (unsigned char) LOCK_ALL_SECTORS )
	{
		write_enable(flash);
		/* Unlock all sectors */
		retval = write_status_reg(flash,0x0000); 
		
		if( retval == 0){
			DEBUG("Sector(s) corrosponding to Address 0x%x to  0x%x is \
					unlocked\n",(unsigned)ofs, (unsigned )(ofs +len));
		}else{
			
			DEBUG("In function  %s write_status_reg failed\n",__FUNCTION__);		
		}		
	}
	DEBUG(" 1 Status reg = 0x%x", read_status_reg(flash));
	up(&flash->lock);
	return retval; 
}

/* Power Management functions */
static int winbond_suspend (struct mtd_info *mtd)
{	
	struct winbond *flash = mtd_to_winbond(mtd);
	struct spi_transfer t[2];
	struct spi_message m;
	memset(t, 0, (sizeof t));
	spi_message_init(&m);

	t[0].tx_buf = flash->command;
	/* operating in Core SPI Mode CMD length needed is 4 bytes no 
	 *  dummy byte required 
	 */
	t[0].len = WINBOND_CMD_FRAME_SIZE;
	spi_message_add_tail(&t[0], &m);

  	down(&flash->lock);

	DEBUG("Inside the function %s:\n",__FUNCTION__);
	write_enable(flash);

	/* Wait until finished previous write command.*/
	if( wait_till_ready(flash) )
	{
		up(&flash->lock);
		printk("wait_till_ready failed returning from function  line %s %d\n",
				__FUNCTION__,__LINE__);
		return 1;
	}
	/* Set up the opcode in the write buffer. */
	flash->command[0] = OPCODE_DEEP_POWER_DOWN;
	flash->command[1] = DUMMY_ADDRESS >> 16;
	flash->command[2] = DUMMY_ADDRESS >> 8;
	flash->command[3] = DUMMY_ADDRESS;
	printk("Going to Deep Sleep ..............Bye\n");
	spi_sync(flash->spi, &m);
  	up(&flash->lock);
	return 0;

}	

static void winbond_resume (struct mtd_info *mtd)
{
	struct winbond *flash = mtd_to_winbond(mtd);
	struct spi_transfer t[2];
	struct spi_message m;

	memset(t, 0, (sizeof t));
	spi_message_init(&m);

	t[0].tx_buf = flash->command;
	/* operating in Core SPI Mode CMD length needed is 4 bytes no 
	 *  dummy byte required 
	 */
	t[0].len = WINBOND_CMD_FRAME_SIZE;
	spi_message_add_tail(&t[0], &m);

	/* No other command works just wake up First */
  	down(&flash->lock);
	DEBUG("Inside the function %s\n",__FUNCTION__);
	/* Set up the opcode in the write buffer. */
	flash->command[0] = OPCODE_RELEASE_POWER_DOWN;
	flash->command[1] = DUMMY_ADDRESS >> 16;
	flash->command[2] = DUMMY_ADDRESS >> 8;
	flash->command[3] = DUMMY_ADDRESS;
	printk("waking Up form ........ Deep Sleep Hi :)\n");
	spi_sync(flash->spi, &m);
  	up(&flash->lock);
}


/*
 *winbond_sfi_transfer Client transfer function used in SFI mode of 
 *operation. 
 * On success returns number of bytes transfered otherwise 0.	
 */
int winbond_sfi_transfer(struct spi_device *spi, struct spi_transfer *t)
{
	int i = 0;
    struct spasnion_sfi_transfer_hdr_t *sfi_hdr = NULL; 

	 struct avalanche_sfi_dev_info_t *sfi = 
		(struct avalanche_sfi_dev_info_t *)spi->controller_data;	

	volatile unsigned char *s_ptr = NULL;
	volatile unsigned char *d_ptr = NULL;

	DEBUG("Inside winbond_sfi_transfer \n");	

	if(sfi->mode != AVALANCHE_SFI_MODE) {
		DEBUG("Spansion: Not in SFI mode\n");
		return 0;
	}

	if(sfi == NULL)
	{
		printk(" Got NULL Pointer sfi = 0x%x\n",(unsigned)sfi);
		return 0;
	}	

	if(t->rx_buf != NULL ){
	   DEBUG("t->rx_buf is 0x%x\n",(unsigned)t->rx_buf );
	   sfi_hdr = (struct spasnion_sfi_transfer_hdr_t*)(t->rx_buf);
	}
	else
	{	
		DEBUG("t->rx_buf is NULL 0x%x\n",(unsigned)t->rx_buf );
		return 0;
	}

    if(sfi_hdr->rx_buff)
	{
		d_ptr = (volatile unsigned char*)(sfi_hdr->rx_buff);
		s_ptr = (volatile unsigned char*)( sfi->sfi_base + 
				(unsigned)(sfi_hdr->rx_addr));
		DEBUG("Doing RX \n");	

		DEBUG("Copyng the data to rx_bf [d_ptr=0x%0x s_ptr=0x%0x] \n",
				(unsigned) d_ptr,(unsigned) s_ptr);
	}

    if(sfi_hdr->tx_buff)
	{
		s_ptr = (unsigned char*)(sfi_hdr->tx_buff);
		d_ptr = (unsigned char*)(sfi->sfi_base + 
				(unsigned int)(sfi_hdr->tx_addr));
		DEBUG("Doing TX \n");	
		DEBUG("Copyng the data to tx_bf [d_ptr=0x%0x s_ptr=0x%0x] \n",
				(unsigned) d_ptr,(unsigned) s_ptr);
	}

	for(i = 0; i< sfi_hdr->len; i++ )
	{
		d_ptr[i] = s_ptr[i]; 

	}	
	DEBUG("\nlength in %s : %d\n", __FUNCTION__, sfi_hdr->len);
    return (int)(sfi_hdr->len);
}

static int winbond_probe(struct spi_device *spi)
{
	unsigned int i;
	struct winbond	*flash;
	struct flash_platform_data	*data;

	struct avalanche_sfi_dev_info_t *sfi = NULL;
	DEBUG("Inside the function %s\n",__FUNCTION__);

	data = spi->dev.platform_data;

    flash = kzalloc(sizeof *flash, SLAB_KERNEL);
    if (!flash)
	{
		printk(" Kzalloc failed in winbond_probe for flash\n");
        return -ENOMEM;
	}

	flash->spi = spi;
	/*
	 * Initialise the controller data  set Read/Fast_read, write , 
	 * read mode(Noraml/Fast) for SFI transfer
	 */
	sfi	=( struct avalanche_sfi_dev_info_t *)spi->controller_data;
	if(  sfi != NULL )
	{
		sfi->initialized = 0;
		sfi->mode 		= AVALANCHE_CORE_SPI_MODE; 
		sfi->write_cmd 	= OPCODE_PAGE_PROGRAM;

		if(winbond_read_mode == WINBOND_NORMAL_READ){
			sfi->dual_read 			= DUAL_READ_DISABLE;
			sfi->num_dummy_bytes	= NORMAL_READ_NUM_DUMMY_BYTES; 
			/* Num adddrss byte count starts form zero i,e -1  */
			sfi->num_addr_bytes 	= (WINBOND_NUM_ADDR_BYTES - 1);
			sfi->read_cmd 			= OPCODE_READ; 
		}else if(winbond_read_mode == WINBOND_FAST_READ){	
			sfi->dual_read 			= DUAL_READ_DISABLE;
			sfi->num_dummy_bytes	= FAST_READ_NUM_DUMMY_BYTES; 
			/* Num adddrss byte count starts form zero i,e -1  */
			sfi->num_addr_bytes 	= (WINBOND_NUM_ADDR_BYTES - 1);
			sfi->read_cmd 			= OPCODE_FAST_READ; 
		}else{
			/* Fast Read CMD values */
			sfi->dual_read 			= DUAL_READ_ENABLE;
			sfi->num_dummy_bytes 	= DUAL_READ_NUM_DUMMY_BYTES; 
			/* Num adddrss byte count starts form zero i, e -1 */
			sfi->num_addr_bytes 	= (WINBOND_NUM_ADDR_BYTES -1);
			sfi->read_cmd 			= OPCODE_FAST_READ_DUAL_OP; 
		}

		sfi->sfi_transfer = winbond_sfi_transfer;
		DEBUG("SFI = 0x%0x  Operating in SFI mode with write_cmd = 0x%x, \
			   read_cmd = 0x%x dual_read = %d num_dummy_bytes = %d \
			   num_addr_bytes %d\n",(unsigned int)sfi,
               (unsigned int)sfi->write_cmd,(unsigned int)sfi->read_cmd,
               sfi->dual_read, sfi->num_dummy_bytes,sfi->num_addr_bytes); 
	}	
	else
	{
		DEBUG(" Operating in Core SPI mode\n");
	}

	init_MUTEX(&flash->lock);
	dev_set_drvdata(&spi->dev, flash);
	flash->mtd.name 		= "winbond";
	flash->mtd.type		 	= MTD_NORFLASH;
	flash->mtd.writesize 	= FLASH_PAGESIZE;
	flash->mtd.flags		= MTD_CAP_NORFLASH;
	flash->mtd.size 		= SECTOR_ERASE_SIZE * NUM_SECTORS;
	flash->mtd.erasesize 	= SECTOR_ERASE_SIZE;
	flash->mtd.numeraseregions = NUM_REGIONS;
	flash->mtd.eraseregions = &winbond_erase_regions[0];
	flash->mtd.erase 		= winbond_erase;
	flash->mtd.read 		= winbond_read;
	flash->mtd.write 		= winbond_write;
	flash->mtd.lock  		= winbond_lock;
	flash->mtd.unlock 		= winbond_unlock;
	flash->mtd.suspend 		= winbond_suspend;
	flash->mtd.resume 		= winbond_resume;

	DEBUG("mtd .name = %s, .size = 0x%.8x (%uM) " 
		  ".erasesize = 0x%.8x (%uK) .numeraseregions = %d\n",
		  flash->mtd.name,	flash->mtd.size,
          flash->mtd.size / (1024*1024),flash->mtd.erasesize, 
          flash->mtd.erasesize / 1024,	flash->mtd.numeraseregions);

	/* partitions should match sector boundaries; and it may be good to
	 * use readonly partitions for writeprotected sectors (BP2..BP0).
	 */
	if( mtd_has_partitions() )
	 {
		int	 nr_parts = 0;
		struct mtd_partition	*parts = NULL;

#ifdef CONFIG_MTD_CMDLINE_PARTS
		static const char *part_probes[] = { "cmdlinepart", NULL,};
		nr_parts = parse_mtd_partitions(&flash->mtd,part_probes, &parts, 0);
#endif
		if( nr_parts <= 0 && data && data->parts ) 
		{
			parts = data->parts;
			nr_parts = data->nr_parts;
		}

		if (nr_parts > 0) {
			for (i = 0; i < nr_parts; i++){
				printk("partitions[%d] = ""{.name = %s, .offset = 0x%.8x," 
                      ".size = 0x%.8x (%uK) }\n",i, parts[i].name,
					  parts[i].offset,parts[i].size,
	                  parts[i].size / 1024);
			}
			flash->partitioned = 1;
			return add_mtd_partitions(&flash->mtd, parts, nr_parts);
		}
	}else if( data->nr_parts ){
		printk("ignoring %d default partitions on %s\n",data->nr_parts,
       		    data->name );
	}
	return add_mtd_device(&flash->mtd) == 1 ? -ENODEV : 0;
}


static int winbond_remove(struct spi_device *spi)
{
	struct 	winbond *flash = dev_get_drvdata(&spi->dev);
	int		status;

	DEBUG("Inside the function %s\n",__FUNCTION__);
	/* Clean up MTD stuff. */
	if( mtd_has_partitions() && flash->partitioned ){
		status = del_mtd_partitions(&flash->mtd);
	}else{
		status = del_mtd_device(&flash->mtd);
	}

	if (status == 0) {
		kfree(flash);
	}
	return 0;
}


static struct spi_driver winbond_mtd_driver = {
    .driver = {
        .name       = "winbond",
        .bus        = &spi_bus_type,
        .owner      = THIS_MODULE,
    },
    .probe   = winbond_probe,
    .remove  = __devexit_p(winbond_remove),
};

static int winbond_init( void )
{
	DEBUG("Inside the function %s\n",__FUNCTION__);
#ifdef CONFIG_MTD_WINBOND_MODULE
	if( sfi_mode == AVALANCHE_CORE_SPI_MODE || 
		 sfi_mode == AVALANCHE_SFI_MODE ) {
		winbond_sfi_mode = sfi_mode;	
	}else{	
		DEBUG("setting winbond_sfi_mode to default  SFI MODE\n");
		winbond_sfi_mode = AVALANCHE_SFI_MODE;
	}
	DEBUG(" winbond_sfi_mode = %d\n",winbond_sfi_mode);

	if( read_mode == WINBOND_NORMAL_READ ||
		read_mode == WINBOND_FAST_READ ||
		 read_mode == WINBOND_DUAL_READ) {
		winbond_read_mode = read_mode;
	}else{
		DEBUG("setting winbond_read_mode to default DUAL read Mode\n");
		winbond_read_mode = WINBOND_DUAL_READ;
	}
	DEBUG("winbond_read_mode = %d\n",winbond_read_mode);
#endif
	return spi_register_driver(&winbond_mtd_driver);
}


static void winbond_exit( void )
{
	DEBUG("Inside the function %s\n",__FUNCTION__);
	spi_unregister_driver(&winbond_mtd_driver);
}


module_init(winbond_init);
module_exit(winbond_exit)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("R.Srinath/Mansoor Ahmed  Texas Instruments India Pvt Ltd");
MODULE_DESCRIPTION("MTD SPI driver for Spansion Serial flash chip");
module_param(sfi_mode,int, S_IRUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(sfi_mode,"\n\t\t Set sfi_mode = 1 for SFI mode, sfi_mode = 0 for CORE SPI mode<Range:0/1> Default is SFI mode");
module_param(read_mode,int, S_IRUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(read_mode,"\n\t\t Set read_mode = 0 for NORMAL READ, read_mode  = 1 for FAST READ read_mode  = 2 Fast Reas Dual Output <Range:0 to 2>Default is Fast Reas Dual Output");
