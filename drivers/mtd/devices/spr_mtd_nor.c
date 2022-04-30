/*
 * linux/drivers/mtd/devices/sp_sflash.c
 *
 *  Copyright (C) 2006 ST MICROELECTRONICS
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
 * AUTHOR :  Arif khan arif.khan@st.com> */

#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <asm/io.h>
#include <asm/arch/irqs.h>
#include <linux/mtd/map.h>
#include <linux/mtd/concat.h>

/* generalizing the place */
#include <asm/arch/spr_nor.h>

#include "spr_mtd_nor_mem_bank.h"
#include "spr_mtd_nor_partition.h"

/* #define SPEAR_DRIVER_DEBUG     1 */

/* global variable decleration */

unsigned int opComplete = 1;    /* Operation completion flag */
unsigned int aligned_write = 0; /* write align flag          */ 
int    sflash_exists = 0;     /* sets if there exists a flash */
int    chip_erased   = 0;     /* erase flag                   */
unsigned int devices_found=0; /* number of flash chips found  */


/************************ interrupt handler *********************************/

/****************************************************************************/
/* NAME:           smi_int_handler		                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This is an interrupt handler					    */
/*      conversion                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :   None                                                             */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        None                                                               */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

void 
smi_int_handler (void)
{
    volatile unsigned int val;
    val = SMICntl->SMI_SR;
    if (val & CHECK_WCF)
    {
        SMICntl->SMI_SR &= ~(WCF); /* WriteCompleteFlag interrupt clear */
        opComplete = 1;
    }
    else
    {
        if (val & CHECK_TFF)
    	SMICntl->SMI_SR &= ~(TFF); /* TranserFinishFlag interrupt clear */
	opComplete = 1;
    }

printk("Entered the smi Interrupt \n");
}

/**************** irq structure  **************/
static struct irqaction spearhead_smi_irq = {
    .name           = "Spearhead smi",
    .flags          = SA_INTERRUPT,
    .handler        = smi_int_handler
};

/********************************** init ************************************/

/****************************************************************************/
/* NAME:          smi_init(unsigned int number)			            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine initializes the SMI                                    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :   number: The number of flash banks present                        */
/*                                                                          */
/* OUT :                                                                    */
/*         None                                                             */
/*                                                                          */
/* RETURN:                                                                  */
/*         None                                                               */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

void 
smi_init (unsigned int number)
{
    unsigned int tmp = 0xffffffff;
    unsigned int i;
    setup_irq( IRQ_BASIC_SMI ,&spearhead_smi_irq );
        
    /* Setting the standard values */
    SMICntl->SMI_CR1 = BANK_EN | DSEL_TIME | PRESCALA;
    sflash_exists = 1;

    /* changed the length from 1 to 4 as */
    /* we will have smi_write in 4 bytes mode. */
    for(i=0;i<number;i++)
    {
        smi_write_hacked(&tmp,(unsigned int*)(0x7c00),4,i);
    //new    smi_write_hacked(&tmp,(unsigned int*)FLASH_START_ADDRESS,4,i);
        SMICntl->SMI_SR |= (WCF); 
    }
}


/****************************************************************************/
/* NAME:  sflash_erase ( struct mtd_info *mtd,				    */
/*			 struct erase_info *instr		            */
/*                     )						    */	
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine erases the flash blocks as specified in the address    */					
/*      range in instr							    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - mtd  : Provides the memory characteristics                        */
/*      - instr: Provides address information                               */
/*                                                                          */
/* INOUT : instr                                                            */
/*                                                                          */
/* RETURN:                                                                  */
/*        int                                                               */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

static int 
sflash_erase (struct mtd_info *mtd, struct erase_info *instr)
{
   //int		page;
   int   	len;
   //int   	pages_per_block;
   int 		ret;
   uint32_t      sflash_start_addr;
   //uint32_t      sflash_blk_num;
   //int           total_blks;
  // int 	        block_shift;
   unsigned int  sectorId;
   unsigned int  numofsectors;
   int 		index;
   struct sflash_chip   *this = mtd->priv;

  DECLARE_WAITQUEUE(wait, current);

  DEBUG (MTD_DEBUG_LEVEL3,
	 "sflash_erase: start = 0x%08x, len = %i\n",
	 (unsigned int) instr->addr, (unsigned int) instr->len);

  /* Start address must align on block boundary */
  if (instr->addr & (mtd->erasesize - 1)) 
  {
      DEBUG (MTD_DEBUG_LEVEL0,
	   "sflash_erase: Unaligned address\n");
      return -EINVAL;
  }

  /* Length must align on block boundary */
  if (instr->len & (mtd->erasesize - 1)) 
  {
      DEBUG (MTD_DEBUG_LEVEL0,
	   "sflash_erase: Length not block aligned\n");
      return -EINVAL;
  }

  /* Do not allow erase past end of device */
  if ((instr->len + instr->addr) > mtd->size) 
  {
      DEBUG (MTD_DEBUG_LEVEL0,
	   "sflash_erase: Erase past end of device\n");
      return -EINVAL;
  }

  retry:
  /* Grab the lock and see if the device is available */
  spin_lock_bh (&this->chip_lock);

  switch (this->state) 
  {
      case SF_READY:
          this->state = SF_ERASI;
          break;

      default:
          set_current_state (TASK_UNINTERRUPTIBLE);
          add_wait_queue (&this->wq, &wait);
          spin_unlock_bh (&this->chip_lock);
          schedule();
          remove_wait_queue (&this->wq, &wait);
          goto retry;
  };

  len = instr->len;
  sflash_start_addr = instr->addr;
  instr->state = MTD_ERASING;

  /* Get sectorID to be erased */ 
  sectorId = getSectorID(sflash_start_addr, memory_indices[mtd->index]);

  /* calculate the number of sectors to be erased */
  numofsectors = instr->len / mtd->erasesize;
  
  for(index = 0;index < numofsectors; index++)
  {
      unsigned int abs_sector;
      int err;

      /* get the absolute sector to be erased on particular chip */
      abs_sector= ((unsigned int )(index+ sectorId))
	           * (sflash_ids[memory_indices[mtd->index]].sectorsize );
   
      /* erase the sector */  
      err = smi_sector_erase(mtd->index,abs_sector); 

      if(err == -1)
      {
	  printk("FLASH ERASE ERROR");
		}
      }  /* end of for */ 
  
  spin_unlock_bh (&this->chip_lock); 
  spin_lock_bh (&this->chip_lock);

  instr->state = MTD_ERASE_DONE;

  spin_unlock_bh (&this->chip_lock);

  ret = instr->state == MTD_ERASE_DONE ? 0 : -EIO;;

  /* Do call back function */
  if (!ret && instr->callback)
      instr->callback (instr);

  /* The device is ready */
  spin_lock_bh (&this->chip_lock);
  this->state = SF_READY;
  spin_unlock_bh (&this->chip_lock);

  /* Return OK */
  return ret;
} 	


/****************************************************************************/
/* NAME:  int getSectorID ( unsigned int start_addr,                        */
/*                          int memory_index                                */
/*                     )						    */	
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine erases the flash blocks as specified in the address    */					
/*      range in instr							    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - start_addr  : Start address                                       */
/*      - memory_index: index of the flash in sflash_ids                    */
/*                                                                          */
/* OUT : None                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        sectorID : sector ID of the given address                         */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

unsigned int 
getSectorID(unsigned int start_addr, int memory_index)
{
    unsigned int sectorId;
    sectorId=(start_addr/sflash_ids[memory_index].sectorsize); 
    return sectorId;
}


/*                           Bulk erase command computation                 */
/******************************************************* ********************/
/* NAME:  int get_bulk_erase_cmd ( unsigned int memId )                     */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine finds the bulk erase commad for the flash block        */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - memId: index of the flash in sflash_ids                           */
/*                                                                          */
/* OUT : None                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        unsigned int : bulk erase command for the flash                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

unsigned int 
get_bulk_erase_cmd (unsigned int memId)
{
    unsigned int bulk_erase_cmd;
    unsigned int vendor;
    unsigned int sstIf;
    vendor = memId&0x000000FF;
    switch(vendor)
    {
	case 0x00000020:      /* ST Microelectronics*/
	    bulk_erase_cmd=0x000000C7;
	    if((memId &0x0000FF00) == 0x00004000)
	    bulk_erase_cmd=0x0; /* Instruction not supported for M45 Series */	
	    break;
			
	case 0x0000001F:     /* Atmel */      
	    bulk_erase_cmd=0x00000062;
	    if((memId & 0x00FF0000) == 0x00040000)
	    {
	        bulk_erase_cmd=0x000000C7;
	    }
	    break;

	case 0x00000001:    /* Spansion */
	    bulk_erase_cmd=0x000000C7;
	    break;
			
	case 0x000000C2:    /* Macronix */
	    bulk_erase_cmd=0x000000C7;
	    break;
			
	case 0x000000BF:    /* SST  */
	    bulk_erase_cmd=0x000000C7;
	    sstIf=memId &0x00FF0000;
	    if(sstIf ==0x00430000 || sstIf ==0x00440000 
		                  ||sstIf ==0x00800000 ) 
 	        bulk_erase_cmd=0x00000052;	
	    break;
			
	default: 
	    bulk_erase_cmd=0x00000000;	
	    break;

    }
       return bulk_erase_cmd;	
}


/****************************************************************************/
/* NAME:  int get_sector_erase_cmd ( unsigned int memId )                   */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine finds the sector erase commad for the flash bloc       */
/*      ID given by memId                                                   */					
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - memId: index of the flash in sflash_ids                           */
/*                                                                          */
/* OUT : None                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        unsigned int : sector erase command for the flash                 */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

unsigned int 
get_sector_erase_cmd (unsigned int memId)
{
    unsigned int sector_erase_cmd;
    unsigned int vendor;
    unsigned int sstIf;
    vendor = memId&0x000000FF;
    switch(vendor)
    {
        case 0x00000020:      /* ST Microelectronics*/
	    sector_erase_cmd=0x000000D8;
	    break;
			
	case 0x0000001F:     /* Atmel */      
	    sector_erase_cmd=0x00000052;
	    if((memId & 0x00FF0000) == 0x00040000)
	    {
		sector_erase_cmd=0x000000D7;
 	    }
	    break;

	case 0x00000001:    /* Spansion */
	    sector_erase_cmd=0x000000D8;
	    break;
			
	case 0x000000C2:    /* Macronix */
	    sector_erase_cmd=0x000000D8;
	    break;
			
	case 0x000000BF:    /* SST  */
	    sector_erase_cmd=0x00000020;
	    sstIf=memId &0x00FF0000;
	    if(sstIf ==0x00430000 || sstIf ==0x00440000 
				  || sstIf ==0x00800000 ) 
		sector_erase_cmd=0x00000020;	
	    break;
			
	default: 
	    sector_erase_cmd=0x00000000;	
 	    break;

    }

    return sector_erase_cmd;	
}


/****************************************************************************/
/* NAME:  smi_bank_erase ( unsigned int bank )                              */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine erases the complete memory bank                        */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - bank: memory bank to be erased                                    */
/*                                                                          */
/* OUT : None                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        int                                                               */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

int 
smi_bank_erase (unsigned int bank)
{
/**** NOTE: BANK ERASE IS PERFORMED IN SW MODE ****/

    unsigned int B_SEL=0;
    //unsigned int SAVE_CR1;
    //unsigned int SAVE_CR2;
    unsigned int WM;
    unsigned int memId;
    unsigned int bulk_erase_command;

    switch(bank)
    {
	case BANK0: B_SEL = BANK0_SEL; WM = WM0; break;
	case BANK1: B_SEL = BANK1_SEL; WM = WM1; break;
	case BANK2: B_SEL = BANK2_SEL; WM = WM2; break;
	case BANK3: B_SEL = BANK3_SEL; WM = WM3; break;
    }

    /*** Saving CR1-2 ***/
    /*** Clearing Error Flags ***/
    SMICntl->SMI_SR &= ~(ERF1 | ERF2);

    if (smi_read_id(bank) != 0)
    {
        /*** Programming CR1 in a Known State ***/
        SMICntl->SMI_CR1 |= SW_MODE;

	/*** Clear TFF ***/
	SMICntl->SMI_SR &= ~TFF;

 	/*** Clearing Error Flags ***/
      	SMICntl->SMI_SR &= ~(ERF1 | ERF2);

	/*** Check if Flash is WIP in HW mode***/
	SMICntl->SMI_CR2 = B_SEL | RD_STATUS_REG;
	while (!(SMICntl->SMI_SR & TFF));
	while (SMICntl->SMI_SR & WIP_BIT)
	{
	    SMICntl->SMI_SR &= ~TFF;
	    SMICntl->SMI_CR2 = B_SEL | RD_STATUS_REG;
	    while (!(SMICntl->SMI_SR & TFF));
	}

	/*** Clear TFF ***/
	SMICntl->SMI_SR &= ~TFF;

	/*********** Put the Flash in WE and check ************/
	SMICntl->SMI_CR2 = WE | B_SEL;

	while (!(SMICntl->SMI_SR & TFF));

	/*** Clear TFF ***/
	SMICntl->SMI_SR &= ~TFF;

	/*** Check if Flash is WEL in HW mode***/
	SMICntl->SMI_CR2 = B_SEL | RD_STATUS_REG;
	while (!(SMICntl->SMI_SR & TFF));
	while (!(SMICntl->SMI_SR & WEL_BIT))
	{
	    SMICntl->SMI_SR &= ~TFF;
	    SMICntl->SMI_CR2 = B_SEL | RD_STATUS_REG;
	    while (!(SMICntl->SMI_SR & TFF));
	}

	/*** Clear TFF ***/
	SMICntl->SMI_SR &= ~TFF;
	memId=smi_read_id(bank); 
	bulk_erase_command=get_bulk_erase_cmd(memId);
	SMICntl->SMI_TR  = bulk_erase_command;
	SMICntl->SMI_CR2 = B_SEL |SEND | TX_LEN_1;
	while (!(SMICntl->SMI_SR & TFF));
	/*** Clear TFF ***/
	SMICntl->SMI_SR &= ~TFF;

	/*** Check if Flash is WIP in HW mode***/
	SMICntl->SMI_CR2 = B_SEL | RD_STATUS_REG;
	while (!(SMICntl->SMI_SR & TFF));
	while (SMICntl->SMI_SR & WIP_BIT)
	{
	    SMICntl->SMI_SR &= ~TFF;
	    SMICntl->SMI_CR2 = B_SEL | RD_STATUS_REG;
	    while (!(SMICntl->SMI_SR & TFF));
	}

	/*** Clear TFF ***/
	SMICntl->SMI_SR &= ~TFF;
	/*** Checking for Errors ***/
	if ((SMICntl->SMI_SR & (ERF1 | ERF2)))
	{
	    /*** Put SMI in HW mode ***/
	    SMICntl->SMI_CR1 &= ~(SW_MODE);

	    /*** Reloading CR1-2 ***/
	    return(-1);
	}
	else
	{
	    /*** Put SMI in HW mode ***/
	    SMICntl->SMI_CR1 &= ~(SW_MODE);
  	    /*** Reloading CR1-2 ***/
	    return(0);
	}
    }
    else
    {
	/*** Put SMI in HW mode ***/
	SMICntl->SMI_CR1 &= ~(SW_MODE);

	/*** Reloading CR1-2 ***/
	return(-1);
    }
}


/***************************** SECTOR Erase ****************************/

/****************************************************************************/
/* NAME:  smi_sector_erase ( unsigned int bank,                             */
/*                           unsigned int sector			    */
/*                          )						    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine erases the complete memory bank                        */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - bank: memory bank on which the sector to be erased                */
/*      - sector: ID of the sector to be erased                             */
/*                                                                          */
/* OUT : None                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        int                                                               */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

int 
smi_sector_erase (unsigned int bank, unsigned int sector)
{
/**** NOTE: SECTOR ERASE IS PERFORMED IN SW MODE ****/

    unsigned int B_SEL=0;
    //unsigned int SAVE_CR1;
    //unsigned int SAVE_CR2;
    unsigned int sect_add;
    unsigned int Instruction;
    unsigned int WM;
    unsigned int memId;
    unsigned int sector_erase_command;
    unsigned int abs_sector;

    switch(bank)
    {
	case BANK0: B_SEL = BANK0_SEL; WM = WM0; break;
	case BANK1: B_SEL = BANK1_SEL; WM = WM1; break;
	case BANK2: B_SEL = BANK2_SEL; WM = WM2; break;
	case BANK3: B_SEL = BANK3_SEL; WM = WM3; break;
    }

    sect_add = sector;  
	
    #ifdef SPEAR_DRIVER_DEBUG
	printk("Erasing sector %d \n", sector/ sflash_ids[memory_indices[bank]].sectorsize);
    #endif
   
   abs_sector=(unsigned int) (sector/sflash_ids[memory_indices[bank]].sectorsize);

    memId=smi_read_id(bank); 
    sector_erase_command=get_sector_erase_cmd(memId);
	
    /* Instruction = ((sect_add>>8)&0x0000FF00)|sector_erase_command; Old one */

    if ( memId == SFLASH_DEVID_M25P10 )
      {
      Instruction = ((sect_add<<8)&0xFFFFFF00)|sector_erase_command; /* Hack Siva Borra */ 
	
        if ( abs_sector == 0x2)
        {
        Instruction = (0x01111100) | sector_erase_command; // Hack Siva Borra 
        }
        
        if(abs_sector == 0x3)
	{
        Instruction = (0xFFFFFF00) | sector_erase_command; // Hack Siva Borra 
        }
     }
     else
     {
     Instruction = ((abs_sector<<8)&0x0000FF00 )| sector_erase_command; /* Hack Siva Borra */
     }

	 
    /*** Clearing Error Flags ***/
    SMICntl->SMI_SR &= ~(ERF1 | ERF2);

    if (smi_read_id(bank) != 0)
    {
       	/*** Programming CR1 in a Known State ***/
		
	SMICntl->SMI_CR1 |= SW_MODE;

	/*** Clear TFF ***/
	SMICntl->SMI_SR &= ~TFF;

 	/*** Clearing Error Flags ***/
 	SMICntl->SMI_SR &= ~(ERF1 | ERF2);

	/*** Check if Flash is WIP in HW mode***/
	SMICntl->SMI_CR2 = B_SEL | RD_STATUS_REG;
	while (!(SMICntl->SMI_SR & TFF));
	while (SMICntl->SMI_SR & WIP_BIT)
	{
	    SMICntl->SMI_SR &= ~TFF;
	    SMICntl->SMI_CR2 = B_SEL | RD_STATUS_REG;
	    while (!(SMICntl->SMI_SR & TFF));
	}

	/*** Clear TFF ***/
	SMICntl->SMI_SR &= ~TFF;

	/*********** Put the Flash in WE and check ************/
	SMICntl->SMI_CR2 = WE | B_SEL;

	while (!(SMICntl->SMI_SR & TFF));

	/*** Clear TFF ***/
	SMICntl->SMI_SR &= ~TFF;

	/*** Check if Flash is WEL in HW mode***/
	SMICntl->SMI_CR2 = B_SEL | RD_STATUS_REG;
	while (!(SMICntl->SMI_SR & TFF));
	while (!(SMICntl->SMI_SR & WEL_BIT))
	{
	    SMICntl->SMI_SR &= ~TFF;
	    SMICntl->SMI_CR2 = B_SEL | RD_STATUS_REG;
	    while (!(SMICntl->SMI_SR & TFF));
	}

	/*** Clear TFF ***/
	SMICntl->SMI_SR &= ~TFF;

	/*** Send SectorErase command in SW Mode ***/
	SMICntl->SMI_TR  = Instruction;
	SMICntl->SMI_CR2 = B_SEL | SEND | TX_LEN_4;
	while (!(SMICntl->SMI_SR & TFF));

	/*** Clear TFF ***/
	SMICntl->SMI_SR &= ~TFF;
	/*** Check if Flash is WIP in HW mode***/
	SMICntl->SMI_CR2 = B_SEL | RD_STATUS_REG;
	while (!(SMICntl->SMI_SR & TFF));
	while (SMICntl->SMI_SR & WIP_BIT)
	{
	    SMICntl->SMI_SR &= ~TFF;
	    SMICntl->SMI_CR2 = B_SEL | RD_STATUS_REG;
	    while (!(SMICntl->SMI_SR & TFF));
	}
	/*** Clear TFF ***/
	SMICntl->SMI_SR &= ~TFF;

	/*** Checking for Errors ***/
	if ((SMICntl->SMI_SR & (ERF1 | ERF2)))
	{
 	    /*** Put SMI in HW mode ***/
	    SMICntl->SMI_CR1 &= ~(SW_MODE);

	    /*** Reloading CR1-2 ***/
	    printk("\n error while erase???\n");
	    return(-1);
	}
	else
	{
	    /*** Put SMI in HW mode ***/
	    SMICntl->SMI_CR1 &= ~(SW_MODE);
	    /*** Reloading CR1-2 ***/
	    return(0);
	}
    }
    else
    {
	/*** Put SMI in HW mode ***/
	SMICntl->SMI_CR1 &= ~(SW_MODE);
	printk("\n Error while read ID?\n");

	/*** Reloading CR1-2 ***/
	return(-1);
    }
	
}


/****************************************************************************/
/* NAME:  sflash_read ( struct mtd_info *mtd,				    */
/* 			loff_t from,					    */
/*			size_t len,                                         */
/*			size_t *retlen,                                     */
/*			u_char *buf					    */
/*		      )							    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine performs read operation into as requested by the user  */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - mtd   : MTD information of the memory bank                        */
/*      - from  : from address						    */
/*      - len   : Number of words to be read                                */
/*                                                                          */
/* OUT : 								    */
/*      -retlen: Number of bytes read                                       */
/*      -buf   : buffer to which the data to be taken			    */
/*                                                                          */
/* RETURN:                                                                  */
/*        int                                                               */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

static int 
sflash_read( struct mtd_info *mtd,
			loff_t from,
			size_t len,
			size_t *retlen,
			u_char *buf )
{

  int erase_state = 0;
  struct sflash_chip *this = mtd->priv;

  DECLARE_WAITQUEUE(wait, current);

  DEBUG ( MTD_DEBUG_LEVEL3,
	  "sflash_read: from = 0x%08x, len = %i\n",
	  (unsigned int) from, (int) len );

  /* Do not allow reads past end of device */
  if ((from + len) > (mtd->size) )
  {
      DEBUG ( MTD_DEBUG_LEVEL0,
	    "sflash_read: Attempt read beyond end of device\n" );
      *retlen = 0;
      spin_lock_bh (&this->chip_lock);
      this->state = SF_READY;
      wake_up (&this->wq);
      spin_unlock_bh (&this->chip_lock);
      return -EINVAL;
  }

  /* Grab the lock and see if the device is available */
   retry:
   spin_lock_bh (&this->chip_lock);
   switch (this->state) {
   case SF_READY:
       this->state = SF_READI;
       spin_unlock_bh (&this->chip_lock);
       break;

  case SF_ERASI:
       this->state = SF_READI;
       erase_state = 1;
       spin_unlock_bh (&this->chip_lock);
       break;

  default:
       set_current_state (TASK_UNINTERRUPTIBLE);
       add_wait_queue (&this->wq, &wait);
       spin_unlock_bh (&this->chip_lock);
       schedule();
       remove_wait_queue (&this->wq, &wait);
    goto retry;
  };

  /* Initialize return value */
  *retlen = 0;

  if (sflash_exists<=0)
   {
    printk(KERN_ERR "sflash_read - serial flash NOT initialized.\n");
    spin_lock_bh (&this->chip_lock);
    this->state = SF_READY;
    wake_up (&this->wq);
    spin_unlock_bh (&this->chip_lock);
    return -EINVAL;
   }

   smi_read((unsigned char*)from, buf,len,mtd->index); 

  *retlen=len;

  /* Wake up anyone waiting on the device */
  spin_lock_bh (&this->chip_lock);
  if (erase_state)
      this->state = SF_ERASI;
  else
      this->state = SF_READY;
      wake_up (&this->wq);
      spin_unlock_bh (&this->chip_lock);

  /* Return OK */
  return 0;

}


/****************************************************************************/
/* NAME: smi_read (unsigned char * src_addr, 				    */
/*	 	   unsigned char* dst_addr,                                 */
/*             	   unsigned int length,                                     */
/*	           unsigned int bank                                        */
/*	           )							    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine performs read operation into as requested by the user  */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - src_addr : source address                                         */
/*      - length   : Number of words to be read                             */
/*      - bank     : The memory bank number				    */
/*                                                                          */
/* OUT : 								    */
/*      -dst_addr   : pointer to which the data to be taken		    */
/*                                                                          */
/* RETURN:                                                                  */
/*        None                                                               */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
void 
smi_read (unsigned char * src_addr, 
	       unsigned char* dst_addr, 
               unsigned int length, 
	       unsigned int bank
	 )
{
    unsigned int i;
    //unsigned int SAVE_CR1;
    //unsigned int SAVE_CR2;
    //unsigned char *dst, *src;

    /*** Saving CR1-2 ***/

    /* src_addr is a pointer so it is multiplied by 4 */
    switch(bank)
    {
	case BANK0: src_addr += 0xf8000000; break;
	case BANK1: src_addr += 0xf9000000; break;
        case BANK2: src_addr += 0xfa000000; break;
	case BANK3: src_addr += 0xfb000000; break;
    }

    /*** Put SMI in HW mode not WBT mode ***/
    SMICntl->SMI_CR1 &= ~(SW_MODE | WB_MODE);

    /*** Clear SR TFF ***/
    SMICntl->SMI_SR &= ~TFF;

    /*** Perform the read command ***/
    for(i=0; i<length; i++)
    {
	dst_addr[i] = src_addr[i];
    }
    
    /*** Clear SR TFF ***/
    /*** Reloading CR1-2 ***/
}



/****************************************************************************/
/* NAME:  sflash_write ( struct mtd_info *mtd,				    */
/* 			loff_t to,					    */
/*			size_t len,                                         */
/*			size_t *retlen,                                     */
/*			u_char *buf					    */
/*		      )							    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine performs write operation into as requested by the user  */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - mtd   : MTD information of the memory bank                        */
/*      - to    : to address						    */
/*      - len   : Number of bytes to be written                             */
/*      - buf   : buffer from which the data to be taken	            */
/*                                                                          */
/* OUT : 								    */
/*      -retlen: Number of bytes read                                       */
/*                                                                          */
/* RETURN:                                                                  */
/*        int                                                               */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int 
sflash_write( struct mtd_info *mtd,
			 loff_t to,
			 size_t len,
			 size_t *retlen,
			 const u_char *buf )
{
   // unsigned char * pdest;
   // int       i, cnt, sflash_blk_size;
    //uint32_t  regVal,sf_page_size,offset;

    struct sflash_chip *this = mtd->priv;

    DECLARE_WAITQUEUE(wait, current);

    DEBUG (MTD_DEBUG_LEVEL3,
	 "sflash_write: to = 0x%08x, len = 0x%x\n",
	 (unsigned int) to, (int) len);

    /* Do not allow write past end of page */
  if ((to + len) > mtd->size) 
  {
      DEBUG (MTD_DEBUG_LEVEL0,
	   "sflash_write: Attempted write past end of device\n");
      return -EINVAL;
  }

 retry:
     /* Grab the lock and see if the device is available */
     spin_lock_bh (&this->chip_lock);
     switch (this->state) 
     {
         case SF_READY:
                this->state = SF_WRITI;
    		spin_unlock_bh (&this->chip_lock);
    		break;

         default:
   		 set_current_state (TASK_UNINTERRUPTIBLE);
     		 add_wait_queue (&this->wq, &wait);
		 spin_unlock_bh (&this->chip_lock);
	 	 schedule();
   		 remove_wait_queue (&this->wq, &wait);
    		 goto retry;
    };

    /* Initialize return length value */
    *retlen = 0;


    if(sflash_exists<=0)
    {
  	printk(KERN_ERR "sflash_write - serial flash NOT initialized.\n");
        spin_lock_bh (&this->chip_lock);
        this->state = SF_READY;
        wake_up (&this->wq);
        spin_unlock_bh (&this->chip_lock);
        return -EIO;
    }
    #ifdef SPEAR_DRIVER_DEBUG
        printk(".");
    #endif

    if (smi_write(( u_char *)buf,(unsigned char *)to ,len,mtd->index) == -1)
    {
	DEBUG("Write Fail");
  	return -1;
    }
   stwsf_read_enable_sequence();

   /* Wake up anyone waiting on the device */
   spin_lock_bh (&this->chip_lock);
   this->state = SF_READY;
   wake_up (&this->wq);
   spin_unlock_bh (&this->chip_lock);

   /* Return OK */
   *retlen = len;
   return 0;

}


/****************************************************************************/
/* NAME: smi_write ( unsigned char * src_addr, 				    */
/*	 	    unsigned char* dst_addr,                                */
/*             	    unsigned int length,                                    */
/*	            unsigned int bank                                       */
/*	           )							    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine performs write operation into as requested by the user */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - src_addr : source address                                         */
/*      - length   : Number of bytes to be written                          */
/*      - bank     : The memory bank number				    */
/*                                                                          */
/* OUT : 								    */
/*      -dst_addr   : pointer to which the data to be written		    */
/*                                                                          */
/* RETURN:                                                                  */
/*        None                                                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
int 
smi_write (unsigned char* src_addr, 
	       unsigned char* dst_addr, 
	       unsigned int length, 
	       unsigned int bank
          )
{
    unsigned int i;
    unsigned int B_SEL=0;
   // unsigned int SAVE_CR1;
    //unsigned int SAVE_CR2;
    unsigned int WM=0;
    unsigned int *src, *dst;
    unsigned int page,k=64;
    unsigned long long *test1;
    unsigned long long *test2;

    /* src_addr is a pointer so it is multiplied by 4 */
    switch(bank)
    {
	case BANK0: 
            B_SEL = BANK0_SEL; 
            WM = WM0;  		
            dst_addr=dst_addr+0xf8000000;
            break;

        case BANK1: 
            B_SEL = BANK1_SEL; 
            WM = WM1; 
            dst_addr=dst_addr+0xf9000000;
            break;

	case BANK2: 
            B_SEL = BANK2_SEL;
            WM = WM2;
            dst_addr=dst_addr+0xfa000000;
            break;
	
	case BANK3:
            B_SEL = BANK3_SEL;
            WM = WM3;
            dst_addr=dst_addr+0xfb000000;
            break;
	}

	src = (unsigned int *)src_addr;
        dst = (unsigned int *)dst_addr;
	test1 = (unsigned long long *)src_addr;
        test2 = (unsigned long long *)dst_addr;
	page = (u32)dst_addr;

	#ifdef SPEAR_DRIVER_DEBUG
	printk("Writing SrcAddr=%x DestAddr=%x\n" , src, dst);
	#endif

        /*** Clearing Error Flags ***/
        SMICntl->SMI_SR &= ~(ERF1 | ERF2);

	/*** Put SMI in HW mode not WBT mode ***/
	SMICntl->SMI_CR1 &= ~(SW_MODE);

	/* Clear the write mode */
	SMICntl->SMI_SR &= ~(WM);

	/*** Clear SR TFF ***/
	SMICntl->SMI_SR &= ~TFF;

	/*** Check if Flash is WIP in HW mode***/
	SMICntl->SMI_CR2 = B_SEL | RD_STATUS_REG;
	while (!(SMICntl->SMI_SR & TFF));
	while (SMICntl->SMI_SR & WIP_BIT)
	{
		SMICntl->SMI_SR &= ~TFF;
		SMICntl->SMI_CR2 = B_SEL | RD_STATUS_REG;
		while (!(SMICntl->SMI_SR & TFF));
	}

	/*** Clear TFF ***/
	SMICntl->SMI_SR &= ~TFF;

	SMICntl->SMI_CR1 |= (WB_MODE);
	/*********** Put the Flash in WE and check ************/
	SMICntl->SMI_CR2 = WE | B_SEL;
	while (!(SMICntl->SMI_SR & WM));

        #if 1 
	{
	    int i,tmp=0xffffffff;
  	    for (i=0;i<length;i++) 
            {
	        page = ((u32)dst_addr) + (i);
		if ((page % 0x100) == 0) /* assuming page size 256 bytes */
		{
                    SMICntl->SMI_CR1 &= ~(SW_MODE | WB_MODE);
 		    for(k=0;k<100000;k++);
		    SMICntl->SMI_CR1 |= (WB_MODE);
                    SMICntl->SMI_CR2 = B_SEL | WE|RD_STATUS_REG;
                    while (!(SMICntl->SMI_SR & TFF));
                    SMICntl->SMI_SR &= ~TFF;
  	            while (!(SMICntl->SMI_SR & WM));
		}

	dst_addr[i]=src_addr[i];
	if(i==(length-1))
	{
	    /* Reset the burst mode  */
	    SMICntl->SMI_CR1 &= ~(WB_MODE);
	}				
	/* while (!(SMICntl->SMI_SR & WCF));*/
	/* SMICntl->SMI_SR &= ~TFF; */

        if ((SMICntl->SMI_SR & (ERF1 | ERF2)))
        {
            /*** Clear SR TFF ***/
            /*** Reloading CR1-2 ***/
 	    printk("Write failed\n");
            return(-1);
        }
    }

    smi_write_hacked(&tmp,(unsigned int *)(0x7c00),4,bank);
    SMICntl->SMI_SR |= (WCF);
    aligned_write=0;
    return 0;
    }

    #endif
 
    /*** Perform the write command ***/
    for(i=0; i<length; i++)
    {
	/*** Clear SR TFF ***/
	SMICntl->SMI_SR &= ~TFF;

	/*** Check if Flash is WIP in HW mode***/
	SMICntl->SMI_CR2 = B_SEL | RD_STATUS_REG;
	while (!(SMICntl->SMI_SR & TFF));
	while (SMICntl->SMI_SR & WIP_BIT)
	{
	    SMICntl->SMI_SR &= ~TFF;
	    SMICntl->SMI_CR2 = B_SEL | RD_STATUS_REG;
	    while (!(SMICntl->SMI_SR & TFF));
	}

	/*** Clear TFF ***/
	SMICntl->SMI_SR &= ~TFF;
	/*********** Put the Flash in WE and check ************/
	SMICntl->SMI_CR2 = WE | B_SEL;
	while (!(SMICntl->SMI_SR & WM));
	dst_addr[i] = src_addr[i];
	if ((SMICntl->SMI_SR & (ERF1 | ERF2)))
	{
 	    /*** Clear SR TFF ***/
	    /*** Reloading CR1-2 ***/
	    return(-1);
	}
    }
    /*** Clear SR TFF ***/
    SMICntl->SMI_SR &= ~(WCF); /* WriteCompleteFlag interrupt clear */

    /*** Reloading CR1-2 ***/
    return(0);
}


/****************************************************************************/
/* NAME: smi_write_hacked ( unsigned char * src_addr,			    */
/*	 	    unsigned char* dst_addr,                                */
/*             	    unsigned int length,                                    */
/*	            unsigned int bank                                       */
/*	           )							    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine performs write operation into as requested by the user */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - src_addr : source address                                         */
/*      - length   : Number of bytes to be written                          */
/*      - bank     : The memory bank number				    */
/*                                                                          */
/* OUT : 								    */
/*      -dst_addr   : pointer to which the data to be written		    */
/*                                                                          */
/* RETURN:                                                                  */
/*        None                                                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

int 
smi_write_hacked ( unsigned int* src_addr, 
		       unsigned int* dst_addr, 
		       unsigned int length, 
                       unsigned int bank
 	         )
{
    unsigned int i;
    unsigned int B_SEL=0;
    //unsigned int SAVE_CR1;
    //unsigned int SAVE_CR2;
    unsigned int WM=0;

/* src_addr is a pointer so it is multiplied by 4 */
    switch(bank)
    {
        case BANK0:
            B_SEL = BANK0_SEL; 
            WM = WM0; 
            dst_addr=dst_addr+(0xf8000000/4);
            break;
	case BANK1: 
            B_SEL = BANK1_SEL; 
            WM = WM1;
            dst_addr=dst_addr+(0xf9000000/4); 
            break;
	case BANK2: 
            B_SEL = BANK2_SEL; 
            WM = WM2;
            dst_addr=dst_addr+(0xfa000000/4);
            break;
	case BANK3: 
     	    B_SEL = BANK3_SEL; 
            WM = WM3;
            dst_addr=dst_addr+(0xfb000000/4); 
            break;
    }

    /*** Clearing Error Flags ***/
    SMICntl->SMI_SR &= ~(ERF1 | ERF2);

    /*** Put SMI in HW mode not WBT mode ***/
    SMICntl->SMI_CR1 &= ~(SW_MODE | WB_MODE);

    /* Clear the write mode */
    SMICntl->SMI_SR &= ~(WM);

    /*** Clear SR TFF ***/
    SMICntl->SMI_SR &= ~TFF;

    /*** Check if Flash is WIP in HW mode***/
    SMICntl->SMI_CR2 = B_SEL | RD_STATUS_REG;
    while (!(SMICntl->SMI_SR & TFF));
    while (SMICntl->SMI_SR & WIP_BIT)
    {
	SMICntl->SMI_SR &= ~TFF;
	SMICntl->SMI_CR2 = B_SEL | RD_STATUS_REG;
	while (!(SMICntl->SMI_SR & TFF));
    }

   /*** Clear TFF ***/
   SMICntl->SMI_SR &= ~TFF;
   /*********** Put the Flash in WE and check ************/
   SMICntl->SMI_CR2 = WE | B_SEL;
   while (!(SMICntl->SMI_SR & WM));

   #ifdef SPEAR_DRIVER_DEBUG
	printk(".");
   #endif
   /*** Perform the write command ***/
   for(i=0; i<length/4; i++)
   {
	/* assuming page size 256 bytes */
	if ((((unsigned int)(dst_addr)+(i*4))%0x100) == 0) 
        {
	    /*** Clear SR TFF ***/
	    SMICntl->SMI_SR &= ~TFF;
 	    /*** Check if Flash is WIP in HW mode***/
	    SMICntl->SMI_CR2 = B_SEL | RD_STATUS_REG;
	    while (!(SMICntl->SMI_SR & TFF));
	    while (SMICntl->SMI_SR & WIP_BIT)
	    {
		SMICntl->SMI_SR &= ~TFF;
		SMICntl->SMI_CR2 = B_SEL | RD_STATUS_REG;
		while (!(SMICntl->SMI_SR & TFF));
	    }

	    /*** Clear TFF ***/
	    SMICntl->SMI_SR &= ~TFF;
            /*********** Put the Flash in WE and check ************/
            SMICntl->SMI_CR2 = WE | B_SEL;
	    while (!(SMICntl->SMI_SR & WM));
	}

	dst_addr[i] = src_addr[i];
	if ((SMICntl->SMI_SR & (ERF1 | ERF2)))
	{
	    /*** Clear SR TFF ***/
	    /*** Reloading CR1-2 ***/
	    return(-1);
	}
    }
    /*** Clear SR TFF ***/
    SMICntl->SMI_SR &= ~(WCF); /* WriteCompleteFlag interrupt clear */

    /*** Reloading CR1-2 ***/
    return(0);
}


/****************************************************************************/
/* NAME:           sflash_sync( struct mtd_info *mtd )                      */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine ensures the the copy operations will take place        */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :   mtd : MTD information                                            */
/*                                                                          */
/* OUT :                                                                    */
/*        None				                                    */
/*                                                                          */
/* RETURN:                                                                  */
/*        None                                                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

static void 
sflash_sync ( struct mtd_info *mtd )
{
    struct sflash_chip *this = mtd->priv;
    DECLARE_WAITQUEUE(wait, current);
    DEBUG (MTD_DEBUG_LEVEL3, "sflash_sync: called\n");

 retry:
  /* Grab the spinlock */
  spin_lock_bh(&this->chip_lock);
  /* See what's going on */

  switch(this->state)
  {
      case SF_READY:
      case SF_SYNCI:
            this->state = SF_SYNCI;
    	    spin_unlock_bh (&this->chip_lock);
    	    break;
      default:
            /* Not an idle state */
            add_wait_queue (&this->wq, &wait);
            spin_unlock_bh (&this->chip_lock);
            schedule ();
            remove_wait_queue (&this->wq, &wait);
            goto retry;
  }

  /* Lock the device */
  spin_lock_bh (&this->chip_lock);

  /* Set the device to be ready again */
  if (this->state == SF_SYNCI) 
  {
      this->state = SF_READY;
      wake_up (&this->wq);
  }

  /* Unlock the device */
  spin_unlock_bh (&this->chip_lock);

  return;
}


/************************************************************************

  STW serial flash support routines.

*************************************************************************/


void 
stwsf_dump_chip_info ( void )
{
    DEBUG( MTD_DEBUG_LEVEL3,
    "STW serial flash chip info struct:\n");

}


void 
stwsf_dump_mtd_info (struct mtd_info *mtd)
{

  DEBUG( MTD_DEBUG_LEVEL3, "mtd_info struct:\n");
  DEBUG( MTD_DEBUG_LEVEL3, "type: 0x%x\n", mtd->type);
  DEBUG( MTD_DEBUG_LEVEL3, "flags: 0x%x\n", mtd->flags);
  DEBUG( MTD_DEBUG_LEVEL3, "total size: 0x%x\n", mtd->size);
  DEBUG( MTD_DEBUG_LEVEL3, "erasesize: 0x%x\n", mtd->erasesize);
  DEBUG( MTD_DEBUG_LEVEL3, "index: 0x%x\n", mtd->index);

}


/****************************************************************************/
/* NAME: stwsf_chip_probe ( struct mtd_info *mtd, 			    */
/*                          unsigned int no_flash_chip                      */
/*                        )                                                 */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine will check whether there exists a flash chip on a given*/
/*      memory bank ID                                                      */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - no_flash_chip     : The memory bank number			    */
/*                                                                          */
/* OUT : 								    */
/*      - mtd      : gets the characteristics of the flash found            */
/*                                                                          */
/* RETURN:                                                                  */
/*        None                                                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

static int 
stwsf_chip_probe ( struct mtd_info *mtd, unsigned int no_flash_chip )
{
    //uint32_t chip_size, sector_size;
    //int      i;
    int chip_index;
    unsigned int id;
    struct sflash_chip *this = mtd->priv;
    id = smi_read_id(no_flash_chip);

    if(id ==0) 
    {
        return -1;
    }

    chip_index=stwsf_get_chip_index(id);

    if (!mtd->size) 
    {
      mtd->name = sflash_ids[chip_index].name;
      mtd->erasesize = sflash_ids[chip_index].sectorsize;
      mtd->size = (1 << sflash_ids[chip_index].chipshift);
      this->chip_shift = sflash_ids[chip_index].chipshift;
      this->page_shift = sflash_ids[chip_index].pageshift;
      mtd->index = no_flash_chip;	
      memory_indices[mtd->index]=chip_index; 
      devices_found++;
    }
    
/*    DEBUG (MTD_DEBUG_LEVEL3,
	   "Serial flash device: (%s) Chip size: 0x%x(0x%x) blk size: 0x%x\n",
	   mtd->name, mtd->size, chip_size, sector_size);*/

    /* Initialize state and spinlock */
    this->state = SF_READY;
    init_waitqueue_head(&this->wq);
    spin_lock_init(&this->chip_lock);

    /* Print warning message for no device */
    if (!mtd->size)
    {
        printk (KERN_WARNING "No serial flash device was found at slot %d!!!\n",no_flash_chip);
        return -1;
    }

    /* Fill in remaining MTD driver data */
    mtd->type = MTD_NORFLASH;
    mtd->flags = MTD_CAP_NORFLASH;
    mtd->owner = THIS_MODULE;
    mtd->erase = sflash_erase;
    mtd->sync = sflash_sync;
    mtd->read = sflash_read;
    mtd->writesize = 1;
    mtd->write = sflash_write;
    mtd->point = NULL;
    mtd->unpoint = NULL;
    mtd->lock = NULL;
    mtd->unlock = NULL;
    mtd->suspend = NULL;
    mtd->resume = NULL;

  /* OK */
  return 0;

}
/**************** End of function stwsf_chip_probe  ********************/
 

/****************************************************************************/
/* NAME: stwsf_chip_erase()						    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*             This routine will erase all the flash chips                  */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      None								    */
/*                                                                          */
/* OUT : 								    */
/*      None							            */
/*                                                                          */
/* RETURN:                                                                  */
/*        int                                                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

static int 
stwsf_chip_erase (void)
{
    uint32_t regVal= 0;
    regVal = smi_bank_erase(BANK0) | smi_bank_erase(BANK1)
             | smi_bank_erase(BANK2)|smi_bank_erase(BANK3); 
    return regVal;

}

/****************************************************************************/
/* NAME: stwsf_read_enable_sequence()					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*             This routine will enable the sequece for read operation      */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      None								    */
/*                                                                          */
/* OUT : 								    */
/*      None							            */
/*                                                                          */
/* RETURN:                                                                  */
/*        None                                                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

void 
stwsf_read_enable_sequence (void)
{
    /*** Put SMI in HW mode not WBT mode ***/
    SMICntl->SMI_CR1 &= ~(SW_MODE | WB_MODE);

    /*** Clear SR TFF ***/
    SMICntl->SMI_SR &= ~TFF;
}


/****************************** Vendor And ID Read **************************/
/****************************************************************************/
/* NAME: smi_read_id ( unsigned int bank )				    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*             This routine will get the Vendor And ID Read of the flash    */
/*             chip present at the given bank				    */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - bank : bank ID on whcih we have to find                           */
/*                                                                          */
/* OUT : 								    */
/*      None							            */
/*                                                                          */
/* RETURN:                                                                  */
/*        int : manufacturer ID of the flash chip                           */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
unsigned int 
smi_read_id (unsigned int bank)
{
/**** NOTE: READ ID IS PERFORMED IN SW MODE ****/

    unsigned int B_SEL=0;
    unsigned int value;
    //unsigned int SAVE_CR1;
    //unsigned int SAVE_CR2;
    //unsigned int track=0;
    int i;
    static int boot_check;
    switch(bank)
    {
	case BANK0: B_SEL = BANK0_SEL; break;
	case BANK1: B_SEL = BANK1_SEL; break;
	case BANK2: B_SEL = BANK2_SEL; break;
	case BANK3: B_SEL = BANK3_SEL; break;
    }

    /*** Saving CR1-2 ***/
    /**************** put SMI in SW mode **************/
    SMICntl->SMI_CR1 |= SW_MODE;
    /*** Clear TFF ***/
    SMICntl->SMI_SR &= ~TFF;
    /*** Check if Flash is WIP in HW mode***/
    SMICntl->SMI_CR2 = B_SEL | RD_STATUS_REG;
    while (!(SMICntl->SMI_SR & TFF));
    if (SMICntl->SMI_SR & WIP_BIT)
    {
	return 0; /* There is no flash chip found at the location */
    }

    for(i=0;i<3;i++)
    {
        /*** Clear TFF ***/
	SMICntl->SMI_SR &= ~TFF;
	/*** Send ReadID command in SW Mode ***/
	SMICntl->SMI_TR  = read_ids[i];
	SMICntl->SMI_CR2 = B_SEL | SEND | TX_LEN_1 | RX_LEN_3;;
	/*** Check if Flash Read_id is Finished ***/
	while (!(SMICntl->SMI_SR & TFF));
	/*** Clear SR TFF ***/
	SMICntl->SMI_SR &= ~TFF;
	value = (SMICntl->SMI_RR & 0x00FFFFFF);
	if(stwsf_get_chip_index(value) != -1)
	{
	    break;
	}
	
	}
	/*** Reloading CR1-2 ***/
	/*** Returning FLASH ID value ***/

	/*** Put SMI in HW mode ***/
	SMICntl->SMI_CR1 &= ~(SW_MODE);
       
	/* Hack here to catch the M25P80 */
	

#ifdef CONFIG_MTD_STATIC_MEMORY_BANKS 


    if(i==3&&boot_check<4)
    {
	printk("\n Memory with unsupported RDID or \
                 unknown memory found at bank %d\n",bank);  
	printk(" The routine will work as per static mapping defined \n");
	boot_check++;
    }
	
    switch(bank)
    {
        case BANK0:
	    if(value != memory_bank_info.mem_bank1 
                          && boot_check <4 && i!=3)
  	    {
		printk("\n Mismatch in Static memory mentioned \
                           and found at Memory bank %d\n",bank);
	    }
	    boot_check++;
	    return memory_bank_info.mem_bank1;
	    break;

	case BANK1:
	    if(value != memory_bank_info.mem_bank2
                    && boot_check <4 && i!=3)
	    {
	        printk("\n Mismatch in Static memory mentioned \
                            and found at Memory bank %d\n",bank);
	    }
	    boot_check++;
	    return memory_bank_info.mem_bank2;
  	    break;
	
        case BANK2:
	    if(value != memory_bank_info.mem_bank3 && boot_check <4 && i!=3)
	    {
	    printk("\n Mismatch in Static memory mentioned \
                        and found at Memory bank %d\n",bank);
	    }
	    boot_check++;
	    return memory_bank_info.mem_bank3;
	    break;

	case BANK3:
	    if(value != memory_bank_info.mem_bank4 && boot_check <4 && i!=3)
	    {
	        printk("\n Mismatch in Static memory mentioned \
                            and found at Memory bank %d\n",bank);
  	    }
	    boot_check++;
	    return memory_bank_info.mem_bank4;
	    break;
	default: 
	    printk("\n Unknown bank %d\n",bank);
	    return 0;
	    break;
    }
	
#endif
    if(i==3)
    {
        if(boot_check<=3)
	{
	printk("\n Memory with unsupported RDID or \
               unknown memory found at bank %d\n",bank);
	printk("\n Static Memory Banks recomanded here \n");  
	boot_check++;
	}
	return 0;
    }
	
    return(value);
}


/****************************************************************************/
/* NAME: smi_read_sr(unsigned int bank)					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*             This routine will get the status register of the flash       */
/*             chip present at the given bank				    */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - bank : bank ID on whcih we have to find                           */
/*                                                                          */
/* OUT : 								    */
/*      None							            */
/*                                                                          */
/* RETURN:                                                                  */
/*        int : startus register			                    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

unsigned int 
smi_read_sr (unsigned int bank)
{
    unsigned int B_SEL=0;
    unsigned int value;
    //unsigned int SAVE_CR1;
    //unsigned int SAVE_CR2;

    switch(bank)
    {
	case BANK0: B_SEL = BANK0_SEL; break;
	case BANK1: B_SEL = BANK1_SEL; break;
	case BANK2: B_SEL = BANK2_SEL; break;
	case BANK3: B_SEL = BANK3_SEL; break;
    }

    /*** Put SMI in HW mode ***/
    SMICntl->SMI_CR1 &= ~(SW_MODE);
    /*** Clear TFF ***/
    SMICntl->SMI_SR &= ~TFF;
    /*** Performing a RSR instruction in HW mode ***/
    SMICntl->SMI_CR2 = B_SEL | RD_STATUS_REG;
    while (!(SMICntl->SMI_SR & TFF));
    /*** Returning STATUS Register value ***/
    value=SMICntl->SMI_SR;
    /*** Clear SR TFF ***/
    SMICntl->SMI_SR &= ~TFF;

    /*** Reloading CR1-2 ***/
    return(value);
}


/****************************************************************************/
/* NAME: stwsf_get_chip_index(unsigned int chip_identity)                   */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*             This routine will get the status register of the flash       */
/*             chip present at the given bank				    */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - bank : bank ID on whcih we have to find                           */
/*                                                                          */
/* OUT : 								    */
/*      None							            */
/*                                                                          */
/* RETURN:                                                                  */
/*        int : manufacturer ID of the flash chip                           */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

static int 
stwsf_get_chip_index (unsigned int chip_identity)
{
    int index;
    for (index = 0; sflash_ids[index].name != NULL; index++) 
    {
	if(sflash_ids[index].device_id==chip_identity)
	{
	    return index;
	}

    }

    /* Memory chip is not listed and not supported */
    return -1;
}


/************************************************************************

 Linux module initialization routine, The intention is to fill 

*************************************************************************/
static struct mtd_info *merged_mtd;


/****************************************************************************/
/* NAME: sflash_init()					                    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*             This routine initializes the MTD subsystem.		    */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      None					                            */
/*                                                                          */
/* OUT : 								    */
/*      None							            */
/*                                                                          */
/* RETURN:                                                                  */
/*        int : 				                            */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int __init 
sflash_init ( void )
{
    struct sflash_chip  *this[MAX_NUM_FLASH_CHIP];
    unsigned int i;
    printk(KERN_INFO "STW Serial Flash initializing...\n");
    /* allocate memory for MTD device struct and private data */
    for(i=0;i<MAX_NUM_FLASH_CHIP;i++)
    {
	stwsf_mtd[i] = kmalloc( sizeof(struct mtd_info), GFP_KERNEL);
	if (!stwsf_mtd[i])
        {
            printk("Unable to allocate memory for \
                    MTD Serial Flash Device %d.\n", i);
	    return -ENOMEM;
  	}
    /* get pointer to chip private data */
    this[i] = kmalloc( sizeof(struct sflash_chip), GFP_KERNEL);

    /* zeroing the structs */
    memset((char *)stwsf_mtd[i], 0, sizeof(struct mtd_info));
    memset((char *)this[i], 0, sizeof(struct sflash_chip));
    stwsf_mtd[i]->priv = this[i];
    }
    
    for( i=0;i<MAX_NUM_FLASH_CHIP;i++)
    {	
        if(stwsf_chip_probe(stwsf_mtd[i], i)<0)
	{
	    printk("\n Memory slot %d is empty\n",i);
	}
  	 
    }

    smi_init(devices_found); 
    
    for(i=0;i<MAX_NUM_FLASH_CHIP;i++)
    {
        /* allocate memory for internal data buffer */
	this[i]->data_buf = kmalloc (sizeof(u_char) *
			    (stwsf_mtd[i]->oobsize),
			    GFP_KERNEL);
	if (!this[i]->data_buf) 
        {
	     printk ("Unable to allocate serial flash data buffer.\n");
             kfree (stwsf_mtd[i]);
	     return -ENOMEM;
	}
    }
  
   /* Register the partitions */
  
    
#ifndef CONFIG_MTD_STATIC_PARTITIONING 
 	stwsf_par_info[1][0]=stwsf_par_info_II[0];
 	stwsf_par_info[2][0]=stwsf_par_info_III[0]; 
	stwsf_par_info[3][0]=stwsf_par_info_IV[0]; 
	
 	stwsf_par_info_primary[3].size = 
			sflash_ids[memory_indices[0]].size_in_bytes 
                        - 0x06F0000;
	
	stwsf_par_info_merge[0].size=stwsf_par_info_primary[3].size;
	
	add_mtd_partitions((stwsf_mtd[0]),
		     (struct mtd_partition*)stwsf_par_info_primary,
		     (int)NUM_SF_PARTITIONS_I);

   
   for(i=1;i<devices_found;i++)
   {
       stwsf_par_info[i][0].size = sflash_ids[memory_indices[i]].size_in_bytes;
       stwsf_par_info_merge[0].size += stwsf_par_info[i][0].size;  
   }
   
   for(i=1;i<devices_found;i++)
   {
       add_mtd_partitions(stwsf_mtd[i],
       (struct mtd_partition*)stwsf_par_info[i],
       (int)NUM_SF_PARTITIONS_II);
   }
     	
#endif

#ifdef CONFIG_MTD_STATIC_PARTITIONING

    printk("\n The partitioning is done as mentioned (static) \n");
    add_mtd_partitions((stwsf_mtd[0]),
		     (struct mtd_partition*)stwsf_par_info_primary,
 		     (int)NUM_SF_PARTITIONS_I);

    for(i=1;i<devices_found;i++)
    {
        if(i==1)
        add_mtd_partitions(stwsf_mtd[i],
        (struct mtd_partition*)stwsf_par_info_II,
        (int)NUM_SF_PARTITIONS_II);
        if(i==2)
        add_mtd_partitions(stwsf_mtd[i],
	     (struct mtd_partition*)stwsf_par_info_III,
	     (int)NUM_SF_PARTITIONS_III);
        if(i==3)
	add_mtd_partitions(stwsf_mtd[i],
	     (struct mtd_partition*)stwsf_par_info_IV,
	     (int)NUM_SF_PARTITIONS_IV);

    }
#endif 		                  

#if 0

    merged_mtd = mtd_concat_create(stwsf_mtd,devices_found , \
				"Flash Banks #0 #1 #2 and 3 ");
    if(merged_mtd)
    {
	add_mtd_partitions(merged_mtd,
               (struct mtd_partition*)stwsf_par_info_merge,
               (int)NUM_SF_PARTITIONS_MERGE);
    }

#endif 

    printk(KERN_INFO "ATI STW Serial Flash Device initialization done.\n");
    return 0;
}


/************************************************************************

 Linux module clean-up/exit routine

************************************************************************/


#ifdef MODULE
static void __exit 
sflash_cleanup ( void )
{

  uint32_t r;
  struct sflash_chip *this = (struct sflash_chip *) &stwsf_mtd[1];


  /* Unregister the device */
  del_mtd_device (stwsf_mtd);
  del_mtd_partitions(stwsf_mtd);

  /* Free internal data buffer */
  kfree (this->data_buf);

  /* Free the MTD device structure */
  kfree (stwsf_mtd);

  printk(KERN_INFO "Exit...STW Serial Flash driver module clean-up done.\n");

}
module_exit(sflash_cleanup);
#endif

module_init(sflash_init);
EXPORT_SYMBOL(stwsf_chip_erase);
MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("arif.khan@st.com");
MODULE_DESCRIPTION ("ST Serial Flash Device Interface");
/********************** End of file ****************************************/

/* End of file - sp_sflash.c */
