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
 * $RCSfile:  $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $SWAuthor: Jun Zuo  $
 *
 * Description:
 *         This file contains all Favorites List Manager (FLM)
 *         function implementations.
 *-----------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
                    include files
 ------------------------------------------------------------------------*/
#include "handle/handle.h"
#include "flm/c_flm.h"
#include "flm/x_flm.h"

#include "rpc.h"
#include "ri_common.h"
#include "ri_desc.h"

INT32 _hndlr_pf_flm_nfy_func(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{  
    if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }   
    
    pt_return->e_type   = ARG_TYPE_VOID;
    
    ((pf_flm_nfy_func)pv_cb_addr)(
            (HANDLE_T)pt_args[0].u.ui4_arg,
          (FLM_COND_T)pt_args[1].u.ui4_arg,
                      pt_args[2].u.ui4_arg,
                      pt_args[3].u.pv_arg,
                      pt_args[4].u.ui4_arg);
                      
    return RPCR_OK;
}

static INT32 _x_flm_create
(
    CHAR*            ps_name,
    VOID*            pv_tag,
    pf_flm_nfy_func  pf_flm_notify,
    HANDLE_T*        ph_fav_handle
)
{
    RPC_DECL(4, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, ps_name));
    
    RPC_ARG_INP(ARG_TYPE_REF_STR,    ps_name);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_flm_notify);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_fav_handle);

    RPC_DO_OP("x_flm_create");

    RPC_RETURN(ARG_TYPE_INT32, FLMR_INV_ARG);
	
}

static INT32 _x_flm_open
(
    CHAR*           ps_name,
    VOID*           pv_tag,
    pf_flm_nfy_func pf_flm_notify,
    HANDLE_T*       ph_fav_handle
)
{
    RPC_DECL(4, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, ps_name));
    
    RPC_ARG_INP(ARG_TYPE_REF_STR,    ps_name);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_flm_notify);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_fav_handle);

    RPC_DO_OP("x_flm_open");

    RPC_RETURN(ARG_TYPE_INT32, FLMR_INV_ARG);
	
}

static INT32 _x_flm_master_change_reg(
    VOID*           pv_tag,
    pf_flm_nfy_func pf_flm_notify,
    HANDLE_T*       ph_notify_handle
)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_flm_notify);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_notify_handle);

    RPC_DO_OP("x_flm_master_change_reg");

    RPC_RETURN(ARG_TYPE_INT32, FLMR_INV_ARG);
}

static INT32 _x_flm_fs_load(
    HANDLE_T        h_dir,
    CHAR*           ps_pathname,
    CHAR*           ps_listname,
    VOID*           pv_tag,
    pf_flm_nfy_func pf_flm_notify,
    HANDLE_T*       ph_list
)
{
    RPC_DECL(6, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, ps_pathname));
    RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, ps_listname));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,            h_dir);
    RPC_ARG_INP(ARG_TYPE_REF_STR,     ps_pathname);
    RPC_ARG_INP(ARG_TYPE_REF_STR,     ps_listname);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,         pv_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,  pf_flm_notify);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,      ph_list);

    RPC_DO_OP("x_flm_fs_load");

    RPC_RETURN(ARG_TYPE_INT32, FLMR_INV_ARG);
}

/*-------------------------------------------------------------------------
 * Name: c_flm_create
 *
 * Description: This API creates a favorites list
 *
 * Inputs: 
 *    ps_name        - name of favorites list
 *    pv_tag         - Pointer to tag
 *    pf_notify_func - Pointer to notification function
 *
 * Outputs:
 *    ph_fav_handle - handle of favorites list
 *
 * Returns:
 *    FLMR_OK            - on success
 *    FLMR_INV_ARG       - ps_name or ph_fav_handle are NULL or
 *                         strlen(ps_name) is too long.
 *    FLMR_OUT_OF_MEM    - out of memory
 *    FLMR_OUT_OF_HANDLE - out of handles
 *    FLMR_LIMIT_EXCEED  - Too many favorites lists.
 *
 * Exceptions
 *    None
 *
 ------------------------------------------------------------------------*/
INT32 c_flm_create
(
    CHAR*            ps_name,
    VOID*            pv_tag,
    pf_flm_nfy_func  pf_flm_notify,
    HANDLE_T*        ph_fav_handle
)
{
    INT32    i4_result;
    i4_result = _x_flm_create(ps_name, pv_tag, pf_flm_notify,
                       ph_fav_handle);
    return i4_result;
}

/*-------------------------------------------------------------------------
 * Name: c_flm_delete
 *
 * Description: This API deletes a favorites list
 *
 * Inputs:  -
 *    h_fav_handle - handle to favorites list
 *
 * Outputs: -
 *
 * Returns: -
 *    FLMR_OK - on success
 *    FLMR_INV_HANDLE - handle is not valid
 *    FLMR_INV_ARG - ps_name is NULL
 *
 * Exceptions
 *    None
 *
 ------------------------------------------------------------------------*/
INT32 c_flm_delete
(
    HANDLE_T h_fav_handle
)
{
    RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_fav_handle);

    RPC_DO_OP("x_flm_delete");

    RPC_RETURN(ARG_TYPE_INT32, FLMR_INV_ARG);
}

/*-------------------------------------------------------------------------
 * Name: c_flm_get_max_lists
 *
 * Description: Gets the maximum number of favorites lists allowed
 *
 * Returns: Maximum number of favorites lists allowed.
 *
 * Exceptions: None
 ------------------------------------------------------------------------*/
UINT16 c_flm_get_max_lists(VOID)
{
	RPC_DECL(0, UINT16);
    RPC_DO_OP("x_flm_get_max_lists");

    RPC_RETURN(ARG_TYPE_UINT16, FLMR_INV_ARG);;
}

/*-------------------------------------------------------------------------
 * Name: c_flm_get_max_size
 *
 * Description: Gets the maximum number of entries allowed in a favorites list
 *
 * Returns: Maximum number of entries allowed in a favorites list
 *
 * Exceptions: None
 ------------------------------------------------------------------------*/
UINT16 c_flm_get_max_size(VOID)
{
	RPC_DECL(0, UINT16);
    RPC_DO_OP("x_flm_get_max_size");

    RPC_RETURN(ARG_TYPE_UINT16, FLMR_INV_ARG);;
}

/*-------------------------------------------------------------------------
 * Name: c_flm_get_num_lists
 *
 * Description: This API gets the number of existing favorites lists
 *
 * Inputs:  -
 *
 * Outputs: -
 *    pui2_numlists - pointer to variable to hold number
 *
 * Returns: -
 *    FLMR_OK - on success
 *    FLMR_INV_ARG - pui2_numlists is NULL
 *
 * Exceptions
 *    Aborts on any other error.
 *
 ------------------------------------------------------------------------*/
INT32 c_flm_get_num_lists(
    UINT16* pui2_numlists
)
{
    RPC_DECL(1, INT32);    
    
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16, pui2_numlists);

    RPC_DO_OP("x_flm_get_num_lists");

    RPC_RETURN(ARG_TYPE_INT32, FLMR_INV_ARG);
}

/*-------------------------------------------------------------------------
 * Name: c_flm_get_name_len
 *
 * Description: This API gets the length of the name of the specified
 *              favorites list
 *
 * Inputs:  -
 *    ui2_listnum  - list number
 *
 * Outputs: -
 *    pt_length - pointer to variable to hold name length
 *
 * Returns: -
 *    FLMR_OK - on success
 *    FLMR_INV_ARG - ui2_listnum or pui1_length is not valid
 *    FLMR_REC_NOT_FOUND - no such list
 *
 * Exceptions
 *    Aborts if h_flm_master_list is NULL
 *
 ------------------------------------------------------------------------*/
INT32 c_flm_get_name_len(
    UINT16  ui2_listnum,
    SIZE_T* pz_length
)
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT16,   ui2_listnum);
    RPC_ARG_OUT(ARG_TYPE_REF_SIZE_T, pz_length);

    RPC_DO_OP("x_flm_get_name_len");

    RPC_RETURN(ARG_TYPE_INT32, FLMR_INV_ARG);
}

/*-------------------------------------------------------------------------
 * Name: c_flm_get_name
 *
 * Description: This API gets the name of the specified
 *              favorites list
 *
 * Inputs:  -
 *    ui2_listnum  - list number
 *    pui4_version
 *
 * Outputs: -
 *    ps_name - pointer to string to hold name
 *    pui4_version
 *
 * Returns: -
 *    FLMR_OK - on success
 *    FLMR_INV_ARG - ui2_listnum or ps_name is not valid
 *    FLMR_REC_NOT_FOUND - no such list
 *    FLMR_MODIFIED - favorites list version is different from expected
 *
 * Exceptions
 *    Aborts if h_flm_master_list is NULL
 *
 ------------------------------------------------------------------------*/
INT32 c_flm_get_name(
    UINT16  ui2_listnum,
    CHAR*   ps_name,
    UINT32* pui4_master_version
)
{
    RPC_DECL(3, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, ps_name));
    
    RPC_ARG_INP(ARG_TYPE_UINT16,   ui2_listnum);
    RPC_ARG_OUT(ARG_TYPE_REF_STR, ps_name);
    RPC_ARG_IO(ARG_TYPE_REF_UINT32, pui4_master_version);

    RPC_DO_OP("x_flm_get_name");

    RPC_RETURN(ARG_TYPE_INT32, FLMR_INV_ARG);
}

/*-------------------------------------------------------------------------
 * Name: c_flm_open
 *
 * Description: This API opens a favorites list
 *
 * Inputs:  -
 *    ps_name - name of favorites list
 *    pv_tag - Pointer to tag
 *    pf_notify_func - Pointer to notification function
 *
 * Outputs: -
 *    ph_fav_handle - handle of favorites list
 *
 * Returns: -
 *    FLMR_OK - on success
 *    FLMR_INV_ARG - ph_fav_handle or ps_name are NULL
 *    FLMR_OUT_OF_MEM - 
 *    FLMR_OUT_OF_HANDLE - 
 *    FLMR_LIST_NOT_FOUND
 *
 * Exceptions
 *    None
 *
 ------------------------------------------------------------------------*/
INT32 c_flm_open
(
    CHAR*           ps_name,
    VOID*           pv_tag,
    pf_flm_nfy_func pf_flm_notify,
    HANDLE_T*       ph_fav_handle
)
{
    INT32       i4_result;
    i4_result = _x_flm_open(ps_name, pv_tag, pf_flm_notify,
                       ph_fav_handle);
    return i4_result;
}

/*-------------------------------------------------------------------------
 * Name: c_flm_close
 *
 * Description: This API closes a favorites list
 *
 * Inputs:
 *    h_fav_handle - handle of favorites list
 *
 * Outputs:
 *    None
 *
 * Returns:
 *    FLMR_OK - on success
 *    FLMR_INV_HANDLE - handle is not valid
 *
 * Exceptions:
 *    None
 *
 ------------------------------------------------------------------------*/
INT32 c_flm_close
(
    HANDLE_T h_fav_handle
)
{
    RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_fav_handle);

    RPC_DO_OP("x_flm_close");

    RPC_RETURN(ARG_TYPE_INT32, FLMR_INV_ARG);
}

/*-------------------------------------------------------------------------
 * Name: c_flm_get_num_recs
 *
 * Description: This API gets the number of recs in a favorites list
 *
 * Inputs:  -
 *    h_fav_handle - handle of favorites list
 *
 * Outputs: -
 *    pui2_num_recs - Pointer to memory to receive number of recs
 *    pui4_fav_version - Pointer to memory to receive version of favorites list.
 *
 * Returns: -
 *    FLMR_OK - on success
 *    FLMR_INV_HANDLE - handle is not valid
 *    FLMR_INV_ARG - pui2_num_recs or pui4_fav_version are not valid
 *
 * Exceptions:
 *    None
 *
 ------------------------------------------------------------------------*/
INT32 c_flm_get_num_recs(
    HANDLE_T h_fav_handle,
    UINT16*  pui2_num_recs,
    UINT32*  pui4_fav_version
)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_fav_handle);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16, pui2_num_recs);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pui4_fav_version);

    RPC_DO_OP("x_flm_get_num_recs");

    RPC_RETURN(ARG_TYPE_INT32, FLMR_INV_ARG);
}

/*-------------------------------------------------------------------------
 * Name: c_flm_lock
 *
 * Description: This API locks the specified favorites list
 *
 * Inputs:
 *    h_fav_handle - handle of favorites list
 *
 * Outputs:
 *    None
 *
 * Returns:
 *    FLMR_OK - on success
 *    FLMR_INV_HANDLE - handle is not valid
 *
 * Exceptions:
 *    None
 *
 ------------------------------------------------------------------------*/
INT32 c_flm_lock
(
    HANDLE_T h_fav_handle
)
{
    RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_fav_handle);

    RPC_DO_OP("x_flm_lock");

    RPC_RETURN(ARG_TYPE_INT32, FLMR_INV_ARG);
}

/*-------------------------------------------------------------------------
 * Name: c_flm_unlock
 *
 * Description: This API unlocks the specified favorites list
 *
 * Inputs:
 *    h_fav_handle - handle of favorites list
 *
 * Outputs:
 *    None
 *
 * Returns:
 *    FLMR_OK - on success
 *    FLMR_INV_HANDLE - handle is not valid
 *
 * Exceptions:
 *    None
 *
 ------------------------------------------------------------------------*/
INT32 c_flm_unlock
(
    HANDLE_T h_fav_handle
)
{
    RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_fav_handle);

    RPC_DO_OP("x_flm_unlock");

    RPC_RETURN(ARG_TYPE_INT32, FLMR_INV_ARG);
}

/*-------------------------------------------------------------------------
 * Name: c_flm_get_position
 *
 * Description: This API gets the current position of the specified
 *              service in a favorites list
 *
 * Inputs:  -
 *    h_fav_handle - handle to favorites list
 *    pt_service - Pointer to service to look for
 *    ui2_record_index - If there are more than one records that
 *                match, this selects between them. 0 = first,
 *                1 = second, etc.
 *    pui4_version
 *
 * Outputs: -
 *    pui2_position - pointer to a variable to retrieve position
 *    pui4_version
 *
 * Returns: -
 *    FLMR_OK - on success
 *    FLMR_INV_HANDLE - handle is not valid
 *    FLMR_INV_ARG - pui2_position or pui4_version is not valid
 *    FLMR_REC_NOT_FOUND - record not found
 *    FLMR_MODIFIED - favorites list version is different from expected
 *
 * Exceptions:
 *
 ------------------------------------------------------------------------*/
INT32 c_flm_get_position
(
    HANDLE_T     h_fav_handle,
    UINT16*      pui2_position,
    FLM_REC_T*   pt_service,
    UINT16       ui2_record_index,
    UINT32*      pui4_version
)
{
    RPC_DECL(5, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_service, RPC_DESC_FLM_REC_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,      h_fav_handle);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16, pui2_position);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,      pt_service);
    RPC_ARG_INP(ARG_TYPE_UINT16,  ui2_record_index);
    RPC_ARG_IO(ARG_TYPE_REF_UINT32,   pui4_version);

    RPC_DO_OP("x_flm_get_position");

    RPC_RETURN(ARG_TYPE_INT32, FLMR_INV_ARG);
}

/*-------------------------------------------------------------------------
 * Name: c_flm_add_rec
 *
 * Description: This API adds the specified service to the specified
 *              favorites list
 *
 * Inputs:
 *    h_fav_handle  - handle to favorites list
 *    pt_rec        - pointer to service to add
 *    pui2_position - where to add service
 *
 * Outputs:
 *    pui2_position - where service was actually added
 *
 * Returns:
 *    FLMR_OK            - on success
 *    FLMR_INV_HANDLE    - handle is not valid
 *    FLMR_INV_ARG       - ui2_flags is not valid
 *    FLMR_LIMIT_EXCEED  - no free position in list
 *    FLMR_REC_NOT_FOUND - corrupted list
 *    FLMR_OUT_OF_MEM    - 
 *
 * Exceptions:
 *    Aborts if an error happened as records were being moved down
 *
 ------------------------------------------------------------------------*/
INT32 c_flm_add_rec(
    HANDLE_T     h_fav_handle,
    FLM_REC_T* pt_rec,
    UINT16*      pui2_position
)
{
    RPC_DECL(3, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_rec, RPC_DESC_FLM_REC_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,      h_fav_handle);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,          pt_rec);
    RPC_ARG_IO(ARG_TYPE_REF_UINT16,  pui2_position);

    RPC_DO_OP("x_flm_add_rec");

    RPC_RETURN(ARG_TYPE_INT32, FLMR_INV_ARG);
}

/*-------------------------------------------------------------------------
 * Name: c_flm_del_rec
 *
 * Description: This API deletes the specified rec from the specified
 *              favorites list
 *
 * Inputs:
 *    h_fav_handle - handle to favorites list
 *    ui2_position - position to delete
 *
 * Outputs:
 *
 * Returns:
 *    FLMR_OK            - on success
 *    FLMR_INV_HANDLE    - handle is not valid
 *    FLMR_INV_ARG       - ui2_position is not valid
 *    FLMR_REC_NOT_FOUND - corrupted list
 *
 * Exceptions
 *    Aborts if an error happened while moving records up
 *
 ------------------------------------------------------------------------*/
INT32 c_flm_del_rec(
    HANDLE_T h_fav_handle,
    UINT16   ui2_position
)
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_fav_handle);
    RPC_ARG_INP(ARG_TYPE_UINT16,    ui2_position);

    RPC_DO_OP("x_flm_del_rec");

    RPC_RETURN(ARG_TYPE_INT32, FLMR_INV_ARG);
}

/*-------------------------------------------------------------------------
 * Name: c_flm_get_rec
 *
 * Description: This API gets the service from the specified position in
 *              the specified favorites list
 *
 * Inputs:
 *    h_fav_handle - handle to favorites list
 *    ui2_position - position to get rec of
 *    pui4_version
 *
 * Outputs:
 *    pt_rec - pointer to variable to store the rec in.
 *    pui4_version
 *
 * Returns:
 *    FLMR_OK - on success
 *    FLMR_INV_HANDLE - handle is not valid
 *    FLMR_INV_ARG - t_rec is not valid
 *    FLMR_REC_NOT_FOUND - record not found
 *    FLMR_MODIFIED - favorites list version is different from expected
 *
 * Exceptions:
 *
 ------------------------------------------------------------------------*/
INT32 c_flm_get_rec(
    HANDLE_T      h_fav_handle,
    UINT16        ui2_position,
    FLM_REC_T* pt_rec,
    UINT32*       pui4_version
)
{
    RPC_DECL(4, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_rec, RPC_DESC_FLM_REC_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,      h_fav_handle);
    RPC_ARG_INP(ARG_TYPE_UINT16,      ui2_position);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,          pt_rec);
    RPC_ARG_IO(ARG_TYPE_REF_UINT32,  pui4_version);

    RPC_DO_OP("x_flm_get_rec");

    RPC_RETURN(ARG_TYPE_INT32, FLMR_INV_ARG);
}

/*-------------------------------------------------------------------------
 * Name: c_flm_swap_rec
 *
 * Description: This API swaps the positions of the specified recs
 *              of the specified favorites list
 *
 * Inputs:  -
 *    h_fav_handle - handle to favorites list
 *    ui2_position1 - position of one of the recs to be swapped
 *    ui2_position2 - position of one of the recs to be swapped
 *
 * Outputs: -
 *
 * Returns: -
 *    FLMR_OK - on success
 *    FLMR_INV_HANDLE - handle is not valid
 *
 * Exceptions:
 *
 ------------------------------------------------------------------------*/
INT32 c_flm_swap_rec(
    HANDLE_T h_fav_handle,
    UINT16 ui2_position1,
    UINT16 ui2_position2
)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_fav_handle);
    RPC_ARG_INP(ARG_TYPE_UINT16,    ui2_position1);
    RPC_ARG_INP(ARG_TYPE_UINT16,    ui2_position2);

    RPC_DO_OP("x_flm_swap_rec");

    RPC_RETURN(ARG_TYPE_INT32, FLMR_INV_ARG);
}

/*-------------------------------------------------------------------------
 * Name: c_flm_move_rec
 *
 * Description: This API moves the specified rec from one position
 *              of the specified favorites list to another.
 *
 * Inputs:
 *    h_fav_handle - handle to favorites list
 *    ui2_from - original position of rec to be moved
 *    pui2_to - requested resting place of rec
 *
 * Outputs:
 *    pui2_to - final resting place of rec
 *
 * Returns:
 *    FLMR_OK - on success
 *    FLMR_INV_HANDLE - handle is not valid
 *    FLMR_INV_ARG - pui2_to is not valid
 *    FLMR_REC_NOT_FOUND - no such record
 *    FLMR_LIMIT_EXCEED- no free position in list
 *
 * Exceptions:
 ------------------------------------------------------------------------*/
INT32 c_flm_move_rec(
    HANDLE_T h_fav_handle,
    UINT16   ui2_from,
    UINT16*  pui2_to
)
{
    RPC_DECL(3, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_fav_handle);
    RPC_ARG_INP(ARG_TYPE_UINT16,     ui2_from);
    RPC_ARG_IO(ARG_TYPE_REF_UINT16,   pui2_to);

    RPC_DO_OP("x_flm_move_rec");

    RPC_RETURN(ARG_TYPE_INT32, FLMR_INV_ARG);
}

/*-------------------------------------------------------------------------
 * Name: c_flm_master_change_reg
 *
 * Description: This API returns a handle to register notification
 *              of the changing of the master list.
 *
 * Inputs:  -
 *    pv_tag         - Pointer to tag
 *    pf_flm_notify  - Pointer to notification function
 *
 * Outputs: -
 *    ph_notify_handle  - pointer to storage for handle to notification
 *
 * Returns:
 *    FLMR_OK
 *    FLMR_OUT_OF_MEM
 *    FLMR_OUT_OF_HANDLE
 *
 * Exceptions
 *    Aborts if could not get master list object
 *
 ------------------------------------------------------------------------*/
INT32 c_flm_master_change_reg(
    VOID*           pv_tag,
    pf_flm_nfy_func pf_flm_notify,
    HANDLE_T*       ph_notify_handle
)
{
    INT32 i4_result;
    i4_result = _x_flm_master_change_reg(pv_tag, pf_flm_notify,
                       ph_notify_handle);
    return i4_result;
}

/*-------------------------------------------------------------------------
 * Name: c_flm_fs_load
 *
 * Description: This API loads a specific favorites list
 *              from persistant storage.
 *
 * Inputs:  -
 *    h_file        - handle to file to load from
 *    ps_pathname   - name of list to load
 *    pv_tag        - client data for notification
 *    pf_flm_notify - client notification function
 *
 * Outputs: -
 *    ph_list - Pointer to handle for loaded favorites list
 *
 * Returns
 *    FLMR_OK
 *    FLMR_INV_ARG          - h_dir is not a valid handle
 *    FLMR_LIMIT_EXCEED     - too many favorites lists in CDB
 *    FLMR_LIST_NOT_FOUND   - could not find pv_listname list
 *    FLMR_INV_FILE         - file is not FLM file
 *    FLMR_OUT_OF_HANDLE    - out of handles
 *    FLMR_OUT_OF_MEM       - out of memory
 *    FLMR_CANT_CREATE_LIST - error trying to create list
 *    FLMR_EOF
 *    FLMR_DEV_ERR
 *    FLMR_OS_ERR
 *    FLMR_INVAL_FILE_PATH
 *    FLMR_PERM_DENY       - don't have permission to access file
 *    FLMR_REC_NOT_FOUND   - if an existing record is deleted from
 *                           CDB but name wasn't in master list.
 *
 * Exceptions
 *   Aborts on unlock error
 *   Aborts on close error
 *   Aborts if can't create raw read lock
 *   Aborts if can't unlock raw read lock
 *
 ------------------------------------------------------------------------*/
INT32 c_flm_fs_load(
    HANDLE_T        h_dir,
    CHAR*           ps_pathname,
    CHAR*           ps_listname,
    VOID*           pv_tag,
    pf_flm_nfy_func pf_flm_notify,
    HANDLE_T*       ph_list
)
{
    INT32    i4_result;
    i4_result = _x_flm_fs_load(h_dir, ps_pathname, 
                       ps_listname, pv_tag, pf_flm_notify,
                       ph_list);
    return i4_result;
}

/*-------------------------------------------------------------------------
 * Name: c_flm_fs_store
 *
 * Description: This API stores a favorites list to persistant storage.
 *
 * Inputs:
 *    h_dir       - handle to directory to store to
 *    ps_pathname - name of file to store to
 *    h_list      - handle to favorites list to store
 *
 * Outputs:
 *
 * Returns:
 *    FLMR_OK
 *    FLMR_CANT_CREATE_FILE
 *    FLMR_CANT_OPEN_FILE
 *    FLMR_NO_WRITE_LOCK
 *    FLMR_INV_ARG
 *    FLMR_INV_HANDLE
 *    FLMR_REC_NOT_FOUND
 *    FLMR_LIST_NOT_FOUND
 *    FLMR_OUT_OF_MEM
 *    FLMR_OUT_OF_HANDLE
 *    FLMR_DEV_ERR
 *    FLMR_OS_ERR
 *    FLMR_INV_FILE_PATH
 *    FLMR_PERM_DENY
 *
 flm_store_list
 *
 * Exceptions:
 *    Aborts if h_flm_master_list is NULL
 *    Aborts if can't get master object
 *    Aborts if can't get master object info
 *    Aborts if can't get list name
 *    Aborts if can't open list
 *    Aborts if can't unlock files
 *    Aborts if can't close files
 *    Aborts if can't create semaphore
 *    Aborts if raw device error
 *    Aborts if no memory for raw device header
 *    Aborts on file write errors for file into/metadata
 *
 ------------------------------------------------------------------------*/
INT32 c_flm_fs_store(
    HANDLE_T h_dir,
    CHAR*    ps_pathname,
    HANDLE_T h_list
)
{
    RPC_DECL(3, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, ps_pathname));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,         h_dir);
    RPC_ARG_INP(ARG_TYPE_REF_STR,  ps_pathname);
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_list);

    RPC_DO_OP("x_flm_fs_store");

    RPC_RETURN(ARG_TYPE_INT32, FLMR_INV_ARG);
}

/*-------------------------------------------------------------------------
 * Name: c_flm_fs_del
 *
 * Description: This API deletes
 *
 * Inputs:
 *    h_dir       - handle to directory to delete from
 *    ps_pathname - name of file to delete from
 *    ps_listname - name of list to delete??
 *
 * Outputs:
 *
 * Returns:
 *
 * Exceptions:
 *
 ------------------------------------------------------------------------*/
INT32 c_flm_fs_del(
    HANDLE_T h_dir,
    CHAR*    ps_pathname,
    CHAR*    ps_listname
)
{
    RPC_DECL(3, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, ps_pathname));
    RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, ps_listname));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,          h_dir);
    RPC_ARG_INP(ARG_TYPE_REF_STR,   ps_pathname);
    RPC_ARG_INP(ARG_TYPE_REF_STR,   ps_listname);

    RPC_DO_OP("x_flm_fs_del");

    RPC_RETURN(ARG_TYPE_INT32, FLMR_INV_ARG);
}

/*-------------------------------------------------------------------------
 * Name: c_flm_fs_qry
 *
 * Description: This API queries
 *
 * Inputs:  -
 *    h_dir       - handle to directory to query
 *    ps_pathname - name of file to query
 *    ui2_index   - index (0=first, 1=second,...)
 *
 * Outputs: -
 *    ps_listname - name of list to query about
 *
 * Returns:
 *
 * Exceptions:
 *
 ------------------------------------------------------------------------*/
INT32 c_flm_fs_qry(
    HANDLE_T h_dir,
    CHAR*    ps_pathname,
    UINT16   ui2_index,
    CHAR*    ps_listname
)
{
    RPC_DECL(4, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, ps_pathname));
    RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, ps_listname));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,          h_dir);
    RPC_ARG_INP(ARG_TYPE_REF_STR,   ps_pathname);
    RPC_ARG_INP(ARG_TYPE_UINT16,      ui2_index);
    RPC_ARG_OUT(ARG_TYPE_REF_STR,   ps_listname);

    RPC_DO_OP("x_flm_fs_qry");

    RPC_RETURN(ARG_TYPE_INT32, FLMR_INV_ARG);
}

INT32 c_rpc_reg_flm_cb_hndlrs()
{
    RPC_REG_CB_HNDLR(pf_flm_nfy_func);    
    return RPCR_OK;
}


