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
 * $RCSfile: x_gl.h,v $
 * $Revision: #2 $
 * $Date: 2012/06/26 $
 * $Author: xiaokuan.shi $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/DTV_X_ATSC/4 $
 * $SWAuthor: Allen Horng $
 * $MD5HEX: 4c5d80baf0d60eabd36d54a2b8d642f6 $
 *
 * Description: 
 *         This header file contains Graphics Library specific definitions,
 *         which are exported.
 *---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_GL
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _X_GL_H_
#define _X_GL_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_sys_name.h"
#include "u_handle.h"
#include "u_fe.h"
#include "u_gl.h"
#include "x_sm_ttx_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @brief x_gl_obj_free  
 *  use to free any GL created handle
 *  @param  [in]    h_obj           - GL object handle
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                  - ok
 *  @retval GLR_INV_HANDLE          - an invalid handle is passed
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_obj_free(GL_HOBJ_T h_obj);

/*------------------------------------------------------------------*/
/*! @brief  enable/disable screen.
 *  @param  [in]    h_screen           - GL Srceen handle
 *  @param  [in]    b_enable           - enable or disable.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                  - ok
 *  @retval GLR_INV_HANDLE          - an invalid handle is passed
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_screen_enable(
    HANDLE_T    h_screen,
    BOOL        b_enable);
/*------------------------------------------------------------------*/
/*! @brief  get panel size
 *  @param  [in]    h_screen        - screen handle
 *  @param  [out]   pui4_w          - screen width
 *  @param  [out]   pui4_h          - screen height
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                  - ok
 *  @retval GLR_INV_HANDLE          - an invalid handle is passed
 *  @retval GLR_INCOMPATIBLE_OBJ    - incompatible GL object(s) is/are passed
 *  @retval GLR_NOT_INIT            - GL is not initialized
 *  @retval GLR_INV_ARG             - invalid argument is passed
 *  @retval GLR_DRV_ERROR           - driver error
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_screen_get_panel_size(
    GL_HSCREEN_T        h_screen,
    UINT32              *pui4_w,
    UINT32              *pui4_h);

/*------------------------------------------------------------------*/
/*! @brief  get screen status
 *  @param  [in]    h_screen        - screen handle
 *  @param  [out]   pt_scr_status   - screen status
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                  - ok
 *  @retval GLR_NOT_INIT            - GL is not initialized
 *  @retval GLR_INV_ARG             - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ    - incompatible GL object(s) is/are passed
 *  @retval GLR_DRV_ERROR           - driver error
 *  @retval GLR_INV_HANDLE          - an invalid handle is passed
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_screen_get_status(
    GL_HSCREEN_T        h_screen, 
    GL_SCREEN_STATUS_T* pt_scr_status);

/*------------------------------------------------------------------*/
/*! @brief  open a screen
 *  @param  [in]    ps_name         - screen name
 *  @param  [in]    pf_screen_nfy   - screen notification function
 *  @param  [in]    pv_nfy_tag      - user defined tag data
 *  @param  [out]   ph_screen       - screen handle
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                  - ok
 *  @retval GLR_OUT_OF_HANDLES      - out of handles
 *  @retval GLR_NOT_INIT            - GL is not initialized
 *  @retval GLR_INV_ARG             - invalid argument is passed
 *  @retval GLR_NO_MEMORY           - out of memory
 *  @retval GLR_DRV_ERROR           - driver error
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_screen_open(
    const CHAR*         ps_name,
    x_gl_nfy_fct        pf_screen_nfy,
    VOID*               pv_nfy_tag,
    GL_HSCREEN_T*       ph_screen);
/*------------------------------------------------------------------*/
/*! @brief  config OSD scaler
 *  @param  [in]    h_plane         - plane handle
 *  @param  [in]    pt_pln_cfg      - OSD scaler configuration
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                  - ok
 *  @retval GLR_NOT_INIT            - GL is not initialized
 *  @retval GLR_INV_ARG             - invalid argument is passed
 *  @retval GLR_DRV_ERROR           - driver error
 *  @retval GLR_UNSUPPORTED         - unsupported function
 */
/*------------------------------------------------------------------*/

/* plane component API */
extern INT32 x_gl_plane_config(
    HANDLE_T            h_plane,
    GL_PLANE_CFG_T*     pt_pln_cfg);

/*------------------------------------------------------------------*/
/*! @brief  c_gl_plane_num
 *  @param  [out]   pui2_num        - number of OSD plane
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                  - ok
 *  @retval GLR_NOT_INIT            - GL is not initialized
 *  @retval GLR_INV_ARG             - invalid argument is passed
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_plane_num(
    UINT16*             pui2_num);

/*------------------------------------------------------------------*/
/*! @brief  enumerate the plane capability of each OSD plane
 *  @param  [in]    h_screen        - screen handle
 *  @param  [in]    ui2_idx         - plane index
 *  @param  [out]   pt_pln_caps     - plane capability
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                  - ok
 *  @retval GLR_NOT_INIT            - GL is not initialized
 *  @retval GLR_INV_ARG             - invalid argument is passed
 *  @retval GLR_DRV_ERROR           - driver error
 *  @retval GLR_HDL_LIB_ERROR       - handle library error
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_plane_enum(
    GL_HSCREEN_T        h_screen, 
    UINT16              ui2_idx, 
    GL_PLANE_CAPS_T*    pt_pln_caps);

/*------------------------------------------------------------------*/
/*! @brief  open a OSD plane
 *  @param  [in]    h_screen        - screen handle
 *  @param  [in]    ui2_idx         - plane index
 *  @param  [out]   ph_plane        - plane handle
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                  - ok
 *  @retval GLR_NOT_INIT            - GL is not initialized
 *  @retval GLR_INV_ARG             - invalid argument is passed
 *  @retval GLR_DRV_ERROR           - driver error
 *  @retval GLR_INV_HANDLE          - an invalid handle is passed
 *  @retval GLR_OUT_OF_HANDLES      - out of handles
 *  @retval GLR_HDL_LIB_ERROR       - handle library error
 *  @retval GLR_UNLINKABLE          - this plane can not be linked
 *  @retval GLR_INCOMPATIBLE_OBJ    - incompatible GL object(s) is/are passed
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_plane_open(
    GL_HSCREEN_T        h_screen, 
    UINT16              ui2_idx,
    GL_HPLANE_T*        ph_plane);

/*------------------------------------------------------------------*/
/*! @brief  link a plane to a screen
 *  @param  [in]    h_plane             - plane handle
 *  @param  [in]    h_screen            - screen handle
 *  @param  [in]    b_force             - if this plane has been linked to a screen, re-link this plane to the desired screen
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_PLANE_ALREADY_LINKED    - this plane has been linked to a screen
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_plane_link(
    GL_HPLANE_T         h_plane,
    GL_HSCREEN_T        h_screen,
    BOOL                b_forced);

/*------------------------------------------------------------------*/
/*! @brief  reorder the OSD plane
 *  @param  [in]    h_plane             - the OSD plane handle
 *  @param  [in]    e_reorder_type      - define the reorder type
 *  @param  [in]    i4_param            - new plane order
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_PLANE_NOT_LINKED        - this plane has not been linked to a screen
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_plane_reorder(
    GL_HPLANE_T         h_plane, 
    UINT32              e_reorder_type, 
    INT32               i4_param);

/*------------------------------------------------------------------*/
/*! @brief  enable or disable the specified OSD plane
 *  @param  [in]    h_plane             - plane handle
 *  @param  [in]    b_enalble           - enable or disable this plane
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_plane_enable(
    GL_HPLANE_T         h_plane,
    BOOL                b_enable);

extern INT32 x_gl_plane_clear(GL_HPLANE_T         h_plane);
extern INT32 x_gl_plane_change_resolution(
       GL_HPLANE_T         h_plane,
       UINT32              ui4_width,
       UINT32              ui4_height);


extern INT32 x_gl_plane_change_resolution_cm(
        GL_HPLANE_T    h_plane,
        UINT32         ui4_width,
        UINT32         ui4_height,
        GL_COLORMODE_T e_cm);
/*------------------------------------------------------------------*/
/*! @brief  flip a view to a plane
 *  @param  [in]    h_plane             - plane handle
 *  @param  [in]    h_view              - view handle
 *  @param  [in]    b_force             - if a plane has been connected to a view, force this flipping operation to be executed
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_OUT_OF_HANDLES          - out of handles
 *  @retval GLR_FLIP_FAILED             - flipping operation failed
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_plane_flip(
    GL_HPLANE_T     h_plane,
    GL_HVIEW_T      h_view,
    BOOL            b_forced);

/*------------------------------------------------------------------*/
/*! @brief  query the plane capability
 *  @param  [in]    h_plane             - plane handle
 *  @param  [in]    h_screen            - screen handle
 *  @param  [out]   pt_pln_caps         - plane capability
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_plane_get_capability(
    GL_HPLANE_T         h_plane, 
    GL_HSCREEN_T        h_screen,
    GL_PLANE_CAPS_T*    pt_pln_caps);

/*------------------------------------------------------------------*/
/*! @brief  get the linked screen of a specific plane
 *  @param  [in]    h_plane             - plane handle
 *  @param  [in]    pf_screen_nfy       - screen status notification function
 *  @param  [in]    pv_nfy_tag          - use defined tag data
 *  @param  [out]   ph_screen           - linked screen
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_OUT_OF_HANDLES          - out of handles
 *  @retval GLR_NO_MEMORY               - out of memory
 *  @retval GLR_PLANE_NOT_LINKED        - this plane has been linked to a screen
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_plane_get_linked_screen(
    GL_HPLANE_T         h_plane, 
    x_gl_nfy_fct        pf_screen_nfy,
    VOID*               pv_nfy_tag,
    GL_HSCREEN_T*       ph_screen);

/*------------------------------------------------------------------*/
/*! @brief  query the order of a specific plane
 *  @param  [in]    h_plane             - plane handle
 *  @param  [out]   pi4_order           - plane order
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_PLANE_NOT_LINKED        - this plane has been linked to a screen
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_plane_get_order(
    GL_HPLANE_T         h_plane, 
    INT32               *pi4_order);

/*------------------------------------------------------------------*/
/*! @brief  get the status of a specific OSD plane
 *  @param  [in]    h_plane             - plane handle
 *  @param  [out]   pt_pln_sts          - plane status
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_PLANE_NOT_LINKED        - this plane has been linked to a screen
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_plane_get_status(
    GL_HPLANE_T         h_plane, 
    GL_PLANE_STATUS_T*  pt_pln_sts);

/*------------------------------------------------------------------*/
/*! @brief  get a alpha value to a specific OSD plane
 *  @param  [in]    h_plane             - plane handle
 *  @param  [in]    ui1_alpha           - alpha value to be set
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_plane_set_alpha(
    GL_HPLANE_T         h_plane,
    UINT8               ui1_alpha);


extern INT32 x_gl_plane_enable_colorkey(
	GL_HPLANE_T         h_plane,
    BOOL                b_new_enable,
    BOOL*               old_enable_fg);

#ifdef MW_PHOTO_SHOW_ON_VDP
extern INT32 x_gl_plane_get_resource(HANDLE_T h_plane,HANDLE_T h_on_screen_surf);
extern INT32 x_gl_plane_release_resource(HANDLE_T h_plane,HANDLE_T h_on_screen_surf);
extern INT32 x_gl_plane_set_vdp_cropping(
                                                GL_HPLANE_T h_plane, 
                                                BOOL        b_3d_mode,
                                                UINT32      ui4_bottom,
                                                UINT32      ui4_top,
                                                UINT32      ui4_left,
                                                UINT32      ui4_right);
extern INT32 x_gl_surface_create_from_vdp(
		const GL_SURFACE_DESC_T*	pt_surf_desc, 
		GL_HSURFACE_T*				ph_surf);

extern INT32 x_gl_surface_vdp_strech_blend(
    GL_HSURFACE_T       h_surf_dst, 
    GL_HSURFACE_T       h_surf_src_pic, 
    GL_HSURFACE_T       h_surf_src_black,
    UINT8               ui1_alpha);


#endif

/*------------------------------------------------------------------*/
/*! @brief  create a view
 *  @param  [in]    ui4_w               - width of this view
 *  @param  [in]    ui4_h               - height of this view
 *  @param  [in]    b_auto_scale        - if the size of this view is not the same as the screen, scale it or not
 *  @param  [out]   ph_view             - view handle
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_NO_MEMORY               - out of memory
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_OUT_OF_HANDLES          - out of handles
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_view_create(
    UINT32                  ui4_w,
    UINT32                  ui4_h,
    BOOL                    b_auto_scale,
    GL_HVIEW_T*             ph_view);

/*------------------------------------------------------------------*/
/*! @brief  create a viewport
 *  @param  [in]    h_view              - view handle
 *  @param  [in]    h_surface           - surface handle
 *  @param  [in]    pt_disp_opt         - viewport configuration
 *  @param  [out]   pui4_id             - viewport ID
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_OUT_OF_HANDLES          - out of handles
 *  @retval GLR_NO_MEMORY               - out of memory
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_view_create_viewport(
    GL_HVIEW_T              h_view, 
    GL_HSURFACE_T           h_surface, 
    const GL_DISPLAY_OPT_T* pt_disp_opt,
    UINT32*                 pui4_id);

/*------------------------------------------------------------------*/
/*! @brief  delete a viewport
 *  @param  [in]    h_view              - view handle
 *  @param  [in]    ui4_id              - viewport ID
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_HDL_LIB_ERROR           - handle library error
 *  @retval GLR_DRV_ERROR               - driver error
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_view_delete_viewport(
    GL_HVIEW_T      h_view,
    UINT32          ui4_id);

/*------------------------------------------------------------------*/
/*! @brief  reconfigure a viewport setting
 *  @param  [in]    h_plane             - plane handle
 *  @param  [in]    h_view              - view handle
 *  @param  [in]    ui4_viewport_id     - viewport ID
 *  @param  [in]    i4_view_x           - x position of a viewport on the view
 *  @param  [in]    i4_view_y           - y position of a viewport on the view
 *  @param  [in]    ui4_width           - viewport width
 *  @param  [in]    ui4_height          - viewport height
 *  @param  [in]    ui4_surf_offset_x   - x offset on the surface for this viewport
 *  @param  [in]    ui4_surf_offset_y   - y offset on the surface for this viewport
 *  @param  [in]    fx_shrink_x         - shrinking ratio in x direction
 *  @param  [in]    fx_shrink_y         - shrinking ratio in y direction
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_view_reconfig_viewport(
    GL_HPLANE_T     h_plane,
    GL_HVIEW_T      h_view,
    UINT32          ui4_viewport_id,
    INT32           i4_view_x,
    INT32           i4_view_y,
    UINT32          ui4_width,
    UINT32          ui4_height,
    UINT32          ui4_surf_offset_x,
    UINT32          ui4_surf_offset_y,
    GL_FIX16_T      fx_shrink_x,
    GL_FIX16_T      fx_shrink_y);


/*------------------------------------------------------------------*/
/*! @brief  chande surface on graphics
 *  @param  [in]    h_graphics              - graphic handle
 *  @param  [in]    h_surface      - surface handle
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_OUT_OF_HANDLES          - out of handles
 *  @retval GLR_NO_MEMORY               - out of memory
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_change_surface(
    GL_HGRAPHICS_T      h_graphics,
    GL_HSURFACE_T       h_surface);
/*------------------------------------------------------------------*/
/*! @brief  create a graphics
 *  @param  [in]    h_surf              - surface handle
 *  @param  [in]    pt_translation      - surface translation
 *  @param  [in]    pt_clip             - clipper region
 *  @param  [out]   ph_graphics         - graphics handle
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_OUT_OF_HANDLES          - out of handles
 *  @retval GLR_NO_MEMORY               - out of memory
 */
/*------------------------------------------------------------------*/

/* composite graphics API */
extern INT32 x_gl_create_graphics(
    GL_HSURFACE_T       h_surf,
    const GL_POINT_T*   pt_translation,
    const GL_RECT_T*    pt_clip,
    GL_HGRAPHICS_T*     ph_graphics);

/*------------------------------------------------------------------*/
/*! @brief  change the translation of a graphics
 *  @param  [in]    h_graphics          - graphics handle
 *  @param  [in]    i4_trans_x          - translation in x direction
 *  @param  [in]    i4_trans_y          - translation in y direction
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_translate(
    GL_HGRAPHICS_T      h_graphics, 
    INT32               i4_trans_x, 
    INT32               i4_trans_y);

/*------------------------------------------------------------------*/
/*! @brief  set the clipper to a graphics
 *  @param  [in]    h_graphics          - graphics handle
 *  @param  [in]    pt_clip             - clipper region
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_set_clip(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_clip);


/*------------------------------------------------------------------*/
/*! @brief  get the clipper to a graphics
 *  @param  [in]    h_graphics          - graphics handle
 *  @param  [out]    pt_clip             - clipper region
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_get_clip(
    GL_HGRAPHICS_T      h_graphics,
    GL_RECT_T*          pt_clip);
/*------------------------------------------------------------------*/
/*! @brief  set graphics color
 *  @param  [in]    h_graphics          - graphics handle
 *  @param  [in]    pt_color            - graphics color
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 */
/*------------------------------------------------------------------*/
        
extern INT32 x_gl_set_color(
    GL_HGRAPHICS_T      h_graphics,
    const GL_COLOR_T*   pt_color);

/*------------------------------------------------------------------*/
/*! @brief  set the pen width for primitive drawing
 *  @param  [in]    h_graphics          - graphics handle
 *  @param  [in]    ui2_width           - pen width
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_set_pen_width(
    GL_HGRAPHICS_T      h_graphics,
    UINT16              ui2_width);

/*------------------------------------------------------------------*/
/*! @brief  draw a line
 *  @param  [in]    h_graphics          - graphics handle
 *  @param  [in]    pt_start            - starting point
 *  @param  [in]    pt_end              - ending point
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INV_COLORMODE           - invalid colormode
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_draw_line(
    GL_HGRAPHICS_T      h_graphics,
    const GL_POINT_T*   pt_start, 
    const GL_POINT_T*   pt_end);

/*------------------------------------------------------------------*/
/*! @brief  draw a rectangle
 *  @param  [in]    h_graphics          - graphics handle
 *  @param  [in]    pt_bound            - contains the rectangle region
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_draw_rect(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound);
    
/*------------------------------------------------------------------*/
/*! @brief  fill a rectangle
 *  @param  [in]    h_graphics          - graphics handle
 *  @param  [in]    pt_bound            - contains the rectangle region
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_fill_rect(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound);
/*------------------------------------------------------------------*/
/*! @brief  draw a rounded-corner rectangle
 *  @param  [in]    h_graphics          - graphics handle
 *  @param  [in]    pt_bound            - contains the rectangle region
 *  @param  [in]    pt_arc              - contains the arc information for rounded corners
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_draw_round_rect(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound,
    const GL_SIZE_T*    pt_arc);
    
/*------------------------------------------------------------------*/
/*! @brief  fill a rounded-corner rectangle
 *  @param  [in]    h_graphics          - graphics handle
 *  @param  [in]    pt_bound            - contains the rectangle region
 *  @param  [in]    pt_arc              - contains the arc information for rounded corners
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_fill_round_rect(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound,
    const GL_SIZE_T*    pt_arc);

/*------------------------------------------------------------------*/
/*! @brief  draw a polygon
 *  @param  [in]    h_graphics          - graphics handle
 *  @param  [in]    ui4_count           - vertex count of this polygon
 *  @param  [in]    at_vertex           - contains the vertex information for this polygon
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_NO_MEMORY               - out of memory
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_draw_polygon(
    GL_HGRAPHICS_T      h_graphics,
    UINT32              ui4_count, 
    const GL_POINT_T    at_vertex[]);


/*------------------------------------------------------------------*/
/*! @brief  draw a polygon on specify surface.
 *  @param  [in]    h_surf          - surface handle
 *  @param  [in]    ui4_count           - vertex count of this polygon
 *  @param  [in]    at_vertex           - contains the vertex information for this polygon
 *  @param  [in]    pt_color           - color of polygon
 *  @param  [in]    ui2_line_width           - line width of polygon
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_NO_MEMORY               - out of memory
 */
/*------------------------------------------------------------------*/    
extern INT32 x_gl_surface_draw_polyline(
    GL_HSURFACE_T       h_surf,
    UINT32              ui4_count, 
    const GL_POINT_T    at_vertex[],
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    UINT32              ui4_flag);

    
/*------------------------------------------------------------------*/
/*! @brief  fill a polygon
 *  @param  [in]    h_graphics          - graphics handle
 *  @param  [in]    ui4_count           - vertex count of this polygon
 *  @param  [in]    at_vertex           - contains the vertex information for this polygon
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_NO_MEMORY               - out of memory
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_fill_polygon(
    GL_HGRAPHICS_T      h_graphics,
    UINT32              ui4_count, 
    const GL_POINT_T    at_vertex[]);

/*------------------------------------------------------------------*/
/*! @brief  draw an ellipse
 *  @param  [in]    h_graphics          - graphics handle
 *  @param  [in]    pt_bound            - contains the rectangle region for this ellipse
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_NO_MEMORY               - out of memory
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_draw_ellipse(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound);
    
/*------------------------------------------------------------------*/
/*! @brief  fill an ellipse
 *  @param  [in]    h_graphics          - graphics handle
 *  @param  [in]    pt_bound            - contains the rectangle region for this ellipse
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_NO_MEMORY               - out of memory
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_fill_ellipse(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound);

/*------------------------------------------------------------------*/
/*! @brief  draw an arc
 *  @param  [in]    h_graphics          - graphics handle
 *  @param  [in]    pt_bound            - contains the rectangle region for this arc
 *  @param  [in]    i4_deg_start        - the starting degree for this arc
 *  @param  [in]    i4_deg_end          - the ending degree for this arc
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_NO_MEMORY               - out of memory
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_draw_arc(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound,
    INT32               i4_deg_start, 
    INT32               i4_deg_end);

/*------------------------------------------------------------------*/
/*! @brief  fill an arc
 *  @param  [in]    h_graphics          - graphics handle
 *  @param  [in]    pt_bound            - contains the rectangle region for this arc
 *  @param  [in]    i4_deg_start        - the starting degree for this arc
 *  @param  [in]    i4_deg_end          - the ending degree for this arc
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_NO_MEMORY               - out of memory
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_fill_arc(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound,
    INT32               i4_deg_start, 
    INT32               i4_deg_end);

/*------------------------------------------------------------------*/
/*! @brief  draw a pie
 *  @param  [in]    h_graphics          - graphics handle
 *  @param  [in]    pt_bound            - contains the rectangle region for this pie
 *  @param  [in]    i4_deg_start        - the starting degree for this pie
 *  @param  [in]    i4_deg_end          - the ending degree for this pie
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_NO_MEMORY               - out of memory
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_draw_pie(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound,
    INT32               i4_deg_start, 
    INT32               i4_deg_end);

/*------------------------------------------------------------------*/
/*! @brief  fill a pie
 *  @param  [in]    h_graphics          - graphics handle
 *  @param  [in]    pt_bound            - contains the rectangle region for this pie
 *  @param  [in]    i4_deg_start        - the starting degree for this pie
 *  @param  [in]    i4_deg_end          - the ending degree for this pie
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_NO_MEMORY               - out of memory
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_fill_pie(
    GL_HGRAPHICS_T      h_graphics,
    const GL_RECT_T*    pt_bound,
    INT32               i4_deg_start, 
    INT32               i4_deg_end);

/*------------------------------------------------------------------*/
/*! @brief  perform the bit-blitting operation
 *  @param  [in]    h_graphics          - graphics handle (destination)
 *  @param  [in]    h_surf              - surface handle (source)
 *  @param  [in]    i4_dst_x            - x position on the destination graphics
 *  @param  [in]    i4_dst_y            - y position on the destination graphics
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_width           - width of the bit-blitting region
 *  @param  [in]    ui4_height          - height of the bit-blitting region
 *  @param  [in]    ui1_alpha_assigned  - if the source surface is without alpha channel, apply this value as a default alpha on the destination
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_bitblt(
    GL_HGRAPHICS_T      h_graphics,
    GL_HSURFACE_T       h_surf,
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha_assigned);

/*------------------------------------------------------------------*/
/*! @brief  perform the bit-blitting operation with pattern repetition
 *  @param  [in]    h_graphics          - graphics handle (destination)
 *  @param  [in]    h_surf              - surface handle (source)
 *  @param  [in]    i4_dst_x            - x position on the destination graphics
 *  @param  [in]    i4_dst_y            - y position on the destination graphics
 *  @param  [in]    ui4_dst_w           - width of the destination bit-blitting region
 *  @param  [in]    ui4_dst_h           - height of the destination bit-blitting region
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_src_w           - width of the source bit-blitting region
 *  @param  [in]    ui4_src_h           - height of the source bit-blitting region
 *  @param  [in]    ui1_alpha_assigned  - if the source surface is without alpha channel, apply this value as a default alpha on the destination
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_patblt(
    GL_HGRAPHICS_T      h_graphics,
    GL_HSURFACE_T       h_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    UINT8               ui1_alpha_assigned);

/*------------------------------------------------------------------*/
/*! @brief  copies the content of the source surface to the target graphics with source color key specified
 *  @param  [in]    h_graphics          - graphics handle (destination)
 *  @param  [in]    h_surf              - surface handle (source)
 *  @param  [in]    i4_dst_x            - x position on the destination graphics
 *  @param  [in]    i4_dst_y            - y position on the destination graphics
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_width           - width of the bit-blitting region
 *  @param  [in]    ui4_height          - height of the bit-blitting region
 *  @param  [in]    ui1_alpha_assigned  - if the source surface is without alpha channel, apply this value as a default alpha on the destination
 *  @param  [in]    b_off_mask          - contains the flag to indicate if an off-mask bit-blitting should be performed. If this field is TRUE, only pixel data outside the specified color key range are copied, otherwise pixel data within the range are blitted
 *  @param  [in]    pt_color_min        - the lower bound of the specified source color key range
 *  @param  [in]    pt_color_max        - the upper bound of the specified source color key range
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_transp_blt(
    GL_HGRAPHICS_T      h_graphics,
    GL_HSURFACE_T       h_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha_assigned,
    BOOL                b_off_mask,
    const GL_COLOR_T*   pt_color_min,
    const GL_COLOR_T*   pt_color_max);

extern INT32 x_gl_get_surface_on_graphice(
	GL_HGRAPHICS_T      h_graphics,
	GL_HSURFACE_T*      ph_surf);

/*------------------------------------------------------------------*/
/*! @brief  copies the content of the source surface to another surface
 *  @param  [in]    h_dst_surf          - destination surface handle (destination)
 *  @param  [in]    h_surf              - surface handle (source)
 *  @param  [in]    i4_dst_x            - x position on the destination graphics
 *  @param  [in]    i4_dst_y            - y position on the destination graphics
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_width           - width of the bit-blitting region
 *  @param  [in]    ui4_height          - height of the bit-blitting region
 *  @param  [in]    e_alignment        - bytes alignment format: GL_ALIGNMENT_MSB = 0, GL_ALIGNMENT_LSB = 1
 *  @param  [in]    h_palette          - palette table handle.
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see 
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_surface_pltblt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    GL_BYTE_ALIGNMENT_T e_alignment,
    GL_HPALETTE_T       h_palette,
    UINT32              ui4_flag);
/*------------------------------------------------------------------*/
/*! @brief  copies the content of the source surface to the target graphics with source color key specified and pattern repetition
 *  @param  [in]    h_graphics          - graphics handle (destination)
 *  @param  [in]    h_surf              - surface handle (source)
 *  @param  [in]    i4_dst_x            - x position on the destination graphics
 *  @param  [in]    i4_dst_y            - y position on the destination graphics
 *  @param  [in]    ui4_dst_w           - width of the destination bit-blitting region
 *  @param  [in]    ui4_dst_h           - height of the destination bit-blitting region
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_src_w           - width of the source bit-blitting region
 *  @param  [in]    ui4_src_h           - height of the source bit-blitting region
 *  @param  [in]    ui1_alpha_assigned  - if the source surface is without alpha channel, apply this value as a default alpha on the destination
 *  @param  [in]    b_off_mask          - contains the flag to indicate if an off-mask bit-blitting should be performed. If this field is TRUE, only pixel data outside the specified color key range are copied, otherwise pixel data within the range are blitted
 *  @param  [in]    pt_color_min        - the lower bound of the specified source color key range
 *  @param  [in]    pt_color_max        - the upper bound of the specified source color key range
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_transp_patblt(
    GL_HGRAPHICS_T      h_graphics,
    GL_HSURFACE_T       h_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    UINT8               ui1_alpha_assigned,
    BOOL                b_off_mask,
    const GL_COLOR_T*   pt_color_min,
    const GL_COLOR_T*   pt_color_max);

/*------------------------------------------------------------------*/
/*! @brief  stretch the source surface into the destination graphics
 *  @param  [in]    h_graphcis          - destination graphics handle
 *  @param  [in]    h_surf              - source surface handle
 *  @param  [in]    i4_dst_x            - x position on the destination graphics
 *  @param  [in]    i4_dst_y            - y position on the destination graphics
 *  @param  [in]    ui4_dst_w           - width of the destination region
 *  @param  [in]    ui4_dst_h           - height of the destination region
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_src_w           - width of the source region
 *  @param  [in]    ui4_src_h           - height of the source region
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_stretch_blt(
    GL_HGRAPHICS_T      h_graphics,
    GL_HSURFACE_T       h_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h);


/*------------------------------------------------------------------*/
/*! @brief  stretch the source surface into the destination graphics (advance function)
 *  @param  [in]    h_graphcis          - destination graphics handle
 *  @param  [in]    h_surf              - source surface handle
 *  @param  [in]    i4_dst_x            - x position on the destination graphics
 *  @param  [in]    i4_dst_y            - y position on the destination graphics
 *  @param  [in]    ui4_dst_w           - width of the destination region
 *  @param  [in]    ui4_dst_h           - height of the destination region
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_src_w           - width of the source region
 *  @param  [in]    ui4_src_h           - height of the source region
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_adv_stretch_blt(
    GL_HGRAPHICS_T      h_graphics,
    GL_HSURFACE_T       h_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h);

/*------------------------------------------------------------------*/
/*! @brief  stretch the source surface into the destination graphics (use image resize drv.)
 *  @param  [in]    h_graphcis          - destination graphics handle
 *  @param  [in]    h_surf              - source surface handle
 *  @param  [in]    i4_dst_x            - x position on the destination graphics
 *  @param  [in]    i4_dst_y            - y position on the destination graphics
 *  @param  [in]    ui4_dst_w           - width of the destination region
 *  @param  [in]    ui4_dst_h           - height of the destination region
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_src_w           - width of the source region
 *  @param  [in]    ui4_src_h           - height of the source region
 *  @param  [in]    e_option           - option for stretching algorithm
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_adv_stretch_blt_ex(
    GL_HGRAPHICS_T      h_graphics,
    GL_HSURFACE_T       h_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    GL_STRETCH_OPTION   e_option);

/*------------------------------------------------------------------*/
/*! @brief  blend the source surface onto the destination graphics
 *  @param  [in]    h_graphics          - destination graphics handle
 *  @param  [in]    h_surf              - source surface handle
 *  @param  [in]    i4_dst_x            - x position on the destination graphics
 *  @param  [in]    i4_dst_y            - y position on the destination graphics
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_width           - width of the blending region
 *  @param  [in]    ui4_height          - height of the blending region
 *  @param  [in]    ui1_alpha           - blending level
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_blend(
    GL_HGRAPHICS_T      h_graphics,
    GL_HSURFACE_T       h_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha);

/*------------------------------------------------------------------*/
/*! @brief  compose the source surface onto the destination graphics
 *  @param  [in]    h_graphics          - destination graphics handle
 *  @param  [in]    h_surf              - source surface handle
 *  @param  [in]    i4_dst_x            - x position on the destination graphics
 *  @param  [in]    i4_dst_y            - y position on the destination graphics
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_width           - width of the composing region
 *  @param  [in]    ui4_height          - height of the composing region
 *  @param  [in]    ui1_pre_alpha       - an alpha value which would be multiplied to each pixel of the source surface
 *  @param  [in]    e_rule              - Porter-Duff composition rule
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_compose(
    GL_HGRAPHICS_T      h_graphics,
    GL_HSURFACE_T       h_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_pre_alpha,
    GL_COMPOSE_RULE_T   e_rule);

/*------------------------------------------------------------------*/
/*! @brief  compose the source surface onto the destination graphics with pattern repetition
 *  @param  [in]    h_graphics          - destination graphics handle
 *  @param  [in]    h_surf              - source surface handle
 *  @param  [in]    i4_dst_x            - x position on the destination graphics
 *  @param  [in]    i4_dst_y            - y position on the destination graphics
 *  @param  [in]    ui4_dst_w           - width of the destination composing region
 *  @param  [in]    ui4_dst_h           - height of the destination composing region
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_src_w           - width of the source composing region
 *  @param  [in]    ui4_src_h           - height of the source composing region
 *  @param  [in]    ui1_pre_alpha       - an alpha value which would be multiplied to each pixel of the source surface
 *  @param  [in]    e_rule              - Porter-Duff composition rule
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_pat_compose(
    GL_HGRAPHICS_T      h_graphics,
    GL_HSURFACE_T       h_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    UINT8               ui1_pre_alpha,
    GL_COMPOSE_RULE_T   e_rule);


/*------------------------------------------------------------------*/
/*! @brief  compose the color onto the destination graphics with pattern repetition
 *  @param  [in]    h_graphics          - destination graphics handle
 *  @param  [in]    pt_color              - source color
 *  @param  [in]    i4_dst_x            - x position on the destination graphics
 *  @param  [in]    i4_dst_y            - y position on the destination graphics
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_width           - width of the composing region
 *  @param  [in]    ui4_height           - height of the composing region
 *  @param  [in]    e_rule              - Porter-Duff composition rule
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_color_compose(
    GL_HGRAPHICS_T      h_graphics,
    const GL_COLOR_T*   pt_color,
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    GL_COMPOSE_RULE_T   e_rule);

/*------------------------------------------------------------------*/
/*! @brief  draw a string
 *  @param  [in]    h_graphics          - destination graphics handle
 *  @param  [in]    i4_x                - x position on the destination graphics
 *  @param  [in]    i4_y                - y position on the destination graphics
 *  @param  [in]    i4_bg_y             - y position of the background on the destination graphics
 *  @param  [in]    w2s_str             - pointer to the string to be drawn
 *  @param  [in]    ui2_strlen          - string length of w2s_str
 *  @param  [in]    w2s_skip            - pointer to the string which contains characters to be skipped in w2s_str
 *  @param  [in]    ui2_skiplen         - string length of w2s_skip
 *  @param  [in]    h_fnt               - font handle
 *  @param  [in]    pt_fg_clr           - foreground color
 *  @param  [in]    pt_bg_clr           - background color
 *  @param  [in]    pt_edge_clr         - edge color
 *  @param  [in]    b_fixed             - indicates if the interval between characters is fixed
 *  @param  [in]    ui2_ch_w            - when b_fixed is TRUE, this indicates what the interval is
 *  @param  [in]    ui2_bg_h            - indicates the height of the background
 *  @param  [in]    i4_bg_adj_x         - indicates the offset of background in the x direction
 *  @param  [in]    e_draw_str_mode         - specify the string drawing mode
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 *  @retval GLR_FE_ERROR                - font library error
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_draw_ustring(
    GL_HGRAPHICS_T      h_graphics,
    INT32               i4_x,
    INT32               i4_y,
    INT32               i4_bg_y,
    UTF16_T*            w2s_str,
    UINT16              ui2_strlen,
    UTF16_T*            w2s_skip,
    UINT16              ui2_skiplen,
    HFONT_T             h_fnt,
    const GL_COLOR_T*   pt_fg_clr,
    const GL_COLOR_T*   pt_bg_clr,
    const GL_COLOR_T*   pt_edge_clr,
    BOOL                b_fixed,
    UINT16              ui2_ch_w,
    UINT16              ui2_bg_h,
    INT32               i4_bg_adj_x,
    GL_DRAW_STR_MODE_T  e_draw_str_mode);

/*------------------------------------------------------------------*/
/*! @brief  draw a string
 *  @param  [in]    h_graphics          - destination graphics handle
 *  @param  [in]    i4_x                - x position on the destination graphics
 *  @param  [in]    i4_y                - y position on the destination graphics
 *  @param  [in]    i4_bg_y             - y position of the background on the destination graphics
 *  @param  [in]    w2s_str             - pointer to the string to be drawn
 *  @param  [in]    ui2_strlen          - string length of w2s_str
 *  @param  [in]    w2s_skip            - pointer to the string which contains characters to be skipped in w2s_str
 *  @param  [in]    ui2_skiplen         - string length of w2s_skip
 *  @param  [in]    h_fnt               - font handle
 *  @param  [in]    pt_fg_clr           - foreground color
 *  @param  [in]    pt_bg_clr           - background color
 *  @param  [in]    pt_edge_clr         - edge color
 *  @param  [in]    b_fixed             - indicates if the interval between characters is fixed
 *  @param  [in]    ui2_ch_w            - when b_fixed is TRUE, this indicates what the interval is
 *  @param  [in]    ui2_bg_h            - indicates the height of the background
 *  @param  [in]    i4_bg_adj_x         - indicates the offset of background in the x direction
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 *  @retval GLR_FE_ERROR                - font library error
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_draw_ustring_ex(
    GL_HGRAPHICS_T      h_graphics,
    INT32               i4_x,
    INT32               i4_y,
    INT32               i4_bg_y,
    VOID*               pv_str,
    UINT16              ui2_strlen,
    VOID*               pv_skip,
    UINT16              ui2_skiplen,
    HFONT_T             h_fnt,
    const GL_COLOR_T*   pt_fg_clr,
    const GL_COLOR_T*   pt_bg_clr,
    const GL_COLOR_T*   pt_edge_clr,
    BOOL                b_fixed,
    UINT16              ui2_ch_w,
    UINT16              ui2_skip_w,
    UINT16              ui2_bg_h,
    INT32               i4_bg_adj_x);


/*------------------------------------------------------------------*/
/*! @brief  draw a string for ttx.
 *  @param  [in]    h_graphics          - destination graphics handle
 *  @param  [in]    i4_x                - x position on the destination graphics
 *  @param  [in]    i4_y                - y position on the destination graphics
 *  @param  [in]    at_drcs_str             - PTU structure array.
 *  @param  [in]    ui2_str_len             - count of PTU structure array.
 *  @param  [in]    ui1_width          - character width
 *  @param  [in]    ui1_height            - character height.
 *  @param  [in]    pt_info         - describe information related to a DRCS string
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 *  @retval GLR_FE_ERROR                - font library error
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_draw_drcs_string(
    GL_HGRAPHICS_T          h_graphics,
    INT32                   i4_x,
    INT32                   i4_y,
    SM_TTX_WGL_DRCS_PTU_T   at_drcs_str[],
    UINT16                  ui2_str_len,
    UINT8                   ui1_width,
    UINT8                   ui1_height,
    GL_DRCS_INFO_T*         pt_info);

/*------------------------------------------------------------------*/
/*! @brief  draw a string for close caption
 *  @param  [in]    h_graphics          - destination graphics handle
 *  @param  [in]    i4_x                - x position on the destination graphics
 *  @param  [in]    i4_y                - y position on the destination graphics
 *  @param  [in]    i4_bg_y             - y position of the background on the destination graphics
 *  @param  [in]    pv_str             - pointer to the string to be drawn
 *  @param  [in]    ui2_strlen          - string length of w2s_str
 *  @param  [in]    pv_skip            - pointer to the string which contains characters to be skipped in w2s_str
 *  @param  [in]    ui2_skiplen         - string length of w2s_skip
 *  @param  [in]    ui2_skip_line_len         - line length of skip character.
 *  @param  [in]    h_fnt               - font handle
 *  @param  [in]    pt_fg_clr           - foreground color
 *  @param  [in]    pt_bg_clr           - background color
 *  @param  [in]    pt_edge_clr         - edge color
 *  @param  [in]    b_fixed             - indicates if the interval between characters is fixed
 *  @param  [in]    ui2_ch_w            - when b_fixed is TRUE, this indicates what the interval is
 *  @param  [in]    ui2_skip_w            - when b_fixed is TRUE, this indicates what the interval is
 *  @param  [in]    ui2_bg_h            - indicates the height of the background
 *  @param  [in]    i4_bg_adj_x         - indicates the offset of background in the x direction
 *  @param  [in]    e_draw_str_mode         - specify the string drawing mode
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 *  @retval GLR_FE_ERROR                - font library error
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_draw_ustring_skip_underline(
    GL_HGRAPHICS_T      h_graphics,
    INT32               i4_x,
    INT32               i4_y,
    INT32               i4_bg_y,
    VOID*               pv_str,
    UINT16              ui2_strlen,
    VOID*               pv_skip,
    UINT16              ui2_skiplen,
    VOID*               pv_skip_line,
    UINT16              ui2_skip_line_len,
    HFONT_T             h_fnt,
    const GL_COLOR_T*   pt_fg_clr,
    const GL_COLOR_T*   pt_bg_clr,
    const GL_COLOR_T*   pt_edge_clr,
    BOOL                b_fixed,
    UINT16              ui2_ch_w,
    UINT16              ui2_skip_w,
    UINT16              ui2_bg_h,
    INT32               i4_bg_adj_x,
    GL_DRAW_STR_MODE_T  e_draw_str_mode);

/*------------------------------------------------------------------*/
/*! @brief  draw a sub string
 *  @param  [in]    h_graphics          - destination graphics handle
 *  @param  [in]    i4_x                - x position on the destination graphics
 *  @param  [in]    i4_y                - y position on the destination graphics
 *  @param  [in]    i4_bg_y             - y position of the background on the destination graphics
 *  @param  [in]    w2s_str             - pointer to the string to be drawn
 *  @param  [in]    ui2_strlen          - string length of w2s_str
 *  @param  [in]    w2s_skip            - pointer to the string which contains characters to be skipped in w2s_str
 *  @param  [in]    ui2_skiplen         - string length of w2s_skip
 *  @param  [in]    h_fnt               - font handle
 *  @param  [in]    pt_fg_clr           - foreground color
 *  @param  [in]    b_fixed             - indicates if the interval between characters is fixed
 *  @param  [in]    ui2_ch_w            - when b_fixed is TRUE, this indicates what the interval is
 *  @param  [in]    ui2_skip_w            - when b_fixed is TRUE, this indicates what the interval is
 *  @param  [in]    ui2_index_num            - index of the characater in string
 *  @param  [in]    aui2_index         - array of index in string.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 *  @retval GLR_FE_ERROR                - font library error
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_draw_subset_ustring(
    GL_HGRAPHICS_T      h_graphics,
    INT32               i4_x,
    INT32               i4_y,
    UTF16_T*            w2s_str,
    UINT16              ui2_strlen,
    UTF16_T*            w2s_skip,
    UINT16              ui2_skiplen,
    HFONT_T             h_fnt,
    const GL_COLOR_T*   pt_fg_clr,
    BOOL                b_fixed,
    UINT16              ui2_ch_w,
    UINT16              ui2_skip_w,
    UINT16              ui2_index_num,
    UINT16*             aui2_index);

/*------------------------------------------------------------------*/
/*! @brief  draw string for ttx.
 *  @param  [in]    h_graphics          - destination graphics handle
 *  @param  [in]    i4_x                - x position on the destination graphics
 *  @param  [in]    i4_y                - y position on the destination graphics
 *  @param  [in]    i4_bg_y             - y position of the background on the destination graphics
 *  @param  [in]    w2s_str             - pointer to the string to be drawn
 *  @param  [in]    ui2_strlen          - string length of w2s_str
 *  @param  [in]    h_fnt               - font handle
 *  @param  [in]    pt_ttx_clr           - foreground and background colors for rendering teletext a string
 *  @param  [in]    e_ttx_size             - double width or double height TTX size
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 *  @retval GLR_FE_ERROR                - font library error
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_draw_ttx_string(
    GL_HGRAPHICS_T      h_graphics,
    INT32               i4_x,
    INT32               i4_y,
    UTF16_T*            w2s_str,
    UINT16              ui2_strlen,
    HFONT_T             h_fnt,
    GL_TTX_COLOR_T*     pt_ttx_clr,
    GL_TTX_STR_SIZE_T   e_ttx_size);


/*------------------------------------------------------------------*/
/*! @brief  init draw string for isdb.
 *  @param  [in]    pt_buf          - string buffer
 *  @param  [in]    ui2_max_width                - max width of one line
 *  @param  [in]    ui2_max_height                -max height of one line
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 *  @retval GLR_FE_ERROR                - font library error
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_isdb_draw_init(
    unsigned char*      pt_buf, 
    UINT16              ui2_max_width, 
    UINT16              ui2_max_height );


/*------------------------------------------------------------------*/
/*! @brief  get character size for isdb.
 *  @param  [in]    ui2_char_code          -character code
 *  @param  [in]    ui2_char_w                - width of font.
 *  @param  [in]    ui2_char_h                -height of font.
 *  @param  [out]    pui2_act_width                -width of character.
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 *  @retval GLR_FE_ERROR                - font library error
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_isdb_get_char_width(
    UINT16              ui2_char_code,
    UINT16              ui2_char_w,
    UINT16              ui2_char_h,
    UINT16*             pui2_act_width );

/*------------------------------------------------------------------*/
/*! @brief  draw string for isdb.
 *  @param  [in]    h_character_surf          -destination surface handle
 *  @param  [in]    e_clrmode                - color mode of destination surface.
 *  @param  [in]    ui2_char_code                -character for draw.
 *  @param  [in]    pt_grid                -describe the position and size of a isdb character block
 *  @param  [in]    pt_char_color                -foreground color
 *  @param  [in]    fg_is_equal_width                -equal width or not
 *  @param  [in]    pui2_grid_act_width                -width of character block
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 *  @retval GLR_FE_ERROR                - font library error
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_isdb_draw_char(
    GL_HSURFACE_T       h_character_surf,
    GL_COLORMODE_T      e_clrmode,
    UINT16              ui2_char_code,
    GL_ISDB_GRID_T*     pt_grid,
    GL_COLOR_T*         pt_char_color,
    GL_COLOR_T*         pt_ORN_color,
    BOOL                fg_is_equal_width,
    UINT16*             pui2_grid_act_width );
/*------------------------------------------------------------------*/
/*! @brief  get the GPU capability
 *  @param  [out]   pt_caps             - GPU capability
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_DRV_ERROR               - driver error
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_get_gpu_capability(
    GL_GPU_CAPS_T       *pt_caps);

/*------------------------------------------------------------------*/
/*! @brief  create a surface
 *  @param  [in]    pt_surf_desc        - surface description
 *  @param  [out]   ph_surf             - surface handle
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_OUT_OF_HANDLES          - out of handles
 *  @retval GLR_NO_MEMORY               - out of memory
 *  @retval GLR_INV_COLORMODE           - invalid colormode
 */
/*------------------------------------------------------------------*/
/* surface component API */
extern INT32 x_gl_surface_create(
    const GL_SURFACE_DESC_T*    pt_surf_desc, 
    GL_HSURFACE_T*              ph_surf);

/*------------------------------------------------------------------*/
/*! @brief  create a surface from FBM
 *  @param  [in]    pt_surf_desc        - surface description
 *  @param  [out]   ph_surf             - surface handle
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_OUT_OF_HANDLES          - out of handles
 *  @retval GLR_NO_MEMORY               - out of memory
 *  @retval GLR_INV_COLORMODE           - invalid colormode
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_create_from_fbm(
    const GL_SURFACE_DESC_T*    pt_surf_desc, 
    GL_HSURFACE_T*              ph_surf);

/*------------------------------------------------------------------*/
/*! @brief  lock a surface to acquire its related information
 *  @param  [in]    h_surf              - surface handle
 *  @param  [in]    pt_rect             - region to be locked. NULL should be passed for now to lock all surface region
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @param  [out]   pt_lock_info        - surface lock information
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_lock(
    GL_HSURFACE_T       h_surf, 
    const GL_RECT_T*    pt_rect, 
    UINT32              ui4_flag, 
    GL_LOCK_INFO_T*     pt_lock_info);

/*------------------------------------------------------------------*/
/*! @brief  unlock a surface
 *  @param  [in]    h_surf              - surface handle
 *  @param  [in]    pt_rect             - region to be locked. NULL should be passed for now to lock all surface region
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_NOT_LOCKED      - this surface is not locked
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_unlock(
    GL_HSURFACE_T       h_surf, 
    const GL_RECT_T*    pt_rect);

/*------------------------------------------------------------------*/
/*! @brief  get surface information
 *  @param  [in]    h_surf              - surface handle
 *  @param  [out]   pt_surf_info        - surface information
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_OUT_OF_HANDLES          - out of handles
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_get_info(
    GL_HSURFACE_T       h_surf, 
    GL_SURF_INFO_T*     pt_surf_info);

/*------------------------------------------------------------------*/
/*! @brief  perform the bit-blitting operation with additional functionality (alpha/color value/map)
 *  @param  [in]    h_dst_surf          - destination surface handle
 *  @param  [in]    h_src_surf          - source surface handle
 *  @param  [in]    i4_dst_x            - x position on the destination surface
 *  @param  [in]    i4_dst_y            - y position on the destination surface
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_width           - width of the bit-blitting region
 *  @param  [in]    ui4_height          - height of the bit-blitting region
 *  @param  [in]    pt_blt_ex           - data used for performing additional functions
 *  @param  [in]    ui4_blt_ex_opt      - indicates which operation to be performed (BLT_EX_OPT_ALPHAVAL_COLORMAP_REP, BLT_EX_OPT_ALPHAVAL_COLORMAP_MUL, BLT_EX_OPT_ALPHAMAP_COLORVAL, BLT_EX_OPT_ALPHAMAP_COLORMAP)
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INV_COLORMODE           - invalid colormode
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_blt_ex(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    const GL_BLT_EX_T*  pt_blt_ex,
    UINT8               ui1_blt_ex_opt,
    UINT32              ui4_flag);
    
/*------------------------------------------------------------------*/
/*! @brief  perform the bit-blitting operation
 *  @param  [in]    h_dst_surf          - destination surface handle
 *  @param  [in]    h_src_surf          - source surface handle
 *  @param  [in]    i4_dst_x            - x position on the destination surface
 *  @param  [in]    i4_dst_y            - y position on the destination surface
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_width           - width of the bit-blitting region
 *  @param  [in]    ui4_height          - height of the bit-blitting region
 *  @param  [in]    ui1_alpha_assigned  - if the source surface is without alpha channel, apply this value as a default alpha on the destination
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_blt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha_assigned,
    UINT32              ui4_flag);

    
/*------------------------------------------------------------------*/
/*! @brief  perform the bit-blitting operation with ROP type.
 *  @param  [in]    h_dst_surf          - destination surface handle
 *  @param  [in]    h_src_surf          - source surface handle
 *  @param  [in]    i4_dst_x            - x position on the destination surface
 *  @param  [in]    i4_dst_y            - y position on the destination surface
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_width           - width of the bit-blitting region
 *  @param  [in]    ui4_height          - height of the bit-blitting region
 *  @param  [in]    e_rop_type  - specify the ROP type
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_surface_rop_blt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    GL_ROP_TYPE_T       e_rop_type,
    UINT32              ui4_flag);

    
/*------------------------------------------------------------------*/
/*! @brief  converter ycbcr surface to rgb.
 *  @param  [in]    h_dst_surf          - destination surface handle
 *  @param  [in]    h_y_surf          - source y surface handle
 *  @param  [in]    h_cbcr_surf          - source cbcr surface handle
 *  @param  [in]    i4_dst_x            - x position on the destination surface
 *  @param  [in]    i4_dst_y            - y position on the destination surface
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_width           - width of the bit-blitting region
 *  @param  [in]    ui4_height          - height of the bit-blitting region
 *  @param  [in]    ui1_alpha_assigned  - if the source surface is without alpha channel, apply this value as a default alpha on the destination
 *  @param  [in]    e_ycbcr_mode            - specify the YCbCr format
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_surface_ycbcr_to_rgb(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_y_surf,
    GL_HSURFACE_T       h_cbcr_surf,
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha_assigned,
    GL_YCBCR_MODE_T     e_ycbcr_mode,
    UINT32              ui4_flag);

    
/*------------------------------------------------------------------*/
/*! @brief  converter ycbcr surface to rgb.
 *  @param  [in]    h_dst_surf          - destination surface handle
 *  @param  [in]    h_y_surf          - source y surface handle
 *  @param  [in]    h_cb_surf          - source cb surface handle
 *  @param  [in]    h_cr_surf          - source cr surface handle
 *  @param  [in]    i4_dst_x            - x position on the destination surface
 *  @param  [in]    i4_dst_y            - y position on the destination surface
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_width           - width of the bit-blitting region
 *  @param  [in]    ui4_height          - height of the bit-blitting region
 *  @param  [in]    ui1_alpha_assigned  - if the source surface is without alpha channel, apply this value as a default alpha on the destination
 *  @param  [in]    GL_JPEG_MODE_T            - specify the JPEG YUV mode
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_surface_ycbcr_to_rgb_3buf(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_y_surf,
    GL_HSURFACE_T       h_cb_surf,
    GL_HSURFACE_T       h_cr_surf,
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha_assigned,
    GL_JPEG_MODE_T      e_jpeg_mode,
    UINT32              ui4_flag);

/*------------------------------------------------------------------*/
/*! @brief  draw a line on a surface
 *  @param  [in]    h_surf              - surface handle
 *  @param  [in]    pt_start            - starting point
 *  @param  [in]    pt_end              - ending point
 *  @param  [in]    pt_color            - line color
 *  @param  [in]    ui2_line_width      - line width
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INV_COLORMODE           - invalid colormode
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_draw_line(
    GL_HSURFACE_T       h_surf,
    const GL_POINT_T*   pt_start, 
    const GL_POINT_T*   pt_end,
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    UINT32              ui4_flag);

/*------------------------------------------------------------------*/
/*! @brief  draw a rectangle on a surface
 *  @param  [in]    h_surface           - surface handle
 *  @param  [in]    pt_rect             - contains the rectangle region
 *  @param  [in]    pt_color            - rectangle color
 *  @param  [in]    ui2_line_width      - line width
 *  @param  [in]    t_style             - rectangle style. It could be GL_STYLE_OUTLINED or GL_STYLE_FILLED
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_draw_rect(
    GL_HSURFACE_T       h_surf,
    const GL_RECT_T*    pt_rect,
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    GL_STYLE_T          t_style,
    UINT32              ui4_flag);


/*------------------------------------------------------------------*/
/*! @brief  query the size of a surface in bytes
 *  @param  [in]    h_surf              - surface handle
 *  @param  [out]   pui4_max_leng       - surface size in bytes
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_get_bitmap_max_leng(
    GL_HSURFACE_T       h_surf,
    UINT32*             pui4_max_leng);
    
/*------------------------------------------------------------------*/
/*! @brief  draw a rounded-corner rectangle on a surface
 *  @param  [in]    h_surf              - surface handle
 *  @param  [in]    pt_rect             - contains the rectangle region
 *  @param  [in]    pt_arc              - contains the arc information for rounded corners
 *  @param  [in]    pt_color            - rectangle color
 *  @param  [in]    ui2_line_width      - line width
 *  @param  [in]    t_style             - rectangle style. It could be GL_STYLE_OUTLINED or GL_STYLE_FILLED
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_draw_round_rect(
    GL_HSURFACE_T           h_surf,
    const GL_RECT_T*        pt_rect,
    const GL_ARC_DESC_T*    pt_arc,
    const GL_COLOR_T*       pt_color,
    UINT16                  ui2_line_width,
    GL_STYLE_T              t_style,
    UINT32                  ui4_flag);

/*------------------------------------------------------------------*/
/*! @brief  draw an ellipse on a surface
 *  @param  [in]    h_surf              - surface handle
 *  @param  [in]    pt_rect             - contains the rectangle region for this ellipse
 *  @param  [in]    pt_color            - ellipse color
 *  @param  [in]    ui2_line_width      - line width
 *  @param  [in]    t_style             - ellipse style. It could be GL_STYLE_OUTLINED or GL_STYLE_FILLED
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_NO_MEMORY               - out of memory
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_draw_ellipse(
    GL_HSURFACE_T       h_surf,
    const GL_RECT_T*    pt_rect,
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    GL_STYLE_T          t_style,
    UINT32              ui4_flag);

/*------------------------------------------------------------------*/
/*! @brief  draw a polygon on a surface
 *  @param  [in]    h_surf              - surface handle
 *  @param  [in]    ui4_count           - vertex count of this polygon
 *  @param  [in]    at_vertex           - contains the vertex information for this polygon
 *  @param  [in]    pt_color            - polygon color
 *  @param  [in]    ui2_line_width      - line width
 *  @param  [in]    t_style             - polygon style. It could be GL_STYLE_OUTLINED or GL_STYLE_FILLED
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_NO_MEMORY               - out of memory
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_draw_polygon(
    GL_HSURFACE_T       h_surf,
    UINT32              ui4_count, 
    const GL_POINT_T    at_vertex[],
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    GL_STYLE_T          t_style,
    UINT32              ui4_flag);

/*------------------------------------------------------------------*/
/*! @brief  draw an arc on a surface
 *  @param  [in]    h_surf              - surface handle
 *  @param  [in]    pt_rect             - contains the rectangle region for this arc
 *  @param  [in]    i4_deg_start        - the starting degree for this arc
 *  @param  [in]    i4_deg_end          - the ending degree for this arc
 *  @param  [in]    pt_color            - arc color
 *  @param  [in]    ui2_line_width      - line width
 *  @param  [in]    t_style             - arc style. It could be GL_STYLE_OUTLINED or GL_STYLE_FILLED
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_NO_MEMORY               - out of memory
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_draw_arc(
    GL_HSURFACE_T       h_surf,
    const GL_RECT_T*    pt_rect,
    INT32               i4_deg_start, 
    INT32               i4_deg_end,
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    GL_STYLE_T          t_style,
    UINT32              ui4_flag);

/*------------------------------------------------------------------*/
/*! @brief  draw a pie on a surface
 *  @param  [in]    h_surface           - surface handle
 *  @param  [in]    pt_rect             - contains the rectangle region for this pie
 *  @param  [in]    i4_deg_start        - the starting degree for this pie
 *  @param  [in]    i4_deg_end          - the ending degree for this pie
 *  @param  [in]    pt_color            - pie color
 *  @param  [in]    ui2_line_width      - line width
 *  @param  [in]    t_style             - pie style. It could be GL_STYLE_OUTLINED or GL_STYLE_FILLED
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_NO_MEMORY               - out of memory
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_draw_pie(
    GL_HSURFACE_T       h_surf,
    const GL_RECT_T*    pt_rect,
    INT32               i4_deg_start, 
    INT32               i4_deg_end,
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    GL_STYLE_T          t_style,
    UINT32              ui4_flag);

/*------------------------------------------------------------------*/
/*! @brief  perform the bit-blitting operation with pattern repetition
 *  @param  [in]    h_dst_surf          - destination surface handle
 *  @param  [in]    h_src_surf          - source surface handle
 *  @param  [in]    i4_dst_x            - x position on the destination surface
 *  @param  [in]    i4_dst_y            - y position on the destination surface
 *  @param  [in]    ui4_dst_w           - width of the destination bit-blitting region
 *  @param  [in]    ui4_dst_h           - height of the destination bit-blitting region
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_src_w           - width of the source bit-blitting region
 *  @param  [in]    ui4_src_h           - height of the source bit-blitting region
 *  @param  [in]    ui1_alpha_assigned  - if the source surface is without alpha channel, apply this value as a default alpha on the destination
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_patblt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    UINT8               ui1_alpha_assigned,
    UINT32              ui4_flag);

/*------------------------------------------------------------------*/
/*! @brief  perform the bit-blitting operation with rotate opration
 *  @param  [in]    h_dst_surf          - destination surface handle
 *  @param  [in]    h_src_surf          - source surface handle
 *  @param  [in]    i4_dst_x            - x position on the destination surface
 *  @param  [in]    i4_dst_y            - y position on the destination surface
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_width           - width of the  bit-blitting region
 *  @param  [in]    ui4_height           - height of the  bit-blitting region
 *  @param  [in]    ui1_alpha_assigned  - if the source surface is without alpha channel, apply this value as a default alpha on the destination
 *  @param  [in]    ui4_rot_op            -value of GL_BLT_ROTATE_NONE,GL_BLT_ROTATE_CW90,GL_BLT_ROTATE_CW180, GL_BLT_ROTATE_CW270 
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_surface_rotate_blt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha_assigned,
    UINT32              ui4_rot_op,
    UINT32              ui4_flag);

/*------------------------------------------------------------------*/
/*! @brief  stretch the source surface into the destination surface with repeat-and-drop algorithm
 *  @param  [in]    h_dst_surf          - destination surface handle
 *  @param  [in]    h_src_surf          - source surface handle
 *  @param  [in]    i4_dst_x            - x position on the destination surface
 *  @param  [in]    i4_dst_y            - y position on the destination surface
 *  @param  [in]    ui4_dst_w           - width of the destination region
 *  @param  [in]    ui4_dst_h           - height of the destination region
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_src_w           - width of the source region
 *  @param  [in]    ui4_src_h           - height of the source region
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_stretch_blt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    UINT32              ui4_flag);

/*------------------------------------------------------------------*/
/*! @brief  stretch the source surface into the destination surface with interpolation algorithm
 *  @param  [in]    h_dst_surf          - destination surface handle
 *  @param  [in]    h_src_surf          - source surface handle
 *  @param  [in]    i4_dst_x            - x position on the destination surface
 *  @param  [in]    i4_dst_y            - y position on the destination surface
 *  @param  [in]    ui4_dst_w           - width of the destination region
 *  @param  [in]    ui4_dst_h           - height of the destination region
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_src_w           - width of the source region
 *  @param  [in]    ui4_src_h           - height of the source region
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_adv_stretch_blt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    UINT32              ui4_flag);

/*------------------------------------------------------------------*/
/*! @brief  stretch the source surface into the destination surface with interpolation algorithm and options
 *  @param  [in]    h_dst_surf          - destination surface handle
 *  @param  [in]    h_src_surf          - source surface handle
 *  @param  [in]    i4_dst_x            - x position on the destination surface
 *  @param  [in]    i4_dst_y            - y position on the destination surface
 *  @param  [in]    ui4_dst_w           - width of the destination region
 *  @param  [in]    ui4_dst_h           - height of the destination region
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_src_w           - width of the source region
 *  @param  [in]    ui4_src_h           - height of the source region
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @param  [in]    e_option            - specifies this operation is for UI style data or photo related data
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_adv_stretch_blt_ex(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    UINT32              ui4_flag,
    GL_STRETCH_OPTION   e_option);

/*------------------------------------------------------------------*/
/*! @brief  copies the content of the source surface to the target surface with source color key specified
 *  @param  [in]    h_dst_surf          - destination surface handle
 *  @param  [in]    h_src_surf          - source surface handle
 *  @param  [in]    i4_dst_x            - x position on the destination surface
 *  @param  [in]    i4_dst_y            - y position on the destination surface
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_width           - width of the bit-blitting region
 *  @param  [in]    ui4_height          - height of the bit-blitting region
 *  @param  [in]    ui1_alpha_assigned  - if the source surface is without alpha channel, apply this value as a default alpha on the destination
 *  @param  [in]    b_off_mask          - contains the flag to indicate if an off-mask bit-blitting should be performed. If this field is TRUE, only pixel data outside the specified color key range are copied, otherwise pixel data within the range are blitted
 *  @param  [in]    pt_color_min        - the lower bound of the specified source color key range
 *  @param  [in]    pt_color_max        - the upper bound of the specified source color key range
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_transp_blt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha_assigned,
    BOOL                b_off_mask,
    const GL_COLOR_T*   pt_color_min,
    const GL_COLOR_T*   pt_color_max,
    UINT32              ui4_flag);

/*------------------------------------------------------------------*/
/*! @brief  rotate a source surface onto the destination surface
 *  @param  [in]    h_dst_surf          - destination surface handle
 *  @param  [in]    h_src_surf          - source surface handle
 *  @param  [in]    i4_dst_x            - x position on the destination surface
 *  @param  [in]    i4_dst_y            - y position on the destination surface
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_width           - width of the rotation region
 *  @param  [in]    ui4_height          - height of the rotation region
 *  @param  [in]    ui1_alpha_assigned  - if the source surface is without alpha channel, apply this value as a default alpha on the destination
 *  @param  [in]    ui4_rot_op          - rotation options
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_compressed_blt(
    GL_HSURFACE_T           h_dst_surf, 
    GL_HSURFACE_T           h_src_surf, 
    INT32                   i4_src_x,
    INT32                   i4_src_y,
    UINT32                  ui4_src_w,
    UINT32                  ui4_src_h,
    GL_COMPRESSION_RATIO_T  e_ratio,
    BOOL                    b_line_separate,
    UINT32                  ui4_flag);

/*------------------------------------------------------------------*/
/*! @brief  copies the content of the source surface to the target surface with source color key specified and pattern repetition
 *  @param  [in]    h_dst_surf          - destination surface handle
 *  @param  [in]    h_src_surf          - source surface handle
 *  @param  [in]    i4_dst_x            - x position on the destination surface
 *  @param  [in]    i4_dst_y            - y position on the destination surface
 *  @param  [in]    ui4_dst_w           - width of the destination bit-blitting region
 *  @param  [in]    ui4_dst_h           - height of the destination bit-blitting region
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_src_w           - width of the source bit-blitting region
 *  @param  [in]    ui4_src_h           - height of the source bit-blitting region
 *  @param  [in]    ui1_alpha_assigned  - if the source surface is without alpha channel, apply this value as a default alpha on the destination
 *  @param  [in]    b_off_mask          - contains the flag to indicate if an off-mask bit-blitting should be performed. If this field is TRUE, only pixel data outside the specified color key range are copied, otherwise pixel data within the range are blitted
 *  @param  [in]    pt_color_min        - the lower bound of the specified source color key range
 *  @param  [in]    pt_color_max        - the upper bound of the specified source color key range
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_transp_patblt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    UINT8               ui1_alpha_assigned,
    BOOL                b_off_mask,
    const GL_COLOR_T*   pt_color_min,
    const GL_COLOR_T*   pt_color_max,
    UINT32              ui4_flag);

/*------------------------------------------------------------------*/
/*! @brief  blend the source surface onto the destination surface
 *  @param  [in]    h_dst_surf          - destination surface handle
 *  @param  [in]    h_src_surf          - source surface handle
 *  @param  [in]    i4_dst_x            - x position on the destination surface
 *  @param  [in]    i4_dst_y            - y position on the destination surface
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_width           - width of the blending region
 *  @param  [in]    ui4_height          - height of the blending region
 *  @param  [in]    ui1_alpha           - blending level
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_blend(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha,
    UINT32              ui4_flag);

/*------------------------------------------------------------------*/
/*! @brief  compose the source surface onto the destination surface
 *  @param  [in]    h_dst_surf          - destination surface handle
 *  @param  [in]    h_src_surf          - source surface handle
 *  @param  [in]    i4_dst_x            - x position on the destination surface
 *  @param  [in]    i4_dst_y            - y position on the destination surface
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_width           - width of the composing region
 *  @param  [in]    ui4_height          - height of the composing region
 *  @param  [in]    ui1_pre_alpha       - an alpha value which would be multiplied to each pixel of the source surface
 *  @param  [in]    e_rule              - Porter-Duff composition rule
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_compose(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_pre_alpha,
    GL_COMPOSE_RULE_T   e_rule,
    UINT32              ui4_flag);

/*------------------------------------------------------------------*/
/*! @brief  compose the source surface onto the destination surface with pattern repetition
 *  @param  [in]    h_dst_surf          - destination surface handle
 *  @param  [in]    h_src_surf          - source surface handle
 *  @param  [in]    i4_dst_x            - x position on the destination surface
 *  @param  [in]    i4_dst_y            - y position on the destination surface
 *  @param  [in]    ui4_dst_w           - width of the destination composing region
 *  @param  [in]    ui4_dst_h           - height of the destination composing region
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_src_w           - width of the source composing region
 *  @param  [in]    ui4_src_h           - height of the source composing region
 *  @param  [in]    ui1_pre_alpha       - an alpha value which would be multiplied to each pixel of the source surface
 *  @param  [in]    e_rule              - Porter-Duff composition rule
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_pat_compose(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    UINT8               ui1_pre_alpha,
    GL_COMPOSE_RULE_T   e_rule,
    UINT32              ui4_flag);
/*------------------------------------------------------------------*/
/*! @brief  compose the source surface onto the destination surface with color
 *  @param  [in]    h_dst_surf          - destination surface handle
 *  @param  [in]    i4_dst_x            - x position on the destination surface
 *  @param  [in]    i4_dst_y            - y position on the destination surface
 *  @param  [in]    ui4_width           - width of the composing region
 *  @param  [in]    ui4_height           - height of the composing region
 *  @param  [in]    ui1_pre_alpha       - an alpha value which would be multiplied to each pixel of the source surface
 *  @param  [in]    e_rule              - Porter-Duff composition rule
 *  @param  [in]    e_src_colormode              -source color mode
 *  @param  [in]    pt_src_color              - source color.
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_surface_rect_src_compose(
    GL_HSURFACE_T       h_dst_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_pre_alpha,
    GL_COMPOSE_RULE_T   e_rule,
    GL_COLORMODE_T      e_src_colormode,
    GL_COLOR_T          *pt_src_color,
    UINT32              ui4_flag);
/*------------------------------------------------------------------*/
/*! @brief  compose the source surface onto the destination surface with color
 *  @param  [in]    h_surf          - destination surface handle
 *  @param  [in]    pt_color              - source color.
 *  @param  [in]    i4_dst_x            - x position on the destination surface
 *  @param  [in]    i4_dst_y            - y position on the destination surface
 *  @param  [in]    i4_src_x            - x position on the source
 *  @param  [in]    i4_src_y            - y position on the source
 *  @param  [in]    ui4_width           - width of the composing region
 *  @param  [in]    ui4_height           - height of the composing region
 *  @param  [in]    e_rule              - Porter-Duff composition rule
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_surface_color_compose(
    GL_HSURFACE_T       h_surf,
    const GL_COLOR_T*   pt_color,
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    GL_COMPOSE_RULE_T   e_rule,
    UINT32              ui4_flag);

/*------------------------------------------------------------------*/
/*! @brief  draw characters onto a surface
 *  @param  [in]    h_surf              - surface handle
 *  @param  [in]    i4_x                - x position on the destination surface
 *  @param  [in]    i4_y                - y position on the destination surface
 *  @param  [in]    aw2_chars           - an array for characters to be drawn
 *  @param  [in]    ui2_len             - length of aw2_chars
 *  @param  [in]    h_fnt               - font handle
 *  @param  [in]    pt_fg_clr           - foreground color
 *  @param  [in]    pt_bg_clr           - background color
 *  @param  [out]   pr_rc_str           - the rendering rectangle covered by drawn characters
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 *  @retval GLR_FE_ERROR                - font library error
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_draw_uchars(
    GL_HSURFACE_T       h_surf,
    INT32               i4_x,
    INT32               i4_y,
    UTF16_T             aw2_chars[],
    UINT16              ui2_len,
    HFONT_T             h_fnt,
    const GL_COLOR_T*   pt_fg_clr,
    const GL_COLOR_T*   pt_bg_clr,
	GL_RECT_T*          pt_rc_str);

/*------------------------------------------------------------------*/
/*! @brief  draw a string on a surface
 *  @param  [in]    h_surf              - destination surface handle
 *  @param  [in]    i4_x                - x position on the destination surface
 *  @param  [in]    i4_y                - y position on the destination surface
 *  @param  [in]    w2s_str             - pointer to the string to be drawn
 *  @param  [in]    ui2_strlen          - string length of w2s_str
 *  @param  [in]    h_fnt               - font handle
 *  @param  [in]    pt_fg_clr           - foreground color
 *  @param  [in]    pt_bg_clr           - background color
 *  @param  [out]   pr_rc_str           - the rendering rectangle covered by drawn characters
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 *  @retval GLR_FE_ERROR                - font library error
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_draw_ustring(
    GL_HSURFACE_T       h_surf,
    INT32               i4_x,
    INT32               i4_y,
    UTF16_T*            w2s_str,
    UINT16              ui2_strlen,
    HFONT_T             h_fnt,
    const GL_COLOR_T*   pt_fg_clr,
    const GL_COLOR_T*   pt_bg_clr,
	GL_RECT_T*          pt_rc_str);
/*------------------------------------------------------------------*/
/*! @brief  draw a string on a surface with more options
 *  @param  [in]    h_surf              - destination surface handle
 *  @param  [in]    i4_x                - x position on the destination surface
 *  @param  [in]    i4_y                - y position on the destination surface
 *  @param  [in]    i4_bg_y             - y position of the background on the destination surface
 *  @param  [in]    w2s_str             - pointer to the string to be drawn
 *  @param  [in]    ui2_strlen          - string length of w2s_str
 *  @param  [in]    w2s_skip            - pointer to the string which contains characters to be skipped in w2s_str
 *  @param  [in]    ui2_skiplen         - string length of w2s_skip
 *  @param  [in]    h_fnt               - font handle
 *  @param  [in]    pt_fg_clr           - foreground color
 *  @param  [in]    pt_bg_clr           - background color
 *  @param  [in]    pt_edge_clr         - edge color
 *  @param  [in]    b_fixed             - indicates if the interval between characters is fixed
 *  @param  [in]    ui2_ch_w            - when b_fixed is TRUE, this indicates what the interval is
 *  @param  [in]    ui2_bg_h            - indicates the height of the background
 *  @param  [in]    i4_bg_adj_x         - indicates the offset of background in the x direction
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 *  @retval GLR_FE_ERROR                - font library error
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_surface_draw_ustring_ex(
    GL_HSURFACE_T       h_surf,
    INT32               i4_x,
    INT32               i4_y,
    INT32               i4_bg_y,
    VOID*               pv_str,
    UINT16              ui2_strlen,
    VOID*               pv_skip,
    UINT16              ui2_skiplen,
    HFONT_T             h_fnt,
    const GL_COLOR_T*   pt_fg_clr,
    const GL_COLOR_T*   pt_bg_clr,
    const GL_COLOR_T*   pt_edge_clr,
    BOOL                b_fixed,
    UINT16              ui2_ch_w,
    UINT16              ui2_skip_w,
    UINT16              ui2_bg_h,
    INT32               i4_bg_adj_x,
    GL_DRAW_STR_MODE_T  e_draw_str_mode);

/*------------------------------------------------------------------*/
/*! @brief  draw a string for ttx.
 *  @param  [in]    h_surf          - destination surface handle
 *  @param  [in]    i4_x                - x position on the destination graphics
 *  @param  [in]    i4_y                - y position on the destination graphics
 *  @param  [in]    at_drcs_str             - PTU structure array.
 *  @param  [in]    ui2_str_len             - count of PTU structure array.
 *  @param  [in]    ui1_width          - character width
 *  @param  [in]    ui1_height            - character height.
 *  @param  [in]    pt_info         - describe information related to a DRCS string
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 *  @retval GLR_FE_ERROR                - font library error
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_surface_draw_drcs_string(
    GL_HSURFACE_T           h_surf,
    INT32                   i4_x,
    INT32                   i4_y,
    SM_TTX_WGL_DRCS_PTU_T   at_drcs_str[],
    UINT16                  ui2_str_len,
    UINT8                   ui1_width,
    UINT8                   ui1_height,
    GL_DRCS_INFO_T*         pt_info);

/*------------------------------------------------------------------*/
/*! @brief  draw a string for close caption
 *  @param  [in]    h_surf          - destination surface handle
 *  @param  [in]    i4_x                - x position on the destination graphics
 *  @param  [in]    i4_y                - y position on the destination graphics
 *  @param  [in]    i4_bg_y             - y position of the background on the destination graphics
 *  @param  [in]    pv_str             - pointer to the string to be drawn
 *  @param  [in]    ui2_strlen          - string length of w2s_str
 *  @param  [in]    pv_skip            - pointer to the string which contains characters to be skipped in w2s_str
 *  @param  [in]    ui2_skiplen         - string length of w2s_skip
 *  @param  [in]    ui2_skip_line_len         - line length of skip character.
 *  @param  [in]    h_fnt               - font handle
 *  @param  [in]    pt_fg_clr           - foreground color
 *  @param  [in]    pt_bg_clr           - background color
 *  @param  [in]    pt_edge_clr         - edge color
 *  @param  [in]    b_fixed             - indicates if the interval between characters is fixed
 *  @param  [in]    ui2_ch_w            - when b_fixed is TRUE, this indicates what the interval is
 *  @param  [in]    ui2_skip_w            - when b_fixed is TRUE, this indicates what the interval is
 *  @param  [in]    ui2_bg_h            - indicates the height of the background
 *  @param  [in]    i4_bg_adj_x         - indicates the offset of background in the x direction
 *  @param  [in]    e_draw_str_mode         - specify the string drawing mode
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 *  @retval GLR_FE_ERROR                - font library error
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_surface_draw_ustring_skip_underline(
    GL_HSURFACE_T       h_surf,
    INT32               i4_x,
    INT32               i4_y,
    INT32               i4_bg_y,
    VOID*               pv_str,
    UINT16              ui2_strlen,
    VOID*               pv_skip,
    UINT16              ui2_skiplen,
    VOID*               pv_skip_line,
    UINT16              ui2_skip_line_len,
    HFONT_T             h_fnt,
    const GL_COLOR_T*   pt_fg_clr,
    const GL_COLOR_T*   pt_bg_clr,
    const GL_COLOR_T*   pt_edge_clr,
    BOOL                b_fixed,
    UINT16              ui2_ch_w,
    UINT16              ui2_skip_w,
    UINT16              ui2_bg_h,
    INT32               i4_bg_adj_x,
    GL_DRAW_STR_MODE_T  e_draw_str_mode);

/*------------------------------------------------------------------*/
/*! @brief  draw a string for top 2 down
 *  @param  [in]    h_surf          - destination surface handle
 *  @param  [in]    i4_x                - x position on the destination graphics
 *  @param  [in]    i4_y                - y position on the destination graphics
 *  @param  [in]    i4_bg_x             - x position of the background on the destination graphics
 *  @param  [in]    pv_str             - pointer to the string to be drawn
 *  @param  [in]    ui2_strlen          - string length of w2s_str
 *  @param  [in]    pv_skip            - pointer to the string which contains characters to be skipped in w2s_str
 *  @param  [in]    ui2_skiplen         - string length of w2s_skip
 *  @param  [in]    ui2_skip_line_len         - line length of skip character.
 *  @param  [in]    h_fnt               - font handle
 *  @param  [in]    pt_fg_clr           - foreground color
 *  @param  [in]    pt_bg_clr           - background color
 *  @param  [in]    pt_edge_clr         - edge color
 *  @param  [in]    b_fixed             - indicates if the interval between characters is fixed
 *  @param  [in]    ui2_ch_w            - when b_fixed is TRUE, this indicates what the interval is
 *  @param  [in]    ui2_skip_w            - when b_fixed is TRUE, this indicates what the interval is
 *  @param  [in]    ui2_bg_h            - indicates the height of the background
 *  @param  [in]    i4_bg_adj_y         - indicates the offset of background in the y direction
 *  @param  [in]    aui2_ch_orient_idx         - array of orientation index.
 *  @param  [in]    ui2_ch_orient_num         - count array of orientation index.
 *  @param  [in]   e_ch_orientation           - character orientation
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 *  @retval GLR_FE_ERROR                - font library error
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_surface_draw_ustring_top_2_down(
    GL_HSURFACE_T       h_surf,
    INT32               i4_x,
    INT32               i4_y,
    INT32               i4_bg_x,
    VOID*               pv_str,
    UINT16              ui2_strlen,
    VOID*               pv_skip,
    UINT16              ui2_skiplen,
    HFONT_T             h_fnt,
    const GL_COLOR_T*   pt_fg_clr,
    const GL_COLOR_T*   pt_bg_clr,
    const GL_COLOR_T*   pt_edge_clr,
    BOOL                b_fixed,
    UINT16              ui2_ch_h,
    UINT16              ui2_skip_h,
    UINT16              ui2_bg_w,
    INT32               i4_bg_adj_y,
    UINT16              *aui2_ch_orient_idx,
    UINT16              ui2_ch_orient_num,
    GL_CHAR_ORIENT_T    e_ch_orientation);
/*------------------------------------------------------------------*/
/*! @brief  draw string for ttx.
 *  @param  [in]    h_surf          - destination surface handle
 *  @param  [in]    i4_x                - x position on the destination graphics
 *  @param  [in]    i4_y                - y position on the destination graphics
 *  @param  [in]    w2s_str             - pointer to the string to be drawn
 *  @param  [in]    ui2_strlen          - string length of w2s_str
 *  @param  [in]    h_fnt               - font handle
 *  @param  [in]    pt_ttx_clr           - foreground and background colors for rendering teletext a string
 *  @param  [in]    e_ttx_size             - double width or double height TTX size
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 *  @retval GLR_FE_ERROR                - font library error
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_surface_draw_ttx_string(
    GL_HSURFACE_T       h_surf,
    INT32               i4_x,
    INT32               i4_y,
    UTF16_T*            w2s_str,
    UINT16              ui2_strlen,
    HFONT_T             h_fnt,
    GL_TTX_COLOR_T*     pt_ttx_clr,
    GL_TTX_STR_SIZE_T   e_ttx_size);
/*------------------------------------------------------------------*/
/*! @brief  fill a surface with a specific color
 *  @param  [in]    h_surf              - surface handle
 *  @param  [in]    pt_color            - color to be filled
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_SURFACE_LOCKED          - the associated surface is locked
 *  @retval GLR_SURFACE_BUSY            - the associated surface is busy
 *  @retval GLR_INV_COLORMODE           - invalid colormode
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_surface_fill(
    GL_HSURFACE_T       h_surf, 
    const GL_COLOR_T*   pt_color,
    UINT32              ui4_flag);

/*------------------------------------------------------------------*/
/*! @brief  reconfig the surface dimension without reallocating memory
 *  @param  [in]    h_surf              - surface handle
 *  @param  [in]    ui4_width           - new surface width
 *  @param  [in]    ui4_height          - new surface height
 *  @param  [in]    e_mode              - new color mode
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_INCOMPATIBLE_SURFACE    - input surface(s) is/are not compatible
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_reconfig(
    GL_HSURFACE_T       h_surf,
    UINT32              ui4_width,
    UINT32              ui4_height,
    GL_COLORMODE_T      e_mode);

/*------------------------------------------------------------------*/
/*! @brief  threshold the alpha value for draw string.
 *  @param  [in]    ui1_threshold              -alpha value of  threshold
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_surface_set_alpha_threshold(
    UINT8               ui1_threshold);

/*------------------------------------------------------------------*/
/*! @brief  set a palette on a surface
 *  @param  [in]    h_surf              - surface handle
 *  @param  [in]    h_palette           - palette handle
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                          - ok
 *  @retval GLR_NOT_INIT                    - GL is not initialized
 *  @retval GLR_INV_ARG                     - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ            - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE                  - an invalid handle is passed
 *  @retval GLR_DRV_ERROR                   - driver error
 *  @retval GLR_OUT_OF_HANDLES              - out of handles
 *  @retval GLR_HDL_LIB_ERROR               - handle library error
 *  @retval GLR_INV_COLORMODE               - invalid colormode
 *  @retval GLR_SURFACE_ALREADY_ATTACHED    - surface has been attached on a view
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_set_palette(
    GL_HSURFACE_T       h_surf, 
    GL_HPALETTE_T       h_palette);

/*------------------------------------------------------------------*/
/*! @brief  set a clipper on a surface
 *  @param  [in]    h_surf                  - surface handle
 *  @param  [in]    h_clipper               - clipper handle
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                          - ok
 *  @retval GLR_NOT_INIT                    - GL is not initialized
 *  @retval GLR_INV_ARG                     - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ            - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE                  - an invalid handle is passed
 *  @retval GLR_OUT_OF_HANDLES              - out of handles
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_surface_set_clipper(
    GL_HSURFACE_T       h_surf, 
    GL_HCLIPPER_T       h_clipper);
    
/*------------------------------------------------------------------*/
/*! @brief  replace colors while doing bitblt
 *  @param  [in]    h_dst_surf          - destination surface handle
 *  @param  [in]    h_src_surf          - source surface handle
 *  @param  [in]    i4_dst_x            - x position on the destination surface
 *  @param  [in]    i4_dst_y            - y position on the destination surface
 *  @param  [in]    i4_src_x            - x position on the source surface
 *  @param  [in]    i4_src_y            - y position on the source surface
 *  @param  [in]    ui4_width           - width of the composing region
 *  @param  [in]    ui4_height          - height of the composing region
 *  @param  [in]    ui1_alpha_assigned  - if the source surface is without alpha channel, apply this value as a default alpha on the destination
 *  @param  [in]    at_original_color            - original color array
 *  @param  [in]    at_new_color            - new color array
 *  @param  [in]    ui1_color_num            - color number
 *  @param  [in]    ui4_flag            - synchorization flag. It could be 0, GL_SYNC, or GL_NOWAIT
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                          - ok
 *  @retval GLR_INV_ARG                    - invalid arguments
 *  @retval GLR_DRV_ERROR                     - driver error
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_surface_replace_color_blt(
    GL_HSURFACE_T       h_dst_surf,
    GL_HSURFACE_T       h_src_surf,
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha_assigned,
    GL_COLOR_T          at_original_color[],
    GL_COLOR_T          at_new_color[],
    UINT8               ui1_color_num,
    UINT32              ui4_flag);


INT32 x_gl_surface_flip(GL_HSURFACE_T       h_surf, UINT32              ui4_flag);

/*------------------------------------------------------------------*/
/*! @brief  create a clipper
 *  @param  [in]    ui2_count           - number of clipping region associated with this clipper. Only 1 region is allowed in current implementation
 *  @param  [in]    pt_rect             - clipping region
 *  @param  [out]   ph_clipper          - clipper handle
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_OUT_OF_HANDLES          - out of handles
 *  @retval GLR_NO_MEMORY               - out of memory
 */
/*------------------------------------------------------------------*/

/* clipper component API */
extern INT32 x_gl_clipper_create(
    UINT16              ui2_count, 
    const GL_RECT_T*    pt_rect, 
    GL_HCLIPPER_T*      ph_clipper);

/*------------------------------------------------------------------*/
/*! @brief  change the clipping list of a clipper
 *  @param  [in]    h_clipper           - clipper handle
 *  @param  [in]    ui2_count           - number of clipping region to be associated with thish clipper. Only 1 region is allowed in current implementation
 *  @param  [in]    pt_rect             - pointer to the clipping region list
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_clipper_set_clip_list(
    GL_HCLIPPER_T       h_clipper, 
    UINT16              ui2_count, 
    const GL_RECT_T*    pt_rect
);
/*------------------------------------------------------------------*/
/*! @brief  get the clipping list of a clipper
 *  @param  [in]    h_clipper           - clipper handle
 *  @param  [out]   pui2_count          - number of clipping region associated with this clipper
 *  @param  [out]   pt_rect             - pointer to the clipping region list
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_clipper_get_clip_list(
    GL_HCLIPPER_T       h_clipper, 
    UINT16*             pui2_count, 
    GL_RECT_T*          pt_rect
);
/*------------------------------------------------------------------*/
/*! @brief  create a palette
 *  @param  [in]    e_clrmode           - color mode of this palette table
 *  @param  [in]    ui2_count           - number of entries in this palette table
 *  @param  [in]    at_color            - an array of colors to be set into this palette table
 *  @param  [out]   ph_palette          - palette handle
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_OUT_OF_HANDLES          - out of handles
 *  @retval GLR_NO_MEMORY               - out of memory
 *  @retval GLR_INV_COLORMODE           - invalid colormode
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_palette_create(
    GL_COLORMODE_T      e_clrmode, 
    UINT16              ui2_count,
    const GL_COLOR_T    at_color[], 
    GL_HPALETTE_T*      ph_palette);

/*------------------------------------------------------------------*/
/*! @brief  create a palette with a specified palette ID
 *  @param  [in]    e_clrmode           - color mode of this palette table
 *  @param  [in]    ui2_count           - number of entries in this palette table
 *  @param  [in]    at_color            - an array of colors to be set into this palette table
 *  @param  [in]    ui4_id              - palette ID
 *  @param  [out]   ph_palette          - palette handle
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 *  @retval GLR_OUT_OF_HANDLES          - out of handles
 *  @retval GLR_NO_MEMORY               - out of memory
 *  @retval GLR_INV_COLORMODE           - invalid colormode
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_palette_create_with_id(
    GL_COLORMODE_T      e_clrmode, 
    UINT16              ui2_count,
    const GL_COLOR_T    at_color[], 
    UINT32              ui4_id,
    GL_HPALETTE_T*      ph_palette);

/*------------------------------------------------------------------*/
/*! @brief  set the palette entry
 *  @param  [in]    h_palette           - palette handle
 *  @param  [in]    ui2_base            - the base palette entry to be set
 *  @param  [in]    ui2_count           - number of palette entries from entry[ui2_base] to be set
 *  @param  [in]    at_color            - an array of colors to be set into the palette entry
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 *  @retval GLR_DRV_ERROR               - driver error
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_palette_set_entries(
    GL_HPALETTE_T       h_palette, 
    UINT16              ui2_base, 
    UINT16              ui2_count, 
    const GL_COLOR_T    at_color[]);

/*------------------------------------------------------------------*/
/*! @brief  get palette entries from a palette table
 *  @param  [in]    h_palette           - palette handle
 *  @param  [in]    ui2_base            - the base palette entry to be got
 *  @param  [in]    ui2_count           - number of palette entries from entry[ui2_base] to be got
 *  @param  [out]   at_color            - an array to get colors from the specified palette entries
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_palette_get_entries(
    GL_HPALETTE_T       h_palette, 
    UINT16              ui2_base, 
    UINT16              ui2_count, 
    GL_COLOR_T          at_color[]);

/*------------------------------------------------------------------*/
/*! @brief  find the closest color in a palette table for a specified color
 *  @param  [in]    h_palette           - palette handle
 *  @param  [in]    b_cmp_alpha         - take the alpha channel into consideration or not
 *  @param  [in]    pt_to_match         - the original color to match
 *  @param  [out]   pt_matched          - the matched color in the palette table
 *  @param  [out]   pui2_index          - index of pt_matched in the palette table
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_palette_match_color(
    GL_HPALETTE_T       h_palette,
    BOOL                b_cmp_alpha,
    const GL_COLOR_T*   pt_to_match,
    GL_COLOR_T*         pt_matched,
    UINT16*             pui2_index);

/*------------------------------------------------------------------*/
/*! @brief  find the closest color in a range of a palette table for a specified color
 *  @param  [in]    h_palette           - palette handle
 *  @param  [in]    ui2_base            - the base palette entry to be queried
 *  @param  [in]    ui2_count           - number of palette entries from entry[ui2_base] to be queried
 *  @param  [in]    b_cmp_alpha         - take the alpha channel into consideration or not
 *  @param  [in]    pt_to_match         - the original color to match
 *  @param  [out]   pt_matched          - the matched color in the palette table
 *  @param  [out]   pui2_index          - index of pt_matched in the palette table
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 */
/*------------------------------------------------------------------*/
extern INT32 x_gl_palette_match_color_ex(
    GL_HPALETTE_T       h_palette,
    UINT16              ui2_base,
    UINT16              ui2_count,
    BOOL                b_cmp_alpha,
    const GL_COLOR_T*   pt_to_match,
    GL_COLOR_T*         pt_matched,
    UINT16*             pui2_index);

/*------------------------------------------------------------------*/
/*! @brief  no segment pattern paint
 *  @param  [in]    h_graphics           - graphic handle
 *  @param  [in]    e_colormode            - color of source image
 *  @param  [in]    pt_colorkey           - color key
 *  @param  [in]    pui1_compressed_img         - compressed image data
 *  @param  [in]    pt_work_rect         - rectangle of draw region
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_sw_nhv_pattern_paint(
    GL_HGRAPHICS_T      h_graphics,
    GL_COLORMODE_T      e_colormode,
    GL_COLOR_T          *pt_colorkey,
    UINT8               *pui1_compressed_img,
    GL_RECT_T           *pt_work_rect);

/*------------------------------------------------------------------*/
/*! @brief  no segment pattern compose
 *  @param  [in]    h_graphics           - graphic handle
 *  @param  [in]    e_colormode            - color of source image
 *  @param  [in]    pt_colorkey           - color key
 *  @param  [in]    pui1_compressed_img         - compressed image data
 *  @param  [in]    pt_work_rect         - rectangle of draw region
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 *  @retval GLR_NOT_INIT                - GL is not initialized
 *  @retval GLR_INV_ARG                 - invalid argument is passed
 *  @retval GLR_INCOMPATIBLE_OBJ        - incompatible GL object(s) is/are passed
 *  @retval GLR_INV_HANDLE              - an invalid handle is passed
 */
/*------------------------------------------------------------------*/


extern INT32 x_gl_sw_nhv_pattern_paint_compose(
    GL_HGRAPHICS_T      h_graphics,
    GL_COLORMODE_T      e_colormode,
    GL_COLOR_T          *pt_colorkey,
    UINT8               *pui1_compressed_img,
    GL_RECT_T           *pt_work_rect);

/*------------------------------------------------------------------*/
/*! @brief  clear image cache
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_nhv_cache_flush(VOID);

/*------------------------------------------------------------------*/
/*! @brief  get fbm surface infomation
 *  @param  [out]    pui1_cnt     - fbm buffer count.
 *  @param  [out]    pui4_mem_size            - size of one fbm buffer
 *  @note
 *  @see
 *  @return return the error code
 *  @retval GLR_OK                      - ok
 */
/*------------------------------------------------------------------*/

extern INT32 x_gl_get_fbm_surface_info(
    UINT8*              pui1_cnt, 
    UINT32*             pui4_mem_size);


extern VOID* x_gl_dfb_get_native_engine(VOID);
extern INT32 x_gl_osd_wait_vdp(VOID);


#ifdef ANDROID
/** get plane z-order
 *  @param  u4PlaneId               plane id:{GLPMX_MAIN, GLPMX_PIP, GLPMX_OSD1, GLPMX_OSD2}
 *  @param  pu4PlaneOrder           plane order:{GLPMX_ORDER1/2/3/4}
 *  @retval GLR_OK                  Success
 *  @retval other              Fail
 */
extern INT32 x_gl_GetPlaneOrder(UINT32 u4PlaneId, UINT32* pu4PlaneOrder);

/** set plane z-order
 *  @param  u4PlaneId               plane id:{GLPMX_MAIN, GLPMX_PIP, GLPMX_OSD1, GLPMX_OSD2}
 *  @param  u4PlaneOrder            plane order:{GLPMX_ORDER1/2/3/4}
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *   Pmx can control the order of Video/Graphic planes and merge into one to output stage.
 *   The sources of Pmx are Main, PIP, OSD1, OSD2.
 *   For example, through pmx, we can assign OSD1 as top-most plane, and then OSD2, PIP, and Main as the bottom plane.
 */
extern INT32 x_gl_SetPlaneOrder(UINT32 u4PlaneId, UINT32 u4PlaneOrder);

/** set plane z-order array
 *  @param  pu4PlaneOrder               plane order array addr
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *   Pmx can control the order of Video/Graphic planes and merge into one to output stage.
 *   The sources of Pmx are Main, PIP, OSD1, OSD2.
 *   In mt5395, OSD planes must be on top of video planes.
 *   For example, through pmx, we can assign OSD2 as top-most plane, and then OSD1, PIP, and Main as the bottom plane.
 *   Then the plane order array will be set as 
 *   const UINT32 au4PlaneArray[4] = { GLPMX_OSD2, GLPMX_OSD1, GLPMX_PIP, GLPMX_MAIN };
 */
extern INT32 x_gl_SetPlaneOrderArray(UINT32* pu4PlaneOrder);


/** set plane z-order array
 *  @param  pu4PlaneOrder               plane order array addr
 *  @param  u4PlaneOrder            plane order:{MTPMX_ORDER1/2/3/4}
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *   Pmx can control the order of Video/Graphic planes and merge into one to output stage.
 *   The sources of Pmx are Main, PIP, OSD1, OSD2.
 *   In mt5395, OSD planes must be on top of video planes.
 *   For example, through pmx, we can assign OSD2 as top-most plane, and then OSD1, PIP, and Main as the bottom plane.
 *   Then the plane order array will be set as 
 *   const UINT32 au4PlaneArray[4] = { PMX_OSD2, PMX_OSD1, MTPMX_PIP, MTPMX_MAIN };
 */
extern INT32 x_gl_GetPlaneOrderArray(UINT32* pu4PlaneOrder, INT32 i4Count);


#endif


#ifdef __cplusplus
}
#endif

#endif

/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_GL*/
/*----------------------------------------------------------------------------*/
