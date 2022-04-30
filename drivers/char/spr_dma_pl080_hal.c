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
#include "spr_dma_pl080_hal.h"
#include <linux/dma-mapping.h>

/*------------------------------------------------------------------------
 * Global Variables 
 *----------------------------------------------------------------------*/

//Channel Base. Used for getting channel registers for the appropriate channel.
uint SPEAr_DMA_CHANNEL_BASE = SPEAr_DMA_CHANNEL0_BASE;

//This is the Free LLI list. Which contains the First Free LLI, Last Free LLI, Number of LLI's available.
t_DMALliListDesc LliFreeList;

//Offset between the physical and logical address of the LLI.
static uint Phy2LogOffset;

//Request Line Status Array.
t_DMARequestDesc DMARequestArray[REQUEST_LINE_MAX]=
{
	{DMA_SCH_O, 0},
	{DMA_SCH_O, 0},
	{DMA_SCH_O, 0},
	{DMA_SCH_O, 0},
	{DMA_SCH_O, 0},
	{DMA_SCH_O, 0},
	{DMA_SCH_O, 0},
	{DMA_SCH_O, 0},
	{DMA_SCH_O, 0},
	{DMA_SCH_O, 0},
	{DMA_SCH_O, 0},
	{DMA_SCH_O, 0},
	{DMA_SCH_O, 0},
	{DMA_SCH_O, 0},
	{DMA_SCH_O, 0},
	{DMA_SCH_O, 0}
};

//Channel Status Array.
t_DMAChannelDesc DMAChannelArray[CHANNEL_MAX]= 
{
	{
		CHANNEL0, NULL, 
		{0, NULL, NULL},
		0x00, 0x00, 0,
		LAST_DMA_DEVICE,
		LAST_DMA_DEVICE,
		FALSE
	},

	{
		CHANNEL1, NULL, 
		{0, NULL, NULL},
		0x00, 0x00, 0,
		LAST_DMA_DEVICE,
		LAST_DMA_DEVICE,
		FALSE
	},

	{
		CHANNEL2, NULL, 
		{0, NULL, NULL},
		0x00, 0x00, 0,
		LAST_DMA_DEVICE,
		LAST_DMA_DEVICE,
		FALSE
	},

	{
		CHANNEL3, NULL, 
		{0, NULL, NULL},
		0x00, 0x00, 0,
		LAST_DMA_DEVICE,
		LAST_DMA_DEVICE,
		FALSE
	},

	{
		CHANNEL4, NULL, 
		{0, NULL, NULL},
		0x00, 0x00, 0,
		LAST_DMA_DEVICE,
		LAST_DMA_DEVICE,
		FALSE
	},

	{
		CHANNEL5, NULL, 
		{0, NULL, NULL},
		0x00, 0x00, 0,
		LAST_DMA_DEVICE,
		LAST_DMA_DEVICE,
		FALSE
	},

	{
		CHANNEL6, NULL, 
		{0, NULL, NULL},
		0x00, 0x00, 0,
		LAST_DMA_DEVICE,
		LAST_DMA_DEVICE,
		FALSE
	},

	{
		CHANNEL7, NULL, 
		{0, NULL, NULL},
		0x00, 0x00, 0,
		LAST_DMA_DEVICE,
		LAST_DMA_DEVICE,
		FALSE
	}
};

//Device Configuration Array.
t_DMADeviceDesc DMADeviceArray[LAST_DMA_DEVICE] = 
{
	//Scheme 00
	MEMORY_DMA_DEVICE_DESCRIPTOR,
#ifdef CONFIG_ARCH_SPEARBASIC
	UART_0_IN_DMA_DEVICE_DESCRIPTOR,
	UART_0_OUT_DMA_DEVICE_DESCRIPTOR,
	SPI_0_IN_DMA_DEVICE_DESCRIPTOR,
	SPI_0_OUT_DMA_DEVICE_DESCRIPTOR,
	I2C_IN_DMA_DEVICE_DESCRIPTOR,
	I2C_OUT_DMA_DEVICE_DESCRIPTOR,
	IRDA_DMA_DEVICE_DESCRIPTOR,
	JPEG_IN_DMA_DEVICE_DESCRIPTOR,
	JPEG_OUT_DMA_DEVICE_DESCRIPTOR,
#endif
#ifdef CONFIG_ARCH_SPEARPLUS
	SPI_1_IN_DMA_DEVICE_DESCRIPTOR,
	SPI_1_OUT_DMA_DEVICE_DESCRIPTOR,
	UART_0_IN_DMA_DEVICE_DESCRIPTOR,
	UART_0_OUT_DMA_DEVICE_DESCRIPTOR,
	UART_1_IN_DMA_DEVICE_DESCRIPTOR,
	UART_1_OUT_DMA_DEVICE_DESCRIPTOR,
	SPI_2_IN_DMA_DEVICE_DESCRIPTOR,
	SPI_2_OUT_DMA_DEVICE_DESCRIPTOR,
	SPI_0_IN_DMA_DEVICE_DESCRIPTOR,
	SPI_0_OUT_DMA_DEVICE_DESCRIPTOR,
	I2C_IN_DMA_DEVICE_DESCRIPTOR,
	I2C_OUT_DMA_DEVICE_DESCRIPTOR,
	IRDA_DMA_DEVICE_DESCRIPTOR,
	JPEG_IN_DMA_DEVICE_DESCRIPTOR,
	JPEG_OUT_DMA_DEVICE_DESCRIPTOR,
#endif

	//Scheme 01
	RAS_0_IN_DMA_DEVICE_DESCRIPTOR,
	RAS_0_OUT_DMA_DEVICE_DESCRIPTOR,
	RAS_1_IN_DMA_DEVICE_DESCRIPTOR,
	RAS_1_OUT_DMA_DEVICE_DESCRIPTOR,
	RAS_2_IN_DMA_DEVICE_DESCRIPTOR,
	RAS_2_OUT_DMA_DEVICE_DESCRIPTOR,
	RAS_3_IN_DMA_DEVICE_DESCRIPTOR,
	RAS_3_OUT_DMA_DEVICE_DESCRIPTOR,
	RAS_4_IN_DMA_DEVICE_DESCRIPTOR,
	RAS_4_OUT_DMA_DEVICE_DESCRIPTOR,
	RAS_5_IN_DMA_DEVICE_DESCRIPTOR,
	RAS_5_OUT_DMA_DEVICE_DESCRIPTOR,
	RAS_6_IN_DMA_DEVICE_DESCRIPTOR,
	RAS_6_OUT_DMA_DEVICE_DESCRIPTOR,
	RAS_7_IN_DMA_DEVICE_DESCRIPTOR,
#ifdef CONFIG_ARCH_SPEARBASIC
	RAS_7_OUT_DMA_DEVICE_DESCRIPTOR
#elif defined(CONFIG_ARCH_SPEARPLUS)
	RAS_7_OUT_DMA_DEVICE_DESCRIPTOR,

	//Scheme 02
	EXT_0_IN_OUT_DMA_DEVICE_DESCRIPTOR,
	EXT_0_OUT_DMA_DEVICE_DESCRIPTOR,
	EXT_1_IN_OUT_DMA_DEVICE_DESCRIPTOR,
	EXT_1_OUT_DMA_DEVICE_DESCRIPTOR,
	EXT_2_IN_OUT_DMA_DEVICE_DESCRIPTOR,
	EXT_2_OUT_DMA_DEVICE_DESCRIPTOR,
	EXT_3_IN_OUT_DMA_DEVICE_DESCRIPTOR,
	EXT_3_OUT_DMA_DEVICE_DESCRIPTOR,
	EXT_4_IN_DMA_DEVICE_DESCRIPTOR,
	EXT_4_OUT_DMA_DEVICE_DESCRIPTOR,
	EXT_5_IN_DMA_DEVICE_DESCRIPTOR,
	EXT_5_OUT_DMA_DEVICE_DESCRIPTOR,
	EXT_6_IN_DMA_DEVICE_DESCRIPTOR,
	EXT_6_OUT_DMA_DEVICE_DESCRIPTOR,
	EXT_7_IN_DMA_DEVICE_DESCRIPTOR,
	EXT_7_OUT_DMA_DEVICE_DESCRIPTOR
#endif
};


/*------------------------------------------------------------------------
 * a global structure representing the entire DMA 
 *----------------------------------------------------------------------*/

/********************************************************************************/
/* NAME:           GetChannelMask						*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This function is used to get the channel mask according to the channel Id.	*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  : 	ChannelId: Channel Id of the channel			 	*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: unsigned int							    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
static uint GetChannelMask(t_DMAChannelId ChannelId)
{
	switch(ChannelId)
	{
		case 0: return CHANNEL0_MASK;
		case 1: return CHANNEL1_MASK;
		case 2: return CHANNEL2_MASK;
		case 3: return CHANNEL3_MASK;
		case 4: return CHANNEL4_MASK;
		case 5: return CHANNEL5_MASK;
		case 6: return CHANNEL6_MASK;
		case 7: return CHANNEL7_MASK;
		default:  return CHANNEL_NONE;
	}
}


/********************************************************************************/
/* NAME:           InitDMAChannels						*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This function is used to initialize all the channels.			*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  : 								    	*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: t_DMAError							    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
static t_DMAError InitDMAChannels(void)
{
	//Setting SPEAr_DMA_CHANNEL_BASE to channel 0 
	SPEAr_DMA_CHANNEL_BASE = SPEAr_DMA_CHANNEL0_BASE;

	//configure each channel of the controller selected by SPEAr_DMA_AHB_BASE.
	while(SPEAr_DMA_CHANNEL_BASE <= SPEAr_DMA_CHANNEL7_BASE)
	{
		//Initializing source and destination Addresses Channel registers with zero.
		*DMACCXSrcAddr = 0;
		*DMACCXDestAddr = 0;

		//Initializing Channel LLI register with AHB_MASTER1.
		*DMACCXLLI = 0;
		*DMACCXLLI = *DMACCXLLI & LM_AHB_MASTER1;

		//Initializing Channel Control Registers.
		*DMACCXControl = 0;
		*DMACCXControl = *DMACCXControl & INT_TC_LLI_DISABLE;
		*DMACCXControl = (*DMACCXControl & PROT_RESET) | USER_NOT_BUFFERABLE_NOT_CACHEABLE;
		*DMACCXControl = *DMACCXControl & DEST_INC_DISABLE & SRC_INC_DISABLE;
		*DMACCXControl = *DMACCXControl & DEST_MASTER1 & SRC_MASTER1;
		*DMACCXControl = (((*DMACCXControl & DEST_WIDTH_RESET) | DEST_WIDTH_BYTE) & SRC_WIDTH_RESET) | SRC_WIDTH_BYTE;
		*DMACCXControl = (((*DMACCXControl & DEST_BURST_RESET) | DEST_BURST_SIZE1) & SRC_BURST_RESET) | SRC_BURST_SIZE1;

		//Initializing Channel Configuration Registers.
		*DMACCXConfiguration = 0;
		*DMACCXConfiguration = *DMACCXConfiguration & TAKE_NEW_REQ & CHANNEL_UNLOCK;
		*DMACCXConfiguration = *DMACCXConfiguration & INT_TC_DISABLE & INT_ERROR_DISABLE;
		*DMACCXConfiguration = (*DMACCXConfiguration & FLOW_RESET) | DMA_FLOW_CTRL_MEM_TO_MEM;
		*DMACCXConfiguration = *DMACCXConfiguration & DEST_PERI_RESET & SRC_PERI_RESET & CHANNEL_DISABLE;

		//Incrementing SPEAr_DMA_CHANNEL_BASE by 0x00000020.
		SPEAr_DMA_CHANNEL_BASE += SPEAr_DMA_CHANNEL1_BASE;
	}
	return DMA_OK;
}


/****************************************************************************/
/* NAME:           DMA_LliInit                                              */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine forms a linked list of the memory passed as parameter  */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : LLIBufferAddr - Logical Base address of the buffer provided for LLi*/
/*			 Management                                         */
/*       PhyLli        - Physical Base address of the buffer provided for   */
/*			 Lli Management                                     */
/*	 LLIBufferSize - Size of the buffer                                 */
/*			 						    */
/* OUT : None                                                               */
/*									    */
/* RETURN: t_DMAError							    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static t_DMAError DMA_LliInit(uint *LLIBufferAddr, dma_addr_t PhyLli, uint LLIBufferSize)
{
	uint Index;
	t_DMALliDesc *LliDesc;

	//Maximum number of LLIs that can be made of the given buffer.
	LliFreeList.NumItems = LLIBufferSize / sizeof(t_DMALliDesc);

	//offset is logical minus physical address.
	//Phy2LogOffset = LLIBufferAddr - (__pa(LLIBufferAddr));
	Phy2LogOffset = (uint)LLIBufferAddr - PhyLli;

	//Initializing the base LLI
	LliDesc = (t_DMALliDesc *)LLIBufferAddr;

	//Initializing every LLI
	for(Index =0; Index < LliFreeList.NumItems; Index++)
	{
		//Setting Source and Destination Address.
		LliDesc[Index].SrcAddrReg = 0;
		LliDesc[Index].DestAddrReg = 0;

		//Setting Next LLI Register, Master as LM_AHB_MASTER1
		LliDesc[Index].NextLli = 0;
		LliDesc[Index].NextLli &= LM_AHB_MASTER1;

		//Setting pointer to Next LLI in the List. Adress is represented by bit [31:2] and bit [1:0] are zero.
		//As the link list should have next lli address as physical address of the LLI, so subtract offset from
		// logical address.
		LliDesc[Index].NextLli |= (((uint)(LliDesc + (Index + 1)) - Phy2LogOffset)>>2)<<2;
	}

	//The last item of the list should contain next address field as zero. And so it ends the linked list.
	LliDesc[Index-1].NextLli = 0;

	//Initializing the LLI Free List.
	LliFreeList.First = LliDesc;
	LliFreeList.Last = LliDesc + (Index-1);

	return DMA_OK;
}


/****************************************************************************/
/* NAME: 		RequestChannel                               	    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine allows to request a DMA channel with specified priority*/
/*      The channel returned will be allocated to user untill it is freed.  */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - Priority:  priority of the request (LOW/NORMAL/HIGH/UNKNOWN)	    */
/*                                                                          */
/* OUT :                                                                    */
/*      - ChannelId: Id of the allocated channel       			    */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_DMAError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static t_DMAError RequestChannel(t_DMATransferPriority Priority, t_DMAChannelId *ChannelId)
{
	t_DMAChannelId SearchBegin, SearchEnd, ChannelNum;
	int Increment;

	//Get the range of Channel whose availability is checked for alloting it for the current request.
	switch(Priority)
	{
		case LOW_PRIORITY:
			SearchBegin = CHANNEL7;
			SearchEnd = CHANNEL6;
			Increment = -1;
			break;
		case NORMAL_PRIORITY:
			SearchBegin = CHANNEL1;
			SearchEnd = CHANNEL7;
			Increment = 1;
			break;
		case HIGH_PRIORITY:
			SearchBegin = CHANNEL0;
			SearchEnd = CHANNEL3;
			Increment = 1;
			break;
		case UNDEFINED_PRIORITY:
			SearchBegin = CHANNEL0;
			SearchEnd = CHANNEL7;
			Increment = 1;
			break;
		default:
			return DMA_INVALID_PRIORITY;
	}

	//Check availability of a channel.
	//Scanning through channel hardware status and channel software status.
	for(ChannelNum = SearchBegin; ChannelNum != (SearchEnd + Increment); ChannelNum += Increment)
	{
		if((DMAChannelArray[ChannelNum].IsUsed==FALSE) && ((*DMACEnbldChns) & GetChannelMask(ChannelNum)) == FALSE)
		{
			//Found available channel.
			//Setting status of channel as USED in software view.
			DMAChannelArray[ChannelNum].IsUsed = TRUE;

			//Setting Channel Id for returning to the caller function.
			*ChannelId = ChannelNum;
			return DMA_OK;
		}
	}

	//If no channel is available
	return DMA_NO_MORE_HW_CHANNELS_AVAILABLE;
}


/****************************************************************************/
/* NAME: 		SetBaseChannel	                            	    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine sets the base channel for 	    			    */
/*      configuring registers                                               */
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
static t_DMAError SetBaseChannel(t_DMAChannelId ChannelId)
{
	//Setting Base Channel
	switch(ChannelId)
	{
		case CHANNEL0:
			SPEAr_DMA_CHANNEL_BASE = SPEAr_DMA_CHANNEL0_BASE;
			break;
		case CHANNEL1:
			SPEAr_DMA_CHANNEL_BASE = SPEAr_DMA_CHANNEL1_BASE;
			break;
		case CHANNEL2:
			SPEAr_DMA_CHANNEL_BASE = SPEAr_DMA_CHANNEL2_BASE;
			break;
		case CHANNEL3:
			SPEAr_DMA_CHANNEL_BASE = SPEAr_DMA_CHANNEL3_BASE;
			break;
		case CHANNEL4:
			SPEAr_DMA_CHANNEL_BASE = SPEAr_DMA_CHANNEL4_BASE;
			break;
		case CHANNEL5:
			SPEAr_DMA_CHANNEL_BASE = SPEAr_DMA_CHANNEL5_BASE;
			break;
		case CHANNEL6:
			SPEAr_DMA_CHANNEL_BASE = SPEAr_DMA_CHANNEL6_BASE;
			break;
		case CHANNEL7:
			SPEAr_DMA_CHANNEL_BASE = SPEAr_DMA_CHANNEL7_BASE;
			break;
		default:
			return DMA_INVALID_CHANNEL_ID;
			break;
	}
	return DMA_OK;
}

/****************************************************************************/
/* NAME: 		ConfigChannelControlReg                  	    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine configures the control register of a particular channel*/
/*      for a particular controller.					    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - SrcDeviceId:     identify the Source DMA Device              	    */
/*      - DestDeviceId:    identify the Destination DMA Device              */
/*      - IsIntReqAfterEachPacketTransfer:                                  */
/*                         interrupt is required after transfer of each LLI.*/
/*                                                                          */
/* OUT :  none                                                              */
/*                                                                          */
/*      - ControlReg:         Configured control register                   */
/* RETURN:                                                                  */
/*        t_DMAError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static t_DMAError ConfigChannelControlReg(t_DMADeviceId SrcDeviceId, t_DMADeviceId DestDeviceId, t_bool IsIntReqAfterEachPacketTransfer, uint *ControlReg)
{
	//Reset Control Register
	*ControlReg = 0;

	//Setting the control register for the LLI List.
	if(IsIntReqAfterEachPacketTransfer == TRUE) //Setting Interrupt after every LLI bit of the channel.
	{
		*ControlReg |= INT_TC_LLI_ENABLE;
	}
	else if(IsIntReqAfterEachPacketTransfer !=FALSE)
	{
		return DMA_INVALID_PARAMETER;
	}

	*ControlReg |= DMADeviceArray[SrcDeviceId].Protection; //Setting Protection bit of channel.

	if(DestDeviceId == MEMORY_DMA_DEVICE) //Enabling destination address increment.
	{
		*ControlReg |= DEST_INC_ENABLE;
	}

	if(SrcDeviceId == MEMORY_DMA_DEVICE) //Enabling source address increment.
	{
		*ControlReg |= SRC_INC_ENABLE; 	
	}

	if(DMADeviceArray[DestDeviceId].AccessDesc.WhichMaster == AHB1_MASTER) //Selecting Master for destination.
	{
		*ControlReg |= DEST_MASTER2;
	}
	else
	{
		*ControlReg &= DEST_MASTER1;
	}

	if(DMADeviceArray[SrcDeviceId].AccessDesc.WhichMaster == AHB1_MASTER) //Selecting Master for Source.
	{
		*ControlReg |= SRC_MASTER2;
	}
	else
	{
		*ControlReg &= SRC_MASTER1;
	}

	switch(DMADeviceArray[DestDeviceId].AccessWidth) //Selecting Access width of destination.
	{
		case BYTE_WIDTH:
			*ControlReg |= DEST_WIDTH_BYTE;
			break;
		case HALFWORD_WIDTH:
			*ControlReg |= DEST_WIDTH_HALFWORD;
			break;
		case WORD_WIDTH:
			*ControlReg |= DEST_WIDTH_WORD;
			break;
	}

	switch(DMADeviceArray[SrcDeviceId].AccessWidth) //Selecting Access width of source.
	{
		case BYTE_WIDTH:
			*ControlReg |= SRC_WIDTH_BYTE;
			break;
		case HALFWORD_WIDTH:
			*ControlReg |= SRC_WIDTH_HALFWORD;
			break;
		case WORD_WIDTH:
			*ControlReg |= SRC_WIDTH_WORD;
			break;
	}


	switch(DMADeviceArray[DestDeviceId].MaxBurstSize) //Selecting Burst Size of Destination.
	{
		case BURST_SIZE_1:
			*ControlReg |= DEST_BURST_SIZE1;
			break;
		case BURST_SIZE_4:
			*ControlReg |= DEST_BURST_SIZE4;
			break;
		case BURST_SIZE_8:
			*ControlReg |= DEST_BURST_SIZE8;
			break;
		case BURST_SIZE_16:
			*ControlReg |= DEST_BURST_SIZE16;
			break;
		case BURST_SIZE_32:
			*ControlReg |= DEST_BURST_SIZE32;
			break;
		case BURST_SIZE_64:
			*ControlReg |= DEST_BURST_SIZE64;
			break;
		case BURST_SIZE_128:
			*ControlReg |= DEST_BURST_SIZE128;
			break;
		case BURST_SIZE_256:
			*ControlReg |= DEST_BURST_SIZE256;
			break;
	}

	switch(DMADeviceArray[SrcDeviceId].MaxBurstSize) //Selecting Burst Size of Source.
	{
		case BURST_SIZE_1:
			*ControlReg |= SRC_BURST_SIZE1;
			break;
		case BURST_SIZE_4:
			*ControlReg |= SRC_BURST_SIZE4;
			break;
		case BURST_SIZE_8:
			*ControlReg |= SRC_BURST_SIZE8;
			break;
		case BURST_SIZE_16:
			*ControlReg |= SRC_BURST_SIZE16;
			break;
		case BURST_SIZE_32:
			*ControlReg |= SRC_BURST_SIZE32;
			break;
		case BURST_SIZE_64:
			*ControlReg |= SRC_BURST_SIZE64;
			break;
		case BURST_SIZE_128:
			*ControlReg |= SRC_BURST_SIZE128;
			break;
		case BURST_SIZE_256:
			*ControlReg |= SRC_BURST_SIZE256;
			break;
	}
	return DMA_OK;
}

/****************************************************************************/
/* NAME: 		DMA_FindNumOfLli	                            */
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
int DMA_FindNumOfLli(t_DMADeviceId SrcDeviceId, t_DMADeviceId DestDeviceId, uint TransferSize)
{
	int DMATransferSizeMax;
	uint NumLli = 0;

	if(!TransferSize)
		return 0;
	
	/*Calculate the max transfer size supported by Src Device for a single LLI.*/
	DMATransferSizeMax = DMA_MAX_TRANSFER_SIZE * DMADeviceArray[SrcDeviceId].AccessWidth;

	/*Calculate the max transfer size supported by Dest Device for a single LLI. which is final transfer size.*/
	DMATransferSizeMax -= DMATransferSizeMax % DMADeviceArray[DestDeviceId].AccessWidth;

	/*Calculate the max number of LLIs required for Src to Dest transfer.*/
	NumLli	= TransferSize/DMATransferSizeMax + ((TransferSize % DMATransferSizeMax)?1:0);

	/*If Data Less than the source access width is left at the end.*/
	switch(TransferSize % DMADeviceArray[SrcDeviceId].AccessWidth)
	{
		case 1:
			if((TransferSize % DMATransferSizeMax) > 3)
			{
				NumLli++;
			}
			break;
		case 2:
			if((TransferSize % DMATransferSizeMax) > 3)
			{
				NumLli++;
			}
			break;
		case 3:
			if((TransferSize % DMATransferSizeMax) > 3)
			{
				NumLli++;
			}
			break;
	}
	return (NumLli-1);
}

/****************************************************************************/
/* NAME: 		PrepareLli	                            	    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Prepares a LLI List for the current transfer.	    */
/*      This routine should be called only for DMA flow controller mode.    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - SrcDeviceId:     identify the Source DMA Device              	    */
/*      - DestDeviceId:    identify the Destination DMA Device              */
/*      - SrcAddress:      identify the Source Address of transfer    	    */
/*      - DestAddress:     identify the Destination Address of transfer     */
/*      - TransferSize:    identify the amount of data to be transferred    */
/*      - IsIntReqAfterEachPacketTransfer:                                  */
/*                         interrupt is required after transfer of each LLI.*/
/*                                                                          */
/* OUT :  none                                                              */
/*                                                                          */
/*      - LliList:         prepared list of Lli                             */
/* RETURN:                                                                  */
/*        t_DMAError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static t_DMAError PrepareLli(t_DMADeviceId SrcDeviceId, t_DMADeviceId DestDeviceId, uint *SrcAddress, uint *DestAddress, uint TransferSize, t_DMALliListDesc *LliList, t_bool IsIntReqAfterEachPacketTransfer)
{
	t_bool IsExtraLliReq = FALSE;
	t_DMAError Status=0;
	int TransferSizeLeft, DMATransferSizeMax, Index;
	uint *SrcAddressTemp =0, *DestAddressTemp =0, NumLli = 0, IndexLli = 0, ExtraDataLeft = 0, ControlReg = 0;
	t_DMALliDesc *CurrLliDesc = NULL, *PrevLliDesc = NULL, *SecondLastLliDesc = NULL;

	//Check if Transfer size is zero.
	if(!TransferSize)
	{
		return DMA_INCOMPATIBLE_TRANSFER_SIZE;
	}

	//Initialize temporary variables for further use.
	SrcAddressTemp = SrcAddress, DestAddressTemp = DestAddress, TransferSizeLeft = TransferSize;

	//Calculate the max transfer size supported by Src Device for a single LLI.
	DMATransferSizeMax = DMA_MAX_TRANSFER_SIZE * DMADeviceArray[SrcDeviceId].AccessWidth;

	//Calculate the max transfer size supported by Dest Device for a single LLI. which is final transfer size.
	DMATransferSizeMax -= DMATransferSizeMax % DMADeviceArray[DestDeviceId].AccessWidth;

	//Calculate the max number of LLIs required for Src to Dest transfer.
	NumLli	= TransferSize/DMATransferSizeMax + ((TransferSize % DMATransferSizeMax)?1:0);

	//If Data Less than the source access width is left at the end.
	switch(TransferSize % DMADeviceArray[SrcDeviceId].AccessWidth)
	{
		case 1:
			ExtraDataLeft = 1;
			IsExtraLliReq = TRUE;

			//After the transfer of all DMATransferSizeMax sized data packets, if data left is more than 3 bytes than we require an extra LLI for transferring the last bytes, which doesn't match the access width of the Source Device, with a source width of 1 Byte.
			if((TransferSize % DMATransferSizeMax) > 3)
			{
				NumLli++;
			}
			break;
		case 2:
			ExtraDataLeft = 2;
			IsExtraLliReq = TRUE;
			if((TransferSize % DMATransferSizeMax) > 3)
			{
				NumLli++;
			}
			break;
		case 3:
			ExtraDataLeft = 3;
			IsExtraLliReq = TRUE;
			if((TransferSize % DMATransferSizeMax) > 3)
			{
				NumLli++;
			}
			break;
	}
	//Check if NumLli number of LLIs are available in the free lli list.
	if(NumLli > LliFreeList.NumItems)
	{
		return DMA_NO_MORE_LLIS_AVAILABLE;
	}

	//Update the free LLI List.
	LliList->First = LliFreeList.First;

	//Setting the first lli as the last lli if only one lli is required.
	if(NumLli == 1)
	{
		LliList->Last = LliList->First;
	}

	//Remaining items in the free list.
	LliFreeList.NumItems -= NumLli;

	//Getting the logical address of the lli NumLli ahead of the First LLI in the list.
	for(Index = 0; Index < NumLli; Index++)
	{
		LliFreeList.First = (t_DMALliDesc *)( ( ( (LliFreeList.First -> NextLli) >>2)<<2 ) + Phy2LogOffset);

		//If the loop is on the second last LLI, set the last LLI of the List.
		if(Index == NumLli-2)
		{
			LliList->Last = LliFreeList.First;
		}
	}

	//Return the newly allocated LLI List.
	LliList->NumItems = NumLli;

	//Setting the control register for the LLI List.
	Status = ConfigChannelControlReg(SrcDeviceId, DestDeviceId, IsIntReqAfterEachPacketTransfer, &ControlReg);
	if(Status)
	{
		return Status;
	}

	//Setting Transfer size.
	ControlReg |= DMATransferSizeMax/DMADeviceArray[SrcDeviceId].AccessWidth;

	//Setting the current lli to point to the first LLI in free list.
	CurrLliDesc = LliList->First;
	PrevLliDesc = CurrLliDesc;

	//configure each lli to be allocated for current transfer.
	while(IndexLli < NumLli)
	{
		//Track the Second Last LLI.
		SecondLastLliDesc = PrevLliDesc;

		//Track the previous LLI.
		PrevLliDesc = CurrLliDesc;

		//Set current LLI to Point to next LLI in list.
		CurrLliDesc = (t_DMALliDesc *)((((CurrLliDesc->NextLli)>>2)<<2) + Phy2LogOffset);
		IndexLli++;

		//Setting the source and destination address of last LLI.
		PrevLliDesc->SrcAddrReg = SrcAddressTemp;
		PrevLliDesc->DestAddrReg = DestAddressTemp;

		//Setting Control Register for every LLI.
		PrevLliDesc->ChannelControlReg = ControlReg;

		//Transfer size left
		TransferSizeLeft -= DMATransferSizeMax;

		//Checking if source and destination address has to be incremented for the present LLI.
		if(DestDeviceId == MEMORY_DMA_DEVICE)
		{
			DestAddressTemp = (uint *)((uint)DestAddressTemp + DMATransferSizeMax);
		}
		if(SrcDeviceId == MEMORY_DMA_DEVICE)
		{
			SrcAddressTemp = (uint *)((uint)SrcAddressTemp + DMATransferSizeMax);
		}
		SPEAR_DMA_PRINT("SrcAdd %x, DestAdd %x, Control %x, Next %x, Me %x\n", PrevLliDesc->SrcAddrReg, PrevLliDesc->DestAddrReg, PrevLliDesc->ChannelControlReg, PrevLliDesc->NextLli, PrevLliDesc);
	}

	if(PrevLliDesc == NULL)
	{
		return DMA_ERROR;
	}

	//if transfer size left is greater than max transfer size after end of available LLIs.
	if(TransferSizeLeft > DMATransferSizeMax)
	{
		return DMA_ERROR;
	}

	//If an Extra LLI is required and there is some more data which can be transfered at normal Access Width.
	if(IsExtraLliReq && ((TransferSize % DMATransferSizeMax) > 3))
	{
		//Set transfer size equal to the transfer size left for the last two LLIs.
		TransferSizeLeft += 2*DMATransferSizeMax;

		//Reset transfer size of Second last LLI.
		SecondLastLliDesc->ChannelControlReg &= TRANSFER_SIZE_RESET;

		//Reset transfer size of last LLI.
		PrevLliDesc->ChannelControlReg &= TRANSFER_SIZE_RESET;

		//Set transfer size of Second last LLI
		SecondLastLliDesc->ChannelControlReg |= ((TransferSizeLeft-ExtraDataLeft)/DMADeviceArray[SrcDeviceId].AccessWidth);

		//Set Transfer size of last LLI
		PrevLliDesc->ChannelControlReg |= ExtraDataLeft; 

		//Set Src and Dest address of last LLI.
		if(DestDeviceId == MEMORY_DMA_DEVICE)
		{
			PrevLliDesc->DestAddrReg = (uint *)((uint)SecondLastLliDesc->DestAddrReg + (TransferSizeLeft - ExtraDataLeft));
		}
		if(SrcDeviceId == MEMORY_DMA_DEVICE)
		{
			PrevLliDesc->SrcAddrReg = (uint *)((uint)SecondLastLliDesc->SrcAddrReg + (TransferSizeLeft - ExtraDataLeft));
		}

		//Set Access Width for Last LLI as Byte Wide. So that 1, 2, 3 byte data can be transferred.
		PrevLliDesc->ChannelControlReg &= SRC_WIDTH_RESET;
		PrevLliDesc->ChannelControlReg |= SRC_WIDTH_BYTE;
		PrevLliDesc->ChannelControlReg &= DEST_WIDTH_RESET;
		PrevLliDesc->ChannelControlReg |= DEST_WIDTH_BYTE;
		SPEAR_DMA_PRINT("SrcAdd %x, DestAdd %x, Control %x, Next %x, Me %x\n", SecondLastLliDesc->SrcAddrReg, SecondLastLliDesc->DestAddrReg, SecondLastLliDesc->ChannelControlReg, SecondLastLliDesc->NextLli, SecondLastLliDesc);
	}
	//If an Extra LLI is required for transferring data less than 3 bytes.
	else if(IsExtraLliReq && ((TransferSize % DMATransferSizeMax) <= 3))
	{
		//Reset transfer size of last LLI.
		PrevLliDesc->ChannelControlReg &= TRANSFER_SIZE_RESET;

		//Set Transfer size of last LLI equal to data left.
		PrevLliDesc->ChannelControlReg |= ExtraDataLeft; 

		//Set Access Width for Last LLI as Byte Wide. So that 1, 2, 3 byte data can be transferred.
		PrevLliDesc->ChannelControlReg &= SRC_WIDTH_RESET;
		PrevLliDesc->ChannelControlReg |= SRC_WIDTH_BYTE;
		PrevLliDesc->ChannelControlReg &= DEST_WIDTH_RESET;
		PrevLliDesc->ChannelControlReg |= DEST_WIDTH_BYTE;
	}
	//If an extra LLI is not required OR the Transfer Size is divisible by Src Access width. 
	else if(TransferSizeLeft != 0)
	{
		//Set transfer size equal to the transfer size left for the last two LLIs.
		TransferSizeLeft += DMATransferSizeMax;
		//Reset transfer size of last LLI.
		PrevLliDesc->ChannelControlReg &= TRANSFER_SIZE_RESET;

		//configure transfer size of last LLI.
		PrevLliDesc->ChannelControlReg |= (TransferSizeLeft/DMADeviceArray[SrcDeviceId].AccessWidth);
	}

	//Set the nexlli address of last LLI to zero to indicate end of data transfer.
	PrevLliDesc->NextLli = 0;

	//To generate interrupt at the end of last LLI. That is at the end of transfer.
	PrevLliDesc->ChannelControlReg |= INT_TC_LLI_ENABLE;

	SPEAR_DMA_PRINT("SrcAdd %x, DestAdd %x, Control %x, Next %x, Me %x\n", PrevLliDesc->SrcAddrReg, PrevLliDesc->DestAddrReg, PrevLliDesc->ChannelControlReg, PrevLliDesc->NextLli, PrevLliDesc);

	//Configuring the channel registers for first DMA transfer.
	*DMACCXSrcAddr = (uint)(LliList->First->SrcAddrReg);
	*DMACCXDestAddr= (uint)(LliList->First->DestAddrReg);

	*DMACCXLLI = LliList->First->NextLli;
	*DMACCXControl = LliList->First->ChannelControlReg;

	return DMA_OK;
}

/****************************************************************************/
/* NAME: 		FreeLli		                            	    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine frees the LLI List occupied by a DMA channel.	    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - ChannelId:	   identify the Channel				    */
/*                                                                          */
/* OUT :  none                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_DMAError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static t_DMAError FreeLli(t_DMAChannelId ChannelId)
{
	//Update the free LLI List.
	LliFreeList.NumItems += DMAChannelArray[ChannelId].LliListDesc.NumItems;

	SPEAR_DMA_PRINT("New Free Lli First is %x, last is %x\n",(uint)DMAChannelArray[ChannelId].LliListDesc.First, DMAChannelArray[ChannelId].LliListDesc.Last);

	LliFreeList.Last->NextLli = (uint)DMAChannelArray[ChannelId].LliListDesc.First - Phy2LogOffset;
	LliFreeList.Last = DMAChannelArray[ChannelId].LliListDesc.Last;

	//Update Channel Descriptor LLI List.
	DMAChannelArray[ChannelId].LliListDesc.NumItems = 0;
	DMAChannelArray[ChannelId].LliListDesc.First = NULL;
	DMAChannelArray[ChannelId].LliListDesc.Last = NULL;

	return DMA_OK;
}

/****************************************************************************/
/* NAME: 		DMA_IsTransferOver	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine tells the current status of an Channel previously 	    */
/*      configured for transfering.            				    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - ChannelId:     identify the DMA channel                     	    */
/*                                                                          */
/* OUT :  none                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_DMAError	                                                    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static t_DMAError DMA_IsTransferOver(t_DMAChannelId ChannelId, t_bool *Result)
{
	t_DMAChannelMask ChannelMask;

	//Checking that the DMA controller is Enabled or not.
	if(!(*DMACConfiguration & CTRL_ENABLE))
	{
		return DMA_CONTROLLER_NOT_INITIALIZED;
	}

	//Preparing Mask for Enabled Channel Register.
	ChannelMask = GetChannelMask(ChannelId);

	//If invalid channel Mask is returned
	if(ChannelMask == CHANNEL_NONE)
	{
		return DMA_INVALID_CHANNEL_ID;
	}

	//If Channel is currently Active.
	if(*DMACEnbldChns & ChannelMask)
	{
		*Result = FALSE;
	}
	//If Channel is currently InActive.
	else
	{
		*Result = TRUE;
	}

	return DMA_OK;
}


/********************************************************************************/
/* NAME:           DMA_GetIntSrc						*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This API is used to get the source of interrutp.			 	*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  :                                                     	                */
/*	- IntType:       identify the interrupt type: Terminal count or error  	*/
/*                                                                          	*/
/* OUT : 								    	*/
/*      - ChannelId:     identify the DMA channel                     		*/
/*      - IsTransferOver To check that interrupt is due to end of entire      	*/
/*			 exchange or transfer of a single LLI.			*/
/*                                                                          	*/
/* RETURN: t_DMAError							    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
t_DMAError DMA_GetIntSrc(t_DMAChannelId *ChannelId, t_DMAIntType *IntType, t_bool *IsTransferOver)
{
	t_DMAChannelMask ChannelMask;

	//Find the Type of Interrupt.
	if(*DMACIntTCStatus)
		*IntType = INT_TC;
	else if(*DMACIntErrorStatus)
		*IntType = INT_ERROR;

	//Checking that the DMA controller is Enabled or not.
	if(!(*DMACConfiguration & CTRL_ENABLE))
	{
		return DMA_CONTROLLER_NOT_INITIALIZED;
	}

	//Checking if Pointer to ChannelId or IsTransferOver are valid or NOT NULL.
	if((ChannelId == NULL)||(IsTransferOver == NULL))
	{
		return DMA_NULL_POINTER;
	}

	//Initializing ChannelId.
	*ChannelId = CHANNEL0;
	ChannelMask = CHANNEL0_MASK;

	SPEAR_DMA_PRINT("DMACIntTCStatus    %x\n", *DMACIntTCStatus);
	if(*IntType == INT_TC)
	{
		//Get Channel Id interrupts on Selected DMA channel.
		while(ChannelMask < CHANNEL_NONE)
		{
			if(*DMACIntTCStatus & ChannelMask)
			{
				//Checking that interrupt is due to end of entire exchange or of a single LLI.
				if(*DMACEnbldChns & ChannelMask)
				{
					*IsTransferOver = FALSE;
				}
				else
				{
					*IsTransferOver = TRUE;
				}
				//Return success if found an interrupt.
				return DMA_OK;
			}
			*ChannelId += 1;
			ChannelMask += ChannelMask;
		}
	}
	else if(*IntType == INT_ERROR)
	{
		//Get Channel Id interrupts on Selected DMA channel.
		while(ChannelMask < CHANNEL_NONE)
		{
			if(*DMACIntErrorStatus & ChannelMask)
			{
				*IsTransferOver = FALSE;
				return DMA_OK;
			}
			*ChannelId += 1;
			ChannelMask += ChannelMask;
		}
	}
	else
	{
		return DMA_ERROR;
	}

	//Report error if no interrupt is found.
	return DMA_NO_INT_ACTIVE;
}


/****************************************************************************/
/* NAME:           DMA_ReleaseRequestLine                                   */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Releases a Request Line				    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : SrcDeviceId							    */
/*	 DestDeviceId			                                    */
/*			 						    */
/* OUT : None                                                               */
/*									    */
/* RETURN: t_DMAError							    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_DMAError DMA_ReleaseRequestLine(t_DMADeviceId SrcDeviceId, t_DMADeviceId DestDeviceId)
{
	t_DMARequestId SrcRequestId, DestRequestId;
	t_MiscDMAScheme SrcConfigScheme, DestConfigScheme;

	//Getting Request Ids into local variables.
	SrcRequestId  = DMADeviceArray[SrcDeviceId].AccessDesc.RequestId;
	DestRequestId = DMADeviceArray[DestDeviceId].AccessDesc.RequestId;

	//Getting Configuration Schemes into local variables.
	SrcConfigScheme  = DMADeviceArray[SrcDeviceId].AccessDesc.ConfigScheme;
	DestConfigScheme = DMADeviceArray[DestDeviceId].AccessDesc.ConfigScheme;

	SPEAR_DMA_PRINT("In Release Src is %d,   Dest is %d  Scheme Src %d, Dest %d \n", SrcRequestId, DestRequestId, SrcConfigScheme, DestConfigScheme);
	//Release the Request Lines if Src or Dest Device is not MEMORY.
	if(SrcDeviceId != MEMORY_DMA_DEVICE)
	{
		//If the SrcDevice has acquired the Request Line before, then release it, else return error.
		if(DMARequestArray[SrcRequestId].CurrentConfigScheme == SrcConfigScheme)
		{
			DMARequestArray[SrcRequestId].RequestCount--;
		}
		else
			return DMA_INVALID_PARAMETER;
	}

	//Release the Request Lines if Src or Dest Device is not MEMORY.
	if(DestDeviceId != MEMORY_DMA_DEVICE)
	{
		//If the DestDevice has acquired the Request Line before, then release it, else return error.
		if(DMARequestArray[DestRequestId].CurrentConfigScheme == DestConfigScheme)
		{
			DMARequestArray[DestRequestId].RequestCount--;
		}
		else
			return DMA_INVALID_PARAMETER;
	}
	return DMA_OK;
}

/****************************************************************************/
/* NAME:           DMA_AcquireRequestLine                                   */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Acquires a Request Line				    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : SrcDeviceId							    */
/*	 DestDeviceId			                                    */
/*			 						    */
/* OUT : None                                                               */
/*									    */
/* RETURN: t_DMAError							    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_DMAError DMA_AcquireRequestLine(t_DMADeviceId SrcDeviceId, t_DMADeviceId DestDeviceId)
{
	t_MiscError Status;
	t_DMARequestId SrcRequestId, DestRequestId;
	t_MiscDMAScheme SrcConfigScheme, DestConfigScheme;

	//Getting Request Ids into local variables.
	SrcRequestId  = DMADeviceArray[SrcDeviceId].AccessDesc.RequestId;
	DestRequestId = DMADeviceArray[DestDeviceId].AccessDesc.RequestId;

	//If Both the Devices have same Request Id, then transfer is not possible.
	if(SrcRequestId == DestRequestId)
		if(SrcDeviceId != MEMORY_DMA_DEVICE)
			return DMA_TRANSFER_NOT_SUPPORTED;

	//Getting Configuration Schemes into local variables.
	SrcConfigScheme  = DMADeviceArray[SrcDeviceId].AccessDesc.ConfigScheme;
	DestConfigScheme = DMADeviceArray[DestDeviceId].AccessDesc.ConfigScheme;

	SPEAR_DMA_PRINT("In Acquire Src is %d,   Dest is %d  Scheme Src %d, Dest %d \n", SrcRequestId, DestRequestId, SrcConfigScheme, DestConfigScheme);
	//Acquire the Request Lines if Src or Dest Device is not MEMORY.
	if(SrcDeviceId != MEMORY_DMA_DEVICE)
	{
		SPEAR_DMA_PRINT("Config Src in Acquire\n");
		//If Line is already acquired by some other peripheral and has ongoing transfer.
		if((DMARequestArray[SrcRequestId].RequestCount != 0) && (DMARequestArray[SrcRequestId].CurrentConfigScheme != SrcConfigScheme))
			return DMA_BUSY;

		//If the Request Line is required to be configured.
		if(DMARequestArray[SrcRequestId].CurrentConfigScheme != SrcConfigScheme)
		{
			SPEAR_DMA_PRINT("Change Config Src in Acquire\n");
			//Configure the Request Line.
			Status = Misc_DMAConfigScheme(SrcConfigScheme, SrcRequestId);
			if(Status)
				return DMA_ERROR;

			//Change status in software.
			DMARequestArray[SrcRequestId].RequestCount =1;
			DMARequestArray[SrcRequestId].CurrentConfigScheme = SrcConfigScheme;
		}
		//If Request Line is not required to be configured.
		else
			DMARequestArray[SrcRequestId].RequestCount++;
	}
	if(DestDeviceId != MEMORY_DMA_DEVICE)
	{
		SPEAR_DMA_PRINT("Config Dest in Acquire\n");
		//If Line is already acquired by some other peripheral and has ongoing transfer.
		if((DMARequestArray[DestRequestId].RequestCount != 0) && (DMARequestArray[DestRequestId].CurrentConfigScheme != DestConfigScheme))
		{
			SPEAR_DMA_PRINT("Release dest in Acquire 1\n");
			DMA_ReleaseRequestLine(SrcDeviceId, MEMORY_DMA_DEVICE);
			return DMA_BUSY;
		}

		//If the Request Line is required to be configured.
		if(DMARequestArray[DestRequestId].CurrentConfigScheme != DestConfigScheme)
		{
			SPEAR_DMA_PRINT("Change Config Dest in Acquire\n");
			Status = Misc_DMAConfigScheme(DestConfigScheme, DestRequestId);
			if(Status)
			{
				SPEAR_DMA_PRINT("Release dest in Acquire\n");
				DMA_ReleaseRequestLine(SrcDeviceId, MEMORY_DMA_DEVICE);
				return DMA_ERROR;
			}

			//Change status in software.
			DMARequestArray[DestRequestId].RequestCount =1;
			DMARequestArray[DestRequestId].CurrentConfigScheme = DestConfigScheme;
		}
		//If Request Line is not required to be configured.
		else
			DMARequestArray[DestRequestId].RequestCount++;
	}
	return DMA_OK;
}


/****************************************************************************/
/* NAME:           DMA_Init                                                 */
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
t_DMAError DMA_Init(uint *LLIBufferAddr, dma_addr_t PhyLli, uint LLIBufferSize)
{
	t_DMAError Status = 0;

	//If Buffer Base Address is Invalid
	if(LLIBufferAddr == NULL)
	{
		return DMA_NULL_POINTER;
	}

	//If Buffer size is Zero, return error.
	if(LLIBufferSize == 0)
	{
		return DMA_INCOMPATIBLE_BUFFER_SIZE;
	}

	//If buffer memory is not word aligned.
	if((((uint)LLIBufferAddr>>2)<<2 != (uint)LLIBufferAddr))
	{
		return DMA_MISALLIGNED_ADDRESS;
	}

	//Channels are initialized of inidividual controllers.
	Status = InitDMAChannels();
	if(Status)
	{
		return Status;
	}
	else
	{
		//Interrupts are cleared: Terminal count and Error interrupts
		*DMACIntTCClear |= ~(0xffffffff<<CHANNEL_MAX);
		*DMACIntErrClear |= ~(0xffffffff<<CHANNEL_MAX);

		//Little Endian mode selected for both the masters of selected controller.
		*DMACConfiguration = *DMACConfiguration & AHB1_LITTLE_ENDIAN & AHB2_LITTLE_ENDIAN;

		//Controller is enabled.
		*DMACConfiguration |= CTRL_ENABLE;
	}


	//Linked list is made from the buffer passed as argument.
	Status = DMA_LliInit(LLIBufferAddr, PhyLli, LLIBufferSize);
	if(Status)
	{
		return Status;
	}

	return DMA_OK;
}


/****************************************************************************/
/* NAME: 		DMA_OpenChannel	                                    */
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
t_DMAError DMA_OpenChannel(t_DMATransferDesc TransferDesc, t_DMAChannelId *ChannelId)
{
	t_DMAError Status;
	uint ControlReg = 0;

	//Checking that the DMA controller is Enabled or not.
	if(!(*DMACConfiguration & CTRL_ENABLE))
	{
		return DMA_CONTROLLER_NOT_INITIALIZED;
	}

	//If Src or Dest Address is not word alligned then return error.
	if(((uint)TransferDesc.SrcAddress%4)||((uint)TransferDesc.DestAddress%4))
	{
		return DMA_MISALLIGNED_ADDRESS;
	}

	//If the Src or Dest device id is invalid.
	if(((TransferDesc.SrcDeviceId < MEMORY_DMA_DEVICE) || (TransferDesc.SrcDeviceId >= LAST_DMA_DEVICE))||((TransferDesc.DestDeviceId < MEMORY_DMA_DEVICE) || (TransferDesc.DestDeviceId >= LAST_DMA_DEVICE)))
	{
		return DMA_INVALID_DEVICE_ID;
	}

	//If the flow controller is invalid.
	if((TransferDesc.FlowController < DMA_MEMORY_TO_MEMORY) || (TransferDesc.FlowController > SRC_PERIPHERAL_TO_PERIPHERAL))
	{
		return DMA_INVALID_FLOW_CONTROLLER;
	}

	//If the Priority is invalid.
	if((TransferDesc.Priority < LOW_PRIORITY) || (TransferDesc.Priority > UNDEFINED_PRIORITY))
	{
		return DMA_INVALID_PRIORITY;
	}

	//Checking if Pointer to channel Id are valid or NOT NULL.
	if(ChannelId == NULL)
	{
		return DMA_NULL_POINTER;
	}

	//If the IsIntReqAfterEachPacketTransfer field is invalid.
	if((TransferDesc.IsIntReqAfterEachPacketTransfer != TRUE) && (TransferDesc.IsIntReqAfterEachPacketTransfer != FALSE))
	{
		return DMA_INVALID_PARAMETER;
	}

	//Checking if Source or Destination Peripheral are not in accordance with the requested transfer mode.
	switch(TransferDesc.FlowController)
	{
		case DMA_MEMORY_TO_MEMORY:
			if((TransferDesc.SrcDeviceId != MEMORY_DMA_DEVICE)||(TransferDesc.DestDeviceId != MEMORY_DMA_DEVICE))
			{
				return DMA_INCORRECT_FLOW_CONTROLLER;
			}
			break;
		case DMA_PERIPHERAL_TO_PERIPHERAL:
		case DEST_PERIPHERAL_TO_PERIPHERAL:
		case SRC_PERIPHERAL_TO_PERIPHERAL:
			if((TransferDesc.SrcDeviceId == MEMORY_DMA_DEVICE)||(TransferDesc.DestDeviceId == MEMORY_DMA_DEVICE))
			{
				return DMA_INCORRECT_FLOW_CONTROLLER;
			}
			break;
		case DMA_PERIPHERAL_TO_MEMORY:
		case SRC_PERIPHERAL_TO_MEMORY:
			if((TransferDesc.SrcDeviceId == MEMORY_DMA_DEVICE)||(TransferDesc.DestDeviceId != MEMORY_DMA_DEVICE))
			{
				return DMA_INCORRECT_FLOW_CONTROLLER;
			}
			break;
		case DMA_MEMORY_TO_PERIPHERAL:
		case DEST_MEMORY_TO_PERIPHERAL:
			if((TransferDesc.SrcDeviceId != MEMORY_DMA_DEVICE)||(TransferDesc.DestDeviceId == MEMORY_DMA_DEVICE))
			{
				return DMA_INCORRECT_FLOW_CONTROLLER;
			}
			break;
	}

	//Acquiring the Request Lines.	
	SPEAR_DMA_PRINT("Before Acquire\n");
	Status = DMA_AcquireRequestLine(TransferDesc.SrcDeviceId, TransferDesc.DestDeviceId);
	if(Status)
	{
		return Status;
	}

	//Getting a Available Channel and verifying the controller if transfer is possible through the controller.
	Status = RequestChannel(TransferDesc.Priority, ChannelId);
	if(Status)
	{
		//Releasing the Request Lines.
		DMA_ReleaseRequestLine(TransferDesc.SrcDeviceId, TransferDesc.DestDeviceId);
		return Status;
	}

	//Set Base Channel
	Status = SetBaseChannel(*ChannelId);
	if(Status)
	{
		//Setting status of channel as FREE in software view.
		DMAChannelArray[*ChannelId].IsUsed = FALSE;

		//Releasing the Request Lines.
		DMA_ReleaseRequestLine(TransferDesc.SrcDeviceId, TransferDesc.DestDeviceId);
		return Status;
	}

	//Setting the Channel Characteristics for further reference.
	DMAChannelArray[*ChannelId].HandlerPtr = TransferDesc.HandlerPtr;
	DMAChannelArray[*ChannelId].SrcDeviceId = TransferDesc.SrcDeviceId;
	DMAChannelArray[*ChannelId].DestDeviceId = TransferDesc.DestDeviceId;

	//Checking if Peripheral is the requested flow controller.
	if((TransferDesc.FlowController >= DEST_PERIPHERAL_TO_PERIPHERAL) && (TransferDesc.FlowController <= SRC_PERIPHERAL_TO_PERIPHERAL))
	{
		//Checking if Destination peripheral is trying to be the flow controller.
		if(TransferDesc.FlowController <= DEST_MEMORY_TO_PERIPHERAL)
		{
			//Checking if Destination Can be flow controller
			if(!(DMADeviceArray[TransferDesc.DestDeviceId].CanBeFlowController))
			{
				//Setting status of channel as FREE in software view.
				DMAChannelArray[*ChannelId].IsUsed = FALSE;

				//Releasing the Request Lines.
				DMA_ReleaseRequestLine(TransferDesc.SrcDeviceId, TransferDesc.DestDeviceId);
				return DMA_DEVICE_CANT_BE_FLOWCTRL;
			}
		}
		//Checking if Source Peripheral is trying to be the flow controller.
		else if(TransferDesc.FlowController >= SRC_PERIPHERAL_TO_MEMORY)
		{
			//Checking if Source Can be flow controller
			if(!(DMADeviceArray[TransferDesc.SrcDeviceId].CanBeFlowController))
			{
				//Setting status of channel as FREE in software view.
				DMAChannelArray[*ChannelId].IsUsed = FALSE;

				//Releasing the Request Lines.
				DMA_ReleaseRequestLine(TransferDesc.SrcDeviceId, TransferDesc.DestDeviceId);
				return DMA_DEVICE_CANT_BE_FLOWCTRL;
			}
		}

		//Setting Channel Descriptor LLI to value Zero for peripheral flow control mode.
		DMAChannelArray[*ChannelId].LliListDesc.NumItems = 0;
		DMAChannelArray[*ChannelId].LliListDesc.First = NULL;
		DMAChannelArray[*ChannelId].LliListDesc.Last = NULL;

		//To Enable interrupt at the end of the transfer, as there is only a single control register configuration.
		TransferDesc.IsIntReqAfterEachPacketTransfer = TRUE;

		//Configure Control Register. The default transfer size is zero.
		Status = ConfigChannelControlReg(TransferDesc.SrcDeviceId, TransferDesc.DestDeviceId, TransferDesc.IsIntReqAfterEachPacketTransfer, &ControlReg);

		SPEAR_DMA_PRINT("Control Reg Val is %x\n",ControlReg);

		if(Status)
		{
			//Setting status of channel as FREE in software view.
			DMAChannelArray[*ChannelId].IsUsed = FALSE;

			//Releasing the Request Lines.
			DMA_ReleaseRequestLine(TransferDesc.SrcDeviceId, TransferDesc.DestDeviceId);
			return Status;
		}

		//Configuring the channel registers for first DMA transfer.
		*DMACCXSrcAddr = (uint)TransferDesc.SrcAddress;
		*DMACCXDestAddr= (uint)TransferDesc.DestAddress;
		*DMACCXLLI = 0;
		*DMACCXControl = ControlReg;
	}

	//Checking if DMA is the requested flow controller.
	else if(TransferDesc.FlowController >= DMA_MEMORY_TO_MEMORY && TransferDesc.FlowController <= DMA_PERIPHERAL_TO_PERIPHERAL)
	{
		//Getting a configured LLI list for the current transfer.
		Status = PrepareLli(TransferDesc.SrcDeviceId, TransferDesc.DestDeviceId, TransferDesc.SrcAddress, TransferDesc.DestAddress, TransferDesc.TransferSize, &(DMAChannelArray[*ChannelId].LliListDesc), TransferDesc.IsIntReqAfterEachPacketTransfer);
		//If an error is occured while Preparing LLIs.
		if(Status)
		{
			//Setting status of channel as FREE in software view.
			DMAChannelArray[*ChannelId].IsUsed = FALSE;
			//Releasing the Request Lines.
			DMA_ReleaseRequestLine(TransferDesc.SrcDeviceId, TransferDesc.DestDeviceId);
			return Status;
		}
	}


	//Configure the configuration register of the channel.
	*DMACCXConfiguration = 0;		//Reset Config register.
	*DMACCXConfiguration |= INT_TC_ENABLE;		//Enable TC interrupt.
	*DMACCXConfiguration |= INT_ERROR_ENABLE;	//Enable Error interrupt.
	if(TransferDesc.DestDeviceId != MEMORY_DMA_DEVICE)
	{
		//Dest Request Id.
		*DMACCXConfiguration |= DMADeviceArray[TransferDesc.DestDeviceId].AccessDesc.RequestId<<DEST_PERI_SHIFT;
	}
	if(TransferDesc.SrcDeviceId != MEMORY_DMA_DEVICE)
	{
		//Src Request Id.
		*DMACCXConfiguration |= DMADeviceArray[TransferDesc.SrcDeviceId].AccessDesc.RequestId<<SRC_PERI_SHIFT;
	}

	//Set Flow Controller Type.
	switch(TransferDesc.FlowController)
	{
		case DMA_MEMORY_TO_MEMORY:
			*DMACCXConfiguration |= DMA_FLOW_CTRL_MEM_TO_MEM;
			break;
		case DMA_MEMORY_TO_PERIPHERAL:
			*DMACCXConfiguration |= DMA_FLOW_CTRL_MEM_TO_PERI;
			break;
		case DMA_PERIPHERAL_TO_MEMORY:
			*DMACCXConfiguration |= DMA_FLOW_CTRL_PERI_TO_MEM;
			break;
		case DMA_PERIPHERAL_TO_PERIPHERAL:
			*DMACCXConfiguration |= DMA_FLOW_CTRL_PERI_TO_PERI;
			break;
		case DEST_MEMORY_TO_PERIPHERAL:
			*DMACCXConfiguration |= DEST_FLOW_CTRL_MEM_TO_PERI;
			break;
		case DEST_PERIPHERAL_TO_PERIPHERAL:
			*DMACCXConfiguration |= DEST_FLOW_CTRL_PERI_TO_PERI;
			break;
		case SRC_PERIPHERAL_TO_MEMORY:
			*DMACCXConfiguration |= SRC_FLOW_CTRL_PERI_TO_MEM;
			break;
		case SRC_PERIPHERAL_TO_PERIPHERAL:
			*DMACCXConfiguration |= SRC_FLOW_CTRL_PERI_TO_PERI;
			break;
		default:
			//Setting status of channel as FREE in software view.
			DMAChannelArray[*ChannelId].IsUsed = FALSE;

			//Releasing the Request Lines.
			DMA_ReleaseRequestLine(TransferDesc.SrcDeviceId, TransferDesc.DestDeviceId);
			return DMA_INVALID_FLOW_CONTROLLER;
	}

	//Setting the Src and Dest device states as Used.
	if (TransferDesc.SrcDeviceId != MEMORY_DMA_DEVICE)
	{
		DMADeviceArray[TransferDesc.SrcDeviceId].IsUsed = TRUE;
	}

	if (TransferDesc.DestDeviceId != MEMORY_DMA_DEVICE)
	{
		DMADeviceArray[TransferDesc.DestDeviceId].IsUsed = TRUE;
	}

	SPEAR_DMA_PRINT("Config Reg Val is %x\n",*DMACCXConfiguration);

	return DMA_OK;
}



/****************************************************************************/
/* NAME: 		DMA_Transfer	                                    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine allows to start transfer on a previously opened        */
/*      communication channel between a source and a destination peripheral.*/
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - ChannelId:      	This field identifies the DMA channel which */
/* 				is used to transfer data		    */
/*                                                                          */
/* OUT  :                                                                   */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_DMAError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_DMAError DMA_Transfer(t_DMAChannelId ChannelId)
{
	t_DMAError Status;

	//Checking that the DMA controller is Enabled or not.
	if(!(*DMACConfiguration & CTRL_ENABLE))
	{
		//Release the LLIs and Close the channel
		DMA_CloseChannel(ChannelId);
		return DMA_CONTROLLER_NOT_INITIALIZED;
	}

	//Set Base Channel
	Status = SetBaseChannel(ChannelId);
	if(Status)
	{
		//Release the LLIs and Close the channel
		DMA_CloseChannel(ChannelId);
		return Status;
	}

	//Enable DMA Channel.
	*DMACCXConfiguration |= CHANNEL_ENABLE;

	return DMA_OK;
}


/****************************************************************************/
/* NAME: 		DMA_CloseChannel	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine allows to Close a communication channel 		    */
/*      between a source and a destination peripheral.		            */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - ChannelId:      	This field identifies the DMA channel which */
/* 				is required to close			    */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_DMAError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_DMAError DMA_CloseChannel(t_DMAChannelId ChannelId)
{
	t_bool Result;
	t_DMAError Status;

	//Check if transfer is complete or not.
	Status = DMA_IsTransferOver(ChannelId, &Result);
	if(Status)
	{
		return Status;
	}
	if(!Result)
	{
		return DMA_TRANSFER_ONGOING;
	}

	//Free Device
	//Setting the Src and Dest device states as Unused.
	if (DMAChannelArray[ChannelId].SrcDeviceId != MEMORY_DMA_DEVICE)
	{
		DMADeviceArray[DMAChannelArray[ChannelId].SrcDeviceId].IsUsed = FALSE;
	}

	if (DMAChannelArray[ChannelId].DestDeviceId != MEMORY_DMA_DEVICE)
	{
		DMADeviceArray[DMAChannelArray[ChannelId].DestDeviceId].IsUsed = FALSE;
	}

	//Releasing the Request Lines.
	DMA_ReleaseRequestLine(DMAChannelArray[ChannelId].SrcDeviceId, DMAChannelArray[ChannelId].DestDeviceId);

	//Free Channel
	DMAChannelArray[ChannelId].IsUsed = FALSE;
	DMAChannelArray[ChannelId].HandlerPtr = NULL;

	//Free LLI List.
	if(DMAChannelArray[ChannelId].LliListDesc.NumItems != 0)
	{
		Status = FreeLli(ChannelId);
		if(Status)
		{
			return Status;
		}
	}

	DMAChannelArray[ChannelId].SrcAddress= 0x0;
	DMAChannelArray[ChannelId].DestAddress= 0x0;
	DMAChannelArray[ChannelId].TransferSize= 0x0;
	
	DMAChannelArray[ChannelId].SrcDeviceId= LAST_DMA_DEVICE;
	DMAChannelArray[ChannelId].DestDeviceId= LAST_DMA_DEVICE;

	return DMA_OK;
}


/****************************************************************************/
/* NAME: 		DMA_HaltTransfer	                            */
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
/*        t_DMAError                                                       */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_DMAError DMA_HaltTransfer(t_DMAChannelId ChannelId)
{
	t_bool Result;
	t_DMAError Status;

	//Check if transfer is complete or not.
	Status = DMA_IsTransferOver(ChannelId, &Result);
	if(Status)
	{
		return Status;
	}
	if(Result)
	{
		return DMA_TRANSFER_OVER;
	}

	//Set Base Channel
	Status = SetBaseChannel(ChannelId);
	if(Status)
	{
		return Status;
	}

	//Halt Transfer on Selected DMA channel.
	*DMACCXConfiguration |= IGNORE_NEW_REQ;
	return DMA_OK;
}


/****************************************************************************/
/* NAME: 		DMA_ResumeTransfer	                            */
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
/*        t_DMAError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_DMAError DMA_ResumeTransfer(t_DMAChannelId ChannelId)
{
	t_bool Result;
	t_DMAError Status;

	//Check if transfer is complete or not.
	Status = DMA_IsTransferOver(ChannelId, &Result);
	if(Status)
	{
		return Status;
	}
	if(Result)
	{
		return DMA_TRANSFER_OVER;
	}

	//Set Base Channel
	Status = SetBaseChannel(ChannelId);
	if(Status)
	{
		return Status;
	}

	//Resume Transfer on Selected DMA channel.
	*DMACCXConfiguration &= TAKE_NEW_REQ;
	return DMA_OK;
}

/****************************************************************************/
/* NAME: 		DMA_AbortTransfer	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine allows to Abort an on-going DMA Transfer  		    */
/*      on a given DMA channel.					            */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - ChannelId:     identify the DMA channel                     	    */
/*      - DataCanBeLost: If TRUE then the channel will be disabled and any  */
/*                       data left in FIFO will be lost.                    */
/*      		 If FALSE then the channel will be disabled after   */
/*                       transferring data left in FIFO.                    */
/*                                                                          */
/*                                                                          */
/* OUT :  none                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_DMAError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_DMAError DMA_AbortTransfer(t_DMAChannelId ChannelId, t_bool DataCanBeLost)
{
	t_DMAError Status;

	//Set Base Channel
	Status = SetBaseChannel(ChannelId);
	if(Status)
	{
		return Status;
	}

	//If data present in the device FIFO can be lost.
	if(DataCanBeLost == TRUE)
	{
		//Abort Transfer on Selected DMA channel.
		*DMACCXConfiguration &= CHANNEL_DISABLE;
	}
	//If data present in the device FIFO should be transered.
	else if(DataCanBeLost == FALSE)
	{
		//Halt Transfer on Selected DMA channel.
		*DMACCXConfiguration |= IGNORE_NEW_REQ;

		//Wait while there is any data in the FIFO or the Active bit is high.
		while(*DMACCXConfiguration & CHANNEL_ACTIVE_MASK);

		//Abort Transfer on Selected DMA channel.
		*DMACCXConfiguration &= CHANNEL_DISABLE;
	}
	else
	{
		return DMA_INVALID_PARAMETER;
	}

	//Closing the Channel in software view.
	Status = DMA_CloseChannel(ChannelId);
	if(Status)
	{
		return Status;
	}

	return DMA_OK;
}

/****************************************************************************/
/* t_DMAError DMA_SetConfigDevice()		                            */
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
/*      - CanBeFlowController:	Can be flow controller.                     */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_DMAError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_DMAError DMA_SetConfigDevice(t_DMADeviceId DeviceId, t_DMAAccessWidth AccessWidth, t_DMABurstSize MaxBurstSize, t_DMAProtection Protection, t_bool CanBeFlowController)
{
	//Checking that the DMA controller is Enabled or not.
	if(!(*DMACConfiguration & CTRL_ENABLE))	
	{
		return DMA_CONTROLLER_NOT_INITIALIZED;
	}

	//If Device Id is invalid.
	if((DeviceId < MEMORY_DMA_DEVICE) || (DeviceId >= LAST_DMA_DEVICE))
	{
		return DMA_INVALID_DEVICE_ID;
	}

	//If Protection Field is invalid
	if(Protection != USER_BUFFERABLE_CACHEABLE && Protection != USER_NOT_BUFFERABLE_CACHEABLE && Protection != USER_BUFFERABLE_NOT_CACHEABLE && Protection != USER_NOT_BUFFERABLE_NOT_CACHEABLE && Protection != PRIV_BUFFERABLE_CACHEABLE && Protection != PRIV_NOT_BUFFERABLE_CACHEABLE && Protection != PRIV_BUFFERABLE_NOT_CACHEABLE && Protection != PRIV_NOT_BUFFERABLE_NOT_CACHEABLE)
	{
		return DMA_INVALID_PROTECTION;
	}

	//If Access Width is invalid
	if((AccessWidth != BYTE_WIDTH) && (AccessWidth != HALFWORD_WIDTH) && (AccessWidth != WORD_WIDTH))
	{
		return DMA_INVALID_ACCESS_WIDTH;
	}

	//If Burst Size is invalid
	if(!((MaxBurstSize >= BURST_SIZE_1) && (MaxBurstSize <= BURST_SIZE_256)))
	{
		return DMA_INVALID_BURST_SIZE;
	}

	//If CanBeFlowController field is invalid.
	if((CanBeFlowController != TRUE) && (CanBeFlowController != FALSE))
	{
		return DMA_INVALID_PARAMETER;
	}

	//Setting Device Configuration.
	DMADeviceArray[DeviceId].Protection = Protection;
	DMADeviceArray[DeviceId].AccessWidth = AccessWidth;
	DMADeviceArray[DeviceId].MaxBurstSize = MaxBurstSize;
	DMADeviceArray[DeviceId].CanBeFlowController = CanBeFlowController;

	return DMA_OK;
}

/****************************************************************************/
/* t_DMAError DMA_GetConfigDevice()		                            */
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
/*      - CanBeFlowController:	Can be flow controller.                     */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_DMAError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_DMAError DMA_GetConfigDevice(t_DMADeviceId DeviceId, t_DMAAccessWidth *AccessWidth, t_DMABurstSize *MaxBurstSize, t_DMAProtection *Protection, t_bool *CanBeFlowController)
{
	//Checking that the DMA controller is Enabled or not.
	if(!(*DMACConfiguration & CTRL_ENABLE))
	{
		return DMA_CONTROLLER_NOT_INITIALIZED;
	}

	//If Device Id is invalid.
	if((DeviceId < MEMORY_DMA_DEVICE) || (DeviceId >= LAST_DMA_DEVICE))
	{
		return DMA_INVALID_DEVICE_ID;
	}

	//Checking if Pointer to AccessWidth or MaxBurstSize or Protection or CanBeFlowController are valid or NOT NULL.
	if((AccessWidth == NULL)||(MaxBurstSize == NULL)||(Protection == NULL)||(CanBeFlowController == NULL))
	{
		return DMA_NULL_POINTER;
	}

	//Getting Device configuration.
	*Protection = DMADeviceArray[DeviceId].Protection;

	*AccessWidth = DMADeviceArray[DeviceId].AccessWidth;

	*MaxBurstSize = DMADeviceArray[DeviceId].MaxBurstSize;

	*CanBeFlowController = DMADeviceArray[DeviceId].CanBeFlowController;

	return DMA_OK;
}

/********************************************************************************/
/* NAME:           DMA_IntEnable						*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This API is used to Enable Interrupt on the specified DMA Pins.	 	*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  :                                                     	                */
/*      - ChannelId:     identify the DMA channel                     		*/
/*                                                                          	*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: t_DMAError							    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
t_DMAError DMA_IntEnable(t_DMAChannelId ChannelId)
{
	t_bool Result;
	t_DMAError Status;

	//Check if transfer is complete or not.
	Status = DMA_IsTransferOver(ChannelId, &Result);
	if(Status)
	{
		return Status;
	}
	if(!Result)
	{
		return DMA_TRANSFER_ONGOING;
	}

	//Set Base Channel
	Status = SetBaseChannel(ChannelId);
	if(Status)
	{
		return Status;
	}

	//Enable interrupts on Selected DMA channel.
	*DMACCXConfiguration |= INT_TC_ENABLE;
	*DMACCXConfiguration |= INT_ERROR_ENABLE;
	return DMA_OK;
}


/********************************************************************************/
/* NAME:           DMA_IntDisable						*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This API is used to Disable Interrupt on the specified DMA Pins.	 	*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  :                                                     	                */
/*      - ChannelId:     identify the DMA channel                     		*/
/*                                                                          	*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: t_DMAError							    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
t_DMAError DMA_IntDisable(t_DMAChannelId ChannelId)
{
	t_bool Result;
	t_DMAError Status;

	//Check if transfer is complete or not.
	Status = DMA_IsTransferOver(ChannelId, &Result);
	if(Status)
	{
		return Status;
	}
	if(!Result)
	{
		return DMA_TRANSFER_ONGOING;
	}

	//Set Base Channel
	Status = SetBaseChannel(ChannelId);
	if(Status)
	{
		return Status;
	}

	//Disable interrupts on Selected DMA channel.
	*DMACCXConfiguration &= INT_TC_DISABLE;
	*DMACCXConfiguration &= INT_ERROR_DISABLE;
	return DMA_OK;
}


/********************************************************************************/
/* NAME:           DMA_IntClear							*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This API is used to clear Interrupts on the specified DMA Pins.	 	*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  :                                                     	                */
/*      - ChannelId:     identify the DMA channel                     		*/
/*                                                                          	*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: t_DMAError							    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
t_DMAError DMA_IntClear(t_DMAChannelId ChannelId)
{
	t_bool Result;
	t_DMAError Status;

	//Check if transfer is complete or not.
	Status = DMA_IsTransferOver(ChannelId, &Result);
	if(Status)
	{
		return Status;
	}
	if(!Result)
	{
		return DMA_TRANSFER_ONGOING;
	}

	//Set Base Channel
	Status = SetBaseChannel(ChannelId);
	if(Status)
	{
		return Status;
	}

	//Clear interrupts on Selected DMA channel.
	*DMACIntTCClear |= 1<<ChannelId;
	*DMACIntErrClear |= 1<<ChannelId;

	return DMA_OK;
}


/********************************************************************************/
/* NAME:           DMA_IntHandler						*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This is Interrupt Handler function terminal count.			        */
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  : 								    	*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: 								    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
void DMA_IntHandler(t_DMAChannelId ChannelId, t_DMAIntType IntType, t_bool IsTransferOver)
{
	t_DMAError Status;

	//Clear Interrupt which has been serviced.
	Status = DMA_IntClear(ChannelId);

	SPEAR_DMA_PRINT("This is IntHandler New Channel Id is %d\n",ChannelId);
	SPEAR_DMA_PRINT("Channel Register values are. Next Lli is %x, Config Reg is %x, Control Reg is %x\n",*DMACCXLLI, *DMACCXConfiguration, *DMACCXControl);

	//Checking if a call back function is registered or not
	if((*DMAChannelArray[ChannelId].HandlerPtr) != NULL)
	{
		//Call the interrupt handler registered for that channel.
		(*DMAChannelArray[ChannelId].HandlerPtr)(IsTransferOver, IntType);
	}

	SPEAR_DMA_PRINT("Before Close\n");
	//Closing the Channel in software view.
	if(IsTransferOver == TRUE)
		while((Status = DMA_CloseChannel(ChannelId)) == DMA_TRANSFER_ONGOING);
}
