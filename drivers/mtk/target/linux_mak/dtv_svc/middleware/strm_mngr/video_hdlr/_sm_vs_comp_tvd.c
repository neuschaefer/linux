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
 * $RCSfile: _sm_vs_comp_tvd.c,v $
 * $Revision: #2 $
 * $Date: 2012/11/03 $
 * $Author: xingju.wang $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/17 $
 * $SWAuthor: Alec Lu $
 * $MD5HEX: ff30432460a0575709cf2e578efcfb8d $
 *
 * Description:
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "dbg/x_dbg.h"
#include "dbg/dbg.h"
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "strm_mngr/x_sm.h"
#include "strm_mngr/_sm_utils.h"

#include "strm_mngr/video_hdlr/_sm_vs_state.h"
#include "strm_mngr/video_hdlr/_sm_video_hdlr.h"
#include "strm_mngr/video_hdlr/_sm_vs_comp_tvd.h"
#include "strm_mngr/sm_drv_map.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
static SELECT_T _rm_comp_sel_fct_tvd(
                    DRV_TYPE_T   e_type,
                    UINT16       ui2_id,
                    VOID*        pv_comp_sel_data,
                    const VOID*  pv_comp_data,
                    SIZE_T       z_comp_data_len,
                    UINT16       ui2_num_active,
                    UINT16       ui2_num_wait,
                    CTRL_TYPE_T  e_ctrl,
                    BOOL         b_would_conflict );

static VOID _rm_nfy_fct_tvd( HANDLE_T   h_obj,
                            UINT8      ui1_port,
                            RM_COND_T  e_nfy_cond,
                            VOID*      pv_tag,
                            UINT32     ui4_data);

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: _rm_comp_sel_fct_tvd
 *
 * Description: <function description>
 *
 * Inputs:  e_type           References.
 *          ui2_id           References.
 *          pv_comp_sel_data References.
 *          pv_comp_data     References.
 *          z_comp_data_len  References.
 *          b_in_use         References.
 *
 * Outputs: -
 *
 * Returns: SEL_COMP_BREAK  Success.
 *
 ----------------------------------------------------------------------------*/
static SELECT_T _rm_comp_sel_fct_tvd(
                    DRV_TYPE_T   e_type,
                    UINT16       ui2_id,
                    VOID*        pv_comp_sel_data,
                    const VOID*  pv_comp_data,
                    SIZE_T       z_comp_data_len,
                    UINT16       ui2_num_active,
                    UINT16       ui2_num_wait,
                    CTRL_TYPE_T  e_ctrl,
                    BOOL         b_would_conflict )
{
    SM_DRV_COMP_INFO_T     t_drv_descr = SM_DRV_COMP_INFO_INIT;

    t_drv_descr.e_type = e_type;
    t_drv_descr.ui2_comp_id = ui2_id;

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if ( sm_drv_map_exists( &t_drv_descr ) )
#else
    if ( sm_drv_map_exists( &t_drv_descr ) || b_would_conflict )
#endif
    {
        return SEL_NEXT;
    }

    return SEL_COMP_BREAK;
}

/*-----------------------------------------------------------------------------
 * Name: _rm_nfy_fct_tvd
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
static VOID _rm_nfy_fct_tvd( HANDLE_T   h_obj,
                            UINT8      ui1_port,
                            RM_COND_T  e_nfy_cond,
                            VOID*      pv_tag,
                            UINT32     ui4_data)
{
    SM_VIDEO_HDLR_MSG_T         t_msg;
    HANDLE_T                    h_stream = (HANDLE_T)pv_tag;
    RM_COND_T                   e_cond   = RM_COND_IGNORE;

    if ( !x_handle_valid( h_stream ) )
    {
        SM_VS_DBG_ERR((
            "[SM] VS, _rm_nfy_fct_tvd(), h_stream(0x%.8X) is invalid!\r\n", h_stream ));
        return;
    }
    /* 
      Return before send message to queue, because not handle the OPERATION info in sm_slctr, 
      except TV_DEC_SET_TYPE_STOP_CC and TV_DEC_SET_TYPE_CTRL.
    */
    e_cond = rm_cond_chg( e_nfy_cond,
                          RM_COND_IGNORE,
                          COND_GRP_OPERATION );
    if( e_cond != RM_COND_NO_CHG && 
        ui4_data != TV_DEC_SET_TYPE_STOP_CC &&
        ui4_data != TV_DEC_SET_TYPE_CTRL ) 
    {
        return;
    }

    t_msg.h_stream = h_stream;
    t_msg.e_msg_type = SM_VIDEO_HDLR_MSG_DEC_RM_NFY;

    t_msg.u.t_msg_rm.h_comp = h_obj;
    t_msg.u.t_msg_rm.ui1_port = ui1_port;
    t_msg.u.t_msg_rm.e_cond = e_nfy_cond;
    t_msg.u.t_msg_rm.ui4_data = ui4_data;
    t_msg.u.t_msg_rm.b_should_update_comp_descr = TRUE;

    sm_vs_send_msg( &t_msg );
}

/*-----------------------------------------------------------------------------
 * Name: _sm_vs_comp_tvd_ctrl_nfy
 *
 * Description: <function description>
 *
 * Inputs:  pv_nfy_tag References.
 *          e_nfy_cond References.
 *          ui4_data_1 References.
 *          ui4_data_2 References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_vs_comp_tvd_ctrl_nfy(
    VOID*               pv_nfy_tag,
    TV_DEC_COND_T          e_nfy_cond,
    UINT32              ui4_data_1,
    UINT32                ui4_data_2 )
{
    SM_VIDEO_HDLR_MSG_T         t_msg;
    HANDLE_T                    h_stream = (HANDLE_T)pv_nfy_tag;

    if ( !x_handle_valid( h_stream ) )
    {
        SM_VS_DBG_ERR((
            "[SM] VS, _sm_vs_comp_tvd_ctrl_nfy(), h_stream(0x%.8X) is invalid!\r\n", h_stream ));
        return;
    }

    t_msg.h_stream = h_stream;
    t_msg.e_msg_type = SM_VIDEO_HDLR_MSG_TVD_CTRL_NFY;

    t_msg.u.t_msg_tvd_ctrl.e_tvd_nfy_cond = e_nfy_cond;
    t_msg.u.t_msg_tvd_ctrl.ui4_data_1 = ui4_data_1;
    t_msg.u.t_msg_tvd_ctrl.ui4_data_2 = ui4_data_2;

    sm_vs_send_msg( &t_msg );
}

/*-----------------------------------------------------------------------------
 * Name: _sm_vs_comp_tvd_cc_nfy
 *
 * Description: <function description>
 *
 * Inputs:  pv_tag    References.
 *          e_cc_cond References.
 *          pv_arg    References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_vs_comp_tvd_cc_nfy (
    VOID*                   pv_tag,
    TV_DEC_CC_COND_T        e_cc_cond,
    VOID*                   pv_arg )
{
    HANDLE_T              h_stream = (HANDLE_T)pv_tag;
    SM_VIDEO_HDLR_MSG_T   t_msg;

    if ( !x_handle_valid( h_stream ) )
    {
        SM_VS_DBG_ERR((
            "[SM] VS, _sm_vs_comp_tvd_cc_nfy(), h_stream(0x%.8X) is invalid!\r\n", h_stream ));
        return;
    }

    t_msg.h_stream = h_stream;

    if ( e_cc_cond == TV_DEC_CC_COND_DATA_ARRIVAL )
    {
        TV_DEC_CC_DATA_T*  pt_vd_cc_data = (TV_DEC_CC_DATA_T*)pv_arg;

        if ( !pv_arg )
        {
            SM_VS_DBG_ERR(( "[SM] ERROR! _sm_vs_comp_tvd_cc_nfy() with NULL pv_arg\r\n" ));
            return;
        }

        t_msg.e_msg_type = SM_VIDEO_HDLR_MSG_TVD_CC_DATA;
        t_msg.u.t_msg_tvd_cc.b_even_field = pt_vd_cc_data->b_even_field;
        t_msg.u.t_msg_tvd_cc.ui1_byte1 = pt_vd_cc_data->ui1_byte1;
        t_msg.u.t_msg_tvd_cc.ui1_byte2 = pt_vd_cc_data->ui1_byte2;

        /* send message */
        sm_vs_send_msg( &t_msg );
    }
    else if ( e_cc_cond == TV_DEC_CC_COND_ERROR )
    {
        /* ignore */
    }
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: sm_vs_comp_tvd_open
 *
 * Description: <function description>
 *
 * Inputs:  h_stream      References.
 *          h_source      References.
 *          pt_comp_descr References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK              Success.
 *          SMR_DRV_COMP_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_comp_tvd_open( HANDLE_T            h_stream,
                          HANDLE_T            h_source,
                          SM_DRV_COMP_DESCR_T* pt_comp_descr )
{
    INT32 i4_ret;

    i4_ret = sm_drv_comp_open( h_source,
                               pt_comp_descr->t_info.e_type,
                               pt_comp_descr->t_info.ui2_comp_id,
                               pt_comp_descr->t_info.s_logical_name,
                               (VOID*)pt_comp_descr,
                               &_rm_comp_sel_fct_tvd,
                               (VOID*)h_stream,
                               &_rm_nfy_fct_tvd,
                               pt_comp_descr );
    if ( i4_ret != SMR_OK || pt_comp_descr->h_comp == NULL_HANDLE )
    {
        return SMR_DRV_COMP_NOT_FOUND;
    }

    /* Send RM notify message */
    {
        SM_VIDEO_HDLR_MSG_T         t_msg;

        t_msg.h_stream = h_stream;
        t_msg.e_msg_type = SM_VIDEO_HDLR_MSG_DEC_RM_NFY;

        t_msg.u.t_msg_rm.h_comp = pt_comp_descr->h_comp;
        t_msg.u.t_msg_rm.ui1_port = pt_comp_descr->t_info.ui1_in_port;
        t_msg.u.t_msg_rm.e_cond = pt_comp_descr->e_comp_cond;
        t_msg.u.t_msg_rm.ui4_data = 0;
        t_msg.u.t_msg_rm.b_should_update_comp_descr = FALSE;

        sm_vs_send_msg( &t_msg );
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_comp_tvd_connect
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_vs_obj References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK              Success.
 *          SMR_DRV_COMP_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_comp_tvd_connect( HANDLE_T            h_stream,
                              SM_VIDEO_STREAM_T*  pt_vs_obj )
{
    INT32             i4_ret;
    CONN_STATE_T      at_conn_state[1];
    UINT16            ui2_num_entries = 1;
    CONN_DIR_TYPE_T   ae_conn_dir[1];

    i4_ret = rm_get_conn_state( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                                pt_vs_obj->t_base.h_source,
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
        i4_ret = sm_drv_comp_connect( &pt_vs_obj->t_vs_comp_grp.t_dec,
                                      pt_vs_obj->t_base.h_source,
                                      (VOID*)h_stream );
        if ( i4_ret != SMR_OK )
        {
            return SMR_DRV_COMP_ERROR;
        }

        /* Send RM notify message */
        {
            SM_VIDEO_HDLR_MSG_T         t_msg;

            t_msg.h_stream = h_stream;
            t_msg.e_msg_type = SM_VIDEO_HDLR_MSG_DEC_RM_NFY;

            t_msg.u.t_msg_rm.h_comp = pt_vs_obj->t_vs_comp_grp.t_dec.h_comp;
            t_msg.u.t_msg_rm.ui1_port = pt_vs_obj->t_vs_comp_grp.t_dec.t_info.ui1_in_port;
            t_msg.u.t_msg_rm.e_cond = pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond;
            t_msg.u.t_msg_rm.ui4_data = 0;
            t_msg.u.t_msg_rm.b_should_update_comp_descr = FALSE;

            sm_vs_send_msg( &t_msg );
        }
    }
    else
    {
        sm_vs_send_msg_do_event( h_stream, SM_EVN_VS_DEC_CONNECTED, 0 );
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_comp_tvd_disconnect
 *
 * Description: <function description>
 *
 * Inputs:  pt_from_comp References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK              Success.
 *          SMR_DRV_COMP_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_comp_tvd_disconnect(
    HANDLE_T             h_stream,
    SM_DRV_COMP_DESCR_T* pt_from_comp )
{
    INT32                i4_ret = SMR_OK;
    RM_COND_T            e_cond = RM_COND_IGNORE;

    e_cond = rm_cond_chg( pt_from_comp->e_comp_cond,
                          RM_COND_IGNORE,
                          COND_GRP_CONN_ORIG );

    if ( e_cond == RM_COND_CONN_ORIGINATOR )
    {
        i4_ret = sm_drv_comp_disconnect(pt_from_comp);
        if ( i4_ret != SMR_OK )
        {
            return i4_ret;
        }

        /* Send RM notify message */
        {
            SM_VIDEO_HDLR_MSG_T         t_msg;

            t_msg.h_stream = h_stream;
            t_msg.e_msg_type = SM_VIDEO_HDLR_MSG_DEC_RM_NFY;

            t_msg.u.t_msg_rm.h_comp = pt_from_comp->h_comp;
            t_msg.u.t_msg_rm.ui1_port = pt_from_comp->t_info.ui1_in_port;
            t_msg.u.t_msg_rm.e_cond = pt_from_comp->e_comp_cond;
            t_msg.u.t_msg_rm.ui4_data = 0;
            t_msg.u.t_msg_rm.b_should_update_comp_descr = FALSE;

            sm_vs_send_msg( &t_msg );
        }
    }
    else
    {
        sm_vs_send_msg_do_event( h_stream, SM_EVN_VS_DEC_DISCONNECTED, 0 );
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_comp_tvd_set_callback
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_vs_obj References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sm_vs_comp_tvd_set_callback( HANDLE_T              h_stream,
                                 SM_VIDEO_STREAM_T*    pt_vs_obj )
{
    INT32                 i4_ret;
    TV_DEC_NFY_INFO_T     t_set_nfy_info;

    t_set_nfy_info.pf_tv_dec_nfy = _sm_vs_comp_tvd_ctrl_nfy;
    t_set_nfy_info.pv_tag = (VOID*)h_stream;

    if( x_handle_valid(pt_vs_obj->t_vs_comp_grp.t_dec.h_comp) == FALSE )
    {
        SM_VS_DBG_ERR((
            "[SM] VS, sm_vs_comp_tvd_set_callback(), handle(0x%.8X) is invalid!\r\n", 
            pt_vs_obj->t_vs_comp_grp.t_dec.h_comp ));
        return;
    }

    i4_ret = rm_set( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                  FALSE,
                  ANY_PORT_NUM,
                  TV_DEC_SET_TYPE_NFY_FCT,
                  (VOID*)&t_set_nfy_info,
                  sizeof(TV_DEC_NFY_INFO_T),
                  &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond );
    if ( i4_ret != RMR_OK )
    {
        x_dbg_stmt(
            "[Stress] [SM] VS, rm_set(TV_DEC_SET_TYPE_NFY_FCT) fail, i4_ret = %d\r\n",
            i4_ret);
        SM_ASSERT( 0 );
        return;
    }
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_comp_tvd_set_callback
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_vs_obj References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sm_vs_comp_tvd_play_cc(
    HANDLE_T             h_stream,
    SM_VIDEO_STREAM_T*   pt_vs_obj )
{
    INT32                 i4_ret;
    TV_DEC_CC_NFY_INFO_T  t_cc_nfy_info;

    if ( x_handle_valid(pt_vs_obj->t_vs_comp_grp.t_dec.h_comp) == FALSE )
    {
        SM_VS_DBG_ERR((
            "[SM] VS, sm_vs_comp_tvd_set_callback(), handle(0x%.8X) is invalid!\r\n", 
            pt_vs_obj->t_vs_comp_grp.t_dec.h_comp ));
        return;
    }

    t_cc_nfy_info.pf_cc_nfy = _sm_vs_comp_tvd_cc_nfy;
    t_cc_nfy_info.pv_tag    = (VOID*)h_stream;

    i4_ret = rm_set( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                  FALSE,
                  ANY_PORT_NUM,
                  TV_DEC_SET_TYPE_PLAY_CC,
                  (VOID*)&t_cc_nfy_info,
                  sizeof(TV_DEC_CC_NFY_INFO_T),
                  &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond );
    if ( i4_ret != RMR_OK )
    {
        x_dbg_stmt(
            "[SM] VS, rm_set(TV_DEC_SET_TYPE_PLAY_CC) fail, i4_ret = %d\r\n",
            i4_ret );
        SM_ASSERT( 0 );
        return;
    }
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_comp_tvd_set_callback
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_vs_obj References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sm_vs_comp_tvd_stop_cc(
    HANDLE_T             h_stream,
    SM_VIDEO_STREAM_T*   pt_vs_obj )
{
    INT32                 i4_ret;

    if ( x_handle_valid(pt_vs_obj->t_vs_comp_grp.t_dec.h_comp) == FALSE )
    {
        SM_VS_DBG_ERR((
            "[SM] VS, sm_vs_comp_tvd_set_callback(), handle(0x%.8X) is invalid!\r\n", 
            pt_vs_obj->t_vs_comp_grp.t_dec.h_comp ));
        return;
    }

    i4_ret = rm_set( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                  FALSE,
                  ANY_PORT_NUM,
                  TV_DEC_SET_TYPE_STOP_CC,
                  (VOID*)NULL,
                  0,
                  &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond );
    if ( i4_ret != RMR_OK )
    {
        x_dbg_stmt(
            "[SM] VS, rm_set(TV_DEC_SET_TYPE_STOP_CC) fail, i4_ret = %d\r\n",
            i4_ret );
        SM_ASSERT( 0 );
        return;
    }
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_comp_tvd_set_op_monitor
 *
 * Description: <function description>
 *
 * Inputs:  pt_vs_obj      References.
 *          ui4_data       References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK              Success.
 *          SMR_DRV_COMP_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_comp_tvd_set_op_monitor(
    SM_VIDEO_STREAM_T*   pt_vs_obj,
    UINT32               ui4_data )
{
    INT32 i4_ret;

    if ( x_handle_valid(pt_vs_obj->t_vs_comp_grp.t_dec.h_comp) == FALSE )
    {
        SM_VS_DBG_ERR((
            "[SM] VS, sm_vs_comp_tvd_set_op_monitor(), handle(0x%.8X) is invalid!\r\n", 
            pt_vs_obj->t_vs_comp_grp.t_dec.h_comp ));
        return SMR_DRV_COMP_ERROR;
    }

    if ( ui4_data == TV_DEC_SET_TYPE_STOP_CC &&
         pt_vs_obj->t_vbi_psr_id != (UINT32)0xFFFFFFFF &&
         pt_vs_obj->t_vbi_nfy.pf_vbi_nfy )
    {
        sm_vs_comp_tvd_play_cc(
            pt_vs_obj->t_base.h_stream,
            pt_vs_obj );
    }
    if ( ui4_data == TV_DEC_SET_TYPE_CTRL )
    {
        TV_DEC_CTRL_T  e_tvd_ctrl;
        SIZE_T         z_size = sizeof(TV_DEC_CTRL_T);

        /* Reset resolution if anyother MW component STOP TV Decoder,
              to avoid resolution nofitication been skipped by change check */
        i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                      FALSE,
                      ANY_PORT_NUM,
                      TV_DEC_GET_TYPE_CTRL,
                      (VOID*)&e_tvd_ctrl,
                      &z_size,
                      &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond );
        if ( i4_ret != RMR_OK )
        {
            SM_VS_DBG_ERR((
                "[SM] VS, rm_get(TV_DEC_GET_TYPE_CTRL) fail, i4_ret = %d\r\n",
                i4_ret ));
            SM_ASSERT( 0 );
            return SMR_OK; /* ignore */
        }
        if ( e_tvd_ctrl == TV_DEC_CTRL_STOP )
        {
            pt_vs_obj->e_tv_signal = TV_DEC_TV_SIG_UNKNOWN;
            x_memset( &pt_vs_obj->t_vid_res, 0, sizeof(pt_vs_obj->t_vid_res) );
        }
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_comp_tvd_color_sys_2_vsh
 *
 * Description: <function description>
 *
 * Inputs:  e_color_sys  References.
 *
 * Outputs: -
 *
 * Returns: COLOR_SYS_T.
 *
 ----------------------------------------------------------------------------*/
COLOR_SYS_T sm_vs_comp_tvd_color_sys_2_vsh(
    TV_DEC_COLOR_SYS_T   e_color_sys )
{
    switch( e_color_sys )
    {
    case TV_DEC_COLOR_SYS_NTSC:
        return COLOR_SYS_NTSC;
    case TV_DEC_COLOR_SYS_NTSC_443:
        return COLOR_SYS_NTSC_443;

    case TV_DEC_COLOR_SYS_PAL:
        return COLOR_SYS_PAL;
    case TV_DEC_COLOR_SYS_PAL_N:
        return COLOR_SYS_PAL_N;
    case TV_DEC_COLOR_SYS_PAL_M:
        return COLOR_SYS_PAL_M;
    case TV_DEC_COLOR_SYS_PAL_60:
        return COLOR_SYS_PAL_60;

    case TV_DEC_COLOR_SYS_SECAM:
        return COLOR_SYS_SECAM;

    case TV_DEC_COLOR_SYS_UNKNOWN:
    default:
        return COLOR_SYS_UNKNOWN;
    }
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_comp_tvd_aud_protect_info_2_vsh
 *
 * Description: <function description>
 *
 * Inputs:  e_drv_aud_protect_info  References.
 *
 * Outputs: -
 *
 * Returns: VSH_AUD_PROTECT_INFO_T
 *
 ----------------------------------------------------------------------------*/
VSH_AUD_PROTECT_INFO_T sm_vs_comp_tvd_aud_protect_info_2_vsh(
    TV_DEC_AUD_PROTECT_INFO_T   e_drv_aud_protect_info )
{
    switch( e_drv_aud_protect_info )
    {
    case TV_DEC_AUD_PROTECT_INFO_DMUTE_AMUTE:
        return VSH_AUD_PROTECT_INFO_DMUTE_AMUTE;

    case TV_DEC_AUD_PROTECT_INFO_DUNMUTE_AMUTE:
        return VSH_AUD_PROTECT_INFO_DUNMUTE_AMUTE;

    case TV_DEC_AUD_PROTECT_INFO_DUNMUTE_AUNMUTE:
        return VSH_AUD_PROTECT_INFO_DUNMUTE_AUNMUTE;

    case TV_DEC_AUD_PROTECT_INFO_DMUTE_AUNMUTE:
        return VSH_AUD_PROTECT_INFO_DMUTE_AUNMUTE;

    default:
        return VSH_AUD_PROTECT_INFO_UNKNOWN;
    }
}

