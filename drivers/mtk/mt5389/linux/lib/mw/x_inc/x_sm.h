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
/*----------------------------------------------------------------------------*/
/*! @file x_sm.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Stream Manager specific definitions,
 *         which are exported.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_STRM_MNGR Stream Manager
 *
 *  @ingroup groupMW
 *  @brief Provides stream manager external function 
 *
 *
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/
#ifndef _X_SM_H_
#define _X_SM_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_sm.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#define DEFAULT_PLAY_TIMEOUT      ((UINT32)20000)      /* ms */
#define DEFAULT_STOP_TIMEOUT     ((UINT32)20000)      /* ms */

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @brief           This function is used to open a session of Stream Manager.
 *  @param [in]    h_connection     The connection handle created by connection  Manager.
 *  @param [in]    ps_sess_name     References to the name of session handler.
 *  @param [in]    pv_sess_data     References to the data used by the session handler.
 *  @param [in]    z_sess_data_size Contains the size of data used by session handler.
 *  @param [in]    i4_speed         Coantins the default play speed of this session.
 *  @param [in]    pf_sess_nfy      References to the session event callback funtion,which should be implement by the caller.
 *  @param [in]    pv_sess_nfy_tag  References to a caller specifiec tag, which will be pass into the pf_sess_nfy callback function.
 *  @param [out]  ph_sess          References to the session handle created by this function .
 *  @note
 *  @see
 *  @return    Return the error code
 *  @retval      -SMR_OK                       Success.
 *  @retval      -SMR_INV_HANDLE               The h_connection is invalid.
 *  @retval      -SMR_INV_ARG                  The ph_sess, pv_sess_date, z_sess_data_size, ps_sess_name or  pf_sess_nfy is invalid.
 *  @retval      -SMR_SESS_ALREADY_EXIST       The session correlsponding to the connection is already opened.
 *  @retval      -SMR_HDLR_NOT_REGISTERED_YET  The ps_sess_name references to no  session handler.
 *  @retval      -SMR_INTERNAL_ERROR           A serious internal error.
 *  @retval      -SMR_INSUFFICIENT_MEMORY      Not enough memory resource.
 *  @retval      -SMR_NOT_INIT                 The Stream Manager is not initialized yet.
 */
/*------------------------------------------------------------------*/
extern INT32 x_sm_open_sess( HANDLE_T              h_connection,
                             const CHAR*           ps_sess_name,
                             const CHAR*           ps_dest_grp_name,
                             VOID*                 pv_sess_data,
                             SIZE_T                z_sess_data_size,
                             INT32                 i4_speed,
                             x_sess_nfy_fct        pf_sess_nfy,
                             VOID*                 pv_sess_nfy_tag,
                             HANDLE_T*             ph_sess );

/*------------------------------------------------------------------*/
/*! @brief  This function is used to close a session of Stream Manager.
 *  @param [in] h_sess References to handle of the session.
 *  @note
 *  @see
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  A serious internal error.
 *  @retval      -SMR_INV_HANDLE      The h_sess is invalid.
 */
/*------------------------------------------------------------------*/
extern INT32 x_sm_close_sess( HANDLE_T             h_sess );

/*------------------------------------------------------------------*/
/*! @brief   This function is used to play a session of Stream Manager.
 *  @param [in]  h_sess   References to handle of the session.
 *  @param [in]  i4_speed References to speed fo the play.
 *  @note
 *  @see
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  A serious internal error.
 *  @retval      -SMR_INV_HANDLE      The h_sess is invalid.
 */
/*------------------------------------------------------------------*/
extern INT32 x_sm_play_sess( HANDLE_T              h_sess,
                             INT32                 i4_speed );

/*------------------------------------------------------------------*/
/*! @brief     This function is used to stop a  session .
 *  @param [in]   h_sess References to handle of the session.
 *  @note
 *  @see
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  A serious internal error.
 *  @retval      -SMR_INV_HANDLE      The h_sess is invalid.
 */
/*------------------------------------------------------------------*/
extern INT32 x_sm_stop_sess( HANDLE_T              h_sess );

/*------------------------------------------------------------------*/
/*! @brief      This function is used to open a stream in a specific stream.
 *  @param [in]   h_sess          References to the session.
 *  @param [in]   pt_strm_comp_id References to the component ID aquired from SCDB.
 *  @param [in]   pt_cmd          References to the command sets to initialize the stream.
 *  @param [in]    pf_strm_nfy     References to a stream event callback function, which is implemented by the caller.
 *  @param [in]  pv_strm_nfy_tag References to a tag to be pass to the stream event  callback function.
 *  @param [in]  b_auto_play     Indicate if the stream sould play automatically.
 *  @param [out] ph_stream       References to the stream handle created by Stream  Manager. 
 *  @note
 *  @see
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  A serious internal error.
 *  @retval      -SMR_INV_HANDLE      The h_sess is invalid.
 *  @retval      -SMR_NOT_SUPPORT     There is no stream handler that support this type of stream.
 *  @retval      - SMR_INV_ARG         The argument h_sess is invalid.
 */
/*------------------------------------------------------------------*/
extern INT32 x_sm_open_stream( HANDLE_T                 h_sess,
                               const STREAM_COMP_ID_T*  pt_strm_comp_id,
                               const SM_COMMAND_T*      pt_cmd_sets,
                               x_stream_nfy_fct         pf_strm_nfy,
                               VOID*                    pv_strm_nfy_tag,
                               BOOL                     b_auto_play,
                               HANDLE_T*                ph_stream ); /* OUT */

/*------------------------------------------------------------------*/
/*! @brief  This function is used to close a stream of Stream Manager.
 *  @param [in]  h_stream References to the handle of the stream.
 *  @note
 *  @see
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  A serious internal error.
 *  @retval      -SMR_INV_HANDLE      The h_sess is invalid.
 */
/*------------------------------------------------------------------*/
extern INT32 x_sm_close_stream( HANDLE_T           h_stream );

/*------------------------------------------------------------------*/
/*! @brief  This function is used to play a stream of Stream Manager
 *  @param [in] h_stream References to the handle of the stream.
 *  @note
 *  @see
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  A serious internal error.
 *  @retval      -SMR_INV_HANDLE      The h_stream is invalid.
 */
/*------------------------------------------------------------------*/
extern INT32 x_sm_play_stream( HANDLE_T            h_stream );

/*------------------------------------------------------------------*/
/*! @brief   This function is used to stop a stream of Stream Manager
 *  @param [in]  h_stream References to the handle of the stream.
 *  @note
 *  @see
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  A serious internal error.
 *  @retval      -SMR_INV_HANDLE      The h_stream is invalid.
 */
/*------------------------------------------------------------------*/
extern INT32 x_sm_stop_stream( HANDLE_T            h_stream );

/*------------------------------------------------------------------*/
/*! @brief  This function is used to set the attribuites of a stream or a session.
 *  @param [in]  h_sm_handle       References to handle of a session or stream.
 *  @param [in]  e_set_type        References to the type of attributes to set.
 *  @param [in]  pv_set_info       References to the data information of the set type.
 *  @param [in]  ui4_set_info_size Contains the size of data information of the set type.
 *  @note
 *  @see
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  A serious internal error.
 *  @retval      -SMR_INV_HANDLE      The h_stream is invalid.
 */
/*------------------------------------------------------------------*/
extern INT32 x_sm_set( HANDLE_T            h_sm_handle,
                       SM_SET_TYPE_T       e_set_type,
                       VOID*               pv_set_info,
                       SIZE_T              z_set_info_size );

/*------------------------------------------------------------------*/
/*! @brief  This function is used to get the attribuites of a stream or a session.
 *  @param [in]  h_sm_handle        References to handle of a session or stream.
 *  @param [in]  e_get_type         References to the type of attributes to get.
 *  @param [in]   pv_get_info        References to the data information of the get   type.
 *  @param [in]   pui4_get_info_size References to the size of data information of the get type.
 *  @param [out]   pv_get_info        References to the data information of the get   type.
 *  @param [out]   pui4_get_info_size References to the size of data information of the get type.
 *  @note
 *  @see
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  A serious internal error.
 *  @retval      -SMR_INV_HANDLE      The h_sess is invalid.
 */
/*------------------------------------------------------------------*/
extern INT32 x_sm_get( HANDLE_T            h_sm_handle,
                       SM_GET_TYPE_T       e_get_type,
                       VOID*               pv_get_info,          /* IN/OUT */
                       SIZE_T*             pz_get_info_size );   /* IN/OUT */

/*------------------------------------------------------------------*/
/*! @brief   This function not use currently
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_sm_register_obsvr(
    const SM_OBSVR_FILTER_T*      pt_fltr,
    x_sm_obsvr_sess_nfy_fct       pf_sess_nfy,
    VOID*                         pv_sess_nfy_tag,
    x_sm_obsvr_strm_nfy_fct       pf_strm_nfy,
    VOID*                         pv_strm_nfy_tag,
    HANDLE_T*                     ph_obsvr );

/*------------------------------------------------------------------*/
/*! @brief   This function not use currently
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_sm_unregister_obsvr( HANDLE_T       h_obsvr );

extern INT32 x_sm_sync_play_stream( HANDLE_T             h_stream,
                                                                   UINT32              ui4_delay);

extern INT32 x_sm_sync_stop_stream( HANDLE_T             h_stream,
                                                      UINT32              ui4_delay);



#endif /* _X_SM_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */                                             /* end of groupMW_STRM_MNGR */
/*----------------------------------------------------------------------------*/

