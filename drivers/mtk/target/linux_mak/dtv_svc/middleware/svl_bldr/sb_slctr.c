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
 * $RCSfile: sb_slctr.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/21 $
 * $SWAuthor: Alec Lu $
 * $MD5HEX: e428000bf32f433be7657104f33115a1 $
 *
 * Description:
 *         This file contains SVL Builder selector specific functions.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/common.h"
#include "os/inc/x_os.h"
#include "dbg/dbg.h"
#include "dbg/def_dbg_level_mw.h"
#include "cli/x_cli.h"

#include "svl_bldr/_sb_eng.h"
#include "svl_bldr/_sb_handle.h"
#include "svl_bldr/_sb_msg.h"
#include "svl_bldr/sb_cli.h"
#include "svl_bldr/sb_slctr.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static BOOL            b_sb_inited        = FALSE;
static THREAD_DESCR_T  t_sb_thread_descr  = { DEFAULT_STACK_SIZE, 
                                              DEFAULT_PRIORITY, 
                                              DEFAULT_NUM_MSGS };

/* SVL Builder Selector Thread info */
static HANDLE_T        h_sb_slctr_thread  = NULL_HANDLE;
static HANDLE_T        h_slctr_msg_q      = NULL_HANDLE;

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
extern BOOL sb_attach_cmd_tbl(VOID);

static INT32 _sb_handle_autofree (HANDLE_T       h_handle,
                                  HANDLE_TYPE_T  e_type);

static VOID _sb_process_scan_nfy_msg(SB_SLCTR_MSG_T* pt_msg);

static VOID _sb_process_cond_chg_nfy_msg(SB_SLCTR_MSG_T* pt_msg);

static VOID _sb_slctr_thread_main(VOID* pv_arg);

/*-----------------------------------------------------------------------------
                    static functions implementation
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: _sb_handle_autofree
 *
 * Description: Autofree function.
 * Inputs:      -
 * Outputs:     -
 * Returns:     -
 *-----------------------------------------------------------------------------*/
static INT32 _sb_handle_autofree (HANDLE_T       h_handle,
                                  HANDLE_TYPE_T  e_type)
{
    INT32 i4_ret = SBR_INTERNAL_ERROR;

    if (e_type < HT_GROUP_SVL_BLDR || e_type >= SBT_LAST_ENTRY) {
        return HR_INV_HANDLE_TYPE;
    }

    switch (e_type) {
    case SBT_BUILDER:
        i4_ret = x_sb_close_builder(h_handle);
        break;

    case SBT_BASE_CTRL:
        /* This type handle is not exported. It shouldn't happen. */
        DBG_ABORT( DBG_MOD_SVL_BLDR );
        break;

    default:
        i4_ret = SBR_INV_HANDLE;
        break;
    }

    return (i4_ret == SBR_OK ? HR_OK : HR_INV_HANDLE);
}


/*-----------------------------------------------------------------------------
 * Name: _sb_process_scan_nfy_msg
 *
 * Description: This function is called by the selector's main function
 *              _sb_slctr_thread_main() when handling the 
 *              SB_SLCTR_MSG_ENG_SCAN_NFY type message.
 *
 * Inputs:  pt_msg  Contains the message content of SB_SLCTR_MSG_ENG_SCAN_NFY.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sb_process_scan_nfy_msg(SB_SLCTR_MSG_T* pt_msg)
{
    HANDLE_T    h_builder;

    if (pt_msg == NULL)
    {
        return;
    }

    if (pt_msg->e_msg_type != SB_SLCTR_MSG_ENG_SCAN_NFY)
    {
        return;
    }

    x_sb_base_ctrl_list_lock();

    if (sb_get_scan_starter_handle(
                        pt_msg->u_data.t_scan_nfy.h_builder_base,
                        &h_builder) == SBR_OK)
    {
        if (h_builder != NULL_HANDLE)
        {
            pt_msg->u_data.t_scan_nfy.pf_nfy(
                h_builder,
                pt_msg->u_data.t_scan_nfy.ui4_nfy_reason,
                pt_msg->u_data.t_scan_nfy.i4_nfy_data,
                pt_msg->u_data.t_scan_nfy.pv_nfy_tag);
        }
    }

    x_sb_base_ctrl_list_unlock();
}


/*-----------------------------------------------------------------------------
 * Name: _sb_process_cond_chg_nfy_msg
 *
 * Description: This function is called by the selector's main function
 *              _sb_slctr_thread_main() when handling the 
 *              SB_SLCTR_MSG_ENG_COND_CHG_NFY type message.
 *
 * Inputs:  pt_msg  Contains the message content of SB_SLCTR_MSG_ENG_COND_CHG_NFY.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sb_process_cond_chg_nfy_msg(SB_SLCTR_MSG_T* pt_msg)
{
    INT32               i4_ret;
    SB_BASE_CTRL_T*     pt_base_ctrl;

    if (pt_msg == NULL)
    {
        return;
    }

    if (pt_msg->e_msg_type != SB_SLCTR_MSG_ENG_COND_CHG_NFY)
    {
        return;
    }

    if (pt_msg->u_data.t_cond_chg_nfy.e_sb_cond == SB_COND_BUILDING)
    {   /* it's impossible that engines make the condition become building. */
        return;
    }

    x_sb_base_ctrl_list_lock();

    i4_ret = sb_get_base_ctrl_by_base_handle(
                pt_msg->u_data.t_cond_chg_nfy.h_builder_base,
                &pt_base_ctrl);
    if (i4_ret == SBR_OK)
    {
        if (pt_msg->u_data.t_cond_chg_nfy.e_sb_cond != SB_COND_UPDATE)
        {
            pt_base_ctrl->e_cond = pt_msg->u_data.t_cond_chg_nfy.e_sb_cond;
            /*pt_base_ctrl->h_scan_starter = NULL_HANDLE;*/
            /* AUTOSEARCH set vAutoSearchType to false*/
             if (sb_set_auto_search_type_by_base_ctrl(pt_base_ctrl, FALSE) != SBR_OK)
             {
                  DBG_ABORT( DBG_MOD_SVL_BLDR );            
             }
        }

        /*
            Justin: should traverse all handles which attach to 
            the same SB_BASE_CTRL_T.
        */
    
        i4_ret = handle_parse(
                    &(pt_base_ctrl->t_handle_link), 
                    pt_msg->u_data.t_cond_chg_nfy.e_sb_cond == SB_COND_UPDATE?
                        sb_handle_parse_update:
                        sb_handle_parse_cond_change, 
                    NULL);

        switch (i4_ret)
        {
        case HR_OK:
            i4_ret = SBR_OK;
            break;
    
        default:
            i4_ret = SBR_INTERNAL_ERROR;
            break;
        }
    }

    x_sb_base_ctrl_list_unlock();
}


/*-----------------------------------------------------------------------------
 * Name: _sb_slctr_thread_main
 *
 * Description: main function of SVL Builder Selector thread
 *
 * Inputs:  pv_arg References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sb_slctr_thread_main(VOID*  pv_arg)
{
    INT32               i4_ret;
    UINT16              ui2_index;
    SB_SLCTR_MSG_T      t_msg;
    SIZE_T              z_msg_size;
    BOOL                b_shutdown = FALSE;

    /* Create message queue for SVL Builder Selector Thread */
    i4_ret = x_msg_q_create( &h_slctr_msg_q,
                             (CHAR*)SB_SLCTR_MSGQ_NAME,
                             sizeof( SB_SLCTR_MSG_T ),
                             t_sb_thread_descr.ui2_num_msgs );
    if ( i4_ret != OSR_OK )
    {
        DBG_ABORT( DBG_MOD_SVL_BLDR );
        /* program abort here. */
    }

    while( !b_shutdown )
    {
        SB_ENG_CTRL_T*      pt_eng;

        z_msg_size = sizeof( SB_SLCTR_MSG_T );

        i4_ret = x_msg_q_receive( &ui2_index,
                                  &t_msg,
                                  &z_msg_size,
                                  &h_slctr_msg_q,
                                  (UINT16)1,
                                  X_MSGQ_OPTION_WAIT );
        if ( i4_ret == OSR_OK )
        {
            switch( t_msg.e_msg_type )
            {
            case SB_SLCTR_MSG_SHUTDOWN:
                b_shutdown = TRUE;
                break;
                
            case SB_SLCTR_MSG_ACTIVATE_ENG_TASK:
                pt_eng = sb_find_eng_by_name(t_msg.u_data.t_eng_task.s_eng_name);
                if (pt_eng != NULL)
                {
                    pt_eng->t_sb_fct_tbl.pf_engine_task();
                }
                break;

            case SB_SLCTR_MSG_ENG_SCAN_NFY:
                _sb_process_scan_nfy_msg(&t_msg);
                break;

            case SB_SLCTR_MSG_ENG_COND_CHG_NFY:
                _sb_process_cond_chg_nfy_msg(&t_msg);
                break;

            default:
                break;
            }
            if (b_shutdown)
            {
                break;
            }
        }
    #if 0
        else if ( i4_ret != OSR_NO_MSG )
        {
            DBG_ABORT( DBG_MOD_SVL_BLDR );
            /* program abort here. */
        }
    #endif

        x_thread_delay( 3 );
    } /* while: the main loop */

    /*
    Justin:
        should free all attached handles, SB_BASE_CTRL_T list,
        and engine list.
    */

    x_msg_q_delete( h_slctr_msg_q );
    sb_handle_exit();
    sb_eng_list_exit();

#ifdef  SB_LEAKAGE_TEST
    x_sb_lm_exit();
#endif  /* SB_LEAKAGE_TEST */

    x_thread_exit();
}

/*-----------------------------------------------------------------------------
                    public functions implementation
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: sb_init
 *
 * Description: initialization function of SVL Builder component
 *
 * Inputs:  pt_sb_thread_descr References.
 *
 * Outputs: - 
 *
 * Returns: SBR_OK              Success.
 *          SBR_INTERNAL_ERROR  Fail.
 *          SBR_ALREADY_INIT    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sb_init(
    const THREAD_DESCR_T*   pt_sb_thread_descr)
{
    INT32                       i4_ret;
    UINT16                      ui2_idx;
    static handle_autofree_fct  apf_autofree_fcts[SBT_LAST_ENTRY - HT_GROUP_SVL_BLDR] = {0};

    if ( b_sb_inited )
    {
        return SBR_ALREADY_INIT;
    }

    for (ui2_idx = 0; ui2_idx < (SBT_LAST_ENTRY - HT_GROUP_SVL_BLDR); ui2_idx++) {
        apf_autofree_fcts[ui2_idx] = _sb_handle_autofree;
    }

    i4_ret = handle_set_autofree_tbl(HT_GROUP_SVL_BLDR, apf_autofree_fcts);

    if ( i4_ret != HR_OK){
        return SBR_INTERNAL_ERROR;
    }

    /* SVL Builder Selector Thread */
    if ( !pt_sb_thread_descr )
    {
        t_sb_thread_descr.ui1_priority = SB_THREAD_DEFAULT_PRIORITY;
        t_sb_thread_descr.ui2_num_msgs = SB_DEFAULT_NUM_OF_MSGS;
        t_sb_thread_descr.z_stack_size = SB_THREAD_DEFAULT_STACK_SIZE;
    }
    else
    {
        t_sb_thread_descr.ui1_priority = 
            (pt_sb_thread_descr->ui1_priority == DEFAULT_PRIORITY)?
            SB_THREAD_DEFAULT_PRIORITY:
            pt_sb_thread_descr->ui1_priority;

        t_sb_thread_descr.ui2_num_msgs = 
            (pt_sb_thread_descr->ui2_num_msgs == DEFAULT_NUM_MSGS)?
            SB_DEFAULT_NUM_OF_MSGS:
            pt_sb_thread_descr->ui2_num_msgs;

        t_sb_thread_descr.z_stack_size = 
            (pt_sb_thread_descr->z_stack_size == DEFAULT_STACK_SIZE)?
            SB_THREAD_DEFAULT_STACK_SIZE:
            pt_sb_thread_descr->z_stack_size;
    }

    
    i4_ret = sb_eng_list_init();
    if ( i4_ret != SBR_OK )
    {
        return SBR_INTERNAL_ERROR;
    }

    i4_ret = sb_handle_init();
    if ( i4_ret != SBR_OK )
    {
        goto SB_INIT_EXIT;
    }

#ifdef  SB_LEAKAGE_TEST
    i4_ret = x_sb_lm_init();
    if (i4_ret != 0)
    {
        goto SB_INIT_EXIT;
    }
#endif /* SB_LEAKAGE_TEST */

    i4_ret = x_thread_create( &h_sb_slctr_thread,
                              SB_SLCTR_THREAD_NAME,
                              t_sb_thread_descr.z_stack_size,
                              t_sb_thread_descr.ui1_priority,
                              &_sb_slctr_thread_main,
                              0,
                              NULL );
    if ( i4_ret != OSR_OK )
    {
        i4_ret = SBR_INTERNAL_ERROR;
        goto SB_INIT_EXIT;
    }

    /* Initialize SVL Builder CLI component */
    i4_ret = sb_cli_init();
    
    if (i4_ret != SBR_OK)
    {
        return SBR_INTERNAL_ERROR;
    }       
    
    b_sb_inited = TRUE;
    x_sb_set_dbg_level(DBG_INIT_LEVEL_MW_SVL_BLDR);
    i4_ret = SBR_OK;

SB_INIT_EXIT:

    if (i4_ret != SBR_OK)
    {
        sb_handle_exit();
    
        sb_eng_list_exit();

#ifdef  SB_LEAKAGE_TEST
        x_sb_lm_exit();
#endif  /* SB_LEAKAGE_TEST */
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: sb_exit
 *
 * Description: shutdown the SB component main thread
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: SBR_OK              Success.
 *          SBR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sb_exit(VOID)
{
    HANDLE_T         h_msg_q;
    SB_SLCTR_MSG_T   t_msg;
    INT32            i4_ret;

    if (!b_sb_inited)
    {
        return SBR_NOT_INIT;
    }

    h_msg_q = sb_get_msg_q_hdl();

    t_msg.e_msg_type = SB_SLCTR_MSG_SHUTDOWN;

    i4_ret = x_msg_q_send(h_msg_q,
                          &t_msg,
                          sizeof(SB_SLCTR_MSG_T),
                          0); /* highest priority */

    if ( i4_ret != OSR_OK )
    {
        return SBR_INTERNAL_ERROR;
    }

    return SBR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: sb_get_msg_q_hdl
 *
 * Description: This API gets the handle of SB message queue.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: SB message queue handle.
 ----------------------------------------------------------------------------*/
HANDLE_T sb_get_msg_q_hdl(VOID)
{
    return h_slctr_msg_q;        
}


/*-----------------------------------------------------------------------------
 * Name: sb_is_inited
 *
 * Description: API to return the initialziation status of SB component
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: TRUE        The SB component had been initialized.
 *          FALSE       The SB component is not initialized.
 *
 ----------------------------------------------------------------------------*/
BOOL sb_is_inited(VOID)
{
    return b_sb_inited;
}


