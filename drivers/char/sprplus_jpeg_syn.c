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
#include <asm/arch/spr_major.h>
#include "spr_dma_pl080.h"
#include "sprplus_jpeg_syn.h"
#include "sprplus_jpeg_hdr_decode.h"

#define SPEAR_JPEG_NAME 	"spear_jpeg"
#define SPEAR_JPEG_MAJOR	MAJOR_JPEG

/*Timeout for sleep*/
#define JPEG_TIMEOUT 500

/*DMA Channel Id's for DMA Transfers*/
t_DMAChannelId MemToJpegChannelId, JpegToMemChannelId;

/*Flag to know JPEG Status*/
t_bool IsJpegFree = TRUE;
t_bool IsDecodingOver = FALSE;

/*Src, Dest address of image and Decoded image size*/
int DecodedImgSize = 0;
volatile uint *SrcImgAddr =0, *DecodedImgAddr = 0;

/*This is information about how many pages are allocated for Src and Dest buffers.*/
uint NumPagesReqSrc = 0, NumPagesReqDest = 0;

/*Creating Jpeg semaphores*/
static DECLARE_MUTEX(SemJpeg);

/*Creating Wait Queue for JPEG Decoding*/
wait_queue_head_t JpegWaitQueue;
int DmaFlagArrived, JpegFlagArrived;

#if SPEAR_JPEG_DEBUG
uint DataToJpeg, DataFromJpeg;

void IntHandlerToJpeg(t_bool IsTransferOver, t_DMAIntType IntType)
{
	SPEAR_JPEG_PRINT("DMA to JPEG is done %s %s\n",IsTransferOver?"TRUE":"FALSE", IntType?"INT_ERROR":"INT_TC");
}
#endif

/*This function is used to get order for allocating memory. No of pages = 2 ^ order*/
int FindNumOfPages(uint Size)
{
	uint NumOfPages = 0, PageSize = 4*1024;

	while(Size>(PageSize * (1<<NumOfPages)))
		NumOfPages++;

	return NumOfPages;
}

/****************************************************************************/
/* 			 JPEG_GotoSleep()	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine is called when JPEG device is required to wait for some */
/*      interval of time.                                                   */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*                                                                          */
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int JPEG_GotoSleep(void)
{
	//Goto Sleep
	wait_event_interruptible_timeout(JpegWaitQueue,((JpegFlagArrived & DmaFlagArrived)!= 0), JPEG_TIMEOUT);

	//if the flag arrived is 1, then it is normal wakeup.
	if((JpegFlagArrived & DmaFlagArrived) == 1)
	{
		JpegFlagArrived = DmaFlagArrived = 0;
	}
	//if the flag arrived is 0, then it is not normal wakeup.
	else
	{
		JpegFlagArrived = DmaFlagArrived = 0;
		return JPEG_SLEEP_ERROR;
	}

	return JPEG_OK;
}

/****************************************************************************/
/* 			 JPEG_WakeUp()	         	                    				*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine is called to wakeup from sleep.					    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*                                                                          */
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void JPEG_WakeUp(void)
{
	//Wake Up from Sleep 
	wake_up_interruptible(&JpegWaitQueue);
}

/****************************************************************************/
/* 			 IntHandlerFromJpeg()	                      				   */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This is interrupt handler of DMA to JPEG Transfer.                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
void IntHandlerFromJpeg(t_bool IsTransferOver, t_DMAIntType IntType)
{
	IsDecodingOver = TRUE;
	DmaFlagArrived = 1;
	JPEG_WakeUp();
	SPEAR_JPEG_PRINT("This is DMA Interrupt Handler Function. IsTransferOver is %s. Int Type is %s\n",IsTransferOver?"TRUE":"FALSE", IntType?"INT_ERROR":"INT_TC");
}

/****************************************************************************/
/* NAME:           JPEG_Flush											    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      Theis routine is used to flush JPEG status for new decodings.    	*/
/*                                                                          */
/****************************************************************************/
static void JPEG_Flush(void)
{
	/*Reset all global variables after decoding is done*/
	if(DecodedImgAddr != NULL)
		free_pages((uint)DecodedImgAddr, NumPagesReqDest);
	if(SrcImgAddr != NULL)
		free_pages((uint)SrcImgAddr, NumPagesReqSrc);

	SrcImgAddr = 0;
	DecodedImgAddr = 0;
	DecodedImgSize = 0;
	IsDecodingOver = FALSE;
}

/****************************************************************************/
/* NAME:           JPEG_Abort											    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      Theis routine is used to Abort JPEG Decoding Process             	*/
/*                                                                          */
/****************************************************************************/
void JPEG_Abort(void)
{
	uint DataCanBeLost = TRUE;
	SPEAr_DMA_AbortTransfer(MemToJpegChannelId, DataCanBeLost);
	SPEAr_DMA_AbortTransfer(JpegToMemChannelId, DataCanBeLost);
	JPEG_Disable();
	JPEG_Flush();
}

/****************************************************************************/
/* NAME:           WriteImgToJpeg and ReadImgFromJpeg					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      These routines are for trnasferring data TO and FROM JPEG. 		    */
/*                                                                          */
/****************************************************************************/
static t_DMAError WriteImgToJpeg(size_t SrcPacketSize)
{
	t_DMATransferDesc TransferDesc;
	t_DMAError Status;

	/*Transfer SrcImg to JPEG*/
	TransferDesc.SrcDeviceId = MEMORY_DMA_DEVICE;
	TransferDesc.DestDeviceId = JPEG_IN_DMA_DEVICE;
	TransferDesc.SrcAddress = (uint *)SrcImgAddr;
	TransferDesc.DestAddress = (uint *)SPEAR_JPEG_FIFO_IN;
	TransferDesc.TransferSize = SrcPacketSize;
	TransferDesc.FlowController = DMA_MEMORY_TO_PERIPHERAL;
	TransferDesc.Priority = UNDEFINED_PRIORITY;
	TransferDesc.IsIntReqAfterEachPacketTransfer = FALSE;
	TransferDesc.HandlerPtr = NULL;
#if SPEAR_JPEG_DEBUG
	TransferDesc.HandlerPtr = IntHandlerToJpeg;
#endif

	Status = SPEAr_DMA_Transfer(TransferDesc, &MemToJpegChannelId);
	SPEAR_JPEG_PRINT("\nStatus for 2nd Transfer is %d and channel is %d\n", Status, MemToJpegChannelId);
	return Status;
}

static t_DMAError ReadImgFromJpeg(void)
{
	t_DMATransferDesc TransferDesc;
	t_DMAError Status;

	/*Receive DecodedImg from JPEG*/
	TransferDesc.SrcDeviceId = JPEG_OUT_DMA_DEVICE;
	TransferDesc.DestDeviceId = MEMORY_DMA_DEVICE;
	TransferDesc.SrcAddress = (uint *)SPEAR_JPEG_FIFO_OUT;
	TransferDesc.DestAddress = (uint *)DecodedImgAddr;
	TransferDesc.FlowController = SRC_PERIPHERAL_TO_MEMORY;
	TransferDesc.Priority = UNDEFINED_PRIORITY;
	TransferDesc.IsIntReqAfterEachPacketTransfer = FALSE;
	TransferDesc.HandlerPtr = IntHandlerFromJpeg;

	Status = SPEAr_DMA_Transfer(TransferDesc, &JpegToMemChannelId);
	SPEAR_JPEG_PRINT("\nStatus for 1st Transfer is %d and channel is %d\n", Status, JpegToMemChannelId);
	return Status;
}

/****************************************************************************/
/* NAME:           SPEAR_JPEG_Open					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine opens the char device for jpeg          		    */
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
	/*Take Jpeg Critical Section Lock*/
	if(down_interruptible(&SemJpeg))
	{
		printk("JPEG: Not able to take semaphore. \n");
		return -JPEG_SEM_ERROR;
	}
	else
	{
		/*Check if JPEG not used by any other user*/
		if(IsJpegFree == TRUE)
		{
			/*Mark JPEG as Busy.*/
			IsJpegFree = FALSE;

			up(&SemJpeg);
			return JPEG_OK;
		}
		up(&SemJpeg);
	}

	return -1;
}

/****************************************************************************/
/* NAME:           SPEAR_JPEG_Release					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine releases the char device for jpeg       		    */
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
	/*Take Jpeg Critical Section Lock*/
	if (down_interruptible(&SemJpeg))
	{
		printk("JPEG: Not able to take semaphore.\n");
		return -JPEG_SEM_ERROR;
	}
	else
	{
		/*Free JPEG.*/
		IsJpegFree = TRUE;
		up(&SemJpeg);
	}

	JPEG_Flush();
	return JPEG_OK;
}

#if 0
/****************************************************************************************/
/* NAME:           SPEAR_JPEG_Read - This API is Not used currently.					*/
/*--------------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             		*/
/*      This routine implements the file Read operation of the Char Device JPEG		*/
/*                                                                          		*/
/* PARAMETERS:                                                              		*/
/* IN  :                                                                		*/
/*         file: Char device file name                                         		*/
/*         len: Number of units to be Read                                 		*/
/* OUT :                                                                    		*/
/*         buffer: Data Read                                         			*/
/*         ppos: Number of units Read                                      		*/
/*                                                                          		*/
/* RETURN:                                                                  		*/
/*       int	                                                            		*/
/*--------------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           		*/
/****************************************************************************************/
static int SPEAR_JPEG_Read(struct file *file, char __user *buffer, size_t len, loff_t *ppos)
{
	int DataToRead;
	static int ReadCount = 0;
	int i;
	uint *DataAddr;
	uint NumByteNotCopied = 0;

	/*only for first read.*/
	if(!ReadCount)
	{
		if(JPEG_GotoSleep() == JPEG_SLEEP_ERROR)
		{
			printk("JPEG: Not able to sleep properly.\n");
			JPEG_Abort();
			SPEAR_JPEG_PRINT("\nDecoding is over\n");
			return -JPEG_SLEEP_ERROR;
		}

	}

	/*DataToRead is smaller of len or (DecodedImgSize-ReadCount)*/
	DataToRead = len>(DecodedImgSize-ReadCount)?(DecodedImgSize-ReadCount):len;

#if SPEAR_JPEG_DEBUG
	printk("DataToJpeg is %d, DataFromJpeg is %d\n", DataToJpeg, DataFromJpeg);
#endif

	/*Changing Endianness of Output Data.*/
	for(i=0;i<DataToRead/4;i++)
	{
		DataAddr = (uint *)((char *)DecodedImgAddr+ReadCount) + i;
		*DataAddr = ((*DataAddr)>>24) | (((*DataAddr)&0x00FF0000)>>8) | (((*DataAddr)&0x0000FF00)<<8) | (((*DataAddr)&0x000000FF)<<24);
	}


	NumByteNotCopied = copy_to_user(buffer, (const char *)((char *)DecodedImgAddr+ReadCount), DataToRead);

	ReadCount += DataToRead;

	if(ReadCount == DecodedImgSize)
	{
		JPEG_Flush();
		ReadCount = 0;
	}

	return DataToRead;
}
#endif

/****************************************************************************************/
/* NAME:           SPEAR_JPEG_Write					    		*/
/*--------------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             		*/
/*      This routine implements the file write operation of the Char Device JPEG	*/
/*                                                                          		*/
/* PARAMETERS:                                                              		*/
/* IN  :                                                                		*/
/*         file: Char device file name                                         		*/
/*         buffer: Data to be written                                         		*/
/*         len: Number of units to be written, data is written in packets of size*/
/*                 4K, i.e. 4096 bytes. Size of last packet will be smaller.		*/
/* OUT :                                                                    		*/
/*         ppos: Number of units written                                      		*/
/*                                                                          		*/
/* RETURN:                                                                  		*/
/*       int	                                                            		*/
/*--------------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           		*/
/****************************************************************************************/
static int SPEAR_JPEG_Write(struct file *file, const char __user *buffer, size_t len, loff_t *ppos)
{
	int NumLli = 0;
	uint NumByteNotCopied = 0;

	/*Free all the resources used by JPEG in the last decoding*/
	JPEG_Flush();

	/*Allocate memory for copying Src Image*/
	NumPagesReqSrc = FindNumOfPages(len);
	if((NumPagesReqSrc > 10) || (!(SrcImgAddr = (uint *)__get_free_pages(GFP_DMA, NumPagesReqSrc))))
	{
		printk("JPEG: Not Able to allocate enough memory\n");
		return -JPEG_NO_MEM_AVAILABLE;
	}

	/*copy src image to kernel buffer.*/
	NumByteNotCopied = copy_from_user((void *)SrcImgAddr,(const char *)buffer, len);

	/*Write Image packet to JPEG.*/
	if(WriteImgToJpeg(len) != 0)
	{
		JPEG_Flush();
		return -JPEG_DMA_ERROR;
	}


	/*Find number of Llis required for transferring data to JPEG.*/
	NumLli = SPEAr_DMA_FindNumOfLli(MEMORY_DMA_DEVICE, JPEG_IN_DMA_DEVICE, len);

	/*Calculate Size of the output decoded image*/
	DecodedImgSize = FindDecodedImgSize((const char *)SrcImgAddr, len);
	if(!DecodedImgSize)
	{
		printk("Decoded Img Size is zero\n");
		//free_pages((uint)SrcImgAddr, NumPagesReqSrc);
		return -JPEG_NO_MEM_AVAILABLE;
	}

	/*Allocate memory for the decoded image.*/
	NumPagesReqDest = FindNumOfPages(DecodedImgSize);

	if((NumPagesReqDest > 11) || (!(DecodedImgAddr = (uint *)__get_free_pages(GFP_DMA, NumPagesReqDest))))
	{
		printk("JPEG: Not Able to allocate enough memory\n");
		JPEG_Flush();
		SPEAr_DMA_AbortTransfer(MemToJpegChannelId, TRUE);
		free_pages((uint)SrcImgAddr, NumPagesReqSrc);
		return -JPEG_NO_MEM_AVAILABLE;
	}

	/*Start transfer of decoded image to the allocated buffer.*/
	if(ReadImgFromJpeg() != 0)
	{
		JPEG_Flush();
		SPEAr_DMA_AbortTransfer(MemToJpegChannelId, TRUE);
		return -JPEG_DMA_ERROR;
	}


	/*This is to indicate end of decoding*/
	IsDecodingOver = FALSE;

	/*Start JPEG Decoding.*/
	JPEG_StartDecoder(NumLli, TRUE);

	return len;
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
	uint NumByteNotCopied = 0;
	switch (cmd)
	{
		case JPEGIOC_GET_DECODED_IMG_SIZE:
			NumByteNotCopied = copy_to_user((uint *)Buf, &DecodedImgSize, sizeof(uint));
			break;
		case JPEGIOC_FLUSH:
			JPEG_Flush();
			break;
		case JPEGIOC_GET_HDR:
			if(IsDecodingOver)
			{
				t_JPEGHdrInfo HdrInfo;
				JPEG_GetHdrInfo(&HdrInfo);
				NumByteNotCopied = copy_to_user((t_JPEGHdrInfo *)Buf, &HdrInfo, sizeof(t_JPEGHdrInfo));
			}
			else
				return -JPEG_DECODING_NOT_OVER;
			break;
		default:
			return (EINVAL);
			break;
	}
	return JPEG_OK;
}

/************************************************************************************/
/* NAME:           SPEAR_JPEG_Mmap					    							*/
/*----------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             		*/
/*      This routine implements the IOCTL operation of the Char Device JPEG. IOCTL  */
/*    is used to configure JPEG Device.                                       		*/
/*                                                                          		*/
/* PARAMETERS:                                                              		*/
/* IN  :                                            	                    		*/
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
static int SPEAR_JPEG_Mmap(struct file * file, struct vm_area_struct * vma)
{
	int i;
	volatile uint *DataAddr;
	size_t size = vma->vm_end - vma->vm_start;

	if(JPEG_GotoSleep() == JPEG_SLEEP_ERROR)
	{
		printk("JPEG: Not able to sleep properly.\n");
		JPEG_Abort();
		SPEAR_JPEG_PRINT("\nDecoding is over\n");
		return -JPEG_SLEEP_ERROR;
	}

#if SPEAR_JPEG_DEBUG
	printk("DataToJpeg is %d, DataFromJpeg is %d\n", DataToJpeg, DataFromJpeg);
#endif

	/*Changing Endianness of Output Data.*/
	for(i=0;i<DecodedImgSize/4;i++)
	{
		DataAddr = DecodedImgAddr + i;
		*DataAddr = ((*DataAddr)>>24) | (((*DataAddr)&0x00FF0000)>>8) | (((*DataAddr)&0x0000FF00)<<8) | (((*DataAddr)&0x000000FF)<<24);
	}

	/*Getting the page offset from the physical address of page*/
	vma->vm_pgoff = (__pa(DecodedImgAddr)) >> PAGE_SHIFT;

	/* Remap-pfn-range will mark the range VM_IO and VM_RESERVED */
	if (remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff, size, vma->vm_page_prot))
		return -EAGAIN;

	return 0;
}

static struct file_operations SPEAR_JPEG_Fops =
{
	.owner	= THIS_MODULE,
	.open   = SPEAR_JPEG_Open, 
	.release= SPEAR_JPEG_Release,
	/*	.read	= SPEAR_JPEG_Read,*/
	.write	= SPEAR_JPEG_Write,
	.ioctl	= SPEAR_JPEG_Ioctl,
	.mmap	= SPEAR_JPEG_Mmap
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
#if SPEAR_JPEG_DEBUG
	DataToJpeg = *JPEG_BYTE_FIFO_TO_CORE;
	DataFromJpeg = *JPEG_BYTE_CORE_TO_FIFO;
#endif
	JPEG_IntClear();
	JPEG_Disable();

	JpegFlagArrived = 1;
	JPEG_WakeUp();
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
static int __init SPEAR_JPEG_Init(void)
{
	int Ret;
	t_JPEGError Status = JPEG_OK;


	Ret = register_chrdev(SPEAR_JPEG_MAJOR, SPEAR_JPEG_NAME, &SPEAR_JPEG_Fops);
	if(Ret < 0)
	{
		printk(KERN_ERR SPEAR_JPEG_NAME ": unable to register character device JPEG\n");
		return Ret;
	}

	Ret = request_irq(IRQ_JPEG, (void *)SPEAR_JPEG_IntHandler, SA_INTERRUPT, SPEAR_JPEG_NAME, NULL);
	if(Ret)
	{
		printk("\n\nFailed to Install sbasic JPEG 0 Irq handler: %d\n\n", Ret);
	}

	//Initialize wait queue heads.
	init_waitqueue_head(&JpegWaitQueue);

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
static void __exit SPEAR_JPEG_Exit(void)
{
	unregister_chrdev(SPEAR_JPEG_MAJOR, SPEAR_JPEG_NAME);
}

//Module Init and Exit.
module_init(SPEAR_JPEG_Init);
module_exit(SPEAR_JPEG_Exit);
