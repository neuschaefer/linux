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

#include "x_typedef.h"
#include "x_assert.h"

#include "../nucleus.h"
#include "fakeview.h"

#if defined(NUCLEUS_THREAD_PROFILING)

extern void TP_ThreadEnter(TC_TCB *thread);
extern void TP_ThreadExit(TC_TCB *thread);
extern void TP_ThreadCreate(TC_TCB *thread);

/* interface for tct.s */

void _NU_Idle_Hook(
		unsigned int reg_0,
		unsigned int u4_int_level,
		unsigned int *tcd_hisr,
		unsigned int *tcd_task)
{
	UNUSED(reg_0);
	UNUSED(u4_int_level);
	UNUSED(tcd_hisr);
	UNUSED(tcd_task);
}

void _NU_Schedule_Task_Hook(TC_TCB *curr_task)
{
	TP_ThreadEnter(curr_task);
}

/* exported fakeview interfaces */

void _RTProf_TaskStatus(TC_TCB* task, unsigned char status)
{
	/*
		RT_TASK_READY, RT_TASK_SLEEPING,
		RT_TASK_SUSPENDED, RT_TASK_WAITING
	*/
	switch (status)
	{
	case RT_TASK_SLEEPING:
	case RT_TASK_SUSPENDED:
	case RT_TASK_WAITING:
		TP_ThreadExit(task);
		break;
	case RT_TASK_READY:
	default:
		break;
	}
}

void _RTProf_Dispatch_LISR_No_INT_Lock(int vector)
{
	UNUSED(vector);
}

void _RTProf_RegisterLisr(int vector)
{
	UNUSED(vector);
}

void _RTProf_DumpTask(TC_TCB* task, unsigned char op)
{
	/*
		CREATE, DELETE, RESET, TERMINATE, RESUME, SUSPEND, SLEEP,
		RELINQUISH, CHANGE_PRIORITY, CHANGE_PREEMPTION,
		CHANGE_TIME_SLICE, CONTROL_SIGNALS, RECEIVE_SIGNALS,
		SEND_SIGNALS
	*/

	switch (op)
	{
	case RT_PROF_CREATE_TASK:
		TP_ThreadCreate(task);
		break;
	case RT_PROF_DELETE_TASK:
	case RT_PROF_RESET_TASK:
	case RT_PROF_TERMINATE_TASK:
	case RT_PROF_RESUME_TASK:
	case RT_PROF_SUSPEND_TASK:
	case RT_PROF_SLEEP:
	case RT_PROF_RELINQUISH:
	case RT_PROF_CHANGE_PRIORITY:
	case RT_PROF_CHANGE_PREEMPTION:
	case RT_PROF_CHANGE_TIME_SLICE:
	case RT_PROF_CONTROL_SIGNALS:
	case RT_PROF_RECEIVE_SIGNALS:
	case RT_PROF_SEND_SIGNALS:
	default:
		break;
	}
}

void _RTProf_DumpHisr(TC_HCB* task, unsigned char op)
{
	UNUSED(task);
	UNUSED(op);
}

void _RTProf_DumpSema(unsigned char op, SM_SCB* sema, unsigned char ret)
{
	UNUSED(op);
	UNUSED(sema);
	UNUSED(ret);
}

void _RTProf_DumpQueue(unsigned char op, QU_QCB* que, unsigned char ret)
{
	UNUSED(op);
	UNUSED(que);
	UNUSED(ret);
}

void _RTProf_DumpMailBox(unsigned char op, MB_MCB* mbox, unsigned char ret)
{
	UNUSED(op);
	UNUSED(mbox);
	UNUSED(ret);
}

void _RTProf_DumpMemoryPool(unsigned char op, DM_PCB* pool, unsigned char ret)
{
	UNUSED(op);
	UNUSED(pool);
	UNUSED(ret);
}

void _RTProf_DumpPipe(unsigned char op, PI_PCB* pipe, unsigned char ret)
{
	UNUSED(op);
	UNUSED(pipe);
	UNUSED(ret);
}

void _RTProf_DumpPartitionPool(unsigned char op, PM_PCB* ppol, unsigned char ret)
{
	UNUSED(op);
	UNUSED(ppol);
	UNUSED(ret);
}

void _RTProf_DumpEventGroup(unsigned char op, EV_GCB* evnt, unsigned char ret)
{
	UNUSED(op);
	UNUSED(evnt);
	UNUSED(ret);
}

void _RTProf_DumpTimer(unsigned char op, TM_APP_TCB* timr, unsigned char ret)
{
	UNUSED(op);
	UNUSED(timr);
	UNUSED(ret);
}

void _RTProf_DumpDriver(unsigned char op, NU_DRIVER* drvr, unsigned char ret)
{
	UNUSED(op);
	UNUSED(drvr);
	UNUSED(ret);
}

#endif /* INCLUDE_PROVIEW */
