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
 * $RCSfile: rtc_sw.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         This file contains implementation of hardware independent interface of RTC driver.
 *---------------------------------------------------------------------------*/

//----------------------------------------------------------------------------
// Constant definitions
//----------------------------------------------------------------------------
#include "x_typedef.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_util.h"
#include "rtc_sw.h"

//----------------------------------------------------------------------------
// Config
//----------------------------------------------------------------------------
#define ENABLE_UTC_BOUNDARY             1

//----------------------------------------------------------------------------
// Constant definitions
//----------------------------------------------------------------------------
#define MAX_COUNT_YEAR                  99

#if ENABLE_UTC_BOUNDARY
// UTC bounds
#define UTC_LOWER_BOUND                 (946684800)   // Jan  1 00:00:00, 2000 UTC time
//#define UTC_UPPER_BOUND                 2147483640  // Jan 19 11:14:00, 2038
#define UTC_UPPER_BOUND                 0xffffffffU

#endif /* ENABLE_UTC_BOUNDARY */

BOOL _IsLeapYear(UINT8 u1Year);

//----------------------------------------------------------------------------
// Static variables
//----------------------------------------------------------------------------
static const UINT8 _au1RTCMonthDays[12] =
    { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };


//----------------------------------------------------------------------------
// Private functions
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/** _IsLeapYear() Check if the given year is a leap year
 *  @param u1Year The year
 *  @retval TRUE: Yes, FALSE: No
 ----------------------------------------------------------------------------*/
BOOL _IsLeapYear(UINT8 u1Year)
{
    UINT64 u8Result;   
    UINT64 u8Remainder;
    u8Result = u8Div6432((UINT64)u1Year, (UINT64)4, &u8Remainder);
    UNUSED(u8Result);
    if(u8Remainder == 0)
    {
        u8Result = u8Div6432((UINT64)u1Year, (UINT64)100, &u8Remainder);
        if(u8Remainder != 0)
        {
            return TRUE;
        }
        u8Result = u8Div6432((UINT64)u1Year, (UINT64)400, &u8Remainder);
        if(u8Remainder == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}


//----------------------------------------------------------------------------
// Public functions
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/** _BcdToBinary() Convert a BCD to a binary
 *  @param u1Bcd The BCD to be converted
 *  @retval The converted binary
 */
//----------------------------------------------------------------------------
UINT8 _RTC_BcdToBinary(UINT8 u1Bcd)
{
    UINT8 u1Hi, u1Lo;
/*
    if (_fgRTCDisableRTC)
    {
        return 1;
    }
*/
    u1Hi = (u1Bcd >> 4) & 0xf;
    if(u1Hi >= 10)
    {
        return 0;
    }

    u1Lo = u1Bcd & 0xf;
    if(u1Lo >= 10)
    {
        return 0;
    }

    return ((u1Hi * 10) + u1Lo);
}

//----------------------------------------------------------------------------
/** _BinaryToBcd() Convert a binary to a BCD
 *  @param u1Binary The binary to be converted
 *  @retval The converted BCD
 */
//----------------------------------------------------------------------------
UINT8 _RTC_BinaryToBcd(UINT8 u1Binary)
{
    UINT8 u1Hi, u1Lo;

    if(u1Binary > 99)
    {
        return 0;
    }


    {
        UINT64 u8Remainder;
        u1Hi = (UINT8)u8Div6432((UINT64)u1Binary, (UINT64)10, &u8Remainder);
        u1Lo = (UINT8)u8Remainder;
    }

    return (u1Hi << 4) + u1Lo;
}

//----------------------------------------------------------------------------
/** RTC_RtcToUtc() Translate RTC to UTC
 *  @param prRtc, Input, The RTC to be translated
 *  @param pu8Utc, Output, The translated UTC
 *  @retval TRUE, Succeed; FALSE, Fail
 */
//----------------------------------------------------------------------------
BOOL _RTC_RtcToUtc(UINT64* pu8Utc, const RTC_T* prRtc)
{
    UINT64 u8Days, u8Seconds;
    UINT8 u1MaxDays, i;
    BOOL fgLeapYear;

    VERIFY(prRtc!=NULL);
    // Check year and month
    if ((prRtc->u1Year > 99) || (prRtc->u1Month < 1) || (prRtc->u1Month > 12))
    {
        return FALSE;
    }

    // Check hour, minite and second
    if ((prRtc->u1Hour > 23) || (prRtc->u1Minute > 59) || (prRtc->u1Second > 59))
    {
        return FALSE;
    }

    // Determine if leap year
    fgLeapYear = _IsLeapYear(prRtc->u1Year);

    // Check day
    u1MaxDays = _au1RTCMonthDays[prRtc->u1Month - 1];
    if ((prRtc->u1Month == 2) && fgLeapYear)
    {
        u1MaxDays++;
    }
    if ((prRtc->u1Day < 1) || (prRtc->u1Day > u1MaxDays))
    {
        return FALSE;
    }

    // Compute total days
    u8Days = 0;
    for (i = 0; i < prRtc->u1Year; i++)
    {
        if (_IsLeapYear(i))
        {
            u8Days += 366;
        }
        else
        {
            u8Days += 365;
        }
    }
    for (i = 0; i < (prRtc->u1Month - 1); i++)
    {
        u8Days += _au1RTCMonthDays[i];
    }
    if ((prRtc->u1Month > 2) && fgLeapYear)
    {
        u8Days++;
    }
    u8Days += prRtc->u1Day - 1;

    // Compute total seconds
    u8Seconds = (((((u8Days * 24) + prRtc->u1Hour) * 60) + prRtc->u1Minute) * 60)
        + prRtc->u1Second;

#if ENABLE_UTC_BOUNDARY
    VERIFY(pu8Utc!=NULL);
    // Translate to UTC
    *pu8Utc = u8Seconds + UTC_LOWER_BOUND;
#else
    UNUSED(u8Seconds);
#endif /* ENABLE_UTC_BOUNDARY */

    return TRUE;
}


//----------------------------------------------------------------------------
/** RTC_UtcToRtc() Translate UTC to RTC
 *  @param u8Utc, Input, The UTC to be translated
 *  @param prRtc, Output, The translated RTC
 *  @retval TRUE, Succeed; FALSE, Fail.
 */
//----------------------------------------------------------------------------
BOOL _RTC_UtcToRtc(RTC_T* prRtc, UINT64 u8Utc)
{
    UINT32 u4Days, u4Seconds;
    UINT8 u1Year, u1Month;
    BOOL fgLeapYear;
    const UINT32 u4SecondsPerDay = 24 * 60 * 60;

    VERIFY(prRtc!=NULL);
#if ENABLE_UTC_BOUNDARY
    // Check if the given UTC is big enough
    if ((u8Utc < UTC_LOWER_BOUND) || (u8Utc > UTC_UPPER_BOUND))
    {
        // return FALSE;
        u8Utc = UTC_LOWER_BOUND;
    }

    // Compute total days and remaining seconds
    u8Utc -= UTC_LOWER_BOUND;
#endif /* ENABLE_UTC_BOUNDARY */


    {
        UINT64 u8Remainder;
        u4Days = (UINT32)u8Div6432(u8Utc, (UINT64)u4SecondsPerDay, &u8Remainder);
    } 
    u4Seconds = (UINT32)(u8Utc - ((UINT64)u4Days * u4SecondsPerDay));

    // Compute seconds, minutes and hours

    {
        UINT64 u8Remainder;
        u4Seconds = (UINT32)u8Div6432((UINT64)u4Seconds, (UINT64)60, &u8Remainder);
        prRtc->u1Second = (UINT8)u8Remainder; 

        u4Seconds = (UINT32)u8Div6432((UINT64)u4Seconds, (UINT64)60, &u8Remainder);
        prRtc->u1Minute = (UINT8)u8Remainder; 
    }

    VERIFY(u4Seconds < 24);
    prRtc->u1Hour = u4Seconds;

    // Compute years
    u1Year = 0;
    while (u4Days > 0)
    {
        UINT32 u4YearDays;

        if (_IsLeapYear(u1Year))
        {
            u4YearDays = 366;
        }
        else
        {
            u4YearDays = 365;
        }

        if (u4Days >= u4YearDays)
        {
            u4Days -= u4YearDays;
            u1Year++;
        }
        else
        {
            break;
        }
    }
    if (u1Year > MAX_COUNT_YEAR)
    {
        return FALSE;
    }

    // Compute months
    fgLeapYear = _IsLeapYear(u1Year);
    u1Month = 0;
    while (u4Days > 0)
    {
        UINT8 u1MonthDays;

        u1MonthDays = _au1RTCMonthDays[u1Month];
        if ((u1Month == 1) && fgLeapYear)
        {
            u1MonthDays++;
        }

        if (u4Days >= u1MonthDays)
        {
            u4Days -= u1MonthDays;
            u1Month++;
        }
        else
        {
            break;
        }
    }
    VERIFY(u1Month < 12);

    prRtc->u1Year = u1Year;
    prRtc->u1Month = u1Month + 1;
    prRtc->u1Day = (UINT8)(u4Days + 1);

    return TRUE;
}

