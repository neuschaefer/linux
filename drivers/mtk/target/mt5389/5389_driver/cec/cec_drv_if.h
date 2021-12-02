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
 * $Date  $
 * $RCSfile: cec_drv_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef CEC_DRV_IF
#define CEC_DRV_IF

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "hw_vdoin.h"
#include "cec_hw_if.h"
#include "cec_if.h"

#include "hw_ckgen.h"
#include "api_eep.h"
#include "eepdef.h"
#include "eeprom_if.h"
#include "drvcust_if.h"
#include "pdwnc_if.h"

#include "x_os.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_pinmux.h"
#include "x_pdwnc.h"
#include "x_bim.h"
#include "x_timer.h"

/// define CEC module debug log function. 
#define DEFINE_IS_LOG      CEC_IsLog
#include "x_debug.h"

LINT_EXT_HEADER_END
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

#ifdef CC_DRIVER_PROGRAM
#define CTS_TEST
#else
#define NOTIFY_CECMW
#endif

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define RETX_MAX_CNT 2

#define CEC_LOG_BROADCAST CEC_LOG_UNREGISTERED

#define CEC_SUCCESS (0)

#define CEC_ERROR (1)

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
typedef enum
{
    CEC_LOG_TV                = 0, 
    CEC_LOG_RECORDING_DEVICE1 = 1, 
    CEC_LOG_RECORDING_DEVICE2 = 2, 
    CEC_LOG_TUNER1            = 3, 
    CEC_LOG_PLAYBACK_DEVICE1  = 4, 
    CEC_LOG_AUDIO_SYSTEM      = 5, 
    CEC_LOG_TUNER2            = 6, 
    CEC_LOG_TUNER3            = 7, 
    CEC_LOG_PLAYBACK_DEVICE2  = 8, 
    CEC_LOG_RECORDING_DEVICE3 = 9, 
    CEC_LOG_TUNER4            = 10, 
    CEC_LOG_PLAYBACK_DEVICE3  = 11, 
    CEC_LOG_RESERVED1         = 12, 
    CEC_LOG_RESERVED2         = 13, 
    CEC_LOG_FREE_USE          = 14, 
    CEC_LOG_UNREGISTERED      = 15
} CEC_LOGICAL_ADDR;

typedef enum
{
    OPCODE_FEATURE_ABORT = 0x00,				// 4
    OPCODE_IMAGE_VIEW_ON = 0x04,				// 2
    OPCODE_TUNER_STEP_INCREMENT = 0x05,			// 2
    OPCODE_TUNER_STEP_DECREMENT = 0x06,			// 2
    OPCODE_TUNER_DEVICE_STATUS = 0x07,			// 7 or 10
    OPCODE_GIVE_TUNER_DEVICE_STATUS = 0x08,			// 3
    OPCODE_RECORD_ON = 0x09,				// 3~10
    OPCODE_RECORD_STATUS = 0x0A,				// 3
    OPCODE_RECORD_OFF = 0x0B,				// 2
    OPCODE_TEXT_VIEW_ON = 0x0D,				// 2
    OPCODE_RECORD_TV_SCREEN = 0x0F,				// 2
    OPCODE_GIVE_DECK_STATUS = 0x1A,				// 3
    OPCODE_DECK_STATUS = 0x1B,				// 3
    OPCODE_SET_MENU_LANGUAGE = 0x32,			// 5
    OPCODE_CLEAR_ANALOGUE_TIMER = 0x33,			// 13
    OPCODE_SET_ANALOGUE_TIMER = 0x34,			// 13
    OPCODE_TIMER_STATUS = 0x35,				// 3 or 5
    OPCODE_STANDBY = 0x36,					// 2
    OPCODE_PLAY = 0x41,					// 3
    OPCODE_DECK_CONTROL = 0x42,				// 3
    OPCODE_TIMER_CLEARED_STATUS = 0x43,			// 3
    OPCODE_USER_CONTROL_PRESSED = 0x44,			// 3
    OPCODE_USER_CONTROL_RELEASED = 0x45,			// 2
    OPCODE_GIVE_OSD_NAME = 0x46,				// 2
    OPCODE_SET_OSD_NAME = 0x47,				// 3~16
    OPCODE_SET_OSD_STRING = 0x64,				// 4~16
    OPCODE_SET_TIMER_PROGRAM_TITLE = 0x67,			// 3~16
    OPCODE_SYSTEM_AUDIO_MODE_REQUEST = 0x70,		// 4
    OPCODE_GIVE_AUDIO_STATUS = 0x71,			// 2
    OPCODE_SET_SYSTEM_AUDIO_MODE = 0x72,			// 3
    OPCODE_REPORT_AUDIO_STATUS = 0x7A,			// 3
    OPCODE_GIVE_SYSTEM_AUDIO_MODE_STATUS = 0x7D,		// 2
    OPCODE_SYSTEM_AUDIO_MODE_STATUS = 0x7E,			// 3
    OPCODE_ROUTING_CHANGE = 0x80,				// 6
    OPCODE_ROUTING_INFORMATION = 0x81,			// 4
    OPCODE_ACTIVE_SOURCE = 0x82,				// 4
    OPCODE_GIVE_PHYSICAL_ADDRESS = 0x83,			// 2
    OPCODE_REPORT_PHYSICAL_ADDRESS = 0x84,			// 5
    OPCODE_REQUEST_ACTIVE_SOURCE = 0x85,			// 2
    OPCODE_SET_STREAM_PATH = 0x86,				// 4
    OPCODE_DEVICE_VENDOR_ID = 0x87,				// 5
    OPCODE_VENDOR_COMMAND = 0x89,				// <= 16
    OPCODE_VENDOR_REMOTE_BUTTON_DOWN = 0x8A,		// <= 16
    OPCODE_VENDOR_REMOTE_BUTTON_UP = 0x8B,			// 2
    OPCODE_GIVE_DEVICE_VENDOR_ID = 0x8C,			// 2
    OPCODE_MENU_REQUEST = 0x8D,				// 3
    OPCODE_MENU_STATUS = 0x8E,				// 3
    OPCODE_GIVE_DEVICE_POWER_STATUS = 0x8F,			// 2
    OPCODE_REPORT_POWER_STATUS = 0x90,			// 3
    OPCODE_GET_MENU_LANGUAGE = 0x91,			// 2
    OPCODE_SELECT_ANALOGUE_SERVICE = 0x92,			// 6
    OPCODE_SELECT_DIGITAL_SERVICE = 0x93,			// 9
    OPCODE_SET_DIGITAL_TIMER = 0x97,			// 16
    OPCODE_CLEAR_DIGITAL_TIMER = 0x99,			// 16
    OPCODE_SET_AUDIO_RATE = 0x9A,				// 3
    OPCODE_INACTIVE_SOURCE = 0x9D,				// 4
    OPCODE_CEC_VERSION = 0x9E,				// 3
    OPCODE_GET_CEC_VERSION = 0x9F,				// 2
    OPCODE_VENDOR_COMMAND_WITH_ID = 0xA0,			// <= 16
    OPCODE_CLEAR_EXTERNAL_TIMER = 0xA1,			// 10 ~ 11
    OPCODE_SET_EXTERNAL_TIMER = 0xA2,			// 10 ~ 11
    OPCODE_ARC_INIT = 0xC0,                                      // 2
    OPCODE_ARC_REPORT_INIT = 0xC1,                      // 2
    OPCODE_ARC_REPORT_TERMINATE = 0xC2,          // 2
    OPCODE_ARC_REQUEST_INIT = 0xC3,                   // 2
    OPCODE_ARC_REQUEST_TERMINATE = 0xC4,       // 2
    OPCODE_ARC_TERMINATE = 0xC5,                        // 2
    OPCODE_CDC_MESSAGE = 0xF8,
    OPCODE_ABORT = 0xFF,					// 2
    OPCODE_NONE = 0xFFF
} CEC_OPCODE;

typedef enum
{
    STATE_TX_SEND_FRAME = 0x0001,
    STATE_TX_WAIT_DATA_TAKEN = 0x0002,  /*Tx with data, not ping frame.*/
    STATE_TX_RESEND = 0x0004,       
    STATE_TX_NOACK = 0x0008,
    STATE_TX_SEND_FRAME_OK = 0x0010,

    STATE_RX_GET_NEW_HEADER = 0x0100,
    STATE_RX_WAIT_COMPLETE = 0x0200
} CEC_SW_STATE;

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
/* RX_EVENT */
#define IS_INT_DATA_RDY() (RegReadFldAlign(RX_EVENT, DATA_RDY))
#define IS_INT_HEADER_RDY() (RegReadFldAlign(RX_EVENT, HEADER_RDY))
#define IS_INT_OV() (RegReadFldAlign(RX_EVENT, OV))
#define IS_INT_BR_SB_RDY() (RegReadFldAlign(RX_EVENT, BR_SB_RDY))
#define IS_INT_SB_RDY() (RegReadFldAlign(RX_EVENT, SB_RDY))
#define IS_INT_BR_RDY() (RegReadFldAlign(RX_EVENT, BR_RDY))
#define ENABLE_INT_DATA_RDY(onoff) ((void)vRegWriteFldAlign(RX_EVENT, onoff, I_EN_DATA))
#define ENABLE_INT_HEADER_RDY(onoff) ((void)vRegWriteFldAlign(RX_EVENT, onoff, I_EN_HEADER))
#define ENABLE_INT_OV(onoff) ((void)vRegWriteFldAlign(RX_EVENT, onoff, I_EN_OV))
#define ENABLE_INT_PULSE(onoff) ((void)vRegWriteFldAlign(RX_EVENT, onoff, I_EN_PULSE))
#define ENABLE_INT_BR_SB_RDY(onoff) ((void)vRegWriteFldAlign(RX_EVENT, onoff, I_EN_BR_SB))
#define ENABLE_INT_SB_RDY(onoff) ((void)vRegWriteFldAlign(RX_EVENT, onoff, I_EN_SB))
#define ENABLE_INT_BR_RDY(onoff) ((void)vRegWriteFldAlign(RX_EVENT, onoff, I_EN_BR))
#define CLR_INT_DATA_RDY() ((void)vRegWriteFldAlign(RX_EVENT, 0, DATA_RDY))
#define CLR_INT_HEADER_RDY() ((void)vRegWriteFldAlign(RX_EVENT, 0, HEADER_RDY))
#define CLR_INT_OV() ((void)vRegWriteFldAlign(RX_EVENT, 0, OV))
#define NOTIFY_RX_HW_DATA_TAKEN() ((void)vRegWriteFldAlign(RX_EVENT, 0, BR_RDY))
#define RX_EVENT_STATUS() (u4RegRead4B(RX_EVENT))

/* TX_EVENT */
#define IS_INT_UN() (RegReadFldAlign(TX_EVENT, UN))
#define IS_INT_FAIL() (RegReadFldAlign(TX_EVENT, FAIL))
#define IS_INT_LOW() (RegReadFldAlign(TX_EVENT, LOW))
#define IS_TX_FINISH() (RegReadFldAlign(TX_EVENT, TX_D_FINISH))
#define IS_INT_RB_RDY() (RegReadFldAlign(TX_EVENT, RB_RDY))
#define ENABLE_INT_DATA_FIN(onoff) ((void)vRegWriteFldAlign(TX_EVENT, onoff, TX_DATA_FIN_E))
#define ENABLE_INT_UN(onoff) ((void)vRegWriteFldAlign(TX_EVENT, onoff, I_EN_UN))
#define ENABLE_INT_FAIL(onoff) ((void)vRegWriteFldAlign(TX_EVENT, onoff, I_EN_FAIL))
#define ENABLE_INT_LOW(onoff) ((void)vRegWriteFldAlign(TX_EVENT, onoff, I_EN_LOW))
#define ENABLE_INT_BS(onoff) ((void)vRegWriteFldAlign(TX_EVENT, onoff, I_EN_BS))
#define ENABLE_INT_RB(onoff) ((void)vRegWriteFldAlign(TX_EVENT, onoff, I_EN_RB))
#define CLR_INT_UN() ((void)vRegWriteFldAlign(TX_EVENT, 0, UN))
#define CLR_INT_LOW() ((void)vRegWriteFldAlign(TX_EVENT, 0, LOW))
#define CLR_TX_FINISH() ((void)vRegWriteFldAlign(TX_EVENT, 0, TX_D_FINISH))
#define TRIGGER_TX_HW() ((void)vRegWriteFldAlign(TX_EVENT, 1, RB_RDY))
#define IS_TX_DATA_TAKEN() (!(IS_INT_RB_RDY()))
#define IS_INT_RB_ENABLE() (RegReadFldAlign(TX_EVENT, I_EN_RB))
#define IS_INT_FAIL_ENABLE() (RegReadFldAlign(TX_EVENT, I_EN_FAIL))
#define TX_EVENT_STATUS() (u4RegRead4B(TX_EVENT))

#define DISABLE_ALL_TX_INT() \
{ \
	ENABLE_INT_FAIL(0); \
	ENABLE_INT_RB(0); \
	ENABLE_INT_LOW(0); \
	ENABLE_INT_UN(0); \
	ENABLE_INT_BS(0); \
}

/* RX FSM status */
#define IS_RX_FSM_IDLE() (RegReadFldAlign(RX_STATUS, RX_FSM) == 0x01)
#define IS_RX_FSM_START() (RegReadFldAlign(RX_STATUS, RX_FSM) == 0x02)
#define IS_RX_FSM_MODE() (RegReadFldAlign(RX_STATUS, RX_FSM) == 0x04)
#define IS_RX_FSM_MODE3_HEADER() (RegReadFldAlign(RX_STATUS, RX_FSM) == 0x08)
#define IS_RX_FSM_MODE3_DATA() (RegReadFldAlign(RX_STATUS, RX_FSM) == 0x10)
#define IS_RX_FSM_ERROR_S() (RegReadFldAlign(RX_STATUS, RX_FSM) == 0x20)
#define IS_RX_FSM_ERROR_D() (RegReadFldAlign(RX_STATUS, RX_FSM) == 0x40)
#define RX_FSM_STATUS() (RegReadFldAlign(RX_STATUS, RX_FSM))

/* TX FSM status */
#define IS_TX_FSM_IDLE() (RegReadFldAlign(TX_STATUS, TX_FSM) == 0x01)
#define IS_TX_FSM_INIT() (RegReadFldAlign(TX_STATUS, TX_FSM) == 0x02)
#define IS_TX_FSM_EOM() (RegReadFldAlign(TX_STATUS, TX_FSM) == 0x04)
#define IS_TX_FSM_RETRASMIT() (RegReadFldAlign(TX_STATUS, TX_FSM) == 0x08)
#define IS_TX_FSM_FAIL() (RegReadFldAlign(TX_STATUS, TX_FSM) == 0x10)
#define IS_TX_FSM_START() (RegReadFldAlign(TX_STATUS, TX_FSM) == 0x20)
#define IS_TX_FSM_MODE() (RegReadFldAlign(TX_STATUS, TX_FSM) == 0x40)
#define IS_TX_FSM_MODE3_HEADER() (RegReadFldAlign(TX_STATUS, TX_FSM) == 0x80)
#define IS_TX_FSM_MODE3_DATA() (RegReadFldAlign(TX_STATUS, TX_FSM) == 0x100)
#define TX_FSM_STATUS() (RegReadFldAlign(TX_STATUS, TX_FSM))

#define ENABLE_TX_EN() ((void)vRegWriteFldAlign(TR_CONFIG, 1, TX_EN))
#define DISABLE_TX_EN() ((void)vRegWriteFldAlign(TR_CONFIG, 0, TX_EN))
#define ENABLE_RX_EN() ((void)vRegWriteFldAlign(TR_CONFIG, 1, RX_EN))
#define DISABLE_RX_EN() ((void)vRegWriteFldAlign(TR_CONFIG, 0, RX_EN))

#define SET_HW_TX_LEN(num) ((void)vRegWriteFldAlign(TX_HD_NEXT, num, WTX_M3_DATA_MASK))
#define FILL_SRC_FIELD(addr) ((void)vRegWriteFldAlign(TX_HD_NEXT, addr, WTX_SRC))
#define FILL_DST_FIELD(addr) ((void)vRegWriteFldAlign(TX_HD_NEXT, addr, WTX_DST))
#define MARK_H_EOM(onoff) ((void)vRegWriteFldAlign(TX_HD_NEXT, onoff, WTX_H_EOM))
#define MARK_D_EOM(onoff) ((void)vRegWriteFldAlign(TX_HD_NEXT, onoff, WTX_D_EOM))
#define FILL_TX_DATA(data) ((void)vRegWriteFldAlign(TX_DATA_NEXT, data, WTX_DATA))

#define GET_HW_RX_LEN() (RegReadFldAlign(RX_HEADER, RXED_M3_DATA_MASK))
#define GET_SRC_FIELD() (RegReadFldAlign(RX_HEADER, RXED_SRC))
#define GET_DST_FIELD() (RegReadFldAlign(RX_HEADER, RXED_DST))
#define GET_SRC_FIELD_RECEIVING() (RegReadFldAlign(RX_HD_NEXT, RXING_SRC))
#define GET_DST_FIELD_RECEIVING() (RegReadFldAlign(RX_HD_NEXT, RXING_DST))
#define IS_RX_H_EOM() (RegReadFldAlign(RX_HEADER, RXED_H_EOM))
#define IS_RX_D_EOM() (RegReadFldAlign(RX_HEADER, RXED_D_EOM))

#define GET_HW_RX_DATA() (RegReadFldAlign(RX_DATA, RXED_DATA)) 

#define FLOW_CONTROL_ACK(onoff) \
    ((void)vRegWriteFldAlign(RX_HD_NEXT, (!(onoff)), RXING_H_ACK)); \
    ((void)vRegWriteFldAlign(RX_HD_NEXT, (!(onoff)), RXING_D_ACK));

#define GET_FOLLOWER_H_ACK() (RegReadFldAlign(TX_HEADER, TXING_H_ACK))
#define GET_FOLLOWER_D_ACK() (RegReadFldAlign(TX_HEADER, TXING_D_ACK))

#define TX_FAIL_MAX() (RegReadFldAlign(TX_FAIL, RETX_MAX))
#define CLR_TX_FAIL_MAX()  ((void)vRegWriteFldAlign(TX_FAIL, 0, RETX_MAX))
#define TX_FAIL_RECORD() (u4RegRead4B(TX_FAIL))

#define GET_DEVICE_LA() (RegReadFldAlign(TR_CONFIG, DEVICE_ADDR1))
#define CHECK_RX_EN() (RegReadFldAlign(TR_CONFIG, RX_EN))

#define RX_FAIL_RECORD() (u4RegRead4B(RX_FAIL))
#define CLR_RX_ERROR_ONCE() ((void)vRegWriteFldAlign(RX_FAIL, 0, ERR_ONCE))

#define RESET_HW_TX() \
    DISABLE_TX_EN(); ENABLE_TX_EN()

#define GET_TX_BIT_COUNTER() (RegReadFldAlign(TX_STATUS, TX_BIT_COUNTER))

#define SetCECStatus(arg) { \
    CRIT_STATE_T rCritState; \
    rCritState = x_crit_start(); \
    (_CEC_Status |= ((UINT16)arg)); \
    x_crit_end(rCritState); }

#define ClrCECStatus(arg) { \
    CRIT_STATE_T rCritState; \
    rCritState = x_crit_start(); \
    (_CEC_Status &= (~((UINT16)arg))); \
    x_crit_end(rCritState); }

#define IsCECStatus(arg) 	((_CEC_Status & ((UINT16)arg)) > 0)

#define CLR_CEC_BUSY_STATUS() (_u2CecBusyStatus = 0)
#define CEC_BUSY_STATUS() _u2CecBusyStatus

#define CEC_IS_BUSY() (_u2CecBusyStatus)

#define CEC_RX_IS_BUSY() (_u2CecBusyStatus & CEC_RX_BUSY)

#define SET_CECRX_BUSY()\
 	do {\
 		_u2CecBusyStatus |= CEC_RX_BUSY;\
 		LOG(7, "RX busy ----->>\n");\
 	   }while(0)		

#define CLR_CECRX_BUSY()\
 	do {\
 		_u2CecBusyStatus &= (~CEC_RX_BUSY);\
 		LOG(7, "Clr RX busy <<-----\n");\
 	   }while(0)		

#define CEC_TX_IS_BUSY() (_u2CecBusyStatus & CEC_TX_BUSY)

#define SET_CECTX_BUSY()\
 	do {\
 		_u2CecBusyStatus |= CEC_TX_BUSY;\
 		LOG(7, "TX busy ----->>\n");\
 	   }while(0)		

#define CLR_CECTX_BUSY()\
 	do {\
 		if (CEC_TX_IS_BUSY())\
 		LOG(7, "Clr TX busy <<-----\n");\
 		_u2CecBusyStatus &= (~CEC_TX_BUSY);\
 	   }while(0)		 

#define CEC_TX_FRAME (TRUE)
#define CEC_RX_FRAME (FALSE)
//---------------------------------------------------------------------------
// Inter-file variable
//---------------------------------------------------------------------------
EXTERN UINT16 _CEC_Status;

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------
EXTERN void CEC_PrintFrame(const CEC_FRAME_DESCRIPTION *pFrame, BOOL fgTxFrame);

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

#endif /* CEC_DRV_IF */

