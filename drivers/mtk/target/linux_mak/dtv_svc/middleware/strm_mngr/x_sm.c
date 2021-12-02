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
 * $RCSfile: x_sm.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/13 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: c1a63987bd3bc970f3f7f02e5d2da324 $
 *
 * Description: 
 *         This header file contains Stream Manager related API's.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "strm_mngr/x_sm.h"

#include "conn_mngr/x_cm.h"
#include "hash/hash_api.h"
#include "strm_mngr/sm_slctr.h"

#include "strm_mngr/_sm_hc_tbl.h"
#include "strm_mngr/_sm_sess_tbl.h"
#include "strm_mngr/_sm_utils.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
extern BOOL            gb_sm_inited;

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions implementation
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: x_sm_open_sess
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
INT32 x_sm_open_sess( HANDLE_T              h_connection,
                      const CHAR*           ps_sess_name,
                      const CHAR*           ps_dest_grp_name,
                      VOID*                 pv_sess_data,
                      SIZE_T                z_sess_data_size,
                      INT32                 i4_speed,
                      x_sess_nfy_fct        pf_sess_nfy,
                      VOID*                 pv_sess_nfy_tag,
                      HANDLE_T*             ph_sess )
{
    INT32                   i4_ret = 0;
    HANDLE_T                h_sess_hdlr;
    HANDLE_TYPE_T           e_hdl_type;
    SM_SESS_HANDLER_T*      pt_sess_hdlr_obj = NULL;
    SM_USR_SESS_NFY_INFO_T* pt_usr_nfy_info = NULL;
    
    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_connection     = 0x%.8X\r\n"
                       "\t\tps_sess_name     = %s\r\n"
                       "\t\tps_dest_grp_name = %s\r\n"
                       "\t\tpv_sess_data     = 0x%.8X\r\n"
                       "\t\tz_sess_data_size = %u\r\n"
                       "\t\ti4_speed         = %i\r\n"
                       "\t\tpf_sess_nfy      = 0x%.8X\r\n"
                       "\t\tpv_sess_nfy_tag  = 0x%.8X\r\n"
                       "\t\tph_sess          = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_connection,
                       (ps_sess_name)?ps_sess_name:"NULL",
                       (ps_dest_grp_name)?ps_dest_grp_name:"NULL",
                       (UINT32)pv_sess_data,
                       z_sess_data_size,
                       i4_speed,
                       (UINT32)pf_sess_nfy,
                       (UINT32)pv_sess_nfy_tag,
                       (UINT32)ph_sess ));
    
    if ( !gb_sm_inited )
    {
        return SMR_NOT_INIT;
    }
    if ( !x_handle_valid( h_connection ) )
    {
        return SMR_INV_HANDLE;    
    }
    if ( !ph_sess || !pf_sess_nfy || !ps_sess_name || !ps_dest_grp_name )
    {
        return SMR_INV_ARG;    
    }
    if ( x_strlen( ps_sess_name ) > RM_NAME_LEN ||
         x_strlen( ps_dest_grp_name ) > RM_NAME_LEN ||
         ps_sess_name[0] == '\0' || ps_dest_grp_name[0] == '\0' )
    {
        return SMR_INV_ARG;    
    }
    
    /* check if this connection is already used to open a session */
    sm_sess_tbl_lock();
    
    i4_ret = sm_sess_tbl_find( h_connection, ph_sess );
     
    sm_sess_tbl_unlock();
    
    if ( i4_ret == SMR_OK )
    {
        return SMR_SESS_ALREADY_EXIST;
    }
    
    /* get session handler */
    h_sess_hdlr = sm_hc_tbl_find_sess_hdlr( ps_sess_name );
    if ( h_sess_hdlr == NULL_HANDLE )
    {
        return SMR_HDLR_NOT_REGISTERED_YET;
    }

    i4_ret = handle_get_type_obj( h_sess_hdlr,
                                  &e_hdl_type,
                                  (VOID**)&pt_sess_hdlr_obj);    
    if ( i4_ret != HR_OK || e_hdl_type != SM_HT_SESS_HANDLER )
    {
        return SMR_INTERNAL_ERROR;
    }

    /* prepare tag infomation, should be free when session is closed. */
    pt_usr_nfy_info = (SM_USR_SESS_NFY_INFO_T*)
        x_mem_alloc( sizeof(SM_USR_SESS_NFY_INFO_T) );
    if ( !pt_usr_nfy_info )
    {
        return SMR_INSUFFICIENT_MEMORY;
    }
    pt_usr_nfy_info->pf_usr_sess_nfy = pf_sess_nfy;
    pt_usr_nfy_info->pv_usr_sess_nfy_tag = pv_sess_nfy_tag;
    
    /* new a session */
    i4_ret = pt_sess_hdlr_obj->t_fct_tbl.pf_open_sess( h_connection,
                                                       h_sess_hdlr,
                                                       ps_dest_grp_name,
                                                       pv_sess_data,
                                                       z_sess_data_size,
                                                       i4_speed,
                                                       (VOID*)pt_usr_nfy_info,
                                                       ph_sess );
    if ( i4_ret != SMR_OK || !ph_sess )
    {
        x_mem_free( pt_usr_nfy_info );
        return i4_ret;    
    }

    /* add session to control table */
    sm_sess_tbl_lock();
    
    i4_ret = sm_sess_tbl_add( h_connection, *ph_sess );
    
    sm_sess_tbl_unlock();
    
    if ( i4_ret != SMR_OK )
    {
        pt_sess_hdlr_obj->t_fct_tbl.pf_close( *ph_sess );
        return i4_ret;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_sm_close_sess
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
INT32 x_sm_close_sess( HANDLE_T             h_sess )
{
    INT32                   i4_ret;
    SM_SESSION_T*           pt_sess_obj = NULL;
    SM_SESS_HANDLER_T*      pt_sess_hdlr_obj = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:( h_sess = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_sess ));

    if ( !gb_sm_inited )
    {
        return SMR_NOT_INIT;
    }

    if ( sm_sess_lock() != SMR_OK )
    {
        return SMR_INV_HANDLE;
    }

    pt_sess_hdlr_obj = sm_get_sess_hdlr_obj( h_sess, 
                                             (VOID**)&pt_sess_obj );
    if ( !pt_sess_hdlr_obj || !pt_sess_obj )
    {
        sm_sess_unlock();
        return SMR_INV_HANDLE;
    }

    if ( sm_lock( pt_sess_obj->h_lock ) != SMR_OK )
    {
        sm_sess_unlock();
        return SMR_INV_HANDLE;
    }                                                    
    
    /* close the session */
    i4_ret = pt_sess_hdlr_obj->t_fct_tbl.pf_close( h_sess );
    
    sm_unlock( pt_sess_obj->h_lock );
    sm_sess_unlock();
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_sm_play_sess
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
INT32 x_sm_play_sess( HANDLE_T             h_sess,
                      INT32                i4_speed )
{
    INT32                   i4_ret;
    SM_SESSION_T*           pt_sess_obj = NULL;
    SM_SESS_HANDLER_T*      pt_sess_hdlr_obj = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:( h_sess = 0x%.8X, i4_speed = %d )\r\n",
                       __FUNCTION__,
                       (UINT32)h_sess,
                       i4_speed ));

    if ( !gb_sm_inited )
    {
        return SMR_NOT_INIT;
    }

    if ( sm_sess_lock() != SMR_OK )
    {
        return SMR_INV_HANDLE;
    }
    
    pt_sess_hdlr_obj = sm_get_sess_hdlr_obj( h_sess, 
                                             (VOID**)&pt_sess_obj );
    if ( !pt_sess_hdlr_obj || !pt_sess_obj )
    {
        sm_sess_unlock();
        return SMR_INV_HANDLE;
    }

    if ( sm_lock( pt_sess_obj->h_lock ) != SMR_OK )
    {
        sm_sess_unlock();
        return SMR_INV_HANDLE;
    } 

    i4_ret = pt_sess_hdlr_obj->t_fct_tbl.pf_set( h_sess,
                                                 SM_SESS_SET_TYPE_PLAY,
                                                 (VOID*)i4_speed,
                                                 sizeof(i4_speed) );

    sm_unlock( pt_sess_obj->h_lock );     
    sm_sess_unlock();

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_sm_stop_sess
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
INT32 x_sm_stop_sess( HANDLE_T              h_sess )
{
    INT32                   i4_ret;
    SM_SESSION_T*           pt_sess_obj = NULL;
    SM_SESS_HANDLER_T*      pt_sess_hdlr_obj = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:( h_sess = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_sess ));
    
    if ( !gb_sm_inited )
    {
        return SMR_NOT_INIT;
    }

    if ( sm_sess_lock() != SMR_OK )
    {
        return SMR_INV_HANDLE;
    }
    
    pt_sess_hdlr_obj = sm_get_sess_hdlr_obj( h_sess, 
                                             (VOID**)&pt_sess_obj );
    if ( !pt_sess_hdlr_obj || !pt_sess_obj )
    {
        sm_sess_unlock();
        return SMR_INV_HANDLE;
    }
    
    if ( sm_lock( pt_sess_obj->h_lock ) != SMR_OK )
    {
        sm_sess_unlock();
        return SMR_INV_HANDLE;
    } 

    i4_ret = pt_sess_hdlr_obj->t_fct_tbl.pf_set( h_sess,
                                                 SM_SESS_SET_TYPE_STOP,
                                                 (VOID*)NULL,
                                                 0 );

    sm_unlock( pt_sess_obj->h_lock );
    sm_sess_unlock();
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_sm_open_stream
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
INT32 x_sm_open_stream( HANDLE_T                h_sess,
                        const STREAM_COMP_ID_T* pt_strm_comp_id,
                        const SM_COMMAND_T*     pt_cmd,
                        x_stream_nfy_fct        pf_strm_nfy,
                        VOID*                   pv_strm_nfy_tag,
                        BOOL                    b_auto_play,
                        HANDLE_T*               ph_stream )    /* OUT */
{
    SM_STREAM_T            t_strm_base;
    INT32                  i4_ret;
    SM_SESSION_T*          pt_sess_obj = NULL;
    SM_SESS_HANDLER_T*     pt_sess_hdlr_obj = NULL;
    CM_CTRL_RM_SRC_COMP_T  t_ctrl_rm_src_comp; /* new structure */
    HANDLE_T               h_handler;
    HANDLE_T               h_stream = NULL_HANDLE;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_sess          = 0x%.8X\r\n"
                       "\t\tpt_strm_comp_id = 0x%.8X\r\n"
                       "\t\tpt_cmd          = 0x%.8X\r\n"
                       "\t\tpf_strm_nfy     = 0x%.8X\r\n"
                       "\t\tpv_strm_nfy_tag = %u\r\n"
                       "\t\tb_auto_play     = %s\r\n"
                       "\t\tph_stream       = 0x%.8X\r\n",
                       __FUNCTION__,
                       (UINT32)h_sess,
                       (UINT32)pt_strm_comp_id,
                       (UINT32)pt_cmd,
                       (UINT32)pf_strm_nfy,
                       (UINT32)pv_strm_nfy_tag,
                       (b_auto_play)?"TRUE":"FALSE",
                       (UINT32)ph_stream ));

    if ( !gb_sm_inited )
    {
        return SMR_NOT_INIT;
    }

    if ( !ph_stream || !pf_strm_nfy || !pt_strm_comp_id )
    {
        return SMR_INV_ARG;    
    }
    
    if ( sm_stream_lock() != SMR_OK )
    {
        return SMR_INV_HANDLE;
    }
    
    pt_sess_hdlr_obj = sm_get_sess_hdlr_obj( h_sess, 
                                             (VOID**)&pt_sess_obj );
    if ( !pt_sess_hdlr_obj || !pt_sess_obj )
    {
        sm_stream_unlock();
        return SMR_INV_HANDLE;
    }

    if ( sm_lock( pt_sess_obj->h_lock ) != SMR_OK )
    {
        sm_stream_unlock();
        return SMR_INV_HANDLE;
    }
    
    /* check if there is a stream handler */    
    h_handler = sm_hc_tbl_find_hdlr( pt_sess_hdlr_obj->t_hdlr_desc.s_hdlr_name, 
                                     pt_strm_comp_id->e_type);
    if ( h_handler == NULL_HANDLE )
    {
        sm_unlock( pt_sess_obj->h_lock ); 
        sm_stream_unlock();
        return SMR_NOT_SUPPORT;    
    }
    

    i4_ret = x_cm_get( pt_sess_obj->h_connection,
                        CM_CTRL_RM_SRC_COMP,
                        (VOID*)&t_ctrl_rm_src_comp);
    
    if ( i4_ret != CMR_OK )
    {
        sm_unlock( pt_sess_obj->h_lock ); 
        sm_stream_unlock();
        return SMR_INTERNAL_ERROR;
    }
        
    t_strm_base.h_sess         = h_sess;
    t_strm_base.h_scdb         = NULL_HANDLE;
    t_strm_base.h_pm_comp      = NULL_HANDLE;
    t_strm_base.h_connection   = pt_sess_obj->h_connection;
    t_strm_base.h_source       = t_ctrl_rm_src_comp.h_rm_comp; /* add after CM API is clear, iolo */
    t_strm_base.h_handler      = h_handler;
    t_strm_base.e_condition    = SM_COND_IGNORE;
    t_strm_base.t_strm_comp_id = *pt_strm_comp_id;
    t_strm_base.pv_nfy_tag     = pv_strm_nfy_tag;
    t_strm_base.pf_nfy         = pf_strm_nfy;
    t_strm_base.h_lock         = NULL_HANDLE;
    t_strm_base.i4_speed       = 100; /* default to 100% speed */
    t_strm_base.pf_sync_stream_unlock = NULL;
    t_strm_base.b_is_sync_stream_stop = FALSE;
    
    i4_ret = pt_sess_hdlr_obj->t_fct_tbl.pf_open_stream( 
                    &t_strm_base,
                    pt_cmd,
                    b_auto_play,
                    sm_stream_nfy,
                    (VOID*)(UINT32)pt_sess_hdlr_obj->t_fct_tbl.pf_strm_nfy,
                    &h_stream );   /* OUT */
    
    if ( i4_ret != SMR_OK || h_stream == NULL_HANDLE )
    {
        sm_unlock( pt_sess_obj->h_lock );
        sm_stream_unlock();
        return i4_ret;
    }
 
    sm_unlock( pt_sess_obj->h_lock );
    sm_stream_unlock();
    
    *ph_stream = h_stream;
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_sm_close_stream
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
INT32 x_sm_close_stream( HANDLE_T h_stream )
{
    INT32              i4_ret;
    SM_STREAM_T*       pt_strm_base = NULL;
    SM_SESSION_T*      pt_sess_obj = NULL;
    SM_SESS_HANDLER_T* pt_sess_hdlr_obj = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:( h_stream = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_stream ));
    
    if ( !gb_sm_inited )
    {
        return SMR_NOT_INIT;
    }

    if ( sm_stream_lock() != SMR_OK )
    {
        return SMR_INV_HANDLE;
    }
    
    pt_strm_base = sm_get_strm_obj( h_stream );
    if ( !pt_strm_base )
    {
        sm_stream_unlock();
        return SMR_INV_HANDLE;
    }
    
    pt_sess_hdlr_obj = sm_get_sess_hdlr_obj( pt_strm_base->h_sess, 
                                             (VOID**)&pt_sess_obj );
    if ( !pt_sess_hdlr_obj )
    {
        sm_stream_unlock();
        return SMR_INTERNAL_ERROR;
    }

    if ( sm_lock( pt_sess_obj->h_lock ) != SMR_OK )
    {
        sm_stream_unlock();
        return SMR_INV_HANDLE;
    }
    
    if ( sm_lock( pt_strm_base->h_lock ) != SMR_OK )
    {
        sm_unlock( pt_sess_obj->h_lock );
        sm_stream_unlock();
        return SMR_INV_HANDLE;
    }
    
    i4_ret = pt_sess_hdlr_obj->t_fct_tbl.pf_close( h_stream );
    
    sm_unlock( pt_strm_base->h_lock ); 
    sm_unlock( pt_sess_obj->h_lock );     
    sm_stream_unlock();

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_sm_play_stream
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
INT32 x_sm_play_stream( HANDLE_T h_stream )
{
    SM_SLCTR_DBG_API(( "[SM] %s:( h_stream = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_stream ));

    return x_sm_set( h_stream,
                     SM_STRM_SET_TYPE_PLAY,
                     (VOID*)NULL,
                     0 );
}

/*-----------------------------------------------------------------------------
 * Name: x_sm_stop_stream
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
INT32 x_sm_stop_stream( HANDLE_T          h_stream )
{
    SM_SLCTR_DBG_API(( "[SM] %s:( h_stream = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_stream ));

    return x_sm_set( h_stream,
                     SM_STRM_SET_TYPE_STOP,
                     (VOID*)NULL,
                     0 );
}

/*-----------------------------------------------------------------------------
 * Name: x_sm_set
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
INT32 x_sm_set( HANDLE_T            h_sm_handle,                           
                SM_SET_TYPE_T       e_set_type,                         
                VOID*               pv_set_info,                        
                SIZE_T              z_set_info_size )
{
    INT32              i4_ret;
    HANDLE_T           h_sess = NULL_HANDLE;
    HANDLE_TYPE_T      e_hdl_type;
    SM_STREAM_T*       pt_strm_base = NULL;
    SM_SESSION_T*      pt_sess_obj = NULL;
    SM_SESS_HANDLER_T* pt_sess_hdlr_obj = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_sm_handle        = 0x%.8X\r\n"
                       "\t\te_set_type         = %d\r\n"
                       "\t\tpv_set_info        = 0x%.8X\r\n"
                       "\t\tui4_set_info_size  = %u )\r\n",
                       __FUNCTION__,
                       (UINT32)h_sm_handle,
                       e_set_type,
                       (UINT32)pv_set_info,
                       z_set_info_size ));
    
    if ( !gb_sm_inited )
    {
        return SMR_NOT_INIT;
    }

    i4_ret = x_handle_get_type( h_sm_handle,
                                &e_hdl_type );
    if ( i4_ret != HR_OK )
    {
        return SMR_INV_HANDLE;
    }

    if ( e_hdl_type == SM_HT_SESSION )
    {
        if ( sm_sess_lock() != SMR_OK )
        {
            return SMR_INV_HANDLE;
        }
        
        h_sess = h_sm_handle;
    }
    else if ( e_hdl_type == SM_HT_STREAM && 
              SM_GET_GRP(e_set_type) != SM_GRP_SESS )
    {
        if ( sm_stream_lock() != SMR_OK )
        {
            return SMR_INV_HANDLE;
        }    
        
        pt_strm_base = sm_get_strm_obj( h_sm_handle );    
        if ( !pt_strm_base )
        {
            sm_stream_unlock();
            return SMR_INV_HANDLE;
        }
        
        h_sess = pt_strm_base->h_sess;
    }
    else
    {
        return SMR_INV_ARG;
    }
    
    pt_sess_hdlr_obj = sm_get_sess_hdlr_obj( h_sess, (VOID**)&pt_sess_obj );
    if ( !pt_sess_hdlr_obj )
    {
        if ( e_hdl_type == SM_HT_SESSION )
        {
            sm_sess_unlock();
        }
        else
        {
            sm_stream_unlock();
        }
    
        return SMR_INV_HANDLE;
    }
        
    if ( e_hdl_type == SM_HT_SESSION )
    {
        if ( sm_lock( pt_sess_obj->h_lock ) != SMR_OK )
        {
            if ( e_hdl_type == SM_HT_SESSION )
            {
                sm_sess_unlock();
            }
            else
            {
                sm_stream_unlock();
            }
            
            return SMR_INV_HANDLE;
        }
    }
    else
    {
        if ( sm_lock( pt_strm_base->h_lock ) != SMR_OK )
        {    
            if ( e_hdl_type == SM_HT_SESSION )
            {
                sm_sess_unlock();
            }
            else
            {
                sm_stream_unlock();
            }
            
            return SMR_INV_HANDLE;                                  
        }    
    }
    
    i4_ret = pt_sess_hdlr_obj->t_fct_tbl.pf_set( h_sm_handle,
                                                 e_set_type,
                                                 (VOID*)pv_set_info,
                                                 z_set_info_size );

    if ( e_hdl_type == SM_HT_SESSION )
    {
        sm_unlock( pt_sess_obj->h_lock );
        sm_sess_unlock();
    }
    else
    {
        sm_unlock( pt_strm_base->h_lock );
        sm_stream_unlock();
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_sm_get
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
INT32 x_sm_get( HANDLE_T            h_sm_handle,                           
                SM_GET_TYPE_T       e_get_type,                         
                VOID*               pv_get_info,            /* IN/OUT */   
                SIZE_T*             pui4_get_info_size )    /* IN/OUT */
{                                                      
    INT32              i4_ret;
    HANDLE_T           h_sess = NULL_HANDLE;
    HANDLE_TYPE_T      e_hdl_type;
    SM_STREAM_T*       pt_strm_base = NULL;
    SM_SESSION_T*      pt_sess_obj = NULL;
    SM_SESS_HANDLER_T* pt_sess_hdlr_obj = NULL;

    SM_SLCTR_DBG_API(( "[SM] %s:(\r\n"
                       "\t\th_sm_handle        = 0x%.8X\r\n"
                       "\t\te_get_type         = %d\r\n"
                       "\t\tpv_get_info        = 0x%.8X\r\n"
                       "\t\tpui4_get_info_size = 0x%.8X )\r\n",
                       __FUNCTION__,
                       (UINT32)h_sm_handle,
                       e_get_type,
                       (UINT32)pv_get_info,
                       (UINT32)pui4_get_info_size ));
    
    if ( !gb_sm_inited )
    {
        return SMR_NOT_INIT;
    }

    i4_ret = x_handle_get_type( h_sm_handle,
                                &e_hdl_type );
    if ( i4_ret != HR_OK )
    {
        return SMR_INV_HANDLE;
    }

    if ( e_hdl_type == SM_HT_SESSION )
    {
        if ( sm_sess_lock() != SMR_OK )
        {
            return SMR_INV_HANDLE;
        }
        
        h_sess = h_sm_handle;
    }
    else if ( e_hdl_type == SM_HT_STREAM )
    {
        if ( sm_stream_lock() != SMR_OK )
        {
            return SMR_INV_HANDLE;
        }
        
        pt_strm_base = sm_get_strm_obj( h_sm_handle );    
        if ( !pt_strm_base )
        {
            sm_stream_unlock();
            return SMR_INV_HANDLE;
        }
        
        h_sess = pt_strm_base->h_sess;
    }
    else
    {
        return SMR_INV_ARG;
    }
    
    pt_sess_hdlr_obj = sm_get_sess_hdlr_obj( h_sess, (VOID**)&pt_sess_obj );
    if ( !pt_sess_hdlr_obj )
    {
        if ( e_hdl_type == SM_HT_SESSION )
        {
            sm_sess_unlock();
        }
        else
        {
            sm_stream_unlock();
        }

        return SMR_INV_HANDLE;
    }
    
    if ( e_hdl_type == SM_HT_SESSION )
    {
        if ( sm_lock( pt_sess_obj->h_lock ) != SMR_OK )
        {
            if ( e_hdl_type == SM_HT_SESSION )
            {
                sm_sess_unlock();
            }
            else
            {
                sm_stream_unlock();
            }
            
            return SMR_INV_HANDLE; 
        }
    }
    else
    {
        if ( sm_lock( pt_strm_base->h_lock ) != SMR_OK )
        {    
            if ( e_hdl_type == SM_HT_SESSION )
            {
                sm_sess_unlock();
            }
            else
            {
                sm_stream_unlock();
            }
            
            return SMR_INV_HANDLE;                                  
        }    
    }
    
    i4_ret = pt_sess_hdlr_obj->t_fct_tbl.pf_get( h_sm_handle,
                                                 e_get_type,
                                                 (VOID*)pv_get_info,
                                                 pui4_get_info_size );
    
    if ( e_hdl_type == SM_HT_SESSION )
    {
        sm_unlock( pt_sess_obj->h_lock );
        sm_sess_unlock();
    }
    else
    {
        sm_unlock( pt_strm_base->h_lock );
        sm_stream_unlock();
    }
    
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_sm_sync_play_stream
 *
 * Description: This function is used to play a stream of Stream Manager
 *
 * Inputs:  h_stream References to the handle of the stream.
 *          ui4_delay           maximum time duration in ms to wait the API
 *                             to finish. if it is zero, it will be set to default value.
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INV_HANDLE      The h_stream is invalid.
 *          SMR_INTERNAL_ERROR  A serious internal error.
 *
 ----------------------------------------------------------------------------*/
INT32 x_sm_sync_play_stream( HANDLE_T             h_stream,
                                                     UINT32                     ui4_delay)
{  
    INT32 i4_ret;
    SIZE_T                           z_get_size = 0;
    SM_COND_T                    e_cond = SM_SESS_COND_IGNORE;
    SM_STREAM_T*               pt_strm_base = NULL;

    pt_strm_base = sm_get_strm_obj( h_stream );
    if ( !pt_strm_base )
    {
        return SMR_INV_HANDLE;
    }

    if (ui4_delay == 0) 
    {
        ui4_delay = DEFAULT_PLAY_TIMEOUT;
    }

    if(pt_strm_base->e_condition==SM_COND_STARTED)
    {
        return SMR_ALREADY_PLAYED;
    }
    
    SM_LOCK_RETURN_ON_FAIL( pt_strm_base->h_lock, SMR_INV_HANDLE );

    pt_strm_base->pf_sync_stream_unlock = sm_sync_stream_unlock;
    
    SM_UNLOCK( pt_strm_base->h_lock );
    
    i4_ret = x_sm_set( h_stream,
                     SM_STRM_SET_TYPE_PLAY,
                     (VOID*)NULL,
                     0 );
    if(SMR_OK != i4_ret)
    {
        return i4_ret;
    }

    i4_ret = sm_sync_stream_lock(ui4_delay);
    if(i4_ret !=SMR_OK)
    {
         return i4_ret;
    }

    z_get_size = sizeof(SM_COND_T);
    x_sm_get( h_stream,
                       SM_STRM_GET_TYPE_STATE,
                       (VOID*)&e_cond,
                       &z_get_size );
     
    if(e_cond == SM_COND_STARTED)
    {
        return SMR_OK;
    }
    else if(e_cond == SM_COND_ERROR)
    {
        return SMR_INTERNAL_ERROR;
    }

    return SMR_OK;
    
}
/*-----------------------------------------------------------------------------
 * Name: x_sm_sync_stop_stream
 *
 * Description: This function is used to stop a stream of Stream Manager
 *
 * Inputs:  h_stream References to the handle of the stream.
 *          ui4_delay           maximum time duration in ms to wait the API
 *                             to finish. if it is zero, it will be set to default value.
 * Outputs: - 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INV_HANDLE      The h_stream is invalid.
 *          SMR_INTERNAL_ERROR  A serious internal error.
 *
 ----------------------------------------------------------------------------*/
INT32 x_sm_sync_stop_stream( HANDLE_T             h_stream,
                                                          UINT32                 ui4_delay)
{
    INT32 i4_ret;
    SIZE_T                           z_get_size = 0;
    SM_COND_T                    e_cond = SM_SESS_COND_IGNORE;
    SM_STREAM_T*               pt_strm_base = NULL;

    pt_strm_base = sm_get_strm_obj( h_stream );
    if ( !pt_strm_base )
    {
        return SMR_INV_HANDLE;
    }

    if (ui4_delay == 0) 
    {
        ui4_delay = DEFAULT_STOP_TIMEOUT;
    }

    if(pt_strm_base->e_condition==SM_COND_OPENED)
    {
        return SMR_ALREADY_STOPPED;
    }
    
    SM_LOCK_RETURN_ON_FAIL( pt_strm_base->h_lock, SMR_INV_HANDLE );

    pt_strm_base->pf_sync_stream_unlock = sm_sync_stream_unlock;
    pt_strm_base->b_is_sync_stream_stop =  TRUE;
    
    SM_UNLOCK( pt_strm_base->h_lock );

    i4_ret = x_sm_set( h_stream,
                     SM_STRM_SET_TYPE_STOP,
                     (VOID*)NULL,
                     0 );
    if(SMR_OK != i4_ret)
    {
        return i4_ret;
    }

    i4_ret = sm_sync_stream_lock(ui4_delay);
    if(i4_ret !=SMR_OK)
    {
         return i4_ret;
    }

    z_get_size = sizeof(SM_COND_T);
    x_sm_get( h_stream,
                       SM_STRM_GET_TYPE_STATE,
                       (VOID*)&e_cond,
                       &z_get_size );
     
    if(e_cond == SM_COND_OPENED)
    {
        return SMR_OK;
    }
    else if(e_cond == SM_COND_ERROR)
    {
        return SMR_INTERNAL_ERROR;
    }

    return SMR_OK;
}

