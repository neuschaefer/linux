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
 * $RCSfile: thread_linux_drv.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains handle specific definitions, which are
 *         exported.
 *---------------------------------------------------------------------------*/

#ifndef _THREAD_LINUX_DRV_H
#define _THREAD_LINUX_DRV_H

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

/* Linux header files */
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <sched.h>
#include <pthread.h>


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*
 * Linux: the higher number, the higher priority
 * Middleware: the smaller number, the higher priority
 * the priority range may be different between the driver-OS and Middleware
 * a virtual mapinng may be needed
 */

/* scale is a relation between middleware and driver-os */

#define THREAD_PRI_MAX          256     /* Middleware pri 0..255 */

/*
 * p: the upper layer (middleware pri)
 * x: the converted pri from "p" to driver OS pri scheme
 * max, min: the driver OS layer pri range
 */
#define DRV_THREAD_PRI(x, p, max, min) \
{ \
    INT32 i4_t, i4_p; \
    i4_t = (UINT32) (p) * (max - min + 1); \
    i4_p = i4_t / THREAD_PRI_MAX; \
    if ((i4_t - (i4_p * THREAD_PRI_MAX)) >= (THREAD_PRI_MAX >> 1)) \
    { \
        ++i4_p; \
    } \
    if (max > min) \
    { \
        /* the higher number (max), the higher pri (x) */ \
        if (i4_p <= max) \
        { \
            x = (i4_p == 0) ? max : (max - i4_p + min); \
        } \
        else \
        { \
            x = min; \
        } \
    } \
    else  \
    { \
        x = i4_p + min; \
        if (x > max) \
        { \
            x = max; \
        } \
    } \
}


/*
 * p: the upper layer (middleware pri)
 * x: the driver OS pri
 * max, min: the driver OS layer pri range
 */
#define IN_THREAD_PRI(p, x, max, min) \
{ \
    if (max > min) \
    { \
        /* the higher number (max), the higher pri (x) */ \
        p = ((((max - ((UINT32) (x))) - min) * THREAD_PRI_MAX) / (max - min + 1)); \
    } \
    else \
    { \
        p = (((((UINT32) (x)) - min) * THREAD_PRI_MAX) / (max - min + 1)); \
    } \
}

/* Linux does not seem to support any suspend and resume */

#define OS_DRV_THREAD_WAIT_TBL_SIZE     32
#define OS_DRV_THREAD_WAIT_TBL_HASH     (OS_DRV_THREAD_WAIT_TBL_SIZE - 1)

#define OS_DRV_TH_GET_HASHTBL_INDEX(m) \
    (((UINT32) (m)) & OS_DRV_THREAD_WAIT_TBL_HASH)

#define OS_DRV_THREAD_OK        ((UINT8) 0x00)
#define OS_DRV_THREAD_ERROR     ((UINT8) 0xfe)

#define MAX_NAME_LEN 16

typedef struct thread_wait
{
    struct thread_wait   *pt_next;
    VOID        *pv_th_id;      /* thread id from underlying OS */
    VOID        *pv_tag;        /* tag value from thread creator */
    VOID        *pv_wait_id;    /* thread wait semaphore */
    VOID        (*pf_main)();   /* thread main function */
    UINT8       ui1_th_pri;     /* driver OS thread pri */
    UINT8       ui1_in_pri;     /* passed in pri */
    UINT8       ui1_flags;
    CHAR        s_name[MAX_NAME_LEN + 1];
} TH_WAIT_T;


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

void * os_drv_thread_main(void *);
TH_WAIT_T * os_drv_thread_waitsema(VOID *);
INT32 os_drv_thread_init(VOID);

#endif /* _THREAD_LINUX_DRV_H */

