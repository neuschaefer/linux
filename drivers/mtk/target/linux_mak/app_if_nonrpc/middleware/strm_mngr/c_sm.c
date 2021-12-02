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
 * $RCSfile: c_sm.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/1 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: 5689893de515121d24d6bb9920084285 $
 *
 * Description: 
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "aee/aee.h"
#include "handle/handle.h"
#include "strm_mngr/c_sm.h"
#include "strm_mngr/x_sm.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: c_sm_open_sess
 *
 * Description: This function is used to open a session of Stream Manager.
 *
 * Inputs:  h_connection     The connection handle created by connection 
 *                           Manager.
 *          ps_sess_name     References to the name of session handler.
 *          pv_sess_data     References to the data used by the session handler.
 *          z_sess_data_size Contains the size of data used by session handler.
 *          i4_speed         Coantins the default play speed of this session.
 *          pf_sess_nfy      References to the session event callback funtion,
 *                           which should be implement by the caller.
 *          pv_sess_nfy_tag  References to a caller specifiec tag, which will be
 *                           pass into the pf_sess_nfy callback function.
 *
 * Outputs: ph_sess          References to the session handle created by this 
 *                           function .
 *
 * Returns: SMR_OK                       Success.
 *          SMR_INV_HANDLE               The h_connection is invalid.
 *          SMR_INV_ARG                  The ph_sess, pv_sess_date, 
 *                                       z_sess_data_size, ps_sess_name or
 *                                       pf_sess_nfy is invalid.
 *          SMR_SESS_ALREADY_EXIST       The session correlsponding to the 
 *                                       connection is already opened.
 *          SMR_HDLR_NOT_REGISTERED_YET  The ps_sess_name references to no
 *                                       session handler.
 *          SMR_INTERNAL_ERROR           A serious internal error.
 *          SMR_INSUFFICIENT_MEMORY      Not enough memory resource.
 *          SMR_NOT_INIT                 The Stream Manager is not initialized
 *                                       yet.
 *
 ----------------------------------------------------------------------------*/
INT32 c_sm_open_sess( HANDLE_T              h_connection,
                      const CHAR*           ps_sess_name,
                      const CHAR*           ps_dest_grp_name,
                      VOID*                 pv_sess_data,
                      SIZE_T                z_sess_data_size,
                      INT32                 i4_speed,
                      x_sess_nfy_fct        pf_sess_nfy,
                      VOID*                 pv_sess_nfy_tag,
                      HANDLE_T*             ph_sess )
{
    HANDLE_T  h_aux;
    INT32     i4_ret;
    
    i4_ret = aee_grab_handle_resource( AEE_FLAG,
                                       1,
                                       &h_aux);
    if ( i4_ret != AEER_OK )
    {
        return SMR_OUT_OF_HANDLES;   
    }
    
    i4_ret = x_sm_open_sess( h_connection,
                             ps_sess_name,
                             ps_dest_grp_name,
                             pv_sess_data,
                             z_sess_data_size,
                             i4_speed,
                             pf_sess_nfy,
                             pv_sess_nfy_tag,
                             ph_sess );
    if ( i4_ret == SMR_OK )
    {
        handle_link_to_aux(h_aux, *ph_sess);
    }
    else
    {
        aee_release_handle_resource(1);
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: c_sm_close_sess
 *
 * Description: This function is used to close a session of Stream Manager.
 *
 * Inputs:  h_sess References to handle of the session.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  A serious internal error.
 *          SMR_INV_HANDLE      The h_sess is invalid.
 *
 ----------------------------------------------------------------------------*/
INT32 c_sm_close_sess( HANDLE_T             h_sess )
{
    return x_sm_close_sess( h_sess );
}

/*-----------------------------------------------------------------------------
 * Name: c_sm_play_sess
 *
 * Description: This function is used to play a session of Stream Manager.
 *
 * Inputs:  h_sess   References to handle of the session.
 *          i4_speed References to speed fo the play.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INV_HANDLE      The h_sess is invalid.
 *          SMR_INTERNAL_ERROR  A serious internal error.
 *
 ----------------------------------------------------------------------------*/
INT32 c_sm_play_sess( HANDLE_T             h_sess,
                      INT32                i4_speed )
{
    return x_sm_play_sess( h_sess, i4_speed );
}

/*-----------------------------------------------------------------------------
 * Name: c_sm_stop_sess
 *
 * Description: This function is used to stop a session of Stream Manager.
 *
 * Inputs:  h_sess References to handle of the session.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INV_HANDLE      The h_sess is invalid.
 *          SMR_INTERNAL_ERROR  A serious internal error.
 *
 ----------------------------------------------------------------------------*/
INT32 c_sm_stop_sess( HANDLE_T              h_sess )
{
    return x_sm_stop_sess( h_sess );
}

/*-----------------------------------------------------------------------------
 * Name: c_sm_open_stream
 *
 * Description: This function is used to open a stream in a specific session.
 *
 * Inputs:  h_sess          References to the session.
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
 * Returns: SMR_OK              Success.
 *          SMR_INV_HANDLE      The h_sess is invalid.
 *          SMR_INTERNAL_ERROR  A serious internal error.
 *          SMR_NOT_SUPPORT     There is no stream handler that support this 
 *                              type of stream.
 *          SMR_INV_ARG         The argument ph_stream, pf_strm_nfy, 
 *                              or pt_strm_comp_id is invalid.
 *
 ----------------------------------------------------------------------------*/
INT32 c_sm_open_stream( HANDLE_T                h_sess,
                        const STREAM_COMP_ID_T* pt_strm_comp_id,
                        const SM_COMMAND_T*     pt_cmd,
                        x_stream_nfy_fct        pf_strm_nfy,
                        VOID*                   pv_strm_nfy_tag,
                        BOOL                    b_auto_play,
                        HANDLE_T*               ph_stream )    /* OUT */
{
    HANDLE_T  h_aux;
    INT32     i4_ret;
    
    i4_ret = aee_grab_handle_resource( AEE_FLAG,
                                       1,
                                       &h_aux);
    if ( i4_ret != AEER_OK )
    {
        return SMR_OUT_OF_HANDLES;   
    }
    
    i4_ret = x_sm_open_stream( h_sess,
                               pt_strm_comp_id,
                               pt_cmd,
                               pf_strm_nfy,
                               pv_strm_nfy_tag,
                               b_auto_play,
                               ph_stream );
    if ( i4_ret == SMR_OK )
    {
        handle_link_to_aux(h_aux, *ph_stream);
    }
    else
    {
        aee_release_handle_resource(1);
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: c_sm_close_stream
 *
 * Description: This function is used to close a stream of Stream Manager.
 *
 * Inputs:  h_stream References to the handle of the stream.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INV_HANDLE      The h_stream is invalid.
 *          SMR_INTERNAL_ERROR  A serious internal error.
 *
 ----------------------------------------------------------------------------*/
INT32 c_sm_close_stream( HANDLE_T h_stream )
{
    return x_sm_close_stream( h_stream );
}

/*-----------------------------------------------------------------------------
 * Name: c_sm_play_stream
 *
 * Description: This function is used to play a stream of Stream Manager
 *
 * Inputs:  h_stream References to the handle of the stream.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INV_HANDLE      The h_stream is invalid.
 *          SMR_INTERNAL_ERROR  A serious internal error.
 *
 ----------------------------------------------------------------------------*/
INT32 c_sm_play_stream( HANDLE_T h_stream )
{
    return x_sm_play_stream( h_stream );
}

/*-----------------------------------------------------------------------------
 * Name: c_sm_stop_stream
 *
 * Description: This function is used to stop a stream of Stream Manager
 *
 * Inputs:  h_stream References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INV_HANDLE      The h_stream is invalid.
 *          SMR_INTERNAL_ERROR  A serious internal error.
 *
 ----------------------------------------------------------------------------*/
INT32 c_sm_stop_stream( HANDLE_T          h_stream )
{
    return x_sm_stop_stream( h_stream );
}

/*-----------------------------------------------------------------------------
 * Name: c_sm_set
 *
 * Description: This function is used to set the attribuites of a stream or a
 *              session.
 *
 * Inputs:  h_sm_handle       References to handle of a session or stream.
 *          e_set_type        References to the type of attributes to set.
 *          pv_set_info       References to the data information of the set 
 *                            type.
 *          ui4_set_info_size Contains the size of data information of the 
 *                            set type.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INV_HANDLE      The h_stream is invalid.
 *          SMR_INTERNAL_ERROR  A serious internal error.
 *
 ----------------------------------------------------------------------------*/
INT32 c_sm_set( HANDLE_T            h_sm_handle,
                SM_SET_TYPE_T       e_set_type,
                VOID*               pv_set_info,
                SIZE_T              z_set_info_size )
{
    return x_sm_set( h_sm_handle,
                     e_set_type,
                     pv_set_info,
                     z_set_info_size );
}

/*-----------------------------------------------------------------------------
 * Name: c_sm_get
 *
 * Description: This function is used to get the attribuites of a stream or a
 *              session.
 *
 * Inputs:  h_sm_handle        References to handle of a session or stream.
 *          e_get_type         References to the type of attributes to get.
 *          pv_get_info        References to the data information of the get 
 *                             type.
 *          pui4_get_info_size References to the size of data information of 
 *                             the get type.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INV_HANDLE      The h_stream is invalid.
 *          SMR_INTERNAL_ERROR  A serious internal error.
 *
 ----------------------------------------------------------------------------*/
INT32 c_sm_get( HANDLE_T            h_sm_handle,                           
                SM_GET_TYPE_T       e_get_type,                         
                VOID*               pv_get_info,            /* IN/OUT */   
                SIZE_T*             pui4_get_info_size )    /* IN/OUT */
{                                                      
    return x_sm_get( h_sm_handle,
                     e_get_type,
                     pv_get_info,
                     pui4_get_info_size );
}

