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
 *
 * SWAuthor: Chun Zhou, Tinghong Liang
 * Description:
 *
 *---------------------------------------------------------------------------*/

#include "os/inc/x_os.h"
#include "dbg/def_dbg_level_mw.h"
#include "wgl/wgl_sets.h"
#include "wgl/u_wgl_rect.h"
#include "wgl/wgl.h"
#include "wgl/_wgl.h"
#include "wgl/wgl_tools.h"
#include "handle/handle.h"
#include "aee/aee.h"
#include "wgl/matrixbox/x_wgl_mbw.h"
#include "wgl/matrixbox/wgl_mbw_cli.h"

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#undef DBG_LEVEL_MODULE
#ifdef CLI_LVL_ALL
#define DBG_LEVEL_MODULE wgl_mbw_cli_dbg_level_get()
#else
#define DBG_LEVEL_MODULE DBG_INIT_LEVEL_MW_WGL_MNGR
#endif

INT32 c_wgl_mbw_create (
     HANDLE_T               h_container,
     const WGL_MBW_INIT_T*  pt_init_info,
     const GL_RECT_T*       pt_rect,
     wgl_widget_proc_fct    pf_wdgt_proc,
     VOID*                  pv_tag,
     HANDLE_T*              ph_mbw)
{
    HANDLE_T  h_aux;
    INT32     i4_res;

    i4_res = aee_grab_handle_resource(
        AEE_FLAG,
        1,
        &h_aux);

    if (i4_res == AEER_OK)
    {
        i4_res = x_wgl_mbw_create(
                                h_container,
                                pt_init_info,
                                pt_rect,
                                pf_wdgt_proc,
                                pv_tag,
                                ph_mbw);

        if (i4_res == WGLR_OK)
        {
            handle_link_to_aux(h_aux, *ph_mbw);
        }
        else
        {
            aee_release_handle_resource(1);
        }
    }
    else
    {
        i4_res = WGLR_OUT_OF_HANDLES;
        DBG_ERROR(("{WGL(MBW)} ERR: Out of handles at L%d\r\n\r\n", __LINE__));
    }

    DBG_API(("{WGL(MBW)} %s: leaving (%d)...\r\n\r\n", __FUNCTION__, i4_res));
    return (i4_res);

}

INT32 c_wgl_mbw_destroy (
    HANDLE_T    h_mbw)
{
    return x_wgl_mbw_destroy(h_mbw);
}

INT32 c_wgl_mbw_default_proc (
    HANDLE_T    h_mbw,
    UINT32      ui4_msg,
    VOID*       pv_param1,
    VOID*       pv_param2)
{
    return x_wgl_mbw_default_proc(h_mbw, ui4_msg, pv_param1, pv_param2);
}

INT32 c_wgl_mbw_default_key_hdlr (
    HANDLE_T                        h_mbw,
    WGL_MBW_KEY_HDLR_TYPE_T         e_key_hdlr_type,
    UINT32                          ui4_data)
{
    return x_wgl_mbw_default_key_hdlr(h_mbw, e_key_hdlr_type, ui4_data);
}

INT32 c_wgl_mbw_default_evn_hdlr (
    HANDLE_T                 h_mbw,
    WGL_MBW_EVN_HDLR_TYPE_T  e_evn_hdlr_type,
    UINT32                   ui4_data)
{
    return x_wgl_mbw_default_evn_hdlr(h_mbw, e_evn_hdlr_type, ui4_data);
}

INT32 c_wgl_mbw_reg_key_hdlr (
    HANDLE_T                        h_mbw,
    const WGL_MBW_KEY_HDLR_INFO_T*  pt_key_hdlr_info)
{
    return x_wgl_mbw_reg_key_hdlr(h_mbw, pt_key_hdlr_info);
}

INT32 c_wgl_mbw_reg_event_hdlr(
    HANDLE_T                        h_mbw,
    const WGL_MBW_EVN_HDLR_INFO_T*  pt_evn_hdlr_info)
{
    return x_wgl_mbw_reg_event_hdlr(h_mbw, pt_evn_hdlr_info);
}

INT32 c_wgl_mbw_get_nvgt_info (
    HANDLE_T                h_mbw,
    UINT32                  ui4_idx_from,
    WGL_MBW_NVGT_DIR_T      e_nvgt_dir,
    WGL_MBW_NVGT_INFO_T*    pt_nvgt_info)
{
    return x_wgl_mbw_get_nvgt_info(h_mbw, ui4_idx_from, e_nvgt_dir, pt_nvgt_info);
}

INT32 c_wgl_mbw_begin_transition (
    HANDLE_T    h_mbw)
{
    return x_wgl_mbw_begin_transition(h_mbw);
}

INT32 c_wgl_mbw_end_transition (
    HANDLE_T    h_mbw)
{
    return x_wgl_mbw_end_transition(h_mbw);
}

INT32 c_wgl_mbw_reload_with_hlt (
    HANDLE_T    h_mbw,
    UINT32      ui4_num_total,
    UINT32      ui4_hlt_item_idx)
{
    return x_wgl_mbw_reload_with_hlt(h_mbw, ui4_num_total,ui4_hlt_item_idx);
}

INT32 c_wgl_mbw_reload (
    HANDLE_T    h_mbw,
    UINT32      ui4_num_total)
{
    return x_wgl_mbw_reload(h_mbw, ui4_num_total);
}

INT32 c_wgl_mbw_get_total_num (
    HANDLE_T    h_mbw,
    UINT32*     pui4_num_total)
{
    return x_wgl_mbw_get_total_num(h_mbw, pui4_num_total);
}

INT32 c_wgl_mbw_reload_item_static (
    HANDLE_T    h_mbw,
    UINT32      ui4_idx)
{
    return x_wgl_mbw_reload_item_static(h_mbw, ui4_idx);
}

INT32 c_wgl_mbw_reload_item_data (
    HANDLE_T                h_mbw,
    UINT32                  ui4_idx)
{
    return x_wgl_mbw_reload_item_data(h_mbw, ui4_idx);
}

INT32 c_wgl_mbw_repaint_item (
    HANDLE_T    h_mbw,
    UINT32      ui4_idx)
{
    return x_wgl_mbw_repaint_item(h_mbw, ui4_idx);
}

INT32 c_wgl_mbw_enable_item_state (
    HANDLE_T    h_mbw,
    UINT32      ui4_idx,
    UINT32      ui4_state_mask)
{
    return x_wgl_mbw_enable_item_state(h_mbw, ui4_idx, ui4_state_mask);
}

INT32 c_wgl_mbw_disable_item_state (
    HANDLE_T    h_mbw,
    UINT32      ui4_idx,
    UINT32      ui4_state_mask)
{
    return x_wgl_mbw_disable_item_state(h_mbw, ui4_idx, ui4_state_mask);
}

INT32 c_wgl_mbw_get_item_state (
    HANDLE_T    h_mbw,
    UINT32      ui4_idx,
    UINT32*     pui4_state)
{
    return x_wgl_mbw_get_item_state(h_mbw, ui4_idx, pui4_state);
}

INT32 c_wgl_mbw_get_hlt_item (
    HANDLE_T    h_mbw,
    UINT32*     pui4_idx_hlt)
{
    return x_wgl_mbw_get_hlt_item(h_mbw, pui4_idx_hlt);
}

INT32 c_wgl_mbw_set_key_map (
    HANDLE_T                    h_mbw,
    const WGL_MBW_KEY_MAP_T*    pt_key_map)
{
    return x_wgl_mbw_set_key_map(h_mbw, pt_key_map);
}

INT32 c_wgl_mbw_get_visible_range_in_vp (
    HANDLE_T    h_mbw,
    UINT32*     pui4_first,
    UINT32*     pui4_last)
{
    return x_wgl_mbw_get_visible_range_in_vp(h_mbw, pui4_first, pui4_last);
}

INT32 c_wgl_mbw_get_item_ctnr_rect (
    HANDLE_T    h_mbw,
    UINT32      ui4_item_ctnr_idx,
    GL_RECT_T*  pt_rect)
{
    return x_wgl_mbw_get_item_ctnr_rect(h_mbw, ui4_item_ctnr_idx, pt_rect);
}

INT32 c_wgl_mbw_set_image (
    HANDLE_T                h_mbw,
    UINT8                   ui1_target,
    const WGL_IMG_INFO_T*   pt_img_info)
{
    return x_wgl_mbw_set_image(h_mbw, ui1_target, pt_img_info);
}

INT32 c_wgl_mbw_set_color (
    HANDLE_T                h_mbw,
    UINT8                   ui1_target,
    const WGL_COLOR_INFO_T* pt_clr_info)
{
    return x_wgl_mbw_set_color(h_mbw, ui1_target, pt_clr_info);
}

INT32 c_wgl_mbw_get_image (
    HANDLE_T                h_mbw,
    UINT8                   ui1_target,
    WGL_IMG_INFO_T*         pt_img_info)
{
    return x_wgl_mbw_get_image(h_mbw, ui1_target, pt_img_info);
}

INT32 c_wgl_mbw_get_color (
    HANDLE_T                h_mbw,
    UINT8                   ui1_target,
    WGL_COLOR_INFO_T*       pt_clr_info)
{
    return x_wgl_mbw_get_color(h_mbw, ui1_target, pt_clr_info);
}

INT32 c_wgl_mbw_set_hlt_item_info (
    HANDLE_T                        h_mbw,
    const WGL_MBW_HLT_ITEM_INFO_T*  pt_hlt_info)
{
    return x_wgl_mbw_set_hlt_item_info(h_mbw, pt_hlt_info);
}

INT32 c_wgl_mbw_get_hlt_item_info (
    HANDLE_T                    h_mbw,
    WGL_MBW_HLT_ITEM_INFO_T*    pt_hlt_info)
{
    return x_wgl_mbw_get_hlt_item_info(h_mbw, pt_hlt_info);
}

INT32 c_wgl_mbw_get_hlt_item_ctnr_info (
    HANDLE_T                    h_mbw,
    WGL_MBW_ITEM_CTNR_INFO_T*   pt_hlt_item_ctnr_info)
{
    return x_wgl_mbw_get_hlt_item_ctnr_info(h_mbw, pt_hlt_item_ctnr_info);
}

INT32 c_wgl_mbw_move_item_ctnr (
    HANDLE_T    h_mbw,
    UINT32      ui4_item_ctnr_idx,
    INT32       i4_offset_x,
    INT32       i4_offset_y)
{
    return x_wgl_mbw_move_item_ctnr(h_mbw, ui4_item_ctnr_idx, i4_offset_x, i4_offset_y);
}


INT32 c_wgl_mbw_update_vp (
    HANDLE_T                h_mbw,
    WGL_MBW_NVGT_INFO_T*    pt_nvgt_info)
{
    return x_wgl_mbw_update_vp(h_mbw, pt_nvgt_info);
}

INT32 c_wgl_mbw_repaint_vp (
    HANDLE_T    h_mbw)
{
    return x_wgl_mbw_repaint_vp(h_mbw);
}

INT32 c_wgl_mbw_reload_static (
    HANDLE_T    h_mbw)
{
    return x_wgl_mbw_reload_static(h_mbw);
}

INT32 c_wgl_mbw_reload_data (
    HANDLE_T                h_mbw)
{
    return x_wgl_mbw_reload_data(h_mbw);
}

INT32 c_wgl_mbw_dm_lock(
    HANDLE_T    h_mbw)
{
    return x_wgl_mbw_dm_lock(h_mbw);
}

INT32 c_wgl_mbw_dm_unlock(
    HANDLE_T    h_mbw)
{
    return x_wgl_mbw_dm_unlock(h_mbw);
}



