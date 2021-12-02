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
 * $RCSfile: rtc_if.h,v $
 * $Revision: #1 $
 *
 * Description:
 *         Header file of RTC related interface.
 *---------------------------------------------------------------------------*/

/** @file rtc_if.h
 *  Header file of RTC related interface.
 */
 
#ifndef _RTC_IF_H_
#define _RTC_IF_H_

//----------------------------------------------------------------------------
// Constant definitions
//----------------------------------------------------------------------------
#include "x_typedef.h"
#include "sif_if.h"

//----------------------------------------------------------------------------
// Config
//----------------------------------------------------------------------------
#define ENABLE_UTC_BOUNDARY             1               ///< Flag to enable UTC boundary.

//----------------------------------------------------------------------------
// Constant definitions
//----------------------------------------------------------------------------
#define MAX_COUNT_YEAR                  99              ///< Max year count.

#if ENABLE_UTC_BOUNDARY
// UTC bounds
#define UTC_LOWER_BOUND                 946656000       ///< UTC lower bound: Jan  1 00:00:00, 2000.
//#define UTC_UPPER_BOUND                 2147483640  // Jan 19 11:14:00, 2038
#define UTC_UPPER_BOUND                 0xffffffffU     ///< UTC upper bound.
#define UTC_INIT_TIME                   946742400       ///< UTC initial time.
#endif /* ENABLE_UTC_BOUNDARY */

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/** RTC alarm handler function pointer 
 */
typedef void (*PFN_RTC_ALERT_HANDLER_T)(void);

/** RTC alram reset function pointer 
 */
typedef void (*PFN_RTC_ALERT_RESET_T)(void);

/** RTC date time structure
 */
typedef struct
{
    UINT8   u1Year;
    UINT8   u1Month;
    UINT8   u1Day;
    UINT8   u1Hour;
    UINT8   u1Minute;
    UINT8   u1Second;
    UINT8   u1Week;        
} RTC_T;

/** RTC function table
 */
typedef struct {
    BOOL (*pfnInit)(void);                                  ///< Pointer to initialization funtion.  
    BOOL (*pfnGetTimeDate)(UINT64*);                        ///< Pointer to get time funciton.            
    BOOL (*pfnSetTimeDate)(const UINT64*);                  ///< Pointer to set time function                                
    BOOL (*pfnGetAlertTime)(UINT64*);                       ///< Pointer to get alert time function
    BOOL (*pfnSetAlertTime)(UINT64);                        ///< Pointer to set alert time function    
    void (*pfnResetAlert)(void);                            ///< Pointer to alert reset function    
    void (*pfnHookAlertHandler)(PFN_RTC_ALERT_HANDLER_T);   ///< Pointer to alert handler hooking function. It is used to install alert handler.
    void (*pfnSetWakeup)(UINT32*);                          ///< Pointer to wakeup set function. It is called upon RTC wakeup enable
    BOOL (*pfnGetRtc)(RTC_T*);    
    BOOL (*pfnSetRtc)(RTC_T);       
} RTC_FuncTbl;

//----------------------------------------------------------------------------
// Static variables
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Public interface
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/** RTC_BcdToBinary
 *  Convert a BCD to a binary
 *  @param u1Bcd The BCD to be converted
 *  @retval The converted binary
 */
//----------------------------------------------------------------------------
EXTERN UINT8 RTC_BcdToBinary(UINT8 u1Bcd);

//----------------------------------------------------------------------------
/** RTC_BinaryToBcd
 *  Convert a binary to a BCD
 *  @param u1Binary The binary to be converted
 *  @retval The converted BCD
 */
//----------------------------------------------------------------------------
EXTERN UINT8 RTC_BinaryToBcd(UINT8 u1Binary);

//----------------------------------------------------------------------------
/** RTC_RtcToUtc
 *  Translate RTC to UTC
 *  @param      prRtc   The RTC to be translated
 *  @param      pu8Utc  The translated UTC
 *  @retval     TRUE    Succeed
 *  @retval     FALSE   Fail
 */
//----------------------------------------------------------------------------
EXTERN BOOL RTC_RtcToUtc(UINT64* pu8Utc, const RTC_T* prRtc);


//----------------------------------------------------------------------------
/** RTC_UtcToRtc
 *  Translate UTC to RTC
 *  @param      u8Utc   The UTC to be translated
 *  @param      prRtc   The translated RTC
 *  @retval     TRUE    Succeed
 *  @retval     FALSE   Fail. 
 */
//----------------------------------------------------------------------------
EXTERN BOOL RTC_UtcToRtc(RTC_T* prRtc, UINT64 u8Utc);

//----------------------------------------------------------------------------
/** RTC_Init
 *  Initialize RTC driver and hardware
 *  @retval     TRUE    Succeed
 *  @retval     FALSE   Fail.
 */
//----------------------------------------------------------------------------
EXTERN BOOL RTC_Init(void);


//----------------------------------------------------------------------------
/** RTC_GetTimeDate
 *  Get time from RTC
 *  @param      prTime      Current time
 *  @retval     TRUE        Succeed
 *  @retval     FALSE       Fail.
 */
//----------------------------------------------------------------------------
EXTERN BOOL RTC_GetTimeDate(UINT64* prTime);


//----------------------------------------------------------------------------
/** RTC_SetTimeDate
 *  Set current time
 *  @param      prTime      The time to set
 *  @retval     TRUE        Succeed
 *  @retval     FALSE       Fail.
 */
//----------------------------------------------------------------------------
EXTERN BOOL RTC_SetTimeDate(const UINT64* prTime);

//----------------------------------------------------------------------------
/** RTC_SetAlertTime
 *  Set RTC alarm time
 *  @param      prTime      The alarm time to set
 *  @retval     TRUE        Succeed
 *  @retval     FALSE       Fail.
 */
//----------------------------------------------------------------------------
EXTERN BOOL RTC_SetAlertTime(UINT64 u8Utc);

//----------------------------------------------------------------------------
/** RTC_GetAlertTime
 *  Get alarm time set for RTC
 *  @param      prTime  The current alarm time setting
 *  @retval     TRUE    Succeed
 *  @retval     FALSE   Fail.
 */
//----------------------------------------------------------------------------
EXTERN BOOL RTC_GetAlertTime(UINT64* prTime);


//----------------------------------------------------------------------------
/** RTC_HookAlertHandler
 *  Hook the alert handler to RTC alarm interrupt
 *  @param      pfnHandler      The alert handler.
 *  @retval     none
 */
//----------------------------------------------------------------------------
EXTERN void RTC_HookAlertHandler(PFN_RTC_ALERT_HANDLER_T pfnHandler);

//----------------------------------------------------------------------------
/** RTC_SetWakeup
 *  Set RTC wakeup enable
 *  @param      pu4PowerDonwEn      The power down and wakeup enable setting.
 *  @retval     none
 */
//----------------------------------------------------------------------------
EXTERN void RTC_SetWakeup(UINT32* pu4PowerDownEn);

#endif
