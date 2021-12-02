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
 * $RCSfile: _sm_sess_gnrc_pcr.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/10 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: 9651a12385e0d96d6be213500a0475f8 $
 *
 * Description: 
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_demux.h"
#include "strm_mngr/sm_slctr.h"
#include "strm_mngr/_sm_utils.h"
#include "strm_mngr/sm_drv_comp.h"
#include "strm_mngr/sess_gnrc_hdlr/sm_sess_gnrc.h"
#include "strm_mngr/sess_gnrc_hdlr/_sm_sess_gnrc_pcr.h"

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
 * Name: _sm_sess_gnrc_pcr_rm_nfy_fct
 *
 * Description: <function description>
 *
 * Inputs:  h_obj      References.
 *          ui1_port   References.
 *          e_nfy_cond References.
 *          pv_tag     References.
 *          ui4_data   References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_sess_gnrc_pcr_rm_nfy_fct( 
    HANDLE_T   h_obj,
    UINT8      ui1_port,
    RM_COND_T  e_nfy_cond,
    VOID*      pv_tag,
    UINT32     ui4_data )
{
    SM_SESS_GNRC_MSG_T     t_msg;
    
    if ( pv_tag == NULL )
    {
        return;
    }
    
    t_msg.e_msg_type = SM_SESS_GNRC_MSG_TYPE_PCR;
    t_msg.h_sess = (HANDLE_T)pv_tag;
    
    t_msg.t_pcr_msg.h_comp = h_obj;
    t_msg.t_pcr_msg.ui1_port = ui1_port;
    t_msg.t_pcr_msg.e_nfy_cond = e_nfy_cond;
    t_msg.t_pcr_msg.ui4_data = ui4_data;
    t_msg.t_pcr_msg.b_should_update_comp_descr = TRUE;
    
    sm_gs_send_msg( &t_msg );
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: sm_sess_gnrc_pcr_open
 *
 * Description: <function description>
 *
 * Inputs:  h_sess      References.
 *          pt_sess_obj References.
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *          SMR_DRV_COMP_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_sess_gnrc_pcr_open( 
    HANDLE_T        h_sess, 
    SM_SESS_GNRC_T* pt_sess_obj )
{
    INT32                  i4_ret;
    RM_COND_T              e_cond;
    
    x_strcpy( pt_sess_obj->t_pcr_descr.t_info.s_logical_name, 
              pt_sess_obj->t_base.s_dest_grp_name );
    
    pt_sess_obj->t_pcr_descr.t_info.e_type = DRVT_DEMUX_TS_PCR;
    i4_ret = sm_drv_comp_open( pt_sess_obj->h_source,
                               pt_sess_obj->t_pcr_descr.t_info.e_type,
                               ANY_COMP_ID,
                               pt_sess_obj->t_base.s_dest_grp_name,
                               NULL,
                               sm_drv_comp_sel_unused_comp_fct,
                               (VOID*)(h_sess),
                               _sm_sess_gnrc_pcr_rm_nfy_fct,
                               &pt_sess_obj->t_pcr_descr );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;   
    }

    e_cond = sm_rm_cond_filter( pt_sess_obj->t_pcr_descr.e_comp_cond,
                                COND_GRP_OBJ_STATE );
    if ( e_cond == RM_COND_OPENED )
    {
        SM_SESS_GNRC_MSG_T     t_msg;
        
        t_msg.e_msg_type = SM_SESS_GNRC_MSG_TYPE_PCR;
        t_msg.h_sess = h_sess;
        
        t_msg.t_pcr_msg.h_comp = pt_sess_obj->t_pcr_descr.h_comp;
        t_msg.t_pcr_msg.ui1_port = pt_sess_obj->t_pcr_descr.t_info.ui1_in_port;
        t_msg.t_pcr_msg.e_nfy_cond = pt_sess_obj->t_pcr_descr.e_comp_cond;
        t_msg.t_pcr_msg.ui4_data = 0;
        t_msg.t_pcr_msg.b_should_update_comp_descr = FALSE;
        
        sm_gs_send_msg( &t_msg );
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_gnrc_pcr_close
 *
 * Description: <function description>
 *
 * Inputs:  h_sess      References.
 *          pt_sess_obj References.
 *
 * Outputs: - 
 *
 * Returns: SMR_DRV_COMP_ERROR  Success.
 *          i4_ret              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_sess_gnrc_pcr_close( 
    HANDLE_T        h_sess, 
    SM_SESS_GNRC_T* pt_sess_obj )
{
    INT32                  i4_ret = SMR_OK;
    RM_COND_T              e_cond;

    pt_sess_obj->b_disable_av_sync = TRUE;
    
    if ( pt_sess_obj->t_pcr_descr.h_comp != NULL_HANDLE &&
         pt_sess_obj->t_pcr_descr.e_comp_cond != RM_COND_CLOSED )
    {
        i4_ret = sm_drv_comp_close( &pt_sess_obj->t_pcr_descr );
        if ( i4_ret != RMR_OK )
        {
        }
    }
    else
    {
        /* Pretend closed, if handle is not existed */
        sm_gs_send_msg_do_event(
            SM_EVN_SESS_PCR_CLOSED,
            h_sess,
            0 );
    }
    
    e_cond = sm_rm_cond_filter( pt_sess_obj->t_pcr_descr.e_comp_cond,
                                COND_GRP_OBJ_STATE );
    if ( e_cond == RM_COND_CLOSED )
    {
        SM_SESS_GNRC_MSG_T     t_msg;
        
        t_msg.e_msg_type = SM_SESS_GNRC_MSG_TYPE_PCR;
        t_msg.h_sess = h_sess;
        
        t_msg.t_pcr_msg.h_comp = pt_sess_obj->t_pcr_descr.h_comp;
        t_msg.t_pcr_msg.ui1_port = pt_sess_obj->t_pcr_descr.t_info.ui1_in_port;
        t_msg.t_pcr_msg.e_nfy_cond = pt_sess_obj->t_pcr_descr.e_comp_cond;
        t_msg.t_pcr_msg.ui4_data = 0;
        t_msg.t_pcr_msg.b_should_update_comp_descr = FALSE;
        
        sm_gs_send_msg( &t_msg );
    }
        
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_gnrc_pcr_connect
 *
 * Description: <function description>
 *
 * Inputs:  h_sess      References.
 *          pt_sess_obj References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK                      Success.
 *          SMR_STRM_COMP_ID_NOT_FOUND  Fail.
 *          SMR_DRV_COMP_ERROR          Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_sess_gnrc_pcr_connect( 
    HANDLE_T        h_sess, 
    SM_SESS_GNRC_T* pt_sess_obj )
{
    INT32                  i4_ret;
    RM_COND_T              e_cond;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    CONN_STATE_T           at_conn_state[1];
    UINT16                 ui2_num_entries = 1;
    CONN_DIR_TYPE_T        ae_conn_dir[1];
    
    i4_ret = rm_get_conn_state( pt_sess_obj->t_pcr_descr.h_comp,
                                pt_sess_obj->h_source,
                                at_conn_state,
                                &ui2_num_entries,
                                ae_conn_dir);
    if ( i4_ret != RMR_OK )
    {
        return SMR_DRV_COMP_ERROR;
    }
    
    if ( !(at_conn_state[0].ui1_conn_state & CONN_STATE_CONNECTED) &&
         !(at_conn_state[0].ui1_conn_state & CONN_STATE_HARD_WIRED) )
    {
#endif
        i4_ret = sm_drv_comp_connect( &pt_sess_obj->t_pcr_descr,
                                      pt_sess_obj->h_source,
                                      (VOID*)h_sess );
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }
        SM_GS_DBG_INFO(( "[SM] PCR CONNECT\r\n" ));
            
        e_cond = sm_rm_cond_filter( pt_sess_obj->t_pcr_descr.e_comp_cond,
                                    COND_GRP_CONN_STATE );
        if ( e_cond == RM_COND_CONNECTED )
        {
            SM_SESS_GNRC_MSG_T     t_msg;
            
            t_msg.e_msg_type = SM_SESS_GNRC_MSG_TYPE_PCR;
            t_msg.h_sess = h_sess;
            
            t_msg.t_pcr_msg.h_comp = pt_sess_obj->t_pcr_descr.h_comp;
            t_msg.t_pcr_msg.ui1_port = pt_sess_obj->t_pcr_descr.t_info.ui1_in_port;
            t_msg.t_pcr_msg.e_nfy_cond = pt_sess_obj->t_pcr_descr.e_comp_cond;
            t_msg.t_pcr_msg.ui4_data = 0;
            t_msg.t_pcr_msg.b_should_update_comp_descr = FALSE;
            
            sm_gs_send_msg( &t_msg );
        }
#ifdef MW_TV_AV_BYPASS_SUPPORT
        pt_sess_obj->b_pcr_owner = TRUE;
    }
    else
    {
        sm_gs_send_msg_do_event(
            SM_EVN_SESS_PCR_CONNECTED,
            h_sess,
            0 );
        SM_GS_DBG_INFO(( "[SM] PCR already CONNECT\r\n" ));
        pt_sess_obj->b_pcr_owner = FALSE;
    }
#endif

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_gnrc_pcr_disconnect
 *
 * Description: <function description>
 *
 * Inputs:  h_sess      References.
 *          pt_sess_obj References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK                      Success.
 *          SMR_STRM_COMP_ID_NOT_FOUND  Fail.
 *          SMR_DRV_COMP_ERROR          Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_sess_gnrc_pcr_disconnect( 
    HANDLE_T        h_sess, 
    SM_SESS_GNRC_T* pt_sess_obj )
{
    INT32                  i4_ret;
    RM_COND_T              e_cond;

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if ( pt_sess_obj->b_pcr_owner &&
         pt_sess_obj->t_pcr_descr.h_comp != NULL_HANDLE &&
         pt_sess_obj->t_pcr_descr.e_comp_cond != RM_COND_CLOSED )
#else
    if ( pt_sess_obj->t_pcr_descr.h_comp != NULL_HANDLE &&
         pt_sess_obj->t_pcr_descr.e_comp_cond != RM_COND_CLOSED )
#endif
    {
        /* Disable PCR */
        i4_ret = rm_set( pt_sess_obj->t_pcr_descr.h_comp , FALSE, 0, 
                         DEMUX_SET_TYPE_CTRL, 
                         (VOID*)FALSE, 
                         sizeof( BOOL ),
                         &pt_sess_obj->t_pcr_descr.e_comp_cond );
        if ( i4_ret != RMR_OK )
        {
           return SMR_DRV_COMP_ERROR;
        }
        SM_GS_DBG_INFO(( "[SM] Set PCR DISABLE\r\n" ));
        
        e_cond = sm_rm_cond_filter( pt_sess_obj->t_pcr_descr.e_comp_cond,
                                    COND_GRP_CONN_STATE );
        if ( e_cond == RM_COND_CONNECTED )
        {
            i4_ret = sm_drv_comp_disconnect( &pt_sess_obj->t_pcr_descr );
            if ( i4_ret != SMR_OK )
            {
                return i4_ret;
            }
            SM_GS_DBG_INFO(( "[SM] PCR DISCONNECT\r\n" ));
        }
        else if (  e_cond == RM_COND_DISCONNECTED )
        {
            sm_gs_send_msg_do_event(
                SM_EVN_SESS_PCR_DISCONNECTED,
                h_sess,
                0 );
            SM_GS_DBG_INFO(( "[SM] PCR already DISCONNECT\r\n" ));
        }
    }
    else
    {
        /* Pretend disconnected, if handle is not existed */
        sm_gs_send_msg_do_event(
            SM_EVN_SESS_PCR_DISCONNECTED,
            h_sess,
            0 );
    }
    
    e_cond = sm_rm_cond_filter( pt_sess_obj->t_pcr_descr.e_comp_cond,
                                COND_GRP_CONN_STATE );
    if ( e_cond == RM_COND_DISCONNECTED )
    {
        SM_SESS_GNRC_MSG_T     t_msg;
        
        t_msg.e_msg_type = SM_SESS_GNRC_MSG_TYPE_PCR;
        t_msg.h_sess = h_sess;
        
        t_msg.t_pcr_msg.h_comp = pt_sess_obj->t_pcr_descr.h_comp;
        t_msg.t_pcr_msg.ui1_port = pt_sess_obj->t_pcr_descr.t_info.ui1_in_port;
        t_msg.t_pcr_msg.e_nfy_cond = pt_sess_obj->t_pcr_descr.e_comp_cond;
        t_msg.t_pcr_msg.ui4_data = 0;
        t_msg.t_pcr_msg.b_should_update_comp_descr = FALSE;
        
        sm_gs_send_msg( &t_msg );
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_gnrc_pcr_set_pid
 *
 * Description: <function description>
 *
 * Inputs:  h_sess      References.
 *          pt_sess_obj References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK                      Success.
 *          SMR_STRM_COMP_ID_NOT_FOUND  Fail.
 *          SMR_DRV_COMP_ERROR          Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_sess_gnrc_pcr_set_pid( 
    HANDLE_T        h_sess, 
    SM_SESS_GNRC_T* pt_sess_obj )
{
    INT32     i4_ret;
    SIZE_T    z_get_size;
    BOOL      b_is_enable = FALSE;
    UINT16    ui2_pcr = pt_sess_obj->t_scdb_cmmn.u_data.t_mpeg.ui2_pcr;
#ifdef MW_SUPPORT_2STC
    UINT32    ui4_stc_id;
#endif 
    if ( pt_sess_obj->b_disable_av_sync )
    {
        return SMR_OK;   
    }
    
    if ( pt_sess_obj->t_base.h_scdb == NULL_HANDLE )
    {
        return SMR_STRM_COMP_ID_NOT_FOUND;   
    }
    
    z_get_size = sizeof( BOOL );
    i4_ret = rm_get( pt_sess_obj->t_pcr_descr.h_comp , FALSE, 0, 
                     DEMUX_GET_TYPE_CTRL, 
                     (VOID*)(UINT32)&b_is_enable, 
                     &z_get_size,
                     &pt_sess_obj->t_pcr_descr.e_comp_cond );
    if ( i4_ret != RMR_OK )
    {
       return SMR_DRV_COMP_ERROR;
    }
    
    if ( b_is_enable )
    {
        /* Disable PCR before set PID */
        i4_ret = rm_set( pt_sess_obj->t_pcr_descr.h_comp , FALSE, 0, 
                         DEMUX_SET_TYPE_CTRL, 
                         (VOID*)FALSE, 
                         sizeof( BOOL ),
                         &pt_sess_obj->t_pcr_descr.e_comp_cond );
        if ( i4_ret != RMR_OK )
        {
           return SMR_DRV_COMP_ERROR;
        }
        SM_GS_DBG_INFO(( "[SM] Set PCR DISABLE\r\n" ));
    }
    
    if ( (pt_sess_obj->t_scdb_cmmn.u_data.t_mpeg.ui2_pcr != pt_sess_obj->ui2_force_pid) &&
         (pt_sess_obj->ui2_force_pid != MPEG_2_NULL_PID) )
    {
        ui2_pcr = pt_sess_obj->ui2_force_pid;
    }
    
    if ( ui2_pcr != MPEG_2_NULL_PID )
    {
        i4_ret = rm_set( pt_sess_obj->t_pcr_descr.h_comp , FALSE, 0, 
                         DEMUX_SET_TYPE_PID, 
                         (VOID*)((UINT32)ui2_pcr),
                         sizeof( MPEG_2_PID_T ),
                         &pt_sess_obj->t_pcr_descr.e_comp_cond );
        if ( i4_ret != RMR_OK )
        {
           return SMR_DRV_COMP_ERROR;
        }
        SM_GS_DBG_INFO(( 
            "[SM] Set PCR DEMUX_SET_TYPE_PID = %u\r\n", 
            ui2_pcr ));
    
#ifdef MW_SUPPORT_2STC

        
        ui4_stc_id = (UINT32)pt_sess_obj->t_stc_descr.t_info.ui2_comp_id;

        x_dbg_stmt ("before set PCR STC %d\n", ui4_stc_id);
        i4_ret = rm_set( pt_sess_obj->t_pcr_descr.h_comp , FALSE, 0, 
                         DEMUX_SET_TYPE_STC_ID, 
                         (VOID*)(ui4_stc_id),
                         sizeof(UINT32),
                         &pt_sess_obj->t_pcr_descr.e_comp_cond );
        if ( i4_ret != RMR_OK )
        {
           return SMR_DRV_COMP_ERROR;
        }
        x_dbg_stmt ("after set PCR STC\n");
#endif 
        /* Enable PCR */
        i4_ret = rm_set( pt_sess_obj->t_pcr_descr.h_comp , FALSE, 0, 
                         DEMUX_SET_TYPE_CTRL, 
                         (VOID*)TRUE, 
                         sizeof( BOOL ),
                         &pt_sess_obj->t_pcr_descr.e_comp_cond );
        if ( i4_ret != RMR_OK )
        {
           return SMR_DRV_COMP_ERROR;
        }
        SM_GS_DBG_INFO(( "[SM] Set PCR ENABLE\r\n" ));
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_gnrc_pcr_apply_av_sync
 *
 * Description: <function description>
 *
 * Inputs:  h_sess            References.
 *          pt_sess_obj       References.
 *          b_disable_av_sync References.
 *
 * Outputs: - 
 *
 * Returns: SMR_OK                      Success.
 *          SMR_STRM_COMP_ID_NOT_FOUND  Fail.
 *          SMR_DRV_COMP_ERROR          Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_sess_gnrc_pcr_apply_av_sync( 
    HANDLE_T                      h_sess, 
    SM_SESS_GNRC_T*               pt_sess_obj,
    SM_SESS_GNRC_AV_SYNC_INFO_T*  pt_av_sync_info )
{
    INT32  i4_ret;
    SIZE_T z_get_size;
    BOOL   b_is_enable = FALSE;
    
    if ( pt_sess_obj->t_pcr_descr.h_comp == NULL_HANDLE )
    {
        return SMR_OK; /* ignore all AV SYNC control */
    }
#ifdef MW_TV_AV_BYPASS_SUPPORT
    else
    {
        CONN_STATE_T           at_conn_state[1];
        UINT16                 ui2_num_entries = 1;
        CONN_DIR_TYPE_T        ae_conn_dir[1];
        
        i4_ret = rm_get_conn_state( pt_sess_obj->t_pcr_descr.h_comp,
                                    pt_sess_obj->h_source,
                                    at_conn_state,
                                    &ui2_num_entries,
                                    ae_conn_dir);
        if ( i4_ret != RMR_OK )
        {
            return SMR_DRV_COMP_ERROR;
        }
        
        if ( !(at_conn_state[0].ui1_conn_state & CONN_STATE_CONNECTED) &&
             !(at_conn_state[0].ui1_conn_state & CONN_STATE_HARD_WIRED) )
        {
            return SMR_OK; /* ignore all AV SYNC control */
        }
    }
#endif
    
    if ( pt_av_sync_info->e_av_sync_type != SM_SESS_GNRC_AV_SYNC_OFF )
    {
        pt_sess_obj->b_disable_av_sync = FALSE;
        if ( pt_av_sync_info->e_av_sync_type == SM_SESS_GNRC_AV_SYNC_FORCE_PCR_PID )
        {
            pt_sess_obj->ui2_force_pid = pt_av_sync_info->u.t_force_pcr_pid;
        }
        else
        {
            pt_sess_obj->ui2_force_pid = MPEG_2_NULL_PID;
        }
        
        return sm_sess_gnrc_pcr_set_pid( h_sess, pt_sess_obj );
    }
    else /* Should disable AV SYNC */
    {
        pt_sess_obj->b_disable_av_sync = TRUE;
        
        z_get_size = sizeof( BOOL );
        i4_ret = rm_get( pt_sess_obj->t_pcr_descr.h_comp , FALSE, 0, 
                         DEMUX_GET_TYPE_CTRL, 
                         (VOID*)(UINT32)&b_is_enable, 
                         &z_get_size,
                         &pt_sess_obj->t_pcr_descr.e_comp_cond );
        if ( i4_ret != RMR_OK )
        {
           return SMR_DRV_COMP_ERROR;
        }
        
        if ( b_is_enable )
        {
            i4_ret = rm_set( pt_sess_obj->t_pcr_descr.h_comp , FALSE, 0, 
                             DEMUX_SET_TYPE_CTRL, 
                             (VOID*)FALSE, 
                             sizeof( BOOL ),
                             &pt_sess_obj->t_pcr_descr.e_comp_cond );
            if ( i4_ret != RMR_OK )
            {
               return SMR_DRV_COMP_ERROR;
            }
            SM_GS_DBG_INFO(( "[SM] Set PCR DISABLE to disable AV SYNC\r\n" ));
        }
    }
    
    return SMR_OK;
}
