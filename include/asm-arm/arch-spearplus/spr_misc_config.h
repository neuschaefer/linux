/********************************************************************************/
/*   Copyright (C) STMicroelectronics 2007 by viresh Kumar  			*/
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
#ifndef SPEAR_MISC_CONFIG_H
#define SPEAR_MISC_CONFIG_H

#include <linux/types.h>
#include <asm/arch-spearplus/misc_regs.h>
#include <asm/arch-spearplus/hardware.h>
#include <../drivers/char/spr_dma_pl080_config_device.h>

/******************************************************************************/
/* Types definitions */
/******************************************************************************/
//Constants for configuring DMA_CHN_CFG Registers
#define DMA_CHN_CFG_SHIFT_FACTOR	2

//Typedef for Different Assignment Schemes for the current silicon version.
typedef enum
{
	DMA_SCH_O = 0x00,	//Core Logic Domain
	DMA_SCH_1 = 0x01,	//Programmable Logic Domain
	DMA_SCH_2 = 0x02,	//External Expi Domain
	DMA_SCH_NONE = 0x11
}t_MiscDMAScheme;

//Return values of the API's
typedef enum
{
	MISC_OK,
	MISC_INVALID_DMA_SCHEME,
	MISC_INVALID_REQUEST_ID
}t_MiscError;

/***********************************************************************************
* PUBLIC Functions
***********************************************************************************/
//This API Configures a DMA Request Line/Channel for a particular assignment scheme.
t_MiscError Misc_DMAConfigScheme(t_MiscDMAScheme DMAScheme, t_DMARequestId RequestId);


#endif // SPEAR_MISC_CONFIG_H
