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
 * $RCSfile: thread_lib.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains handle specific definitions, which are
 *         exported.
 *---------------------------------------------------------------------------*/

#ifndef _THREAD_LIB_H_
#define _THREAD_LIB_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "x_driver_os.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define THREAD_NAME_LEN         16
#define THREAD_PRI_RANGE_LOW    (UINT8) 1
#define THREAD_PRI_RANGE_HIGH   (UINT8) 255

#define FLAG_TH_CLOSE           0x01    /* indicate from x_thread_exit */
#define FLAG_TH_EV_GRP_CLOSE    0x02


/* The following structure contains the thread specific private data. */
typedef struct _OS_PVT_CTRL_T
{
    struct _OS_PVT_CTRL_T*  pt_next;      /* Double linked list. */
    struct _OS_PVT_CTRL_T*  pt_prev;

    x_os_thread_pvt_del_fct  pf_pvt_del;  /* Private data delete function. */

    VOID*  pv_pvt;                        /* Private data. */

    UINT32  ui4_key;                      /* Key. */
}   OS_PVT_CTRL_T;

typedef struct os_thread_obj
{
    struct os_thread_obj  *pt_next;            /* forward pointer */
    struct os_thread_obj  *pt_ev_grp_waiting;  /* next waiting */
    x_os_thread_main_fct  pf_main;
    VOID                  *pv_th_id;           /* thread id of driver */
    VOID                  *pv_arg;
    SIZE_T                z_stacksize;
    HANDLE_T              h_th_hdl;            /* Middleware handle */
    HANDLE_T              h_sema_hdl;          /* semaphore handle for msg Q */
    EV_GRP_OPERATION_T    e_ev_grp_wait_op;    /* ev group wait operation */
    EV_GRP_EVENT_T        e_ev_grp_event_wait; /* ev group event flag wait */
    EV_GRP_EVENT_T        e_ev_grp_event_got;  /* ev group event flag got */
    UINT8                 ui1_priority;
    UINT8                 ui1_flag;
    OS_PVT_CTRL_T*        pt_pvt_head;         /* References the head of the thread specific private data. */
    CHAR                  s_name[THREAD_NAME_LEN + 1];
} OS_THREAD_T;

extern BOOL b_thread_inited;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

INT32 os_check_handle(HANDLE_T, HANDLE_TYPE_T, VOID *);
INT32 thread_put_th_obj(OS_THREAD_T *);
BOOL thread_free_cb(HANDLE_T, HANDLE_TYPE_T, VOID *, VOID *, BOOL);
BOOL thread_get_th_obj_by_name(const CHAR *);
OS_THREAD_T * thread_get_th_obj_by_id(VOID *);
VOID thread_remove_th_obj(OS_THREAD_T *);
VOID thread_clean_up(OS_THREAD_T *);
x_os_drv_thread_main_fct thread_main(VOID);
INT32 _thread_init(VOID);

extern VOID thread_add_pvt_key (OS_THREAD_T*    pt_thread,
                                OS_PVT_CTRL_T*  pt_pvt_ctrl);
extern VOID thread_del_pvt_key (OS_THREAD_T*    pt_thread,
                                OS_PVT_CTRL_T*  pt_pvt_ctrl);
extern OS_PVT_CTRL_T* thread_find_pvt_key (OS_THREAD_T*  pt_thread,
                                           UINT32        ui4_key);

#endif /* _THREAD_LIB_H_ */
