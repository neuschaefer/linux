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
 * $RCSfile: c_svctx.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/6 $
 * $SWAuthor: Weider Chang $
 * $MD5HEX: a7b5992f37975582d6d8dbacc02ac58d $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 *  include files
 *---------------------------------------------------------------------------*/
#include "svctx/x_svctx.h"
#include "os/inc/x_os.h"

#include "aee/aee.h"
#include "handle/handle.h"
#include "svctx/svctx_dbg.h"

/*-----------------------------------------------------------------------------
 * SVCTX export methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: c_svctx_open
 *
 * Description: This API open a new service context from the service context
 *              pool or attaches to an existing service context if the name
 *              is existed.
 *
 * Inputs:  ps_svctx_name      References the service context name for the open.
 *
 * Outputs: ph_svctx           Contains the service context handle.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_INV_ARG     Invalid argument.
 *          SVCTXR_NO_RESOURCE Not enough memory to open the service context.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_FAIL        The Handle link operation is failed.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_open(
                    const CHAR*                     ps_svctx_name,    /* in  */
                    HANDLE_T*                       ph_svctx          /* out */
                    )
{
    HANDLE_T  h_aux;
    INT32     i4_ret;

    i4_ret = aee_grab_handle_resource(
                    AEE_FLAG,
                    1,
                    &h_aux
                    );

    if (i4_ret == AEER_OK) {
        i4_ret = x_svctx_open(
                    ps_svctx_name,
                    ph_svctx
                    );
        if (i4_ret == SVCTXR_OK){
            handle_link_to_aux(h_aux, *ph_svctx);
        } else {
            aee_release_handle_resource(1);
        }
    } else if (i4_ret == AEER_OUT_OF_RESOURCES) {
        i4_ret = SVCTXR_NO_RESOURCE;
    } else {
        i4_ret = SVCTXR_FAIL;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_close
 *
 * Description: This API performs a close. If the handle is the last client's
 *              handle, this service context instance will return to the service
 *              context pool.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_FAIL        Failed to close the service context
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_close(
                    HANDLE_T                        h_svctx           /* in  */
                    )
{
    INT32 i4_ret;

    i4_ret = x_svctx_close(h_svctx);

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_select_svc
 *
 * Description: Selects a service to be presented in this service context
 *              instance in asynchronous way. If the instance is already
 *              presenting content, the new selection request will replace
 *              the previous request being presented. If the
 *              instance is not presenting, successful conclusion of this
 *              operation results in the instance entering the presenting
 *              state. Successful completion of the selection is notified
 *              via pf_nfy with
 *                  e_nfy_cond = SVCTX_COND_PRESENTING,
 *                  e_code     = SVCTX_NTFY_CODE_NORMAL
 *              If the signal is loss, the client will be notified via pf_nfy
 *                  e_nfy_cond = SVCTX_COND_READY,
 *                  e_code     = SVCTX_NTFY_CODE_SIGNAL_LOSS
 *              And in such case, the service context will try to re-connect.
 *
 *              In processing the selection, the client will be notified
 *              when the signal is locked (connection connected) via pf_nfy with
 *                  e_nfy_cond = SVCTX_COND_PENDING,
 *                  e_code     = SVCTX_NTFY_CODE_SIGNAL_LOCKED
 *              In processing the selection, the client will be notified
 *              when the service ls blocked after being checked by the x_svctx_svc_block_check_fct
 *              function provided by client in pt_snk_desc via pf_nfy with
 *                  e_nfy_cond = SVCTX_COND_PENDING,
 *                  e_code     = SVCTX_NTFY_CODE_SERVICE_BLOCKED
 *              In processing the selection, the client will be notified
 *              when a kind of stream is not existed in SCDB  via pf_nfy with
 *                  e_nfy_cond    = SVCTX_COND_PENDING,
 *                  e_code        = SVCTX_NTFY_CODE_NO_STREAM
 *                  e_stream_type = the actual stream type
 *              If the selection process fails becuase lack of resources
 *              the client will be notified via pf_nfy.
 *                  e_nfy_cond    = current service context state,
 *                  e_code        = SVCTX_NTFY_CODE_NO_RESOURCES
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          ui4_stream_mode    References the desired stream types for
 *                             the select operation. Client should use the
 *                             macros ST_MASK_AUDIO, ST_MASK_VIDEO,
 *                             ST_MASK_CLOSED_CAPTION and so on (defined in
 *                             u_common.h) to OR them for this input argument.
 *          pt_src_desc        References the source description.
 *          pt_snk_desc        References the sink description.
 *          pf_nfy             References the service notify function.
 *          pv_nfy_tag         References the tag passed to the notify function.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     Invalid argument.
 *          SVCTXR_NO_RESOURCE Not enough memory to allocate this request.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_FAIL        Failed to send request
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_select_svc(
                    HANDLE_T                        h_svctx,          /* in  */
                    UINT32                          ui4_stream_mode,  /* in  */
                    const SRC_DESC_T*               pt_src_desc,      /* in  */
                    const SNK_DESC_T*               pt_snk_desc,      /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    )
{
    return x_svctx_select_svc(
                    h_svctx,
                    ui4_stream_mode,
                    pt_src_desc,
                    pt_snk_desc,
                    pf_nfy,
                    pv_nfy_tag
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_stop_svc
 *
 * Description: This API causes the service context instance to stop presenting
 *              content and enter the READY state in asynchronous way.
 *              Connection, SCDB, Session, and Streams used in the presentation
 *              will be released. The client
 *              that issues the service selection request will be notified
 *              via its pf_nfy (set in x_svctx_select_svc()) with
 *                  e_nfy_cond = SVCTX_COND_READY,
 *                  e_code     = SVCTX_NTFY_CODE_USER_STOP
 *              If the service is already in stopping, the client
 *              that issues the service selection request will still be notified
 *              via its pf_nfy (set in x_svctx_select_svc()) with
 *                  e_nfy_cond = SVCTX_COND_PENDING,
 *                  e_code     = SVCTX_NTFY_CODE_IN_STOPPING
 *              If the service context instance is in the READY state, the client
 *              that issues the service selection request will still be notified
 *              via its pf_nfy (set in x_svctx_select_svc()) with
 *                  e_nfy_cond = SVCTX_COND_READY,
 *                  e_code     = SVCTX_NTFY_CODE_STOPPED
 *              If the service context instance is in the POOLED state,
 *              no action is performed
 * Inputs:  h_svctx            Contains a handle to a service context.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_FAIL        Failed to send request.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_stop_svc(
                    HANDLE_T                        h_svctx           /* in  */
                    )
{
    return x_svctx_stop_svc(h_svctx);
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_is_running
 *
 * Description: This API is used to check the service context instance is in
 *              running or not
 * Inputs:  h_svctx            Contains a handle to a service context.
 *
 * Outputs: pb_result          Contains the result.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_INV_ARG     Invalid argument.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_FAIL        Failed to send request
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_is_running (
                    HANDLE_T                    h_svctx,
                    BOOL*                       pb_result
                    )
{
    return x_svctx_is_running(
                    h_svctx,
                    pb_result
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_sync_stop_svc
 *
 * Description: This API causes the service context instance to stop presenting
 *              content and enter the READY state synchronously.
 *              Connection, SCDB, Session,
 *              Streams used in the presention will be released. The client
 *              that issues the service selection request will be notified
 *              via its pf_nfy (set in x_svctx_select_svc()) with
 *              (SVCTX_COND_READY, SVCTX_NTFY_CODE_USER_STOP).
 *              This API completes asynchronously. No action is performed if the
 *              service context instance is already in the READY or POOLED state.
 * Inputs:  h_svctx            Contains a handle to a service context.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_sync_stop_svc(
                    HANDLE_T                        h_svctx,
                    UINT32                          ui4_time
                    )
{
    return x_svctx_sync_stop_svc(
                    h_svctx,
                    ui4_time
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_set_volume
 *
 * Description: This API is used to set the audio volume.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          pt_volume_info     References to the structure containing the
 *                             scc audio volume information data.
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_ARG     pt_volume_info is NULL, or volume is over
 *                             AUDIO_MAX_VOLUME or
 *                             out port is SCC_AUD_OUT_PORT_2_CH but e_ch is not
 *                             one of SCC_AUD_CHANNEL_FRONT_LEFT,
 *                             SCC_AUD_CHANNEL_FRONT_RIGHT, and
 *                             SCC_AUD_CHANNEL_ALL.
 *          SVCTXR_FAIL        The operation for accessing device is failed.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_set_volume(
                    HANDLE_T                        h_svctx,          /* in  */
                    const SCC_AUD_VOLUME_INFO_T*    pt_volume_info    /* in  */
                    )
{
    return x_svctx_set_volume(h_svctx, pt_volume_info);
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_get_volume
 *
 * Description: This API is used to get the audio volume.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 * Outputs: pt_volume_info     References to the structure containing the
 *                             scc audio volume information data.
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_ARG     pt_volume_info is NULL
 *          SVCTXR_FAIL        The operation for accessing device is failed.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_get_volume(
                    HANDLE_T                        h_svctx,            /* in  */
                    SCC_AUD_VOLUME_INFO_T*          pt_volume_info      /* out */
                    )
{
    return x_svctx_get_volume(h_svctx, pt_volume_info);
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_set_mute
 *
 * Description: This API is used to set the audio to mute or unmute.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          b_is_mute          Contains a boolean value for setting audio
 *                             to mute or unmute.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_FAIL        The operation for accessing device is failed.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_set_mute(
                    HANDLE_T                        h_svctx,          /* in  */
                    BOOL                            b_is_mute         /* in  */
                    )
{
    return x_svctx_set_mute(h_svctx, b_is_mute);
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_get_mute
 *
 * Description: This API is used to check the audio is mute or unmute.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *
 * Outputs: pb_is_mute         References to a boolean value for retrieving audio
 *                             is mute or unmute.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_INV_ARG     pb_is_mute is NULL
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_FAIL        The operation for accessing device is failed.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_get_mute(
                    HANDLE_T                        h_svctx,          /* in  */
                    BOOL*                           pb_is_mute        /* out */
                    )
{
    return x_svctx_get_mute(h_svctx, pb_is_mute);
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_get_stream_comp_id
 *
 * Description: This API retrieves the stream id and stream data from
 *              stream component database by a specific type and a filter
 *              function provided by caller. If the data is not populated into
 *              stream component database, caller will get a SVCTXR_WRONG_STATE
 *              return value.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          e_strm_type        Contains the stream component type for filtering
 *          pf_filter          References to the filter function provided by caller.
 *                             It can't be NULL.
 *          pv_fltr_tag        References to a caller-defined tag. It will pass to
 *                             to caller when the pf_filter is invoked.
 * Outputs: pt_comp_id         References to the structure containing the
 *                             stream component id. It can't be NULL.
 *          pt_scdb_rec        References to the structure containing the
 *                             stream component data.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pf_filter or pt_comp_id is NULL.
 *          SVCTXR_WRONG_STATE If related SCDB is not ready and opened.
 *          SVCTXR_NOT_EXIST   The stream component is not existed.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_get_stream_comp_id(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_strm_type,        /* in  */
                    x_svctx_strm_comp_filter_fct    pf_filter,          /* in  */
                    VOID*                           pv_fltr_tag,        /* in  */
                    STREAM_COMP_ID_T*               pt_comp_id,         /* out */
                    SCDB_REC_T*                     pt_scdb_rec         /* out */
                    )
{
    return x_svctx_get_stream_comp_id(
                    h_svctx,
                    e_strm_type,
                    pf_filter,
                    pv_fltr_tag,
                    pt_comp_id,
                    pt_scdb_rec
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_get_strm_req_type
 *
 * Description: This API retrieves 
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *
 * Outputs: pe_strm_req_type   References to the enumeration containing the
 *                             stream req type.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_INV_ARG     pe_strm_req_type is NULL.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NOT_EXIST   The stream component is not existed.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_get_strm_req_type(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_strm_type,        /* in  */
                    STRM_REQ_TYPE_T*                pe_strm_req_type    /* out */
                    )
{
    return x_svctx_get_strm_req_type(
                    h_svctx,
                    e_strm_type,
                    pe_strm_req_type
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_get_using_scdb_rec
 *
 * Description: This API retrieves the selected stream of a specific type from
 *              a presenting service context object.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          e_strm_type        Contains the stream component type for filtering
 *
 * Outputs: pt_scdb_rec        References to the structure containing the
 *                             stream component data.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_INV_ARG     pt_scdb_rec is NULL.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NOT_EXIST   The stream component is not existed.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_get_using_scdb_rec(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_strm_type,        /* in  */
                    SCDB_REC_T*                     pt_scdb_rec         /* out */
                    )
{
    return x_svctx_get_using_scdb_rec(
                    h_svctx,
                    e_strm_type,
                    pt_scdb_rec
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_select_stream
 *
 * Description: This APIis used to stop a specific stream via stream type
 *              in asynchronous way.
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          e_strm_type        Contains the stream type for stopping
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_FAIL        Send request failed
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_select_stream(
                    HANDLE_T                        h_svctx,            /* in  */
                    const STREAM_COMP_ID_T*         pt_comp_id,         /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,             /* in  */
                    VOID*                           pv_nfy_tag          /* in  */
                    )
{
    return x_svctx_select_stream(
                    h_svctx,
                    pt_comp_id,
                    pf_nfy,
                    pv_nfy_tag
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_select_mpeg_stream_by_pid
 *
 * Description: This API is used to selects a specific stream via PID
 *              in asynchronous way.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          e_strm_type        Contains the stream type for selecting
 *          t_mpeg_2_pid       Contains the MPEG2 PID value
 *          pf_nfy             References the stream notify function.
 *          pv_nfy_tag         References the tag passed to the notify function.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NO_RESOURCE Not enough memory to allocate this request.
 *          SVCTXR_INV_ARG     pt_comp_id is NULL.
 *          SVCTXR_FAIL        Send request failed
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_NOT_EXIST   The related SCDB record is not existed.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_select_mpeg_stream_by_pid(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_strm_type,
                    MPEG_2_PID_T                    t_mpeg_2_pid,
                    x_svctx_select_nfy_fct          pf_nfy,
                    VOID*                           pv_nfy_tag
                    )
{
    return x_svctx_select_mpeg_stream_by_pid(
                    h_svctx,
                    e_strm_type,
                    t_mpeg_2_pid,
                    pf_nfy,
                    pv_nfy_tag
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_select_mpeg_stream_by_pid_ex
 *
 * Description: This API is used to selects a specific stream via PID
 *              in asynchronous way.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          pt_stream_info     References the stream info data structure.     
 *          pf_nfy             References the stream notify function.
 *          pv_nfy_tag         References the tag passed to the notify function.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NO_RESOURCE Not enough memory to allocate this request.
 *          SVCTXR_INV_ARG     pt_comp_id is NULL.
 *          SVCTXR_FAIL        Send request failed
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_NOT_EXIST   The related SCDB record is not existed.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_select_mpeg_stream_by_pid_ex(
                    HANDLE_T                        h_svctx,
                    SVCTX_STREAM_INFO_T*            pt_stream_info,
                    x_svctx_select_nfy_fct          pf_nfy,
                    VOID*                           pv_nfy_tag
                    )
{
    return x_svctx_select_mpeg_stream_by_pid_ex(
                    h_svctx,
                    pt_stream_info,
                    pf_nfy,
                    pv_nfy_tag
                    );                        
}                  

/*-----------------------------------------------------------------------------
 * Name: c_svctx_select_mpeg_ad_stream_by_pid
 *
 * Description: This API is used to selects a specific AD stream via PID
 *              in asynchronous way.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          pt_stream_info     References the stream info data structure.     
 *          pf_nfy             References the stream notify function.
 *          pv_nfy_tag         References the tag passed to the notify function.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NO_RESOURCE Not enough memory to allocate this request.
 *          SVCTXR_INV_ARG     pt_comp_id is NULL.
 *          SVCTXR_FAIL        Send request failed
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_NOT_EXIST   The related SCDB record is not existed.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_select_mpeg_ad_stream_by_pid(
                    HANDLE_T                        h_svctx,
                    SVCTX_STREAM_INFO_T*            pt_stream_info,
                    x_svctx_select_nfy_fct          pf_nfy,
                    VOID*                           pv_nfy_tag
                    )
{
    return x_svctx_select_mpeg_ad_stream_by_pid(
                    h_svctx,
                    pt_stream_info,
                    pf_nfy,
                    pv_nfy_tag
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_stop_stream
 *
 * Description: This APIis used to stop a specific stream via stream type
 *              in asynchronous way.
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          e_strm_type        Contains the stream type for stopping
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_FAIL        Send request failed
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_stop_stream(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_strm_type
                    )
{
    return x_svctx_stop_stream(
                    h_svctx,
                    e_strm_type
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_sync_stop_stream
 *
 * Description: This APIis used to stop a specific stream via stream type
 *              in synchronous way.
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          e_strm_type        Contains the stream type for stopping
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_FAIL        Send request failed
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_sync_stop_stream(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_strm_type,
                    UINT32                          ui4_time
                    )
{
    return x_svctx_sync_stop_stream(
                    h_svctx,
                    e_strm_type,
                    ui4_time
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_select_audio_stream
 *
 * Description: This API selects a specific audio stream via ISO639 language
 *              code in asynchronous way.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          pt_lang            References to the structure containing the
 *                             language code.
 *          pf_nfy             References the stream notify function.
 *          pv_nfy_tag         References the tag passed to the notify function.
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_ARG     pt_lang is NULL.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NO_RESOURCE Not enough memory to allocate this request.
 *          SVCTXR_FAIL        Send request failed
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_NOT_EXIST   The stream component is not existed.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_select_audio_stream(
                    HANDLE_T                        h_svctx,            /* in  */
                    const ISO_639_LANG_T*           pt_lang,            /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,             /* in  */
                    VOID*                           pv_nfy_tag          /* in  */
                    )
{
    return x_svctx_select_audio_stream(
                    h_svctx,
                    pt_lang,
                    pf_nfy,
                    pv_nfy_tag
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_select_cc_stream
 *
 * Description: This API selects a specific closed captioning stream in
 *              asynchronous way. If the cc stream is selected successfully,
 *              caller will be notified via the pf_nfy with
 *                  e_code        = SVCTX_NTFY_CODE_STREAM_STARTED
 *                  e_stream_type = ST_SUBTITLE
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          h_cc_gl_plane      Contains a handle to a graphic plane for closed
 *                             captioning handler to draw.
 *          pt_cc_filter       References to the structure containing the
 *                             cc filter data.
 *          pf_nfy             References the stream notify function.
 *          pv_nfy_tag         References the tag passed to the notify function.
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_ARG     pt_cc_filter is NULL or h_cc_gl_plane is NULL_HANDLE.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NO_RESOURCE Not enough memory to allocate this request.
 *          SVCTXR_FAIL        Send request failed
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_select_cc_stream(
                    HANDLE_T                        h_svctx,          /* in  */
                    HANDLE_T                        h_cc_gl_plane,    /* in  */
                    const SM_CC_FILTER_T*           pt_cc_filter,     /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    )
{
    return x_svctx_select_cc_stream(
                    h_svctx,
                    h_cc_gl_plane,
                    pt_cc_filter,
                    pf_nfy,
                    pv_nfy_tag
                    );
}
#ifdef MW_ISDB_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: c_svctx_select_isdb_caption_stream
 *
 * Description: 
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_select_isdb_caption_stream(
                    HANDLE_T                        h_svctx,          /* in  */
                    HANDLE_T                        h_gl_plane,       /* in  */
                    UINT32                          ui4_caption_idx,  /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    )
{
    return x_svctx_select_isdb_caption_stream(
                    h_svctx,
                    h_gl_plane,
                    ui4_caption_idx,
                    pf_nfy,
                    pv_nfy_tag
                    );
}

/*-----------------------------------------------------------------------------
 * Name: c_svctx_select_isdb_text_stream
 *
 * Description: 
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_select_isdb_text_stream(
                    HANDLE_T                        h_svctx,          /* in  */
                    HANDLE_T                        h_gl_plane,       /* in  */
                    UINT32                          ui4_text_idx,     /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    )
{
    return x_svctx_select_isdb_text_stream(
                    h_svctx,
                    h_gl_plane,
                    ui4_text_idx,
                    pf_nfy,
                    pv_nfy_tag
                    );
}
#endif
/*-----------------------------------------------------------------------------
 * Name: c_svctx_select_subtitle_stream
 *
 * Description: This API selects a specific subtitle stream in
 *              asynchronous way. If the subtitle stream is selected successfully,
 *              caller will be notified via the pf_nfy with
 *                  e_nfy_cond    = SVCTX_COND_PRESENTING,
 *                  e_code        = SVCTX_NTFY_CODE_STREAM_STARTED
 *                  e_stream_type = ST_CLOSED_CAPTION
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          h_gl_plane         Contains a handle to a graphic plane for closed
 *                             captioning handler to draw.
 *          pt_lang            References to the structure containing the
 *                             language code.
 *          pf_nfy             References the stream notify function.
 *          pv_nfy_tag         References the tag passed to the notify function.
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_ARG     pt_lang is NULL or h_gl_plane is NULL_HANDLE.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NO_RESOURCE Not enough memory to allocate this request.
 *          SVCTXR_FAIL        Send request failed
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_select_subtitle_stream(
                    HANDLE_T                        h_svctx,          /* in  */
                    HANDLE_T                        h_gl_plane,       /* in  */
                    const ISO_639_LANG_T*           pt_lang,          /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    )
{
    return x_svctx_select_subtitle_stream(
                    h_svctx,
                    h_gl_plane,
                    pt_lang,
                    pf_nfy,
                    pv_nfy_tag
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_select_subtitle_stream_by_comp_id
 *
 * Description: This API selects a specific subtitle stream in
 *              asynchronous way. If the subtitle stream is selected successfully,
 *              caller will be notified via the pf_nfy with
 *                  e_code        = SVCTX_NTFY_CODE_STREAM_STARTED
 *                  e_stream_type = ST_SUBTITLE
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          h_gl_plane         Contains a handle to a graphic plane for closed
 *                             captioning handler to draw.
 *          pt_comp_id         References to the structure containing the
 *                             stream component id.
 *          pf_nfy             References the stream notify function.
 *          pv_nfy_tag         References the tag passed to the notify function.
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_ARG     pt_lang is NULL or h_gl_plane is NULL_HANDLE.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NO_RESOURCE Not enough memory to allocate this request.
 *          SVCTXR_FAIL        Send request failed
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_select_subtitle_stream_by_comp_id(
                    HANDLE_T                        h_svctx,          /* in  */
                    HANDLE_T                        h_gl_plane,       /* in  */
                    const STREAM_COMP_ID_T*         pt_comp_id,       /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    )
{
    return x_svctx_select_subtitle_stream_by_comp_id(
                    h_svctx,          
                    h_gl_plane,       
                    pt_comp_id,       
                    pf_nfy,           
                    pv_nfy_tag        
                    );
}                    
/*-----------------------------------------------------------------------------
 * Name: c_svctx_select_subtitle_stream_by_comp_id_ex
 *
 * Description: This API selects a specific subtitle stream in
 *              asynchronous way. If the subtitle stream is selected successfully,
 *              caller will be notified via the pf_nfy with
 *                  e_code        = SVCTX_NTFY_CODE_STREAM_STARTED
 *                  e_stream_type = ST_SUBTITLE
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          h_gl_plane         Contains a handle to a graphic plane for closed
 *                             captioning handler to draw.
 *          pt_sbtl_info       Contains extern information for subtitle playback.
 *          pt_comp_id         References to the structure containing the
 *                             stream component id.
 *          pf_nfy             References the stream notify function.
 *          pv_nfy_tag         References the tag passed to the notify function.
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_ARG     pt_lang is NULL or h_gl_plane is NULL_HANDLE.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NO_RESOURCE Not enough memory to allocate this request.
 *          SVCTXR_FAIL        Send request failed
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_select_subtitle_stream_by_comp_id_ex(
                    HANDLE_T                        h_svctx,          /* in  */
                    HANDLE_T                        h_gl_plane,       /* in  */
                    const SM_SBTLH_STREAM_INFO*     pt_sbtl_info,     /* in  */
                    const STREAM_COMP_ID_T*         pt_comp_id,       /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    )
{
    return x_svctx_select_subtitle_stream_by_comp_id_ex(
                    h_svctx,          
                    h_gl_plane,
                    pt_sbtl_info,
                    pt_comp_id,
                    pf_nfy,           
                    pv_nfy_tag        
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_get_svc_rec_by_channel_id
 *
 * Description: This API is used to get service list record by channel id and
 *              surfing mode, i.e. next channel, previous channel, or this channel
 * Inputs:      h_svl               Contains a handle to a service list.
 *              ui4_nw_mask         Contains the network mask for filtering service
 *                                  record.
 *              e_surf_mode         Contains the surfing mode.
 *              pui4_channel_id     References to the variable containing the
 *                                  channel id.
 * Outputs:     pui4_channel_id     References to the variable containing the
 *                                  resulted channel id.
 *              pt_svl_rec          References to the structure containing the
 *                                  service list record data.
 * Returns: SVCTXR_OK           Routine successful.
 *          SVCTXR_INV_HANDLE   h_svl is invalid
 *          SVCTXR_INV_ARG      pui4_channel_id or pt_svl_rec is NULL.
 *          SVCTXR_NO_RECORD    There's no any record in this service list
 *          SVCTXR_FAIL         Accessing service list failed.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_get_svc_rec_by_channel_id(
                    HANDLE_T                        h_svl,              /* in */
                    UINT32                            ui4_nw_mask,        /* in */
                    SURF_MODE_T                     e_surf_mode,        /* in */
                    UINT32*                         pui2_channel_id,    /* i/o*/
                    SVL_REC_T*                      pt_svl_rec          /* out*/
                    )
{
    return x_svctx_get_svc_rec_by_channel_id(
                    h_svl,
                    ui4_nw_mask,
                    e_surf_mode,
                    pui2_channel_id,
                    pt_svl_rec
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_set_video_plane
 *
 * Description:
 *          This API is used to set video plane to normal, blank or quat-video.
 *          The details about e_mode should refer to stream manager's
 *          SCC_VID_MODE_T.
 * Inputs:  h_svctx     Contains a handle to a service context.
 *          e_mode      Contains the mode to set the video plane.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_WRONG_STATE The Service Context object is blocked.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_set_video_plane(
                    HANDLE_T                        h_svctx,
                    SCC_VID_MODE_T                  e_mode
                    )
{
    return x_svctx_set_video_plane(
                    h_svctx,
                    e_mode
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_set_session_attr
 *
 * Description: This function is used to set the attribuites of a session
 *
 * Inputs:  h_svctx           Contains a handle to a service context.
 *          e_pipe_type       Contains the pipe type
 *          e_set_type        Contains the type of attributes to set.
 *          pv_set_info       References to the data information of the set
 *                            type.
 *          ui4_set_info_size Contains the size of data information of the
 *                            set type.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_SUPPORT The e_set_type is not supported.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_set_session_attr(
                    HANDLE_T                    h_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    SM_SET_TYPE_T               e_set_type,
                    VOID*                       pv_set_info,
                    SIZE_T                      z_set_info_size
                    )
{
    return x_svctx_set_session_attr(
                    h_svctx,
                    e_pipe_type,
                    e_set_type,
                    pv_set_info,
                    z_set_info_size
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_set_stream_attr
 *
 * Description: This function is used to set the attribuites of a stream
 *
 * Inputs:  h_svctx           Contains a handle to a service context.
 *          e_strm_type       Contains the stream type
 *          e_set_type        Contains the type of attributes to set.
 *          pv_set_info       References to the data information of the set
 *                            type.
 *          ui4_set_info_size Contains the size of data information of the
 *                            set type.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_set_stream_attr(
                    HANDLE_T                    h_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    SM_SET_TYPE_T               e_set_type,
                    VOID*                       pv_set_info,
                    SIZE_T                      z_set_info_size
                    )
{
    return x_svctx_set_stream_attr(
                    h_svctx,
                    e_strm_type,
                    e_set_type,
                    pv_set_info,
                    z_set_info_size
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_get_session_attr
 *
 * Description: This function is used to get the attribuites of a session
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          e_pipe_type        Contains the pipe type to get.
 *          e_get_type         Contains the type of attributes to get.
 *          pv_get_info        References to the data information of the get
 *                             type.
 *          pui4_get_info_size References to the size of data information of
 *                             the get type.
 *
 * Outputs: pv_get_info
 *          pui4_get_info_size
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_get_session_attr(
                    HANDLE_T                    h_svctx,
                    STREAM_TYPE_T               e_pipe_type,
                    SM_GET_TYPE_T               e_get_type,
                    VOID*                       pv_get_info,            /* IN/OUT */
                    SIZE_T*                     pz_get_info_size        /* IN/OUT */
                    )
{
    return x_svctx_get_session_attr(
                    h_svctx,
                    e_pipe_type,
                    e_get_type,
                    pv_get_info,
                    pz_get_info_size
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_get_stream_attr
 *
 * Description: This function is used to get the attribuites of a stream
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          e_strm_type        Contains the stream type to get.
 *          e_get_type         Contains the type of attributes to get.
 *          pv_get_info        References to the data information of the get
 *                             type.
 *          pui4_get_info_size References to the size of data information of
 *                             the get type.
 *
 * Outputs: pv_get_info
 *          pui4_get_info_size
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_get_stream_attr(
                    HANDLE_T                    h_svctx,
                    STREAM_TYPE_T               e_strm_type,
                    SM_GET_TYPE_T               e_get_type,
                    VOID*                       pv_get_info,            /* IN/OUT */
                    SIZE_T*                     pz_get_info_size        /* IN/OUT */
                    )
{
    return x_svctx_get_stream_attr(
                    h_svctx,
                    e_strm_type,
                    e_get_type,
                    pv_get_info,
                    pz_get_info_size
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_get_audio_info
 *
 * Description:
 *          This API is used to get audio information.
 * Inputs:  h_svctx         Contains a handle to a service context.
 *
 * Outputs: pt_resolution   References to the variable containing the
 *                          ASH_AUDIO_INFO_T.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pt_resolution is NULL
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_get_audio_info(
                    HANDLE_T                    h_svctx,
                    ASH_AUDIO_INFO_T*           pt_info
                    )
{
    return x_svctx_get_audio_info(
                    h_svctx,
                    pt_info
                    );
}

/*-----------------------------------------------------------------------------
 * Name: c_svctx_get_video_src_resolution
 *
 * Description:
 *          This API is used to get video source's resolution.
 * Inputs:  h_svctx         Contains a handle to a service context.
 *
 * Outputs: pt_resolution   References to the variable containing the
 *                          VSH_SRC_RESOLUTION_INFO_T.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pt_resolution is NULL
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_get_video_src_resolution(
                    HANDLE_T                        h_svctx,
                    VSH_SRC_RESOLUTION_INFO_T*      pt_resolution
                    )
{
    return x_svctx_get_video_src_resolution(
                    h_svctx,
                    pt_resolution
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_unblock_svc
 *
 * Description: This API is used to unblock a blocked service (all streams are stopped).
 *              If b_force_unblock is TRUE, the blocked service will be unblocked
 *              and streams will be played. If b_force_unblock is FALSE, the client
 *              provided x_svctx_svc_block_check_fct function (if any) will be invoked
 *              again to check the service can be unblocked or not.
 *              If the service presented in the service context is not blocked,
 *              no action is performed
 * Inputs:  h_svctx         Contains a handle to a service context.
 *          b_force_unblock Contains the boolean to force unblock or not
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK               Routine successful.
 *          SVCTXR_ALREADY_STOPPED  The service conetxt is already stopped
 *          SVCTXR_NOT_INIT         The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE       Invalid service context handle.
 *          SVCTXR_FAIL             Failed to send request
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_unblock_svc(
                    HANDLE_T                        h_svctx,
                    BOOL                            b_force_unblock
                    )
{
    return x_svctx_unblock_svc(
                    h_svctx,
                    b_force_unblock
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_set_default_parameters
 *
 * Description: This API is used to set the default parameters for service context
 * Inputs:  h_svctx             Contains a handle to a service context.
 *          ui4_stream_mode     Contains the default desired stream types
 *          e_vid_mode          Contains the mode of the video plane.
 *          b_able_set_pmx_bg   Specifiy to be able to set pmx bg
 *          pf_block_check_fct  Contains the block check fucntion pointer
 *          ps_src_grp_name     Contains the connection source name
 *          ps_snk_grp_name     Contains the sink group name
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_set_default_parameters(
                    HANDLE_T                    h_svctx,
                    UINT32                      ui4_stream_mode,
                    SCC_VID_MODE_T              e_vid_mode,
                    BOOL                        b_able_set_pmx_bg,
                    x_svctx_svc_block_check_fct pf_block_check_fct,
                    const CHAR*                 ps_src_grp_name,
                    const CHAR*                 ps_snk_grp_name,
                    const CHAR*                 ps_snk_grp_name_aux
                    )
{
    return x_svctx_set_default_parameters(
                    h_svctx,
                    ui4_stream_mode,
                    e_vid_mode,
                    b_able_set_pmx_bg,
                    pf_block_check_fct,
                    ps_src_grp_name,
                    ps_snk_grp_name,
                    ps_snk_grp_name_aux
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_set_default_strm_filter
 *
 * Description: This API is used to set stream's default filter function and tag.
 *              When select proper streams to play, if client doesn't provide
 *              filter funtion, this filter will be used.
 *              SCC_VID_MODE_T.
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          e_strm_type        Contains the stream component type for filtering
 *          pf_filter          References to the filter function provided by caller.
 *                             It can't be NULL.
 *          pv_fltr_tag        References to a caller-defined tag. It will pass to
 *                             to caller when the pf_filter is invoked.
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_INV_ARG     pt_filter_fct is NULL
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_set_default_strm_filter(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_strm_type,
                    x_svctx_strm_comp_filter_fct    pt_filter_fct,
                    VOID*                           pv_filter_tag
                    )
{
    return x_svctx_set_default_strm_filter(
                    h_svctx,
                    e_strm_type,
                    pt_filter_fct,
                    pv_filter_tag
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_set_screen_mode
 *
 * Description: This API is used to set screen mode.
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          e_scr_mode         Contains the screen mode
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_FAIL        The operation for accessing device is failed.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_set_screen_mode(
                    HANDLE_T                    h_svctx,
                    SCC_VID_SCREEN_MODE_T       e_scr_mode
                    )
{
    return x_svctx_set_screen_mode(
                    h_svctx,
                    e_scr_mode
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_get_screen_mode
 *
 * Description: This API is used to set screen mode.
 * Inputs:  h_svctx            Contains a handle to a service context.
 *
 * Outputs: pe_scr_mode        References to the variable containing the
 *                             SCC_VID_SCREEN_MODE_T.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_FAIL        The operation for accessing device is failed.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pe_scr_mode is NULL
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_get_screen_mode(
                    HANDLE_T                    h_svctx,
                    SCC_VID_SCREEN_MODE_T*      pe_scr_mode
                    )
{
    return x_svctx_get_screen_mode(
                    h_svctx,
                    pe_scr_mode
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_get_signal_level
 *
 * Description: This API is used to get tuner's signal level.
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          pui1_signal_level  References to the variable containing signal level
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     ui4_delay_in_ms is zero
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_get_signal_level(
                    HANDLE_T                    h_svctx,
                    UINT8*                      pui1_signal_level
                    )
{
    return x_svctx_get_signal_level(
                    h_svctx,
                    pui1_signal_level
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_get_dbm_signal_level
 *
 * Description: This API is used to get tuner's DBM signal level.
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          pui1_signal_level  References to the variable containing signal level
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     ui4_delay_in_ms is zero
 *          SVCTXR_FAIL        cannot get the DBM signal level
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_get_dbm_signal_level(
                    HANDLE_T                    h_svctx,
                    INT16*                      pi2_dbm_signal_level
                    )
{
    return x_svctx_get_dbm_signal_level(
                    h_svctx,
                    pi2_dbm_signal_level
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_get_tuner_status
 *
 * Description: This API is used to get tuner's status.
 * Inputs:  h_svctx            Contains a handle to a service context.
 *
 * Outputs: pui1_tuner_status  References to the variable containing tuner's 
 *                             status.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pi2_dbm_signal_level is NULL
 *          SVCTXR_FAIL        The operation for accessing device is failed.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_get_tuner_status(
                    HANDLE_T                    h_svctx,
                    UINT8*                      pui1_tuner_status
                    )
{
    return x_svctx_get_tuner_status(
                    h_svctx,
                    pui1_tuner_status
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_get_display_resolution
 *
 * Description:
 *          This API is used to get display's resolution.
 * Inputs:  h_svctx         Contains a handle to a service context.
 *
 * Outputs: pt_resolution   References to the variable containing the
 *                          SCC_DISP_RESOLUTION_T.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pt_resolution is NULL
 *          SVCTXR_FAIL        The scc component for display is not opened.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_get_display_resolution(
                    HANDLE_T                    h_svctx,
                    SCC_DISP_RESOLUTION_T*      pt_resolution
                    )
{
    return x_svctx_get_display_resolution(
                    h_svctx,
                    pt_resolution
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_set_display_resolution
 *
 * Description:
 *          This API is used to set display's resolution.
 * Inputs:  h_svctx         Contains a handle to a service context.
 *          pt_resolution   References to the variable containing the
 *                          SCC_DISP_RESOLUTION_T.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pt_resolution is NULL
 *          SVCTXR_FAIL        The scc component for display is not opened.
 *-----------------------------------------------------------------------------*/
 INT32 c_svctx_set_display_resolution(
                    HANDLE_T                    h_svctx,
                    SCC_DISP_RESOLUTION_T*      pt_resolution
                    )
{
    return x_svctx_set_display_resolution(
                    h_svctx,
                    pt_resolution
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_is_signal_loss
 *
 * Description: This API is used to query it is signal loss or not
 * Inputs:  h_svctx         Contains a handle to a service context.
 *          pb_signal_loss  Contains the vairable for signal loss or not.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pb_signal_loss is NULL
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_is_signal_loss(
                    HANDLE_T                    h_svctx,
                    BOOL*                       pb_signal_loss
                    )
{
    return x_svctx_is_signal_loss(
                    h_svctx,
                    pb_signal_loss
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_get_display_aspect_ratio
 *
 * Description:
 *          This API is used to get display's aspect ratio.
 * Inputs:  h_svctx         Contains a handle to a service context.
 *
 * Outputs: pe_aspect_ratio   References to the variable containing the
 *                          SCC_DISP_ASPECT_RATIO_T.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pt_resolution is NULL
 *          SVCTXR_FAIL        The scc component for display is not opened.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_get_display_aspect_ratio(
                    HANDLE_T                    h_svctx,
                    SCC_DISP_ASPECT_RATIO_T*    pe_aspect_ratio
                    )
{
    return x_svctx_get_display_aspect_ratio(
                    h_svctx,
                    pe_aspect_ratio
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_set_display_aspect_ratio
 *
 * Description:
 *          This API is used to set display's aspect ratio.
 * Inputs:  h_svctx         Contains a handle to a service context.
 *          e_aspect_ratio  Contains the value of SCC_DISP_ASPECT_RATIO_T value
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_FAIL        The scc component for display is not opened.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_set_display_aspect_ratio(
                    HANDLE_T                    h_svctx,
                    SCC_DISP_ASPECT_RATIO_T     e_aspect_ratio
                    )
{
    return x_svctx_set_display_aspect_ratio(
                    h_svctx,
                    e_aspect_ratio
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_get_scdb_num_recs
 *
 * Description: This API is used to set the number of records for a specific
 *              stream type. Note: for closed caption, it only return the
 *              amount that detected in the caption_service_descriptor.
 * Inputs:  h_svctx          Contains a handle to a service context.
 *          e_strm_type      Contains the stream type value.
 *
 * Outputs: pui2_num_recs    References to the variable containing the number of
 *                           records
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     ui4_delay_in_ms is zero
 *          SVCTXR_WRONG_STATE If related SCDB is not ready and opened.
 *          SVCTXR_FAIL        SCDB error, cannot get the value.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_get_scdb_num_recs(
                    HANDLE_T                     h_svctx,            /* in  */
                    STREAM_TYPE_T                e_strm_type,        /* in  */
                    UINT16*                      pui2_num_recs       /* in  */
                    )
{
    return x_svctx_get_scdb_num_recs(
                    h_svctx,
                    e_strm_type,
                    pui2_num_recs
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_get_using_strm_data
 *
 * Description: This API retrieves the data of selected stream of a specific type from
 *              a presenting service context object.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          e_strm_type        Contains the stream component type for filtering
 *
 * Outputs: pt_comp_id         References to the structure containing the
 *                             stream component id.
 *          pt_scdb_rec        References to the structure containing the
 *                             stream component data.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_INV_ARG     pt_scdb_rec is NULL.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NOT_EXIST   The stream component is not existed.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_FAIL        SCDB error, can not get the record
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_get_using_strm_data(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_strm_type,        /* in  */
                    STREAM_COMP_ID_T*               pt_comp_id,         /* out */
                    SCDB_REC_T*                     pt_scdb_rec         /* out */
                    )
{
    return x_svctx_get_using_strm_data(
                    h_svctx,
                    e_strm_type,
                    pt_comp_id,
                    pt_scdb_rec
                    );
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_set_listener
 *
 * Description: This API is used to set the listener for monitoring service
 *              context's activities.
 * Inputs:  h_svctx          Contains a handle to a service context.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     ui4_delay_in_ms is zero
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_set_listener(
                    HANDLE_T                        h_svctx,            /* in  */
                    x_svctx_listener_nfy_fct        pf_listener_fct,    /* in  */
                    VOID*                           pv_nfy_tag          /* in  */
                    )
{
    return x_svctx_set_listener(
                    h_svctx,
                    pf_listener_fct,
                    pv_nfy_tag
                    );
}

/*-----------------------------------------------------------------------------
 * Name: x_svctx_set_listener_no_lock
 *
 * Description: This API is used to set the listener for monitoring service
 *              context's activities, without svctx lock.
 * Inputs:  h_svctx          Contains a handle to a service context.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     ui4_delay_in_ms is zero
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_set_listener_no_lock(
                    HANDLE_T                        h_svctx,            /* in  */
                    x_svctx_listener_nfy_fct        pf_listener_fct,    /* in  */
                    VOID*                           pv_nfy_tag          /* in  */
                    )
{
    return x_svctx_set_listener_no_lock(
                    h_svctx,
                    pf_listener_fct,
                    pv_nfy_tag
                    );
}

/*-----------------------------------------------------------------------------
 * Name: c_svctx_surf_svc
 *
 * Description: This API is used to surf service from current service. It only allows
 *              the client already invoking c_svctx_select_svc to obtain the control of
 *              the service context. This API will uses the latest cached service
 *              selection parameters (source information, sink information, notification
 *              callback fucntion, notification tag) to surf service.
 *
 *              Basically, it will find the target service list record based on current
 *              service record id cached in current service context objcet,
 *              e_surf_mode (SURF_MODE_THIS, SURF_MODE_NEXT, and SURF_MODE_PREV),
 *              and ui4_nw_chk_mask and ui4_nw_chk_value.
 *
 *              The possible tagret service list record's ui4_nw_mask will be used to
 *              decide it is target service list record or not. For example,
 *              ui4_nw_chk_mask  = 0x00000007
 *              ui4_nw_chk_value = 0x00000073
 *              The API will find the service list record's ui4_nw_mask & ui4_nw_chk_mask
 *              is equals to ui4_nw_chk_mask & ui4_nw_chk_value,
 *              i.e. the first  bit of service list record's ui4_nw_mask = 1
 *                   the second bit of service list record's ui4_nw_mask = 1
 *                   the third bit of service list record's ui4_nw_mask  = 0
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          ui4_nw_mask        Contains the target svl record's network check mask
 *          ui4_nw_value       Contains the target svl record's network value for
 *                             comparison.
 *          e_surf_mode        Contains the variable for surf service direction
 * Outputs: pt_svl_rec         References to the structure containing the
 *                             target svl record.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     ui4_nw_chk_mask = 0
 *          SVCTXR_NO_RECORD   Cannot find any service list record to surf
 *          SVCTXR_NO_RESOURCE No enough memory to allocate this request.
 *          SVCTXR_FAIL        No any previous service selection request is issued.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_surf_svc(
                    HANDLE_T                        h_svctx,            /* in  */
                    UINT32                          ui4_nw_chk_mask,    /* in  */
                    UINT32                          ui4_nw_chk_value,   /* in  */
                    SURF_MODE_T                     e_surf_mode,        /* in  */
                    SVL_REC_T*                      pt_svl_rec          /* out */
                    )
{
    return x_svctx_surf_svc(
                    h_svctx,
                    ui4_nw_chk_mask,
                    ui4_nw_chk_value,
                    e_surf_mode,
                    pt_svl_rec
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_grab_ownership
 *
 * Description: This API is used to grab the ownership
 * Inputs:  h_svctx          Contains a handle to a service context.
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     ui4_delay_in_ms is zero
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_grab_ownership(
                    HANDLE_T                        h_svctx             /* in  */
                    )
{
    return x_svctx_grab_ownership(
                    h_svctx
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_release_ownership
 *
 * Description: This API is used to release the ownership
 * Inputs:  h_svctx          Contains a handle to a service context.
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     ui4_delay_in_ms is zero
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_release_ownership(
                    HANDLE_T                        h_svctx             /* in  */
                    )
{
    return x_svctx_release_ownership(
                    h_svctx
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_get_cp_info
 *
 * Description: This API is used to get CP(Copy Protection) information for
 *              a specific stream. Normally, it is used when SVCTX_NTFY_CODE_CP
 *              is received.
 * Inputs:  h_svctx          Contains a handle to a service context.
 *          e_strm_type      Contains the stream type value.
 *
 * Outputs: pt_cp_info      References to the variable containing the Copy
 *                          Protection information
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pt_cp_info is NULL
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_get_cp_info(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_strm_type,        /* in  */
                    CP_INFO_LIST_T*                 pt_cp_info          /* out  */
                    )
{
    return x_svctx_get_cp_info(
                    h_svctx,
                    e_strm_type,
                    pt_cp_info
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_get_ca_info
 *
 * Description: This API is used to get CA(Conditional Access) information
 *              for a specific stream. Normally, it is used when
 *              SVCTX_NTFY_CODE_CA is received.
 * Inputs:  h_svctx          Contains a handle to a service context.
 *          e_strm_type      Contains the stream type value.
 *
 * Outputs: pt_ca_info      References to the variable containing the CA information
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_INV_ARG     pt_ca_info is NULL
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_get_ca_info(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_strm_type,        /* in  */
                    SVCTX_CA_INFO_T*                pt_ca_info          /* out  */
                    )
{
    return x_svctx_get_ca_info(
                    h_svctx,
                    e_strm_type,
                    pt_ca_info
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_set
 *
 * Description: This API is used to set the attribuites of a service context
 *
 * Inputs:  h_svctx           References to handle of a service context.
 *          e_set_type        References to the type of attributes to set.
 *          pv_set_info       References to the data information of the set
 *                            type.
 *          ui4_set_info_size Contains the size of data information of the
 *                            set type.
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_set(
                    HANDLE_T                        h_svctx,            /* in  */
                    SVCTX_SET_TYPE_T                e_set_type,         /* in  */
                    VOID*                           pv_set_info,        /* in  */
                    SIZE_T                          z_set_info_size     /* in  */
                    )
{
    return x_svctx_set(
                    h_svctx,
                    e_set_type,
                    pv_set_info,
                    z_set_info_size
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_get
 *
 * Description: This API is used to get the attribuites of a service context.
 *
 * Inputs:  h_svctx            References to handle of a service context.
 *          e_get_type         References to the type of attributes to get.
 *          pv_get_info        References to the data information of the get
 *                             type.
 *          pui4_get_info_size References to the size of data information of
 *                             the get type.
 *
 * Outputs: pv_get_info        References to the data information of the get
 *                             type.
 *          pui4_get_info_size References to the size of data information of
 *                             the get type.
 *
 * Returns: SVCTXR_OK              Success.
 *          SVCTXR_INV_HANDLE      The h_svctx is invalid.
 *          SVCTXR_WRONG_STATE     cannot get relative info when no any runngin request
 *          SVCTXR_NOT_SUPPORT     not support
 *          SVCTXR_FAIL            failed
 ----------------------------------------------------------------------------*/
INT32 c_svctx_get(
                    HANDLE_T                        h_svctx,            /* in  */
                    SVCTX_GET_TYPE_T                e_get_type,         /* in  */
                    VOID*                           pv_get_info,        /* in/out */
                    SIZE_T*                         pz_get_info_size    /* in/out */
                    )
{
    return x_svctx_get(
                    h_svctx,
                    e_get_type,
                    pv_get_info,
                    pz_get_info_size
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_get_current_event_info_len
 *
 * Description:
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 c_svctx_get_current_event_info_len (
                    HANDLE_T                        h_svctx,            /* in  */
                    EVCTX_KEY_TYPE_T                e_key_type,         /* in  */
                    VOID*                           pv_key_info,        /* in  */
                    SIZE_T*                         pz_event_info_len   /* out */
                    )
{
    return x_svctx_get_current_event_info_len (
                    h_svctx,
                    e_key_type,
                    pv_key_info,
                    pz_event_info_len
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_get_current_event_info
 *
 * Description:
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 c_svctx_get_current_event_info (
                    HANDLE_T                        h_svctx,            /* in  */
                    EVCTX_KEY_TYPE_T                e_key_type,         /* in  */
                    VOID*                           pv_key_info,        /* in  */
                    SIZE_T*                         pz_event_info_len,  /* in/out */
                    VOID*                           pv_event_info       /* out */
                    )
{
    return x_svctx_get_current_event_info (
                    h_svctx,
                    e_key_type,
                    pv_key_info,
                    pz_event_info_len,
                    pv_event_info
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_get_device_profile
 * Description This API is used to set AV device profile structure
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 c_svctx_get_device_profile(
                    DEVICE_TYPE_T               e_dev_type,
                    DEVICE_TYPE_PROFILE_T*      pt_profile
                    )
{
    return x_svctx_get_device_profile(
                    e_dev_type,
                    pt_profile
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_freeze
 *
 * Description: This API is used to freeze svctx. The minimum frozen stream
 *              is video stream. If ui4_freeze_strm_mask doesn't include
 *              ST_MASK_VIDEO, this API will add video stream to freeze
 *              automatically.
 *
 * Inputs:  h_svctx                Contains a handle to a service context.
 *          ui4_freeze_strm_mask   Contains the mask to indicate which streams
 *                                 will be frozen
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_freeze(
                    HANDLE_T                        h_svctx,
                    UINT32                          ui4_freeze_strm_mask
                    )
{
    return x_svctx_freeze(
                    h_svctx,
                    ui4_freeze_strm_mask
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_unfreeze
 *
 * Description: This API is used to unfreeze svctx.
 * Inputs:  h_svctx                Contains a handle to a service context
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *-----------------------------------------------------------------------------*/
INT32 c_svctx_unfreeze(
                    HANDLE_T                        h_svctx
                    )
{
    return x_svctx_unfreeze(
                    h_svctx
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_is_frozen
 * Description This API is used to check svctx is frozen or not
 * Inputs:  h_svctx                Contains a handle to a service context
 * Outputs: pb_result              Contains a flag for the result
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 c_svctx_is_frozen(
                    HANDLE_T                        h_svctx,
                    BOOL*                           pb_result
                    )
{
    return x_svctx_is_frozen(
                    h_svctx,
                    pb_result
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_get_well_defined_video_heigh
 * Description This API is used to get well-defined video height
 * Inputs:  ui4_raw_height         Contains the raw video height, e.g. 1088
 * Outputs: pui4_defined_height    Contains the well-defined video height. e.g. 1080
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 c_svctx_get_well_defined_video_heigh(
                    UINT32                          ui4_raw_height,
                    UINT32*                         pui4_defined_height
                    )
{
    return x_svctx_get_well_defined_video_heigh(
                    ui4_raw_height,
                    pui4_defined_height
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_get_cc_capability
 * Description This API is used to get cc capability of a svctx
 * Inputs:  h_svctx                References to handle of a service context.
 * Outputs: pt_capablility         Contains the cc capable condition
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 c_svctx_get_cc_capability(
                    HANDLE_T                    h_svctx,
                    SVCTX_CAPABILITY_T*         pt_capablility
                    )
{
    return x_svctx_get_cc_capability(
                    h_svctx,
                    pt_capablility
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_get_subtitle_capability
 * Description This API is used to get subtitle capability of a svctx
 * Inputs:  h_svctx                References to handle of a service context.
 * Outputs: pt_capablility         Contains the cc capable condition
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 c_svctx_get_subtitle_capability(
                    HANDLE_T                    h_svctx,
                    SVCTX_CAPABILITY_T*         pt_capablility
                    )
{
    return x_svctx_get_subtitle_capability(
                    h_svctx,
                    pt_capablility
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_get_teletext_capability
 * Description This API is used to get teletext capability of a svctx
 * Inputs:  h_svctx                References to handle of a service context.
 * Outputs: pt_capablility         Contains the cc capable condition
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 c_svctx_get_teletext_capability(
                    HANDLE_T                    h_svctx,
                    SVCTX_CAPABILITY_T*         pt_capablility
                    )
{
    return x_svctx_get_teletext_capability(
                    h_svctx,
                    pt_capablility
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_get_event_capability
 * Description This API is used to get event capability of a svctx
 * Inputs:  h_svctx                References to handle of a service context.
 *          ui4_event_key_mask     = MAKE_BIT_MASK_32(EVCTX_KEY_TYPE_T)
 * Outputs: pt_capablility         Contains the cc capable condition
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 c_svctx_get_event_capability(
                    HANDLE_T                    h_svctx,
                    UINT32                      ui4_event_key_mask,
                    SVCTX_CAPABILITY_T*         pt_capablility
                    )
{
    return x_svctx_get_event_capability(
                    h_svctx,
                    ui4_event_key_mask,
                    pt_capablility
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_open_scdb
 * Description This API returns a handle to the SCDB object for a specified svctx.
 *             Basically, it is a help function to let caller to handle
 *             the SCDB by itself, but the return value  is SVCTXR_XXX
 *              not SMR_XXX
 * Inputs:  h_svctx        Specifies the svctx handle that was
 *                         used to create the SCDB object.
 *          e_pipe_type    Additional info for specifing pipe. it could be ST_VIDEO,
 *                         ST_AUDIO, or ST_UNKNOWN
 *          pv_tag         Private tag value associated with the
 *                         calling client.
 *          pf_nfy         Client notification function.
 * Outputs: ph_scdb        pointer to a SCDB handle
 * Returns: SVCTXR_OK           (SCDBR_OK)               A new handle is created
 *          SVCTXR_NO_RESOURCE  (SCDBR_OUT_OF_HANDLE)    No more handle available,
 *                              (SCDBR_OUT_OF_MEM)       or Out of memory.
 *          SVCTXR_INV_ARG      (SCDBR_INV_ARG)          ph_scdb is NULL.
 *          SVCTXR_NOT_EXIST    (SCDBR_NOT_FOUND)        Specified database object is not found.
 *          SVCTXR_INV_HANDLE                            bad 'h_svctx' handle.
 *          SVCTXR_WRONG_STATE                           connection is not available to create SCDB
 *          SVCTXR_FAIL                                  failed
 *---------------------------------------------------------------------------*/
INT32 c_svctx_open_scdb(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_pipe_type,
                    HANDLE_T*                       ph_scdb,
                    VOID*                           pv_tag,
                    x_scdb_nfy_fct                  pf_nfy
                    )
{
    return x_svctx_open_scdb(
                    h_svctx,
                    e_pipe_type,
                    ph_scdb,
                    pv_tag,
                    pf_nfy
                    );
}
/*-----------------------------------------------------------------------------
 * Name:  c_svctx_close_scdb
 *
 * Description: This API frees the specified handle to the SCDB database.
 *              Basically, it is a help function to let caller to handle
 *              the SCDB by itself, but the return value is SVCTXR_XXX
 *              not SMR_XXX
 * Inputs:  h_scdb:  handle to be freed.
 *
 * Outputs: None.
 *
 * Returns: SVCTXR_OK           (SCDBR_OK)            SCDB handle and its resource are free.
 *          SVCTXR_INV_HANDLE   (SCDBR_INV_HANDLE)    Bad SCDB handle.
 ----------------------------------------------------------------------------*/
INT32 c_svctx_close_scdb(
                    HANDLE_T                        h_scdb
                    )
{
    return x_svctx_close_scdb(h_scdb);
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_open_stream
 *
 * Description: This function is used to open a stream in a specific svctx.
 *              Basically, it is a help function to let caller to handle
 *              the stream by itself, but the return value  is SVCTXR_XXX
 *              not SMR_XXX
 * Inputs:  h_svctx         References to the svctx.
 *          e_pipe_type     Additional info for specifing pipe. it could be ST_VIDEO,
 *                          ST_AUDIO, or ST_UNKNOWN
 *          pt_strm_comp_id References to the component ID aquired from SCDB.
 *          pt_cmd          References to the command sets to initialize the
 *                          stream.
 *          pf_strm_nfy     References to a stream event callback function,
 *                          which is implemented by the caller.
 *          pv_strm_nfy_tag References to a tag to be pass to the stream event
 *                          callback function.
 *          b_auto_play     Indicate if the stream sould play automatically.
 *
 * Outputs: ph_stream       References to the stream handle created by Stream
 *                          Manager.
 *
 * Returns: SVCTXR_OK          (SMR_OK)             Success.
 *          SVCTXR_NOT_SUPPORT (SMR_NOT_SUPPORT)    There is no stream handler
 *                                                  that support this
 *                                                  type of stream.
 *          SVCTXR_INV_ARG     (SMR_INV_ARG)        The argument ph_stream,
 *                                                  pf_strm_nfy,or
 *                                                  pt_strm_comp_id is invalid.
 *          SVCTXR_INV_HANDLE                       The h_svctx is invalid.
 *          SVCTXR_WRONG_STATE                      session is not available
 *          SVCTXR_FAIL                             failed
  *---------------------------------------------------------------------------*/
INT32 c_svctx_open_stream(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_pipe_type,
                    const STREAM_COMP_ID_T*         pt_strm_comp_id,
                    const SM_COMMAND_T*             pt_cmd_sets,
                    x_stream_nfy_fct                pf_strm_nfy,
                    VOID*                           pv_strm_nfy_tag,
                    BOOL                            b_auto_play,
                    HANDLE_T*                       ph_stream
                    )
{
    return x_svctx_open_stream(
                    h_svctx,
                    e_pipe_type,
                    pt_strm_comp_id,
                    pt_cmd_sets,
                    pf_strm_nfy,
                    pv_strm_nfy_tag,
                    b_auto_play,
                    ph_stream
                    );
}
/*-----------------------------------------------------------------------------
 * Name: c_svctx_open_stream_ex
 *
 * Description: This function is used to open a stream in a specific svctx.
 *              Basically, it is a help function to let caller to handle
 *              the stream by itself, but the return value  is SVCTXR_XXX
 *              not SMR_XXX
 * Inputs:  h_svctx         References to the svctx.
 *          e_pipe_type     Additional info for specifing pipe. it could be ST_VIDEO,
 *                          ST_AUDIO, or ST_UNKNOWN
 *          pt_strm_comp_id References to the component ID aquired from SCDB.
 *          pt_cmd          References to the command sets to initialize the
 *                          stream.
 *          pf_strm_nfy     References to a stream event callback function,
 *                          which is implemented by the caller.
 *          pv_strm_nfy_tag References to a tag to be pass to the stream event
 *                          callback function.
 *          b_auto_play     Indicate if the stream sould play automatically.
 *          ui4_serial_num  Contain a serial number indicating the current 
 *                          open stream operation.
 *
 * Outputs: ph_stream       References to the stream handle created by Stream
 *                          Manager.
 *
 * Returns: SVCTXR_OK          (SMR_OK)             Success.
 *          SVCTXR_NOT_SUPPORT (SMR_NOT_SUPPORT)    There is no stream handler
 *                                                  that support this
 *                                                  type of stream.
 *          SVCTXR_INV_ARG     (SMR_INV_ARG)        The argument ph_stream,
 *                                                  pf_strm_nfy,or
 *                                                  pt_strm_comp_id is invalid.
 *          SVCTXR_INV_HANDLE                       The h_svctx is invalid.
 *          SVCTXR_WRONG_STATE                      session is not available
 *          SVCTXR_FAIL                             failed
  *---------------------------------------------------------------------------*/
INT32 c_svctx_open_stream_ex(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_pipe_type,
                    const STREAM_COMP_ID_T*         pt_strm_comp_id,
                    const SM_COMMAND_T*             pt_cmd_sets,
                    x_stream_nfy_fct                pf_strm_nfy,
                    VOID*                           pv_strm_nfy_tag,
                    BOOL                            b_auto_play,
                    UINT32                          ui4_serial_num,
                    HANDLE_T*                       ph_stream
                    )
{
    return x_svctx_open_stream_ex(
                    h_svctx,
                    e_pipe_type,
                    pt_strm_comp_id,
                    pt_cmd_sets,
                    pf_strm_nfy,
                    pv_strm_nfy_tag,
                    b_auto_play,
                    ui4_serial_num,
                    ph_stream
                    );    
}                    
/*-----------------------------------------------------------------------------
 * Name: c_svctx_close_stream
 *
 * Description: This function is used to close a stream of Stream Manager.
 *              Basically, it is a help function to let caller to handle
 *              the stream by itself, but the return value  is SVCTXR_XXX
 *              not SMR_XXX
 * Inputs:  h_stream References to the handle of the stream.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          (SMR_OK)             Success.
 *          SVCTXR_INV_HANDLE  (SMR_INV_HANDLE)     The h_stream is invalid.
 *          SVCTXR_FAIL        (SMR_INTERNAL_ERROR) A serious internal error.
 ----------------------------------------------------------------------------*/
INT32 c_svctx_close_stream(
                    HANDLE_T                        h_stream
                    )
{
    return x_svctx_close_stream(h_stream);
}
/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_get_conn_attr
 * Description This API gets information about a connection.
 * Inputs:  h_svctx        Specifies the svctx handle that was
 *                         used to create the SCDB object.
 *          e_pipe_type    Additional info for specifing pipe. it could be ST_VIDEO,
 *                         ST_AUDIO, or ST_UNKNOWN
 *          e_ctrl         Contains the control code, and it is defined in CM's header file
 * Outputs: pv_ctrl_data   Contains information about the connection.
 * Returns: SVCTXR_OK            successfully
 *          SVCTXR_NO_RESOURCE   No more handle available,
 *                               or Out of memory.
 *          SVCTXR_INV_HANDLE    bad 'h_svctx' handle.
 *          SVCTXR_WRONG_STATE   connection is not available
 *          SVCTXR_FAIL          failed
 *---------------------------------------------------------------------------*/
INT32 c_svctx_get_conn_attr(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_pipe_type,
                    CM_CTRL_TYPE_T                  e_ctrl,
                    VOID*                           pv_ctrl_data
                    )
{
    return x_svctx_get_conn_attr(
                    h_svctx,
                    e_pipe_type,
                    e_ctrl,
                    pv_ctrl_data
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_set_conn_attr
 * Description This API sets some attributes of a connection.
 * Inputs:  h_svctx        Specifies the svctx handle that was
 *                         used to create the SCDB object.
 *          e_pipe_type    Additional info for specifing pipe. it could be ST_VIDEO,
 *                         ST_AUDIO, or ST_UNKNOWN
 *          e_ctrl         Contains the control code, and it is defined in CM's header file
 *          pv_ctrl_data   References a data structure which content depends on
 *                         e_ctrl.
 *
 * Outputs: pv_ctrl_data   Contains values that may have been modified.
 * Returns: SVCTXR_OK            successfully
 *          SVCTXR_NO_RESOURCE   No more handle available,
 *                               or Out of memory.
 *          SVCTXR_INV_HANDLE    bad 'h_svctx' handle.
 *          SVCTXR_WRONG_STATE   connection is not available
 *          SVCTXR_FAIL          failed
 *---------------------------------------------------------------------------*/
INT32 c_svctx_set_conn_attr(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_pipe_type,
                    CM_CTRL_TYPE_T                  e_ctrl,
                    VOID*                           pv_ctrl_data
                    )
{
    return x_svctx_set_conn_attr(
                    h_svctx,
                    e_pipe_type,
                    e_ctrl,
                    pv_ctrl_data
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_set_analog_tuner_out_attr
 * Description This API sets analog tuner out attributes
 * Inputs:  pt_tuner_out   References a data structure which content tuner out
 *                         parameters.
 *
 * Outputs: -
 * Returns: SVCTXR_OK            successfully
 *          SVCTXR_INV_ARG       pt_tuner_out is NULL.
 *          SVCTXR_FAIL          failed
 *---------------------------------------------------------------------------*/
extern INT32 c_svctx_set_analog_tuner_out_attr(
                    TUNER_OUT_DESC_T*               pt_tuner_out
                    )
{
    return x_svctx_set_analog_tuner_out_attr(
                    pt_tuner_out
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_svc_block_check
 * Description 
 *      This API is used to perfrom service block check, however, this
 *      API is just a check, it doesn't perfrom anu service block/unblock
 *      operation.
 * Inputs:  
 *      h_svctx           Contains a handle to a service context.
 *      e_check_guide     Contains the block check guide          
 *      ps_src_name       Contains the source group name
 *      pt_svl_rec        Contains the SVL record if source is TV
 *      pv_evn_rating     Contains the event's rating buffer obtained from EVctx
 * Outputs: -
 * Returns: 
 *      SVC_BLOCK_COND_T        the check result
 *---------------------------------------------------------------------------*/
SVC_BLOCK_COND_T c_svctx_svc_block_check(
                    HANDLE_T                        h_svctx,
                    SVC_BLOCK_CHECK_GUDIE_T         e_check_guide, 
                    const CHAR*                     ps_src_name,   
                    const SVL_REC_T*                pt_svl_rec,  
                    const VOID*                     pv_evn_rating
                    )
{
    return x_svctx_svc_block_check(
                    h_svctx,
                    e_check_guide, 
                    ps_src_name,   
                    pt_svl_rec,  
                    pv_evn_rating
                    );
}

INT32 c_svctx_set_ad_state(
                    HANDLE_T                        h_svctx,          /* in  */
                    BOOL                            b_enable          /* in  */
                    )
{
    return (x_svctx_set_ad_state(h_svctx, b_enable));
}

/*-----------------------------------------------------------------------------
 * Name: c_svctx_set_ad_state_ex 
 *
 * Description: This API tries to open an ad stream specified by *pt_scdb_rec.
 *
 * Inputs:  h_svctx            Contains a handle to a service context.
 *          pt_scdb_rec        References to the structure containing the
 *                               scdb data of the ad stream.
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_INV_ARG     pt_scdb_rec is NULL or the ui2_num_recs is zero.
 *          SVCTXR_NOT_SUPPORT The stream type is not supported in this service
 *                             context object.
 *          SVCTXR_NOT_EXIST   The stream component is not existed.
 *          SVCTXR_NOT_INIT    The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE  Invalid service context handle.
 *          SVCTXR_FAIL        SCDB error, can not get the record
 *          SVCTXR_NOT_SUPPORT The operation is not supported in current
 *                               scenaro.
 *-----------------------------------------------------------------------------*/

INT32 c_svctx_set_ad_state_ex(
                    HANDLE_T                        h_svctx,          /* in  */
                    const SCDB_REC_T*               pt_scdb_rec       /* in  */
                    )
{
    return (x_svctx_set_ad_state_ex(h_svctx, pt_scdb_rec));
}


INT32 c_svctx_get_ad_state(
                    HANDLE_T                        h_svctx,          /* in  */
                    BOOL*                           pb_enable         /* out */
                    )
{
    return (x_svctx_get_ad_state(h_svctx, pb_enable));
}

#ifdef MW_TV_AV_BYPASS_SUPPORT
INT32 c_svctx_create_associate(
                    const CHAR*                     ps_name,
                    const SVCTX_ASSC_CFG_T*         pt_assc_cfg,
                    HANDLE_T*                       ph_svctx)
{
    HANDLE_T  h_aux;
    INT32     i4_ret;

    i4_ret = aee_grab_handle_resource(
                    AEE_FLAG,
                    1,
                    &h_aux
                    );

    if (i4_ret == AEER_OK) {
        i4_ret = x_svctx_create_associate(ps_name, pt_assc_cfg, ph_svctx);
        
        if (i4_ret == SVCTXR_OK){
            handle_link_to_aux(h_aux, *ph_svctx);
        } else {
            aee_release_handle_resource(1);
        }
    } else if (i4_ret == AEER_OUT_OF_RESOURCES) {
        i4_ret = SVCTXR_NO_RESOURCE;
    } else {
        i4_ret = SVCTXR_FAIL;
    }

    return i4_ret;
}

#ifdef MW_PIP_POP_SUPPORT
INT32 c_svctx_add_associate(
    HANDLE_T    h_svctx,
    HANDLE_T    h_svctx_assc)
{
    return (x_svctx_add_associate(h_svctx, h_svctx_assc));
}

INT32 c_svctx_add_exclusion(
    HANDLE_T    h_svctx,
    HANDLE_T    h_svctx_excl,
    UINT32      ui4_excl_src_mask)
{
    return (x_svctx_add_exclusion(h_svctx, h_svctx_excl, ui4_excl_src_mask));
}

INT32 c_svctx_start_exclusion(
    HANDLE_T        h_svctx,
    SRC_TYPE_T      e_src_type)
{
    return (x_svctx_start_exclusion(h_svctx, e_src_type));
}

INT32 c_svctx_stop_exclusion(
    HANDLE_T        h_svctx,
    SRC_TYPE_T      e_src_type)
{
    return (x_svctx_stop_exclusion(h_svctx, e_src_type));
}
#endif
#endif

#ifdef SYS_RECORD_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: c_svctx_rec_create
 *
 * Description: This API creates a Recrod SVCTX.
 *              
 * Inputs:  ps_svctx_name  Specifies the service context name for the create.
 *                         The name must be unique to other SVCTX.
 *          
 *          pt_rec_attr    Contains the attributes for Record SVCTX.
 *
 * Outputs: ph_svctx_rec   Contains the Record SVCTX handle.
 *
 * Returns: SVCTXR_OK           Routine successful.
 *          SVCTXR_INV_ARG      Invalid argument.
 *          SVCTXR_NO_RESOURCE  Not enough memory to open the service context.
 *          SVCTXR_NOT_INIT     The Service Context Module is not initialized yet.
 *          SVCTXR_FAIL         The creation is fail.
 *          < 0                 Other errors.
 ----------------------------------------------------------------------------*/
INT32 c_svctx_rec_create (
    const CHAR*                     ps_svctx_name,    
    const SVCTX_REC_ATTR_T*         pt_rec_attr,      
    HANDLE_T*                       ph_svctx_rec)
{
    HANDLE_T  h_aux;
    INT32     i4_ret;
    
    i4_ret = aee_grab_handle_resource (AEE_FLAG,
                                       1,
                                       & h_aux );
    
    switch (i4_ret)
    {
    case AEER_OK:
        {
            i4_ret = x_svctx_rec_create (ps_svctx_name,
                                         pt_rec_attr,
                                         ph_svctx_rec);
            if (i4_ret == SVCTXR_OK)
            {
                handle_link_to_aux (h_aux, *ph_svctx_rec);
            }
            else
            {
                aee_release_handle_resource(1);
            }
        }
        break;
        
    case AEER_OUT_OF_RESOURCES:
        {
            i4_ret = SVCTXR_NO_RESOURCE;
        }
        break; 
       
    default:
        {
            i4_ret = SVCTXR_FAIL;
        }
        break;
    }
    
    return i4_ret;
}
    
 /*-----------------------------------------------------------------------------
 * Name: c_svctx_rec_start
 *
 * Description: This API starts the recording session.
 *              
 * Inputs:  h_svctx_rec  Specifies the Record SVCTX handle.
 *          
 *          pt_config    Contains the config for recording.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK           Routine successful.
 *          SVCTXR_NOT_INIT     The Service Context Module is not initialized yet.
 *          SVCTXR_INV_ARG      Invalid argument.
 *          SVCTXR_INV_HANDLE   Invalid service context handle.
 *          SVCTXR_WRONG_STATE  Invalid service context state to start recording.
 *          SVCTXR_NO_RESOURCE  Not enough memory / resource.
 *          < 0                 Other errors. 
 ----------------------------------------------------------------------------*/
INT32 c_svctx_rec_start (
    HANDLE_T                        h_svctx_rec, 
    const SVCTX_REC_CONFIG_T*       pt_config)
{
    return x_svctx_rec_start (h_svctx_rec, pt_config);
}
    
 /*-----------------------------------------------------------------------------
 * Name: c_svctx_rec_stop
 *
 * Description: This API stops the recording session. The 
 *              
 * Inputs:  h_svctx_rec  Specifies the Record SVCTX handle.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK           Routine successful.
 *          SVCTXR_NOT_INIT     The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE   Invalid service context handle.
 *          SVCTXR_WRONG_STATE  Invalid service context state to stop recording.
 *          < 0                 Other errors. 
 ----------------------------------------------------------------------------*/
INT32 c_svctx_rec_stop (
    HANDLE_T                        h_svctx_rec)
{
    return x_svctx_rec_stop (h_svctx_rec);
}
    
 /*-----------------------------------------------------------------------------
 * Name: c_svctx_rec_pause
 *
 * Description: This API pauses the recording session.
 *              
 * Inputs:  h_svctx_rec  Specifies the Record SVCTX handle.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK           Routine successful.
 *          SVCTXR_NOT_INIT     The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE   Invalid service context handle.
 *          SVCTXR_WRONG_STATE  Invalid service context state to pause recording.
 *          < 0                 Other errors. 
 ----------------------------------------------------------------------------*/
INT32 c_svctx_rec_pause (
    HANDLE_T                        h_svctx_rec)
{
    return x_svctx_rec_pause (h_svctx_rec);
}
    
/*-----------------------------------------------------------------------------
 * Name: c_svctx_rec_resume
 *
 * Description: This API resumes the recording session.
 *              
 * Inputs:  h_svctx_rec  Specifies the Record SVCTX handle.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK           Routine successful.
 *          SVCTXR_NOT_INIT     The Service Context Module is not initialized yet.
 *          SVCTXR_INV_HANDLE   Invalid service context handle.
 *          SVCTXR_WRONG_STATE  Invalid service context state to pause recording.
 *          < 0                 Other errors. 
 ----------------------------------------------------------------------------*/
INT32 c_svctx_rec_resume (
    HANDLE_T                        h_svctx_rec)
{
    return x_svctx_rec_resume (h_svctx_rec);
}

#endif

#ifdef GINGA_SUPPORT
/*------------------------------------------------------------------*/
/*! @brief  This API to register the pre-select stream.
 *  Inputs: pf_pre_select_stream_fct - Specifies pre-select stream function.
 *  Returns:
 *          SVCTXR_OK - Routine successful.
 *          SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *          SVCTXR_INV_ARG  - pf_pre_select_stream_fct is NULL. 
 ------------------------------------------------------------------*/
INT32 c_svctx_reg_pre_select_stream_cb(
                   x_svctx_pre_select_stream_nfy_fct pf_pre_select_stream_fct /* in*/
                   )
{
	  return x_svctx_reg_pre_select_stream_cb(pf_pre_select_stream_fct);
}

#endif
