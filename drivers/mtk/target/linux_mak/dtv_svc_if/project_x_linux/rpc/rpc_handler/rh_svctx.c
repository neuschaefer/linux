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
#include "svctx/x_svctx.h"
#include "os/inc/x_os.h"

#include "handle/handle.h"
#include "svctx/svctx_dbg.h"
#include "wgl/x_wgl.h"

#include "rpc.h"
#include "ri_common.h"

#include "evctx/evctx_atsc_eng/u_evctx_atsc_eng.h"

#include "ri_desc.h"
#include "rh_cb_wrapper.h"

#include "svctx/svctx.h"
extern HANDLE_T svctx_get_running_svc_req_strm_cb_handle(HANDLE_T  h_svctx);
extern VOID svctx_reg_rpc_handler_util_func(VOID*   pv_fct_1,
                                            VOID*   pv_fct_2,
                                            VOID*   pv_fct_3,
                                            VOID*   pv_fct_4);
extern INT32 x_wgl_get_canvas(
                                GL_HPLANE_T         h_plane,
                                HANDLE_T*           ph_canvas);
extern INT32 x_gl_screen_open(
                                const CHAR*         ps_name,
                                x_gl_nfy_fct        pf_screen_nfy,
                                VOID*               pv_nfy_tag,
                                GL_HSCREEN_T*       ph_screen);
extern INT32 x_gl_plane_open(
                                GL_HSCREEN_T        h_screen, 
                                UINT16              ui2_idx,
                                GL_HPLANE_T*        ph_plane);
extern INT32 x_gl_plane_link(
                            GL_HPLANE_T         h_plane,
                            GL_HSCREEN_T        h_screen,
                            BOOL                b_forced);
extern VOID x_rrctx_base_ctrl_list_lock (VOID);
extern VOID x_rrctx_base_ctrl_list_unlock (VOID);
extern HANDLE_T gl_dfb_cc_helper_get_plane();


extern INT32 svctx_set_default_parameters(
        HANDLE_T                    h_svctx,
        UINT32                      ui4_stream_mode,
        SCC_VID_MODE_T              e_vid_mode,
        BOOL                        b_able_set_pmx_bg,
        svctx_svc_block_check_fct   pf_block_check_fct,
        VOID*                       pv_tag,
        const CHAR*                 ps_src_grp_name,
        const CHAR*                 ps_snk_grp_name,
        const CHAR*                 ps_snk_grp_name_aux
        );


static BOOL 
_x_svctx_strm_comp_filter_fct_wrapper (
    UINT16                      ui2_num_recs,           /* in  */
    UINT16                      ui2_idx,                /* in  */
    const STREAM_COMP_ID_T*     pt_comp_id,             /* in  */
    const SCDB_REC_T*           pt_scdb_rec,            /* in  */
    VOID*                       pv_fltr_tag             /* in  */
    )
{
    RPC_DECL(5, INT32);

    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_fltr_tag;

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_comp_id, RPC_DESC_STREAM_COMP_ID_T, NULL));
    RPC_CHECK(RPC_ADD_REF_VAL(RPC_DEFAULT_ID, &pt_comp_id->pv_stream_tag));
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_scdb_rec, RPC_DESC_SCDB_REC_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT16,   ui2_num_recs);
    RPC_ARG_INP(ARG_TYPE_UINT16,   ui2_idx);        
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_comp_id);        
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_scdb_rec);    
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_svctx_strm_comp_filter_fct", pt_nfy_tag->pv_cb_addr);
    
    RPC_RETURN(ARG_TYPE_BOOL, FALSE);    
}

static INT32 _hndlr_x_svctx_stop_svc(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }
   
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_stop_svc(pt_args[0].u.ui4_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_is_running(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_is_running(pt_args[0].u.ui4_arg, pt_args[1].u.pb_arg);
    
    return RPCR_OK;
}


static VOID 
_x_svctx_select_nfy_fct_wrapper(
    HANDLE_T                    h_svctx,                /* in  */
    SVCTX_COND_T                e_nfy_cond,             /* in  */
    SVCTX_NTFY_CODE_T           e_code,                 /* in  */
    STREAM_TYPE_T               e_stream_type,          /* in  */
    VOID*                       pv_nfy_tag              /* in  */)
{    
    RPC_DECL_VOID(5);
    
    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_nfy_tag;
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);
    RPC_ARG_INP(ARG_TYPE_UINT32,   e_nfy_cond);        
    RPC_ARG_INP(ARG_TYPE_UINT32,   e_code);        
    RPC_ARG_INP(ARG_TYPE_INT32,    e_stream_type);    
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_svctx_select_nfy_fct", pt_nfy_tag->pv_cb_addr);
    
    RPC_RETURN_VOID;    
}

#ifndef NO_WGL_INIT
static HANDLE_T h_cmpb_screen        = NULL_HANDLE;
static HANDLE_T h_cmpb_plane         = NULL_HANDLE;
static HANDLE_T h_cmpb_canvas        = NULL_HANDLE;
#endif

static INT32 _hndlr_x_svctx_select_svc(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T *      pt_nfy_tag;
    x_svctx_select_nfy_fct  pf_nfy;
    SNK_DESC_T * pt_snk_cpy = NULL;
    SNK_DESC_T * pt_snk_tmp = NULL;
    HANDLE_T h_canvas;
    INT32 i4_ret = -1;
    
    if(ui4_num_args != 6)
    {
        pt_return->e_type   = ARG_TYPE_INT32;        
        pt_return->u.i4_arg = SVCTXR_FAIL;    
        return RPCR_INV_ARGS;
    }

    pt_snk_tmp = (SNK_DESC_T*)(pt_args[3].u.pv_desc);

    if(pt_args[3].u.pv_desc != NULL)
    {   
        pt_snk_cpy = malloc(sizeof(SNK_DESC_T));
        if(pt_snk_cpy == NULL)
        {
            return RPCR_OUTOFMEMORY;
        }    
        memcpy(pt_snk_cpy, pt_args[3].u.pv_desc, sizeof(SNK_DESC_T));
        
        /* Video */
        if(pt_snk_tmp->pt_video_strm_fltr_fct == NULL)
        {
            pt_snk_cpy->pt_video_strm_fltr_fct = NULL;
            pt_snk_cpy->pv_video_strm_fltr_tag = pt_snk_tmp->pv_video_strm_fltr_tag;     
        }
        else
        {
            RPC_CB_TAG(t_rpc_id, pt_nfy_tag, pt_snk_tmp->pt_video_strm_fltr_fct,
                pt_snk_tmp->pv_video_strm_fltr_tag);
            pt_snk_cpy->pt_video_strm_fltr_fct = _x_svctx_strm_comp_filter_fct_wrapper;
            pt_snk_cpy->pv_video_strm_fltr_tag = pt_nfy_tag;     
        }    

        /* Audio */
        if(pt_snk_tmp->pt_audio_strm_fltr_fct == NULL)
        {
            pt_snk_cpy->pt_audio_strm_fltr_fct = NULL;
            pt_snk_cpy->pv_audio_strm_fltr_tag = pt_snk_tmp->pv_audio_strm_fltr_tag;     
        }
        else
        {
            RPC_CB_TAG(t_rpc_id, pt_nfy_tag, pt_snk_tmp->pt_audio_strm_fltr_fct,
                pt_snk_tmp->pv_audio_strm_fltr_tag);
            pt_snk_cpy->pt_audio_strm_fltr_fct = _x_svctx_strm_comp_filter_fct_wrapper;
            pt_snk_cpy->pv_audio_strm_fltr_tag = pt_nfy_tag;     
        }

        /* CC */
        if(pt_snk_tmp->pt_cc_strm_fltr_fct == NULL)
        {
            pt_snk_cpy->pt_cc_strm_fltr_fct = NULL;
            pt_snk_cpy->pv_cc_strm_fltr_tag = pt_snk_tmp->pv_cc_strm_fltr_tag;     
        }
        else
        {
            RPC_CB_TAG(t_rpc_id, pt_nfy_tag, pt_snk_tmp->pt_cc_strm_fltr_fct,
                pt_snk_tmp->pv_cc_strm_fltr_tag);
            pt_snk_cpy->pt_cc_strm_fltr_fct = _x_svctx_strm_comp_filter_fct_wrapper;
            pt_snk_cpy->pv_cc_strm_fltr_tag = pt_nfy_tag;     
        }    

        /* Subtitile */
        if((pt_snk_tmp->e_text_strm_type == ST_SUBTITLE)||(pt_snk_tmp->e_text_strm_type == ST_MM_SUBTITLE))
        {   
#ifndef NO_WGL_INIT 
            /* Use helper plane's canvas. */
            i4_ret = x_wgl_get_canvas(gl_dfb_cc_helper_get_plane(), &h_canvas);
            /*printf("-->RPC subtitle canvas handle:%x \n", (UINT32)h_canvas);*/
            if(WGLR_OK != i4_ret)
            {
                return RPCR_INV_ARGS;
            }
            pt_snk_cpy->u_text_strm.t_subtitle.h_gl_plane = h_canvas;
#endif
            
            if(pt_snk_tmp->u_text_strm.t_subtitle.pt_fltr_fct == NULL)
            {
                pt_snk_cpy->u_text_strm.t_subtitle.pt_fltr_fct = NULL;
                pt_snk_cpy->u_text_strm.t_subtitle.pv_fltr_tag = pt_snk_tmp->u_text_strm.t_subtitle.pv_fltr_tag;     
            }
            else
            {
                RPC_CB_TAG(t_rpc_id, pt_nfy_tag, pt_snk_tmp->u_text_strm.t_subtitle.pt_fltr_fct,
                    pt_snk_tmp->u_text_strm.t_subtitle.pv_fltr_tag);
                pt_snk_cpy->u_text_strm.t_subtitle.pt_fltr_fct = _x_svctx_strm_comp_filter_fct_wrapper;
                pt_snk_cpy->u_text_strm.t_subtitle.pv_fltr_tag = pt_nfy_tag;     
            }
        }
        
/*        memset(&pt_snk_cpy->u_text_strm, 0, sizeof(pt_snk_cpy->u_text_strm));*/
    }
    if(pt_args[4].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy      = _x_svctx_select_nfy_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, (VOID**)(&pt_args[4].u.pv_func), 1, pt_args[5].u.pv_arg);
    }
    
    /*Open the screen */
#ifndef NO_WGL_INIT
    if (NULL_HANDLE == h_cmpb_screen)
    {
        i4_ret = x_gl_screen_open (SN_PRES_MAIN_DISPLAY,
                                   NULL,
                                   NULL,
                                   & h_cmpb_screen);
        if (GL_FAILED(i4_ret))
        {
            x_dbg_stmt("Can't open screen\n");
            pt_return->u.i4_arg = SVCTXR_FAIL;
            return RPCR_OK;
        }
    }
    if (NULL_HANDLE == h_cmpb_plane)
    {
        /*Open the plane */
       i4_ret = x_gl_plane_open (h_cmpb_screen,
                                 1,
                                 &h_cmpb_plane);
       if (GL_FAILED(i4_ret))
       {
           x_dbg_stmt("Can't open plane\n");
           x_handle_free(h_cmpb_screen);
           pt_return->u.i4_arg = SVCTXR_FAIL;
           return RPCR_OK;
       }
       /*Link the plane*/
       i4_ret = x_gl_plane_link (h_cmpb_plane, h_cmpb_screen, TRUE);
       if (GL_FAILED(i4_ret))
       {
           x_dbg_stmt("Can't link plane\n");
           x_handle_free(h_cmpb_screen);
           x_handle_free(h_cmpb_plane);
           pt_return->u.i4_arg = SVCTXR_FAIL;
           return RPCR_OK;
       }
    }
    if (NULL_HANDLE == h_cmpb_canvas)
    {
        x_wgl_get_canvas(h_cmpb_plane, &h_cmpb_canvas);
    }

    pt_snk_cpy->u_text_strm.t_subtitle.h_gl_plane = h_cmpb_canvas;
#endif

    pt_return->e_type = ARG_TYPE_INT32;
    pt_return->u.i4_arg 
        = x_svctx_select_svc(
                (HANDLE_T)pt_args[0].u.ui4_arg,
                pt_args[1].u.ui4_arg,
                (const SRC_DESC_T *)pt_args[2].u.pv_desc,
                pt_snk_cpy,
                pf_nfy,
                pt_nfy_tag);

    if(pt_return->u.i4_arg == SVCTXR_OK)
    {
    }
    else if (pt_nfy_tag != NULL)
    {
        ri_free_cb_tag(pt_nfy_tag);
    }

    if(pt_snk_cpy != NULL){free(pt_snk_cpy);}
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_stream_comp_id(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T *              pt_nfy_tag;
    x_svctx_strm_comp_filter_fct    pf_nfy;
    
    if(ui4_num_args != 6)
    {
        pt_return->e_type   = ARG_TYPE_INT32;        
        pt_return->u.i4_arg = SVCTXR_FAIL;    
        return RPCR_INV_ARGS;
    }

    if(pt_args[2].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy      = _x_svctx_strm_comp_filter_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, (VOID**)(&pt_args[2].u.pv_func), 1, pt_args[3].u.pv_arg);
    }
    
    pt_return->e_type = ARG_TYPE_INT32;
    pt_return->u.i4_arg 
        = x_svctx_get_stream_comp_id(
                (HANDLE_T)pt_args[0].u.ui4_arg,
                pt_args[1].u.ui4_arg,
                pf_nfy,
                pt_nfy_tag,
                pt_args[4].u.pv_desc,
                pt_args[5].u.pv_desc);

    if (pt_nfy_tag != NULL)
    {
        ri_free_cb_tag(pt_nfy_tag);
    }

    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_open(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_open(pt_args[0].u.ps_str, pt_args[1].u.pui4_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_close(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_close(pt_args[0].u.ui4_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_sync_stop_svc(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_sync_stop_svc(pt_args[0].u.ui4_arg, pt_args[1].u.ui4_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_set_volume(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_set_volume(pt_args[0].u.ui4_arg, pt_args[1].u.pv_desc);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_volume(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_volume(pt_args[0].u.ui4_arg, pt_args[1].u.pv_desc);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_set_mute(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_set_mute(pt_args[0].u.ui4_arg, pt_args[1].u.b_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_mute(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_mute(pt_args[0].u.ui4_arg, pt_args[1].u.pb_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_using_scdb_rec(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_using_scdb_rec(pt_args[0].u.ui4_arg, pt_args[1].u.i4_arg, pt_args[2].u.pv_desc);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_select_stream(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T *      pt_nfy_tag;
    x_svctx_select_nfy_fct  pf_nfy;
    
    if(ui4_num_args != 4)
    {
        pt_return->e_type   = ARG_TYPE_INT32;        
        pt_return->u.i4_arg = SVCTXR_FAIL;    
        return RPCR_INV_ARGS;
    }

    if(pt_args[2].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy      = _x_svctx_select_nfy_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, (VOID**)(&pt_args[2].u.pv_func), 1, pt_args[3].u.pv_arg);
    }
    
    pt_return->e_type = ARG_TYPE_INT32;
    pt_return->u.i4_arg 
        = x_svctx_select_stream(
                (HANDLE_T)pt_args[0].u.ui4_arg,
                (const STREAM_COMP_ID_T*)pt_args[1].u.pv_desc,
                pf_nfy,
                pt_nfy_tag);

    if(pt_return->u.i4_arg == SVCTXR_OK)
    {
        ri_bind_cb_tag(
                svctx_get_running_svc_req_strm_cb_handle((HANDLE_T)pt_args[0].u.ui4_arg),
                pt_nfy_tag);
    }
    else if (pt_nfy_tag != NULL)
    {
        ri_free_cb_tag(pt_nfy_tag);
    }

    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_select_mpeg_stream_by_pid(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T *      pt_nfy_tag;
    x_svctx_select_nfy_fct  pf_nfy;
    
    if(ui4_num_args != 5)
    {
        pt_return->e_type   = ARG_TYPE_INT32;        
        pt_return->u.i4_arg = SVCTXR_FAIL;    
        return RPCR_INV_ARGS;
    }
    if(pt_args[3].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy      = _x_svctx_select_nfy_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, (VOID**)(&pt_args[3].u.pv_func), 1, pt_args[4].u.pv_arg);
    }
    
    pt_return->e_type = ARG_TYPE_INT32;
    pt_return->u.i4_arg 
        = x_svctx_select_mpeg_stream_by_pid(
                (HANDLE_T)pt_args[0].u.ui4_arg,
                (STREAM_TYPE_T)pt_args[1].u.i4_arg,
                (MPEG_2_PID_T)pt_args[2].u.ui2_arg,
                pf_nfy,
                pt_nfy_tag);

    if(pt_return->u.i4_arg == SVCTXR_OK)
    {
        ri_bind_cb_tag(
                svctx_get_running_svc_req_strm_cb_handle((HANDLE_T)pt_args[0].u.ui4_arg),
                pt_nfy_tag);
    }
    else if (pt_nfy_tag != NULL)
    {
        ri_free_cb_tag(pt_nfy_tag);
    }

    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_select_mpeg_stream_by_pid_ex(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T * pt_nfy_tag;
    x_svctx_select_nfy_fct  pf_nfy;
    
    if(ui4_num_args != 4)
    {
        pt_return->e_type   = ARG_TYPE_INT32;        
        pt_return->u.i4_arg = SVCTXR_FAIL;    
        return RPCR_INV_ARGS;
    }
    
    if(pt_args[2].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy      = _x_svctx_select_nfy_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, (VOID**)(&pt_args[2].u.pv_func), 1, pt_args[3].u.pv_arg);
    }
    

    pt_return->e_type = ARG_TYPE_INT32;
    pt_return->u.i4_arg 
        = x_svctx_select_mpeg_stream_by_pid_ex(
                (HANDLE_T)pt_args[0].u.ui4_arg,
                (SVCTX_STREAM_INFO_T*)pt_args[1].u.pv_desc,
                pf_nfy,
                pt_nfy_tag);

    if(pt_return->u.i4_arg == SVCTXR_OK)
    {
        ri_bind_cb_tag(
                svctx_get_running_svc_req_strm_cb_handle((HANDLE_T)pt_args[0].u.ui4_arg),
                pt_nfy_tag);
    }
    else if (pt_nfy_tag != NULL)
    {
        ri_free_cb_tag(pt_nfy_tag);
    }

    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_stop_stream(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_stop_stream(pt_args[0].u.ui4_arg, pt_args[1].u.i4_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_sync_stop_stream(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_sync_stop_stream(pt_args[0].u.ui4_arg, pt_args[1].u.i4_arg, pt_args[2].u.ui4_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_select_audio_stream(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T * pt_nfy_tag;
    x_svctx_select_nfy_fct          pf_nfy;
    if(ui4_num_args != 4)
    {
        pt_return->e_type   = ARG_TYPE_INT32;        
        pt_return->u.i4_arg = SVCTXR_FAIL;    
        return RPCR_INV_ARGS;
    }

    if(pt_args[2].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy      = _x_svctx_select_nfy_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, (VOID**)(&pt_args[2].u.pv_func), 1, pt_args[3].u.pv_arg);
    }
    
    pt_return->e_type = ARG_TYPE_INT32;
    pt_return->u.i4_arg 
        = x_svctx_select_audio_stream(
                (HANDLE_T)pt_args[0].u.ui4_arg,
                (const ISO_639_LANG_T*)pt_args[1].u.ps_str,
                pf_nfy,
                pt_nfy_tag);

    if(pt_return->u.i4_arg == SVCTXR_OK)
    {
        ri_bind_cb_tag(
                svctx_get_running_svc_req_strm_cb_handle((HANDLE_T)pt_args[0].u.ui4_arg),
                pt_nfy_tag);
    }
    else if (pt_nfy_tag != NULL)
    {
        ri_free_cb_tag(pt_nfy_tag);
    }

    return RPCR_OK;
}

#ifndef NO_WGL_INIT
static INT32 _hndlr_x_svctx_select_cc_stream(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    HANDLE_T h_canvas;
    INT32 i4_ret;
    RPC_CB_NFY_TAG_T * pt_nfy_tag;
    x_svctx_select_nfy_fct          pf_nfy;
    
    if(ui4_num_args != 5)
    {
        pt_return->e_type   = ARG_TYPE_INT32;        
        pt_return->u.i4_arg = SVCTXR_FAIL;    
        return RPCR_INV_ARGS;
    }

    if(pt_args[3].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy      = _x_svctx_select_nfy_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, (VOID**)(&pt_args[3].u.pv_func), 1, pt_args[4].u.pv_arg);
    }
    
  
    /* Use helper plane's canvas. */
    i4_ret = x_wgl_get_canvas(gl_dfb_cc_helper_get_plane(), &h_canvas);
    
    pt_return->e_type = ARG_TYPE_INT32;
    pt_return->u.i4_arg 
        = x_svctx_select_cc_stream(
                (HANDLE_T)pt_args[0].u.ui4_arg,
                /* We cannot use APP context  handle. */
                h_canvas,
                /*(HANDLE_T)pt_args[1].u.ui4_arg,*/
                (const SM_CC_FILTER_T*)pt_args[2].u.pv_desc,
                pf_nfy,
                pt_nfy_tag);

    if(pt_return->u.i4_arg == SVCTXR_OK)
    {
        ri_bind_cb_tag(
                svctx_get_running_svc_req_strm_cb_handle((HANDLE_T)pt_args[0].u.ui4_arg),
                pt_nfy_tag);
    }
    else if (pt_nfy_tag != NULL)
    {
        ri_free_cb_tag(pt_nfy_tag);
    }

    return RPCR_OK;
}
#endif

static INT32 _hndlr_x_svctx_select_subtitle_stream(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T * pt_nfy_tag;
    x_svctx_select_nfy_fct          pf_nfy;
    if(ui4_num_args != 5)
    {
        pt_return->e_type   = ARG_TYPE_INT32;        
        pt_return->u.i4_arg = SVCTXR_FAIL;    
        return RPCR_INV_ARGS;
    }
    if(pt_args[3].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy      = _x_svctx_select_nfy_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, (VOID**)(&pt_args[3].u.pv_func), 1, pt_args[4].u.pv_arg);
    }
    
    
    pt_return->e_type = ARG_TYPE_INT32;
    pt_return->u.i4_arg 
        = x_svctx_select_subtitle_stream(
                (HANDLE_T)pt_args[0].u.ui4_arg,
                (HANDLE_T)pt_args[1].u.ui4_arg,
                (const ISO_639_LANG_T*)pt_args[2].u.ps_str,
                pf_nfy,
                pt_nfy_tag);

    if(pt_return->u.i4_arg == SVCTXR_OK)
    {
        ri_bind_cb_tag(
                svctx_get_running_svc_req_strm_cb_handle((HANDLE_T)pt_args[0].u.ui4_arg),
                pt_nfy_tag);
    }
    else if (pt_nfy_tag != NULL)
    {
        ri_free_cb_tag(pt_nfy_tag);
    }

    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_select_subtitle_stream_by_comp_id(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T * pt_nfy_tag;
    x_svctx_select_nfy_fct          pf_nfy;
    if(ui4_num_args != 5)
    {
        pt_return->e_type   = ARG_TYPE_INT32;        
        pt_return->u.i4_arg = SVCTXR_FAIL;    
        return RPCR_INV_ARGS;
    }
    if(pt_args[3].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy      = _x_svctx_select_nfy_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, (VOID**)(&pt_args[3].u.pv_func), 1, pt_args[4].u.pv_arg);
    }
    
    pt_return->e_type = ARG_TYPE_INT32;
    pt_return->u.i4_arg 
        = x_svctx_select_subtitle_stream_by_comp_id(
                (HANDLE_T)pt_args[0].u.ui4_arg,
                (HANDLE_T)pt_args[1].u.ui4_arg,
                (const STREAM_COMP_ID_T*)pt_args[2].u.pv_desc,
                pf_nfy,
                pt_nfy_tag);

    if(pt_return->u.i4_arg == SVCTXR_OK)
    {
        ri_bind_cb_tag(
                svctx_get_running_svc_req_strm_cb_handle((HANDLE_T)pt_args[0].u.ui4_arg),
                pt_nfy_tag);
    }
    else if (pt_nfy_tag != NULL)
    {
        ri_free_cb_tag(pt_nfy_tag);
    }

    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_svc_rec_by_channel_id(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_svc_rec_by_channel_id(pt_args[0].u.ui4_arg,
                                                            pt_args[1].u.ui4_arg,
                                                            pt_args[2].u.ui4_arg,
                                                            pt_args[3].u.pui4_arg,
                                                            pt_args[4].u.pv_desc);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_set_video_plane(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_set_video_plane(pt_args[0].u.ui4_arg, pt_args[1].u.ui4_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_set_session_attr(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_set_session_attr(pt_args[0].u.ui4_arg,
                                                   pt_args[1].u.i4_arg,
                                                   pt_args[2].u.ui4_arg,
                                                   pt_args[3].u.pv_arg,
                                                   pt_args[4].u.z_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_session_attr(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_session_attr(pt_args[0].u.ui4_arg,
                                                   pt_args[1].u.i4_arg,
                                                   pt_args[2].u.ui4_arg,
                                                   pt_args[3].u.pv_arg,
                                                   pt_args[4].u.pz_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_set_stream_attr(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_set_stream_attr(pt_args[0].u.ui4_arg,
                                                  pt_args[1].u.i4_arg,
                                                  pt_args[2].u.ui4_arg,
                                                  pt_args[3].u.pv_desc,
                                                  pt_args[4].u.z_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_stream_attr(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_stream_attr(pt_args[0].u.ui4_arg,
                                                  pt_args[1].u.i4_arg,
                                                  pt_args[2].u.ui4_arg,
                                                  pt_args[3].u.pv_desc,
                                                  pt_args[4].u.pz_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_audio_info(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_audio_info(pt_args[0].u.ui4_arg, pt_args[1].u.pv_desc);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_video_src_resolution(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_video_src_resolution(pt_args[0].u.ui4_arg, pt_args[1].u.pv_desc);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_unblock_svc(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_unblock_svc(pt_args[0].u.ui4_arg, pt_args[1].u.b_arg);
    
    return RPCR_OK;
}

static SVC_BLOCK_COND_T
_svctx_svc_block_check_fct_wrapper(
    SVC_BLOCK_CHECK_GUDIE_T     e_check_guide,          /* in  */
    const CHAR*                 ps_src_name,            /* in  */
    const SVL_REC_T*            pt_svl_rec,             /* in  */
    const VOID*                 pv_evn_rating,          /* in  */
    VOID*                       pv_tag                  /* in  */)
{
    RPC_DECL(4, INT32);
    
    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_tag;
    /*EVCTX_ATSC_RATING_LIST_T * pt_list;*/
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_svl_rec, RPC_DESC_SVL_REC_T, NULL));
    /*Fix me*/
    RPC_CHECK(ri_rrctx_rating_list_add_desc((VOID*)pv_evn_rating));

    RPC_ARG_INP(ARG_TYPE_UINT32,   e_check_guide);
    RPC_ARG_INP(ARG_TYPE_REF_STR,  ps_src_name);        
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_svl_rec);        
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pv_evn_rating);    
    
    x_rrctx_base_ctrl_list_lock();
    RPC_DO_CB(pt_nfy_tag->t_id, "svctx_svc_block_check_fct", pt_nfy_tag->pv_cb_addr);
    x_rrctx_base_ctrl_list_unlock();
    
    RPC_RETURN(ARG_TYPE_INT32, SVC_BLOCK_COND_EXCEED_RATING);
}

static INT32 _hndlr_x_svctx_set_default_parameters(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T * pt_nfy_tag;
    svctx_svc_block_check_fct pf_nfy;
    
    if(ui4_num_args != 8)
    {
        pt_return->e_type   = ARG_TYPE_INT32;        
        pt_return->u.i4_arg = SVCTXR_FAIL;    
        return RPCR_INV_ARGS;
    }

    if(pt_args[4].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy      = _svctx_svc_block_check_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, (VOID**)(&pt_args[4].u.pv_func), 1, NULL);
    }
    
    pt_return->e_type = ARG_TYPE_INT32;
    pt_return->u.i4_arg 
        = svctx_set_default_parameters(
                (HANDLE_T)pt_args[0].u.ui4_arg,
                pt_args[1].u.ui4_arg,
                (SCC_VID_MODE_T)pt_args[2].u.ui4_arg,
                pt_args[3].u.b_arg,
                pf_nfy,
                pt_nfy_tag,
                pt_args[5].u.ps_str,
                pt_args[6].u.ps_str,
                pt_args[7].u.ps_str);

    if(pt_return->u.i4_arg == SVCTXR_OK)
    {
    }
    else if (pt_nfy_tag != NULL)
    {
        ri_free_cb_tag(pt_nfy_tag);
    }

    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_set_default_strm_filter(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T * pt_nfy_tag;
    x_svctx_strm_comp_filter_fct pf_nfy;
    
    if(ui4_num_args != 4)
    {
        pt_return->e_type   = ARG_TYPE_INT32;        
        pt_return->u.i4_arg = SVCTXR_FAIL;    
        return RPCR_INV_ARGS;
    }
    if(pt_args[2].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy      = _x_svctx_strm_comp_filter_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, (VOID**)(&pt_args[2].u.pv_func), 1, pt_args[3].u.pv_arg);
    }
    
    pt_return->e_type = ARG_TYPE_INT32;
    pt_return->u.i4_arg 
        = x_svctx_set_default_strm_filter(
                (HANDLE_T)pt_args[0].u.ui4_arg,
                (HANDLE_T)pt_args[1].u.i4_arg,
                pf_nfy,
                pt_nfy_tag);

    if(pt_return->u.i4_arg == SVCTXR_OK)
    {
    }
    else if (pt_nfy_tag != NULL)
    {
        ri_free_cb_tag(pt_nfy_tag);
    }

    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_set_screen_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_set_screen_mode(pt_args[0].u.ui4_arg, pt_args[1].u.ui4_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_screen_mode(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_screen_mode(pt_args[0].u.ui4_arg, (SCC_VID_SCREEN_MODE_T*)pt_args[1].u.pui4_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_signal_level(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_signal_level(pt_args[0].u.ui4_arg, pt_args[1].u.pui1_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_dbm_signal_level(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_dbm_signal_level(pt_args[0].u.ui4_arg, pt_args[1].u.pi2_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_display_resolution(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_display_resolution(pt_args[0].u.ui4_arg, pt_args[1].u.pv_desc);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_set_display_resolution(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_set_display_resolution(pt_args[0].u.ui4_arg, pt_args[1].u.pv_desc);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_is_signal_loss(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_is_signal_loss(pt_args[0].u.ui4_arg, pt_args[1].u.pb_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_display_aspect_ratio(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_display_aspect_ratio(pt_args[0].u.ui4_arg, (SCC_DISP_ASPECT_RATIO_T*)pt_args[1].u.pui4_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_set_display_aspect_ratio(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_set_display_aspect_ratio(pt_args[0].u.ui4_arg, pt_args[1].u.ui4_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_scdb_num_recs(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_scdb_num_recs(pt_args[0].u.ui4_arg, pt_args[1].u.i4_arg, pt_args[2].u.pui2_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_using_strm_data(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_using_strm_data(pt_args[0].u.ui4_arg,
                                                      pt_args[1].u.i4_arg,
                                                      pt_args[2].u.pv_desc,
                                                      pt_args[3].u.pv_desc);
    
    return RPCR_OK;
}


static VOID 
_x_svctx_listener_nfy_fct_wrapper(
    HANDLE_T                    h_svctx,                /* in  */
    SVCTX_COND_T                e_nfy_cond,             /* in  */
    SVCTX_NTFY_CODE_T           e_code,                 /* in  */
    STREAM_TYPE_T               e_stream_type,          /* in  */
    VOID*                       pv_nfy_tag              /* in  */)
{    
    RPC_DECL_VOID(5);
    
    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_nfy_tag;
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);
    RPC_ARG_INP(ARG_TYPE_UINT32,   e_nfy_cond);        
    RPC_ARG_INP(ARG_TYPE_UINT32,   e_code);        
    RPC_ARG_INP(ARG_TYPE_INT32,    e_stream_type);    
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_svctx_listener_nfy_fct", pt_nfy_tag->pv_cb_addr);
    
    RPC_RETURN_VOID;    
}


static INT32 _hndlr_x_svctx_set_listener(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T * pt_nfy_tag;
    x_svctx_listener_nfy_fct        pf_nfy;
    if(ui4_num_args != 3)
    {
        pt_return->e_type   = ARG_TYPE_INT32;        
        pt_return->u.i4_arg = SVCTXR_FAIL;    
        return RPCR_INV_ARGS;
    }
    if(pt_args[1].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy      = _x_svctx_listener_nfy_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, (VOID**)(&pt_args[1].u.pv_func), 1, pt_args[2].u.pv_arg);
    }

    pt_return->e_type = ARG_TYPE_INT32;
    pt_return->u.i4_arg 
        = x_svctx_set_listener(
                (HANDLE_T)pt_args[0].u.ui4_arg,
                pf_nfy,
                pt_nfy_tag);

    if(pt_return->u.i4_arg == SVCTXR_OK)
    {
        ri_bind_cb_tag((HANDLE_T)pt_args[0].u.ui4_arg, pt_nfy_tag);
    }
    else if (pt_nfy_tag != NULL)
    {
        ri_free_cb_tag(pt_nfy_tag);
    }

    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_surf_svc(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_surf_svc(pt_args[0].u.ui4_arg,
                                           pt_args[1].u.ui4_arg,
                                           pt_args[2].u.ui4_arg,
                                           pt_args[3].u.ui4_arg,
                                           pt_args[4].u.pv_desc);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_grab_ownership(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_grab_ownership(pt_args[0].u.ui4_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_release_ownership(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_release_ownership(pt_args[0].u.ui4_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_cp_info(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_cp_info(pt_args[0].u.ui4_arg, pt_args[1].u.i4_arg, pt_args[2].u.pv_desc);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_ca_info(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_ca_info(pt_args[0].u.ui4_arg, pt_args[1].u.i4_arg, pt_args[2].u.pv_desc);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_set(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    if ((SVCTX_SET_TYPE_T)pt_args[1].u.ui4_arg != SVCTX_COMMON_SET_DEFALUT_BLOCK_CHECK_FCT)
    {
        pt_return->u.i4_arg = x_svctx_set(pt_args[0].u.ui4_arg,
                                          pt_args[1].u.ui4_arg,
                                          pt_args[2].u.pv_desc,
                                          pt_args[3].u.z_arg);
    }
    else
    {
        RPC_CB_NFY_TAG_T *          pt_nfy_tag;
        svctx_svc_block_check_fct   pf_nfy;
        SVCTX_BLOCK_CHECK_FCT_TAG_T t_block_check_tag;

        if(pt_args[2].u.pv_func == NULL)
        {
            pf_nfy      = NULL;
            pt_nfy_tag  = NULL;
        }
        else
        {
            pf_nfy      = _svctx_svc_block_check_fct_wrapper;
            pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, (VOID**)(&pt_args[2].u.pv_func), 1, NULL);
        }

        t_block_check_tag.pf_block_check_fct = pf_nfy;
        t_block_check_tag.pv_tag             = pt_nfy_tag;

        pt_return->u.i4_arg = x_svctx_set(pt_args[0].u.ui4_arg,
                                          SVCTX_COMMON_SET_DEFALUT_BLOCK_CHECK_FCT_TAG,
                                          &t_block_check_tag,
                                          sizeof(t_block_check_tag));
    }
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get(pt_args[0].u.ui4_arg,
                                      pt_args[1].u.ui4_arg,
                                      pt_args[2].u.pv_arg,
                                      pt_args[3].u.pz_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_freeze(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_freeze(pt_args[0].u.ui4_arg, pt_args[1].u.ui4_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_unfreeze(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_unfreeze(pt_args[0].u.ui4_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_is_frozen(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_is_frozen(pt_args[0].u.ui4_arg, pt_args[1].u.pb_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_current_event_info_len(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_current_event_info_len(pt_args[0].u.ui4_arg,
                                                             pt_args[1].u.ui4_arg,
                                                             pt_args[2].u.pv_desc,
                                                             pt_args[3].u.pz_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_current_event_info(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_current_event_info(pt_args[0].u.ui4_arg,
                                                         pt_args[1].u.ui4_arg,
                                                         pt_args[2].u.pv_desc,
                                                         pt_args[3].u.pz_arg,
                                                         pt_args[4].u.pv_desc);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_mapped_dev_info(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 7)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_mapped_dev_info(pt_args[0].u.ui2_arg,
                                                      pt_args[1].u.pui4_arg,
                                                      pt_args[2].u.pui4_arg,
                                                      pt_args[3].u.pi1_arg,
                                                      pt_args[4].u.pui4_arg,
                                                      pt_args[5].u.pui4_arg,
                                                      pt_args[6].u.pi1_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_device_profile(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_device_profile(pt_args[0].u.ui4_arg, pt_args[1].u.pv_desc);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_device_profile_by_drv_type(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_device_profile_by_drv_type(pt_args[0].u.ui2_arg, pt_args[1].u.pv_desc);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_well_defined_video_heigh(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_well_defined_video_heigh(pt_args[0].u.ui4_arg, pt_args[1].u.pui4_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_cc_capability(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_cc_capability(pt_args[0].u.ui4_arg, (SVCTX_CAPABILITY_T*)pt_args[1].u.pui4_arg);
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_subtitle_capability(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_subtitle_capability(pt_args[0].u.ui4_arg, (SVCTX_CAPABILITY_T*)pt_args[1].u.pui4_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_teletext_capability(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_teletext_capability(pt_args[0].u.ui4_arg, (SVCTX_CAPABILITY_T*)pt_args[1].u.pui4_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_event_capability(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_event_capability(pt_args[0].u.ui4_arg, pt_args[1].u.ui4_arg, (SVCTX_CAPABILITY_T*)pt_args[2].u.pui4_arg);
    
    return RPCR_OK;
}


static VOID 
_x_scdb_nfy_fct_wrapper(
    HANDLE_T                    h_scdb,                 /* in  */
    SCDB_COND_T                 e_cond,                 /* in  */
    UINT32                      ui4_reason,             /* in  */
    VOID*                       pv_tag,                 /* in  */
    UINT32                      ui4_data                /* in  */)
{    
    RPC_DECL_VOID(5);
    
    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_tag;
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_scdb);
    RPC_ARG_INP(ARG_TYPE_UINT32,   e_cond);        
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_reason);            
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_data);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_scdb_nfy_fct", pt_nfy_tag->pv_cb_addr);
    
    RPC_RETURN_VOID;    
}


static INT32 _hndlr_x_svctx_open_scdb(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T * pt_nfy_tag;
    x_scdb_nfy_fct                  pf_nfy;
    if(ui4_num_args != 5)
    {
        pt_return->e_type   = ARG_TYPE_INT32;        
        pt_return->u.i4_arg = SVCTXR_FAIL;    
        return RPCR_INV_ARGS;
    }
    if(pt_args[4].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy      = _x_scdb_nfy_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, (VOID**)(&pt_args[4].u.pv_func), 1, pt_args[3].u.pv_arg);
    }
    
    pt_return->e_type = ARG_TYPE_INT32;
    pt_return->u.i4_arg 
        = x_svctx_open_scdb(
                (HANDLE_T)pt_args[0].u.ui4_arg,
                (STREAM_TYPE_T)pt_args[1].u.i4_arg,
                (HANDLE_T*)pt_args[2].u.pui4_arg,
                pt_nfy_tag,
                pf_nfy);

    if(pt_return->u.i4_arg == SVCTXR_OK)
    {
        ri_bind_cb_tag(*((HANDLE_T *)pt_args[2].u.pui4_arg), pt_nfy_tag);
    }
    else if (pt_nfy_tag != NULL)
    {
        ri_free_cb_tag(pt_nfy_tag);
    }

    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_close_scdb(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_close_scdb(pt_args[0].u.ui4_arg);
    
    return RPCR_OK;
}





static INT32 _hndlr_x_svctx_open_stream(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T * pt_nfy_tag;
    x_stream_nfy_fct                pf_nfy;
    
    if(ui4_num_args != 8)
    {
        pt_return->e_type   = ARG_TYPE_INT32;        
        pt_return->u.i4_arg = SVCTXR_FAIL;    
        return RPCR_INV_ARGS;
    }
    if(pt_args[4].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy      = ri_x_stream_nfy_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, (VOID**)(&pt_args[4].u.pv_func), 1, pt_args[5].u.pv_arg);
    }
    
    pt_return->e_type = ARG_TYPE_INT32;
    pt_return->u.i4_arg 
        = x_svctx_open_stream(
                (HANDLE_T)pt_args[0].u.ui4_arg,
                (STREAM_TYPE_T)pt_args[1].u.i4_arg,
                (const STREAM_COMP_ID_T*)pt_args[2].u.pv_desc,
                (const SM_COMMAND_T*)pt_args[3].u.pv_desc,
                pf_nfy,
                pt_nfy_tag,
                pt_args[6].u.b_arg,
                (HANDLE_T*)pt_args[7].u.pui4_arg);

    if(pt_return->u.i4_arg == SVCTXR_OK)
    {
        ri_bind_cb_tag(*((HANDLE_T *)pt_args[7].u.pui4_arg), pt_nfy_tag);
    }
    else if (pt_nfy_tag != NULL)
    {
        ri_free_cb_tag(pt_nfy_tag);
    }

    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_open_stream_ex(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T * pt_nfy_tag;
    x_stream_nfy_fct pf_nfy;
    
    if(ui4_num_args != 9)
    {
        pt_return->e_type   = ARG_TYPE_INT32;        
        pt_return->u.i4_arg = SVCTXR_FAIL;    
        return RPCR_INV_ARGS;
    }
    if(pt_args[4].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy      = ri_x_stream_nfy_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, (VOID**)(&pt_args[4].u.pv_func), 1, pt_args[5].u.pv_arg);
    }
    
    pt_return->e_type = ARG_TYPE_INT32;
    pt_return->u.i4_arg 
        = x_svctx_open_stream_ex(
                (HANDLE_T)pt_args[0].u.ui4_arg,
                (STREAM_TYPE_T)pt_args[1].u.i4_arg,
                (const STREAM_COMP_ID_T*)pt_args[2].u.pv_desc,
                (const SM_COMMAND_T*)pt_args[3].u.pv_desc,
                pf_nfy,
                pt_nfy_tag,
                pt_args[6].u.b_arg,
                pt_args[7].u.ui4_arg,
                (HANDLE_T*)pt_args[8].u.pui4_arg);

    if(pt_return->u.i4_arg == SVCTXR_OK)
    {
        ri_bind_cb_tag(*((HANDLE_T *)pt_args[8].u.pui4_arg), pt_nfy_tag);
    }
    else if (pt_nfy_tag != NULL)
    {
        ri_free_cb_tag(pt_nfy_tag);
    }

    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_close_stream(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_close_stream(pt_args[0].u.ui4_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_conn_attr(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_conn_attr(pt_args[0].u.ui4_arg,
                                                pt_args[1].u.i4_arg,
                                                pt_args[2].u.ui4_arg,
                                                pt_args[3].u.pv_desc);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_set_conn_attr(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_set_conn_attr(pt_args[0].u.ui4_arg,
                                                pt_args[1].u.i4_arg,
                                                pt_args[2].u.ui4_arg,
                                                pt_args[3].u.pv_desc);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_set_analog_tuner_out_attr(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_set_analog_tuner_out_attr(pt_args[0].u.pv_desc);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_svc_block_check(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_svc_block_check(pt_args[0].u.ui4_arg,
                                                  pt_args[1].u.ui4_arg,
                                                  pt_args[2].u.ps_str,
                                                  pt_args[3].u.pv_desc,
                                                  pt_args[4].u.pv_desc);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_set_ad_state(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_set_ad_state(pt_args[0].u.ui4_arg, pt_args[1].u.b_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_get_ad_state(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_get_ad_state(pt_args[0].u.ui4_arg, pt_args[1].u.pb_arg);
    
    return RPCR_OK;
}

#ifdef MW_TV_AV_BYPASS_SUPPORT
static INT32 _hndlr_x_svctx_create_associate(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_create_associate(pt_args[0].u.ps_str, pt_args[1].u.pv_desc, pt_args[2].u.pui4_arg);
    
    return RPCR_OK;
}

#ifdef MW_PIP_POP_SUPPORT
static INT32 _hndlr_x_svctx_add_associate(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_add_associate(pt_args[0].u.ui4_arg, pt_args[1].u.ui4_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_add_exclusion(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_op,    
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_add_exclusion(pt_args[0].u.ui4_arg, pt_args[1].u.ui4_arg, pt_args[2].u.ui4_arg);
    
    return RPCR_OK;
}
#endif
#endif

static BOOL ri_svctx_same_select_listener_nfy_fct(
        x_svctx_select_nfy_fct      pf_sel_nfy,
        VOID*                       pv_sel_tag,
        x_svctx_listener_nfy_fct    pf_lsn_nfy,
        VOID*                       pv_lsn_tag);
static VOID ri_svctx_free_rpc_cb_tag_if_needed(
        VOID*                       pv_nfy,
        VOID*                       pv_nfy_tag);
static VOID* ri_svctx_get_block_check_fct_cb_addr(
        svctx_svc_block_check_fct   pf_block_check_fct,
        VOID*                       pv_block_check_tag);

INT32 c_rpc_reg_svctx_op_hndlrs()
{
    RPC_REG_OP_HNDLR(x_svctx_is_running);
    RPC_REG_OP_HNDLR(x_svctx_stop_svc);
    RPC_REG_OP_HNDLR(x_svctx_get_stream_comp_id);
    RPC_REG_OP_HNDLR(x_svctx_select_svc);   
    RPC_REG_OP_HNDLR(x_svctx_open);
    RPC_REG_OP_HNDLR(x_svctx_close);
    RPC_REG_OP_HNDLR(x_svctx_sync_stop_svc);
    RPC_REG_OP_HNDLR(x_svctx_set_volume);
    RPC_REG_OP_HNDLR(x_svctx_get_volume);
    RPC_REG_OP_HNDLR(x_svctx_set_mute);
    RPC_REG_OP_HNDLR(x_svctx_get_mute);
    RPC_REG_OP_HNDLR(x_svctx_get_using_scdb_rec);
    RPC_REG_OP_HNDLR(x_svctx_select_stream);
    RPC_REG_OP_HNDLR(x_svctx_select_mpeg_stream_by_pid);
    RPC_REG_OP_HNDLR(x_svctx_select_mpeg_stream_by_pid_ex);
    RPC_REG_OP_HNDLR(x_svctx_stop_stream);
    RPC_REG_OP_HNDLR(x_svctx_sync_stop_stream);
    RPC_REG_OP_HNDLR(x_svctx_select_audio_stream);
#ifndef NO_WGL_INIT
    RPC_REG_OP_HNDLR(x_svctx_select_cc_stream);
#endif
    RPC_REG_OP_HNDLR(x_svctx_select_subtitle_stream);
    RPC_REG_OP_HNDLR(x_svctx_select_subtitle_stream_by_comp_id);
    RPC_REG_OP_HNDLR(x_svctx_get_svc_rec_by_channel_id);
    RPC_REG_OP_HNDLR(x_svctx_set_video_plane);
    RPC_REG_OP_HNDLR(x_svctx_set_session_attr);
    RPC_REG_OP_HNDLR(x_svctx_get_session_attr);
    RPC_REG_OP_HNDLR(x_svctx_set_stream_attr);
    RPC_REG_OP_HNDLR(x_svctx_get_stream_attr);
    RPC_REG_OP_HNDLR(x_svctx_get_audio_info);
    RPC_REG_OP_HNDLR(x_svctx_get_video_src_resolution);
    RPC_REG_OP_HNDLR(x_svctx_unblock_svc);
    RPC_REG_OP_HNDLR(x_svctx_set_default_parameters);
    RPC_REG_OP_HNDLR(x_svctx_set_default_strm_filter);
    RPC_REG_OP_HNDLR(x_svctx_set_screen_mode);
    RPC_REG_OP_HNDLR(x_svctx_get_screen_mode);
    RPC_REG_OP_HNDLR(x_svctx_get_signal_level);
    RPC_REG_OP_HNDLR(x_svctx_get_dbm_signal_level);
    RPC_REG_OP_HNDLR(x_svctx_get_display_resolution);
    RPC_REG_OP_HNDLR(x_svctx_set_display_resolution);
    RPC_REG_OP_HNDLR(x_svctx_is_signal_loss);
    RPC_REG_OP_HNDLR(x_svctx_get_display_aspect_ratio);
    RPC_REG_OP_HNDLR(x_svctx_set_display_aspect_ratio);
    RPC_REG_OP_HNDLR(x_svctx_get_scdb_num_recs);
    RPC_REG_OP_HNDLR(x_svctx_get_using_strm_data);
    RPC_REG_OP_HNDLR(x_svctx_set_listener);
    RPC_REG_OP_HNDLR(x_svctx_surf_svc);
    RPC_REG_OP_HNDLR(x_svctx_grab_ownership);
    RPC_REG_OP_HNDLR(x_svctx_release_ownership);
    RPC_REG_OP_HNDLR(x_svctx_get_cp_info);
    RPC_REG_OP_HNDLR(x_svctx_get_ca_info);
    RPC_REG_OP_HNDLR(x_svctx_set);
    RPC_REG_OP_HNDLR(x_svctx_get);
    RPC_REG_OP_HNDLR(x_svctx_freeze);
    RPC_REG_OP_HNDLR(x_svctx_unfreeze);
    RPC_REG_OP_HNDLR(x_svctx_is_frozen);
    RPC_REG_OP_HNDLR(x_svctx_get_current_event_info_len);
    RPC_REG_OP_HNDLR(x_svctx_get_current_event_info);
    RPC_REG_OP_HNDLR(x_svctx_get_mapped_dev_info);
    RPC_REG_OP_HNDLR(x_svctx_get_device_profile);
    RPC_REG_OP_HNDLR(x_svctx_get_device_profile_by_drv_type);
    RPC_REG_OP_HNDLR(x_svctx_get_well_defined_video_heigh);
    RPC_REG_OP_HNDLR(x_svctx_get_cc_capability);
    RPC_REG_OP_HNDLR(x_svctx_get_subtitle_capability);
    RPC_REG_OP_HNDLR(x_svctx_get_teletext_capability);
    RPC_REG_OP_HNDLR(x_svctx_get_event_capability);
    RPC_REG_OP_HNDLR(x_svctx_open_scdb);
    RPC_REG_OP_HNDLR(x_svctx_close_scdb);
    RPC_REG_OP_HNDLR(x_svctx_open_stream);
    RPC_REG_OP_HNDLR(x_svctx_open_stream_ex);
    RPC_REG_OP_HNDLR(x_svctx_close_stream);
    RPC_REG_OP_HNDLR(x_svctx_get_conn_attr);
    RPC_REG_OP_HNDLR(x_svctx_set_conn_attr);
    RPC_REG_OP_HNDLR(x_svctx_set_analog_tuner_out_attr);
    RPC_REG_OP_HNDLR(x_svctx_svc_block_check);
    RPC_REG_OP_HNDLR(x_svctx_set_ad_state);
    RPC_REG_OP_HNDLR(x_svctx_get_ad_state);
#ifdef MW_TV_AV_BYPASS_SUPPORT
    RPC_REG_OP_HNDLR(x_svctx_create_associate);
#ifdef MW_PIP_POP_SUPPORT
    RPC_REG_OP_HNDLR(x_svctx_add_associate);
    RPC_REG_OP_HNDLR(x_svctx_add_exclusion);
#endif
#endif

    svctx_reg_rpc_handler_util_func(
            ri_svctx_same_select_listener_nfy_fct,
            ri_svctx_free_rpc_cb_tag_if_needed,
            ri_svctx_get_block_check_fct_cb_addr,
            NULL
            );

    return RPCR_OK;
}


static BOOL ri_svctx_same_select_listener_nfy_fct(
        x_svctx_select_nfy_fct      pf_sel_nfy,
        VOID*                       pv_sel_tag,
        x_svctx_listener_nfy_fct    pf_lsn_nfy,
        VOID*                       pv_lsn_tag)
{
    if (pf_sel_nfy == _x_svctx_select_nfy_fct_wrapper &&
        pf_lsn_nfy == _x_svctx_listener_nfy_fct_wrapper)
    {
        RPC_CB_NFY_TAG_T *      pt_sel_tag = (RPC_CB_NFY_TAG_T *) pv_sel_tag;
        RPC_CB_NFY_TAG_T *      pt_lsn_tag = (RPC_CB_NFY_TAG_T *) pv_lsn_tag;

        if (pt_sel_tag == NULL || pt_lsn_tag == NULL)
        {
            return FALSE;
        }
        else
        {
            return pt_sel_tag->pv_cb_addr == pt_lsn_tag->pv_cb_addr;
        }
    }
    else
    {
        return pf_sel_nfy == pf_lsn_nfy;
    }
}

static VOID ri_svctx_free_rpc_cb_tag_if_needed(
        VOID*                       pv_nfy,
        VOID*                       pv_nfy_tag)
{
    if (pv_nfy_tag != NULL && pv_nfy != NULL &&
        ((x_svctx_strm_comp_filter_fct) pv_nfy == _x_svctx_strm_comp_filter_fct_wrapper ||
         (x_svctx_select_nfy_fct)       pv_nfy == _x_svctx_select_nfy_fct_wrapper       ||
         (svctx_svc_block_check_fct)    pv_nfy == _svctx_svc_block_check_fct_wrapper    ||
         (x_svctx_listener_nfy_fct)     pv_nfy == _x_svctx_listener_nfy_fct_wrapper
        )
       )
    {
        ri_free_cb_tag((RPC_CB_NFY_TAG_T *)pv_nfy_tag);
    }
}

static VOID* ri_svctx_get_block_check_fct_cb_addr(
        svctx_svc_block_check_fct   pf_block_check_fct,
        VOID*                       pv_block_check_tag)
{
    VOID*   pv_result = NULL;
    if (pf_block_check_fct != _svctx_svc_block_check_fct_wrapper)
    {
        /* when SVCTX_SPLIT_SUPPORT is enabled, pf_block_check_fct is either
         * _svctx_svc_block_check_fct_wrapper or orig_cb_wrapper */
        pv_result = pv_block_check_tag;
    }
    else if (pv_block_check_tag == NULL)
    {
        pv_result = NULL;
    }
    else
    {
        pv_result = ((RPC_CB_NFY_TAG_T *)pv_block_check_tag)->pv_cb_addr;
    }
    return pv_result;
}
