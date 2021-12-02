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
 * $RCSfile:$
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $SWAuthor: Jun Zuo  $
 *
 * Description: 
 *         This file contains all Table Manager API related implementations.
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/
#include "handle/handle.h"
#include "tbl_mngr/x_tm.h"

#include "rpc.h"
#include "ri_common.h"
#include "ri_desc.h"

/*-----------------------------------------------------------------------------
                    functions implementation
 ----------------------------------------------------------------------------*/

static INT32 _hndlr_x_tm_nfy_fct(
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
    
    pt_return->e_type   = ARG_TYPE_INT32;
    
    pt_return->u.i4_arg = ((x_tm_nfy_fct)pv_cb_addr)(
            (HANDLE_T)pt_args[0].u.ui4_arg,
       (HANDLE_TYPE_T)pt_args[1].u.ui2_arg,
           (TM_COND_T)pt_args[2].u.i4_arg,
                      pt_args[3].u.pv_arg,
                      pt_args[4].u.ui4_arg);
                      
    return RPCR_OK;
}

static INT32 _x_tm_open_root (const CHAR*    ps_name,
                      TM_SRC_TYPE_T  e_src_type,
                      VOID*          pv_src_info,
                      VOID*          pv_nfy_tag,
                      x_tm_nfy_fct   pf_nfy,
                      HANDLE_T*      ph_root,
                      TM_COND_T*     pe_cond)
{
    RPC_DECL(7, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_REF_STR,     ps_name);
    RPC_ARG_INP(ARG_TYPE_UINT8,    e_src_type);
    RPC_ARG_INP(ARG_TYPE_REF_STR, pv_src_info);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,     pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,  ph_root);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,   pe_cond);

    RPC_DO_OP("x_tm_open_root");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}

/*------------------------------------------------------------------------
 * Name: c_tm_change_src
 *
 * Description: This API changes the source of a Table Manager object.
 *
 * Inputs:  h_obj        Contains the Table Manager object.
 *          e_src_type   Specifies the source type for loading the requested
 *                       table.
 *          pv_src_info  Contains additional information describing the source.
 *
 * Outputs: pe_cond  Contains the Table Manager objects state or condition.
 *
 * Returns: TMR_OK                   Routine successful.
 *          TMR_NOT_INIT             The Table Manager is not initialized.
 *          TMR_INV_HANDLE           Invalid handle.
 *          TMR_OBJ_DEL_NOT_ALLOWED  The object cannot be deleted.
 *          TMR_NO_SUPPORT           No support.
 -----------------------------------------------------------------------*/
INT32 c_tm_change_src (HANDLE_T       h_obj,
                       TM_SRC_TYPE_T  e_src_type,
                       VOID*          pv_src_info,
                       TM_COND_T*     pe_cond)
{
#if 0 /* To do */
    return x_tm_change_src(h_obj,e_src_type,pv_src_info,pe_cond);
#else
    RI_ERR("\n -- Error: RPC wrapper line [%d] not implement function %s -- \n",__LINE__, __FUNCTION__);
    return TMR_NO_SUPPORT;
#endif
}


/*---------------------------------------------------------------------------
 * Name: c_tm_delete_obj
 *
 * Description: This API performs a Table Managers delete operation.
 *
 * Inputs:  h_obj  Contains the Table Manager object.
 *
 * Outputs: -
 *
 * Returns: TMR_OK                   Routine successful.
 *          TMR_NOT_INIT             The Table Manager is not initialized.
 *          TMR_INV_HANDLE           Invalid handle.
 *          TMR_OBJ_DEL_NOT_ALLOWED  The object cannot be deleted.
 *          TMR_NO_SUPPORT           No support.
 --------------------------------------------------------------------------*/
INT32 c_tm_delete_obj (HANDLE_T  h_obj)
{
#if 0 /* To do */
    return x_tm_delete_obj(h_obj);
#else
    RI_ERR("\n -- Error: RPC wrapper line [%d] not implement function %s -- \n",__LINE__, __FUNCTION__);
    return TMR_NO_SUPPORT;
#endif
}


/*---------------------------------------------------------------------------
 * Name: c_tm_find_root
 *
 * Description: This API finds an existing root
 *
 * Inputs:  ps_name  Specifies the root name.
 *
 * Outputs: -
 *
 * Returns: TRUE if the root object with the given name exists else FALSE.
 --------------------------------------------------------------------------*/
BOOL c_tm_find_root (const CHAR*  ps_name)
{
#if 0 /* To do */
    return x_tm_find_root(ps_name);
#else
    RI_ERR("\n -- Error: RPC wrapper line [%d] not implement function %s -- \n",__LINE__, __FUNCTION__);
    return TMR_NO_SUPPORT;
#endif
}


/*---------------------------------------------------------------------------
 * Name: c_tm_free
 *
 * Description: This API frees a Table Manager object.
 *
 * Inputs:  h_obj  Contains the Table Manager object.
 *
 * Outputs: -
 *
 * Returns: TMR_OK          Routine successful.
 *          TMR_NOT_INIT    The Table Manager is not initialized.
 *          TMR_INV_HANDLE  Invalid handle.
 --------------------------------------------------------------------------*/
INT32 c_tm_free (HANDLE_T  h_obj)
{
    RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,  h_obj);

    RPC_DO_OP("x_tm_free");

    RPC_RETURN(ARG_TYPE_INT32, TMR_INV_ARG);
}


/*---------------------------------------------------------------------------
 * Name: c_tm_get_cond
 *
 * Description: This API returns a Table Manager objects state or condition.
 *
 * Inputs:  h_obj  Contains the Table Manager object.
 *
 * Outputs: pe_cond  Contains the Table Manager objects state or condition.
 *
 * Returns: TMR_OK          Routine successful.
 *          TMR_NOT_INIT    The Table Manager is not initialized.
 *          TMR_INV_ARG     One or more of the arguments contain invalid data.
 *          TMR_INV_HANDLE  Invalid handle.
 --------------------------------------------------------------------------*/
INT32 c_tm_get_cond (HANDLE_T    h_obj,
                     TM_COND_T*  pe_cond)
{
#if 0 /* To do */
    return x_tm_get_cond(h_obj, pe_cond);
#else
    RI_ERR("\n -- Error: RPC wrapper line [%d] not implement function %s -- \n",__LINE__, __FUNCTION__);
    return TMR_NO_SUPPORT;
#endif
}


/*---------------------------------------------------------------------------
 * Name: c_tm_get_obj
 *
 * Description: This API performs a Table Managers get operation.
 *
 * Inputs:  h_obj        Contains the Table Manager object.
 *          e_get_type   Contains the get operation.
 *          pv_get_info  Contains or references additional information which is
 *                       required for the get operation.
 *          pz_data_len  Contains the length of the data buffer referenced by
 *                       argument "pv_data".
 *
 * Outputs: pv_data      Contains the data returned by the get operation.
 *          pz_data_len  Contains the length of the valid data in the buffer
 *                       referenced by argument "pv_data".
 *          pe_cond      Contains the condition of the table object.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_NO_SUPPORT        No support.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 *          TMR_NOT_ENOUGH_SPACE  Not enough space.
 *          TMR_INV_GET_TYPE      Invalid et operation.
 *          TMR_INV_GET_INFO      Invalid get operation data.
 --------------------------------------------------------------------------*/
INT32 c_tm_get_obj (HANDLE_T       h_obj,
                    TM_GET_TYPE_T  e_get_type,
                    VOID*          pv_get_info,
                    VOID*          pv_data,
                    SIZE_T*        pz_data_len,
                    TM_COND_T*     pe_cond)
{
#if 0 /* To do */
    return x_tm_get_obj(h_obj, e_get_type, pv_get_info,pv_data,pz_data_len,pe_cond);
#else
    RI_ERR("\n -- Error: RPC wrapper line [%d] not implement function %s -- \n",__LINE__, __FUNCTION__);
    return TMR_NO_SUPPORT;
#endif
}

/*---------------------------------------------------------------------------
 * Name: c_tm_get_protection
 *
 * Description: This API returns a Table Manager objects protection state.
 *
 * Inputs:  h_obj  Contains the Table Manager object.
 *
 * Outputs: pb_protected  Contains the objects protection status.
 *          pe_cond       Contains the condition of the table object.
 *
 * Returns: TMR_OK          Routine successful.
 *          TMR_NOT_INIT    The Table Manager is not initialized.
 *          TMR_INV_ARG     One or more of the arguments contain invalid data.
 *          TMR_INV_HANDLE  Invalid handle.
 --------------------------------------------------------------------------*/
INT32 c_tm_get_protection (HANDLE_T    h_obj,
                           BOOL*       pb_protected,
                           TM_COND_T*  pe_cond)
{
#if 0 /* To do */
    return x_tm_get_protection(h_obj,pb_protected,pe_cond);
#else
    RI_ERR("\n -- Error: RPC wrapper line [%d] not implement function %s -- \n",__LINE__, __FUNCTION__);
    return TMR_NO_SUPPORT;
#endif
}

/*---------------------------------------------------------------------------
 * Name: c_tm_get_tbl_version
 *
 * Description: This API returns a Table Manager objects version.
 *
 * Inputs:  h_obj  Contains the Table Manager object.
 *
 * Outputs: pui1_version  Contains the objects version.
 *          pe_cond       Contains the condition of the table object.
 *
 * Returns: TMR_OK          Routine successful.
 *          TMR_NOT_INIT    The Table Manager is not initialized.
 *          TMR_INV_ARG     One or more of the arguments contain invalid data.
 *          TMR_INV_HANDLE  Invalid handle.
 --------------------------------------------------------------------------*/
INT32 c_tm_get_tbl_version (HANDLE_T    h_obj,
                            UINT8*      pui1_version,
                            TM_COND_T*  pe_cond)
{
#if 0 /* To do */
    return x_tm_get_tbl_version(h_obj, pui1_version, pe_cond);
#else
    RI_ERR("\n -- Error: RPC wrapper line [%d] not implement function %s -- \n",__LINE__, __FUNCTION__);
    return TMR_NO_SUPPORT;
#endif
}


/*---------------------------------------------------------------------------
 * Name: c_tm_load_obj
 *
 * Description: This API performs a Table Managers load operation.
 *
 * Inputs:  h_parent      Contains the Table Manager parent object.
 *          e_obj_type    Contains the table object type which shall be
 *                        loaded.
 *          e_load_type   Contains the load operation.
 *          pv_load_info  Contains or references additional information which
 *                        is required for the load operation.
 *          e_src_type    Specifies the source type for loading the requested
 *                        table.
 *          pv_src_info   Contains additional information describing the source.
 *          pv_nfy_tag    Contains a notify tag value.
 *          pf_nfy        References the notify function.
 *
 * Outputs: ph_obj    Contains a handle which references the newly allocate
 *                    Table Manager object.
 *          pe_cond   Contains the condition of the table object.
 *
 * Returns: TMR_OK                   Routine successful.
 *          TMR_NOT_INIT             The Table Manager is not initialized.
 *          TMR_INV_ARG              One or more of the arguments contain
 *                                   invalid data.
 *          TMR_INV_HANDLE           Invalid handle.
 *          TMR_OUT_OF_HANDLES       No handles available.
 *          TMR_OUT_OF_RESOURCES     Out of resources.
 *          TMR_NO_SUPPORT           No support.
 *          TMR_INV_HANDLE_STATE     Invalid handle state.
 *          TMR_INV_OBJ_TYPE         Invalid object type.
 *          TMR_INV_LOAD_TYPE        Invalid load operation.
 *          TMR_INV_LOAD_INFO        Invalid load operation data.
 *          TMR_INV_FLAG_SETTING     Invalid flag settings.
 *          TMR_INV_SRC_TYPE         Invalid source type.
 *          TMR_INV_SRC_INFO         Invalid source information.
 *          TMR_SINGLE_SRC_REQUIRED  Single source required.
 *          TMR_SRC_PARTIAL_OVERLAP  Sources partial overlap.
 *          TMR_AEE_OUT_OF_RESOURCES
 *          TMR_AEE_NO_RIGHTS
 --------------------------------------------------------------------------*/
INT32 c_tm_load_obj (HANDLE_T        h_parent,
                     HANDLE_TYPE_T   e_obj_type,
                     TM_LOAD_TYPE_T  e_load_type,
                     VOID*           pv_load_info,
                     TM_SRC_TYPE_T   e_src_type,
                     VOID*           pv_src_info,
                     VOID*           pv_nfy_tag,
                     x_tm_nfy_fct    pf_nfy,
                     HANDLE_T*       ph_obj,
                     TM_COND_T*      pe_cond)
{
#if 0 /* To do */
    INT32    i4_result;
    HANDLE_T h_aux;

    i4_result = aee_grab_handle_resource(AEE_FLAG, 1, &h_aux);

    switch(i4_result)
    {
    case AEER_OK:
        i4_result = x_tm_load_obj(h_parent, e_obj_type, e_load_type, pv_load_info,
                                  e_src_type, pv_src_info, pv_nfy_tag, pf_nfy,
                                  ph_obj, pe_cond);
                      
        if (i4_result == TMR_OK)
        {
            handle_link_to_aux(h_aux, *ph_obj);
        }
        else
        {
            aee_release_handle_resource(1);
        }
        break;
    case AEER_OUT_OF_RESOURCES:
        i4_result = TMR_AEE_OUT_OF_RESOURCES;
        break;
    default:
        i4_result = TMR_AEE_NO_RIGHTS;
        break;
    }

    return(i4_result);
#else
    RI_ERR("\n -- Error: RPC wrapper line [%d] not implement function %s -- \n",__LINE__, __FUNCTION__);
    return TMR_NO_SUPPORT;
#endif
    
}


/*---------------------------------------------------------------------------
 * Name: c_tm_monitor_obj
 *
 * Description: This API performs a Table Managers monitor operation.
 *
 * Inputs:  h_parent     Contains the Table Manager parent object.
 *          e_obj_type   Contains the table object type which shall be
 *                       monitored.
 *          e_mon_type   Contains the monitor operation.
 *          pv_mon_info  Contains or references additional information which is
 *                       required for the monitor operation.
 *          e_src_type   Specifies the source type for loading the requested
 *                       table.
 *          pv_src_info  Contains additional information describing the source.
 *          pv_nfy_tag   Contains a notify tag value.
 *          pf_nfy       References the notify function.
 *
 * Outputs: ph_obj    Contains a handle which references the newly allocate
 *                    Table Manager object.
 *          pe_cond   Contains the condition of the table object.
 *
 * Returns: TMR_OK                   Routine successful.
 *          TMR_NOT_INIT             The Table Manager is not initialized.
 *          TMR_INV_ARG              One or more of the arguments contain
 *                                   invalid data.
 *          TMR_INV_HANDLE           Invalid handle.
 *          TMR_OUT_OF_HANDLES       No handles available.
 *          TMR_OUT_OF_RESOURCES     Out of resources.
 *          TMR_NO_SUPPORT           No support.
 *          TMR_INV_HANDLE_STATE     Invalid handle state.
 *          TMR_INV_OBJ_TYPE         Invalid object type.
 *          TMR_INV_MON_TYPE         Invalid monitor operation.
 *          TMR_INV_MON_INFO         Invalid monitor operation data.
 *          TMR_INV_SRC_TYPE         Invalid source type.
 *          TMR_INV_SRC_INFO         Invalid source information.
 *          TMR_SINGLE_SRC_REQUIRED  Single source required.
 *          TMR_SRC_PARTIAL_OVERLAP  Sources partial overlap.
 *          TMR_AEE_OUT_OF_RESOURCES
 *          TMR_AEE_NO_RIGHTS
 --------------------------------------------------------------------------*/
INT32 c_tm_monitor_obj (HANDLE_T       h_parent,
                        HANDLE_TYPE_T  e_obj_type,
                        TM_MON_TYPE_T  e_mon_type,
                        VOID*          pv_mon_info,
                        TM_SRC_TYPE_T  e_src_type,
                        VOID*          pv_src_info,
                        VOID*          pv_nfy_tag,
                        x_tm_nfy_fct   pf_nfy,
                        HANDLE_T*      ph_obj,
                        TM_COND_T*     pe_cond)
{
#if 0 /* To do */
    INT32    i4_result;
    HANDLE_T h_aux;

    i4_result = aee_grab_handle_resource(AEE_FLAG, 1, &h_aux);

    switch(i4_result)
    {
    case AEER_OK:
        i4_result = c_tm_monitor_obj(h_parent, e_obj_type, e_mon_type,
                                     pv_mon_info, e_src_type, pv_src_info,
                                     pv_nfy_tag, pf_nfy, ph_obj, pe_cond);
        if (i4_result == TMR_OK)
        {
            handle_link_to_aux(h_aux, *ph_obj);
        }
        else
        {
            aee_release_handle_resource(1);
        }
        break;
    case AEER_OUT_OF_RESOURCES:
        i4_result = TMR_AEE_OUT_OF_RESOURCES;
        break;
    default:
        i4_result = TMR_AEE_NO_RIGHTS;
        break;
    }

    return (i4_result);
#else
    RI_ERR("\n -- Error: RPC wrapper line [%d] not implement function %s -- \n",__LINE__, __FUNCTION__);
    return TMR_NO_SUPPORT;
#endif
}


/*---------------------------------------------------------------------------
 * Name: c_tm_open_root
 *
 * Description: This API opens a new or existing root control object.
 *
 * Inputs:  ps_name    Specifies the root name.
 *          pv_nfy_tag  Contains a tag value which is returned in the notify
 *                      function.
 *          pf_nfy      References the notify function.
 *
 * Outputs: ph_root  Contains a handle referencing the root object.
 *          pe_cond  Contains the state of the root object.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_OUT_OF_HANDLES    No free handle available.
 *          TMR_INV_NAME          The root name is invalid.
 *          TMR_OUT_OF_RESOURCES  Out of resources.
 *          TMR_INV_SRC_TYPE      Invalid source type.
 *          TMR_INV_SRC_INFO      Invalid source information.
 *          TMR_AEE_OUT_OF_RESOURCES
 *          TMR_AEE_NO_RIGHTS
 --------------------------------------------------------------------------*/
INT32 c_tm_open_root (const CHAR*    ps_name,
                      TM_SRC_TYPE_T  e_src_type,
                      VOID*          pv_src_info,
                      VOID*          pv_nfy_tag,
                      x_tm_nfy_fct   pf_nfy,
                      HANDLE_T*      ph_root,
                      TM_COND_T*     pe_cond)
{
    INT32    i4_result;
    i4_result = _x_tm_open_root(ps_name, e_src_type, pv_src_info, pv_nfy_tag,
                               pf_nfy, ph_root, pe_cond);
    return (i4_result);
}


/*---------------------------------------------------------------------------
 * Name: c_tm_parse_obj
 *
 * Description: This API performs a Table Managers parse operation.
 *
 * Inputs:  h_obj          Contains the Table Manager object.
 *          e_parse_type   Contains the parse operation.
 *          pv_parse_info  Contains or references additional information which is
 *                         required for the parse operation.
 *          pv_data        References private data which is passed to the parsing
 *                         function.
 *          z_data_len     Contains the length of the data buffer referenced by
 *                         argument "pv_data".
 *          pf_parse       References the parsing function.
 *
 * Outputs: pe_cond  Contains the condition of the table object.
 *
 * Returns: TMR_OK                 Routine successful.
 *          TMR_NOT_INIT           The Table Manager is not initialized.
 *          TMR_INV_ARG            One or more of the arguments contain invalid
 *                                 data.
 *          TMR_INV_HANDLE         Invalid handle.
 *          TMR_NO_SUPPORT         No support.
 *          TMR_INV_HANDLE_STATE   Invalid handle state.
 *          TMR_INV_PARSE_TYPE     Invalid parse operation.
 *          TMR_INV_PARSE_INFO     Invalid parse operation data.
 *          TMR_PARSE_FCT_FAILURE  Parse function returned a failure.
 --------------------------------------------------------------------------*/
INT32 c_tm_parse_obj (HANDLE_T         h_obj,
                      TM_PARSE_TYPE_T  e_parse_type,
                      VOID*            pv_parse_info,
                      VOID*            pv_data,
                      SIZE_T           z_data_len,
                      x_tm_parse_fct   pf_parse,
                      TM_COND_T*       pe_cond)
{
#if 0 /* To do */
    return x_tm_parse_obj(h_obj, e_parse_type, pv_parse_info, pv_data,
                          z_data_len, pf_parse, pe_cond);
#else
    RI_ERR("\n -- Error: RPC wrapper line [%d] not implement function %s -- \n",__LINE__, __FUNCTION__);
    return TMR_NO_SUPPORT;
#endif
}


/*---------------------------------------------------------------------------
 * Name: c_tm_refresh_obj
 *
 * Description: This API performs a Table Managers refresh operation.
 *
 * Inputs:  h_obj  Contains the Table Manager object.
 *
 * Outputs: pe_cond  Contains the condition of the table object.
 *
 * Returns: TMR_OK          Routine successful.
 *          TMR_NOT_INIT    The Table Manager is not initialized.
 *          TMR_INV_HANDLE  Invalid handle.
 *          TMR_NO_SUPPORT  No support.
 --------------------------------------------------------------------------*/
INT32 c_tm_refresh_obj (HANDLE_T    h_obj,
                        TM_COND_T*  pe_cond)
{
#if 0 /* To do */
    return x_tm_refresh_obj(h_obj, pe_cond);
#else
    RI_ERR("\n -- Error: RPC wrapper line [%d] not implement function %s -- \n",__LINE__, __FUNCTION__);
    return TMR_NO_SUPPORT;
#endif
}

/*--------------------------------------------------------------------------
 * Name: c_tm_set_obj
 *
 * Description: This API performs a Table Managers set operation.
 *
 * Inputs:  h_obj        Contains the Table Manager object.
 *          e_set_type   Contains the set operation.
 *          pv_set_info  Contains or references additional information which is
 *                       required for the set operation.
 *
 * Outputs:
 *          pe_cond      Contains the condition of the table object.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TMR_NO_SUPPORT        No support.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 *          TMR_NOT_ENOUGH_SPACE  Not enough space.
 *          TMR_INV_GET_TYPE      Invalid et operation.
 *          TMR_INV_GET_INFO      Invalid get operation data.
 -------------------------------------------------------------------------*/
INT32 c_tm_set_obj (HANDLE_T       h_obj,
                    TM_SET_TYPE_T  e_set_type,
                    VOID*          pv_set_info,
                    SIZE_T         z_len,
                    TM_COND_T*     pe_cond)
{
#if 0 /* To do */
    return x_tm_set_obj(h_obj, e_set_type, pv_set_info, z_len, pe_cond);
#else
    RI_ERR("\n -- Error: RPC wrapper line [%d] not implement function %s -- \n",__LINE__, __FUNCTION__);
    return TMR_NO_SUPPORT;
#endif
}


/*---------------------------------------------------------------------------
 * Name: c_tm_set_protection
 *
 * Description: This API protects an object from deletion in case the last
 *              handle referencing the object is reed.
 *
 * Inputs:  h_obj        Contains the Table Manager object.
 *          b_protected  Specifies if the object shall be protected or not.
 *
 * Outputs: pe_cond  Contains the condition of the table object.
 *
 * Returns: TMR_OK          Routine successful.
 *          TMR_NOT_INIT    The Table Manager is not initialized.
 *          TMR_INV_HANDLE  Invalid handle.
 --------------------------------------------------------------------------*/
INT32 c_tm_set_protection (HANDLE_T    h_obj,
                           BOOL        b_protected,
                           TM_COND_T*  pe_cond)
{
#if 0 /* To do */
    return x_tm_set_protection(h_obj, b_protected, pe_cond);
#else
    RI_ERR("\n -- Error: RPC wrapper line [%d] not implement function %s -- \n",__LINE__, __FUNCTION__);
    return TMR_NO_SUPPORT;
#endif
}


/*---------------------------------------------------------------------------
 * Name: c_tm_store_obj
 *
 * Description: This API performs a Table Managers store operation.
 *
 * Inputs:  h_obj          Contains the Table Manager object.
 *          e_store_type   Contains the store operation.
 *          pv_store_info  Contains or references additional information which
 *                         is required for the store operation.
 *
 * Outputs: pe_cond  Contains the condition of the table object.
 *
 * Returns: TMR_OK                Routine successful.
 *          TMR_NOT_INIT          The Table Manager is not initialized.
 *          TMR_INV_HANDLE        Invalid handle.
 *          TmR_OUT_OF_RESOURCES  Not enough resources.
 *          TMR_NO_SUPPORT        No support.
 *          TMR_INV_HANDLE_STATE  Invalid handle state.
 *          TMR_INV_STORE_TYPE    Invalid store operation.
 *          TMR_INV_STORE_INFO    Invalid store operation data.
 --------------------------------------------------------------------------*/
INT32 c_tm_store_obj (HANDLE_T         h_obj,
                      TM_STORE_TYPE_T  e_store_type,
                      VOID*            pv_store_info,
                      TM_COND_T*       pe_cond)
{
#if 0 /* To do */
    return x_tm_store_obj(h_obj, e_store_type, pv_store_info, pe_cond);
#else
    RI_ERR("\n -- Error: RPC wrapper line [%d] not implement function %s -- \n",__LINE__, __FUNCTION__);
    return TMR_NO_SUPPORT;
#endif
}

INT32 c_rpc_reg_tm_cb_hndlrs()
{
    RPC_REG_CB_HNDLR(x_tm_nfy_fct);    
    return RPCR_OK;
}



