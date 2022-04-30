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
#ifndef SPEAr_DMA_H
#define SPEAr_DMA_H

#include "spr_dma_pl080_hal.h"
#include <asm/arch/kernel_defs.h>

#if SPEAR_DMA_DEBUG
static int major = 254;		/* Major Number */
#define NAME "SPEAr_DMA"
#define SPEAr_DMA_BASE 'D'
#define DMAIOC_TRANSFER _IOWR(SPEAr_DMA_BASE,1, t_DMATransferDesc)
#define DMAIOC_TESTING _IOWR(SPEAr_DMA_BASE,3,t_DMATest)

typedef enum
{
	HALT,
	RESUME,
	ABORT,
	SETCONFIG,
	GETCONFIG,
	PRINTREG	
}t_DMACommand;

typedef struct
{
	t_DMAChannelId ChannelId;
	t_DMADeviceId DeviceId;
	t_DMADeviceDesc Device;
	t_DMACommand Command;
}t_DMATest;

#endif

/***********************************************************************************
 * PUBLIC Functions
 ***********************************************************************************/
//This API is used to transfer data.
t_DMAError SPEAr_DMA_Transfer(t_DMATransferDesc TransferDesc, t_DMAChannelId *ChannelId);
t_DMAError SPEAr_DMA_HaltTransfer(t_DMAChannelId ChannelId);
t_DMAError SPEAr_DMA_ResumeTransfer(t_DMAChannelId ChannelId);
t_DMAError SPEAr_DMA_AbortTransfer(t_DMAChannelId ChannelId, t_bool DataCanBeLost);

t_DMAError SPEAr_DMA_SetConfigDevice(t_DMADeviceId DeviceId, t_DMAAccessWidth AccessWidth, t_DMABurstSize MaxBurstSize, t_DMAProtection Protection, t_bool CanBeFlowController);

t_DMAError SPEAr_DMA_GetConfigDevice(t_DMADeviceId DeviceId, t_DMAAccessWidth *AccessWidth, t_DMABurstSize *MaxBurstSize, t_DMAProtection *Protection, t_bool *CanBeFlowController);

int SPEAr_DMA_FindNumOfLli(t_DMADeviceId SrcDeviceId, t_DMADeviceId DestDeviceId, uint TransferSize);
#endif // SPEAr_DMA_H
