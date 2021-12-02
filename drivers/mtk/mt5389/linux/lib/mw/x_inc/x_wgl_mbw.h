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
 * Description:
 *         This header file contains public interfaces, constant and type
 *         definitions of the Widget Library.
 *---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_WGL_MATRIXBOX
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _X_WGL_MBW_H_
#define _X_WGL_MBW_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_wgl.h"
#include "u_wgl_mbw.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* matrix box specific commands */



/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_init(VOID);

/*------------------------------------------------------------------*/
/*! @brief This API creates a new MatrixBox with the initialization data.
*  @param [in]  h_container Contains either the handle of the parent widget or the handle of the canvas on which this widget should be displayed. If a canvas handle is present, the created MatrixBox will be top-level.
*  @param [in]   pt_init_info     References a WGL_MBW_INIT_T  structure which defines the data used for MatrixBox creation and initialization.
*  @param [in]   pt_rect References a GL_RECT_T structure which defines the display area of the widget to be created.
*  @param [in]   pf_wdgt_proc    Contains the function pointer of the application defined message procedure for the new MatrixBox. A NULL value indicates the default widget message procedure will be used.
*  @param [in]   pv_tag         Contains an application-defined tag value which is attached to the returned widget handle. The application can retrieve this value with the handle API x_handle_get_tag().
*  @param [out]   ph_mbw                                 References the handle of the created MatrixBox if this routine was successful. In all other cases, this field is undefined.
*  @note
*  @see
*  @return  return error code
*  @retval  WGLR_OK The routine was successful.
*  @retval   WGLR_INV_ARG    The argument pt_rect is NULL or references an empty rectangle. Or ph_mbwis NULL. Or pt_init_info contains some invalid data.
*  @retval   WGLR_INV_HANDLE The argument h_container contains an invalid widget handle except NULL_HANDLE.
*  @retval   WGLR_OUT_OF_MEMORY  There is not enough memory to complete the operation.
*  @retval   WGLR_OS_ERROR   This API is not called from a thread context.
*  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
*  @retval  WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_create (
    HANDLE_T                h_container,
    const WGL_MBW_INIT_T*   pt_init_info,
    const GL_RECT_T*        pt_rect,
    wgl_widget_proc_fct     pf_wdgt_proc,
    VOID*                   pv_tag,
    HANDLE_T*               ph_mbw);

/*------------------------------------------------------------------*/
/*! @brief This API destroys the MatrixBox represent by h_mbw. The MatrixBox and all its children are removed from the widget hierarchy immediately. An additional repaint might be induced if necessary.
 *  @param [in]  h_mbw	                                  Contains the handle of the MatrixBox to be destroyed.
 *  @param [out]  NULL
 *  @note
 *  @see
 *  @return  return error code
 *  @retval  WGLR_OK The routine was successful.
 *  @retval  WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle.
 *  @retval  WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval  WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval  WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE  The function is not permitted in TRANSITION state.
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_destroy (
    HANDLE_T                h_mbw);

/*------------------------------------------------------------------*/
/*! @brief This API calls the default message procedure of of mbw. For instance,The arguments passed to this API are identical to those passed to the message procedure.
 *  @param [in]   h_mbw                                     Contains the handle of the MatrixBox.
 *  @param [in]    ui4_msg Contains  the message id.
 *  @param [in]    pv_param1   Specifies the additional information. The actual content of this argument depends on ui4_msg.
 *  @param [in]    pv_param2   Specifies more additional information. The actual content of this argument depends on ui4_msg.
 *  @param [out] NULL
 *  @note
 *  @see
 *  @return  return error code
 *  @retval  WGLR_OK The routine was successful.
 *  @retval  WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle.
 *  @retval  WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval  WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_default_proc (
    HANDLE_T                h_mbw,
    UINT32                  ui4_msg,
    VOID*                   pv_param1,
    VOID*                   pv_param2);

/*------------------------------------------------------------------*/
/*! @brief This API executes the default key handler procedure of the h_mbw.
 *  @param [in]  h_mbw                                  Contains the handle of the MatrixBox.
 *  @param [in]   e_key_hdlr_type                  Contains the type of the key handler.
 *  @param [in]   ui4_data                               should be set as the ui4_data argument from the callback (*wgl_mbw_key_hdlr_fct) (). 
 *  @param [out] NULL
 *  @note
 *  @see
 *  @return  return error code
 *  @retval  WGLR_OK The routine was successful.
 *  @retval  WGLR_INV_ARG    The argument e_key_hdlr_type is invalid. Or the ui4_data is invalid.
 *  @retval  WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval  WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval  WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_default_key_hdlr (
    HANDLE_T                        h_mbw,
    WGL_MBW_KEY_HDLR_TYPE_T         e_key_hdlr_type,
    UINT32                          ui4_data);

/*------------------------------------------------------------------*/
/*! @brief This API executes the default key handler procedure of the h_mbw.
 *  @param [in]   h_mbw                                  Contains the handle of the MatrixBox.
 *  @param [in]  e_evn_hdlr_type                  Contains the type of the event handler.
 *  @param [in]  ui4_data                               should be set as the ui4_data argument from the callback (*wgl_mbw_event_hdlr_fct) (). 
 *  @param [out] NULL
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument e_key_hdlr_type is invalid. Or the ui4_data is invalid.
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_default_evn_hdlr (
    HANDLE_T                        h_mbw,
    WGL_MBW_EVN_HDLR_TYPE_T         e_evn_hdlr_type,
    UINT32                          ui4_data);

/*------------------------------------------------------------------*/
/*! @brief This API registers the key handler information with the Matrix. 
 *  @param [in]   h_mbw                                  Contains the handle of the MatrixBox to be registered.
 *  @param [in]    pt_key_hdlr_info                  References a WGL_MBW_KEY_HDLR_INFO_T structure which defines the special key handler callback function.
 *  @param [out] NULL
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument pt_key_hdlr_info contains some invalid data.
 *  @retval    WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval   WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE  The function is not permitted in TRANSITION state.
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_reg_key_hdlr (
    HANDLE_T                        h_mbw,
    const WGL_MBW_KEY_HDLR_INFO_T*  pt_key_hdlr_info);

/*------------------------------------------------------------------*/
/*! @brief  This API is called to enter TRANSITION state. In TRANSITION state, MatrixBox can't receive WGL_MSG_KEY_* message and some type commands.
 *  @param [in] h_mbw	                               Contains the handle of the MatrixBox.
 *  @param [out] NULL   
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval   WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE  The function is not permitted in TRANSITION state.
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_begin_transition (
    HANDLE_T                h_mbw);

/*------------------------------------------------------------------*/
/*! @brief  MatrixBox exit transition state
 *  @param [in] h_mbw	                               Contains the handle of the MatrixBox
 *  @param [out]  NULL
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_end_transition (
    HANDLE_T                h_mbw);

/*------------------------------------------------------------------*/
/*! @brief  This API retrieves the total item number in the data model.
 *  @param [in]  h_mbw	                             Contains the handle of the MatrixBox.   
 *  @param [out]  pui4_num_total                  Contains the total item number in the data model.
 *  @note
 *  @see
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument pui4_num_total is NULL. 
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_get_total_num (
    HANDLE_T                h_mbw,
    UINT32*                 pui4_num_total);

/*------------------------------------------------------------------*/
/*! @brief  This API retrieves the navigation information with the specified navigation direction and index of the specified item.
 *  @param [in]   h_mbw                           Contains the handle of the MatrixBox.
 *  @param [in]   ui4_idx_from                 Contains the index of the item to start the navigation. The index is based on the total item number.
 *  @param [in]   e_nvgt_dir                     Contains the direction of the navigation.
 *  @param [out]  pt_nvgt_info                  References a WGL_MBW_NVGT_INFO_T structure which indicates whether the item in such navigation direction is navigable and some other information like wrap_over, vp_changed.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument ui4_idx_from is invalid. Or the e_nvgt_dir is invalid. Or the pt_nvgt_info is NULL.
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_get_nvgt_info (
    HANDLE_T                h_mbw,
    UINT32                  ui4_idx_from,
    WGL_MBW_NVGT_DIR_T      e_nvgt_dir,
    WGL_MBW_NVGT_INFO_T*    pt_nvgt_info);

/* should take care WGL_MBW_ITEM_STATE_HLTED */
/*--------------------------- ---------------------------------------*/
/*! @brief   This API enables the item with special state. An app should not change an item state in Data Model by other way except for calling c_wgl_mbw_reload().
 *                Enable an item to be highlighted state will cause previous highlighted item lose its highlighted state.System will also ensure the highlighted item to be displayed in view port, so if user want to enable an item not in view port to be highlighted state, system will update view port automatically to let the new highlighted item displayed in view port. 
 *  @param [in]   h_mbw                                    Contains the handle of the MatrixBox.
 *  @param [in]   ui4_idx                                   Contains the index of the item to be enabled with special state. The index is based on the total item number.
 *  @param [in]   ui4_state_mask                     Contains the state mask to be enabled
 *  @param [out]  NULL
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval    WGLR_INV_ARG    The argument ui4_idx is invalid. Or the ui4_state_mask is invalid. 
 *  @retval    WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval    WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval    WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_enable_item_state (
    HANDLE_T    h_mbw,
    UINT32      ui4_idx,
    UINT32      ui4_state_mask);

/*------------------------------------------------------------------*/
/*! @brief  This API disables the item with special state. An app should not change an item state in Data Model by other way except for calling c_wgl_mbw_reload().
 *  @param [in]   h_mbw                                    Contains the handle of the MatrixBox.
 *  @param [in]   ui4_idx                                   Contains the index of the item to be disabled with special state. The index is based on the total item number.
 *  @param [in]   ui4_state_mask                     Contains the state mask to be disabled. 
 *  @param [out]  NULL
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument ui4_idx is invalid. Or the ui4_state_mask is invalid. 
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_disable_item_state (
    HANDLE_T    h_mbw,
    UINT32      ui4_idx,
    UINT32      ui4_state_mask);

/*------------------------------------------------------------------*/
/*! @brief  This API retrieves the item's state.
 *  @param [in]   h_mbw                                    Contains the handle of the MatrixBox.
 *  @param [in]   ui4_idx                                   Contains the index of the item to be retrieved. The index is based on the total item number.
 *  @param [out]   pui4_state                              Contains the retrieved state of the item.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval  WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument ui4_idx is invalid. Or the pui4_state is NULL. 
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_get_item_state (
    HANDLE_T    h_mbw,
    UINT32      ui4_idx,
    UINT32*     pui4_state);

/* return WGLR_MBW_NO_HIGHLIGHT_ITEM if no highlight item */
/*------------------------------------------------------------------*/
/*! @brief  This API retrieves the index of current highlight item. If there is no highlight item, return WGLR_MBW_NO_HIGHLIGHT_ITEM
 *  @param [in]  h_mbw	                                 Contains the handle of the MatrixBox.
 *  @param [out]  pui4_idx _hlt                          Contains the index of the current highlight item. The index is based on the total item number.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval    WGLR_OK The routine was successful.
 *  @retval    WGLR_INV_ARG    The argument pui4_idx_hlt is NULL. 
 *  @retval    WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval    WGLR_MBW_NO_HIGHLIGHT_ITEM There is no highlight item,
 *  @retval    WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval    WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval      - 
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_get_hlt_item (
    HANDLE_T    h_mbw,
    UINT32*     pui4_idx_hlt);

/*------------------------------------------------------------------*/
/*! @brief  This API sets the key map of the MatrixBox.
 *  @param [in]   h_mbw                                    Contains the handle of the MatrixBox.
 *  @param [in]   pt_key_map                          References a WGL_MBW_KEY_MAP_T structure which defines the customized key map.
 *  @param [out] NULL
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument pt_key_map is NULL. 
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval   WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE  The function is not permitted in TRANSITION state.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_set_key_map (
    HANDLE_T                    h_mbw,
    const WGL_MBW_KEY_MAP_T*    pt_key_map);

/*------------------------------------------------------------------*/
/*! @brief  This API retrieves the visible items' index range in the current viewport.
 *  @param [in]  h_mbw	                                Contains the handle of the MatrixBox.
 *  @param [out]   pui4_first                               Contains the index of the first item in the current viewport. The index is based on the total item number.
 *  @param [out]   pui4_first                               Contains the index of the last item in the current viewport. The index is based on the total item number.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument pui4_first or pui4_last is NULL. 
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_get_visible_range_in_vp (
    HANDLE_T    h_mbw,
    UINT32*     pui4_first,
    UINT32*     pui4_last);

/*------------------------------------------------------------------*/
/*! @brief  This API retrieves the item's display region.
 *  @param [in]   h_mbw                                   Contains the handle of the MatrixBox.
 *  @param [in]   ui4_item_ctnr_idx                  Contains the index of the item containerto be retrieved. The index is based on the total item container number.
 *  @param [out]  pt_rect                                   References a GL_RECT_T structure which defines the display area of the item. Its coordinate is based on the Matrix Box.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument ui4_idx is invalid or the pt_rect is NULL. 
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_get_item_ctnr_rect (
    HANDLE_T    h_mbw,
    UINT32      ui4_item_ctnr_idx,
    GL_RECT_T*  pt_rect);

/*------------------------------------------------------------------*/
/*! @brief This API sets image to the special target of the MatrixBox.
 *  @param [in]   h_mbw                                   Contains the handle of the MatrixBox.
 *  @param [in]    ui1_target                             Indicates the target to be set image. Now only WGL_IMG_MBW_VP_BK can be used.
 *  @param [in]    pt_img_info                           Contains the image information.
 *  @param [out] NULL
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument ui1_target is invalid or the pt_img_info is NULL. 
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_set_image (
    HANDLE_T                h_mbw,
    UINT8                   ui1_target,
    const WGL_IMG_INFO_T*   pt_img_info );

/*------------------------------------------------------------------*/
/*! @brief  This API sets color  to the special target of the MatrixBox.
 *  @param [in]   h_mbw                                   Contains the handle of the MatrixBox.
 *  @param [in]   ui1_target                             Indicates the target to be set image. Now only WGL_CLR_MBW_VP_BK can be used.
 *  @param [in]   pt_clr_info                           Contains the color information.
 *  @param [out] NULL
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument ui1_target is invalid or the pt_clr_info is NULL. 
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_set_color (
    HANDLE_T                h_mbw,
    UINT8                   ui1_target,
    const WGL_COLOR_INFO_T* pt_clr_info);

/*------------------------------------------------------------------*/
/*! @brief  This API get image for the special target of the MatrixBox.
 *  @param [in]   h_mbw                                   Contains the handle of the MatrixBox.
 *  @param [in]   ui1_target                             Indicates the target to be set image. Now only WGL_IMG_MBW_VP_BK can be used.
 *  @param [out]  pt_img_info                           Contains the image information.
 *  @note 
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument ui1_target is invalid. 
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_get_image (
    HANDLE_T                h_mbw,
    UINT8                   ui1_target,
    WGL_IMG_INFO_T*         pt_img_info);

/*------------------------------------------------------------------*/
/*! @brief  This API gets color for the special target of the MatrixBox.
 *  @param [in]   h_mbw                                   Contains the handle of the MatrixBox.
 *  @param [in]   ui1_target                             Indicates the target to be set image. Now only WGL_CLR_MBW_VP_BK can be used.
 *  @param [out]  pt_clr_info                           Contains the color information.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument ui1_target is invalid. 
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_get_color (
    HANDLE_T                h_mbw,
    UINT8                   ui1_target,
    WGL_COLOR_INFO_T*       pt_clr_info);

/*------------------------------------------------------------------*/
/*! @brief This API registers the event handler information with the Matrix. 
 *  @param [in]   h_mbw                                  Contains the handle of the MatrixBox to be registered.
 *  @param [in]   pt_evn_hdlr_info                  References a WGL_MBW_EVN_HDLR_INFO_T structure which defines the special event handler callback function.
 *  @param [out]  NULL
 *  @note
 *  @see
 *  @return  return error code
 *  @retval    WGLR_OK The routine was successful.
 *  @retval    WGLR_INV_ARG    The argument pt_evn_hdlr_info contains some invalid data.
 *  @retval    WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval    WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval    WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval    WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE  The function is not permitted in TRANSITION state.
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_reg_event_hdlr(
    HANDLE_T                       h_mbw,
    const WGL_MBW_EVN_HDLR_INFO_T* pt_evn_hdlr_info);

/*------------------------------------------------------------------*/
/*! @brief  This API sets the highlight information of the MatrixBox.
 *  @param [in]   h_mbw                                   Contains the handle of the MatrixBox.
 *  @param [in]   pt_hlt_info                             References a WGL_MBW_HLT_ITEM_INFO_T structure which defines data used for highlight an item.
 *  @param [out] NULL
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument pt_hlt_info is NULL. 
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval   WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE  The function is not permitted in TRANSITION state.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_set_hlt_item_info (
    HANDLE_T                        h_mbw,
    const WGL_MBW_HLT_ITEM_INFO_T*  pt_hlt_info);

/*------------------------------------------------------------------*/
/*! @brief  This API retrieves the highlight information of the MatrixBox.
 *  @param [in]  h_mbw	                                Contains the handle of the MatrixBox.
 *  @param [out] pt_hlt_info                             References a WGL_MBW_HLT_ITEM_INFO_T structure which defines data used for highlight an item.
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument pt_hlt_info is NULL. 
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_get_hlt_item_info (
    HANDLE_T                    h_mbw,
    WGL_MBW_HLT_ITEM_INFO_T*    pt_hlt_info);
    
extern INT32 x_wgl_mbw_get_hlt_item_ctnr_info (
    HANDLE_T                    h_mbw,
    WGL_MBW_ITEM_CTNR_INFO_T*   pt_hlt_item_ctnr_info);
/*------------------------------------------------------------------*/
/*! @brief  This API moves the special item container to a new position.
 *  @param [in]   h_mbw                                   Contains the handle of the MatrixBox.
 *  @param [in]   ui4_item_cntr                        Contains the index of the item container to be moved.
 *  @param [in]   i4_offset_x                            Indicates the moving offset in x-axis.
 *  @param [in]   i4_offset_y                            Indicates the moving offset in y-axis.
 *  @param [out] NULL
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument ui4_item_ctnr or i4_offset_x or i4_offset_y is invalid. 
 *  @retval    WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval    WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval    WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_move_item_ctnr (
    HANDLE_T    h_mbw,
    UINT32      ui4_item_ctnr_idx,
    INT32       i4_offset_x,
    INT32       i4_offset_y);

/*------------------------------------------------------------------*/
/*! @brief  This API updates the viewport using the navigation information.
 *  @param [in]   h_mbw                           Contains the handle of the MatrixBox.
 *  @param [in]   pt_nvgt_info                  References a WGL_MBW_NVGT_INFO_T structure which indicates whether the item in such navigation direction is navigable and some other information like wrap_over, vp_changed.
 *  @param [out]  NULL 
 *  @note
 *  @see
 *  @return  return error code
 *  @retval  WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument pt_nvgt_info  is NULL.
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval    WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_update_vp (
    HANDLE_T                h_mbw,
    WGL_MBW_NVGT_INFO_T*    pt_nvgt_info);

/*------------------------------------------------------------------*/
/*! @brief   This API repaints the viewport of the MatrixBox.
 *  @param [in]  h_mbw	                        Contains the handle of the MatrixBox.
 *  @param [out]  NULL
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_repaint_vp (
    HANDLE_T    h_mbw);

/*------------------------------------------------------------------*/
/*! @brief This API repaints the item specified by the item index.
 *  @param [in]   h_mbw                           Contains the handle of the MatrixBox.
 *  @param [in]    ui4_idx                          Contains the index of the item to be repainted. The index is based on the total item number.
 *  @param [out]  NULL
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument ui4_idx is invalid. 
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval    WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval    WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_repaint_item (
    HANDLE_T    h_mbw,
    UINT32      ui4_idx);

/*------------------------------------------------------------------*/
/*! @brief  Used to lock Data Model database, because MatrixBox data is close with Data Model data, so data in MatrixBox and Data Model need to use the same lock to protect.
 *  @param [in]  h_mbw	                               Contains the handle of the MatrixBox.
 *  @param [out]  NULL
 *  @note
 *  @see
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_dm_lock(
    HANDLE_T    h_mbw);

/*------------------------------------------------------------------*/
/*! @brief  This API is used to unlock Data Model database.
 *  @param [in]  h_mbw	                               Contains the handle of the MatrixBox.
 *  @param [out] NULL
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_dm_unlock(
    HANDLE_T    h_mbw);

/*------------------------------------------------------------------*/
/*! @brief  This API resets MatrixBox when the current data model is replaced by a new data model.
 *  @param [in]   h_mbw                                  Contains the handle of the MatrixBox.
 *  @param [in]   ui4_num_total                      Indicates the total item number of the new data model.
 *  @param [out]  NULL
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument ui4_num_total > WGL_MBW_MAX_ITEM_NUM 
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval   WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE  The function is not permitted in TRANSITION state.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_reload (
    HANDLE_T    h_mbw,
    UINT32      ui4_num_total);


/*------------------------------------------------------------------*/
/*! @brief  This API resets MatrixBox when the current data model is replaced by a new data model. And will set highlight item according to input item index.
 *  @param [in]   h_mbw                                  Contains the handle of the MatrixBox.
 *  @param [in]   ui4_num_total                      Indicates the total item number of the new data model.
 *  @param [in]   ui4_hlt_item_idx                   Indicates the item index to be highlighted in the data model and this value can be WGL_MBW_NULL_INDEX.
 *  @param [out]  NULL
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument ui4_num_total > WGL_MBW_MAX_ITEM_NUM or ui4_hlt_item_idx >= ui4_num_total && ui4_hlt_item_idx != WGL_MBW_NULL_INDEX
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval   WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE  The function is not permitted in TRANSITION state.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_reload_with_hlt (
    HANDLE_T    h_mbw,
    UINT32      ui4_num_total,
    UINT32      ui4_hlt_item_idx);


/*------------------------------------------------------------------*/
/*! @brief This API reloads the item static data specified by the item index.
 *  @param [in]   h_mbw                           Contains the handle of the MatrixBox.
 *  @param [in]   ui4_idx                          Contains the index of the item to be reloaded. The index is based on the total item number.
 *  @param [out] NULL
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument ui4_idx is invalid. 
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval   WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE  The function is not permitted in TRANSITION state.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_reload_item_static (
    HANDLE_T    h_mbw,
    UINT32      ui4_idx);

/*------------------------------------------------------------------*/
/*! @brief  Whenever an item data in Data Model changed, this API must be called to synchronize with Data Model.
 *  @param [in]   h_mbw                           Contains the handle of the MatrixBox.
 *  @param [in]   ui4_idx                          Contains the index of the item to be reloaded. The index is based on the total item number.
 *  @param [out] NULL
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_ARG    The argument ui4_idx is invalid. 
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval   WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE  The function is not permitted in TRANSITION state.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_reload_item_data (
    HANDLE_T    h_mbw,
    UINT32      ui4_idx);

/*------------------------------------------------------------------*/
/*! @brief  This API updates MatrixBox when item static data in current data model has changed.
 *  @param [in]  h_mbw	                               Contains the handle of the MatrixBox.
 *  @param [out] NULL
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval   WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE  The function is not permitted in TRANSITION state.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_reload_static (    
    HANDLE_T    h_mbw);

/*------------------------------------------------------------------*/
/*! @brief  Whenever item data in Data Model changed, this API must be called to synchronize with Data Model.
 *  @param [in]  h_mbw	                               Contains the handle of the MatrixBox.
 *  @param [out] NULL
 *  @note
 *  @see
 *  @return  return error code
 *  @retval   WGLR_OK The routine was successful.
 *  @retval   WGLR_INV_HANDLE The argument h_mbw contains an invalid MatrixBox handle except NULL_HANDLE.
 *  @retval   WGLR_LOCK_ERROR Locking the internal protection semaphore failed.
 *  @retval   WGLR_UNLOCK_ERROR   Releasing the internal protection semaphore failed.
 *  @retval   WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE  The function is not permitted in TRANSITION state.
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_mbw_reload_data (
    HANDLE_T    h_mbw);



#ifdef __cplusplus
}
#endif

#endif
/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_WGL_MATRIXBOX*/
/*----------------------------------------------------------------------------*/


