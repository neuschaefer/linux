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
 * $RCSfile: cap.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/3 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: e17ac86ac37ac1a4cc1889d2d8f012de $ Asa
 *
 * Description: 
 *         This file contains all the image engine selector APIs   
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/ 
#ifdef _WIN32_
#include "inc/x_os.h"
#else
#include "os/inc/x_os.h"
#endif
#include "inc/x_common.h"
#include "inc/common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "dbg/x_dbg.h"
#include "cap/cap.h"
#include "cap/u_cap.h"
#include "cap/cap_cli.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static HANDLE_T     gh_notify_async_exe_thread = NULL_HANDLE;
static HANDLE_T     gh_notify_async_exe_msgq   = NULL_HANDLE;
static BOOL         gb_init                    = FALSE;


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name:  cap_logo_send_notify_msg
 *
 * Description: 
 *
 * Inputs:  
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/      
INT32 cap_logo_send_notify_msg (HANDLE_T                    h_cap,
                                CAP_LOGO_MSG_TYPE_T         e_msg_type,
                                CAP_EVENT_TYPE_T            e_event)
{
    INT32                               i4_ret;
    CAP_LOGO_NOTIFY_ASYNC_EXEC_MSG_T    t_msg;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (h_cap == NULL_HANDLE) 
    {
        return CAPR_INV_ARG;
    }
    /************************************************************************/
    /* Do send message                                                      */
    /************************************************************************/
    t_msg.h_cap         = h_cap;
    t_msg.e_event       = e_event;
    t_msg.e_msg_type    = e_msg_type;
    
    i4_ret = x_msg_q_send( gh_notify_async_exe_msgq,
                           &t_msg,
                           sizeof(CAP_LOGO_NOTIFY_ASYNC_EXEC_MSG_T),
                           10 );
    if ( i4_ret != OSR_OK )
    {
        return CAPR_MSG_Q_ERR;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: cap_logo_notify_async_exec_thread_main
 *
 * Description: 
 *
 * Inputs:  pv_arg References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID cap_logo_notify_async_exec_thread_main( VOID*  pv_arg )
{
    INT32                               i4_ret;
    SIZE_T                              z_msg_size;  
    UINT16                              ui2_index;
    CAP_LOGO_NOTIFY_ASYNC_EXEC_MSG_T    t_msg;
    HANDLE_TYPE_T                       e_handle_type;
    CAP_OBJ_T*                          pt_obj;

    z_msg_size = sizeof(CAP_LOGO_NOTIFY_ASYNC_EXEC_MSG_T);
        
    while(1)
    {
        i4_ret = x_msg_q_receive( &ui2_index,
                                  &t_msg,
                                  &z_msg_size,
                                  &gh_notify_async_exe_msgq,
                                  (UINT16)1,
                                  X_MSGQ_OPTION_WAIT );
        if ( i4_ret == OSR_OK )
        {
            i4_ret = handle_get_type_obj(t_msg.h_cap, &e_handle_type, (VOID*)&pt_obj);
            if ( (i4_ret != HR_OK)
                || (e_handle_type != HT_CAP_TYPE) 
                || (pt_obj == NULL))
            {
                continue;
            } 
            if (t_msg.e_msg_type == CAP_LOGO_MSG_NOTIFY_TYPE)
            {
                if (pt_obj->pf_nfy != NULL)
                {
                    pt_obj->pf_nfy( t_msg.h_cap, 
                                    pt_obj->pv_nfy_tag,
                                    t_msg.e_event, 
                                    0);
                }
            }
            else if (t_msg.e_msg_type == CAP_LOGO_MSG_ASYNC_EXEC_TYPE)
            {
                if(t_msg.e_event == CAP_EVENT_TYPE_DO_CAPTURING)
                {
                    x_cap_do_capturing(t_msg.h_cap);
                }
                else if(t_msg.e_event == CAP_EVENT_TYPE_DO_SAVING)
                {
                    x_cap_do_saving(t_msg.h_cap);
                }
                else
                {
                }
            }
            else
            {
            }
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name:  cap_init
 *
 * Description: This function is a selector's API. This function is called by system initial procedure when system boot-up to init media format recognizer module.
 *
 * Inputs:  NONe.
 * Outputs: NONE.
 *
 * Returns: CAPR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 cap_init (void)
{
    INT32 i4_ret;
    
    if (gb_init == TRUE)
    {
        return CAPR_OK;
    }

    if (gh_notify_async_exe_msgq == NULL_HANDLE)
    {
        /* Create notify and async execute msg queue */
        i4_ret = x_msg_q_create( &gh_notify_async_exe_msgq,
                                 (CHAR*)CAP_NFY_ASYNC_EXEC_MSG_Q_NAME,
                                 sizeof( CAP_LOGO_NOTIFY_ASYNC_EXEC_MSG_T ),
                                 CAP_NFY_ASYNC_EXEC_NUM_OF_MSGS );
        if ( i4_ret != OSR_OK )
        {
            ASSERT(0);
            return CAPR_INITIAL_ERR;
        }
    }

    if (gh_notify_async_exe_thread == NULL_HANDLE)
    {
        /* Start image Selector Thread */
        i4_ret = x_thread_create( &gh_notify_async_exe_thread,
                                  CAP_NFY_ASYNC_EXEC_THREAD_NAME,
                                  CAP_NFY_ASYNC_EXEC_THREAD_DEFAULT_STACK_SIZE,
                                  CAP_NFY_ASYNC_EXEC_THREAD_DEFAULT_PRIORITY,
                                  &cap_logo_notify_async_exec_thread_main,
                                  0,
                                  NULL );
        if ( i4_ret != OSR_OK )
        {
            ASSERT(0);
            return CAPR_INITIAL_ERR;
        }
    }

#ifdef CLI_LVL_ALL
    /* Initialize capture library CLI component */        
    if (cap_cli_init() != CAPR_OK)
    {
        return CAPR_CLI_ERROR;
    } 
#endif

    gb_init = TRUE;

    return CAPR_OK;
}




