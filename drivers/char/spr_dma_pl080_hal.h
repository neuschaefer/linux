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
#ifndef SPEAr_DMA_HAL_H
#define SPEAr_DMA_HAL_H

#include <linux/types.h>
#include <asm/arch/hcl_defs.h>
#include <asm/arch/spear.h>
#include <asm/arch/hardware.h>
#include <asm/arch/spr_misc_config.h>
#include "spr_dma_pl080_config_device.h"

#define SPEAR_DMA_DEBUG 0

#define SPEAR_DMA_PRINT(x...) ;
//#define SPEAR_DMA_PRINT(x...) printk(x)

#ifndef NULL
	#define NULL 0
#endif

//Maximum number of Request Lines supported by controller.
#define REQUEST_LINE_MAX 16

//Maximum number of channels supported by controller.
#define CHANNEL_MAX 8

//Maximum transfer size using a single LLI.
#define DMA_MAX_TRANSFER_SIZE ((1<<12)-1)

//Base Address of DMA Controller.

#ifdef CONFIG_ARCH_SPEARPLUS
#define SPEAr_DMA_AHB_BASE VA_SPEARPLUS_START_DMA
#endif

#ifdef CONFIG_ARCH_SPEARBASIC
#define SPEAr_DMA_AHB_BASE VA_SPEARBASIC_START_DMA
#endif

//Base of DMA Channel Registers.
#define SPEAr_DMA_CHANNEL0_BASE 0x00000000
#define SPEAr_DMA_CHANNEL1_BASE 0x00000020
#define SPEAr_DMA_CHANNEL2_BASE 0x00000040
#define SPEAr_DMA_CHANNEL3_BASE 0x00000060
#define SPEAr_DMA_CHANNEL4_BASE 0x00000080
#define SPEAr_DMA_CHANNEL5_BASE 0x000000a0
#define SPEAr_DMA_CHANNEL6_BASE 0x000000c0
#define SPEAr_DMA_CHANNEL7_BASE 0x000000e0


//DMA Registers Base Address Mapping
#define DMACIntStatus 		((volatile uint *)(SPEAr_DMA_AHB_BASE + 0x000))
#define DMACIntTCStatus 	((volatile uint *)(SPEAr_DMA_AHB_BASE + 0x004))  
#define DMACIntTCClear 		((volatile uint *)(SPEAr_DMA_AHB_BASE + 0x008))
#define DMACIntErrorStatus 	((volatile uint *)(SPEAr_DMA_AHB_BASE + 0x00c))  
#define DMACIntErrClear 	((volatile uint *)(SPEAr_DMA_AHB_BASE + 0x010))  
#define DMACRawIntTCStatus 	((volatile uint *)(SPEAr_DMA_AHB_BASE + 0x014))  
#define DMACRawIntErrorStatus 	((volatile uint *)(SPEAr_DMA_AHB_BASE + 0x018))  
#define DMACEnbldChns 		((volatile uint *)(SPEAr_DMA_AHB_BASE + 0x01c))  
#define DMACSoftBReq 		((volatile uint *)(SPEAr_DMA_AHB_BASE + 0x020))  
#define DMACSoftSReq 		((volatile uint *)(SPEAr_DMA_AHB_BASE + 0x024))  
#define DMACSoftLBReq 		((volatile uint *)(SPEAr_DMA_AHB_BASE + 0x028))  
#define DMACSoftLSReq 		((volatile uint *)(SPEAr_DMA_AHB_BASE + 0x02c))  
#define DMACConfiguration 	((volatile uint *)(SPEAr_DMA_AHB_BASE + 0x030))  
#define DMACSync 		((volatile uint *)(SPEAr_DMA_AHB_BASE + 0x034))  
#define DMACCXSrcAddr		((volatile uint *)(SPEAr_DMA_AHB_BASE + SPEAr_DMA_CHANNEL_BASE + 0x100))  
#define DMACCXDestAddr 		((volatile uint *)(SPEAr_DMA_AHB_BASE + SPEAr_DMA_CHANNEL_BASE + 0x104))  
#define DMACCXLLI 		((volatile uint *)(SPEAr_DMA_AHB_BASE + SPEAr_DMA_CHANNEL_BASE + 0x108))  
#define DMACCXControl 		((volatile uint *)(SPEAr_DMA_AHB_BASE + SPEAr_DMA_CHANNEL_BASE + 0x10c))  
#define DMACCXConfiguration 	((volatile uint *)(SPEAr_DMA_AHB_BASE + SPEAr_DMA_CHANNEL_BASE + 0x110))  
#define DMACPeriphID0 		((volatile uint *)(SPEAr_DMA_AHB_BASE + 0xfe0))  
#define DMACPeriphID1 		((volatile uint *)(SPEAr_DMA_AHB_BASE + 0xfe4))  
#define DMACPeriphID2 		((volatile uint *)(SPEAr_DMA_AHB_BASE + 0xfe8))  
#define DMACPeriphID3 		((volatile uint *)(SPEAr_DMA_AHB_BASE + 0xfec))  
#define DMACPCellID0 		((volatile uint *)(SPEAr_DMA_AHB_BASE + 0xff0))  
#define DMACPCellID1 		((volatile uint *)(SPEAr_DMA_AHB_BASE + 0xff4))  
#define DMACPCellID2 		((volatile uint *)(SPEAr_DMA_AHB_BASE + 0xff8))  
#define DMACPCellID3 		((volatile uint *)(SPEAr_DMA_AHB_BASE + 0xffc))  
#define DMACITCR 		((volatile uint *)(SPEAr_DMA_AHB_BASE + 0x500))  
#define DMACITOP1 		((volatile uint *)(SPEAr_DMA_AHB_BASE + 0x0000))  
#define DMACITOP2 		((volatile uint *)(SPEAr_DMA_AHB_BASE + 0x0000))  
#define DMACITOP3 		((volatile uint *)(SPEAr_DMA_AHB_BASE + 0x0000))  


//Constants for configuring DMA Registers

// DMA Configuration Register Description
typedef enum
{
	CTRL_ENABLE = 0X01,
	CTRL_DISABLE = ~ CTRL_ENABLE,
	AHB1_BIG_ENDIAN = 0x02,
	AHB1_LITTLE_ENDIAN = ~AHB1_BIG_ENDIAN,
	AHB2_BIG_ENDIAN = 0x04,
	AHB2_LITTLE_ENDIAN = ~AHB2_BIG_ENDIAN
}t_DMACofigReg;


// Channel linked list item register
typedef enum
{
	LM_AHB_MASTER2 = 0x00000001,
	LM_AHB_MASTER1 = ~LM_AHB_MASTER2
} t_DMAChannelXLliReg;


/* Channel Control register */

#define INT_TC_LLI_ENABLE	0x80000000
#define INT_TC_LLI_DISABLE	~(INT_TC_ENABLE)

typedef enum
{
	PROT_RESET 				= ~(0x70000000),
	USER_BUFFERABLE_CACHEABLE		= 0x60000000,
	USER_NOT_BUFFERABLE_CACHEABLE		= 0x40000000,
	USER_BUFFERABLE_NOT_CACHEABLE		= 0x20000000,
	USER_NOT_BUFFERABLE_NOT_CACHEABLE	= 0x00000000,
	PRIV_BUFFERABLE_CACHEABLE		= 0x70000000,
	PRIV_NOT_BUFFERABLE_CACHEABLE		= 0x50000000,
	PRIV_BUFFERABLE_NOT_CACHEABLE		= 0x30000000,
	PRIV_NOT_BUFFERABLE_NOT_CACHEABLE	= 0x10000000
}t_DMAProtection;

typedef enum
{
	CONTROL_RESET			= 0x00000000,

	DEST_INC_ENABLE = 0x08000000,
	DEST_INC_DISABLE= ~DEST_INC_ENABLE,

	SRC_INC_ENABLE  = 0x04000000,
	SRC_INC_DISABLE = ~SRC_INC_ENABLE,

	DEST_MASTER2    = 0x02000000,
	DEST_MASTER1    = ~DEST_MASTER2,

	SRC_MASTER2 	= 0x01000000,
	SRC_MASTER1 	= ~SRC_MASTER2,

	DEST_WIDTH_RESET 	= ~(0x00e00000),
	DEST_WIDTH_BYTE 	= 0x00000000,
	DEST_WIDTH_HALFWORD	= 0x00200000,
	DEST_WIDTH_WORD 	= 0x00400000,

	SRC_WIDTH_RESET 	= ~(0x001c0000),
	SRC_WIDTH_BYTE 		= 0x00000000,
	SRC_WIDTH_HALFWORD	= 0x00040000,
	SRC_WIDTH_WORD 		= 0x00080000,

	DEST_BURST_RESET 	= ~(0x00038000),
	DEST_BURST_SIZE1 	= 0x00000000,
	DEST_BURST_SIZE4 	= 0x00008000,
	DEST_BURST_SIZE8 	= 0x00010000,
	DEST_BURST_SIZE16 	= 0x00018000,
	DEST_BURST_SIZE32 	= 0x00020000,
	DEST_BURST_SIZE64 	= 0x00028000,
	DEST_BURST_SIZE128 	= 0x00030000,
	DEST_BURST_SIZE256 	= 0x00038000,

	SRC_BURST_RESET 	= ~(0x00007000),
	SRC_BURST_SIZE1 	= 0x00000000,
	SRC_BURST_SIZE4 	= 0x00001000,
	SRC_BURST_SIZE8 	= 0x00002000,
	SRC_BURST_SIZE16 	= 0x00003000,
	SRC_BURST_SIZE32 	= 0x00004000,
	SRC_BURST_SIZE64 	= 0x00005000,
	SRC_BURST_SIZE128 	= 0x00006000,
	SRC_BURST_SIZE256 	= 0x00007000,

	TRANSFER_SIZE_RESET	= ~(0x00000fff)
} t_DMAChannelXCtrlReg;


// Channel Configuration register
typedef enum
{
	CONFIG_RESET		= 0x00000000,

	IGNORE_NEW_REQ		= 0x00040000,
	TAKE_NEW_REQ		= ~IGNORE_NEW_REQ,

	CHANNEL_ACTIVE_MASK	= 0x00020000,

	CHANNEL_LOCK		= 0x00010000,
	CHANNEL_UNLOCK		= ~CHANNEL_LOCK,

	INT_TC_ENABLE		= 0x00008000,
	INT_TC_DISABLE		= ~INT_TC_ENABLE,

	INT_ERROR_ENABLE	= 0x00004000,
	INT_ERROR_DISABLE	= ~INT_ERROR_ENABLE,

	FLOW_RESET				= ~(0x00003800),
	DMA_FLOW_CTRL_MEM_TO_MEM		= 0x00000000,
	DMA_FLOW_CTRL_MEM_TO_PERI		= 0x00000800,
	DMA_FLOW_CTRL_PERI_TO_MEM		= 0x00001000,
	DMA_FLOW_CTRL_PERI_TO_PERI		= 0x00001800,
	DEST_FLOW_CTRL_PERI_TO_PERI		= 0x00002000,
	DEST_FLOW_CTRL_MEM_TO_PERI		= 0x00002800,
	SRC_FLOW_CTRL_PERI_TO_MEM		= 0x00003000,
	SRC_FLOW_CTRL_PERI_TO_PERI		= 0x00003800,

	DEST_PERI_RESET		= ~(0x000003c0),
	DEST_PERI_SHIFT		= 6,
	DEST_PERI0 		= 0x00000000,
	DEST_PERI1		= 0x00000040,
	DEST_PERI2		= 0x00000080,
	DEST_PERI3		= 0x000000c0,
	DEST_PERI4		= 0x00000100,
	DEST_PERI5		= 0x00000140,
	DEST_PERI6		= 0x00000180,
	DEST_PERI7		= 0x000001c0,
	DEST_PERI8		= 0x00000200,
	DEST_PERI9		= 0x00000240,
	DEST_PERI10		= 0x00000280,
	DEST_PERI11		= 0x000002c0,
	DEST_PERI12		= 0x00000300,
	DEST_PERI13		= 0x00000340,
	DEST_PERI14		= 0x00000380,
	DEST_PERI15		= 0x000003c0,


	SRC_PERI_RESET		= ~(0x0000001e),
	SRC_PERI_SHIFT		= 1,
	SRC_PERI0 		= 0x00000000,
	SRC_PERI1		= 0x00000002,
	SRC_PERI2		= 0x00000004,
	SRC_PERI3		= 0x00000006,
	SRC_PERI4		= 0x00000008,
	SRC_PERI5		= 0x0000000a,
	SRC_PERI6		= 0x0000000c,
	SRC_PERI7		= 0x0000000e,
	SRC_PERI8		= 0x00000010,
	SRC_PERI9		= 0x00000012,
	SRC_PERI10		= 0x00000014,
	SRC_PERI11		= 0x00000016,
	SRC_PERI12		= 0x00000018,
	SRC_PERI13		= 0x0000001a,
	SRC_PERI14		= 0x0000001c,
	SRC_PERI15		= 0x0000001e,

	CHANNEL_ENABLE		= 0x00000001,
	CHANNEL_DISABLE		= ~CHANNEL_ENABLE
} t_DMAChannelXConfigReg;


//Enums used in DMA Driver.
//DMA Channel Mask
typedef enum
{
	CHANNEL0_MASK = 0x01,
	CHANNEL1_MASK = 0x02,
	CHANNEL2_MASK = 0x04,
	CHANNEL3_MASK = 0x08,
	CHANNEL4_MASK = 0x10,
	CHANNEL5_MASK = 0x20,
	CHANNEL6_MASK = 0x40,
	CHANNEL7_MASK = 0x80,
	CHANNEL_NONE  = 0x100
}t_DMAChannelMask;

//Channel List
typedef enum
{
	CHANNEL0,
	CHANNEL1,
	CHANNEL2,
	CHANNEL3,
	CHANNEL4,
	CHANNEL5,
	CHANNEL6,
	CHANNEL7,
	CHANNEL_INVALID
}t_DMAChannelId;

//Define the four levels of priority of DMA transfer
typedef enum 
{
	LOW_PRIORITY,
	NORMAL_PRIORITY,
	HIGH_PRIORITY,
	UNDEFINED_PRIORITY
} t_DMATransferPriority;


//Interupt Types
typedef enum
{
	INT_TC,
	INT_ERROR
}t_DMAIntType;

//Define the Burst size of DMA Requests
typedef enum
{
	BURST_SIZE_1,
	BURST_SIZE_4,
	BURST_SIZE_8,
	BURST_SIZE_16,
	BURST_SIZE_32,
	BURST_SIZE_64,
	BURST_SIZE_128,
	BURST_SIZE_256
} t_DMABurstSize;


//Define the type of flows and their flow controllers.
typedef enum
{
	DMA_MEMORY_TO_MEMORY,
	DMA_MEMORY_TO_PERIPHERAL,
	DMA_PERIPHERAL_TO_MEMORY,
	DMA_PERIPHERAL_TO_PERIPHERAL,
	DEST_PERIPHERAL_TO_PERIPHERAL,
	DEST_MEMORY_TO_PERIPHERAL,
	SRC_PERIPHERAL_TO_MEMORY,
	SRC_PERIPHERAL_TO_PERIPHERAL
}t_DMAFlowCtrl;

//Define various AHB Masters available.
typedef enum
{
	AHB0_MASTER,
	AHB1_MASTER
} t_DMAMaster;

//Define the various access width supported by the DMA.
typedef enum
{
	BYTE_WIDTH = 1,
	HALFWORD_WIDTH = 2,
	WORD_WIDTH = 4
} t_DMAAccessWidth;

//Define the type used to describe the possible errors of that can occurin.
typedef enum
{
	DMA_OK = 0,             //0
	DMA_INVALID_BURST_SIZE, //1
	DMA_INVALID_ACCESS_WIDTH,
	DMA_INVALID_PROTECTION,
	DMA_INVALID_CHANNEL_ID,
	DMA_INVALID_INT_HANDLER,//5
	DMA_INCORRECT_FLOW_CONTROLLER,
	DMA_INVALID_DEVICE_ID,
	DMA_INVALID_PRIORITY,
	DMA_INVALID_PARAMETER,
	DMA_INCOMPATIBLE_DMA_DEVICE,//10
	DMA_NO_MORE_LLIS_AVAILABLE,
	DMA_NO_MORE_HW_CHANNELS_AVAILABLE,
	DMA_NO_INT_ACTIVE,
	DMA_TRANSFER_OVER,
	DMA_TRANSFER_ONGOING,
	DMA_INCOMPATIBLE_TRANSFER_SIZE,
	DMA_INCOMPATIBLE_BUFFER_SIZE,
	DMA_DEVICE_CANT_BE_FLOWCTRL,
	DMA_TRANSFER_NOT_SUPPORTED,
	DMA_BUSY,
	DMA_INVALID_FLOW_CONTROLLER,
	DMA_CONTROLLER_NOT_INITIALIZED,
	DMA_NULL_POINTER,
	DMA_MISALLIGNED_ADDRESS,
	DMA_ERROR
}t_DMAError;




//New Types used in DMA

//typedef for a pointer to a interrupt handler
typedef void (* t_DMAIntHandler)(t_bool IsTransferOver, t_DMAIntType IntType);

//Structures used in DMA.

// LLI Descriptor
typedef struct
{
	uint *SrcAddrReg;
	uint *DestAddrReg;
	uint NextLli;
	uint ChannelControlReg;
}t_DMALliDesc;

//LLI List Descriptor.
typedef struct
{
	uint NumItems;	//Total number of LLI's in the list.
	t_DMALliDesc *First;	//Pointer to the first LLI of list.
	t_DMALliDesc *Last;	//pointer to the last LLI of list.
}t_DMALliListDesc;


//Channel Descriptor
typedef struct
{
	t_DMAChannelId ChannelId;
	t_DMAIntHandler HandlerPtr;
	t_DMALliListDesc LliListDesc;
	uint *SrcAddress;
	uint *DestAddress;
	uint TransferSize;
	t_DMADeviceId SrcDeviceId;
	t_DMADeviceId DestDeviceId;
	t_bool IsUsed;
}t_DMAChannelDesc;

//Define a structure used to declare how a DMA device could be accessed (on which DMA AHB Master interface, and with which DMA Request signal)
typedef struct
{
	t_DMAMaster WhichMaster;
	t_DMARequestId RequestId;
	t_MiscDMAScheme	ConfigScheme;
}t_DMAAccessDesc;

//Request Line Descriptor
typedef struct
{
	t_MiscDMAScheme	CurrentConfigScheme;
	uint RequestCount;
}t_DMARequestDesc;

//Device Descriptor
typedef struct
{
	t_DMABurstSize	MaxBurstSize;
	t_DMAProtection	Protection;
	t_DMAAccessDesc	AccessDesc;
	t_DMAAccessWidth AccessWidth;
	t_bool CanBeFlowController;     //Device is able to control/Use LBREQ or LSREQ signals
	t_bool IsUsed;
}t_DMADeviceDesc;

//Transfer Descriptor
typedef struct
{
	t_DMADeviceId SrcDeviceId;
	t_DMADeviceId DestDeviceId;
	uint *SrcAddress;
	uint *DestAddress;
	uint TransferSize;
	t_DMAFlowCtrl FlowController;
	t_DMATransferPriority Priority;
	t_bool IsIntReqAfterEachPacketTransfer;
	t_DMAIntHandler HandlerPtr;
}t_DMATransferDesc;


/***********************************************************************************
 * PUBLIC Functions
 ***********************************************************************************/
//This API is used to initialize the DMA.
t_DMAError DMA_Init(uint *LLIBufferAddr, dma_addr_t PhyLli, uint LLIBufferSize);

//This API is used to open a new channel.
t_DMAError DMA_OpenChannel(t_DMATransferDesc TransferDesc, t_DMAChannelId *ChannelId);

//This API is used to transfer data.
t_DMAError DMA_Transfer(t_DMAChannelId ChannelId);

//This API is used to close a previously opened channel.
t_DMAError DMA_CloseChannel(t_DMAChannelId ChannelId);

//This API is used to Halt transfer on a previously opened channel.
t_DMAError DMA_HaltTransfer(t_DMAChannelId ChannelId);

//This API is used to Resume transfer on a previously halted channel.
t_DMAError DMA_ResumeTransfer(t_DMAChannelId ChannelId);

//This API is used to Abort transfer on a previously Opened channel with or without data loss.
t_DMAError DMA_AbortTransfer(t_DMAChannelId ChannelId, t_bool DataCanBeLost);

//This API is used to configure a device.
t_DMAError DMA_SetConfigDevice(t_DMADeviceId DeviceId, t_DMAAccessWidth AccessWidth, t_DMABurstSize MaxBurstSize, t_DMAProtection Protection, t_bool CanBeFlowController);

//This API is used to get configuration of a device.
t_DMAError DMA_GetConfigDevice(t_DMADeviceId DeviceId, t_DMAAccessWidth *AccessWidth, t_DMABurstSize *MaxBurstSize, t_DMAProtection *Protection, t_bool *CanBeFlowController);

//API's for Interrupt Handling
t_DMAError DMA_IntEnable(t_DMAChannelId ChannelId);
t_DMAError DMA_IntDisable(t_DMAChannelId ChannelId);
t_DMAError DMA_IntClear(t_DMAChannelId ChannelId);

//Getting Interrupt Sources
t_DMAError DMA_GetIntSrc(t_DMAChannelId *ChannelId, t_DMAIntType *IntType, t_bool *IsTransferOver);

//Interrupt Handler
void DMA_IntHandler(t_DMAChannelId ChannelId, t_DMAIntType IntType, t_bool IsTransferOver);

int DMA_FindNumOfLli(t_DMADeviceId SrcDeviceId, t_DMADeviceId DestDeviceId, uint TransferSize);
#endif // SPEAr_DMA_HAL_H
