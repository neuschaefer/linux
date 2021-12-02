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
 * $RCSfile: measure_time.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file time.c
 *  Time measurement functions
 */

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_timer.h"
#include "x_os.h"
#include "x_printf.h"

#ifdef __KERNEL__
#include <asm/atomic.h>
#include <linux/kernel.h>
#include <asm/string.h>
#endif // ifdef __KERNEL

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define MAX_MEASURE_SLOTS 64
#define MAX_NAME_LENGTH   31


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef enum {
    MEASURE_STATE_UNINIT = 0,
    MEASURE_STATE_INIT,
    MEASURE_STATE_START,
    MEASURE_STATE_STOP
} MEASURE_STATE_T;

typedef struct {
    UINT32           u4Id;
    MEASURE_STATE_T  u4State;
    HAL_RAW_TIME_T   rStartTime;
    HAL_RAW_TIME_T   rStopTime;
    UINT8            au1Name[MAX_NAME_LENGTH + 1];
} MEASURE_TIME_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static MEASURE_TIME_T _arTimeSlots[MAX_MEASURE_SLOTS];
#ifdef __KERNEL__
static atomic_t       _rCurSlot = ATOMIC_INIT(-1);
#else
static INT32          _i4CurSlot = -1;
#endif

//-----------------------------------------------------------------------------
// Import functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/** TIME_RegisterEntry
 *  Register new time counting entry
 *  @param  UINT8       pau1Name, it is used in printing results
 *  @retval >= 0        Successful, ID returned
 *  @retval < 0         Error
 */

INT32 TIME_RegisterEntry(UINT8 * pu1Name)
{
    UINT32 u4Len;
    INT32 i;
    
    if (!pu1Name)
    {
        return -1;
    }

    u4Len = x_strlen((const CHAR *)pu1Name);

#ifdef __KERNEL__
    i = (INT32) atomic_inc_return(&_rCurSlot);
#else
    i = _i4CurSlot++;
#endif
    
    if (i >= MAX_MEASURE_SLOTS)
    {
#ifdef __KERNEL__
        atomic_sub(1, &_rCurSlot);
#else
        _i4CurSlot--;
#endif
        return -2;
    }

    _arTimeSlots[i].u4Id = (UINT32) i;
    _arTimeSlots[i].u4State = MEASURE_STATE_INIT;
    HAL_GetRawTime(&_arTimeSlots[i].rStartTime);
    _arTimeSlots[i].rStopTime = _arTimeSlots[i].rStartTime;
    if (u4Len < MAX_NAME_LENGTH)
    {
        x_memcpy((void*)&_arTimeSlots[i].au1Name, (void*)pu1Name, (UINT32)u4Len);
        _arTimeSlots[i].au1Name[u4Len] = 0;
    }
    else
    {
        x_memcpy((void*)&_arTimeSlots[i].au1Name, (void*)pu1Name, (UINT32)MAX_NAME_LENGTH);
        _arTimeSlots[i].au1Name[MAX_NAME_LENGTH] = 0;
    }
    
    return i;
}


/** TIME_StartMeasure
 *  Start measuring time
 *  @param  UINT32      u4Id
 *  @retval >= 0        Successful
 *  @retval < 0         Error
 */

INT32 TIME_StartMeasure(UINT32 u4Id)
{
    if (u4Id >= MAX_MEASURE_SLOTS)
    {
        return -1;
    }

    if (_arTimeSlots[u4Id].u4State == MEASURE_STATE_UNINIT)
    {
        return -2;
    }

    HAL_GetRawTime(&_arTimeSlots[u4Id].rStartTime);
    _arTimeSlots[u4Id].rStopTime = _arTimeSlots[u4Id].rStartTime;

    _arTimeSlots[u4Id].u4State = MEASURE_STATE_START;
    
    return 0;
}


/** TIME_StopMeasure
 *  Stop measuring time
 *  @param  UINT32      u4Id
 *  @retval >= 0        Successful
 *  @retval < 0         Error
 */

INT32 TIME_StopMeasure(UINT32 u4Id)
{
    if (u4Id >= MAX_MEASURE_SLOTS)
    {
        return -1;
    }

    if (_arTimeSlots[u4Id].u4State != MEASURE_STATE_START)
    {
        return -2;
    }

    HAL_GetRawTime(&_arTimeSlots[u4Id].rStopTime);
    _arTimeSlots[u4Id].u4State = MEASURE_STATE_STOP;
    
    return 0;
}


/** TIME_PrintElapsedTime
 *  Print measured time information
 *  @param  UINT32      u4Id
 *  @retval >= 0        Successful
 *  @retval < 0         Error
 */

INT32 TIME_PrintElapsedTime(UINT32 u4Id)
{
    HAL_RAW_TIME_T rResult;
    HAL_TIME_T rTime;
    
    if (u4Id >= MAX_MEASURE_SLOTS)
    {
        return -1;
    }

    if (_arTimeSlots[u4Id].u4State != MEASURE_STATE_STOP)
    {
        return -2;
    }

    /* print name */
    Printf("[Id:%02d], Name: %s, Elapsed time: ", (int) u4Id, _arTimeSlots[u4Id].au1Name);
    
    /* print elspsed time */
    HAL_GetDeltaRawTime(&rResult, &_arTimeSlots[u4Id].rStartTime, &_arTimeSlots[u4Id].rStopTime);
    //printk("Ticks:%d, cycles:%d\n", (int) rResult.u4Ticks, (int) rResult.u4Cycles);
    HAL_RawToTime(&rResult, &rTime);
    Printf("%d.%06d s\n", (int) rTime.u4Seconds, (int) rTime.u4Micros);
    
    return 0;
}


/** TIME_PrintAllElapsedTime
 *  Print all registered measured time information
 *  @retval >= 0        Successful
 *  @retval < 0         Error
 */

INT32 TIME_PrintAllElapsedTime(void)
{
    INT32 i, i4Total;

#ifdef __KERNEL__
    i4Total = atomic_read(&_rCurSlot);
#else
    i4Total = _i4CurSlot;
#endif
    
    for (i = 0; i <= i4Total; i++)
    {
        TIME_PrintElapsedTime((UINT32)i);
    }
    
    return 0;
}


