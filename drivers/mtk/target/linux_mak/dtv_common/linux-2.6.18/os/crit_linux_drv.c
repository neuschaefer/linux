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
 * $RCSfile: crit_linux_drv.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains handle specific definitions, which are
 *         exported.
 *---------------------------------------------------------------------------*/

/* driver-OS header files */
#include "x_start_common.h"
#include "driver_os.h"
#include "thread_linux_drv.h"
#include "sema_linux_drv.h"
#include "crit_linux_drv.h"

static CRIT_SEMA_T *pt_crit_sema = NULL;
static void *sema_id = NULL;

extern void *malloc();
extern void free();


/*----------------------------------------------------------------------------
 * Function: x_os_drv_crit_start()
 *
 * Description:
 *      this API performs
 *      1. perform acquiring mutex sema. 
 *      2. mask off interrupt.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      previous CPU status level.
 *---------------------------------------------------------------------------*/
 void * pv_cur_crit_th;
CRIT_STATE_T
x_os_drv_crit_start(VOID)
{
    VOID *pv_th_id;
    CRIT_STATE_T t_old;
    
#if 0
    (VOID) x_os_drv_sema_lock(pt_crit_sema->pv_sema_id,
                                  X_SEMA_OPTION_WAIT);
#endif
    /*x_os_drv_sema_lock(sema_id,X_SEMA_OPTION_WAIT);*/
    pv_th_id = (VOID *) pthread_self();

    /* allow multiple entries from the owner of the crit sema */    
    if (pv_th_id == pt_crit_sema->pv_th_id)
    {
        pt_crit_sema->i2_count++;
       
    }
    else
    {
        (VOID) x_os_drv_sema_lock(pt_crit_sema->pv_sema_id,
                                  X_SEMA_OPTION_WAIT);
        pv_cur_crit_th = pv_th_id;
    
        if (pt_crit_sema->pv_th_id == NULL)
        {
            pt_crit_sema->pv_th_id = pv_th_id;
            pt_crit_sema->i2_count = (INT16) 1;
        }
    }
    t_old = (CRIT_STATE_T)(pt_crit_sema->i2_count);
    
    /* mask all interrupts and save the current level */
    /*OS_DRV_MASK_INTR(t_old);*/
    /*x_os_drv_sema_unlock(sema_id);*/

    return(t_old);
}

                                                                                
/*----------------------------------------------------------------------------
 * Function: x_os_drv_crit_start()
 *
 * Description:
 *      this API performs
 *      1. perform releasing mutex sema. 
 *      2. restore interrupt.
 *
 * Inputs:
 *      t_old_level : previous CPU level
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID
x_os_drv_crit_end(CRIT_STATE_T t_old_level)
{
    VOID *pv_th_id;

#if 0
    (VOID) x_os_drv_sema_unlock(pt_crit_sema->pv_sema_id);
    x_os_drv_sema_lock(sema_id,X_SEMA_OPTION_WAIT);

    if (pt_crit_sema->i2_count != 0)
    {
            pt_crit_sema->i2_count = (INT16) 0;
            pt_crit_sema->pv_th_id = (VOID *) 0;
            (VOID) x_os_drv_sema_unlock(pt_crit_sema->pv_sema_id);
            
            return;
    }
#endif
    
    pv_th_id = (VOID *) pthread_self();

    if (pv_th_id == pt_crit_sema->pv_th_id)
    {
        /*CHUN for sim level passed*/
        if(pt_crit_sema->i2_count != (INT16)t_old_level)
        {
            printf("Level not as last\n");
        }
        pt_crit_sema->i2_count = (INT16)t_old_level;
        if (pt_crit_sema->i2_count > (INT16) 1)
        {
            pt_crit_sema->i2_count--;
        }
        else
        {
            pt_crit_sema->i2_count = (INT16) 0;
            pt_crit_sema->pv_th_id = (VOID *) 0;
            (VOID) x_os_drv_sema_unlock(pt_crit_sema->pv_sema_id);
        }
    }
    
    /* restore to saved interrupt level */
    /*OS_DRV_RESTORE_INTR(t_old);*/
/*    x_os_drv_sema_unlock(sema_id);*/
}


/*----------------------------------------------------------------------------
 * Function: os_drv_crit_init()
 *
 * Description:
 *      this API initialize critical section protection mechanism. it performs
 *      1. allocate vector table
 *      2. initialize each table entry.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_DRV_OK : routine was successful.
 *      OSR_DRV_FAIL: OS driver related error.
 *---------------------------------------------------------------------------*/
INT32
os_drv_crit_init()
{
    INT32 i4_ret;
 #if 0   
    if (pt_crit_sema != NULL)
        return 0;
    pt_crit_sema = (CRIT_SEMA_T *) malloc(sizeof(CRIT_SEMA_T));
    i4_ret = x_os_drv_sema_create(&(pt_crit_sema->pv_sema_id),
                                      X_SEMA_STATE_UNLOCK);
    if (i4_ret != OSR_DRV_OK)
    {
        return(INITR_FAIL);
    }
#endif
    i4_ret = x_os_drv_sema_create(&sema_id,X_SEMA_STATE_UNLOCK);
    if (pt_crit_sema != (CRIT_SEMA_T *) NULL)
    {
        if (pt_crit_sema->pv_sema_id != (VOID *) NULL)
        {
            (VOID) x_os_drv_sema_delete(pt_crit_sema->pv_sema_id);
        }
        free((VOID *) pt_crit_sema);
        pt_crit_sema = (CRIT_SEMA_T *) NULL;
    }

    pt_crit_sema = (CRIT_SEMA_T *) malloc(sizeof(CRIT_SEMA_T));

    if (pt_crit_sema != (CRIT_SEMA_T *) NULL)
    {
        pt_crit_sema->pv_sema_id = (VOID *) NULL;

        i4_ret = x_os_drv_sema_create(&(pt_crit_sema->pv_sema_id),
                                      X_SEMA_STATE_UNLOCK);
        if (i4_ret == OSR_DRV_OK)
        {
            pt_crit_sema->pv_th_id = (VOID *) NULL;
            pt_crit_sema->i2_count = (INT16) 0;
            return(INITR_OK);
        }
        else
        {
            free((VOID *) pt_crit_sema);
            pt_crit_sema = (CRIT_SEMA_T *) NULL;
            return(INITR_FAIL);
        }
    }
    else
    {
        return(INITR_FAIL);
    }
}


