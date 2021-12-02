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
 * $RCSfile: cec_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file drv_hdmicec.c
 *  This C file implements the CEC hardware protocol functions.  
 */
//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "general.h"
#include "hw_bim.h"
#include "hw_cec.h"
#include "drv_hdmicec.h"
#include "hw_reg.h"
#include "drv_pdwnc.h"

#if ENABLE_CEC
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
#define RETX_MAX_CNT 2

//#define TOSHIBA_VENDOR_ID //for toshiba

#define ERROR_HANDLE
 
//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static UINT16 _CEC_Status;
static CEC_FRAME_DESCRIPTION *prActiveRxFrame;
static UINT32 u4CecRxTag;
static CEC_FRAME_DESCRIPTION  rCECTxFrame;

#ifdef CEC_FEATURE_3_1 
CEC_OSD_NAME _rCECOsdName;
CEC_WAKEUP_OPCODE _rCECWkpOpCode;
#endif

/*
Caution: Must add "xdata" at data type when you use "_at_ XXXX"
*/
#ifdef CC_MT8223

static xdata CEC_FRAME_DESCRIPTION CEC_rx_msg_queue[RX_Q_SIZE] _at_ 0xD100; //Don't change, 0xf800 causes abnormal
static UINT8 CEC_rxQ_read_idx;
static UINT8 CEC_rxQ_write_idx;

static xdata CEC_FRAME_DESCRIPTION CEC_tx_msg_queue[TX_Q_SIZE] _at_ 0xD180;
static UINT8 CEC_txQ_read_idx;
static UINT8 CEC_txQ_write_idx;

UINT8 xdata u1CecPendingMsgIdx             _at_ 0xD200;
UINT8 xdata fgCec8032Ctrl			_at_ 0xD201; 
UINT8 xdata fgCecEnRx				_at_ 0xD202;
UINT8 xdata fgCecEnOneTouchPlay 		_at_ 0xD203;

UINT8 xdata fgCec8032Run			_at_ 0xD204;
UINT8 xdata fgCecStandbyToOn		_at_ 0xD205;
UINT8 xdata u1CecTVLogicAddr		_at_ 0xD206;
UINT8 xdata u1CecVersion			_at_ 0xD207;

UINT8 xdata u1CecMenuLanguage[3]		_at_ 0xD208;
UINT8 xdata u1CecVendorID[3]			_at_ 0xD20B;

static xdata CEC_FRAME_DESCRIPTION CEC_pending_msg[PENDING_MSG_SIZE] _at_ 0xD210;
	
#else

static xdata CEC_FRAME_DESCRIPTION CEC_rx_msg_queue[RX_Q_SIZE] _at_ 0xf900; //Don't change, 0xf800 causes abnormal
static UINT8 CEC_rxQ_read_idx;
static UINT8 CEC_rxQ_write_idx;

static xdata CEC_FRAME_DESCRIPTION CEC_tx_msg_queue[TX_Q_SIZE] _at_ 0xf980;
static UINT8 CEC_txQ_read_idx;
static UINT8 CEC_txQ_write_idx;

UINT8 xdata u1CecPendingMsgIdx            _at_ 0xFA00;
UINT8 xdata fgCec8032Ctrl                          _at_ 0xFA01; 
UINT8 xdata fgCecEnRx                                _at_ 0xFA02;
UINT8 xdata fgCecEnOneTouchPlay               _at_ 0xFA03;

UINT8 xdata fgCec8032Run                           _at_ 0xFA04;
UINT8 xdata fgCecStandbyToOn                    _at_ 0xFA05;
UINT8 xdata u1CecTVLogicAddr                     _at_ 0xFA06;
UINT8 xdata u1CecVersion		            _at_ 0xFA07;

UINT8 xdata u1CecMenuLanguage[3] 	    _at_ 0xFA08;
UINT8 xdata u1CecVendorID[3] 		    _at_ 0xFA0B;

static xdata CEC_FRAME_DESCRIPTION CEC_pending_msg[PENDING_MSG_SIZE] _at_ 0xFA10;

#endif
static UINT8 fgStoreMessage;
static CEC_FRAME_DESCRIPTION * prActiveTxFrame;

UINT8 u1ViewOnStoreIdx;
UINT8 u1ActiveSrcStoreIdx;
UINT8 u1RoutingStoreIdx;
UINT8 u1ActiveSrcLA;
UINT8 u1GetMenuLanguageStoreIdx;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
#if CEC_log 
static void _Cec_PrintFrame(CEC_FRAME_DESCRIPTION* frame) reentrant
{
    INT8 i;

    LogS(">>>>>>\n");
    LogS("frame description:\n");
    LogS("size reTXcnt init dest");
    Log4B(frame->size, frame->reTXcnt, frame->blocks.header.initiator, frame->blocks.header.destination);
    #if 0
    LogS("size: ");
    LogB(frame->size);
    LogS("sendidx:");
    LogB(frame->sendidx);
    LogS("reTXcnt:");
    LogB( frame->reTXcnt);
    LogS("header.initiator:");
    LogB(  frame->blocks.header.initiator);	
    LogS("header.destination:");
    LogB(  frame->blocks.header.destination);
    #endif
    if (frame->size != 1)
    {
        LogS("opcode:");
        LogB(frame->blocks.opcode);

        for (i=0; i<(INT8)((frame->size)-2); i++)
        {
            LogB(frame->blocks.operand[i]);
        }
    } 
    LogS("\n");
    LogS("<<<<<<\n");
}
#endif

static UINT8 _Cec_SendFrame(CEC_FRAME_DESCRIPTION *frame)
{
    UINT8 errcode = 0;

    UINT8 size;
    UINT8 *sendidx;
    UINT8 *blocks;
    UINT32 bufferdata;
    UINT8 i, j;

    SetCECStatus(STATE_TXING_FRAME);

    // CYJ.NOTE: Leave "TX hardware error handling" to _CEC_Mainloop
    if (IS_TX_FSM_FAIL() | (TX_FAIL_RECORD() > 0))
    {
        RESET_HW_TX();

#if CEC_log

        LogS("Detect TX FAIL in \n");
#endif
    }

    size = frame->size;
    sendidx = &(frame->sendidx);
    blocks = &(frame->blocks.opcode);

    if (size == 0)
    {
        ClrCECStatus(STATE_TXING_FRAME);
        return 2;
    }
    else if (size > 16)
    {
        ClrCECStatus(STATE_TXING_FRAME);
        return 2;
    }

    // CYJ.NOTE: TX HW is not idle
    if (!IS_TX_FSM_IDLE())
    {
        RESET_HW_TX();
    //ClrCECStatus(STATE_TXING_FRAME);
    //return 1;
    }

    prActiveTxFrame = frame;

    ClrCECStatus(STATE_TX_FRAME_SUCCESS);
    ClrCECStatus(STATE_TX_NOACK);

    /* fill header */
    FILL_SRC_FIELD(frame->blocks.header.initiator);
    FILL_DST_FIELD(frame->blocks.header.destination);
    size -= 1;

    // header-only
    if (size == 0)
    {
        SET_HW_TX_LEN(0);

        MARK_H_EOM(1);
        MARK_D_EOM(0);
        size = 0;
    }

    /* fill data */
    if (size > 4)
    {
        SET_HW_TX_LEN(0xf);

        MARK_H_EOM(0);
        MARK_D_EOM(0);
    }
    else if (size == 4)
    {
        SET_HW_TX_LEN(0xf);

        MARK_H_EOM(0);
        MARK_D_EOM(1);
    }
    else if (size == 3)
    {
        SET_HW_TX_LEN(0x7);

        MARK_H_EOM(0);
        MARK_D_EOM(1);
    }
    else if (size == 2)
    {
        SET_HW_TX_LEN(0x3);

        MARK_H_EOM(0);
        MARK_D_EOM(1);
    }
    else if (size == 1)
    {
        SET_HW_TX_LEN(0x1);

        MARK_H_EOM(0);
        MARK_D_EOM(1);
    }

    bufferdata = 0;

    for (i = 0, j = size; i < 4; i++)
    {
        bufferdata >>= 8;

        bufferdata |= ((UINT32)(blocks[( *sendidx)])) << 24;
        if (i < j)
        {
            (*sendidx)++;
            size--;
        }
    }

    /* EOM */
    if (size == 0)
    {
        ENABLE_INT_FAIL(1);

        ENABLE_INT_RB(0);
        ENABLE_INT_LOW(0);
        ENABLE_INT_UN(0);
        ENABLE_INT_BS(0);
    }
    else
    {
        ENABLE_INT_FAIL(1);

        ENABLE_INT_RB(1);
        ENABLE_INT_LOW(1);
        ENABLE_INT_UN(1);
        ENABLE_INT_BS(0);

        SetCECStatus(STATE_WAIT_TX_DATA_TAKEN);
    }

    FILL_TX_DATA(bufferdata);

    // CYJ.NOTE: RB_RDY interrrupt is triggered immediately after TRIGGER_TX_HW()
    // CYJ.NOTE: DONOT NEED "CRITICAL SECTION" because no access to shared resources follows TRIGGER_TX_HW()
    CLR_TX_FINISH();
    TRIGGER_TX_HW();

    return errcode;
}

static UINT8 _Cec_SendNextDataFrame(void)
{
    CEC_FRAME_DESCRIPTION *frame;

    UINT8 errcode = 0;
    UINT8 size;
    UINT8 *sendidx;
    UINT8 *blocks;
    UINT32 bufferdata;
    UINT8 i, j;

    if (IsCECStatus(STATE_WAIT_TX_DATA_TAKEN))
    {
        if (IS_INT_RB_ENABLE() && IS_TX_DATA_TAKEN())
        {
            ClrCECStatus(STATE_WAIT_TX_DATA_TAKEN);
        }
        else
        {
            // tx buffer is not emply
            return 0;
        }
    }
    else
    {
        return 0;
    }

    /* request current active TX frame */
    frame = prActiveTxFrame;

    size = frame->size;
    sendidx = &(frame->sendidx);
    blocks = &(frame->blocks.opcode);

    // CYJ.NOTE: Leave "TX hardware error handling" to _CEC_Mainloop
    if (IS_TX_FSM_FAIL() | (TX_FAIL_RECORD() > 0))
    {
        return 3;
    }

    size -= (( *sendidx)+1);

    if (size == 0)
    {
        return 0;
    }

    /* CYJ:TODO duplicate (as _Cec_SendFrame())! */
    /* fill data */
    if (size > 4)
    {
        SET_HW_TX_LEN(0xf);

        MARK_H_EOM(0);
        MARK_D_EOM(0);
    }
    else if (size == 4)
    {
        SET_HW_TX_LEN(0xf);

        MARK_H_EOM(0);
        MARK_D_EOM(1);
    }
    else if (size == 3)
    {
        SET_HW_TX_LEN(0x7);

        MARK_H_EOM(0);
        MARK_D_EOM(1);
    }
    else if (size == 2)
    {
        SET_HW_TX_LEN(0x3);

        MARK_H_EOM(0);
        MARK_D_EOM(1);
    }
    else if (size == 1)
    {
        SET_HW_TX_LEN(0x1);

        MARK_H_EOM(0);
        MARK_D_EOM(1);
    }

    bufferdata = 0;

    for (i = 0, j = size; i < 4; i++)
    {
        bufferdata >>= 8;

        bufferdata |= ((UINT32)blocks[( *sendidx)]) << 24;
        if (i < j)
        {
            (*sendidx)++;
            size--;
        }
    }

    /* EOM */
    if (size == 0)
    {
        ENABLE_INT_FAIL(1);

        ENABLE_INT_RB(0);
        ENABLE_INT_LOW(0);
        ENABLE_INT_UN(0);
        ENABLE_INT_BS(0);
    }
    else
    {
        ENABLE_INT_FAIL(1);

        ENABLE_INT_RB(1);
        ENABLE_INT_LOW(1);
        ENABLE_INT_UN(1);
        ENABLE_INT_BS(0);

        SetCECStatus(STATE_WAIT_TX_DATA_TAKEN);
    }

    FILL_TX_DATA(bufferdata);
    //LOG(7, "TRIGGER_TX_HW in %s, size: %x\n", __FUNCTION__, size);
    CLR_TX_FINISH();
    TRIGGER_TX_HW();

    return errcode;
}

static void _Cec_InitQueue(void)
{
    CEC_rxQ_write_idx = 0;
    CEC_rxQ_read_idx = 0;
    CEC_txQ_write_idx = 0;
    CEC_txQ_read_idx = 0;
}

static UINT8 _Cec_InitTxFrame(CEC_FRAME_DESCRIPTION *frame)
{
    if (!frame)
    {
        return CEC_ERROR;
    }

    if ((frame->size > CEC_MAX_MESG_SIZE) || (frame->size == 0))
    {   
        return CEC_ERROR;
    }

    // valid tx frame
    frame->reTXcnt = 0;
    frame->sendidx = 0;

    return CEC_SUCCESS;
}

static UINT8 _Cec_EnqueueTx(CEC_FRAME_DESCRIPTION* frame)
{
    if (_Cec_InitTxFrame(frame))
    {
        return CEC_ERROR;
    }

    if (IS_TX_Q_FULL())
    {
        return CEC_ERROR;
    }

    memcpy(&(CEC_tx_msg_queue[CEC_txQ_write_idx]), frame, sizeof(CEC_FRAME_DESCRIPTION));

    // CYJ.NOTE: no critical section
    CEC_txQ_write_idx = (CEC_txQ_write_idx + 1) % TX_Q_SIZE;

    return CEC_SUCCESS;
}

static CEC_FRAME_DESCRIPTION *_Cec_GetRxWriteFrame(void)
{
    if (IS_RX_Q_FULL())
    {
        return NULL;
    }

    if (CEC_rxQ_write_idx >= RX_Q_SIZE)
    {
        return NULL;
    }
    
    return (&(CEC_rx_msg_queue[CEC_rxQ_write_idx]));
}

static CEC_FRAME_DESCRIPTION *_Cec_GetRxReadFrame(void)
{
    if (IS_RX_Q_EMPTY())
    {
        return NULL;
    }

    if (CEC_rxQ_read_idx >= RX_Q_SIZE)
    {
        return NULL;
    }
    
    return (&(CEC_rx_msg_queue[CEC_rxQ_read_idx]));
}

static void _Cec_EnqueueRx(void)
{
    CEC_rxQ_write_idx = (CEC_rxQ_write_idx + 1) % RX_Q_SIZE;   
}

static void _Cec_DequeueRx(void)
{
    CEC_rxQ_read_idx = (CEC_rxQ_read_idx + 1) % RX_Q_SIZE;
}

static void _Cec_ProcessRxIsr(void)
{
    UINT32 bufferdata;
    UINT8 i, rxlen, is_d_eom;

    // <polling message> only
    if (GET_HW_RX_LEN() == 0)
    {
        NOTIFY_RX_HW_DATA_TAKEN();
        #if CEC_log
        LogS ("GET_HW_RX_LEN");
        #endif
        return;
    }

    /* new incoming message */
    if (IsCECStatus(STATE_RX_GET_NEW_HEADER))
    {
        ClrCECStatus(STATE_RX_GET_NEW_HEADER);
        if (IsCECStatus(STATE_WAIT_RX_FRAME_COMPLETE))
        {
            #if CEC_log
            LogS ("Lost EOM:2\n");
            #endif
        }
        SetCECStatus(STATE_WAIT_RX_FRAME_COMPLETE);

        prActiveRxFrame = _Cec_GetRxWriteFrame();
        if (!prActiveRxFrame)
        {
            return;
        }

        prActiveRxFrame->size = 1;
        prActiveRxFrame->sendidx = 0;
        prActiveRxFrame->reTXcnt = 0;

        // Rx do not use txtag, use this as a sequential driver tag.    
        prActiveRxFrame->txtag = (void *)(u4CecRxTag ++);        

        prActiveRxFrame->blocks.header.initiator = GET_SRC_FIELD();
        prActiveRxFrame->blocks.header.destination = GET_DST_FIELD();
    }

    if (!IsCECStatus(STATE_WAIT_RX_FRAME_COMPLETE))
    {
        NOTIFY_RX_HW_DATA_TAKEN();
        #if CEC_log
        LogS ("!STATE_WAIT_RX_FRAME_COMPLETE");
        #endif

        return;
    }

    rxlen = GET_HW_RX_LEN();
    bufferdata = GET_HW_RX_DATA();
    is_d_eom = IS_RX_D_EOM();
    NOTIFY_RX_HW_DATA_TAKEN();

    if (rxlen == 0x3)
    {
        rxlen = 2;
    }
    else if (rxlen == 0x7)
    {
        rxlen = 3;
    }
    else if (rxlen == 0xf)
    {
        rxlen = 4;
    }
    else if (rxlen != 0x1)
    {
        #if CEC_log
        LogS( "invalid rx length occurs\n");
        #endif
    }

    if (!prActiveRxFrame)
    {
        return;
    }

    // Get opcode
    if (prActiveRxFrame->size == 1)
    {
        prActiveRxFrame->blocks.opcode = bufferdata & 0xff;
        prActiveRxFrame->size++;
        bufferdata >>= 8;
        rxlen--;
    }

    // Get operand.
    for (i=0; i<rxlen; i++)
    {
        if (prActiveRxFrame->size >= 16)
        {
            break;
        }
    
        prActiveRxFrame->blocks.operand[(prActiveRxFrame->size-2)] = bufferdata & 0xff;
        prActiveRxFrame->size++;        
        bufferdata >>= 8;        
    }

    // Check if data complete.
    if (is_d_eom)
    {
        // Fill 0 to the unused operand.
        for (i=(prActiveRxFrame->size-2); i<14; i++)
        {
            prActiveRxFrame->blocks.operand[i] = 0;
        }
        
        ClrCECStatus(STATE_WAIT_RX_FRAME_COMPLETE);

        // push into rx_queue.
        _Cec_EnqueueRx();

        prActiveRxFrame = NULL;

        #if CEC_log
        LogS( "_Cec_ProcessRxIsr done");
        #endif        
    }
}

static CEC_FRAME_DESCRIPTION* _Cec_GetTxReadFrame(void)
{
    if (IS_TX_Q_EMPTY())
    {
    	return NULL;
    }

    return (&(CEC_tx_msg_queue[CEC_txQ_read_idx]));
}

static UINT8 _Cec_DequeueTx(void)
{
    if (IS_TX_Q_EMPTY())
    {
        return FALSE;
    }

    // CYJ.NOTE: no critical section
    CEC_txQ_read_idx = (CEC_txQ_read_idx + 1) % TX_Q_SIZE;

    return TRUE;
}

static UINT8 _Cec_CheckFrame(CEC_FRAME_DESCRIPTION *frame)
{
    UINT8 size = frame->size;

    UINT8 i1ret = TRUE;

    /* opcode-aware */
    /* CYJ.NOTE: code size issue */
    switch (frame->blocks.opcode)
    {
    // length == 2
    case OPCODE_ARC_INIT:
    case OPCODE_ARC_REPORT_INIT:    
    case OPCODE_ARC_REPORT_TERMINATE:    
    case OPCODE_ARC_REQUEST_INIT:
    case OPCODE_ARC_REQUEST_TERMINATE:
    case OPCODE_ARC_TERMINATE:
    case OPCODE_IMAGE_VIEW_ON:
    case OPCODE_TEXT_VIEW_ON:
    case OPCODE_REQUEST_ACTIVE_SOURCE:
    case OPCODE_STANDBY:
    case OPCODE_RECORD_OFF:
    case OPCODE_RECORD_TV_SCREEN:
    case OPCODE_GET_CEC_VERSION:
    case OPCODE_GIVE_PHYSICAL_ADDRESS:
    case OPCODE_GET_MENU_LANGUAGE:
    case OPCODE_TUNER_STEP_DECREMENT:
    case OPCODE_TUNER_STEP_INCREMENT:
    case OPCODE_GIVE_DEVICE_VENDOR_ID:
    case OPCODE_VENDOR_REMOTE_BUTTON_UP:
    case OPCODE_GIVE_OSD_NAME:
    case OPCODE_USER_CONTROL_RELEASED:
    case OPCODE_GIVE_DEVICE_POWER_STATUS:
    case OPCODE_ABORT:
    case OPCODE_GIVE_AUDIO_STATUS:
    case OPCODE_GIVE_SYSTEM_AUDIO_MODE_STATUS:
        if (size != 2)
        {
            i1ret = FALSE;
        }
        break;

    case OPCODE_SYSTEM_AUDIO_MODE_REQUEST:
        if ((size != 2) && (size != 4))
        {
            i1ret = FALSE;
        }
        break;

    // length == 3
    case OPCODE_RECORD_STATUS:
    case OPCODE_TIMER_CLEARED_STATUS:
    case OPCODE_CEC_VERSION:
    case OPCODE_DECK_CONTROL:
    case OPCODE_DECK_STATUS:
    case OPCODE_GIVE_DECK_STATUS:
    case OPCODE_PLAY:
    case OPCODE_GIVE_TUNER_DEVICE_STATUS:
    case OPCODE_MENU_REQUEST:
    case OPCODE_MENU_STATUS:
    case OPCODE_REPORT_POWER_STATUS:
    case OPCODE_REPORT_AUDIO_STATUS:
    case OPCODE_SET_SYSTEM_AUDIO_MODE:
    case OPCODE_SYSTEM_AUDIO_MODE_STATUS:
    case OPCODE_SET_AUDIO_RATE:
        if (size != 3)
        {
            i1ret = FALSE;
        }
        break;

    case OPCODE_USER_CONTROL_PRESSED:
        if ((size != 3) && (size != 4))
        {
            i1ret = FALSE;
        }
        break;

    // length == 4
    case OPCODE_ACTIVE_SOURCE:
    case OPCODE_INACTIVE_SOURCE:
    case OPCODE_ROUTING_INFORMATION:
    case OPCODE_SET_STREAM_PATH:
    case OPCODE_FEATURE_ABORT:
        if (size != 4)
        {
            i1ret = FALSE;
        }
        break;

    // length == 5
    case OPCODE_REPORT_PHYSICAL_ADDRESS:
    case OPCODE_SET_MENU_LANGUAGE:
    case OPCODE_DEVICE_VENDOR_ID:
        if (size != 5)
        {
            i1ret = FALSE;
        }
        break;

    // length == 6
    case OPCODE_ROUTING_CHANGE:
    case OPCODE_SELECT_ANALOGUE_SERVICE:
        if (size != 6)
        {
            i1ret = FALSE;
        }
        break;

    // length == 9
    case OPCODE_SELECT_DIGITAL_SERVICE:
        if (size != 9)
        {
            i1ret = FALSE;
        }
        break;

    // length == 13
    case OPCODE_CLEAR_ANALOGUE_TIMER:
    case OPCODE_SET_ANALOGUE_TIMER:
        if (size != 13)
        {
            i1ret = FALSE;
        }
        break;

    // length == 16
    case OPCODE_CLEAR_DIGITAL_TIMER:
    case OPCODE_SET_DIGITAL_TIMER:
        if (size != 16)
        {
            i1ret = FALSE;
        }
        break;

    case OPCODE_RECORD_ON:
        if ((size < 3) || (size > 10))
        {
            i1ret = FALSE;
        }
        break;

    // length == 10 ~ 11
    case OPCODE_CLEAR_EXTERNAL_TIMER:
    case OPCODE_SET_EXTERNAL_TIMER:
        if ((size < 10) || (size > 11))
        {
            i1ret = FALSE;
        }
        break;

    case OPCODE_TIMER_STATUS:
        if ((size != 3) && (size != 5))
        {
            i1ret = FALSE;
        }
        break;

    case OPCODE_TUNER_DEVICE_STATUS:
        if ((size != 7) && (size != 10))
        {
            i1ret = FALSE;
        }
        break;

    case OPCODE_VENDOR_COMMAND:
    case OPCODE_VENDOR_COMMAND_WITH_ID:
    case OPCODE_VENDOR_REMOTE_BUTTON_DOWN:
        if (size > 16)
        {
            i1ret = FALSE;
        }
        break;

    case OPCODE_SET_OSD_STRING:
        if ((size < 3) || (size > 16))
        {
            i1ret = FALSE;
        }
        break;

    case OPCODE_SET_TIMER_PROGRAM_TITLE:
    case OPCODE_SET_OSD_NAME:
        if ((size < 3) || (size > 16))
        {
            i1ret = FALSE;
        }
        break;

    case OPCODE_CDC_MESSAGE:
        i1ret = TRUE;
        break;

     default:
        i1ret = FALSE;
        break;
    }

    if (i1ret == FALSE)
    {

#if CEC_log

        LogS("receive invalid frame:");

        LogB(frame->blocks.opcode);
        _Cec_PrintFrame(frame);
#endif

    }

    return i1ret;
}

/*
    Customization rx frame handling behavior.
*/
#if (CEC_FEATURE_2 == 1)
/*
    Sony.
*/
static void _Cec_CustRxFrame(CEC_FRAME_DESCRIPTION *frame)
{
    switch (frame->blocks.opcode)
    {
    case OPCODE_ABORT:
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            rCECTxFrame.size = 4;

            rCECTxFrame.sendidx = 0;
            rCECTxFrame.reTXcnt = 0;
            rCECTxFrame.txtag = NULL;
            rCECTxFrame.blocks.header.destination = frame->blocks.header.initiator;
            rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
            rCECTxFrame.blocks.opcode = OPCODE_FEATURE_ABORT;
            rCECTxFrame.blocks.operand[0] = OPCODE_ABORT;
            rCECTxFrame.blocks.operand[1] = REFUSED;
            _Cec_EnqueueTx(&rCECTxFrame);
        }
        break;

    case OPCODE_GIVE_PHYSICAL_ADDRESS:
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            rCECTxFrame.size = 5;

            rCECTxFrame.sendidx = 0;
            rCECTxFrame.reTXcnt = 0;
            rCECTxFrame.txtag = NULL;
            rCECTxFrame.blocks.header.destination = CEC_LOG_BROADCAST;
            rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
            rCECTxFrame.blocks.opcode = OPCODE_REPORT_PHYSICAL_ADDRESS;
            rCECTxFrame.blocks.operand[0] = 0;
            rCECTxFrame.blocks.operand[1] = 0;
            rCECTxFrame.blocks.operand[2] = 0;
            _Cec_EnqueueTx(&rCECTxFrame);
        }
        break;

    case OPCODE_GIVE_DEVICE_POWER_STATUS:
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            rCECTxFrame.size = 3;

            rCECTxFrame.sendidx = 0;
            rCECTxFrame.reTXcnt = 0;
            rCECTxFrame.txtag = NULL;
            rCECTxFrame.blocks.header.destination = frame->blocks.header.initiator;
            rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
            rCECTxFrame.blocks.opcode = OPCODE_REPORT_POWER_STATUS;

            if (fgCecStandbyToOn == 1)
            {
                rCECTxFrame.blocks.operand[0] = 0x02; //In transition standby to On
            }
            else
            {
                rCECTxFrame.blocks.operand[0] = 0x01; //standby
            }
            _Cec_EnqueueTx(&rCECTxFrame);
        }
        break;

    case OPCODE_GIVE_DEVICE_VENDOR_ID:
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            rCECTxFrame.size = 5;

            rCECTxFrame.sendidx = 0;
            rCECTxFrame.reTXcnt = 0;
            rCECTxFrame.txtag = NULL;
            rCECTxFrame.blocks.header.destination = 0x0f;
            rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
            rCECTxFrame.blocks.opcode = OPCODE_DEVICE_VENDOR_ID;
            // 24bits vendor id: Sony.
            u1CecVendorID[0] = 0x08;
            u1CecVendorID[1] = 0x00;
            u1CecVendorID[2] = 0x46;                
            rCECTxFrame.blocks.operand[0] = u1CecVendorID[0];
            rCECTxFrame.blocks.operand[1] = u1CecVendorID[1];
            rCECTxFrame.blocks.operand[2] = u1CecVendorID[2];
            _Cec_EnqueueTx(&rCECTxFrame);
        }
        break;

    case OPCODE_GET_CEC_VERSION:
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            rCECTxFrame.size = 3;

            rCECTxFrame.sendidx = 0;
            rCECTxFrame.reTXcnt = 0;
            rCECTxFrame.txtag = NULL;
            rCECTxFrame.blocks.header.destination = frame->blocks.header.initiator;
            rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
            rCECTxFrame.blocks.opcode = OPCODE_CEC_VERSION;
            rCECTxFrame.blocks.operand[0] = u1CecVersion;
            _Cec_EnqueueTx(&rCECTxFrame);
        }
        break;

    case OPCODE_GIVE_OSD_NAME:
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            rCECTxFrame.size = 4;

            rCECTxFrame.sendidx = 0;
            rCECTxFrame.reTXcnt = 0;
            rCECTxFrame.txtag = NULL;
            rCECTxFrame.blocks.header.destination = frame->blocks.header.initiator;
            rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
            rCECTxFrame.blocks.opcode = OPCODE_SET_OSD_NAME;
            rCECTxFrame.blocks.operand[0] = 0x54; // 'T'
            rCECTxFrame.blocks.operand[1] = 0x56; // 'V'
            _Cec_EnqueueTx(&rCECTxFrame);
        }
        break;

    case OPCODE_INACTIVE_SOURCE:
        if ((u1ActiveSrcStoreIdx != 0xFF) && (frame->blocks.header.initiator == u1ActiveSrcLA))
        {
            memset(&(CEC_pending_msg[u1ActiveSrcStoreIdx]), 0, sizeof(CEC_FRAME_DESCRIPTION));
            u1ActiveSrcLA = 0;
        }
        break;

    case OPCODE_USER_CONTROL_PRESSED:
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            if ((frame->blocks.operand[0] == 0x40) || 
                (frame->blocks.operand[0] == 0x6B) || 
                (frame->blocks.operand[0] == 0x6D))
            {
                if ((fgCecStandbyToOn == 0) && (fgCecEnOneTouchPlay == 1))
                {
#if CEC_log

                    LogS("USER_CONTROL_PRESSED Wake Up.\n");
#endif

                    vWakeUpSystem(PDWNC_WAKE_UP_REASON_HDMI, 0);
                }
            }
            else
            {
                rCECTxFrame.size = 4;

                rCECTxFrame.sendidx = 0;
                rCECTxFrame.reTXcnt = 0;
                rCECTxFrame.txtag = NULL;
                rCECTxFrame.blocks.header.destination = frame->blocks.header.initiator;
                rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
                rCECTxFrame.blocks.opcode = OPCODE_FEATURE_ABORT;
                rCECTxFrame.blocks.operand[0] = frame->blocks.opcode;
                rCECTxFrame.blocks.operand[1] = REFUSED;
                _Cec_EnqueueTx(&rCECTxFrame);
            }
        }
        break;

    //---------- Buffer --------------------------------
    case OPCODE_IMAGE_VIEW_ON:
    case OPCODE_TEXT_VIEW_ON:
        if ((fgCecStandbyToOn == 0) && (fgCecEnOneTouchPlay == 1))
        {
#if CEC_log

            LogS("IMAGE/TEXT_VIEW_ON Wake Up.\n");
#endif

            vWakeUpSystem(PDWNC_WAKE_UP_REASON_HDMI, 0);
        }

        if (u1ViewOnStoreIdx == 0xFF)
        {
            u1ViewOnStoreIdx = u1CecPendingMsgIdx;
            u1CecPendingMsgIdx ++;                                
        }                        
#if CEC_log

            LogS("IMAGE/TEXT_VIEW_ON store.\n");
#endif
        memcpy(&(CEC_pending_msg[u1ViewOnStoreIdx]), frame, sizeof(CEC_FRAME_DESCRIPTION));

        if (u1ActiveSrcStoreIdx != 0xFF)
        {
            memset(&(CEC_pending_msg[u1ActiveSrcStoreIdx]), 0, sizeof(CEC_FRAME_DESCRIPTION));
            u1ActiveSrcLA = 0;
        }
        break;

    case OPCODE_ACTIVE_SOURCE:
        if ((frame->blocks.header.destination == CEC_LOG_BROADCAST) && 
             (fgCecEnOneTouchPlay == 1)) //Buffer Only when enable One Touch Play
        {
            if (u1ActiveSrcStoreIdx == 0xFF)
            {
                // Get active source index, and use this as fixed active source index.
                u1ActiveSrcStoreIdx = u1CecPendingMsgIdx;
                u1CecPendingMsgIdx ++;
            }

#if CEC_log

            LogS("fgStoreActiveSrc save.\n");
#endif

            // Always use u1ActiveSrcStoreIdx to be the fixed active store index.
            memcpy(&(CEC_pending_msg[u1ActiveSrcStoreIdx]), frame, sizeof(CEC_FRAME_DESCRIPTION));
            u1ActiveSrcLA = frame->blocks.header.initiator;
            
            if (u1RoutingStoreIdx != 0xFF)
            {
                memset(&(CEC_pending_msg[u1RoutingStoreIdx]), 0, sizeof(CEC_FRAME_DESCRIPTION));
            }
        }
        break;
    
    case OPCODE_ROUTING_CHANGE:
    case OPCODE_ROUTING_INFORMATION:
        if ((frame->blocks.header.destination == CEC_LOG_BROADCAST) && 
             (fgCecEnOneTouchPlay == 1)) //Buffer Only when enable One Touch Play
        {
            if (u1RoutingStoreIdx == 0xFF)
            {
                // Get store routing index, and use this as fixed store routing index.                
                u1RoutingStoreIdx = u1CecPendingMsgIdx;
                u1CecPendingMsgIdx ++;                
            }

            memcpy(&(CEC_pending_msg[u1RoutingStoreIdx]), frame, sizeof(CEC_FRAME_DESCRIPTION));
        }
        break;

    //---------- don't do anything --------------------
    case OPCODE_REQUEST_ACTIVE_SOURCE:
    case OPCODE_SET_STREAM_PATH:
    case OPCODE_STANDBY:
    case OPCODE_REPORT_PHYSICAL_ADDRESS:
    case OPCODE_SET_MENU_LANGUAGE:
    case OPCODE_DEVICE_VENDOR_ID:
    case OPCODE_USER_CONTROL_RELEASED:
    case OPCODE_FEATURE_ABORT:
    case OPCODE_SYSTEM_AUDIO_MODE_STATUS:
    case OPCODE_REPORT_AUDIO_STATUS:
        break;

    //----- <Feature Abort> with unrecognized opcode ----
    case OPCODE_RECORD_OFF:
    case OPCODE_RECORD_ON:
    case OPCODE_RECORD_STATUS:
    case OPCODE_RECORD_TV_SCREEN:
    case OPCODE_DECK_CONTROL:
    case OPCODE_DECK_STATUS:
    case OPCODE_GIVE_DECK_STATUS:
    case OPCODE_PLAY:
    case OPCODE_GIVE_TUNER_DEVICE_STATUS:
    case OPCODE_SELECT_DIGITAL_SERVICE:
    case OPCODE_TUNER_DEVICE_STATUS:
    case OPCODE_TUNER_STEP_DECREMENT:
    case OPCODE_TUNER_STEP_INCREMENT:
    case OPCODE_VENDOR_COMMAND:
    case OPCODE_VENDOR_REMOTE_BUTTON_DOWN:
    case OPCODE_VENDOR_REMOTE_BUTTON_UP:
    case OPCODE_SET_OSD_STRING:
    case OPCODE_MENU_REQUEST:
    case OPCODE_MENU_STATUS:
    case OPCODE_SET_ANALOGUE_TIMER:
    case OPCODE_CLEAR_ANALOGUE_TIMER:
    case OPCODE_SET_DIGITAL_TIMER:
    case OPCODE_CLEAR_DIGITAL_TIMER:
    case OPCODE_SET_EXTERNAL_TIMER:
    case OPCODE_CLEAR_EXTERNAL_TIMER:
    case OPCODE_SET_TIMER_PROGRAM_TITLE:
    case OPCODE_TIMER_CLEARED_STATUS:
    case OPCODE_TIMER_STATUS:
    case OPCODE_SELECT_ANALOGUE_SERVICE:
    case OPCODE_GIVE_SYSTEM_AUDIO_MODE_STATUS:
    case OPCODE_SYSTEM_AUDIO_MODE_REQUEST:
    case OPCODE_GIVE_AUDIO_STATUS:
    case OPCODE_SET_AUDIO_RATE:
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            rCECTxFrame.size = 4;

            rCECTxFrame.sendidx = 0;
            rCECTxFrame.reTXcnt = 0;
            rCECTxFrame.txtag = NULL;
            rCECTxFrame.blocks.header.destination = frame->blocks.header.initiator;
            rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
            rCECTxFrame.blocks.opcode = OPCODE_FEATURE_ABORT;
            rCECTxFrame.blocks.operand[0] = frame->blocks.opcode;
            rCECTxFrame.blocks.operand[1] = UNRECOGNIZED_OPCODE; //unrecognized opcode
            _Cec_EnqueueTx(&rCECTxFrame);
        }
        break;

    //----- <Feature Abort> with refused ----
    case OPCODE_GET_MENU_LANGUAGE:
    case OPCODE_SET_OSD_NAME:
    case OPCODE_VENDOR_COMMAND_WITH_ID:
    case OPCODE_SET_SYSTEM_AUDIO_MODE:
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            rCECTxFrame.size = 4;
            rCECTxFrame.sendidx = 0;
            rCECTxFrame.reTXcnt = 0;
            rCECTxFrame.txtag = NULL;
            rCECTxFrame.blocks.header.destination = frame->blocks.header.initiator;
            rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
            rCECTxFrame.blocks.opcode = OPCODE_FEATURE_ABORT;
            rCECTxFrame.blocks.operand[0] = frame->blocks.opcode;
            rCECTxFrame.blocks.operand[1] = NOT_IN_CORRECT_MODE_TO_RESPONSE; //spec.change from Refused
            _Cec_EnqueueTx(&rCECTxFrame);
            
            if ((frame->blocks.opcode == OPCODE_GET_MENU_LANGUAGE) && 
                (fgCecEnOneTouchPlay == 1))                               //Buffer Only when enable One Touch Play
            {
                if (u1GetMenuLanguageStoreIdx == 0xFF)
                {
                    u1GetMenuLanguageStoreIdx = u1CecPendingMsgIdx;
                    u1CecPendingMsgIdx ++;
                }
                memcpy(&(CEC_pending_msg[u1GetMenuLanguageStoreIdx]), 
                    frame, sizeof(CEC_FRAME_DESCRIPTION));            
            }
        } 
        break;

    default:
        break;
    }    
}

#elif (CEC_FEATURE_3 == 1)  
/*
    Philips.
*/
void _Cec_CustRxFrame(CEC_FRAME_DESCRIPTION *frame)
{
#ifdef CEC_FEATURE_3_1
    UINT8 i;
    UINT8 fgWkpOpcode = 0;
#endif

    fgStoreMessage = 0;

#ifdef CEC_FEATURE_3_1

    for (i = 0; i < _rCECWkpOpCode.u1Size; i++)
    {
        if (frame->blocks.opcode == _rCECWkpOpCode.au1WkpCode[i])
        {
            fgWkpOpcode = 1;
            break;
        }
    }

    if (fgWkpOpcode == 1) //Wakeup OP Code
    {
        if ((fgCecStandbyToOn == 0) && (fgCecEnOneTouchPlay == 1))
        {
#if CEC_log

            LogS("OP code Wake Up.\n");
#endif

            vWakeUpSystem(PDWNC_WAKE_UP_REASON_HDMI, 0);
        }
    }
    else //not Wakeup OP Code
#endif /* CEC_FEATURE_3_1 */

    switch (frame->blocks.opcode)
    {
    case OPCODE_ABORT:
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            rCECTxFrame.size = 4;

            rCECTxFrame.sendidx = 0;
            rCECTxFrame.reTXcnt = 0;
            rCECTxFrame.txtag = NULL;
            rCECTxFrame.blocks.header.destination = frame->blocks.header.initiator;
            rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
            rCECTxFrame.blocks.opcode = OPCODE_FEATURE_ABORT;
            rCECTxFrame.blocks.operand[0] = OPCODE_ABORT;
            rCECTxFrame.blocks.operand[1] = REFUSED;
            _Cec_EnqueueTx(&rCECTxFrame);
        }
        break;

    case OPCODE_GIVE_PHYSICAL_ADDRESS:
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            rCECTxFrame.size = 5;

            rCECTxFrame.sendidx = 0;
            rCECTxFrame.reTXcnt = 0;
            rCECTxFrame.txtag = NULL;
            rCECTxFrame.blocks.header.destination = CEC_LOG_BROADCAST;
            rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
            rCECTxFrame.blocks.opcode = OPCODE_REPORT_PHYSICAL_ADDRESS;
            rCECTxFrame.blocks.operand[0] = 0;
            rCECTxFrame.blocks.operand[1] = 0;
            rCECTxFrame.blocks.operand[2] = 0;
            _Cec_EnqueueTx(&rCECTxFrame);
        }
        break;

    case OPCODE_IMAGE_VIEW_ON:
    case OPCODE_TEXT_VIEW_ON:
        if ((fgCecStandbyToOn == 0) && (fgCecEnOneTouchPlay == 1))
        {
#if CEC_log

            LogS("IMAGE/TEXT_VIEW_ON Wake Up.\n");
#endif

            vWakeUpSystem(PDWNC_WAKE_UP_REASON_HDMI, 0);
        }

        fgStoreMessage = 1;
        break;

    case OPCODE_ACTIVE_SOURCE:
        fgStoreMessage = 1;
        break;

    case OPCODE_GIVE_DEVICE_POWER_STATUS:
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            rCECTxFrame.size = 3;

            rCECTxFrame.sendidx = 0;
            rCECTxFrame.reTXcnt = 0;
            rCECTxFrame.txtag = NULL;
            rCECTxFrame.blocks.header.destination = frame->blocks.header.initiator;
            rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
            rCECTxFrame.blocks.opcode = OPCODE_REPORT_POWER_STATUS;

            if (fgCecStandbyToOn == 1)
            {
                rCECTxFrame.blocks.operand[0] = 0x02; //In transition standby to On
            }
            else
            {
                rCECTxFrame.blocks.operand[0] = 0x01; //standby
            }
            _Cec_EnqueueTx(&rCECTxFrame);
        }
        break;

    case OPCODE_GIVE_DEVICE_VENDOR_ID:
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            rCECTxFrame.size = 5;

            rCECTxFrame.sendidx = 0;
            rCECTxFrame.reTXcnt = 0;
            rCECTxFrame.txtag = NULL;
            rCECTxFrame.blocks.header.destination = 0x0f;
            rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
            rCECTxFrame.blocks.opcode = OPCODE_DEVICE_VENDOR_ID;
            // 24bits vendor id: Philips.
            u1CecVendorID[0] = 0x00;
            u1CecVendorID[1] = 0x90;
            u1CecVendorID[2] = 0x3E;                
            rCECTxFrame.blocks.operand[0] = u1CecVendorID[0];
            rCECTxFrame.blocks.operand[1] = u1CecVendorID[1];
            rCECTxFrame.blocks.operand[2] = u1CecVendorID[2];
            _Cec_EnqueueTx(&rCECTxFrame);
        }
        break;

    case OPCODE_GET_CEC_VERSION:
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            rCECTxFrame.size = 3;

            rCECTxFrame.sendidx = 0;
            rCECTxFrame.reTXcnt = 0;
            rCECTxFrame.txtag = NULL;
            rCECTxFrame.blocks.header.destination = frame->blocks.header.initiator;
            rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
            rCECTxFrame.blocks.opcode = OPCODE_CEC_VERSION;
            rCECTxFrame.blocks.operand[0] = u1CecVersion;
            _Cec_EnqueueTx(&rCECTxFrame);
        }
        break;

    case OPCODE_GET_MENU_LANGUAGE:
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            rCECTxFrame.size = 5;

            rCECTxFrame.sendidx = 0;
            rCECTxFrame.reTXcnt = 0;
            rCECTxFrame.txtag = NULL;
            rCECTxFrame.blocks.header.destination = 0x0f;
            rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
            rCECTxFrame.blocks.opcode = OPCODE_SET_MENU_LANGUAGE;
            rCECTxFrame.blocks.operand[0] = u1CecMenuLanguage[0];
            rCECTxFrame.blocks.operand[1] = u1CecMenuLanguage[1];
            rCECTxFrame.blocks.operand[2] = u1CecMenuLanguage[2];
            _Cec_EnqueueTx(&rCECTxFrame);
        }
        break;

#ifdef CEC_FEATURE_3_1

    case OPCODE_GIVE_OSD_NAME:
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            rCECTxFrame.size = _rCECOsdName.u1Size + 2;

            rCECTxFrame.sendidx = 0;
            rCECTxFrame.reTXcnt = 0;
            rCECTxFrame.txtag = NULL;
            rCECTxFrame.blocks.header.destination = frame->blocks.header.initiator;
            rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
            rCECTxFrame.blocks.opcode = OPCODE_SET_OSD_NAME;

            for (i = 0; i < _rCECOsdName.u1Size; i++)
            {
                rCECTxFrame.blocks.operand[i] = _rCECOsdName.au1OsdName[i];
            }
            _Cec_EnqueueTx(&rCECTxFrame);
        }
        break;
#endif

    default:
        if ((fgCecStandbyToOn == 0) && (frame->blocks.header.destination != CEC_LOG_BROADCAST))
        {
            rCECTxFrame.size = 4;

            rCECTxFrame.sendidx = 0;
            rCECTxFrame.reTXcnt = 0;
            rCECTxFrame.txtag = NULL;
            rCECTxFrame.blocks.header.destination = frame->blocks.header.initiator;
            rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
            rCECTxFrame.blocks.opcode = OPCODE_FEATURE_ABORT;
            rCECTxFrame.blocks.operand[0] = OPCODE_ABORT;

            if (frame->blocks.opcode == OPCODE_GIVE_SYSTEM_AUDIO_MODE_STATUS)
            {
                rCECTxFrame.blocks.operand[1] = UNRECOGNIZED_OPCODE; //unrecognized opcode
            }
            else
            {
                rCECTxFrame.blocks.operand[1] = NOT_IN_CORRECT_MODE_TO_RESPONSE; //not in correct mode to response
            }
            _Cec_EnqueueTx(&rCECTxFrame);
        }
        break;
    }

    if ((fgCecStandbyToOn == 1) && (fgStoreMessage == 1))
    {
        if (u1CecPendingMsgIdx < PENDING_MSG_SIZE)
        {
#if CEC_log

            LogS("Pending message\n");
            Log4B(u1CecPendingMsgIdx, 0x00, 0x00, 0x00);
#endif

            memcpy(&(CEC_pending_msg[u1CecPendingMsgIdx]), frame, sizeof(CEC_FRAME_DESCRIPTION));
            u1CecPendingMsgIdx ++;
        }
    }
}
#else
/*
    MediaTek.
*/
void _Cec_CustRxFrame(CEC_FRAME_DESCRIPTION *frame)
{
    switch (frame->blocks.opcode)
    {
    case OPCODE_ABORT:
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            rCECTxFrame.size = 4;

            rCECTxFrame.sendidx = 0;
            rCECTxFrame.reTXcnt = 0;
            rCECTxFrame.txtag = NULL;
            rCECTxFrame.blocks.header.destination = frame->blocks.header.initiator;
            rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
            rCECTxFrame.blocks.opcode = OPCODE_FEATURE_ABORT;
            rCECTxFrame.blocks.operand[0] = OPCODE_ABORT;
            rCECTxFrame.blocks.operand[1] = REFUSED;
            _Cec_EnqueueTx(&rCECTxFrame);
        }
        break;

    case OPCODE_GIVE_PHYSICAL_ADDRESS:
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            rCECTxFrame.size = 5;

            rCECTxFrame.sendidx = 0;
            rCECTxFrame.reTXcnt = 0;
            rCECTxFrame.txtag = NULL;
            rCECTxFrame.blocks.header.destination = CEC_LOG_BROADCAST;
            rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
            rCECTxFrame.blocks.opcode = OPCODE_REPORT_PHYSICAL_ADDRESS;
            rCECTxFrame.blocks.operand[0] = 0;
            rCECTxFrame.blocks.operand[1] = 0;
            rCECTxFrame.blocks.operand[2] = 0;
            _Cec_EnqueueTx(&rCECTxFrame);
        }
        break;

    case OPCODE_GIVE_DEVICE_POWER_STATUS:
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            rCECTxFrame.size = 3;

            rCECTxFrame.sendidx = 0;
            rCECTxFrame.reTXcnt = 0;
            rCECTxFrame.txtag = NULL;
            rCECTxFrame.blocks.header.destination = frame->blocks.header.initiator;
            rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
            rCECTxFrame.blocks.opcode = OPCODE_REPORT_POWER_STATUS;

            if (fgCecStandbyToOn == 1)
            {
                rCECTxFrame.blocks.operand[0] = 0x02; //In transition standby to On
            }
            else
            {
                rCECTxFrame.blocks.operand[0] = 0x01; //standby
            }
            _Cec_EnqueueTx(&rCECTxFrame);
        }
        break;

    case OPCODE_GIVE_DEVICE_VENDOR_ID:
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            rCECTxFrame.size = 5;

            rCECTxFrame.sendidx = 0;
            rCECTxFrame.reTXcnt = 0;
            rCECTxFrame.txtag = NULL;
            rCECTxFrame.blocks.header.destination = 0x0f;
            rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
            rCECTxFrame.blocks.opcode = OPCODE_DEVICE_VENDOR_ID;

            #ifdef TOSHIBA_VENDOR_ID
            // 24bits vendor id: Toshiba.
            u1CecVendorID[0] = 0x00; 
            u1CecVendorID[1] = 0x00;
            u1CecVendorID[2] = 0x39;
            #else
            u1CecVendorID[0] = 0x4D;   // 'M'
            u1CecVendorID[1] = 0x54;    // 'T'
            u1CecVendorID[2] = 0x4B;    // 'K'     
            #endif
            
            rCECTxFrame.blocks.operand[0] = u1CecVendorID[0];
            rCECTxFrame.blocks.operand[1] = u1CecVendorID[1];
            rCECTxFrame.blocks.operand[2] = u1CecVendorID[2];
            _Cec_EnqueueTx(&rCECTxFrame);
        }
        break;

    case OPCODE_GET_CEC_VERSION:
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            rCECTxFrame.size = 3;

            rCECTxFrame.sendidx = 0;
            rCECTxFrame.reTXcnt = 0;
            rCECTxFrame.txtag = NULL;
            rCECTxFrame.blocks.header.destination = frame->blocks.header.initiator;
            rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
            rCECTxFrame.blocks.opcode = OPCODE_CEC_VERSION;
            rCECTxFrame.blocks.operand[0] = u1CecVersion;
            _Cec_EnqueueTx(&rCECTxFrame);
        }
        break;

    case OPCODE_GIVE_OSD_NAME:
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            rCECTxFrame.size = 4;

            rCECTxFrame.sendidx = 0;
            rCECTxFrame.reTXcnt = 0;
            rCECTxFrame.txtag = NULL;
            rCECTxFrame.blocks.header.destination = frame->blocks.header.initiator;
            rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
            rCECTxFrame.blocks.opcode = OPCODE_SET_OSD_NAME;
            rCECTxFrame.blocks.operand[0] = 0x54; //T
            rCECTxFrame.blocks.operand[1] = 0x56; //V
            _Cec_EnqueueTx(&rCECTxFrame);
        }
        break;

    case OPCODE_INACTIVE_SOURCE:
        if ((u1ActiveSrcStoreIdx != 0xFF) && (frame->blocks.header.initiator == u1ActiveSrcLA))
        {
            memset(&(CEC_pending_msg[u1ActiveSrcStoreIdx]), 0, sizeof(CEC_FRAME_DESCRIPTION));
            u1ActiveSrcLA = 0;
        }
        break;

    case OPCODE_USER_CONTROL_PRESSED:
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            if ((frame->blocks.operand[0] == 0x40) || 
                (frame->blocks.operand[0] == 0x6B) || 
                (frame->blocks.operand[0] == 0x6D))
            {
                if ((fgCecStandbyToOn == 0) && (fgCecEnOneTouchPlay == 1))
                {
#if CEC_log

                    LogS("USER_CONTROL_PRESSED Wake Up.\n");
#endif

                    vWakeUpSystem(PDWNC_WAKE_UP_REASON_HDMI, 0);
                }
            }
            else
            {
                rCECTxFrame.size = 4;

                rCECTxFrame.sendidx = 0;
                rCECTxFrame.reTXcnt = 0;
                rCECTxFrame.txtag = NULL;
                rCECTxFrame.blocks.header.destination = frame->blocks.header.initiator;
                rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
                rCECTxFrame.blocks.opcode = OPCODE_FEATURE_ABORT;
                rCECTxFrame.blocks.operand[0] = frame->blocks.opcode;
                rCECTxFrame.blocks.operand[1] = REFUSED;
                _Cec_EnqueueTx(&rCECTxFrame);
            }
        }
        break;

    //---------- Buffer --------------------------------
    case OPCODE_IMAGE_VIEW_ON:
    case OPCODE_TEXT_VIEW_ON:
        if ((fgCecStandbyToOn == 0) && (fgCecEnOneTouchPlay == 1))
        {
#if CEC_log

            LogS("IMAGE/TEXT_VIEW_ON Wake Up.\n");
#endif
            vWakeUpSystem(PDWNC_WAKE_UP_REASON_HDMI, 0);
        }            

        if (u1ViewOnStoreIdx == 0xFF)
        {
            u1ViewOnStoreIdx = u1CecPendingMsgIdx;
            u1CecPendingMsgIdx ++;                                
        }            
#if CEC_log

            LogS("IMAGE/TEXT_VIEW_ON store.\n");
#endif        
        memcpy(&(CEC_pending_msg[u1ViewOnStoreIdx]), frame, sizeof(CEC_FRAME_DESCRIPTION));

        if (u1ActiveSrcStoreIdx != 0xFF)
        {
            memset(&(CEC_pending_msg[u1ActiveSrcStoreIdx]), 0, sizeof(CEC_FRAME_DESCRIPTION));
            u1ActiveSrcLA = 0;
        }
        break;

    case OPCODE_ACTIVE_SOURCE:
        if ((frame->blocks.header.destination == CEC_LOG_BROADCAST) && 
             (fgCecEnOneTouchPlay == 1)) //Buffer Only when enable One Touch Play
        {
            if (u1ActiveSrcStoreIdx == 0xFF)
            {
#if CEC_log

            LogS("fgStoreActiveSrc = 1.\n");
#endif
                u1ActiveSrcStoreIdx = u1CecPendingMsgIdx;
                u1CecPendingMsgIdx ++;                                
            }

#if CEC_log

            LogS("fgStoreActiveSrc store.\n");
#endif
            memcpy(&(CEC_pending_msg[u1ActiveSrcStoreIdx]), frame, sizeof(CEC_FRAME_DESCRIPTION));

            u1ActiveSrcLA = frame->blocks.header.initiator;
            if (u1RoutingStoreIdx != 0xFF)
            {
                memset(&(CEC_pending_msg[u1RoutingStoreIdx]), 0, sizeof(CEC_FRAME_DESCRIPTION));
            }
        }
        break;
    
    case OPCODE_ROUTING_CHANGE:
    case OPCODE_ROUTING_INFORMATION:
        if ((frame->blocks.header.destination == CEC_LOG_BROADCAST) && 
             (fgCecEnOneTouchPlay == 1)) //Buffer Only when enable One Touch Play
        {
            if (u1RoutingStoreIdx == 0xFF)
            {
                u1RoutingStoreIdx = u1CecPendingMsgIdx;
                u1CecPendingMsgIdx ++;
            }

            memcpy(&(CEC_pending_msg[u1RoutingStoreIdx]), frame, sizeof(CEC_FRAME_DESCRIPTION));
            u1ActiveSrcLA = frame->blocks.header.initiator;
            if (u1ActiveSrcStoreIdx != 0xFF)
            {
                memset(&(CEC_pending_msg[u1ActiveSrcStoreIdx]), 0, sizeof(CEC_FRAME_DESCRIPTION));
            }            
        }
        break;

    //---------- don't do anything --------------------
    case OPCODE_REQUEST_ACTIVE_SOURCE:
    case OPCODE_SET_STREAM_PATH:
    case OPCODE_STANDBY:
    case OPCODE_REPORT_PHYSICAL_ADDRESS:
    case OPCODE_SET_MENU_LANGUAGE:
    case OPCODE_DEVICE_VENDOR_ID:
    case OPCODE_USER_CONTROL_RELEASED:
    case OPCODE_FEATURE_ABORT:
    case OPCODE_SYSTEM_AUDIO_MODE_STATUS:
    case OPCODE_REPORT_AUDIO_STATUS:
        break;

    //----- <Feature Abort> with unrecognized opcode ----
    case OPCODE_RECORD_OFF:
    case OPCODE_RECORD_ON:
    case OPCODE_RECORD_STATUS:
    case OPCODE_RECORD_TV_SCREEN:
    case OPCODE_DECK_CONTROL:
    case OPCODE_DECK_STATUS:
    case OPCODE_GIVE_DECK_STATUS:
    case OPCODE_PLAY:
    case OPCODE_GIVE_TUNER_DEVICE_STATUS:
    case OPCODE_SELECT_DIGITAL_SERVICE:
    case OPCODE_TUNER_DEVICE_STATUS:
    case OPCODE_TUNER_STEP_DECREMENT:
    case OPCODE_TUNER_STEP_INCREMENT:
    case OPCODE_VENDOR_COMMAND:
    case OPCODE_VENDOR_REMOTE_BUTTON_DOWN:
    case OPCODE_VENDOR_REMOTE_BUTTON_UP:
    case OPCODE_SET_OSD_STRING:
    case OPCODE_MENU_REQUEST:
    case OPCODE_MENU_STATUS:
    case OPCODE_SET_ANALOGUE_TIMER:
    case OPCODE_CLEAR_ANALOGUE_TIMER:
    case OPCODE_SET_DIGITAL_TIMER:
    case OPCODE_CLEAR_DIGITAL_TIMER:
    case OPCODE_SET_EXTERNAL_TIMER:
    case OPCODE_CLEAR_EXTERNAL_TIMER:
    case OPCODE_SET_TIMER_PROGRAM_TITLE:
    case OPCODE_TIMER_CLEARED_STATUS:
    case OPCODE_TIMER_STATUS:
    case OPCODE_SELECT_ANALOGUE_SERVICE:
    case OPCODE_GIVE_SYSTEM_AUDIO_MODE_STATUS:
    case OPCODE_SYSTEM_AUDIO_MODE_REQUEST:
    case OPCODE_GIVE_AUDIO_STATUS:
    case OPCODE_SET_AUDIO_RATE:
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            rCECTxFrame.size = 4;

            rCECTxFrame.sendidx = 0;
            rCECTxFrame.reTXcnt = 0;
            rCECTxFrame.txtag = NULL;
            rCECTxFrame.blocks.header.destination = frame->blocks.header.initiator;
            rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
            rCECTxFrame.blocks.opcode = OPCODE_FEATURE_ABORT;
            rCECTxFrame.blocks.operand[0] = frame->blocks.opcode;
            rCECTxFrame.blocks.operand[1] = UNRECOGNIZED_OPCODE; //unrecognized opcode
            _Cec_EnqueueTx(&rCECTxFrame);
        }
        break;

    //----- <Feature Abort> with refused ----
    case OPCODE_GET_MENU_LANGUAGE:
    case OPCODE_SET_OSD_NAME:
    case OPCODE_VENDOR_COMMAND_WITH_ID:
    case OPCODE_SET_SYSTEM_AUDIO_MODE:
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            rCECTxFrame.size = 4;
            rCECTxFrame.sendidx = 0;
            rCECTxFrame.reTXcnt = 0;
            rCECTxFrame.txtag = NULL;
            rCECTxFrame.blocks.header.destination = frame->blocks.header.initiator;
            rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
            rCECTxFrame.blocks.opcode = OPCODE_FEATURE_ABORT;
            rCECTxFrame.blocks.operand[0] = frame->blocks.opcode;
            rCECTxFrame.blocks.operand[1] = NOT_IN_CORRECT_MODE_TO_RESPONSE; //spec.change from Refused
            _Cec_EnqueueTx(&rCECTxFrame);
            
            if ((frame->blocks.opcode == OPCODE_GET_MENU_LANGUAGE) && 
                (fgCecEnOneTouchPlay == 1))                               //Buffer Only when enable One Touch Play
            {
                if (u1GetMenuLanguageStoreIdx == 0xFF)
                {
                    u1GetMenuLanguageStoreIdx = u1CecPendingMsgIdx;
                    u1CecPendingMsgIdx ++;
                }
                memcpy(&(CEC_pending_msg[u1GetMenuLanguageStoreIdx]), 
                    frame, sizeof(CEC_FRAME_DESCRIPTION));
            }
        } 
        break;

    default:
#ifdef ERROR_HANDLE
        if (frame->blocks.header.destination != CEC_LOG_BROADCAST)
        {
            rCECTxFrame.size = 4;
            rCECTxFrame.sendidx = 0;
            rCECTxFrame.reTXcnt = 0;
            rCECTxFrame.txtag = NULL;
            rCECTxFrame.blocks.header.destination = frame->blocks.header.initiator;
            rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
            rCECTxFrame.blocks.opcode = OPCODE_FEATURE_ABORT;
            rCECTxFrame.blocks.operand[0] = OPCODE_ABORT;
            rCECTxFrame.blocks.operand[1] = NOT_IN_CORRECT_MODE_TO_RESPONSE; 
            _Cec_EnqueueTx(&rCECTxFrame);
        }
#endif        
        break;
    }    
}
#endif

static void _Cec_HandleRxTask(void)
{
    CEC_FRAME_DESCRIPTION* frame;

    while (1)
    {
        frame = _Cec_GetRxReadFrame();

        if (frame == NULL)
        {            
            return;
        }

        if (_Cec_CheckFrame(frame))
        {
            #if CEC_log
            LogS("receive message\n");
            _Cec_PrintFrame(frame);
            #endif

            if (fgCecEnRx == 1)
            {
                _Cec_CustRxFrame(frame);
            }
        }

        _Cec_DequeueRx();
    }
}

static void _Cec_HandleTxResult(void)
{
    if (IsCECStatus(STATE_TXING_FRAME))
    {
        if (IsCECStatus(STATE_HW_RETX))
        {
            if ((TX_FAIL_RECORD() != 0))
            {
                DISABLE_ALL_TX_INT();

                SetCECStatus(STATE_TX_NOACK);
                ClrCECStatus(STATE_HW_RETX);

                ClrCECStatus(STATE_TXING_FRAME);

                if (IS_TX_FSM_FAIL())
                {

#if CEC_log

                    LogS("TX NO ACK\n");
#endif

                }
                else
                {

#if CEC_log

                    LogS("other TX error\n");
#endif

                }

#if CEC_log

                LogS("H ACK:");
                LogB(GET_FOLLOWER_H_ACK());
                LogS("D ACK:");
                LogB(GET_FOLLOWER_D_ACK());
#endif

                RESET_HW_TX();
            }
        }
        else if ((prActiveTxFrame->sendidx + 1) == (prActiveTxFrame->size))
        {
            if (IS_TX_FSM_IDLE() && IS_TX_FINISH())
            {
                DISABLE_ALL_TX_INT();

                SetCECStatus(STATE_TX_FRAME_SUCCESS);
                ClrCECStatus(STATE_TXING_FRAME);

#if CEC_log

                LogS("TX is COMPLETED with: H ACK:  and D ACK ");
                LogB(GET_FOLLOWER_H_ACK());
                LogB(GET_FOLLOWER_D_ACK());
#endif

            }
        }
    }
}

static void _Cec_HandleTxTask(void)
{
    CEC_FRAME_DESCRIPTION *frame;

    /* if the tx message queue is empty */
    if (IS_TX_Q_EMPTY())
    {
        return;
    }

    /* if the tx is active, check the result */
    if (IsCECStatus(STATE_TXING_FRAME))
    {
        _Cec_HandleTxResult();

        if (IsCECStatus(STATE_TX_FRAME_SUCCESS))
        {

#if CEC_log

            LogS("This message is successful\n");
#endif

            frame = _Cec_GetTxReadFrame();

            if (frame == NULL)
            {

#if CEC_log

                LogS("[CEC] Error 1! \n");
#endif

            }
            _Cec_DequeueTx();
        }
        
        if (IsCECStatus(STATE_TX_NOACK))
        {
            frame = _Cec_GetTxReadFrame();

#if CEC_log

            LogS("dedicated with no ACK:");
            LogB(frame->reTXcnt);
#endif

            if (frame == NULL)
            {

#if CEC_log

                LogS("[CEC] Error 2! \n");
#endif

            }

            frame->reTXcnt++;
            frame->sendidx = 0;

            ClrCECStatus(STATE_TX_NOACK);
            // CYJ.NOTE: retransmission
            if (frame->reTXcnt == RETX_MAX_CNT)
            {

#if CEC_log

                LogS("ReTX reach MAX");
#endif

                _Cec_DequeueTx();
            }
        }
    }
    else
    /* if the tx is not active, send the next message */
    {
        frame = _Cec_GetTxReadFrame();

        if (frame == NULL)
        {

#if CEC_log

            LogS("[CEC] Error 3! \n");
#endif

        }

#if CEC_log

        LogS("Send a new message");
        _Cec_PrintFrame(frame);
#endif

        _Cec_SendFrame(frame);
    }
}

static void _Cec_EnableInterrupt(void)
{
    MCEC_EN_INT();
    // TV_EVENT && RX_EVENT
    vRegWrite4BMsk(TX_EVENT, 0x00, 0xff);
    vRegWrite4BMsk(RX_EVENT, 0x00, 0xff);
    // RX_EVENT
    ENABLE_INT_OV(1);
    ENABLE_INT_BR_RDY(1);
    ENABLE_INT_HEADER_RDY(1);
    // TX_EVENT
    ENABLE_INT_UN(0);
    ENABLE_INT_LOW(0);
    ENABLE_INT_FAIL(0);
    ENABLE_INT_BS(0);
    ENABLE_INT_RB(0);
}
//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
#if CEC_FEATURE_3
void CEC_Report_Physic_Addr(void)
{	
    prActiveRxFrame = _Cec_GetRxWriteFrame();
    if (!prActiveRxFrame)
    {
        return;
    }

    prActiveRxFrame->size = 2;
    prActiveRxFrame->sendidx = 0;
    prActiveRxFrame->reTXcnt = 0;

    // Rx do not use txtag, use this as a sequential driver tag.    
    prActiveRxFrame->txtag = (void *)(u4CecRxTag ++);        

    prActiveRxFrame->blocks.header.initiator = CEC_LOG_PLAYBACK_DEVICE1;
    prActiveRxFrame->blocks.header.destination = u1CecTVLogicAddr;
    prActiveRxFrame->blocks.opcode = OPCODE_GIVE_PHYSICAL_ADDRESS;

    _Cec_EnqueueRx();
}

void CEC_Device_Vendor_ID(void)
{
    prActiveRxFrame = _Cec_GetRxWriteFrame();
    if (!prActiveRxFrame)
    {
        return;
    }

    prActiveRxFrame->size = 2;
    prActiveRxFrame->sendidx = 0;
    prActiveRxFrame->reTXcnt = 0;

    // Rx do not use txtag, use this as a sequential driver tag.    
    prActiveRxFrame->txtag = (void *)(u4CecRxTag ++);        

    prActiveRxFrame->blocks.header.initiator = CEC_LOG_PLAYBACK_DEVICE1;
    prActiveRxFrame->blocks.header.destination = u1CecTVLogicAddr;
    prActiveRxFrame->blocks.opcode = OPCODE_GIVE_DEVICE_VENDOR_ID;

    _Cec_EnqueueRx();
}

void CEC_System_Standby(void)
{	
    rCECTxFrame.size = 2;
    rCECTxFrame.sendidx = 0;
    rCECTxFrame.reTXcnt = 0;
    rCECTxFrame.txtag = NULL;
    rCECTxFrame.blocks.header.destination = 0x0f;
    rCECTxFrame.blocks.header.initiator = u1CecTVLogicAddr;
    rCECTxFrame.blocks.opcode = OPCODE_STANDBY;
    _Cec_EnqueueTx(&rCECTxFrame);
}
#endif /* CEC_FEATURE_3 */

void CEC_Interrupt(void)
{
    if (fgCec8032Run == 0)
    {
        // Hand over to ARM control CEC.
        MCEC_DIS_INT();    
        // 8032 do not handle CEC.
        return;
    }

#if CEC_log

    LogS("CEC ISR");
#endif

    //RX_EVENT
    if (HW_RX_HEADER_ARRIVED())
    {
        if ((GET_DST_FIELD_RECEIVING() == u1CecTVLogicAddr) || 
            (GET_DST_FIELD_RECEIVING() == CEC_LOG_BROADCAST))
        {
#if CEC_log

            LogS("RX:H\n");
#endif

            if (IsCECStatus(STATE_RX_GET_NEW_HEADER))
            {
#if CEC_log

                LogS("Lost EOM:1\n");
#endif
            }

            SetCECStatus(STATE_RX_GET_NEW_HEADER);
        }
        else
        {

#if CEC_log

            LogS("RX:H False\n");
#endif

        }
        CLR_INT_HEADER_RDY();
    }

    if (HW_RX_DATA_ARRIVED())
    {

#if CEC_log

        LogS("RX:D\n");
#endif

        _Cec_ProcessRxIsr();
    }

    if (IS_INT_OV())
    {

#if CEC_log

        LogS("Overflow\n");
#endif

        SetCECStatus(STATE_HW_RX_OVERFLOW);
        CLR_INT_OV();
    }

    //TX_EVENT
    if (IsCECStatus(STATE_TXING_FRAME))
    {
        if (IS_INT_UN())
        {

#if CEC_log

            LogS("Underrun\n");
#endif

        }

        if (IS_INT_LOW())
        {

#if CEC_log

            LogS("Buffer Low\n");
#endif

            if (!IS_INT_RB_RDY())
            {

#if CEC_log

                LogS("FW is slow to trigger the following blocks\n");
#endif

            }
        }

        if (IS_INT_RB_ENABLE() && IS_TX_DATA_TAKEN())
        {

#if CEC_log

            LogS("TX Data Taken\n");
#endif

            _Cec_SendNextDataFrame();
        }

        // CYJ.NOTE TX Failure Detection
        if (IS_INT_FAIL_ENABLE() && (TX_FAIL_RECORD() != 0))
        {
            DISABLE_ALL_TX_INT();

            SetCECStatus(STATE_HW_RETX);
            if (TX_FAIL_MAX() | IS_TX_FSM_FAIL())
            {

#if CEC_log

                LogS("TX Fail MAX\n");
#endif

            }
            else
            {

#if CEC_log

                LogS("TX Fail: \n");
                LogB(TX_FAIL_RECORD());
#endif

            }
        }

#if CEC_log

        LogS("CEC TX HW FSMl: \n");
        LogB(TX_FSM_STATUS());
#endif

    }
}

void CEC_Init(void)
{
    // Init global variable
    if (fgCec8032Ctrl == 1)
    {
        fgCec8032Run = 1;
    }
    else
    {
        fgCec8032Run = 0;
    }
    
    #ifdef EN_ONE_TOUCH_PLAY_TEST
    fgCecEnOneTouchPlay = 1;
    #endif    

    u1ActiveSrcLA = 0;
    u1ViewOnStoreIdx = 0xFF;
    u1ActiveSrcStoreIdx = 0xFF;
    u1RoutingStoreIdx = 0xFF;
    u1GetMenuLanguageStoreIdx = 0xFF;
    
    #ifdef TOSHIBA_VENDOR_ID
    // 24bits vendor id: Toshiba.
    u1CecVendorID[0] = 0x00; 
    u1CecVendorID[1] = 0x00;
    u1CecVendorID[2] = 0x39;
    #else
    u1CecVendorID[0] = 0x4D;   // 'M'
    u1CecVendorID[1] = 0x54;    // 'T'
    u1CecVendorID[2] = 0x4B;    // 'K'     
    #endif
    
    u1CecVersion = 0x04;    /*support HDMI version 1.3A*/
    
    fgCecStandbyToOn = 0;

    prActiveRxFrame = NULL;
    u4CecRxTag = 0;

    fgStoreMessage = 0;
    u1CecPendingMsgIdx = 0;

    /* Software initialization */
    _CEC_Status = 0;
    _Cec_InitQueue();

    if (MGET_SYS_PRE_STANDBY_STATUS() == STBY_FROM_AC_ON)     //From AC on, need to initialize CEC register
    {
        fgCecEnRx = 1;
    
        u1CecTVLogicAddr = CEC_LOG_TV;	//From AC on to standby	
    
        //vRegWriteFldAlign(CEC_PIN_MUX, 0, CEC_MUX);
        /* Set CEC TX/RX Timing */
#if defined(MT5387_CEC) || defined(MT5363_CEC)
        // Power down clock run at 3 Mhz.
        vRegWrite4B(CKGEN, 0x0000000f);
#else
        // Power down clock run at 27 Mhz.
        vRegWrite4B(CKGEN, 0x00000087);
#endif

        vRegWrite4B(TR_CONFIG, 0x00000001);

        vRegWrite4B(RX_T_START_R, 0x0189015b);
        vRegWrite4B(RX_T_START_F, 0x01db01ae);
        vRegWrite4B(RX_T_DATA, 0x006e00c8);
        vRegWrite4B(RX_T_ACK, 0x00000096);
        vRegWrite4B(RX_T_ERROR, 0x01680212);
        vRegWrite4B(TX_T_START, 0x01c20172);
        vRegWrite4B(TX_T_DATA_R, 0x003c0096);
        vRegWrite4B(TX_T_DATA_F, 0x00f000f0);

        vRegWriteFldAlign(TX_ARB, 6, BCNT_NEW_INIT);
        vRegWriteFldAlign(TX_ARB, 9, BCNT_NEW_MSG);
        vRegWriteFldAlign(TX_ARB, 4, BCNT_RETRANSMIT);
        vRegWriteFldAlign(TX_ARB, 0, MAX_RETRANSMIT);
        vRegWriteFldAlign(TX_ARB, 0x19, TX_INPUT_DELAY);

        FLOW_CONTROL_ACK(1);

#if defined(MT5387_CEC) || defined(MT5363_CEC)
        vRegWrite4B(TR_CONFIG, 0x0ff01101 | ((UINT32)u1CecTVLogicAddr << 16));
#else
        vRegWrite4B(TR_CONFIG, 0x0ff01151 | ((UINT32)u1CecTVLogicAddr << 16));
#endif        
    }
    else if (MGET_SYS_PRE_STANDBY_STATUS() == STBY_FROM_NORMAL_POWER)  //From normal to standby,only enable T8032 CEC interrupt
    {
        u1CecTVLogicAddr = GET_DEVICE_LA();  //From normal to standby		

        // Reset CEC.
        vRegWrite4B(TR_CONFIG, 0x00000001);

#if DISABLE_CEC_WITHOUT_ACK
        if (fgCecEnRx == 0)
        {
            vRegWrite4B(TR_CONFIG, 0x0ff00001 | ((UINT32)u1CecTVLogicAddr << 16));
        }
        else
#endif
        {
#if defined(MT5387_CEC) || defined(MT5363_CEC)
            vRegWrite4B(TR_CONFIG, 0x0ff01101 | ((UINT32)u1CecTVLogicAddr << 16));
#else
            vRegWrite4B(TR_CONFIG, 0x0ff01151 | ((UINT32)u1CecTVLogicAddr << 16));
#endif        
        }
    }

    /* register ISR and enable interrupt */
    _Cec_EnableInterrupt();
}

void CEC_Mainloop(void)
{
    if (fgCec8032Run == 0)
    {
        // Hand over to ARM control CEC.
        MCEC_DIS_INT();    
    }
    else
    {         
        MCEC_EN_INT();
        
        _Cec_HandleTxTask();

        // NOTE: the priority between tx and rx
        if (!IsCECStatus(STATE_TXING_FRAME))
        {
            _Cec_HandleRxTask();
        }

        // Check if ARM request to control CEC.
        if (fgCec8032Ctrl == 0)
        {           
            // TX: check tx queue. RX: check FSM is idle.
            if (IS_TX_Q_EMPTY() && IS_RX_FSM_IDLE())
            {            
                // Hand over to ARM control CEC.
                MCEC_DIS_INT();
                fgCec8032Run = 0;
                return;
            }       
        }        
    }
}
#endif

