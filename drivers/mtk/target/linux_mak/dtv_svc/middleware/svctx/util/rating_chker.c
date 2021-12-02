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
 * $RCSfile: rating_chker.c,v $
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
#include "svctx/util/rating_chker.h"
#include "svctx/svctx.h"
#include "svctx/svctx_dbg.h"
#include "handle/handle.h"
#include "svl/x_svl.h"
/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#define _RATING_CHKER_MSG_BLOCK_DELAY_NTFY  ((UINT8)220)
#define _RATING_CHKER_MSG_SVL_UPDATE_NTFY   ((UINT8)221)

#undef  _INFO_HEADER
#undef  _API_HEADER
#undef  _ERROR_HEADER
#define _INFO_HEADER        "{SVCTX:RATING_CHKER}Info: "
#define _API_HEADER         "{SVCTX:RATING_CHKER}API: "
#define _ERROR_HEADER       "{SVCTX:RATING_CHKER}Err: "

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
/*-----------------------------------------------------------------------------
 * private variable declarations
 *---------------------------------------------------------------------------*/
static DLIST_T(_RATING_CHKER_T)   t_g_using_rating_chker_q;
static DLIST_T(_RATING_CHKER_T)   t_g_pooled_rating_chker_q;
static RATING_CHKER_T*            pt_g_rating_chker_objs = NULL;
static BOOL                       b_g_init               = FALSE;
/*-----------------------------------------------------------------------------
 * private methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      _rating_chker_valid
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static BOOL _rating_chker_valid (
                    RATING_CHKER_T*             pt_chker
                    )
{
    RATING_CHKER_T* pt_chkee;

    if (pt_chker == NULL) {
        return FALSE;
    }

    DLIST_FOR_EACH(pt_chkee, &t_g_using_rating_chker_q, t_dlink) {
        if (pt_chkee == pt_chker) {
            return TRUE;
        }
    }

    DBG_INFO((_INFO_HEADER"%s() - pt_chker=0x%08x not existed\r\n ", __func__, pt_chker));

    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _rating_chker_handle_delay_timeout
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _rating_chker_handle_delay_timeout (
                    RATING_CHKER_T*             pt_this,
                    HANDLE_T                    h_timer
                    )
{
    if (h_timer != pt_this->h_block_timer) {
        return SVCTXR_OK;
    }

    if (pt_this->b_block_svc_delay_on == FALSE) {
        return SVCTXR_OK;
    }

    if (pt_this->e_block_cond != SVC_BLOCK_COND_PASSED
            && pt_this->e_block_cond != SVC_BLOCK_COND_PASSED_AND_NO_RATING_SET) {
        return SVCTXR_OK;
    }

    pt_this->b_block_svc_delay_on = FALSE;

    pt_this->pf_nty(
                    pt_this,
                    RATING_CHKER_NTFY_CODE_DELAY_TIMEOUT_MSG,
                    pt_this->pv_tag,
                    (UINT32)NULL
                    );
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _rating_chker_internal_msg
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _rating_chker_internal_msg (
                    VOID*                       pv_tag,
                    SVCTX_MSG_T*                pt_msg
                    )
{
    RATING_CHKER_T* pt_this = (RATING_CHKER_T*) pv_tag;
    INT32           i4_ret  = SVCTXR_OK;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_EXIST;
    }

    if (_rating_chker_valid(pt_this) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s(), invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_NOT_EXIST;
    }

    switch(pt_msg->ui1_msg_id) {
    case _RATING_CHKER_MSG_BLOCK_DELAY_NTFY:
        i4_ret = _rating_chker_handle_delay_timeout(
                    pt_this,
                    pt_msg->u.t_timer.h_obj
                    );
        break;
    case _RATING_CHKER_MSG_SVL_UPDATE_NTFY:
        i4_ret = pt_this->pf_nty(
                    pt_this,
                    RATING_CHKER_NTFY_CODE_SVL_UPDATE,
                    pt_this->pv_tag,
                    (UINT32)pt_msg
                    );
        break;
    default:
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _rating_chker_block_delay_nfy_fct
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static VOID _rating_chker_block_delay_nfy_fct(
                    HANDLE_T                    h_timer,
                    VOID*                       pv_tag
                    )
{
    RATING_CHKER_T* pt_this = (RATING_CHKER_T*) pv_tag;
    SVCTX_MSG_T     t_msg   = {0};

    t_msg.ui1_msg_id      = _RATING_CHKER_MSG_BLOCK_DELAY_NTFY;
    t_msg.u.t_timer.h_obj = h_timer;
    t_msg.pf_invoke_func  = _rating_chker_internal_msg;
    t_msg.pv_tag          = pv_tag;

    pt_this->pf_nty(
                    pt_this,
                    RATING_CHKER_NTFY_CODE_REQ_SEND_MSG,
                    pt_this->pv_tag,
                    (UINT32)&t_msg
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      _rating_chker_svl_nfy_fct
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static VOID _rating_chker_svl_nfy_fct(
                    HANDLE_T                    h_svl,
                    SVL_COND_T                  e_cond,
                    UINT32                      ui4_reason,
                    VOID*                       pv_tag,
                    UINT32                      ui4_data
                    )
{
    RATING_CHKER_T* pt_this = (RATING_CHKER_T*) pv_tag;
    SVCTX_MSG_T     t_msg = {0};

    if (e_cond == SVL_UPDATING || 
            (e_cond == SVL_UPDATED && ui4_reason == SVL_REASON_UNKNOWN)) {
        return;
    }
    
    t_msg.ui1_msg_id     = _RATING_CHKER_MSG_SVL_UPDATE_NTFY;
    t_msg.u.t_svl.h_obj  = h_svl;
    t_msg.pf_invoke_func = _rating_chker_internal_msg;
    t_msg.pv_tag         = pv_tag;

    pt_this->pf_nty(
                    pt_this,
                    RATING_CHKER_NTFY_CODE_REQ_SEND_MSG,
                    pt_this->pv_tag,
                    (UINT32)&t_msg
                    );
}
 /*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: rating_chker_init
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *-----------------------------------------------------------------------------*/
INT32 rating_chker_init(
                    UINT16                      ui2_amount
                    )
{
    UINT16 ui2_idx;

    if (b_g_init == TRUE) {
        return SVCTXR_OK;
    }

    DBG_API((_API_HEADER"%s("
                    "ui2_amount=%d, "
                    "each size=%d)\r\n",
                    __func__,
                    ui2_amount,
                    sizeof(RATING_CHKER_T)
                    ));

    if(ui2_amount == 0){
          DBG_ERROR((_ERROR_HEADER"create memory for RATING_CHKER_T pool failed for ui2_amount = 0\r\n"));
          return SVCTXR_FAIL; 
      }

    pt_g_rating_chker_objs = (RATING_CHKER_T*) x_mem_alloc(sizeof(RATING_CHKER_T) * ui2_amount);
    if(pt_g_rating_chker_objs == NULL) {
        DBG_ERROR((_ERROR_HEADER"create memory for RATING_CHKER_T pool failed\r\n"));
        return SVCTXR_FAIL;
    }

    x_memset(pt_g_rating_chker_objs, 0, sizeof(RATING_CHKER_T) * ui2_amount);

    DLIST_INIT(&t_g_using_rating_chker_q);
    DLIST_INIT(&t_g_pooled_rating_chker_q);

    for (ui2_idx = 0; ui2_idx < ui2_amount; ui2_idx++) {
        RATING_CHKER_T* pt_rating_chker = &(pt_g_rating_chker_objs[ui2_idx]);

        DLIST_INSERT_HEAD(pt_rating_chker, &t_g_pooled_rating_chker_q, t_dlink);
    }

    b_g_init = TRUE;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: rating_chker_deinit
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *-----------------------------------------------------------------------------*/
VOID rating_chker_deinit(
                    VOID
                    )
{
    if (b_g_init == FALSE) {
        return;
    }

    x_mem_free(pt_g_rating_chker_objs);

    DLIST_INIT(&t_g_using_rating_chker_q);
    DLIST_INIT(&t_g_pooled_rating_chker_q);

    pt_g_rating_chker_objs = NULL;
    b_g_init               = FALSE;
}
 /*-----------------------------------------------------------------------------
 * Name
 *      rating_chker_open
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 rating_chker_open(
#ifdef SVCTX_SPLIT_SUPPORT
                    svctx_svc_block_check_fct   pf_block_check_fct,
                    VOID*                       pf_block_check_fct_tag,
#else
                    x_svctx_svc_block_check_fct pf_block_check_fct,
#endif
                    UINT32                      ui4_block_delay,
                    rating_chk_nty_fct          pf_nty,
                    VOID*                       pv_tag,
                    RATING_CHKER_T**            ppt_this
                    )
{
    RATING_CHKER_T* pt_this;
    INT32           i4_ret;

    DBG_API((_API_HEADER"%s("
                    "ui4_block_delay = %d "
                    ")\r\n",
                    __func__,
                    ui4_block_delay
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (ppt_this == NULL || pf_nty == NULL || pf_block_check_fct == NULL) {
        return SVCTXR_INV_ARG;
    }

    do {
        /* check the pool is empty or not */
        pt_this = DLIST_HEAD(&t_g_pooled_rating_chker_q);
        if (pt_this == NULL) {
            DBG_ERROR((_ERROR_HEADER"%s() failed! ret=SVCTXR_NO_RESOURCE\r\n", __func__));
            i4_ret = SVCTXR_NO_RESOURCE;
            break;
        }

        /* move the instance from pooled to used */
        DLIST_REMOVE(pt_this, &t_g_pooled_rating_chker_q, t_dlink);
        DLIST_INSERT_HEAD(pt_this, &t_g_using_rating_chker_q, t_dlink);

        pt_this->h_block_timer        = NULL_HANDLE;
        pt_this->h_svl                = NULL_HANDLE;
        pt_this->ui4_block_delay      = ui4_block_delay;
        pt_this->b_block_svc_delay_on = FALSE;
        pt_this->e_block_cond         = SVC_BLOCK_COND_PASSED;
        pt_this->pf_block_check_fct   = pf_block_check_fct;
#ifdef SVCTX_SPLIT_SUPPORT
        pt_this->pf_block_check_fct_tag = pf_block_check_fct_tag;
#endif
        pt_this->pf_nty               = pf_nty;
        pt_this->pv_tag               = pv_tag;

        if (pt_this->ui4_block_delay > 0) {
            i4_ret = x_timer_create(&pt_this->h_block_timer);
            if (i4_ret != OSR_OK) {
                DBG_ERROR((_ERROR_HEADER"%s(), create h_block_timer failed! ret=%d\r\n", __func__, i4_ret));
                i4_ret = SVCTXR_FAIL;
                break;
            }
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

        i4_ret = rating_chker_close(pt_this);
        if (i4_ret != SVCTXR_OK) {
            DBG_ERROR((_ERROR_HEADER"%s(), rating_chker_close() failed! ret=%d\r\n", __func__, i4_ret));
        }
    }

    *ppt_this = NULL;
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      rating_chker_close
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 rating_chker_close(
                    RATING_CHKER_T*             pt_this
                    )
{
    DBG_API((_API_HEADER"%s("
                    "pt_this = 0x%x "
                    ")\r\n",
                    __func__,
                    pt_this
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_rating_chker_valid(pt_this) == FALSE) {
        DBG_INFO((_INFO_HEADER"%s(), invalid rating_chker = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_OK;
    }

    if (x_handle_valid(pt_this->h_svl) == TRUE) {
        x_svl_close(pt_this->h_svl);
    }

    if (x_handle_valid(pt_this->h_block_timer) == TRUE) {
        x_timer_delete(pt_this->h_block_timer);
    }

    pt_this->h_svl         = NULL_HANDLE;
    pt_this->h_block_timer = NULL_HANDLE;

    DLIST_REMOVE(pt_this, &t_g_using_rating_chker_q, t_dlink);
    DLIST_INSERT_HEAD(pt_this, &t_g_pooled_rating_chker_q, t_dlink);

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      rating_chker_stop_delay
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 rating_chker_stop_delay(
                    RATING_CHKER_T*             pt_this
                    )
{
    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_rating_chker_valid(pt_this) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s() invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_INV_ARG;
    }

    if (pt_this->ui4_block_delay == 0) {
        return SVCTXR_NOT_SUPPORT;
    }

    if (pt_this->b_block_svc_delay_on == TRUE) {
        x_timer_stop(pt_this->h_block_timer);
        pt_this->b_block_svc_delay_on = FALSE;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      rating_chker_start_delay
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 rating_chker_start_delay(
                    RATING_CHKER_T*             pt_this
                    )
{
    INT32 i4_ret;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_rating_chker_valid(pt_this) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s() invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_INV_ARG;
    }

    if (pt_this->ui4_block_delay == 0) {
        return SVCTXR_NOT_SUPPORT;
    }

    i4_ret = x_timer_stop(pt_this->h_block_timer);
    if (i4_ret != OSR_OK) {
        DBG_ERROR((_ERROR_HEADER"%s() x_timer_stop() failed, i4_ret=%d\r\n", __func__, i4_ret));
    }
    pt_this->b_block_svc_delay_on = FALSE;

    if (pt_this->ui4_block_delay > 0) {
        i4_ret = x_timer_start(
                        pt_this->h_block_timer,
                        pt_this->ui4_block_delay,
                        X_TIMER_FLAG_ONCE,
                        _rating_chker_block_delay_nfy_fct,
                        (VOID*)pt_this
                        );
        if (i4_ret != OSR_OK) {
            return SVCTXR_FAIL;
        }

        pt_this->b_block_svc_delay_on = TRUE;
    }
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      rating_chker_open_svl
 * Description
 *
 * Input arguments
 *      pt_svctx        the pointer points to an SVCTX_T object
 *      pt_msg          the pointer points to an SVCTX_MSG_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK       the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 rating_chker_open_svl(
                    RATING_CHKER_T*             pt_this,
                    UINT16                      ui2_svc_id
                    )
{
    INT32  i4_ret;
    UINT16 ui2_crnt_svl_id;
    CHAR   s_crnt_svl_name[MAX_SVL_NAME_LEN+1];

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_rating_chker_valid(pt_this) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s() invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_INV_ARG;
    }

    if (x_handle_valid(pt_this->h_svl) == TRUE) {
        i4_ret = x_svl_get_handle_info(
                    pt_this->h_svl,
                    &ui2_crnt_svl_id,
                    s_crnt_svl_name
                    );
        if (i4_ret != SVLR_OK) {
            return SVCTXR_FAIL;
        }

        if (ui2_crnt_svl_id == ui2_svc_id) {
            return SVCTXR_OK;
        }

        i4_ret = x_svl_close(pt_this->h_svl);
        if (i4_ret != SVLR_OK) {
            return SVCTXR_FAIL;
        }
    }

    i4_ret = x_svl_open(
                    ui2_svc_id,
                    (VOID*)pt_this,
                    _rating_chker_svl_nfy_fct,
                    &pt_this->h_svl
                    );
    if (i4_ret != SVLR_OK) {
        return SVCTXR_FAIL;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      rating_chker_get_svl_handle
 * Description
 *
 * Input arguments
 *      pt_this         the pointer points to an RATING_CHKER_T object
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
HANDLE_T rating_chker_get_svl_handle(
                    RATING_CHKER_T*             pt_this
                    )
{
    if (b_g_init == FALSE) {
        return NULL_HANDLE;
    }

    if (_rating_chker_valid(pt_this) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s() failed, invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return NULL_HANDLE;
    }

    return pt_this->h_svl;
}
/*-----------------------------------------------------------------------------
 * Name
 *      rating_chker_set_block_cond
 * Description
 *
 * Input arguments
 *      pt_this         the pointer points to an RATING_CHKER_T object
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
INT32 rating_chker_set_block_cond(
                    RATING_CHKER_T*             pt_this,
                    SVC_BLOCK_COND_T            e_block_cond
                    )
{
    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_rating_chker_valid(pt_this) == FALSE) {
        DBG_INFO((_INFO_HEADER"%s() invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_INV_ARG;
    }

    pt_this->e_block_cond = e_block_cond;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      rating_chker_get_block_cond
 * Description
 *
 * Input arguments
 *      pt_this         the pointer points to an RATING_CHKER_T object
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
SVC_BLOCK_COND_T rating_chker_get_block_cond(
                    RATING_CHKER_T*             pt_this
                    )
{
    if (b_g_init == FALSE) {
        return SVC_BLOCK_COND_PASSED;
    }

    if (_rating_chker_valid(pt_this) == FALSE) {
        DBG_INFO((_INFO_HEADER"%s() failed, invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return SVC_BLOCK_COND_PASSED;
    }

    return (pt_this->e_block_cond);
}
/*-----------------------------------------------------------------------------
 * Name
 *      rating_chker_check
 * Description
 *
 * Input arguments
 *      pt_svctx        the pointer points to an SVCTX_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK       the routine is successfully
 *-----------------------------------------------------------------------------*/
SVC_BLOCK_COND_T rating_chker_check(
                    RATING_CHKER_T*             pt_this,
                    CHAR*                       ps_src_grp_name,
                    UINT16                      ui2_svl_rec_id,
                    SVC_BLOCK_CHECK_GUDIE_T     e_check_guide,
                    VOID*                       pv_rating
                    )
{
    SVC_BLOCK_COND_T e_block_cond = SVC_BLOCK_COND_PASSED;
    UINT32           ui4_ver_id   = SVL_NULL_VER_ID;
    INT32            i4_ret       = SVLR_OK;
    SVL_REC_T        t_svl_rec    = {0};

    do {
        if (b_g_init == FALSE) {
            break;
        }

        if (_rating_chker_valid(pt_this) == FALSE) {
            DBG_ERROR((_ERROR_HEADER"%s() failed, invalid pt_this = 0x%x\r\n", __func__, pt_this));
            break;
        }

        if (pt_this->pf_block_check_fct == NULL) {
            DBG_ERROR((_ERROR_HEADER"%s() failed, pf_block_check_fct is NULL\r\n", __func__));
            break;
        }

        if (ui2_svl_rec_id != SVL_NULL_REC_ID && x_handle_valid(pt_this->h_svl) == TRUE) {
            i4_ret = x_svl_get_rec (
                            pt_this->h_svl,
                            ui2_svl_rec_id,
                            &t_svl_rec,
                            &ui4_ver_id
                            );
            if (i4_ret != SVLR_OK) {
                DBG_ERROR((_ERROR_HEADER"x_svl_get_rec(h_svl=%d, ui2_svl_rec_id=%d) failed, i4_ret=%d\r\n",
                            pt_this->h_svl,
                            ui2_svl_rec_id,
                            i4_ret));
                break;
            }
        }

        pt_this->e_block_cond = e_block_cond = pt_this->pf_block_check_fct(
                    e_check_guide,
                    ps_src_grp_name,
                    &t_svl_rec,
                    pv_rating
#ifdef SVCTX_SPLIT_SUPPORT
                    ,pt_this->pf_block_check_fct_tag
#endif
                    );
    } while (0);

    return e_block_cond;
}
/*-----------------------------------------------------------------------------
 * Name
 *      rating_chker_get_using_head
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
RATING_CHKER_T* rating_chker_get_using_head(
                    VOID
                    )
{
    return DLIST_HEAD(&t_g_using_rating_chker_q);
}
/*-----------------------------------------------------------------------------
 * Name
 *      rating_chker_get_pooled_head
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
RATING_CHKER_T* rating_chker_get_pooled_head(
                    VOID
                    )
{
    return DLIST_HEAD(&t_g_pooled_rating_chker_q);
}
/*-----------------------------------------------------------------------------
 * Name
 *      rating_chker_next
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
RATING_CHKER_T* rating_chker_next(
                    RATING_CHKER_T*             pt_this
                    )
{
    if (b_g_init == FALSE || pt_this == NULL) {
        return NULL;
    }

    return (DLIST_NEXT(pt_this, t_dlink));
}
/*-----------------------------------------------------------------------------
 * Name
 *      rating_chker_print_self
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
VOID rating_chker_print_self(
                    RATING_CHKER_T*             pt_this
                    )
{
#ifdef CLI_LVL_ALL
    x_dbg_stmt("------------------------------------ size = %d\r\n", sizeof(RATING_CHKER_T));
    x_dbg_stmt("pt_rating_chker     - 0x%x\r\n", pt_this);
    if (pt_this != NULL) {
        x_dbg_stmt("h_block_timer       - %-10d\r\n", pt_this->h_block_timer);
        x_dbg_stmt("h_svl               - %-10d\r\n", pt_this->h_svl);
        x_dbg_stmt("ui4_block_delay     - %d\r\n", pt_this->ui4_block_delay);
        x_dbg_stmt("b_block_svc_delay_on- %s\r\n", (pt_this->b_block_svc_delay_on == TRUE ? "TRUE" : "FALSE"));
        x_dbg_stmt("e_block_cond        - %s\r\n", s_g_svc_block_conds[pt_this->e_block_cond]);
        x_dbg_stmt("pf_block_check_fct  - 0x%x\r\n", pt_this->pf_block_check_fct);
#ifdef SVCTX_SPLIT_SUPPORT
        x_dbg_stmt("pf_block_check_fct_tag  - 0x%x\r\n", pt_this->pf_block_check_fct_tag);
#endif
        x_dbg_stmt("pf_nty              - 0x%x\r\n", pt_this->pf_nty);
        x_dbg_stmt("pv_tag              - 0x%x\r\n", pt_this->pv_tag);
    }
    x_dbg_stmt("====================================\r\n");
#endif
}
