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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>

#include "conn_mngr/c_cm.h"
#include "conn_mngr/x_cm.h"
#include "conn_mngr/cm_playback/u_playback_handler.h"

#include "svctx/x_svctx.h"
#include "os/inc/x_os.h"

#include "handle/handle.h"

#include "rpc.h"
#include "ri_common.h"
#include "ri_desc.h"


static int mem_fd = 0;
static int rmmgr_fd = 0;


#define RET_ON_FAIL(stmt) \
do {\
    INT32 __i4_ret;\
    __i4_ret = (stmt);\
    if(__i4_ret != RPCR_OK)\
    {   RPC_ERROR("RPC Failure %s, #%d\n", __FUNCTION__, __LINE__);\
        return __i4_ret;\
    }\
}while(0)

static INT32 _set_mm_svc_cmd_array( MM_SVC_COMMAND_T *  pt_cmd_sets)
{
    UINT32 ui4_i;
    MM_SVC_COMMAND_T*   pt_iter;
/*    MM_SVC_CMD_CODE_T   e_cmd_type;*/
    
  
    if(pt_cmd_sets == NULL)
    {
        return RPCR_OK;
    }
    
    pt_iter = pt_cmd_sets;
    
    for (ui4_i = 0; pt_cmd_sets[ui4_i].e_code != MM_SVC_CMD_CODE_END; ui4_i ++)
    {        
        switch(pt_cmd_sets[ui4_i].e_code)
        {
            case MM_SVC_CMD_CODE_SVL_NAME:
            case MM_SVC_CMD_CODE_MEDIA_PATH:
            case MM_SVC_CMD_CODE_SET_PROXY_URL:
            case MM_SVC_CMD_CODE_SET_AGENT_URL:
            case MM_SVC_CMD_CODE_SET_COOKIE_URL:
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 &pt_cmd_sets[ui4_i].u, 
                                 RPC_DESC_MM_SVC_COMMAND_T_u, 
                                 "u.ps_text"));
                RET_ON_FAIL(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_cmd_sets[ui4_i].u.ps_text));
            break;
            case MM_SVC_CMD_CODE_SVL_ID:
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 &pt_cmd_sets[ui4_i].u, 
                                 RPC_DESC_MM_SVC_COMMAND_T_u, 
                                 NULL));
                break;
            case MM_SVC_CMD_CODE_SRC_TYPE:
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 &pt_cmd_sets[ui4_i].u, 
                                 RPC_DESC_MM_SVC_COMMAND_T_u, 
                                 NULL));
                break;
            case MM_SVC_CMD_CODE_SET_SRC_SUB_TYPE:
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 &pt_cmd_sets[ui4_i].u, 
                                 RPC_DESC_MM_SVC_COMMAND_T_u, 
                                 NULL));
                break;
            case MM_SVC_CMD_CODE_SET_BOOT_ANIM:
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 &pt_cmd_sets[ui4_i].u, 
                                 RPC_DESC_MM_SVC_COMMAND_T_u, 
                                 NULL));
                break;
            case MM_SVC_CMD_CODE_SET_STRM_INFO:
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 &pt_cmd_sets[ui4_i].u, 
                                 RPC_DESC_MM_SVC_COMMAND_T_u, 
                                 "u.pv_data"));  
                /* Fix me: TO Be added ............ .............*/
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 pt_cmd_sets[ui4_i].u.pv_data, 
                                 RPC_DESC_MM_STRM_INFO_T, NULL));  
                
                if(((MM_STRM_INFO_T*)pt_cmd_sets[ui4_i].u.pv_data)->t_format.e_media_type == MEDIA_TYPE_CONTAINER &&
                   ((MM_STRM_INFO_T*)pt_cmd_sets[ui4_i].u.pv_data)->t_format.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_CMPB)
                {
                    UINT8 ui1 = 0;
                    MINFO_TYPE_STM_ATRBT_T* pt_strm_atrbt = NULL;
                    for (ui1=0; ui1<SUPPORTED_STRM_NUM_MAX; ui1++)
                    {
                        if(((MM_STRM_INFO_T*)pt_cmd_sets[ui4_i].u.pv_data)->at_stm_atrbt[ui1].e_type == ST_VIDEO)
                        {
                            pt_strm_atrbt = &(((MM_STRM_INFO_T*)pt_cmd_sets[ui4_i].u.pv_data)->at_stm_atrbt[ui1].t_stm_atrbt);
                            break;
                        }
                    }
                    if(pt_strm_atrbt)
                    {
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 &(pt_strm_atrbt->u.t_cmpb_video_stm_attr), 
                                 RPC_DESC_MINFO_CMPB_VIDEO_STM_ATRBT_T, NULL)); 
                        
                        RET_ON_FAIL(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID,
                                 pt_strm_atrbt->u.t_cmpb_video_stm_attr.pu1_buff,
                                 pt_strm_atrbt->u.t_cmpb_video_stm_attr.ui4_buff_size));
                    }
                }
                break;
            case MM_SVC_CMD_CODE_SET_PUSH_MODE_INFO:
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 &pt_cmd_sets[ui4_i].u, 
                                 RPC_DESC_MM_SVC_COMMAND_T_u, 
                                 "u.pv_data"));  
                /* Fix me: TO Be added ............ .............*/
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 pt_cmd_sets[ui4_i].u.pv_data, 
                                 RPC_DESC_MM_PB_CTRL_PUSH_MODE_INFO_T, NULL));  
                break;

            case MM_SVC_CMD_CODE_SET_PULL_MODE_INFO:
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 &pt_cmd_sets[ui4_i].u, 
                                 RPC_DESC_MM_SVC_COMMAND_T_u, 
                                 "u.pv_data"));  
                /* Fix me: TO Be added ............ .............*/
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 pt_cmd_sets[ui4_i].u.pv_data, 
                                 RPC_DESC_MM_PB_CTRL_PULL_INFO_T, NULL));  
                break;

            case MM_SVC_CMD_CODE_EXT_SBTL_PATH:
                 RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 &pt_cmd_sets[ui4_i].u,
                                 RPC_DESC_MM_SVC_COMMAND_T_u, 
                                 "u.pv_data"));  
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 pt_cmd_sets[ui4_i].u.pv_data, 
                                 RPC_DESC_MM_EXT_SBTL_INFO, 
                                 NULL));  
                switch(((MM_EXT_SBTL_INFO *)pt_cmd_sets[ui4_i].u.pv_data)->e_type)
                {
                    case MM_EXT_SBTL_TYPE_FULL:
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         &((MM_LYRIC_INFO *)pt_cmd_sets[ui4_i].u.pv_data)->u,
                                         RPC_DESC_MM_EXT_SBTL_INFO_u, 
                                         "u.ps_full"));             
                        RET_ON_FAIL(RPC_ADD_REF_STR(RPC_DEFAULT_ID, 
                                         ((MM_EXT_SBTL_INFO *)pt_cmd_sets[ui4_i].u.pv_data)->u.ps_full));
                        break;
                    case MM_EXT_SBTL_TYPE_LST:
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         &((MM_LYRIC_INFO *)pt_cmd_sets[ui4_i].u.pv_data)->u,
                                         RPC_DESC_MM_EXT_SBTL_INFO_u, 
                                         "u.ps_ext_lst"));             
                        RET_ON_FAIL(RPC_ADD_REF_STR(RPC_DEFAULT_ID, 
                                         ((MM_EXT_SBTL_INFO *)pt_cmd_sets[ui4_i].u.pv_data)->u.ps_ext_lst));
                        break;
                    default:
                        break;
                }
                
                RET_ON_FAIL(RPC_ADD_REF_STR(RPC_DEFAULT_ID, 
                                 ((MM_EXT_SBTL_INFO *)pt_cmd_sets[ui4_i].u.pv_data)->ps_prf_lang_lst));
                break;
             case MM_SVC_CMD_CODE_EXT_LYRIC_PATH:                 
                 RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 &pt_cmd_sets[ui4_i].u,
                                 RPC_DESC_MM_SVC_COMMAND_T_u, 
                                 "u.pv_data"));  
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 pt_cmd_sets[ui4_i].u.pv_data, 
                                 RPC_DESC_MM_LYRIC_INFO, 
                                 NULL));  
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 &((MM_LYRIC_INFO *)pt_cmd_sets[ui4_i].u.pv_data)->u,
                                 RPC_DESC_MM_LYRIC_INFO_u, 
                                 "u.ps_lyric_list"));                
                   RET_ON_FAIL(RPC_ADD_REF_STR(RPC_DEFAULT_ID, 
                                 ((MM_LYRIC_INFO *)pt_cmd_sets[ui4_i].u.pv_data)->u.ps_lyric_list));
                
                RET_ON_FAIL(RPC_ADD_REF_STR(RPC_DEFAULT_ID, 
                                 ((MM_LYRIC_INFO *)pt_cmd_sets[ui4_i].u.pv_data)->ps_prf_lang_lst));
                break;
            case MM_SVC_CMD_CODE_SET_PROTOCL:
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 &pt_cmd_sets[ui4_i].u, 
                                 RPC_DESC_MM_SVC_COMMAND_T_u, 
                                 NULL));
                break;
            case MM_SVC_CMD_CODE_SET_META_INFO:
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 &pt_cmd_sets[ui4_i].u, 
                                 RPC_DESC_MM_SVC_COMMAND_T_u, 
                                 "u.pv_data"));  
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 pt_cmd_sets[ui4_i].u.pv_data, 
                                 RPC_DESC_MINFO_INFO_T, NULL)); 
                break;
            case MM_SVC_CMD_CODE_SET_DRM_INFO:
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 &pt_cmd_sets[ui4_i].u,
                                 RPC_DESC_MM_SVC_COMMAND_T_u, 
                                 "u.pv_data"));   
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 pt_cmd_sets[ui4_i].u.pv_data, 
                                 RPC_DESC_MM_DRM_INFO_T, 
                                 NULL));
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 &((MM_DRM_INFO_T *)pt_cmd_sets[ui4_i].u.pv_data)->u,
                                 RPC_DESC_MM_DRM_INFO_T_u, 
                                 "u.t_mln_drm_inf")); 
                
                RET_ON_FAIL(RPC_ADD_REF_STR(RPC_DEFAULT_ID, 
                                 (CHAR*)(((MM_DRM_INFO_T *)pt_cmd_sets[ui4_i].u.pv_data)->u.t_mln_drm_inf.pui1_act_tkn_url)));
                
                RET_ON_FAIL(RPC_ADD_REF_STR(RPC_DEFAULT_ID, 
                                 (CHAR*)(((MM_DRM_INFO_T *)pt_cmd_sets[ui4_i].u.pv_data)->u.t_mln_drm_inf.pui1_afl_tkn)));
                break;
            case MM_SVC_CMD_CODE_SET_FILL_SCDB_MODE:    
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                 &pt_cmd_sets[ui4_i].u, 
                                 RPC_DESC_MM_SVC_COMMAND_T_u, 
                                 NULL));
                break;   
            case MM_SVC_CMD_CODE_SET_PVR_TOTAl_DUR:
            RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                             &pt_cmd_sets[ui4_i].u, 
                             RPC_DESC_MM_SVC_COMMAND_T_u, 
                             NULL));
            break;
            default:
                return RPCR_INV_ARGS;
        }
     }


    RET_ON_FAIL(RPC_ADD_REF_DESC_ARR(RPC_DEFAULT_ID, 
                         ui4_i + 1, 
                         pt_cmd_sets, 
                         RPC_DESC_MM_SVC_COMMAND_T, 
                         NULL));
    return RPCR_OK;
}


INT32 _hndlr_x_mm_playback_push_nfy_fct(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        VOID *       pv_cb_addr,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{  
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }
    /*Maybe not needed*/
    pt_return->e_type = ARG_TYPE_INT32;

    ((x_mm_playback_push_nfy_fct)pv_cb_addr)(
            (MM_PLAYBACK_CTRL_PUSH_EVENT_T)pt_args[0].u.ui4_arg,
            (VOID*)pt_args[1].u.ui4_arg,
            (UINT32)pt_args[2].u.i4_arg);

    return RPCR_OK;
}

INT32 _hndlr_x_svctx_media_prc_nfy_fct(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        VOID *       pv_cb_addr,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{  
    if(ui4_num_args != 6)
    {
        return RPCR_INV_ARGS;
    }
    /*Maybe not needed*/
    pt_return->e_type = ARG_TYPE_INT32;
    
    ((x_svctx_media_prc_nfy_fct)pv_cb_addr)(
            (HANDLE_T)pt_args[0].u.ui4_arg,
            (SVCTX_COND_T)pt_args[1].u.ui4_arg,
            (SVCTX_NTFY_CODE_T)pt_args[2].u.i4_arg,
            (STREAM_TYPE_T)pt_args[3].u.i4_arg,
            (VOID*)pt_args[4].u.pv_arg,
            (VOID*)pt_args[5].u.pv_arg);

    return RPCR_OK;
}

INT32 c_svctx_media_open(
                    HANDLE_T                        h_svctx,
                    MM_SVC_COMMAND_T*               pt_cmd,
                    x_svctx_media_prc_nfy_fct       pf_nfy,
                    VOID*                           pv_tag,
                    HANDLE_T*                       ph_media                      
                    )
{
    RPC_DECL(5, INT32);
    RPC_CHECK(_set_mm_svc_cmd_array(pt_cmd));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_svctx);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,   pt_cmd);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_tag);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_media);
    
    RPC_DO_OP("x_svctx_media_open");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}


INT32 c_svctx_media_close(
                    HANDLE_T                        h_svctx,
                    HANDLE_T                        h_media)
{
    RPC_DECL(2, INT32);
    
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_svctx);
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_media);
    
    RPC_DO_OP("x_svctx_media_close");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_media_get_info
 * Description 
 *      This API is used to get media information.
 * Inputs:  
 *  
 * Outputs: -
 * Returns: 
 *  
 *---------------------------------------------------------------------------*/
INT32 c_svctx_media_get_info(
                            HANDLE_T            h_svctx,                  
                            HANDLE_T            h_media,
                            SVCTX_GET_TYPE_T    t_svctx_get_type,
                            VOID*               pv_get_info,
                            SIZE_T              z_get_info_size)
{
    MINFO_INFO_T * pt_info;
    pt_info = pv_get_info;

    RPC_DECL(5, INT32);
        
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_svctx);
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_media);
    RPC_ARG_INP(ARG_TYPE_UINT32,     t_svctx_get_type);
    RPC_ARG_IO (ARG_TYPE_REF_DESC,   pv_get_info);
    RPC_ARG_IO (ARG_TYPE_SIZE_T,     z_get_info_size);
    
    switch(t_svctx_get_type)
    {
        case SVCTX_DRM_GET_TYPE_INFO:
        {
            DRM_INFO_SVCTX_T * pt_dvix_info = pv_get_info;
            if(pt_dvix_info == NULL)
            {
                break;
            }
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_get_info, RPC_DESC_DRM_INFO_SVCTX_T, NULL));
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, &pt_dvix_info->u, RPC_DESC_DRM_INFO_SVCTX_T_u, "u.t_divx_drm_info_svctx"));
#if 0
            if(pt_dvix_info->u.t_divx_drm_info_svctx.pt_divx_drm_basic_info != NULL)
            {
                RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_dvix_info->u.t_divx_drm_info_svctx.pt_divx_drm_basic_info, RPC_DESC_DIVX_DRM_BASIC_INFO_T, NULL));
            }
            if(pt_dvix_info->u.t_divx_drm_info_svctx.pt_drm_output_signal_protection_info != NULL)
            {
                RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_dvix_info->u.t_divx_drm_info_svctx.pt_drm_output_signal_protection_info, RPC_DESC_DRM_OUTPUT_SIGNAL_PROTECTION_INFO_T, NULL));
            }
#endif
            break;
        }
        case SVCTX_MM_GET_TYPE_MEDIA_FORMAT:
        {
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_get_info, RPC_DESC_MEDIA_FORMAT_T, NULL));
            break;
        }
        case SVCTX_MM_GET_TYPE_META_DATA:
        {
            if(z_get_info_size != 0)            
            { 
                RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_get_info, RPC_DESC_MINFO_INFO_T, NULL));

                if(pt_info->e_info_type == MINFO_INFO_TYPE_ID3_DATA)
                {
                    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, &pt_info->u, RPC_DESC_MINFO_INFO_T_u, "u.t_id3_data"));

                    if((pt_info->u.t_id3_data.pwc_str_id3_year != NULL)&&(pt_info->u.t_id3_data.z_id3_year_len != 0))
                    {    
                        RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_id3_data.pwc_str_id3_year,    sizeof(UTF16_T) * pt_info->u.t_id3_data.z_id3_year_len));
                    }
                    if((pt_info->u.t_id3_data.pwc_str_id3_title != NULL)&&(pt_info->u.t_id3_data.z_id3_title_len != 0))
                    {    
                        RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_id3_data.pwc_str_id3_title, sizeof(UTF16_T) * pt_info->u.t_id3_data.z_id3_title_len));
                    }
                    if((pt_info->u.t_id3_data.pwc_str_id3_artist != NULL)&&(pt_info->u.t_id3_data.z_id3_artist_len != 0))
                    {    
                        RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_id3_data.pwc_str_id3_artist,sizeof(UTF16_T) * pt_info->u.t_id3_data.z_id3_artist_len));
                    }
                    if((pt_info->u.t_id3_data.pwc_str_id3_album != NULL)&&(pt_info->u.t_id3_data.z_id3_album_len != 0))
                    {    
                        RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_id3_data.pwc_str_id3_album,    sizeof(UTF16_T) * pt_info->u.t_id3_data.z_id3_album_len));
                    }
                    if((pt_info->u.t_id3_data.pwc_str_id3_track != NULL)&&(pt_info->u.t_id3_data.z_id3_track_len != 0))
                    {    
                        RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_id3_data.pwc_str_id3_track,    sizeof(UTF16_T) * pt_info->u.t_id3_data.z_id3_track_len));
                    }
                    if((pt_info->u.t_id3_data.pwc_str_id3_genre != NULL)&&(pt_info->u.t_id3_data.z_id3_genre_len != 0))
                    {    
                        RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_id3_data.pwc_str_id3_genre,    sizeof(UTF16_T) * pt_info->u.t_id3_data.z_id3_genre_len));
                    }
                    break;
                }
                else if(pt_info->e_info_type == MINFO_INFO_TYPE_META_DATA)
                {
                    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, &pt_info->u, RPC_DESC_MINFO_INFO_T_u, "u.t_meta_data"));

                    if((pt_info->u.t_meta_data.pwc_str_title == NULL) &&
                        (pt_info->u.t_meta_data.pwc_str_director == NULL) &&
                        (pt_info->u.t_meta_data.pwc_str_copyright == NULL) &&
                        (pt_info->u.t_meta_data.pwc_str_genre == NULL) &&
                        (pt_info->u.t_meta_data.pwc_str_artist == NULL) &&
                        (pt_info->u.t_meta_data.pwc_str_album == NULL) &&
                        (pt_info->u.t_meta_data.pwc_str_track == NULL) &&
                        (pt_info->u.t_meta_data.pwc_str_year == NULL))
                    {
                        break;
                    }

                    if((pt_info->u.t_meta_data.pwc_str_title != NULL)&&(pt_info->u.t_meta_data.z_title_len != 0))
                    {    
                        RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_meta_data.pwc_str_title,    sizeof(UTF16_T) * pt_info->u.t_meta_data.z_title_len));
                    }
                    if((pt_info->u.t_meta_data.pwc_str_director != NULL)&&(pt_info->u.t_meta_data.z_director_len != 0))
                    {    
                        RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_meta_data.pwc_str_director, sizeof(UTF16_T) * pt_info->u.t_meta_data.z_director_len));
                    }
                    if((pt_info->u.t_meta_data.pwc_str_copyright != NULL)&&(pt_info->u.t_meta_data.z_copyright_len != 0))
                    {    
                        RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_meta_data.pwc_str_copyright,sizeof(UTF16_T) * pt_info->u.t_meta_data.z_copyright_len));
                    }
                    if((pt_info->u.t_meta_data.pwc_str_genre != NULL)&&(pt_info->u.t_meta_data.z_genre_len != 0))
                    {    
                        RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_meta_data.pwc_str_genre,    sizeof(UTF16_T) * pt_info->u.t_meta_data.z_genre_len));
                    }
                    if((pt_info->u.t_meta_data.pwc_str_artist != NULL)&&(pt_info->u.t_meta_data.z_artist_len != 0))
                    {    
                        RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_meta_data.pwc_str_artist,    sizeof(UTF16_T) * pt_info->u.t_meta_data.z_artist_len));
                    }
                    if((pt_info->u.t_meta_data.pwc_str_album != NULL)&&(pt_info->u.t_meta_data.z_album_len != 0))
                    {    
                        RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_meta_data.pwc_str_album,    sizeof(UTF16_T) * pt_info->u.t_meta_data.z_album_len));
                    }
                    if((pt_info->u.t_meta_data.pwc_str_track != NULL)&&(pt_info->u.t_meta_data.z_track_len != 0))
                    {    
                        RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_meta_data.pwc_str_track,    sizeof(UTF16_T) * pt_info->u.t_meta_data.z_track_len));
                    }
                    if((pt_info->u.t_meta_data.pwc_str_year != NULL)&&(pt_info->u.t_meta_data.z_year_len != 0))
                    {    
                        RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_info->u.t_meta_data.pwc_str_year,     sizeof(UTF16_T) * pt_info->u.t_meta_data.z_year_len));
                    }
                }
            }
            break;
        }
        case SVCTX_MM_GET_THUMBNAIL:
        {
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_get_info, RPC_DESC_THUMBNAIL_GET_INFO_T, NULL));
            break;
        }
        default:
        {
            RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_get_info, z_get_info_size));
            break;
        }
    }
    
    RPC_DO_OP("x_svctx_media_get_info");
    
    if (t_svctx_get_type == SVCTX_MM_GET_TYPE_PUSH_BUFF)
    {
        if (0 == mem_fd)
        {
            mem_fd = open("/dev/rmmgr", O_RDWR|O_SYNC);
            if(mem_fd < 0)
            {
                mem_fd = 0;
            }
            else
            {
                PLAYBACK_HANDLER_SHARED_MEMORY_T* pt_share_mem;
                
                pt_share_mem = (PLAYBACK_HANDLER_SHARED_MEMORY_T*)pv_get_info;
                pt_share_mem->pv_private2 = pt_share_mem->pv_buffer;
                pt_share_mem->pv_buffer = mmap(0, 
                                               pt_share_mem->z_buffer_size, 
                                               PROT_READ|PROT_WRITE, 
                                               MAP_SHARED, 
                                               mem_fd, 
                                               (UINT32)pt_share_mem->pv_private);
            }
        }
        else
        {
            PLAYBACK_HANDLER_SHARED_MEMORY_T* pt_share_mem;
            
            pt_share_mem = (PLAYBACK_HANDLER_SHARED_MEMORY_T*)pv_get_info;
            pt_share_mem->pv_private2 = pt_share_mem->pv_buffer;
            pt_share_mem->pv_buffer = mmap(0, 
                                           pt_share_mem->z_buffer_size, 
                                           PROT_READ|PROT_WRITE, 
                                           MAP_SHARED, 
                                           mem_fd, 
                                           (UINT32)pt_share_mem->pv_private);
        }
            
    }

    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_media_set_info
 * Description 
 *      This API is used to set media information.
 * Inputs:  
 *  
 * Outputs: -
 * Returns: 
 *  
 *---------------------------------------------------------------------------*/
INT32 c_svctx_media_set_info(
                            HANDLE_T h_svctx,                  
                            HANDLE_T h_media,
                            SVCTX_GET_TYPE_T t_svctx_set_type,
                            VOID* pv_set_info,
                            SIZE_T z_set_info_size)
{
    MINFO_INFO_T * pt_info;
    pt_info = pv_set_info;

    RPC_DECL(5, INT32);
        
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_svctx);
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_media);
    RPC_ARG_INP(ARG_TYPE_UINT32,     t_svctx_set_type);
    switch(t_svctx_set_type)
    {
        case SVCTX_MM_SET_TYPE_SEND_PUSH_BUF:
        {
            RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_set_info);
            RPC_ARG_INP (ARG_TYPE_SIZE_T, z_set_info_size);
            break;
        }

        case SVCTX_MM_SET_TYPE_REL_PUSH_BUF:
        {
            PLAYBACK_HANDLER_SHARED_MEMORY_T* pt_share_mem;
            pt_share_mem = (PLAYBACK_HANDLER_SHARED_MEMORY_T*)pv_set_info;

            munmap(pt_share_mem->pv_buffer, pt_share_mem->z_buffer_size);

            pt_share_mem->pv_buffer = pt_share_mem->pv_private2;

            RPC_ARG_INP (ARG_TYPE_REF_DESC,   pv_set_info);
            RPC_ARG_INP (ARG_TYPE_SIZE_T, z_set_info_size);
            RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_set_info, z_set_info_size));

            break;
        }

        default:
        {
            RPC_ARG_INP (ARG_TYPE_REF_DESC,   pv_set_info);
            RPC_ARG_INP (ARG_TYPE_SIZE_T, z_set_info_size);
            RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_set_info, z_set_info_size));
            break;
        }
    }
    
    RPC_DO_OP("x_svctx_media_set_info");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}


/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_generic_get_info
 * Description 
 *      This API is used to get generic media information. It doesn't need to open media first.
 * Inputs:  
 *  
 * Outputs: -
 * Returns: 
 *  
 *---------------------------------------------------------------------------*/
INT32 c_svctx_generic_get_info(
                                HANDLE_T            h_svctx,                  
                                HANDLE_T            h_media,
                                SVCTX_GET_TYPE_T    t_svctx_get_type,
                                VOID*               pv_get_info,
                                SIZE_T              z_get_info_size)
{
    RPC_DECL(5, INT32);
    
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_svctx);
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_media);
    RPC_ARG_INP(ARG_TYPE_UINT32,     t_svctx_get_type);
    RPC_ARG_IO (ARG_TYPE_REF_DESC,   pv_get_info);
    RPC_ARG_INP(ARG_TYPE_SIZE_T,     z_get_info_size);
    
    if(z_get_info_size)
    {
        if (t_svctx_get_type == SVCTX_GENGRIC_GET_TYPE_DIVX_DRM_REGISTRATION_CODE)
        {
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_get_info, RPC_DESC_DIVX_DRM_UNION_GENERIC_INFO_T, NULL));
        }
        else if (t_svctx_get_type == SVCTX_GENGRIC_GET_TYPE_DIVX_DRM_UI_HELP_INFO)
        {
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_get_info, RPC_DESC_DIVX_DRM_UNION_GENERIC_INFO_T, NULL));
        }
        else
        {
            RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_get_info, z_get_info_size));
        }
    }
    else
    {
        RPC_FAIL;
    }
    
    RPC_DO_OP("x_svctx_generic_get_info");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_generic_set_info
 * Description 
 *      This API is used to set generic media information. It doesn't need to open media first.
 * Inputs:  
 *  
 * Outputs: -
 * Returns: 
 *  
 *---------------------------------------------------------------------------*/
INT32 c_svctx_generic_set_info(
                                HANDLE_T            h_svctx,                  
                                HANDLE_T            h_media,
                                SVCTX_SET_TYPE_T    t_svctx_set_type,
                                VOID*               pv_set_info,
                                SIZE_T              z_set_info_size)
{
    RPC_DECL(5, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_svctx);
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_media);
    RPC_ARG_INP(ARG_TYPE_UINT32,     t_svctx_set_type);
    RPC_ARG_IO (ARG_TYPE_REF_DESC,   pv_set_info);
    RPC_ARG_INP(ARG_TYPE_SIZE_T,     z_set_info_size);
    
    if(z_set_info_size)
    {
        if (t_svctx_set_type == SVCTX_GENGRIC_SET_TYPE_DIVX_DRM_DEACTIVATION)
        {
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pv_set_info, RPC_DESC_DIVX_DRM_UNION_GENERIC_INFO_T, NULL));
        }
        else
        {
            RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_set_info, z_set_info_size));
        }
    }
    
    RPC_DO_OP("x_svctx_generic_set_info");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}


/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_media_seek
 * Description 
 *      This API is used to perfrom media search function 
 * Inputs:  
 *---------------------------------------------------------------------------*/
INT32 c_svctx_media_seek(
                    HANDLE_T                        h_svctx,
                    x_svctx_select_nfy_fct          pf_nfy,
                    VOID*                           pv_tag,
                    MM_SVC_POS_INFO_T*              pt_pos_info
                    )
{
    RPC_DECL(4, INT32);
    
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_svctx);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_tag);
    RPC_ARG_IO(ARG_TYPE_REF_DESC,   pt_pos_info);
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_pos_info, RPC_DESC_MM_SVC_POS_INFO_T, NULL));    
    
    RPC_DO_OP("x_svctx_media_seek");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_media_load_thumbnail
 * Description 
 *      This API is used to load video thumbnail
 * Inputs:  
 *---------------------------------------------------------------------------*/
INT32 c_svctx_media_load_thumbnail(
                    HANDLE_T                        h_svctx,
                    HANDLE_T                        h_media,
                    SVCTX_VIDEO_THUMBNAIL_INFO_T*   pt_thumbnail_info
                    )
{
    RPC_DECL(3, INT32);
    
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_svctx);
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_media);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,   pt_thumbnail_info);
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_thumbnail_info, RPC_DESC_SVCTX_VIDEO_THUMBNAIL_INFO_T, NULL));    

    RPC_DO_OP("x_svctx_media_load_thumbnail");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

/*-----------------------------------------------------------------------------
 * Name
 *      c_svctx_media_sync_cancel_load_thumbnail
 * Description 
 *      This API is used to sync cancel the loading of video thumbnail
 * Inputs:  
 *---------------------------------------------------------------------------*/
INT32 c_svctx_media_sync_cancel_load_thumbnail(
                    HANDLE_T                        h_svctx,
                    HANDLE_T                        h_media
                    )
{
    RPC_DECL(2, INT32);
    
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_svctx);
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_media);
    
    RPC_DO_OP("x_svctx_media_sync_cancel_load_thumbnail");
    
    RPC_RETURN(ARG_TYPE_INT32, SVCTXR_FAIL);
}

static INT32 _hndlr_x_mm_playback_pull_open_fct(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        VOID *       pv_cb_addr,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    printf("enter _hndlr_x_mm_playback_pull_open_fct() \n" );
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type = ARG_TYPE_INT32;
    
    pt_return->u.i4_arg = ((x_mm_playback_pull_open_fct)pv_cb_addr)(
                                (HANDLE_T)pt_args[0].u.ui4_arg,
                                (HANDLE_T*)pt_args[1].u.pui4_arg,
                                (VOID*)pt_args[2].u.pv_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mm_playback_pull_associate_fct(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        VOID *       pv_cb_addr,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    VOID*   pv_virt_addr = 0;
    
    printf("enter _hndlr_x_mm_playback_pull_associate_fct() \n" );
    
    if(ui4_num_args != 2)
    {
        printf("_hndlr_x_mm_playback_pull_associate_fct(), ui4_num_args != 2, return \n");
        return RPCR_INV_ARGS;
    }
    pt_return->e_type = ARG_TYPE_INT32;

    {
        if (0 == rmmgr_fd)
        {
            rmmgr_fd = open("/dev/rmmgr", O_RDWR|O_SYNC);
            printf("_hndlr_x_mm_playback_pull_associate_fct(), rmmgr_fd = %d \n", rmmgr_fd);
            if(rmmgr_fd < 0)
            {
                rmmgr_fd = 0;
            }
            else
            {
                pv_virt_addr = mmap(0, 
                                    (UINT32)pt_args[1].u.ui4_arg, 
                                    PROT_READ|PROT_WRITE, 
                                    MAP_SHARED, 
                                    rmmgr_fd, 
                                    pt_args[0].u.ui4_arg);
            }
        }
        else
        {
            printf("_hndlr_x_mm_playback_pull_associate_fct(), go else case() \n");
            pv_virt_addr = mmap(0, 
                                (UINT32)pt_args[1].u.ui4_arg, 
                                PROT_READ|PROT_WRITE, 
                                MAP_SHARED, 
                                rmmgr_fd, 
                                pt_args[0].u.ui4_arg);
        }
    }

    pt_return->u.i4_arg = (INT32)pv_virt_addr;

    printf("_hndlr_x_mm_playback_pull_associate_fct(), phy addr = %x, len = %d, viraddr = %x \n" , 
        (unsigned int)pt_args[0].u.ui4_arg, (int)pt_args[1].u.ui4_arg, (unsigned int)pv_virt_addr);

    return RPCR_OK;
}


static INT32 _hndlr_x_mm_playback_pull_dissociate_fct(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        VOID *       pv_cb_addr,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type = ARG_TYPE_INT32;
    
    munmap((VOID*)pt_args[0].u.ui4_arg, (SIZE_T)pt_args[1].u.ui4_arg);
    
    pt_return->u.i4_arg = 0;

    return RPCR_OK;
}


static INT32 _hndlr_x_mm_playback_pull_close_fct(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        VOID *       pv_cb_addr,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 2)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type = ARG_TYPE_INT32;
    
    pt_return->u.i4_arg = ((x_mm_playback_pull_close_fct)pv_cb_addr)(
                                (HANDLE_T)pt_args[0].u.ui4_arg,
                                (VOID*)pt_args[1].u.pv_arg);

    return RPCR_OK;
}


static INT32 _hndlr_x_mm_playback_pull_read_fct(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        VOID *       pv_cb_addr,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    printf("enter _hndlr_x_mm_playback_pull_read_fct() \n" );
    if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type = ARG_TYPE_INT32;
    
    pt_return->u.i4_arg = ((x_mm_playback_pull_read_fct)pv_cb_addr)(
                                (HANDLE_T)pt_args[0].u.ui4_arg,
                                (VOID*)pt_args[1].u.pv_arg,
                                (UINT8*)pt_args[2].u.pv_arg,
                                (UINT32)pt_args[3].u.ui4_arg,
                                (UINT32*)pt_args[4].u.pui4_arg);

    return RPCR_OK;
}

static HANDLE_T h_cmpb_cb_thread = NULL_HANDLE;
static HANDLE_T h_cmpb_cb_msg_que = NULL_HANDLE;
static BOOL b_cb_inited = FALSE;

typedef struct
{
    MM_PLAYBACK_CTRL_PULL_EVENT_T  e_event;
    VOID*                          pv_tag;
    HANDLE_T                       h_req;
    UINT32                         ui4_data;
} PULL_CB_MSG_T;


static VOID _cmpb_cb_send_msg(const PULL_CB_MSG_T* pt_msg)
{
    INT32               i4_ret; 
    INT32               i4_retry;

    i4_retry = 0;
    while ( i4_retry < 10 )
    {
        i4_retry++;
        i4_ret = x_msg_q_send( h_cmpb_cb_msg_que,
                               pt_msg,
                               sizeof( PULL_CB_MSG_T ),
                               0 );
        if ( i4_ret != OSR_OK )
        {
            /* Send message fail, wait 10 ms and retry for 10 times */
            x_thread_delay( 10 );
            continue;
        }

        /* success */
        return;
    }
}

static VOID _cmbp_callback_thread(VOID*   pv_data)
{
    INT32                       i4_ret;
    PULL_CB_MSG_T               t_msg;
    SIZE_T                      z_msg_size = sizeof(PULL_CB_MSG_T);
    UINT16                      ui2_index;

    while (1)
    {
        x_memset( &t_msg, 0, sizeof(PULL_CB_MSG_T) );

        i4_ret = x_msg_q_receive( &ui2_index,
                                  &t_msg,
                                  &z_msg_size,
                                  &h_cmpb_cb_msg_que,
                                  (UINT16)1,
                                  X_MSGQ_OPTION_WAIT );
        if ( i4_ret == OSR_NO_MSG )
        {
            continue;
        }
        
        if ( i4_ret != OSR_OK )
        {
            //ASSERT(0, ("_cmbp_callback_thread x_msg_q_receive error=%d\n", i4_ret));
            x_thread_exit();
            return;
        }

        {
            RPC_CB_NFY_TAG_T*           pt_nfy_tag;
            RPC_DECL(5, INT32);
            
            pt_nfy_tag = t_msg.pv_tag;
            
            RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pt_nfy_tag->pv_cb_addr);
            RPC_ARG_INP(ARG_TYPE_UINT32,     t_msg.e_event);
            RPC_ARG_INP(ARG_TYPE_REF_VOID,   pt_nfy_tag->pv_tag);
            RPC_ARG_INP(ARG_TYPE_UINT32,     t_msg.h_req);
            RPC_ARG_INP(ARG_TYPE_UINT32,     t_msg.ui4_data);
            
            RPC_DO_OP("x_mm_playback_pull_nfy_fct");
            
            ri_free_cb_tag(pt_nfy_tag);
            __ret = 0;/*just to fix warning*/
        }
        

    }
}

static VOID _cmpb_cb_init(VOID)
{
    INT32   i4_ret;
    
    if (b_cb_inited == TRUE)
    {
        return;
    }

    if (NULL_HANDLE == h_cmpb_cb_msg_que)
    {
        i4_ret = x_msg_q_create( &h_cmpb_cb_msg_que,
                                 "rw_pull_callback_queue",
                                 sizeof( PULL_CB_MSG_T ),
                                 100 );
        if ( i4_ret != OSR_OK )
        {
            return;
        }
    }

    if (NULL_HANDLE == h_cmpb_cb_thread)
    {
        i4_ret = x_thread_create(&h_cmpb_cb_thread,
                                 "rw_pull_callback_thread",
                                 8192,
                                 100,
                                 _cmbp_callback_thread,
                                 sizeof(HANDLE_T *),
                                 (VOID *)&h_cmpb_cb_msg_que);

        if (i4_ret != OSR_OK)
        {
            x_msg_q_delete(h_cmpb_cb_thread);
            return;
        }
    }
}

static INT32 _x_mm_playback_pull_nfy_fct_wrapper(
                        MM_PLAYBACK_CTRL_PULL_EVENT_T  e_event,
                        VOID*                          pv_tag,
                        HANDLE_T                       h_req,
                        UINT32                         ui4_data)

{
#if 0
#if 1
    RPC_CB_NFY_TAG_T*           pt_nfy_tag;
    RPC_DECL(5, INT32);

    pt_nfy_tag = pv_tag;

    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pt_nfy_tag->pv_cb_addr);
    RPC_ARG_INP(ARG_TYPE_UINT32,     e_event);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pt_nfy_tag->pv_tag);
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_req);
    RPC_ARG_INP(ARG_TYPE_UINT32,     ui4_data);

    printf("rw rasync nfy 1\n");
    RPC_DO_OP("x_mm_playback_pull_nfy_fct");
    printf("rw rasync nfy 2\n");

    ri_free_cb_tag(pt_nfy_tag);
    
    RPC_RETURN(ARG_TYPE_INT32, MM_PLAYBACK_CTRL_PULL_FAIL);
#else
    RPC_DECL(4, INT32);

    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_tag;

    RPC_ARG_INP(ARG_TYPE_UINT32,    e_event);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,  pt_nfy_tag->apv_cb_addr_ex[0]);
    RPC_ARG_INP(ARG_TYPE_UINT32,    pt_nfy_tag->pv_tag);
    RPC_ARG_INP(ARG_TYPE_UINT32,    ui4_data);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_mm_playback_pull_nfy_fct", pt_nfy_tag->pv_cb_addr);

    ri_free_cb_tag(pt_nfy_tag);
    RPC_RETURN(ARG_TYPE_INT32, MM_PLAYBACK_CTRL_PULL_FAIL);
#endif
#else
    PULL_CB_MSG_T t_msg;

    t_msg.e_event = e_event;
    t_msg.h_req = h_req;
    t_msg.pv_tag = pv_tag;
    t_msg.ui4_data = ui4_data;

    _cmpb_cb_send_msg(&t_msg);
    return 0;
#endif
}


static INT32 _hndlr_x_mm_playback_pull_read_async_fct(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        VOID *       pv_cb_addr,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T*           pt_nfy_tag;
    x_mm_playback_pull_nfy_fct  pf_nfy;

    if(ui4_num_args != 7)
    {
        return RPCR_INV_ARGS;
    }

    pf_nfy      = _x_mm_playback_pull_nfy_fct_wrapper;
    pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, &pt_args[4].u.pv_func, 1, pt_args[5].u.pv_arg);
    

    pt_return->e_type = ARG_TYPE_INT32;

    pt_return->u.i4_arg = ((x_mm_playback_pull_read_async_fct)pv_cb_addr)(
                                (HANDLE_T)pt_args[0].u.ui4_arg,
                                (VOID*)pt_args[1].u.pv_arg,
                                (UINT8*)pt_args[2].u.pv_arg,
                                (UINT32)pt_args[3].u.ui4_arg,
                                (x_mm_playback_pull_nfy_fct)pf_nfy,
                                (VOID*)pt_nfy_tag,
                                (HANDLE_T*)pt_args[6].u.pui4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mm_playback_pull_abort_read_async_fct(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        VOID *       pv_cb_addr,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type = ARG_TYPE_INT32;
    
    pt_return->u.i4_arg = ((x_mm_playback_pull_abort_read_async_fct)pv_cb_addr)(
                                (HANDLE_T)pt_args[0].u.ui4_arg,
                                (VOID*)pt_args[1].u.pv_arg,
                                (HANDLE_T)pt_args[2].u.ui4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mm_playback_pull_byteseek_fct(
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
    pt_return->e_type = ARG_TYPE_INT32;

    pt_return->u.i4_arg = ((x_mm_playback_pull_byteseek_fct)pv_cb_addr)(
                                (HANDLE_T)pt_args[0].u.ui4_arg,
                                (VOID*)pt_args[1].u.pv_arg,
                                (UINT64)pt_args[2].u.ui8_arg,
                                (UINT8)pt_args[3].u.ui1_arg,
                                (UINT64*)pt_args[4].u.pui8_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_mm_playback_pull_get_input_lengh_fct(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_cb_type,
                        VOID *       pv_cb_addr,
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }
    pt_return->e_type = ARG_TYPE_INT32;

    pt_return->u.i4_arg = ((x_mm_playback_pull_get_input_lengh_fct)pv_cb_addr)(
                                (HANDLE_T)pt_args[0].u.ui4_arg,
                                (VOID*)pt_args[1].u.pv_arg,
                                (UINT64*)pt_args[2].u.pui8_arg);

    return RPCR_OK;
}

INT32 _hndlr_divx_drm_memory_read_func(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }
    /*Maybe not needed*/
    pt_return->e_type = ARG_TYPE_BOOL;
    
    pt_return->u.b_arg = ((divx_drm_memory_read_func)pv_cb_addr)(
                            (UINT8*)pt_args[0].u.pv_arg);

    return RPCR_OK;
}

INT32 _hndlr_divx_drm_memory_write_func(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{
    if(ui4_num_args != 1)
    {
        return RPCR_INV_ARGS;
    }
    /*Maybe not needed*/
    pt_return->e_type = ARG_TYPE_BOOL;
    
    pt_return->u.b_arg = ((divx_drm_memory_write_func)pv_cb_addr)(
                            (UINT8*)pt_args[0].u.pv_arg);

    return RPCR_OK;
}

INT32 c_rpc_reg_mm_svctx_cb_hndlrs()
{
    _cmpb_cb_init();
    RPC_REG_CB_HNDLR(x_svctx_media_prc_nfy_fct);
    RPC_REG_CB_HNDLR(x_mm_playback_push_nfy_fct);
    
    RPC_REG_CB_HNDLR(x_mm_playback_pull_open_fct);
    RPC_REG_CB_HNDLR(x_mm_playback_pull_associate_fct);
    RPC_REG_CB_HNDLR(x_mm_playback_pull_dissociate_fct);
    RPC_REG_CB_HNDLR(x_mm_playback_pull_close_fct);
    RPC_REG_CB_HNDLR(x_mm_playback_pull_read_fct);
    RPC_REG_CB_HNDLR(x_mm_playback_pull_read_async_fct);
    RPC_REG_CB_HNDLR(x_mm_playback_pull_abort_read_async_fct);
    RPC_REG_CB_HNDLR(x_mm_playback_pull_byteseek_fct);
    RPC_REG_CB_HNDLR(x_mm_playback_pull_get_input_lengh_fct);
    RPC_REG_CB_HNDLR(divx_drm_memory_read_func);
    RPC_REG_CB_HNDLR(divx_drm_memory_write_func);
    return RPCR_OK;
}



