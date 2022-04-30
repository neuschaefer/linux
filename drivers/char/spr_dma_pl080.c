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
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <linux/spinlock.h>
#include "spr_dma_pl080.h"

MODULE_LICENSE("GPL");

#define SPEAr_DMA_NAME		"SPEAr_dma"

#if SPEAR_DMA_DEBUG
volatile uint TransferFlag = 0;
void TESTCASE(void);
volatile uint DMA_ONGOING = 1;
t_DMAChannelId ChannelId;
#endif

//Physical address of the LLI buffer.
dma_addr_t PhyLli;

//Logical address of the LLI buffer.
uint *LLIBufferAddr;

//Size of the LLI Buffer.
uint LLIBufferSize = 16 * 4096;

//Spin Lock Variable
static spinlock_t DMA_Spin_Lock;	//dma_LliList_lock;

extern t_DMAChannelDesc DMAChannelArray[];

/****************************************************************************/
/* 			 DMA_LockUnlock()                 	            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Allows to spin lock or Unlock.			    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - LockStatus: Lock Status, Lock or Unlock                           */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_DMAError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void DMA_LockUnlock(t_LockStatus LockStatus)
{
	//Select Lock or Unlock
	if(LockStatus == LOCK)
	{
		spin_lock(&DMA_Spin_Lock);
	}
	else if(LockStatus == UNLOCK)
	{
		spin_unlock(&DMA_Spin_Lock);
	}
}

/****************************************************************************/
/* NAME: 		SPEAr_DMA_FindNumOfLli	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine finds the number of LLI's for a DMA transfer	    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - SrcDeviceId and DestDeviceId:     identify the DMA device         */
/*      - TransferSize: Amount of Data to transfer                          */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_DMAError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
int SPEAr_DMA_FindNumOfLli(t_DMADeviceId SrcDeviceId, t_DMADeviceId DestDeviceId, uint TransferSize)
{
	return DMA_FindNumOfLli(SrcDeviceId, DestDeviceId, TransferSize);
}


/****************************************************************************/
/* NAME: 		SPEAr_DMA_Transfer	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine allows to open a communication channel 		    */
/*      between a source and a destination through the DMA Engine.          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - t_DMATransferDesc:    This field identifies the source DMA device,*/
/*                              Destination Device, Src and Dest Address,   */
/*				Transfer Size, Flow Controller, Priority    */
/*				Is Interrupt required after every LLI 	    */
/*				transfer, and Pointer to the interrupt 	    */
/*				handler routine to be called when the 	    */
/*				transfer completes or when some error occurs*/
/*				during transfer	   			    */
/*						   			    */
/* OUT : t_DMAChannelId:      This parameter is used to refer to the	    */
/* 				Open Channel				    */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_DMAError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_DMAError SPEAr_DMA_Transfer(t_DMATransferDesc TransferDesc, t_DMAChannelId *ChannelId)
{
	t_DMAError Status;
	uint *lSrcAddress = TransferDesc.SrcAddress, *lDestAddress = TransferDesc.DestAddress;

	//Checking if source or destination address are NULL.
	if(((TransferDesc.SrcAddress) == NULL) || ((TransferDesc.DestAddress) == NULL))
		return DMA_NULL_POINTER;

	//Getting the Physical addresses from logical addresses of Source or Dest Memory only and not peripheral.
	//As already physical address is only passed.
	if(TransferDesc.SrcDeviceId == MEMORY_DMA_DEVICE)
	{
		//To synchronize the data in cache with that of memory
		consistent_sync (TransferDesc.SrcAddress, TransferDesc.TransferSize, DMA_TO_DEVICE);
		TransferDesc.SrcAddress = (uint *)__pa(TransferDesc.SrcAddress);
	}
	if(TransferDesc.DestDeviceId == MEMORY_DMA_DEVICE)
	{
		//To synchronize the data in cache with that of memory
		consistent_sync (TransferDesc.DestAddress, TransferDesc.TransferSize, DMA_TO_DEVICE);
		TransferDesc.DestAddress = (uint *)__pa(TransferDesc.DestAddress);
	}

	SPEAR_DMA_PRINT("Physical Address: Src %x, Dest %x\n", TransferDesc.SrcAddress, TransferDesc.DestAddress);

	//Opening a DMA Channel
	//START SPIN LOCK.
	DMA_LockUnlock(LOCK);

	Status = DMA_OpenChannel(TransferDesc, ChannelId);

	DMA_LockUnlock(UNLOCK);
	//END SPIN LOCK.

	if(Status)
	{
		SPEAR_DMA_PRINT("Message Open Function is %d\n", Status);
		return(Status);
	}

	//Start transfer on a previously allocated channel
	//START SPIN LOCK.
	DMA_LockUnlock(LOCK);

	//Save Addresses for consistent sync in Interrupt handler
	DMAChannelArray[*ChannelId].SrcAddress = lSrcAddress;
	DMAChannelArray[*ChannelId].DestAddress = lDestAddress;
	DMAChannelArray[*ChannelId].TransferSize= TransferDesc.TransferSize;

	Status = DMA_Transfer(*ChannelId);

	DMA_LockUnlock(UNLOCK);
	//END SPIN LOCK.

	if(Status)
	{
		return(Status);
	}

	return DMA_OK;
}

/****************************************************************************/
/* NAME: 		SPEAr_DMA_HaltTransfer	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine allows to freeze or Halt an on-going DMA Transfer      */
/*      on a given DMA channel.                                             */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - ChannelId:     identify the DMA channel                     	    */
/*                                                                          */
/* OUT :  none                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_DMAError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_DMAError SPEAr_DMA_HaltTransfer(t_DMAChannelId ChannelId)
{
	t_DMAError Status;

	//START SPIN LOCK.
	DMA_LockUnlock(LOCK);

	Status = DMA_HaltTransfer(ChannelId);

	DMA_LockUnlock(UNLOCK);
	//END SPIN LOCK.

	return Status;
}


/****************************************************************************/
/* NAME: 		SPEAr_DMA_ResumeTransfer	                    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine allows to unfreeze or Resume an on-going DMA Transfer  */
/*      on a given DMA channel which has been halted previously.            */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - ChannelId:     identify the DMA channel                     	    */
/*                                                                          */
/* OUT :  none                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_DMAError                                                       */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_DMAError SPEAr_DMA_ResumeTransfer(t_DMAChannelId ChannelId)
{
	t_DMAError Status;

	//START SPIN LOCK.
	DMA_LockUnlock(LOCK);

	Status = DMA_ResumeTransfer(ChannelId);

	DMA_LockUnlock(UNLOCK);
	//END SPIN LOCK.

	return Status;
}


/****************************************************************************/
/* NAME: 		SPEAr_DMA_AbortTransfer	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine allows to Abort an on-going DMA Transfer  		    */
/*      on a given DMA channel.					            */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - ChannelId:     identify the DMA channel                     	    */
/*                                                                          */
/* OUT :  none                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_DMAError                                                       */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_DMAError SPEAr_DMA_AbortTransfer(t_DMAChannelId ChannelId, t_bool DataCanBeLost)
{
	t_DMAError Status;

	//START SPIN LOCK.
	DMA_LockUnlock(LOCK);

	Status = DMA_AbortTransfer(ChannelId, DataCanBeLost);

	DMA_LockUnlock(UNLOCK);
	//END SPIN LOCK.

	return Status;
}

/****************************************************************************/
/* t_DMAError SPEAr_DMA_SetConfigDevice()		                    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine allows to configure how a DMA device is seen by the DMA*/
/*      Depending of the DMA Device, the user can configure:                */
/*      MaxBurstSize, Protection, AccessWidth, CanBeFlowController.         */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - DeviceId:     identify the DMA device                             */
/*      - AccessWidth:   BYTE/HALFWORD/WORD                                 */
/*      - MaxBurstSize: identify the Max Burst Size                         */
/*      - Protection:   identify the protection information.                */
/*      - CanBeFlowController:	Can be flow controller.                      */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_DMAError                                                       */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_DMAError SPEAr_DMA_SetConfigDevice(t_DMADeviceId DeviceId, t_DMAAccessWidth AccessWidth, t_DMABurstSize MaxBurstSize, t_DMAProtection Protection, t_bool CanBeFlowController)
{
	t_DMAError Status;

	//START SPIN LOCK.
	DMA_LockUnlock(LOCK);

	Status = DMA_SetConfigDevice(DeviceId, AccessWidth, MaxBurstSize, Protection, CanBeFlowController);

	DMA_LockUnlock(UNLOCK);
	//END SPIN LOCK.

	return Status;
}

/****************************************************************************/
/* t_DMAError SPEAr_DMA_GetConfigDevice()		                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Gets the configuration of a DMA device		    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - DeviceId:     identify the DMA device                             */
/*                                                                          */
/* OUT : none                                                               */
/*      - AccessWidth:   BYTE/HALFWORD/WORD                                 */
/*      - MaxBurstSize: identify the Max Burst Size                         */
/*      - Protection:   identify the protection information.                */
/*      - CanBeFlowController:	Can be flow controller.                      */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_DMAError                                                       */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_DMAError SPEAr_DMA_GetConfigDevice(t_DMADeviceId DeviceId, t_DMAAccessWidth *AccessWidth, t_DMABurstSize *MaxBurstSize, t_DMAProtection *Protection, t_bool *CanBeFlowController)
{
	t_DMAError Status;

	//START SPIN LOCK.
	DMA_LockUnlock(LOCK);

	Status = DMA_GetConfigDevice(DeviceId, AccessWidth, MaxBurstSize, Protection, CanBeFlowController);

	DMA_LockUnlock(UNLOCK);
	//END SPIN LOCK.

	return Status;
}

/********************************************************************************/
/* NAME:           SPEAr_DMA_IntHandler						*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This is Interrupt Handler function.						*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  : 								    	*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: 								    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
static irqreturn_t SPEAr_DMA_IntHandler(int irq, void *dev_id, struct pt_regs *regs)
{
	t_DMAChannelId ChannelId;
	t_bool IsTransferOver;
	t_DMAIntType IntType;

	//START SPIN LOCK.
	DMA_LockUnlock(LOCK);

#if SPEAR_DMA_DEBUG
	printk("This is DMA Interrupt Handler \n");
	DMA_ONGOING = 0;
#endif
	//While there is any Interrupt left.
	while(!DMA_GetIntSrc(&ChannelId, &IntType, &IsTransferOver))
	{
		if(IsTransferOver == TRUE)
		{
			//To synchronize the data in cache with that of memory
			if(DMAChannelArray[ChannelId].DestDeviceId == MEMORY_DMA_DEVICE)
			{
				consistent_sync (DMAChannelArray[ChannelId].DestAddress, DMAChannelArray[ChannelId].TransferSize, DMA_FROM_DEVICE);
			}
		}

		DMA_IntHandler(ChannelId, IntType, IsTransferOver);
	}

	DMA_LockUnlock(UNLOCK);
	//END SPIN LOCK.
#if SPEAR_DMA_DEBUG
	printk("\n now going out of DMA Interrupt Handler \n");
#endif
	return IRQ_HANDLED;
}

#if SPEAR_DMA_DEBUG
void IntHandler(t_bool IsTransferOver, t_DMAIntType IntType)
{
	//	SPEAr_DMA_HaltTransfer(ChannelId);
	TransferFlag = 1;
	printk("This is DMA Interrupt Handler Function. IsTransferOver is %s. Int Type is %s\n",IsTransferOver?"TRUE":"FALSE", IntType?"INT_ERROR":"INT_TC");
}

void TESTCASE()
{
	t_DMAError Status;
	t_DMATransferDesc TransferDesc;
	//	t_DMAChannelId ChannelId;
	uint Index;

	//JPEG TO MEM...
	TransferDesc.SrcDeviceId = MEMORY_DMA_DEVICE;
	TransferDesc.DestDeviceId = MEMORY_DMA_DEVICE;
	TransferDesc.TransferSize = 1024*2;
	TransferDesc.FlowController = DMA_MEMORY_TO_MEMORY;
	TransferDesc.Priority = HIGH_PRIORITY;
	TransferDesc.IsIntReqAfterEachPacketTransfer = TRUE;
	TransferDesc.HandlerPtr = (t_DMAIntHandler)IntHandler;
	//	TransferDesc.SrcAddress = (uint)((uint *)__get_free_pages(GFP_DMA, 10));
	//	TransferDesc.DestAddress = (uint)((uint *)__get_free_pages(GFP_DMA, 10));
	TransferDesc.SrcAddress = (uint)((uint *)kmalloc(TransferDesc.TransferSize, GFP_DMA));
	TransferDesc.DestAddress = (uint)((uint *)kmalloc(TransferDesc.TransferSize, GFP_DMA));

	//Change Data
	for(Index=0; Index<5; Index++)
		//for(Index=0; Index<TransferDesc.TransferSize/4; Index++)
	{
		*((uint *)TransferDesc.SrcAddress + Index) = 0x12345678;
		*((uint *)TransferDesc.DestAddress + Index) = 0x00000000;
	}

	printk("src address is %x, dest is %x\n", TransferDesc.SrcAddress, TransferDesc.DestAddress);

	//Print Data Before.
	for(Index=0; Index<5; Index++)
	{
		printk("SrcAdd Val at %d\t is %x\tand at Dest is %x\n", Index, *((uint *)TransferDesc.SrcAddress +Index), *((uint *)TransferDesc.DestAddress));
	}

	Status = SPEAr_DMA_Transfer(TransferDesc, &ChannelId);
	//	Status = SPEAr_DMA_HaltTransfer(ChannelId);
	//	Status = SPEAr_DMA_ResumeTransfer(ChannelId);

	if(!Status)
	{
		while(!TransferFlag)
			printk("still not over\n");
		TransferFlag = 0;
		//consistent_sync (TransferDesc.DestAddress, TransferDesc.TransferSize, DMA_FROM_DEVICE);
	//	consistent_sync (TransferDesc.DestAddress, TransferDesc.TransferSize, DMA_BIDIRECTIONAL);
	printk("src address is %x, dest is %x\n", TransferDesc.SrcAddress, TransferDesc.DestAddress);
		if(!memcmp((uint *)TransferDesc.SrcAddress,(uint *)TransferDesc.DestAddress, TransferDesc.TransferSize))
		{
			printk("After Transfer Correct Data copied..\n");
			//Print Data Before.
			for(Index=0; Index<5; Index++)
			{
				printk("SrcAdd Val at %d\t is %x\tand at Dest is %x\n", Index, *((uint *)TransferDesc.SrcAddress +Index), *((uint *)TransferDesc.DestAddress));
			}
		}
		else
		{
			printk("After Transfer Incorrect data copied.\n");
		}

		//		free_pages(TransferDesc.SrcAddress, 10);
		//		free_pages(TransferDesc.DestAddress, 10);
	}
	else
	{
		printk("Error in DMA Transfer. %d\n",Status);
	}
}



void PrintReg(void)
{
	printk("Register Values are.\n");
	printk("DMACIntStatus is %x\n",*((volatile uint *)DMACIntStatus));
	printk("DMACRawIntTCStatus is %x\n",*((volatile uint *)DMACRawIntTCStatus));
	printk("DMACRawIntErrorStatus is %x\n",*((volatile uint *)DMACRawIntErrorStatus));
	printk("DMACIntTCStatus %x\n", *((volatile uint *)DMACIntTCStatus));
	printk("DMACIntErrorStatus %x\n", *((volatile uint *)DMACIntErrorStatus));
	printk("DMACEnbldChns %x\n", *((volatile uint *)DMACEnbldChns));
	printk("DMACConfiguration %x\n", *((volatile uint *)DMACConfiguration));
}

static int SPEAr_DMA_Open(struct inode *inode, struct file *file) 
{
	return 0;
}

static int SPEAr_DMA_Release(struct inode *inode, struct file *file) 
{
	return 0;
}

t_DMAError Testing(t_DMATest *TestingBuffer)
{
	switch(TestingBuffer->Command)
	{
		case HALT:
			return(SPEAr_DMA_HaltTransfer((*TestingBuffer).ChannelId));
			break;
		case RESUME:
			return(SPEAr_DMA_ResumeTransfer((*TestingBuffer).ChannelId));
			break;
		case ABORT:
			return(SPEAr_DMA_AbortTransfer((*TestingBuffer).ChannelId, 1));
			break;
		case SETCONFIG:
			return (SPEAr_DMA_SetConfigDevice((*TestingBuffer).DeviceId, (*TestingBuffer).Device.AccessWidth, (*TestingBuffer).Device.MaxBurstSize, (*TestingBuffer).Device.Protection, (*TestingBuffer).Device.CanBeFlowController));
			break;
		case GETCONFIG:
			return (SPEAr_DMA_GetConfigDevice((*TestingBuffer).DeviceId, &((*TestingBuffer).Device.AccessWidth), &((*TestingBuffer).Device.MaxBurstSize), &((*TestingBuffer).Device.Protection), &((*TestingBuffer).Device.CanBeFlowController)));
			break;
		case PRINTREG:
			PrintReg();
			return 0;
			break;
	}
	return 0;
}

static size_t SPEAr_DMA_IOCTL(struct inode *inode, struct file *file, uint cmd, char __user *buf)
{
	t_DMATransferDesc TransferDesc;
	t_DMAError Error = 0;
	t_DMATest TestingBuffer;
	t_DMAChannelId ChannelId;
	switch(cmd)
	{
		case DMAIOC_TRANSFER:
			printk("This is Transfer.\n");
			copy_from_user(&TransferDesc,(struct t_DMATransferDesc *)buf, sizeof(TransferDesc));

			//			TransferDesc.SrcAddress = dma_alloc_coherent (NULL, 4096*16, &SrcPhy, GFP_DMA);
			//			TransferDesc.DestAddress = dma_alloc_coherent (NULL, 4096*16, &DestPhy, GFP_DMA);
			TransferDesc.SrcAddress = (uint *)kmalloc(TransferDesc.TransferSize, GFP_DMA);
			TransferDesc.DestAddress = (uint *)kmalloc(TransferDesc.TransferSize, GFP_DMA);

			//			TransferDesc.SrcAddress = (uint *)__get_free_pages(GFP_DMA, 10);
			//			TransferDesc.DestAddress = (uint *)__get_free_pages(GFP_DMA, 10);
			//			TransferDesc.TransferSize = 1048576;

			TransferDesc.HandlerPtr = (t_DMAIntHandler)IntHandler;

			printk("Is Transfer req is %d\n", TransferDesc.IsIntReqAfterEachPacketTransfer);
			Error = SPEAr_DMA_Transfer(TransferDesc, &ChannelId);

			while(DMA_ONGOING);
			if(!memcmp(TransferDesc.SrcAddress,TransferDesc.DestAddress, TransferDesc.TransferSize))
				printk("After Transfer Correct Data copied..\n");
			else
				printk("After Transfer Incorrect data copied.\n");

			//			free_pages((uint)TransferDesc.SrcAddress, 10);
			//			free_pages((uint)TransferDesc.DestAddress, 10);

			return Error;
			break;

		default:
			printk("This is Testing.\n");
			copy_from_user(&TestingBuffer,(struct t_DMATest *)buf, sizeof(TestingBuffer));
			if((Error = Testing(&TestingBuffer)) != DMA_OK)
				return Error;
			copy_to_user((struct t_DMATest *)buf, &TestingBuffer, sizeof(TestingBuffer));
			break;
	}
	return 0;
}


//Structure defining input output operations on the char device.
static struct file_operations SPEAr_DMA_Fops =
{
	.owner	= THIS_MODULE,
	.ioctl	= SPEAr_DMA_IOCTL,
	.open   = SPEAr_DMA_Open, 
	.release= SPEAr_DMA_Release
};



#endif

/****************************************************************************/
/* NAME:           SPEAr_DMA_Init                                           */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Initializes the DMA				    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : LLIBufferAddr - Base address of the buffer provided for LLi 	    */	
/*			 Management                                         */
/*	 LLIBufferSize - Size of the buffer                                 */
/*			 						    */
/* OUT : None                                                               */
/*									    */
/* RETURN: t_DMAError							    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int __init SPEAr_DMA_Init(void)
{
	int Ret;
	t_DMAError Status;

#if SPEAR_DMA_DEBUG
	Ret = register_chrdev(major, SPEAr_DMA_NAME, &SPEAr_DMA_Fops);
	if(!Ret)
		printk("\nDMA Registered\n");

#endif

	//Installing interrupt handler.
	Ret = request_irq(IRQ_BASIC_DMA, (void *)SPEAr_DMA_IntHandler, SA_INTERRUPT, SPEAr_DMA_NAME, NULL);
	if(Ret)
	{
		printk("\n\nFailed to Install spear DMA Irq handler: %d\n\n", Ret);
		return Ret;
	}

	//Allocating memory for LLI Buffer
	LLIBufferAddr = dma_alloc_coherent(NULL, LLIBufferSize, &PhyLli, GFP_DMA);

	spin_lock_init(&DMA_Spin_Lock);

	//START SPIN LOCK.
	DMA_LockUnlock(LOCK);

	//Initializing DMA and passing the LLI buffer as parameter
	Status = DMA_Init(LLIBufferAddr, PhyLli, LLIBufferSize);

	DMA_LockUnlock(UNLOCK);
	//END SPIN LOCK.

#if SPEAR_DMA_DEBUG
	TESTCASE();
	//TEST_JPEG();
#endif

	return Status;
}


/****************************************************************************/
/* NAME:           SPEAr_DMA_Exit					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine performs the finishing tasks                           */
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
static void __exit SPEAr_DMA_Exit(void)
{
	//Freeing the memory of LLI Buffer.
	dma_free_coherent(NULL, LLIBufferSize, (void *)LLIBufferAddr, PhyLli);
}


//Module Init and Exit.
module_init(SPEAr_DMA_Init);
module_exit(SPEAr_DMA_Exit);

//Export Symbols
EXPORT_SYMBOL(SPEAr_DMA_Transfer);
EXPORT_SYMBOL(SPEAr_DMA_HaltTransfer);
EXPORT_SYMBOL(SPEAr_DMA_ResumeTransfer);
EXPORT_SYMBOL(SPEAr_DMA_AbortTransfer);
EXPORT_SYMBOL(SPEAr_DMA_SetConfigDevice);
EXPORT_SYMBOL(SPEAr_DMA_GetConfigDevice);
EXPORT_SYMBOL(SPEAr_DMA_FindNumOfLli);
