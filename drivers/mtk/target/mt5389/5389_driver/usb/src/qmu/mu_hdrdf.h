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

#ifndef __MUSB_HDR_DEFS_H__
#define __MUSB_HDR_DEFS_H__

//#include "x_hal_ic.h"
#include "mu_arch.h"
#undef BIG_ENDIAN

extern unsigned int _u4UsbQDebugLevel;

#ifndef USB_HW_BASE_ADDR
#define USB_HW_BASE_ADDR MUSB_BASE
#endif

/*add by tianhao.fei for QMU*/
#if 0 //!PORTING
#define USB_HW_QMU_OFF	0x02001000
#define USB_HW_QUCS_OFF	0x02005000
#define USB_HW_QCC_OFF	0x02005000
#define USB_HW_QIRQ_OFF	0x02010000
#define USB_HW_QDBG_OFF	0x02020000
#else
#define USB_HW_QMU_OFF	        0x00000800
#define USB_HW_QUCS_OFF	0x00000B00
#define USB_HW_QIRQ_OFF	0x00000C00
#define USB_HW_QDBG_OFF	0x00000CF0
#endif
/*end*/

#define MGC_MAX_USB_ENDS	        9
#define MGC_M_FIFO_EP0               0x20

#define MGC_O_DRC_INDEX             0x0E
#define MGC_O_DRC_FIFOSIZE	        0x1F

/* Interrupt register bit masks */
#define MGC_M_INTR_SUSPEND        0x01
#define MGC_M_INTR_RESUME          0x02
#define MGC_M_INTR_RESET             0x04
#define MGC_M_INTR_BABBLE          0x04
#define MGC_M_INTR_SOF                0x08 
#define MGC_M_INTR_CONNECT        0x10
#define MGC_M_INTR_DISCONNECT   0x20
#define MGC_M_INTR_SESSREQ         0x40
#define MGC_M_INTR_VBUSERROR     0x80               /* FOR SESSION END */

#define MGC_M_INTR_EP0                 0x01              /* FOR EP0 INTERRUPT */

/* DEVCTL */

#define MGC_M_DEVCTL_BDEVICE    0x80   
#define MGC_M_DEVCTL_FSDEV      0x40
#define MGC_M_DEVCTL_LSDEV      0x20
#define MGC_M_DEVCTL_HM         0x04
#define MGC_M_DEVCTL_HR         0x02
#define MGC_M_DEVCTL_SESSION    0x01

/* CSR0 */
#define MGC_M_CSR0_TXPKTRDY       0x0002
#define MGC_M_CSR0_RXPKTRDY       0x0001

/* CSR0 in Peripheral mode */
#define MGC_M_CSR0_P_SVDSETUPEND  0x0080
#define MGC_M_CSR0_P_SVDRXPKTRDY  0x0040
#define MGC_M_CSR0_P_SENDSTALL    0x0020
#define MGC_M_CSR0_P_SETUPEND     0x0010
#define MGC_M_CSR0_P_DATAEND      0x0008
#define MGC_M_CSR0_P_SENTSTALL    0x0004

/* CSR0 in Host mode */
#define	MGC_M_CSR0_H_NAKTIMEOUT   0x0080
#define MGC_M_CSR0_H_STATUSPKT    0x0040
#define MGC_M_CSR0_H_REQPKT       0x0020
#define MGC_M_CSR0_H_ERROR        0x0010
#define MGC_M_CSR0_H_SETUPPKT     0x0008
#define MGC_M_CSR0_H_RXSTALL      0x0004
 
/* Vbus values */
#define MGC_VBUS_BELOW_SESSION_END  0
#define MGC_VBUS_ABOVE_SESSION_END  1
#define MGC_VBUS_ABOVE_AVALID       2
#define MGC_VBUS_ABOVE_VBUS_VALID   3
#define MGC_VBUS_ERROR		    	0xff

#define FEATURE_SOFT_CONNECT 1
#define FEATURE_DMA_PRESENT 2
#define FEATURE_HDRC_FS 4
#define FEATURE_HIGH_BW 8
#define FEATURE_DFIFO 16
#define FEATURE_MULTILAYER 32
#define FEATURE_I2C 64

/*
 *     MUSBHDRC Register map 
 */

/* Common USB registers */

#define MGC_O_HDRC_FADDR	0x00	/* 8-bit */
//#define MGC_O_HDRC_FADDR	0x480
#define MGC_O_HDRC_TXFADDR( _n )  (0x480 + (((_n))*8) )
#define MGC_O_HDRC_TXHUBADDR( _n )  (0x482 + (((_n))*8) )
#define MGC_O_HDRC_TXPORTADDR( _n ) (0x483 + (((_n))*8) )
#define MGC_O_HDRC_RXFADDR( _n )  (0x484 + (((_n))*8) )
#define MGC_O_HDRC_RXHUBADDR( _n )  (0x486 + (((_n))*8) )
#define MGC_O_HDRC_RXPORTADDR( _n ) (0x487 + (((_n))*8) )
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
#define   MGC_O_HDRC_PWRUPCNT    0x61

/* These are actually indexed: */
#define MGC_O_HDRC_TXFIFOSZ	0x62	/* 8-bit (see masks) */
#define MGC_O_HDRC_RXFIFOSZ	0x63	/* 8-bit (see masks) */
#define MGC_O_HDRC_TXFIFOADD	0x64	/* 16-bit offset shifted right 3 */
#define MGC_O_HDRC_RXFIFOADD	0x66	/* 16-bit offset shifted right 3 */

/* ULPI & Additional Configuration Register */
#define MGC_O_HDRC_BUSPERF1    0x70
#define MGC_O_HDRC_BUSPERF2    0x72
#define MGC_O_HDRC_BUSPERF3    0x74
#define MGC_O_HDRC_LINKINFO     0x7A
#define MGC_O_HDRC_VPLEN         0x7B
#define MGC_O_HDRC_HSEOF1       0x7C
#define MGC_O_HDRC_FSEOF1       0x7D
#define MGC_O_HDRC_LSEOF1       0x7E
#define MGC_O_HDRC_VCONTROL	0x68
#define MGC_O_HDRC_VSTATUS	0x68
#define MGC_O_HDRC_HWVERS	0x6C
#define MGC_O_HDRC_EPINFO	0x78
#define MGC_O_HDRC_RAMINFO	0x79

/* RqPktCount */
#define MGC_O_HDRC_RQPKTCOUNT_BASE	0x300
#define MGC_O_HDRC_RQPKTCOUNT_1  0x304
#define MGC_O_HDRC_RQPKTCOUNT_2  0x308
#define MGC_O_HDRC_RQPKTCOUNT_3  0x30C
#define MGC_O_HDRC_RQPKTCOUNT_4  0x310
#define MGC_O_HDRC_RQPKTCOUNT_5  0x314

/* Endpoint registers */
#define MGC_O_HDRC_INDEXEPBASE  0x10
#define MGC_O_HDRC_TXMAXP	0x00
#define MGC_O_HDRC_TXCSR	0x02
#define MGC_O_HDRC_CSR0		MGC_O_HDRC_TXCSR	/* re-used for EP0 */
#define MGC_O_HDRC_RXMAXP	0x04
#define MGC_O_HDRC_RXCSR	0x06
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
#if 1
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

/*wimax only*/
/*
#define MGC_O_QMU_RQCSRBE0	0x0090
#define MGC_O_QMU_RQSARBE0	0x0094
#define MGC_O_QMU_RQCPRBE0	0x0098
#define MGC_O_QMU_RQCSRBE1	0x00A0
#define MGC_O_QMU_RQSARBE1	0x00A4
#define MGC_O_QMU_RQCPRBE1	0x00A8
#define MGC_O_QMU_RQCSRBE2	0x00B0
#define MGC_O_QMU_RQSARBE2	0x00B4
#define MGC_O_QMU_RQCPRBE2	0x00B8
#define MGC_O_QMU_RQCSRBE3	0x00C0
#define MGC_O_QMU_RQSARBE3	0x00C4
#define MGC_O_QMU_RQCPRBE3	0x00C8
*/
#define MGC_O_QMU_RQTR_BASE	0x0090
#define MGC_O_QMU_RQTR(n)		(MGC_O_QMU_RQTR_BASE+0x4*(n))
/*
#define MGC_O_QMU_DSIR			0x00D0
#define MGC_O_QMU_RBEDISR		0x00D4
#define MGC_O_QMU_DTBUDTR		0x00D8
*/
/*end*/

#define MGC_O_QMU_RQLDPR0		0x0100
#define MGC_O_QMU_RQLDPR1		0x0104
#define MGC_O_QMU_RQLDPR2		0x0108
#define MGC_O_QMU_RQLDPR3		0x010A
#define MGC_O_QMU_RQLDPR(n) 	(MGC_O_QMU_RQLDPR0+0x4*((n)-1))

/*wimax only*/
//#define MGC_O_QMU_RQLDPRBE0		0x0120
//#define MGC_O_QMU_RQLDPRBE1		0x0124
//#define MGC_O_QMU_RQLDPRBE2		0x0128
//#define MGC_O_QMU_RQLDPRBE3		0x012A
/*end*/

#define MGC_O_QMU_TQCSR0	0x0200
#define MGC_O_QMU_TQSAR0	0x0204
#define MGC_O_QMU_TQCPR0	0x0208
#define MGC_O_QMU_TQCSR1	0x0210
#define MGC_O_QMU_TQSAR1	0x0214
#define MGC_O_QMU_TQCPR1	0x0218
#define MGC_O_QMU_TQCSR2	0x0220
#define MGC_O_QMU_TQSAR2	0x0224
#define MGC_O_QMU_TQCPR2	0x0228
#define MGC_O_QMU_TQCSR3	0x0230
#define MGC_O_QMU_TQSAR3	0x0234
#define MGC_O_QMU_TQCPR3	0x0238
#define MGC_O_QMU_TQCSR4	0x0240
#define MGC_O_QMU_TQSAR4	0x0244
#define MGC_O_QMU_TQCPR4	0x0248
#define MGC_O_QMU_TQCSR5	0x0250
#define MGC_O_QMU_TQSAR5	0x0254
#define MGC_O_QMU_TQCPR5	0x0258
#define MGC_O_QMU_TQCSR(n) (MGC_O_QMU_TQCSR0+0x0010*((n)-1))
#define MGC_O_QMU_TQSAR(n) (MGC_O_QMU_TQSAR0+0x0010*((n)-1))
#define MGC_O_QMU_TQCPR(n) (MGC_O_QMU_TQCPR0+0x0010*((n)-1))

#define MGC_O_QMU_QAR		0x0300

#define MGC_O_QUCS_USBGCSR	0x0000
/*wimax only*/
#define MGC_O_QUCS_TQUSBSC_BASE		0x0000
#define MGC_O_QUCS_TQUSBSC(n)			(MGC_O_QUCS_TQUSBSC_BASE+0x4*(n))
#define MGC_O_QUCS_TQSSIZE_BASE		0x0000
#define MGC_O_QUCS_TQSSIZE(n)			(MGC_O_QUCS_TQSSIZE_BASE+0x4*(n))
#define MGC_O_QUCS_TQSPCOUNT_BASE	0x0002
#define MGC_O_QUCS_TQSPCOUNT(n)		(MGC_O_QUCS_TQSPCOUNT_BASE+0x4*(n))
#define MGC_O_QUCS_FW1			0x0010
#define MGC_O_QUCS_FW2			0x0014
#define MGC_O_QUCS_RQHDR_BASE	0x0020
#define MGC_O_QUCS_RQHDR(n)		(MGC_O_QUCS_RQHDR_BASE+0x4*(n))
//#define MGC_O_QUCS_R2QHDR		0x0028
//#define MGC_O_QUCS_BEQHDR		0x002C
/*end*/

//0x59CXX
#define MGC_O_QIRQ_QISAR		0x0000
#define MGC_O_QIRQ_QIMR		0x0004
#define MGC_O_QIRQ_QIMCR		0x0008
#define MGC_O_QIRQ_QIMSR		0x000C
/*wimax only*/
#define MGC_O_QIRQ_QSISAR		0x0010
#define MGC_O_QIRQ_DSIMR		0x0014
#define MGC_O_QIRQ_DSIMCR		0x0018
#define MGC_O_QIRQ_DSIMSR		0x001C
#define MGC_O_QIRQ_DSICR		0x0020
/*end*/
#ifdef CC_MT5368 ||CC_MT5389
#define MGC_O_QIRQ_IOCDISR      0x0030
#endif

#define MGC_O_QIRQ_TEPEMPR	0x0060
#define MGC_O_QIRQ_TEPEMPMR	0x0064
#define MGC_O_QIRQ_TEPEMPMCR	0x0068
#define MGC_O_QIRQ_TEPEMPMSR	0x006C
#define MGC_O_QIRQ_REPEMPR	0x0070
#define MGC_O_QIRQ_REPEMPMR	0x0074
#define MGC_O_QIRQ_REPEMPMCR	0x0078
#define MGC_O_QIRQ_REPEMPMSR	0x007C

/*wimax only
#define MGC_O_QIRQ_RBEQEIR		0x0080
#define MGC_O_QIRQ_RBEQEIMR	0x0084
#define MGC_O_QIRQ_RBEQEIMCR	0x0088
#define MGC_O_QIRQ_RBEQEIMSR	0x008C
end*/

// 0x29CXX

#define MGC_O_QIRQ_RQEIR		0x0090
#define MGC_O_QIRQ_RQEIMR		0x0094
#define MGC_O_QIRQ_RQEIMCR	0x0098
#define MGC_O_QIRQ_RQEIMSR	0x009C
#define MGC_O_QIRQ_REPEIR		0x00A0
#define MGC_O_QIRQ_REPEIMR	0x00A4
#define MGC_O_QIRQ_REPEIMCR	0x00A8
#define MGC_O_QIRQ_REPEIMSR	0x00AC
#define MGC_O_QIRQ_TQEIR		0x00B0
#define MGC_O_QIRQ_TQEIMR		0x00B4
#define MGC_O_QIRQ_TQEIMCR	0x00B8
#define MGC_O_QIRQ_TQEIMSR	0x00BC
#define MGC_O_QIRQ_TEPEIR		0x00C0
#define MGC_O_QIRQ_TEPEIMR	0x00C4
#define MGC_O_QIRQ_TEPEIMCR	0x00C8
#define MGC_O_QIRQ_TEPEIMSR	0x00CC


#define MGC_O_QDBG_DFCR	0x0000
#define MGC_O_QDBG_DFMR	0x0004

/*Add by tianhao.fei for QMU*/
/**
 *  @brief  Queue Control value Definition
 */                            
#define DQMU_QUE_START	0x00000001
#define DQMU_QUE_RESUME	0x00000002
#define DQMU_QUE_STOP		0x00000004
#define DQMU_QUE_ACTIVE	0x00008000

/**
 *  @brief  USB QMU Special Control USBGCSR value Definition
 */    
 //#define USB_QMU_EN 			0x00000001
#define USB_QMU_Tx0_EN 			0x00000001
#define USB_QMU_Tx_EN(n) 			(USB_QMU_Tx0_EN<<((n)-1))
#if 0 //!PORTING
#define USB_QMU_Rx0_EN 			0x00000020
#else
#define USB_QMU_Rx0_EN 			0x00010000
#endif
#define USB_QMU_Rx_EN(n) 			(USB_QMU_Rx0_EN<<((n)-1))
#define USB_QMU_HIFEVT_EN	 		0x00000010
#define USB_QMU_HIFCMD_EN	 		0x00000200
#define DQMU_SW_RESET		0x00010000
#define DQMU_CS16B_EN		0x80000000
#define DQMU_TQ0CS_EN		0x00010000
#define DQMU_TQCS_EN(n)	(DQMU_TQ0CS_EN<<((n)-1))
//#define DQMU_BEQSEQ_EN	0x00008000
//#define DQMU_BEQSHARE_EN	0x00004000
#define DQMU_RQ0CS_EN		0x00000001
#define DQMU_RQCS_EN(n)	(DQMU_RQ0CS_EN<<((n)-1))
#define DQMU_TX0_ZLP 		0x01000000
#define DQMU_TX_ZLP(n)		(DQMU_TX0_ZLP<<((n)-1))
#define DQMU_TX0_MULTIPLE	0x00010000
#define DQMU_TX_MULTIPLE(n)	(DQMU_TX0_MULTIPLE<<((n)-1))
#define DQMU_TX_PADDING	0x00000002
#define DQMU_TX_SDUHDR	0x00000001
#define DQMU_RX0_MULTIPLE	0x00010000
#define DQMU_RX_MULTIPLE(n)	(DQMU_RX0_MULTIPLE<<((n)-1))
#define DQMU_RX_SDUHDR	0x00000001
#define DQMU_RX0_ZLP 		0x01000000
#define DQMU_RX_ZLP(n)		(DQMU_RX0_ZLP<<((n)-1))

#define DQMU_M_TXEP_ERR	0x10000000
#define DQMU_M_TXQ_ERR	0x08000000
#define DQMU_M_RXEP_ERR	0x04000000
#define DQMU_M_RXQ_ERR	0x02000000
//#define DQMU_M_BEQ_ERR	0x01000000
//#define DQMU_M_BEQ_DEQ	0x00100000
//#define DQMU_M_BEQ_EMPTY	0x00080000
//#define DQMU_M_BEQ_DONE	0x00040000
#define DQMU_M_RQ_EMPTY	0x00020000
#define DQMU_M_TQ_EMPTY	0x00010000
#define DQMU_M_RX0_EMPTY	0x00000001
#define DQMU_M_RX_EMPTY(n) 	(DQMU_M_RX0_EMPTY<<((n)-1))
#define DQMU_M_TX0_EMPTY	0x00000001
#define DQMU_M_TX_EMPTY(n) 	(DQMU_M_TX0_EMPTY<<((n)-1))
#define DQMU_M_RX0_DONE	0x00000100
#define DQMU_M_RX_DONE(n)	(DQMU_M_RX0_DONE<<((n)-1))
#define DQMU_M_TX0_DONE	0x00000001
#define DQMU_M_TX_DONE(n)	(DQMU_M_TX0_DONE<<((n)-1))

#define DQMU_M_RX0_ZLP_ERR	0x01000000
#define DQMU_M_RX_ZLP_ERR(n)	(DQMU_M_RX0_ZLP_ERR<<((n)-1))
#define DQMU_M_RX0_HDR_ERR	0x00010000
#define DQMU_M_RX_HDR_ERR(n)	(DQMU_M_RX0_HDR_ERR<<((n)-1))
#define DQMU_M_RX0_LEN_ERR	0x00000100
#define DQMU_M_RX_LEN_ERR(n)	(DQMU_M_RX0_LEN_ERR<<((n)-1))
#define DQMU_M_RX0_GPDCS_ERR		0x00000001
#define DQMU_M_RX_GPDCS_ERR(n)	(DQMU_M_RX0_GPDCS_ERR<<((n)-1))

#define DQMU_M_TX0_LEN_ERR	0x00010000
#define DQMU_M_TX_LEN_ERR(n)	(DQMU_M_TX0_LEN_ERR<<((n)-1))
#define DQMU_M_TX0_GPDCS_ERR	0x00000100
#define DQMU_M_TX_GPDCS_ERR(n)	(DQMU_M_TX0_GPDCS_ERR<<((n)-1))
#define DQMU_M_TX0_BDCS_ERR		0x00000001
#define DQMU_M_TX_BDCS_ERR(n)	(DQMU_M_TX0_BDCS_ERR<<((n)-1))

#define DQMU_M_TX0_EP_ERR		0x00000001
#define DQMU_M_TX_EP_ERR(n)	(DQMU_M_TX0_EP_ERR<<((n)-1))

#define DQMU_M_RX0_EP_ERR		0x00000001
#define DQMU_M_RX_EP_ERR(n)	(DQMU_M_RX0_EP_ERR<<((n)-1)) 

#ifdef CC_MT5368 || CC_MT5389
#define MGC_O_QMU_TXIOC_DIS(n) (0x0001<<((n)-1))
#define MGC_O_QMU_RXIOC_DIS(n) (0x0100<<((n)-1))

#endif


#define MGC_O_QDBG_DFCR	0x0000
#define MGC_O_QDBG_DFMR	0x0004

/*
 *     MUSBHDRC Register bit masks
 */

/* POWER */

#define MGC_M_POWER_ISOUPDATE   0x80 
#define MGC_M_POWER_SOFTCONN    0x40
#define MGC_M_POWER_HSENAB	0x20
#define MGC_M_POWER_HSMODE	0x10
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

#define MGC_M_CSR0_FLUSHFIFO      0x0100

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
#define MGC_M_TXCSR_SETDATATOG    0x0200
#define MGC_M_TXCSR_DATATOGSTATUS  0x0100
#define MGC_M_TXCSR_INCOMPTX    0x0080
#define MGC_M_TXCSR_CLRDATATOG    0x0040
#define MGC_M_TXCSR_FLUSHFIFO     0x0008
#define MGC_M_TXCSR_FIFONOTEMPTY  0x0002
#define MGC_M_TXCSR_TXPKTRDY      0x0001
#define MGC_M_TXCSR_SETTXPKTRDYTWICE   0x0100

/* TXCSR in Peripheral mode */
#define MGC_M_TXCSR_P_SENTSTALL   0x0020
#define MGC_M_TXCSR_P_SENDSTALL   0x0010
#define MGC_M_TXCSR_P_UNDERRUN    0x0004

/* TXCSR in Host mode */
#define MGC_M_TXCSR_H_NAKTIMEOUT  0x0080
#define MGC_M_TXCSR_H_RXSTALL     0x0020
#define MGC_M_TXCSR_H_ERROR       0x0004

/* RXCSR in Peripheral and Host mode */
#define MGC_M_RXCSR_AUTOCLEAR     0x8000
#define MGC_M_RXCSR_DMAENAB       0x2000
#define MGC_M_RXCSR_DISNYET       0x1000
#define MGC_M_RXCSR_PIDERROR    0x1000
#define MGC_M_RXCSR_DMAMODE       0x0800
#define MGC_M_RXCSR_SETDATATOG    0x0400
#define MGC_M_RXCSR_DATATOGSTATUS 0x0200
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
#define MGC_M_RXCSR_H_RXSTALL     0x0040
#define MGC_M_RXCSR_H_REQPKT      0x0020
#define MGC_M_RXCSR_H_ERROR       0x0004
#define MGC_M_RXCSR_H_NAKTIMEOUT  0x0008

/* Added in HDRC 1.9(?) & MHDRC 1.4 */
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



#define MGC_O_HSDMA_BASE    0x200
#define MGC_O_HSDMA_INTR    0x200
#ifdef CC_MT5368 || CC_MT5389
#define MGC_O_HSDMA_INTR_MASK    0x201   // 0x202
#define MGC_O_HSDMA_INTR_CLEAR   0x202
#define MGC_O_HSDMA_INTR_SET        0x203
#endif

#define MGC_O_HSDMA_LIMITER 0x210
#define MGC_O_HSDMA_CONFIG    0x220

#define MGC_O_HSDMA_CONTROLLER( _n )  (0x204 + (((_n)-1)*0x10) )
#define MGC_O_HSDMA_ADDRESS1( _n )     (0x208 + (((_n)-1)*0x10) )
#define MGC_O_HSDMA_COUNT1( _n )       (0x20c + (((_n)-1)*0x10) )
#define MGC_O_HSDMA_REALCNT( _n )     (0x280 + (((_n)-1)*0x10) )


/* debug flag */
#define MGC_O_HSDMA_DFCR     0x600
#define MGC_O_HSDMA_DFER     0x602
#define MGC_O_DEBUG_SRP      0x680
#define MGC_O_DEBUG_SRAMA    0x684
#define MGC_O_DEBUG_SRAMD    0x688
#define MGC_O_DEBUG_OPSTATE  0x610

#define MGC_S_DEBUG_SRP      8
#define MGC_S_DEBUG_SUP      0
#define MGC_S_SRAMDBG        16

/* data toggle */
#define MGC_O_RXTOG        0x80
#define MGC_O_RXTOGEN      0x82
#define MGC_O_TXTOG        0x84
#define MGC_O_TXTOGEN      0x86

/* control register (16-bit): */
#define MGC_S_HSDMA_ENABLE	0
#define MGC_S_HSDMA_TRANSMIT	1
#define MGC_S_HSDMA_MODE1	2
#define MGC_S_HSDMA_IRQENABLE	3
#define MGC_S_HSDMA_ENDPOINT	4
#define MGC_S_HSDMA_BUSERROR	8
#define MGC_S_HSDMA_BURSTMODE	9
#define MGC_M_HSDMA_BURSTMODE	(3 << MGC_S_HSDMA_BURSTMODE)
#define MGC_HSDMA_BURSTMODE_UNSPEC  0
#define MGC_HSDMA_BURSTMODE_INCR4   1
#define MGC_HSDMA_BURSTMODE_INCR8   2
#define MGC_HSDMA_BURSTMODE_INCR16  3

/* USB MAC extra register */
 #define MGC_M_MACEXTRA_1KBOUNDARY_PROT  0x00000008

/* 
 *  DRC register access macros
 */

/* Get offset for a given FIFO */
#define MGC_FIFO_OFFSET(_bEnd) (MGC_M_FIFO_EP0 + (_bEnd * 4))

#define MGC_END_OFFSET(_bEnd, _bOffset)	(0x100 + (0x10*_bEnd) + _bOffset)

#define MGC_SelectEnd(_bEnd)

#if 0 //!PORTING
#define MGC_QREG_Read8(  _offset) *((volatile unsigned char *)(USB_HW_BASE_ADDR + _offset))

#define MGC_ReadQMU8(  _offset) *((volatile unsigned char *)(USB_HW_BASE_ADDR + USB_HW_QMU_OFF + _offset))

#define MGC_ReadQUCS8(  _offset) *((volatile unsigned char *)(USB_HW_BASE_ADDR + USB_HW_QUCS_OFF + _offset))

#define MGC_ReadQIRQ8(  _offset) *((volatile unsigned char *)(USB_HW_BASE_ADDR + USB_HW_QIRQ_OFF + _offset))

/**
 * Read a 16-bit register from the core
 * @param _pBase core base address in memory
 * @param _offset offset into the core's register space
 * @return 16-bit datum
 */
#define MGC_QREG_Read16(_offset) *((volatile unsigned short *)(USB_HW_BASE_ADDR + _offset))

#define MGC_ReadQMU16(  _offset) *((volatile unsigned short *)(USB_HW_BASE_ADDR + USB_HW_QMU_OFF + _offset))

#define MGC_ReadQUCS16(  _offset) *((volatile unsigned short *)(USB_HW_BASE_ADDR + USB_HW_QUCS_OFF + _offset))

#define MGC_ReadQIRQ16(  _offset) *((volatile unsigned short *)(USB_HW_BASE_ADDR + USB_HW_QIRQ_OFF + _offset))


/**
 * Read a 32-bit register from the core
 * @param _pBase core base address in memory
 * @param _offset offset into the core's register space
 * @return 32-bit datum
 */
#define MGC_QREG_Read32( _offset) *((volatile unsigned int *)(USB_HW_BASE_ADDR + _offset))

#define MGC_ReadQMU32(  _offset) *((volatile unsigned int *)(USB_HW_BASE_ADDR + USB_HW_QMU_OFF + _offset))

#define MGC_ReadQUCS32(  _offset) *((volatile unsigned int *)(USB_HW_BASE_ADDR + USB_HW_QUCS_OFF + _offset))

#define MGC_ReadQIRQ32(  _offset) *((volatile unsigned int *)(USB_HW_BASE_ADDR + USB_HW_QIRQ_OFF + _offset))

/**
 * Write an 8-bit core register
 * @param _pBase core base address in memory
 * @param _offset offset into the core's register space
 * @param _data 8-bit datum
 */
#define MGC_QMU_Write8(_offset, _data) \
    (*((volatile unsigned char *)(USB_HW_BASE_ADDR + _offset)) = _data)
/*    DBG (3, "Write8(%x, %02x)\r\n", _offset, _data); \ */

#define MGC_WriteQMU8(_offset, _data) \
    (*((volatile unsigned char *)(USB_HW_BASE_ADDR + USB_HW_QMU_OFF + _offset)) = _data)

#define MGC_WriteQUCS8(_offset, _data) \
    (*((volatile unsigned char *)(USB_HW_BASE_ADDR + USB_HW_QUCS_OFF + _offset)) = _data)

#define MGC_WriteQIRQ8(_offset, _data) \
    (*((volatile unsigned char *)(USB_HW_BASE_ADDR + USB_HW_QIRQ_OFF + _offset)) = _data)

/**
 * Write a 16-bit core register
 * @param _pBase core base address in memory
 * @param _offset offset into the core's register space
 * @param _data 16-bit datum
 */
#define MGC_QMU_Write16(_offset, _data) \
    (*((volatile unsigned short *)(USB_HW_BASE_ADDR + _offset)) = _data)
/*    DBG (3, "Write16(%x, %02x)\r\n", _offset, _data); \ */

#define MGC_WriteQMU16(_offset, _data) \
    (*((volatile unsigned short *)(USB_HW_BASE_ADDR + USB_HW_QMU_OFF + _offset)) = _data)

#define MGC_WriteQUCS16(_offset, _data) \
    (*((volatile unsigned short *)(USB_HW_BASE_ADDR + USB_HW_QUCS_OFF + _offset)) = _data)

#define MGC_WriteQIRQ16(_offset, _data) \
    (*((volatile unsigned short *)(USB_HW_BASE_ADDR + USB_HW_QIRQ_OFF + _offset)) = _data)

/**
 * Write a 32-bit core register
 * @param _pBase core base address in memory
 * @param _offset offset into the core's register space
 * @param _data 32-bit datum
 */
#define MGC_QREG_Write32(_offset, _data) \
    (*((volatile unsigned int *)(USB_HW_BASE_ADDR + _offset)) = _data)
/*    DBG (3, "Write32(%x, %02x)\r\n", _offset, _data); \ */

#define MGC_WriteQMU32(_offset, _data) \
    (*((volatile unsigned int *)(USB_HW_BASE_ADDR + USB_HW_QMU_OFF + _offset)) = _data)

#define MGC_WriteQUCS32(_offset, _data) \
    (*((volatile unsigned int *)(USB_HW_BASE_ADDR + USB_HW_QUCS_OFF + _offset)) = _data)

#define MGC_WriteQIRQ32(_offset, _data) \
    (*((volatile unsigned int *)(USB_HW_BASE_ADDR + USB_HW_QIRQ_OFF + _offset)) = _data)


#define Reg_Read8(_offset) \
	*((volatile unsigned char *)(_offset))
 
#define Reg_Read16(_offset) \
	*((volatile unsigned short *)(_offset))
	
#define Reg_Read32(_offset) \
	*((volatile unsigned int *)(_offset))
	
#define Reg_Write8(_offset, _data) \
	(*((volatile unsigned char *)( _offset)) = _data) 
 
#define Reg_Write16(_offset, _data) \
	(*((volatile unsigned short *)( _offset)) = _data) 
	
#define Reg_Write32(_offset, _data) \
	(*((volatile unsigned int *)( _offset)) = _data)
#else

#define Reg_Read8(_offset) \
	*((volatile unsigned char *)((unsigned int)(_offset)))
 
#define Reg_Read16(_offset) \
	*((volatile unsigned short *)((unsigned int)(_offset)))
	
#define Reg_Read32(_offset) \
	*((volatile unsigned int *)((unsigned int)(_offset)))

#if 1//PORTING
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
#endif

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

#endif

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

#define MGC_MIN(_n1_, _n2_) (_n2_ < _n1_ ? _n2_ : _n1_)
#define MGC_MAX(_n1_, _n2_) (_n2_ > _n1_ ? _n2_ : _n1_)


#endif	/* multiple inclusion protection */
