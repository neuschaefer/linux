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
 * $RCSfile: _sm_vs_state.c,v $
 * $Revision: #3 $
 * $Date: 2012/07/20 $
 * $Author: jiangbo.wang $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/DTV_X_ATSC/5 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: 8de1bd3424b086407b1e2b8a116c87fc $
 *
 * Description:
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "scdb/x_scdb_api.h"

#include "conn_mngr/x_cm.h"

#include "res_mngr/rm.h"
#include "res_mngr/drv/x_vid_dec.h"
#include "res_mngr/drv/x_vid_plane.h"
#include "res_mngr/drv/x_plane_mxr.h"
#include "res_mngr/drv/x_tv_enc.h"

#include "strm_mngr/sess_gnrc_hdlr/x_sm_sess_gnrc.h"
#include "strm_mngr/video_hdlr/_sm_video_hdlr.h"
#include "strm_mngr/video_hdlr/_sm_vs_state.h"
#include "strm_mngr/video_hdlr/_sm_vs_comp_vd.h"
#include "strm_mngr/video_hdlr/_sm_vs_comp_tvd.h"
#include "strm_mngr/video_hdlr/_sm_vs_comp_vp.h"
#include "strm_mngr/sm_drv_map.h"
#include "strm_mngr/_sm_utils.h"
#include "time_msrt/x_time_msrt.h"

#ifdef DEBUG
#include "strm_mngr/sm_cli.h"
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#ifdef TIME_MEASUREMENT
#define VS_HANDLE_IGNORE_2_OPEN_STR            "VS:IGNORE-OPENED"
#define VS_HANDLE_IGNORE_2_STARTED_STR         "VS:IGNORE-STARTED"
#define VS_HANDLE_STARTED_2_STOPPED_STR        "VS:STARTED-STOPPED"
#define VS_HANDLE_STARTED_2_CLOSED_STR         "VS:STARTED-CLOSED"

#define VS_HANDLE_PM_OPENED                    "VS:PM-OPENED"
#define VS_HANDLE_PM_STARTED                   "VS:PM-STARTED"
#define VS_HANDLE_PM_CLOSED                    "VS:PM-CLOSED"

#endif /* TIME_MEASUREMENT */


#define TVE_NOT_EXIST_OR_LAST_EVENT_IS( pt_vs_obj, event )                   \
    ( pt_vs_obj->t_vs_comp_grp.t_tve.h_comp == NULL_HANDLE ||                \
      pt_vs_obj->ui4_lastest_tve_event == event )

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
static INT32 _sm_vs_state_vid_drv_play(
    HANDLE_T              h_stream,
    SM_VIDEO_STREAM_T*    pt_vs_obj );

static INT32 _sm_vs_state_vid_drv_stop(
    HANDLE_T              h_stream,
    SM_VIDEO_STREAM_T*    pt_vs_obj );


/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

#ifdef TIME_MEASUREMENT
static VOID _sm_vs_state_time_msrt(
    UINT32      ui4_curr_state,
    UINT32      ui4_next_state,
    UINT32      ui4_event )
{
    if ( ui4_curr_state == SM_COND_IGNORE &&
         ui4_next_state == SM_COND_OPENING )
    {
        TMS_BEGIN( VS_HANDLE_IGNORE_2_OPEN_STR );
        TMS_BEGIN_EX(TMS_FLAG_CHG_CHL, VS_HANDLE_IGNORE_2_OPEN_STR, VS_HANDLE_IGNORE_2_OPEN_STR); 
        TMS_BEGIN( VS_HANDLE_IGNORE_2_STARTED_STR );
        TMS_BEGIN_EX(TMS_FLAG_CHG_CHL, VS_HANDLE_IGNORE_2_STARTED_STR, VS_HANDLE_IGNORE_2_STARTED_STR); 
    }
    else if ( ui4_curr_state == SM_COND_OPENING &&
              ui4_next_state == SM_COND_OPENED )
    {
        TMS_END( VS_HANDLE_IGNORE_2_OPEN_STR );
        TMS_END_EX(TMS_FLAG_CHG_CHL, VS_HANDLE_IGNORE_2_OPEN_STR, VS_HANDLE_IGNORE_2_OPEN_STR);
    }
    else if ( ui4_curr_state == SM_COND_STARTING &&
              ui4_next_state == SM_COND_STARTED )
    {
        TMS_END( VS_HANDLE_IGNORE_2_STARTED_STR );
        TMS_END_EX(TMS_FLAG_CHG_CHL, VS_HANDLE_IGNORE_2_STARTED_STR, VS_HANDLE_IGNORE_2_STARTED_STR);
    }
    else if ( ui4_curr_state == SM_COND_STARTED &&
              ui4_next_state == SM_COND_STOPPING )
    {
        TMS_BEGIN( VS_HANDLE_STARTED_2_STOPPED_STR );
        TMS_BEGIN_EX(TMS_FLAG_CHG_CHL, VS_HANDLE_STARTED_2_STOPPED_STR, VS_HANDLE_STARTED_2_STOPPED_STR); 
        TMS_BEGIN( VS_HANDLE_STARTED_2_CLOSED_STR );
        TMS_BEGIN_EX(TMS_FLAG_CHG_CHL, VS_HANDLE_STARTED_2_CLOSED_STR, VS_HANDLE_STARTED_2_CLOSED_STR); 
    }
    else if ( ui4_curr_state == SM_COND_STOPPING &&
              ui4_next_state == SM_COND_OPENED )
    {
        TMS_END( VS_HANDLE_STARTED_2_STOPPED_STR );
        TMS_END_EX(TMS_FLAG_CHG_CHL, VS_HANDLE_STARTED_2_STOPPED_STR, VS_HANDLE_STARTED_2_STOPPED_STR);
    }
    else if ( ui4_curr_state == SM_COND_CLOSING &&
              ui4_next_state == SM_COND_CLOSED )
    {
        TMS_END( VS_HANDLE_STARTED_2_CLOSED_STR );
        TMS_END_EX(TMS_FLAG_CHG_CHL, VS_HANDLE_STARTED_2_CLOSED_STR, VS_HANDLE_STARTED_2_CLOSED_STR);
    }
}

/*-----------------------------------------------------------------------------
 * Name: _sm_vs_drv_time_msrt
 *
 * Description: <function description>
 *
 * Inputs:  ui4_event   References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_vs_drv_time_msrt( UINT32 ui4_event )
{

    switch( ui4_event )
    {
    case SM_EVN_VS_DEC_OPENED:
        TMS_END( VS_HANDLE_VD_OPENED );
        break;

    case SM_EVN_VS_DEC_CLOSED:
        TMS_END( VS_HANDLE_VD_CLOSED );
        break;

    case SM_EVN_VS_DEC_CONNECTED:
        TMS_END( VS_HANDLE_VD_CONNECTED );
        break;

    case SM_EVN_VS_DEC_DISCONNECTED:
        TMS_END( VS_HANDLE_VD_DISCONNECTED );
        break;

    case SM_EVN_VS_DEC_PLAYED:
        TMS_END( VS_HANDLE_VD_STARTED );
        break;

    case SM_EVN_VS_DEC_STOPPED:
        TMS_END( VS_HANDLE_VD_STOPPED );
        break;

    case SM_EVN_VS_VP_OPENED:
        TMS_END( VS_HANDLE_VP_OPENED );
        break;

    case SM_EVN_VS_VP_CLOSED:
        TMS_END( VS_HANDLE_VP_CLOSED );
        break;

    case SM_EVN_VS_VP_CONNECTED:
        TMS_END( VS_HANDLE_VP_CONNECTED );
        break;

    case SM_EVN_VS_VP_DISCONNECTED:
        TMS_END( VS_HANDLE_VP_DISCONNECTED );
        break;

    case SM_EVN_VS_VP_PLAYED:
        TMS_END( VS_HANDLE_VP_STARTED );
        break;

    case SM_EVN_VS_VP_STOPPED:
        TMS_END( VS_HANDLE_VP_STOPPED );
        break;

    case SM_EVN_VS_PMX_OPENED:
        TMS_END( VS_HANDLE_PMX_OPENED );
        break;

    case SM_EVN_VS_PMX_CLOSED:
        TMS_END( VS_HANDLE_PMX_CLOSED );
        break;

    case SM_EVN_VS_PMX_CONNECTED:
        TMS_END( VS_HANDLE_PMX_CONNECTED );
        break;

    case SM_EVN_VS_PMX_DISCONNECTED:
        TMS_END( VS_HANDLE_PMX_DISCONNECTED );
        break;

    case SM_EVN_VS_TVE_OPENED:
        TMS_END( VS_HANDLE_TVE_OPENED );
        break;

    case SM_EVN_VS_TVE_CLOSED:
        TMS_END( VS_HANDLE_TVE_CLOSED );
        break;

    case SM_EVN_VS_TVE_CONNECTED:
        TMS_END( VS_HANDLE_TVE_CONNECTED );
        break;

    case SM_EVN_VS_TVE_DISCONNECTED:
        TMS_END( VS_HANDLE_TVE_DISCONNECTED );
        break;

    default:
        break;
    }
}

#endif /* TIME_MEASUREMENT */

/*-----------------------------------------------------------------------------
 * Name: _sm_vs_pm_ca_cond_2_vsh
 *
 * Description: <function description>
 *
 * Inputs:  e_ca_cond   References.
 *
 * Outputs: -
 *
 * Returns: VSH defined CA events
 *
 ----------------------------------------------------------------------------*/
static UINT32 _sm_vs_pm_ca_cond_2_vsh( PM_CA_COND_T e_ca_cond )
{
    switch( e_ca_cond )
    {
    case PM_CA_COND_CLOSED:
        return SM_EVN_CA_COND_CLOSED;

    case PM_CA_COND_UNKNOWN:
        return SM_EVN_CA_COND_UNKNOWN;

    case PM_CA_COND_FREE:
        return SM_EVN_CA_COND_FREE;

    case PM_CA_COND_AUTHORIZED:
        return SM_EVN_CA_COND_AUTHORIZED;

    case PM_CA_COND_AUTHORIZED_CA_SYS_ID:
        return SM_EVN_CA_COND_AUTHORIZED_CA_SYS_ID;

    case PM_CA_COND_NOT_AUTHORIZED:
        return SM_EVN_CA_COND_NOT_AUTHORIZED;

    default:
        return SM_EVN_CA_COND_CLOSED;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _sm_vs_state_prc_ca_event
 *
 * Description: <function description>
 *
 * Inputs:  pt_vs_obj   References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_vs_state_prc_ca_event( SM_VIDEO_STREAM_T*    pt_vs_obj )
{
    SM_VS_STATE_TAG_T     t_state_tag;

    t_state_tag.h_stream = pt_vs_obj->t_base.h_stream;
    t_state_tag.pt_vs_obj = pt_vs_obj;

    switch( pt_vs_obj->ui4_lastest_ca_event )
    {
    case SM_EVN_CA_COND_CLOSED:
    case SM_EVN_CA_COND_UNKNOWN:
    case SM_EVN_CA_COND_FREE:
    case SM_EVN_CA_COND_AUTHORIZED:
    case SM_EVN_CA_COND_AUTHORIZED_CA_SYS_ID:
        /*_sm_vs_state_vid_drv_play(pt_vs_obj->t_base.h_stream, pt_vs_obj);*/
        break;

    case SM_EVN_CA_COND_NOT_AUTHORIZED:
        /*_sm_vs_state_vid_drv_stop(pt_vs_obj->t_base.h_stream, pt_vs_obj);*/
        break;

    default:
        SM_VS_DBG_INFO((
             "[SM] VS(0x%.8X), ERROR! Unknown CA event %u.\r\n",
             (UINT32)pt_vs_obj->t_base.h_stream,
             pt_vs_obj->ui4_lastest_ca_event ));
        return;
    }

    sm_vs_state_notify_app(
        &t_state_tag,
        (SM_COND_T)pt_vs_obj->t_state.ui4_curr_state,
        pt_vs_obj->ui4_lastest_ca_event,
        pt_vs_obj->ui4_lastest_ca_detail_code );
}

/*-----------------------------------------------------------------------------
 * Name: _sm_vs_state_scdb_nfy_fct
 *
 * Description: <function description>
 *
 * Inputs:  h_scdb      References.
 *          e_cond      References.
 *          ui4_reason  References.
 *          pv_tag      References.
 *          ui4_data    References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_vs_state_scdb_nfy_fct(
    HANDLE_T        h_scdb,
    SCDB_COND_T     e_cond,
    UINT32          ui4_reason,
    VOID*           pv_tag,
    UINT32          ui4_data )
{
    SM_VIDEO_HDLR_MSG_T              t_msg;
    HANDLE_T                         h_stream = (HANDLE_T)pv_tag;

    if ( !x_handle_valid( h_stream ) )
    {
        return;
    }
    
    t_msg.e_msg_type = SM_VIDEO_HDLR_MSG_SCDB_NFY;
    t_msg.h_stream = h_stream;
    t_msg.u.t_msg_scdb.e_scdb_cond = e_cond;
    t_msg.u.t_msg_scdb.ui4_reason = ui4_reason;
    t_msg.u.t_msg_scdb.ui4_data = ui4_data;
    if ((e_cond == SCDB_UPDATED && (ui4_reason & (SCDB_RECORD_DEL | SCDB_RECORD_MOD)) != 0 ) ||
        (e_cond == SCDB_CLOSED))
    {
       sm_vs_send_msg( &t_msg );
    }
}

/*-----------------------------------------------------------------------------
 * Name: _sm_vs_cp_nfy
 *
 * Description: <function description>
 *
 * Inputs:  h_component References.
 *          pv_cci      References.
 *          pv_tag      References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_vs_cp_nfy(
    HANDLE_T        h_component,
    CP_INFO_LIST_T  t_cp_info,
    VOID*           pv_tag )
{
    CP_INFO_LIST_T*      pt_cp_info_lst = (CP_INFO_LIST_T*)&t_cp_info;
    SM_VIDEO_HDLR_MSG_T  t_msg;

    if ( h_component==NULL_HANDLE || !pv_tag )
    {
        return;
    }

    t_msg.e_msg_type = SM_VIDEO_HDLR_MSG_CCI_NFY;
    t_msg.h_stream = (HANDLE_T)pv_tag;
    t_msg.u.t_msg_cci.h_component = h_component;
    t_msg.u.t_msg_cci.t_pm_cp_info_lst.ui1_ctrl_size = pt_cp_info_lst->ui1_ctrl_size;

    if ( t_msg.u.t_msg_cci.t_pm_cp_info_lst.ui1_ctrl_size > CP_TYPE_MAX )
    {
        SM_VS_DBG_ERR((
            "[SM] VS, %s:%d, ERROR! _sm_vs_cp_nfy(0x%.8X) with invalid size %u!\r\n",
            __FILE__, __LINE__, (UINT32)t_msg.h_stream,
            t_msg.u.t_msg_cci.t_pm_cp_info_lst.ui1_ctrl_size));
        return;
    }

    x_memcpy( t_msg.u.t_msg_cci.t_pm_cp_info_lst.at_cp_ctrl_list,
              pt_cp_info_lst->at_cp_ctrl_list,
              sizeof( CP_INFO_T ) * pt_cp_info_lst->ui1_ctrl_size );

    sm_vs_send_msg( &t_msg );
}

/*-----------------------------------------------------------------------------
 * Name: _sm_vs_state_pm_ca_nfy
 *
 * Description: Callback function for Process Manager to notify CA information.
 *
 * Inputs:  h_component     References.
 *          e_nfy_cond      References.
 *          ui4_detail_code References.
 *          pv_nfy_tag      References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_vs_state_pm_ca_nfy(
    HANDLE_T       h_component,
    PM_CA_COND_T   e_nfy_cond,
    UINT32         ui4_detail_code,
    VOID*          pv_nfy_tag)
{
    SM_VIDEO_HDLR_MSG_T         t_msg;
    HANDLE_T                    h_stream = (HANDLE_T)pv_nfy_tag;

    if ( !x_handle_valid( h_stream ) )
    {
        return;
    }

    t_msg.u.t_msg_ca.ui4_ca_event = _sm_vs_pm_ca_cond_2_vsh( e_nfy_cond );

    t_msg.h_stream = h_stream;
    t_msg.e_msg_type = SM_VIDEO_HDLR_MSG_CA_NFY;
    t_msg.u.t_msg_ca.ui4_detail_code = ui4_detail_code;

    sm_vs_send_msg( &t_msg );
}

/*-----------------------------------------------------------------------------
 * Name: _sm_vs_state_pm_nfy
 *
 * Description: Callback function for Process Manager to notify component
 *              information.
 *
 * Inputs:  h_component     References.
 *          e_nfy_cond      References.
 *          ui4_detail_code References.
 *          pv_nfy_tag      References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_vs_state_pm_nfy(
    HANDLE_T    h_component,
    PM_COND_T   e_nfy_cond,
    UINT32      ui4_detail_code,
    VOID*       pv_nfy_tag)
{
    SM_VIDEO_HDLR_MSG_T         t_msg;
    HANDLE_T                    h_stream = (HANDLE_T)pv_nfy_tag;

    if ( !x_handle_valid( h_stream ) )
    {
        return;
    }

    switch( e_nfy_cond )
    {
    case PM_COND_CLOSED:
        t_msg.u.t_msg_pm.ui4_pm_event = SM_EVN_VS_PM_COND_CLOSED;
        break;

    case PM_COND_PLAYED:
        t_msg.u.t_msg_pm.ui4_pm_event = SM_EVN_VS_PM_COND_PLAYED;
        break;

    case PM_COND_STOPPED:
        t_msg.u.t_msg_pm.ui4_pm_event = SM_EVN_VS_PM_COND_STOPPED;
        break;

    case PM_COND_OPENING:
    case PM_COND_STOPPING:
    case PM_COND_CLOSING:
    case PM_COND_PLAYING:
    default:
        return;
    }

    t_msg.h_stream = h_stream;
    t_msg.e_msg_type = SM_VIDEO_HDLR_MSG_PM_NFY;
    t_msg.u.t_msg_pm.ui4_detail_code = ui4_detail_code;

    sm_vs_send_msg( &t_msg );
}


/*-----------------------------------------------------------------------------
 * Name: _sm_vs_state_vid_drvs_connect
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_vs_obj References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _sm_vs_state_vid_drvs_connect(
    HANDLE_T            h_stream,
    SM_VIDEO_STREAM_T*  pt_vs_obj )
{
    INT32                 i4_ret;

    /* connect */
    i4_ret = sm_vs_comp_grp_connect( h_stream, &pt_vs_obj->t_vs_comp_grp );
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }

    TMS_BEGIN( VS_HANDLE_VD_CONNECTED );
    if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
    {
        /* connect Video Decoder to Demux */
        i4_ret = sm_vs_comp_vd_connect( h_stream, pt_vs_obj );
    }
    else
    {
        /* connect TV Decoder to analog source */
        i4_ret = sm_vs_comp_tvd_connect( h_stream, pt_vs_obj );
    }
    if ( i4_ret != SMR_OK )
    {
        sm_vs_comp_grp_disconnect( h_stream, &pt_vs_obj->t_vs_comp_grp );
        return i4_ret;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _sm_vs_state_vid_drv_set_pcr
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_vs_obj References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _sm_vs_state_vid_drv_set_pcr(
    HANDLE_T            h_stream,
    SM_VIDEO_STREAM_T*  pt_vs_obj )
{
    INT32                 i4_ret;
    UINT16                ui2_pcr;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    RM_COND_T             e_cond = RM_COND_IGNORE;

    e_cond = rm_cond_chg( pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond,
                          RM_COND_IGNORE,
                          COND_GRP_CONN_ORIG );
    if ( e_cond == RM_COND_CONN_ORIGINATOR )
#endif
    {
        ui2_pcr = pt_vs_obj->t_base.ui2_pcr_id;

        if ( ui2_pcr != ANY_COMP_ID )
        {
            /* Set PCR ID to Video Decoder. */
            i4_ret = rm_set( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                             FALSE,
                             ANY_PORT_NUM,
                             VID_DEC_SET_TYPE_PCR_ID,
                             (VOID*)((UINT32)ui2_pcr),
                             sizeof(UINT16),
                             &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond);
            if ( i4_ret != RMR_OK )
            {
                return SMR_DRV_COMP_ERROR;
            }
            SM_VS_DBG_INFO(( "[SM] VS, Set PCR ID %u\r\n", ui2_pcr ));
        }
    }

    return SMR_OK;
}

/* for 3D video */
/*-----------------------------------------------------------------------------
 * Name: _sm_vs_state_vid_drv_set_rd3d_info
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_vs_obj References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _sm_vs_state_vid_drv_set_rd3d_info(
    HANDLE_T            h_stream,
    SM_VIDEO_STREAM_T*  pt_vs_obj )
{
    INT32                   i4_ret;
    VID_DEC_RD3D_REG_DESC_T t_rd3d_info;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    RM_COND_T             e_cond = RM_COND_IGNORE;

    e_cond = rm_cond_chg( pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond,
                          RM_COND_IGNORE,
                          COND_GRP_CONN_ORIG );
    if ( e_cond == RM_COND_CONN_ORIGINATOR )
#endif
    {
        if (!pt_vs_obj->t_rd3d_info.b_rd3d) /* double check */
        {
            return SMR_OK;
        }

        switch (pt_vs_obj->t_rd3d_info.e_layer_type)
        {
        case REALD_BASE_LAYER:
            t_rd3d_info.e_layer_type = VD_REALD_BASE_LAYER;
            break;
            
        case REALD_ENHANCEMENT_LAYER:
        default:
            t_rd3d_info.e_layer_type = VD_REALD_ENHANCEMENT_LAYER;
            break;
        }
        
        switch (pt_vs_obj->t_rd3d_info.e_coding_method)
        {
        case CRITICALLY_SAMPLED_METHOD:
            t_rd3d_info.e_coding_method = VD_CRITICALLY_SAMPLED_METHOD;
            break;
            
        case NON_CRITICALLY_SAMPLED_METHOD:
        default:
            t_rd3d_info.e_coding_method = VD_NON_CRITICALLY_SAMPLED_METHOD;
            break;
        }
        
        /* Set RD3D info to Video Decoder. */
        i4_ret = rm_set( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                         FALSE,
                         ANY_PORT_NUM,
                         VID_DEC_SET_TYPE_RD3D_REG_DESC,
                         (VOID*)(&t_rd3d_info),
                         sizeof(VID_DEC_RD3D_REG_DESC_T),
                         &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond);
        if ( i4_ret != RMR_OK )
        {
            return SMR_DRV_COMP_ERROR;
        }
        SM_VS_DBG_INFO(( "[SM] VS, Set RD3D OK! layer type=%u, coding_method=%u\r\n", 
                         t_rd3d_info.e_layer_type, t_rd3d_info.e_coding_method ));
    }
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _sm_vs_state_vid_drv_set_mvc_ext_desc
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_vs_obj References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _sm_vs_state_vid_drv_set_mvc_ext_desc(
    HANDLE_T            h_stream,
    SM_VIDEO_STREAM_T*  pt_vs_obj )
{
    INT32                   i4_ret;
    VID_DEC_MVC_EXT_DESC_T  t_mvc_ext_info;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    RM_COND_T             e_cond = RM_COND_IGNORE;

    e_cond = rm_cond_chg( pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond,
                          RM_COND_IGNORE,
                          COND_GRP_CONN_ORIG );
    if ( e_cond == RM_COND_CONN_ORIGINATOR )
#endif
    {
        if (!pt_vs_obj->t_mvc_ext.b_mvc_video) /* double check */
        {
            return SMR_OK;
        }

        t_mvc_ext_info.ui2_avg_bit_rate = pt_vs_obj->t_mvc_ext.ui2_avg_bit_rate;
        t_mvc_ext_info.ui2_max_bit_rate = pt_vs_obj->t_mvc_ext.ui2_max_bit_rate;
        t_mvc_ext_info.ui2_view_ord_idx_min = pt_vs_obj->t_mvc_ext.ui2_view_ord_idx_min;
        t_mvc_ext_info.ui2_view_ord_idx_max = pt_vs_obj->t_mvc_ext.ui2_view_ord_idx_max;
        t_mvc_ext_info.ui1_temporal_id_start = pt_vs_obj->t_mvc_ext.ui1_temporal_id_start;
        t_mvc_ext_info.ui1_temporal_id_end = pt_vs_obj->t_mvc_ext.ui1_temporal_id_end;
        t_mvc_ext_info.b_no_sei_nal_present = pt_vs_obj->t_mvc_ext.b_no_sei_nal_present;
        t_mvc_ext_info.b_no_prefix_nal_present = pt_vs_obj->t_mvc_ext.b_no_prefix_nal_present;
        
        /* Set MVC_EXT_DESC info to Video Decoder. */
        i4_ret = rm_set( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                         FALSE,
                         ANY_PORT_NUM,
                         VID_DEC_SET_TYPE_MVC_EXT_DESC,
                         (VOID*)(&t_mvc_ext_info),
                         sizeof(VID_DEC_MVC_EXT_DESC_T),
                         &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond);
        if ( i4_ret != RMR_OK )
        {
            return SMR_DRV_COMP_ERROR;
        }
        SM_VS_DBG_INFO(( "[SM] VS, Set MVC_EXT_DESC OK! \r\n" ));
    }
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _sm_vs_state_vid_drv_init
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_vs_obj References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _sm_vs_state_vid_drv_init(
    HANDLE_T            h_stream,
    SM_VIDEO_STREAM_T*  pt_vs_obj )
{
    INT32                 i4_ret;
    SM_HANDLER_T*         pt_hdlr_obj = NULL;

    /*************************************************************************
       If it's the first time entering SM_COND_OPENED state,
       1. Set display and source region.
     ************************************************************************/

    if ( IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
    {
        /* for 3D video */
        /* set 3D info before set codec for convenient communication between vdec and swdmx */
        if (pt_vs_obj->t_rd3d_info.b_rd3d)
        {
            /*set RD3D info to video decoder */
            i4_ret = _sm_vs_state_vid_drv_set_rd3d_info( h_stream, pt_vs_obj );
            if ( i4_ret != RMR_OK )
            {
                /* only show log info when set rd3d fail */
                SM_VS_DBG_INFO(( "[SM] VS, %s:%d INFO _sm_vs_state_vid_drv_set_rd3d_info() fail, i4_ret = %d\r\n",
                                  __FILE__, __LINE__, i4_ret ));
            } 
        }
        if (pt_vs_obj->t_mvc_ext.b_mvc_video)
        {
            /*set mvc_ext info to video decoder */
            i4_ret = _sm_vs_state_vid_drv_set_mvc_ext_desc( h_stream, pt_vs_obj );
            if ( i4_ret != RMR_OK )
            {
                /* only show log info when set mvc_ext_desc fail */
                SM_VS_DBG_INFO(( "[SM] VS, %s:%d INFO _sm_vs_state_vid_drv_set_mvc_ext_desc() fail, i4_ret = %d\r\n",
                                  __FILE__, __LINE__, i4_ret ));
            }
        }
        
        i4_ret = sm_vs_comp_vd_set_codec( h_stream, pt_vs_obj );
        if ( i4_ret < SMR_OK )
        {
            SM_VS_DBG_ERR(( "[SM] VS, %s,%d, Error, sm_vs_comp_vd_set_codec() fail, i4_ret = %d\r\n",
                             __FILE__, __LINE__, i4_ret ));
            return i4_ret;
        }
    }

#if 0
    i4_ret = x_sm_set( h_stream,
                       SM_VSH_SET_TYPE_DISP_REGION,
                       (VOID*)&pt_vs_obj->t_disp_region,
                       sizeof( VSH_REGION_INFO_T ) );
#else
    i4_ret = handle_get_obj( pt_vs_obj->t_base.h_handler,
                             (VOID**)&pt_hdlr_obj );
            
    i4_ret = pt_hdlr_obj->t_fct_tbl.pf_set( h_stream,
                                            SM_VSH_SET_TYPE_DISP_REGION,
                                            (VOID*)&pt_vs_obj->t_disp_region,
                                            sizeof( VSH_REGION_INFO_T ) );
#endif
    if ( i4_ret < SMR_OK )
    {
        SM_VS_DBG_ERR(( "[SM] VS, _sm_vs_state_vid_drv_init(), set SM_VSH_SET_TYPE_DISP_REGION fail, i4_ret = %d\r\n",
                         i4_ret ));
        
        return i4_ret;
    }

#if 0
    i4_ret = x_sm_set( h_stream,
                       SM_VSH_SET_TYPE_VIDEO_REGION,
                       (VOID*)&pt_vs_obj->t_vid_region,
                       sizeof( VSH_REGION_INFO_T ) );
#else
    i4_ret = pt_hdlr_obj->t_fct_tbl.pf_set( h_stream,
                                            SM_VSH_SET_TYPE_VIDEO_REGION,
                                            (VOID*)&pt_vs_obj->t_vid_region,
                                            sizeof( VSH_REGION_INFO_T ) );
#endif    
    if ( i4_ret < SMR_OK )
    {
        SM_VS_DBG_ERR(( "[SM] VS, _sm_vs_state_vid_drv_init(), set SM_VSH_SET_TYPE_VIDEO_REGION fail, i4_ret = %d\r\n",
                         i4_ret ));

        return i4_ret;
    }

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _sm_vs_state_vid_drv_play
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
 *          SMR_INV_HANDLE      Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 _sm_vs_state_vid_drv_play(
    HANDLE_T              h_stream,
    SM_VIDEO_STREAM_T*    pt_vs_obj )
{
    INT32                 i4_ret;
    SM_SESSION_T*         pt_sess_obj = NULL;
#if 0
#ifdef MW_TV_AV_BYPASS_SUPPORT
    RM_COND_T             e_cond = RM_COND_IGNORE;
#endif
#endif

    pt_sess_obj = (SM_SESSION_T*)sm_get_sess_obj( pt_vs_obj->t_base.h_sess );
    if ( !pt_sess_obj )
    {
        return SMR_INV_HANDLE;
    }

    /* TV Encoder */
    if( x_handle_valid( pt_vs_obj->t_vs_comp_grp.t_tve.h_comp ) == TRUE )
    {
        i4_ret = rm_set( pt_vs_obj->t_vs_comp_grp.t_tve.h_comp,
                         FALSE,
                         ANY_PORT_NUM,
                         TV_ENC_SET_TYPE_CTRL,
                         (VOID*)TV_ENC_CTRL_ENABLE,
                         sizeof(TV_ENC_CTRL_T),
                         &pt_vs_obj->t_vs_comp_grp.t_tve.e_comp_cond);
        if ( i4_ret != RMR_OK )
        {
            return SMR_DRV_COMP_ERROR;
        }
        SM_VS_DBG_INFO(( "[SM] Set TV encoder TV_ENC_CTRL_ENABLE.\r\n" ));
    }

    TMS_BEGIN( VS_HANDLE_VP_STARTED );
    /* Video Plane */
    if ( x_handle_valid(pt_vs_obj->t_vs_comp_grp.t_vp.h_comp) == TRUE || 
         pt_vs_obj->ui4_lastest_vp_event != SM_EVN_VS_VP_PLAYED )
    {
        i4_ret = rm_set( pt_vs_obj->t_vs_comp_grp.t_vp.h_comp,
                         FALSE,
                         ANY_PORT_NUM,
                         VID_PLA_SET_TYPE_CTRL,
                         (VOID*)VID_PLA_CTRL_ENABLE,
                         sizeof(VID_PLA_CTRL_T),
                         &pt_vs_obj->t_vs_comp_grp.t_vp.e_comp_cond);
        if ( i4_ret != RMR_OK )
        {
            return SMR_DRV_COMP_ERROR;
        }
        SM_VS_DBG_INFO(( "[SM] Set Video plane VID_PLA_CTRL_ENABLE.\r\n" ));

        /* simulate VP played event */
        sm_vs_send_msg_do_event(
            h_stream,
            SM_EVN_VS_VP_PLAYED,
            0 );
    }

    TMS_BEGIN( VS_HANDLE_VD_STARTED );

#if 0
#ifdef MW_TV_AV_BYPASS_SUPPORT
    e_cond = rm_cond_chg( pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond,
                          RM_COND_IGNORE,
                          COND_GRP_CONN_ORIG );
    if ( e_cond != RM_COND_CONN_ORIGINATOR )
    {
        /* simulate Decoder played event */
        sm_vs_send_msg_do_event(
            h_stream,
            SM_EVN_VS_DEC_PLAYED,
            0 );
        return SMR_OK;
    }
#endif
#endif

    if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
    {
        /* Video Decoder */
        if ( x_handle_valid(pt_vs_obj->t_vs_comp_grp.t_dec.h_comp) == TRUE ||
             pt_vs_obj->ui4_lastest_dec_event != SM_EVN_VS_DEC_PLAYED )
        {
            pt_vs_obj->e_vd_ctrl_type = ( (pt_sess_obj->i4_speed == 0) ?
                                          VID_DEC_CTRL_FREEZE :
                                          VID_DEC_CTRL_PLAY );

            i4_ret = rm_set( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                             FALSE,
                             ANY_PORT_NUM,
                             VID_DEC_SET_TYPE_CTRL,
                             (VOID*)pt_vs_obj->e_vd_ctrl_type,
                             sizeof(VID_DEC_CTRL_T),
                             &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond);
            if ( i4_ret != RMR_OK && i4_ret != RMR_ASYNC_NFY )
            {
                return SMR_DRV_COMP_ERROR;
            }
            SM_VS_DBG_INFO(( "[SM] Set Video decoder VID_DEC_CTRL_PLAY.\r\n" ));
        }
    }
    else /* analog Video stream */
    {
        /* TV Decoder */
        if ( x_handle_valid(pt_vs_obj->t_vs_comp_grp.t_dec.h_comp) == TRUE ||
             pt_vs_obj->ui4_lastest_dec_event != SM_EVN_VS_DEC_PLAYED )
        {
            pt_vs_obj->e_tvd_ctrl_type = TV_DEC_CTRL_PLAY;

            i4_ret = rm_set( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                             FALSE,
                             ANY_PORT_NUM,
                             TV_DEC_SET_TYPE_CTRL,
                             (VOID*)pt_vs_obj->e_tvd_ctrl_type,
                             sizeof(TV_DEC_CTRL_T),
                             &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond);
            if ( i4_ret != RMR_OK && i4_ret != RMR_ASYNC_NFY )
            {
                return SMR_DRV_COMP_ERROR;
            }
            SM_VS_DBG_INFO(( "[SM] Set TV decoder TV_DEC_CTRL_PLAY.\r\n" ));
        }
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _sm_vs_state_vid_drv_stop
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_vs_obj References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 _sm_vs_state_vid_drv_stop(
    HANDLE_T              h_stream,
    SM_VIDEO_STREAM_T*    pt_vs_obj )
{
    INT32                 i4_ret;

    if ( x_handle_valid(pt_vs_obj->t_vs_comp_grp.t_dec.h_comp) == TRUE &&
         pt_vs_obj->ui4_lastest_dec_event == SM_EVN_VS_DEC_PLAYED )
    {
#ifdef MW_TV_AV_BYPASS_SUPPORT
        RM_COND_T    e_cond = RM_COND_IGNORE;
#endif
        TMS_BEGIN( VS_HANDLE_VD_STOPPED );
#ifdef MW_TV_AV_BYPASS_SUPPORT
        e_cond = rm_cond_chg( pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond,
                              RM_COND_IGNORE,
                              COND_GRP_CONN_ORIG );
        if ( e_cond != RM_COND_CONN_ORIGINATOR )
        {
            if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
            {
                pt_vs_obj->e_vd_ctrl_type = VID_DEC_CTRL_STOP;
            }
            else
            {
                pt_vs_obj->e_tvd_ctrl_type = TV_DEC_CTRL_STOP;
            }
            /* simulate Decoder stopped event */
            sm_vs_send_msg_do_event(
                h_stream,
                SM_EVN_VS_DEC_STOPPED,
                0 );
        }
        else
#endif
        if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
        {
            pt_vs_obj->e_vd_ctrl_type = VID_DEC_CTRL_STOP;
            i4_ret = rm_set(
                pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                FALSE,
                ANY_PORT_NUM,
                VID_DEC_SET_TYPE_CTRL,
                (VOID*)pt_vs_obj->e_vd_ctrl_type,
                sizeof(VID_DEC_CTRL_T),
                &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond);
            if ( i4_ret != RMR_OK && i4_ret != RMR_ASYNC_NFY )
            {
                return SMR_INTERNAL_ERROR;
            }

            SM_VS_DBG_INFO(( "[SM] Set Video Decoder VID_DEC_CTRL_STOP.\r\n" ));
        }
        else
        {
            RM_COND_T  e_cond = RM_COND_IGNORE;

            e_cond = rm_cond_chg( pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond,
                                  RM_COND_IGNORE,
                                  COND_GRP_CONN_STATE );
            if ( e_cond != RM_COND_CONNECTED && e_cond != RM_COND_HARD_WIRED )
            {
                return SMR_INTERNAL_ERROR;
            }

            pt_vs_obj->e_tvd_ctrl_type = TV_DEC_CTRL_STOP;
            i4_ret = rm_set(
                pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                FALSE,
                ANY_PORT_NUM,
                TV_DEC_SET_TYPE_CTRL,
                (VOID*)pt_vs_obj->e_tvd_ctrl_type,
                sizeof(TV_DEC_CTRL_T),
                &pt_vs_obj->t_vs_comp_grp.t_dec.e_comp_cond);
            if ( i4_ret != RMR_OK && i4_ret != RMR_ASYNC_NFY )
            {
                return SMR_INTERNAL_ERROR;
            }

            SM_VS_DBG_INFO(( "[SM] Set TV Decoder TV_DEC_CTRL_STOP.\r\n" ));
        }
    }
#ifdef MW_TV_AV_BYPASS_SUPPORT
    else if ( x_handle_valid(pt_vs_obj->t_vs_comp_grp.t_dec.h_comp) == TRUE &&
              pt_vs_obj->ui4_lastest_dec_event == SM_EVN_VS_DEC_DISCONNECTED )
    {
        /* simulate Decoder stopped event */
        sm_vs_send_msg_do_event(
            h_stream,
            SM_EVN_VS_DEC_STOPPED,
            0 );
    }
#endif

    if ( x_handle_valid(pt_vs_obj->t_vs_comp_grp.t_vp.h_comp) == TRUE &&
         pt_vs_obj->ui4_lastest_vp_event == SM_EVN_VS_VP_PLAYED )
    {
        TMS_BEGIN( VS_HANDLE_VP_STOPPED );
        i4_ret = rm_set( pt_vs_obj->t_vs_comp_grp.t_vp.h_comp,
                        FALSE,
                        ANY_PORT_NUM,
                        VID_PLA_SET_TYPE_CTRL,
                        (VOID*)VID_PLA_CTRL_DISABLE,
                        sizeof(VID_PLA_CTRL_T),
                        &pt_vs_obj->t_vs_comp_grp.t_vp.e_comp_cond);
        if ( i4_ret != RMR_OK )
        {
            return SMR_INTERNAL_ERROR;
        }
        SM_VS_DBG_INFO(( "[SM] Set Video Plane VID_PLA_CTRL_DISABLE.\r\n" ));
    }
    /* simulate VP stopped event */
    sm_vs_send_msg_do_event(
        h_stream,
        SM_EVN_VS_VP_STOPPED,
        0 );

    /* Should we disable all other drivers?? -- iolo
        if ( pt_vs_obj->t_vs_comp_grp.t_pmx.h_comp != NULL_HANDLE )
        {
            i4_ret = rm_set( pt_vs_obj->t_vs_comp_grp.t_pmx.h_comp,
                          FALSE,
                          ANY_PORT_NUM,
                          PLA_MXR_SET_TYPE_CTRL,
                          (VOID*)PLA_MXR_CTRL_DISABLE,
                          sizeof(PLA_MXR_CTRL_T),
                          &pt_vs_obj->t_vs_comp_grp.t_pmx.e_comp_cond);
            if ( i4_ret != RMR_OK )
            {
                return SMR_INTERNAL_ERROR;
            }
            SM_VS_DBG_INFO(( "[SM] Set Plane Mixer PLA_MXR_CTRL_DISABLE\r\n" ));
        }

        if ( pt_vs_obj->t_vs_comp_grp.t_tve.h_comp != NULL_HANDLE )
        {
            i4_ret = rm_set( pt_vs_obj->t_vs_comp_grp.t_tve.h_comp,
                          FALSE,
                          ANY_PORT_NUM,
                          TV_ENC_SET_TYPE_CTRL,
                          (VOID*)TV_ENC_CTRL_DISABLE,
                          sizeof(TV_ENC_CTRL_T),
                          &pt_vs_obj->t_vs_comp_grp.t_tve.e_comp_cond);
            if ( i4_ret != RMR_OK )
            {
                return SMR_INTERNAL_ERROR;
            }
            SM_VS_DBG_INFO(( "[SM] Set TV Encoder TV_ENC_CTRL_DISABLE\r\n" ));
        }
       */

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _sm_vs_state_record_latest_event
 *
 * Description: <function description>
 *
 * Inputs:  pt_vs_obj References.
 *          ui4_event References.
 *
 * Outputs: -
 *
 * Returns: TRUE    Event is updated.
 *          FALSE   No changes.
 *
 ----------------------------------------------------------------------------*/
static BOOL _sm_vs_state_record_latest_event(
    SM_VIDEO_STREAM_T*    pt_vs_obj,
    UINT32                ui4_event )
{
    switch ( SM_EVN_GET_GRP(ui4_event) )
    {
    case SM_EVN_VS_GRP_DEC:
        if ( pt_vs_obj->ui4_lastest_dec_event == ui4_event )
        {
            return FALSE;
        }
        pt_vs_obj->ui4_lastest_dec_event = ui4_event;
        break;

    case SM_EVN_VS_GRP_VP:
        if ( pt_vs_obj->ui4_lastest_vp_event == ui4_event )
        {
            return FALSE;
        }
        pt_vs_obj->ui4_lastest_vp_event = ui4_event;
        break;

    case SM_EVN_VS_GRP_PMX:
        if ( pt_vs_obj->ui4_lastest_pmx_event == ui4_event )
        {
            return FALSE;
        }
        pt_vs_obj->ui4_lastest_pmx_event = ui4_event;
        break;

    case SM_EVN_VS_GRP_TVE:
        if ( pt_vs_obj->ui4_lastest_tve_event == ui4_event )
        {
            return FALSE;
        }
        pt_vs_obj->ui4_lastest_tve_event = ui4_event;
        break;

    case SM_EVN_VS_GRP_PM:
        if ( pt_vs_obj->ui4_lastest_pm_event == ui4_event )
        {
            return FALSE;
        }
        pt_vs_obj->ui4_lastest_pm_event = ui4_event;
        break;

    case SM_EVN_GRP_CA:
        if ( pt_vs_obj->ui4_lastest_ca_event == ui4_event )
        {
            return FALSE;
        }
        pt_vs_obj->ui4_lastest_ca_event = ui4_event;
        break;

    case SM_EVN_VS_GRP_APP:
    default:
        /* Ignore un-handled events */
        return FALSE;
    }

#ifdef TIME_MEASUREMENT
    _sm_vs_drv_time_msrt( ui4_event );
#endif /* TIME_MEASUREMENT */

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: _sm_vs_state_do_vid_drv_event
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_vs_obj References.
 *          ui4_event References.
 *
 * Outputs: -
 *
 * Returns: i4_ret  Success.
 *          SMR_OK  Fail.
 *
 ----------------------------------------------------------------------------*/
static INT32 _sm_vs_state_do_vid_drv_event(
    HANDLE_T              h_stream,
    SM_VIDEO_STREAM_T*    pt_vs_obj,
    UINT32                ui4_event )
{
    INT32    i4_ret;

    if ( pt_vs_obj->ui4_lastest_dec_event == SM_EVN_VS_DEC_OPENED &&
         pt_vs_obj->ui4_lastest_vp_event == SM_EVN_VS_VP_OPENED &&
         pt_vs_obj->ui4_lastest_pmx_event == SM_EVN_VS_PMX_OPENED &&
         TVE_NOT_EXIST_OR_LAST_EVENT_IS( pt_vs_obj, SM_EVN_VS_TVE_OPENED ) )
    {
        pt_vs_obj->t_vs_comp_grp.e_grp_cond = SM_VS_COMP_GRP_COND_OPENED;
        pt_vs_obj->t_vs_comp_grp.e_grp_status = SM_VS_COMP_GRP_STATUS_OPENED;

        i4_ret = _sm_vs_state_vid_drvs_connect(
            h_stream,
            pt_vs_obj );
        if ( i4_ret != SMR_OK )
        {
            SM_VS_DBG_ERR(( "[SM] _sm_vs_state_vid_drvs_connect() fail return %d\r\n", i4_ret));
            return i4_ret;
        }
    }
    else if ( pt_vs_obj->ui4_lastest_dec_event == SM_EVN_VS_DEC_CLOSED &&
              pt_vs_obj->ui4_lastest_vp_event == SM_EVN_VS_VP_CLOSED &&
              pt_vs_obj->ui4_lastest_pmx_event == SM_EVN_VS_PMX_CLOSED &&
              TVE_NOT_EXIST_OR_LAST_EVENT_IS( pt_vs_obj, SM_EVN_VS_TVE_CLOSED ) )
    {
        pt_vs_obj->t_vs_comp_grp.e_grp_cond = SM_VS_COMP_GRP_COND_CLOSED;
        pt_vs_obj->t_vs_comp_grp.e_grp_status = SM_VS_COMP_GRP_STATUS_CLOSED;
    }
    else if ( pt_vs_obj->ui4_lastest_dec_event == SM_EVN_VS_DEC_CONNECTED &&
              pt_vs_obj->ui4_lastest_vp_event == SM_EVN_VS_VP_CONNECTED &&
              pt_vs_obj->ui4_lastest_pmx_event == SM_EVN_VS_PMX_CONNECTED &&
              TVE_NOT_EXIST_OR_LAST_EVENT_IS( pt_vs_obj, SM_EVN_VS_TVE_CONNECTED ) )
    {
        pt_vs_obj->t_vs_comp_grp.e_grp_cond = SM_VS_COMP_GRP_COND_CONNECTED;
        pt_vs_obj->t_vs_comp_grp.e_grp_status = SM_VS_COMP_GRP_STATUS_CONNECTED;
    }
    else if ( pt_vs_obj->ui4_lastest_dec_event == SM_EVN_VS_DEC_DISCONNECTED &&
              pt_vs_obj->ui4_lastest_vp_event == SM_EVN_VS_VP_DISCONNECTED &&
              pt_vs_obj->ui4_lastest_pmx_event == SM_EVN_VS_PMX_DISCONNECTED &&
              TVE_NOT_EXIST_OR_LAST_EVENT_IS( pt_vs_obj, SM_EVN_VS_TVE_DISCONNECTED ) )
    {
        pt_vs_obj->t_vs_comp_grp.e_grp_cond = SM_VS_COMP_GRP_COND_DISCONNECTED;
        pt_vs_obj->t_vs_comp_grp.e_grp_status = SM_VS_COMP_GRP_STATUS_DISCONNECTED;

        i4_ret = sm_vs_comp_grp_close( h_stream, &pt_vs_obj->t_vs_comp_grp );
        if ( i4_ret != SMR_OK )
        {
            SM_VS_DBG_ERR(( "[SM] sm_vs_comp_grp_close() fail return %d\r\n", i4_ret));
            return i4_ret;
        }
    }
    else if ( pt_vs_obj->ui4_lastest_dec_event == SM_EVN_VS_DEC_PLAYED &&
              pt_vs_obj->ui4_lastest_vp_event == SM_EVN_VS_VP_PLAYED &&
              pt_vs_obj->ui4_lastest_pmx_event == SM_EVN_VS_PMX_CONNECTED &&
              TVE_NOT_EXIST_OR_LAST_EVENT_IS( pt_vs_obj, SM_EVN_VS_TVE_CONNECTED ) )
    {
        pt_vs_obj->t_vs_comp_grp.e_grp_cond = SM_VS_COMP_GRP_COND_STARTED;
        pt_vs_obj->t_vs_comp_grp.e_grp_status = SM_VS_COMP_GRP_STATUS_STARTED;
    }
    else if ( ( pt_vs_obj->ui4_lastest_dec_event == SM_EVN_VS_DEC_STOPPED ||
                pt_vs_obj->ui4_lastest_dec_event == SM_EVN_VS_DEC_DISCONNECTED ) &&
              pt_vs_obj->ui4_lastest_vp_event == SM_EVN_VS_VP_STOPPED &&
              pt_vs_obj->ui4_lastest_pmx_event == SM_EVN_VS_PMX_CONNECTED &&
              TVE_NOT_EXIST_OR_LAST_EVENT_IS( pt_vs_obj, SM_EVN_VS_TVE_CONNECTED ) )
    {
        pt_vs_obj->t_vs_comp_grp.e_grp_cond = SM_VS_COMP_GRP_COND_STOPPED;
        pt_vs_obj->t_vs_comp_grp.e_grp_status = SM_VS_COMP_GRP_STATUS_STOPPED;
    }
    else
    {
        pt_vs_obj->t_vs_comp_grp.e_grp_cond = SM_VS_COMP_GRP_COND_IGNORE;
    }
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _sm_vs_state_do_pm_event
 *
 * Description: <function description>
 *
 * Inputs:  h_stream  References.
 *          pt_vs_obj References.
 *          ui4_event References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _sm_vs_state_do_pm_event(
    HANDLE_T              h_stream,
    SM_VIDEO_STREAM_T*    pt_vs_obj,
    UINT32                ui4_event )
{
    switch ( ui4_event )
    {
    case SM_EVN_VS_PM_COND_CLOSED:
        pt_vs_obj->t_base.h_pm_comp = NULL_HANDLE;
        break;

    case SM_EVN_VS_PM_COND_PLAYED:
        /*TMS_END( VS_HANDLE_PM_STARTED );*/
        break;

    case SM_EVN_VS_PM_COND_STOPPED:
        if ( pt_vs_obj->t_state.ui4_curr_state == SM_COND_OPENING )
        {
            TMS_END( VS_HANDLE_PM_OPENED );
        }
        break;

    default:
        break;
    }

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_opening
 *
 * Description: <function description>
 *
 * Inputs:  ui4_previous_state References.
 *          ui4_event          References.
 *          ui4_curr_state     References.
 *          pv_tag             References.
 *
 * Outputs: -
 *
 * Returns: i4_ret              Success.
 *          SMR_SESS_NOT_EXIST  Fail.
 *          SMR_INTERNAL_ERROR  Fail.
 *          SMR_OK              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_opening(
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    INT32                 i4_ret;
    SM_SESSION_T*         pt_sess_obj = NULL;
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;
    HANDLE_T              h_stream;

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;
    h_stream = pt_vs_obj->t_base.h_stream;

    _sm_vs_state_record_latest_event( pt_vs_obj, ui4_event );

    pt_sess_obj = (SM_SESSION_T*)sm_get_sess_obj( pt_vs_obj->t_base.h_sess );
    if ( !pt_sess_obj )
    {
        SM_VS_DBG_ERR((
            "[SM] VS opening: sm_get_sess_obj(0x%.8x) error!\r\n",
            (UINT32)pt_vs_obj->t_base.h_sess ));

        sm_vs_send_msg_do_event(
            h_stream,
            SM_EVN_VS_INTERNAL_ERR,
            (UINT32)SMR_SESS_NOT_EXIST );
        return SMR_SESS_NOT_EXIST;
    }

    /* open SCDB */
    i4_ret = x_scdb_open( pt_sess_obj->h_connection,
                          &pt_vs_obj->t_base.h_scdb,
                          (VOID*)h_stream,
                          &_sm_vs_state_scdb_nfy_fct );
    if ( i4_ret != SCDBR_OK )
    {
        SM_VS_DBG_ERR(( "[SM] VS opening: x_scdb_open() fail, i4_ret = %d\r\n", i4_ret ));
        sm_vs_send_msg_do_event(
            h_stream,
            SM_EVN_VS_INTERNAL_ERR,
            (UINT32)SMR_STRM_COMP_ID_NOT_FOUND );
        return SMR_STRM_COMP_ID_NOT_FOUND;
    }

    if ( pt_vs_obj->ui2_force_pid == MPEG_2_NULL_PID )
    {
        i4_ret = x_scdb_get_rec( pt_vs_obj->t_base.h_scdb,
                                 &pt_vs_obj->t_base.t_strm_comp_id,
                                 &pt_vs_obj->t_base.t_scdb_rec );
        if ( i4_ret != SCDBR_OK ||
             ( !IS_ANALOG_VID_STRM( pt_vs_obj ) &&
               !IS_MPEG_VID_STRM( pt_vs_obj ) &&
               !IS_MULTIMEDIA_VID_STRM( pt_vs_obj ) )
           )
        {
            SM_VS_DBG_ERR((
                "[SM] VS opening: x_scdb_get_rec(0x%.8X) fail, i4_ret = %d\r\n",
                (UINT32)pt_vs_obj->t_base.h_scdb, i4_ret ));
            sm_vs_send_msg_do_event(
                h_stream,
                SM_EVN_VS_INTERNAL_ERR,
                (UINT32)SMR_STRM_COMP_ID_NOT_FOUND );
            return SMR_STRM_COMP_ID_NOT_FOUND;
        }
        if(pt_vs_obj->t_base.t_scdb_rec.e_rec_type == SCDB_REC_TYPE_VIDEO_MPEG)
        {
            /* for 3D video */
            pt_vs_obj->t_rd3d_info = pt_vs_obj->t_base.t_scdb_rec.u.t_video_mpeg.t_rd3d;
            pt_vs_obj->t_mvc_ext = pt_vs_obj->t_base.t_scdb_rec.u.t_video_mpeg.t_mvc_ext;
        }
    }
    else /* force PID */
    {
        /* fake a SCDB record */
        pt_vs_obj->t_base.t_scdb_rec.e_rec_type = SCDB_REC_TYPE_VIDEO_MPEG;
        pt_vs_obj->t_base.t_scdb_rec.u.t_video_mpeg.ui2_pid = pt_vs_obj->ui2_force_pid;
        pt_vs_obj->t_base.t_scdb_rec.u.t_video_mpeg.e_enc = (VID_ENC_T)pt_vs_obj->ui1_force_enc;
        pt_vs_obj->t_base.t_scdb_rec.u.t_video_mpeg.e_vid_fmt = VID_FMT_UNKNOWN;
        pt_vs_obj->t_base.t_scdb_rec.u.t_video_mpeg.b_default = FALSE;
        /* for 3D video, default set false ??? */
        pt_vs_obj->t_base.t_scdb_rec.u.t_video_mpeg.t_rd3d.b_rd3d = FALSE;
        pt_vs_obj->t_base.t_scdb_rec.u.t_video_mpeg.t_mvc_ext.b_mvc_video = FALSE;
        pt_vs_obj->t_base.t_scdb_rec.u.t_video_mpeg.t_mvc_ext.b_mvc_ext_desc = FALSE;
    }
    
    /* set vs obj actual enc */
    sm_vs_comp_vd_get_actual_enc(pt_vs_obj);

    /* If decoder type is unknown, set the decoder type according to SCDB.
       If not, check if the decoder type given by application command is
       conflict with SCBD's record type */
    if ( pt_vs_obj->t_vs_comp_grp.t_dec.t_info.e_type != DRVT_UNKNOWN )
    {
        if ( ( IS_ANALOG_VID_STRM( pt_vs_obj ) && pt_vs_obj->t_vs_comp_grp.t_dec.t_info.e_type != DRVT_TV_DEC ) ||
             ( ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) ) &&
                pt_vs_obj->t_vs_comp_grp.t_dec.t_info.e_type != DRVT_VID_DEC ) )
        {
            SM_VS_DBG_ERR((
                "[SM] VS opening: ERROR, User specified decoder type %u, mismatch"
                " SCDB record type %u\r\n",
                (UINT32)pt_vs_obj->t_vs_comp_grp.t_dec.t_info.e_type,
                (UINT32)pt_vs_obj->t_base.t_scdb_rec.e_rec_type ));
            sm_vs_send_msg_do_event(
                h_stream,
                SM_EVN_VS_INTERNAL_ERR,
                (UINT32)SMR_INV_COMMAND );
            return SMR_INV_COMMAND;
        }
    }
    else if ( IS_ANALOG_VID_STRM( pt_vs_obj ) )
    {
        pt_vs_obj->t_vs_comp_grp.t_dec.t_info.e_type = DRVT_TV_DEC;
    }
    else /* is MPEG video stream */
    {
        pt_vs_obj->t_vs_comp_grp.t_dec.t_info.e_type = DRVT_VID_DEC;
        pt_vs_obj->t_vs_comp_grp.t_dec.t_info.b_shared_comp_id = TRUE;
    }

    if ( pt_vs_obj->t_base.h_source == NULL_HANDLE )
    {
        CM_CTRL_RM_SRC_COMP_T t_ctrl_rm_src_comp;

        t_ctrl_rm_src_comp.pv_stream_tag = pt_vs_obj->t_base.t_strm_comp_id.pv_stream_tag;
        /* get source driver component */
        i4_ret = x_cm_get( pt_vs_obj->t_base.h_connection,
                            CM_CTRL_RM_SRC_COMP,
                            (VOID*)&t_ctrl_rm_src_comp);
        if ( i4_ret != CMR_OK )
        {
            SM_VS_DBG_ERR((
                "[SM] VS opening: cm_get_op(0x%.8x) fail, i4_ret = %d\r\n",
                (UINT32)pt_vs_obj->t_base.h_connection, i4_ret ));
            return SMR_INTERNAL_ERROR;
        }
        pt_vs_obj->t_base.h_source = t_ctrl_rm_src_comp.h_rm_comp;
    }

    /* open rest components that are not opened by commands */
    sm_drv_map_lock();

    i4_ret = sm_vs_comp_grp_open( h_stream,
                                  pt_vs_obj->t_base.h_source,
                                  &pt_vs_obj->t_vs_comp_grp );

    sm_drv_map_unlock();

    if ( i4_ret != SMR_OK )
    {
        SM_VS_DBG_ERR((
            "[SM] VS opening: sm_vs_comp_grp_open(0x%.8x) fail, i4_ret = %d\r\n",
            (UINT32)h_stream, i4_ret ));

        sm_vs_send_msg_do_event(
            h_stream,
            SM_EVN_VS_DRV_ERR,
            0 );
        return i4_ret;
    }

    /* open PM component */
    if ( IS_MPEG_VID_STRM(pt_vs_obj) )
    {
        PM_ES_REQUEST_T       t_es_comp;

        x_memset( ((VOID*)(&t_es_comp)), 0, sizeof(PM_ES_REQUEST_T) );

        if ( pt_vs_obj->ui2_force_pid == MPEG_2_NULL_PID )
        {
            t_es_comp.e_es_info_type = PM_PRESENT_ES_WO_SETTING;
            t_es_comp.u_es_info.t_stream_comp_id =
                pt_vs_obj->t_base.t_strm_comp_id;
        }
        else
        {
            t_es_comp.e_es_info_type = PM_PRESENT_ES_WI_SETTING;
            t_es_comp.u_es_info.t_stream_setting.e_type = ST_VIDEO;
            t_es_comp.u_es_info.t_stream_setting.t_pid =
                pt_vs_obj->ui2_force_pid;
        }
        t_es_comp.h_component = NULL_HANDLE;

        t_es_comp.t_filter_nfy_info.pf_nfy = &_sm_vs_state_pm_nfy;
        t_es_comp.t_filter_nfy_info.pv_tag = (VOID*)h_stream;
        t_es_comp.t_filter_nfy_info.e_cond = PM_COND_CLOSED;
        t_es_comp.t_filter_nfy_info.ui4_detail_code = 0;

#ifdef SYS_MHP_SUPPORT
        t_es_comp.e_ca_require_lvl = pt_vs_obj->e_ca_req_lvl;
#endif

        t_es_comp.t_ca_nfy_info.pf_nfy = &_sm_vs_state_pm_ca_nfy;
        t_es_comp.t_ca_nfy_info.pv_tag = (VOID*)h_stream;
        t_es_comp.t_ca_nfy_info.e_cond = PM_CA_COND_CLOSED;
        t_es_comp.t_ca_nfy_info.ui4_detail_code = 0;
        t_es_comp.t_sink_comp_info.e_type = pt_vs_obj->t_vs_comp_grp.t_dec.t_info.e_type;
        t_es_comp.t_sink_comp_info.ui2_id = pt_vs_obj->t_vs_comp_grp.t_dec.t_info.ui2_comp_id;

        t_es_comp.t_cp_nfy_info.pf_nfy = _sm_vs_cp_nfy;
        t_es_comp.t_cp_nfy_info.pv_tag = (VOID*)h_stream;
        t_es_comp.t_cp_nfy_info.t_cp_info.ui1_ctrl_size = 0;

        TMS_BEGIN( VS_HANDLE_PM_OPENED );
        i4_ret = pm_add_component( pt_sess_obj->h_pm_service,
                                   (VOID*)&t_es_comp );
        if ( i4_ret != PMR_OK || t_es_comp.h_component == NULL_HANDLE )
        {
            SM_VS_DBG_ERR((
                "[SM] VS opening: pm_add_component(0x%.8X) fail, i4_ret = %d\r\n", 
                (UINT32)pt_sess_obj->h_pm_service, i4_ret ));

            sm_vs_send_msg_do_event(
                h_stream,
                SM_EVN_VS_INTERNAL_ERR,
                (UINT32)SMR_INTERNAL_ERROR );
            return SMR_INTERNAL_ERROR;
        }

        pt_vs_obj->t_base.h_pm_comp = t_es_comp.h_component;
        pt_vs_obj->ui4_lastest_ca_event =
            _sm_vs_pm_ca_cond_2_vsh( t_es_comp.t_ca_nfy_info.e_cond );
        pt_vs_obj->ui4_lastest_ca_detail_code =
            t_es_comp.t_ca_nfy_info.ui4_detail_code;

        _sm_vs_state_pm_nfy(
            pt_vs_obj->t_base.h_pm_comp,
            t_es_comp.t_filter_nfy_info.e_cond,
            0,
            (VOID*)h_stream );
    }
#ifdef ENABLE_MULTIMEDIA
    else if ( IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
    {
        PM_MM_REQUEST_T       t_mm_comp;

        x_memset( ((VOID*)(&t_mm_comp)), 0, sizeof(PM_MM_REQUEST_T) );

#ifdef TIME_SHIFT_SUPPORT
        if ( IS_TS_VID_STRM(pt_vs_obj) && 
             ( pt_vs_obj->t_crypt_info.u.t_pvr_crypt.e_mode != MM_CRYPT_MODE_INVALID || 
               pt_vs_obj->ui2_force_pid != MPEG_2_NULL_PID ))
        {
            t_mm_comp.e_es_info_type = PM_MM_ES_WI_SETTING;
            t_mm_comp.u_es_info.t_stream_setting.e_type = STREAM_ID_TYPE_TS;
            if (pt_vs_obj->ui2_force_pid == MPEG_2_NULL_PID)
            {
                t_mm_comp.u_es_info.t_stream_setting.u.t_stm_ts_id = 
                    pt_vs_obj->t_base.t_scdb_rec.u.t_video_mpeg.ui2_pid;
            }
            else
            {
                t_mm_comp.u_es_info.t_stream_setting.u.t_stm_ts_id = 
                    pt_vs_obj->ui2_force_pid;
            }
            t_mm_comp.t_crypt_info = pt_vs_obj->t_crypt_info;
        }
        else
#endif /* TIME_SHIFT_SUPPORT */
        if ( (pt_vs_obj->t_base.t_scdb_rec.e_rec_type == SCDB_REC_TYPE_VIDEO_MP4) && 
             (MM_CRYPT_TYPE_MLN == pt_vs_obj->t_crypt_info.e_mm_crypt_type))
        {
            t_mm_comp.e_es_info_type = PM_MM_ES_WI_SETTING;
            t_mm_comp.u_es_info.t_stream_setting.e_type = STREAM_ID_TYPE_ES;
            t_mm_comp.u_es_info.t_stream_setting.u.t_stm_es_id =
                pt_vs_obj->t_base.t_scdb_rec.u.t_video_mp4.ui4_stream_id;

            t_mm_comp.t_crypt_info = pt_vs_obj->t_crypt_info;
        }
        else
        {
            t_mm_comp.e_es_info_type = PM_MM_ES_WO_SETTING;
            t_mm_comp.u_es_info.t_stream_comp_id =
                pt_vs_obj->t_base.t_strm_comp_id;
        }
        t_mm_comp.h_component = NULL_HANDLE;

        t_mm_comp.t_filter_nfy_info.pf_nfy = &_sm_vs_state_pm_nfy;
        t_mm_comp.t_filter_nfy_info.pv_tag = (VOID*)h_stream;
        t_mm_comp.t_filter_nfy_info.e_cond = PM_COND_CLOSED;
        t_mm_comp.t_filter_nfy_info.ui4_detail_code = 0;

        t_mm_comp.t_ca_nfy_info.pf_nfy = &_sm_vs_state_pm_ca_nfy;
        t_mm_comp.t_ca_nfy_info.pv_tag = (VOID*)h_stream;
        t_mm_comp.t_ca_nfy_info.e_cond = PM_CA_COND_CLOSED;
        t_mm_comp.t_ca_nfy_info.ui4_detail_code = 0;

        t_mm_comp.t_sink_comp_info.e_type = pt_vs_obj->t_vs_comp_grp.t_dec.t_info.e_type;
        t_mm_comp.t_sink_comp_info.ui2_id = pt_vs_obj->t_vs_comp_grp.t_dec.t_info.ui2_comp_id;

        TMS_BEGIN( VS_HANDLE_PM_OPENED );
        i4_ret = pm_add_component( pt_sess_obj->h_pm_service,
                                   (VOID*)&t_mm_comp );

        if ( i4_ret != PMR_OK || t_mm_comp.h_component == NULL_HANDLE )
        {
            SM_VS_DBG_ERR((
                "[SM] VS opening: pm_add_component(0x%.8X) fail, i4_ret = %d\r\n", 
                (UINT32)pt_sess_obj->h_pm_service, i4_ret ));

            sm_vs_send_msg_do_event(
                h_stream,
                SM_EVN_VS_INTERNAL_ERR,
                (UINT32)SMR_INTERNAL_ERROR );
            return SMR_INTERNAL_ERROR;
        }

        pt_vs_obj->t_base.h_pm_comp = t_mm_comp.h_component;

        _sm_vs_state_pm_nfy(
            pt_vs_obj->t_base.h_pm_comp,
            t_mm_comp.t_filter_nfy_info.e_cond,
            0,
            (VOID*)h_stream );
    }
#endif

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_opened
 *
 * Description: <function description>
 *
 * Inputs:  ui4_previous_state References.
 *          ui4_event          References.
 *          ui4_curr_state     References.
 *          pv_tag             References.
 *
 * Outputs: -
 *
 * Returns: i4_ret  Success.
 *          SMR_OK  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_opened(
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    INT32                 i4_ret;
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;
    HANDLE_T              h_stream;

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;
    h_stream = pt_vs_obj->t_base.h_stream;

    _sm_vs_state_record_latest_event( pt_vs_obj, ui4_event );

    if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
    {
        sm_vs_comp_vd_set_callback( h_stream, pt_vs_obj );
        sm_vs_comp_vp_set_callback( h_stream, pt_vs_obj );
    }
    else
    {
        sm_vs_comp_tvd_set_callback( h_stream, pt_vs_obj );

        /* start filtering VBI info */
        SM_VS_DBG_INFO(( "[SM] VS(0x%.8X) opened: play TVD cc.\r\n",
            (UINT32)h_stream ));
        sm_vs_comp_tvd_play_cc( h_stream, pt_vs_obj );
    }

    sm_vs_comp_vp_set_op_monitor( pt_vs_obj, VID_PLA_SET_TYPE_MODE );

    /* Set TV signal to an invalid value, so that the first notify after PLAY
       will be treated as change */
    pt_vs_obj->e_tv_signal = TV_DEC_TV_SIG_UNKNOWN;
    pt_vs_obj->e_dec_status = VID_DEC_DECODE_UNKNOWN;
    pt_vs_obj->e_color_sys = COLOR_SYS_UNKNOWN;
    x_memset( &pt_vs_obj->t_vid_res, 0, sizeof(VSH_SRC_RESOLUTION_INFO_T) );

    if ( ui4_previous_state == SM_COND_OPENING )
    {
        i4_ret = _sm_vs_state_vid_drv_init(
            h_stream,
            pt_vs_obj );
        if ( i4_ret != SMR_OK )
        {
            /* delete video component first, otherwise PM will set this ctrl to swdmx sometimes */
            /* timing issue, for CR DTV00080871 */
            if ( pt_vs_obj->t_base.h_pm_comp != NULL_HANDLE )
            {
                i4_ret = pm_del_component( pt_vs_obj->t_base.h_pm_comp );
                if ( i4_ret != PMR_OK )
                {
                    SM_VS_DBG_ERR((
                        "[SM] VS opened : pm_del_component(0x%.8x) fail, i4_ret = %d\r\n",
                        (UINT32)pt_vs_obj->t_base.h_pm_comp, i4_ret ));
                }
            }
            
            sm_vs_send_msg_do_event(
                h_stream,
                SM_EVN_VS_DRV_ERR,
                0 );
            return i4_ret;
        }
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_starting
 *
 * Description: <function description>
 *
 * Inputs:  ui4_previous_state References.
 *          ui4_event          References.
 *          ui4_curr_state     References.
 *          pv_tag             References.
 *
 * Outputs: -
 *
 * Returns: SMR_INTERNAL_ERROR  Success.
 *          SMR_DRV_COMP_ERROR  Fail.
 *          SMR_OK              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_starting(
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    INT32                 i4_ret;
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;
    HANDLE_T              h_stream;

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;
    h_stream = pt_vs_obj->t_base.h_stream;

    _sm_vs_state_record_latest_event( pt_vs_obj, ui4_event );

    if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
    {
        /* for 3D video */
        /* set 3D info before set codec for convenient communication between vdec and swdmx */
        if ( pt_vs_obj->t_base.t_scdb_rec.e_rec_type == SCDB_REC_TYPE_VIDEO_MPEG )
        {
            if (pt_vs_obj->t_rd3d_info.b_rd3d)
            {
                /*set RD3D info to video decoder */
                i4_ret = _sm_vs_state_vid_drv_set_rd3d_info( h_stream, pt_vs_obj );
                if ( i4_ret != RMR_OK )
                {
                    /* only show log info when set rd3d fail */
                    SM_VS_DBG_ERR(( "[SM] VS starting: Error, _sm_vs_state_vid_drv_set_rd3d_info() fail, i4_ret = %d\r\n",
                                     i4_ret ));
                }
            }
            if (pt_vs_obj->t_mvc_ext.b_mvc_video)
            {
                /*set mvc_ext info to video decoder */
                i4_ret = _sm_vs_state_vid_drv_set_mvc_ext_desc( h_stream, pt_vs_obj );
                if ( i4_ret != RMR_OK )
                {
                    /* only show log info when set mvc_ext_desc fail */
                    SM_VS_DBG_ERR(( "[SM] VS starting: Error, _sm_vs_state_vid_drv_set_mvc_ext_desc() fail, i4_ret = %d\r\n",
                                     i4_ret ));
                }
            }
        }
        
        i4_ret = sm_vs_comp_vd_set_codec( h_stream, pt_vs_obj );
        if ( i4_ret < SMR_OK )
        {
            /* only show log info when set codec fail */
            SM_VS_DBG_ERR(( "[SM] VS starting: Error, sm_vs_comp_vd_set_codec() fail, i4_ret = %d\r\n",
                             i4_ret ));
        }
        /* Set PCR to Video Decoder */
        i4_ret = _sm_vs_state_vid_drv_set_pcr( h_stream, pt_vs_obj );
        if ( i4_ret != RMR_OK )
        {
            SM_VS_DBG_ERR(( "[SM] VS starting: Error, _sm_vs_state_vid_drv_set_pcr() fail, i4_ret = %d\r\n",
                             i4_ret ));
        }
        
    }

    /* set Process Manager components */
    if ( ui4_event == SM_EVN_VS_APP_PLAY_REQ )
    {
        if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
        {
            i4_ret = pm_play_component( pt_vs_obj->t_base.h_pm_comp );
            if ( i4_ret != PMR_OK )
            {
                SM_VS_DBG_ERR((
                    "[SM] VS starting: pm_play_component(0x%.8x) fail, i4_ret = %d\r\n",
                    (UINT32)pt_vs_obj->t_base.h_pm_comp, i4_ret ));

                sm_vs_send_msg_do_event(
                    h_stream,
                    SM_EVN_VS_INTERNAL_ERR,
                    (UINT32)SMR_INTERNAL_ERROR );
                return SMR_INTERNAL_ERROR;
            }
            SM_VS_DBG_INFO(( "[SM] VS starting: PM play video component done.\r\n" ));
        }
    }

    i4_ret = _sm_vs_state_vid_drv_play( h_stream, pt_vs_obj );
    if ( i4_ret != SMR_OK )
    {
        SM_VS_DBG_ERR((
            "[SM] VS starting: Error, _sm_vs_state_vid_drv_play(0x%.8x) fail, i4_ret = %d\r\n",
            (UINT32)h_stream, i4_ret ));

        sm_vs_send_msg_do_event(
            h_stream,
            SM_EVN_VS_DRV_ERR,
            0 );
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_started
 *
 * Description: <function description>
 *
 * Inputs:  ui4_previous_state References.
 *          ui4_event          References.
 *          ui4_curr_state     References.
 *          pv_tag             References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_started(
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    INT32                 i4_ret;
    SIZE_T                z_size = 0;
    RM_COND_T             e_rm_cond;
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;
#ifdef DEBUG
    HANDLE_T              h_stream;
#endif

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;
#ifdef DEBUG
    h_stream = pt_vs_obj->t_base.h_stream;
#endif

    _sm_vs_state_record_latest_event( pt_vs_obj, ui4_event );

    if ( pt_vs_obj->ui4_lastest_ca_event != SM_EVN_CA_COND_UNKNOWN )
    {
        _sm_vs_state_prc_ca_event( pt_vs_obj );
    }

    /* Ignore TV signal, color system, resolution check, if it is a MPEG stream. */
    if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
    {
        if ( pt_vs_obj->b_pending_res_chg )
        {
            /* Just call sm_vs_state_do_res_chg(), it will check if there is resolution change and notify APP. */
            sm_vs_state_do_res_chg(
                ui4_curr_state,
                SM_EVN_VS_VIDEO_RESOLUTION_CHG,
                ui4_curr_state,
                pv_tag1,
                NULL );
        }
        if ( pt_vs_obj->e_dec_status == VID_DEC_DECODE_UNKNOWN )
        {
            VID_DEC_DECODE_STATUS_T e_dec_status = VID_DEC_DECODE_UNKNOWN;

            z_size = sizeof(VID_DEC_DECODE_STATUS_T);
            i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                FALSE,
                0,
                VID_DEC_GET_TYPE_DECODE_STATUS,
                &e_dec_status,
                &z_size,
                &e_rm_cond );
            if ( i4_ret != RMR_OK )
            {
                SM_VS_DBG_ERR((
                    "[SM] VS: sm_vs_state_do_started(0x%.8x), "
                    "rm_get(VID_DEC_GET_TYPE_DECODE_STATUS) fail, i4_ret = %d\r\n",
                    (UINT32)h_stream, i4_ret ));
            }
            else
            {
                SM_VS_DBG_INFO(( "[SM] VS started: rm_get VD decode status(%d).\r\n", (INT32)e_dec_status ));
                if ( e_dec_status != pt_vs_obj->e_dec_status )
                {
                    sm_vs_state_do_dec_status(
                        ui4_curr_state,
                        SM_EVN_VS_DIG_DECODE_STATUS_CHG,
                        ui4_curr_state,
                        pv_tag1,
                        (VOID*)(UINT32)e_dec_status );
                }
            }
        }
        return SMR_OK;
    }

    if ( pt_vs_obj->e_tv_signal == TV_DEC_TV_SIG_UNKNOWN )
    {
        TV_DEC_TV_SIG_T        e_tv_signal = TV_DEC_TV_SIG_UNKNOWN;

        z_size = sizeof(TV_DEC_TV_SIG_T);
        i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
            FALSE,
            0,
            TV_DEC_GET_TYPE_TV_SIGNAL,
            &e_tv_signal,
            &z_size,
            &e_rm_cond );
        if ( i4_ret != RMR_OK )
        {
            SM_VS_DBG_ERR((
                "[SM] VS: sm_vs_state_do_started(0x%.8x), "
                "rm_get(TV_DEC_GET_TYPE_TV_SIGNAL) fail, i4_ret = %d\r\n",
                (UINT32)h_stream, i4_ret ));
        }
        else
        {
            SM_VS_DBG_INFO(( "[SM] VS started: rm_get TV signal(%d).\r\n", (INT32)e_tv_signal ));
            if ( e_tv_signal != pt_vs_obj->e_tv_signal )
            {
                sm_vs_state_do_tv_sig(
                    ui4_curr_state,
                    SM_EVN_VS_ANA_TV_SIGNAL,
                    ui4_curr_state,
                    pv_tag1,
                    (VOID*)(UINT32)e_tv_signal );
            }
        }
    }

    if ( pt_vs_obj->e_tv_signal == TV_DEC_TV_SIG_STABLE &&
         pt_vs_obj->e_color_sys == COLOR_SYS_UNKNOWN )
    {
        COLOR_SYS_T            e_vsh_color_sys = COLOR_SYS_UNKNOWN;
        TV_DEC_COLOR_SYS_T     e_drv_color_sys;
        UINT32                 ui4_tvd_cap = 0;

        z_size = sizeof(UINT32);
        i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
            FALSE,
            0,
            TV_DEC_GET_TYPE_CAPABILITY,
            &ui4_tvd_cap,
            &z_size,
            &e_rm_cond );
        if ( i4_ret != RMR_OK )
        {
            SM_VS_DBG_ERR((
                "[SM] VS: sm_vs_state_do_started(0x%.8x), "
                "rm_get(TV_DEC_GET_TYPE_CAPABILITY) fail, i4_ret = %d\r\n",
                (UINT32)h_stream, i4_ret ));
        }
        else if ( ui4_tvd_cap & TV_DEC_CAP_COLOR_SYS )
        {
            z_size = sizeof(TV_DEC_COLOR_SYS_T);
            i4_ret = rm_get( pt_vs_obj->t_vs_comp_grp.t_dec.h_comp,
                FALSE,
                0,
                TV_DEC_GET_TYPE_COLOR_SYS,
                &e_drv_color_sys,
                &z_size,
                &e_rm_cond );
            if ( i4_ret != RMR_OK )
            {
                SM_VS_DBG_ERR((
                    "[SM] VS: sm_vs_state_do_started(0x%.8x), "
                    "rm_get(TV_DEC_GET_TYPE_COLOR_SYS) fail, i4_ret = %d\r\n",
                    (UINT32)h_stream, i4_ret ));
            }
            else
            {
                e_vsh_color_sys = sm_vs_comp_tvd_color_sys_2_vsh( e_drv_color_sys );
                if ( e_vsh_color_sys != pt_vs_obj->e_color_sys )
                {
                    sm_vs_state_do_color_sys(
                        ui4_curr_state,
                        SM_EVN_VS_ANA_COLOR_SYSTEM,
                        ui4_curr_state,
                        pv_tag1,
                        (VOID*)(UINT32)e_vsh_color_sys );
                }
            }
        }
    }

    if ( pt_vs_obj->e_tv_signal == TV_DEC_TV_SIG_STABLE &&
         pt_vs_obj->t_vid_res.ui4_frame_rate == 0 &&
         pt_vs_obj->t_vid_res.ui4_width == 0 &&
         pt_vs_obj->t_vid_res.ui4_height == 0 &&
         pt_vs_obj->t_vid_res.b_is_progressive == FALSE &&
         pt_vs_obj->t_vid_res.e_src_asp_ratio == VSH_SRC_ASPECT_RATIO_UNKNOWN &&
         pt_vs_obj->t_vid_res.e_timing_type == VSH_SRC_TIMING_UNKNOWN &&
         pt_vs_obj->t_vid_res.ui4_res_idx == 0 )
    {
        /* Just call sm_vs_state_do_res_chg(), it will check if there is resolution change and notify APP. */
        sm_vs_state_do_res_chg(
            ui4_curr_state,
            SM_EVN_VS_VIDEO_RESOLUTION_CHG,
            ui4_curr_state,
            pv_tag1,
            NULL );
    }

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_stopping
 *
 * Description: <function description>
 *
 * Inputs:  ui4_previous_state References.
 *          ui4_event          References.
 *          ui4_curr_state     References.
 *          pv_tag             References.
 *
 * Outputs: -
 *
 * Returns: SMR_INTERNAL_ERROR  Success.
 *          SMR_OK              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_stopping(
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    INT32                 i4_ret;
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;
    HANDLE_T              h_stream;

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;
    h_stream = pt_vs_obj->t_base.h_stream;

    _sm_vs_state_record_latest_event( pt_vs_obj, ui4_event );

    if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
    {
        if ( pt_vs_obj->ui4_lastest_pm_event != SM_EVN_VS_PM_COND_CLOSED &&
             pt_vs_obj->ui4_lastest_pm_event != SM_EVN_VS_PM_COND_STOPPED )
        {
            i4_ret = pm_stop_component( pt_vs_obj->t_base.h_pm_comp );
            if ( i4_ret != PMR_OK )
            {
                if ( i4_ret == PMR_INV_HANDLE &&
                   ( ui4_event == SM_EVN_VS_APP_CLOSE_REQ ||
                     ui4_event == SM_EVN_SESS_APP_STOP_REQ ) )
                {
                    /* PM component may already closed by session layer, treat as OK */
                }
                else
                {
                    SM_VS_DBG_ERR((
                        "[SM] VS stopping: pm_stop_component(0x%.8x) fail, i4_ret = %d\r\n",
                        (UINT32)pt_vs_obj->t_base.h_pm_comp, i4_ret ));

                    sm_vs_send_msg_do_event(
                        h_stream,
                        SM_EVN_VS_INTERNAL_ERR,
                        (UINT32)SMR_INTERNAL_ERROR );
                    return SMR_INTERNAL_ERROR;
                }
            }
        }
        if ( ui4_event == SM_EVN_VS_RESTART )
        {
            pt_vs_obj->b_codec_not_support = TRUE;
        }
    }

    i4_ret = _sm_vs_state_vid_drv_stop( h_stream, pt_vs_obj );
    if ( i4_ret != SMR_OK )
    {
        SM_VS_DBG_ERR((
            "[SM] VS stopping: _sm_vs_state_vid_drv_stop(0x%.8x) fail, i4_ret = %d\r\n",
            (UINT32)h_stream, i4_ret ));

        sm_vs_send_msg_do_event(
            h_stream,
            SM_EVN_VS_DRV_ERR,
            0 );
        return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_closing
 *
 * Description: <function description>
 *
 * Inputs:  ui4_previous_state References.
 *          ui4_event          References.
 *          ui4_curr_state     References.
 *          pv_tag             References.
 *
 * Outputs: -
 *
 * Returns: i4_ret              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *          SMR_OK              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_closing(
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    INT32                 i4_ret;
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;
    HANDLE_T              h_stream;

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;
    h_stream = pt_vs_obj->t_base.h_stream;

    _sm_vs_state_record_latest_event( pt_vs_obj, ui4_event );

    /* Stop parsing of VBI data */
    if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
    {
    }
    else /* analog video stream */
    {
        /* STOP TV-decoder's CC notify */
        SM_VS_DBG_INFO(( "[SM] VS(0x%.8X) closing: stop TVD cc.\r\n",
                          (UINT32)h_stream ));

        sm_vs_comp_tvd_stop_cc(
            h_stream,
            pt_vs_obj );
    }

    if ( pt_vs_obj->t_base.h_scdb != NULL_HANDLE )
    {
        i4_ret = x_scdb_close( pt_vs_obj->t_base.h_scdb );
        if ( i4_ret != SCDBR_OK )
        {
            SM_VS_DBG_ERR((
                "[SM] VS closing: x_scdb_close(0x%.8x) fail, i4_ret = %d\r\n",
                (UINT32)pt_vs_obj->t_base.h_scdb, i4_ret ));
        }
        pt_vs_obj->t_base.h_scdb = NULL_HANDLE;
    }

    if ( pt_vs_obj->t_vs_comp_grp.e_grp_cond == SM_VS_COMP_GRP_COND_CONNECTED ||
         pt_vs_obj->t_vs_comp_grp.e_grp_cond == SM_VS_COMP_GRP_COND_STARTED ||
         pt_vs_obj->t_vs_comp_grp.e_grp_cond == SM_VS_COMP_GRP_COND_STOPPED )
    {
        /* disconnect TVE to PMX to VP */
        i4_ret = sm_vs_comp_grp_disconnect( h_stream, &pt_vs_obj->t_vs_comp_grp );
        if ( i4_ret != SMR_OK )
        {
            SM_VS_DBG_ERR((
                "[SM] VS closing: sm_vs_comp_grp_disconnect(0x%.8x) fail, i4_ret = %d\r\n",
                (UINT32)h_stream, i4_ret ));
        }

        TMS_BEGIN( VS_HANDLE_VD_DISCONNECTED );
        if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
        {
            /* Disconnect Video Decoder to Demux */
            i4_ret = sm_vs_comp_vd_disconnect( h_stream, &pt_vs_obj->t_vs_comp_grp.t_dec );
        }
        else
        {
            /* Disconnect TV Decoder to Demux */
            i4_ret = sm_vs_comp_tvd_disconnect( h_stream, &pt_vs_obj->t_vs_comp_grp.t_dec );
        }
        if ( i4_ret != SMR_OK )
        {
            SM_VS_DBG_ERR((
                "[SM] VS closing: sm_vs_comp_vd_disconnect(0x%.8x) fail, i4_ret = %d\r\n",
                (UINT32)h_stream, i4_ret ));

            /* fake disconnect message to let state machine continue.. */
            sm_vs_send_msg_do_event( h_stream, SM_EVN_VS_DEC_DISCONNECTED, 0 );
        }
    }
    else
    {
        i4_ret = sm_vs_comp_grp_close( h_stream, &pt_vs_obj->t_vs_comp_grp );
        if ( i4_ret != SMR_OK )
        {
            SM_VS_DBG_ERR((
                "[SM] VS closing: sm_vs_comp_grp_close(0x%.8x) fail, i4_ret = %d\r\n",
                (UINT32)h_stream, i4_ret ));
        }
    }

    if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
    {
        if ( pt_vs_obj->t_base.h_pm_comp != NULL_HANDLE )
        {
            /* Stop PM component if previous state is ERROR, and PM condition is not in STOPPED/CLOSED state */
            if ( ui4_previous_state == SM_COND_ERROR &&
                 pt_vs_obj->ui4_lastest_pm_event != SM_EVN_VS_PM_COND_STOPPED &&
                 pt_vs_obj->ui4_lastest_pm_event != SM_EVN_VS_PM_COND_CLOSED )
            {
                i4_ret = pm_stop_component( pt_vs_obj->t_base.h_pm_comp );
                if ( i4_ret != PMR_OK )
                {
                    SM_VS_DBG_ERR((
                        "[SM] VS closing: pm_stop_component(0x%.8x) fail, i4_ret = %d\r\n",
                        (UINT32)pt_vs_obj->t_base.h_pm_comp, i4_ret ));
                }
            }

            i4_ret = pm_del_component( pt_vs_obj->t_base.h_pm_comp );
            if ( i4_ret != PMR_OK )
            {
                SM_VS_DBG_ERR((
                    "[SM] VS closing: pm_del_component(0x%.8x) fail, i4_ret = %d\r\n",
                    (UINT32)pt_vs_obj->t_base.h_pm_comp, i4_ret ));
            }
        }
        else
        {
            sm_vs_send_msg_do_event(
                h_stream,
                SM_EVN_VS_PM_COND_CLOSED,
                0 );
        }
    }

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_closed
 *
 * Description: <function description>
 *
 * Inputs:  ui4_previous_state References.
 *          ui4_event          References.
 *          ui4_curr_state     References.
 *          pv_tag             References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_closed(
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;

    _sm_vs_state_record_latest_event( pt_vs_obj, ui4_event );

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_error
 *
 * Description: <function description>
 *
 * Inputs:  ui4_previous_state References.
 *          ui4_event          References.
 *          ui4_curr_state     References.
 *          pv_tag             References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_error(
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;

    _sm_vs_state_record_latest_event( pt_vs_obj, ui4_event );

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_ignore_excpt
 *
 * Description: <function description>
 *
 * Inputs:  ui4_event      References.
 *          ui4_curr_state References.
 *          pv_tag         References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_ignore_excpt(
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;

    _sm_vs_state_record_latest_event( pt_vs_obj, ui4_event );

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_opening_excpt
 *
 * Description: <function description>
 *
 * Inputs:  ui4_event      References.
 *          ui4_curr_state References.
 *          pv_tag         References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_opening_excpt(
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;

    _sm_vs_state_record_latest_event( pt_vs_obj, ui4_event );

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_opened_excpt
 *
 * Description: <function description>
 *
 * Inputs:  ui4_event      References.
 *          ui4_curr_state References.
 *          pv_tag         References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_opened_excpt(
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;

    _sm_vs_state_record_latest_event( pt_vs_obj, ui4_event );

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_starting_excpt
 *
 * Description: <function description>
 *
 * Inputs:  ui4_event      References.
 *          ui4_curr_state References.
 *          pv_tag         References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_starting_excpt(
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;

    _sm_vs_state_record_latest_event( pt_vs_obj, ui4_event );

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_started_excpt
 *
 * Description: <function description>
 *
 * Inputs:  ui4_event      References.
 *          ui4_curr_state References.
 *          pv_tag         References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_started_excpt(
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;

    _sm_vs_state_record_latest_event( pt_vs_obj, ui4_event );

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_stopping_excpt
 *
 * Description: <function description>
 *
 * Inputs:  ui4_event      References.
 *          ui4_curr_state References.
 *          pv_tag         References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_stopping_excpt(
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;

    _sm_vs_state_record_latest_event( pt_vs_obj, ui4_event );

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_closing_excpt
 *
 * Description: <function description>
 *
 * Inputs:  ui4_event      References.
 *          ui4_curr_state References.
 *          pv_tag         References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_closing_excpt(
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;

    _sm_vs_state_record_latest_event( pt_vs_obj, ui4_event );

    SM_VS_DBG_ERR((
        "[SM] VS(0x%.8X), SM_COND_CLOSING state receive exception event 0x%.8X\r\n",
        pt_vs_obj->t_base.h_stream, ui4_event ));

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_closed_excpt
 *
 * Description: <function description>
 *
 * Inputs:  ui4_event      References.
 *          ui4_curr_state References.
 *          pv_tag         References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_closed_excpt(
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;

    _sm_vs_state_record_latest_event( pt_vs_obj, ui4_event );

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_error_excpt
 *
 * Description: <function description>
 *
 * Inputs:  ui4_event      References.
 *          ui4_curr_state References.
 *          pv_tag         References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_error_excpt(
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;

    _sm_vs_state_record_latest_event( pt_vs_obj, ui4_event );

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_notify_app
 *
 * Description: <function description>
 *
 * Inputs:  pv_tag   References.
 *          e_cond   References.
 *          e_event  References.
 *          ui4_data References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sm_vs_state_notify_app(
    VOID*                 pv_tag,
    SM_COND_T             e_cond,
    UINT32                ui4_event,
    UINT32                ui4_data )
{
    INT32                 i4_ret;
    SM_VS_STATE_TAG_T*    pt_state_tag = (SM_VS_STATE_TAG_T*)pv_tag;
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;
    HANDLE_T              h_stream;

    SM_ASSERT( pt_state_tag );
    SM_ASSERT( pt_state_tag->h_stream != NULL_HANDLE );
    SM_ASSERT( pt_state_tag->pt_vs_obj );

    pt_vs_obj = pt_state_tag->pt_vs_obj;
    h_stream = pt_state_tag->h_stream;


#ifdef TIME_MEASUREMENT
    _sm_vs_state_time_msrt(
        (UINT32)pt_vs_obj->t_base.e_condition,
        (UINT32)e_cond,
        ui4_event );
#endif /* TIME_MEASUREMENT */

    pt_vs_obj->t_base.e_condition = e_cond;

    i4_ret = pt_vs_obj->pf_sess_hdlr_sm_nfy(
        h_stream,
        pt_vs_obj->t_base.e_condition,
        pt_vs_obj->pv_sess_hdlr_sm_nfy_tag,
        ui4_event,
        ui4_data );
    if ( i4_ret != SMR_OK )
    {
        /* do nothing */
    }


    {
        const CHAR* ps_state = sm_state_event_to_str(ui4_event);

        if ( ps_state )
        {
            x_dbg_stmt(
                 "[SM] VS(0x%.8X), %s, %s, DATA(0x%.8X)\r\n",
                (UINT32)h_stream,
                sm_strm_cond_2_str(e_cond),
                ps_state,
                (UINT32)ui4_data );
        }
        else
        {
            x_dbg_stmt(
                 "[SM] VS(0x%.8X), %s, EVN(0x%.8X), DATA(0x%.8X)\r\n",
                (UINT32)h_stream,
                sm_strm_cond_2_str(e_cond),
                (UINT32)ui4_event,
                (UINT32)ui4_data );
        }
    }



    return;
}

/* Event handle functions ****************************************************/
/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_event_nfy
 *
 * Description: <function description>
 *
 * Inputs:  ui4_curr_state References.
 *          ui4_event      References.
 *          ui4_next_state References.
 *          pv_tag         References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_event_nfy(
    UINT32     ui4_curr_state,
    UINT32     ui4_event,
    UINT32     ui4_next_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_VS_STATE_TAG_T             t_state_tag;
    SM_VIDEO_STREAM_T*            pt_vs_obj = NULL;

    SM_ASSERT( pv_tag1 );
    t_state_tag.pt_vs_obj = pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;
    t_state_tag.h_stream = pt_vs_obj->t_base.h_stream;

    sm_vs_state_notify_app(
        &t_state_tag,
        ui4_curr_state,
        ui4_event,
        (UINT32)pv_tag2 );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_ca_events
 *
 * Description: <function description>
 *
 * Inputs:  ui4_curr_state References.
 *          ui4_event      References.
 *          ui4_next_state References.
 *          pv_tag         References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_cci_events(
    UINT32     ui4_curr_state,
    UINT32     ui4_event,
    UINT32     ui4_next_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
#ifndef CI_PLUS_SUPPORT
    UINT32                        ui4_i;
#endif
    INT32                         i4_ret = SMR_OK;
    SM_VS_STATE_TAG_T             t_state_tag;
    CP_INFO_LIST_T*               pt_cci_info_lst = NULL;
    SM_VIDEO_STREAM_T*            pt_vs_obj = NULL;
    HANDLE_T                      h_stream;
    SM_HANDLER_T*                 pt_hdlr_obj = NULL;

    SM_ASSERT( pv_tag1 );
    SM_ASSERT( pv_tag2 );
    t_state_tag.pt_vs_obj = pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;
    t_state_tag.h_stream = h_stream = pt_vs_obj->t_base.h_stream;

    pt_cci_info_lst = (CP_INFO_LIST_T*)pv_tag2;
    pt_vs_obj->t_cp_info_lst = *pt_cci_info_lst;

#ifdef CI_PLUS_SUPPORT
    #if 0
    i4_ret = x_sm_set( h_stream, 
                       SM_STRM_SET_TYPE_CCI_INFO, 
                       (VOID*)pt_cci_info_lst,
                       sizeof(CP_INFO_LIST_T) );
    #else
    i4_ret = handle_get_obj( pt_vs_obj->t_base.h_handler,
                             (VOID**)&pt_hdlr_obj );
    
    i4_ret = pt_hdlr_obj->t_fct_tbl.pf_set( h_stream,
                                            SM_STRM_SET_TYPE_CCI_INFO,
                                            (VOID*)pt_cci_info_lst,
                                            sizeof(CP_INFO_LIST_T) );
    #endif 
    if ( i4_ret != SMR_OK )
    {
        SM_VS_DBG_ERR((
            "[SM] VS, ERROR! x_sm_set(0x%.8x, SM_STRM_SET_TYPE_CCI_INFO) "
            "return %d\r\n",
            h_stream, i4_ret ));
    }
#else
    for( ui4_i = 0; ui4_i < pt_cci_info_lst->ui1_ctrl_size; ui4_i++ )
    {
        if ( ui4_i < CP_TYPE_MAX )
        {
            #if 0
            i4_ret = x_sm_set( h_stream, 
                               SM_STRM_SET_TYPE_CCI_INFO, 
                               (VOID*)&pt_cci_info_lst->at_cp_ctrl_list[ui4_i],
                               sizeof(CP_INFO_T) );
            #else
            i4_ret = handle_get_obj( pt_vs_obj->t_base.h_handler,
                                     (VOID**)&pt_hdlr_obj );
            
            i4_ret = pt_hdlr_obj->t_fct_tbl.pf_set( h_stream,
                                                    SM_STRM_SET_TYPE_CCI_INFO,
                                                    (VOID*)&pt_cci_info_lst->at_cp_ctrl_list[ui4_i],
                                                    sizeof(CP_INFO_T) );
            #endif 
            if ( i4_ret != SMR_OK )
            {
                SM_VS_DBG_ERR((
                    "[SM] VS, ERROR! x_sm_set(0x%.8x, SM_STRM_SET_TYPE_CCI_INFO) "
                    "return %d\r\n",
                    h_stream, i4_ret ));
            }
        }
        else
        {
            SM_VS_DBG_ERR(( 
                "[SM] VS, ERROR! stream (0x%.8x), ui1_ctrl_size outside the bounds) \r\n",
                h_stream ));
            return SMR_INV_ARG;
        }
    }
#endif    

    sm_vs_state_notify_app(
        &t_state_tag,
        (SM_COND_T)pt_vs_obj->t_state.ui4_curr_state,
        ui4_event,
        (UINT32)&pt_vs_obj->t_cp_info_lst );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_ca_events
 *
 * Description: <function description>
 *
 * Inputs:  ui4_curr_state References.
 *          ui4_event      References.
 *          ui4_next_state References.
 *          pv_tag         References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_ca_events(
    UINT32     ui4_curr_state,
    UINT32     ui4_event,
    UINT32     ui4_next_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    UINT32                ui4_detail_code = (UINT32)pv_tag2;
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;

    _sm_vs_state_record_latest_event( pt_vs_obj, ui4_event );

    pt_vs_obj->ui4_lastest_ca_detail_code = ui4_detail_code;

    /* for CR DTV00354322 */
    if( ui4_event == SM_EVN_CA_COND_NOT_AUTHORIZED )
    {
        x_memset( &pt_vs_obj->t_vid_res, 0, sizeof(VSH_SRC_RESOLUTION_INFO_T) );
    }

    if ( ui4_curr_state == SM_COND_STARTED )
    {
        _sm_vs_state_prc_ca_event( pt_vs_obj );
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_drv_events
 *
 * Description: <function description>
 *
 * Inputs:  ui4_curr_state References.
 *          ui4_event      References.
 *          ui4_next_state References.
 *          pv_tag         References.
 *
 * Outputs: -
 *
 * Returns: SMR_DRV_COMP_ERROR  Success.
 *          SMR_OK              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_drv_events(
    UINT32     ui4_curr_state,
    UINT32     ui4_event,
    UINT32     ui4_next_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    INT32                 i4_ret = SMR_OK;
    SM_VIDEO_STREAM_T*    pt_vs_obj = NULL;
    HANDLE_T              h_stream;

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;
    h_stream = pt_vs_obj->t_base.h_stream;

    if ( !_sm_vs_state_record_latest_event( pt_vs_obj, ui4_event ) )
    {
        /* Skip the event if there is no changes */
        return SMR_OK;
    }

    switch ( SM_EVN_GET_GRP(ui4_event) )
    {
    case SM_EVN_VS_GRP_DEC:
    case SM_EVN_VS_GRP_VP:
    case SM_EVN_VS_GRP_PMX:
    case SM_EVN_VS_GRP_TVE:
        i4_ret = _sm_vs_state_do_vid_drv_event( h_stream, pt_vs_obj, ui4_event );
        break;

    case SM_EVN_VS_GRP_PM:
        i4_ret = _sm_vs_state_do_pm_event( h_stream, pt_vs_obj, ui4_event );
        break;

    default:
        SM_VS_DBG_ERR((
            "[SM] VS, sm_vs_state_do_drv_events() receives unhandled event 0x%.8X\r\n",
            ui4_event ));
        SM_ABORT( SM_DBG_ABRT_INV_EVENT );
    }

    if ( i4_ret != SMR_OK )
    {
        sm_vs_send_msg_do_event(
            h_stream,
            SM_EVN_VS_DRV_ERR,
            0 );
        return SMR_DRV_COMP_ERROR;
    }

    if ( IS_MPEG_VID_STRM(pt_vs_obj) || IS_MULTIMEDIA_VID_STRM(pt_vs_obj) )
    {
        if ( SM_EVN_GET_GRP(ui4_event) != SM_EVN_VS_GRP_PM &&
             pt_vs_obj->t_vs_comp_grp.e_grp_cond == SM_VS_COMP_GRP_COND_CONNECTED &&
             ( pt_vs_obj->ui4_lastest_pm_event == SM_EVN_VS_PM_COND_STOPPED ||
               pt_vs_obj->ui4_lastest_pm_event == SM_EVN_VS_PM_COND_PLAYED ) )
        {
            sm_vs_send_msg_do_event(
                h_stream,
                SM_EVN_VS_DRV_OPENED,
                0 );
        }
        else if ( pt_vs_obj->t_vs_comp_grp.e_grp_cond == SM_VS_COMP_GRP_COND_CONNECTED &&
                  ui4_event == SM_EVN_VS_PM_COND_STOPPED )
        {
            sm_vs_send_msg_do_event(
                h_stream,
                SM_EVN_VS_DRV_OPENED,
                0 );
        }
        else if ( pt_vs_obj->t_vs_comp_grp.e_grp_cond == SM_VS_COMP_GRP_COND_STARTED &&
                  pt_vs_obj->ui4_lastest_pm_event == SM_EVN_VS_PM_COND_PLAYED )
        {
            sm_vs_send_msg_do_event(
                h_stream,
                SM_EVN_VS_DRV_PLAYED,
                0 );
        }
        else if ( SM_EVN_GET_GRP(ui4_event) != SM_EVN_VS_GRP_PM &&
                  pt_vs_obj->t_vs_comp_grp.e_grp_cond == SM_VS_COMP_GRP_COND_STOPPED &&
                  pt_vs_obj->ui4_lastest_pm_event == SM_EVN_VS_PM_COND_CLOSED )
        {
            /* If current event is Video driver events, and PM is already closed by
                     session layer, simulate a driver stopped message. */
            sm_vs_send_msg_do_event(
                h_stream,
                SM_EVN_VS_DRV_STOPPED,
                0 );
        }
        else if ( pt_vs_obj->t_vs_comp_grp.e_grp_cond == SM_VS_COMP_GRP_COND_STOPPED &&
                  pt_vs_obj->ui4_lastest_pm_event == SM_EVN_VS_PM_COND_STOPPED )
        {
            sm_vs_send_msg_do_event(
                h_stream,
                SM_EVN_VS_DRV_STOPPED,
                0 );
        }
        else if ( pt_vs_obj->t_vs_comp_grp.e_grp_cond == SM_VS_COMP_GRP_COND_CLOSED &&
                  ( pt_vs_obj->ui4_lastest_pm_event == SM_EVN_VS_PM_COND_CLOSED ||
                    pt_vs_obj->ui4_lastest_pm_event == SM_EVN_IGNORE ) )
        {
            sm_vs_send_msg_do_event(
                h_stream,
                SM_EVN_VS_DRV_CLOSED,
                0 );
        }
    }
    else /* Analog video stream */
    {
        if ( pt_vs_obj->t_vs_comp_grp.e_grp_cond == SM_VS_COMP_GRP_COND_CONNECTED )
        {
            sm_vs_send_msg_do_event(
                h_stream,
                SM_EVN_VS_DRV_OPENED,
                0 );
        }
        else if ( pt_vs_obj->t_vs_comp_grp.e_grp_cond == SM_VS_COMP_GRP_COND_STARTED )
        {
            sm_vs_send_msg_do_event(
                h_stream,
                SM_EVN_VS_DRV_PLAYED,
                0 );
        }
        else if ( pt_vs_obj->t_vs_comp_grp.e_grp_cond == SM_VS_COMP_GRP_COND_STOPPED )
        {
            sm_vs_send_msg_do_event(
                h_stream,
                SM_EVN_VS_DRV_STOPPED,
                0 );
        }
        else if ( pt_vs_obj->t_vs_comp_grp.e_grp_cond == SM_VS_COMP_GRP_COND_CLOSED )
        {
            sm_vs_send_msg_do_event(
                h_stream,
                SM_EVN_VS_DRV_CLOSED,
                0 );
        }
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_res_chg
 *
 * Description: <function description>
 *
 * Inputs:  ui4_curr_state References.
 *          ui4_event      References.
 *          ui4_next_state References.
 *          pv_tag         References.
 *
 * Outputs: -
 *
 * Returns: SMR_DRV_COMP_ERROR  Success.
 *          SMR_OK              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_res_chg(
    UINT32     ui4_curr_state,
    UINT32     ui4_event,
    UINT32     ui4_next_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    INT32                      i4_ret;
    SIZE_T                     z_size;
    VSH_SRC_RESOLUTION_INFO_T  t_res;
    SM_VS_STATE_TAG_T          t_state_tag;
    SM_VIDEO_STREAM_T*         pt_vs_obj = NULL;
    HANDLE_T                   h_stream;
    SM_HANDLER_T*              pt_hdlr_obj = NULL;

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;
    h_stream = pt_vs_obj->t_base.h_stream;

    if ( ui4_curr_state == SM_COND_STARTING )
    {
        pt_vs_obj->b_pending_res_chg = TRUE;
        return SMR_OK;
    }
    if ( ui4_curr_state != SM_COND_STARTED )
    {
        return SMR_OK;
    }

    z_size = sizeof( VSH_SRC_RESOLUTION_INFO_T );
#if 0    
    i4_ret = x_sm_get( h_stream,
                       SM_VSH_GET_TYPE_VIDEO_SRC_RESOLUTION,
                       &t_res,
                       &z_size );
#else 
    i4_ret = handle_get_obj( pt_vs_obj->t_base.h_handler,
                             (VOID**)&pt_hdlr_obj );
            
    i4_ret = pt_hdlr_obj->t_fct_tbl.pf_get( h_stream,
                                            SM_VSH_GET_TYPE_VIDEO_SRC_RESOLUTION,
                                            &t_res,
                                            &z_size );
#endif
    if ( i4_ret != SMR_OK )
    {
        SM_VS_DBG_ERR(( "[SM] VS, Error SM_VSH_GET_TYPE_VIDEO_SRC_RESOLUTION\r\n"  ));
        return SMR_INTERNAL_ERROR;
    }

    if ( x_memcmp( &pt_vs_obj->t_vid_res, &t_res, z_size ) != 0 )
    {
        pt_vs_obj->t_vid_res = t_res;

        t_state_tag.h_stream = h_stream;
        t_state_tag.pt_vs_obj = pt_vs_obj;

        x_dbg_stmt(
            "[SM] VS: res_chg(%u x %u), %c, F.R.(%u), ASP(%u), TIMING(%u), COLOR_FMT(%u), "
            "COLOR_SPACE(%u), RES_IDX(%u), WSS(%u),TAG3D(%u)\r\n",
            t_res.ui4_width, t_res.ui4_height,
            ((t_res.b_is_progressive)?'P':'I'),
            t_res.ui4_frame_rate,
            t_res.e_src_asp_ratio,
            t_res.e_timing_type,
            t_res.e_color_fmt,
            t_res.e_src_color_space,
            t_res.ui4_res_idx,
            t_res.ui4_ext,
            t_res.e_src_tag3d_type );

        sm_vs_state_notify_app(
            &t_state_tag,
            ui4_curr_state,
            ui4_event,
            (UINT32)&pt_vs_obj->t_vid_res );

        pt_vs_obj->b_pending_res_chg = FALSE;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_auto_done
 *
 * Description: <function description>
 *
 * Inputs:  ui4_curr_state References.
 *          ui4_event      References.
 *          ui4_next_state References.
 *          pv_tag         References.
 *
 * Outputs: -
 *
 * Returns: SMR_DRV_COMP_ERROR  Success.
 *          SMR_OK              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_auto_done(
    UINT32     ui4_curr_state,
    UINT32     ui4_event,
    UINT32     ui4_next_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_VS_STATE_TAG_T          t_state_tag;
    SM_VIDEO_STREAM_T*         pt_vs_obj = NULL;
    HANDLE_T                   h_stream;

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;
    h_stream = pt_vs_obj->t_base.h_stream;

    t_state_tag.h_stream = h_stream;
    t_state_tag.pt_vs_obj = pt_vs_obj;

    sm_vs_state_notify_app(
        &t_state_tag,
        ui4_curr_state,
        ui4_event,
        (UINT32)pv_tag2 );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_tv_sig
 *
 * Description: <function description>
 *
 * Inputs:  ui4_curr_state References.
 *          ui4_event      References.
 *          ui4_next_state References.
 *          pv_tag         References.
 *
 * Outputs: -
 *
 * Returns: SMR_DRV_COMP_ERROR  Success.
 *          SMR_OK              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_tv_sig(
    UINT32     ui4_curr_state,
    UINT32     ui4_event,
    UINT32     ui4_next_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_VS_STATE_TAG_T          t_state_tag;
    SM_VIDEO_STREAM_T*         pt_vs_obj = NULL;
    HANDLE_T                   h_stream;
    VSH_ANA_TV_SIG_STATUS_T    e_vsh_tv_sig_status = VSH_ANA_TV_SIG_UNKNOWN;

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;
    h_stream = pt_vs_obj->t_base.h_stream;

    t_state_tag.h_stream = h_stream;
    t_state_tag.pt_vs_obj = pt_vs_obj;

    SM_VS_DBG_INFO((
        "[SM] VS: Pre-Notify APP TV-signal(%d)\r\n", (INT32)pv_tag2 ));

    if ( ui4_event == SM_EVN_VS_ANA_TV_SIGNAL_DETECTING )
    {
        SM_VS_DBG_INFO((
            "[SM] VS: Notify APP TV-signal detecting(%d)\r\n", (INT32)pv_tag2 ));

        sm_vs_state_notify_app(
            &t_state_tag,
            ui4_curr_state,
            ui4_event,
            (UINT32)pv_tag2  );
    }
    else if ( (TV_DEC_TV_SIG_T)(UINT32)pv_tag2 != pt_vs_obj->e_tv_signal &&
              ui4_curr_state == SM_COND_STARTED )
    {
        pt_vs_obj->e_tv_signal = (TV_DEC_TV_SIG_T)(UINT32)pv_tag2;

        if ( pt_vs_obj->e_tv_signal == TV_DEC_TV_SIG_NONE )
        {
            x_memset( &pt_vs_obj->t_vid_res,
                      0,
                      sizeof(VSH_SRC_RESOLUTION_INFO_T) );
            pt_vs_obj->e_color_sys = COLOR_SYS_UNKNOWN;
        }
        
        switch ( pt_vs_obj->e_tv_signal )
        {
        case TV_DEC_TV_SIG_NONE:
            e_vsh_tv_sig_status = VSH_ANA_TV_SIG_NONE;
            break;

        case TV_DEC_TV_SIG_STABLE:
            e_vsh_tv_sig_status = VSH_ANA_TV_SIG_STABLE;
            break;

        case TV_DEC_TV_SIG_UNKNOWN:
        default:
            e_vsh_tv_sig_status = VSH_ANA_TV_SIG_UNKNOWN;
            break;
        }

        SM_VS_DBG_INFO((
            "[SM] VS: Notify APP TV-signal(%d)\r\n", (INT32)pv_tag2 ));

        sm_vs_state_notify_app(
            &t_state_tag,
            ui4_curr_state,
            ui4_event,
            (UINT32)e_vsh_tv_sig_status );
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_dec_status
 *
 * Description: <function description>
 *
 * Inputs:  ui4_curr_state References.
 *          ui4_event      References.
 *          ui4_next_state References.
 *          pv_tag         References.
 *
 * Outputs: -
 *
 * Returns: SMR_DRV_COMP_ERROR  Success.
 *          SMR_OK              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_dec_status(
    UINT32     ui4_curr_state,
    UINT32     ui4_event,
    UINT32     ui4_next_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_VS_STATE_TAG_T          t_state_tag;
    SM_VIDEO_STREAM_T*         pt_vs_obj = NULL;
    HANDLE_T                   h_stream;
    VSH_DIG_DECODE_STATUS_T    e_vsh_dec_status = VSH_DIG_DECODE_UNKNOWN;

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;
    h_stream = pt_vs_obj->t_base.h_stream;

    t_state_tag.h_stream = h_stream;
    t_state_tag.pt_vs_obj = pt_vs_obj;

    SM_VS_DBG_INFO((
        "[SM] VS: Pre-Notify APP VD decode status(%d)\r\n", (INT32)pv_tag2 ));

    if ( (VID_DEC_DECODE_STATUS_T)(UINT32)pv_tag2 != VID_DEC_DECODE_UNKNOWN &&
         (VID_DEC_DECODE_STATUS_T)(UINT32)pv_tag2 != pt_vs_obj->e_dec_status &&
         ui4_curr_state == SM_COND_STARTED )
    {
        pt_vs_obj->e_dec_status = (VID_DEC_DECODE_STATUS_T)(UINT32)pv_tag2;

        switch ( pt_vs_obj->e_dec_status )
        {
        case VID_DEC_DECODE_NO_DATA:
            e_vsh_dec_status = VSH_DIG_DECODE_NO_DATA;
            break;

        case VID_DEC_DECODE_NORMAL:
            e_vsh_dec_status = VSH_DIG_DECODE_NORMAL;
            break;

        case VID_DEC_DECODE_ERROR:
            e_vsh_dec_status = VSH_DIG_DECODE_ERROR;
            break;

        case VID_DEC_DECODE_HD_NOT_SUPPORT:
            e_vsh_dec_status = VSH_DIG_DECODE_HD_NOT_SUPPORT;
            break;

        case VID_DEC_DECODE_CODEC_NOT_SUPPORT:
            e_vsh_dec_status = VSH_DIG_DECODE_CODEC_NOT_SUPPORT;
            break;

        case VID_DEC_DECODE_DATA_RECEIVE:
            e_vsh_dec_status = VSH_DIG_DECODE_DATA_RECEIVE;
            break;

        case VID_DEC_DECODE_RES_NOT_SUPPORT:
            e_vsh_dec_status = VSH_DIG_DECODE_RES_NOT_SUPPORT;
            break;

        case VID_DEC_DECODE_CODEC_TYPE_CHG:
            return SMR_OK;

        case VID_DEC_DECODE_UNKNOWN:
        default:
            e_vsh_dec_status = VSH_DIG_DECODE_UNKNOWN;
        }

        SM_VS_DBG_INFO((
            "[SM] VS: Notify APP VD decode status(%d)\r\n", (INT32)pv_tag2 ));

        sm_vs_state_notify_app(
            &t_state_tag,
            ui4_curr_state,
            ui4_event,
            (UINT32)e_vsh_dec_status );
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_color_sys
 *
 * Description: <function description>
 *
 * Inputs:  ui4_curr_state References.
 *          ui4_event      References.
 *          ui4_next_state References.
 *          pv_tag         References.
 *
 * Outputs: -
 *
 * Returns: SMR_DRV_COMP_ERROR  Success.
 *          SMR_OK              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_color_sys(
    UINT32     ui4_curr_state,
    UINT32     ui4_event,
    UINT32     ui4_next_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_VS_STATE_TAG_T          t_state_tag;
    SM_VIDEO_STREAM_T*         pt_vs_obj = NULL;
    HANDLE_T                   h_stream;

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;
    h_stream = pt_vs_obj->t_base.h_stream;

    if ( ui4_curr_state != SM_COND_STARTED )
    {
        return SMR_OK;
    }

    t_state_tag.h_stream = h_stream;
    t_state_tag.pt_vs_obj = pt_vs_obj;

    pt_vs_obj->e_color_sys = (COLOR_SYS_T)(UINT32)pv_tag2;

    sm_vs_state_notify_app(
        &t_state_tag,
        ui4_curr_state,
        ui4_event,
        (UINT32)pv_tag2 );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_cnt_adv
 *
 * Description: <function description>
 *
 * Inputs:  ui4_curr_state References.
 *          ui4_event      References.
 *          ui4_next_state References.
 *          pv_tag         References.
 *
 * Outputs: -
 *
 * Returns: SMR_DRV_COMP_ERROR  Success.
 *          SMR_OK              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_cnt_adv(
    UINT32     ui4_curr_state,
    UINT32     ui4_event,
    UINT32     ui4_next_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_VS_STATE_TAG_T          t_state_tag;
    SM_VIDEO_STREAM_T*         pt_vs_obj = NULL;
    HANDLE_T                   h_stream;

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;
    h_stream = pt_vs_obj->t_base.h_stream;

    t_state_tag.h_stream = h_stream;
    t_state_tag.pt_vs_obj = pt_vs_obj;

    pt_vs_obj->t_cnt_adv = *(VSH_CNT_ADV_INFO_T*)pv_tag2;

    sm_vs_state_notify_app(
        &t_state_tag,
        ui4_curr_state,
        ui4_event,
        (UINT32)pv_tag2 );

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_vs_state_do_cc_ind
 *
 * Description: <function description>
 *
 * Inputs:  ui4_curr_state References.
 *          ui4_event      References.
 *          ui4_next_state References.
 *          pv_tag         References.
 *
 * Outputs: -
 *
 * Returns: SMR_DRV_COMP_ERROR  Success.
 *          SMR_OK              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_vs_state_do_cc_ind(
    UINT32     ui4_curr_state,
    UINT32     ui4_event,
    UINT32     ui4_next_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_VS_STATE_TAG_T          t_state_tag;
    SM_VIDEO_STREAM_T*         pt_vs_obj = NULL;
    HANDLE_T                   h_stream;

    SM_ASSERT( pv_tag1 );
    pt_vs_obj = (SM_VIDEO_STREAM_T*)pv_tag1;
    h_stream = pt_vs_obj->t_base.h_stream;

    t_state_tag.h_stream = h_stream;
    t_state_tag.pt_vs_obj = pt_vs_obj;

    SM_VS_DBG_INFO(( "[SM] VS: CC indicator %s\r\n",
        ((pv_tag2)? "TRUE": "FALSE") ));

    sm_vs_state_notify_app(
        &t_state_tag,
        ui4_curr_state,
        ui4_event,
        (UINT32)pv_tag2 );

    return SMR_OK;
}
