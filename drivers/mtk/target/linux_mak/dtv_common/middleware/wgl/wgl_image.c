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
 * $RCSfile: wgl_image.c,v $
 * $Revision: #2 $
 * $Date: 2012/12/02 $
 * $Author: xiuqin.wang $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/5 $
 * $SWAuthor: Al Hsieh $
 * $MD5HEX: 5c7f9b779bd3e3428d24b3fb6f494fb5 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "handle/handle.h"
#include "dbg/u_dbg.h"
#include "aee/aee.h"
#include "wgl/wgl_sets.h"
#include "wgl/wgl_image.h"
#include "wgl/x_wgl_rect.h"
#include "wgl/wgl_tools.h"
#include "wgl/wgl.h"
#include "graphic/x_gl.h"
#include "graphic/c_gl.h"
#include "wgl/wgl_sets_cli.h"
#include "cl/x_cl.h"
#include "cl/zip_eng/x_cl_zip_eng.h"
#include "wgl/_wgl_image.h"
#include "util/u_lnk_list.h"
#include "os/thread/thread_lib.h"

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#undef DBG_LEVEL_MODULE
#ifdef CLI_LVL_ALL
#define DBG_LEVEL_MODULE wgl_sets_cli_dbg_level_get()
#else
#define DBG_LEVEL_MODULE DBG_INIT_LEVEL_MW_WGL_MNGR
#endif


static HANDLE_T _gh_image_decode_lock = NULL_HANDLE;
static VOID*    _gpv_image_status = NULL;
/* if the deocde procedure is error, this flag would be set FALSE to tell the status to jpg image decode callback*/
static BOOL     _gfg_image_decode_statuc = FALSE;

#ifndef JPG_EXTENSION_NAME
#define JPG_EXTENSION_NAME "JPG"
#endif

#ifndef PNG_EXTENSION_NAME
#define PNG_EXTENSION_NAME "PNG"
#endif

#ifndef BMP_EXTENSION_NAME
#define BMP_EXTENSION_NAME "BMP"
#endif

#ifndef GIF_EXTENSION_NAME
#define GIF_EXTENSION_NAME "GIF"
#endif


#define _IS_VALID_IMG(pt_img)                                           \
    ( (0 != (pt_img)->ui2_width)                                        \
        && (0 != (pt_img)->ui2_lines)                                   \
        && (NULL != (pt_img)->pui1_image_data)                          \
        && (0 != (pt_img)->ui4_image_size) )

#define _IS_16B_IMG(pt_img)                                             \
    ( (16 == (pt_img)->ui1_bits_per_pixel)                              \
        && (0 == (pt_img)->ui2_table_columns)                           \
        && (NULL == (pt_img)->pt_table_data) )

#define _IS_32B_IMG(pt_img)                                             \
    ( (32 == (pt_img)->ui1_bits_per_pixel)                              \
        && (0 == (pt_img)->ui2_table_columns)                           \
        && (NULL == (pt_img)->pt_table_data) )

#define _IS_8B_IMG(pt_img)                                              \
    (8 == (pt_img)->ui1_bits_per_pixel)

#define _IS_SUPPORT_IMG(pt_img)                                         \
    (_IS_32B_IMG(pt_img) || _IS_16B_IMG(pt_img) || _IS_8B_IMG(pt_img))

#define _IS_FOUR_BYTES_ALIGNED(ui4_image_type)                          \
    ( (ui4_image_type) & WGL_IMG_TYPE_ALIGN_MASK )

#define _IS_COMPRESSED_IMAGE(ui4_image_type)                            \
    ((ui4_image_type) & WGL_IMG_TYPE_COMPRESSED_MASK)

#define _IS_ZIPPED_IMAGE(ui4_image_type)                                \
    ((ui4_image_type) & WGL_IMG_TYPE_ZIPPED_MASK)

#define _IS_AUTO_STRETCH_IMAGE(ui4_image_type)                                \
    ((ui4_image_type) & WGL_IMG_TYPE_STRETCH_MASK)


#define _IS_JPG_IMAGE(ui4_image_type)                                \
    ((ui4_image_type) & WGL_IMG_TYPE_JPG)

#define _IS_PNG_IMAGE(ui4_image_type)                                \
    ((ui4_image_type) & WGL_IMG_TYPE_PNG)

#define _IS_BMP_IMAGE(ui4_image_type)                                \
    ((ui4_image_type) & WGL_IMG_TYPE_BMP)

#ifndef MW_GL_NO_COMPRESSED_IMG
    #define _IS_IMG_TPL_COMPRESSED(pt_template)                         \
        ((pt_template->pt_image != NULL) &&                              \
        _IS_COMPRESSED_IMAGE(pt_template->pt_image->ui4_image_type)      \
        && pt_template->h_surface == NULL_HANDLE)
#else
    #define _IS_IMG_TPL_COMPRESSED(pt_template)                         \
        (FALSE)
#endif

#ifndef MW_GL_NO_COMPRESSED_IMG
    #define _IS_IMG_TPL_JPG(pt_template)                                \
        (((pt_template)->pt_image != NULL) &&                           \
        _IS_JPG_IMAGE((pt_template)->pt_image->ui4_image_type))

    #define _IS_IMG_TPL_PNG(pt_template)                                \
        (((pt_template)->pt_image != NULL) &&                           \
        _IS_PNG_IMAGE((pt_template)->pt_image->ui4_image_type))

    #define _IS_IMG_TPL_BMP(pt_template)                                \
        (((pt_template)->pt_image != NULL) &&                           \
        _IS_BMP_IMAGE((pt_template)->pt_image->ui4_image_type))
#else
    #define _IS_IMG_TPL_JPG(pt_template)                                \
        (FALSE)

    #define _IS_IMG_TPL_PNG(pt_template)                                \
        (FALSE)

    #define _IS_IMG_TPL_BMP(pt_template)                                \
        (FALSE)
#endif


#define _GET_FOUR_BYTES_ALIGNED_PITCH(ui2_width, ui1_bits_per_pixel)    \
    (((((UINT32)ui2_width) * ((UINT32)ui1_bits_per_pixel) + 31) >> 5 ) << 2)

#define _IS_SYSTEM_PROVIDE(ui2_tpl_flag)                                \
    (WGL_IMG_FLAG_SYS_PROVIDE & (ui2_tpl_flag))

#define _IS_IMG_PREFETCH(ui2_tpl_flag)                                \
    (WGL_IMG_FLAG_PERFETCH & (ui2_tpl_flag))


#define _WGL_INVOKE_PAINT_FUNC(_h_graphic, _pt_cvsst, _h_surf, _i4_dst_x, _i4_dst_y, _i4_src_x, _i4_src_y, _ui4_width, _ui4_height, _ui1_alpha, _e_paint_method)                \
    ((WGL_TPL_PAINT_BITBLT == (_e_paint_method)) ?                                                                                                                              \
          x_gl_bitblt((_h_graphic), (_h_surf), (_i4_dst_x), (_i4_dst_y), (_i4_src_x), (_i4_src_y), (_ui4_width), (_ui4_height), (_ui1_alpha))                                   \
        : GL_IS_PALETTE_MODE((_pt_cvsst)->e_clr_mode) ?                                                                                                                         \
               x_gl_transp_blt((_h_graphic), (_h_surf), (_i4_dst_x), (_i4_dst_y), (_i4_src_x), (_i4_src_y), (_ui4_width), (_ui4_height), (_ui1_alpha), TRUE, &(_pt_cvsst)->t_colorkey, &(_pt_cvsst)->t_colorkey) \
             : x_gl_compose((_h_graphic), (_h_surf), (_i4_dst_x), (_i4_dst_y), (_i4_src_x), (_i4_src_y), (_ui4_width), (_ui4_height), (_ui1_alpha), GL_SRC_OVER) )

#define _WGL_INVOKE_PATTERN_PAINT(_h_graphic, _pt_cvsst, _h_surf, _i4_dst_x, _i4_dst_y, _ui4_dst_w, _ui4_dst_h, _i4_src_x, _i4_src_y, _ui4_src_w, _ui4_src_h, _ui1_alpha, _e_paint_method) \
    ((WGL_TPL_PAINT_BITBLT == (_e_paint_method)) ?                                                                                                                              \
          x_gl_transp_patblt((_h_graphic), (_h_surf), (INT32)(_i4_dst_x), (INT32)(_i4_dst_y), (UINT32)(_ui4_dst_w), (UINT32)(_ui4_dst_h), (INT32)(_i4_src_x), (INT32)(_i4_src_y), (UINT32)(_ui4_src_w), (UINT32)(_ui4_src_h), (_ui1_alpha), TRUE, &(_pt_cvsst)->t_colorkey, &(_pt_cvsst)->t_colorkey) \
        : GL_IS_PALETTE_MODE((_pt_cvsst)->e_clr_mode) ?                                                                                                                        \
              x_gl_transp_patblt((_h_graphic), (_h_surf), (INT32)(_i4_dst_x), (INT32)(_i4_dst_y), (UINT32)(_ui4_dst_w), (UINT32)(_ui4_dst_h), (INT32)(_i4_src_x), (INT32)(_i4_src_y), (UINT32)(_ui4_src_w), (UINT32)(_ui4_src_h), (_ui1_alpha), TRUE, &(_pt_cvsst)->t_colorkey, &(_pt_cvsst)->t_colorkey) \
            : x_gl_pat_compose((_h_graphic), (_h_surf), (INT32)(_i4_dst_x), (INT32)(_i4_dst_y), (UINT32)(_ui4_dst_w), (UINT32)(_ui4_dst_h), (INT32)(_i4_src_x), (INT32)(_i4_src_y), (UINT32)(_ui4_src_w), (UINT32)(_ui4_src_h), (_ui1_alpha), GL_SRC_OVER) )

#define WGL_IMG_TYPE_MASK       ((UINT32) 0xFF)

typedef struct _WGL_IMAGE_CATCH_T
{
    DLIST_ENTRY_T(_WGL_IMAGE_CATCH_T) t_img_cache_entry;
    VOID*                          pv_data;
    HANDLE_T                       h_surf;
    UINT32                         ui4_reference_cnt;
}WGL_IMAGE_CATCH_T;

#ifdef CLI_LVL_ALL
typedef struct _IMG_APP_INFO_T
{
    HANDLE_T                h_thread;
    UINT32                  ui4_img_count;
    DLIST_T(_WGL_IMG_TPL_T) t_img_tpl_list;
    DLIST_ENTRY_T(_IMG_APP_INFO_T) t_link;
}IMG_APP_INFO_T;

DLIST_T(_IMG_APP_INFO_T) t_img_app_list;
#endif

DLIST_T(_WGL_IMAGE_CATCH_T) t_img_cache_list;

static HANDLE_T h_image_cache_lock = NULL_HANDLE;

static HANDLE_T h_wgl_image_module_lock = NULL_HANDLE;
static BOOL     gb_wgl_image_inited = FALSE;

#define DBG_ABRT_CANNOT_LOCK_WGL_IMG                ((UINT32) 0x00000003)
#define DBG_ABRT_CANNOT_UNLOCK_WGL_IMG              ((UINT32) 0x00000004)

#define ABORT_IMAGE(_cat, _val) DBG_ABORT(DBG_MOD_WGL| (_cat) | (_val))

static WGL_IMG_DECODE_FUNCTION_TABLE_T decode_funtion_table;

/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/


 /*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/


static VOID _wgl_img_module_lock(VOID);

static VOID _wgl_img_module_unlock(VOID);

static BOOL _wgl_img_tpl_free_cb(
    HANDLE_T                h_handle,
    HANDLE_TYPE_T           e_type,
    VOID*                   pv_obj,
    VOID*                   pv_tag,
    BOOL                    b_req_handle);

/*No illegal parameter checking*/
static INT32 _wgl_img_tpl_HO_paint(
    WGL_IMG_TPL_T*          pt_template,
    const GL_RECT_T*        pt_output_rect,
    const GL_RECT_T*        pt_invalid_rect,
    GL_HGRAPHICS_T          h_graphics,
    const WGL_CANVAS_STATUS_T* pt_cvsst,
    WGL_TPL_PAINT_METHOD_T  e_paint_method,
    WGL_TPL_PAINT_ALGSM_T   e_algorism);

/*No illegal parameter checking*/
static INT32 _wgl_img_tpl_VO_paint(
    WGL_IMG_TPL_T*          pt_template,
    const GL_RECT_T*        pt_output_rect,
    const GL_RECT_T*        pt_invalid_rect,
    GL_HGRAPHICS_T          h_graphics,
    const WGL_CANVAS_STATUS_T* pt_cvsst,
    WGL_TPL_PAINT_METHOD_T  e_paint_method,
    WGL_TPL_PAINT_ALGSM_T   e_algorism);

/*No illegal parameter checking*/
static INT32 _wgl_img_tpl_HV_paint(
    WGL_IMG_TPL_T*          pt_template,
    const GL_RECT_T*        pt_output_rect,
    const GL_RECT_T*        pt_invalid_rect,
    GL_HGRAPHICS_T          h_graphics,
    const WGL_CANVAS_STATUS_T* pt_cvsst,
    WGL_TPL_PAINT_METHOD_T  e_paint_method,
    WGL_TPL_PAINT_ALGSM_T   e_algorism);

/*No illegal parameter checking*/
static INT32 _wgl_img_tpl_NHV_paint(
    WGL_IMG_TPL_T*          pt_template,
    const GL_RECT_T*        pt_output_rect,
    const GL_RECT_T*        pt_invalid_rect,
    GL_HGRAPHICS_T          h_graphics,
    const WGL_CANVAS_STATUS_T* pt_cvsst,
    WGL_TPL_PAINT_METHOD_T  e_paint_method);

/*get surface color mode*/
static INT32 _wgl_img_get_color_mode(
    UINT32                  ui4_image_type,
    GL_COLORMODE_T*         pe_color_mode);

/* software-version NHV pattern paint */
static INT32 wgl_sw_nhv_pattern_paint(
    GL_HGRAPHICS_T              h_graphics,
    const WGL_CANVAS_STATUS_T   *pt_cvsst,
    WGL_IMG_TPL_T               *pt_template,
    GL_RECT_T                   *pt_work_rect);

static INT32 _wgl_img_tpl_strech_paint(
    WGL_IMG_TPL_T*             pt_template,
    const GL_RECT_T*           pt_output_rect,
    const GL_RECT_T*           pt_invalid_rect,
    GL_HGRAPHICS_T             h_graphics,
    const WGL_CANVAS_STATUS_T  *pt_cvsst,
    WGL_TPL_PAINT_METHOD_T     e_paint_method);

static BOOL _wgl_image_cache_find(
    VOID*                       pt_image,
    HANDLE_T*                   ph_surface,
    BOOL                        fg_add_reference);


static INT32 _wgl_image_cache_add(
    VOID*                       pt_image,
    HANDLE_T                    h_surface);


static INT32 _wgl_image_cache_delete(
    VOID*                       pt_image);

static VOID _wgl_img_tpl_jpg_decode_cb(
     HANDLE_T                h_handle,
    const VOID*             pv_tag,
    IMG_NFY_EVENT_TYPE_T    e_event,
    const VOID*             pv_data);

static INT32 _wgl_surface_invert_image(HANDLE_T h_surf,
      UINT32                            ui4_width,
      INT32                             i4_src_x,
      INT32                             i4_src_y,
      UINT32                            ui4_src_height,
      INT32                             i4_dst_x,
      INT32                             i4_dst_y,
      UINT32                            ui4_dst_height);

static INT32 _wgl_img_decode_lock(VOID);

static INT32 _wgl_img_decode_unlock(VOID);

static BOOL _wgl_img_check_status_data(const VOID** ppv_tag);

static VOID _wgl_img_free_status_data(VOID** ppv_tag);

static VOID _wgl_img_alloc_status_data(VOID** ppv_tag, UINT16 ui2_nbs);

static INT32 wgl_img_tpl_create_surface(
    WGL_IMG_TPL_T*             pt_template);

static INT32 wgl_img_tpl_destroy_surface(
    WGL_IMG_TPL_T*             pt_template);

#ifdef CLI_LVL_ALL
static INT32 _wgl_img_add_to_list(
    WGL_IMG_TPL_T*             pt_template);

static VOID _wgl_img_remove_from_list(
    WGL_IMG_TPL_T*             pt_template);

#endif

/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/
/*******************************************************************************
 * Name
 *          wgl_img_tpl_create
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/


INT32 wgl_image_init(VOID)
{
    INT32 i4_ret = WGLR_OK;

	if (! gb_wgl_image_inited)
	{
	    DLIST_INIT(&t_img_cache_list);
#ifdef CLI_LVL_ALL    
	    DLIST_INIT(&t_img_app_list);
#endif
	    i4_ret = x_sema_create(&h_image_cache_lock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK);
	    if (i4_ret != OSR_OK)
	    {
	        return WGLR_INTERNAL_ERROR;
	    }

	    decode_funtion_table.pf_async_load = NULL;
	    decode_funtion_table.pf_sync_load = NULL;
	    decode_funtion_table.pf_close = NULL;
	    decode_funtion_table.pf_fopen = NULL;
	    decode_funtion_table.pf_mopen = NULL;
	    decode_funtion_table.pf_get_dimension = NULL;
		decode_funtion_table.pf_set_sw_decode_force = NULL;

		i4_ret = x_sema_create(&h_wgl_image_module_lock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK);
	    if (i4_ret != OSR_OK)
	    {
	        return WGLR_INTERNAL_ERROR;
	    }
		
		gb_wgl_image_inited = TRUE;
	}
	
	return i4_ret;
}

BOOL _wgl_image_cache_find(VOID* pt_image, HANDLE_T* ph_surface, BOOL fg_add_reference)
{
    WGL_IMAGE_CATCH_T      *pt_cache = NULL;

    if (pt_image == NULL || ph_surface == NULL)
    {
        return FALSE;
    }

    x_sema_lock(h_image_cache_lock, X_SEMA_OPTION_WAIT);

    pt_cache = DLIST_HEAD(&(t_img_cache_list));

    while(pt_cache)
    {
        if(pt_cache->pv_data == pt_image)
        {
            DLIST_REMOVE(pt_cache, &(t_img_cache_list), t_img_cache_entry);
            DLIST_INSERT_HEAD(pt_cache,&(t_img_cache_list),t_img_cache_entry);

            if (fg_add_reference)
            {
                pt_cache->ui4_reference_cnt ++;
            }

            *ph_surface = pt_cache->h_surf;
            x_sema_unlock(h_image_cache_lock);
            return TRUE;
        }
        pt_cache = DLIST_NEXT(pt_cache,t_img_cache_entry);
    }

    x_sema_unlock(h_image_cache_lock);

    *ph_surface  = NULL_HANDLE;

    return FALSE;
}

INT32 _wgl_image_cache_add(VOID* pt_image, HANDLE_T h_surface)
{
    WGL_IMAGE_CATCH_T      *pt_cache = NULL;
    if (pt_image == NULL || h_surface == NULL_HANDLE)
    {
        return WGLR_OK;
    }
    x_sema_lock(h_image_cache_lock, X_SEMA_OPTION_WAIT);
    #if 0
    pt_cache = DLIST_HEAD(&(t_img_cache_list));

    while(pt_cache)
    {
        if(pt_cache->pv_data == pt_image)
        {
            DLIST_REMOVE(pt_cache, &(t_img_cache_list), t_img_cache_entry);
            DLIST_INSERT_HEAD(pt_cache,&(t_img_cache_list),t_img_cache_entry);

            pt_cache->ui4_reference_cnt ++;

            x_sema_unlock(h_image_cache_lock);

            return WGLR_OK;
        }
        pt_cache = DLIST_NEXT(pt_cache,t_img_cache_entry);
    }
    #endif

    pt_cache = (WGL_IMAGE_CATCH_T*)WGL_MEM_ALLOC(sizeof(WGL_IMAGE_CATCH_T));
    if (pt_cache == NULL)
    {
        x_sema_unlock(h_image_cache_lock);
        return WGLR_OK;
    }
    pt_cache->pv_data = pt_image;
    pt_cache->h_surf = h_surface;
    pt_cache->ui4_reference_cnt = 1;

    DLIST_INSERT_HEAD(pt_cache,&(t_img_cache_list),t_img_cache_entry);

    x_sema_unlock(h_image_cache_lock);

    return WGLR_OK;
}


INT32 _wgl_image_cache_delete(VOID* pt_image)
{
    WGL_IMAGE_CATCH_T      *pt_cache = NULL;
    INT32                   i4_ret;

    if (pt_image == NULL)
    {
        return WGLR_OK;
    }

    x_sema_lock(h_image_cache_lock, X_SEMA_OPTION_WAIT);

    pt_cache = DLIST_HEAD(&(t_img_cache_list));

    while(pt_cache)
    {
        if(pt_cache->pv_data == pt_image)
        {
            pt_cache->ui4_reference_cnt --;

            if (pt_cache->ui4_reference_cnt == 0)
            {
                DLIST_REMOVE(pt_cache, &(t_img_cache_list), t_img_cache_entry);

                i4_ret = x_gl_obj_free(pt_cache->h_surf);
                if (i4_ret != GLR_OK)
                {
                    x_sema_unlock(h_image_cache_lock);
                    return WGLR_INTERNAL_ERROR;
                }

                WGL_MEM_FREE(pt_cache);

                x_sema_unlock(h_image_cache_lock);

                return WGLR_OK;
            }
            x_sema_unlock(h_image_cache_lock);
            return WGLR_OK;
        }
        pt_cache = DLIST_NEXT(pt_cache,t_img_cache_entry);
    }
    x_sema_unlock(h_image_cache_lock);
    return WGLR_OK;
}



INT32 wgl_img_tpl_create(
    WGL_IMG_ST_T                e_segment_type,
    UINT16                      ui2_flag,
    const WGL_IMG_T*            pt_image,
    WGL_HIMG_TPL_T*             ph_img_tpl)
{
    /*variable declarations*/
    HANDLE_T                    h_template_handle = NULL_HANDLE;
    WGL_IMG_TPL_T*              pt_template = NULL;
    GL_SURFACE_DESC_T           t_surf_dest = {0};
    GL_LOCK_INFO_T              t_surf_lock_info;
    UINT32                      ui4_cur_line = 0;
    UINT8*                      pui1_line_start = NULL;
    UINT8*                      pui1_img_line_start = NULL;
    UINT32                      ui4_img_line_bytes = 0;
    INT32                       i4_diff = 0;
    GL_COLORMODE_T              e_color_mode;
    INT32                       i4_ret;
#ifndef MW_GL_NO_COMPRESSED_IMG
    HANDLE_T                    h_zip_engine = NULL_HANDLE, h_zip_state = NULL_HANDLE;
    UINT8                       *pui1_line_buffer = NULL;
    SIZE_T                      z_img_pitch, z_compressed_length = 0;
    UINT8                       *pui1_compressed_img_start = NULL;
#endif
    /*check parameter*/
    if(NULL == pt_image || NULL == ph_img_tpl)
    {
        return WGLR_INV_ARG;
    }

    /*initialize return value*/
    *ph_img_tpl = NULL_HANDLE;

    /*check image*/
    if(!_IS_VALID_IMG(pt_image))
    {
        return WGLR_INV_ARG;
    }
    if(!_IS_SUPPORT_IMG(pt_image))
    {
        return WGLR_NOT_SUPPORT_FORMAT;
    }

    /* overwrite the segment type if it has been specified in the BitmapConverter */
    if (pt_image->ui4_image_type & WGL_IMG_SEGMENT_MASK)
    {
        e_segment_type = (WGL_IMG_ST_T)(((pt_image->ui4_image_type & WGL_IMG_SEGMENT_MASK) >> WGL_IMG_SEGMENT_OFFSET) & 0xFF);
    }
    else if ((_IS_COMPRESSED_IMAGE(pt_image->ui4_image_type) && e_segment_type != WGL_IMG_ST_NO_SEGMENT) ||
             (_IS_ZIPPED_IMAGE(pt_image->ui4_image_type)     && e_segment_type == WGL_IMG_ST_NO_SEGMENT))
    {
        x_dbg_stmt("{WGL} the segment type of this image template is different from what specified in the .i file.\n");

        return WGLR_INV_ARG;
    }

    /*get color mode*/
    i4_ret = _wgl_img_get_color_mode(pt_image->ui4_image_type, &e_color_mode);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }

    /*allocate memory*/
    pt_template = (WGL_IMG_TPL_T*) WGL_MEM_ALLOC(sizeof(WGL_IMG_TPL_T));
    if(NULL == pt_template)
    {
        return WGLR_OUT_OF_MEMORY;
    }

    /*initialize template image*/
    x_memset(pt_template, 0, sizeof(WGL_IMG_TPL_T));
    pt_template->h_surface = NULL_HANDLE;
#ifndef MW_GL_NO_COMPRESSED_IMG
    pt_template->pt_image = (WGL_IMG_T*)pt_image; /* (_IS_COMPRESSED_IMAGE(pt_image->ui4_image_type))? (WGL_IMG_T*)pt_image: NULL;*/

    /* initialize the zip engine */
    if (_IS_ZIPPED_IMAGE(pt_image->ui4_image_type) || _IS_AUTO_STRETCH_IMAGE(pt_image->ui4_image_type))
    {
        if (x_cl_open_eng(SN_CL_ZIP, NULL, NULL, &h_zip_engine) != CLR_OK)
        {
            DBG_ERROR(("{WGL} unzip engine initialization failed. Line: %d\n", __LINE__));
			x_mem_free(pt_template);
            return WGLR_INTERNAL_ERROR;
        }
    }
    if (_IS_JPG_IMAGE(pt_image->ui4_image_type) && decode_funtion_table.pf_mopen != NULL)
    {
        pt_template->pt_image = (WGL_IMG_T*)pt_image;

        i4_ret = decode_funtion_table.pf_mopen(
                 pt_image->pui1_image_data,
                 pt_image->ui4_image_size,
                 JPG_EXTENSION_NAME,
                 _wgl_img_tpl_jpg_decode_cb,
                 &_gpv_image_status,
                 &pt_template->h_img);
        if (IMGR_OK != i4_ret)
        {
            DBG_ERROR(("[WGL IMG]Open Jpg image failed. %d \n", i4_ret));
			x_mem_free(pt_template);
            return WGLR_INTERNAL_ERROR;
        }
        if (decode_funtion_table.pf_get_dimension != NULL &&
            ( pt_image->ui2_lines == DEFAULT_IMAGE_WIDTH ||pt_image->ui2_width == DEFAULT_IMAGE_HEIGHT))
        {
            i4_ret = decode_funtion_table.pf_get_dimension(pt_template->h_img,
                                                           INTRA_IMG_PRIMARY_IMG_TYPE,
                                                           (SIZE_T*)(&(pt_template->t_surface_info.ui4_width)),
                                                           (SIZE_T*)(&(pt_template->t_surface_info.ui4_height))
                                                           );
            if (i4_ret != IMGR_OK)
            {
                decode_funtion_table.pf_close(pt_template->h_img);
				x_mem_free(pt_template);
                return WGLR_INTERNAL_ERROR;
            }
          /*
            pt_image->ui2_lines = (UINT16)pt_template->t_surface_info.ui4_height;
            pt_image->ui2_width = (UINT16)pt_template->t_surface_info.ui4_width;
            */
        }
        else
        {
            pt_template->t_surface_info.ui4_height= (UINT32)pt_image->ui2_lines;
            pt_template->t_surface_info.ui4_width = (UINT32)pt_image->ui2_width;
        }
        pt_template->t_surface_info.e_clrmode = e_color_mode;
        pt_template->t_surface_info.h_signal  = NULL_HANDLE;

    }
    else if (_IS_PNG_IMAGE(pt_image->ui4_image_type) && decode_funtion_table.pf_mopen != NULL)
    {
        pt_template->pt_image = (WGL_IMG_T*)pt_image;

        i4_ret = decode_funtion_table.pf_mopen(
                 pt_image->pui1_image_data,
                 pt_image->ui4_image_size,
                 PNG_EXTENSION_NAME,
                 _wgl_img_tpl_jpg_decode_cb,
                 &_gpv_image_status,
                 &pt_template->h_img);
        if (IMGR_OK != i4_ret)
        {
            DBG_ERROR(("[WGL IMG]Open png image failed. %d \n", i4_ret));
			x_mem_free(pt_template);
            return WGLR_INTERNAL_ERROR;
        }

        if (decode_funtion_table.pf_get_dimension != NULL &&
            ( pt_image->ui2_lines == DEFAULT_IMAGE_WIDTH ||pt_image->ui2_width == DEFAULT_IMAGE_HEIGHT))
        {
            i4_ret = decode_funtion_table.pf_get_dimension(pt_template->h_img,
                                                           INTRA_IMG_PRIMARY_IMG_TYPE,
                                                           (SIZE_T*)(&(pt_template->t_surface_info.ui4_width)),
                                                           (SIZE_T*)(&(pt_template->t_surface_info.ui4_height))
                                                           );
            if (i4_ret != IMGR_OK)
            {
                decode_funtion_table.pf_close(pt_template->h_img);
				x_mem_free(pt_template);
                return WGLR_INTERNAL_ERROR;
            }

            /*
            pt_image->ui2_lines = (UINT16)pt_template->t_surface_info.ui4_height;
            pt_image->ui2_width = (UINT16)pt_template->t_surface_info.ui4_width;
            */
        }
        else
        {
            pt_template->t_surface_info.ui4_height= (UINT32)pt_image->ui2_lines;
            pt_template->t_surface_info.ui4_width = (UINT32)pt_image->ui2_width;
        }
        pt_template->t_surface_info.e_clrmode = e_color_mode;
        pt_template->t_surface_info.h_signal  = NULL_HANDLE;

    }
    else if (_IS_BMP_IMAGE(pt_image->ui4_image_type) && decode_funtion_table.pf_mopen != NULL)
    {
        pt_template->pt_image = (WGL_IMG_T*)pt_image;

        i4_ret = decode_funtion_table.pf_mopen(
                 pt_image->pui1_image_data,
                 pt_image->ui4_image_size,
                 BMP_EXTENSION_NAME,
                 _wgl_img_tpl_jpg_decode_cb,
                 &_gpv_image_status,
                 &pt_template->h_img);
        if (IMGR_OK != i4_ret)
        {
            DBG_ERROR(("[WGL IMG]Open bmp image failed. %d \n", i4_ret));
			x_mem_free(pt_template);
            return WGLR_INTERNAL_ERROR;
        }
        if (decode_funtion_table.pf_get_dimension != NULL &&
            ( pt_image->ui2_lines == DEFAULT_IMAGE_WIDTH ||pt_image->ui2_width == DEFAULT_IMAGE_HEIGHT))
        {
            i4_ret = decode_funtion_table.pf_get_dimension(pt_template->h_img,
                                                           INTRA_IMG_PRIMARY_IMG_TYPE,
                                                           (SIZE_T*)(&(pt_template->t_surface_info.ui4_width)),
                                                           (SIZE_T*)(&(pt_template->t_surface_info.ui4_height))
                                                           );
             if (i4_ret != IMGR_OK)
            {
                decode_funtion_table.pf_close(pt_template->h_img);
				x_mem_free(pt_template);
                return WGLR_INTERNAL_ERROR;
            }

            /*
            pt_image->ui2_lines = (UINT16)pt_template->t_surface_info.ui4_height;
            pt_image->ui2_width = (UINT16)pt_template->t_surface_info.ui4_width;
            */
        }
        else
        {
            pt_template->t_surface_info.ui4_height= (UINT32)pt_image->ui2_lines;
            pt_template->t_surface_info.ui4_width = (UINT32)pt_image->ui2_width;
        }
        pt_template->t_surface_info.e_clrmode = e_color_mode;
        pt_template->t_surface_info.h_signal  = NULL_HANDLE;
    }
#endif

    /*allocate handle*/
    if (HR_OK != handle_alloc(HT_WGL_IMG_TPL, pt_template, NULL, _wgl_img_tpl_free_cb, &h_template_handle))
    {
        i4_ret = WGLR_OUT_OF_HANDLES;
        goto _err_out;
    }

    /* if the image is in the compressed format, then a surface need not to be created. */
    if ((_IS_COMPRESSED_IMAGE(pt_image->ui4_image_type) && !_IS_AUTO_STRETCH_IMAGE(pt_image->ui4_image_type))|| _IS_JPG_IMAGE(pt_image->ui4_image_type) ||
        _IS_PNG_IMAGE(pt_image->ui4_image_type) || _IS_BMP_IMAGE(pt_image->ui4_image_type))
    {
        pt_template->h_surface = NULL_HANDLE;
    }
    else if (! _wgl_image_cache_find((VOID*)pt_image, &(pt_template->h_surface), TRUE))
    {
        /*prepare information to create surface*/
        t_surf_dest.ui4_width = pt_image->ui2_width;
        t_surf_dest.ui4_height = pt_image->ui2_lines;
        t_surf_dest.e_clrmode = e_color_mode;
        t_surf_dest.h_palette = NULL_HANDLE;
        t_surf_dest.h_clipper = NULL_HANDLE;


        /*create surface*/
        if(GLR_OK != x_gl_surface_create(&t_surf_dest, &pt_template->h_surface))
        {
            i4_ret = WGLR_SYS_RC_CREATE_FAIL;
            goto _err_out;
        }

        /*cache sufrface info*/
        if(GLR_OK != x_gl_surface_get_info(pt_template->h_surface, &pt_template->t_surface_info))
        {
            i4_ret = WGLR_INTERNAL_ERROR;
            goto _err_out;
        }

        /*Lock surface*/
        if(GLR_OK != x_gl_surface_lock(pt_template->h_surface, NULL, GL_SYNC, &t_surf_lock_info))
        {
            i4_ret = WGLR_LOCK_ERROR;
            goto _err_out;
        }


        /*Copy Image Data*/
        /*surface start pos*/
        pui1_line_start = t_surf_lock_info.at_subplane[0].pv_bits;

        /*image start pos*/
        pui1_img_line_start = pt_image->pui1_image_data;

        /*bytes_per_line of image*/
        ui4_img_line_bytes = (_IS_FOUR_BYTES_ALIGNED(pt_image->ui4_image_type)) ?
                             _GET_FOUR_BYTES_ALIGNED_PITCH(pt_image->ui2_width, pt_image->ui1_bits_per_pixel) :
                             (((UINT32)pt_image->ui2_width) * ((UINT32)pt_image->ui1_bits_per_pixel) >> 3);

    #ifndef MW_GL_NO_COMPRESSED_IMG
        if (_IS_ZIPPED_IMAGE(pt_image->ui4_image_type) || _IS_AUTO_STRETCH_IMAGE(pt_image->ui4_image_type))
        {
            /* allocate memory for the line buffer */
            pui1_line_buffer    = (UINT8 *)WGL_MEM_ALLOC(ui4_img_line_bytes * sizeof(UINT8));
            pui1_img_line_start = pui1_line_buffer;

            if (pui1_line_buffer == NULL)
            {
                i4_ret = WGLR_OUT_OF_MEMORY;
                goto _err_out;
            }

            /* get the info of the compressed image */
            z_img_pitch               = (SIZE_T)GET_UINT32_FROM_PTR_LITTLE_END(pt_image->pui1_image_data);
            z_compressed_length       = (SIZE_T)GET_UINT32_FROM_PTR_LITTLE_END(pt_image->pui1_image_data + sizeof(SIZE_T));
            pui1_compressed_img_start = pt_image->pui1_image_data + sizeof(INT32) * 2;
        }
    #endif

        /*difference between image and surface for each line*/
        i4_diff = ((INT32)t_surf_lock_info.at_subplane[0].ui4_pitch) - ((INT32)ui4_img_line_bytes);
        /*WGL_ASSERT(i4_diff >= 0);*/

        /*copy operation*/
        for(ui4_cur_line = 0; ui4_cur_line < pt_image->ui2_lines; ui4_cur_line++)
        {
        #ifndef MW_GL_NO_COMPRESSED_IMG
            if (_IS_ZIPPED_IMAGE(pt_image->ui4_image_type) || _IS_AUTO_STRETCH_IMAGE(pt_image->ui4_image_type))
            {
                z_img_pitch = (SIZE_T)GET_UINT32_FROM_PTR_LITTLE_END(pt_image->pui1_image_data);
                /* decompress one line of the zipped image */
                x_cl_decompress(
                    h_zip_engine,
                    pui1_compressed_img_start,
                    z_compressed_length,
                    0,
                    CL_NULL_PARAM_TYPE,
                    NULL,
                    pui1_img_line_start,
                    &z_img_pitch,
                    &h_zip_state);
            }
        #endif

            if (!_IS_ZIPPED_IMAGE(pt_image->ui4_image_type) && !_IS_AUTO_STRETCH_IMAGE(pt_image->ui4_image_type))
            {
                WGL_ASSERT(pui1_img_line_start < pt_image->pui1_image_data + pt_image->ui4_image_size);
            }

            if(i4_diff > 0)
            {
                x_memcpy(pui1_line_start, pui1_img_line_start, ui4_img_line_bytes);
                x_memset(pui1_line_start + ui4_img_line_bytes, 0, (SIZE_T)i4_diff);
            }
            else
            {
                x_memcpy(pui1_line_start, pui1_img_line_start, t_surf_lock_info.at_subplane[0].ui4_pitch);
            }

            if (!_IS_ZIPPED_IMAGE(pt_image->ui4_image_type) && !_IS_AUTO_STRETCH_IMAGE(pt_image->ui4_image_type))
            {
                pui1_img_line_start += ui4_img_line_bytes;
            }
            pui1_line_start += t_surf_lock_info.at_subplane[0].ui4_pitch;
        }

        /*Unlock surface*/
        if(GLR_OK != x_gl_surface_unlock(pt_template->h_surface, NULL))
        {
            i4_ret = WGLR_UNLOCK_ERROR;
            goto _err_out;
        }

    #ifndef MW_GL_NO_COMPRESSED_IMG
        if (_IS_ZIPPED_IMAGE(pt_image->ui4_image_type) || _IS_AUTO_STRETCH_IMAGE(pt_image->ui4_image_type))
        {
            x_cl_close(h_zip_engine);
            x_cl_close(h_zip_state);
            WGL_MEM_FREE(pui1_img_line_start);
        }
    #endif

        _wgl_image_cache_add((VOID*)pt_image, pt_template->h_surface);

    }
    else
    {
        /*cache surface info*/
        if(GLR_OK != x_gl_surface_get_info(pt_template->h_surface, &pt_template->t_surface_info))
        {
            i4_ret = WGLR_INTERNAL_ERROR;
            goto _err_out;
        }

        if (_IS_ZIPPED_IMAGE(pt_image->ui4_image_type) || _IS_AUTO_STRETCH_IMAGE(pt_image->ui4_image_type))
        {
            if (h_zip_engine != NULL_HANDLE)
            {
                x_cl_close(h_zip_engine);
            }
            if (h_zip_state != NULL_HANDLE)
            {
                x_cl_close(h_zip_state);
            }
        }
    }

    /*Segment Type*/
    pt_template->e_segment_type = e_segment_type;
    /*Extra Flag*/
    pt_template->ui2_flag = ui2_flag;
#ifdef CLI_LVL_ALL
    if(_wgl_img_add_to_list(pt_template)!= WGLR_OK)
    {
         i4_ret = WGLR_INTERNAL_ERROR;
          goto _err_out;
    }
    pt_template->h_this = h_template_handle;
#endif
    /*prepare return value*/
    *ph_img_tpl = h_template_handle;
    return WGLR_OK;

_err_out:
    if(NULL_HANDLE != h_template_handle)
    {
        handle_free(h_template_handle, FALSE);
    }
    else
    {
        WGL_MEM_FREE(pt_template);
    }
    return i4_ret;
}
#if 0
INT32 wgl_img_tpl_create_ex(
    WGL_IMG_ST_T                e_segment_type,
    UINT16                      ui2_flag,
    const WGL_IMG_T*            pt_image,
    WGL_HIMG_TPL_T*             ph_img_tpl)
{
    /*variable declarations*/
    HANDLE_T                    h_template_handle = NULL_HANDLE;
    WGL_IMG_TPL_T*              pt_template = NULL;
    GL_SURFACE_DESC_T           t_surf_dest = {0};
    GL_LOCK_INFO_T              t_surf_lock_info;
    UINT32                      ui4_cur_line = 0;
    UINT8*                      pui1_line_start = NULL;
    UINT8*                      pui1_img_line_start = NULL;
    UINT32                      ui4_img_line_bytes = 0;
    INT32                       i4_diff = 0;
    GL_COLORMODE_T              e_color_mode;
    INT32                       i4_ret;
    HANDLE_T                    h_zip_engine, h_zip_state;
    UINT8                       *pui1_line_buffer = NULL;
    SIZE_T                      z_img_pitch, z_compressed_length = 0;
    UINT8                       *pui1_compressed_img_start = NULL;

    /*check parameter*/
    if(NULL == pt_image || NULL == ph_img_tpl)
    {
        return WGLR_INV_ARG;
    }

    /*initialize return value*/
    *ph_img_tpl = NULL_HANDLE;

    /*check image*/
    if(!_IS_VALID_IMG(pt_image))
    {
        return WGLR_INV_ARG;
    }
    if(!_IS_SUPPORT_IMG(pt_image))
    {
        return WGLR_NOT_SUPPORT_FORMAT;
    }
    if (!(_IS_COMPRESSED_IMAGE(pt_image->ui4_image_type) || _IS_ZIPPED_IMAGE(pt_image->ui4_image_type)))
    {
        return WGLR_NOT_SUPPORT_FORMAT;
    }
    /* overwrite the segment type if it has been specified in the BitmapConverter */
    if (pt_image->ui4_image_type & WGL_IMG_SEGMENT_MASK)
    {
        e_segment_type = (WGL_IMG_ST_T)(((pt_image->ui4_image_type & WGL_IMG_SEGMENT_MASK) >> WGL_IMG_SEGMENT_OFFSET) & 0xFF);
    }
    else if ((_IS_COMPRESSED_IMAGE(pt_image->ui4_image_type) && e_segment_type != WGL_IMG_ST_NO_SEGMENT) ||
             (_IS_ZIPPED_IMAGE(pt_image->ui4_image_type)     && e_segment_type == WGL_IMG_ST_NO_SEGMENT))
    {
        x_dbg_stmt("{WGL} the segment type of this image template is different from what specified in the .i file.\n");

        return WGLR_INV_ARG;
    }

    /*get color mode*/

    i4_ret = _wgl_img_get_color_mode(pt_image->ui4_image_type, &e_color_mode);
    if(WGLR_OK != i4_ret)
    {
        return i4_ret;
    }

    /*allocate memory*/
    pt_template = (WGL_IMG_TPL_T*) WGL_MEM_ALLOC(sizeof(WGL_IMG_TPL_T));
    if(NULL == pt_template)
    {
        return WGLR_OUT_OF_MEMORY;
    }

    /*initialize template image*/
    x_memset(pt_template, 0, sizeof(WGL_IMG_TPL_T));
    pt_template->h_surface = NULL_HANDLE;
    pt_template->pt_image =  NULL;

    /* initialize the zip engine */
    if (_IS_ZIPPED_IMAGE(pt_image->ui4_image_type))
    {
        if (x_cl_open_eng(SN_CL_ZIP, NULL, NULL, &h_zip_engine) != CLR_OK)
        {
            DBG_ERROR(("{WGL} unzip engine initialization failed. Line: %d\n", __LINE__));
            return WGLR_INTERNAL_ERROR;
        }
    }

    /*allocate handle*/
    if (HR_OK != handle_alloc(HT_WGL_IMG_TPL, pt_template, NULL, _wgl_img_tpl_free_cb, &h_template_handle))
    {
        i4_ret = WGLR_OUT_OF_HANDLES;
        goto _err_out;
    }

    /* if the image is in the compressed format, then a surface need not to be created. */
    {
        /*prepare information to create surface*/
        t_surf_dest.ui4_width = pt_image->ui2_width;
        t_surf_dest.ui4_height = pt_image->ui2_lines;
        t_surf_dest.e_clrmode = e_color_mode;
        t_surf_dest.h_palette = NULL_HANDLE;
        t_surf_dest.h_clipper = NULL_HANDLE;


        /*create surface*/
        if(GLR_OK != x_gl_surface_create(&t_surf_dest, &pt_template->h_surface))
        {
            i4_ret = WGLR_SYS_RC_CREATE_FAIL;
            goto _err_out;
        }

        /*cache sufrface info*/
        if(GLR_OK != x_gl_surface_get_info(pt_template->h_surface, &pt_template->t_surface_info))
        {
            i4_ret = WGLR_INTERNAL_ERROR;
            goto _err_out;
        }

        /*Lock surface*/
        if(GLR_OK != x_gl_surface_lock(pt_template->h_surface, NULL, GL_SYNC, &t_surf_lock_info))
        {
            i4_ret = WGLR_LOCK_ERROR;
            goto _err_out;
        }


        /*Copy Image Data*/
        /*surface start pos*/
        pui1_line_start = t_surf_lock_info.at_subplane[0].pv_bits;

        /*image start pos*/
        pui1_img_line_start = pt_image->pui1_image_data;

        /*bytes_per_line of image*/
        ui4_img_line_bytes = (_IS_FOUR_BYTES_ALIGNED(pt_image->ui4_image_type)) ?
                             _GET_FOUR_BYTES_ALIGNED_PITCH(pt_image->ui2_width, pt_image->ui1_bits_per_pixel) :
                             (((UINT32)pt_image->ui2_width) * ((UINT32)pt_image->ui1_bits_per_pixel) >> 3);


        /* allocate memory for the line buffer */
        pui1_line_buffer    = (UINT8 *)WGL_MEM_ALLOC(ui4_img_line_bytes * sizeof(UINT8));
        pui1_img_line_start = pui1_line_buffer;

        if (pui1_line_buffer == NULL)
        {
            i4_ret = WGLR_OUT_OF_MEMORY;
            goto _err_out;
        }

        /* get the info of the compressed image */
        z_img_pitch               = (SIZE_T)GET_UINT32_FROM_PTR_LITTLE_END(pt_image->pui1_image_data);
        z_compressed_length       = (SIZE_T)GET_UINT32_FROM_PTR_LITTLE_END(pt_image->pui1_image_data + sizeof(SIZE_T));
        pui1_compressed_img_start = pt_image->pui1_image_data + sizeof(INT32) * 2;


        /*difference between image and surface for each line*/
        i4_diff = ((INT32)t_surf_lock_info.at_subplane[0].ui4_pitch) - ((INT32)ui4_img_line_bytes);
        /*WGL_ASSERT(i4_diff >= 0);*/

        /*copy operation*/
        for(ui4_cur_line = 0; ui4_cur_line < pt_image->ui2_lines; ui4_cur_line++)
        {

            z_img_pitch = (SIZE_T)GET_UINT32_FROM_PTR_LITTLE_END(pt_image->pui1_image_data);
            /* decompress one line of the zipped image */
            x_cl_decompress(
                h_zip_engine,
                pui1_compressed_img_start,
                z_compressed_length,
                0,
                CL_NULL_PARAM_TYPE,
                NULL,
                pui1_img_line_start,
                &z_img_pitch,
                &h_zip_state);

            if(i4_diff > 0)
            {
                x_memcpy(pui1_line_start, pui1_img_line_start, ui4_img_line_bytes);
                x_memset(pui1_line_start + ui4_img_line_bytes, 0, (SIZE_T)i4_diff);
            }
            else
            {
                x_memcpy(pui1_line_start, pui1_img_line_start, t_surf_lock_info.at_subplane[0].ui4_pitch);
            }
            pui1_line_start += t_surf_lock_info.at_subplane[0].ui4_pitch;
        }

        /*Unlock surface*/
        if(GLR_OK != x_gl_surface_unlock(pt_template->h_surface, NULL))
        {
            i4_ret = WGLR_UNLOCK_ERROR;
            goto _err_out;
        }


        x_cl_close(h_zip_engine);
        x_cl_close(h_zip_state);
        WGL_MEM_FREE(pui1_img_line_start);
    }

    /*Segment Type*/
    pt_template->e_segment_type = e_segment_type;
    /*Extra Flag*/
    pt_template->ui2_flag = ui2_flag;

    /*prepare return value*/
    *ph_img_tpl = h_template_handle;
    return WGLR_OK;

_err_out:
    if(NULL_HANDLE != h_template_handle)
    {
        handle_free(h_template_handle, FALSE);
    }
    else
    {
        WGL_MEM_FREE(pt_template);
    }
    return i4_ret;
}
#endif

INT32 wgl_img_tpl_decompress(
    WGL_IMG_ST_T                e_segment_type,
    WGL_IMG_T*                  pt_image_src,
    WGL_IMG_T*                  pt_image_dst)
{
    /*variable declarations*/
    UINT8*                      pui1_img_line_start = NULL;
    UINT32                      ui4_img_line_bytes = 0;
    INT32                       i4_ret;
    HANDLE_T                    h_zip_engine, h_zip_state;
    SIZE_T                      z_dst_size = 0,z_compressed_length = 0;
    UINT8                       *pui1_compressed_img_start = NULL;
    /*check parameter*/
    if(NULL == pt_image_src || NULL == pt_image_dst)
    {
        return WGLR_INV_ARG;
    }

    /*check image*/
    if(!_IS_VALID_IMG(pt_image_src))
    {
        return WGLR_INV_ARG;
    }
    if(!_IS_SUPPORT_IMG(pt_image_src))
    {
        return WGLR_NOT_SUPPORT_FORMAT;
    }
    x_memset(pt_image_dst, 0, sizeof(WGL_IMG_T));
    if ((_IS_COMPRESSED_IMAGE(pt_image_src->ui4_image_type))  ||  (_IS_ZIPPED_IMAGE(pt_image_src->ui4_image_type) ))
    {
        x_memcpy(pt_image_dst, pt_image_src, sizeof(WGL_IMG_T));
        z_compressed_length   = (SIZE_T)GET_UINT32_FROM_PTR_LITTLE_END(pt_image_src->pui1_image_data + sizeof(SIZE_T));
        pui1_compressed_img_start = pt_image_src->pui1_image_data + sizeof(INT32) * 2;

        ui4_img_line_bytes = (_IS_FOUR_BYTES_ALIGNED(pt_image_src->ui4_image_type)) ?
                             _GET_FOUR_BYTES_ALIGNED_PITCH(pt_image_src->ui2_width, pt_image_src->ui1_bits_per_pixel) :
                             (((UINT32)pt_image_src->ui2_width) * ((UINT32)pt_image_src->ui1_bits_per_pixel) >> 3);

        pt_image_dst->pui1_image_data = (UINT8 *)WGL_MEM_ALLOC(ui4_img_line_bytes * pt_image_src->ui2_lines);
        if (pt_image_dst->pui1_image_data == NULL || pt_image_dst->pui1_image_data == pt_image_src->pui1_image_data)
        {
            return WGLR_OUT_OF_MEMORY;
        }
        pt_image_dst->ui4_image_size = ui4_img_line_bytes * pt_image_src->ui2_lines;
        z_dst_size = (SIZE_T)pt_image_dst->ui4_image_size;
        x_memset(pt_image_dst->pui1_image_data, 0 , ui4_img_line_bytes * pt_image_src->ui2_lines);
        pui1_img_line_start = pt_image_dst->pui1_image_data;
        if (x_cl_open_eng(SN_CL_ZIP, NULL, NULL, &h_zip_engine) != CLR_OK)
        {
            DBG_ERROR(("{WGL} unzip engine initialization failed. Line: %d\n", __LINE__));
            return WGLR_INTERNAL_ERROR;
        }
        i4_ret = x_cl_decompress(
                        h_zip_engine,
                        pui1_compressed_img_start,
                        z_compressed_length,
                        0,
                        CL_NULL_PARAM_TYPE,
                        NULL,
                        pui1_img_line_start,
                        &z_dst_size,
                        &h_zip_state);
        if (i4_ret != CLR_OK)
        {
            x_cl_close(h_zip_engine);
            x_cl_close(h_zip_state);
            return WGLR_INTERNAL_ERROR;
        }
        pt_image_dst->ui4_image_type &= ~WGL_IMG_TYPE_COMPRESSED_MASK;
        pt_image_dst->ui4_image_type &= ~WGL_IMG_TYPE_ZIPPED_MASK;
        x_cl_close(h_zip_engine);
        x_cl_close(h_zip_state);
        return WGLR_OK;
    }
    return WGLR_INTERNAL_ERROR;

}


/*******************************************************************************
 * Name
 *          wgl_img_tpl_create_by_surf
 * Description
 *
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_img_tpl_create_by_surf(
    WGL_IMG_ST_T                 e_segment_type,
    UINT16                       ui2_flag,
    GL_HSURFACE_T                h_src_surf,
    WGL_HIMG_TPL_T*              ph_img_tpl)
{
    /*variable declarations*/
    HANDLE_T                    h_template_handle = NULL_HANDLE;
    WGL_IMG_TPL_T*              pt_template = NULL;
    GL_SURFACE_DESC_T           t_surf_dest = {0};
    GL_SURF_INFO_T              t_src_info;
    INT32                       i4_ret;

    /*check parameter*/
    if(NULL_HANDLE == h_src_surf || NULL == ph_img_tpl)
    {
        return WGLR_INV_ARG;
    }

    /*initialize return value*/
    *ph_img_tpl = NULL_HANDLE;

    /*allocate memory*/
    pt_template = (WGL_IMG_TPL_T*) WGL_MEM_ALLOC(sizeof(WGL_IMG_TPL_T));
    if(NULL == pt_template)
    {
        return WGLR_OUT_OF_MEMORY;
    }

    /*initialize template image*/
    x_memset(pt_template, 0, sizeof(WGL_IMG_TPL_T));
    pt_template->h_surface = NULL_HANDLE;
#ifndef MW_GL_NO_COMPRESSED_IMG
    pt_template->pt_image = NULL;
#endif

    /*allocate handle*/
    if (HR_OK != handle_alloc(HT_WGL_IMG_TPL, pt_template, NULL, _wgl_img_tpl_free_cb, &h_template_handle))
    {
        i4_ret = WGLR_OUT_OF_HANDLES;
        goto _err_out;
    }

    /*get source surface info*/
    if(GLR_OK != x_gl_surface_get_info(h_src_surf, &t_src_info))
    {
        i4_ret = WGLR_INV_ARG;
        goto _err_out;
    }

    /*prepare information to create surface*/
    t_surf_dest.ui4_width = t_src_info.ui4_width;
    t_surf_dest.ui4_height = t_src_info.ui4_height;
    t_surf_dest.e_clrmode = t_src_info.e_clrmode;
    t_surf_dest.h_palette = NULL_HANDLE;
    t_surf_dest.h_clipper = NULL_HANDLE;

    /*create surface*/
    if(GLR_OK != x_gl_surface_create(&t_surf_dest, &pt_template->h_surface))
    {
        i4_ret = WGLR_SYS_RC_CREATE_FAIL;
        goto _err_out;
    }

    /*cache surface info*/
    if(GLR_OK != x_gl_surface_get_info(pt_template->h_surface, &pt_template->t_surface_info))
    {
        i4_ret = WGLR_INTERNAL_ERROR;
        goto _err_out;
    }

    /*copy image data*/
    if(GLR_OK != x_gl_surface_blt(pt_template->h_surface,
                                  h_src_surf,
                                  0,
                                  0,
                                  0,
                                  0,
                                  t_src_info.ui4_width,
                                  t_src_info.ui4_height,
                                  255,
                                  GL_SYNC))
    {
        i4_ret = WGLR_IMG_DRAW_FAIL;
        goto _err_out;
    }

    /*Segment Type*/
    pt_template->e_segment_type = e_segment_type;
    /*Extra Flag*/
    pt_template->ui2_flag = ui2_flag;

    _wgl_image_cache_add((VOID*) pt_template->pt_image,pt_template->h_surface);
    
#ifdef CLI_LVL_ALL
        if(_wgl_img_add_to_list(pt_template)!= WGLR_OK)
        {
             i4_ret = WGLR_INTERNAL_ERROR;
              goto _err_out;
        }
      pt_template->h_this = h_template_handle;
#endif

    /*prepare return value*/
    *ph_img_tpl = h_template_handle;
    return WGLR_OK;

_err_out:
    if(NULL_HANDLE != h_template_handle)
    {
        handle_free(h_template_handle, FALSE);
    }
    else
    {
        WGL_MEM_FREE(pt_template);
    }
    return i4_ret;
}


INT32 x_wgl_set_img_decode_function(WGL_IMG_DECODE_FUNCTION_TABLE_T* pt_dec_table)
{
    if (pt_dec_table == NULL || (pt_dec_table->pf_async_load == NULL && pt_dec_table->pf_sync_load == NULL)||
        pt_dec_table->pf_close == NULL ||(pt_dec_table->pf_fopen == NULL && pt_dec_table->pf_mopen == NULL) ||
        pt_dec_table->pf_get_dimension == NULL)
    {
        return WGLR_INV_ARG;
    }

    decode_funtion_table.pf_async_load = pt_dec_table->pf_async_load;
    decode_funtion_table.pf_sync_load  = pt_dec_table->pf_sync_load;
    decode_funtion_table.pf_close      = pt_dec_table->pf_close;
    decode_funtion_table.pf_fopen      = pt_dec_table->pf_fopen;
    decode_funtion_table.pf_mopen      = pt_dec_table->pf_mopen;
    decode_funtion_table.pf_get_dimension = pt_dec_table->pf_get_dimension;
	decode_funtion_table.pf_set_sw_decode_force = pt_dec_table->pf_set_sw_decode_force;
    return WGLR_OK;
}


INT32 x_wgl_set_image_decode_sw_force(BOOL fg_sw)
{
	if (decode_funtion_table.pf_set_sw_decode_force != NULL)
	{
		decode_funtion_table.pf_set_sw_decode_force(fg_sw);
    	return WGLR_OK;
	}
	return WGLR_FUNC_NOT_IMPL;
}

INT32 x_wgl_set_invert_height(WGL_HIMG_TPL_T                          h_img_tpl,
                                       UINT16                                  ui2_invert_lines)
{
    WGL_IMG_TPL_T*              pt_template = NULL;
    HANDLE_TYPE_T               e_type;

    DBG_API(("{WGL(IMG)} %s: h_img_tpl = 0x%.8x \n", __FUNCTION__, h_img_tpl));

	_wgl_img_module_lock();

    if(ui2_invert_lines == 0 ||NULL_HANDLE == h_img_tpl || HR_OK != handle_get_type_obj((HANDLE_T)h_img_tpl, &e_type, (VOID**)&pt_template) || HT_WGL_IMG_TPL != e_type)
    {
        DBG_ERROR(("{WGL(IMG)} ERR: Invalid arguments. Free NULL template image. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
		_wgl_img_module_unlock();
		return WGLR_INV_ARG;
    }
    if(NULL == pt_template)
    {
        DBG_ERROR(("{WGL(IMG)} ERR: Template image's instance is NULL. (%d) at L%d\n\n", WGLR_INTERNAL_ERROR, __LINE__));
        _wgl_img_module_unlock();
		return WGLR_INTERNAL_ERROR;
    }
    if (pt_template->t_surface_info.ui4_height < ui2_invert_lines * 2)
    {
        DBG_ERROR(("{WGL(IMG)} ERR: Template image's instance is NULL. (%d) at L%d\n\n", WGLR_INTERNAL_ERROR, __LINE__));
        _wgl_img_module_unlock();
		return WGLR_INTERNAL_ERROR;
    }
    pt_template->ui2_invert_lines = ui2_invert_lines;
    pt_template->ui2_flag |= WGL_IMG_FLAG_PERFETCH;
	_wgl_img_module_unlock();
    return WGLR_OK;

}

/*******************************************************************************
 * Name
 *          x_wgl_img_tpl_image_data_clone
 * Description
 *          please call x_gl_obj_free() to free ph_surface when you don't need it.
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 x_wgl_img_tpl_image_data_clone(
    WGL_HIMG_TPL_T                          h_img_tpl,
    WGL_IMG_ST_T*                           pe_type,
    GL_HSURFACE_T*                          ph_surface)
{
    INT32                           i4_ret;
    WGL_IMG_TPL_CLONE_FCT_TABLE_T   t_clone_fct_tbl = {x_gl_surface_create,
                                                       x_gl_surface_blt,
                                                       x_gl_obj_free};

    DBG_API(("{WGL(IMG)} %s: h_img_tpl = 0x%.8x \tpe_type = 0x%.8x \tph_surface = 0x%.8x\n",
        __FUNCTION__,
        h_img_tpl,
        pe_type,
        ph_surface));

	_wgl_img_module_lock();

    i4_ret = wgl_img_tpl_image_data_clone(&t_clone_fct_tbl, h_img_tpl, pe_type, ph_surface);
    WGL_SETS_CLI_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(IMG)} ERR: Fail to clone image data from template image. (%d) at L%d\n\n", i4_ret, __LINE__)));
	_wgl_img_module_unlock();
	return i4_ret;
}


/*******************************************************************************
 * Name
 *          x_wgl_img_tpl_create
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 x_wgl_img_tpl_create(
    WGL_IMG_ST_T                e_segment_type,
    const WGL_IMG_T*            pt_image,
    WGL_HIMG_TPL_T*             ph_img_tpl)
{
    INT32                       i4_ret;

    DBG_API(("{WGL(IMG)} %s: e_segment_type = 0x%.8x \tpt_image = 0x%.8x \tph_img_tpl = 0x%.8x\n",
        __FUNCTION__,
        e_segment_type,
        pt_image,
        ph_img_tpl));

    DBG_INFO(("{WGL(IMG)} pt_image (w, h, d)= 0x%.8x (%d, %d, %d)\n"
              "\ttype = 0x%.8x   size = %d   data = 0x%.8x\n"
              "\ttable_cols = %d   table_data = 0x%.8x\n",
              pt_image, pt_image->ui2_width, pt_image->ui2_lines,   pt_image->ui1_bits_per_pixel,
              pt_image->ui4_image_type, pt_image->ui4_image_size, pt_image->pui1_image_data,
              pt_image->ui2_table_columns, pt_image->pt_table_data));
	_wgl_img_module_lock();
    i4_ret = wgl_img_tpl_create(e_segment_type, 0, pt_image, ph_img_tpl);
    WGL_SETS_CLI_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(IMG)} ERR: Fail to create template image. (%d) at L%d\n\n", i4_ret, __LINE__)));
	_wgl_img_module_unlock();
    return i4_ret;
}

INT32 x_wgl_img_tpl_create_ex(
    WGL_IMG_ST_T                e_segment_type,
    const WGL_IMG_T*            pt_image,
    WGL_HIMG_TPL_T*             ph_img_tpl)
{
    INT32                       i4_ret;

    DBG_API(("{WGL(IMG)} %s: e_segment_type = 0x%.8x \tpt_image = 0x%.8x \tph_img_tpl = 0x%.8x\n",
        __FUNCTION__,
        e_segment_type,
        pt_image,
        ph_img_tpl));

    DBG_INFO(("{WGL(IMG)} pt_image (w, h, d)= 0x%.8x (%d, %d, %d)\n"
              "\ttype = 0x%.8x   size = %d   data = 0x%.8x\n"
              "\ttable_cols = %d   table_data = 0x%.8x\n",
              pt_image, pt_image->ui2_width, pt_image->ui2_lines,   pt_image->ui1_bits_per_pixel,
              pt_image->ui4_image_type, pt_image->ui4_image_size, pt_image->pui1_image_data,
              pt_image->ui2_table_columns, pt_image->pt_table_data));
#if 1
    WGL_IMG_T*  pt_image_dst = (WGL_IMG_T*)WGL_MEM_ALLOC(sizeof(WGL_IMG_T));
    if (pt_image_dst == NULL)
    {
        return WGLR_OUT_OF_MEMORY;
    }
    x_memset(pt_image_dst, 0, sizeof(WGL_IMG_T));
	_wgl_img_module_lock();
    i4_ret = wgl_img_tpl_decompress(e_segment_type,(WGL_IMG_T*)pt_image,pt_image_dst);
    if (i4_ret != WGLR_OK)
    {
        WGL_MEM_FREE(pt_image_dst);
        i4_ret = wgl_img_tpl_create(e_segment_type, 0, pt_image, ph_img_tpl);
        WGL_SETS_CLI_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(IMG)} ERR: Fail to create template image. (%d) at L%d\n\n", i4_ret, __LINE__)));

		_wgl_img_module_unlock();
		return i4_ret;
    }
    i4_ret = wgl_img_tpl_create(e_segment_type, 0, pt_image_dst, ph_img_tpl);
    if(pt_image_dst->pui1_image_data != NULL)
    {
        WGL_MEM_FREE(pt_image_dst->pui1_image_data);
    }
    WGL_MEM_FREE(pt_image_dst);
#else
    i4_ret = wgl_img_tpl_create_ex(e_segment_type, 0, pt_image, ph_img_tpl);
#endif
    WGL_SETS_CLI_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(IMG)} ERR: Fail to create template image. (%d) at L%d\n\n", i4_ret, __LINE__)));
	_wgl_img_module_unlock();

    return i4_ret;
}

/*******************************************************************************
 * Name
 *          x_wgl_img_tpl_create_by_surf
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 x_wgl_img_tpl_create_by_surf(
    WGL_IMG_ST_T                   e_segment_type,
    GL_HSURFACE_T                  h_src_surf,
    WGL_HIMG_TPL_T*                ph_img_tpl)
{
    INT32                       i4_ret;

    DBG_API(("{WGL(IMG)} %s: e_segment_type = 0x%.8x \th_src_surf = 0x%.8x \tph_img_tpl = 0x%.8x\n",
        __FUNCTION__,
        e_segment_type,
        h_src_surf,
        ph_img_tpl));
	_wgl_img_module_lock();
    i4_ret = wgl_img_tpl_create_by_surf(e_segment_type, 0, h_src_surf, ph_img_tpl);
    WGL_SETS_CLI_REPORT(WGLR_OK, i4_ret, DBG_ERROR(("{WGL(IMG)} ERR: Fail to create template image. (%d) at L%d\n\n", i4_ret, __LINE__)));
	_wgl_img_module_unlock();
    return i4_ret;
}


/*******************************************************************************
 * Name
 *          x_wgl_img_tpl_destroy
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 x_wgl_img_tpl_destroy(
    WGL_HIMG_TPL_T              h_img_tpl)
{
    WGL_IMG_TPL_T*              pt_template = NULL;
    HANDLE_TYPE_T               e_type;

    DBG_API(("{WGL(IMG)} %s: h_img_tpl = 0x%.8x \n", __FUNCTION__, h_img_tpl));
	_wgl_img_module_lock();
    if(NULL_HANDLE == h_img_tpl || HR_OK != handle_get_type_obj((HANDLE_T)h_img_tpl, &e_type, (VOID**)&pt_template) || HT_WGL_IMG_TPL != e_type)
    {
        DBG_ERROR(("{WGL(IMG)} ERR: Invalid arguments. Free NULL template image. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
		_wgl_img_module_unlock();
        return WGLR_INV_ARG;
    }

    if(NULL == pt_template)
    {
        DBG_ERROR(("{WGL(IMG)} ERR: Template image's instance is NULL. (%d) at L%d\n\n", WGLR_INTERNAL_ERROR, __LINE__));
		_wgl_img_module_unlock();
        return WGLR_INTERNAL_ERROR;
    }

    if(_IS_SYSTEM_PROVIDE(pt_template->ui2_flag))
    {
        DBG_ERROR(("{WGL(IMG)} ERR: This template image's instance is provided by the system. (%d) at L%d\n\n", WGLR_SYS_RC_CANNOT_FREE, __LINE__));
		_wgl_img_module_unlock();
        return WGLR_SYS_RC_CANNOT_FREE;
    }

    if(HR_OK != handle_free(h_img_tpl, FALSE))
    {
        DBG_ERROR(("{WGL(IMG)} ERR: Fail to free a template image handle. (%d) at L%d\n\n", WGLR_INTERNAL_ERROR, __LINE__));
		_wgl_img_module_unlock();
        return WGLR_INTERNAL_ERROR;
    }
	_wgl_img_module_unlock();
    return WGLR_OK;
}

INT32 x_wgl_img_tpl_paint(
    WGL_HIMG_TPL_T             h_img_tpl,
    const GL_RECT_T*           pt_output_rect,
    const GL_RECT_T*           pt_invalid_rect,
    GL_HGRAPHICS_T             h_graphics,
    const WGL_CANVAS_STATUS_T  *pt_cvsst,
    WGL_TPL_PAINT_METHOD_T     e_paint_method)
{
    return wgl_img_tpl_paint(
        h_img_tpl,
        pt_output_rect,
        pt_invalid_rect,
        h_graphics,
        pt_cvsst,
        e_paint_method);
}


INT32 x_wgl_img_cache_flush(VOID)
{
    return x_gl_nhv_cache_flush();
}
/*******************************************************************************
 * Name
 *          x_wgl_img_tpl_update_by_surf
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 x_wgl_img_tpl_update_by_surf(
    WGL_HIMG_TPL_T                 h_dest_img_tpl,
    GL_HSURFACE_T                  h_src_surf)
{
    WGL_IMG_TPL_T*                 pt_template = NULL;
    HANDLE_TYPE_T                  e_type;

    /*check parameter and get template image info*/
	_wgl_img_module_lock();
    if(NULL_HANDLE == h_dest_img_tpl ||
       HR_OK != handle_get_type_obj((HANDLE_T)h_dest_img_tpl, &e_type, (VOID**)&pt_template) ||
       HT_WGL_IMG_TPL != e_type)
    {
    	_wgl_img_module_unlock();
        return WGLR_INV_ARG;
    }
    if(NULL == pt_template)
    {
    	_wgl_img_module_unlock();
        return WGLR_INTERNAL_ERROR;
    }
    /* if this image template is in the compressed format, it can not be updated by a surface. */
    if (_IS_IMG_TPL_COMPRESSED(pt_template) ||
        _IS_IMG_TPL_PNG(pt_template)||
        _IS_IMG_TPL_JPG(pt_template)||
        _IS_IMG_TPL_BMP(pt_template))
    {
    	_wgl_img_module_unlock();
        return WGLR_INV_CMD;
    }

    /*copy image data*/
    if(GLR_OK != x_gl_surface_blt(pt_template->h_surface,
                                  h_src_surf,
                                  0,
                                  0,
                                  0,
                                  0,
                                  pt_template->t_surface_info.ui4_width,
                                  pt_template->t_surface_info.ui4_height,
                                  255,
                                  GL_SYNC))
    {
    	_wgl_img_module_unlock();
        return WGLR_IMG_DRAW_FAIL;
    }
	_wgl_img_module_unlock();
    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *          x_wgl_img_tpl_destroy_surface
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 x_wgl_img_tpl_destroy_surface(
        const WGL_HIMG_TPL_T*                  ph_img_tpl,
        UINT16                                 ui2_img_nbs)
{
    WGL_IMG_TPL_T**                ppt_template = NULL;
    HANDLE_TYPE_T                  e_type;
    UINT16                         ui2_i = 0;


    /*check parameter*/
    if (NULL == ph_img_tpl || ui2_img_nbs <= 0)
    {
        return WGLR_INV_ARG;
    }
    _wgl_img_module_lock();
    /* check the img handle valid */
    ppt_template = (WGL_IMG_TPL_T**)WGL_MEM_ALLOC(ui2_img_nbs * sizeof(WGL_IMG_TPL_T*));
    if (NULL == ppt_template)
    {
    	_wgl_img_module_unlock();
        return WGLR_OUT_OF_MEMORY;
    }
    x_memset(ppt_template, 0, ui2_img_nbs * sizeof(WGL_IMG_TPL_T*));

    for(ui2_i = 0; ui2_i < ui2_img_nbs; ui2_i++)
    {
        if(NULL_HANDLE == ph_img_tpl[ui2_i] ||
           HR_OK != handle_get_type_obj((HANDLE_T)ph_img_tpl[ui2_i], &e_type, (VOID**)&ppt_template[ui2_i]) ||
           HT_WGL_IMG_TPL != e_type ||
           NULL == ppt_template[ui2_i])
        {
            WGL_MEM_FREE(ppt_template);
        	_wgl_img_module_unlock();
            return WGLR_INV_ARG;
        }
        /* if this image template is not the type of WGL_IMG_TYPE_PNG */
        if (!_IS_IMG_TPL_JPG(ppt_template[ui2_i]) &&
            !_IS_IMG_TPL_PNG(ppt_template[ui2_i]) &&
            !_IS_IMG_TPL_BMP(ppt_template[ui2_i]) )
        {
            WGL_MEM_FREE(ppt_template);
        	_wgl_img_module_unlock();
            return WGLR_INV_ARG;
        }
    }

    for (ui2_i = 0; ui2_i < ui2_img_nbs; ui2_i++)
    {
        if (WGLR_OK != wgl_img_tpl_destroy_surface(ppt_template[ui2_i]))
        {
            WGL_MEM_FREE(ppt_template);
			_wgl_img_module_unlock();
            return WGLR_INTERNAL_ERROR;
        }
    }

    WGL_MEM_FREE(ppt_template);
    ppt_template = NULL;
	_wgl_img_module_unlock();
    return WGLR_OK;
}


/*******************************************************************************
 * Name
 *          x_wgl_img_tpl_create_surface
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 x_wgl_img_tpl_create_surface(
        const WGL_HIMG_TPL_T*                  ph_img_tpl,
        UINT16                                 ui2_img_nbs)
{
    WGL_IMG_TPL_T**                ppt_template = NULL;
    HANDLE_TYPE_T                  e_type;
    UINT16                         ui2_i = 0;


    /*check parameter*/
    if (NULL == ph_img_tpl || ui2_img_nbs <= 0)
    {
        return WGLR_INV_ARG;
    }

    /* begin to decode the png images */
    _wgl_img_decode_lock();

    /* init the png img decode status data */
    /* all of the png img decode status set FALSE */
    _wgl_img_free_status_data(&_gpv_image_status);
    _wgl_img_alloc_status_data(&_gpv_image_status, ui2_img_nbs);
    if (_gpv_image_status == NULL)
    {
        _wgl_img_decode_unlock();
        return WGLR_OUT_OF_MEMORY;
    }

    /* check the img handle valid */
    ppt_template = (WGL_IMG_TPL_T**)WGL_MEM_ALLOC(ui2_img_nbs * sizeof(WGL_IMG_TPL_T*));
    if (NULL == ppt_template)
    {
        _wgl_img_free_status_data(&_gpv_image_status);
        _wgl_img_decode_unlock();
        return WGLR_OUT_OF_MEMORY;
    }
    x_memset(ppt_template, 0, ui2_img_nbs * sizeof(WGL_IMG_TPL_T*));

    HANDLE_T* ph_img = ((WGL_IMG_TPL_JPG_DECODE_STATUS_T*)_gpv_image_status)->ph_img;
    for(ui2_i = 0; ui2_i < ui2_img_nbs; ui2_i++)
    {
        if(NULL_HANDLE == ph_img_tpl[ui2_i] ||
           HR_OK != handle_get_type_obj((HANDLE_T)ph_img_tpl[ui2_i], &e_type, (VOID**)&ppt_template[ui2_i]) ||
           HT_WGL_IMG_TPL != e_type ||
           NULL == ppt_template[ui2_i])
        {
            _wgl_img_free_status_data(&_gpv_image_status);
            WGL_MEM_FREE(ppt_template);
            _wgl_img_decode_unlock();
            return WGLR_INV_ARG;
        }
        /* if this image template is not the type of WGL_IMG_TYPE_PNG */
        if (!_IS_IMG_TPL_JPG(ppt_template[ui2_i]) && !_IS_IMG_TPL_PNG(ppt_template[ui2_i]) && !_IS_IMG_TPL_BMP(ppt_template[ui2_i]))
        {
            _wgl_img_free_status_data(&_gpv_image_status);
            WGL_MEM_FREE(ppt_template);
            _wgl_img_decode_unlock();
            return WGLR_INV_ARG;
        }
        ph_img[ui2_i] = ppt_template[ui2_i]->h_img;
    }

    _gfg_image_decode_statuc = TRUE;

    for (ui2_i = 0; ui2_i < ui2_img_nbs; ui2_i++)
    {
        if (WGLR_OK != wgl_img_tpl_create_surface(ppt_template[ui2_i]))
        {
            DBG_ERROR(("{WGL(IMG)} %s: decode png handle %d return error!\r\n", __FUNCTION__,
                ph_img_tpl[ui2_i]));
            _gfg_image_decode_statuc = FALSE;
            _wgl_img_free_status_data(&_gpv_image_status);
            WGL_MEM_FREE(ppt_template);
            _wgl_img_decode_unlock();
            x_wgl_img_tpl_destroy_surface(ph_img_tpl, ui2_img_nbs);
            return WGLR_INTERNAL_ERROR;
        }
    }

    _gfg_image_decode_statuc = FALSE;

    /* now all of the png img have been decoded, so the data should be free */
    _wgl_img_free_status_data(&_gpv_image_status);
    WGL_MEM_FREE(ppt_template);
    ppt_template = NULL;

    _wgl_img_decode_unlock();

    return WGLR_OK;
}
/*******************************************************************************
 * Name
 *          wgl_img_tpl_paint
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/

INT32 wgl_img_tpl_strech_paint(
    WGL_HIMG_TPL_T             h_img_tpl,
    const GL_RECT_T*           pt_output_rect,
    const GL_RECT_T*           pt_invalid_rect,
    GL_HGRAPHICS_T             h_graphics,
    const WGL_CANVAS_STATUS_T  *pt_cvsst,
    WGL_TPL_PAINT_METHOD_T     e_paint_method)
{
    /*local variable declaration*/
    WGL_IMG_TPL_T*              pt_template = NULL;
	INT32                       i4_ret = WGLR_OK;
    /*check parameter*/
    if(NULL_HANDLE == h_img_tpl || NULL == pt_output_rect || NULL_HANDLE == h_graphics)
    {
        return WGLR_INV_ARG;
    }

    
    if(WGL_RECT_IS_EMPTY(pt_output_rect))
    {
        return WGLR_OK;
    }

    if(pt_invalid_rect && WGL_RECT_IS_EMPTY(pt_invalid_rect))
    {
        return WGLR_OK;
    }

	_wgl_img_module_lock();
    /*get template image information*/
    if(HR_OK != handle_get_obj((HANDLE_T)h_img_tpl, (void**)&pt_template))
    {
		_wgl_img_module_unlock();
        return WGLR_INV_HANDLE;
    }
    if(NULL == pt_template)
    {
		_wgl_img_module_unlock();
        return WGLR_INTERNAL_ERROR;
    }

    if (pt_template->e_segment_type != WGL_IMG_ST_NO_SEGMENT || pt_template->h_surface == NULL_HANDLE)
    {
		_wgl_img_module_unlock();
        return WGLR_NOT_SUPPORT_FORMAT;
    }
     
    
#ifdef WGL_ANIM_SUPPORT 
    if(e_paint_method == WGL_TPL_PAINT_BITBLT)
    {
        e_paint_method = WGL_TPL_PAINT_COMPOSE;
    }
#else
#ifdef WGL_IMG_ALWAYS_COMPOSE 
    if(e_paint_method == WGL_TPL_PAINT_BITBLT)
    {
        e_paint_method = WGL_TPL_PAINT_COMPOSE;
    }
#endif
#endif
	
    i4_ret = _wgl_img_tpl_strech_paint(pt_template, pt_output_rect,pt_invalid_rect,h_graphics,pt_cvsst,e_paint_method);
	_wgl_img_module_unlock();
	return i4_ret;
}


INT32 _wgl_img_tpl_strech_paint(
    WGL_IMG_TPL_T*             pt_template,
    const GL_RECT_T*           pt_output_rect,
    const GL_RECT_T*           pt_invalid_rect,
    GL_HGRAPHICS_T             h_graphics,
    const WGL_CANVAS_STATUS_T  *pt_cvsst,
    WGL_TPL_PAINT_METHOD_T     e_paint_method)
{
    /*local variable declaration*/
    INT32                       i4_ret;
    GL_HSURFACE_T               h_temp_surf = NULL_HANDLE;    
    GL_SURFACE_DESC_T           t_surf_dest = {0};
    /*check parameter*/
    if(NULL == pt_output_rect || NULL_HANDLE == h_graphics)
    {
        return WGLR_INV_ARG;
    }

    
    if(WGL_RECT_IS_EMPTY(pt_output_rect))
    {
        return WGLR_OK;
    }

    if(pt_invalid_rect && WGL_RECT_IS_EMPTY(pt_invalid_rect))
    {
        return WGLR_OK;
    }
    
    if (pt_template->e_segment_type != WGL_IMG_ST_NO_SEGMENT || pt_template->h_surface == NULL_HANDLE)
    {
        return WGLR_NOT_SUPPORT_FORMAT;
    }
     
    if (e_paint_method == WGL_TPL_PAINT_COMPOSE)
    {
        t_surf_dest.ui4_width  = RECT_W(pt_output_rect);
        t_surf_dest.ui4_height = RECT_H(pt_output_rect);
        t_surf_dest.e_clrmode  = pt_template->t_surface_info.e_clrmode;
        t_surf_dest.h_palette  = NULL_HANDLE;
        t_surf_dest.h_clipper  = NULL_HANDLE;

        i4_ret = x_gl_surface_create(&t_surf_dest,&h_temp_surf);
        if (i4_ret != GLR_OK || h_temp_surf == NULL_HANDLE)
        {
            return WGLR_INTERNAL_ERROR;
        }

        i4_ret = x_gl_surface_adv_stretch_blt(h_temp_surf,
                                          pt_template->h_surface,
                                          0,
                                          0,
                                          RECT_W(pt_output_rect),
                                          RECT_H(pt_output_rect),
                                          0,
                                          0,
                                          pt_template->t_surface_info.ui4_width,
                                          pt_template->t_surface_info.ui4_height,
                                          GL_SYNC);
        if (i4_ret != GLR_OK)
        {
            x_gl_obj_free(h_temp_surf);
            return WGLR_INTERNAL_ERROR;
        }

        i4_ret = x_gl_compose(h_graphics,
                              h_temp_surf,
                              pt_output_rect->i4_left,
                              pt_output_rect->i4_top,
                              0,
                              0,
                              RECT_W(pt_output_rect),
                              RECT_H(pt_output_rect),
                              255,
                              GL_SRC_OVER);
        if (i4_ret != GLR_OK)
        {
            x_gl_obj_free(h_temp_surf);
            return WGLR_INTERNAL_ERROR;
        }

        x_gl_obj_free(h_temp_surf);

        return GLR_OK;
        
    }

    i4_ret = x_gl_adv_stretch_blt_ex(h_graphics,
                              pt_template->h_surface,
                              pt_output_rect->i4_left,
                              pt_output_rect->i4_top,
                              RECT_W(pt_output_rect),
                              RECT_H(pt_output_rect),
                              0,
                              0,
                              pt_template->t_surface_info.ui4_width,
                              pt_template->t_surface_info.ui4_height,
                              GL_STRETCH_PHOTO);
   
    if (i4_ret != GLR_OK)
    {
        return WGLR_INTERNAL_ERROR;
    }
    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *          wgl_img_tpl_paint
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_img_tpl_paint(
    WGL_HIMG_TPL_T             h_img_tpl,
    const GL_RECT_T*           pt_output_rect,
    const GL_RECT_T*           pt_invalid_rect,
    GL_HGRAPHICS_T             h_graphics,
    const WGL_CANVAS_STATUS_T  *pt_cvsst,
    WGL_TPL_PAINT_METHOD_T     e_paint_method)
{
    /*local variable declaration*/
    WGL_IMG_TPL_T*              pt_template = NULL;
    BOOL                        b_create_tmp_surface = FALSE;
    INT32                       i4_ret;
    /*check parameter*/
    if(NULL_HANDLE == h_img_tpl || NULL == pt_output_rect || NULL_HANDLE == h_graphics)
    {
        return WGLR_INV_ARG;
    }

#ifdef WGL_ANIM_SUPPORT
    if(e_paint_method == WGL_TPL_PAINT_BITBLT)
    {
        e_paint_method = WGL_TPL_PAINT_COMPOSE;
    }
#else
#ifdef WGL_IMG_ALWAYS_COMPOSE
    if(e_paint_method == WGL_TPL_PAINT_BITBLT)
    {
        e_paint_method = WGL_TPL_PAINT_COMPOSE;
    }
#endif
#endif

	_wgl_img_module_lock();
    /*get template image information*/
    if(HR_OK != handle_get_obj((HANDLE_T)h_img_tpl, (void**)&pt_template))
    {
    	_wgl_img_module_unlock();
        return WGLR_INV_HANDLE;
    }
    if(NULL == pt_template)
    {
    	_wgl_img_module_unlock();
        return WGLR_INTERNAL_ERROR;
    }

    if(WGL_RECT_IS_EMPTY(pt_output_rect))
    {
    	_wgl_img_module_unlock();
        return WGLR_OK;
    }

    if(pt_invalid_rect && WGL_RECT_IS_EMPTY(pt_invalid_rect))
    {
    	_wgl_img_module_unlock();
        return WGLR_OK;
    }

    if (_IS_IMG_TPL_JPG(pt_template) || _IS_IMG_TPL_PNG(pt_template) || _IS_IMG_TPL_BMP(pt_template))
    {
        if (pt_template->h_surface == NULL_HANDLE)
        {
        	_wgl_img_module_unlock();
            b_create_tmp_surface = TRUE;
            i4_ret = x_wgl_img_tpl_create_surface(
                &h_img_tpl, 1);
            if (WGLR_OK != i4_ret)
            {
                return i4_ret;
            }
			
            _wgl_img_module_lock();
        }
    }

    /*check segment type*/
    if (pt_template->pt_image != NULL && _IS_AUTO_STRETCH_IMAGE(pt_template->pt_image->ui4_image_type))
    {
        i4_ret = _wgl_img_tpl_strech_paint(pt_template,pt_output_rect,pt_invalid_rect,h_graphics, pt_cvsst, e_paint_method);
    }
    else
    {
        switch (pt_template->e_segment_type)
        {
        case WGL_IMG_ST_H_ONLY:
            i4_ret = _wgl_img_tpl_HO_paint(pt_template, pt_output_rect, pt_invalid_rect, h_graphics, pt_cvsst, e_paint_method, WGL_TPL_PAINT_ALGSM_L2R_T2B_OVERLAP);
            break;
        case WGL_IMG_ST_V_ONLY:
            i4_ret = _wgl_img_tpl_VO_paint(pt_template, pt_output_rect, pt_invalid_rect, h_graphics, pt_cvsst, e_paint_method, WGL_TPL_PAINT_ALGSM_L2R_T2B_OVERLAP);
            break;
        case WGL_IMG_ST_HV:
            i4_ret = _wgl_img_tpl_HV_paint(pt_template, pt_output_rect, pt_invalid_rect, h_graphics, pt_cvsst, e_paint_method, WGL_TPL_PAINT_ALGSM_L2R_T2B_OVERLAP);
            break;
        case WGL_IMG_ST_NO_SEGMENT:
            i4_ret = _wgl_img_tpl_NHV_paint(pt_template, pt_output_rect, pt_invalid_rect, h_graphics, pt_cvsst, e_paint_method);
            break;
        default:
            i4_ret = WGLR_INTERNAL_ERROR;
        }
    }
    
	_wgl_img_module_unlock();

    if ( (_IS_IMG_TPL_JPG(pt_template) || _IS_IMG_TPL_PNG(pt_template) || _IS_IMG_TPL_BMP(pt_template)) && b_create_tmp_surface)
    {
        x_wgl_img_tpl_destroy_surface(&h_img_tpl, 1);
    }
    return i4_ret;
}

/*******************************************************************************
 * Name
 *          wgl_img_tpl_paint_ex
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
extern INT32 wgl_img_tpl_paint_ex(
    WGL_HIMG_TPL_T                h_img_tpl,
    const GL_RECT_T*              pt_output_rect,
    const GL_RECT_T*              pt_invalid_rect,
    GL_HGRAPHICS_T                h_graphics,
    const WGL_CANVAS_STATUS_T*    pt_cvsst,
    WGL_TPL_PAINT_METHOD_T        e_paint_method,
    WGL_TPL_PAINT_ALGSM_T         e_algorism)
{
    /*local variable declaration*/
    WGL_IMG_TPL_T*              pt_template = NULL;
    BOOL                        b_create_tmp_surface = FALSE;
    INT32                       i4_ret;

    /*check parameter*/
    if(NULL_HANDLE == h_img_tpl || NULL == pt_output_rect || NULL_HANDLE == h_graphics)
    {
        return WGLR_INV_ARG;
    }
	_wgl_img_module_lock();
    /*get template image information*/
    if(HR_OK != handle_get_obj((HANDLE_T)h_img_tpl, (void**)&pt_template))
    {
    	_wgl_img_module_unlock();
        return WGLR_INV_HANDLE;
    }
    if(NULL == pt_template)
    {
    	_wgl_img_module_unlock();
        return WGLR_INTERNAL_ERROR;
    }

    if(WGL_RECT_IS_EMPTY(pt_output_rect))
    {
    	_wgl_img_module_unlock();
        return WGLR_OK;
    }

    if(pt_invalid_rect && WGL_RECT_IS_EMPTY(pt_invalid_rect))
    {
    	_wgl_img_module_unlock();
        return WGLR_OK;
    }
    if (_IS_IMG_TPL_JPG(pt_template) || _IS_IMG_TPL_PNG(pt_template) || _IS_IMG_TPL_BMP(pt_template))
    {
        if (pt_template->h_surface == NULL_HANDLE)
        {
        	_wgl_img_module_unlock();
            b_create_tmp_surface = TRUE;
            i4_ret = x_wgl_img_tpl_create_surface(
                &h_img_tpl, 1);
            if (WGLR_OK != i4_ret)
            {
                return i4_ret;
            }
			_wgl_img_module_lock();
        }
    }
    /*check segment type*/
    switch (pt_template->e_segment_type)
    {
    case WGL_IMG_ST_H_ONLY:
        i4_ret = _wgl_img_tpl_HO_paint(pt_template, pt_output_rect, pt_invalid_rect, h_graphics, pt_cvsst, e_paint_method, e_algorism);
        break;
    case WGL_IMG_ST_V_ONLY:
        i4_ret = _wgl_img_tpl_VO_paint(pt_template, pt_output_rect, pt_invalid_rect, h_graphics, pt_cvsst, e_paint_method, e_algorism);
        break;
    case WGL_IMG_ST_HV:
        i4_ret = _wgl_img_tpl_HV_paint(pt_template, pt_output_rect, pt_invalid_rect, h_graphics, pt_cvsst, e_paint_method, e_algorism);
        break;
    case WGL_IMG_ST_NO_SEGMENT:
        i4_ret = _wgl_img_tpl_NHV_paint(pt_template, pt_output_rect, pt_invalid_rect, h_graphics, pt_cvsst, e_paint_method);
        break;
    default:
        i4_ret = WGLR_INTERNAL_ERROR;
    }
	_wgl_img_module_unlock();
    if ( (_IS_IMG_TPL_JPG(pt_template) || _IS_IMG_TPL_PNG(pt_template) || _IS_IMG_TPL_BMP(pt_template))&& b_create_tmp_surface)
    {
        x_wgl_img_tpl_destroy_surface(&h_img_tpl, 1);
    }
    return i4_ret;
}

/*******************************************************************************
 * Name
 *          x_wgl_img_tpl_get_size
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 x_wgl_img_tpl_get_size(
    WGL_HIMG_TPL_T              h_img_tpl,
    UINT32*                     pui4_width,
    UINT32*                     pui4_height)
{
    /*local variable declaration*/
    WGL_IMG_TPL_T*              pt_template = NULL;
    HANDLE_TYPE_T               e_type;

    DBG_API(("{WGL(IMG)} %s: h_img_tpl = 0x%.8x   pui4_width = 0x%.8x   pui4_height = 0x%.8x\n", __FUNCTION__, h_img_tpl, pui4_width, pui4_height));

    /*check parameter*/
    if(NULL_HANDLE == h_img_tpl || NULL == pui4_width || NULL == pui4_height)
    {
        DBG_ERROR(("{WGL(IMG)} ERR: Invalid arguments. One of its arguments is NULL. (%d) at L%d\n\n", WGLR_INV_ARG, __LINE__));
        return WGLR_INV_ARG;
    }
	_wgl_img_module_lock();
    /*get template image information*/
    if(HR_OK != handle_get_type_obj((HANDLE_T)h_img_tpl, &e_type, (VOID**)&pt_template) || HT_WGL_IMG_TPL != e_type)
    {
        DBG_ERROR(("{WGL(IMG)} ERR: The input handle isn't template image handle. (%d) at L%d\n\n", WGLR_INV_HANDLE, __LINE__));
		_wgl_img_module_unlock();
		return WGLR_INV_HANDLE;
    }
    if(NULL == pt_template)
    {
        DBG_ERROR(("{WGL(IMG)} ERR: Template image's instance is NULL. (%d) at L%d\n\n", WGLR_INTERNAL_ERROR, __LINE__));
        _wgl_img_module_unlock();
		return WGLR_INTERNAL_ERROR;
    }

#ifndef MW_GL_NO_COMPRESSED_IMG
    *pui4_width = (_IS_IMG_TPL_COMPRESSED(pt_template))? (UINT32)pt_template->pt_image->ui2_width: pt_template->t_surface_info.ui4_width;
    *pui4_height = (_IS_IMG_TPL_COMPRESSED(pt_template))? (UINT32)pt_template->pt_image->ui2_lines: pt_template->t_surface_info.ui4_height;
#else
    *pui4_width = pt_template->t_surface_info.ui4_width;
    *pui4_height = pt_template->t_surface_info.ui4_height;
#endif
	_wgl_img_module_unlock();

    return WGLR_OK;
}


/*-----------------------------------------------------------------------------
 * private VIEW methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: _wgl_img_tpl_free_cb
 *
 * Description: This API delinks the specified handle from the data object. If
 *              this is the last handle attached to the object, the data object
 *              and the resources it holds get freed.
 *
 * Inputs:  h_handle        Contains the handle, which shall be freed.
 *          e_type          Contains the handle type.
 *          pv_obj          References a clipper object.
 *          pv_tag          Contains the handles private tag.
 *          b_req_handle    If this argument is set to TRUE then the free
 *                          operation is specifically requested on this handle.
 * Outputs: -
 *
 * Returns: TRUE    Routine succeeded.
 ----------------------------------------------------------------------------*/
static BOOL _wgl_img_tpl_free_cb(
    HANDLE_T                    h_handle,
    HANDLE_TYPE_T               e_type,
    VOID*                       pv_obj,
    VOID*                       pv_tag,
    BOOL                        b_req_handle)
{
    WGL_IMG_TPL_T*              pt_template;
    HANDLE_T                    h_surface = NULL_HANDLE;
    WGL_ASSERT(HT_WGL_IMG_TPL == e_type);

    pt_template = (WGL_IMG_TPL_T*)pv_obj;



    if (_wgl_image_cache_find((VOID *) pt_template->pt_image, &h_surface, FALSE))
    {
        _wgl_image_cache_delete((VOID *)pt_template->pt_image);
    }
    else if (pt_template->h_surface != NULL_HANDLE)
    {
        x_gl_obj_free(pt_template->h_surface);
    }
#ifdef CLI_LVL_ALL
    _wgl_img_remove_from_list(pt_template);
#endif
    if (pt_template->h_img != NULL_HANDLE)
    {
        if (decode_funtion_table.pf_close == NULL)
        {
            x_dbg_stmt("[WGL][IMAGE] Decode funtion not init \r\n");
        }
        else
        {
            decode_funtion_table.pf_close(pt_template->h_img);
        }
    }

    WGL_MEM_FREE(pv_obj);
    return TRUE;
}


/*******************************************************************************
 * Name
 *          _wgl_img_tpl_HO_paint
 * Description
 *          No illegal parameter checking
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
/*
 For L to R:
 +--------------+--------------+--------------+
 |   segment 1  |   segment 2  |   segment 3  |
 +--------------+--------------+--------------+
 For R to L:
 +--------------+--------------+--------------+
 |   segment 3  |   segment 2  |   segment 1  |
 +--------------+--------------+--------------+
*/

static INT32 _wgl_img_tpl_HO_paint(
    WGL_IMG_TPL_T*                 pt_template,
    const GL_RECT_T*               pt_output_rect,
    const GL_RECT_T*               pt_invalid_rect,
    GL_HGRAPHICS_T                 h_graphics,
    const WGL_CANVAS_STATUS_T*     pt_cvsst,
    WGL_TPL_PAINT_METHOD_T         e_paint_method,
    WGL_TPL_PAINT_ALGSM_T          e_algorism)
{
    /*locak varaible declaration*/
    GL_RECT_T                   t_intersect_rect = {0};
    UINT32                      ui4_seg_w, ui4_seg_h;
    UINT32                      ui4_output_w, ui4_left_w;
    UINT32                      ui4_center_rep;
    UINT32                      ui4_temp;
    INT32                       i4_ret;
    INT32                       i4_start_x, i4_end_x;
    INT32                       i4_blk_start_x, i4_blk_end_x, i4_blk_size, i4_src_start_x;
    BOOL                        b_draw_all = FALSE, b_draw_seg_1 = FALSE, b_draw_seg_3 = FALSE;
    BOOL                        b_is_overlap, b_is_l2r;

    /* no support for rendering compressed images in _wgl_img_tpl_HO_paint() */
    if (_IS_IMG_TPL_COMPRESSED(pt_template) ||
        _IS_IMG_TPL_PNG(pt_template)||
        _IS_IMG_TPL_JPG(pt_template)||
        _IS_IMG_TPL_BMP(pt_template))
    {
        return WGLR_NOT_SUPPORT_FORMAT;
    }

    /*calcuate segment width, height, and repeat times*/
    ui4_output_w = (UINT32)WGL_RECT_GET_WIDTH(pt_output_rect);

    ui4_seg_w = (pt_template->t_surface_info.ui4_width + 2) / 3;
    ui4_seg_h = pt_template->t_surface_info.ui4_height;
    /*check block's width and height*/
    if(0 == ui4_seg_w || 0 == ui4_seg_h)
    {
        return WGLR_OK;
    }

    ui4_temp = (ui4_output_w + ui4_seg_w - 1) / ui4_seg_w;
    ui4_center_rep = (ui4_temp > 2) ? ui4_temp - 2 : 0;

    if(pt_invalid_rect)
    {
        /*get intersect rectangle*/
        i4_ret = x_wgl_rect_intersect(&t_intersect_rect, pt_output_rect, pt_invalid_rect);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }

        /*check the intersect rectangle*/
        if(WGL_RECT_IS_EMPTY(&t_intersect_rect))
        {
            return WGLR_OK;
        }

        i4_start_x = ((t_intersect_rect.i4_left - pt_output_rect->i4_left) / ui4_seg_w) * ui4_seg_w + pt_output_rect->i4_left;
        i4_end_x = t_intersect_rect.i4_right;
    }
    else
    {
        b_draw_all = TRUE;
        i4_start_x = pt_output_rect->i4_left;
        i4_end_x = pt_output_rect->i4_right;
    }

    /*Because HO doesn't care about vertical, ignore T <--> B*/
    b_is_overlap = WGL_TPL_IS_OVERLAP(e_algorism);
    b_is_l2r = WGL_TPL_IS_L2R(e_algorism);

    ui4_left_w = (ui4_output_w > ui4_seg_w) ? ui4_seg_w : ui4_output_w;
    if(i4_start_x < (pt_output_rect->i4_left + ui4_seg_w))
    {
        b_draw_seg_1 = TRUE;
    }
    if(i4_end_x >= (pt_output_rect->i4_right - ui4_seg_w))
    {
        b_draw_seg_3 = TRUE;
    }

    /*paint segment-1. L2R: left segment, R2L: right segment*/
    if(b_draw_all || b_draw_seg_1)
    {
        if(FALSE == b_is_l2r)
        {
            i4_blk_start_x = pt_output_rect->i4_right - ui4_left_w;
            i4_src_start_x = (INT32)(pt_template->t_surface_info.ui4_width - ui4_left_w);
        }
        else
        {
            i4_blk_start_x = pt_output_rect->i4_left;
            i4_src_start_x = 0;
        }
        if(GLR_OK != _WGL_INVOKE_PATTERN_PAINT(h_graphics,
                                 pt_cvsst,
                                 pt_template->h_surface,
                                 i4_blk_start_x,
                                 pt_output_rect->i4_top,
                                 ui4_left_w,
                                 ui4_seg_h,
                                 i4_src_start_x,
                                 0,
                                 ui4_left_w,
                                 ui4_seg_h,
                                 255,
                                 e_paint_method))
        {
            return WGLR_INTERNAL_ERROR;
        }
    }

    /*paint center segment*/
    if(0 != ui4_center_rep)
    {
        /*the right boundary of the center segment should not overlap with the left boundary of the right segment.*/
        if(b_draw_all ||
           !((i4_start_x >= (pt_output_rect->i4_right - ui4_seg_w)) ||
             (i4_end_x < (pt_output_rect->i4_left + ui4_seg_w))))
        {
            i4_blk_start_x = pt_output_rect->i4_left + ui4_seg_w;
            i4_blk_start_x = (i4_start_x > i4_blk_start_x) ? i4_start_x : i4_blk_start_x;
            i4_blk_end_x = pt_output_rect->i4_right - ui4_seg_w;
            i4_blk_end_x = (i4_end_x < i4_blk_end_x) ? i4_end_x : i4_blk_end_x;

            if(i4_blk_end_x > i4_blk_start_x)
            {
                if(GLR_OK != _WGL_INVOKE_PATTERN_PAINT(h_graphics,
                                         pt_cvsst,
                                         pt_template->h_surface,
                                         i4_blk_start_x,
                                         pt_output_rect->i4_top,
                                         i4_blk_end_x - i4_blk_start_x,                                      /*dest width*/
                                         ui4_seg_h,
                                         ui4_seg_w,
                                         0,
                                         ui4_seg_w,
                                         ui4_seg_h,
                                         255,
                                         e_paint_method))
                {
                    return WGLR_INTERNAL_ERROR;
                }
            }
        }
    }

    /*paint segment-3. L2R: right segment, R2L: left segment*/
    if(ui4_output_w > ui4_seg_w)
    {
        if(b_draw_all || b_draw_seg_3)
        {
            if(ui4_center_rep > 0 && FALSE == b_is_overlap)
            {
                b_is_overlap = TRUE;
            }

            if(b_is_overlap && b_is_l2r)
            {
                i4_blk_size = (INT32)ui4_seg_w;
                i4_blk_start_x = pt_output_rect->i4_right - ui4_seg_w;
                i4_src_start_x = (INT32)(pt_template->t_surface_info.ui4_width - ui4_seg_w);
            }
            else if(b_is_overlap && FALSE == b_is_l2r)
            {
                i4_blk_size = (INT32)ui4_seg_w;
                i4_blk_start_x = pt_output_rect->i4_left;
                i4_src_start_x = 0;
            }
            else if(FALSE == b_is_overlap && b_is_l2r)
            {
                i4_blk_start_x = pt_output_rect->i4_left + ui4_seg_w;
                i4_blk_size = pt_output_rect->i4_right - i4_blk_start_x;
                i4_src_start_x = (INT32)(ui4_seg_w + ui4_seg_w);
            }
            else /*FALSE == b_is_overlap && FALSE == b_is_l2r*/
            {
                i4_blk_start_x = pt_output_rect->i4_left;
                i4_blk_size = (pt_output_rect->i4_right - ui4_seg_w) - i4_blk_start_x;
                i4_src_start_x = (INT32)(ui4_seg_w - i4_blk_size);
            }

            if(GLR_OK != _WGL_INVOKE_PATTERN_PAINT(h_graphics,
                                     pt_cvsst,
                                     pt_template->h_surface,
                                     i4_blk_start_x,
                                     pt_output_rect->i4_top,
                                     i4_blk_size,
                                     ui4_seg_h,
                                     i4_src_start_x,
                                     0,
                                     i4_blk_size,
                                     ui4_seg_h,
                                     255,
                                     e_paint_method))
            {
                return WGLR_INTERNAL_ERROR;
            }
        }
    }
    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *          _wgl_img_tpl_VO_paint
 * Description
 *          No illegal parameter checking
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
/*
    +--------------+
    |   segment 1  |
    +--------------+
    |   segment 2  |
    +--------------+
    |   segment 3  |
    +--------------+
*/
static INT32 _wgl_img_tpl_VO_paint(
    WGL_IMG_TPL_T*                  pt_template,
    const GL_RECT_T*                pt_output_rect,
    const GL_RECT_T*                pt_invalid_rect,
    GL_HGRAPHICS_T                  h_graphics,
    const WGL_CANVAS_STATUS_T*      pt_cvsst,
    WGL_TPL_PAINT_METHOD_T          e_paint_method,
    WGL_TPL_PAINT_ALGSM_T           e_algorism)
{
    /*locak varaible declaration*/
    GL_RECT_T                       t_intersect_rect = {0};
    UINT32                          ui4_seg_w, ui4_seg_h;
    UINT32                          ui4_output_h, ui4_left_h;
    UINT32                          ui4_center_rep;
    UINT32                          ui4_temp;
    INT32                           i4_ret;
    INT32                           i4_start_y, i4_end_y;
    INT32                           i4_blk_start_y, i4_blk_end_y, i4_blk_size, i4_src_start_y;
    BOOL                            b_draw_all = FALSE, b_draw_seg_1 = FALSE, b_draw_seg_3 = FALSE;
    BOOL                            b_is_overlap, b_is_t2b;

    /* no support for rendering compressed images in _wgl_img_tpl_VO_paint() */
    if (_IS_IMG_TPL_COMPRESSED(pt_template) ||
            _IS_IMG_TPL_PNG(pt_template)||
            _IS_IMG_TPL_JPG(pt_template)||
            _IS_IMG_TPL_BMP(pt_template))
    {
        return WGLR_NOT_SUPPORT_FORMAT;
    }

    /*calcuate segment width, height, and repeat times*/
    ui4_output_h = (UINT32)WGL_RECT_GET_HEIGHT(pt_output_rect);
    ui4_seg_w = pt_template->t_surface_info.ui4_width;
    ui4_seg_h = (pt_template->t_surface_info.ui4_height + 2) / 3;

    /*check block's width and height*/
    if(0 == ui4_seg_w || 0 == ui4_seg_h)
    {
        return WGLR_OK;
    }

    ui4_temp = (ui4_output_h + ui4_seg_h - 1) / ui4_seg_h;
    ui4_center_rep = (ui4_temp > 2) ? ui4_temp - 2 : 0;


    if(pt_invalid_rect)
    {
        /*get intersect rectangle*/
        i4_ret = x_wgl_rect_intersect(&t_intersect_rect, pt_output_rect, pt_invalid_rect);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }

        /*check the intersect rectangle*/
        if(WGL_RECT_IS_EMPTY(&t_intersect_rect))
        {
            return WGLR_OK;
        }

        i4_start_y = ((t_intersect_rect.i4_top - pt_output_rect->i4_top) / ui4_seg_h) * ui4_seg_h + pt_output_rect->i4_top;
        i4_end_y = t_intersect_rect.i4_bottom;
    }
    else
    {
        b_draw_all = TRUE;
        i4_start_y = pt_output_rect->i4_top;
        i4_end_y = pt_output_rect->i4_bottom;
    }

    /*Because VO doesn't care about horizontal, ignore L <--> R*/
    b_is_overlap = WGL_TPL_IS_OVERLAP(e_algorism);
    b_is_t2b = WGL_TPL_IS_T2B(e_algorism);

    /**/
    ui4_left_h = (ui4_output_h > ui4_seg_h) ? ui4_seg_h : ui4_output_h;
    if(i4_start_y < (pt_output_rect->i4_top + ui4_seg_h))
    {
        if(b_is_t2b)
        {
            b_draw_seg_1 = TRUE;
        }
        else
        {
            b_draw_seg_3 = TRUE;
        }
    }
    if(i4_end_y >= (pt_output_rect->i4_bottom - ui4_seg_h))
    {
        if(b_is_t2b)
        {
            b_draw_seg_3 = TRUE;
        }
        else
        {
            b_draw_seg_1 = TRUE;
        }
    }

    /*paint top segment*/
    if(b_draw_all || b_draw_seg_1)
    {
        if(FALSE == b_is_t2b)
        {
            i4_blk_start_y = pt_output_rect->i4_bottom - ui4_left_h;
            i4_src_start_y = (INT32)(pt_template->t_surface_info.ui4_height - ui4_left_h);
        }
        else
        {
            i4_blk_start_y = pt_output_rect->i4_top;
            i4_src_start_y = 0;
        }

        if(GLR_OK != _WGL_INVOKE_PATTERN_PAINT(h_graphics,
                                 pt_cvsst,
                                 pt_template->h_surface,
                                 pt_output_rect->i4_left,
                                 i4_blk_start_y,
                                 ui4_seg_w,
                                 ui4_left_h,
                                 0,
                                 i4_src_start_y,
                                 ui4_seg_w,
                                 ui4_left_h,
                                 255,
                                 e_paint_method))
        {
            return WGLR_INTERNAL_ERROR;
        }
    }

    /*paint middle segment*/
    if(0 != ui4_center_rep)
    {
        /*the bottom boundary of the middle segment should not overlap with the top boundary of the bottom segment.*/
        if(b_draw_all ||
           !((i4_start_y >= (pt_output_rect->i4_bottom - ui4_seg_h)) ||
             (i4_end_y < (pt_output_rect->i4_top + ui4_seg_h))))
        {
            i4_blk_start_y = pt_output_rect->i4_top + ui4_seg_h;
            i4_blk_start_y = (i4_start_y > i4_blk_start_y) ? i4_start_y : i4_blk_start_y;
            i4_blk_end_y = pt_output_rect->i4_bottom - ui4_seg_h;
            i4_blk_end_y = (i4_end_y < i4_blk_end_y) ? i4_end_y : i4_blk_end_y;

            if(i4_blk_end_y > i4_blk_start_y)
            {
                if(GLR_OK != _WGL_INVOKE_PATTERN_PAINT(h_graphics,
                                         pt_cvsst,
                                         pt_template->h_surface,
                                         pt_output_rect->i4_left,
                                         i4_blk_start_y,
                                         ui4_seg_w,
                                         i4_blk_end_y - i4_blk_start_y,                                  /*dest height*/
                                         0,
                                         ui4_seg_h,
                                         ui4_seg_w,
                                         ui4_seg_h,
                                         255,
                                         e_paint_method))
                {
                    return WGLR_INTERNAL_ERROR;
                }
            }
        }
    }

    /*paint bottom segment*/
    if(ui4_output_h > ui4_seg_h)
    {
        if(b_draw_all || b_draw_seg_3)
        {
            if((ui4_center_rep > 0) && (FALSE == b_is_overlap))
            {
                b_is_overlap = TRUE;
            }

            if(b_is_overlap && b_is_t2b)
            {
                i4_blk_size = (INT32)ui4_seg_h;
                i4_blk_start_y = pt_output_rect->i4_bottom - ui4_seg_h;
                i4_src_start_y = (INT32)(pt_template->t_surface_info.ui4_height - ui4_seg_h);
            }
            else if(b_is_overlap && FALSE == b_is_t2b)
            {
                i4_blk_size = (INT32)ui4_seg_h;
                i4_blk_start_y = pt_output_rect->i4_top;
                i4_src_start_y = 0;
            }
            else if(FALSE == b_is_overlap && b_is_t2b)
            {
                i4_blk_start_y = (INT32)(pt_output_rect->i4_top + ui4_seg_h);
                i4_blk_size = pt_output_rect->i4_bottom - i4_blk_start_y;
                i4_src_start_y = (INT32)(ui4_seg_h + ui4_seg_h);
            }
            else /*FALSE == b_is_overlap && FALSE == b_is_t2b*/
            {
                i4_blk_start_y = pt_output_rect->i4_top;
                i4_blk_size = (pt_output_rect->i4_bottom - ui4_seg_h) - i4_blk_start_y;
                /*i4_blk_start_y = pt_output_rect->i4_bottom - ui4_seg_h;
                i4_blk_size = pt_output_rect->i4_top - i4_blk_start_y;*/
                i4_src_start_y = (INT32)ui4_seg_h - i4_blk_size;
            }

            if(GLR_OK != _WGL_INVOKE_PATTERN_PAINT(h_graphics,
                                     pt_cvsst,
                                     pt_template->h_surface,
                                     pt_output_rect->i4_left,
                                     i4_blk_start_y,
                                     ui4_seg_w,
                                     i4_blk_size,
                                     0,
                                     i4_src_start_y,
                                     ui4_seg_w,
                                     i4_blk_size,
                                     255,
                                     e_paint_method))
            {
                return WGLR_INTERNAL_ERROR;
            }
        }
    }

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *          _wgl_img_tpl_HV_paint
 * Description
 *          No illegal parameter checking
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
/*
    +--------------+--------------+--------------+
    |   segment 1  |   segment 4  |   segment 7  |
    +--------------+--------------+--------------+
    |   segment 2  |   segment 5  |   segment 8  |
    +--------------+--------------+--------------+
    |   segment 3  |   segment 6  |   segment 9  |
    +--------------+--------------+--------------+
*/
static INT32 _wgl_img_tpl_HV_paint(
    WGL_IMG_TPL_T*                 pt_template,
    const GL_RECT_T*               pt_output_rect,
    const GL_RECT_T*               pt_invalid_rect,
    GL_HGRAPHICS_T                 h_graphics,
    const WGL_CANVAS_STATUS_T*     pt_cvsst,
    WGL_TPL_PAINT_METHOD_T         e_paint_method,
    WGL_TPL_PAINT_ALGSM_T          e_algorism)
{
    /*locak varaible declaration*/
    GL_RECT_T                           t_intersect_rect = {0};
    UINT32                              ui4_seg_w, ui4_seg_h;
    UINT32                              ui4_output_w, ui4_output_h;
    UINT32                              ui4_h_rep, ui4_v_rep;
    UINT32                              ui4_left_w, ui4_left_h;
    UINT32                              ui4_temp;
    INT32                               i4_ret;
    INT32                               i4_start_x, i4_start_y, i4_end_x, i4_end_y;
    INT32                               i4_blk_start_x, i4_blk_start_y, i4_blk_end_x, i4_blk_end_y;
    INT32                               i4_right_dest_start = 0, i4_right_src_start = 0, i4_right_size = 0;
    INT32                               i4_seg_1_d_y = 0, i4_seg_1_s_y = 0;
    INT32                               i4_seg_3_d_y = 0, i4_seg_3_s_y = 0, i4_seg_3_h = 0;
    BOOL                                b_draw_all = FALSE;
    BOOL                                b_draw_seg_1 = FALSE, b_draw_seg_3 = FALSE;
    BOOL                                b_draw_seg_4 = FALSE, b_draw_seg_6 = FALSE;
    BOOL                                b_draw_seg_7 = FALSE, b_draw_seg_9 = FALSE;
    BOOL                                b_is_overlap, b_is_t2b;

    /* no support for rendering compressed images in _wgl_img_tpl_HV_paint() */
    if (_IS_IMG_TPL_COMPRESSED(pt_template) ||
            _IS_IMG_TPL_PNG(pt_template)||
            _IS_IMG_TPL_JPG(pt_template)||
            _IS_IMG_TPL_BMP(pt_template))
    {
        return WGLR_NOT_SUPPORT_FORMAT;
    }

    /*calcuate segment width, height, and repeat times*/
    ui4_output_w = (UINT32)WGL_RECT_GET_WIDTH(pt_output_rect);
    ui4_output_h = (UINT32)WGL_RECT_GET_HEIGHT(pt_output_rect);
    ui4_seg_w = (pt_template->t_surface_info.ui4_width + 2) / 3;
    ui4_seg_h = (pt_template->t_surface_info.ui4_height + 2) / 3;

    /*check block's width and height*/
    if(0 == ui4_seg_w || 0 == ui4_seg_h)
    {
        return WGLR_OK;
    }

    ui4_temp = (ui4_output_w + ui4_seg_w - 1) / ui4_seg_w;
    ui4_h_rep =  (ui4_temp > 2) ? ui4_temp - 2 : 0;
    ui4_temp = (ui4_output_h + ui4_seg_h - 1) / ui4_seg_h;
    ui4_v_rep =  (ui4_temp > 2) ? ui4_temp - 2 : 0;


    if(pt_invalid_rect)
    {
        /*get intersect rectangle*/
        i4_ret = x_wgl_rect_intersect(&t_intersect_rect, pt_output_rect, pt_invalid_rect);
        if(WGLR_OK != i4_ret)
        {
            return i4_ret;
        }

        /*check the intersect rectangle*/
        if(WGL_RECT_IS_EMPTY(&t_intersect_rect))
        {
            return WGLR_OK;
        }

        i4_start_x = ((t_intersect_rect.i4_left - pt_output_rect->i4_left) / ui4_seg_w) * ui4_seg_w + pt_output_rect->i4_left;
        i4_start_y = ((t_intersect_rect.i4_top - pt_output_rect->i4_top) / ui4_seg_h) * ui4_seg_h + pt_output_rect->i4_top;
        i4_end_x = t_intersect_rect.i4_right;
        i4_end_y = t_intersect_rect.i4_bottom;
    }
    else
    {
        b_draw_all = TRUE;
        i4_start_x = pt_output_rect->i4_left;
        i4_start_y = pt_output_rect->i4_top;
        i4_end_x = pt_output_rect->i4_right;
        i4_end_y = pt_output_rect->i4_bottom;
    }

    /*Now HV doesn't support L <--> R, so ignore it*/
    b_is_overlap = WGL_TPL_IS_OVERLAP(e_algorism);
    b_is_t2b = WGL_TPL_IS_T2B(e_algorism);

    ui4_left_w = (ui4_output_w > ui4_seg_w) ? ui4_seg_w : ui4_output_w;
    ui4_left_h = (ui4_output_h > ui4_seg_h) ? ui4_seg_h : ui4_output_h;

    /*width for image > output*/
    if(ui4_output_w > ui4_seg_w)
    {
        if(ui4_h_rep > 0 || b_is_overlap || FALSE == b_is_t2b)
        {
            i4_right_dest_start = pt_output_rect->i4_right - ui4_seg_w;
            i4_right_size = (INT32)ui4_seg_w;
            i4_right_src_start = (INT32)(pt_template->t_surface_info.ui4_width - ui4_seg_w);
        }
        else
        {
            i4_right_dest_start = (INT32)pt_output_rect->i4_left + ui4_seg_w;
            i4_right_size = pt_output_rect->i4_right - i4_right_dest_start;
            i4_right_src_start = (INT32)(ui4_seg_w + ui4_seg_w);
        }
    }

    /**/
    if(b_is_t2b)
    {
        if(i4_start_x < (pt_output_rect->i4_left + ui4_seg_w) && i4_start_y < (pt_output_rect->i4_top + ui4_seg_h))
        {
            b_draw_seg_1 = TRUE;
        }
        if(!(i4_start_x >= (pt_output_rect->i4_left + ui4_seg_w) || i4_end_y < (pt_output_rect->i4_bottom - ui4_seg_h)))
        {
            b_draw_seg_3 = TRUE;
        }
        if(i4_end_x >= (pt_output_rect->i4_left + ui4_seg_w) && i4_start_y < (pt_output_rect->i4_top + ui4_seg_h))
        {
            b_draw_seg_4 = TRUE;
        }
        if(!(i4_end_y < (pt_output_rect->i4_bottom - ui4_seg_h) ||
             i4_start_x >= (pt_output_rect->i4_right - ui4_seg_w) ||
             i4_end_x < (pt_output_rect->i4_left + ui4_seg_w)))
        {
            b_draw_seg_6 = TRUE;
        }
        if(i4_end_x >= (pt_output_rect->i4_right - ui4_seg_w) && i4_start_y < (pt_output_rect->i4_top + ui4_seg_h))
        {
            b_draw_seg_7 = TRUE;
        }
        if(!(i4_end_x < (pt_output_rect->i4_right - ui4_seg_w) || i4_end_y < (pt_output_rect->i4_bottom - ui4_seg_h)))
        {
            b_draw_seg_9 = TRUE;
        }

        if(b_draw_seg_1 || b_draw_seg_4 || b_draw_seg_7)
        {
            i4_seg_1_d_y = pt_output_rect->i4_top;
            i4_seg_1_s_y = 0;
        }
    }
    else
    {
        if(i4_start_x < (pt_output_rect->i4_left + ui4_seg_w) && i4_start_y < (pt_output_rect->i4_top + ui4_seg_h))
        {
            b_draw_seg_3 = TRUE;
        }
        if(!(i4_start_x >= (pt_output_rect->i4_left + ui4_seg_w) || i4_end_y < (pt_output_rect->i4_bottom - ui4_seg_h)))
        {
            b_draw_seg_1 = TRUE;
        }
        if(i4_end_x >= (pt_output_rect->i4_left + ui4_seg_w) && i4_start_y < (pt_output_rect->i4_top + ui4_seg_h))
        {
            b_draw_seg_6 = TRUE;
        }
        if(!(i4_end_y < (pt_output_rect->i4_bottom - ui4_seg_h) ||
             i4_start_x >= (pt_output_rect->i4_right - ui4_seg_w) ||
             i4_end_x < (pt_output_rect->i4_left + ui4_seg_w)))
        {
            b_draw_seg_4 = TRUE;
        }
        if(i4_end_x >= (pt_output_rect->i4_right - ui4_seg_w) && i4_start_y < (pt_output_rect->i4_top + ui4_seg_h))
        {
            b_draw_seg_9 = TRUE;
        }
        if(!(i4_end_x < (pt_output_rect->i4_right - ui4_seg_w) || i4_end_y < (pt_output_rect->i4_bottom - ui4_seg_h)))
        {
            b_draw_seg_7 = TRUE;
        }

        if(b_draw_seg_1 || b_draw_seg_4 || b_draw_seg_7)
        {
            i4_seg_1_d_y = pt_output_rect->i4_bottom - ui4_left_h;
            i4_seg_1_s_y = (INT32)(pt_template->t_surface_info.ui4_height - ui4_left_h);
        }
    }

    /*height for image > output*/
    if((ui4_output_h > ui4_seg_h) && (b_draw_seg_3 || b_draw_seg_6 || b_draw_seg_9))
    {
        if((ui4_v_rep > 0) && (FALSE == b_is_overlap))
        {
            b_is_overlap = TRUE;
        }

        if(b_is_overlap && b_is_t2b)
        {
            i4_seg_3_h = (INT32)ui4_seg_h;
            i4_seg_3_d_y = pt_output_rect->i4_bottom - ui4_seg_h;
            i4_seg_3_s_y = (INT32)(pt_template->t_surface_info.ui4_height - ui4_seg_h);
        }
        else if(b_is_overlap && FALSE == b_is_t2b)
        {
            i4_seg_3_h = (INT32)ui4_seg_h;
            i4_seg_3_d_y = pt_output_rect->i4_top;
            i4_seg_3_s_y = 0;
        }
        else if(FALSE == b_is_overlap && b_is_t2b)
        {
            i4_seg_3_d_y = pt_output_rect->i4_top + ui4_seg_h;
            i4_seg_3_h = pt_output_rect->i4_bottom - i4_seg_3_d_y;
            i4_seg_3_s_y = (INT32)(ui4_seg_h + ui4_seg_h);
        }
        else /*FALSE == b_is_overlap && FALSE == b_is_t2b*/
        {
            i4_seg_3_d_y = pt_output_rect->i4_bottom - ui4_seg_h;
            i4_seg_3_h = pt_output_rect->i4_top - i4_seg_3_d_y;
            i4_seg_3_s_y = (INT32)ui4_seg_h - i4_seg_3_h;
        }
    }

    /*paint left-top segment*/
    if(b_draw_all || b_draw_seg_1)
    {
        if(GLR_OK != _WGL_INVOKE_PATTERN_PAINT(h_graphics,
                                 pt_cvsst,
                                 pt_template->h_surface,
                                 pt_output_rect->i4_left,
                                 i4_seg_1_d_y,
                                 ui4_left_w,
                                 ui4_left_h,
                                 0,
                                 i4_seg_1_s_y,
                                 ui4_seg_w,
                                 ui4_left_h,
                                 255,
                                 e_paint_method))
        {
            return WGLR_INTERNAL_ERROR;
        }
    }

    /*paint left-center segment*/
    if(0 != ui4_v_rep)
    {
        if(b_draw_all ||
           !((i4_start_x >= (pt_output_rect->i4_left + ui4_seg_w)) ||
             (i4_start_y >= (pt_output_rect->i4_bottom - ui4_seg_h)) ||
             (i4_end_y < (pt_output_rect->i4_top + ui4_seg_h))))
        {
            i4_blk_start_y = pt_output_rect->i4_top + ui4_seg_h;
            i4_blk_start_y = (i4_start_y > i4_blk_start_y) ? i4_start_y : i4_blk_start_y;
            i4_blk_end_y = pt_output_rect->i4_bottom - ui4_seg_h;
            i4_blk_end_y = (i4_end_y < i4_blk_end_y) ? i4_end_y : i4_blk_end_y;

            if(i4_blk_end_y > i4_blk_start_y)
            {
                if(GLR_OK != _WGL_INVOKE_PATTERN_PAINT(h_graphics,
                                         pt_cvsst,
                                         pt_template->h_surface,
                                         pt_output_rect->i4_left,
                                         i4_blk_start_y,
                                         ui4_left_w,
                                         i4_blk_end_y - i4_blk_start_y,                            /*dest height*/
                                         0,
                                         ui4_seg_h,
                                         ui4_seg_w,
                                         ui4_seg_h,
                                         255,
                                         e_paint_method))
                {
                    return WGLR_INTERNAL_ERROR;
                }
            }
        }
    }

    /*paint left-bottom segment*/
    if(ui4_output_h > ui4_seg_h)
    {
        if(b_draw_all || b_draw_seg_3)
        {
            if(GLR_OK != _WGL_INVOKE_PATTERN_PAINT(h_graphics,
                                     pt_cvsst,
                                     pt_template->h_surface,
                                     pt_output_rect->i4_left,
                                     i4_seg_3_d_y,
                                     ui4_left_w,
                                     i4_seg_3_h,
                                     0,
                                     i4_seg_3_s_y,
                                     ui4_seg_w,
                                     i4_seg_3_h,
                                     255,
                                     e_paint_method))
            {
                return WGLR_INTERNAL_ERROR;
            }
        }
    }

    /*paint center-top segment*/
    if(0 != ui4_h_rep)
    {
        if(b_draw_all || b_draw_seg_4)
        {
            i4_blk_start_x = pt_output_rect->i4_left + ui4_seg_w;
            i4_blk_start_x = (i4_start_x > i4_blk_start_x) ? i4_start_x : i4_blk_start_x;
            i4_blk_end_x = pt_output_rect->i4_right - ui4_seg_w;
            i4_blk_end_x = (i4_end_x < i4_blk_end_x) ? i4_end_x : i4_blk_end_x;

            if(i4_blk_end_x > i4_blk_start_x)
            {
                if(GLR_OK != _WGL_INVOKE_PATTERN_PAINT(h_graphics,
                                         pt_cvsst,
                                         pt_template->h_surface,
                                         i4_blk_start_x,
                                         i4_seg_1_d_y,
                                         i4_blk_end_x - i4_blk_start_x,                            /*dest width*/
                                         ui4_left_h,
                                         ui4_seg_w,
                                         i4_seg_1_s_y,
                                         ui4_seg_w,
                                         ui4_left_h,
                                         255,
                                         e_paint_method))
                {
                    return WGLR_INTERNAL_ERROR;
                }
            }
        }
    }

    /*paint center-center segment*/
    if(0 != ui4_h_rep && 0 != ui4_v_rep)
    {
        if(b_draw_all ||
           !((i4_start_x >= (pt_output_rect->i4_right - ui4_seg_w)) ||
             (i4_start_y >= (pt_output_rect->i4_bottom - ui4_seg_h)) ||
             (i4_end_x < (pt_output_rect->i4_left + ui4_seg_w)) ||
             (i4_end_y < (pt_output_rect->i4_top + ui4_seg_h))))
        {
            i4_blk_start_x = pt_output_rect->i4_left + ui4_seg_w;
            i4_blk_start_x = (i4_start_x > i4_blk_start_x) ? i4_start_x : i4_blk_start_x;
            i4_blk_end_x = pt_output_rect->i4_right - ui4_seg_w;
            i4_blk_end_x = (i4_end_x < i4_blk_end_x) ? i4_end_x : i4_blk_end_x;
            i4_blk_start_y = pt_output_rect->i4_top + ui4_seg_h;
            i4_blk_start_y = (i4_start_y > i4_blk_start_y) ? i4_start_y : i4_blk_start_y;
            i4_blk_end_y = pt_output_rect->i4_bottom - ui4_seg_h;
            i4_blk_end_y = (i4_end_y < i4_blk_end_y) ? i4_end_y : i4_blk_end_y;

            if((i4_blk_end_x > i4_blk_start_x) && (i4_blk_end_y > i4_blk_start_y))
            {
                if(GLR_OK != _WGL_INVOKE_PATTERN_PAINT(h_graphics,
                                         pt_cvsst,
                                         pt_template->h_surface,
                                         i4_blk_start_x,
                                         i4_blk_start_y,
                                         i4_blk_end_x - i4_blk_start_x,        /*dest width*/
                                         i4_blk_end_y - i4_blk_start_y,        /*dest height*/
                                         ui4_seg_w,
                                         ui4_seg_h,
                                         ui4_seg_w,
                                         ui4_seg_h,
                                         255,
                                         e_paint_method))
                {
                    return WGLR_INTERNAL_ERROR;
                }
            }
        }
    }

    /*paint center-bottom segment*/
    if(ui4_output_h > ui4_seg_h)
    {
        if(0 != ui4_h_rep)
        {
            if(b_draw_all || b_draw_seg_6)
            {
                i4_blk_start_x = pt_output_rect->i4_left + ui4_seg_w;
                i4_blk_start_x = (i4_start_x > i4_blk_start_x) ? i4_start_x : i4_blk_start_x;
                i4_blk_end_x = pt_output_rect->i4_right - ui4_seg_w;
                i4_blk_end_x = (i4_end_x < i4_blk_end_x) ? i4_end_x : i4_blk_end_x;

                if(i4_blk_end_x > i4_blk_start_x)
                {
                    if(GLR_OK != _WGL_INVOKE_PATTERN_PAINT(h_graphics,
                                             pt_cvsst,
                                             pt_template->h_surface,
                                             i4_blk_start_x,
                                             i4_seg_3_d_y,
                                             i4_blk_end_x - i4_blk_start_x,                            /*dest width*/
                                             i4_seg_3_h,
                                             ui4_seg_w,
                                             i4_seg_3_s_y,
                                             ui4_seg_w,
                                             i4_seg_3_h,
                                             255,
                                             e_paint_method))
                    {
                        return WGLR_INTERNAL_ERROR;
                    }
                }
            }
        }
    }

    if(ui4_output_w > ui4_seg_w)
    {
        /*paint right-top segment*/
        if(b_draw_all || b_draw_seg_7)
        {
            if(GLR_OK != _WGL_INVOKE_PATTERN_PAINT(h_graphics,
                                                   pt_cvsst,
                                                   pt_template->h_surface,
                                                   i4_right_dest_start,
                                                   i4_seg_1_d_y,
                                                   i4_right_size,
                                                   ui4_left_h,
                                                   i4_right_src_start,
                                                   i4_seg_1_s_y,
                                                   i4_right_size,
                                                   ui4_left_h,
                                                   255, e_paint_method))
            {
                return WGLR_INTERNAL_ERROR;
            }
        }

        /*paint right-center segment*/
        if(0 != ui4_v_rep)
        {
            if(b_draw_all ||
               !((i4_end_x < (pt_output_rect->i4_right - ui4_seg_w)) ||
                 (i4_start_y >= (pt_output_rect->i4_bottom - ui4_seg_h)) ||
                 (i4_end_y < (pt_output_rect->i4_top + ui4_seg_h))))
            {
                i4_blk_start_y = pt_output_rect->i4_top + ui4_seg_h;
                i4_blk_start_y = (i4_start_y > i4_blk_start_y) ? i4_start_y : i4_blk_start_y;
                i4_blk_end_y = pt_output_rect->i4_bottom - ui4_seg_h;
                i4_blk_end_y = (i4_end_y < i4_blk_end_y) ? i4_end_y : i4_blk_end_y;

                if(i4_blk_end_y > i4_blk_start_y)
                {
                    if(GLR_OK != _WGL_INVOKE_PATTERN_PAINT(h_graphics,
                                                           pt_cvsst,
                                                           pt_template->h_surface,
                                                           i4_right_dest_start,
                                                           i4_blk_start_y,
                                                           i4_right_size,
                                                           i4_blk_end_y - i4_blk_start_y,                           /*dest height*/
                                                           i4_right_src_start,
                                                           ui4_seg_h,
                                                           i4_right_size,
                                                           ui4_seg_h,
                                                           255,
                                                           e_paint_method))
                    {
                        return WGLR_INTERNAL_ERROR;
                    }
                }
            }
        }

        /*paint right-bottom segment*/
        if(ui4_output_h > ui4_seg_h)
        {
            if(b_draw_all || b_draw_seg_9)
            {
                if(GLR_OK != _WGL_INVOKE_PATTERN_PAINT(h_graphics,
                                                       pt_cvsst,
                                                       pt_template->h_surface,
                                                       i4_right_dest_start,
                                                       i4_seg_3_d_y,
                                                       i4_right_size,
                                                       i4_seg_3_h,
                                                       i4_right_src_start,
                                                       i4_seg_3_s_y,
                                                       i4_right_size,
                                                       i4_seg_3_h,
                                                       255,
                                                       e_paint_method))
                {
                    return WGLR_INTERNAL_ERROR;
                }
            }
        }
    }

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *          _wgl_img_tpl_NHV_paint
 * Description
 *          No illegal parameter checking
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_img_tpl_NHV_paint(
    WGL_IMG_TPL_T*                      pt_template,
    const GL_RECT_T*                    pt_output_rect,
    const GL_RECT_T*                    pt_invalid_rect,
    GL_HGRAPHICS_T                      h_graphics,
    const WGL_CANVAS_STATUS_T*          pt_cvsst,
    WGL_TPL_PAINT_METHOD_T              e_paint_method)
{
    INT32        i4_width, i4_height;
    INT32        i4_ret;
    GL_RECT_T    t_work_rect;

    i4_width = WGL_RECT_GET_WIDTH(pt_output_rect);
    i4_height = WGL_RECT_GET_HEIGHT(pt_output_rect);

    if (_IS_IMG_TPL_COMPRESSED(pt_template))
    {
    #ifndef MW_GL_NO_COMPRESSED_IMG
        i4_width = (i4_width > (INT32)pt_template->pt_image->ui2_width)? (INT32)pt_template->pt_image->ui2_width: i4_width;
        i4_height = (i4_height > (INT32)pt_template->pt_image->ui2_lines)? (INT32)pt_template->pt_image->ui2_lines: i4_height;
    #endif
    }
    else
    {
        i4_width = ((UINT32)i4_width > pt_template->t_surface_info.ui4_width) ? (INT32)pt_template->t_surface_info.ui4_width : i4_width;
        i4_height = ((UINT32)i4_height > pt_template->t_surface_info.ui4_height) ? (INT32)pt_template->t_surface_info.ui4_height : i4_height;
    }

    /* check block's width and height */
    if(0 >= i4_width || 0 >= i4_height)
    {
        return WGLR_OK;
    }

    SET_RECT_BY_SIZE(&t_work_rect, pt_output_rect->i4_left, pt_output_rect->i4_top, i4_width, i4_height);

    i4_ret = (!_IS_IMG_TPL_COMPRESSED(pt_template))?
                _WGL_INVOKE_PATTERN_PAINT(
                    h_graphics,
                    pt_cvsst,
                    pt_template->h_surface,
                    pt_output_rect->i4_left,
                    pt_output_rect->i4_top,
                    i4_width,
                    i4_height,
                    0,
                    0,
                    i4_width,
                    i4_height,
                    255,
                    e_paint_method):
                wgl_sw_nhv_pattern_paint(
                    h_graphics,
                    pt_cvsst,
                    pt_template,
                    &t_work_rect);

    if(GLR_OK != i4_ret)
    {
        return WGLR_INTERNAL_ERROR;
    }

    return WGLR_OK;
}

/*******************************************************************************
 * Name
 *          wgl_sw_nhv_pattern_paint
 * Description
 *          software-version NHV pattern paint
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_sw_nhv_pattern_paint(
    GL_HGRAPHICS_T              h_graphics,
    const WGL_CANVAS_STATUS_T   *pt_cvsst,
    WGL_IMG_TPL_T               *pt_template,
    GL_RECT_T                   *pt_work_rect)
{
#ifndef MW_GL_NO_COMPRESSED_IMG
    GL_COLOR_T          t_colorkey;
    GL_COLORMODE_T      e_colormode;
    INT32               i4_ret;

    if (pt_cvsst != NULL)
    {
        t_colorkey  = pt_cvsst->t_colorkey;
        e_colormode = pt_cvsst->e_clr_mode;
    }
    else
    {
        return WGLR_INV_ARG;
    }

    if (pt_cvsst->e_clr_mode != GL_COLORMODE_ARGB_D1555 &&
        pt_cvsst->e_clr_mode != GL_COLORMODE_ARGB_D4444 &&
        pt_cvsst->e_clr_mode != GL_COLORMODE_ARGB_D8888 &&
        pt_cvsst->e_clr_mode != GL_COLORMODE_ARGB_CLUT8 &&
        pt_cvsst->e_clr_mode != GL_COLORMODE_AYUV_CLUT8)
    {
        /* since the color mode between the image and the surface must be identical, only three mdoes are supoorted */
        return WGLR_NOT_SUPPORT_FORMAT;
    }

    if (pt_template == NULL || h_graphics == NULL_HANDLE || pt_cvsst == NULL)
    {
        return WGLR_INV_ARG;
    }

    if (!wgl_using_compressed_compose())
    {
        i4_ret = x_gl_sw_nhv_pattern_paint(
            h_graphics,
            e_colormode,
            &t_colorkey,
            pt_template->pt_image->pui1_image_data,
            pt_work_rect);
    }
    else
    {
        i4_ret = x_gl_sw_nhv_pattern_paint_compose(
            h_graphics,
            e_colormode,
            &t_colorkey,
            pt_template->pt_image->pui1_image_data,
            pt_work_rect);
    }

    return i4_ret;
#else
    return WGLR_NOT_SUPPORT_FORMAT;
#endif
}

/*******************************************************************************
 * Name
 *          _wgl_img_get_color_mode
 * Description
 *          Get surface color mode. No illegal parameter checking
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 _wgl_img_get_color_mode(
    UINT32                          ui4_image_type,
    GL_COLORMODE_T*                 pe_color_mode)
{
    switch (ui4_image_type & WGL_IMG_TYPE_MASK)
    {
    case WGL_IMG_TYPE_1555:
    case WGL_IMG_TYPE_1555_ALIGN:
    case WGL_IMG_TYPE_1555_ALIGN_ZIPPED:
    case WGL_IMG_TYPE_1555_ALIGN_COMPRESSED:
        *pe_color_mode = GL_COLORMODE_ARGB_D1555;
        return WGLR_OK;

    case WGL_IMG_TYPE_4444:
    case WGL_IMG_TYPE_4444_ALIGN:
    case WGL_IMG_TYPE_4444_ALIGN_ZIPPED:
    case WGL_IMG_TYPE_4444_ALIGN_COMPRESSED:
        *pe_color_mode = GL_COLORMODE_ARGB_D4444;
        return WGLR_OK;

    case WGL_IMG_TYPE_CLUT8:
    case WGL_IMG_TYPE_CLUT8_ALIGN:
    case WGL_IMG_TYPE_CLUT8_ALIGN_ZIPPED:
    case WGL_IMG_TYPE_CLUT8_ALIGN_COMPRESSED:
        *pe_color_mode = GL_COLORMODE_ARGB_CLUT8;
        return WGLR_OK;

    case WGL_IMG_TYPE_8888:
    case WGL_IMG_TYPE_8888_ALIGN:
    case WGL_IMG_TYPE_8888_ALIGN_ZIPPED:
    case WGL_IMG_TYPE_8888_ALIGN_COMPRESSED:
        *pe_color_mode = GL_COLORMODE_ARGB_D8888;
        return WGLR_OK;

    case WGL_IMG_TYPE_YUV_CLUT8:
    case WGL_IMG_TYPE_YUV_CLUT8_ALIGN:
    case WGL_IMG_TYPE_YUV_CLUT8_ALIGN_ZIPPED:
    case WGL_IMG_TYPE_YUV_CLUT8_ALIGN_COMPRESSED:
        *pe_color_mode = GL_COLORMODE_AYUV_CLUT8;
        return WGLR_OK;
    }
    switch (ui4_image_type)
    {
    case WGL_IMG_TYPE_JPG:
    case WGL_IMG_TYPE_PNG:
    case WGL_IMG_TYPE_BMP:
    case WGL_IMG_TYPE_GIF:
        *pe_color_mode = GL_COLORMODE_ARGB_D8888;
        break;
    case WGL_IMG_TYPE_JPG_4444:
    case WGL_IMG_TYPE_PNG_4444:
    case WGL_IMG_TYPE_BMP_4444:
    case WGL_IMG_TYPE_GIF_4444:
        *pe_color_mode = GL_COLORMODE_ARGB_D4444;
        break;
    default:
        return WGLR_NOT_SUPPORT_FORMAT;
    }

    return WGLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _wgl_img_tpl_png_decode_cb
 *
 * Description: This API get the notify of png image decode status.
 *
 * Inputs:  HANDLE_T h_handle,    Specifies which handle will be notified.
 *                   const VOID*             pv_tag,      The client data specified by the client when callback has been called, the callback function will send it again..
 *                   IMG_NFY_EVENT_TYPE_T    e_event,     it identifies the event type of notification.
 *                   const VOID*             pv_data);    specify the notification data which is followed with the notification event.
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
static VOID _wgl_img_tpl_jpg_decode_cb(
     HANDLE_T                h_handle,
    const VOID*             pv_tag,
    IMG_NFY_EVENT_TYPE_T    e_event,
    const VOID*             pv_data)
{
    if (NULL_HANDLE == h_handle)
    {
        DBG_ERROR(("{WGL(IMG)} %s: argument h_handle is NULL!\r\n", __FUNCTION__));
        return;
    }

    /* check if the decode producedure is error and the callback should be discurded*/
    if (!_gfg_image_decode_statuc)
    {
        DBG_ERROR(("{WGL(IMG)} %s: decode procedure is error and callback should be discurded!\r\n", __FUNCTION__));
        return;
    }

    switch (e_event)
    {
        case IMG_NFY_LOAD_COMPLETE_TYPE:
        {
            UINT16 ui2_i = 0;
            WGL_IMG_TPL_JPG_DECODE_STATUS_T** ppt_status =
                (WGL_IMG_TPL_JPG_DECODE_STATUS_T**)pv_tag;
            WGL_IMG_TPL_JPG_DECODE_STATUS_T* pt_status = *ppt_status;

            for (ui2_i = 0; ui2_i < pt_status->ui2_nbs; ui2_i++)
            {
                if (pt_status->ph_img[ui2_i] == h_handle)
                {
                    break;
                }
            }
            if (ui2_i < pt_status->ui2_nbs)
            {
                pt_status->pfg_status[ui2_i] = TRUE;
            }
            if (_wgl_img_check_status_data((const VOID**)pv_tag))
            {
                _wgl_img_decode_unlock();
            }
            break;
        }
        default:
            break;
    }
}

static INT32 _wgl_img_decode_lock(VOID)
{
    return (x_sema_lock(_gh_image_decode_lock, X_SEMA_OPTION_WAIT) == OSR_OK ? WGLR_OK : WGLR_OS_ERROR);
}

static INT32 _wgl_img_decode_unlock(void)
{
     return (x_sema_unlock(_gh_image_decode_lock) == OSR_OK ? WGLR_OK : WGLR_OS_ERROR);
}

static BOOL _wgl_img_check_status_data(const VOID** ppv_tag)
{
    UINT32 ui2_nbs = 0;
    /*check the input argument */
    if (NULL == ppv_tag || NULL == *ppv_tag)
    {
        return FALSE;
    }

    const WGL_IMG_TPL_JPG_DECODE_STATUS_T* pt_status =
        (const WGL_IMG_TPL_JPG_DECODE_STATUS_T*)*ppv_tag;

    if ((ui2_nbs = pt_status->ui2_nbs) == 0)
    {
        return FALSE;
    }
    while (ui2_nbs != 0)
    {
        if (pt_status->pfg_status[--ui2_nbs] == FALSE)
        {
            return FALSE;
        }
    }
    return TRUE;
}

static VOID _wgl_img_free_status_data(VOID** ppv_tag)
{
    if (NULL != ppv_tag && NULL != *ppv_tag)
    {
        WGL_IMG_TPL_JPG_DECODE_STATUS_T* pt_status =
            (WGL_IMG_TPL_JPG_DECODE_STATUS_T*)*ppv_tag;
        if (NULL != pt_status->pfg_status)
        {
            WGL_MEM_FREE(pt_status->pfg_status);
        }
        if (NULL != pt_status->ph_img)
        {
            WGL_MEM_FREE(pt_status->ph_img);
        }
        WGL_MEM_FREE(*ppv_tag);
        *ppv_tag = NULL;
    }
}

static VOID _wgl_img_alloc_status_data(VOID** ppv_tag, UINT16 ui2_nbs)
{
    WGL_IMG_TPL_JPG_DECODE_STATUS_T* pt_status = NULL;

    /*init the png status data*/
    pt_status = (WGL_IMG_TPL_JPG_DECODE_STATUS_T*)
        WGL_MEM_ALLOC(sizeof(WGL_IMG_TPL_JPG_DECODE_STATUS_T));
    if (NULL == pt_status)
    {
        *ppv_tag = NULL;
        return;
    }
    x_memset(pt_status, 0, sizeof(WGL_IMG_TPL_JPG_DECODE_STATUS_T));

    pt_status->ui2_nbs = ui2_nbs;

    pt_status->pfg_status = (BOOL*)WGL_MEM_ALLOC(pt_status->ui2_nbs * sizeof(BOOL));
    if (NULL == pt_status->pfg_status)
    {
        WGL_MEM_FREE(pt_status);
        *ppv_tag = NULL;
        return;
    }
    x_memset(pt_status->pfg_status, 0, pt_status->ui2_nbs * sizeof(BOOL));

    pt_status->ph_img = (HANDLE_T*)WGL_MEM_ALLOC(pt_status->ui2_nbs * sizeof(HANDLE_T));
    if (NULL == pt_status->ph_img)
    {
        WGL_MEM_FREE(pt_status->pfg_status);
        WGL_MEM_FREE(pt_status);
        *ppv_tag = NULL;
        return;
    }
    x_memset(pt_status->ph_img, 0, pt_status->ui2_nbs * sizeof(HANDLE_T));

    *ppv_tag = pt_status;
}


/*******************************************************************************
 * Name
 *          wgl_img_tpl_image_data_clone
 * Description
 *          this static function won't check pt_clone_fct_tbl
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_img_tpl_image_data_clone(
    const WGL_IMG_TPL_CLONE_FCT_TABLE_T*    pt_clone_fct_tbl,
    WGL_HIMG_TPL_T                          h_img_tpl,
    WGL_IMG_ST_T*                           pe_type,
    GL_HSURFACE_T*                          ph_surface)
{
    WGL_IMG_TPL_T*      pt_template = NULL;
    HANDLE_TYPE_T       e_type;
    GL_SURFACE_DESC_T   t_surf_dest = {0};
    GL_HSURFACE_T       h_surf_dest;
    INT32               i4_ret = WGLR_OK, i4_temp;
#ifndef MW_GL_NO_COMPRESSED_IMG
    GL_LOCK_INFO_T      t_surf_lock_info;
    UINT8               *pui1_line_start, *pui1_img_line_start;
    UINT32              ui4_img_line_bytes, ui4_cur_line;
    INT32               i4_diff;
    HANDLE_T            h_zip_eng, h_zip_state;
    UINT8               *pui1_line_buffer = NULL;
    SIZE_T              z_decomp_len;
#endif

    /*check parameter and get template image info*/
    if(NULL_HANDLE == h_img_tpl ||
       NULL == pe_type ||
       NULL == ph_surface||
       HR_OK != handle_get_type_obj((HANDLE_T)h_img_tpl, &e_type, (VOID**)&pt_template) ||
       HT_WGL_IMG_TPL != e_type)
    {
        return WGLR_INV_ARG;
    }
    if(NULL == pt_template)
    {
        return WGLR_INTERNAL_ERROR;
    }

    /*init return value*/
    *ph_surface = NULL_HANDLE;

    /* check if this image is compressed */
    if (_IS_IMG_TPL_COMPRESSED(pt_template))
    {
    #ifndef MW_GL_NO_COMPRESSED_IMG
        /*prepare surface description*/
        t_surf_dest.ui4_width = (UINT32)pt_template->pt_image->ui2_width;
        t_surf_dest.ui4_height = (UINT32)pt_template->pt_image->ui2_lines;
        _wgl_img_get_color_mode(pt_template->pt_image->ui4_image_type, &t_surf_dest.e_clrmode);
        t_surf_dest.h_palette = NULL_HANDLE;
        t_surf_dest.h_clipper = NULL_HANDLE;
    #endif
    }
    else
    {
        /*prepare surface description*/
        t_surf_dest.ui4_width = pt_template->t_surface_info.ui4_width;
        t_surf_dest.ui4_height = pt_template->t_surface_info.ui4_height;
        t_surf_dest.e_clrmode = pt_template->t_surface_info.e_clrmode;
        t_surf_dest.h_palette = NULL_HANDLE;
        t_surf_dest.h_clipper = NULL_HANDLE;
    }

    /*create surface*/
    if(GLR_OK != (pt_clone_fct_tbl->pf_surf_create)(&t_surf_dest, &h_surf_dest))
    {
        i4_ret = WGLR_SYS_RC_CREATE_FAIL;
        return i4_ret;
    }

    /* check if this image is compressed */
    if (_IS_IMG_TPL_COMPRESSED(pt_template))
    {
    #ifndef MW_GL_NO_COMPRESSED_IMG
        /* decompress and copy */
        do
        {
            /* Open the decompression engine */
            if ((i4_ret = x_cl_open_eng(SN_CL_ZIP, NULL, NULL, &h_zip_eng)) != CLR_OK)
            {
                DBG_ERROR(("x_cl_open_eng UZIP ERROR %d Line: %d\n", (int)i4_ret, __LINE__));
                i4_ret = WGLR_INTERNAL_ERROR;
                break;
            }

            /* Lock surface */
            if(GLR_OK != x_gl_surface_lock(h_surf_dest, NULL, GL_SYNC, &t_surf_lock_info))
            {
                i4_ret = WGLR_LOCK_ERROR;
                break;
            }

            /* Copy Image Data */
            /* surface start pos */
            pui1_line_start = t_surf_lock_info.at_subplane[0].pv_bits;

            /* bytes_per_line of image */
            ui4_img_line_bytes = (UINT32)GET_UINT32_FROM_PTR_LITTLE_END(pt_template->pt_image->pui1_image_data);

            /* allocate a line buffer for decompression */
            pui1_line_buffer    = (UINT8 *)WGL_MEM_ALLOC(ui4_img_line_bytes * sizeof(UINT8));
            pui1_img_line_start = pui1_line_buffer;
            z_decomp_len        = (SIZE_T)ui4_img_line_bytes;

            if (pui1_line_buffer == NULL)
            {
                DBG_ERROR(("{WGL} x_malloc() ERROR Line: %d\n", __LINE__));
                i4_ret = WGLR_OUT_OF_MEMORY;
                break;
            }

            /* difference between image and surface for each line */
            i4_diff = ((INT32)t_surf_lock_info.at_subplane[0].ui4_pitch) - ((INT32)ui4_img_line_bytes);
            /* WGL_ASSERT(i4_diff >= 0); */

            /* copy operation */
            for (ui4_cur_line = 0; ui4_cur_line < (UINT32)pt_template->pt_image->ui2_lines; ui4_cur_line++)
            {
                WGL_ASSERT(pui1_img_line_start < pt_template->pt_image->pui1_image_data + pt_template->pt_image->ui4_image_size);

                /* decompress one single line of the image and point it to pui1_img_line_start */
                x_cl_decompress(
                    h_zip_eng,
                    pt_template->pt_image->pui1_image_data + sizeof(INT32),
                    sizeof(pt_template->pt_image->pui1_image_data) - sizeof(INT32),
                    0,
                    CL_NULL_PARAM_TYPE,
                    NULL,
                    pui1_img_line_start,
                    &z_decomp_len,
                    &h_zip_state);

                if (i4_diff > 0)
                {
                    x_memcpy(pui1_line_start, pui1_img_line_start, ui4_img_line_bytes);
                    x_memset(pui1_line_start + ui4_img_line_bytes, 0, (SIZE_T)i4_diff);
                }
                else
                {
                    x_memcpy(pui1_line_start, pui1_img_line_start, t_surf_lock_info.at_subplane[0].ui4_pitch);
                }

                pui1_line_start += t_surf_lock_info.at_subplane[0].ui4_pitch;
            }

            /* Unlock surface */
            if(GLR_OK != x_gl_surface_unlock(h_surf_dest, NULL))
            {
                i4_ret = WGLR_UNLOCK_ERROR;
                break;
            }

            /* release the line buffer */
            WGL_MEM_FREE(pui1_line_buffer);

            /* close the decompression state */
            if ((i4_ret = x_cl_close(h_zip_state)) != CLR_OK)
            {
                DBG_ERROR(("x_cl_close ERROR %d Line: %d\n", (int)i4_ret, __LINE__));
                i4_ret = WGLR_INTERNAL_ERROR;
                break;
            }
            /* close the decompression engine */
            if ((i4_ret = x_cl_close(h_zip_eng)) != CLR_OK)
            {
                DBG_ERROR(("x_cl_close ERROR %d Line: %d\n", (int)i4_ret, __LINE__));
                i4_ret = WGLR_INTERNAL_ERROR;
                break;
            }
        }while (0);
    #endif
    }
    else if (_IS_IMG_TPL_JPG(pt_template) || _IS_IMG_TPL_PNG(pt_template) || _IS_IMG_TPL_BMP(pt_template))
     {
        SIZE_T z_width = (SIZE_T)t_surf_dest.ui4_width, z_height = (SIZE_T)t_surf_dest.ui4_height;
        if (decode_funtion_table.pf_get_dimension != NULL)
        {
            decode_funtion_table.pf_get_dimension(pt_template->h_img, INTRA_IMG_PRIMARY_IMG_TYPE, &z_width, &z_height);
        }

        /*Copy Image Data*/
        //x_dbg_stmt("begin to decode png img 0x%0x!\r\n", pt_template->h_img);
        if (decode_funtion_table.pf_sync_load != NULL)
        {
            if (pt_template->t_surface_info.ui4_width == 0 && _IS_IMG_PREFETCH(pt_template->ui2_flag))
            {
                i4_ret = decode_funtion_table.pf_sync_load(
                    pt_template->h_img,
                    INTRA_IMG_PRIMARY_IMG_TYPE,
                    NULL,
                    IMG_ROTATE_CLOCKWS_0_OP_TYPE,
                    0,
                    0,
                    (UINT32)z_width,
                    (UINT32)z_height,
                    0,
                    0,
                    t_surf_dest.ui4_width,
                    t_surf_dest.ui4_height,
                    h_surf_dest);
            }
            else
            {
                i4_ret = decode_funtion_table.pf_sync_load(
                    pt_template->h_img,
                    INTRA_IMG_PRIMARY_IMG_TYPE,
                    NULL,
                    IMG_ROTATE_CLOCKWS_0_OP_TYPE,
                    0,
                    0,
                    (UINT32)z_width,
                    (UINT32)z_height,
                    0,
                    0,
                    t_surf_dest.ui4_width,
                    t_surf_dest.ui4_height - pt_template->ui2_invert_lines,
                    h_surf_dest);
            }

            if (i4_ret != IMGR_OK)
            {
                return WGLR_INTERNAL_ERROR;
            }

            if (_IS_IMG_PREFETCH(pt_template->ui2_flag))
            {
                _wgl_surface_invert_image(h_surf_dest,
                                          t_surf_dest.ui4_width,
                                          0,
                                          0,
                                          t_surf_dest.ui4_height - pt_template->ui2_invert_lines,
                                          0,
                                          t_surf_dest.ui4_height - pt_template->ui2_invert_lines,
                                          (UINT32)pt_template->ui2_invert_lines);
            }

        }
        else
        {
            i4_ret = WGLR_INTERNAL_ERROR;
        }
    }
    else
    {
        do
        {
            /*copy image data to destination*/
            if(GLR_OK != (pt_clone_fct_tbl->pf_surf_blt)(h_surf_dest,
                                          pt_template->h_surface,
                                          0,
                                          0,
                                          0,
                                          0,
                                          t_surf_dest.ui4_width,
                                          t_surf_dest.ui4_height,
                                          255,
                                          GL_SYNC))
            {
                i4_ret = WGLR_IMG_DRAW_FAIL;
                break;
            }

            /*set return value*/
            *pe_type = pt_template->e_segment_type;
            *ph_surface = h_surf_dest;
            return WGLR_OK;

        } while(0);
    }


    /*free the clone surface*/
    i4_temp = (pt_clone_fct_tbl->pf_obj_free)(h_surf_dest);
	x_mem_free(pui1_line_buffer);
    WGL_ASSERT(GLR_OK == i4_temp);
    return i4_ret;
}

UINT8 wgl_bit_count(GL_COLORMODE_T e_mode)
{
    static UINT8 s_aui1gl_bit_count[] = { 2, 4, 8, 16, 16, 32, 2, 4, 8, 16, 16, 16, 32, 0, 0, 0, 0, 0, 0, 0 };

    return (s_aui1gl_bit_count[e_mode]);
}


INT32 _wgl_surface_invert_image(HANDLE_T h_surf,
                                      UINT32 ui4_width,
                                      INT32 i4_src_x,
                                      INT32 i4_src_y,
                                      UINT32 ui4_src_height,
                                      INT32 i4_dst_x,
                                      INT32 i4_dst_y,
                                      UINT32 ui4_dst_height)
{
    GL_LOCK_INFO_T  t_lock_info;
    UINT32  ui4_temp;
    UINT32  ui4_pixel_bytes;
    UINT8*  pv_src = NULL;
    UINT8*  pv_dst = NULL;

    x_gl_surface_lock(h_surf,
                      NULL,
                      0,
                      &t_lock_info);

    ui4_pixel_bytes = (UINT32)(wgl_bit_count(t_lock_info.e_clrmode) >> 3);

    pv_src = (UINT8*)t_lock_info.at_subplane[0].pv_bits + ((UINT32)i4_src_x + ui4_src_height)*t_lock_info.at_subplane[0].ui4_pitch +((UINT32)i4_src_y)*ui4_pixel_bytes;

    pv_dst = (UINT8*)t_lock_info.at_subplane[0].pv_bits + ((UINT32)i4_dst_x)*t_lock_info.at_subplane[0].ui4_pitch + ((UINT32)i4_dst_y)*ui4_pixel_bytes;

    for (ui4_temp = 0; ui4_temp < ui4_dst_height; ui4_temp ++)
    {
        x_memcpy(pv_dst, pv_src, ui4_width * ui4_pixel_bytes);
        pv_src = pv_src - t_lock_info.at_subplane[0].ui4_pitch;
        pv_dst = pv_dst + t_lock_info.at_subplane[0].ui4_pitch;
    }

    x_gl_surface_unlock(h_surf, NULL);
    return GLR_OK;
}


static INT32 wgl_img_tpl_create_surface(
    WGL_IMG_TPL_T*             pt_template)
{
    INT32 i4_ret;
    GL_SURFACE_DESC_T   t_surf_dest = {0};
    //GL_LOCK_INFO_T              t_surf_lock_info;
//    GL_COLOR_T                  t_color;
    SIZE_T z_width = 0, z_height = 0;
	GL_HSURFACE_T       h_temp_surf = NULL_HANDLE;

    /*prepare information to create surface*/

    t_surf_dest.ui4_width = pt_template->t_surface_info.ui4_width;
    t_surf_dest.ui4_height = pt_template->t_surface_info.ui4_height;

    t_surf_dest.e_clrmode = pt_template->t_surface_info.e_clrmode;
    t_surf_dest.h_palette = NULL_HANDLE;
    t_surf_dest.h_clipper = NULL_HANDLE;

    /*create surface*/
    if(GLR_OK != x_gl_surface_create(&t_surf_dest, &pt_template->h_surface))
    {
        i4_ret = WGLR_SYS_RC_CREATE_FAIL;
        goto _err_out;
    }

    /*cache sufrface info*/
    if(GLR_OK != x_gl_surface_get_info(pt_template->h_surface,
                                       &pt_template->t_surface_info))
    {
        i4_ret = WGLR_INTERNAL_ERROR;
        goto _err_out;
    }


    if (decode_funtion_table.pf_get_dimension != NULL)
    {
        decode_funtion_table.pf_get_dimension(pt_template->h_img, INTRA_IMG_PRIMARY_IMG_TYPE, &z_width, &z_height);
    }

    /*Copy Image Data*/
    //x_dbg_stmt("begin to decode png img 0x%0x!\r\n", pt_template->h_img);
    if (decode_funtion_table.pf_sync_load != NULL)
    {
    	if (h_temp_surf == NULL_HANDLE)
    {
        if (pt_template->t_surface_info.ui4_width == 0 && _IS_IMG_PREFETCH(pt_template->ui2_flag))
        {
            i4_ret = decode_funtion_table.pf_sync_load(
                pt_template->h_img,
                INTRA_IMG_PRIMARY_IMG_TYPE,
                NULL,
                IMG_ROTATE_CLOCKWS_0_OP_TYPE,
                0,
                0,
                (UINT32)z_width,
                (UINT32)z_height,
                0,
                0,
                t_surf_dest.ui4_width,
                t_surf_dest.ui4_height,
                pt_template->h_surface);
        }
        else
        {
            i4_ret = decode_funtion_table.pf_sync_load(
                pt_template->h_img,
                INTRA_IMG_PRIMARY_IMG_TYPE,
                NULL,
                IMG_ROTATE_CLOCKWS_0_OP_TYPE,
                0,
                0,
                (UINT32)z_width,
                (UINT32)z_height,
                0,
                0,
                t_surf_dest.ui4_width,
                t_surf_dest.ui4_height - pt_template->ui2_invert_lines,
                pt_template->h_surface);
        }
    	}
		else
		{
			i4_ret = decode_funtion_table.pf_sync_load(
	                pt_template->h_img,
	                INTRA_IMG_PRIMARY_IMG_TYPE,
	                NULL,
	                IMG_ROTATE_CLOCKWS_0_OP_TYPE,
	                0,
	                0,
	                (UINT32)z_width,
	                (UINT32)z_height,
	                0,
	                0,
	                t_surf_dest.ui4_width,
	                t_surf_dest.ui4_height,
	                h_temp_surf);
			if (i4_ret != IMGR_OK)
	        {
	            goto _err_out;
	        }
			
			i4_ret = x_gl_surface_adv_stretch_blt(pt_template->h_surface,
												  h_temp_surf,
												  0,
												  0,
												  pt_template->t_surface_info.ui4_width,
												  pt_template->t_surface_info.ui4_height - pt_template->ui2_invert_lines,
												  0,
												  0,
												  t_surf_dest.ui4_width,
												  t_surf_dest.ui4_height,
												  GL_SYNC);
			i4_ret = x_gl_obj_free(h_temp_surf);
			
			
		}
		
        if (i4_ret != IMGR_OK)
        {
            goto _err_out;
        }

        if (_IS_IMG_PREFETCH(pt_template->ui2_flag))
        {
            _wgl_surface_invert_image(pt_template->h_surface,
                                      t_surf_dest.ui4_width,
                                      0,
                                      0,
                                      t_surf_dest.ui4_height - pt_template->ui2_invert_lines,
                                      0,
                                      t_surf_dest.ui4_height - pt_template->ui2_invert_lines,
                                      (UINT32)pt_template->ui2_invert_lines);
        }

    }
    else
    {
        x_dbg_stmt("[WGL][IMAGE] Decode funtion not init!\r\n");
        return WGLR_INIT_FAILED;
    }

    return WGLR_OK;

_err_out:

    return i4_ret;
}

/*******************************************************************************
 * Name
 *          wgl_img_tpl_destroy_surface
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
static INT32 wgl_img_tpl_destroy_surface(
    WGL_IMG_TPL_T*             pt_template)
{
    if (pt_template->h_surface != NULL_HANDLE)
    {
        x_gl_obj_free(pt_template->h_surface);
        pt_template->h_surface = NULL_HANDLE;
        pt_template->t_surface_info.h_signal = NULL_HANDLE;
    }

    return WGLR_OK;
}

#ifdef CLI_LVL_ALL
static INT32 _wgl_img_add_to_list(
    WGL_IMG_TPL_T*             pt_template)
{
        HANDLE_T                    h_thread;
        BOOL                b_app_found = FALSE;
        IMG_APP_INFO_T    *pt_app_element;
        
        if(OSR_OK != x_thread_self(&h_thread))
        {
            return  WGLR_OS_ERROR;
        }
        DLIST_FOR_EACH(pt_app_element, &t_img_app_list, t_link)
        {
            if (pt_app_element->h_thread == h_thread)
            {
                b_app_found = TRUE;
                break;
            }
        }

        /* NOT found */
        if (!b_app_found)
        {
            /* add this app to the app_list */
            pt_app_element = (IMG_APP_INFO_T *)WGL_MEM_ALLOC(sizeof(IMG_APP_INFO_T));

            if (pt_app_element)
            {
                pt_app_element->h_thread    = h_thread;
                pt_app_element->ui4_img_count=0;
                
                DLIST_INIT(&(pt_app_element->t_img_tpl_list)); 
                
                DLIST_INSERT_TAIL(pt_app_element, &t_img_app_list, t_link);
            }

         }
        DLIST_INSERT_TAIL(pt_template,&(pt_app_element->t_img_tpl_list),t_link);
        pt_app_element->ui4_img_count++;
        return WGLR_OK;
}

static VOID _wgl_img_remove_from_list(
   WGL_IMG_TPL_T*             pt_template)
{
        BOOL                b_img_found = FALSE;
        IMG_APP_INFO_T    *pt_app_element;        
        WGL_IMG_TPL_T *pt_img_element = NULL;
        
        DLIST_FOR_EACH(pt_app_element, &t_img_app_list, t_link)
        {
            DLIST_FOR_EACH(pt_img_element, &pt_app_element->t_img_tpl_list, t_link)
            {
                if (pt_img_element == pt_template)
                {
                    b_img_found = TRUE;
                    break;
                }
           }
           if(b_img_found == TRUE)
           {
                break;
           }
        }
        
        if(b_img_found == TRUE)
        {
            /* remove this img from the list */
            DLIST_REMOVE(pt_img_element,&pt_app_element->t_img_tpl_list,t_link);
            pt_app_element->ui4_img_count--;

            /* if the img list is empty, remove this app from the app_list */
            if (pt_app_element->ui4_img_count == 0)
            {
                DLIST_REMOVE(pt_app_element,&t_img_app_list, t_link);
                WGL_MEM_FREE(pt_app_element);
            }
        }
}
INT32 wgl_img_print_mem_log(VOID)
{
    IMG_APP_INFO_T*    pt_app_info;
    WGL_IMAGE_CATCH_T*  pt_img_cache;
    GL_LOCK_INFO_T _t_lock_info;
    UINT32 ui4_img_surf_mem_size = 0;
        
    x_sema_lock(h_image_cache_lock,X_SEMA_OPTION_WAIT);
    
    DLIST_FOR_EACH(pt_img_cache, &t_img_cache_list, t_img_cache_entry)
    {
        x_dbg_stmt("Image Surface Handle: 0x%x, Image Surface Reference Count: %d, ",pt_img_cache->h_surf,pt_img_cache->ui4_reference_cnt);
        if(pt_img_cache->h_surf != NULL_HANDLE)
        {
           if (x_gl_surface_lock(pt_img_cache->h_surf, NULL, GL_SYNC, &_t_lock_info) != GLR_OK)
            {
                x_dbg_stmt("Error: x_gl_surface_lock(pt_img_cache->h_surf) return failed\n ");
                x_sema_unlock(h_image_cache_lock);
                return WGLR_LOCK_ERROR;
            }
           x_dbg_stmt("Image with surface bitmap size: %d bytes \n",(_t_lock_info.at_subplane[0].ui4_pitch*_t_lock_info.at_subplane[0].ui4_height));
           x_gl_surface_unlock(pt_img_cache->h_surf, NULL);
           ui4_img_surf_mem_size += _t_lock_info.at_subplane[0].ui4_pitch*_t_lock_info.at_subplane[0].ui4_height;
        }
    }
    x_dbg_stmt("Images with all surfaces bitmap size: %d bytes \n",ui4_img_surf_mem_size);
    
    x_sema_unlock(h_image_cache_lock);
    
    DLIST_FOR_EACH(pt_app_info, &t_img_app_list, t_link)
    {
       OS_THREAD_T     *pt_thread = NULL;
       WGL_IMG_TPL_T*   pt_img_template;
       
        /*1, print per app's thread name*/
        if (handle_get_obj(pt_app_info->h_thread , (VOID **)&pt_thread) == HR_OK)
        {
            if (pt_thread != NULL)
            {
                x_dbg_stmt("APP: %s \n",pt_thread->s_name);
            }
        }
        DLIST_FOR_EACH(pt_img_template, &(pt_app_info->t_img_tpl_list), t_link)
        {
           x_dbg_stmt("\n Image Template Handle: 0x%x, Image Surface Handle: 0x%x \n",pt_img_template->h_this,pt_img_template->h_surface);
        }
    }
    return WGLR_OK;
}

#endif

VOID _wgl_img_module_lock(VOID)
{
   INT32 i4_res;

   if (!gb_wgl_image_inited)
   {
   		wgl_image_init();
   }
   
   i4_res = x_sema_lock(h_wgl_image_module_lock,
						X_SEMA_OPTION_WAIT);

   if (i4_res != OSR_OK)
   {
	   ABORT_IMAGE(DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_WGL_IMG);
   }

}


VOID _wgl_img_module_unlock(VOID)
{
	INT32 i4_res;

	if (!gb_wgl_image_inited)
    {
   		wgl_image_init();
    }
	
	i4_res = x_sema_unlock(h_wgl_image_module_lock);
	
	if (i4_res != OSR_OK)
	{
		ABORT_IMAGE(DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_WGL_IMG);
	}
}



