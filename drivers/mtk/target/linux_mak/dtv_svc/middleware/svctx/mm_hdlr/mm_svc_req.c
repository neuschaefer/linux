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
 * $RCSfile: mm_svc_req.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/13 $
 * $SWAuthor: Weider Chang $
 * $MD5HEX: 7244f8264301adfae5367f1061228312 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "svctx/mm_hdlr/mm_svc_req.h"
#include "svctx/svctx_dbg.h"
/*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * private variable declarations
 *---------------------------------------------------------------------------*/
static BOOL                     b_g_svc_req_init       = FALSE;
static BOOL                     b_g_strm_req_init      = FALSE;
/* service request pool */
static DLIST_T(_MM_SVC_REQ_T)   t_g_using_svc_req_q;
static DLIST_T(_MM_SVC_REQ_T)   t_g_pooled_svc_req_q;
static MM_SVC_REQ_T*            pt_g_svc_req_objs      = NULL;

/* stream request pool */
static DLIST_T(_STRM_REQ_T)     t_g_using_strm_req_q;
static DLIST_T(_STRM_REQ_T)     t_g_pooled_strm_req_q;
static STRM_REQ_T*              pt_g_strm_req_objs     = NULL;
/*-----------------------------------------------------------------------------
 * private methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      _mm_svc_req_valid
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static BOOL _mm_svc_req_valid (
                    MM_SVC_REQ_T*               pt_chker
                    )
{
    MM_SVC_REQ_T* pt_chkee;

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
 *      _mm_strm_req_valid
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static BOOL _mm_strm_req_valid (
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
 *      _mm_svc_req_reset
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static VOID _mm_svc_req_reset(
                    MM_SVC_REQ_T*               pt_this
                    )
{
    UINT8 ui1_idx;
    
    if (pt_this == NULL) {
        return;
    }

    /*Add hooking media handler in which obj is allocated. Invoking handler's open code */
    /*MM_SVC_UOP_HANDLER_FCT_TBL_T      t_fct_tbl;    
    MEDIA_FORMAT_T t_format;
    t_format = pt_this->t_media_describ.t_media_fmt;
    if(SVCTXR_OK != mm_hdlr_find_handler(&t_format, &t_fct_tbl))
    {
        return SVCTXR_NOT_SUPPORT;
    }
    pt_this->t_uop_fct_tbl = t_fct_tbl;*/

    for (ui1_idx = 0; ui1_idx < (UINT8)STRM_TYPE_COUNT; ui1_idx++) {
        pt_this->pt_pending_strm_reqs[ui1_idx] = NULL;
        svctx_reset_strm_req(&(pt_this->t_strm_reqs[ui1_idx]));
    }
    
    svctx_reset_svc_req(&(pt_this->t_parent));

    pt_this->t_src_info.s_src_grp_name[0] = '\0';
    pt_this->h_gl_plane                   = NULL_HANDLE;
    pt_this->t_cc_filter.e_cc_type        = SM_CC_TYPE_IGNORE;
    pt_this->pt_player                    = NULL;
    pt_this->pt_strm_fmt_chker            = NULL;
}

/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: mm_svc_req_init
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
INT32 mm_svc_req_init(
                    UINT16                      ui2_amount
                    )
{
    UINT16 ui2_idx;

    DBG_API((_API_HEADER"%s("
                    "ui2_amount=%d, "
                    "each size=%d)\r\n",
                    __func__,
                    ui2_amount,
                    sizeof(MM_SVC_REQ_T)
                    ));

    if (b_g_svc_req_init == TRUE) {
        return SVCTXR_OK;
    }

    pt_g_svc_req_objs = (MM_SVC_REQ_T*) x_mem_alloc(
                    sizeof(MM_SVC_REQ_T) * ui2_amount
                    );
    if(pt_g_svc_req_objs == NULL) {
        DBG_ERROR((_ERROR_HEADER"create memory for SVC_REQ_T pool failed\r\n"));
        return SVCTXR_FAIL;
    }

    x_memset(pt_g_svc_req_objs, 0, sizeof(MM_SVC_REQ_T) * ui2_amount);

    DLIST_INIT(&t_g_using_svc_req_q);
    DLIST_INIT(&t_g_pooled_svc_req_q);

    for (ui2_idx = 0; ui2_idx < ui2_amount; ui2_idx++) {
        MM_SVC_REQ_T* pt_svc_req = &(pt_g_svc_req_objs[ui2_idx]);

        DLIST_INSERT_HEAD(pt_svc_req, &t_g_pooled_svc_req_q, t_dlink);
    }

    b_g_svc_req_init = TRUE;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: mm_svc_req_deinit
 *
 * Description: This API initializes the Service Request Pool
 *
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_FAIL
 *-----------------------------------------------------------------------------*/
VOID mm_svc_req_deinit(
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
 * Name: mm_strm_req_init
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
INT32 mm_strm_req_init(
                    UINT16                      ui2_amount
                    )
{
    UINT16 ui2_idx;

    DBG_API((_API_HEADER"%s(ui2_amount=%d, each size=%d)\r\n", __func__, ui2_amount, sizeof(STRM_REQ_T)));

    if (b_g_strm_req_init == TRUE) {
        return SVCTXR_OK;
    }

    pt_g_strm_req_objs = (STRM_REQ_T*) x_mem_alloc(
                    sizeof(STRM_REQ_T) * ui2_amount
                    );
    if(pt_g_strm_req_objs == NULL) {
        DBG_ERROR((_ERROR_HEADER"create memory for STRM_REQ_T pool failed\r\n"));
        return SVCTXR_FAIL;
    }

    x_memset(pt_g_strm_req_objs, 0, sizeof(STRM_REQ_T) * ui2_amount);

    DLIST_INIT(&t_g_using_strm_req_q);
    DLIST_INIT(&t_g_pooled_strm_req_q);

    for (ui2_idx = 0; ui2_idx < ui2_amount; ui2_idx++) {
        STRM_REQ_T* pt_strm_req = &(pt_g_strm_req_objs[ui2_idx]);

        DLIST_INSERT_HEAD(pt_strm_req, &t_g_pooled_strm_req_q, t_dlink);
    }

    b_g_strm_req_init = TRUE;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: mm_strm_req_deinit
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
VOID mm_strm_req_deinit(
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
 *      mm_strm_req_allocate
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 mm_strm_req_allocate(
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
 *      mm_strm_req_free
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 mm_strm_req_free(
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

    if (_mm_strm_req_valid(pt_this) == FALSE) {
        return SVCTXR_INV_ARG;
    }

    DLIST_REMOVE(pt_this, &t_g_using_strm_req_q, t_dlink);
    DLIST_INSERT_HEAD(pt_this, &t_g_pooled_strm_req_q, t_dlink);

    return SVCTXR_OK;
}


/*-----------------------------------------------------------------------------
 * Name
 *      mm_svc_req_allocate
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 mm_svc_req_allocate(
                    MM_SVC_REQ_T**              ppt_this
                    )
{
    MM_SVC_REQ_T* pt_this = NULL;

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
    _mm_svc_req_reset(pt_this);

    *ppt_this = pt_this;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      mm_svc_req_free
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 mm_svc_req_free(
                    MM_SVC_REQ_T*               pt_this
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

    if (_mm_svc_req_valid(pt_this) == FALSE) {
        return SVCTXR_INV_ARG;
    }

    for (ui1_idx = 0; ui1_idx < (UINT8)STRM_TYPE_COUNT; ui1_idx++) {
        if (pt_this->pt_pending_strm_reqs[ui1_idx] != NULL) {
            mm_strm_req_free(pt_this->pt_pending_strm_reqs[ui1_idx]);
            pt_this->pt_pending_strm_reqs[ui1_idx] = NULL;
        }

        /*
        if (pt_this->pt_running_strm_reqs[ui1_idx] != NULL) {
            mm_strm_req_free(pt_this->pt_running_strm_reqs[ui1_idx]);
            pt_this->pt_running_strm_reqs[ui1_idx] = NULL;
        }
        */
    }

    DLIST_REMOVE(pt_this, &t_g_using_svc_req_q, t_dlink);
    DLIST_INSERT_HEAD(pt_this, &t_g_pooled_svc_req_q, t_dlink);

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      mm_svc_req_equals
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
BOOL mm_svc_req_equals(
                    MM_SVC_REQ_T*               pt_chker,
                    MM_SVC_REQ_T*               pt_chkee
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

    if (pt_chker->t_parent.e_src_type == SRC_TYPE_MEDIA_TIME_SHIFT) {
        if (pt_chker->t_src_info.u_info.t_svl.ui2_svc_lst_id
                != pt_chkee->t_src_info.u_info.t_svl.ui2_svc_lst_id) {
            return FALSE;
        }
#if 0
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
#endif
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
#if 0
            }
        }

        if (pt_chker->t_src_info.u_info.t_svl.b_detect_tv_sys != pt_chkee->t_src_info.u_info.t_svl.b_detect_tv_sys) {
            return FALSE;
        }

        if (pt_chker->t_src_info.u_info.t_svl.e_scart_mode != pt_chkee->t_src_info.u_info.t_svl.e_scart_mode) {
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
#endif
    } else {
        return FALSE;
    }

    if (x_strcmp(pt_chker->t_parent.s_snk_grp_name, pt_chkee->t_parent.s_snk_grp_name) != 0) {
        return FALSE;
    }

    return TRUE;
}

