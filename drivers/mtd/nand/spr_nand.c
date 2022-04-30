/********************************************************************************/
/*   Copyright (C) STMicroelectronics 2008 by Ashish Priyadarshi  				*/
/*   ashish.priyadarshi@st.com													*/
/*																				*/
/*   This program is free software; you can redistribute it and/or modify		*/
/*   it under the terms of the GNU General Public License as published by		*/
/*   the Free Software Foundation; either version 2 of the License, or			*/
/*   (at your option) any later version.										*/
/*																				*/
/*   This program is distributed in the hope that it will be useful,			*/
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of 			*/
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				*/
/*   GNU General Public License for more details.								*/
/*																				*/
/*   You should have received a copy of the GNU General Public License			*/
/*   along with this program; if not, write to the								*/
/*   Free Software Foundation, Inc.,											*/
/*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.					*/
/********************************************************************************/
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/types.h>
#include <asm/arch/debug.h>
#include "spr_fsmc_st_hal.h"

/*
 * Function Declarations
 */
int SPEAr_NAND_bbt (struct mtd_info *mtd);
static int SPEAr_NAND_probe(void);
static void SPEAr_NAND_error_fix  (t_uint8 * data, t_uint32 row, t_uint32 column);
static int SPEAr_NAND_block_markbad(struct mtd_info *mtd, loff_t ofs);

/*
 * MTD structure for SPEAr  board
 */
static struct mtd_info *spearnand_mtd = NULL;

/*
 * ECC layout for OOB of NAND
 */
static struct nand_ecclayout spear_nand_oobinfo = {
	.eccbytes = 3, 
	.eccpos = {	2, 3, 4 },
	.oobfree = {{8, 8}}
};

unsigned long cmemd_va, cmema_va,cmemc_va;

uint8_t scan_ff_pattern[] = { 0xff,0xff };

struct nand_bbt_descr spear_smallpage_memorybased = {
	.options = NAND_BBT_2BIT | NAND_BBT_SEARCH | NAND_BBT_CREATE /*| NAND_BBT_SCANALLPAGES*/,
	.pages[0]=0, .pages[1]=0, .pages[2]=0, .pages[3]=0, .pages[4]=0,  .pages[5]=0,  .pages[6]=0, .pages[7]=0 ,
	.offs =5 ,
	.veroffs=0,
	.version[0]=0,.version[1]=0,.version[2]=0,.version[3]=0,.version[4]=0,.version[5]=0,.version[6]=0,.version[7]=0,
	.len = 1,
	.maxblocks=8,
	.reserved_block_code=0,
	.pattern = scan_ff_pattern

};

/*
struct nand_bbt_descr spear_smallpage_memorybased = {
	.options = 0,
	.pages[0]=0, .pages[1]=0, .pages[2]=0, .pages[3]=0, .pages[4]=0,  .pages[5]=0,  .pages[6]=0, .pages[7]=0 ,
	.offs =5 ,
	.veroffs=0,
	.version[0]=0,.version[1]=0,.version[2]=0,.version[3]=0,.version[4]=0,.version[5]=0,.version[6]=0,.version[7]=0,
	.len = 1,
	.maxblocks=0,
	.reserved_block_code=0,
	.pattern = scan_ff_pattern

};
*/
struct nand_bbt_descr spear_largepage_memorybased = {
	.options = 0,
	.pages[0]=0, .pages[1]=0, .pages[2]=0, .pages[3]=0, .pages[4]=0,  .pages[5]=0,  .pages[6]=0, .pages[7]=0 ,
	.offs = 0,
	.veroffs=0,
	.version[0]=0,.version[1]=0,.version[2]=0,.version[3]=0,.version[4]=0,.version[5]=0,.version[6]=0,.version[7]=0,
	.len = 1,
	.maxblocks=0,
	.reserved_block_code=0,
	.pattern = scan_ff_pattern
};

#define NUM_PARTITIONS  16

//65536 * 0x00004000
static const struct mtd_partition partition_info1024M[] = {
	{ .name = "Xloader",
		.offset =  0,
		.size =   0x00010000 },  // 4 * 0x00004000
	{ .name = "Uboot",
		.offset = 0x00010000,
		.size =   0x00050000 },  //20 * 0x00004000
	{ .name = "Kernel",
		.offset = 0x00060000,
		.size =   0x00400000},  //256 * 0x00004000
	{ .name = "Root File system",
		.offset = 0x00460000,
		.size =   0x3FBA0000},  //65256 * 0x00004000
};

//32768 * 0x00004000
static const struct mtd_partition partition_info512M[] = {
	{ .name = "Xloader",
		.offset =  0,
		.size =   0x00010000 },  // 4 * 0x00004000
	{ .name = "Uboot",
		.offset = 0x00010000,
		.size =   0x00050000 },  //20 * 0x00004000
	{ .name = "Kernel",
		.offset = 0x00060000,
		.size =   0x00400000},  //256 * 0x00004000
	{ .name = "Root File system",
		.offset = 0x00460000,
		.size =   0x1FBA0000},  //32488 * 0x00004000
};

//16384 * 0x00004000
static const struct mtd_partition partition_info256M[] = {
	{ .name = "Xloader",
		.offset =  0,
		.size =   0x00010000 },  // 4 * 0x00004000
	{ .name = "Uboot",
		.offset = 0x00010000,
		.size =   0x00050000 },  //20 * 0x00004000
	{ .name = "Kernel",
		.offset = 0x00060000,
		.size =   0x00400000},  //256 * 0x00004000
	{ .name = "Root File system",
		.offset = 0x00460000,
		.size =   0x0FBA0000},  //16104 * 0x00004000
};

//8192 * 0x00004000
static const struct mtd_partition partition_info128M[] = {
	{ .name = "Xloader",
		.offset =  0,
		.size =   0x00010000 },  //  4 * 0x00004000
	{ .name = "Uboot",
		.offset = 0x00010000,
		.size =   0x00050000 },  // 20 * 0x00004000
	{ .name = "Kernel",
		.offset = 0x00060000,
		.size =   0x00400000},  // 256 * 0x00004000
	{ .name = "Root File system",
		.offset = 0x00460000,
		.size =   0x07BA0000},  //7912 * 0x00004000
};

//4096 * 0x00004000
static const struct mtd_partition partition_info64M[] = {
	{ 	.name   = "xloader",
		.offset = 0x00000000,// Block 0x00  =    0
		.size   = 0x00010000 // 64Kb
	},
	{	.name   = "uboot",
		.offset = 0x00010000,// Block 0x04  =    4
		.size   = 0x0004C000 //304Kb 
	},
        // Note: Block 23 is hard coded for the U-Boot environment!
	{	.name   = "ubootenv",
		.offset = 0x0005C000,// Block 0x17  =   23
		.size   = 0x00004000 // 16Kb
	},
 	{
 		.name	= "data",
 		.offset	= 0x00060000,// Block 0x18  =   24
 		.size	= 0x00020000 //128Kb
 	},
 	{
 		.name	= "kernel",
 		.offset	= 0x00080000,// Block 0x20  =   32
 		.size	= 0x00200000 //  2Mb
 	},
 	{
 		.name	= "rootfs",
 		.offset	= 0x00280000,// Block 0xA0  =  160
 		.size	= 0x03700000 // 55Mb
 	},
	{
		.name   = "kernel2",
 		.offset	= 0x03980000,// Block 0xE60 = 3680
 		.size	= 0x00180000 //  1.5Mb
	},
	{
		.name   = "rootfs2",
		.offset = 0x03B00000,// Block 0xEC0 = 3776
		.size   = 0x00500000 //  5Mb
	},
        //                             =======
        //                              64.0Mb

        // Convenience partitions
	{	.name   = "uboot and ubootenv",
		.offset = 0x00010000,// Block 0x04 =  4
		.size   = 0x00050000 //320Kb
	},
	{
		.name   = "rootfs clone",
 		.offset	= 0x00280000,
 		.size	= 0x03700000 // 55Mb
	},
	{
		.name   = "all without data",
		.offset = 0x00080000,
		.size   = 0x03F80000 // 63.5Mb
	},
	{
		.name   = "whole flash",
		.offset = 0x00000000,
		.size   = 0x04000000 // 64Mb
	},
	{
		.name   = "kernel and rootfs",
 		.offset	= 0x00080000,
		.size   = 0x03900000 // 57Mb
	},
	{
		.name   = "kernel2 and rootfs2",
 		.offset	= 0x03980000,
		.size   = 0x00680000 //  6.5Mb
	},

        // Android partitions (replacing rootfs)
	{
		.name   = "system",
 		.offset	= 0x00280000,
 		.size	= 0x02300000 // 35Mb
	},
	{
		.name   = "userdata",
 		.offset	= 0x02580000,
		.size   = 0x01400000 // 20Mb
	},
}; 

//2048 * 0x00004000
static const struct mtd_partition partition_info32M[] = {
	{ .name = "Xloader",
		.offset =  0,
		.size   =  0x00010000 },   //4 * 0x00004000 
	{ .name = "Uboot",
		.offset =  0x00010000,
		.size   =  0x00050000 }, //20 * 0x00004000
	{ .name = "Kernel",
		.offset =  0x00060000,
		.size   =  0x00400000 }, //256 * 0x00004000
	{ .name = "Root File system",
		.offset = 0x00460000,
		.size =   0x01BA0000},   //1768 * 0x00004000
};


#ifdef SINGLE_PARTITION

#define NUM_PARTITIONS 1
static const struct mtd_partition partition_info1024M[] = {
	{ .name = "X-Loader",
		.offset =  0,
		.size =   8192 * 0x00020000 },
};

static const struct mtd_partition partition_info512M[] = {
	{ .name = "X-Loader",
		.offset =  0,
		.size =   4096 * 0x00020000 },
};

static const struct mtd_partition partition_info256M[] = {
	{ .name = "X-Loader",
		.offset =  0,
		.size =   2048 * 0x00020000 },
};

static const struct mtd_partition partition_info128M[] = {
	{ .name = "X-Loader",
		.offset =  0,
		.size =   1024 * 0x00020000 },
};

static const struct mtd_partition partition_info64M[] = {
	{ .name = "X-Loader",
		.offset =  0,
		.size =   512 * 0x00020000 },
};

static const struct mtd_partition partition_info32M[] = {
	{ .name = "X-Loader",
		.offset =  0,
		.size =   256 * 0x00020000 },
};
#endif


#ifdef CONFIG_ARCH_SPEARBASIC
volatile unsigned int FSMC_BASE         = 0;
volatile unsigned int NAND_B0_CMEM_DATA = 0;
volatile unsigned int NAND_B0_CMEM_CMD  = 0;
volatile unsigned int NAND_B0_CMEM_ADDR = 0;
/****************************************************************************/
/* NAME:  EnablePhotoFrameMode_fsmc                                         */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Initializes the FSMC of the RAS. In SPEArBasic we have */
/*      FSMC in RAS. So first RAS need to be enabled and then Base address  */
/*      of RAS need to be mapped.                                           */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : None                                                               */
/* OUT : None                                                               */
/*				                                    					    */
/* RETURN: None                             							    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
void  EnablePhotoFrameMode_fsmc(void)
{
	///////////////////////////////////////////////////////////////////////
	//SYNOPSIS                                                           //
	//ioremap - map bus memory into CPU space                            //
	//    void * ioremap (unsigned long offset, unsigned long size);     //
	//Arguments:                                                         //
	//  offset -  bus address of the memory                              //
	//  size - size of the resource to map                               //
	///////////////////////////////////////////////////////////////////////
	//Map Base address of the FSMC  
	FSMC_BASE = (unsigned int)ioremap(VA_SPEARBASIC_START_NAND_FLASH_CTRLR, 4); 	

	//Map register addresses of the NAND
	NAND_B0_CMEM_DATA = (unsigned int)ioremap(VA_SPEARBASIC_START_NAND_FLASH_MEM,4);   	
	NAND_B0_CMEM_CMD  = (unsigned int)ioremap(VA_SPEARBASIC_START_NAND_FLASH_CMD,4);   	
	NAND_B0_CMEM_ADDR = (unsigned int)ioremap(VA_SPEARBASIC_START_NAND_FLASH_ADDR,4);   	

	printk("spearbasic RAS fsmc enable\n");
}

/****************************************************************************/
/* NAME:  DisablePhotoFrameMode_fsmc                                        */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      In SPEArBasic we have FSMC in RAS. So first Base address of fsmc of */
/*        RAS need to be unmapped and then registers of RAS need to be      */
/*        unmapped.                                                         */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : None                                                               */
/* OUT : None                                                               */
/*									                                        */
/* RETURN: None     							                            */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
void  DisablePhotoFrameMode_fsmc(void)
{
	///////////////////////////////////////////////////////////////////////
	//SYNOPSIS                                                           //
	//iounmap - unmap CPU space of bus memory                            //
	//    void * iounmap (unsigned long offset);                         //
	//Arguments:                                                         //
	//  offset -  bus address of the memory                              //
	///////////////////////////////////////////////////////////////////////

	//UnMap register addresses of the NAND
	iounmap((void*)VA_SPEARBASIC_START_NAND_FLASH_ADDR);   	
	iounmap((void*)VA_SPEARBASIC_START_NAND_FLASH_CMD);   	
	iounmap((void*)VA_SPEARBASIC_START_NAND_FLASH_MEM);   	

	//UnMap Base address of the FSMC  
	iounmap((void*)VA_SPEARBASIC_START_NAND_FLASH_CTRLR); 	

	printk("spearbasic RAS fsmc disable\n");
}
#endif


/****************************************************************************/
/* NAME: 		SPEAr_NAND_hwcontrol										*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/* This routine allows hardware specific access to control-lines(ALE,CLE)   */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :    mtd: mtd information structure                                  */
/*      -   cmd: setting ALE, CLE 					    					*/
/* OUT : 		                                        	    			*/
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void SPEAr_NAND_hwcontrol(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	struct nand_chip *this = mtd->priv;

	if (ctrl & NAND_CTRL_CHANGE) 
	{
		if(ctrl & NAND_CLE) 
		{
			this->IO_ADDR_R = (void __iomem *)cmemc_va;
			this->IO_ADDR_W = (void __iomem *)cmemc_va;    
		}
		else if(ctrl & NAND_ALE)
		{
			this->IO_ADDR_R = (void __iomem *)cmema_va;
			this->IO_ADDR_W = (void __iomem *)cmema_va;    
		}
		else
		{
			this->IO_ADDR_R = (void __iomem *)cmemd_va;
			this->IO_ADDR_W = (void __iomem *)cmemd_va;      
		}
	}

	if (cmd != NAND_CMD_NONE)
		writeb(cmd, this->IO_ADDR_W);
}

/****************************************************************************/
/* NAME: 		SPEAr_NAND_device_setup	                            		*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION: This routine is called by SPEAr_NAND_probe. It configures   */
/* the FSMC,configures the nand control timings(Attribute & Common Memory 	*/
/*   space) wprot of nor                                                    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :    								    							*/
/* OUT : 								    								*/
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
void SPEAr_NAND_device_setup(void)
{
	/*
	* FSMC initialization
	*/
	t_fsmc_bank_index			  bank=FSMC_BANK0;
	t_fsmc_nor_chip_index		  bank_nor=FSMC_NOR_CHIP0;
	t_fsmc_pc_nand_mem_space	  mem_space;
	t_struct_fsmc_pc_nand_tmng	  nand_timing;
	t_struct_fsmc_pc_nand_control nand_ctrl;
	t_write_protected			  fsmc_status;
	t_fsmc_error 				  error;

	/**Initializing Gen Mem Ctrl for NOR */
	fsmc_status=FSMC_WRITE_PROTECTED;//To set as 1

	error=FSMC_WriteProtectNor(bank_nor,fsmc_status);
	if(error!=FSMC_OK)
		printk("\nFSMC_ERROR# %d",error);

	/**Initializing Mem Ctrl for NAND */
	nand_ctrl.wait_on    = FSMC_WAIT_ON;
	nand_ctrl.enable_bank= FSMC_BANK_ENABLE;
	nand_ctrl.mem_type   = FSMC_NAND_MEM;
	nand_ctrl.dev_width  = FSMC_ACCESS_8;
	nand_ctrl.tar		 = 0x04;
	nand_ctrl.tclr		 = 0x04;
	error=FSMC_SetNandControl(bank,&nand_ctrl);
	if(error!=FSMC_OK)
		printk("\nFSMC_ERROR# %d",error);

	/**common memory space */
	nand_timing.data_bus_hiz_phase = 0x1;
	nand_timing.addr_hold_phase    = 0x4;
	nand_timing.wait_phase  	   = 0x6;
	nand_timing.addr_setup_phase   = 0x0;

	mem_space = FSMC_PC_NAND_COMMON_MEM_SPACE;
	error=FSMC_SetNandTiming(bank,mem_space,&nand_timing);
	if(error!=FSMC_OK)
		printk("\nFSMC_ERROR# %d",error);

	/*attr memory space */
	mem_space = FSMC_PC_NAND_ATTR_MEM_SPACE;
	error=FSMC_SetNandTiming(bank,mem_space,&nand_timing);
	if(error!=FSMC_OK)
		printk("\nFSMC_ERROR# %d",error);
}


/****************************************************************************/
/* NAME: 		SPEAr_NAND_correct_data				    					*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION: Called by upper layer for correcting 1 bit error	    	*/
/*  when the ECC stored is not same as ECC calculated by FSMC  		    	*/
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN      mtd: mtd information structure                                   */
/*         page: 512 byte page of data                                      */
/*		   ecc : ecc from chip(NAND spare area)                     		*/
/*		   ecc_error:ecc calculated from Raw data(FSMC)		    			*/
/* OUT : 			                                            			*/
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int SPEAr_NAND_correct_data(struct mtd_info *mtd, u_char *page, u_char *ecc, u_char *ecc_error)
{
	t_uint32 row, column;
	t_uint8 cnt_ecc, tmp_ecc, ecc_xor[4];
	t_uint32 i, ii;


	for (cnt_ecc = 0, i = 0; i < ECC_BYTES; i++)
	{
		tmp_ecc = (t_uint8) (ecc[i] ^ ecc_error[i]);
		ecc_xor[i] = tmp_ecc;

		for (ii = 1; ii <= 128; ii = ii * 2)
		{
			if ((tmp_ecc & ii) != 0)
				cnt_ecc++;
		}
	}

	if (cnt_ecc == 0 || cnt_ecc == 1)
		return (BOOT_OK);

	if (cnt_ecc != ECC_CORRECTABLE)
	{
		return 1;
	}

	//----------------------------------------------------------------------------
	//byte3 P4_0,    P4_1,    P2_0,    P2_1,    P1_0,    P1_1,    0,    0
	//----------------------------------------------------------------------------
	//row= bits  P4_0, P2_0, P1_0

	column = 0;
	if ((ecc_xor[2] & 0x80) != 0)
		column = column | 0x4;

	if ((ecc_xor[2] & 0x20) != 0)
		column = column | 0x2;

	if ((ecc_xor[2] & 0x8) != 0)
		column = column | 0x1;

	//----------------------------------------------------------------------------
	//byte 0   P64_0,   P64_1,  P32_0,   P32_1,   P16_0,   P16_1,    P8_0,    P8_1
	//----------------------------------------------------------------------------
	//byte 1 P1024_0, P1024_1, P512_0,  P512_1,  P256_0,  P256_1,  P128_0,  P128_1
	//----------------------------------------------------------------------------
	//byte 2    P4_0,    P4_1,   P2_0,    P2_1,    P1_0,    P1_1, P2048_0, P2048_1
	//----------------------------------------------------------------------------


	//column= bits P8192_0, P4096_0, P2048_0, P1024_0, P512_0, P256_0, P128_0, P64_0, P32_0, P16_0, P8_0


	row = 0;
	if ((ecc_xor[0] & 0x2) != 0)
		row = row | 0x1;

	if ((ecc_xor[0] & 0x8) != 0)
		row = row | 0x2;

	if ((ecc_xor[0] & 0x20) != 0)
		row = row | 0x4;

	if ((ecc_xor[0] & 0x80) != 0)
		row = row | 0x8;

	if ((ecc_xor[1] & 0x2) != 0)
		row = row | 0x10;

	if ((ecc_xor[1] & 0x8) != 0)
		row = row | 0x20;

	if ((ecc_xor[1] & 0x20) != 0)
		row = row | 0x40;

	if ((ecc_xor[1] & 0x80) != 0)
		row = row | 0x80;

	if ((ecc_xor[2] & 0x2) != 0)
		row = row | 0x100;

	SPEAr_NAND_error_fix  (page, row, column);
	return 0;
}

/****************************************************************************/
/* NAME: 		SPEAr_NAND_error_fix				    					*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION: Called by SPEAr_NAND_correct_data 		            		*/
/*				for fixing the error in a page              				*/
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN                                      				    				*/
/*         data: 512 byte page of data                                      */
/*		   row : row of error bit 				    						*/
/*		   column:column of error bit 				    					*/
/* OUT : 			                                            			*/
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void SPEAr_NAND_error_fix (t_uint8 * data, t_uint32 row, t_uint32 column)
{
	if ((data[row] & 1 << column) == 0)
	{
		data[row] = (t_uint8) (data[row] | 1 << column);
	}
	else
	{
		data[row] = (t_uint8) (data[row] & ~(1 << column));
	}
}

/****************************************************************************/
/* NAME: 		SPEAr_NAND_calculate_ecc			     					*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION: Read ECC calculated by FSMC				     				*/
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN                                      				     				*/
/*        mtd: mtd_info structure                                           */
/*		   dat : pointer to page of data 			     					*/
/*		   ecc_code:ecc value read/calculated			     				*/
/* OUT : 			                                             			*/
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/*****************************************************************************/
static int SPEAr_NAND_calculate_ecc(struct mtd_info *mtd, const u_char *dat, u_char *ecc_code)
{
	t_uint32 ecc;

	t_fsmc_bank_index bank=FSMC_BANK0;
	ecc=FSMC_ReadEcc(bank);

	ecc_code[0] = (t_uint8) (ecc & 0xFF);
	ecc_code[1] = (t_uint8) ((ecc & 0xFF00) >> 8);
	ecc_code[2] = (t_uint8) ((ecc & 0xFF0000) >> 16);
	return 0;
}

/****************************************************************************/
/* NAME: 		SPEAr_NAND_enable_hwecc				    					*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION: Read ECC calculated by FSMC				    				*/
/*                                                                         	*/
/* PARAMETERS:                                                             	*/
/* IN                                      				    				*/
/*        mtd: mtd_info structure                                          	*/
/*		   mode: Constants for h/w ECC 				    					*/
/* OUT :								    								*/
/* RETURN:                                                                 	*/
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                          	*/
/****************************************************************************/
static void SPEAr_NAND_enable_hwecc(struct mtd_info *mtd, int mode)
{
	t_fsmc_error 	   err  = FSMC_OK;
	t_fsmc_ecc 		   state= FSMC_ECC_ENABLE;
	t_fsmc_bank_index  bank = FSMC_BANK0;
	t_ecc_pagelen 	   page_len= FSMC_ECC_PAGELEN_512;

	err = FSMC_NandEccLogic(bank, state, page_len);
	if(err != FSMC_OK)
		printk ("Error in FSMC Nand ECC:%d\n", err);

}


/****************************************************************************/
/* NAME: 		SPEAr_NAND_probe				    						*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION: Called  from init. It initializes nand_chip structure with  */
/* the func pointers Allocated mtd_info,nand_chipnnand_oobinfo structures.  */
/* initializes the FSMC Registers, MTD paritions, IO_ADDR_R,IO_ADDR_W       */
/* with the virtual address of data	                                    	*/
/* PARAMETERS:                                                              */
/* IN                                      				    				*/
/* OUT :								    								*/
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int SPEAr_NAND_probe(void)
{
	struct nand_chip *this;
	int err = 0;

	/*
	* Allocate memory for MTD device structure and private data
	*/
	spearnand_mtd = kmalloc (sizeof(struct mtd_info) +sizeof (struct nand_chip), GFP_KERNEL);

	if (!spearnand_mtd) {
		printk ("Unable to allocate SPEAr NAND MTD device structure.\n");
		err = -ENOMEM;
		goto out;
	}

	printk("\nInside nand probe function\n");

	/*
	*  Initialize FSMC for NAND access
	*/
	SPEAr_NAND_device_setup();

	/*
	* ioremap:
	*	cmemd_va: cmem data space
	* 	cmemc_va: cmem cmd space
	* 	cmema_va: cmem addr space
	*/
	cmemd_va = NAND_B0_CMEM_DATA;
	cmemc_va = NAND_B0_CMEM_CMD;
	cmema_va = NAND_B0_CMEM_ADDR;

	/* Get pointer to private data */
	this = (struct nand_chip *) (&spearnand_mtd[1]);

	memset((char *) spearnand_mtd, 0, sizeof(struct mtd_info));
	memset((char *) this, 0, sizeof(struct nand_chip));

	/* Link the private data with the MTD structure */
	spearnand_mtd->priv = this;

	/*
	* Set the required values in nand_chip before calling nand_scan
	* for others the default functionality/default functions
	* in the nand_base generic drivers are sufficient
	*/
	this->IO_ADDR_R = (void __iomem *)NAND_B0_CMEM_DATA;
	this->IO_ADDR_W = (void __iomem *)NAND_B0_CMEM_DATA;

	this->cmd_ctrl    = SPEAr_NAND_hwcontrol;
	this->scan_bbt    = SPEAr_NAND_bbt;
	this->chip_delay  = 30;
	//this->ecc.mode      = NAND_ECC_SOFT; 
	this->ecc.mode      = NAND_ECC_HW;  //Hw necessary for JFFS2
	this->ecc.calculate = SPEAr_NAND_calculate_ecc;
	this->ecc.correct   = SPEAr_NAND_correct_data;
	this->ecc.hwctl     = SPEAr_NAND_enable_hwecc;
	this->ecc.size      = 512;
	this->ecc.bytes      = 3;
	this->ecc.layout      = &spear_nand_oobinfo;//Adding OOB info
	this->options = NAND_USE_FLASH_BBT;
	this->block_markbad = SPEAr_NAND_block_markbad;

	/*
	* Scan to find existance of the device
	*/
	if (nand_scan(spearnand_mtd, 1)) {
		err = -ENXIO;
		printk("NO SPEAR NAND Device found!\n");
		goto out_ior;
	}

	/*
	* Register the partitions
	*/
	switch (spearnand_mtd->size) {
/*	case 0x2000000: 		//32MiB
		add_mtd_partitions(spearnand_mtd, partition_info32M,
				NUM_PARTITIONS);
		break;*/
	case 0x4000000:			//64MiB
		add_mtd_partitions(spearnand_mtd, partition_info64M,
				NUM_PARTITIONS);

		break;
/*		
	case 0x8000000:			//128MiB==1Gb
		add_mtd_partitions(spearnand_mtd, partition_info128M,
				NUM_PARTITIONS);
		break;
	case 0x10000000:			//256MiB ==2Gb
		add_mtd_partitions(spearnand_mtd, partition_info256M,
				NUM_PARTITIONS);
		break;
	case 0x20000000:			//512MiB ==4Gb
		add_mtd_partitions(spearnand_mtd, partition_info512M,
				NUM_PARTITIONS);
		break;
	case 0x40000000:			//1024MiB==8Gb
		add_mtd_partitions(spearnand_mtd, partition_info1024M,
				NUM_PARTITIONS);
		break;
*/
	default:
		err = -ENXIO;
		printk("UNSUPPORTED NAND SIZE\n");
		goto out_us;
	}

	goto out;

out_us:
	nand_release(spearnand_mtd);
	kfree (spearnand_mtd);

out_ior:
out:
	return err;
}

/****************************************************************************/
/* NAME: 		SPEAr_NAND_block_markbad			    					*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION: 				    										*/
/*                                                                         	*/
/* PARAMETERS:                                                             	*/
/* IN                                      				    				*/
/* OUT :								    								*/
/* RETURN:                                                                 	*/
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                          	*/
/****************************************************************************/
static int SPEAr_NAND_block_markbad(struct mtd_info *mtd, loff_t ofs)
{
	struct nand_chip *this = mtd->priv;
	u_char buf[2] = {0, 0};
	int block;
	int column, page, status, ret = -EIO, chipnr;

	/* Get block number */
	block = ((int) ofs) >> this->bbt_erase_shift;
	this->bbt[block >> 2] |= 0x01 << ((block & 0x03) << 1);

	/* Do we have a flash based bad block table ? */
	if (this->options & NAND_USE_FLASH_BBT)
		return nand_update_bbt (mtd, ofs);

	/* We write two bytes, so we dont have to mess with 16 bit access */
	ofs += mtd->oobsize + this->badblockpos;

	/* Shift to get page */
	page = (int) (ofs >> this->page_shift);
	chipnr = (int) (ofs >> this->chip_shift);

	/* Mask to get column */
	column = ofs & (mtd->oobsize - 1);

	/* Do not allow write past end of page */
	if ((column + 2) > mtd->oobsize) {
		return -EINVAL;
	}

	/* Select the NAND device */
	this->select_chip(mtd, chipnr);

	/* Reset the chip */
	this->cmdfunc(mtd, NAND_CMD_RESET, -1, -1);

	this->cmdfunc (mtd, NAND_CMD_STATUS, -1, -1);
	if ((this->read_byte(mtd) & 0x80) ? 0 : 1) {
		printk("oobwrite failed: write protected\n");
		goto out;
	}

	/* Invalidate the page cache, if we write to the cached page */
	if (page == this->pagebuf)
		this->pagebuf = -1;

	/* Write out desired data */
	//	this->cmdfunc (mtd, NAND_CMD_SEQIN, mtd->oobblock + column, page & this->pagemask); //??? debug

	/* write data */
	this->write_buf(mtd, buf, 2);

	/* Send command to program the OOB data */
	this->cmdfunc (mtd, NAND_CMD_PAGEPROG, -1, -1);

	status = this->waitfunc (mtd, this);
	//status = this->waitfunc (mtd, this, FL_WRITING);

	/* See if device thinks it succeeded */
	if (status & 0x01) {
		printk("nand_write_oob: " "Failed write, page 0x%08x\n", page);
		ret = -EIO;
		goto out;
	}

#ifdef CONFIG_MTD_NAND_VERIFY_WRITE
	/* Send command to read back the data */
	this->cmdfunc (mtd, NAND_CMD_READOOB, column, page & this->pagemask);

	if (this->verify_buf(mtd, buf, 2)) {
		printk("nand_write_oob: " "Failed write verify, page 0x%08x\n", page);
		ret = -EIO;
		goto out;
	}
#endif
	ret = 0;
out:
	/* Deselect and wake up anyone waiting on the device */
	this->select_chip(mtd, -1);

	return ret;
}

/****************************************************************************/
/* NAME: 		SPEAr_NAND_bbt					    						*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION: Called at the boot time. It scans the Bad blocks &          */
/*	updates the bad block tables 					    					*/
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN                                      				    				*/
/*        mtd: mtd_info structure                                           */
/* OUT :								    								*/
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
int SPEAr_NAND_bbt (struct mtd_info *mtd)
{
	struct nand_chip *this = mtd->priv;
	this->bbt_td = NULL;
	this->bbt_md = NULL;
	//if (mtd->oobblock > 512)
	//	if (mtd->oobsize > 512)
	//		return nand_scan_bbt (mtd, &spear_largepage_memorybased);
	//	else
	return nand_scan_bbt (mtd, &spear_smallpage_memorybased);
	//	return 0;
}


/****************************************************************************/
/* NAME: 		SPEAr_NAND_remove				    						*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION: Clean up routine. Called from exit() 			    		*/
/*                                                                         	*/
/* PARAMETERS:                                                             	*/
/* IN                                      				    				*/
/* OUT :								    								*/
/* RETURN:                                                                 	*/
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                          	*/
/****************************************************************************/
static int SPEAr_NAND_remove(void)
{
	nand_release(spearnand_mtd);

	/* Free the MTD device structure */
	kfree (spearnand_mtd);
	return 0;
}

/****************************************************************************/
/* NAME: 		SPEAr_NAND_init					    						*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION: Init function for nand driver. called from kernel 	    	*/
/*				init() while boot up			    						*/
/*                                                                         	*/
/* PARAMETERS:                                                             	*/
/* IN                                      				    				*/
/* OUT :								    								*/
/* RETURN:                                                                 	*/
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                          	*/
/****************************************************************************/
	static int __init
SPEAr_NAND_init (void)
{
#ifdef CONFIG_ARCH_SPEARBASIC
	EnablePhotoFrameMode_fsmc();
#endif

	return SPEAr_NAND_probe();
}

/********************************************************************************/
/* NAME: 		SPEAr_NAND_exit					        						*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION: Exit function for nand driver. called from kernel when unloaded */
/*	          as a module														*/
/*																				*/
/*                                                                             	*/
/* PARAMETERS:                                                                 	*/
/* IN                                      										*/
/* OUT :																		*/
/* RETURN:                                                                     	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                              	*/
/********************************************************************************/
	static void __exit
SPEAr_NAND_exit (void)
{
#ifdef CONFIG_ARCH_SPEARBASIC
	DisablePhotoFrameMode_fsmc();
#endif

	SPEAr_NAND_remove();
	return;
}

EXPORT_SYMBOL(SPEAr_NAND_bbt);
module_init(SPEAr_NAND_init);
module_exit(SPEAr_NAND_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("NAND driver for Spear");
