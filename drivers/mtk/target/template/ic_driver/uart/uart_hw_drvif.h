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
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: uart_hw_drvif.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file uart_hw_drvif.h
 *  Brief of file uart_hw_drvif.h.
 *  Details of file uart_hw_drvif.h (optional).
 */

#ifndef UART_HW_DRVIF_H
#define UART_HW_DRVIF_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_serial.h"
#include "x_os.h"
#include "x_bim.h"
#include "uart_if.h"

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

// *********************************************************************
// UART fifo size
// *********************************************************************
#define UART0_FIFO_SIZE             ((UINT8)32 - 1)
#define UART1_FIFO_SIZE             ((UINT8)32 - 1)
#define UARTPD_FIFO_SIZE            ((UINT8)32 - 1)

// *********************************************************************
// UART register related
// *********************************************************************
#define UART_PD_BASE               (0x800)

#define REG_SER_PORT                0x00    // Data port for port 0
#define REG_SER01_PORT              0xc0    // Data port for port 1
#define REG_SERPD_PORT              0x800   // Data port for port 2 (PD)

#define REG_SER_CTL                 0x04
#define REG_SERPD_CTL               (UART_PD_BASE + 0x04)
#define TRANS_MODE_ON               (0x1 << 6)
#define SET_TRANS_MODE_ON           0xE2
#define REG_SER01_PORT_4BYTES       0xc4

// Communication control register for port 0, port 1 and port 2 (port PD)
#define REG_SER_CTR                 0x48
#define REG_SER01_CTR               0xc8
#define REG_SERPD_CTR               (UART_PD_BASE + 0x30)

#define BUAD_RATE_115200            (0x0<<8)
#define BUAD_RATE_115200x2          (0x1<<8)
#define BUAD_RATE_115200x4          (0x2<<8)
#define BUAD_RATE_115200x8          (0x3<<8)
#define BUAD_RATE_57600             (0x4<<8)
#define BUAD_RATE_38400             (0x5<<8)
#define BUAD_RATE_19200             (0x6<<8)
#define BUAD_RATE_9600              (0x7<<8)
#define BUAD_RATE_4800              (0x8<<8)
#define BUAD_RATE_2400              (0x9<<8)
#define BUAD_RATE_1200              (0xA<<8)
#define BUAD_RATE_300               (0xB<<8)
#define BUAD_RATE_110               (0xC<<8)
#define CHECK_PARITY_ON             (0x1<<4)
#define CHECK_PARITY_OFF            (0x0<<4)
#define PARITY_EVEN                 (0x1<<5)
#define PARITY_ODD                  (0x0<<5)
#define STOP_BIT_1                  (0x0<<2)
#define STOP_BIT_1_2                (0x1<<2)
#define STOP_BIT_2                  (0x1<<2)
#define BIT_LEN_5                   (0x3<<0)
#define BIT_LEN_6                   (0x2<<0)
#define BIT_LEN_7                   (0x1<<0)
#define BIT_LEN_8                   (0x0<<0)
#define UART_PARITY_ENABLE          CHECK_PARITY_ON
#define UART_PARITY_EVEN            PARITY_EVEN
#define CTR_NORMAL                  (BUAD_RATE_115200 | CHECK_PARITY_OFF | BIT_LEN_8)
#define CTR_HIGH_SPEED              (BUAD_RATE_115200x8 | CHECK_PARITY_OFF | BIT_LEN_8)

// Buffer status register for port 0, port 1 and port 2 (port PD)
#define REG_SER_STATUS              0x4c
#define REG_SER01_STATUS            0xcc
#define REG_SERPD_STATUS            (UART_PD_BASE + 0x34)
#define SER_WRITE_ALLOW             (UART0_FIFO_SIZE << 8)
#define SER_READ_ALLOW              (UART0_FIFO_SIZE)

#define REG_SER_BUF_CTL             0x50 // Buffer control register for port 0
#define REG_SER01_BUF_CTL           0xd0 // Buffer control register for port 1
#define REG_SERPD_BUF_CTL           ((UART_PD_BASE) + 0x38)
#define CLEAR_RX_BUFFER             (1<<6)
#define CLEAR_TX_BUFFER             (1<<7)

#define REG_SER_INT_EN              0x0c // Interrupt enable register
#define REG_SER_INT_STATUS          0x10 // Interrupt status register
#define INT_SER0_TX_EMPTY           (0x1<<19)
#define INT_SER0_RX_TOUT            (0x1<<18)
#define INT_SER0_RX_FULL            (0x1<<17)
#define INT_SER0_RX_ERR             (0x1<<16)
#define INT_SER0_RX_OVERFLOW        (0x1<<21)
#define INT_SER0_RX_RISC_DATA       (0x1<<22) // Normal mode
#define INT_SER1_TX_EMPTY           (0x1<<3)
#define INT_SER1_RX_TOUT            (0x1<<2)
#define INT_SER1_RX_FULL            (0x1<<1)
#define INT_SER1_RX_ERR             (0x1<<0)
#define INT_SER1_RX_OVERFLOW        (0x1<<5)
#define INT_SER1_MODEM              (0x1<<4)
#define INT_SER0_MASK               0x007F0000
#define INT_SER1_MASK               0x00000F3F

#define INT_SER1_DMAR_EMPTY         (0x1<<8)
#define INT_SER1_DMAW_FULL          (0x1<<9)
#define INT_SER1_DMAW_OVERFLOW      (0x1<<10)
#define INT_SER1_DMAW_TOUT          (0x1<<11)

#define REG_SERPD_INT_EN            (UART_PD_BASE + 0x0c)
#define REG_SERPD_INT_STATUS        (UART_PD_BASE + 0x10)
#define INT_SERPD_T8032_DATA        (0x1<<7)
#define INT_SERPD_RISC_DATA         (0x1<<6) // Normal mode
#define INT_SERPD_RX_OVERFLOW       (0x1<<5)
#define INT_SERPD_MODEM             (0x1<<4)
#define INT_SERPD_TX_EMPTY          (0x1<<3)
#define INT_SERPD_RX_TOUT           (0x1<<2)
#define INT_SERPD_RX_FULL           (0x1<<1)
#define INT_SERPD_RX_ERR            (0x1<<0)
#define INT_SERPD_MASK              0x0000007F

#define REG_SER1_DMAW_EN            0x140
#define DMAW_CIR                    (0x1<<1)
#define DMAW_EN                     (0x1<<0)
#define REG_SER1_DMAW_RST           0x144
#define REG_SER1_DMAW_LEVEL         0x148
#define REG_SER1_DMAW_SPTR          0x150
#define REG_SER1_DMAW_EPTR          0x154
#define REG_SER1_DMAW_WPTR          0x158
#define REG_SER1_DMAW_RPTR          0x15c

#define REG_SER1_DMAR_EN            0x160
#define DMAR_CIR                    (0x1<<1)
#define DMAR_EN                     (0x1<<0)
#define REG_SER1_DMAR_RST           0x164
#define REG_SER1_DMAR_LEVEL         0x168
#define REG_SER1_DMAR_SPTR          0x170
#define REG_SER1_DMAR_EPTR          0x174
#define REG_SER1_DMAR_WPTR          0x178
#define REG_SER1_DMAR_RPTR          0x17c

#define REG_SER_MERGE_MODE          0x14 // Merge mode control register
#define REG_NROMAL_MODE_BUF_SZ      0x1c

#define REG_SER_MSR                 0xd4
#define MODEM_DTR                   (1 << 0)
#define MODEM_RTS                   (1 << 1)
#define MODEM_HW_RTS                (1 << 2)
#define MODEM_HW_CTS                (1 << 3)
#define MODEM_RTS_TRIGGER_MASK      (0xf << 4)
#define MODEM_DSR                   (1 << 8)
#define MODEM_CTS                   (1 << 9)
#define MODEM_RI                    (1 << 10)
#define MODEM_DCD                   (1 << 11)
#define MODEM_DSR_CHG               (1 << 12)
#define MODEM_CTS_CHG               (1 << 13)
#define MODEM_RI_CHG                (1 << 14)
#define MODEM_DCD_CHG               (1 << 15)

#define MODEM_RTS_TRIGGER_VALUE     (0xf << 4)

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#define UART_READ8(offset)              ((offset < UART_PD_BASE) ? \
                                         IO_READ8(RS232_BASE, (offset)) : \
                                         IO_READ8(PDWNC_BASE, (offset)))
#define UART_READ16(offset)             ((offset < UART_PD_BASE) ? \
                                         IO_READ16(RS232_BASE, (offset)) : \
                                         IO_READ16(PDWNC_BASE, (offset)))
#define UART_READ32(offset)             ((offset < UART_PD_BASE) ? \
                                         IO_READ32(RS232_BASE, (offset)) : \
                                         IO_READ32(PDWNC_BASE, (offset)))

#define UART_ReadPort32(port, offset) \
    IO_READ32(RS232_BASE, ((port*0x80) + (offset)))

#define UART_WritePort32(port, offset, value) \
    IO_WRITE32(RS232_BASE, ((port*0x80) + (offset)), (value))

#define UART_WRITE8(offset, value)      ((offset < UART_PD_BASE) ? \
                                         IO_WRITE8(RS232_BASE, (offset), (value)) : \
                                         IO_WRITE8(PDWNC_BASE, (offset), (value)))
#define UART_WRITE16(offset, value)     ((offset < UART_PD_BASE) ? \
                                         IO_WRITE16(RS232_BASE, (offset), (value)) : \
                                         IO_WRITE16(PDWNC_BASE, (offset), (value)))
#define UART_WRITE32(offset, value)     ((offset < UART_PD_BASE) ? \
                                         IO_WRITE32(RS232_BASE, (offset), (value)) : \
                                         IO_WRITE32(PDWNC_BASE, (offset), (value)))

#define UART_SET_BIT(Reg, Bit)          UART_WRITE32(Reg, UART_READ32(Reg) | (Bit))
#define UART_CLR_BIT(Reg, Bit)          UART_WRITE32(Reg, UART_READ32(Reg) & (~(Bit)))

#define UART_WRITE_DATA(u1Port, u1Data) ((u1Port < UART_PORT_2) ? \
                                         UART_WRITE8(REG_SER_PORT + ((u1Port)*(0xc0)), u1Data) : \
                                         UART_WRITE8(REG_SERPD_PORT, u1Data))
#define UART_READ_DATA(u1Port)          ((u1Port < UART_PORT_2) ? \
                                         UART_READ8(REG_SER_PORT + ((u1Port)*(0xc0))) : \
                                         UART_READ8(REG_SERPD_PORT))

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

extern void UART_HwWaitTxBufClear(UINT32 u4Port);

extern void UART_HwEnableTxIntr(UINT32 u4Port);

extern void UART_HwDiableTxIntr(UINT32 u4Port);

extern void UART_HwInit(void);

extern void UART_HwSetParity(UINT32 u4Port, UINT32 u4Parity);

extern void UART_HwSetCustomBaudRate(UINT32 u4Port, UINT32 u4BaudRate);

extern void UART_HwSetBaudRate(UINT32 u4Port, UINT32 u4BaudRate);

extern void UART_HwSetStopBit(UINT32 u4Port, UINT32 u4StopBit);

extern void UART_HwSetDataLen(UINT32 u4Port, UINT32 u4DataLen);

extern void UART_HwGetBaudRate(UINT32 u4Port, RS_232_SETUP_INFO_T * prSetupInfo);

extern void UART_HwGetParity(UINT32 u4Port, RS_232_SETUP_INFO_T * prSetupInfo);

extern void UART_HwGetStopBit(UINT32 u4Port, RS_232_SETUP_INFO_T * prSetupInfo);

extern void UART_HwGetDataLen(UINT32 u4Port, RS_232_SETUP_INFO_T * prSetupInfo);

extern UINT32 UART_HwGetRxDataCnt(UINT32 u4Port);

extern UINT32 UART_HwGetTxEmptyCnt(UINT32 u4Port);

extern BOOL UART_HwWriteAllow(UINT32 u4Port);

extern BOOL UART_HwReadAllow(UINT32 u4Port);

extern void UART_HwClearFifo(UINT32 u4Port);

extern UINT32 UART_HwGetIntStatus(UINT32 u4Port);

extern void UART_HwClearIntStatus(UINT32 u4Port);

extern void UART_HwWrite(UINT32 u4Port, const UINT8 * pu1Data, UINT32 u4NumberToWrite);

extern void UART_HwModemCtr(BOOL fgEnable);

extern UINT32 UART_GetRxDMABuf(UINT32 u4Port, UINT8 *pBuffer, UINT32 u4NumToRead);

extern void UART_HwDmaWriteDram(UINT32 u4SPTR, UINT32 u4Size, UINT32 u4Threshold);

extern void UART_HwDmaReadDisable(void);

extern void UART_HwDmaReadDram(UINT32 u4SPTR, UINT32 u4Size, BOOL fgCircular);

extern BOOL UART_HwTransMode(void);

extern BOOL UART_HwCheckPortTransMode(UINT32 u4Port);

#endif // UART_HW_DRVIF_H
