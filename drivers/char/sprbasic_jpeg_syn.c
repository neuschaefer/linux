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
/*------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------*/
#include "spr_dma_pl080.h"
#include "sprbasic_jpeg_syn.h"

static int RDmaFlagArrived = 0;
static int RJpegFlagArrived = 0;
static int DataWritten = 0;
static t_bool IsJpegError = FALSE;
static t_bool IsFullDataRead = FALSE;
static t_bool IsFirstWrite = TRUE;
static volatile uint DataToJpeg, DataFromJpeg;

static t_JPEGDev JPEGDev=
{
	.CurrentOwner = NULL,
	.DevNum[JPEG_READ] = MKDEV(SPEAR_JPEG_MAJOR, 0),
	.DevNum[JPEG_WRITE] = MKDEV(SPEAR_JPEG_MAJOR, 1),
	.FlagArrived[JPEG_READ] = 0,
	.FlagArrived[JPEG_WRITE] = 0,
	.ChannelId[JPEG_READ] = CHANNEL_INVALID,
	.ChannelId[JPEG_WRITE] = CHANNEL_INVALID,
	.RBuf = {NULL, NULL},
	.CurrentDecodingBuf = 0,
	.WBuf = NULL,
	.NumPageReq[JPEG_READ] = 0,
	.NumPageReq[JPEG_WRITE] = 0,
	.ImgSize[JPEG_READ] = 0,
	.ImgSize[JPEG_WRITE] = 0,
	.BufSize[JPEG_READ] = 0,
	.BufSize[JPEG_WRITE] = 0,
	.IsJpegOpen[JPEG_READ] = FALSE,
	.IsJpegOpen[JPEG_WRITE] = FALSE,
	.IsDecodingOver = TRUE
};

/****************************************************************************/
/* 			 				FindNumOfLli()	  		                        */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*		This function is used to get order for allocating memory. No of 	*/
/*		pages = 2^order														*/
/****************************************************************************/
static int FindNumOfLli(void)
{
	uint NumLli = 0, NumOfTransfers = 0;

	/*This is number of different DMA transfers. for ex: Img Size = 586, Buf size = 100, so there will be 5 transfers of */
	/*size 100 and one of size 86. Now NumOfTransfers = 6*/
	NumOfTransfers = (JPEGDev.ImgSize[JPEG_WRITE] + JPEGDev.BufSize[JPEG_WRITE] - 1)/JPEGDev.BufSize[JPEG_WRITE];

	if(NumOfTransfers == 1)
	{
		/*Find number of Llis required for transferring data to JPEG.*/
		/*This will give num of LLI for each transfer of size BufSize.*/
		NumLli = SPEAr_DMA_FindNumOfLli(MEMORY_DMA_DEVICE, JPEG_IN_DMA_DEVICE, JPEGDev.ImgSize[JPEG_WRITE]);
	}
	else
	{
		/*Find number of Llis required for transferring data to JPEG.*/
		/*This will give num of LLI for each transfer of size BufSize.*/
		NumLli = SPEAr_DMA_FindNumOfLli(MEMORY_DMA_DEVICE, JPEG_IN_DMA_DEVICE, JPEGDev.BufSize[JPEG_WRITE]);

		/*Now NumLli will be num of LLI for all the BufSize transfers, i.e. 5, plus one LLI for every transfer other than the*/
		/*First transfer.*/
		NumLli = (NumLli * (NumOfTransfers - 1)) + NumOfTransfers - 1;

		/*This adds NumLli of the last transfer.*/
		NumLli += SPEAr_DMA_FindNumOfLli(MEMORY_DMA_DEVICE, JPEG_IN_DMA_DEVICE, (JPEGDev.ImgSize[JPEG_WRITE] - (NumOfTransfers - 1)* JPEGDev.BufSize[JPEG_WRITE]));
	}

	return NumLli;
}

/****************************************************************************/
/* 			 				FindNumOfPages()	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*		This function is used to get order for allocating memory. No of 	*/
/*		pages = 2^order														*/
/****************************************************************************/
static int FindNumOfPages(uint Size)
{
	uint NumOfPages = 0;

	while(Size>(PAGE_SIZE * (1<<NumOfPages)))
		NumOfPages++;

	return NumOfPages;
}

/****************************************************************************/
/* 			 				FindDataPerBurst()	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine gives the amount of data transferred in one burst.     */
/****************************************************************************/
static int FindDataPerBurst(t_DMADeviceId DeviceId)
{
	t_DMAAccessWidth AccessWidth;
	t_DMABurstSize MaxBurstSize;
	t_DMAProtection Protection;
	t_bool CanBeFlowController;
	uint BurstSize = 0;

	/*Get JPEG configuration from DMA Driver.*/
	SPEAr_DMA_GetConfigDevice(DeviceId, &AccessWidth, &MaxBurstSize, &Protection, &CanBeFlowController);

	switch(MaxBurstSize)
	{
		case BURST_SIZE_1:
			BurstSize = 1;
			break;
		default:
			/*Shifting is done because of the type of t_DMABurstSize enum. The values are 1,2,3,4 but required values are 1,2,4,8,16*/
			BurstSize = 2<<MaxBurstSize;
			break;
	}

	return (AccessWidth*BurstSize);
}

/****************************************************************************/
/* 			 JPEG_GotoSleep()	                           					*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine is called when JPEG device is required to wait for some*/
/*      interval of time.                                                   */
/****************************************************************************/
static int JPEG_GotoSleep(t_JPEGDevType DevType)
{
	//Goto Sleep
	wait_event_interruptible_timeout(JPEGDev.WaitQueue[DevType], JPEGDev.FlagArrived[DevType]!= 0, JPEG_TIMEOUT);

	//if the flag arrived is 1, then it is normal wakeup.
	if(JPEGDev.FlagArrived[DevType] != 0)
	{
		JPEGDev.FlagArrived[DevType]--;
		return JPEG_OK;
	}
	//if the flag arrived is 0, then it is not normal wakeup.
	else
	{
		return JPEG_SLEEP_ERROR;
	}
}

/****************************************************************************/
/* 			 JPEG_WakeUp()	         	                    				*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine is called to wakeup from sleep.					    */
/****************************************************************************/
static void JPEG_WakeUp(t_JPEGDevType DevType)
{
	//Wake Up from Sleep 
	JPEGDev.FlagArrived[DevType]++;
	wake_up_interruptible(&JPEGDev.WaitQueue[DevType]);
}

/****************************************************************************/
/* 			 IntHandlerMemToJpeg()	                      				   	*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This is interrupt handler of MEM to JPEG Transfer.                  */
/****************************************************************************/
static void IntHandlerMemToJpeg(t_bool IsTransferOver, t_DMAIntType IntType)
{
	JPEGDev.ChannelId[JPEG_WRITE] = CHANNEL_INVALID;
	JPEG_WakeUp(JPEG_WRITE);
}

/****************************************************************************/
/* 			 IntHandlerJpegToMem()	                      				    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This is interrupt handler of JPEG to MEM Transfer.                  */
/****************************************************************************/
static void IntHandlerJpegToMem(t_bool IsTransferOver, t_DMAIntType IntType)
{
	/*Set Flag of occurance of DMA interupt. Both DMA and JPEG interrupts should come before the read call is wake up.*/
	RDmaFlagArrived = 1;
	JPEGDev.ChannelId[JPEG_READ] = CHANNEL_INVALID;
	if(RJpegFlagArrived)
	{
		RDmaFlagArrived = RJpegFlagArrived = 0;
		JPEG_WakeUp(JPEG_READ);
	}
}

/****************************************************************************/
/* NAME:           JPEG_FreeBuf											    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      Theis routine is used to free memory allocated for Jpeg buffers.    */
/****************************************************************************/
static void JPEG_FreeBuf(t_JPEGDevType DevType)
{
	switch(DevType)
	{
		case JPEG_READ:
			if(JPEGDev.RBuf[0] != NULL)
			{
				free_pages((int)JPEGDev.RBuf[0], JPEGDev.NumPageReq[DevType]);
				JPEGDev.RBuf[0] = NULL;
				JPEGDev.RBuf[1] = NULL;
			}
			break;
		case JPEG_WRITE:
			if(JPEGDev.WBuf != NULL)
			{
				free_pages((int)JPEGDev.WBuf, JPEGDev.NumPageReq[DevType]);
				JPEGDev.WBuf = NULL;
			}
			break;
		default:
			return;
			break;
	}
	JPEGDev.NumPageReq[DevType] = 0;
	JPEGDev.BufSize[DevType] = 0;
}

/****************************************************************************/
/* NAME:           JPEG_AllocBuf										    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      Theis routine is used to allocate memory for JPEG Buffers.        	*/
/****************************************************************************/
static t_JPEGError JPEG_AllocBuf(t_JPEGDevType DevType, size_t size)
{
	if(JPEGDev.IsDecodingOver)
	{
		/*Free any memory previously aquired after opening current JPEG Nodes..*/
		JPEG_FreeBuf(DevType);
		switch(DevType)
		{
			case JPEG_READ:
				/*Firstly find number of pages required and then allocate memory for them*/
				JPEGDev.NumPageReq[DevType] = FindNumOfPages(size);
				if(!(JPEGDev.RBuf[0] = (uint *)__get_free_pages(GFP_DMA, JPEGDev.NumPageReq[DevType])))
				{
					JPEGDev.NumPageReq[DevType] = 0;
					printk("JPEG: Not Able to allocate enough memory\n");
					return -JPEG_NO_MEM_AVAILABLE;
				}
				/*size is the memory required for two buffers for writing decoded image.BufSize is size of one buffer.*/
				size /=2;
				JPEGDev.RBuf[1] = (uint *)((char *)JPEGDev.RBuf[0] + size);
				break;
			case JPEG_WRITE:
				JPEGDev.NumPageReq[DevType] = FindNumOfPages(size);
				if(!(JPEGDev.WBuf = (uint *)__get_free_pages(GFP_DMA, JPEGDev.NumPageReq[DevType])))
				{
					JPEGDev.NumPageReq[DevType] = 0;
					printk("JPEG: Not Able to allocate enough memory\n");
					return -JPEG_NO_MEM_AVAILABLE;
				}
				break;
			default:
				return (-EINVAL);
				break;
		}
		JPEGDev.BufSize[DevType] = size;
		return JPEG_OK;
	}
	else
	{
		return -JPEG_BUSY;
	}
}

/****************************************************************************/
/* NAME:           JPEG_Remap											    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      Theis routine is used to remap physical memory to virtual memory.  	*/
/****************************************************************************/
static int JPEG_Remap(uint *PhyAddr, struct vm_area_struct * vma)
{
	/*Calculate size requested for mapping*/
	size_t size = vma->vm_end - vma->vm_start;

	/*Getting the page offset from the physical address of page*/
	vma->vm_pgoff = (__pa(PhyAddr)) >> PAGE_SHIFT;

	/*This code is responsible for allocating uncached memory to user space.*/
	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
	
	/* Remap-pfn-range will mark the range VM_IO and VM_RESERVED */
	if (remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff, size, vma->vm_page_prot))
		return -EAGAIN;

	return JPEG_OK;
}

/****************************************************************************/
/* NAME:           JPEG_Flush											    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine is used to flush JPEG status for new decodings.    	*/
/****************************************************************************/
static void JPEG_Flush(void)
{
	JPEGDev.FlagArrived[JPEG_READ] = 0;
	JPEGDev.FlagArrived[JPEG_WRITE] = 0;
	JPEGDev.ChannelId[JPEG_READ] = CHANNEL_INVALID;
	JPEGDev.ChannelId[JPEG_WRITE] = CHANNEL_INVALID;
	JPEGDev.IsDecodingOver = TRUE;
	JPEGDev.CurrentDecodingBuf = 0;
	JPEGDev.ImgSize[JPEG_READ] = 0;
	JPEGDev.ImgSize[JPEG_WRITE] = 0;

	RDmaFlagArrived = 0;
	RJpegFlagArrived = 0;
	IsFullDataRead = FALSE;
	DataWritten = 0;
	IsFirstWrite = TRUE;
	IsJpegError = FALSE;
}

/****************************************************************************/
/* NAME:           JPEG_Abort											    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      Theis routine is used to Abort JPEG Decoding Process             	*/
/****************************************************************************/
static void JPEG_Abort(void)
{
	uint DataCanBeLost = TRUE;

	/*If DMA is busy doing transfer with JPEG, then Abort them.*/
	if(JPEGDev.ChannelId[JPEG_READ] != CHANNEL_INVALID)
		SPEAr_DMA_AbortTransfer(JPEGDev.ChannelId[JPEG_READ], DataCanBeLost);
	if(JPEGDev.ChannelId[JPEG_WRITE] != CHANNEL_INVALID)
		SPEAr_DMA_AbortTransfer(JPEGDev.ChannelId[JPEG_WRITE], DataCanBeLost);

	JPEG_Reset();
	JPEG_Flush();
	/*Indicate occurance of error*/
	IsJpegError = TRUE;
	/*wake up any sleeping process.*/
	JPEG_WakeUp(JPEG_WRITE);
	JPEG_WakeUp(JPEG_READ);
}

/****************************************************************************/
/* NAME:           WriteImgToJpeg and ReadImgFromJpeg					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      These routines are for trnasferring data TO and FROM JPEG. 		    */
/****************************************************************************/
static t_DMAError WriteImgToJpeg(size_t SrcPacketSize)
{
	t_DMATransferDesc TransferDesc;
	t_DMAError Status;

	/*Configure Transfer Descriptor of DMA*/
	TransferDesc.SrcDeviceId = MEMORY_DMA_DEVICE;
	TransferDesc.DestDeviceId = JPEG_IN_DMA_DEVICE;
	TransferDesc.SrcAddress = (uint *)JPEGDev.WBuf;
	TransferDesc.DestAddress = (uint *)SPEAR_JPEG_FIFO_IN;
	TransferDesc.TransferSize = SrcPacketSize;
	TransferDesc.FlowController = DMA_MEMORY_TO_PERIPHERAL;
	TransferDesc.Priority = UNDEFINED_PRIORITY;
	TransferDesc.IsIntReqAfterEachPacketTransfer = FALSE;
	TransferDesc.HandlerPtr = IntHandlerMemToJpeg;

	Status = SPEAr_DMA_Transfer(TransferDesc, &JPEGDev.ChannelId[JPEG_WRITE]);
	SPEAR_JPEG_PRINT("\nStatus for 2nd Transfer is %d and channel is %d\n", Status, JPEGDev.ChannelId[JPEG_WRITE]);
	return Status;
}

static t_DMAError ReadImgFromJpeg(void)
{
	t_DMATransferDesc TransferDesc;
	t_DMAError Status;

	/*Configure Transfer Descriptor of DMA*/
	TransferDesc.SrcDeviceId = JPEG_OUT_DMA_DEVICE;
	TransferDesc.DestDeviceId = MEMORY_DMA_DEVICE;
	TransferDesc.SrcAddress = (uint *)SPEAR_JPEG_FIFO_OUT;
	TransferDesc.DestAddress = (uint *)JPEGDev.RBuf[JPEGDev.CurrentDecodingBuf];
	TransferDesc.FlowController = SRC_PERIPHERAL_TO_MEMORY;
	TransferDesc.Priority = UNDEFINED_PRIORITY;
	TransferDesc.IsIntReqAfterEachPacketTransfer = FALSE;
	TransferDesc.HandlerPtr = IntHandlerJpegToMem;

	Status = SPEAr_DMA_Transfer(TransferDesc, &JPEGDev.ChannelId[JPEG_READ]);
	SPEAR_JPEG_PRINT("\nStatus for 1st Transfer is %d and channel is %d\n", Status, JPEGDev.ChannelId[JPEG_READ]);
	return Status;
}

/****************************************************************************/
/* NAME:           				StartDecoding 								*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      These routines is used to start decoding and also write data chunk  */
/*      to JPEG.                                                            */
/****************************************************************************/
static t_JPEGError StartDecoding(uint SrcImgSize)
{
	int NumLli = 0, NumBurst = 0;

	/*If Image is decoded and extra data is sent from source side, then indicate overflow*/
	if(IsFullDataRead)
		return -JPEG_SRC_IMAGE_OVERFLOW;

	/*If memory is not allocated for Src or Dest image or SrcImgSize is zero, then indicate error.*/
	if((JPEGDev.BufSize[JPEG_READ] == 0) || (JPEGDev.BufSize[JPEG_WRITE] == 0) || SrcImgSize == 0)
		return -JPEG_IMAGE_SIZE_ZERO;

	/*Write Image packet to JPEG.*/
	if(WriteImgToJpeg(SrcImgSize) != 0)
	{
		JPEG_Abort();
		return -JPEG_DMA_ERROR;
	}

	/*Do this only for first write.*/
	if(IsFirstWrite == TRUE)
	{
		JPEGDev.IsDecodingOver = FALSE;

		/*Find number of Llis required for transferring data to JPEG.*/
		NumLli = FindNumOfLli();
		/*If number of LLI's is more than 255, then return error. This is not supported by H/W.*/
		if(NumLli > (MASK_NUM_LLI >> SHIFT_NUM_LLI))
		{
			JPEG_Abort();
			return -JPEG_DMA_ERROR;
		}

		/*find number of bursts after which we want interrupt. Interrupt should come after transfer of BufSize amount of data.*/
		NumBurst = JPEGDev.BufSize[JPEG_READ]/FindDataPerBurst(JPEG_OUT_DMA_DEVICE);

		/*Start transfer of decoded image to the allocated buffer.*/
		if(ReadImgFromJpeg() != 0)
		{
			JPEG_Abort();
			return -JPEG_DMA_ERROR;
		}

		/*Calculate Size of the output decoded image*/
/*		JPEGDev.ImgSize[JPEG_READ] = FindDecodedImgSize((const char *)JPEGDev.WBuf, SrcImgSize);
		if(!JPEGDev.ImgSize[JPEG_READ])
		{
			printk("Decoded Img Size is zero\n");
			JPEG_Abort();
			return -JPEG_IMAGE_SIZE_ZERO;
		}
*/
		/*Start JPEG Decoding.*/
		JPEG_StartDecoder(NumLli, NumBurst, TRUE);
		IsFirstWrite = FALSE;
	}

	/*Sleep while Data Transfer to JPEG is not over*/
	if(JPEG_GotoSleep(JPEG_WRITE) == JPEG_SLEEP_ERROR)
	{
		printk("JPEG Write: Not able to sleep properly.\n");
		JPEG_Abort();
		return -JPEG_SLEEP_ERROR;
	}
	/*Check if some error occured while this process is waiting.*/
	if(IsJpegError == TRUE)
		return -JPEG_ERROR;

	/*Update amount of data written to JPEG. Not used currently, but may be used.*/
	DataWritten += SrcImgSize;
	
	return JPEG_OK;
}

/****************************************************************************/
/* NAME:           				GetDecodedData 								*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      These routines is used to Get Data After Data is available in Buffer*/
/****************************************************************************/
static t_JPEGError GetDecodedData(volatile uint *DataSize)
{
	int i;
	volatile uint *DataAddr;

	/*If Image is decoded and extra data is sent from source side, then indicate overflow*/
	if(IsFullDataRead)
	{
		*DataSize = 0;
		return JPEG_OK;
	}

	/*Sleep while Decoding is not completed*/
	if(JPEG_GotoSleep(JPEG_READ) == JPEG_SLEEP_ERROR)
	{
		printk("JPEG Read: Not able to sleep properly.\n");
		JPEG_Abort();
		return -JPEG_SLEEP_ERROR;
	}
	/*Check if some error occured while this process is waiting.*/
	if(IsJpegError == TRUE)
		return -JPEG_ERROR;

	/*Store Buffer Address for changing endianness.*/
	DataAddr = JPEGDev.RBuf[JPEGDev.CurrentDecodingBuf];

	if(JPEGDev.IsDecodingOver)
	{
		/*Go here if Decoding is over.*/
		/*Calculate Data transfer in last transfer*/
		if(DataFromJpeg < JPEGDev.ImgSize[JPEG_READ])
		{
			*DataSize = 0;
			return -JPEG_ERROR;
		}
		
		*DataSize = DataFromJpeg - JPEGDev.ImgSize[JPEG_READ];

		IsFullDataRead = TRUE;
		/*Wake up write process. Abort any DMA Transfers ongoing on write side. Used if the image has more than data actually required.*/
		JPEG_WakeUp(JPEG_WRITE);
		if(JPEGDev.ChannelId[JPEG_WRITE] != CHANNEL_INVALID)
			SPEAr_DMA_AbortTransfer(JPEGDev.ChannelId[JPEG_WRITE], TRUE);
	}
	else
	{
		/*Go here for burst transfer interrupts.*/
		*DataSize = JPEGDev.BufSize[JPEG_READ];

		/*Change buffer for decoded data and start JPEG transfer.*/
		BUF_SHUFFLE(JPEGDev.CurrentDecodingBuf);
		if(ReadImgFromJpeg() != 0)
		{
			SPEAr_DMA_AbortTransfer(JPEGDev.ChannelId[JPEG_WRITE], TRUE);
			JPEG_Flush();
			return -JPEG_DMA_ERROR;
		}
	}

	/*Update total amount of file decoded.*/
	JPEGDev.ImgSize[JPEG_READ] += *DataSize;

	/*Changing Endianness of Output Data. This will be done by JPEG h/w in next release.*/
	for(i=0;i<*DataSize/4;i++)
	{
		*(DataAddr + i) = ((*(DataAddr + i))>>24) | (((*(DataAddr + i))&0x00FF0000)>>8) | (((*(DataAddr + i))&0x0000FF00)<<8) | (((*(DataAddr + i))&0x000000FF)<<24);
	}

	/*Synchronize data b/w cache and RAM. It should work with DMA_TO_DEVICE but was not working so DMA_BIDIRECTIONAL is used.*/
	consistent_sync ((uint *)DataAddr, *DataSize, DMA_BIDIRECTIONAL);
	return JPEG_OK;
}

/****************************************************************************/
/* NAME:           	SPEAR_JPEG_Open											*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      These routines opens the char device for jpeg reading and writing.  */
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
static int SPEAR_JPEG_Open(struct inode *inode, struct file *file)
{
	t_JPEGError Status = -JPEG_BUSY;
	t_JPEGDevType DevType;

	//Getting the Minor number of the caller.
	DevType = (t_JPEGDevType)iminor(inode);

	/*Check if JPEG not used by any other user*/
	if(JPEGDev.IsJpegOpen[DevType] == FALSE)
	{
		/*Mark JPEG as Busy.*/
		JPEGDev.IsJpegOpen[DevType] = TRUE;
		Status = JPEG_OK;
	}

	return Status;
}

/****************************************************************************/
/* NAME:           				SPEAR_JPEG_Release					    	*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine releases the char device for jpeg       		    	*/
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
static int SPEAR_JPEG_Release(struct inode *inode, struct file *file)
{
	t_JPEGDevType DevType;

	//Getting the Minor number of the caller.
	DevType = (t_JPEGDevType)iminor(inode);

	/*Mark JPEG as Busy.*/
	JPEGDev.IsJpegOpen[DevType] = FALSE;

	JPEG_FreeBuf(DevType);
	return JPEG_OK;
}

/************************************************************************************/
/* NAME:           SPEAR_JPEG_Ioctl					    							*/
/*----------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             		*/
/*      This routine implements the IOCTL operation of the Char Device JPEG. IOCTL  */
/*    is used to configure JPEG Device.                                       		*/
/*                                                                          		*/
/* PARAMETERS:                                                              		*/
/* IN  :                                            	                    		*/
/*         inode: index node of the device      		            		*/
/*         file : char device file name					    		*/
/*         cmd  : command specifying the operation to be done               		*/
/*         arg  : Parameters to be recieved/to send                         		*/
/* OUT :                                                                    		*/
/*                                                                          		*/
/* RETURN:                                                                  		*/
/*       int	                                                            		*/
/*----------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           		*/
/************************************************************************************/
static int SPEAR_JPEG_Ioctl(struct inode *inode, struct file *file, uint cmd, unsigned long Buf)
{
	t_JPEGDevType DevType;
	t_JPEGError Status = JPEG_OK;
	uint DataSize = 0;
	uint NumByteNotCopied = 0;

	//Getting the Minor number of the caller.
	DevType = (t_JPEGDevType)iminor(inode);

	switch (DevType)
	{
		case JPEG_READ:
			switch (cmd)
			{
				/*For getting Header information, only after completion of Decoding.*/
				case JPEGIOC_GET_HDR:
					if(JPEGDev.IsDecodingOver)
					{
						t_JPEGHdrInfo HdrInfo;
						JPEG_GetHdrInfo(&HdrInfo);
						NumByteNotCopied = copy_to_user((t_JPEGHdrInfo *)Buf, &HdrInfo, sizeof(t_JPEGHdrInfo));
					}
					else
						return -JPEG_DECODING_NOT_OVER;
					break;
				/*Get amount of Data read*/
				case JPEGIOC_GET_DECODED_DATA_SIZE:
					if((Status = GetDecodedData(&DataSize)) == JPEG_OK)
						NumByteNotCopied = copy_to_user((uint *)Buf, &DataSize, sizeof(uint));
					break;
				default:
					return (-EINVAL);
					break;
			}
			break;

		case JPEG_WRITE:
			switch (cmd)
			{
				/*This tells the amount of data to be transferred to JPEG.*/
				case JPEGIOC_START_DECODE:
					Status = StartDecoding(Buf);
					break;
				/*Used for setting JPEG Src Image size. This must be called before every transfer.*/
				case JPEGIOC_SET_SRC_IMG_SIZE:
					JPEG_Flush();
//					JPEG_FlushRxFifo();
					JPEG_Reset();
					JPEG_Disable();
					if(Buf == 0)
						return -JPEG_IMAGE_SIZE_ZERO;
					JPEGDev.ImgSize[JPEG_WRITE] = Buf;
					break;
				default:
					return (-EINVAL);
					break;
			}
			break;

		default:
			return (-EINVAL);
			break;
	}
	return Status;
}

/************************************************************************************/
/* NAME:           SPEAR_JPEG_RMmap					    							*/
/*----------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             		*/
/*      This routine implements the Mmap operation of the Char Device JPEG Read.	*/
/*                                                                          		*/
/* PARAMETERS:                                                              		*/
/* IN  :                                            	                    		*/
/*         file : char device file name					    						*/
/*         vma: struct having info of virtual memory area.                  		*/
/* OUT :                                                                    		*/
/*                                                                          		*/
/* RETURN:                                                                  		*/
/*       int	                                                            		*/
/*----------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           		*/
/************************************************************************************/
static int SPEAR_JPEG_RMmap(struct file * file, struct vm_area_struct * vma)
{
	size_t size = vma->vm_end - vma->vm_start;
	t_JPEGError Status = JPEG_OK;

	/*Memory for both Read buffers is allocated in a single call from user level. and is remaped.*/
	if((Status = JPEG_AllocBuf(JPEG_READ, size)) == JPEG_OK)
	{
		JPEG_Remap(JPEGDev.RBuf[0], vma);
	}

	return Status;
}


/************************************************************************************/
/* NAME:           SPEAR_JPEG_WMmap					    							*/
/*----------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             		*/
/*      This routine implements the Mmap operation of the Char Device JPEG Write.   */
/*                                                                          		*/
/* PARAMETERS:                                                              		*/
/* IN  :                                            	                    		*/
/*         file : char device file name					    						*/
/*         vma: struct having info of virtual memory area.                  		*/
/* OUT :                                                                    		*/
/*                                                                          		*/
/* RETURN:                                                                  		*/
/*       int	                                                            		*/
/*----------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           		*/
/************************************************************************************/

static int SPEAR_JPEG_WMmap(struct file * file, struct vm_area_struct * vma)
{
	size_t size = vma->vm_end - vma->vm_start;
	t_JPEGError Status = JPEG_OK;

	/*Allocate memory for JPEG Write buffer and remap it.*/
	if((Status = JPEG_AllocBuf(JPEG_WRITE, size)) == JPEG_OK)
	{
		JPEG_Remap(JPEGDev.WBuf, vma);
	}

	return Status;
}

/*This structure is used to show interface to user level*/
static struct file_operations SPEAR_JPEG_Fops[DEV_TYPE_MAX] =
{
	{
		.owner	= THIS_MODULE,
		.open   = SPEAR_JPEG_Open, 
		.release= SPEAR_JPEG_Release,
		.ioctl	= SPEAR_JPEG_Ioctl,
		.mmap	= SPEAR_JPEG_RMmap
	},
	{
		.owner	= THIS_MODULE,
		.open   = SPEAR_JPEG_Open, 
		.release= SPEAR_JPEG_Release,
		.ioctl	= SPEAR_JPEG_Ioctl,
		.mmap	= SPEAR_JPEG_WMmap
	}
};

/****************************************************************************/
/* NAME:   					SPEAR_JPEG_IntHandler 	    */
/*                                                 			    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*                                                                          */
/* PARAMETERS:                                                              */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: 					                           */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static irqreturn_t SPEAR_JPEG_IntHandler(int Irq, void *dev_id, struct pt_regs *regs)
{
	/*Get amount of data transfered to and from JPEG.*/
	JPEG_GetCtrlDataStatus((uint *)&DataToJpeg, (uint *)&DataFromJpeg);

	/*Check if interrupt came due to End of decoding.*/
	if((JPEGDev.IsDecodingOver = JPEG_IsDecodingOver()) == TRUE)
	{
		/*End of Decoding.*/
		JPEG_IntClear();
		JPEG_Disable();
	}
	else
	{
		/*Burst Interrupt.*/
		JPEG_BurstIntClear();
	}

	/*Set Flag of occurance of JPEG interupt. Both DMA and JPEG interrupts should come before the read call is wake up.*/
	RJpegFlagArrived = 1;
	if(RDmaFlagArrived)
	{
		RDmaFlagArrived = RJpegFlagArrived = 0;
		JPEG_WakeUp(JPEG_READ);
	}

	return IRQ_HANDLED;
}

/****************************************************************************/
/* NAME:  			SPEAR_JPEG_Init  				  						*/
/*                                                			    			*/  
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*        This is JPEG init routine.                                        */
/* PARAMETERS:                                                              */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: 						                 						    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
int __init SPEAR_JPEG_Init(void)
{
	t_JPEGError Status = JPEG_OK;
	int Ret;
	t_JPEGDevType DevType;

	//Registering Character Device.
	Ret = register_chrdev_region(JPEGDev.DevNum[JPEG_READ], DEV_TYPE_MAX, SPEAR_JPEG_NAME);
	if(Ret < 0)
	{
		printk("unable to register JPEG character device\n");
		return Ret;
	}

	/*Initializing device for Read and Write nodes of JPEG.*/
	for(DevType = JPEG_READ; DevType < DEV_TYPE_MAX; DevType += JPEG_WRITE)
	{
		JPEGDev.Cdev[DevType].owner = THIS_MODULE;
		cdev_init(&(JPEGDev.Cdev[DevType]), &SPEAR_JPEG_Fops[DevType]);
		Ret = cdev_add(&(JPEGDev.Cdev[DevType]), JPEGDev.DevNum[DevType], 1);
		if (Ret)
		{
			SPEAR_JPEG_Exit();
			return -EINVAL;
		}

		//Initialize wait queue heads.
		init_waitqueue_head(&JPEGDev.WaitQueue[DevType]);
	}

	/*register interrupt*/
	Ret = request_irq(IRQ_JPEG, (void *)SPEAR_JPEG_IntHandler, SA_INTERRUPT, SPEAR_JPEG_NAME, NULL);
	if(Ret)
	{
		printk("\n\nFailed to Install sbasic JPEG 0 Irq handler: %d\n\n", Ret);
	}

	return Status;
}


/****************************************************************************/
/* NAME:           SPEAR_JPEG_Exit					    */
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
void  SPEAR_JPEG_Exit(void)
{
	t_JPEGDevType DevType;

	//Unregistering Devices.
	for(DevType = JPEG_READ; DevType < DEV_TYPE_MAX; DevType += JPEG_WRITE)
	{
		cdev_del(&(JPEGDev.Cdev[DevType]));
	}

	/*Unregister Interrupt*/
	unregister_chrdev_region(JPEGDev.DevNum[JPEG_READ], DEV_TYPE_MAX);
}

//Module Init and Exit.
module_init(SPEAR_JPEG_Init);
module_exit(SPEAR_JPEG_Exit);
