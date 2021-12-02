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

#ifndef __MUSB_FDRC_DEFS_H__
#define __MUSB_FDRC_DEFS_H__

/*
 * FDRC-specific definitions
 * $Revision: #1 $
 */

#include "mu_drcdf.h"

/* Define MUSBFDRC register addresses */

#define MGC_O_FDRC_FADDR      0x00   /* 8 bit */
#define MGC_O_FDRC_POWER      0x01   /* 8 bit */
#define MGC_O_FDRC_INTRTX1    0x02
#define MGC_O_FDRC_INTRTX2    0x03
#define MGC_O_FDRC_INTRRX1    0x04
#define MGC_O_FDRC_INTRRX2    0x05
#define MGC_O_FDRC_INTRUSB    0x06
#define MGC_O_FDRC_INTRTX1E   0x07
#define MGC_O_FDRC_INTRTX2E   0x08
#define MGC_O_FDRC_INTRRX1E   0x09
#define MGC_O_FDRC_INTRRX2E   0x0A
#define MGC_O_FDRC_INTRUSBE   0x0B
#define MGC_O_FDRC_FRAME1     0x0C
#define MGC_O_FDRC_FRAME2     0x0D
#define MGC_O_FDRC_INDEX      0x0E
#define MGC_O_FDRC_DEVCTL     0x0F
#define MGC_O_FDRC_TXMAXP     0x10
#define MGC_O_FDRC_CSR0       0x11
#define MGC_O_FDRC_TXCSR1     0x11
#define	MGC_O_FDRC_CSR02      0x12
#define MGC_O_FDRC_TXCSR2     0x12
#define MGC_O_FDRC_RXMAXP     0x13
#define MGC_O_FDRC_RXCSR1     0x14
#define MGC_O_FDRC_RXCSR2     0x15
#define MGC_O_FDRC_COUNT0     0x16
#define MGC_O_FDRC_RXCOUNT1   0x16
#define MGC_O_FDRC_RXCOUNT2   0x17
#define MGC_O_FDRC_TXTYPE     0x18
#define MGC_O_FDRC_TXINTERVAL 0x19
#define	MGC_O_FDRC_NAKLIMIT0  0x19
#define MGC_O_FDRC_RXTYPE     0x1A
#define MGC_O_FDRC_RXINTERVAL 0x1B

/* Dynamic FIFO sizing: */
#define MGC_O_FDRC_TXFIFO1	0x1C	/* lower 8-bits of 8-byte-aligned address */
#define MGC_O_FDRC_TXFIFO2	0x1D	/* 3-bit size, double-buffer flag, upper 4 address bits */
#define MGC_O_FDRC_RXFIFO1	0x1E	/* lower 8-bits of 8-byte-aligned address */
#define MGC_O_FDRC_RXFIFO2	0x1F	/* 3-bit size, double-buffer flag, upper 4 address bits */

/* No dynamic FIFO sizing: */
#define	MGC_O_FDRC_FIFOSIZE	 0x1F
#define	MGC_O_FDRC_CONFIGDATA	 0x1F

/* CSR02 */
#define MGC_M_CSR02_FLUSHFIFO	0x01

/* TXCSR1 bit masks mode-agnostic */
#define MGC_M_TXCSR1_TXPKTRDY      0x01
#define MGC_M_TXCSR1_FIFONOTEMPTY  0x02
#define MGC_M_TXCSR1_FLUSHFIFO     0x08
#define MGC_M_TXCSR1_CLRDATATOG    0x40

/* TXCSR1 bit masks slave mode */
#define MGC_M_TXCSR1_P_UNDERRUN    0x04
#define MGC_M_TXCSR1_P_SENDSTALL   0x10       
#define MGC_M_TXCSR1_P_SENTSTALL   0x20

/* TXCSR1 bit masks MASTER MODE */ 
#define MGC_M_TXCSR1_H_ERROR       0x04
#define MGC_M_TXCSR1_H_RXSTALL     0x20

/* RXCSR1 bit masks mode-agnostic */
#define MGC_M_RXCSR1_RXPKTRDY      0x01
#define MGC_M_RXCSR1_FIFOFULL      0x02
#define MGC_M_RXCSR1_DATAERR       0x08
#define MGC_M_RXCSR1_FLUSHFIFO     0x10       
#define MGC_M_RXCSR1_CLRDATATOG    0x80

/* RXCSR1 bit masks slave mode */
#define MGC_M_RXCSR1_P_OVERRUN     0x04
#define MGC_M_RXCSR1_P_SENDSTALL   0x20
#define MGC_M_RXCSR1_P_SENTSTALL   0x40

/* RXCSR1 bit masks MASTER MODE */
#define MGC_M_RXCSR1_H_ERROR      0x04
#define MGC_M_RXCSR1_H_REQPKT     0x20
#define MGC_M_RXCSR1_H_RXSTALL    0x40

/* TXCSR2 bit masks** THERE IS NO DIFFERENCE FOR MASTER AND SLAVE MODE */
#define MGC_M_TXCSR2_DMAMODE  0x04
#define MGC_M_TXCSR2_FRCDT    0x08
#define MGC_M_TXCSR2_DMAENAB  0x10
#define MGC_M_TXCSR2_MODE     0x20
#define MGC_M_TXCSR2_ISO      0x40
#define MGC_M_TXCSR2_AUTOSET  0x80

/* RXCSR2 bit masks */
#define MGC_M_RXCSR2_DMAMODE   0x10
#define MGC_M_RXCSR2_DMAENAB   0x20
#define MGC_M_RXCSR2_P_ISO     0x40   /* in peripheral mode */
#define MGC_M_RXCSR2_H_AUTOREQ 0x40   /* in host mode */
#define MGC_M_RXCSR2_AUTOCLEAR 0x80 

/* TxType/RxType */
#define MGC_M_TYPE_PROTO	0x30
#define MGC_S_TYPE_PROTO	4
#define MGC_M_TYPE_REMOTE_END	0xf

/* For POWER REGISTER */
#define MGC_M_POWER_ENSUSPEND 0x01
#define MGC_M_POWER_SUSPENDM  0x02
#define MGC_M_POWER_RESUME    0x04
#define MGC_M_POWER_RESET     0x08
#define MGC_M_POWER_VBUSLO    0x10
#define MGC_M_POWER_VBUSSESS  0x20
#define MGC_M_POWER_VBUSVAL   0x40
#define MGC_M_POWER_ISOUP     0x80

/* FOR EP0 INTERRUPT in INTRTX1 register */
#define MGC_M_INTR_EP1      0x02

/* dynamic FIFOs */
#define MGC_M_FIFO_DPB	    0x10
#define MGC_M_FIFO_SZ	    0xe0
#define MGC_S_FIFO_SZ	    5

/* 
 *  DRC register access macros
 */

/* Get offset for a given FIFO */
#ifdef MUSB_FDRC_AHB
#define MGC_FIFO_OFFSET(_bEnd) (MGC_M_FIFO_EP0 + (_bEnd * 4))
#else
#define MGC_FIFO_OFFSET(_bEnd) (MGC_M_FIFO_EP0 + _bEnd)
#endif

__inline uint16_t MGC_ReadRxCount(MGC_Controller* pController)
{
    uint16_t count = 0;

    count = MGC_Read8((uint8_t*)pController->pControllerAddressIst, MGC_O_FDRC_RXCOUNT2);
    count <<= 8;
    count = MGC_Read8((uint8_t*)pController->pControllerAddressIst, MGC_O_FDRC_RXCOUNT1);
    return count;
}

#define MGC_VBUS_MASK            0x70    /* Power D6 - D4 */

#endif	/* multiple inclusion protection */
