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
/*! @file u_wgl.h
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *
 *  @par Description:
 *         This header file contains the exported constant and type definitions
 *         of the Widget Library.
 */
/*----------------------------------------------------------------------------*/

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


#ifndef _U_WGL_H_
#define _U_WGL_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_handle.h"
#include "u_handle_grp.h"
#include "u_gl.h"
#include "u_img.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* Widget Library return values */



#define ARAB_EXIST_SUPPORT


#define WGLR_USER                           ((INT32)  128)    /**< starting point of the user-defined return value */

#define WGLR_ALREADY_DESTROYED              ((INT32)    2)    /**< this component has already been destroyed */
#define WGLR_ALREADY_REGISTERED             ((INT32)    1)    /**< this component has already been registered */

#define WGLR_OK                             ((INT32)    0)    /**< Indicates the operation requested has been accomplished successfully. */
#define WMPR_DONE                           WGLR_OK           /**< okay */

#define WGLR_INV_ARG                        ((INT32) -  1)    /**<Indicates at least one argument is invalid. */
#define WGLR_INV_HANDLE                     ((INT32) -  2)    /**<Indicates the specified handle is invalid. */
#define WGLR_INV_TYPE                       ((INT32) -  3)    /**<Indicates the specified type of widget, content or border is invalid.*/
#define WGLR_INV_CONTEXT                    ((INT32) -  4)    /**< Indicates the calling thread context is invalid.*/
#define WGLR_OUT_OF_MEMORY                  ((INT32) -  5)    /**< Indicates memory is exhausted and the requested operation cannot be accomplished.*/
#define WGLR_INV_MSG                        ((INT32) -  6)    /**<Indicates the message id is undefined. */
#define WGLR_INV_CMD                        ((INT32) -  7)    /**< Indicates the command id is invalid or undefined for some specific type of widget (or content and border). */
#define WGLR_INV_STATE                      ((INT32) -  8)    /**<Indicates the specified widget is not in a suitable state to perform the requested operation. */
#define WGLR_OS_ERROR                       ((INT32) -  9)    /**< Indicates the OSAI returned fail. */
#define WGLR_LOCK_ERROR                     ((INT32) - 10)    /**< Indicates locking the Widget Manager failed. */
#define WGLR_UNLOCK_ERROR                   ((INT32) - 11)    /**<Indicating unlocking the Widget Manager failed.*/
#define WGLR_NO_FOCUSED_WIDGET              ((INT32) - 12)    /**< Indicates there's no focused widget available. */
#define WGLR_INTERNAL_ERROR                 ((INT32) - 13)    /**< Indicates some internal error occurred.*/
#define WGLR_CANNOT_BE_FOCUSED              ((INT32) - 14)    /**< Indicates the specified widget cannot be focused. */
#define WGLR_EVENT_IGNORED                  ((INT32) - 15)    /**< Indicates the passing event (message) is ignored by the Widget Manager. */
#define WGLR_OUT_OF_HANDLES                 ((INT32) - 16)    /**< system is out of handle */
#define WGLR_SYS_RC_CREATE_FAIL             ((INT32) - 17)    /**< failed when creating system resource */
#define WGLR_SYS_RC_CANNOT_FREE             ((INT32) - 18)    /**< failed when freeing system resource */
#define WGLR_SYS_RC_NOT_INIT                ((INT32) - 19)    /**< resource is not initialized */
#define WGLR_NOT_SUPPORT_FORMAT             ((INT32) - 20)    /**< non-supported format */
#define WGLR_NOT_SUPPORT_TARGET             ((INT32) - 21)    /**< non-supported target */
#define WGLR_NOT_SUPPORT_RUNTIME_UPDATE     ((INT32) - 22)    /**< resource can not be updated in runtime */
#define WGLR_OVER_SUPPORT_LIMIT             ((INT32) - 23)    /**< it exceeds the supporting limit */
#define WGLR_NO_BORDER                      ((INT32) - 24)    /**< no border specified */
#define WGLR_NO_IMAGE                       ((INT32) - 25)    /**< no image specified */
#define WGLR_BUFFER_NOT_ENOUGH              ((INT32) - 26)    /**< buffer is not enough */
#define WGLR_GET_FONT_INFO_FAIL             ((INT32) - 27)    /**< failed when getting the font information */
#define WGLR_TEXT_SURF_CREATE_FAIL          ((INT32) - 28)    /**< failed when creating the surface for rendering a font */
#define WGLR_TEXT_DRAW_FAIL                 ((INT32) - 29)    /**< failed when drawing texts */
#define WGLR_IMG_DRAW_FAIL                  ((INT32) - 30)    /**< failed when drawing images */
#define WGLR_FUNC_NOT_IMPL                  ((INT32) - 31)    /**< this function is not implemented */
#define WGLR_INV_COLORMODE                  ((INT32) - 32)    /**< invalid color mode */
#define WGLR_OUT_OF_RESOURCE                ((INT32) - 33)    /**< out of resource */
#define WGLR_SHADOW_EXIST                   ((INT32) - 34)    /**< the shadow canvas already exists */
#define WGLR_ANIM_TYPE_CONFLICT             ((INT32) - 35)    /**< widget's animation type conflicts with running animations.
                                                                                                                     Conflict condition :
                                                                                                                     one of the widget's ancestor is doing animation which has style: WGL_STL_GL_ANIM_AFFECT_CHILD
                                                                                                                       then the widget can not do animation at the same time or vice versa*/
#define WGLR_ANIM_TYPE_NOT_SUPPORT          ((INT32) - 36)    /**< widget's animation type is not supported in system, path library method and WGL_ANIM_TYPE_SIZE and WGL_ANIM_TYPE_BREATHE are not supported at present*/
#define WGLR_ANIM_ALREADY_RUNNING           ((INT32) - 37)    /**< widget's animation is running so can not start the new animation for the widget before the running animation is ended*/
#define WGLR_KEY_MSG_NOT_PROCESSED          ((INT32) - 38)    /**< widget dose not process the msg*/
#define WGLR_OUT_OF_CACHED                  ((INT32) - 39)    /**< out of cached buffer*/
#define WGLR_CANVAS_HAS_SHADOW              ((INT32) - 40)    /**< the canvas is reference canvas and its shadow canvas has not been destroyed*/
#define WGLR_CANVAS_IS_USEING               ((INT32) - 41)    /**< there exists some apps who are using the canvas at the time,so the canvas can not be destroyed*/

#define WGLR_INIT_FAILED                    ((INT32) - 100)   /**< failed when initializing */

#define WGLR_WIDGET_SPF_START               ((INT32) - 1000)  /**< starting point of the user-defined error return value */

#define WGL_FAILED(ret)             ((ret) < WGLR_OK)                               /**< macro for testing the success of WGL return value */

#define HT_WGL_CLIENT               (HT_GROUP_WGL + ((HANDLE_TYPE_T)  0))           /**< This definition is the type id of a client handle of Widget Library. A client of Widget Library can be a customizable application, such as navigator or EPG. It can also be an internal module of Middleware, such as Close Caption handler. */
#define HT_WGL_CANVAS               (HT_GROUP_WGL + ((HANDLE_TYPE_T)  1))           /**< This definition is the type id used to specify a canvas. A canvas is where the widgets are placed. It is the root container of all top level widgets. */
#define HT_WGL_TIMER                (HT_GROUP_WGL + ((HANDLE_TYPE_T)  2))           /**< specify the timer WGL type */
#define HT_WGL_TRANP_LIST           (HT_GROUP_WGL + ((HANDLE_TYPE_T)  3))           /**< specify the transparent list WGL type */
#define HT_WGL_ANIM_HANDLE          (HT_GROUP_WGL + ((HANDLE_TYPE_T)  4))           /**< specify the transparent list WGL type */

/* widget types */
#define HT_WGL_WIDGET_START         (HT_GROUP_WGL + ((HANDLE_TYPE_T)  5))           /**< This marks the start of all widget type definitions.*/

#define HT_WGL_WIDGET_FRAME         (HT_WGL_WIDGET_START)                           /**<This definition is the type id used to specify a Frame widget. A frame widget is no more than a container to keep other widgets.*/
#define HT_WGL_WIDGET_ICON          (HT_WGL_WIDGET_START + ((HANDLE_TYPE_T)  1))    /**< This definition is the type id used to specify an Icon widget. An icon widget represents a small piece of image.*/
#define HT_WGL_WIDGET_TEXT          (HT_WGL_WIDGET_START + ((HANDLE_TYPE_T)  2))    /**<This definition is the type id used to specify a Text widget. A Text widget is used to display static read-only textual and/or graphical content.*/
#define HT_WGL_WIDGET_BUTTON        (HT_WGL_WIDGET_START + ((HANDLE_TYPE_T)  3))    /**< This definition is the type id used to specify a Button widget. A Button widget represents as a push-button with textual or graphical content.*/
#define HT_WGL_WIDGET_PROGRESS_BAR  (HT_WGL_WIDGET_START + ((HANDLE_TYPE_T)  4))    /**< This definition is the type id used to specify a ProgressBar widget. A ProgressBar widget is used to display a static value which is within a fixed range. */
#define HT_WGL_WIDGET_EDIT_BOX      (HT_WGL_WIDGET_START + ((HANDLE_TYPE_T)  5))    /**< This definition is the type id used to specify an EditBox widget. An EditBox widget is used to receive a single line of alphanumeric entry from the user and can also be used for password input. */
#define HT_WGL_WIDGET_LIST_BOX      (HT_WGL_WIDGET_START + ((HANDLE_TYPE_T)  6))    /**< This definition is the type id used to specify a ListBox widget. A ListBox widget is a user interface component representing a list of selectable items which contain graphical and/or textual content.*/
#define HT_WGL_WIDGET_CC            (HT_WGL_WIDGET_START + ((HANDLE_TYPE_T)  7))    /**< This definition is the type id used to specify a CC widget. A CC widget is specialized for use of Close Caption, including DTVCC and NTSCCC.*/
#define HT_WGL_WIDGET_TKTP          (HT_WGL_WIDGET_START + ((HANDLE_TYPE_T)  8))    /**< tktp widget */
#define HT_WGL_WIDGET_HTS           (HT_WGL_WIDGET_START + ((HANDLE_TYPE_T)  9))    /**< This definition is the type id used to specify a HyperText widget. A HyperText widget is used to display textual content with rich format, such as font, text layout, color, animation, etc.*/
#define HT_WGL_WIDGET_SBTL          (HT_WGL_WIDGET_START + ((HANDLE_TYPE_T)  10))   /**< subtitle widget */
#define HT_WGL_WIDGET_TTX           (HT_WGL_WIDGET_START + ((HANDLE_TYPE_T)  11))   /**< teletext widget */
#define HT_WGL_WIDGET_ANIM          (HT_WGL_WIDGET_START + ((HANDLE_TYPE_T)  12))   /**< animation widget */
#define HT_WGL_WIDGET_GROUP         (HT_WGL_WIDGET_START + ((HANDLE_TYPE_T)  13))   /**< group widget */
#define HT_WGL_WIDGET_CC_ISDB       (HT_WGL_WIDGET_START + ((HANDLE_TYPE_T)  14))   /**< ISDB CC widget */

#define HT_WGL_WIDGET_END           (HT_WGL_WIDGET_START + ((HANDLE_TYPE_T)  15))   /**< This marks the end of all widget type definitions.*/

#define HT_WGL_IMG_TPL              (HT_GROUP_WGL + HT_GROUP_SIZE)                  /**< This definition is the type id of used to specify a image template.  */

#define IS_WIDGET_TYPE(e_type)      (((HANDLE_TYPE_T) (e_type) >= HT_WGL_WIDGET_START) && ((HANDLE_TYPE_T) (e_type) < HT_WGL_WIDGET_END))    /**< check if this widget is a valid widget type */

#define WGL_MAKE_PARAM(_x)          ((VOID*) (_x))                    /**< cast the input argument as the desired one */

/* widget state definition */
#define WGL_STATE_ENABLED           ((UINT16) 0x0001)                 /**< Indicates the specified widget is enabled. */
#define WGL_STATE_VISIBLE           ((UINT16) 0x0002)                 /**< Indicates the specified widget is visible. */
#define WGL_STATE_ACTIVATED         ((UINT16) 0x0004)                 /**< Indicates the specified widget is activated. */
#define WGL_STATE_FOCUSED           ((UINT16) 0x0008)                 /**< Indicates the specified widget has the focus. */

/* key state definition */
#define WGL_KEY_STATE_DOWN          ((UINT32) 0)                      /**< key down state */
#define WGL_KEY_STATE_UP            ((UINT32) 1)                      /**< key up state */
#define WGL_KEY_STATE_REPEAT        ((UINT32) 2)                      /**< key repeat */

/* maximum key link entries */
#define WGL_MAX_KEY_LINK            ((UINT32) 6)                      /**< maximum value of the key link number */

#define WGL_NO_KEY_SKIP             ((UINT32) 0xffffffff)             /**< specifies that there is no key skipping */

/*widget message*/
#define WGL_MSG_SYSTEM              ((UINT32) 0x0100)                 /**< mask for system message */
#define WGL_MSG_USER                ((UINT32) 0x8000)                 /**< mask for user message */

 /**
 The WGL_MSG_PAINT message is sent to the widget's message procedure whenever it is necessary to update the appearance of the widget on the screen. This might be induced due to an explicitly call of x_wgl_repaint() or because that overlapping with the update region is detected.

 Input arguments

 h_widget:    Contains the handle of the widget being updated.

 ui4_msg : WGL_MSG_PAINT

 pv_param1:   Contains the handle of type GL_HGRAPHICS_T prepared by the Widget Manager. This handle can be used for performing graphics operations. See the document SW Design Middleware-Graphics Library.

 pv_param2 :  Contains the combination mask of various state flags.

 Output arguments None

 */
#define WGL_MSG_PAINT               ((UINT32) WGL_MSG_SYSTEM +  0)
/**
The WGL_MSG_GET_FOCUS message is sent to the widget's message procedure when the specified widget gets the input focus.

Input arguments

h_widget:	Contains the handle of the widget getting the focus.

ui4_msg:	WGL_MSG_GET_FOCUS

pv_param1:	Contains TRUE if the owner application of h_widget is currently active. Otherwise this value is FALSE.

pv_param2:	Not used.

*/
#define WGL_MSG_GET_FOCUS           ((UINT32) WGL_MSG_SYSTEM +  1)
/**
The WGL_MSG_LOSE_FOCUS message is sent to the widget's message procedure when the specified widget lost the input focus.

Input arguments

h_widget:	Contains the handle of the widget lost the focus.

ui4_msg:	WGL_MSG_LOSE_FOCUS

pv_param1:	Contains TRUE if the owner application of h_widget is currently active. Otherwise this value is FALSE.

pv_param2:	Not used.

*/
#define WGL_MSG_LOSE_FOCUS          ((UINT32) WGL_MSG_SYSTEM +  2)
/**
The WGL_MSG_KEY_DOWN message is sent to the widget's message procedure when a key down event is caught by the IO Manager. The owner application of the widget should be in the activated state and the widget is the current focused widget.

Input arguments

h_widget:	Contains the handle of the current focused widget.

ui4_msg:	WGL_MSG_KEY_DOWN

pv_param1:	Contains the actual key code.

pv_param2:	Not used.
*/
#define WGL_MSG_KEY_DOWN            ((UINT32) WGL_MSG_SYSTEM +  3)
/**
The WGL_MSG_KEY_UP message is sent to the widget's message procedure when a key up event is caught by the IO Manager. The owner application of the widget should be in the activated state and the widget is the current focused widget.

Input arguments

h_widget:     Contains the handle of the current focused widget.

ui4_msg:	WGL_MSG_KEY_UP

pv_param1:	Contains the actual key code.

pv_param2	Not used.
*/
#define WGL_MSG_KEY_UP              ((UINT32) WGL_MSG_SYSTEM +  4)
/**
The WGL_MSG_KEY_REPEAT message is sent to the widget's message procedure when a key repeat event is caught by the IO Manager. The owner application of the widget should be in the activated state and the widget is the current focused widget.

Input arguments

h_widget:	Contains the handle of the current focused widget.

ui4_msg:	WGL_MSG_KEY_REPEAT

pv_param1:	Contains the actual key code.

pv_param2:	Not used.
*/
#define WGL_MSG_KEY_REPEAT          ((UINT32) WGL_MSG_SYSTEM +  5)
/**
The WGL_MSG_ACTIVATE message is sent to the top-level widget's message procedure whenever the focus changes. If the widget losing focus and the widget getting focus have different top-level ancestors, their top-level ancestor widgets will both receive this message.

The WGL_MSG_ACTIVATE message is sent to both the widget being activated and the widget being deactivated. Use the argument pv_param1 to determine either the widget is being activated or deactivated.

Input arguments

h_widget:	Contains the handle of the widget being activated/deactivated.

ui4_msg:	WGL_MSG_ACTIVATE

pv_param1:	Contains TRUE if h_widget is activated, or FALSE if deactivated.

pv_param2:	Contains TRUE if the owner application of h_widget is currently active. Otherwise this value is FALSE.
*/
#define WGL_MSG_ACTIVATE            ((UINT32) WGL_MSG_SYSTEM +  6)
/**
The WGL_MSG_MOVE message is sent to the widget's message procedure after the position of the widget is changed.

Input arguments

h_widget:	Contains the handle of the widget.

ui4_msg:	WGL_MSG_MOVE

pv_param1:	Contains the new position of the widget. The high order 16 bits contains the x coordinate and the low order 16 bits contain the y coordinate. The coordinates are specified in the parent's local coordinate system.

pv_param2:	Not used.
*/
#define WGL_MSG_MOVE                ((UINT32) WGL_MSG_SYSTEM +  7)
/**
The WGL_MSG_SIZE message is sent to the widget's message procedure after the dimension of the widget is changed.

Input arguments

h_widget:	Contains the handle of the widget.

ui4_msg:	WGL_MSG_SIZE

pv_param1:	Contains the new size of the widget. The high order 16 bits contains width and the low order 16 bits contain height.

pv_param2:	Not used.
*/
#define WGL_MSG_SIZE                ((UINT32) WGL_MSG_SYSTEM +  8)
/**
The WGL_MSG_ENABLE message is sent to the widget's message procedure when the widget is being enabled/disabled.

Input arguments

h_widget:	Contains the handle of the widget.

ui4_msg:	WGL_MSG_ENABLE

pv_param1:	Contains a Boolean value indicating enabling or disabling the widget. TRUE means enabling and FALSE means disabling.

pv_param2:	Not used.
*/
#define WGL_MSG_ENABLE              ((UINT32) WGL_MSG_SYSTEM +  9)
/**
The WGL_MSG_NOTIFY message is sent to the widget's message procedure when it is notified by one of its child widgets.

Input arguments

h_widget:	Contains the handle of the widget.

ui4_msg:	WGL_MSG_NOTIFY

pv_param1:	References a WGL_NOTIFY_DATA_T structure which contains the relevant information processing this notification. Note that this structure is prepared by the caller and might have a larger size than WGL_NOTIFY_DATA_T. The message procedure should cast this pointer to the type expected.

pv_param2:  	Not used
*/
#define WGL_MSG_NOTIFY              ((UINT32) WGL_MSG_SYSTEM + 10)
/**
The WGL_MSG_COMMAND message is sent to the widget's message procedure while clients call x_wgl_do_cmd() on the specified widget.

Input arguments

h_widget:	Contains the handle of the specified widget.

ui4_msg:	WGL_MSG_COMMAND

pv_param1:	Contains the command id. This is detailed in the individual widget, content or border design spec.

pv_param2:    References a WGL_PACKED_PARAM_T structure. The two fields of this structure contain the third and fourth arguments while c_wgl_do_cmd() is called. The actual meaning of those fields are detailed in the individual widget, content or border design spec.
*/
#define WGL_MSG_COMMAND             ((UINT32) WGL_MSG_SYSTEM + 11)
/**
The WGL_MSG_CREATE message is sent to the widget's message procedure after the widget instance has been created.

Input arguments

h_widget:	Contains the handle of the widget.

ui4_msg:	WGL_MSG_CREATE

pv_param1:	References a WGL_CREATE_INFO_T that contains information for widget creation.

pv_param2:	Not used.
*/
#define WGL_MSG_CREATE              ((UINT32) WGL_MSG_SYSTEM + 12)
/**
The WGL_MSG_DESTROY message is sent to the widget's message procedure before the widget instance is destroyed.

Input arguments

h_widget:	Contains the handle of the widget.

ui4_msg:	WGL_MSG_DESTROY

pv_param1:	Not used.

pv_param2:	Not used.
*/
#define WGL_MSG_DESTROY             ((UINT32) WGL_MSG_SYSTEM + 13)
/**
The WGL_MSG_RESIZE_CANVAS message is sent to the canvas's callback function:(*wgl_canvas_callback_fct)() after the canvas is resized.

Input arguments

h_canvas:	Contains the handle of the canvas

ui4_msg:	WGL_MSG_RESIZE_CANVAS

pv_param1:	((UINT32)pv_param1) >> 16 is canvas's new width,((UINT32)pv_param1) & 0x0000FFFF is  canvas' new height.

pv_param2:	Not used.
*/
#define WGL_MSG_RESIZE_CANVAS       ((UINT32) WGL_MSG_SYSTEM + 14)
/**
The WGL_MSG_SWITCH_CANVAS message is sent to the canvas's callback function:(*wgl_canvas_callback_fct)() after the canvas and its shadow switched showing.

Input arguments

h_canvas:	Contains the handle of the canvas

ui4_msg:	WGL_MSG_SWITCH_CANVAS

pv_param1:	BOOL value indicates whether h_canvas is showing

pv_param2:	BOOL value indicates whether there exists any visible widget on the canvas..
*/

#define WGL_MSG_SWITCH_CANVAS       ((UINT32) WGL_MSG_SYSTEM + 15)

/**
The WGL_MSG_UPDATE_CANVAS message is sent to the canvas's callback function:(*wgl_canvas_callback_fct)() after the canvas's state updated.

Input arguments

h_canvas:	Contains the handle of the canvas

ui4_msg:	WGL_MSG_UPDATE_CANVAS

pv_param1:	BOOL value indicates whether h_canvas is showing

pv_param2:	BOOL value indicates whether there exists any visible widget on the canvas..
*/
#define WGL_MSG_UPDATE_CANVAS       ((UINT32) WGL_MSG_SYSTEM + 16)
/**
The WGL_MSG_TIMER message is sent to the widget's message procedure after one  timer tick is reached.

Input arguments

h_widget:	Contains the handle of the widget.

ui4_msg:	WGL_MSG_TIMER

pv_param1:	HANDLE_T value, time hander

pv_param2:	VOID* value, references the private data provided by application. Note that this field contains the same value with the pv_tag argument while c_wgl_start_timer() is called.
*/
#define WGL_MSG_TIMER               ((UINT32) WGL_MSG_SYSTEM + 17)
/**
The WGL_MSG_CHANGE_COLOR_CFG message is sent to the widget's message procedure after related canvas's configuration changed.

Input arguments

h_widget:	Contains the handle of the widget.

ui4_msg:	WGL_MSG_CHANGE_COLOR_CFG

pv_param1:	WGL_CVS_CLR_CFG_T* value, canvas configuration value

pv_param2:	Reserved, must be NULL
*/
#define WGL_MSG_CHANGE_COLOR_CFG    ((UINT32) WGL_MSG_SYSTEM + 18)
/**
The WGL_MSG_ANIMATION_TICK message is sent to the widget's message procedure after one animation timer tick is reached.

Input arguments

h_widget:	Contains the handle of the widget.

ui4_msg:	WGL_MSG_ANIMATION_TICK

pv_param1:	Reserved, must be NULL

pv_param2:	UINT32 value, indicates the animation timer tick number which is counted when c_wgl_begin_animation() is called.
*/
#define WGL_MSG_ANIMATION_TICK      ((UINT32) WGL_MSG_SYSTEM + 19)
/**
The WGL_MSG_NOTIFY_ANIM_COND message is sent to the widget's message procedure after one animation timer tick is reached.

Input arguments

h_widget:	Contains the handle of the widget.

ui4_msg:	WGL_MSG_NOTIFY_ANIM_COND

pv_param1:	Reserved, must be NULL

pv_param2:	UINT32 value, indicates the animation timer tick number which is counted when c_wgl_begin_animation() is called.
*/
#define WGL_MSG_NOTIFY_ANIM_COND    ((UINT32) WGL_MSG_SYSTEM + 20)

/* definition of general notify code */
#define WGL_NC_GL_KEY_DOWN      ((UINT32) 1)                          /**< key down notification */
#define WGL_NC_GL_KEY_UP        ((UINT32) 2)                          /**< key up notification */
#define WGL_NC_GL_KEY_REPEAT    ((UINT32) 3)                          /**< key repeat notification */
#define WGL_NC_GL_LOSE_FOCUS    ((UINT32) 4)                          /**< lose focus notification */
#define WGL_NC_GL_GET_FOCUS     ((UINT32) 5)                          /**< get focus notification */
#define WGL_NC_WIDGET_SPF_START ((UINT32) WGL_NC_GL_GET_FOCUS + 1)    /**<        */

#define WGL_NO_AUTO_REPAINT     ((UINT8) 0)                           /**< Indicates the automatic repainting is disabled. */
#define WGL_SYNC_AUTO_REPAINT   ((UINT8) 1)                           /**< Indicates the automatic repainting is performed synchronously.*/
#define WGL_ASYNC_AUTO_REPAINT  ((UINT8) 2)                           /**<Indicates the automatic repainting is performed asynchronously. */

/* Applicatino Z order constant definitions */
#define WGL_APP_NOT_TO_END      ((UINT8) 0x0)                         /**< not to the end z-order definition */
#define WGL_APP_TOPMOST         ((UINT8) 0x1)                         /**< top-most z-order definition */
#define WGL_APP_BOTTOMMOST      ((UINT8) 0x2)                         /**< bottom-most z-order definintion */

/* Canvas configuration flags */
#define WGL_CANVAS_CFG_WIDTH                	((UINT32) 0x0001)         /**< canvas width configuration */
#define WGL_CANVAS_CFG_HEIGHT               	((UINT32) 0x0002)         /**< canvas height configuration */
#define WGL_CANVAS_CFG_COLORMODE            	((UINT32) 0x0004)         /**< canvas color mode configuration */
#define WGL_CANVAS_CFG_PALETTE              	((UINT32) 0x0008)         /**< canvas palette configuration */
#define WGL_CANVAS_CFG_AUTOSCALE            	((UINT32) 0x0010)         /**< canvas auto scale configuration */
#define WGL_CANVAS_CFG_COLORKEY             	((UINT32) 0x0020)         /**< canvas colorkey configuration */
#define WGL_CANVAS_CFG_CHG_PLT_NO_CLEAN     	((UINT32) 0x0040)         /**< canvas would not be updated when changing the palette table */
#define WGL_CANVAS_CFG_FORCE_NO_INTER_COMPOSE	((UINT32) 0x0080)         /**< force the canvas as a double-buffered one */
#define WGL_CANVAS_CFG_ALL                  	((UINT32) 0xffff)         /**< full canvas configuration mask */

/*------------------------------------------------------------------*/
/*! @brief  The application callback, as its name implies, is an application-defined callback function. This procedure must be specified when registering the application to the Widget Manager. It is called whenever events from input devices, for instance the remote controller, are sent to the Widget Manager. Only the callback of the active application is invoked. Note that this callback is executed in the context of Middleware module. If applications need to perform time-consuming tasks, it's suggested the message be routed to the application's own thread
 *  @param  [in]    ui4_msg         - Contains  the message id.
 *  @param  [in]    pv_param1       - Specifies the additional information. The actual content of this argument depends on ui4_msg.
 *  @param  [in]    pv_param2       - Specifies more additional information. The actual content of this argument depends on ui4_msg.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK			The function call is successful.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wgl_app_callback_fct)(
                UINT32              ui4_msg,
                VOID*               pv_param1,
                VOID*               pv_param2);

/*------------------------------------------------------------------*/
/*! @brief  The application callback, as its name implies, is an application-defined callback function. This procedure must be specified when registering the application to the Widget Manager. It is called whenever key events from APP's message queue are sent to the Widget Manager. Note that this callback is executed in the context of active's application.
 *  @param  [in]    h_thread        - Specifies the handle of active application's thread context.
 *  @param  [in]    ui4_msg         - Contains  the message id,maybe WGL_MSG_KEY_DOWN,WGL_MSG_KEY_REPEAT or WGL_MSG_KEY_UP.
 *  @param  [in]    ui4_key_code  - Specifies key code.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval     WGLR_OK			The function call is successful.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wgl_widget_callback_fct)(
                HANDLE_T            h_thread,
                UINT32              ui4_msg,
                UINT32              ui4_key_code);

/*------------------------------------------------------------------*/
/*! @brief  callback function for canvas status update
 *  @param  [in]    ui4_msg         - notification message
 *  @param  [in]    h_canvas        - canvas handle
 *  @param  [in]    pv_param1       - parameter 1
 *  @param  [in]    pv_param2       - parameter 2
 *  @note
 *  @see
 *  @return return the error code
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wgl_canvas_callback_fct)(
                UINT32              ui4_msg,
                HANDLE_T            h_canvas,
                VOID*               pv_param1,
                VOID*               pv_param2);

/*------------------------------------------------------------------*/
/*! @brief  canvas information callback
 *  @note
 *  @see
 *  @return return the information
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef UINT8 (*wgl_canvas_info_callback_fct)(VOID);

/*------------------------------------------------------------------*/
/*! @brief  The widget message procedure is an application-defined callback function. This procedure processes messages sent to the specified widget. In most cases, this callback function is provided by each individual widget. Under the circumstances the application intends to customize the behavior of the widget, the default implementation of the message procedure can be replaced and the messages for the specified widget will be routed to the user-defined one.
 *  @param  [in]    h_widget        - Contains the handle to the specified widget.
 *  @param  [in]    ui4_msg         -Contains  the message id.
 *  @param  [in]    pv_param1       - Specifies the additional information. The actual content of this argument depends on ui4_msg.
 *  @param  [in]    pv_param2       - Specifies more additional information. The actual content of this argument depends on ui4_msg.
 *  @note
 *  @see
 *  @return return the error code
 *  @retval      WGLR_OK			The function call is successful.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wgl_widget_proc_fct)(
                HANDLE_T            h_widget,
                UINT32              ui4_msg,
                VOID*               pv_param1,
                VOID*               pv_param2);

/*------------------------------------------------------------------*/
/*! @enum WGL_SW_CMD_T
 *  @brief  display option
 *  @code
 *  typedef enum _WGL_SW_CMD_T
 *  {
 *      WGL_SW_NORMAL,
 *      WGL_SW_RECURSIVE,
 *      WGL_SW_HIDE,
 *      WGL_SW_HIDE_RECURSIVE
 *  } WGL_SW_CMD_T;
 *  @endcode
 *  @li@c  WGL_SW_NORMAL                                     - Indicates the widget should be changed to the visible state. Note that only the state of the specified widget is changed.
 *  @li@c  WGL_SW_RECURSIVE                                  - Indicates the widget and all its child widgets should be changed to the visible state.
 *  @li@c  WGL_SW_HIDE                                       - Indicates the widget should be hidden.Note that only the state of the specified widget is changed.
 *  @li@c  WGL_SW_HIDE_RECURSIVE                             -Indicates the widget and all its child widgets should be hidden.
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_SW_CMD_T
{
    WGL_SW_NORMAL,
    WGL_SW_RECURSIVE,
    WGL_SW_HIDE,
    WGL_SW_HIDE_RECURSIVE
} WGL_SW_CMD_T;

/*------------------------------------------------------------------*/
/*! @enum WGL_COORDS_T
 *  @brief  specifies the coordinate system
 *  @code
 *  typedef enum _WGL_COORDS_T
 *  {
 *      WGL_COORDS_LOCAL,
 *      WGL_COORDS_PARENT,
 *      WGL_COORDS_SCREEN
 *  } WGL_COORDS_T;
 *  @endcode
 *  @li@c  WGL_COORDS_LOCAL                                  - Indicates the coordinates returned are in the local coordinate system of the specified widget.
 *  @li@c  WGL_COORDS_PARENT                                 - Indicates the coordinates returned are in the coordinate system of the parent widget.
 *  @li@c  WGL_COORDS_SCREEN                                 - Indicates the coordinates returned are in the system coordinate system with (0, 0) at the top-left corner of screen.
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_COORDS_T
{
    WGL_COORDS_LOCAL,
    WGL_COORDS_PARENT,
    WGL_COORDS_SCREEN
} WGL_COORDS_T;

/*------------------------------------------------------------------*/
/*! @enum WGL_BG_MAP_MODE_T
 *  @brief  set the background map mode
 *  @code
 *  typedef enum _WGL_BG_MAP_MODE_T
 *  {
 *      WGL_BG_MAP_CENTERED,
 *      WGL_BG_MAP_TILED,
 *      WGL_BG_MAP_STRETCHED
 *  } WGL_BG_MAP_MODE_T;
 *  @endcode
 *  @li@c  WGL_BG_MAP_CENTERED                               - centered background
 *  @li@c  WGL_BG_MAP_TILED                                  - tiled background
 *  @li@c  WGL_BG_MAP_STRETCHED                              - stretched background
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_BG_MAP_MODE_T
{
    WGL_BG_MAP_CENTERED,
    WGL_BG_MAP_TILED,
    WGL_BG_MAP_STRETCHED
} WGL_BG_MAP_MODE_T;

/*------------------------------------------------------------------*/
/*! @enum WGL_INSERT_MODE_T
 *  @brief  specifies the widget order option
 *  @code
 *  typedef enum _WGL_INSERT_MODE_T
 *  {
 *      WGL_INSERT_BEFORE = 0,
 *      WGL_INSERT_AFTER,
 *      WGL_INSERT_TOPMOST,
 *      WGL_INSERT_BOTTOMMOST
 *  } WGL_INSERT_MODE_T;
 *  @endcode
 *  @li@c  WGL_INSERT_BEFORE                                 - Indicates the specified widget should be inserted on top of the reference widget and thus has a smaller z order.
 *  @li@c  WGL_INSERT_AFTER                                  - Indicates the specified widget should be inserted on bottom of the reference widget and thus has a larger z order.
 *  @li@c  WGL_INSERT_TOPMOST                                - Indicates the specified widget should be inserted on bottom of all other widgets and thus has the largest z order.
 *  @li@c  WGL_INSERT_BOTTOMMOST                             - Indicates the specified widget should be inserted on top of all other widgets and thus has the smallest z order.
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_INSERT_MODE_T
{
    WGL_INSERT_BEFORE = 0,
    WGL_INSERT_AFTER,
    WGL_INSERT_TOPMOST,
    WGL_INSERT_BOTTOMMOST
} WGL_INSERT_MODE_T;

/*------------------------------------------------------------------*/
/*! @enum WGL_TICK_MODE_T
 *  @brief  specifies the animation tick type
 *  @code
 *  typedef enum _WGL_TICK_MODE_T
 *  {
 *      WGL_TICK_UNKNOWN,
 *      WGL_TICK_CONTINUOUS,
 *      WGL_TICK_ONE_SHOT,
 *      WGL_TICK_ONE_SHOT_DROP_FRAME
 *  } WGL_TICK_MODE_T;
 *  @endcode
 *  @li@c  WGL_TICK_UNKNOWN                                  - unknown type
 *  @li@c  WGL_TICK_CONTINUOUS                               - continuous tick message will send to animation widget even though the previous tick message has not been processed by the animation widget.
 *  @li@c  WGL_TICK_ONE_SHOT                                 - new  tick message will be sent to animation widget until
 *                                                                                 the previous tick message has been processed and the new tick message using continuous tick index based on the previous one
 *  @li@c  WGL_TICK_ONE_SHOT_DROP_FRAME                      - new  tick message will be sent to animation widget until
 *                                                                                 the previous tick message has been processed and the new tick message using tick index not based on the previous one
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_TICK_MODE_T
{
    WGL_TICK_UNKNOWN,
    WGL_TICK_CONTINUOUS,
    WGL_TICK_ONE_SHOT,
    WGL_TICK_ONE_SHOT_DROP_FRAME
} WGL_TICK_MODE_T;

/* Timer setting */
/*------------------------------------------------------------------*/
/*! @enum WGL_TIMER_SYNC_MODE_T
 *  @brief  specifies the timer mode that widget will receive timer message:WGL_MSG_TIMER
 *  @code
 *  typedef enum _WGL_TIMER_SYNC_MODE_T
 *  {
 *      WGL_TIMER_SYNC = 0,
 *      WGL_TIMER_ASYNC
 *  } WGL_TIMER_SYNC_MODE_T;
 *  @endcode
 *  @li@c  WGL_TIMER_SYNC                                    - timer message will send to widget's message procedure function in synchronous mode(application's thread context).
 *  @li@c  WGL_TIMER_ASYNC                                   - timer message will send to widget's message procedure function in asynchronous mode(widget manager's thread context).
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_TIMER_SYNC_MODE_T
{
    WGL_TIMER_SYNC = 0,
    WGL_TIMER_ASYNC
} WGL_TIMER_SYNC_MODE_T;

/*------------------------------------------------------------------*/
/*! @enum WGL_ANIM_CONTEXT_T
 *  @brief  specifies which context to render the animation
 *  @code
 *  typedef enum _WGL_ANIM_CONTEXT_T
 *  {
 *      ANIM_CONTEXT_UNKNOWN,
 *      ANIM_CONTEXT_APP,
 *      ANIM_CONTEXT_APP_SYNC,
 *      ANIM_CONTEXT_WM
 *  } WGL_ANIM_CONTEXT_T;
 *  @endcode
 *  @li@c  ANIM_CONTEXT_UNKNOWN                              - unknown context type
 *  @li@c  ANIM_CONTEXT_APP                                  - APP context with asynchronously tick-dispatching manner
 *  @li@c  ANIM_CONTEXT_APP_SYNC                             - APP context with synchronous manner
 *  @li@c  ANIM_CONTEXT_WM                                   - WM context
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_ANIM_CONTEXT_T
{
    ANIM_CONTEXT_UNKNOWN,
    ANIM_CONTEXT_APP,
    ANIM_CONTEXT_APP_SYNC,
    ANIM_CONTEXT_WM
} WGL_ANIM_CONTEXT_T;

/*------------------------------------------------------------------*/
/*! @enum WGL_CVS_CB_TYPE_T
 *  @brief  specifies the canvas callback type
 *  @code
 *  typedef enum _WGL_CVS_CB_TYPE_T
 *  {
 *      WGL_CVS_CB_UNKNOWN = 0,
 *      WGL_CVS_CB_TXT_DIR
 *  } WGL_CVS_CB_TYPE_T;
 *  @endcode
 *  @li@c  WGL_CVS_CB_UNKNOWN                                - unknown
 *  @li@c  WGL_CVS_CB_TXT_DIR                                - callback for text direction
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_CVS_CB_TYPE_T
{
    WGL_CVS_CB_UNKNOWN = 0,
    WGL_CVS_CB_TXT_DIR
} WGL_CVS_CB_TYPE_T;

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*! @struct WGL_ANIM_CONFIG_T
 *  @brief  specifies the animation configuration
 *  @code
 *  typedef struct _WGL_ANIM_CONFIG_T
 *  {
 *      UINT32              ui4_tick_interval;
 *      WGL_TICK_MODE_T     e_tick_mode;
 *  } WGL_ANIM_CONFIG_T;
 *  @endcode
 *  @li@c  ui4_tick_interval                                 - tick interval indicated how many milliseconds between two animaiton ticks.
 *  @li@c  e_tick_mode                                       - tick mode
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_ANIM_CONFIG_T
{
    UINT32              ui4_tick_interval;
    WGL_TICK_MODE_T     e_tick_mode;
} WGL_ANIM_CONFIG_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_CANVAS_CFG_T
 *  @brief  specifies the canvas configuration
 *  @code
 *  typedef struct _WGL_CANVAS_CFG_T
 *  {
 *      UINT32          ui4_flag;
 *      UINT32          ui4_width;
 *      UINT32          ui4_height;
 *      BOOL            b_auto_scale;
 *      GL_COLORMODE_T  e_clr_mode;
 *      GL_COLOR_T      t_colorkey;
 *      GL_HPALETTE_T   h_palette;
 *  } WGL_CANVAS_CFG_T;
 *  @endcode
 *  @li@c  ui4_flag                                          - flag for specifying which options to be set
 *  @li@c  ui4_width                                         - canvas width
 *  @li@c  ui4_height                                        - canvas height
 *  @li@c  b_auto_scale                                      - auto scale or not
 *  @li@c  e_clr_mode                                        - color mode
 *  @li@c  t_colorkey                                        - colorkey
 *  @li@c  h_palette                                         - palette handle
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_CANVAS_CFG_T
{
    UINT32          ui4_flag;
    UINT32          ui4_width;
    UINT32          ui4_height;
    BOOL            b_auto_scale;
    GL_COLORMODE_T  e_clr_mode;
    GL_COLOR_T      t_colorkey;
    GL_HPALETTE_T   h_palette;
	BOOL			b_force_no_inter_compose;
} WGL_CANVAS_CFG_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_CVS_CLR_CFG_T
 *  @brief  specifies the canvas color configuration
 *  @code
 *  typedef struct _WGL_CVS_CLR_CFG_T
 *  {
 *      UINT32          ui4_flag;
 *      GL_COLOR_T      t_colorkey;
 *      GL_HPALETTE_T   h_palette;
 *  } WGL_CVS_CLR_CFG_T;
 *  @endcode
 *  @li@c  ui4_flag                                          - flag for specifying which options to be set
 *  @li@c  t_colorkey                                        - canvas color key
 *  @li@c  h_palette                                         - canvas palette table
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_CVS_CLR_CFG_T
{
    UINT32          ui4_flag;
    GL_COLOR_T      t_colorkey;
    GL_HPALETTE_T   h_palette;
    GL_COLORMODE_T  e_clr_mode;
} WGL_CVS_CLR_CFG_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_CANVAS_INFO_T
 *  @brief  structure for describing the canvas information
 *  @code
 *  typedef struct _WGL_CANVAS_INFO_T
 *  {
 *      UINT32          ui4_width;
 *      UINT32          ui4_height;
 *      GL_COLORMODE_T  e_clr_mode;
 *      GL_COLOR_T      t_colorkey;
 *      BOOL            b_auto_scale;
 *      HANDLE_T        h_canvas;
 *  } WGL_CANVAS_INFO_T;
 *  @endcode
 *  @li@c  ui4_width                                         - canvas width
 *  @li@c  ui4_height                                        - canvas height
 *  @li@c  e_clr_mode                                        - canvas color mode
 *  @li@c  t_colorkey                                        - canvas color key
 *  @li@c  b_auto_scale                                      - auto scaling or not
 *  @li@c  h_canvas                                          - canvas handle
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_CANVAS_INFO_T
{
    UINT32          ui4_width;
    UINT32          ui4_height;
    GL_COLORMODE_T  e_clr_mode;
    GL_COLOR_T      t_colorkey;
    BOOL            b_auto_scale;
    HANDLE_T        h_canvas;
} WGL_CANVAS_INFO_T;

/* definition of notify structures */
/*------------------------------------------------------------------*/
/*! @struct WGL_NOTIFY_DATA_T
 *  @brief  definition of notifying structures
 *  @code
 *  typedef struct _WGL_NOTIFY_DATA_T
 *  {
 *      UINT32          ui4_nc_code;
 *      union {
 *          struct {
 *              UINT32          ui4_key_state;
 *              UINT32          ui4_key_code;
 *          } t_key;
 *          struct {
 *              INT32           i4_old_pos;
 *              INT32           i4_new_pos;
 *          } t_pos;
 *          struct
 *          {
 *              BOOL            b_is_highlighted;
 *              UINT16          ui2_elem_idx;
 *          } t_lb_elem_hlt;
 *          struct
 *          {
 *              BOOL            b_is_selected;
 *              UINT16          ui2_elem_idx;
 *          } t_lb_elem_sel;
 *          struct
 *          {
 *              BOOL            b_is_pushed;
 *              UINT16          ui2_elem_idx;
 *          } t_lb_elem_push;
 *          struct
 *          {
 *              GL_SIZE_T       t_size_new;
 *          } t_cc_resized;
 *          struct
 *          {
 *              GL_POINT_T      t_location_new;
 *          } t_cc_moved;
 *          struct
 *          {
 *              BOOL            b_is_scroll_up;
 *          } t_cc_scrolled;
 *          struct
 *          {
 *              UINT16      ui2_transc_id;
 *              HANDLE_T    h_cc;
 *              INT32       i4_result;
 *          } t_cc_cmd_finished;
 *      } u;
 *  } WGL_NOTIFY_DATA_T;
 *  @endcode
 *  @li@c  ui4_nc_code                                       - notification type
 *  @li@c  ui4_key_state                                     - key state
 *  @li@c  ui4_key_code                                      - key code
 *  @li@c  i4_old_pos                                        - old position
 *  @li@c  i4_new_pos                                        - new position
 *  @li@c  b_is_highlighted                                  - is highlighted
 *  @li@c  ui2_elem_idx                                      - element index
 *  @li@c  b_is_selected                                     - is selected
 *  @li@c  ui2_elem_idx                                      - element index
 *  @li@c  b_is_pushed                                       - is pushed
 *  @li@c  ui2_elem_idx                                      - element index
 *  @li@c  t_size_new                                        - new size
 *  @li@c  t_location_new                                    - new location
 *  @li@c  b_is_scroll_up                                    - is scrolled up
 *  @li@c  ui2_transc_id                                     - transaction index
 *  @li@c  h_cc                                              - CC handle
 *  @li@c  i4_result                                         - result
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_NOTIFY_DATA_T
{
    UINT32          ui4_nc_code;

    union {
        struct {
            UINT32          ui4_key_state;
            UINT32          ui4_key_code;
        } t_key;

        struct {
            INT32           i4_old_pos;
            INT32           i4_new_pos;
        } t_pos;

        /*List Box Notification START */
        struct
        {
            BOOL            b_is_highlighted;
            UINT16          ui2_elem_idx;
        } t_lb_elem_hlt;
        struct
        {
            BOOL            b_is_selected;
            UINT16          ui2_elem_idx;
        } t_lb_elem_sel;
        struct
        {
            BOOL            b_is_pushed;
            UINT16          ui2_elem_idx;
        } t_lb_elem_push;
        /*List Box Notification END */

        /*Closed Caption Notification START */
        struct
        {
            GL_SIZE_T       t_size_new;
        } t_cc_resized;

        struct
        {
            GL_POINT_T      t_location_new;
        } t_cc_moved;

        struct
        {
            BOOL            b_is_scroll_up;
        } t_cc_scrolled;

        struct
        {
            UINT16      ui2_transc_id;
            HANDLE_T    h_cc;
            INT32       i4_result;
        } t_cc_cmd_finished;
        /*Closed Caption Notification END*/
    } u;
} WGL_NOTIFY_DATA_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_CREATE_INFO_T
 *  @brief  This structure is used while the message WGL_MSG_CREATE is sent to the message procedure of the widget being created. The third argument pv_param1 of message procedure will reference a structure of this type if the message being passed is WGL_MSG_CREATE.
 *  @code
 *  typedef struct _WGL_CREATE_INFO_T
 *  {
 *      GL_RECT_T*      pt_rect;
 *      VOID*           pv_param;
 *  } WGL_CREATE_INFO_T;
 *  @endcode
 *  @li@c  pt_rect                                           - References a GL_RECT_T structure which defines the dimension and position of the created widget.
 *  @li@c  pv_param                                          - References the private data provided by application. Note that this field contains the same value with the pv_param argument while x_wgl_create_widget() is called.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_CREATE_INFO_T
{
    GL_RECT_T*      pt_rect;
    VOID*           pv_param;
} WGL_CREATE_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_PACKED_PARAM_T
 *  @brief This structure is simply used as a content holder and packs the third and fourth arguments of x_wgl_do_cmd() in one structure.
 *  @code
 *  typedef struct _WGL_PACKED_PARAM_T
 *  {
 *      VOID*           pv_param1;
 *      VOID*           pv_param2;
 *  } WGL_PACKED_PARAM_T;
 *  @endcode
 *  @li@c  pv_param1                                         - References additional data. The real meaning of this field is context-dependent. It's the same as the third argument of x_wgl_do_cmd().
 *  @li@c  pv_param2                                         - References additional data. The real meaning of this field is context-dependent. It's the same as the fourth argument of x_wgl_do_cmd().
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_PACKED_PARAM_T
{
    VOID*           pv_param1;
    VOID*           pv_param2;
} WGL_PACKED_PARAM_T;

/*------------------------------------------------------------------*/
/*! @struct  WGL_KEY_LINK_T
 *  @brief  This structure is used in the API c_wgl_set_navigation() to specify the correlation between navigation key and target widget.
 *  @code
 *  typedef struct _WGL_KEY_LINK_T
 *  {
 *      UINT32      ui4_key;
 *      HANDLE_T    h_widget;
 *  } WGL_KEY_LINK_T;
 *  @endcode
 *  @li@c  ui4_key                                           - Contains the key code of specified navigation key.
 *  @li@c  h_widget                                          - Contains the handle of the target widget.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_KEY_LINK_T
{
    UINT32      ui4_key;
    HANDLE_T    h_widget;
} WGL_KEY_LINK_T;


typedef INT32 (*c_wgl_img_async_load)         (    HANDLE_T             h_img,
                                                        IMG_INTRA_TYPE_T     e_type,
                                                        IMG_TYPE_DATA_T*     pt_type_data,
                                                        IMG_ROTATE_OP_TYPE_T e_rotate_type,
                                                        UINT32               ui4_src_x,
                                                        UINT32               ui4_src_y,
                                                        UINT32               ui4_src_w,
                                                        UINT32               ui4_src_h,
                                                        UINT32               ui4_dst_x,
                                                        UINT32               ui4_dst_y,
                                                        UINT32               ui4_dst_w,
                                                        UINT32               ui4_dst_h,
                                                        GL_HSURFACE_T        h_dst_surf);

typedef INT32  (*c_wgl_img_sync_load)    ( HANDLE_T             h_img,
                                           IMG_INTRA_TYPE_T     e_type,
                                           IMG_TYPE_DATA_T*     pt_type_data,
                                           IMG_ROTATE_OP_TYPE_T e_rotate_type,
                                           UINT32               ui4_src_x,
                                           UINT32               ui4_src_y,
                                           UINT32               ui4_src_w,
                                           UINT32               ui4_src_h,
                                           UINT32               ui4_dst_x,
                                           UINT32               ui4_dst_y,
                                           UINT32               ui4_dst_w,
                                           UINT32               ui4_dst_h,
                                           GL_HSURFACE_T        h_dst_surf);


typedef INT32 (*c_wgl_img_mopen)  ( const VOID*     pv_mem_data,
                                        SIZE_T          z_mem_leng,
                                        const CHAR*     ps_format,
                                        x_img_nfy_fct   pf_nfy,
                                        const VOID*     pv_tag,
                                        HANDLE_T*       ph_img);


typedef INT32 (*c_wgl_img_fopen)  ( const CHAR*     ps_name,
                                    UINT8           ui1_open_mode,
                                    x_img_nfy_fct   pf_nfy,
                                    const VOID*     pv_tag,
                                    HANDLE_T*       ph_img);

typedef INT32 (*c_wgl_img_get_dimension) ( HANDLE_T         h_img,      /* (I) specify the already opened image handle. */
                                                 IMG_INTRA_TYPE_T e_type,     /* (I) specify what type of dimension want to get. */
                                                 SIZE_T*          pz_width,   /* (O) retrieve the width of the image. */
                                                 SIZE_T*          pz_height); /* (O) retrieve the height of the image. */


typedef INT32 (*c_wgl_img_force_sw_decode) (BOOL b_sw);

typedef INT32 (*c_wgl_img_close) (HANDLE_T h_img);


typedef struct _WGL_IMG_DECODE_FUNCTION_TABLE_T
{
    c_wgl_img_async_load    pf_async_load;
    c_wgl_img_sync_load     pf_sync_load;
    c_wgl_img_mopen         pf_mopen;
    c_wgl_img_fopen         pf_fopen;
    c_wgl_img_close         pf_close;
    c_wgl_img_get_dimension pf_get_dimension;
	c_wgl_img_force_sw_decode   pf_set_sw_decode_force;
}WGL_IMG_DECODE_FUNCTION_TABLE_T;


#define DEFAULT_IMAGE_WIDTH   ((UINT32)1)
#define DEFAULT_IMAGE_HEIGHT  ((UINT32)1)


#ifdef __cplusplus
}
#endif

#endif

/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_WGL*/
/*----------------------------------------------------------------------------*/
