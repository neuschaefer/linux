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
 * $RCSfile: x_wgl.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/9 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: b72a3323698943ee5fcb9a4e0fbb2e95 $
 *
 * Description: 
 *         This header file contains the exported constant and type definitions 
 *         of the Widget Library.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_WGL
 *  @{ 
 */
/*----------------------------------------------------------------------------*/ 
#ifndef _X_WGL_H_
#define _X_WGL_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_handle.h"
#include "u_os.h"
#include "u_gl.h"
#include "u_wgl.h"
#include "u_wgl_animation.h"
#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @brief  add exclusive regions to a transparent list,these regions will have no transparent effect.
 *  @param  [in]    h_rgn_list          - Contains the region list handle created using c_wgl_create_transparent_list()
 *  @param  [in]    at_exclusive_rect   - Contains an array of GL_RECT_T to specify the regions to be exclusive
 *  @param  [in]    ui4_rect_count      - Contains exclusive region number
 *  @note
 *  @see c_wgl_create_transparent_list()
 *  @return return the error code
 *  @retval    WGLR_OK             The routine was successful.
 *  @retval   WGLR_INV_HNADLE     The argument h_rgn_list contains an invalid region handle.
 *  @retval    WGLR_INV_ARG        The argument h_rgn_list is NULL HANDLE.
 *  @retval    WGLR_OUT_OF_RESOURCE    There isn't sufficient memory.
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_add_transparent_list_exclusion(
                    HANDLE_T                h_rgn_list,
                    GL_RECT_T               at_exclusive_rect[],
                    UINT32                  ui4_rect_count);

/*! @brief  check if there is anything visible on the specified canvas
 *  @param  [in]    h_canvas            - canvas handle
 *  @note
 *  @see
 *  @return if there is anything visible on the specified canvas
 *  @retval      TRUE  there exists at least one visible widget on the specified canvas
 *  @retval      FALSE  there is no visible widget on the specified canvas
 */
/*------------------------------------------------------------------*/
extern BOOL     x_wgl_canvas_with_visibles(HANDLE_T h_canvas);

/*------------------------------------------------------------------*/
/*! @brief  This API is used to broadcast command message to the specified h_container and all its descendant widgets.
 *  @param  [in]    h_container            - Contains either the handle of the widget 
 *                              or the handle of the canvas, all widgets related to it will received the broadcasted message.
 *                              or the handle of the plane, all widgets related to  all canvases on the plane will received the broadcasted message.
 *                              is present, the created widget will be a top-level widget.
 *  @param  [in]    ui4_cmd             - Contains the command id. This is detailed in the individual widget, content or border document.
 *  @param  [in]    pv_param1           - References the data relevant to the specified command.Please refer to individual widget document.
 *  @param  [in]    pv_param2           - References the data relevant to the specified command.Please refer to individual widget document.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval      WGLR_INV_HANDLE     The argument h_container contains an invalid widget handle or canvas handle or plane handle.
 *  @retval     WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval      WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/

extern INT32    x_wgl_broadcast_cmd(
                    HANDLE_T                h_container,
                    UINT32                  ui4_cmd, 
                    VOID*                   pv_param1,
                    VOID*                   pv_param2);

/*------------------------------------------------------------------*/
/*! @brief  create a timer for the widget
 *  @param  [in]    h_widget            - Contains the widget handle
 *  @param  [in]    e_sync_mode         - timer mode,details please refer to WGL_TIMER_SYNC_MODE_T
 *  @param  [out]   ph_timer            - References the handle of the created timer if this routine was successful. In all other cases, this field is undefined.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval   WGLR_OK         The routine was successful.
 *  @retval      WGLR_INV_ARG    The argument ph_timer is NULL or e_sync_mode is invalid value. 
 *  @retval     WGLR_INV_HANDLE The argument h_widget contains an invalid widget handle.
 *  @retval       WGLR_OUT_OF_MEMORY  There is not enough memory to complete the operation.
 *  @retval      WGLR_OUT_OF_HANDLES There isn't enough handles or the caller's  handle resource is exhausted.
 *  @retval      WGLR_OUT_OF_RESOURCE    Timer created failed.
 *  @retval       WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval       WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_create_timer(
                    HANDLE_T                h_widget,
                    WGL_TIMER_SYNC_MODE_T   e_sync_mode,
                    HANDLE_T*               ph_timer);

/*------------------------------------------------------------------*/
/*! @brief  create a transparent list,UI among  transparent list regions in canvas will look through background video. after a list is successfully created, it is DISABLED by default
 *  @param  [in]    h_cnavas            - handle of the target canvas
 *  @param  [in]    at_rect             - an array of GL_RECT_T to specify the transparent regions
 *  @param  [in]    ui4_rect_count      - rectangle counts of at_rect[]
 *  @param  [out]   ph_rgn_list         - handle of the transparent list
 *  @note
 *  @see
 *  @return return the error code
 *  @retval    WGLR_OK             The routine was successful.
 *  @retval   WGLR_INV_HNADLE     The argument h_canvas contains an invalid canvas handle.
 *  @retval    WGLR_INV_ARG        The argument h_canvas is NULL handle or at_rect is NULL or ui4_rect_count is 0 .
 *  @retval    WGLR_OUT_OF_RESOURCE    There isn't sufficient memory..

 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_create_transparent_list(
                    HANDLE_T                h_canvas,               /* in:  handle of the target canvas*/
                    GL_RECT_T               at_rect[],              /* in:  an array of GL_RECT_T */
                    UINT32                  ui4_rect_count,         /* in:  rectangle counts of at_rect[] */
                    HANDLE_T                *ph_rgn_list);          /* out: handle of the transparent list */

/*------------------------------------------------------------------*/
/*! @brief  This API creates a new widget with type specified in 
 *              e_type_widget. If the creation succeeds, the new widget is 
 *              inserted into the widget hierarchy of the calling thread.
 *  @param  [in]    h_container         - Contains either the handle of the parent widget 
 *                              or the handle of the canvas on which this widget should be displayed. If a canvas handle 
 *                              is present, the created widget will be a top-level widget.
 *  @param  [in]    e_type_widget       - Contains the handle type of the widget to be created.
 *  @param  [in]    ui4_type_content    - Contains the content type of the widget to be created.
 *  @param  [in]    ui4_type_border     - Contains the border type of the widget to be created.
 *  @param  [in]    pt_rect             - References a GL_RECT_T structure which defines the display area of the widget to be created.
 *  @param  [in]    pf_wdgt_proc        - Contains the function pointer of the application defined message procedure for the 
 *                              new widget. A NULL value indicates the default widget message procedure will be used.
 *  @param  [in]    ui1_alpha           -  Contains the transparency level of the widget. 
 *                              A completely transparent widget is with value 0, while an opaque one is with value 255.
 *  @param  [in]    pv_param            -  References the data used for widget creation and initialization. The real content is dependent on the actual widget type.
 *  @param  [in]    pv_tag              -  Contains an application-defined tag value which is attached to the returned widget handle. The 
 *                                                     application can retrieve this value with the handle API c_handle_get_tag().
 *  @param  [out]   ph_widget           - References the handle of the created widget if this routine was successful. In all other cases, this field is undefined.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK         The routine was successful.
 *  @retval      WGLR_INV_ARG    The argument pt_rect is NULL or references an empty rectangle. Or ph_widget is NULL.
 *  @retval     WGLR_INV_HANDLE The argument h_parent contains an invalid widget handle except NULL_HANDLE.
 *  @retval      WGLR_INV_ TYPE  The argument e_type_widget contains an invalid 
 *                          widget type, or the argument ui4_type_content 
 *                          contains either an invalid or incompatible content 
 *                          type, or the argument ui4_type_border contains an invalid border type.
 *
 *  @retval       WGLR_OUT_OF_MEMORY  There is not enough memory to complete the operation.
 *  @retval      WGLR_OS_ERROR       This API is not called from a thread context.
 *  @retval       WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval       WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_create_widget(
                    HANDLE_T                h_container,
                    HANDLE_TYPE_T           e_type_widget,
                    UINT32                  ui4_type_content,
                    UINT32                  ui4_type_border,
                    const GL_RECT_T*        pt_rect,
                    wgl_widget_proc_fct     pf_wdgt_proc,
                    UINT8                   ui1_alpha,
                    VOID*                   pv_param,
                    VOID*                   pv_tag,
                    HANDLE_T*               ph_widget);

/*------------------------------------------------------------------*/
/*! @brief  This API destroys the timer represent by h_timer 
 *  @param  [in]    h_timer             - Contains the handle of the timer to be destroyed.
 *  @note
 *  @see c_wgl_create_timer()
 *  @return return the error code
 *  @retval  WGLR_OK             The function call is successful.
 *  @retval   WGLR_INV_ARG     The argument h_timer is NULL_HANDLE .
 *  @retval   WGLR_INV_HANDLE     The argument h_timer is not a valid timer handle.
 *  @retval    WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval    WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_delete_timer(HANDLE_T h_timer);

/*------------------------------------------------------------------*/
/*! @brief  This API destroys the transparent list represent by h_rgn_list 
 *  @param  [in]    h_rgn_list             - Contains the handle of the transparent list  to be destroyed.
 *  @note
 *  @see c_wgl_create_transparent_list()
 *  @return return the error code
 *  @retval  WGLR_OK             The function call is successful.
 *  @retval   WGLR_INV_HANDLE     The argument h_timer is not a valid timer handle.
 *  @retval    WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval    WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
extern INT32    x_wgl_delete_transparent_list(
                    HANDLE_T            h_rgn_list);
/*------------------------------------------------------------------*/
/*! @brief  This API calls the default message procedure of of h_widget. 
 *              For instance, if h_widget represents a Button widget, the 
 *              message will be routed to the default message procedure 
 *              provided by Button. The arguments passed to this API are 
 *              identical to those passed to the message procedure.
 *  @param  [in]    h_widget            - Contains the handle to the specified widget.
 *  @param  [in]    ui4_msg             - Contains  the message id.
 *  @param  [in]    pv_param1           -Specifies the additional information. The actual 
 *                      content of this argument depends on ui4_msg.
 *  @param  [in]    pv_param2           - Specifies more additional information. The actual 
 *                      content of this argument depends on ui4_msg.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval  WGLR_OK             The function call is successful.
 *  @retval   WGLR_INV_HANDLE     The argument h_widget is not a valid widegt handle.
 *  @retval    WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval    WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_default_msg_proc(
                    HANDLE_T            h_widget,
                    UINT32              ui4_msg,
                    VOID*               pv_param1,
                    VOID*               pv_param2);


/*------------------------------------------------------------------*/
/*! @brief  This API destroys all widgets owned by the calling thread. If 
 *              the calling thread is the thread of the current active 
 *              application, the application behind along the application list 
 *              becomes active.
 *  @param  [in] None
 *  @note
 *  @see
 *  @return return the error code
 *  @retval      WGLR_OK             The routine was successful.
 *  @retval     WGLR_INV_CONTEXT    No widgets are ever created in the calling thread.
 *  @retval    WGLR_OS_ERROR       This API is not called from a thread context.
 *  @retval    WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval    WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_destroy_app_widget(VOID);

/*------------------------------------------------------------------*/
/*! @brief  This API destroys the widget represent by h_widget. The widget 
 *              and all its child widgets are removed from the widget hierarchy 
 *              immediately. An additional repaint might be induced if 
 *              necessary.
 *  @param  [in]    h_widget            - Contains the handle of the widget to be destroyed.
 *  @note   ASYNChronous repaint would be triggered while destroying
 *  @see
 *  @return return the error code
 *  @retval      WGLR_OK             The routine was successful.
 *  @retval       WGLR_INV_HANDLE     The argument h_widget contains an invalid widget handle.
 *  @retval     WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval     WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_destroy_widget(
                    HANDLE_T            h_widget);

/*------------------------------------------------------------------*/
/*! @brief  This API destroys the widget represent by h_widget. The widget 
 *              and all its child widgets are removed from the widget hierarchy 
 *              immediately. An additional repaint might be induced if 
 *              necessary.
 *  @param  [in]    h_widget            -  Contains the handle of the widget to be destroyed.
 *  @param  [in]    b_repaint           - Indicate whether or not to repaint the specified widget and its children after being destroyed.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval     WGLR_INV_HANDLE     The argument h_widget contains an invalid widget handle.
 *  @retval       WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval       WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_destroy_widget_ex(
                    HANDLE_T            h_widget, 
                    BOOL                b_repaint);
/*------------------------------------------------------------------*/
/*! @brief  This API is used to change or obtain the states of the specified widget.
 *  @param  [in]    h_widget            - Contains the handle of the specified widget.
 *  @param  [in]    ui4_cmd             - Contains the command id. This is detailed in the individual widget, content or border document.
 *  @param  [in]    pv_param1           - References the data relevant to the specified command.Please refer to individual widget document.
 *  @param  [in]    pv_param2           - References the data relevant to the specified command.Please refer to individual widget document.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval      WGLR_INV_HANDLE     The argument h_widget contains an invalid widget handle.
 *  @retval     WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval      WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_do_cmd(
                    HANDLE_T            h_widget,
                    UINT32              ui4_cmd, 
                    VOID*               pv_param1,
                    VOID*               pv_param2);

/*------------------------------------------------------------------*/
/*! @brief  This API retrieves the focused widget of the calling thread and 
 *              dispatched the specified message to the message procedure of 
 *              the widget. Note that this API should be called in the application thread context.
 *  @param  [in]    ui4_msg             - Contains the message id.
 *  @param  [in]    pv_param1           - Specifies the additional information. The actual 
 *                      content of this argument depends on ui4_msg.
 *  @param  [in]    pv_param2           - Specifies more additional information. The actual 
 *                      content of this argument depends on ui4_msg.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval      WGLR_OK             The function call is successful.
 *  @retval    WGLR_INV_CONTEXT    No widgets are ever created in the calling thread.
 *  @retval    WGLR_EVENT_IGNORED  The passed message is ignored. That's probably because the delay exceeds the pre-setting delay tolerance.
 *  @retval    WGLR_OS_ERROR       This API is not called from a thread context.
 *  @retval     WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval     WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval      WGLR_NO_FOCUSED_WIDGET  There's no focused widget of the calling thread.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_dispatch_event(
                    UINT32              ui4_msg,
                    VOID*               pv_param1,
                    VOID*               pv_param2);

/*------------------------------------------------------------------*/
/*! @brief  This API enables/disables h_widget. A disabled widget does not 
 *              get the input focus and may be presented in a different outlook 
 *              from an enabled one. However, the ultimate appearance depends 
 *              on the implementation of the content and border of h_widget. 
 *              In addition, if h_widget is disabled, all its child widgets 
 *              will be disabled implicitly.
 *  @param  [in]    h_widget            - Contains the handle of the specified widge.
 *  @param  [in]    b_enable            - Contains TRUE for enabling the widget; FALSE for disabling it.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval     WGLR_INV_HANDLE     The argument h_widget contains an invalid widget handle.
 *  @retval      WGLR_INV_STATE      The argument b_enable is FALSE while h_widget is the current focused widget.
 *  @retval      WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval      WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_enable(
                    HANDLE_T            h_widget,  
                    BOOL                b_enable);

/*------------------------------------------------------------------*/
/*! @brief  This API change the enable state of a transparent list
 *  @param [in]     h_rgn_list          - handle of a transparent list
 *  @param [in]     b_enabled           - enable this list or not
 *  @param [in]     b_exclusive         - set its state as exclusive or union,
 *                                                     if value is TRUE menas exclusive, in this case if b_enabled is TRUE then all other transparent list belong the same canvas will be set to disable.
 *                                                     if value is FALSE means union then all other transparent list belongs to the same canvas will keep their enable states.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval     WGLR_INV_HANDLE     The argument h_rgn_list contains an invalid transparent list handle.
 *                                                    Or the handle of the canvas ,h_rgn_list belongs to ,is not valid handle.
 *                                                    Or no such transparent list in associated canvas.
 *  @retval   WGLR_INV_ARG        The argument h_rgn_list is NULL.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_enable_transparent_list(
                    HANDLE_T            h_rgn_list,             /* in: handle of a transparent list */
                    BOOL                b_enabled,              /* in: enable this list or not */
                    BOOL                b_exclusive);           /* in: set its state as exclusive or union */

/*------------------------------------------------------------------*/
/*! @brief  flip the specified canvas as the on-screen canvas
 *  @param  [in]    h_canvas            - canvas handle
 *  @note
 *  @see
 *  @return return the error code
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_flip_canvas(
                    HANDLE_T            h_canvas);

/*------------------------------------------------------------------*/
/*! @brief  This API set the specified widget and its descendant as float or not
 *  @param  [in]    h_widget            - Contains widget handle
 *  @param  [in]    b_float             - float or not
 *  @param  [in]    ui1_repaint         - repaint method, if is WGL_SYNC_AUTO_REPAINT or WGL_ASYNC_AUTO_REPAINT a repaint operation is occured.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK             The routine was successful.
  *  @retval     WGLR_INV_HANDLE     The argument h_widget contains an invalid widget handle.
  *  @retval      WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
  *  @retval      WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.

 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_float(
                    HANDLE_T            h_widget,
                    BOOL                b_float,
                    UINT8               ui1_repaint);

/*------------------------------------------------------------------*/
/*! @brief  get the app order
 *  @param  [in]    h_app               - APP handle
 *  @param  [out]   pui1_order          - order
 *  @note
 *  @see
 *  @return return the error code
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_get_app_order(
                    HANDLE_T            h_thread,
                    UINT8               *pui1_order);

/*------------------------------------------------------------------*/
/*! @brief  This API retrieves the handle of the canvas (a widget plane) associated with the specified OSD plane represented by h_plane.
 *                If there is no canvas accociated with the specified OSD plane then a new canvas handle will be created otherwise the canvas handle
 *                created before will be retured.
 *  @param  [in]    h_plane             -Contains the handle of the specified OSD plane.
 *  @param  [out]   ph_canvas           - References the handle of the associated canvas if this routine was successful. In all other cases, this field is undetermined.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval     WGLR_INV_HANDLE     The argument h_plane contains an invalid OSD plane handle.
 *  @retval   WGLR_INV_ARG        The argument ph_canvas is NULL.
 *  @retval     WGLR_OUT_OF_MEMORY  There is not enough memory to complete the operation.
 *  @retval      WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval      WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_get_canvas(
                    GL_HPLANE_T         h_plane,
                    HANDLE_T*           ph_canvas);

/*------------------------------------------------------------------*/
/*! @brief  This API retrieves the handle of the canvas (a widget plane) associated with the specified OSD plane represented by h_plane.
 *                If there is no canvas accociated with the specified OSD plane then a new canvas handle will be created otherwise 
 *                if there has been already existed one canvas handle related to the specified OSD plane then a shadow canvas related to the specified OSD plane
 *                will be created.
 *                Note: the canvas and its shadow canvas related to the same OSD plane will not show on UI simultaneously.
 *  @param  [in]    h_plane             -Contains the handle of the specified OSD plane.
 *  @param  [in]    pt_cfg               -Contains the configuration of the new canvas handle.Please refer to WGL_CANVAS_CFG_T for details.
 *  @param  [out]   ph_canvas           - References the handle of the associated canvas if this routine was successful. In all other cases, this field is undetermined.
 *  @note
 *  @see
 *  @return return the error code 
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval     WGLR_INV_HANDLE     The argument h_plane contains an invalid OSD plane handle.
 *  @retval     WGLR_INV_ARG        The argument ph_canvas is NULL. Or pt_cfg is null or contains invalid configuration value.
 *  @retval     WGLR_SHADOW_EXIST  There is already one canvas and one shadow canvas related to the specified OSD plane.
 *  @retval     WGLR_OUT_OF_MEMORY  There is not enough memory to complete the operation.
 *  @retval      WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval      WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_get_canvas_ex(
                    GL_HPLANE_T             h_plane,
                    const WGL_CANVAS_CFG_T* pt_cfg,
                    HANDLE_T*               ph_canvas);

/*------------------------------------------------------------------*/
/*! @brief  get canvas information for the canvas represented by h_obj or return global system canvas infomation if h_obj is null.
 *  @param  [in]    h_obj               - Contains a canvas or widget handle, allows to be NULL.
 *  @param  [out]   pt_cvs_info         - canvas information
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK             The routine was successful.
  *  @retval     WGLR_INV_HANDLE     The argument h_obj is not NULL and not an invalid canvans handle or widget handle.
  *  @retval     WGLR_INV_ARG        The argument pt_cvs_info is NULL. 
  *  @retval      WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
  *  @retval      WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.

 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_get_canvas_info(
                    HANDLE_T            h_obj,
                    WGL_CANVAS_INFO_T*  pt_cvs_info);

/*------------------------------------------------------------------*/
/*! @brief  get palette of  the canvas represented by h_obj or return global system palette if h_obj is null.
 *  @param  [in]    h_obj               - Contains a canvas or widget handle, allows to be NULL.
 *  @param  [out]   ph_palette         - Contains a palette handle to be retrieved.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK             The routine was successful.
  *  @retval     WGLR_INV_HANDLE     The argument h_obj is not NULL and not an invalid canvans handle or widget handle.
  *  @retval     WGLR_INV_ARG        The argument ph_palette is NULL. 
  *  @retval      WGLR_OUT_OF_HANDLES There isn't enough handles or the caller's  handle resource is exhausted.
  *  @retval      WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
  *  @retval      WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.

 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_get_canvas_palette(
                    HANDLE_T        h_obj,
                    GL_HPALETTE_T*  ph_palette);

/*------------------------------------------------------------------*/
/*! @brief  This API retrieves the dimension of the specified canvas. Note 
 *              that the size returned is actually the size of the internal frame buffer.
 *  @param  [in]    h_canvas            - Contains the handle of the specified canvas.
 *  @param  [out]   pui4_width          - References the width of the internal frame buffer 
 *                          if this routine was successful. In all other cases, this field is undetermined.
 *  @param  [out]   pui4_height         -References the height of the internal frame buffer 
 *                          if this routine was successful. In all other cases, this field is undetermined.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval      WGLR_INV_HANDLE     The argument h_plane contains an invalid OSD plane handle.
 *  @retval    WGLR_INV_ARG        The argument ph_canvas is NULL.
 *  @retval      WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval      WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_get_canvas_size(
                    HANDLE_T    h_canvas,
                    UINT32*     pui4_width,
                    UINT32*     pui4_height);

/*------------------------------------------------------------------*/
/*! @brief  This API retrieves the plane handle which the canvas belongs to.
 *  @param  [in]    h_canvas            - Contains the handle of the specified canvas.
 *  @param  [out]   ph_plane          - References the plane handle 
 *                          if this routine was successful. In all other cases, this field is undetermined.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval      WGLR_INV_HANDLE     The argument h_canvas contains an invalid canvas handle.
 *  @retval    WGLR_INV_ARG        The argument ph_plane is NULL.
 *  @retval      WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval      WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_get_canvas_plane(
                     HANDLE_T       h_canvas,
                     GL_HPLANE_T*   ph_plane);
/*------------------------------------------------------------------*/
/*! @brief  This API destroy the canvas referenced h_canvas, the surface resources and transparent regions 
 * linked to this canvas will also be released. The canvas will only be destroyed in following two conditons satisfied:
 * 1, If the canvas has shadow canvas linked to it then the shadow canvas needs to be destroyed first.
 * 2, All APPs who used this canvas need to be unregistered(using x_wgl_unregister()) first.
 *  @param  [in]    h_canvas            - Contains the handle of the specified canvas.
 *  @param  [out]  NULL
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval      WGLR_INV_HANDLE     The argument h_plane contains an invalid OSD plane handle.
 *  @retval      WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval      WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval      WGLR_CANVAS_HAS_SHADOW   The canvas has one shadow canvas linked to it.
 *  @retval      WGLR_CANVAS_IS_USEING   There exist one app who is using this canvas.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_destroy_canvas(
                    HANDLE_T    h_canvas);

/*------------------------------------------------------------------*/
/*! @brief  query the children order of a parent
 *  @param  [in]        h_h_parent      - parent widget handle
 *  @param  [out]       ah_children     - array to store the children handle in order
 *  @param  [in/out]    pi4_number      - [in] specify the array size [out] children number
 *  @note
 *  @see
 *  @return return the error code
 *  @retval WGLR_OK                     - ok
 *  @retval WGLR_INV_HANDLE             - an invalid handle is passed
 *  @retval WGLR_INV_ARG                - invalid argument is passed
 *  @retval WGLR_BUFFER_NOT_ENOUGH      - children number is larger than the array size
 *  @retval WGLR_LOCK_ERROR             - WGL lock error
 *  @retval WGLR_UNLOCK_ERROR           - WGL unlock error
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_get_children_order(
                    HANDLE_T            h_parent,
                    HANDLE_T            ah_children[],
                    INT32               *pi4_number);

/*------------------------------------------------------------------*/
/*! @brief This API retrieves the coordinates of the bounding rectangle of the specified widget.
 *  @param  [in]    h_widget            - Contains the handle of the specified widget.
 *  @param  [in]    e_coords            -Contains an enumerated value to indicate whether the 
 *                      coordinates should be in screen coordinates or in the local coordinates of the parent of h_widget.
 *  @param  [out]   pt_rect             -  References a GL_RECT_T structure which contains the 
 *                      coordinates of the bounding rectangle of h_widget if this routine was successful. In all other cases, this field is undetermined.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval    WGLR_INV_HANDLE     The argument h_widget contains an invalid widget handle.
 *  @retval     WGLR_INV_ARG        Either the argument e_coords contains an invalid enumerated value or pt_rect is NULL.
 *  @retval       WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval     WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_get_coords(
                    HANDLE_T            h_widget,
                    WGL_COORDS_T        e_coords,
                    GL_RECT_T*          pt_rect);
#ifdef WGL_COORDINATE_INVERSE    
/*------------------------------------------------------------------*/
/*! @brief This API retrieves the coordinates of the bounding rectangle of the specified widget.
 *  @param  [in]    h_widget            - Contains the handle of the specified widget.
 *  @param  [in]    e_coords            -Contains an enumerated value to indicate whether the 
 *                      coordinates should be in screen coordinates or in the local coordinates of the parent of h_widget.
 *  @param  [out]   pt_rect             -  References a GL_RECT_T structure which contains the 
 *                      coordinates of the bounding rectangle of h_widget if this routine was successful. In all other cases, this field is undetermined.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval    WGLR_INV_HANDLE     The argument h_widget contains an invalid widget handle.
 *  @retval     WGLR_INV_ARG        Either the argument e_coords contains an invalid enumerated value or pt_rect is NULL.
 *  @retval       WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval     WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_get_coords_for_inverse(
                    HANDLE_T            h_widget,
                    WGL_COORDS_T        e_coords,
                    GL_RECT_T*          pt_rect);
#endif
/*------------------------------------------------------------------*/
/*! @brief  This API retrieves the current focused widget of the calling thread.
 *  @param  [out]   ph_widget           - References the handle of the current focused widget if this routine was successful. In all other cases, this field is undetermined.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval      WGLR_OK             The routine was successful.
 *  @retval     WGLR_INV_ARG        The argument ph_widget is NULL.
 *  @retval       WGLR_INV_CONTEXT    No widgets are ever created in the calling thread.
 *  @retval     WGLR_OS_ERROR       This API is not called from a thread context.
 *  @retval   WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval     WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval     WGLR_NO_FOCUSED_WIDGET  There's no focused widget of the calling thread.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_get_focus(
                    HANDLE_T*           ph_widget);

/*------------------------------------------------------------------*/
/*! @brief  This API retrieves the handle of the parent of the specified 
 *              widget h_widget. If h_widget is a top-level widget, NULL_HANDLE is returned.
 *  @param  [in]    h_widget            - Contains the handle of the specified widget.
 *  @param  [out]   ph_parent           -References the parent handle of h_widget if this 
 *                      routine was successful. In all other cases, this field is undetermined.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval      WGLR_OK             The routine was successful.
 *  @retval      WGLR_INV_HANDLE     The argument h_widget contains an invalid widget handle.
 *  @retval       WGLR_INV_ARG        The argument ph_parent is NULL.
 *  @retval       WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval     WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_get_parent(
                    HANDLE_T            h_widget,
                    HANDLE_T*           ph_parent);

/*------------------------------------------------------------------*/
/*! @brief  This API retrieves the state of the specified widget h_widget. 
 *  @param  [in]    h_widget            - Contains the handle of the specified widget.
 *  @param  [out]   pui4_state           -Contains the state of h_widget widget if this routine was successful. 
 *                      The state maybe is WGL_STATE_ENABLED, WGL_STATE_VISIBLE,WGL_STATE_ACTIVATED , WGL_STATE_FOCUSED or combined states.
 *                      In all other cases, this field is undetermined.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval      WGLR_OK             The routine was successful.
 *  @retval      WGLR_INV_HANDLE     The argument h_widget contains an invalid widget handle.
 *  @retval       WGLR_INV_ARG        The argument pui4_state is NULL.
 *  @retval       WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval     WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_get_state(
                    HANDLE_T        h_widget,
                    UINT32*         pui4_state);

/*------------------------------------------------------------------*/
/*! @brief   This API retrieves the invalid region that needs to be updated. 
 *              Note that the region is specified in the local coordinates of h_widget.
 *  @param  [in]        h_widget        -Contains the handle of the specified widget.
 *  @param  [out]       pui4_num_rect   References the number of GL_RECT_T entries referenced by pt_upt_rect if this routine was successful. In all other cases, this field is undetermined.
 *  @param  [in/out]    pt_upt_rect     This is an input/output argument. If this is NULL, pui4_num_rect is set with the number of invalid rectangles, which can be used to calculate the appropriate memory size to hold the rectangle structure. If this is not NULL, it references an array of GL_RECT_T which contains the list of invalid rectangles of the widget if this routine was successful. In all other cases, this field is undetermined.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval     WGLR_INV_HANDLE     The argument h_widget contains an invalid widget handle.
 *  @retval      WGLR_INV_ARG        The argument pui4_num_rect is NULL.
 *  @retval       WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval       WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_get_update_region(
                    HANDLE_T        h_widget,
                    UINT32*         pui4_num_rect,
                    GL_RECT_T*      pt_upt_rect);

/*------------------------------------------------------------------*/
/*! @brief  This API retrieves the visibility of the specified widget and all its ancestors. 
 *  @param  [in]    h_widget            - Contains the handle of the specified widget.
 *  @param  [out]   pb_visible           -Contains the visibility of h_widget widget if this routine was successful. 
 *                      *pb_visible is TRUE if and only if the widget and all its ancestors are visible.
 *                      In all other cases, this field is undetermined.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval      WGLR_OK             The routine was successful.
 *  @retval      WGLR_INV_HANDLE     The argument h_widget contains an invalid widget handle.
 *  @retval       WGLR_INV_ARG        The argument pui4_state is NULL.
 *  @retval       WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval     WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/

extern INT32    x_wgl_get_visibility(
                    HANDLE_T        h_widget,
                    BOOL            *pb_visible);

/*------------------------------------------------------------------*/
/*! @brief  This API retrieves tmessage process functio of the specified widget and all its ancestor. 
 *  @param  [in]    h_widget            - Contains the handle of the specified widget.
 *  @param  [out]   ppf_proc           -Contains the message process function pointer if this routine was successful. 
 *                                                    In all other cases, this field is undetermined.
 *  @note
 *  @see c_wgl_set_widget_proc()
 *  @return return the error code
 *  @retval      WGLR_OK             The routine was successful.
 *  @retval      WGLR_INV_HANDLE     The argument h_widget contains an invalid widget handle.
 *  @retval       WGLR_INV_ARG        The argument ppf_proc is NULL.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_get_widget_proc(
                    HANDLE_T                h_widget,
                    wgl_widget_proc_fct     *ppf_proc);
/*------------------------------------------------------------------*/
/*! @brief  make a widget pass the visibility detect or not
 *  @param  [in]    h_widget            - widget handle
 *  @param  [in]    b_ignore            - ignore or not
 *  @note
 *  @see
 *  @return return the error code
 *  @retval      WGLR_OK             The routine was successful.
 *  @retval     WGLR_INV_HANDLE     The argument h_widget contains an invalid widget handle.
 *  @retval      WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval     WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/

extern INT32    x_wgl_ignore_visibility_detect(
                    HANDLE_T    h_widget,
                    BOOL        b_ignore);
/*------------------------------------------------------------------*/
/*! @brief  This API inserts the specified widget before/after the 
 *              reference widget along the z-axis. Note that if h_ref_widget is 
 *              NULL_HANDLE, h_widget is placed at the top/bottom-most level.
 *  @param  [in]    h_widget            - Contains the widget handle.
 *  @param  [in]    h_ref_widget        - Contains the handle of the reference widget. Note that this argument can be NULL_HANDLE.
 *  @param  [in]    e_insert            - Contains an enumerated value to specify inserting h_widget before or after h_ref_widget.
 *  @param  [in]    ui1_repaint         - Contains a flag indicating if synchronous/asynchronous repainting should be 
 *                          performed automatically after the insertion is done.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval      WGLR_OK             The routine was successful.
 *  @retval     WGLR_INV_HANDLE     The argument h_widget or h_ref_widget contains an invalid widget handle.
 *  @retval       WGLR_INV_ARG        The argument e_insert is invalid.
 *  @retval      WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval     WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/

extern INT32    x_wgl_insert(
                    HANDLE_T            h_widget,
                    HANDLE_T            h_ref_widget,
                    WGL_INSERT_MODE_T   e_insert,
                    UINT8               ui1_repaint);
/*------------------------------------------------------------------*/
/*! @brief  This API changes the position or size of the specified widget.
 *  @param  [in]    h_widget            - Contains the widget handle.
 *  @param  [in]    pt_rect             - Reference a GL_RECT_T structure defining the new 
 *                      position and size h_widget. The coordinates are 
 *                      relative to the top-left corner of the parent of 
 *                      h_widget. If h_widget is a top-level widget, this is 
 *                      given in screen coordinates.
 *  @param  [in]    ui1_repaint         - Contains a flag indicating if synchronous/asynchronous 
 *                      repainting should be performed automatically after the move is done.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval      WGLR_OK             The routine was successful.
 *  @retval       WGLR_INV_HANDLE     The argument h_widget contains an invalid widget handle.
 *  @retval     WGLR_INV_ARG        The argument pt_rect is NULL or pt_rect references an empty rectangle.
 *  @retval       WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval       WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/

extern INT32    x_wgl_move(
                    HANDLE_T            h_widget,
                    const GL_RECT_T*    pt_rect,
                    UINT8               ui1_repaint);
/*------------------------------------------------------------------*/
/*! @brief  This API get information from the canvas information callback
 *  @param  [in]    h_canvas            -Contains canvas handle
 *  @param  [in]    e_type              - information type,it should be WGL_CVS_CB_TXT_DIR at moment.
 *  @param  [in,out]    pz_size              - Contains size of pv_data, should be sizeof(UINT8)
 *  @param  [out]    pv_data           - Contains callback function pointer, it will be set to widget's canvas's  wgl_canvas_info_callback_fct callback function  if e_type is WGL_CVS_CB_TXT_DIR 
 *  @note
 *  @see
 *  @return return the error code
 *  @retval WGLR_OK                 - The routine was successful.
 *  @retval WGLR_INV_HANDLE         - h_widget is an invalid widget handle
 *  @retval WGLR_INV_ARG            - One of following case:
 *                                                   h_widget is NULL handle or 
 *                                                   pv_data is NULL or
 *                                                    pz_size is NULL or *pz_size != sizeof(UINT8) or
 *                                                    e_type is not equals to WGL_CVS_CB_TXT_DIR
 *  @retval WGLR_FUNC_NOT_IMPL         - widget's canvas has no wgl_canvas_info_callback_fct  callback function attached.
 */
/*------------------------------------------------------------------*/

extern INT32    x_wgl_query_canvas_info_callback(
                    HANDLE_T            h_widget,
                    WGL_CVS_CB_TYPE_T   e_type,
                    VOID                *pv_data,
                    SIZE_T              *pz_size);

/*------------------------------------------------------------------*/
/*! @brief  This API registers the calling thread with the Widget Manager.This API 
 *              sets the callback function of the application. The 
 *              callback function is invoked whenever a user input event 
 *              occurs.
 *  @param  [in]    pf_callback         -Contains the application defined callback function.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval      WGLR_OK             The routine was successful.
 *          WGLR_INV_ARG        Either the argument pf_callback.
 *  @retval     WGLR_OS_ERROR       This API is not called from a thread context.
 *  @retval      WGLR_OUT_OF_MEMORY  There is not enough memory to complete the operation.
 *  @retval       WGLR_ALREADY_REGISTERED  This API has been called from the calling thread context.
 *  @retval      WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval       WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/

extern INT32    x_wgl_register(
                    wgl_app_callback_fct    pf_callback,
                    HANDLE_T*               ph_client);

/*------------------------------------------------------------------*/
/*! @brief  This API issues a repaint request to the Repaint Manager. 
 *              Depends on the value of b_sync, this API could execute in an 
 *              asynchronous or a synchronous way. If the asynchronous behavior 
 *              is selected, a painting message is sent to the Repaint 
 *              Manager's thread and this API returns immediately. On the 
 *              contrary, if the synchronous behavior is chosen, this routine 
 *              will not return until the completion of the requested 
 *              repainting or until the occurrence of an error.
 *  @param  [in]    h_container         - Contains the widget or the canvas handle being updated.
 *  @param  [in]    pt_rect             - References a GL_RECT_T structure which gives the area 
 *                      being redrawn. The coordinates specified in this 
 *                      structure are in the coordinates of h_container. A 
 *                      NULL value indicates that whole area of h_container 
 *                      must be updated.
 *  @param  [in]    b_sync              - If this argument is TRUE, the requested painting 
 *                      executes synchronously. Otherwise it executes asynchronously.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval    WGLR_INV_HANDLE     The argument h_container contains an invalid handle.
 *  @retval      WGLR_INV_ARG        The argument pt_rect is not NULL but the intersection of pt_rect and the widget area is empty.
 *  @retval     WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/

extern INT32    x_wgl_repaint(
                    HANDLE_T            h_container,  
                    const GL_RECT_T*    pt_rect,
                    BOOL                b_sync);
/*------------------------------------------------------------------*/
/*! @brief  his API sends the specified message and its accompanying data 
 *              to h_widget. The routine is synchronous and doesn't return 
 *              until the requested operation is accomplished.
 *  @param  [in]    h_widget            - Contains the widget handle that will receive the message.
 *  @param  [in]    ui4_msg             -Contains the message id.
 *  @param  [in]    pv_param1           - Specifies the additional information. The actual content of this argument depends on ui4_msg.
 *  @param  [in]    pv_param2           - Specifies more additional information. The actual content of this argument depends on ui4_msg.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval      WGLR_OK             The routine was successful.
 *  @retval    WGLR_INV_HANDLE     The argument h_widget does not contain a valid widget handle.
 *  @retval    WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval     WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/

extern INT32    x_wgl_send_msg(
                    HANDLE_T            h_widget,
                    UINT32              ui4_msg,
                    VOID*               pv_param1,
                    VOID*               pv_param2);
/*------------------------------------------------------------------*/
/*! @brief  This API sends a WGL_MSG_NOTIFY message to h_parent of the source widget  with the notification data specified in pt_nfy_data. 
 *  @param  [in]    h_parent            - Contains the widget's parent handle that will receive the notification.
 *  @param  [in]    ui4_nc_code         - Contains the notification code
 *  @param  [in]    h_widget            - Contains the source widget handle 
 *  @param  [in]    pt_nfy_data         - References a WGL_NOTIFY_DATA_T structure which contains the information relevant to this notification. 
 *  @note
 *  @see
 *  @return return the error code
 *  @retval    WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_HNADLE The argument h_widget contains an invalid widget handle.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 
 */
/*------------------------------------------------------------------*/

extern INT32    x_wgl_send_notify(
                    HANDLE_T            h_parent,
                    UINT32              ui4_nc_code,
                    HANDLE_T            h_widget,
                    WGL_NOTIFY_DATA_T*  pt_nfy_data);
/*------------------------------------------------------------------*/
/*! @brief  This API sets the application whose main thread is h_thread as 
 *              the current active application. Those widgets of the active 
 *              application are displayed on top of all other application 
 *              widgets. The user input is routed to the active application as well.
 *  @param  [in]    h_app               - Contains the handle of the main thread of the application to be activated.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval     WGLR_INV_HANDLE     The argument h_thread does not contain a valid  thread handle.
 *  @retval    WGLR_LOCK_ERROR     Locking the internal protection semaphore  failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/

extern INT32    x_wgl_set_active_app(HANDLE_T h_thread);

/*------------------------------------------------------------------*/
/*! @brief  This API sets the alpha value of the specified widget.
 *  @param  [in]    h_widget            - Contains the widget whose alpha is going to be changed.
 *  @param  [in]    ui1_alpha           - Contains the alpha value.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval      WGLR_OK             The routine was successful.
 *  @retval      WGLR_INV_HANDLE     The argument h_widget does not contain a valid widget handle.
 *  @retval       WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval     WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/

extern INT32    x_wgl_set_alpha(
                    HANDLE_T            h_widget,
                    UINT8               ui1_alpha);
/*------------------------------------------------------------------*/
/*! @brief  set the key handling function for ignored key
 *  @param  [in]    pf_app_ignore_key_fct   - callback function
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK             The routine was successful.
 */
/*------------------------------------------------------------------*/

extern INT32    x_wgl_set_app_ignore_key_fct(wgl_app_callback_fct pf_app_ignore_key_fct);
/*------------------------------------------------------------------*/
/*! @brief  set the key handling function for widget ignored key
 *  @param  [in]    pf_widget_ignore_key_fct   - callback function
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK             The routine was successful.
 */
/*------------------------------------------------------------------*/

extern INT32   x_wgl_set_widget_ignore_key_fct(wgl_widget_callback_fct pf_widget_ignore_key_fct);

/*------------------------------------------------------------------*/
/*! @brief  set the APP order
 *  @param  [in]    h_app               - APP handle
 *  @param  [in]    ui1_order           - order to be set, maybe is WGL_APP_NOT_TO_END,WGL_APP_TOPMOST,WGL_APP_BOTTOMMOST
 *  @note 
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK             The routine was successful.
*  @retval     WGLR_INV_HANDLE         - he argument h_app does not contain a valid APP handle.
 *  @retval     WGLR_INV_ARG            - ui1_order is not WGL_APP_NOT_TO_END,WGL_APP_TOPMOST or WGL_APP_BOTTOMMOST
 *  @retval     WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval     WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
  */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_set_app_order(HANDLE_T h_thread, UINT8 ui1_dir);

/*------------------------------------------------------------------*/
/*! @brief  set the canvas callback function
 *  @param  [in]    h_canvas            - Contains the canvas handle
 *  @param  [in]    pf_cvs_nfy_fct      -Contains callback function to receive the canvas notifications
 *  @param  [in]    b_enable            - to enable or disable this callback.
 *                                                     if b_enable is FALSE and pf_cvs_nfy_fct has been already set in h_canvas then means disable the pf_cvs_nfy_fct 
 *                                                     so pf_cvs_nfy_fct will be removed from canvas's callback function list.
 *                                                     if b_enable is TRUE and pf_cvs_nfy_fct is no set in h_canvas then means enable the pf_cvs_nfy_fct 
 *                                                     so pf_cvs_nfy_fct will be added in canvas's callback function list.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval     WGLR_INV_HANDLE         - he argument h_canvas does not contain a valid canvas handle.
 *  @retval     WGLR_INV_ARG            - pf_cvs_nfy_fct is NULL
 *  @retval     WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval     WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_set_canvas_callback(
    HANDLE_T                    h_canvas,
    wgl_canvas_callback_fct     pf_cvs_nfy_fct,
    BOOL                        b_enable);

/*------------------------------------------------------------------*/
/*! @brief  set the canvas information callback function
 *  @param  [in]    h_canvas            - canvas handle
 *  @param  [in]    e_type              - information type,it should be WGL_CVS_CB_TXT_DIR at moment.
 *  @param  [in]    pv_cb_fct           - callback function pointer, it should be (*wgl_canvas_info_callback_fct)()  type if e_type is WGL_CVS_CB_TXT_DIR 
 *  @note
 *  @see
 *  @return return the error code
 *  @retval WGLR_OK                 - ok
 *  @retval      WGLR_OUT_OF_HANDLES There isn't enough handles or the caller's  handle resource is exhausted.
 *  @retval WGLR_INV_HANDLE         - h_canvas is an invalid canvas handle
 *  @retval WGLR_INV_ARG            - e_type is not equals to WGL_CVS_CB_TXT_DIR
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_set_canvas_info_callback(
    HANDLE_T                    h_canvas,
    WGL_CVS_CB_TYPE_T           e_type,
    VOID                        *pv_cb_fct);

/*------------------------------------------------------------------*/
/*! @brief  This API change  the canvas's configuration 
 *  @param  [in]    h_canvas            - Contains the canvas handle
 *  @param  [in]    pt_cvs_clr      -Contains config infomation. Please refer to WGL_CVS_CLR_CFG_T for details.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval     WGLR_INV_HANDLE         - he argument h_canvas does not contain a valid canvas handle.
 *  @retval     WGLR_INV_ARG            - pf_cvs_nfy_fct is NULL
 *  @retval    WGLR_INTERNAL_ERROR   -Some internal calls return error.  
 *  @retval     WGLR_INV_COLORMODE   -canvas's color mode is dismatched with pt_cvs_clr   
 *  @retval     WGLR_LOCK_ERROR     -Locking the internal protection semaphore failed.
 *  @retval     WGLR_UNLOCK_ERROR   -Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_set_canvas_color(
                    HANDLE_T                    h_canvas,
                    const WGL_CVS_CLR_CFG_T*    pt_cvs_clr);

/*------------------------------------------------------------------*/
/*! @brief  set the children order of a parent
 *  @param  [in]        h_h_parent  - parent widget handle
 *  @param  [in]        ah_children - array to store the children handle in the desired order
 *  @param  [in]        i4_number   - specify the array size
 *  @note
 *  @see
 *  @return return the error code
 *  @retval WGLR_OK                 - ok
 *  @retval WGLR_INV_HANDLE         - an invalid handle is passed
 *  @retval WGLR_INV_ARG            - invalid argument is passed
 *  @retval WGLR_BUFFER_NOT_ENOUGH  - children number is larger than the array size
 *  @retval WGLR_LOCK_ERROR         - WGL lock error
 *  @retval WGLR_UNLOCK_ERROR       - WGL unlock error
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_set_children_order(
                    HANDLE_T            h_parent,
                    HANDLE_T            ah_children[],
                    INT32               i4_number);

/*------------------------------------------------------------------*/
/*! @brief  This API sets the key delay tolerance of the application corresponding to the calling thread.
 *  @param  [in]    ui4_delay           - Contains the key delay tolerance in ms. Note that this 
 *                      vlaue represents the maximum delay between the time 
 *                      where the event occurred and where the event is 
 *                      dispatched. If the time difference (lag) exceeds the 
 *                      delay tolerance, the key event is ignored.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval      WGLR_INV_CONTEXT    The calling thread is not a registered application thread.
 *  @retval       WGLR_OS_ERROR       This API is not called from a thread context.
 *  @retval      WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval     WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_set_delay_tolerance(UINT32 ui4_delay);

/*------------------------------------------------------------------*/
/*! @brief  This API sets h_widget as the focused widget of its owner 
 *              application. The routine sends a WGL_MSG_LOSE_FOCUS message to 
 *              the widget that loses the focus and a WGL_MSG_GET_FOCUS message 
 *              to the widget that receives the focus. If h_widget is a 
 *              top-level widget, it also receives a WGL_MSG_ACTIVATE message, 
 *              or the top-level parent of h_widget receives it.
 *  @param  [in]    h_widget            - Contains the widget handle that will get the focus.
 *  @param  [in]    ui1_repaint         - Contains a flag indicating if synchronous/asynchronous 
 *                      repainting should be performed automatically after changing the focus.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK  Success.
 *  @retval     WGLR_INV_HNADLE The argument h_widget contains an invalid widget handle.
 *  @retval      WGLR_CANNOT_BE_FOCUSED  The specified widget h_widget cannot be focused. A widget can only be focused if all its parent and ancestors are enabled.
 *  @retval      WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval      WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_set_focus(
                    HANDLE_T            h_widget,
                    UINT8               ui1_repaint);

/*------------------------------------------------------------------*/
/*! @brief  This API sets the navigation map of h_widget. Once the 
 *              navigation map is set, every time x_wgl_dispatch_event() or 
 *              x_wgl_send_msg() is called, the navigation map of the 
 *              destination widget is checked. If the message being sent is 
 *              WGL_MSG_KEY_DOWN and the key code matches one of the keys in 
 *              the navigation map, the Widget Manager sets the focus to the 
 *              widget corresponding to the key in the navigation map. In this 
 *              situation, the message itself, however, is not routed to the 
 *              message procedure of the original destination widget.
 *
 *  @param  [in]    h_widget            - Contains the widget handle that will get the focus.
 *  @param  [in]    ui1_cnt             - Contains the number of entries of at_key_lnk. This number must fall in the range (0, WGL_MAX_KEY_LINK).
 *  @param  [in]    at_key_lnk          - References an array of WGL_KEY_LINK_T structure. Each 
 *                      structure entry defines the mapping of key and target widget handle.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval   WGLR_OK             The routine was successful.
 *  @retval   WGLR_INV_HNADLE     The argument h_widget contains an invalid widget handle.
 *  @retval   WGLR_INV_ARG        The argument at_key_lnk is NULL or ui1_cnt is not in the valid range.
 *  @retval   WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval    WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_set_navigation(
                    HANDLE_T                h_widget,
                    UINT8                   ui1_cnt,
                    const WGL_KEY_LINK_T    at_key_lnk[]);

/*------------------------------------------------------------------*/
/*! @brief  set the blocking time for changing from the UI canvas to the Shadow one
 *  @param  [in]    ui4_timeout         - timeout time
 *  @note
 *  @see
 *  @return return the error code
 *  @retval    WGLR_OK             The routine was successful.
*  @retval    WGLR_INV_ARG        The argument ui4_timeout is 0. 

 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_set_shadow_canvas_block_time(
                    UINT32              ui4_timeout);

/*------------------------------------------------------------------*/
/*! @brief  reset the transparent list region for the transparent list handle,the enable state is not affected by this operation.
 *  @param  [in]    h_rgn_list          -Contains the region list handle created using c_wgl_create_transparent_list()
 *  @param  [in]    at_rect             - Contains an array of GL_RECT_T to specify the transparent regions
 *  @param  [in]    ui4_rect_count      -  Contains rectangle counts of at_rect[]
 *  @note
 *  @see c_wgl_create_transparent_list()
 *  @return return the error code
  *  @retval    WGLR_OK             The routine was successful.
  *  @retval   WGLR_INV_HNADLE     The argument h_rgn_list contains an invalid region handle.
  *  @retval    WGLR_INV_ARG        The argument h_rgn_list is NULL handle or at_rect is NULL or ui4_rect_count is 0 .
  *  @retval    WGLR_OUT_OF_RESOURCE    There isn't sufficient memory..
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_set_transparent_list(
                    HANDLE_T            h_rgn_list,
                    GL_RECT_T           at_rect[],
                    UINT32              ui4_rect_count);

/*------------------------------------------------------------------*/
/*! @brief  This API is used to alter the visibility flag of the specified widget and/or its child widgets.
 *  @param  [in]    h_widget           -Contains the widget handle.
 *  @param  [in]    e_sw_cmd           - Contains an enumerated value of type WGL_SW_CMD_T, which defines the way to set the visibility flag.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval    WGLR_OK             The routine was successful.
  *  @retval   WGLR_INV_HNADLE     The argument h_widget contains an invalid widget handle.
 *  @retval    WGLR_INV_ARG        The argument e_sw_cmd contains an invalid enumerated value.
  *  @retval    WGLR_LOCK_ERROR     Locking the internal protection semaphore  failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_set_visibility(
                    HANDLE_T            h_widget,
                    WGL_SW_CMD_T        e_sw_cmd);

/*------------------------------------------------------------------*/
/*! @brief  This API sets the widget proc callback of the specified widget.
 *  @param  [in]    h_widget            - Contains the widget whose alpha is going to be changed.
 *  @param  [in]    pf_wdgt_proc   Contains the widget message procedure function pointer and this value can not be NULL.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval      WGLR_OK             The routine was successful.
 *  @retval      WGLR_INV_ARG         pf_wdgt_proc is NULL.
 *  @retval      WGLR_INV_HANDLE     The argument h_widget does not contain a valid widget handle.
 *  @retval       WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval     WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/

extern INT32    x_wgl_set_widget_proc(
                     HANDLE_T               h_widget,
                        wgl_widget_proc_fct pf_wdgt_proc);

/*------------------------------------------------------------------*/
/*! @brief  Invocation of this API shows/hides the specified widget and/or its child widgets.
 *  @param  [in]    h_widget            - Contains the widget handle.
 *  @param  [in]    e_sw_cmd            - Contains an enumerated value of type WGL_SW_CMD_T,which defines the way to display the widget.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval   WGLR_OK             The routine was successful.
 *  @retval   WGLR_INV_HNADLE     The argument h_widget contains an invalid widget handle.
 *  @retval   WGLR_INV_ARG        The argument e_sw_cmd contains an invalid enumerated value.
*  @retval    WGLR_LOCK_ERROR     Locking the internal protection semaphore  failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore  failed.  
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_show(
                    HANDLE_T            h_widget,
                    WGL_SW_CMD_T        e_sw_cmd);

/*------------------------------------------------------------------*/
/*! @brief  start a widget timer which is created uing c_wgl_create_timer()
 *  @param  [in]    h_timer             - Contains the timer handle to be started
 *  @param  [in]    ui4_delay           - Contains the timer delay time,for every ui4_delay time interval the widget will recieve WGL_MSG_TIMER in widget's message procedure function:(*wgl_widget_proc_fct)()
 *  @param  [in]    e_flag              - Contains timer flag, details please refer to TIMER_FLAG_T
 *  @param  [in]    pv_tag              -Contains private tag data which will be set to pv_param2 argument in widget's message procedure function: (*wgl_widget_proc_fct)()  
 *  @note
 *  @see c_wgl_create_timer()
 *  @return return the error code
 *  @retval   WGLR_OK             The routine was successful.
  *  @retval   WGLR_INV_HNADLE     The argument h_timer contains an invalid timer handle.
  *  @retval   WGLR_INV_ARG        The argument h_timer is NULL handle.
 *  @retval    WGLR_LOCK_ERROR     Locking the internal protection semaphore  failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore  failed.  
 *  @retval   WGLR_INTERNAL_ERROR   Some internal calls return error.  
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_start_timer(
                    HANDLE_T            h_timer,
                    UINT32              ui4_delay,
                    TIMER_FLAG_T        e_flag,
                    VOID*               pv_tag);

/*------------------------------------------------------------------*/
/*! @brief  stop a widget timer started before using c_wgl_start_timer()
 *  @param  [in]    h_timer             -Contains the timer handle to be stopped.
 *  @note
 *  @see c_wgl_create_timer(),c_wgl_start_timer()
 *  @return return the error code
 *  @retval   WGLR_OK             The routine was successful.
  *  @retval   WGLR_INV_HNADLE     The argument h_timer contains an invalid timer handle.
  *  @retval   WGLR_INV_ARG        The argument h_timer is NULL handle.
 *  @retval    WGLR_LOCK_ERROR     Locking the internal protection semaphore  failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore  failed.  
 *  @retval   WGLR_INTERNAL_ERROR   Some internal calls return error.  
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_stop_timer(HANDLE_T h_timer);


/*------------------------------------------------------------------*/
/*! @brief  This API unregisters the client represented by h_client and 
 *              frees this client handle. All widgets ever created in the main 
 *              thread context of h_client will be destroyed as well. If 
 *              h_client is the current active application, the next 
 *              application along the application list becomes active.
 *  @param  [in]    h_client    Contains the client (application) handle.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval     WGLR_INV_HNADLE     The argument h_client contains an invalid client handle.
 *  @retval     WGLR_INTERNAL_ERROR Freeing the client handle failed.
 */
/*------------------------------------------------------------------*/

extern INT32    x_wgl_unregister(
                    HANDLE_T            h_client);

/*------------------------------------------------------------------*/
/*! @brief  This API calculates the pixel size of the string (when displayed on screen)
 *  @param  [in]    h_fnt               - Contains a handle of font object.
 *  @param  [in]    w2s_str             - The string to be evaluated.
 *  @param  [in]    i4_length           - Maximum length to be evaluated. If <0, use whole w2s_str. (NULL-terminated)
 *  @param  [out]   pi4_width           - References the width of the string if the routine was successful. In all other cases, the value of this argument is undetermined.
 *  @param  [out]   pi4_height          - References the height of the string if the routine was successful. In all other cases, the value of this argument is undetermined.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     FER_OK              The routine was successful.
 *  @retval     FER_INV_ARG         Invalid arguments.
 *  @retval     FER_NOT_INIT        The Font Engine is not initialized yet.
 *  @retval     FER_INV_HANDLE      The argument h_fnt is an invalid font handle.
 *  @retval     FER_INV_ARG         One or more characters in the string is invalid
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_get_string_size(HFONT_T h_fnt,
            const UTF16_T * w2s_str,
            INT32 i4_length,
            INT32 * pi4_width,
            INT32 * pi4_height);

/*------------------------------------------------------------------*/
/*! @brief  This API retrieve on screen surface handle which is related to canvas 
 *               and this api is just for CLI command usage.
 *  @param  [in]    id    Contains the canvas id.
 *  @note
 *  @see
 *  @return return the surface handle
 *  @retval  
 */
/*------------------------------------------------------------------*/
extern HANDLE_T x_wgl_get_canvas_surface_handle(HANDLE_T h_canvas);

/*------------------------------------------------------------------*/
/*! @brief  This API re-create canvas surface resource associated with the specified OSD plane represented by h_plane.
 *               NOTE:  input canvas can not be reference canvas or shadow canvas and only can be single canvas.
 *  @param  [in]    h_obj               - Contains a canvas handle to be reconfig, can not be NULL.
 *  @param  [in]    h_plane             -Contains the handle of the specified OSD plane.
 *  @param  [in]    pt_cfg               -Contains the configuration of the new canvas surface info.Please refer to WGL_CANVAS_CFG_T for details.
 *  @note
 *  @see
 *  @return return the error code 
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval     WGLR_INV_HANDLE     The argument h_plane contains an invalid OSD plane handle.
 *  @retval     WGLR_INV_ARG        The argument h_canvas is NULL. Or pt_cfg is null or contains invalid configuration value.
 *  @retval     WGLR_INTERNAL_ERROR  Input Canvas is not single canvas and is reference or shadow canvas.
 *  @retval     WGLR_OUT_OF_MEMORY  There is not enough memory to complete the operation.
 *  @retval      WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval      WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/

extern INT32 x_wgl_canvas_reconfig(
    HANDLE_T                h_canvas,
    GL_HPLANE_T             h_plane,
    const WGL_CANVAS_CFG_T* pt_cfg);

extern INT32 x_wgl_get_canvas_config(HANDLE_T h_canvas,
    							   WGL_CANVAS_CFG_T* pt_cfg);


#ifdef WGL_ANIM_SUPPORT    

/*------------------------------------------------------------------*/
/*! @brief This API  initialize animation related resource . 
 *  @param  [in]    pt_config            - Contains the animation config structure pointer.
 *  @note
 *  @see
 *  @return return the error code 
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval      WGLR_INV_ARG    The argument pt_config is NULL or conains invalid value . 
 *  @retval     WGLR_OS_ERROR     Get thread, message queue or semphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_init_animation_resource(
                    WGL_ANIM_CONFIG_T *pt_config);

/*------------------------------------------------------------------*/
/*! @brief This API  begin an animation process. 
 *              Depends on the value of e_context, this API could execute in an 
 *              asynchronous or a synchronous way. If the asynchronous behavior 
 *              is selected, this API returns immediately. 
 *              Following WGL_MSG_ANIMATION_TICK message is sent from Animation Manager Thread.
 *              On the contrary, if the synchronous behavior is chosen, this routine 
 *              will not return until the completion of the requested animation process or until the occurrence of an error.
 *  @param  [in]    h_widget            - Contains the animation widget handle
 *  @param  [in]    pt_anim_data      - Pointer to a WGL_ANIMATION_DATA_T that contain animation information for the animation 
 *  @param  [in]    e_context           - Contains rendering context,please refer to WGL_ANIM_CONTEXT_T for details.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval     WGLR_INV_ARG    The argument h_widget is NULL or e_context is not valid value. 
 *  @retval     WGLR_INV_HANDLE     The argument h_widget contains an invalid widget handle.
 *  @retval     WGLR_SYS_RC_NOT_INIT  Animation Resource is not initialized.
 *  @retval     WGLR_OUT_OF_MEMORY  There is not enough memory to complete the operation.
 *  @retval     WGLR_INV_CONTEXT  There is no valid application related to calling thread context.
 *  @retval     WGLR_OS_ERROR     Get thread infomation failed.
 *  @retval     WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval     WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval     WGLR_INTERNAL_ERROR  There is internal error occured.
 *  @retval     WGLR_ANIM_TYPE_CONFLICT     pt_anim_data has animation type conflicts with its ancestor's animation.
 *  @retval     WGLR_ANIM_TYPE_NOT_SUPPORT     pt_anim_data has unsupported animation type.
 *  @retval     WGLR_ANIM_ALREADY_RUNNING   The widget has already started animaiton process.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_start_animation(
                    HANDLE_T                h_widget,
                    WGL_ANIMATION_DATA_T*   pt_anim_data,
                    WGL_ANIM_CONTEXT_T      e_context);

/*------------------------------------------------------------------*/
/*! @brief This API  end an animation process . 
 *  @param  [in]    h_widget            - Contains the animation widget handle
 *  @param  [in]    pv_param1           - Reserved, must be NULL.
 *  @param  [in]    pv_param2           - Reserved, must be NULL.
 *  @note
 *  @see
 *  @return return the error code 
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval      WGLR_INV_ARG    The argument h_widget is NULL . 
 *  @retval      WGLR_INV_HANDLE     The argument h_widget contains an invalid widget handle.
 *  @retval       WGLR_SYS_RC_NOT_INIT  Animation Resource is not initialized.
 *  @retval       WGLR_INV_CONTEXT  There is no valid application related to calling thread context.
 *  @retval     WGLR_OS_ERROR     Get thread infomation failed.
 *  @retval     WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval      WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_wgl_end_animation(
                    HANDLE_T            h_widget,
                    VOID                *pv_param1,
                    VOID                *pv_param2);


/*------------------------------------------------------------------*/
/*! @brief This API  register animation callback function to the widget and widget will notify animation process and status to user by calling pf_anim_nfy_fct. 
 *  @param  [in]    h_widget            - Contains the animation widget handle
 *  @param  [in]    pf_anim_nfy_fct           - callback function pointer, can be null.
 *  @param  [in]    pv_private_tag           - private data set by user and this data will return to user through pf_anim_nfy_fct calling.
 *  @note
 *  @see
 *  @return return the error code 
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval      WGLR_INV_ARG    The argument h_widget is NULL. 
 *  @retval      WGLR_INV_HANDLE     The argument h_widget contains an invalid widget handle.
 *  @retval       WGLR_SYS_RC_NOT_INIT  Animation Resource is not initialized.
 *  @retval     WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval      WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_set_anim_callback(
                    HANDLE_T h_widget,
                    wgl_app_anim_callback_fct pf_anim_nfy_fct,
                    void* pv_private_tag);

/*------------------------------------------------------------------*/
/*! @brief This API  retrieve animation callback function to the widget which is set before using x_wgl_set_anim_callback(). 
 *  @param  [in]    h_widget            - Contains the animation widget handle
 *  @param  [out]    pf_anim_nfy_fct           - References to callback function , can not be null.
 *  @note
 *  @see x_wgl_set_anim_callback
 *  @return return the error code 
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval      WGLR_INV_ARG    The argument h_widget or pt_anim_callback is NULL. 
 *  @retval      WGLR_INV_HANDLE     The argument h_widget contains an invalid widget handle.
 *  @retval       WGLR_SYS_RC_NOT_INIT  Animation Resource is not initialized.
 *  @retval     WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval      WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/

extern INT32 x_wgl_get_anim_callback(
                    HANDLE_T h_widget,
                    WGL_ANIM_CB_T* pt_anim_callback);


/*------------------------------------------------------------------*/
/*! @brief This API set animation image sequence information for the widget
 *  @param  [in]    h_widget            - Contains the animation widget handle
 *  @param  [in]    ui1_img_set_index - The image sequence index to set which will be based on max image sequence number set by c_wgl_set_animation_max_img_set().
                                                          Valid value is among 0 to max image sequence number-1.
 *  @param  [in]    pt_anim_img_seq           - Point ot WGL_ANIM_IMG_SEQ_T,it contain the image sequence information.
 *  @note
 *  @see
 *  @return return the error code 
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval      WGLR_INV_ARG    The argument h_widget is NULL or ui1_img_set_index is not valid or pt_anim_img_seq is NULL. 
 *  @retval      WGLR_INV_HANDLE     The argument h_widget contains an invalid widget handle.
 *  @retval       WGLR_SYS_RC_NOT_INIT  Animation Resource is not initialized.
 *  @retval     WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval      WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_set_animation_img_set(
                	HANDLE_T                h_widget,
                    UINT8                   ui1_img_set_index,     
                	const WGL_ANIM_IMG_SEQ_T*       pt_anim_img_seq);

/*------------------------------------------------------------------*/
/*! @brief This API  set maximum animation image sequence number to widget. It must be set before setting image 
sequence information by c_wgl_set_animation_img_set() and the number can not change after it set.
 *  @param  [in]    h_widget            - Contains the animation widget handle
 *  @param  [in]    ui2_max_img_set  - Contains maximum animation image sequence number.
 *  @note
 *  @see
 *  @return return the error code 
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval      WGLR_INV_ARG    The argument h_widget is NULL. 
 *  @retval      WGLR_INV_HANDLE     The argument h_widget contains an invalid widget handle.
 *  @retval       WGLR_SYS_RC_NOT_INIT  Animation Resource is not initialized.
 *  @retval     WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval      WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_set_animation_max_img_set(
                    HANDLE_T h_widget,
                    UINT16 ui2_max_img_set);

/*------------------------------------------------------------------*/
/*! @brief This API set action-animation map table to widget, after the table set, when one action event occured on the widget then the 
         corresponding animtion will start automatically, at present only supports WGL_ANIM_ACTION_FOCUS,WGL_ANIM_ACTION_UNFOCUS action type . 
 *  @param  [in]    h_widget            - Contains the animation widget handle
 *  @param  [in]    ui1_map_count           - Action animation map table count which can not be larger than WGL_ANIM_MAX_MAP_CNT value.
 *  @param  [in]    pt_anim_map           - Point to WGL_ANIM_ACTION_MAP_T array, it contains the action animation map table infomation and if it is NULL means clear map data set before.
 *  @note
 *  @see
 *  @return return the error code 
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval      WGLR_INV_ARG    The argument h_widget is NULL or pt_anim_img_seq is not NULL and ui1_map_count is larger than WGL_ANIM_MAX_MAP_CNT value. 
 *  @retval      WGLR_INV_HANDLE     The argument h_widget contains an invalid widget handle.
 *  @retval       WGLR_SYS_RC_NOT_INIT  Animation Resource is not initialized.
 *  @retval     WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval      WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_set_animation_map(
                	HANDLE_T                h_widget,
                    UINT8                   ui1_map_count,     
                	const WGL_ANIM_ACTION_MAP_T*       pt_anim_map);

/*------------------------------------------------------------------*/
/*! @brief  get the tick interval of the animation tick
 *  @note
 *  @see
 *  @return return the animation tick interval
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern UINT32   x_wgl_get_tick_interval(VOID);

/*------------------------------------------------------------------*/
/*! @brief  get the animation tick mode
 *  @note
 *  @see
 *  @return return the animation tick mode
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern WGL_TICK_MODE_T x_wgl_get_tick_mode(VOID);

/*------------------------------------------------------------------*/
/*! @brief  set the animation tick interval value
 *  @param  [in]    ui4_data            - Contains the animation tick interval value
 *  @note
 *  @see
 *  @return None
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern VOID     x_wgl_set_tick_interval(UINT32 ui4_data);
/*------------------------------------------------------------------*/
/*! @brief  get the animation status
 *  @param  [in]    h_widget            - animation widget handle
 *  @param  [out]   pui2_anim_type      - animation type,maybe is  WGL_ANIM_TYPE_MOVE ,WGL_ANIM_TYPE_SCALE,WGL_ANIM_TYPE_ALPHA or WGL_ANIM_TYPE_IMG or composition of multiple types.
 *  @note  
 *  @see 
 *  @return return the error code   
 *  @retval     WGLR_OK             The routine was successful.
 *  @retval     WGLR_INV_HANDLE     The argument h_widget contains an invalid widget handle.
 *  @retval     WGLR_INV_ARG        The argument h_widget or pui2_anim_type is NULL.
 *  @retval      WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval      WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.

 */
/*------------------------------------------------------------------*/

extern INT32    x_wgl_get_anim_status(
                    HANDLE_T            h_widget,
                    UINT16              *pui2_anim_type);
#endif

#ifdef WGL_COORDINATE_INVERSE
/*------------------------------------------------------------------*/
/*! @brief  This API sets all UI widgets's x coordinate is inverse or not, for example, if one widget is left-top alignment then 
 *  the widget will be right-top alignment after this api is called with parameter b_coordinate_reverse=TRUR
 *  @param  [in]    b_coordinate_reverse - Contains a flag indicating if inverse x coordinate for all ui widgets.
 *  @param  [in]    ui1_repaint         - Contains a flag indicating if synchronous/asynchronous 
 *                      repainting should be performed automatically .
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK  Success.
 *  @retval     WGLR_INV_HNADLE The argument h_widget contains an invalid widget handle.
 *  @retval      WGLR_CANNOT_BE_FOCUSED  The specified widget h_widget cannot be focused. A widget can only be focused if all its parent and ancestors are enabled.
 *  @retval      WGLR_LOCK_ERROR     Locking the internal protection semaphore failed.
 *  @retval      WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/

extern INT32 x_wgl_set_coordinate_reverse(BOOL b_coordinate_reverse,UINT8 ui1_repaint);
#endif

#ifdef __cplusplus
}
#endif

#endif
/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_WGL*/
/*----------------------------------------------------------------------------*/

