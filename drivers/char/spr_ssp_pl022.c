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

/*------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------*/
#include <linux/serial_core.h>
#include <linux/tty.h>
#include <linux/serial.h>
#include <linux/console.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/timex.h>	
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <asm/arch/spr_major.h>
#include "spr_ssp_pl022.h"

#define SPEAR_SSP_NAME		"SSP"


//DMA Handler Prototypes.
static void DMAIntHandler0(t_bool, t_DMAIntType);
#ifdef CONFIG_ARCH_SPEARPLUS
static void DMAIntHandler1(t_bool, t_DMAIntType);
static void DMAIntHandler2(t_bool, t_DMAIntType);
#endif

//Storing the Current Status.
static t_SSPStatus SSPCurrentStatus[SSP_NUM]=
{
	{
		.SSPId = SSP0, 
		.TxRxMode = SSP_INTERRUPT, 
		.DMAChannelId[0] = 8,
		.DMAChannelId[1] = 8,
		.DataLeft = 0, 
		.Buffer = NULL, 
		.CurrentOwner = NULL, 
		.DeviceNumber = MKDEV(MAJOR_SSP, 0), 
		.DMAIntHandler = DMAIntHandler0, 
		.FlagArrived = 0
	}
#ifdef CONFIG_ARCH_SPEARPLUS
	,
	{
		.SSPId = SSP1, 
		.TxRxMode = SSP_INTERRUPT, 
		.DMAChannelId[0] = 8,
		.DMAChannelId[1] = 8,
		.DataLeft = 0, 
		.Buffer = NULL, 
		.CurrentOwner = NULL, 
		.DeviceNumber = MKDEV(MAJOR_SSP, 1), 
		.DMAIntHandler = DMAIntHandler1, 
		.FlagArrived = 0
	},
	{
		.SSPId = SSP2, 
		.TxRxMode = SSP_INTERRUPT, 
		.DMAChannelId[0] = 8,
		.DMAChannelId[1] = 8,
		.DataLeft = 0, 
		.Buffer = NULL, 
		.CurrentOwner = NULL, 
		.DeviceNumber = MKDEV(MAJOR_SSP, 2), 
		.DMAIntHandler = DMAIntHandler2, 
		.FlagArrived = 0
	}
#endif
};

//Storing the Latest Configurations.
t_SSPConfigure SSPCurrentConfig[SSP_NUM]=
{
	{SSP_MASTER, {MOTOROLA_SPI, CLK_POL_HIGH, CLK_RISING_EDGE}, DATA_SIZE_8, SLAVE_OUTPUT_ENABLE, 3000000, 0}
#ifdef CONFIG_ARCH_SPEARPLUS
	,
	{SSP_MASTER, {MOTOROLA_SPI, CLK_POL_HIGH, CLK_RISING_EDGE}, DATA_SIZE_8, SLAVE_OUTPUT_ENABLE, 3000000, 0},
	{SSP_MASTER, {MOTOROLA_SPI, CLK_POL_HIGH, CLK_RISING_EDGE}, DATA_SIZE_8, SLAVE_OUTPUT_ENABLE, 3000000, 0}
#endif
};

/****************************************************************************/
/* 			 SSP_SaveConfig()                	            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Sets the Current Configuration of a SSP Device.        */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - SSPId: Identify the SSP Device	                            */
/*      - Config: Pointer to Configuration Information.          	    */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static inline void SSP_SaveConfig(t_SSPId SSPId, t_SSPConfigure *Config)
{
	SSPCurrentConfig[SSPId].Mode = Config->Mode;
	SSPCurrentConfig[SSPId].FrameFormat.Interface = Config->FrameFormat.Interface;
	SSPCurrentConfig[SSPId].FrameFormat.MotoClkPolarity = Config->FrameFormat.MotoClkPolarity;
	SSPCurrentConfig[SSPId].FrameFormat.MotoClkPhase = Config->FrameFormat.MotoClkPhase;
	SSPCurrentConfig[SSPId].DataSize = Config->DataSize;
	SSPCurrentConfig[SSPId].SlaveOutput = Config->SlaveOutput;
	SSPCurrentConfig[SSPId].RequiredFrequency = Config->RequiredFrequency;
	SSPCurrentConfig[SSPId].AttainedFrequency = Config->AttainedFrequency;
}

/****************************************************************************/
/* 			 SSP_IsSameAsLastConfig()              	            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Checks that the required configuration is same as the  */
/*      Last configuration or not.                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - SSPId: Identify the SSP Device	                            */
/*      - Config: Pointer to Configuration Information.          	    */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_SSPError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static inline t_bool SSP_IsSameAsLastConfig(t_SSPId SSPId, t_SSPConfigure *Config)
{
	if((Config->Mode == SSPCurrentConfig[SSPId].Mode) && \
			(Config->FrameFormat.Interface == SSPCurrentConfig[SSPId].FrameFormat.Interface) && \
			(Config->FrameFormat.MotoClkPolarity == SSPCurrentConfig[SSPId].FrameFormat.MotoClkPolarity) && \
			(Config->FrameFormat.MotoClkPhase == SSPCurrentConfig[SSPId].FrameFormat.MotoClkPhase) && \
			(Config->DataSize == SSPCurrentConfig[SSPId].DataSize) && \
			(Config->SlaveOutput == SSPCurrentConfig[SSPId].SlaveOutput) && \
			(Config->RequiredFrequency == SSPCurrentConfig[SSPId].RequiredFrequency))
	{
		//if required config and current config are same, then return current frequency.
		Config->AttainedFrequency = SSPCurrentConfig[SSPId].AttainedFrequency;
		return TRUE;
	}

	return FALSE;
}

/****************************************************************************/
/* 			 SPEAR_SSP_Acquire()                 	            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine acquired SSP Device.		    		    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - SSPId: Identify the SSP Device	                            */
/*      - DeviceId: DeviceId of Caller Driver Or In case of User Application*/
/*                  it is pointer to the current task_struct.               */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_SSPError SPEAR_SSP_Acquire(t_SSPId SSPId, void *DeviceId)
{
	t_SSPError Status = SSP_BUSY;

	//Checking whether the SSPId is correct or not.
	if((SSPId < SSP0) || (SSPId >= SSP_ALL))
		return SSP_INVALID_ID;

	if(DeviceId == NULL)
		return SSP_NULL_POINTER;

	spin_lock(&SSPCurrentStatus[SSPId].SSPSpinLock);

	//Checking that the requested SSP is Free or Busy.
	if(SSPCurrentStatus[SSPId].CurrentOwner == NULL)
	{
		//If free then allocate it to the calling device. The calling device must free the SSP device after use.
		SSPCurrentStatus[SSPId].CurrentOwner = DeviceId;
		Status = SSP_OK;
	}

	spin_unlock(&SSPCurrentStatus[SSPId].SSPSpinLock);
	return Status;
}

/****************************************************************************/
/* 			 SPEAR_SSP_Free()                 	            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Releases a previously acquired SSP Device.		    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - SSPId: Identify the SSP Device	                            */
/*      - DeviceId: DeviceId of Caller Driver Or In case of User Application*/
/*                  it is pointer to the current task_struct.               */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_SSPError SPEAR_SSP_Free(t_SSPId SSPId, void *DeviceId)
{
	t_SSPError Status = SSP_ERROR;

	//Checking whether the SSPId is correct or not.
	if((SSPId < SSP0) || (SSPId >= SSP_ALL))
		return SSP_INVALID_ID;

	if(DeviceId == NULL)
		return SSP_NULL_POINTER;

	spin_lock(&SSPCurrentStatus[SSPId].SSPSpinLock);

	//Checking that the SSP Device is owned by the caller or somebody else.
	if(SSPCurrentStatus[SSPId].CurrentOwner == DeviceId)
	{
		//Free device if current user has acquired it before.
		SSPCurrentStatus[SSPId].CurrentOwner = NULL;
		Status = SSP_OK;
	}

	spin_unlock(&SSPCurrentStatus[SSPId].SSPSpinLock);
	return Status;
}


/****************************************************************************/
/* 			 SPEAR_SSP_Configure()	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine allows to configure a SSP Device. It calls 	    */
/*      SSP_Configure of HAL Layer				            */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - SSPId: Identify the SSP Device	                            */
/*      - Config: Configuration Information.           	    		    */
/*      - DeviceId: DeviceId of Caller Driver Or In case of User Application*/
/*                  it is pointer to the current task_struct.               */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_SSPError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_SSPError SPEAR_SSP_Configure(t_SSPId SSPId, t_SSPConfigure *Config, void *DeviceId)
{
	t_SSPError Status = SSP_OK;

	//Checking whether the SSPId is correct or not.
	if((SSPId < SSP0) || (SSPId >= SSP_ALL))
		return SSP_INVALID_ID;

	if(DeviceId == NULL)
		return SSP_NULL_POINTER;

	spin_lock(&SSPCurrentStatus[SSPId].SSPSpinLock);

	//Checking that the SSP Device is owned by the caller or somebody else.
	if(SSPCurrentStatus[SSPId].CurrentOwner != DeviceId)
		return SSP_BUSY;

	spin_unlock(&SSPCurrentStatus[SSPId].SSPSpinLock);

	//Checking if there is any need of configuring SSP Again.
	if(!SSP_IsSameAsLastConfig(SSPId, Config))
	{
		//Configure SSP.
		if((Status = SSP_Configure(SSPId, Config)) == SSP_OK)
			SSP_SaveConfig(SSPId, Config);
	}
	return Status;
}


/****************************************************************************/
/* 			 TransferAbort()	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine allows to abort any ongoing transfer. This is called   */
/*      in case the application is not closed in normal way.                */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - SSPId: Identify the SSP Device	                            */
/*                                                                          */
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void TransferAbort(t_SSPId SSPId)
{
	t_SSPIntType IntType = SSP_RX_INT | SSP_RX_OR_INT | SSP_RX_TO_INT | SSP_TX_INT;

	switch(SSPCurrentStatus[SSPId].TxRxMode)
	{
		//Disable interrupt for Interrupt mode.
		case SSP_INTERRUPT:
			SSPCurrentStatus[SSPId].DataLeft = 0;
			SSP_IntDisable(SSPId, IntType);
			break;
		//Abort DMA Transfer in case of DMA Mode.
		case SSP_DMA:
			SSPCurrentStatus[SSPId].DataLeft = 0;

			//Close channels of main transfer and dummy transfer.
			SPEAr_DMA_AbortTransfer(SSPCurrentStatus[SSPId].DMAChannelId[0], 1);
			SPEAr_DMA_AbortTransfer(SSPCurrentStatus[SSPId].DMAChannelId[1], 1);

			//save wrong ChannelId, so if somehow abort is called then normal channels are not effected.
			SSPCurrentStatus[SSPId].DMAChannelId[0] = 8;
			SSPCurrentStatus[SSPId].DMAChannelId[1] = 8;
			break;
	}
}


/****************************************************************************/
/* 			 SSP_GotoSleep()	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine is called when ssp device is required to wait for some */
/*      interval of time.                                                   */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - SSPId: Identify the SSP Device	                            */
/*                                                                          */
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int SSP_GotoSleep(t_SSPId SSPId)
{
	//Goto Sleep
	wait_event_interruptible(SSPCurrentStatus[SSPId].WaitQueue,(SSPCurrentStatus[SSPId].FlagArrived != 0));

	//if the flag arrived is 1, then it is normal wakeup.
	if(SSPCurrentStatus[SSPId].FlagArrived == 1)
		SSPCurrentStatus[SSPId].FlagArrived = 0;
	//if the flag arrived is 0, then it is not normal wakeup.
	else
	{
		SSPCurrentStatus[SSPId].FlagArrived = 0;

		//Abort Transfer in case of abnormal wakeup.
		TransferAbort(SSPId);
		return SSP_ERROR;
	}

	return SSP_OK;
}

/****************************************************************************/
/* 			 SSP_WakeUp()	         	                    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine is called to wakeup from sleep.			    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - SSPId: Identify the SSP Device	                            */
/*                                                                          */
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void SSP_WakeUp(t_SSPId SSPId)
{
	//Wake Up from Sleep 
	SSPCurrentStatus[SSPId].FlagArrived = 1;
	wake_up_interruptible(&SSPCurrentStatus[SSPId].WaitQueue);
}

/****************************************************************************/
/* 			 DMAIntHandler()	         	            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      These  are three DMAIntHandlers specific to each SSP Device         */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void DMAIntHandler0(t_bool IsTransferOver, t_DMAIntType IntType)
{
	SSP_WakeUp(SSP0);

	//save wrong ChannelId, so if somehow abort is called then normal channels are not effected.
	SSPCurrentStatus[SSP0].DMAChannelId[0] = 8;
	SSPCurrentStatus[SSP0].DMAChannelId[1] = 8;
	SPEAR_SSP_PRINT("This is SSP0 Int Handler\n");
	SPEAR_SSP_PRINT("This is DMA Interrupt Handler Function. IsTransferOver is %s. Int Type is %s\n",IsTransferOver?"TRUE":"FALSE", IntType?"INT_ERROR":"INT_TC");
}

#ifdef CONFIG_ARCH_SPEARPLUS
static void DMAIntHandler1(t_bool IsTransferOver, t_DMAIntType IntType)
{
	SSP_WakeUp(SSP1);

	//save wrong ChannelId, so if somehow abort is called then normal channels are not effected.
	SSPCurrentStatus[SSP1].DMAChannelId[0] = 8;
	SSPCurrentStatus[SSP1].DMAChannelId[1] = 8;
	SPEAR_SSP_PRINT("This is SSP1 Int Handler\n");
	SPEAR_SSP_PRINT("This is DMA Interrupt Handler Function. IsTransferOver is %s. Int Type is %s\n",IsTransferOver?"TRUE":"FALSE", IntType?"INT_ERROR":"INT_TC");
}

static void DMAIntHandler2(t_bool IsTransferOver, t_DMAIntType IntType)
{
	SSP_WakeUp(SSP2);

	//save wrong ChannelId, so if somehow abort is called then normal channels are not effected.
	SSPCurrentStatus[SSP2].DMAChannelId[0] = 8;
	SSPCurrentStatus[SSP2].DMAChannelId[1] = 8;
	SPEAR_SSP_PRINT("This is SSP2 Int Handler\n");
	SPEAR_SSP_PRINT("This is DMA Interrupt Handler Function. IsTransferOver is %s. Int Type is %s\n",IsTransferOver?"TRUE":"FALSE", IntType?"INT_ERROR":"INT_TC");
}
#endif


/****************************************************************************/
/* NAME: 		DMATransfer	 	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine configures DMA for transfer.		   	    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - SSPId: identify the SSP Device	                            */
/*      - TransferType: Transmit or Receive                                 */
/*      - Buffer: Data Buffer.                                              */
/*      - IntHandler: DMA Interrupt Handler.                                */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_SSPError                                                        */
/*--------------------------------------------------------------------------*/
static t_SSPError DMATransfer(t_SSPId SSPId, t_SSPTransferType TransferType, char * Buffer, t_DMAIntHandler IntHandler)
{
	t_DMAError DMAStatus;
	t_DMATransferDesc TransferDesc;
	t_DMAChannelId ChannelId;
	volatile uint SPEAr_SSP_APB_BASE;

	//Configure to Transfer data.
	if(TransferType == TRANSMIT_DATA)
	{
		TransferDesc.SrcDeviceId = MEMORY_DMA_DEVICE;

		switch(SSPId)
		{
			case SSP0:
				TransferDesc.DestDeviceId = SPI_0_OUT_DMA_DEVICE;
				SPEAr_SSP_APB_BASE = SPEAr_SSP0_APB_BASE;
				break;
#ifdef CONFIG_ARCH_SPEARPLUS
			case SSP1:
				TransferDesc.DestDeviceId = SPI_1_OUT_DMA_DEVICE;
				SPEAr_SSP_APB_BASE = SPEAr_SSP1_APB_BASE;
				break;
			case SSP2:
				TransferDesc.DestDeviceId = SPI_2_OUT_DMA_DEVICE;
				SPEAr_SSP_APB_BASE = SPEAr_SSP2_APB_BASE;
				break;
#endif
			default:
				return SSP_ERROR;
				break;
		}
		TransferDesc.SrcAddress = (uint *)Buffer;
		TransferDesc.DestAddress = (uint *)SSPDR;
		TransferDesc.FlowController = DMA_MEMORY_TO_PERIPHERAL;
	}
	//configure to receive data.
	else
	{
		switch(SSPId)
		{
			case SSP0:
				TransferDesc.SrcDeviceId = SPI_0_IN_DMA_DEVICE;
				SPEAr_SSP_APB_BASE = SPEAr_SSP0_APB_BASE;
				break;
#ifdef CONFIG_ARCH_SPEARPLUS
			case SSP1:
				TransferDesc.SrcDeviceId = SPI_1_IN_DMA_DEVICE;
				SPEAr_SSP_APB_BASE = SPEAr_SSP1_APB_BASE;
				break;
			case SSP2:
				TransferDesc.SrcDeviceId = SPI_2_IN_DMA_DEVICE;
				SPEAr_SSP_APB_BASE = SPEAr_SSP2_APB_BASE;
				break;
#endif
			default:
				return SSP_ERROR;
				break;
		}
		TransferDesc.DestDeviceId = MEMORY_DMA_DEVICE;
		TransferDesc.SrcAddress = (uint *)SSPDR;
		TransferDesc.DestAddress = (uint *)Buffer;
		TransferDesc.FlowController = DMA_PERIPHERAL_TO_MEMORY;
	}

	TransferDesc.TransferSize = SSPCurrentStatus[SSPId].DataLeft;
	TransferDesc.Priority  = UNDEFINED_PRIORITY;
	TransferDesc.IsIntReqAfterEachPacketTransfer = FALSE;
	TransferDesc.HandlerPtr = IntHandler;

	SSPCurrentStatus[SSPId].TxRxMode = SSP_DMA;

	//Enable DMA from SSP
	SSP_DMAEnable(SSPId);

	//Call DMA Transfer Function.
	DMAStatus = SPEAr_DMA_Transfer(TransferDesc, &ChannelId);

	//Save Channel Id in location 0 for main transfer, and on location 1 for dummy transfer.
	if(IntHandler != NULL)
		SSPCurrentStatus[SSPId].DMAChannelId[0] = ChannelId;
	else if(SSPCurrentStatus[SSPId].DMAChannelId[0] != 8)
		SSPCurrentStatus[SSPId].DMAChannelId[1] = ChannelId;

	return DMAStatus;
}

/****************************************************************************/
/* NAME: 		SPEAR_SSP_Transmit	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine allows to start transmit x number of bytes. It calls   */
/*      SSP_Transmit of HAL Layer				            */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - SSPId: identify the SSP Device	                            */
/*      - NumOfCharToTx: This is the number of characters to be transmitted.*/
/*      - DeviceId: DeviceId of Caller Driver Or In case of User Application*/
/*                  it is pointer to the current task_struct.               */
/*                                                                          */
/* OUT  :                                                                   */
/*      - Buffer: This is the pointer to the characters to be transmitted.  */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_SSPError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_SSPError SPEAR_SSP_Transmit(t_SSPId SSPId, uint NumOfCharToTx, const char *Buffer, void *DeviceId)
{
	t_SSPError Status = SSP_OK;
	uint TempTxCount = 0;

	//Checking whether the SSPId is correct or not.
	if((SSPId < SSP0) || (SSPId >= SSP_ALL))
		return SSP_INVALID_ID;

	if((DeviceId == NULL)||(Buffer == NULL))
		return SSP_NULL_POINTER;

	//Checking that the SSP Device is owned by the caller or somebody else.
	spin_lock(&SSPCurrentStatus[SSPId].SSPSpinLock);

	if(SSPCurrentStatus[SSPId].CurrentOwner != DeviceId)
		return SSP_BUSY;

	spin_unlock(&SSPCurrentStatus[SSPId].SSPSpinLock);

	//Setting Status of SSP Device.
	SSPCurrentStatus[SSPId].DataLeft = NumOfCharToTx;
	SSPCurrentStatus[SSPId].Buffer = (char *)Buffer;

	//Transfer Data in interrupt mode.
	if(NumOfCharToTx <= SSP_INTERRUPT)
	{
		t_SSPIntType IntType = SSP_TX_INT;

		SSPCurrentStatus[SSPId].TxRxMode = SSP_INTERRUPT;
		TempTxCount = 0;

		//Transmit Max Data in first call.
		if((Status = SSP_TransmitMaxChar(SSPId, SSPCurrentStatus[SSPId].DataLeft, SSPCurrentStatus[SSPId].Buffer, &TempTxCount)) == SSP_OK)
		{
			//update data left to be transmitted
			SSPCurrentStatus[SSPId].DataLeft -= TempTxCount;
			SSPCurrentStatus[SSPId].Buffer += TempTxCount;

			//Enabling Interrupts on SSP Device.
			if(SSPCurrentStatus[SSPId].DataLeft)
			{
				SSP_IntClear(SSPId, INT_ALL);
				SSP_IntEnable(SSPId, IntType);

				//Goto Sleep 
				Status = SSP_GotoSleep(SSPId);
			}
		}
	}
	//Transmit Data in DMA Mode.
	else
	{
		t_DMAError DMAStatus;

		SSPCurrentStatus[SSPId].TxRxMode = SSP_DMA;

		//Start DMA Transfer
		DMAStatus = DMATransfer(SSPId, TRANSMIT_DATA, SSPCurrentStatus[SSPId].Buffer, SSPCurrentStatus[SSPId].DMAIntHandler);
		if(DMAStatus)
			Status = SSP_ERROR;

		//Goto Sleep 
		Status = SSP_GotoSleep(SSPId);
		if(Status == SSP_OK)
			SSPCurrentStatus[SSPId].DataLeft = 0;
	}

	if(SSPCurrentStatus[SSPId].DataLeft > 0)
		return SSP_ERROR;

	return Status;
}

/****************************************************************************/
/* NAME: 		SPEAR_SSP_Receive	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine allows to start receive x number of bytes.	It calls    */
/*      SSP_Receive of HAL Layer				            */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - SSPId: identify the SSP Device	                            */
/*      - NumOfCharToRx: This is the number of characters to be Received.   */
/*      - DeviceId: DeviceId of Caller Driver Or In case of User Application*/
/*                  it is pointer to the current task_struct.               */
/*                                                                          */
/* OUT  :                                                                   */
/*      - Buffer: This is the pointer to the buffer where the data is stored*/
/*      - NumOfCharRx: This is the number of characters Received before     */
/*		       error is encountered.   				    */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_SSPError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_SSPError SPEAR_SSP_Receive(t_SSPId SSPId, uint NumOfCharToRx, char *Buffer, uint *NumOfCharRx, void *DeviceId)
{
	t_SSPError Status = SSP_OK;
	uint TempRxCount = 0;

	//Checking whether the SSPId is correct or not.
	if((SSPId < SSP0) || (SSPId >= SSP_ALL))
		return SSP_INVALID_ID;

	if((DeviceId == NULL)||(NumOfCharRx == NULL)||(Buffer == NULL))
		return SSP_NULL_POINTER;

	//Checking that the SSP Device is owned by the caller or somebody else.
	spin_lock(&SSPCurrentStatus[SSPId].SSPSpinLock);

	if(SSPCurrentStatus[SSPId].CurrentOwner != DeviceId)
		return SSP_BUSY;

	spin_unlock(&SSPCurrentStatus[SSPId].SSPSpinLock);

	//Setting Status of SSP Device.
	SSPCurrentStatus[SSPId].DataLeft = NumOfCharToRx;
	SSPCurrentStatus[SSPId].Buffer = Buffer;

	if(SSPCurrentConfig[SSPId].Mode == SSP_MASTER)
	{
		//Flush Rx FIFO, if Master is Receiver.
		SSP_FlushRxFIFO(SSPId);
	}

	//Receive Data in interrupt Mode.
	if(NumOfCharToRx <= SSP_INTERRUPT)
	{
		//Receive Data in interrupt Mode.
		t_SSPIntType IntType = SSP_RX_INT | SSP_RX_OR_INT | SSP_RX_TO_INT;

		SSPCurrentStatus[SSPId].TxRxMode = SSP_INTERRUPT;
		TempRxCount = 0;

		if(SSPCurrentConfig[SSPId].Mode == SSP_MASTER)
		{
			//Send Dummy Data
			//Check if Data Left is less than 8 bytes.
			if(SSPCurrentStatus[SSPId].DataLeft > SSP_FIFO_SIZE/2)
				SSP_TransmitDummyData(SSPId, SSP_FIFO_SIZE/2);
			else if(SSPCurrentStatus[SSPId].DataLeft > 0)
				SSP_TransmitDummyData(SSPId, SSPCurrentStatus[SSPId].DataLeft);
		}

		//Receive Max Data in first call.
		if((Status = SSP_ReceiveMaxChar(SSPId, SSPCurrentStatus[SSPId].DataLeft, SSPCurrentStatus[SSPId].Buffer, &TempRxCount)) == SSP_OK)
		{
			SSPCurrentStatus[SSPId].DataLeft -= TempRxCount;
			SSPCurrentStatus[SSPId].Buffer += TempRxCount;

			//Enabling Interrupts on SSP Device.
			if(SSPCurrentStatus[SSPId].DataLeft)
			{
				SSP_IntClear(SSPId, INT_ALL);
				SSP_IntEnable(SSPId, IntType);

				//Goto Sleep 
				Status = SSP_GotoSleep(SSPId);
			}
		}
	}
	//Receive Data in DMA Mode.
	else
	{
		t_DMAError DMAStatus;
		char *DataBuffer = NULL;

		SSPCurrentStatus[SSPId].TxRxMode = SSP_DMA;

		//Receiving Data.
		DMAStatus = DMATransfer(SSPId, RECEIVE_DATA, SSPCurrentStatus[SSPId].Buffer, SSPCurrentStatus[SSPId].DMAIntHandler);
		if(DMAStatus)
			Status = SSP_ERROR;

		if(SSPCurrentConfig[SSPId].Mode == SSP_MASTER)
		{
			//Allocating a buffer for sending dummy bytes
			DataBuffer = (char *)kmalloc(SSPCurrentStatus[SSPId].DataLeft, GFP_ATOMIC);
			if(DataBuffer == NULL)
				return SSP_ERROR;

			//Initializing memory with zero.
			memset(DataBuffer, 0, SSPCurrentStatus[SSPId].DataLeft);

			//Transmitting Dummy Data by DMA if it is a slave.
			DMAStatus = DMATransfer(SSPId, TRANSMIT_DATA, DataBuffer, NULL);
			if(DMAStatus)
				Status = SSP_ERROR;

			kfree(DataBuffer);
		}

		//Goto Sleep 
		Status = SSP_GotoSleep(SSPId);
		if(Status == SSP_OK)
			SSPCurrentStatus[SSPId].DataLeft = 0;
	}

	*NumOfCharRx = NumOfCharToRx - SSPCurrentStatus[SSPId].DataLeft;

	return Status;
}


/****************************************************************************/
/* NAME:   		SPEAR_SSP_IntHandler				    */
/*                                                 			    */  
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*                                                                          */
/* PARAMETERS:                                                              */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: 					                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static irqreturn_t SPEAR_SSP_IntHandler(int Irq, void *Dev_id, struct pt_regs *Reg)
{
	volatile uint IntSrc = 0;
	t_SSPId SSPId = SSP0;
	t_SSPError Status;
#ifdef CONFIG_ARCH_SPEARPLUS
	switch(Irq)
	{
		case IRQ_SSP_1:
			SSPId = SSP0;
			break;
		case IRQ_SSP_2:
			SSPId = SSP1;
			break;
		case IRQ_APPL_SSP:
			SSPId = SSP2;
			break;
		default:
			printk("Wrong Interrupt Handler Called\n");
	}
#endif

	//Get source of interrupt.
	IntSrc = SSP_GetIntSrc(SSPId);

	//If Transmit interrupt is active.
	if(IntSrc & SSP_TX_INT)
	{
		uint TempTxCount = 0;

		//Transmit Max Data
		if(SSP_TransmitMaxChar(SSPId, SSPCurrentStatus[SSPId].DataLeft,SSPCurrentStatus[SSPId].Buffer, &TempTxCount))
			printk("\nError in Transmit Max Char....\n");
		else
		{
			SSPCurrentStatus[SSPId].DataLeft -= TempTxCount;
			SSPCurrentStatus[SSPId].Buffer += TempTxCount;
		}

		//Clear the interrupt
		SSP_IntClear(SSPId, SSP_TX_INT);
	}
	//If Receive interrupt is active.
	if(IntSrc & (SSP_RX_INT | SSP_RX_TO_INT | SSP_RX_OR_INT))
	{
		uint TempRxCount = 0;

		if((Status = SSP_ReceiveMaxChar(SSPId, SSPCurrentStatus[SSPId].DataLeft, SSPCurrentStatus[SSPId].Buffer, &TempRxCount)) == SSP_OK)
		{
			SSPCurrentStatus[SSPId].DataLeft -= TempRxCount;
			SSPCurrentStatus[SSPId].Buffer += TempRxCount;
		}

		//Send Dummy Data if Master is Receiver
		if(SSPCurrentConfig[SSPId].Mode == SSP_MASTER)
		{
			//Check if Data Left is less than 8 bytes.
			if(SSPCurrentStatus[SSPId].DataLeft > SSP_FIFO_SIZE/2)
				SSP_TransmitDummyData(SSPId, SSP_FIFO_SIZE/2);
			else if(SSPCurrentStatus[SSPId].DataLeft > 0)
				SSP_TransmitDummyData(SSPId, SSPCurrentStatus[SSPId].DataLeft);
		}

		//Clear the interrupt
		SSP_IntClear(SSPId, (SSP_RX_TO_INT | SSP_RX_OR_INT));
	}

	//Disabling Interrupt if Transfer is over.
	if((SSPCurrentStatus[SSPId].DataLeft < 1) || (IntSrc & SSP_RX_OR_INT))
	{
		SSP_IntDisable(SSPId, INT_ALL);
		SSP_WakeUp(SSPId);
	}

	return IRQ_HANDLED;
}

/****************************************************************************/
/* NAME:           SPEAR_SSP_Open					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine opens the char device for ssp          		    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :   none                                                             */
/*                                                                          */
/* OUT :                                                                    */
/*       None                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*       int	                                                            */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int SPEAR_SSP_Open(struct inode *inode, struct file *file)
{
	t_SSPError Status = SSP_OK;
	t_SSPId SSPId;

	//Getting the Minor number of the caller.
	SSPId = iminor(inode);

	//Acquiring a SSP Device.
	if((Status = SPEAR_SSP_Acquire(SSPId, current)) == SSP_OK)
	{
		//Saving Status Structure of SSP Device for future use.
		file->private_data = &SSPCurrentStatus[SSPId];
		printk("Acquired \n");
	}
	else
		printk("not Acquired \n");

	return Status;
}



/****************************************************************************/
/* NAME:           SPEAR_SSP_Release					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine releases the char device for ssp       		    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :   none                                                             */
/*                                                                          */
/* OUT :                                                                    */
/*       None                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*       int	                                                            */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int SPEAR_SSP_Release(struct inode *inode, struct file *file)
{
	t_SSPError Status;
	t_SSPId SSPId;

	//Getting the Minor number of the caller.
	SSPId = iminor(inode);

	//Freeing SSP Device previously acquired.
	Status = SPEAR_SSP_Free(SSPId, current);

	return Status;
}



/****************************************************************************************/
/* NAME:           SPEAR_SSP_Read					    		*/
/*--------------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             		*/
/*      This routine implements the file Read operation of the Char Device SSP		*/
/*                                                                          		*/
/* PARAMETERS:                                                              		*/
/* IN  :                                                                		*/
/*         file: Char device file name                                         		*/
/*         len: Number of units to be Read                                 		*/
/* OUT :                                                                    		*/
/*         Buf: Data Read                                         			*/
/*         ppos: Number of units Read                                      		*/
/*                                                                          		*/
/* RETURN:                                                                  		*/
/*       int	                                                            		*/
/*--------------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           		*/
/****************************************************************************************/
static int SPEAR_SSP_Read(struct file *file, char __user *Buf, size_t len, loff_t *ppos)
{
	t_SSPError Status;
	t_SSPId SSPId;
	char *DataBuffer;
	uint NumOfCharRx;

	//Getting the SSP Id of the device.
	SSPId = ((t_SSPStatus *)(file->private_data))->SSPId;

	//Allocating a buffer for copying data in kernel space.
	DataBuffer = (char *)kmalloc(len, GFP_ATOMIC);
	if(DataBuffer == NULL)
		return -SSP_BUSY;

	//Initializing memory with zero.
	memset(DataBuffer, 0, len);

	//Calling Receive API.
	if((Status = SPEAR_SSP_Receive(SSPId, len, DataBuffer, &NumOfCharRx, current)) == SSP_OK)
	{
		//If Transfer is successful then copy data to User Buffer.
		copy_to_user(Buf, DataBuffer, NumOfCharRx);
	}

	//Freeing Previously allocated memory.
	kfree(DataBuffer);

	if(Status == SSP_OK)
		return NumOfCharRx;
	else
		return Status;
}



/****************************************************************************************/
/* NAME:           SPEAR_SSP_Write					    		*/
/*--------------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             		*/
/*      This routine implements the file write operation of the Char Device SSP	*/
/*                                                                          		*/
/* PARAMETERS:                                                              		*/
/* IN  :                                                                		*/
/*         file: Char device file name                                         		*/
/*         Buf: Data to be written                                         		*/
/*         len: Number of units to be written                                 		*/
/* OUT :                                                                    		*/
/*         ppos: Number of units written                                      		*/
/*                                                                          		*/
/* RETURN:                                                                  		*/
/*       int	                                                            		*/
/*--------------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           		*/
/****************************************************************************************/
static int SPEAR_SSP_Write(struct file *file, const char __user *Buf, size_t len, loff_t *ppos) 
{
	t_SSPError Status;
	t_SSPId SSPId;
	char *DataBuffer;

	//Allocating a buffer for copying data in kernel space.
	DataBuffer = (char *)kmalloc(len, GFP_ATOMIC);

	if(DataBuffer == NULL)
		return -SSP_BUSY;

	//Copying Data from User Space to Kernel Space.
	copy_from_user(DataBuffer,(char *)Buf, len);

	//Getting the SSP Id of the device.
	SSPId = ((t_SSPStatus *)(file->private_data))->SSPId;

	//Transmitting Data.
	Status = SPEAR_SSP_Transmit(SSPId, len, DataBuffer, current);

	//Freeing Previously allocated memory.
	kfree(DataBuffer);

	if(Status == SSP_OK)
		return len;
	else
		return Status;
}

#if SPEAR_SSP_DEBUG
t_SSPError SSP_IOCTL_TEST(unsigned long Buf)
{
	t_SSPTest SSPTest;

	//Copying Configuration Information from User Space to Kernel Space.
	copy_from_user(&SSPTest,(struct t_SSPTest *)Buf, sizeof(t_SSPTest));

	switch(SSPTest.Cmd)
	{
		case ACQUIRE:
			SSPTest.Status = SPEAR_SSP_Acquire(SSPTest.SSPId, SSPTest.DeviceId);
			break;
		case FREE:
			SSPTest.Status = SPEAR_SSP_Free(SSPTest.SSPId, SSPTest.DeviceId);
			break;
		case CONFIGURE:
			SSPTest.Status = SPEAR_SSP_Configure(SSPTest.SSPId, &SSPTest.Config, SSPTest.DeviceId);
			break;
		case TRANSMIT:
			SSPTest.Status = SPEAR_SSP_Transmit(SSPTest.SSPId, SSPTest.NumOfCharTo, SSPTest.BufferTx, SSPTest.DeviceId);
			break;
		case RECEIVE:
			SSPTest.Status = SPEAR_SSP_Receive(SSPTest.SSPId, SSPTest.NumOfCharTo, SSPTest.BufferRx, &SSPTest.NumOfCharRx, SSPTest.DeviceId);
			break;
		default:
			printk("Invalid Argument.\n");
	}

	copy_to_user((struct t_SSPTest *)Buf, &SSPTest, sizeof(t_SSPTest));

	return SSPTest.Status;
}
#endif

/****************************************************************************************/
/* NAME:           SPEAR_SSP_Ioctl					    		*/
/*--------------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             		*/
/*      This routine implements the IOCTL operation of the Char Device SSP. IOCTL is    */
/*      used to configure SSP Device.                                       		*/
/*                                                                          		*/
/* PARAMETERS:                                                              		*/
/* IN  :                                                                		*/
/*         inode: index node of the device      		            		*/
/*         file : char device file name					    		*/
/*         cmd  : command specifying the operation to be done               		*/
/*         arg  : Parameters to be recieved/to send                         		*/
/* OUT :                                                                    		*/
/*                                                                          		*/
/* RETURN:                                                                  		*/
/*       int	                                                            		*/
/*--------------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           		*/
/****************************************************************************************/
static int SPEAR_SSP_Ioctl(struct inode *inode, struct file *file, uint cmd, unsigned long Buf)
{
	t_SSPId SSPId;
	t_SSPConfigure SSPConfigure;
	t_SSPError Status = SSP_INVALID_PARAMETER;

	//Checking if Config Command is called.
	if(cmd == SSPIOC_CONFIG)
	{
		//Getting the SSP Id of the device.
		SSPId = ((t_SSPStatus *)(file->private_data))->SSPId;
		printk("SSP id is %d\n", SSPId);

		//Copying Configuration Information from User Space to Kernel Space.
		copy_from_user(&SSPConfigure,(struct t_SSPConfigure *)Buf, sizeof(SSPConfigure));

		//Configuring SSP Device.
		Status = SPEAR_SSP_Configure(SSPId, &SSPConfigure, current);
	}
#if SPEAR_SSP_DEBUG
	else if(cmd == SSPIOC_TEST)
	{
		Status = SSP_IOCTL_TEST(Buf);
	}
#endif

	return Status;
}


static struct file_operations SPEAR_SSP_Fops =
{
	.owner	= THIS_MODULE,
	.open   = SPEAR_SSP_Open, 
	.release= SPEAR_SSP_Release,
	.write	= SPEAR_SSP_Write,
	.read	= SPEAR_SSP_Read,
	.ioctl	= SPEAR_SSP_Ioctl
};

/****************************************************************************/
/* NAME:  			SPEAR_SSP_Exit 				    */
/*                                                 			    */  
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*                                                                          */
/* PARAMETERS:                                                              */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: t_ssp_error :  SSP error code	                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void SPEAR_SSP_Exit(void)
{
	int Index;

	//Unregistering Devices.
	for(Index=0;Index<SSP_NUM;Index++)
	{
		cdev_del(&(SSPCurrentStatus[Index].Cdev));
	}

	unregister_chrdev_region(SSPCurrentStatus[0].DeviceNumber, SSP_NUM);
}


/****************************************************************************/
/* NAME:  			SPEAR_SSP_Init 				    */
/*                                                			    */  
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*        initializes the ssp                                               */
/* PARAMETERS:                                                              */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: 						                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int __init SPEAR_SSP_Init(void)
{
	int Ret, Index;
	t_SSPError Status;
	t_SSPId SSPId;

	//Registering Character Device.
	Ret = register_chrdev_region(SSPCurrentStatus[0].DeviceNumber, SSP_NUM, SPEAR_SSP_NAME);
	if(Ret < 0)
	{
		printk(KERN_ERR SPEAR_SSP_NAME ": unable to register character device\n");
		return Ret;
	}

	//Initializing device instance for every SSP Device.
	for(Index=0;Index<SSP_NUM;Index++)
	{
		SSPCurrentStatus[Index].Cdev.owner = THIS_MODULE;

		cdev_init(&(SSPCurrentStatus[Index].Cdev), &SPEAR_SSP_Fops);
		Ret = cdev_add(&(SSPCurrentStatus[Index].Cdev), SSPCurrentStatus[Index].DeviceNumber, 1);
		if (Ret)
		{
			//If Error occurs while adding a cdev instance then exit.
			printk(KERN_ERR SPEAR_SSP_NAME ": unable to register character device\n");
			SPEAR_SSP_Exit();
			return -EINVAL;
		}
	}

	//Initializing All SSP Devices with the default settings.
	Status = SSP_Init();

	//Registering Interrupt Handlers.
#ifdef CONFIG_ARCH_SPEARPLUS
	Ret = request_irq(IRQ_SSP_1, (void *)SPEAR_SSP_IntHandler, SA_INTERRUPT, SPEAR_SSP_NAME, NULL);
	if(Ret)
	{
		printk("\n\nFailed to Install splus SSP 0 Irq handler: %d\n\n", Ret);
	}

	Ret = request_irq(IRQ_SSP_2, (void *)SPEAR_SSP_IntHandler, SA_INTERRUPT, SPEAR_SSP_NAME, NULL);
	if(Ret)
	{
		printk("\n\nFailed to Install splus SSP 1 Irq handler: %d\n\n", Ret);
	}

	Ret = request_irq(IRQ_APPL_SSP, (void *)SPEAR_SSP_IntHandler, SA_INTERRUPT, SPEAR_SSP_NAME, NULL);
	if(Ret)
	{
		printk("\n\nFailed to Install splus SSP 2 Irq handler: %d\n\n", Ret);
	}
#else
	Ret = request_irq(IRQ_SSP, (void *)SPEAR_SSP_IntHandler, SA_INTERRUPT, SPEAR_SSP_NAME, NULL);
	if(Ret)
	{
		printk("\n\nFailed to Install sbasic 0 Irq handler: %d\n\n", Ret);
	}

#endif

	//Initialize wait queue heads.
	for(SSPId = SSP0; SSPId < SSP_ALL; SSPId++)
	{
		init_waitqueue_head(&(SSPCurrentStatus[SSPId].WaitQueue));
	}

	return Status;
}

module_init(SPEAR_SSP_Init);
module_exit(SPEAR_SSP_Exit);

MODULE_AUTHOR("ST Microelectronic");
MODULE_DESCRIPTION("SPEAR generic serial port driver");
MODULE_SUPPORTED_DEVICE("ttyS");
MODULE_LICENSE("GPL");
