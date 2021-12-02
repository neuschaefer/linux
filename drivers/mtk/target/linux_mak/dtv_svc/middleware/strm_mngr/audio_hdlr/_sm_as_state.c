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
 * $RCSfile: _sm_as_state.c,v $
 * $Revision: #2 $
 * $Date: 2012/07/20 $
 * $Author: jiangbo.wang $
 * $CCRevision: /main/DTV_X_HQ_int/13 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: 4daba80c66c947097fcbfa73ed527b7e $
 *
 * Description: 
 *         This header file contains Stream Manager Audio Stream state 
 *         machine specific implementations.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "handle/x_handle.h"

#include "prc_mngr/prc_mngr.h"

#include "strm_mngr/_sm_utils.h"
#include "strm_mngr/sm_state.h"
#include "strm_mngr/audio_hdlr/_sm_as_state.h"
#include "strm_mngr/audio_hdlr/_sm_as_ad.h"
#include "strm_mngr/scc/x_scc.h"
#include "conn_mngr/x_cm.h"
#include "conn_mngr/brdcst/u_brdcst.h"
#include "res_mngr/drv/x_tuner.h"

#ifdef DEBUG
#include "strm_mngr/sm_cli.h"
#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#ifdef TIME_MEASUREMENT
#define AS_HANDLE_IGNORE_2_OPEN_STR            "AS:IGNORE-OPENED"
#define AS_HANDLE_IGNORE_2_STARTED_STR         "AS:IGNORE-STARTED"
#define AS_HANDLE_STARTED_2_STOPPED_STR        "AS:STARTED-STOPPED"
#define AS_HANDLE_STARTED_2_CLOSED_STR         "AS:STARTED-CLOSED"

#define AS_HANDLE_PM_OPENED                    "AS:PM-OPENED"
#define AS_HANDLE_PM_STARTED                   "AS:PM-STARTED"
#define AS_HANDLE_PM_CLOSED                    "AS:PM-CLOSED"

#endif /* TIME_MEASUREMENT */

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/
/*extern VOID dvb_get_country(ISO_3166_COUNT_T t_country_code);*/

#ifdef TIME_MEASUREMENT
static VOID _sm_as_state_time_msrt( 
    UINT32      ui4_curr_state,
    UINT32      ui4_next_state,
    UINT32      ui4_event )
{
    if ( ui4_curr_state == SM_COND_IGNORE && 
         ui4_next_state == SM_COND_OPENING )
    {
        TMS_BEGIN( AS_HANDLE_IGNORE_2_OPEN_STR );
        TMS_BEGIN_EX(TMS_FLAG_CHG_CHL, AS_HANDLE_IGNORE_2_OPEN_STR, AS_HANDLE_IGNORE_2_OPEN_STR);
        TMS_BEGIN( AS_HANDLE_IGNORE_2_STARTED_STR );
        TMS_BEGIN_EX(TMS_FLAG_CHG_CHL, AS_HANDLE_IGNORE_2_STARTED_STR, AS_HANDLE_IGNORE_2_STARTED_STR);
    }
    else if ( ui4_curr_state == SM_COND_OPENING && 
              ui4_next_state == SM_COND_OPENED )
    {
        TMS_END( AS_HANDLE_IGNORE_2_OPEN_STR );
        TMS_END_EX(TMS_FLAG_CHG_CHL, AS_HANDLE_IGNORE_2_OPEN_STR, AS_HANDLE_IGNORE_2_OPEN_STR);
    }
    else if ( ui4_curr_state == SM_COND_STARTING && 
              ui4_next_state == SM_COND_STARTED )
    {
        TMS_END( AS_HANDLE_IGNORE_2_STARTED_STR );
        TMS_END_EX(TMS_FLAG_CHG_CHL, AS_HANDLE_IGNORE_2_STARTED_STR, AS_HANDLE_IGNORE_2_STARTED_STR);
    }
    else if ( ui4_curr_state == SM_COND_STARTED && 
              ui4_next_state == SM_COND_STOPPING )
    {
        TMS_BEGIN( AS_HANDLE_STARTED_2_STOPPED_STR );
        TMS_BEGIN_EX(TMS_FLAG_CHG_CHL, AS_HANDLE_STARTED_2_STOPPED_STR, AS_HANDLE_STARTED_2_STOPPED_STR);
        TMS_BEGIN( AS_HANDLE_STARTED_2_CLOSED_STR );
        TMS_BEGIN_EX(TMS_FLAG_CHG_CHL, AS_HANDLE_STARTED_2_CLOSED_STR, AS_HANDLE_STARTED_2_CLOSED_STR);
    }
    else if ( ui4_curr_state == SM_COND_STOPPING && 
              ui4_next_state == SM_COND_OPENED )
    {
        TMS_END( AS_HANDLE_STARTED_2_STOPPED_STR );
        TMS_END_EX(TMS_FLAG_CHG_CHL, AS_HANDLE_STARTED_2_STOPPED_STR, AS_HANDLE_STARTED_2_STOPPED_STR);
    }
    else if ( ui4_curr_state == SM_COND_CLOSING && 
              ui4_next_state == SM_COND_CLOSED )
    {
        TMS_END( AS_HANDLE_STARTED_2_CLOSED_STR );
        TMS_END_EX(TMS_FLAG_CHG_CHL, AS_HANDLE_STARTED_2_CLOSED_STR, AS_HANDLE_STARTED_2_CLOSED_STR);
    }
}

static VOID _sm_as_drv_time_msrt( UINT32 ui4_event )
{

    switch( ui4_event )
    {
    case SM_EVN_AS_AD_OPENED:
        TMS_END( AS_HANDLE_AD_OPENED );
        break;
    
    case SM_EVN_AS_AD_CLOSED:
        TMS_END( AS_HANDLE_AD_CLOSED );
        break;
    
    case SM_EVN_AS_AD_CONNECTED:
        TMS_END( AS_HANDLE_AD_CONNECTED );
        break;
    
    case SM_EVN_AS_AD_DISCONNECTED:
        TMS_END( AS_HANDLE_AD_DISCONNECTED );
        break;
    
    case SM_EVN_AS_AD_PLAYED:
    case SM_EVN_AS_AD_PAUSED:
        TMS_END( AS_HANDLE_AD_STARTED );
        break;
    
    case SM_EVN_AS_AD_STOPPED:
        TMS_END( AS_HANDLE_AD_STOPPED );
        break;

    default:
        break;    
    }
}

#endif /* TIME_MEASUREMENT */

static INT32 _sm_as_state_do_play_drv(
    HANDLE_T              h_stream,
    UINT32                  ui4_event,
    SM_AUDIO_STREAM_T*    pt_as_obj,
    VOID*                pv_tag )
{
    INT32                 i4_ret;
    i4_ret = sm_as_ad_play_drvs( h_stream, pt_as_obj, (INT32)pv_tag );
    if ( i4_ret != SMR_OK )
    {
        SM_AS_DBG_ERR((
            "[SM] AS: sm_as_ad_play_drvs(0x%.8x) error return %d\r\n",
            (UINT32)h_stream, i4_ret ));
        return SMR_DRV_COMP_ERROR;
    }
    
    /* set Process Manager components */
    if ( ui4_event == SM_EVN_AS_APP_PLAY_REQ ||
         pt_as_obj->ui4_lastest_pm_event == SM_EVN_AS_PM_COND_STOPPED )
    {
        if ( !IS_ANALOG_AUD_STRM(pt_as_obj) && 
             !IS_DIGITAL_AUD_STRM(pt_as_obj) )
        {
            i4_ret = pm_play_component( pt_as_obj->t_base.h_pm_comp );
            if ( i4_ret != PMR_OK )
            {
                SM_AS_DBG_ERR((
                    "[SM] AS: pm_play_component(0x%.8x) error return %d\r\n",
                    (UINT32)pt_as_obj->t_base.h_pm_comp, i4_ret ));
                return SMR_INTERNAL_ERROR;    
            }
            SM_AS_DBG_ERR(( "[SM] PM play audio component done\r\n" ));
        }
    }
    
    return SMR_OK;

}


/*-----------------------------------------------------------------------------
 * Name: _sm_as_pm_ca_cond_2_ash
 *
 * Description: <function description>
 *
 * Inputs:  e_ca_cond   References.
 *
 * Outputs: -
 *
 * Returns: ASH defined CA events
 *
 ----------------------------------------------------------------------------*/
static UINT32 _sm_as_pm_ca_cond_2_ash( PM_CA_COND_T e_ca_cond )
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
    case PM_CA_COND_AUTHORIZED_CA_SYS_ID:
        return SM_EVN_CA_COND_AUTHORIZED;

    case PM_CA_COND_NOT_AUTHORIZED:
        return SM_EVN_CA_COND_NOT_AUTHORIZED;

    default:
        return SM_EVN_CA_COND_CLOSED;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _sm_as_state_prc_ca_event
 *
 * Description: <function description>
 *
 * Inputs:  pt_as_obj   References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_as_state_prc_ca_event( SM_AUDIO_STREAM_T*    pt_as_obj )
{
    SM_AS_STATE_TAG_T     t_state_tag;

    t_state_tag.h_stream = (HANDLE_T)pt_as_obj->t_base.h_stream;
    t_state_tag.pt_as_obj = pt_as_obj;
    
    switch( pt_as_obj->ui4_lastest_ca_event )
    {
    case SM_EVN_CA_COND_CLOSED:
    case SM_EVN_CA_COND_UNKNOWN:
    case SM_EVN_CA_COND_FREE:
    case SM_EVN_CA_COND_AUTHORIZED:
        /*sm_as_ad_play_drvs(pt_as_obj->t_base.h_stream, pt_as_obj);*/
        break;
    
    case SM_EVN_CA_COND_NOT_AUTHORIZED:
        /*sm_as_ad_stop_drvs(pt_as_obj->t_base.h_stream, pt_as_obj);*/
        break;

    default:
        SM_AS_DBG_INFO((
             "[SM] AS(0x%.8X), ERROR! Unknown CA event %u.\r\n",
             (UINT32)pt_as_obj->t_base.h_stream,
             pt_as_obj->ui4_lastest_ca_event ));
        return;
    }
        
    sm_as_state_notify_app(
        &t_state_tag,
        (SM_COND_T)pt_as_obj->t_state.ui4_curr_state,
        pt_as_obj->ui4_lastest_ca_event,
        pt_as_obj->ui4_lastest_ca_detail_code );
}

/*-----------------------------------------------------------------------------
 * Name: _sm_as_state_scdb_nfy_fct
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
static VOID _sm_as_state_scdb_nfy_fct( 
    HANDLE_T        h_scdb,
    SCDB_COND_T     e_cond,
    UINT32          ui4_reason,
    VOID*           pv_tag,
    UINT32          ui4_data )
{
    SM_AUDIO_HDLR_MSG_T            t_msg;
    
    if ( !x_handle_valid( (HANDLE_T)pv_tag ) )
    {
        return;
    }
    
    t_msg.e_msg_type = SM_AUDIO_HDLR_MSG_SCDB_NFY;
    t_msg.h_stream = (HANDLE_T)pv_tag;
    
    t_msg.u.t_msg_scdb.e_scdb_cond = e_cond;
    t_msg.u.t_msg_scdb.ui4_reason = ui4_reason;
    t_msg.u.t_msg_scdb.ui4_data = ui4_data;
    if ((e_cond == SCDB_UPDATED && (ui4_reason & (SCDB_RECORD_DEL | SCDB_RECORD_MOD)) != 0 ) ||
        (e_cond == SCDB_CLOSED))
    {
        sm_as_send_msg( &t_msg);
    }
}

/*-----------------------------------------------------------------------------
 * Name: _sm_as_cp_nfy
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
static VOID _sm_as_cp_nfy(
    HANDLE_T        h_component, 
    CP_INFO_LIST_T  t_cp_info,
    VOID*           pv_tag )
{
    CP_INFO_LIST_T*      pt_cp_info_lst = (CP_INFO_LIST_T*)&t_cp_info;
    SM_AUDIO_HDLR_MSG_T  t_msg;
    
    if ( h_component==NULL_HANDLE || !pv_tag )
    {
        return;
    }
    
    t_msg.e_msg_type = SM_AUDIO_HDLR_MSG_CCI_NFY;
    t_msg.h_stream = (HANDLE_T)pv_tag;
    t_msg.u.t_msg_cci.h_component = h_component;
    t_msg.u.t_msg_cci.t_pm_cp_info_lst.ui1_ctrl_size= pt_cp_info_lst->ui1_ctrl_size;
    
    if ( t_msg.u.t_msg_cci.t_pm_cp_info_lst.ui1_ctrl_size > CP_TYPE_MAX )
    {
        SM_AS_DBG_ERR(( 
            "[SM] $s:%d, ERROR! _sm_as_cp_nfy(0x%.8X) with invalid size %u!\r\n", 
            __FILE__, __LINE__, (UINT32)t_msg.h_stream, 
            t_msg.u.t_msg_cci.t_pm_cp_info_lst.ui1_ctrl_size));
        return;
    } 
    
    x_memcpy( t_msg.u.t_msg_cci.t_pm_cp_info_lst.at_cp_ctrl_list, 
              pt_cp_info_lst->at_cp_ctrl_list,
              sizeof( CP_INFO_T ) * pt_cp_info_lst->ui1_ctrl_size );
    
    sm_as_send_msg( &t_msg );
}

/*-----------------------------------------------------------------------------
 * Name: _sm_as_pm_ca_nfy
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
static VOID _sm_as_pm_ca_nfy( 
    HANDLE_T       h_component,
    PM_CA_COND_T   e_nfy_cond,
    UINT32         ui4_detail_code,
    VOID*          pv_nfy_tag)
{
    SM_AUDIO_HDLR_MSG_T            t_msg;
    
    if ( !x_handle_valid( (HANDLE_T)pv_nfy_tag ) )
    {
        return;
    }
    
    t_msg.e_msg_type = SM_AUDIO_HDLR_MSG_CA_NFY;
    t_msg.h_stream = (HANDLE_T)pv_nfy_tag;
    
    t_msg.u.t_msg_ca.ui4_detail_code = ui4_detail_code;
    t_msg.u.t_msg_ca.ui4_ca_event = _sm_as_pm_ca_cond_2_ash(e_nfy_cond);

    sm_as_send_msg( &t_msg);
}

/*-----------------------------------------------------------------------------
 * Name: _sm_as_pm_nfy
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
static VOID _sm_as_pm_nfy( 
    HANDLE_T    h_component,
    PM_COND_T   e_nfy_cond,
    UINT32      ui4_detail_code,
    VOID*       pv_nfy_tag )
{
    HANDLE_T                    h_stream = (HANDLE_T)pv_nfy_tag;
    SM_AUDIO_HDLR_MSG_T         t_msg;
    
    if ( !x_handle_valid( h_stream ) )
    {
        return;
    }
    
    t_msg.e_msg_type = SM_AUDIO_HDLR_MSG_PM_NFY;
    t_msg.h_stream = (HANDLE_T)pv_nfy_tag;
    
    switch( e_nfy_cond )
    {
    case PM_COND_CLOSED:
        t_msg.u.t_msg_pm.ui4_pm_event = SM_EVN_AS_PM_COND_CLOSED;
        break;
        
    case PM_COND_PLAYED:
        t_msg.u.t_msg_pm.ui4_pm_event = SM_EVN_AS_PM_COND_PLAYED;
        break;

    case PM_COND_STOPPED:
        t_msg.u.t_msg_pm.ui4_pm_event = SM_EVN_AS_PM_COND_STOPPED;
        break;
        
    case PM_COND_OPENING:
    case PM_COND_STOPPING:
    case PM_COND_CLOSING:
    case PM_COND_PLAYING:
    default:
        return;
    }
    
    t_msg.u.t_msg_pm.ui4_detail_code = ui4_detail_code;
    
    sm_as_send_msg( (VOID*)&t_msg );
}

/*-----------------------------------------------------------------------------
 * Name: _sm_as_state_record_latest_event
 *
 * Description: <function description>
 *
 * Inputs:  pt_as_obj References.
 *          ui4_event References.
 *
 * Outputs: - 
 *
 * Returns: TRUE    Event is updated.
 *          FALSE   No changes.
 *
 ----------------------------------------------------------------------------*/
static BOOL _sm_as_state_record_latest_event(
    SM_AUDIO_STREAM_T*    pt_as_obj,
    UINT32                ui4_event )
{
    switch ( SM_EVN_GET_GRP(ui4_event) )
    {
    case SM_EVN_AS_GRP_AD:
        if ( pt_as_obj->ui4_lastest_ad_event == ui4_event )
        {
            return FALSE;
        }
        pt_as_obj->ui4_lastest_ad_event = ui4_event;
        break;
        
    case SM_EVN_AS_GRP_PM:
        if ( pt_as_obj->ui4_lastest_pm_event == ui4_event )
        {
            return FALSE;
        }
        pt_as_obj->ui4_lastest_pm_event = ui4_event;
        break;

    case SM_EVN_GRP_CA:
        if ( pt_as_obj->ui4_lastest_ca_event == ui4_event )
        {
            return FALSE;
        }
        pt_as_obj->ui4_lastest_ca_event = ui4_event;
        break;
        
    case SM_EVN_AS_GRP_APP:
    default:
        /* Ignore un-handled events */
        return FALSE;
    }

#ifdef TIME_MEASUREMENT
    _sm_as_drv_time_msrt( ui4_event );
#endif /* TIME_MEASUREMENT */
    
    return TRUE;
}

static INT32 _sm_as_state_do_pm_event(
    HANDLE_T              h_stream,
    SM_AUDIO_STREAM_T*    pt_as_obj,
    UINT32                ui4_event )
{
    switch ( ui4_event )
    {
    case SM_EVN_AS_PM_COND_CLOSED:
        pt_as_obj->t_base.h_pm_comp = NULL_HANDLE;
        break;

    case SM_EVN_AS_PM_COND_PLAYED:
        break;
    
    case SM_EVN_AS_PM_COND_STOPPED:
        break;
    
    default:
        break;
    }
    
    return SMR_OK;
}

static INT32 _sm_as_state_do_ad_event(
    HANDLE_T              h_stream,
    SM_AUDIO_STREAM_T*    pt_as_obj,
    UINT32                ui4_event )
{
    INT32 i4_ret;
    
    switch ( ui4_event )
    {
    case SM_EVN_AS_AD_OPENED:
        /* connect audio decoder */
        i4_ret = sm_as_ad_connect( h_stream, pt_as_obj );
        if ( i4_ret != SMR_OK )
        {
            SM_AS_DBG_INFO((
                "[SM] AS: sm_as_ad_connect(0x%.8x) error return %d\r\n",
                (UINT32)h_stream, i4_ret ));

            sm_as_send_msg_do_event( 
                h_stream,
                SM_EVN_AS_DRV_ERR,
                0 );
            return i4_ret;
        }
        break;

    case SM_EVN_AS_AD_CLOSED:
        break;

    case SM_EVN_AS_AD_CONNECTED:
        break;

    case SM_EVN_AS_AD_DISCONNECTED:
        i4_ret = sm_as_close_ad( h_stream, pt_as_obj );
        if ( i4_ret != SMR_OK )
        {
            SM_AS_DBG_INFO((
                "[SM] AS: sm_as_close_ad(0x%.8x) error return %d\r\n",
                (UINT32)h_stream, i4_ret ));
        }
        break;

    case SM_EVN_AS_AD_PLAYED:
        break;

    case SM_EVN_AS_AD_STOPPED:
        break;

    default:
        break;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
INT32 sm_as_state_do_opening(
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    INT32                 i4_ret;
    PM_ES_REQUEST_T       t_es_comp;
#ifdef ENABLE_MULTIMEDIA
    PM_MM_REQUEST_T       t_mm_comp;
#endif
    SM_SESSION_T*         pt_sess_obj;
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;
    HANDLE_T              h_stream;
    SCDB_COMMON_T         t_scdb_cmmn;
    UINT32                ui4_version = 0;
    UINT16                ui2_num_recs = 0;

    SM_ASSERT( pv_tag1 );
    pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag1;
    h_stream = pt_as_obj->t_base.h_stream;
    
    _sm_as_state_record_latest_event( pt_as_obj, ui4_event );

    pt_sess_obj = (SM_SESSION_T*)sm_get_sess_obj( pt_as_obj->t_base.h_sess );
    if ( !pt_sess_obj )
    {
        SM_ABORT( SM_DBG_ABRT_INV_SESS_OBJ );
    }
        
    /* open SCDB */
    i4_ret = x_scdb_open( pt_sess_obj->h_connection,
                          &pt_as_obj->t_base.h_scdb,
                          (VOID*)h_stream,
                          &_sm_as_state_scdb_nfy_fct );
    if ( i4_ret != SCDBR_OK )
    {
        SM_AS_DBG_ERR(( "[SM] AS: x_scdb_open() error return %d\r\n", i4_ret ));
        sm_as_send_msg_do_event( 
            h_stream,
            SM_EVN_AS_INTERNAL_ERR,
            0 );
        return SMR_STRM_COMP_ID_NOT_FOUND;
    }
    
    i4_ret = x_scdb_get_common( pt_sess_obj->h_scdb, 
                                &t_scdb_cmmn );
    if ( i4_ret != SCDBR_OK )
    {
        SM_AS_DBG_ERR(( 
            "[SM] AS, x_scdb_get_common(0x%.8X) error, return %d!\r\n",
            (UINT32)pt_sess_obj->h_scdb, i4_ret ));

        sm_as_send_msg_do_event(
            h_stream,
            SM_EVN_AS_INTERNAL_ERR,
            0 );
        return SMR_STRM_COMP_ID_NOT_FOUND;
    }
    if ( t_scdb_cmmn.e_feed_mode == FEED_MODE_PULL )
    {
        pt_as_obj->e_feed_mod = t_scdb_cmmn.e_feed_mode;
    }
        
    if ( pt_as_obj->ui2_force_pid == MPEG_2_NULL_PID )
    {
        i4_ret = x_scdb_get_rec( pt_as_obj->t_base.h_scdb,
                                 &pt_as_obj->t_base.t_strm_comp_id,
                                 &pt_as_obj->t_base.t_scdb_rec );
        if ( i4_ret != SCDBR_OK ||
             ( !IS_ANALOG_AUD_STRM(pt_as_obj) && 
               !IS_MPEG_AUD_STRM(pt_as_obj)   &&
               !IS_DIGITAL_AUD_STRM(pt_as_obj)&& 
               !IS_MULTIMEDIA_AUD_STRM(pt_as_obj)) )
        {
            SM_AS_DBG_ERR((
                "[SM] AS: x_scdb_get_rec(0x%.8X) error return %d\r\n",
                (UINT32)pt_as_obj->t_base.h_scdb, i4_ret ));
            sm_as_send_msg_do_event( 
                h_stream,
                SM_EVN_AS_INTERNAL_ERR,
                0 );
            return SMR_STRM_COMP_ID_NOT_FOUND;
        }
    }
    else
    {
        /* fake a SCDB record */
        pt_as_obj->t_base.t_scdb_rec.e_rec_type = SCDB_REC_TYPE_AUDIO_MPEG;
        pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.ui2_pid = pt_as_obj->ui2_force_pid;
        pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.e_aud_fmt = AUD_FMT_UNKNOWN;
        pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.e_enc = (AUD_ENC_T)pt_as_obj->ui1_force_enc;
        pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.e_layer = AUD_LAYER_UNKNOWN;
        pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.s_lang[0] = '\0';
        pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.b_default = FALSE;
    }
    
    i4_ret = x_scdb_get_num_recs( pt_as_obj->t_base.h_scdb,
                                  ST_VIDEO,
                                  &ui2_num_recs,
                                  &ui4_version );
    if ( i4_ret == SCDBR_OK && ui2_num_recs == 0 )
    {
        pt_as_obj->e_av_sync = AUD_DEC_AV_SYNC_FREE_RUN;
    }
    
    if(pt_as_obj->e_force_av_sync != AUD_DEC_AV_SYNC_NULL)
    {
       pt_as_obj->e_av_sync = pt_as_obj->e_force_av_sync;
    }
    
    /* open rest components that are not opened by commands */
    i4_ret = sm_as_open_ad( h_stream, pt_as_obj );
    if ( i4_ret != SMR_OK )
    {
        SM_AS_DBG_ERR((
            "[SM] AS: sm_as_open_ad(0x%.8X) error return %d\r\n",
            (UINT32)h_stream, i4_ret ));
        sm_as_send_msg_do_event( 
            h_stream,
            SM_EVN_AS_DRV_ERR,
            0 );
        return i4_ret;    
    }

    /* open PM component */
    if ( !IS_ANALOG_AUD_STRM(pt_as_obj) && 
         !IS_DIGITAL_AUD_STRM(pt_as_obj)&&
         !IS_MULTIMEDIA_AUD_STRM(pt_as_obj))
    {
        x_memset( ((VOID*)(&t_es_comp)), 0, sizeof(PM_ES_REQUEST_T) );
        
        if ( pt_as_obj->ui2_force_pid == MPEG_2_NULL_PID )
        {
            t_es_comp.e_es_info_type = PM_PRESENT_ES_WO_SETTING;
            t_es_comp.u_es_info.t_stream_comp_id = 
                pt_as_obj->t_base.t_strm_comp_id;
        }
        else
        {
            t_es_comp.e_es_info_type = PM_PRESENT_ES_WI_SETTING;
            t_es_comp.u_es_info.t_stream_setting.e_type = ST_AUDIO;
            t_es_comp.u_es_info.t_stream_setting.t_pid = 
                pt_as_obj->ui2_force_pid;
        }
        t_es_comp.h_component = NULL_HANDLE;
        
        t_es_comp.t_filter_nfy_info.pf_nfy = _sm_as_pm_nfy;
        t_es_comp.t_filter_nfy_info.pv_tag = (VOID*)h_stream;
        t_es_comp.t_filter_nfy_info.e_cond = PM_COND_CLOSED;
        t_es_comp.t_filter_nfy_info.ui4_detail_code = 0;
 
#ifdef SYS_MHP_SUPPORT       
        t_es_comp.e_ca_require_lvl = pt_as_obj->e_ca_req_lvl;
#endif
       
        t_es_comp.t_ca_nfy_info.pf_nfy = _sm_as_pm_ca_nfy;
        t_es_comp.t_ca_nfy_info.pv_tag = (VOID*)h_stream;
        t_es_comp.t_ca_nfy_info.e_cond = PM_CA_COND_CLOSED;
        t_es_comp.t_ca_nfy_info.ui4_detail_code = 0;
        t_es_comp.t_sink_comp_info.e_type = pt_as_obj->t_ad_descr.t_info.e_type;
        t_es_comp.t_sink_comp_info.ui2_id = pt_as_obj->t_ad_descr.t_info.ui2_comp_id;
    
        t_es_comp.t_cp_nfy_info.pf_nfy = _sm_as_cp_nfy;
        t_es_comp.t_cp_nfy_info.pv_tag = (VOID*)h_stream;
        t_es_comp.t_cp_nfy_info.t_cp_info.ui1_ctrl_size = 0;
        
        i4_ret = pm_add_component( pt_sess_obj->h_pm_service,
                                   (VOID*)&t_es_comp ); 
        if ( i4_ret != SMR_OK || t_es_comp.h_component == NULL_HANDLE )
        {
            SM_AS_DBG_ERR((
                "[SM] AS: pm_add_component() error return %d\r\n",
                i4_ret ));
            sm_as_send_msg_do_event(
                h_stream,
                SM_EVN_AS_INTERNAL_ERR,
                0 );
            return SMR_INTERNAL_ERROR;
        }
        
        pt_as_obj->t_base.h_pm_comp = t_es_comp.h_component;
        pt_as_obj->ui4_lastest_ca_event = 
            _sm_as_pm_ca_cond_2_ash( t_es_comp.t_ca_nfy_info.e_cond );
        pt_as_obj->ui4_lastest_ca_detail_code = 
            t_es_comp.t_ca_nfy_info.ui4_detail_code;
            
        if ( t_es_comp.t_filter_nfy_info.e_cond == PM_COND_CLOSED ||
             t_es_comp.t_filter_nfy_info.e_cond == PM_COND_PLAYED ||
             t_es_comp.t_filter_nfy_info.e_cond == PM_COND_STOPPED )
        {
            _sm_as_pm_nfy( 
                pt_as_obj->t_base.h_pm_comp,
                t_es_comp.t_filter_nfy_info.e_cond,
                t_es_comp.t_filter_nfy_info.ui4_detail_code,
                t_es_comp.t_filter_nfy_info.pv_tag );
        }
        
        if ( t_es_comp.t_cp_nfy_info.t_cp_info.ui1_ctrl_size != 0 )
        {
            _sm_as_cp_nfy(
                pt_as_obj->t_base.h_pm_comp, 
                t_es_comp.t_cp_nfy_info.t_cp_info,
                (VOID*)h_stream );
        }
    }
#ifdef ENABLE_MULTIMEDIA        
    else if (IS_MULTIMEDIA_AUD_STRM(pt_as_obj))
    {
        x_memset( ((VOID*)(&t_mm_comp)), 0, sizeof(PM_MM_REQUEST_T) );
        
#ifdef TIME_SHIFT_SUPPORT
        if ( IS_TS_AUD_STRM(pt_as_obj) && 
             ( pt_as_obj->t_crypt_info.u.t_pvr_crypt.e_mode != MM_CRYPT_MODE_INVALID || 
               pt_as_obj->ui2_force_pid != MPEG_2_NULL_PID ))
        {
            t_mm_comp.e_es_info_type = PM_MM_ES_WI_SETTING;
            t_mm_comp.u_es_info.t_stream_setting.e_type = STREAM_ID_TYPE_TS;
            if (pt_as_obj->ui2_force_pid == MPEG_2_NULL_PID)
            {
                t_mm_comp.u_es_info.t_stream_setting.u.t_stm_ts_id = 
                    pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.ui2_pid;
            }
            else
            {
                t_mm_comp.u_es_info.t_stream_setting.u.t_stm_ts_id = 
                    pt_as_obj->ui2_force_pid;
            }
            t_mm_comp.t_crypt_info = pt_as_obj->t_crypt_info;
        }
        else
#endif /* TIME_SHIFT_SUPPORT */
        if ( (pt_as_obj->t_base.t_scdb_rec.e_rec_type == SCDB_REC_TYPE_AUDIO_MP4) && 
             (MM_CRYPT_TYPE_MLN == pt_as_obj->t_crypt_info.e_mm_crypt_type))
        {
            t_mm_comp.e_es_info_type = PM_MM_ES_WI_SETTING;
            t_mm_comp.u_es_info.t_stream_setting.e_type = STREAM_ID_TYPE_ES;
            t_mm_comp.u_es_info.t_stream_setting.u.t_stm_es_id =
                pt_as_obj->t_base.t_scdb_rec.u.t_audio_mp4.ui4_stream_id;

            t_mm_comp.t_crypt_info = pt_as_obj->t_crypt_info;
        }
        else
        {
            t_mm_comp.e_es_info_type = PM_MM_ES_WO_SETTING;
            t_mm_comp.u_es_info.t_stream_comp_id =
                pt_as_obj->t_base.t_strm_comp_id;
        }

        t_mm_comp.h_component = NULL_HANDLE;        
        t_mm_comp.t_filter_nfy_info.pf_nfy = _sm_as_pm_nfy;
        t_mm_comp.t_filter_nfy_info.pv_tag = (VOID*)h_stream;
        t_mm_comp.t_filter_nfy_info.e_cond = PM_COND_CLOSED;
        t_mm_comp.t_filter_nfy_info.ui4_detail_code = 0;
        t_mm_comp.t_sink_comp_info.e_type = pt_as_obj->t_ad_descr.t_info.e_type;
        t_mm_comp.t_sink_comp_info.ui2_id = pt_as_obj->t_ad_descr.t_info.ui2_comp_id;   
        
        t_mm_comp.t_ca_nfy_info.pf_nfy = _sm_as_pm_ca_nfy;
        t_mm_comp.t_ca_nfy_info.pv_tag = (VOID*)h_stream;
        t_mm_comp.t_ca_nfy_info.e_cond = PM_CA_COND_CLOSED;
        t_mm_comp.t_ca_nfy_info.ui4_detail_code = 0;
            
        i4_ret = pm_add_component( pt_sess_obj->h_pm_service,
                                  (VOID*)&t_mm_comp );  
        if ( i4_ret != SMR_OK || t_mm_comp.h_component == NULL_HANDLE )
        {
            SM_AS_DBG_ERR((
                "[SM] AS: pm_add_component() error return %d\r\n",
                i4_ret ));
            sm_as_send_msg_do_event(
                h_stream,
                SM_EVN_AS_INTERNAL_ERR,
                0 );
            return SMR_INTERNAL_ERROR;
        }

            pt_as_obj->t_base.h_pm_comp = t_mm_comp.h_component;
            if ( t_mm_comp.t_filter_nfy_info.e_cond == PM_COND_CLOSED ||
                 t_mm_comp.t_filter_nfy_info.e_cond == PM_COND_PLAYED ||
                 t_mm_comp.t_filter_nfy_info.e_cond == PM_COND_STOPPED )
            {
                _sm_as_pm_nfy( 
                    pt_as_obj->t_base.h_pm_comp,
                    t_mm_comp.t_filter_nfy_info.e_cond,
                    t_mm_comp.t_filter_nfy_info.ui4_detail_code,
                    t_mm_comp.t_filter_nfy_info.pv_tag );
            }           
        }
        
#endif
    return SMR_OK;   
}

INT32 sm_as_state_do_opened(
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    INT32                 i4_ret;
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;
    HANDLE_T              h_stream;

    SM_ASSERT( pv_tag1 );
    pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag1;
    h_stream = pt_as_obj->t_base.h_stream;
    
    _sm_as_state_record_latest_event( pt_as_obj, ui4_event );


    i4_ret = sm_as_ad_set_callback( h_stream, pt_as_obj );
    if ( i4_ret != SMR_OK )
    {
        SM_AS_DBG_ERR((
                    "[SM]  %s:%d,AS: sm_as_ad_set_callback(0x%.8x) error return %d\r\n",
                    __FILE__, __LINE__,(UINT32)h_stream, i4_ret ));
		sm_as_send_msg_do_event( 
            h_stream,
            SM_EVN_AS_DRV_ERR,
            0 );
        return SMR_DRV_COMP_ERROR;
    }
    pt_as_obj->e_aud_decode_status = AUD_DEC_DECODE_UNKNOWN;    
#ifdef MW_SUPPORT_2STC

    i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
                     FALSE,
                     ANY_PORT_NUM,
                     AUD_DEC_SET_TYPE_STC_ID,
                     (VOID*)((UINT32)pt_as_obj->t_base.ui2_stc_id),
                     sizeof(UINT16),
                     &pt_as_obj->t_ad_descr.e_comp_cond);
   if ( i4_ret != RMR_OK )
   {
       return SMR_DRV_COMP_ERROR;
   }
#endif
    return SMR_OK;   
}

INT32 sm_as_state_do_starting(
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    INT32                 i4_ret;
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;
    HANDLE_T              h_stream;
    AUD_DEC_FMT_INFO_T    t_aud_fmt = { AUD_DEC_FMT_AC3, NULL };

    SM_ASSERT( pv_tag1 );
    pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag1;
    h_stream = pt_as_obj->t_base.h_stream;
    
    _sm_as_state_record_latest_event( pt_as_obj, ui4_event );

    if( IS_ANALOG_AUD_STRM(pt_as_obj) )
    {
        if(pt_as_obj->t_base.t_scdb_rec.u.t_audio_analog.b_enable_aud_sys_auto_detect)
        {
            AUD_DEC_AUTO_DETECT_AUD_SYSTEM_INFO_T t_aud_system_info;
            /*dvb_get_country(t_aud_system_info.t_country_code);*/
            /*default set country to UK*/
            x_strcpy(t_aud_system_info.t_country_code, "GBR");
            t_aud_system_info.ui4_tv_sys = pt_as_obj->t_base.t_scdb_rec.u.t_audio_analog.ui4_tv_sys;
            t_aud_system_info.ui4_aud_sys= pt_as_obj->t_base.t_scdb_rec.u.t_audio_analog.ui4_audio_sys;
            i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
                                FALSE,
                                ANY_PORT_NUM,
                                AUD_DEC_SET_TYPE_AUTO_DETECT_AUD_SYS,
                                (VOID*)&t_aud_system_info,
                                sizeof(AUD_DEC_AUTO_DETECT_AUD_SYSTEM_INFO_T),
                                &pt_as_obj->t_ad_descr.e_comp_cond);
            if ( i4_ret != RMR_OK && i4_ret != RMR_ASYNC_NFY )
            {
                SM_AS_DBG_INFO(( "[SM] %s:%d, Set Audio Decoder AUD_DEC_SET_TYPE_AUTO_DETECT_AUD_SYS error %d\r\n", 
                        __FILE__, __LINE__, i4_ret  ));
            }

            /*Set analog tv format*/
            if ( pt_as_obj->t_base.t_scdb_rec.u.t_audio_analog.e_enc != AUD_ENC_PCM )
            {
                SM_AS_DBG_ERR(( "[SM] %s:%d, ERROR! Analog Audio stream encoding is %u not PCM\r\n", 
                                 __FILE__, __LINE__,(UINT32)pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.e_enc ));
            }
            
            if (pt_as_obj->t_base.t_scdb_rec.u.t_audio_analog.e_enc == AUD_ENC_TV_SYS)
            {
                t_aud_fmt.e_fmt = AUD_DEC_FMT_TV_SYS;
            }
            else if (pt_as_obj->t_base.t_scdb_rec.u.t_audio_analog.e_enc == AUD_ENC_EU_CANAL_PLUS)
            {
                t_aud_fmt.e_fmt = AUD_DEC_FMT_EU_CANAL_PLUS;
            }
            else if (pt_as_obj->t_base.t_scdb_rec.u.t_audio_analog.e_enc == AUD_ENC_FM_RADIO)
            {
                t_aud_fmt.e_fmt = AUD_DEC_FMT_FMRDO;
            }
            else
            {
                t_aud_fmt.e_fmt = AUD_DEC_FMT_PCM;
            }
            i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
                             FALSE,
                             ANY_PORT_NUM,
                             AUD_DEC_SET_TYPE_FMT,
                             (VOID*)&t_aud_fmt,
                             sizeof(AUD_DEC_FMT_INFO_T),
                             &pt_as_obj->t_ad_descr.e_comp_cond);
            if ( i4_ret != RMR_OK )
            {
                SM_AS_DBG_INFO(( "[SM] %s:%d, Set Audio Decoder AUD_DEC_SET_TYPE_FMT error %d\r\n", 
                        __FILE__, __LINE__, i4_ret  ));
                return SMR_DRV_COMP_ERROR;
            }

            pt_as_obj->e_ad_ctrl_type = AUD_DEC_CTRL_DETECT_TV_AUD_SYS_N_PLAY;
            i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
                             FALSE,
                             ANY_PORT_NUM,
                             AUD_DEC_SET_TYPE_CTRL,
                             (VOID*)pt_as_obj->e_ad_ctrl_type,
                             sizeof(AUD_DEC_CTRL_T),
                             &pt_as_obj->t_ad_descr.e_comp_cond);
            if ( i4_ret != RMR_OK && i4_ret != RMR_ASYNC_NFY )
            {
                SM_AS_DBG_INFO(( "[SM] %s:%d, Set Audio Decoder AUD_DEC_SET_TYPE_CTRL error %d\r\n", 
                        __FILE__, __LINE__, i4_ret  ));
                return SMR_DRV_COMP_ERROR;    
            }

        }
        else
        {
            i4_ret = _sm_as_state_do_play_drv(h_stream,ui4_event,pt_as_obj,pv_tag2);
            if ( i4_ret != SMR_OK )
            {
                SM_AS_DBG_ERR((
                    "[SM]  %s:%d,AS: sm_as_ad_play_analog_tv(0x%.8x) error return %d\r\n",
                    __FILE__, __LINE__,(UINT32)h_stream, i4_ret ));
                
                sm_as_send_msg_do_event( 
                    h_stream,
                    SM_EVN_AS_DRV_ERR,
                    0 );
                return SMR_DRV_COMP_ERROR;
            }
        }
    }
    else
    {
           i4_ret = _sm_as_state_do_play_drv(h_stream,ui4_event,pt_as_obj,pv_tag2);
           if ( i4_ret != SMR_OK )
           {
               SM_AS_DBG_ERR((
                   "[SM] %s:%d,AS: sm_as_ad_play_dtv_or_mm(0x%.8x) error return %d\r\n",
                   __FILE__, __LINE__,(UINT32)h_stream, i4_ret ));
               
               sm_as_send_msg_do_event( 
                   h_stream,
                   SM_EVN_AS_DRV_ERR,
                   0 );
               return SMR_DRV_COMP_ERROR;
           }
    }
#if 0
    i4_ret = sm_as_ad_play_drvs( h_stream, pt_as_obj, (INT32)pv_tag2 );
    if ( i4_ret != SMR_OK )
    {
        SM_AS_DBG_ERR((
            "[SM] AS: sm_as_ad_play_drvs(0x%.8x) error return %d\r\n",
            (UINT32)h_stream, i4_ret ));
        
        sm_as_send_msg_do_event( 
            h_stream,
            SM_EVN_AS_DRV_ERR,
            0 );
        return SMR_DRV_COMP_ERROR;
    }
    
    /* set Process Manager components */
    if ( ui4_event == SM_EVN_AS_APP_PLAY_REQ ||
         pt_as_obj->ui4_lastest_pm_event == SM_EVN_AS_PM_COND_STOPPED )
    {
        if ( !IS_ANALOG_AUD_STRM(pt_as_obj) && 
             !IS_DIGITAL_AUD_STRM(pt_as_obj) )
        {
            i4_ret = pm_play_component( pt_as_obj->t_base.h_pm_comp );
            if ( i4_ret != PMR_OK )
            {
                SM_AS_DBG_ERR((
                    "[SM] AS: pm_play_component(0x%.8x) error return %d\r\n",
                    (UINT32)pt_as_obj->t_base.h_pm_comp, i4_ret ));
                sm_as_send_msg_do_event( 
                    h_stream,
                    SM_EVN_AS_INTERNAL_ERR,
                    0 );
                return SMR_INTERNAL_ERROR;    
            }
            SM_AS_DBG_ERR(( "[SM] PM play audio component done\r\n" ));
        }
    }
#endif
    return SMR_OK;   
}

#ifdef SM_ASH_NO_SIG_AUTO_MUTE
VOID sm_as_state_signal_monitor_tm_cb(
    HANDLE_T  pt_tm_handle,
    VOID* pv_tag)
{
   
    SM_AUDIO_STREAM_T* pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag;
    if ( IS_MPEG_AUD_STRM(pt_as_obj) )
    {
        sm_as_send_msg_do_event( 
                pt_as_obj->t_base.h_stream,
                SM_EVN_AS_SIG_MONITOR_TM,
                0 );
    }
    return;
}
#endif
INT32 sm_as_state_do_started(
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;
#ifdef SM_ASH_NO_SIG_AUTO_MUTE    
    INT32 i4_ret;;
#endif
    SM_ASSERT( pv_tag1 );
    pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag1;
    
    /*if audio info not ready, not sent event to upper layer*/ 
    if ( pt_as_obj->t_aud_info.e_aud_fmt != ASH_AUDIO_FMT_UNKNOWN )
    {
        sm_as_state_do_aud_info_chg(
                    ui4_curr_state,
                    SM_EVN_AS_AUD_INFO_CHG,
                    ui4_curr_state,
                    pv_tag1,
                    NULL);       
    }
    
    _sm_as_state_record_latest_event( pt_as_obj, ui4_event );

    if ( pt_as_obj->ui4_lastest_ca_event != SM_EVN_IGNORE )
    {
        _sm_as_state_prc_ca_event( pt_as_obj );
    }

#ifdef SM_ASH_NO_SIG_AUTO_MUTE
    if ( IS_MPEG_AUD_STRM(pt_as_obj))
    { 
        SM_COMMAND_T    at_cmd[3];    
        /* command for opening an audio component */
       at_cmd[0].e_code     = SCC_CMD_CODE_GRP_TYPE;
       at_cmd[0].u.ui4_data = SCC_CMD_TYPE_AUDIO;
       at_cmd[1].e_code     = SCC_CMD_CODE_NAME;
       at_cmd[1].u.ps_text  = SN_PRES_MAIN_DISPLAY;
       at_cmd[2].e_code     = SM_CMD_CODE_END;

       /* open the audio component */    
       i4_ret = x_scc_comp_open(at_cmd,&pt_as_obj->h_scc_grp_aud_comp);
       if(i4_ret != SMR_OK)
       {
           SM_AS_DBG_ERR(( "[SM] AS : auto mute intial scc component failed %d\r\n",
                                i4_ret ));
        }

        /*created signal level detecting timer*/
        i4_ret = x_timer_create(&(pt_as_obj->h_tm_sig_detect));
        if(i4_ret != OSR_OK)
        {
            SM_AS_DBG_ERR(( "[SM] AS : auto mute create timer failed %d\r\n",
                                 i4_ret ));
        }
        x_timer_start(pt_as_obj->h_tm_sig_detect, 800,X_TIMER_FLAG_REPEAT,sm_as_state_signal_monitor_tm_cb,pt_as_obj);
        if(i4_ret != OSR_OK)
        {
            SM_AS_DBG_ERR(( "[SM] AS :  auto mute start timer failed %d\r\n",
                                 i4_ret ));
        }    
    }
#endif
    return SMR_OK;
}

INT32 sm_as_state_do_stopping(
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    INT32                 i4_ret;
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;
    HANDLE_T              h_stream;
#ifdef CI_PLUS_SUPPORT
    AUD_DEC_COPY_PROTECT_INFO_T t_ad_cp_info ;
    RM_COND_T                e_drv_cond;
#endif    
    SM_ASSERT( pv_tag1 );
    pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag1;
    h_stream = pt_as_obj->t_base.h_stream;

    _sm_as_state_record_latest_event( pt_as_obj, ui4_event );

    /* stop PM component */
    if ( pt_as_obj->ui4_lastest_pm_event != SM_EVN_AS_PM_COND_CLOSED &&
         pt_as_obj->ui4_lastest_pm_event != SM_EVN_AS_PM_COND_STOPPED )
    {
        if ( !IS_ANALOG_AUD_STRM(pt_as_obj) && 
             !IS_DIGITAL_AUD_STRM(pt_as_obj) )
        {   
            SM_AS_DBG_INFO((
                        "[SM] AS: pm_stop_component(0x%.8x) start\r\n",
                        (UINT32)pt_as_obj->t_base.h_pm_comp));
            i4_ret = pm_stop_component( pt_as_obj->t_base.h_pm_comp );
            SM_AS_DBG_INFO((
                        "[SM] AS: pm_stop_component(0x%.8x) end\r\n",
                        (UINT32)pt_as_obj->t_base.h_pm_comp));
            if ( i4_ret != PMR_OK )
            {
                if ( i4_ret == PMR_INV_HANDLE &&
                     ( ui4_event == SM_EVN_AS_APP_CLOSE_REQ ||
                       ui4_event == SM_EVN_SESS_APP_STOP_REQ ) )
                {
                    /* PM component may already closed by session layer,
                       Treat as OK */
                }
                else
                {
                    SM_AS_DBG_ERR((
                        "[SM] AS: pm_stop_component(0x%.8x) error return %d\r\n",
                        (UINT32)pt_as_obj->t_base.h_pm_comp, i4_ret ));
        
                    sm_as_send_msg_do_event( 
                        h_stream,
                        SM_EVN_AS_INTERNAL_ERR,
                        0 );
                    return SMR_INTERNAL_ERROR;
                }
            }
        }
    }
#ifdef CI_PLUS_SUPPORT    
    t_ad_cp_info.t_mode = AUD_DEC_CP_MODE_CI;
    t_ad_cp_info.t_protect_info = AUD_DEC_COPY_PROTECT_OFF;
    
    i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
                     FALSE,
                     ANY_PORT_NUM,
                     AUD_DEC_SET_TYPE_COPY_PROTECT,
                     (VOID*)&t_ad_cp_info,
                     sizeof( AUD_DEC_COPY_PROTECT_INFO_T ),
                     &e_drv_cond );
    if ( i4_ret != RMR_OK )
    {
        SM_AS_DBG_ERR(( "[SM] Set Audio Decoder in stopping AUD_DEC_SET_TYPE_COPY_PROTECT error %d\r\n",
            i4_ret ));
    }
#endif    
    /* stop Audio Decoder */
    i4_ret = sm_as_ad_stop_drvs( h_stream, pt_as_obj );
    if ( i4_ret != SMR_OK )
    {
        SM_AS_DBG_ERR((
            "[SM] AS: sm_as_ad_stop_drvs(0x%.8x) error return %d\r\n",
            (UINT32)h_stream, i4_ret ));

        sm_as_send_msg_do_event( 
            h_stream,
            SM_EVN_AS_DRV_ERR,
            0 );
        return SMR_DRV_COMP_ERROR;
    }
    return SMR_OK;   
}

INT32 sm_as_state_do_closing(
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    INT32                 i4_ret;
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;
    HANDLE_T              h_stream;

    SM_ASSERT( pv_tag1 );
    pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag1;
    h_stream = pt_as_obj->t_base.h_stream;

    _sm_as_state_record_latest_event( pt_as_obj, ui4_event );
   
#ifdef SM_ASH_NO_SIG_AUTO_MUTE
   if ( IS_MPEG_AUD_STRM(pt_as_obj) )
   {
       i4_ret =  x_scc_aud_switch(pt_as_obj->h_scc_grp_aud_comp,TRUE);
       if ( i4_ret != SMR_OK && i4_ret != SMR_ASYNC_NFY )
       {
           SM_AS_DBG_ERR(( "[SM] AS : Auto mute , Mute on/off  Failed %d\r\n",
                                          i4_ret ));
       }
       pt_as_obj->b_had_auto_mute = TRUE;
       /*close signal detect timer*/
       i4_ret = x_timer_delete(pt_as_obj->h_tm_sig_detect);
       if(i4_ret != OSR_OK)
       {
           SM_AS_DBG_ERR(( "[SM] AS : delete signal detect timer failed %d\r\n",
                                i4_ret ));
       }
       i4_ret = x_scc_comp_close(pt_as_obj->h_scc_grp_aud_comp);
       if(i4_ret != SMR_OK)
       {
           SM_AS_DBG_ERR(( "[SM] AS : auto mute close scc component failed %d\r\n",
                                i4_ret ));
       }
   }
#endif

    if ( pt_as_obj->t_base.h_scdb != NULL_HANDLE )
    {
        i4_ret = x_scdb_close( pt_as_obj->t_base.h_scdb );
        if ( i4_ret != SCDBR_OK )
        {
            SM_AS_DBG_INFO((
                "[SM] AS: x_scdb_close(0x%.8x) error return %d\r\n",
                (UINT32)pt_as_obj->t_base.h_scdb, i4_ret ));
        }
        pt_as_obj->t_base.h_scdb = NULL_HANDLE;
    }

    if ( pt_as_obj->t_ad_descr.h_comp != NULL_HANDLE )
    {
        RM_COND_T  e_conn_cond;

        if ( ui4_previous_state == SM_COND_ERROR &&
             pt_as_obj->ui4_lastest_ad_event != SM_EVN_AS_AD_STOPPED &&
             pt_as_obj->ui4_lastest_ad_event != SM_EVN_AS_AD_CLOSED )
        {
            i4_ret = sm_as_ad_stop_drvs( h_stream, pt_as_obj );
            if ( i4_ret != SMR_OK )
            {
                SM_AS_DBG_INFO(( "[SM] AS: sm_as_ad_stop_drvs(0x%.8x) error return %d\r\n",                 
                                              (UINT32)pt_as_obj->t_ad_descr.h_comp, i4_ret ));
            }
        }
        
        e_conn_cond = rm_cond_chg( pt_as_obj->t_ad_descr.e_comp_cond,
                                   0,
                                   COND_GRP_CONN_STATE );

        if ( e_conn_cond == RM_COND_CONNECTING || 
             e_conn_cond == RM_COND_CONNECTED )
        {
            i4_ret = sm_as_ad_disconnect( h_stream, pt_as_obj );
            if ( i4_ret != SMR_OK )
            {
                SM_AS_DBG_INFO((
                    "[SM] AS: sm_as_ad_disconnect(0x%.8x) error return %d\r\n",
                    (UINT32)h_stream, i4_ret ));
            }
        }
        else
        {
            i4_ret = sm_as_close_ad( h_stream, pt_as_obj );
            if ( i4_ret != SMR_OK )
            {
                SM_AS_DBG_INFO((
                    "[SM] AS: sm_as_close_ad(0x%.8x) error return %d\r\n",
                    (UINT32)h_stream, i4_ret ));
            }
        }
    }
    else
    {
        sm_as_send_msg_do_event( 
            h_stream,
            SM_EVN_AS_AD_CLOSED,
            0 );
    }
    
    if ( !IS_ANALOG_AUD_STRM(pt_as_obj) && 
         !IS_DIGITAL_AUD_STRM(pt_as_obj) )
    {
        if ( pt_as_obj->t_base.h_pm_comp != NULL_HANDLE )
        {
            /* Stop PM component if previous state is ERROR, and PM condition
               is not in STOPPED/CLOSED state */
            if ( ui4_previous_state == SM_COND_ERROR &&
                 pt_as_obj->ui4_lastest_pm_event != SM_EVN_AS_PM_COND_STOPPED &&
                 pt_as_obj->ui4_lastest_pm_event != SM_EVN_AS_PM_COND_CLOSED )
            {   
                SM_AS_DBG_INFO((
                        "[SM] AS: pm_stop_component(0x%.8x) start\r\n",
                        (UINT32)pt_as_obj->t_base.h_pm_comp));
                i4_ret = pm_stop_component( pt_as_obj->t_base.h_pm_comp );
				SM_AS_DBG_INFO((
                        "[SM] AS: pm_stop_component(0x%.8x) end\r\n",
                        (UINT32)pt_as_obj->t_base.h_pm_comp));
                if ( i4_ret != PMR_OK )
                {
                    SM_AS_DBG_INFO((
                        "[SM] AS: pm_stop_component(0x%.8x) error return %d\r\n",
                        (UINT32)pt_as_obj->t_base.h_pm_comp, i4_ret ));
                }
            }

            i4_ret = pm_del_component( pt_as_obj->t_base.h_pm_comp ); 
            if ( i4_ret != SMR_OK )
            {
                SM_AS_DBG_INFO((
                    "[SM] AS: pm_del_component(0x%.8x) error return %d\r\n",
                    (UINT32)pt_as_obj->t_base.h_pm_comp, i4_ret ));
            }
            pt_as_obj->t_base.h_pm_comp = NULL_HANDLE;
        }
        else
        {
            sm_as_send_msg_do_event( 
                h_stream,
                SM_EVN_AS_PM_COND_CLOSED,
                0 );
        }
    }
    return SMR_OK;   
}

INT32 sm_as_state_do_closed(
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;

    SM_ASSERT( pv_tag1 );
    pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag1;

    _sm_as_state_record_latest_event( pt_as_obj, ui4_event );

    return SMR_OK;   
}

INT32 sm_as_state_do_error(
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;

    SM_ASSERT( pv_tag1 );
    pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag1;

    _sm_as_state_record_latest_event( pt_as_obj, ui4_event );

    return SMR_OK;   
}

INT32 sm_as_state_do_ignore_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;

    SM_ASSERT( pv_tag1 );
    pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag1;

    _sm_as_state_record_latest_event( pt_as_obj, ui4_event );
    
    return SMR_CAN_NOT_OP_IN_THIS_STATE;   
}

INT32 sm_as_state_do_opening_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;

    SM_ASSERT( pv_tag1 );
    pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag1;

    _sm_as_state_record_latest_event( pt_as_obj, ui4_event );

    return SMR_OK;
}

INT32 sm_as_state_do_opened_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;
    HANDLE_T              h_stream;

    SM_ASSERT( pv_tag1 );
    pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag1;
    h_stream = pt_as_obj->t_base.h_stream;

    _sm_as_state_record_latest_event( pt_as_obj, ui4_event );

    switch ( ui4_event )
    {
    case SM_EVN_AS_APP_STOP_REQ:
    case SM_EVN_SESS_APP_STOP_REQ:
        return SMR_ALREADY_STOPPED;
    
    case SM_EVN_AS_PM_COND_PLAYED:
        /* ignore the message */
        return SMR_OK;
    
    default:
        /* Ignore un-handled events */
        SM_AS_DBG_ERR((
            "[SM] sm_as_state_do_opened_excpt() un-handled event = %.8X\r\n",
            ui4_event ));
        sm_as_send_msg_do_event( 
            h_stream,
            SM_EVN_AS_INTERNAL_ERR,
            0 );
        return SMR_CAN_NOT_OP_IN_THIS_STATE;
    }
}

INT32 sm_as_state_do_starting_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;

    SM_ASSERT( pv_tag1 );
    pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag1;

    _sm_as_state_record_latest_event( pt_as_obj, ui4_event );

    return SMR_OK;
}

INT32 sm_as_state_do_started_excpt(
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    INT32                 i4_ret = SMR_OK;
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;
    HANDLE_T              h_stream;

    SM_ASSERT( pv_tag1 );
    pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag1;
    h_stream = pt_as_obj->t_base.h_stream;

    _sm_as_state_record_latest_event( pt_as_obj, ui4_event );

    switch ( ui4_event )
    {
    case SM_EVN_AS_APP_PLAY_REQ:
        return SMR_ALREADY_PLAYED;
    case SM_EVN_SESS_APP_PLAY_REQ:
        {
            INT32 i4_speed = (INT32) pv_tag2;
            
            if ( i4_speed != pt_as_obj->t_base.i4_speed )
            {
                i4_ret = sm_as_ad_chg_speed( h_stream, pt_as_obj, i4_speed );
                if ( i4_ret != SMR_OK )
                {
                    SM_AS_DBG_ERR((
                        "[SM] AS: sm_as_ad_chg_speed(0x%.8x) error return %d\r\n",
                        (UINT32)h_stream, i4_ret ));
                    
                    sm_as_send_msg_do_event( 
                        h_stream,
                        SM_EVN_AS_DRV_ERR,
                        0 );
                    return SMR_DRV_COMP_ERROR;
                }
            }
        }
        return SMR_ALREADY_PLAYED;
        
    case SM_EVN_AS_PM_COND_STOPPED:
    case SM_EVN_AS_PM_COND_CLOSED:
        /* ignore the message */
        return SMR_OK;

    default:
        /* Ignore un-handled events */
        SM_AS_DBG_ERR((
            "[SM] sm_as_state_do_started_excpt() un-handled event = %.8X\r\n",
            ui4_event ));
        sm_as_send_msg_do_event( 
            h_stream,
            SM_EVN_AS_INTERNAL_ERR,
            0 );
        return SMR_CAN_NOT_OP_IN_THIS_STATE;
    }
}

INT32 sm_as_state_do_stopping_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;

    SM_ASSERT( pv_tag1 );
    pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag1;

    _sm_as_state_record_latest_event( pt_as_obj, ui4_event );

    return SMR_OK;
}

INT32 sm_as_state_do_closing_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;

    SM_ASSERT( pv_tag1 );
    pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag1;

    _sm_as_state_record_latest_event( pt_as_obj, ui4_event );

    SM_AS_DBG_INFO((
        "[SM] AS(0x%.8X), SM_COND_CLOSING state receive exception event 0x%.8X\r\n",
        pt_as_obj->t_base.h_stream, ui4_event ));
        
    return SMR_OK;
}

INT32 sm_as_state_do_closed_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;

    SM_ASSERT( pv_tag1 );
    pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag1;

    _sm_as_state_record_latest_event( pt_as_obj, ui4_event );

    return SMR_CAN_NOT_OP_IN_THIS_STATE;   
}

INT32 sm_as_state_do_error_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;

    SM_ASSERT( pv_tag1 );
    pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag1;

    _sm_as_state_record_latest_event( pt_as_obj, ui4_event );

    return SMR_CAN_NOT_OP_IN_THIS_STATE;   
}

VOID sm_as_state_notify_app( 
    VOID*                 pv_tag,
    SM_COND_T             e_cond,
    UINT32                ui4_event,
    UINT32                ui4_data )
{
    INT32                 i4_ret;
    SM_AS_STATE_TAG_T*    pt_state_tag = (SM_AS_STATE_TAG_T*)pv_tag;
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;
    HANDLE_T              h_stream;
    
    SM_ASSERT( pt_state_tag );
    SM_ASSERT( pt_state_tag->pt_as_obj );
    SM_ASSERT( pt_state_tag->h_stream != NULL_HANDLE );
    
    pt_as_obj = pt_state_tag->pt_as_obj;
    h_stream = pt_state_tag->h_stream;
    
#ifdef TIME_MEASUREMENT
    _sm_as_state_time_msrt( 
        (UINT32)pt_as_obj->t_base.e_condition,
        (UINT32)e_cond,
        ui4_event );
#endif /* TIME_MEASUREMENT */
        
    pt_as_obj->t_base.e_condition = e_cond;

    i4_ret = pt_as_obj->pf_sess_hdlr_sm_nfy(
        h_stream,
        pt_as_obj->t_base.e_condition,
        pt_as_obj->pv_sess_hdlr_sm_nfy_tag,
        ui4_event,
        ui4_data );
    if ( i4_ret != SMR_OK )
    {
        /* do nothing */
    }


    {
        const CHAR* ps_event = sm_state_event_to_str(ui4_event);
        
        if ( ps_event )
        {
            x_dbg_stmt(
                "[SM] AS(0x%.8X), %s, %s, DATA(0x%.8X)\r\n",
                (UINT32)h_stream,
                sm_strm_cond_2_str(e_cond),
                ps_event,
                (UINT32)ui4_data );
        }
        else
        {
            x_dbg_stmt(
                "[SM] AS(0x%.8X), %s, EVN(0x%.8X), DATA(0x%.8X)\r\n",
                (UINT32)h_stream,
                sm_strm_cond_2_str(e_cond),
                (UINT32)ui4_event,
                (UINT32)ui4_data );
        }
    }


}

/* Event handle functions ****************************************************/
/*-----------------------------------------------------------------------------
 * Name: sm_as_state_do_cci_events
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
INT32 sm_as_state_do_cci_events(
    UINT32     ui4_curr_state,
    UINT32     ui4_event,
    UINT32     ui4_next_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
 #ifndef CI_PLUS_SUPPORT
    INT32                         i4_i;
 #endif
    INT32                         i4_ret = SMR_OK;
    SM_AS_STATE_TAG_T             t_state_tag;
    CP_INFO_LIST_T* pt_cci_info_lst = NULL;
    SM_AUDIO_STREAM_T*            pt_as_obj = NULL;
    HANDLE_T                      h_stream;
    SM_HANDLER_T*                 pt_hdlr_obj = NULL;    
    
    SM_ASSERT( pv_tag1 );
    SM_ASSERT( pv_tag2 );
    t_state_tag.pt_as_obj = pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag1;
    t_state_tag.h_stream = h_stream = pt_as_obj->t_base.h_stream;

    pt_cci_info_lst = (CP_INFO_LIST_T*)pv_tag2;

    pt_as_obj->t_cp_info_lst = *pt_cci_info_lst;

#ifndef CI_PLUS_SUPPORT
    for( i4_i = 0; i4_i < (INT32)pt_cci_info_lst->ui1_ctrl_size; i4_i++ )
    {
        if(i4_i <CP_TYPE_MAX)
        {
    #if 0
            i4_ret = x_sm_set( h_stream, 
                               SM_STRM_SET_TYPE_CCI_INFO, 
                               (VOID*)&pt_cci_info_lst->at_cp_ctrl_list[i4_i], 
                               sizeof(CP_INFO_T) );
    #else
            i4_ret = handle_get_obj( pt_as_obj->t_base.h_handler,
                                     (VOID**)&pt_hdlr_obj );
                    
            i4_ret = pt_hdlr_obj->t_fct_tbl.pf_set( h_stream,
                                                    SM_STRM_SET_TYPE_CCI_INFO,
                                                    (VOID*)&pt_cci_info_lst->at_cp_ctrl_list[i4_i],
                                                    sizeof(CP_INFO_T) );
    #endif
            if ( i4_ret != SMR_OK )
            {
                SM_AS_DBG_INFO(( 
                    "[SM] AS, ERROR! x_sm_set(0x%.8x, SM_STRM_SET_TYPE_CCI_INFO) "
                    "return %d\r\n",
                    h_stream, i4_ret ));
            }
        }
        else
        {
            SM_AS_DBG_INFO(( 
                "[SM] AS, ERROR! stream (0x%.8x), ui1_ctrl_size outside the bounds) \r\n",
                h_stream ));
            return SMR_INV_ARG;
        }

    }
#else
    #if 0
    i4_ret = x_sm_set( h_stream, 
                       SM_STRM_SET_TYPE_CCI_INFO, 
                       (VOID*)pt_cci_info_lst, 
                       sizeof(CP_INFO_LIST_T) );

    #else
    i4_ret = handle_get_obj( pt_as_obj->t_base.h_handler,
                             (VOID**)&pt_hdlr_obj );
            
    i4_ret = pt_hdlr_obj->t_fct_tbl.pf_set( h_stream,
                                            SM_STRM_SET_TYPE_CCI_INFO,
                                            (VOID*)pt_cci_info_lst,
                                            sizeof(CP_INFO_LIST_T) );
    #endif
    if ( i4_ret != SMR_OK )
    {
        SM_AS_DBG_INFO(( 
            "[SM] AS, ERROR! x_sm_set(0x%.8x, SM_STRM_SET_TYPE_CCI_INFO) "
            "return %d\r\n",
            h_stream, i4_ret ));
    }
#endif
    sm_as_state_notify_app(
        &t_state_tag,
        (SM_COND_T)pt_as_obj->t_state.ui4_curr_state,
        ui4_event,
        (UINT32)&pt_as_obj->t_cp_info_lst );
    
    return SMR_OK;
}

INT32 sm_as_state_do_ca_events(
    UINT32     ui4_curr_state,
    UINT32     ui4_event,
    UINT32     ui4_next_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;

    SM_ASSERT( pv_tag1 );
    pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag1;
    
    _sm_as_state_record_latest_event( pt_as_obj, ui4_event );

    pt_as_obj->ui4_lastest_ca_detail_code = (UINT32)pv_tag2;
    if ( ui4_curr_state == SM_COND_STARTED )
    {
        _sm_as_state_prc_ca_event( pt_as_obj );
    }

    return SMR_OK;
}

INT32 sm_as_state_do_drv_events(
    UINT32     ui4_curr_state,
    UINT32     ui4_event,
    UINT32     ui4_next_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    INT32                 i4_ret = SMR_OK;
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;
    HANDLE_T              h_stream;

    SM_ASSERT( pv_tag1 );
    pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag1;
    h_stream = pt_as_obj->t_base.h_stream;

    if ( !_sm_as_state_record_latest_event( pt_as_obj, ui4_event ) )
    {
        /* Skip the event if there is no changes */
        return SMR_OK;
    }

    switch ( SM_EVN_GET_GRP(ui4_event) )
    {
    case SM_EVN_AS_GRP_AD:
        i4_ret = _sm_as_state_do_ad_event( h_stream, pt_as_obj, ui4_event );
        break;
        
    case SM_EVN_AS_GRP_PM:
        i4_ret = _sm_as_state_do_pm_event( h_stream, pt_as_obj, ui4_event );
        break;
        
    default:
        SM_AS_DBG_ERR((
            "[SM] sm_as_state_do_drv_events() receives invalid event 0x%.8X\r\n",
            ui4_event ));
        SM_ABORT( SM_DBG_ABRT_INV_EVENT );
    }
    
    if ( i4_ret != SMR_OK )
    {
        sm_as_send_msg_do_event(
            h_stream,
            SM_EVN_AS_DRV_ERR,
            0 );
        return SMR_DRV_COMP_ERROR;
    }
   
    if ( !IS_ANALOG_AUD_STRM(pt_as_obj) && 
         !IS_DIGITAL_AUD_STRM(pt_as_obj) )
    {
        if ( ui4_event == SM_EVN_AS_AD_CONNECTED &&
             ( pt_as_obj->ui4_lastest_pm_event == SM_EVN_AS_PM_COND_STOPPED ||
               pt_as_obj->ui4_lastest_pm_event == SM_EVN_AS_PM_COND_PLAYED ) )
        {
            sm_as_send_msg_do_event( 
                h_stream,
                SM_EVN_AS_DRV_OPENED,
                0 );
        }
        else if ( ui4_event == SM_EVN_AS_PM_COND_STOPPED &&
                  pt_as_obj->ui4_lastest_ad_event == SM_EVN_AS_AD_CONNECTED )
        {
            sm_as_send_msg_do_event( 
                h_stream,
                SM_EVN_AS_DRV_OPENED,
                0 );
        }
        else if ( ( pt_as_obj->ui4_lastest_ad_event == SM_EVN_AS_AD_PLAYED
                    /*for CR326264 */
                    #if 0
                    ||pt_as_obj->ui4_lastest_ad_event == SM_EVN_AS_AD_PAUSED
                    #endif
                    )&&
                  pt_as_obj->ui4_lastest_pm_event == SM_EVN_AS_PM_COND_PLAYED )
        {
            sm_as_send_msg_do_event( 
                h_stream,
                SM_EVN_AS_DRV_PLAYED,
                0 );
        }
        else if ( ui4_event == SM_EVN_AS_AD_STOPPED &&
                  pt_as_obj->ui4_lastest_pm_event == SM_EVN_AS_PM_COND_CLOSED )
        {
            /* If current event is AD stopped, and PM is already closed by session
               layer, simulate a driver stopped message. */
            sm_as_send_msg_do_event( 
                h_stream,
                SM_EVN_AS_DRV_STOPPED,
                0 );
        }
        else if ( pt_as_obj->ui4_lastest_ad_event == SM_EVN_AS_AD_STOPPED &&
                  pt_as_obj->ui4_lastest_pm_event == SM_EVN_AS_PM_COND_STOPPED )
        {
            sm_as_send_msg_do_event( 
                h_stream,
                SM_EVN_AS_DRV_STOPPED,
                0 );
        }
        else if ( pt_as_obj->ui4_lastest_ad_event == SM_EVN_AS_AD_CLOSED &&
                  ( pt_as_obj->ui4_lastest_pm_event == SM_EVN_AS_PM_COND_CLOSED ||
                    pt_as_obj->ui4_lastest_pm_event == SM_EVN_IGNORE ) )
        {
            sm_as_send_msg_do_event( 
                h_stream,
                SM_EVN_AS_DRV_CLOSED,
                0 );
        }
    }
    else
    {
        if ( ui4_event == SM_EVN_AS_AD_CONNECTED )
        {
            sm_as_send_msg_do_event( 
                h_stream,
                SM_EVN_AS_DRV_OPENED,
                0 );
        }
        else if ( ui4_event == SM_EVN_AS_AD_PLAYED
                /*for CR326264 */
                #if 0
                ||ui4_event == SM_EVN_AS_AD_PAUSED
                #endif
                )
        {
            sm_as_send_msg_do_event( 
                h_stream,
                SM_EVN_AS_DRV_PLAYED,
                0 );
        }
        else if ( ui4_event == SM_EVN_AS_AD_STOPPED )
        {
            sm_as_send_msg_do_event( 
                h_stream,
                SM_EVN_AS_DRV_STOPPED,
                0 );
        }
        else if ( ui4_event == SM_EVN_AS_AD_CLOSED )
        {
            sm_as_send_msg_do_event( 
                h_stream,
                SM_EVN_AS_DRV_CLOSED,
                0 );
        }
    }
    
    return SMR_OK;
}

static AUD_ENC_T ash_aud_decoder_type_2_scdb_aud_type(
    ASH_DECODE_TYPE_T e_decoder_type )
{
  switch( e_decoder_type )
  {
    case ASH_DECODE_TYPE_UNKNOWN:
        return AUD_ENC_UNKNOWN;

    case ASH_DECODE_TYPE_AC3:
        return AUD_ENC_AC3;

    case ASH_DECODE_TYPE_EAC3:
        return AUD_ENC_E_AC3;
    
    default:
        return AUD_ENC_UNKNOWN;
  }
}

static AUD_FMT_T ash_aud_type_2_scdb_aud_type(
    ASH_CHANNELS_T e_ash_aud_channel )
{
  switch( e_ash_aud_channel )
  {
    case ASH_CHANNELS_MONO:
        return AUD_FMT_MONO;

    case ASH_CHANNELS_STEREO:
        return AUD_FMT_STEREO;

    case ASH_CHANNELS_5_1:
        return AUD_FMT_TYPE_5_1;
    
    case ASH_CHANNELS_DUAL_MONO:
        return AUD_FMT_DUAL_MONO;
    
    default:
        return ASH_CHANNELS_UNKNOWN;
  }
}


INT32 sm_as_state_do_aud_info_chg
(
    UINT32     ui4_curr_state,
    UINT32     ui4_event,
    UINT32     ui4_next_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_AS_STATE_TAG_T     t_state_tag;
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;
    INT32 i4_ret;    
    BOOL  fg_is_scdb_update = FALSE;    
    BRDCST_CTRL_AUDIO_STREAM_FMT_T t_brdst_aud_format;
	BRDCST_CTRL_AUDIO_ENC_T        t_enc;

    SM_ASSERT( pv_tag1 );
    t_state_tag.pt_as_obj = pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag1;
    t_state_tag.h_stream = (HANDLE_T)pt_as_obj->t_base.h_stream;
    x_dbg_stmt(
           "[SM] Audio information change:stream handler(0x%.8X),SCDB aud fmt= [%d],driver aud e_channels= [%d],driver aud fmt= [%d],\r\n \tdriver sample rate = [0x%.8X],driver data rate = [0x%.8X],driver bit depth = [0x%.2X],driver aud_decoder= [%d]\r\n",
                                       (UINT32)t_state_tag.h_stream,
                                       pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.e_aud_fmt,
                                       pt_as_obj->t_aud_info.e_channels,
                                       pt_as_obj->t_aud_info.e_aud_fmt,
                                       (UINT32)pt_as_obj->t_aud_info.ui4_sample_rate,
                                       (UINT32)pt_as_obj->t_aud_info.ui4_data_rate,
                                       (UINT8)pt_as_obj->t_aud_info.ui1_bit_depth,
                                       pt_as_obj->t_aud_info.e_dec_type);
    /*Dual-Mono double confirm:*/ 
    /*If audio format reported by driver isn't Dual-Mono one , correct scdb records*/
    if(IS_MPEG_AUD_STRM(pt_as_obj)
	/*add for CR DTV00368137*/
	||(IS_TS_AUD_STRM( pt_as_obj )&&IS_MULTIMEDIA_AUD_STRM( pt_as_obj )))
    { 
      if(pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.e_aud_fmt == AUD_FMT_DUAL_MONO)
      {
          if(pt_as_obj->t_aud_info.e_channels != ASH_CHANNELS_DUAL_MONO)
          {
              /*   When get format form scdb is dual mono and the format from drive is non-dual mono*/
              /*   not to infor CM to update SCDB.                 CR[DTV00134316]                                                      */
              
      	       /*fg_is_scdb_update = TRUE;*/ 
      	       
               /*Not Stereo Bitstream ,resest stero mode by default*/
               i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
  			       FALSE,
  			       ANY_PORT_NUM,
  			       AUD_DEC_SET_TYPE_DOWNMIX,
  			       (VOID*)(UINT32)AUD_DEC_DM_STEREO,
  			       sizeof(AUD_DEC_DOWNMIX_MODE_T),
  			       &pt_as_obj->t_ad_descr.e_comp_cond);
  	           if ( i4_ret != RMR_OK && i4_ret != RMR_ASYNC_NFY )
               {
                  SM_AS_DBG_ERR(( "[SM] Set Audio Decoder AUD_DEC_SET_TYPE_DOWNMIX error %d\r\n",
                  i4_ret ));
                   /* Ignore this error
  	               return SMR_DRV_COMP_ERROR;
   	            */
               } 
      	  }
      }
      else
      {
      	  if(pt_as_obj->t_aud_info.e_channels == ASH_CHANNELS_DUAL_MONO)
      	  {
              /*   When get format form scdb is non-dual mono and the format from drive is dual mono*/
              /*   not to infor CM to update SCDB.                 CR[DTV00141103]                             */
#ifdef   MW_ASH_DUALMONO_RECOVER
              fg_is_scdb_update = TRUE;
#endif
           {
	       pt_as_obj->b_downmix_recover = TRUE;
	       /*dual1 will change in some case*/
               i4_ret = rm_set( pt_as_obj->t_ad_descr.h_comp,
                    FALSE,
                    ANY_PORT_NUM,
                    AUD_DEC_SET_TYPE_DOWNMIX,
                    (VOID*)(UINT32)AUD_DEC_DM_DUAL1,
                    sizeof(AUD_DEC_DOWNMIX_MODE_T),
                    &pt_as_obj->t_ad_descr.e_comp_cond);
                if ( i4_ret != RMR_OK && i4_ret != RMR_ASYNC_NFY )
                {
                    SM_AS_DBG_ERR(( "[SM] Set Audio Decoder AUD_DEC_SET_TYPE_DOWNMIX error %d\r\n",
                    i4_ret ));
                }
           }
      	  }
      }        
      if(fg_is_scdb_update)
      {
         t_brdst_aud_format.ui1_index = pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.ui1_index;
         t_brdst_aud_format.e_audio_format = ash_aud_type_2_scdb_aud_type(pt_as_obj->t_aud_info.e_channels);     
         t_brdst_aud_format.ui2_pid = pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.ui2_pid;
         i4_ret = x_cm_set(pt_as_obj->t_base.h_connection, BRDCST_CTRL_SET_AUDIO_STREAM_FMT, &t_brdst_aud_format);
         if((i4_ret!=CMR_OK ) && (i4_ret!= CMR_ASYNC_NFY))
         {
            SM_AS_DBG_ERR((
            "[SM] CM update audio format type failed! 0x%.8X\r\n",
            i4_ret )); 
         }
      }
	  if(((pt_as_obj->t_aud_info.e_aud_fmt == ASH_AUDIO_FMT_AC3) || (pt_as_obj->t_aud_info.e_aud_fmt == ASH_AUDIO_FMT_E_AC3)) && (pt_as_obj->t_aud_info.e_dec_type != ASH_DECODE_TYPE_UNKNOWN))
	  {
		  t_enc.e_audio_enc = ash_aud_decoder_type_2_scdb_aud_type(pt_as_obj->t_aud_info.e_dec_type);
		  t_enc.ui1_index = pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.ui1_index;
		  t_enc.ui2_pid  = pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.ui2_pid;
		  i4_ret = x_cm_set(pt_as_obj->t_base.h_connection, BRDCST_CTRL_SET_AUDIO_ENC, &t_enc);
		  if((i4_ret!=CMR_OK ) && (i4_ret!= CMR_ASYNC_NFY))
		  {
			 SM_AS_DBG_ERR((
			 "[SM] CM update audio decoder type failed! 0x%.8X\r\n",
			 i4_ret )); 
		  }
	  }
    }
    
    if ( pt_as_obj->t_base.e_condition == SM_COND_STARTED ) 
    {     
        sm_as_state_notify_app(
            &t_state_tag,
            (SM_COND_T)ui4_curr_state,
            ui4_event,
            (UINT32)&pt_as_obj->t_aud_info );
    }
    return SMR_OK;
}

INT32 sm_as_state_do_aud_decode_status_chg
(
    UINT32     ui4_curr_state,
    UINT32     ui4_event,
    UINT32     ui4_next_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_AS_STATE_TAG_T          t_state_tag;
    SM_AUDIO_STREAM_T*         pt_as_obj = NULL;
    AUD_DEC_DECODE_STATUS_T    e_aud_decode_status; 

    SM_ASSERT( pv_tag1 );
    t_state_tag.pt_as_obj = pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag1;
    t_state_tag.h_stream = (HANDLE_T)pt_as_obj->t_base.h_stream;
    
    e_aud_decode_status = (AUD_DEC_DECODE_STATUS_T)((INT32)pv_tag2); 

    if((IS_MULTIMEDIA_AUD_STRM(pt_as_obj)) 
        && (e_aud_decode_status == AUD_DEC_DECODE_NOT_SUPPORT ||
            e_aud_decode_status == AUD_DEC_DECODE_FMT_CONFLICT))
    {
         /* for play mpeg file contain that mp3 audio
          *  When first play the file ,audio driver will notify format conflict, AS will set e_layer to layer 3,
          *  then send restart event to restart the audio stream .
          */
        if( ((pt_as_obj->t_enc == AUD_ENC_MPEG_1) || (pt_as_obj->t_enc == AUD_ENC_MPEG_2))&&
             (pt_as_obj->t_layer != AUD_LAYER_3))

            {
            pt_as_obj->t_base.t_scdb_rec.e_rec_type             = SCDB_REC_TYPE_AUDIO_MPEG;
            pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.e_enc = pt_as_obj->t_enc;
                pt_as_obj->t_base.t_scdb_rec.u.t_audio_mpeg.e_layer = AUD_LAYER_3;
            
            sm_as_send_msg_do_event(
                pt_as_obj->t_base.h_stream,
                SM_EVN_AS_RESTART,
                (UINT32)pt_as_obj->t_base.i4_speed );
        }
        else
        {
            sm_as_send_msg_do_event(
            t_state_tag.h_stream,
            SM_EVN_AS_CODEC_NOT_SUPPORT,
            (UINT32)0 );
            
            sm_as_send_msg_do_event( 
                t_state_tag.h_stream,
                SM_EVN_AS_APP_CLOSE_REQ,
                0 );
        }
    }

    sm_as_state_notify_app(
        &t_state_tag,
        (SM_COND_T)ui4_curr_state,
        ui4_event,
        (UINT32)pt_as_obj->e_aud_decode_status);
    
    return SMR_OK;
}
INT32 sm_as_state_do_tv_aud_sys
(
    UINT32     ui4_curr_state,
    UINT32     ui4_event,
    UINT32     ui4_next_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_AS_STATE_TAG_T          t_state_tag;
    SM_AUDIO_STREAM_T*         pt_as_obj = NULL;

    SM_ASSERT( pv_tag1 );
    t_state_tag.pt_as_obj = pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag1;
    t_state_tag.h_stream = (HANDLE_T)pt_as_obj->t_base.h_stream;

    sm_as_state_notify_app(
        &t_state_tag,
        (SM_COND_T)ui4_curr_state,
        ui4_event,
        (UINT32)&pt_as_obj->t_aud_sys_info);
    
    return SMR_OK;
}

#ifdef ENABLE_MULTIMEDIA
INT32 sm_as_state_do_event_nfy
(
    UINT32     ui4_curr_state,
    UINT32     ui4_event,
    UINT32     ui4_next_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_AS_STATE_TAG_T     t_state_tag;
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;

    SM_ASSERT( pv_tag1 );
    t_state_tag.pt_as_obj = pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag1;
    t_state_tag.h_stream = (HANDLE_T)pt_as_obj->t_base.h_stream;

    sm_as_state_notify_app(
        &t_state_tag,
        (SM_COND_T)ui4_curr_state,
        ui4_event,
        (UINT32)pt_as_obj->e_aud_decode_status);
    
    return SMR_OK;
}
#endif

#ifdef SM_ASH_NO_SIG_AUTO_MUTE
static ASH_NO_SIG_AUTO_MUTE_T audio_hdlr_mute_state_to_mute_mode( BOOL b_is_mute ) 
{
    ASH_NO_SIG_AUTO_MUTE_T  e_auto_mute = ASH_NO_SIG_AUTO_MUTE_UNKNOWN;
    if( b_is_mute == TRUE)        
    {
        e_auto_mute = ASH_NO_SIG_AUTO_MUTE_MUTE;
    }    
    else 
    {
        e_auto_mute = ASH_NO_SIG_AUTO_MUTE_UNMUTE;
    }
    return e_auto_mute;
}

INT32 sm_as_state_do_signal_check
(
    UINT32     ui4_curr_state,
    UINT32     ui4_event,
    UINT32     ui4_next_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_AUDIO_STREAM_T*    pt_as_obj = NULL;
    TUNER_PACKET_STATUS_T t_tuner_status; 
    INT32 i4_ret;
    BOOL b_state_change = FALSE;
    BOOL b_is_mute;
    BOOL b_send_auto_mute_info = FALSE;

    INT8 i1_lost_rate_up_bound;
    INT8 i1_lost_rate_low_bound;
    INT8 i1_error_rate_bound;
    INT8 i1_react_freq;

    SM_AS_STATE_TAG_T             t_state_tag;
    ASH_NO_SIG_AUTO_MUTE_T     e_auto_mute_mode;
    
     SM_ASSERT( pv_tag1 );
     pt_as_obj = (SM_AUDIO_STREAM_T*)pv_tag1;

     t_state_tag.pt_as_obj = pt_as_obj ;
     t_state_tag.h_stream = (HANDLE_T)pt_as_obj->t_base.h_stream;
     
     sm_get_as_force_mute_parm (&i1_lost_rate_up_bound,&i1_lost_rate_low_bound,&i1_error_rate_bound,&i1_react_freq);
     i4_ret = x_cm_get(pt_as_obj->t_base.h_connection,(CM_CTRL_TYPE_T)BRDCST_CTRL_GET_PACKET_STATUS, &t_tuner_status);
     if(i4_ret != CMR_OK)
     {
        SM_AS_DBG_ERR(( "[SM] AS : Packet status information invalid %d\r\n",
                                           i4_ret ));
     }
     SM_AS_DBG_INFO(("[SM] AS: lost rate %d\r\n", t_tuner_status.ui1_lost_rate));    
     SM_AS_DBG_INFO(("[SM] AS: error rate %d\r\n", t_tuner_status.ui1_error_rate));
     SM_AS_DBG_INFO(("[SM] AS: counts %d\r\n",  pt_as_obj->i1_sig_err_count));                               
    if((t_tuner_status.ui1_lost_rate>=i1_lost_rate_up_bound)/*&&(t_tuner_status.ui1_lost_rate!=100)*/ ||
        (t_tuner_status.ui1_error_rate)>= i1_error_rate_bound)
    {
       if(pt_as_obj->i1_sig_err_count < i1_react_freq)
       {
           if(pt_as_obj->i1_sig_err_count ==  -i1_react_freq)
           {
                pt_as_obj->i1_sig_err_count = 0;    
           }
           pt_as_obj->i1_sig_err_count++;
           if(pt_as_obj->i1_sig_err_count == i1_react_freq)
           { 
               b_is_mute = TRUE;
               b_state_change = TRUE; 
           }
       }
    }
    else if(t_tuner_status.ui1_lost_rate < i1_lost_rate_low_bound/*|| t_tuner_status.ui1_error_rate==0&&(t_tuner_status.ui1_lost_rate!=100)*/)
    {
       if(pt_as_obj->i1_sig_err_count >-i1_react_freq)
       {
           if(pt_as_obj->i1_sig_err_count ==  i1_react_freq)
            {
                pt_as_obj->i1_sig_err_count = 0;    
            }
           pt_as_obj->i1_sig_err_count--;
           if(pt_as_obj->i1_sig_err_count == -i1_react_freq)
           {  
               b_is_mute = FALSE; 
               b_state_change = TRUE;
           }
       }
    }
    else
    {
        pt_as_obj->i1_sig_err_count = 0;
    }
    if( b_state_change)
    {
        i4_ret =  x_scc_aud_switch(pt_as_obj->h_scc_grp_aud_comp,!b_is_mute);
        if ( i4_ret != SMR_OK && i4_ret != SMR_ASYNC_NFY )
        {
            SM_AS_DBG_ERR(( "[SM] AS : Auto mute , Mute on/off  Failed %d\r\n",
                                           i4_ret ));
            return SMR_DRV_COMP_ERROR;    
        }
        
        e_auto_mute_mode = audio_hdlr_mute_state_to_mute_mode(b_is_mute);

        if(e_auto_mute_mode == ASH_NO_SIG_AUTO_MUTE_UNMUTE)
        { 
             if( pt_as_obj->b_had_auto_mute)
             {
                 b_send_auto_mute_info = TRUE;
                 pt_as_obj->b_had_auto_mute = FALSE; 
             }
             else
             {
                 b_send_auto_mute_info = FALSE;
             }
        }
        else if(e_auto_mute_mode == ASH_NO_SIG_AUTO_MUTE_MUTE)
        {
            if( pt_as_obj->b_had_auto_mute == FALSE)
            {
                b_send_auto_mute_info = TRUE;
                pt_as_obj->b_had_auto_mute = TRUE; 
            }
            else
            {
                b_send_auto_mute_info = FALSE;
            }
        }
        else
        {
            b_send_auto_mute_info = FALSE;
        }
        
        if(b_send_auto_mute_info)
        {
        sm_as_state_notify_app(
        &t_state_tag,
        (SM_COND_T)pt_as_obj->t_state.ui4_curr_state,
        SM_EVN_AS_SIG_MONITOR_AUTO_MUTE,
        (UINT32) e_auto_mute_mode);
        }
    }
    return SMR_OK;
}
#endif

