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
#include <sys/types.h>
#include <sys/ipc.h>
#ifndef ANDROID
#include <sys/shm.h>
#endif

#include "svctx/x_svctx.h"
#include "handle/handle.h"
#include "svctx/svctx_dbg.h"
#include "rpc.h"
#include "ri_common.h"
#include "ri_desc.h"

typedef struct _DIVX_DRM_MEMORY_RW_FUNC_CB_T
{
    RPC_ID_T                    t_rpc_id;
    divx_drm_memory_read_func   pfn_divx_drm_memory_read_func;
    divx_drm_memory_read_func   pfn_divx_drm_memory_write_func;
}  DIVX_DRM_MEMORY_RW_FUNC_CB_T ;

typedef struct _PULL_MEMMAP_T
{
    HANDLE_T    h_pull;
    UINT32      ui4_phy_addr;
    UINT32      ui4_size;
    UINT32      ui4_virt_addr;
}  PULL_MEMMAP_T ;

static DIVX_DRM_MEMORY_RW_FUNC_CB_T t_divx_drm_cb = {0};

static VOID _pull_tag_free_fnc(RPC_CB_NFY_TAG_T*  pt_cb_tag)
{
    UINT8 ui1_idx;
    
    for (ui1_idx=10; ui1_idx<20; ui1_idx++)
    {
        if (pt_cb_tag->apv_cb_addr_ex[ui1_idx])
        {
            free(pt_cb_tag->apv_cb_addr_ex[ui1_idx]);
        }
    }
}

static VOID _x_mm_playback_push_nfy_fct_wrapper(
                                MM_PLAYBACK_CTRL_PUSH_EVENT_T   e_event,
                                VOID*                           pv_tag,
                                UINT32                          ui4_data)
{
    RPC_DECL_VOID(3);

    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_tag;

    RPC_ARG_INP(ARG_TYPE_UINT32,   e_event);         
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_data);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_mm_playback_push_nfy_fct", pt_nfy_tag->pv_cb_addr);
    
    RPC_RETURN_VOID;
}

static VOID _x_svctx_media_prc_nfy_fct_wrapper(
                    HANDLE_T                    h_svctx,          /* in  */
                    SVCTX_COND_T                e_nfy_cond,       /* in  */
                    SVCTX_NTFY_CODE_T           e_code,           /* in  */
                    STREAM_TYPE_T               e_stream_type,    /* in  */
                    VOID*                       pv_nfy_tag,       /* in  */
                    VOID*                       pv_data           /* in  */)
{    
    RPC_DECL_VOID(6);
    
    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_nfy_tag;
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_svctx);
    RPC_ARG_INP(ARG_TYPE_UINT32,   e_nfy_cond);        
    RPC_ARG_INP(ARG_TYPE_INT32,    e_code);             
    RPC_ARG_INP(ARG_TYPE_INT32,    e_stream_type);          
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->pv_tag);
    RPC_ARG_INP(ARG_TYPE_UINT32,   pv_data);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_svctx_media_prc_nfy_fct", pt_nfy_tag->pv_cb_addr);
    
    RPC_RETURN_VOID;    
}

static INT32 _x_mm_playback_pull_open_fct_wrapper(
                HANDLE_T    h_pb,
                HANDLE_T*   ph_pull,
                VOID*       pv_app_tag)
{
    RPC_DECL(3, INT32);

    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_app_tag;

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_pb);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_pull);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->apv_cb_addr_ex[0]);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_mm_playback_pull_open_fct", pt_nfy_tag->apv_cb_addr_ex[1]);

    RPC_RETURN(ARG_TYPE_INT32, MM_PLAYBACK_CTRL_PULL_FAIL);
}

static INT32 _x_mm_playback_pull_associate_fct_wrapper(
                HANDLE_T    h_pull,
                VOID*       pv_app_tag,
                UINT32      ui4_phy_addr,
                UINT32      ui4_size)

{
    UINT8 ui1_idx;
    RPC_DECL(2, INT32);

    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_app_tag;

    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_phy_addr);
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_size);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_mm_playback_pull_associate_fct", pt_nfy_tag->apv_cb_addr_ex[8]);

    for (ui1_idx=10; ui1_idx<20; ui1_idx++)
    {
        if (NULL == pt_nfy_tag->apv_cb_addr_ex[ui1_idx])
        {
            PULL_MEMMAP_T* pv_tmp;
            pt_nfy_tag->apv_cb_addr_ex[ui1_idx] = malloc(sizeof(PULL_MEMMAP_T));
            pv_tmp = pt_nfy_tag->apv_cb_addr_ex[ui1_idx];
            pv_tmp->h_pull = h_pull;
            pv_tmp->ui4_phy_addr = ui4_phy_addr;
            pv_tmp->ui4_size = ui4_size;
            pv_tmp->ui4_virt_addr = __t_ret.u.ui4_arg;
            break;
        }
    }
    if (ui1_idx >= 20)
    {
        printf("!!!!! _x_mm_playback_pull_associate_fct_wrapper too many association\n");
    }
    

    RPC_RETURN(ARG_TYPE_INT32, MM_PLAYBACK_CTRL_PULL_FAIL);
}

static INT32 _x_mm_playback_pull_dissociate_fct_wrapper(
                HANDLE_T  h_pull,
                VOID*     pv_app_tag)
{
    UINT8 ui1_idx;
    PULL_MEMMAP_T* pv_tmp;

    RPC_DECL(2, INT32);

    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_app_tag;

    for (ui1_idx=10; ui1_idx<20; ui1_idx++)
    {
        if (pt_nfy_tag->apv_cb_addr_ex[ui1_idx])
        {
            pv_tmp = pt_nfy_tag->apv_cb_addr_ex[ui1_idx];

            if (pv_tmp->h_pull == h_pull)
            {
                break;
            }
        }
    }
    if (ui1_idx >= 20)
    {
        return 0;
    }

    RPC_ARG_INP(ARG_TYPE_UINT32, pv_tmp->ui4_virt_addr);
    RPC_ARG_INP(ARG_TYPE_UINT32, pv_tmp->ui4_size);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_mm_playback_pull_dissociate_fct", (VOID *)pv_tmp->ui4_virt_addr);

    free(pv_tmp);
    pt_nfy_tag->apv_cb_addr_ex[ui1_idx] = NULL;

    RPC_RETURN(ARG_TYPE_INT32, MM_PLAYBACK_CTRL_PULL_FAIL);

}

static INT32 _x_mm_playback_pull_close_fct_wrapper(
                HANDLE_T  h_pull,
                VOID*     pv_app_tag)
{
    RPC_DECL(2, INT32);

    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_app_tag;

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_pull);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->apv_cb_addr_ex[0]);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_mm_playback_pull_close_fct", pt_nfy_tag->apv_cb_addr_ex[2]);

    RPC_RETURN(ARG_TYPE_INT32, MM_PLAYBACK_CTRL_PULL_FAIL);

}


static INT32 _x_mm_playback_pull_read_fct_wrapper(
                HANDLE_T  h_pull,
                VOID*     pv_app_tag,
                UINT8*    pu1_dst,
                UINT32    ui4_count,                                           
                UINT32*   pui4_read)
{
    VOID*   pv_addr;
    
    RPC_DECL(5, INT32);

    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_app_tag;

    PULL_MEMMAP_T* pv_tmp;
    UINT8 ui1_idx;

    for (ui1_idx=10; ui1_idx<20; ui1_idx++)
    {
        if (pt_nfy_tag->apv_cb_addr_ex[ui1_idx])
        {
            pv_tmp = pt_nfy_tag->apv_cb_addr_ex[ui1_idx];

            if (pv_tmp->h_pull == h_pull)
            {
                break;
            }
        }
    }
    if (ui1_idx >= 20)
    {
        return MM_PLAYBACK_CTRL_PULL_FAIL;
    }

    pv_addr = (VOID *)(pv_tmp->ui4_virt_addr + ((UINT32)pu1_dst-pv_tmp->ui4_phy_addr));

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_pull);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->apv_cb_addr_ex[0]);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_addr);
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_count);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,   pui4_read);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_mm_playback_pull_read_fct", pt_nfy_tag->apv_cb_addr_ex[3]);

    RPC_RETURN(ARG_TYPE_INT32, MM_PLAYBACK_CTRL_PULL_FAIL);
}

#if 1
static INT32 _hndlr_x_mm_playback_pull_nfy_fct(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_op,    
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    x_mm_playback_pull_nfy_fct pf_nfy;
    
    if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }

    pf_nfy = (x_mm_playback_pull_nfy_fct)pt_args[0].u.pv_func;
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = pf_nfy(
                            (MM_PLAYBACK_CTRL_PULL_EVENT_T)pt_args[1].u.ui4_arg,
                            (VOID*)pt_args[2].u.pv_arg,
                            (HANDLE_T)pt_args[3].u.ui4_arg,
                            (UINT32)pt_args[4].u.ui4_arg);
    
    return RPCR_OK;
}
#else
static INT32 _hndlr_x_mm_playback_pull_nfy_fct(
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
    pt_return->e_type = ARG_TYPE_INT32;
    
    pt_return->u.i4_arg = ((x_mm_playback_pull_nfy_fct)pv_cb_addr)(
                                (MM_PLAYBACK_CTRL_PULL_EVENT_T)pt_args[0].u.ui4_arg,
                                (VOID*)pt_args[1].u.pv_arg,
                                (HANDLE_T)pt_args[2].u.ui4_arg,
                                (UINT32)pt_args[3].u.ui4_arg);

    return RPCR_OK;
}
#endif


static INT32 _x_mm_playback_pull_read_async_fct_wrapper(
                HANDLE_T                    h_pull,
                VOID*                       pv_app_tag,
                UINT8*                      pu1_dst,
                UINT32                      ui4_count,
                x_mm_playback_pull_nfy_fct  pf_nfy,
                VOID*                       pv_tag,
                HANDLE_T*                   ph_req)
{
    RPC_DECL(7, INT32);

    VOID*   pv_addr;
    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_app_tag;
    PULL_MEMMAP_T* pv_tmp;
    UINT8 ui1_idx;

    for (ui1_idx=10; ui1_idx<20; ui1_idx++)
    {
        if (pt_nfy_tag->apv_cb_addr_ex[ui1_idx])
        {
            pv_tmp = pt_nfy_tag->apv_cb_addr_ex[ui1_idx];

            if (pv_tmp->h_pull == h_pull)
            {
                break;
            }
        }
    }
    if (ui1_idx >= 20)
    {
        return MM_PLAYBACK_CTRL_PULL_FAIL;
    }

    pv_addr = (VOID*)(pv_tmp->ui4_virt_addr + ((UINT32)pu1_dst-pv_tmp->ui4_phy_addr));

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_pull);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->apv_cb_addr_ex[0]);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_addr);
    RPC_ARG_INP(ARG_TYPE_UINT32,   ui4_count);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_tag);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,   ph_req);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_mm_playback_pull_read_async_fct", pt_nfy_tag->apv_cb_addr_ex[4]);

    RPC_RETURN(ARG_TYPE_INT32, MM_PLAYBACK_CTRL_PULL_FAIL);
}

static INT32 _x_mm_playback_pull_abort_read_async_fct_wrapper(
                HANDLE_T            h_pull,
                VOID*               pv_app_tag,
                HANDLE_T            h_req)
{
    RPC_DECL(3, INT32);

    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_app_tag;

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_pull);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->apv_cb_addr_ex[0]);
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_req);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_mm_playback_pull_abort_read_async_fct", pt_nfy_tag->apv_cb_addr_ex[5]);

    RPC_RETURN(ARG_TYPE_INT32, MM_PLAYBACK_CTRL_PULL_FAIL);
}

static INT32 _x_mm_playback_pull_byteseek_fct_wrapper(
                HANDLE_T  h_pull,
                VOID*     pv_app_tag,
                UINT64    u8SeekPos,
                UINT8     u1Whence,
                UINT64*   pu8CurPos)
{
    RPC_DECL(5, INT32);

    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_app_tag;

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_pull);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->apv_cb_addr_ex[0]);
    RPC_ARG_INP(ARG_TYPE_UINT64,   u8SeekPos);
    RPC_ARG_INP(ARG_TYPE_UINT8,   u1Whence);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT64,   pu8CurPos);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_mm_playback_pull_byteseek_fct", pt_nfy_tag->apv_cb_addr_ex[6]);

    RPC_RETURN(ARG_TYPE_INT32, MM_PLAYBACK_CTRL_PULL_FAIL);
}

static INT32 _x_mm_playback_pull_get_input_lengh_fct_wrapper(
                HANDLE_T  h_pull,
                VOID*     pv_app_tag,
                UINT64*   pui8_len)
{
    RPC_DECL(3, INT32);

    RPC_CB_NFY_TAG_T * pt_nfy_tag = pv_app_tag;

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_pull);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pt_nfy_tag->apv_cb_addr_ex[0]);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT64,   pui8_len);

    RPC_DO_CB(pt_nfy_tag->t_id, "x_mm_playback_pull_get_input_lengh_fct", pt_nfy_tag->apv_cb_addr_ex[7]);

    RPC_RETURN(ARG_TYPE_INT32, MM_PLAYBACK_CTRL_PULL_FAIL);
}


static INT32 _hndlr_x_svctx_media_open(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_op,    
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T*           pt_nfy_tag=NULL;
    x_svctx_media_prc_nfy_fct   pf_nfy;

    RPC_CB_NFY_TAG_T*               pt_nfy_tag_push=NULL;
    RPC_CB_NFY_TAG_T*               pt_nfy_tag_pull=NULL;
    x_mm_playback_push_nfy_fct      pf_nfy_push;
    UINT32                          ui4_i = 0;
    MM_SVC_COMMAND_T*               pt_iter;
    MM_PB_CTRL_PUSH_MODE_INFO_T*    pt_push_info;
    MM_PB_CTRL_PULL_INFO_T*         pt_pull_info;

    if(ui4_num_args != 5)
    {
        pt_return->e_type   = ARG_TYPE_INT32;        
        pt_return->u.i4_arg = SVCTXR_FAIL;    
        return RPCR_INV_ARGS;
    }

    pt_iter = (MM_SVC_COMMAND_T*)(pt_args[1].u.pv_desc);

    for (ui4_i = 0; pt_iter[ui4_i].e_code != MM_SVC_CMD_CODE_END; ui4_i ++)
    {
        if (pt_iter[ui4_i].e_code == MM_SVC_CMD_CODE_SET_PUSH_MODE_INFO)
        {
            pt_push_info = (MM_PB_CTRL_PUSH_MODE_INFO_T*)pt_iter[ui4_i].u.pv_data;
            pf_nfy_push = pt_push_info->pf_nfy;
            pt_push_info->pf_nfy = _x_mm_playback_push_nfy_fct_wrapper;
            pt_nfy_tag_push  = ri_create_cb_tag(t_rpc_id, (VOID *)&pf_nfy_push, 1, (VOID *)pt_push_info->hHandle);
            pt_push_info->hHandle = (HANDLE_T)pt_nfy_tag_push;
        }
        else if(pt_iter[ui4_i].e_code == MM_SVC_CMD_CODE_SET_PULL_MODE_INFO)
        {
            VOID* av_cb[10];

            pt_pull_info = (MM_PB_CTRL_PULL_INFO_T*)pt_iter[ui4_i].u.pv_data;
            av_cb[0] = (VOID*)pt_pull_info->pv_app_tag;
            av_cb[1] = (VOID*)pt_pull_info->pf_open;
            av_cb[2] = (VOID*)pt_pull_info->pf_close;
            av_cb[3] = (VOID*)pt_pull_info->pf_read;
            av_cb[4] = (VOID*)pt_pull_info->pf_read_async;
            av_cb[5] = (VOID*)pt_pull_info->pf_abort_read_async;
            av_cb[6] = (VOID*)pt_pull_info->pf_byteseek;
            av_cb[7] = (VOID*)pt_pull_info->pf_get_input_len;
            av_cb[8] = (VOID*)_x_mm_playback_pull_associate_fct_wrapper;
            av_cb[9] = (VOID*)_x_mm_playback_pull_dissociate_fct_wrapper;

            pt_nfy_tag_pull = ri_create_cb_tag_with_free(t_rpc_id, av_cb, 10, pt_pull_info->pv_app_tag, _pull_tag_free_fnc);

            pt_pull_info->pv_app_tag = (VOID*)pt_nfy_tag_pull;
            pt_pull_info->pf_open = _x_mm_playback_pull_open_fct_wrapper;
            pt_pull_info->pf_close = _x_mm_playback_pull_close_fct_wrapper;
            pt_pull_info->pf_read = _x_mm_playback_pull_read_fct_wrapper;
            pt_pull_info->pf_read_async = _x_mm_playback_pull_read_async_fct_wrapper;
            pt_pull_info->pf_abort_read_async = _x_mm_playback_pull_abort_read_async_fct_wrapper;
            pt_pull_info->pf_byteseek = _x_mm_playback_pull_byteseek_fct_wrapper;
            pt_pull_info->pf_get_input_len = _x_mm_playback_pull_get_input_lengh_fct_wrapper;
            pt_pull_info->pf_associate = _x_mm_playback_pull_associate_fct_wrapper;
            pt_pull_info->pf_dissociate = _x_mm_playback_pull_dissociate_fct_wrapper;
        }
    }
    
    if(pt_args[2].u.pv_func == NULL)
    {
        pf_nfy      = NULL;
        pt_nfy_tag  = NULL;
    }
    else
    {
        pf_nfy      = _x_svctx_media_prc_nfy_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, &pt_args[2].u.pv_func, 1, pt_args[3].u.pv_arg);
    }
   
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_media_open(pt_args[0].u.ui4_arg,
                                            (MM_SVC_COMMAND_T*)pt_args[1].u.pv_desc,
                                            pf_nfy,
                                            pt_nfy_tag,
                                            (HANDLE_T*)pt_args[4].u.pui4_arg);

    if(pt_return->u.i4_arg == SVCTXR_OK)
    {
        if (pt_nfy_tag)
        {
            ri_bind_cb_tag(*(HANDLE_T*)pt_args[4].u.pui4_arg, pt_nfy_tag);
        }

        if (pt_nfy_tag_push)
        {
            ri_bind_cb_tag(*(HANDLE_T*)pt_args[4].u.pui4_arg, pt_nfy_tag_push);
        }

        if (pt_nfy_tag_pull)
        {
            ri_bind_cb_tag(*(HANDLE_T*)pt_args[4].u.pui4_arg, pt_nfy_tag_pull);
        }
    }

    return RPCR_OK;
}


static INT32 _hndlr_x_svctx_media_close(
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
    pt_return->u.i4_arg = x_svctx_media_close(pt_args[0].u.ui4_arg,
                                              pt_args[1].u.ui4_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_media_get_info(
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

    if(pt_args[2].u.ui4_arg == SVCTX_MM_GET_TYPE_META_DATA)
    {
        MINFO_INFO_T*   pt_info = NULL;
        
        pt_info =       pt_args[3].u.pv_desc;
        
        if(pt_info->e_info_type == MINFO_INFO_TYPE_ID3_DATA)
        {
            if((pt_info->u.t_id3_data.pui1_img_data_buf != NULL) && 
               (pt_info->u.t_id3_data.ui4_img_buf_len != 0))
            {
#if 0
                INT32   shmid   = 0;
                char*   pc_shm  = NULL;
                if((shmid = shmget ((key_t)(pt_info->u.t_id3_data.pui1_img_data_buf), 
                                    pt_info->u.t_id3_data.ui4_img_buf_len, 
                                    0666)) < 0)
                {
                    printf("\r\n%s(), line %d shmget fail !\r\n", __func__, __LINE__);
                }

                if((pc_shm = shmat(shmid, NULL, 0)) == (char*) -1)
                {
                    printf("\r\n%s(), line %d shmat fail !\r\n", __func__, __LINE__);
                }
                pt_info->u.t_id3_data.pui1_img_data_buf = (UINT8*)pc_shm;
#else
                VOID* pv_vir_addr = NULL;
                pv_vir_addr = x_mmap_share_mem((VOID*)(pt_info->u.t_id3_data.pui1_img_data_buf), 
                                                pt_info->u.t_id3_data.ui4_img_buf_len);
                if(pv_vir_addr == NULL)
                {
                    printf("\r\n%s(), line %d x_mmap_share_mem fail !\r\n", __func__, __LINE__);
                    return RPCR_OUTOFMEMORY;
                }
                else
                {
                    pt_info->u.t_id3_data.pui1_img_data_buf = (UINT8*)pv_vir_addr;
                }
#endif
            }
        }
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_media_get_info(pt_args[0].u.ui4_arg,
                                                  pt_args[1].u.ui4_arg,
                                                  pt_args[2].u.ui4_arg,
                                                  pt_args[3].u.pv_desc,
                                                  pt_args[4].u.z_arg);
    
    return RPCR_OK;
}


static INT32 _hndlr_x_svctx_media_set_info(
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
    pt_return->u.i4_arg = x_svctx_media_set_info(pt_args[0].u.ui4_arg,
                                                 pt_args[1].u.ui4_arg,
                                                 pt_args[2].u.ui4_arg,
                                                 pt_args[3].u.pv_desc,
                                                 pt_args[4].u.z_arg);
    
    return RPCR_OK;
}

static INT32 _hndlr_x_svctx_generic_get_info(
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
    pt_return->u.i4_arg = x_svctx_generic_get_info(pt_args[0].u.ui4_arg,
                                                  pt_args[1].u.ui4_arg,
                                                  pt_args[2].u.ui4_arg,
                                                  pt_args[3].u.pv_desc,
                                                  pt_args[4].u.z_arg);
    
    return RPCR_OK;
}

static BOOL _divx_drm_memory_read_func(UINT8* pui1_divx_drm_mem)
{    
    RPC_DECL(1, BOOL);
    
    RPC_ARG_IO (ARG_TYPE_REF_DESC,   pui1_divx_drm_mem);
    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pui1_divx_drm_mem, 56));

    if (NULL == t_divx_drm_cb.pfn_divx_drm_memory_read_func)
    {
        return FALSE;
    }

    RPC_DO_CB(t_divx_drm_cb.t_rpc_id, "divx_drm_memory_read_func", t_divx_drm_cb.pfn_divx_drm_memory_read_func);

    RPC_RETURN(ARG_TYPE_BOOL, FALSE);
}

static BOOL _divx_drm_memory_write_func(UINT8* pui1_divx_drm_mem)
{
    RPC_DECL(1, BOOL);
    
    RPC_ARG_IO (ARG_TYPE_REF_DESC,   pui1_divx_drm_mem);
    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pui1_divx_drm_mem, 56));

    if (NULL == t_divx_drm_cb.pfn_divx_drm_memory_write_func)
    {
        return FALSE;
    }

    RPC_DO_CB(t_divx_drm_cb.t_rpc_id, "divx_drm_memory_write_func", t_divx_drm_cb.pfn_divx_drm_memory_write_func);

    RPC_RETURN(ARG_TYPE_BOOL, FALSE);
}

static INT32 _hndlr_x_svctx_generic_set_info(
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

    if ((SVCTX_GET_TYPE_T)pt_args[2].u.ui4_arg == SVCTX_GENGRIC_SET_TYPE_DIVX_DRM_MEMORY_RW_FUNC)
    {
        DIVX_DRM_UNION_GENERIC_INFO_T* pt_generic;
        pt_generic = (DIVX_DRM_UNION_GENERIC_INFO_T*)pt_args[3].u.pv_desc;
        
        t_divx_drm_cb.t_rpc_id = t_rpc_id;
        t_divx_drm_cb.pfn_divx_drm_memory_read_func = pt_generic->t_divx_drm_memory_rw_func_info.pfn_divx_drm_memory_read_func;
        t_divx_drm_cb.pfn_divx_drm_memory_write_func = pt_generic->t_divx_drm_memory_rw_func_info.pfn_divx_drm_memory_write_func;
        
        pt_generic->t_divx_drm_memory_rw_func_info.pfn_divx_drm_memory_read_func = _divx_drm_memory_read_func;
        pt_generic->t_divx_drm_memory_rw_func_info.pfn_divx_drm_memory_write_func = _divx_drm_memory_write_func;
    }

    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_generic_set_info(pt_args[0].u.ui4_arg,
                                                  pt_args[1].u.ui4_arg,
                                                  pt_args[2].u.ui4_arg,
                                                  pt_args[3].u.pv_desc,
                                                  pt_args[4].u.z_arg);
    
    return RPCR_OK;
}


static VOID _x_svctx_select_nfy_fct_wrapper(
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


static INT32 _hndlr_x_svctx_media_seek(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_op,    
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    RPC_CB_NFY_TAG_T*           pt_nfy_tag;
    x_svctx_select_nfy_fct      pf_nfy;
    
    if(ui4_num_args != 4)
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
        pf_nfy      = _x_svctx_select_nfy_fct_wrapper;
        pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, &pt_args[1].u.pv_func, 1, pt_args[2].u.pv_arg);
    }
   
    pt_return->e_type = (ARG_TYPE_T)ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_media_seek(pt_args[0].u.ui4_arg,
                                            (x_svctx_select_nfy_fct)pf_nfy,
                                            pt_nfy_tag,
                                            (MM_SVC_POS_INFO_T*)pt_args[3].u.pv_desc);
    return RPCR_OK;
}


static INT32 _hndlr_x_svctx_media_load_thumbnail(
                        RPC_ID_T     t_rpc_id,
                        const CHAR*  ps_op,    
                        UINT32       ui4_num_args,
                        ARG_DESC_T*  pt_args,
                        ARG_DESC_T*  pt_return)
{
    SVCTX_VIDEO_THUMBNAIL_INFO_T * pt_thumb_cpy = NULL;
    SVCTX_VIDEO_THUMBNAIL_INFO_T * pt_thumb_tmp = NULL;
    RPC_CB_NFY_TAG_T *             pt_nfy_tag   = NULL;
    
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }
    
    pt_thumb_tmp = (SVCTX_VIDEO_THUMBNAIL_INFO_T*)(pt_args[2].u.pv_desc);
    
    if(pt_args[2].u.pv_desc != NULL)
    {
        pt_thumb_cpy = malloc(sizeof(SVCTX_VIDEO_THUMBNAIL_INFO_T));
        if (pt_thumb_cpy == NULL)
        {
            return RPCR_OUTOFMEMORY;
        }
        memcpy(pt_thumb_cpy, pt_args[2].u.pv_desc, sizeof(SVCTX_VIDEO_THUMBNAIL_INFO_T));
        if(pt_thumb_tmp->pf_nfy == NULL)
        {
            pt_thumb_cpy->pf_nfy = NULL;
            pt_thumb_cpy->pv_tag = pt_thumb_tmp->pv_tag;     
        }
        else
        {
            pt_nfy_tag  = ri_create_cb_tag(t_rpc_id, (VOID*)&pt_thumb_tmp->pf_nfy, 1, pt_thumb_tmp->pv_tag);
            pt_thumb_cpy->pf_nfy = _x_svctx_select_nfy_fct_wrapper;
            pt_thumb_cpy->pv_tag = pt_nfy_tag;     
        }   
    }
    
    pt_return->e_type = ARG_TYPE_INT32;    
    pt_return->u.i4_arg = x_svctx_media_load_thumbnail(pt_args[0].u.ui4_arg,
                                                      pt_args[1].u.ui4_arg,
                                                      pt_thumb_cpy);
    if(pt_return->u.i4_arg == SVCTXR_OK)
    {
        ri_bind_cb_tag((HANDLE_T)pt_args[0].u.ui4_arg, pt_nfy_tag);
    }
    
    if(pt_thumb_cpy != NULL){free(pt_thumb_cpy);}
    return RPCR_OK;
}


static INT32 _hndlr_x_svctx_media_sync_cancel_load_thumbnail(
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
    pt_return->u.i4_arg = x_svctx_media_sync_cancel_load_thumbnail(
                                                    pt_args[0].u.ui4_arg,
                                                    pt_args[1].u.ui4_arg);
    
    return RPCR_OK;
}

INT32 c_rpc_reg_mm_svctx_op_hndlrs()
{
    RPC_REG_OP_HNDLR(x_svctx_media_open);
    RPC_REG_OP_HNDLR(x_svctx_media_close);
    RPC_REG_OP_HNDLR(x_svctx_media_get_info);
    RPC_REG_OP_HNDLR(x_svctx_media_set_info);
    RPC_REG_OP_HNDLR(x_svctx_generic_get_info);   
    RPC_REG_OP_HNDLR(x_svctx_generic_set_info);
    RPC_REG_OP_HNDLR(x_svctx_media_seek);
    RPC_REG_OP_HNDLR(x_svctx_media_load_thumbnail);
    RPC_REG_OP_HNDLR(x_svctx_media_sync_cancel_load_thumbnail);
    RPC_REG_OP_HNDLR(x_mm_playback_pull_nfy_fct);
    return RPCR_OK;
}



