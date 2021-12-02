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

#if ! defined(FAKEVIEW_H)
#define FAKEVIEW_H

#if defined(NUCLEUS_THREAD_PROFILING)

#include "../sm_defs.h"
#include "../qu_defs.h"
#include "../mb_defs.h"
#include "../dm_defs.h"
#include "../pi_defs.h"
#include "../pm_defs.h"
#include "../ev_defs.h"


typedef enum _FV_RETCODE_T
{
	RT_PROF_OK,
	RT_PROF_WAIT,
	RT_PROF_FAIL,
	/* last */
	RT_PROF_RETCODE_END
} FV_RETCODE_T;


/*_RTProf_DumpMemoryPool*/
typedef enum _FV_MEMORY_POOL_OP_T
{
	RT_PROF_CREATE_MEMORY_POOL,
	RT_PROF_DELETE_MEMORY_POOL,
	RT_PROF_ALLOCATE_MEMORY,
	RT_PROF_DEALLOCATE_MEMORY,
	/* last */
	RT_PROF_MEMOP_END
} FV_MEMORY_POOL_OP_T;


/* _RTProf_DumpEventGroup */
typedef enum _FV_EVENT_GROUP_OP_T
{
	RT_PROF_CREATE_EVENT_GROUP,
	RT_PROF_DELETE_EVENT_GROUP,
	RT_PROF_SET_EVENTS,
	RT_PROF_RETRIEVE_EVENTS,
	/* last */
	RT_PROF_EVENT_END
} FV_EVENT_GROUP_OP_T;

/* _RTProf_DumpDriver */
typedef enum _FV_DRIVER_OP_T
{
	RT_PROF_CREATE_DRIVER,
	RT_PROF_DELETE_DRIVER,
	RT_PROF_REQUEST_DRIVER,
	RT_PROF_RESUME_DRIVER,
	RT_PROF_SUSPEND_DRIVER,
	/* last */
	RT_PROF_DRIVER_END
} FV_DRIVER_OP_T;

/* _RTProf_DumpMailBox */
typedef enum _FV_MAIL_BOX_OP_T
{
	RT_PROF_CREATE_MAILBOX,
	RT_PROF_DELETE_MAILBOX,
	RT_PROF_SEND_TO_MAILBOX,
	RT_PROF_RECEIVE_FROM_MAILBOX,
	RT_PROF_RESET_MAILBOX,
	RT_PROF_BROADCAST_TO_MAILBOX,
	/* last */
	RT_PROF_MAILBOX_END
} FV_MAIL_BOX_OP_T;

/* _RTProf_DumpPipe */
typedef enum _FV_PIPE_OP_T
{
	RT_PROF_CREATE_PIPE,
	RT_PROF_DELETE_PIPE,
	RT_PROF_SEND_TO_PIPE,
	RT_PROF_RECEIVE_FROM_PIPE,
	RT_PROF_RESET_PIPE,
	RT_PROF_SEND_TO_FRONT_OF_PIPE,
	RT_PROF_BROADCAST_TO_PIPE,
	/* last */
	RT_PROF_PIPE_END
} FV_PIPE_OP_T;

/* _RTProf_DumpPartitionPool */
typedef enum _FV_PARTITION_POOL_OP_T
{
	RT_PROF_CREATE_PARTITION_POOL,
	RT_PROF_DELETE_PARTITION_POOL,
	RT_PROF_ALLOCATE_PARTITION,
	RT_PROF_DEALLOCATE_PARTITION,
	/* last */
	RT_PROF_PARTPOOL_END
} FV_PARTITION_POOL_T;

/* _RTProf_DumpQueue */
typedef enum _FV_QUEUE_OP_T
{
	RT_PROF_CREATE_QUEUE,
	RT_PROF_DELETE_QUEUE,
	RT_PROF_SEND_TO_QUEUE,
	RT_PROF_RECEIVE_FROM_QUEUE,
	RT_PROF_RESET_QUEUE,
	RT_PROF_SEND_TO_FRONT_OF_QUEUE,
	RT_PROF_BROADCAST_TO_QUEUE,
	/* last */
	RT_PROF_QUEUE_END
} FV_QUEUE_OP_T;

/* _RTProf_DumpSema */
typedef enum _FV_SEMA_OP_T
{
	RT_PROF_CREATE_SEMAPHORE,
	RT_PROF_DELETE_SEMAPHORE,
	RT_PROF_OBTAIN_SEMAPHORE,
	RT_PROF_RELEASE_SEMAPHORE,
	RT_PROF_RESET_SEMAPHORE,
	/* last */
	RT_PROF_SEMA_END
} FV_SEMA_OP_T;

/* _RTProf_DumpTask */
typedef enum _FV_TASK_OP_T
{
	RT_PROF_CREATE_TASK,
	RT_PROF_CREATE_HISR,
	RT_PROF_DELETE_TASK,
	RT_PROF_DELETE_HISR,
	RT_PROF_RESET_TASK,
	RT_PROF_TERMINATE_TASK,
	/*RT_TASK_READY,*/
	RT_PROF_RESUME_TASK,
	RT_PROF_SUSPEND_TASK,
	RT_PROF_SLEEP,
	RT_PROF_RELINQUISH,
	RT_PROF_CHANGE_PRIORITY,
	RT_PROF_CHANGE_PREEMPTION,
	RT_PROF_CHANGE_TIME_SLICE,
	RT_PROF_CONTROL_SIGNALS,
	RT_PROF_RECEIVE_SIGNALS,
	RT_PROF_SEND_SIGNALS,
	/* last */
	RT_PROF_TASK_END
} FV_TASK_OP_T;

typedef enum _FV_TASK_STATUS_T
{
	RT_TASK_READY,
	RT_TASK_SLEEPING,
	RT_TASK_SUSPENDED,
	RT_TASK_WAITING,
	/* last */
	RT_TASK_STATUS_END
} FV_TASK_STATUS_T;

/* _RTProf_DumpTimer */
typedef enum _FV_TIMER_OP_T
{
	RT_PROF_APP_TIMER_EXPIRED,
	RT_PROF_CREATE_TIMER,
	RT_PROF_DELETE_TIMER,
	RT_PROF_RESET_TIMER,
	RT_PROF_CONTROL_TIMER,
	/* last */
	RT_PROF_TIMER_END
} FV_TIMER_OP_T;

void _RTProf_TaskStatus					(TC_TCB* task, unsigned char status);
void _RTProf_Dispatch_LISR_No_INT_Lock	(int vector);
void _RTProf_RegisterLisr				(int vector);
void _RTProf_DumpTask					(TC_TCB* task, unsigned char op);
void _RTProf_DumpHisr					(TC_HCB* task, unsigned char op);
void _RTProf_DumpSema					(unsigned char op, SM_SCB* sema, unsigned char ret);
void _RTProf_DumpQueue					(unsigned char op, QU_QCB* que, unsigned char ret);
void _RTProf_DumpMailBox				(unsigned char op, MB_MCB* mbox, unsigned char ret);
void _RTProf_DumpMemoryPool				(unsigned char op, DM_PCB* pool, unsigned char ret);
void _RTProf_DumpPipe					(unsigned char op, PI_PCB* pipe, unsigned char ret);
void _RTProf_DumpPartitionPool			(unsigned char op, PM_PCB* ppol, unsigned char ret);
void _RTProf_DumpEventGroup				(unsigned char op, EV_GCB* evnt, unsigned char ret);
void _RTProf_DumpTimer					(unsigned char op, TM_APP_TCB* timr, unsigned char ret);
void _RTProf_DumpDriver					(unsigned char op, NU_DRIVER* drvr, unsigned char ret);

void _NU_Idle_Hook						(unsigned int reg_0, unsigned int u4_int_level, unsigned int *tcd_hisr, unsigned int *tcd_task);
void _NU_Schedule_Task_Hook				(TC_TCB *curr_task);

#endif /* NUCLEUS_THREAD_PROFILING */

#endif /* FAKEVIEW_H */
