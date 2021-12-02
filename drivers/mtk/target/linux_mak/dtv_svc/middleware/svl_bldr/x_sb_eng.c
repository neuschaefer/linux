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
 * $RCSfile: x_sb_eng.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: 8265dd08d6cb6e597784e9163e680343 $
 *
 * Description:
 *         This file contains SVL Builder Engine-related exported API's.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "dbg/u_dbg.h"
#include "util/u_sort_search.h"
#include "svl/u_svl.h"
#include "svl_bldr/_sb_eng.h"
#include "svl_bldr/_sb_handle.h"
#include "svl_bldr/x_sb_eng.h"
#include "svl_bldr/sb_slctr.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
static INT32 _sb_eng_cmp_by_chan_id (UINT32 ui4_chan_id_1,
                                     UINT32 ui4_chan_id_2);


/*-----------------------------------------------------------------------------
                    static functions implementation
 ----------------------------------------------------------------------------*/
static INT32 _sb_eng_cmp_by_chan_id (UINT32 ui4_chan_id_1,
                                     UINT32 ui4_chan_id_2)
{
    UINT16  ui2_major_1, ui2_major_2;

    ui2_major_1 = SB_ATSC_GET_MAJOR_CHANNEL_NUM(ui4_chan_id_1);
    ui2_major_2 = SB_ATSC_GET_MAJOR_CHANNEL_NUM(ui4_chan_id_2);

    if (ui2_major_1 < ui2_major_2)
    {
        return (RC_SMALLER);
    }
    else if (ui2_major_1 > ui2_major_2)
    {
        return (RC_GREATER);
    }
    else
    {
        UINT16 ui2_minor_1, ui2_minor_2;

        ui2_minor_1 = SB_ATSC_GET_MINOR_CHANNEL_NUM(ui4_chan_id_1);
        ui2_minor_2 = SB_ATSC_GET_MINOR_CHANNEL_NUM(ui4_chan_id_2);

        if (ui2_minor_1 < ui2_minor_2)
        {
            return (RC_SMALLER);
        }
        else if (ui2_minor_1 > ui2_minor_2)
        {
            return (RC_GREATER);
        }
        else
        {
            UINT8 ui1_order_1, ui1_order_2;
            
            if (SB_ATSC_IS_NON_PSIP_CHANNEL_ID(ui4_chan_id_1))
            {
                ui1_order_1 = 2;
            }
            else if (SB_ATSC_IS_ONE_PART_CHANNEL_ID(ui4_chan_id_1))
            {
                ui1_order_1 = 0;
            }
            else
            {
                ui1_order_1 = 1;
            }
            
            if (SB_ATSC_IS_NON_PSIP_CHANNEL_ID(ui4_chan_id_2))
            {
                ui1_order_2 = 2;
            }
            else if (SB_ATSC_IS_ONE_PART_CHANNEL_ID(ui4_chan_id_2))
            {
                ui1_order_2 = 0;
            }
            else
            {
                ui1_order_2 = 1;
            }

            if (ui1_order_1 < ui1_order_2)
            {
                return (RC_SMALLER);
            }
            else if (ui1_order_1 > ui1_order_2)
            {
                return (RC_GREATER);
            }
            else
            {
                UINT16 ui2_index_1, ui2_index_2;

                ui2_index_1 = SB_ATSC_GET_CHANNEL_INDEX(ui4_chan_id_1);
                ui2_index_2 = SB_ATSC_GET_CHANNEL_INDEX(ui4_chan_id_2);

                if (ui2_index_1 < ui2_index_2)
                {
                    return (RC_SMALLER);
                }
                else if (ui2_index_1 > ui2_index_2)
                {
                    return (RC_GREATER);
                }
                else
                {
                    return (RC_EQUAL);
                }
            }
        }
    }
}

/*-----------------------------------------------------------------------------
                    exported functions implementation
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * Name: x_sb_reg_engine
 *
 * Description: API to register a builder engine
 *
 * Inputs:  ps_eng_name     The engine name.
 *          pt_eng_fct_tbl  Contains the builder engine function table.
 *
 * Outputs: - 
 *
 * Returns: SBR_OK              Success.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_DUP_NAME        Fail. ps_eng_name is already used.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 x_sb_reg_engine(
    const CHAR*         ps_eng_name,
    SB_ENG_FCT_TBL_T*   pt_eng_fct_tbl)
{
    INT32 i4_ret;
    SB_ENG_CTRL_T*  pt_eng;

    if (ps_eng_name == NULL || pt_eng_fct_tbl == NULL)
    {
        return SBR_INV_ARG;
    }

    if (pt_eng_fct_tbl->pf_create_builder_data == NULL ||
        pt_eng_fct_tbl->pf_delete_builder_data == NULL ||
        pt_eng_fct_tbl->pf_match_builder_data == NULL ||
        pt_eng_fct_tbl->pf_engine_task == NULL ||
        pt_eng_fct_tbl->pf_start_scan == NULL ||
        pt_eng_fct_tbl->pf_cancel_scan == NULL ||
        pt_eng_fct_tbl->pf_save_lists == NULL ||
        pt_eng_fct_tbl->pf_clean_lists == NULL)
    {
        return SBR_INV_ARG;
    }

    if (!sb_is_inited())
    {
        return SBR_NOT_INIT;
    }

    /* Justin: should lock here. */
    sb_eng_list_lock();

    i4_ret = SBR_DUP_NAME;
    
    if (sb_find_eng_by_name(ps_eng_name) == NULL)
    {   /* means engine not found, add a new one */

        i4_ret = SBR_OUT_OF_MEM;

        pt_eng = (SB_ENG_CTRL_T*)x_sb_mem_alloc(sizeof(SB_ENG_CTRL_T));
        if (pt_eng != NULL)
        {
            /* init every byte to 0 */
            x_memset(((VOID*) pt_eng), 0, sizeof(SB_ENG_CTRL_T));
            
            /* set engine name */
            x_strncpy(pt_eng->s_name, ps_eng_name, SB_MAX_ENG_NAME_LEN);
            
            /* copy the function table */
            pt_eng->t_sb_fct_tbl = (*pt_eng_fct_tbl);

            sb_eng_list_add(pt_eng);

            i4_ret = SBR_OK;

        } /* allocate an engine node */
    } /* check whether if there's confliction */

    sb_eng_list_unlock();

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: x_sb_base_ctrl_list_lock
 *
 * Description: API to lock SB_BASE_CTRL_T list
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID x_sb_base_ctrl_list_lock( VOID )
{
    sb_base_ctrl_list_lock();
}


/*-----------------------------------------------------------------------------
 * Name: x_sb_base_ctrl_list_unlock
 *
 * Description: API to unlock SB_BASE_CTRL_T list
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID x_sb_base_ctrl_list_unlock( VOID )
{
    sb_base_ctrl_list_unlock();
}


/*-----------------------------------------------------------------------------
 * Name: x_sb_get_bldr_data
 *
 * Description: get the builder specific data which the handle uses.
 *
 * Inputs:  h_builder_base  Contains the handle of the builder base object.
 *
 * Outputs: ppv_bldr_data   Contains the builder data pointer.
 *
 * Returns: SBR_OK              Success.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_INV_HANDLE      Fail. Invalid builder handle.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 x_sb_get_bldr_data(
    HANDLE_T            h_builder_base,
    VOID**              ppv_bldr_data)
{
    INT32 i4_ret;
    SB_BASE_CTRL_T* pt_base_ctrl;

    if (ppv_bldr_data == NULL)
    {
        return SBR_INV_ARG;
    }

    i4_ret = sb_get_base_ctrl_by_base_handle(h_builder_base, &pt_base_ctrl);
    if (i4_ret == SBR_OK)
    {
    	if (NULL !=pt_base_ctrl->pv_data)
        {
            (*ppv_bldr_data) = pt_base_ctrl->pv_data;	
        }
        else
        {
           DBG_ERROR(("builder data is not ready\n\r"));
           i4_ret = SBR_INV_COND;
        }
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: x_sb_activate_eng_task_basic
 *
 * Description: API to ask the selector to execute the engine task.
 *
 * Inputs:  ps_eng_name     The requested engine name.
 *          b_retry         HISR callback should set this parameter to FALSE.
 *
 * Outputs: -
 *
 * Returns: SBR_OK              Success.
 *          SBR_INTERNAL_ERROR  Fail. Cannot send message to the selector.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 x_sb_activate_eng_task_basic(
    const CHAR*         ps_eng_name,
    BOOL                b_retry)
{
    INT32           i4_retry = 0;
    HANDLE_T        h_msg_q;
    SB_SLCTR_MSG_T  t_msg;

    if (ps_eng_name == NULL)
    {
        return SBR_INV_ARG;
    }

    if (!sb_is_inited())
    {
        return SBR_NOT_INIT;
    }

    x_memset(&t_msg, 0, sizeof(SB_SLCTR_MSG_T));

    t_msg.e_msg_type = SB_SLCTR_MSG_ACTIVATE_ENG_TASK;

    x_strncpy(t_msg.u_data.t_eng_task.s_eng_name, 
              ps_eng_name, 
              SB_MAX_ENG_NAME_LEN);

    h_msg_q = sb_get_msg_q_hdl();

    do
    {
        if (x_msg_q_send(h_msg_q,
                         &t_msg,
                         sizeof(SB_SLCTR_MSG_T),
                         0 /* highest priority */) == OSR_OK)
        {
            return SBR_OK;
        }
        
        if (!b_retry)
        {
            break;
        }

        /* Send message fail, wait 10 ms and retry for 10 times */
        x_thread_delay( 10 );
    } while ( ++i4_retry < 10 );

    return SBR_INTERNAL_ERROR;
}


/*-----------------------------------------------------------------------------
 * Name: x_sb_activate_eng_task
 *
 * Description: API to ask the selector to execute the engine task.
 *              Retry will be done if sending message to selector failed.
 *
 * Inputs:  ps_eng_name     The requested engine name.
 *
 * Outputs: -
 *
 * Returns: SBR_OK              Success.
 *          SBR_INTERNAL_ERROR  Fail. Cannot send message to the selector.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 x_sb_activate_eng_task(
    const CHAR*         ps_eng_name)
{
    return x_sb_activate_eng_task_basic(ps_eng_name, TRUE);
}


/*-----------------------------------------------------------------------------
 * Name: x_sb_activate_eng_task_by_timer
 *
 * Description: API to ask the selector to execute the engine task.
 *              Retry won't be done if sending message to selector failed.
 *              This API is used by the HISR callback function.
 *
 * Inputs:  ps_eng_name     The requested engine name.
 *
 * Outputs: -
 *
 * Returns: SBR_OK              Success.
 *          SBR_INTERNAL_ERROR  Fail. Cannot send message to the selector.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 x_sb_activate_eng_task_by_timer(
    const CHAR*         ps_eng_name)
{
    return x_sb_activate_eng_task_basic(ps_eng_name, FALSE);
}


/*-----------------------------------------------------------------------------
 * Name: x_sb_eng_cond_change_nfy
 *
 * Description: API to notify the selector of the SVL update.
 *
 * Inputs:  h_builder_base      The handle of builder base object.
 *          e_sb_cond           The new condition of the builder object.
 *
 * Outputs: -
 *
 * Returns: SBR_OK              Success.
 *          SBR_INTERNAL_ERROR  Fail. Cannot send message to the selector.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 x_sb_eng_cond_change_nfy(
    HANDLE_T            h_builder_base,
    SB_COND_T           e_sb_cond)
{
    INT32           i4_retry = 0;
    HANDLE_T        h_msg_q;
    SB_SLCTR_MSG_T  t_msg;

    if (!sb_is_inited())
    {
        return SBR_NOT_INIT;
    }

    x_memset(&t_msg, 0, sizeof(SB_SLCTR_MSG_T));

    t_msg.e_msg_type = SB_SLCTR_MSG_ENG_COND_CHG_NFY;
    t_msg.u_data.t_cond_chg_nfy.h_builder_base = h_builder_base;
    t_msg.u_data.t_cond_chg_nfy.e_sb_cond = e_sb_cond;

    h_msg_q = sb_get_msg_q_hdl();
    
    while ( ++i4_retry <= 10 )
    {
        if (x_msg_q_send(h_msg_q,
                        &t_msg,
                        sizeof(SB_SLCTR_MSG_T),
                        0 /* highest priority */) != OSR_OK)
        {
            /* Send message fail, wait 10 ms and retry for 10 times */
            x_thread_delay( 10 );
            continue;
        }

        return SBR_OK;
    }

    return SBR_INTERNAL_ERROR;
}



INT32 x_sb_eng_svl_sort_cmp (const SVL_REC_T*     pt_svl_rec_1,
                             const SVL_REC_T*     pt_svl_rec_2,
                             SVL_COMPARE_INFO_T   e_compare_info)
{
    return (_sb_eng_cmp_by_chan_id(
        pt_svl_rec_1->uheader.t_rec_hdr.ui4_channel_id,
        pt_svl_rec_2->uheader.t_rec_hdr.ui4_channel_id));
}


INT32 x_sb_eng_svl_search_cmp (const SVL_REC_T*     pt_svl_rec,
                               const VOID*          pv_data_to_compare,
                               SVL_COMPARE_INFO_T   e_compare_info)
{
    return (_sb_eng_cmp_by_chan_id(
        pt_svl_rec->uheader.t_rec_hdr.ui4_channel_id,
        (UINT32) pv_data_to_compare));
}


