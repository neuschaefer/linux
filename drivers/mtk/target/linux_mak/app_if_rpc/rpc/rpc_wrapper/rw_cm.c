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
#include "conn_mngr/c_cm.h"
#include "conn_mngr/x_cm.h"
#include "conn_mngr/cm_playback/u_playback_handler.h"

#include "handle/handle.h"

#include "rpc.h"
#include "ri_common.h"

#include "ri_desc.h"


#define RET_ON_FAIL(stmt) \
do {\
    INT32 __i4_ret;\
    __i4_ret = (stmt);\
    if(__i4_ret != RPCR_OK)\
    {   RPC_ERROR("RPC Failure %s, #%d\n", __FUNCTION__, __LINE__);\
        return __i4_ret;\
    }\
}while(0)

static INT32 _add_cm_cmd_array(const CM_COMMAND_T *  pt_cmd_sets)
{

    UINT32 ui4_i;
    if(pt_cmd_sets == NULL)
    {
        return RPCR_OK;
    }
    for (ui4_i = 0; pt_cmd_sets[ui4_i].e_code != CONN_END; ui4_i ++)
    {
        switch(pt_cmd_sets[ui4_i].e_code)
        {
            case CONN_SRC_TYPE:
            case PLAYBACK_CONN_VIRTUAL_SOURCE_NAME:
            case PLAYBACK_CONN_BUFFER_AGENT_NAME:
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                             &pt_cmd_sets[ui4_i].u,
                                             RPC_DESC_CM_COMMAND_T_u,                                             
                                             "u.ps_name"));
                RET_ON_FAIL(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_cmd_sets[ui4_i].u.ps_name));
            break;   
            default:
                RI_LOG("CM CMD 0x%x, val: 0x%x\n", pt_cmd_sets[ui4_i].e_code, pt_cmd_sets[ui4_i].u.ui4_number);
        }
    }

    RET_ON_FAIL(RPC_ADD_REF_DESC_ARR(RPC_DEFAULT_ID, ui4_i + 1, pt_cmd_sets, RPC_DESC_CM_COMMAND_T, NULL));
    
    return RPCR_OK;
}


static INT32 _hndlr_x_cm_nfy_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{  
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }   
    
    pt_return->e_type   = ARG_TYPE_VOID;

    ((x_cm_nfy_fct)pv_cb_addr)((HANDLE_T )pt_args[0].u.ui4_arg,
                               (CM_COND_T)pt_args[1].u.ui4_arg,
                               (VOID*    )pt_args[2].u.pv_arg,
                               (UINT32   )pt_args[3].u.ui4_arg);

    return RPCR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: c_cm_open_pipe
 *
 * Description: This API opens a pipe.
 *
 * Inputs:  pt_pipe_cmd   References the commands set for the open pipe
 *                        operation.
 *          ui1_priority  Contains the pipe priority.
 *          ui4_flags     Contains the pipe flags.
 *          pv_nfy_tag    References the tag passed to the notify function.
 *          pf_nfy        References the notify function.
 *
 * Outputs: ph_pipe  Contains the pipe handle.
 *          pe_cond  Contains the pipe condition (CLOSED, CLOSING)
 *
 * Returns: CMR_OK                    Routine successful.
 *          CMR_INV_HANDLE            Invalid pipe handle.
 *          CMR_AEE_NO_RIGHTS         Permission denied.
 *          CMR_AEE_OUT_OF_RESOURCES  The AEE does not have enough resources.
 *          CMR_FAIL                  Routine unsuccessful (other reason).
 ----------------------------------------------------------------------------*/
INT32 c_cm_open_pipe (const CM_COMMAND_T*  pt_pipe_cmd,
                      UINT8                ui1_priority,
                      UINT32               ui4_flags,
                      VOID*                pv_nfy_tag,
                      x_cm_nfy_fct         pf_nfy,
                      HANDLE_T*            ph_pipe,
                      CM_COND_T*           pe_cond)
{
    RPC_DECL(7, INT32);        

    _add_cm_cmd_array(pt_pipe_cmd);
    
    RPC_ARG_INP(ARG_TYPE_REF_DESC,      pt_pipe_cmd);
    RPC_ARG_INP(ARG_TYPE_UINT8,         ui1_priority);
    RPC_ARG_INP(ARG_TYPE_UINT32,        ui4_flags);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,      pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,      pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_pipe);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    pe_cond);
            
    
    RPC_DO_OP("x_cm_open_pipe");

    RPC_RETURN(ARG_TYPE_INT32, CMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_cm_close_pipe
 *
 * Description: This API closes a pipe (previously open with c_cm_open_pipe).
 *
 * Inputs:  h_pipe  Contains a handle to the pipe to close.
 *
 * Outputs: pe_cond  Contains the pipe condition (CLOSED, CLOSING)
 *
 * Returns: CMR_OK          Routine successful.
 *          CMR_INV_HANDLE  Invalid pipe handle.
 *          CMR_FAIL        Routine unsuccessful (other reason).
 ----------------------------------------------------------------------------*/
INT32 c_cm_close_pipe (HANDLE_T    h_pipe,
                       CM_COND_T*  pe_cond)
{
    RPC_DECL(2, INT32);        
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_pipe);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    pe_cond);
    
    RPC_DO_OP("x_cm_close_pipe");

    RPC_RETURN(ARG_TYPE_INT32, CMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_cm_connect
 *
 * Description: This API initiates a connection.
 *
 * Inputs:  h_obj           Contains a handle to an existing connection or to a
 *                          pipe.
 *          pt_connect_cmd  References the commands set for the connect
 *                          operation.
 *          pv_nfy_tag      References the tag passed to the notify function.
 *          pf_nfy          References the notify function.
 *
 * Outputs: ph_conn  Contains the connection handle.
 *          pe_cond  Contains the connection condition (CONNECTING, etc)
 *
 * Returns: CMR_OK                    Routine successful.
 *          CMR_INV_HANDLE            Invalid connection handle.
 *          CMR_AEE_NO_RIGHTS         Permission denied.
 *          CMR_AEE_OUT_OF_RESOURCES  The AEE does not have enough resources.
 *          CMR_FAIL                  Routine unsuccessful (other reason).
 ----------------------------------------------------------------------------*/
INT32 c_cm_connect (HANDLE_T             h_obj,
                    const CM_COMMAND_T*  pt_connect_cmd,
                    VOID*                pv_nfy_tag,
                    x_cm_nfy_fct         pf_nfy,
                    HANDLE_T*            ph_conn,
                    CM_COND_T*           pe_cond)
{
    RPC_DECL(6, INT32);        

    _add_cm_cmd_array(pt_connect_cmd);
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_obj);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,      pt_connect_cmd);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,      pv_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,      pf_nfy);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_conn);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    pe_cond);
            
    
    RPC_DO_OP("x_cm_connect");

    RPC_RETURN(ARG_TYPE_INT32, CMR_INV_ARG);
}

INT32 c_cm_disconnect (HANDLE_T    h_conn,
                       CM_COND_T*  pe_cond)
{
    RPC_DECL(2, INT32);        
    
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_conn);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    pe_cond);            
    
    RPC_DO_OP("x_cm_disconnect");

    RPC_RETURN(ARG_TYPE_INT32, CMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_cm_get
 *
 * Description: This API gets information about a connection.
 *
 * Inputs:  h_conn        Contains a handle to a connection.
 *          e_ctrl        Contains the control code.
 *
 * Outputs: pv_ctrl_data  Contains information about the connection.
 *
 * Returns: CMR_OK          Routine successful.
 *          CMR_INV_ARG     Invalid argument.
 *          CMR_INV_HANDLE  Invalid connection handle.
 *          CMR_FAIL        Routine unsuccessful (other reason).
 ----------------------------------------------------------------------------*/
INT32 c_cm_get (HANDLE_T        h_conn,
                CM_CTRL_TYPE_T  e_ctrl,
                VOID*           pv_get_data)
{
    RPC_DECL(3, INT32);        

    /* TODO: Add ref types */
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_conn);
    RPC_ARG_INP(ARG_TYPE_UINT32,        e_ctrl);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,      pv_get_data);            
    
    RPC_DO_OP("x_cm_get");

    RPC_RETURN(ARG_TYPE_INT32, CMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_cm_set
 *
 * Description: This API sets some attributes of a connection.
 *
 * Inputs:  h_conn        Contains a handle to a connection.
 *          e_ctrl        Contains the control code.
 *          pv_ctrl_data  References a data structure which content depends on
 *                        e_ctrl.
 *
 * Outputs: pv_ctrl_data  Contains values that may have been modified.
 *
 * Returns: CMR_OK          Routine successful.
 *          CMR_INV_ARG     Invalid argument.
 *          CMR_INV_HANDLE  Invalid connection handle.
 *          CMR_FAIL        Routine unsuccessful (other reason).
 ----------------------------------------------------------------------------*/
INT32 c_cm_set (HANDLE_T        h_conn,
                CM_CTRL_TYPE_T  e_ctrl,
                VOID*           pv_set_data)
{
    
    RPC_DECL(3, INT32);        

    switch(e_ctrl)
    {
        case PLAYBACK_CTRL_SET_QUEUE_BUFFER:
        {
            PLAYBACK_HANDLER_QUEUE_BUFFER_T * pt_q_buf = pv_set_data;
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 2, ARG_TYPE_REF_DESC | ARG_DIR_IO));
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_q_buf, RPC_DESC_PLAYBACK_HANDLER_QUEUE_BUFFER_T, NULL));
            RPC_CHECK(RPC_ADD_REF_DESC_ARR(RPC_DEFAULT_ID, 
                                           pt_q_buf->ui4_nb_records,
                                           pt_q_buf->pt_scdb_info, 
                                           RPC_DESC_PLAYBACK_HANDLER_ITEM_SCDB_INFO_T, 
                                           NULL));
            
            if (pt_q_buf->pt_scdb_info != NULL)
            {
                UINT32  ui4_i;
                
                for (ui4_i = 0; ui4_i < pt_q_buf->ui4_nb_records; ui4_i++)
                {
                    RET_ON_FAIL(RPC_ADD_REF_VAL(RPC_DEFAULT_ID, & pt_q_buf->pt_scdb_info[ui4_i].t_stream_id.pv_stream_tag));
                }
            }
            
            RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_q_buf->pv_data, (SIZE_T)pt_q_buf->ui8_size));
            
        }
        break;
        
        case PLAYBACK_CTRL_SET_UNQUEUE:
        {
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 2, ARG_TYPE_UINT32 | ARG_DIR_INP));
        }
        break;
        
        default:
            RI_LOG("c_cm_set specify unkown type %d, 0x%x\n", e_ctrl, pv_set_data);
    }
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_conn);
    RPC_ARG_INP(ARG_TYPE_UINT32,        e_ctrl);
    RPC_ARG(ARG_TYPE_VARIABLE,          pv_set_data);
    
    RPC_DO_OP("x_cm_set");

    RPC_RETURN(ARG_TYPE_INT32, CMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_cm_get_cond
 *
 * Description: This API returns the condition of a connection.
 *
 * Inputs:  h_conn  Contains a handle to a connection.
 *
 * Outputs: pe_cond  Contains the connection condition.
 *
 * Returns: CMR_OK          Routine successful.
 *          CMR_INV_HANDLE  Invalid connection handle.
 *          CMR_FAIL        Routine unsuccessful.
 ----------------------------------------------------------------------------*/
INT32 c_cm_get_cond (HANDLE_T    h_conn,
                     CM_COND_T*  pe_cond)
{
    RPC_DECL(2, INT32);            
    
    RPC_ARG_INP(ARG_TYPE_UINT32,        h_conn);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    pe_cond);
    
    RPC_DO_OP("x_cm_get_cond");

    RPC_RETURN(ARG_TYPE_INT32, CMR_INV_ARG);
}

INT32 c_rpc_reg_cm_cb_hndlrs()
{
    RPC_REG_CB_HNDLR(x_cm_nfy_fct); 
    return RPCR_OK;
}

