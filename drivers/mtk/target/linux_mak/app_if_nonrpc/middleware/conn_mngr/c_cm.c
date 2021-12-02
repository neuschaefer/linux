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
 * $RCSfile: c_cm.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/3 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: 7073777ef840de31d560641fd88f5e7f $
 *
 * Description: 
 *         This file contains the implementation of the Connection Manager c_*
 *         APIs.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "conn_mngr/c_cm.h"
#include "conn_mngr/x_cm.h"
#include "aee/aee.h"
#include "handle/handle.h"


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
    return x_cm_close_pipe(h_pipe, pe_cond);
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
    HANDLE_T  h_aux;
    INT32     i4_res;
    
    i4_res = aee_grab_handle_resource(AEE_FLAG,
                                      1,
                                      & h_aux);

    switch (i4_res)
    {
        case AEER_OK:
        {
            i4_res = x_cm_connect(h_obj,
                                  pt_connect_cmd,
                                  pv_nfy_tag,
                                  pf_nfy,
                                  ph_conn,
                                  pe_cond);
            
            if (i4_res == CMR_OK)
            {
                handle_link_to_aux(h_aux, *ph_conn);
            }
            else
            {
                aee_release_handle_resource(1);
            }
        }
        break;
        
        case AEER_OUT_OF_RESOURCES:
        {
            i4_res = CMR_AEE_OUT_OF_RESOURCES;
        }
        break;

        default:
        {
            i4_res = CMR_AEE_NO_RIGHTS;
        }
        break;
    }
    
    return i4_res;
}

INT32 c_cm_disconnect (HANDLE_T    h_conn,
                       CM_COND_T*  pe_cond)
{
    return x_cm_disconnect(h_conn, pe_cond);
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
    return x_cm_get(h_conn, e_ctrl, pv_get_data);
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
    return x_cm_set(h_conn, e_ctrl, pv_set_data);
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
    return x_cm_get_cond(h_conn, pe_cond);
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
    HANDLE_T  h_aux;
    INT32     i4_res;
    
    i4_res = aee_grab_handle_resource(AEE_FLAG,
                                      1,
                                      & h_aux);

    switch (i4_res)
    {
        case AEER_OK:
        {
            i4_res = x_cm_open_pipe(pt_pipe_cmd,
                                    ui1_priority,
                                    ui4_flags,
                                    pv_nfy_tag,
                                    pf_nfy,
                                    ph_pipe,
                                    pe_cond);
            
            if (i4_res == CMR_OK)
            {
                handle_link_to_aux(h_aux, *ph_pipe);
            }
            else
            {
                aee_release_handle_resource(1);
            }
        }
        break;
        
        case AEER_OUT_OF_RESOURCES:
        {
            i4_res = CMR_AEE_OUT_OF_RESOURCES;
        }
        break;

        default:
        {
            i4_res = CMR_AEE_NO_RIGHTS;
        }
        break;
    }
    
    return i4_res;
}

/*------------------------------------------------------------------*/
/*! @brief Store input main, broadcast source and channel id to MW EEPROM 
                 part when one of them changed.
 *              if not any change, the write operation will be ignore.
 *  @param [in]  b_last_dtv_src    -   Indicate the last src is dtv or not. 
 *  @param [in]  ui1_last_svl_lst   -   Contains last svl list id.
 *  @param [in]  ui4_last_channel_id      -   Contains the last channel id user selected.
 *  @return Return the error code
 *  @retval CMR_OK              -   The routine was successful.
 *  @retval CMR_FAIL         -   The operation failed for eeprom
 */
/*------------------------------------------------------------------*/
INT32 c_cm_store_pre_conn_info(BOOL b_last_dtv_src, 
                                     UINT8 ui1_last_svl_lst, 
                                     UINT32 ui4_last_channel_id)
{
    return x_cm_store_pre_conn_info(b_last_dtv_src, 
                                    ui1_last_svl_lst, 
                                    ui4_last_channel_id);
}

