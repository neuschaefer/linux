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
 * Description: 
 *          This file contains the implementation of exported APIs of the 
 *          Widget Library.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "inc/u_common.h"
#include "handle/handle.h"
#include "os/inc/u_os.h"
#include "wgl/c_wgl.h"
#include "wgl/x_wgl.h"
#include "aee/aee.h"
#include "wgl/wgl.h"
#include "dbg/dbg.h"
#include "dbg/def_dbg_level_mw.h"
#include "wgl/animation/u_wgl_animation.h"

/* for debug use */
#undef DBG_LEVEL_MODULE
#ifdef CLI_LVL_ALL
#define DBG_LEVEL_MODULE            wgl_get_dbg_level()
#else
#define DBG_LEVEL_MODULE DBG_INIT_LEVEL_MW_WGL_MNGR
#endif

/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/
INT32 c_wgl_add_transparent_list_exclusion(
    HANDLE_T                h_rgn_list,
    GL_RECT_T               at_exclusive_rect[],
    UINT32                  ui4_rect_count)
{
    return x_wgl_add_transparent_list_exclusion(h_rgn_list, at_exclusive_rect, ui4_rect_count);
}

INT32 c_wgl_broadcast_cmd(
    HANDLE_T                h_container,
    UINT32                  ui4_cmd, 
    VOID*                   pv_param1,
    VOID*                   pv_param2)
{
    return x_wgl_broadcast_cmd(h_container, ui4_cmd, pv_param1, pv_param2);
}

BOOL c_wgl_canvas_with_visibles(HANDLE_T h_canvas)
{
    return x_wgl_canvas_with_visibles(h_canvas);
}

INT32 c_wgl_create_timer(
    HANDLE_T                h_widget,
    WGL_TIMER_SYNC_MODE_T   e_sync_mode,
    HANDLE_T*               ph_timer)
{
    HANDLE_T  h_aux;
    INT32     i4_res;
    
    i4_res = aee_grab_handle_resource(
        AEE_FLAG,
        1,
        &h_aux);
    
    if (i4_res == AEER_OK)
    {
        i4_res = x_wgl_create_timer(
            h_widget,
            e_sync_mode,
            ph_timer);
        
        if (i4_res == WGLR_OK)
        {
            handle_link_to_aux(h_aux, *ph_timer);
        }
        else
        {
            aee_release_handle_resource(1);
        }
    }
    else
    {
        i4_res = WGLR_OUT_OF_HANDLES;
        DBG_ERROR(("{WGL} ERR: Out of handles at L%d\r\n\r\n", __LINE__));
    }
    
    DBG_API(("{WGL} %s: leaving (%d)...\r\n\r\n", __FUNCTION__, i4_res));
    return (i4_res);
}

/* create a transparent list. after a list is successfully created, it is DISABLED by default */
INT32 c_wgl_create_transparent_list(
    HANDLE_T                h_canvas,               /* in:  handle of the target canvas*/
    GL_RECT_T               at_rect[],              /* in:  an array of GL_RECT_T */
    UINT32                  ui4_rect_count,         /* in:  rectangle counts of at_rect[] */
    HANDLE_T                *ph_rgn_list)           /* out: handle of the transparent list */
{
    HANDLE_T  h_aux;
    INT32     i4_res;
    
    i4_res = aee_grab_handle_resource(
        AEE_FLAG,
        1,
        &h_aux);
    
    if (i4_res == AEER_OK)
    {
        i4_res = x_wgl_create_transparent_list(h_canvas, at_rect, ui4_rect_count, ph_rgn_list);
        
        if (i4_res == WGLR_OK)
        {
            handle_link_to_aux(h_aux, *ph_rgn_list);
        }
        else
        {
            aee_release_handle_resource(1);
        }
    }
    else
    {
        i4_res = WGLR_OUT_OF_HANDLES;
        DBG_ERROR(("{WGL} ERR: Out of handles at L%d\r\n\r\n", __LINE__));
    }
    
    DBG_API(("{WGL} %s: leaving (%d)...\r\n\r\n", __FUNCTION__, i4_res));
    return (i4_res);
}

INT32 c_wgl_create_widget(
    HANDLE_T                h_container,
    HANDLE_TYPE_T           e_type_widget,
    UINT32                  ui4_type_content,
    UINT32                  ui4_type_border,
    const GL_RECT_T*        pt_rect,
    wgl_widget_proc_fct     pf_wdgt_proc,
    UINT8                   ui1_alpha,
    VOID*                   pv_param,
    VOID*                   pv_tag,
    HANDLE_T*               ph_widget)
{
    HANDLE_T  h_aux;
    INT32     i4_res;
    
    i4_res = aee_grab_handle_resource(
        AEE_FLAG,
        1,
        &h_aux);
    
    if (i4_res == AEER_OK)
    {
        i4_res = x_wgl_create_widget(
            h_container,
            e_type_widget,
            ui4_type_content,
            ui4_type_border,
            pt_rect,
            pf_wdgt_proc,
            ui1_alpha,
            pv_param,
            pv_tag,
            ph_widget);
        
        if (i4_res == WGLR_OK)
        {
            handle_link_to_aux(h_aux, *ph_widget);
        }
        else
        {
            aee_release_handle_resource(1);
        }
    }
    else
    {
        i4_res = WGLR_OUT_OF_HANDLES;
        DBG_ERROR(("{WGL} ERR: Out of handles at L%d\r\n\r\n", __LINE__));
    }
    
    DBG_API(("{WGL} %s: leaving (%d)...\r\n\r\n", __FUNCTION__, i4_res));
    return (i4_res);
}

INT32 c_wgl_delete_timer(HANDLE_T h_timer)
{
    return (x_wgl_delete_timer(h_timer));
}

/* delete a transparent list */
INT32 c_wgl_delete_transparent_list(
    HANDLE_T            h_rgn_list)             /* in: handle of a transparent list to be deleted */
{
    return x_wgl_delete_transparent_list(h_rgn_list);
}

INT32 c_wgl_default_msg_proc(
    HANDLE_T            h_widget,
    UINT32              ui4_msg,
    VOID*               pv_param1,
    VOID*               pv_param2)
{
    return (x_wgl_default_msg_proc(h_widget, ui4_msg, pv_param1, pv_param2));
}

INT32 c_wgl_destroy_app_widget(VOID)
{
    return (x_wgl_destroy_app_widget());
}

/* NOTE: ASYNChronous repaint would be triggered while destroying */
INT32 c_wgl_destroy_widget(HANDLE_T h_widget)
{
    return (x_wgl_destroy_widget(h_widget));
}

INT32 c_wgl_destroy_widget_ex(HANDLE_T h_widget, BOOL b_repaint)
{
    return (x_wgl_destroy_widget_ex(h_widget, b_repaint));
}

INT32 c_wgl_do_cmd(
    HANDLE_T            h_widget,
    UINT32              ui4_cmd, 
    VOID*               pv_param1,
    VOID*               pv_param2)
{
    return (x_wgl_do_cmd(h_widget, ui4_cmd, pv_param1, pv_param2));
}

INT32 c_wgl_dispatch_event(
    UINT32              ui4_msg,
    VOID*               pv_param1,
    VOID*               pv_param2)
{
    return (x_wgl_dispatch_event(ui4_msg, pv_param1, pv_param2));
}

INT32 c_wgl_enable(
    HANDLE_T            h_widget,  
    BOOL                b_enable)
{
    return (x_wgl_enable(h_widget, b_enable));
}

/* change the state of a transparent list */
INT32 c_wgl_enable_transparent_list(
    HANDLE_T            h_rgn_list,             /* in: handle of a transparent list */
    BOOL                b_enabled,              /* in: enable this list or not */
    BOOL                b_exclusive)            /* in: set its state as exclusive or union */
{
    return x_wgl_enable_transparent_list(h_rgn_list, b_enabled, b_exclusive);
}


INT32 c_wgl_flip_canvas(
    HANDLE_T            h_canvas)
{
    return x_wgl_flip_canvas(h_canvas);
}

INT32 c_wgl_float(
    HANDLE_T            h_widget,
    BOOL                b_float,
    UINT8               ui1_repaint)
{
    return (x_wgl_float(h_widget, b_float, ui1_repaint));
}

INT32 c_wgl_get_app_order(
    HANDLE_T            h_app,
    UINT8               *pui1_order)
{
    HANDLE_T h_thread;

    if (h_app != NULL_HANDLE)
    {
        if (AEER_OK == aee_get_thread(h_app, &h_thread))
        {
            return (x_wgl_get_app_order(h_thread, pui1_order));
        }
    }

    return WGLR_INV_HANDLE;
}

INT32 c_wgl_get_canvas(
    GL_HPLANE_T         h_plane,
    HANDLE_T*           ph_canvas)
{
    /* since it's not necessary to free canvas, the handle is not recorded in AEE */
    return (x_wgl_get_canvas(h_plane, ph_canvas));
}

INT32 c_wgl_get_canvas_ex(
    GL_HPLANE_T             h_plane,
    const WGL_CANVAS_CFG_T* pt_cfg,
    HANDLE_T*               ph_canvas)
    
{
    /* since it's not necessary to free canvas, the handle is not recorded in AEE */
    return (x_wgl_get_canvas_ex(h_plane, pt_cfg, ph_canvas));
}

INT32 c_wgl_get_canvas_info(
    HANDLE_T            h_obj,
    WGL_CANVAS_INFO_T*  pt_cvs_info)
{
    return (x_wgl_get_canvas_info(h_obj, pt_cvs_info));
}

INT32 c_wgl_get_canvas_palette(
    HANDLE_T        h_obj,
    GL_HPALETTE_T*  ph_palette)
{
    HANDLE_T  h_aux;
    INT32     i4_res;
    
    i4_res = aee_grab_handle_resource(
        AEE_FLAG,
        1,
        &h_aux);
    
    if (i4_res == AEER_OK)
    {
        i4_res = x_wgl_get_canvas_palette(h_obj, ph_palette);
        
        if (i4_res == WGLR_OK)
        {
            handle_link_to_aux(h_aux, *ph_palette);
        }
        else
        {
            aee_release_handle_resource(1);
        }
    }
    else
    {
        i4_res = WGLR_OUT_OF_HANDLES;
        DBG_ERROR(("{WGL} ERR: Out of handles at L%d\r\n\r\n", __LINE__));
    }
    
    return (i4_res);
}

INT32 c_wgl_get_canvas_size(
    HANDLE_T    h_canvas,
    UINT32*     pui4_width,
    UINT32*     pui4_height)
{
    return (x_wgl_get_canvas_size(h_canvas, pui4_width, pui4_height));
}
INT32    c_wgl_destroy_canvas(
     HANDLE_T    h_canvas)
{
    return (x_wgl_destroy_canvas(h_canvas));
}

INT32 c_wgl_get_children_order(
    HANDLE_T            h_parent,
    HANDLE_T            ah_children[],
    INT32               *pi4_number)
{
    return x_wgl_get_children_order(h_parent, ah_children, pi4_number);
}

INT32 c_wgl_get_coords(
    HANDLE_T            h_widget,
    WGL_COORDS_T        e_coords,
    GL_RECT_T*          pt_rect)
{
#ifdef WGL_COORDINATE_INVERSE    
    return (x_wgl_get_coords_for_inverse(h_widget, e_coords, pt_rect));
#else
    return (x_wgl_get_coords(h_widget, e_coords, pt_rect));
#endif
}

INT32 c_wgl_get_focus(HANDLE_T* ph_widget)
{
    return (x_wgl_get_focus(ph_widget));
}

INT32 c_wgl_get_parent(
    HANDLE_T            h_widget,
    HANDLE_T*           ph_parent)
{
    return (x_wgl_get_parent(h_widget, ph_parent));
}

INT32 c_wgl_get_state(
    HANDLE_T        h_widget,
    UINT32*         pui4_state)
{
    return (x_wgl_get_state(h_widget, pui4_state));
}

INT32 c_wgl_get_update_region(
    HANDLE_T        h_widget,
    UINT32*         pui4_num_rect,
    GL_RECT_T*      pt_upt_rect)
{
    return (x_wgl_get_update_region(h_widget, pui4_num_rect, pt_upt_rect));
}

INT32 c_wgl_get_visibility(
    HANDLE_T        h_widget,
    BOOL            *pb_visible)
{
    return x_wgl_get_visibility(h_widget, pb_visible);
}

INT32 c_wgl_get_widget_proc(
    HANDLE_T                h_widget,
    wgl_widget_proc_fct     *ppf_proc)
{
    return x_wgl_get_widget_proc(h_widget, ppf_proc);
}

INT32 c_wgl_ignore_visibility_detect(
    HANDLE_T    h_widget,
    BOOL        b_ignore)
{
    return (x_wgl_ignore_visibility_detect(h_widget, b_ignore));
}

INT32 c_wgl_insert(
    HANDLE_T            h_widget,
    HANDLE_T            h_ref_widget,
    WGL_INSERT_MODE_T   e_insert,
    UINT8               ui1_repaint)
{
    return (x_wgl_insert(h_widget, h_ref_widget, e_insert, ui1_repaint));
}

INT32 c_wgl_move(
    HANDLE_T            h_widget,
    const GL_RECT_T*    pt_rect,
    UINT8               ui1_repaint)
{
    return (x_wgl_move(h_widget, pt_rect, ui1_repaint));
}

INT32 c_wgl_query_canvas_info_callback(
    HANDLE_T            h_widget,
    WGL_CVS_CB_TYPE_T   e_type,
    VOID                *pv_data,
    SIZE_T              *pz_size)
{
    return (x_wgl_query_canvas_info_callback(h_widget, e_type, pv_data, pz_size));
}

INT32 c_wgl_register(wgl_app_callback_fct pf_callback)
{
    HANDLE_T    h_client;
    HANDLE_T    h_aux;
    INT32       i4_res;
    
    DBG_API(
        ("{WGL} %s:\r\n"
         "\tpf_callback = 0x%.8x\r\n\r\n",
         __FUNCTION__,
         pf_callback));
    
    i4_res = aee_grab_handle_resource(
        AEE_FLAG,
        1,
        &h_aux);
    
    if (i4_res == AEER_OK)
    {
        i4_res = x_wgl_register(pf_callback, &h_client);
        
        if (i4_res == WGLR_OK)
        {
            handle_link_to_aux(h_aux, h_client);
        }
        else if (i4_res == WGLR_ALREADY_REGISTERED)
        {
            aee_release_handle_resource(1);
            i4_res = WGLR_OK;
        }
        else /* FAIL */
        {
            aee_release_handle_resource(1);
        }
    }
    else
    {
        i4_res = WGLR_OUT_OF_HANDLES;
        DBG_ERROR(("{WGL} ERR: Out of handles at L%d\r\n\r\n", __LINE__));
    }
    
    DBG_API(("{WGL} %s: leaving (%d)...\r\n\r\n", __FUNCTION__, i4_res));
    return (i4_res);
}

INT32 c_wgl_repaint(
    HANDLE_T            h_container,  
    const GL_RECT_T*    pt_rect,
    BOOL                b_sync)
{
    return (x_wgl_repaint(h_container, pt_rect, b_sync));
}

INT32 c_wgl_send_msg(
    HANDLE_T            h_widget,
    UINT32              ui4_msg,
    VOID*               pv_param1,
    VOID*               pv_param2)
{
    return (x_wgl_send_msg(h_widget, ui4_msg, pv_param1, pv_param2));
}

INT32 c_wgl_send_notify(
    HANDLE_T            h_parent,
    UINT32              ui4_nc_code,
    HANDLE_T            h_widget,
    WGL_NOTIFY_DATA_T*  pt_nfy_data)
{
    return (x_wgl_send_notify(h_parent, ui4_nc_code, h_widget, pt_nfy_data));
}

INT32 c_wgl_set_active_app(HANDLE_T h_app)
{
    HANDLE_T h_thread;

    DBG_API(
        ("{WGL} %s:\r\n"
         "\th_app = %d\r\n\r\n",
         __FUNCTION__,
         h_app));
    
    if (h_app != NULL_HANDLE)
    {
        if (AEER_OK == aee_get_thread(h_app, &h_thread))
        {
            return (x_wgl_set_active_app(h_thread));
        }
    }
    else
    {
        return (x_wgl_set_active_app(NULL_HANDLE));
    }

    DBG_ERROR(("{WGL} ERR: Invalid client handle at L%d\r\n\r\n", __LINE__));
    DBG_API(("{WGL} %s: leaving (%d)...\r\n\r\n", __FUNCTION__, WGLR_INV_HANDLE));
    return WGLR_INV_HANDLE;
}

INT32 c_wgl_set_app_order(HANDLE_T h_app, UINT8 ui1_order)
{
    HANDLE_T h_thread;

    if (h_app != NULL_HANDLE)
    {
        if (AEER_OK == aee_get_thread(h_app, &h_thread))
        {
            return (x_wgl_set_app_order(h_thread, ui1_order));
        }
    }

    return WGLR_INV_HANDLE;
}

INT32 c_wgl_set_alpha(
    HANDLE_T            h_widget,
    UINT8               ui1_alpha)
{
    return (x_wgl_set_alpha(h_widget, ui1_alpha));
}

INT32 c_wgl_set_app_ignore_key_fct(wgl_app_callback_fct pf_app_ignore_key_fct)
{
    return (x_wgl_set_app_ignore_key_fct(pf_app_ignore_key_fct));
}

INT32 c_wgl_set_widget_ignore_key_fct(wgl_widget_callback_fct pf_widget_ignore_key_fct)
{
    return (x_wgl_set_widget_ignore_key_fct(pf_widget_ignore_key_fct));
}


INT32 c_wgl_set_canvas_callback(
    HANDLE_T                    h_canvas,
    wgl_canvas_callback_fct     pf_cvs_nfy_fct,
    BOOL                        b_enable)
{
    return (x_wgl_set_canvas_callback(h_canvas, pf_cvs_nfy_fct, b_enable));
}

INT32 c_wgl_set_canvas_color(
    HANDLE_T                    h_canvas,
    const WGL_CVS_CLR_CFG_T*    pt_cvs_clr)
{
    return (x_wgl_set_canvas_color(h_canvas, pt_cvs_clr));
}

INT32 c_wgl_set_canvas_info_callback(
    HANDLE_T                    h_canvas,
    WGL_CVS_CB_TYPE_T           e_type,
    VOID                        *pv_cb_fct)
{
    return (x_wgl_set_canvas_info_callback(h_canvas, e_type, pv_cb_fct));
}

INT32 c_wgl_set_children_order(
    HANDLE_T            h_parent,
    HANDLE_T            ah_children[],
    INT32               i4_number)
{
    return x_wgl_set_children_order(h_parent, ah_children, i4_number);
}

INT32 c_wgl_set_delay_tolerance(UINT32 ui4_delay)
{
    return (x_wgl_set_delay_tolerance(ui4_delay));
}

INT32 c_wgl_set_focus(
    HANDLE_T            h_widget,
    UINT8               ui1_repaint)
{
    return (x_wgl_set_focus(h_widget, ui1_repaint));
}

INT32 c_wgl_set_navigation(
    HANDLE_T                h_widget,
    UINT8                   ui1_cnt,
    const WGL_KEY_LINK_T    at_key_lnk[])
{
    return (x_wgl_set_navigation(h_widget, ui1_cnt, at_key_lnk));
}

INT32 c_wgl_set_shadow_canvas_block_time(
    UINT32              ui4_timeout)
{
    return x_wgl_set_shadow_canvas_block_time(ui4_timeout);
}

INT32 c_wgl_set_transparent_list(
    HANDLE_T            h_rgn_list,
    GL_RECT_T           at_rect[],
    UINT32              ui4_rect_count)
{
    return x_wgl_set_transparent_list(h_rgn_list, at_rect, ui4_rect_count);
}

INT32 c_wgl_set_visibility(
    HANDLE_T            h_widget,
    WGL_SW_CMD_T        e_sw_cmd)
{
    return (x_wgl_set_visibility(h_widget, e_sw_cmd));
}

INT32 c_wgl_set_widget_proc(
    HANDLE_T            h_widget,
    wgl_widget_proc_fct pf_proc)
{
    return x_wgl_set_widget_proc(h_widget, pf_proc);
}

INT32 c_wgl_show(
    HANDLE_T            h_widget,
    WGL_SW_CMD_T        e_sw_cmd)
{
    return (x_wgl_show(h_widget, e_sw_cmd));
}

INT32 c_wgl_start_timer(
    HANDLE_T            h_timer,
    UINT32              ui4_delay,
    TIMER_FLAG_T        e_flag,
    VOID*               pv_tag)
{
    return (x_wgl_start_timer(h_timer, ui4_delay, e_flag, pv_tag));
}

INT32 c_wgl_stop_timer(HANDLE_T h_timer)
{
    return (x_wgl_stop_timer(h_timer));
}

INT32 c_wgl_get_string_size(
    HFONT_T           h_fnt,
    const UTF16_T*                w2s_str,        /* string to be evaluated        */
    INT32                   i4_length,      /* max length to be evaluated    */
    INT32*                  pi4_width,      /* width of string               */
    INT32*                  pi4_height)     /* height of string              */

{
    return x_wgl_get_string_size(h_fnt, w2s_str, i4_length, pi4_width, pi4_height);
}

HANDLE_T c_wgl_get_canvas_surface_handle(HANDLE_T h_canvas)
{
    return x_wgl_get_canvas_surface_handle(h_canvas);
}
INT32 c_wgl_canvas_reconfig(
    HANDLE_T                h_canvas,
    GL_HPLANE_T             h_plane,
    const WGL_CANVAS_CFG_T* pt_cfg)
{
    return x_wgl_canvas_reconfig(h_canvas,h_plane,pt_cfg);
}

INT32 c_wgl_get_canvas_config(HANDLE_T h_canvas,
    							   WGL_CANVAS_CFG_T* pt_cfg)
{
	return x_wgl_get_canvas_config(h_canvas, pt_cfg);
}

#ifdef WGL_ANIM_SUPPORT    

INT32 c_wgl_start_animation(HANDLE_T h_widget, WGL_ANIMATION_DATA_T* pt_anim_data,WGL_ANIM_CONTEXT_T e_context)
{
    return x_wgl_start_animation(h_widget, pt_anim_data, e_context);
}


INT32 c_wgl_end_animation(HANDLE_T h_widget, VOID *pv_param1, VOID *pv_param2)
{
    return x_wgl_end_animation(h_widget, pv_param1, pv_param2);
}



INT32 c_wgl_set_anim_callback(
                    HANDLE_T h_widget,
                    wgl_app_anim_callback_fct pf_anim_nfy_fct,
                    void* pv_private_tag)
{
    return x_wgl_set_anim_callback(h_widget,pf_anim_nfy_fct,pv_private_tag);
}
INT32 c_wgl_set_animation_max_img_set(
                    HANDLE_T h_widget,
                    UINT16 ui2_max_img_set)
{
    return x_wgl_set_animation_max_img_set(h_widget,ui2_max_img_set);
}

INT32 c_wgl_set_animation_img_set(
                	HANDLE_T                h_widget,
                    UINT8                   ui1_img_set_index,     
                	const WGL_ANIM_IMG_SEQ_T*       pt_anim_img_seq)
{
    return x_wgl_set_animation_img_set(h_widget,ui1_img_set_index,pt_anim_img_seq);
}

INT32 c_wgl_set_animation_map(
                	HANDLE_T                h_widget,
                    UINT8                   ui1_map_count,     
                	const WGL_ANIM_ACTION_MAP_T*       pt_anim_map)
{
    return x_wgl_set_animation_map(h_widget,ui1_map_count,pt_anim_map);
}

UINT32 c_wgl_get_tick_interval(VOID)
{
    return x_wgl_get_tick_interval();
}

WGL_TICK_MODE_T c_wgl_get_tick_mode(VOID)
{
    return x_wgl_get_tick_mode();
}

INT32 c_wgl_get_anim_status(
    HANDLE_T            h_widget,
    UINT16              *pui2_anim_type)
{
    return x_wgl_get_anim_status(h_widget, pui2_anim_type);
}

#endif

#ifdef WGL_COORDINATE_INVERSE
INT32 c_wgl_set_coordinate_reverse(BOOL b_coordinate_reverse,UINT8 ui1_repaint)
{
    return x_wgl_set_coordinate_reverse(b_coordinate_reverse,ui1_repaint);
}
#endif
