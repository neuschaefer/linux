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
 * $RCSfile: rec_util_thread.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/6 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: ab263bfe093fb6a9cf962d72692852e1 $
 *
 * Description:
 *         This file contains Event Context Selector specific functions.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#if defined(LINUX_TURNKEY_SOLUTION)//&& defined(MW_FBM_SUPPORT_PVR)  
#include <stdio.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#endif

#include "inc/common.h"
#include "os/inc/x_os.h"
#include "dbg/dbg.h"
#include "dbg/def_dbg_level_mw.h"

#include "rec_util/_rec_util.h"
#include "rec_util/rec_util_db.h"
#include "rec_util/rec_util_thread.h"
#include "rec_util/rec_util_msg.h"
#include "rec_util/rec_util_utils.h"

#include "rec_util/rec_util_cli.h"


 /*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define REC_UTIL_MSGQ_SND_MAX_CNT    (10)
#define REC_UTIL_PVR_FBM_ADDR        (0x0e128400)
#define REC_UTIL_PVR_FBM_BUFFER_SIZE (0x00380000)
typedef struct _REC_UTIL_PVR_BUF_POOL_T
{
    BOOL                      b_pool;           /*indicate if ring buffer is  pooled*/
    BOOL                      b_used;           /*indicate if pool is used*/
    UINT32                    ui4_buf_sz_align; /*pool alignment*/ 
    VOID*                     pv_buf_physi;     /*physic address*/
    SIZE_T                    z_size_physi;     /*physic alloc size*/
    REC_UTIL_PBA_BUF_INFO_T   t_buf_info_pool;  /*information of the riing buffer*/
} REC_UTIL_PVR_BUF_POOL_T;

 /*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static BOOL            b_rec_util_inited       = FALSE;
static THREAD_DESCR_T  t_rec_util_thread_descr = { DEFAULT_STACK_SIZE, 
                                                   DEFAULT_PRIORITY, 
                                                   DEFAULT_NUM_MSGS };

/* RecUtil Thread info */
static HANDLE_T        h_rec_util_thread      = NULL_HANDLE;
static HANDLE_T        h_rec_util_msg_q       = NULL_HANDLE;
static HANDLE_T        h_rec_util_lock_sema   = NULL_HANDLE;

REC_UTIL_SYS_CONFIG_T                   t_rec_util_sys_cfg;
static REC_UTIL_PVR_BUF_POOL_T          g_t_pvr_buf_pool;
 /*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
static VOID _rec_util_thread_main(VOID* pv_arg);

/*-----------------------------------------------------------------------------
 * Name:  rec_util_send_msg
 *
 * Description: This function sends message to record utility's queque and triggers
 *              thread to handle it.
 *
 * Inputs:  
 *       pt_msg : message
 *       
 * Outputs: NULL  
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-05-06 : initial
 ------------------------------------------------------------------------------*/
VOID rec_util_send_msg(const REC_UTIL_MSG_T* pt_msg)
{
    INT32       i4_ret = REC_UTILR_OK;
    UINT32      i4_i = 0;
    HANDLE_T    h_thread = NULL_HANDLE;

    /*Check parameters*/
    if (NULL == pt_msg)
    {
        REC_UTIL_DBG_ERR(("rec_util_send_msg:NULL message!\r\n"));
        return;
    }

    while (i4_i++ < REC_UTIL_MSGQ_SND_MAX_CNT)
    {
        i4_ret = x_msg_q_send(h_rec_util_msg_q,
                              (VOID*)pt_msg,
                              sizeof(REC_UTIL_MSG_T ),
                              0);  /* highest priority */
        if (i4_ret != OSR_OK)
        {
            i4_ret = x_thread_self(&h_thread);
            if ( (i4_ret != OSR_OK )
                 || ((i4_ret == OSR_OK) && (FALSE == x_handle_valid(h_thread))))
            {
                break;
            }

            /* Send message fail, wait 10 ms and retry */
            x_thread_delay( 10 );
            continue;
        }
        
        return;  /* success */
    }
    
    REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_SEND_TO_MSG_Q);
    return;
}


 
/*-----------------------------------------------------------------------------
                    static functions implementation
 ----------------------------------------------------------------------------*/
 /*-----------------------------------------------------------------------------
 * Name: _rec_util_thread_main
 *
 * Description: main function of record utility thread
 *
 * Inputs:  pv_arg References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
 static VOID _rec_util_thread_main(VOID* pv_arg)
{
    INT32               i4_ret;
    UINT16              ui2_index;
    REC_UTIL_MSG_T      t_msg;
    SIZE_T              z_msg_size; 
    BOOL                b_rec_util_exit = FALSE;
    UINT32              ui4_req_id;

    /* Create message queue for Record Utility Thread */
    i4_ret = x_msg_q_create( &h_rec_util_msg_q,
                             (CHAR*)REC_UTIL_MSGQ_NAME,
                             sizeof( REC_UTIL_MSG_T ),
                             t_rec_util_thread_descr.ui2_num_msgs );
    if ( i4_ret != OSR_OK )
    {
        REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_CREATE_MSG_QUEUE);
    }
    REC_UTIL_DBG_INFO(("_rec_util_thread_main:%s!\r\n", REC_UTIL_THREAD_NAME));
    
    while (!b_rec_util_exit) 
    {
        z_msg_size = sizeof(REC_UTIL_MSG_T);

        i4_ret = x_msg_q_receive(&ui2_index,
                                 &t_msg,
                                 &z_msg_size,
                                 &h_rec_util_msg_q,
                                 (UINT16)1,
                                 X_MSGQ_OPTION_WAIT );
        if (i4_ret == OSR_OK)
        {
            rec_util_lock_with_tag(REC_UTIL_TAG_MAIN_THREAD);  
            switch(t_msg.e_msg_type)
            {
                case REC_UTIL_MSG_EXIT:
                    b_rec_util_exit = TRUE;
                    break;
                    
                /*do cfg*/
                case REC_UTIL_MSG_DO_CFG:
                    i4_ret = rec_util_do_cfg(t_msg.u_data.t_do_cfg.h_rec_util, 
                                             &(t_msg.u_data.t_do_cfg));
                    if (REC_UTILR_OK != i4_ret)
                    {
                        REC_UTIL_DBG_ERR(("_rec_util_thread_main: do cfg fail!\r\n"));
                    }
                    break;
                    
                /*fm async write ack*/
                case REC_UTIL_MSG_FM_ASYNC_WRITE_ACK:
                    i4_ret = rec_util_do_db_write(t_msg.u_data.t_fm_async_write.h_rec_util, 
                                                  &(t_msg.u_data.t_fm_async_write),
                                                  &ui4_req_id);
                    if (REC_UTILR_OK != i4_ret)
                    {
                        REC_UTIL_DBG_ERR((  "_rec_util_thread_main: do db write "
                                            "fail(%d)!\r\n", i4_ret));
                    }
                    break;
                    
                /*abort ack*/
                case REC_UTIL_MSG_WRITE_TIMEOUT:
                    x_dbg_stmt( "{REC_UTIL} ERR: Write req [%d] timeout. data lost!\r\n", 
                            t_msg.u_data.t_write_timeout.ui4_abort_req_id);
                    i4_ret = rec_util_do_write_timeout(t_msg.u_data.t_write_timeout.h_rec_util, 
                                                        &(t_msg.u_data.t_write_timeout));
                    if (REC_UTILR_OK != i4_ret)
                    {
                        REC_UTIL_DBG_ERR((  "_rec_util_thread_main: do abort for "
                                            "timeout fail!\r\n"));
                    }
                    break;
                    
                /*Notify all clients*/
                case REC_UTIL_MSG_NFY_COND_CHG:
                    i4_ret = rec_util_nfy_all_clients(t_msg.u_data.t_cond_chg_nfy.h_db_head, 
                                                      REC_UTIL_NTFY_CODE_COND_CHANGED, 
                                                      NULL, 
                                                      (UINT32)&(t_msg.u_data.t_cond_chg_nfy));
                    if (REC_UTILR_OK != i4_ret)
                    {
                        REC_UTIL_DBG_ERR(("_rec_util_thread_main: cond chg nfy fail!\r\n"));
                    }
                    break;
                    
                case REC_UTIL_MSG_NFY_VALID_RANGE_UPDATED:
                    i4_ret = rec_util_nfy_all_clients(t_msg.u_data.t_valid_range_nfy.h_db_head, 
                                                      REC_UTIL_NTFY_CODE_VALID_RANGE_UPDATED, 
                                                      NULL, 
                                                      (UINT32)&(t_msg.u_data.t_valid_range_nfy));
                    if (REC_UTILR_OK != i4_ret)
                    {
                        REC_UTIL_DBG_ERR(("_rec_util_thread_main: valid range nfy fail!\r\n"));
                    }
                    break;
                    
                case REC_UTIL_MSG_NFY_VALID_RANGE_INFO_UPDATED:
                    i4_ret = 
                    rec_util_nfy_all_clients(t_msg.u_data.t_valid_range_info_nfy.h_db_head, 
                                                      REC_UTIL_NTFY_CODE_VALID_RANGE_INFO_UPDATED, 
                                                      NULL, 
                                                      0);
                    if (REC_UTILR_OK != i4_ret)
                    {
                        REC_UTIL_DBG_ERR(("_rec_util_thread_main: valid range info nfy fail!\r\n"));
                    }
                    break;
                    
                case REC_UTIL_MSG_NFY_RESET_REQ:
                    i4_ret = rec_util_nfy_all_clients(t_msg.u_data.t_reset_req_ack.h_db_head, 
                                                      REC_UTIL_NTFY_CODE_RESET_REQ, 
                                                      NULL, 
                                                      (UINT32)(t_msg.u_data.t_reset_req_ack.ui4_reset_req_id));
                    if (REC_UTILR_OK != i4_ret)
                    {
                        REC_UTIL_DBG_ERR(("_rec_util_thread_main: reset req nfy fail!\r\n"));
                    }
                    break;
                    
                /*Notify req client only*/
                /*
                            case REC_UTIL_MSG_NFY_LOAD_FILE_ACK:
                                i4_ret = rec_util_nfy_self_client(t_msg.h_rec_util, 
                                                                  REC_UTIL_NTFY_CODE_LOAD_FILE_ACK,
                                                                  NULL, 
                                                                  0);
                                if (REC_UTILR_OK != i4_ret)
                                {
                                    REC_UTIL_DBG_ERR(("_rec_util_thread_main: load file ack nfy fail!\r\n"));
                                }
                                break;
                                
                            case REC_UTIL_MSG_NFY_STORE_FILE_ACK:
                                i4_ret = rec_util_nfy_self_client(t_msg.h_rec_util, 
                                                                 REC_UTIL_NTFY_CODE_STORE_FILE_ACK,
                                                                  NULL, 
                                                                  0);
                                if (REC_UTILR_OK != i4_ret)
                                {
                                    REC_UTIL_DBG_ERR(("_rec_util_thread_main: store file ack nfy fail!\r\n"));
                                }
                                break;
                            */
                case REC_UTIL_MSG_NFY_WRITE_ACK:
                    i4_ret = rec_util_nfy_self_client(t_msg.u_data.t_write_ack.h_rec_util, 
                                                      REC_UTIL_NTFY_CODE_WRITE_ACK,
                                                      NULL, 
                                                      (UINT32)&(t_msg.u_data.t_write_ack));
                    if (REC_UTILR_OK != i4_ret)
                    {
                        REC_UTIL_DBG_ERR(("_rec_util_thread_main: write ack nfy fail!\r\n"));
                    }
                    break;
                    
                case REC_UTIL_MSG_NFY_SET_CFG_ACK:
                    i4_ret = rec_util_nfy_self_client(t_msg.u_data.t_set_cfg_ack.h_rec_util, 
                                                      REC_UTIL_NTFY_CODE_SET_CONFIG_ACK,
                                                      NULL, 
                                                      (UINT32)&(t_msg.u_data.t_set_cfg_ack));
                    if (REC_UTILR_OK != i4_ret)
                    {
                        REC_UTIL_DBG_ERR(("_rec_util_thread_main: set cfg ack nfy fail!\r\n"));
                    }
                    break;
                
            #ifdef REC_UTIL_AYNC_WRITE
                case REC_UTIL_MSG_ASYNC_WRITE_REQ:
                    i4_ret = rec_util_req_async_write_hdlr(t_msg.u_data.t_write_req.h_rec_util, 
                                                           &(t_msg.u_data.t_write_req.t_data), 
                                                           t_msg.u_data.t_write_req.ui4_req_id);
                     if (REC_UTILR_OK != i4_ret)
                    {
                        REC_UTIL_DBG_ERR(("_rec_util_thread_main: rec_util_write_req_hdlr fail!\r\n"));
                    }
                    break;
            #endif        
                default:
                    REC_UTIL_DBG_ERR(("_rec_util_thread_main: invalid msg type!\r\n"));
                    break;
            }
            rec_util_unlock_with_tag(REC_UTIL_TAG_MAIN_THREAD);
            
            if (b_rec_util_exit)
            {
                break;
            }
        }    
    } /* while: the main loop */

    /*need to do--delete rec util db objs*/
    rec_util_free_db_head_list();

    if (h_rec_util_msg_q != NULL_HANDLE)
    {
        x_msg_q_delete(h_rec_util_msg_q);
    }
    
    if (x_handle_valid(h_rec_util_lock_sema) == TRUE)
    {
        x_sema_delete(h_rec_util_lock_sema);
    }

    b_rec_util_inited = FALSE;

    x_thread_exit();
}

/*-----------------------------------------------------------------------------
                    public functions implementation
 ----------------------------------------------------------------------------*/
 /*-----------------------------------------------------------------------------
 * Name: evctx_get_msg_q_hdl
 *
 * Description: Get the MSGQ handle of rec utility thread
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: the MSGQ handle of rec utility thread
 *
 ----------------------------------------------------------------------------*/
HANDLE_T rec_util_get_msg_q_hdl (VOID)
{
    return h_rec_util_msg_q;
}

 /*-----------------------------------------------------------------------------
 * Name: rec_util_init
 *
 * Description: initialization function of Record Utility component
 *
 * Inputs:  pt_rec_util_thread_descr References.
 *
 * Outputs: - 
 *
 * Returns: REC_UTILR_OK               Success.
 *          REC_UTILR_INTERNAL_ERROR   Fail.
 *          REC_UTILR_ALREADY_INIT     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 rec_util_init(const THREAD_DESCR_T*   pt_rec_util_thread_descr)
{
    INT32                       i4_ret = REC_UTILR_INTERNAL_ERROR;

    if ( b_rec_util_inited )
    {
        return REC_UTILR_ALREADY_INIT;
    }
    
    if ( !pt_rec_util_thread_descr )
    {
        t_rec_util_thread_descr.ui1_priority = REC_UTIL_THREAD_DEFAULT_PRIORITY;
        t_rec_util_thread_descr.ui2_num_msgs = REC_UTIL_DEFAULT_NUM_OF_MSGS;
        t_rec_util_thread_descr.z_stack_size = REC_UTIL_THREAD_DEFAULT_STACK_SIZE;
    }
    else
    {
        t_rec_util_thread_descr.ui1_priority = 
            (pt_rec_util_thread_descr->ui1_priority == DEFAULT_PRIORITY)?
             REC_UTIL_THREAD_DEFAULT_PRIORITY:
             pt_rec_util_thread_descr->ui1_priority;

        t_rec_util_thread_descr.ui2_num_msgs = 
            (pt_rec_util_thread_descr->ui2_num_msgs == DEFAULT_NUM_MSGS)?
             REC_UTIL_DEFAULT_NUM_OF_MSGS:
             pt_rec_util_thread_descr->ui2_num_msgs;

        t_rec_util_thread_descr.z_stack_size = 
            (pt_rec_util_thread_descr->z_stack_size == DEFAULT_STACK_SIZE)?
             REC_UTIL_THREAD_DEFAULT_STACK_SIZE:
             pt_rec_util_thread_descr->z_stack_size;
    }

    i4_ret = x_sema_create(&h_rec_util_lock_sema,
                           X_SEMA_TYPE_BINARY,
                           X_SEMA_STATE_UNLOCK);
    if ( i4_ret != OSR_OK || h_rec_util_lock_sema == NULL_HANDLE )
    {
        REC_UTIL_DBG_ERR(("rec_util_init: sema create fail!\r\n"));
        return REC_UTILR_INTERNAL_ERROR;
    }
    
    i4_ret = x_thread_create( &h_rec_util_thread,
                              REC_UTIL_THREAD_NAME,
                              t_rec_util_thread_descr.z_stack_size,
                              t_rec_util_thread_descr.ui1_priority,
                              &_rec_util_thread_main,
                              0,
                              NULL );
    if ( i4_ret != OSR_OK )
    {
        REC_UTIL_DBG_ERR(("rec_util_init: thread create fail!\r\n"));
        x_sema_delete(h_rec_util_lock_sema);
        h_rec_util_lock_sema = NULL_HANDLE;
        return REC_UTILR_INTERNAL_ERROR;
    }
    
    /* Initialize Rec Utility CLI component */
    i4_ret = rec_util_cli_init();
    
    if (i4_ret != REC_UTILR_OK)
    {
        REC_UTIL_DBG_ERR(("rec_util_init: cli init fail!\r\n"));
        return REC_UTILR_INTERNAL_ERROR;
    }       

    b_rec_util_inited = TRUE;
    rec_util_set_dbg_level(DBG_INIT_LEVEL_MW_REC_UTIL);

    return REC_UTILR_OK;
}

 /*-----------------------------------------------------------------------------
 * Name: rec_util_exit
 *
 * Description: exit function of Record Utility component
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: REC_UTILR_OK               Success.
 *          REC_UTILR_INTERNAL_ERROR   Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 rec_util_exit(VOID)
{
    INT32               i4_ret;
    REC_UTIL_MSG_T      t_msg;
    HANDLE_T            h_msg_q;
    
    t_msg.e_msg_type = REC_UTIL_MSG_EXIT;
    h_msg_q = h_rec_util_msg_q;

    if (!b_rec_util_inited)
    {
        return REC_UTILR_NOT_INIT;
    }

    i4_ret = x_msg_q_send(h_msg_q,
                          &t_msg,
                          sizeof(REC_UTIL_MSG_T),
                          0); /* highest priority */

    if ( i4_ret != OSR_OK )
    {
        REC_UTIL_DBG_ERR(("rec_util_exit: send exit msg fail!\r\n"));
        return REC_UTILR_INTERNAL_ERROR;
    }

    return REC_UTILR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_is_inited
 *
 * Description: API to return the initialziation status of Record Utility component
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: TRUE        The Record Utility component had been initialized.
 *          FALSE       The Record Utility component is not initialized.
 *
 ----------------------------------------------------------------------------*/
BOOL rec_util_is_inited(VOID)
{
    return b_rec_util_inited;
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_lock_with_tag
 *
 * Description: API to lock the record utility
 *
 * Inputs:  ui1_tag    Specify the lock tag.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID rec_util_lock_with_tag(UINT8 ui1_tag)
{
    INT32 i4_ret;

    i4_ret = x_sema_lock(h_rec_util_lock_sema,
                         X_SEMA_OPTION_WAIT );
    if (i4_ret != OSR_OK)
    {
        if (b_rec_util_inited)
        {  
            /* should not fail when record utility inited */
            ui1_tag = ui1_tag;
            REC_UTIL_DBG_ERR(("rec_util_lock: sema lock fail(tag=%d)!\r\n", ui1_tag));
            DBG_ABORT(DBG_MOD_REC_UTIL);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_unlock
 *
 * Description: API to unlock the record utility.
 *
 * Inputs:  ui1_tag    Specify the unlock tag.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID rec_util_unlock_with_tag(UINT8 ui1_tag)
{
    INT32 i4_ret;

    i4_ret = x_sema_unlock(h_rec_util_lock_sema);
    if (i4_ret != OSR_OK)
    {
        if (b_rec_util_inited)
        {   
            /* should not fail when record utility inited */
            ui1_tag = ui1_tag;
            REC_UTIL_DBG_ERR(("rec_util_unlock: sema unlock fail(tag=%d)!\r\n", ui1_tag));
            DBG_ABORT(DBG_MOD_REC_UTIL);
        }
    }
}

BOOL rec_util_pvr_buf_is_pooled(const REC_UTIL_PBA_BUF_INFO_T* pt_buf_info_pool)
{
    if (!pt_buf_info_pool)
    {
        REC_UTIL_DBG_ERR(("rec_util_pvr_buf_is_pooled: pt_buf_info_pool is null\r\n"));
        return FALSE;
    }
    
    if (!g_t_pvr_buf_pool.b_pool)
    {
        REC_UTIL_DBG_INFO(("rec_util_pvr_buf_is_pooled: no pool!\r\n"));
        return FALSE;
    }
    if (((UINT32)(pt_buf_info_pool->pv_buf) 
           >= (UINT32)(g_t_pvr_buf_pool.t_buf_info_pool.pv_buf))
        && ((((UINT32)(pt_buf_info_pool->pv_buf)
              + (UINT32)(pt_buf_info_pool->z_size))) 
            <= ((UINT32)(g_t_pvr_buf_pool.t_buf_info_pool.pv_buf) 
              + (UINT32)(g_t_pvr_buf_pool.t_buf_info_pool.z_size))))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
INT32 rec_util_get_ring_buffer(REC_UTIL_PBA_BUF_INFO_T* pt_buf_info_pool,
                                     UINT32*                  pui4_buf_sz_align)
{
    if ((!pt_buf_info_pool) || (!pui4_buf_sz_align))
    {
        REC_UTIL_DBG_ERR(("rec_util_get_ring_buffer: pt_buf_info_pool is null\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    if (!g_t_pvr_buf_pool.b_pool)
    {
        REC_UTIL_DBG_ERR(("rec_util_get_ring_buffer: pt_buf_info_pool is not pooled\r\n"));
        return REC_UTILR_PVR_BUF_NOT_POOLED;    
    }
    
    *pui4_buf_sz_align = g_t_pvr_buf_pool.ui4_buf_sz_align;
    g_t_pvr_buf_pool.b_used = TRUE;
    x_memcpy(pt_buf_info_pool, &(g_t_pvr_buf_pool.t_buf_info_pool), sizeof(REC_UTIL_PBA_BUF_INFO_T));
    return REC_UTILR_OK;
}
INT32 rec_util_put_ring_buffer(const REC_UTIL_PBA_BUF_INFO_T* pt_buf_info_pool)
{
    if (!pt_buf_info_pool)
    {
        REC_UTIL_DBG_ERR(("rec_util_put_ring_buffer: pt_buf_info_pool is null\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    g_t_pvr_buf_pool.b_used = FALSE;
    return REC_UTILR_OK;
}

static BOOL _rec_util_pool_alloc ( VOID*                           pv_tag,
                                        const REC_UTIL_PBA_PARAM_T*     pt_param,
                                        REC_UTIL_PVR_BUF_POOL_T*        pt_pool_info)
{
#ifdef LINUX_TURNKEY_SOLUTION
/*defined (MW_FBM_SUPPORT_PVR) && defined (LINUX_TURNKEY_SOLUTION)*/
    INT32  fd_pvr            = -1;
    UINT32 ui4_physical_addr = 0;
    UINT32 ui4_buffer_size   = 0;
#endif
    if ((!pt_param) || (!pt_pool_info))
    {
        REC_UTIL_DBG_ERR(("_rec_util_pool_alloc:null parameter\r\n"));
        return FALSE;
    }
#if  1/*def MW_FBM_SUPPORT_PVR*/
    #ifdef LINUX_TURNKEY_SOLUTION
    if((fd_pvr = open("/dev/fbm_pvr",O_RDWR)) < 0)
    {
        x_dbg_stmt("\nRec_util_thread: Open fbm_pvr fail\n");

        REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
    }
    if(ioctl(fd_pvr,0,&ui4_physical_addr) < 0)
    {
        x_dbg_stmt("\nRec_util_thread: Get fbm_pvr addr fail\n");
    
        REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
    }
    if(ioctl(fd_pvr,1,&ui4_buffer_size) < 0 )
    {
        x_dbg_stmt("\nRec_util_thread: Get fbm_pvr size fail\n");
    
        REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
    }
    pt_pool_info->z_size_physi = ui4_buffer_size;
    pt_pool_info->pv_buf_physi = (void*)ui4_physical_addr;
    #else
    pt_pool_info->z_size_physi = REC_UTIL_PVR_FBM_BUFFER_SIZE;
    pt_pool_info->pv_buf_physi = (void*)REC_UTIL_PVR_FBM_ADDR;
    #endif
#else
    pt_pool_info->z_size_physi = pt_param->z_size + pt_param->ui4_mem_align;  
    pt_pool_info->pv_buf_physi = x_share_mem_alloc (pt_pool_info->z_size_physi);
#endif
    #ifndef LINUX_TURNKEY_SOLUTION
    if ( pt_pool_info->pv_buf_physi == NULL )
    #else
    if (pt_pool_info->pv_buf_physi == NULL ||
        (((UINT32) pt_pool_info->pv_buf_physi) + ((UINT32) pt_pool_info->z_size_physi)) >= 0x40000000 )
    #endif
    {
        return FALSE;
    }
    
    
    #ifndef LINUX_TURNKEY_SOLUTION
    pt_pool_info->t_buf_info_pool.pv_buf = 
            (VOID *) (((((UINT32) pt_pool_info->pv_buf_physi) + (pt_param->ui4_mem_align - 1)) / 
                        pt_param->ui4_mem_align) * pt_param->ui4_mem_align);
#if  1/*def MW_FBM_SUPPORT_PVR*/
    pt_pool_info->t_buf_info_pool.z_size = pt_pool_info->z_size_physi + (UINT32)pt_pool_info->pv_buf_physi - (UINT32)pt_pool_info->t_buf_info_pool.pv_buf;
#else
    pt_pool_info->t_buf_info_pool.z_size = pt_param->z_size;
#endif
    #else
    /*In linux here is at least 4K alignment , should satisfy pt_param->ui4_mem_align*/
    if (0 == (((UINT32) pt_pool_info->pv_buf_physi) % pt_param->ui4_mem_align))
    {
#if   1/*def MW_FBM_SUPPORT_PVR*/        
        pt_pool_info->t_buf_info_pool.pv_buf = pt_pool_info->pv_buf_physi;
        pt_pool_info->t_buf_info_pool.z_size = pt_pool_info->z_size_physi;
#else
        pt_pool_info->t_buf_info_pool.pv_buf = pt_pool_info->pv_buf_physi;
        pt_pool_info->t_buf_info_pool.z_size = pt_param->z_size;
#endif
    }
    else
    {
        /*here should  assert*/
        REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
    }
    #endif
#if  1 /*def MW_FBM_SUPPORT_PVR*/
    x_dbg_stmt("\nRec_util_thread:buffer addr %#0.12x,size %#x\n",pt_pool_info->t_buf_info_pool.pv_buf,pt_pool_info->t_buf_info_pool.z_size);
#endif
    return TRUE;
}

static VOID _rec_util_init_ring_buffer_pool(VOID)
{
    UINT16                        ui2_idx             = 0;
    REC_UTIL_BITRATE_MAP_ITEM_T*  pt_bitrate_item     = NULL;
    UINT32                        ui4_buf_sz_align;
    UINT32                        ui4_byte_per_second = 0;
    SIZE_T                        z_size;
    REC_UTIL_PBA_PARAM_T          t_pba_param;

    ui4_buf_sz_align  = rec_util_get_smallest_multi_num(REC_UTIL_LOGIC_BLOCK_SIZE,
                                                        REC_UTIL_DEF_STRG_BLOCK_SIZE);
    for (ui2_idx = 0; ui2_idx < t_rec_util_sys_cfg.ui2_bitrate_map_items; ++ui2_idx)
    {
         pt_bitrate_item = &(t_rec_util_sys_cfg.at_bitrate_map[ui2_idx]);
         if (pt_bitrate_item->ui4_byte_per_second > ui4_byte_per_second)
         {
            ui4_byte_per_second = pt_bitrate_item->ui4_byte_per_second;
         }
    }

    if (!ui4_byte_per_second)
    {
         REC_UTIL_DBG_ERR(("_rec_util_init_ring_buffer_pool:seach bitrate fail!\r\n"));
         REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
         return;
    }

    z_size  = (SIZE_T) ((((UINT64) t_rec_util_sys_cfg.ui4_pvr_buf_depth) 
                       * ((UINT64) ui4_byte_per_second)) / 1000);

    z_size  = ((z_size + (ui4_buf_sz_align - 1)) / ui4_buf_sz_align) * (ui4_buf_sz_align);

    t_pba_param.e_brdcst_medium = BRDCST_TYPE_UNKNOWN;
    t_pba_param.e_brdcst_type   = BRDCST_MEDIUM_UNKNOWN;
    t_pba_param.pv_rec_purpose  = NULL;
    /*default memory alignment : 32*/
    t_pba_param.ui4_mem_align   = 32; 
    t_pba_param.z_size          = z_size;

    x_memset((VOID*)&g_t_pvr_buf_pool, 0, sizeof(REC_UTIL_PVR_BUF_POOL_T));

    if (!(_rec_util_pool_alloc(t_rec_util_sys_cfg.pv_tag_pvr_buf_cb,
                               &t_pba_param,
                               &g_t_pvr_buf_pool)))
    {
         REC_UTIL_DBG_ERR(("_rec_util_init_ring_buffer_pool: alloc pvr buf failed!\r\n"));
         return;
    }
    g_t_pvr_buf_pool.b_pool = TRUE;
    g_t_pvr_buf_pool.ui4_buf_sz_align = ui4_buf_sz_align;

    return;
}

INT32 x_rec_util_init(
     const REC_UTIL_SYS_CONFIG_T*    pt_sys_cfg)

{
    INT32 i4_ret = REC_UTILR_OK;

    if(NULL == pt_sys_cfg)
    {
        REC_UTIL_DBG_ERR(("x_rec_util_init: param invalid!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    if( 1 != pt_sys_cfg->ui4_strct_ver || 
        0 != pt_sys_cfg->ui4_flags ||
        0 != pt_sys_cfg->ui4_write_fail_rate ||
        0 == pt_sys_cfg->ui2_bitrate_map_items ||
        NULL == pt_sys_cfg->at_bitrate_map ||
        0 == pt_sys_cfg->ui4_pvr_buf_depth ||
        NULL == pt_sys_cfg->t_fct_tbl_pvr_buf.pf_alloc_pvr_buf ||
        NULL == pt_sys_cfg->t_fct_tbl_pvr_buf.pf_free_pvr_buf ||
        NULL == pt_sys_cfg->t_fct_tbl_pvr_buf.pf_query_avail_pvr_buf)
    {
        REC_UTIL_DBG_ERR(("x_rec_util_init: Illegal REC_UTIL_SYS_CONFIG!\r\n"));
        return REC_UTILR_INV_ARG;
    }

    x_memcpy((VOID*)(&t_rec_util_sys_cfg), 
                     (VOID*)pt_sys_cfg, 
                     sizeof(REC_UTIL_SYS_CONFIG_T));
    /*if we need allocate ring buffer in advance in case failed to allocate later , -----now will do!*/
    _rec_util_init_ring_buffer_pool();

    rec_util_utils_init();
    
    i4_ret = rec_util_init(NULL);
    if (REC_UTILR_OK != i4_ret)
    {
        REC_UTIL_DBG_ERR(("x_rec_util_init: rec util init fail!\r\n"));
        return i4_ret;
    }    
    return i4_ret;
}

 
