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
 * $RCSfile: x_svctx.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/6 $
 * $SWAuthor: Weider Chang $
 * $MD5HEX: 29fe902dc7b0175781aa7c8566cead6f $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#ifndef _X_SVCTX_H_
#define _X_SVCTX_H_

/*-----------------------------------------------------------------------------
 *  include files
 *---------------------------------------------------------------------------*/
#include "u_cm.h"
#include "u_brdcst.h"
#include "u_sm.h"
#include "u_sm_video_hdlr.h"
#include "u_sm_audio_hdlr.h"
#include "u_scc.h"
#include "u_svctx.h"
#include "u_svl.h"
#include "u_device_type.h"
#include "x_mm_common.h"
#include "x_rm_dev_types.h"
#ifdef SYS_RECORD_SUPPORT
#include "u_rec_hdlr.h"
#endif

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_SVCTX
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
typedef struct _AVC_TYPE_MAPPING
{
    DRV_TYPE_T              e_drv_type;
    DEVICE_TYPE_PROFILE_T   t_profile;
} AVC_TYPE_MAPPING;

#define ORDER_UNKNOWN                   (-1)
/*-----------------------------------------------------------------------------
 * functions declarations
 *---------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @brief  This API open a new service context from the service context
 *              pool or attaches to an existing service context if the name
 *              is existed. 
 *  @param  [in]  ps_svctx_name - References the service context name for the open.
 *  @param  [out] ph_svctx - Contains the service context handle.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_INV_ARG - Invalid argument.
 *  @retval SVCTXR_NO_RESOURCE - Not enough memory to open the service context.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - The Handle link operation is failed.
 */
/*------------------------------------------------------------------*/ 
extern INT32 x_svctx_open(
                    const CHAR*                     ps_svctx_name,    /* in  */
                    HANDLE_T*                       ph_svctx          /* out */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API performs a close. If the handle is the last client's
 *              handle, this service context instance will return to the service
 *              context pool.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Failed to close the service context.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_close(
                    HANDLE_T                        h_svctx           /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  Selects a service to be presented in this service context
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
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  ui4_stream_mode - References the desired stream types for
 *                             the select operation. Client should use the
 *                             macros ST_MASK_AUDIO, ST_MASK_VIDEO,
 *                             ST_MASK_CLOSED_CAPTION and so on (defined in
 *                             u_common.h) to OR them for this input argument.
 *  @param  [in]  pt_src_desc - References the source description.
 *  @param  [in]  pt_snk_desc - References the sink description.
 *  @param  [in]  pf_nfy - References the service notify function. 
 *  @param  [in]  pv_nfy_tag - References the tag passed to the notify function. 
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - Invalid argument.
 *  @retval SVCTXR_NO_RESOURCE - Not enough memory to allocate this request.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Failed to send request.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_select_svc(
                    HANDLE_T                        h_svctx,          /* in  */
                    UINT32                          ui4_stream_mode,  /* in  */
                    const SRC_DESC_T*               pt_src_desc,      /* in  */
                    const SNK_DESC_T*               pt_snk_desc,      /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API causes the service context instance to stop presenting
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
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Failed to send request.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_stop_svc(
                    HANDLE_T                        h_svctx           /* in  */
                    );

extern INT32 x_svctx_stop_svc_ex(
                    HANDLE_T                        h_svctx,          /* in  */
                    x_svctx_stop_nfy_fct            pf_nfy,           /* in  */
                    VOID*                           pv_tag            /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to check the service context instance is in
 *              running or not
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Failed to send request.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_is_running (
                    HANDLE_T                        h_svctx,            /* in */
                    BOOL*                           pb_result           /* out*/
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API causes the service context instance to stop presenting
 *              content and enter the READY state synchronously.
 *              Connection, SCDB, Session,
 *              Streams used in the presention will be released. The client
 *              that issues the service selection request will be notified
 *              via its pf_nfy (set in x_svctx_select_svc()) with
 *              (SVCTX_COND_READY, SVCTX_NTFY_CODE_USER_STOP).
 *              This API completes asynchronously. No action is performed if the
 *              service context instance is already in the READY or POOLED state.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_sync_stop_svc(
                    HANDLE_T                        h_svctx,
                    UINT32                          ui4_time
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to set the audio volume.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  pt_volume_info - References to the structure containing the
 *                             scc audio volume information data.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_ARG - pt_volume_info is NULL or
 *                             out port is SCC_AUD_OUT_PORT_2_CH but e_ch is not
 *                             one of SCC_AUD_CHANNEL_FRONT_LEFT,
 *                             SCC_AUD_CHANNEL_FRONT_RIGHT, and
 *                             SCC_AUD_CHANNEL_ALL.
 *  @retval SVCTXR_FAIL - The operation for accessing device is failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_set_volume(
                    HANDLE_T                        h_svctx,          /* in  */
                    const SCC_AUD_VOLUME_INFO_T*    pt_volume_info    /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to get the audio volume.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [out] pt_volume_info - References to the structure containing the
 *                             scc audio volume information data.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_ARG - pt_volume_info is NULL
 *  @retval SVCTXR_FAIL - The operation for accessing device is failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_volume(
                    HANDLE_T                        h_svctx,          /* in  */
                    SCC_AUD_VOLUME_INFO_T*          pt_volume_info    /* out */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to set the audio to mute or unmute.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  b_is_mute - Contains a boolean value for setting audio
 *                             to mute or unmute.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - The operation for accessing device is failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_set_mute(
                    HANDLE_T                        h_svctx,          /* in  */
                    BOOL                            b_is_mute         /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to check the audio is mute or unmute.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [out] pb_is_mute - References to a boolean value for retrieving audio
 *                             is mute or unmute.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_INV_ARG - pb_is_mute is NULL
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - The operation for accessing device is failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_mute(
                    HANDLE_T                        h_svctx,          /* in  */
                    BOOL*                           pb_is_mute        /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API retrieves the stream id and stream data from
 *              stream component database by a specific type and a filter
 *              function provided by caller. If the data is not populated into
 *              stream component database, caller will get a SVCTXR_WRONG_STATE
 *              return value.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_strm_type - Contains the stream component type for filtering.
 *  @param  [in]  pf_filter - References to the filter function provided by caller.
 *                             It can't be NULL.
 *  @param  [in]  pv_fltr_tag - References to a caller-defined tag. It will pass to
 *                             to caller when the pf_filter is invoked.
 *  @param  [out] pt_comp_id - References to the structure containing the
 *                             stream component id. It can't be NULL.
 *  @param  [out] pt_scdb_rec - References to the structure containing the
 *                             stream component data.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - pf_filter or pt_comp_id is NULL.
 *  @retval SVCTXR_WRONG_STATE - If related SCDB is not ready and opened.
 *  @retval SVCTXR_NOT_EXIST - The stream component is not existed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_stream_comp_id(
                    HANDLE_T                        h_svctx,          /* in  */
                    STREAM_TYPE_T                   e_strm_type,      /* in  */
                    x_svctx_strm_comp_filter_fct    pf_filter,        /* in  */
                    VOID*                           pv_fltr_tag,      /* in  */
                    STREAM_COMP_ID_T*               pt_comp_id,       /* out */
                    SCDB_REC_T*                     pt_scdb_rec       /* out */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API retrieves the selected stream of a specific type from
 *              a presenting service context object.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_strm_type - Contains the stream component type for filtering.
 *  @param  [out] pt_scdb_rec - Contains the stream component type for filtering.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_INV_ARG - pt_scdb_rec is NULL.
 *  @retval SVCTXR_NOT_SUPPORT - The stream type is not supported in this service
 *                             context object.
 *  @retval SVCTXR_NOT_EXIST - The stream component is not existed.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_using_scdb_rec(
                    HANDLE_T                        h_svctx,          /* in  */
                    STREAM_TYPE_T                   e_strm_type,      /* in  */
                    SCDB_REC_T*                     pt_scdb_rec       /* out */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API retrieves the strm_req_type of a presenting stream.
 *
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_strm_type - Contains the stream component type for filtering.
 *  @param  [out] pe_strm_req_type  - Contains the stream req type.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_INV_ARG - pt_scdb_rec is NULL.
 *  @retval SVCTXR_NOT_SUPPORT - The stream type is not supported in this service
 *                             context object.
 *  @retval SVCTXR_NOT_EXIST - The stream component is not existed.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_strm_req_type(
                    HANDLE_T                        h_svctx,          /* in  */
                    STREAM_TYPE_T                   e_strm_type,      /* in  */
                    STRM_REQ_TYPE_T*                pe_strm_req_type  /* out */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This APIis used to stop a specific stream via stream type
 *              in asynchronous way.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [out] e_strm_type - Contains the stream type for stopping.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_NOT_SUPPORT - The stream type is not supported in this service
 *                             context object.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_select_stream(
                    HANDLE_T                        h_svctx,          /* in  */
                    const STREAM_COMP_ID_T*         pt_comp_id,       /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to selects a specific stream via PID
 *              in asynchronous way.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_strm_type - Contains the stream type for selecting.
 *  @param  [in]  t_mpeg_2_pid - Contains the MPEG2 PID value.
 *  @param  [in]  pf_nfy - References the stream notify function.
 *  @param  [in]  pv_nfy_tag - References the tag passed to the notify function.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_NOT_SUPPORT - The stream type is not supported in this service
 *                             context object.
 *  @retval SVCTXR_NO_RESOURCE - Not enough memory to allocate this request.
 *  @retval SVCTXR_INV_ARG - pt_comp_id is NULL.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_NOT_EXIST - The related SCDB record is not existed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_select_mpeg_stream_by_pid(
                    HANDLE_T                        h_svctx,          /* in  */
                    STREAM_TYPE_T                   e_strm_type,      /* in  */
                    MPEG_2_PID_T                    t_mpeg_2_pid,     /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to selects a specific stream via PID
 *              in asynchronous way.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  pt_stream_info - References the stream info data structure.
 *  @param  [in]  pf_nfy - References the stream notify function.
 *  @param  [in]  pv_nfy_tag - References the tag passed to the notify function.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_NOT_SUPPORT - The stream type is not supported in this service
 *                             context object.
 *  @retval SVCTXR_NO_RESOURCE - Not enough memory to allocate this request.
 *  @retval SVCTXR_INV_ARG - pt_comp_id is NULL.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_NOT_EXIST - The related SCDB record is not existed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_select_mpeg_stream_by_pid_ex(
                    HANDLE_T                        h_svctx,          /* in  */
                    SVCTX_STREAM_INFO_T*            pt_stream_info,   /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to selects a specific AD stream via PID
 *              in asynchronous way.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  pt_stream_info - References the stream info data structure.
 *  @param  [in]  pf_nfy - References the stream notify function.
 *  @param  [in]  pv_nfy_tag - References the tag passed to the notify function.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_NOT_SUPPORT - The stream type is not supported in this service
 *                             context object.
 *  @retval SVCTXR_NO_RESOURCE - Not enough memory to allocate this request.
 *  @retval SVCTXR_INV_ARG - pt_comp_id is NULL.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_NOT_EXIST - The related SCDB record is not existed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_select_mpeg_ad_stream_by_pid(
                    HANDLE_T                        h_svctx,
                    SVCTX_STREAM_INFO_T*            pt_stream_info,
                    x_svctx_select_nfy_fct          pf_nfy,
                    VOID*                           pv_nfy_tag
                    );
/*------------------------------------------------------------------*/
/*! @brief  This APIis used to stop a specific stream via stream type
 *              in asynchronous way.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_strm_type - Contains the stream type for stopping.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_stop_stream(
                    HANDLE_T                        h_svctx,          /* in  */
                    STREAM_TYPE_T                   e_strm_type       /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This APIis used to stop a specific stream via stream type
 *              in synchronous way.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_strm_type - Contains the stream type for stopping.
 *  @param  [in]  ui4_time - duration of stop
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_sync_stop_stream(
                    HANDLE_T                        h_svctx,          /* in  */
                    STREAM_TYPE_T                   e_strm_type,      /* in  */
                    UINT32                          ui4_time          /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API selects a specific audio stream via ISO639 language
 *              code in asynchronous way.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  pt_lang - References to the structure containing the
 *                             language code.
 *  @param  [in]  pf_nfy - References the stream notify function.
 *  @param  [in]  pv_nfy_tag - References the tag passed to the notify function.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_NOT_SUPPORT - The stream type is not supported in this service
 *                             context object.
 *  @retval SVCTXR_NO_RESOURCE - Not enough memory to allocate this request.
 *  @retval SVCTXR_INV_ARG - pt_lang is NULL.
 *  @retval SVCTXR_NOT_EXIST - The stream component is not existed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_select_audio_stream(
                    HANDLE_T                        h_svctx,          /* in  */
                    const ISO_639_LANG_T*           pt_lang,          /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API selects a specific closed captioning stream in
 *              asynchronous way. If the cc stream is selected successfully,
 *              caller will be notified via the pf_nfy with
 *                  e_nfy_cond    = SVCTX_COND_PRESENTING,
 *                  e_code        = SVCTX_NTFY_CODE_STREAM_STARTED
 *                  e_stream_type = ST_CLOSED_CAPTION
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  h_cc_gl_plane - Contains a handle to a graphic plane for closed
 *                             captioning handler to draw.
 *  @param  [in]  pt_cc_filter - References to the structure containing the
 *                             cc filter data.
 *  @param  [in]  pf_nfy - References the stream notify function.
 *  @param  [in]  pv_nfy_tag - References the tag passed to the notify function.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - pt_cc_filter is NULL or h_cc_gl_plane is NULL_HANDLE.
 *  @retval SVCTXR_NOT_SUPPORT - The stream type is not supported in this service
 *                             context object.
 *  @retval SVCTXR_NO_RESOURCE - Not enough memory to allocate this request.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_select_cc_stream(
                    HANDLE_T                        h_svctx,          /* in  */
                    HANDLE_T                        h_gl_plane,       /* in  */
                    const SM_CC_FILTER_T*           pt_cc_filter,     /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    );

#ifdef MW_ISDB_SUPPORT
/*------------------------------------------------------------------*/
/*! @brief  This API selects a specific closed captioning stream in
 *              asynchronous way. If the cc stream is selected successfully,
 *              caller will be notified via the pf_nfy with
 *                  e_nfy_cond    = SVCTX_COND_PRESENTING,
 *                  e_code        = SVCTX_NTFY_CODE_STREAM_STARTED
 *                  e_stream_type = ST_CLOSED_CAPTION
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  h_cc_gl_plane - Contains a handle to a graphic plane for closed
 *                             captioning handler to draw.
 *  @param  [in]  pt_cc_filter - References to the structure containing the
 *                             cc filter data.
 *  @param  [in]  pf_nfy - References the stream notify function.
 *  @param  [in]  pv_nfy_tag - References the tag passed to the notify function.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - pt_cc_filter is NULL or h_cc_gl_plane is NULL_HANDLE.
 *  @retval SVCTXR_NOT_SUPPORT - The stream type is not supported in this service
 *                             context object.
 *  @retval SVCTXR_NO_RESOURCE - Not enough memory to allocate this request.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_select_isdb_caption_stream(
                    HANDLE_T                        h_svctx,          /* in  */
                    HANDLE_T                        h_gl_plane,       /* in  */
                    UINT32                          ui4_caption_idx,  /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API selects a specific closed captioning stream in
 *              asynchronous way. If the cc stream is selected successfully,
 *              caller will be notified via the pf_nfy with
 *                  e_nfy_cond    = SVCTX_COND_PRESENTING,
 *                  e_code        = SVCTX_NTFY_CODE_STREAM_STARTED
 *                  e_stream_type = ST_CLOSED_CAPTION
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  h_cc_gl_plane - Contains a handle to a graphic plane for closed
 *                             captioning handler to draw.
 *  @param  [in]  pt_cc_filter - References to the structure containing the
 *                             cc filter data.
 *  @param  [in]  pf_nfy - References the stream notify function.
 *  @param  [in]  pv_nfy_tag - References the tag passed to the notify function.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - pt_cc_filter is NULL or h_cc_gl_plane is NULL_HANDLE.
 *  @retval SVCTXR_NOT_SUPPORT - The stream type is not supported in this service
 *                             context object.
 *  @retval SVCTXR_NO_RESOURCE - Not enough memory to allocate this request.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_select_isdb_text_stream(
                    HANDLE_T                        h_svctx,          /* in  */
                    HANDLE_T                        h_gl_plane,       /* in  */
                    UINT32                          ui4_text_idx,     /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    );
#endif

/*------------------------------------------------------------------*/
/*! @brief  This API selects a specific subtitle stream in
 *              asynchronous way. If the subtitle stream is selected successfully,
 *              caller will be notified via the pf_nfy with
 *                  e_code        = SVCTX_NTFY_CODE_STREAM_STARTED
 *                  e_stream_type = ST_SUBTITLE
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  h_gl_plane - Contains a handle to a graphic plane for closed
 *                             captioning handler to draw.
 *  @param  [in]  pt_lang - References to the structure containing the
 *                             language code.
 *  @param  [in]  pf_nfy - References the stream notify function.
 *  @param  [in]  pv_nfy_tag - References the tag passed to the notify function.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - pt_lang is NULL or h_gl_plane is NULL_HANDLE.
 *  @retval SVCTXR_NOT_SUPPORT - The stream type is not supported in this service
 *                             context object.
 *  @retval SVCTXR_NO_RESOURCE - Not enough memory to allocate this request.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_select_subtitle_stream(
                    HANDLE_T                        h_svctx,          /* in  */
                    HANDLE_T                        h_gl_plane,       /* in  */
                    const ISO_639_LANG_T*           pt_lang,          /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API selects a specific subtitle stream in
 *              asynchronous way. If the subtitle stream is selected successfully,
 *              caller will be notified via the pf_nfy with
 *                  e_code        = SVCTX_NTFY_CODE_STREAM_STARTED
 *                  e_stream_type = ST_SUBTITLE
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  h_gl_plane - Contains a handle to a graphic plane for closed
 *                             captioning handler to draw.
 *  @param  [in]  pt_comp_id - References to the structure containing the
 *                             stream component id.
 *  @param  [in]  pf_nfy - References the stream notify function.
 *  @param  [in]  pv_nfy_tag - References the tag passed to the notify function.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - pt_lang is NULL or h_gl_plane is NULL_HANDLE.
 *  @retval SVCTXR_NOT_SUPPORT - The stream type is not supported in this service
 *                             context object.
 *  @retval SVCTXR_NO_RESOURCE - Not enough memory to allocate this request.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_select_subtitle_stream_by_comp_id(
                    HANDLE_T                        h_svctx,          /* in  */
                    HANDLE_T                        h_gl_plane,       /* in  */
                    const STREAM_COMP_ID_T*         pt_comp_id,       /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API selects a specific subtitle stream in
 *              asynchronous way. If the subtitle stream is selected successfully,
 *              caller will be notified via the pf_nfy with
 *                  e_code        = SVCTX_NTFY_CODE_STREAM_STARTED
 *                  e_stream_type = ST_SUBTITLE
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  h_gl_plane - Contains a handle to a graphic plane for closed
 *                             captioning handler to draw.
 *  @param  [in]  pt_sbtl_info - Contains extern information for subtitle playback.
 *  @param  [in]  pt_comp_id - References to the structure containing the
 *                             stream component id.
 *  @param  [in]  pf_nfy - References the stream notify function.
 *  @param  [in]  pv_nfy_tag - References the tag passed to the notify function.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - pt_lang is NULL or h_gl_plane is NULL_HANDLE.
 *  @retval SVCTXR_NOT_SUPPORT - The stream type is not supported in this service
 *                             context object.
 *  @retval SVCTXR_NO_RESOURCE - Not enough memory to allocate this request.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_select_subtitle_stream_by_comp_id_ex(
                    HANDLE_T                        h_svctx,          /* in  */
                    HANDLE_T                        h_gl_plane,       /* in  */
                    const SM_SBTLH_STREAM_INFO*     pt_sbtl_info,     /* in  */
                    const STREAM_COMP_ID_T*         pt_comp_id,       /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to get service list record by channel id and
 *              surfing mode, i.e. next channel, previous channel, or this channel.
 *  @param  [in]  h_svl - Contains a handle to a service list.
 *  @param  [in]  ui4_nw_mask - Contains the network mask for filtering service
 *                                  record.
 *  @param  [in]  e_surf_mode - Contains the surfing mode.
 *  @param  [in]  pui4_channel_id - References to the variable containing the
 *                                  channel id.
 *  @param  [out] pui4_channel_id - References to the variable containing the
 *                                  resulted channel id.
 *  @param  [out] pt_svl_rec - References to the structure containing the
 *                                  service list record data.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - pui4_channel_id or pt_svl_rec is NULL.
 *  @retval SVCTXR_NO_RECORD - There's no any record in this service list
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_svc_rec_by_channel_id(
                    HANDLE_T                        h_svl,              /* in */
                    UINT32                          ui4_nw_mask,        /* in */
                    SURF_MODE_T                     e_surf_mode,        /* in */
                    UINT32*                         pui2_channel_number,/* i/o*/
                    SVL_REC_T*                      pt_svl_rec          /* out*/
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to set video plane to normal, blank or quat-video.
 *          The details about e_mode should refer to stream manager's
 *          SCC_VID_MODE_T.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_mode - Contains the mode to set the video plane.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_WRONG_STATE - The Service Context object is blocked.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_set_video_plane(
                    HANDLE_T                        h_svctx,
                    SCC_VID_MODE_T                  e_mode
                    );

/*------------------------------------------------------------------*/
/*! @brief  This function is used to set the attribuites of a session.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_pipe_type - Contains the pipe type.
 *  @param  [in]  e_set_type - Contains the type of attributes to set.
 *  @param  [in]  pv_set_info - References to the data information of the set type.
 *  @param  [in]  ui4_set_info_size - Contains the size of data information of the
 *                            set type.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_NOT_SUPPORT - The e_set_type is not supported.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_set_session_attr(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_pipe_type,
                    SM_SET_TYPE_T                   e_set_type,
                    VOID*                           pv_set_info,
                    SIZE_T                          z_set_info_size
                    );

/*------------------------------------------------------------------*/
/*! @brief  This function is used to get the attribuites of a session.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_pipe_type - Contains the pipe type to get.
 *  @param  [in]  e_get_type - Contains the type of attributes to get.
 *  @param  [in]  pv_get_info - References to the data information of the get type.
 *  @param  [in]  pui4_get_info_size - References to the size of data information of
 *                             the get type.
 *  @param  [out] pv_get_info - content of information
 *  @param  [out] pui4_get_info_size - size of information
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_session_attr(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_pipe_type,
                    SM_GET_TYPE_T                   e_get_type,
                    VOID*                           pv_get_info,
                    SIZE_T*                         pz_get_info_size
                    );

/*------------------------------------------------------------------*/
/*! @brief  This function is used to set the attribuites of a stream.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_strm_type - Contains the stream type.
 *  @param  [in]  e_set_type - Contains the type of attributes to set.
 *  @param  [in]  pv_set_info - References to the data information of the set type.
 *  @param  [in]  ui4_set_info_size - Contains the size of data information of the
 *                            set type.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_NOT_SUPPORT  - The stream type is not supported in this service
 *                             context object.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_set_stream_attr(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_strm_type,
                    SM_SET_TYPE_T                   e_set_type,
                    VOID*                           pv_set_info,
                    SIZE_T                          z_set_info_size
                    );

/*------------------------------------------------------------------*/
/*! @brief  This function is used to get the attribuites of a stream.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_strm_type - Contains the stream type.
 *  @param  [in]  e_set_type - Contains the type of attributes to set.
 *  @param  [in]  pv_set_info - References to the data information of the set type.
 *  @param  [in]  ui4_set_info_size - Contains the size of data information of the
 *                            set type.
 *  @param  [out] pv_get_info - content of information
 *  @param  [out] pui4_get_info_size - size of information
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_NOT_SUPPORT  - The stream type is not supported in this service
 *                             context object.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_stream_attr(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_strm_type,
                    SM_GET_TYPE_T                   e_get_type,
                    VOID*                           pv_get_info,
                    SIZE_T*                         pz_get_info_size
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to get audio information.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [out] pt_resolution - References to the variable containing the
 *                          ASH_AUDIO_INFO_T.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - pt_resolution is NULL.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_audio_info(
                    HANDLE_T                        h_svctx,
                    ASH_AUDIO_INFO_T*               pt_info
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to get video source's resolution.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [out] pt_resolution - References to the variable containing the
 *                          VSH_SRC_RESOLUTION_INFO_T.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - pt_resolution is NULL
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_video_src_resolution(
                    HANDLE_T                        h_svctx,
                    VSH_SRC_RESOLUTION_INFO_T*      pt_resolution
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to unblock a blocked service (all streams are stopped).
 *              If b_force_unblock is TRUE, the blocked service will be unblocked
 *              and streams will be played. If b_force_unblock is FALSE, the client
 *              provided x_svctx_svc_block_check_fct function (if any) will be invoked
 *              again to check the service can be unblocked or not.
 *              If the service presented in the service context is not blocked,
 *              no action is performed.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  b_force_unblock - Contains the boolean to force unblock or not.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_ALREADY_STOPPED - The service conetxt is already stopped.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_unblock_svc(
                    HANDLE_T                        h_svctx,
                    BOOL                            b_force_unblock
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to set the default parameters for service context.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  ui4_stream_mode - Contains the default desired stream types.
 *  @param  [in]  e_vid_mode - Contains the default desired stream types.
 *  @param  [in]  b_able_set_pmx_bg - Specifiy to be able to set pmx bg.
 *  @param  [in]  pf_block_check_fct- Contains the block check fucntion pointer.
 *  @param  [in]  ps_src_grp_name - Contains the connection source name.
 *  @param  [in]  ps_snk_grp_name - Contains the sink group name.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_set_default_parameters(
                    HANDLE_T                    h_svctx,
                    UINT32                      ui4_stream_mode,
                    SCC_VID_MODE_T              e_vid_mode,
                    BOOL                        b_able_set_pmx_bg,
                    x_svctx_svc_block_check_fct pf_block_check_fct,
                    const CHAR*                 ps_src_grp_name,
                    const CHAR*                 ps_snk_grp_name,
                    const CHAR*                 ps_snk_grp_name_aux
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to set stream's default filter function and tag.
 *              When select proper streams to play, if client doesn't provide
 *              filter funtion, this filter will be used.
 *              SCC_VID_MODE_T.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_strm_type - Contains the stream component type for filtering.
 *  @param  [in]  pf_filter - References to the filter function provided by caller.
 *                             It can't be NULL.
 *  @param  [in]  pv_fltr_tag - References to a caller-defined tag. It will pass to
 *                             to caller when the pf_filter is invoked.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_NOT_SUPPORT - The stream type is not supported in this service
 *                             context object.
 *  @retval SVCTXR_INV_ARG - pt_filter_fct is NULL.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_set_default_strm_filter(
                    HANDLE_T                        h_svctx,
                    STREAM_TYPE_T                   e_strm_type,
                    x_svctx_strm_comp_filter_fct    pt_filter_fct,
                    VOID*                           pv_filter_tag
                    );

/*------------------------------------------------------------------*/
/*! @brief  his API is used to set screen mode.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_scr_mode - Contains the screen mode.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_set_screen_mode(
                    HANDLE_T                        h_svctx,
                    SCC_VID_SCREEN_MODE_T           e_scr_mode
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to set screen mode.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [out] pe_scr_mode - References to the variable containing the
 *                             SCC_VID_SCREEN_MODE_T.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - pe_scr_mode is NULL.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_screen_mode(
                    HANDLE_T                        h_svctx,
                    SCC_VID_SCREEN_MODE_T*          pe_scr_mode
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to get tuner's signal level.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [out] pui1_signal_level - References to the variable containing signal 
 *                                                           level.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - ui4_delay_in_ms is zero.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_signal_level(
                    HANDLE_T                        h_svctx,
                    UINT8*                          pui1_signal_level
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to get tuner's DBM signal level.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [out] pui1_signal_level - References to the variable containing signal 
 *                                                           level.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - ui4_delay_in_ms is zero.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_dbm_signal_level(
                    HANDLE_T                        h_svctx,
                    INT16*                          pi2_dbm_signal_level
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to get tuner's status.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [out] pui1_tuner_status - References to the variable containing tuner's 
 *                             status.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - pi2_dbm_signal_level is NULL.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_tuner_status(
                    HANDLE_T                        h_svctx,
                    UINT8*                          pui1_tuner_status
                    );

extern INT32 x_svctx_get_cell_id(
                    HANDLE_T                        h_svctx,
                    UINT16*                         pui2_cell_id
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to get display's resolution.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [out] pt_resolution - References to the variable containing the
 *                          SCC_DISP_RESOLUTION_T.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - pt_resolution is NULL.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_display_resolution(
                    HANDLE_T                        h_svctx,            /* in  */
                    SCC_DISP_RESOLUTION_T*          pt_resolution       /* out */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to set display's resolution.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [out] pt_resolution - References to the variable containing the
 *                          SCC_DISP_RESOLUTION_T.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - pt_resolution is NULL.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_set_display_resolution(
                    HANDLE_T                        h_svctx,            /* in  */
                    SCC_DISP_RESOLUTION_T*          pt_resolution       /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to query it is signal loss or not.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [out] pb_signal_loss - Contains the vairable for signal loss or not.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_is_signal_loss(
                    HANDLE_T                        h_svctx,            /* in  */
                    BOOL*                           pb_signal_loss      /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to get display's aspect ratio.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [out] pe_aspect_ratio - References to the variable containing the
 *                          SCC_DISP_ASPECT_RATIO_T.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - pt_resolution is NULL.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_display_aspect_ratio(
                    HANDLE_T                        h_svctx,            /* in  */
                    SCC_DISP_ASPECT_RATIO_T*        pe_aspect_ratio     /* out */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to set display's aspect ratio.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_aspect_ratio - Contains the value of SCC_DISP_ASPECT_RATIO_T
 *                                                      value.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_set_display_aspect_ratio(
                    HANDLE_T                        h_svctx,            /* in  */
                    SCC_DISP_ASPECT_RATIO_T         e_aspect_ratio      /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to set the number of records for a specific
 *              stream type. Note: for closed caption, it only return the
 *              amount that detected in the caption_service_descriptor.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_strm_type - Contains the stream type value.
 *  @param  [out] pui2_num_recs - References to the variable containing the number of
 *                           records
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - ui4_delay_in_ms is zero.
 *  @retval SVCTXR_WRONG_STATE - f related SCDB is not ready and opened.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_scdb_num_recs(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_strm_type,        /* in  */
                    UINT16*                         pui2_num_recs       /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API retrieves the data of selected stream of a specific type from
 *              a presenting service context object.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_strm_type - Contains the stream component type for filtering.
 *  @param  [out] pt_comp_id - References to the structure containing the
 *                             stream component id.
 *  @param  [out] pt_scdb_rec - References to the structure containing the
 *                             stream component data.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - pt_scdb_rec is NULL.
 *  @retval SVCTXR_NOT_EXIST - The stream component is not existed.
 *  @retval SVCTXR_NOT_SUPPORT - The stream type is not supported in this service
 *                             context object.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_using_strm_data(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_strm_type,        /* in  */
                    STREAM_COMP_ID_T*               pt_comp_id,         /* out */
                    SCDB_REC_T*                     pt_scdb_rec         /* out */
                    );

extern INT32 x_svctx_set_listener_no_lock(
                    HANDLE_T                        h_svctx,            /* in  */
                    x_svctx_listener_nfy_fct        pf_listener_fct,    /* in  */
                    VOID*                           pv_nfy_tag          /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to set the listener for monitoring service
 *              context's activities.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [out]  
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - ui4_delay_in_ms is zero.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_set_listener(
                    HANDLE_T                        h_svctx,            /* in  */
                    x_svctx_listener_nfy_fct        pf_listener_fct,    /* in  */
                    VOID*                           pv_nfy_tag          /* in  */
                    );
extern INT32 x_svctx_grab_ownership(
                    HANDLE_T                        h_svctx             /* in  */
                    );
extern INT32 x_svctx_release_ownership(
                    HANDLE_T                        h_svctx             /* in  */
                    );
                    
extern INT32 x_svctx_grab_ownership(
                    HANDLE_T                        h_svctx             /* in  */
                    );
extern INT32 x_svctx_release_ownership(
                    HANDLE_T                        h_svctx             /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to surf service from current service. It only allows
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
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  ui4_nw_mask - Contains the target svl record's network check mask.
 *  @param  [in]  ui4_nw_value - Contains the target svl record's network value for
 *                             comparison.
 *  @param  [in]  e_surf_mode - Contains the variable for surf service direction.
 *  @param  [out] pt_svl_rec - References to the structure containing the
 *                             target svl record.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - ui4_nw_chk_mask = 0
 *  @retval SVCTXR_NO_RECORD - Cannot find any service list record to surf.
 *  @retval SVCTXR_NO_RESOURCE - No enough memory to allocate this request.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_surf_svc(
                    HANDLE_T                        h_svctx,            /* in  */
                    UINT32                          ui4_nw_chk_mask,    /* in  */
                    UINT32                          ui4_nw_chk_value,   /* in  */
                    SURF_MODE_T                     e_surf_mode,        /* in  */
                    SVL_REC_T*                      pt_svl_rec          /* out */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to grab the ownership.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [out]
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - ui4_delay_in_ms is zero.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_grab_ownership(
                    HANDLE_T                        h_svctx             /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to release the ownership.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [out]  
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - ui4_delay_in_ms is zero.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_release_ownership(
                    HANDLE_T                        h_svctx             /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to get CP(Copy Protection) information for
 *              a specific stream. Normally, it is used when SVCTX_NTFY_CODE_CP
 *              is received.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_strm_type - Contains the stream type value.
 *  @param  [out] pt_cp_info - References to the variable containing the Copy
 *                          Protection information.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - pt_cp_info is NULL.
 *  @retval SVCTXR_NOT_SUPPORT - The stream type is not supported in this service
 *                             context object.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_cp_info(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_strm_type,        /* in  */
                    CP_INFO_LIST_T*                 pt_cp_info          /* out  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to get CA(Conditional Access) information
 *              for a specific stream. Normally, it is used when
 *              SVCTX_NTFY_CODE_CA is received.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_strm_type - Contains the stream type value.
 *  @param  [out] pt_ca_info - References to the variable containing the CA 
 *                                                information.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - pt_ca_info is NULL.
 *  @retval SVCTXR_NOT_SUPPORT - The stream type is not supported in this service
 *                             context object.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_ca_info(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_strm_type,        /* in  */
                    SVCTX_CA_INFO_T*                pt_ca_info          /* out  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to set the attribuites of a service context.
 *  @param  [in]  h_svctx - References to handle of a service context.
 *  @param  [in]  e_set_type - References to the type of attributes to set.
 *  @param  [in]  pv_set_info - References to the data information of the set type.
 *  @param  [in]  ui4_set_info_size - Contains the size of data information of the
 *                            set type.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_set(
                    HANDLE_T                        h_svctx,            /* in  */
                    SVCTX_SET_TYPE_T                e_set_type,         /* in  */
                    VOID*                           pv_set_info,        /* in  */
                    SIZE_T                          z_set_info_size     /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to get the attribuites of a service context.
 *  @param  [in]  h_svctx - References to handle of a service context.
 *  @param  [in]  e_set_type - References to the type of attributes to set.
 *  @param  [in]  pv_set_info - References to the data information of the set type.
 *  @param  [in]  ui4_set_info_size - Contains the size of data information of the
 *                            set type.
 *  @param  [out] pv_set_info - content of information.
 *  @param  [out] pv_set_info_size - size of information.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get(
                    HANDLE_T                        h_svctx,            /* in  */
                    SVCTX_GET_TYPE_T                e_get_type,         /* in  */
                    VOID*                           pv_get_info,        /* in/out */
                    SIZE_T*                         pz_get_info_size    /* in/out */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to freeze svctx. The minimum frozen stream
 *              is video stream. If ui4_freeze_strm_mask doesn't include
 *              ST_MASK_VIDEO, this API will add video stream to freeze 
 *              automatically.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  ui4_freeze_strm_mask - Contains the mask to indicate which streams
 *                                 will be frozen.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_freeze(
                    HANDLE_T                        h_svctx,                    /* in  */
                    UINT32                          ui4_freeze_strm_mask        /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to unfreeze svctx.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_unfreeze(
                    HANDLE_T                        h_svctx                     /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to check svctx is frozen or not.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [out] pb_result - Contains a flag for the result.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_ARG - pb_result is NULL.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_is_frozen(
                    HANDLE_T                        h_svctx,                    /* in  */
                    BOOL*                           pb_result                   /* out */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to get current event info's length.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_key_type - Event key type.
 *  @param  [in]  pv_key_info - Event key infor.
 *  @param  [out] pz_event_info_len - size of event info.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_current_event_info_len (
                    HANDLE_T                        h_svctx,            /* in  */
                    EVCTX_KEY_TYPE_T                e_key_type,         /* in  */
                    VOID*                           pv_key_info,        /* in  */
                    SIZE_T*                         pz_event_info_len   /* out */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to get current event info.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_key_type - Event key type.
 *  @param  [in]  pv_key_info - Event key infor.
 *  @param  [out] pz_event_info_len - size of event info.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_current_event_info (
                    HANDLE_T                        h_svctx,            /* in  */
                    EVCTX_KEY_TYPE_T                e_key_type,         /* in  */
                    VOID*                           pv_key_info,        /* in  */
                    SIZE_T*                         pz_event_info_len,  /* out */
                    VOID*                           pv_event_info       /* out */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to get device profile record.
 *  @param  [in]  e_dev_type - device type.
 *  @param  [out] pt_profile - device content.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_ARG - pt_profile is NULL.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_device_profile(                           
                    DEVICE_TYPE_T                   e_dev_type,                 /* in  */
                    DEVICE_TYPE_PROFILE_T*          pt_profile                  /* out */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to get well-defined video height.
 *  @param  [in]  ui4_raw_height - Contains the raw video height, e.g. 1088
 *  @param  [out] pui4_defined_height - Contains the well-defined video height. e.g. 1080
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_INV_ARG - pui4_defined_height is NULL.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_well_defined_video_heigh(
                    UINT32                          ui4_raw_height,             /* in  */
                    UINT32*                         pui4_defined_height         /* out */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to get cc capability of a svctx.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [out] pt_capablility - Contains the cc capable condition.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_INV_ARG - pt_capablility is NULL.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_cc_capability(
                    HANDLE_T                        h_svctx,                    /* in  */
                    SVCTX_CAPABILITY_T*             pt_capablility              /* out */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to get subtile capability of a svctx.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [out] pt_capablility - Contains the subtile capable condition.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_INV_ARG - pt_capablility is NULL.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_subtitle_capability(
                    HANDLE_T                        h_svctx,                    /* in  */
                    SVCTX_CAPABILITY_T*             pt_capablility              /* out  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to get ttx capability of a svctx.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [out] pt_capablility - Contains the ttx capable condition.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_INV_ARG - pt_capablility is NULL.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_teletext_capability(
                    HANDLE_T                        h_svctx,                    /* in  */
                    SVCTX_CAPABILITY_T*             pt_capablility              /* out  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to get event capability of a svctx.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [out] pt_capablility - Contains the event capable condition.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_INV_ARG - pt_capablility is NULL.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_event_capability(
                    HANDLE_T                        h_svctx,                    /* in  */
                    UINT32                          ui4_event_key_mask,         /* in  */
                    SVCTX_CAPABILITY_T*             pt_capablility              /* out */
                    );

extern INT32 x_svctx_get_device_profile_by_drv_type(
                    DRV_TYPE_T                      e_drv_type,                 /* in  */
                    DEVICE_TYPE_PROFILE_T*          pt_profile                  /* out */
                    );


extern INT32 x_svctx_init_device_mapping_table(
                    const AVC_TYPE_MAPPING*         pt_mapping_table,           /* in  */
                    SIZE_T                          z_num_of_element            /* in  */
                    );
extern INT32 x_svctx_get_mapped_dev_info(
                    DRV_TYPE_T                      e_drv_type,                 /* in  */
                    DEVICE_TYPE_T*                  pe_video_dev_type,          /* out */
                    DEVICE_TYPE_T*                  pe_best_video_dev_type,     /* out */
                    INT8*                           pi1_best_video_dev_order,   /* out */
                    DEVICE_TYPE_T*                  pe_audio_dev_type,          /* out */
                    DEVICE_TYPE_T*                  pe_best_audio_dev_type,     /* out */
                    INT8*                           pi1_best_audio_dev_order    /* out */
                    );


/*------------------------------------------------------------------*/
/*! @brief  This API returns a handle to the SCDB object for a specified svctx.
 *             Basically, it is a help function to let caller to handle
 *             the SCDB by itself, but the return value is SVCTXR_XXX
 *              not SMR_XXX.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_pipe_type - Additional info for specifing pipe. it could be ST_VIDEO,
 *                         ST_AUDIO, or ST_UNKNOWN.
 *  @param  [in]  pv_tag - Private tag value associated with the calling client.
 *  @param  [in]  pf_nfy - Client notification function.
 *  @param  [out] ph_scdb - pointer to a SCDB handle.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - (SCDBR_INV_ARG)          ph_scdb is NULL.
 *  @retval SVCTXR_NO_RESOURCE - (SCDBR_OUT_OF_HANDLE)    No more handle available,
 *                              (SCDBR_OUT_OF_MEM)       or Out of memory.
 *  @retval SVCTXR_WRONG_STATE - connection is not available to create SCDB.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_open_scdb(
                    HANDLE_T                        h_svctx,                    /* in */
                    STREAM_TYPE_T                   e_pipe_type,                /* in */
                    HANDLE_T*                       ph_scdb,                    /* out*/
                    VOID*                           pv_tag,                     /* in */
                    x_scdb_nfy_fct                  pf_nfy                      /* in */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API frees the specified handle to the SCDB database.
 *              Basically, it is a help function to let caller to handle
 *              the SCDB by itself, but the return value is SVCTXR_XXX
 *              not SMR_XXX.
 *  @param  [in]  h_scdb - handle to be freed.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_close_scdb(                    
                    HANDLE_T                        h_scdb                      /* in */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This function is used to open a stream in a specific svctx.
 *              Basically, it is a help function to let caller to handle
 *              the stream by itself, but the return value  is SVCTXR_XXX
 *              not SMR_XXX.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_pipe_type - Additional info for specifing pipe. it could be ST_VIDEO,
 *                          ST_AUDIO, or ST_UNKNOWN.
 *  @param  [in]  pt_strm_comp_id - References to the component ID aquired from SCDB.
 *  @param  [in]  pt_cmd - References to the command sets to initialize the stream.
 *  @param  [in]  pf_strm_nfy - References to a stream event callback function,
 *                          which is implemented by the caller.
 *  @param  [in]  pv_strm_nfy_tag - References to a tag to be pass to the stream event
 *                          callback function.
 *  @param  [in]  b_auto_play - Indicate if the stream sould play automatically.
 *  @param  [out] ph_stream - References to the stream handle created by Stream
 *                                                Manager.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - (SMR_INV_ARG)        The argument ph_stream, 
 *                                                  pf_strm_nfy,or pt_strm_comp_id is invalid.
 *  @retval SVCTXR_NOT_SUPPORT - (SMR_NOT_SUPPORT)    There is no stream handler 
 *                                                  that support this type of stream.
 *  @retval SVCTXR_WRONG_STATE - session is not available.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_open_stream(                   
                    HANDLE_T                        h_svctx,                    /* in */
                    STREAM_TYPE_T                   e_pipe_type,                /* in */
                    const STREAM_COMP_ID_T*         pt_strm_comp_id,            /* in */
                    const SM_COMMAND_T*             pt_cmd_sets,                /* in */
                    x_stream_nfy_fct                pf_strm_nfy,                /* in */
                    VOID*                           pv_strm_nfy_tag,            /* in */
                    BOOL                            b_auto_play,                /* in */
                    HANDLE_T*                       ph_stream                   /* out*/
                    );

/*------------------------------------------------------------------*/
/*! @brief  This function is used to open a stream in a specific svctx.
 *              Basically, it is a help function to let caller to handle
 *              the stream by itself, but the return value  is SVCTXR_XXX
 *              not SMR_XXX.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_pipe_type - Additional info for specifing pipe. it could be ST_VIDEO,
 *                          ST_AUDIO, or ST_UNKNOWN.
 *  @param  [in]  pt_strm_comp_id - References to the component ID aquired from SCDB.
 *  @param  [in]  pt_cmd - References to the command sets to initialize the stream.
 *  @param  [in]  pf_strm_nfy - References to a stream event callback function,
 *                          which is implemented by the caller.
 *  @param  [in]  pv_strm_nfy_tag - References to a tag to be pass to the stream event
 *                          callback function.
 *  @param  [in]  b_auto_play - Indicate if the stream sould play automatically.
 *  @param  [out] ph_stream - References to the stream handle created by Stream
 *                                                Manager.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - (SMR_INV_ARG)        The argument ph_stream, 
 *                                                  pf_strm_nfy,or pt_strm_comp_id is invalid.
 *  @retval SVCTXR_NOT_SUPPORT - (SMR_NOT_SUPPORT)    There is no stream handler 
 *                                                  that support this type of stream.
 *  @retval SVCTXR_WRONG_STATE - session is not available.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_open_stream_ex(
                    HANDLE_T                        h_svctx,                    /* in */
                    STREAM_TYPE_T                   e_pipe_type,                /* in */
                    const STREAM_COMP_ID_T*         pt_strm_comp_id,            /* in */
                    const SM_COMMAND_T*             pt_cmd_sets,                /* in */
                    x_stream_nfy_fct                pf_strm_nfy,                /* in */
                    VOID*                           pv_strm_nfy_tag,            /* in */
                    BOOL                            b_auto_play,                /* in */
                    UINT32                          ui4_serial_num,             /* in */
                    HANDLE_T*                       ph_stream                   /* out*/
                    );

/*------------------------------------------------------------------*/
/*! @brief  This function is used to close a stream of Stream Manager.
 *              Basically, it is a help function to let caller to handle
 *              the stream by itself, but the return value  is SVCTXR_XXX
 *              not SMR_XXX.
 *  @param  [in]  h_stream - References to the handle of the stream.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_close_stream(                  
                    HANDLE_T                        h_stream                    /* in */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API gets information about a connection.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_pipe_type - Additional info for specifing pipe. it could be ST_VIDEO,
 *                         ST_AUDIO, or ST_UNKNOWN.
 *  @param  [in]  e_ctrl - Contains the control code, and it is defined in CM's header file.
 *  @param  [out] pv_ctrl_data - Contains information about the connection.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_NO_RESOURCE - No more handle available,
 *                               or Out of memory.
 *  @retval SVCTXR_WRONG_STATE - connection is not available.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_conn_attr(
                    HANDLE_T                        h_svctx,                    /* in */
                    STREAM_TYPE_T                   e_pipe_type,                /* in */
                    CM_CTRL_TYPE_T                  e_ctrl,                     /* in */
                    VOID*                           pv_ctrl_data                /* out */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API sets some attributes of a connection.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_pipe_type - Additional info for specifing pipe. it could be ST_VIDEO,
 *                         ST_AUDIO, or ST_UNKNOWN
 *  @param  [in]  e_ctrl - Contains the control code, and it is defined in CM's header file.
 *  @param  [in]  pv_ctrl_data - References a data structure which content depends on
 *                         e_ctrl.
 *  @param  [out] pv_ctrl_data - Contains values that may have been modified.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_NO_RESOURCE - No more handle available,
 *                               or Out of memory.
 *  @retval SVCTXR_WRONG_STATE - connection is not available.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_set_conn_attr(
                    HANDLE_T                        h_svctx,                    /* in */
                    STREAM_TYPE_T                   e_pipe_type,                /* in */
                    CM_CTRL_TYPE_T                  e_ctrl,                     /* in */
                    VOID*                           pv_ctrl_data                /* in/out */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API sets analog tuner out attributes.
 *  @param  [in]  pt_tuner_out - References a data structure which content tuner out
 *                         parameters.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_set_analog_tuner_out_attr(
                    TUNER_OUT_DESC_T*               pt_tuner_out                /* in */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to perfrom service block check, however, this
 *      API is just a check, it doesn't perfrom anu service block/unblock
 *      operation.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_check_guide - Contains the block check guide.
 *  @param  [in]  ps_src_name - Contains the source group name.
 *  @param  [in]  pt_svl_rec - Contains the SVL record if source is TV.
 *  @param  [in]  pv_evn_rating -  Contains the event's rating buffer obtained from EVctx.
 *  @return return block value
 *  @retval SVC_BLOCK_COND_T - the check result.
 */
/*------------------------------------------------------------------*/
extern SVC_BLOCK_COND_T x_svctx_svc_block_check(
                    HANDLE_T                        h_svctx,                    /* in */
                    SVC_BLOCK_CHECK_GUDIE_T         e_check_guide,              /* in */
                    const CHAR*                     ps_src_name,                /* in */
                    const SVL_REC_T*                pt_svl_rec,                 /* in */
                    const VOID*                     pv_evn_rating               /* in */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to set aux audio enable or not
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  b_enable - Contains set value.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_set_ad_state(
                    HANDLE_T                        h_svctx,          /* in  */
                    BOOL                            b_enable          /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API tries to open an ad stream specified by *pt_scdb_rec.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  pt_scdb_rec - References to the structure containing the
 *                               scdb data of the ad stream.
 *  @return return the error code
 *  @retval SVCTXR_OK         -  Routine successful.
 *  @retval SVCTXR_INV_ARG    -  pt_scdb_rec is NULL.
 *  @retval SVCTXR_NOT_SUPPORT-  The stream type is not supported in this service
 *                       context object.
 *  @retval SVCTXR_NOT_EXIST  -  The stream component is not existed.
 *  @retval SVCTXR_NOT_INIT   -  The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE -  Invalid service context handle.
 *  @retval SVCTXR_FAIL       -  SCDB error, can not get the record
 *  @retval SVCTXR_WRONG_STATE-  The operation is not permitted in current
 *                               scenaro.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_set_ad_state_ex(
                    HANDLE_T                        h_svctx,          /* in  */
                    const SCDB_REC_T*               pt_scdb_rec       /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to get aux audio enable or not
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [out]  pb_enable - Contains return value.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_get_ad_state(
                    HANDLE_T                        h_svctx,          /* in  */
                    BOOL*                           pb_enable         /* out */
                    );

#ifdef MW_TV_AV_BYPASS_SUPPORT
/*------------------------------------------------------------------*/
/*! @brief  This API is used to create association instance for a svctx instance.
 *  @param  [in]  ps_name - Svctx instance name.
 *  @param  [in]  pt_assc_cfg - Association Configuration.
 *  @param  [out]  ph_svctx - Return a association instance.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_NO_RESOURCE - No more handle available,
 *                               or Out of memory.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_create_associate(
                    const CHAR*                     ps_name,
                    const SVCTX_ASSC_CFG_T*         pt_assc_cfg,
                    HANDLE_T*                       ph_svctx);

#ifdef MW_PIP_POP_SUPPORT
/*------------------------------------------------------------------*/
/*! @brief  This API is used to add association instance for a svctx instance.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  h_svctx_excl - Contains a exclustion handle to a service context.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - Svctx don't init.
 *  @retval SVCTXR_INV_ARG - ui4_excl_src_mask == 0.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_add_associate(
                    HANDLE_T                        h_svctx,
                    HANDLE_T                        h_svctx_assc);

/*------------------------------------------------------------------*/
/*! @brief  This API is used to add exclustion instance for a svctx instance.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  h_svctx_excl - Contains a exclustion handle to a service context.
 *  @param  [in]  ui4_excl_src_mask - Mask for exclustion source. 
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - Svctx don't init.
 *  @retval SVCTXR_INV_ARG - ui4_excl_src_mask == 0.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_add_exclusion(
                    HANDLE_T                        h_svctx,
                    HANDLE_T                        h_svctx_excl,
                    UINT32                          ui4_excl_src_mask);

/*------------------------------------------------------------------*/
/*! @brief  This API is used to enter pip/pop status.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_src_type - SRC_TYPE_BRDCST_SVL, SRC_TYPE_BRDCST_FREQ.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - Svctx don't init.
 *  @retval SVCTXR_NO_RIGHT - Handle isn't owner.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_start_exclusion(
                    HANDLE_T                        h_svctx,
                    SRC_TYPE_T                      e_src_type);

/*------------------------------------------------------------------*/
/*! @brief  This API is used to exit pip/pop status.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_src_type - SRC_TYPE_BRDCST_SVL, SRC_TYPE_BRDCST_FREQ.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - Svctx don't init.
 *  @retval SVCTXR_NO_RIGHT - Handle isn't owner.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_stop_exclusion(
                    HANDLE_T                        h_svctx,
                    SRC_TYPE_T                      e_src_type);
#endif
#endif

/*------------------------------------------------------------------*/
/*! @brief  This API is used to perfrom media file open function 
 *      it would scan media content to check the content correctness
 *      and generate the service list for AP further service selection. 
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  pt_cmd - Commands list should include src_type , svl_name,svl_id and
 *               media_path
 *  @param  [in]  pf_nfy - Callback function when open finished.
 *  @param  [in]  pv_tag - Tag.
 *  @param  [out]  ph_media - Contains return media handle.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Media open fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_media_open(
                    HANDLE_T                        h_svctx,                    /* in */
                    MM_SVC_COMMAND_T*               pt_cmd,                     /* in */    
                    x_svctx_media_prc_nfy_fct       pf_nfy,                     /* in */    
                    VOID*                           pv_tag,                     /* in */    
                    HANDLE_T*                       ph_media                    /* out */  
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to perfrom media file or clip close function.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  h_svctx - Contains a handle to a media.
 *  @param  [out] 
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_FAIL - Send request failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_media_close(
                    HANDLE_T                        h_svctx,                    /* in */
                    HANDLE_T                        h_media                     /* in */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to perfrom media search function.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  pf_nfy - Callback function when open finished.
 *  @param  [in]  pv_tag - Tag.
 *  @param  [in]  pt_pos_info - Position to be seeked to.
 *  @param  [out] 
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_FAIL - Routine failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_media_seek(
                    HANDLE_T                        h_svctx,                    /* in */
                    x_svctx_select_nfy_fct          pf_nfy,                     /* in */
                    VOID*                           pv_tag,                     /* in */    
                    MM_SVC_POS_INFO_T*              pt_pos_info                 /* in */    
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to load video thumbnail.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  h_media - Contains media handle.
 *  @param  [in]  pt_thumbnail_info - Contains the thumbnail information.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_FAIL - Routine failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_media_load_thumbnail(
                    HANDLE_T                        h_svctx,                    /* in */
                    HANDLE_T                        h_media,                    /* in */
                    SVCTX_VIDEO_THUMBNAIL_INFO_T*   pt_thumbnail_info           /* in */    
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to sync cancel the loading of video thumbnail.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  h_media - Contains media handle.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_FAIL - Routine failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_media_sync_cancel_load_thumbnail(
                    HANDLE_T                        h_svctx,                    /* in */
                    HANDLE_T                        h_media                     /* in */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to get media information.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  h_media - Contains media handle.
 *  @param  [in]  t_svctx_get_type - Get type.
 *  @param  [in]  z_get_info_size - Return value size.
 *  @param  [out]  pv_get_info - Return value to be stored.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_FAIL - Routine failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_media_get_info(
                    HANDLE_T h_svctx,                                   /* in */
                    HANDLE_T h_media,                                   /* in */
                    SVCTX_GET_TYPE_T t_svctx_get_type,      /* in */
                    VOID* pv_get_info,                                  /* in/out */
                    SIZE_T z_get_info_size                             /* in */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to get media information.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  h_media - Contains media handle.
 *  @param  [in]  t_svctx_set_type - Set type.
 *  @param  [in]  z_set_info_size - Set value size.
 *  @param  [in]  pv_set_info - Value to be set.
 *  @note   
 *  @see    
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_FAIL - Routine failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_media_set_info(
                    HANDLE_T h_svctx,                                   /* in */
                    HANDLE_T h_media,                                   /* in */
                    SVCTX_SET_TYPE_T t_svctx_set_type,      /* in */
                    VOID* pv_set_info,                                  /* in */
                    SIZE_T z_set_info_size                             /* in */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to get generic media information. It doesn't need to open media first.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  h_media - Contains media handle.
 *  @param  [in]  t_svctx_get_type - Get type.
 *  @param  [in]  z_get_info_size - The size of the buffer to store return value.
 *  @param  [out]  pv_get_info - Return value to be stored.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_FAIL - Routine failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_generic_get_info(
                    HANDLE_T h_svctx,                                   /* in */
                    HANDLE_T h_media,                                   /* in */
                    SVCTX_GET_TYPE_T t_svctx_get_type,      /* in */
                    VOID* pv_get_info,                                  /* in/out */
                    SIZE_T z_get_info_size                             /* in */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API is used to set generic media information. It doesn't need to open media first.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  h_media - Contains media handle.
 *  @param  [in]  t_svctx_get_type - Get value type.
 *  @param  [in]  pv_get_info - Return value to be stored.
 *  @param  [in]  z_get_info_size - The size of the buffer to store return value.
 *  @param  [out] pv_get_info - Return value.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_FAIL - Routine failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_generic_set_info(
                    HANDLE_T h_svctx,                                   /* in */
                    HANDLE_T h_media,                                   /* in */
                    SVCTX_SET_TYPE_T t_svctx_set_type,      /* in */
                    VOID* pv_set_info,                                  /* in/out */
                    SIZE_T z_set_info_size);                            /* in */


#ifdef MM_TIME_SHIFT_UNIT_TEST
extern VOID x_svctx_time_shift_update_valid_range(
                    HANDLE_T                        h_svctx,                    /* in */
                    MM_BUF_RANGE_INFO_T*            pt_range                    /* in */
                    );
#endif /* MM_TIME_SHIFT_UNIT_TEST */

#ifdef SYS_RECORD_SUPPORT
/*------------------------------------------------------------------*/
/*! @brief  This API creates a Recrod SVCTX.
 *  @param  [in]  ps_svctx_name - Specifies the service context name for the create.
 *                         The name must be unique to other SVCTX.
 *  @param  [in]  pt_rec_attr - Contains the attributes for Record SVCTX.
 *  @param  [out] ph_svctx_rec - Contains the Record SVCTX handle.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_FAIL - Send request failed.
 *  @retval SVCTXR_INV_ARG - pt_rec_attr is NULL.
 *  @retval SVCTXR_NO_RESOURCE - Not enough memory to open the service context.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_rec_create (
                    const CHAR*                     ps_svctx_name,              /* in */
                    const SVCTX_REC_ATTR_T*         pt_rec_attr,                /* in */
                    HANDLE_T*                       ph_svctx_rec);              /* out */

/*------------------------------------------------------------------*/
/*! @brief  This API starts the recording session.
 *  @param  [in]  h_svctx_rec - Specifies the Record SVCTX handle.
 *  @param  [in]  pt_config - Contains the config for recording.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_INV_ARG - pt_config is NULL.
 *  @retval SVCTXR_WRONG_STATE - Invalid service context state to start recording.
 *  @retval SVCTXR_NO_RESOURCE - Not enough memory / resource.
 *          < 0                 Other errors. 
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_rec_start (
                    HANDLE_T                        h_svctx_rec,                /* in */
                    const SVCTX_REC_CONFIG_T*       pt_config                   /* in */
                    );

/*------------------------------------------------------------------*/
/*! @brief  This API stops the recording session. 
 *  @param  [in]  h_svctx_rec - Specifies the Record SVCTX handle.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_WRONG_STATE - Invalid service context state to stop recording.
 *          < 0                 Other errors. 
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_rec_stop (
                    HANDLE_T                        h_svctx_rec                 /* in */
                    );               

/*------------------------------------------------------------------*/
/*! @brief  This API pauses the recording session.
 *  @param  [in]  h_svctx_rec - Specifies the Record SVCTX handle.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_WRONG_STATE - Invalid service context state to pause recording.
 *          < 0                 Other errors. 
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_rec_pause (
                    HANDLE_T                        h_svctx_rec                 /* in */
                    );               

/*------------------------------------------------------------------*/
/*! @brief  This API resumes the recording session.
 *  @param  [in]  h_svctx_rec - Specifies the Record SVCTX handle.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_HANDLE - Invalid service context handle.
 *  @retval SVCTXR_WRONG_STATE - Invalid service context state to pause recording.
 *          < 0                 Other errors.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_rec_resume (
                    HANDLE_T                        h_svctx_rec                 /* in */
                    );               
#endif

#ifdef GINGA_SUPPORT
/*------------------------------------------------------------------*/
/*! @brief  This API to register the pre-select stream.
 *  @param  [in]  pf_pre_select_stream_fct - Specifies pre-select stream function.
 *  @return return the error code
 *  @retval SVCTXR_OK - Routine successful.
 *  @retval SVCTXR_NOT_INIT - The Service Context Module is not initialized yet.
 *  @retval SVCTXR_INV_ARG  - pf_pre_select_stream_fct is NULL.
 
 */
/*------------------------------------------------------------------*/
extern INT32 x_svctx_reg_pre_select_stream_cb(
                   x_svctx_pre_select_stream_nfy_fct pf_pre_select_stream_fct /* in*/
                   );
#endif

#endif

/*----------------------------------------------------------------------------*/
/*! @} */                                           /* end of groupMW_SVCTX */
/*----------------------------------------------------------------------------*/


