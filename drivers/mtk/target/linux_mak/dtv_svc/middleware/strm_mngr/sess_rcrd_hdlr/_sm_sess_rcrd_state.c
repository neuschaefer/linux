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
 * $RCSfile: _sm_sess_rcrd_state.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains Stream Manager's record session state 
 *         transfer info.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "inc/x_mm_common.h"
#include "handle/u_handle.h"
#include "handle/x_handle.h"
#include "util/x_lnk_list.h"
#include "os/inc/x_os.h"

#include "hash/hash_api.h"
#include "scdb/scdb_api.h"
    
#include "prc_mngr/prc_mngr.h"
    
#include "strm_mngr/sess_rcrd_hdlr/_sm_sess_rcrd_state.h"
#include "strm_mngr/sess_rcrd_hdlr/_sm_sess_rcrd_pvr.h"
#include "strm_mngr/sess_rcrd_hdlr/_sm_sess_rcrd_rutil.h"
#include "strm_mngr/sess_rcrd_hdlr/sm_sess_rcrd.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#ifdef TIME_MEASUREMENT
#define RCRD_SESS_HANDLE_IGNORE_2_OPEN_STR            "RCRD_SESS:IGNORE-OPENED"
#define RCRD_SESS_HANDLE_IGNORE_2_STARTED_STR         "RCRD_SESS:IGNORE-STARTED"
#define RCRD_SESS_HANDLE_STARTED_2_STOPPED_STR        "RCRD_SESS:STARTED-STOPPED"
#define RCRD_SESS_HANDLE_STARTED_2_CLOSED_STR         "RCRD_SESS:STARTED-CLOSED"

#define RCRD_SESS_HANDLE_PM_OPENED                    "RCRD_SESS:PM-OPENED"
#define RCRD_SESS_HANDLE_PM_STARTED                   "RCRD_SESS:PM-STARTED"
#define RCRD_SESS_HANDLE_PM_STOPPED                   "RCRD_SESS:PM-STOPPED"
#define RCRD_SESS_HANDLE_PM_CLOSED                    "RCRD_SESS:PM-CLOSED"

#define RCRD_SESS_HANDLE_PVR_OPENED                   "RCRD_SESS:PVR-OPEN"
#define RCRD_SESS_HANDLE_PVR_CONNECTED                "RCRD_SESS:PVR-CONN"
#define RCRD_SESS_HANDLE_PVR_DISCONNECTED             "RCRD_SESS:PVR-DISCONN"
#define RCRD_SESS_HANDLE_PVR_CLOSED                   "RCRD_SESS:PVR-CLOSE"

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


/* The function may need be modified and I am not sure of it*/
#ifdef TIME_MEASUREMENT
static VOID _sm_rcrd_sess_state_time_msrt(UINT32  ui4_curr_state,
                                          UINT32  ui4_next_state,
                                          UINT32  ui4_event)
{
    if ((ui4_curr_state == SM_SESS_COND_IGNORE)  
         && (ui4_next_state == SM_SESS_COND_OPENING) )
    {
        TMS_BEGIN(RCRD_SESS_HANDLE_IGNORE_2_OPEN_STR);
        TMS_BEGIN(RCRD_SESS_HANDLE_IGNORE_2_STARTED_STR);
    }
    else if ((ui4_curr_state == SM_SESS_COND_OPENING) 
              && (ui4_next_state == SM_SESS_COND_OPENED))
    {
        TMS_END(RCRD_SESS_HANDLE_IGNORE_2_OPEN_STR);
    }
    else if ((ui4_curr_state == SM_SESS_COND_STARTING) 
              && (ui4_next_state == SM_SESS_COND_STARTED))
    {
        TMS_END(RCRD_SESS_HANDLE_IGNORE_2_STARTED_STR);
    }
    else if ((ui4_curr_state == SM_SESS_COND_STARTED) 
              && (ui4_next_state == SM_SESS_COND_STOPPING))
    {
        TMS_BEGIN(RCRD_SESS_HANDLE_STARTED_2_STOPPED_STR);
        TMS_BEGIN(RCRD_SESS_HANDLE_STARTED_2_CLOSED_STR);
    }
    else if ((ui4_curr_state == SM_SESS_COND_STOPPING) 
             && (ui4_next_state == SM_SESS_COND_OPENED))
    {
        TMS_END(RCRD_SESS_HANDLE_STARTED_2_STOPPED_STR);
    }
    else if ((ui4_curr_state == SM_SESS_COND_CLOSING)  
              && (ui4_next_state == SM_SESS_COND_CLOSED))
    {
        TMS_END(RCRD_SESS_HANDLE_STARTED_2_CLOSED_STR);
    }
    
    return;
}

/* The function may need be modified and I am not sure of it*/
static VOID _sm_rcrd_sess_drv_time_msrt(UINT32 ui4_event)
{

    switch( ui4_event )
    {
        case SM_EVN_SESS_RCRD_ALL_OPENED:
            TMS_END(RCRD_SESS_HANDLE_PVR_OPENED);
            break;
        
        case SM_EVN_SESS_RCRD_PVR_CLOSED:
            TMS_END(RCRD_SESS_HANDLE_PVR_CLOSED);
            break;
        default:
            break;    
    }

    return;
}
#endif /* TIME_MEASUREMENT */

/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_sess_state_record_latest_event
 *
 * Description: This function records latest events for do state machine transfering 
 *
 * Inputs:  
 *       pt_sess_obj:  session object
 *       e_event:      event e.g.PM-DMX/RM-PVR_CTRL/Record Utiltiy/SCDB/APP
 *         
 * Outputs: 
 *       Record latest events in session object
 *
 * Returns: TRUE:  record.
 *          FALSE: has be recorded before
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
static BOOL _sm_rcrd_sess_state_record_latest_event(SM_SESS_RCRD_T* pt_sess_obj,
                                                    UINT32          ui4_event)
{
    switch (SM_EVN_GET_GRP(ui4_event))
    {
        case SM_EVN_SESS_RCRD_GRP_PVR:
            if (pt_sess_obj->ui4_lastest_pvr_event == ui4_event)
            {
                return FALSE;
            }
            pt_sess_obj->ui4_lastest_pvr_event = ui4_event;
            break;

        case SM_EVN_SESS_RCRD_GRP_PM:
            /* TIME_MEASUREMENT */
            #ifdef TIME_MEASUREMENT
            if ((SM_EVN_IGNORE == pt_sess_obj->ui4_lastest_pm_event)
                 && (SM_EVN_SESS_RCRD_PM_COND_STOPPED == ui4_event))
            {
                TMS_END(RCRD_SESS_HANDLE_PM_OPENED);
            }
            else if ((SM_EVN_SESS_RCRD_PM_COND_PLAYED == pt_sess_obj->ui4_lastest_pm_event)
                      && (SM_EVN_SESS_RCRD_PM_COND_STOPPED == ui4_event))
            {
                TMS_END(RCRD_SESS_HANDLE_PM_STOPPED);
            }
            else if (SM_EVN_SESS_RCRD_PM_COND_PLAYED == ui4_event)
            {
                TMS_END(RCRD_SESS_HANDLE_PM_STARTED);
            }
            #endif 

            if (pt_sess_obj->ui4_lastest_pm_event == ui4_event)
            {
                return FALSE;
            }
            pt_sess_obj->ui4_lastest_pm_event = ui4_event;
            break;
            
        case SM_EVN_SESS_RCRD_GRP_STRM:
            if ((pt_sess_obj->ui4_lastest_strm_event == ui4_event)
                 || (pt_sess_obj->ui4_lastest_all_strm_event == ui4_event))
            {
                return FALSE;
            }
            if ((SM_EVN_SESS_RCRD_STRM_ALL_OPENED == ui4_event)
                 || (SM_EVN_SESS_RCRD_STRM_ALL_PLAYED == ui4_event)
                 || (SM_EVN_SESS_RCRD_STRM_ALL_CLOSED == ui4_event))
            {
                pt_sess_obj->ui4_lastest_all_strm_event = ui4_event;

            }
            else
            {
                pt_sess_obj->ui4_lastest_strm_event = ui4_event;
            }
            break;

        case SM_EVN_SESS_RCRD_GRP_RUTI:
            if (pt_sess_obj->ui4_lastest_rutil_event == ui4_event)
            {
                return FALSE;
            }
            pt_sess_obj->ui4_lastest_rutil_event = ui4_event;
            break;

        case SM_EVN_SESS_RCRD_GRP_APP:
        default:
            /* Ignore un-handled events */
            return FALSE;
    }
    
    /* TIME_MEASUREMENT */
    #ifdef TIME_MEASUREMENT
    _sm_rcrd_sess_drv_time_msrt(ui4_event);
    #endif 
    
    return TRUE;
}


/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_sess_scdb_nfy_fct
 *
 * Description: This function is scdb notify function,Send a message to record 
 *              session handler to check if relates to itself
 *
 * Inputs:  
 *       h_scdb:   SCDB handle
 *       e_cond:   SCDB condition
 *       e_cond:   condition change reason
 *       pv_tag:   notify tag
 *       ui4_data: additional data
 *
 * Outputs: NULL
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
static VOID _sm_rcrd_sess_scdb_nfy_fct(HANDLE_T        h_scdb,
                                       SCDB_COND_T     e_cond,
                                       UINT32          ui4_reason,
                                       VOID*           pv_tag,
                                       UINT32          ui4_data)
{
    SM_SESS_RCRD_MSG_T     t_msg;
    HANDLE_T               h_sess = (HANDLE_T)pv_tag;
    
    t_msg.e_msg_type = SM_SESS_RCRD_MSG_TYPE_SCDB;
    t_msg.h_sess = h_sess;
    t_msg.u.t_rcrd_scdb_msg.e_scdb_cond = e_cond;
    t_msg.u.t_rcrd_scdb_msg.ui4_reason = ui4_reason;
    t_msg.u.t_rcrd_scdb_msg.ui4_data = ui4_data;
        
    sm_rcrd_sess_send_msg(&t_msg);
}

/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_sess_stream_play
 *
 * Description: This function plays a stream
 *
 * Inputs:  
 *       h_stream:    stream handle
 *       pt_sess_obj: session object
 *
 * Outputs: NULL
 *
 * Returns: 
 *        SMR_OK: Succeed
 *        Others: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
static INT32 _sm_rcrd_sess_stream_play(HANDLE_T           h_stream,
                                       SM_SESS_RCRD_T*    pt_sess_obj)
{
    SM_STREAM_T*    pt_strm_obj;
    SM_HANDLER_T*   pt_hdlr_obj;
    INT32           i4_ret;
    
    /* get stream object */
    pt_hdlr_obj = (SM_HANDLER_T*)sm_get_strm_hdlr_obj(h_stream, 
                                                      (VOID**)&pt_strm_obj);
    if ((NULL == pt_hdlr_obj) || (NULL == pt_strm_obj))
    {
        SM_RSS_DBG_ERR(("_sm_rcrd_sess_stream_play:invalid arguments!\r\n"));
        return SMR_INTERNAL_ERROR;
    }
    
    /* assume stream already closed, if lock is fail */
    SM_LOCK_RETURN_ON_FAIL(pt_strm_obj->h_lock, HASH_OK)
    
    i4_ret = pt_hdlr_obj->t_fct_tbl.pf_set(h_stream,
                                           SM_SESS_SET_TYPE_PLAY,
                                           (VOID*)pt_sess_obj->t_base.i4_speed,
                                            sizeof(pt_sess_obj->t_base.i4_speed));
    SM_UNLOCK(pt_strm_obj->h_lock)

    /* play should always be async */
    if ((i4_ret != SMR_OK) 
         && (i4_ret != SMR_ASYNC_NFY) 
         && (i4_ret != SMR_PENDING))  
    {
        SM_RSS_DBG_ERR(("_sm_rcrd_sess_stream_play:stream set err=%d!\r\n", 
                        i4_ret));
        return i4_ret;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_sess_stream_stop
 *
 * Description: This function stops a stream
 *
 * Inputs:  
 *       h_stream:    stream handle
 *
 * Outputs: NULL
 *
 * Returns: 
 *        SMR_OK: Succeed
 *        Others: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
static INT32 _sm_rcrd_sess_stream_stop(HANDLE_T   h_stream)
{
    SM_STREAM_T*    pt_strm_obj;
    SM_HANDLER_T*   pt_hdlr_obj;
    INT32           i4_ret;
    
    /* get stream object */
    pt_hdlr_obj = (SM_HANDLER_T*)sm_get_strm_hdlr_obj(h_stream, 
                                                      (VOID**)&pt_strm_obj);
    if ((NULL == pt_hdlr_obj) || (NULL == pt_strm_obj))
    {
        SM_RSS_DBG_ERR(("_sm_rcrd_sess_stream_stop:invalid arguments!\r\n"));
        return SMR_INTERNAL_ERROR;
    }
    
    /* assume stream already closed, if lock is fail */
    SM_LOCK_RETURN_ON_FAIL(pt_strm_obj->h_lock, HASH_OK)
    
    i4_ret = pt_hdlr_obj->t_fct_tbl.pf_set(h_stream,
                                           SM_SESS_SET_TYPE_STOP,
                                           (VOID*)NULL,
                                           0);
    SM_UNLOCK(pt_strm_obj->h_lock)

    if ((i4_ret != SMR_OK)
         && (i4_ret != SMR_ASYNC_NFY) 
         && (i4_ret != SMR_PENDING)) /* play should always be async */
    {
        SM_RSS_DBG_ERR(("_sm_rcrd_sess_stream_stop:stream set err=%d!\r\n", 
                        i4_ret));
        return i4_ret;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_sess_strm_start_hash_cb
 *
 * Description: This function plays all streams in a session
 *
 * Inputs:  
 *       h_hash:     stream ctrl hash table handle
 *       ui4_counter:the element order in a hash table
 *       pv_obj:     element pointer
 *       pv_parse_tag:hash function tag
 *
 * Outputs: NULL
 *
 * Returns: 
 *        SMR_OK: Succeed
 *        Others: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
static INT32 _sm_rcrd_sess_strm_start_hash_cb(HANDLE_T    h_hash,
                                              UINT32      ui4_counter,
                                              VOID*       pv_obj,
                                              VOID*       pv_parse_tag)
{
    HANDLE_T        h_stream = (HANDLE_T)pv_obj;
    SM_SESS_RCRD_T* pt_sess_obj = (SM_SESS_RCRD_T*)pv_parse_tag;
    SM_STREAM_T*    pt_strm_obj;
    SM_HANDLER_T*   pt_hdlr_obj;
    INT32           i4_ret;
    
    /*Check: get stream object */
    pt_hdlr_obj = (SM_HANDLER_T*)sm_get_strm_hdlr_obj(h_stream, 
                                                     (VOID**)&pt_strm_obj);
    if ((NULL == pt_hdlr_obj) || (NULL == pt_strm_obj))
    {
        SM_RSS_DBG_ERR(("_sm_rcrd_sess_strm_start_hash_cb:invalid arguments!\r\n"));
        return HASH_OK;
    }

    /*Play stream*/
    i4_ret = _sm_rcrd_sess_stream_play(h_stream, pt_sess_obj);
    if ( i4_ret != SMR_OK )
    {
        /* error, but try to process other streams */
        SM_RSS_DBG_ERR(("_sm_rcrd_sess_stream_stop:play err=%d!\r\n", i4_ret));
        /*return HASH_OK;*/
    }
    
    return HASH_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_sess_strm_stop_hash_cb
 *
 * Description: This function stops all streams in a session
 *
 * Inputs:  
 *       h_hash:     stream ctrl hash table handle
 *       ui4_counter:the element order in a hash table
 *       pv_obj:     element pointer
 *       pv_parse_tag:hash function tag
 *
 * Outputs: NULL
 *
 * Returns: 
 *        SMR_OK: Succeed
 *        Others: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
static INT32 _sm_rcrd_sess_strm_stop_hash_cb(HANDLE_T   h_hash,
                                             UINT32     ui4_counter,
                                             VOID*      pv_obj,
                                             VOID*      pv_parse_tag)
{
    HANDLE_T        h_stream = (HANDLE_T)pv_obj;
    SM_STREAM_T*    pt_strm_obj;
    SM_HANDLER_T*   pt_hdlr_obj;
    INT32           i4_ret;
    
    /*Check: get stream object */
    pt_hdlr_obj = (SM_HANDLER_T*)sm_get_strm_hdlr_obj(h_stream, 
                                                     (VOID**)&pt_strm_obj);
    if ( (NULL == pt_hdlr_obj) || (NULL == pt_strm_obj))
    {
        SM_RSS_DBG_ERR(("_sm_rcrd_sess_strm_stop_hash_cb:invalid arguments!\r\n"));
        return HASH_OK;
    }
    
    /*Stop stream*/
    i4_ret = _sm_rcrd_sess_stream_stop(h_stream);
    if (i4_ret != SMR_OK)
    {
        /* error, but try to process other streams */
        SM_RSS_DBG_ERR(("_sm_rcrd_sess_strm_stop_hash_cb:stop err=%d!\r\n", i4_ret));
        /*return HASH_OK;*/
    }
    
    return HASH_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_sess_strm_close_hash_cb
 *
 * Description: This function closes all streams in a session
 *
 * Inputs:  
 *       h_hash:     stream ctrl hash table handle
 *       ui4_counter:the element order in a hash table
 *       pv_obj:     element pointer
 *       pv_parse_tag:hash function tag
 *
 * Outputs: NULL
 *
 * Returns: 
 *        SMR_OK: Succeed
 *        Others: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
static INT32 _sm_rcrd_sess_strm_close_hash_cb(HANDLE_T   h_hash,
                                              UINT32     ui4_counter,
                                              VOID*      pv_obj,
                                              VOID*      pv_parse_tag)
{
    HANDLE_T        h_stream = (HANDLE_T)pv_obj;
    SM_STREAM_T*    pt_strm_obj;
    SM_HANDLER_T*   pt_hdlr_obj;
    INT32           i4_ret;
    
    /* get stream object */
    pt_hdlr_obj = (SM_HANDLER_T*)sm_get_strm_hdlr_obj(h_stream, 
                                                      (VOID**)&pt_strm_obj);
    if ((NULL == pt_hdlr_obj)|| (NULL == pt_strm_obj))
    {
        /* error, but try to process other streams */
        SM_RSS_DBG_ERR(("_sm_rcrd_sess_strm_close_hash_cb:invalid arguments!\r\n"));
        return HASH_OK;
    }
    
    /* assume stream already closed, if lock is fail */
    SM_LOCK_RETURN_ON_FAIL(pt_strm_obj->h_lock, HASH_OK)
    
    i4_ret = pt_hdlr_obj->t_fct_tbl.pf_close(h_stream);

    SM_UNLOCK(pt_strm_obj->h_lock)

    if ((i4_ret != SMR_OK) && (i4_ret != SMR_PENDING))
    {
        /* error, but try to process other streams */
        SM_RSS_DBG_ERR(("_sm_rcrd_sess_strm_close_hash_cb(0x%.8X) close fail!\r\n", 
                        (UINT32)h_stream));
        return HASH_OK;
    }
    
    return HASH_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_sess_state_do_opening
 *
 * Description: This function open a session
 *
 * Inputs:  
 *       ui4_previous_state: previous state
 *       e_event:            event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:     current state
 *       pv_tag1:            jenerally is session obejct pointer
 *       pv_tag2:            jenerally is a NULL
 *
 * Outputs: 
 *       Open some components
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_sess_state_do_opening(UINT32    ui4_previous_state,
                                    UINT32    ui4_event,
                                    UINT32    ui4_curr_state,
                                    VOID*     pv_tag1,
                                    VOID*     pv_tag2)
{
    INT32               i4_ret;
    SM_SESS_RCRD_T*     pt_sess_obj;
    HANDLE_T            h_sess;
    
    SM_ASSERT(pv_tag1);
    
    pt_sess_obj = (SM_SESS_RCRD_T*)pv_tag1;
    h_sess      = pt_sess_obj->t_base.h_sess;

    /*1.Record latest events*/
    _sm_rcrd_sess_state_record_latest_event(pt_sess_obj, ui4_event);
    
    /*2.Open SCDB so that session can perceive any change of SCDB*/
    i4_ret = x_scdb_open(pt_sess_obj->t_base.h_connection,
                         &pt_sess_obj->t_base.h_scdb,
                         (VOID*)h_sess,
                         _sm_rcrd_sess_scdb_nfy_fct);
    if (i4_ret != SCDBR_OK)
    {
        pt_sess_obj->t_base.h_scdb = NULL_HANDLE;
        SM_RSS_DBG_ERR(("sm_rcrd_sess_state_do_opening:scdb(0x%.8X) error=%d!\r\n",
                        (UINT32)h_sess, 
                        i4_ret));
        
        sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_COMP_ERR,
                                       h_sess,
                                       0);
        return SMR_STRM_COMP_ID_NOT_FOUND;
    }
    
    /*3.Open Record Utility*/
    i4_ret = sm_sess_rcrd_rutil_open(h_sess, pt_sess_obj);
    if (i4_ret != SMR_OK)
    {
        pt_sess_obj->t_rutil_info.h_record_util = NULL_HANDLE;
        SM_RSS_DBG_ERR(("sm_rcrd_sess_state_do_opening:Record utility(0x%.8X) error=%d!\r\n",
                       (UINT32)h_sess,
                       i4_ret));
    
        sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_COMP_ERR,
                                       h_sess,
                                       0);
        return SMR_INTERNAL_ERROR;
    }
    
    /*4.Open PM service*/
    #ifdef TIME_MEASUREMENT
    TMS_BEGIN(RCRD_SESS_HANDLE_PM_OPENED);
    #endif
    i4_ret = pm_open_service(pt_sess_obj->t_base.h_connection,   
                             PM_RECORDING,
                             NULL,
                             &pt_sess_obj->t_base.h_pm_service);
    if (i4_ret != PMR_OK)
    {
        pt_sess_obj->t_base.h_pm_service = NULL_HANDLE;
        SM_RSS_DBG_ERR(("sm_rcrd_sess_state_do_opening:pm(0x%.8X) error=%d!\r\n",
                         (UINT32)h_sess, 
                         i4_ret));

        sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_COMP_ERR,
                                       h_sess,
                                       0);
        return SMR_INTERNAL_ERROR;
    }
    sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_PM_COND_OPENED,
                                   h_sess,
                                   0);
   
    /*5.Open PVR Ctrl driver*/
    #ifdef TIME_MEASUREMENT
    TMS_BEGIN(RCRD_SESS_HANDLE_PVR_OPENED);
    #endif
    i4_ret = sm_sess_rcrd_pvr_open(h_sess, pt_sess_obj);
    if (i4_ret != SMR_OK)
    {
        pt_sess_obj->t_pvr_descr.h_comp = NULL_HANDLE;
        SM_RSS_DBG_ERR(("sm_rcrd_sess_state_do_opening:pvr(0x%.8X) error=%d!\r\n",
                        (UINT32)h_sess,
                        i4_ret));
        sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_COMP_ERR,
                                       h_sess,
                                       0);
        return SMR_DRV_COMP_ERROR;
    }

    /*6.After above components are opend
        If 
          6.1 PM condition is stopped  (sync)
          6.2 Record utility condition is stopped (async)
          6.3 When PVR ctrl is opened(async) 
              6.3.1 Init PVR Ctrl, Including:
                    PVR_SET_TYPE_BUFFER_ALIGN,  PVR_SET_TYPE_TICK_PERIOD
                    PVR_SET_TYPE_NFY_FRAME_TYPES, PVR_SET_TYPE_NFY_FCT
              6.3.2 Notify APP that PVR Ctrl is opend so that APP configs 
                    record utility
                    
        Then sessin enters opened state
    */
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_sess_state_do_opened
 *
 * Description: This function indicates session is opened
 *
 * Inputs:  
 *       ui4_previous_state: previous state
 *       e_event:            event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:     current state
 *       pv_tag1:            jenerally is session obejct pointer
 *       pv_tag2:            jenerally is a NULL
 *
 * Outputs: 
 *       Opened some components
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_sess_state_do_opened(UINT32     ui4_previous_state,
                                   UINT32     ui4_event,
                                   UINT32     ui4_curr_state,
                                   VOID*      pv_tag1,
                                   VOID*      pv_tag2)
{
    SM_SESS_RCRD_T*     pt_sess_obj;
	INT32               i4_ret;
    
    SM_ASSERT(pv_tag1);
    
    pt_sess_obj = (SM_SESS_RCRD_T*)pv_tag1;

    /*1.Record event*/
    _sm_rcrd_sess_state_record_latest_event(pt_sess_obj, ui4_event);

    /*2.Reserverd for later*/
    
    /*if stooping-->opened , need flush  pvr buffer remainder data*/
	if (ui4_previous_state == SM_COND_STOPPING)
	{
		
		i4_ret = rec_util_flush_pvr_buf(pt_sess_obj->t_rutil_info.h_record_util);
		if (i4_ret != REC_UTILR_OK)
		{
			SM_RSS_DBG_ERR(("sm_rcrd_sess_state_do_opened:flush pvr buffer data failed(%d)!\r\n",
							i4_ret));
		}
	}
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_sess_state_do_starting
 *
 * Description: This function start a session 
 *
 * Inputs:  
 *       ui4_previous_state: previous state
 *       e_event:            event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:     current state
 *       pv_tag1:            jenerally is session obejct pointer
 *       pv_tag2:            jenerally is a NULL
 *
 * Outputs: 
 *       start some components
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_sess_state_do_starting(UINT32     ui4_previous_state,
                                     UINT32     ui4_event,
                                     UINT32     ui4_curr_state,
                                     VOID*      pv_tag1,
                                     VOID*      pv_tag2)
{
    INT32               i4_ret;
    SM_SESS_RCRD_T*     pt_sess_obj;
    HANDLE_T            h_sess;
    SIZE_T              z_num_strm;
    
    SM_ASSERT(pv_tag1);

    pt_sess_obj = (SM_SESS_RCRD_T*)pv_tag1;
    h_sess = pt_sess_obj->t_base.h_sess;
    
    /*1.Record event*/
    _sm_rcrd_sess_state_record_latest_event(pt_sess_obj, ui4_event);

    /*
      2.Set PVR ctrl flush buffer:
        2.1  PVR_SET_TYPE_FLUSH_BUFFER
        2.2 PVR_GET_TYPE_BUFFER_STATUS (&t_buf_status)
        2.3 rec_util_flush_pvr_buf_status (&t_buf_status)
    */
    i4_ret = sm_sess_rcrd_pvr_ctrl_play_init(pt_sess_obj);
    if (i4_ret != SMR_OK)
    {
        SM_RSS_DBG_ERR(("sm_rcrd_sess_state_do_starting: sess=%u,PVR error=%d!\r\n",
                       (UINT32)h_sess,
                        i4_ret));
        sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_COMP_ERR,
                                       h_sess,
                                       0);
        return SMR_INTERNAL_ERROR;
    }
    
    /*3. start all streams: reference stream start sequence */
    hash_lock(pt_sess_obj->t_base.h_strm_ctrl_tbl);
    hash_get_size(pt_sess_obj->t_base.h_strm_ctrl_tbl, &z_num_strm);
    if (z_num_strm > 0)
    {
        i4_ret = hash_parse(pt_sess_obj->t_base.h_strm_ctrl_tbl,
                            _sm_rcrd_sess_strm_start_hash_cb,
                            (VOID*)pt_sess_obj);
        hash_unlock(pt_sess_obj->t_base.h_strm_ctrl_tbl);
        if (i4_ret != HASH_OK)
        {
            SM_RSS_DBG_ERR(("sm_rcrd_sess_state_do_starting: sess=%u,hash_parse "
                            "error=%d!\r\n",
                           (UINT32)h_sess,
                            i4_ret));
        
            sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_INTERNAL_ERR,
                                           h_sess,
                                           0);
            return SMR_INTERNAL_ERROR;
        }
    
    }
    /*If no stream enable PVR Ctrl*/
    else
    {
        hash_unlock(pt_sess_obj->t_base.h_strm_ctrl_tbl);
        /*Enable PVR Ctrl*/
        i4_ret = sm_sess_rcrd_pvr_ctrl(pt_sess_obj, TRUE);
		if (i4_ret != SMR_OK)
		{
			SM_RSS_DBG_ERR(("sm_rcrd_sess_state_do_starting:sess=%u,Enable PVR error=%d!\r\n",
						   (UINT32)h_sess,
							i4_ret));
			sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_COMP_ERR,
										   h_sess,
										   0);
		}
        sm_state_on_event(&pt_sess_obj->t_state,
                          SM_EVN_SESS_RCRD_PVR_PLAYED,
                          (VOID*)pt_sess_obj,
                          NULL,
                          NULL);
    }

    /* 4.start service */
    TMS_BEGIN(RCRD_SESS_HANDLE_PM_STARTED);
    i4_ret = pm_play_service(pt_sess_obj->t_base.h_pm_service, 
                             pt_sess_obj->t_base.i4_speed);
    if (i4_ret != PMR_OK)
    {
        SM_RSS_DBG_ERR(("sm_rcrd_sess_state_do_starting:pm_play_service(0x%.8X)"
                        "error=%d!\r\n",
                        (UINT32)pt_sess_obj->t_base.h_pm_service, 
                        i4_ret));

        sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_COMP_ERR,
                                       h_sess,
                                       0);
        return SMR_INTERNAL_ERROR;
    }
    sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_PM_COND_PLAYED,
                                   h_sess,
                                   0);

    /*5.If 
           5.1 All streams are opened(async)
           5.2 PM is opened(sync)
        Then enables PVR_CTRL 
        
        Thus session eneters started state
    */
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_sess_state_do_started
 *
 * Description: This function indicates session is started
 *
 * Inputs:  
 *       ui4_previous_state: previous state
 *       e_event:            event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:     current state
 *       pv_tag1:            jenerally is session obejct pointer
 *       pv_tag2:            jenerally is a NULL
 *
 * Outputs: 
 *       started some components
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_sess_state_do_started(UINT32     ui4_previous_state,
                                    UINT32     ui4_event,
                                    UINT32     ui4_curr_state,
                                    VOID*      pv_tag1,
                                    VOID*      pv_tag2)
{
    /*INT32               i4_ret;*/
    SM_SESS_RCRD_T*     pt_sess_obj;
    /*HANDLE_T            h_sess;*/
        
    SM_ASSERT(pv_tag1);
    
    pt_sess_obj = (SM_SESS_RCRD_T*)pv_tag1;
    /*h_sess = pt_sess_obj->t_base.h_sess;*/

    /*1.Record event*/
    _sm_rcrd_sess_state_record_latest_event(pt_sess_obj, ui4_event);

    /*2.Reserved for later*/

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_sess_state_do_stopping
 *
 * Description: This function stops a session 
 *
 * Inputs:  
 *       ui4_previous_state: previous state
 *       e_event:            event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:     current state
 *       pv_tag1:            jenerally is session obejct pointer
 *       pv_tag2:            jenerally is a NULL
 *
 * Outputs: 
 *       stop some components
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_sess_state_do_stopping(UINT32     ui4_previous_state,
                                     UINT32     ui4_event,
                                     UINT32     ui4_curr_state,
                                     VOID*      pv_tag1,
                                     VOID*      pv_tag2)
{
    INT32               i4_ret;
    SM_SESS_RCRD_T*     pt_sess_obj;
    HANDLE_T            h_sess;
    
    SM_ASSERT(pv_tag1);
    
    pt_sess_obj = (SM_SESS_RCRD_T*)pv_tag1;
    h_sess = pt_sess_obj->t_base.h_sess;

    /*1,Record Event*/
    _sm_rcrd_sess_state_record_latest_event(pt_sess_obj, ui4_event);

    /* 2. Disable PVR ctrl */
    i4_ret = sm_sess_rcrd_pvr_ctrl(pt_sess_obj, FALSE);
    if (i4_ret != SMR_OK)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_state_do_stopping:(0x%.8X) stop pvr error=%d!\r\n",
                         (UINT32)pt_sess_obj->t_pvr_descr.h_comp, 
                         i4_ret));

        sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_COMP_ERR,
                                       h_sess,
                                       0);
        return SMR_INTERNAL_ERROR;
    }
    sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_PVR_STOPPED,
                                   h_sess,
                                   0);

    /* 2. Stop service */
    TMS_BEGIN(RCRD_SESS_HANDLE_PM_STOPPED);
    i4_ret = pm_stop_service(pt_sess_obj->t_base.h_pm_service);
    if (i4_ret != PMR_OK)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_state_do_stopping:(0x%.8X) stop pm error=%d!\r\n",
                        (UINT32)pt_sess_obj->t_base.h_pm_service, 
                        i4_ret));
        sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_COMP_ERR,
                                       h_sess,
                                       0);
        return SMR_INTERNAL_ERROR;        
    }
    sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_PM_COND_STOPPED,
                                   h_sess,
                                   0);
    
    /* 3.Stop all streams: reference stream stop sequence*/
    hash_lock(pt_sess_obj->t_base.h_strm_ctrl_tbl);

    i4_ret = hash_parse(pt_sess_obj->t_base.h_strm_ctrl_tbl,
                        _sm_rcrd_sess_strm_stop_hash_cb,
                        (VOID*)pt_sess_obj);

    hash_unlock(pt_sess_obj->t_base.h_strm_ctrl_tbl);

    if (i4_ret != HASH_OK)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_state_do_stopping:(0x%.8x), "
                         "hash_parse error=%d!\r\n",
                         (UINT32)h_sess, 
                         i4_ret));

        sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_INTERNAL_ERR,
                                       h_sess,
                                       0);
        return SMR_INTERNAL_ERROR;        
    }

    /*4.If
          4.1 PVR is disabled (sync)
          4.2 PM is stopped (sync)
          4.3 All stream are stopped (async)
          4.4 Record utility completes writing all PVR Tick Data (async) 
          
        Then session enters opened state

        Notes:  Session should trace PVR buffer flow(PVR will stop automatically)
                if it is true should stops and restarts it automatically and 
                notifies APP buffer overflow.
                1)wait for all tick message are consumed 
                2)and reord utility complete writing
    */
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_sess_state_do_closing
 *
 * Description: This function closes a session 
 *
 * Inputs:  
 *       ui4_previous_state: previous state
 *       e_event:            event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:     current state
 *       pv_tag1:            jenerally is session obejct pointer
 *       pv_tag2:            jenerally is a NULL
 *
 * Outputs: 
 *       close some components
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_sess_state_do_closing(UINT32     ui4_previous_state,
                                    UINT32     ui4_event,
                                    UINT32     ui4_curr_state,
                                    VOID*      pv_tag1,
                                    VOID*      pv_tag2)
{
    INT32               i4_ret;
    SIZE_T              z_num_strm;
    SM_SESS_RCRD_T*     pt_sess_obj;
    HANDLE_T            h_sess;
    
    SM_ASSERT(pv_tag1);
    
    pt_sess_obj = (SM_SESS_RCRD_T*)pv_tag1;
    h_sess      = pt_sess_obj->t_base.h_sess;

    i4_ret      = SMR_OK;

    /*1.Record Event*/
    _sm_rcrd_sess_state_record_latest_event(pt_sess_obj, ui4_event);

    /*2.Close all streams: even come from error state ,do not need stop stream, clsoe them directly */
    hash_lock(pt_sess_obj->t_base.h_strm_ctrl_tbl);
    hash_get_size(pt_sess_obj->t_base.h_strm_ctrl_tbl, &z_num_strm);
    if (z_num_strm > 0)
    {
        i4_ret = hash_parse(pt_sess_obj->t_base.h_strm_ctrl_tbl,
                            _sm_rcrd_sess_strm_close_hash_cb,
                            (VOID*)pt_sess_obj);
    }
    hash_unlock(pt_sess_obj->t_base.h_strm_ctrl_tbl);
    if (i4_ret != HASH_OK)
    {
        SM_RSS_DBG_ERR(("sm_rcrd_sess_state_do_closing:(0x%.8x), hash_parse "
                        "error=return %d!\r\n",
                        (UINT32)h_sess, 
                        i4_ret));
        SM_ASSERT(0);
        
        /* In release version, pretend that all streams are closed. */
        sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_STRM_ALL_CLOSED,
                                       h_sess,
                                       0);
    }
	
    /*
      2.Close PM service, if there is no stream in the session;
      May do it when receive all stream closed messages
    */
    if (0 == z_num_strm)
    {
        if (pt_sess_obj->t_base.h_pm_service != NULL_HANDLE)
        {
            /* Stop PM component if previous state is ERROR, and PM condition
                        is not in STOPPED/CLOSED state */
            if ((SM_SESS_COND_ERROR == ui4_previous_state) 
                 && (pt_sess_obj->ui4_lastest_pm_event != SM_EVN_SESS_RCRD_PM_COND_STOPPED)
                 && (pt_sess_obj->ui4_lastest_pm_event != SM_EVN_SESS_RCRD_PM_COND_CLOSED))
            {
                TMS_BEGIN(SM_EVN_SESS_RCRD_PM_COND_STOPPED);
                i4_ret = pm_stop_service(pt_sess_obj->t_base.h_pm_service);
                if (i4_ret != PMR_OK)
                {
                    SM_RSS_DBG_ERR(("sm_rcrd_sess_state_do_closing:pm_stop_service(0x%.8x)"
                                    "error=%d\r\n",
                                    (UINT32)pt_sess_obj->t_base.h_pm_service, 
                                    i4_ret));
                }
            }
        
            TMS_BEGIN(RCRD_SESS_HANDLE_PM_CLOSED);
            
            i4_ret = pm_close_service(pt_sess_obj->t_base.h_pm_service);
            if ((i4_ret != PMR_OK) && (i4_ret != PMR_INV_HANDLE))
            {
                SM_RSS_DBG_ERR(("sm_rcrd_sess_state_do_closing:close PM fail=%d!\r\n",
                                 i4_ret));            
            }
    
            sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_PM_COND_CLOSED, h_sess, 0);
            /* if pm_close_service() success, service handle is already closed 
               synchronously. */
            /*pt_sess_obj->t_base.h_pm_service = NULL_HANDLE;*/
        }
        else
        {
            /* Pretend that PM Service is closed, if there is no handle.*/
            sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_PM_COND_CLOSED,
                                           h_sess,
                                           0);
        }
    }

    /*3.Close PVR Ctrl, it will close component, if stream number =0 close pvr ctrl else wait for stream closed event */
    if (0 == z_num_strm)
    {
    TMS_BEGIN(RCRD_SESS_HANDLE_PVR_DISCONNECTED);
    i4_ret = sm_sess_rcrd_pvr_close(h_sess, pt_sess_obj);
    if (i4_ret != SMR_OK)
    {
        SM_RSS_DBG_ERR(("sm_rcrd_sess_state_do_closing:PVR disconnect fail=%d!\r\n",
                         i4_ret));
        /* In release version, pretend that PVR is disconnected. */
        sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_PVR_CLOSED,
                                       h_sess,
                                       0);
        /* continue close other components */
        }
    }

    /*4.Close Record Utility*/
    i4_ret = sm_sess_rcrd_rutil_close(h_sess, pt_sess_obj);
    if (i4_ret != SMR_OK)
    {
        /*pt_sess_obj->t_rutil_info.h_record_util = NULL_HANDLE;*/
        SM_RSS_DBG_ERR(("sm_rcrd_sess_state_do_closing:Record utility(0x%.8X)error=%d!\r\n",
                         (UINT32)h_sess,
                         i4_ret));
        if (i4_ret != SMR_NOT_ALLOW)
        {
            sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_RUTI_CLOSED,
                                           h_sess,
                                           0);
        }
    }
    
    /*5.Close SCDB*/
    if (pt_sess_obj->t_base.h_scdb != NULL_HANDLE)
    {
        i4_ret = x_scdb_close(pt_sess_obj->t_base.h_scdb);
        if (i4_ret != SCDBR_OK)
        {
            SM_RSS_DBG_ERR(("sm_rcrd_sess_state_do_closing:close scdb fail=%d!\r\n",
                             i4_ret));            
            /* In release version, pretend that SCDB is closed. */
            sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_SCDB_CLOSED,
                                           h_sess,
                                           0);
        }
        pt_sess_obj->t_base.h_scdb = NULL_HANDLE;
    }
    else
    {
        /* Pretend that SCDB is closed, if there is no SCDB handle. */
        sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_SCDB_CLOSED,
                                       h_sess,
                                       0);
    }

    /*6. If
           6.1 SCDB is closed (sync)
           6.2 Record utility is closed (async)
           6.3 PM is closed (May be async)
           6.4 All stream are closed(async)
           6.5 PVR Ctrl is closed (sync)
         Then session enters closed state
    */
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_sess_state_do_closed
 *
 * Description: This function indicates a session is closed
 *
 * Inputs:  
 *       ui4_previous_state: previous state
 *       e_event:            event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:     current state
 *       pv_tag1:            jenerally is session obejct pointer
 *       pv_tag2:            jenerally is a NULL
 *
 * Outputs: 
 *       closed some components
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_sess_state_do_closed(UINT32     ui4_previous_state,
                                   UINT32     ui4_event,
                                   UINT32     ui4_curr_state,
                                   VOID*      pv_tag1,
                                   VOID*      pv_tag2)
{
    SM_SESS_RCRD_T*     pt_sess_obj;
    
    SM_ASSERT(pv_tag1);
    
    pt_sess_obj = (SM_SESS_RCRD_T*)pv_tag1;

    /*1.Record event*/
    _sm_rcrd_sess_state_record_latest_event(pt_sess_obj, ui4_event);

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_sess_state_do_error
 *
 * Description: This function do session error handling
 *
 * Inputs:  
 *       ui4_previous_state: previous state
 *       e_event:            event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:     current state
 *       pv_tag1:            jenerally is session obejct pointer
 *       pv_tag2:            jenerally is a NULL
 *
 * Outputs: 
 *       handles session error case
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_sess_state_do_error(UINT32     ui4_previous_state,
                                  UINT32     ui4_event,
                                  UINT32     ui4_curr_state,
                                  VOID*      pv_tag1,
                                  VOID*      pv_tag2)
{
    SM_SESS_RCRD_T*     pt_sess_obj;
    
    SM_ASSERT(pv_tag1);
    
    pt_sess_obj = (SM_SESS_RCRD_T*)pv_tag1;

    _sm_rcrd_sess_state_record_latest_event(pt_sess_obj, ui4_event);

    /*Stop write reqs*/
    if (pt_sess_obj->t_rutil_info.ui4_req_total_cnt != 0)
    {
        sm_sess_rcrd_check_rutil_req_status(pt_sess_obj);
        SM_RSS_DBG_ERR(("sm_rcrd_sess_state_do_error:%d write reqs "
                        "are not done!!!\r\n",
                        pt_sess_obj->t_rutil_info.ui4_req_total_cnt));
        //pt_sess_obj->t_rutil_info.ui4_req_total_cnt = 0;
    }

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_sess_state_do_ignore_excpt
 *
 * Description: This function handles exception events in ignore state
 *
 * Inputs:  
 *       e_event:         event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is session obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Just records excepton events in session object
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_sess_state_do_ignore_excpt(UINT32   ui4_event,
                                         UINT32   ui4_curr_state,
                                         VOID*    pv_tag1,
                                         VOID*    pv_tag2)
{
    SM_SESS_RCRD_T*     pt_sess_obj;
    
    SM_ASSERT(pv_tag1);
    
    pt_sess_obj = (SM_SESS_RCRD_T*)pv_tag1;

    _sm_rcrd_sess_state_record_latest_event(pt_sess_obj, ui4_event);

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_sess_state_do_opening_excpt
 *
 * Description: This function handles exception events in opening state
 *
 * Inputs:  
 *       e_event:         event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is session obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Just records excepton events in session object
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_sess_state_do_opening_excpt(UINT32   ui4_event,
                                          UINT32   ui4_curr_state,
                                          VOID*    pv_tag1,
                                          VOID*    pv_tag2)
{
    SM_SESS_RCRD_T*     pt_sess_obj;
    
    SM_ASSERT(pv_tag1);
    
    pt_sess_obj = (SM_SESS_RCRD_T*)pv_tag1;

    _sm_rcrd_sess_state_record_latest_event(pt_sess_obj, ui4_event);

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_sess_state_do_ignore_opened_excpt
 *
 * Description: This function handles exception events in opened state
 *
 * Inputs:  
 *       e_event:         event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is session obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Just records excepton events in session object
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_sess_state_do_opened_excpt(UINT32   ui4_event,
                                         UINT32   ui4_curr_state,
                                         VOID*    pv_tag1,
                                         VOID*    pv_tag2)
{
    SM_SESS_RCRD_T*     pt_sess_obj;
    
    SM_ASSERT(pv_tag1);
    
    pt_sess_obj = (SM_SESS_RCRD_T*)pv_tag1;

    _sm_rcrd_sess_state_record_latest_event(pt_sess_obj, ui4_event);

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_sess_state_do_starting_excpt
 *
 * Description: This function handles exception events in starting state
 *
 * Inputs:  
 *       e_event:         event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is session obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Just records excepton events in session object
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_sess_state_do_starting_excpt(UINT32   ui4_event,
                                           UINT32   ui4_curr_state,
                                           VOID*    pv_tag1,
                                           VOID*    pv_tag2)
{
    SM_SESS_RCRD_T*     pt_sess_obj;
    
    SM_ASSERT(pv_tag1);
    
    pt_sess_obj = (SM_SESS_RCRD_T*)pv_tag1;

    _sm_rcrd_sess_state_record_latest_event(pt_sess_obj, ui4_event);

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_rcrd_sess_state_do_started_excpt
 *
 * Description: This function handles exception events in started state
 *
 * Inputs:  
 *       e_event:         event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is session obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Just records excepton events in session object
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_sess_state_do_started_excpt(UINT32   ui4_event,
                                          UINT32   ui4_curr_state,
                                          VOID*    pv_tag1,
                                          VOID*    pv_tag2)
{
    SM_SESS_RCRD_T*     pt_sess_obj;
    
    SM_ASSERT(pv_tag1);
    
    pt_sess_obj = (SM_SESS_RCRD_T*)pv_tag1;

    _sm_rcrd_sess_state_record_latest_event(pt_sess_obj, ui4_event);

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_sess_state_do_stopping_excpt
 *
 * Description: This function handles exception events in stopping state
 *
 * Inputs:  
 *       e_event:         event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is session obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Just records excepton events in session object
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_sess_state_do_stopping_excpt(UINT32   ui4_event,
                                           UINT32   ui4_curr_state,
                                           VOID*    pv_tag1,
                                           VOID*    pv_tag2)
{
    SM_SESS_RCRD_T*     pt_sess_obj;
    
    SM_ASSERT(pv_tag1);
    
    pt_sess_obj = (SM_SESS_RCRD_T*)pv_tag1;

    _sm_rcrd_sess_state_record_latest_event(pt_sess_obj, ui4_event);

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_sess_state_do_closing_excpt
 *
 * Description: This function handles exception events in closing state
 *
 * Inputs:  
 *       e_event:         event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is session obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Just records excepton events in session object
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_sess_state_do_closing_excpt(UINT32   ui4_event,
                                          UINT32   ui4_curr_state,
                                          VOID*    pv_tag1,
                                          VOID*    pv_tag2)
{
    SM_SESS_RCRD_T*     pt_sess_obj;
    
    SM_ASSERT(pv_tag1);
    
    pt_sess_obj = (SM_SESS_RCRD_T*)pv_tag1;

    _sm_rcrd_sess_state_record_latest_event(pt_sess_obj, ui4_event);

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_sess_state_do_closed_excpt
 *
 * Description: This function handles exception events in closed state
 *
 * Inputs:  
 *       e_event:         event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is session obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Just records excepton events in session object
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_sess_state_do_closed_excpt(UINT32   ui4_event,
                                         UINT32   ui4_curr_state,
                                         VOID*    pv_tag1,
                                         VOID*    pv_tag2)
{
    SM_SESS_RCRD_T*     pt_sess_obj;
    
    SM_ASSERT(pv_tag1);
    
    pt_sess_obj = (SM_SESS_RCRD_T*)pv_tag1;

    _sm_rcrd_sess_state_record_latest_event(pt_sess_obj, ui4_event);

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_sess_state_do_error_excpt
 *
 * Description: This function handles exception events in error state
 *
 * Inputs:  
 *       e_event:         event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is session obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Just records excepton events in session object
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-09 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_sess_state_do_error_excpt(UINT32   ui4_event,
                                        UINT32   ui4_curr_state,
                                        VOID*    pv_tag1,
                                        VOID*    pv_tag2)
{
    SM_SESS_RCRD_T*     pt_sess_obj;
    
    SM_ASSERT(pv_tag1);
    
    pt_sess_obj = (SM_SESS_RCRD_T*)pv_tag1;

    _sm_rcrd_sess_state_record_latest_event(pt_sess_obj, ui4_event);

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_sess_state_notify_app
 *
 * Description: This function notifies session information to application
 *
 * Inputs:  
 *       pv_tag :  record session handle and sessin object pointer
 *       e_cond :  session current status
 *       ui4_event:current event 
 *       ui4_data :additional info e.g reason information
 *
 * Outputs: NULL  
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-06 : initial
 ------------------------------------------------------------------------------*/
extern INT32 Printf(const CHAR *format, ...);
VOID sm_rcrd_sess_state_notify_app(VOID*             pv_tag,
                                   SM_SESS_COND_T    e_cond,
                                   UINT32            ui4_event,
                                   UINT32            ui4_data)
{
    INT32                      i4_ret;
    SM_RCRD_SESS_STATE_TAG_T*  pt_state_tag;
    SM_SESS_RCRD_T*            pt_sess_obj = NULL;
    HANDLE_T                   h_sess;


    /*Assert check*/
    pt_state_tag = (SM_RCRD_SESS_STATE_TAG_T*)pv_tag;
    pt_sess_obj = NULL;
    SM_ASSERT(pt_state_tag);
    SM_ASSERT(pt_state_tag->h_sess != NULL_HANDLE);
    SM_ASSERT(pt_state_tag->pt_sess_obj);


    /*Get session handle and object pointer*/
    h_sess = (HANDLE_T)pt_state_tag->h_sess;
    pt_sess_obj = (SM_SESS_RCRD_T*)pt_state_tag->pt_sess_obj;
    /*Update session condition state*/
    if (pt_sess_obj->t_base.e_sess_cond != e_cond)
    {
        Printf("{SM_RCRD_SESS}:SESS(0x%08X), %s, EVN(0x%08X), DATA(0x%08X)\r\n",
              (UINT32)h_sess,
               sm_sess_cond_2_str(e_cond),
              (UINT32)ui4_event,
              (UINT32)ui4_data);
    }
    pt_sess_obj->t_base.e_sess_cond = e_cond;


    /*Time Measure */
    #ifdef TIME_MEASUREMENT
    _sm_rcrd_sess_state_time_msrt((UINT32)pt_sess_obj->t_base.e_sess_cond,
                                  (UINT32)e_cond,
                                  ui4_event);
    #endif 

    
    /* Notify Session event to application */
    i4_ret = sm_sess_nfy(h_sess, 
                         e_cond, 
                         pt_sess_obj->t_base.pv_sess_nfy_tag, 
                         ui4_event,
                         ui4_data );
    if (i4_ret != SMR_OK)
    {
        SM_RSS_DBG_ERR(( 
                        "sm_rcrd_sess_state_notify_app:error, SESS(0x%.8X), COND(0x%.8X), "
                        "EVN(0x%.8X), DATA(0x%.8X), return = %d\r\n",
                        (UINT32)h_sess,
                        (UINT32)e_cond,
                        (UINT32)ui4_event,
                        (UINT32)ui4_data,
                        (UINT32)i4_ret));
        return;
    }


    /*Debug info*/
    #ifdef DEBUG
    {
        const CHAR* ps_state = sm_state_event_to_str(ui4_event);
        
        if (ps_state != NULL)
        {
            SM_RSS_DBG_INFO(("sm_rcrd_sess_state_notify_app:"
                             "SESS(0x%.8X), %s, %s, DATA(0x%.8X)\r\n",
                             (UINT32)h_sess,
                             sm_sess_cond_2_str(e_cond),
                             ps_state,
                             (UINT32)ui4_data));
        }
        else
        {
            SM_RSS_DBG_INFO(("sm_rcrd_sess_state_notify_app"
                             "SESS(0x%.8X), %s, EVN(0x%.8X), DATA(0x%.8X)\r\n",
                             (UINT32)h_sess,
                              sm_sess_cond_2_str(e_cond),
                             (UINT32)ui4_event,
                             (UINT32)ui4_data));
        }
    }
    #endif 
}



/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_sess_state_do_pvr_event
 *
 * Description: Handles PVR events which are generated by PVR-RM notify messages 
 *              processing function
 *
 * Inputs:  
 *       h_sess:         session handle
 *       pth_sess_rs_obj:record session pointer 
 *       e_event:        PVR event
 *
 * Outputs: NULL
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
static INT32 _sm_rcrd_sess_state_do_pvr_event(HANDLE_T          h_sess,
                                              SM_SESS_RCRD_T*   pt_sess_obj,
                                              UINT32            ui4_event)
{
    INT32                 i4_ret = SMR_OK;

    switch (ui4_event)
    {
        /*If Opened:*/    
        case SM_EVN_SESS_RCRD_PVR_OPENED:
            /*1. Init PVR Ctrl, Including:
                          PVR_SET_TYPE_BUFFER_ALIGN,  PVR_SET_TYPE_TICK_PERIOD
                          PVR_SET_TYPE_NFY_FRAME_TYPES, PVR_SET_TYPE_NFY_FCT
                    */
            
            /*2. Notify APP that PVR Ctrl is opend so that APP configs record utility*/
            sm_rcrd_sess_state_notify_app(pt_sess_obj->t_state.pv_state_nfy_tag,
                                          pt_sess_obj->t_state.ui4_curr_state,
                                          SM_EVN_SESS_RCRD_PVR_CTRL_OPENED,
                                          0);
            SM_RSS_DBG_INFO(("_sm_rcrd_sess_state_do_pvr_event:PVR Ctrl is opened!\r\n"));
            
            break;

        case SM_EVN_SESS_RCRD_PVR_PLAYED:
            SM_RSS_DBG_INFO(("_sm_rcrd_sess_state_do_pvr_event:PVR Ctrl is started!\r\n"));
            break;

        case SM_EVN_SESS_RCRD_PVR_STOPPED:
            SM_RSS_DBG_INFO(("_sm_rcrd_sess_state_do_pvr_event:PVR Ctrl is stopped!\r\n"));
            break;

        case SM_EVN_SESS_RCRD_PVR_CLOSED:
            SM_RSS_DBG_INFO(("_sm_rcrd_sess_state_do_pvr_event:PVR Ctrl is closed!\r\n"));
			pt_sess_obj->t_pvr_descr.h_comp = NULL_HANDLE;
            break;
			
        case SM_EVN_SESS_RCRD_PVR_OVERFLOW:
            /*1.stop session if skip tick and if writen down need restart session*/
            SM_RSS_DBG_ERR(("_sm_rcrd_sess_state_do_pvr_event:PVR Ctrl overflow!\r\n"));
            if (SM_SESS_RCRD_OVERFLOW_OPT_SKIP_TICK == pt_sess_obj->t_sess_cfg.e_pvr_overflow_opt)
            {
                sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_PVR_CTRL_OVERFLOW,
                                               h_sess,
                                               0);
                SM_RSS_DBG_ERR(("_sm_rcrd_sess_state_do_pvr_event:PVR Ctrl should "
                                "stop then restart again!\r\n"));
            }
            /*Permit send out empty tick, if writen down need flush pvr*/
            else
            {
                extern INT32 Printf (const CHAR *format, ...);
                SM_RSS_DBG_ERR(("_sm_rcrd_sess_state_do_pvr_event:PVR Ctrl should "
                                "be flushed after tick data is consumed!\r\n"));

                SM_RSS_DBG_INFO(("\r\nPVR overflow:ui4_lastest_rutil_event = %u\r\n",
                                 pt_sess_obj->ui4_lastest_rutil_event));
                
                if ((SM_EVN_SESS_RCRD_RUTI_READY == pt_sess_obj->ui4_lastest_rutil_event)
                     || (SM_EVN_SESS_RCRD_RUTI_STOPPED == pt_sess_obj->ui4_lastest_rutil_event))
                {
                    i4_ret = sm_sess_rcrd_pvr_ctrl_play_init(pt_sess_obj);
                    if (i4_ret != SMR_OK)
                    {
                        SM_RSS_DBG_ERR(("_sm_rcrd_sess_state_do_pvr_event:flush err=%d\r\n",
                                        i4_ret));
						sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_PVR_ERR,
													   h_sess,
													   0);
                        return i4_ret;
                    }
                    
                    /* this is extremely important message, so must dump it even in release version */
                    Printf ("\r\nPVR flushed after overflow.\r\n");
                }
                else
                {
                    pt_sess_obj->b_pvr_overflow = TRUE;
                }
            }
            
            /*2.If PVR over flow we should restart session by pending 
              SM_EVN_SESS_APP_PLAY_REQ*/
            break;
        case SM_EVN_SESS_RCRD_PVR_FLUSHED:
            sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_PVR_CTRL_FLUSHED,
                                           h_sess,
                                           0);
            break;
        case SM_EVN_SESS_RCRD_PVR_ERR:
            SM_RSS_DBG_ERR(("_sm_rcrd_sess_state_do_pvr_event:PVR Ctrl exists error!\r\n"));
        default:
            return SMR_DRV_COMP_ERROR;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_sess_state_do_pm_event
 *
 * Description: Handles PM events which are generated by PM notify messages 
 *              processing function
 *
 * Inputs:  
 *       h_sess:         session handle
 *       pth_sess_rs_obj:record session pointer 
 *       e_event:        PM event
 *
 * Outputs: NULL
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
static INT32 _sm_rcrd_sess_state_do_pm_event(HANDLE_T          h_sess,
                                             SM_SESS_RCRD_T*   pt_sess_obj,
                                             UINT32            ui4_event)
{
    INT32     i4_ret;


    i4_ret = SMR_OK;
    
    switch (ui4_event)
    {
        /*If Opened:*/    
        case SM_EVN_SESS_RCRD_PM_COND_OPENED:
            break;

        case SM_EVN_SESS_RCRD_PM_COND_PLAYED:
            break;

        case SM_EVN_SESS_RCRD_PM_COND_STOPPED:
            break;

        case SM_EVN_SESS_RCRD_PM_COND_CLOSED:
			pt_sess_obj->t_base.h_pm_service = NULL_HANDLE;
            break;
        case SM_EVN_SESS_RCRD_PM_COND_ERR:
        default:
            return SMR_DRV_COMP_ERROR;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_sess_state_do_rutil_event
 *
 * Description: Handles Rutil events which are generated by Rutil notify messages 
 *              processing function
 *
 * Inputs:  
 *       h_sess:         session handle
 *       pth_sess_rs_obj:record session pointer 
 *       e_event:        record utility event
 *
 * Outputs: NULL
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
static INT32 _sm_rcrd_sess_state_do_rutil_event(HANDLE_T          h_sess,
                                                SM_SESS_RCRD_T*   pt_sess_obj,
                                                UINT32            ui4_event)
{
    INT32        i4_ret;

    i4_ret = SMR_OK;
    
    switch (ui4_event)
    {
        /*If Opened:*/    
        case SM_EVN_SESS_RCRD_RUTI_OPENED:
            break;
        case SM_EVN_SESS_RCRD_RUTI_READY:
            i4_ret = sm_sess_rcrd_pvr_ctrl_open_init(pt_sess_obj);
            if (i4_ret != SMR_OK)
            {
                SM_RSS_DBG_ERR(("_sm_rcrd_sess_state_do_pvr_event:PVR init err=%d\r\n",
                                i4_ret));
                return i4_ret;
            }
            break;
        case SM_EVN_SESS_RCRD_RUTI_TRANSITION:
            break;
        case SM_EVN_SESS_RCRD_RUTI_PLAYED:
            break;

        case SM_EVN_SESS_RCRD_RUTI_STOPPED:
            if (TRUE == pt_sess_obj->b_pvr_overflow)
            {
                extern INT32 Printf (const CHAR *format, ...);
                
                pt_sess_obj->b_pvr_overflow = FALSE;
                i4_ret = sm_sess_rcrd_pvr_ctrl_play_init(pt_sess_obj);
                if (i4_ret != SMR_OK)
                {
                    SM_RSS_DBG_ERR(("_sm_rcrd_sess_state_do_rutil_event:flush err=%d\r\n",
                                    i4_ret));
					sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_PVR_ERR,
												   h_sess,
												   0);
                    return i4_ret;
                }
                
                /* this is extremely important message, so must dump it even in release version */
                Printf ("\r\nPVR flushed after overflow.\r\n");
            }
            break;

        case SM_EVN_SESS_RCRD_RUTI_CLOSED:
            pt_sess_obj->t_rutil_info.h_record_util = NULL_HANDLE;
            break;
            
        case SM_EVN_SESS_RCRD_RUTI_REACH_BOUND:
            sm_rcrd_sess_state_notify_app(pt_sess_obj->t_state.pv_state_nfy_tag,
                                          pt_sess_obj->t_state.ui4_curr_state,
                                          SM_EVN_SESS_RCRD_REACH_BOUND,
                                          0);
            SM_RSS_DBG_INFO(("_sm_rcrd_sess_state_do_rutil_event:reach bound!\r\n"));
            break;

        case SM_EVN_SESS_RCRD_RUTI_STRG_FULL:
            sm_rcrd_sess_state_notify_app(pt_sess_obj->t_state.pv_state_nfy_tag,
                                          pt_sess_obj->t_state.ui4_curr_state,
                                          SM_EVN_SESS_RCRD_STRG_FULL,
                                          0);
            SM_RSS_DBG_INFO(("_sm_rcrd_sess_state_do_rutil_event:strg full!\r\n"));
            break;
            
        case SM_EVN_SESS_RCRD_RUTI_ERR:
        default:
            return SMR_DRV_COMP_ERROR;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_sess_state_do_strm_event
 *
 * Description: This function handles event from stream handler.
 *
 * Inputs:  
 *       h_sess:       session handle
 *       pt_sess_obj:  session obejct
 *       ui4_event:    stream state
 *       ui4_evn_data: additional data
 *
 * Outputs: NULL
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-17 : initial
 ------------------------------------------------------------------------------*/
static INT32 _sm_rcrd_sess_state_do_strm_event(HANDLE_T         h_sess,
                                               SM_SESS_RCRD_T*  pt_sess_obj,
                                               UINT32           ui4_event,
                                               UINT32           ui4_evn_data)
{
    INT32        i4_ret;
    HANDLE_T     h_stream = (HANDLE_T)ui4_evn_data;

    i4_ret = SMR_OK;
    
    if ((SM_EVN_SESS_RCRD_STRM_ALL_CLOSED == ui4_event)
         && (SM_SESS_COND_CLOSING == pt_sess_obj->t_state.ui4_curr_state))
    {
        if (pt_sess_obj->t_base.h_pm_service != NULL_HANDLE)
        {
            /*1. should  stop PM service*/
            if ((pt_sess_obj->ui4_lastest_pm_event != SM_EVN_SESS_RCRD_PM_COND_STOPPED)
                 && (pt_sess_obj->ui4_lastest_pm_event != SM_EVN_SESS_RCRD_PM_COND_CLOSED))
            {
                TMS_BEGIN(SM_EVN_SESS_RCRD_PM_COND_STOPPED);
                i4_ret = pm_stop_service(pt_sess_obj->t_base.h_pm_service);
                if (i4_ret != PMR_OK)
                {
                    SM_RSS_DBG_ERR(("_sm_rcrd_sess_state_do_strm_event:pm_stop_service(0x%.8x)"
                                    "error=%d\r\n",
                                    (UINT32)pt_sess_obj->t_base.h_pm_service, 
                                    i4_ret));
                }
            }

            /*close PM service*/
            i4_ret = pm_close_service(pt_sess_obj->t_base.h_pm_service);
            if ((i4_ret != PMR_OK) && (i4_ret != PMR_INV_HANDLE))
            {
                SM_RSS_DBG_ERR(("_sm_rcrd_sess_state_do_strm_event:fail,return %d!\r\n",
                                i4_ret));
            }
            
            sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_PM_COND_CLOSED,
                                           h_sess,
                                           0);
            
            /* if pm_close_service() success, service handle is already closed 
               synchronously. */
            /*pt_sess_obj->t_base.h_pm_service = NULL_HANDLE;*/

            /*2. close pvr ctrl*/
            i4_ret = sm_sess_rcrd_pvr_close(h_sess, pt_sess_obj);
            if (i4_ret != SMR_OK)
            {
                SM_RSS_DBG_ERR(("_sm_rcrd_sess_state_do_strm_event:PVR disconnect fail=%d!\r\n",
                                 i4_ret));
                /* In release version, pretend that PVR is disconnected. */
                sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_PVR_CLOSED, h_sess, 0);
                /* continue close other components */
            }
        }
        else
        {
            /* Pretend that PM Service is closed, if there is no handle. */
            sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_PM_COND_CLOSED,
                                              h_sess,
                                              0);
        }
    }
    else if((SM_EVN_SESS_RCRD_STRM_ALL_PLAYED == ui4_event)
         && (SM_SESS_COND_STARTING == pt_sess_obj->t_state.ui4_curr_state))
    {
        /*Enable PVR Ctrl*/
        i4_ret = sm_sess_rcrd_pvr_ctrl(pt_sess_obj, TRUE);
		if (i4_ret != SMR_OK)
		{
			SM_RSS_DBG_ERR(("_sm_rcrd_sess_state_do_strm_event:sess=%u,Enable PVR error=%d!\r\n",
						   (UINT32)h_sess,
							i4_ret));
			sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_COMP_ERR,
										   h_sess,
										   0);
			return i4_ret;
		}
        sm_state_on_event(&pt_sess_obj->t_state,
                          SM_EVN_SESS_RCRD_PVR_PLAYED,
                          (VOID*)pt_sess_obj,
                          NULL,
                          NULL);
    }
    else if (SM_EVN_SESS_RCRD_STRM_OPENED == ui4_event)
    {
        if (SM_SESS_COND_STARTING == pt_sess_obj->t_state.ui4_curr_state)
        {
            /* play the stream */
            i4_ret = _sm_rcrd_sess_stream_play(h_stream, pt_sess_obj);
        }
        else if (SM_SESS_COND_CLOSING == pt_sess_obj->t_state.ui4_curr_state)
        {
            /* close the stream */
            i4_ret = sm_sess_rcrd_close(h_stream);
        }
        else
        {
            
        }
    }
    else if (SM_EVN_SESS_RCRD_STRM_PLAYED == ui4_event)
    {
        if (SM_SESS_COND_STOPPING == pt_sess_obj->t_state.ui4_curr_state)
        {
            /* stop the stream */
            i4_ret = _sm_rcrd_sess_stream_stop(h_stream);
        }
        else if (SM_SESS_COND_CLOSING == pt_sess_obj->t_state.ui4_curr_state)
        {
            /* close the stream */
            i4_ret = sm_sess_rcrd_close(h_stream);
        }
        else
        {
            
        }
    }
    else
    {
        
    }
    
    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_sess_state_do_drv_events
 *
 * Description: This function collects all events from SCDB/RUtil/PVR/PM/etc.
 *
 * Inputs:  
 *       ui4_previous_state: previous state
 *       e_event:            event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:     current state
 *       pv_tag1:            jenerally is session obejct pointer
 *       pv_tag2:            jenerally is a NULL
 *
 * Outputs: 
 *       Send event to transfer state
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-17 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_sess_state_do_drv_events(UINT32     ui4_curr_state,
                                       UINT32     ui4_event,
                                       UINT32     ui4_next_state,
                                       VOID*      pv_tag1,
                                       VOID*      pv_tag2)
{
    INT32               i4_ret;
    SM_SESS_RCRD_T*     pt_sess_obj;
    HANDLE_T            h_sess;
    
    SM_ASSERT(pv_tag1);
    
    pt_sess_obj = (SM_SESS_RCRD_T*)pv_tag1;
    h_sess      = pt_sess_obj->t_base.h_sess;
    
    /*1.Focus on events which may cause state transfer and 
        Skip the event if there is no change*/
    if (!_sm_rcrd_sess_state_record_latest_event(pt_sess_obj, ui4_event))
    {
        return SMR_OK;
    }

    i4_ret = SMR_OK;
    
    /*2.Every group handles respective events*/
    switch (SM_EVN_GET_GRP(ui4_event))
    {
        case SM_EVN_SESS_RCRD_GRP_PVR:
        i4_ret = _sm_rcrd_sess_state_do_pvr_event(h_sess, pt_sess_obj, ui4_event);
        break;

        case SM_EVN_SESS_RCRD_GRP_PM:
        i4_ret = _sm_rcrd_sess_state_do_pm_event(h_sess, pt_sess_obj, ui4_event);
        break;

        case SM_EVN_SESS_RCRD_GRP_STRM:
        i4_ret = _sm_rcrd_sess_state_do_strm_event(h_sess, 
                                                   pt_sess_obj, 
                                                   ui4_event, 
                                                   (UINT32)pv_tag2);
        break;

        case SM_EVN_SESS_RCRD_GRP_RUTI:
        i4_ret = _sm_rcrd_sess_state_do_rutil_event(h_sess, pt_sess_obj, ui4_event);
        break;

        default:
        SM_RSS_DBG_ERR(("[SM] sm_rcrd_sess_state_do_drv_events;receives invalid "
                        "event 0x%.8X\r\n",
                        ui4_event));
        SM_ABORT(SM_DBG_ABRT_INV_EVENT);
    }
    
    if (i4_ret != SMR_OK)
    {
        sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_COMP_ERR,
                                       h_sess,
                                       0);
        return i4_ret;
    }


    /*******************3.Collect events to send transfer events*****************/
    /*3.1 Enter Opened which is ready for playback: cause by Open opration*/
    if ((SM_EVN_SESS_RCRD_PM_COND_OPENED == pt_sess_obj->ui4_lastest_pm_event)
         && (SM_EVN_SESS_RCRD_RUTI_READY == pt_sess_obj->ui4_lastest_rutil_event)
         && ((SM_EVN_IGNORE == pt_sess_obj->ui4_lastest_all_strm_event)
              || (SM_EVN_SESS_RCRD_STRM_ALL_OPENED == pt_sess_obj->ui4_lastest_all_strm_event)
              || (SM_EVN_SESS_RCRD_STRM_ALL_CLOSED == pt_sess_obj->ui4_lastest_all_strm_event))
         && (SM_EVN_SESS_RCRD_PVR_OPENED == pt_sess_obj->ui4_lastest_pvr_event))
    {
        /*Enter Opened by open opt*/
        if (SM_SESS_COND_OPENING == pt_sess_obj->t_state.ui4_curr_state)
        {
            sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_ALL_OPENED,
                                           h_sess,
                                           0);
        }
        /*Enter Opened by stop opt*/
        else
        {
            /*Debug error*/
        }
    }
    /*Enter Started: Caused by play opt*/
    else if ((SM_EVN_SESS_RCRD_PM_COND_PLAYED == pt_sess_obj->ui4_lastest_pm_event)
             && ((SM_EVN_SESS_RCRD_RUTI_READY == pt_sess_obj->ui4_lastest_rutil_event)
                  || (SM_EVN_SESS_RCRD_RUTI_STOPPED == pt_sess_obj->ui4_lastest_rutil_event))
             && ((SM_EVN_IGNORE == pt_sess_obj->ui4_lastest_all_strm_event)
                  || (SM_EVN_SESS_RCRD_STRM_ALL_PLAYED == pt_sess_obj->ui4_lastest_all_strm_event)
                  || (SM_EVN_SESS_RCRD_STRM_ALL_CLOSED == pt_sess_obj->ui4_lastest_all_strm_event))
             &&  (SM_EVN_SESS_RCRD_PVR_PLAYED == pt_sess_obj->ui4_lastest_pvr_event))
    {
        sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_ALL_PLAYED,
                                       h_sess,
                                       0);
    }
    /*Enter Stoped(Opened): Caused by stop opt*/
    else if ((SM_EVN_SESS_RCRD_PM_COND_STOPPED == pt_sess_obj->ui4_lastest_pm_event)
              && ((SM_EVN_SESS_RCRD_RUTI_READY == pt_sess_obj->ui4_lastest_rutil_event)
                  || (SM_EVN_SESS_RCRD_RUTI_STOPPED == pt_sess_obj->ui4_lastest_rutil_event))
              && ((SM_EVN_IGNORE == pt_sess_obj->ui4_lastest_all_strm_event)
                   || (SM_EVN_SESS_RCRD_STRM_ALL_OPENED == pt_sess_obj->ui4_lastest_all_strm_event)
                   || (SM_EVN_SESS_RCRD_STRM_ALL_CLOSED == pt_sess_obj->ui4_lastest_all_strm_event))
              && (SM_EVN_SESS_RCRD_PVR_STOPPED == pt_sess_obj->ui4_lastest_pvr_event))
    {
        sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_ALL_STOPPED,
                                       h_sess,
                                       0);
        
        
    }
    /*Enter Closed: Caused by close opt*/
    else if ((SM_EVN_SESS_RCRD_PM_COND_CLOSED == pt_sess_obj->ui4_lastest_pm_event)
         && (SM_EVN_SESS_RCRD_RUTI_CLOSED == pt_sess_obj->ui4_lastest_rutil_event)
         && ((SM_EVN_IGNORE == pt_sess_obj->ui4_lastest_all_strm_event)
              || (SM_EVN_SESS_RCRD_STRM_ALL_CLOSED == pt_sess_obj->ui4_lastest_all_strm_event))
         && (SM_EVN_SESS_RCRD_PVR_CLOSED == pt_sess_obj->ui4_lastest_pvr_event))
    {
        sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_ALL_CLOSED,
                                       h_sess,
                                       0);
    }
    else
    {
        /*Nothing*/
    }
    
    return SMR_OK;
}


