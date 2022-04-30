/********************************************************************************/
/*   Copyright (C) STMicroelectronics 2006 by viresh Kumar  			*/
/*   viresh.kumar@st.com							*/
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
#ifndef SPEAR_JPEG_HAL_H
#define SPEAR_JPEG_HAL_H

#include "sprplus_jpeg_syn_usr.h"
#include <linux/types.h>
#include <asm/arch/spear.h>
#include <asm/arch/hardware.h>
#include <asm/arch/hcl_defs.h>

#define SPEAR_JPEG_PRINT(x...) ;/*printk*/
#define SPEAR_JPEG_DEBUG 0

/*Base Address of JPEG*/
#ifdef CONFIG_ARCH_SPEARPLUS
#define SPEAR_JPEG_AHB_BASE VA_SPEARPLUS_START_AHB_JPEG
#elif defined(CONFIG_ARCH_SPEARBASIC)
#define SPEAR_JPEG_AHB_BASE VA_SPEARBASIC_START_AHB_JPEG
#endif

/*Base Address of JPEG Controller*/
#define SPEAR_JPEG_CTRL_BASE (SPEAR_JPEG_AHB_BASE + 0x200)
/*Base Address of JPEG FIFO's*/
#define SPEAR_JPEG_FIFO_IN ((volatile uint *)(SPEAR_JPEG_AHB_BASE + 0x400))
#define SPEAR_JPEG_FIFO_OUT ((volatile uint *)(SPEAR_JPEG_AHB_BASE + 0x600))

/*JPEG Registers Mapping*/
#define JPEG_REG0	((volatile uint *)(SPEAR_JPEG_AHB_BASE + 0x000))
#define JPEG_REG1	((volatile uint *)(SPEAR_JPEG_AHB_BASE + 0x004))
#define JPEG_REG2	((volatile uint *)(SPEAR_JPEG_AHB_BASE + 0x008))
#define JPEG_REG3	((volatile uint *)(SPEAR_JPEG_AHB_BASE + 0x00c))
#define JPEG_REG4	((volatile uint *)(SPEAR_JPEG_AHB_BASE + 0x010))
#define JPEG_REG5	((volatile uint *)(SPEAR_JPEG_AHB_BASE + 0x014))
#define JPEG_REG6	((volatile uint *)(SPEAR_JPEG_AHB_BASE + 0x018))
#define JPEG_REG7	((volatile uint *)(SPEAR_JPEG_AHB_BASE + 0x01c))

/*JPEG Controller Registers Mapping*/
#define JPEG_CTRL_STATUS	((volatile uint *)(SPEAR_JPEG_CTRL_BASE + 0x000))
#define JPEG_BYTE_FIFO_TO_CORE	((volatile uint *)(SPEAR_JPEG_CTRL_BASE + 0x004))
#define JPEG_BYTE_CORE_TO_FIFO	((volatile uint *)(SPEAR_JPEG_CTRL_BASE + 0x008))

#ifndef NULL
#define NULL 0
#endif

/*Constants for configuring JPEG Registers*/
typedef enum
{
		/*R0*/
		JPEG_ENABLE				= 0x00000001,
		JPEG_DISABLE			= 0x00000000,

		/*R1*/
		MASK_NUM_COLOR_CMP		= 0x00000003,
		SHIFT_NUM_COLOR_CMP		= 0x00000000,
		RST_ENABLE				= 0x00000004,
		RST_DISABLE				= ~RST_ENABLE,
		DECODER_ENABLE			= 0x00000008,
		ENCODER_ENABLE			= ~DECODER_ENABLE,
		MASK_COL_SPC_TYPE		= 0x00000030,
		SHIFT_COL_SPC_TYPE		= 0x00000004,
		MASK_NUM_CMP_SCAN_HDR	= 0x000000C0,
		SHIFT_NUM_CMP_SCAN_HDR	= 0x00000006,
		HDR_ENABLE				= 0x00000100,
		HDR_DISABLE				= ~HDR_ENABLE,
		MASK_Y_SIZE				= 0xFFFF0000,
		SHIFT_Y_SIZE			= 0x00000010,

		/*R2*/
		MASK_NUM_MCU			= 0x03FFFFFF,
		SHIFT_NUM_MCU			= 0x00000000,

		/*R3*/
		MASK_RST_NUM_MCU		= 0x0000FFFF,
		SHIFT_RST_NUM_MCU		= 0x00000000,
		MASK_X_SIZE				= 0xFFFF0000,
		SHIFT_X_SIZE			= 0x00000010,

		/*R4-R7*/
		HUFF_TBL_DC				= 0x00000001,
		HUFF_TBL_AC				= 0x00000002,
		MASK_QUANT_TBL			= 0x0000000C,
		SHIFT_QUANT_TBL			= 0x00000002,
		MASK_NUM_DATA_UNITS		= 0x000000F0,
		SHIFT_NUM_DATA_UNITS	= 0x00000004,
		MASK_HORZ_SAMP			= 0x00000F00,
		SHIFT_HORZ_SAMP			= 0x00000008,
		MASK_VERT_SAMP			= 0x0000F000,
		SHIFT_VERT_SAMP			= 0x0000000C
}t_JPEGReg;

/*Constants for configuring JPEG Controller Registers*/
typedef enum
{
		/*Control Status Reg*/
		INT_CLR						= ~(0x00000001),
		MASK_NUM_BYTE_INVALID_LWORD	= 0x00000006,
		SHIFT_NUM_BYTE_INVALID_LWORD= 0x00000001,
		MASK_NUM_LLI				= 0x000007F8,
		SHIFT_NUM_LLI				= 0x00000003
}t_JPEGCtrlReg;

typedef enum
{
		JPEG_ENCODER,
		JPEG_DECODER
}t_JPEGConfig;

/***********************************************************************************
 * PUBLIC Functions
 ***********************************************************************************/
/*This API is used to Start the JPEG Decoding.*/
t_JPEGError JPEG_StartDecoder(uint NumOfLli, t_bool HdrEnable);

/*This API is used to Disable JPEG.*/
void JPEG_Disable(void);

/*This API is used to clear JPEG interrupt.*/
void JPEG_IntClear(void);

/*This API is used to get Header Info.*/
void JPEG_GetHdrInfo(t_JPEGHdrInfo *pHdrInfo);

#endif /* SPEAR_JPEG_HAL_H*/
