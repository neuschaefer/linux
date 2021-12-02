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
 * $RCSfile: ch_svc_req.c,v $
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
#include "svctx/ch_hdlr/ch_svc_req.h"
#include "svctx/svctx_dbg.h"
/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#undef  _INFO_HEADER
#undef  _API_HEADER
#undef  _ERROR_HEADER
#define _INFO_HEADER        "{SVCTX:CH_HDLR}Info: "
#define _API_HEADER         "{SVCTX:CH_HDLR}API:  "
#define _ERROR_HEADER       "{SVCTX:CH_HDLR}Err:  "

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
static BOOL                     b_g_svc_req_init       = FALSE;
static BOOL                     b_g_strm_req_init      = FALSE;
/* service request pool */
static DLIST_T(_CH_SVC_REQ_T)   t_g_using_svc_req_q;
static DLIST_T(_CH_SVC_REQ_T)   t_g_pooled_svc_req_q;
static CH_SVC_REQ_T*            pt_g_svc_req_objs      = NULL;

/* stream request pool */
static DLIST_T(_STRM_REQ_T)     t_g_using_strm_req_q;
static DLIST_T(_STRM_REQ_T)     t_g_pooled_strm_req_q;
static STRM_REQ_T*              pt_g_strm_req_objs     = NULL;

/*-----------------------------------------------------------------------------
 * private methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_svc_req_valid
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static BOOL _ch_svc_req_valid (
                    CH_SVC_REQ_T*               pt_chker
                    )
{
    CH_SVC_REQ_T* pt_chkee;

    if (pt_chker == NULL) {
        return FALSE;
    }

    DLIST_FOR_EACH(pt_chkee, &t_g_using_svc_req_q, t_dlink) {
        if (pt_chkee == pt_chker) {
            return TRUE;
        }
    }

    DBG_INFO((_INFO_HEADER"%s() - pt_chker=0x%08x not existed\r\n ", __func__, pt_chker));

    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_strm_req_valid
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static BOOL _ch_strm_req_valid (
                    STRM_REQ_T*                 pt_chker
                    )
{
    STRM_REQ_T* pt_chkee;

    if (pt_chker == NULL) {
        return FALSE;
    }

    DLIST_FOR_EACH(pt_chkee, &t_g_using_strm_req_q, t_dlink) {
        if (pt_chkee == pt_chker) {
            return TRUE;
        }
    }

    DBG_INFO((_INFO_HEADER"%s() - pt_chker=0x%08x not existed\r\n ", __func__, pt_chker));

    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _ch_svc_req_reset
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static VOID _ch_svc_req_reset(
                    CH_SVC_REQ_T*               pt_this
                    )
{
    UINT8 ui1_idx;
    
    if (pt_this == NULL) {
        return;
    }

    for (ui1_idx = 0; ui1_idx < (UINT8) STRM_TYPE_COUNT_WITH_AD; ui1_idx++) {
        pt_this->pt_pending_strm_reqs[ui1_idx] = NULL;
        svctx_reset_strm_req(&(pt_this->t_strm_reqs[ui1_idx]));
    }
    
    svctx_reset_svc_req(&(pt_this->t_parent));

    pt_this->t_src_info.s_src_grp_name[0] = '\0';
    pt_this->e_brdcst_type                = BRDCST_TYPE_UNKNOWN;
    pt_this->e_crnt_tuner_type            = DEV_UNKNOWN;
    pt_this->h_gl_plane                   = NULL_HANDLE;
    pt_this->t_cc_filter.e_cc_type        = SM_CC_TYPE_IGNORE;
    pt_this->pt_player                    = NULL;
    pt_this->pt_crnt_event                = NULL;
    pt_this->pt_rating_chker              = NULL;
    pt_this->pt_strm_fmt_chker            = NULL;
    pt_this->pt_scart_avm                 = NULL;
    pt_this->e_ch_src_type                = CH_SRC_TYPE_TUNER;
    pt_this->ui4_empty_strm_comp_cond     = 0;
    pt_this->ui4_exist_strm_comp_cond     = 0;
    pt_this->b_scramble_audio             = FALSE;
    pt_this->b_scramble_video             = FALSE;
}

static BOOL _ch_svc_req_same_digital_svc(
    CH_SVC_REQ_T* pt_chker, 
    CH_SVC_REQ_T* pt_chkee)
{
    CH_SRC_INFO_T* pt_src_info_chker = &(pt_chker->t_src_info);
    CH_SRC_INFO_T* pt_src_info_chkee = &(pt_chkee->t_src_info);

    if ((pt_src_info_chker->ui2_on_id  == pt_src_info_chkee->ui2_on_id) &&
        (pt_src_info_chker->ui2_ts_id  == pt_src_info_chkee->ui2_ts_id) &&
        (pt_src_info_chker->ui2_svc_id == pt_src_info_chkee->ui2_svc_id))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: ch_svc_req_init
 *
 * Description: This API initializes the Service Request Pool
 *
 * Inputs:  ui2_num_svc_reqs  Contains the maximum number of service request
 *                            instances supported by the Service Context Module.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_FAIL
 *-----------------------------------------------------------------------------*/
INT32 ch_svc_req_init(
                    UINT16                      ui2_amount
                    )
{
    UINT16        ui2_idx = 0;
    CH_SVC_REQ_T* pt_objs = NULL;

    DBG_API((_API_HEADER"%s("
                    "ui2_amount=%d, "
                    "each size=%d)\r\n",
                    __func__,
                    ui2_amount,
                    sizeof(CH_SVC_REQ_T)
                    ));

    if (b_g_svc_req_init == TRUE) {
        return SVCTXR_OK;
    }

    if(ui2_amount == 0){
        DBG_ERROR((_ERROR_HEADER"create memory for SVC_REQ_T failed for the ui2_amount is 0\r\n"));
        return SVCTXR_FAIL;
    }

    pt_objs = (CH_SVC_REQ_T*) x_mem_alloc( sizeof(CH_SVC_REQ_T) * ui2_amount);
    if(pt_objs == NULL) {
        DBG_ERROR((_ERROR_HEADER"create memory for SVC_REQ_T pool failed\r\n"));
        return SVCTXR_FAIL;
    }

    x_memset(pt_objs, 0, sizeof(CH_SVC_REQ_T) * ui2_amount);

    DLIST_INIT(&t_g_using_svc_req_q);
    DLIST_INIT(&t_g_pooled_svc_req_q);

    for (ui2_idx = 0; ui2_idx < ui2_amount; ui2_idx++) {
        CH_SVC_REQ_T* pt_svc_req = &(pt_objs[ui2_idx]);

        DLIST_INSERT_HEAD(pt_svc_req, &t_g_pooled_svc_req_q, t_dlink);
    }

    pt_g_svc_req_objs = pt_objs;

    b_g_svc_req_init = TRUE;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: ch_svc_req_deinit
 *
 * Description: This API initializes the Service Request Pool
 *
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_FAIL
 *-----------------------------------------------------------------------------*/
VOID ch_svc_req_deinit(
                    VOID
                    )
{
    if (b_g_svc_req_init == FALSE) {
        return;
    }

    x_mem_free(pt_g_svc_req_objs);

    b_g_svc_req_init = FALSE;
}
/*-----------------------------------------------------------------------------
 * Name: ch_strm_req_init
 *
 * Description: This API initializes the Service Stream Request Pool
 *
 * Inputs:  ui2_num_svc_reqs  Contains the maximum number of service request
 *                            instances supported by the Service Context Module.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_FAIL
 *-----------------------------------------------------------------------------*/
INT32 ch_strm_req_init(
                    UINT16                      ui2_amount
                    )
{
    UINT16      ui2_idx = 0;
    STRM_REQ_T* pt_objs = NULL;

    DBG_API((_API_HEADER"%s(ui2_amount=%d, each size=%d)\r\n", __func__, ui2_amount, sizeof(STRM_REQ_T)));

    if (b_g_strm_req_init == TRUE) {
        return SVCTXR_OK;
    }

    pt_objs = (STRM_REQ_T*) x_mem_alloc(sizeof(STRM_REQ_T) * ui2_amount);
    if(pt_objs == NULL) {
        DBG_ERROR((_ERROR_HEADER"create memory for STRM_REQ_T pool failed\r\n"));
        return SVCTXR_FAIL;
    }

    x_memset(pt_objs, 0, sizeof(STRM_REQ_T) * ui2_amount);

    DLIST_INIT(&t_g_using_strm_req_q);
    DLIST_INIT(&t_g_pooled_strm_req_q);

    for (ui2_idx = 0; ui2_idx < ui2_amount; ui2_idx++) {
        STRM_REQ_T* pt_strm_req = &(pt_objs[ui2_idx]);

        DLIST_INSERT_HEAD(pt_strm_req, &t_g_pooled_strm_req_q, t_dlink);
    }

    pt_g_strm_req_objs = pt_objs;

    b_g_strm_req_init = TRUE;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: ch_strm_req_deinit
 *
 * Description: This API initializes the Service Stream Request Pool
 *
 * Inputs:  ui2_num_svc_reqs  Contains the maximum number of service request
 *                            instances supported by the Service Context Module.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_FAIL
 *-----------------------------------------------------------------------------*/
VOID ch_strm_req_deinit(
                    VOID
                    )
{
    if (b_g_strm_req_init == FALSE) {
        return;
    }

    x_mem_free(pt_g_strm_req_objs);

    b_g_strm_req_init = FALSE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_strm_req_free
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 ch_strm_req_free(
                    STRM_REQ_T*                 pt_this
                    )
{
    DBG_API((_API_HEADER"%s("
                    "pt_this = 0x%x"
                    ")\r\n",
                    __func__,
                    pt_this
                    ));

    if (b_g_strm_req_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_ch_strm_req_valid(pt_this) == FALSE) {
        return SVCTXR_INV_ARG;
    }

    DLIST_REMOVE(pt_this, &t_g_using_strm_req_q, t_dlink);
    DLIST_INSERT_HEAD(pt_this, &t_g_pooled_strm_req_q, t_dlink);

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_svc_req_free
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 ch_svc_req_free(
                    CH_SVC_REQ_T*               pt_this
                    )
{
    UINT8 ui1_idx;

    DBG_API((_API_HEADER"%s("
                    "pt_this = 0x%x"
                    ")\r\n",
                    __func__,
                    pt_this
                    ));

    if (b_g_svc_req_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_ch_svc_req_valid(pt_this) == FALSE) {
        return SVCTXR_INV_ARG;
    }

    for (ui1_idx = 0; ui1_idx < (UINT8) STRM_TYPE_COUNT_WITH_AD; ui1_idx++) {
        if (pt_this->pt_pending_strm_reqs[ui1_idx] != NULL) {
            ch_strm_req_free(pt_this->pt_pending_strm_reqs[ui1_idx]);
            pt_this->pt_pending_strm_reqs[ui1_idx] = NULL;
        }

        /*
        if (pt_this->pt_running_strm_reqs[ui1_idx] != NULL) {
            ch_strm_req_free(pt_this->pt_running_strm_reqs[ui1_idx]);
            pt_this->pt_running_strm_reqs[ui1_idx] = NULL;
        }
        */
    }

    if (pt_this->t_parent.t_scart_out.pt_descs != NULL) {
        x_mem_free(pt_this->t_parent.t_scart_out.pt_descs);
        pt_this->t_parent.t_scart_out.pt_descs = NULL;
    }

    pt_this->t_parent.t_scart_out.b_valid       = FALSE;
    pt_this->t_parent.t_scart_out.ui1_num_descs = 0;

    DLIST_REMOVE(pt_this, &t_g_using_svc_req_q, t_dlink);
    DLIST_INSERT_HEAD(pt_this, &t_g_pooled_svc_req_q, t_dlink);

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_strm_req_allocate
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 ch_strm_req_allocate(
                    STRM_REQ_T**                ppt_this
                    )
{
    STRM_REQ_T* pt_this = NULL;

    if (b_g_strm_req_init == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s() not init!\r\n", __func__));
        return SVCTXR_NOT_INIT;
    }

    if (ppt_this == NULL) {
        return SVCTXR_INV_ARG;
    }

    pt_this = DLIST_HEAD(&t_g_pooled_strm_req_q);
    if (pt_this == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s() = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_NO_RESOURCE;
    }

    DLIST_REMOVE(pt_this, &t_g_pooled_strm_req_q, t_dlink);
    DLIST_INSERT_HEAD(pt_this, &t_g_using_strm_req_q, t_dlink);
    svctx_reset_strm_req(pt_this);
    
    DBG_API((_API_HEADER"%s() = 0x%x\r\n",
                    __func__,
                    pt_this
                    ));

    *ppt_this = pt_this;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_svc_req_allocate
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 ch_svc_req_allocate(
                    CH_SVC_REQ_T**              ppt_this
                    )
{
    CH_SVC_REQ_T* pt_this = NULL;

    if (b_g_svc_req_init == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s() not init!\r\n", __func__));
        return SVCTXR_NOT_INIT;
    }

    if (ppt_this == NULL) {
        return SVCTXR_INV_ARG;
    }

    pt_this = DLIST_HEAD(&t_g_pooled_svc_req_q);
    if (pt_this == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s() = 0x%x\r\n",  __func__, pt_this));
        return SVCTXR_NO_RESOURCE;
    }

    DLIST_REMOVE(pt_this, &t_g_pooled_svc_req_q, t_dlink);
    DLIST_INSERT_HEAD(pt_this, &t_g_using_svc_req_q, t_dlink);
    _ch_svc_req_reset(pt_this);

    *ppt_this = pt_this;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_svc_req_equals
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
BOOL ch_svc_req_equals(
                    CH_SVC_REQ_T*               pt_chker,
                    CH_SVC_REQ_T*               pt_chkee
                    )
{
    if (pt_chkee == NULL || pt_chker == NULL) {
        return FALSE;
    }

    if (pt_chker->t_parent.e_src_type != pt_chkee->t_parent.e_src_type) {
        return FALSE;
    }

    if (x_strcmp(pt_chker->t_src_info.s_src_grp_name, pt_chkee->t_src_info.s_src_grp_name) != 0) {
        return FALSE;
    }

    if (pt_chker->t_parent.e_src_type == SRC_TYPE_BRDCST_SVL) {
        if (pt_chker->t_src_info.u_info.t_svl.ui2_svc_lst_id
                != pt_chkee->t_src_info.u_info.t_svl.ui2_svc_lst_id) {
            return FALSE;
        }

        if (BRDCST_TYPE_ANALOG != pt_chker->e_brdcst_type &&
            BRDCST_TYPE_ANALOG != pt_chkee->e_brdcst_type)
        {
            if (FALSE == _ch_svc_req_same_digital_svc(pt_chker, pt_chkee))
            {
                return FALSE;
            }
        }
        
	    /* logic to decide if silent tuning is enabled */
        if (pt_chker->t_src_info.u_info.t_svl.t_silent.b_enable == TRUE) {
            if (pt_chkee->t_src_info.u_info.t_svl.t_silent.b_enable == TRUE) {
                /* pt_chker = silent tuning v.s. pt_chkee = silent tuning */
                if (pt_chker->t_src_info.u_info.t_svl.t_silent.ui2_svl_rec_id
                        != pt_chkee->t_src_info.u_info.t_svl.t_silent.ui2_svl_rec_id) {
                    return FALSE;
                }
            } else {
                /* pt_chker = silent tuning v.s. pt_chkee = normal tuning */
                if (pt_chker->t_src_info.u_info.t_svl.t_silent.ui2_svl_rec_id
                        != pt_chkee->t_src_info.u_info.t_svl.ui2_svl_rec_id) {
                    return FALSE;
                }
            }
        } else {
            if (pt_chkee->t_src_info.u_info.t_svl.t_silent.b_enable == TRUE) {
                /* pt_chker = normal tuning v.s. pt_chkee = silent tuning */
                if (pt_chker->t_src_info.u_info.t_svl.ui2_svl_rec_id
                        != pt_chkee->t_src_info.u_info.t_svl.t_silent.ui2_svl_rec_id) {
                    return FALSE;
                }
            } else {
                /* pt_chker = normal tuning v.s. pt_chkee = normal tuning */
                if (pt_chker->t_src_info.u_info.t_svl.ui2_svl_rec_id
                        != pt_chkee->t_src_info.u_info.t_svl.ui2_svl_rec_id) {
                    return FALSE;
                }  
                /*
                if (pt_chker->t_src_info.u_info.t_svl.ui4_channel_id
                        != pt_chkee->t_src_info.u_info.t_svl.ui4_channel_id) {
                    return FALSE;
                }*/
            }
        }

        if (pt_chker->t_src_info.u_info.t_svl.b_detect_tv_sys != pt_chkee->t_src_info.u_info.t_svl.b_detect_tv_sys) {
            return FALSE;
        }

        if (pt_chker->t_src_info.u_info.t_svl.e_scart_mode != pt_chkee->t_src_info.u_info.t_svl.e_scart_mode) {
            return FALSE;
        }

        if (pt_chker->t_src_info.u_info.t_svl.b_ad_to_independent_ad != pt_chkee->t_src_info.u_info.t_svl.b_ad_to_independent_ad) {
            return FALSE;
        }

        if ((BRDCST_TYPE_ANALOG == pt_chker->e_brdcst_type) &&
            (BRDCST_TYPE_ANALOG == pt_chkee->e_brdcst_type))
        {
            if (pt_chker->t_src_info.b_fine_tune != pt_chkee->t_src_info.b_fine_tune)
            {
                return FALSE;
            }
        }

        if (0 != x_strcmp(
                pt_chker->t_src_info.u_info.t_svl.t_ch_encode.s_scart_name,
                pt_chkee->t_src_info.u_info.t_svl.t_ch_encode.s_scart_name))
        {
            return FALSE;
        }

        if (pt_chker->t_riks_tv.b_tune_alter_freq_if_loss_signal != pt_chkee->t_riks_tv.b_tune_alter_freq_if_loss_signal) {
            return FALSE;
        }
    } else {
        return FALSE;
    }

    if (x_strcmp(pt_chker->t_parent.s_snk_grp_name, pt_chkee->t_parent.s_snk_grp_name) != 0) {
        return FALSE;
    }

    return TRUE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_svc_req_get_using_head
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *-----------------------------------------------------------------------------*/
CH_SVC_REQ_T* ch_svc_req_get_using_head(
                    VOID
                    )
{
    return DLIST_HEAD(&t_g_using_svc_req_q);
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_svc_req_get_pooled_head
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
CH_SVC_REQ_T* ch_svc_req_get_pooled_head(
                    VOID
                    )
{
    return DLIST_HEAD(&t_g_pooled_svc_req_q);
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_svc_req_next
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
CH_SVC_REQ_T* ch_svc_req_next(
                    CH_SVC_REQ_T*               pt_this
                    )
{
    if (b_g_svc_req_init == FALSE || pt_this == NULL) {
        return NULL;
    }

    return (DLIST_NEXT(pt_this, t_dlink));
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_strm_req_get_using_head
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *-----------------------------------------------------------------------------*/
STRM_REQ_T* ch_strm_req_get_using_head(
                    VOID
                    )
{
    return DLIST_HEAD(&t_g_using_strm_req_q);
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_strm_req_get_pooled_head
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
STRM_REQ_T* ch_strm_req_get_pooled_head(
                    VOID
                    )
{
    return DLIST_HEAD(&t_g_pooled_strm_req_q);
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_strm_req_next
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
STRM_REQ_T* ch_strm_req_next(
                    STRM_REQ_T*                 pt_this
                    )
{
    if (b_g_strm_req_init == FALSE || pt_this == NULL) {
        return NULL;
    }

    return (DLIST_NEXT(pt_this, t_dlink));
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_strm_req_print_self
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
VOID ch_strm_req_print_self(
                    STRM_REQ_T*                 pt_this
                    )
{
#ifdef CLI_LVL_ALL
    x_dbg_stmt("------------------------------------ size = %d\r\n", sizeof(STRM_REQ_T));
    x_dbg_stmt("e_req_type           - %s\r\n", s_g_strm_req_types[pt_this->e_req_type]);
    if (pt_this->e_req_type == STRM_REQ_SCDB) {
        x_dbg_stmt("e_strm_type          - %s\r\n", s_g_strm_types[pt_this->u.t_scdb.t_comp_id.e_type]);
    } else if (pt_this->e_req_type == STRM_REQ_PID) {
        x_dbg_stmt("e_strm_type          - %s\r\n", s_g_strm_types[pt_this->u.t_pid.e_strm_type]);
        x_dbg_stmt("t_mpeg_2_pid         - 0x%x\r\n", s_g_strm_types[pt_this->u.t_pid.t_mpeg_2_pid]);
    }
    x_dbg_stmt("e_strm_cond          - %s\r\n", s_g_strm_conds[pt_this->e_strm_cond]);
    x_dbg_stmt("e_penging_status     - %s\r\n", s_g_strm_pending_statuses[pt_this->e_penging_status]);
#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (pt_this->b_open_queued)
    {
        x_dbg_stmt("b_open_queued        - Y\r\n");
    }
    if (pt_this->b_close_queued)
    {
        x_dbg_stmt("b_close_queued       - Y\r\n");
    }
#endif
    x_dbg_stmt("====================================\r\n");
#endif
}
/*-----------------------------------------------------------------------------
 * Name
 *      ch_svc_req_print_self
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
VOID ch_svc_req_print_self(
                    CH_SVC_REQ_T*               pt_this
                    )
{
#ifdef CLI_LVL_ALL
    UINT8 ui1_idx;

    x_dbg_stmt("------------------------------------ size = %d\r\n", sizeof(CH_SVC_REQ_T));
    x_dbg_stmt("pt_ch_svc_req          - 0x%x\r\n", pt_this);
    if (pt_this != NULL) {
        x_dbg_stmt("------------------------------------ t_parent, size = %d \r\n", sizeof(SVC_REQ_T));
        x_dbg_stmt("h_svctx                - %d\r\n", pt_this->t_parent.h_svctx);
        x_dbg_stmt("pf_svc_nfy             - 0x%x\r\n", pt_this->t_parent.pf_svc_nfy);
        x_dbg_stmt("pv_svc_nfy_tag         - 0x%x\r\n", pt_this->t_parent.pv_svc_nfy_tag);
        x_dbg_stmt("ui4_stream_mode        - 0x%x\r\n", pt_this->t_parent.ui4_stream_mode);
        x_dbg_stmt("t_stop_cb.pf_nfy       - 0x%x\r\n", pt_this->t_parent.t_stop_cb.pf_nfy);
        x_dbg_stmt("t_stop_cb.pv_tag       - 0x%x\r\n", pt_this->t_parent.t_stop_cb.pv_tag);
        x_dbg_stmt("b_force_unblock        - %s\r\n", (pt_this->t_parent.b_force_unblock == TRUE ? "Y" : "N"));
        x_dbg_stmt("s_snk_grp_name         - %s\r\n", pt_this->t_parent.s_snk_grp_name);
        x_dbg_stmt("i4_speed               - %d\r\n", pt_this->t_parent.i4_speed);
        x_dbg_stmt("e_src_type             - %s\r\n", s_g_src_types[pt_this->t_parent.e_src_type]);
        x_dbg_stmt("e_crnt_tuner_type      - %s\r\n", s_g_tuner_types[DEV_GET_DEV_ID(pt_this->e_crnt_tuner_type)]);
        x_dbg_stmt("pt_hdlr                - 0x%x\r\n", pt_this->t_parent.pt_hdlr);
        x_dbg_stmt("------------------------------------ \r\n");
        x_dbg_stmt("s_conn_src_name        - %s\r\n", pt_this->t_src_info.s_src_grp_name);
        if (pt_this->t_parent.e_src_type == SRC_TYPE_BRDCST_SVL) {
            x_dbg_stmt("e_brdcst_type          - %s\r\n", s_g_brdcst_types[pt_this->e_brdcst_type]);
            x_dbg_stmt("ui2_svc_lst_id         - %d\r\n", pt_this->t_src_info.u_info.t_svl.ui2_svc_lst_id);
            x_dbg_stmt("ui2_svl_rec_id         - %d\r\n", pt_this->t_src_info.u_info.t_svl.ui2_svl_rec_id);
            x_dbg_stmt("ui4_channel_id         - %d\r\n", pt_this->t_src_info.u_info.t_svl.ui4_channel_id);
            if (pt_this->t_src_info.u_info.t_svl.t_silent.b_enable == TRUE){
                x_dbg_stmt("silent ui2_svl_rec_id  - %d\r\n", pt_this->t_src_info.u_info.t_svl.t_silent.ui2_svl_rec_id);
            }
            x_dbg_stmt("b_detect_tv_sys        - %s\r\n", (pt_this->t_src_info.u_info.t_svl.b_detect_tv_sys == TRUE ? "Y" : "N"));
            x_dbg_stmt("e_scart_mode           - %s\r\n", s_g_scart_mode[pt_this->t_src_info.u_info.t_svl.e_scart_mode]);
            x_dbg_stmt("t_ch_encode.type       - %s\r\n", s_g_ch_decode_types[pt_this->t_src_info.u_info.t_svl.t_ch_encode.e_type]);
            x_dbg_stmt("t_ch_encode.scart_name - %s\r\n", pt_this->t_src_info.u_info.t_svl.t_ch_encode.s_scart_name);
        } else if (pt_this->t_parent.e_src_type == SRC_TYPE_BRDCST_FREQ) {
            x_dbg_stmt("t_freq.e_type          - %s)\r\n", s_g_freq_info_types[pt_this->t_src_info.u_info.t_freq.e_type]);
        }
        x_dbg_stmt("h_gl_plane             - %d\r\n", pt_this->h_gl_plane);
        if (pt_this->t_cc_filter.e_cc_type == SM_CC_TYPE_NTSC_LINE21) {
            x_dbg_stmt("t_cc_filter            - (%s, %s)\r\n", s_g_cc_types[pt_this->t_cc_filter.e_cc_type], s_g_ntsc_cc_types[pt_this->t_cc_filter.u.e_data_type]);
        } else if (pt_this->t_cc_filter.e_cc_type == SM_CC_TYPE_NTSC_LINE21) {
            x_dbg_stmt("t_cc_filter            - (%s, svc_%d)\r\n", s_g_cc_types[pt_this->t_cc_filter.e_cc_type], pt_this->t_cc_filter.u.ui1_service_num);
        } else {
            x_dbg_stmt("t_cc_filter            - NONE\r\n");
        }
        for (ui1_idx = 0; ui1_idx < (UINT8)STRM_TYPE_COUNT_WITH_AD; ui1_idx++) {
            x_dbg_stmt("t_strm_reqs[%d]- 0x%x\r\n", ui1_idx, &pt_this->t_strm_reqs[ui1_idx]);
            ch_strm_req_print_self(&pt_this->t_strm_reqs[ui1_idx]);
        }

        for (ui1_idx = 0; ui1_idx < (UINT8)STRM_TYPE_COUNT_WITH_AD; ui1_idx++) {
            x_dbg_stmt("pt_pending_strm_reqs[%d]- 0x%x\r\n", ui1_idx, pt_this->pt_pending_strm_reqs[ui1_idx]);
            if (pt_this->pt_pending_strm_reqs[ui1_idx] != NULL) {
                ch_strm_req_print_self(pt_this->pt_pending_strm_reqs[ui1_idx]);
            }
        }

        x_dbg_stmt("empty_strm_comp_cond   - 0x%x\r\n", pt_this->ui4_empty_strm_comp_cond);
        x_dbg_stmt("exist_strm_comp_cond   - 0x%x\r\n", pt_this->ui4_exist_strm_comp_cond);
        x_dbg_stmt("in_scdb_empty_check    - %s\r\n", (pt_this->b_in_scdb_empty_check_delay == TRUE ? "Y" : "N"));

        if (pt_this->t_parent.t_scart_out.b_valid == TRUE) {
            for (ui1_idx = 0; ui1_idx < pt_this->t_parent.t_scart_out.ui1_num_descs; ui1_idx++) {
                x_dbg_stmt("scart out              - [%d][%s][%s]\r\n",
                                ui1_idx,
                                pt_this->t_parent.t_scart_out.pt_descs[ui1_idx].b_enable ? "Y" : "N",
                                pt_this->t_parent.t_scart_out.pt_descs[ui1_idx].s_name
                                );
            }
        }

        x_dbg_stmt("pt_player              - 0x%x\r\n", pt_this->pt_player);
        x_dbg_stmt("pt_crnt_event          - 0x%x\r\n", pt_this->pt_crnt_event);
        x_dbg_stmt("pt_rating_chker        - 0x%x\r\n", pt_this->pt_rating_chker);
        x_dbg_stmt("pt_strm_fmt_chker      - 0x%x\r\n", pt_this->pt_strm_fmt_chker);
        x_dbg_stmt("pt_scart_avm           - 0x%x\r\n", pt_this->pt_scart_avm);
    }
    x_dbg_stmt("====================================\r\n");
#endif
}
