/********************************************************************************/
/*   Copyright (C) STMicroelectronics 2007 by Ashish Priyadarshi  			*/
/*   ashish.priyadarshi@st.com								*/
/*										*/
/*   This program is free software; you can redistribute it and/or modify	*/
/*   it under the terms of the GNU General Public License as published by	*/
/*   the Free Software Foundation; either version 2 of the License, or		*/
/*   (at your option) any later version.					*/
/*										*/
/*   This program is distributed in the hope that it will be useful,		*/
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of 		*/
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		*/
/*   GNU General Public License for more details.				*/
/*										*/
/*   You should have received a copy of the GNU General Public License		*/
/*   along with this program; if not, write to the				*/
/*   Free Software Foundation, Inc.,						*/
/*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.			*/
/********************************************************************************/

#ifndef _FSMC_H
#define _FSMC_H
#ifdef __cplusplus
extern  "C" {           /* C declarations in C++ */
#endif

	/*------------------------------------------------------------------------
	 * Includes
	 *----------------------------------------------------------------------*/

#include <linux/types.h>
#include <asm/arch/hcl_defs.h>
#include <asm/arch/hardware.h>
#include <asm/arch/spear.h>

	//#define DBG
#ifdef DBG
#define DEBUGOUT(S)		printk( S "\n")
#define DEBUGOUT1(S, A...)	printk( S "\n", A)
#else
#define DEBUGOUT(S)
#define DEBUGOUT1(S, A...)
#endif

#ifdef CONFIG_ARCH_SPEARPLUS
#define FSMC_BASE           VA_SPEARPLUS_START_NAND_FLASH_CTRLR 	
#define NAND_B0_CMEM_DATA   VA_SPEARPLUS_START_NAND_FLASH_MEM   	
#define NAND_B0_CMEM_CMD    VA_SPEARPLUS_START_NAND_FLASH_CMD   	
#define NAND_B0_CMEM_ADDR   VA_SPEARPLUS_START_NAND_FLASH_ADDR   	
#endif
 
#ifdef CONFIG_ARCH_SPEARBASIC
extern volatile unsigned int FSMC_BASE; 	
extern volatile unsigned int NAND_B0_CMEM_DATA;
extern volatile unsigned int NAND_B0_CMEM_CMD;  
extern volatile unsigned int NAND_B0_CMEM_ADDR;   
#endif
    
   
#define ECC_BYTES 3
#define ECC_CORRECTABLE 12
#define BOOT_OK          0
#define ECC_EN         0x00000040
#define ECC_PAGELEN    0x00000080

#define DEV_WIDTH_8			0x00
#define DEV_WIDTH_16	0x10
#define DEV_WIDTH_32	0x20


	/*****************************************************************************
	 * 			Defines for FSMC Registers			     *
	 *****************************************************************************/
#define GenMemCtrl0		((volatile unsigned int *)(FSMC_BASE + 0x00))
#define GenMemCtrl_tim0	((volatile unsigned int *)(FSMC_BASE + 0x04))
#define GenMemCtrl1		((volatile unsigned int *)(FSMC_BASE + 0x08))
#define GenMemCtrl_tim1	((volatile unsigned int *)(FSMC_BASE + 0x0C))
#define GenMemCtrl2		((volatile unsigned int *)(FSMC_BASE + 0x10))
#define GenMemCtrl_tim2	((volatile unsigned int *)(FSMC_BASE + 0x14))
#define GenMemCtrl3		((volatile unsigned int *)(FSMC_BASE + 0x18))
#define GenMemCtrl_tim3	((volatile unsigned int *)(FSMC_BASE + 0x1C))
#define GenMemCtrl4		((volatile unsigned int *)(FSMC_BASE + 0x20))
#define GenMemCtrl_tim4	((volatile unsigned int *)(FSMC_BASE + 0x24))
#define GenMemCtrl5		((volatile unsigned int *)(FSMC_BASE + 0x28))
#define GenMemCtrl_tim5	((volatile unsigned int *)(FSMC_BASE + 0x2C))
#define GenMemCtrl6		((volatile unsigned int *)(FSMC_BASE + 0x30))
#define GenMemCtrl_tim6	((volatile unsigned int *)(FSMC_BASE + 0x34))
#define GenMemCtrl7		((volatile unsigned int *)(FSMC_BASE + 0x38))
#define GenMemCtrl_tim7	((volatile unsigned int *)(FSMC_BASE + 0x3C))

#define GenMemCtrl_PC0		((volatile unsigned int *)(FSMC_BASE + 0x40))
#define GenMemCtrl_Int0		((volatile unsigned int *)(FSMC_BASE + 0x44))
#define GenMemCtrl_Comm0	((volatile unsigned int *)(FSMC_BASE + 0x48))
#define GenMemCtrl_Attrib0	((volatile unsigned int *)(FSMC_BASE + 0x4C))
#define GenMemCtrl_IO_ATA0	((volatile unsigned int *)(FSMC_BASE + 0x50))
#define GenMemCtrl_ECCr0	((volatile unsigned int *)(FSMC_BASE + 0x54))

#define GenMemCtrl_PC1		((volatile unsigned int *)(FSMC_BASE+ 0x60))
#define GenMemCtrl_Int1		((volatile unsigned int *)(FSMC_BASE+ 0x64))
#define GenMemCtrl_Comm1	((volatile unsigned int *)(FSMC_BASE+ 0x68))
#define GenMemCtrl_Attrib1	((volatile unsigned int *)(FSMC_BASE+ 0x6C))
#define GenMemCtrl_IO_ATA1	((volatile unsigned int *)(FSMC_BASE+ 0x70))
#define GenMemCtrl_ECCr1	((volatile unsigned int *)(FSMC_BASE+ 0x74))

#define GenMemCtrl_PC2		((volatile unsigned int *)(FSMC_BASE+ 0x80))
#define GenMemCtrl_Int2		((volatile unsigned int *)(FSMC_BASE+ 0x84))
#define GenMemCtrl_Comm2	((volatile unsigned int *)(FSMC_BASE+ 0x88))
#define GenMemCtrl_Attrib2	((volatile unsigned int *)(FSMC_BASE+ 0x8C))
#define GenMemCtrl_IO_ATA2	((volatile unsigned int *)(FSMC_BASE+ 0x90))
#define GenMemCtrl_ECCr2	((volatile unsigned int *)(FSMC_BASE+ 0x94))

#define GenMemCtrl_PC3		((volatile unsigned int *)(FSMC_BASE+ 0xA0))
#define GenMemCtrl_Int3		((volatile unsigned int *)(FSMC_BASE+ 0xA4))
#define GenMemCtrl_Comm3	((volatile unsigned int *)(FSMC_BASE+ 0xA8))
#define GenMemCtrl_Attrib3	((volatile unsigned int *)(FSMC_BASE+ 0xAC))
#define GenMemCtrl_IO_ATA3	((volatile unsigned int *)(FSMC_BASE+ 0xB0))
#define GenMemCtrl_ECCr3	((volatile unsigned int *)(FSMC_BASE+ 0xB4))

#define GenMemCtrl_TSTCR	((volatile unsigned int *)(FSMC_BASE+ 0xC0))
#define GenMemCtrl_ITIP_1	((volatile unsigned int *)(FSMC_BASE+ 0xC4))
#define GenMemCtrl_ITIP_2	((volatile unsigned int *)(FSMC_BASE+ 0xC8))
#define GenMemCtrl_ITOP_1	((volatile unsigned int *)(FSMC_BASE+ 0xCC))
#define GenMemCtrl_ITOP_2	((volatile unsigned int *)(FSMC_BASE+ 0xD0))
#define GenMemCtrl_ITOP_3	((volatile unsigned int *)(FSMC_BASE+ 0xD4))
#define GenMemCtrl_ITOP_4	((volatile unsigned int *)(FSMC_BASE+ 0xD8))

#define GenMemCtrl_delay	((volatile unsigned int *)(FSMC_BASE+ 0xFDC))
#define GenMemCtrl_PeriphD0	((volatile unsigned int *)(FSMC_BASE+ 0xFE0))
#define GenMemCtrl_PeriphD1	((volatile unsigned int *)(FSMC_BASE+ 0xFE4))
#define GenMemCtrl_PeriphD2	((volatile unsigned int *)(FSMC_BASE+ 0xFE8))
#define GenMemCtrl_PeriphD3	((volatile unsigned int *)(FSMC_BASE+ 0xFEC))
#define GenMemCtrl_PCellD0	((volatile unsigned int *)(FSMC_BASE+ 0xFF0))
#define GenMemCtrl_PCellD1	((volatile unsigned int *)(FSMC_BASE+ 0xFF4))
#define GenMemCtrl_PCellD2	((volatile unsigned int *)(FSMC_BASE+ 0xFF8))
#define GenMemCtrl_PCellD3	((volatile unsigned int *)(FSMC_BASE+ 0xFFC))

	//*****************************************************************************



	/*-----------------------------------------------------------------------------
	  Generic Macros
	  -----------------------------------------------------------------------------*/
	/* Bit setting or clearing */
#define FSMC_SET_BITS(reg,mask)			((reg) |=  (mask))
#define FSMC_CLEAR_BITS(reg,mask)		((reg) &= ~(mask))
#define FSMC_READ_BITS(reg,mask)			((reg) &   (mask))
#define FSMC_WRITE_BITS(reg,val,mask)	((reg) =   (((reg) & ~(mask)) | ((val) & (mask))))
#define FSMC_READ_REG(reg)				(reg)
#define FSMC_WRITE_REG(reg,val)			((reg) = (val))

#define FSMC_PUT_BITS(reg,val,shift,mask)   ((reg) =   (((reg) & ~(mask)) | (((t_uint32)val << shift) & (mask))))
#define FSMC_GET_BITS(reg,shift,mask)       ((((reg) & (mask)) >> shift))


	typedef enum {
		FSMC_BANK0			= 0, /*PC card 0*/
		FSMC_BANK1			= 1, /* PC card 1*/
		FSMC_BANK2			= 2, /* PC card 2*/
		FSMC_BANK3			= 3 /*PC card 3*/
	}t_fsmc_bank_index;

	typedef enum {
		FSMC_NOR_CHIP0		= 0,
		FSMC_NOR_CHIP1		= 1,
		FSMC_NOR_CHIP2		= 2,
		FSMC_NOR_CHIP3		= 3,
		FSMC_NOR_CHIP4		= 4,
		FSMC_NOR_CHIP5		= 5,
		FSMC_NOR_CHIP6		= 6,
		FSMC_NOR_CHIP7		= 7
	}t_fsmc_nor_chip_index;

	typedef enum {
		FSMC_PC_NAND_COMMON_MEM_SPACE,
		FSMC_PC_NAND_ATTR_MEM_SPACE,
		FSMC_PC_NAND_IO_SPACE
	} t_fsmc_pc_nand_mem_space;

	typedef enum {
		FSMC_STATE_DISABLE = 0,
		FSMC_STATE_ENABLE = 1
	} t_fsmc_state;

	typedef enum {
		FSMC_WAIT_ON=0,
		FSMC_WAIT_OFF
	} t_fsmc_wait;

	typedef enum {
		FSMC_BANK_DISABLE=0,
		FSMC_BANK_ENABLE
	} t_fsmc_bank;

	typedef enum {
		FSMC_NAND_MEM =0,
		FAMC_PCCARD_MEM
	} t_fsmc_mem;

	typedef enum {
		FSMC_SRAM =0,
		FSMC_CELLULAR_RAM,
		FSMC_NOR_FLASH,
		FSMC_COSMO_RAM
	}t_fsmc_nor_mem;

	typedef enum {
		FSMC_ACCESS_8 =0,
		FSMC_ACCESS_16,
		FSMC_ACCESS_32
	} t_dev_width;

	typedef enum {
		FSMC_ECC_DISABLE =0,
		FSMC_ECC_ENABLE
	} t_fsmc_ecc;

	typedef enum {
		FSMC_ECC_PAGELEN_256 =0,
		FSMC_ECC_PAGELEN_512
	}t_ecc_pagelen;

	typedef enum {
		FSMC_WRITE_PROTECTED = 0,
		FSMC_WRITE_NOT_PROTECTED
	}t_write_protected;

	typedef enum {
		FSMC_BURST_DISABLE=0,
		FSMC_BURST_ENABLE
	} t_fsmc_burst;

	/**GenMemCtrl_Comm1X, GenMemCtrl_Attrib:PCCARD COMMON MEMORY/ ATTRIBUTE MEMORY TIMING REGISTER VALUES**/
	typedef enum {
		Thiz_msk   =    0x000000ff,
		Thiz_shift =    0x0,

		Thold_msk   =   0x0000ff00,
		Thold_shift =   0x8,

		Twait_msk   =   0x00ff0000,
		Twait_shift =    0x10,

		Tset_msk    =   0xff000000,
		Tset_shift  =   0x18,
	} t_nand_timing_values;

	/*Enum for Nand Control register values */
	typedef enum {
		NAND_RESET=0x00000001,

		WAIT_ON= 0x00000002,
		WAIT_DISABLE = ~WAIT_ON,

		NAND_ENABLE=0x000000004,
		NAND_DISABLE =~NAND_ENABLE,

		NAND_TYPE=0x00000008,
		PCCARD_TYPE=~NAND_TYPE,

		/*DEV WIDTHS */
		NAND_DEV_WIDTH_8=DEV_WIDTH_8,
		NAND_DEV_WIDTH_16=DEV_WIDTH_16,
		NAND_DEV_WIDTH_32=DEV_WIDTH_32,

		ECCENABLED=0x00000040,
		ECCDISABLED=~ECCENABLED,

		ECCPAGELEN_256=0x00000080,
		ECCPAGELEN_512=~ECCPAGELEN_256,

		ADDRMUXED=0x00000100,
		ADDRDEMUX=~ADDRMUXED,

		TCLR_MSK =	0x00001E00,
		TCLR_SHIFT	=	0x09,
		TAR_MSK	=	0x0001E000,
		TAR_SHIFT	=	0xD
	} t_fsmc_pc_nand_ctrl;

	/*Enum for Nor Control register values */
	typedef enum {
		BANKENABLE= 0x00000001,
		BANKDISABLE=~BANKENABLE,
		MUXED_MEMORY=0x00000002,
		NON_MUXED_MEMORY=~MUXED_MEMORY,
		/*Memory types*/
		SRAM_MEM=0x0,
		CELLULAR_RAM=0x00000004,
		NOR_FLASH=0X00000008,
		COSMO_RAM=0x0000000C,

		/*DEV WIDTHS */
		NOR_DEV_WIDTH_8=DEV_WIDTH_8,
		NOR_DEV_WIDTH_16=DEV_WIDTH_16,
		NOR_DEV_WIDTH_32=DEV_WIDTH_32,

		RSTPOWERDOWN=0x00000040,
		WPROT=0x00000080,
		WPROT_DIS=~WPROT,
		BURSTENABLE=0x00000100,
		BURSTDISABLE=~BURSTENABLE,
		WAITPOLARITYHIGH=0x00000200,
		WAITPOLARITYLOW=~WAITPOLARITYHIGH,
		WRAPSUPPORTED=0x00000400,
		WRAPNOTSUPPORTED=~WRAPSUPPORTED,
		WAITSAME=0x00000800,
		WAITDELAY=~WAITSAME,
		IFNORWRITEENABLE=0x00001000,
		IFNORWRITEDISABLE=~IFNORWRITEENABLE,
		WAITEN=0x00002000,
		WAITDIS=~WAITEN,
		CBURSTRW=0x00080000,
		CBURSTRWDIS=~CBURSTRW

	} t_fsmc_sram_nor_ctrl ;

	/*Define the type used to describe the possible errors of that can occur.*/
	typedef enum
	{
		FSMC_OK = 0,
		FSMC_INVALID_MEMORY_SPACE,
		FSMC_INVALID_BURST_SIZE,
		FSMC_INVALID_MEMORY_TYPE,
		FSMC_INVALID_ACCESS_WIDTH,
		FSMC_INVALID_PROTECTION,
		FSMC_INVALID_BANK_NO,
		FSMC_INVALID_PARAMETER,
		FSMC_ERROR,
		FSMC_BAD_PARAM,
	}t_fsmc_error;


	/* Structure FSMC NAND Timing register */
	typedef struct {
		t_uint8 data_bus_hiz_phase;
		t_uint8 addr_hold_phase;
		t_uint8 wait_phase;
		t_uint8 addr_setup_phase;
	} t_struct_fsmc_pc_nand_tmng;

	/* Structure FSMC NAND Control register */
	typedef struct {
		t_uint8 sw_reset;
		t_uint8 wait_on;
		t_uint8 enable_bank;
		t_uint8 mem_type;
		t_uint8 dev_width;
		t_uint8 ecc;
		t_uint8 eccpagelen;
		t_uint8 tar;
		t_uint8 tclr;
	} t_struct_fsmc_pc_nand_control;

	/* Structure FSMC NOR Control register */
	typedef struct {
		t_uint8 enable_bank;
		t_uint8 mem_type;
		t_uint8 dev_width;
		t_uint8 wprot;
		t_uint8 burst_en;
		t_uint8 waitpolarity;
		t_uint8 wrapsupported;
		t_uint8 wait_delay;
		t_uint8 if_we;
		t_uint8 wait_en;
		t_uint8 cburst;
	} t_struct_fsmc_pc_nor_control;

	PUBLIC t_fsmc_error FSMC_SetSramNorControl( t_fsmc_nor_chip_index bank_no, t_struct_fsmc_pc_nor_control* nor_control);
	PUBLIC t_fsmc_error FSMC_SetNandControl(t_fsmc_bank_index bank_no, t_struct_fsmc_pc_nand_control* nand_ctrl);
	PUBLIC t_fsmc_error FSMC_SetNandTiming (t_fsmc_bank_index bank_no,
			t_fsmc_pc_nand_mem_space mem_space_type,
			t_struct_fsmc_pc_nand_tmng* nand_timing);
	PUBLIC t_fsmc_error FSMC_GetNandTiming ( t_fsmc_bank_index bank_no,
			t_fsmc_pc_nand_mem_space mem_space_type,
			t_struct_fsmc_pc_nand_tmng* nand_timing);
	PUBLIC t_uint32 FSMC_ReadEcc( t_fsmc_bank_index bank_no);
	PUBLIC t_fsmc_error FSMC_NandEccLogic(t_fsmc_bank_index bank_no, t_fsmc_ecc status,t_ecc_pagelen page_len);
	PUBLIC t_fsmc_error FSMC_WriteProtectNor(   t_fsmc_bank_index    bank_no,
			t_write_protected         protect);

	PUBLIC t_bool FSMC_IsWaitEnabled(t_fsmc_bank_index bank_no);
#ifdef __cplusplus
} /* C declarations in C++ */
#endif
#endif
