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
/*----------------------------------------------------------------------------*/
/*! @file u_wgl_mbw.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains public interfaces, constant and type
 *         definitions of the Widget Library.
 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_WGL_MATRIXBOX Matrixbox Widget
 *
 *  @ingroup groupMW_WGL
 *  @brief Provides definitions,commands of the Matrixbox widget.The Matrix Box widget presents the user with 2-D list box. It is a widget wrapper. Each item's content is customized by user, for example an item can contain one text widget and one icon widget. 
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW_WGL
 *  @{ 
 */
/*----------------------------------------------------------------------------*/ 

#ifndef _U_WGL_MBW_H_
#define _U_WGL_MBW_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_handle.h"
#include "u_gl.h"
#include "u_wgl_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* MBW specific return types */
#define WGLR_MBW_NOT_PERMIT_IN_TRANS_STATE           (WGLR_WIDGET_SPF_START - ((INT32) 0))    /**<    Indicates the MatrixBox is in transition state.    */
#define WGLR_MBW_NO_HIGHLIGHT_ITEM                   (WGLR_WIDGET_SPF_START - ((INT32) 1))    /**<  	Indicates the MatrixBox has not highlighted item.      */
#define WGLR_MBW_INVALID_VP_RANGE                    (WGLR_WIDGET_SPF_START - ((INT32) 2))    /**<  Indicates the viewport range is invalid      */
#define WGLR_MBW_DM_INCONSISTENT_ITEM_STATE          (WGLR_WIDGET_SPF_START - ((INT32) 3))    /**<  Indicates the item state in Data Model is not valid.      */
#define WGLR_MBW_DM_INCONSISTENT_ITEM_DATA           (WGLR_WIDGET_SPF_START - ((INT32) 4))    /**<   Indicates the item data  in Data Model is not valid     */
#define WGLR_MBW_DM_NOT_SUPPORT_EX_SET_TYPE          (WGLR_WIDGET_SPF_START - ((INT32) 5))    /**< 	Indicates the MatrixBox dose not support Data Model (*wgl_mbw_dm_ex_set_fct)() callback       */
#define WGLR_MBW_DM_NOT_SUPPORT_EX_GET_TYPE          (WGLR_WIDGET_SPF_START - ((INT32) 6))    /**<  Indicates the MatrixBox dose not support Data Model (*wgl_mbw_dm_ex_get_fct)() callback.      */

/* item index constraints */
#define WGL_MBW_MAX_ITEM_NUM                         ((UINT32) 0x3FFFFFF0)    /**< Indicates the maximal number of items a Matrix Box may contain.        */
#define WGL_MBW_NULL_INDEX                           (WGL_MBW_MAX_ITEM_NUM + ((UINT32) 0))    /**<  current highlight item index in Matrix Box is NULL      */

/* MBW container style */
#define WGL_MBW_CW_STL_NO_IMG_UI                     MAKE_BIT_MASK_32 (0)    /**<  Indicates the container has no background image.      */
#define WGL_MBW_CW_STL_NO_BK                         MAKE_BIT_MASK_32 (1)    /**<	Indicates the container has no background image and color.        */
#define WGL_MBW_CW_STL_USE_SYS_IMG                   MAKE_BIT_MASK_32 (2)    /**<  Indicates the container using system background image.      */


typedef UINT32 WGL_MBW_ITEM_STATE_T;    /**<  item state       */
#define WGL_MBW_ITEM_STATE_DISABLED                  (((UINT32) 0x01) << 0)    /**<  Indicates the item is disabled.     */
#define WGL_MBW_ITEM_STATE_GRAYED                    (((UINT32) 0x01) << 1)    /**<   	Indicates the item is grayed.     */
#define WGL_MBW_ITEM_STATE_SELECTED                  (((UINT32) 0x01) << 2)    /**<Indicates the item is selected.        */
#define WGL_MBW_ITEM_STATE_HLTED                     (((UINT32) 0x01) << 3)    /**<   Indicates the item is highlighted.     */

#define WGL_MBW_ITEM_STATE_CUSTOM_START              (((UINT32) 0x01) << 4)    /**< Indicates the customized item state.       */

#define WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATIC         (((UINT32) 0x01) << 0)    /**<   Indicates the item's static data is required.      */
#define WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATE          (((UINT32) 0x01) << 1)    /**<   Indicates the data is required when the item's state has changed     */

#define WGL_MBW_ITEM_CTNR_UPDATE_MASK_CUSTOM_START   (((UINT32) 0x01) << 2)    /**<  Indicates the data is required when the item's custom data has changed      */


typedef UINT8 WGL_MBW_NVGT_DIR_T;    /**<  navigation direction       */
#define WGL_MBW_NVGT_DIR_LEFT                        ((WGL_MBW_NVGT_DIR_T)  0)    /**<  Indicates the navigation direction is left.      */
#define WGL_MBW_NVGT_DIR_RIGHT                       ((WGL_MBW_NVGT_DIR_T)  1)    /**<  Indicates the navigation direction is right.       */
#define WGL_MBW_NVGT_DIR_UP                          ((WGL_MBW_NVGT_DIR_T)  2)    /**<  Indicates the navigation direction is up.       */
#define WGL_MBW_NVGT_DIR_DOWN                        ((WGL_MBW_NVGT_DIR_T)  3)    /**<  Indicates the navigation direction is down.     */
#define WGL_MBW_NVGT_DIR_PG_UP                       ((WGL_MBW_NVGT_DIR_T)  4)    /**<  Indicates the navigation direction is page up.       */
#define WGL_MBW_NVGT_DIR_PG_DOWN                     ((WGL_MBW_NVGT_DIR_T)  5)    /**<  Indicates the navigation direction is page down.       */

#define WGL_MBW_MAKE_NVGT_DIR_MASK(_e_nvgt_dir)      MAKE_BIT_MASK_32((UINT8) (_e_nvgt_dir))    /**<   navigation direction mask     */

#define WGL_MBW_NVGT_DIR_MASK_LEFT                   WGL_MBW_MAKE_NVGT_DIR_MASK(WGL_MBW_NVGT_DIR_LEFT)    /**<  Indicates the navigation direction mask is left      */
#define WGL_MBW_NVGT_DIR_MASK_RIGHT                  WGL_MBW_MAKE_NVGT_DIR_MASK(WGL_MBW_NVGT_DIR_RIGHT)    /**< Indicates the navigation direction mask is right       */
#define WGL_MBW_NVGT_DIR_MASK_UP                     WGL_MBW_MAKE_NVGT_DIR_MASK(WGL_MBW_NVGT_DIR_UP)    /**<   Indicates the navigation direction mask is up.      */
#define WGL_MBW_NVGT_DIR_MASK_DOWN                   WGL_MBW_MAKE_NVGT_DIR_MASK(WGL_MBW_NVGT_DIR_DOWN)    /**< Indicates the navigation direction mask is down.       */
#define WGL_MBW_NVGT_DIR_MASK_PG_UP                  WGL_MBW_MAKE_NVGT_DIR_MASK(WGL_MBW_NVGT_DIR_PG_UP)    /**< Indicates the navigation direction mask is page up       */
#define WGL_MBW_NVGT_DIR_MASK_PG_DOWN                WGL_MBW_MAKE_NVGT_DIR_MASK(WGL_MBW_NVGT_DIR_PG_DOWN)    /**< Indicates the navigation direction mask is page up       */


typedef UINT8   WGL_MBW_KEY_HDLR_TYPE_T;    /**<  key handler type      */

#define WGL_MBW_KEY_HDLR_TYPE_SELECT                 ((WGL_MBW_KEY_HDLR_TYPE_T)  1)    /**<     Indicates the key handler type is selection   */
#define WGL_MBW_KEY_HDLR_TYPE_NVGT_LEFT              ((WGL_MBW_KEY_HDLR_TYPE_T)  2)    /**<  Indicates the key handler type is navigation left.       */
#define WGL_MBW_KEY_HDLR_TYPE_NVGT_RIGHT             ((WGL_MBW_KEY_HDLR_TYPE_T)  3)    /**<  Indicates the key handler type is navigation right.       */
#define WGL_MBW_KEY_HDLR_TYPE_NVGT_UP                ((WGL_MBW_KEY_HDLR_TYPE_T)  4)    /**<  Indicates the key handler type is navigation up.       */
#define WGL_MBW_KEY_HDLR_TYPE_NVGT_DOWN              ((WGL_MBW_KEY_HDLR_TYPE_T)  5)    /**<   Indicates the key handler type is navigation down.      */
#define WGL_MBW_KEY_HDLR_TYPE_NVGT_PAGE_UP           ((WGL_MBW_KEY_HDLR_TYPE_T)  6)    /**<  Indicates the key handler type is navigation page p.       */
#define WGL_MBW_KEY_HDLR_TYPE_NVGT_PAGE_DOWN         ((WGL_MBW_KEY_HDLR_TYPE_T)  7)    /**<  Indicates the key handler type is navigation page down.Key Handler Mask Macros     */


#define WGL_MBW_MAKE_KEY_HDLR_MASK(_e_key_hdlr_type) MAKE_BIT_MASK_32((UINT8) (_e_key_hdlr_type))    /**<  key handler mask      */

#define WGL_MBW_KEY_HDLR_MASK_SELECT                 WGL_MBW_MAKE_KEY_HDLR_MASK(WGL_MBW_KEY_HDLR_TYPE_SELECT)    /**< Indicates the key handler type WGL_MBW_KEY_HDLR_TYPE_SELECT is valid       */
#define WGL_MBW_KEY_HDLR_MASK_NVGT_LEFT              WGL_MBW_MAKE_KEY_HDLR_MASK(WGL_MBW_KEY_HDLR_TYPE_NVGT_LEFT)    /**<   Indicates the key handler type   WGL_MBW_KEY_HDLR_TYPE_ NVGT_LEFT is valid     */
#define WGL_MBW_KEY_HDLR_MASK_NVGT_RIGHT             WGL_MBW_MAKE_KEY_HDLR_MASK(WGL_MBW_KEY_HDLR_TYPE_NVGT_RIGHT)    /**<  Indicates the key handler type   WGL_MBW_KEY_HDLR_TYPE_ NVGT_RIGHT is valid      */
#define WGL_MBW_KEY_HDLR_MASK_NVGT_UP                WGL_MBW_MAKE_KEY_HDLR_MASK(WGL_MBW_KEY_HDLR_TYPE_NVGT_UP)    /**<  Indicates the key handler type   WGL_MBW_KEY_HDLR_TYPE_ NVGT_UP is valid      */
#define WGL_MBW_KEY_HDLR_MASK_NVGT_DOWN              WGL_MBW_MAKE_KEY_HDLR_MASK(WGL_MBW_KEY_HDLR_TYPE_NVGT_DOWN)    /**<   Indicates the key handler type   WGL_MBW_KEY_HDLR_TYPE_ NVGT_DOWN is valid     */
#define WGL_MBW_KEY_HDLR_MASK_NVGT_PAGE_UP           WGL_MBW_MAKE_KEY_HDLR_MASK(WGL_MBW_KEY_HDLR_TYPE_NVGT_PAGE_UP)    /**<  Indicates the key handler type   WGL_MBW_KEY_HDLR_TYPE_ NVGT_PAGE_UP is valid      */
#define WGL_MBW_KEY_HDLR_MASK_NVGT_PAGE_DOWN         WGL_MBW_MAKE_KEY_HDLR_MASK(WGL_MBW_KEY_HDLR_TYPE_NVGT_PAGE_DOWN)    /**<  Indicates the key handler type   WGL_MBW_KEY_HDLR_TYPE_ NVGT_PAGE_ DOWN is valid      */


typedef UINT8   WGL_MBW_EVN_HDLR_TYPE_T;    /**<   event handler type      */

#define WGL_MBW_EVN_HDLR_TYPE_NVGT_IN_VP             ((WGL_MBW_EVN_HDLR_TYPE_T) 1)    /**< Indicates the event handler type is navigation within current view port.       */
#define WGL_MBW_EVN_HDLR_TYPE_NVGT_WITH_VP_CHG       ((WGL_MBW_EVN_HDLR_TYPE_T) 2)    /**<  Indicates the event handler type is navigation with view port change.       */
#define WGL_MBW_EVN_HDLR_TYPE_SELECT                 ((WGL_MBW_EVN_HDLR_TYPE_T) 3)    /**< Indicates the event handler type is selection.        */
#define WGL_MBW_EVN_HDLR_TYPE_DESEL                  ((WGL_MBW_EVN_HDLR_TYPE_T) 4)    /**<  Indicates the event handler type is de-selection.        */

#define WGL_MBW_MAKE_EVN_HDLR_MASK(_e_evn_hdlr_type) MAKE_BIT_MASK_32((UINT8) (_e_evn_hdlr_type))    /**<  event handler mask      */

#define WGL_MBW_EVN_HDLR_MASK_NVGT_IN_VP             WGL_MBW_MAKE_EVN_HDLR_MASK(WGL_MBW_EVN_HDLR_TYPE_NVGT_IN_VP)    /**<  Indicates the event handler type WGL_MBW_EVN_HDLR_TYPE_NVGT_IN_VP is valid      */
#define WGL_MBW_EVN_HDLR_MASK_NVGT_WITH_VP_CHG       WGL_MBW_MAKE_EVN_HDLR_MASK(WGL_MBW_EVN_HDLR_TYPE_NVGT_WITH_VP_CHG)    /**< Indicates the event handler type WGL_MBW_EVN_HDLR_TYPE_NVGT_WITH_VP_CHG is valid       */
#define WGL_MBW_EVN_HDLR_MASK_SELECT                 WGL_MBW_MAKE_EVN_HDLR_MASK(WGL_MBW_EVN_HDLR_TYPE_SELECT)    /**<Indicates the event handler type WGL_MBW_EVN_HDLR_TYPE_SELECT is valid        */
#define WGL_MBW_EVN_HDLR_MASK_DESEL                  WGL_MBW_MAKE_EVN_HDLR_MASK(WGL_MBW_EVN_HDLR_TYPE_DESEL)    /**<   Indicates the event handler type WGL_MBW_EVN_HDLR_TYPE_DESEL is valid     */


/* image target */
#define WGL_IMG_MBW_VP_BK    (WGL_IMG_WGT_SPF_START + ((UINT8) 0x00))    /**<    Indicates image suit is set to view port background     */

/* color target */
#define WGL_CLR_MBW_VP_BK    (WGL_CLR_WGT_SPF_START + ((UINT8) 0x00))    /**<  Indicates color is set to view port background      */

/*------------------------------------------------------------------*/
/*! @enum WGL_MBW_NVGT_HORIZ_CUSTOM_T
 *  @brief When user navigate horizontal in one row and reach the row's boundary, this structure defines the boundary move behaviour. 
 *            navigation horizontal means navigate left/right  in one row in Vertical Aarrange Mode and navigate up/down in one row in Horizontal Arrange Mode.
 *  @code
 * typedef enum _WGL_MBW_NVGT_HORIZ_CUSTOM_T
 *{
 *   WGL_MBW_NVGT_HORIZ_NORMAL      = 0,
 *   WGL_MBW_NVGT_HORIZ_LOOP_IN_ONE_PAGE  = 1,
 *   WGL_MBW_NVGT_HORIZ_LOOP_CROSS_PAGES  = 2
 *} WGL_MBW_NVGT_HORIZ_CUSTOM_T;
 *  @endcode
 *  @li@c  WGL_MBW_NVGT_HORIZ_NORMAL               -Indicate user will navigate to the first item of the next row or the last item of the previous row when navigate horizontal in one row and reach the row's right or left bourndary.
 *  @li@c  WGL_MBW_NVGT_HORIZ_LOOP_IN_ONE_PAGE  -Indicate user will navigate to the first item of the same row or the last item of the same row when navigate horizontal in one row and reach the row's right or left bourndary.
 *  @li@c  WGL_MBW_NVGT_HORIZ_LOOP_CROSS_PAGES      -The deference of this define from WGL_MBW_NVGT_HORIZ_LOOP_IN_ONE_PAGE is the same row's range is in all pages rather than in one page.
 */
/*------------------------------------------------------------------*/

typedef enum _WGL_MBW_NVGT_HORIZ_CUSTOM_T
{
    WGL_MBW_NVGT_HORIZ_NORMAL      = 0,
    WGL_MBW_NVGT_HORIZ_LOOP_IN_ONE_PAGE  = 1,
    WGL_MBW_NVGT_HORIZ_LOOP_CROSS_PAGES  = 2
} WGL_MBW_NVGT_HORIZ_CUSTOM_T;


/*------------------------------------------------------------------*/
/*! @enum WGL_MBW_NVGT_VERTI_CUSTOM_T
 *  @brief When user navigate vertical in one column and reach the column's boundary, this structure defines the boundary move behaviour. 
 *            navigation vertical means navigate up/down  in one column in Vertical Aarrange Mode and navigate left/right in one column in Horizontal Arrange Mode.
 *  @code
 * typedef enum _WGL_MBW_NVGT_VERTI_CUSTOM_T
 *{
 *   WGL_MBW_NVGT_VERTI_NORMAL      = 0,
 *   WGL_MBW_NVGT_VERTI_LOOP_IN_ONE_PAGE = 1
 *} WGL_MBW_NVGT_VERTI_CUSTOM_T;
 *  @endcode
 *  @li@c  WGL_MBW_NVGT_VERTI_NORMAL               -Indicate user will navigate to the first item of the next column or the last item of the previous column when navigate vertical in one column and reach the column's top or bottom bourndary.
 *  @li@c  WGL_MBW_NVGT_VERTI_LOOP_IN_ONE_PAGE  -Indicate user will navigate to the first item of the same column or the last item of the same column when navigate vertical in one column and reach the column's top or bottom  bourndary.
 */
/*------------------------------------------------------------------*/

typedef enum _WGL_MBW_NVGT_VERTI_CUSTOM_T
{
    WGL_MBW_NVGT_VERTI_NORMAL      = 0,
    WGL_MBW_NVGT_VERTI_LOOP_IN_ONE_PAGE = 1
} WGL_MBW_NVGT_VERTI_CUSTOM_T;

/*------------------------------------------------------------------*/
/*! @enum WGL_MBW_NVGT_PG_DOWN_CUSTOM_T
 *  @brief Define Matrixbox navigate page down's behaviour.
 *  @code
 *  typedef enum _WGL_MBW_NVGT_PG_DOWN_CUSTOM_T
 * {
 *   WGL_MBW_NVGT_PG_DOWN_NORMAL      = 0,
 *   WGL_MBW_NVGT_PG_DOWN_CUSTOM_1       = 1
 * } WGL_MBW_NVGT_PG_DOWN_CUSTOM_T;
 *  @endcode
 *  @li@c  WGL_MBW_NVGT_PG_DOWN_NORMAL             -
 *Single page grid view:
 *The RC_Next key will bring the highlight moved from current highlight item to the last item of the same column on the screen and remain there if the same key is pressed again.
 *Multiple page grid view:
 *The RC_Next key will bring the highlight moved from current highlight item to the last item of the same column on the same page.
 * The RC_Next key will bring user to the next available page if the current highlight item has already been the last item of the column. The highlight is placed on the first item of the same column on
 *the new page. 
 *  @li@c  WGL_MBW_NVGT_PG_DOWN_CUSTOM_1         -
 *Single page grid view:
 *The RC_Next key will bring the highlight to the last icon on the screen and remain there if the same key is pressed again.
 *Multiple page grid view:
 *The RC_Next key will bring user to the next available page . The highlight is placed on the first icon on
 *the new page. On the last page, RC_Next will move the highlight icon to the last icon.
 */
/*------------------------------------------------------------------*/

typedef enum _WGL_MBW_NVGT_PG_DOWN_CUSTOM_T
{
    WGL_MBW_NVGT_PG_DOWN_NORMAL      = 0,
    WGL_MBW_NVGT_PG_DOWN_CUSTOM_1     = 1
} WGL_MBW_NVGT_PG_DOWN_CUSTOM_T;

/*------------------------------------------------------------------*/
/*! @enum WGL_MBW_NVGT_PG_UP_CUSTOM_T
 *  @brief Define Matrixbox navigate page up's behaviour.
 *  @code
 *  typedef enum _WGL_MBW_NVGT_PG_UP_CUSTOM_T
 * {
 *   WGL_MBW_NVGT_PG_UP_NORMAL      = 0,
 *   WGL_MBW_NVGT_PG_UP_CUSTOM_1       = 1
 * } WGL_MBW_NVGT_PG_UP_CUSTOM_T;
 *  @endcode
 *  @li@c  WGL_MBW_NVGT_PG_UP_NORMAL             -
 *Single page grid view:
 *The RC_Prev key will bring the highlight moved from current highlight item to the first item of the same column on the screen and remain there if the same key is pressed again.
 *Multiple page grid view:
 *The RC_Prev key will bring the highlight moved from current highlight item to the first item of the same column on the same page.
 * The RC_Prev key will bring user to the privious available page if the current highlight item has already been the first item of the column and the highlight is placed on the last item of the same column on
 *the new page. 
 *  @li@c  WGL_MBW_NVGT_PG_UP_CUSTOM_1               -
 * Single page grid view:
 * The RC_Prev key will bring the highlight to the first icon on the screen and remain there if the same key is pressed again.
 * Multiple page grid view
 * The RC_Prev key will bring user to the previous available page . The highlight is placed on the first icon
 * on the new page. There will be no action if highlight is on the first icon on the first page and RC_Prev is pressed .
 */
/*------------------------------------------------------------------*/

typedef enum _WGL_MBW_NVGT_PG_UP_CUSTOM_T
{
    WGL_MBW_NVGT_PG_UP_NORMAL      = 0,
    WGL_MBW_NVGT_PG_UP_CUSTOM_1     = 1
} WGL_MBW_NVGT_PG_UP_CUSTOM_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_MBW_NVGT_INFO_T
 *  @brief   navigation info 
 *  @code
 *  typedef struct _WGL_MBW_NVGT_INFO_T
 *  {
 *      UINT8       ui1_strct_ver;
 *      BOOL        b_navigable;
 *      BOOL        b_wrap_over;
 *      BOOL        b_vp_changed;
 *      WGL_MBW_NVGT_DIR_T  e_nvgt_dir;
 *      UINT32      ui4_idx_new;
 *      UINT32      ui4_idx_orig;
 *  } WGL_MBW_NVGT_INFO_T;
 *  @endcode
 *  @li@c  ui1_strct_ver                                     -used for version control, must be 1
 *  @li@c  b_navigable                                       -indicates whether the item in such navigation direction is navigable
 *  @li@c  b_wrap_over                                       -indicates whether needs wrap over
 *  @li@c  b_vp_changed                                      -indicates  whether view port needs change
 *  @li@c  e_nvgt_dir                                        -records the navigation direction
 *  @li@c  ui4_idx_new                                       -indicates the new highlight item after the navigation operation
 *  @li@c  ui4_idx_orig                                      -indicates the original item before the navigation operation
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_MBW_NVGT_INFO_T
{
    UINT8       ui1_strct_ver;

    BOOL        b_navigable;

    BOOL        b_wrap_over;

    BOOL        b_vp_changed;

    WGL_MBW_NVGT_DIR_T  e_nvgt_dir;

    UINT32      ui4_idx_new;

    UINT32      ui4_idx_orig;

} WGL_MBW_NVGT_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct  WGL_MBW_NVGT_CUSTOM_T
 *  @brief  navigation custom info
 *  @code
 *  typedef struct _WGL_MBW_NVGT_CUSTOM_T
 *  {
 *      UINT8       ui1_strct_ver;
 *      WGL_MBW_NVGT_HORIZ_CUSTOM_T t_nvgt_horiz_custom;
 *      WGL_MBW_NVGT_VERTI_CUSTOM_T t_nvgt_verti_custom;
 *      WGL_MBW_NVGT_PG_DOWN_CUSTOM_T t_nvgt_pg_down_custom;
 *      WGL_MBW_NVGT_PG_UP_CUSTOM_T t_nvgt_pg_up_custom;
 *  } WGL_MBW_NVGT_CUSTOM_T;
 *  @endcode
 *  @li@c  ui1_strct_ver                            -used for version control, must be 1
 *  @li@c  t_nvgt_horiz_custom                      -  indicates the boundary move behavior in same row.
 *  @li@c  t_nvgt_verti_custom                      -indicates the boundary move behavior in same column.
 *  @li@c  t_nvgt_pg_down_custom                    -  indicates page down key's behaviour.
 *  @li@c  t_nvgt_pg_up_custom                      - indicates page up key's behaviour.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_MBW_NVGT_CUSTOM_T
{
    UINT8                       ui1_strct_ver;
    WGL_MBW_NVGT_HORIZ_CUSTOM_T t_nvgt_horiz_custom;
    WGL_MBW_NVGT_VERTI_CUSTOM_T t_nvgt_verti_custom;
    WGL_MBW_NVGT_PG_DOWN_CUSTOM_T t_nvgt_pg_down_custom;
    WGL_MBW_NVGT_PG_UP_CUSTOM_T t_nvgt_pg_up_custom;
} WGL_MBW_NVGT_CUSTOM_T;


/*------------------------------------------------------------------*/
/*! @struct  WGL_MBW_KEY_MAP_T
 *  @brief  Item navigation map
 *  @code
 *  typedef struct _WGL_MBW_KEY_MAP_T
 *  {
 *      UINT8                       ui1_strct_ver;
 *      UINT32                      ui4_key_select;
 *      UINT32                      ui4_key_nvgt_left;
 *      UINT32                      ui4_key_nvgt_right;
 *      UINT32                      ui4_key_nvgt_up;
 *      UINT32                      ui4_key_nvgt_down;
 *      UINT32                      ui4_key_nvgt_page_up;
 *      UINT32                      ui4_key_nvgt_page_down;
 *  } WGL_MBW_KEY_MAP_T;
 *  @endcode
 *  @li@c  ui1_strct_ver                                     -Used for version control, must be 1
 *  @li@c  ui4_key_select                                    -Contains the key code of the key that selects or deselects the highlighted item.
 *  @li@c  ui4_key_nvgt_left                                 -Contains the key code of the key that changes the highlighted item to the left one.
 *  @li@c  ui4_key_nvgt_right                                -Contains the key code of the key that changes the highlighted item to the right one.
 *  @li@c  ui4_key_nvgt_up                                   -Contains the key code of the key that changes the highlighted item to the upper one.
 *  @li@c  ui4_key_nvgt_down                                 -Contains the key code of the key that changes the highlighted item to the downer one.
 *  @li@c  ui4_key_nvgt_page_up                              -Contains the key code of the key that performs the page-up operation.
 *  @li@c  ui4_key_nvgt_page_down                            -Contains the key code of the key that performs the page-down operation.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_MBW_KEY_MAP_T
{
    UINT8                       ui1_strct_ver;

    UINT32                      ui4_key_select;
    UINT32                      ui4_key_nvgt_left;
    UINT32                      ui4_key_nvgt_right;
    UINT32                      ui4_key_nvgt_up;
    UINT32                      ui4_key_nvgt_down;
    UINT32                      ui4_key_nvgt_page_up;
    UINT32                      ui4_key_nvgt_page_down;

} WGL_MBW_KEY_MAP_T;


/*------------------------------------------------------------------*/
/*! @struct  WGL_MBW_ITEM_NVGT_MAP_T
 *  @brief  Item navigation map
 *  @code
 *  typedef struct _WGL_MBW_ITEM_NVGT_MAP_T
 *  {
 *      UINT8                       ui1_strct_ver;
 *      UINT32                      ui4_item_nvgt_dir_mask;
 *      UINT32                      ui4_item_idx_nvgt_left;
 *      UINT32                      ui4_item_idx_nvgt_right;
 *      UINT32                      ui4_item_idx_nvgt_up;
 *      UINT32                      ui4_item_idx_nvgt_down;
 *      UINT32                      ui4_item_idx_nvgt_page_up;
 *      UINT32                      ui4_item_idx_nvgt_page_down;
 *  } WGL_MBW_ITEM_NVGT_MAP_T;
 *  @endcode
 *  @li@c  ui1_strct_ver                                     -used for version control, must be 1
 *  @li@c  ui4_item_nvgt_dir_mask                            -indicates which navigation direction is valid.
 *  @li@c  ui4_item_idx_nvgt_left                            -indicates the new item index after left navigation.
 *  @li@c  ui4_item_idx_nvgt_right                           -indicates the new item index after right navigation
 *  @li@c  ui4_item_idx_nvgt_up                              -indicates the new item index after up navigation.
 *  @li@c  ui4_item_idx_nvgt_down                            -indicates the new item index after down navigation.
 *  @li@c  ui4_item_idx_nvgt_page_up                         -indicates the new item index after page up navigation.
 *  @li@c  ui4_item_idx_nvgt_page_down                       -indicates the new item index after page down navigation.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_MBW_ITEM_NVGT_MAP_T
{
    UINT8                       ui1_strct_ver;

    UINT32                      ui4_item_nvgt_dir_mask;
    UINT32                      ui4_item_idx_nvgt_left;
    UINT32                      ui4_item_idx_nvgt_right;
    UINT32                      ui4_item_idx_nvgt_up;
    UINT32                      ui4_item_idx_nvgt_down;
    UINT32                      ui4_item_idx_nvgt_page_up;
    UINT32                      ui4_item_idx_nvgt_page_down;

} WGL_MBW_ITEM_NVGT_MAP_T;

#define WGL_MBW_ITEM_DATA_MASK_NVGT_MAP           (((UINT32) 0x01) << 0)    /**<    Indicates the item's navigation map is required or set.    */


/*------------------------------------------------------------------*/
/*! @struct  WGL_MBW_ITEM_DATA_T
 *  @brief  Item data structure
 *  @code
 *  typedef struct _WGL_MBW_ITEM_DATA_T
 *  {
 *      UINT8                    ui1_strct_ver;
 *      UINT32                   ui4_data_mask;
 *      WGL_MBW_ITEM_NVGT_MAP_T  t_item_nvgt_map;
 *  } WGL_MBW_ITEM_DATA_T;
 *  @endcode
 *  @li@c  ui1_strct_ver                                     -used for version control, must be 1
 *  @li@c  ui4_data_mask                                     -indicates which item data is valid.
 *  @li@c  t_item_nvgt_map                                   -records the item's own navigation map
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_MBW_ITEM_DATA_T
{
    UINT8                    ui1_strct_ver;
    
    UINT32                   ui4_data_mask;

    WGL_MBW_ITEM_NVGT_MAP_T  t_item_nvgt_map;
    /*some other data referred to the item maybe added later*/
} WGL_MBW_ITEM_DATA_T;

/*------------------------------------------------------------------*/
/*! @struct  WGL_MBW_EXTRA_MARGIN_T
 *  @brief  define extra margin
 *  @code
 *  typedef struct _WGL_MBW_EXTRA_MARGIN_T
 *  {
 *      UINT16              ui2_left;
 *      UINT16              ui2_top;
 *      UINT16              ui2_right;
 *      UINT16              ui2_bottom;
 *  } WGL_MBW_EXTRA_MARGIN_T;
 *  @endcode
 *  @li@c  ui2_left                                          -indicates the left extra margin of the highlight item.
 *  @li@c  ui2_top                                           -indicates the right extra margin of the highlight item.
 *  @li@c  ui2_right                                         -indicates the top extra margin of the highlight item.
 *  @li@c  ui2_bottom                                        -indicates the bottom extra margin of the highlight item.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_MBW_EXTRA_MARGIN_T
{
    UINT16              ui2_left;
    UINT16              ui2_top;
    UINT16              ui2_right;
    UINT16              ui2_bottom;

} WGL_MBW_EXTRA_MARGIN_T;


/*------------------------------------------------------------------*/
/*! @struct   WGL_MBW_ITEM_CTNR_INFO_T
 *  @brief  Item container info
 *  @code
 *  typedef struct _WGL_MBW_ITEM_CTNR_INFO_T
 *  {
 *      UINT8       ui1_strct_ver;
 *      HANDLE_T    h_item_ctnr;
 *      VOID*       pv_item_ctnr_info;
 *  }WGL_MBW_ITEM_CTNR_INFO_T;
 *  @endcode
 *  @li@c  ui1_strct_ver                                     - used for version control, must be 1
 *  @li@c  h_item_ctnr                                       -  indicates item container handler.
 *  @li@c  pv_item_ctnr_info                                 - records private data related to h_item_ctnr by Data Model
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_MBW_ITEM_CTNR_INFO_T
{
    UINT8       ui1_strct_ver;
    
    HANDLE_T    h_item_ctnr;
    
    VOID*       pv_item_ctnr_info;
    
}WGL_MBW_ITEM_CTNR_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct  WGL_MBW_DM_INIT_INFO_T  
 *  @brief  data model init info
 *  @code
 *  typedef struct _WGL_MBW_DM_INIT_INFO_T
 *  {
 *      UINT8                       ui1_strct_ver;
 *      UINT32                      ui4_num_item_ctnr;
 *      WGL_MBW_ITEM_CTNR_INFO_T*   at_item_ctnr_info;     
 *      WGL_MBW_ITEM_CTNR_INFO_T*   pt_hlt_item_ctnr_info;  
 *  }WGL_MBW_DM_INIT_INFO_T;
 *  @endcode
 *  @li@c  ui1_strct_ver                                     -  used for version control, must be 1
 *  @li@c  ui4_num_item_ctnr                                 -  records item container number(excluding highlight item container)
 *  @li@c  at_item_ctnr_info                                 -  item container info array  
 *                                                                           in:  WGL_MBW_ITEM_CTNR_INFO_T.h_item_ctnr 
 *                                                                          out: WGL_MBW_ITEM_CTNR_INFO_T.pv_item_ctnr_info
 *  @li@c  pt_hlt_item_ctnr_info                             - highlight item container info pointer
 *                                                                          in:  WGL_MBW_ITEM_CTNR_INFO_T.h_item_ctnr 
 *                                                                         out: WGL_MBW_ITEM_CTNR_INFO_T.pv_item_ctnr_info
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_MBW_DM_INIT_INFO_T
{
    UINT8                       ui1_strct_ver;
    
    UINT32                      ui4_num_item_ctnr;
    
    WGL_MBW_ITEM_CTNR_INFO_T*   at_item_ctnr_info;    
    WGL_MBW_ITEM_CTNR_INFO_T*   pt_hlt_item_ctnr_info;  /*  */                                                        
}WGL_MBW_DM_INIT_INFO_T;


#define WGL_MBW_HLT_ITEM_STL_ENLRG_SIZE       MAKE_BIT_MASK_32 (0)    /**<   highlight item style:enlarge isze      */

#define WGL_MBW_HLT_ITEM_STL_LAST_STYLE       WGL_MBW_HLT_ITEM_STL_ENLRG_SIZE    /**<highlight item last style equals to   WGL_MBW_HLT_ITEM_STL_ENLRG_SIZE      */

/*------------------------------------------------------------------*/
/*! @struct WGL_MBW_HLT_ITEM_INFO_T
 *  @brief
 *  @code
 *  typedef struct _WGL_MBW_HLT_ITEM_INFO_T
 *  {
 *      UINT8                       ui1_strct_ver;
 *      UINT32                      ui4_style;
 *      WGL_MBW_EXTRA_MARGIN_T      t_ex_margin;
 *  } WGL_MBW_HLT_ITEM_INFO_T;
 *  @endcode
 *  @li@c  ui1_strct_ver                                     -
 *  @li@c  ui4_style                                         -
 *  @li@c  t_ex_margin                                       -
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_MBW_HLT_ITEM_INFO_T
{
    UINT8                       ui1_strct_ver;

    UINT32                      ui4_style;

    /* WGL_MBW_HLT_EFCT_STL_ENLARGE_SIZE */
    WGL_MBW_EXTRA_MARGIN_T      t_ex_margin;

} WGL_MBW_HLT_ITEM_INFO_T;

/*MatrixBox style*/
#define WGL_MBW_STL_HORIZ_ARRANGE       MAKE_BIT_MASK_32 (0)   /**<If this style is assigned, the Matrix Box arrange the items in horizontal mode; otherwise the Matrix Box arrange the items in vertical mode        */
#define WGL_MBW_STL_PAGE_MODE           MAKE_BIT_MASK_32 (1)   /**<If this style is assigned, when users attempt to highlight an invisible element by remote control keys, Matrix Box shows up invisible elements in page-up/page-down manner, instead of scrolling elements into the viewport.*/
#define WGL_MBW_STL_WRAP_OVER_HORIZ     MAKE_BIT_MASK_32 (2) /**<  In Vertical Arrange Mode, if this style is not assigned, the customized boundary move style is ignored. For example, we assume the case that when the current highlight item is in right boundary. If the user presses the "right" control key, either of the following cases will happen:

                                                                                                             	If this style is NOT assigned, the highlighted item remains on the boundary and hence this event doesn't cause any effect

                                                                                                             	If this style is assigned, the highlighted item will changed by the customized boundary move style.

                                                                                                                In Horizontal Arrange Mode, if this style is assigned, items in the Matrix Box will be highlighted in wrap over mode. For example, we assume the case that when the last item in a Matrix Box is highlighted. If the user presses the "next" control key, either of the following two cases will happen:

                                                                                                             	If this style is assigned, the highlighted item is changed to the first element of the Matrix Box.

                                                                                                             	If this style is NOT assigned, the highlighted item remains on the last item and hence this event doesn't cause any effect. 
                                                                                                             	*/

#define WGL_MBW_STL_WRAP_OVER_VERTI     MAKE_BIT_MASK_32 (3)    /**<In Horizontal Arrange Mode, if this style is not assigned, the customized boundary move style is ignored. For example, we assume the case that when the current highlight item is in upper boundary. If the user presses the "up" control key, either of the following cases will happen:

                                                                                                                 	If this style is NOT assigned, the highlighted item remains on the boundary and hence this event doesn't cause any effect

                                                                                                                 	If this style is assigned, the highlighted item will changed by the customized boundary move style.

                                                                                                                       In Vertical Arrange Mode, if this style is assigned, items in the Matrix Box will be highlighted in wrap over mode. For example, we assume the case that when the last item in a Matrix Box is highlighted. If the user presses the "next" control key, either of the following two cases will happen:

                                                                                                                 	If this style is assigned, the highlighted item is changed to the first element of the Matrix Box.

                                                                                                                 	If this style is NOT assigned, the highlighted item remains on the last item and hence this event doesn't cause any effect.
                                                                                                                */

#define WGL_MBW_STL_LAST_STYLE          WGL_MBW_STL_WRAP_OVER_VERTI    /**< LAST style equals to   WGL_MBW_STL_WRAP_OVER_VERTI     */



/*------------------------------------------------------------------*/
/*! @brief The application callback, as its name implies, is an application-defined callback function. This procedure must be specified when creating the mbw widget. It is called when key message received by mbw. This callback function intends to customize app key message process flow.
 *  @param [in]  pv_tag  Contains an application-defined tag value which is stalled in WGL_MBW_KEY_HDLR_INFO_T when creating the mbw widget or registering key handler through c_wgl_mbw_reg_key_hdlr()..
 *  @param [in]   h_mbw   Contains the handle to the mbw widget
 *  @param [in]   e_key_hdlr_type                    key handler function type
 *  @param [in]   ui4_data                                 reserved for future, now only 0 is valid.
 *  @param [out] None
 *  @note
 *  @see
 *  @return None
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*wgl_mbw_key_hdlr_fct) (
    HANDLE_T                    h_mbw,
    VOID*                       pv_tag,
    WGL_MBW_KEY_HDLR_TYPE_T     e_key_hdlr_type,
    UINT32                      ui4_data);

/*------------------------------------------------------------------*/
/*! @brief The application callback, as its name implies, is an application-defined callback function. This procedure must be specified when creating the mbw widget. It is called when related event occurred. This callback function intends to customize app event process flow.
 *  @param [in]  pv_tag  Contains an application-defined tag value which is stalled in WGL_MBW_EVN_HDLR_INFO_T when creating the mbw widget or registering event handler through c_wgl_mbw_reg_event_hdlr().
 *  @param [in]   h_mbw   Contains the handle to the mbw widget
 *  @param [in]   e_evn_hdlr_type                    event handler function type
 *  @param [in]   ui4_data                                 reserved for future, now only 0 is valid.
 *  @param [out] None
 *  @note
 *  @see
 *  @return None
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*wgl_mbw_event_hdlr_fct) (
    HANDLE_T                    h_mbw,
    VOID*                       pv_tag,
    WGL_MBW_EVN_HDLR_TYPE_T     e_evn_hdlr_type,
    UINT32                      ui4_data);



typedef UINT32 WGL_MBW_DM_EX_SET_TYPE_T;    /**<   Intends to be expanded in future    */

typedef UINT32 WGL_MBW_DM_EX_GET_TYPE_T;    /**< Intends to be expanded in future      */

/* data model function table */
/*------------------------------------------------------------------*/
/*! @brief  The application callback, as its name implies, is an application-defined callback function. This procedure must be specified when creating the mbw widget. It is called during mbw widget creation process. This callback function intends to initialize Data Model build up by the application, Including:
 *              1, setup item component template. For example the application want to let every item in mbw has a Text widget and a Icon widget, thus application can setup a Text widget and a Icon widget for every item container in this api.
 *              2, setup highlight  item component template. For example the application want to let every highlight item in mbw has a Text widget and a Icon widget, thus application can setup a Text widget and a Icon widget for the highlight item container in this api.
 *  @param [in]   h_mbw   Contains the handle to the mbw widget
 *  @param [in]   pt_dm_init_info WGL_MBW_DM_INIT_INFO_T structure pointer, h_item_ctnr  in at_item_ctnr_info is input. h_item_ctnr  in pt_hlt_item_ctnr_info is input.
 *  @param [out]   ppv_dm_data                        Data Model private data which is stored in Matrix widget and sent to Data Model when other Data Model callback function called.
 *  @param [out]  pt_dm_init_info WGL_MBW_DM_INIT_INFO_T structure pointer, pv_item_ctnr_info  in at_item_ctnr_info is output set by Data Model.
 *                          Data Model can add its item component to h_item_ctnr  in at_item_ctnr_info. pv_item_ctnr_info in pt_hlt_item_ctnr_info is output set by Data Model.
 *                          Data Model can add its highlight item component to h_item_ctnr  in pt_hlt_item_ctnr_info
 *  @note
 *  @see
 *  @return None
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef BOOL (*wgl_mbw_dm_init_fct) (
    HANDLE_T                h_mbw,
    VOID**                  ppv_dm_data,        /* out */
    WGL_MBW_DM_INIT_INFO_T* pt_dm_init_info);   /* in & out */

/*------------------------------------------------------------------*/
/*! @brief  The application callback, as its name implies, is an application-defined callback function. This procedure must be specified when creating the mbw widget. It is called during mbw widget destroy process. This callback function intends to deinitialize Data Model build up by the application.
 *              Including:
 *              1, clear item component template. For example the application setup every item in mbw with a Text widget and a Icon widget, thus application need to clear the Text widget and the Icon widget from its item container in this api.
 *              2, clear highlight  item component template. For example the application setup every highlight item in mbw with a Text widget and a Icon widget, thus application need to clear the Text widget and the Icon widget from the highlight item container in this api.
 *  @param [in]   pv_dm_data  Data Model private data which is stalled in Matrix widget when wgl_mbw_dm_init_fct is called.
 *  @param [in]    h_mbw   Contains the handle to the mbw widget
 *  @param [in]    pt_dm_init_info                      WGL_MBW_DM_INIT_INFO_T structure pointer
 *  @param [out]   pt_dm_init_info WGL_MBW_DM_INIT_INFO_T structure pointer, Data Model need remove its item component from h_item_ctnr  in at_item_ctnr_info.
 *                         Data Model need also remove its highlight item component from h_item_ctnr  in pt_hlt_item_ctnr_info.
 *  @note
 *  @see
 *  @return None
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*wgl_mbw_dm_deinit_fct) (
    VOID*                         pv_dm_data,
    HANDLE_T                      h_mbw,
    const WGL_MBW_DM_INIT_INFO_T* pt_dm_init_info); /* in */

/*------------------------------------------------------------------*/
/*! @brief The application callback, as its name implies, is an application-defined callback function. This procedure must be specified when creating the mbw widget. It is called whenever item container is updated. This callback function intends to fill item component data and container background image or color in input item container info array..
 *  @param [in]   pv_dm_data  Data Model private data which is stalled in Matrix widget when wgl_mbw_dm_init_fct is called.
 *  @param [in]   h_mbw   Contains the handle to the mbw widget
 *  @param [in]    ui2_state_mbw                       record the mbw widget current state
 *  @param [in]   ui4_ctnr_update_mask           
                            *valid values:
*                         1, WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATIC: indicates this update is caused by item container data changed 
*                         2, WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATE: indicates this update is caused by item state or widget state change
 *  @param [in]    ui4_idx_start                          begin item index of updated item container array, valid value range is 0-(item number-1) and can not larger than ui4_idx_end.
 *  @param [in]    ui4_idx_end                           end item index of updated item container array, valid value range is (0-item number-1) and can not less than ui4_idx_start.
 *  @param [in]    at_item_ctnr_info                  item container info array, array length is (ui4_idx_end- ui4_idx_start + 1)
 *  @param [out]  at_item_ctnr_info                   item container info array, filled in updated item component data.
 *  @note
 *  @see
 *  @return  None
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*wgl_mbw_dm_update_item_ctnr_fct) (
    VOID*                       pv_dm_data,
    HANDLE_T                    h_mbw,
    UINT16                      ui2_state_mbw,
    UINT32                      ui4_ctnr_update_mask,
    UINT32                      ui4_idx_start,
    UINT32                      ui4_idx_end,
    WGL_MBW_ITEM_CTNR_INFO_T*   at_item_ctnr_info);    /* in & out */

/*------------------------------------------------------------------*/
/*! @brief  The application callback, as its name implies, is an application-defined callback function. This procedure must be specified when creating the mbw widget. It is called whenever highlight item container is updated. This callback function intends to fill highlight item component data and container background image or color in input highlight  item container info.
 *  @param [in]   pv_dm_data  Data Model private data which is stalled in Matrix widget when wgl_mbw_dm_init_fct is called.
 *  @param [in]   h_mbw   Contains the handle to the mbw widget
 *  @param [in]    ui2_state_mbw                       record the mbw widget current state
 *  @param [in]   ui4_ctnr_update_mask           valid values:
 *                       1, WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATIC: indicates this update is caused by item container data changed 
 *                       2, WGL_MBW_ITEM_CTNR_UPDATE_MASK_STATE: indicates this update is caused by item state or widget state change
 *  @param [in]    ui4_idx_hlt                             indicates highlight item index
 *  @param [in]  pt_item_ctnr_info                   highlight item container info
 *  @param [out]  ph_item_ctnr                        highlight item container info, filled in updated highlight item component data and highlight item container background color or image data.
 *  @note
 *  @see
 *  @return None
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*wgl_mbw_dm_update_hlt_item_ctnr_fct) (
    VOID*                       pv_dm_data,
    HANDLE_T                    h_mbw,
    UINT16                      ui2_state_mbw,
    UINT32                      ui4_ctnr_update_mask,
    UINT32                      ui4_idx_hlt,
    WGL_MBW_ITEM_CTNR_INFO_T*   pt_item_ctnr_info);    /* in & out */

/*------------------------------------------------------------------*/
/*! @brief  The application callback, as its name implies, is an application-defined callback function. This procedure must be specified when creating the mbw widget. It is called whenever item state mask is enabled. This callback function intends to enable item state mask.
 *  @param [in]    pv_dm_data  Data Model private data which is stalled in Matrix widget when wgl_mbw_dm_init_fct is called.
 *  @param [in]    h_mbw   Contains the handle to the mbw widget
 *  @param [in]    ui4_idx                                   item index whose state mask need to be enable 
 *  @param [in]    ui4_state_mask                     item state mask
 *  @param [out] None
 *  @note
 *  @see
 *  @return None
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*wgl_mbw_dm_enable_item_state_fct) (
    VOID*       pv_dm_data,
    HANDLE_T    h_mbw,
    UINT32      ui4_idx,
    UINT32      ui4_state_mask);

/*------------------------------------------------------------------*/
/*! @brief  The application callback, as its name implies, is an application-defined callback function. This procedure must be specified when creating the mbw widget. It is called whenever item state mask is disabled. This callback function intends to disableitem state mask.
 *  @param [in]   pv_dm_data  Data Model private data which is stalled in Matrix widget when wgl_mbw_dm_init_fct is called.
 *  @param [in]   h_mbw   Contains the handle to the mbw widget
 *  @param [in]   ui4_idx                                   item index whose state mask need to be disable
 *  @param [in]   ui4_state_mask                     item state mask
 *  @param [out] None
 *  @note
 *  @see
 *  @return None
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*wgl_mbw_dm_disable_item_state_fct) (
    VOID*       pv_dm_data,
    HANDLE_T    h_mbw,
    UINT32      ui4_idx,
    UINT32      ui4_state_mask);

/*------------------------------------------------------------------*/
/*! @brief The application callback, as its name implies, is an application-defined callback function. This procedure must be specified when creating the mbw widget. It is called whenever item state mask is disabled. This callback function intends to disableitem state mask.
 *  @param [in]   pv_dm_data  Data Model private data which is stalled in Matrix widget when wgl_mbw_dm_init_fct is called.
 *  @param [in]   h_mbw   Contains the handle to the mbw widget
  *  @param [in]   ui4_idx                                   item index whose state mask need to be disable
 *  @param [in]   ui4_state_mask                     item state mask
 *  @param [out] None
 *  @note
 *  @see
 *  @return None
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*wgl_mbw_dm_get_item_state_fct) (
    VOID*       pv_dm_data,
    HANDLE_T    h_mbw,
    UINT32      ui4_idx_start,
    UINT32      ui4_idx_end,
    UINT32*     aui4_item_state);                     /* out */

/*------------------------------------------------------------------*/
/*! @brief  The application callback, as its name implies, is an application-defined callback function. This procedure must be specified when creating the mbw widget. It is called whenever items data are queried. This callback function intends to return items data to caller.
 *  @param [in]   pv_tag  Contains an application-defined tag value which is stalled in WGL_MBW_DATA_MODEL_INFO_T when creating the mbw widget.
 *  @param [in]   h_mbw   Contains the handle to the mbw widget
  *  @param [in]   ui4_idx_start                          begin item index of queried item array, valid value range is 0-(item number-1) and can not larger than ui4_idx_end.
 *  @param [in]   ui4_idx_end                           end item index of queried item array, valid value range is (0-item number-1) and can not less than ui4_idx_start.
 *  @param [in]   ui4_data_mask                      data mask indicates what type of data needed
 *  @param [in]   at_item_data                        item data array, array length is (ui4_idx_end- ui4_idx_start + 1)
 *  @param [out] at_item_data                         item data array, filled in requested item data, if an item has not data indicated by input data mask,  then the ui4_data_mask belongs to the item returned can not has corresponding data mask.  
 *  @note
 *  @see
 *  @return None
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*wgl_mbw_dm_get_item_data_fct) (
    VOID*                   pv_dm_data,
    HANDLE_T                h_mbw,
    UINT32                  ui4_idx_start,
    UINT32                  ui4_idx_end,
    UINT32                  ui4_data_mask,
    WGL_MBW_ITEM_DATA_T*    at_item_data);            /* out */

/*------------------------------------------------------------------*/
/*! @brief The application callback, as its name implies, is an application-defined callback function. This procedure must be specified when creating the mbw widget. This api intends to be expand in future. It is called whenever need stall  item private data in Data Model.
 *  @param [in]   pv_dm_data  Data Model private data which is stalled in Matrix widget when wgl_mbw_dm_init_fct is called.
 *  @param [in]    h_mbw   Contains the handle to the mbw widget
  *  @param [in]    e_set_type                             set type, expand in future
 *  @param [in]    pv_set_info                            set info it is private for application.
 *  @param [in]    z_set_info_len                      set info length
 *  @param [out] None
 *  @note
 *  @see
 *  @return None
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wgl_mbw_dm_ex_set_fct) (
    VOID*                       pv_dm_data,
    HANDLE_T                    h_mbw,
    WGL_MBW_DM_EX_SET_TYPE_T    e_set_type,
    const VOID*                 pv_set_info,
    SIZE_T                      z_set_info_len);

/*------------------------------------------------------------------*/
/*! @brief  The application callback, as its name implies, is an application-defined callback function. This procedure must be specified when creating the mbw widget. This api intends to be expand in future. It is called whenever need query stalled item private data in Data Model.
 *  @param [in]   
 *  @param [in]    pv_dm_data  Data Model private data which is stalled in Matrix widget when wgl_mbw_dm_init_fct is called.
 *  @param [in]     h_mbw   Contains the handle to the mbw widget
  *  @param [in]    e_set_type                             get type, expand in future
 *  @param [in]    pv_set_info                            get info it is private for application.
 *  @param [in]    z_set_info_len                      get info length
 *  @param [out] None
 *  @note
 *  @see
 *  @return None
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wgl_mbw_dm_ex_get_fct) (
    VOID*                      pv_dm_data,
    HANDLE_T                   h_mbw,
    WGL_MBW_DM_EX_GET_TYPE_T   e_get_type,
    VOID*                      pv_get_info,
    SIZE_T                     z_get_info_len);


/* other functions */

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*! @struct WGL_MBW_DATA_MODEL_FCT_TBL_T
 *  @brief  data model function table
 *  @code
 *  typedef struct _WGL_MBW_DATA_MODEL_FCT_TBL_T
 *  {
 *      UINT8                                       ui1_strct_ver;
 *      wgl_mbw_dm_init_fct                         pf_dm_init;
 *      wgl_mbw_dm_deinit_fct                       pf_dm_deinit;
 *      wgl_mbw_dm_update_item_ctnr_fct             pf_update_item_ctnr;
 *      wgl_mbw_dm_update_hlt_item_ctnr_fct         pf_update_hlt_item_ctnr;
 *      wgl_mbw_dm_enable_item_state_fct            pf_enable_item_state;
 *      wgl_mbw_dm_disable_item_state_fct           pf_disable_item_state;
 *      wgl_mbw_dm_get_item_data_fct                pf_get_item_data;
 *      wgl_mbw_dm_get_item_state_fct               pf_get_item_state;
 *      wgl_mbw_dm_ex_set_fct                       pf_ex_set;
 *      wgl_mbw_dm_ex_get_fct                       pf_ex_get;
 *  } WGL_MBW_DATA_MODEL_FCT_TBL_T;
 *  @endcode
 *  @li@c  ui1_strct_ver                                     - used for version control, must be 1
 *  @li@c  pf_dm_init                                        -  model initialize callback function pointer   
 *  @li@c  pf_dm_deinit                                      - data model de-initialize callback function pointer
 *  @li@c  pf_update_item_ctnr                               - data model update item container callback function pointer
 *  @li@c  pf_update_hlt_item_ctnr                           -  data model update highlight item container callback function pointer
 *  @li@c  pf_enable_item_state                              - data model enable item state callback function pointer
 *  @li@c  pf_disable_item_state                             - data model disable item state callback function pointer
 *  @li@c  pf_get_item_data                                  -  data model get item data callback function pointer
 *  @li@c  pf_get_item_state                                 - data model get item state callback function pointer
 *  @li@c  pf_ex_set                                         -  data model set extended data callback function pointer
 *  @li@c  pf_ex_get                                         -  data model get extended data callback function pointer
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_MBW_DATA_MODEL_FCT_TBL_T
{
    UINT8                                       ui1_strct_ver;
    
    wgl_mbw_dm_init_fct                         pf_dm_init;
    wgl_mbw_dm_deinit_fct                       pf_dm_deinit;
    wgl_mbw_dm_update_item_ctnr_fct             pf_update_item_ctnr;
    wgl_mbw_dm_update_hlt_item_ctnr_fct         pf_update_hlt_item_ctnr;
    wgl_mbw_dm_enable_item_state_fct            pf_enable_item_state;
    wgl_mbw_dm_disable_item_state_fct           pf_disable_item_state;
    wgl_mbw_dm_get_item_data_fct                pf_get_item_data;
    wgl_mbw_dm_get_item_state_fct               pf_get_item_state;
    wgl_mbw_dm_ex_set_fct                       pf_ex_set;
    wgl_mbw_dm_ex_get_fct                       pf_ex_get;

} WGL_MBW_DATA_MODEL_FCT_TBL_T;


/*-------------------------------------------------------------------*/
/*! @struct WGL_MBW_DATA_MODEL_INFO_T
 *  @brief data model info 
 *  @code
 *  typedef struct _WGL_MBW_DATA_MODEL_INFO_T
 *  {
 *      UINT8                           ui1_strct_ver;
 *      WGL_MBW_DATA_MODEL_FCT_TBL_T    t_fct_tbl;
 *  } WGL_MBW_DATA_MODEL_INFO_T;
 *  @endcode
 *  @li@c  ui1_strct_ver                                     - used for version control, must be 1
 *  @li@c  t_fct_tbl                                         -  data model callback function table
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_MBW_DATA_MODEL_INFO_T
{
    UINT8                           ui1_strct_ver;

    WGL_MBW_DATA_MODEL_FCT_TBL_T    t_fct_tbl;

} WGL_MBW_DATA_MODEL_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct WGL_MBW_KEY_HDLR_INFO_T
 *  @brief   key handler info
 *  @code
 *  typedef struct _WGL_MBW_KEY_HDLR_INFO_T
 *  {
 *      UINT8                       ui1_strct_ver;
 *      UINT32                      ui4_key_hdlr_mask;  
 *      VOID*                       pv_tag_key_hdlr;
 *      wgl_mbw_key_hdlr_fct        pf_key_hdlr;        
 *  } WGL_MBW_KEY_HDLR_INFO_T;
 *  @endcode 
 *  @li@c  ui1_strct_ver                                     - used for version control, must be 1
 *  @li@c  ui4_key_hdlr_mask                                 - key handler mask
 *  @li@c  pv_tag_key_hdlr                                   - attached data with the key handler
 *  @li@c  pf_key_hdlr                                       -  specified key handler procedure callback function, if  ui4_key_hdlr_mask is 0 then pf_key_hdlr can be NULL
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_MBW_KEY_HDLR_INFO_T
{
    UINT8                       ui1_strct_ver;

    UINT32                      ui4_key_hdlr_mask;  /* (WGL_MBW_KEY_HDLR_MASK_*) */

    VOID*                       pv_tag_key_hdlr;

    wgl_mbw_key_hdlr_fct        pf_key_hdlr;        /* can be NULL if (ui4_key_hdlr_mask == 0) */

} WGL_MBW_KEY_HDLR_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct  WGL_MBW_EVN_HDLR_INFO_T
 *  @brief   event handler info
 *  @code
 *  typedef struct _WGL_MBW_EVN_HDLR_INFO_T
 *  {
 *      UINT8                       ui1_strct_ver;
 *      UINT32                      ui4_evn_hdlr_mask; 
 *      VOID*                       pv_tag_evn_hdlr;
 *      wgl_mbw_event_hdlr_fct      pf_event_hdlr;       
 *  } WGL_MBW_EVN_HDLR_INFO_T;
 *  @endcode
  *  @li@c  ui1_strct_ver                                     - used for version control, must be 1
   *  @li@c  ui4_evn_hdlr_mask                                 - event handler mask
  *  @li@c  pv_tag_evn_hdlr                                   - attached data with the event handler
 *  @li@c  pf_event_hdlr                                     - specified event handler procedure callback function, if  ui4_evn_hdlr_mask is 0 then pf_event_hdlr can be NULL.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_MBW_EVN_HDLR_INFO_T
{
    UINT8                       ui1_strct_ver;
    
    UINT32                      ui4_evn_hdlr_mask; /* (WGL_MBW_EVN_HDLR_MASK_*) */
    
    VOID*                       pv_tag_evn_hdlr;
    
    wgl_mbw_event_hdlr_fct      pf_event_hdlr;       /* can be NULL if (ui4_evn_hdlr_mask == 0) */
    
} WGL_MBW_EVN_HDLR_INFO_T;


/* (*wgl_mbw_event_hdlr_fct)( e_evn_hdlr_type == WGL_MBW_EVN_HDLR_TYPE_NVGT_IN_VP,
                          ==> (WGL_MBW_EVN_HDLR_INFO_NVGT_IN_VP_T *) ui4_data ) */
/*------------------------------------------------------------------*/
/*! @struct  WGL_MBW_EVN_HDLR_INFO_NVGT_IN_VP_T
 *  @brief  This structure is used when wgl_mbw_event_hdlr_fct is called with event type is WGL_MBW_EVN_HDLR_TYPE_NVGT_IN_VP, the parameter ui4_data is set to WGL_MBW_EVN_HDLR_INFO_NVGT_IN_VP_T pointer
 *  @code
 *  typedef struct _WGL_MBW_EVN_HDLR_INFO_NVGT_IN_VP_T
 *  {
 *      UINT8                       ui1_strct_ver; 
 *      WGL_MBW_NVGT_INFO_T         t_nvgt_info;
 *  } WGL_MBW_EVN_HDLR_INFO_NVGT_IN_VP_T;
 *  @endcode
 *  @li@c  ui1_strct_ver                                     - used for version control, must be 1
 *  @li@c  t_nvgt_info                                       - specified navigation info for this event.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_MBW_EVN_HDLR_INFO_NVGT_IN_VP_T
{
    UINT8                       ui1_strct_ver;
    
    WGL_MBW_NVGT_INFO_T         t_nvgt_info;
    
} WGL_MBW_EVN_HDLR_INFO_NVGT_IN_VP_T;


/* (*wgl_mbw_evn_hdlr_fct)( e_evn_hdlr_type == WGL_MBW_EVN_HDLR_TYPE_NVGT_WITH_VP_CHG,
                          ==> (WGL_MBW_EVN_HDLR_INFO_NVGT_WITH_VP_CHG_T *) ui4_data ) */
/*------------------------------------------------------------------*/
/*! @struct WGL_MBW_EVN_HDLR_INFO_NVGT_WITH_VP_CHG_T
 *  @brief This structure is used when wgl_mbw_event_hdlr_fct is called with event type is WGL_MBW_EVN_HDLR_TYPE_NVGT_WITH_VP_CHG, the parameter ui4_data is set to WGL_MBW_EVN_HDLR_INFO_NVGT_WITH_VP_CHG_T pointer
 *  @code
 *  typedef struct _WGL_MBW_EVN_HDLR_INFO_NVGT_WITH_VP_CHG_T
 *  {
 *      UINT8                       ui1_strct_ver;
 *      WGL_MBW_NVGT_INFO_T         t_nvgt_info;
 *  } WGL_MBW_EVN_HDLR_INFO_NVGT_WITH_VP_CHG_T;
 *  @endcode
 *  @li@c  ui1_strct_ver                                     - used for version control, must be 1
 *  @li@c  t_nvgt_info                                       - specified navigation info for this event.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_MBW_EVN_HDLR_INFO_NVGT_WITH_VP_CHG_T
{
    UINT8                       ui1_strct_ver;
    
    WGL_MBW_NVGT_INFO_T         t_nvgt_info;
    
} WGL_MBW_EVN_HDLR_INFO_NVGT_WITH_VP_CHG_T;


/* (*wgl_mbw_event_hdlr_fct)( e_evn_hdlr_type == WGL_MBW_EVN_HDLR_TYPE_SELECT,
                          ==> (WGL_MBW_EVN_HDLR_INFO_SELECT_T *) ui4_data ) */
/*------------------------------------------------------------------*/
/*! @struct WGL_MBW_EVN_HDLR_INFO_SELECT_T
 *  @brief  This structure is used when wgl_mbw_event_hdlr_fct is called with event type is WGL_MBW_EVN_HDLR_TYPE_SELECT, the parameter ui4_data is set to WGL_MBW_EVN_HDLR_INFO_SELECT_T pointer
 *  @code
 *  typedef struct _WGL_MBW_EVN_HDLR_INFO_SELECT_T
 *  {
 *      UINT8       ui1_strct_ver;
 *      UINT32      ui4_idx_sel;
 *      UINT32      ui4_state_item_orig;
 *  } WGL_MBW_EVN_HDLR_INFO_SELECT_T;
 *  @endcode
 *  @li@c  ui1_strct_ver                                     - used for version control, must be 1
 *  @li@c  ui4_idx_sel                                       - specified selected item index.
 *  @li@c  ui4_state_item_orig                               - records item state before selected.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_MBW_EVN_HDLR_INFO_SELECT_T
{
    UINT8       ui1_strct_ver;
    
    UINT32      ui4_idx_sel;
    
    UINT32      ui4_state_item_orig;
    
} WGL_MBW_EVN_HDLR_INFO_SELECT_T;


/* (*wgl_mbw_event_hdlr_fct)( e_evn_hdlr_type == WGL_MBW_EVN_HDLR_TYPE_DESEL,
                          ==> (WGL_MBW_EVN_HDLR_INFO_DESEL_T *) ui4_data ) */
/*------------------------------------------------------------------*/
/*! @struct WGL_MBW_EVN_HDLR_INFO_DESEL_T
 *  @brief   This structure is used when wgl_mbw_event_hdlr_fct is called with event type is WGL_MBW_EVN_HDLR_TYPE_DESEL, the parameter ui4_data is set to WGL_MBW_EVN_HDLR_INFO_DESEL_T pointer
 *  @code
 *  typedef struct _WGL_MBW_EVN_HDLR_INFO_DESEL_T
 *  {
 *      UINT8       ui1_strct_ver;
 *      UINT32      ui4_idx_desel;
 *      UINT32      ui4_state_item_orig;
 *  } WGL_MBW_EVN_HDLR_INFO_DESEL_T;
 *  @endcode
 *  @li@c  ui1_strct_ver                                     -  used for version control, must be 1.
 *  @li@c  ui4_idx_desel                                     - specified deselected item index
 *  @li@c  ui4_state_item_orig                               - records item state before deselected.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_MBW_EVN_HDLR_INFO_DESEL_T
{
    UINT8       ui1_strct_ver;
    
    UINT32      ui4_idx_desel;
    
    UINT32      ui4_state_item_orig;
    
} WGL_MBW_EVN_HDLR_INFO_DESEL_T;

/* MBW container style */
/*------------------------------------------------------------------*/
/*! @struct WGL_MBW_CTNR_WGT_STYLE_T
 *  @brief
 *  @code
 *  typedef struct _WGL_MBW_CTNR_WGT_STYLE_T
 *  {
 *      UINT8             ui1_strct_ver;
 *      UINT32            ui4_cw_style_mbw;
 *      UINT32            ui4_cw_style_vp;
 *      UINT32            ui4_cw_style_item_ctnr;
 *      UINT32            ui4_cw_style_hlt_item_ctnr;
 *  } WGL_MBW_CTNR_WGT_STYLE_T;
 *  @endcode
 *  @li@c  ui1_strct_ver                                     - used for version control, must be 1
 *  @li@c  ui4_cw_style_mbw                                  - indicates mbw  UI style
 *  @li@c  ui4_cw_style_vp                                   - indicates view port UI style
 *  @li@c  ui4_cw_style_item_ctnr                            - indicates item container UI style
 *  @li@c  ui4_cw_style_hlt_item_ctnr                        -indicates highlight item container UI style
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_MBW_CTNR_WGT_STYLE_T
{
    UINT8             ui1_strct_ver;

    UINT32            ui4_cw_style_mbw;
    UINT32            ui4_cw_style_vp;
    UINT32            ui4_cw_style_item_ctnr;
    UINT32            ui4_cw_style_hlt_item_ctnr;

} WGL_MBW_CTNR_WGT_STYLE_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_MBW_INIT_T      
 *  @brief
 *  @code
 *  typedef struct _WGL_MBW_INIT_T
 *  {
 *      UINT8                           ui1_strct_ver;
 *      UINT32                          ui4_data; 
 *      UINT32                          ui4_style;
 *      UINT32                          ui4_count_row;
 *      UINT32                          ui4_count_col;
 *      UINT32                          ui4_hlt_margin;
 *      WGL_MBW_DATA_MODEL_INFO_T       t_data_model;
 *      WGL_MBW_KEY_HDLR_INFO_T         t_key_hdlr_info;
 *      WGL_MBW_EVN_HDLR_INFO_T         t_evn_hdlr_info;
 *      WGL_MBW_NVGT_CUSTOM_T           t_nvgt_custom;
 *      INT32                           i4_width_item;
 *      INT32                           i4_height_item;
 *      INT32                           i4_dist_horiz;
 *      INT32                           i4_dist_verti;
 *      WGL_INSET_T                     t_inset_vp;
 *      WGL_MBW_HLT_ITEM_INFO_T         t_hlt_item_info;
 *      WGL_MBW_CTNR_WGT_STYLE_T        t_mbw_ctnr_wgt_style;
 *  }WGL_MBW_INIT_T;
 *  @endcode
 *  @li@c  ui1_strct_ver                                     - used for version control, must be 1
 *  @li@c  ui4_data                                          - reserved for future
 *  @li@c  ui4_style                                         - mbw style
 *  @li@c  ui4_count_row                                     - mbw's row number
 *  @li@c  ui4_count_col                                     - mbw's col number          
 *  @li@c  ui4_hlt_margin                                    -  ui4_hlt_margin is valid only in SCRL_MODE, indicates the position to start scroll
 *  @li@c  t_data_model                                      - record mbw data model info
 *  @li@c  t_key_hdlr_info                                   - record mbw key handlerl info
 *  @li@c  t_evn_hdlr_info                                   - record mbw event handler info
 *  @li@c  t_nvgt_custom                                     - record mbw boundary move style
 *  @li@c  i4_width_item                                     - items' width
 *  @li@c  i4_height_item                                    - items' height
 *  @li@c  i4_dist_horiz                                     - horizontal distance between two item
 *  @li@c  i4_dist_verti                                     - vertical distance between two item
 *  @li@c  t_inset_vp                                        - inset of the viewport
 *  @li@c  t_hlt_item_info                                   - record mbw highlight item info
 *  @li@c  t_mbw_ctnr_wgt_style                              - record mbw and item container, highlight item container UI style
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_MBW_INIT_T
{
    UINT8                           ui1_strct_ver;
    
    UINT32                          ui4_data; /*reserved use*/

    /* ----- control logic parameters ----- */

    UINT32                          ui4_style;

    UINT32                          ui4_count_row;

    UINT32                          ui4_count_col;

    /* ui4_hlt_margin is valid only in SCRL_MODE, i.e. (!WGL_MBW_STL_PAGE_MODE) */
    UINT32                          ui4_hlt_margin;

    WGL_MBW_DATA_MODEL_INFO_T       t_data_model;

    WGL_MBW_KEY_HDLR_INFO_T         t_key_hdlr_info;

    WGL_MBW_EVN_HDLR_INFO_T         t_evn_hdlr_info;

    WGL_MBW_NVGT_CUSTOM_T           t_nvgt_custom;

    /* ----- content parameters ----- */
    INT32                           i4_width_item;

    INT32                           i4_height_item;

    INT32                           i4_dist_horiz;

    INT32                           i4_dist_verti;

    WGL_INSET_T                     t_inset_vp;

    WGL_MBW_HLT_ITEM_INFO_T         t_hlt_item_info;

    WGL_MBW_CTNR_WGT_STYLE_T        t_mbw_ctnr_wgt_style;

}WGL_MBW_INIT_T;




#ifdef __cplusplus
}
#endif

#endif

/*----------------------------------------------------------------------------*/
/*! @} */   /* end of groupMW_WGL_MATRIXBOX*/
/*----------------------------------------------------------------------------*/

