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
 * $RCSfile: x_wgl_common.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/3 $
 * $SWAuthor: Al Hsieh $
 * $MD5HEX: ee6e2de6d3dc3066e4041b8ccac1cde2 $
 *
 * Description: 
 *         This header file contains public interfaces, constant and type 
 *         definitions of the Widget Library.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_WGL   Widget Library
 *
 *  @ingroup groupMW
 *  @brief Provides constant,type definitions and function of the widget library.
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/ 
#ifndef _X_WGL_COMMON_H_
#define _X_WGL_COMMON_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "x_common.h"
#include "u_wgl_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define WGL_STATE_ENABLED               ((UINT16) 0x0001)/**< Indicates the specified widget is enabled. */
#define WGL_STATE_VISIBLE               ((UINT16) 0x0002)/**< Indicates the specified widget is visible. */
#define WGL_STATE_ACTIVATED             ((UINT16) 0x0004)/**< Indicates the specified widget is activated. */
#define WGL_STATE_FOCUSED               ((UINT16) 0x0008)/**< Indicates the specified widget has the focus. */
#define WGL_STATE_PUSHED                ((UINT16) 0x0010)/**< Indicates the specified widget is in pushed state. */


#define WGL_ATTR_FOCUSABLE              ((UINT32) 0x0001) /**< Indicates the specified widget has the capability to get focus. */

#define IS_STATE_ENABLED(ui2_state)                                     \
        ((ui2_state) & WGL_STATE_ENABLED)

#define IS_STATE_VISIBLE(ui2_state)                                     \
        ((ui2_state) & WGL_STATE_VISIBLE)

#define IS_STATE_ACTIVATED(ui2_state)                                   \
        ((ui2_state) & WGL_STATE_ACTIVATED)

#define IS_STATE_FOCUSED(ui2_state)                                     \
        ((ui2_state) & WGL_STATE_FOCUSED)

#define IS_STATE_PUSHED(ui2_state)                                      \
        ((ui2_state) & WGL_STATE_PUSHED)

typedef VOID *WGL_WIDGET_CONTEXT_HANDLE_T;/**< Indicates the widget context handle type pointer. */

typedef VOID *WGL_WIDGET_ANIM_CONTEXT_T;/**< Indicates the widget context structure type pointer. */

typedef struct _WIDGET_CREATE_INPUT_T WGL_WIDGET_CREATE_INPUT_T;/**< Indicates the widget widget create input structure type pointer. */

/*------------------------------------------------------------------*/
/*! @brief  The widget create instance function pointer which is called in x_wgl_create_widget() and implemented by every type widget
 *  @param  [in]    pt_ctx_wgt         - Contains  the widget context handle pointer
 *  @param  [in]    pt_wgt_create       -Contains the pointer to create input information
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK			The function call is successful.
 *  @retval     WGLR_INV_ARG			pt_ctx_wgt or pt_wgt_create is NULL .
 *  @retval     WGLR_OUT_OF_MEMORY			Memory resource is not enough
 *  @retval     WGLR_INV_TYPE			pt_wgt_create contains invalid content or border type.
 *  @retval     WGLR_NO_BORDER			The widget has null border and with create style WGL_STL_GL_BDR_FILL_CNT_BK.
 */
/*------------------------------------------------------------------*/

typedef INT32 (*wgl_create_widget_instance_fct)( 
    WGL_WIDGET_CONTEXT_HANDLE_T         *pt_ctx_wgt,
    WGL_WIDGET_CREATE_INPUT_T           *pt_wgt_create);


/*------------------------------------------------------------------*/
/*! @brief  The widget destroy instance function pointer which is called in x_wgl_destroy_widget() and implemented by every type widget
 *  @param  [in]    pt_ctx_wgt         - Contains  the widget context handle pointer
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK			The function call is successful.
 *  @retval     WGLR_INV_ARG			pt_ctx_wgt  is NULL .
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wgl_destroy_widget_instance_fct)(
    WGL_WIDGET_CONTEXT_HANDLE_T         t_ctx_wgt);

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @struct _WIDGET_CREATE_INPUT_T
 *  @brief  This structure is used while the widget instance is created
 *  @code
 *struct _WIDGET_CREATE_INPUT_T
 *{
 *  UINT32                  ui4_type_content;
 *   UINT32                  ui4_type_border;
 *  const GL_RECT_T*        pt_rect;
 *   WGL_CANVAS_INFO_T       t_cvs_info;
 *   GL_HPALETTE_T           h_palette;
 *   VOID*                   pv_param;
 * };
 *  @endcode
 *  @li@c  ui4_type_content                               - Indicates the widget's content type.
 *  @li@c  ui4_type_border                                - Indicates the widget's border type
 *  @li@c  pt_rect                                             - Indicates the widget's region whose coordinate is based on the widget's parent.
 *  @li@c  t_cvs_info                                         - Indicates the widget's canvas info.
 *  @li@c  t_cvs_info                                         - Indicates the widget's canvas's palette handle if has.
 *  @li@c  pv_param                                         - Indicates the private data and defined by concrete widget type.
 */
/*------------------------------------------------------------------*/
struct _WIDGET_CREATE_INPUT_T
{
    UINT32                  ui4_type_content;
    UINT32                  ui4_type_border;
    const GL_RECT_T*        pt_rect;
    WGL_CANVAS_INFO_T       t_cvs_info;
    GL_HPALETTE_T           h_palette;
    VOID*                   pv_param;
};

/*------------------------------------------------------------------*/
/*! @struct WGL_WIDGET_CTRL_INTF_T
 *  @brief  This structure defines widget's function table which must be implemented by every type widget.
 *  @code
 *typedef struct _WGL_WIDGET_CTRL_INTF_T
 *{
 *  wgl_create_widget_instance_fct      pf_create_instance;
 *   wgl_destroy_widget_instance_fct     pf_destroy_instance;
 *   wgl_widget_proc_fct                 pf_msg_proc;
 *   UINT32                              ui4_attribute;
 * } WGL_WIDGET_CTRL_INTF_T;
 *  @endcode
 *  @li@c  pf_create_instance                               - Indicates the widget's  create function pointer.
 *  @li@c  pf_destroy_instance                                - Indicates the widget's destroy function pointer
 *  @li@c  pf_msg_proc                                      - Indicates the widget's message handling function pointer
 *  @li@c  ui4_attribute                                         - Indicates the widget's attribute, now only WGL_ATTR_FOCUSABLE is valid value.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_WIDGET_CTRL_INTF_T
{
    wgl_create_widget_instance_fct      pf_create_instance;
    wgl_destroy_widget_instance_fct     pf_destroy_instance;
    wgl_widget_proc_fct                 pf_msg_proc;
    UINT32                              ui4_attribute;
} WGL_WIDGET_CTRL_INTF_T;


/*------------------------------------------------------------------*/
/*! @struct WGL_MTX_OBJ_T
 *  @brief  This structure wrapper animation context handle
 *  @code
 *typedef struct _WGL_MTX_OBJ_T
 *{
 *   WGL_WIDGET_ANIM_CONTEXT_T                        t_anim;
 * } WGL_MTX_OBJ_T;
 *  @endcode
 *  @li@c  t_anim                               - Indicates widget element 's animation handle.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_MTX_OBJ_T
{
#ifdef CLI_LVL_ALL     
    UINT32                          ui4_mem_size;/*this field must be the first field in the structure*/
#endif  
    WGL_WIDGET_ANIM_CONTEXT_T       t_anim;
} WGL_MTX_OBJ_T;

/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif
/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_WGL*/
/*----------------------------------------------------------------------------*/

