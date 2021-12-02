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
 * $RCSfile: _sm_gs_state.c,v $
 * $Revision: #2 $
 * $Date: 2012/07/26 $
 * $Author: jiangbo.wang $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: c8cea9c91f05a976402a89aef6552b73 $
 *
 * Description:  
 *         This header file contains Stream Manager Generic Sessionn State
 *         Machine specific implementations.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "handle/x_handle.h"
#include "hash/hash_api.h"

#include "prc_mngr/prc_mngr.h"

#include "strm_mngr/_sm_utils.h"
#include "strm_mngr/sm_state.h"
#include "strm_mngr/sess_gnrc_hdlr/sm_sess_gnrc.h"
#include "strm_mngr/sess_gnrc_hdlr/_sm_gs_state.h"
#include "strm_mngr/sess_gnrc_hdlr/_sm_sess_gnrc_pcr.h"
#include "strm_mngr/sess_gnrc_hdlr/_sm_sess_gnrc_stc.h"
#include "strm_mngr/sess_gnrc_hdlr/_sm_sess_gnrc_dmx.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#ifdef TIME_MEASUREMENT
#define GSS_HANDLE_IGNORE_2_OPEN_STR            "GSS:IGNORE-OPENED"
#define GSS_HANDLE_IGNORE_2_STARTED_STR         "GSS:IGNORE-STARTED"
#define GSS_HANDLE_STARTED_2_STOPPED_STR        "GSS:STARTED-STOPPED"
#define GSS_HANDLE_STARTED_2_CLOSED_STR         "GSS:STARTED-CLOSED"

#define GSS_HANDLE_PM_OPENED                    "GSS:PM-OPENED"
#define GSS_HANDLE_PM_STARTED                   "GSS:PM-STARTED"
#define GSS_HANDLE_PM_STOPPED                   "GSS:PM-STOPPED"
#define GSS_HANDLE_PM_CLOSED                    "GSS:PM-CLOSED"

#define GSS_HANDLE_PCR_OPENED                   "GSS:PCR-OPEN"
#define GSS_HANDLE_PCR_CONNECTED                "GSS:PCR-CONN"
#define GSS_HANDLE_PCR_DISCONNECTED             "GSS:PCR-DISCONN"
#define GSS_HANDLE_PCR_CLOSED                   "GSS:PCR-CLOSE"

#endif /* TIME_MEASUREMENT */

#define DMX_NOT_EXIST_OR_LAST_EVENT_IS( pt_sess_obj, event )                 \
    ( pt_sess_obj->t_dmx_descr.h_comp == NULL_HANDLE ||                      \
      pt_sess_obj->ui4_lastest_dmx_event == event )

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
static VOID _sm_gs_scdb_nfy_fct( 
    HANDLE_T        h_scdb,
    SCDB_COND_T     e_cond,
    UINT32          ui4_reason,
    VOID*           pv_tag,
    UINT32          ui4_data );

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

#ifdef TIME_MEASUREMENT
static VOID _sm_gss_state_time_msrt( 
    UINT32      ui4_curr_state,
    UINT32      ui4_next_state,
    UINT32      ui4_event )
{
    if ( ui4_curr_state == SM_SESS_COND_IGNORE && 
         ui4_next_state == SM_SESS_COND_OPENING )
    {
        TMS_BEGIN( GSS_HANDLE_IGNORE_2_OPEN_STR );
        TMS_BEGIN_EX(TMS_FLAG_CHG_CHL, GSS_HANDLE_IGNORE_2_OPEN_STR, GSS_HANDLE_IGNORE_2_OPEN_STR); 
        TMS_BEGIN( GSS_HANDLE_IGNORE_2_STARTED_STR );
        TMS_BEGIN_EX(TMS_FLAG_CHG_CHL, GSS_HANDLE_IGNORE_2_STARTED_STR, GSS_HANDLE_IGNORE_2_STARTED_STR);
        
    }
    else if ( ui4_curr_state == SM_SESS_COND_OPENING && 
              ui4_next_state == SM_SESS_COND_OPENED )
    {
        TMS_END( GSS_HANDLE_IGNORE_2_OPEN_STR );
        TMS_END_EX(TMS_FLAG_CHG_CHL, GSS_HANDLE_IGNORE_2_OPEN_STR, GSS_HANDLE_IGNORE_2_OPEN_STR);
    }
    else if ( ui4_curr_state == SM_SESS_COND_STARTING && 
              ui4_next_state == SM_SESS_COND_STARTED )
    {
        TMS_END( GSS_HANDLE_IGNORE_2_STARTED_STR );
        TMS_END_EX(TMS_FLAG_CHG_CHL, GSS_HANDLE_IGNORE_2_STARTED_STR, GSS_HANDLE_IGNORE_2_STARTED_STR);
    }
    else if ( ui4_curr_state == SM_SESS_COND_STARTED && 
              ui4_next_state == SM_SESS_COND_STOPPING )
    {
        TMS_BEGIN( GSS_HANDLE_STARTED_2_STOPPED_STR );
        TMS_BEGIN_EX(TMS_FLAG_CHG_CHL,GSS_HANDLE_STARTED_2_STOPPED_STR,GSS_HANDLE_STARTED_2_STOPPED_STR);
        TMS_BEGIN( GSS_HANDLE_STARTED_2_CLOSED_STR );
        TMS_BEGIN_EX(TMS_FLAG_CHG_CHL,GSS_HANDLE_STARTED_2_CLOSED_STR,GSS_HANDLE_STARTED_2_CLOSED_STR);
    }
    else if ( ui4_curr_state == SM_SESS_COND_STOPPING && 
              ui4_next_state == SM_SESS_COND_OPENED )
    {
        TMS_END( GSS_HANDLE_STARTED_2_STOPPED_STR );
        TMS_END_EX(TMS_FLAG_CHG_CHL, GSS_HANDLE_STARTED_2_STOPPED_STR, GSS_HANDLE_STARTED_2_STOPPED_STR);
    }
    else if ( ui4_curr_state == SM_SESS_COND_CLOSING && 
              ui4_next_state == SM_SESS_COND_CLOSED )
    {
        TMS_END( GSS_HANDLE_STARTED_2_CLOSED_STR );
        TMS_END_EX(TMS_FLAG_CHG_CHL, GSS_HANDLE_STARTED_2_CLOSED_STR, GSS_HANDLE_STARTED_2_CLOSED_STR);
    }
}

static VOID _sm_gss_drv_time_msrt( UINT32 ui4_event )
{

    switch( ui4_event )
    {
    case SM_EVN_SESS_PCR_OPENED:
        TMS_END( GSS_HANDLE_PCR_OPENED );
        break;
    
    case SM_EVN_SESS_PCR_CLOSED:
        TMS_END( GSS_HANDLE_PCR_CLOSED );
        break;
    
    case SM_EVN_SESS_PCR_CONNECTED:
        TMS_END( GSS_HANDLE_PCR_CONNECTED );
        break;
    
    case SM_EVN_SESS_PCR_DISCONNECTED:
        TMS_END( GSS_HANDLE_PCR_DISCONNECTED );
        break;
    
    default:
        break;    
    }
}
#endif /* TIME_MEASUREMENT */

/*-----------------------------------------------------------------------------
 * Name: _sm_gs_scdb_nfy_fct
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
static VOID _sm_gs_scdb_nfy_fct( 
    HANDLE_T        h_scdb,
    SCDB_COND_T     e_cond,
    UINT32          ui4_reason,
    VOID*           pv_tag,
    UINT32          ui4_data )
{
    SM_SESS_GNRC_MSG_T     t_msg;
    HANDLE_T               h_sess = (HANDLE_T)pv_tag;
    if ( !x_handle_valid( h_scdb ) )
    {
        return;
    }

    if( (e_cond != SCDB_UPDATED) &&
        (e_cond != SCDB_CLOSED) )
    {
        return;
    }

    if( ui4_reason == SCDB_REASON_UNK )
    {
        return;
    }
    
    t_msg.e_msg_type = SM_SESS_GNRC_MSG_TYPE_SCDB;
    t_msg.h_sess = h_sess;
    t_msg.t_scdb_msg.e_scdb_cond = e_cond;
    t_msg.t_scdb_msg.ui4_reason = ui4_reason;
    t_msg.t_scdb_msg.ui4_data = ui4_data;
        
    sm_gs_send_msg( &t_msg );
}

/*-----------------------------------------------------------------------------
 * Name: _sm_gs_strm_close_hash_cb
 *
 * Description: <function description>
 *
 * Inputs:  h_hash       References.
 *          ui4_counter  References.
 *          pv_obj       References.
 *          pv_parse_tag References.
 *
 * Outputs: - 
 *
 * Returns: HASH_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _sm_gs_strm_close_hash_cb( 
    HANDLE_T          h_hash,
    UINT32            ui4_counter,
    VOID*             pv_obj,
    VOID*             pv_parse_tag )
{
    HANDLE_T        h_stream = (HANDLE_T)pv_obj;
    SM_STREAM_T*    pt_strm_obj = NULL;
    SM_HANDLER_T*   pt_hdlr_obj = NULL;
    INT32           i4_ret;
    
    /* get stream object */
    pt_hdlr_obj = (SM_HANDLER_T*)sm_get_strm_hdlr_obj( h_stream, 
                                                       (VOID**)&pt_strm_obj );
    if ( !pt_hdlr_obj || !pt_strm_obj )
    {
        /* error, but try to process other streams */
        return HASH_OK;
    }
    
    /* assume stream already closed, if lock is fail */
    SM_LOCK_RETURN_ON_FAIL( pt_strm_obj->h_lock, HASH_OK );
    
    i4_ret = pt_hdlr_obj->t_fct_tbl.pf_close( h_stream );

    SM_UNLOCK( pt_strm_obj->h_lock );

    if ( i4_ret != SMR_OK &&
         i4_ret != SMR_PENDING )
    {
        /* error, but try to process other streams */
        SM_GS_DBG_ERR(( 
            "[SM] $s:%d, stream(0x%.8X) close fail!\r\n", 
            __FILE__, __LINE__, (UINT32)h_stream ));
        return HASH_OK;
    }
    
    return HASH_OK;
}


static INT32 _sm_gs_stream_play( 
    HANDLE_T           h_stream,
    SM_SESS_GNRC_T*    pt_sess_obj )
{
    SM_STREAM_T*    pt_strm_obj = NULL;
    SM_HANDLER_T*   pt_hdlr_obj = NULL;
    INT32           i4_ret;
    
    /* get stream object */
    pt_hdlr_obj = (SM_HANDLER_T*)sm_get_strm_hdlr_obj( h_stream, 
                                                       (VOID**)&pt_strm_obj );
    if ( !pt_hdlr_obj || !pt_strm_obj )
    {
        return SMR_INTERNAL_ERROR;
    }
    
    /* assume stream already closed, if lock is fail */
    SM_LOCK_RETURN_ON_FAIL( pt_strm_obj->h_lock, HASH_OK );
    
    i4_ret = pt_hdlr_obj->t_fct_tbl.pf_set( h_stream,
                                            SM_SESS_SET_TYPE_PLAY,
                                            (VOID*)pt_sess_obj->t_base.i4_speed,
                                            sizeof(pt_sess_obj->t_base.i4_speed) );

    SM_UNLOCK( pt_strm_obj->h_lock );

    if ( i4_ret != SMR_ASYNC_NFY ) /* play should always be async */
    {
        return i4_ret;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _sm_gs_start_hash_cb
 *
 * Description: <function description>
 *
 * Inputs:  h_hash       References.
 *          ui4_counter  References.
 *          pv_obj       References.
 *          pv_parse_tag References.
 *
 * Outputs: - 
 *
 * Returns: HASH_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _sm_gs_start_hash_cb( 
    HANDLE_T          h_hash,
    UINT32            ui4_counter,
    VOID*             pv_obj,
    VOID*             pv_parse_tag )
{
    HANDLE_T        h_stream = (HANDLE_T)pv_obj;
    SM_SESS_GNRC_T* pt_sess_obj = (SM_SESS_GNRC_T*)pv_parse_tag;
    SM_STREAM_T*    pt_strm_obj = NULL;
    SM_HANDLER_T*   pt_hdlr_obj = NULL;
    INT32           i4_ret;
    
    /* get stream object */
    pt_hdlr_obj = (SM_HANDLER_T*)sm_get_strm_hdlr_obj( h_stream, 
                                                       (VOID**)&pt_strm_obj );
    if ( !pt_hdlr_obj || !pt_strm_obj )
    {
        /* error, but try to process other streams */
        return HASH_OK;
    }
    
    i4_ret = _sm_gs_stream_play( h_stream, pt_sess_obj );
    if ( i4_ret != SMR_OK )
    {
        /* error, but try to process other streams */
        return HASH_OK;
    }
    
    return HASH_OK;
}

static INT32 _sm_gs_stream_stop( 
    HANDLE_T           h_stream )
{
    SM_STREAM_T*    pt_strm_obj = NULL;
    SM_HANDLER_T*   pt_hdlr_obj = NULL;
    INT32           i4_ret;
    
    /* get stream object */
    pt_hdlr_obj = (SM_HANDLER_T*)sm_get_strm_hdlr_obj( h_stream, 
                                                       (VOID**)&pt_strm_obj );
    if ( !pt_hdlr_obj || !pt_strm_obj )
    {
        return SMR_INTERNAL_ERROR;
    }
    
    /* assume stream already closed, if lock is fail */
    SM_LOCK_RETURN_ON_FAIL( pt_strm_obj->h_lock, HASH_OK );

    i4_ret = pt_hdlr_obj->t_fct_tbl.pf_set( h_stream,
                                            SM_SESS_SET_TYPE_STOP,
                                            (VOID*)NULL,
                                            0 );

    SM_UNLOCK( pt_strm_obj->h_lock );

    if ( i4_ret != SMR_ASYNC_NFY &&
         i4_ret != SMR_PENDING ) /* stop should always be async */
    {
        return i4_ret;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _sm_gs_stop_hash_cb
 *
 * Description: <function description>
 *
 * Inputs:  h_hash       References.
 *          ui4_counter  References.
 *          pv_obj       References.
 *          pv_parse_tag References.
 *
 * Outputs: - 
 *
 * Returns: HASH_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _sm_gs_stop_hash_cb( 
    HANDLE_T          h_hash,
    UINT32            ui4_counter,
    VOID*             pv_obj,
    VOID*             pv_parse_tag )
{
    HANDLE_T        h_stream = (HANDLE_T)pv_obj;
    SM_STREAM_T*    pt_strm_obj = NULL;
    SM_HANDLER_T*   pt_hdlr_obj = NULL;
    INT32           i4_ret;
    
    /* get stream object */
    pt_hdlr_obj = (SM_HANDLER_T*)sm_get_strm_hdlr_obj( h_stream, 
                                                       (VOID**)&pt_strm_obj );
    if ( !pt_hdlr_obj || !pt_strm_obj )
    {
        /* error, but try to process other streams */
        return HASH_OK;
    }
    
    /* assume stream already closed, if lock is fail */
    SM_LOCK_RETURN_ON_FAIL( pt_strm_obj->h_lock, HASH_OK );

    i4_ret = _sm_gs_stream_stop( h_stream );

    SM_UNLOCK( pt_strm_obj->h_lock );

    if ( i4_ret != SMR_ASYNC_NFY &&
         i4_ret != SMR_PENDING ) /* stop should always be async */
    {
        /* error, but try to process other streams */
        return HASH_OK;
    }
    
    return HASH_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _sm_gs_state_record_latest_event
 *
 * Description: <function description>
 *
 * Inputs:  pt_sess_obj References.
 *          ui4_event   References.
 *
 * Outputs: - 
 *
 * Returns: TRUE    Event is updated.
 *          FALSE   No changes.
 *
 ----------------------------------------------------------------------------*/
static BOOL _sm_gs_state_record_latest_event(
    SM_SESS_GNRC_T*       pt_sess_obj,
    UINT32                ui4_event )
{
    switch ( SM_EVN_GET_GRP(ui4_event) )
    {
    case SM_EVN_SESS_GRP_PCR:
        if ( pt_sess_obj->ui4_lastest_pcr_event == ui4_event )
        {
            return FALSE;
        }
        pt_sess_obj->ui4_lastest_pcr_event = ui4_event;
        break;

    case SM_EVN_SESS_GRP_PM:
#ifdef TIME_MEASUREMENT
        if ( pt_sess_obj->ui4_lastest_pm_event == SM_EVN_IGNORE &&
             ui4_event == SM_EVN_SESS_PM_COND_STOPPED )
        {
            TMS_END( GSS_HANDLE_PM_OPENED );
        }
        else if ( pt_sess_obj->ui4_lastest_pm_event == SM_EVN_SESS_PM_COND_PLAYED &&
                  ui4_event == SM_EVN_SESS_PM_COND_STOPPED )
        {
            TMS_END( GSS_HANDLE_PM_STOPPED );
        }
        else if ( ui4_event == SM_EVN_SESS_PM_COND_PLAYED )
        {
            TMS_END( GSS_HANDLE_PM_STARTED );
        }
#endif /* TIME_MEASUREMENT */

        if ( pt_sess_obj->ui4_lastest_pm_event == ui4_event )
        {
            return FALSE;
        }
        pt_sess_obj->ui4_lastest_pm_event = ui4_event;
        break;
        
    case SM_EVN_SESS_GRP_STRM:
        if ( pt_sess_obj->ui4_lastest_strm_event == ui4_event )
        {
            return FALSE;
        }
        pt_sess_obj->ui4_lastest_strm_event = ui4_event;
        break;

    case SM_EVN_SESS_GRP_STC:
        if ( pt_sess_obj->ui4_lastest_stc_event == ui4_event )
        {
            return FALSE;
        }
        pt_sess_obj->ui4_lastest_stc_event = ui4_event;
        break;

    case SM_EVN_SESS_GRP_DMX:
        if ( pt_sess_obj->ui4_lastest_dmx_event == ui4_event )
        {
            return FALSE;
        }
        pt_sess_obj->ui4_lastest_dmx_event = ui4_event;
        break;

    case SM_EVN_SESS_GRP_APP:
    default:
        /* Ignore un-handled events */
        return FALSE;
    }

#ifdef TIME_MEASUREMENT
    _sm_gss_drv_time_msrt( ui4_event );
#endif /* TIME_MEASUREMENT */
    
    return TRUE;
}

static INT32 _sm_gs_state_do_pcr_event(
    HANDLE_T              h_sess,
    SM_SESS_GNRC_T*       pt_sess_obj,
    UINT32                ui4_event )
{
    INT32    i4_ret = SMR_OK;
    
    switch ( ui4_event )
    {
    case SM_EVN_SESS_PCR_OPENED:
        TMS_BEGIN( GSS_HANDLE_PCR_CONNECTED );
        i4_ret = sm_sess_gnrc_pcr_connect( h_sess, pt_sess_obj );
        if ( i4_ret != SMR_OK )
        {
            sm_gs_send_msg_do_event(
                SM_EVN_SESS_DRV_ERR,
                h_sess,
                0 );
            return SMR_DRV_COMP_ERROR;
        }
        break;

    case SM_EVN_SESS_PCR_CONNECTED:
        sm_gs_send_msg_do_event(
            SM_EVN_SESS_PCR_PID_UPDATE,
            h_sess,
            0 );
        break;
        
    case SM_EVN_SESS_PCR_PID_UPDATE:
        x_dbg_stmt("[SM]SM_EVN_SESS_PCR_PID_UPDATE:%d,%d.\r\n",pt_sess_obj->b_disable_av_sync,
                                                              pt_sess_obj->b_pcr_owner);
#ifdef MW_TV_AV_BYPASS_SUPPORT
        if( ( !pt_sess_obj->b_disable_av_sync ) && ( pt_sess_obj->b_pcr_owner )) 
#else
        if ( !pt_sess_obj->b_disable_av_sync )
#endif            
        {
            x_dbg_stmt("[SM]sm_sess_gnrc_pcr_set_pid.\r\n");
            i4_ret = sm_sess_gnrc_pcr_set_pid( h_sess, pt_sess_obj );
            if ( i4_ret != SMR_OK )
            {
                sm_gs_send_msg_do_event(
                    SM_EVN_SESS_DRV_ERR,
                    h_sess,
                    0 );
                return SMR_DRV_COMP_ERROR;
            }
        }
        break;
    
    case SM_EVN_SESS_PCR_DISCONNECTED:
        TMS_BEGIN( GSS_HANDLE_PCR_CLOSED );
        i4_ret = sm_sess_gnrc_pcr_close( h_sess, pt_sess_obj );
        if ( i4_ret != SMR_OK )
        {
            /* Pretend that PCR is closed */
            sm_gs_send_msg_do_event(
                SM_EVN_SESS_PCR_CLOSED,
                h_sess,
                0 );
            return SMR_OK;
        }
        break;

    case SM_EVN_SESS_PCR_CLOSED:
    default:
        /* ignore */
        break;
    }
    
    return SMR_OK;
}

static INT32 _sm_gs_state_do_pm_event(
    HANDLE_T              h_sess,
    SM_SESS_GNRC_T*       pt_sess_obj,
    UINT32                ui4_event )
{
    return SMR_OK;
}

static INT32 _sm_gs_state_do_strm_event(
    HANDLE_T              h_sess,
    SM_SESS_GNRC_T*       pt_sess_obj,
    UINT32                ui4_event,
    UINT32                ui4_evn_data )
{
    INT32        i4_ret;
    HANDLE_T     h_stream = (HANDLE_T)ui4_evn_data;
    
    if ( ui4_event == SM_EVN_SESS_STRM_ALL_CLOSED &&
         pt_sess_obj->t_state.ui4_curr_state == SM_SESS_COND_CLOSING )
    {
        if ( pt_sess_obj->t_base.h_pm_service != NULL_HANDLE )
        {
            i4_ret = pm_close_service( pt_sess_obj->t_base.h_pm_service );
            if ( i4_ret != PMR_OK && i4_ret != PMR_INV_HANDLE )
            {
                SM_GS_DBG_INFO(( 
                    "[SM] pm_close_service() fail, return %d!\r\n",
                    i4_ret ));
            }
            
            sm_gs_send_msg_do_event(
                SM_EVN_SESS_PM_COND_CLOSED,
                h_sess,
                0 );
            
            /* if pm_close_service() success, service handle is already closed 
               synchronously. */
            pt_sess_obj->t_base.h_pm_service = NULL_HANDLE;
        }
        else
        {
            /* Pretend that PM Service is closed, if there is no handle. */
            sm_gs_send_msg_do_event(
                SM_EVN_SESS_PM_COND_CLOSED,
                h_sess,
                0 );
        }
    }
    else if ( ui4_event == SM_EVN_SESS_STRM_OPENED )
    {
        if ( pt_sess_obj->t_state.ui4_curr_state == SM_SESS_COND_STARTING )
        {
            /* play the stream */
            i4_ret = _sm_gs_stream_play( h_stream, pt_sess_obj );
        }
        else if ( pt_sess_obj->t_state.ui4_curr_state == SM_SESS_COND_CLOSING )
        {
            /* close the stream */
            i4_ret = sm_sess_gnrc_close( h_stream );
        }
    }
    else if ( ui4_event == SM_EVN_SESS_STRM_PLAYED )
    {
        if ( pt_sess_obj->t_state.ui4_curr_state == SM_SESS_COND_STOPPING )
        {
            /* stop the stream */
            i4_ret = _sm_gs_stream_stop( h_stream );
        }
        else if ( pt_sess_obj->t_state.ui4_curr_state == SM_SESS_COND_CLOSING )
        {
            /* close the stream */
            i4_ret = sm_sess_gnrc_close( h_stream );
        }
    }
    
    return SMR_OK;
}

static INT32 _sm_gs_state_do_stc_event(
    HANDLE_T              h_sess,
    SM_SESS_GNRC_T*       pt_sess_obj,
    UINT32                ui4_event )
{
    return SMR_OK;
}

static INT32 _sm_gs_state_do_dmx_event(
    HANDLE_T              h_sess,
    SM_SESS_GNRC_T*       pt_sess_obj,
    UINT32                ui4_event )
{
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
INT32 sm_gs_state_do_opening(     
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    INT32               i4_ret;
    SM_SESS_GNRC_T*     pt_sess_obj = NULL;
    HANDLE_T            h_sess;
    
    SM_ASSERT( pv_tag1 );
    
    pt_sess_obj = (SM_SESS_GNRC_T*)pv_tag1;
    h_sess = pt_sess_obj->t_base.h_sess;
    
    _sm_gs_state_record_latest_event( pt_sess_obj, ui4_event );
    
    /* Open SCDB */
    i4_ret = x_scdb_open( pt_sess_obj->t_base.h_connection,
                          &pt_sess_obj->t_base.h_scdb,
                          (VOID*)h_sess,
                          _sm_gs_scdb_nfy_fct );
    if ( i4_ret != SCDBR_OK )
    {
        pt_sess_obj->t_base.h_scdb = NULL_HANDLE;
        SM_GS_DBG_INFO(( 
            "[SM] SESS, x_scdb_open(0x%.8X) error, return %d!\r\n",
            (UINT32)h_sess, i4_ret ));
        
        sm_gs_send_msg_do_event(
            SM_EVN_SESS_INTERNAL_ERR,
            h_sess,
            0 );
        return SMR_STRM_COMP_ID_NOT_FOUND;
    }
    
    i4_ret = x_scdb_get_common( pt_sess_obj->t_base.h_scdb, 
                                &pt_sess_obj->t_scdb_cmmn );
    if ( i4_ret != SCDBR_OK )
    {
        SM_GS_DBG_INFO(( 
            "[SM] SESS, x_scdb_get_common(0x%.8X) error, return %d!\r\n",
            (UINT32)pt_sess_obj->t_base.h_scdb, i4_ret ));

        sm_gs_send_msg_do_event(
            SM_EVN_SESS_INTERNAL_ERR,
            h_sess,
            0 );
        return SMR_STRM_COMP_ID_NOT_FOUND;
    }
        
    /* Open PM service */
    TMS_BEGIN( GSS_HANDLE_PM_OPENED );
#ifdef ENABLE_MULTIMEDIA
    if (pt_sess_obj->t_media_fmt.e_media_type == MEDIA_TYPE_UNKNOWN)
#endif
    
    {
        i4_ret = pm_open_service( pt_sess_obj->t_base.h_connection,   
                                  PM_PRESENT,
                                  NULL,
                                  &pt_sess_obj->t_base.h_pm_service );
    }
#ifdef ENABLE_MULTIMEDIA
    else
    {
        PM_MULTIMEDIA_INFO_T t_mm_info;

        t_mm_info.t_media_fmt = pt_sess_obj->t_media_fmt;
        t_mm_info.e_priority = pt_sess_obj->e_priority;
        t_mm_info.ui4_list_size = 0;
        t_mm_info.pt_component_list = NULL;
        t_mm_info.b_es_with_pts_in = pt_sess_obj->b_es_with_pts_in;  
        i4_ret = pm_open_service( pt_sess_obj->t_base.h_connection,   
                                  PM_MULTIMEDIA,
                                  (VOID*)&t_mm_info,
                                  &pt_sess_obj->t_base.h_pm_service );
    }
#endif

    if ( i4_ret != PMR_OK )
    {
        pt_sess_obj->t_base.h_pm_service = NULL_HANDLE;
        SM_GS_DBG_INFO(( 
            "[SM] SESS, pm_open_service(0x%.8X) error, return %d!\r\n",
            (UINT32)h_sess, i4_ret ));

        sm_gs_send_msg_do_event(
            SM_EVN_SESS_INTERNAL_ERR,
            h_sess,
            0 );
        return SMR_INTERNAL_ERROR;
    }
    sm_gs_send_msg_do_event(
        SM_EVN_SESS_PM_COND_STOPPED,
        h_sess,
        0 );

        /* Open STC for multimedia playback on pull mode */
    if ( ( pt_sess_obj->t_media_fmt.e_media_type != MEDIA_TYPE_UNKNOWN &&
           !MM_IS_TIME_SHIFT(pt_sess_obj->t_media_fmt) &&
           !MM_IS_TIME_SHIFT_CMPB(pt_sess_obj->t_media_fmt)) ||
         ( (MM_IS_TIME_SHIFT(pt_sess_obj->t_media_fmt) ||
            MM_IS_TIME_SHIFT_CMPB(pt_sess_obj->t_media_fmt))&&
           pt_sess_obj->b_disable_av_sync == TRUE ) )
    {
        x_dbg_stmt("[SM]Open STC for multimedia playback on pull mode.\r\n");
        pt_sess_obj->b_disable_av_sync = TRUE;
#if defined( SYS_MHP_SUPPORT )||defined( MW_SUPPORT_2STC )
    }   
    {   // everytime stc must be opened
    #endif //SYS_MHP_SUPPORT
        i4_ret = sm_sess_gnrc_stc_open( h_sess, pt_sess_obj );
        if ( i4_ret != SMR_OK )
        {
            /* Ignore when the system has no STC controller */
            pt_sess_obj->t_stc_descr.h_comp = NULL_HANDLE;
            /*
            SM_GS_DBG_INFO(( 
                "[SM] SESS, sm_sess_gnrc_stc_open(0x%.8X) error, return %d!\r\n",
                (UINT32)h_sess, i4_ret ));

            sm_gs_send_msg_do_event(
                SM_EVN_SESS_DRV_ERR,
                h_sess,
                0 );
            return SMR_DRV_COMP_ERROR;
            */
        }
    }    
    /* Open PCR if application requests AV SYNC. */
#if defined( SYS_MHP_SUPPORT )||defined( MW_SUPPORT_2STC )
    if ( !pt_sess_obj->b_disable_av_sync )
    #else
    else if ( !pt_sess_obj->b_disable_av_sync )
    #endif //SYS_MHP_SUPPORT
    {
        TMS_BEGIN( GSS_HANDLE_PCR_OPENED );
        x_dbg_stmt("[SM]sm_sess_gnrc_pcr_open on pull mode.\r\n");
        i4_ret = sm_sess_gnrc_pcr_open( h_sess, pt_sess_obj );
        if ( i4_ret == SMR_DRV_COMP_NOT_FOUND )
        {
            /* Disable AV sync. if no driver is available */
            pt_sess_obj->b_disable_av_sync = TRUE;
        }
        else if ( i4_ret != SMR_OK )
        {
            pt_sess_obj->t_pcr_descr.h_comp = NULL_HANDLE;
            SM_GS_DBG_INFO(( 
                "[SM] SESS, sm_sess_gnrc_pcr_open(0x%.8X) error, return %d!\r\n",
                (UINT32)h_sess, i4_ret ));

            sm_gs_send_msg_do_event(
                SM_EVN_SESS_DRV_ERR,
                h_sess,
                0 );
            return SMR_DRV_COMP_ERROR;
        }
    }
    
   if (pt_sess_obj->t_media_fmt.e_media_type != MEDIA_TYPE_UNKNOWN)
   {
    /* Open DRVT_DEMUX_CONTROL to set demux mode */
    i4_ret = sm_sess_gnrc_dmx_open( h_sess, pt_sess_obj );
    if ( i4_ret == SMR_DRV_COMP_NOT_FOUND )
    {

    }
    else if ( i4_ret != SMR_OK )
    {
        /* Ignore when the system has no STC controller */
        pt_sess_obj->t_dmx_descr.h_comp = NULL_HANDLE;
        /*
        SM_GS_DBG_INFO(( 
            "[SM] SESS, sm_sess_gnrc_dmx_open(0x%.8X) error, return %d!\r\n",
            (UINT32)h_sess, i4_ret ));

        sm_gs_send_msg_do_event(
            SM_EVN_SESS_DRV_ERR,
            h_sess,
            0 );
        return SMR_DRV_COMP_ERROR;
        */
    }
   } 
    return SMR_OK;
}

INT32 sm_gs_state_do_opened(     
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_SESS_GNRC_T*     pt_sess_obj = NULL;
    DEMUX_MODE_T        e_dmx_mode = DEMUX_MODE_UNKNOWN;
    
    SM_ASSERT( pv_tag1 );
    
    pt_sess_obj = (SM_SESS_GNRC_T*)pv_tag1;
    
    _sm_gs_state_record_latest_event( pt_sess_obj, ui4_event );
    
    if(pt_sess_obj->b_sub_freerun)
    {
#ifdef ENABLE_MULTIMEDIA
        if ( PMR_OK != pm_set( pt_sess_obj->t_base.h_pm_service,
                               PM_SET_STC_ID,
#ifdef MW_SUPPORT_2STC
                               (VOID*)0xFF ) )
#else
                               (VOID*)1 ) )
#endif
        {
           x_dbg_stmt("[SM]set PM_SET_STC_ID ERROR! session %s, %d, sess=%x\r\n",__func__,__LINE__,pt_sess_obj->t_base.h_sess);
        }
        if ( PMR_OK != pm_set( pt_sess_obj->t_base.h_pm_service,
                               PM_SET_FREE_RUN,
                               (VOID*)TRUE ) )
        {
           x_dbg_stmt("[SM]set PM_SET_FREE_RUN ERROR! session %s, %d, sess=%x\r\n",__func__,__LINE__,pt_sess_obj->t_base.h_sess);
        }   
#endif        
        if ( pt_sess_obj->t_stc_descr.h_comp != NULL_HANDLE &&
             pt_sess_obj->t_stc_descr.e_comp_cond != RM_COND_CLOSED )
        {
            INT32 i4_ret;
            i4_ret = rm_set( pt_sess_obj->t_stc_descr.h_comp,
                             FALSE,
                             ANY_PORT_NUM,
                             STC_CTRL_SET_TYPE_STC_INVALID,
                             (VOID*)TRUE,
                             sizeof(BOOL),
                             &pt_sess_obj->t_stc_descr.e_comp_cond );
            if ( i4_ret != RMR_OK )
            {
                x_dbg_stmt("[SM]set STC_CTRL_SET_TYPE_STC_INVALID ERROR! session %s, %d, sess=%x\r\n",__func__,__LINE__,pt_sess_obj->t_base.h_sess);;
            }
        }
        
    }

/*    because of mm handler select svc  set e_mm_sync_mode as SM_SESS_GNRC_MM_SYNC_UNKNOWN 
         *    the code is not necessary 
         */
    #if 0
    if ( pt_sess_obj->e_mm_sync_mode == SM_SESS_GNRC_MM_SYNC_AUDIO_MASTER )
    {
        sm_sess_gnrc_stc_av_sync_mode( pt_sess_obj, STC_AV_SYNC_MODE_AUDIO_MASTER );
    }
    else if ( pt_sess_obj->e_mm_sync_mode == SM_SESS_GNRC_MM_SYNC_VIDEO_MASTER )
    {
        sm_sess_gnrc_stc_av_sync_mode( pt_sess_obj, STC_AV_SYNC_MODE_VIDEO_MASTER );
    }
    else if ( pt_sess_obj->e_mm_sync_mode == SM_SESS_GNRC_MM_SYNC_SYSTEM_MASTER )
    {
        sm_sess_gnrc_stc_av_sync_mode( pt_sess_obj, STC_AV_SYNC_MODE_SYSTEM_MASTER );
    }
    else if ( pt_sess_obj->e_mm_sync_mode == SM_SESS_GNRC_MM_SYNC_FREE_RUN )
    {
        sm_sess_gnrc_stc_av_sync_mode( pt_sess_obj, STC_AV_SYNC_MODE_FREE_RUN );
    }
    #endif
    if(SM_EVN_SESS_DRV_STOPPED != ui4_event)
    {
       /*no need to set stc when stopped*/
        sm_sess_gnrc_stc_set ( pt_sess_obj, pt_sess_obj->t_stc_init );
    }

    if ( ui4_previous_state == SM_COND_OPENING )
    {
        if ( MM_IS_TIME_SHIFT(pt_sess_obj->t_media_fmt) )
        {
            e_dmx_mode = DEMUX_MODE_TIME_SHIFT;
        }
        else if ( pt_sess_obj->t_media_fmt.e_media_type != MEDIA_TYPE_UNKNOWN )
        {
            e_dmx_mode = DEMUX_MODE_MM;
        }
        else
        {
            e_dmx_mode = DEMUX_MODE_DTV;
        }
        sm_sess_gnrc_dmx_set_mode ( pt_sess_obj, e_dmx_mode );
    }
    
    return SMR_OK;
}

INT32 sm_gs_state_do_starting(     
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    INT32               i4_ret;
    SM_SESS_GNRC_T*     pt_sess_obj = NULL;
    HANDLE_T            h_sess;
    
    SM_ASSERT( pv_tag1 );
    
    pt_sess_obj = (SM_SESS_GNRC_T*)pv_tag1;
    h_sess = pt_sess_obj->t_base.h_sess;
    
    _sm_gs_state_record_latest_event( pt_sess_obj, ui4_event );

    /* start service */
    TMS_BEGIN( GSS_HANDLE_PM_STARTED );
    i4_ret = pm_play_service( 
        pt_sess_obj->t_base.h_pm_service, 
        pt_sess_obj->t_base.i4_speed );
    if ( i4_ret != PMR_OK )
    {
        SM_GS_DBG_INFO(( 
            "[SM] SESS, pm_play_service(0x%.8X) error, return %d!\r\n",
            (UINT32)pt_sess_obj->t_base.h_pm_service, i4_ret ));

        sm_gs_send_msg_do_event(
            SM_EVN_SESS_DRV_ERR,
            h_sess,
            0 );
        return SMR_INTERNAL_ERROR;
    }
    sm_gs_send_msg_do_event(
        SM_EVN_SESS_PM_COND_PLAYED,
        h_sess,
        0 );
        
    /* start all streams */
    hash_lock( pt_sess_obj->t_base.h_strm_ctrl_tbl );

    i4_ret = hash_parse( pt_sess_obj->t_base.h_strm_ctrl_tbl,
                         _sm_gs_start_hash_cb,
                         (VOID*)pt_sess_obj );

    hash_unlock( pt_sess_obj->t_base.h_strm_ctrl_tbl );

    if ( i4_ret != HASH_OK )
    {
        SM_GS_DBG_INFO(( 
            "[SM] SESS(0x%.8x), hash_parse(_sm_gs_start_hash_cb) error, return %d!\r\n",
            (UINT32)h_sess, i4_ret ));

        sm_gs_send_msg_do_event(
            SM_EVN_SESS_INTERNAL_ERR,
            h_sess,
            0 );
        return SMR_INTERNAL_ERROR;
    }
    
    return SMR_OK;
}

INT32 sm_gs_state_do_started(
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    /*INT32             i4_ret;*/
    SM_SESS_GNRC_T*     pt_sess_obj = NULL;
    /*HANDLE_T          h_sess;*/
    
    SM_ASSERT( pv_tag1 );
    
    pt_sess_obj = (SM_SESS_GNRC_T*)pv_tag1;
    /*h_sess = pt_sess_obj->t_base.h_sess;*/
    
    _sm_gs_state_record_latest_event( pt_sess_obj, ui4_event );

    /* start STC */
    /*i4_ret = sm_sess_gnrc_stc_ctrl( pt_sess_obj, STC_CTRL_START );
    if ( i4_ret != SMR_OK )
    {
        SM_GS_DBG_INFO(( 
            "[SM] SESS, sm_sess_gnrc_stc_ctrl(0x%.8X) start error, return %d!\r\n",
            (UINT32)pt_sess_obj->t_stc_descr.h_comp, i4_ret ));

        sm_gs_send_msg_do_event(
            SM_EVN_SESS_DRV_ERR,
            h_sess,
            0 );
        return SMR_INTERNAL_ERROR;
    }*/

    return SMR_OK;
}

INT32 sm_gs_state_do_stopping(     
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    INT32               i4_ret;
    SM_SESS_GNRC_T*     pt_sess_obj = NULL;
    HANDLE_T            h_sess;
    
    SM_ASSERT( pv_tag1 );
    
    pt_sess_obj = (SM_SESS_GNRC_T*)pv_tag1;
    h_sess = pt_sess_obj->t_base.h_sess;

    _sm_gs_state_record_latest_event( pt_sess_obj, ui4_event );

    /* stop STC */
    i4_ret = sm_sess_gnrc_stc_ctrl( pt_sess_obj, STC_CTRL_STOP );
    if ( i4_ret != SMR_OK )
    {
        SM_GS_DBG_INFO(( 
            "[SM] SESS, sm_sess_gnrc_stc_ctrl(0x%.8X) stop error, return %d!\r\n",
            (UINT32)pt_sess_obj->t_stc_descr.h_comp, i4_ret ));

        sm_gs_send_msg_do_event(
            SM_EVN_SESS_DRV_ERR,
            h_sess,
            0 );
        return SMR_INTERNAL_ERROR;
    }

    /* stop service */
    TMS_BEGIN( GSS_HANDLE_PM_STOPPED );
    SM_GS_DBG_INFO(( 
            "[SM] SESS, pm_stop_service(0x%.8X) start \r\n",
            (UINT32)pt_sess_obj->t_base.h_pm_service ));
    i4_ret = pm_stop_service( pt_sess_obj->t_base.h_pm_service );
    SM_GS_DBG_INFO(( 
            "[SM] SESS, pm_stop_service(0x%.8X) end \r\n",
            (UINT32)pt_sess_obj->t_base.h_pm_service ));
    if ( i4_ret != PMR_OK )
    {
        SM_GS_DBG_INFO(( 
            "[SM] SESS, pm_stop_service(0x%.8X) error, return %d!\r\n",
            (UINT32)pt_sess_obj->t_base.h_pm_service, i4_ret ));

        sm_gs_send_msg_do_event(
            SM_EVN_SESS_DRV_ERR,
            h_sess,
            0 );
        return SMR_INTERNAL_ERROR;        
    }
    sm_gs_send_msg_do_event(
        SM_EVN_SESS_PM_COND_STOPPED,
        h_sess,
        0 );
    
    /* stop all streams */
    hash_lock( pt_sess_obj->t_base.h_strm_ctrl_tbl );

    i4_ret = hash_parse( pt_sess_obj->t_base.h_strm_ctrl_tbl,
                         _sm_gs_stop_hash_cb,
                         (VOID*)pt_sess_obj );

    hash_unlock( pt_sess_obj->t_base.h_strm_ctrl_tbl );

    if ( i4_ret != HASH_OK )
    {
        SM_GS_DBG_INFO(( 
            "[SM] SESS(0x%.8x), hash_parse(_sm_gs_stop_hash_cb) error, return %d!\r\n",
            (UINT32)h_sess, i4_ret ));

        sm_gs_send_msg_do_event(
            SM_EVN_SESS_INTERNAL_ERR,
            h_sess,
            0 );
        return SMR_INTERNAL_ERROR;        
    }

    return SMR_OK;
}

INT32 sm_gs_state_do_closing(     
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    INT32               i4_ret = SMR_OK;
    SIZE_T              z_num_strm = 0;
    SM_SESS_GNRC_T*     pt_sess_obj = NULL;
    HANDLE_T            h_sess;
    
    SM_ASSERT( pv_tag1 );
    
    pt_sess_obj = (SM_SESS_GNRC_T*)pv_tag1;
    h_sess = pt_sess_obj->t_base.h_sess;
    
    _sm_gs_state_record_latest_event( pt_sess_obj, ui4_event );

    /* close all streams */
    hash_lock( pt_sess_obj->t_base.h_strm_ctrl_tbl );
    
    hash_get_size( pt_sess_obj->t_base.h_strm_ctrl_tbl, &z_num_strm );
    
    if ( z_num_strm > 0 )
    {
        i4_ret = hash_parse( pt_sess_obj->t_base.h_strm_ctrl_tbl,
                             _sm_gs_strm_close_hash_cb,
                             (VOID*)pt_sess_obj );
    }
    
    hash_unlock( pt_sess_obj->t_base.h_strm_ctrl_tbl );
    
    if ( i4_ret != HASH_OK )
    {
        SM_GS_DBG_INFO(( 
            "[SM] SESS(0x%.8x), hash_parse(_sm_gs_strm_close_hash_cb) error, return %d!\r\n",
            (UINT32)h_sess, i4_ret ));
        SM_ASSERT( 0 );
        
        /* In release version, pretend that all streams are closed. */
        sm_gs_send_msg_do_event(
            SM_EVN_SESS_STRM_ALL_CLOSED,
            h_sess,
            0 );
    }

    if ( !pt_sess_obj->b_disable_av_sync || 
         pt_sess_obj->t_pcr_descr.h_comp != NULL_HANDLE )
    {
        TMS_BEGIN( GSS_HANDLE_PCR_DISCONNECTED );
        
        i4_ret = sm_sess_gnrc_pcr_disconnect( h_sess, pt_sess_obj );
        if ( i4_ret != SMR_OK )
        {
            SM_GS_DBG_INFO(( 
                "[SM] sm_sess_gnrc_pcr_disconnect() fail, return %d!\r\n",
                i4_ret ));
            
            /* In release version, pretend that PCR is disconnected. */
            sm_gs_send_msg_do_event(
                SM_EVN_SESS_PCR_DISCONNECTED,
                h_sess,
                0 );
            /* continue close other components */
        }
    }

    if ( pt_sess_obj->t_stc_descr.h_comp != NULL_HANDLE )
    {
        if(pt_sess_obj->b_sub_freerun)
        {
             if ( pt_sess_obj->t_stc_descr.h_comp != NULL_HANDLE &&
                 pt_sess_obj->t_stc_descr.e_comp_cond != RM_COND_CLOSED )
            {
                INT32 i4_ret;
                i4_ret = rm_set( pt_sess_obj->t_stc_descr.h_comp,
                                 FALSE,
                                 ANY_PORT_NUM,
                                 STC_CTRL_SET_TYPE_STC_INVALID,
                                 (VOID*)FALSE,
                                 sizeof(BOOL),
                                 &pt_sess_obj->t_stc_descr.e_comp_cond );
                if ( i4_ret != RMR_OK )
                {
                    x_dbg_stmt("[SM]set STC_CTRL_SET_TYPE_STC_INVALID ERROR! session %s, %d, sess=%x\r\n",__func__,__LINE__,pt_sess_obj->t_base.h_sess);;
                }
            }
        }
        i4_ret = sm_sess_gnrc_stc_close( h_sess, pt_sess_obj );
        if ( i4_ret != SMR_OK )
        {
            SM_GS_DBG_INFO(( 
                "[SM] sm_sess_gnrc_stc_close() fail, return %d!\r\n",
                i4_ret ));

            /* Pretend closed */
            sm_gs_send_msg_do_event(
                SM_EVN_SESS_STC_CLOSED,
                h_sess,
                0 );
            /* continue close other components */
        }
    }
    
    if ( pt_sess_obj->t_dmx_descr.h_comp != NULL_HANDLE )
    {
        sm_sess_gnrc_dmx_set_mode ( pt_sess_obj, DEMUX_MODE_DTV );
        i4_ret = sm_sess_gnrc_dmx_close( h_sess, pt_sess_obj );
        if ( i4_ret != SMR_OK )
        {
            SM_GS_DBG_INFO(( 
                "[SM] sm_sess_gnrc_dmx_close() fail, return %d!\r\n",
                i4_ret ));

            /* Pretend closed */
            sm_gs_send_msg_do_event(
                SM_EVN_SESS_DMX_CLOSED,
                h_sess,
                0 );
            /* continue close other components */
        }
    }
    
    if ( pt_sess_obj->t_base.h_scdb != NULL_HANDLE )
    {
        i4_ret = x_scdb_close( pt_sess_obj->t_base.h_scdb );
        if ( i4_ret != SCDBR_OK )
        {
            SM_GS_DBG_INFO(( 
                "[SM] x_scdb_close() fail, return %d!\r\n",
                i4_ret ));            
            /* In release version, pretend that SCDB is closed. */
            sm_gs_send_msg_do_event(
                SM_EVN_SESS_SCDB_CLOSED,
                h_sess,
                0 );
        }
        pt_sess_obj->t_base.h_scdb = NULL_HANDLE;
    }
    else
    {
        /* Pretend that SCDB is closed, if there is no SCDB handle. */
        sm_gs_send_msg_do_event(
            SM_EVN_SESS_SCDB_CLOSED,
            h_sess,
            0 );
    }
    
    /* Close PM service, if there is no stream in the session */
    if ( z_num_strm == 0 )
    {
        if ( pt_sess_obj->t_base.h_pm_service != NULL_HANDLE )
        {
            TMS_BEGIN( GSS_HANDLE_PM_CLOSED );
            
            i4_ret = pm_close_service( pt_sess_obj->t_base.h_pm_service );
            if ( i4_ret != PMR_OK && i4_ret != PMR_INV_HANDLE )
            {
                SM_GS_DBG_INFO(( 
                    "[SM] pm_close_service() fail, return %d!\r\n",
                    i4_ret ));            
            }
    
            sm_gs_send_msg_do_event(
                SM_EVN_SESS_PM_COND_CLOSED,
                h_sess,
                0 );
           
            /* if pm_close_service() success, service handle is already closed 
               synchronously. */
            pt_sess_obj->t_base.h_pm_service = NULL_HANDLE;
        }
        else
        {
            /* Pretend that PM Service is closed, if there is no handle. */
            sm_gs_send_msg_do_event(
                SM_EVN_SESS_PM_COND_CLOSED,
                h_sess,
                0 );
        }
    }
    
    return SMR_OK;
}

INT32 sm_gs_state_do_closed(     
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_SESS_GNRC_T*     pt_sess_obj = NULL;
    
    SM_ASSERT( pv_tag1 );
    
    pt_sess_obj = (SM_SESS_GNRC_T*)pv_tag1;

    _sm_gs_state_record_latest_event( pt_sess_obj, ui4_event );

    return SMR_OK;
}

INT32 sm_gs_state_do_error(     
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_SESS_GNRC_T*     pt_sess_obj = NULL;
    
    SM_ASSERT( pv_tag1 );
    
    pt_sess_obj = (SM_SESS_GNRC_T*)pv_tag1;

    _sm_gs_state_record_latest_event( pt_sess_obj, ui4_event );

    return SMR_OK;
}

INT32 sm_gs_state_do_ignore_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_SESS_GNRC_T*     pt_sess_obj = NULL;
    
    SM_ASSERT( pv_tag1 );
    
    pt_sess_obj = (SM_SESS_GNRC_T*)pv_tag1;

    _sm_gs_state_record_latest_event( pt_sess_obj, ui4_event );

    return SMR_OK;
}

INT32 sm_gs_state_do_opening_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_SESS_GNRC_T*     pt_sess_obj = NULL;
    
    SM_ASSERT( pv_tag1 );
    
    pt_sess_obj = (SM_SESS_GNRC_T*)pv_tag1;
        
    _sm_gs_state_record_latest_event( pt_sess_obj, ui4_event );

    return SMR_OK;
}

INT32 sm_gs_state_do_opened_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_SESS_GNRC_T*     pt_sess_obj = NULL;
    
    SM_ASSERT( pv_tag1 );
    
    pt_sess_obj = (SM_SESS_GNRC_T*)pv_tag1;

    _sm_gs_state_record_latest_event( pt_sess_obj, ui4_event );

    return SMR_OK;
}

INT32 sm_gs_state_do_starting_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_SESS_GNRC_T*     pt_sess_obj = NULL;
    
    SM_ASSERT( pv_tag1 );
    
    pt_sess_obj = (SM_SESS_GNRC_T*)pv_tag1;

    _sm_gs_state_record_latest_event( pt_sess_obj, ui4_event );
    
    return SMR_OK;
}

INT32 sm_gs_state_do_started_excpt(
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_SESS_GNRC_T*     pt_sess_obj = NULL;
    
    SM_ASSERT( pv_tag1 );
    
    pt_sess_obj = (SM_SESS_GNRC_T*)pv_tag1;

    _sm_gs_state_record_latest_event( pt_sess_obj, ui4_event );
    
    return SMR_OK;
}

INT32 sm_gs_state_do_stopping_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_SESS_GNRC_T*     pt_sess_obj = NULL;
    
    SM_ASSERT( pv_tag1 );
    
    pt_sess_obj = (SM_SESS_GNRC_T*)pv_tag1;
    
    _sm_gs_state_record_latest_event( pt_sess_obj, ui4_event );
    
    if ( ui4_event == SM_EVN_SESS_APP_CLOSE_REQ )
    {
        
    }
    
    return SMR_OK;
}

INT32 sm_gs_state_do_closing_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_SESS_GNRC_T*     pt_sess_obj = NULL;
    
    SM_ASSERT( pv_tag1 );
    
    pt_sess_obj = (SM_SESS_GNRC_T*)pv_tag1;

    _sm_gs_state_record_latest_event( pt_sess_obj, ui4_event );
    
    return SMR_OK;
}

INT32 sm_gs_state_do_closed_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_SESS_GNRC_T*     pt_sess_obj = NULL;
    
    SM_ASSERT( pv_tag1 );
    
    pt_sess_obj = (SM_SESS_GNRC_T*)pv_tag1;

    _sm_gs_state_record_latest_event( pt_sess_obj, ui4_event );
    
    return SMR_OK;
}

INT32 sm_gs_state_do_error_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SM_SESS_GNRC_T*     pt_sess_obj = NULL;
    
    SM_ASSERT( pv_tag1 );
    
    pt_sess_obj = (SM_SESS_GNRC_T*)pv_tag1;

    _sm_gs_state_record_latest_event( pt_sess_obj, ui4_event );
    
    return SMR_OK;
}

VOID sm_gs_state_notify_app( 
    VOID*                 pv_tag,
    SM_SESS_COND_T        e_cond,
    UINT32                ui4_event,
    UINT32                ui4_data )
{
    INT32               i4_ret;
    SM_GS_STATE_TAG_T*  pt_state_tag = (SM_GS_STATE_TAG_T*)pv_tag;
    SM_SESS_GNRC_T*     pt_sess_obj = NULL;
    HANDLE_T            h_sess;
    
    SM_ASSERT( pt_state_tag );
    SM_ASSERT( pt_state_tag->h_sess != NULL_HANDLE );
    SM_ASSERT( pt_state_tag->pt_sess_obj );
    
    h_sess = (HANDLE_T)pt_state_tag->h_sess;
    pt_sess_obj = (SM_SESS_GNRC_T*)pt_state_tag->pt_sess_obj;

#ifdef TIME_MEASUREMENT
    _sm_gss_state_time_msrt( 
        (UINT32)pt_sess_obj->t_base.e_sess_cond,
        (UINT32)e_cond,
        ui4_event );
#endif /* TIME_MEASUREMENT */
    
    pt_sess_obj->t_base.e_sess_cond = e_cond;
    
    /* Notify stream event to application */
    i4_ret = sm_sess_nfy( h_sess, 
                          e_cond, 
                          pt_sess_obj->t_base.pv_sess_nfy_tag, 
                          ui4_event,
                          ui4_data );
    if ( i4_ret != SMR_OK )
    {
        SM_GS_DBG_ERR(( 
            "[SM] sm_sess_nfy() error, SESS(0x%.8X), COND(0x%.8X), "
            "EVN(0x%.8X), DATA(0x%.8X), return = %d\r\n",
            (UINT32)h_sess,
            (UINT32)e_cond,
            (UINT32)ui4_event,
            (UINT32)ui4_data,
            (UINT32)i4_ret ));
        return;
    }
#ifdef DEBUG
    {
        const CHAR* ps_state = sm_state_event_to_str(ui4_event);
        
        if ( ps_state )
        {
            SM_GS_DBG_INFO((
                "[SM] SESS(0x%.8X), %s, %s, DATA(0x%.8X)\r\n",
                (UINT32)h_sess,
                sm_sess_cond_2_str(e_cond),
                ps_state,
                (UINT32)ui4_data ));
        }
        else
        {
            SM_GS_DBG_INFO((
                "[SM] SESS(0x%.8X), %s, EVN(0x%.8X), DATA(0x%.8X)\r\n",
                (UINT32)h_sess,
                sm_sess_cond_2_str(e_cond),
                (UINT32)ui4_event,
                (UINT32)ui4_data ));
        }
    }
#endif /* DEBUG */
}

INT32 sm_gs_state_do_drv_events(     
    UINT32     ui4_curr_state,
    UINT32     ui4_event,
    UINT32     ui4_next_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    INT32               i4_ret = SMR_OK;
    SM_SESS_GNRC_T*     pt_sess_obj = NULL;
    HANDLE_T            h_sess;
    
    SM_ASSERT( pv_tag1 );
    
    pt_sess_obj = (SM_SESS_GNRC_T*)pv_tag1;
    h_sess = pt_sess_obj->t_base.h_sess;
    
    if ( !_sm_gs_state_record_latest_event( pt_sess_obj, ui4_event ) )
    {
        /* Skip the event if there is no changes */
        return SMR_OK;
    }

    switch ( SM_EVN_GET_GRP(ui4_event) )
    {
    case SM_EVN_SESS_GRP_PCR:
        i4_ret = _sm_gs_state_do_pcr_event( h_sess, pt_sess_obj, ui4_event );
        break;

    case SM_EVN_SESS_GRP_PM:
        i4_ret = _sm_gs_state_do_pm_event( h_sess, pt_sess_obj, ui4_event );
        break;

    case SM_EVN_SESS_GRP_STRM:
        i4_ret = _sm_gs_state_do_strm_event( 
            h_sess, 
            pt_sess_obj, 
            ui4_event, 
            (UINT32)pv_tag2 );
        break;

    case SM_EVN_SESS_GRP_STC:
        i4_ret = _sm_gs_state_do_stc_event( h_sess, pt_sess_obj, ui4_event );
        break;

    case SM_EVN_SESS_GRP_DMX:
        i4_ret = _sm_gs_state_do_dmx_event( h_sess, pt_sess_obj, ui4_event );
        break;

    default:
        SM_GS_DBG_ERR(( 
            "[SM] sm_gs_state_do_drv_events() receives invalid event 0x%.8X\r\n",
            ui4_event ));
        SM_ABORT( SM_DBG_ABRT_INV_EVENT );
    }
    
    if ( i4_ret != SMR_OK )
    {
        return i4_ret;
    }
#if !defined( SYS_MHP_SUPPORT )&&!defined( MW_SUPPORT_2STC )
    if ( pt_sess_obj->ui4_lastest_pm_event == SM_EVN_SESS_PM_COND_STOPPED &&
         ( ( pt_sess_obj->b_disable_av_sync &&
             pt_sess_obj->t_media_fmt.e_media_type == MEDIA_TYPE_UNKNOWN ) ||
           pt_sess_obj->ui4_lastest_pcr_event == SM_EVN_SESS_PCR_PID_UPDATE ||
           pt_sess_obj->ui4_lastest_stc_event == SM_EVN_SESS_STC_OPENED ) &&
         DMX_NOT_EXIST_OR_LAST_EVENT_IS(pt_sess_obj, SM_EVN_SESS_DMX_OPENED) &&
         ( pt_sess_obj->ui4_lastest_strm_event == SM_EVN_IGNORE ||
           pt_sess_obj->ui4_lastest_strm_event == SM_EVN_SESS_STRM_ALL_OPENED ||
           pt_sess_obj->ui4_lastest_strm_event == SM_EVN_SESS_STRM_ALL_CLOSED ) )
    #else
    if (( pt_sess_obj->ui4_lastest_pm_event == SM_EVN_SESS_PM_COND_STOPPED &&
         ( ( pt_sess_obj->b_disable_av_sync &&
             pt_sess_obj->t_media_fmt.e_media_type == MEDIA_TYPE_UNKNOWN) ||
           (  !pt_sess_obj->b_disable_av_sync &&pt_sess_obj->ui4_lastest_pcr_event == SM_EVN_SESS_PCR_PID_UPDATE) ||
           (pt_sess_obj->b_disable_av_sync &&(pt_sess_obj->t_media_fmt.e_media_type != MEDIA_TYPE_UNKNOWN))) &&
         DMX_NOT_EXIST_OR_LAST_EVENT_IS(pt_sess_obj, SM_EVN_SESS_DMX_OPENED) &&
         ( pt_sess_obj->ui4_lastest_strm_event == SM_EVN_IGNORE ||
           pt_sess_obj->ui4_lastest_strm_event == SM_EVN_SESS_STRM_ALL_OPENED ||
           pt_sess_obj->ui4_lastest_strm_event == SM_EVN_SESS_STRM_ALL_CLOSED ) )&&pt_sess_obj->ui4_lastest_stc_event == SM_EVN_SESS_STC_OPENED)
    #endif
    {
        if ( pt_sess_obj->t_state.ui4_curr_state == SM_SESS_COND_OPENING )
        {
            sm_gs_send_msg_do_event(
                SM_EVN_SESS_DRV_OPENED,
                h_sess,
                0 );
        }
        else
        {
            sm_gs_send_msg_do_event(
                SM_EVN_SESS_DRV_STOPPED,
                h_sess,
                0 );
        }
    }
#if !defined( SYS_MHP_SUPPORT )&&!defined( MW_SUPPORT_2STC )
    else if ( pt_sess_obj->ui4_lastest_pm_event == SM_EVN_SESS_PM_COND_PLAYED &&
              ( ( pt_sess_obj->b_disable_av_sync &&
                  pt_sess_obj->t_media_fmt.e_media_type == MEDIA_TYPE_UNKNOWN ) ||
                pt_sess_obj->ui4_lastest_pcr_event == SM_EVN_SESS_PCR_PID_UPDATE ||
                pt_sess_obj->ui4_lastest_stc_event == SM_EVN_SESS_STC_OPENED ) &&
              DMX_NOT_EXIST_OR_LAST_EVENT_IS(pt_sess_obj, SM_EVN_SESS_DMX_OPENED) &&
              ( pt_sess_obj->ui4_lastest_strm_event == SM_EVN_IGNORE ||
                pt_sess_obj->ui4_lastest_strm_event == SM_EVN_SESS_STRM_ALL_PLAYED ||
                pt_sess_obj->ui4_lastest_strm_event == SM_EVN_SESS_STRM_ALL_CLOSED ) )
   #else
     else if (( pt_sess_obj->ui4_lastest_pm_event == SM_EVN_SESS_PM_COND_PLAYED &&
              ( ( pt_sess_obj->b_disable_av_sync &&
                  pt_sess_obj->t_media_fmt.e_media_type == MEDIA_TYPE_UNKNOWN ) ||
                ( !pt_sess_obj->b_disable_av_sync &&pt_sess_obj->ui4_lastest_pcr_event == SM_EVN_SESS_PCR_PID_UPDATE )||
                (pt_sess_obj->b_disable_av_sync &&(pt_sess_obj->t_media_fmt.e_media_type != MEDIA_TYPE_UNKNOWN))) &&
              DMX_NOT_EXIST_OR_LAST_EVENT_IS(pt_sess_obj, SM_EVN_SESS_DMX_OPENED) &&
              ( pt_sess_obj->ui4_lastest_strm_event == SM_EVN_IGNORE ||
                pt_sess_obj->ui4_lastest_strm_event == SM_EVN_SESS_STRM_ALL_PLAYED ||
                pt_sess_obj->ui4_lastest_strm_event == SM_EVN_SESS_STRM_ALL_CLOSED ) )&&pt_sess_obj->ui4_lastest_stc_event == SM_EVN_SESS_STC_OPENED)
    #endif
    {
        sm_gs_send_msg_do_event(
            SM_EVN_SESS_DRV_PLAYED,
            h_sess,
            0 );
    }
#if !defined( SYS_MHP_SUPPORT )&&!defined( MW_SUPPORT_2STC )
    else if ( pt_sess_obj->ui4_lastest_pm_event == SM_EVN_SESS_PM_COND_CLOSED &&
              ( ( pt_sess_obj->b_disable_av_sync &&
                  pt_sess_obj->t_media_fmt.e_media_type == MEDIA_TYPE_UNKNOWN ) || 
                pt_sess_obj->ui4_lastest_pcr_event == SM_EVN_SESS_PCR_CLOSED ||
                pt_sess_obj->ui4_lastest_stc_event == SM_EVN_SESS_STC_CLOSED ) &&
              DMX_NOT_EXIST_OR_LAST_EVENT_IS(pt_sess_obj, SM_EVN_SESS_DMX_CLOSED) &&
              ( pt_sess_obj->ui4_lastest_strm_event == SM_EVN_IGNORE ||
                pt_sess_obj->ui4_lastest_strm_event == SM_EVN_SESS_STRM_ALL_CLOSED ) )
    #else
    else if (( pt_sess_obj->ui4_lastest_pm_event == SM_EVN_SESS_PM_COND_CLOSED &&
              ( ( pt_sess_obj->b_disable_av_sync &&
                  pt_sess_obj->t_media_fmt.e_media_type == MEDIA_TYPE_UNKNOWN ) || 
                (  !pt_sess_obj->b_disable_av_sync &&pt_sess_obj->ui4_lastest_pcr_event == SM_EVN_SESS_PCR_CLOSED ) ||
                ( pt_sess_obj->b_disable_av_sync &&pt_sess_obj->t_media_fmt.e_media_type != MEDIA_TYPE_UNKNOWN) ) &&
              DMX_NOT_EXIST_OR_LAST_EVENT_IS(pt_sess_obj, SM_EVN_SESS_DMX_CLOSED) &&
              ( pt_sess_obj->ui4_lastest_strm_event == SM_EVN_IGNORE ||
                pt_sess_obj->ui4_lastest_strm_event == SM_EVN_SESS_STRM_ALL_CLOSED ) )&&pt_sess_obj->ui4_lastest_stc_event == SM_EVN_SESS_STC_CLOSED)
    #endif
    {
        sm_gs_send_msg_do_event(
            SM_EVN_SESS_DRV_CLOSED,
            h_sess,
            0 );
    }
    
    return SMR_OK;
}
