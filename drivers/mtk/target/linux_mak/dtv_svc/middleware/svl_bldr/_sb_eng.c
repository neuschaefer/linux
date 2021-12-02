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
 * $RCSfile: _sb_eng.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/3 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: 1d7d0101afe9ccda2e76d0b6a50380fc $
 *
 * Description:
 *         This file contains SVL Builder Engine-related API's.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "dbg/dbg.h"

#include "svl_bldr/_sb_eng.h"
#include "svl_bldr/_sb_handle.h"
#include "svl_bldr/sb_slctr.h"
#include "res_mngr/x_rm.h"
#include "res_mngr/drv/u_drv_cust.h"
#include "res_mngr/u_rm_dev_types.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static BOOL                 b_sb_eng_inited      = FALSE;

/* Customized engine list */
static SB_ENG_LIST_T        t_sb_eng_list;
static HANDLE_T             h_sb_eng_list_lock   = NULL_HANDLE;

/*
    This flag is used to judge how many engine is in the BUILDING state.
    Each bit represent one engine state.
    
*/
static UINT32 ui4_autosearch_flag = 0;


/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
static VOID _sb_eng_free_list(VOID);


/*-----------------------------------------------------------------------------
                    static functions implementation
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: _sb_eng_free_list
 *
 * Description: API to free all registered engines from the engine list.
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID _sb_eng_free_list(VOID)
{
    SB_ENG_CTRL_T*  pt_eng;

    sb_eng_list_lock();

    while (!DLIST_IS_EMPTY(&t_sb_eng_list))
    {
        pt_eng = DLIST_HEAD(&t_sb_eng_list);
        DLIST_REMOVE(pt_eng, &t_sb_eng_list, t_link);
        x_sb_mem_free((VOID *) pt_eng);
    }

    sb_eng_list_unlock();
}


/*-----------------------------------------------------------------------------
                    private functions implementation
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: sb_eng_list_init
 *
 * Description: API to initialize the engine list
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: SBR_OK               The routine was successful.
 *          SBR_ALREADY_INIT     The engine list was already inited.
 *          SBR_INTERNAL_ERROR   Internal error happened.
 *
 ----------------------------------------------------------------------------*/
INT32 sb_eng_list_init(VOID)
{
    INT32 i4_ret;

    if (b_sb_eng_inited)
    {
        return SBR_ALREADY_INIT;
    }

    DLIST_INIT(&t_sb_eng_list);

    i4_ret = x_sema_create( &h_sb_eng_list_lock,
                            X_SEMA_TYPE_MUTEX,
                            X_SEMA_STATE_UNLOCK );
    if ( i4_ret != OSR_OK || h_sb_eng_list_lock == NULL_HANDLE )
    {
        return SBR_INTERNAL_ERROR;
    }

    b_sb_eng_inited = TRUE;

    return SBR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: sb_eng_list_exit
 *
 * Description: API to clean and free the engine list and semaphore.
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sb_eng_list_exit(VOID)
{
    if (!b_sb_eng_inited)
    {
        return;
    }

    _sb_eng_free_list();
    x_sema_delete(h_sb_eng_list_lock);
    h_sb_eng_list_lock = NULL_HANDLE;

    b_sb_eng_inited = FALSE;
}


/*-----------------------------------------------------------------------------
 * Name: sb_eng_list_lock
 *
 * Description: API to lock the engine list
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sb_eng_list_lock(VOID)
{
    INT32 i4_ret;

    i4_ret = x_sema_lock( h_sb_eng_list_lock,
                       X_SEMA_OPTION_WAIT );
    if ( i4_ret != OSR_OK )
    {
        if (b_sb_eng_inited)
        {   /* should not fail when engine list inited */
            DBG_ABORT( DBG_MOD_SVL_BLDR );
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: sb_eng_list_unlock
 *
 * Description: API to unlock the engine list.
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sb_eng_list_unlock(VOID)
{
    INT32 i4_ret;

    i4_ret = x_sema_unlock( h_sb_eng_list_lock );
    if ( i4_ret != OSR_OK )
    {
        if (b_sb_eng_inited)
        {   /* should not fail when engine list inited */
            DBG_ABORT( DBG_MOD_SVL_BLDR );
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: sb_eng_list_add
 *
 * Description: This API is called to add a new engine node to the engine
 *              list.
 *
 * Inputs:  pt_eng  Contains engine node to be added.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID sb_eng_list_add (SB_ENG_CTRL_T* pt_eng)
{
    if (!b_sb_eng_inited)
    {
        return;
    }

    DLIST_INSERT_TAIL(pt_eng, &t_sb_eng_list, t_link);
}


/*-----------------------------------------------------------------------------
 * Name: sb_find_eng_by_handle
 *
 * Description: find a builder engine which the handle uses.
 *
 * Inputs:  h_builder   Contains the builder handle.
 *
 * Outputs: ppt_eng     Contains the engine pointer.
 *
 * Returns: SBR_OK              Success.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_INV_HANDLE      Fail. Invalid builder handle.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 sb_find_eng_by_handle(
    HANDLE_T            h_builder,
    SB_ENG_CTRL_T**     ppt_eng)
{
    INT32 i4_ret;
    SB_CTRL_T* pt_sb_ctrl;

    if (ppt_eng == NULL)
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

        (*ppt_eng) = pt_sb_ctrl->pt_base_ctrl->pt_eng;
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: sb_find_eng_by_name
 *
 * Description: This API searches for an engine control object with a given
 *              name.
 *
 * Inputs:  ps_name  References a zero terminated engine name.
 *
 * Outputs: -
 *
 * Returns: A reference to the found engine control object or NULL.
 ----------------------------------------------------------------------------*/
SB_ENG_CTRL_T* sb_find_eng_by_name (const CHAR*  ps_name)
{
    SB_ENG_CTRL_T* pt_eng;

    pt_eng = NULL;

    if (ps_name != NULL)
    {
        pt_eng = DLIST_HEAD(&t_sb_eng_list);

        while ((pt_eng != NULL) && 
               (x_strcmp (pt_eng->s_name, ps_name) != 0))
        {
            pt_eng = DLIST_NEXT(pt_eng, t_link);
        }
    }

    return (pt_eng);
}


/*-----------------------------------------------------------------------------
 * Name: _sb_set_auto_search_type_by_name
 *
 * Description: get the flag position of a builder engine and set the bit of ui4_autosearch_flag
 *
 * Inputs:  ps_name  References a zero terminated engine name.
 *             b_auto_search  the boolean of auto search type.
 * Outputs: 
 *
 * Returns: SBR_OK              Success.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_INV_ARG
 *          
 *
 ----------------------------------------------------------------------------*/

INT32 _sb_set_auto_search_type_by_name (
    const CHAR*  ps_name,
    BOOL          b_auto_search)
{
    SB_ENG_CTRL_T* pt_eng;
    UINT8 ui1_pos = 0;
    UINT32 ui4_base_flag = 0;

    pt_eng = NULL;

    if (ps_name != NULL)
    {
        pt_eng = DLIST_HEAD(&t_sb_eng_list);
        ui1_pos++;
        while ((pt_eng != NULL) && 
               (x_strcmp (pt_eng->s_name, ps_name) != 0))
        {
            ui1_pos++;
            pt_eng = DLIST_NEXT(pt_eng, t_link);
        }
    }


    if(ui1_pos > 0)
    {
        /*todo: call driver set to do auto search type*/
        DRV_CUST_OPERATION_INFO_T t_op_info;
        SIZE_T z_size = sizeof(DRV_CUST_OPERATION_INFO_T);
        INT32 i4_ret = 0;
        AUTO_SEARCH_MODE_T e_autoserch = AUTOSEARCH_CHANGE_CHANNEL;
        
        ui4_base_flag = MAKE_BIT_MASK_32(ui1_pos);
        ui4_autosearch_flag = (ui4_autosearch_flag &(~ui4_base_flag)) |(b_auto_search?ui4_base_flag:0);

        /*todo: call driver 
        vSetAutoSearchType(ui4_autosearch_flag>0?TRUE:FALSE);
        */
        e_autoserch = ui4_autosearch_flag>0?AUTOSEARCH_SCAN:AUTOSEARCH_CHANGE_CHANNEL;

        t_op_info.e_op_type = DRV_CUSTOM_OPERATION_TYPE_MISC_SET;
        t_op_info.u.t_misc_set_info.e_misc_type = DRV_CUSTOM_MISC_TYPE_AUTO_SEARCH;
        t_op_info.u.t_misc_set_info.pv_set_info = &e_autoserch;
	    t_op_info.u.t_misc_set_info.z_size = sizeof(e_autoserch);
	
        
        i4_ret = x_rm_set_comp(
                DRVT_CUST_DRV,
                DRV_CUST_COMP_ID,
                FALSE,
                ANY_PORT_NUM,
                0,
                &t_op_info,
                z_size
                );
        
        if (i4_ret < RMR_OK)
        {
            return SBR_INTERNAL_ERROR;
        }

        /*x_dbg_stmt(" autosearch flag:%x\n",ui4_autosearch_flag);*/
        
        return SBR_OK;
    }        
    return SBR_INV_ARG;
}

/*-----------------------------------------------------------------------------
 * Name: sb_set_auto_search_type_by_handle
 *
 * Description: set auto search type by handle
 *
 * Inputs:  h_builder   Contains the builder handle.
 *             b_auto_search     the boolean of auto search type.
 *
 *
 * Returns: SBR_OK              Success.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_INV_HANDLE      Fail. Invalid builder handle.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 sb_set_auto_search_type_by_handle(
    HANDLE_T            h_builder,
    BOOL               b_auto_search)
{
    SB_ENG_CTRL_T* pt_eng = NULL;
    INT32 i4_ret = 0;

    i4_ret = sb_find_eng_by_handle(h_builder,&pt_eng);

    if(i4_ret != SBR_OK)
    {
        return i4_ret;
    }

    return _sb_set_auto_search_type_by_name(pt_eng->s_name,b_auto_search);
}
/*-----------------------------------------------------------------------------
 * Name: sb_set_auto_search_type_by_base_ctrl
 *
 * Description: set auto search type by base ctrl
 *
 * Inputs:  h_builder   Contains the builder handle.
 *             b_auto_search     the boolean of auto search type.
 *
 *
 * Returns: SBR_OK              Success.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_INV_HANDLE      Fail. Invalid builder handle.
 *
 ----------------------------------------------------------------------------*/
INT32 sb_set_auto_search_type_by_base_ctrl(
     SB_BASE_CTRL_T*            pt_base_ctrl,
        BOOL               b_auto_search
)
{

    if((pt_base_ctrl == NULL)||(pt_base_ctrl->pt_eng == NULL))
    {
        return SBR_INV_HANDLE;
    }

    return _sb_set_auto_search_type_by_name(pt_base_ctrl->pt_eng->s_name,b_auto_search);
}

/*-----------------------------------------------------------------------------
 * Name: sb_get_eng_fct_tbl
 *
 * Description: get the function table in a builder engine which the handle uses.
 *
 * Inputs:  h_builder       Contains the builder handle.
 *
 * Outputs: pt_eng_fct_tbl  Contains the function table pointer.
 *
 * Returns: SBR_OK              Success.
 *          SBR_INV_ARG         Fail. Invalid arguments.
 *          SBR_INV_HANDLE      Fail. Invalid builder handle.
 *          SBR_NOT_INIT        Fail. The SB component is not initialized yet.
 *
 ----------------------------------------------------------------------------*/
INT32 sb_get_eng_fct_tbl(
    HANDLE_T            h_builder,
    SB_ENG_FCT_TBL_T*   pt_eng_fct_tbl)
{
    INT32 i4_ret;
    SB_ENG_CTRL_T* pt_eng;

    if (pt_eng_fct_tbl == NULL)
    {
        return SBR_INV_ARG;
    }

    i4_ret = sb_find_eng_by_handle(h_builder, &pt_eng);
    if (i4_ret == SBR_OK)
    {
        (*pt_eng_fct_tbl) = pt_eng->t_sb_fct_tbl;
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: sb_eng_is_inited
 *
 * Description: API to return the initialziation status of SB engine list.
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: TRUE        The SB engine list had been initialized.
 *          FALSE       The SB engine list is not initialized.
 *
 ----------------------------------------------------------------------------*/
BOOL sb_eng_is_inited(VOID)
{
    return b_sb_eng_inited;
}


