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
/*-----------------------------------------------------------------------------
 *-----------------------------------------------------------------------------
 * $Date: 2012/04/27 $
 * $RCSfile: smc_hw.h,v $
 * $Revision: #1 $
 *
 * Description:
 *         This header file contains hardware definitions of SmartCard.
 *---------------------------------------------------------------------------*/

#ifndef _SMC_HW_H
#define _SMC_HW_H


#include "x_typedef.h"
#include "x_bim.h"


/******************************************************************************
 *  SmartCard registers and bitmaps
 *****************************************************************************/
#ifdef CC_MT5391
  #define VECTOR_SMARTCARD 11  
#endif
#define REG_SMC_CON             0x0         // SMC module control register
	#define SMCON			    (1 << 0)	// SMC card power up/down control
	#define CSTOP   			(1 << 1)	// Enable clock stop module
    #define WRST   			    (1 << 2)	// SMC card warm reset control
#define REG_SMC_CNF             0x4         // SMC module configuration
    #define CNF_RXACK           (1 << 0)    // Reception error handshake
    #define CNF_TXACK           (1 << 1)    // Transmission error handshake
    #define CNF_CPOL            (1 << 2)    // SMCCLK polarity control
    #define CNF_SINV            (1 << 3)    // Data inverter
    #define CNF_SDIR            (1 << 4)    // Data Transfer Direction
    #define CNF_ODD             (1 << 5)    // Select odd or even parity
    #define CNF_SMCSEL          (1 << 6)    // Supply voltage select
    #define CNF_TOUT            (1 << 7)    // Waiting time counter control
    #define CNF_T1EN            (1 << 8)    // T=1 protocol controller
    #define CNF_T0EN            (1 << 9)    // T=0 protocol controller
    #define CNF_HFEN            (1 << 10)   // Hardware flow control
    #define CNF_DIRECT          0x0000
    #define CNF_INDIRECT        0x0038
#define REG_SMC_BRR             0x8         // SMC Baud Rate Register
    #define BRR_SMCCLK_MASK		0x0003
    #define BRR_ETU_MASK        0x07FC
    #define BRR_SMCCLK_DIV2	    0x0000
    #define BRR_SMCCLK_DIV4	    0x0001
    #define BRR_SMCCLK_DIV8	    0x0002
    #define BRR_SMCCLK_DIV12	0x0003
#define REG_SMC_IRQEN           0x10        // SMC Interrupt enable register
    #define IRQ_TXTIDE          (1 << 0)    // Transmit FIFO tide
    #define IRQ_RXTIDE          (1 << 1)    // Receive FIFO tide
    #define IRQ_OVRUN           (1 << 2)    // T/R FIFO overrun
    #define IRQ_TOUT            (1 << 3)    // Between character timeout
    #define IRQ_TXERR           (1 << 4)    // Character transmission error
    #define IRQ_ATRERR          (1 << 5)    // ATR start time-out
    #define IRQ_SMCOFF          (1 << 6)    // Deactivation complete
    #define IRQ_T0END           (1 << 7)    // T=0 data transfer completed
    #define IRQ_RXERR           (1 << 8)    // Character reception error
    #define IRQ_T1END           (1 << 9)    // T=1 data transfer completed
    #define IRQ_EDCERR          (1 << 10)   // T=1 CRC error
    #define IRQEN_MASK          0x07FF      // Mask for all interrupts
#define REG_SMC_STA             0x14        // SMC module status register
    #define STA_TXTIDE          (1 << 0)    // Transmit FIFO tide
    #define STA_RXTIDE          (1 << 1)    // Receive FIFO tide
    #define STA_OVRUN           (1 << 2)    // T/R FIFO overrun
    #define STA_TOUT            (1 << 3)    // Between character timeout
    #define STA_TXERR           (1 << 4)    // Character transmission error
    #define STA_ATRERR          (1 << 5)    // ATR start time-out
    #define STA_SMCOFF          (1 << 6)    // Deactivation complete
    #define STA_T0END           (1 << 7)    // T=0 data transfer completed
    #define STA_RXERR           (1 << 8)    // Character reception error
    #define STA_T1END           (1 << 9)    // T=1 data transfer completed
    #define STA_EDCERR          (1 << 10)   // T=1 CRC error
#define REG_SMC_RETRY           0x20        // SMC retry limit register
    #define RETRY_RXRETRY_MASK	0x0007
    #define RETRY_TXRETRY_MASK	0x0700
#define REG_SMC_TIDE            0x24        // SMC retry limit register
    #define TIDE_RXMASK		    0x000f
    #define TIDE_TXMASK		    0x0f00
#define REG_SMC_DATA            0x30        // Tx/Rx data register
#define REG_SMC_COUNT           0x34        // SMC FIFO count register
    #define COUNT_MASK		    0x001f
#define REG_SMC_ATIME           0x40        // SMC activation time register
#define REG_SMC_DTIME           0x44        // SMC deactivation time register
#define REG_SMC_WTIME           0x48        // Char to Char waiting time reg
#define REG_SMC_GTIME           0x4C        // block to block guard time reg
#define REG_SMC_ETIME           0x50        // Transimit error dection reg
#define REG_SMC_INS             0x60        // SMC command header reg : INS
    #define SMCINS_MASK  		0x00ff
    #define INSD                (1 << 8)
#define REG_SMC_P3              0x64        // SMC command header reg : P3
    #define P3_MASK             0x01ff
#define REG_SMC_SW1             0x68        // SMC procedure byte reg : SW1
    #define SW1_MASK            0xFF
#define REG_SMC_SW2             0x6C        // SMC procedure byte reg : SW2
    #define SW2_MASK            0xFF


/******************************************************************************
 *  Definitions
 *****************************************************************************/
#define     HIGH                TRUE
#define     LOW                 FALSE
#define 	ACK_NULL_CHAR       0x60

#define IRQEN_Normal            (IRQ_RXERR|IRQ_ATRERR|IRQ_TXERR|IRQ_TOUT|IRQ_OVRUN|IRQ_RXTIDE)
#define IRQEN_CMD_T0_Normal     (IRQ_T0END|IRQ_RXTIDE|IRQ_OVRUN|IRQ_TXERR|IRQ_RXERR|IRQ_EDCERR)
#define IRQEN_CMD_T1_Normal     (IRQ_T1END|IRQ_RXTIDE|IRQ_OVRUN|IRQ_TXERR|IRQ_RXERR|IRQ_EDCERR)


/******************************************************************************
 *  Macros
 *****************************************************************************/
#if 0 //release
#define SMC_READ8(addr)		    IO_READ8(SMART_CARD_BASE, (addr))
#define SMC_READ16(addr)		IO_READ16(SMART_CARD_BASE, (addr))
#define SMC_WRITE8(addr, val)   IO_WRITE8(SMART_CARD_BASE, (addr), (val))
#define SMC_WRITE16(addr, val)  IO_WRITE16(SMART_CARD_BASE, (addr), (val))
#else  //debug
UINT8 SMC_READ8(UINT32 addr);
UINT16 SMC_READ16(UINT32 addr);
void SMC_WRITE8(UINT32 addr, UINT8 val);
void SMC_WRITE16(UINT32 addr, UINT16 val);  
#endif
#define SmcSetP3(_P3)           SMC_WRITE16(REG_SMC_P3, _P3)
#define SmcSetIns(_INS)         SMC_WRITE16(REG_SMC_INS, _INS)
#define SmcSetInsTx(INS)        SMC_SetINS(INS | INSD)
#define SmcSetInsRx(INS)        SMC_SetINS(INS & (~INSD))
#define SmcFlushFifo()          SMC_WRITE16(REG_SMC_COUNT, 0)

#define SmcActive()             SMC_WRITE16(REG_SMC_CON, 0x0001)
#define SmcDeactive()	        SMC_WRITE16(REG_SMC_CON, 0x0000)

#define SmcEnableT0Ctrl()       SMC_WRITE16(REG_SMC_CNF, SMC_READ16(REG_SMC_CNF) | CNF_T0EN)
#define SmcDisableT0Ctrl()      SMC_WRITE16(REG_SMC_CNF, SMC_READ16(REG_SMC_CNF) | CNF_T0EN)
#define SmcEnableFlowCtrl()     SMC_WRITE16(REG_SMC_CNF, SMC_READ16(REG_SMC_CNF) & ~CNF_HFEN)
#define SmcDisableFlowCtrl()    SMC_WRITE16(REG_SMC_CNF, SMC_READ16(REG_SMC_CNF) & ~CNF_HFEN)

#define SmcEnableInt(_Intr)     SMC_WRITE16(REG_SMC_IRQEN, SMC_READ16(REG_SMC_IRQEN) | _Intr)
#define SmcEnableAllInt()       SmcEnableInt(IRQEN_MASK)

#define SmcDisableInt(_Intr)    SMC_WRITE16(REG_SMC_IRQEN, SMC_READ16(REG_SMC_IRQEN) & ~_Intr)
#define SmcDisableAllInt()      SmcDisableInt(IRQEN_MASK)

#define SmcGetFifoCnt()         (SMC_READ16(REG_SMC_COUNT) & COUNT_MASK)


/******************************************************************************
 *  Exported Functions
 *****************************************************************************/
void SmcSetRxTide(UINT16 u2RxTide);
void SmcSetTxTide(UINT16 u2TxTide);
void SmcSetRxRetry(UINT16 u2RXRetry);
void SmcSetTxRetry(UINT16 u2TXRetry);
void SmcIdle(BOOL bLevel);
void SmcActiveClk(void);
void SmcReset(void);
void SmcWarmRst(void);
void SmcSetTout(UINT16 u2Tout);
UINT16 SmcReadSw(void);
void SmcSetEtu(UINT16 _u2Etu);


#endif	/*_SMC_HW_H*/


