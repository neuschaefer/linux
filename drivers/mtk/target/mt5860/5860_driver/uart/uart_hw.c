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
 * $RCSfile: uart_hw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file uart_hw.c
 *  Brief of file uart_hw.c.
 *  Details of file uart_hw.c (optional).
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_serial.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_assert.h"
#include "x_pinmux.h"
#include "uart_debug.h"
#include "uart_hw_drvif.h"
#include "uart_drv.h"
#include "drvcust_if.h"

LINT_EXT_HEADER_END

#ifdef CC_UART_PD_SUPPORT
#define GET_COMMUNICATE_CTRL_REG(u4Port, u4RegAddr) \
    switch (u4Port)                                 \
    {                                               \
    case UART_PORT_0:                               \
        u4RegAddr = REG_SER_CTR;                    \
        break;                                      \
    case UART_PORT_1:                               \
        u4RegAddr = REG_SER01_CTR;                  \
        break;                                      \
    case UART_PORT_PD:                              \
        u4RegAddr = REG_SERPD_CTR;                  \
        break;                                      \
    default:                                        \
        return;                                     \
    }
#else
#define GET_COMMUNICATE_CTRL_REG(u4Port, u4RegAddr) \
    switch (u4Port)                                 \
    {                                               \
    case UART_PORT_0:                               \
        u4RegAddr = REG_SER_CTR;                    \
        break;                                      \
    case UART_PORT_1:                               \
        u4RegAddr = REG_SER01_CTR;                  \
        break;                                      \
    default:                                        \
        return;                                     \
    }
#endif /* CC_UART_PD_SUPPORT */

// *********************************************************************
// Function : void UART_HwWaitTxBufClear(void)
// Description : wait until UR1 TxBuf Clear
// Parameter : None
// Return    : None
// *********************************************************************

void UART_WRITE_DATA(UINT32 u4Port, UINT8 u1Data)
{
    switch (u4Port)
    {
    case UART_PORT_0:
        UART_WRITE8(REG_SER_PORT, u1Data);
        break;
        
    case UART_PORT_1:
        UART_WRITE8(REG_SER01_PORT, u1Data);
        break;
        
#ifdef CC_UART_PD_SUPPORT
    case UART_PORT_PD:
        UART_WRITE8(REG_SERPD_PORT, u1Data);
        break;
#endif /* CC_UART_PD_SUPPORT */

    default:
        return;
    }
}

UINT8 UART_READ_DATA(UINT32 u4Port)
{
    switch (u4Port)
    {
    case UART_PORT_0:
        return (UART_READ8(REG_SER_PORT));
        
    case UART_PORT_1:
        return (UART_READ8(REG_SER01_PORT));
        
#ifdef CC_UART_PD_SUPPORT
    case UART_PORT_PD:
        return (UART_READ8(REG_SERPD_PORT));
#endif /* CC_UART_PD_SUPPORT */

    default:
        return 0;
    }
}

void UART_HwWaitTxBufClear(UINT32 u4Port)
{
    UINT32 u4RegAddr;

    switch (u4Port)
    {
    case UART_PORT_0:
        u4RegAddr = REG_SER_STATUS;
        break;

    case UART_PORT_1:
        u4RegAddr = REG_SER01_STATUS;
        break;

#ifdef CC_UART_PD_SUPPORT
    case UART_PORT_PD:
        u4RegAddr = REG_SERPD_STATUS;
        break;
#endif /* CC_UART_PD_SUPPORT */

    default:
        return;
    }
    
    while ((UART_READ32(u4RegAddr) & SER_WRITE_ALLOW) != SER_WRITE_ALLOW);
}

void UART_HwEnableTxIntr(UINT32 u4Port)
{
    switch (u4Port)
    {
    case UART_PORT_0:
        UART_SET_BIT(REG_SER_INT_EN, INT_SER0_TX_EMPTY);
        break;

    case UART_PORT_1:
        UART_SET_BIT(REG_SER_INT_EN, INT_SER1_TX_EMPTY);
        break;

#ifdef CC_UART_PD_SUPPORT
    case UART_PORT_PD:
        UART_SET_BIT(REG_SERPD_INT_EN, INT_SERPD_TX_EMPTY);
        break;
#endif /* CC_UART_PD_SUPPORT */
        
    default:
        break;
    }
}

void UART_HwDiableTxIntr(UINT32 u4Port)
{
    switch (u4Port)
    {
    case UART_PORT_0:
        UART_CLR_BIT(REG_SER_INT_EN, INT_SER0_TX_EMPTY);
        break;

    case UART_PORT_1:
        UART_CLR_BIT(REG_SER_INT_EN, (INT_SER1_TX_EMPTY | INT_SER1_DMAR_EMPTY));
        break;

#ifdef CC_UART_PD_SUPPORT
    case UART_PORT_PD:
        UART_CLR_BIT(REG_SERPD_INT_EN, INT_SERPD_TX_EMPTY);
        break;
#endif /* CC_UART_PD_SUPPORT */
        
    default:
        break;
    }
}

void UART_HwModemCtr(BOOL fgEnable)
{
    if (fgEnable)
    {
        PINMUX0_WRITE((PINMUX0_REG() & ~(0xf << 18)) | (5 << 18));

        PINMUX1_WRITE((PINMUX1_REG() & ~(0x3 << 18)) | (2 << 18));

        UART_WRITE32(REG_SER_MSR,
                     MODEM_DSR_CHG | MODEM_CTS_CHG | MODEM_RI_CHG | MODEM_DCD_CHG | MODEM_HW_RTS
                         | MODEM_HW_CTS | MODEM_RTS_TRIGGER_VALUE);
        UART_SET_BIT(REG_SER_INT_EN, INT_SER1_MODEM);
    }
    else
    {
        UART_WRITE32(REG_SER_MSR, 0);
        UART_CLR_BIT(REG_SER_INT_EN, INT_SER1_MODEM);
    }
}

void UART_HwInit(void)
{
//    UINT32 u4ExtraUartSupport;
    
    UART_HwClearFifo(UART_PORT_0, CLEAR_TX_BUFFER | CLEAR_RX_BUFFER);
    UART_HwClearFifo(UART_PORT_1, CLEAR_TX_BUFFER | CLEAR_RX_BUFFER);
#ifdef CC_UART_PD_SUPPORT    
    UART_HwClearFifo(UART_PORT_PD, CLEAR_TX_BUFFER | CLEAR_RX_BUFFER);
#endif

    // Set uart port to be baudrate = 115200 & discard all zero RX data
    UART_WRITE32(REG_SER_CTR, CTR_NORMAL | DIS_RX_ZERO_ON);
    UART_WRITE32(REG_SER01_CTR, CTR_NORMAL | DIS_RX_ZERO_ON);
#ifdef CC_UART_PD_SUPPORT    
    UART_WRITE32(REG_SERPD_CTR, CTR_NORMAL | DIS_RX_ZERO_ON);
#endif

    // timeout cycle : 256*cnt/27 us => 9.48*cnt us
    UART_WRITE32(REG_SER_BUF_CTL, 0x1F00 | (UART0_FIFO_SIZE - 5));
    UART_WRITE32(REG_SER01_BUF_CTL, 0x1F00 | (UART1_FIFO_SIZE - 5));
#ifdef CC_UART_PD_SUPPORT    
    UART_WRITE32(REG_SERPD_BUF_CTL, 0x1F00 | (UARTPD_FIFO_SIZE - 5));
#endif

    UART_HwWaitTxBufClear(UART_PORT_0);
    UART_HwWaitTxBufClear(UART_PORT_1);
#ifdef CC_UART_PD_SUPPORT    
    UART_HwWaitTxBufClear(UART_PORT_PD);
#endif

    // Clear interrupt status
    UART_WRITE32(REG_SER_INT_STATUS, 0);
#ifdef CC_UART_PD_SUPPORT    
    UART_WRITE32(REG_SERPD_INT_STATUS, 0);
#endif

    #if defined(CC_UART_DEBUG_FC)
    // Enable interrupt mask
    UART_WRITE32(REG_SER_INT_EN,
                 INT_SER0_RX_OVERFLOW | INT_SER0_RX_TOUT | INT_SER0_RX_FULL | 
                 INT_SER0_RX_RISC_DATA | INT_SER1_DMAW_TOUT |
                 INT_SER1_DMAW_OVERFLOW | INT_SER1_DMAW_FULL |
                 INT_SER1_RX_OVERFLOW | INT_SER1_MODEM);
    #else
    // Enable interrupt mask
    UART_WRITE32(REG_SER_INT_EN,
                 INT_SER0_RX_OVERFLOW | INT_SER0_RX_TOUT | INT_SER0_RX_FULL | 
                 INT_SER0_RX_RISC_DATA | INT_SER1_DMAW_TOUT |
                 INT_SER1_DMAW_OVERFLOW | INT_SER1_DMAW_FULL);
    #endif
#ifdef CC_UART_PD_SUPPORT    
    UART_WRITE32(REG_SERPD_INT_EN,
                 INT_SERPD_RX_OVERFLOW | INT_SERPD_RX_TOUT | INT_SERPD_RX_FULL | 
                 INT_SERPD_RISC_DATA);
#endif

    // Disable Modem control in default
    UART_HwModemCtr(FALSE);

#ifdef CC_SET_UART_TRANSPARENT
    // Set to transparent mode
    UART_WRITE32(REG_SER_CTL, SET_TRANS_MODE_ON);
#endif
#ifdef CC_UART_PD_SUPPORT    
    UART_WRITE32(REG_PD_NROMAL_MODE_BUF_SZ, 0);
    UART_WRITE32(REG_SERPD_CTL, SET_TRANS_MODE_ON);
#endif
#if 0
    // Open uart1 Tx/Rx pinmux
    VERIFY(DRVCUST_OptQuery(eUART1Support, &u4ExtraUartSupport) == 0);
    if (u4ExtraUartSupport)
    {
        VERIFY(BSP_PinSet(U2RX_I, 1) == OSR_OK);
    }
#endif    
}

UINT32 UART_GetRxDMABuf(UINT32 u4Port, UINT8 *pBuffer, UINT32 u4NumToRead)
{
    UINT32 u4WPTR;
    UINT32 u4RPTR;    
    UINT32 u4EPTR;    
    UINT32 u4SPTR;    
    UINT32 u4RxLength = 0;    
    UINT32 u4Length;    
    
    // only uart 1 support DMA buffer.
    ASSERT(u4Port== UART_PORT_1);
    ASSERT(pBuffer);

    u4Length = UART_READ32(REG_SER1_DMAW_LEVEL);
    if (u4Length == 0)
    {
        return 0;
    }

    u4WPTR = UART_READ32(REG_SER1_DMAW_WPTR);
    u4RPTR = UART_READ32(REG_SER1_DMAW_RPTR);    
    u4SPTR = UART_READ32(REG_SER1_DMAW_SPTR);            
    u4EPTR = UART_READ32(REG_SER1_DMAW_EPTR);            

    if (u4WPTR > u4RPTR)
    {
        u4Length = MIN((u4WPTR - u4RPTR), u4NumToRead);
        x_memcpy((VOID *)pBuffer, (const VOID *)u4RPTR, u4Length);     
        UART_WRITE32(REG_SER1_DMAW_RPTR, (u4RPTR + u4Length));        
        return u4Length;
    }
    else
    {
        u4Length = MIN((u4EPTR - u4RPTR), u4NumToRead);
        x_memcpy((VOID *)pBuffer, (const VOID *)u4RPTR, u4Length);
        UART_WRITE32(REG_SER1_DMAW_RPTR, (u4RPTR + u4Length));
        if ((u4RPTR + u4Length) == u4EPTR)
        {
            UART_WRITE32(REG_SER1_DMAW_RPTR, u4SPTR);
        }
        
        if (u4Length == u4NumToRead)
        {
            return u4Length;
        }
        u4RxLength += u4Length;
        pBuffer += u4Length;
        u4NumToRead -= u4Length;

        u4Length = MIN((u4WPTR - u4SPTR), u4NumToRead);
        u4RxLength += u4Length;
        x_memcpy((VOID *)pBuffer, (const VOID *)u4SPTR, u4Length);
        UART_WRITE32(REG_SER1_DMAW_RPTR, (u4SPTR + u4Length));
        return u4RxLength;        
    }
}

VOID UART_HwDmaWriteDram(UINT32 u4SPTR, UINT32 u4Size, UINT32 u4Threshold)
{
    UINT32 u4Reg = 0;

    u4Threshold &= 0xFFFFFF;
    ASSERT(u4Threshold <= u4Size);
    
    UART_WRITE32(REG_SER1_DMAW_EN, 0);         

    u4Reg = UART_READ32(REG_SER_INT_EN);
    u4Reg |= INT_SER1_DMAW_TOUT | INT_SER1_DMAW_FULL | INT_SER1_DMAW_OVERFLOW;
    UART_WRITE32(REG_SER_INT_EN, u4Reg);

    UART_WRITE32(REG_SER1_DMAW_SPTR, u4SPTR);
    UART_WRITE32(REG_SER1_DMAW_EPTR, (u4SPTR + u4Size));

    UART_WRITE32(REG_SER1_DMAW_RST, 1);
    UART_WRITE32(REG_SER1_DMAW_EN, (u4Threshold << 8) | 3);
}

VOID UART_HwDmaReadDisable(void)
{
    UART_WRITE32(REG_SER1_DMAR_EN, 0);         
}

VOID UART_HwDmaReadDram(UINT32 u4SPTR, UINT32 u4Size, BOOL fgCircular)
{
    UINT32 u4Reg = 0;

    UART_WRITE32(REG_SER1_DMAR_EN, 0);         

    u4Reg = UART_READ32(REG_SER_INT_EN);
    u4Reg |= INT_SER1_DMAR_EMPTY;
    UART_WRITE32(REG_SER_INT_EN, u4Reg);

    UART_WRITE32(REG_SER1_DMAR_SPTR, u4SPTR);

    u4Reg = 0;
    if (!fgCircular)
    {
        UART_WRITE32(REG_SER1_DMAR_EPTR, (u4SPTR + u4Size + 1));
    }
    else
    {
        UART_WRITE32(REG_SER1_DMAR_EPTR, (u4SPTR + u4Size));
        u4Reg |= DMAR_CIR;
    }
    
    UART_WRITE32(REG_SER1_DMAR_RST, 1);

    u4Reg |= DMAR_EN;
    UART_WRITE32(REG_SER1_DMAR_EN, u4Reg);         

    if (!fgCircular)
    {
        // just do the DRAM read from u4SPTR to EPTR.
        UART_WRITE32(REG_SER1_DMAR_WPTR, (u4SPTR+u4Size));
    }
}

void UART_HwSetParity(UINT32 u4Port, UINT32 u4Parity)
{
    UINT32 u4Ctr;
    UINT32 u4RegAddr;
    
    GET_COMMUNICATE_CTRL_REG(u4Port, u4RegAddr)

    u4Ctr  = UART_READ32(u4RegAddr);
    u4Ctr &= (UINT32)0xFFCF;

    switch (u4Parity)
    {
    case RS_232_PARITY_NONE:
        u4Ctr |= CHECK_PARITY_OFF;
        break;

    case RS_232_PARITY_EVEN:
        u4Ctr |= CHECK_PARITY_ON | PARITY_EVEN;
        break;

    case RS_232_PARITY_ODD:
        u4Ctr |= CHECK_PARITY_ON | PARITY_ODD;
        break;

    default:
        u4Ctr |= CHECK_PARITY_OFF;
        LOG(1, "Parity %d not support\n", u4Parity);
        break;
    }

    UART_WRITE32(u4RegAddr, u4Ctr);
}

void UART_HwSetCustomBaudRate(UINT32 u4Port, UINT32 u4BaudRate)
{
    UINT32 u4Ctr;
    UINT32 u4Reg;
    UINT32 u4RegAddr;

    GET_COMMUNICATE_CTRL_REG(u4Port, u4RegAddr)
    
    u4Ctr = UART_READ32(u4RegAddr);
    
    // select custom baud rate.
    u4Ctr &= ~(UINT32)0xFFFF0F00;
    u4Ctr |= (UINT32)0x0D00;

    u4Reg = ((30*1000*1000*16)/(u4BaudRate)) - 16;

    u4Ctr |= (UINT32)(u4Reg << 16);
    
    UART_WRITE32(u4RegAddr, u4Ctr);
}

void UART_HwSetBaudRate(UINT32 u4Port, UINT32 u4BaudRate)
{
    UINT32 u4Ctr;
    UINT32 u4RegAddr;

    GET_COMMUNICATE_CTRL_REG(u4Port, u4RegAddr)

    u4Ctr  = UART_READ32(u4RegAddr);
    u4Ctr &= (UINT32)0x00FF;

    switch (u4BaudRate)
    {
    case RS_232_SPEED_110:
        u4Ctr |= BUAD_RATE_110;
        break;

    case RS_232_SPEED_300:
        u4Ctr |= BUAD_RATE_300;
        break;

    case RS_232_SPEED_1200:
        u4Ctr |= BUAD_RATE_1200;
        break;

    case RS_232_SPEED_2400:
#ifdef CC_UART_PD_SUPPORT    
        u4Ctr |= (u4Port == UART_PORT_PD) ? BUAD_RATE_PD_2400 : BUAD_RATE_2400;
#else
        u4Ctr |= BUAD_RATE_2400;
#endif /* CC_UART_PD_SUPPORT */
        break;

    case RS_232_SPEED_4800:
        u4Ctr |= BUAD_RATE_4800;
        break;

    case RS_232_SPEED_9600:
#ifdef CC_UART_PD_SUPPORT    
        u4Ctr |= (u4Port == UART_PORT_PD) ? BUAD_RATE_PD_9600 : BUAD_RATE_9600;
#else
        u4Ctr |= BUAD_RATE_9600;
#endif /* CC_UART_PD_SUPPORT */
        break;

    case RS_232_SPEED_19200:
#ifdef CC_UART_PD_SUPPORT    
        u4Ctr |= (u4Port == UART_PORT_PD) ? BUAD_RATE_PD_19200 : BUAD_RATE_19200;
#else
        u4Ctr |= BUAD_RATE_19200;
#endif /* CC_UART_PD_SUPPORT */
        break;

    case RS_232_SPEED_38400:
#ifdef CC_UART_PD_SUPPORT    
        u4Ctr |= (u4Port == UART_PORT_PD) ? BUAD_RATE_PD_38400 : BUAD_RATE_38400;
#else
        u4Ctr |= BUAD_RATE_38400;
#endif /* CC_UART_PD_SUPPORT */
        break;

    case RS_232_SPEED_57600:
#ifdef CC_UART_PD_SUPPORT    
        u4Ctr |= (u4Port == UART_PORT_PD) ? BUAD_RATE_PD_57600 : BUAD_RATE_57600;
#else
        u4Ctr |= BUAD_RATE_57600;
#endif /* CC_UART_PD_SUPPORT */
        break;

    case RS_232_SPEED_115200:
        u4Ctr |= BUAD_RATE_115200;
        break;

    case RS_232_SPEED_230400:
        u4Ctr |= BUAD_RATE_115200x2;
        break;

    case RS_232_SPEED_460800:
        u4Ctr |= BUAD_RATE_115200x4;
        break;

    case RS_232_SPEED_921600:
        u4Ctr |= BUAD_RATE_115200x8;
        break;

    default:
        LOG(1, "BaudRate %d not support\n", u4BaudRate);
        break;
    }

    UART_WRITE32(u4RegAddr, u4Ctr);
}

void UART_HwSetStopBit(UINT32 u4Port, UINT32 u4StopBit)
{
    UINT32 u4Ctr;
    UINT32 u4RegAddr;

    GET_COMMUNICATE_CTRL_REG(u4Port, u4RegAddr)

    u4Ctr  = UART_READ32(u4RegAddr);
    u4Ctr &= (UINT32)0xFFFB;

    switch (u4StopBit)
    {
    case RS_232_STOP_BIT_1:
        u4Ctr |= (u4Port == UART_PORT_1) ? STOP_BIT_1_EXT : STOP_BIT_1;
        break;

    case RS_232_STOP_BIT_1_5:
        u4Ctr |= (u4Port == UART_PORT_1) ? STOP_BIT_1_2_EXT : STOP_BIT_1_2;
        break;

    case RS_232_STOP_BIT_2:
        u4Ctr |= (u4Port == UART_PORT_1) ? STOP_BIT_2_EXT : STOP_BIT_2;
        break;

    default:
        LOG(1, "StopBit %d not support\n", u4StopBit);
    }

    UART_WRITE32(u4RegAddr, u4Ctr);
}

void UART_HwSetDataLen(UINT32 u4Port, UINT32 u4DataLen)
{
    UINT32 u4Ctr;
    UINT32 u4RegAddr;

    GET_COMMUNICATE_CTRL_REG(u4Port, u4RegAddr)

    u4Ctr  = UART_READ32(u4RegAddr);
    u4Ctr &= (UINT32)0xFFFC;

    switch (u4DataLen)
    {
    case RS_232_DATA_LEN_5:
        u4Ctr |= BIT_LEN_5;
        break;

    case RS_232_DATA_LEN_6:
        u4Ctr |= BIT_LEN_6;
        break;

    case RS_232_DATA_LEN_7:
        u4Ctr |= BIT_LEN_7;
        break;

    case RS_232_DATA_LEN_8:
        u4Ctr |= BIT_LEN_8;
        break;

    default:
        LOG(1, "DataLen %d not support\n", u4DataLen);
    }

    UART_WRITE32(u4RegAddr, u4Ctr);
}

void UART_HwGetBaudRate(UINT32 u4Port, RS_232_SETUP_INFO_T *prSetupInfo)
{
    UINT32 u4BaudRate;
    UINT32 u4RegAddr;

    ASSERT(prSetupInfo);

    GET_COMMUNICATE_CTRL_REG(u4Port, u4RegAddr)
        
    u4BaudRate = (UART_READ32(u4RegAddr) & 0xFF00) >> 8;

    switch (u4BaudRate)
    {
    case 0:
        prSetupInfo->e_speed = RS_232_SPEED_115200;
        break;

    case 1:
        prSetupInfo->e_speed = RS_232_SPEED_230400;
        break;

    case 2:
        prSetupInfo->e_speed = RS_232_SPEED_460800;
        break;

    case 3:
        prSetupInfo->e_speed = RS_232_SPEED_921600;
        break;

    case 4:
        prSetupInfo->e_speed = RS_232_SPEED_57600;
        break;

    case 5:
        prSetupInfo->e_speed = RS_232_SPEED_38400;
        break;

    case 6:
        prSetupInfo->e_speed = RS_232_SPEED_19200;
        break;

    case 7:
        prSetupInfo->e_speed = RS_232_SPEED_9600;
        break;

    case 8:
        prSetupInfo->e_speed = RS_232_SPEED_4800;
        break;

    case 9:
        prSetupInfo->e_speed = RS_232_SPEED_2400;
        break;

    case 10:
        prSetupInfo->e_speed = RS_232_SPEED_1200;
        break;

    case 11:
        prSetupInfo->e_speed = RS_232_SPEED_300;
        break;

    case 12:
        prSetupInfo->e_speed = RS_232_SPEED_110;
        break;

    default:
        LOG(1, "ERROR \n");
    }
}

void UART_HwGetParity(UINT32 u4Port, RS_232_SETUP_INFO_T *prSetupInfo)
{
    UINT32 u4RegLCR = 0;
    UINT32 u4RegAddr;

    ASSERT(prSetupInfo);
    
    GET_COMMUNICATE_CTRL_REG(u4Port, u4RegAddr)

    u4RegLCR = UART_READ32(u4RegAddr);

    // Parity	
    if (u4RegLCR & (UINT32)UART_PARITY_ENABLE)
    {
        if (u4RegLCR & (UINT32)UART_PARITY_EVEN)
        {
            prSetupInfo->e_parity = RS_232_PARITY_EVEN;
        }
        else
        {
            prSetupInfo->e_parity = RS_232_PARITY_ODD;
        }
    }
    else
    {
        prSetupInfo->e_parity = RS_232_PARITY_NONE;
    }
}

void UART_HwGetStopBit(UINT32 u4Port, RS_232_SETUP_INFO_T *prSetupInfo)
{
    UINT32 u4RegLCR = 0;
    UINT32 u4RegAddr;

    ASSERT(prSetupInfo);

    GET_COMMUNICATE_CTRL_REG(u4Port, u4RegAddr)
        
    u4RegLCR = UART_READ32(u4RegAddr);

    // Stop Bit	
    if (u4RegLCR & (UINT32)STOP_BIT_2)
    {
        prSetupInfo->e_stop_bit = RS_232_STOP_BIT_2;
    }
    else
    {
        prSetupInfo->e_stop_bit = RS_232_STOP_BIT_1;
    }
}

void UART_HwGetDataLen(UINT32 u4Port, RS_232_SETUP_INFO_T *prSetupInfo)
{
    UINT32 u4RegLCR = 0;
    UINT32 u4RegAddr;
    UINT32 u4Tmp;

    ASSERT(prSetupInfo);

    GET_COMMUNICATE_CTRL_REG(u4Port, u4RegAddr)
        
    u4RegLCR = UART_READ32(u4RegAddr);
    u4Tmp = u4RegLCR & (UINT32)0x03;
    
    // data length	
    switch (u4Tmp)
    {
    case 0:
        prSetupInfo->e_data_len = RS_232_DATA_LEN_8;
        break;

    case 1:
        prSetupInfo->e_data_len = RS_232_DATA_LEN_7;
        break;

    case 2:
        prSetupInfo->e_data_len = RS_232_DATA_LEN_6;
        break;

    case 3:
        prSetupInfo->e_data_len = RS_232_DATA_LEN_5;
        break;

    default:
        ASSERT(0);
        break;
    }
}

UINT32 UART_HwGetRxDataCnt(UINT32 u4Port)
{
    UINT32 u4RxCnt = 0;

    switch (u4Port)
    {
    case UART_PORT_0:
        // In normal mode, get actual rx size from REG_NROMAL_MODE_BUF_SZ register.
        if (UART_HwCheckPortTransMode(u4Port))
        {
            u4RxCnt = (UART_READ32(REG_SER_STATUS) & SER_READ_ALLOW);
        }
        else
        {
            u4RxCnt = UART_READ32(REG_NROMAL_MODE_BUF_SZ) + 1;
        }
        break;

    case UART_PORT_1:
        u4RxCnt = UART_READ32(REG_SER1_DMAW_LEVEL);
        break;

#ifdef CC_UART_PD_SUPPORT    
    case UART_PORT_PD:
        // In normal mode, get actual rx size from REG_NROMAL_MODE_BUF_SZ register.
        if (UART_HwCheckPortTransMode(u4Port))
        {
            u4RxCnt = (UART_READ32(REG_SERPD_STATUS) & SER_READ_ALLOW);
        }
        else
        {
            u4RxCnt = UART_READ32(UART_PD_BASE + REG_NROMAL_MODE_BUF_SZ) + 1;
        }
        break;
#endif /* CC_UART_PD_SUPPORT */
        
    default:
        ASSERT(0);
        break;
    }

    return u4RxCnt;
}

UINT32 UART_HwGetTxEmptyCnt(UINT32 u4Port)
{
    UINT32 u4TxCnt = 0;

    switch (u4Port)
    {
    case UART_PORT_0:
        u4TxCnt = UART_READ32(REG_SER_STATUS);
        break;
        
    case UART_PORT_1:
        u4TxCnt = UART_READ32(REG_SER01_STATUS);
        break;
        
#ifdef CC_UART_PD_SUPPORT    
    case UART_PORT_PD:
        u4TxCnt = UART_READ32(REG_SERPD_STATUS);
        break;
#endif /* CC_UART_PD_SUPPORT */
    }

    u4TxCnt = (u4TxCnt & SER_WRITE_ALLOW) >> 8;

    // Reserve 4 byte for safety
    //u4TxCnt = (u4TxCnt >= 4) ? (u4TxCnt - 4) : 0;

    return u4TxCnt;
}

BOOL UART_HwWriteAllow(UINT32 u4Port)
{
    return (UART_HwGetTxEmptyCnt(u4Port) > 0) ? TRUE : FALSE;
}

BOOL UART_HwReadAllow(UINT32 u4Port)
{
    return (UART_HwGetRxDataCnt(u4Port) > 0) ? TRUE : FALSE;
}

void UART_HwClearFifo(UINT32 u4Port, UINT32 u4Option)
{
    UINT32 u4Ctr;
    UINT32 u4Addr;

    switch (u4Port)
    {
    case UART_PORT_0:
        u4Addr = REG_SER_BUF_CTL;
        break;

    case UART_PORT_1:
        u4Addr = REG_SER01_BUF_CTL;
        break;

#ifdef CC_UART_PD_SUPPORT    
    case UART_PORT_PD:
        u4Addr = REG_SERPD_BUF_CTL;
        break;
#endif /* CC_UART_PD_SUPPORT */
        
    default:
        return;
    }
    
    u4Ctr = UART_READ32(u4Addr);
    u4Ctr |= (u4Option);
    UART_WRITE32(u4Addr, u4Ctr);
}

UINT32 UART_HwGetIntStatus(UINT32 u4Port)
{    
    UINT32 u4IntStatus;

#ifdef CC_UART_PD_SUPPORT    
    if (u4Port == UART_PORT_PD)
    {
        u4IntStatus  = UART_READ32(REG_SERPD_INT_STATUS);
        u4IntStatus &= UART_READ32(REG_SERPD_INT_EN);
        u4IntStatus &= INT_SERPD_MASK;
    }
    else
#endif /* CC_UART_PD_SUPPORT */
    {
        u4IntStatus  = UART_READ32(REG_SER_INT_STATUS);
        u4IntStatus &= UART_READ32(REG_SER_INT_EN);
        u4IntStatus &= (INT_SER0_MASK | INT_SER1_MASK);
    }
    
    return u4IntStatus;
}

void UART_HwClearIntStatus(UINT32 u4Port,UINT32 u4Data)
{    
    switch (u4Port)
    {
    case UART_PORT_0:
    case UART_PORT_1:
        UART_WRITE32(REG_SER_INT_STATUS, u4Data);
        break;

#ifdef CC_UART_PD_SUPPORT    
    case UART_PORT_PD:
        UART_WRITE32(REG_SERPD_INT_STATUS, u4Data);
        break;
#endif /* CC_UART_PD_SUPPORT */

    default:
        break;
    }
}

void UART_HwWrite(UINT32 u4Port, const UINT8 *pu1Data, UINT32 u4NumberToWrite)
{
    UINT32 i;

    ASSERT(pu1Data);
    ASSERT(u4NumberToWrite <= 0x1F);

    for (i = 0; i < u4NumberToWrite; i++)
    {
        UART_WRITE_DATA(u4Port, *pu1Data);
        pu1Data++;
    }
}

BOOL UART_HwTransMode(void)
{
    return ((UART_READ32(REG_SER_CTL) & (UINT32)TRANS_MODE_ON) > 0);
}

BOOL UART_HwCheckPortTransMode(UINT32 u4Port)
{
    switch (u4Port)
    {
    case UART_PORT_0:
        return ((UART_READ32(REG_SER_CTL) & (UINT32)TRANS_MODE_ON) > 0);

#ifdef CC_UART_PD_SUPPORT    
    case UART_PORT_PD:
        return ((UART_READ32(REG_SERPD_CTL) & (UINT32)TRANS_MODE_ON) > 0);
#endif /* CC_UART_PD_SUPPORT */

    default:
        return FALSE;
    }
}

