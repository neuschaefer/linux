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

/*************************************************************************/
/*                                                                       */
/* FILE NAME                                               VERSION       */
/*                                                                       */
/*      tpc.h                                                1.14        */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      THREAD PROFILER -                                                */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file processes Nucleus PLUS' context switch time            */
/*                                                                       */
/* DATA STRUCTURES                                                       */
/*                                                                       */
/*                                                                       */
/*                                                                       */
/* FUNCTIONS                                                             */
/*                                                                       */
/*      None                                                             */
/*      void TP_ThreadEnter(TC_TCB *thread)                              */
/*      void TP_ThreadExit(TC_TCB *thread)                               */
/*      void TP_TimeAccumulate(void)                                     */
/*      void TP_ReportDump(void)                                         */
/*      void TP_TimeReset(void)                                          */
/*                                                                       */
/*************************************************************************/

#if defined(NUCLEUS_THREAD_PROFILING)

#include "nucleus.h"
#include "tc_defs.h"
#include "tp_defs.h"

#include "x_os.h"
#include "x_typedef.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_timer.h"
#include "x_hal_5381.h"

typedef struct _TP_THREAD_INFO_T
{
	TC_TCB *pr_thread;
	TP_TIME_STAT r_time_statistics;
	BOOL fg_active;
} TP_THREAD_INFO_T;


void TP_ThreadEnter(TC_TCB *thread);
void TP_ThreadExit(TC_TCB *thread);

#define MAX_NUM_TASK 256

static BOOL _fgSysClockInit;
static TC_TCB* _prCurrThread;
static unsigned int _ui_created_tasks = 1;
static TP_THREAD_INFO_T _ar_task_info[MAX_NUM_TASK];

static UINT32 _u4SysClock = 0;
extern CS_NODE         *TCD_Created_Tasks_List;
extern UNSIGNED         TCD_Total_Tasks;

void TP_ThreadCreate(TC_TCB *thread)
{
	UNSIGNED ui_thread_id;
	ui_thread_id = _ui_created_tasks;
	_ui_created_tasks++;
	thread->tc_app_reserved_1 = ui_thread_id;
	_ar_task_info[ui_thread_id].pr_thread = thread;
	_ar_task_info[ui_thread_id].fg_active = TRUE;
    if (_u4SysClock == 0)
    {
        _u4SysClock = BSP_GetSystemClock();
    }
}

void TP_ThreadEnter(TC_TCB *thread)
{
	HAL_RAW_TIME_T rCurrTime;
	TP_TIME_STAT* prTimeStat;
	CRIT_STATE_T rState;
	UINT16 u2_thread_id;

	if (!_fgSysClockInit)
	{
		return;
	}

	if (_prCurrThread)
	{
		if (_prCurrThread != thread)
		{
			TP_ThreadExit(_prCurrThread);
		}
		else
		{
			return;
		}
	}

	if (thread && TCD_Created_Tasks_List && TCD_Total_Tasks && thread != (TC_TCB*)2 &&
		thread->tc_app_reserved_1 < MAX_NUM_TASK)
	{
		u2_thread_id = (UINT16)thread->tc_app_reserved_1;
		prTimeStat = &_ar_task_info[u2_thread_id].r_time_statistics;
		HAL_GetRawTime(&rCurrTime);
		rState = x_crit_start();
			prTimeStat->u4StartTicks		= rCurrTime.u4Ticks;
			prTimeStat->u4StartCycles		= rCurrTime.u4Cycles;
		x_crit_end(rState);
	}
	_prCurrThread = thread;
}

void TP_ThreadExit(TC_TCB *thread)
{
	HAL_RAW_TIME_T rStartTime;
	HAL_RAW_TIME_T rCurrTime;
	HAL_RAW_TIME_T rDiffTime;
	TP_TIME_STAT* prTimeStat;
	CRIT_STATE_T rState;
	UINT16 u2_thread_id;

	if (!_fgSysClockInit)
	{
		return;
	}

	if (thread && TCD_Created_Tasks_List && TCD_Total_Tasks &&
		thread != (TC_TCB*)2 && thread == _prCurrThread)
	{
		u2_thread_id = (UINT16)thread->tc_app_reserved_1;
		prTimeStat = &_ar_task_info[u2_thread_id].r_time_statistics;

		rState = x_crit_start();
			rStartTime.u4Ticks		= prTimeStat->u4StartTicks;
			rStartTime.u4Cycles		= prTimeStat->u4StartCycles;
		x_crit_end(rState);

		if (rStartTime.u4Ticks || rStartTime.u4Cycles)
		{
			HAL_GetRawTime(&rCurrTime);
			if ((rStartTime.u4Ticks > rCurrTime.u4Ticks) ||
				((rStartTime.u4Ticks == rCurrTime.u4Ticks) && (rStartTime.u4Cycles >= rCurrTime.u4Cycles)))
			{
				// start time error, add onely one cycle
				rDiffTime.u4Ticks		= 0;
				rDiffTime.u4Cycles		= 1;
			}
			else
			{
				HAL_GetDeltaRawTime(&rDiffTime, &rStartTime, &rCurrTime);
			}

			rState = x_crit_start();
				prTimeStat->u4StartTicks		= 0;
				prTimeStat->u4StartCycles		= 0;
			x_crit_end(rState);
		}
		else
		{
			/* thread exit without a start, counts 1 cycle */
			rDiffTime.u4Ticks		= 0;
			rDiffTime.u4Cycles		= 1;
			/* ASSERT(rStartTime.u4Ticks || rStartTime.u4Cycles); */
		}
		rState = x_crit_start();
        do
        {
            UINT64 u8DiffTime;
            UINT64 u8AccTime;
            u8DiffTime = ((UINT64)rDiffTime.u4Ticks << 32) | rDiffTime.u4Cycles;
            u8AccTime  = ((UINT64)prTimeStat->u4AccuTicks << 32) | prTimeStat->u4AccuCycles;
            u8AccTime += u8DiffTime;
            prTimeStat->u4AccuTicks  = u8AccTime >> 32;
            prTimeStat->u4AccuCycles = u8AccTime & 0xffffffff;
        } while (0);
		x_crit_end(rState);

	}

	if (thread == _prCurrThread)
	{
		_prCurrThread = NULL;
	}
}

void TP_ReportDump(void)
{
	TC_TCB	*pr_thread;
	UINT16 u2_thread_id;
	TP_TIME_STAT* prTimeStat;
	//UNSIGNED u4TotalTicks;
	//UNSIGNED u4TotalCycles;
	//double fTotalTime;
	//CRIT_STATE_T rState;
	HAL_TIME_T rHalTime;
	HAL_RAW_TIME_T rRawTime;
	UINT64 u8TotalCycles, u8Cycles;
    UINT64 u8TotalTime = 0;
    UINT32 u4IdleTasks = 0;
	UINT32 u4Ratio = 0;

	if (!_fgSysClockInit)
	{
		return;
	}

	_fgSysClockInit = FALSE;

    // Calculate total time first
    u8TotalCycles = 0;
    for (u2_thread_id = 0; u2_thread_id < _ui_created_tasks; u2_thread_id++)
    {
		prTimeStat = &_ar_task_info[u2_thread_id].r_time_statistics;
        u8Cycles = prTimeStat->u4AccuTicks;
        u8Cycles <<= 32;
        u8Cycles += prTimeStat->u4AccuCycles;
        u8TotalCycles += u8Cycles;
    }

	Printf("  THREAD  :    SCHED    :    TIME     :     %%\n");
	Printf("----------+-------------+-------------+----------\n");

	prTimeStat = &_ar_task_info[0].r_time_statistics;
	rRawTime.u4Ticks = prTimeStat->u4AccuTicks;
	rRawTime.u4Cycles = prTimeStat->u4AccuCycles;
	HAL_RawToTime(&rRawTime, &rHalTime);
    u8Cycles = ((UINT64)rRawTime.u4Ticks << 32) | rRawTime.u4Cycles;
    if (u8TotalCycles != 0)
    {
        u4Ratio = (UINT32)((u8Cycles * 1000) / u8TotalCycles);
    }
	Printf("[%8s]   %9u   %6u.%06u    %3u.%u\n",
		"HISR", 0,
		rHalTime.u4Seconds, rHalTime.u4Micros, u4Ratio / 10, u4Ratio % 10);
    u8TotalTime += u8Cycles;


	for (u2_thread_id = 1; u2_thread_id < _ui_created_tasks; u2_thread_id++)
	{
		if (!_ar_task_info[u2_thread_id].fg_active)
		{
			continue;
		}
		pr_thread = _ar_task_info[u2_thread_id].pr_thread;
		if (pr_thread->tc_id != TC_TASK_ID)
		{
			continue;
		}
		prTimeStat = &_ar_task_info[u2_thread_id].r_time_statistics;
		rRawTime.u4Ticks = prTimeStat->u4AccuTicks;
		rRawTime.u4Cycles = prTimeStat->u4AccuCycles;
        if ( rRawTime.u4Ticks || rRawTime.u4Cycles )
        {
            u8Cycles = ((UINT64)rRawTime.u4Ticks << 32) | rRawTime.u4Cycles;
            if (u8TotalCycles != 0)
            {
                u4Ratio = (UINT32)((u8Cycles * 1000) / u8TotalCycles);
            }
	    	HAL_RawToTime(&rRawTime, &rHalTime);
		    Printf("[%8s]   %9u   %6u.%06u    %3u.%u\n",
			    pr_thread->tc_name, pr_thread->tc_scheduled,
			    rHalTime.u4Seconds, rHalTime.u4Micros, u4Ratio / 10, u4Ratio % 10);
            u8TotalTime += u8Cycles;
        }

	}

	Printf("----------+-------------+-------------+----------\n");
    rRawTime.u4Ticks  = u8TotalTime >> 32;
    rRawTime.u4Cycles = u8TotalTime & 0xffffffff;
    HAL_RawToTime(&rRawTime, &rHalTime);
    Printf("                         %6u.%06u\n", rHalTime.u4Seconds, rHalTime.u4Micros);
	Printf("----------+-------------+-------------+----------\n");
    Printf("Idle tasks (time == 0.0):\n");
    for (u2_thread_id = 1; u2_thread_id < _ui_created_tasks; u2_thread_id++)
    {
		if (!_ar_task_info[u2_thread_id].fg_active)
		{
			continue;
		}
		pr_thread = _ar_task_info[u2_thread_id].pr_thread;
		if (pr_thread->tc_id != TC_TASK_ID)
		{
			continue;
		}
		prTimeStat = &_ar_task_info[u2_thread_id].r_time_statistics;
		rRawTime.u4Ticks = prTimeStat->u4AccuTicks;
		rRawTime.u4Cycles = prTimeStat->u4AccuCycles;
        if (rRawTime.u4Ticks == 0 && rRawTime.u4Cycles == 0)
        {
            Printf("[%8s] ", pr_thread->tc_name);
            u4IdleTasks++;
            if ((u4IdleTasks % 6) == 0)
            {
                Printf("\n");
            }
        }
    }
    Printf("\n");
	_fgSysClockInit = TRUE;

}

void TP_TimeReset(void)
{
	if (!_fgSysClockInit)
	{
		HAL_InitTimer();
		_u4SysClock = BSP_GetSystemClock();
		if (_u4SysClock == 0)
		{
			_u4SysClock = 162000000;	/* default value */
		}
	}
	_fgSysClockInit = TRUE;
}



#endif /* NUCLEUS_THREAD_PROFILING */
