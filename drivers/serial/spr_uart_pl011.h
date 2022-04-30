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
#ifndef SPEAR_UART_H
#define SPEAR_UART_H

#include "spr_uart_pl011_hal.h"
#include <asm/arch/kernel_defs.h>

/***********************************************************************************
 * PUBLIC Functions
 ***********************************************************************************/
//This API is used to initialize the UART.
static int __init SPEAR_UART_Init(void);
static void __exit SPEAR_UART_Exit(void);

//Registered Serial Driver Operations.
static u_int SPEAR_UART_IsTxEmpty(struct uart_port *Port);
static void SPEAR_UART_SetMctrl(struct uart_port *Port, u_int Mctrl);
static u_int SPEAR_UART_GetMctrl(struct uart_port *Port);
static void SPEAR_UART_TxStop(struct uart_port *Port/*, u_int From_tty*/);
static void SPEAR_UART_TxStart(struct uart_port *Port/*, u_int From_tty*/);
static void SPEAR_UART_RxStop(struct uart_port *Port);
static void SPEAR_UART_EnableMS(struct uart_port *Port);
static void SPEAR_UART_BreakCtl(struct uart_port *Port, int BreakState);
static int SPEAR_UART_Startup(struct uart_port *Port);
static void SPEAR_UART_Shutdown(struct uart_port *Port);
static void SPEAR_UART_SetTermios(struct uart_port *Port, struct termios *Termios , struct termios *Old);
static void SPEAR_UART_PM(struct uart_port *Port, unsigned int State, unsigned int OldState);
static int SPEAR_UART_SetWake(struct uart_port *Port, unsigned int State);
static const char *SPEAR_UART_Type(struct uart_port *Port);
static void SPEAR_UART_ConfigPort(struct uart_port *Port, int Flags);
static void SPEAR_UART_ReleasePort(struct uart_port *Port);
static int SPEAR_UART_RequestPort(struct uart_port *Port);
static int SPEAR_UART_VerifyPort(struct uart_port *Port, struct serial_struct *Serial);

//Interrupts handler.
static irqreturn_t SPEAR_UART_IntHandler(int Irq, void *Dev_id, struct pt_regs *Reg);

//Console Driver Interface.
static int __init SPEAR_UART_ConsoleInit(void);
static void SPEAR_UART_ConsoleWrite(struct console *Console, const char *S, u_int Count);
static int __init SPEAR_UART_ConsoleSetup(struct console *Console, char *Options);

//Kernel Layer API's to be called directly.
//This API is used to Configure UART.
t_UARTError SPEAR_UART_Configure(t_UARTId UARTId, t_UARTConfigure Config);

//This API is used to transmit a number of characters.
t_UARTError SPEAR_UART_Transmit(t_UARTId UARTId, uint NumOfCharToTx, char *Buffer);

//This API is used to Receive a number of characters.
t_UARTError SPEAR_UART_Receive(t_UARTId UARTId, uint NumOfCharToRx, char *Buffer, uint *NumOfCharRx, t_UARTRxError *RxError);

//This API is used to Disable a UART.
t_UARTError SPEAR_UART_Disable(t_UARTId UARTId);


#ifdef CONFIG_ARCH_SPEARBASIC
#define VA_SPEAR_START_APB_UART0	VA_SPEARBASIC_START_APB_UART0
#define VA_SPEAR_START_APB_UART1	VA_SPEARBASIC_START_APB_UART1
#define SPEAR_START_APB_UART0		SPEARBASIC_START_APB_UART0
#define SPEAR_START_APB_UART1		SPEARBASIC_START_APB_UART1
#else
#define VA_SPEAR_START_APB_UART0	VA_SPEARPLUS_START_APB_UART0
#define VA_SPEAR_START_APB_UART1	VA_SPEARPLUS_START_APB_UART1
#define SPEAR_START_APB_UART0		SPEARPLUS_START_APB_UART0
#define SPEAR_START_APB_UART1		SPEARPLUS_START_APB_UART1
#endif



#endif // SPEAR_UART_H
