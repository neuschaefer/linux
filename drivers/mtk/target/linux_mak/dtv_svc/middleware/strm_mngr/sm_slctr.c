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
 * $RCSfile: sm_slctr.c,v $
 * $Revision: #4 $
 * $Date: 2012/07/20 $
 * $Author: xingju.wang $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/14 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: 699661aa61c6f652e58018c4a8807650 $
 *
 * Description:
 *         This header file contains Stream Manager related API's.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/common.h"
#include "os/inc/x_os.h"
#include "handle/x_handle.h"
#include "dbg/x_dbg.h"
#include "dbg/dbg.h"
#include "dbg/u_dbg.h"

#include "strm_mngr/x_sm.h"
#include "strm_mngr/c_sm.h"
#include "strm_mngr/scc/c_scc.h"
#include "strm_mngr/scc/_scc.h"
#include "hash/hash_api.h"
#include "strm_mngr/sm_slctr.h"
#include "strm_mngr/sm_obsvr.h"
#include "strm_mngr/sess_gnrc_hdlr/sm_sess_gnrc.h"
#include "strm_mngr/cc_hdlr/sm_cc_hdlr.h"
#include "strm_mngr/video_hdlr/sm_video_hdlr.h"
#include "strm_mngr/audio_hdlr/sm_audio_hdlr.h"
#ifdef MW_TV_AV_BYPASS_SUPPORT
#include "strm_mngr/bypass_hdlr/sm_bypass_hdlr.h"
#endif

#include "strm_mngr/sm_drv_map.h"
#include "strm_mngr/sm_cli.h"
#include "strm_mngr/_sm_hc_tbl.h"
#include "strm_mngr/_sm_sess_tbl.h"
#include "strm_mngr/_sm_utils.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* message type definition */
typedef UINT32 SM_SLCTR_MSG_TYPE;

#define SM_SLCTR_MSG_IGNORE                        ((SM_SLCTR_MSG_TYPE) 0)
#define SM_SLCTR_MSG_SHUTDOWN                      ((SM_SLCTR_MSG_TYPE) 1)
#define SM_SLCTR_MSG_TRIGGER_TASK                  ((SM_SLCTR_MSG_TYPE) 2)
#define SM_SLCTR_MSG_CREATE_THREAD_FOR_HDLR        ((SM_SLCTR_MSG_TYPE) 3)

typedef struct _SM_SLCTR_MSG_TASK_INFO_T
{
    VOID*           pv_tag;
    sm_task_fct     pf_task;
} SM_SLCTR_MSG_TASK_INFO_T;

typedef struct _SM_SLCTR_MSG_CREATE_THREAD_INFO_T
{
    HANDLE_T          h_handler;
    THREAD_DESCR_T    t_thread_descr;
} SM_SLCTR_MSG_CREATE_THREAD_INFO_T;

typedef struct
{
    SM_SLCTR_MSG_TYPE   e_msg_type;

    union
    {
        SM_SLCTR_MSG_TASK_INFO_T           t_task_info;
        SM_SLCTR_MSG_CREATE_THREAD_INFO_T  t_thread_info;
        VOID*                              pv_data;
    } u;
} SM_SLCTR_MSG_T;

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

UINT16                 gui2_slctr_max_num_msgs = 0;
BOOL                   gb_sm_inited            = FALSE;
static THREAD_DESCR_T  gt_sm_thread_descr      = { DEFAULT_STACK_SIZE,
                                                   DEFAULT_PRIORITY,
                                                   DEFAULT_NUM_MSGS };


/* Stream Selector Thread info */
static HANDLE_T        gh_slctr_thread         = NULL_HANDLE;
static HANDLE_T        gh_slctr_msgq           = NULL_HANDLE;
static handle_autofree_fct apf_sm_autofree_fcts[HT_GROUP_SIZE];

static HANDLE_T   h_g_sync_stream_lock   = NULL_HANDLE;
HANDLE_T          h_g_sess_lock          = NULL_HANDLE;
HANDLE_T          h_g_stream_lock          = NULL_HANDLE;
/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

static VOID _sm_msg_create_thread_for_hdlr( SM_SLCTR_MSG_T* pt_msg );
static VOID _sm_slctr_thread_main( VOID*  pv_arg );

/*-----------------------------------------------------------------------------
                    static functions implementation
 ----------------------------------------------------------------------------*/
static VOID _sm_msg_create_thread_for_hdlr( SM_SLCTR_MSG_T* pt_msg )
{
    INT32                   i4_ret;
    HANDLE_TYPE_T           e_hdl_type;
    VOID*                   pv_obj;

    i4_ret = handle_get_type_obj( pt_msg->u.t_thread_info.h_handler,
                                  &e_hdl_type,
                                  (VOID**)&pv_obj );
    if ( i4_ret != HR_OK )
    {
        SM_ABORT( SM_DBG_ABRT_INV_HANDLER_HANDLE );
    }
    if ( !pv_obj )
    {
        SM_ABORT( SM_DBG_ABRT_INV_HANDLER_OBJ );
    }

    if ( e_hdl_type == SM_HT_SESS_HANDLER )
    {
        SM_SESS_HANDLER_T* pt_sess_hdlr_obj = (SM_SESS_HANDLER_T*)pv_obj;

        if ( pt_sess_hdlr_obj->b_run_task_as_thread &&
             pt_sess_hdlr_obj->h_hdlr_thread == NULL_HANDLE )
        {
            i4_ret = x_thread_create( &pt_sess_hdlr_obj->h_hdlr_thread,
                                      pt_sess_hdlr_obj->t_hdlr_desc.s_hdlr_name,
                                      pt_msg->u.t_thread_info.t_thread_descr.z_stack_size,
                                      pt_msg->u.t_thread_info.t_thread_descr.ui1_priority,
                                      pt_sess_hdlr_obj->t_fct_tbl.pf_task,
                                      sizeof(pt_sess_hdlr_obj),
                                      pt_sess_hdlr_obj );
            if ( i4_ret != OSR_OK )
            {
                SM_ABORT( SM_DBG_ABRT_CANNOT_CREATE_THREAD );
            }
        }
        else
        {
            SM_ABORT( SM_DBG_ABRT_INV_CASE_STMT );
        }
    }
    else if ( e_hdl_type == SM_HT_HANDLER )
    {
        SM_HANDLER_T* pt_hdlr_obj = (SM_HANDLER_T*)pv_obj;

        if ( pt_hdlr_obj->b_run_task_as_thread &&
             pt_hdlr_obj->h_hdlr_thread == NULL_HANDLE )
        {
            i4_ret = x_thread_create( &pt_hdlr_obj->h_hdlr_thread,
                                      pt_hdlr_obj->t_hdlr_desc.s_hdlr_name,
                                      pt_msg->u.t_thread_info.t_thread_descr.z_stack_size,
                                      pt_msg->u.t_thread_info.t_thread_descr.ui1_priority,
                                      pt_hdlr_obj->t_fct_tbl.pf_task,
                                      sizeof(pt_hdlr_obj),
                                      pt_hdlr_obj );
            if ( i4_ret != OSR_OK )
            {
                SM_ABORT( SM_DBG_ABRT_CANNOT_CREATE_THREAD );
            }
        }
        else
        {
            SM_ABORT( SM_DBG_ABRT_INV_CASE_STMT );
        }
    }
    else
    {
        /* error */
        SM_ABORT( SM_DBG_ABRT_INV_CASE_STMT );
    }
}

/*-----------------------------------------------------------------------------
 * Name: _sm_slctr_thread_main
 *
 * Description: <function description>
 *
 * Inputs:  pv_arg References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_slctr_thread_main( VOID*  pv_arg )
{
    INT32               i4_ret;
    UINT16              ui2_index;
    SM_SLCTR_MSG_T      t_msg;
    SIZE_T              z_msg_size;
    BOOL                b_shutdown = FALSE;
    SM_SESS_HANDLER_T** ppt_sess_hdlr = NULL;

    while( !b_shutdown )
    {
        z_msg_size = sizeof( SM_SLCTR_MSG_T );

        i4_ret = x_msg_q_receive( &ui2_index,
                                  &t_msg,
                                  &z_msg_size,
                                  &gh_slctr_msgq,
                                  (UINT16)1,
                                  X_MSGQ_OPTION_WAIT );
        if ( i4_ret == OSR_OK )
        {
            switch( t_msg.e_msg_type )
            {
            case SM_SLCTR_MSG_SHUTDOWN:
                b_shutdown = TRUE;
                break;

            case SM_SLCTR_MSG_TRIGGER_TASK:
                t_msg.u.t_task_info.pf_task( t_msg.u.t_task_info.pv_tag );
                break;

            case SM_SLCTR_MSG_CREATE_THREAD_FOR_HDLR:
                _sm_msg_create_thread_for_hdlr( &t_msg );
                break;

            default:
                break;
            }
            continue;
        }
        else if ( i4_ret != OSR_NO_MSG )
        {
            SM_ABORT( SM_DBG_ABRT_CANNOT_RECV_FROM_MSG_Q );
        }
    }

    x_mem_free( ppt_sess_hdlr );
    i4_ret = x_msg_q_delete( gh_slctr_msgq );

    if (x_handle_valid(h_g_sync_stream_lock) == TRUE) 
    {
        x_sema_delete(h_g_sync_stream_lock);
    }

    if (x_handle_valid(h_g_sess_lock) == TRUE) 
    {
        x_sema_delete(h_g_sess_lock);
    }
    if (x_handle_valid(h_g_stream_lock) == TRUE) 
    {
        x_sema_delete(h_g_stream_lock);
    }

    x_thread_exit();
}

/*-----------------------------------------------------------------------------
 * Name: _sm_aee_handle_autofree
 *
 * Description: This API calls c_sm_close_sess() or c_sm_close_stream() to
 *              close the handle.
 *
 * Inputs:  h_handle  Contains the handle to delete
 *          e_type    Contains the type of the handle.
 *
 * Outputs: -
 *
 * Returns: HR_OK  Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _sm_aee_handle_autofree( HANDLE_T       h_handle,
                                      HANDLE_TYPE_T  e_type )
{
    if ( e_type == SM_HT_SESSION )
    {
        /* -MHF Linux, lzhang, Jun/19/2009 */
        /* c_sm_close_sess(h_handle); */
        x_sm_close_sess(h_handle);
    }
    else if ( e_type == SM_HT_STREAM )
    {
        /* -MHF Linux, lzhang, Jun/19/2009 */
        /* c_sm_close_stream(h_handle); */
        x_sm_close_stream(h_handle);
    }
    else if ( e_type == SM_HT_SCC_GRP_NAME )
    {
        /* -MHF Linux, lzhang, Jun/19/2009 */
        /* c_scc_comp_close(h_handle); */
        x_scc_comp_close(h_handle);
    }
    else if ( e_type == SM_HT_SCC_STRM_HANDLE )
    {
        /* -MHF Linux, lzhang, Jun/19/2009 */
        /* c_scc_comp_close(h_handle); */
        x_scc_comp_close(h_handle);
    }
    else
    {
        SM_SLCTR_DBG_ERR((
                    "[SM] %s:%d invalid handle type 0x%.8X\r\n",
                    __FILE__, __LINE__, e_type ));
        SM_ASSERT(0);
    }

    return HR_OK;
}

/*-----------------------------------------------------------------------------
                    functions implementation
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: sm_init
 *
 * Description: <function description>
 *
 * Inputs:  pt_sm_thread_descr References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK              Success.
 *          SMR_INV_ARG         Fail.
 *          SMR_INTERNAL_ERROR  Fail.
 *          SMR_ALREADY_INIT    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_init( const THREAD_DESCR_T*      pt_sm_thread_descr )
{
    INT32                    i4_ret;
    UINT32                   ui4_i;

    if ( gb_sm_inited )
    {
        return SMR_ALREADY_INIT;
    }

    i4_ret = sm_dbg_init();
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

#ifdef CLI_LVL_ALL
    i4_ret = sm_cli_init();
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

#if 0 /* You can cancel to mark it for auto cli when power on*/
    extern VOID sm_cli_auto( VOID );
    sm_cli_auto();
#endif
    
#endif /* CLI_LVL_ALL */

    i4_ret = sm_hc_tbl_init();
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    i4_ret = sm_sess_tbl_init();
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    i4_ret = sm_drv_map_init();
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    /* Setup SCC API */
    i4_ret = scc_init();
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    /* Setup Stream Selector Thread parameters*/
    if ( !pt_sm_thread_descr )
    {
        gt_sm_thread_descr.ui1_priority = SM_THREAD_DEFAULT_PRIORITY;
        gt_sm_thread_descr.ui2_num_msgs = SM_DEFAULT_NUM_OF_MSGS;
        gt_sm_thread_descr.z_stack_size = SM_THREAD_DEFAULT_STACK_SIZE;
    }
    else
    {
        gt_sm_thread_descr = (*pt_sm_thread_descr);

        if ( pt_sm_thread_descr->ui1_priority == DEFAULT_PRIORITY )
        {
            gt_sm_thread_descr.ui1_priority = SM_THREAD_DEFAULT_PRIORITY;
        }
        if (  pt_sm_thread_descr->ui2_num_msgs == DEFAULT_NUM_MSGS )
        {
            gt_sm_thread_descr.ui2_num_msgs = SM_DEFAULT_NUM_OF_MSGS;
        }
        if (  pt_sm_thread_descr->z_stack_size == DEFAULT_STACK_SIZE )
        {
            gt_sm_thread_descr.z_stack_size = SM_THREAD_DEFAULT_STACK_SIZE;
        }
    }

    /* Create message queue for Stream Selector Thread */
    i4_ret = x_msg_q_create( &gh_slctr_msgq,
                             (CHAR*)SM_SLCTR_MSGQ_NAME,
                             sizeof( SM_SLCTR_MSG_T ),
                             gt_sm_thread_descr.ui2_num_msgs );
    if ( i4_ret != OSR_OK )
    {
        return SMR_INTERNAL_ERROR;
    }

    /* Start Stream Selector Thread */
    i4_ret = x_thread_create( &gh_slctr_thread,
                              SM_SLCTR_THREAD_NAME,
                              gt_sm_thread_descr.z_stack_size,
                              gt_sm_thread_descr.ui1_priority,
                              &_sm_slctr_thread_main,
                              0,
                              NULL );
    if ( i4_ret != OSR_OK )
    {
        return SMR_INTERNAL_ERROR;
    }

    /* set inited flag */
    gb_sm_inited = TRUE;

    /* register default handlers */
    i4_ret = sm_sess_gnrc_init();
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }
        
    i4_ret = sm_video_hdlr_init();
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }
    
    i4_ret = sm_audio_hdlr_init();
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    i4_ret = sm_bypass_hdlr_init();
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }
#endif

    i4_ret = x_sema_create( &h_g_sync_stream_lock,
                            X_SEMA_TYPE_BINARY,
                            X_SEMA_STATE_LOCK );
    if ( i4_ret != OSR_OK )
    {
        return SMR_INTERNAL_ERROR;
    }
    
    i4_ret = x_sema_create( &h_g_sess_lock,
                            X_SEMA_TYPE_MUTEX,
                            X_SEMA_STATE_UNLOCK );
    if ( i4_ret != OSR_OK )
    {
        return SMR_INTERNAL_ERROR;
    }
    i4_ret = x_sema_create( &h_g_stream_lock,
                            X_SEMA_TYPE_MUTEX,
                            X_SEMA_STATE_UNLOCK );
    if ( i4_ret != OSR_OK )
    {
        return SMR_INTERNAL_ERROR;
    }

    /* Setup AEE for "c_" APIs */
    for ( ui4_i = 0; ui4_i < HT_GROUP_SIZE; ++ui4_i )
    {
        apf_sm_autofree_fcts[ui4_i] = &_sm_aee_handle_autofree;
    }

    i4_ret = handle_set_autofree_tbl( HT_GROUP_SM, apf_sm_autofree_fcts );
    if ( i4_ret != HR_OK )
    {
        return SMR_INTERNAL_ERROR;
    }

    /* init the flag: gb_not_disable_tve_as_stop FALSE */
    gb_not_disable_tve_as_stop = FALSE;

    gb_enable_tve_as_play = TRUE;

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: sm_sync_stream_lock
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 sm_sync_stream_lock(UINT32 ui4_delay)
{
    INT32   i4_ret;
    
    i4_ret = _WAIT_TIMEOUT( h_g_sync_stream_lock, ui4_delay );
    if ( i4_ret != OSR_OK )
    {
       return SMR_INTERNAL_ERROR;
    }
    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: sm_sync_stream_unlock
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sm_sync_stream_unlock(VOID)
{
    INT32   i4_ret;
    
    i4_ret = x_sema_unlock( h_g_sync_stream_lock );
    if ( i4_ret != OSR_OK )
    {
        SM_ABORT( SM_DBG_ABRT_CANNOT_UNLOCK_SEMAPHORE );
    }
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_lock
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 sm_sess_lock(VOID)
{
    INT32   i4_ret;
    
    i4_ret = x_sema_lock( h_g_sess_lock, X_SEMA_OPTION_WAIT );
    if ( i4_ret != OSR_OK )
    {
       return SMR_INTERNAL_ERROR;
    }
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_unlock
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sm_sess_unlock(VOID)
{
    INT32   i4_ret;
    
    i4_ret = x_sema_unlock( h_g_sess_lock );
    if ( i4_ret != OSR_OK )
    {
        SM_ABORT( SM_DBG_ABRT_CANNOT_UNLOCK_SEMAPHORE );
    }
}
/*-----------------------------------------------------------------------------
 * Name: sm_stream_lock
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 sm_stream_lock(VOID)
{
    INT32   i4_ret;
    
    i4_ret = x_sema_lock( h_g_stream_lock, X_SEMA_OPTION_WAIT );
    if ( i4_ret != OSR_OK )
    {
       return SMR_INTERNAL_ERROR;
    }
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_unlock
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sm_stream_unlock(VOID)
{
    INT32   i4_ret;
    
    i4_ret = x_sema_unlock( h_g_stream_lock );
    if ( i4_ret != OSR_OK )
    {
        SM_ABORT( SM_DBG_ABRT_CANNOT_UNLOCK_SEMAPHORE );
    }
}
/*-----------------------------------------------------------------------------
 * Name: sm_clean_up
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_clean_up( VOID )
{
    SM_SLCTR_MSG_T   t_msg;
    INT32            i4_ret;

    t_msg.e_msg_type = SM_SLCTR_MSG_SHUTDOWN;

    i4_ret = x_msg_q_send( gh_slctr_msgq,
                           &t_msg,
                           sizeof( SM_SLCTR_MSG_T ),
                           0 ); /* highest priority */
    if ( i4_ret != OSR_OK )
    {
        return SMR_INTERNAL_ERROR;
    }

    gb_sm_inited = FALSE;

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_reg_strm_handler
 *
 * Description: <function description>
 *
 * Inputs:  pt_strm_hdlr_desc References.
 *          ps_sess_hdlr_name References.
 *          pt_fct_tbl        References.
 *          ph_strm_hdlr      References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK       Success.
 *          SMR_INV_ARG  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_reg_strm_handler( const SM_HDLR_DESC_T*    pt_strm_hdlr_desc,
                           const CHAR*              ps_sess_hdlr_name,
                           const SM_HDLR_FCT_TBL_T* pt_fct_tbl,
                           BOOL                     b_run_task_as_thread,
                           const THREAD_DESCR_T*    pt_thread_descr,
                           HANDLE_T*                ph_strm_hdlr )
{
    INT32           i4_ret;

    if ( !pt_strm_hdlr_desc || !ps_sess_hdlr_name || !pt_fct_tbl ||
         !ph_strm_hdlr )
    {
        return SMR_INV_ARG;
    }

    if ( !gb_sm_inited )
    {
        return SMR_NOT_INIT;
    }

    if ( !pt_fct_tbl->pf_open || !pt_fct_tbl->pf_close ||
         !pt_fct_tbl->pf_set || !pt_fct_tbl->pf_get )
    {
        return SMR_INV_ARG;
    }

    sm_hc_tbl_lock();

    i4_ret = sm_hc_tbl_reg_strm_hdlr( pt_strm_hdlr_desc,
                                      ps_sess_hdlr_name,
                                      pt_fct_tbl,
                                      b_run_task_as_thread,
                                      ph_strm_hdlr );

    sm_hc_tbl_unlock();

    if ( b_run_task_as_thread )
    {
        SM_SLCTR_MSG_T   t_msg;

        t_msg.e_msg_type = SM_SLCTR_MSG_CREATE_THREAD_FOR_HDLR;
        t_msg.u.t_thread_info.t_thread_descr.z_stack_size =
            ( (pt_thread_descr && pt_thread_descr->z_stack_size!=DEFAULT_STACK_SIZE )?
              pt_thread_descr->z_stack_size :
              gt_sm_thread_descr.z_stack_size );
        t_msg.u.t_thread_info.t_thread_descr.ui1_priority =
            ( (pt_thread_descr && pt_thread_descr->ui1_priority!=DEFAULT_PRIORITY )?
              pt_thread_descr->ui1_priority :
              gt_sm_thread_descr.ui1_priority );
        t_msg.u.t_thread_info.h_handler = (*ph_strm_hdlr);

        i4_ret = x_msg_q_send( gh_slctr_msgq,
                               &t_msg,
                               sizeof( SM_SLCTR_MSG_T ),
                               0 ); /* highest priority */
        if ( i4_ret != OSR_OK )
        {
            return SMR_INTERNAL_ERROR;
        }
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: sm_reg_sess_handler
 *
 * Description: <function description>
 *
 * Inputs:  pt_hdlr_desc References.
 *          pt_fct_tbl   References.
 *          ph_sess_hdlr References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK       Success.
 *          SMR_INV_ARG  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_reg_sess_handler( const SM_HDLR_DESC_T*          pt_hdlr_desc,
                           const SM_SESS_HDLR_FCT_TBL_T*  pt_fct_tbl,
                           BOOL                           b_run_task_as_thread,
                           const THREAD_DESCR_T*          pt_thread_descr,
                           HANDLE_T*                      ph_sess_hdlr )
{
    INT32           i4_ret;

    if ( !pt_hdlr_desc || !pt_fct_tbl || !ph_sess_hdlr )
    {
        return SMR_INV_ARG;
    }

    if ( !gb_sm_inited )
    {
        return SMR_NOT_INIT;
    }

    if ( !pt_fct_tbl->pf_open_sess || !pt_fct_tbl->pf_open_stream ||
         !pt_fct_tbl->pf_close || !pt_fct_tbl->pf_set || !pt_fct_tbl->pf_get ||
         !pt_fct_tbl->pf_strm_nfy )
    {
        return SMR_INV_ARG;
    }

    sm_hc_tbl_lock();

    i4_ret = sm_hc_tbl_reg_sess_hdlr( pt_hdlr_desc,
                                      pt_fct_tbl,
                                      b_run_task_as_thread,
                                      ph_sess_hdlr );

    sm_hc_tbl_unlock();

    if ( b_run_task_as_thread && i4_ret == SMR_OK )
    {
        SM_SLCTR_MSG_T   t_msg;

        t_msg.e_msg_type = SM_SLCTR_MSG_CREATE_THREAD_FOR_HDLR;
        t_msg.u.t_thread_info.t_thread_descr.z_stack_size =
            ( (pt_thread_descr && pt_thread_descr->z_stack_size!=DEFAULT_STACK_SIZE )?
              pt_thread_descr->z_stack_size :
              gt_sm_thread_descr.z_stack_size );
        t_msg.u.t_thread_info.t_thread_descr.ui1_priority =
            ( (pt_thread_descr && pt_thread_descr->ui1_priority!=DEFAULT_PRIORITY )?
              pt_thread_descr->ui1_priority :
              gt_sm_thread_descr.ui1_priority );
        t_msg.u.t_thread_info.h_handler = (*ph_sess_hdlr);

        i4_ret = x_msg_q_send( gh_slctr_msgq,
                               &t_msg,
                               sizeof( SM_SLCTR_MSG_T ),
                               0 ); /* highest priority */
        if ( i4_ret != OSR_OK )
        {
            return SMR_INTERNAL_ERROR;
        }
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_nfy
 *
 * Description: <function description>
 *
 * Inputs:  h_sess       References.
 *          e_cond_type  References.
 *          pv_slctr_tag References.
 *          e_event      References.
 *          ui4_data     References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *          SMR_INV_ARG         Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_sess_nfy( HANDLE_T              h_sess,
                   SM_SESS_COND_T        e_cond_type,
                   VOID*                 pv_slctr_tag,
                   SM_EVN_TYPE_T         e_event,
                   UINT32                ui4_data )
{
    INT32                     i4_ret;
    SM_USR_SESS_NFY_INFO_T*   pt_nfy_info;
    SM_SESSION_T*             pt_sess_obj = NULL;

    if ( !pv_slctr_tag )
    {
        return SMR_INV_ARG;
    }

    if ( e_cond_type == SM_SESS_COND_CLOSED )
    {
        /* get session object */
        pt_sess_obj = (SM_SESSION_T*)sm_get_sess_obj( h_sess );
        if ( !pt_sess_obj )
        {
            return SMR_INTERNAL_ERROR;
        }

        /* remove session from control table */
        sm_sess_tbl_lock();

        i4_ret = sm_sess_tbl_remove( pt_sess_obj->h_connection );

        sm_sess_tbl_unlock();

        if ( i4_ret != SMR_OK )
        {
            return SMR_INTERNAL_ERROR;
        }
    }

    /* notify application */
    pt_nfy_info = (SM_USR_SESS_NFY_INFO_T*)pv_slctr_tag;

    if ( pt_nfy_info->pf_usr_sess_nfy )
    {
        i4_ret = pt_nfy_info->pf_usr_sess_nfy( 
            h_sess,
            e_cond_type,
            pt_nfy_info->pv_usr_sess_nfy_tag,
            e_event,
            ui4_data );
    }

    sm_obsvr_notify(
            h_sess,
            e_cond_type,
            pt_nfy_info->pv_usr_sess_nfy_tag,
            e_event,
            ui4_data );

    if ( e_cond_type == SM_SESS_COND_CLOSED )
    {
        /* if condition is closed, free the tag, which is allocated in
           x_sm_open_sess() function. */
        pt_sess_obj->pv_sess_nfy_tag = NULL;
        x_mem_free( pv_slctr_tag );
    }

    return SMR_OK;
}

INT32 sm_stream_nfy( 
    HANDLE_T        h_stream,
    SM_COND_T       e_nfy_cond,
    VOID*           pv_nfy_tag,
    SM_EVN_TYPE_T   e_event,
    UINT32          ui4_data )
{
    x_stream_nfy_fct    pf_strm_nfy = (x_stream_nfy_fct)pv_nfy_tag;
    INT32               i4_ret = SMR_OK;
    SM_STREAM_T*                     pt_strm_base = NULL;

    pt_strm_base = sm_get_strm_obj( h_stream );
    if ( !pt_strm_base )
    {
        return SMR_INV_HANDLE;
    }

    if((e_nfy_cond == SM_SESS_COND_STARTED) || 
        ((e_nfy_cond == SM_SESS_COND_OPENED) && (pt_strm_base->b_is_sync_stream_stop == TRUE)) ||
        (e_nfy_cond == SM_SESS_COND_ERROR))
    {
        if(pt_strm_base->pf_sync_stream_unlock != NULL)
        {
            pt_strm_base->pf_sync_stream_unlock();
            pt_strm_base->pf_sync_stream_unlock = NULL;
            if(pt_strm_base->b_is_sync_stream_stop == TRUE)
            {
                pt_strm_base->b_is_sync_stream_stop = FALSE;
            }
        }
    }
    
    if ( pf_strm_nfy )
    {
        i4_ret = pf_strm_nfy(
            h_stream,
            e_nfy_cond,
            NULL,
            e_event,
            ui4_data );
    }
    
    sm_obsvr_notify(
            h_stream,
            e_nfy_cond,
            pv_nfy_tag,
            e_event,
            ui4_data );
    
    return i4_ret;
}


INT32 sm_trigger_task( HANDLE_T h_handler, VOID* pv_tag )
{
    INT32                     i4_ret;
    SM_SLCTR_MSG_T            t_msg;
    HANDLE_TYPE_T             e_hdl_type;
    sm_task_fct               pf_task;
    VOID*                     pv_obj;
    UINT32                    ui4_msg_send_cnt = 0;

    i4_ret = handle_get_type_obj( h_handler,
                                  &e_hdl_type,
                                  (VOID**)&pv_obj );
    if ( i4_ret != HR_OK )
    {
        SM_ABORT( SM_DBG_ABRT_INV_HANDLER_HANDLE );
    }
    if ( !pv_obj )
    {
        SM_ABORT( SM_DBG_ABRT_INV_HANDLER_OBJ );
    }

    if ( e_hdl_type == SM_HT_SESS_HANDLER )
    {
        SM_SESS_HANDLER_T* pt_sess_hdlr_obj = (SM_SESS_HANDLER_T*)pv_obj;
        pf_task = pt_sess_hdlr_obj->t_fct_tbl.pf_task;
    }
    else if ( e_hdl_type == SM_HT_HANDLER )
    {
        SM_HANDLER_T* pt_hdlr_obj = (SM_HANDLER_T*)pv_obj;
        pf_task = pt_hdlr_obj->t_fct_tbl.pf_task;
    }
    else
    {
        return SMR_INV_HANDLE;
    }


    t_msg.e_msg_type = SM_SLCTR_MSG_TRIGGER_TASK;
    t_msg.u.t_task_info.pf_task = pf_task;
    t_msg.u.t_task_info.pv_tag = pv_tag;

    /* Try send message for at least 20 times, if it fails */
    while( ++ui4_msg_send_cnt <= 20 )
    {
        UINT16   ui2_num_msgs = 0;

        i4_ret = x_msg_q_send( gh_slctr_msgq,
                               &t_msg,
                               sizeof( SM_SLCTR_MSG_T ),
                               10 );
        if ( i4_ret != OSR_OK )
        {
            HANDLE_T    h_thread = NULL_HANDLE;

            i4_ret = x_thread_self( &h_thread );
            if ( i4_ret != OSR_OK || !x_handle_valid( h_thread ) )
            {
                break;
            }

            /* Give up some time for other thread to process the messages */
            x_thread_delay( 20 );
            continue;
        }

        i4_ret = x_msg_q_num_msgs( gh_slctr_msgq, &ui2_num_msgs );
        if ( i4_ret != OSR_OK )
        {
            SM_SLCTR_DBG_ERR(( "[SM] x_msg_q_num_msgs() fail return %d", i4_ret ));
        }
        if ( ui2_num_msgs > gui2_slctr_max_num_msgs )
        {
            gui2_slctr_max_num_msgs = ui2_num_msgs;
        }

        return SMR_OK;
    }

    return SMR_INTERNAL_ERROR;
}

