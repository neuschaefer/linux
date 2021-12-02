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
 * $RCSfile: strm_fmt_chker.c,v $
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
#include "svctx/util/strm_fmt_chker.h"
#include "svctx/svctx_dbg.h"
#include "svctx/svctx.h"
#include "handle/handle.h"
#ifdef MW_TV_AV_BYPASS_SUPPORT
#include "svctx/svctx_mgr.h"
#endif

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#define _STRM_FMT_CHKER_MSG_VIDEO_DELAY_NTFY  ((UINT8)230)
#define _STRM_FMT_CHKER_MSG_AUDIO_DELAY_NTFY  ((UINT8)231)

#undef  _INFO_HEADER
#undef  _API_HEADER
#undef  _ERROR_HEADER
#define _INFO_HEADER        "{SVCTX:STRM_FMT_CHKER}Info: "
#define _API_HEADER         "{SVCTX:STRM_FMT_CHKER}API: "
#define _ERROR_HEADER       "{SVCTX:STRM_FMT_CHKER}Err: "
/*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * private variable declarations
 *---------------------------------------------------------------------------*/
static DLIST_T(_STRM_FMT_CHKER_T) t_g_using_strm_fmt_chker_q;
static DLIST_T(_STRM_FMT_CHKER_T) t_g_pooled_strm_fmt_chker_q;
static STRM_FMT_CHKER_T*          pt_g_strm_fmt_chker_objs    = NULL;
static BOOL                       b_g_init                    = FALSE;
/*-----------------------------------------------------------------------------
 * private methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      _strm_fmt_chker_valid
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static BOOL _strm_fmt_chker_valid (
                    STRM_FMT_CHKER_T*           pt_chker
                    )
{
    STRM_FMT_CHKER_T* pt_chkee;

    if (pt_chker == NULL) {
        return FALSE;
    }

    DLIST_FOR_EACH(pt_chkee, &t_g_using_strm_fmt_chker_q, t_dlink) {
        if (pt_chkee == pt_chker) {
            return TRUE;
        }
    }

    DBG_INFO((_INFO_HEADER"%s() - pt_chker=0x%08x not existed\r\n ", __func__, pt_chker));

    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _strm_fmt_chker_internal_msg
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _strm_fmt_chker_internal_msg (
                    VOID*                       pv_tag,
                    SVCTX_MSG_T*                pt_msg
                    )
{
    STRM_FMT_CHKER_T* pt_this = (STRM_FMT_CHKER_T*) pv_tag;
    INT32             i4_ret  = SVCTXR_OK;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_EXIST;
    }
    
    if (_strm_fmt_chker_valid(pt_this) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s(), invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_NOT_EXIST;
    }
    
    switch(pt_msg->ui1_msg_id) {
    case _STRM_FMT_CHKER_MSG_VIDEO_DELAY_NTFY:
        if (pt_this->h_video_timer != pt_msg->u.t_timer.h_obj) {
            return SVCTXR_OK;
        }

        pt_this->pf_nty(
                    pt_this,
                    STRM_FMT_CHKER_NTFY_CODE_DELAY_TIMEOUT_MSG,
                    pt_this->pv_tag,
                    (UINT32)ST_VIDEO
                    );
        break;
    case _STRM_FMT_CHKER_MSG_AUDIO_DELAY_NTFY:
        if (pt_this->h_audio_timer != pt_msg->u.t_timer.h_obj) {
            return SVCTXR_OK;
        }

        pt_this->pf_nty(
                    pt_this,
                    STRM_FMT_CHKER_NTFY_CODE_DELAY_TIMEOUT_MSG,
                    pt_this->pv_tag,
                    (UINT32)ST_AUDIO
                    );
        break;
    default:
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _strm_fmt_chker_video_fmt_timeout_nfy
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static VOID _strm_fmt_chker_video_fmt_timeout_nfy(
                    HANDLE_T                    h_timer,
                    VOID*                       pv_tag
                    )
{
    STRM_FMT_CHKER_T* pt_this = (STRM_FMT_CHKER_T*) pv_tag;
    SVCTX_MSG_T       t_msg   = {0};

    t_msg.ui1_msg_id      = _STRM_FMT_CHKER_MSG_VIDEO_DELAY_NTFY;
    t_msg.u.t_timer.h_obj = h_timer;
    t_msg.pf_invoke_func  = _strm_fmt_chker_internal_msg;
    t_msg.pv_tag          = pv_tag;

    pt_this->pf_nty(
                    pt_this,
                    STRM_FMT_CHKER_NTFY_CODE_REQ_SEND_MSG,
                    pt_this->pv_tag,
                    (UINT32)&t_msg
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      _strm_fmt_chker_audio_fmt_timeout_nfy
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static VOID _strm_fmt_chker_audio_fmt_timeout_nfy(
                    HANDLE_T                    h_timer,
                    VOID*                       pv_tag
                    )
{
    STRM_FMT_CHKER_T* pt_this = (STRM_FMT_CHKER_T*) pv_tag;
    SVCTX_MSG_T       t_msg   = {0};

    t_msg.ui1_msg_id      = _STRM_FMT_CHKER_MSG_AUDIO_DELAY_NTFY;
    t_msg.u.t_timer.h_obj = h_timer;
    t_msg.pf_invoke_func  = _strm_fmt_chker_internal_msg;
    t_msg.pv_tag          = pv_tag;

    pt_this->pf_nty(
                    pt_this,
                    STRM_FMT_CHKER_NTFY_CODE_REQ_SEND_MSG,
                    pt_this->pv_tag,
                    (UINT32)&t_msg
                    );
}
 /*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: strm_fmt_chker_init
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *-----------------------------------------------------------------------------*/
INT32 strm_fmt_chker_init(
                    UINT16                      ui2_amount
                    )
{
    UINT16 ui2_idx;
    STRM_FMT_CHKER_T* pt_objs = NULL;

    if (b_g_init == TRUE) {
        DBG_ERROR((_ERROR_HEADER"%s(), already init\r\n", __func__));
        return SVCTXR_OK;
    }

    DBG_API((_API_HEADER"%s("
                    "ui2_amount=%d, "
                    "each size=%d)\r\n",
                    __func__,
                    ui2_amount,
                    sizeof(STRM_FMT_CHKER_T)
                    ));

    pt_objs = (STRM_FMT_CHKER_T*) x_mem_alloc(sizeof(STRM_FMT_CHKER_T) * ui2_amount);
    if(pt_objs == NULL) {
        DBG_ERROR((_ERROR_HEADER"create memory for STRM_FMT_CHKER_T pool failed\r\n"));
        return SVCTXR_FAIL;
    }

    x_memset(pt_objs, 0, sizeof(STRM_FMT_CHKER_T) * ui2_amount);

    DLIST_INIT(&t_g_using_strm_fmt_chker_q);
    DLIST_INIT(&t_g_pooled_strm_fmt_chker_q);

    for (ui2_idx = 0; ui2_idx < ui2_amount; ui2_idx++) {
        STRM_FMT_CHKER_T* pt_strm_fmt_chker = &(pt_objs[ui2_idx]);

        DLIST_INSERT_HEAD(pt_strm_fmt_chker, &t_g_pooled_strm_fmt_chker_q, t_dlink);
    }

    pt_g_strm_fmt_chker_objs = pt_objs;

    b_g_init = TRUE;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: strm_fmt_chker_deinit
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *-----------------------------------------------------------------------------*/
VOID strm_fmt_chker_deinit(
                    VOID
                    )
{
    if (b_g_init == FALSE) {
        return;
    }

    x_mem_free(pt_g_strm_fmt_chker_objs);

    DLIST_INIT(&t_g_using_strm_fmt_chker_q);
    DLIST_INIT(&t_g_pooled_strm_fmt_chker_q);

    pt_g_strm_fmt_chker_objs = NULL;
    b_g_init                 = FALSE;
}
 /*-----------------------------------------------------------------------------
 * Name
 *      strm_fmt_chker_open
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 strm_fmt_chker_open(
                    UINT32                      ui4_delay,
                    strm_fmt_chk_nty_fct        pf_nty,
                    VOID*                       pv_tag,
                    STRM_FMT_CHKER_T**          ppt_this
                    )
{
    STRM_FMT_CHKER_T* pt_this;
    INT32             i4_ret;

    if (b_g_init == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s() failed, not init\r\n", __func__));
        return SVCTXR_NOT_INIT;
    }

    if (ppt_this == NULL || pf_nty == NULL || ui4_delay == 0) {
        return SVCTXR_INV_ARG;
    }

    do {
        /* check the pool is empty or not */
        pt_this = DLIST_HEAD(&t_g_pooled_strm_fmt_chker_q);
        if (pt_this == NULL) {
            DBG_ERROR((_ERROR_HEADER"%s() failed! ret=SVCTXR_NO_RESOURCE\r\n", __func__));
            i4_ret = SVCTXR_NO_RESOURCE;
            break;
        }

        /* move the instance from pooled to used */
        DLIST_REMOVE(pt_this, &t_g_pooled_strm_fmt_chker_q, t_dlink);
        DLIST_INSERT_HEAD(pt_this, &t_g_using_strm_fmt_chker_q, t_dlink);

        pt_this->h_video_timer         = NULL_HANDLE;
        pt_this->h_audio_timer         = NULL_HANDLE;
        pt_this->ui4_strm_format_cond  = 0;
        pt_this->ui4_strm_present_cond = STRM_PRESENT_COND_INIT_VAL;
        pt_this->ui4_delay             = ui4_delay;
        pt_this->pf_nty                = pf_nty;
        pt_this->pv_tag                = pv_tag;

        i4_ret = x_timer_create(&pt_this->h_video_timer);
        if (i4_ret != OSR_OK) {
            DBG_ERROR((_ERROR_HEADER"create h_video_timer failed! ret=%d\r\n", i4_ret));
            i4_ret = SVCTXR_FAIL;
            break;
        }

        i4_ret = x_timer_create(&pt_this->h_audio_timer);
        if (i4_ret != OSR_OK) {
            DBG_ERROR((_ERROR_HEADER"create h_audio_timer failed! ret=%d\r\n", i4_ret));
            i4_ret = SVCTXR_FAIL;
            break;
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

        i4_ret = strm_fmt_chker_close(pt_this);
        if (i4_ret != SVCTXR_OK) {
            DBG_ERROR((_ERROR_HEADER"%s(), strm_fmt_chker_close() failed! ret=%d\r\n", __func__, i4_ret));
        }
    }

    *ppt_this = NULL;
    return SVCTXR_FAIL;
}
/*-----------------------------------------------------------------------------
 * Name
 *      strm_fmt_chker_close
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 strm_fmt_chker_close(
                    STRM_FMT_CHKER_T*           pt_this
                    )
{
    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_strm_fmt_chker_valid(pt_this) == FALSE) {
        DBG_INFO((_INFO_HEADER"%s(), invalid strm fmt chker = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_OK;
    }

    if (x_handle_valid(pt_this->h_video_timer) == TRUE) {
        x_timer_delete(pt_this->h_video_timer);
    }
    if (x_handle_valid(pt_this->h_audio_timer) == TRUE) {
        x_timer_delete(pt_this->h_audio_timer);
    }

    pt_this->h_video_timer         = NULL_HANDLE;
    pt_this->h_audio_timer         = NULL_HANDLE;
    pt_this->ui4_strm_format_cond  = 0;
    pt_this->ui4_strm_present_cond = STRM_PRESENT_COND_INIT_VAL;
    
    DLIST_REMOVE(pt_this, &t_g_using_strm_fmt_chker_q, t_dlink);
    DLIST_INSERT_HEAD(pt_this, &t_g_pooled_strm_fmt_chker_q, t_dlink);
    
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      strm_fmt_chker_reset
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 strm_fmt_chker_reset(
                    STRM_FMT_CHKER_T*           pt_this
                    )
{
    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_strm_fmt_chker_valid(pt_this) == FALSE) {
        return SVCTXR_INV_ARG;
    }

    x_timer_stop(pt_this->h_video_timer);
    x_timer_stop(pt_this->h_audio_timer);
    pt_this->ui4_strm_format_cond  = 0;
    pt_this->ui4_strm_present_cond = STRM_PRESENT_COND_INIT_VAL;
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      strm_fmt_chker_start_delay
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 strm_fmt_chker_start_delay(
                    STRM_FMT_CHKER_T*           pt_this,
                    STREAM_TYPE_T               e_strm_type
                    )
{
    INT32 i4_ret;
    UINT32      ui4_delay;
#ifdef MW_PIP_POP_SUPPORT
    SVCTX_T*    pt_svctx = NULL;
#endif

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_strm_fmt_chker_valid(pt_this) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s() failed, invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_INV_ARG;
    }

#ifdef MW_PIP_POP_SUPPORT
    if ((pt_svctx=(SVCTX_T*)pt_this->pv_tag)==NULL)
    {
        return SVCTXR_INV_ARG;
    }
#endif

    switch(e_strm_type) {
    case ST_VIDEO:
        i4_ret = x_timer_start(
                    pt_this->h_video_timer,
                    pt_this->ui4_delay,
                    X_TIMER_FLAG_ONCE,
                    _strm_fmt_chker_video_fmt_timeout_nfy,
                    (VOID*)pt_this
                    );
        i4_ret = (i4_ret != OSR_OK) ? SVCTXR_FAIL : SVCTXR_OK;
        break;
    case ST_AUDIO:
#ifdef MW_PIP_POP_SUPPORT
        if (svctx_is_pap(pt_svctx) && (pt_svctx->e_av_status == SVC_AV_STATUS_NO_AUDIO_VIDEO) 
#ifdef MW_TV_AV_BYPASS_SUPPORT
            && (svctx_mgr_convert_to_svc_type(pt_svctx->pt_running_svc_req->e_src_type) == SVC_SELECT_TV)
#endif            
            )
        {            
            ui4_delay = 1500;
        }                
        else
        {            
            ui4_delay = pt_this->ui4_delay;
        }            
#else                    
        ui4_delay = pt_this->ui4_delay;
#endif       
        
        i4_ret = x_timer_start(
                    pt_this->h_audio_timer,
                    ui4_delay,
                    X_TIMER_FLAG_ONCE,
                    _strm_fmt_chker_audio_fmt_timeout_nfy,
                    (VOID*)pt_this
                    );
        i4_ret = (i4_ret != OSR_OK) ? SVCTXR_FAIL : SVCTXR_OK;
        break;
    default:
        i4_ret = SVCTXR_NOT_SUPPORT;
        break;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      strm_fmt_chker_start_delay
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 strm_fmt_chker_stop_delay(
                    STRM_FMT_CHKER_T*           pt_this,
                    STREAM_TYPE_T               e_strm_type
                    )
{
    INT32 i4_ret = SVCTXR_OK;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_strm_fmt_chker_valid(pt_this) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s() failed, invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_INV_ARG;
    }

    switch(e_strm_type) {
        case ST_VIDEO:
            x_timer_stop(pt_this->h_video_timer);
            DBG_INFO((_INFO_HEADER"stop video format checker timer.\n"));
            break;
        case ST_AUDIO:
            x_timer_stop(pt_this->h_audio_timer);
            DBG_INFO((_INFO_HEADER"stop audio format checker timer.\n"));
            break;
        default:
            i4_ret = SVCTXR_NOT_SUPPORT;
            break;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      strm_fmt_chker_get_format_status
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
UINT32 strm_fmt_chker_get_format_status(
                    STRM_FMT_CHKER_T*           pt_this
                    )
{
    if (b_g_init == FALSE) {
        return 0;
    }

    if (_strm_fmt_chker_valid(pt_this) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s() failed, invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return 0;
    }

    return pt_this->ui4_strm_format_cond;
}
/*-----------------------------------------------------------------------------
 * Name
 *      strm_fmt_chker_reset_format_status
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 strm_fmt_chker_reset_format_status(
                    STRM_FMT_CHKER_T*           pt_this,
                    STREAM_TYPE_T               e_strm_type
                    )
{
    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_strm_fmt_chker_valid(pt_this) == FALSE) {
        return SVCTXR_INV_ARG;
    }

    pt_this->ui4_strm_format_cond &= ~MAKE_BIT_MASK_32(e_strm_type);
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      strm_fmt_chker_set_format_status
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 strm_fmt_chker_set_format_status(
                    STRM_FMT_CHKER_T*           pt_this,
                    STREAM_TYPE_T               e_strm_type
                    )
{
    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_strm_fmt_chker_valid(pt_this) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s() failed, invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_INV_ARG;
    }

    pt_this->ui4_strm_format_cond |= MAKE_BIT_MASK_32(e_strm_type);
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      strm_fmt_chker_get_format_status
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
UINT32 strm_fmt_chker_get_present_status(
                    STRM_FMT_CHKER_T*           pt_this
                    )
{
    if (b_g_init == FALSE) {
        return 0;
    }

    if (_strm_fmt_chker_valid(pt_this) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s() failed, invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return 0;
    }

    return pt_this->ui4_strm_present_cond;
}
/*-----------------------------------------------------------------------------
 * Name
 *      strm_fmt_chker_set_present_status
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 strm_fmt_chker_set_present_status(
                    STRM_FMT_CHKER_T*           pt_this,
                    STREAM_TYPE_T               e_strm_type
                    )
{
    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_strm_fmt_chker_valid(pt_this) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s() failed, invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_INV_ARG;
    }

    pt_this->ui4_strm_present_cond |= MAKE_BIT_MASK_32(e_strm_type);
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      strm_fmt_chker_init_present_status
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 strm_fmt_chker_init_present_status(
                    STRM_FMT_CHKER_T*           pt_this
                    )
{
    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_strm_fmt_chker_valid(pt_this) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s() failed, invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_INV_ARG;
    }

    pt_this->ui4_strm_present_cond = STRM_PRESENT_COND_INIT_VAL;
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      strm_fmt_chker_force_present_status
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 strm_fmt_chker_force_present_status(
                    STRM_FMT_CHKER_T*           pt_this,
                    UINT32                      ui4_strm_present_cond
                    )
{
    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_strm_fmt_chker_valid(pt_this) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s() failed, invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_INV_ARG;
    }

    pt_this->ui4_strm_present_cond = ui4_strm_present_cond;
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      strm_fmt_chker_reset_format_status
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 strm_fmt_chker_reset_present_status(
                    STRM_FMT_CHKER_T*           pt_this,
                    STREAM_TYPE_T               e_strm_type
                    )
{
    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_strm_fmt_chker_valid(pt_this) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s() failed, invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_INV_ARG;
    }

    pt_this->ui4_strm_present_cond &= ~MAKE_BIT_MASK_32(e_strm_type);
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      strm_fmt_chker_get_using_head
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
STRM_FMT_CHKER_T* strm_fmt_chker_get_using_head(
                    VOID
                    )
{
    return DLIST_HEAD(&t_g_using_strm_fmt_chker_q);
}
/*-----------------------------------------------------------------------------
 * Name
 *      strm_fmt_chker_get_pooled_head
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
STRM_FMT_CHKER_T* strm_fmt_chker_get_pooled_head(
                    VOID
                    )
{
    return DLIST_HEAD(&t_g_pooled_strm_fmt_chker_q);
}
/*-----------------------------------------------------------------------------
 * Name
 *      strm_fmt_chker_next
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
STRM_FMT_CHKER_T* strm_fmt_chker_next(
                    STRM_FMT_CHKER_T*           pt_this
                    )
{
    if (b_g_init == FALSE || pt_this == NULL ) {
        return NULL;
    }

    return (DLIST_NEXT(pt_this, t_dlink));
}
/*-----------------------------------------------------------------------------
 * Name
 *      strm_fmt_chker_print_self
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
VOID strm_fmt_chker_print_self(
                    STRM_FMT_CHKER_T*             pt_this
                    )
{
#ifdef CLI_LVL_ALL
    x_dbg_stmt("------------------------------------ size = %d\r\n", sizeof(STRM_FMT_CHKER_T));
    x_dbg_stmt("pt_strm_fmt_chker     - 0x%x\r\n", pt_this);
    if (pt_this != NULL) {
        x_dbg_stmt("ui4_strm_format_cond  - 0x%x\r\n", pt_this->ui4_strm_format_cond);
        x_dbg_stmt("ui4_strm_present_cond - 0x%x\r\n", pt_this->ui4_strm_present_cond);
        x_dbg_stmt("h_video_timer         - %d\r\n",   pt_this->h_video_timer);
        x_dbg_stmt("h_audio_timer         - %d\r\n",   pt_this->h_audio_timer);
        x_dbg_stmt("ui4_delay             - %d\r\n",   pt_this->ui4_delay);    
        x_dbg_stmt("pf_nty                - 0x%x\r\n", pt_this->pf_nty);
        x_dbg_stmt("pv_tag                - 0x%x\r\n", pt_this->pv_tag);
    }
    x_dbg_stmt("====================================\r\n");
#endif
}
