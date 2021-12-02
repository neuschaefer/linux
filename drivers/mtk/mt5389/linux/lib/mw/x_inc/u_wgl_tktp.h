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
/*! @file u_wgl_tktp.h 
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
/*! @defgroup groupMW_WGL_TKTP   Ticker Tape Widget
 *
 *  @ingroup groupMW_WGL
 *  @brief Provides definitions and exported function of the Ticker Tape widget.A Ticker Tape widget provides a way for applications to present a long text-based message in a constrained area of the OSD. The Ticker Tape widget presents the message by horizontally and smoothly scrolling the text. Besides, by optionally specifying some Ticker Tape widget styles, the message can be repeatedly scrolled around the viewport or stopped scrolling after one cycle. 
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW_WGL
 *  @{ 
 */
/*----------------------------------------------------------------------------*/ 

#ifndef _U_WGL_TKTP_H_
#define _U_WGL_TKTP_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_wgl_common.h"
#include "u_common.h"
#include "u_gl.h"
#include "u_wgl_tktp_cnt_common.h"
 
 
#ifdef __cplusplus
extern "C" {
#endif


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* Widget Commands */

/**  
This command retrieves the widget style. Note that widget style can only be specified during its creation. The value returned by this command will always identical to the value specified in the field ui4_style of the structure WGL_TKTP_INIT_T.

Input arguments

pv_param2	Reserved. It must be NULL.

Output arguments

pv_param1	Contains an (UINT32 *) pointer of a single UINT32 buffer to receive the widget style. Note that this argument must be of type (UINT32 *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGL_INV_ARG	The parameter pv_param1 is NULL.
*/
#define WGL_CMD_TKTP_GET_STYLE                      WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TKTP, 0)  
/**  
This command queries the current state of the Ticker Tape widget. At any given time, a Ticker Tape widget may stays in one of the following states. 

 	WGL_TKTP_STATE_HIDDEN

 	WGL_TKTP_STATE_SCRL_DISABLED

 	WGL_TKTP_STATE_SCRL_STOPPED

 	WGL_TKTP_STATE_SCRLING_FIRST_CYCLE

 	WGL_TKTP_STATE_SCRLING_MORE_CYCLE

For more information about these states, refer to the enumeration WGL_TKTP_STATE_T.

Input arguments

pv_param2	Reserved. It must be NULL.

Output arguments

pv_param1	Contains a (WGL_TKTP_STATE_T *) pointer of a single WGL_TKTP_STATE_T buffer to receive the widget state. Note that this argument must be of type (WGL_TKTP_STATE_T *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGL_INV_ARG	The parameter pv_param1 is NULL.
*/
#define WGL_CMD_TKTP_GET_STATE                      WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TKTP, 1)  
/**  
This command makes the Ticker Tape widget visible if it is originally hidden. Duplicated WGL_CMD_TKTP_SHOW commands sent to a visible Ticker Tape widget takes no effects. If a Ticker Tape widget was made hidden at the time when it was scrolling a message text, this command will makes the widget visible and then automatically resumes the scrolling if the text still needs to be scrolled. 

Input arguments

pv_param1	Reserved. It must be NULL.

pv_param2	Reserved. It must be NULL.


Returns

WGLR_OK	The function call is successful.
*/
#define WGL_CMD_TKTP_SHOW                           WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TKTP, 2)   
/**  
This command makes the Ticker Tape widget hidden if it is originally visible in one of the following manners. The hiding mode has to be specified in the argument pv_param1.

 	WGL_TKTP_HIDE_IMMEDIATELY mode

 	Immediately hides the widget

 	WGL_TKTP_HIDE_AFTER_SCRL_ONCE mode

 	In case the messages text is scrolling in its first cycle, this command hides the widget in an asynchronous manner and the hiding request is postponed until the text has been scrolled once. 

 	In case the messages text is scrolling in its second or more cycle, the widget will be hidden immediately. 

 	In case the widget is not scrolling the message text, e.g. the text is short enough to fit into the viewport and thus no need to be scrolled, this command immediately hide the widget.

Duplicated hide commands with the same hiding mode sent to a hidden Ticker Tape widget takes no effects. However, a hide command with WGL_TKTP_HIDE_IMMEDIATELY mode will override a previously issued hide command with WGL_TKTP_HIDE_AFTER_SCRL_ONCE mode, and in this case, the widget is immediately hidden by the late coming command. In addition, a destroy command WGL_CMD_TKTP_DESTROY always overrides the hide command with WGL_TKTP_HIDE_AFTER_SCRL_ONCE mode. Refer to the command WGL_CMD_TKTP_DESTROY for more information.

If the message text is scrolling and the widget is going to be hidden, the text automatically stops scrolling until next time the widget became visible.


Input arguments

pv_param1	Indicates the hiding mode with one of the following value

 	WGL_TKTP_HIDE_IMMEDIATELY

 	WGL_TKTP_HIDE_AFTER_SCRL_ONCE

This argument must be of type WGL_TKTP_HIDE_MODE_T. 

pv_param2	Contains a (VOID *) pointer indicating any data block which is transparent to the Ticker Tape widget and only meaningful to the application. The pointer will be passed in the third argument of the callback function (*wgl_tktp_nfy_fct) when the widget is going to hide itself. Note that the value in the argument pv_param2 of duplicated WGL_CMD_TKTP_HIDE commands will override that of the previous sent WGL_CMD_TKTP_HIDE commands.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The hiding mode specified in the argument pv_param1 is invalid.
*/
#define WGL_CMD_TKTP_HIDE                           WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TKTP, 3)
/**
This command destroys the Ticker Tape widget in one of the following manners. The destroy mode has to be specified in the argument pv_param1.

 	WGL_TKTP_DESTROY_IMMEDIATELY mode

 	Immediately destroys the widget

 	WGL_TKTP_DESTROY_AFTER_SCRL_ONCE mode

 	In case the messages text is scrolling in its first cycle, this command destroys the widget in an asynchronous manner and the destroy request is postponed until the text has been scrolled once.

 	In case the messages text is scrolling in the second or more cycle, the widget will be destroyed immediately.

 	In case the widget is not scrolling the message text, e.g. the text is short enough to fit into the viewport and thus no need to be scrolled; the widget will be immediately destroyed.

Duplicated destroy commands with the same destroy mode will be ignored, but the new destroy command will override the pv_tag passed in the argument pv_param2 of the old command. However, a destroy command with WGL_TKTP_DESTROY_IMMEDIATELY mode will override a previously issued destroy command with WGL_TKTP_DESTROY_AFTER_SCRL_ONCE mode, and in this case, the widget is immediately destroyed by the late coming command. In addition, a destroy command will always overrides the hide command WGL_CMD_TKTP_HIDE with WGL_TKTP_HIDE_AFTER_SCRL_ONCE mode. 


Input arguments

pv_param1	Indicates the destroy mode with one of the following value

 	WGL_TKTP_DESTROY_IMMEDIATELY

 	WGL_TKTP_DESTROY_AFTER_SCRL_ONCE

This argument must be of type WGL_TKTP_DESTROY_MODE_T. 

pv_param2	Contains a (VOID *) pointer indicating any data block which is transparent to the Ticker Tape widget and only meaningful to the application. The pointer will be passed in the third argument of the callback function (*wgl_tktp_nfy_fct) when the widget is going to destroy itself. Note that the value in the argument pv_param2 of duplicated WGL_CMD_TKTP_DESTROY commands will override that of the previous sent WGL_CMD_TKTP_DESTROY commands.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The destroy mode specified in the argument pv_param1 is invalid.
*/
#define WGL_CMD_TKTP_DESTROY                        WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TKTP, 4)
/**
This command sets the parameters for Ticker Tape widget to automatically adjust its location and size. Note that this command is valid only when the widget style WGL_STL_TKTP_AUTO_LCSZ_CTRL is specified during creating the widget. Note that this command automatically restarts text scrolling.

Input arguments

pv_param1	Specifies a pointer to the structure WGL_TKTP_LCSZ_CTRL_T containing the new parameters for auto relocation and resizing mechanism. This argument must be of type (WGL_TKTP_LCSZ_CTRL_T *) and cannot be NULL.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGL_INV_ARG	At least one of the following cases occurs:

 	The parameter pv_param1 is NULL.

 	The parameter of type WGL_TKTP_LCSZ_CTRL_T specified in pv_param1 is invalid.

WGLR_INV_CMD	The widget style WGL_STL_TKTP_AUTO_LCSZ_CTRL is not specified. 
*/
#define WGL_CMD_TKTP_SET_LCSZ_CTRL                  WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TKTP, 5)
/**
This command retrieves the current parameters for auto relocation and resizing control. Note that this command is valid only when the widget style WGL_STL_TKTP_AUTO_LCSZ_CTRL is specified.

Input arguments

pv_param2	Reserved. It must be NULL.

Output arguments

pv_param1	Contains a (WGL_TKTP_LCSZ_CTRL_T *) pointer of a single WGL_TKTP_LCSZ_CTRL_T buffer to receive the LCSZ parameters. Note that this argument must be of type (WGL_TKTP_LCSZ_CTRL_T *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGL_INV_ARG	The parameter pv_param1 is NULL.

WGLR_INV_CMD	The widget style WGL_STL_TKTP_AUTO_LCSZ_CTRL is not specified. 
*/
#define WGL_CMD_TKTP_GET_LCSZ_CTRL                  WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TKTP, 6) 
/**
This command sets the text of the message to be presented by Ticker Tape widget. If the text is too wide to fit into the viewport when the Ticker Tape widget is visible and the scrolling effect is not disabled, the widget automatically begins to scroll the given text. In the case that a short text which doesn't need to be scrolled is specified and the original message text is under scrolling before this command, the scrolling effect will be stopped after the new text is presented.

Input arguments

pv_param1	Contains a serial of UTF16_T characters to be set. Note that the specified text is not necessary to be a NULL-terminated string since its length can be specified in pv_param2. This argument must be of type (UTF16_T *) and cannot be NULL.

pv_param2	Contains the number of UTF16_T characters in pv_param1 to be set as the message of the Ticker Tape widget. This argument must be of type UINT16 and must be larger than zero.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	At least one of the following cases occurs:

 	The argument pv_param1 is NULL

 	The text length specified in the argument pv_param2 is zero. 
*/
#define WGL_CMD_TKTP_SET_TEXT                       WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TKTP, 7)
/**
This command gets the text of the message to be presented by Ticker Tape widget. Note that the caller of this command shall prepare an UTF16_T string buffer to receive the text of the Ticker Tape widget.

Input & Output arguments

pv_param1	Contains the pointer to an UTF16_T string buffer to receive the text of Ticker Tape widget. The text is returned as a NULL-terminated string. Note that if the buffer is not large enough, the error message WGLR_BUFFER_NOT_ENOUGH is returned. This argument must be of type (UTF16_T *) and cannot be NULL.

Input arguments

pv_param2	Contains the length (in UTF16_T character count) of the UTF16_T string buffer that is specified in pv_param1.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL

WGLR_BUFFER_NOT_ENOUGH	
The buffer size (in UTF16_T character count) specified in the argument pv_param2 is not enough to contain the whole message text.
*/
#define WGL_CMD_TKTP_GET_TEXT                       WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TKTP, 8)
/**
This command clears the message text of Ticker Tape widget. 

Input arguments

pv_param1	Reserved. It must be NULL.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.
*/
#define WGL_CMD_TKTP_CLEAR_TEXT                     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TKTP, 9) 
/**
This command enables or disables the scrolling effect for the message text of Ticker Tape widget. If the message text is too wide to fit into the viewport, it will be scrolled around the viewport if the scrolling effect is enabled. But if the scrolling effect is disabled, the text will not be scrolled. By default, the scrolling effect of a newly created Ticker Tape widget is enabled.

Input arguments

pv_param1	Enables (TRUE) or disables (FALSE) the scrolling effect. Note that this argument must be of type BOOL.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The value specified in the argument pv_param1 is invalid. 
*/
#define WGL_CMD_TKTP_ENABLE_SCRL                    WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TKTP, 10)
/**
This command restarts scrolling the message text. In case the text is short enough to fit into the viewport and hence doesn't need to be scrolled, this command does not take any effect; Otherwise, this command makes a scrolling effect restart from begin of the text and reset the scrolling context as if it is in the first cycle of the scrolling.

Input arguments

pv_param1	Reserved. It must be NULL.

pv_param2	Reserved. It must be NULL.


Returns

WGLR_OK	The function call is successful.
*/
#define WGL_CMD_TKTP_RESTART_SCRL                   WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TKTP, 11)
/** 
This command resumes scrolling the message text from the point last time the scrolling effect is frozen by the command WGL_CMD_TKTP_PAUSE_SCRL. If this command is applied upon a text that is already scrolling, it doesn't take any effect. Besides, duplicated WGL_CMD_TKTP_RESUME_SCRL commands are ignored.

Input arguments

pv_param1	Reserved. It must be NULL.

pv_param2	Reserved. It must be NULL.


Returns

WGLR_OK	The function call is successful.
*/
#define WGL_CMD_TKTP_RESUME_SCRL                    WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TKTP, 12)
/**
This command freezes the scrolling effect of the message text. If the text is not scrolling, this command takes no effects. After receiving this command, a scrolling text is immediately frozen and remains the frozen state until receiving the command WGL_CMD_TKTP_RESUME_SCRL or one of the following commands that will automatically restart the scrolling effect.

 	WGL_CMD_TKTP_SET_LCSZ_CTRL

 	WGL_CMD_TKTP_SET_TEXT

 	WGL_CMD_TKTP_RESTART_SCRL

 	WGL_CMD_GL_SET_FONT

Note that duplicated WGL_CMD_TKTP_PAUSE_SCRL commands are ignored. 

Input arguments

pv_param1	Reserved. It must be NULL.

pv_param2	Reserved. It must be NULL.


Returns

WGLR_OK	The function call is successful.
*/
#define WGL_CMD_TKTP_PAUSE_SCRL                     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TKTP, 13)
/**
This command sets the step size (in pixel) each time the message text shall be scrolled. Note that the specified minimal step size must be larger or equal to the constant WGL_TKTP_MIN_SCRL_STEP_SIZE. Besides, if the step size is too large, it will be automatically truncated as the "width" of the text message's font. If the viewport size is smaller than the font width, the step size will be truncated as WGL_TKTP_MIN_SCRL_STEP_SIZE. This prevents that too many characters are scrolled out of the viewport at once each time the message text is scrolled.

Input arguments

pv_param1	Contains the scrolling step size in pixel. This argument must be of type (UINT16).

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The specified scrolling step size is too small.
*/
#define WGL_CMD_TKTP_SET_SCRL_STEP_SIZE             WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TKTP, 14)
/**
This command gets the current setting for scrolling step size in pixel.

Input arguments

pv_param2	Reserved. It must be NULL.

Output arguments

pv_param1	Returns the current setting for scrolling step size in pixel. This argument must be of type (UINT16 *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL
*/
#define WGL_CMD_TKTP_GET_SCRL_STEP_SIZE             WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TKTP, 15)
/**
This command sets the delay time (in milliseconds) between two contiguous scrolling events. The scrolling delay must be equal to or larger than WGL_TKTP_MIN_SCRL_DELAY. 

Input arguments

pv_param1	Contains the scrolling delay time in milliseconds. This argument must be of type (UINT16).

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The delay time specified in argument pv_param1 is smaller than WGL_TKTP_MIN_SCRL_DELAY.
*/
#define WGL_CMD_TKTP_SET_SCRL_DELAY                 WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TKTP, 16)
/**
This command gets the current setting for scrolling delay time in milliseconds.

Input arguments

pv_param2	Reserved. It must be NULL.

Output arguments

pv_param1	Returns the current setting for scrolling delay time in milliseconds. This argument must be of type (UINT16 *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL
*/
#define WGL_CMD_TKTP_GET_SCRL_DELAY                 WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TKTP, 17)
/**
This command registers a notify callback function to Ticker Tape widget. The notify function is called under one of the following conditions:

 	WGL_TKTP_NFY_COND_SCRL_END_REACHED: indicates the condition that each time when the Ticker Tape widget finishes presenting the whole message text by scrolling. Note that the notification function will be repeatedly called each time the end of the message text is reached when the text is scrolling.

 	WGL_TKTP_NFY_COND_HIDDEN: indicates the condition that the Ticker Tape widget becomes hidden. Note that the notification function may be called just before the widget becomes hidden or right after it is already hidden.

 	WGL_TKTP_NFY_COND_DESTROYED: indicates the condition that the Ticker Tape widget is going to be destroyed. Note that the notification function will always be called just before the widget is going to be destroyed. 

The condition codes listed above are defined in the enumeration WGL_TKTP_NFY_COND_T. Note that the handling procedure in the notify callback function is recommended to be as short as possible. If the application registers a NULL function, the Ticker Tape widget would not issue any notification.

Input arguments

pv_param1	Contains the functions pointer of type (*wgl_tktp_nfy_fct). If this argument is NULL, the Ticker Tape widget will stop issuing any notification.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.
*/
#define WGL_CMD_TKTP_REG_NFY                        WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TKTP, 18)  

/* Specific Widget Styles */
/** If this style is specified, the Ticker Tape widget automatically adjusts its location and size according to the pixel size of message text and the parameter structure WGL_TKTP_LCSZ_CTRL_T. The parameter structure WGL_TKTP_LCSZ_CTRL_T can be specified in the initialization structure WGL_TKTP_INIT_T during creating the widget and can also be updated by the command WGL_CMD_TKTP_SET_LCSZ_CTRL after the widget is created.  */
#define WGL_STL_TKTP_AUTO_LCSZ_CTRL                 (((UINT32)(WGL_STL_WGT_SPF_START))<<0) 
/**  If this style is specified, the Ticker Tape widget automatically hides itself right after it is created. Otherwise, if this style is not specified, the widget is visible by default after created.      */
#define WGL_STL_TKTP_HIDE_AFTER_CREATED             (((UINT32)(WGL_STL_WGT_SPF_START))<<1) 
/**  If this style is specified, the Ticker Tape widget automatically stops scrolling right after it has scrolled the message text once. Otherwise, if this style is not specified, the message text will be cyclically and repeatedly scrolled.   */
#define WGL_STL_TKTP_SCRL_ONCE                      (((UINT32)(WGL_STL_WGT_SPF_START))<<2) 
/** If this style is specified, the Ticker Tape widget does not scroll if the message text is short enough to fit in the viewport. Otherwise, if this style is not specified, the message text will always be scrolled no matter it is short or long. */
#define WGL_STL_TKTP_NO_SCRL_SHORT_TEXT             (((UINT32)(WGL_STL_WGT_SPF_START))<<3)
/**  last syle equals to  WGL_STL_TKTP_NO_SCRL_SHORT_TEXT  */
#define WGL_STL_TKTP_LAST_STYLE                     WGL_STL_TKTP_NO_SCRL_SHORT_TEXT   

    
/* Constants */
#define WGL_TKTP_MIN_SCRL_STEP_SIZE                 1    /**<  This constant defines the minimal scrolling step size.      */
#define WGL_TKTP_MIN_SCRL_DELAY                     10    /**<  This constant defines the minimal scrolling delay for each time a step of the text scrolling is performed.      */

/*------------------------------------------------------------------*/
/*! @enum WGL_TKTP_HIDE_MODE_T
 *  @brief This enumeration specifies the hiding modes when an application tries to hide the Ticker Tape widget by issuing the WGL_CMD_TKTP_HIDE command
 *  @code
 *  typedef enum _WGL_TKTP_HIDE_MODE_T
 *  {
 *      WGL_TKTP_HIDE_IMMEDIATELY       =       0,
 *      WGL_TKTP_HIDE_AFTER_SCRL_ONCE
 *  } WGL_TKTP_HIDE_MODE_T;
 *  @endcode
 *  @li@c  WGL_TKTP_HIDE_IMMEDIATELY                         -In this hiding mode the WGL_CMD_TKTP_HIDE command immediately hides the Ticker Tape widget. Refer to the command WGL_CMD_TKTP_HIDE for more information.
 *  @li@c  WGL_TKTP_HIDE_AFTER_SCRL_ONCE                     -In this hiding mode the WGL_CMD_TKTP_HIDE command asynchronously postpones hiding the Ticker Tape widget until the message text has been scrolled once. Refer to the command WGL_CMD_TKTP_HIDE for more information.
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_TKTP_HIDE_MODE_T
{
    WGL_TKTP_HIDE_IMMEDIATELY       =       0,
    WGL_TKTP_HIDE_AFTER_SCRL_ONCE
} WGL_TKTP_HIDE_MODE_T;

#define WGL_TKTP_IS_VALID_HIDE_MODE(_e_mode)        ((_e_mode)<=1)    /**<        */


/*------------------------------------------------------------------*/
/*! @enum WGL_TKTP_DESTROY_MODE_T
 *  @brief This enumeration specifies the destroy modes when an application tries to destroy the Ticker Tape widget by issuing the WGL_CMD_TKTP_DESTROY command. 
 *  @code
 *  typedef enum _WGL_TKTP_DESTROY_MODE_T
 *  {
 *      WGL_TKTP_DESTROY_IMMEDIATELY    =       0,
 *      WGL_TKTP_DESTROY_AFTER_SCRL_ONCE
 *  } WGL_TKTP_DESTROY_MODE_T;
 *  @endcode
 *  @li@c  WGL_TKTP_DESTROY_IMMEDIATELY                      - In this destroy mode the WGL_CMD_TKTP_DESTROY command immediately destroys the Ticker Tape widget. Refer to the command WGL_CMD_TKTP_DESTROY for more information.
 *  @li@c  WGL_TKTP_DESTROY_AFTER_SCRL_ONCE                  - In this destroy mode the WGL_CMD_TKTP_DESTROY command asynchronously postpones hiding the Ticker Tape widget until the message text has been scrolled once. Refer to the command WGL_CMD_TKTP_DESTROY for more information.
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_TKTP_DESTROY_MODE_T
{
    WGL_TKTP_DESTROY_IMMEDIATELY    =       0,
    WGL_TKTP_DESTROY_AFTER_SCRL_ONCE
} WGL_TKTP_DESTROY_MODE_T;

#define WGL_TKTP_IS_VALID_DESTROY_MODE(_e_mode)     ((_e_mode)<=1)    /**<        */


/*------------------------------------------------------------------*/
/*! @enum WGL_TKTP_STATE_T
 *  @brief This enumeration specifies the Ticker Tape widget's states that may be queried by the WGL_CMD_TKTP_GET_STATE command. 
 *  @code 
 *  typedef enum _WGL_TKTP_STATE_T
 *  {
 *      WGL_TKTP_STATE_HIDDEN           =       0,
 *      WGL_TKTP_STATE_SCRL_DISABLED,
 *      WGL_TKTP_STATE_SCRL_STOPPED,
 *      WGL_TKTP_STATE_SCRLING_FIRST_CYCLE,
 *      WGL_TKTP_STATE_SCRLING_MORE_CYCLE
 *  }   WGL_TKTP_STATE_T;    
 *  @endcode
 *  @li@c  WGL_TKTP_STATE_HIDDEN                             - Indicates the widget is currently in hidden state.
 *  @li@c  WGL_TKTP_STATE_SCRL_DISABLED                      -Indicates the widget is visible but its scrolling effect is disabled. The scrolling effect may be disabled by the WGL_CMD_TKTP_ENABLE_SCRL command with FALSE parameter.
 *  @li@c  WGL_TKTP_STATE_SCRL_STOPPED                       - Indicates the widget is currently not scrolling the message text while it's visible and its scrolling effect is enabled. For one of the following reasons the Ticker Tape widget stays in this state: 
 *                                                                                         The message text is short enough that can be completely fit in the viewport and doesn't need to be scrolled around.
 *                                                                                          The widget style WGL_STL_TKTP_SCRL_ONCE is specified and the scrolling effect is automatically stopped after the message text has been scrolled around the viewport once.
 *                                                                                           A scrolling message text was paused by the WGL_CMD_TKTP_PAUSE_SCRL command.
 *  @li@c  WGL_TKTP_STATE_SCRLING_FIRST_CYCLE                - Indicates the widget is visible and currently in the first cycle of the message text scrolling. For one of the following reasons the Ticker Tape widget stays in this state:
 *                                                                                              The message text is updated by the WGL_CMD_TKTP_SET_TEXT command and it's too wide to fit in the viewport. In this case, the Ticker Tape widget automatically starts to scroll the text.
 *                                                                                               One of the following commands is issued for a wide message text.
 *                                                                                                  WGL_CMD_TKTP_SET_LCSZ_CTRL
 *                                                                                                   WGL_CMD_TKTP_RESTART_SCRL
 *                                                                                                  WGL_CMD_GL_SET_FONT
 *  @li@c  WGL_TKTP_STATE_SCRLING_MORE_CYCLE                 -Indicates the widget is visible and currently in the second or more cycles of the message text scrolling. 
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_TKTP_STATE_T
{
    WGL_TKTP_STATE_HIDDEN           =       0,
    WGL_TKTP_STATE_SCRL_DISABLED,
    WGL_TKTP_STATE_SCRL_STOPPED,
    WGL_TKTP_STATE_SCRLING_FIRST_CYCLE,
    WGL_TKTP_STATE_SCRLING_MORE_CYCLE
}   WGL_TKTP_STATE_T;    



/*------------------------------------------------------------------*/
/*! @enum  WGL_TKTP_ANCHOR_MODE_T
 *  @brief This enumeration specifies the anchor mode which is used in the WGL_TKTP_LCSZ_CTRL_T structure.
 *  @code
 *  typedef enum _WGL_TKTP_ANCHOR_MODE_T
 *  {
 *      WGL_TKTP_ANCHOR_TOP             =       0,
 *      WGL_TKTP_ANCHOR_BOTTOM
 *  } WGL_TKTP_ANCHOR_MODE_T;
 *  @endcode
 *  @li@c  WGL_TKTP_ANCHOR_TOP                               -Specifies the TOP anchor mode.  
 *  @li@c  WGL_TKTP_ANCHOR_BOTTOM                            -Specifies the BOTTOM anchor mode. 
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_TKTP_ANCHOR_MODE_T
{
    WGL_TKTP_ANCHOR_TOP             =       0,
    WGL_TKTP_ANCHOR_BOTTOM
} WGL_TKTP_ANCHOR_MODE_T;

#define WGL_TKTP_IS_VALID_ANCHOR_MODE(_e_mode)      ((_e_mode)<=1)    /**<        */
    

/*------------------------------------------------------------------*/
/*! @enum WGL_TKTP_NFY_COND_T
 *  @brief  This enumeration specifies the notification conditions when the Ticker Tape widget notify application by calling the callback function (*wgl_tktp_nfy_fct). Refer to the WGL_CMD_TKTP_REG_NFY command for more information.
 *  @code
 *  typedef enum _WGL_TKTP_NFY_COND_T
 *  {
 *      WGL_TKTP_NFY_COND_SCRL_END_REACHED   =   0,
 *      WGL_TKTP_NFY_COND_HIDDEN,
 *      WGL_TKTP_NFY_COND_DESTROYED
 *  } WGL_TKTP_NFY_COND_T;    
 *  @endcode
 *  @li@c  WGL_TKTP_NFY_COND_SCRL_END_REACHED                -This notification code indicates the condition that each time when the Ticker Tape widget finishes presenting the whole message text by scrolling. Note that the notification function will be repeatedly called each time the end of the message text is reached when the text is scrolling. 
 *  @li@c  WGL_TKTP_NFY_COND_HIDDEN                          -This notification code indicates the condition that the Ticker Tape widget becomes hidden. Note that the notification function may be called just before the widget becomes hidden or right after it is already hidden.
 *  @li@c  WGL_TKTP_NFY_COND_DESTROYED                       -This notification code indicates the condition that the Ticker Tape widget is going to be destroyed. Note that the notification function will always be called just before the widget is going to be destroyed.
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_TKTP_NFY_COND_T
{
    WGL_TKTP_NFY_COND_SCRL_END_REACHED   =   0,
    WGL_TKTP_NFY_COND_HIDDEN,
    WGL_TKTP_NFY_COND_DESTROYED
    
} WGL_TKTP_NFY_COND_T;    


/* Notification Function */
/*------------------------------------------------------------------*/
/*! @brief  This API is called by the Ticker Tape widget to inform the application about one of the following conditions: 
*                WGL_TKTP_NFY_COND_SCRL_END_REACHED: indicates the condition that each time when the Ticker Tape widget finishes presenting the whole message text by scrolling. Note that the notification function will be repeatedly called each time the end of the message text is reached when the text is scrolling.
*               WGL_TKTP_NFY_COND_HIDDEN: indicates the condition that the Ticker Tape widget becomes hidden. Note that the notification function may be called just before the widget becomes hidden or right after it is already hidden.
*                WGL_TKTP_NFY_COND_DESTROYED: indicates the condition that the Ticker Tape widget is going to be destroyed. Note that the notification function will always be called just before the widget is going to be destroyed. 
*                The condition codes listed above are defined in the enumeration WGL_TKTP_NFY_COND_T. A notification callback function can be register by the WGL_CMD_TKTP_REG_NFY command. Note that the handling procedure in the notify callback function is recommended to be as short as possible.
 *  @param [in]  h_tktp  Contains the handle of Ticker Tape widget instance in which the notification condition occurs.
 *  @param [in]   e_id_nfy    Indicates the notification condition under which the notification function is called. 
 *  @param [in]   pv_tag  Contains a tag value which is specified in the second argument pv_param2 of one of the following commands:
*                        WGL_CMD_TKTP_HIDE
*                        WGL_CMD_TKTP_DESTROY 
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*wgl_tktp_nfy_fct)(
                HANDLE_T                h_tktp,
                WGL_TKTP_NFY_COND_T     e_id_nfy,
                VOID                    *pv_tag);

    

/*-----------------------------------------------------------------------------
                    structures
----------------------------------------------------------------------------*/

/* Auto LoCation & SiZe (LCSZ) Control Structure */
/*------------------------------------------------------------------*/
/*! @struct WGL_TKTP_LCSZ_CTRL_T
 *  @brief This structure holds the parameters for the automatic relocation and resizing mechanism of the Ticker Tape widget. Note that this structure is effective only when the WGL_STL_TKTP_AUTO_LCSZ_CTRL widget style is specified.
 *  @code
 *  typedef struct _WGL_TKTP_LCSZ_CTRL_T
 *  {
 *      WGL_TKTP_ANCHOR_MODE_T          e_anchor;               
 *      GL_RECT_T                       t_rc_ref;               
 *  } WGL_TKTP_LCSZ_CTRL_T;    
 *  @endcode
 *  @li@c  e_anchor                                          - Contains the anchor mode. Refer to the WGL_TKTP_ANCHOR_MODE_T enumeration for more information.
 *  @li@c  t_rc_ref                                          -   Contains the rectangle of the virtual reference rectangle. 
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TKTP_LCSZ_CTRL_T
{
    WGL_TKTP_ANCHOR_MODE_T          e_anchor;               /*Anchor Mode*/
    GL_RECT_T                       t_rc_ref;               /*Virtual Reference Boundary*/
    
} WGL_TKTP_LCSZ_CTRL_T;    


/* Initialization Structure */
/*------------------------------------------------------------------*/
/*! @struct WGL_TKTP_INIT_T
 *  @brief This structure holds the initialization parameters for the creation of a Ticker Tape widget instance. The structure must be passed as the argument pv_param of the API x_wgl_create_widget. 
 *  @code
 *  typedef struct _WGL_TKTP_INIT_T
 *  {
 *      UINT32                          ui4_style;              
 *      WGL_TKTP_LCSZ_CTRL_T            t_lcsz_ctrl;            
 *  } WGL_TKTP_INIT_T;    
 *  @endcode
 *  @li@c  ui4_style                                         -Contains the widget styles . These styles can be bitwise or-ed together. 
 *  @li@c  t_lcsz_ctrl                                       -Contains the parameters for auto relocation and resizing mechanism. Note that this structure is valid only when the WGL_STL_TKTP_AUTO_LCSZ_CTRL widget style is specified in the field ui4_style. If the style is not specified, this field is ignored by the Ticker Tape widget.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TKTP_INIT_T
{
    UINT32                          ui4_style;              /*Styles*/
    WGL_TKTP_LCSZ_CTRL_T            t_lcsz_ctrl;            /*Location & Size Auto Control Parameters. Valid ONLY when WGL_STL_TKTP_AUTO_LS_CTRL is turned on*/
    
} WGL_TKTP_INIT_T;    



#ifdef __cplusplus
}
#endif


#endif
/*----------------------------------------------------------------------------*/
/*! @} */   /* end of groupMW_WGL_TKTP*/
/*----------------------------------------------------------------------------*/

