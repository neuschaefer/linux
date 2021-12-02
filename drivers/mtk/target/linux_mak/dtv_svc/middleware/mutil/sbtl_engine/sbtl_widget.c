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
 * $RCSfile: sbtl_widget.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description:
 *
 * History:
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#ifndef __KERNEL__
#include "inc/x_common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "dbg/x_dbg.h"

#include "mutil/sbtl_engine/u_mm_sbtl_engine.h"
#include "mutil/sbtl_engine/sbtl_dbg.h"
#include "mutil/sbtl_engine/sbtl_widget.h"
#include "unicode/u_uc_str.h"
#include "unicode/x_uc_str.h"
#else /*__KERNEL__*/
#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "x_dbg.h"
#include "x_lnk_list.h"
#include "x_mm_common.h"

#include "u_mm_sbtl_engine.h"
#include "sbtl_dbg.h"
#include "sbtl_widget.h"
#include "u_uc_str.h"
#endif/*__KERNEL__*/

INT32 sbtl_wgl_register(
    sbtl_wgl_app_callback_fct   pf_callback,
    HANDLE_T*                   ph_client)
{
#ifndef __KERNEL__
    return x_wgl_register(pf_callback, ph_client);
#else
    return MMSBTLR_OK;
#endif
}
    
INT32 sbtl_wgl_set_app_order(HANDLE_T h_thread, UINT8 ui1_order)
{
#ifndef __KERNEL__
    return x_wgl_set_app_order(h_thread, ui1_order);
#else
    return MMSBTLR_OK;
#endif
}

INT32 sbtl_wgl_create_widget(
    HANDLE_T                h_container,
    HANDLE_TYPE_T           e_type_widget,
    UINT32                  ui4_type_border,
    const GL_RECT_T*        pt_rect,
    UINT8                   ui1_alpha,
    VOID*                   pv_param,
    VOID*                   pv_tag,
    HANDLE_T *              ph_widget)
{
#ifndef __KERNEL__
    if (HT_WGL_WIDGET_SBTL == e_type_widget)
    {
        return x_wgl_create_widget(h_container,
                                   e_type_widget,
                                   WGL_CONTENT_SBTL_DEF,
                                   ui4_type_border,
                                   pt_rect,
                                   NULL,
                                   ui1_alpha,
                                   pv_param,
                                   NULL,
                                   ph_widget);

    }
    else if (HT_WGL_WIDGET_TEXT == e_type_widget)
    {
        return x_wgl_create_widget(h_container,
                                   e_type_widget,
                                   WGL_CONTENT_TEXT_DEF,
                                   ui4_type_border,
                                   pt_rect,
                                   NULL,
                                   ui1_alpha,
                                   pv_param,
                                   NULL,
                                   ph_widget);

    }
    else
    {
        return MMSBTLR_NOT_SUPPORT;
    }
    
#else
    return MMSBTLR_OK;
#endif
}

INT32 sbtl_wgl_destroy_widget(HANDLE_T h_widget)
{
#ifndef __KERNEL__
    return x_wgl_destroy_widget(h_widget);
#else
    return MMSBTLR_OK;
#endif
}


INT32 sbtl_wgl_do_cmd(
    HANDLE_T            h_widget,
    UINT32              ui4_cmd, 
    VOID*               pv_param1,
    VOID*               pv_param2)
{
#ifndef __KERNEL__
    return x_wgl_do_cmd(h_widget,
                        ui4_cmd, 
                        pv_param1,
                        pv_param2);
#else
    return MMSBTLR_OK;
#endif
}

INT32 sbtl_wgl_show(
    HANDLE_T            h_widget,
    BOOL                b_show)
{
#ifndef __KERNEL__
    if (b_show)
    {
        return x_wgl_show(h_widget, WGL_SW_NORMAL);
    }
    else
    {
        return x_wgl_show(h_widget, WGL_SW_HIDE);
    }
#else
    return MMSBTLR_OK;
#endif
}

INT32 sbtl_wgl_get_canvas_size(
    HANDLE_T    h_canvas,
    UINT32*     pui4_width,
    UINT32*     pui4_height)
{
#ifndef __KERNEL__
    return x_wgl_get_canvas_size(h_canvas, pui4_width, pui4_height);
#else
    return MMSBTLR_OK;
#endif
}

INT32 sbtl_wgl_move(
    HANDLE_T            h_widget,
    const GL_RECT_T*    pt_rect,
    UINT8               ui1_repaint)
{
#ifndef __KERNEL__
        return x_wgl_move(h_widget, pt_rect, ui1_repaint);
#else
        return MMSBTLR_OK;
#endif

}

INT32 sbtl_wgl_float(
    HANDLE_T            h_widget,
    BOOL                b_float,
    UINT8               ui1_repaint)
{
#ifndef __KERNEL__
        return x_wgl_float(h_widget, b_float, ui1_repaint);
#else
        return MMSBTLR_OK;
#endif
}

INT32 sbtl_multi_byte_to_unicode(
    UINT16*                     pui2_multi_byte, 
    UINT32                      ui4_len, 
    UINT32*                     pui4_cut_pos,
    E_MUL_BYTE_TYPE             e_multi_type, 
    UINT16*                     pui2_unicode,
    UINT32*                     pui4_count)
{
#ifndef __KERNEL__
        return x_multi_byte_to_unicode_ex(pui2_multi_byte, 
                                          ui4_len, 
                                          pui4_cut_pos,
                                          e_multi_type, 
                                          pui2_unicode,
                                          pui4_count);
 
#else
        return MMSBTLR_OK;
#endif
}


