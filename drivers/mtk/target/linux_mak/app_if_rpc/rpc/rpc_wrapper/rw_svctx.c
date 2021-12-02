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

#include "rpc.h"
#include "ri_common.h"

#include "ri_desc.h"

/* added to resolve the compiling warnings */
extern INT32 ri_event_key_add_ref_desc(
                     EVCTX_KEY_TYPE_T    e_key_type, 
                     VOID*               pv_key_info,
                     UINT32              ui4_arg_idx);

INT32 _hndlr_x_svctx_select_nfy_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{  
    if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }
    /*Maybe not needed*/
    pt_return->e_type = ARG_TYPE_VOID;
    
    ((x_svctx_select_nfy_fct)pv_cb_addr)(
            (HANDLE_T)pt_args[0].u.ui4_arg,
            (SVCTX_COND_T)pt_args[1].u.ui4_arg,
            (SVCTX_NTFY_CODE_T)pt_args[2].u.ui4_arg,
            (STREAM_TYPE_T)pt_args[3].u.i4_arg,
            pt_args[4].u.pv_arg);

    return RPCR_OK;
}


INT32 c_svctx_select_svc(
                    HANDLE_T                        h_svctx,            /* in  */
                    UINT32                          ui4_stream_mode,    /* in  */
                    const SRC_DESC_T*               pt_src_desc,        /* in  */
                    const SNK_DESC_T*               pt_snk_desc,        /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,             /* in  */
                    VOID*                           pv_nfy_tag          /* in  */
                    )
{
    BOOL b_log = rpcu_tl_log_start(); 
    RPC_DECL(6, INT32);    
    if(pt_src_desc != NULL)
    {
          
        RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_src_desc->ps_conn_src_name));
        /* NEVER Do this! They don't put it be NULL though it is deprecated*/
        /*RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_src_desc->ps_conn_src_type));*/
        RPC_CHECK(RPC_ADD_REF_VAL(RPC_DEFAULT_ID, &pt_src_desc->ps_conn_src_type));

        switch(pt_src_desc->e_type)
        {
            case SRC_TYPE_AVC:
                RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_src_desc->pv_details, RPC_DESC_SRC_AVC_T, NULL));
                break;
            case SRC_TYPE_BRDCST_FREQ:
                RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_src_desc->pv_details, RPC_DESC_SRC_FREQ_INFO_T, NULL));
                break;
            case SRC_TYPE_BRDCST_SVL:
                RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_src_desc->pv_details, RPC_DESC_SRC_SVL_INFO_T, NULL));                
                break;   
			case SRC_TYPE_MEDIA_STORGE:
			case SRC_TYPE_MEDIA_NETWORK:
                RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_src_desc->pv_details, RPC_DESC_SRC_MM_INFO_T, NULL));                
				break;   
			case SRC_TYPE_MEDIA_MEM_CLIP:
                RI_LOG(" ~~!!!!!!!! c_svctx_select_svc NOT to be deal with pt_src_desc->e_type: %d\n", pt_src_desc->e_type);
				break;   
			case SRC_TYPE_MEDIA_TIME_SHIFT:
                RI_LOG(" ~~!!!!!!!! c_svctx_select_svc NOT to be deal with pt_src_desc->e_type: %d\n", pt_src_desc->e_type);
				break;   
			case SRC_TYPE_MEDIA_PVR:
                RI_LOG(" ~~!!!!!!!! c_svctx_select_svc NOT to be deal with pt_src_desc->e_type: %d\n", pt_src_desc->e_type);
				break;
            default:
                RI_LOG(" ~~!!!!!!!! c_svctx_select_svc NOT SUPPORT pt_src_desc->e_type: %d\n", pt_src_desc->e_type);
                break;
        }
        RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_src_desc, RPC_DESC_SRC_DESC_T, NULL));
    }

    if(pt_snk_desc != NULL)
    {   
        /* Deprecated should be removed */
        RPC_CHECK(RPC_ADD_REF_VAL(RPC_DEFAULT_ID, &pt_snk_desc->pt_volume_info));
        RPC_CHECK(RPC_ADD_REF_VAL(RPC_DEFAULT_ID, &pt_snk_desc->pt_cc_filter));
        RPC_CHECK(RPC_ADD_REF_VAL(RPC_DEFAULT_ID, &pt_snk_desc->pv_audio_strm_fltr_tag));

        RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_snk_desc->ps_snk_grp_name));
        RPC_CHECK(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_snk_desc->ps_snk_grp_name_aux));
        RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_snk_desc, RPC_DESC_SNK_DESC_T, NULL));
        /* TMP not support the txt/cc */
    }
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_stream_mode);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_src_desc);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_snk_desc);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_nfy_tag);    

    RPC_DO_OP("x_svctx_select_svc");
    rpcu_tl_log_end(b_log); 
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
	
}

INT32 c_svctx_stop_svc(
                    HANDLE_T                        h_svctx             /* in  */
                   )
{
    ARG_DESC_T at_args[1];
    ARG_DESC_T t_ret;

    at_args[0].e_type       = ARG_TYPE_UINT32   | ARG_DIR_INP;
    at_args[0].u.ui4_arg    = h_svctx;

    if(rpc_do_op(
            RPC_DEFAULT_ID,
            "x_svctx_stop_svc",
            1,
            at_args,
            &t_ret,
            0xffffffff) == RPCR_OK)
    {
        if(t_ret.e_type == ARG_TYPE_INT32)
        {
            return t_ret.u.i4_arg;        
        }
        else
        {
            return SVCTXR_FAIL;
        }
    }
    else
    {
        return SVCTXR_FAIL;
    }
}

INT32 c_svctx_is_running (
                    HANDLE_T                    h_svctx,                /* in */
                    BOOL*                       pb_result               /* out */
                    )
{    
    RPC_DECL(2, INT32);
        
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL, pb_result);    

    RPC_DO_OP("x_svctx_is_running");

    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}


INT32 _hndlr_x_svctx_strm_comp_filter_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }
    /*Maybe not needed*/
    pt_return->e_type  = ARG_TYPE_BOOL;
    
    pt_return->u.b_arg = ((x_svctx_strm_comp_filter_fct)pv_cb_addr)(
            pt_args[0].u.ui2_arg,
            pt_args[1].u.ui2_arg,
            (STREAM_COMP_ID_T *)pt_args[2].u.pv_desc,
            (SCDB_REC_T *)pt_args[3].u.pv_desc,
            pt_args[4].u.pv_arg);

    return RPCR_OK;
}


INT32 c_svctx_get_stream_comp_id(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_strm_type,        /* in  */
                    x_svctx_strm_comp_filter_fct    pf_filter,          /* in  */
                    VOID*                           pv_fltr_tag,        /* in  */
                    STREAM_COMP_ID_T*               pt_comp_id,         /* out */
                    SCDB_REC_T*                     pt_scdb_rec         /* out */
                    )
{
    RPC_DECL(6, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_comp_id, RPC_DESC_STREAM_COMP_ID_T, NULL));
    RPC_CHECK(RPC_ADD_REF_VAL(RPC_DEFAULT_ID, &pt_comp_id->pv_stream_tag));
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_scdb_rec, RPC_DESC_SCDB_REC_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_INP(ARG_TYPE_INT32,    e_strm_type);    
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_filter);    
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_fltr_tag);        
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_comp_id);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_scdb_rec);
    
    RPC_DO_OP("x_svctx_get_stream_comp_id");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

static INT32 _svctx_open(
                    const CHAR*                 ps_svctx_name,          /* in  */
                    HANDLE_T*                   ph_svctx                /* out */
                    )
{
    RPC_DECL(2, INT32);
    
    RPC_ARG_INP(ARG_TYPE_REF_STR,    ps_svctx_name);    
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_svctx);    
    
    RPC_DO_OP("x_svctx_open");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_open(
                    const CHAR*                     ps_svctx_name,      /* in  */
                    HANDLE_T*                       ph_svctx            /* out */
                    )
{
    INT32     i4_ret;

    i4_ret = _svctx_open(
                ps_svctx_name,
                ph_svctx
                );

    return i4_ret;
}

INT32 c_svctx_close(
                    HANDLE_T                        h_svctx             /* in  */
                    )
{
    RPC_DECL(1, INT32);
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_svctx);    
    
    RPC_DO_OP("x_svctx_close");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_sync_stop_svc(
                    HANDLE_T                        h_svctx,            /* in  */
                    UINT32                          ui4_time            /* in  */
                    )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_svctx);    
    RPC_ARG_INP(ARG_TYPE_UINT32, ui4_time);    
    
    RPC_DO_OP("x_svctx_sync_stop_svc");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_set_volume(
                    HANDLE_T                        h_svctx,            /* in  */
                    const SCC_AUD_VOLUME_INFO_T*    pt_volume_info      /* in  */
                    )
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_volume_info, RPC_DESC_SCC_AUD_VOLUME_INFO_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_volume_info);    
    
    RPC_DO_OP("x_svctx_set_volume");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_volume(
                    HANDLE_T                        h_svctx,            /* in  */
                    SCC_AUD_VOLUME_INFO_T*          pt_volume_info      /* out */
                    )
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_volume_info, RPC_DESC_SCC_AUD_VOLUME_INFO_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_volume_info);    
    
    RPC_DO_OP("x_svctx_get_volume");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_set_mute(
                    HANDLE_T                        h_svctx,            /* in  */
                    BOOL                            b_is_mute           /* in  */
                    )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_svctx);    
    RPC_ARG_INP(ARG_TYPE_BOOL,   b_is_mute);    
    
    RPC_DO_OP("x_svctx_set_mute");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_mute(
                    HANDLE_T                        h_svctx,            /* in  */
                    BOOL*                           pb_is_mute          /* out */
                    )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL, pb_is_mute);
    
    RPC_DO_OP("x_svctx_get_mute");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_using_scdb_rec(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_strm_type,        /* in  */
                    SCDB_REC_T*                     pt_scdb_rec         /* out */
                    )
{
    RPC_DECL(3, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_scdb_rec, RPC_DESC_SCDB_REC_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_INP(ARG_TYPE_INT32,    e_strm_type);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_scdb_rec);    
    
    RPC_DO_OP("x_svctx_get_using_scdb_rec");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_select_stream(
                    HANDLE_T                        h_svctx,            /* in  */
                    const STREAM_COMP_ID_T*         pt_comp_id,         /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,             /* in  */
                    VOID*                           pv_nfy_tag          /* in  */
                    )
{
    RPC_DECL(4, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_comp_id, RPC_DESC_STREAM_COMP_ID_T, NULL));
    RPC_CHECK(RPC_ADD_REF_VAL(RPC_DEFAULT_ID, &pt_comp_id->pv_stream_tag));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_comp_id);   
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_nfy_tag);
    
    RPC_DO_OP("x_svctx_select_stream");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_select_mpeg_stream_by_pid(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_strm_type,        /* in  */
                    MPEG_2_PID_T                    t_mpeg_2_pid,       /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,             /* in  */
                    VOID*                           pv_nfy_tag          /* in  */
                    )
{
    RPC_DECL(5, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);     
    RPC_ARG_INP(ARG_TYPE_INT32,    e_strm_type);
    RPC_ARG_INP(ARG_TYPE_UINT16,   t_mpeg_2_pid);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_nfy_tag);
    
    RPC_DO_OP("x_svctx_select_mpeg_stream_by_pid");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_select_mpeg_stream_by_pid_ex(
                    HANDLE_T                        h_svctx,            /* in  */
                    SVCTX_STREAM_INFO_T*            pt_stream_info,     /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,             /* in  */
                    VOID*                           pv_nfy_tag          /* in  */
                    )
{
    RPC_DECL(4, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_stream_info, RPC_DESC_SVCTX_STREAM_INFO_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_stream_info);   
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_nfy_tag);
    
    RPC_DO_OP("x_svctx_select_mpeg_stream_by_pid_ex");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_stop_stream(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_strm_type         /* in  */
                    )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_svctx);    
    RPC_ARG_INP(ARG_TYPE_INT32,  e_strm_type);    
    
    RPC_DO_OP("x_svctx_stop_stream");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_sync_stop_stream(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_strm_type,        /* in  */
                    UINT32                          ui4_time            /* in  */
                    )
{
    RPC_DECL(3, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_svctx);    
    RPC_ARG_INP(ARG_TYPE_INT32,  e_strm_type);  
    RPC_ARG_INP(ARG_TYPE_UINT32, ui4_time);     
    
    RPC_DO_OP("x_svctx_sync_stop_stream");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_select_audio_stream(
                    HANDLE_T                        h_svctx,            /* in  */
                    const ISO_639_LANG_T*           pt_lang,            /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,             /* in  */
                    VOID*                           pv_nfy_tag          /* in  */
                    )
{
    RPC_DECL(4, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_INP(ARG_TYPE_REF_STR,  pt_lang);   
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_nfy_tag);
    
    RPC_DO_OP("x_svctx_select_audio_stream");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_select_cc_stream(
                    HANDLE_T                        h_svctx,          /* in  */
                    HANDLE_T                        h_cc_gl_plane,    /* in  */
                    const SM_CC_FILTER_T*           pt_cc_filter,     /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    )
{
    RPC_DECL(5, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_cc_filter, RPC_DESC_SM_CC_FILTER_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_cc_gl_plane);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_cc_filter);   
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_nfy_tag);
    
    RPC_DO_OP("x_svctx_select_cc_stream");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

#ifdef MW_ISDB_SUPPORT
INT32 c_svctx_select_isdb_caption_stream(
                    HANDLE_T                        h_svctx,          /* in  */
                    HANDLE_T                        h_gl_plane,       /* in  */
                    UINT32                          ui4_caption_idx,  /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    )
{
    RPC_DECL(5, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_gl_plane);
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_caption_idx);   
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_nfy_tag);
    
    RPC_DO_OP("x_svctx_select_isdb_caption_stream");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_select_isdb_text_stream(
                    HANDLE_T                        h_svctx,          /* in  */
                    HANDLE_T                        h_gl_plane,       /* in  */
                    UINT32                          ui4_text_idx,     /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    )
{
    RPC_DECL(5, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_gl_plane);
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_text_idx);   
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_nfy_tag);
    
    RPC_DO_OP("x_svctx_select_isdb_text_stream");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}
#endif

INT32 c_svctx_select_subtitle_stream(
                    HANDLE_T                        h_svctx,          /* in  */
                    HANDLE_T                        h_gl_plane,       /* in  */
                    const ISO_639_LANG_T*           pt_lang,          /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    )
{
    RPC_DECL(5, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_gl_plane);
    RPC_ARG_INP(ARG_TYPE_REF_STR,  pt_lang);   
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_nfy_tag);
    
    RPC_DO_OP("x_svctx_select_subtitle_stream");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_select_subtitle_stream_by_comp_id(
                    HANDLE_T                        h_svctx,          /* in  */
                    HANDLE_T                        h_gl_plane,       /* in  */
                    const STREAM_COMP_ID_T*         pt_comp_id,       /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    )
{
    RPC_DECL(5, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_comp_id, RPC_DESC_STREAM_COMP_ID_T, NULL));
    RPC_CHECK(RPC_ADD_REF_VAL(RPC_DEFAULT_ID, &pt_comp_id->pv_stream_tag));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_gl_plane);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_comp_id);   
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_nfy_tag);
    
    RPC_DO_OP("x_svctx_select_subtitle_stream_by_comp_id");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_select_subtitle_stream_by_comp_id_ex(
                    HANDLE_T                        h_svctx,          /* in  */
                    HANDLE_T                        h_gl_plane,       /* in  */
                    const SM_SBTLH_STREAM_INFO*     pt_sbtl_info,     /* in  */
                    const STREAM_COMP_ID_T*         pt_comp_id,       /* in  */
                    x_svctx_select_nfy_fct          pf_nfy,           /* in  */
                    VOID*                           pv_nfy_tag        /* in  */
                    )
{
    RPC_DECL(6, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_comp_id, RPC_DESC_STREAM_COMP_ID_T, NULL));
    RPC_CHECK(RPC_ADD_REF_VAL(RPC_DEFAULT_ID, &pt_comp_id->pv_stream_tag));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_gl_plane);
    RPC_ARG_INP(ARG_TYPE_UINT32,   pt_sbtl_info);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_comp_id);   
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_nfy_tag);
    
    RPC_DO_OP("x_svctx_select_subtitle_stream_by_comp_id_ex");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_svc_rec_by_channel_id(
                    HANDLE_T                        h_svl,              /* in */
                    UINT32                          ui4_nw_mask,        /* in */
                    SURF_MODE_T                     e_surf_mode,        /* in */
                    UINT32*                         pui2_channel_id,    /* i/o*/
                    SVL_REC_T*                      pt_svl_rec          /* out*/
                    )
{
    RPC_DECL(5, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_svl_rec, RPC_DESC_SVL_REC_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_svl);    
    RPC_ARG_INP(ARG_TYPE_UINT32,     ui4_nw_mask);
    RPC_ARG_INP(ARG_TYPE_UINT32,     e_surf_mode);  
    RPC_ARG_IO (ARG_TYPE_REF_UINT32, pui2_channel_id);  
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,   pt_svl_rec);    
    
    RPC_DO_OP("x_svctx_get_svc_rec_by_channel_id");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_set_video_plane(
                    HANDLE_T                        h_svctx,            /* in */
                    SCC_VID_MODE_T                  e_mode              /* in */
                    )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_svctx);    
    RPC_ARG_INP(ARG_TYPE_UINT32, e_mode);    
    
    RPC_DO_OP("x_svctx_set_video_plane");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_set_session_attr(
                    HANDLE_T                    h_svctx,                /* in */
                    STREAM_TYPE_T               e_pipe_type,            /* in */
                    SM_SET_TYPE_T               e_set_type,             /* in */
                    VOID*                       pv_set_info,            /* in */
                    SIZE_T                      z_set_info_size         /* in */
                    )
{
    RPC_DECL(5, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_INP(ARG_TYPE_INT32,    e_pipe_type);    
    RPC_ARG_INP(ARG_TYPE_UINT32,   e_set_type);   
    RPC_ARG_INP (ARG_TYPE_REF_DESC,   pv_set_info); 
    RPC_ARG_INP(ARG_TYPE_SIZE_T,   z_set_info_size);   

    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_set_info, z_set_info_size));

    RPC_DO_OP("x_svctx_set_session_attr");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_session_attr(
                    HANDLE_T                    h_svctx,                /* in */
                    STREAM_TYPE_T               e_pipe_type,            /* in */
                    SM_GET_TYPE_T               e_get_type,             /* in */
                    VOID*                       pv_get_info,            /* in/out */
                    SIZE_T*                     pz_get_info_size        /* in/out */
                    )
{
    RPC_DECL(5, INT32);

    if(pz_get_info_size)
    {    
        RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_get_info, *pz_get_info_size));
    }
    else
    {
        RPC_FAIL;
    }
    
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_svctx);    
    RPC_ARG_INP(ARG_TYPE_INT32,      e_pipe_type);    
    RPC_ARG_INP(ARG_TYPE_UINT32,     e_get_type);   
    RPC_ARG_IO (ARG_TYPE_REF_VOID,   pv_get_info);   
    RPC_ARG_IO (ARG_TYPE_REF_SIZE_T, pz_get_info_size);   
    
    RPC_DO_OP("x_svctx_get_session_attr");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_set_stream_attr(
                    HANDLE_T                    h_svctx,                /* in */
                    STREAM_TYPE_T               e_strm_type,            /* in */
                    SM_SET_TYPE_T               e_set_type,             /* in */
                    VOID*                       pv_set_info,            /* in */
                    SIZE_T                      z_set_info_size         /* in */
                    )
{
    RPC_DECL(5, INT32);

    if (e_set_type == SM_CCH_SET_TYPE_CC_FLTR)
    {
        RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_set_info, RPC_DESC_SM_CC_FILTER_T, NULL));
    }
    else if (e_set_type == SM_VSH_SET_TYPE_VGA_TIMING_OPTION)
    {
        RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_set_info, RPC_DESC_VSH_VGA_TIMING_OPTION_T, NULL));
    }
    else if (e_set_type == SM_VSH_SET_TYPE_DISP_REGION)
    {
        RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_set_info, RPC_DESC_VSH_REGION_INFO_T, NULL));
    }
    else if (e_set_type == SM_VSH_SET_TYPE_DISP_REGION)
    {
        RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_set_info, RPC_DESC_VSH_REGION_INFO_T, NULL));
    }
    else if (e_set_type == SM_VSH_SET_TYPE_PLANE_ORDER)
    {
        RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_set_info, RPC_DESC_VSH_SET_PLANE_ORDER_INFO_T, NULL));
    }
    else if (e_set_type == SM_VSH_SET_TYPE_VIDEO_REGION)
    {
        RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_set_info, RPC_DESC_VSH_REGION_INFO_T, NULL));
    }
    else
    {
        RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_set_info, z_set_info_size));
    }

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_INP(ARG_TYPE_INT32,    e_strm_type);    
    RPC_ARG_INP(ARG_TYPE_UINT32,   e_set_type);   
    if (e_set_type == SM_VSH_SET_TYPE_VID_CLOCK ||
        e_set_type == SM_VSH_SET_TYPE_VID_PHASE ||
        e_set_type == SM_VSH_SET_TYPE_VID_H_POS ||
        e_set_type == SM_VSH_SET_TYPE_VID_V_POS ||
        e_set_type == SM_SBTLH_SET_TYPE_VISIBLE ||
        e_set_type == SM_SBTLH_SET_TYPE_INVISIBLE ||
        e_set_type == SM_SBTLH_SET_TYPE_SPEED)
    {
        RPC_ARG_INP(ARG_TYPE_UINT32,   pv_set_info);   
    }
    else
    {
        RPC_ARG_INP(ARG_TYPE_REF_DESC, pv_set_info);   
    }
    RPC_ARG_INP(ARG_TYPE_SIZE_T,   z_set_info_size);   
    
    RPC_DO_OP("x_svctx_set_stream_attr");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_stream_attr(
                    HANDLE_T                    h_svctx,                /* in */
                    STREAM_TYPE_T               e_strm_type,            /* in */
                    SM_GET_TYPE_T               e_get_type,             /* in */
                    VOID*                       pv_get_info,            /* in/out */
                    SIZE_T*                     pz_get_info_size        /* in/out */
                    )
{
    RPC_DECL(5, INT32);

    if(pz_get_info_size)
    {
        if (e_get_type == SM_VSH_GET_TYPE_VID_MIN_MAX)
        {
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_get_info, RPC_DESC_VSH_VID_MIN_MAX_INFO_T, NULL));
        }
        else if ( (e_get_type == SM_CCH_GET_TYPE_CC_FLTR) ||
                  (e_get_type == SM_CCH_GET_TYPE_SELECTED_CC_FLTR) )
        {
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_get_info, RPC_DESC_SM_CC_FILTER_T, NULL));
        }
        else if (e_get_type == SM_CCH_GET_TYPE_EXISTED_CC_IND)
        {
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_get_info, RPC_DESC_SM_CC_AUTO_SEL_T, NULL));
        }
        else if (e_get_type == SM_VSH_GET_TYPE_AMBIG_VGA_TIMING)
        {
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_get_info, RPC_DESC_VSH_AMBIG_VGA_TIMING_INFO_T, NULL));
        }
        else if ( (e_get_type == SM_VSH_GET_TYPE_DISP_REGION) ||
                  (e_get_type == SM_VSH_GET_TYPE_VIDEO_REGION) ||
                  (e_get_type == SM_VSH_GET_TYPE_FINAL_DISP_REGION) )
        {
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_get_info, RPC_DESC_VSH_REGION_INFO_T, NULL));
        }
        else if (e_get_type == SM_VSH_GET_TYPE_REGION_CAPABILITY)
        {
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_get_info, RPC_DESC_VSH_REGION_CAPABILITY_INFO_T, NULL));
        }
        else
        {
            RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_get_info, *pz_get_info_size));
        }
    }
    else
    {
        RPC_FAIL;
    }

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_svctx);    
    RPC_ARG_INP(ARG_TYPE_INT32,      e_strm_type);    
    RPC_ARG_INP(ARG_TYPE_UINT32,     e_get_type);   
    RPC_ARG_IO (ARG_TYPE_REF_DESC,   pv_get_info);   
    RPC_ARG_IO (ARG_TYPE_REF_SIZE_T, pz_get_info_size);   
    
    RPC_DO_OP("x_svctx_get_stream_attr");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_audio_info(
                    HANDLE_T                    h_svctx,                /* in */
                    ASH_AUDIO_INFO_T*           pt_info                 /* out */
                    )
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_info, RPC_DESC_ASH_AUDIO_INFO_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_info);    
    
    RPC_DO_OP("x_svctx_get_audio_info");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_video_src_resolution(
                    HANDLE_T                        h_svctx,            /* in */
                    VSH_SRC_RESOLUTION_INFO_T*      pt_resolution       /* out */
                    )
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_resolution, RPC_DESC_VSH_SRC_RESOLUTION_INFO_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_resolution);    
    
    RPC_DO_OP("x_svctx_get_video_src_resolution");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_unblock_svc(
                    HANDLE_T                        h_svctx,            /* in */
                    BOOL                            b_force_unblock     /* in */
                    )
{
    RPC_DECL(2, INT32);
 
    RPC_ARG_INP(ARG_TYPE_UINT32, h_svctx);    
    RPC_ARG_INP(ARG_TYPE_BOOL,   b_force_unblock);    
    
    RPC_DO_OP("x_svctx_unblock_svc");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}


INT32 _hndlr_svctx_svc_block_check_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }
    /*Maybe not needed*/
    pt_return->e_type = ARG_TYPE_INT32;
    
    pt_return->u.i4_arg = ((x_svctx_svc_block_check_fct)pv_cb_addr)(
            (SVC_BLOCK_CHECK_GUDIE_T)pt_args[0].u.ui4_arg,
            pt_args[1].u.ps_str,
            (SVL_REC_T *)pt_args[2].u.pv_desc,
            pt_args[3].u.pv_arg);

    return RPCR_OK;
}


INT32 c_svctx_set_default_parameters(
                    HANDLE_T                    h_svctx,                /* in */
                    UINT32                      ui4_stream_mode,        /* in */
                    SCC_VID_MODE_T              e_vid_mode,             /* in */
                    BOOL                        b_able_set_pmx_bg,      /* in */
                    x_svctx_svc_block_check_fct pf_block_check_fct,     /* in */
                    const CHAR*                 ps_src_grp_name,        /* in */
                    const CHAR*                 ps_snk_grp_name,        /* in */
                    const CHAR*                 ps_snk_grp_name_aux     /* in */
                    )
{
    RPC_DECL(8, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_stream_mode);
    RPC_ARG_INP(ARG_TYPE_UINT32,   e_vid_mode);
    RPC_ARG_INP(ARG_TYPE_BOOL,     b_able_set_pmx_bg);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_block_check_fct);
    RPC_ARG_INP(ARG_TYPE_REF_STR,  ps_src_grp_name);
    RPC_ARG_INP(ARG_TYPE_REF_STR,  ps_snk_grp_name);
    RPC_ARG_INP(ARG_TYPE_REF_STR,  ps_snk_grp_name_aux);
    
    RPC_DO_OP("x_svctx_set_default_parameters");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_set_default_strm_filter(
                    HANDLE_T                        h_svctx,            /* in */
                    STREAM_TYPE_T                   e_strm_type,        /* in */
                    x_svctx_strm_comp_filter_fct    pt_filter_fct,      /* in */
                    VOID*                           pv_filter_tag       /* in */
                    )
{
    RPC_DECL(4, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_INP(ARG_TYPE_INT32,    e_strm_type);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pt_filter_fct);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_filter_tag);
    
    RPC_DO_OP("x_svctx_set_default_strm_filter");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_set_screen_mode(
                    HANDLE_T                    h_svctx,                /* in */
                    SCC_VID_SCREEN_MODE_T       e_scr_mode              /* in */
                    )
{
    RPC_DECL(2, INT32);
 
    RPC_ARG_INP(ARG_TYPE_UINT32, h_svctx);    
    RPC_ARG_INP(ARG_TYPE_UINT32, e_scr_mode);    
    
    RPC_DO_OP("x_svctx_set_screen_mode");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_screen_mode(
                    HANDLE_T                    h_svctx,                /* in */
                    SCC_VID_SCREEN_MODE_T*      pe_scr_mode             /* out */
                    )
{
    RPC_DECL(2, INT32);
 
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_svctx);    
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_scr_mode);    
    
    RPC_DO_OP("x_svctx_get_screen_mode");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_signal_level(
                    HANDLE_T                    h_svctx,                /* in */
                    UINT8*                      pui1_signal_level       /* out */
                    )
{
    RPC_DECL(2, INT32);
 
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_svctx);    
    RPC_ARG_OUT(ARG_TYPE_REF_UINT8, pui1_signal_level);    
    
    RPC_DO_OP("x_svctx_get_signal_level");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_dbm_signal_level(
                    HANDLE_T                    h_svctx,                /* in */
                    INT16*                      pi2_dbm_signal_level    /* out */
                    )
{
    RPC_DECL(2, INT32);
 
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_svctx);    
    RPC_ARG_OUT(ARG_TYPE_REF_INT16, pi2_dbm_signal_level);    
    
    RPC_DO_OP("x_svctx_get_dbm_signal_level");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_tuner_status(
                    HANDLE_T                    h_svctx,
                    UINT8*                      pui1_tuner_status
                    )
{
    RPC_DECL(2, INT32);
 
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_svctx);
    RPC_ARG_OUT(ARG_TYPE_REF_INT8,  pui1_tuner_status);    
    
    RPC_DO_OP("x_svctx_get_tuner_status");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_display_resolution(
                    HANDLE_T                    h_svctx,                /* in */
                    SCC_DISP_RESOLUTION_T*      pt_resolution           /* out */
                    )
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_resolution, RPC_DESC_SCC_DISP_RESOLUTION_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_resolution);    
    
    RPC_DO_OP("x_svctx_get_display_resolution");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_set_display_resolution(
                    HANDLE_T                    h_svctx,                /* in */
                    SCC_DISP_RESOLUTION_T*      pt_resolution           /* in */
                    )
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_resolution, RPC_DESC_SCC_DISP_RESOLUTION_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_resolution);    
    
    RPC_DO_OP("x_svctx_set_display_resolution");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_is_signal_loss(
                    HANDLE_T                    h_svctx,                /* in */
                    BOOL*                       pb_signal_loss          /* out */
                    )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL, pb_signal_loss);    
    
    RPC_DO_OP("x_svctx_is_signal_loss");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_display_aspect_ratio(
                    HANDLE_T                    h_svctx,                /* in */
                    SCC_DISP_ASPECT_RATIO_T*    pe_aspect_ratio         /* out */
                    )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_svctx);    
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_aspect_ratio);    
    
    RPC_DO_OP("x_svctx_get_display_aspect_ratio");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_set_display_aspect_ratio(
                    HANDLE_T                    h_svctx,                /* in */
                    SCC_DISP_ASPECT_RATIO_T     e_aspect_ratio          /* in */
                    )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_svctx);    
    RPC_ARG_INP(ARG_TYPE_UINT32, e_aspect_ratio);    
    
    RPC_DO_OP("x_svctx_set_display_aspect_ratio");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_scdb_num_recs(
                    HANDLE_T                     h_svctx,               /* in  */
                    STREAM_TYPE_T                e_strm_type,           /* in  */
                    UINT16*                      pui2_num_recs          /* out  */
                    )
{
    RPC_DECL(3, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_svctx);    
    RPC_ARG_INP(ARG_TYPE_INT32,      e_strm_type);    
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16, pui2_num_recs);
    
    RPC_DO_OP("x_svctx_get_scdb_num_recs");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_using_strm_data(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_strm_type,        /* in  */
                    STREAM_COMP_ID_T*               pt_comp_id,         /* out */
                    SCDB_REC_T*                     pt_scdb_rec         /* out */
                    )
{
    RPC_DECL(4, INT32);
/*TODO : Fix */
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_comp_id, RPC_DESC_STREAM_COMP_ID_T, NULL));
    RPC_CHECK(RPC_ADD_REF_VAL(RPC_DEFAULT_ID, &pt_comp_id->pv_stream_tag));
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_scdb_rec, RPC_DESC_SCDB_REC_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_INP(ARG_TYPE_INT32,    e_strm_type);    
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_comp_id);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_scdb_rec);
    
    RPC_DO_OP("x_svctx_get_using_strm_data");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}


INT32 _hndlr_x_svctx_listener_nfy_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{  
    if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }
    /*Maybe not needed*/
    pt_return->e_type = ARG_TYPE_VOID;
    
    ((x_svctx_listener_nfy_fct)pv_cb_addr)(
            (HANDLE_T)pt_args[0].u.ui4_arg,
            (SVCTX_COND_T)pt_args[1].u.ui4_arg,
            (SVCTX_NTFY_CODE_T)pt_args[2].u.ui4_arg,
            (STREAM_TYPE_T)pt_args[3].u.i4_arg,
            pt_args[4].u.pv_arg);

    return RPCR_OK;
}


INT32 c_svctx_set_listener(
                    HANDLE_T                        h_svctx,            /* in  */
                    x_svctx_listener_nfy_fct        pf_listener_fct,    /* in  */
                    VOID*                           pv_nfy_tag          /* in  */
                    )
{
    RPC_DECL(3, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_listener_fct);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_nfy_tag);
    
    RPC_DO_OP("x_svctx_set_listener");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_surf_svc(
                    HANDLE_T                        h_svctx,            /* in  */
                    UINT32                          ui4_nw_chk_mask,    /* in  */
                    UINT32                          ui4_nw_chk_value,   /* in  */
                    SURF_MODE_T                     e_surf_mode,        /* in  */
                    SVL_REC_T*                      pt_svl_rec          /* out */
                    )
{
    RPC_DECL(5, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_svl_rec, RPC_DESC_SVL_REC_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_nw_chk_mask);    
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_nw_chk_value);
    RPC_ARG_INP(ARG_TYPE_UINT32,   e_surf_mode);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_svl_rec);
    
    RPC_DO_OP("x_svctx_surf_svc");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_grab_ownership(
                    HANDLE_T                        h_svctx             /* in  */
                    )
{
    RPC_DECL(1, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_svctx);
    
    RPC_DO_OP("x_svctx_grab_ownership");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_release_ownership(
                    HANDLE_T                        h_svctx             /* in  */
                    )
{
    RPC_DECL(1, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_svctx);
    
    RPC_DO_OP("x_svctx_release_ownership");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_cp_info(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_strm_type,        /* in  */
                    CP_INFO_LIST_T*                 pt_cp_info          /* out  */
                    )
{
    RPC_DECL(3, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_cp_info, RPC_DESC_CP_INFO_LIST_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_INP(ARG_TYPE_INT32,    e_strm_type);    
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_cp_info);
    
    RPC_DO_OP("x_svctx_get_cp_info");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_ca_info(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_strm_type,        /* in  */
                    SVCTX_CA_INFO_T*                pt_ca_info          /* out  */
                    )
{
    RPC_DECL(3, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_ca_info, RPC_DESC_SVCTX_CA_INFO_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_INP(ARG_TYPE_INT32,    e_strm_type);    
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_ca_info);
    
    RPC_DO_OP("x_svctx_get_ca_info");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_set(
                    HANDLE_T                        h_svctx,            /* in  */
                    SVCTX_SET_TYPE_T                e_set_type,         /* in  */
                    VOID*                           pv_set_info,        /* in  */
                    SIZE_T                          z_set_info_size     /* in  */
                    )
{
    RPC_DECL(4, INT32);

    if (e_set_type == SVCTX_COMMON_SET_ATOMIC_CMD_GRP)
    {
        SVCTX_ATOMIC_CMD_T* pt_cmd = (SVCTX_ATOMIC_CMD_T*)pv_set_info;
        INT32 i4_idx = 0;
        for (i4_idx = 0; pt_cmd[i4_idx].t_type != SVCTX_COMMON_SET_ATOMIC_CMD_END; i4_idx++)
        {
        }
        RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 2, ARG_TYPE_REF_DESC));
        RPC_CHECK(RPC_ADD_REF_DESC_ARR(RPC_DEFAULT_ID, i4_idx+1, pv_set_info, RPC_DESC_SVCTX_ATOMIC_CMD_T, NULL));
        
        for (i4_idx = 0; pt_cmd[i4_idx].t_type != SVCTX_COMMON_SET_ATOMIC_CMD_END; i4_idx++)
        {
            switch(pt_cmd[i4_idx].t_type)
            {
                case SVCTX_COMMON_SET_ATOMIC_DISP_RGN: {
                    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_cmd[i4_idx].pv_data, RPC_DESC_VSH_REGION_INFO_T, NULL));
                    break;
                }
                case SVCTX_COMMON_SET_ATOMIC_SCRN_MODE: {
                    RPC_CHECK(RPC_ADD_REF_VAL(RPC_DEFAULT_ID, pt_cmd[i4_idx].pv_data));
                    break;
                }
                case SVCTX_COMMON_SET_ATOMIC_OVERSCAN: {
                    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_cmd[i4_idx].pv_data, RPC_DESC_SCC_VID_OVER_SCAN_CLIPPER_T, NULL));
                    break;
                }
                default:
                    break;
            }
        }
    }
    else if (e_set_type == SVCTX_COMMON_SET_VIDEO_MUTE)
    {
        RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 2, ARG_TYPE_REF_DESC));
        RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_set_info, RPC_DESC_SVCTX_VIDEO_MUTE_INFO_T, NULL));
    }
#ifdef MW_TV_AV_BYPASS_SUPPORT
    else if (e_set_type == SVCTX_COMMON_SET_COPY_PROTECTION)
    {
        RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 2, ARG_TYPE_REF_DESC));
        RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_set_info, RPC_DESC_SVCTX_COPY_PROTECTION_T, NULL));
    }
#endif
    else if (e_set_type == SVCTX_COMMON_SET_DEFALUT_BLOCK_CHECK_FCT)
    {
        RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 2, ARG_TYPE_REF_FUNC));
    }
    else if (e_set_type == SVCTX_COMMON_SET_SCART_OUT)
    {
        RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 2, ARG_TYPE_REF_DESC));
        RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_set_info, RPC_DESC_SCART_OUT_INFO_T, NULL));
    }
    else if (e_set_type == SVCTX_MM_SET_TYPE_TRICK)
    {
        RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 2, ARG_TYPE_REF_DESC));
        RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_set_info, RPC_DESC_MM_SVC_TRICK_INFO_T, NULL));
    }
    else if (e_set_type == SVCTX_MM_SET_TYPE_REPEAT)
    {
        RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 2, ARG_TYPE_REF_DESC));
        RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_set_info, RPC_DESC_MM_SVC_REPEAT_INFO_T, NULL));
    }
    else
    {
        RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 2, ARG_TYPE_REF_DESC));
        RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_set_info, z_set_info_size));
    }
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_INP(ARG_TYPE_UINT32,   e_set_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE, pv_set_info);
    RPC_ARG_INP(ARG_TYPE_SIZE_T,   z_set_info_size);
    
    RPC_DO_OP("x_svctx_set");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get(
                    HANDLE_T                        h_svctx,            /* in  */
                    SVCTX_GET_TYPE_T                e_get_type,         /* in  */
                    VOID*                           pv_get_info,        /* in/out */
                    SIZE_T*                         pz_get_info_size    /* in/out */
                    )
{
    RPC_DECL(4, INT32);

    if(pz_get_info_size)
    {
        if (e_get_type == SVCTX_COMMON_GET_VIDEO_MUTE)
        {
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_get_info, RPC_DESC_SVCTX_VIDEO_MUTE_INFO_T, NULL));
        }
        else if (e_get_type == SVCTX_MM_GET_TYPE_TIME_CODE || e_get_type == SVCTX_MM_GET_TYPE_TIME_CODE_MS)
        {
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_get_info, RPC_DESC_MM_SVC_TIME_CODE_INFO_T, NULL));
        }
        else if (e_get_type == SVCTX_MM_GET_TYPE_DUR)
        {
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_get_info, RPC_DESC_MM_SVC_DUR_INFO_T, NULL));
        }
        else if (e_get_type == SVCTX_MM_GET_TYPE_POSIT_INFO)
        {
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_get_info, RPC_DESC_MM_SVC_POS_INFO_T, NULL));
        }
        else if (e_get_type == SVCTX_MM_GET_TYPE_VIDEO_INFO)
        {
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_get_info, RPC_DESC_MM_VIDEO_INFO_T, NULL));
        }
        else if (e_get_type == SVCTX_MM_GET_TYPE_META_DATA)
        {
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_get_info, RPC_DESC_MINFO_INFO_T, NULL));
        }
        else if (e_get_type == SVCTX_MM_GET_TYPE_AFIFO_AMOUNT)
        {
            RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_get_info, *pz_get_info_size));
        }
        else
        {
            RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_get_info, *pz_get_info_size));
        }
    }
    else
    {
        RPC_FAIL;
    }

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_svctx);    
    RPC_ARG_INP(ARG_TYPE_UINT32,     e_get_type);
    RPC_ARG_IO (ARG_TYPE_REF_DESC,   pv_get_info);
    RPC_ARG_IO (ARG_TYPE_REF_SIZE_T, pz_get_info_size);
    
    RPC_DO_OP("x_svctx_get");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_freeze(
                    HANDLE_T                    h_svctx,                /* in  */
                    UINT32                      ui4_freeze_strm_mask    /* in  */
                    )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_svctx);    
    RPC_ARG_INP(ARG_TYPE_UINT32, ui4_freeze_strm_mask);
    
    RPC_DO_OP("x_svctx_freeze");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_unfreeze(
                    HANDLE_T                        h_svctx             /* in  */
                    )
{
    RPC_DECL(1, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_svctx);    
    
    RPC_DO_OP("x_svctx_unfreeze");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_is_frozen(
                    HANDLE_T                        h_svctx,            /* in  */
                    BOOL*                           pb_result           /* out */
                    )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL, pb_result);  
    
    RPC_DO_OP("x_svctx_is_frozen");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_current_event_info_len (
                    HANDLE_T                        h_svctx,            /* in  */
                    EVCTX_KEY_TYPE_T                e_key_type,         /* in  */
                    VOID*                           pv_key_info,        /* in  */
                    SIZE_T*                         pz_event_info_len   /* out */
                    )
{
    RPC_DECL(4, INT32);
    
    ri_event_key_add_ref_desc(e_key_type,  pv_key_info, 2);
    
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_svctx);    
    RPC_ARG_INP(ARG_TYPE_UINT32,     e_key_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE,   pv_key_info);
    RPC_ARG_OUT(ARG_TYPE_REF_SIZE_T, pz_event_info_len);  
    
    RPC_DO_OP("x_svctx_get_current_event_info_len");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_current_event_info (
                    HANDLE_T                        h_svctx,            /* in  */
                    EVCTX_KEY_TYPE_T                e_key_type,         /* in  */
                    VOID*                           pv_key_info,        /* in  */
                    SIZE_T*                         pz_event_info_len,  /* in/out */
                    VOID*                           pv_event_info       /* out */
                    )
{
    RPC_DECL(5, INT32);
    
    ri_event_key_add_ref_desc(e_key_type,  pv_key_info, 2);
        
    if(pz_event_info_len)
    {
        if(e_key_type == EVCTX_KEY_TYPE_RATING)
        {
            /*
             * Add this desc as size of pv_envent_info and we need add description for the type of 
             * EVCTX_RATING_LIST_T. Because the server will fill some new meta data into this buffer. 
             * RPC will do the address fixing if the address in the buffer range.
             * But currently new meta list data cannot linked automaticly. FIxme.
             */
            
            memset(pv_event_info, 0, *pz_event_info_len);
            
            RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_event_info, *pz_event_info_len));
        }
        else if(e_key_type == EVCTX_KEY_TYPE_EVENT_LINK)
        {
            memset(pv_event_info, 0, *pz_event_info_len);
            RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_event_info, *pz_event_info_len));
        }
        else
        {
            RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_event_info, *pz_event_info_len));
        }
    }
    else
    {
        RPC_FAIL;
    }

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_svctx);    
    RPC_ARG_INP(ARG_TYPE_UINT32,     e_key_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE,   pv_key_info);
    RPC_ARG_IO (ARG_TYPE_REF_SIZE_T, pz_event_info_len);  
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,   pv_event_info);
    
    RPC_DO_OP("x_svctx_get_current_event_info");


    /* support for EVCTX_RATING_LIST_T */
    if(e_key_type == EVCTX_KEY_TYPE_RATING)
    {
    	SIZE_T 	z_event_info_len = 0;
		SIZE_T 	z_temp = 0;
		UINT8* 	pui1_data;
		EVCTX_RATING_LIST_T* pt_list;

		pt_list = (EVCTX_RATING_LIST_T *)pv_event_info;

		pui1_data = (UINT8*)pv_event_info;
		
		do
		{		
			z_temp = 0;
			
			pt_list->ui2_rating_region = (UINT16)*(pui1_data + z_event_info_len);
			pt_list->ui2_rated_dimensions = (UINT16)*(pui1_data + 2 + z_event_info_len);

			/*Rebuild the pt_ratings*/
			if(pt_list->ui2_rated_dimensions != 0)
			{
				pt_list->pt_ratings = (EVCTX_RATING_T*)(pui1_data + 16 + z_event_info_len);
			}
			else
			{
				pt_list->pt_ratings = NULL;
			}

			/*Rebuild the ps_rating_descrption */
			if(pt_list->ps_rating_descrption != NULL)
			{
				pt_list->ps_rating_descrption = (CHAR*)(pui1_data + 16 + z_event_info_len +	pt_list->ui2_rated_dimensions*4);
				z_temp = strlen(pt_list->ps_rating_descrption)*sizeof(CHAR) + 1;
				z_temp = ((z_temp + 3) / 4) * 4;
			}		

			/*Rebuild the pt_next*/
			z_event_info_len = z_event_info_len + 16 + pt_list->ui2_rated_dimensions * 4 + z_temp;

			if((SIZE_T)*pz_event_info_len > z_event_info_len + sizeof(EVCTX_RATING_LIST_T))
			{
				pt_list->pt_next = (EVCTX_RATING_LIST_T*)(pui1_data + z_event_info_len);	
				pt_list = pt_list->pt_next;
			}
			else
			{
				pt_list->pt_next = NULL;
			}
		}while((SIZE_T)*pz_event_info_len > z_event_info_len);
    }
    else if(e_key_type == EVCTX_KEY_TYPE_EVENT_LINK)
    {

        EVCTX_EVENT_LINKAGE_LIST_T*         pt_list            = NULL;
        UINT32                              ui4_target_address = 0;
        pt_list = (EVCTX_EVENT_LINKAGE_LIST_T *)pv_event_info;
        ui4_target_address  = (UINT32) ((VOID*)pt_list);
        ui4_target_address += sizeof(EVCTX_EVENT_LINKAGE_LIST_T);
        pt_list->pt_event_linkage = (EVCTX_EVENT_LINKAGE_T*)((VOID*)ui4_target_address);
    }

    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_mapped_dev_info(
                    DRV_TYPE_T                  e_drv_type,                 /* in */
                    DEVICE_TYPE_T*              pe_video_dev_type,          /* in/out */
                    DEVICE_TYPE_T*              pe_best_video_dev_type,     /* out */
                    INT8*                       pi1_best_video_dev_order,   /* in/out */
                    DEVICE_TYPE_T*              pe_audio_dev_type,          /* in/out */
                    DEVICE_TYPE_T*              pe_best_audio_dev_type,     /* out */
                    INT8*                       pi1_best_audio_dev_order    /* in/out */
                    )
{
    RPC_DECL(7, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT16,     e_drv_type);
    RPC_ARG_IO (ARG_TYPE_REF_UINT32, pe_video_dev_type);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_best_video_dev_type);
    RPC_ARG_IO (ARG_TYPE_REF_INT8,   pi1_best_video_dev_order);
    RPC_ARG_IO (ARG_TYPE_REF_UINT32, pe_audio_dev_type);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pe_best_audio_dev_type);
    RPC_ARG_IO (ARG_TYPE_REF_INT8,   pi1_best_audio_dev_order);
    RPC_DO_OP("x_svctx_get_mapped_dev_info");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_device_profile(
                    DEVICE_TYPE_T               e_dev_type,             /* in  */
                    DEVICE_TYPE_PROFILE_T*      pt_profile              /* out  */
                    )
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_profile, RPC_DESC_DEVICE_TYPE_PROFILE_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   e_dev_type);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_profile);
    
    RPC_DO_OP("x_svctx_get_device_profile");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_device_profile_by_drv_type(
                    DRV_TYPE_T                  e_drv_type,             /* in  */
                    DEVICE_TYPE_PROFILE_T*      pt_profile              /* out  */
                    )
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_profile, RPC_DESC_DEVICE_TYPE_PROFILE_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT16,   e_drv_type);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_profile);
    
    RPC_DO_OP("x_svctx_get_device_profile_by_drv_type");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_well_defined_video_heigh(
                    UINT32                          ui4_raw_height,     /* in  */
                    UINT32*                         pui4_defined_height /* out  */
                    )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,     ui4_raw_height);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pui4_defined_height);
    
    RPC_DO_OP("x_svctx_get_well_defined_video_heigh");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_cc_capability(
                    HANDLE_T                    h_svctx,                /* in  */
                    SVCTX_CAPABILITY_T*         pt_capablility          /* out  */
                    )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_svctx);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pt_capablility);
    
    RPC_DO_OP("x_svctx_get_cc_capability");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_subtitle_capability(
                    HANDLE_T                    h_svctx,                /* in  */
                    SVCTX_CAPABILITY_T*         pt_capablility          /* out  */
                    )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_svctx);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pt_capablility);
    
    RPC_DO_OP("x_svctx_get_subtitle_capability");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_teletext_capability(
                    HANDLE_T                    h_svctx,                /* in  */
                    SVCTX_CAPABILITY_T*         pt_capablility          /* out  */
                    )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_svctx);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pt_capablility);
    
    RPC_DO_OP("x_svctx_get_teletext_capability");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_event_capability(
                    HANDLE_T                    h_svctx,                /* in  */
                    UINT32                      ui4_event_key_mask,     /* in  */
                    SVCTX_CAPABILITY_T*         pt_capablility          /* out  */
                    )
{
    RPC_DECL(3, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_svctx);
    RPC_ARG_INP(ARG_TYPE_UINT32,     ui4_event_key_mask);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pt_capablility);
    
    RPC_DO_OP("x_svctx_get_event_capability");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}


INT32 _hndlr_x_scdb_nfy_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{  
    if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }
    /*Maybe not needed*/
    pt_return->e_type = ARG_TYPE_VOID;
    
    ((x_scdb_nfy_fct)pv_cb_addr)(
            (HANDLE_T)pt_args[0].u.ui4_arg,
            (SCDB_COND_T)pt_args[1].u.ui4_arg,
            pt_args[2].u.ui4_arg,
            pt_args[3].u.pv_arg,
            pt_args[4].u.ui4_arg);

    return RPCR_OK;
}


INT32 c_svctx_open_scdb(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_pipe_type,        /* in  */
                    HANDLE_T*                       ph_scdb,            /* out  */
                    VOID*                           pv_tag,             /* in  */
                    x_scdb_nfy_fct                  pf_nfy              /* in  */
                    )
{
    RPC_DECL(5, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_svctx);
    RPC_ARG_INP(ARG_TYPE_INT32,      e_pipe_type);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_scdb);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_tag);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_nfy);
    
    RPC_DO_OP("x_svctx_open_scdb");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_close_scdb(
                    HANDLE_T                        h_scdb              /* in  */
                    )
{
    RPC_DECL(1, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_scdb);
    
    RPC_DO_OP("x_svctx_close_scdb");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}


INT32 _hndlr_x_stream_nfy_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{  
    if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }
    /*Maybe not needed*/
    pt_return->e_type   = ARG_TYPE_INT32;
    
    pt_return->u.i4_arg = ((x_stream_nfy_fct)pv_cb_addr)(
            (HANDLE_T)pt_args[0].u.ui4_arg,
            (SM_COND_T)pt_args[1].u.ui4_arg,
            pt_args[2].u.pv_arg,
            (SM_EVN_TYPE_T)pt_args[3].u.ui4_arg,
            pt_args[4].u.ui4_arg);

    return RPCR_OK;
}


INT32 c_svctx_open_stream(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_pipe_type,        /* in  */
                    const STREAM_COMP_ID_T*         pt_strm_comp_id,    /* in  */
                    const SM_COMMAND_T*             pt_cmd_sets,        /* in  */
                    x_stream_nfy_fct                pf_strm_nfy,        /* in  */
                    VOID*                           pv_strm_nfy_tag,    /* in  */
                    BOOL                            b_auto_play,        /* in  */
                    HANDLE_T*                       ph_stream           /* out  */
                    )
{
    RPC_DECL(8, INT32);
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_strm_comp_id, RPC_DESC_STREAM_COMP_ID_T, NULL));

    RPC_CHECK(RPC_ADD_REF_VAL(RPC_DEFAULT_ID, &pt_strm_comp_id->pv_stream_tag));

    /*Fix this*/
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_cmd_sets, RPC_DESC_SM_COMMAND_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_svctx);
    RPC_ARG_INP(ARG_TYPE_INT32,      e_pipe_type);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,   pt_strm_comp_id);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,   pt_cmd_sets);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_strm_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_strm_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_BOOL,       b_auto_play);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_stream);
    
    RPC_DO_OP("x_svctx_open_stream");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_open_stream_ex(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_pipe_type,        /* in  */
                    const STREAM_COMP_ID_T*         pt_strm_comp_id,    /* in  */
                    const SM_COMMAND_T*             pt_cmd_sets,        /* in  */
                    x_stream_nfy_fct                pf_strm_nfy,        /* in  */
                    VOID*                           pv_strm_nfy_tag,    /* in  */
                    BOOL                            b_auto_play,        /* in  */
                    UINT32                          ui4_serial_num,     /* in  */
                    HANDLE_T*                       ph_stream           /* out  */
                    )
{
    RPC_DECL(9, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_strm_comp_id, RPC_DESC_STREAM_COMP_ID_T, NULL));
    RPC_CHECK(RPC_ADD_REF_VAL(RPC_DEFAULT_ID, &pt_strm_comp_id->pv_stream_tag));
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_cmd_sets, RPC_DESC_SM_COMMAND_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_svctx);
    RPC_ARG_INP(ARG_TYPE_INT32,      e_pipe_type);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,   pt_strm_comp_id);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,   pt_cmd_sets);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_strm_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_strm_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_BOOL,       b_auto_play);
    RPC_ARG_INP(ARG_TYPE_UINT32,     ui4_serial_num);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_stream);
    
    RPC_DO_OP("x_svctx_open_stream_ex");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);    
}   

INT32 c_svctx_close_stream(
                    HANDLE_T                        h_stream            /* in  */
                    )
{
    RPC_DECL(1, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_stream);
    
    RPC_DO_OP("x_svctx_close_stream");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_conn_attr(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_pipe_type,        /* in  */
                    CM_CTRL_TYPE_T                  e_ctrl,             /* in  */
                    VOID*                           pv_ctrl_data        /* out  */
                    )
{
    RPC_DECL(4, INT32);

    switch (e_ctrl)
    {
        case BRDCST_CTRL_GET_BER:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 3, ARG_TYPE_REF_INT32 | ARG_DIR_OUT));
            break;
            
        default:
            RI_ERR("[ RPC INTERFACE ] Unsupported e_ctrl in wrapper \n");
            break;
    }
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);
    RPC_ARG_INP(ARG_TYPE_INT32,    e_pipe_type);
    RPC_ARG_INP(ARG_TYPE_UINT32,   e_ctrl);
    RPC_ARG_OUT(ARG_TYPE_VARIABLE, pv_ctrl_data);
    
    RPC_DO_OP("x_svctx_get_conn_attr");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_set_conn_attr(
                    HANDLE_T                        h_svctx,            /* in  */
                    STREAM_TYPE_T                   e_pipe_type,        /* in  */
                    CM_CTRL_TYPE_T                  e_ctrl,             /* in  */
                    VOID*                           pv_ctrl_data        /* in/out  */
                    )
{
    RPC_DECL(4, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);
    RPC_ARG_INP(ARG_TYPE_INT32,    e_pipe_type);
    RPC_ARG_INP(ARG_TYPE_UINT32,   e_ctrl);
    RPC_ARG_IO (ARG_TYPE_VARIABLE, pv_ctrl_data);
    {
        switch(e_ctrl)
        {
            case ANALOG_BRDCST_CTRL_SET_AUTO_SEARCH_MODE:
                RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 3, ARG_TYPE_REF_INT32));
                break;
            case ANALOG_BRDCST_CTRL_SET_TUNE_ATTRIBUTES:
                RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 3, ARG_TYPE_REF_DESC));
                RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                         pv_ctrl_data,
                         RPC_DESC_ANALOG_BRDCST_CTRL_TUNE_ATTRIBUTES_T,
                         NULL));
                break;
            default:
                break;
        }
    }
    
    RPC_DO_OP("x_svctx_set_conn_attr");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_set_analog_tuner_out_attr(
                    TUNER_OUT_DESC_T*               pt_tuner_out        /* in  */
                    )
{
    RPC_DECL(1, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_tuner_out, RPC_DESC_TUNER_OUT_DESC_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_tuner_out);
    
    RPC_DO_OP("x_svctx_set_analog_tuner_out_attr");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

SVC_BLOCK_COND_T c_svctx_svc_block_check(
                    HANDLE_T                        h_svctx,            /* in  */
                    SVC_BLOCK_CHECK_GUDIE_T         e_check_guide,      /* in  */
                    const CHAR*                     ps_src_name,        /* in  */
                    const SVL_REC_T*                pt_svl_rec,         /* in  */
                    VOID*                           pv_evn_rating       /* in  */
                    )
{
    RPC_DECL(5, INT32);

    ri_rrctx_rating_list_add_desc(pv_evn_rating);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_svl_rec, RPC_DESC_SVL_REC_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);
    RPC_ARG_INP(ARG_TYPE_UINT32,   e_check_guide);
    RPC_ARG_INP(ARG_TYPE_REF_STR,  ps_src_name);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_svl_rec);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pv_evn_rating);
    
    RPC_DO_OP("x_svctx_svc_block_check");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_set_ad_state(
                    HANDLE_T                        h_svctx,            /* in  */
                    BOOL                            b_enable            /* in  */
                    )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_svctx);    
    RPC_ARG_INP(ARG_TYPE_BOOL,   b_enable);
    
    RPC_DO_OP("x_svctx_set_ad_state");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_get_ad_state(
                    HANDLE_T                        h_svctx,            /* in  */
                    BOOL*                           pb_enable           /* out */
                    )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);    
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL, pb_enable);
    
    RPC_DO_OP("x_svctx_get_ad_state");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

#ifdef MW_TV_AV_BYPASS_SUPPORT
static INT32 _svctx_create_associate(
    const CHAR*                     ps_name,                            /* in  */
    const SVCTX_ASSC_CFG_T*         pt_assc_cfg,                        /* in  */
    HANDLE_T*                       ph_svctx                            /* out  */
    )
{
    RPC_DECL(3, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_assc_cfg, RPC_DESC_SVCTX_ASSC_CFG_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_REF_STR,    ps_name);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,   pt_assc_cfg);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_svctx);
    
    RPC_DO_OP("x_svctx_create_associate");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_create_associate(
                    const CHAR*                     ps_name,            /* in  */
                    const SVCTX_ASSC_CFG_T*         pt_assc_cfg,        /* in  */
                    HANDLE_T*                       ph_svctx)           /* out  */
{
    INT32     i4_ret;

    i4_ret = _svctx_create_associate(ps_name, pt_assc_cfg, ph_svctx);
    return i4_ret;
}

#ifdef MW_PIP_POP_SUPPORT
INT32 c_svctx_add_associate(
                    HANDLE_T            h_svctx,                        /* in  */
                    HANDLE_T            h_svctx_assc                    /* in  */
                    )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_svctx);    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_svctx_assc);  
    
    RPC_DO_OP("x_svctx_add_associate");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_add_exclusion(
                    HANDLE_T            h_svctx,                        /* in  */
                    HANDLE_T            h_svctx_excl,                   /* in  */
                    UINT32              ui4_excl_src_mask               /* in  */
                    )
{
    RPC_DECL(3, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_svctx);    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_svctx_excl);  
    RPC_ARG_INP(ARG_TYPE_UINT32, ui4_excl_src_mask); 
    
    RPC_DO_OP("x_svctx_add_exclusion");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_start_exclusion(
    HANDLE_T        h_svctx,
    SRC_TYPE_T      e_src_type)
{
    RPC_DECL(2, INT32);
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_svctx);
    RPC_ARG_INP(ARG_TYPE_UINT8, e_src_type);
    
    RPC_DO_OP("x_svctx_start_exclusion");

    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_stop_exclusion(
    HANDLE_T        h_svctx,
    SRC_TYPE_T      e_src_type)
{
    RPC_DECL(2, INT32);
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_svctx);
    RPC_ARG_INP(ARG_TYPE_UINT8, e_src_type);
    
    RPC_DO_OP("x_svctx_stop_exclusion");

    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}
#endif
#endif

#ifdef SYS_RECORD_SUPPORT
static INT32 _svctx_rec_create (
    const CHAR*                     ps_svctx_name,    
    const SVCTX_REC_ATTR_T*         pt_rec_attr,      
    HANDLE_T*                       ph_svctx_rec)
{
    RPC_DECL(3, INT32);
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_rec_attr, RPC_DESC_SVCTX_REC_ATTR_T, NULL));
    RPC_ARG_INP(ARG_TYPE_REF_STR,       ps_svctx_name);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,      pt_rec_attr);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,    ph_svctx_rec);
    RPC_DO_OP("x_svctx_rec_create");
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_rec_create (
     const CHAR*                     ps_svctx_name,    
     const SVCTX_REC_ATTR_T*         pt_rec_attr,      
     HANDLE_T*                       ph_svctx_rec)
{
    return _svctx_rec_create(ps_svctx_name, pt_rec_attr, ph_svctx_rec);
}

INT32 c_svctx_rec_start (
    HANDLE_T                        h_svctx_rec, 
    const SVCTX_REC_CONFIG_T*       pt_config)
{
    RPC_DECL(2, INT32);
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_config, RPC_DESC_SVCTX_REC_CONFIG_T, NULL));
    RPC_ARG_INP(ARG_TYPE_REF_UINT32,    h_svctx_rec);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,      pt_config);
    RPC_DO_OP("x_svctx_rec_start");
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_rec_stop (
    HANDLE_T                        h_svctx_rec)
{
    RPC_DECL(1, INT32);
    RPC_ARG_INP(ARG_TYPE_REF_UINT32, h_svctx_rec);
    RPC_DO_OP("x_svctx_rec_stop");
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

INT32 c_svctx_rec_pause (
    HANDLE_T                        h_svctx_rec)
{
    RPC_DECL(1, INT32);
    RPC_ARG_INP(ARG_TYPE_REF_UINT32, h_svctx_rec);
    RPC_DO_OP("x_svctx_rec_pause");
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}
 
INT32 c_svctx_rec_resume (
    HANDLE_T                        h_svctx_rec)
{
    RPC_DECL(1, INT32);
    RPC_ARG_INP(ARG_TYPE_REF_UINT32, h_svctx_rec);
    RPC_DO_OP("x_svctx_rec_resume");
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}
#endif

INT32 c_rpc_reg_svctx_cb_hndlrs()
{
    RPC_REG_CB_HNDLR(x_svctx_strm_comp_filter_fct);
    RPC_REG_CB_HNDLR(x_svctx_select_nfy_fct);
    RPC_REG_CB_HNDLR(x_svctx_listener_nfy_fct);
    RPC_REG_CB_HNDLR(x_scdb_nfy_fct);
    RPC_REG_CB_HNDLR(x_stream_nfy_fct);
    RPC_REG_CB_HNDLR(svctx_svc_block_check_fct);
    return RPCR_OK;
}


