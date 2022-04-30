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
#ifndef SPEAR_UART_HAL_H
#define SPEAR_UART_HAL_H


#include <asm/arch/spear.h>
#include <asm/arch/hardware.h>
#include <asm/arch/hcl_defs.h>
#include <linux/types.h>

//Total Number of UARTs
#ifdef CONFIG_ARCH_SPEARBASIC
#define UART_NUM 1 

#else
#define UART_NUM 2
#endif
//Max FIFO Size
#define UART0_FIFO_SIZE 16
#define UART1_FIFO_SIZE 16


//Base Address of UART Devices..
#ifdef CONFIG_ARCH_SPEARBASIC
#define SPEAR_UART0_APB_BASE VA_SPEARBASIC_START_APB_UART0
#define SPEAR_UART1_APB_BASE VA_SPEARBASIC_START_APB_UART1
#else
#define SPEAR_UART0_APB_BASE VA_SPEARPLUS_START_APB_UART0
#define SPEAR_UART1_APB_BASE VA_SPEARPLUS_START_APB_UART1
#endif

//UART Registers Base Address Mapping
#define UARTDR 		((volatile uint *)(SPEAR_UART_APB_BASE + 0x000))
#define UARTRSR	 	((volatile uint *)(SPEAR_UART_APB_BASE + 0x004))  
#define UARTECR		((volatile uint *)(SPEAR_UART_APB_BASE + 0x004))
#define UARTFR		((volatile uint *)(SPEAR_UART_APB_BASE + 0x018))  
#define UARTIBRD	((volatile uint *)(SPEAR_UART_APB_BASE + 0x024))  
#define UARTFBRD	((volatile uint *)(SPEAR_UART_APB_BASE + 0x028))  
#define UARTLCR_H	((volatile uint *)(SPEAR_UART_APB_BASE + 0x02c))  
#define UARTCR		((volatile uint *)(SPEAR_UART_APB_BASE + 0x030))  
#define UARTIFLS	((volatile uint *)(SPEAR_UART_APB_BASE + 0x034))  
#define UARTIMSC	((volatile uint *)(SPEAR_UART_APB_BASE + 0x038))  
#define UARTRIS		((volatile uint *)(SPEAR_UART_APB_BASE + 0x03c))  
#define UARTMIS		((volatile uint *)(SPEAR_UART_APB_BASE + 0x040))  
#define UARTICR		((volatile uint *)(SPEAR_UART_APB_BASE + 0x044))  
#define UARTDMACR	((volatile uint *)(SPEAR_UART_APB_BASE + 0x048))  


//Constants for configuring UART Registers

//UART Data Register.
typedef enum
{
	DATA_MASK 		= 0x000000ff,
	FRAMING_ERR_MASK 	= 0x00000100,
	PARITY_ERR_MASK 	= 0x00000200,
	BREAK_ERR_MASK 		= 0x00000400,
	OVERRUN_ERR_MASK 	= 0x00000800,
	ANY_ERR_MASK		= 0x00000f00
}t_UARTDataReg;

//UART Error Clear Register.
typedef enum
{
	FRAMING_ERR_CLEAR 	= 0x00000001,
	PARITY_ERR_CLEAR 	= 0x00000002,
	BREAK_ERR_CLEAR 	= 0x00000004,
	OVERRUN_ERR_CLEAR 	= 0x00000008
}t_UARTErrClearReg;

//UART Flag Register.
typedef enum
{
	UART_BUSY_MASK	 	= 0x00000008,
	RX_FIFO_EMPTY_MASK 	= 0x00000010,
	TX_FIFO_FULL_MASK 	= 0x00000020,
	RX_FIFO_FULL_MASK 	= 0x00000040,
	TX_FIFO_EMPTY_MASK 	= 0x00000080
}t_UARTFlagReg;

//UART Baud Rate Register 
typedef enum
{
	BAUD_INTEGER_MASK 	= 0x0000ffff,
	BAUD_FRACTION_MASK 	= 0x0000003f
}t_UARTBaudRateReg;

//Define the various FIFO States.
typedef enum
{
	FIFO_ENABLE		= 0x00000010,
	FIFO_DISABLE		= ~ FIFO_ENABLE
}t_UARTFIFOStatus;

//UART Line Control Register
typedef enum
{
	SEND_BREAK_ENABLE 	= 0x00000001,
	SEND_BREAK_DISABLE 	= ~ SEND_BREAK_ENABLE,
	PARITY_ENABLE	 	= 0x00000002,
	EVEN_PARITY		= 0x00000004,
	ODD_PARITY		= ~ EVEN_PARITY,
	TWO_STOP_BIT		= 0x00000008,
	ONE_STOP_BIT		= ~ TWO_STOP_BIT,
	WORD_LENGTH_RESET	= ~(0x00000060),
	WORD_LENGTH_5		= 0x00000000,
	WORD_LENGTH_6		= 0x00000020,
	WORD_LENGTH_7		= 0x00000040,
	WORD_LENGTH_8		= 0x00000060,

	STICK_PARITY_ENABLE	= 0x00000080,
	STICK_PARITY_DISABLE	= ~ STICK_PARITY_ENABLE
}t_UARTLineCtrlReg;

//UART Control Register
typedef enum
{
	UART_ENABLE 		= 0x00000001,
	UART_DISABLE 		= ~ UART_ENABLE,
	TX_ENABLE	 	= 0x00000100,
	TX_DISABLE	 	= ~ TX_ENABLE,
	RX_ENABLE	 	= 0x00000200,
	RX_DISABLE	 	= ~ RX_ENABLE
}t_UARTCtrlReg;

//UART Interrupt FIFO Level Select Register
typedef enum
{
	TX_FIFO_TRIG_RESET	= ~(0x00000007),
	TX_FIFO_TRIG_1BY8	= 0x00000000,
	TX_FIFO_TRIG_1BY4	= 0x00000001,
	TX_FIFO_TRIG_1BY2	= 0x00000002,
	TX_FIFO_TRIG_3BY4	= 0x00000003,
	TX_FIFO_TRIG_7BY8	= 0x00000004,

	RX_FIFO_TRIG_RESET	= ~(0x00000031),
	RX_FIFO_TRIG_1BY8	= 0x00000000,
	RX_FIFO_TRIG_1BY4	= 0x00000008,
	RX_FIFO_TRIG_1BY2	= 0x00000010,
	RX_FIFO_TRIG_3BY4	= 0x00000018,
	RX_FIFO_TRIG_7BY8	= 0x00000020
}t_UARTFIFOTrigReg;

//UART Interrupt Mask Register
typedef enum
{
	RX_INT_DISABLE		= 0x00000010,
	RX_INT_ENABLE		= ~ RX_INT_DISABLE,

	TX_INT_DISABLE		= 0x00000020,
	TX_INT_ENABLE		= ~ TX_INT_DISABLE,

	RX_TIMEOUT_INT_DISABLE	= 0x00000040,
	RX_TIMEOUT_INT_ENABLE	= ~ RX_TIMEOUT_INT_DISABLE,

	FRAMING_INT_DISABLE	= 0x00000080,
	FRAMING_INT_ENABLE	= ~ FRAMING_INT_DISABLE,

	PARITY_INT_DISABLE	= 0x00000100,
	PARITY_INT_ENABLE	= ~ PARITY_INT_DISABLE,

	BREAK_INT_DISABLE	= 0x00000200,
	BREAK_INT_ENABLE	= ~ BREAK_INT_DISABLE,

	OVERRUN_INT_DISABLE	= 0x00000400,
	OVERRUN_INT_ENABLE	= ~ OVERRUN_INT_DISABLE
}t_UARTIntEnableReg;

//UART Interrupt Status Register Masks.
typedef enum
{
	RX_INT_MASK		= 0x00000010,
	TX_INT_MASK		= 0x00000020,
	RX_TIMEOUT_INT_MASK	= 0x00000040,
	FRAMING_INT_MASK	= 0x00000080,
	PARITY_INT_MASK		= 0x00000100,
	BREAK_INT_MASK		= 0x00000200,
	OVERRUN_INT_MASK	= 0x00000400,
	INT_ALL_MASK		= 0x000007f0
}t_UARTIntStatusReg;

//UART Interrupt Clear Register.
typedef enum
{
	RX_INT_CLEAR		= 0x00000010,
	TX_INT_CLEAR		= 0x00000020,
	RX_TIMEOUT_INT_CLEAR	= 0x00000040,
	FRAMING_INT_CLEAR	= 0x00000080,
	PARITY_INT_CLEAR	= 0x00000100,
	BREAK_INT_CLEAR		= 0x00000200,
	OVERRUN_INT_CLEAR	= 0x00000400
}t_UARTIntClearReg;

//UART DMA Control Register.
typedef enum
{
	RX_DMA_ENABLE		= 0x00000001,
	RX_DMA_DISABLE		= ~ RX_DMA_ENABLE,
	TX_DMA_ENABLE		= 0x00000002,
	TX_DMA_DISABLE		= ~ TX_DMA_ENABLE,
	DMA_ON_ERROR_ENABLE	= 0x00000004,
	DMA_ON_ERROR_DISABLE	= ~ DMA_ON_ERROR_ENABLE
}t_UARTDmaCtrlReg;

//Define the various UART Device ID's.
typedef enum
{
	UART0,
	UART1,
	UART_ALL
}t_UARTId;

//Define the various BAUD Rates supported by UART
typedef enum
{
	BAUD110,
	BAUD300,
	BAUD1200,
	BAUD2400,
	BAUD4800,
	BAUD9600,
	BAUD19200,
	BAUD38400,
	BAUD57600,
	BAUD115200,
	BAUD230400,
	BAUD460800
}t_UARTBaudRate;

//Define the various Word Lengths supported per character transfer.
typedef enum
{
	DATA5,
	DATA6,
	DATA7,
	DATA8
}t_UARTWordLength;

//Define the various Stop Bits supported per character transfer.
typedef enum
{
	STOP1,
	STOP2
}t_UARTStopBitSelect;

//Define the various Parity types.
typedef enum
{
	ODD,
	EVEN,
	PARITY_DISABLE
}t_UARTParitySelect;

//Define the FIFO Levels/WaterMark Levels for generating interrupts.
typedef enum
{
	OneByEight,
	OneByFour,
	OneByTwo,
	ThreeByFour,
	SevenByEight
}t_UARTWatermarkLevel;

//Define the Receive Errors.
typedef enum
{
	FRAMING,
	PARITY,
	BREAK,
	OVERRUN,
	NO_ERROR
}t_UARTRxError;

//Define the Various Interrupt Types.
typedef enum
{
	RX_INT 		= 0x00000001,
	TX_INT 		= 0x00000002,
	RXTIMEOUT_INT 	= 0x00000004,
	FRAMING_INT 	= 0x00000008,
	PARITY_INT 	= 0x00000010,
	BREAK_INT 	= 0x00000020,
	OVERRUN_INT 	= 0x00000040,
	INT_ALL		= 0x0000007f
}t_UARTIntType;

//Define the type used to describe the possible errors of that can occurin.
typedef enum
{
	UART_OK = 0,
	UART_INVALID_ID,
	UART_INVALID_FIFO_STATUS,
	UART_INVALID_BAUD_RATE,
	UART_INVALID_WORD_LENGTH,
	UART_INVALID_STOP_BIT_SELECT,
	UART_INVALID_PARITY_SELECT,
	UART_INVALID_WATERMARK_LEVEL,
	UART_INVALID_PARAMETER,
	UART_NO_DATA_TO_WRITE,
	UART_NO_DATA_TO_READ,
	UART_INVALID_INT_TYPE,
	UART_NO_INT_ACTIVE,
	UART_NOT_INITIALIZED,
	UART_RX_ERROR,
	UART_NULL_POINTER,
	UART_ERROR
}t_UARTError;

//Define the Type defined for Setting the configurations.
typedef struct
{
	t_UARTFIFOStatus FIFOStatus;
	t_UARTBaudRate BaudRate;
	t_UARTWordLength WordLength;
	t_UARTStopBitSelect StopBitSelect;
	t_UARTParitySelect ParitySelect;
	t_UARTWatermarkLevel TxWatermarkLevel;
	t_UARTWatermarkLevel RxWatermarkLevel;
	t_bool TxEnable;
	t_bool RxEnable;
}t_UARTConfigure;



//New Types used in UART

//typedef for a pointer to a interrupt handler
typedef void (* t_UARTIntHandler)(void);


/***********************************************************************************
 * PUBLIC Functions
***********************************************************************************/
//This API is used to initialize the UART.
t_UARTError UART_Init(void);

//This API is used to Configure UART.
t_UARTError UART_Configure(t_UARTId UARTId, t_UARTConfigure Config);

//This API is used to transmit a number of characters.
t_UARTError UART_Transmit(t_UARTId UARTId, uint NumOfCharToTx, char *Buffer);

//This API is used to Receive a number of characters.
t_UARTError UART_Receive(t_UARTId UARTId, uint NumOfCharToRx, char *Buffer, uint *NumOfCharRx, t_UARTRxError *RxError);

//This API is used to Disable a UART.
t_UARTError UART_Disable(t_UARTId UARTId);

//This API is used to check wheather TxFIFO is Full.
t_bool UART_IsTxFIFOFull(t_UARTId UARTId);

//This API is used to check wheather TxFIFO is Empty.
t_bool  UART_IsTxFIFOEmpty(t_UARTId UARTId);

//This API is used to check wheather RxFIFO is Full.
t_bool UART_IsRxFIFOFull(t_UARTId UARTId);

//This API is used to check wheather RxFIFO is Empty.
t_bool UART_IsRxFIFOEmpty(t_UARTId UARTId);

//API's for Interrupt Handling
t_UARTError UART_IntEnable(t_UARTId UARTId, t_UARTIntType IntType);
t_UARTError UART_IntDisable(t_UARTId UARTId, t_UARTIntType IntType);
t_UARTError UART_IntClear(t_UARTId UARTId, t_UARTIntType IntType);
uint UART_GetIntSrc(t_UARTId UARTId);

#endif // SPEAR_UART_HAL_H
