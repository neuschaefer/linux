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
#include "spr_uart_pl011.h"


#define SPEAR_UART_NAME		"ttyS"
#define SPEAR_UART_MAJOR	MAJOR_UART
#define SPEAR_UART_MINOR	64
#define SPEAR_UART_NR		UART_NUM
#define PORT_SPEAR		123
#define __DRIVER_NAME		"ST Microelectronics SPEAR UART -- CPG "
#define GET_UART_ID(membase) 	((membase == (unsigned char *) VA_SPEAR_START_APB_UART0)? UART0 :UART1)
#define CONFIG_SERIAL_SPEAR_CONSOLE 1

uint UART_CLOCK_TICK_RATE = CLOCK_TICK_RATE;
char *CarriageReturn = "\r";

//Spin Lock Variable
static spinlock_t UART0_Spin_Lock, UART1_Spin_Lock;


/****************************************************************************/
/* 			 ReceiveMaxChar                  	            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine receives max number of characters possible.	    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :   Port:	Identify UART		                	    */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_UARTError                                                       */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_UARTError ReceiveMaxChar(struct uart_port * Port)
{
	unsigned int MaxCount = 256, Flag = TTY_NORMAL;
	char RxChar;
	uint NumOfCharToRx = 1, NumOfCharRx;
	t_UARTRxError RxError;
	t_UARTError Status;
	struct tty_struct *Tty = Port->info->tty;

	//Get UART Id
	t_UARTId UARTId = GET_UART_ID(Port->membase);

	//Receive Data while RxFifo is not empty and MaxCount is less than 256
	while (!(UART_IsRxFIFOEmpty(UARTId)) && MaxCount--) 
	{
		#if 0
		if (Tty->flip.count >= TTY_FLIPBUF_SIZE)  
		{
			Tty->flip.work.func((void *)Tty);

			if (Tty->flip.count >= TTY_FLIPBUF_SIZE) 
			{
				printk("TTY_DONT_FLIP set\n");
				return UART_ERROR;
			}
		}
		#endif 

		//Receive a single byte.
		Status = SPEAR_UART_Receive(UARTId, NumOfCharToRx, &RxChar, &NumOfCharRx, &RxError);
		//Write the read byte to the Kernel buffer.
		//*Tty->flip.char_buf_ptr = RxChar;

		//Increment the Error count if any.
		switch(RxError)
		{
			case NO_ERROR:
				Flag = TTY_NORMAL;
				break;
			case FRAMING:
				Port->icount.frame++;
				Flag = TTY_FRAME;
				break;
			case PARITY:
				Port->icount.parity++;
				Flag = TTY_PARITY;
				break;
			case BREAK:
				Flag = TTY_BREAK;
				break;
			case OVERRUN:
				Port->icount.overrun++;
				Flag = TTY_OVERRUN;
				RxChar =0;
				break;
		}

		Port->icount.rx++;
	
		tty_insert_flip_char(Tty, RxChar, Flag);	
	}
	tty_flip_buffer_push(Tty);
	return UART_OK;
}

/****************************************************************************/
/* 			 TransmitMaxChar                  	            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine transmits max number of characters possible.	    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :   Port:	Identify UART		                	    */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_UARTError                                                       */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_UARTError TransmitMaxChar(struct uart_port *Port)
{
	struct circ_buf *CircBuf = &Port->info->xmit;
	t_UARTError Status;

	//Get UART Id
	t_UARTId UARTId = GET_UART_ID(Port->membase);

	//Transmit while TxFIFO is not full and circular buffer is not empty.
	while((!UART_IsTxFIFOFull(UARTId)) & (!uart_circ_empty(CircBuf)))
	{
		//Transmit a carriage return '\r' before a '\n' character. so that the prompt comes to start of line.
		if(CircBuf->buf[CircBuf->tail] == '\n')
		{
			if((Status = SPEAR_UART_Transmit(UARTId, 1, CarriageReturn)) != UART_OK)
				return Status;
		}

		//Transmit the original character.
		if((Status = SPEAR_UART_Transmit(UARTId, 1,(char *)&(CircBuf->buf[CircBuf->tail]))) != UART_OK)
			return Status;

		//Increment tail value.
		CircBuf->tail = (CircBuf->tail + 1) & (UART_XMIT_SIZE  - 1);
		Port->icount.tx++;

	}

	//Check if any characters are pending.
	if (uart_circ_chars_pending(CircBuf) < WAKEUP_CHARS)
		uart_write_wakeup(Port);

	//Check if circular buffer is empty.
	if (uart_circ_empty(CircBuf))
		SPEAR_UART_TxStop(Port);

	return UART_OK;
}


/****************************************************************************/
/* 			 UART_LockUnlock()                 	            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Allows to spin lock or Unlock a UART Device.	    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - UARTId: Identify the UART Device	                            */
/*      - LockStatus: Lock Status, Lock or Unlock                           */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_UARTError                                                       */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static t_UARTError UART_LockUnlock(t_UARTId UARTId, t_LockStatus LockStatus)
{
	//Select Lock or Unlock
	switch(LockStatus)
	{
		case LOCK:
			//Select Uart to be spin locked.
			switch(UARTId)
			{
				//START SPIN LOCK.
				case UART0:
					spin_lock(&UART0_Spin_Lock);
					break;
				case UART1:
					spin_lock(&UART1_Spin_Lock);
					break;
				default:
					return UART_INVALID_ID;
			}
			break;
		case UNLOCK:
			//Select Uart to be spin unlocked.
			switch(UARTId)
			{
				//END SPIN LOCK.
				case UART0:
					spin_unlock(&UART0_Spin_Lock);
					break;
				case UART1:
					spin_unlock(&UART1_Spin_Lock);
					break;
				default:
					return UART_INVALID_ID;
			}
			break;
		default:
			return UART_INVALID_PARAMETER;
	}
	return UART_OK;
}


/****************************************************************************/
/* 			 SPEAR_UART_Configure()	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine allows to configure a UART Device. It calls 	    */
/*      UART_Configure of HAL Layer				            */
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
t_UARTError SPEAR_UART_Configure(t_UARTId UARTId, t_UARTConfigure Config)
{
	t_UARTError Status, Ret;

	//START SPIN LOCK.
	if((Ret = UART_LockUnlock(UARTId, LOCK)) != UART_OK)
		return Ret;

	//Configure UART.
	Status = UART_Configure(UARTId, Config);

	//END SPIN LOCK.
	if((Ret = UART_LockUnlock(UARTId, UNLOCK)) != UART_OK)
		return Ret;

	return Status;
}


/****************************************************************************/
/* NAME: 		SPEAR_UART_Transmit	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine allows to start transmit x number of bytes. It calls   */
/*      UART_Transmit of HAL Layer				            */
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
t_UARTError SPEAR_UART_Transmit(t_UARTId UARTId, uint NumOfCharToTx, char *Buffer)
{
	t_UARTError Status, Ret;

	//START SPIN LOCK.
	if((Ret = UART_LockUnlock(UARTId, LOCK)) != UART_OK)
		return Ret;

	//Transmit data.
	Status = UART_Transmit(UARTId, NumOfCharToTx, Buffer);

	//END SPIN LOCK.
	if((Ret = UART_LockUnlock(UARTId, UNLOCK)) != UART_OK)
		return Ret;

	return Status;
}

/****************************************************************************/
/* NAME: 		SPEAR_UART_Receive	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine allows to start receive x number of bytes.	It calls    */
/*      UART_Receive of HAL Layer				            */
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
t_UARTError SPEAR_UART_Receive(t_UARTId UARTId, uint NumOfCharToRx, char *Buffer, uint *NumOfCharRx, t_UARTRxError *RxError)
{
	t_UARTError Status, Ret;

	//START SPIN LOCK.
	if((Ret = UART_LockUnlock(UARTId, LOCK)) != UART_OK)
		return Ret;

	//Receive Data.
	Status = UART_Receive(UARTId, NumOfCharToRx, Buffer, NumOfCharRx, RxError);

	//END SPIN LOCK.
	if((Ret = UART_LockUnlock(UARTId, UNLOCK)) != UART_OK)
		return Ret;

	return Status;
}

/****************************************************************************/
/* NAME: 		SPEAR_UART_Disable	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine allows to disable UART and stop transmission or 	    */
/*      reception after the transfer of the current  byte.		    */
/*      It calls UART_Disable of HAL Layer.				    */
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
t_UARTError SPEAR_UART_Disable(t_UARTId UARTId)
{
	t_UARTError Status, Ret;

	//START SPIN LOCK.
	if((Ret = UART_LockUnlock(UARTId, LOCK)) != UART_OK)
		return Ret;

	//Disable UART.
	Status = UART_Disable(UARTId);

	//END SPIN LOCK.
	if((Ret = UART_LockUnlock(UARTId, UNLOCK)) != UART_OK)
		return Ret;

	return Status;
}
/****************************************************************************/
/* NAME:   		SPEAR_UART_IntHandler				    */
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
static irqreturn_t SPEAR_UART_IntHandler(int Irq, void *Dev_id, struct pt_regs *Reg)
{
	struct uart_port *Port = Dev_id;
	volatile uint IntSrc = 0;

	//Get UART Id
	t_UARTId UARTId = GET_UART_ID(Port->membase);

	//Get source of interrupt.
	IntSrc = UART_GetIntSrc(UARTId);

	//Call appropriate function according to the interrupt type.
	while(IntSrc)
	{
		//If Transmit interrupt is active.
		if(IntSrc & TX_INT)
		{
			//Disable the interrupt bit of IntSrc variable.
			IntSrc &= (~TX_INT);

			//Transmit Data.
			if(TransmitMaxChar(Port))
				printk("\nError in Transmit Max Char....\n");

			//Clear the interrupt
			UART_IntClear(UARTId, TX_INT);
		}
		//If Receive interrupt is active.
		if(IntSrc & (RX_INT | FRAMING_INT | PARITY_INT | BREAK_INT | OVERRUN_INT))
		{
			//Disable the interrupt bit of IntSrc variable.
			IntSrc &= (~(RX_INT | FRAMING_INT | PARITY_INT | BREAK_INT | OVERRUN_INT));

			//Receive Data
			if(ReceiveMaxChar(Port))
				printk("\nError in Receive Max Char.\n");

			//Clear the interrupt
			UART_IntClear(UARTId, RX_INT);
		}
		//If receive timeout interrupt is active.
		if(IntSrc & RXTIMEOUT_INT)
		{
			//Disable the interrupt bit of IntSrc variable.
			IntSrc &= (~RXTIMEOUT_INT);

			//Receive Data
			if(ReceiveMaxChar(Port))
				printk("\nError in Receive Max Char.\n");

			//Clear the interrupt
			UART_IntClear(UARTId, RXTIMEOUT_INT);
		}
	}

	return IRQ_HANDLED;
}

/****************************************************************************/
/* NAME:   		SPEAR_UART_IsTxEmpty				    */
/*                                       				    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*	This function tests whether the transmitter fifo and shifter	    */
/*	for the port described by 'port' is empty.  If it is empty,	    */
/*	this function should return TIOCSER_TEMT, otherwise return 0.	    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :   Port:	Identify UART		                	    */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: u_int				                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

static u_int SPEAR_UART_IsTxEmpty(struct uart_port *Port)
{
	//Delay added to lesser the kernel delay.....
	switch(Port->info->tty->termios->c_cflag & CBAUD)
	{
		case B110:	msleep(3500);   break;
		case B300:	msleep(600);   break;
		case B1200:	msleep(150);   break;
		case B2400:	msleep(80);   break;
		case B4800:
		case B9600:	msleep(50);   break;
		case B19200:
		case B38400:
		case B57600:
		case B230400:
		case B460800:
		case B115200:
		default:		msleep(10); break;
	}

	return(UART_IsTxFIFOEmpty(GET_UART_ID(Port->membase))?TIOCSER_TEMT:0);
}

/****************************************************************************/
/* NAME:  SPEAR_UART_SetMctrl 						    */
/*                                              			    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*	This function sets the modem control lines for port described	    */
/*	by 'port' to the state described by mctrl.			    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :   Port:	Identify UART		                	    */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: 					                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void SPEAR_UART_SetMctrl(struct uart_port *Port, u_int Mctrl)
{
}

/****************************************************************************/
/* NAME:  		SPEAR_UART_GetMctrl           			    */
/*                                           				    */  
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*	Returns the current state of modem control inputs.  The state	    */
/*	of the outputs should not be returned, since the core keeps	    */
/*	track of their state.						    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :   Port:	Identify UART		                	    */
/*                                                                          */
/* RETURN: 					                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static u_int SPEAR_UART_GetMctrl(struct uart_port *Port)
{
	return 0;
}


/****************************************************************************/
/* NAME:    SPEAR_UART_TxStop                                               */
/*                                                                          */  
/*--------------------------------------------------------------------------*/
/* DESCRIPTION: Stops the tx of data                                        */
/*                    						 	    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :   Port:	Identify UART		                	    */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: t_uart_error :  UART error code	                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void SPEAR_UART_TxStop(struct uart_port *Port/*, u_int From_tty*/)
{
	//Get UART Id
	t_UARTId UARTId = GET_UART_ID(Port->membase);

	UART_LockUnlock(UARTId, LOCK);

	//Disable Interrupts.
	UART_IntDisable(UARTId, TX_INT);

	UART_LockUnlock(UARTId, UNLOCK);
}

/****************************************************************************/
/* NAME: SPEAR_UART_TxStart                                                 */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*  Starts transmission of data                                             */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :   Port:	Identify UART		                	    */
/* IN  :   u_int from_tty                                                   */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: Void                                                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void SPEAR_UART_TxStart(struct uart_port *Port/*, u_int From_tty*/)
{
	t_UARTIntType IntType;

	//Get UART Id
	t_UARTId UARTId = GET_UART_ID(Port->membase);

	//Select Interrupt type
	IntType = TX_INT;

	//Transmit max data.
	if(TransmitMaxChar(Port))
		printk("\nError While Transmitting characters\n");

	UART_LockUnlock(UARTId, LOCK);

	//Enable Interrupts.
	UART_IntEnable(UARTId, IntType);

	UART_LockUnlock(UARTId, UNLOCK);
}


/****************************************************************************/
/* NAME:     SPEAR_UART_RxStop                                              */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine configures UART registers and checks                   */
/*      It also flush Tx and Rx fifo to enable a new transfer               */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :   Port:	Identify UART		                	    */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: t_uart_error :  UART error code	                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void SPEAR_UART_RxStop(struct uart_port *Port)
{
	t_UARTIntType IntType;

	//Get UART Id
	t_UARTId UARTId = GET_UART_ID(Port->membase);

	//Select Interrupt type
	IntType = RX_INT | FRAMING_INT | PARITY_INT | BREAK_INT | OVERRUN_INT | RXTIMEOUT_INT;

	UART_LockUnlock(UARTId, LOCK);

	//Disable Interrupts.
	UART_IntDisable(UARTId, IntType);

	UART_LockUnlock(UARTId, UNLOCK);
}

/****************************************************************************/
/* NAME:  	SPEAR_UART_EnableMS                                         */
/*                                                                          */  
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*          Enable the modem status interrupts.                             */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :   Port:	Identify UART		                	    */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: 					                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void SPEAR_UART_EnableMS(struct uart_port *Port)
{
}

/****************************************************************************/
/* NAME: 		 SPEAR_UART_BreakCtl 				    */
/*                                               			    */  
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      Control the transmission of a break signal.  If ctl is              */
/*      nonzero, the break signal should be transmitted.  The signal        */
/*      should be terminated when another call is made with a zero ctl.     */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :   Port:	Identify UART		                	    */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: 					                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void SPEAR_UART_BreakCtl(struct uart_port *Port, int BreakState)
{
}

/****************************************************************************/
/* NAME:  		SPEAR_UART_Startup 				    */
/*                                                 			    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION: It is called at the time the port is opened. It registers   */
/*              Interrupt and initializes UART with default configurations. */
/*              It Also Enables the Receive Time Interrupts.                */
/*                                                                          */
/* PARAMETERS:                                                              */
/*                                                                          */
/* IN  :   Port:	Identify UART		                	    */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/* RETURN: int					                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int SPEAR_UART_Startup(struct uart_port *Port)
{
	int Retval;
	t_UARTConfigure Config;
	t_UARTError Ret;

	//Select Interrupt type
	t_UARTIntType IntType = RX_INT | RXTIMEOUT_INT;

	//Get UART Id
	t_UARTId UARTId = GET_UART_ID(Port->membase);

	//request Interrupt
	if((Retval = request_irq (Port->irq, (void *)SPEAR_UART_IntHandler, 0, "SPEAR_uart", Port)) != 0)
	{
		return Retval;
	}

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

	//Configure UART.
	SPEAR_UART_Configure(UARTId, Config);

	if((Ret = UART_LockUnlock(UARTId, LOCK)) != UART_OK)
		return Ret;

	//Enable Interrupts.
	UART_IntEnable(UARTId, IntType);

	if((Ret = UART_LockUnlock(UARTId, UNLOCK)) != UART_OK)
		return Ret;

	return 0;
}

/****************************************************************************/
/* NAME: 		SPEAR_UART_Shutdown 				    */
/*                                              			    */  
/*--------------------------------------------------------------------------*/
/* DESCRIPTION: It is called at the time the port is closed. 	  	    */
/*		It Disables all the interrupts.	                	    */
/*                                                                          */
/* PARAMETERS:                                                              */
/*                                                                          */
/* IN  :   Port:	Identify UART		                	    */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/* RETURN:  void				                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

static void SPEAR_UART_Shutdown(struct uart_port *Port)
{
	t_UARTIntType IntType;

	//Get UART Id
	t_UARTId UARTId = GET_UART_ID(Port->membase);

	//Select Interrupt type
	IntType = RX_INT | FRAMING_INT | TX_INT | PARITY_INT | BREAK_INT | OVERRUN_INT | RXTIMEOUT_INT;

	//Free Interrupts.
	free_irq (Port->irq, Port);

	UART_LockUnlock(UARTId, LOCK);

	//Disable Interrupts.
	UART_IntDisable(UARTId, IntType);

	UART_LockUnlock(UARTId, UNLOCK);
}

/****************************************************************************/
/* NAME:  		SPEAR_UART_SetTermios 				    */
/*                                           				    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/* Configures the terminal ios                                              */
/*                                                                          */
/* PARAMETERS:                                                              */
/*                                                                          */
/* IN  :   Port:	Identify UART		                	    */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/* RETURN: t_uart_error :  UART error code	                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

static void SPEAR_UART_SetTermios(struct uart_port *Port, 
		struct termios *Termios , struct termios *Old)
{
	t_UARTConfigure Config;

	//Get UART Id
	t_UARTId UARTId = GET_UART_ID(Port->membase);

	//Set the Configuration Variable.
	//Setting FIFO Status.
	if (Port->fifosize > 1)
		Config.FIFOStatus 	= FIFO_ENABLE;
	else
		Config.FIFOStatus 	= FIFO_DISABLE;

	//Setting Baud Rate.
	switch(Termios->c_cflag & CBAUD)
	{
		case B110:	Config.BaudRate = BAUD110;	break;
		case B300:	Config.BaudRate = BAUD300;	break;
		case B1200:	Config.BaudRate = BAUD1200;	break;
		case B2400:	Config.BaudRate = BAUD2400;	break;
		case B4800:	Config.BaudRate = BAUD4800;	break;
		case B9600:	Config.BaudRate = BAUD9600;	break;
		case B19200:	Config.BaudRate = BAUD19200;	break;
		case B38400:	Config.BaudRate = BAUD38400;	break;
		case B57600:	Config.BaudRate = BAUD57600;	break;
		case B230400:	Config.BaudRate = BAUD230400;	break;
		case B460800:	Config.BaudRate = BAUD460800;	break;
		case B115200:
		default:	Config.BaudRate = BAUD115200;	break;
	}

	//Setting Word Size.
	switch (Termios->c_cflag & CSIZE)
	{
		case CS5:	Config.WordLength = DATA5;	break;
		case CS6:	Config.WordLength = DATA6;	break;
		case CS7:	Config.WordLength = DATA7;	break;
		case CS8:
		default:	Config.WordLength = DATA8;	break;
	}

	//Setting Stop Bits.
	if (Termios->c_cflag & CSTOPB)
		Config.StopBitSelect 	= STOP2;
	else
		Config.StopBitSelect 	= STOP1;

	//Setting Parity.
	if(Termios->c_cflag & PARENB)
	{
		if(Termios->c_cflag & PARODD)
			Config.ParitySelect 	= ODD;
		else
			Config.ParitySelect 	= EVEN;
	}
	else
	{
		Config.ParitySelect 	= PARITY_DISABLE;
	}

	//Setting WaterMark Level.
	Config.TxWatermarkLevel = OneByTwo;
	Config.RxWatermarkLevel = OneByTwo;

	//Enabling or Disabling Receiver.
	if (Termios->c_cflag & CREAD)
		Config.RxEnable 	= TRUE;
	else
		Config.RxEnable 	= TRUE;

	//Enabling Transmitter.
	Config.TxEnable 	= TRUE;

	//Configure UART.
	SPEAR_UART_Configure(UARTId, Config);
}

/****************************************************************************/
/* NAME:SPEAR_UART_PM  						 	    */
/*                                           				    */  
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*    Perform any power management related activities on the specified port.*/
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :   Port:	Identify UART		                	    */
/*                                                                          */
/* RETURN: 					                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void SPEAR_UART_PM(struct uart_port *Port, unsigned int State, unsigned int OldState)
{
}


/****************************************************************************/
/* NAME:  		SPEAR_UART_SetWake				    */
/*                                        				    */  
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :   Port:	Identify UART		                	    */
/* 	   State				                	    */
/*                                                                          */
/* RETURN: 					                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int SPEAR_UART_SetWake(struct uart_port *Port, unsigned int State)
{
	return 0;
}


/****************************************************************************/
/* NAME:  		SPEAR_UART_Type 				    */
/****************************************************************************/
/* DESCRIPTION:                                                             */
/*    Return a pointer to a string constant describing the specified Port,  */
/*    or return NULL, in which case the string 'unknown' is substituted.    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :   Port:	Identify UART		                	    */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: char *				                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

static const char *SPEAR_UART_Type(struct uart_port *Port)
{
	return __DRIVER_NAME;
}


/****************************************************************************/
/* NAME:  		SPEAR_UART_ConfigPort 				    */
/*                                              			    */  
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      Perform any autoconfiguration steps required for the port.          */
/* PARAMETERS:                                                              */
/*                                                                          */
/* IN  :   Port:	Identify UART		                	    */
/*                                                                          */
/* RETURN: 					                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

static void SPEAR_UART_ConfigPort(struct uart_port *Port, int Flags)
{
	if (Flags & UART_CONFIG_TYPE)
		Port->type = PORT_SPEAR;
}

/****************************************************************************/
/* NAME:   		SPEAR_UART_ReleasePort				    */
/*                                                 			    */  
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*    Release any memory and IO region resources currently in use by the    */
/*    port.                                                                 */
/* PARAMETERS:                                                              */
/* IN  :   Port:	Identify UART		                	    */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: 					                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

static void SPEAR_UART_ReleasePort(struct uart_port *Port)
{
}


/****************************************************************************/
/* NAME: 		SPEAR_UART_RequestPort 				    */
/*                                         				    */  
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*   Request any memory and IO region resources required by the port.       */
/* PARAMETERS:                                                              */
/* IN  :   Port:	Identify UART		                	    */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: 					                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

static int SPEAR_UART_RequestPort(struct uart_port *Port)
{
	return 0;
}



/****************************************************************************/
/* NAME: 		SPEAR_UART_VerifyPort				    */
/*                                                			    */  
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*    Verify the new serial port information contained within serinfo is    */
/*	suitable for this port type.                                        */
/* PARAMETERS:                                                              */
/*                                                                          */
/* IN  :   Port:	Identify UART		                	    */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: t_uart_error :  UART error code	                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int SPEAR_UART_VerifyPort(struct uart_port *Port, struct serial_struct *Serial)
{
	if ((Serial->type != PORT_SPEAR)||(Serial->irq >= NR_IRQS)|| (Serial->irq < 0))
		return -EINVAL;

	return 0;
}

static struct uart_ops SPEAR_UART_Ops =
{
		  tx_empty	: SPEAR_UART_IsTxEmpty,
		  set_mctrl	: SPEAR_UART_SetMctrl,
		  get_mctrl	: SPEAR_UART_GetMctrl,
		  stop_tx	: SPEAR_UART_TxStop,
		  start_tx	: SPEAR_UART_TxStart,
		  stop_rx	: SPEAR_UART_RxStop,
		  enable_ms	: SPEAR_UART_EnableMS,
		  break_ctl	: SPEAR_UART_BreakCtl,
		  startup	: SPEAR_UART_Startup,
		  shutdown	: SPEAR_UART_Shutdown,
		  set_termios	: SPEAR_UART_SetTermios,
		  pm		: SPEAR_UART_PM,
		  set_wake	: SPEAR_UART_SetWake,
		  type		: SPEAR_UART_Type,
		  config_port	: SPEAR_UART_ConfigPort,
		  release_port	: SPEAR_UART_ReleasePort,
		  request_port	: SPEAR_UART_RequestPort,
		  verify_port	: SPEAR_UART_VerifyPort
};

static struct uart_port SPEAR_UART_Ports[SPEAR_UART_NR]=
{
	{
membase 	: (unsigned char __iomem *)(VA_SPEAR_START_APB_UART0),
		  mapbase	: SPEAR_START_APB_UART0,
		  irq		: IRQ_UART_0,
		  uartclk	: CLOCK_TICK_RATE,
		  fifosize	: UART0_FIFO_SIZE,
		  ops		: &SPEAR_UART_Ops,
		  type		: PORT_SPEAR,
		  flags		: ASYNC_BOOT_AUTOCONF,
		  line		: 0
	}
#ifdef CONFIG_ARCH_SPEARPLUS
	,{
membase 	: (unsigned char __iomem *)(VA_SPEAR_START_APB_UART1),
		  mapbase	: SPEAR_START_APB_UART1,
		  irq		: IRQ_UART_1,
		  uartclk	: CLOCK_TICK_RATE,
		  fifosize	: UART1_FIFO_SIZE,
		  ops		: &SPEAR_UART_Ops,
		  type		: PORT_SPEAR,
		  flags		: ASYNC_BOOT_AUTOCONF,
		  line		: 1                        
	}
#endif
};

#ifdef CONFIG_SERIAL_SPEAR_CONSOLE

/****************************************************************************/
/* NAME:   		SPEAR_UART_ConsoleWrite				    */
/*                                                 			    */  
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*                                                                          */
/* PARAMETERS:                                                              */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: t_uart_error :  UART error code	                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

static void SPEAR_UART_ConsoleWrite(struct console *Co, const char *Buffer, u_int Count)
{
	struct uart_port *Port = &SPEAR_UART_Ports[Co->index];

	//Get UART Id
	t_UARTId UARTId = GET_UART_ID(Port->membase);
	t_UARTError Status;
	uint Index = 0;

    if(UART_LockUnlock(UARTId, LOCK) != UART_OK)
	    return;

	//Transmit Data.
	for (Index = 0; Index < Count; Index++)
	{
		//Transmit single byte
		Status = UART_Transmit(UARTId, 1, (char *)(Buffer + Index));
		if(Status != UART_OK)
		{
		    UART_LockUnlock(UARTId, UNLOCK);
			return;
		}

		//transmit '\r' if data transmitted is '\n'.
		if (Buffer[Index] == '\n')
		{
			Status = UART_Transmit(UARTId, 1, CarriageReturn);
			if(Status != UART_OK)
			{
				printk(KERN_ERR "Error:console_write %d", Status);
				UART_LockUnlock(UARTId, UNLOCK);
				return;
			}
		}
	}
	UART_LockUnlock(UARTId, UNLOCK);
}


/****************************************************************************/
/* NAME: 		SPEAR_UART_ConsoleSetup 			    */
/*                                               			    */  
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/* PARAMETERS:                                                              */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: t_uart_error :  UART error code	                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int __init SPEAR_UART_ConsoleSetup(struct console *Co, char *Options)
{
	struct uart_port *Port;
	int Baud = 115200;
	int Bits = 8;
	int Parity = 'n';
	int Flow = 'n';

	//Check whether an invalid uart number has been specified, and if so, search for the first available port 
	//that does have console support.
	if (Co->index >= SPEAR_UART_NR)
		Co->index = 0;
	Port = &SPEAR_UART_Ports[Co->index];

	//Initializing Locks.
	spin_lock_init(&UART0_Spin_Lock);
	spin_lock_init(&UART1_Spin_Lock);

	if (Options)
		uart_parse_options(Options, &Baud, &Parity, &Bits, &Flow);

	return uart_set_options(Port, Co, Baud, Parity, Bits, Flow);
}

static struct uart_driver SPEAR_UART_Reg;
static struct console SPEAR_UART_Console =
{
	   .name     = SPEAR_UART_NAME,
	   .write    = SPEAR_UART_ConsoleWrite,
	   .device   = uart_console_device,
	   .setup    = SPEAR_UART_ConsoleSetup,
	   .flags    = CON_PRINTBUFFER,
	   .index    = -1,
	   .data     = &SPEAR_UART_Reg,
};

/****************************************************************************/
/* NAME: 		SPEAR_UART_ConsoleInit                              */
/*                                               			    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*  Initializes the console of apb                       		    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: t_uart_error :  UART error code	                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

static int __init SPEAR_UART_ConsoleInit(void)
{
	register_console(&SPEAR_UART_Console);
	return(UART_Init());
}

console_initcall(SPEAR_UART_ConsoleInit);

#define SPEAR_CONSOLE	 &SPEAR_UART_Console
#else   /* CONFIG_SERIAL_SPEAR_CONSOLE*/
#define SPEAR_CONSOLE	 NULL
#endif  /* CONFIG_SERIAL_SPEAR_CONSOLE*/

static struct uart_driver SPEAR_UART_Reg = 
{
		  .owner	= THIS_MODULE,
		  .driver_name	= "SPEAR_UART",
		  .dev_name	= SPEAR_UART_NAME,
		  .major	= SPEAR_UART_MAJOR,
		  .minor	= SPEAR_UART_MINOR,
		  .nr		= SPEAR_UART_NR,
		  .cons		= SPEAR_CONSOLE
};


/****************************************************************************/
/* NAME:  			SPEAR_UART_Init 			    */
/*                                                			    */  
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*        initializes the uart                                              */
/* PARAMETERS:                                                              */
/* IN  :   t_uart_number  unumber:identify UART to configure                */
/* IN  :   t_uart_configuration  *pconfig:  pointer on global               */
/*                                          configuration structure         */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: t_uart_error :  UART error code	                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int __init SPEAR_UART_Init(void)
{
	int Ret, Index;

	printk("SPEAR :: Initializing in drivers %s inside UART\n", __DRIVER_NAME);

	//Registering UART Driver.
	Ret = uart_register_driver(&SPEAR_UART_Reg);

	//Adding All the ports.
	if (Ret== 0)
	{
		for (Index = 0; Index < SPEAR_UART_NR; Index++)
			uart_add_one_port(&SPEAR_UART_Reg, &SPEAR_UART_Ports[Index]);
	}
	else
		printk("\n\nFailed to Install SPEAR UART Driver\n");

	//Call UART Init...
	return(UART_Init());
}

/****************************************************************************/
/* NAME:  			SPEAR_UART_Exit 			    */
/*                                                 			    */  
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*                                                                          */
/* PARAMETERS:                                                              */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: t_uart_error :  UART error code	                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void __exit SPEAR_UART_Exit(void)
{
	int Index;
	//Removing All the ports.
	for (Index = 0; Index < SPEAR_UART_NR; Index++)
		uart_remove_one_port(&SPEAR_UART_Reg, &SPEAR_UART_Ports[Index]);

	//UnRegistering UART Driver.
	uart_unregister_driver(&SPEAR_UART_Reg);
}



module_init(SPEAR_UART_Init);
module_exit(SPEAR_UART_Exit);

MODULE_AUTHOR("ST Microelectronic");
MODULE_DESCRIPTION("SPEAR generic serial port driver");
MODULE_SUPPORTED_DEVICE("ttyS");
MODULE_LICENSE("GPL");

