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
 * $RCSfile: inp_svc_req.c,v $
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
#include "svctx/inp_hdlr/inp_svc_req.h"
#include "svctx/svctx_dbg.h"
/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#undef  _INFO_HEADER
#undef  _API_HEADER
#undef  _ERROR_HEADER
#define _INFO_HEADER        "{SVCTX:INP_HDLR}Info: "
#define _API_HEADER         "{SVCTX:INP_HDLR}API:  "
#define _ERROR_HEADER       "{SVCTX:INP_HDLR}Err:  "

#if 0
#undef  DBG_ERROR
#undef  DBG_API
#undef  DBG_INFO
#define DBG_ERROR(_stmt)    x_dbg_stmt _stmt
#define DBG_API(_stmt)      x_dbg_stmt _stmt
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
static DLIST_T(_INP_SVC_REQ_T)  t_g_using_svc_req_q;
static DLIST_T(_INP_SVC_REQ_T)  t_g_pooled_svc_req_q;
static UINT16                   ui2_g_svc_req_amount   = 0;
static INP_SVC_REQ_T*           pt_g_svc_req_objs      = NULL;

/* stream request pool */
static DLIST_T(_STRM_REQ_T)     t_g_using_strm_req_q;
static DLIST_T(_STRM_REQ_T)     t_g_pooled_strm_req_q;
static STRM_REQ_T*              pt_g_strm_req_objs     = NULL;

extern const STREAM_TYPE_T e_g_inp_strm_types[]; /* QQQQQQ */
/*-----------------------------------------------------------------------------
 * private methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      _inp_svc_req_valid
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static BOOL _inp_svc_req_valid (
                    INP_SVC_REQ_T*              pt_chker
                    )
{
    INP_SVC_REQ_T* pt_chkee;

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
 *      _inp_strm_req_valid
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static BOOL _inp_strm_req_valid (
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
 *      inp_svc_req_allocate
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
static VOID _inp_svc_req_reset(
                    INP_SVC_REQ_T*              pt_this
                    )
{
    STREAM_TYPE_T e_strm_type;
    UINT8         ui1_idx;

    if (pt_this == NULL) {
        return;
    }

    for (ui1_idx = 0; ui1_idx < INP_STRM_TYPE_COUNT; ui1_idx++) {
        e_strm_type = e_g_inp_strm_types[ui1_idx];
        
        if (((UINT32)e_strm_type >= INP_STRM_TYPE_COUNT) || (e_strm_type==ST_UNKNOWN))
        {
            ASSERT(0, ("%s(), e_strm_type=%d out of boundary\r\n", __func__, e_strm_type));
            continue;
        }

        pt_this->pt_pending_strm_reqs[e_strm_type] = NULL;
        svctx_reset_strm_req(&(pt_this->t_strm_reqs[e_strm_type]));
    }

    svctx_reset_svc_req(&(pt_this->t_parent));
    
    pt_this->t_src_info.s_src_grp_name[0]   = '\0';
    pt_this->h_gl_plane                     = NULL_HANDLE;
    pt_this->t_cc_filter.e_cc_type          = SM_CC_TYPE_IGNORE;
    pt_this->pt_video_player                = NULL;
    pt_this->pt_audio_player                = NULL;
    pt_this->pt_audio_avm                   = NULL;
    pt_this->pt_video_avm                   = NULL;
    pt_this->b_replace_audio                = FALSE;
    pt_this->b_replace_video                = FALSE;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    pt_this->b_combi_switch                 = FALSE;
#endif
    pt_this->b_play_audio_from_video_device = FALSE;
    pt_this->b_hdmi_signal                  = FALSE;
    pt_this->pt_crnt_event                  = NULL;
    pt_this->pt_rating_chker                = NULL;
}
/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: inp_svc_req_init
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
INT32 inp_svc_req_init(
                    UINT16                      ui2_amount
                    )
{
    UINT16 ui2_idx;

    DBG_API((_API_HEADER"%s("
                    "ui2_amount=%d, "
                    "each size=%d)\r\n",
                    __func__,
                    ui2_amount,
                    sizeof(INP_SVC_REQ_T)
                    ));

    if (b_g_svc_req_init == TRUE) {
        return SVCTXR_OK;
    }

    pt_g_svc_req_objs = (INP_SVC_REQ_T*) x_mem_alloc(
                    sizeof(INP_SVC_REQ_T) * ui2_amount
                    );
    if(pt_g_svc_req_objs == NULL) {
        DBG_ERROR((_ERROR_HEADER"create memory for SVC_REQ_T pool failed\r\n"));
        return SVCTXR_FAIL;
    }

    x_memset(pt_g_svc_req_objs, 0, sizeof(INP_SVC_REQ_T) * ui2_amount);

    DLIST_INIT(&t_g_using_svc_req_q);
    DLIST_INIT(&t_g_pooled_svc_req_q);

    ui2_g_svc_req_amount = ui2_amount;

    for (ui2_idx = 0; ui2_idx < ui2_g_svc_req_amount; ui2_idx++) {
        INP_SVC_REQ_T* pt_svc_req = &(pt_g_svc_req_objs[ui2_idx]);

        DLIST_INSERT_HEAD(pt_svc_req, &t_g_pooled_svc_req_q, t_dlink);
    }

    b_g_svc_req_init = TRUE;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: inp_svc_req_deinit
 *
 * Description: This API initializes the Service Request Pool
 *
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK          Routine successful.
 *          SVCTXR_FAIL
 *-----------------------------------------------------------------------------*/
VOID inp_svc_req_deinit(
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
 * Name: inp_strm_req_init
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
INT32 inp_strm_req_init(
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
 * Name: inp_strm_req_deinit
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
VOID inp_strm_req_deinit(
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
 *      inp_strm_req_free
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 inp_strm_req_free(
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

    if (_inp_strm_req_valid(pt_this) == FALSE) {
        return SVCTXR_INV_ARG;
    }

    DLIST_REMOVE(pt_this, &t_g_using_strm_req_q, t_dlink);
    DLIST_INSERT_HEAD(pt_this, &t_g_pooled_strm_req_q, t_dlink);

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_svc_req_free
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 inp_svc_req_free(
                    INP_SVC_REQ_T*              pt_this
                    )
{
    STREAM_TYPE_T e_strm_type;
    UINT8         ui1_idx;

    DBG_API((_API_HEADER"%s("
                    "pt_this = 0x%x"
                    ")\r\n",
                    __func__,
                    pt_this
                    ));

    if (b_g_svc_req_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_inp_svc_req_valid(pt_this) == FALSE) {
        return SVCTXR_INV_ARG;
    }
    
    for (ui1_idx = 0; ui1_idx < INP_STRM_TYPE_COUNT; ui1_idx++) {
        e_strm_type = e_g_inp_strm_types[ui1_idx];

        if (((UINT32)e_strm_type >= INP_STRM_TYPE_COUNT) || (e_strm_type==ST_UNKNOWN))
        {
            ASSERT(0, ("%s(), e_strm_type=%d out of boundary\r\n", __func__, e_strm_type));
            continue;
        }

        if (pt_this->pt_pending_strm_reqs[e_strm_type] != NULL) {
            inp_strm_req_free(pt_this->pt_pending_strm_reqs[e_strm_type]);
        }

        pt_this->pt_pending_strm_reqs[e_strm_type] = NULL;
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
 *      inp_strm_req_allocate
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 inp_strm_req_allocate(
                    STRM_REQ_T**                ppt_this
                    )
{
    STRM_REQ_T* pt_this = NULL;

    if (b_g_strm_req_init == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s() not init!\r\n", __func__));
        return SVCTXR_NOT_INIT;
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
 *      inp_svc_req_allocate
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
INT32 inp_svc_req_allocate(
                    INP_SVC_REQ_T**             ppt_this
                    )
{
    INP_SVC_REQ_T* pt_this = NULL;

    if (b_g_svc_req_init == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s() not init!\r\n", __func__));
        return SVCTXR_NOT_INIT;
    }

    pt_this = DLIST_HEAD(&t_g_pooled_svc_req_q);
    if (pt_this == NULL) {
        DBG_ERROR((_ERROR_HEADER"%s() = 0x%x\r\n",  __func__, pt_this));
        return SVCTXR_NO_RESOURCE;
    }

    DLIST_REMOVE(pt_this, &t_g_pooled_svc_req_q, t_dlink);
    DLIST_INSERT_HEAD(pt_this, &t_g_using_svc_req_q, t_dlink);
    _inp_svc_req_reset(pt_this);

    *ppt_this = pt_this;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_svc_req_equals
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *-----------------------------------------------------------------------------*/
BOOL inp_svc_req_equals(
                    BOOL                        b_associate,
                    INP_SVC_REQ_T*              pt_chker,
                    INP_SVC_REQ_T*              pt_chkee
                    )
{
    if (pt_chkee == NULL || pt_chker == NULL) {
        DBG_INFO((_INFO_HEADER"%s() pt_chkee = 0x%x, pt_chker = 0x%x\r\n",
                    __func__,
                    pt_chkee,
                    pt_chker
                    ));
        return FALSE;
    }

    if (pt_chker->t_parent.e_src_type != pt_chkee->t_parent.e_src_type) {
        DBG_INFO((_INFO_HEADER"%s() different e_type %d - %d\r\n",
                    __func__,
                    pt_chkee->t_parent.e_src_type,
                    pt_chker->t_parent.e_src_type
                    ));
        return FALSE;
    }

    if (x_strcmp(pt_chker->t_src_info.s_src_grp_name, pt_chkee->t_src_info.s_src_grp_name) != 0) {
        DBG_INFO((_INFO_HEADER"%s() different s_conn_src_name %s - %s\r\n",
                    __func__,
                    pt_chkee->t_src_info.s_src_grp_name,
                    pt_chker->t_src_info.s_src_grp_name
                    ));
        return FALSE;
    }

    if (!b_associate)
    {
        /* check audio parameters - device type */
        if (pt_chker->t_src_info.t_av_src.e_audio_type != pt_chkee->t_src_info.t_av_src.e_audio_type) {
            DBG_INFO((_INFO_HEADER"%s() different e_audio_type %d - %d\r\n",
                        __func__,
                        pt_chker->t_src_info.t_av_src.e_audio_type,
                        pt_chkee->t_src_info.t_av_src.e_audio_type
                        ));
            return FALSE;
        }
    
        /* check audio parameters - hint type */
        if (pt_chker->t_src_info.t_av_src.t_audio_hint.e_type != pt_chkee->t_src_info.t_av_src.t_audio_hint.e_type) {
            DBG_INFO((_INFO_HEADER"%s() different t_audio_hint.e_type %d - %d\r\n",
                        __func__,
                        pt_chker->t_src_info.t_av_src.t_audio_hint.e_type,
                        pt_chkee->t_src_info.t_av_src.t_audio_hint.e_type
                        ));
            return FALSE;
        }
    
        /* check audio parameters - hint device type */
        if (pt_chker->t_src_info.t_av_src.t_audio_hint.e_type == SRC_AVC_HINT_SPECIFIC_DEVICE_ONLY
                || pt_chker->t_src_info.t_av_src.t_audio_hint.e_type == SRC_AVC_HINT_SPECIFIC_DEVICE_FIRST) {
            if (pt_chker->t_src_info.t_av_src.t_audio_hint.e_dev_type != pt_chkee->t_src_info.t_av_src.t_audio_hint.e_dev_type) {
                DBG_INFO((_INFO_HEADER"%s() different t_audio_hint.e_dev_type %d - %d\r\n",
                        __func__,
                        pt_chker->t_src_info.t_av_src.t_audio_hint.e_dev_type,
                        pt_chkee->t_src_info.t_av_src.t_audio_hint.e_dev_type
                        ));
                return FALSE;
            }
        }
    
        /* check video parameters - device type */
        if (pt_chker->t_src_info.t_av_src.e_video_type != pt_chkee->t_src_info.t_av_src.e_video_type) {
            DBG_INFO((_INFO_HEADER"%s() different e_video_type %d - %d\r\n",
                        __func__,
                        pt_chker->t_src_info.t_av_src.e_video_type,
                        pt_chkee->t_src_info.t_av_src.e_video_type
                        ));
            return FALSE;
        }
    
        /* check video parameters - hint type */
        if (pt_chker->t_src_info.t_av_src.t_video_hint.e_type != pt_chkee->t_src_info.t_av_src.t_video_hint.e_type) {
            DBG_INFO((_INFO_HEADER"%s() different t_video_hint.e_type %d - %d\r\n",
                        __func__,
                        pt_chker->t_src_info.t_av_src.t_video_hint.e_type,
                        pt_chkee->t_src_info.t_av_src.t_video_hint.e_type
                        ));
            return FALSE;
        }
    
        /* check video parameters - hint device type */
        if (pt_chker->t_src_info.t_av_src.t_video_hint.e_type == SRC_AVC_HINT_SPECIFIC_DEVICE_ONLY
                || pt_chker->t_src_info.t_av_src.t_video_hint.e_type == SRC_AVC_HINT_SPECIFIC_DEVICE_FIRST) {
            if (pt_chker->t_src_info.t_av_src.t_video_hint.e_dev_type != pt_chkee->t_src_info.t_av_src.t_video_hint.e_dev_type) {
                DBG_INFO((_INFO_HEADER"%s() different t_video_hint.e_dev_type %d - %d\r\n",
                        __func__,
                        pt_chker->t_src_info.t_av_src.t_video_hint.e_dev_type,
                        pt_chkee->t_src_info.t_av_src.t_video_hint.e_dev_type
                        ));
                return FALSE;
            }
        }
    }

    if (pt_chker->t_src_info.t_av_src.e_video_type == DEV_AVC_SCART) {
        if (pt_chker->t_src_info.t_av_src.e_scart_mode != pt_chkee->t_src_info.t_av_src.e_scart_mode) {
            return FALSE;
        }
    }

    if (x_strcmp(pt_chker->t_parent.s_snk_grp_name, pt_chkee->t_parent.s_snk_grp_name) != 0) {
        DBG_INFO((_INFO_HEADER"%s() different s_snk_grp_name %s - %s\r\n",
                    __func__,
                    pt_chkee->t_parent.s_snk_grp_name,
                    pt_chker->t_parent.s_snk_grp_name
                    ));
        return FALSE;
    }

    DBG_INFO((_INFO_HEADER"%s() equivalent request 0x%x - 0x%x\r\n",
                    __func__,
                    pt_chkee,
                    pt_chker
                    ));
    return TRUE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_svc_req_get_using_head
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *-----------------------------------------------------------------------------*/
INP_SVC_REQ_T* inp_svc_req_get_using_head(
                    VOID
                    )
{
    return DLIST_HEAD(&t_g_using_svc_req_q);
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_svc_req_get_pooled_head
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
INP_SVC_REQ_T* inp_svc_req_get_pooled_head(
                    VOID
                    )
{
    return DLIST_HEAD(&t_g_pooled_svc_req_q);
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_svc_req_next
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
INP_SVC_REQ_T* inp_svc_req_next(
                    INP_SVC_REQ_T*              pt_this
                    )
{
    if (b_g_svc_req_init == FALSE || pt_this == NULL) {
        return NULL;
    }

    return (DLIST_NEXT(pt_this, t_dlink));
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_strm_req_get_using_head
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *-----------------------------------------------------------------------------*/
STRM_REQ_T* inp_strm_req_get_using_head(
                    VOID
                    )
{
    return DLIST_HEAD(&t_g_using_strm_req_q);
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_strm_req_get_pooled_head
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
STRM_REQ_T* inp_strm_req_get_pooled_head(
                    VOID
                    )
{
    return DLIST_HEAD(&t_g_pooled_strm_req_q);
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_strm_req_next
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
STRM_REQ_T* inp_strm_req_next(
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
 *      inp_strm_req_print_self
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
VOID inp_strm_req_print_self(
                    STRM_REQ_T*                 pt_this
                    )
{
#ifdef CLI_LVL_ALL
    x_dbg_stmt("------------------------------------ size = %d\r\n", sizeof(STRM_REQ_T));
    x_dbg_stmt("pt_inp_strm_req       - 0x%x\r\n", pt_this);
    x_dbg_stmt("====================================\r\n");
#endif
}
/*-----------------------------------------------------------------------------
 * Name
 *      inp_svc_req_print_self
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
VOID inp_svc_req_print_self(
                    INP_SVC_REQ_T*              pt_this
                    )
{
#ifdef CLI_LVL_ALL
    UINT8 ui1_idx;

    x_dbg_stmt("------------------------------------ size = %d\r\n", sizeof(INP_SVC_REQ_T));
    x_dbg_stmt("pt_inp_svc_req             - 0x%x\r\n", pt_this);
    if (pt_this != NULL) {
        x_dbg_stmt("------------------------------------ t_parent, size = %d \r\n", sizeof(SVC_REQ_T));
        x_dbg_stmt("h_svctx                    - %d\r\n", pt_this->t_parent.h_svctx);
        x_dbg_stmt("pf_svc_nfy                 - 0x%x\r\n", pt_this->t_parent.pf_svc_nfy);
        x_dbg_stmt("pv_svc_nfy_tag             - 0x%x\r\n", pt_this->t_parent.pv_svc_nfy_tag);
        x_dbg_stmt("ui4_stream_mode            - 0x%x\r\n", pt_this->t_parent.ui4_stream_mode);
        x_dbg_stmt("t_stop_cb.pf_nfy           - 0x%x\r\n", pt_this->t_parent.t_stop_cb.pf_nfy);
        x_dbg_stmt("t_stop_cb.pv_tag           - 0x%x\r\n", pt_this->t_parent.t_stop_cb.pv_tag);
        x_dbg_stmt("b_force_unblock            - %s\r\n", (pt_this->t_parent.b_force_unblock == TRUE ? "Y" : "N"));
        x_dbg_stmt("s_snk_grp_name             - %s\r\n", pt_this->t_parent.s_snk_grp_name);
        x_dbg_stmt("i4_speed                   - %d\r\n", pt_this->t_parent.i4_speed);
        x_dbg_stmt("e_src_type                 - %s\r\n", s_g_src_types[pt_this->t_parent.e_src_type]);
        x_dbg_stmt("pt_hdlr                    - 0x%x\r\n", pt_this->t_parent.pt_hdlr);
        x_dbg_stmt("pt_video_player            - 0x%x\r\n", pt_this->pt_video_player);
        x_dbg_stmt("pt_audio_player            - 0x%x\r\n", pt_this->pt_audio_player);
        x_dbg_stmt("s_src_grp_name             - %s\r\n", pt_this->t_src_info.s_src_grp_name);
        x_dbg_stmt("ui2_crnt_video_combi_index - %d\r\n", pt_this->t_src_info.ui2_crnt_video_combi_index);
        x_dbg_stmt("ui2_crnt_audio_combi_index - %d\r\n", pt_this->t_src_info.ui2_crnt_audio_combi_index);
        x_dbg_stmt("e_crnt_video_type          - %s/%s\r\n", s_g_avc_comp_types[DEV_GET_DEV_ID(pt_this->t_src_info.e_crnt_video_type)], s_g_strm_conds[pt_this->t_strm_reqs[ST_VIDEO].e_strm_cond]);
        x_dbg_stmt("e_crnt_audio_type          - %s/%s\r\n", s_g_avc_comp_types[DEV_GET_DEV_ID(pt_this->t_src_info.e_crnt_audio_type)], s_g_strm_conds[pt_this->t_strm_reqs[ST_AUDIO].e_strm_cond]);
        x_dbg_stmt("video_info                 - %s/%s/%s\r\n", s_g_avc_comp_types[DEV_GET_DEV_ID(pt_this->t_src_info.t_av_src.e_video_type)], s_g_avc_hints[pt_this->t_src_info.t_av_src.t_video_hint.e_type], s_g_avc_comp_types[DEV_GET_DEV_ID(pt_this->t_src_info.t_av_src.t_video_hint.e_dev_type)]);
        x_dbg_stmt("audio_info                 - %s/%s/%s\r\n", s_g_avc_comp_types[DEV_GET_DEV_ID(pt_this->t_src_info.t_av_src.e_audio_type)], s_g_avc_hints[pt_this->t_src_info.t_av_src.t_audio_hint.e_type], s_g_avc_comp_types[DEV_GET_DEV_ID(pt_this->t_src_info.t_av_src.t_audio_hint.e_dev_type)]);
        x_dbg_stmt("e_scart_mode               - %s\r\n", s_g_scart_mode[pt_this->t_src_info.t_av_src.e_scart_mode]);

        if (pt_this->t_parent.t_scart_out.b_valid == TRUE) {
            for (ui1_idx = 0; ui1_idx < pt_this->t_parent.t_scart_out.ui1_num_descs; ui1_idx++) {
                x_dbg_stmt("scart out                  - [%d][%s][%s]\r\n",
                                ui1_idx,
                                pt_this->t_parent.t_scart_out.pt_descs[ui1_idx].b_enable ? "Y" : "N",
                                pt_this->t_parent.t_scart_out.pt_descs[ui1_idx].s_name
                                );
            }
        }
    }
    x_dbg_stmt("====================================\r\n");
#endif
}
