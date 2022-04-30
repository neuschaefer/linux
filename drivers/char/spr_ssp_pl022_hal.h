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
#ifndef SPEAr_SSP_HAL_H
#define SPEAr_SSP_HAL_H

#include <asm/arch/spear.h>
#include <asm/arch/hardware.h>
#include <linux/types.h>
#include <asm/arch/hcl_defs.h>

#define SPEAR_SSP_DEBUG 0

#define SPEAR_SSP_PRINT(x...) ;
//#define SPEAR_SSP_PRINT(x...) printk(x)

//Base Address of SSP Devices..
#ifdef CONFIG_ARCH_SPEARPLUS
#define SPEAr_SSP0_APB_BASE VA_SPEARPLUS_START_SSP0
#define SPEAr_SSP1_APB_BASE VA_SPEARPLUS_START_SSP1
#define SPEAr_SSP2_APB_BASE VA_SPEARPLUS_START_ICM2_SSP2
#elif defined(CONFIG_ARCH_SPEARBASIC)
#define SPEAr_SSP0_APB_BASE VA_SPEARBASIC_START_SSP
#endif

//SSP Registers Base Address Mapping
#define SSPCR0 		((volatile uint *)(SPEAr_SSP_APB_BASE + 0x000))
#define SSPCR1	 	((volatile uint *)(SPEAr_SSP_APB_BASE + 0x004))
#define SSPDR		((volatile short int *)(SPEAr_SSP_APB_BASE + 0x008))
#define SSPSR		((volatile uint *)(SPEAr_SSP_APB_BASE + 0x00C))
#define SSPCPSR		((volatile uint *)(SPEAr_SSP_APB_BASE + 0x010))
#define SSPIMSC		((volatile uint *)(SPEAr_SSP_APB_BASE + 0x014))
#define SSPRIS		((volatile uint *)(SPEAr_SSP_APB_BASE + 0x018))
#define SSPMIS		((volatile uint *)(SPEAr_SSP_APB_BASE + 0x01C))
#define SSPICR		((volatile uint *)(SPEAr_SSP_APB_BASE + 0x020))
#define SSPDMACR	((volatile uint *)(SPEAr_SSP_APB_BASE + 0x024))

//Total Number of SSPs
#ifdef CONFIG_ARCH_SPEARPLUS
#define SSP_NUM 3
#else
#define SSP_NUM 1
#endif

//Max FIFO Size
#define SSP_FIFO_SIZE 16

//Min and Max values of Clock Rate and Prescale divisor
#define MIN_PSDVSR 0x02
#define MAX_PSDVSR 0xFE
#define MIN_SCR 0x00
#define MAX_SCR 0xFF

#ifndef NULL
#define NULL 0
#endif

//Setting Base Address Depending on the SSPId
#ifdef CONFIG_ARCH_SPEARPLUS
#define SELECT_DEVICE (SPEAr_SSP_APB_BASE = ((SSPId == SSP0) ? SPEAr_SSP0_APB_BASE:((SSPId == SSP1) ? SPEAr_SSP1_APB_BASE:SPEAr_SSP2_APB_BASE)))
#else
#define SELECT_DEVICE (SPEAr_SSP_APB_BASE = SPEAr_SSP0_APB_BASE)
#endif

//Macros for getting the status of the FIFOs.
//To Check that Tx FIFO is full or Not.
#define SSP_IsTxFIFOFull ((*SSPSR & TX_FIFO_NOT_FULL_MASK) ? FALSE:TRUE)

//To Check that Tx FIFO is empty or Not.
#define SSP_IsTxFIFOEmpty ((*SSPSR & TX_FIFO_EMPTY_MASK) ? TRUE:FALSE)

//To Check that Rx FIFO is full or Not.
#define SSP_IsRxFIFOFull ((*SSPSR & RX_FIFO_FULL_MASK) ? TRUE:FALSE)

//To Check that Rx FIFO is empty or Not.
#define SSP_IsRxFIFOEmpty ((*SSPSR & RX_FIFO_NOT_EMPTY_MASK) ? FALSE:TRUE)

//Dummy Word used for receiving Data.
#define DUMMY_DATA 0x0000


//Constants for configuring SSP Registers

//SSP Control Register 0.
typedef enum
{
	FRAME_FORMAT_SHIFT	= 0x00000004,
	CLK_RATE_SHIFT		= 0x00000008,
	CLK_RATE_MASK		= 0x0000ff00
}t_SSPCtrlReg0;

//SSP Data Register.
typedef enum
{
	DATA_MASK 		= 0x0000ffff
}t_SSPDataReg;

//SSP Status Register.
typedef enum
{
	SSP_BUSY_MASK	 	= 0x00000010,
	RX_FIFO_FULL_MASK 	= 0x00000008,
	RX_FIFO_NOT_EMPTY_MASK 	= 0x00000004,
	TX_FIFO_NOT_FULL_MASK 	= 0x00000002,
	TX_FIFO_EMPTY_MASK 	= 0x00000001
}t_SSPStatusReg;

//SSP Prescale Register.
typedef enum
{
	PRESCALE_MASK 		= 0x000000ff
}t_SSPPrescaleReg;

//SSP Error Clear Register.
typedef enum
{
	RECEIVE_TIMEOUT_CLEAR	= 0x00000002,
	RECEIVE_OVERRUN_CLEAR	= 0x00000001
}t_SSPErrClearReg;

//SSP DMA Control Register 
typedef enum
{
	SSP_DMA_ENABLE 		= 0x00000003,
	SSP_DMA_DISABLE 	= 0x00000000
}t_SSPDMACtrlReg;

//Define the various SSP Device ID's.
typedef enum
{
	SSP0,
#ifdef CONFIG_ARCH_SPEARPLUS
	SSP1,
	SSP2,
#endif
	SSP_ALL
}t_SSPId;

//Define the various Data Sizes supported by SSP
typedef enum
{
	DATA_SIZE_4 = 3,
	DATA_SIZE_5 = 4,
	DATA_SIZE_6 = 5,
	DATA_SIZE_7 = 6,
	DATA_SIZE_8 = 7,
	DATA_SIZE_9 = 8,
	DATA_SIZE_10 = 9,
	DATA_SIZE_11 = 10,
	DATA_SIZE_12 = 11,
	DATA_SIZE_13 = 12,
	DATA_SIZE_14 = 13,
	DATA_SIZE_15 = 14,
	DATA_SIZE_16 = 15
}t_SSPDataSize;

//Define the various Intefaces.
typedef enum
{
	MOTOROLA_SPI,
	TI_SYNC_SERIAL,
	NATIONAL_MICROWIRE
}t_SSPInterface;

//Define the various Polarities for Motorola SPI interface only.
typedef enum
{
	CLK_POL_HIGH		= 0x00000040,
	CLK_POL_LOW		= ~ CLK_POL_HIGH
}t_SSPClkPolarity;

//Define the Closk Phase for Motorola SPI interface only.
typedef enum
{
	CLK_RISING_EDGE		= 0x00000080,
	CLK_FALLING_EDGE	= ~ CLK_RISING_EDGE
}t_SSPClkPhase;

//Define the SSP Enable, Disable bits.
typedef enum
{
	SSP_ENABLE	= 0x00000002,
	SSP_DISABLE	= ~SSP_ENABLE
}t_SSPEnable;

//Define the SSP Mode Select bits.
typedef enum
{
	SSP_SLAVE	= 0x00000004,
	SSP_MASTER	= ~SSP_SLAVE
}t_SSPMode;

//Define the Slave Mode Output Disable bits.
typedef enum
{
	SLAVE_OUTPUT_DISABLE	= 0x00000008,
	SLAVE_OUTPUT_ENABLE	= ~SLAVE_OUTPUT_DISABLE
}t_SSPSlaveOutput;

//Define the Interrupts.
typedef enum
{
	SSP_RX_OR_INT	= 0x00000001,
	SSP_RX_TO_INT	= 0x00000002,
	SSP_RX_INT	= 0x00000004,
	SSP_TX_INT	= 0x00000008,
	INT_ALL		= 0x0000000f
}t_SSPIntType;

//Define the type used to describe the possible errors of that can occurin.
typedef enum
{
	SSP_OK = 0,
	SSP_INVALID_ID,
	SSP_INVALID_DATA_SIZE,
	SSP_INVALID_INTERFACE,
	SSP_INVALID_CLK_POLARITY,
	SSP_INVALID_CLK_PHASE,
	SSP_INVALID_MODE,
	SSP_INVALID_SLAVE_OUTPUT,
	SSP_INVALID_INT_STATUS,
	SSP_INVALID_FREQUENCY,
	SSP_INVALID_INT_TYPE,
	SSP_NO_DATA_TO_WRITE,
	SSP_NO_DATA_TO_READ,
	SSP_RX_ERROR,
	SSP_NULL_POINTER,
	SSP_INVALID_PARAMETER,
	SSP_BUSY,
	SSP_ERROR
}t_SSPError;

//Define Type defined for the Frame Format.
typedef struct
{
	t_SSPInterface Interface;
	t_SSPClkPolarity MotoClkPolarity;
	t_SSPClkPhase MotoClkPhase;
}t_SSPFrameFormat;

//Define the Type defined for Setting the configurations.
typedef struct
{
	t_SSPMode Mode;
	t_SSPFrameFormat FrameFormat;
	t_SSPDataSize DataSize;
	t_SSPSlaveOutput SlaveOutput;
	uint RequiredFrequency;
	uint AttainedFrequency;
}t_SSPConfigure;


//New Types used in SSP

/***********************************************************************************
 * PUBLIC Function
***********************************************************************************/
//This API is used to initialize the SSP.
t_SSPError SSP_Init(void);

//This API is used to Configure SSP.
t_SSPError SSP_Configure(t_SSPId SSPId, t_SSPConfigure *Config);

//This API is used to transmit Dummy Data.
void SSP_TransmitDummyData(t_SSPId SSPId, uint NumOfCharToTx);

//This API is used to transmit maximum number of characters.
t_SSPError SSP_TransmitMaxChar(t_SSPId SSPId, uint NumOfCharToTx, char *Buffer, uint *NumOfCharTx);

//This API is used to Receive maximum number of characters.
t_SSPError SSP_ReceiveMaxChar(t_SSPId SSPId, uint NumOfCharToRx, char *Buffer, uint *NumOfCharRx);

//This API is used to flush Rx fifo.
void SSP_FlushRxFIFO(t_SSPId SSPId);

//API's to Enable and Disable DMA.
t_SSPError SSP_DMAEnable(t_SSPId SSPId);
t_SSPError SSP_DMADisable(t_SSPId SSPId);

//API's for Interrupt Handling
t_SSPError SSP_IntEnable(t_SSPId SSPId, t_SSPIntType IntType);
t_SSPError SSP_IntDisable(t_SSPId SSPId, t_SSPIntType IntType);
t_SSPError SSP_IntClear(t_SSPId SSPId, t_SSPIntType IntType);
uint SSP_GetIntSrc(t_SSPId SSPId);

#endif // SPEAr_SSP_HAL_H
