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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: rtc_ext.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         External RTC hardware independent implementation.
 *---------------------------------------------------------------------------*/

//----------------------------------------------------------------------------
// Constant definitions
//----------------------------------------------------------------------------
#include "x_lint.h"
 LINT_EXT_HEADER_BEGIN
#include "x_os.h"

#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#include "x_bim_mt82.h"
#include "x_ckgen_mt82.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_pdwnc_mt82.h"
#include "hw_pdwnc.h"
#else
#include "x_bim.h"
#include "x_ckgen.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_pdwnc.h"
#endif
 LINT_EXT_HEADER_END
 
#include "drvcust_if.h"
#include "rtc_sw.h"
#include "rtc_hw.h"
#include "rtc_ext.h"

#ifdef CC_UP8032_ATV
typedef UINT32  SIZE_T;
#endif

#define MAX_RTCEVENT_COUNT			 32

static PFN_RTC_ALERT_HANDLER_T _pfnRTCHookHandler = NULL;
static HANDLE_T _hRTCEventHandler;
static HANDLE_T _hRTCEventMsgq;
#ifdef CC_UP8032_ATV
static UINT8 _fgAlertHit = FALSE;
static UINT8 _fgRTCInitCommon = FALSE;
static UINT8 _fgInitFail = FALSE;
#else
static BOOL _fgAlertHit = FALSE;
static BOOL _fgRTCInitCommon = FALSE;
static BOOL _fgInitFail = FALSE;
#endif

static RTC_FuncTbl* _prFunc;

#ifdef CC_UP8032_ATV
RTC_FuncTbl* _RTC_EXT_GetFunc(void)
{
    return NULL;
}
#else
EXTERN RTC_FuncTbl* _RTC_EXT_GetFunc(void);
#endif

//----------------------------------------------------------------------------
/** RTC_GetAlertTime() Get time from RTC
 *  @param prTime, Output, The time
 *  @retval TRUE, Succeed; FALSE, Fail.
 */
//----------------------------------------------------------------------------
static void _ResetAlert(void)
{

}

//----------------------------------------------------------------------------
/** _RTCEventHandler() RTC interrupt event handler.
 */
//----------------------------------------------------------------------------
static void _EventHandler(void)
{    
#ifndef CC_UP8032_ATV    
    UINT16 u2MsgIdx;
    SIZE_T u4Size = sizeof(RTC_MSG_T);
    RTC_MSG_T rMsg;
    while(!_fgInitFail)
    {        
        if(x_msg_q_receive(&u2MsgIdx, (void*)&rMsg, &u4Size, &_hRTCEventMsgq, 1, X_MSGQ_OPTION_WAIT) == OSR_OK)
        {
            if(_pfnRTCHookHandler != NULL)
            {
                _fgAlertHit = FALSE;
                _prFunc->pfnResetAlert();
                _pfnRTCHookHandler();
            }
        }
    }
#endif
}

//----------------------------------------------------------------------------
/** _RTCAlertIsr() RTC interrupt routine.
 */
//----------------------------------------------------------------------------
static void _AlertIsr(INT32 i4Gpio, BOOL fgStatus)
{
    RTC_MSG_T rMsg;      

    UNUSED(i4Gpio);
    UNUSED(fgStatus);
    if (_pfnRTCHookHandler && !_fgAlertHit)
    {
        _fgAlertHit = TRUE;
        rMsg.u4Dummy = 0;
#ifndef CC_UP8032_ATV    
        VERIFY(x_msg_q_send(_hRTCEventMsgq, (void *)&rMsg, sizeof(RTC_MSG_T), 254) == OSR_OK);
#else
	_hRTCEventMsgq = rMsg.u4Dummy;
#endif
        UNUSED(rMsg);
    }
}

static void _SetWakeup(UINT32* pu4PowerDownEn)
{
    UINT32 i4Gpio;
    //UINT32 u4Tmp;
    ASSERT(pu4PowerDownEn != NULL);
    if( DRVCUST_OptQuery(eRtcExtIntGpio, &i4Gpio) < 0)
    {
        return;
    }
        //david: why overwrite pu4PowerDownEn?
/*    
    // Register wake-up GPIO
    *pu4PowerDownEn = PDWNC_READ32(REG_RW_PWDIOCFG);
    *pu4PowerDownEn &= ~(GPIOX_POL(i4Gpio - 200));
    PDWNC_WRITE32(REG_RW_PWDIOCFG, *pu4PowerDownEn);

    *pu4PowerDownEn = PDWNC_READ32(REG_RW_PDWAKE_EN);
    *pu4PowerDownEn |= (GPIO_WAKEN(WAK_GPIOX(i4Gpio - 200)));
    PDWNC_WRITE32(REG_RW_PDWAKE_EN, *pu4PowerDownEn);  
*/
    vIO32Write4B(PDWNC_PDIO, u4IO32Read4B(PDWNC_PDIO) & (~(GPIOX_POL(i4Gpio - OPCTRL(0)))));
    vIO32Write4B(PDWNC_WAKEN, (*pu4PowerDownEn) | (GPIO_WAKEN(WAK_GPIOX(i4Gpio - OPCTRL(0)))));
}

//----------------------------------------------------------------------------
/** RTC_HookAlertHandler()
 *  @param pfnHandler the callback function.
 */
//----------------------------------------------------------------------------
static void _HookAlertHandler(PFN_RTC_ALERT_HANDLER_T pfnHandler)
{
    INT32 i4Gpio;
    
    if(DRVCUST_OptQuery(eRtcExtIntGpio, (UINT32*)&i4Gpio) < 0)
    {
        return;
    }
    
    _pfnRTCHookHandler = pfnHandler;
    if (pfnHandler==NULL)
    {
        // Nothing to hook...
    }
    else
    {
	#ifndef CC_UP8032_ATV    
        VERIFY(0 <= GPIO_Reg((INT32)i4Gpio, GPIO_TYPE_INTR_FALL, _AlertIsr));
	#endif
    }
}

BOOL _RTC_EXT_Init(void)
{

    if (_fgRTCInitCommon)
    {
        // Already initialized    
        return TRUE;
    }
    
    _prFunc = _RTC_EXT_GetFunc();
    
    if(_prFunc == NULL)
    {
        return FALSE;
    }

    _fgRTCInitCommon = TRUE;        
    UNUSED(_hRTCEventHandler);
    UNUSED(_hRTCEventMsgq);
    
#ifndef CC_UP8032_ATV    
    VERIFY(x_msg_q_create(&_hRTCEventMsgq, "RTCMSGQ", sizeof(RTC_MSG_T), MAX_RTCEVENT_COUNT) == OSR_OK);
    if(x_thread_create(&_hRTCEventHandler, "RTCHDL", 1024, 100, (x_os_thread_main_fct)_EventHandler, 0, NULL) != OSR_OK)
    {
        Printf("Thread create error!!\n");
        VERIFY(0);
    }
   
    if ((_prFunc->pfnInit == NULL) || (!_prFunc->pfnInit()))
    {
        UNUSED(x_msg_q_delete(_hRTCEventMsgq));
        _fgInitFail = TRUE;
        return FALSE;
    }
#endif   
    if(_prFunc->pfnResetAlert == NULL)
    {
        _prFunc->pfnResetAlert = _ResetAlert;
    }	
    if(_prFunc->pfnSetWakeup == NULL)
    {
        _prFunc->pfnSetWakeup = _SetWakeup;
    }
    if(_prFunc->pfnHookAlertHandler == NULL)
    {
        _prFunc->pfnHookAlertHandler = _HookAlertHandler;
    }

    return TRUE;
}
