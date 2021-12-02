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
 * $RCSfile: _sb_handle.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/6 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: 1da8eea70ca51ce0615e5c941c40f22b $
 *
 * Description:
 *         This file contains SVL Builder Handle-related API's.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "dbg/dbg.h"

#include "svl_bldr/_sb_handle.h"
#include "svl_bldr/sb_slctr.h"
#include "svl_bldr/x_sb_eng.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static BOOL                 b_sb_handle_inited           = FALSE;

/* the base ctrl list */
static SB_BASE_CTRL_LIST_T  t_sb_base_ctrl_list;
static HANDLE_T             h_sb_base_ctrl_list_lock  = NULL_HANDLE;

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    static functions implementation
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    private functions implementation
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: sb_handle_init
 *
 * Description: API to initialize the handle semaphore
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: SBR_OK               The routine was successful.
 *          SBR_ALREADY_INIT     Already inited.
 *          SBR_INTERNAL_ERROR   Internal error happened.
 *
 ----------------------------------------------------------------------------*/
INT32 sb_handle_init(VOID)
{
    INT32 i4_ret;

    if (b_sb_handle_inited)
    {
        return SBR_ALREADY_INIT;
    }

    DLIST_INIT(&t_sb_base_ctrl_list);

    i4_ret = x_sema_create( &h_sb_base_ctrl_list_lock,
                            X_SEMA_TYPE_MUTEX,
                            X_SEMA_STATE_UNLOCK );
    if ( i4_ret != OSR_OK || h_sb_base_ctrl_list_lock == NULL_HANDLE )
    {
        return SBR_INTERNAL_ERROR;
    }

    b_sb_handle_inited = TRUE;

    return SBR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: sb_handle_exit
 *
 * Description: API to free the handle semaphore.
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sb_handle_exit(VOID)
{
    if (!b_sb_handle_inited)
    {
        return;
    }

    x_sema_delete(h_sb_base_ctrl_list_lock);
    h_sb_base_ctrl_list_lock = NULL_HANDLE;

    b_sb_handle_inited = FALSE;
}


/*-----------------------------------------------------------------------------
 * Name: sb_base_ctrl_list_lock
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
VOID sb_base_ctrl_list_lock(VOID)
{
    INT32 i4_ret;

    i4_ret = x_sema_lock( h_sb_base_ctrl_list_lock,
                          X_SEMA_OPTION_WAIT );
    if ( i4_ret != OSR_OK )
    {
        if (b_sb_handle_inited)
        {   /* should not fail when SB handle inited. */
            DBG_ABORT( DBG_MOD_SVL_BLDR );
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: sb_base_ctrl_list_unlock
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
VOID sb_base_ctrl_list_unlock(VOID)
{
    INT32 i4_ret;

    i4_ret = x_sema_unlock( h_sb_base_ctrl_list_lock );
    if ( i4_ret != OSR_OK )
    {
        if (b_sb_handle_inited)
        {   /* should not fail when SB handle inited. */
            DBG_ABORT( DBG_MOD_SVL_BLDR );
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: sb_link_base_ctrl
 *
 * Description: API to link the specified pt_base_ctrl to the 
 *              SB_BASE_CTRL_T list.
 *
 * Inputs:  pt_base_ctrl    pointer to the SB_BASE_CTRL_T type node to be 
 *                          linked.
 * Outputs: -
 *
 * Returns: SBR_OK          Success
 *          SBR_INV_ARG     Invalid argument
 *          SBR_NOT_INIT    SB handle is not inited
 *
 ----------------------------------------------------------------------------*/
INT32 sb_link_base_ctrl(
    SB_BASE_CTRL_T*     pt_base_ctrl)
{
    if (pt_base_ctrl == NULL)
    {
        return SBR_INV_ARG;
    }

    if (!b_sb_handle_inited)
    {
        return SBR_NOT_INIT;
    }

    DLIST_INSERT_TAIL(pt_base_ctrl, &t_sb_base_ctrl_list, t_link);

    return SBR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: sb_delink_base_ctrl
 *
 * Description: API to de-link the specified pt_base_ctrl from the 
 *              SB_BASE_CTRL_T list.
 *
 * Inputs:  pt_base_ctrl    pointer to the SB_BASE_CTRL_T type node to be 
 *                          de-linked.
 * Outputs: -
 *
 * Returns: SBR_OK          Success
 *          SBR_INV_ARG     Invalid argument
 *          SBR_NOT_INIT    SB handle is not inited
 *
 ----------------------------------------------------------------------------*/
INT32 sb_delink_base_ctrl(
    SB_BASE_CTRL_T*     pt_base_ctrl)
{
    if (pt_base_ctrl == NULL)
    {
        return SBR_INV_ARG;
    }

    if (!b_sb_handle_inited)
    {
        return SBR_NOT_INIT;
    }

    DLIST_REMOVE(pt_base_ctrl, &t_sb_base_ctrl_list, t_link);

    return SBR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: sb_handle_is_inited
 *
 * Description: API to return the initialziation status of SB handle.
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: TRUE        The SB handle had been initialized.
 *          FALSE       The SB handle is not initialized.
 *
 ----------------------------------------------------------------------------*/
BOOL sb_handle_is_inited(VOID)
{
    return b_sb_handle_inited;
}


/*-----------------------------------------------------------------------------
 * Name: sb_get_sb_ctrl
 *
 * Description: get the SB_CTRL_T of a builder handle.
 *
 * Inputs:  h_builder   Contains the builder handle.
 *
 * Outputs: ppt_sb_ctrl Contains the builder handle's pv_obj.
 *
 * Returns: SBR_OK              Success.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_INV_HANDLE      Fail. Invalid builder handle.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 sb_get_sb_ctrl(
    HANDLE_T            h_builder,
    SB_CTRL_T**         ppt_sb_ctrl)
{
    INT32 i4_ret;

    if (ppt_sb_ctrl == NULL)
    {
        return SBR_INV_ARG;
    }

    if (!sb_is_inited())
    {
        return SBR_NOT_INIT;
    }

    /* handle_get_obj will valid it in the same way
    if (x_handle_valid(h_builder) == FALSE)
    {
        return SBR_INV_HANDLE;
    }
    */

    i4_ret = handle_get_obj(h_builder, (VOID**) ppt_sb_ctrl);
    if (i4_ret != HR_OK)
    {
        switch (i4_ret)
        {
        case HR_INV_ARG:
            i4_ret = SBR_INV_ARG;
            break;
        case HR_INV_HANDLE:
            i4_ret = SBR_INV_HANDLE;
            break;
        default:
            DBG_ABORT( DBG_MOD_SVL_BLDR );
            /* program abort here. */
        }
    }
    else
    {
        i4_ret = SBR_OK;
    }
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: sb_get_base_ctrl_by_base_handle
 *
 * Description: get the SB_BASE_CTRL_T of a builder base object.
 *
 * Inputs:  h_builder_base  Contains the handle of builder base object.
 *
 * Outputs: ppt_base_ctrl   Contains the builder base object.
 *
 * Returns: SBR_OK              Success.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_INV_HANDLE      Fail. Invalid builder base object handle.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 sb_get_base_ctrl_by_base_handle(
    HANDLE_T            h_builder_base,
    SB_BASE_CTRL_T**    ppt_base_ctrl)
{
    INT32 i4_ret;
	HANDLE_TYPE_T     e_handle_type;

    if (ppt_base_ctrl == NULL)
    {
        return SBR_INV_ARG;
    }

    if (!sb_is_inited())
    {
        return SBR_NOT_INIT;
    }

    i4_ret = handle_get_type_obj(h_builder_base,
		        &e_handle_type,
		        (VOID**) ppt_base_ctrl);	
    if (i4_ret != HR_OK)
    {
        switch (i4_ret)
        {
        case HR_INV_ARG:
            i4_ret = SBR_INV_ARG;
            break;
        case HR_INV_HANDLE:
            i4_ret = SBR_INV_HANDLE;
            break;
        default:
            DBG_ABORT( DBG_MOD_SVL_BLDR );
            /* program abort here. */
        }
    }
	else if (SBT_BASE_CTRL != e_handle_type)
    {
        i4_ret = SBR_INV_HANDLE;
	}	
    else
    {
        i4_ret = SBR_OK;
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: sb_get_builder_base_handle
 *
 * Description: function to get the handle of builder base object.
 *
 * Inputs:  h_builder       Contains the builder handle.
 *
 * Outputs: ph_builder_base Contains the handle of builder base object.
 *
 * Returns: SBR_OK              Success.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_INV_HANDLE      Fail. Invalid builder handle.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 sb_get_builder_base_handle(
    HANDLE_T            h_builder,
    HANDLE_T*           ph_builder_base)
{
    INT32 i4_ret;
    SB_CTRL_T* pt_sb_ctrl;

    if (ph_builder_base == NULL)
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

        (*ph_builder_base) = pt_sb_ctrl->pt_base_ctrl->h_base_ctrl;
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: sb_is_scan_starter
 *
 * Description: check whether if the input handle is the one who starts 
 *              the scan process.
 *
 * Inputs:  h_builder   Contains the builder handle.
 *
 * Outputs: pb_starter  A boolean indicating h_builder is starter of not.
 *
 * Returns: SBR_OK              Success.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_INV_HANDLE      Fail. Invalid builder handle.
 *
 ----------------------------------------------------------------------------*/
INT32 sb_is_scan_starter(
    HANDLE_T            h_builder,
    BOOL*               pb_starter)
{
    INT32               i4_ret;
    SB_BASE_CTRL_T*     pt_sb_base_ctrl = NULL;
    SB_CTRL_T*          pt_sb_ctrl = NULL;

    if (pb_starter == NULL)
    {
        return SBR_INV_ARG;
    }

    i4_ret = sb_get_sb_ctrl(h_builder, &pt_sb_ctrl);
    if (i4_ret != SBR_OK)
    {
        goto SB_ICS_EXIT;
    }

    if (pt_sb_ctrl == NULL ||
        pt_sb_ctrl->pt_base_ctrl == NULL)
    {
        /*DBG_ABORT( DBG_MOD_SVL_BLDR );
         program abort here. */

        i4_ret = SBR_INV_HANDLE;
        goto SB_ICS_EXIT;
    }

    pt_sb_base_ctrl = pt_sb_ctrl->pt_base_ctrl;

    if (/*pt_sb_base_ctrl->e_cond == SB_COND_BUILDING &&*/
        pt_sb_base_ctrl->h_scan_starter == h_builder)
    {
        *pb_starter = TRUE;
    }
    else
    {
        *pb_starter = FALSE;
    }

SB_ICS_EXIT:

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: sb_find_base_ctrl
 *
 * Description: search a SB_BASE_CTRL_T node which matches the engine and the 
 *              engine-specific pv_open_data in the t_sb_base_ctrl_list.
 *
 * Inputs:  pt_eng          The first search key.
 *          pv_open_data    The second search key, which is engine-specific.
 *
 * Outputs: ppt_base_ctrl   Contains the searched SB_BASE_CTRL_T pointer or 
 *                          NULL.
 *
 * Returns: SBR_OK          Success.
 *          SBR_INV_ARG     Fail. Invalid arguments.
 *
 * if return SBR_OK
 *      if ppt_base_ctrl != NULL => found
 *      else => not found
 * else error
 *
 ----------------------------------------------------------------------------*/
INT32 sb_find_base_ctrl(
    const SB_ENG_CTRL_T*    pt_eng,
    const VOID*             pv_open_data,
    SB_BASE_CTRL_T**        ppt_base_ctrl)
{
    INT32               i4_ret;
    SB_BASE_CTRL_T*     pt_base_ctrl;

    if (pt_eng == NULL ||
        ppt_base_ctrl == NULL)
    {
        return SBR_INV_ARG;
    }

    /* Justin: lock base ctrl list */
    DLIST_FOR_EACH(pt_base_ctrl, &t_sb_base_ctrl_list, t_link)
    {
        if (pt_eng == pt_base_ctrl->pt_eng)
        {
            i4_ret = pt_eng->t_sb_fct_tbl.pf_match_builder_data(
                                            pv_open_data,
                                            pt_base_ctrl->pv_data);
            if (i4_ret == SBR_OK) /* match */
            {
                break;
            }
            else if (i4_ret != SBR_NOT_MATCH) /* error */
            {
                return i4_ret;
            }
        }
    }

    (*ppt_base_ctrl) = pt_base_ctrl;

    /* Justin: unlock base ctrl list */

    return SBR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: sb_set_cond
 *
 * Description: set the current builder condition
 *
 * Inputs:  h_builder       Contains the builder handle.
 *          e_builder_cond  Contains the condition to be set.
 *
 * Outputs: -
 *
 * Returns: SBR_OK              Success.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_INV_HANDLE      Fail. Invalid builder handle.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 sb_set_cond(
    HANDLE_T            h_builder,
    SB_COND_T           e_builder_cond)
{
    INT32 i4_ret;
    SB_CTRL_T* pt_sb_ctrl;

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

        pt_sb_ctrl->pt_base_ctrl->e_cond = e_builder_cond;
        pt_sb_ctrl->pt_base_ctrl->h_scan_starter = 
                /*e_builder_cond == SB_COND_BUILDING?*/
                    h_builder/* : NULL_HANDLE*/;

         /*AUTOSEARCH kenny set autosearch to true*/
        if(e_builder_cond == SB_COND_BUILDING){
            sb_set_auto_search_type_by_handle(h_builder,TRUE);
        }
         
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: sb_free_handle_cb
 *
 * Description: A "template" API designed for handle_alloc.
 *              SB component does the free job by itself, 
 *              therefre sb_free_handle_cb simply returns TRUE.
 *
 * Inputs:  refer to the input arguments of (*handle_free_fct) in 
 *          the Handles component.
 * Outputs: -
 *
 * Returns: always TRUE
 *
 ----------------------------------------------------------------------------*/
BOOL sb_free_handle_cb(HANDLE_T       h_obj,
                       HANDLE_TYPE_T  e_type,
                       VOID*          pv_obj,
                       VOID*          pv_tag,
                       BOOL           b_req_handle)
{
    /* Justin: */

    return (TRUE);
}


/*-----------------------------------------------------------------------------
 * Name: sb_handle_parse_cond_change
 *
 * Description: This API is called by the handle_parse function to 
 *              notify the client that the condition of the builder
 *              object was changed.
 *
 * Inputs:  ui2_count      Contains the current count, starting at '0'.
 *          ui2_max_count  Contains the maximum count value.
 *          h_comp         Contains the handle referencing an client object.
 *                         
 *          e_type         Contains the type.
 *          pv_obj         Contains the object pointer.
 *          pv_tag         Contains the tag value.
 *          pv_parse_data  Reason for calling notification function.
 *                        
 *
 * Outputs: -
 *
 * Returns: HP_NEXT or HP_BREAK.
 ----------------------------------------------------------------------------*/
HP_TYPE_T sb_handle_parse_cond_change (
            UINT16         ui2_count,
            UINT16         ui2_max_count,
            HANDLE_T       h_elem,
            HANDLE_TYPE_T  e_type,
            VOID*          pv_obj,
            VOID*          pv_tag,
            VOID*          pv_parse_data)
{
    HP_TYPE_T           e_return;
    SB_CTRL_T*          pt_sb_ctrl;
    
    e_return = HP_NEXT;

    pt_sb_ctrl = (SB_CTRL_T*) pv_obj;
    if (pt_sb_ctrl != NULL)
    {
        if (pt_sb_ctrl->pf_nfy != NULL)
        {
            if (pt_sb_ctrl->pt_base_ctrl != NULL)
            {
                pt_sb_ctrl->pf_nfy(h_elem, 
                                   pt_sb_ctrl->pt_base_ctrl->e_cond, 
                                   pt_sb_ctrl->pv_nfy_tag);
            }
        }
    }

    return (e_return);
}


/*-----------------------------------------------------------------------------
 * Name: sb_handle_parse_update
 *
 * Description: This API is called by the handle_parse function to 
 *              notify the client that the SVL was updated.
 *
 * Inputs:  ui2_count      Contains the current count, starting at '0'.
 *          ui2_max_count  Contains the maximum count value.
 *          h_comp         Contains the handle referencing an client object.
 *                         
 *          e_type         Contains the type.
 *          pv_obj         Contains the object pointer.
 *          pv_tag         Contains the tag value.
 *          pv_parse_data  Reason for calling notification function.
 *                        
 *
 * Outputs: -
 *
 * Returns: HP_NEXT or HP_BREAK.
 ----------------------------------------------------------------------------*/
HP_TYPE_T sb_handle_parse_update (
            UINT16         ui2_count,
            UINT16         ui2_max_count,
            HANDLE_T       h_elem,
            HANDLE_TYPE_T  e_type,
            VOID*          pv_obj,
            VOID*          pv_tag,
            VOID*          pv_parse_data)
{
    HP_TYPE_T           e_return;
    SB_CTRL_T*          pt_sb_ctrl;
    
    e_return = HP_NEXT;

    pt_sb_ctrl = (SB_CTRL_T*) pv_obj;
    if (pt_sb_ctrl != NULL)
    {
        if (pt_sb_ctrl->pf_nfy != NULL)
        {
            if (pt_sb_ctrl->pt_base_ctrl != NULL)
            {
                pt_sb_ctrl->pf_nfy(h_elem, 
                                   SB_COND_UPDATE, 
                                   pt_sb_ctrl->pv_nfy_tag);
            }
        }
    }

    return (e_return);
}


/*-----------------------------------------------------------------------------
 * Name: sb_call_nfy_fct
 *
 * Description: call the notification function of the handle
 *
 * Inputs:  h_builder   Contains the builder handle.
 *
 * Outputs: -
 *
 * Returns: SBR_OK              Success.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_INV_HANDLE      Fail. Invalid builder handle.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 sb_call_nfy_fct(HANDLE_T h_builder)
{
    INT32               i4_ret;
    SB_CTRL_T*          pt_sb_ctrl;

    i4_ret = sb_get_sb_ctrl(h_builder, &pt_sb_ctrl);
    if (i4_ret != SBR_OK)
    {
        return i4_ret;
    }

    if (pt_sb_ctrl == NULL ||
        pt_sb_ctrl->pt_base_ctrl == NULL)
    {
        return SBR_INV_HANDLE;
    }

    /*
        Justin: should traverse all handles which attach to 
        the same SB_BASE_CTRL_T.
    */

    i4_ret = handle_parse(&(pt_sb_ctrl->pt_base_ctrl->t_handle_link), 
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

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: sb_get_scan_starter_handle
 *
 * Description: get the builder specific data which the handle uses.
 *
 * Inputs:  h_builder_base  Contains the handle of the builder base object.
 *
 * Outputs: ph_builder      Contains the handle of scan stater.
 *
 * Returns: SBR_OK              Success.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_INV_HANDLE      Fail. Invalid builder handle.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 sb_get_scan_starter_handle(
            HANDLE_T            h_builder_base,
            HANDLE_T*           ph_builder)
{
    INT32 i4_ret;
    SB_BASE_CTRL_T* pt_base_ctrl;

    if (ph_builder == NULL)
    {
        return SBR_INV_ARG;
    }

    if (!sb_is_inited())
    {
        return SBR_NOT_INIT;
    }

    i4_ret = handle_get_obj(h_builder_base, (VOID**) &pt_base_ctrl);
    if (i4_ret != HR_OK)
    {
        switch (i4_ret)
        {
        case HR_INV_ARG:
            i4_ret = SBR_INV_ARG;
            break;
        case HR_INV_HANDLE:
            i4_ret = SBR_INV_HANDLE;
            break;
        default:
            /*DBG_ABORT( DBG_MOD_SVL_BLDR );*/
            /* program abort here. */
            i4_ret = SBR_INTERNAL_ERROR;
            break;
        }
    }
    else
    {
        /*if (pt_base_ctrl->e_cond == SB_COND_BUILDING)
        {*/
            (*ph_builder) = pt_base_ctrl->h_scan_starter;
        /*}
        else
        {
            (*ph_builder) = NULL_HANDLE;
        }*/

        i4_ret = SBR_OK;
    }

    return i4_ret;
}


