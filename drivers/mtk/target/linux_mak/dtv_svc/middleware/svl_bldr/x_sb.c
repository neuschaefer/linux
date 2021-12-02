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
 * $RCSfile: x_sb.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/22 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: 569b2b344bef50af4e0a80b49df29325 $
 *
 * Description:
 *         This file contains SVL Builder related API's.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/common.h"
#include "os/inc/x_os.h"
#include "handle/x_handle.h"
#include "dbg/x_dbg.h"

#include "svl_bldr/_sb_eng.h"
#include "svl_bldr/_sb_handle.h"
#include "svl_bldr/_sb_msg.h"
#include "svl_bldr/sb_slctr.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*
    SB_START_SCAN_DATA is the structure used in pv_nfy_tag argument
    of the API (*sb_start_scan_fct) (...).  This structure "encapsulates"
    the notify function (pf_nfy) and private data (pv_nfy_tag.  NOTE:
    it is NOT the same with the pv_nfy_tag argument of the API
    (*sb_start_scan_fct) (...)), which are specified by the application.
 */
typedef struct _SB_START_SCAN_DATA
{
    x_sb_scan_nfy_fct   pf_nfy;
    VOID*               pv_nfy_tag;
} SB_START_SCAN_DATA;


/* List operations */
typedef enum
{
    SB_LIST_OP_SAVE = 0,
    SB_LIST_OP_LOAD,
    SB_LIST_OP_CLEAN
} SB_LIST_OP_T;

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static UINT16       ui2_sb_dbg_level;

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

static VOID _sb_eng_scan_nfy (
                    HANDLE_T        h_builder_base,
                    UINT32          ui4_nfy_reason,
                    INT32           i4_data,
                    VOID*           pv_nfy_tag);


static INT32 _sb_lists_op(
                    HANDLE_T        h_builder,
                    const CHAR*     ps_file_name,
                    SB_LIST_OP_T    e_list_op);

/*-----------------------------------------------------------------------------
                    static functions implementation
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: _sb_eng_scan_nfy
 *
 * Description: _sb_eng_scan_nfy is in the call back function type 
 *              (*sb_scan_nfy_fct) (...), which is called by the 
 *              builder engines.
 *
 * Inputs:  h_builder_base  Contains the handle of builder base object.
 *          ui4_nfy_reason  Contains the notify reason.
 *          i4_data         Contains data about the scan process.
 *          pv_nfy_tag      Contains the caller private tag.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sb_eng_scan_nfy (
     HANDLE_T           h_builder_base,
     UINT32             ui4_nfy_reason,
     INT32              i4_data,
     VOID*              pv_nfy_tag)
{
    INT32               i4_retry = 0;
    HANDLE_T            h_msg_q;
    SB_SLCTR_MSG_T      t_msg;
    SB_START_SCAN_DATA* pt_start_scan_data = (SB_START_SCAN_DATA *) pv_nfy_tag;

    if (pt_start_scan_data == NULL)
    {
        return;
    }

    h_msg_q = sb_get_msg_q_hdl();
    
    x_memset(&t_msg, 0, sizeof(SB_SLCTR_MSG_T));

    t_msg.e_msg_type = SB_SLCTR_MSG_ENG_SCAN_NFY;
    t_msg.u_data.t_scan_nfy.h_builder_base = h_builder_base;
    t_msg.u_data.t_scan_nfy.ui4_nfy_reason = ui4_nfy_reason;
    t_msg.u_data.t_scan_nfy.i4_nfy_data = i4_data;
    t_msg.u_data.t_scan_nfy.pf_nfy = pt_start_scan_data->pf_nfy;
    t_msg.u_data.t_scan_nfy.pv_nfy_tag = pt_start_scan_data->pv_nfy_tag;

    if ((ui4_nfy_reason & SB_REASON_COMPLETE) ||
        (ui4_nfy_reason & SB_REASON_CANCEL) ||
        (ui4_nfy_reason & SB_REASON_ABORT))
    {
        x_sb_mem_free((VOID*) pt_start_scan_data);
    }

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

        return;
    }

    /*DBG_ABORT( DBG_MOD_SVL_BLDR );*/
    /* program abort here. */
}


/*-----------------------------------------------------------------------------
 * Name: _sb_lists_op
 *
 * Description: function to perform list operations on the SVL and TSL.
 *
 * Inputs:  h_builder       The builder handle.
 *          ps_file_name    The file which stores the lists.
 *          e_list_op       The list operation to be performed.
 *
 * Outputs: -
 *
 * Returns: SBR_OK              Success.
 *          SBR_FILE_NOT_FOUND  Fail. The specified ps_file_name was not found.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_INV_COND        Fail. Invalid condition.
 *          SBR_INV_HANDLE      Fail. Invalid builder handle.
 *          SBR_LIST_NOT_FOUND  FAIL. The SVL/TSL doesn't exist in ps_file_name.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
static INT32 _sb_lists_op(HANDLE_T      h_builder,
                          const CHAR*   ps_file_name,
                          SB_LIST_OP_T  e_list_op)
{
    INT32               i4_ret;
    HANDLE_T            h_builder_base;
    SB_COND_T           e_builder_cond;
    SB_ENG_FCT_TBL_T    t_eng_fct_tbl;

    if (ps_file_name == NULL || ps_file_name[0] == '\x0')
    {
        return SBR_INV_ARG;
    }

    if (!x_sb_is_inited())
    {
        return SBR_NOT_INIT;
    }

    x_sb_base_ctrl_list_lock();

    i4_ret = x_sb_get_cond(h_builder, &e_builder_cond);
    if (i4_ret == SBR_OK)
    {
        switch (e_list_op)
        {
        case SB_LIST_OP_SAVE:
        case SB_LIST_OP_LOAD:
        case SB_LIST_OP_CLEAN:
            if (e_builder_cond != SB_COND_AVAIL && 
                e_builder_cond != SB_COND_UNCONFIG)
            {
                /* invalid condition for saving/loading/cleaning lists */
                i4_ret = SBR_INV_COND;
            }
            break;

        default:
            i4_ret = SBR_INV_ARG;
            break;
        }
    }

    if (i4_ret != SBR_OK)
    {
        goto SB_LOP_EXIT;
    }

    i4_ret = sb_get_eng_fct_tbl(h_builder, &t_eng_fct_tbl);

    if (i4_ret != SBR_OK)
    {
        goto SB_LOP_EXIT;
    }

    i4_ret = sb_get_builder_base_handle(h_builder,
                                        &h_builder_base);

SB_LOP_EXIT:

    if (i4_ret == SBR_OK)
    {
        switch (e_list_op)
        {
        case SB_LIST_OP_SAVE:
            i4_ret = t_eng_fct_tbl.pf_save_lists(h_builder_base, ps_file_name);
            break;

        case SB_LIST_OP_LOAD:
            i4_ret = t_eng_fct_tbl.pf_load_lists(h_builder_base, ps_file_name);
            break;

        case SB_LIST_OP_CLEAN:
            i4_ret = t_eng_fct_tbl.pf_clean_lists(h_builder_base, ps_file_name);
            break;

        default:
            /* no default */
            break;
        }
    }

    x_sb_base_ctrl_list_unlock();

    return i4_ret;
}

/*-----------------------------------------------------------------------------
                    functions implementation
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: x_sb_open_builder
 *
 * Description: Create a builder instance and get it's handle.
 *
 * Inputs:  ps_builder_name     The engine name.
 *          pv_open_data        Contains engine-specific data for opening
 *                              th builder.
 *          pv_nfy_tag          Contains the caller private tag.
 *          pf_nfy              The notification function, which will be 
 *                              called when the builder condition changes.
 *
 * Outputs: ph_buidler          Contains the builder handle.
 *          pe_builder_cond     Contains the current builder condition.
 *
 * Returns: SBR_OK              Success.
 *          SBR_BLDR_NOT_FOUND  Fail. The builder engine was not found.
 *          SBR_FILE_NOT_FOUND  Fail. The ps_file_name was not found.
 *          SBR_INTERNAL_ERROR  Fail. Internal error happens.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_LIST_NOT_FOUND  Fail. The named SVL doesn't exist.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *          SBR_OUT_OF_HANDLES  Fail. Run out of handles.
 *          SBR_OUT_OF_MEM      Fail. Run out of memory.
 *
 ----------------------------------------------------------------------------*/
INT32 x_sb_open_builder(
    const CHAR*         ps_builder_name,
    VOID*               pv_open_data,
    VOID*               pv_nfy_tag,
    x_sb_nfy_fct        pf_nfy,
    HANDLE_T*           ph_builder,
    SB_COND_T*          pe_builder_cond)
{
    INT32               i4_ret;
    SB_ENG_CTRL_T*      pt_eng = NULL;
    SB_BASE_CTRL_T*     pt_sb_base_ctrl = NULL;
    SB_CTRL_T*          pt_sb_ctrl = NULL;
    BOOL                b_new_base_ctrl = FALSE;
    BOOL                b_link_base_ctrl = FALSE;
    BOOL                b_link_handle = FALSE;
    HANDLE_T            h_builder = NULL_HANDLE;
    SB_COND_T           e_builder_cond;

    DBG_API((_SB_API"%s: builder name [%s], pv_open_data = 0x%x, pv_nfy_tag = 0x%x, pf_nfy = 0x%x, ph_builder = 0x%x, pe_builder_cond = 0x%x\n\r",
             __FUNCTION__, ps_builder_name, pv_open_data, pv_nfy_tag, pf_nfy, ph_builder, pe_builder_cond));

    if (ps_builder_name == NULL ||
        pv_open_data == NULL ||
        pf_nfy == NULL ||
        ph_builder == NULL ||
        pe_builder_cond == NULL)
    {
        return SBR_INV_ARG;
    }

    if (!x_sb_is_inited())
    {
        return SBR_NOT_INIT;
    }

    pt_eng = sb_find_eng_by_name(ps_builder_name);
    if (pt_eng == NULL)
    {
        return SBR_BLDR_NOT_FOUND;
    }

    x_sb_base_ctrl_list_lock();

    /* search the SB_BASE_CTRL_T list */
    if (sb_find_base_ctrl(pt_eng, pv_open_data, &pt_sb_base_ctrl) != SBR_OK)
    {
        i4_ret = SBR_INTERNAL_ERROR;
        goto SB_OB_EXIT;
    }

    /* if match, use the existed SB_BASE_CTRL_T */
    /* else if not, allocate a new SB_BASE_CTRL_T */
    if (pt_sb_base_ctrl == NULL)
    {
        pt_sb_base_ctrl = (SB_BASE_CTRL_T*) x_sb_mem_alloc(sizeof(SB_BASE_CTRL_T));
        if (pt_sb_base_ctrl == NULL)
        {
            i4_ret = SBR_OUT_OF_MEM;
            goto SB_OB_EXIT;
        }

        x_memset(((VOID*) pt_sb_base_ctrl), 0, sizeof(SB_BASE_CTRL_T));
        b_new_base_ctrl = TRUE;

        pt_sb_base_ctrl->pt_eng = pt_eng;
        pt_sb_base_ctrl->pv_data = NULL;
        pt_sb_base_ctrl->e_cond = SB_COND_INITING;
        pt_sb_base_ctrl->h_base_ctrl = NULL_HANDLE;
        pt_sb_base_ctrl->h_scan_starter = NULL_HANDLE;

        /* allocate a new handle for keeping this SB_BASE_CTRL unique */
        if (handle_alloc (SBT_BASE_CTRL /* Justin: */,
                          ((VOID*) pt_sb_base_ctrl),
                          NULL,
                          sb_free_handle_cb /* do nothing but return TRUE */,
                          &(pt_sb_base_ctrl->h_base_ctrl)) != HR_OK)
        {
            i4_ret = SBR_OUT_OF_HANDLES;
            goto SB_OB_EXIT;
        }

        if (handle_link_init(&(pt_sb_base_ctrl->t_handle_link)) != HR_OK)
        {
            i4_ret = SBR_INTERNAL_ERROR;
            goto SB_OB_EXIT;
        }

        i4_ret = sb_link_base_ctrl(pt_sb_base_ctrl);

        if (i4_ret != SBR_OK)
        {
            goto SB_OB_EXIT;
        }
        b_link_base_ctrl = TRUE;
    }

    /* allocate a new SB_CTRL_T, set the pf_nfy and pv_nfy_tag */
    pt_sb_ctrl = (SB_CTRL_T*) x_sb_mem_alloc(sizeof(SB_CTRL_T));
    if (pt_sb_ctrl == NULL)
    {
        i4_ret = SBR_OUT_OF_MEM;
        goto SB_OB_EXIT;
    }

    pt_sb_ctrl->pf_nfy = pf_nfy;
    pt_sb_ctrl->pv_nfy_tag = pv_nfy_tag;
    pt_sb_ctrl->pt_base_ctrl = pt_sb_base_ctrl;

    /* allocate a new handle for return */
    if (handle_alloc (SBT_BUILDER /* Justin: */,
                      ((VOID*) pt_sb_ctrl),
                      NULL,
                      sb_free_handle_cb /* do nothing but return TRUE */,
                      &h_builder) != HR_OK)
    {
        i4_ret = SBR_OUT_OF_HANDLES;
        goto SB_OB_EXIT;
    }

    pt_sb_ctrl->h_obj = h_builder;

    if (handle_link(&(pt_sb_base_ctrl->t_handle_link), h_builder) == HR_OK)
    {
        b_link_handle = TRUE;
    }
    else
    {
        i4_ret = SBR_INTERNAL_ERROR;
        goto SB_OB_EXIT;
    }

    /* call pt_eng->t_sb_fct_tbl.pf_create_builder_data to 
        create pv_builder_data */
    if (b_new_base_ctrl)
    {
        VOID* pv_builder_data;

        i4_ret = pt_eng->t_sb_fct_tbl.pf_create_builder_data(
                                        pt_sb_base_ctrl->h_base_ctrl,
                                        pv_open_data,
                                        &pv_builder_data,
                                        &e_builder_cond);
        if (i4_ret != SBR_OK)
        {
            goto SB_OB_EXIT;
        }

        pt_sb_base_ctrl->pv_data = pv_builder_data;
        pt_sb_base_ctrl->e_cond = e_builder_cond;
    } /* if (b_new_base_ctrl) */

    (*ph_builder) = h_builder;
    (*pe_builder_cond) = pt_sb_base_ctrl->e_cond;
    i4_ret = SBR_OK;

 SB_OB_EXIT:

    if (i4_ret != SBR_OK)
    {
        /* Justin: how to free handle? */
        if (h_builder != NULL_HANDLE)
        {
            if (b_link_handle && pt_sb_base_ctrl)
            {
                handle_delink(&(pt_sb_base_ctrl->t_handle_link), h_builder);
            }

            x_handle_free(h_builder);
            /* my implementation only frees the handle from the H_CTRL_T list.
               See its pf_free function sb_free_handle_cb(); */
        }

        if (pt_sb_ctrl)
        {
            /* should I free pt_sb_ctrl->pv_nfy_tag if we make a duplication. */
            x_sb_mem_free((VOID*) pt_sb_ctrl);
        }

        if (b_new_base_ctrl && pt_sb_base_ctrl)
        {
            if (pt_sb_base_ctrl->h_base_ctrl != NULL_HANDLE)
            {
                x_handle_free(pt_sb_base_ctrl->h_base_ctrl);
            }

            if (b_link_base_ctrl)
            {
                sb_delink_base_ctrl(pt_sb_base_ctrl);
            }

            if (pt_sb_base_ctrl->pv_data)
            {
                pt_eng->t_sb_fct_tbl.pf_delete_builder_data(
                                        pt_sb_base_ctrl->pv_data);
            }
            x_sb_mem_free((VOID*) pt_sb_base_ctrl);
        }
    }

    x_sb_base_ctrl_list_unlock();
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_sb_close_builder
 *
 * Description: delete a builder instance and free it's handle.
 *
 * Inputs:  h_builder       The builder handle.
 * Outputs: -
 *
 * Returns: SBR_OK              Success.
 *          SBR_INTERNAL_ERROR  Fail. Internal error happens.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_INV_HANDLE      Fail. Invalid builder handle.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 x_sb_close_builder(
    HANDLE_T            h_builder)
{
    INT32               i4_ret;
    SB_BASE_CTRL_T*     pt_sb_base_ctrl = NULL;
    SB_CTRL_T*          pt_sb_ctrl = NULL;
    x_sb_nfy_fct        pf_nfy = NULL;
    VOID*               pv_nfy_tag = NULL;

    DBG_API((_SB_API"%s: h_builder = 0x%x\n\r", __FUNCTION__, h_builder));

    if (!x_sb_is_inited())
    {
        return SBR_NOT_INIT;
    }

    x_sb_base_ctrl_list_lock();

    i4_ret = sb_get_sb_ctrl(h_builder, &pt_sb_ctrl);
    if (i4_ret != SBR_OK)
    {
        goto SB_CB_EXIT;
    }

    if (pt_sb_ctrl == NULL ||
        pt_sb_ctrl->pt_base_ctrl == NULL)
    {
        /*DBG_ABORT( DBG_MOD_SVL_BLDR );
         program abort here. */

        i4_ret = SBR_INV_HANDLE;
        goto SB_CB_EXIT;
    }

    pt_sb_base_ctrl = pt_sb_ctrl->pt_base_ctrl;
    pf_nfy = pt_sb_ctrl->pf_nfy;
    pv_nfy_tag = pt_sb_ctrl->pv_nfy_tag;

    if (pt_sb_base_ctrl->e_cond == SB_COND_BUILDING &&
        pt_sb_base_ctrl->h_scan_starter == h_builder)
    {
#if 1
        /* 
            I am the scan starter, but going to be closed,
            so cancel the scan process first.
        */
        if (pt_sb_base_ctrl->pt_eng)
        {
            pt_sb_base_ctrl->pt_eng->t_sb_fct_tbl.pf_cancel_scan(
                                            pt_sb_base_ctrl->h_base_ctrl);
        }

#else
        /* Just returns it a error "invalid condition". */
        i4_ret = SBR_INV_COND;
        goto SB_CB_EXIT;
#endif
    }

    i4_ret = handle_delink(&(pt_sb_base_ctrl->t_handle_link), h_builder);
    if (i4_ret != HR_OK)
    {
        i4_ret = SBR_INTERNAL_ERROR;
        goto SB_CB_EXIT;
    }

    pt_sb_ctrl->pt_base_ctrl = NULL;
    x_handle_free(h_builder);
    /* should I free pt_sb_ctrl->pv_nfy_tag if we make a duplication 
    when open the builder (see x_sb_open_builder() ) */
    x_sb_mem_free((VOID*) pt_sb_ctrl);

    if (handle_link_is_empty(&(pt_sb_base_ctrl->t_handle_link)) == TRUE)
    {
        /* no more handle attched to this pt_sb_base_ctrl */
        x_handle_free(pt_sb_base_ctrl->h_base_ctrl);

        sb_delink_base_ctrl(pt_sb_base_ctrl);
        if (pt_sb_base_ctrl->pv_data)
        {
            if (pt_sb_base_ctrl->pt_eng)
            {
                pt_sb_base_ctrl->pt_eng->t_sb_fct_tbl.pf_delete_builder_data(
                                                    pt_sb_base_ctrl->pv_data);
            }
            else /* error, actually! */
            {
                x_sb_mem_free((VOID*) pt_sb_base_ctrl->pv_data);
            }
        }
        x_sb_mem_free((VOID*) pt_sb_base_ctrl);
    }

    i4_ret = SBR_OK;

SB_CB_EXIT:

    x_sb_base_ctrl_list_unlock();
    
    if (i4_ret == SBR_OK)
    {
        pf_nfy(h_builder, SB_COND_CLOSED, pv_nfy_tag);
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_sb_start_scan
 *
 * Description: API to start a service/channel scan.
 *
 * Inputs:  h_builder       The builder handle.
 *
 *          pv_scan_data    Contains the builder engine specific scan data.
 *
 *          pv_nfy_tag      Contains the caller private tag.
 *
 *          pf_nfy          The notification function, which will be called 
 *                          during the scan.
 *
 * Outputs: pe_builder_cond Contains the current buidler condition.
 *
 * Returns: SBR_OK              Success.
 *          SBR_INTERNAL_ERROR  Fail. Internal error happens.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_INV_COND        Fail. Invalid condition.
 *          SBR_INV_HANDLE      Fail. Invalid builder handle.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 x_sb_start_scan(
    HANDLE_T            h_builder,
    VOID*               pv_scan_data,
    VOID*               pv_nfy_tag,
    x_sb_scan_nfy_fct   pf_nfy,
    SB_COND_T*          pe_builder_cond)
{
    INT32               i4_ret;
    HANDLE_T            h_builder_base = NULL_HANDLE;
    SB_COND_T           e_builder_cond;
    SB_ENG_FCT_TBL_T    t_eng_fct_tbl;
    SB_START_SCAN_DATA* pt_start_scan_data = NULL;

    DBG_API((_SB_API"%s: h_builder = 0x%x, pv_scan_data = 0x%x, pv_nfy_tag = 0x%x, pf_nfy = 0x%x, pe_builder_cond = 0x%x\n\r",
             __FUNCTION__, h_builder, pv_scan_data, pv_nfy_tag, pf_nfy, pe_builder_cond));

    if (pv_scan_data == NULL ||
        pf_nfy == NULL || 
        pe_builder_cond == NULL)
    {
        return SBR_INV_ARG;
    }

    if (!x_sb_is_inited())
    {
        return SBR_NOT_INIT;
    }

    x_sb_base_ctrl_list_lock();

    i4_ret = x_sb_get_cond(h_builder, &e_builder_cond);
    if (i4_ret == SBR_OK)
    {
        if (e_builder_cond == SB_COND_BUILDING)
        {
            i4_ret = SBR_INV_COND; /* invalid condition for scan */
        }
    }

    if (i4_ret != SBR_OK)
    {
        goto SB_UNLOCK;
    }

    i4_ret = sb_get_eng_fct_tbl(h_builder, &t_eng_fct_tbl);

    if (i4_ret != SBR_OK)
    {
        goto SB_UNLOCK;
    }

    i4_ret = sb_get_builder_base_handle(h_builder, &h_builder_base);
    if (i4_ret != SBR_OK)
    {
        goto SB_UNLOCK;
    }

    /* Justin:
    1. builder object should change the condition here, 
    since the scan had been started.
    2. also, send condiction change notificaton
    */
    i4_ret = sb_set_cond(h_builder, SB_COND_BUILDING);
    
    if (i4_ret != SBR_OK)
    {
        goto SB_UNLOCK;
        /*DBG_ABORT( DBG_MOD_SVL_BLDR );*/
        /* program abort here. */
    }

    /*i4_ret = */sb_call_nfy_fct(h_builder);
    /* Justin: if i4_ret != SBR_OK, should we stop the scan? */

    pt_start_scan_data = (SB_START_SCAN_DATA *) 
                            x_sb_mem_alloc(sizeof(SB_START_SCAN_DATA));

    if (pt_start_scan_data == NULL)
    {
        i4_ret = SBR_OUT_OF_MEM;
        if (sb_set_cond(h_builder, e_builder_cond) != SBR_OK)
        {
            DBG_ASSERT( 0, DBG_MOD_SVL_BLDR );
            /* program abort here. */
        }
        sb_call_nfy_fct(h_builder);
        goto SB_UNLOCK;
    }

    pt_start_scan_data->pf_nfy = pf_nfy;
    pt_start_scan_data->pv_nfy_tag = pv_nfy_tag;

    i4_ret = t_eng_fct_tbl.pf_start_scan(
                h_builder_base,
                pv_scan_data,
                pt_start_scan_data,
                _sb_eng_scan_nfy);

    if (i4_ret != SBR_OK)
    {
        if (sb_set_cond(h_builder, e_builder_cond) != SBR_OK)
        {
            DBG_ASSERT( 0, DBG_MOD_SVL_BLDR );
            /* program abort here. */
        }
        /*i4_ret = */sb_call_nfy_fct(h_builder);
        /* Justin: if i4_ret != SBR_OK, should we stop the scan? */
    }

SB_UNLOCK:

    x_sb_base_ctrl_list_unlock();

    /* Send notification of abortion */
    if (i4_ret != SBR_OK)
    {
        if (pt_start_scan_data != NULL)
        {
            x_sb_mem_free((VOID*) pt_start_scan_data);
        }
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: x_sb_cancel_scan
 *
 * Description: API to cancel a service/channel scan.
 *
 * Inputs:  h_builder   The builder handle.
 * Outputs: -
 *
 * Returns: SBR_OK              Success.
 *          SBR_INTERNAL_ERROR  Fail. Internal error happens.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_INV_COND        Fail. Invalid condition.
 *          SBR_INV_HANDLE      Fail. Invalid builder handle.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 x_sb_cancel_scan(
    HANDLE_T            h_builder)
{
    INT32               i4_ret;
    HANDLE_T            h_builder_base;
    SB_COND_T           e_builder_cond;
    SB_ENG_FCT_TBL_T    t_eng_fct_tbl;

    DBG_API((_SB_API"%s: h_builder = 0x%x\n\r", __FUNCTION__, h_builder));

    if (!x_sb_is_inited())
    {
        return SBR_NOT_INIT;
    }

    x_sb_base_ctrl_list_lock();

    i4_ret = x_sb_get_cond(h_builder, &e_builder_cond);
    if (i4_ret == SBR_OK)
    {
        if (e_builder_cond != SB_COND_BUILDING)
        {
            i4_ret = SBR_INV_COND; /* invalid condition for cancellation */
        }
        else /* check whether if h_builder is the scan starter. */
        {
            BOOL b_starter;

            i4_ret = sb_is_scan_starter(h_builder, &b_starter);
            if (i4_ret == SBR_OK && b_starter == FALSE)
            {
                i4_ret = SBR_NOT_STARTER;
            }
        }
    }

    if (i4_ret != SBR_OK)
    {
        goto SB_CS_EXIT;
    }

    i4_ret = sb_get_eng_fct_tbl(h_builder, &t_eng_fct_tbl);

    if (i4_ret != SBR_OK)
    {
        goto SB_CS_EXIT;
    }

    i4_ret = sb_get_builder_base_handle(h_builder,
                                        &h_builder_base);
    if (i4_ret != SBR_OK)
    {
        goto SB_CS_EXIT;
    }
        
    i4_ret = t_eng_fct_tbl.pf_cancel_scan(h_builder_base);

    if (i4_ret != SBR_OK)
    {
        goto SB_CS_EXIT;
    }
            
#if 0   /* this is replaced by _sb_scan_nfy(...) in sb_slctr.c */
    /* Justin:
    1. builder object should change the condition here, 
    since the scan had been cancelled.
    2. also, send condiction change notificaton
    */
    i4_ret = sb_set_cond(h_builder, SB_COND_AVAIL /* Justin: XXX */);
    if (i4_ret != SBR_OK)
    {
        /* DBG_ABORT( DBG_MOD_SVL_BLDR );*/
        /* program abort here. */
        i4_ret = SBR_INTERNAL_ERROR;
        goto SB_CS_EXIT;
    }

    /*i4_ret = */sb_call_nfy_fct(h_builder);
#endif

SB_CS_EXIT:

    x_sb_base_ctrl_list_unlock();

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_sb_save_lists
 *
 * Description: API to store the SVL and TSL.
 *
 * Inputs:  h_builder       The builder handle.
 *          ps_file_name    The file in which the lists are stored.
 *
 * Outputs: -
 *
 * Returns: SBR_OK              Success.
 *          SBR_ERR_SAVE_LIST   Fail. An error happened when saving SVL or TSL.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_INV_COND        Fail. Invalid condition.
 *          SBR_INV_HANDLE      Fail. Invalid builder handle.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 x_sb_save_lists(
    HANDLE_T            h_builder,
    const CHAR*         ps_file_name)
{
    DBG_API((_SB_API"%s: h_builder = 0x%x, ps_file_name = [%s]\n\r", 
             __FUNCTION__, h_builder, ps_file_name));

    return _sb_lists_op(h_builder,
                        ps_file_name,
                        SB_LIST_OP_SAVE);
}

/*-----------------------------------------------------------------------------
 * Name: x_sb_load_lists
 *
 * Description: API to load the SVL and TSL.
 *
 * Inputs:  h_builder       The builder handle.
 *          ps_file_name    The file from which the lists are loaded.
 *
 * Outputs: -
 *
 * Returns: SBR_OK              Success.
 *          SBR_ERR_LOAD_LIST   Fail. An error happened when loading SVL or TSL. 
 *          SBR_FILE_NOT_FOUND  Fail. The specified ps_file_name was not found.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_INV_COND        Fail. Invalid condition.
 *          SBR_INV_HANDLE      Fail. Invalid builder handle.
 *          SBR_LIST_NOT_FOUND  FAIL. The SVL doesn't exist in ps_file_name.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 x_sb_load_lists(
    HANDLE_T            h_builder,
    const CHAR*         ps_file_name)
{
    DBG_API((_SB_API"%s: h_builder = 0x%x, ps_file_name = [%s]\n\r", 
             __FUNCTION__, h_builder, ps_file_name));

    return _sb_lists_op(h_builder,
                        ps_file_name,
                        SB_LIST_OP_LOAD);
}

/*-----------------------------------------------------------------------------
 * Name: x_sb_clean_lists
 *
 * Description: API to clean the SVL and TSL from a file.
 *
 * Inputs:  h_builder       The builder handle.
 *          ps_file_name    The file in which the lists are stored.
 *
 * Outputs: -
 *
 * Returns: SBR_OK              Success.
 *          SBR_ERR_CLEAN_LIST  Fail. An error happened when cleaning SVL or TSL.
 *          SBR_FILE_NOT_FOUND  Fail. The specified ps_file_name was not found.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_INV_COND        Fail. Invalid builder condition.
 *          SBR_INV_HANDLE      Fail. Invalid builder handle.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 x_sb_clean_lists(
    HANDLE_T            h_builder,
    const CHAR*         ps_file_name)
{
    DBG_API((_SB_API"%s: h_builder = 0x%x, ps_file_name = [%s]\n\r", 
             __FUNCTION__, h_builder, ps_file_name));

    return _sb_lists_op(h_builder,
                        ps_file_name,
                        SB_LIST_OP_CLEAN);
}


/*-----------------------------------------------------------------------------
 * Name: x_sb_get
 *
 * Description: API to get requested type data from the builder engine.
 *
 * Inputs:  h_builder       The builder handle.
 *          e_key_type      The requested type to get.
 *          pv_key_info     The auxiliary info for the requested get type.
 *          pz_key_data_len Contains length of the buffer to which pv_key_data
 *                          points.
 *
 * Outputs: pz_key_data_len Contains the real length of retrieved key data if
 *                          the API successfully returns.
 *          pv_key_data     Contains the requested get data.
 *
 * Returns: SBR_OK              Success.
 *          SBR_ERR_CLEAN_LIST  Fail. An error happened when cleaning SVL or TSL.
 *          SBR_FILE_NOT_FOUND  Fail. The specified ps_file_name was not found.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_INV_COND        Fail. Invalid builder condition.
 *          SBR_INV_HANDLE      Fail. Invalid builder handle.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 x_sb_get(
    HANDLE_T            h_builder,
    SB_KEY_TYPE_T       e_key_type,
    VOID*               pv_key_info,
    SIZE_T*             pz_key_data_len,
    VOID*               pv_key_data)
{
    INT32               i4_ret;
    HANDLE_T            h_builder_base = NULL_HANDLE;
    SB_ENG_FCT_TBL_T    t_eng_fct_tbl;

    DBG_API((_SB_API"%s: h_builder = 0x%x, e_key_type = %u, pv_key_info = 0x%x, *pz_key_data_len = %u, pv_key_data = 0x%x\n\r",
             __FUNCTION__, h_builder, e_key_type, pv_key_info, *pz_key_data_len, pv_key_data));

    if (pz_key_data_len == NULL ||
        pv_key_data == NULL)
    {
        return SBR_INV_ARG;
    }

    if (!x_sb_is_inited())
    {
        return SBR_NOT_INIT;
    }

    x_sb_base_ctrl_list_lock();

    i4_ret = sb_get_eng_fct_tbl(h_builder, &t_eng_fct_tbl);

    if (i4_ret != SBR_OK)
    {
        goto SB_UNLOCK;
    }

    i4_ret = sb_get_builder_base_handle(h_builder, &h_builder_base);
    if (i4_ret != SBR_OK)
    {
        goto SB_UNLOCK;
    }

    if (t_eng_fct_tbl.pf_get)
    {
        i4_ret = t_eng_fct_tbl.pf_get(
                    h_builder_base,
                    e_key_type,
                    pv_key_info,
                    pz_key_data_len,
                    pv_key_data);
    }
    else
    {
        i4_ret = SBR_NOT_SUPPORT;
    }

SB_UNLOCK:

    x_sb_base_ctrl_list_unlock();

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: x_sb_set
 *
 * Description: API to set desired type data into the builder engine.
 *
 * Inputs:  h_builder       The builder handle.
 *          e_key_type      The desired type to set.
 *          pv_key_info     The auxiliary info for the desired set type.
 *          z_key_data_len  Contains length of the set data to which pv_key_data
 *                          points.
 *          pv_key_data     Contains the set data.
 *
 * Outputs: -
 *
 * Returns: SBR_OK              Success.
 *          SBR_ERR_CLEAN_LIST  Fail. An error happened when cleaning SVL or TSL.
 *          SBR_FILE_NOT_FOUND  Fail. The specified ps_file_name was not found.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_INV_COND        Fail. Invalid builder condition.
 *          SBR_INV_HANDLE      Fail. Invalid builder handle.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 x_sb_set(
    HANDLE_T            h_builder,
    SB_KEY_TYPE_T       e_key_type,
    VOID*               pv_key_info,
    SIZE_T              z_key_data_len,
    VOID*               pv_key_data)
{
    INT32               i4_ret;
    HANDLE_T            h_builder_base = NULL_HANDLE;
    SB_ENG_FCT_TBL_T    t_eng_fct_tbl;

    DBG_API((_SB_API"%s: h_builder = 0x%x, e_key_type = %u, pv_key_info = 0x%x, z_key_data_len = %u, pv_key_data = 0x%x\n\r",
             __FUNCTION__, h_builder, e_key_type, pv_key_info, z_key_data_len, pv_key_data));

    if (z_key_data_len != 0 && pv_key_data == NULL)
    {
        return SBR_INV_ARG;
    }

    if (!x_sb_is_inited())
    {
        return SBR_NOT_INIT;
    }

    x_sb_base_ctrl_list_lock();

    i4_ret = sb_get_eng_fct_tbl(h_builder, &t_eng_fct_tbl);

    if (i4_ret != SBR_OK)
    {
        goto SB_UNLOCK;
    }

    i4_ret = sb_get_builder_base_handle(h_builder, &h_builder_base);
    if (i4_ret != SBR_OK)
    {
        goto SB_UNLOCK;
    }

    if (t_eng_fct_tbl.pf_set)
    {
        i4_ret = t_eng_fct_tbl.pf_set(
                    h_builder_base,
                    e_key_type,
                    pv_key_info,
                    z_key_data_len,
                    pv_key_data);
    }
    else
    {
        i4_ret = SBR_NOT_SUPPORT;
    }

SB_UNLOCK:

    x_sb_base_ctrl_list_unlock();

    return i4_ret;
}

/* cliu 20090706, Sync cancel scan function to call API of sub egnine */
/*-----------------------------------------------------------------------------
 * Name: x_sb_cancel_scan_sync
 *
 * Description: API to cancel a service/channel scan in a thread context.
 *
 * Inputs:  h_builder   The builder handle.
 * Outputs: -
 *
 * Returns: SBR_OK              Success.
 *          SBR_INTERNAL_ERROR  Fail. Internal error happens.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_INV_COND        Fail. Invalid condition.
 *          SBR_INV_HANDLE      Fail. Invalid builder handle.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 x_sb_cancel_scan_sync(
    HANDLE_T            h_builder)
{
    INT32               i4_ret;
    HANDLE_T            h_builder_base;
    SB_COND_T           e_builder_cond;
    SB_ENG_FCT_TBL_T    t_eng_fct_tbl;

    DBG_API((_SB_API"%s: h_builder = 0x%x\n\r", __FUNCTION__, h_builder));

    if (!x_sb_is_inited())
    {
        return SBR_NOT_INIT;
    }

    x_sb_base_ctrl_list_lock();

    i4_ret = x_sb_get_cond(h_builder, &e_builder_cond);
    if (i4_ret == SBR_OK)
    {
        if (e_builder_cond != SB_COND_BUILDING)
        {
            i4_ret = SBR_INV_COND; /* invalid condition for cancellation */
        }
        else /* check whether if h_builder is the scan starter. */
        {
            BOOL b_starter;

            i4_ret = sb_is_scan_starter(h_builder, &b_starter);
            if (i4_ret == SBR_OK && b_starter == FALSE)
            {
                i4_ret = SBR_NOT_STARTER;
            }
        }
    }

    if (i4_ret != SBR_OK)
    {
        goto SB_CS_EXIT;
    }

    i4_ret = sb_get_eng_fct_tbl(h_builder, &t_eng_fct_tbl);

    if (i4_ret != SBR_OK)
    {
        goto SB_CS_EXIT;
    }

    i4_ret = sb_get_builder_base_handle(h_builder,
                                        &h_builder_base);
    if (i4_ret != SBR_OK)
    {
        goto SB_CS_EXIT;
    }

    /* unlock function must be called inside pf_cancel_scan_sync. 
       otherwise the deak lock will happen. because SB engine is locked now,
       and AP will be locked later. Just unlock AP when SB receive a message,
       but now SB is locked, so the system will dead lock.*/    
    i4_ret = t_eng_fct_tbl.pf_cancel_scan_sync(h_builder_base);

    if (i4_ret != SBR_OK)
    {
        goto SB_CS_EXIT;
    }            

SB_CS_EXIT:

    x_sb_base_ctrl_list_unlock();

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_sb_reg_scan_map
 *
 * Description: API to cancel a service/channel scan in a thread context.
 *
 * Inputs:  h_builder   The builder handle.
 * Outputs: -
 *
 * Returns: SBR_OK              Success.
 *          SBR_INTERNAL_ERROR  Fail. Internal error happens.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_INV_COND        Fail. Invalid condition.
 *          SBR_INV_HANDLE      Fail. Invalid builder handle.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 x_sb_reg_scan_map(
    const CHAR*          ps_name,
    VOID*                pv_nfy_fct,
    VOID*                pt_nfy_tag)
{
    INT32               i4_ret = SBR_OK;
    SB_ENG_CTRL_T*      pt_eng_ctrl = NULL;

    DBG_API((_SB_API"%s: eng name = %s\n\r", __FUNCTION__, ps_name));

    if (!x_sb_is_inited())
    {
        return SBR_NOT_INIT;
    }

    x_sb_base_ctrl_list_lock();

    pt_eng_ctrl = sb_find_eng_by_name(ps_name);

    if (pt_eng_ctrl == NULL)
    {
        goto SB_EXIT;
    }
    
    i4_ret = pt_eng_ctrl->t_sb_fct_tbl.pf_reg_scan_map(pv_nfy_fct, pt_nfy_tag);

    if (i4_ret != SBR_OK)
    {
        goto SB_EXIT;
    }            

SB_EXIT:

    x_sb_base_ctrl_list_unlock();

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: x_sb_get_cond
 *
 * Description: get the current builder condition.
 *
 * Inputs:  h_builder       Contains the builder handle.
 *
 * Outputs: pe_builder_cond Contains the current builder condition.
 *
 * Returns: SBR_OK              Success.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_INV_HANDLE      Fail. Invalid builder handle.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 x_sb_get_cond(
    HANDLE_T            h_builder,
    SB_COND_T*          pe_builder_cond)
{
    INT32 i4_ret;
    SB_CTRL_T* pt_sb_ctrl;

    DBG_API((_SB_API"%s: h_builder = 0x%x, pe_builder_cond = 0x%x\n\r", 
             __FUNCTION__, h_builder, pe_builder_cond));

    if (pe_builder_cond == NULL)
    {
        return SBR_INV_ARG;
    }

    i4_ret = sb_get_sb_ctrl(h_builder, &pt_sb_ctrl);
    if (i4_ret == SBR_OK)
    {
        if (pt_sb_ctrl == NULL ||
            pt_sb_ctrl->pt_base_ctrl == NULL)
        {
            /*DBG_ABORT( DBG_MOD_SVL_BLDR );
             program abort here. */

            return SBR_INV_HANDLE;
        }

        (*pe_builder_cond) = pt_sb_ctrl->pt_base_ctrl->e_cond;
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: x_sb_is_inited
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
BOOL x_sb_is_inited(VOID)
{
    DBG_API((_SB_API"%s.\n\r", __FUNCTION__));
    return sb_is_inited();
}


/*-----------------------------------------------------------------------------
 * Name: x_sb_get_dbg_level
 *
 * Description: This API returns the current setting of debug level.
 *
 * Inputs: -  
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/ 
UINT16 x_sb_get_dbg_level(VOID)
{
    if (sb_is_inited())
    {    
        return ui2_sb_dbg_level;    
    }      
    else
    {
        return 0;
    }    
}


/*-----------------------------------------------------------------------------
 * Name: x_sb_set_dbg_level
 *
 * Description: This API changes the setting of debug level to new one.
 *
 * Inputs: ui2_level            The new setting of debug level.  
 *
 * Outputs: -
 *
 * Returns: TRUE            Routine successful.
 *          FALSE           Routine failed.
 ----------------------------------------------------------------------------*/ 
BOOL x_sb_set_dbg_level(UINT16 ui2_level)
{
    if (sb_is_inited())
    {        
        ui2_sb_dbg_level = ui2_level;    
        return TRUE;
    }    
    else
    {
        return FALSE;
    }
}

