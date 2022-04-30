/*
 * linux/include/asm/arch/platform.h
 *
 *  Copyright(C)2006 ST MICROELECTRONICS
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
 * AUTHOR :  Arif Khan <arif.khan@st.com>
 */

#ifndef __address_h
#define __address_h                     1



/* Spear basic only */

/* ================================================ */

#define SPEARBASIC_VIC1_BASE             0xF1100000  
//#define SPEARBASIC_VIC2_BASE				0xF1000000

#define SPEARBASIC_START_SDRAM           0x00000000
#define SPEARBASIC_SIZE_SDRAM            0x04000000

/*
 *  Interrupt bit positions
 *
 */

#define INT_HW_ACCEL_MOD_0				0	
#define INT_INTRCOMM_RAS_ARM			1
#define INT_GPT0						2
#define INT_GPT1						3
#define INT_BASIC_TIMER_1_1         	4
#define INT_BASIC_TIMER_1_2         	5
#define INT_BASIC_TIMER_2_1         	6
#define INT_BASIC_TIMER_2_2         	7
#define INT_BASIC_DMA            		8
#define INT_BASIC_SMI               	9
#define INT_BASIC_RTC               	10
#define INT_BASIC_GPIO              	11
#define INT_BASIC_WDT              		12
#define INT_DDR_CONTROLLER          	13
#define INT_SYS_ERROR		          	14
#define INT_WAKEUP_RCV					15
#define INT_JPEG						16
#define INT_IRDA						17
#define INT_ADC		                	18
#define INT_UART_0						19
#define INT_SSP							20
#define INT_I2C							21
#define INT_MAC_1      	 				22
#define INT_MAC_2      	 				23
#define INT_USB_DEV      				24
#define INT_USB_H_OHCI_0				25	
#define INT_USB_H_EHCI_0 				26
#define	INT_USB_H_EHCI_1 				INT_USB_H_EHCI_0
#define INT_USB_H_OHCI_1 				27
#define INT_GEN_RAS_1					28
#define INT_GEN_RAS_2					29
#define INT_GEN_RAS_3					30
#define INT_HW_ACCEL_MOD_1				31	

/* ------------------------------------------------------------------------
 *  SPEARBASIC Interrupt Controller - control registers
 * ------------------------------------------------------------------------
 *  Offsets from interrupt controller base
 */

#define VIC_IRQ_STATUS                  0
#define VIC_FIQ_STATUS                  0x04
#define VIC_IRQ_RAW_STATUS              0x08
#define VIC_INT_SELECT                  0x0C	/* 1 = FIQ, 0 = IRQ */
#define VIC_IRQ_ENABLE                  0x10	/* 1 = enable, 0 = disable */
#define VIC_IRQ_ENABLE_CLEAR            0x14
#define VIC_IRQ_SOFT                    0x18
#define VIC_IRQ_SOFT_CLEAR              0x1C
#define VIC_PROTECT                     0x20
#define VIC_VECT_ADDR                   0x30
#define VIC_DEF_VECT_ADDR               0x34
#define VIC_VECT_ADDR0                  0x100	/* 0 to 15 */
#define VIC_VECT_CNTL0                  0x200	/* 0 to 15 */
#define VIC_ITCR                        0x300   /* VIC test control register */

#define VIC_FIQ_RAW_STATUS              0x08  //Arif : to check
#define VIC_FIQ_ENABLE                  0x10	/* 1 = enable, 0 = disable */
#define VIC_FIQ_ENABLE_CLEAR            0x14
#define VIC_FIQ_SOFT                    0x18
#define VIC_FIQ_SOFT_CLEAR              0x1C


/* ================================================ */

/*
 *  These are useconds NOT ticks.
 *
 */
#define mSEC_1                          1000
#define mSEC_5                          (mSEC_1 * 5)
#define mSEC_10                         (mSEC_1 * 10)
#define mSEC_25                         (mSEC_1 * 25)
#define SEC_1                           (mSEC_1 * 1000)

#define VICVectCntl_Enable	(1 << 5)


#define MAXIRQNUM		63
#define MAXFIQNUM		63
#define MAXSWINUM		63



#define ASC_TX_BUFFER   (*(volatile unsigned short *)(IO_ADDRESS(SPEARBASIC_START_APB_UART0) + 0x00 )) //Arif: Changed the address of UART
#define ASC_STATUS      (*(volatile unsigned short *)(IO_ADDRESS(SPEARBASIC_START_APB_UART0) + 0x18 )) //Arif: Changed the address of UART

#endif /*END*/


