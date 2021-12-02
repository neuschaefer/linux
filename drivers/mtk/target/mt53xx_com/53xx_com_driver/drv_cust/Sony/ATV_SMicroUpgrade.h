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

#ifndef SMICRO_UPGRADE_H
#define SMICRO_UPGRADE_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_pinmux.h"
#include "SMicro/x_crc16.h"
#include "SMicro/SMicro_prc.h"
#include "SMicro/SMicro_if.h"

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define UART_READ32(offset)         (IO_READ32(RS232_BASE, offset))
#define UART_WRITE32(offset, val)   (IO_WRITE32(RS232_BASE, offset, val))

#define UART1_DATA1                 (0x00c0)
#define UART1_WRITE_BYTE(ch)        (UART_WRITE32(UART1_DATA1, (ch & 0xffU)))
#define UART1_READ_BYTE()           (UART_READ32(UART1_DATA1) & 0xffU)

#define UART1_COMMU                 (0x00c8)
#define UART1_BAUDRATE              (0x0fU << 8)
#define BAUDRATE_115200             (0 << 8)
#define BAUDRATE_921600             (3 << 8)
#define UART_SET_BAUDRATE(x)        UART_WRITE32(UART1_COMMU, (UART_READ32(UART1_COMMU) & ~UART1_BAUDRATE) | x)

#define UART1_STATUS                (0x00cc)
#define UART1_RXD_BUF_STATE         (0x1fU << 0)
#define IS_UART1_RXBUF_EMPTY()      ((UART_READ32(UART1_STATUS) & UART1_RXD_BUF_STATE) == 0)
#define UART1_TXD_BUF_STATE         (0x1fU << 8)
#define IS_UART1_TXBUF_FULL()       ((UART_READ32(UART1_STATUS) & UART1_TXD_BUF_STATE) == 0)

#define UART1_BUF_CTL               (0x00d0)
#define UART1_CLEAR_RBUF            (1 << 6)
#define UART1_CLEAR_TBUF            (1 << 7)

#define PCB_TYPE_2             0x02

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define DATA_DUMP(u4Len, pBuffer)   {                                   \
                                        UINT32 i;                       \
                                        for (i = 0; i < u4Len; i++)     \
                                        {                               \
                                            Printf("%2x ", pBuffer[i]); \
                                        }                               \
                                    }

#define UART1_FLUSH_BUFFER()                                            \
    {                                                                   \
        while ((UART_READ32(UART1_STATUS) & UART1_TXD_BUF_STATE) !=     \
               UART1_TXD_BUF_STATE);                                    \
        UINT32 u4Ctr = UART_READ32(UART1_BUF_CTL);                      \
        u4Ctr |= (UART1_CLEAR_RBUF | UART1_CLEAR_TBUF);                 \
        UART_WRITE32(UART1_BUF_CTL, u4Ctr);                             \
    }

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
static BOOL   _bfgIrisSMicroInit = FALSE;
static UINT8  _u1SMHdrToggle = SMICOR_SOF_N0_ODD;
static UINT8  _u1SMRespToggle = SMICOR_SOF_N0_ODD;
static UINT8  _u1SMRxData[SMICRO_PRC_PAYLOAD_MAX];
static UINT8  _u1SMTxData[SMICRO_PRC_PACKET_MAX];
static UINT32 _u4RetryCnt = SMICRO_CMD_RETRY_MAX;

static SMICRO_FRAME_HEADER _rSMTxFrameHdr;
static SMICRO_FRAME_HEADER _rSMRxFrameHdr;
static SMICRO_PAYLOAD_HEADER _rSMTxPayloadHdr;
static SMICRO_PAYLOAD_HEADER _rSMRxPayloadHdr;
static SMICRO_CRC _rSMCRC16;

extern UINT32 _u4gPCBType;

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
static INT32 uart1_write(UINT8 u1Ch)
{
    HAL_TIME_T rOrgTime, rNewTime, rDiffTime;

    HAL_GetTime(&rOrgTime);
    while (IS_UART1_TXBUF_FULL())
    {
        HAL_GetTime(&rNewTime);
        HAL_GetDeltaTime(&rDiffTime, &rOrgTime, &rNewTime);
        if ((rDiffTime.u4Micros > 50000) || (rDiffTime.u4Seconds > 1))
        {
            return -1;
        }
    }

    UART1_WRITE_BYTE(u1Ch);
    return 0;
}

static INT32 uart1_read(UINT8 *pu1Ch)
{
    HAL_TIME_T rOrgTime, rNewTime, rDiffTime;

    HAL_GetTime(&rOrgTime);
    while (IS_UART1_RXBUF_EMPTY())
    {
        HAL_GetTime(&rNewTime);
        HAL_GetDeltaTime(&rDiffTime, &rOrgTime, &rNewTime);
        if ((rDiffTime.u4Micros > 50000) || (rDiffTime.u4Seconds > 1))
        {
            return -1;
        }
    }

    *pu1Ch = UART1_READ_BYTE();

    return 0;
}

void SMicro_prc_rtx_Init(void)
{
    if (!_bfgIrisSMicroInit)
    {
        
        // Open uart1 Tx/Rx pinmux
        VERIFY(BSP_PinSet(U2RX, 1) == OSR_OK);
        
        _bfgIrisSMicroInit = TRUE;
    }
}

SMICRO_CMD_TYPE SMicroGetCmdType(UINT32 u4ModuleId, UINT32 u4CmdId)
{
    SMICRO_CMD_TYPE eCmdType = SMICRO_CMD_NO_REPLY;
    
    switch (u4ModuleId)
    {
    case MODULEID_GENERIC:
        eCmdType = (u4CmdId == CMD0_GET_ACK) ? 
                   SMICRO_CMD_NO_REPLY : SMICRO_CMD_SYNC_ONCE;
        break;
        
    case MODULEID_FACTORY:
        eCmdType = (u4CmdId == CMD6_SET_FACTORY_DATA) ? 
                   SMICRO_CMD_NO_REPLY : SMICRO_CMD_ASYNC_CONTINUOUS;
        break;
        
    case MODULEID_DOWNLOAD:
        if ((u4CmdId == CMD7_START_DOWNLOAD) || 
            (u4CmdId == CMD7_SEND_DOWNLOAD_DATA))
        {
            eCmdType = SMICRO_CMD_ASYNC_ONCE;
        }
        else
        {
            eCmdType = SMICRO_CMD_SYNC_ONCE;
        }
        break;
        
    case MODULEID_PANEL_PARAM:
        switch (u4CmdId)
        {
        case CMD17_PRE_SET_VERTICAL_FREQ:
            eCmdType = SMICRO_CMD_SYNC_ONCE;
            break;
        case CMD17_ENABLE_BACKEND:
        case CMD17_NOTIFY_TCON_READY:
            eCmdType = SMICRO_CMD_ASYNC_ONCE;
            break;
        case CMD17_NOTIFY_BACKEND_READY:
        case CMD17_NOTIFY_TEMP:
            eCmdType = SMICRO_CMD_ASYNC_CONTINUOUS;
            break;
        }
        break;
    }
    
    return eCmdType;
}

INT32 SMicro_prc_tx(UINT8 *pBuffer, UINT32 u4NumberToWrite)
{
    UINT8  u1TxCh;
    UINT8  *pPtr = pBuffer;
    UINT32 i;
    
    // Read the rx data
    for (i = 0; i < u4NumberToWrite; i++)
    {
        u1TxCh = *(pPtr++);
        if (uart1_write(u1TxCh) == -1)
        {
            break;
        }
    }
    
    return (i);
}


INT32 SMicro_prc_rx(UINT8 *pBuffer, UINT32 u4NumberToRead)
{
    UINT32 i;

    // Read the rx data
    for (i = 0; i < u4NumberToRead; i++)
    {
        if (uart1_read(pBuffer + i) == -1)
        {
            return SMICRO_PRC_FAIL;
        }
    }
    
    return SMICRO_PRC_OK;
}

INT32 SMicro_prc_tx_ack(UINT32 u4Module, UINT32 u4Command)
{
    UINT32 u4NumberToWrite, u4WriteLen;
    UINT32 u4Index = 0;
    UINT16 u2CRC;
    
    // Set SOF
    _rSMTxFrameHdr.u1SOF.Field.u1Header = SMICRO_SOF_FIX_DATA;
    _rSMTxFrameHdr.u1SOF.Field.u1DataType = SMICRO_SOF_DATA_TYPE_ACK;
    _rSMTxFrameHdr.u1SOF.Field.u1N2 = SMICRO_SOF_N2_ACK_RESPONSE;
    _rSMTxFrameHdr.u1SOF.Field.u1N1 = SMICRO_SOF_N1_NACK_CRC_ERR;
    _rSMTxFrameHdr.u1SOF.Field.u1N0 = _u1SMRespToggle;

    // Set ISOF
    _rSMTxFrameHdr.u1ISOF = ~(_rSMTxFrameHdr.u1SOF.Value);

    // CRC calculate
    u4Index = 2;
    x_memcpy(_u1SMTxData, &_rSMTxFrameHdr, sizeof(SMICRO_FRAME_HEADER));
    u2CRC = crc16_ccitt(_u1SMTxData, u4Index);
    _rSMTxFrameHdr.u1Length[1] = (UINT8)u2CRC;
    _rSMTxFrameHdr.u1Length[0] = (UINT8)(u2CRC >> 8);
    
    // Data packing
    x_memcpy(_u1SMTxData, &_rSMTxFrameHdr, sizeof(SMICRO_FRAME_HEADER));
    u4NumberToWrite = sizeof(SMICRO_FRAME_HEADER);
    u4WriteLen = SMicro_prc_tx(_u1SMTxData, u4NumberToWrite);
    UNUSED(u4WriteLen);

#ifndef NDEBUG
    Printf("[SMicro_prc_tx_ack] data : ");
    DATA_DUMP(sizeof(SMICRO_FRAME_HEADER), _u1SMTxData);
    Printf("\n");
#endif /* NDEBUG */

    return SMICRO_PRC_OK;
}

INT32 SMicro_prc_tx_cmd(UINT32 u4Module, UINT32 u4Command, BOOL bRetry,
                        UINT32 u4DataLen, UINT8 *pu1Data)
{
    UINT32 u4NumberToWrite, u4WriteLen;
    UINT32 u4PayloadLen, u4Index = 0;
    UINT16 u2CRC;
    UINT8  *pu1Payload;
    
    // Set SOF
    _rSMTxFrameHdr.u1SOF.Field.u1Header = SMICRO_SOF_FIX_DATA;
    _rSMTxFrameHdr.u1SOF.Field.u1DataType = SMICRO_SOF_DATA_TYPE_CMD;
    _rSMTxFrameHdr.u1SOF.Field.u1N2 = SMICRO_SOF_N2_ACK_REQUEST;
    _rSMTxFrameHdr.u1SOF.Field.u1N1 = 
        (bRetry) ? SMICRO_SOF_N1_RETRANSMIT : SMICRO_SOF_N1_FIRST_TRANS;
    _rSMTxFrameHdr.u1SOF.Field.u1N0 = _u1SMHdrToggle;

    // Set ISOF
    _rSMTxFrameHdr.u1ISOF = ~(_rSMTxFrameHdr.u1SOF.Value);

    // Set Length
    u4PayloadLen = u4DataLen + sizeof(SMICRO_PAYLOAD_HEADER);
    _rSMTxFrameHdr.u1Length[1] = (UINT8)u4PayloadLen;
    _rSMTxFrameHdr.u1Length[0] = (UINT8)(u4PayloadLen >> 8);

    // Set Frame payload header
    _rSMTxPayloadHdr.u1Type = 0;
    _rSMTxPayloadHdr.u1ModelId = (UINT8)u4Module;
    _rSMTxPayloadHdr.u1CommandId = (UINT8)u4Command;
    _rSMTxPayloadHdr.u1MessageId = 0;

    // Data packing
    x_memcpy(_u1SMTxData, &_rSMTxFrameHdr, sizeof(SMICRO_FRAME_HEADER));
    u4Index += sizeof(SMICRO_FRAME_HEADER);
    pu1Payload = &_u1SMTxData[u4Index];
    x_memcpy(pu1Payload, &_rSMTxPayloadHdr, sizeof(SMICRO_PAYLOAD_HEADER));
    u4Index += sizeof(SMICRO_PAYLOAD_HEADER);
    x_memcpy(&_u1SMTxData[u4Index], pu1Data, u4DataLen);
    u4Index += u4DataLen;

    // CRC calculate
    u2CRC = crc16_ccitt(_u1SMTxData, u4Index);
    _rSMCRC16.u1CRC[1] = (UINT8)u2CRC;
    _rSMCRC16.u1CRC[0] = (UINT8)(u2CRC >> 8);
    
    x_memcpy(&_u1SMTxData[u4Index], &_rSMCRC16, sizeof(SMICRO_CRC));
    u4NumberToWrite = sizeof(SMICRO_FRAME_HEADER) + sizeof(SMICRO_PAYLOAD_HEADER) +
                      u4DataLen + sizeof(SMICRO_CRC);
    u4WriteLen = SMicro_prc_tx(_u1SMTxData, u4NumberToWrite);
    UNUSED(u4WriteLen);

#ifndef NDEBUG
    Printf("[SMicro_prc_tx_cmd] data : ");
    DATA_DUMP((u4PayloadLen + sizeof(SMICRO_FRAME_HEADER) + sizeof(SMICRO_CRC)),
              _u1SMTxData);
    Printf("\n");
#endif /* NDEBUG */

    return SMICRO_PRC_OK;
}

INT32 SMicro_prc_rx_ack(UINT32 u4Module, UINT32 u4Command)
{
    INT32 i4Ret = SMICRO_PRC_OK;
    UINT32 u4NumberToRead = sizeof(SMICRO_FRAME_HEADER);

    // Read the Frame Header of the data respond
    i4Ret = SMicro_prc_rx((UINT8 *)(VOID *)&_rSMRxFrameHdr, u4NumberToRead);
    if (i4Ret == SMICRO_PRC_FAIL)
    {
        return SMICRO_PRC_FAIL;
    }

    // Check Frame Header
    if ((_rSMRxFrameHdr.u1SOF.Field.u1Header != SMICRO_SOF_FIX_DATA) || 
        (_rSMRxFrameHdr.u1ISOF != (UINT8)(~_rSMRxFrameHdr.u1SOF.Value)))
    {
#ifndef NDEBUG
        Printf("[SMicro_prc_rx_ack] Invalid Frame header : ");
        Printf(" SOF = %2x, ISOF = %2x \n", 
               (UINT8)_rSMRxFrameHdr.u1SOF.Value, _rSMRxFrameHdr.u1ISOF);
#endif /* NDEBUG */
        return SMICRO_PRC_FAIL;
    }

    if (_rSMRxFrameHdr.u1SOF.Field.u1DataType == SMICRO_SOF_DATA_TYPE_ACK)
    {
        if (_rSMRxFrameHdr.u1SOF.Field.u1N2 == SMICRO_SOF_N2_NACK_RESPONSE)
        {
            Printf("[SMicro_prc_rx_ack] Nack response (%2x) !\n", 
                   (UINT8)_rSMRxFrameHdr.u1SOF.Value);
            return SMICRO_PRC_FAIL;
        }
    }
    else
    {
        _u1SMRespToggle = _rSMRxFrameHdr.u1SOF.Field.u1N0;
    }
    
    return SMICRO_PRC_OK;
}

INT32 SMicro_prc_rx_cmd(BOOL bIsResponse, UINT32 u4Module, UINT32 u4Command,
                    UINT32 u4DataLen, UINT8 *pu1Data)
{
    INT32 i4Ret = SMICRO_PRC_OK;
    UINT32 u4NumberToRead;
    UINT32 u4PayloadLen;
    SMICRO_CRC rSMCRC16;

    i4Ret = SMicro_prc_rx_ack(u4Module, u4Command);
    if (i4Ret == SMICRO_PRC_FAIL)
    {
        return SMICRO_PRC_FAIL;
    }
    
    // Read the Frame Payload Header of the data respond
    u4NumberToRead = sizeof(SMICRO_PAYLOAD_HEADER);
    i4Ret = SMicro_prc_rx((UINT8 *)(VOID *)&_rSMRxPayloadHdr, u4NumberToRead);
    if (i4Ret == SMICRO_PRC_FAIL)
    {
        return SMICRO_PRC_FAIL;
    }

#ifndef NDEBUG
    Printf("[SMicro_prc_rx_cmd] data : ");
    DATA_DUMP(sizeof(SMICRO_FRAME_HEADER), ((UINT8 *)(VOID *)&_rSMRxFrameHdr));
    DATA_DUMP(sizeof(SMICRO_PAYLOAD_HEADER), ((UINT8 *)(VOID *)&_rSMRxPayloadHdr));
#endif /* NDEBUG */

    // Check Frame Header
    if ((_rSMRxPayloadHdr.u1ModelId != (UINT8)u4Module) && 
        (_rSMRxPayloadHdr.u1CommandId != (UINT8)u4Command))
    {
#ifndef NDEBUG
        Printf("[SMicro_prc_rx_cmd] Invalid payload header : ");
        Printf(" Module ID = %2x, Command ID = %2x \n", 
               _rSMRxPayloadHdr.u1ModelId, _rSMRxPayloadHdr.u1CommandId);
#endif /* NDEBUG */
    }

    // Get data payload
    u4PayloadLen = _rSMRxFrameHdr.u1Length[1];
    u4PayloadLen += (((UINT32)_rSMRxFrameHdr.u1Length[0]) << 8);
    u4PayloadLen -= sizeof(SMICRO_PAYLOAD_HEADER);

    // Read the Frame Payload of the data response
    u4NumberToRead = u4PayloadLen;
    i4Ret = SMicro_prc_rx(_u1SMRxData, u4NumberToRead);
    if (i4Ret == SMICRO_PRC_FAIL)
    {
        return SMICRO_PRC_FAIL;
    }

#ifndef NDEBUG
    DATA_DUMP(u4PayloadLen, _u1SMRxData);
#endif /* NDEBUG */

    // Read the CRC16 of the data respond
    u4NumberToRead = sizeof(SMICRO_CRC);
    i4Ret = SMicro_prc_rx((UINT8 *)(VOID *)&rSMCRC16, u4NumberToRead);
    if (i4Ret == SMICRO_PRC_FAIL)
    {
        return SMICRO_PRC_FAIL;
    }

#ifndef NDEBUG
    DATA_DUMP(sizeof(SMICRO_CRC), ((UINT8 *)(VOID *)&rSMCRC16));
    Printf("\n");
#endif /* NDEBUG */

    x_memcpy(pu1Data, _u1SMRxData, u4DataLen);
    return SMICRO_PRC_OK;
}

UINT32 u4SMicroSendCmd(UINT32 u4ModuleId, UINT32 u4CmdId, void *pData,
                       UINT32 u4DataLen, void *pAckData, UINT32 u4AckLen)
{
    BOOL bRetry;
    UINT32 u4SendCnt = 0;
    INT32 i4Ret = SMICRO_PRC_OK;
    SMICRO_CMD_TYPE eCmdType;

    if (_u4gPCBType != PCB_TYPE_2)
    {
        return SMICRO_PRC_OK;
    }
    
    SMicro_prc_rtx_Init();
    if (u4DataLen > SMICRO_PRC_PAYLOAD_MAX)
    {
        Printf("[IrisSMicroSendCmd] Input data length (%d) > Max. payload",
            u4DataLen);
        return SMICRO_PRC_FAIL;
    }
    
    if (u4AckLen > SMICRO_PRC_PAYLOAD_MAX)
    {
        Printf("[IrisSMicroSendCmd] Output data length (%d) > Max. payload",
            u4AckLen);
        return SMICRO_PRC_FAIL;
    }
    
    bRetry = FALSE;
    _u1SMHdrToggle = (_u1SMHdrToggle == SMICOR_SOF_N0_EVEN) ?
                     SMICOR_SOF_N0_ODD : SMICOR_SOF_N0_EVEN;
    eCmdType = SMicroGetCmdType(u4ModuleId, u4CmdId);

    do
    {
        i4Ret = SMicro_prc_tx_cmd(u4ModuleId, u4CmdId, bRetry, u4DataLen, pData);
        i4Ret = SMicro_prc_rx_ack(u4ModuleId, u4CmdId);
#ifndef NDEBUG
        Printf("[SMicro_prc_rx_ack] data : ");
        DATA_DUMP(sizeof(SMICRO_FRAME_HEADER), ((UINT8 *)(VOID *)&_rSMRxFrameHdr));
#endif /* NDEBUG */

        switch (eCmdType)
        {
        case SMICRO_CMD_SYNC_ONCE:
        case SMICRO_CMD_ASYNC_ONCE:
        case SMICRO_CMD_ASYNC_CONTINUOUS:
#ifndef NDEBUG
        Printf("\n");
#endif /* NDEBUG */

            i4Ret = SMicro_prc_rx_cmd(eCmdType, u4ModuleId, u4CmdId, u4AckLen, pAckData);
            if (i4Ret == SMICRO_PRC_OK)
            {
                i4Ret = SMicro_prc_tx_ack(u4ModuleId, u4CmdId);
            }
            break;
        }
#ifndef NDEBUG
        Printf("\n");
#endif /* NDEBUG */
        
        if (i4Ret == SMICRO_PRC_FAIL)
        {
            Printf("[u4SMicroSendCmd] UART1 Rx/Tx Error \n");
            
            // Clear Rx/Tx buffer
            UART1_FLUSH_BUFFER();

            // Set retry counter
            u4SendCnt++;
            bRetry = TRUE;
        }
        else
        {
            break;
        }
    } while ((u4SendCnt < _u4RetryCnt) && bRetry);

    return (i4Ret);
}


UINT32 u4SMicroGetVersion(UINT32 u4ModelId, SMICRO_VER *pVerInfo)
{
//    LOG(3, "Loader u4SMicroGetVersion(%d)\n", u4ModelId);

    if (pVerInfo == NULL)
    {
  //      LOG(1, "Invalid parameters\n");
        return SM_NG;
    }
    
    if (u4SMicroSendCmd(MODULEID_GENERIC, CMD0_GET_VERSION, (void*)&u4ModelId, 4, pVerInfo, 128) != SMICRO_PRC_OK)
    {
        Printf("u4SMicroGetVersion Failed!\n");
        return SM_NG;
    }   

    return SM_OK;
}

#endif /* SMICRO_UPGRADE_H */

