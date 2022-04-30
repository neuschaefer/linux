/*
 * linux/include/asm/arch/uart.h
 *
 *  Copyright (C)2008  ST MICROELECTRONICS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * AUTHOR :  Deepak Sikri <deepak.sikri@st.com> */


#ifndef __UART_H
#define __UART_H
#include "hcl_defs.h"
/*------------------------------------------------------------------------------
-- ACCENT APB UART
------------------------------------------------------------------------------*/

#define APB_UART_FIFOSIZE		16	/* deep 16 and wide 8 bits */

#define APB_UART_OFFSET_BAUDRATE	0x00	/* RW: input clock divider 1-2^16 */
#define APB_UART_OFFSET_BUFFER_TX	0x04	/* -W: char to be sent */
#define APB_UART_OFFSET_BUFFER_RX	0x08	/* R-: char to be read */
#define APB_UART_OFFSET_CONTROL		0x0C	/* RW: operating mode */
#define APB_UART_OFFSET_INT_ENABLE	0x10	/* RW: interrupts source mask */
#define APB_UART_OFFSET_STATUS		0x14	/* R-: interrupts status */
#define APB_UART_OFFSET_GUARD_TIME	0x18	/* RW: time schedule upon empty tx fifo */
#define APB_UART_OFFSET_TIMEOUT		0x1C	/* RW: timer between rx chars */
#define APB_UART_OFFSET_RESET_TX	0x20	/* -W: reset tx fifo */
#define APB_UART_OFFSET_RESET_RX	0x24	/* -W: reset rx fifo */


typedef volatile struct apb_uart {
	t_uint32 baudrate;	/* RW: input clock divider 1-2^16 */
	t_uint32 buffer_tx;	/* -W: char to be sent */
	t_uint32 buffer_rx;	/* R-: char to be read */
	t_uint32 control;	/* RW: operating mode */
	t_uint32 int_enable;	/* RW: interrupts source mask */
	t_uint32 status;	/* R-: interrupts status */
	t_uint32 guard_time;	/* RW: time schedule upon empty tx fifo */
	t_uint32 timeout;	/* RW: timer between rx chars */
	t_uint32 reset_tx;	/* -W: reset tx fifo */
	t_uint32 reset_rx;	/* -W: reset rx fifo */
} apb_uart_t;

/*------------------------------------------------------------------------------
-- APB_UART_CONTROL register settings
------------------------------------------------------------------------------*/

#define APB_UART_DATA_MASK			0x0007	/* data mask */
#define APB_UART_DATA_8_BITS_NOPARITY		0x0001	/* 8 bits */
#define APB_UART_DATA_7_BITS_PARITY		0x0003	/* 7 bits + parity */
#define APB_UART_DATA_9_BITS_NOPARITY		0x0004	/* 9 bits */
#define APB_UART_DATA_8_BITS_WAKEUP		0x0005	/* 8 bits + wakeup */
#define APB_UART_DATA_8_BITS_PARITY		0x0007	/* 8 bits + parity */

#define APB_UART_STOP_BITS_MASK			0x0018	/* stop bits mask */
#define APB_UART_STOP_BITS_0_5			0x0000	/* 0.5 stop bits */
#define APB_UART_STOP_BITS_1_0			0x0008	/* 1.0 stop bits */
#define APB_UART_STOP_BITS_1_5			0x0010	/* 1.5 stop bits */
#define APB_UART_STOP_BITS_2_0			0x0018	/* 2.0 stop bits */

#define APB_UART_PARITY_STATUS			0x0020	/* parity status */
#define APB_UART_PARITY_ODD			0x0020	/* parity odd */
#define APB_UART_PARITY_EVEN			0x0000	/* parity even */

#define APB_UART_LOOPBACK_STATUS		0x0040	/* loopback mode status */
#define APB_UART_LOOPBACK_ON			0x0040	/* loopback mode on */
#define APB_UART_LOOPBACK_OFF			0x0000	/* standard tx/rx */

#define APB_UART_BRG_STATUS			0x0080	/* BRG status */
#define APB_UART_BRG_ON				0x0080	/* BRG enabled */
#define APB_UART_BRG_OFF			0x0000	/* BRG disabled */

#define APB_UART_RXE_STATUS			0x0100	/* receiver status */
#define APB_UART_RXE_ON				0x0100	/* receiver enabled */
#define APB_UART_RXE_OFF			0x0000	/* receiver disabled */

#define APB_UART_SCE_STATUS			0x0200	/* smart card status */
#define APB_UART_SCE_ON				0x0200	/* smart card enabled */
#define APB_UART_SCE_OFF			0x0000	/* smart card disabled */

#define APB_UART_FIFO_STATUS			0x0400	/* FIFOs status */
#define APB_UART_FIFO_ON			0x0400	/* FIFOs enabled */
#define APB_UART_FIFO_OFF			0x0000	/* FIFOs disabled */

/*------------------------------------------------------------------------------
-- APB_UART_INT_ENABLE & APB_UART_STATUS registers settings
------------------------------------------------------------------------------*/

#define APB_UART_RXBF		0x0001	/* RX buffer full */
#define APB_UART_TXBE		0x0002	/* TX buffer empty */
#define APB_UART_TXBH		0x0004	/* TX buffer half */
#define APB_UART_PE		0x0008	/* parity error */
#define APB_UART_FE		0x0010	/* frame error */
#define APB_UART_OE		0x0020	/* overrun error */
#define APB_UART_TNE		0x0040	/* timeout not empty */
#define APB_UART_TI		0x0080	/* timeout idle */
#define APB_UART_RXBH		0x0100	/* RX buffer half */
#define APB_UART_TXBF		0x0200	/* TX buffer full */

/*------------------------------------------------------------------------------
-- APB UART default settings
------------------------------------------------------------------------------*/

#define APB_UART_DEFAULT_CONTROL	(	APB_UART_DATA_8_BITS_NOPARITY | \
						APB_UART_STOP_BITS_1_0 | \
						APB_UART_BRG_ON | \
						APB_UART_RXE_ON | \
						APB_UART_FIFO_ON )

#define APB_UART_DEFAULT_INT_ENABLE	(	APB_UART_RXBF | \
						APB_UART_TXBE | \
						APB_UART_TXBH | \
						APB_UART_PE | \
						APB_UART_FE | \
						APB_UART_OE | \
						APB_UART_RXBH )

#endif /* __UART_H */
