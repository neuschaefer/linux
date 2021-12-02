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

#include "os/inc/x_os.h"
#include "inc/common.h"
#include "df_mngr.h"
#include "dfm_eng.h"
#include "x_df_mngr.h"
#include "dfm_lock.h"

#include "dbg/x_dbg.h"
#include "dbg/def_dbg_level_mw.h"
#include "dfm_dbg.h"


static HANDLE_T h_dfm_thread;   /* Thread. */
static HANDLE_T h_dfm_sema;     /* Semaphore. */
static HANDLE_T h_dfm_msg_q;    /* Message queue. */

/*-----------------------------------------------------------------------------
 * Name: dfm_msg_q_send
 *
 * Description: This API sends the specified message.
 *
 * Inputs:  pt_msg        References the message.
 *
 * Outputs: -
 *
 * Returns: TRUE                   Routine successful.
 ----------------------------------------------------------------------------*/
BOOL dfm_msg_q_send (DFM_MSG_T*  pt_msg)
{
    BOOL   b_msg_sent;
    UINT16 ui2_cnt;

    ui2_cnt    = 0;
    b_msg_sent = FALSE;

    while ((! (b_msg_sent))             &&
           (ui2_cnt < MSG_RETRY_COUNT))
    {
        b_msg_sent = (x_msg_q_send (h_dfm_msg_q,
                                    ((VOID*) pt_msg),
                                    sizeof (DFM_MSG_T),
                                    DFM_MSGQ_DEFAULT_PRIORITY) == OSR_OK);

        if (! (b_msg_sent))
        {
            /* Delay some time and retry sending te message. */
            x_thread_delay (MSG_RETRY_DELAY);

            ui2_cnt++;
        }
    }

    if (! (b_msg_sent))
    {
        /* Did not manage to send the message. */
        /* Something is seriously wrong.       */
        ABORT (DBG_CAT_MESSAGE, DBG_ABRT_CANNOT_SEND_MSG);
    }

    return b_msg_sent;
}



/*-----------------------------------------------------------------------------
 * Name: thread_main
 *
 * Description: This is the entry point of the DVB-SI engine thread. This
 *              API should NEVER return.
 *
 * Inputs:  pv_argv  Ignored.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID thread_main (VOID*  pv_argv)
{
    DFM_MSG_T           t_msg;
    UINT16              ui2_idx;
    SIZE_T              z_msg_size;
    LOGO_DATA_T*        pt_logo_data = NULL;
    LOGO_CTRL_T*        pt_logo_ctrl = NULL;
    INT32               i4_ret;

    /* Endless routine. This thread NEVER terminates. */
    while (TRUE)
    {
        z_msg_size = sizeof(t_msg);
        /* Set the message queue receive arguments. */
        if (x_msg_q_receive (&ui2_idx,
                             ((VOID*) &t_msg),
                             &z_msg_size,
                             &h_dfm_msg_q,
                             1,
                             X_MSGQ_OPTION_WAIT) == OSR_OK)
        {
            while (TRUE)
            {
                i4_ret = dfm_class_lock();
                if (i4_ret == DFMR_OK)
                {
                    break;
                }
                x_thread_delay (MSG_RETRY_DELAY);
                continue;
            }

            (VOID) x_sema_lock(h_dfm_sema, X_SEMA_OPTION_WAIT);

            switch (t_msg.e_msg_type)
            {
                case MSG_TYPE_SDT_OBJ_AVAILABLE:
                    pt_logo_ctrl = (LOGO_CTRL_T*)(t_msg.pv_data);
                    if (pt_logo_ctrl == NULL)
                    {
                        break;
                    }
                    DBG_INFO((_DFM_INFO"MSG: available, SDT Service_ID 0x%x.\n\r", pt_logo_ctrl->ui2_svc_id));
                    dfm_sdt_available_process(pt_logo_ctrl);                    
                    break;

                case MSG_TYPE_SDT_OBJ_UNAVAILABLE:
                    pt_logo_ctrl = (LOGO_CTRL_T*)(t_msg.pv_data);
                    if (pt_logo_ctrl == NULL)
                    {
                        break;
                    }
                    DBG_INFO((_DFM_INFO"MSG: unavailable, SDT Service_ID 0x%x.\n\r", pt_logo_ctrl->ui2_svc_id));
                    dfm_sdt_unavailable_process(pt_logo_ctrl);                  
                    break;
                
                case MSG_TYPE_CDT_OBJ_AVAILABLE:
                    pt_logo_data = (LOGO_DATA_T*)(t_msg.pv_data);
                    if (pt_logo_data == NULL)
                    {
                        break;
                    }
                    DBG_INFO((_DFM_INFO"MSG: available, CDT LOGO_ID 0x%x.\n\r", pt_logo_data->ui2_logo_id));
                    dfm_logo_available_process(pt_logo_data);                    
                    break;

                case MSG_TYPE_CDT_OBJ_UNAVAILABLE:
                    pt_logo_data = (LOGO_DATA_T*)(t_msg.pv_data);
                    if (pt_logo_data == NULL)
                    {
                        break;
                    }
                    DBG_INFO((_DFM_INFO"MSG: unavailable, CDT LOGO_ID 0x%x.\n\r", pt_logo_data->ui2_logo_id));
                    dfm_logo_unavailable_process(pt_logo_data);                 
                    break;
                    
                default:
                    ABORT (DBG_CAT_INV_OP, DBG_ABRT_UNEXPECTED_VALUE);
                    break;
            }

            (VOID) x_sema_unlock(h_dfm_sema);

            dfm_class_unlock ();
        }
        else
        {
            /* Abort when we have an error here. */
            ABORT (DBG_CAT_MESSAGE, DBG_ABRT_CANNOT_RECEIVE_MSG);
        }
    }
}


INT32 dfm_eng_init(VOID)
{
    /* Create the message queue. */
    if (x_msg_q_create (&h_dfm_msg_q, 
                        DFM_MSGQ_NAME, 
                        sizeof (DFM_MSG_T), 
                        256) != OSR_OK)
    {
        ABORT (DBG_CAT_INIT, DBG_ABRT_CANNOT_CREATE_MSG_Q);
    }

    /* Create a semaphore to sync with thread. */
    if (x_sema_create(&h_dfm_sema,
                      X_SEMA_TYPE_MUTEX,
                      X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        ABORT (DBG_CAT_INIT, DBG_ABRT_CANNOT_CREATE_SEMA);
    }

    /* Create the thread. */
    if (x_thread_create (&h_dfm_thread,
                         DFM_THREAD_NAME,
                         DFM_THREAD_DEFAULT_STACK_SIZE,
                         DFM_THREAD_DEFAULT_PRIORITY,
                         thread_main,
                         0,
                         NULL) != OSR_OK)
    {
        ABORT (DBG_CAT_INIT, DBG_ABRT_CANNOT_CREATE_THREAD);
    }

    return DFMR_OK;

}
