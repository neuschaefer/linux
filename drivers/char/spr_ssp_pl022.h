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
#ifndef SPEAR_SSP_H
#define SPEAR_SSP_H

#include "spr_ssp_pl022_hal.h"
#include "spr_dma_pl080.h"
#include <asm/arch/kernel_defs.h>


#define SPEAR_SSP_BASE 'S'
#define SSPIOC_CONFIG _IOWR(SPEAR_SSP_BASE,1,t_SSPConfigure)

#if SPEAR_SSP_DEBUG
#define SSPIOC_TEST _IOWR(SPEAR_SSP_BASE,2, t_SSPTest)

typedef enum
{
	ACQUIRE,
	FREE,
	CONFIGURE,
	TRANSMIT,
	RECEIVE
}t_SSPCMD;

typedef struct
{
	t_SSPId SSPId;
	void * DeviceId;
	t_SSPConfigure Config;
	uint NumOfCharTo;
	uint NumOfCharRx;
	char BufferTx[40];
	char BufferRx[40];
	t_SSPCMD Cmd;
	t_SSPError Status;
}t_SSPTest;

#endif

typedef enum
{
	TRANSMIT_DATA,
	RECEIVE_DATA
}t_SSPTransferType;

//Define the types of transmission and also max limit of transfer size for a particular transmission type.
typedef enum
{
	SSP_INTERRUPT = 50,
	SSP_DMA
}t_SSPTxRxMode;

//Define the type used for storing current status of a SSP Device.
typedef struct
{
	t_SSPId SSPId;
	t_SSPTxRxMode TxRxMode;
	t_DMAChannelId DMAChannelId[2];
	uint DataLeft;
	char *Buffer;
	void *CurrentOwner;
	uint DeviceNumber;
	spinlock_t SSPSpinLock;	//To Take a lock when Current User is to be changed or verified.
	struct cdev Cdev;
	t_DMAIntHandler DMAIntHandler;
	int FlagArrived;	//Related to WaitQueue.
	wait_queue_head_t WaitQueue;	//For Waiting in a system call.
}t_SSPStatus;

/***********************************************************************************
 * PUBLIC Functions
 ***********************************************************************************/
//This API is used to initialize the SSP.
static int __init SPEAR_SSP_Init(void);
static void __exit SPEAR_SSP_Exit(void);

//This API is used to Acquire a SSP Device.
t_SSPError SPEAR_SSP_Acquire(t_SSPId SSPId, void *DeviceId);

//This API is used to Release a SSP Device.
t_SSPError SPEAR_SSP_Free(t_SSPId SSPId, void *DeviceId);

//This API is used to Configure SSP.
t_SSPError SPEAR_SSP_Configure(t_SSPId SSPId, t_SSPConfigure *Config, void *DeviceId);

//This API is used to transmit a number of characters.
t_SSPError SPEAR_SSP_Transmit(t_SSPId SSPId, uint NumOfCharToTx, const char *Buffer, void *DeviceId);

//This API is used to Receive a number of characters.
t_SSPError SPEAR_SSP_Receive(t_SSPId SSPId, uint NumOfCharToRx, char *Buffer, uint *NumOfCharRx, void *DeviceId);

//Interrupts handler.
static irqreturn_t SPEAR_SSP_IntHandler(int Irq, void *DeviceId, struct pt_regs *Reg);

#endif // SPEAR_SSP_H
