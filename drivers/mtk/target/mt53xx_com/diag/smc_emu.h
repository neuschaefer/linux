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
#ifndef __SMC_EMU_H
#define __SMC_EMU_H


#include "x_stl_lib.h"
#include "x_hal_5381.h" //#include "x_hal_5371.h"
#include "smc_drvif.h"  //#include "../5371/smc/smc_drv.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* Macros for register read/write */
/*
#define SMC_READ8(offset)         IO_READ8(SMART_CARD_BASE, (offset))
#define SMC_READ16(offset)        IO_READ16(SMART_CARD_BASE, (offset))
#define SMC_WRITE8(offset,value)  IO_WRITE8(SMART_CARD_BASE, (offset),(value))
#define SMC_WRITE16(offset,value) IO_WRITE16(SMART_CARD_BASE, (offset),(value))
*/

#define BitValue(data, pos)                 ((data & pos) ? 1 : 0)


/* SmartCard registers and bitmaps */
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
#define REG_SMC_BRR             0x8         // SMC Baud Rate Register
#define REG_SMC_IRQEN           0x10        // SMC Interrupt enable register
    #define IRQEN_MASK          0x07FF      // Mask for all interrupts
#define REG_SMC_STA             0x14        // SMC module status register
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
#define REG_SMC_RETRY           0x20        // SMC retry limit register
#define REG_SMC_TIDE            0x24        // SMC retry limit register
#define REG_SMC_DATA            0x30        // Tx/Rx data register
#define REG_SMC_COUNT           0x34        // SMC FIFO count register
#define REG_SMC_ATIME           0x40        // SMC activation time register
#define REG_SMC_DTIME           0x44        // SMC deactivation time register
#define REG_SMC_WTIME           0x48        // Char to Char waiting time reg
#define REG_SMC_GTIME           0x4C        // block to block guard time reg
#define REG_SMC_ETIME           0x50        // Transimit error dection reg
#define REG_SMC_INS             0x60        // SMC command header reg : INS
    #define INSD                (1 << 8)
#define REG_SMC_P3              0x64        // SMC command header reg : P3
#define REG_SMC_SW1             0x68        // SMC procedure byte reg : SW1
#define REG_SMC_SW2             0x6C        // SMC procedure byte reg : SW2


/* Return values */
#define SMCR_OK                ((INT32)  0)
#define SMCR_INV_ARG           ((INT32) -1)


typedef struct SMC_CON_T
{
    UINT16          u2SMCON             :   1;
    UINT16          u2CSTOP             :   1;
    UINT16          u2WRST              :   1;
    UINT16                              :  13;
} SMC_CON;


typedef struct SMC_CNF_T
{
    UINT16          u2RXACK             :   1;
    UINT16          u2TXACK             :   1;
    UINT16          u2CPOL              :   1;
    UINT16          u2SINV              :   1;
    UINT16          u2SDIR              :   1;
    UINT16          u2ODD               :   1;
    UINT16          u2SMCSEL            :   1;
    UINT16          u2TOUT              :   1;
    UINT16          u2T1EN              :   1;
    UINT16          u2T0EN              :   1;
    UINT16          u2HFEN              :   1;
    UINT16                              :   5;
} SMC_CNF;


typedef struct SMC_BRR_T
{
    UINT16          u2SMCCLK            :   2;
    UINT16          u2ETU               :   9;
    UINT16                              :   5;
} SMC_BRR;


typedef struct IRQEN_T
{
    UINT16          u2TXTIDE            :   1;
    UINT16          u2RXTIDE            :   1;
    UINT16          u2OVRUN             :   1;
    UINT16          u2TOUT              :   1;
    UINT16          u2TXERR             :   1;
    UINT16          u2ATRERR            :   1;
    UINT16          u2SMCOFF            :   1;
    UINT16          u2T0END             :   1;
    UINT16          u2RXERR             :   1;
    UINT16          u2T1END             :   1;
    UINT16          u2EDCERR            :   1;
    UINT16                              :   5;
} SMC_IRQEN;


typedef struct SMC_STA_T
{
    UINT16          u2TXTIDE            :   1;
    UINT16          u2RXTIDE            :   1;
    UINT16          u2OVRUN             :   1;
    UINT16          u2TOUT              :   1;
    UINT16          u2TXERR             :   1;
    UINT16          u2ATRERR            :   1;
    UINT16          u2SMCOFF            :   1;
    UINT16          u2T0END             :   1;
    UINT16          u2RXERR             :   1;
    UINT16          u2T1END             :   1;
    UINT16          u2EDCERR            :   1;
    UINT16                              :   5;
} SMC_STA;


typedef struct SMC_RETRY_T
{
    UINT16          u2RXRETRY           :   3;
    UINT16                              :   5;
    UINT16          u2TXRETRY           :   3;
    UINT16                              :   5;
} SMC_RETRY;


typedef struct SMC_TIDE_T
{
    UINT16          u2RXTIDE            :   4;
    UINT16                              :   4;
    UINT16          u2TXTIDE            :   4;
    UINT16                              :   4;
} SMC_TIDE;


typedef struct SMC_DATA_T
{
    UINT16          u2DATA              :   8;
    UINT16                              :   8;
} SMC_DATA;


typedef struct SMC_COUNT_T
{
    UINT16          u2COUNT             :   5;
    UINT16                              :  11;
} SMC_COUNT;


typedef struct SMC_ATIME_T
{
    UINT16          u2ATIME             :  16;
} SMC_ATIME;


typedef struct SMC_DTIME_T
{
    UINT16          u2DTIME             :  12;
    UINT16                              :   4;
} SMC_DTIME;


typedef struct SMC_WTIME_T
{
    UINT16          u2WTIME             :  16;
} SMC_WTIME;


typedef struct SMC_GTIME_T
{
    UINT16          u2GTIME             :   4;
    UINT16                              :  12;
} SMC_GTIME;


typedef struct SMC_ETIME_T
{
    UINT16          u2ETIME             :   6;
    UINT16                              :  10;
} SMC_ETIME;


typedef struct SMC_INS_T
{
    UINT16          u2SMCINS            :   8;
    UINT16          u2INSD              :   1;
    UINT16                              :   7;
} SMC_INS;


typedef struct SMC_P3_T
{
    UINT16          u2SMCP3             :   9;
    UINT16                              :   7;
} SMC_P3;


typedef struct SMC_SW1_T
{
    UINT16          u2SMCSW1            :   8;
    UINT16                              :   8;
} SMC_SW1;


typedef struct SMC_SW2_T
{
    UINT16          u2SMCSW2            :   8;
    UINT16                              :   8;
} SMC_SW2;


/*-----------------------------------------------------------------------------
                    Exported functions
 ----------------------------------------------------------------------------*/
void SmcRegSetDfl(void);

// Test 1
BOOL SmcRegDflTest(void);
BOOL SmcRegRwTest(void);

// Test 2
BOOL SmcIntTest(void);

// Test 3
BOOL SmcDeactTest(void);
BOOL SmcActTest(void);
BOOL SmcToutTest(void);
BOOL SmcDirTest(void);
BOOL SmcPpsTest(void);
BOOL SmcStopClkTest(void);

// Test 4
BOOL SmcTimerTest(void);

// Test 5
BOOL SmcT0Test(void);

// Test 6
BOOL SmcT1Test(void);


#endif //__SMC_EMU_H

