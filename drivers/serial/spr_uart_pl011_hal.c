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
#include "spr_uart_pl011_hal.h"

/*------------------------------------------------------------------------
 * Global Variables 
 *----------------------------------------------------------------------*/

#ifndef NULL
#define NULL 0
#endif

#define UART_TOGGLE (SPEAR_UART_APB_BASE == SPEAR_UART0_APB_BASE)?(SPEAR_UART_APB_BASE = SPEAR_UART1_APB_BASE):(SPEAR_UART_APB_BASE = SPEAR_UART1_APB_BASE)

extern uint UART_CLOCK_TICK_RATE;

//UART Status Variables.
t_bool IsUART0Initialized = FALSE;
t_bool IsUART1Initialized = FALSE;

/****************************************************************************/
/* NAME: 		SetBaseDevice		                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Sets Uart Base Address				    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - UARTId: identify the UART Device	                            */
/*                                                                          */
/* OUT  :                                                                   */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_UARTError                                                       */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static t_UARTError SetBaseDevice(volatile uint *SPEAR_UART_APB_BASE, t_UARTId UARTId)
{
	//Setting base address of the UART
	if(UARTId == UART0)
		*SPEAR_UART_APB_BASE = SPEAR_UART0_APB_BASE;
	else if(UARTId == UART1)
		*SPEAR_UART_APB_BASE = SPEAR_UART1_APB_BASE;
	else
		return UART_INVALID_ID;

	return UART_OK;
}


/****************************************************************************/
/* NAME: 		GetBaudDivisor	            		            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine gives integer and fractional values register values    */
/*	of the Baud rate Divisor                                            */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :   BaudRate - Baud Rate to be configured Constant value             */
/*                                                                          */
/* OUT  :  IntegralVal - Baud Rate divisors integral value	            */
/*                                                                          */
/* RETURN: 	                                                            */
/* 	   FractionalVal - Baud Rate divisors fractional value	            */
/*         -1 on error							    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static uint GetBaudDivisor(t_UARTBaudRate BaudRate, uint *IntegralVal)
{
	unsigned long BaudRateActual, MultFactor = 1000, NewClockRate;

	//If NULL Pointer is sent then return error.
	if(IntegralVal == NULL)
		return UART_NULL_POINTER;

	//Setting the actual baud rate.
	switch(BaudRate)
	{
		case BAUD110:	BaudRateActual = 110;
			break;
		case BAUD300:	BaudRateActual = 300;
			break;
		case BAUD1200:	BaudRateActual = 1200;
			break;
		case BAUD2400:	BaudRateActual = 2400;
			break;
		case BAUD4800:	BaudRateActual = 4800;
			break;
		case BAUD9600:	BaudRateActual = 9600;
			break;
		case BAUD19200:	BaudRateActual = 19200;
			break;
		case BAUD38400:	BaudRateActual = 38400;
			break;
		case BAUD57600:	BaudRateActual = 57600;
			break;
		case BAUD115200:BaudRateActual = 115200;
			break;
		case BAUD230400:BaudRateActual = 230400;
			break;
		case BAUD460800:BaudRateActual = 460800;
			break;
		default: return -1;
	}

	//Finding the divisor value
	//NOTE:- As we do not have floating point in Kernel, we divide the computation in such a way, 
	//as to find the closest approximated value of floating point register. MultFactor is used for this purpose.

	//Finding the Integral Part.
	*IntegralVal = UART_CLOCK_TICK_RATE/(16*BaudRateActual);

	//Finding the Part of UART Clock responsible for the float part in the divisor.
	NewClockRate = UART_CLOCK_TICK_RATE - (*IntegralVal)*16*BaudRateActual;

	//Finding the Floating point register value to be configured.
	return ((((NewClockRate * MultFactor)/(16*BaudRateActual))*64 + 5*MultFactor/10)/MultFactor);
}


/****************************************************************************/
/* NAME: 		UART_Init	            		            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine configures UART with default settings and Enables it.  */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_UARTError                                                       */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_UARTError UART_Init()
{
	t_UARTConfigure Config;

	//Set the Default Settings.
	Config.FIFOStatus 	= FIFO_ENABLE;
	Config.BaudRate 	= BAUD115200;
	Config.WordLength 	= DATA8;
	Config.StopBitSelect 	= STOP2;
	Config.ParitySelect 	= EVEN;
	Config.TxWatermarkLevel = OneByTwo;
	Config.RxWatermarkLevel = OneByTwo;
	Config.TxEnable 	= TRUE;
	Config.RxEnable 	= TRUE;

	//Both the UART's are initialized with default settings configured.
	return(	UART_Configure(UART_ALL, Config));
}

/****************************************************************************/
/* 			 UART_Configure()	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine allows to configure a UART Device. It first disables   */
/*      the UART, then configures it and finally enables it again.          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - UARTId: Identify the UART Device	                            */
/*      - Config: Configuration Information.           	    		    */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_UARTError                                                       */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_UARTError UART_Configure(t_UARTId UARTId, t_UARTConfigure Config)
{
	uint IntegralBaud, FractionalBaud, LineCtrlReg = 0, CtrlReg = 0, FIFOTriggerLevelReg = 0, UARTCount = 0;
	volatile uint SPEAR_UART_APB_BASE;
	t_UARTError Status;
	t_UARTIntType IntType = 0;

	//Changing Status of UART.
	if(UARTId == UART0)
		IsUART0Initialized = FALSE;
	else if(UARTId == UART1)
		IsUART1Initialized = FALSE;
	else if(UARTId == UART_ALL)
	{
		IsUART0Initialized = FALSE;
		IsUART1Initialized = FALSE;
	}
	else
		return UART_INVALID_ID;

	//Disable UART.
	if((Status = UART_Disable(UARTId)) != UART_OK)
		return Status;

	//Finding Integral and Fractional Baud Rate
	if((FractionalBaud = GetBaudDivisor(Config.BaudRate, &IntegralBaud)) == -1)
		return UART_INVALID_BAUD_RATE;

	//Configuring LineCtrlReg 

	//Setting Parity Bits.
	switch(Config.ParitySelect)
	{
		case EVEN:
			LineCtrlReg = EVEN_PARITY | PARITY_ENABLE;
			break;
		case ODD:
			LineCtrlReg = PARITY_ENABLE; //It is already Zero, i.e. Odd Parity selected.
			break;
		case PARITY_DISABLE:
			//Already Disabled
			break;
		default:
			return UART_INVALID_PARITY_SELECT;
			break;
	}

	//Setting WordLength
	switch(Config.WordLength)
	{
		case DATA8:
			LineCtrlReg |= WORD_LENGTH_8;
			break;
		case DATA7:
			LineCtrlReg |= WORD_LENGTH_7;
			break;
		case DATA6:
			LineCtrlReg |= WORD_LENGTH_6;
			break;
		case DATA5:
			//Already Zero. i.e. DATA5
			break;
		default:
			return UART_INVALID_WORD_LENGTH;
	}

	//Setting Stop Bits. Already selected as zero. i.e. OneStopBit.
	if(Config.StopBitSelect == STOP2)
		LineCtrlReg |= TWO_STOP_BIT;
	else if(Config.StopBitSelect != STOP1)
		return UART_INVALID_STOP_BIT_SELECT;

	//Setting FIFO Status. Already selected as zero. i.e. Disabled.
	if(Config.FIFOStatus == FIFO_ENABLE)
		LineCtrlReg |= FIFO_ENABLE;
	else if(Config.FIFOStatus != FIFO_DISABLE)
		return UART_INVALID_FIFO_STATUS;


	//Configuring Control Register.
	if(Config.TxEnable & Config.RxEnable)
		CtrlReg = TX_ENABLE | RX_ENABLE;
	else 
	{
		if(Config.TxEnable)
			CtrlReg = TX_ENABLE;
		if(Config.RxEnable)
			CtrlReg = RX_ENABLE;
	}


	//Configuring Interrupt FIFO Level Select Register. Already set to 1/8 level.
	switch(Config.TxWatermarkLevel)
	{
		case OneByEight:
			break;
		case OneByFour:
			FIFOTriggerLevelReg = TX_FIFO_TRIG_1BY4;
			break;
		case OneByTwo:
			FIFOTriggerLevelReg = TX_FIFO_TRIG_1BY2;
			break;
		case ThreeByFour:
			FIFOTriggerLevelReg = TX_FIFO_TRIG_3BY4;
			break;
		case SevenByEight:
			FIFOTriggerLevelReg = TX_FIFO_TRIG_7BY8;
			break;
		default:
			return UART_INVALID_WATERMARK_LEVEL;
	}

	switch(Config.RxWatermarkLevel)
	{
		case OneByEight:
			break;
		case OneByFour:
			FIFOTriggerLevelReg |= RX_FIFO_TRIG_1BY4;
			break;
		case OneByTwo:
			FIFOTriggerLevelReg |= RX_FIFO_TRIG_1BY2;
			break;
		case ThreeByFour:
			FIFOTriggerLevelReg |= RX_FIFO_TRIG_3BY4;
			break;
		case SevenByEight:
			FIFOTriggerLevelReg |= RX_FIFO_TRIG_7BY8;
			break;
		default:
			return UART_INVALID_WATERMARK_LEVEL;
	}

	switch(UARTId)
	{
		case UART0:
			UARTCount = 1;
			//Setting Base Address of Registers
			SPEAR_UART_APB_BASE = SPEAR_UART0_APB_BASE;
			break;
		case UART1:
			UARTCount = 1;
			//Setting Base Address of Registers
			SPEAR_UART_APB_BASE = SPEAR_UART1_APB_BASE;
			break;
		case UART_ALL:
			UARTCount = 2;
			//Setting a starting value in case all the UART's are required to be configured
			SPEAR_UART_APB_BASE = SPEAR_UART0_APB_BASE;
			break;
	}

	//While the all the UART's required to be configured are configured.
	while(UARTCount > 0)
	{
		//Decrementing count.
		UARTCount--;

		//Configuring the Registers.
		//Clearing Errors if any.
		*UARTECR = 0x0000000f;

		//Setting Baud Rates.
		*UARTIBRD = IntegralBaud & BAUD_INTEGER_MASK;
		*UARTFBRD = FractionalBaud & BAUD_FRACTION_MASK;

		//Setting Line Control Register.
		*UARTLCR_H = LineCtrlReg;

		//Setting Control Register.
		*UARTCR	= CtrlReg;

		//Setting Interrupt FIFO Level Select Register.
		*UARTIFLS = FIFOTriggerLevelReg;

		//Clearing all the interrupts.
		*UARTICR  = 0x000007f0;

		//Enabling the UART.
		*UARTCR	= *UARTCR | UART_ENABLE;

		//Toggle UART Base address to select the other uart.
		UART_TOGGLE;
	}

	//Selecting the Interrupts to be Disabled.
	IntType = TX_INT | FRAMING_INT | PARITY_INT | BREAK_INT | OVERRUN_INT;

	//Changing Status of UART.
	if(UARTId == UART0)
	{
		UART_IntDisable(UART0, IntType);
		IsUART0Initialized = TRUE;
	}
	else if(UARTId == UART1)
	{
		UART_IntDisable(UART1, IntType);
		IsUART1Initialized = TRUE;
	}
	else
	{
		UART_IntDisable(UART0, IntType);
		UART_IntDisable(UART1, IntType);
		IsUART0Initialized = TRUE;
		IsUART1Initialized = TRUE;
	}

	return UART_OK;
}


/****************************************************************************/
/* NAME: 		UART_Transmit	                                    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine allows to start transmit x number of bytes		    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - UARTId: identify the UART Device	                            */
/*      - NumOfCharToTx: This is the number of characters to be transmitted.*/
/*                                                                          */
/* OUT  :                                                                   */
/*      - Buffer: This is the pointer to the characters to be transmitted.  */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_UARTError                                                       */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_UARTError UART_Transmit(t_UARTId UARTId, uint NumOfCharToTx, char *Buffer)
{
	volatile uint SPEAR_UART_APB_BASE;
	uint Index = 0;

	//If Number of Characters to be transmitted is zero.
	if(!NumOfCharToTx)
		return UART_NO_DATA_TO_WRITE;

	//Setting Base Address and Changing Status of UART.
	if(UARTId == UART0)
	{
		SPEAR_UART_APB_BASE = SPEAR_UART0_APB_BASE;
		if(IsUART0Initialized == FALSE)
			return UART_NOT_INITIALIZED;
	}
	else if(UARTId == UART1)
	{
		SPEAR_UART_APB_BASE = SPEAR_UART1_APB_BASE;
		if(IsUART1Initialized == FALSE)
			return UART_NOT_INITIALIZED;
	}
	else
		return UART_INVALID_ID;

	//If Buffer Pointer Sent is null.
	if(Buffer == NULL)
		return UART_NULL_POINTER;

	//Transmit data.
	while(Index<NumOfCharToTx)
	{
		//Wait while Tx is FULL.
		while(UART_IsTxFIFOFull(UARTId));

		//Write Character by Character.
		*UARTDR = *(Buffer + Index);
		Index++;
	}

	return UART_OK;
}

/****************************************************************************/
/* NAME: 		UART_Receive	                                    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine allows to start receive x number of bytes		    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - UARTId: identify the UART Device	                            */
/*      - NumOfCharToRx: This is the number of characters to be Received.   */
/*                                                                          */
/* OUT  :                                                                   */
/*      - Buffer: This is the pointer to the buffer where the data is stored*/
/*      - NumOfCharRx: This is the number of characters Received before     */
/*		       error is encountered.   				    */
/*      - RxError: This is the Error occured during the transmission.	    */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_UARTError                                                       */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_UARTError UART_Receive(t_UARTId UARTId, uint NumOfCharToRx, char *Buffer, uint *NumOfCharRx, t_UARTRxError *RxError)
{
	volatile uint SPEAR_UART_APB_BASE;
	uint CharReceived;
	t_bool ErrorStatus = FALSE;

	//If Number of Characters to be received is zero.
	if(!NumOfCharToRx)
		return UART_NO_DATA_TO_READ;

	//Setting Base Address and Changing Status of UART.
	if(UARTId == UART0)
	{
		SPEAR_UART_APB_BASE = SPEAR_UART0_APB_BASE;
		if(IsUART0Initialized == FALSE)
			return UART_NOT_INITIALIZED;
	}
	else if(UARTId == UART1)
	{
		SPEAR_UART_APB_BASE = SPEAR_UART1_APB_BASE;
		if(IsUART1Initialized == FALSE)
			return UART_NOT_INITIALIZED;
	}
	else
		return UART_INVALID_ID;

	//If Pointers passed are null.
	if((Buffer == NULL)||(NumOfCharRx == NULL)||(RxError == NULL))
		return UART_NULL_POINTER;

	//Setting Number of characters received as 0.
	*NumOfCharRx = 0;

	//Receive data.
	while(*NumOfCharRx<NumOfCharToRx)
	{
		//Wait while Rx is EMPTY.
		while(UART_IsRxFIFOEmpty(UARTId));

		//Read Character by Character.
		CharReceived = *UARTDR;

		//Check if any error has occured during reception.
		if(CharReceived & ANY_ERR_MASK)
		{
			if(CharReceived & FRAMING_ERR_MASK)
				*RxError = FRAMING;
			else if(CharReceived & PARITY_ERR_MASK)
				*RxError = PARITY;
			else if(CharReceived & BREAK_ERR_MASK)
				*RxError = BREAK;
			else if(CharReceived & OVERRUN_ERR_MASK)
				*RxError = OVERRUN;
			ErrorStatus = TRUE;
		}
		else
		{
			*RxError = NO_ERROR;
		}

		//write received character into the Rx Buffer.
		*(Buffer + *NumOfCharRx) = CharReceived & DATA_MASK;
		(*NumOfCharRx)++;
	}

	if(ErrorStatus)
		return UART_RX_ERROR;

	return UART_OK;
}


/****************************************************************************/
/* NAME: 		UART_Disable	                                    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine allows to disable UART and stop transmission or 	    */
/*      reception after the transfer of the current  byte.		    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - UARTId: identify the UART Device	                            */
/*                                                                          */
/* OUT  :                                                                   */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_UARTError                                                       */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_UARTError UART_Disable(t_UARTId UARTId)
{
	volatile uint SPEAR_UART_APB_BASE;
	int UARTCount;

	//Setting Base Address of UART.
	switch(UARTId)
	{
		case UART0:
			UARTCount = 1;
			//Setting Base Address of Registers
			SPEAR_UART_APB_BASE = SPEAR_UART0_APB_BASE;
			break;
		case UART1:
			UARTCount = 1;
			//Setting Base Address of Registers
			SPEAR_UART_APB_BASE = SPEAR_UART1_APB_BASE;
			break;
		case UART_ALL:
			UARTCount = 2;
			//Setting a starting value in case all the UART's are required to be configured
			SPEAR_UART_APB_BASE = SPEAR_UART0_APB_BASE;
			break;
		default:
			return UART_INVALID_ID;
	}
	//While the all the UART's required to be disabled are disabled.
	while(UARTCount>0)
	{
		//Decrementing count.
		UARTCount--;

		//Disable UART.
		*UARTCR = *UARTCR &  UART_DISABLE;

		//Flush the Tx FIFO.
		*UARTLCR_H = *UARTLCR_H & FIFO_DISABLE;

		//Toggle UART Base address to select the other uart.
		UART_TOGGLE;
	}

	return UART_OK;
}

/****************************************************************************/
/* NAME: 		UART_IsTxFIFOFull	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine tells that the Tx FIFO is Full or not		    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - UARTId: identify the UART Device	                            */
/*                                                                          */
/* OUT  :                                                                   */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_UARTError                                                       */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_bool UART_IsTxFIFOFull(t_UARTId UARTId)
{
	volatile uint SPEAR_UART_APB_BASE;

	//Set Base address of UART
	SetBaseDevice(&SPEAR_UART_APB_BASE, UARTId);

	//Finding that Tx FIFO is Full or not.
	return (((*UARTFR) & TX_FIFO_FULL_MASK)? TRUE:FALSE);
}

/****************************************************************************/
/* NAME: 		UART_IsTxFIFOEmpty	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine tells that the Tx FIFO is empty or not		    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - UARTId: identify the UART Device	                            */
/*                                                                          */
/* OUT  :                                                                   */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_UARTError                                                       */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_bool UART_IsTxFIFOEmpty(t_UARTId UARTId)
{
	volatile uint SPEAR_UART_APB_BASE;

	//Set Base address of UART
	SetBaseDevice(&SPEAR_UART_APB_BASE, UARTId);

	//Finding that Tx FIFO is empty or not.
	return (((*UARTFR) & TX_FIFO_EMPTY_MASK)? TRUE:FALSE);
}

/****************************************************************************/
/* NAME: 		UART_IsRxFIFOFull	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine tells that the Rx FIFO is Full or not		    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - UARTId: identify the UART Device	                            */
/*                                                                          */
/* OUT  :                                                                   */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_UARTError                                                       */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_bool UART_IsRxFIFOFull(t_UARTId UARTId)
{
	volatile uint SPEAR_UART_APB_BASE;

	//Set Base address of UART
	SetBaseDevice(&SPEAR_UART_APB_BASE, UARTId);

	//Finding that Rx FIFO is Full or not.
	return (((*UARTFR) & RX_FIFO_FULL_MASK)? TRUE:FALSE);
}

/****************************************************************************/
/* NAME: 		UART_IsRxFIFOEmpty	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine tells that the Rx FIFO is empty or not		    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - UARTId: identify the UART Device	                            */
/*                                                                          */
/* OUT  :                                                                   */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_UARTError                                                       */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_bool UART_IsRxFIFOEmpty(t_UARTId UARTId)
{
	volatile uint SPEAR_UART_APB_BASE;

	//Set Base address of UART
	SetBaseDevice(&SPEAR_UART_APB_BASE, UARTId);

	//Finding that Rx FIFO is empty or not.
	return (((*UARTFR) & RX_FIFO_EMPTY_MASK)? TRUE:FALSE);
}

/********************************************************************************/
/* NAME:           UART_IntEnable						*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This API is used to Enable Interrupt on the specified UART.	 		*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  :                                                     	                */
/*      - UARTId: identify the UART Device	                            	*/
/*      - IntType: identify the Interrupt Type. All the interrupts required   	*/
/*      	   to be enabled should be ORed first and then passed into this */
/*      	   argument							*/
/*                                                                          	*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: t_UARTError							    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
t_UARTError UART_IntEnable(t_UARTId UARTId, t_UARTIntType IntType)
{
	volatile uint SPEAR_UART_APB_BASE;
	t_UARTError Status;

	//setting base Address of the UART.
	if((Status = SetBaseDevice(&SPEAR_UART_APB_BASE, UARTId)) != UART_OK)
		return Status;

	//If IntType is incorrect.
	if((IntType > 0x0000007f) ||(IntType < RX_INT))
		return UART_INVALID_INT_TYPE;

	//Enabling Interrupts
	*UARTIMSC = *UARTIMSC | (IntType<<4);

	return UART_OK;
}

/********************************************************************************/
/* NAME:           UART_IntDisable						*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This API is used to Disable Interrupt on the specified UART.	 		*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  :                                                     	                */
/*      - UARTId: identify the UART Device	                            	*/
/*      - IntType: identify the Interrupt Type. All the interrupts required   	*/
/*      	   to be disabled should be ORed first and then passed into this*/
/*      	   argument							*/
/*                                                                          	*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: t_UARTError							    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
t_UARTError UART_IntDisable(t_UARTId UARTId, t_UARTIntType IntType)
{
	volatile uint SPEAR_UART_APB_BASE;
	t_UARTError Status;

	//setting base Address of the UART.
	if((Status = SetBaseDevice(&SPEAR_UART_APB_BASE, UARTId)) != UART_OK)
		return Status;

	//If IntType is incorrect.
	if((IntType > 0x0000007f) ||(IntType < RX_INT))
		return UART_INVALID_INT_TYPE;

	//Disabling Interrupts
	*UARTIMSC = *UARTIMSC & (~(IntType<<4));

	return UART_OK;
}

/********************************************************************************/
/* NAME:           UART_IntClear						*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This API is used to Clear Interrupt of the specified UART.	 		*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  :                                                     	                */
/*      - UARTId: identify the UART Device	                            	*/
/*      - IntType: identify the Interrupt Type. All the interrupts required   	*/
/*      	   to be cleared should be ORed first and then passed into this */
/*      	   argument							*/
/*                                                                          	*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: t_UARTError							    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
t_UARTError UART_IntClear(t_UARTId UARTId, t_UARTIntType IntType)
{
	volatile uint SPEAR_UART_APB_BASE;
	t_UARTError Status;

	//setting base Address of the UART.
	if((Status = SetBaseDevice(&SPEAR_UART_APB_BASE, UARTId)) != UART_OK)
		return Status;

	//If IntType is incorrect.
	if((IntType > 0x0000007f) ||(IntType < RX_INT))
		return UART_INVALID_INT_TYPE;

	//Clearing Interrupts
	*UARTICR = *UARTICR |(IntType<<4);

	return UART_OK;
}

/********************************************************************************/
/* NAME:           UART_GetIntSrc						*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This API is used to Get the source of Interrupt.		 		*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  :                                                     	                */
/*      - UARTId: identify the UART Device	                            	*/
/*                                                                          	*/
/* OUT : 				    					*/
/*									    	*/
/* RETURN: uint representing the interrupts				    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
uint UART_GetIntSrc(t_UARTId UARTId)
{
	volatile uint SPEAR_UART_APB_BASE;
	t_UARTError Status;

	//setting base Address of the UART.
	if((Status = SetBaseDevice(&SPEAR_UART_APB_BASE, UARTId)) != UART_OK)
		return Status;

	//Return the Active Interrupts.
	return(((*UARTMIS) & INT_ALL_MASK) >> 4);
}
