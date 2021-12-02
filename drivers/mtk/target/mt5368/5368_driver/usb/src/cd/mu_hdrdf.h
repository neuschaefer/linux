/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/

#ifndef __MUSB_HDRC_DEFS_H__
#define __MUSB_HDRC_DEFS_H__

#include "mu_drcdf.h"

/*
 * HDRC-specific definitions
 * $Revision: #1 $
 */
extern unsigned int _u4UsbQDebugLevel;

#ifndef USB_HW_BASE_ADDR
#define USB_HW_BASE_ADDR MUSB_BASE
#endif


#define USB_HW_QMU_OFF	        0x00000800
#define USB_HW_QUCS_OFF	0x00000B00
#define USB_HW_QIRQ_OFF	0x00000C00
#define USB_HW_QDBG_OFF	0x00000CF0

/*
 *     MUSBHDRC Register map 
 */

/* Common USB registers */

#define MGC_O_HDRC_FADDR	0x00	/* 8-bit */
#define MGC_O_HDRC_POWER	0x01	/* 8-bit */

#define MGC_O_HDRC_INTRTX	0x02	/* 16-bit */
#define MGC_O_HDRC_INTRRX       0x04
#define MGC_O_HDRC_INTRTXE      0x06  
#define MGC_O_HDRC_INTRRXE      0x08  
#define MGC_O_HDRC_INTRUSB      0x0A   /* 8 bit */
#define MGC_O_HDRC_INTRUSBE     0x0B   /* 8 bit */
#define MGC_O_HDRC_FRAME        0x0C  
#define MGC_O_HDRC_INDEX        0x0E   /* 8 bit */
#define MGC_O_HDRC_TESTMODE     0x0F   /* 8 bit */

/* Additional Control Registers */

#define	MGC_O_HDRC_DEVCTL	   0x60	   /* 8 bit */    

/* These are actually indexed: */
#define MGC_O_HDRC_TXFIFOSZ	0x62	/* 8-bit (see masks) */
#define MGC_O_HDRC_RXFIFOSZ	0x63	/* 8-bit (see masks) */
#define MGC_O_HDRC_TXFIFOADD	0x64	/* 16-bit offset shifted right 3 */
#define MGC_O_HDRC_RXFIFOADD	0x66	/* 16-bit offset shifted right 3 */

/* Endpoint registers */
#define MGC_O_HDRC_TXMAXP	0x00
#define MGC_O_HDRC_TXCSR	0x02
#define MGC_O_HDRC_TXCSR2	0x03
#define MGC_O_HDRC_CSR0		MGC_O_HDRC_TXCSR	/* re-used for EP0 */
#define MGC_O_HDRC_RXMAXP	0x04
#define MGC_O_HDRC_RXCSR	0x06
#define MGC_O_HDRC_RXCSR2	0x07
#define MGC_O_HDRC_RXCOUNT	0x08
#define MGC_O_HDRC_COUNT0	MGC_O_HDRC_RXCOUNT	/* re-used for EP0 */
#define MGC_O_HDRC_TXTYPE	0x0A
#define MGC_O_HDRC_TYPE0	MGC_O_HDRC_TXTYPE	/* re-used for EP0 */
#define MGC_O_HDRC_TXINTERVAL	0x0B
#define MGC_O_HDRC_NAKLIMIT0	MGC_O_HDRC_TXINTERVAL	/* re-used for EP0 */
#define MGC_O_HDRC_RXTYPE	0x0C
#define MGC_O_HDRC_RXINTERVAL	0x0D
#define MGC_O_HDRC_FIFOSIZE	0x0F
#define MGC_O_HDRC_CONFIGDATA	MGC_O_HDRC_FIFOSIZE	/* re-used for EP0 */


//level 1 interrupt
#if defined(CC_MT5368) || defined(CC_MT5396)
#define M_REG_INTRLEVEL1 0xA0
#define M_REG_INTRLEVEL1EN 0xA4
#endif

#define MGC_O_HDRC_TXFADDR( _n )  (0x480 + (((_n))*8) )
#define MGC_O_HDRC_RXFADDR( _n )  (0x484 + (((_n))*8) )

/* Added in HDRC 1.9(?) & MHDRC 1.4 */
/* ULPI pass-through */
#define MGC_O_HDRC_ULPI_VBUSCTL	0x70
#define MGC_O_HDRC_ULPI_REGDATA 0x74
#define MGC_O_HDRC_ULPI_REGADDR 0x75
#define MGC_O_HDRC_ULPI_REGCTL	0x76

/* USB PHY Register */
#define MGC_O_HDRC_UTMI             0x500
#define MGC_O_HDRC_PHYDCTL0     0x504
#define MGC_O_HDRC_PHYDCTL1     0x508
#define MGC_O_HDRC_PHYACTL0     0x50C
#define MGC_O_HDRC_PHYACTL1     0x510
#define MGC_O_HDRC_PHYACTL3     0x514
#define MGC_O_HDRC_PHYACTL4     0x518
#define MGC_O_HDRC_PHYACTL5     0x51C
#define MGC_O_HDRC_PHYACTL15   0x520
#define MGC_O_HDRC_PHYACTL16   0x524
#define MGC_O_HDRC_PHYACTL17   0x528
#define MGC_O_HDRC_PHYACTL18   0x52C
#define MGC_O_HDRC_PHYACTL19   0x530
#define MGC_O_HDRC_PHYACTL20   0x534
#define MGC_O_HDRC_PHYACTL21   0x538
#define MGC_O_HDRC_PHYACTL22   0x53C
#define MGC_O_HDRC_PHYACTL23   0x540
#define MGC_O_HDRC_PHYACTL24   0x544
#define MGC_O_HDRC_PHYACTL25   0x548
#define MGC_O_HDRC_PHYACTL26   0x54C
#define MGC_O_HDRC_PHYACTL27   0x550
#define MGC_O_HDRC_PHYACTL28   0x554
#define MGC_O_HDRC_PHYACTL29   0x558

/* USB MAC Extra Register */
#define MGC_O_HDRC_MACEXTRA  0x55C

/* USB QMU Extra Register */
#define MGC_O_QMU_QCR0	0x0000
/*wimax only*/
#define MGC_O_QMU_QCR1	0x0004
/*end*/
#define MGC_O_QMU_QCR2	0x0008
#define MGC_O_QMU_QCR3	0x000C

#define MGC_O_QMU_RQCSR0	0x0010
#define MGC_O_QMU_RQSAR0	0x0014
#define MGC_O_QMU_RQCPR0	0x0018
#define MGC_O_QMU_RQCSR1	0x0020
#define MGC_O_QMU_RQSAR1	0x0024
#define MGC_O_QMU_RQCPR1	0x0028
#define MGC_O_QMU_RQCSR2	0x0030
#define MGC_O_QMU_RQSAR2	0x0034
#define MGC_O_QMU_RQCPR2	0x0038
#define MGC_O_QMU_RQCSR3	0x0040
#define MGC_O_QMU_RQSAR3	0x0044
#define MGC_O_QMU_RQCPR3	0x0048
#define MGC_O_QMU_RQCSR(n) (MGC_O_QMU_RQCSR0+0x0010*((n)-1))
#define MGC_O_QMU_RQSAR(n) (MGC_O_QMU_RQSAR0+0x0010*((n)-1))
#define MGC_O_QMU_RQCPR(n) (MGC_O_QMU_RQCPR0+0x0010*((n)-1))

#define MGC_O_QMU_DEBUG_FLAG	0x0000
#define MGC_O_QMU_DEBUG_MONITOR	0x0004

#define MGC_O_QIRQ_QISAR		0x0000
#define MGC_O_QIRQ_QIMR		0x0004
#define MGC_O_QIRQ_QIMCR		0x0008
#define MGC_O_QIRQ_QIMSR		0x000C

#ifdef MUSB_QMU
#define MGC_O_HSDMA_BASE    0x200
#define MGC_O_HSDMA_INTR    0x200
#define MGC_O_HSDMA_LIMITER 0x210
#define MGC_O_HSDMA_CONFIG    0x220

#define MGC_O_HSDMA_CONTROLLER( _n )  (0x204 + (((_n)-1)*0x10) )
#define MGC_O_HSDMA_ADDRESS1( _n )     (0x208 + (((_n)-1)*0x10) )
#define MGC_O_HSDMA_COUNT1( _n )       (0x20c + (((_n)-1)*0x10) )
#define MGC_O_HSDMA_REALCNT( _n )     (0x280 + (((_n)-1)*0x10) )
#endif

/*
    Begin of MTK created register definition.
*/
#if defined(CC_MT5368) || defined(CC_MT5396)
#define M_REG_FIFOBYTECNT         0x690
#else
#ifdef MTK_MHDRC
//  MTK Notice: Max Liao, 2006/08/29.
//  added for non-32 bits aligned fifo read/write. Base addr = USB_BASE = 0x20029600.
#define	M_REG_FIFOBYTECNT	 0xEC
#else
//  MTK Notice: Max Liao, 2006/10/02.
//  added for non-32 bits aligned fifo read/write. Base addr = USB_BASE = 0x20029800.
#define	M_REG_FIFOBYTECNT	 0x80
#endif
#endif

//  MTK Notice: Max Liao, 2006/08/18.
//  support one to one mapping ep and device address. Base addr = USB_BASE = 0x20029800.
#define	M_REG_EP0ADDR	 0x90
#define	M_REG_EP1ADDR	 0x94
#define	M_REG_EP2ADDR	 0x98
#define	M_REG_EP3ADDR	 0x9C
#define	M_REG_EP4ADDR	 0xA0
#define	M_REG_EPXADDR(X)	 (M_REG_EP0ADDR + ((X) << 2))

//  MTK Notice: Max Liao, 2006/05/22.
//  read PHY line state. Base addr = USB_PHYBASE = 0x20029400.
#define M_REG_PHYC0		        0x00
#define M_REG_PHYC1		        0x04
#define M_REG_PHYC2		        0x08
#define M_REG_PHYC3		        0x0c
#define M_REG_PHYC4		        0x10
#define M_REG_PHYC5		        0x14
#define M_REG_PHYC6		        0x18
#define M_REG_PHYC7 	                0x1c
#define M_REG_LINESTATE_MASK	0x00030000

#define LINESTATE_DISCONNECT         0x00000000
#define LINESTATE_FS_SPEED              0x00010000
#define LINESTATE_LS_SPEED              0x00020000
#define LINESTATE_HWERROR              0x00030000
   
//  MTK Notice: Max Liao, 2006/05/29.
//  MTK add: soft reset register. Base addr = USB_MISCBASE = 0x20029600.
#define M_REG_SOFTRESET		                    0x0
#define M_REG_SOFT_RESET_ACTIVE             0x0 
#ifdef MTK_MHDRC
#define M_REG_SOFT_RESET_DEACTIVE         0x3
#else
#define M_REG_SOFT_RESET_DEACTIVE         0x1
#endif
//  MTK add: access unit control register. Base addr = USB_MISCBASE = 0x20029600.
/*
0x20029604
bit[1:0] reg_size  : should be always  2'b10
bit[4] Function address enable : enable function address selected by endpoint, default :1(enable)
bit[8] Force DRAM read byte enable : Force Byte enable = 0xFFFF during DRAM read, default: 0(disable)
bit[9]: Enable group2 DRAM agent, Select group2 DRAM agent, default: 0(group3)
*/
#define M_REG_ACCESSUNIT		                    0x4
#define M_REG_ACCESSUNIT_8BIT                  0x0
#define M_REG_ACCESSUNIT_16BIT                0x1
#define M_REG_ACCESSUNIT_32BIT                0x2
#define M_REG_DEV_ADDR_MODE                    0x10

//  MTK add: data toggle control register. Base addr = USB_MISCBASE = 0x20029600.
#if defined(CC_MT5368) || defined(CC_MT5396)
#define M_REG_RXDATATOG		            0x80
#define M_REG_TXDATATOG		            0x84
#define M_REG_SET_DATATOG(ep, v)	    (((1 << ep) << 16) | (v << ep))
#else
#define M_REG_RXDATATOG		            0x10
#define M_REG_TXDATATOG		            0x14
#define M_REG_SET_DATATOG(ep, v)	    (((1 << ep) << 16) | (v << ep))
#endif

//  MTK add: request packet number. Base addr = USB_DMABASE = 0x20029A00.
#define M_REG_REQPKT(ep)	                    (0x100 + ((ep)<<2))	      

//  MTK Notice: Max Liao, 2006/09/19.
//  MTK add: IN packet interrupt. Base addr = USB_MISCBASE = 0x20029600.
/*
0x20029608[15:0] : Interrupt mask ( default : 16'hFFFF, will change to 16'h0 later)
0x2002960C [15:0]: interrupt status ( default : 0)
bit[15:0] RX IN endpoint request bit[0] : EP0, bit[1] : EP1, ...
Notes: Endpoint number is logical endpoint number, not physical.
*/
#define M_REG_INPKT_ENABLE	                    0x8
#define M_REG_INPKT_STATUS	                    0xC

//  MTK Notice: Max Liao, 2006/09/19.
//  MTK add: Powe down register. Base addr = USB_MISCBASE = 0x20029600.
/*
bit 0 : Enable DRAM clock, default : 1<Enable>
bit 1 : Enable Hardware Auto-PowerDown/Up, default : 0<Disable>, Auto-Clear after PowerUp
bit 2 : Read Only, 1: PHY Clock valid, 0: PHY clock is off.
After turn off DRAM clock, only 0x20029680 registers is accessable.
Write other registers makes no effect, and read other registers return constant value, 32'hDEAD_BEAF
*/
#define M_REG_AUTOPOWER	                    0x80
#define M_REG_AUTOPOWER_DRAMCLK         0x01
#define M_REG_AUTOPOWER_ON                     0x02
#define M_REG_AUTOPOWER_PHYCLK             0x04
/*
    End of MTK created register definition.
*/

/* Added in HDRC 1.9(?) & MHDRC 1.4 */
/* ULPI pass-through */
#define MGC_O_HDRC_ULPI_VBUSCTL	0x70
#define MGC_O_HDRC_ULPI_REGDATA 0x74
#define MGC_O_HDRC_ULPI_REGADDR 0x75
#define MGC_O_HDRC_ULPI_REGCTL	0x76

#if defined(CC_MT5368) || defined(CC_MT5396)
#define M_REG_PERFORMANCE1 0x70
#define M_REG_PERFORMANCE2 0x72
#define M_REG_PERFORMANCE3 0x74
#endif

/* extended config & PHY control */
#define MGC_O_HDRC_ENDCOUNT	0x78
#define MGC_O_HDRC_DMARAMCFG	0x79
#define MGC_O_HDRC_PHYWAIT	0x7A
#define MGC_O_HDRC_PHYVPLEN	0x7B	/* units of 546.1 us */
#define MGC_O_HDRC_HSEOF1	0x7C	/* units of 133.3 ns */
#define MGC_O_HDRC_FSEOF1	0x7D	/* units of 533.3 ns */
#define MGC_O_HDRC_LSEOF1	0x7E	/* units of 1.067 us */

/* "bus control" registers */
#ifdef MTK_MHDRC
#define MGC_O_MHDRC_TXFUNCADDR	0x00
#define MGC_O_MHDRC_TXHUBADDR	0x02
#define MGC_O_MHDRC_TXHUBPORT	0x03

#define MGC_O_MHDRC_RXFUNCADDR	0x04
#define MGC_O_MHDRC_RXHUBADDR	0x06
#define MGC_O_MHDRC_RXHUBPORT	0x07
#endif
/*
 *     MUSBHDRC Register bit masks
 */

/* POWER */

#define MGC_M_POWER_ISOUPDATE   0x80 
#define	MGC_M_POWER_SOFTCONN    0x40
#define	MGC_M_POWER_HSENAB	0x20
#define	MGC_M_POWER_HSMODE	0x10
#define MGC_M_POWER_RESET       0x08
#define MGC_M_POWER_RESUME      0x04
#define MGC_M_POWER_SUSPENDM    0x02
#define MGC_M_POWER_ENSUSPEND   0x01

/* TESTMODE */

#define MGC_M_TEST_FORCE_HOST	0x80
#define MGC_M_TEST_FIFO_ACCESS	0x40
#define MGC_M_TEST_FORCEFS      0x20
#define MGC_M_TEST_FORCEHS      0x10
#define MGC_M_TEST_PACKET       0x08
#define MGC_M_TEST_K            0x04
#define MGC_M_TEST_J            0x02
#define MGC_M_TEST_SE0_NAK      0x01

/* allocate for double-packet buffering (effectively doubles assigned _SIZE) */
#define MGC_M_FIFOSZ_DPB	0x10
/* allocation size (8, 16, 32, ... 4096) */
#define MGC_M_FIFOSZ_SIZE	0x0f

/* CSR0 in Peripheral and Host mode */

#define	MGC_M_CSR0_FLUSHFIFO      0x0100

/* New in 15-July-2005 (MHDRC v1.4 HDRC ) */
#define MGC_M_CSR0_H_NO_PING	0x0800

/* TxType/RxType */
#define MGC_M_TYPE_PROTO	0x30
#define MGC_S_TYPE_PROTO	4
#define MGC_M_TYPE_REMOTE_END	0xf

/* CONFIGDATA */

#define MGC_M_CONFIGDATA_MPRXE      0x80	/* auto bulk pkt combining */
#define MGC_M_CONFIGDATA_MPTXE      0x40	/* auto bulk pkt splitting */
/* TODO: was this in an older HDRC?
#define MGC_M_CONFIGDATA_DMA        0x40
*/
#define MGC_M_CONFIGDATA_BIGENDIAN  0x20
#define MGC_M_CONFIGDATA_HBRXE      0x10
#define MGC_M_CONFIGDATA_HBTXE      0x08
#define MGC_M_CONFIGDATA_DYNFIFO    0x04
#define MGC_M_CONFIGDATA_SOFTCONE   0x02
#define MGC_M_CONFIGDATA_UTMIDW     0x01   /* data width 0 => 8bits, 1 => 16bits */

/* TXCSR in Peripheral and Host mode */

#define MGC_M_TXCSR_AUTOSET       0x8000
#define MGC_M_TXCSR_ISO           0x4000
#define MGC_M_TXCSR_MODE          0x2000
#define MGC_M_TXCSR_DMAENAB       0x1000
#define MGC_M_TXCSR_FRCDATATOG    0x0800
#define MGC_M_TXCSR_DMAMODE       0x0400
#define MGC_M_TXCSR_CLRDATATOG    0x0040
#define MGC_M_TXCSR_FLUSHFIFO     0x0008
#define MGC_M_TXCSR_FIFONOTEMPTY  0x0002
#define MGC_M_TXCSR_TXPKTRDY      0x0001

/* TXCSR in Peripheral mode */

#define MGC_M_TXCSR_P_INCOMPTX    0x0080
#define MGC_M_TXCSR_P_SENTSTALL   0x0020
#define MGC_M_TXCSR_P_SENDSTALL   0x0010
#define MGC_M_TXCSR_P_UNDERRUN    0x0004

/* TXCSR in Host mode */
#define MGC_M_TXCSR_H_WR_DATATOGGLE   0x0200
#define MGC_M_TXCSR_H_DATATOGGLE      0x0100
#define MGC_M_TXCSR_H_NAKTIMEOUT  0x0080
#define MGC_M_TXCSR_H_RXSTALL     0x0020
#define MGC_M_TXCSR_H_ERROR       0x0004

/* RXCSR in Peripheral and Host mode */

#define MGC_M_RXCSR_AUTOCLEAR     0x8000
#define MGC_M_RXCSR_DMAENAB       0x2000
#define MGC_M_RXCSR_DISNYET       0x1000
#define MGC_M_RXCSR_DMAMODE       0x0800
#define MGC_M_RXCSR_INCOMPRX      0x0100
#define MGC_M_RXCSR_CLRDATATOG    0x0080
#define MGC_M_RXCSR_FLUSHFIFO     0x0010
#define MGC_M_RXCSR_DATAERR       0x0008
#define MGC_M_RXCSR_FIFOFULL      0x0002
#define MGC_M_RXCSR_RXPKTRDY      0x0001

/* RXCSR in Peripheral mode */

#define MGC_M_RXCSR_P_ISO         0x4000
#define MGC_M_RXCSR_P_SENTSTALL   0x0040
#define MGC_M_RXCSR_P_SENDSTALL   0x0020
#define MGC_M_RXCSR_P_OVERRUN     0x0004

/* RXCSR in Host mode */

#define MGC_M_RXCSR_H_AUTOREQ     0x4000
#define MGC_M_RXCSR_H_WR_DATATOGGLE   0x0400
#define MGC_M_RXCSR_H_DATATOGGLE        0x0200
#define MGC_M_RXCSR_H_RXSTALL     0x0040
#define MGC_M_RXCSR_H_REQPKT      0x0020
#define MGC_M_RXCSR_H_ERROR       0x0004

/* Added in HDRC 1.9(?) & MHDRC 1.4 */
#if 0
/* ULPI */
#define MGC_M_ULPI_VBUSCTL_USEEXTVBUSIND    0x02
#define MGC_M_ULPI_VBUSCTL_USEEXTVBUS	    0x01
#define MGC_M_ULPI_REGCTL_INT_ENABLE	    0x08
#define MGC_M_ULPI_REGCTL_READNOTWRITE	    0x04
#define MGC_M_ULPI_REGCTL_COMPLETE	    0x02
#define MGC_M_ULPI_REGCTL_REG		    0x01
/* extended config & PHY control */
#define MGC_M_ENDCOUNT_TXENDS	0x0f
#define MGC_S_ENDCOUNT_TXENDS	0
#define MGC_M_ENDCOUNT_RXENDS	0xf0
#define MGC_S_ENDCOUNT_RXENDS	4
#define MGC_M_DMARAMCFG_RAMBITS	0x0f	    /* RAMBITS-1 */
#define MGC_S_DMARAMCFG_RAMBITS	0
#define MGC_M_DMARAMCFG_DMACHS	0xf0
#define MGC_S_DMARAMCFG_DMACHS	4
#define MGC_M_PHYWAIT_WAITID	0x0f	    /* units of 4.369 ms */
#define MGC_S_PHYWAIT_WAITID	0
#define MGC_M_PHYWAIT_WAITCON	0xf0	    /* units of 533.3 ns */
#define MGC_S_PHYWAIT_WAITCON	4
#endif

/* 
 *  DRC register access macros
 */

/* Get offset for a given FIFO */
#define MGC_FIFO_OFFSET(_bEnd) (MGC_M_FIFO_EP0 + (_bEnd * 4))

#define MGC_END_OFFSET(_bEnd, _bOffset)	(0x100 + (0x10*_bEnd) + _bOffset)

#if defined(CC_MT5368) || defined(CC_MT5396)
#define MGC_BUSCTL_OFFSET(_bEnd, _bOffset)	(0x480 + (8*_bEnd) + _bOffset)
#else
#ifdef MTK_MHDRC
#define MGC_BUSCTL_OFFSET(_bEnd, _bOffset)	(0x80 + (8*_bEnd) + _bOffset)
#endif
#endif

/* indexed vs. flat register model */
#ifdef MUSB_FLAT_REG
#define MGC_SelectEnd(_pBase, _bEnd)
#define MGC_ReadCsr8(_pBase, _bOffset, _bEnd) \
    MGC_Read8(_pBase, MGC_END_OFFSET(_bEnd, _bOffset))
#define MGC_ReadCsr16(_pBase, _bOffset, _bEnd) \
    MGC_Read16(_pBase, MGC_END_OFFSET(_bEnd, _bOffset))
#define MGC_WriteCsr8(_pBase, _bOffset, _bEnd, _bData) \
    MGC_Write8(_pBase, MGC_END_OFFSET(_bEnd, _bOffset), _bData)
#define MGC_WriteCsr16(_pBase, _bOffset, _bEnd, _bData) \
    MGC_Write16(_pBase, MGC_END_OFFSET(_bEnd, _bOffset), _bData)
#else
#define MGC_SelectEnd(_pBase, _bEnd) \
    MGC_Write8(_pBase, MGC_O_HDRC_INDEX, _bEnd)
#define MGC_ReadCsr8(_pBase, _bOffset, _bEnd) \
    MGC_Read8(_pBase, (_bOffset + 0x10))
#define MGC_ReadCsr16(_pBase, _bOffset, _bEnd) \
    MGC_Read16(_pBase, (_bOffset + 0x10))
#define MGC_WriteCsr8(_pBase, _bOffset, _bEnd, _bData) \
    MGC_Write8(_pBase, (_bOffset + 0x10), _bData)
#define MGC_WriteCsr16(_pBase, _bOffset, _bEnd, _bData) \
    MGC_Write16(_pBase, (_bOffset + 0x10), _bData)
#endif

#define Reg_Read8(_offset) \
	*((volatile unsigned char *)((unsigned int)(_offset)))
 
#define Reg_Read16(_offset) \
	*((volatile unsigned short *)((unsigned int)(_offset)))
	
#define Reg_Read32(_offset) \
	*((volatile unsigned int *)((unsigned int)(_offset)))


#if 1
#define Reg_Write8(_offset, _data) \
{                                                                                            \
volatile unsigned int u4TmpVar;                                                                  \
u4TmpVar = *((volatile unsigned int*)(((unsigned int)(_offset)) & 0XFFFFFFFC)); \
u4TmpVar &= ~(((unsigned int)0xFF) << (8*((_offset) & 0x03)));                                   \
u4TmpVar |= (unsigned int)(((_data) & 0xFF) << (8*((_offset) & 0x03)));                          \
*((volatile unsigned int*)(((unsigned int)(_offset)) & 0xFFFFFFFC)) = u4TmpVar; \
if(_u4UsbQDebugLevel >= 6)\
    Printf("Reg_Write8(0x%08X, 0x%08X)\n", _offset, _data);\
}
#endif

#define Reg_Write16(_offset, _data) \
{                                                                                            \
volatile unsigned int u4TmpVar;                                                                  \
u4TmpVar = *((volatile unsigned int*)(((unsigned int)(_offset)) & 0XFFFFFFFC)); \
u4TmpVar &= ~(((unsigned int)0xFFFF) << (8*((_offset) & 0x03)));                                   \
u4TmpVar |= (unsigned int)(((_data) & 0xFFFF) << (8*((_offset) & 0x03)));                          \
*((volatile unsigned int*)(((unsigned int)(_offset)) & 0xFFFFFFFC)) = u4TmpVar; \
if(_u4UsbQDebugLevel >= 6)\
    Printf("Reg_Write16(0x%08X, 0x%08X)\n", _offset, _data);\
}

#define Reg_Write32(_offset, _data) \
{ \
    if(_u4UsbQDebugLevel >= 6)\
        Printf("Reg_Write32(0x%08X, 0x%08X)\n", _offset, _data);\
	(*((volatile unsigned int *)((unsigned int)(_offset))) = (_data));\
}

#define MGC_QREG_Read8(_offset) \
    Reg_Read8(USB_HW_BASE_ADDR + _offset)

#define MGC_ReadQMU8(_offset) \
    Reg_Read8(USB_HW_BASE_ADDR + USB_HW_QMU_OFF + _offset)

#define MGC_ReadQUCS8(_offset) \
    Reg_Read8(USB_HW_BASE_ADDR + USB_HW_QUCS_OFF + _offset)

#define MGC_ReadQIRQ8(_offset) \
    Reg_Read8(USB_HW_BASE_ADDR + USB_HW_QIRQ_OFF + _offset)

/**
 * Read a 16-bit register from the core
 * @param _pBase core base address in memory
 * @param _offset offset into the core's register space
 * @return 16-bit datum
 */
#define MGC_QREG_Read16(_offset) \
    Reg_Read16(USB_HW_BASE_ADDR + _offset)

#define MGC_ReadQMU16(_offset) \
    Reg_Read16(USB_HW_BASE_ADDR + USB_HW_QMU_OFF + _offset)

#define MGC_ReadQUCS16(_offset) \
    Reg_Read16(USB_HW_BASE_ADDR + USB_HW_QUCS_OFF + _offset)

#define MGC_ReadQIRQ16(_offset) \
    Reg_Read16(USB_HW_BASE_ADDR + USB_HW_QIRQ_OFF + _offset)


/**
 * Read a 32-bit register from the core
 * @param _pBase core base address in memory
 * @param _offset offset into the core's register space
 * @return 32-bit datum
 */
#define MGC_QREG_Read32( _offset) \
    Reg_Read32(USB_HW_BASE_ADDR + _offset)

#define MGC_ReadQMU32(  _offset) \
    Reg_Read32(USB_HW_BASE_ADDR + USB_HW_QMU_OFF + _offset)

#define MGC_ReadQUCS32(  _offset) \
    Reg_Read32(USB_HW_BASE_ADDR + USB_HW_QUCS_OFF + _offset)

#define MGC_ReadQIRQ32(  _offset) \
    Reg_Read32(USB_HW_BASE_ADDR + USB_HW_QIRQ_OFF + _offset)

#define MGC_ReadQDBGQ32(  _offset) \
    Reg_Read32(USB_HW_BASE_ADDR + USB_HW_QDBG_OFF + _offset)

/**
 * Write an 8-bit core register
 * @param _pBase core base address in memory
 * @param _offset offset into the core's register space
 * @param _data 8-bit datum
 */
#define MGC_QMU_Write8(_offset, _data) \
    Reg_Write8((USB_HW_BASE_ADDR + _offset), (_data))

#define MGC_WriteQMU8(_offset, _data) \
    Reg_Write8((USB_HW_BASE_ADDR + USB_HW_QMU_OFF + _offset), (_data))

#define MGC_WriteQUCS8(_offset, _data) \
    Reg_Write8((USB_HW_BASE_ADDR + USB_HW_QUCS_OFF + _offset), (_data))

#define MGC_WriteQIRQ8(_offset, _data) \
    Reg_Write8((USB_HW_BASE_ADDR + USB_HW_QIRQ_OFF + _offset), (_data))

/**
 * Write a 16-bit core register
 * @param _pBase core base address in memory
 * @param _offset offset into the core's register space
 * @param _data 16-bit datum
 */
#define MGC_QMU_Write16(_offset, _data) \
    Reg_Write16((USB_HW_BASE_ADDR + _offset), _data)
    //(*((volatile unsigned short *)(USB_HW_BASE_ADDR + _offset)) = _data)
/*    DBG (3, "Write16(%x, %02x)\r\n", _offset, _data); \ */

#define MGC_WriteQMU16(_offset, _data) \
    Reg_Write16((USB_HW_BASE_ADDR + USB_HW_QMU_OFF + _offset), _data)
    //(*((volatile unsigned short *)(USB_HW_BASE_ADDR + USB_HW_QMU_OFF + _offset)) = _data)

#define MGC_WriteQUCS16(_offset, _data) \
    Reg_Write16((USB_HW_BASE_ADDR + USB_HW_QUCS_OFF + _offset), _data)
    //(*((volatile unsigned short *)(USB_HW_BASE_ADDR + USB_HW_QUCS_OFF + _offset)) = _data)

#define MGC_WriteQIRQ16(_offset, _data) \
    Reg_Write16((USB_HW_BASE_ADDR + USB_HW_QIRQ_OFF + _offset), _data)
    //(*((volatile unsigned short *)(USB_HW_BASE_ADDR + USB_HW_QIRQ_OFF + _offset)) = _data)

/**
 * Write a 32-bit core register
 * @param _pBase core base address in memory
 * @param _offset offset into the core's register space
 * @param _data 32-bit datum
 */
#define MGC_QREG_Write32(_offset, _data) \
    Reg_Write32(((unsigned int)USB_HW_BASE_ADDR + _offset), _data)
    //(*((volatile unsigned int *)((unsigned int)USB_HW_BASE_ADDR + _offset)) = _data)
/*    DBG (3, "Write32(%x, %02x)\r\n", _offset, _data); \ */

#define MGC_WriteQMU32(_offset, _data) \
    Reg_Write32(((unsigned int)USB_HW_BASE_ADDR + USB_HW_QMU_OFF + _offset), _data)
    //(*((volatile unsigned int *)((unsigned int)USB_HW_BASE_ADDR + USB_HW_QMU_OFF + _offset)) = _data)

#define MGC_WriteQUCS32(_offset, _data) \
    Reg_Write32(((unsigned int)USB_HW_BASE_ADDR + USB_HW_QUCS_OFF + _offset), _data)
    //(*((volatile unsigned int *)((unsigned int)USB_HW_BASE_ADDR + USB_HW_QUCS_OFF + _offset)) = _data)

#define MGC_WriteQIRQ32(_offset, _data) \
    Reg_Write32(((unsigned int)USB_HW_BASE_ADDR + USB_HW_QIRQ_OFF + _offset), _data)
    //(*((volatile unsigned int *)((unsigned int)USB_HW_BASE_ADDR + USB_HW_QIRQ_OFF + _offset)) = _data)

#define MGC_WriteQDBGQ32(_offset, _data) \
    Reg_Write32(((unsigned int)USB_HW_BASE_ADDR + USB_HW_QDBG_OFF + _offset), _data)


/**
 * Swap the given 16-bit quantity from USB byte order (little-endian) 
 * to host byte order.
 * @param _data the 16-bit quantity
 * @return quantity in host byte order
 */
//#define MUSB_SWAP16(_data) _data

/**
 * Swap the given 32-bit quantity from USB byte order (little-endian) 
 * to host byte order.
 * @param _data the 32-bit quantity
 * @return quantity in host byte order
 */
//#define MUSB_SWAP32(_data) _data

 
/**
 * Swap the pointed-to 16-bit quantity from USB byte order (little-endian) 
 * to host byte order, in an alignment-safe manner.
 * @param _pData pointer (as unsigned char*)
 * @return quantity in host byte order
 */
//#define MUSB_SWAP16P(_pData) ((*(_pData+1) << 8) | *(_pData))

/**
 * Swap the pointed-to 24-bit quantity from USB byte order (little-endian) 
 * to host byte order, in an alignment-safe manner.
 * @param _pData pointer (as unsigned char*)
 * @return quantity in host byte order
 */
//#define MUSB_SWAP24P(_pData) ((*(_pData+2) << 16) | (*(_pData+1) << 8) | *(_pData))

/**
 * Swap the pointed-to 32-bit quantity from USB byte order (little-endian) 
 * to host byte order, in an alignment-safe manner.
 * @param _pData pointer (as unsigned char*)
 * @return quantity in host byte order
 */
//#define MUSB_SWAP32P(_pData) ((*(_pData+3) << 24) | (*(_pData+2) << 16) | (*(_pData+1) << 8) | *(_pData))

 
 
#define MGC_QReadCsr8( _bOffset, _bEnd) \
    MGC_QREG_Read8(MGC_END_OFFSET(_bEnd, _bOffset))
#define MGC_QReadCsr16(_bOffset, _bEnd) \
    MGC_QREG_Read16(MGC_END_OFFSET(_bEnd, _bOffset))
#define MGC_QWriteCsr8(  _bOffset, _bEnd, _bData) \
    MGC_QMU_Write8(  MGC_END_OFFSET(_bEnd, _bOffset), _bData)
#define MGC_QWriteCsr16(  _bOffset, _bEnd, _bData) \
    MGC_QMU_Write16( MGC_END_OFFSET(_bEnd, _bOffset), _bData)
 

#define MGC_VBUS_MASK            0x18    /* DevCtl D4 - D3 */

#endif	/* multiple inclusion protection */
