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
 * $RCSfile: rtc_hw.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         MT53xx built-in RTC hardware driver implementation.
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
#include "hw_pdwnc.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_pdwnc_mt82.h"
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
#ifdef CC_MT8223
#define RTC_READ_POLLING() \
    do{ \
        vIO32WriteFldAlign(PDWNC_ASYNC_RD, 0x1, FLD_ASYNC_RD);\
        vIO32WriteFldAlign(PDWNC_ASYNC_RD, 0x0, FLD_ASYNC_RD);\
        x_thread_delay(5);\
    } while(0)

#endif

//----------------------------------------------------------------------------
// Prototypes
//----------------------------------------------------------------------------
static BOOL _RTC_Init(void);
#ifndef CC_UP8032_ATV
static BOOL _RTC_GetTimeDate(UINT64* prTime);
static BOOL _RTC_SetTimeDate(const UINT64* prTime);
static BOOL _RTC_SetAlertTime(UINT64 u8Utc);
static BOOL _RTC_SetRtc(RTC_T rRtc);
#else
BOOL _RTC_SetRtc(RTC_T rRtc);
#endif
static void _RTC_HookAlertHandler(PFN_RTC_ALERT_HANDLER_T pfnHandler);
static BOOL _RTC_GetRtc(RTC_T* prRtc);

void _ReadTimeDate(RTC_T* prRtc);

//----------------------------------------------------------------------------
// Constant definitions
//----------------------------------------------------------------------------
#ifdef CC_UP8032_ATV
typedef UINT32  SIZE_T;
#endif

//----------------------------------------------------------------------------
// Static variables
//----------------------------------------------------------------------------
#ifdef CC_UP8032_ATV
static UINT8 _fgRTCInit = FALSE;
static UINT8 _fgRTCDisableRTC = FALSE;
#else
static BOOL _fgRTCInit = FALSE;
static BOOL _fgRTCDisableRTC = FALSE;
#endif
static PFN_RTC_ALERT_HANDLER_T _pfnRTCHookHandler = NULL;
static RTC_FuncTbl _RTC_FuncTbl =
{
#ifdef CC_UP8032_ATV
    _RTC_Init,
    NULL, //_RTC_ResetAlert,    
    NULL,//	    _RTC_HookAlertHandler,
    NULL,
    _RTC_GetRtc,
    _RTC_SetRtc,    
#else
    _RTC_Init,
    _RTC_GetTimeDate,
    _RTC_SetTimeDate,
    NULL, //_RTC_GetAlertTime,
    _RTC_SetAlertTime,    
    NULL, //_RTC_ResetAlert,    
    _RTC_HookAlertHandler,
    NULL,
    _RTC_GetRtc,
    _RTC_SetRtc,    
#endif
} ;

//----------------------------------------------------------------------------
// Private functions
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/** _RTCAlertIsr() RTC interrupt routine.
 */
//----------------------------------------------------------------------------
static void _AlertIsr(UINT16 u2VctId)
{
    UNUSED(u2VctId);
    if (_pfnRTCHookHandler)
    {
        _pfnRTCHookHandler();
    }
    //VERIFY(BIM_ClearIrq(VECTOR_RTCAL));
}

//----------------------------------------------------------------------------
/** _ReadTimeDate() RTC interrupt routine.
 */
//----------------------------------------------------------------------------
void _ReadTimeDate(RTC_T* prRtc)
{
    VERIFY(prRtc != NULL);
#ifdef CC_MT8223
	RTC_READ_POLLING();
#endif
    prRtc->u1Second = (UINT8)u4IO32Read4B(PDWNC_SECOND);//PDWNC_READ8(REG_RW_RTC_SEC);
    prRtc->u1Minute =  (UINT8)u4IO32Read4B(PDWNC_MINUTE);//PDWNC_READ8(REG_RW_RTC_MIN);    
    prRtc->u1Hour = (UINT8)u4IO32Read4B(PDWNC_HOUR);//PDWNC_READ8(REG_RW_RTC_HOUR);
    prRtc->u1Day = (UINT8)u4IO32Read4B(PDWNC_DAY);//PDWNC_READ8(REG_RW_RTC_DAY);
    prRtc->u1Month = (UINT8)u4IO32Read4B(PDWNC_MONTH);//PDWNC_READ8(REG_RW_RTC_MONTH);
    prRtc->u1Year = (UINT8)u4IO32Read4B(PDWNC_YEAR);//PDWNC_READ8(REG_RW_RTC_YEAR);
    prRtc->u1Week = (UINT8)u4IO32Read4B(PDWNC_WEEK);//PDWNC_READ8(REG_RW_RTC_WEEK);   
//    Printf("--------_ReadTimeDate------\nyear= %04d\nMonth= %02d\nday=%02d\n",
//		prRtc->u1Year,prRtc->u1Month,prRtc->u1Day);
}

//----------------------------------------------------------------------------
// Public functions
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/** RTC_IsAllRegsBcd() Check all RTC register in BCD format.
 *  @retval FALSE - not all BCD, TRUE - all BCD.
 */
//----------------------------------------------------------------------------
static BOOL RTC_IsAllRegsBcd(void)
{
    UINT8 u1Bcd;

    u1Bcd = (UINT8)u4IO32Read4B(PDWNC_YEAR);//PDWNC_READ8(REG_RW_RTC_YEAR);
    if (((u1Bcd >> 4) > 9) || ((u1Bcd & 0xf) > 9)) { return FALSE; }  
    u1Bcd = (UINT8)u4IO32Read4B(PDWNC_MONTH);//PDWNC_READ8(REG_RW_RTC_MONTH);
    if (((u1Bcd >> 4) > 9) || ((u1Bcd & 0xf) > 9)) { return FALSE; }
    u1Bcd = (UINT8)u4IO32Read4B(PDWNC_DAY);//PDWNC_READ8(REG_RW_RTC_DAY);
    if (((u1Bcd >> 4) > 9) || ((u1Bcd & 0xf) > 9)) { return FALSE; }
    u1Bcd = (UINT8)u4IO32Read4B(PDWNC_HOUR);//PDWNC_READ8(REG_RW_RTC_HOUR);
    if (((u1Bcd >> 4) > 9) || ((u1Bcd & 0xf) > 9)) { return FALSE; }
    u1Bcd = (UINT8)u4IO32Read4B(PDWNC_MINUTE);//PDWNC_READ8(REG_RW_RTC_MIN);
    if (((u1Bcd >> 4) > 9) || ((u1Bcd & 0xf) > 9)) { return FALSE; }
    u1Bcd = (UINT8)u4IO32Read4B(PDWNC_SECOND);//PDWNC_READ8(REG_RW_RTC_SEC);
    if (((u1Bcd >> 4) > 9) || ((u1Bcd & 0xf) > 9)) { return FALSE; }

    return TRUE;
}

//----------------------------------------------------------------------------
/** RTC_Init() Initialize RTC driver and hardware
 *  @retval TRUE, Succeed; FALSE, Fail.
 */
//----------------------------------------------------------------------------
static BOOL _RTC_Init(void)
{
    CRIT_STATE_T rCs;
    //UINT32 u4Ctrl;

    rCs = x_crit_start();

    if (_fgRTCInit)
    {
        x_crit_end(rCs);
    
        // Already initialized
        return TRUE;
    }

    // Pass write protection
    vIO32WriteFldAlign(PDWNC_PROT, RTC_WRITE_PROTECT_CODE1, FLD_PROT);//PDWNC_WRITE32(REG_RW_RTC_PROT, RTC_WRITE_PROTECT_CODE1);
    vIO32WriteFldAlign(PDWNC_PROT, RTC_WRITE_PROTECT_CODE2, FLD_PROT);

/*    // Check RTC flags
    u4Ctrl = PDWNC_READ32(REG_RW_RTC_CTRL);
    
    if ((u4Ctrl & RTCTRL_PASS) == 0)
    {
        _fgRTCDisableRTC = TRUE;
        
        // Enable write protection
        PDWNC_WRITE32(REG_RW_RTC_PROT, 0x0);
        
        x_crit_end(rCs);        
        
        Printf("Swap on RTC.\n");
        return TRUE;
    }
*/
#ifdef CC_MT8223
	RTC_READ_POLLING();
#endif

    if(IO32ReadFldAlign(PDWNC_RTCCTL, FLD_PASS) == 0)
    {
    
        _fgRTCDisableRTC = TRUE;
        // Enable write protection
        vIO32WriteFldAlign(PDWNC_PROT, 0x0, FLD_PROT);
        
        x_crit_end(rCs);        
        
        Printf("Swap on RTC.\n");
        return TRUE;
    }

 /*   
    u4Ctrl = RTCTRL_H24 | RTCTRL_PASS;//0x100;
    PDWNC_WRITE32(REG_RW_RTC_CTRL, u4Ctrl);    
*/
    vIO32WriteFldAlign(PDWNC_RTCCTL, 1, FLD_H24);
    vIO32WriteFldAlign(PDWNC_RTCCTL, 1, FLD_PASS);
    // In 5371, do not need to Check RTC key
#ifndef CC_UP8032_ATV
    if (!RTC_IsAllRegsBcd() || !PDWNC_IsBootByWakeup())                                               // 537X
    {      
        UINT64 u8Utc = UTC_INIT_TIME;
		//Printf("-----_RTC_Init UINT64 u8Utc = UTC_INIT_TIME\n");

        if(!_RTC_SetTimeDate(&u8Utc))
        {
            // Enable write protection
            vIO32WriteFldAlign(PDWNC_PROT, 0x0, FLD_PROT);//PDWNC_WRITE32(REG_RW_RTC_PROT, 0x0);
            
            x_crit_end(rCs);
            
            return FALSE;
        }
    }
#endif
    // Enable write protection
        vIO32WriteFldAlign(PDWNC_PROT, 0x0, FLD_PROT);//PDWNC_WRITE32(REG_RW_RTC_PROT, 0x0);

    _fgRTCInit = TRUE;
    
    
    x_crit_end(rCs);
    
    return TRUE;
}
#ifndef CC_UP8032_ATV
//----------------------------------------------------------------------------
/** RTC_GetTimeDate() Get time from RTC
 *  @param prTime, Output, The time
 *  @retval TRUE, Succeed; FALSE, Fail.
 */
//----------------------------------------------------------------------------
static BOOL _RTC_GetTimeDate(UINT64* prTime)
{
    CRIT_STATE_T rCs;
    RTC_T rRtc, rRtc2;
    UINT64 u8Utc;

    VERIFY(prTime!=NULL);

    rCs = x_crit_start();
   //Printf("_RTC_GetTimeDate->_fgRTCDisableRTC=%d\n",_fgRTCDisableRTC);

    if (_fgRTCDisableRTC)
    {
        *prTime = 0;
        
        x_crit_end(rCs);
        
        return FALSE;
    }

    // Get time in second from RTC   
    // If two consecutive read RTC values have difference in min, hr, day, mon, yr,
    // It indicates carry occurs upon RTC read and the read value is not reliable,
    // thus we perform another two consecutive read
    do
    {
        _ReadTimeDate(&rRtc);
        _ReadTimeDate(&rRtc2);   
    }
    while((rRtc.u1Year != rRtc2.u1Year) ||
    	(rRtc.u1Month != rRtc2.u1Month) ||
    	(rRtc.u1Day != rRtc2.u1Day) ||
    	(rRtc.u1Hour != rRtc2.u1Hour) ||
    	(rRtc.u1Minute != rRtc2.u1Minute));

    rRtc.u1Year = _RTC_BcdToBinary(rRtc2.u1Year);
    rRtc.u1Month = _RTC_BcdToBinary(rRtc2.u1Month);
    rRtc.u1Day = _RTC_BcdToBinary(rRtc2.u1Day);
    rRtc.u1Hour = _RTC_BcdToBinary(rRtc2.u1Hour);
    rRtc.u1Minute = _RTC_BcdToBinary(rRtc2.u1Minute);
    rRtc.u1Second = _RTC_BcdToBinary(rRtc2.u1Second);

    x_crit_end(rCs);

    // Translate to TIME_T(UINT64)
    if (_RTC_RtcToUtc(&u8Utc, &rRtc))
    {
        *prTime = (UINT64)u8Utc;       
        return TRUE;
    }

    *prTime = 0;    
    return FALSE;
}


//----------------------------------------------------------------------------
/** RTC_SetTimeDate() Set time to RTC
 *  @param prTime, Input, The time to set.
 *  @retval TRUE, Succeed; FALSE, Fail.
 */
//----------------------------------------------------------------------------
static BOOL _RTC_SetTimeDate(const UINT64* prTime)
{
    RTC_T rRtc;
    UINT64 u8Utc;
    CRIT_STATE_T rCs;
    
    VERIFY(prTime!=NULL);
    // Translate to RTC
    u8Utc = (UINT64)*prTime;
    if (!_RTC_UtcToRtc(&rRtc, u8Utc))
    {
        return FALSE;
    }

    rCs = x_crit_start();
/*
    // Pass write protection
    PDWNC_WRITE32(REG_RW_RTC_PROT, RTC_WRITE_PROTECT_CODE1);
    PDWNC_WRITE32(REG_RW_RTC_PROT, RTC_WRITE_PROTECT_CODE2);
*/
    vIO32WriteFldAlign(PDWNC_PROT, RTC_WRITE_PROTECT_CODE1, FLD_PROT);//PDWNC_WRITE32(REG_RW_RTC_PROT, RTC_WRITE_PROTECT_CODE1);
    vIO32WriteFldAlign(PDWNC_PROT, RTC_WRITE_PROTECT_CODE2, FLD_PROT);

#ifdef CC_MT8223
		RTC_READ_POLLING();
#endif
//Printf("_RTC_SetTimeDate->_fgRTCDisableRTC = %d\n",_fgRTCDisableRTC);
    if (!_fgRTCDisableRTC)
    {
        
        // Set H24 and Stop mode
        // RTC stop has problem in 538x
        //PDWNC_WRITE32(REG_RW_RTC_CTRL, (RTCTRL_STOP | RTCTRL_H24));
        vIO32WriteFldAlign(PDWNC_RTCCTL, 0x1, FLD_H24);
        vIO32WriteFldAlign(PDWNC_RTCCTL, 0x1, FLD_STOP);		
        // Set to RTC

        if (rRtc.u1Second != 59)
        {
            vIO32Write4B(PDWNC_SECOND, _RTC_BinaryToBcd(rRtc.u1Second));//PDWNC_WRITE8(REG_RW_RTC_SEC, _RTC_BinaryToBcd(rRtc.u1Second));
        }
        else
        {
            vIO32Write4B(PDWNC_SECOND, 0x00);//PDWNC_WRITE8(REG_RW_RTC_SEC, 0x00);
        }

        vIO32Write4B(PDWNC_YEAR, _RTC_BinaryToBcd(rRtc.u1Year));//PDWNC_WRITE8(REG_RW_RTC_YEAR, _RTC_BinaryToBcd(rRtc.u1Year));
        vIO32Write4B(PDWNC_MONTH, _RTC_BinaryToBcd(rRtc.u1Month));
        vIO32Write4B(PDWNC_DAY, _RTC_BinaryToBcd(rRtc.u1Day));
        vIO32Write4B(PDWNC_HOUR, _RTC_BinaryToBcd(rRtc.u1Hour));
        vIO32Write4B(PDWNC_MINUTE, _RTC_BinaryToBcd(rRtc.u1Minute));
		vIO32Write4B(PDWNC_LEAPYEAR, _RTC_BinaryToBcd(rRtc.u1Year%4));


        if (rRtc.u1Second == 59)
        {
            vIO32Write4B(PDWNC_SECOND, 0x59);//PDWNC_WRITE8(REG_RW_RTC_SEC, 0x59);
        }
        
        // Disable RTC STOP
        vIO32WriteFldAlign(PDWNC_RTCCTL, 0x0, FLD_STOP);		
        // RTC stop has problem in 538x
        //PDWNC_WRITE32(REG_RW_RTC_CTRL, RTCTRL_H24);

    }

    // Enable write protection
    vIO32WriteFldAlign(PDWNC_PROT, 0x0, FLD_PROT);//PDWNC_WRITE32(REG_RW_RTC_PROT, 0x0);
    
    x_crit_end(rCs);
        
    return TRUE;
}

#endif
//----------------------------------------------------------------------------
/** RTC_GetTimeDate() Get time from RTC
 *  @param prTime, Output, The time
 *  @retval TRUE, Succeed; FALSE, Fail.
 */
//----------------------------------------------------------------------------
static BOOL _RTC_GetRtc(RTC_T *prRtc)
{
    CRIT_STATE_T rCs;
    RTC_T rRtc, rRtc2;
//    UINT64 u8Utc;

    VERIFY(prRtc!=NULL);

    rCs = x_crit_start();
    if (_fgRTCDisableRTC)
    {
        
        x_crit_end(rCs);
        
        return FALSE;
    }


    // Get time in second from RTC   
    // If two consecutive read RTC values have difference in min, hr, day, mon, yr,
    // It indicates carry occurs upon RTC read and the read value is not reliable,
    // thus we perform another two consecutive read
    do
    {
        _ReadTimeDate(&rRtc);
        _ReadTimeDate(&rRtc2);   
    }
    while((rRtc.u1Year != rRtc2.u1Year) ||
    	(rRtc.u1Month != rRtc2.u1Month) ||
    	(rRtc.u1Day != rRtc2.u1Day) ||
    	(rRtc.u1Hour != rRtc2.u1Hour) ||
    	(rRtc.u1Minute != rRtc2.u1Minute));

    prRtc->u1Week = _RTC_BcdToBinary(rRtc2.u1Week);
    prRtc->u1Year = _RTC_BcdToBinary(rRtc2.u1Year);
    prRtc->u1Month = _RTC_BcdToBinary(rRtc2.u1Month);
    prRtc->u1Day = _RTC_BcdToBinary(rRtc2.u1Day);
    prRtc->u1Hour = _RTC_BcdToBinary(rRtc2.u1Hour);
    prRtc->u1Minute = _RTC_BcdToBinary(rRtc2.u1Minute);
    prRtc->u1Second = _RTC_BcdToBinary(rRtc2.u1Second);

    x_crit_end(rCs);
  
    return TRUE;
}


//----------------------------------------------------------------------------
/** RTC_SetTimeDate() Set time to RTC
 *  @param prTime, Input, The time to set.
 *  @retval TRUE, Succeed; FALSE, Fail.
 */
//----------------------------------------------------------------------------
#ifdef CC_UP8032_ATV
BOOL _RTC_SetRtc(RTC_T rRtc)
#else
static BOOL _RTC_SetRtc(RTC_T rRtc)
#endif
{
    //UINT64 u8Utc;
    CRIT_STATE_T rCs;
    rCs = x_crit_start();

    // Pass write protection
    vIO32WriteFldAlign(PDWNC_PROT, RTC_WRITE_PROTECT_CODE1, FLD_PROT);//PDWNC_WRITE32(REG_RW_RTC_PROT, RTC_WRITE_PROTECT_CODE1);
    vIO32WriteFldAlign(PDWNC_PROT, RTC_WRITE_PROTECT_CODE2, FLD_PROT);

/*    
    PDWNC_WRITE32(REG_RW_RTC_PROT, RTC_WRITE_PROTECT_CODE1);
    PDWNC_WRITE32(REG_RW_RTC_PROT, RTC_WRITE_PROTECT_CODE2);
*/    
#ifdef CC_MT8223
		RTC_READ_POLLING();
#endif

    if (!_fgRTCDisableRTC)
    {
        
        // Set H24 and Stop mode
        // RTC stop has problem in 538x
        //PDWNC_WRITE32(REG_RW_RTC_CTRL, (RTCTRL_STOP | RTCTRL_H24));
        
        // Set to RTC

        if (rRtc.u1Second != 59)
        {
             vIO32Write4B(PDWNC_SECOND,  _RTC_BinaryToBcd(rRtc.u1Second));//PDWNC_WRITE8(REG_RW_RTC_SEC, _RTC_BinaryToBcd(rRtc.u1Second));
        }
        else
        {
             vIO32Write4B(PDWNC_SECOND, 0x00);//PDWNC_WRITE8(REG_RW_RTC_SEC, 0x00);
        }
/*
        PDWNC_WRITE8(REG_RW_RTC_WEEK, _RTC_BinaryToBcd(rRtc.u1Week));
        PDWNC_WRITE8(REG_RW_RTC_YEAR, _RTC_BinaryToBcd(rRtc.u1Year));
        PDWNC_WRITE8(REG_RW_RTC_MONTH, _RTC_BinaryToBcd(rRtc.u1Month));
        PDWNC_WRITE8(REG_RW_RTC_DAY, _RTC_BinaryToBcd(rRtc.u1Day));
        PDWNC_WRITE8(REG_RW_RTC_HOUR, _RTC_BinaryToBcd(rRtc.u1Hour));
        PDWNC_WRITE8(REG_RW_RTC_MIN, _RTC_BinaryToBcd(rRtc.u1Minute));
*/
        vIO32Write4B(PDWNC_WEEK, _RTC_BinaryToBcd(rRtc.u1Week));
        vIO32Write4B(PDWNC_YEAR, _RTC_BinaryToBcd(rRtc.u1Year));
        vIO32Write4B(PDWNC_MONTH, _RTC_BinaryToBcd(rRtc.u1Month));
        vIO32Write4B(PDWNC_DAY, _RTC_BinaryToBcd(rRtc.u1Day));
        vIO32Write4B(PDWNC_HOUR, _RTC_BinaryToBcd(rRtc.u1Hour));
        vIO32Write4B(PDWNC_MINUTE, _RTC_BinaryToBcd(rRtc.u1Minute));


        if (rRtc.u1Second == 59)
        {
            vIO32Write4B(PDWNC_SECOND, 0x59);//PDWNC_WRITE8(REG_RW_RTC_SEC, 0x59);
        }
        
        // Disable RTC STOP

        // RTC stop has problem in 538x
        //PDWNC_WRITE32(REG_RW_RTC_CTRL, RTCTRL_H24);

    }

    // Enable write protection
    vIO32WriteFldAlign(PDWNC_PROT, 0x0, FLD_PROT);//PDWNC_WRITE32(REG_RW_RTC_PROT, 0x0);
    
    x_crit_end(rCs);
        
    return TRUE;
}

#ifndef CC_UP8032_ATV
//----------------------------------------------------------------------------
/** RTC_SetAlertTime() Set time to RTC
 *  @param prTime, Input, The time to set.
 *  @retval TRUE, Succeed; FALSE, Fail.
 */
//----------------------------------------------------------------------------
static BOOL _RTC_SetAlertTime(UINT64 u8Utc)
{
    CRIT_STATE_T rCs;
    RTC_T rRtc;

    if (!_RTC_UtcToRtc(&rRtc, u8Utc))
    {        
        Printf("SetAlertTime failed.\n");
        return FALSE;
    }

    rCs = x_crit_start();    

    // Pass write protection
/*    
    PDWNC_WRITE32(REG_RW_RTC_PROT, RTC_WRITE_PROTECT_CODE1);
    PDWNC_WRITE32(REG_RW_RTC_PROT, RTC_WRITE_PROTECT_CODE2);
*/
    vIO32WriteFldAlign(PDWNC_PROT, RTC_WRITE_PROTECT_CODE1, FLD_PROT);//PDWNC_WRITE32(REG_RW_RTC_PROT, RTC_WRITE_PROTECT_CODE1);
    vIO32WriteFldAlign(PDWNC_PROT, RTC_WRITE_PROTECT_CODE2, FLD_PROT);
    // Set H24 and Stop mode
//    PDWNC_WRITE32(REG_RW_RTC_CTRL, (RTCTRL_STOP | RTCTRL_H24));
    // Set to RTC
/*    
    PDWNC_WRITE8(REG_RW_RTC_AYEAR, _RTC_BinaryToBcd(rRtc.u1Year));
    PDWNC_WRITE8(REG_RW_RTC_AMONTH, _RTC_BinaryToBcd(rRtc.u1Month));
    PDWNC_WRITE8(REG_RW_RTC_ADAY, _RTC_BinaryToBcd(rRtc.u1Day));
    PDWNC_WRITE8(REG_RW_RTC_AHOUR, _RTC_BinaryToBcd(rRtc.u1Hour));
    PDWNC_WRITE8(REG_RW_RTC_AMIN, _RTC_BinaryToBcd(rRtc.u1Minute));
    PDWNC_WRITE8(REG_RW_RTC_ASEC, _RTC_BinaryToBcd(rRtc.u1Second));
*/
#ifdef CC_MT8223
			RTC_READ_POLLING();
#endif

    vIO32Write4B(PDWNC_AYEAR, _RTC_BinaryToBcd(rRtc.u1Year));
    vIO32Write4B(PDWNC_AMONTH, _RTC_BinaryToBcd(rRtc.u1Month));
    vIO32Write4B(PDWNC_ADAY, _RTC_BinaryToBcd(rRtc.u1Day));
    vIO32Write4B(PDWNC_AHOUR, _RTC_BinaryToBcd(rRtc.u1Hour));
    vIO32Write4B(PDWNC_AMINUTE, _RTC_BinaryToBcd(rRtc.u1Minute));
    vIO32Write4B(PDWNC_ASECOND, _RTC_BinaryToBcd(rRtc.u1Second));
    // Set Alarm configuration, disable Week match in alarm.
    vIO32Write4B(PDWNC_ALMINT, ((RTCALM_FULL_EN & (~(RTCALM_WEEK_EN))) | RTCALM_INTR_EN));//PDWNC_WRITE32(REG_RW_RTC_ALMINT, ((RTCALM_FULL_EN & (~(RTCALM_WEEK_EN))) | RTCALM_INTR_EN));
    // Disable RTC STOP
//    PDWNC_WRITE32(REG_RW_RTC_CTRL, RTCTRL_H24);

    // Enable write protection
    vIO32WriteFldAlign(PDWNC_PROT, 0x0, FLD_PROT);//PDWNC_WRITE32(REG_RW_RTC_PROT, 0x0);
    
    x_crit_end(rCs);  

    return TRUE;
}
#endif
//----------------------------------------------------------------------------
/** RTC_HookAlertHandler()
 *  @param pfnHandler the callback function.
 */
//----------------------------------------------------------------------------
static void _RTC_HookAlertHandler(PFN_RTC_ALERT_HANDLER_T pfnHandler)
{
    INT32 i4Val;
    void (* pfnOldIsr)(UINT16);

    _pfnRTCHookHandler = pfnHandler;
    if (pfnHandler==NULL)
    {
        //i4Val = x_reg_isr(VECTOR_RTCAL, NULL, &pfnOldIsr);
        i4Val = PDWNC_RegIsr((UINT16)/*PINTNO_RTC*/PDWNC_INTNO_RTC, NULL, &pfnOldIsr);                
    }
    else
    {
        //i4Val = x_reg_isr(VECTOR_RTCAL, _AlertIsr, &pfnOldIsr);
        i4Val = PDWNC_RegIsr((UINT16)/*PINTNO_RTC*/PDWNC_INTNO_RTC, _AlertIsr, &pfnOldIsr);        
    }
    if (i4Val != OSR_OK)
    {
        Printf("Hook RTC Alert failed\n");
    }
    //VERIFY(BIM_ClearIrq(VECTOR_RTCAL));
}

//----------------------------------------------------------------------------
/** RTC_HookAlertHandler()
 *  @param pfnHandler the callback function.
 */
//----------------------------------------------------------------------------
RTC_FuncTbl* _RTC_GetFunc(void)
{
    return &_RTC_FuncTbl;
}


