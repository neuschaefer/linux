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
 * Description:
 *      execution time profile test module
 *
 *---------------------------------------------------------------------------*/


#include "x_lint.h"
LINT_SAVE_AND_DISABLE

#include "x_timeprofile.h"
#include "x_printf.h"
#include "x_os.h"
#include "x_timer.h"
#if (TIMEPROFILE_LOG == 1)
//------------------------------------------------------------------------------
/** Start to record time
 */
//------------------------------------------------------------------------------
void ExeTimeRecordStart(HAL_RAW_TIME_T *pRawTimeStart)
{
    HAL_GetRawTime(pRawTimeStart);
  //  Printf("starttick:%x, cycle:%x\n", pRawTimeStart->u4Ticks, pRawTimeStart->u4Cycles);

}
//------------------------------------------------------------------------------
/** Stop to record time
 */
//------------------------------------------------------------------------------
void ExeTimeRecordStop(EXETIME_MEASURE *pExeTime)
{
    HAL_RAW_TIME_T Delta;
    HAL_RAW_TIME_T RawTimeRecordEnd;
    UINT32 tmp;

    HAL_GetRawTime(&RawTimeRecordEnd);
//Printf("endtick:%x, cycle:%x\n", RawTimeRecordEnd.u4Ticks, RawTimeRecordEnd.u4Cycles);
    HAL_GetDeltaRawTime(&Delta, (const HAL_RAW_TIME_T*)&pExeTime->RawTimeStart
        ,(const HAL_RAW_TIME_T*)&RawTimeRecordEnd);
//Printf("deltatick:%d, cycle:%d\n", Delta.u4Ticks, Delta.u4Cycles);

    tmp = (~pExeTime->RawTimeTotal.u4Cycles);
    if ( tmp < Delta.u4Cycles)
    {
        pExeTime->RawTimeTotal.u4Cycles = (Delta.u4Cycles - tmp -1);
        pExeTime->RawTimeTotal.u4Ticks++;
    }
    else
    {
        pExeTime->RawTimeTotal.u4Cycles += Delta.u4Cycles;
    }
    pExeTime->RawTimeTotal.u4Ticks += Delta.u4Ticks;

    pExeTime->u4EnterCntL++;
    if (pExeTime->u4EnterCntL == 0)
    {
        pExeTime->u4EnterCntH++;
    }

    if (pExeTime->RawTimeMax.u4Ticks < Delta.u4Ticks)
    {
        pExeTime->RawTimeMax.u4Ticks = Delta.u4Ticks;
        pExeTime->RawTimeMax.u4Cycles = Delta.u4Cycles;
    }
    else if (pExeTime->RawTimeMax.u4Ticks == Delta.u4Ticks)
    {
        if (pExeTime->RawTimeMax.u4Cycles < Delta.u4Cycles)
        {
            pExeTime->RawTimeMax.u4Cycles = Delta.u4Cycles;
        }
    }

}
#ifdef __KERNEL__
#include "x_linux.h"
#endif /* ifdef __KERNEL */
//------------------------------------------------------------------------------
/** excution time statistics
 */
//------------------------------------------------------------------------------
UINT32 ExeTimeStatistics(EXETIME_MEASURE *pExeTime, HAL_RAW_TIME_T *pAvgRawTime, HAL_RAW_TIME_T *pMaxRawTime)
{
    UINT64 TotalEntryCnt;
    CRIT_STATE_T TimeLock;
#ifdef __KERNEL__
        UINT64 u8Remainder;
#endif /* #ifdef __KERNEL__ */
    TimeLock = x_crit_start();
    TotalEntryCnt = pExeTime->u4EnterCntH;
    TotalEntryCnt = (UINT64)(TotalEntryCnt<<32) + pExeTime->u4EnterCntL;

    if (TotalEntryCnt==0)
    {
        x_crit_end(TimeLock);
        return 1;
    }
#ifdef __KERNEL__
    pAvgRawTime->u4Ticks = u8Div6432(pExeTime->RawTimeTotal.u4Ticks, (UINT64)TotalEntryCnt, &u8Remainder);
    pAvgRawTime->u4Cycles = u8Div6432(pExeTime->RawTimeTotal.u4Cycles, TotalEntryCnt, NULL);
#else
    pAvgRawTime->u4Ticks = pExeTime->RawTimeTotal.u4Ticks/TotalEntryCnt;
    pAvgRawTime->u4Cycles = pExeTime->RawTimeTotal.u4Cycles/TotalEntryCnt;
#endif
    pMaxRawTime->u4Ticks = pExeTime->RawTimeMax.u4Ticks;
    pMaxRawTime->u4Cycles = pExeTime->RawTimeMax.u4Cycles;

    x_crit_end(TimeLock);
    return 0;
}
EXTERN EXETIME_MEASURE ExeTime[40];
//------------------------------------------------------------------------------
/** excution time statistics
 */
//------------------------------------------------------------------------------
UINT32 CritExeTimeStatistics(EXETIME_MEASURE *pExeTime, HAL_RAW_TIME_T *pAvgRawTime, HAL_RAW_TIME_T *pMaxRawTime)
{
    UINT64 TotalEntryCnt;
//    CRIT_STATE_T TimeLock;
//    CRIT_STATE_T TimeLock2;
//    CRIT_STATE_T TimeLock3;
#ifdef __KERNEL__
        UINT64 u8Remainder;
#endif /* #ifdef __KERNEL__ */


    TotalEntryCnt = pExeTime->u4EnterCntH;
    TotalEntryCnt = (UINT64)(TotalEntryCnt<<32) + pExeTime->u4EnterCntL;
 //   if (TotalEntryCnt==0)
    {
 //       x_crit_end(TimeLock);
 //       return 1;
}
    if(TotalEntryCnt==0)
    {
    pAvgRawTime->u4Ticks = 0;
        pAvgRawTime->u4Cycles = 0;
    }
    else
        {
        #ifdef __KERNEL__
        pAvgRawTime->u4Ticks = u8Div6432(pExeTime->RawTimeTotal.u4Ticks, (UINT64)TotalEntryCnt, &u8Remainder);
        pAvgRawTime->u4Cycles = u8Div6432(pExeTime->RawTimeTotal.u4Cycles, (UINT64)TotalEntryCnt, NULL);
        #else
        pAvgRawTime->u4Ticks = pExeTime->RawTimeTotal.u4Ticks/TotalEntryCnt;
        pAvgRawTime->u4Cycles = pExeTime->RawTimeTotal.u4Cycles/TotalEntryCnt;
        #endif
    }
    pMaxRawTime->u4Ticks = pExeTime->RawTimeMax.u4Ticks;
    pMaxRawTime->u4Cycles = pExeTime->RawTimeMax.u4Cycles;

    return 0;
}
//------------------------------------------------------------------------------
/** save previous stop time and current start time
 */
//------------------------------------------------------------------------------
void SaveExeStartStopTime(EXETIME_MEASURE *pCurrStartTime, EXETIME_MEASURE *pPrevStopTime)
{
    HAL_RAW_TIME_T CurrRawTime;
    HAL_GetRawTime(&CurrRawTime);
    if (pCurrStartTime)
    {
        pCurrStartTime->RawTimeStart.u4Cycles = CurrRawTime.u4Cycles;
        pCurrStartTime->RawTimeStart.u4Ticks = CurrRawTime.u4Ticks;
    }
    if(pPrevStopTime)
    {
        pPrevStopTime->RawTimeStop.u4Cycles = CurrRawTime.u4Cycles;
        pPrevStopTime->RawTimeStop.u4Ticks = CurrRawTime.u4Ticks;
    }
}
//------------------------------------------------------------------------------
/** cal total max and avg statistics
 */
//------------------------------------------------------------------------------
void CalMaxAvgTime(EXETIME_MEASURE *pdata, UINT32 u4Cnt)
{
    INT32 i;
    HAL_RAW_TIME_T Delta;
    UINT32 tmp;
    UINT64 TotalEntryCnt;
#ifdef __KERNEL__
        UINT64 u8Remainder;
#endif /* #ifdef __KERNEL__ */
    for (i = (u4Cnt-1); i >= 0; i--)
    {
            HAL_GetDeltaRawTime(&Delta, (const HAL_RAW_TIME_T*)&pdata[i].RawTimeStart
                , (const HAL_RAW_TIME_T*)&pdata[i].RawTimeStop);
            tmp = (~pdata[i].RawTimeTotal.u4Cycles);
            if ( tmp < Delta.u4Cycles)
            {
                pdata[i].RawTimeTotal.u4Cycles = (Delta.u4Cycles - tmp -1);
                pdata[i].RawTimeTotal.u4Ticks++;
            }
            else
            {
                pdata[i].RawTimeTotal.u4Cycles += Delta.u4Cycles;
            }
            pdata[i].RawTimeTotal.u4Ticks += Delta.u4Ticks;
            pdata[i].u4EnterCntL++;
            if (pdata[i].u4EnterCntL == 0)
            {
                pdata[i].u4EnterCntH++;
            }
            if (pdata[i].RawTimeMax.u4Ticks < Delta.u4Ticks)
            {
                pdata[i].RawTimeMax.u4Ticks = Delta.u4Ticks;
                pdata[i].RawTimeMax.u4Cycles = Delta.u4Cycles;
            }
            else if (pdata[i].RawTimeMax.u4Ticks == Delta.u4Ticks)
            {
                if (pdata[i].RawTimeMax.u4Cycles < Delta.u4Cycles)
                {
                    pdata[i].RawTimeMax.u4Cycles = Delta.u4Cycles;
                }
            }
        TotalEntryCnt = pdata[i].u4EnterCntH;
        TotalEntryCnt = (UINT64)(TotalEntryCnt<<32) + pdata[i].u4EnterCntL;
        if (TotalEntryCnt==0)
        {
            pdata[i].RawTimeAvg.u4Ticks = 0;
            pdata[i].RawTimeAvg.u4Cycles= 0;
    }
        else
        {
        #ifdef __KERNEL__
            pdata[i].RawTimeAvg.u4Ticks = u8Div6432(pdata[i].RawTimeTotal.u4Ticks, TotalEntryCnt, &u8Remainder);
            pdata[i].RawTimeAvg.u4Cycles = u8Div6432(pdata[i].RawTimeTotal.u4Cycles, TotalEntryCnt, NULL);
        #else
            pdata[i].RawTimeAvg.u4Ticks = pdata[i].RawTimeTotal.u4Ticks/TotalEntryCnt;
            pdata[i].RawTimeAvg.u4Cycles = pdata[i].RawTimeTotal.u4Cycles/TotalEntryCnt;
        #endif
}
    }
}
#endif  // #if (TIMEPROFILE_LOG == 1)
LINT_RESTORE
