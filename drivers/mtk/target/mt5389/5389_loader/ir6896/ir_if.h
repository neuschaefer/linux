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
 * $Author: hongjun.chu $
 * $Date: 2012/04/28 $
 * $RCSfile: ir_if.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file ir_if.h
 *  This header file defines the primitive and interface functions \n
 *  for IR, both receiver and blaster.
 */


#ifndef DRV_IR_H
#define DRV_IR_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_typedef.h"
#include "x_lint.h"

//#include "ir_api.h"

#ifdef CHANNEL_CHANGE_LOG
#include "x_timer.h"
#else
#ifdef IRRC_LOG
#include "x_timer.h"
#endif
#endif

LINT_EXT_HEADER_BEGIN
#include "x_irrc_btn_def.h"
LINT_EXT_HEADER_END

#include "drvcust_if.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#ifndef KEY_GROUP_DIGIT

/** Definiton of key group
 *
 */
#define KEY_GROUP_DIGIT         ((UINT32) 0x00010000)       ///< Button group digit.
#define KEY_GROUP_ALPHA         ((UINT32) 0x00020000)       ///< Button group alpha.
#define KEY_GROUP_CURSOR        ((UINT32) 0x00030000)       ///< Button group cursor.
#define KEY_GROUP_SEL_CTRL      ((UINT32) 0x00040000)       ///< Button group set control.
#define KEY_GROUP_PRG_CTRL      ((UINT32) 0x00050000)       ///< Button group program control.
#define KEY_GROUP_AUD_CTRL      ((UINT32) 0x00060000)       ///< Button group audio control.
#define KEY_GROUP_FCT_CTRL      ((UINT32) 0x00070000)       ///< Button group function contrl.
#define KEY_GROUP_USER_DEF      ((UINT32) 0x00080000)       ///< Button group user define.
#define KEY_GROUP_MAX           ((UINT32) 0x00090000)       ///< Button group max.

/** Definiton of key ID 
 *
 */
#define BTN_DIGIT_0             ((UINT32) (KEY_GROUP_DIGIT | ((UINT32) '0')))   ///< Button digit 0.
#define BTN_DIGIT_1             ((UINT32) (KEY_GROUP_DIGIT | ((UINT32) '1')))   ///< Button digit 1.
#define BTN_DIGIT_2             ((UINT32) (KEY_GROUP_DIGIT | ((UINT32) '2')))   ///< Button digit 2.
#define BTN_DIGIT_3             ((UINT32) (KEY_GROUP_DIGIT | ((UINT32) '3')))   ///< Button digit 3.
#define BTN_DIGIT_4             ((UINT32) (KEY_GROUP_DIGIT | ((UINT32) '4')))   ///< Button digit 4.
#define BTN_DIGIT_5             ((UINT32) (KEY_GROUP_DIGIT | ((UINT32) '5')))   ///< Button digit 5.
#define BTN_DIGIT_6             ((UINT32) (KEY_GROUP_DIGIT | ((UINT32) '6')))   ///< Button digit 6.
#define BTN_DIGIT_7             ((UINT32) (KEY_GROUP_DIGIT | ((UINT32) '7')))   ///< Button digit 7.
#define BTN_DIGIT_8             ((UINT32) (KEY_GROUP_DIGIT | ((UINT32) '8')))   ///< Button digit 8.
#define BTN_DIGIT_9             ((UINT32) (KEY_GROUP_DIGIT | ((UINT32) '9')))   ///< Button digit 9.
#define BTN_DIGIT_PLUS_100      ((UINT32) (KEY_GROUP_DIGIT | 0x0000f000))           ///< Button digit plus 100.
#define BTN_DIGIT_DOT           ((UINT32) (KEY_GROUP_DIGIT | 0x0000f001))           ///< Button digit dot.

#define BTN_CURSOR_LEFT         ((UINT32) (KEY_GROUP_CURSOR | 0x0000f000))          ///< Button cursor left.
#define BTN_CURSOR_RIGHT        ((UINT32) (KEY_GROUP_CURSOR | 0x0000f001))          ///< Button cursor right.       
#define BTN_CURSOR_UP           ((UINT32) (KEY_GROUP_CURSOR | 0x0000f002))          ///< Button cursor up.
#define BTN_CURSOR_DOWN         ((UINT32) (KEY_GROUP_CURSOR | 0x0000f003))          ///< Button cursor down.

#define BTN_EXIT                ((UINT32) (KEY_GROUP_SEL_CTRL | 0x0000f000))        ///< Button exit.         
#define BTN_SELECT              ((UINT32) (KEY_GROUP_SEL_CTRL | 0x0000f001))        ///< Button select.

#define BTN_PRG_UP              ((UINT32) (KEY_GROUP_PRG_CTRL | 0x0000f000))        ///< Button program up.         
#define BTN_PRG_DOWN            ((UINT32) (KEY_GROUP_PRG_CTRL | 0x0000f001))        ///< Button program down.
#define BTN_PREV_PRG            ((UINT32) (KEY_GROUP_PRG_CTRL | 0x0000f002))        ///< Button return to previous program.
#define BTN_POP                 ((UINT32) (KEY_GROUP_PRG_CTRL | 0x0000f003))        ///< Button POP.
#define BTN_ZOOM                ((UINT32) (KEY_GROUP_PRG_CTRL | 0x0000f004))        ///< Button ZOOM.
#define BTN_SWAP                ((UINT32) (KEY_GROUP_PRG_CTRL | 0x0000f005))        ///< Button SWAP.
#define BTN_FAVORITE            ((UINT32) (KEY_GROUP_PRG_CTRL | 0x0000f006))        ///< Button favorite.
#define BTN_FAV_CH              ((UINT32) (KEY_GROUP_PRG_CTRL | 0x0000f007))        ///< Button favorite program.
#define BTN_PRG_INFO            ((UINT32) (KEY_GROUP_PRG_CTRL | 0x0000f008))        ///< Button program information.
#define BTN_ADD_ERASE           ((UINT32) (KEY_GROUP_PRG_CTRL | 0x0000f009))        ///< Button add / erase.
#define BTN_CC                  ((UINT32) (KEY_GROUP_PRG_CTRL | 0x0000f00a))        ///< Button CC.
#define BTN_INPUT_SRC           ((UINT32) (KEY_GROUP_PRG_CTRL | 0x0000f00b))        ///< Button input source chnage.

#define BTN_VOL_UP              ((UINT32) (KEY_GROUP_AUD_CTRL | 0x0000f000))        ///< Button volume up.
#define BTN_VOL_DOWN            ((UINT32) (KEY_GROUP_AUD_CTRL | 0x0000f001))        ///< Button volume down.
#define BTN_MUTE                ((UINT32) (KEY_GROUP_AUD_CTRL | 0x0000f002))        ///< Button audio mute.
#define BTN_MTS                 ((UINT32) (KEY_GROUP_AUD_CTRL | 0x0000f003))        ///< Button MTS.

#define BTN_POWER               ((UINT32) (KEY_GROUP_FCT_CTRL | 0x0000f000))        ///< Button power.
#define BTN_MENU                ((UINT32) (KEY_GROUP_FCT_CTRL | 0x0000f001))        ///< Button menu.
#define BTN_EPG                 ((UINT32) (KEY_GROUP_FCT_CTRL | 0x0000f002))        ///< Button EPG.
#define BTN_MEM_CARD            ((UINT32) (KEY_GROUP_FCT_CTRL | 0x0000f003))        ///< Button memory card.

#define BTN_RED                 ((UINT32) (KEY_GROUP_USER_DEF | 0x0000f000))        ///< Button red.
#define BTN_GREEN               ((UINT32) (KEY_GROUP_USER_DEF | 0x0000f001))        ///< Button green.
#define BTN_YELLOW              ((UINT32) (KEY_GROUP_USER_DEF | 0x0000f002))        ///< Button yellow.
#define BTN_BLUE                ((UINT32) (KEY_GROUP_USER_DEF | 0x0000f003))        ///< Button blue.

#endif /* KEY_GROUP_DIGITS */

#define BTN_PICSIZE             BTN_PIP_SIZE                                        ///< Button PIP size change.

#define BTN_INPUT               BTN_INPUT_SRC                                       ///< Button input change.
#define BTN_OSD                 BTN_PRG_INFO                                        ///< Button program information.
#define BTN_FVRTCHNL            BTN_FAV_CH                                          ///< Button favorite program.
#define BTN_CLOCK               BTN_EPG                                             ///< Button clock.
#define BTN_UPDATE              BTN_MEM_CARD                                        ///< Button update.
#define BTN_CE                  BTN_EXIT                                            ///< Button cancel.
#define BTN_TTTV                BTN_TTX                                             ///< Button teletext.
#define BTN_PIPPOS              BTN_PIP_POS                                         ///< Button PIP position.
#define BTN_COLORSYS            BTN_ANT_CBL                                         ///< Button color system.    
#define BTN_ACTCTRL             BTN_ASPECT                                          ///< Button act control.

#ifndef BTN_ANT_CBL
#define BTN_ANT_CBL             BTN_TV                                              ///< Button analog TV / cable TV switch.
#endif /* BTN_ANT_CBL */

#ifndef BTN_PREV_PRG
#define BTN_PREV_PRG            BTN_PREV_CH                                         ///< Button return to previous program.
#endif /* BTN_PREV_PRG */

#ifndef BTN_FAVORITE
#define BTN_FAVORITE            BTN_FAV_CH_UP                                       ///< Button favorite. 
#endif /* BTN_FAVORITE */

#ifndef BTN_EXIT
#define BTN_EXIT                BTN_EXIT_RETURN                                     ///< Button exit.
#endif /* BTN_EXIT */

/***************************************************************************/
#define KEY_GROUP_NO_DEF        ((UINT32) 0xffff0000)                               ///< Undefined button group.

#ifndef BTN_SMARTPIC
#define BTN_SMARTPIC            ((UINT32) (KEY_GROUP_NO_DEF | 0x00000003))          ///< Button smart picture.
#endif /* BTN_SMARTPIC */

#ifndef BTN_CAPTURE
#define BTN_CAPTURE             ((UINT32) (KEY_GROUP_NO_DEF | 0x0000000d))          ///< Button capture.
#endif /* BTN_CAPTURE */

#ifndef BTN_SLEEP
#define BTN_SLEEP               ((UINT32) (KEY_GROUP_NO_DEF | 0x00000010))          ///< Button sleep.
#endif /* BTN_SLEEP */

#ifndef BTN_INDEX
#define BTN_INDEX               ((UINT32) (KEY_GROUP_NO_DEF | 0x00000014))          ///< Button index.
#endif

#ifndef BTN_VCHIP
#define BTN_VCHIP               ((UINT32) (KEY_GROUP_NO_DEF | 0x0000001a))          ///< Button V-Chip.
#endif /* BTN_VCHIP */

#ifndef BTN_SNDEFCT
#ifndef BTN_S_EFFECT
#define BTN_SNDEFCT             ((UINT32) (KEY_GROUP_NO_DEF | 0x0000001c))          ///< Button sound effect.
#else
#define BTN_SNDEFCT             BTN_S_EFFECT                                        ///< Button sound effect.        
#endif
#endif /* BTN_SNDEFCT */


/**
 * u4Info:
 *   Bit 31~24 is the value of the sampling counter in the 3rd pulse.
 *   Bit 23~16 is the value of the sampling counter in the 2nd pulse.
 *   Bit 15~08 is the value of the sampling counter in the 1st pulse.
 *   Bit 05~00 is the bit count of this IR command.
 */
#define INFO_TO_BITCNT(u4Info)      ((u4Info & IRRX_CH_BITCNT_MASK)    >> IRRX_CH_BITCNT_BITSFT)        ///< IR bit count information.
#define INFO_TO_1STPULSE(u4Info)    ((u4Info & IRRX_CH_1ST_PULSE_MASK) >> IRRX_CH_1ST_PULSE_BITSFT)     ///< IR fisrt pulse.
#define INFO_TO_2NDPULSE(u4Info)    ((u4Info & IRRX_CH_2ND_PULSE_MASK) >> IRRX_CH_2ND_PULSE_BITSFT)     ///< IR second pulse.
#define INFO_TO_3RDPULSE(u4Info)    ((u4Info & IRRX_CH_3RD_PULSE_MASK) >> IRRX_CH_3RD_PULSE_BITSFT)     ///< IR third pulse.

//-----------------------------------------------------------------------------
#define KEY_SRC_DRIVER          ((UINT32) 0xf0000000)                                                   ///< Button event is from driver.                
#define KEY_GROUP_MASK          ((UINT32) 0x0fff0000)                                                   ///< Button group mask
#define KEY_GROUP_DRIVER1       ((UINT32) 0x0ff10000)                                                   ///< Button group driver 1.
#define KEY_GROUP_DRIVER2       ((UINT32) 0x0ff20000)                                                   ///< Button group driver 2.
#define BTN_NO_DEF              ((UINT32) 0xfffffffe)                                                   ///< Undefined button.
#define BTN_DRVKEY_REPEAT       ((UINT32) 0xfffffffd)                                                   ///< Button repeat.

/** Definiton of factory key ID 
 * 0 - BTN_DVD ; 1~127 - BTN_ATV_#
 */
#define BTN_DRIVER1_FACTORY(x)  ((UINT32) (KEY_SRC_DRIVER | KEY_GROUP_DRIVER1 | (x)))   ///< Button factory setting 1.        
#define BTN_DRIVER2_FACTORY(x)  ((UINT32) (KEY_SRC_DRIVER | KEY_GROUP_DRIVER2 | (x)))   ///< Button factory setting 2.        


/** IR interface return value
 */
#define IR_FAIL             (-1)            ///< IR interface invocation failed.
#define IR_SUCC             (0)             ///< IR interface invocation succeeded.

/** IR button state
 */
#define IRRX_COND_BTN_UP    (0)             ///< Button is released.
#define IRRX_COND_BTN_DOWN  (1)             ///< Button is pressed.


#define IRRC_LOG_SIZE (256)
#define IRRX_MAX_NUM_ADDR_FILTER (16)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/** IR RX callback function
 */
typedef void (* PFN_IRRXCB_T)(UINT32 u4Info, const UINT8 *pu1Data);

/** IR RX callback thread - BtnState 0:Up 1:Down 
 */
typedef void (* PFN_IRRXCB_THREAD_T)(UINT32 u4BtnState, UINT32 u4Key, IRRX_RAW_DATA_T *prRawData);

/** IR waveform structure
 */
typedef struct _rWaveForm {
    UINT32 u4SyncHigh;
    UINT32 u4SyncLow;
    UINT32 u4Data0High;
    UINT32 u4Data0Low;
    UINT32 u4Data1High;
    UINT32 u4Data1Low;
} IRTXWAVE_T;

/** IR external key lookup function type
 */
typedef UINT32 (*FN_KEYMAP_LOOKUP_T)(UINT32 u4GroupCode, UINT32 u4KeyCode);


typedef enum
{
    IRRX_RAW_DATA_NONE = 0,
    IRRX_RAW_DATA_NEC,
    IRRX_RAW_DATA_RC5,
    IRRX_RAW_DATA_RC6,
    IRRX_RAW_DATA_FACTORY,    
}   IRRX_RAW_DATA_TYPE_T;

typedef struct 
{
    INT32 i4Src;
    INT32 i4Sys;
    INT32 i4Cmd;
} IRRX_RC56_WAKEUP_KEY_T;

/** IR log record
 */

#ifdef IRRC_LOG
typedef struct {
    UINT32 u4Raw;
    HAL_TIME_T rTime;	
} IRRC_LOG_T;

/** IR logger
 */

typedef struct {
    UINT32 u4CurrIdx;	
    IRRC_LOG_T arLog[IRRC_LOG_SIZE];
} IRRC_LOGGER_T;
#endif


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/** IR_Diag
 *  IR Diagnostic function
 *  @param	none
 *  @return 	diagnostic result
 */
EXTERN INT32 IR_Diag(void);

/** IRRC_DisconnectMiddleware
 *  Disconnect IR from middleware
 *  @param	none
 *  @return 	none
 */
EXTERN VOID IRRC_DisconnectMiddleware( VOID );

/** IRRC_WaitPowerKeyReboot
 *  Wait until power key reboot
 *  @param	none
 *  @return 	none
 */
EXTERN VOID IRRC_WaitPowerKeyReboot( VOID );

/** IRRC_LedBlinkConfig
 *  Set the LED blink configuration
 *  @param	bEnable	led config
 *  @return 	none
 */
EXTERN VOID IRRC_LedBlinkConfig( BOOL );

#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389) 
/** IRRC_MixSound
 *  Trigger IR mixsound 
 *  @param	u4BtnState key DOWN/UP state
 *  @param	u4Key button event 
 *  @param	prRaw IR raw data 
 *  @return 	none
 */
EXTERN void IRRC_MixSound(UINT32 u4BtnState, UINT32 u4Key, IRRX_RAW_DATA_T* prRaw);
#endif

/** IRRC_DataProcessingThread
 *  IRRC data processing thread
 *  @param	pvArg	Callback tag
 *  @return 	none
 */
EXTERN INT32 IRRC_ThreadInit( PFN_IRRXCB_THREAD_T );

/* IRRX IF functions. */

/** IRRX_InitMtkIrRegSetup() to initial IRRX register setup.
 *  @param u4Type to set RC56 or NEC IRRX type.
 *  @retval IR_FAIL run failed.
 *  @retval IR_OK run successfully.
 */
EXTERN INT32 IRRX_InitMtkIrRegSetup(UINT32 u4Type);

/** IRRX_InitMtkIr() to initialize the IRRX of MTK TV remote controller unit.
 *  @param u4Type to set RC56 or NEC IRRX type.
 *  @retval IR_FAIL run failed.
 *  @retval IR_OK run successfully.
 */
EXTERN INT32 IRRX_InitMtkIr(UINT32 u4Type);

/** IRRX_StopMtkIr() to stop the interrupt of IRRX of MTK TV remote controller
 *                  unit.
 *  @retval IR_FAIL run failed.
 *  @retval IR_OK run successfully.
 */
EXTERN INT32 IRRX_StopMtkIr(void);

/** IRRX_StopMtkIr() to stop the interrupt of IRRX of MTK TV remote controller
 *                  unit.
 *  @retval IR_FAIL run failed.
 *  @retval IR_OK run successfully.
 */
EXTERN INT32 IRRX_ResetMtkIr(void);

/** IRRX_StopSendMtkIr() to insert the IRRX key into buffer.
 *                  unit.
 *  @retval IR_FAIL run failed.
 *  @retval IR_OK run successfully.
 */
EXTERN INT32 IRRX_SendMtkIr(UINT32 u4Key);
/** IRRX_SendDebugMtkIr() to set mtrc debug log enable/disable.
 *                  unit.
 *  @retval IR_FAIL run failed.
 *  @retval IR_OK run successfully.
 */
EXTERN INT32 IRRX_SendDebugMtkIr(UINT32 u4Key,IRRX_RAW_DATA_T* prRaw);


/** IRRX_SendMtkIrRaw() to check there is any key available.
 *  @retval IR_FAIL run failed.
 *  @retval IR_OK run successfully.
 */
EXTERN INT32 IRRX_SendMtkIrRaw(UINT32 u4Key, IRRX_RAW_DATA_T* prRaw);

/** IRRX_IRRX_PoolMtkIr() to check there is any key available.
 *  @retval IR_FAIL run failed.
 *  @retval IR_OK run successfully.
 */
EXTERN INT32 IRRX_PollMtkIr(UINT32 *pu4Key, IRRX_RAW_DATA_T *prRaw);

/** IRRX_IsTheKey() to check the ir register content is the key of input.
 *  @param u4Key The key to check.
 *  @retval TRUE for yes, FALSE for no.
 */
EXTERN INT32 IRRX_IsTheKey(UINT32 u4Key);

/** IRRX_QuerySetRepeat() to set the repeat flag
 *  @param pi4Data pointer to a value to set the repeat flag.
 *  @return the new setting repeat flag
 */
EXTERN INT32 IRRX_QuerySetRepeat(const INT32 *pi4Data);

/** IRRX_QuerySet1stPulse() to set the 1st pulse value of IRRX signal
 *  @param pi4Data pointer to a value to set the 1st pulse value
 *  @return the new setting 1st pulse value
 */
EXTERN UINT32 IRRX_QuerySet1stPulse(const UINT32 *pu4Data);

/** IRRX_QuerySet2ndPulse() to set the 2nd pulse value of IRRX signal
 *  @param pi4Data pointer to a value to set the 2nd pulse value
 *  @return the new setting 2nd pulse value
 */
EXTERN UINT32 IRRX_QuerySet2ndPulse(const UINT32 *pu4Data);

/** IRRX_QuerySet3rdPulse() to set the 3rd pulse value of IRRX signal
 *  @param pi4Data pointer to a value to set the 3rd pulse value
 *  @return the new setting 3rd pulse value
 */
EXTERN UINT32 IRRX_QuerySet3rdPulse(const UINT32 *pu4Data);

/** IRRX_QuerySetRepeatTime() to set the repeat time slice
 *  @param pi4Data pointer to a value to set the repeat time slice.
 *  @return the new setting repeat time slice.
 */
EXTERN UINT32 IRRX_QuerySetRepeatTime(const UINT32 *pu4Data);

/** IRRX_QuerySetTimeout() to set the IR polling timeout
 *  @param pi4Data pointer to a value to set the timeout value.
 *  @return the new setting repeat time slice.
 */
EXTERN UINT32 IRRX_QuerySetTimeout(const UINT32 *pu4Data);

/** IRRX_QuerySetRepeatDuration() to set the valid repeat time duration
 *  @param pi4Data pointer to a value to set the duration.
 *  @return the new setting repeat time slice.
 */
EXTERN UINT32 IRRX_QuerySetRepeatDuration(const UINT32 *pu4Data);

/** IRHW_RxHwState
 *  Get HWIR RX state
 *  @param pi4Data pointer to a value to set the duration.
 *  @return the new setting repeat time slice.
 */
EXTERN UINT32 IRHW_RxHwState(void);

/** IRHW_RxRdConf() to read IRRX configration
 *  @param pi4Config to read IRRX_CONFIG_HIGH_REG
 *  @param pi4SaPeriod to read IRRX_CONFIG_LOW_REG
 *  @param pi4Threshold to read IRRX_THRESHOLD_REG
 */
EXTERN void IRHW_RxRdConf(INT32 *pi4Config, INT32 *pi4SaPeriod, INT32 *pi4Threshold);

/** IRHW_RxWrConf() to set IRRX configration
 *  @param i4Config to set IRRX_CONFIG_HIGH_REG
 *  @param i4SaPeriod to set IRRX_CONFIG_LOW_REG
 *  @param i4Threshold to set IRRX_THRESHOLD_REG
 */
EXTERN void IRHW_RxWrConf(INT32 i4Config, INT32 i4SaPeriod, INT32 i4Threshold);

/** IRHW_Init() to set IRRX configration and start IRRX ISR
 *  @param i4Config to set IRRX_CONFIG_HIGH_REG
 *  @param i4SaPeriod to set IRRX_CONFIG_LOW_REG
 *  @param i4Threshold to set IRRX_THRESHOLD_REG
 *  @retval 0 is operating successfully, otherwise failed.
 */
EXTERN INT32 IRHW_RxInit(INT32 i4Config, INT32 i4SaPeriod, INT32 i4Threshold);

/** IRHW_Stop() to stop IRRX ISR
 *  @retval 0 is operating successfully, otherwise failed.
 */
EXTERN INT32 IRHW_RxStop(void);

/** IRHW_RxSetCallback() to register interrupt callback routine.
 *  @param pfnCallback the new register callback routine
 *  @param ppfnOld pointer to the old callback routine
 *  @retval 0 is operating successfully, otherwise failed.
 */
EXTERN INT32 IRHW_RxSetCallback(PFN_IRRXCB_T pfnCallback, PFN_IRRXCB_T *ppfnOld);


/******************************************************************************/
/* IRTX IF functions. */

/* HWIR TX external functions declare. */

/** IRHW_TxRdConf() to read IRTX configuration
 *  @param pi4Config
 *  @param prWave
 *  @param pi4Modulation
 */
EXTERN void IRHW_TxRdConf(INT32 *pi4Config, IRTXWAVE_T *prWave, INT32 *pi4Modulation);

/** IRHW_TxWrConf
 *  Write IRTX configuration
 *  @param pi4Config
 *  @param prWave
 *  @param pi4Modulation
 */
EXTERN void IRHW_TxWrConf(INT32 i4Config, const IRTXWAVE_T *prWave, INT32 i4Modulation);

/** IRHW_TxSendData() to read IRTX configuration
 *  @param *pi4DataArray the transferred data array
 *  @param i4BitNum the bitnumber of data array to transferred 
 */
EXTERN void IRHW_TxSendData(const INT32 *pi4DataArray, INT32 i4BitNum);

/** IRRX_GetCrystalKey() to query crystalkey by group id & data.
 *  @param u4GroupId ir key group id.
 *  @param u1Data ir key data.
 *  @retval The crystal key.
 */
EXTERN UINT32 IRRX_GetCrystalKey(UINT32 u4GroupId, UINT8 u1Data);

/** IRRX_ReadSvadc() to read stable svadc value.
 *  @param u4Channel the channel number.
 *  @retval the svadc value(0~255)
 */
EXTERN UINT32 IRRX_ReadSvadc(UINT32 u4Channel);

//-----------------------------------------------------------------------------
/** IRRX_RawToCrystal() translate raw (addr + cmd) ir code to crystal code.
 *  @param u4Info the IRRX received related attributes of the current data.
 *  @param pu1Data the IRRX received and current data.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 IRRX_RawToCrystal(UINT32  u4AddrCode, UINT32 u4CmdCode);

//-----------------------------------------------------------------------------
/** IRRX_RawBitsToCrystal() translate raw ir code bits to crystal code.
 *  @param u4Info the IRRX received related attributes of the current data.
 *  @param pu1Data the IRRX received and current data.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 IRRX_RawBitsToCrystal(UINT32  u4Raw);

//-----------------------------------------------------------------------------
/** IRRX_FilterSystemCode() translate raw (addr + cmd) ir code to crystal code.
 *  @param u4Info the IRRX received related attributes of the current data.
 *  @param pu1Data the IRRX received and current data.
 */
//-----------------------------------------------------------------------------
EXTERN INT32 IRRX_FilterSystemCode(BOOL fgEnable, BOOL fgPositiveFilter, UINT32 u4Num, UINT32 au4FilterArray[]);

//-----------------------------------------------------------------------------
/** IRRX_FilterSystemCode() translate raw (addr + cmd) ir code to crystal code.
 *  @param u4Info the IRRX received related attributes of the current data.
 *  @param pu1Data the IRRX received and current data.
 */
//-----------------------------------------------------------------------------
EXTERN INT32 IRRX_SetWakeupKeys(UINT32 u4NumKey, IRRX_RC56_WAKEUP_KEY_T arKeyArray[]);
	
//-----------------------------------------------------------------------------
/** IRRC_RegCallbackFunc() register callback function.
 *  @param pfnCallback the IRRC callback function.
 */
//-----------------------------------------------------------------------------
EXTERN INT32 IRRC_RegCallbackFunc(PFN_IRRXCB_THREAD_T pfnCallback);

//-----------------------------------------------------------------------------
/**  IRRX_BtnSendKey() function.
 *  @param pfnCallback the IRRC callback function.
 */
//-----------------------------------------------------------------------------
EXTERN void IRRX_BtnSendKey(UINT32 u4Key);
	
//-----------------------------------------------------------------------------
/**  _IRRC_GetCurrentIrRawData() function.
 *	@param pIrRawData: point to IrRawData variable.
 */
//-----------------------------------------------------------------------------

EXTERN INT32 _IRRC_GetCurrentIrRawData(UINT32 * pu4IrRawData, BOOL *bRepeat);
EXTERN void _BTN_IsSystemCodeAllowed(UINT32 u4System);	
#ifdef CHANNEL_CHANGE_LOG
EXTERN void IRRX_GetPressTime(HAL_TIME_T* pTime);
EXTERN void IRRX_RstPressTime(void);
#endif
#endif /* DRV_IR_H */

