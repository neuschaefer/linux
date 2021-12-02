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
 * $RCSfile: c_wgl_image.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/5 $
 * $SWAuthor: Al Hsieh $
 * $MD5HEX: 5c7f9b779bd3e3428d24b3fb6f494fb5 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#ifdef WGL_ANIM_SUPPORT 
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "handle/handle.h"
#include "aee/aee.h"
#include "wgl/wgl_anim.h"
#include "wgl/wgl_tools.h"
#include "wgl/animation/u_wgl_animation.h"
#include "wgl/animation/x_wgl_animation.h"
#include "wgl/animation/c_wgl_anim.h"

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
/*wgl animation path lib api function*/
INT32 c_wgl_anim_path_init(VOID)
{
    return x_wgl_anim_path_init();
}

INT32 c_wgl_anim_lib_add_path(
    IN WGL_ANIMATION_PATH_MODE_T*     pt_path_mode,
    IN UINT16                         ui2_path_cnt,
    OUT UINT16*                       pui2_added_cnt)
{
    return x_wgl_anim_lib_add_path(pt_path_mode,ui2_path_cnt,pui2_added_cnt);
}

INT32 c_wgl_anim_lib_remove_path(
    IN UINT32*                        pui4_path_id,
    IN UINT16                         ui2_path_cnt)
{
    return x_wgl_anim_lib_remove_path(pui4_path_id,ui2_path_cnt);
}

BOOL c_wgl_anim_lib_get_path(
    IN  UINT32                         ui4_path_id,
    OUT WGL_ANIMATION_PATH_MODE_T*     pt_path_mode)
{
    return x_wgl_anim_lib_get_path(ui4_path_id,pt_path_mode);
}

INT32 c_wgl_anim_lib_get_path_cnt(
        OUT UINT32*                        pui4_count)
{
    return x_wgl_anim_lib_get_path_cnt(pui4_count);
}

INT32 c_wgl_anim_add_default_actmap(
     IN HANDLE_TYPE_T                       e_type_widget,
     IN WGL_ANIM_ACTION_MAP_T*              pt_action_map,
     IN UINT16                              ui2_count)
{
    return x_wgl_anim_add_default_actmap(e_type_widget,pt_action_map,ui2_count);
}

INT32 c_wgl_anim_remove_default_actmap(
    IN HANDLE_TYPE_T                        e_type_widget,
    IN BOOL                                 b_remove_all,
    IN WGL_ANIM_ACTION_TYPE_T               e_anim_action_type)
{
    return x_wgl_anim_remove_default_actmap(e_type_widget,b_remove_all,e_anim_action_type);
}

INT32 c_wgl_anim_get_default_actmap(
  IN  HANDLE_TYPE_T                         e_type_widget,
  IN  WGL_ANIM_ACTION_TYPE_T                e_anim_action_type,
  OUT WGL_ANIM_ACTION_MAP_T**               ppt_action_map)
{
    return x_wgl_anim_get_default_actmap(e_type_widget,e_anim_action_type,ppt_action_map);
}

INT32 c_wgl_set_lb_default_anim(BOOL b_flag)
{
    return x_wgl_set_lb_default_anim(b_flag);
}

BOOL c_wgl_lb_default_anim_en(VOID)
{
    return x_wgl_lb_default_anim_en();
}

#endif


