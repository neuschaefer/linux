/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2008-2018, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : hi_serial.h
  版 本 号   : 初稿
  生成日期   : D2014_03_09

******************************************************************************/

#ifndef __HI_SERIAL_H__
#define __HI_SERIAL_H__

#define HI_UART_DRVNAME              "hi_uart"
#define HI_UART_DEVNAME              "ttyAMA"
#define HI_UART_NR                   2
#define HI_UART_MAJOR                204
#define HI_UART_MINOR                64
#define HI_ISR_PASS_LMT              256
                                     
#define HI_UPIO_MEM                  2
#define HI_UPF_BOOT_AUTOCONF        (1 << 28)

#define HI_RECEIVERR                (0x06)    /* HIGHEST   */
#define HI_RECEIVEAVA               (0x04)    /* SECOND  RECEIVE date */
#define HI_RECTIMEOUT               (0x0C)    /* SECOND    */
#define HI_TRANSEMP                 (0x02)    /* THIRD transmit hold reg empty */
#define HI_NOINTERRUPT              (0x01)    /* NO interrupt pending */
#define HI_MODEMSTA                 (0)       /* FOURTH modem int    */

#define HI_TEMT                     (1<<6)
#define HI_DR                       (1)

#define HI_TEMPT                    (0x01)  /* Transmitter physically empty */
#define HI_PTIME                    (1<<7)
#define HI_ELSI                     (1<<2)
#define HI_ETBEI                    (1<<1)
#define HI_ERBFI                    (1)

#define HI_UART_BUSY                (0x01)
#define HI_UART_DLAB                (1 << 7)
#define HI_UART_BREAK               (1 << 6)
#define HI_UART_EPS                 (1 << 4)
#define HI_UART_PEN                 (1 << 3)
#define HI_UART_STOP                (1 << 2)

#define HI_UART_8bit                0x3
#define HI_UART_7bit                0x2
#define HI_UART_6bit                0x1
#define HI_UART_5bit                0x0

#define HI_UART_RBR                 0x00     /*  Receive buff. */
#define HI_UART_THR                 0x00     /*  Transmit Holding  */
#define HI_UART_DLL                 0x00     /*  Divisor Latch Low */
#define HI_UART_DLH                 0x04     /*  Divisor Latch High */
#define HI_UART_IER                 0x04     /*  int enable*/
#define HI_UART_IIR                 0x08     /*  interrupt indentification REG*/
#define HI_UART_FCR                 0x08     /*  FIFO control*/
#define HI_UART_LCR                 0x0C     /*  Line control reg */
#define HI_UART_LSR                 0x14     /*  Line  statue  */
#define HI_UART_USR                 0x7C     /*  Uart statues */

#define HI_UART_PORT_AMBA           32
#define HI_UART_PORT_UNKNOWN        0

#define HI_UART_DR_OE               (1 << 9)
#define HI_UART_DR_BE               (1 << 12)
#define HI_UART_DR_PE               (1 << 10)
#define HI_UART_DR_FE               (1 << 11)
#define HI_UART_DR_ERR              (HI_UART_DR_OE|HI_UART_DR_BE|HI_UART_DR_PE|HI_UART_DR_FE)

#define HI_UART_DUMMY_DR_RX         (1 << 16)


#endif /* __HI_SERIAL_H__ */
