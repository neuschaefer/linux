/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *  SYS_RECORD_SUPPORT                                                                          *
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
 * $RCSfile: mm_svctx.c,v $
 * $Revision: #11 $
 * $Date: 2012/09/11 $
 * $Author: jifeng.fan $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/26 $
 * $SWAuthor: Weider Chang $
 * $MD5HEX: 9cacb426bc6af60b7f3e5a1ad05af25e $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#ifdef ENABLE_MULTIMEDIA
#include <stdio.h>
#include "os/inc/x_os.h"
#include "inc/common.h"
#include "handle/handle.h"
#include "svctx/svctx.h"
#include "svctx/svctx_dbg.h"
#include "mutil/mfmtrecg/x_mfmtrecg.h"
#include "mutil/minfo/x_minfo.h"
#include "mutil/sbtl_engine/u_mm_sbtl_engine.h"
#include "mm_hdlr/mm_svc_req.h"
#include "mm_hdlr/mm_hdlr.h"
#include "conn_mngr/cm_playback/x_playback_handler.h"
#ifdef ENABLE_DIVX_DRM
#include "drm/divx_drm/x_divx_drm.h"
#endif
#include "graphic/x_gl.h"
#include "file_mngr/x_fm_mfw.h"
#include "font/x_fe.h"  /*add for font info*/

#include "mutil/minfo/u_minfo.h"
#include "mutil/mm_util.h"
#include "inet/http/x_httpc_api.h"
#include "file_mngr/x_fm.h"
#include "file_mngr/x_fm_mfw.h"
#include "mutil/mm_util_input.h"
#include "strm_mngr/sess_gnrc_hdlr/u_sm_sess_gnrc.h"
#ifdef ENABLE_MMS_SUPPORT
#include "inc/u_common.h"
#include "inet/mms/include/libmms/mms_streaming.h"
#endif

#ifdef DLNA_SUPPORT
#include "file_mngr/dlnafs/x_fm_dlnafs.h"
#endif

#undef  _INFO_HEADER
#undef  _API_HEADER
#undef  _ERROR_HEADER
#define _INFO_HEADER        "\n{SVCTX:MM_SVCTX}Info: "
#define _API_HEADER         "\n{SVCTX:MM_SVCTX}API:  "
#define _ERROR_HEADER       "\n{SVCTX:MM_SVCTX}Err:  "

 /*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#define MAX_EXTENSION_NAME_LEN 16
#define dwBigFourCC(a,b,c,d)    ((((UINT32)a)<<24)|((b)<<16)|((c)<<8)|(d))
/*#define SINGLE_URI_HANDLE*/
 /*-----------------------------------------------------------------------------
 * SVCTX private methods declarations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
 /*-----------------------------------------------------------------------------
 * SVCTX private methods implementations
 *---------------------------------------------------------------------------*/
static INT32 _svctx_create_uri_media_feeder_instace(MEDIA_DESCRIBT_T* pt_media_desc);
static INT32 _svctx_close_media_feeder_instace(MEDIA_DESCRIBT_T* pt_media_desc);
static INT32 _svctx_create_pull_media_feeder_instace(MEDIA_DESCRIBT_T* pt_media_desc);
static INT32 _svctx_get_favor_format_by_ext(MEDIA_DESCRIBT_T* pt_media_desc,
                                            MEDIA_FORMAT_T*   pt_format);
static INT32 _svctx_set_meta_info(MEDIA_DESCRIBT_T* pt_media_desc,
                                  MINFO_INFO_T*     pt_meta_info);
static VOID _svctx_free_meta_info(MEDIA_DESCRIBT_T* pt_media_desc);
static INT32 _svctx_handle_media_cmd(
                    MM_SVC_COMMAND_T*               pt_cmd,
                    MEDIA_DESCRIBT_T*               pt_media_desc
                    );
static INT32 _svctx_handle_lyric_info(
                    MEDIA_DESCRIBT_T*               pt_media_desc
                    );
static INT32 _svctx_handle_extra_subtl(
                    HANDLE_T                        h_svctx,
                    MEDIA_DESCRIBT_T*               pt_media_desc
                    );
static INT32 _svctx_media_descrp_free(MEDIA_DESCRIBT_T* pt_media_desc);

#ifdef ENABLE_MMS_SUPPORT
static INT32 _svctx_get_mms_header(CHAR* pc_mms_file, CHAR** pc_header_buf, 
                                         MMS_HANDLE_T* ph_mms_handle, UINT32* pui4_hdr_sz);
static INT32 _svctx_release_mms_header(MMS_HANDLE_T h_file, CHAR* pc_header_buf);
#endif
#ifdef MM_LAST_MEMORY_SUPPORT
#define MAX_MUTI_CLIP_LOOPS_CNT  64
static INT32 _svctx_get_mm_identifier(MEDIA_DESCRIBT_T*     pt_media_desc);
static INT32 _svctx_get_file_identifier(const CHAR*     ps_path,
                                          VOID*            pv_get_info,
                                          SIZE_T           z_info_size);
static INT32 _svctx_get_mem_identifier(const VOID*      pv_mem_data, 
                                            SIZE_T           z_mem_leng,
                                            VOID*            pv_get_info,
                                            SIZE_T           z_info_size);
static INT32 _svctx_cvt_identifier(UINT64*         pui8_id,
                                     ID_CVT_CNDTN_T* pv_cndtn);
#endif

 /*-----------------------------------------------------------------------------
 * Name
 *      _svctx_media_handle_free_fct
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *      TRUE
 *      FALSE
 *-----------------------------------------------------------------------------*/
static BOOL _svctx_media_handle_free_fct(
                    HANDLE_T                    h_handle,
                    HANDLE_TYPE_T               e_type,
                    VOID*                       pv_obj,
                    VOID*                       pv_tag,
                    BOOL                        b_req_handle
                    )
{
    if(e_type != HT_MEDIA_STORGE_ELM_TYPE)
    {
        return FALSE;
    }
    x_mem_free(pv_obj);
    pv_obj = NULL;
    
    return TRUE;
}


/*-----------------------------------------------------------------------------
 * Name
 *      _mm_hdlr_minfo_nty
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *      TRUE
 *      FALSE
 *-----------------------------------------------------------------------------*/
static INT32 _svctx_media_minfo_nty(
                    HANDLE_T                    h_minfo,
                    MINFO_NFY_EVENT_TYPE_T      e_event,
                    const VOID*                 pv_data1,
                    const VOID*                 pv_data2,
                    const VOID*                 pv_tag)
{
    HANDLE_T h_media_desc = (HANDLE_T)pv_tag;
    MEDIA_DESCRIBT_T* pt_media_desc;
    INT32    i4_ret;
    SVCTX_NTFY_CODE_T e_code = SVCTX_NTFY_CODE_MEDIA_PRE_PROCESSED;
    i4_ret = _mm_hdlr_get_media_obj(h_media_desc,&pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }

    if(e_event == MINFO_NFY_EVENT_PARSING_DONE)
    {
        DBG_INFO((_INFO_HEADER"minfo parsing file is done\n"));       
        e_code = SVCTX_NTFY_CODE_MEDIA_PRE_PROCESSED;
    }
    else if(e_event == MINFO_NFY_EVENT_ERROR)
    {
        x_dbg_stmt(_INFO_HEADER"minfo parsing file is error\n"); 
        e_code = SVCTX_NTFY_CODE_MEDIA_ERROR;
    }
    else
    {
        e_code = e_code;
    }

    if (e_event != MINFO_NFY_EVENT_HANDLE_CLOSED)
    {
        pt_media_desc->pf_nfy(pt_media_desc->h_svctx,SVCTX_COND_PENDING,
                              e_code,ST_UNKNOWN,
                              pt_media_desc->pv_tag,
                              (VOID*)h_media_desc);

    }
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_media_load_thumbnail_select_nfy_fct
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
#ifndef ANDROID
static VOID _svctx_media_load_thumbnail_select_nfy_fct(
                    HANDLE_T                        h_svctx,
                    SVCTX_COND_T                    e_nfy_cond,
                    SVCTX_NTFY_CODE_T               e_code,
                    STREAM_TYPE_T                   e_stream_type,
                    VOID*                           pv_nfy_tag
                    )
{
    SVCTX_T*            pt_svctx            = NULL;
    MM_SVC_REQ_T*       pt_running_svc_req  = NULL;
    STRM_REQ_T*         pt_video_strm_req   = NULL;
    HANDLE_T            h_media             = NULL_HANDLE;
    INT32               i4_ret              = 0;
    MEDIA_DESCRIBT_T*   pt_media_desc       = NULL;
    SIZE_T              z_info_len          = 0;


    i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
    if (i4_ret != SVCTXR_OK)
    {
        DBG_ERROR((_ERROR_HEADER
                   "svctx_get_obj(h_svctx=%d) failed!\r\n"
                   "i4_ret=%d, pt_svctx=%d\r\n",
                   h_svctx, i4_ret, pt_svctx));
        ASSERT(FALSE, ("%s(), pt_svctx is NULL\r\n", __func__));
    }
    pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    pt_video_strm_req = &(pt_running_svc_req->t_strm_reqs[ST_VIDEO]);

    h_media = pt_running_svc_req->h_media_desc;
    i4_ret = _mm_hdlr_get_media_obj(h_media,&pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        return ;
    }

    if((SVCTX_COND_PRESENTING                     == e_nfy_cond)
         &&
       (SVCTX_NTFY_CODE_MEDIA_EOS                 == e_code))
    {
        GL_LOCK_INFO_T t_lock_info;
        VSH_THUMBNAIL_INFO_T t_video_thumbnail_info;

        if(pt_media_desc->t_video_thumbnail_info.h_surf != NULL_HANDLE)
        {
        i4_ret = x_gl_surface_lock(pt_media_desc->t_video_thumbnail_info.h_surf,
                                   NULL,
                                   GL_SYNC,
                                   &t_lock_info);
        if (i4_ret != GLR_OK)
        {
            if(pt_media_desc->t_video_thumbnail_info.pf_nfy)
            {
                pt_media_desc->t_video_thumbnail_info.pf_nfy(h_svctx,
                                                         e_nfy_cond,
                                                         SVCTX_NTFY_CODE_MEDIA_ERROR,
                                                         e_stream_type,
                                                         pt_media_desc->t_video_thumbnail_info.pv_tag);
            }
            return;
        }

        switch (t_lock_info.e_clrmode)
        {
        case GL_COLORMODE_AYUV_CLUT2:
            t_video_thumbnail_info.e_canvas_colormode =
                VSH_COLORMODE_AYUV_CLUT2;
            break;
        case GL_COLORMODE_AYUV_CLUT4:
            t_video_thumbnail_info.e_canvas_colormode =
                VSH_COLORMODE_AYUV_CLUT4;
            break;
        case GL_COLORMODE_AYUV_CLUT8:
            t_video_thumbnail_info.e_canvas_colormode =
                VSH_COLORMODE_AYUV_CLUT8;
            break;
        case GL_COLORMODE_UYVY_16:
            t_video_thumbnail_info.e_canvas_colormode =
                VSH_COLORMODE_UYVY_16;
            break;
        case GL_COLORMODE_YUYV_16:
            t_video_thumbnail_info.e_canvas_colormode =
                VSH_COLORMODE_YUYV_16;
            break;
        case GL_COLORMODE_AYUV_D8888:
            t_video_thumbnail_info.e_canvas_colormode =
                VSH_COLORMODE_AYUV_D8888;
            break;
        case GL_COLORMODE_ARGB_CLUT2:
            t_video_thumbnail_info.e_canvas_colormode =
                VSH_COLORMODE_ARGB_CLUT2;
            break;
        case GL_COLORMODE_ARGB_CLUT4:
            t_video_thumbnail_info.e_canvas_colormode =
                VSH_COLORMODE_ARGB_CLUT4;
            break;
        case GL_COLORMODE_ARGB_CLUT8:
            t_video_thumbnail_info.e_canvas_colormode =
                VSH_COLORMODE_ARGB_CLUT8;
            break;
        case GL_COLORMODE_RGB_D565:
            t_video_thumbnail_info.e_canvas_colormode =
                VSH_COLORMODE_RGB_D565;
            break;
        case GL_COLORMODE_ARGB_D1555:
            t_video_thumbnail_info.e_canvas_colormode =
                VSH_COLORMODE_ARGB_D1555;
            break;
        case GL_COLORMODE_ARGB_D4444:
            t_video_thumbnail_info.e_canvas_colormode =
                VSH_COLORMODE_ARGB_D4444;
            break;
        case GL_COLORMODE_ARGB_D8888:
            t_video_thumbnail_info.e_canvas_colormode =
                VSH_COLORMODE_ARGB_D8888;
            break;
        case GL_COLORMODE_YUV_420_BLK:
            t_video_thumbnail_info.e_canvas_colormode =
                VSH_COLORMODE_YUV_420_BLK;
            break;
        case GL_COLORMODE_YUV_420_RS:
            t_video_thumbnail_info.e_canvas_colormode =
                VSH_COLORMODE_YUV_420_RS;
            break;
        case GL_COLORMODE_YUV_422_BLK:
            t_video_thumbnail_info.e_canvas_colormode =
                VSH_COLORMODE_YUV_422_BLK;
            break;
        case GL_COLORMODE_YUV_422_RS:
            t_video_thumbnail_info.e_canvas_colormode =
                VSH_COLORMODE_YUV_422_RS;
            break;
        case GL_COLORMODE_YUV_444_BLK:
            t_video_thumbnail_info.e_canvas_colormode =
                VSH_COLORMODE_YUV_444_BLK;
            break;
        case GL_COLORMODE_YUV_444_RS:
            t_video_thumbnail_info.e_canvas_colormode =
                VSH_COLORMODE_YUV_444_RS;
            break;
        default:
            if(pt_media_desc->t_video_thumbnail_info.pf_nfy)
            {
                pt_media_desc->t_video_thumbnail_info.pf_nfy(h_svctx,
                                                         e_nfy_cond,
                                                         SVCTX_NTFY_CODE_MEDIA_ERROR,
                                                         e_stream_type,
                                                         pt_media_desc->t_video_thumbnail_info.pv_tag);
            }
            return;
        }
        t_video_thumbnail_info.pc_canvas_buffer =
            (UCHAR*)t_lock_info.at_subplane[0].pv_bits;
        t_video_thumbnail_info.ui4_canvas_width =
            t_lock_info.at_subplane[0].ui4_width;
        t_video_thumbnail_info.ui4_canvas_height =
            t_lock_info.at_subplane[0].ui4_height;
        t_video_thumbnail_info.ui4_canvas_pitch =
            t_lock_info.at_subplane[0].ui4_pitch;
        t_video_thumbnail_info.ui4_thumbnail_x =
            pt_media_desc->t_video_thumbnail_info.ui4_thumbnail_x;
        t_video_thumbnail_info.ui4_thumbnail_y =
            pt_media_desc->t_video_thumbnail_info.ui4_thumbnail_y;
        t_video_thumbnail_info.ui4_thumbnail_width =
            pt_media_desc->t_video_thumbnail_info.ui4_thumbnail_width;
        t_video_thumbnail_info.ui4_thumbnail_height =
            pt_media_desc->t_video_thumbnail_info.ui4_thumbnail_height;

        /* get video thumbnail */
        z_info_len = sizeof(VSH_THUMBNAIL_INFO_T);
        i4_ret = x_sm_get(pt_video_strm_req->h_stream,
                          SM_VSH_GET_TYPE_THUMBNAIL,
                          (VOID*)(&t_video_thumbnail_info),
                          &z_info_len);
        if(i4_ret != SMR_OK)
        {
            if(pt_media_desc->t_video_thumbnail_info.pf_nfy)
            {
                pt_media_desc->t_video_thumbnail_info.pf_nfy(h_svctx,
                                                         e_nfy_cond,
                                                         SVCTX_NTFY_CODE_MEDIA_ERROR,
                                                         e_stream_type,
                                                         pt_media_desc->t_video_thumbnail_info.pv_tag);
            }
            return;
        }
    }
    }
    else if((SVCTX_COND_PENDING                        == e_nfy_cond)
              &&
            (SVCTX_NTFY_CODE_MEDIA_THUMBNAIL_DONE      == e_code))
    {
        pt_media_desc->b_video_thumbnail_done = TRUE;

        if(pt_media_desc->t_video_thumbnail_info.h_surf != NULL_HANDLE)
        {
        i4_ret = x_gl_surface_unlock(pt_media_desc->t_video_thumbnail_info.h_surf, NULL);
        if (i4_ret != GLR_OK)
        {
            if(pt_media_desc->t_video_thumbnail_info.pf_nfy)
            {
                pt_media_desc->t_video_thumbnail_info.pf_nfy(h_svctx,
                                                         e_nfy_cond,
                                                         SVCTX_NTFY_CODE_MEDIA_ERROR,
                                                         e_stream_type,
                                                         pt_media_desc->t_video_thumbnail_info.pv_tag);
            }
        }
        }
        return;
    }
    else if((SVCTX_COND_READY                     == e_nfy_cond)
              &&
            (SVCTX_NTFY_CODE_STOPPED              == e_code))
    {
        if(pt_media_desc->t_video_thumbnail_info.h_surf != NULL_HANDLE)
        {
        i4_ret = x_gl_surface_unlock(pt_media_desc->t_video_thumbnail_info.h_surf, NULL);
        if ((i4_ret != GLR_OK) && (i4_ret != GLR_SURFACE_NOT_LOCKED))
        {
            if(pt_media_desc->t_video_thumbnail_info.pf_nfy)
            {
                pt_media_desc->t_video_thumbnail_info.pf_nfy(h_svctx,
                                                         e_nfy_cond,
                                                         SVCTX_NTFY_CODE_MEDIA_ERROR,
                                                         e_stream_type,
                                                         pt_media_desc->t_video_thumbnail_info.pv_tag);
            }
        }
        }
        if (pt_media_desc->b_video_thumbnail_done)
        {
            e_code = SVCTX_NTFY_CODE_MEDIA_THUMBNAIL_DONE;
            pt_media_desc->t_video_thumbnail_info.h_surf = NULL_HANDLE;
        }
    }

    if(pt_media_desc->t_video_thumbnail_info.pf_nfy)
    {
        pt_media_desc->t_video_thumbnail_info.pf_nfy(h_svctx,
                                                 e_nfy_cond,
                                                 e_code,
                                                 e_stream_type,
                                                 pt_media_desc->t_video_thumbnail_info.pv_tag);
    }
}
#else
static VOID _svctx_media_load_thumbnail_select_nfy_fct(
                    HANDLE_T                        h_svctx,
                    SVCTX_COND_T                    e_nfy_cond,
                    SVCTX_NTFY_CODE_T               e_code,
                    STREAM_TYPE_T                   e_stream_type,
                    VOID*                           pv_nfy_tag
                    )
{
    SVCTX_T*            pt_svctx            = NULL;
    MM_SVC_REQ_T*       pt_running_svc_req  = NULL;
    HANDLE_T            h_media             = NULL_HANDLE;
    INT32               i4_ret              = 0;
    MEDIA_DESCRIBT_T*   pt_media_desc       = NULL;


    i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
    if (i4_ret != SVCTXR_OK)
    {
        DBG_ERROR((_ERROR_HEADER
                   "svctx_get_obj(h_svctx=%d) failed!\r\n"
                   "i4_ret=%d, pt_svctx=%d\r\n",
                   h_svctx, i4_ret, pt_svctx));
        ASSERT(FALSE, ("%s(), pt_svctx is NULL\r\n", __func__));
    }
    pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

    h_media = pt_running_svc_req->h_media_desc;
    i4_ret = _mm_hdlr_get_media_obj(h_media,&pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        return ;
    }

    if((SVCTX_COND_PRESENTING                     == e_nfy_cond)
         &&
       (SVCTX_NTFY_CODE_MEDIA_EOS                 == e_code))
    {
        /* do nothing*/
    }
    else if((SVCTX_COND_PENDING                        == e_nfy_cond)
              &&
            (SVCTX_NTFY_CODE_MEDIA_THUMBNAIL_DONE      == e_code))
    {
        pt_media_desc->b_video_thumbnail_done = TRUE;
        return;
    }
    else if((SVCTX_COND_READY                     == e_nfy_cond)
              &&
            (SVCTX_NTFY_CODE_STOPPED              == e_code))
    {
        if (pt_media_desc->b_video_thumbnail_done)
        {
            e_code = SVCTX_NTFY_CODE_MEDIA_THUMBNAIL_DONE;
            pt_media_desc->t_video_thumbnail_info.h_surf = NULL_HANDLE;
        }
    }

    if(pt_media_desc->t_video_thumbnail_info.pf_nfy)
    {
        pt_media_desc->t_video_thumbnail_info.pf_nfy(h_svctx,
                                                 e_nfy_cond,
                                                 e_code,
                                                 e_stream_type,
                                                 pt_media_desc->t_video_thumbnail_info.pv_tag);
    }
}
#endif

/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_media_strm_comp_filter_fct
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
static BOOL _svctx_media_strm_comp_filter_fct(
                    UINT16                          ui2_num_recs,
                    UINT16                          ui2_idx,
                    const STREAM_COMP_ID_T*         pt_comp_id,
                    const SCDB_REC_T*               pt_scdb_rec,
                    VOID*                           pv_fltr_tag
                    )
{
    if(NULL == pt_comp_id
        ||
       NULL == pt_scdb_rec)
    {
        ASSERT(pt_comp_id != NULL, ("%s(), pt_comp_id is NULL\r\n", __func__));
        ASSERT(pt_scdb_rec != NULL, ("%s(), pt_scdb_rec is NULL\r\n", __func__));
        return FALSE;
    }

    switch(pt_scdb_rec->e_rec_type)
    {
        case SCDB_REC_TYPE_AUDIO_MPEG    :
        case SCDB_REC_TYPE_AUDIO_ANALOG  :
        case SCDB_REC_TYPE_AUDIO_DIGITAL :
        case SCDB_REC_TYPE_AUDIO_AVI     :
        case SCDB_REC_TYPE_AUDIO_MP4     :
        case SCDB_REC_TYPE_AUDIO_ASF     :
        case SCDB_REC_TYPE_AUDIO_PS      :
        case SCDB_REC_TYPE_AUDIO_MKV     :
        case SCDB_REC_TYPE_AUDIO_RM      :
        case SCDB_REC_TYPE_AUDIO_ES      :
            /*add for flv*/
        case SCDB_REC_TYPE_AUDIO_FLV     :            
        /*add for ogg*/
    case SCDB_REC_TYPE_AUDIO_OGG     :
        case SCDB_REC_TYPE_VIDEO_MPEG    :
        case SCDB_REC_TYPE_VIDEO_ANALOG  :
        case SCDB_REC_TYPE_VIDEO_AVI     :
        case SCDB_REC_TYPE_VIDEO_MP4     :
        case SCDB_REC_TYPE_SUBTITLE_MP4  :
        case SCDB_REC_TYPE_VIDEO_ASF     :
        case SCDB_REC_TYPE_VIDEO_PS      :
        case SCDB_REC_TYPE_VIDEO_MKV     :
        case SCDB_REC_TYPE_VIDEO_RM      :
        case SCDB_REC_TYPE_VIDEO_ES      :
            /*add for flv*/
        case SCDB_REC_TYPE_VIDEO_FLV     :
        /*add for ogg*/
        case SCDB_REC_TYPE_VIDEO_OGG     :
        {
            return TRUE;
        }
    }

    return FALSE;
}

static VOID _svctx_fake_minfo_cb(
    HANDLE_T    h_timer,
    VOID*       pv_tag
)
{
    HANDLE_T             h_media_desc = (HANDLE_T)pv_tag;
    MEDIA_DESCRIBT_T*    pt_media_desc;
    INT32                i4_ret;

    i4_ret = _mm_hdlr_get_media_obj(h_media_desc, &pt_media_desc);

    if (i4_ret != SVCTXR_OK)
    {
        return;
    }

    if (pt_media_desc->pf_nfy != NULL)
    {
        pt_media_desc->pf_nfy(pt_media_desc->h_svctx,
                              SVCTX_COND_PENDING,
                              SVCTX_NTFY_CODE_MEDIA_PRE_PROCESSED,
                              ST_UNKNOWN,
                              pt_media_desc->pv_tag,
                              (VOID*)h_media_desc);
    }
}

INT32 _svctx_get_ts_strm_info(SVCTX_T*            pt_svctx,
                              MM_STRM_INFO_T*     pt_strm_info)
{
    INT32            i4_ret;
    UINT8            ui1_strm_type_idx  = 0;
    UINT32           ui4_db_version     = SCDB_NULL_VER_ID;
    UINT16           ui2_num_recs       = 0;
    UINT16           ui2_idx            = 0;
    MM_SVC_REQ_T*    pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
    HANDLE_T         h_scdb             = NULL_HANDLE;
    SCDB_REC_T       t_scdb_rec;
    STREAM_COMP_ID_T t_comp_id;
    MINFO_TYPE_STM_ATRBT_T* pt_strm_atrbt = NULL;
    STREAM_TYPE_T   e_mm_strm_types[3] = {ST_AUDIO, ST_VIDEO, ST_SUBTITLE};
    
    if((pt_svctx == NULL) || (pt_strm_info == NULL))
    {
        return SVCTXR_INV_ARG;
    }
    ASSERT(pt_running_svc_req != NULL, 
           ("%s(), pt_running_svc_req is NULL\r\n", 
           __func__));
    
    h_scdb = player_get_scdb_handle(pt_running_svc_req->pt_player);
    if (x_handle_valid(h_scdb) == FALSE) 
    {
        return SVCTXR_WRONG_STATE;
    }
    
    for(ui1_strm_type_idx = 0; ui1_strm_type_idx < 3; ui1_strm_type_idx++)
    {
        i4_ret = x_scdb_get_num_recs(h_scdb,
                                     e_mm_strm_types[ui1_strm_type_idx],
                                     &ui2_num_recs,
                                     &ui4_db_version);
        if (i4_ret != SCDBR_OK) 
        {
            DBG_ERROR((_ERROR_HEADER"x_scdb_get_num_recs() failed, return = %d\r\n", i4_ret));
            return SVCTXR_FAIL;
        }

        /* Only Fill Current Playing One*/
        for (ui2_idx = 0; ui2_idx < ui2_num_recs; ui2_idx++) 
        {
            x_memset(&t_scdb_rec, 0, sizeof(SCDB_REC_T));
            i4_ret = x_scdb_get_rec_by_idx (h_scdb,
                                            e_mm_strm_types[ui1_strm_type_idx],
                                            ui2_idx,
                                            &t_comp_id,
                                            &t_scdb_rec,
                                            &ui4_db_version);
            pt_strm_atrbt = &(pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt);
            if (i4_ret == SCDBR_OK) 
            {/* Now only filled the member cmpb needed, Later other info need get from scdb */
                pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].e_type 
                        = e_mm_strm_types[ui1_strm_type_idx];
                pt_strm_info->ui2_num_stm++;
                
                if(e_mm_strm_types[ui1_strm_type_idx] == ST_AUDIO)
                {
                    pt_strm_atrbt->u.t_ts_audio_stm_attr.ui2_pid
                            = t_scdb_rec.u.t_audio_mpeg.ui2_pid;
                    pt_strm_atrbt->u.t_ts_audio_stm_attr.e_enc 
                            = mm_hdlr_aud_enc_convert_ash_to_minfo(pt_svctx->t_audio_info.e_aud_fmt);
                    pt_strm_atrbt->u.t_ts_audio_stm_attr.e_layer
                            = t_scdb_rec.u.t_audio_mpeg.e_layer;
                    pt_strm_atrbt->u.t_ts_audio_stm_attr.e_type
                            = t_scdb_rec.u.t_audio_mpeg.e_type;
                    pt_strm_atrbt->u.t_ts_audio_stm_attr.e_prof_lvl
                            = t_scdb_rec.u.t_audio_mpeg.e_prof_lvl;
                    pt_strm_atrbt->u.t_ts_audio_stm_attr.ui1_index
                            = t_scdb_rec.u.t_audio_mpeg.ui1_index;
                    pt_strm_atrbt->u.t_ts_audio_stm_attr.ui2_pmt_index
                            = t_scdb_rec.u.t_audio_mpeg.ui2_pmt_index;                    
                    pt_strm_atrbt->u.t_ts_audio_stm_attr.e_mix_type
                            = t_scdb_rec.u.t_audio_mpeg.e_mix_type;
                    pt_strm_atrbt->u.t_ts_audio_stm_attr.e_class
                            = t_scdb_rec.u.t_audio_mpeg.e_class;
                    pt_strm_atrbt->u.t_ts_audio_stm_attr.ui1_component_tag
                            = t_scdb_rec.u.t_audio_mpeg.ui1_component_tag;
                    pt_strm_atrbt->u.t_ts_audio_stm_attr.b_default
                            = t_scdb_rec.u.t_audio_mpeg.b_default;
                    x_memcpy(pt_strm_atrbt->u.t_ts_audio_stm_attr.s_lang, 
                             t_scdb_rec.u.t_audio_mpeg.s_lang,
                             ISO_639_LANG_LEN);                    
                    pt_strm_atrbt->u.t_ts_audio_stm_attr.ui2_channels
                            = pt_svctx->t_audio_info.e_channels;//TBD
                    pt_strm_atrbt->u.t_ts_audio_stm_attr.ui4_sample_rate
                            = pt_svctx->t_audio_info.ui4_sample_rate;
                    pt_strm_atrbt->u.t_ts_audio_stm_attr.ui4_data_rate
                            = pt_svctx->t_audio_info.ui4_data_rate;
                }
                else if(e_mm_strm_types[ui1_strm_type_idx] == ST_VIDEO)
                {
                    pt_strm_atrbt->u.t_ts_video_stm_attr.ui2_pid
                            = t_scdb_rec.u.t_video_mpeg.ui2_pid;
                    pt_strm_atrbt->u.t_ts_video_stm_attr.e_enc 
                            = mm_hdlr_vid_enc_convert_scdb_to_minfo(t_scdb_rec.u.t_video_mpeg.e_enc);
                    pt_strm_atrbt->u.t_ts_video_stm_attr.e_vid_fmt
                            = t_scdb_rec.u.t_video_mpeg.e_vid_fmt;
                    pt_strm_atrbt->u.t_ts_video_stm_attr.b_default
                            = t_scdb_rec.u.t_video_mpeg.b_default;
                    pt_strm_atrbt->u.t_ts_video_stm_attr.ui1_component_tag
                            = t_scdb_rec.u.t_video_mpeg.ui1_component_tag;
                    pt_strm_atrbt->u.t_ts_video_stm_attr.ui2_pmt_index
                            = t_scdb_rec.u.t_video_mpeg.ui2_pmt_index;   
                    x_memcpy(&(pt_strm_atrbt->u.t_ts_video_stm_attr.t_rd3d),
                             &(t_scdb_rec.u.t_video_mpeg.t_rd3d),
                             sizeof(RD3D_REG_DESC_T));
                    x_memcpy(&(pt_strm_atrbt->u.t_ts_video_stm_attr.t_mvc_ext),
                             &(t_scdb_rec.u.t_video_mpeg.t_mvc_ext),
                             sizeof(MVC_EXTENSION_DESC_T));
                    pt_strm_atrbt->u.t_ts_video_stm_attr.ui4_width 
                            = pt_svctx->t_video_info.ui4_width;
                    pt_strm_atrbt->u.t_ts_video_stm_attr.ui4_height
                            = pt_svctx->t_video_info.ui4_height;
                }
                else if(e_mm_strm_types[ui1_strm_type_idx] == ST_SUBTITLE)
                {
                    pt_strm_atrbt->u.t_ts_sp_stm_attr.ui2_pid
                            = t_scdb_rec.u.t_dvb_sbtl.ui2_pid;
                    pt_strm_atrbt->u.t_ts_sp_stm_attr.ui2_comp_pg_id
                            = t_scdb_rec.u.t_dvb_sbtl.ui2_comp_pg_id;
                    pt_strm_atrbt->u.t_ts_sp_stm_attr.ui2_anci_pg_id
                            = t_scdb_rec.u.t_dvb_sbtl.ui2_anci_pg_id;
                    pt_strm_atrbt->u.t_ts_sp_stm_attr.e_sbtl_type
                            = t_scdb_rec.u.t_dvb_sbtl.e_sbtl_type;
                    pt_strm_atrbt->u.t_ts_sp_stm_attr.ui2_pmt_index
                            = t_scdb_rec.u.t_dvb_sbtl.ui2_pmt_index;
                    x_memcpy(pt_strm_atrbt->u.t_ts_sp_stm_attr.s_lang,
                             t_scdb_rec.u.t_dvb_sbtl.s_lang,
                             ISO_639_LANG_LEN);
                }
            }
        }
    }
    return SVCTXR_OK;
}

static UINT8 _get_pixel_byte_by_clrmode(VSH_COLORMODE_T e_vsh_clrmode)
{
    UINT8  ui1_bytes_per_pixel = 0;
    switch(e_vsh_clrmode)
    {
    case VSH_COLORMODE_UYVY_16:
    case VSH_COLORMODE_YUYV_16:
    case VSH_COLORMODE_RGB_D565:
    case VSH_COLORMODE_ARGB_D1555:
    case VSH_COLORMODE_ARGB_D4444:
    case VSH_COLORMODE_YUV_422_BLK:
    case VSH_COLORMODE_YUV_422_RS: 
        ui1_bytes_per_pixel = 2; 
        break;
        
    case VSH_COLORMODE_AYUV_D8888:
    case VSH_COLORMODE_ARGB_D8888:
    case VSH_COLORMODE_YUV_444_BLK:
    case VSH_COLORMODE_YUV_444_RS:
        ui1_bytes_per_pixel = 4;
        break;
        
    //not used now, so do as 0.
    case VSH_COLORMODE_AYUV_CLUT2:
    case VSH_COLORMODE_AYUV_CLUT4:
    case VSH_COLORMODE_AYUV_CLUT8:
    case VSH_COLORMODE_ARGB_CLUT2:
    case VSH_COLORMODE_ARGB_CLUT4:
    case VSH_COLORMODE_ARGB_CLUT8:
    case VSH_COLORMODE_YUV_420_BLK:
    case VSH_COLORMODE_YUV_420_RS:
    default:
        ui1_bytes_per_pixel = 0;
        break;        
    }
    return ui1_bytes_per_pixel;
}

static VOID _fill_es_strm_atrbt_by_fmt(MM_STRM_INFO_T* pt_strm_info)
{
    if(pt_strm_info == NULL)
    {
        return;
    }
    
    if(pt_strm_info->t_format.e_media_type == MEDIA_TYPE_AUDIO_ES)
    {
        pt_strm_info->at_stm_atrbt[0].e_type = ST_AUDIO;
        switch(pt_strm_info->t_format.t_media_subtype.u.e_aud_subtype)
        {
        case MEDIA_AUD_SUBTYPE_MP2:
            pt_strm_info->at_stm_atrbt[0].t_stm_atrbt.u.t_es_audio_stm_attr.e_enc = MINFO_INFO_AUD_ENC_MPEG_1;
            pt_strm_info->at_stm_atrbt[0].t_stm_atrbt.u.t_es_audio_stm_attr.e_layer= 2;
            break;
        case MEDIA_AUD_SUBTYPE_MP3:
            pt_strm_info->at_stm_atrbt[0].t_stm_atrbt.u.t_es_audio_stm_attr.e_enc = MINFO_INFO_AUD_ENC_MPEG_1;
            pt_strm_info->at_stm_atrbt[0].t_stm_atrbt.u.t_es_audio_stm_attr.e_layer= 3;
            break;
        //case MEDIA_AUD_SUBTYPE_WMA:
        //    pt_strm_info->at_stm_atrbt[0].t_stm_atrbt.u.t_es_audio_stm_attr.e_enc = ;
        //    break;
        case MEDIA_AUD_SUBTYPE_LPCM:
            pt_strm_info->at_stm_atrbt[0].t_stm_atrbt.u.t_es_audio_stm_attr.e_enc = MINFO_INFO_AUD_ENC_PCM;
            break;
        case MEDIA_AUD_SUBTYPE_AAC:
            pt_strm_info->at_stm_atrbt[0].t_stm_atrbt.u.t_es_audio_stm_attr.e_enc = MINFO_INFO_AUD_ENC_AAC;
            break;
        case MEDIA_AUD_SUBTYPE_AMR:
            pt_strm_info->at_stm_atrbt[0].t_stm_atrbt.u.t_es_audio_stm_attr.e_enc = MINFO_INFO_AUD_ENC_AMR;
            break;
        case MEDIA_AUD_SUBTYPE_AWB:
            pt_strm_info->at_stm_atrbt[0].t_stm_atrbt.u.t_es_audio_stm_attr.e_enc = MINFO_INFO_AUD_ENC_AWB;
            break;
        //case MEDIA_AUD_SUBTYPE_WAVE:
        //    pt_strm_info->at_stm_atrbt[0].t_stm_atrbt.u.t_es_audio_stm_attr.e_enc = ;
        //    break;
        case MEDIA_AUD_SUBTYPE_AIF:
            pt_strm_info->at_stm_atrbt[0].t_stm_atrbt.u.t_es_audio_stm_attr.e_enc = MINFO_INFO_AUD_ENC_PCM;
            break;
        case MEDIA_AUD_SUBTYPE_AC3:
            pt_strm_info->at_stm_atrbt[0].t_stm_atrbt.u.t_es_audio_stm_attr.e_enc = MINFO_INFO_AUD_ENC_AC3;
            break;
        default:
            break;
        }
    }
    else if(pt_strm_info->t_format.e_media_type == MEDIA_TYPE_VIDEO_ES)
    {
        pt_strm_info->at_stm_atrbt[0].e_type = ST_VIDEO;
        switch(pt_strm_info->t_format.t_media_subtype.u.e_vid_subtype)
        {
        case MEDIA_VID_SUBTYPE_M1V:
            pt_strm_info->at_stm_atrbt[0].t_stm_atrbt.u.t_es_video_stm_attr.e_enc = MINFO_INFO_VID_ENC_MPEG_1;
            break;
        case MEDIA_VID_SUBTYPE_M2V:
            pt_strm_info->at_stm_atrbt[0].t_stm_atrbt.u.t_es_video_stm_attr.e_enc = MINFO_INFO_VID_ENC_MPEG_2;
            break;
        case MEDIA_VID_SUBTYPE_M4V:
            pt_strm_info->at_stm_atrbt[0].t_stm_atrbt.u.t_es_video_stm_attr.e_enc = MINFO_INFO_VID_ENC_MPEG_4;
            break;
        case MEDIA_VID_SUBTYPE_H264:
            pt_strm_info->at_stm_atrbt[0].t_stm_atrbt.u.t_es_video_stm_attr.e_enc = MINFO_INFO_VID_ENC_H264;
            break;
        case MEDIA_VID_SUBTYPE_VC1:
            pt_strm_info->at_stm_atrbt[0].t_stm_atrbt.u.t_es_video_stm_attr.e_enc = MINFO_INFO_VID_ENC_WVC1;
            break;
        case MEDIA_VID_SUBTYPE_AVS:
            pt_strm_info->at_stm_atrbt[0].t_stm_atrbt.u.t_es_video_stm_attr.e_enc = MINFO_INFO_VID_ENC_AVS;
            break;
        default:
            break;
        }
    }
}

#ifdef TIME_SHIFT_SUPPORT
static VOID _svctx_rec_util_fake_cb(
    HANDLE_T    h_timer,
    VOID*       pv_tag
)
{
    HANDLE_T              h_media_desc = (HANDLE_T)pv_tag;
    MEDIA_DESCRIBT_T*     pt_media_desc;
    REC_UTIL_SHARE_DATA_T t_rec_share_data;
    INT32                 i4_ret;

    i4_ret = _mm_hdlr_get_media_obj(h_media_desc, &pt_media_desc);

    if (i4_ret != SVCTXR_OK)
    {
        return;
    }
    

    i4_ret = rec_util_get_shared_data(pt_media_desc->h_rec_util,
                                      &t_rec_share_data);
    if (i4_ret != REC_UTILR_OK)
    {
        ASSERT(FALSE, ("%s(), rec_util_get_shared_data() failed!\r\n", __func__));
    }
    pt_media_desc->t_rec_tick_ctrl_blk = t_rec_share_data.t_tick_cb;

    pt_media_desc->ps_media_path = (CHAR*)x_mem_alloc(x_strlen(t_rec_share_data.ps_strg_path) + 1);
    if (pt_media_desc->ps_media_path == NULL)
    {
        ASSERT(FALSE,("very very dead !"));
    }
    
    x_strcpy(pt_media_desc->ps_media_path, t_rec_share_data.ps_strg_path);
    
    pt_media_desc->pf_nfy(pt_media_desc->h_svctx,
                          SVCTX_COND_PENDING,
                          SVCTX_NTFY_CODE_MEDIA_PRE_PROCESSED,
                          ST_UNKNOWN,
                          pt_media_desc->pv_tag,
                          (VOID*)pt_media_desc->h_media_desc);
}

static BOOL _svctx_media_rec_util_nfy_fct (
                    HANDLE_T                        h_rec_util,
                    VOID*                           pv_nfy_tag,
                    REC_UTIL_NTFY_CODE_T            e_ntfy_code,
                    UINT32                          ui4_data
                    )
{
    SVCTX_MSG_T         t_msg   = {0};
    BOOL                b_ret   = TRUE;
    INT32               i4_ret = SVCTXR_OK;
    MEDIA_DESCRIBT_T*   pt_media_desc = (MEDIA_DESCRIBT_T*)pv_nfy_tag;
    SVCTX_T*            pt_svctx;
    REC_UTIL_COND_T     e_cond_new;

    ASSERT(pt_media_desc != NULL, ("%s(), pt_media_desc is NULL\r\n", __func__));
    i4_ret = svctx_get_obj(pt_media_desc->h_svctx, &pt_svctx);
    if (i4_ret != SVCTXR_OK)
    {
        ASSERT(FALSE, ("%s(), pt_svctx is NULL\r\n", __func__));
    }

    t_msg.ui1_msg_id       = _SVCTX_MSG_REC_UTIL_NTFY;
    t_msg.u.t_rec.h_obj    = h_rec_util;
    t_msg.u.t_rec.e_code   = e_ntfy_code;

    DBG_INFO((_INFO_HEADER"%s(), e_ntfy_code=%x\r\n", __func__, e_ntfy_code));

    switch ( e_ntfy_code )
    {
    case REC_UTIL_NTFY_CODE_COND_CHANGED:
        e_cond_new = ((REC_UTIL_COND_CHG_NTFY_INFO_T*)ui4_data)->e_cond_new;
        if ((e_cond_new == REC_UTIL_COND_READY) && (pt_media_desc->pf_nfy != NULL))
        {
            x_dbg_stmt(_INFO_HEADER"%s(CODE_COND_CHANGED), READY!\r\n", __func__);

            
            REC_UTIL_SHARE_DATA_T* pt_rec_share_data;
            
            pt_rec_share_data = (REC_UTIL_SHARE_DATA_T*)(((REC_UTIL_COND_CHG_NTFY_INFO_T*)ui4_data)->ui4_data);

            if(!pt_rec_share_data)
            {
                /*reuse the same the timer with minfo*/
                if(!pt_media_desc->h_minfo_delay_timer)
                {
                    x_timer_create(&(pt_media_desc->h_minfo_delay_timer));
                    if(!pt_media_desc->h_minfo_delay_timer)
                    {
                        ASSERT(FALSE,("Timeshift create timer failed\n file % s,line %d ",__FILE__,__LINE__));
                    }
                } 
                
                x_timer_start(
                        pt_media_desc->h_minfo_delay_timer,
                        0,
                        X_TIMER_FLAG_ONCE,
                        _svctx_rec_util_fake_cb,
                        (VOID*)(pt_media_desc->h_media_desc)
                        );
             }
            else
            {
                pt_media_desc->t_rec_tick_ctrl_blk = pt_rec_share_data->t_tick_cb;
                DBG_INFO(("\nCtrl block end addr %d,\n"
                            "start addr %d\n"
                            "entry num  %d\n"
                            "entry size %d\n"
                            "max entry num %d\n"
                            "tick period %d\n"
                            "lba int     %d\n"
                            "offset      %lld\n"
                            "packet num  %d\n",
                            pt_media_desc->t_rec_tick_ctrl_blk.pv_end_address,
                            pt_media_desc->t_rec_tick_ctrl_blk.pv_start_address,
                            pt_media_desc->t_rec_tick_ctrl_blk.ui4_entry_num,
                            pt_media_desc->t_rec_tick_ctrl_blk.ui4_entry_size,
                            pt_media_desc->t_rec_tick_ctrl_blk.ui4_max_vld_entry_num,
                            pt_media_desc->t_rec_tick_ctrl_blk.ui4_tick_period,
                            pt_media_desc->t_rec_tick_ctrl_blk.ui4_lba_init,
                            pt_media_desc->t_rec_tick_ctrl_blk.ui8_fifo_offset,
                            pt_media_desc->t_rec_tick_ctrl_blk.ui4_fifo_pkt_num));    
                pt_media_desc->ps_media_path = (CHAR*)x_mem_alloc(x_strlen(pt_rec_share_data->ps_strg_path) + 1);
                if (pt_media_desc->ps_media_path == NULL)
                {
                    return FALSE;
                }
                x_strcpy(pt_media_desc->ps_media_path, pt_rec_share_data->ps_strg_path);
                pt_media_desc->pf_nfy(pt_media_desc->h_svctx,
                              SVCTX_COND_PENDING,
                              SVCTX_NTFY_CODE_MEDIA_PRE_PROCESSED,
                              ST_UNKNOWN,
                              pt_media_desc->pv_tag,
                              (VOID*)pt_media_desc->h_media_desc);
            }
        }
        t_msg.u.t_rec.uinfo.t_cond_chg = *((REC_UTIL_COND_CHG_NTFY_INFO_T*)ui4_data);
        break;
    case REC_UTIL_NTFY_CODE_VALID_RANGE_UPDATED:
        t_msg.u.t_rec.uinfo.t_range = *((REC_UTIL_VALID_RANGE_NTFY_INFO_T*)ui4_data);
        break;
    case REC_UTIL_NTFY_CODE_VALID_RANGE_INFO_UPDATED:
        t_msg.u.t_rec.uinfo.ui4_data = ui4_data;
        break;
    case REC_UTIL_NTFY_CODE_RESET_REQ:
        t_msg.u.t_rec.uinfo.ui4_data = ui4_data;
        b_ret = FALSE;
        break;
    case REC_UTIL_NTFY_CODE_LOAD_FILE_ACK:
    case REC_UTIL_NTFY_CODE_STORE_FILE_ACK:
    case REC_UTIL_NTFY_CODE_WRITE_ACK:
    case REC_UTIL_NTFY_CODE_SET_CONFIG_ACK:
    default:
        return TRUE;
    }

    if(pt_svctx->pt_running_svc_req != NULL)
    {
        if (pt_svctx->pt_running_svc_req->e_src_type != SRC_TYPE_MEDIA_TIME_SHIFT)
        {
            return TRUE;
        }
    }
    else
    {
        return TRUE;
    }
    if (e_ntfy_code == REC_UTIL_NTFY_CODE_RESET_REQ)
    {
        x_dbg_stmt(_INFO_HEADER"%s(CODE_RESET_REQ)\r\n", __func__);
    }

    svctx_send_msg(pt_media_desc->h_svctx, &t_msg);

    return b_ret;
}

#endif /* TIME_SHIFT_SUPPORT */

/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_handle_media_cmd
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
static INT32 _svctx_handle_media_cmd(
                    MM_SVC_COMMAND_T*               pt_cmd,
                    MEDIA_DESCRIBT_T*               pt_media_desc
                    )
{
    MINFO_INFO_T*                    pt_meta_info = NULL;
    INT32               i4_ret = 0;
    MM_STRM_INFO_T*                  pt_strm_info = NULL;
    CHAR*                            ps_buf        = NULL;

    if (pt_cmd == NULL || pt_media_desc == NULL)
    {
        return SVCTXR_INV_ARG;
    }
    
    while ( pt_cmd->e_code != MM_SVC_CMD_CODE_END )
    {
        switch ( pt_cmd->e_code )
        {
        case MM_SVC_CMD_CODE_SVL_NAME:
            pt_media_desc->ps_svl_name = pt_cmd->u.ps_text;
            break;
            
        case MM_SVC_CMD_CODE_SVL_ID:
            pt_media_desc->ui2_svl_id = pt_cmd->u.ui2_data;
            break;
            
        case MM_SVC_CMD_CODE_SRC_TYPE:
            pt_media_desc->e_src_type = pt_cmd->u.ui1_data;
            break;
        
        case MM_SVC_CMD_CODE_SET_SRC_SUB_TYPE:
            pt_media_desc->e_src_sub_type = (SRC_SUB_TYPE_T)pt_cmd->u.ui1_data;
            break;
            
        case MM_SVC_CMD_CODE_MEDIA_PATH:
            if(pt_cmd->u.ps_text)
            {
                pt_media_desc->ps_media_path = (CHAR*)x_mem_alloc(x_strlen(pt_cmd->u.ps_text) + 1);
                if (pt_media_desc->ps_media_path == NULL)
                {
                    return SVCTXR_NO_RESOURCE;
                  
                }
                x_strcpy(pt_media_desc->ps_media_path, pt_cmd->u.ps_text);
            }
            break;
            
        case MM_SVC_CMD_CODE_SET_PLAY_MODE:
            pt_media_desc->e_play_mode = (MM_PLAY_MODE_T)(pt_cmd->u.ui1_data);
            break;

        case MM_SVC_CMD_CODE_EXT_SBTL_PATH:
            pt_media_desc->pt_ext_sbtl_info = x_mem_alloc(sizeof(MM_EXT_SBTL_INFO));
            if(pt_media_desc->pt_ext_sbtl_info == NULL)
            {
                return SVCTXR_NO_RESOURCE;
            }
            x_memset(pt_media_desc->pt_ext_sbtl_info,0, sizeof(MM_EXT_SBTL_INFO));
            pt_media_desc->pt_ext_sbtl_info->e_type = ((MM_EXT_SBTL_INFO*)pt_cmd->u.pv_data)->e_type;
            pt_media_desc->pt_ext_sbtl_info->u.ps_full = ((MM_EXT_SBTL_INFO*)pt_cmd->u.pv_data)->u.ps_full;
            pt_media_desc->pt_ext_sbtl_info->u.ps_ext_lst = ((MM_EXT_SBTL_INFO*)pt_cmd->u.pv_data)->u.ps_ext_lst;
            if(((MM_EXT_SBTL_INFO*)pt_cmd->u.pv_data)->ps_prf_lang_lst != NULL)
            {
                pt_media_desc->pt_ext_sbtl_info->ps_prf_lang_lst =
                    (CHAR*)x_mem_alloc(x_strlen(((MM_EXT_SBTL_INFO*)pt_cmd->u.pv_data)->ps_prf_lang_lst) + 1);
                if(pt_media_desc->pt_ext_sbtl_info->ps_prf_lang_lst == NULL)
                {
                    return SVCTXR_NO_RESOURCE;
                }
                x_memcpy(pt_media_desc->pt_ext_sbtl_info->ps_prf_lang_lst,
                         ((MM_EXT_SBTL_INFO*)pt_cmd->u.pv_data)->ps_prf_lang_lst,
                         x_strlen(((MM_EXT_SBTL_INFO*)pt_cmd->u.pv_data)->ps_prf_lang_lst) + 1);
            }
            break;
            
        case MM_SVC_CMD_CODE_EXT_LYRIC_PATH:
            pt_media_desc->pt_lyric_info = x_mem_alloc(sizeof(MM_LYRIC_INFO));
            if(pt_media_desc->pt_lyric_info == NULL)
            {
               return SVCTXR_NO_RESOURCE;
            }
            x_memset(pt_media_desc->pt_lyric_info,0, sizeof(MM_LYRIC_INFO));
            pt_media_desc->pt_lyric_info->e_type = ((MM_LYRIC_INFO*)pt_cmd->u.pv_data)->e_type;
            pt_media_desc->pt_lyric_info->u.ps_lyric_list = ((MM_LYRIC_INFO*)pt_cmd->u.pv_data)->u.ps_lyric_list;
            if(((MM_LYRIC_INFO*)pt_cmd->u.pv_data)->ps_prf_lang_lst != NULL)
            {
                pt_media_desc->pt_lyric_info->ps_prf_lang_lst =
                    (CHAR*)x_mem_alloc(x_strlen(((MM_LYRIC_INFO*)pt_cmd->u.pv_data)->ps_prf_lang_lst) + 1);
                if(pt_media_desc->pt_lyric_info->ps_prf_lang_lst == NULL)
                {
                    return SVCTXR_NO_RESOURCE;
                }
                x_memcpy(pt_media_desc->pt_lyric_info->ps_prf_lang_lst,
                         ((MM_LYRIC_INFO*)pt_cmd->u.pv_data)->ps_prf_lang_lst,
                         x_strlen(((MM_LYRIC_INFO*)pt_cmd->u.pv_data)->ps_prf_lang_lst) + 1);
            }
            break;

        case MM_SVC_CMD_CODE_SET_STRM_INFO:
            /* all information of streams would be put in pt_strm_info */
            pt_strm_info = (MM_STRM_INFO_T*)(pt_cmd->u.pv_data);

            if (pt_strm_info == NULL)
            {
                return SVCTXR_INV_COMMAND;
            }

            /* copy stream information to pt_media_desc */
            pt_media_desc->pt_strm_info = (MM_STRM_INFO_T*)x_mem_alloc(sizeof(MM_STRM_INFO_T));
            if(pt_media_desc->pt_strm_info == NULL)
            {
                return SVCTXR_NO_RESOURCE;
            }
            x_memcpy((VOID*)(pt_media_desc->pt_strm_info), (VOID*)pt_strm_info, sizeof(MM_STRM_INFO_T));
            break;
            
        case MM_SVC_CMD_CODE_SET_BOOT_ANIM:
            pt_media_desc->b_boot_anim = pt_cmd->u.b_boolean;
           break;

        /*add for common API*/
        case MM_SVC_CMD_CODE_SET_PROTOCL:
            pt_media_desc->e_svc_protocol = (MM_SVC_PROTOCOL_T)pt_cmd->u.ui4_data;
           break;
           
        case MM_SVC_CMD_CODE_SET_META_INFO:            
            pt_meta_info = (MINFO_INFO_T*)(pt_cmd->u.pv_data);

            if (pt_meta_info == NULL || 
                pt_meta_info->e_info_type != MINFO_INFO_TYPE_META_DATA)
            {
                break;/*return SVCTXR_INV_COMMAND; //temp allowed null for test*/
            }
            i4_ret = _svctx_set_meta_info(pt_media_desc, pt_meta_info);
            if(SVCTXR_OK != i4_ret)
            {
                return i4_ret;
            }
            pt_media_desc->ui8_media_dur = pt_meta_info->u.t_meta_data.ui8_pb_duration * 90000;
            break;
            
        case MM_SVC_CMD_CODE_SET_PULL_MODE_INFO:
            pt_media_desc->t_playback_ctrl_pull_info
                        = *((MM_PB_CTRL_PULL_INFO_T*)pt_cmd->u.ui4_data);
           break;
           
        case MM_SVC_CMD_CODE_SET_PUSH_MODE_INFO:
            pt_media_desc->t_pbctrl_push_mode_info
                        = *((MM_PB_CTRL_PUSH_MODE_INFO_T*)pt_cmd->u.ui4_data);
            pt_media_desc->ui8_media_sz  = (UINT64)(-1);
            pt_media_desc->ui8_media_dur = (UINT64)(-1);
           break;
#ifdef ENABLE_NAUTILUS
        case MM_SVC_CMD_CODE_SET_DRM_INFO:
        {
            MM_DRM_INFO_T*   pt_drm_info = (MM_DRM_INFO_T*)(pt_cmd->u.pv_data);
            pt_media_desc->t_drm_info.e_drm_type 
                = pt_drm_info->e_drm_type;
            
            pt_media_desc->t_drm_info.u.t_mln_drm_inf.pui1_act_tkn_url
                = (UINT8*)x_mem_calloc(1, sizeof(UINT8) * (pt_drm_info->u.t_mln_drm_inf.ui2_act_tkn_url_length+1));
            if(pt_media_desc->t_drm_info.u.t_mln_drm_inf.pui1_act_tkn_url == NULL)
            {
                return SVCTXR_NO_RESOURCE;
            }
            x_memcpy(pt_media_desc->t_drm_info.u.t_mln_drm_inf.pui1_act_tkn_url,
                     pt_drm_info->u.t_mln_drm_inf.pui1_act_tkn_url,
                     pt_drm_info->u.t_mln_drm_inf.ui2_act_tkn_url_length);
            pt_media_desc->t_drm_info.u.t_mln_drm_inf.ui2_act_tkn_url_length 
                = pt_drm_info->u.t_mln_drm_inf.ui2_act_tkn_url_length;
            
            pt_media_desc->t_drm_info.u.t_mln_drm_inf.pui1_afl_tkn
                = (UINT8*)x_mem_calloc(1, sizeof(UINT8) * (pt_drm_info->u.t_mln_drm_inf.ui2_afl_tkn_length+1));
            if(pt_media_desc->t_drm_info.u.t_mln_drm_inf.pui1_afl_tkn == NULL)
            {
                return SVCTXR_NO_RESOURCE;
            }
            x_memcpy(pt_media_desc->t_drm_info.u.t_mln_drm_inf.pui1_afl_tkn,
                     pt_drm_info->u.t_mln_drm_inf.pui1_afl_tkn,
                     pt_drm_info->u.t_mln_drm_inf.ui2_afl_tkn_length);
            pt_media_desc->t_drm_info.u.t_mln_drm_inf.ui2_afl_tkn_length 
                = pt_drm_info->u.t_mln_drm_inf.ui2_afl_tkn_length;
        }
            break;
#endif       
        case MM_SVC_CMD_CODE_SET_PROXY_URL:
            if(pt_cmd->u.ps_text)
            {
                pt_media_desc->ps_proxy_url = (CHAR*)x_mem_alloc(x_strlen(pt_cmd->u.ps_text) + 1);
                if (pt_media_desc->ps_proxy_url == NULL)
                {
                    return SVCTXR_NO_RESOURCE;
                }
                x_strcpy(pt_media_desc->ps_proxy_url, pt_cmd->u.ps_text);
            }
            break;
        case MM_SVC_CMD_CODE_SET_AGENT_URL:
            if(pt_cmd->u.ps_text)
            {
                pt_media_desc->ps_agent_url = (CHAR*)x_mem_alloc(x_strlen(pt_cmd->u.ps_text) + 1);
                if (pt_media_desc->ps_agent_url == NULL)
                {
                    return SVCTXR_NO_RESOURCE;
                }
                x_strcpy(pt_media_desc->ps_agent_url, pt_cmd->u.ps_text);
            }
            break;
        case MM_SVC_CMD_CODE_SET_COOKIE_URL:
            if(pt_cmd->u.ps_text)
            {
                pt_media_desc->ps_cookie_url = (CHAR*)x_mem_alloc(x_strlen(pt_cmd->u.ps_text) + 1);
                if (pt_media_desc->ps_cookie_url == NULL)
                {
                    return SVCTXR_NO_RESOURCE;
                }
                x_strcpy(pt_media_desc->ps_cookie_url, pt_cmd->u.ps_text);
            }
            break;
        case MM_SVC_CMD_CODE_SET_FILL_SCDB_MODE:
            pt_media_desc->e_fill_scdb_mode = (MM_SVC_FILL_SCDB_MODE)pt_cmd->u.ui1_data;
           break;    

		case MM_SVC_CMD_CODE_SET_PVR_TOTAl_DUR :
            pt_media_desc->ui8_media_dur = (UINT64)pt_cmd->u.ui4_data * 90000;
           // x_dbg_stmt("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%llu\n", pt_media_desc->ui8_media_dur);
            break;
        default:
            return SVCTXR_INV_COMMAND;
        }
        pt_cmd = &pt_cmd[1];
    }

    if(x_strncasecmp(pt_media_desc->ps_media_path, "file://", 7) == 0)
    {
        pt_media_desc->e_src_type = SRC_TYPE_MEDIA_STORGE;    
    }
    else if(x_strncasecmp(pt_media_desc->ps_media_path, "http:", 5) == 0)
    {
        pt_media_desc->e_src_type = SRC_TYPE_MEDIA_NETWORK;
    }
    else if(x_strncasecmp(pt_media_desc->ps_media_path, "https:", 6) == 0)
    {
        pt_media_desc->e_src_type = SRC_TYPE_MEDIA_NETWORK;
    }
#ifdef ENABLE_MMS_SUPPORT
    else if((x_strncasecmp(pt_media_desc->ps_media_path, "mms:", 4) == 0) || 
            (x_strncasecmp(pt_media_desc->ps_media_path, "mmst:", 5) == 0))
    {
        pt_media_desc->e_src_type = SRC_TYPE_MEDIA_NETWORK;
    }
#endif

    ps_buf = pt_media_desc->ps_media_path;
    if (x_strncasecmp(ps_buf, "file://", 7) == 0)
    {
        x_strcpy(pt_media_desc->ps_media_path, ps_buf + 7);
    }
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _print_file_format
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
static void _print_file_format(MEDIA_FORMAT_T t_format)
{
    switch (t_format.e_media_type)
    {
        case MEDIA_TYPE_UNKNOWN:
            x_dbg_stmt(_INFO_HEADER" media format is UNKNOWN!\n");
            break;
        case MEDIA_TYPE_AUDIO_ES:
        {
            switch (t_format.t_media_subtype.u.e_aud_subtype)
            {
                case MEDIA_AUD_SUBTYPE_UNKNOWN:
                    x_dbg_stmt(_INFO_HEADER" media format is UNKNOWN!\n");
                    break;
                case MEDIA_AUD_SUBTYPE_MP2:
                case MEDIA_AUD_SUBTYPE_MP3:
                    x_dbg_stmt(_INFO_HEADER" media format is MP3!\n");
                    break;
                case MEDIA_AUD_SUBTYPE_WMA:
                    x_dbg_stmt(_INFO_HEADER" media format is WMA!\n");
                    break;
                case MEDIA_AUD_SUBTYPE_WAVE:
                    x_dbg_stmt(_INFO_HEADER" media format is WAV!\n");
                    break;
                case MEDIA_AUD_SUBTYPE_AAC:
                    x_dbg_stmt(_INFO_HEADER" media format is AAC!\n");
                    break;
                case MEDIA_AUD_SUBTYPE_AC3:
                    x_dbg_stmt(_INFO_HEADER" media format is AC3!\n");
                    break;
                case MEDIA_AUD_SUBTYPE_AIF:
                    x_dbg_stmt(_INFO_HEADER" media format is AIF!\n");
                    break;
                case MEDIA_AUD_SUBTYPE_LPCM:
                    x_dbg_stmt(_INFO_HEADER" media format is LPCM!\n");
                    break;
                case MEDIA_AUD_SUBTYPE_AMR:
                    x_dbg_stmt(_INFO_HEADER" media format is AMR!\n");
                    break;
                case MEDIA_AUD_SUBTYPE_AWB:
                    x_dbg_stmt(_INFO_HEADER" media format is AWB!\n");
                    break;
                default:
                    break;
            }
        }
            break;
        case MEDIA_TYPE_VIDEO_ES:
        {
            switch (t_format.t_media_subtype.u.e_vid_subtype)
            {
                case MEDIA_VID_SUBTYPE_UNKNOWN:
                    x_dbg_stmt(_INFO_HEADER" media format is UNKNOWN!\n");
                    break;
                case MEDIA_VID_SUBTYPE_AVS:
                    x_dbg_stmt(_INFO_HEADER" media format is VIDEO ES AVS!\n");
                    break;
                default:
                    x_dbg_stmt(_INFO_HEADER" media format is VIDEO ES!\n");
                    break;
            }
        }
            break;
        case MEDIA_TYPE_CONTAINER:
        {
            switch (t_format.t_media_subtype.u.e_contnr_subtype)
            {
                case MEDIA_CONTNR_SUBTYPE_UNKNOWN:
                    x_dbg_stmt(_INFO_HEADER" media format is UNKNOWN!\n");
                    break;
                case MEDIA_CONTNR_SUBTYPE_AVI:
                    x_dbg_stmt(_INFO_HEADER" media format is AVI!\n");
                    break;
                case MEDIA_CONTNR_SUBTYPE_ASF:
                    x_dbg_stmt(_INFO_HEADER" media format is ASF!\n");
                    break;
                case MEDIA_CONTNR_SUBTYPE_MP4:
                    x_dbg_stmt(_INFO_HEADER" media format is MP4!\n");
                    break;
                case MEDIA_CONTNR_SUBTYPE_MATROSKA:
                    x_dbg_stmt(_INFO_HEADER" media format is MKV!\n");
                    break;
                case MEDIA_CONTNR_SUBTYPE_RM:
                    x_dbg_stmt(_INFO_HEADER" media format is RM!\n");
                    break;
                case MEDIA_CONTNR_SUBTYPE_FLV:
                    x_dbg_stmt(_INFO_HEADER" media format is FLV!\n");
                    break;
                case MEDIA_CONTNR_SUBTYPE_CMPB:
                    x_dbg_stmt(_INFO_HEADER" media format is CMPB!\n");
                    break;
                case MEDIA_CONTNR_SUBTYPE_OGG:
                    x_dbg_stmt(_INFO_HEADER" media format is OGG/OGM!\n");
                    break;
            }
        }
            break;
        case MEDIA_TYPE_PS:
            x_dbg_stmt(_INFO_HEADER" media format is PS!\n");
            break;
        case MEDIA_TYPE_TS:
            x_dbg_stmt(_INFO_HEADER" media format is TS!\n");
            break;
        default:
            x_dbg_stmt(_INFO_HEADER" media format is UNKNOWN!\n");
            break;
    }
}

/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_handle_lyric_info
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
static INT32 _svctx_handle_lyric_info(
                    MEDIA_DESCRIBT_T*               pt_media_desc
                    )
{
    CHAR*                           ps_buf        = NULL;
    CHAR*                           ps_lyric_list = NULL;
    UINT32                          ui4_path_length = 0;
    CHAR                            ac_extension[MAX_EXTENSION_NAME_LEN];
    UINT32                          i4_ret = 0;

    /* check param */
    if (pt_media_desc == NULL)
    {
        return SVCTXR_INV_ARG;
    }
    
    if (pt_media_desc->pt_lyric_info != NULL)
    {
        if ((pt_media_desc->pt_lyric_info->e_type == MM_LYRIC_TYPE_LRC)
            && (pt_media_desc->ps_media_path != NULL))
        {
            ps_buf = x_strrchr(pt_media_desc->ps_media_path, '.');//ps_buf
            ps_buf++; /* cut off '.' */
            ui4_path_length = (UINT32)(ps_buf - pt_media_desc->ps_media_path);
            pt_media_desc->ps_lyric_full_path =
                x_mem_alloc(ui4_path_length + MAX_EXTENSION_NAME_LEN);
            if(pt_media_desc->ps_lyric_full_path == NULL)
            {
                return SVCTXR_NO_RESOURCE;
            }
            x_memset(pt_media_desc->ps_lyric_full_path,
                     0,
                     ui4_path_length + MAX_EXTENSION_NAME_LEN);
            x_memcpy(pt_media_desc->ps_lyric_full_path,
                     pt_media_desc->ps_media_path,
                     ui4_path_length);

            ps_lyric_list = pt_media_desc->pt_lyric_info->u.ps_lyric_list;
            while(x_strlen(ps_lyric_list) > 0)
            {
                x_memset(ac_extension, 0 , MAX_EXTENSION_NAME_LEN * sizeof(CHAR));
                ps_buf = x_strchr(ps_lyric_list, ';');
                x_memcpy(ac_extension,
                         ps_lyric_list,
                         (SIZE_T)(ps_buf - ps_lyric_list));
                ps_lyric_list = ps_buf + 1; /* skip ';' */
                /* parser cmd :internal */
                ps_buf = x_strchr(ac_extension, ':');
                if (ps_buf != NULL)
                {
                    pt_media_desc->b_is_lyric = FALSE;
                    break;
                }

                ps_buf = pt_media_desc->ps_lyric_full_path;
                x_strcat(ps_buf, ac_extension);
#ifndef ANDROID
                i4_ret = x_fm_open( FM_ROOT_HANDLE, 
                                    ps_buf,
                                    FM_READ_ONLY, 
                                    0777,
                                    FALSE, 
                                    &pt_media_desc->h_lyric);
#else
                {
                    FILE *pt_file = NULL;
                    pt_file = fopen (ps_buf, "r");
                    if(pt_file == NULL)
                    {
                        return SVCTXR_INV_ARG;
                    }
                    pt_media_desc->h_lyric = (HANDLE_T)pt_file;
                }
#endif
                if ((i4_ret == FMR_OK) && (pt_media_desc->h_lyric != NULL_HANDLE))
                {
                    pt_media_desc->b_is_lyric = TRUE;
                    break;
                }
                else
                {
                    x_memset(pt_media_desc->ps_lyric_full_path + ui4_path_length,
                             0,
                             MAX_EXTENSION_NAME_LEN * sizeof(CHAR));
                }
            }
        }
        else if((pt_media_desc->pt_lyric_info->e_type == MM_LYRIC_TYPE_LRC_FULL_PATH)
                    && (pt_media_desc->ps_media_path != NULL))
        {
            ps_buf = pt_media_desc->pt_lyric_info->u.ps_full;
            if(x_strncasecmp(ps_buf, "file://", 7) == 0)
            {
                x_strcpy(pt_media_desc->pt_lyric_info->u.ps_full, ps_buf + 7);
            }
            else if((x_strncasecmp(ps_buf, "http:", 5) == 0) ||
                    (x_strncasecmp(ps_buf, "https:", 6) == 0))
            {
                pt_media_desc->b_is_ext_sbtl_http_file = TRUE;
            }
            if(pt_media_desc->b_is_ext_sbtl_http_file)
            {
#ifdef INET_SUPPORT
                i4_ret = x_httpc_fm_open_proxy_agent_timeout_cookie(
                                            pt_media_desc->pt_lyric_info->u.ps_full, 
                                            pt_media_desc->ps_proxy_url, 
                                            pt_media_desc->ps_agent_url, 
                                            0,
                                            128*1024,
                                            &pt_media_desc->h_lyric,
                                            pt_media_desc->ps_cookie_url);
#endif
            }
            else
            {
#ifndef ANDROID
                i4_ret = x_fm_open( FM_ROOT_HANDLE, 
                                    pt_media_desc->pt_lyric_info->u.ps_full,
                                    FM_READ_ONLY, 
                                    0777,
                                    FALSE, 
                                    &pt_media_desc->h_lyric);
#else
                {
                    FILE *pt_file = NULL;
                    pt_file = fopen (ps_buf, "r");
                    if(pt_file == NULL)
                    {
                        return SVCTXR_INV_ARG;
                    }
                    pt_media_desc->h_lyric = (HANDLE_T)pt_file;
                }
#endif
            }
            if ((i4_ret == FMR_OK) && (pt_media_desc->h_lyric != NULL_HANDLE))
            {
                pt_media_desc->b_is_lyric = TRUE;
            }
            else
            {
                x_memset(pt_media_desc->ps_lyric_full_path + ui4_path_length,
                         0,
                         MAX_EXTENSION_NAME_LEN * sizeof(CHAR));
            }
        }
        if(pt_media_desc->h_lyric != NULL_HANDLE)
        {/* Set Lyric default attribute */
            SVCTX_T*    pt_svctx = NULL;

            i4_ret = svctx_get_obj(pt_media_desc->h_svctx, &pt_svctx);
            if (i4_ret != SVCTXR_OK)
            {
                return i4_ret;
            }
            i4_ret = _mm_hdlr_set_lyric_sbtl(pt_svctx,
                                             SVCTX_MM_SBTL_SET_TYPE_DEFAULT,
                                             NULL,
                                             (SIZE_T)0);
        }
    }
    
    return SVCTXR_OK;
}


static INT32 _svctx_get_ext_sbtl_info( MEDIA_DESCRIBT_T*       pt_media_desc,
                                       SBTL_LANG_DESC_T*       pt_sbtl_lang_desc)
{
    INT32       i4_ret          = SVCTXR_OK;
    UINT32      ui4_i           = 0;
    UINT32      ui4_cur_total   = 0;
    HANDLE_T    h_ext_subtitle  = NULL_HANDLE;
    HANDLE_T    h_rela_subtitle = NULL_HANDLE;
    
    if((pt_media_desc== NULL) || (pt_sbtl_lang_desc == NULL))
    {
        return SVCTXR_INV_ARG;
    }
    
    i4_ret = x_sbtl_parse_lang(pt_sbtl_lang_desc);
    x_dbg_stmt("======x_sbtl_parse_lang ret %d, sub_cnt=%d, ====\r\n", 
        i4_ret, pt_sbtl_lang_desc->ui1_lang_count);
    if (i4_ret != MMSBTLR_OK || pt_sbtl_lang_desc->ui1_lang_count == 0)
    {
        return SVCTXR_OK;
    }
        
    pt_media_desc->b_is_ext_first =TRUE;
    
    if(pt_sbtl_lang_desc->b_is_http_file)
    {
#ifdef INET_SUPPORT
        i4_ret = x_httpc_fm_open_proxy_agent_timeout_cookie(
                                    pt_sbtl_lang_desc->pc_sbtl_file_name, 
                                    pt_media_desc->ps_proxy_url, 
                                    pt_media_desc->ps_agent_url, 
                                    0,
                                    128*1024,
                                    &h_ext_subtitle,
                                    pt_media_desc->ps_cookie_url);
#endif
    }
    else
    {
#ifndef ANDROID
        i4_ret = x_fm_open( FM_ROOT_HANDLE, 
                            pt_sbtl_lang_desc->pc_sbtl_file_name,
                            FM_READ_ONLY, 
                            0777,
                            FALSE, 
                            &h_ext_subtitle);
#else
        FILE *pt_file = NULL;
        pt_file = fopen (pt_sbtl_lang_desc->pc_sbtl_file_name, "r");
        if(pt_file == NULL)
        {
            return SVCTXR_INV_ARG;
        }
        h_ext_subtitle = (HANDLE_T)pt_file;
#endif
    }
    if (i4_ret != FMR_OK)
    {
        return SVCTXR_NOT_EXIST;
    }
    
    ui4_cur_total = pt_media_desc->ui2_total_ext_subtitle_num;
    if (ui4_cur_total + pt_sbtl_lang_desc->ui1_lang_count >= 8)
    {
        pt_sbtl_lang_desc->ui1_lang_count = 8 - ui4_cur_total;
    }
    
    if (x_strlen(pt_sbtl_lang_desc->pc_related_file_name) != 0)
    {
        if(pt_sbtl_lang_desc->b_is_http_file)
        {
#ifdef INET_SUPPORT
            i4_ret = x_httpc_fm_open_proxy_agent_timeout_cookie(
                                        pt_sbtl_lang_desc->pc_related_file_name, 
                                        pt_media_desc->ps_proxy_url, 
                                        pt_media_desc->ps_agent_url, 
                                        0,
                                        128*1024,
                                        &h_rela_subtitle,
                                        pt_media_desc->ps_cookie_url);
#endif
        }
        else
        {
#ifndef ANDROID
            i4_ret = x_fm_open( FM_ROOT_HANDLE, 
                                pt_sbtl_lang_desc->pc_related_file_name,
                                FM_READ_ONLY, 
                                0777,
                                FALSE, 
                                &h_rela_subtitle);
#else
            FILE *pt_file = NULL;
            pt_file = fopen (pt_sbtl_lang_desc->pc_related_file_name, "r");
            if(pt_file == NULL)
            {
                return SVCTXR_INV_ARG;
            }
            h_rela_subtitle = (HANDLE_T)pt_file;
#endif
        }
        if (i4_ret != FMR_OK)
        {
            return SVCTXR_NOT_EXIST;
        }
    }
    
    for (ui4_i = 0; ui4_i < pt_sbtl_lang_desc->ui1_lang_count; ui4_i++)
    {
        if ((ui4_cur_total + ui4_i >= EXT_SUBTITLE_MAX_NUM) ||
            (ui4_i >= MM_SBTL_PREFERED_LANG_LIST_LEN))
        {
            break;
        }
        pt_media_desc->t_mm_svctx_sbtl_desc[ui4_cur_total + ui4_i].h_ext_subtitle = h_ext_subtitle;
        x_memcpy(pt_media_desc->t_mm_svctx_sbtl_desc[ui4_cur_total + ui4_i].c_lang_list,
                 pt_sbtl_lang_desc->c_lang_list[ui4_i],
                 3);
        pt_media_desc->t_mm_svctx_sbtl_desc[ui4_cur_total + ui4_i].ui2_idx = ui4_i;
        if (x_strlen(pt_sbtl_lang_desc->pc_related_file_name) != 0)
        {
            pt_media_desc->t_mm_svctx_sbtl_desc[ui4_cur_total + ui4_i].h_related_subtitle = h_rela_subtitle;
            pt_media_desc->t_mm_svctx_sbtl_desc[ui4_cur_total + ui4_i].b_is_related = TRUE;
        }
    }
    pt_media_desc->ui2_total_ext_subtitle_num += pt_sbtl_lang_desc->ui1_lang_count;
    
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_handle_extra_subtl
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
static INT32 _svctx_handle_extra_subtl(
                    HANDLE_T                        h_svctx,
                    MEDIA_DESCRIBT_T*               pt_media_desc
                    )
{
    CHAR*                               ps_buf              = NULL;
    UINT32                              ui4_path_length     = 0;
    UINT32                              ui4_filename_length = 0;
    INT32                               i4_ret              = SVCTXR_ASYNC_NFY;
    CHAR*                               ps_dir_buf          = NULL;
    CHAR*                               ps_filename_no_exten= NULL;
    CHAR*                               ps_ext_list         = NULL;
    HANDLE_T                            h_dir_hdl           = NULL_HANDLE;
    FM_DIR_ENTRY_T                      t_dir_entr;
    UINT32                              ui4_num_entr        = 0;
    INT32                               i4_chk_ret          = SVCTXR_OK;
    SBTL_LANG_DESC_T*                   pt_sbtl_lang_desc   = NULL;
    CHAR*                               ps_file_name        = NULL;
    CHAR*                               ac_extension        = NULL;
    CHAR                                s_extension[MAX_EXTENSION_NAME_LEN];
    
    if (pt_media_desc == NULL)
    {
        return SVCTXR_INV_ARG;
    }
    
    if (pt_media_desc->pt_ext_sbtl_info != NULL)
    {
        do
        {
            pt_sbtl_lang_desc = x_mem_alloc(sizeof(SBTL_LANG_DESC_T));
            if(pt_sbtl_lang_desc == NULL)
            {
                i4_chk_ret = SVCTXR_NO_RESOURCE;
                break;
            }
            x_memset(pt_sbtl_lang_desc, 0, sizeof(SBTL_LANG_DESC_T));
            
            pt_sbtl_lang_desc->pc_related_file_name = x_mem_alloc((FM_MAX_FILE_LEN+1)*4);
            if(pt_sbtl_lang_desc->pc_related_file_name == NULL)
            {
                i4_chk_ret = SVCTXR_NO_RESOURCE;
                break;
            }
            x_memset(pt_sbtl_lang_desc->pc_related_file_name, 0, (FM_MAX_FILE_LEN+1)*4);
                    
            if (pt_media_desc->pt_ext_sbtl_info->e_type == MM_EXT_SBTL_TYPE_FULL)
            {
                pt_media_desc->ps_ext_full_path =
                    x_mem_alloc(x_strlen(pt_media_desc->pt_ext_sbtl_info->u.ps_full)+1);
                if(pt_media_desc->ps_ext_full_path == NULL)
                {
                    i4_chk_ret = SVCTXR_NO_RESOURCE;
                    break;
                }
                x_memset(pt_media_desc->ps_ext_full_path,
                         0,
                         x_strlen(pt_media_desc->pt_ext_sbtl_info->u.ps_full)+1);
                x_memcpy(pt_media_desc->ps_ext_full_path,
                         pt_media_desc->pt_ext_sbtl_info->u.ps_full,
                         x_strlen(pt_media_desc->pt_ext_sbtl_info->u.ps_full)+1);
                
                ps_buf = pt_media_desc->ps_ext_full_path;
                if(x_strncasecmp(ps_buf, "file://", 7) == 0)
                {
                    x_strcpy(pt_media_desc->ps_ext_full_path, ps_buf + 7);
                }
                else if((x_strncasecmp(ps_buf, "http:", 5) == 0) ||
                        (x_strncasecmp(ps_buf, "https:", 6) == 0))
                {
                    pt_sbtl_lang_desc->b_is_http_file = TRUE;
                    pt_media_desc->b_is_ext_sbtl_http_file = TRUE;
                }
        
                pt_sbtl_lang_desc->pc_sbtl_file_name=  NULL;
                pt_sbtl_lang_desc->ui1_lang_count   = 0;
                pt_sbtl_lang_desc->pc_sbtl_file_name= pt_media_desc->ps_ext_full_path;                 
                i4_ret = _svctx_get_ext_sbtl_info(  pt_media_desc,
                                                    pt_sbtl_lang_desc);
                if(i4_ret != SVCTXR_OK)
                {
                    break;
                }
            }
            else if ((pt_media_desc->pt_ext_sbtl_info->e_type == MM_EXT_SBTL_TYPE_LST)
                && (pt_media_desc->ps_media_path != NULL))
            {
                ps_buf = x_strrchr(pt_media_desc->ps_media_path, '/');//ps_buf
                ps_buf++;
                ui4_path_length = (UINT32)(ps_buf - pt_media_desc->ps_media_path);
                ps_dir_buf = x_mem_alloc(ui4_path_length + 1);
                if(ps_dir_buf == NULL)
                {
                    i4_chk_ret = SVCTXR_NO_RESOURCE;
                    break;
                }
                x_memset(ps_dir_buf,    0,  ui4_path_length + 1);
                x_memcpy(ps_dir_buf,
                         pt_media_desc->ps_media_path,
                         ui4_path_length);
                ps_filename_no_exten = ps_buf;
                ps_buf = x_strrchr(ps_filename_no_exten, '.');
                ui4_filename_length = (UINT32)(ps_buf - ps_filename_no_exten);
                ps_buf = ps_filename_no_exten;
                ps_filename_no_exten = x_mem_alloc(ui4_filename_length + 1);
                if(ps_filename_no_exten == NULL)
                {
                    i4_chk_ret = SVCTXR_NO_RESOURCE;
                    break;
                }
                x_memset(ps_filename_no_exten,  0,      ui4_filename_length + 1);
                x_memcpy(ps_filename_no_exten,  ps_buf, ui4_filename_length);
                               
                ps_file_name = x_mem_alloc((FM_MAX_FILE_LEN+1)*4); 
                if(ps_file_name == NULL)
                {
                    i4_chk_ret = SVCTXR_NO_RESOURCE;
                    break;
                }
                
                i4_ret = x_fm_open_dir(FM_ROOT_HANDLE, ps_dir_buf, &h_dir_hdl);
                if (FMR_OK != i4_ret)
                {
                    x_dbg_stmt("x_fm_open_dir() returns %d\n", i4_ret);
                    i4_chk_ret = SVCTXR_FAIL;
                    break;
                }
                ps_ext_list = pt_media_desc->pt_ext_sbtl_info->u.ps_ext_lst;
                
                do
                {
                    x_memset(&t_dir_entr, 0, sizeof(FM_DIR_ENTRY_T));
                    
                    i4_ret = x_fm_read_dir_entries(h_dir_hdl, &t_dir_entr, 1, &ui4_num_entr);
                    if (FMR_EOF == i4_ret)
                    {
                        i4_chk_ret = SVCTXR_NO_RECORD;
                        x_dbg_stmt("x_fm_read_dir_entries() arrives at FMR_EOF\n");
                        break;
                    }
                    else if (FMR_OK != i4_ret || 0 == ui4_num_entr)
                    {
                        i4_chk_ret = SVCTXR_FAIL;
                        x_dbg_stmt("x_fm_read_dir_entries() returns %d\n", i4_ret);
                        break;
                    }
                    
                    if ((0 == x_strcmp(t_dir_entr.s_name, "."))  ||
                        (0 == x_strcmp(t_dir_entr.s_name, "..")) ||
                        FM_IS_DIR(t_dir_entr.t_file_info.ui4_mode))
                    {
                        continue;
                    }
                    if (0 != x_memcmp(t_dir_entr.s_name,ps_filename_no_exten,ui4_filename_length))
                    {
                        continue;
                    }
                    /*use subtitle function to acquire language num*/
                    x_memset(ps_file_name, 0, (FM_MAX_FILE_LEN+1)*4);
                    if (ui4_path_length >= (FM_MAX_FILE_LEN+1)*4)
                    {
                        ui4_path_length = (FM_MAX_FILE_LEN+1)*4;
                    }
                    x_memcpy(ps_file_name, ps_dir_buf, ui4_path_length);
                    x_strcat(ps_file_name, t_dir_entr.s_name);
                    ac_extension = x_strrchr(ps_file_name, '.');
                    if (ac_extension == NULL)
                    {
                        continue;
                    }
                    ac_extension ++;
                    x_strncpy(s_extension, ac_extension, MAX_EXTENSION_NAME_LEN);
                    ps_buf = x_strstr(ps_ext_list , x_str_toupper(s_extension));
                    if (ps_buf == NULL)
                    {
                        ps_buf = x_strstr(ps_ext_list , x_str_tolower(s_extension));
                        if (ps_buf == NULL)
                        {
                            continue;
                        }
                    }
                    
                    x_memset(pt_sbtl_lang_desc->pc_related_file_name,0,(FM_MAX_FILE_LEN+1)*4);
                    pt_sbtl_lang_desc->pc_sbtl_file_name=  NULL;
                    pt_sbtl_lang_desc->ui1_lang_count   = 0;
                    pt_sbtl_lang_desc->pc_sbtl_file_name= ps_file_name;

                    i4_ret = _svctx_get_ext_sbtl_info(  pt_media_desc,
                                                        pt_sbtl_lang_desc);
                    if(i4_ret != SVCTXR_OK)
                    {
                        break;
                    }
                }while(TRUE);
                
                i4_ret = x_fm_close(h_dir_hdl);
                if (FMR_OK != i4_ret)
                {
                    x_dbg_stmt("x_fm_close() returns %d\n", i4_ret);
                    ASSERT(FALSE, ("%s(), x_fm_close() failed!\r\n", __func__));
                    i4_chk_ret = SVCTXR_FAIL;
                }
                if (i4_chk_ret != SVCTXR_OK)
                {
                    break;
                }
            }
        }while(0);
        
        if (ps_dir_buf != NULL)
        {
            x_mem_free(ps_dir_buf);
            ps_dir_buf = NULL;
        }
        if (ps_filename_no_exten != NULL)
        {
            x_mem_free(ps_filename_no_exten);
            ps_filename_no_exten = NULL;
        }
        if (ps_file_name != NULL)
        {
            x_mem_free(ps_file_name);
            ps_file_name = NULL;
        }
        if (pt_sbtl_lang_desc != NULL &&
            pt_sbtl_lang_desc->pc_related_file_name != NULL)
        {
            x_mem_free(pt_sbtl_lang_desc->pc_related_file_name);
            pt_sbtl_lang_desc->pc_related_file_name = NULL;
        }
        if (pt_sbtl_lang_desc != NULL)
        {
            x_mem_free(pt_sbtl_lang_desc);
            pt_sbtl_lang_desc = NULL;
        }
    }
    
    return i4_chk_ret;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_handle_media_format_recognize
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
static INT32 _svctx_handle_media_format_recognize(
                                            MEDIA_FORMAT_T          t_format,
                                            MEDIA_DESCRIBT_T*       pt_media_desc)
{
    MM_STRM_INFO_T*                     pt_strm_info = NULL;
    INT32                               i4_ret = 0;

    if (pt_media_desc == NULL)
    {
        return SVCTXR_INV_ARG;
    }
    
    pt_strm_info = pt_media_desc->pt_strm_info;
    /* media format validation checking */
    if (!MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt))
    {
#ifdef SYS_RECORD_SUPPORT
        BOOL b_is_pvr_file = FALSE;
        if(SVCTXR_OK == svctx_rec_check_pvr_file(pt_media_desc->ps_media_path, 
                                                &b_is_pvr_file))
        {
            /*do nothing*/
        }
        if(b_is_pvr_file)
        {
            t_format.e_media_type = MEDIA_TYPE_TS;
            t_format.t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_192_ENCRYPTION;
            pt_media_desc->e_src_type = SRC_TYPE_MEDIA_PVR;
            pt_media_desc->t_media_fmt = t_format;
#ifdef MM_LAST_MEMORY_SUPPORT
            if (pt_media_desc->t_video_thumbnail_info.h_surf == NULL_HANDLE)
            {
                i4_ret = _svctx_get_mm_identifier(pt_media_desc);
            }
#endif
        }
        else
#endif
        /* media format validation checking */
        if (pt_strm_info == NULL)
        {
            DBG_INFO((_INFO_HEADER"file name : %s !\n",pt_media_desc->ps_media_path));
            if (pt_media_desc->b_non_seekable != TRUE) /*Willy*/
            {
                if((pt_media_desc->e_svc_protocol == PROTOCL_MODE_URI ||
                    pt_media_desc->e_svc_protocol == PROTOCL_MODE_PULL)
                    && (pt_media_desc->e_src_type == SRC_TYPE_MEDIA_NETWORK))
                {
                    i4_ret = _svctx_get_favor_format_by_ext(pt_media_desc, &t_format);
                    if(i4_ret != SVCTXR_OK)
                    {
                        t_format.e_media_type = MEDIA_TYPE_TS;
                    }
#ifdef ENABLE_MMS_SUPPORT
                    if(pt_media_desc->pc_mms_header_buf && pt_media_desc->ui4_mms_hdr_sz)
                    {
                        i4_ret = x_mfmtrecg_get_mem_fmt ( 
                                                     pt_media_desc->pc_mms_header_buf, 
                                                     pt_media_desc->ui4_mms_hdr_sz,   
                                                     &t_format);
                    }
                    else
#endif
                    {

                        i4_ret = x_mfmtrecg_get_mem_fmt ( 
                                                     pt_media_desc->pt_media_feeder, 
                                                     0,   
                                                     &t_format);
                        x_dbg_stmt("x_mfmtrecg_get_mem_fmt : ret = %d !\n",i4_ret);
                    }            
                }            
                else 
                {
                    if((pt_media_desc->e_play_mode == MM_PLAY_MODE_PREV) &&
                       (pt_media_desc->e_src_type == SRC_TYPE_MEDIA_NETWORK))
                    {
                        t_format.b_fav_recg_only = TRUE;
                    }
                    i4_ret = x_mfmtrecg_get_file_fmt(pt_media_desc->ps_media_path, &t_format);
                }
                if((i4_ret != MFMTRECGR_OK)
                    || ( (t_format.e_media_type == MEDIA_TYPE_CONTAINER)
                         && (0
    #ifdef MMP_AVI_NOT_SUPPORT
                            ||(t_format.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_AVI)
    #endif
    #ifdef MMP_ASF_NOT_SUPPORT
                            ||(t_format.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_ASF)
    #endif
    #ifdef MMP_MP4_NOT_SUPPORT
                            ||(t_format.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MP4)
    #endif
    #ifdef MMP_MKV_NOT_SUPPORT
                            ||(t_format.t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MATROSKA)
    #endif
                            )))
                {
                    if (i4_ret == FMR_NO_ENTRY)
                    {
                        i4_ret = SVCTXR_INV_ENTRY;
                    }
                    else if (i4_ret == MFMTRECGR_OUT_OF_MEMORY)
                    {
                        ASSERT(FALSE,("mem alloc fail ! %s line(%d)", __func__, __LINE__));
                    }
                    else
                    {
                        i4_ret = SVCTXR_NOT_SUPPORT;
                    }
                    return i4_ret;
                }
            }

             if (pt_media_desc->e_src_sub_type == SRC_SUB_TYPE_VESTEL_TS)
             {
                x_dbg_stmt("[MM_SVCTX]open the vestel ts ,set MEDIA_TS_SUBTYPE_TS_192_ENCRYPTION.\r\n");
                t_format.e_media_type = MEDIA_TYPE_TS;
                t_format.t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_192_ENCRYPTION;
             }
            _print_file_format(t_format);
            pt_media_desc->t_media_fmt = t_format;
#ifdef MM_LAST_MEMORY_SUPPORT
            if (pt_media_desc->t_video_thumbnail_info.h_surf == NULL_HANDLE)
            {
                i4_ret = _svctx_get_mm_identifier(pt_media_desc);
            }
#endif
        }
        else
        {
            _print_file_format(pt_strm_info->t_format);
            /* check stream info provided by AP is valid or NOT */
            if ((pt_strm_info->t_format.e_media_type == MEDIA_TYPE_AUDIO_ES)  &&
                 ((pt_strm_info->t_format.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_LPCM) ||
                  (pt_strm_info->t_format.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_WAVE) ||
                  (pt_strm_info->t_format.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_AIF)))
            {    /*add for wave support*/
                if (pt_strm_info->at_stm_atrbt[0].t_stm_atrbt.u.t_es_audio_stm_attr.ui4_sampling_frequency == 0)
                {
                    i4_ret = SVCTXR_NOT_SUPPORT;
                    return i4_ret;
                }
            }
            pt_media_desc->t_media_fmt = pt_strm_info->t_format;
        }
    }

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_handle_media_minfo_parse
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
static INT32 _svctx_handle_media_minfo_parse(
                                            HANDLE_T                h_svctx,
                                            VOID*                   pv_tag,
                                            BOOL                    b_has_minfo,
                                            MEDIA_DESCRIBT_T*       pt_media_desc
                                            )
{
    INT32               i4_ret = 0;    
    
    if (pt_media_desc == NULL)
    {
        return SVCTXR_INV_ARG;
    }
    
    if((MM_HDLR_WITH_MINFO(pt_media_desc)|| b_has_minfo) && 
       (pt_media_desc->pt_meta_info == NULL) /*add for common API*/)
    {
        if((pt_media_desc->e_svc_protocol == PROTOCL_MODE_URI ||
            pt_media_desc->e_svc_protocol == PROTOCL_MODE_PULL)
            && (pt_media_desc->e_src_type == SRC_TYPE_MEDIA_NETWORK))
        {
#ifdef ENABLE_MMS_SUPPORT
            if(pt_media_desc->pc_mms_header_buf && pt_media_desc->ui4_mms_hdr_sz)
            {
                pt_media_desc->t_media_fmt.b_only_has_hdr = TRUE;
                i4_ret = x_minfo_mopen(pt_media_desc->pc_mms_header_buf,
                                      pt_media_desc->ui4_mms_hdr_sz,
                                      (x_minfo_nfy_fct)_svctx_media_minfo_nty,
                                      (VOID*)pt_media_desc->h_media_desc,
                                      &(pt_media_desc->t_media_fmt),
                                      &(pt_media_desc->h_media_info));
            }
            else
#endif
            {
                i4_ret = x_minfo_mopen(pt_media_desc->pt_media_feeder,
                                  0,
                                  (x_minfo_nfy_fct)_svctx_media_minfo_nty,
                                  (VOID*)pt_media_desc->h_media_desc,
                                  &(pt_media_desc->t_media_fmt),
                                  &(pt_media_desc->h_media_info));
            }
        }
        else
        {
            i4_ret = x_minfo_fopen(pt_media_desc->ps_media_path,
                                  (x_minfo_nfy_fct)_svctx_media_minfo_nty,
                                  (VOID*)pt_media_desc->h_media_desc,
                                  &(pt_media_desc->t_media_fmt),
                                  &(pt_media_desc->h_media_info));
        }

        if(i4_ret != MINFOR_OK)
        {
            if (i4_ret == MINFOR_OUT_OF_MEMORY)
            {
                ASSERT(FALSE,("mem alloc fail ! %s line(%d)", __func__, __LINE__));
            }
            i4_ret = SVCTXR_FAIL;
            return i4_ret;
        }
    }
#ifdef TIME_SHIFT_SUPPORT
    else if (MM_IS_TIME_SHIFT(pt_media_desc->t_media_fmt))
    {
        REC_UTIL_COND_T e_cond;

        pt_media_desc->ps_rec_db = SN_MAIN_REC_UTIL_TS;
        i4_ret = rec_util_open(pt_media_desc->ps_rec_db,
                               (rec_util_nfy_fct)_svctx_media_rec_util_nfy_fct,
                               (VOID*)pt_media_desc,
                               &(pt_media_desc->h_rec_util));
        if (i4_ret != REC_UTILR_OK || pt_media_desc->h_rec_util == NULL_HANDLE)
        {
            ASSERT(FALSE, ("%s(), rec_util_open() failed!\r\n", __func__));
        }

        i4_ret = rec_util_get_cond(pt_media_desc->h_rec_util, &e_cond);
        if (i4_ret == REC_UTILR_OK)
        {
            if (e_cond == REC_UTIL_COND_READY)
            {
                REC_UTIL_SHARE_DATA_T t_rec_share_data;

                i4_ret = rec_util_get_shared_data(pt_media_desc->h_rec_util,
                                                  &t_rec_share_data);
                if (i4_ret != REC_UTILR_OK)
                {
                    ASSERT(FALSE, ("%s(), rec_util_get_shared_data() failed!\r\n", __func__));
                    return SVCTXR_FAIL;
                }
                pt_media_desc->t_rec_tick_ctrl_blk = t_rec_share_data.t_tick_cb;

                pt_media_desc->ps_media_path = (CHAR*)x_mem_alloc(x_strlen(t_rec_share_data.ps_strg_path) + 1);
                if (pt_media_desc->ps_media_path == NULL)
                {
                    return SVCTXR_NO_RESOURCE;
                }
                x_strcpy(pt_media_desc->ps_media_path, t_rec_share_data.ps_strg_path);

                pt_media_desc->pf_nfy(h_svctx,
                        SVCTX_COND_PENDING,
                        SVCTX_NTFY_CODE_MEDIA_PRE_PROCESSED,
                        ST_UNKNOWN,
                        pv_tag,
                        (VOID*)pt_media_desc->h_media_desc);
                x_dbg_stmt(_INFO_HEADER"%s(), rec_util is ready!\r\n", __func__);
            }
            else
            {
                x_dbg_stmt(_INFO_HEADER"%s(), rec_util is NOT ready!\r\n", __func__);
            }
        }
        else
        {
            ASSERT(FALSE, ("%s(), rec_util_get_cond() failed!\r\n", __func__));
        }
    }
#endif /* TIME_SHIFT_SUPPORT */
    else
    {
        x_timer_create(&(pt_media_desc->h_minfo_delay_timer));

        if (pt_media_desc->h_minfo_delay_timer != NULL_HANDLE)
        {
            x_timer_start(
                    pt_media_desc->h_minfo_delay_timer,
                    DEFAULT_FAKE_MINFO_TIME_OUT,
                    X_TIMER_FLAG_ONCE,
                    _svctx_fake_minfo_cb,
                    (VOID*)(pt_media_desc->h_media_desc)
                    );

        }
        else
        {
            return SVCTXR_FAIL;
        }
    }

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      x_svctx_media_open
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
INT32 x_svctx_media_open(
                    HANDLE_T                        h_svctx,
                    MM_SVC_COMMAND_T*               pt_cmd,
                    x_svctx_media_prc_nfy_fct       pf_nfy,
                    VOID*                           pv_tag,
                    HANDLE_T*                       ph_media
                    )
{
    INT32                            i4_ret        = SVCTXR_ASYNC_NFY;
    MEDIA_DESCRIBT_T*                pt_media_desc = NULL;
    MEDIA_FORMAT_T                   t_format;
    /*add for common API*/
    BOOL                            b_has_minfo = FALSE;
    BOOL                            b_need_feeder = FALSE;
#ifdef INET_SUPPORT
    UINT8*                          paui1_rsp_hdr = NULL;
#endif

    x_dbg_stmt(_INFO_HEADER"x_svctx_media_open\n");

    if (pt_cmd == NULL || pf_nfy == NULL)
    {
        return SVCTXR_INV_ARG;
    }

    if (svctx_is_init() == FALSE) 
    {
        return SVCTXR_NOT_INIT;
        }

    pt_media_desc =  x_mem_alloc(sizeof(MEDIA_DESCRIBT_T));
    if(pt_media_desc == NULL)
    {
       i4_ret = SVCTXR_NO_RESOURCE;
       goto EXIT;
    }
    x_memset(pt_media_desc,0,sizeof(MEDIA_DESCRIBT_T));
    x_memset(&t_format,    0,sizeof(MEDIA_FORMAT_T));
    pt_media_desc->e_src_type = SRC_TYPE_MEDIA_STORGE;
    
    /* parse command*/
    i4_ret = _svctx_handle_media_cmd(pt_cmd,pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        goto EXIT;
    }

    pt_media_desc->e_priority = MM_PRIORITY_DEFAULT;
    pt_media_desc->h_svctx = h_svctx;
            
    if ( pt_media_desc->e_src_type == SRC_TYPE_MEDIA_STORGE )//subtype not support now
    {
        pt_media_desc->b_non_seekable = FALSE; 
    }
    else if ( pt_media_desc->e_src_type == SRC_TYPE_MEDIA_NETWORK )
    {
        /*common code*/
        /******/
        if ( pt_media_desc->e_svc_protocol == PROTOCL_MODE_DLNA )     
        {
            pt_media_desc->b_non_seekable = FALSE;
        }
        else if ( pt_media_desc->e_svc_protocol == PROTOCL_MODE_PULL )
        {
            pt_media_desc->b_non_seekable = FALSE;
            b_need_feeder = TRUE;
        }
        else if ( pt_media_desc->e_svc_protocol == PROTOCL_MODE_PUSH )
        {
            pt_media_desc->b_non_seekable = TRUE; 
        }
        else if ( pt_media_desc->e_svc_protocol == PROTOCL_MODE_URI )
        {
            if (x_strstr(pt_media_desc->ps_media_path, "http") && x_strstr(pt_media_desc->ps_media_path, "pandora.com"))
            {
#ifdef INET_SUPPORT
                FM_FILE_INFO_T t_info;                

                i4_ret = x_httpc_fm_open_proxy_agent_timeout_cookie(
                                            pt_media_desc->ps_media_path, 
                                            pt_media_desc->ps_proxy_url, 
                                            pt_media_desc->ps_agent_url, 
                                            0,
#ifdef SINGLE_URI_HANDLE
                                            0x800000, 
#else
                                            128*1024,
#endif
                                            &pt_media_desc->h_uri,
                                            pt_media_desc->ps_cookie_url);
                if (FMR_OK != i4_ret)
                {
                    i4_ret = SVCTXR_FAIL;
                    pt_media_desc->h_uri = NULL_HANDLE;
                    goto EXIT;
                }

                i4_ret = x_httpc_fm_get_info_by_handle(pt_media_desc->h_uri, &t_info);
                if (FMR_OK != i4_ret)
                {
                    i4_ret = SVCTXR_FAIL;
                    goto EXIT;
                }

                pt_media_desc->ui8_media_sz = t_info.ui8_size;
#ifdef SINGLE_URI_HANDLE
                pt_media_desc->h_uri_cm = pt_media_desc->h_uri;
#else
                pt_media_desc->h_uri_cm = NULL_HANDLE;
#endif
                t_format.e_media_type = MEDIA_TYPE_AUDIO_ES;
                t_format.t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_MP3;
                pt_media_desc->b_non_seekable = TRUE; 
                pt_media_desc->t_media_fmt = t_format;
                b_has_minfo = TRUE;
#else
                pt_media_desc->b_non_seekable = FALSE;
#endif
            }
            else if ((x_strstr(pt_media_desc->ps_media_path, "http") || x_strstr(pt_media_desc->ps_media_path, "https"))//(fm radio)
                  && (pt_media_desc->e_svc_protocol == PROTOCL_MODE_URI))
            {
#ifdef INET_SUPPORT
                UINT32  ui4_len = 1024;
                FM_FILE_INFO_T t_info;

                paui1_rsp_hdr = x_mem_alloc(sizeof(UINT8)*1024);
                if(paui1_rsp_hdr == NULL)
                {
                    i4_ret = SVCTXR_FAIL;
                    goto EXIT;
                }

                i4_ret = x_httpc_fm_open_proxy_agent_timeout_cookie(
                                            pt_media_desc->ps_media_path, 
                                            pt_media_desc->ps_proxy_url, 
                                            pt_media_desc->ps_agent_url, 
                                            0,
                                            0x800000, 
                                            &pt_media_desc->h_uri,
                                            pt_media_desc->ps_cookie_url);
                if (FMR_OK != i4_ret)
                {
                    i4_ret = SVCTXR_TIMEOUT;//SVCTXR_FAIL;  //open file timeout in media open
                    pt_media_desc->h_uri = NULL_HANDLE;
                    goto EXIT;
                }

                //////notify http file handle to cmpb so that cmpb can cancel this file reading/////
                pf_nfy( h_svctx,
                    SVCTX_COND_PENDING,
                    SVCTX_NTFY_CODE_MEDIA_URI_FILE_OPENED,
                    ST_UNKNOWN,
                    pv_tag,
                    (VOID*)pt_media_desc->h_uri);
                ///////////////////////end of notify///////////////
                x_dbg_stmt("x_svctx_media_open(), create feeder ok, handle = %x \n", pt_media_desc->h_uri);

                i4_ret = x_httpc_fm_get_info_by_handle(pt_media_desc->h_uri, &t_info);
                if (FMR_OK != i4_ret)
                {
                    i4_ret = SVCTXR_FAIL;
                    goto EXIT;
                }

                pt_media_desc->ui8_media_sz = t_info.ui8_size;
#ifdef SINGLE_URI_HANDLE
                pt_media_desc->h_uri_cm = pt_media_desc->h_uri;
#else
                pt_media_desc->h_uri_cm = NULL_HANDLE;
#endif
                if (t_info.ui8_size == (UINT64)(-1))
                {
                    x_dbg_stmt(_INFO_HEADER"!!!!! t_info.ui8_size=-1 \n");
                    i4_ret = x_httpc_fm_get_rsp_hdr(pt_media_desc->h_uri, (CHAR *)paui1_rsp_hdr, &ui4_len);
                    if (FMR_OK != i4_ret)
                    {
                        i4_ret = SVCTXR_FAIL;
                        goto EXIT;
                    }

                    if (x_strstr((const CHAR*)paui1_rsp_hdr, "audio/aac"))
                    {
                        pt_media_desc->b_non_seekable = TRUE;
                        t_format.e_media_type = MEDIA_TYPE_AUDIO_ES;
                        t_format.t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_AAC;
                        b_has_minfo = TRUE;
                    }
					#if 0
                    else if (x_strstr((const CHAR*)paui1_rsp_hdr, "audio/mpeg"))
                    {
                        t_format.e_media_type = MEDIA_TYPE_AUDIO_ES;
                        t_format.t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_MP3;
                        pt_media_desc->b_non_seekable = TRUE; 
                        b_has_minfo = TRUE;
                    }
					#endif
                    else
                    {
                        x_dbg_stmt(_INFO_HEADER"!!!!! svctx not handle non seekable file format\n");
                        pt_media_desc->b_non_seekable = FALSE;
                    }
                }
                else
                {
                    x_dbg_stmt(_INFO_HEADER"!!!!! t_info.ui8_size=%d\n", (UINT32)(t_info.ui8_size));
                    i4_ret = x_httpc_fm_get_rsp_hdr(pt_media_desc->h_uri, (CHAR *)paui1_rsp_hdr, &ui4_len);
                    if (FMR_OK != i4_ret)
                    {
                        i4_ret = SVCTXR_FAIL;
                        goto EXIT;
                    }
                    if (x_strstr((const CHAR*)paui1_rsp_hdr, "audio/aac"))
                    {
                        pt_media_desc->b_non_seekable = TRUE;
                        t_format.e_media_type = MEDIA_TYPE_AUDIO_ES;
                        t_format.t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_AAC;
                                b_has_minfo = TRUE;
                    }
					#if 0
                    else if (x_strstr((const CHAR*)paui1_rsp_hdr, "audio/mpeg"))
                    {
                        t_format.e_media_type = MEDIA_TYPE_AUDIO_ES;
                        t_format.t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_MP3;
                        pt_media_desc->b_non_seekable = TRUE; 
                                b_has_minfo = TRUE;
                    }
                    else if (x_strstr((const CHAR*)pt_media_desc->ps_media_path, ".mp3"))
                    {
                        t_format.e_media_type = MEDIA_TYPE_AUDIO_ES;
                        t_format.t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_MP3;
                        pt_media_desc->b_non_seekable = TRUE; 
                                b_has_minfo = TRUE;
                    }
					#endif
                    else if (x_strstr((const CHAR*)pt_media_desc->ps_media_path, ".aac"))
                    {
                        pt_media_desc->b_non_seekable = TRUE;
                        t_format.e_media_type = MEDIA_TYPE_AUDIO_ES;
                        t_format.t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_AAC;
                                b_has_minfo = TRUE;
                    }
                }
#else
                pt_media_desc->b_non_seekable = FALSE;
#endif
            }
#ifdef ENABLE_MMS_SUPPORT
            else if ((x_strncasecmp(pt_media_desc->ps_media_path, "mms://", 6) == 0) ||
                 (x_strncasecmp(pt_media_desc->ps_media_path, "mmst://", 7) == 0))
            {
                UINT32 ui4_file_len;

                t_format.e_media_type = MEDIA_TYPE_CONTAINER;
                t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_ASF;
                        b_has_minfo = TRUE;
                if(SVCTXR_OK != _svctx_get_mms_header(pt_media_desc->ps_media_path, &pt_media_desc->pc_mms_header_buf, 
                                                      &pt_media_desc->h_mms_handle, &pt_media_desc->ui4_mms_hdr_sz))
                {
                    i4_ret = SVCTXR_FAIL;
                    goto EXIT;
                }

                ui4_file_len = 0;
                if(MMS_ERROR_CODE_OK != x_mms_fm_pull_get_lengeth(pt_media_desc->h_mms_handle, NULL, &ui4_file_len))
                {
                    x_dbg_stmt(_ERROR_HEADER"get mms length error!!!!\n");
                    i4_ret = SVCTXR_FAIL;
                    goto EXIT;
                }
                pt_media_desc->ui8_media_sz = ui4_file_len;

                //pt_media_desc->h_uri = pt_media_desc->h_mms_handle;

                //release file handle now...
                _svctx_release_mms_header(pt_media_desc->h_mms_handle, NULL);
                pt_media_desc->h_mms_handle = (MMS_HANDLE_T)0;
            }
#endif
            b_need_feeder = TRUE;
        }
        else   //network space
        {
            pt_media_desc->b_non_seekable = FALSE;
        }

        if ( pt_media_desc->e_src_sub_type == SRC_SUB_TYPE_NETWORK_MP3_ONLY )
        {
                t_format.e_media_type = MEDIA_TYPE_AUDIO_ES;
                t_format.t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_MP3;
                pt_media_desc->b_non_seekable = TRUE; 
                pt_media_desc->t_media_fmt = t_format;
                b_has_minfo = TRUE;
                b_need_feeder = TRUE;
        }  
        else if (pt_media_desc->e_src_sub_type == SRC_SUB_TYPE_VESTEL_TS)
        {
            t_format.e_media_type = MEDIA_TYPE_TS;
            t_format.t_media_subtype.u.e_aud_subtype = MEDIA_TS_SUBTYPE_TS_188;
            pt_media_desc->b_non_seekable = FALSE; 
            pt_media_desc->t_media_fmt = t_format;
            b_has_minfo = FALSE;
        }
    }
    else if ( pt_media_desc->e_src_type == SRC_TYPE_MEDIA_MEM_CLIP )
    {
        if ( pt_media_desc->e_svc_protocol != PROTOCL_MODE_UNKNOWN )
        {
             ASSERT(FALSE,("protocol type error"));
        }  
        pt_media_desc->b_non_seekable = FALSE;   
    }
    else if ( pt_media_desc->e_src_type == SRC_TYPE_MEDIA_TIME_SHIFT )
    {
        if ( pt_media_desc->e_svc_protocol != PROTOCL_MODE_UNKNOWN )
        {
            ASSERT(FALSE,("protocol type error"));
        }
#ifdef TIME_SHIFT_SUPPORT
        t_format.e_media_type = MEDIA_TYPE_TS;
        t_format.t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_TIME_SHIFT;
        pt_media_desc->t_media_fmt = t_format;  
#endif /* TIME_SHIFT_SUPPORT */
        pt_media_desc->b_non_seekable = FALSE;    
    }
    else if ( pt_media_desc->e_src_type == SRC_TYPE_MEDIA_PVR )
    {
        if ( pt_media_desc->e_svc_protocol != PROTOCL_MODE_UNKNOWN )
        {
            ASSERT(FALSE,("protocol type error"));
        }
            pt_media_desc->b_non_seekable = FALSE;   
    }
    else
    {
        ASSERT(FALSE,("source type error"));
    }

    if(b_need_feeder)
    {
        if ((!pt_media_desc->b_non_seekable) || b_has_minfo)
        {
            if(pt_media_desc->e_svc_protocol == PROTOCL_MODE_URI
#ifdef ENABLE_MMS_SUPPORT
            && pt_media_desc->pc_mms_header_buf == NULL
#endif
              )
            {
                i4_ret = _svctx_create_uri_media_feeder_instace(pt_media_desc);
            }
            else if(pt_media_desc->e_svc_protocol == PROTOCL_MODE_PULL &&
                pt_media_desc->pt_strm_info == NULL /*it's not app master*/)
            {
                i4_ret = _svctx_create_pull_media_feeder_instace(pt_media_desc);
            }
            if(i4_ret != SVCTXR_OK)
            {
                if (pt_media_desc->h_uri)
                {
#ifdef INET_SUPPORT
                    x_httpc_fm_close(pt_media_desc->h_uri);
#endif
                    pt_media_desc->h_uri = NULL_HANDLE;
                }
                goto EXIT;
            }
            pt_media_desc->h_uri = NULL_HANDLE;
        }
        else if (pt_media_desc->h_uri)
        {
#ifdef INET_SUPPORT
            x_httpc_fm_close(pt_media_desc->h_uri);
#endif
            pt_media_desc->h_uri = NULL_HANDLE;
        }
    }

    i4_ret = handle_alloc(  HT_MEDIA_STORGE_ELM_TYPE,
                            pt_media_desc,
                            NULL,
                            _svctx_media_handle_free_fct,
                            ph_media);
    if(i4_ret!=HR_OK)
    {
        i4_ret = SVCTXR_FAIL;
        goto EXIT;
    }
    pt_media_desc->h_media_desc = *ph_media;
    
    //////notify MEDIA DECR HANDLE to cmpb so that cmpb can cancel this file reading in file recognizings/////
    pf_nfy( h_svctx,
        SVCTX_COND_PENDING,
        SVCTX_NTFY_CODE_MEDIA_DESC_ALLOCED,
        ST_UNKNOWN,
        pv_tag,
        (VOID*)*ph_media);   
    ///////////////////////end of notify///////////////
    
    /* format recognizer */
    i4_ret = _svctx_handle_media_format_recognize(t_format, pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        x_dbg_stmt(_ERROR_HEADER"meida format recognizer error, i4_ret = %d !!\n",i4_ret);
        goto EXIT;
    }
    
    /* lyric info*/
    i4_ret = _svctx_handle_lyric_info(pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
            
    }
    /* external subtitle */
    i4_ret = _svctx_handle_extra_subtl(h_svctx,pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        
    }
    
    pt_media_desc->pf_nfy = pf_nfy;
    pt_media_desc->pv_tag = pv_tag;
    x_memset(&pt_media_desc->t_video_thumbnail_info,0,sizeof(SVCTX_VIDEO_THUMBNAIL_INFO_T));

    pt_media_desc->ui2_svl_id = SVL_NULL_ID;
    pt_media_desc->ui2_svl_rec_id = SVL_NULL_REC_ID;
    pt_media_desc->ui2_svc_id = 0;
    pt_media_desc->ui2_svc_pid = 0;
    pt_media_desc->ui2_pmt_idx = 0;
    i4_ret = x_sema_create(
                    &pt_media_desc->h_minfo_wait_sema,
                    X_SEMA_TYPE_BINARY,
                    X_SEMA_STATE_LOCK
                    );
    if(i4_ret != OSR_OK)
    {
        i4_ret = SVCTXR_FAIL;
        goto EXIT;
    }

    /* parsing media header information */
    pf_nfy( h_svctx,
            SVCTX_COND_PENDING,
            SVCTX_NTFY_CODE_MEDIA_PRE_PROCESSING,
            ST_UNKNOWN,
            pv_tag,
            (VOID*)pt_media_desc->h_media_desc);

    i4_ret = _svctx_handle_media_minfo_parse(h_svctx, pv_tag, b_has_minfo, pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        x_dbg_stmt(_ERROR_HEADER"media minfo parse open error!!\n");
        goto EXIT;
    }
    
EXIT:

#ifdef INET_SUPPORT
    if (paui1_rsp_hdr != NULL)
        {
        x_mem_free(paui1_rsp_hdr);
        paui1_rsp_hdr = NULL;
        }
#endif
    if(i4_ret != SVCTXR_OK)
            {
        _svctx_media_descrp_free(pt_media_desc);
    }
    
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      x_svctx_media_close
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
INT32 x_svctx_media_close(
                    HANDLE_T                        h_svctx,
                    HANDLE_T                        h_media
                    )
{
    INT32                         i4_ret = 0;
    MEDIA_DESCRIBT_T*             pt_media_desc = NULL;
    SVCTX_T*                      pt_svctx = NULL;
    MM_SVC_REQ_T*                 pt_running_svc_req = NULL;
    UINT32                        ui4_handle_num = 0;

    x_dbg_stmt(_INFO_HEADER"x_svctx_media_close\n");
    
    i4_ret = svctx_get_obj(h_svctx,&pt_svctx);
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }

    i4_ret = _mm_hdlr_get_media_obj(h_media,&pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }

    pt_running_svc_req     = (MM_SVC_REQ_T*)(pt_svctx->pt_running_svc_req);
    if ((pt_running_svc_req != NULL) && (pt_running_svc_req->h_media_desc == h_media))
    {
        x_sema_lock(pt_media_desc->h_minfo_wait_sema, X_SEMA_OPTION_WAIT);
    }
    
#ifdef TIME_SHIFT_SUPPORT
    if (x_handle_valid(pt_media_desc->h_rec_util))
    {
        i4_ret = rec_util_close(pt_media_desc->h_rec_util);
        if (i4_ret != REC_UTILR_OK)
        {
            DBG_ERROR((_ERROR_HEADER"%s: rec_util_close() failed, code = %d\r\n", __FUNCTION__, i4_ret));
        }
        pt_media_desc->h_rec_util = NULL_HANDLE;
    }
#endif /* TIME_SHIFT_SUPPORT */

#ifdef ENABLE_DIVX_DRM
    if (x_handle_valid(pt_media_desc->h_drm))
    {
        i4_ret = x_drm_close (pt_media_desc->h_drm);
        if (i4_ret != DRMR_OK)
        {
            DBG_ERROR((_ERROR_HEADER"%s: x_drm_close failed, code = %d\r\n", __FUNCTION__, i4_ret));
        }
        pt_media_desc->t_drm_type = DRM_TYPE_NONE;
        pt_media_desc->h_drm = NULL_HANDLE;
    }
#endif

    if(x_handle_valid(pt_media_desc->h_media_info))
    {
        i4_ret = x_minfo_close(pt_media_desc->h_media_info);
        if(i4_ret != MINFOR_OK)
        {
            return SVCTXR_FAIL;
        }
    }

    /* close extra subtitle file handle */
    do
    {
        if (x_handle_valid(pt_media_desc->t_mm_svctx_sbtl_desc[ui4_handle_num].h_ext_subtitle))
        {
            if(pt_media_desc->b_is_ext_sbtl_http_file)
            {
#ifdef INET_SUPPORT
                i4_ret = x_httpc_fm_close(pt_media_desc->t_mm_svctx_sbtl_desc[ui4_handle_num].h_ext_subtitle);
#endif
            }
            else
            {
#ifndef ANDROID
                i4_ret = x_fm_close(pt_media_desc->t_mm_svctx_sbtl_desc[ui4_handle_num].h_ext_subtitle);
#else
                i4_ret = fclose((FILE*)(pt_media_desc->t_mm_svctx_sbtl_desc[ui4_handle_num].h_ext_subtitle));
#endif
            }
            if(i4_ret != FMR_OK)
            {
                return SVCTXR_FAIL;
            }
            pt_media_desc->t_mm_svctx_sbtl_desc[ui4_handle_num].h_ext_subtitle = NULL_HANDLE;
            
            if (x_handle_valid(pt_media_desc->t_mm_svctx_sbtl_desc[ui4_handle_num].h_related_subtitle))
            {
                if(pt_media_desc->b_is_ext_sbtl_http_file)
                {
#ifdef INET_SUPPORT
                    i4_ret = x_httpc_fm_close(pt_media_desc->t_mm_svctx_sbtl_desc[ui4_handle_num].h_related_subtitle);
#endif
                }
                else
                {
#ifndef ANDROID
                    i4_ret = x_fm_close(pt_media_desc->t_mm_svctx_sbtl_desc[ui4_handle_num].h_related_subtitle);
#else
                    i4_ret = fclose((FILE*)(pt_media_desc->t_mm_svctx_sbtl_desc[ui4_handle_num].h_related_subtitle));
#endif
                }
                if (i4_ret != FMR_OK)
                {
                    return SVCTXR_FAIL;
                }
                pt_media_desc->t_mm_svctx_sbtl_desc[ui4_handle_num].h_related_subtitle = NULL_HANDLE;
            }
        }
        ui4_handle_num++;
    } while(ui4_handle_num < 8);

    if (x_handle_valid(pt_media_desc->h_lyric))
    {
        if(pt_media_desc->b_is_ext_sbtl_http_file)
        {
#ifdef INET_SUPPORT
            i4_ret = x_httpc_fm_close(pt_media_desc->h_lyric);
#endif
        }
        else
        {
#ifndef ANDROID
            i4_ret = x_fm_close(pt_media_desc->h_lyric);
#else
            i4_ret = fclose((FILE*)(pt_media_desc->h_lyric));
#endif
        }
    }
            
    if (x_handle_valid(pt_media_desc->h_minfo_delay_timer))
    {
        i4_ret = x_timer_delete(pt_media_desc->h_minfo_delay_timer);
        if (i4_ret != OSR_OK)
        {
            return SVCTXR_FAIL;
        }
    }

    /* free mem */
    _svctx_media_descrp_free(pt_media_desc);
    
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_media_descrp_free
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
static INT32 _svctx_media_descrp_free(MEDIA_DESCRIBT_T* pt_media_desc)
{
    INT32                           i4_ret = 0;
    FONT_INFO_T*                    pt_cur_font = NULL;
    FONT_INFO_T*                    pt_next_font = NULL;

    if(pt_media_desc == NULL)
    {
        return i4_ret;
    }
    _svctx_close_media_feeder_instace(pt_media_desc);

    if (pt_media_desc->h_uri)
    {
#ifdef INET_SUPPORT
        x_httpc_fm_close(pt_media_desc->h_uri);
#endif
        pt_media_desc->h_uri = NULL_HANDLE;
    }

    if (pt_media_desc->ps_media_path != NULL)
    {
        x_mem_free((VOID*)pt_media_desc->ps_media_path);
        pt_media_desc->ps_media_path = NULL;
    }
    /* free extra subtitle */
    if(pt_media_desc->ps_ext_full_path != NULL)
    {
        x_mem_free(pt_media_desc->ps_ext_full_path);
        pt_media_desc->ps_ext_full_path = NULL;
    }
    if(pt_media_desc->pt_ext_sbtl_info != NULL)
    {
        if(pt_media_desc->pt_ext_sbtl_info->ps_prf_lang_lst != NULL)
        {
            x_mem_free(pt_media_desc->pt_ext_sbtl_info->ps_prf_lang_lst);
            pt_media_desc->pt_ext_sbtl_info->ps_prf_lang_lst = NULL;
        }
        x_mem_free(pt_media_desc->pt_ext_sbtl_info);
        pt_media_desc->pt_ext_sbtl_info = NULL;
    }
    /* free lyric info */
    if(pt_media_desc->ps_lyric_full_path != NULL)
    {
        x_mem_free(pt_media_desc->ps_lyric_full_path);
        pt_media_desc->ps_lyric_full_path = NULL;
    }
    if(pt_media_desc->pt_lyric_info != NULL)
    {
        if(pt_media_desc->pt_lyric_info->ps_prf_lang_lst != NULL)
        {
            x_mem_free(pt_media_desc->pt_lyric_info->ps_prf_lang_lst);
            pt_media_desc->pt_lyric_info->ps_prf_lang_lst = NULL;
        }
        x_mem_free(pt_media_desc->pt_lyric_info);
        pt_media_desc->pt_lyric_info = NULL;
    }
    /*free font info*/
    if(pt_media_desc->pt_font_info)
    {
        pt_cur_font = pt_media_desc->pt_font_info;
        while(pt_cur_font)
        {
            pt_next_font = pt_cur_font->pt_next_font;
            if(pt_cur_font->pv_file_data)
            {
                x_fe_remove_font((CHAR*)(pt_cur_font->ac_file_name),  X_FE_OPTION_FORCE);
                x_mem_free(pt_cur_font->pv_file_data);
                pt_cur_font->pv_file_data = NULL;
            }
            x_mem_free(pt_cur_font);
            pt_cur_font = pt_next_font;
        }
        pt_media_desc->pt_font_info = NULL;
    }

#ifdef ENABLE_MMS_SUPPORT
    if(pt_media_desc->pc_mms_header_buf)
    {
        _svctx_release_mms_header(pt_media_desc->h_mms_handle, pt_media_desc->pc_mms_header_buf);
        pt_media_desc->pc_mms_header_buf = NULL;
        pt_media_desc->h_mms_handle      = (MMS_HANDLE_T)0;
    }
#endif

    if(pt_media_desc->pt_strm_info != NULL)
    {
        x_mem_free(pt_media_desc->pt_strm_info);
        pt_media_desc->pt_strm_info = NULL;
    }
 
    /*add for common API*/
    _svctx_free_meta_info(pt_media_desc);
    
    if(pt_media_desc->h_minfo_delay_timer != NULL_HANDLE)
    {
        x_timer_stop(pt_media_desc->h_minfo_delay_timer);
        x_timer_delete(pt_media_desc->h_minfo_delay_timer);
        pt_media_desc->h_minfo_delay_timer = NULL_HANDLE;
    }
 
    if(x_handle_valid(pt_media_desc->h_minfo_wait_sema))
    {
        x_sema_delete(pt_media_desc->h_minfo_wait_sema);
        pt_media_desc->h_minfo_wait_sema = NULL_HANDLE;
    }
  
#ifdef ENABLE_NAUTILUS
    if(pt_media_desc->t_drm_info.u.t_mln_drm_inf.pui1_act_tkn_url != NULL)
    {
        x_mem_free(pt_media_desc->t_drm_info.u.t_mln_drm_inf.pui1_act_tkn_url);
    }
        
    if(pt_media_desc->t_drm_info.u.t_mln_drm_inf.pui1_afl_tkn != NULL)
    {
        x_mem_free(pt_media_desc->t_drm_info.u.t_mln_drm_inf.pui1_afl_tkn);
    }
#endif
    if(pt_media_desc->ps_proxy_url != NULL)
    {
        x_mem_free(pt_media_desc->ps_proxy_url);
        pt_media_desc->ps_proxy_url = NULL;
    }
    if(pt_media_desc->ps_agent_url != NULL)
    {
        x_mem_free(pt_media_desc->ps_agent_url);
        pt_media_desc->ps_agent_url = NULL;
    }
    if(pt_media_desc->ps_cookie_url != NULL)
    {
        x_mem_free(pt_media_desc->ps_cookie_url);
        pt_media_desc->ps_cookie_url = NULL;
    }
    if(x_handle_valid(pt_media_desc->h_media_desc))
    {/*This should be last one as it will free pt_media_desc obj*/
        i4_ret = x_handle_free(pt_media_desc->h_media_desc);
        if(i4_ret != HR_OK)
        {
            return SVCTXR_FAIL;
        }
    }
  
    return SVCTXR_OK;
}

INT32 _svctx_media_fill_strm_info(MEDIA_DESCRIBT_T*  pt_media_desc,
                                  VOID*              pv_get_info,
                                  SIZE_T             z_get_info_size)
{
    INT32               i4_ret      = 0;
    UINT8               ui1_cur_idx = 0;
    SIZE_T              z_rec_length= 0;
    MM_STRM_INFO_T*     pt_strm_info= NULL;
    SVCTX_T*            pt_svctx    = NULL;
    MINFO_INFO_T        t_minfo;

    if(pt_media_desc == NULL)
    {
        return SVCTXR_INV_ARG;
    }    

    i4_ret = svctx_get_obj(pt_media_desc->h_svctx, &pt_svctx);
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }
    
    pt_strm_info = (MM_STRM_INFO_T*)pv_get_info;
    if(pt_strm_info == NULL)
    {
        return SVCTXR_INV_ARG;
    }

    if(pt_media_desc->pt_strm_info != NULL)
    {
        pt_strm_info = pt_media_desc->pt_strm_info;
    }
    else if (MM_HDLR_WITH_MINFO(pt_media_desc))
    {
        i4_ret = SVCTXR_OK;
        z_rec_length = sizeof(MINFO_INFO_T);

        pt_strm_info->t_format = pt_media_desc->t_media_fmt;
        pt_strm_info->ui2_num_stm = 0;
        
        while(1)
        {/*only support 1 video*/
            z_rec_length = sizeof(MINFO_INFO_T);
            i4_ret = x_minfo_get_info(pt_media_desc->h_media_info, 
                                      MINFO_INFO_TYPE_VIDEO_ATTR, 
                                      (MINFO_REC_IDX_T)(ui1_cur_idx + 1),
                                      &t_minfo,
                                      &z_rec_length);
            if(i4_ret != MINFOR_OK)
            {
                if((i4_ret == MINFOR_OUT_OF_RECORD) || (i4_ret == MINFOR_INFO_NOT_FOUND))
                {
                    ui1_cur_idx = 0;
                    break;
                }
                else
                {
                    return i4_ret;
                }
            }
            pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].e_type = ST_VIDEO;
            pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt
                                            = t_minfo.u.t_stm_attribute;
            ui1_cur_idx = 0;
            pt_strm_info->ui2_num_stm++;
            break;
        }

        while(1)
        {/*only support 8 audio*/
            z_rec_length = sizeof(MINFO_INFO_T);
            i4_ret = x_minfo_get_info(pt_media_desc->h_media_info, 
                                      MINFO_INFO_TYPE_AUDIO_ATTR, 
                                      (MINFO_REC_IDX_T)(ui1_cur_idx + 1),
                                      &t_minfo,
                                      &z_rec_length);
            if(i4_ret != MINFOR_OK)
            {
                if((i4_ret == MINFOR_OUT_OF_RECORD) || (i4_ret == MINFOR_INFO_NOT_FOUND))
                {
                    ui1_cur_idx = 0;
                    break;
                }
                else
                {
                    return i4_ret;
                }
            }
            pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].e_type = ST_AUDIO;
            pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt
                                            = t_minfo.u.t_stm_attribute;
            ui1_cur_idx++;
            pt_strm_info->ui2_num_stm++;
            if(pt_strm_info->ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
            {
                break;
            }
        }

        while(1)
        {/*only support 8 subtitle*/
            z_rec_length = sizeof(MINFO_INFO_T);
            i4_ret = x_minfo_get_info(pt_media_desc->h_media_info, 
                                      MINFO_INFO_TYPE_SP_ATTR, 
                                      (MINFO_REC_IDX_T)(ui1_cur_idx + 1),
                                      &t_minfo,
                                      &z_rec_length);
            if(i4_ret != MINFOR_OK)
            {
                if((i4_ret == MINFOR_OUT_OF_RECORD) || (i4_ret == MINFOR_INFO_NOT_FOUND))
                {
                    ui1_cur_idx = 0;
                    break;
                }
                else
                {
                    return i4_ret;
                }
            }
            pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].e_type = ST_MM_SUBTITLE;
            pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt
                                            = t_minfo.u.t_stm_attribute;
            ui1_cur_idx++;
            pt_strm_info->ui2_num_stm++;
            if(pt_strm_info->ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
            {
                break;
            }
        }
    }
    else if((pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_AUDIO_ES) ||
            (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_VIDEO_ES))
    {
        pt_strm_info->t_format              = pt_media_desc->t_media_fmt;
        pt_strm_info->ui2_num_stm           = 1;
        _fill_es_strm_atrbt_by_fmt(pt_strm_info);
    }    
    else if(pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_TS)
    {   /* As TS without minfo, it need get from scdb after scdb updated */
        /* Only for cmpb, need fill this first, then will come to get AVS count*/
        pt_strm_info->t_format              = pt_media_desc->t_media_fmt;
        if(pt_media_desc->b_is_scdb_updated)
        {
            i4_ret = _svctx_get_ts_strm_info(pt_svctx, pt_strm_info);
        }
    }
    
    if(pt_media_desc->b_is_lyric)
    {
        pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].e_type = ST_MM_SUBTITLE;
        pt_strm_info->ui2_num_stm++;
        if(pt_strm_info->ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
        {
            return SVCTXR_OK;
        }
    }
    else if (pt_media_desc->b_is_ext_first)
    {
        MINFO_EXT_SUBTITLE_STM_ATRBT_T*  pt_ext_subtitle_stm_attr = NULL;

        pt_ext_subtitle_stm_attr = &(pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].t_stm_atrbt.u.t_ex_subtt_stm_attr);

        for(ui1_cur_idx = 0; ui1_cur_idx < pt_media_desc->ui2_total_ext_subtitle_num; ui1_cur_idx ++)
        {
            pt_strm_info->at_stm_atrbt[pt_strm_info->ui2_num_stm].e_type = ST_MM_SUBTITLE;
            pt_ext_subtitle_stm_attr->h_ext_subtitle 
                        = pt_media_desc->t_mm_svctx_sbtl_desc[ui1_cur_idx].h_ext_subtitle;
            x_memcpy(&pt_ext_subtitle_stm_attr->c_lang_list,
                     pt_media_desc->t_mm_svctx_sbtl_desc[ui1_cur_idx].c_lang_list,
                     3);
            pt_ext_subtitle_stm_attr->h_related_subtitle 
                        = pt_media_desc->t_mm_svctx_sbtl_desc[ui1_cur_idx].h_related_subtitle;
            pt_ext_subtitle_stm_attr->ui2_idx 
                        = pt_media_desc->t_mm_svctx_sbtl_desc[ui1_cur_idx].ui2_idx;
            pt_ext_subtitle_stm_attr->b_is_related 
                        = pt_media_desc->t_mm_svctx_sbtl_desc[ui1_cur_idx].b_is_related;
            pt_strm_info->ui2_num_stm++;
            if(pt_strm_info->ui2_num_stm >= SUPPORTED_STRM_NUM_MAX)
            {
                return SVCTXR_OK;
            }
        }
    }
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      x_svctx_media_get_info
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
INT32 x_svctx_media_get_info(
                             HANDLE_T           h_svctx,
                             HANDLE_T           h_media,
                             SVCTX_GET_TYPE_T   t_svctx_get_type,
                             VOID*              pv_get_info,
                             SIZE_T             z_get_info_size
                             )
{
    INT32                       i4_ret        = 0;
    SVCTX_T*                    pt_svctx      = NULL;
    MEDIA_DESCRIBT_T*           pt_media_desc = NULL;
    SIZE_T                      z_rec_length = 0;
    MINFO_INFO_T                t_minfo_rec;
#ifdef ENABLE_DIVX_DRM    
    DRM_COMMAND_T               t_drm_cmd_sets[2];
    DIVX_DRM_UNION_INFO_T       t_divx_drm_info;
    DRM_INFO_SVCTX_T*           pt_drm_info_svctx = NULL;
#endif
    MINFO_INFO_T*               pt_minfo_meta = NULL;
    MEDIA_FORMAT_T*             pt_format = NULL;
#ifdef SYS_RECORD_SUPPORT
    BOOL                        b_is_pvr_file = FALSE;
#endif
    MINFO_INFO_CODEC_ENC_T*       t_minfo_info_codec_enc;

    if (h_svctx == NULL_HANDLE || h_media == NULL_HANDLE || pv_get_info == NULL)
    {
        return SVCTXR_INV_ARG;
    }

    i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }

    i4_ret = _mm_hdlr_get_media_obj(h_media, &pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }
    //x_memset(&t_minfo_rec, 0, sizeof(MINFO_INFO_T));
    
    switch (t_svctx_get_type)
    {
#ifdef ENABLE_DIVX_DRM
        case SVCTX_DRM_GET_TYPE_INFO:
            pt_drm_info_svctx = (DRM_INFO_SVCTX_T*)pv_get_info;
            t_minfo_rec.u.t_drm_info.t_minfo_drm_type = DRM_TYPE_NONE;
            if (MM_HDLR_WITH_MINFO(pt_media_desc))
            {
                z_rec_length = sizeof(MINFO_INFO_T);
                i4_ret = x_minfo_get_info(pt_media_desc->h_media_info, MINFO_INFO_TYPE_DRM_INFO, pt_drm_info_svctx->ui4_idx, (VOID*)&t_minfo_rec, &z_rec_length);
                if (i4_ret != MINFOR_OK && i4_ret != MINFOR_INFO_NOT_FOUND)
                {
                    DBG_ERROR((_ERROR_HEADER"%s: x_minfo_get_info failed, code = %d\r\n", __FUNCTION__, i4_ret));
                    return SVCTXR_FAIL;
                }
            }

            pt_drm_info_svctx->t_drm_type = pt_media_desc->t_drm_type = t_minfo_rec.u.t_drm_info.t_minfo_drm_type;

            if (pt_drm_info_svctx->t_drm_type == DRM_TYPE_DIVX_DRM)
            {
                if (z_get_info_size < sizeof(DRM_INFO_SVCTX_T))
                {
                    return SVCTXR_INV_ARG;
                }

                t_drm_cmd_sets[0].e_code = DRM_CMD_CODE_DIVX_DRM_STRD_CHUNK;
                t_drm_cmd_sets[0].u.pv_data = t_minfo_rec.u.t_drm_info.u.t_minfo_divx_drm_obj.pui1_strd_chunk;
                t_drm_cmd_sets[1].e_code = DRM_CMD_CODE_END;

                if (!x_handle_valid(pt_media_desc->h_drm))
                {
                    i4_ret = x_drm_open(t_drm_cmd_sets, t_minfo_rec.u.t_drm_info.t_minfo_drm_type, &pt_media_desc->h_drm);
                    if (i4_ret != DRMR_OK)
                    {
                        DBG_ERROR((_ERROR_HEADER"%s: x_drm_open failed, code = %d\r\n", __FUNCTION__, i4_ret));
                        return SVCTXR_FAIL;
                    }
                }
                x_memset(&t_divx_drm_info,0,sizeof(DIVX_DRM_UNION_INFO_T));
                i4_ret = x_drm_get_info(pt_media_desc->h_drm, DRM_GET_TYPE_DIVX_DRM_BASIC_INFO, &t_divx_drm_info, sizeof(DIVX_DRM_UNION_INFO_T));
                if (i4_ret != DRMR_OK)
                {
                    DBG_ERROR((_ERROR_HEADER"%s: x_drm_get_info failed, code = %d\r\n", __FUNCTION__, i4_ret));
                    return SVCTXR_FAIL;
                }
                if(pt_drm_info_svctx->u.t_divx_drm_info_svctx.pt_divx_drm_basic_info != NULL)
                {
                    *(pt_drm_info_svctx->u.t_divx_drm_info_svctx.pt_divx_drm_basic_info) 
                        = *(t_divx_drm_info.pt_divx_drm_basic_info);
                    
                    if (pt_drm_info_svctx->u.t_divx_drm_info_svctx.pt_divx_drm_basic_info->ui1_flag & DIVX_DRM_AUTH_ERROR)
                    {
                        DBG_INFO((_INFO_HEADER"%s: x_drm_get_info - DIVX_DRM_AUTH_ERROR\r\n", __FUNCTION__));
                        /* return SVCTXR_FAIL; */
                    }
                    else if (pt_drm_info_svctx->u.t_divx_drm_info_svctx.pt_divx_drm_basic_info->ui1_flag & DIVX_DRM_RENTAL_EXPIRED)
                    {
                        DBG_INFO((_INFO_HEADER"%s: x_drm_get_info - DIVX_DRM_RENTAL_EXPIRED\r\n", __FUNCTION__));
                        /* return SVCTXR_FAIL; */
                    }
                    else if (pt_drm_info_svctx->u.t_divx_drm_info_svctx.pt_divx_drm_basic_info->ui1_flag & DIVX_DRM_RENTAL)
                    {
                        DBG_INFO((_INFO_HEADER"%s: x_drm_get_info - DIVX_DRM_RENTAL\r\n", __FUNCTION__));
                        /* return SVCTXR_FAIL; */
                    }
                }

                i4_ret = x_drm_get_info(pt_media_desc->h_drm, DRM_GET_TYPE_OUTPUT_SIGNAL_PROTECTION_INFO, &t_divx_drm_info, sizeof(DIVX_DRM_UNION_INFO_T));
                if (i4_ret != DRMR_OK)
                {
                    DBG_ERROR((_ERROR_HEADER"%s: x_drm_get_info failed, code = %d\r\n", __FUNCTION__, i4_ret));
                    return SVCTXR_FAIL;
                }
                if(pt_drm_info_svctx->u.t_divx_drm_info_svctx.pt_drm_output_signal_protection_info != NULL)
                {
                    *(pt_drm_info_svctx->u.t_divx_drm_info_svctx.pt_drm_output_signal_protection_info) 
                        = *(t_divx_drm_info.pt_drm_output_signal_protection_info);
                    
                    DBG_INFO((_INFO_HEADER"%s: x_drm_get_info, DRM cgmsa_flag = %d\r\n", __FUNCTION__, pt_drm_info_svctx->u.t_divx_drm_info_svctx.pt_drm_output_signal_protection_info->ui1_cgmsa_flag));
                    DBG_INFO((_INFO_HEADER"%s: x_drm_get_info, DRM acptb_flag = %d\r\n", __FUNCTION__, pt_drm_info_svctx->u.t_divx_drm_info_svctx.pt_drm_output_signal_protection_info->ui1_acptb_flag));
                    DBG_INFO((_INFO_HEADER"%s: x_drm_get_info, DRM digital_protection_flag = %d\r\n", __FUNCTION__, pt_drm_info_svctx->u.t_divx_drm_info_svctx.pt_drm_output_signal_protection_info->ui1_digital_protection_flag));
                }
            }
            break;
#endif
        case SVCTX_MM_GET_TYPE_META_DATA:
            pt_minfo_meta = (MINFO_INFO_T*)pv_get_info;
            z_rec_length = sizeof(MINFO_INFO_T);
            /*add for common API*/
            if(pt_media_desc->pt_meta_info)
            {
                x_memcpy(pt_minfo_meta, pt_media_desc->pt_meta_info, sizeof(*pt_minfo_meta));
            }
            else if(pt_media_desc->e_svc_protocol != PROTOCL_MODE_PUSH)            
            {
                i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,
                                          (pt_minfo_meta->e_info_type != MINFO_INFO_TYPE_UNKNOWN) ? pt_minfo_meta->e_info_type : MINFO_INFO_TYPE_META_DATA,
                                          1,
                                          pt_minfo_meta,
                                          &z_rec_length);
                if (i4_ret != MINFOR_OK)
                {
                    x_dbg_stmt("mm_hdlr : Get Meta Info ERROR!\r\n");
                    return SVCTXR_FAIL;
                }
            }

            pt_minfo_meta->u.t_meta_data.ui8_pb_duration =
                pt_minfo_meta->u.t_meta_data.ui8_pb_duration / 90000;
            break;

        case SVCTX_MM_GET_TYPE_MEDIA_FORMAT:
            pt_format = (MEDIA_FORMAT_T*)pv_get_info;
#ifdef SYS_RECORD_SUPPORT
            if(SVCTXR_OK == svctx_rec_check_pvr_file(pt_media_desc->ps_media_path, 
                                                    &b_is_pvr_file))
            {
                /*do nothing*/
            }
            if(b_is_pvr_file)
            {
                pt_format->e_media_type = MEDIA_TYPE_TS;
                pt_format->t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_192_ENCRYPTION;
                pt_format->e_mm_svc_type = MEDIA_SVC_TYPE_UNKNOWN;
                break;
            }
#endif
            if (pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_UNKNOWN)
            {
                if((pt_media_desc->e_svc_protocol == PROTOCL_MODE_URI ||
                    pt_media_desc->e_svc_protocol == PROTOCL_MODE_PULL)
                    && (pt_media_desc->e_src_type == SRC_TYPE_MEDIA_NETWORK))
                {
                    i4_ret = x_mfmtrecg_get_mem_fmt (pt_media_desc->pt_media_feeder, 
                                                     0,   
                                                     pt_format);
                    if (i4_ret != MINFOR_OK)
                    {
                        x_dbg_stmt("mm_hdlr : Get Format Info ERROR!\r\n");
                        return SVCTXR_FAIL;
                    }
                    break;
                }
                else if(pt_media_desc->e_svc_protocol == PROTOCL_MODE_PUSH)
                {
                    *pt_format = pt_media_desc->t_media_fmt;
                    break;
                }
                pt_format->b_fav_recg_only = FALSE;
                i4_ret = x_mfmtrecg_get_file_fmt(pt_media_desc->ps_media_path, pt_format);
                if (i4_ret != MINFOR_OK)
                {
                    x_dbg_stmt("mm_hdlr : Get Format Info ERROR!\r\n");
                    return SVCTXR_FAIL;
                }
            }
            else
            {
                *pt_format = pt_media_desc->t_media_fmt;
            }
            break;
        /*add for common API*/
        case SVCTX_MM_GET_BUF_STATUS:
            {
                HANDLE_T                        h_conn;
                MM_SVC_REQ_T*                   pt_running_svc_req = NULL;
                PLAYBACK_GET_BUFFER_STATUS_T    t_buf_status;
                MM_SVC_BUF_STATUS_T*            pt_buf_status;

                if (NULL == pv_get_info)
                {
                    return SVCTXR_INV_ARG;
                }

                pt_buf_status = (MM_SVC_BUF_STATUS_T*)pv_get_info;
                
                if (z_get_info_size < sizeof(MM_SVC_BUF_STATUS_T))
                {
                    return SVCTXR_INV_ARG;
                }

                pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
                
                if (NULL == pt_running_svc_req)
                {
                    return SVCTXR_FAIL;
                }

                h_conn = player_get_conn_handle(pt_running_svc_req->pt_player);
                if (x_handle_valid(h_conn) == FALSE) 
                {
                    return SVCTXR_WRONG_STATE;
                }

                x_memset(&t_buf_status, 0, sizeof(PLAYBACK_GET_BUFFER_STATUS_T));
                i4_ret = x_cm_get(h_conn,
                          PLAYBACK_CTRL_GET_BUFFER_STATUS,
                          (VOID*)&t_buf_status);
                if (i4_ret != CMR_OK)
                {
                    return SVCTXR_FAIL;
                }
                
                pt_buf_status->ui8_begin_pos = t_buf_status.ui8_begin_pos;
                pt_buf_status->ui8_end_pos = t_buf_status.ui8_end_pos;
            }
            break;

        case SVCTX_MM_GET_TYPE_BUFFCTRL_INFO:
            {
                PLAYBACK_GET_BUFFER_FULLNESS_T  t_buff_fullness;
                MM_SVC_BUFFCTRL_INFO_T*         pt_buff_ctrl_info;
                HANDLE_T                        h_conn;
                MM_SVC_REQ_T*                   pt_running_svc_req = NULL;

                if (z_get_info_size < sizeof(MM_SVC_BUFFCTRL_INFO_T))
                {
                    return SVCTXR_INV_ARG;
                }

                pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
                
                if (NULL == pt_running_svc_req)
                {
                    return SVCTXR_FAIL;
                }
                
                h_conn = player_get_conn_handle(pt_running_svc_req->pt_player);
                if (x_handle_valid(h_conn) == FALSE) 
                {
                    return SVCTXR_WRONG_STATE;
                }

                x_memset(&t_buff_fullness, 0, sizeof(PLAYBACK_GET_BUFFER_FULLNESS_T));
                i4_ret = x_cm_get(h_conn,
                          PLAYBACK_CTRL_GET_BUFFER_FULLNESS,
                          (VOID*)&t_buff_fullness);

                pt_buff_ctrl_info = (MM_SVC_BUFFCTRL_INFO_T*)pv_get_info;
                x_memset(pt_buff_ctrl_info, 0, sizeof(MM_SVC_BUFFCTRL_INFO_T));
                if (i4_ret == CMR_OK)
                {
                    if(t_buff_fullness.ui4_percentage > 100)
                    {
                        t_buff_fullness.ui4_percentage = 100;
                    }
                    pt_buff_ctrl_info->ui4_buf_size.ui4_bytes = t_buff_fullness.ui4_length;
                    pt_buff_ctrl_info->ui4_buff_fullness = 
                        t_buff_fullness.ui4_length * t_buff_fullness.ui4_percentage / 100;
                    pt_buff_ctrl_info->ui4_buff_emptyness = 
                        t_buff_fullness.ui4_length - pt_buff_ctrl_info->ui4_buff_fullness;                    
                }
            }
            break;     

        case SVCTX_MM_GET_TYPE_TIME_TO_SIZE:
            {
                PLAYBACK_HANDLER_TIME_TO_SIZE_T     t_time2size;
                MM_SVC_TIME_TO_SIZE_T*              pt_tim2size;
                HANDLE_T                            h_conn;
                MM_SVC_REQ_T*                       pt_running_svc_req = NULL;

                if (z_get_info_size < sizeof(MM_SVC_TIME_TO_SIZE_T))
                {
                    return SVCTXR_INV_ARG;
                }

                pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

                if (NULL == pt_running_svc_req)
                {
                    return SVCTXR_FAIL;
                }

                h_conn = player_get_conn_handle(pt_running_svc_req->pt_player);
                if (x_handle_valid(h_conn) == FALSE) 
                {
                    return SVCTXR_WRONG_STATE;
                }
                
                pt_tim2size = (MM_SVC_TIME_TO_SIZE_T*)pv_get_info;
                x_memset(&t_time2size, 0, sizeof(PLAYBACK_HANDLER_TIME_TO_SIZE_T));
                t_time2size.ui4_ms = pt_tim2size->ui4_ms;
                i4_ret = x_cm_get(h_conn,
                          PLAYBACK_CTRL_GET_TIME_TO_SIZE,
                          (VOID*)&t_time2size);

                if (i4_ret == CMR_OK)
                {
                    pt_tim2size->ui4_size = t_time2size.ui4_size;
                }
                else
                {
                    return SVCTXR_FAIL;
                }
            }
            break;   

        case SVCTX_MM_GET_TYPE_DUR:
        {
            SIZE_T                          z_info_len = 0;
            MM_SVC_DUR_INFO_T*              pt_dur_code;
            MM_SVC_UOP_HANDLER_FCT_TBL_T    t_fct_tbl;
            
            
            if (SVCTXR_OK != mm_hdlr_find_handler(&pt_media_desc->t_media_fmt, &t_fct_tbl))
            {
                return SVCTXR_NOT_SUPPORT;
            }
            
            pt_dur_code = (MM_SVC_DUR_INFO_T*)pv_get_info;
            /*Byte duration*/
            if( pt_dur_code->t_dur_type == MM_SVC_DUR_TYPE_ALL_OFFSET )
            {
                z_info_len = sizeof(MINFO_INFO_T);
                i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,
                                          MINFO_INFO_TYPE_GENERIC,
                                          1,
                                          &t_minfo_rec,
                                          &z_info_len);
                if (i4_ret == MINFOR_OK)
                {
                    pt_dur_code->ui8_duration =
                        t_minfo_rec.u.t_general_info.ui8_media_sz;
                }
                else
                {   /*get file size for file system directly*/
                    pt_dur_code->ui8_duration =
                         pt_media_desc->ui8_media_sz;
                }
                return SVCTXR_OK;
            }/*Time duration*/
            else if( pt_dur_code->t_dur_type == MM_SVC_DUR_TYPE_ALL )
            {
                if(t_fct_tbl.pf_get_dur == NULL)
                {
                    return SVCTXR_FAIL;
                }
                i4_ret = t_fct_tbl.pf_get_dur(pt_media_desc,pt_dur_code);
                if (pt_dur_code->ui8_duration == 0)
                {
                    //i4_ret = SVCTXR_NOT_SUPPORT;
                }
                else if ((pt_dur_code->ui8_duration != pt_media_desc->ui8_media_dur) &&
                         (pt_media_desc->ui8_media_dur!= 0))
                {
                    pt_dur_code->ui8_duration = (pt_media_desc->ui8_media_dur / 90000);
                }
                else
                {
                    pt_dur_code->ui8_duration = pt_dur_code->ui8_duration / 90000;
                }
                if (i4_ret != SVCTXR_OK)
                {
                    return i4_ret;
                }
            }
        }
            break;   
            
        case SVCTX_MM_GET_TYPE_GENERIC_INFO:
            pt_minfo_meta = (MINFO_INFO_T*)pv_get_info;
            z_rec_length = sizeof(MINFO_INFO_T);
            x_memset(pt_minfo_meta, 0, z_rec_length);
            /*add for common API*/
            if(pt_media_desc->pt_meta_info)
            {
                x_memcpy(pt_minfo_meta, pt_media_desc->pt_meta_info, sizeof(*pt_minfo_meta));
            }
            else
            {
                i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,
                                          MINFO_INFO_TYPE_GENERIC,
                                          1,
                                          pt_minfo_meta,
                                          &z_rec_length);
                if (i4_ret != MINFOR_OK)/*Only get DRM info and allowed fail as some file has no DRM*/
                {
                    x_dbg_stmt("mm_hdlr : Get Generic Info(DRM) ERROR!\r\n");
                    break;
                }
            }
            break;       
            
        case SVCTX_MM_GET_TYPE_UOP_CAP_INFO:
        {
                SM_SESS_GNRC_UOP_CAP_INFO_T*         pt_uop_cap_info = NULL;
            
            pt_uop_cap_info = (SM_SESS_GNRC_UOP_CAP_INFO_T*)pv_get_info;
            z_rec_length = sizeof(SM_SESS_GNRC_UOP_CAP_INFO_T);

            if (pt_uop_cap_info == NULL || pt_media_desc == NULL)
            {
                return SVCTXR_INV_ARG;
            }
            
            /*check uop capability first*/
            if(pt_uop_cap_info->e_uop == _SM_SESS_GNRC_UOP_UNKNOWN)
            {
                pt_uop_cap_info->e_uop = _SM_SESS_GNRC_UOP_SEEK;
            }
            
            if (pt_uop_cap_info->e_uop == _SM_SESS_GNRC_UOP_SEEK && 
                pt_media_desc->e_speed != MM_SPEED_TYPE_FORWARD_1X &&        
                pt_media_desc->e_speed != MM_SPEED_TYPE_PAUSE      &&
                pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_AUDIO_ES)
            {
                return SVCTXR_FAIL;
            }
            if( pt_media_desc->b_non_seekable && 
               (pt_uop_cap_info->e_uop == _SM_SESS_GNRC_UOP_SEEK))
            {
                return SVCTXR_FAIL;
            }

            pt_uop_cap_info->u.t_speed = pt_media_desc->e_speed;
			if(((MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req)==NULL)
			{
				x_dbg_stmt("\n---------------pt_svctx->pt_running_svc_req is NULL!!!------------------\n");
				return SVCTXR_OK;
			}
            i4_ret = x_sm_get(((MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req)->pt_player->h_sess,
                              SM_SESS_GNRC_GET_TYPE_UOP_CAP,
                              (VOID*)pt_uop_cap_info,
                              &z_rec_length);
            if(i4_ret != SVCTXR_OK)
            {
                return SVCTXR_FAIL;
            }
            
#ifdef DLNA_SUPPORT
            if((pt_uop_cap_info->e_uop == _SM_SESS_GNRC_UOP_SEEK)
             &&(pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_AUDIO_ES)
             &&(pt_media_desc->e_src_type == SRC_TYPE_MEDIA_NETWORK)
             &&(pt_media_desc->e_svc_protocol == PROTOCL_MODE_DLNA))
            {
                if(!x_fm_dlna_is_pause_supported(pt_media_desc->ps_media_path))
                {
                    return SVCTXR_NOT_SUPPORT;
                }
            }
#endif
        }
        break;
           
        case SVCTX_MM_GET_TYPE_STRM_INFO:
        {
            i4_ret = _svctx_media_fill_strm_info(pt_media_desc,
                                                 pv_get_info,
                                                 z_get_info_size);
        }
            break;
            
        case SVCTX_MM_GET_TYPE_PUSH_BUFF:
        {
            MM_PB_CTRL_PUSH_BUF_INFO_T*   pt_pbctrl_push_buf_info;
            
            pt_pbctrl_push_buf_info = (MM_PB_CTRL_PUSH_BUF_INFO_T*)pv_get_info;
            
            pt_pbctrl_push_buf_info->h_mem_part
                            = NULL_HANDLE;//pt_media_desc->t_pbctrl_push_buf_info.h_mem_part;
            pt_pbctrl_push_buf_info->pv_buffer
                            = pt_media_desc->t_pbctrl_push_buf_info.pv_buffer;
            pt_pbctrl_push_buf_info->z_buffer_size
                            = pt_media_desc->t_pbctrl_push_buf_info.z_buffer_size;
            pt_pbctrl_push_buf_info->pv_private
                            = pt_media_desc->t_pbctrl_push_buf_info.pv_private;
         }
         break;    
         
        case SVCTX_MM_GET_TYPE_TITLE_NUM:
            z_rec_length = sizeof(UINT32);
            i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,
                                      MINFO_INFO_TYPE_TITLE_NUM,
                                      0,
                                      pv_get_info,
                                      &z_rec_length);
            break;
        
        case SVCTX_MM_GET_TYPE_TITLE_INFO:
            z_rec_length = sizeof(MINFO_TITLE_INFO_T);
            i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,
                                      MINFO_INFO_TYPE_TITLE_INFO,
                                      0,
                                      pv_get_info,
                                      &z_rec_length);
            break;

        case SVCTX_MM_GET_TYPE_PLAYLIST_INFO:
            {
                z_rec_length = sizeof(MINFO_PLAYLIST_INFO_T);
                i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,
                                          MINFO_INFO_TYPE_PLAYLIST_INFO,
                                          0,
                                          pv_get_info,
                                          &z_rec_length);
            }        
            break;
            
        case SVCTX_MM_GET_TYPE_CHAP_INFO:
            {
                z_rec_length = sizeof(MINFO_CHAP_INFO_T);
                i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,
                                          MINFO_INFO_TYPE_CHAP_INFO,
                                          0,
                                          pv_get_info,
                                          &z_rec_length);
            }        
            break;
            
        case SVCTX_MM_GET_TYPE_DISP_INFO:
            {
                z_rec_length = sizeof(MINFO_DISP_INFO_T);
                i4_ret = x_minfo_get_info(pt_media_desc->h_media_info,
                                          MINFO_INFO_TYPE_DISP_INFO,
                                          0,
                                          pv_get_info,
                                          &z_rec_length);
            }
            break;
           
        case SVCTX_MM_GET_MEM_BAND_STATUS:
        {
            STRM_REQ_T*         pt_video_strm_req   = NULL;
            
            pt_video_strm_req = &(((MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req)->t_strm_reqs[ST_VIDEO]);
            
            i4_ret = x_sm_get(pt_video_strm_req->h_stream,
                              SM_VSH_GET_TYPE_MEM_BAND_STATUS,
                              pv_get_info,
                              &z_get_info_size);
        }
            break;
        case SVCTX_MM_GET_PROGRAM_NUM:
        {
            UINT8* pTmp = (UINT8*)pv_get_info;
            
            if((pt_media_desc == NULL) || (pt_media_desc->pt_pmt_list == NULL))
            {
                return SVCTXR_FAIL;
            }

            *pTmp = pt_media_desc->pt_pmt_list->ui1_pmt_num;
        }
            break;
#ifdef MM_LAST_MEMORY_SUPPORT
        case SVCTX_MM_GET_MM_IDENTIFIER:
            if(z_get_info_size != sizeof(UINT64))
            {
                return SVCTXR_INV_ARG;
            }
            x_memcpy(pv_get_info, (const VOID*)&pt_media_desc->ui8_mm_identifier, z_get_info_size);
            break;
#endif
        case SVCTX_MM_GET_TYPE_VID_CODEC_INFO:
            x_memset(&t_minfo_rec,0,sizeof(MINFO_INFO_T));
            z_rec_length = sizeof(MINFO_INFO_T);
            i4_ret = x_minfo_get_info(pt_media_desc->h_media_info, MINFO_INFO_TYPE_CODEC, 1, &t_minfo_rec, &z_rec_length);
            if (i4_ret != MINFOR_OK)
            {
                DBG_ERROR((_ERROR_HEADER"%s: x_minfo_get_info failed, code = %d\r\n", __FUNCTION__, i4_ret));
                return SVCTXR_FAIL;
            }
            t_minfo_info_codec_enc = (MINFO_INFO_CODEC_ENC_T*)pv_get_info;
            *t_minfo_info_codec_enc = t_minfo_rec.u.t_divx_info.t_minfo_info_codec_enc;
            break;
        case SVCTX_MM_GET_TYPE_3D_VID_INFO:
        {
            MM_3D_VID_INFO_T* pt_3d_vid = (MM_3D_VID_INFO_T*)pv_get_info;
            
            if (pt_3d_vid == NULL || z_get_info_size != sizeof(MM_3D_VID_INFO_T))
            {
                return SVCTXR_FAIL;
            }
            switch (pt_svctx->t_video_info.e_src_tag3d_type)
            {    
                case VSH_SRC_TAG3D_2D:
                    pt_3d_vid->e_tag3d_type = MM_SRC_TAG3D_2D;
                    break;
                case VSH_SRC_TAG3D_MVC:
                    pt_3d_vid->e_tag3d_type = MM_SRC_TAG3D_MVC;
                    break;
                case VSH_SRC_TAG3D_FP:
                    pt_3d_vid->e_tag3d_type = MM_SRC_TAG3D_FP;
                    break;
                case VSH_SRC_TAG3D_FS:
                    pt_3d_vid->e_tag3d_type = MM_SRC_TAG3D_FS;
                    break;
                case VSH_SRC_TAG3D_TB:
                    pt_3d_vid->e_tag3d_type = MM_SRC_TAG3D_TB;
                    break;
                case VSH_SRC_TAG3D_SBS:
                    pt_3d_vid->e_tag3d_type = MM_SRC_TAG3D_SBS;
                    break;
                case VSH_SRC_TAG3D_REALD:
                    pt_3d_vid->e_tag3d_type = MM_SRC_TAG3D_REALD;
                    break;
                case VSH_SRC_TAG3D_SENSIO:
                    pt_3d_vid->e_tag3d_type = MM_SRC_TAG3D_SENSIO;
                    break;
                case VSH_SRC_TAG3D_TTDO:
                    pt_3d_vid->e_tag3d_type = MM_SRC_TAG3D_TTDO;
                    break;
                case VSH_SRC_TAG3D_NOT_SUPPORT:
                default:
                    pt_3d_vid->e_tag3d_type = MM_SRC_TAG3D_NOT_SUPPORT;
                    break;
            }
        }    
            break;
#ifdef MINFO_GET_RAW_DATA_SUPPORT             
        case SVCTX_MM_GET_TYPE_RAW_DATA:
            if (z_get_info_size != sizeof(MINFO_TYPE_RAW_DATA_T))
            {
                return SVCTXR_INV_ARG;
            }
            z_rec_length = z_get_info_size;
            i4_ret = x_minfo_get_info(pt_media_desc->h_media_info, MINFO_INFO_TYPE_RAW_DATA, 1, pv_get_info, &z_rec_length);
            if (i4_ret != MINFOR_OK)
            {
                DBG_ERROR((_ERROR_HEADER"%s: x_minfo_get_info failed, code = %d\r\n", __FUNCTION__, i4_ret));
                return SVCTXR_FAIL;
            }
            break;
#endif     
        case SVCTX_MM_GET_TYPE_ID3_RAW_DATA:
            if (z_get_info_size != sizeof(MINFO_ID3_RAW_DATA_T))
            {
                return SVCTXR_INV_ARG;
            }
            z_rec_length = z_get_info_size;
            i4_ret = x_minfo_get_info(pt_media_desc->h_media_info, MINFO_INFO_TYPE_ID3_RAW_DATA, 1, pv_get_info, &z_rec_length);
            if (i4_ret != MINFOR_OK)
            {
                DBG_ERROR((_ERROR_HEADER"%s: x_minfo_get_info failed, code = %d\r\n", __FUNCTION__, i4_ret));
                return SVCTXR_FAIL;
            }
            break;
        case SVCTX_MM_GET_THUMBNAIL:
        {/* get video thumbnail */
            UINT8                       ui1_byte_per_pixel  = 0;
            VSH_THUMBNAIL_INFO_T        t_vsh_thumbnail;
            THUMBNAIL_GET_INFO_T*       pt_thumb_info       = NULL;
            STRM_REQ_T*                 pt_video_strm_req   = NULL;

            x_memset(&t_vsh_thumbnail, 0, sizeof(VSH_THUMBNAIL_INFO_T));
            
            pt_thumb_info       = (THUMBNAIL_GET_INFO_T*)pv_get_info;
            pt_video_strm_req   = &(((MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req)->t_strm_reqs[ST_VIDEO]);
            
            ui1_byte_per_pixel = _get_pixel_byte_by_clrmode(pt_thumb_info->eCanvasColormode);
            t_vsh_thumbnail.pc_canvas_buffer    = pt_thumb_info->u1CanvasBuffer;
            t_vsh_thumbnail.e_canvas_colormode  = pt_thumb_info->eCanvasColormode;
            t_vsh_thumbnail.ui4_canvas_width    = pt_thumb_info->u4ThumbnailWidth;
            t_vsh_thumbnail.ui4_canvas_height   = pt_thumb_info->u4ThumbnailHeight;
            t_vsh_thumbnail.ui4_canvas_pitch    = (pt_thumb_info->u4ThumbnailWidth*ui1_byte_per_pixel+15)&0xFFFFFFF0;
            t_vsh_thumbnail.ui4_thumbnail_x     = 0;
            t_vsh_thumbnail.ui4_thumbnail_y     = 0;
            t_vsh_thumbnail.ui4_thumbnail_width = pt_thumb_info->u4ThumbnailWidth;
            t_vsh_thumbnail.ui4_thumbnail_height= pt_thumb_info->u4ThumbnailHeight;
            
            z_get_info_size = sizeof(VSH_THUMBNAIL_INFO_T);
            i4_ret = x_sm_get(pt_video_strm_req->h_stream,
                              SM_VSH_GET_TYPE_THUMBNAIL,
                              (VOID*)(&t_vsh_thumbnail),
                              &z_get_info_size);
            if(i4_ret != SMR_OK)
            {
                return SVCTXR_FAIL;
            }
            while(!pt_media_desc->b_video_thumbnail_done)
            {
                x_thread_delay(5);
            }
        }
        break;

        case SVCTX_MM_GET_HTTP_DOWNLOAD_SPEED:
            {
                HANDLE_T        h_conn              = NULL_HANDLE;
                MM_SVC_REQ_T*   pt_running_svc_req  = NULL;

                if((NULL == pv_get_info) || (z_get_info_size < sizeof(DOUBLE)))
                {
                    return SVCTXR_INV_ARG;
                }
                
                pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;                
                if (NULL == pt_running_svc_req)
                {
                    return SVCTXR_FAIL;
                }

                h_conn = player_get_conn_handle(pt_running_svc_req->pt_player);
                if (x_handle_valid(h_conn) == FALSE) 
                {
                    return SVCTXR_WRONG_STATE;
                }

                i4_ret = x_cm_get(h_conn,
                                  PLAYBACK_CTRL_GET_HTTP_DOWNLOAD_SPEED,
                                  pv_get_info);
                if (i4_ret != CMR_OK)
                {
                    return SVCTXR_FAIL;
                }
            }
            break;
        default:
            return SVCTXR_NOT_SUPPORT;
    }

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      x_svctx_media_set_info
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
INT32 x_svctx_media_set_info(
    HANDLE_T h_svctx,
    HANDLE_T h_media,
    SVCTX_SET_TYPE_T t_svctx_set_type,
    VOID* pv_set_info,
    SIZE_T z_set_info_size)
{
    INT32 i4_ret;
    SVCTX_T* pt_svctx;
    MEDIA_DESCRIBT_T* pt_media_desc;
    MM_STRM_INFO_T*  pt_strm_info;

    //x_dbg_stmt("x_svctx_media_set_info() : type = %d, SVCTX_MM_SET_CANCEL_URI_READ = %d \n", 
       // t_svctx_set_type, SVCTX_MM_SET_CANCEL_URI_READ);

    if(SVCTX_MM_SET_CANCEL_URI_READ != t_svctx_set_type)
    {
    if (((h_svctx == NULL_HANDLE) && (SVCTX_MM_SET_TYPE_LANGUAGE != t_svctx_set_type)) || 
         h_media == NULL_HANDLE || pv_set_info == NULL)
    {
        return SVCTXR_INV_ARG;
    }
    
    if(SVCTX_MM_SET_TYPE_LANGUAGE != t_svctx_set_type)  //this type need not svctx
    {
        i4_ret = svctx_get_obj(h_svctx,&pt_svctx);
        if (i4_ret != SVCTXR_OK)
        {
            return i4_ret;
        }
    }

    i4_ret = _mm_hdlr_get_media_obj(h_media,&pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }
    }

    switch (t_svctx_set_type)
    {
#ifdef ENABLE_DIVX_DRM
        case SVCTX_DRM_SET_TYPE_PLAYBACK:
            if (pt_media_desc->t_drm_type == DRM_TYPE_DIVX_DRM)
            {
                if (!x_handle_valid(pt_media_desc->h_drm))
                {
                    return SVCTXR_FAIL;
                }
                
                i4_ret = x_drm_set_info(pt_media_desc->h_drm, DRM_SET_TYPE_DIVX_DRM_PLAYBACK, NULL, 0);
                if (i4_ret != DRMR_OK)
                {
                    DBG_ERROR((_ERROR_HEADER"%s: x_drm_set_info failed, code = %d\r\n", __FUNCTION__, i4_ret));
                    return SVCTXR_FAIL;
                }
            }
            break;
#endif
        case SVCTX_MM_SET_TYPE_SEND_PUSH_BUF:
        {
            HANDLE_T                            h_conn;
            MM_SVC_REQ_T*                       pt_running_svc_req = NULL;
            PLAYBACK_HANDLER_QUEUE_BUFFER_T     t_queue_buf_item = {0};

            pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
            
            if (NULL == pt_running_svc_req)
            {
                return SVCTXR_FAIL;
            }

            h_conn = player_get_conn_handle(pt_running_svc_req->pt_player);
            if (x_handle_valid(h_conn) == FALSE) 
            {
                return SVCTXR_WRONG_STATE;
            }

            t_queue_buf_item.ui4_id           = 0;
            t_queue_buf_item.ui4_flags        = CM_PLAYBACK_FLAG_SHARED_MEMORY | CM_PLAYBACK_FLAG_CANNOT_SET_OFFSET;   
            t_queue_buf_item.e_format         = DATA_FMT_UNKNOWN; 
            t_queue_buf_item.ui8_start_offset = 0;
            t_queue_buf_item.ui8_end_offset   = 0;
            t_queue_buf_item.ui8_size         = z_set_info_size;
            t_queue_buf_item.pv_data          = pv_set_info;
            t_queue_buf_item.pt_scdb_info     = NULL;
            t_queue_buf_item.ui4_nb_records   = 0;
            t_queue_buf_item.ui4_buf_agt_index= 0;
            x_memset(&t_queue_buf_item.t_extra, 0, sizeof(PLAYBACK_HANDLER_ITEM_EXTRA_INFO_T));

            i4_ret = x_cm_set(h_conn,
                              PLAYBACK_CTRL_SET_QUEUE_BUFFER,
                              (VOID*) & t_queue_buf_item);
        }
            break;
 
        case SVCTX_MM_SET_TYPE_SEND_IBC:
        {
            HANDLE_T                            h_conn;
            MM_SVC_REQ_T*                       pt_running_svc_req = NULL;
            PLAYBACK_HANDLER_QUEUE_BUFFER_T     t_queue_buf_item = {0};
        
            pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;

            if (NULL == pt_running_svc_req)
            {
                return SVCTXR_FAIL;
            }

            h_conn = player_get_conn_handle(pt_running_svc_req->pt_player);
            if (x_handle_valid(h_conn) == FALSE) 
            {
                return SVCTXR_WRONG_STATE;
            }
 
            t_queue_buf_item.ui4_id           = 0;
            t_queue_buf_item.ui4_flags        = CM_PLAYBACK_FLAG_SHARED_MEMORY | CM_PLAYBACK_FLAG_CANNOT_SET_OFFSET;   
            t_queue_buf_item.e_format         = DATA_FMT_IBC;
            t_queue_buf_item.ui8_start_offset = 0;
            t_queue_buf_item.ui8_end_offset   = 0;
            t_queue_buf_item.ui8_size         = 0;
            t_queue_buf_item.pv_data          = NULL;
            t_queue_buf_item.pt_scdb_info     = NULL;
            t_queue_buf_item.ui4_nb_records   = 0;
            t_queue_buf_item.ui4_buf_agt_index= 0; 
            x_memcpy(&t_queue_buf_item.t_extra, pv_set_info, sizeof(PLAYBACK_HANDLER_ITEM_EXTRA_INFO_T));
            
            i4_ret = x_cm_set(h_conn,
                              PLAYBACK_CTRL_SET_QUEUE_BUFFER,
                              (VOID*) & t_queue_buf_item);
        }
            break;

        case SVCTX_MM_SET_TYPE_CRYPT_KEY:

            if (z_set_info_size == sizeof(MM_CRYPT_INFO_T))
            {
                x_memcpy(&(pt_media_desc->t_crypt_info), pv_set_info, sizeof(MM_CRYPT_INFO_T));

                x_dbg_stmt("%s#%d:  crypt mode :%d.\n", __func__, __LINE__,pt_media_desc->t_crypt_info.u.t_pvr_crypt.e_mode);
            }
            else
            {
                x_dbg_stmt(" crypt key size is error.\n");
            }
            
            break;


        case SVCTX_MM_SET_TYPE_REL_PUSH_BUF:
        {
            /*Nothing need to be done now*/                
        }
            break;

        case SVCTX_MM_SET_TYPE_PULL_INFO:
        {   /*add for range info*/
            MM_PB_CTRL_PULL_INFO_T*   pt_pull_info;
            pt_pull_info = (MM_PB_CTRL_PULL_INFO_T*)pv_set_info;

            pt_media_desc->t_playback_ctrl_pull_info.ui8_preroll         = pt_pull_info->ui8_preroll;
            pt_media_desc->t_playback_ctrl_pull_info.ui4_max_acces_unt   = pt_pull_info->ui4_max_acces_unt;
            pt_media_desc->t_playback_ctrl_pull_info.ui4_min_acces_unt   = pt_pull_info->ui4_min_acces_unt;
            pt_media_desc->t_playback_ctrl_pull_info.ui8_media_data_ofst = pt_pull_info->ui8_media_data_ofst;
            pt_media_desc->t_playback_ctrl_pull_info.ui8_media_duration  = pt_pull_info->ui8_media_duration;
            pt_media_desc->t_playback_ctrl_pull_info.ui8_media_data_size = pt_pull_info->ui8_media_data_size;

            pt_media_desc->ui8_media_dur = pt_pull_info->ui8_media_duration;
            pt_media_desc->ui8_media_sz  = pt_pull_info->ui8_media_data_size;
        }
            break;
        case SVCTX_MM_SET_TYPE_STRM_INFO:
            /* all information of streams would be put in pt_strm_info */
            pt_strm_info = (MM_STRM_INFO_T*)pv_set_info;

            /* copy stream information to pt_media_desc */
            if(pt_media_desc->pt_strm_info == NULL)
            {
                pt_media_desc->pt_strm_info = (MM_STRM_INFO_T*)x_mem_alloc(sizeof(MM_STRM_INFO_T));
                if(pt_media_desc->pt_strm_info == NULL)
                {
                    return SVCTXR_NO_RESOURCE;
                }
            }
            x_memcpy(&pt_media_desc->t_media_fmt, &pt_strm_info->t_format, sizeof(MEDIA_FORMAT_T));
            x_memcpy((VOID*)(pt_media_desc->pt_strm_info), (VOID*)pt_strm_info, sizeof(MM_STRM_INFO_T));
            break;       

        case SVCTX_MM_SET_TYPE_STRM_CODEC_INFO:
            {
                MM_STRM_CODEC_INFO_T* pt_strm_codec_info;
                
                /* all information of streams would be put in pt_strm_info */
                pt_strm_codec_info = (MM_STRM_CODEC_INFO_T*)pv_set_info;

                /* copy stream information to pt_media_desc */
                if(pt_media_desc->pt_strm_info == NULL)
                {
                    pt_media_desc->pt_strm_info = (MM_STRM_INFO_T*)x_mem_alloc(sizeof(MM_STRM_INFO_T));
                    if(pt_media_desc->pt_strm_info == NULL)
                    {
                        return SVCTXR_NO_RESOURCE;
                    }
                }

                if(pt_strm_codec_info->ui1_strm_idx < SUPPORTED_STRM_NUM_MAX)
                {
                    if (pt_media_desc->pt_strm_info->at_stm_atrbt[pt_strm_codec_info->ui1_strm_idx].e_type == ST_AUDIO)
                    {
                        pt_media_desc->pt_strm_info->at_stm_atrbt[pt_strm_codec_info->ui1_strm_idx].t_stm_atrbt.u.t_asf_audio_stm_attr.pui1_codec_info 
                                = x_mem_alloc(pt_strm_codec_info->ui4_codec_info_size);
                        x_memcpy(pt_media_desc->pt_strm_info->at_stm_atrbt[pt_strm_codec_info->ui1_strm_idx].t_stm_atrbt.u.t_asf_audio_stm_attr.pui1_codec_info,
                                 pt_strm_codec_info->aui1_codec_info,
                                 pt_strm_codec_info->ui4_codec_info_size);
                    }
                    else if (pt_media_desc->pt_strm_info->at_stm_atrbt[pt_strm_codec_info->ui1_strm_idx].e_type == ST_VIDEO)
                    {
                        pt_media_desc->pt_strm_info->at_stm_atrbt[pt_strm_codec_info->ui1_strm_idx].t_stm_atrbt.u.t_asf_video_stm_attr.pui1_codec_info 
                                = x_mem_alloc(pt_strm_codec_info->ui4_codec_info_size);
                        x_memcpy(pt_media_desc->pt_strm_info->at_stm_atrbt[pt_strm_codec_info->ui1_strm_idx].t_stm_atrbt.u.t_asf_video_stm_attr.pui1_codec_info,
                                 pt_strm_codec_info->aui1_codec_info,
                                 pt_strm_codec_info->ui4_codec_info_size);
                    }
                }
            }
            break;

        case SVCTX_MM_SET_TYPE_TICK_CTRL_BLK:
            {
                PVR_TICK_CTRL_BLK_T* pt_tick_ctrl_blk;
                
                /* all information of streams would be put in pt_strm_info */
                pt_tick_ctrl_blk = (PVR_TICK_CTRL_BLK_T*)pv_set_info;

                if (NULL == pt_tick_ctrl_blk)
                {
                    return SVCTXR_FAIL;
                }

                pt_media_desc->t_rec_tick_ctrl_blk = *pt_tick_ctrl_blk;
            }
            break;

        case SVCTX_MM_SET_TYPE_3D_INFO:
            {
                UINT32 ui4_3d_type;

                if (NULL == pv_set_info)
                {
                    return SVCTXR_FAIL;
                }

                ui4_3d_type = *((UINT32*)pv_set_info);

                pt_media_desc->e_scopic_type = (MM_SCOPIC_TYPE_T)ui4_3d_type;
            }
            break;            
        case SVCTX_MM_SET_TYPE_LANGUAGE:            
            {                
                if (NULL == pv_set_info) 
                {                    
                    return SVCTXR_FAIL;                
                }  
                
                if ( pt_media_desc->t_media_fmt.e_media_type == MEDIA_TYPE_AUDIO_ES &&              
                    (pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_MP2 || 
                     pt_media_desc->t_media_fmt.t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_MP3))   
                {                    
                    i4_ret = x_minfo_set_info(pt_media_desc->h_media_info,  
                                              MINFO_INFO_TYPE_SET_LANGUAGE,             
                                              0,                                   
                                              pv_set_info,                
                                              &z_set_info_size);            
                    if (i4_ret != MINFOR_OK)   
                    {                 
                        return SVCTXR_FAIL;         
                    }                
                } 
                
            }      
            break;
        case SVCTX_MM_SET_CANCEL_URI_READ:
            if(0 != (*((HANDLE_T*)pv_set_info)) && 
                0xFFFFFFFF != (*((HANDLE_T*)pv_set_info)))
            {
#ifdef INET_SUPPORT
                x_dbg_stmt("URI mode cancel http read, handle is valid \n");
                i4_ret = x_httpc_fm_read_break(*((HANDLE_T*)pv_set_info));
#endif
            }
            x_dbg_stmt("URI mode cancel http read, handle = %x, ret = %d \n", 
                *((HANDLE_T*)pv_set_info), i4_ret);
            
            i4_ret = _mm_hdlr_get_media_obj(h_media,&pt_media_desc);
            if (i4_ret == SVCTXR_OK)
            {
                MEDIA_FEEDER_NW_T* pt_feeder;
                if(pt_media_desc && pt_media_desc->pt_media_feeder)
                {
                    pt_media_desc->pt_media_feeder->b_canceled = TRUE;
                    x_dbg_stmt("URI mode cancel http read, set feeder flag to cancel \n");
                    pt_feeder = (MEDIA_FEEDER_NW_T*)(pt_media_desc->pt_media_feeder->h_feeder);
                    if(pt_feeder)
                    {
                        pt_feeder->b_canceled = TRUE;
                        x_dbg_stmt("URI mode cancel http read, set NW feeder flag to cancel \n");
                    }
                }
            }
            break;
       case SVCTX_MM_SET_TYPE_PCR_PID:    
       {
            if (z_set_info_size == sizeof(UINT16))
            {
                pt_media_desc->ui2_pcr_pid = *((UINT16*)pv_set_info);
            }
            else
            {
                return SVCTXR_INV_ARG;
            }
       }
       break;
        default:
            break;
    }

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      x_svctx_generic_get_info
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
INT32 x_svctx_generic_get_info(
    HANDLE_T h_svctx,
    HANDLE_T h_media,
    SVCTX_GET_TYPE_T t_svctx_get_type,
    VOID* pv_get_info,
    SIZE_T z_get_info_size)
{
    INT32 i4_ret = 0;
    MEDIA_FORMAT_T*   pt_format = NULL;
    SVCTX_T* pt_svctx = NULL;

    if (h_svctx == NULL_HANDLE || pv_get_info == NULL)
    {
        return SVCTXR_INV_ARG;
    }

    i4_ret = svctx_get_obj(h_svctx,&pt_svctx);
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }

    switch (t_svctx_get_type)
    {
#ifdef ENABLE_DIVX_DRM
        case SVCTX_GENGRIC_GET_TYPE_DIVX_DRM_REGISTRATION_CODE:
            if ((pv_get_info == NULL) || (z_get_info_size < sizeof(DIVX_DRM_UNION_GENERIC_INFO_T)))
            {
                return SVCTXR_INV_ARG;
            }

            i4_ret = x_drm_get_generic_info(DRM_TYPE_DIVX_DRM, DRM_GET_TYPE_DIVX_DRM_GENERIC_REGISTRATION_INFO, pv_get_info, z_get_info_size);
            if (i4_ret != DRMR_OK)
            {
                DBG_ERROR((_ERROR_HEADER"%s: x_drm_get_generic_info (registration code) failed, code = %d\r\n", __FUNCTION__, i4_ret));
                return SVCTXR_FAIL;
            }

            DBG_INFO((_INFO_HEADER"%s: x_drm_get_generic_info, registration code = %s\r\n", __FUNCTION__, pv_get_info));
            break;
        case SVCTX_GENGRIC_GET_TYPE_DIVX_DRM_UI_HELP_INFO:
            if ((pv_get_info == NULL) || (z_get_info_size < sizeof(DIVX_DRM_UNION_GENERIC_INFO_T)))
            {
                return SVCTXR_INV_ARG;
            }

            i4_ret = x_drm_get_generic_info(DRM_TYPE_DIVX_DRM, DRM_GET_TYPE_DIVX_DRM_GENERIC_UI_HELP_INFO, pv_get_info, z_get_info_size);
            if (i4_ret != DRMR_OK)
            {
                DBG_ERROR((_ERROR_HEADER"%s: x_drm_get_generic_info (ui help info) failed, code = %d\r\n", __FUNCTION__, i4_ret));
                return SVCTXR_FAIL;
            }

            DBG_INFO((_INFO_HEADER"%s: x_drm_get_generic_info, ui help info = \r\n", __FUNCTION__));
            DBG_INFO((_INFO_HEADER"%s: %s to confirm for deactivation\n", __FUNCTION__, *((UINT32*)pv_get_info) & DIVX_DRM_DEACTIVATION_CONFIRMATION ? "Need" : "Don't need"));
            DBG_INFO((_INFO_HEADER"%s: %s to hide registration code\n", __FUNCTION__, *((UINT32*)pv_get_info) & DIVX_DRM_REGISTRATION_CODE_HIDE ? "Need" : "Don't need"));
            break;
#endif
        case SVCTX_MM_GET_TYPE_MEDIA_FORMAT:
            {
                MEDIA_DESCRIBT_T* pt_media_desc = NULL;
                if (h_media != NULL_HANDLE)
                {
                    i4_ret = _mm_hdlr_get_media_obj(h_media,&pt_media_desc);
                    if (i4_ret != SVCTXR_OK)
                    {
                        return i4_ret;
                    }
                }
                else 
                {
                    return SVCTXR_FAIL;
                }
                
                pt_format = (MEDIA_FORMAT_T*)pv_get_info;
                pt_format->b_fav_recg_only = FALSE;
                i4_ret = x_mfmtrecg_get_file_fmt(pt_media_desc->ps_media_path, pt_format);
                if (i4_ret != MINFOR_OK)
                {
                    x_dbg_stmt("mm_hdlr : Get Format Info ERROR!\r\n");
                    return SVCTXR_FAIL;
                }
            }
            break;
        default:
            break;
    }
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      x_svctx_generic_set_info
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
INT32 x_svctx_generic_set_info(
    HANDLE_T h_svctx,
    HANDLE_T h_media,
    SVCTX_SET_TYPE_T t_svctx_set_type,
    VOID* pv_set_info,
    SIZE_T z_set_info_size)
{
    INT32 i4_ret;
    SVCTX_T* pt_svctx;
#ifdef ENABLE_DIVX_DRM
    MEDIA_DESCRIBT_T* pt_media_desc;
#endif

    if (h_svctx == NULL_HANDLE || 
        (pv_set_info == NULL && SVCTX_MM_SBTL_SET_TYPE_DEFAULT != t_svctx_set_type))
    {
        return SVCTXR_INV_ARG;
    }

    i4_ret = svctx_get_obj(h_svctx,&pt_svctx);
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }

#ifdef ENABLE_DIVX_DRM
    if (h_media != NULL_HANDLE)
    {
        i4_ret = _mm_hdlr_get_media_obj(h_media,&pt_media_desc);
        if (i4_ret != SVCTXR_OK)
        {
            return i4_ret;
        }
    }
#endif

    switch (t_svctx_set_type)
    {
#ifdef ENABLE_DIVX_DRM
        case SVCTX_GENGRIC_SET_TYPE_DIVX_DRM_DEACTIVATION:
            if ((pv_set_info == NULL) || (z_set_info_size < sizeof(DIVX_DRM_UNION_GENERIC_INFO_T)))
            {
                return SVCTXR_INV_ARG;
            }

            i4_ret = x_drm_set_generic_info(DRM_TYPE_DIVX_DRM, DRM_SET_TYPE_DIVX_DRM_GENERIC_DEACTIVATION, pv_set_info, z_set_info_size);
            if (i4_ret != DRMR_OK)
            {
                DBG_ERROR((_ERROR_HEADER"%s: x_drm_set_generic_info (deactivation) failed, code = %d\r\n", __FUNCTION__, i4_ret));
                return SVCTXR_FAIL;
            }

            DBG_INFO((_INFO_HEADER"%s: x_drm_set_generic_info, deactivation code = %s\r\n", __FUNCTION__, pv_set_info));
            break;
        case SVCTX_GENGRIC_SET_TYPE_DIVX_DRM_MEMORY_RW_FUNC:
            if ((pv_set_info == NULL) || (z_set_info_size < sizeof(DIVX_DRM_UNION_GENERIC_INFO_T)))
            {
                return SVCTXR_INV_ARG;
            }

            i4_ret = x_drm_set_generic_info(DRM_TYPE_DIVX_DRM, DRM_SET_TYPE_DIVX_DRM_GENERIC_MEMORY_RW_FUNC, pv_set_info, z_set_info_size);
            if (i4_ret != DRMR_OK)
            {
                DBG_ERROR((_ERROR_HEADER"%s: x_drm_set_generic_info  memory R/W failed, code = %d\r\n", __FUNCTION__, i4_ret));
                return SVCTXR_FAIL;
            }

            DBG_INFO((_INFO_HEADER"%s: x_drm_set_generic_info, memory R/W\r\n", __FUNCTION__));
            break;
#endif

        case SVCTX_MM_SBTL_SET_TYPE_DISP_MODE:
        case SVCTX_MM_SBTL_SET_TYPE_HILT_STL:
        case SVCTX_MM_SBTL_SET_TYPE_TIME_OFST:
        case SVCTX_MM_SBTL_SET_TYPE_FONT_ENC:
        case SVCTX_MM_SBTL_SET_TYPE_HILT_ATTR:
        case SVCTX_MM_SBTL_SET_TYPE_NRML_ATTR:
        case SVCTX_MM_SBTL_SET_TYPE_SHOW_HIDE:
        case SVCTX_MM_SBTL_SET_TYPE_DEFAULT:
            i4_ret = _mm_hdlr_set_lyric_sbtl(pt_svctx,t_svctx_set_type,pv_set_info,z_set_info_size);
            break;            
            
        case SVCTX_MM_SBTL_SET_FONT_BG_COLOR:
            {
                WGL_COLOR_INFO_T* pt_set_color = (WGL_COLOR_INFO_T*)pv_set_info;
                WGL_COLOR_INFO_T* pt_clr_info  = NULL;
                
                pt_clr_info  = &pt_svctx->t_mm_sbtl_lyric_info.t_font_bg_color;
                x_memcpy(pt_clr_info, pt_set_color, sizeof(WGL_COLOR_INFO_T));
                pt_svctx->t_mm_sbtl_lyric_info.b_is_font_bg_clr_set = TRUE;
            }
            break;

        default:
            break;
    }
    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      x_svctx_media_seek
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
INT32 x_svctx_media_seek(
                    HANDLE_T                        h_svctx,
                    x_svctx_select_nfy_fct          pf_nfy,
                    VOID*                           pv_tag,
                    MM_SVC_POS_INFO_T*              pt_pos_info
                    )
{

    INT32 i4_ret;


    i4_ret = x_svctx_set(
                    h_svctx,
                    SVCTX_MM_SET_TYPE_SEEK,
                    (VOID*)pt_pos_info,
                    sizeof(MM_SVC_POS_INFO_T)
                    );
    if(SVCTXR_OK != i4_ret)
    {
        return SVCTXR_FAIL;
    }
    return SVCTXR_ASYNC_NFY;
}

static VOID _svctx_thumbnail_timeout_cb(
                    HANDLE_T                    h_timer,
                    VOID*                       pv_tag
                    )
{
   SVCTX_T*       pt_svctx           = (SVCTX_T*) pv_tag;
   MM_SVC_REQ_T*  pt_running_svc_req = NULL;
   HANDLE_T       h_svctx            = NULL_HANDLE;

   pt_running_svc_req = (MM_SVC_REQ_T*)pt_svctx->pt_running_svc_req;
   if (pt_running_svc_req == NULL)
   {
        return;
   }
   h_svctx = pt_running_svc_req->t_parent.h_svctx;
   
   _mm_hdlr_send_msg(h_svctx,
                     NULL_HANDLE,
                     _SVCTX_MSG_THUMBNAIL_TIMEOUT_NTFY,
                     pv_tag,
                     0,
                     0,
                     0);
}

/*-----------------------------------------------------------------------------
 * Name
 *      x_svctx_media_load_thumbnail
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
INT32 x_svctx_media_load_thumbnail(
                    HANDLE_T                        h_svctx,
                    HANDLE_T                        h_media,
                    SVCTX_VIDEO_THUMBNAIL_INFO_T*   pt_thumbnail_info
                    )
{
    SRC_MM_INFO_T t_mm = {0};
    VSH_REGION_INFO_T t_video_region = {0};
    VSH_REGION_INFO_T t_disp_region = {0};
    VSH_SET_PLANE_ORDER_INFO_T t_video_plane_order;
    SRC_DESC_T t_src_desc = {0};
    SNK_DESC_T t_snk_desc = {0};
    INT32 i4_ret = 0;
    MEDIA_DESCRIBT_T* pt_media_desc;
    SVCTX_T*      pt_svctx;

    i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
    if (i4_ret != SVCTXR_OK)
    {
        DBG_ERROR((_ERROR_HEADER
                   "svctx_get_obj(h_svctx=%d) failed!\r\n"
                   "i4_ret=%d, pt_svctx=%d\r\n",
                   h_svctx, i4_ret, pt_svctx));
        ASSERT(FALSE, ("%s(), pt_svctx is NULL\r\n", __func__));
    }


    i4_ret = _mm_hdlr_get_media_obj(h_media,&pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }

    /*if (pt_thumbnail_info->h_surf == NULL_HANDLE) //for cmpb, it allowed null
    {
        return SVCTXR_INV_HANDLE;
    }
    else */if (pt_thumbnail_info->ui4_thumbnail_width == 0 ||
             pt_thumbnail_info->ui4_thumbnail_height == 0)
    {
        return SVCTXR_INV_ARG;
    }
    pt_media_desc->t_video_thumbnail_info = *pt_thumbnail_info;
    pt_media_desc->b_video_thumbnail_done = FALSE;
    pt_media_desc->b_is_thumbnail_playback= TRUE;

    t_mm.h_media = h_media;
    t_mm.t_speed = MM_SPEED_TYPE_FORWARD_1X;
    t_mm.t_pos_info.t_pos_type = MM_SVC_POS_TYPE_PICTURE_IDX;
    t_mm.t_pos_info.ui2_idx = (UINT16)0;
    t_mm.t_pos_info.ui8_pos_val = pt_thumbnail_info->ui8_pic_idx;

    t_video_region.ui4_x = 0;
    t_video_region.ui4_y = 0;
    t_video_region.ui4_width  = VSH_REGION_MAX_WIDTH;
    t_video_region.ui4_height = VSH_REGION_MAX_HEIGHT;

    t_disp_region.ui4_x = 0;
    t_disp_region.ui4_y = 0;
    t_disp_region.ui4_width  = 0;
    t_disp_region.ui4_height = 0;

    t_video_plane_order.e_order_ctrl = VSH_PLANE_ORDER_CTRL_BOTTOM;
    t_video_plane_order.u.ui1_num_layers = 0;

    t_src_desc.e_type = SRC_TYPE_MEDIA_STORGE;
    t_src_desc.u_info.t_mm = t_mm;

    t_snk_desc.pt_video_region = &t_video_region;
    t_snk_desc.pt_disp_region = &t_disp_region;
    t_snk_desc.pt_video_plane_order = &t_video_plane_order;
    t_snk_desc.pt_video_strm_fltr_fct = _svctx_media_strm_comp_filter_fct;
    t_snk_desc.pv_video_strm_fltr_tag = NULL;
    t_snk_desc.ps_snk_grp_name      = SN_PRES_MAIN_DISPLAY;

    svctx_timer_start(
                        pt_svctx,
                        (pt_thumbnail_info->h_surf == NULL_HANDLE) ? 10000 : DEFAULT_THUMB_TIME_OUT,/* Network case may need long time to read data*/
                        _svctx_thumbnail_timeout_cb,
                        (VOID*)pt_svctx);

    i4_ret = x_svctx_select_svc(
                    h_svctx,
                    ST_MASK_VIDEO,
                    &t_src_desc,
                    &t_snk_desc,
                    _svctx_media_load_thumbnail_select_nfy_fct,
                    (VOID*)h_media
                    );
    if(SVCTXR_OK != i4_ret)
    {
        pt_media_desc->t_video_thumbnail_info.h_surf = NULL_HANDLE;
        pt_media_desc->t_video_thumbnail_info.pf_nfy = NULL;
        pt_media_desc->t_video_thumbnail_info.pv_tag = NULL;
        DBG_ERROR(("x_svctx_select_svc() returns %d\n", i4_ret));
        return SVCTXR_FAIL;
    }
    return SVCTXR_ASYNC_NFY;
}

/*-----------------------------------------------------------------------------
 * Name
 *      x_svctx_media_sync_cancel_load_thumbnail
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
INT32 x_svctx_media_sync_cancel_load_thumbnail(
                    HANDLE_T                        h_svctx,
                    HANDLE_T                        h_media
                    )
{
    INT32 i4_ret = 0;
    SVCTX_T* pt_svctx;
    MEDIA_DESCRIBT_T* pt_media_desc;

    i4_ret = svctx_get_obj(h_svctx, &pt_svctx);
    if (i4_ret != SVCTXR_OK)
    {
        DBG_ERROR((_ERROR_HEADER
                   "svctx_get_obj(h_svctx=%d) failed!\r\n"
                   "i4_ret=%d, pt_svctx=%d\r\n",
                   h_svctx, i4_ret, pt_svctx));
        ASSERT(FALSE, ("%s(), pt_svctx is NULL\r\n", __func__));
    }

    i4_ret = _mm_hdlr_get_media_obj(h_media,&pt_media_desc);
    if (i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }
#ifndef ANDROID
    if(pt_media_desc->t_video_thumbnail_info.h_surf != NULL_HANDLE)
    {
        i4_ret = x_gl_surface_unlock(pt_media_desc->t_video_thumbnail_info.h_surf, NULL);
        if (i4_ret != GLR_OK)
        {
            if(pt_media_desc->t_video_thumbnail_info.pf_nfy != NULL)
            {
                pt_media_desc->t_video_thumbnail_info.pf_nfy(
                                            h_svctx,
                                            pt_svctx->e_state,
                                            SVCTX_NTFY_CODE_MEDIA_ERROR,
                                            ST_VIDEO,
                                            pt_media_desc->t_video_thumbnail_info.pv_tag);
            }
        }
    }
#endif
    svctx_timer_stop(pt_svctx);
    i4_ret = x_svctx_sync_stop_svc(
                    h_svctx,
                    DEFAULT_STOP_SVC_TIMEOUT
                    );
    if(SVCTXR_OK != i4_ret)
    {
        DBG_ERROR(("x_svctx_sync_stop_svc() returns %d\n", i4_ret));
        return i4_ret;
    }
    pt_media_desc->b_is_thumbnail_playback = FALSE;
    pt_media_desc->t_video_thumbnail_info.h_surf = NULL_HANDLE;
    pt_media_desc->t_video_thumbnail_info.pf_nfy = NULL;
    pt_media_desc->t_video_thumbnail_info.pv_tag = NULL;
    pt_svctx->t_disp_region = pt_svctx->t_raw_disp_region;

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_create_uri_media_feeder_instace
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
static INT32 _svctx_create_uri_media_feeder_instace(MEDIA_DESCRIBT_T* pt_media_desc)
{

    INT32           i4_ret = SVCTXR_FAIL;
    HANDLE_T        h_feeder_handle = NULL_HANDLE;
    MEDIA_FEEDER_NW_T*  pt_feeder = NULL;
    MEDIA_FEEDER_T*     pt_input;
    UINT32                  ui4_idx;
    MEDIA_MULTI_CACHE_T     *pt_cache;

    if(pt_media_desc == NULL)
    {
        return SVCTXR_INV_ARG;
    }
    else if(pt_media_desc->ps_media_path == NULL)
    {
        return SVCTXR_INV_ARG;
    }

#ifdef INET_SUPPORT
    if (pt_media_desc->h_uri)
    {
        i4_ret = SVCTXR_OK;
        h_feeder_handle = pt_media_desc->h_uri;
    }
    else
#endif
    {
        i4_ret = x_fm_open(FM_ROOT_HANDLE, pt_media_desc->ps_media_path, 
                           FM_READ_ONLY, 0777, 0, &h_feeder_handle);
        x_dbg_stmt("the opened path is %s \n", pt_media_desc->ps_media_path);
    }


    if(i4_ret != SVCTXR_OK)
    {
        return i4_ret;
    }

    pt_input = (MEDIA_FEEDER_T*)x_mem_alloc(sizeof(MEDIA_FEEDER_T));
    pt_feeder = (MEDIA_FEEDER_NW_T*)x_mem_alloc(sizeof(MEDIA_FEEDER_NW_T));
    if(pt_feeder == NULL || pt_input == NULL)
    {
#ifdef INET_SUPPORT
        if(x_strncasecmp(pt_media_desc->ps_media_path, "http:", 5) == 0)
        {
            x_httpc_fm_close(h_feeder_handle);
        }
        else if(x_strncasecmp(pt_media_desc->ps_media_path, "https:", 6) == 0)
        {
            x_httpc_fm_close(h_feeder_handle);
        }
        else
#endif
        {
            x_fm_close(h_feeder_handle);
        }
        return SVCTXR_NO_RESOURCE;
    }

    x_memset(pt_input, 0, sizeof(MEDIA_FEEDER_T));

    pt_input->h_feeder = (HANDLE_T)pt_feeder;
    x_memset(pt_feeder, 0, sizeof(MEDIA_FEEDER_NW_T));

    pt_feeder->pt_free_cache = &pt_feeder->at_multi_cache[0];
    pt_cache = pt_feeder->pt_free_cache;
    for (ui4_idx=1; ui4_idx<MULTI_CACHE_COUNT; ui4_idx++)
    {
        pt_cache->pt_next = &pt_feeder->at_multi_cache[ui4_idx];
        pt_cache = pt_cache->pt_next;
    }
    pt_cache->pt_next = NULL;

    pt_feeder->e_type = MEDIA_FEEDER_NW_TYPE_URI;

#ifdef INET_SUPPORT
    pt_feeder->ui8_file_size = pt_media_desc->ui8_media_sz;
#else
    {
        UINT64              ui8_start_pos = 0;
        UINT64              ui8_end_pos = 0;
        x_fm_lseek(h_feeder_handle, 0, FM_SEEK_BGN, &ui8_start_pos);
        x_fm_lseek(h_feeder_handle, 0, FM_SEEK_END, &ui8_end_pos);
        pt_feeder->ui8_file_size = ui8_end_pos - ui8_start_pos;
    }
#endif

    pt_feeder->h_feeder = h_feeder_handle;

#ifdef INET_SUPPORT
    pt_feeder->t_feeder_le_fct_tbl.pf_copybytes = mm_util_network_uri_copybytes_fct;
    pt_feeder->t_feeder_le_fct_tbl.pf_input4 = mm_util_network_uri_input4bytes_l_fct;
    pt_feeder->t_feeder_le_fct_tbl.pf_input3 = mm_util_network_uri_input3bytes_l_fct;
    pt_feeder->t_feeder_le_fct_tbl.pf_input2 = mm_util_network_uri_input2bytes_l_fct;
    pt_feeder->t_feeder_le_fct_tbl.pf_input1 = mm_util_network_uri_input1bytes_fct;
    pt_feeder->t_feeder_le_fct_tbl.pf_set_pos = mm_util_network_uri_setpos_fct;
    pt_feeder->t_feeder_le_fct_tbl.pf_get_pos = mm_util_network_uri_getpos_fct;
    
    pt_feeder->t_feeder_be_fct_tbl.pf_copybytes = mm_util_network_uri_copybytes_fct;
    pt_feeder->t_feeder_be_fct_tbl.pf_input4 = mm_util_network_uri_input4bytes_b_fct;
    pt_feeder->t_feeder_be_fct_tbl.pf_input3 = mm_util_network_uri_input3bytes_b_fct;
    pt_feeder->t_feeder_be_fct_tbl.pf_input2 = mm_util_network_uri_input2bytes_b_fct;
    pt_feeder->t_feeder_be_fct_tbl.pf_input1 = mm_util_network_uri_input1bytes_fct;
    pt_feeder->t_feeder_be_fct_tbl.pf_set_pos = mm_util_network_uri_setpos_fct;
    pt_feeder->t_feeder_be_fct_tbl.pf_get_pos = mm_util_network_uri_getpos_fct;
    
    
    pt_input->t_feeder_le_fct_tbl.pf_copybytes = mm_util_network_uri_copybytes_fct;
    pt_input->t_feeder_le_fct_tbl.pf_input4 = mm_util_network_uri_input4bytes_l_fct;
    pt_input->t_feeder_le_fct_tbl.pf_input3 = mm_util_network_uri_input3bytes_l_fct;
    pt_input->t_feeder_le_fct_tbl.pf_input2 = mm_util_network_uri_input2bytes_l_fct;
    pt_input->t_feeder_le_fct_tbl.pf_input1 = mm_util_network_uri_input1bytes_fct;
    pt_input->t_feeder_le_fct_tbl.pf_set_pos = mm_util_network_uri_setpos_fct;
    pt_input->t_feeder_le_fct_tbl.pf_get_pos = mm_util_network_uri_getpos_fct;
    
    pt_input->t_feeder_be_fct_tbl.pf_copybytes = mm_util_network_uri_copybytes_fct;
    pt_input->t_feeder_be_fct_tbl.pf_input4 = mm_util_network_uri_input4bytes_b_fct;
    pt_input->t_feeder_be_fct_tbl.pf_input3 = mm_util_network_uri_input3bytes_b_fct;
    pt_input->t_feeder_be_fct_tbl.pf_input2 = mm_util_network_uri_input2bytes_b_fct;
    pt_input->t_feeder_be_fct_tbl.pf_input1 = mm_util_network_uri_input1bytes_fct;
    pt_input->t_feeder_be_fct_tbl.pf_set_pos = mm_util_network_uri_setpos_fct;
    pt_input->t_feeder_be_fct_tbl.pf_get_pos = mm_util_network_uri_getpos_fct;
#else
    pt_feeder->t_feeder_le_fct_tbl.pf_copybytes = mm_util_fcopybytes_fct;
    pt_feeder->t_feeder_le_fct_tbl.pf_input4 = mm_util_finput4bytes_l_fct;
    pt_feeder->t_feeder_le_fct_tbl.pf_input3 = mm_util_finput3bytes_l_fct;
    pt_feeder->t_feeder_le_fct_tbl.pf_input2 = mm_util_finput2bytes_l_fct;
    pt_feeder->t_feeder_le_fct_tbl.pf_input1 = mm_util_finput1bytes_fct;
    pt_feeder->t_feeder_le_fct_tbl.pf_set_pos = mm_util_fsetpos_fct;
    pt_feeder->t_feeder_le_fct_tbl.pf_get_pos = mm_util_fgetpos_fct;

    pt_feeder->t_feeder_be_fct_tbl.pf_copybytes = mm_util_fcopybytes_fct;
    pt_feeder->t_feeder_be_fct_tbl.pf_input4 = mm_util_finput4bytes_b_fct;
    pt_feeder->t_feeder_be_fct_tbl.pf_input3 = mm_util_finput3bytes_b_fct;
    pt_feeder->t_feeder_be_fct_tbl.pf_input2 = mm_util_finput2bytes_b_fct;
    pt_feeder->t_feeder_be_fct_tbl.pf_input1 = mm_util_finput1bytes_fct;
    pt_feeder->t_feeder_be_fct_tbl.pf_set_pos = mm_util_fsetpos_fct;
    pt_feeder->t_feeder_be_fct_tbl.pf_get_pos = mm_util_fgetpos_fct;
    

    pt_input->t_feeder_le_fct_tbl.pf_copybytes = mm_util_fcopybytes_fct;
    pt_input->t_feeder_le_fct_tbl.pf_input4 = mm_util_finput4bytes_l_fct;
    pt_input->t_feeder_le_fct_tbl.pf_input3 = mm_util_finput3bytes_l_fct;
    pt_input->t_feeder_le_fct_tbl.pf_input2 = mm_util_finput2bytes_l_fct;
    pt_input->t_feeder_le_fct_tbl.pf_input1 = mm_util_finput1bytes_fct;
    pt_input->t_feeder_le_fct_tbl.pf_set_pos = mm_util_fsetpos_fct;
    pt_input->t_feeder_le_fct_tbl.pf_get_pos = mm_util_fgetpos_fct;
    
    pt_input->t_feeder_be_fct_tbl.pf_copybytes = mm_util_fcopybytes_fct;
    pt_input->t_feeder_be_fct_tbl.pf_input4 = mm_util_finput4bytes_b_fct;
    pt_input->t_feeder_be_fct_tbl.pf_input3 = mm_util_finput3bytes_b_fct;
    pt_input->t_feeder_be_fct_tbl.pf_input2 = mm_util_finput2bytes_b_fct;
    pt_input->t_feeder_be_fct_tbl.pf_input1 = mm_util_finput1bytes_fct;
    pt_input->t_feeder_be_fct_tbl.pf_set_pos = mm_util_fsetpos_fct;
    pt_input->t_feeder_be_fct_tbl.pf_get_pos = mm_util_fgetpos_fct;
#endif
    
    pt_media_desc->pt_media_feeder = (MEDIA_FEEDER_T*)pt_input;

    return SVCTXR_OK;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_close_media_feeder_instace
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
static INT32 _svctx_close_media_feeder_instace(MEDIA_DESCRIBT_T* pt_media_desc)
{
    MEDIA_FEEDER_NW_T*  pt_feeder;

    if(pt_media_desc->pt_media_feeder != NULL)
    {
        pt_feeder = (MEDIA_FEEDER_NW_T*)pt_media_desc->pt_media_feeder->h_feeder;

        if (pt_feeder->e_type == MEDIA_FEEDER_NW_TYPE_URI)
        {
#ifdef INET_SUPPORT
            if(x_strncasecmp(pt_media_desc->ps_media_path, "http:", 5) == 0)
            {
                x_httpc_fm_close(pt_feeder->h_feeder);
            }
            else if(x_strncasecmp(pt_media_desc->ps_media_path, "https:", 6) == 0)
            {
                x_httpc_fm_close(pt_feeder->h_feeder);
            }
            else
#endif
            {
                x_fm_close(pt_feeder->h_feeder);
            }
        }
        else
        {
            if (pt_media_desc->t_playback_ctrl_pull_info.pf_close)
            {
#ifdef LINUX_TURNKEY_SOLUTION
                if (pt_feeder->aui1_share_buf)
                {
                    x_munmap_share_mem(pt_feeder->aui1_share_buf, NW_FEEDER_CACHE_SIZE);
                }

                if (pt_feeder->aui1_phy_buf)
                {
                    x_share_mem_free(pt_feeder->aui1_phy_buf, NW_FEEDER_CACHE_SIZE);
                }

                if (pt_media_desc->t_playback_ctrl_pull_info.pf_dissociate)
                {
                    pt_media_desc->t_playback_ctrl_pull_info.pf_dissociate(
                                        pt_feeder->h_pull,
                                        pt_media_desc->t_playback_ctrl_pull_info.pv_app_tag);
                }
#endif

                pt_media_desc->t_playback_ctrl_pull_info.pf_close(
                                    pt_feeder->h_pull,
                                    pt_media_desc->t_playback_ctrl_pull_info.pv_app_tag);
            }
            else
            {
                /*something wrong*/
                x_dbg_stmt("mm_hdlr : null pf_close in pt_media_desc->t_playback_ctrl_pull_info\n");
            }
        }

        x_mem_free(pt_feeder);
        x_mem_free(pt_media_desc->pt_media_feeder);
        pt_media_desc->pt_media_feeder = NULL;
    }
    
    return SVCTXR_OK; 
}

/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_create_pull_media_feeder_instace
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
static INT32 _svctx_create_pull_media_feeder_instace(MEDIA_DESCRIBT_T* pt_media_desc)
{
    INT32               i4_ret = SVCTXR_OK;
    HANDLE_T            h_feeder_handle = NULL_HANDLE;
    MEDIA_FEEDER_NW_T*  pt_feeder = NULL;
    MEDIA_FEEDER_T*     pt_input;

    if(pt_media_desc == NULL)
    {
        return SVCTXR_INV_ARG;
    }
    /*else if(pt_media_desc->ps_media_path == NULL)
    {
        return SVCTXR_INV_ARG;
    }*/
    
    pt_input = (MEDIA_FEEDER_T*)x_mem_alloc(sizeof(MEDIA_FEEDER_T));
    pt_feeder = (MEDIA_FEEDER_NW_T*)x_mem_alloc(sizeof(MEDIA_FEEDER_NW_T));
    if(pt_feeder == NULL || pt_input == NULL)
    {
        return SVCTXR_NO_RESOURCE;
    }
    x_memset(pt_input, 0, sizeof(MEDIA_FEEDER_T));
    pt_input->h_feeder = (HANDLE_T)pt_feeder;
    x_memset(pt_feeder, 0, sizeof(MEDIA_FEEDER_NW_T));

    pt_feeder->e_type = MEDIA_FEEDER_NW_TYPE_PULL;
    
    pt_feeder->h_feeder = h_feeder_handle;

    pt_feeder->t_feeder_le_fct_tbl.pf_copybytes = mm_util_network_pull_copybytes_fct;
    pt_feeder->t_feeder_le_fct_tbl.pf_input4 = mm_util_network_pull_input4bytes_l_fct;
    pt_feeder->t_feeder_le_fct_tbl.pf_input3 = mm_util_network_pull_input3bytes_l_fct;
    pt_feeder->t_feeder_le_fct_tbl.pf_input2 = mm_util_network_pull_input2bytes_l_fct;
    pt_feeder->t_feeder_le_fct_tbl.pf_input1 = mm_util_network_pull_input1bytes_fct;
    pt_feeder->t_feeder_le_fct_tbl.pf_set_pos = mm_util_network_pull_setpos_fct;
    pt_feeder->t_feeder_le_fct_tbl.pf_get_pos = mm_util_network_pull_getpos_fct;
    
    pt_feeder->t_feeder_be_fct_tbl.pf_copybytes = mm_util_network_pull_copybytes_fct;
    pt_feeder->t_feeder_be_fct_tbl.pf_input4 = mm_util_network_pull_input4bytes_b_fct;
    pt_feeder->t_feeder_be_fct_tbl.pf_input3 = mm_util_network_pull_input3bytes_b_fct;
    pt_feeder->t_feeder_be_fct_tbl.pf_input2 = mm_util_network_pull_input2bytes_b_fct;
    pt_feeder->t_feeder_be_fct_tbl.pf_input1 = mm_util_network_pull_input1bytes_fct;
    pt_feeder->t_feeder_be_fct_tbl.pf_set_pos = mm_util_network_pull_setpos_fct;
    pt_feeder->t_feeder_be_fct_tbl.pf_get_pos = mm_util_network_pull_getpos_fct;

    /*store pull read function pointer*/
    pt_feeder->t_pull_read_fct_tbl = pt_media_desc->t_playback_ctrl_pull_info;

    pt_feeder->pv_app_tag = pt_media_desc->t_playback_ctrl_pull_info.pv_app_tag;
    if (pt_media_desc->t_playback_ctrl_pull_info.pf_open)
    {
        i4_ret = pt_media_desc->t_playback_ctrl_pull_info.pf_open(
                            pt_media_desc->t_playback_ctrl_pull_info.h_pb,
                            &pt_feeder->h_pull,
                            pt_media_desc->t_playback_ctrl_pull_info.pv_app_tag);
        if(i4_ret != SVCTXR_OK)
        {
            return i4_ret;
        }

#ifdef LINUX_TURNKEY_SOLUTION
        if (pt_feeder->t_pull_read_fct_tbl.pf_associate)
        {
            pt_feeder->aui1_phy_buf = x_share_mem_alloc(NW_FEEDER_CACHE_SIZE);
            
            pt_feeder->aui1_share_buf = x_mmap_share_mem_rmmgr(pt_feeder->aui1_phy_buf, NW_FEEDER_CACHE_SIZE);

            pt_feeder->t_pull_read_fct_tbl.pf_associate(
                pt_feeder->h_pull,
                pt_feeder->t_pull_read_fct_tbl.pv_app_tag,
                (UINT32)pt_feeder->aui1_phy_buf,
                NW_FEEDER_CACHE_SIZE);

            //printf("create pull feeder: phisical addr = %x, share mem addr = %x \n", 
                //(unsigned int)pt_feeder->aui1_phy_buf, (unsigned int)pt_feeder->aui1_share_buf);

        }
#endif

        if (pt_media_desc->t_playback_ctrl_pull_info.pf_get_input_len)
        {
            pt_media_desc->t_playback_ctrl_pull_info.pf_get_input_len(
                                pt_feeder->h_pull,
                                pt_media_desc->t_playback_ctrl_pull_info.pv_app_tag,
                                &pt_feeder->ui8_file_size);
            pt_media_desc->ui8_media_sz = pt_feeder->ui8_file_size;
        }
        else
        {
            pt_feeder->ui8_file_size = (UINT64)-1;
            pt_media_desc->ui8_media_sz = (UINT64)-1;
        }

    }
    else
    {
        /*something wrong*/
        pt_feeder->h_pull = NULL_HANDLE;
        x_dbg_stmt("mm_hdlr : null pf_open in pt_media_desc->t_playback_ctrl_pull_info\n");
    }

    pt_input->t_feeder_le_fct_tbl.pf_copybytes = mm_util_network_pull_copybytes_fct;
    pt_input->t_feeder_le_fct_tbl.pf_input4 = mm_util_network_pull_input4bytes_l_fct;
    pt_input->t_feeder_le_fct_tbl.pf_input3 = mm_util_network_pull_input3bytes_l_fct;
    pt_input->t_feeder_le_fct_tbl.pf_input2 = mm_util_network_pull_input2bytes_l_fct;
    pt_input->t_feeder_le_fct_tbl.pf_input1 = mm_util_network_pull_input1bytes_fct;
    pt_input->t_feeder_le_fct_tbl.pf_set_pos = mm_util_network_pull_setpos_fct;
    pt_input->t_feeder_le_fct_tbl.pf_get_pos = mm_util_network_pull_getpos_fct;
    
    pt_input->t_feeder_be_fct_tbl.pf_copybytes = mm_util_network_pull_copybytes_fct;
    pt_input->t_feeder_be_fct_tbl.pf_input4 = mm_util_network_pull_input4bytes_b_fct;
    pt_input->t_feeder_be_fct_tbl.pf_input3 = mm_util_network_pull_input3bytes_b_fct;
    pt_input->t_feeder_be_fct_tbl.pf_input2 = mm_util_network_pull_input2bytes_b_fct;
    pt_input->t_feeder_be_fct_tbl.pf_input1 = mm_util_network_pull_input1bytes_fct;
    pt_input->t_feeder_be_fct_tbl.pf_set_pos = mm_util_network_pull_setpos_fct;
    pt_input->t_feeder_be_fct_tbl.pf_get_pos = mm_util_network_pull_getpos_fct;

    
    pt_media_desc->pt_media_feeder = (MEDIA_FEEDER_T*)pt_input;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_get_favor_format_by_ext
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
static INT32 _svctx_get_favor_format_by_ext(MEDIA_DESCRIBT_T* pt_media_desc,
                                            MEDIA_FORMAT_T*   pt_format)
{
    if(pt_media_desc->ps_media_path != NULL)
    {
        CHAR*   ps_extension = NULL; 
        UINT32  ui4_ext_value = 0;
        UINT32  ui4_str_length;
        UINT32  ui4_idx;
        UINT8   aui1_str[5];
        
        ps_extension = NULL;
        ps_extension = x_strstr(pt_media_desc->ps_media_path, ".mp4" );
        if (ps_extension)
        {
            pt_format->e_media_type = MEDIA_TYPE_CONTAINER;
            pt_format->t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_MP4;
            return SVCTXR_OK;
        }
        x_memset(aui1_str, 0, 5);
        ps_extension = x_strrchr(pt_media_desc->ps_media_path, '.' );/* ".mp3" */
        
        if (ps_extension == NULL)
        {
            return SVCTXR_FAIL; 
        }
        
        ps_extension++; /* cut off'.'=>"mp3" */ 
        ui4_str_length = x_strlen(ps_extension);
        for (ui4_idx=0; ui4_idx<4 && ui4_idx<ui4_str_length; ui4_idx++)
        {
            aui1_str[ui4_idx] = (UINT8)ps_extension[ui4_idx];
        }
        
        ps_extension = x_str_tolower((CHAR *)aui1_str);

        while(*ps_extension)
        {
            ui4_ext_value = (ui4_ext_value<<8) | ((UINT8) (*ps_extension++));
        }
        if((ui4_ext_value>>24) == 0)
        {
            if((ui4_ext_value>>16) == 0)
            {
                ui4_ext_value <<= 16;
            }
            else
            {
                ui4_ext_value <<= 8;
            }
        }
        switch(ui4_ext_value)
        {
        case dwBigFourCC('m','p','3',0):
        case dwBigFourCC('m','3','u',0):
        case dwBigFourCC('m','3','u','8'):
            pt_format->e_media_type = MEDIA_TYPE_AUDIO_ES;
            break;
        case dwBigFourCC('a','i','f',0):
            pt_format->e_media_type = MEDIA_TYPE_AUDIO_ES;
            pt_format->t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_AIF;
            break;
        case dwBigFourCC('p','c','m',0):
        case dwBigFourCC('a','a','c',0):
        case dwBigFourCC('a','c','3',0):
            pt_format->e_media_type = MEDIA_TYPE_AUDIO_ES;
            break;            
        case dwBigFourCC('m','2','v',0):
        case dwBigFourCC('2','6','4',0):
        case dwBigFourCC('v','c','l',0):
            pt_format->e_media_type = MEDIA_TYPE_VIDEO_ES;
            break;
        case dwBigFourCC('a','v','i',0):
        case dwBigFourCC('d','i','v','x'):
        case dwBigFourCC('w','a','v',0):
            pt_format->e_media_type = MEDIA_TYPE_CONTAINER;
            pt_format->t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_AVI;
            break;
        case dwBigFourCC('m','p','4',0):
        case dwBigFourCC('m','o','v',0):
        case dwBigFourCC('m','4','v',0):
        case dwBigFourCC('m','4','a',0):
        case dwBigFourCC('3','g','p',0):
            pt_format->e_media_type = MEDIA_TYPE_CONTAINER;
            pt_format->t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_MP4;
            break;
        case dwBigFourCC('a','s','f',0):
        case dwBigFourCC('w','m','v',0):
        case dwBigFourCC('a','s','x',0):
        case dwBigFourCC('w','m','a',0):
            pt_format->e_media_type = MEDIA_TYPE_CONTAINER;
            pt_format->t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_ASF;
            break;
        case dwBigFourCC('m','p','g',0):
        case dwBigFourCC('m','p','e','g'):
        case dwBigFourCC('v','o','b',0):
        case dwBigFourCC('d','a','t',0):
        case dwBigFourCC('m','1','v',0):
        case dwBigFourCC('m','p','e',0):
        case dwBigFourCC('m','t','s',0):
            pt_format->e_media_type = MEDIA_TYPE_PS;
            break;
        case dwBigFourCC('m','k','v',0):
            pt_format->e_media_type = MEDIA_TYPE_CONTAINER;
            pt_format->t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_MATROSKA;
            break;
        case dwBigFourCC('t','s',  0,0):
        case dwBigFourCC('m','2','t','s'):
        case dwBigFourCC('p','v','r',0):
        case dwBigFourCC('t','r','p',0):
            pt_format->e_media_type = MEDIA_TYPE_TS;
            break;
        case dwBigFourCC('t','t','s',0):
            break;
        case dwBigFourCC('r','m',  0,0):
        case dwBigFourCC('r','m','v','b'):
            pt_format->e_media_type = MEDIA_TYPE_CONTAINER;
            pt_format->t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_RM;
            break;
        case dwBigFourCC('f','l','v',0):
            pt_format->e_media_type = MEDIA_TYPE_CONTAINER;
            pt_format->t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_FLV;
            break;
        case dwBigFourCC('o','g','m',0):
        case dwBigFourCC('o','g','g',0):
            pt_format->e_media_type = MEDIA_TYPE_CONTAINER;
            pt_format->t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_OGG;
        }
    }
    
    return SVCTXR_OK; 
}


/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_set_meta_info
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
static INT32 _svctx_set_meta_info(MEDIA_DESCRIBT_T* pt_media_desc,
                                  MINFO_INFO_T*     pt_meta_info)
{
    if((pt_media_desc == NULL) || (pt_meta_info == NULL))
    {
        return SVCTXR_INV_ARG;
    }
    /* copy stream information to pt_media_desc */
    pt_media_desc->pt_meta_info = (MINFO_INFO_T*)x_mem_alloc(sizeof(MINFO_INFO_T));
    if(pt_media_desc->pt_meta_info == NULL)
    {
       return SVCTXR_NO_RESOURCE;
    }
    x_memcpy((VOID*)(pt_media_desc->pt_meta_info), (VOID*)pt_meta_info, sizeof(MINFO_INFO_T));

    /* alloc and copy more pointer data if there are pionters in substructures */
    if(pt_meta_info->u.t_meta_data.pwc_str_title && 
       pt_meta_info->u.t_meta_data.z_title_len)
    {
        pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_title = 
            (UTF16_T*)x_mem_alloc(pt_meta_info->u.t_meta_data.z_title_len+sizeof(UTF16_T));
        if(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_title == NULL)
        {
           return SVCTXR_NO_RESOURCE;
        }
        x_memcpy((VOID*)(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_title), 
            (VOID*)pt_meta_info->u.t_meta_data.pwc_str_title, 
              pt_meta_info->u.t_meta_data.z_title_len);
    }

    if(pt_meta_info->u.t_meta_data.pwc_str_director && 
       pt_meta_info->u.t_meta_data.z_director_len)
    {
        pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_director = 
            (UTF16_T*)x_mem_alloc(pt_meta_info->u.t_meta_data.z_director_len+sizeof(UTF16_T));
        if(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_director == NULL)
        {
           return SVCTXR_NO_RESOURCE;
        }
        x_memcpy((VOID*)(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_director), 
            (VOID*)pt_meta_info->u.t_meta_data.pwc_str_director, 
              pt_meta_info->u.t_meta_data.z_director_len);
    }

    if(pt_meta_info->u.t_meta_data.pwc_str_copyright && 
       pt_meta_info->u.t_meta_data.z_copyright_len)
    {
        pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_copyright = 
            (UTF16_T*)x_mem_alloc(pt_meta_info->u.t_meta_data.z_copyright_len+sizeof(UTF16_T));
        if(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_copyright == NULL)
        {
           return SVCTXR_NO_RESOURCE;
        }
        x_memcpy((VOID*)(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_copyright), 
            (VOID*)pt_meta_info->u.t_meta_data.pwc_str_copyright, 
              pt_meta_info->u.t_meta_data.z_copyright_len);
    }

    if(pt_meta_info->u.t_meta_data.pwc_str_genre && 
       pt_meta_info->u.t_meta_data.z_genre_len)
    {
        pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_genre = 
            (UTF16_T*)x_mem_alloc(pt_meta_info->u.t_meta_data.z_genre_len+sizeof(UTF16_T));
        if(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_genre == NULL)
        {
           return SVCTXR_NO_RESOURCE;
        }
        x_memcpy((VOID*)(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_genre), 
            (VOID*)pt_meta_info->u.t_meta_data.pwc_str_genre, 
              pt_meta_info->u.t_meta_data.z_genre_len);
    }

    if(pt_meta_info->u.t_meta_data.pwc_str_artist && 
       pt_meta_info->u.t_meta_data.z_artist_len)
    {
        pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_artist = 
            (UTF16_T*)x_mem_alloc(pt_meta_info->u.t_meta_data.z_artist_len+sizeof(UTF16_T));
        if(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_artist == NULL)
        {
           return SVCTXR_NO_RESOURCE;
        }
        x_memcpy((VOID*)(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_artist), 
            (VOID*)pt_meta_info->u.t_meta_data.pwc_str_artist, 
              pt_meta_info->u.t_meta_data.z_artist_len);
    }

    if(pt_meta_info->u.t_meta_data.pwc_str_album && 
       pt_meta_info->u.t_meta_data.z_album_len)
    {
        pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_album = 
            (UTF16_T*)x_mem_alloc(pt_meta_info->u.t_meta_data.z_album_len+sizeof(UTF16_T));
        if(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_album == NULL)
        {
           return SVCTXR_NO_RESOURCE;
        }
        x_memcpy((VOID*)(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_album), 
            (VOID*)pt_meta_info->u.t_meta_data.pwc_str_album, 
              pt_meta_info->u.t_meta_data.z_album_len);
    }

    if(pt_meta_info->u.t_meta_data.pwc_str_track && 
       pt_meta_info->u.t_meta_data.z_track_len)
    {
        pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_track = 
            (UTF16_T*)x_mem_alloc(pt_meta_info->u.t_meta_data.z_track_len+sizeof(UTF16_T));
        if(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_track == NULL)
        {
           return SVCTXR_NO_RESOURCE;
        }
        x_memcpy((VOID*)(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_track), 
            (VOID*)pt_meta_info->u.t_meta_data.pwc_str_track, 
              pt_meta_info->u.t_meta_data.z_track_len);
    }

    if(pt_meta_info->u.t_meta_data.pwc_str_year && 
       pt_meta_info->u.t_meta_data.z_year_len)
    {
        pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_year = 
            (UTF16_T*)x_mem_alloc(pt_meta_info->u.t_meta_data.z_year_len+sizeof(UTF16_T));
        if(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_year == NULL)
        {
           return SVCTXR_NO_RESOURCE;
        }
        x_memcpy((VOID*)(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_year), 
            (VOID*)pt_meta_info->u.t_meta_data.pwc_str_year, 
              pt_meta_info->u.t_meta_data.z_year_len);
    }            
    return SVCTXR_OK;
}


/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_free_meta_info
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
static VOID _svctx_free_meta_info(MEDIA_DESCRIBT_T* pt_media_desc)
{
    if(pt_media_desc == NULL)
    {
        return ;
    }
    if (pt_media_desc->pt_meta_info != NULL)
    {
        if(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_title)
        {
            x_mem_free(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_title);
            pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_title = NULL;       
        }

        if(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_director)
        {
            x_mem_free(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_director);
            pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_director = NULL;       
        }

        if(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_copyright)
        {
            x_mem_free(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_copyright);
            pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_copyright = NULL;       
        }

        if(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_genre)
        {
            x_mem_free(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_genre);
            pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_genre = NULL;       
        }

        if(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_artist)
        {
            x_mem_free(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_artist);
            pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_artist = NULL;       
        }

        if(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_album)
        {
            x_mem_free(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_album);
            pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_album = NULL;       
        }
        
        if(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_track)
        {
            x_mem_free(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_track);
            pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_track = NULL;       
        }

        if(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_year)
        {
            x_mem_free(pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_year);
            pt_media_desc->pt_meta_info->u.t_meta_data.pwc_str_year = NULL;       
        }
        
        x_mem_free(pt_media_desc->pt_meta_info);
        pt_media_desc->pt_meta_info = NULL;
    }
}

#ifdef ENABLE_MMS_SUPPORT
#define MMS_BUFFER_SIZE     4096*10
static INT32 _svctx_get_mms_header(CHAR* pc_mms_file, CHAR** pc_header_buf, 
                                         MMS_HANDLE_T* ph_mms_handle, UINT32* pui4_hdr_sz)
{
    UINT64 ui8_hdr_len = 0;
    INT32  i4_read_len = 0;
    CHAR*  pc_hdr_buf = NULL;
    MMS_ERROR_CODE_T  e_ret;
    MMS_HANDLE_T    h_file = 0;

    if(pc_mms_file == NULL   || pc_header_buf == NULL || 
       ph_mms_handle == NULL || pui4_hdr_sz == NULL)
    {
        return SVCTXR_FAIL;
    }

    do
    {
        x_mms_fm_pull_init();
        
        e_ret = x_mms_fm_pull_open(pc_mms_file, MMS_BUFFER_SIZE, &h_file);
        if(e_ret != MMS_ERROR_CODE_OK)
        {
            break;
        }
        
        e_ret = x_mms_fm_pull_get_asf_header_len(h_file, NULL, &ui8_hdr_len);
        if(e_ret != MMS_ERROR_CODE_OK)
        {
            break;
        }

        pc_hdr_buf = x_mem_alloc((ui8_hdr_len + 4) & ~4);/*4 alliment*/
        if(pc_hdr_buf == NULL)
        {
            break;
        }
        x_memset(pc_hdr_buf, 0, (ui8_hdr_len + 4) & ~4);

        e_ret = x_mms_fm_pull_peek_header(h_file,
                                          NULL,
                                          pc_hdr_buf,
                                          (INT32)((ui8_hdr_len + 4) & ~4),
                                          &i4_read_len);
        if(e_ret != MMS_ERROR_CODE_OK)
        {
            break;
        }
        
        *pc_header_buf = pc_hdr_buf;
        *ph_mms_handle = h_file;
        *pui4_hdr_sz   = (UINT32)i4_read_len;

        return SVCTXR_OK;
    }while(0);

    if(pc_hdr_buf)
    {
        x_mem_free(pc_hdr_buf);
        pc_hdr_buf = NULL;
    }
    if(h_file)
    {
        x_mms_fm_pull_close(h_file, NULL);
        h_file = 0;
    }
    
    return SVCTXR_FAIL;
}

static INT32 _svctx_release_mms_header(MMS_HANDLE_T h_file, CHAR* pc_header_buf)
{
    if(pc_header_buf)
    {
        x_mem_free(pc_header_buf);
    }
    
    if(h_file)
    {
        x_mms_fm_pull_close(h_file, NULL);
    }

    return SVCTXR_OK;
}
#endif

#ifdef MM_LAST_MEMORY_SUPPORT
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_get_mm_identifier
 * Description: 
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
static INT32 _svctx_get_mm_identifier(MEDIA_DESCRIBT_T*     pt_media_desc)
{
    ID_CVT_CNDTN_T at_cndtn[2];
    INT32          i4_ret           = SVCTXR_OK;

    if(pt_media_desc == NULL)
    {
        return SVCTXR_INV_ARG;
    }

    if(pt_media_desc->e_src_type != SRC_TYPE_MEDIA_STORGE &&
       pt_media_desc->e_src_type != SRC_TYPE_MEDIA_PVR)
    {
        return SVCTXR_OK;
    }

    do
    {
        x_memset((VOID*)at_cndtn, 0, sizeof(ID_CVT_CNDTN_T)*2);
        
        if ((pt_media_desc->e_svc_protocol == PROTOCL_MODE_URI ||
            pt_media_desc->e_svc_protocol == PROTOCL_MODE_PULL)
            && (pt_media_desc->e_src_type == SRC_TYPE_MEDIA_NETWORK))
        {              
            i4_ret = _svctx_get_mem_identifier((const VOID*)pt_media_desc->pt_media_feeder, 
                                               #ifdef ENABLE_MMS_SUPPORT
                                               (SIZE_T)pt_media_desc->ui4_mms_hdr_sz,  
                                               #else
                                               (SIZE_T)0,
                                               #endif
                                               (VOID*)&pt_media_desc->ui8_mm_identifier,
                                               (SIZE_T)sizeof(UINT64));
            if (i4_ret != SVCTXR_OK)
            {
                x_dbg_stmt("\n[error][mm_svctx] _svctx_get_mem_identifier fail ret=%d line=%d\n", i4_ret, __LINE__);
                break;
            }
        }
        else
        {
            i4_ret = _svctx_get_file_identifier((const CHAR*)pt_media_desc->ps_media_path, 
                                                (VOID*)&pt_media_desc->ui8_mm_identifier,
                                                (SIZE_T)sizeof(UINT64));
            if (i4_ret != SVCTXR_OK)
            {
                x_dbg_stmt("\n[error][mm_svctx] _svctx_get_file_identifier fail ret=%d line=%d\n", i4_ret, __LINE__);
                break;
            }               
        }
        
        at_cndtn[0].e_cndtn_type = ID_CNDTN_SRC;
        at_cndtn[0].u.t_src_val  = (UINT8)pt_media_desc->e_src_type;
        at_cndtn[1].e_cndtn_type = ID_CNDTN_END;
        
        i4_ret = _svctx_cvt_identifier(&pt_media_desc->ui8_mm_identifier,
                                       at_cndtn);
        if (i4_ret != SVCTXR_OK)
        {
            x_dbg_stmt("\n[error][mm_svctx] _svctx_cvt_identifier fail ret=%d line=%d\n", i4_ret, __LINE__);
            break;
        }
    }while(FALSE);

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_get_file_identifier
 * Description: compute the file identifier by Getting a bit per regular size.
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
static INT32 _svctx_get_file_identifier(const CHAR*     ps_path,
                                          VOID*            pv_get_info,
                                          SIZE_T           z_info_size)
{    
    INT32                       i4_ret          = FMR_OK;
    UINT64                      ui8_magic_num   = 0;
    UINT64                      ui8_div_size;
    UINT8                       ui1_i, ui1_tag;
    UINT64                      ui8_cur_pos;
    UINT32                      ui4_read;
    HANDLE_T                    h_file;
    
    /* commom get magic number */
    if(ps_path == NULL || pv_get_info == NULL)
    {
        return SVCTXR_INV_ARG;
    }
    
    /* open file */
    i4_ret = x_fm_mfw_open(FM_ROOT_HANDLE, ps_path, 
                       FM_READ_ONLY, 0777, 
                       FALSE, &h_file);
    if(i4_ret != FMR_OK)
    {
        return SVCTXR_INV_ARG;
    }
    
    do{
        /* get file size */
        i4_ret = x_fm_mfw_lseek(h_file, 0, FM_SEEK_END, &ui8_cur_pos);
        if(i4_ret != FMR_OK)
        {
            break;
        }
        if(ui8_cur_pos < MAX_MUTI_CLIP_LOOPS_CNT)
        {
            /*think
                       have a defect: if 64 < the size of mutimeida < 128, the magic number just obtain the first 64 bytes.
                       if the size of mutimeida < 64, the magic number is zero. 
                   */
            ui8_magic_num = 0;
            break;
        }
        
        ui8_cur_pos -= MAX_MUTI_CLIP_LOOPS_CNT;
        ui8_div_size = _mm_div64(ui8_cur_pos, 64, NULL);  
        i4_ret = x_fm_mfw_lseek(h_file, 0, FM_SEEK_BGN, &ui8_cur_pos);
        if(i4_ret != FMR_OK)
        { 
            break;
        }
        for(ui1_i=1; ui1_i<(MAX_MUTI_CLIP_LOOPS_CNT + 1); ui1_i++)
        {
            i4_ret = x_fm_mfw_lseek(h_file, ui8_div_size, FM_SEEK_CUR, &ui8_cur_pos);
            if(i4_ret != FMR_OK)
            { 
                break;
            }
            i4_ret = x_fm_mfw_read(h_file, &ui1_tag, 1, &ui4_read);
            if(i4_ret != FMR_OK)
            {
                break;
            }
            ui1_tag = ui1_tag & 0x01;
            if(ui1_i == 1)
            {
                ui8_magic_num = (UINT64)ui1_tag;
            }
            else
            {
                ui8_magic_num |=((UINT64)ui1_tag)<<(ui1_i-1);
            }
        }
        if (i4_ret != FMR_OK)
        {
            break;
        }
    }while(FALSE);
    
    if (i4_ret == FMR_OK)
    {
        x_memcpy(pv_get_info, (const VOID*)&ui8_magic_num, z_info_size);
        i4_ret = SVCTXR_OK;
    }
    else
    {
        i4_ret = SVCTXR_FAIL;
    }
    
    if(x_fm_mfw_close(h_file) != FMR_OK)
    {
        return SVCTXR_INV_HANDLE;
    }
    
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_get_mem_identifier
 * Description
 * Inputs:
 * Outputs:
 * Returns:
 *---------------------------------------------------------------------------*/
static INT32 _svctx_get_mem_identifier(const VOID*      pv_mem_data, 
                                            SIZE_T           z_mem_leng,
                                            VOID*            pv_get_info,
                                            SIZE_T           z_info_size)
{
    MEDIA_FEEDER_T*            pt_src_feeder;
    feeder_input1bytes_fct     pf_input1;
    feeder_setpos_fct          pf_set_pos;
    feeder_getpos_fct          pf_get_pos;
    UINT64                     ui8_feeder_size,ui8_mem_size;
    UINT64                     ui8_skip_size = 0;
    UINT64                     ui8_magic_num = 0;
    UINT64                     ui8_div_size;
    UINT8                      ui1_i, ui1_tag;
    
    if ((pv_mem_data == NULL) || (pv_get_info == NULL))
    {
        return SVCTXR_INV_ARG;
    }
    
    do
    {
        if(0 == z_mem_leng)
        {        
            pt_src_feeder = (MEDIA_FEEDER_T*)pv_mem_data;
            pf_input1     = pt_src_feeder->t_feeder_be_fct_tbl.pf_input1;
            pf_set_pos    = pt_src_feeder->t_feeder_be_fct_tbl.pf_set_pos;
            pf_get_pos    = pt_src_feeder->t_feeder_be_fct_tbl.pf_get_pos;

            if((pf_input1 == NULL) || (pf_set_pos == NULL) || (pf_get_pos == NULL))
            {
                return MFMTRECGR_INV_ARG;
            }
            pf_set_pos(&pt_src_feeder->h_feeder, 0, MEDIA_SEEK_END);
            pf_get_pos(&pt_src_feeder->h_feeder, &ui8_feeder_size);
            if(ui8_feeder_size < MAX_MUTI_CLIP_LOOPS_CNT)
            {
                /*think*/
                ui8_magic_num = 0;
                break;   
            }
            ui8_feeder_size -= MAX_MUTI_CLIP_LOOPS_CNT;
            ui8_div_size = _mm_div64(ui8_feeder_size, 64, NULL);
            pf_set_pos(&pt_src_feeder->h_feeder, 0, MEDIA_SEEK_BGN);
            for(ui1_i=1; ui1_i<(MAX_MUTI_CLIP_LOOPS_CNT + 1); ui1_i++)
            {
                pf_set_pos(&pt_src_feeder->h_feeder, ui8_div_size, MEDIA_SEEK_CUR);
                pf_input1(&pt_src_feeder->h_feeder, &ui1_tag);
                ui1_tag = ui1_tag & 0x01;

                if(ui1_i == 1)
                {
                    ui8_magic_num = (UINT64)ui1_tag;
                }
                else
                {
                    ui8_magic_num |=((UINT64)ui1_tag)<<(ui1_i-1);
                }
            }
            /* reset to origal position*/
            pf_set_pos(&pt_src_feeder->h_feeder, 0, MEDIA_SEEK_BGN);
        }
        else
        {
            ui8_mem_size = (UINT64)z_mem_leng;
            if(ui8_mem_size < MAX_MUTI_CLIP_LOOPS_CNT)
            {
                /*think*/
                ui8_magic_num = 0;
                break;
            }
            ui8_mem_size -= MAX_MUTI_CLIP_LOOPS_CNT;
            ui8_div_size = _mm_div64(ui8_mem_size, 64, NULL);
            for(ui1_i=1; ui1_i<(MAX_MUTI_CLIP_LOOPS_CNT + 1); ui1_i++)
            {
                ui8_skip_size += ui8_div_size;
                ui1_tag = *((UINT8*)pv_mem_data + ui8_skip_size++);     
                ui1_tag = ui1_tag & 0x01;

                if(ui1_i == 1)
                {
                    ui8_magic_num = (UINT64)ui1_tag;
                }
                else
                {
                    ui8_magic_num |=((UINT64)ui1_tag)<<(ui1_i-1);
                }
            }
        }
    }while(0);
    
    x_memcpy(pv_get_info, (const VOID*)&ui8_magic_num, z_info_size);
    return MFMTRECGR_OK;
}
static INT32 _svctx_cvt_identifier(UINT64*         pui8_id,
                                     ID_CVT_CNDTN_T* pv_cndtn)
{
    UINT64  ui8_mask = 0;
    UINT8   ui1_i    = 0;
    
    if (pui8_id == NULL || pv_cndtn == NULL)
    {
        return SVCTXR_INV_ARG;
    } 

    while(pv_cndtn[ui1_i].e_cndtn_type != ID_CNDTN_END)
    {
        switch (pv_cndtn[ui1_i].e_cndtn_type)
        {
            case ID_CNDTN_SRC:
                switch(pv_cndtn[ui1_i].u.t_src_val)
                {
                    /* High bit set 0 */
                    case SRC_TYPE_MEDIA_NETWORK:
                        ui8_mask = 0x7FFFFFFFFFFFFFFF;
                        *pui8_id &= ui8_mask;
                        break;
                    /* High bit set 1 */    
                    case SRC_TYPE_MEDIA_STORGE:
                    default:
                        ui8_mask = 0x8000000000000000;
                        *pui8_id |= ui8_mask;
                        break;
                }
                break;
            default:
                break;
        }
        ui1_i++;
    }
    return SVCTXR_OK;
}
#endif

#endif /*ENABLE_MULTIMEDIA*/


