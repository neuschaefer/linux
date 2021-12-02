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
 * $RCSfile: crnt_evn.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/26 $
 * $SWAuthor: Weider Chang $
 * $MD5HEX: 9cacb426bc6af60b7f3e5a1ad05af25e $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "svctx/util/crnt_evn.h"
#include "svctx/svctx.h"
#include "svctx/svctx_dbg.h"
#include "svctx/svctx_cli.h"
#include "handle/handle.h"
#include "dt/x_dt.h"

#ifdef MW_EDB_SUPPORT
#include "edb/x_edb.h"
#endif

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#define _CRNT_EVN_MSG_EVCTX_NTFY            ((UINT8)210)
#define _CRNT_EVN_MSG_REQUERY_TIMEOUT_NTFY  ((UINT8)211)


#ifdef MW_EDB_SUPPORT
#define EDB_MAX_CMD        10
#define EDB_MAX_OPEN_DATA  8
#endif

#undef  _INFO_HEADER
#undef  _API_HEADER
#undef  _ERROR_HEADER
#define _INFO_HEADER        "{SVCTX:CRNT_EVN}Info: "
#define _API_HEADER         "{SVCTX:CRNT_EVN}API: "
#define _ERROR_HEADER       "{SVCTX:CRNT_EVN}Err: "

#if 0
#undef  DBG_ERROR
#define DBG_ERROR(_stmt)    x_dbg_stmt _stmt
#endif

#if 0
#undef  DBG_API
#define DBG_API(_stmt)      x_dbg_stmt _stmt
#endif

#if 0
#undef  DBG_INFO
#define DBG_INFO(_stmt)     x_dbg_stmt _stmt
#endif

/*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
static INT32 _crnt_evn_internal_msg (
                    VOID*                       pv_tag,
                    SVCTX_MSG_T*                pt_msg
                    );
static VOID _crnt_evn_nfy_fct(
                    HANDLE_T                    h_evctx,
#ifdef MW_EDB_SUPPORT
                    EDB_NFY_REASON_T            e_cond,
#else
                    EVCTX_COND_T                e_cond,
#endif

                    VOID*                       pv_tag,
#ifdef MW_EDB_SUPPORT  
                    UINT32                      ui4_data1,
                    UINT32                      ui4_data2                    
#else                    
                    UINT32                      ui4_data
#endif                    
                    );
#ifndef MW_EDB_SUPPORT
static VOID _crnt_evn_requery_timeout_nfy_fct(
                    HANDLE_T                    h_timer,
                    VOID*                       pv_tag
                    );
#endif
static INT32 _crnt_evn_handle_requery_timeout (
                    CRNT_EVENT_T*               pt_this,
                    HANDLE_T                    h_timer
                    );
static VOID _crnt_evn_retrieve_rating(
                    CRNT_EVENT_T*               pt_this
                    );
static INT32 _crnt_evn_handle_evctx_event(
                    CRNT_EVENT_T*               pt_this,
                    HANDLE_T                    h_evctx,
#ifdef MW_EDB_SUPPORT
                    EDB_NFY_REASON_T            e_cond,
                    UINT32                      ui4_data1,
                    UINT32                      ui4_data2
#else                    
                    EVCTX_COND_T                e_cond,
                    UINT32                      ui4_data
#endif                    
                    );
/*-----------------------------------------------------------------------------
 * private variable declarations
 *---------------------------------------------------------------------------*/
static DLIST_T(_CRNT_EVENT_T) t_g_using_crnt_evn_q;
static DLIST_T(_CRNT_EVENT_T) t_g_pooled_crnt_evn_q;
static CRNT_EVENT_T*          pt_g_crnt_evn_objs = NULL;
static BOOL                   b_g_init           = FALSE;
/*-----------------------------------------------------------------------------
 * private methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      _crnt_evn_valid
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static BOOL _crnt_evn_valid (
                    CRNT_EVENT_T*               pt_chker
                    )
{
    CRNT_EVENT_T* pt_chkee;

    if (pt_chker == NULL) {
        return FALSE;
    }

    DLIST_FOR_EACH(pt_chkee, &t_g_using_crnt_evn_q, t_dlink) {
        if (pt_chkee == pt_chker) {
            return TRUE;
        }
    }

    DBG_INFO((_INFO_HEADER"%s() - pt_chker=0x%08x not existed\r\n ", __func__, pt_chker));

    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _crnt_evn_stop_query
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _crnt_evn_stop_query(
                    CRNT_EVENT_T*               pt_this
                    )
{
    INT32 i4_ret;

    if (_crnt_evn_valid(pt_this) == FALSE) {
        return SVCTXR_NOT_EXIST;
    }

    if (x_handle_valid(pt_this->h_evctx) == TRUE) {
#ifndef MW_EDB_SUPPORT        
        CLI_FUNC_CALL_TRACE("x_evctx_delete_event");
        i4_ret = x_evctx_delete_event(pt_this->h_evctx);
        if (i4_ret != EVCTXR_OK ) {
#else
        CLI_FUNC_CALL_TRACE("x_edb_destroy");
        i4_ret = x_edb_destroy(pt_this->h_evctx);
        if (i4_ret != EDBR_OK ) {
#endif
            DBG_ERROR((_ERROR_HEADER"%s(), fail to del event(0x%x), i4_ret=%d\r\n",
                        __func__,
                        pt_this->h_evctx,
                        i4_ret
                        ));
        }
    }

    x_timer_stop(pt_this->h_requery_evn_timer);

    pt_this->h_evctx = NULL_HANDLE;

    if (pt_this->b_reset_rating_as_stop == TRUE) {
        pt_this->pv_crnt_evn_rating     = NULL;
        pt_this->z_crnt_evn_rating_size = 0;
#ifndef MW_EDB_SUPPORT        
        pt_this->e_evctx_cond           = EVCTX_COND_TRANSITION;
#endif
    } else {
        pt_this->b_start_from_stop  = TRUE;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _crnt_evn_internal_msg
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _crnt_evn_internal_msg (
                    VOID*                       pv_tag,
                    SVCTX_MSG_T*                pt_msg
                    )
{
    CRNT_EVENT_T* pt_this = (CRNT_EVENT_T*)pv_tag;
    INT32         i4_ret  = SVCTXR_OK;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_EXIST;
    }

    if (_crnt_evn_valid(pt_this) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s(), invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_NOT_EXIST;
    }

    switch(pt_msg->ui1_msg_id) {
    case _CRNT_EVN_MSG_EVCTX_NTFY:
        i4_ret = _crnt_evn_handle_evctx_event(
                    pt_this,
                    pt_msg->u.t_evctx.h_obj,
#ifndef MW_EDB_SUPPORT
                    pt_msg->u.t_evctx.e_cond,
                    pt_msg->u.t_evctx.ui4_data
#else
                    pt_msg->u.t_evctx.e_cond,
                    pt_msg->u.t_evctx.ui4_data1,
                    pt_msg->u.t_evctx.ui4_data2
#endif
                    );
        break;
    case _CRNT_EVN_MSG_REQUERY_TIMEOUT_NTFY:
        i4_ret = _crnt_evn_handle_requery_timeout(
                    pt_this,
                    pt_msg->u.t_timer.h_obj
                    );
        break;
    default:
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _crnt_evn_nfy_fct
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static VOID _crnt_evn_nfy_fct(
                    HANDLE_T                    h_evctx,
#ifdef MW_EDB_SUPPORT   
                    EDB_NFY_REASON_T            e_cond,
#else
                    EVCTX_COND_T                e_cond,
#endif
                    VOID*                       pv_tag,
#ifdef MW_EDB_SUPPORT  
                    UINT32                      ui4_data1,
                    UINT32                      ui4_data2   
#else                    
                    UINT32                      ui4_data
#endif      
                    )
{
    CRNT_EVENT_T* pt_this = (CRNT_EVENT_T*)pv_tag;
    SVCTX_MSG_T   t_msg   = {0};

    t_msg.ui1_msg_id         = _CRNT_EVN_MSG_EVCTX_NTFY;
    t_msg.pf_invoke_func     = _crnt_evn_internal_msg;
    t_msg.pv_tag             = pv_tag;
    t_msg.u.t_evctx.h_obj    = h_evctx;
    t_msg.u.t_evctx.e_cond   = e_cond;
    
#ifdef MW_EDB_SUPPORT    
    t_msg.u.t_evctx.ui4_data1 = ui4_data1;
    t_msg.u.t_evctx.ui4_data2 = ui4_data2;
#else    
    t_msg.u.t_evctx.ui4_data = ui4_data;
#endif


    pt_this->pf_nty(
                    pt_this,
                    CRNT_EVENT_NTFY_CODE_REQ_SEND_MSG,
                    pt_this->pv_tag,
                    (UINT32)&t_msg
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      _crnt_evn_requery_timeout_nfy_fct
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
#ifndef MW_EDB_SUPPORT 
static VOID _crnt_evn_requery_timeout_nfy_fct(
                    HANDLE_T                    h_timer,
                    VOID*                       pv_tag
                    )
{
    CRNT_EVENT_T* pt_this = (CRNT_EVENT_T*)pv_tag;
    SVCTX_MSG_T   t_msg   = {0};

    t_msg.ui1_msg_id      = _CRNT_EVN_MSG_REQUERY_TIMEOUT_NTFY;
    t_msg.u.t_timer.h_obj = h_timer;
    t_msg.pf_invoke_func  = _crnt_evn_internal_msg;
    t_msg.pv_tag          = pv_tag;

    pt_this->pf_nty(
                    pt_this,
                    CRNT_EVENT_NTFY_CODE_REQ_SEND_MSG,
                    pt_this->pv_tag,
                    (UINT32)&t_msg
                    );
}
#endif

/*-----------------------------------------------------------------------------
 * Name
 *      _crnt_evn_handle_requery_timeout
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _crnt_evn_handle_requery_timeout (
                    CRNT_EVENT_T*               pt_this,
                    HANDLE_T                    h_timer
                    )
{
    INT32 i4_ret;

    if (pt_this->h_requery_evn_timer != h_timer) {
        return SVCTXR_OK;
    }

    if (pt_this->b_in_requerying == FALSE) {
        return SVCTXR_OK;
    }

    i4_ret = crnt_evn_start_query(
                    pt_this,
                    pt_this->e_dev_type,
                    pt_this->s_src_grp_name,
                    pt_this->h_conn_handle,
                    &pt_this->t_key
                    );
    pt_this->b_in_requerying = TRUE;
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _crnt_evn_retrieve_rating
 * Description
 * Input arguments
 * Output arguments
 * Returns  -
 *---------------------------------------------------------------------------*/
static VOID _crnt_evn_retrieve_rating(
                    CRNT_EVENT_T*               pt_this
                    )
{
#ifndef MW_EDB_SUPPORT    
    EVCTX_COND_T e_cond = EVCTX_COND_UNAVAIL;
#else
    EDB_CH_EVT_ID_T      t_ch_evt_id = {0};
#endif
    SIZE_T       z_len  = 0;
    INT32        i4_ret;

    if (x_handle_valid(pt_this->h_evctx) == FALSE) {
        return;
    }

    pt_this->pv_crnt_evn_rating     = NULL;
    pt_this->z_crnt_evn_rating_size = 0;

#ifdef MW_EDB_SUPPORT 
    /* Key info prepare */
    t_ch_evt_id.e_brdcst_type     = pt_this->t_key.t_svl.e_brdcst_type;
    t_ch_evt_id.ui2_svl_id        = pt_this->t_key.t_svl.ui2_svc_lst_id;
    t_ch_evt_id.ui4_channel_id    = pt_this->t_key.t_svl.ui4_channel_id;
    t_ch_evt_id.b_auto_event_id   = TRUE;

    i4_ret = x_edb_get_info_len(
                        pt_this->h_evctx,
                        EDB_KEY_TYPE_RATING_LIST_BY_ID,
                        &t_ch_evt_id, /* get the first content advisory regardless the language */
                        &z_len
                        );
#else
    i4_ret = x_evctx_get_event_info_len(
                    pt_this->h_evctx,
                    EVCTX_KEY_TYPE_RATING,
                    NULL, /* get the first content advisory regardless the language */
                    &z_len,
                    &e_cond
                    );
#endif    
    DBG_INFO((_INFO_HEADER"get rating info, i4_ret = %d, len=%d\r\n", i4_ret, z_len));
    if (i4_ret < 0 || z_len == 0) {
        return;
    }

    pt_this->z_crnt_evn_rating_size = z_len;

    DBG_INFO((_INFO_HEADER"crnt rating need buf size=%d, crnt buf size=%d\r\n",
                    z_len,
                    pt_this->z_evn_rating_buf_size
                    ));
    /* if needed event rating buffer size > current event rating
     * buffer size or current buffer is empty, free old one if not empty,
     * create a new one and store the buffer size.
     */
    if (z_len > pt_this->z_evn_rating_buf_size || pt_this->pv_evn_rating_buf == NULL) {
        if (pt_this->pv_evn_rating_buf != NULL) {
            x_mem_free(pt_this->pv_evn_rating_buf);
            pt_this->pv_evn_rating_buf = NULL;
            DBG_INFO((_INFO_HEADER"free memory(size=%d) for event rating\r\n",
                    pt_this->z_evn_rating_buf_size
                    ));
        }

        pt_this->pv_evn_rating_buf = (VOID*) x_mem_alloc(z_len);
        if (pt_this->pv_evn_rating_buf == NULL) {
            pt_this->z_evn_rating_buf_size = 0;
            DBG_ERROR((_ERROR_HEADER"x_mem_alloc() failed\r\n"));
            return;
        }
        pt_this->z_evn_rating_buf_size = z_len;
        DBG_INFO((_INFO_HEADER"alloc memory(size=%d) for event rating\r\n", z_len));
    }

#ifndef MW_EDB_SUPPORT
    i4_ret = x_evctx_get_event_info(
                    pt_this->h_evctx,
                    EVCTX_KEY_TYPE_RATING,
                    NULL, /* get the first content advisory regardless the language */
                    &z_len,
                    pt_this->pv_evn_rating_buf,
                    &e_cond
                    );
#else
    CLI_FUNC_CALL_TRACE("x_edb_get_info");
    i4_ret = x_edb_get_info(
                    pt_this->h_evctx,
                    EDB_KEY_TYPE_RATING_LIST_BY_ID,
                    &t_ch_evt_id, /* get the first content advisory regardless the language */
                    &z_len,
                    pt_this->pv_evn_rating_buf
                    );
#endif
    if (i4_ret < 0) {    
        pt_this->z_crnt_evn_rating_size = 0;
        pt_this->pv_crnt_evn_rating     = NULL;
    } else {
        pt_this->pv_crnt_evn_rating     = pt_this->pv_evn_rating_buf;
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _crnt_evn_handle_evctx_event
 * Description
 *      The API is used to handle the events sent from a event context object
 * Input arguments
 *      pt_this    a pointer points to the Service Context
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _crnt_evn_handle_evctx_event(
                    CRNT_EVENT_T*               pt_this,
                    HANDLE_T                    h_evctx,
#ifdef MW_EDB_SUPPORT
                    EDB_NFY_REASON_T            e_cond,
                    UINT32                      ui4_data1,
                    UINT32                      ui4_data2
#else                    
                    EVCTX_COND_T                e_cond,
                    UINT32                      ui4_data
#endif                   
                    )
{
#ifdef MW_EDB_SUPPORT    

    /* Check and discard garbage event for current obj */
    if (DEV_TUNER != pt_this->e_dev_type 
            && DEV_1394 != pt_this->e_dev_type
            && DEV_AVC  != pt_this->e_dev_type)
    {
        return SVCTXR_INV_ARG;
    }
    if (DEV_TUNER == pt_this->e_dev_type 
        && ((UINT16)ui4_data1 != pt_this->t_key.t_svl.ui2_svc_lst_id
                 || ui4_data2 != pt_this->t_key.t_svl.ui4_channel_id))
    {
        DBG_ERROR((_ERROR_HEADER"%s()SVL info not match!\r\n", __func__));
        return SVCTXR_INV_ARG;
    }

    if ((e_cond & EDB_REASON_NOW_EVENT_UPDATED)
        || (e_cond & EDB_REASON_NEX_EVENT_UPDATED)
        || (e_cond & EDB_REASON_EVENT_IN_ACTIVE_WIN_UPDATED))
    {
        pt_this->b_in_requerying = FALSE;
                _crnt_evn_retrieve_rating(pt_this);
                pt_this->b_notify_evn_not_ready = FALSE;
                pt_this->pf_nty(
                            pt_this,
                            CRNT_EVENT_NTFY_CODE_EVN_READY,
                            pt_this->pv_tag,
                            (UINT32)e_cond
                            );        
    }
    else 
    {
        pt_this->pf_nty(
                    pt_this,
                    CRNT_EVENT_NTFY_CODE_EVN_NOT_READY,
                    pt_this->pv_tag,
                    (UINT32)e_cond
                    );
        pt_this->b_notify_evn_not_ready = TRUE;
    }

    return SVCTXR_OK; 

    
#else    

    INT32 i4_ret = SVCTXR_OK;

    if (pt_this->h_evctx != h_evctx) {
        return SVCTXR_OK;
    }

    pt_this->e_evctx_cond = e_cond;

    switch(e_cond) {
    case EVCTX_COND_TRANSITION:
        if (pt_this->b_in_requerying == TRUE) {
            pt_this->e_evctx_cond = EVCTX_COND_UNAVAIL;
        }
        return SVCTXR_OK;
    case EVCTX_COND_EXPIRED:/* the event becomes expired */
        pt_this->b_in_requerying = FALSE;
        i4_ret = crnt_evn_start_query(
                    pt_this,
                    pt_this->e_dev_type,
                    pt_this->s_src_grp_name,
                    pt_this->h_conn_handle,
                    &pt_this->t_key
                    );
        return i4_ret;
    case EVCTX_COND_UNAVAIL:
        /* should start to re-query EIT-0 */
        pt_this->pv_crnt_evn_rating     = NULL;
        pt_this->z_crnt_evn_rating_size = 0;

        _crnt_evn_stop_query(pt_this);

        /* force to set EVCTX_COND_UNAVAIL after stop query */
        pt_this->e_evctx_cond = EVCTX_COND_UNAVAIL;

        x_timer_start(
                    pt_this->h_requery_evn_timer,
                    pt_this->ui4_requery_evn_delay,
                    X_TIMER_FLAG_ONCE,
                    _crnt_evn_requery_timeout_nfy_fct,
                    (VOID*)pt_this
                    );
        if (pt_this->b_in_requerying == TRUE) {
            break;
        }
        pt_this->b_in_requerying = TRUE;
        if (pt_this->b_notify_evn_not_ready == TRUE) {
            break;
        }
        pt_this->pf_nty(
                    pt_this,
                    CRNT_EVENT_NTFY_CODE_EVN_NOT_READY,
                    pt_this->pv_tag,
                    (UINT32)e_cond
                    );
        pt_this->b_notify_evn_not_ready = TRUE;
        break;
    case EVCTX_COND_ACTIVE: /* the event becomes the current event */
    case EVCTX_COND_AVAIL:  /* the event becomes available */
    case EVCTX_COND_UPDATED:/* the content of event is updated */
        x_cm_set(pt_this->h_conn_handle, BRDCST_CTRL_SET_PRESENT_EVENT, (VOID*)pt_this->h_evctx);
        
        pt_this->b_in_requerying = FALSE;
        _crnt_evn_retrieve_rating(pt_this);
        pt_this->b_notify_evn_not_ready = FALSE;
        pt_this->pf_nty(
                    pt_this,
                    CRNT_EVENT_NTFY_CODE_EVN_READY,
                    pt_this->pv_tag,
                    (UINT32)e_cond
                    );
        break;
    default:
        break;
    }

    return SVCTXR_OK;

#endif
    
}

#ifdef MW_EDB_SUPPORT
static BOOL _crnt_evn_test_evn_ready(
            CRNT_EVENT_T*       pt_this)
{
    EDB_BUCKET_INFO_T    t_bucket_info = {0};
    EDB_CH_TIME_FILTER_T t_ch_time_filter = {0};
    INT32                i4_ret = 0;
    SIZE_T               t_size = 0;
    EDB_CH_EVT_ID_T      t_ch_evt_id[2] = {{0}, {0}};
    UINT16               ui2_event_id_bucket_num = 2;

    ASSERT(pt_this != NULL, ("%s(), NULL PTR!\r\n", __func__));

    if (x_handle_valid(pt_this->h_evctx) == FALSE)
    {
        return FALSE;
    }

    /* Query now and next event by range */
    do
    {
        x_memset (&t_bucket_info, 0, sizeof(EDB_BUCKET_INFO_T));
        x_memset (&t_ch_time_filter, 0, sizeof(EDB_CH_TIME_FILTER_T));    

        /* Key info prepare */
        t_ch_time_filter.e_brdcst_type  = pt_this->t_key.t_svl.e_brdcst_type;
        t_ch_time_filter.ui2_svl_id     = pt_this->t_key.t_svl.ui2_svc_lst_id; 
        t_ch_time_filter.ui4_channel_id = 0;
        t_ch_time_filter.t_start_time   = x_dt_get_brdcst_utc(NULL,NULL); 
        t_ch_time_filter.t_end_time     = 0;
        t_ch_time_filter.pf_filter_fct  = NULL;
        t_ch_time_filter.pv_filter_tag  = NULL;
        t_ch_time_filter.pf_nfy_fct     = NULL; 
        t_ch_time_filter.pv_nfy_tag     = NULL;

        /* Bucket info translation */
        t_bucket_info.pui2_num_events   = &ui2_event_id_bucket_num;
        
        x_memset(&(t_ch_evt_id[0]), 0, sizeof(t_ch_evt_id));
        t_bucket_info.pui2_ch_event_id  = &(t_ch_evt_id[0]);

        t_size = sizeof(EDB_BUCKET_INFO_T);

        CLI_FUNC_CALL_TRACE("x_edb_get_info");
        i4_ret = x_edb_get_info (pt_this->h_evctx,
                                 EDB_KEY_TYPE_NOW_NEXT_EVENT_NUM_BY_UTC,
                                 & t_ch_time_filter,
                                 & t_size, 
                                 & t_bucket_info);
    }while (0); 

    return (i4_ret == EDBR_OK && ui2_event_id_bucket_num > (UINT16)0 );
}
#endif  /* MW_EDB_SUPPORT */

/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: crnt_evn_init
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *-----------------------------------------------------------------------------*/
INT32 crnt_evn_init(
                    UINT16                      ui2_amount
                    )
{
    UINT16 ui2_idx;
    CRNT_EVENT_T*   pt_crnt_evn = NULL;

    if (b_g_init == TRUE) {
        return SVCTXR_OK;
    }

    pt_g_crnt_evn_objs = (CRNT_EVENT_T*) x_mem_alloc(sizeof(CRNT_EVENT_T) * ui2_amount);
    if(pt_g_crnt_evn_objs == NULL) {
        DBG_ERROR((_ERROR_HEADER"create memory for CRNT_EVENT_T pool failed\r\n"));
        return SVCTXR_FAIL;
    }

    x_memset(pt_g_crnt_evn_objs, 0, sizeof(CRNT_EVENT_T) * ui2_amount);

    DLIST_INIT(&t_g_using_crnt_evn_q);
    DLIST_INIT(&t_g_pooled_crnt_evn_q);

    for (ui2_idx = 0; ui2_idx < ui2_amount; ui2_idx++) {
        pt_crnt_evn = &(pt_g_crnt_evn_objs[ui2_idx]);

        DLIST_INSERT_HEAD(pt_crnt_evn, &t_g_pooled_crnt_evn_q, t_dlink);
    }

    b_g_init = TRUE;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: crnt_evn_deinit
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *-----------------------------------------------------------------------------*/
VOID crnt_evn_deinit(
                    VOID
                    )
{
    if (b_g_init == FALSE) {
        return;
    }

    x_mem_free(pt_g_crnt_evn_objs);

    DLIST_INIT(&t_g_using_crnt_evn_q);
    DLIST_INIT(&t_g_pooled_crnt_evn_q);

    pt_g_crnt_evn_objs = NULL;
    b_g_init           = FALSE;
}

 /*-----------------------------------------------------------------------------
 * Name
 *      crnt_evn_open
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 crnt_evn_open(
                    UINT32                      ui4_requery_evn_delay,
                    BOOL                        b_reset_rating_as_stop,
                    crnt_evn_nty_fct            pf_nty,
                    VOID*                       pv_tag,
                    CRNT_EVENT_T**              ppt_this
                    )
{
    CRNT_EVENT_T* pt_this = NULL;
    INT32         i4_ret;

    DBG_API((_API_HEADER"%s("
                    "ui4_requery_evn_delay = %d,"
                    "pf_nty = 0x%x,"
                    "pv_tag = 0x%x,"
                    "ppt_this = 0x%x"
                    ")\r\n",
                    __func__,
                    ui4_requery_evn_delay,
                    pf_nty,
                    pv_tag,
                    ppt_this
                    ));
    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (ppt_this == NULL || pf_nty == NULL || ui4_requery_evn_delay == 0) {
        return SVCTXR_INV_ARG;
    }

    do {
        /* check the pool is empty or not */
        pt_this = DLIST_HEAD(&t_g_pooled_crnt_evn_q);
        if (pt_this == NULL) {
            DBG_ERROR((_ERROR_HEADER"%s() failed! ret=SVCTXR_NO_RESOURCE\r\n", __func__));
            i4_ret = SVCTXR_NO_RESOURCE;
            break;
        }

        /* move the instance from pooled to used */
        DLIST_REMOVE(pt_this, &t_g_pooled_crnt_evn_q, t_dlink);
        DLIST_INSERT_HEAD(pt_this, &t_g_using_crnt_evn_q, t_dlink);

        pt_this->h_evctx                    = NULL_HANDLE;
        pt_this->pv_evn_rating_buf          = NULL;
        pt_this->z_crnt_evn_rating_size     = 0;
        pt_this->pv_crnt_evn_rating         = NULL;
        pt_this->z_evn_rating_buf_size      = 0;
#ifndef MW_EDB_SUPPORT        
        pt_this->e_evctx_cond               = EVCTX_COND_TRANSITION;
#else
        pt_this->e_evctx_cond               = EDB_COND_UNCONFIG;
        pt_this->t_key.t_svl.e_brdcst_type  = BRDCST_TYPE_UNKNOWN;
#endif
        pt_this->h_requery_evn_timer        = NULL_HANDLE;
        pt_this->b_in_requerying            = FALSE;
        pt_this->b_notify_evn_not_ready     = FALSE;
        pt_this->b_reset_rating_as_stop     = b_reset_rating_as_stop;
        pt_this->b_start_from_stop          = FALSE;

        pt_this->e_dev_type                 = DEV_UNKNOWN;
        pt_this->t_key.t_svl.ui2_svc_lst_id = SVL_NULL_ID;
        pt_this->t_key.t_svl.ui2_svl_rec_id = SVL_NULL_REC_ID;
        pt_this->s_src_grp_name[0]          = '\0';
        pt_this->h_conn_handle              = NULL_HANDLE;

        pt_this->ui4_requery_evn_delay      = ui4_requery_evn_delay;
        pt_this->pf_nty                     = pf_nty;
        pt_this->pv_tag                     = pv_tag;

        i4_ret = x_timer_create(&pt_this->h_requery_evn_timer);
        if (i4_ret != OSR_OK) {
            DBG_ERROR((_ERROR_HEADER"create h_requery_evn_timer failed! ret=%d\r\n", i4_ret));
            i4_ret = SVCTXR_FAIL;
        }

        *ppt_this = pt_this;
        return SVCTXR_OK;
    } while (0);

    DBG_ERROR((_ERROR_HEADER"----------------%s() failed!\r\n\r\n", __func__));

    /* error handling */
    if (pt_this != NULL) {
        DBG_ERROR((_ERROR_HEADER"%s() failed, return pt_this = 0x%x to pool\r\n",
                        __func__,
                        pt_this
                        ));

        i4_ret = crnt_evn_close(pt_this);
        if (i4_ret != SVCTXR_OK) {
            DBG_ERROR((_ERROR_HEADER"%s(), crnt_evn_close() failed! ret=%d\r\n", __func__, i4_ret));
        }
    }

    *ppt_this = NULL;
    return i4_ret;
}
 /*-----------------------------------------------------------------------------
 * Name
 *      crnt_evn_close
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 crnt_evn_close(
                    CRNT_EVENT_T*               pt_this
                    )
{
    INT32 i4_ret;

    DBG_API((_API_HEADER"%s("
                    "pt_this = 0x%x, "
                    "h_evctx = %d"
                    ")\r\n",
                    __func__,
                    pt_this,
                    pt_this->h_evctx
                    ));
    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_crnt_evn_valid(pt_this) == FALSE) {
        DBG_INFO((_INFO_HEADER"%s(), invalid crnt_event = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_OK;
    }

    if (x_handle_valid(pt_this->h_evctx) == TRUE) {
#ifndef MW_EDB_SUPPORT        
        CLI_FUNC_CALL_TRACE("x_evctx_delete_event");
        i4_ret = x_evctx_delete_event(pt_this->h_evctx);
        if (i4_ret != EVCTXR_OK) {
#else
        CLI_FUNC_CALL_TRACE("x_edb_destroy");
        i4_ret = x_edb_destroy(pt_this->h_evctx);
        if (i4_ret != EDBR_OK) {
#endif
            DBG_ERROR((_ERROR_HEADER"%s() invokes x_evctx_delete_event(%d) failed, i4_ret=%d\r\n",
                        __func__,
                        pt_this->h_evctx,
                        i4_ret
                        ));
        }
    }

    if (pt_this->pv_evn_rating_buf != NULL) {
        x_mem_free(pt_this->pv_evn_rating_buf);
    }

    if (x_handle_valid(pt_this->h_requery_evn_timer) == TRUE) {
        x_timer_delete(pt_this->h_requery_evn_timer);
    }

    pt_this->h_evctx                = NULL_HANDLE;
    pt_this->pv_crnt_evn_rating     = NULL;
    pt_this->z_crnt_evn_rating_size = 0;
    pt_this->pv_evn_rating_buf      = NULL;
    pt_this->z_evn_rating_buf_size  = 0;
    pt_this->h_requery_evn_timer    = NULL_HANDLE;

    DLIST_REMOVE(pt_this, &t_g_using_crnt_evn_q, t_dlink);
    DLIST_INSERT_HEAD(pt_this, &t_g_pooled_crnt_evn_q, t_dlink);

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      crnt_evn_start_query
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 crnt_evn_start_query(
                    CRNT_EVENT_T*               pt_this,
                    DEVICE_TYPE_T               e_dev_type,
                    CHAR*                       ps_src_grp_name,
                    HANDLE_T                    h_conn_handle,
                    CRNT_EVENT_KEY_T*           pt_key
                    )
{
#ifndef MW_EDB_SUPPORT
    EVCTX_COMMAND_T at_cmd[6];
    EVCTX_COND_T    e_cond = EVCTX_COND_UNAVAIL;
#else
    EDB_COMMAND_T           at_cmd[EDB_MAX_CMD];
    EDB_ENG_OPEN_DATA_T     at_eng_open_data[EDB_MAX_OPEN_DATA];
#endif

    INT32           i4_ret = SVCTXR_OK;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_crnt_evn_valid(pt_this) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s(), invalid crnt_event = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_NOT_EXIST;
    }

    if (ps_src_grp_name == NULL) {
        return SVCTXR_INV_ARG;
    }

    if (x_handle_valid(pt_this->h_evctx) == TRUE) {
        _crnt_evn_stop_query(pt_this);
    }

    pt_this->h_conn_handle = h_conn_handle;
    pt_this->e_dev_type    = DEV_GET_GROUP_ID(e_dev_type);
    x_strcpy(pt_this->s_src_grp_name, ps_src_grp_name);

#ifndef MW_EDB_SUPPORT 
    switch(pt_this->e_dev_type) {
    case DEV_1394:
    case DEV_TUNER:
        if (pt_key == NULL) {
            return SVCTXR_INV_ARG;
        }

        pt_this->t_key         = *pt_key;
        at_cmd[0].e_code       = EVCTX_CMD_DEV_TYPE;
        at_cmd[0].u.e_dev_type = pt_this->e_dev_type;
        at_cmd[1].e_code       = EVCTX_CMD_SRC_INFO;
        at_cmd[1].u.ps_name    = pt_this->s_src_grp_name;
        at_cmd[2].e_code       = EVCTX_CMD_CONN_HANDLE;
        at_cmd[2].u.h_obj      = pt_this->h_conn_handle;
        at_cmd[3].e_code       = EVCTX_CMD_SVL_ID;
        at_cmd[3].u.ui2_number = pt_this->t_key.t_svl.ui2_svc_lst_id;
        at_cmd[4].e_code       = EVCTX_CMD_SVL_REC_ID;
        at_cmd[4].u.ui2_number = pt_this->t_key.t_svl.ui2_svl_rec_id;
        at_cmd[5].e_code       = EVCTX_CMD_END;
        break;
    case DEV_AVC:
        at_cmd[0].e_code       = EVCTX_CMD_DEV_TYPE;
        at_cmd[0].u.e_dev_type = pt_this->e_dev_type;
        at_cmd[1].e_code       = EVCTX_CMD_SRC_INFO;
        at_cmd[1].u.ps_name    = pt_this->s_src_grp_name;
        at_cmd[2].e_code       = EVCTX_CMD_CONN_HANDLE;
        at_cmd[2].u.h_obj      = pt_this->h_conn_handle;
        at_cmd[3].e_code       = EVCTX_CMD_END;
        break;
    default:
        return SVCTXR_INV_ARG;
    }

    if (pt_this->b_in_requerying == FALSE) {
        pt_this->b_notify_evn_not_ready = FALSE;
    }

    CLI_FUNC_CALL_TRACE("x_evctx_load_current_event");
    i4_ret = x_evctx_load_current_event (
                    at_cmd,
                    (VOID*)pt_this,
                    _crnt_evn_nfy_fct,
                    &pt_this->h_evctx,
                    &e_cond
                    );
    if (i4_ret != EVCTXR_OK) {
        pt_this->e_evctx_cond = e_cond = EVCTX_COND_UNAVAIL;
    }

    if (e_cond != EVCTX_COND_TRANSITION) {
        i4_ret = _crnt_evn_handle_evctx_event(
                    pt_this,
                    pt_this->h_evctx,
                    e_cond,
                    0
                    );
    } else { /* e_cond == EVCTX_COND_TRANSITION */
        if (pt_this->b_in_requerying == TRUE) {
            pt_this->e_evctx_cond = EVCTX_COND_UNAVAIL;
        } else {
            /* send CRNT_EVENT_NTFY_CODE_START_QUERY only in EVCTX_COND_TRANSITION */
            if (pt_this->b_reset_rating_as_stop == FALSE
                    && pt_this->b_start_from_stop == TRUE) {
                return SVCTXR_OK;
            }

            pt_this->pf_nty(
                    pt_this,
                    CRNT_EVENT_NTFY_CODE_START_QUERY,
                    pt_this->pv_tag,
                    (UINT32)EVCTX_COND_TRANSITION
                    );
        }
    }
#else

    x_memset (at_cmd, 0, sizeof(at_cmd));
    x_memset (at_eng_open_data, 0, sizeof(at_eng_open_data));

    switch(pt_this->e_dev_type) {
    case DEV_1394:
    case DEV_TUNER:
        if (pt_key == NULL) {
            return SVCTXR_INV_ARG;
        }

        pt_this->t_key         = *pt_key;


        at_eng_open_data[0].e_brdcst_type = pt_this->t_key.t_svl.e_brdcst_type;
        at_eng_open_data[0].ui2_svl_id    = pt_this->t_key.t_svl.ui2_svc_lst_id;
        at_eng_open_data[0].ps_tuner_name = pt_this->s_src_grp_name;


        at_cmd[0].e_code                = EDB_CMD_OPEN_DATA;
        at_cmd[0].u.pt_eng_open_data    = at_eng_open_data;

        at_cmd[1].e_code       = EVCTX_CMD_END;
        break;
    case DEV_AVC:
        at_cmd[0].e_code       = EVCTX_CMD_END;
        break;
    default:
        return SVCTXR_INV_ARG;
    }

    if (pt_this->b_in_requerying == FALSE) {
        pt_this->b_notify_evn_not_ready = FALSE;
    }

    CLI_FUNC_CALL_TRACE("x_edb_create");
    i4_ret = x_edb_create("EDB_MAIN", at_cmd, NULL, NULL, &pt_this->h_evctx);
    if (i4_ret != EDBR_OK) {
        DBG_ERROR((_ERROR_HEADER"%s(), edb open failed! i4_ret=%d\r\n", __func__, i4_ret));
        return SVCTXR_FAIL;
    }

    x_edb_set_listener(pt_this->h_evctx, (VOID*)pt_this, _crnt_evn_nfy_fct);
    pt_this->pf_nty(
            pt_this,
            CRNT_EVENT_NTFY_CODE_START_QUERY,
            pt_this->pv_tag,
            (UINT32)EVCTX_COND_TRANSITION
            );
    
    if (pt_this->h_evctx != NULL_HANDLE)
    {
        if (_crnt_evn_test_evn_ready(pt_this))
        {
             pt_this->pf_nty(
                    pt_this,
                    CRNT_EVENT_NTFY_CODE_EVN_READY,
                    pt_this->pv_tag,
                    (UINT32)EDB_COND_STARTED
                    );
        }
    }
    _crnt_evn_retrieve_rating(pt_this);
#endif /* MW_EDB_SUPPORT */ 

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      crnt_evn_stop_query
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
INT32 crnt_evn_stop_query(
                    CRNT_EVENT_T*               pt_this
                    )
{
    INT32 i4_ret;

    DBG_API((_API_HEADER"%s("
                    "pt_this = 0x%x "
                    "h_evctx = %d"
                    ")\r\n",
                    __func__,
                    pt_this,
                    pt_this->h_evctx
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }
    
    if (pt_this == NULL) {
        return SVCTXR_OK;
    }

    i4_ret = _crnt_evn_stop_query(pt_this);
    if (i4_ret == SVCTXR_OK) {
        pt_this->b_notify_evn_not_ready = FALSE;
        pt_this->b_in_requerying        = FALSE;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      crnt_evn_is_in_querying
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
BOOL crnt_evn_is_in_querying(
                    CRNT_EVENT_T*               pt_this
                    )
{
    if (b_g_init == FALSE) {
        return FALSE;
    }

    if (_crnt_evn_valid(pt_this) == FALSE) {
        return FALSE;
    }

    return x_handle_valid(pt_this->h_evctx);
}
/*-----------------------------------------------------------------------------
 * Name
 *      crnt_evn_is_ready
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
BOOL crnt_evn_is_ready(
                    CRNT_EVENT_T*               pt_this
                    )
{
    if (b_g_init == FALSE) {
        return FALSE;
    }

    if (_crnt_evn_valid(pt_this) == FALSE) {
        return FALSE;
    }
    
#ifndef MW_EDB_SUPPORT    
    if (pt_this->e_evctx_cond == EVCTX_COND_ACTIVE
            || pt_this->e_evctx_cond == EVCTX_COND_AVAIL
            || pt_this->e_evctx_cond == EVCTX_COND_UPDATED) {
        return TRUE;
    }

    return FALSE;
#else
    return pt_this->e_evctx_cond == EDB_REASON_NOW_EVENT_UPDATED;
#endif

}
/*-----------------------------------------------------------------------------
 * Name
 *      crnt_evn_get_rating
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
VOID* crnt_evn_get_rating(
                    CRNT_EVENT_T*               pt_this
                    )
{
    if (b_g_init == FALSE) {
        return NULL;
    }

    if (_crnt_evn_valid(pt_this) == FALSE) {
        DBG_INFO((_INFO_HEADER"%s() invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return NULL;
    }
    
    if (pt_this->z_crnt_evn_rating_size == 0) {
        pt_this->pv_crnt_evn_rating = NULL;
    }

    return (pt_this->pv_crnt_evn_rating);
}
/*-----------------------------------------------------------------------------
 * Name
 *      crnt_evn_get_rating_size
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
SIZE_T crnt_evn_get_rating_size(
                    CRNT_EVENT_T*               pt_this
                    )
{
    if (b_g_init == FALSE) {
        return 0;
    }

    if (_crnt_evn_valid(pt_this) == FALSE) {
        DBG_INFO((_INFO_HEADER"%s() invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return 0;
    }
    
    if (pt_this->pv_crnt_evn_rating == NULL) {
        pt_this->z_crnt_evn_rating_size = 0;
    }
    
    return (pt_this->z_crnt_evn_rating_size);
}
/*-----------------------------------------------------------------------------
 * Name
 *      crnt_evn_get_rating
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
#ifndef MW_EDB_SUPPORT 
EVCTX_COND_T 
#else
EDB_COND_T
#endif
crnt_evn_get_cond(
                    CRNT_EVENT_T*               pt_this
                    )
{
    if (b_g_init == FALSE) {
#ifndef MW_EDB_SUPPORT        
        return EVCTX_COND_TRANSITION;
#else
        return EDB_COND_UNCONFIG;
#endif
    }

    if (_crnt_evn_valid(pt_this) == FALSE) {
        DBG_INFO((_INFO_HEADER"%s() invalid pt_this = 0x%x\r\n", __func__, pt_this));
#ifndef MW_EDB_SUPPORT        
        return EVCTX_COND_TRANSITION;
#else
        return EDB_COND_STOPPED;
#endif
    }

#ifndef MW_EDB_SUPPORT        
    return (pt_this->e_evctx_cond);
#else
    return EDB_COND_STARTED;
#endif
}
/*-----------------------------------------------------------------------------
 * Name
 *      crnt_evn_get_handle
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
HANDLE_T crnt_evn_get_handle(
                    CRNT_EVENT_T*               pt_this
                    )
{
    if (b_g_init == FALSE) {
        return NULL_HANDLE;
    }

    if (_crnt_evn_valid(pt_this) == FALSE) {
        DBG_INFO((_INFO_HEADER"%s() invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return NULL_HANDLE;
    }

    return (pt_this->h_evctx);
}
/*-----------------------------------------------------------------------------
 * Name
 *      crnt_evn_get_using_head
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
CRNT_EVENT_T* crnt_evn_get_using_head(
                    VOID
                    )
{
    return DLIST_HEAD(&t_g_using_crnt_evn_q);
}
/*-----------------------------------------------------------------------------
 * Name
 *      crnt_evn_get_pooled_head
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
CRNT_EVENT_T* crnt_evn_get_pooled_head(
                    VOID
                    )
{
    return DLIST_HEAD(&t_g_pooled_crnt_evn_q);
}
/*-----------------------------------------------------------------------------
 * Name
 *      crnt_evn_next
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
CRNT_EVENT_T* crnt_evn_next(
                    CRNT_EVENT_T*             pt_this
                    )
{
    if (b_g_init == FALSE || pt_this == NULL ) {
        return NULL;
    }

    return (DLIST_NEXT(pt_this, t_dlink));
}
/*-----------------------------------------------------------------------------
 * Name
 *      crnt_evn_print_self
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
VOID crnt_evn_print_self(
                    CRNT_EVENT_T*             pt_this
                    )
{
#ifdef CLI_LVL_ALL
#if 0
    x_dbg_stmt("------------------------------------ size = %d\r\n", sizeof(CRNT_EVENT_T));
    x_dbg_stmt("pt_crnt_evn           - 0x%x\r\n",   pt_this);
    if (pt_this != NULL) {
        x_dbg_stmt("h_evctx               - %d, %s\r\n", pt_this->h_evctx, s_g_evctx_conds[pt_this->e_evctx_cond]);
        x_dbg_stmt("h_requery_evn_timer   - %d\r\n",     pt_this->h_requery_evn_timer);
        x_dbg_stmt("ui4_requery_evn_delay - %d\r\n",     pt_this->ui4_requery_evn_delay);
        x_dbg_stmt("b_in_requerying       - %s\r\n",    (pt_this->b_in_requerying == TRUE ? "TRUE" : "FALSE"));
        x_dbg_stmt("pv_crnt_evn_rating    - 0x%x\r\n",   pt_this->pv_crnt_evn_rating);
        x_dbg_stmt("z_crnt_evn_rating_size- %d\r\n",     pt_this->z_crnt_evn_rating_size);
        x_dbg_stmt("pv_evn_rating_buf     - 0x%x\r\n",   pt_this->pv_evn_rating_buf);
        x_dbg_stmt("z_evn_rating_buf_size - %d\r\n",     pt_this->z_evn_rating_buf_size);

        x_dbg_stmt("e_dev_type            - %d\r\n",     pt_this->e_dev_type);
        x_dbg_stmt("s_src_grp_name        - %s\r\n",     pt_this->s_src_grp_name);
        if (pt_this->e_dev_type == DEV_TUNER || pt_this->e_dev_type == DEV_1394) {
            x_dbg_stmt("ui2_svc_lst_id        - %d\r\n",     pt_this->t_key.t_svl.ui2_svc_lst_id);
            x_dbg_stmt("ui2_svl_rec_id        - %d\r\n",     pt_this->t_key.t_svl.ui2_svl_rec_id);
        }
        x_dbg_stmt("pf_nty                - 0x%x\r\n",   pt_this->pf_nty);
        x_dbg_stmt("pv_tag                - 0x%x\r\n",   pt_this->pv_tag);
    }
    x_dbg_stmt("====================================\r\n");
#endif
#endif
}
