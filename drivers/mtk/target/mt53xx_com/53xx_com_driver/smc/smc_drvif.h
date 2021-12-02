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
 * $RCSfile: smc_drvif.h,v $
 * $Revision: #1 $
 *
 * Description:
 *         This header file contains driver implementation of SmartCard.
 *---------------------------------------------------------------------------*/

/** @file smc_drvif.h
 *  smc_drvif.h is the header of SmartCard Interface.
 */

#ifndef _SMC_DRVIF_H
#define _SMC_DRVIF_H

#include "x_typedef.h"
#include "x_debug.h"
#include "smc_if.h"

// #define CC_FPGA
#define SMC_ERROR_PRINT(format, ...)      LOG(5, format, ## __VA_ARGS__)
#define SMC_LOG_PRINT(format, ...)      LOG(6, format, ## __VA_ARGS__)
#define SMC_DEBUG_PRINT(format, ...)      LOG(7, format, ## __VA_ARGS__)
#define SMC_INFO_PRINT(format, ...)      LOG(8, format, ## __VA_ARGS__)


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
 *  Constant definitions
 *****************************************************************************/
#define SMCR_OK             ((INT32)  0)
#define SMCR_INV_ARG        ((INT32) -1)
#define SMCR_FAIL           ((INT32) -2)
#define SMCR_CARD_WRITE_ERROR     ((INT32) -3)
#define SMCR_CARD_READ_ERROR     ((INT32) -4)
#define SMCR_LRC_ERROR     ((INT32) -5)
#define SMCR_CARD_ERROR     ((INT32) -6)
#define SMCR_NO_CARD        ((INT32) -7)

/*ATR data define*/
#define TA_MASK             0x10
#define TB_MASK             0x20
#define TC_MASK             0x40
#define TD_MASK             0x80

#define ATR_BUF_LEN 256

#define SmcSetResult(_SMC, _result) \
{                                   \
   _SMC.eCmdRes = _result;          \
}


/******************************************************************************
 *  Type definitions
 *****************************************************************************/
typedef enum SMC_EMU_MSG_T
{
    EMU_MSG_TXTIDE,
    EMU_MSG_RXTIDE,
    EMU_MSG_OVRUN,
    EMU_MSG_TOUT,
    EMU_MSG_TXERR,
    EMU_MSG_ATRERR,
    EMU_MSG_SIMOFF,
    EMU_MSG_T0END,
    EMU_MSG_RXERR,
    EMU_MSG_T1END,
    EMU_MSG_EDCERR
} SMC_EMU_MSG;


/* Event */
#define MSG_DEACT_END 0
#define MSG_INIRET 1
#define MSG_RST_READY 2
#define MSG_CMD_END 3
#define MSG_PTS_END 4
#define MSG_ATR_END 5
#define MSG_CLK_PROC 6
#define MSG_T1_SEND_END 7
#define MSG_T1_RECV_END 8
#define MSG_CHAR_SEND_END 9
#define MSG_CHAR_RECV_END 10  
#define MSG_CHAR_SEND_TIDE 11
#define MSG_REMOVE 12
#define MSG_UNKNOWN 13

#define MSG_MSK_DEACT_END (1 << MSG_DEACT_END)
#define MSG_MSK_INIRET (1 << MSG_INIRET)
#define MSG_MSK_RST_READY (1 << MSG_RST_READY)
#define MSG_MSK_CMD_END (1 << MSG_CMD_END)
#define MSG_MSK_PTS_END (1 << MSG_PTS_END)
#define MSG_MSK_ATR_END (1 << MSG_ATR_END)
#define MSG_MSK_CLK_PROC (1 << MSG_CLK_PROC)
#define MSG_MSK_T1_SEND_END (1 << MSG_T1_SEND_END)
#define MSG_MSK_T1_RECV_END (1 << MSG_T1_RECV_END)
#define MSG_MSK_CHAR_SEND_END (1 << MSG_CHAR_SEND_END)
#define MSG_MSK_CHAR_RECV_END (1 << MSG_CHAR_RECV_END) 
#define MSG_MSK_CHAR_SEND_TIDE (1 << MSG_CHAR_SEND_TIDE)
#define MSG_MSK_REMOVE (1 << MSG_REMOVE)
#define MSG_MSK_UNKNOWN (1 << MSG_UNKNOWN)

/* SMC State   */
typedef enum SMC_STATE_T
{
    STA_WAIT_FOR_ATR = 0, /* reset SMC card and wait ATR */
    STA_PROCESS_ATR,      /* receiving ATR data */
    STA_PROCESS_PTS,      /* receiving PTS response data */
    STA_PROCESS_CMD,
    STA_SERIOUS_ERR,      /* serous error due to txerr*/
    STA_POWER_OFF,
    STA_WAIT_REJECT_DONE
} SMC_STATE;


/* SMC Command State   */
typedef enum SMC_CMD_STATE_T
{
    CMD_STA_PROCESS_CLK = 0,
    CMD_STA_STOP_CLK,
    CMD_STA_WAIT_END,
    CMD_STA_T1_SEND,
    CMD_STA_T1_RECV,
    CMD_STA_CHAR_SEND,
    CMD_STA_CHAR_RECV  
} SMC_CMD_STATE;


/* eSpeed   */
typedef enum SMC_SPEED_T
{
    SPEED_372 = 0,
    SPEED_64,
    SPEED_32
} SMC_SPEED;


/* data format   */
typedef enum SMC_DATA_FMT_T
{
    FMT_DIRECT = 0,
    FMT_INDIRECT
} SMC_DATA_FMT;


/* Command Result */
typedef enum SMC_CMD_RESULT_T
{
    CMD_RES_SUCCESS     = 0,
    CMD_RES_NOT_READY      ,
    CMD_RES_CARD_ERR       ,
    CMD_RES_NO_CARD        ,
    CMD_RES_PTS_ERR        ,
    CMD_RES_TX_ERR         ,
    CMD_RES_RX_ERR         ,
    CMD_RES_TOUT           ,
    CMD_RES_CLK_PROC       ,
    CMD_RES_ATR_FMT_ERR
} SMC_CMD_RESULT;

// typedef void (* PFN_SMC_HOTPLUG_CALLBACK)(UINT8 u1HotPlugStatus);

typedef struct SMART_CARD_T
{
    SMC_STATE       eState;
    SMC_DATA_FMT    eDataFmt;               // FMT_DIRECT, FMT_INDIRECT
    SMC_CMD_RESULT  eCmdRes;                // for ATR, CMD, RST
    UINT8           pucRcvBuf[ATR_BUF_LEN]; // PTS or ATR data
    BOOL            bRcvErr;                //
    UINT16          u2RcvLen;               // for command, ATR process

    UINT8           *pucTxBuf;              //
    UINT16          u2TxSize;               //
    UINT16          u2TxIdx;                //
    UINT8           *pucRxBuf;              //

    // For clock stop mode
    volatile SMC_CMD_STATE   eCmdSta;                //
    SMC_SPEED       eSpeed;                 // Speed372,Speed64,Speed32
    BOOL            bClkStop;               // Clok Stop Enable
    BOOL            bClkStopLvl;            // Clok Stop level
} SMART_CARD;



/******************************************************************************
 *  Export definition
 *****************************************************************************/
extern BOOL bTestAtrErr;
extern BOOL bTestTxTide;
extern BOOL bTestTxRxErr;
extern BOOL bTestWTime;

extern HANDLE_T hSmcEmuMsgQ;


/******************************************************************************
 *  Export API
 *****************************************************************************/
void SmcInitDrv(void);
void SmcReject(void);
BOOL SmcResetNoATR(void);
void SmcInitialize(SMC_DATA_FMT eFormat);
BOOL SmcProcATR(void);
UINT16 SmcT0Cmd(UINT8 *txData,
                UINT16 txSize,
                UINT8 *pucRxBuf,
                UINT16 *rcvSize,
                UINT8 *Error);
UINT16 SmcT1Cmd(UINT8 *txData,
               UINT16 txSize,
               UINT8 *pucRxBuf,
               UINT16 *rcvSize,
               UINT8 *Error);
BOOL SmcResetModule(void);
INT32 SmcResetCard(UINT8 *pucATR, UINT32 u4BufLen, UINT32 *u4ATRLen);

BOOL SmcSendPps(UINT8 u1T, UINT8 u1FD);

INT32 SmcActCard(UINT8 *pucATR, UINT32 u4BufLen, UINT32 *u4ATRLen);
INT32 SmcDeactCard(void);
void SmcReset(void);

UINT16 _SMC_SendBlock(UINT8 *txData,
               UINT32 txSize,
               UINT8 *Error);
UINT16 _SMC_ReceiveBlock(UINT8 *pucRxBuf,
               UINT32 *rcvSize,
               UINT8 *Error);

INT32 _SMC_Op_Lock(void);

INT32 _SMC_Op_Unlock(void);

INT32 _SMC_Reactivate (void);

void _SMC_SetBWT(UINT16 u2BWT);

void _SMC_SetCWT(UINT16 u2CWT);

#endif /* _SMC_DRVIF_H */

