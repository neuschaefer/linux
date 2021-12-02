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
/*! @file u_wgl_common.h 
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
/*! @addtogroup groupMW_WGL
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

 
#ifndef _U_WGL_COMMON_H_
#define _U_WGL_COMMON_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_wgl.h"
#include "u_gl.h"
#include "u_fe.h"


#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*General Commands*/

/**Description
This command is used to store caller's private data in the widget.
Input arguments
pv_param1	Specifies the pointer to the private data that the caller wants to store. If it's NULL, 
it means to remove the old one. Note that this command only remove the pointer but not free the 
instance, and it's the caller's responsibility.
pv_param2	Reserved. It must be NULL.
Returns
WGLR_OK	The function call is successful.
*/
#define WGL_CMD_GL_SET_ATTACH_DATA      WGL_MAKE_GL_WIDGET_CMD(0)    

/**Description
This command is used to get the private data stored in the widget.
Input arguments
pv_param1	Pointer to a void* to receive the private data. If the return value is NULL, it means 
no attached data in this widget.
pv_param2	Reserved. It must be NULL.
Returns
WGLR_OK	The function call is successful.
WGLR_INV_ARG	The argument pv_param1 is NULL.
*/
#define WGL_CMD_GL_GET_ATTACH_DATA      WGL_MAKE_GL_WIDGET_CMD(1)    

/**Description
This command is used to set the widget's color attribute, and the target may be widget's 
background, foreground, string, or others.
Input arguments
pv_param1	Specify the target. About the available target.
pv_param2	Pointer to a WGL_COLOR_INFO_T. If it is NULL, it means to use the system's default 
setting.
Returns
WGLR_OK	The function call is successful.
WGLR_NOT_SUPPORT_TARGET	The receiver doesn't support the target specified by pv_param1.
Note
Widget classes may override this command. If it does, there will be additional descriptions for 
the extension.
*/
#define WGL_CMD_GL_SET_COLOR            WGL_MAKE_GL_WIDGET_CMD(2)    

/**Description
This command is used to get the widget's color attribute, and the source may be widget's 
background, foreground, string, or others.
Input arguments
pv_param1	Specify the source. About the available source.
pv_param2	Pointer to a WGL_COLOR_INFO_T to receive color data. It cannot be NULL.
Returns
WGLR_OK	The function call is successful.
WGLR_INV_ARG	The argument pv_param1 is invalid or pv_param2 is NULL.
WGLR_NOT_SUPPORT_TARGET	The receiver doesn't support the specified source.
Note
Widget classes may override this command. If it does, there will be some additional descriptions 
for the extension.
*/
#define WGL_CMD_GL_GET_COLOR            WGL_MAKE_GL_WIDGET_CMD(3)    

/**Description
This command is used to set the widget's font information.
Input arguments
pv_param1	Pointer to a WGL_FONT_INFO_T structure that contains font information. If it is NULL, 
it means to use the system default setting.
pv_param2	Reserved. It must be NULL.
Returns
WGLR_OK	The function call is successful.
WGLR_INV_ARG	One or more member of WGL_FONT_INFO_T is invalid.
*/
#define WGL_CMD_GL_SET_FONT             WGL_MAKE_GL_WIDGET_CMD(4)    

/**Description
This command is used to get widget's font information.
Input arguments
pv_param1	Pointer to a WGL_FONT_INFO_T variable to receive the information.
pv_param2	Reserved. It must be NULL.
Returns
WGLR_OK	The function call is successful.
WGLR_INV_ARG	The argument pv_param1 is NULL.
*/
#define WGL_CMD_GL_GET_FONT             WGL_MAKE_GL_WIDGET_CMD(5) 

/**Description
This command is used to set the widget's image attribute, and the target may be widget's 
background, foreground, or others.
Input arguments
pv_param1	Specify the target. For example:WGL_IMG_BK or WGL_IMG_FG 
pv_param2	Pointer to a WGL_IMG_INFO_T. If it is NULL, it means to use the system's default setting.
Returns
WGLR_OK	Function call is successful.
WGLR_INV_ARG	One or more member of WGL_IMG_INFO_T is invalid.
WGLR_NOT_SUPPORT_TARGET	The receiver doesn't support the target specified by pv_param1.
*/
#define WGL_CMD_GL_SET_IMAGE            WGL_MAKE_GL_WIDGET_CMD(6) 

/**Description
This command is used to get the widget's image attribute, and the source may be widget's 
background, foreground, or others.
Input arguments
pv_param1	Specify the source. For example:WGL_IMG_BK or WGL_IMG_FG 
pv_param2	Pointer to a WGL_IMG_INFO_T to receive image information. It cannot be NULL.
Returns
WGLR_OK	The function call is successful.
WGLR_INV_ARG	The argument pv_param2 is NULL.
WGLR_NOT_SUPPORT_TARGET	The receiver doesn't support the source specified by pv_param1.
WGLR_SYS_RC_NOT_INIT	This error is caused if the image setting of the command receiver is using 
system default image but the default image doesn't be initialized.
WGLR_INTERNAL_ERROR	This error is caused if the image setting of the command receiver is using 
system default image but the image handle or image data kept in widget library is invalid.
*/
#define WGL_CMD_GL_GET_IMAGE            WGL_MAKE_GL_WIDGET_CMD(7) 


/**Description
This command sets the text's reading direction attributes of the widget. The horizontal alignment 
in different direction actually works oppositely. The vertical alignment is not affected by text 
direction.

The default text direction is WGL_TEXT_DIR_LEFT_TO_RIGHT.
Input arguments
pv_param1	Specifies the text's reading direction. The supported directions are listed as follows: 
 	WGL_TEXT_DIR_LEFT_TO_RIGHT
 	WGL_TEXT_DIR_RIGHT_TO_LEFT

This argument must be of type UINT8.

pv_param2	Reserved. It must be NULL.
Returns
WGLR_OK	The function call is successful.
WGLR_INV_ARG	The argument pv_param1 is invalid.
*/
#define WGL_CMD_GL_SET_TEXT_DIRECTION   WGL_MAKE_GL_WIDGET_CMD(8)   

/**Description
This command is used to get the widget's text's reading direction attribute.
Input arguments
pv_param1	Returns the text direction information. The possible values are
 	WGL_TEXT_DIR_LEFT_TO_RIGHT
 	WGL_TEXT_DIR_RIGHT_TO_LEFT
This argument must be of type UINT8.

pv_param2	Reserved. It must be NULL.
Returns
WGLR_OK	The function call is successful.
WGLR_INV_ARG	The argument pv_param1 is invalid.
*/
#define WGL_CMD_GL_GET_TEXT_DIRECTION   WGL_MAKE_GL_WIDGET_CMD(9)   

/**Description
This command is used to set the widget's extend font information.
Input arguments
pv_param1	Pointer to a WGL_FONT_INFO_EX_T structure that contains extend font information. If it 
is NULL, it means to use the system default setting.
pv_param2	Reserved. It must be NULL.
Returns
WGLR_OK	The function call is successful.
WGLR_INV_ARG	One or more member of WGL_FONT_INFO_EX_T is invalid.
*/
#define WGL_CMD_GL_SET_FONT_EX          WGL_MAKE_GL_WIDGET_CMD(10)  

/**Description
This command is used to get widget's extend font information.

Input arguments
pv_param1	Pointer to a WGL_FONT_INFO_EX_T variable to receive the information.
pv_param2	Reserved. It must be NULL.

Returns
WGLR_OK	The function call is successful.
WGLR_INV_ARG	The argument pv_param1 is NULL.
*/
#define WGL_CMD_GL_GET_FONT_EX          WGL_MAKE_GL_WIDGET_CMD(11)  


/**Description
This command is used to set text attribute information.For Arab font reordering algorithm, there is one concept:
embedding level, this value will affect final reordering result and APP can set this value in some special case for 
example: string "abc***", '*' represents one abrab character,
if embedding level = 0 then reordering result is abc***,
if embedding level = 1 then reordering result is ***abc.
   
Input arguments
pv_param1	Pointer to a WGL_TEXT_ATTR_T variable to receive the information.
pv_param2	Reserved. 
Returns
WGLR_OK	The function call is successful.
WGLR_INV_ARG	The argument pv_param1 is NULL.
*/
#define WGL_CMD_GL_SET_TEXT_ATTR  WGL_MAKE_GL_WIDGET_CMD(12)

/**Description
This command is to get text attribute information.

Input arguments
pv_param1	Pointer to a WGL_TEXT_ATTR_T variable to receive the information.
pv_param2	Reserved. 
Returns
WGLR_OK	The function call is successful.
WGLR_INV_ARG	The argument pv_param1 is NULL.
*/
#define WGL_CMD_GL_GET_TEXT_ATTR  WGL_MAKE_GL_WIDGET_CMD(13)

/**Description
This command is to get the preferred size of icon or text size.

Output arguments
pv_param1	Point to the size in the struct GL_SIZE_T.
pv_param2	Reserved. 
Returns
WGLR_INV_ARG    The argument pv_param1 is NULL.
*/
#define WGL_CMD_GL_GET_PREFERRED_SIZE    WGL_MAKE_GL_WIDGET_CMD(14)  

/**Description
This command is to retrieve widget render condition.Just for inner implement use

Output arguments
pv_param1	UINT16 value indicates widget's paint state same as WGL_MSG_PAINT pv_param2. 
pv_param2	Point to UINT8 value, valid value: WGL_RENDER_TRANS,WGL_RENDER_SEMI_TRANS,WGL_RENDER_OPAQUE,WGL_RENDER_TRANS_OPAQUE.
Returns
WGLR_INV_ARG    The argument pv_param1 is NULL.
*/

#define WGL_CMD_GL_GET_RENDER_COND    WGL_MAKE_GL_WIDGET_CMD(15)  

#ifdef CLI_LVL_ALL
/**Description
This command is to retrieve widget allocated memory.Just for inner implement use

Output arguments
pv_param1	Point to UINT32 value, is output argument,will be set to widget's allocated memory size. 
pv_param2	Reserved
WGLR_INV_ARG    The argument pv_param1 is NULL.
*/
#define WGL_CMD_GL_GET_ALLOC_MEM    WGL_MAKE_GL_WIDGET_CMD(16)  
#endif

/*General Widget Style*/
#define WGL_STL_GL_NO_IMG_UI            ((UINT32) 0x00000001)     /**<If this style is assigned, it means the color sets are used to paint the widget's appearance; 
otherwise, the associated images are used.*/
#define WGL_STL_GL_NO_BK                ((UINT32) 0x00000002)     /**<If this style is assigned, the widget's background will be transparent. */
#define WGL_STL_GL_USE_SYS_IMG          ((UINT32) 0x00000004)     /**<If this style is assigned, the template image used for drawing will be default images while it is 
NULL_HANDLE.*/
#define WGL_STL_GL_BDR_FILL_CNT_BK      ((UINT32) 0x00000008)     /**<If this style is assigned, the border will draw its content's background.*/


/**In this style:WGL_STL_GL_FIX_TEXT_DIR,widget's text direction is fixed to WGL_TEXT_DIR_LEFT_TO_RIGHT
Command WGL_CMD_GL_SET_TEXT_DIRECTION and app's callback function registered through 
c_wgl_set_canvas_info_callback() will have no effect.*/
#define WGL_STL_GL_FIX_TEXT_DIR         ((UINT32) 0x00000010)            

/**In this style:WGL_STL_GL_FIX_TEXT_DIR_ALIGN,widget's text alignment on 
UI will not have reversed effect with user's setting even if text direction is right to left.
But text order will still take effect if WGL_STL_GL_FIX_TEXT_DIR is not set*/
#define WGL_STL_GL_FIX_TEXT_DIR_ALIGN   ((UINT32) 0x00000020)   

/**In this style:WGL_STL_GL_ANIM_AFFECT_CHILD,widget's Move,Scale or Alpha animation effect will affect its descendent. */
#define WGL_STL_GL_ANIM_AFFECT_CHILD   ((UINT32) 0x00000040)

/**In this style:WGL_STL_GL_FIX_COORDINATE_DIR,widget's horizontal coordinate is not affected by x_wgl_set_coordinate_reverse() */
#define WGL_STL_GL_FIX_COORDINATE_DIR   ((UINT32) 0x00000080)

/**Those widget styles for individual widget must start from this value, and they can be the same 
between different widget classes.*/
#define WGL_STL_WGT_SPF_START           ((UINT32) 0x00001000)    

/*Alignment*/
#define WGL_AS_LEFT_TOP                 ((UINT8) 0x11)    /**<Specify the foreground is left justified in the horizontal direction and top justified in the vertical direction.*/
#define WGL_AS_LEFT_CENTER              ((UINT8) 0x12)    /**<Specify the foreground is left justified in the horizontal direction and center justified in the vertical direction.*/
#define WGL_AS_LEFT_BOTTOM              ((UINT8) 0x14)    /**<Specify the foreground is left justified in the horizontal direction and bottom justified in thevertical direction.*/
#define WGL_AS_CENTER_TOP               ((UINT8) 0x21)    /**<Specify the foreground is center justified in horizontal direction and top justified in the vertical direction.*/
#define WGL_AS_CENTER_CENTER            ((UINT8) 0x22)    /**<Specify the foreground is center justified in horizontal direction and center justified in the vertical direction.*/
#define WGL_AS_CENTER_BOTTOM            ((UINT8) 0x24)    /**<Specify the foreground is center justified in horizontal direction and bottom justified in the vertical direction.*/
#define WGL_AS_RIGHT_TOP                ((UINT8) 0x41)    /**<Specify the foreground is right justified in horizontal direction and top justified in the vertical direction.*/
#define WGL_AS_RIGHT_CENTER             ((UINT8) 0x42)    /**<Specify the foreground is right justified in horizontal direction and center justified in the vertical direction.*/
#define WGL_AS_RIGHT_BOTTOM             ((UINT8) 0x44)    /**<Specify the foreground is right justified in horizontal direction and bottom justified in the vertical direction.*/

#define WGL_IS_VALID_ALIGN(_x)                                                                  \
    ( (_x)==WGL_AS_LEFT_TOP || (_x)==WGL_AS_LEFT_CENTER || (_x)==WGL_AS_LEFT_BOTTOM ||          \
      (_x)==WGL_AS_CENTER_TOP || (_x)==WGL_AS_CENTER_CENTER || (_x)==WGL_AS_CENTER_BOTTOM ||    \
      (_x)==WGL_AS_RIGHT_TOP || (_x)==WGL_AS_RIGHT_CENTER || (_x)==WGL_AS_RIGHT_BOTTOM          \
    )    /**< Judge alignment is valid or not.*/


/*Other*/
#define WGL_MAX_SEC_STR_BYTE            ((UINT16) 256)    /**< Widget max string byte. */

/*Text Direction*/
#define WGL_TEXT_DIR_LEFT_TO_RIGHT      ((UINT8)0x01)    /**< Left to right direction. */
#define WGL_TEXT_DIR_RIGHT_TO_LEFT      ((UINT8)0x02)    /**< right to left direction. */

typedef HANDLE_T WGL_HIMG_TPL_T;    /**<Template Image Handle*/

/*Color Command Target*/
#define WGL_CLR_BK                      ((UINT8) 0x01)    /**<It indicates the color for drawing widget's background. */
#define WGL_CLR_TEXT                    ((UINT8) 0x02)    /**<It indicates the color for drawing widget's text.*/
#define WGL_CLR_EDGE                    ((UINT8) 0x03)    /**< It indicates the color for drawing widget's edege. */
#define WGL_CLR_TEXT_BG                 ((UINT8) 0x04)    /**< It indicates the color for drawing widget's edege. */
#define WGL_CLR_WGT_SPF_START           ((UINT8) 0x80)    /**< For other widget specific template images, their index must be over WGL_IMG_WGT_SPF_START (
included), and widgets might use the same values with different explanations.*/
#define WGL_CLR_CUSTOM_START            ((UINT8) 0xF0)    /**<	For the customized contents, they can define their own indexes that are over 
WGL_IMG_CUSTOM_START (included).*/


/*Image Command Target*/
#define WGL_IMG_BK                      ((UINT8) 0x01)    /**<This definition indicates the template image for drawing widget's background.*/
#define WGL_IMG_FG                      ((UINT8) 0x02)    /**<This definition indicates the template image for drawing widget's foreground. */
#define WGL_IMG_WGT_SPF_START           ((UINT8) 0x80)    /**<For other widget specific template images, their index must be over WGL_IMG_WGT_SPF_START (
included), and widgets might use the same values with different explanations.*/
#define WGL_IMG_CUSTOM_START            ((UINT8) 0xF0)    /**<For the customized contents, they can define their own indexes that are over WGL_IMG_CUSTOM_START 
(included).*/

/*Widget Render Condition*/
#define WGL_RENDER_SEMI_TRANS                ((UINT8) 0x00)    /**<This definition indicates the widget's background or foreground is semi transparent,for example, widget's background color 's alpha is >0 and <255. */
#define WGL_RENDER_TRANS                     ((UINT8) 0x01)    /**<This definition indicates the widget's background and foreground is transparent which means the widget have no ui.*/
#define WGL_RENDER_TRANS_OPAQUE              ((UINT8) 0x02)    /**<This definition indicates the widget's some region is transparent and other region has opaque image,color or text, for example, text widget has no background and has string content.*/ 
#define WGL_RENDER_OPAQUE                    ((UINT8) 0x03)    /**<This definition indicates the widget's region has opaque ui, for example, text widget has background color with alpha is 255 and has string content*/

/*System Template Image ID*/
#define WGL_SYS_IMG_FM_BK               ((UINT16) 0)    /**<Index of Frame's background image in enabled state (enabled and not focused). */
#define WGL_SYS_IMG_FM_BK_DIS           ((UINT16) 1)    /**<Index of Frame's background image in disabled state.*/
#define WGL_SYS_IMG_FM_BK_HT            ((UINT16) 2)    /**<Index of Frame's background image in highlighted state.*/
#define WGL_SYS_IMG_STATIC              ((UINT16) 3)    /**<Index of background image of Icon Static and Text Static in enabled state (enabled and not focused).*/
#define WGL_SYS_IMG_STATIC_DIS          ((UINT16) 4)    /**<Index of background image of Icon Static and Text Static in disabled state. */
#define WGL_SYS_IMG_STATIC_HT           ((UINT16) 5)    /**<Index of background image of Icon Static and Text Static in highlighted state.*/
#define WGL_SYS_IMG_BTN                 ((UINT16) 6)    /**<Index of Button's background image in enabled state (enabled, not focused, and not pushed).*/
#define WGL_SYS_IMG_BTN_DIS             ((UINT16) 7)    /**<Index of Button's background image in disabled state.*/
#define WGL_SYS_IMG_BTN_HT              ((UINT16) 8)    /**<Index of Button's background image in highlighted state */
#define WGL_SYS_IMG_BTN_PUSHED          ((UINT16) 9)    /**<Index of Button's background image in pushed state.*/
#define WGL_SYS_IMG_BTN_PUSHED_UNHT     ((UINT16) 10)    /**<Index of Button's background image in pushed state(unhightlight) */
#define WGL_SYS_IMG_PG_BK               ((UINT16) 11)    /**<Index of ProgressBar's background image in enabled state (enabled and not focused). */
#define WGL_SYS_IMG_PG_BK_DIS           ((UINT16) 12)    /**<Index of ProgressBar's background image in disabled state.*/
#define WGL_SYS_IMG_PG_BK_HT            ((UINT16) 13)    /**<Index of ProgressBar's background image in highlighted state. */
#define WGL_SYS_IMG_PG_BODY             ((UINT16) 14)    /**<Index of horizontal ProgressBar's body image in enabled state.*/
#define WGL_SYS_IMG_PG_BODY_DIS         ((UINT16) 15)    /**<Index of horizontal ProgressBar's body image in disabled state.*/
#define WGL_SYS_IMG_PG_BODY_HT          ((UINT16) 16)    /**<Index of horizontal ProgressBar's body image in highlighted state.*/
#define WGL_SYS_IMG_PG_LEVEL1           ((UINT16) 17)    /**<Index of horizontal ProgressBar's level 1 image in enabled state.*/
#define WGL_SYS_IMG_PG_LEVEL1_DIS       ((UINT16) 18)    /**<Index of horizontal ProgressBar's level 1 image in disabled state.*/
#define WGL_SYS_IMG_PG_LEVEL1_HT        ((UINT16) 19)    /**<Index of horizontal ProgressBar's level 1 image in highlighted state.*/
#define WGL_SYS_IMG_PG_LEVEL2           ((UINT16) 20)    /**<Index of horizontal ProgressBar's level 2 image in enabled state.*/
#define WGL_SYS_IMG_PG_LEVEL2_DIS       ((UINT16) 21)    /**<Index of horizontal ProgressBar's level 2 image in disabled state.*/
#define WGL_SYS_IMG_PG_LEVEL2_HT        ((UINT16) 22)    /**<Index of horizontal ProgressBar's level 2 image in highlighted state.*/
#define WGL_SYS_IMG_PG_LEVEL3           ((UINT16) 23)    /**<Index of horizontal ProgressBar's level 3 image in enabled state.*/
#define WGL_SYS_IMG_PG_LEVEL3_DIS       ((UINT16) 24)    /**<Index of horizontal ProgressBar's level 3 image in disabled state.*/
#define WGL_SYS_IMG_PG_LEVEL3_HT        ((UINT16) 25)    /**<Index of horizontal ProgressBar's level 3 image in highlighted state.*/
#define WGL_SYS_IMG_PG_LEVEL4           ((UINT16) 26)    /**<Index of horizontal ProgressBar's level 4 image in enabled state.*/
#define WGL_SYS_IMG_PG_LEVEL4_DIS       ((UINT16) 27)    /**<Index of horizontal ProgressBar's level 4 image in disabled state.*/
#define WGL_SYS_IMG_PG_LEVEL4_HT        ((UINT16) 28)    /**<Index of horizontal ProgressBar's level 4 image in highlighted state.*/
#define WGL_SYS_IMG_PG_LEVEL5           ((UINT16) 29)    /**<Index of horizontal ProgressBar's level 5 image in enabled state.*/
#define WGL_SYS_IMG_PG_LEVEL5_DIS       ((UINT16) 30)    /**<Index of horizontal ProgressBar's level 5 image in disabled state.*/
#define WGL_SYS_IMG_PG_LEVEL5_HT        ((UINT16) 31)    /**<Index of horizontal ProgressBar's level 5 image in highlighted state.*/
#define WGL_SYS_IMG_PG_V_BODY           ((UINT16) 32)    /**<Index of vertical ProgressBar's body image in enabled state.*/
#define WGL_SYS_IMG_PG_V_BODY_DIS       ((UINT16) 33)    /**<Index of vertical ProgressBar's body image in disabled state.*/
#define WGL_SYS_IMG_PG_V_BODY_HT        ((UINT16) 34)    /**<Index of vertical ProgressBar's body image in highlighted state.*/
#define WGL_SYS_IMG_PG_V_POINT          ((UINT16) 35)    /**<        */
#define WGL_SYS_IMG_PG_V_POINT_DIS      ((UINT16) 36)    /**<        */
#define WGL_SYS_IMG_PG_V_POINT_HT       ((UINT16) 37)    /**<        */
#define WGL_SYS_IMG_PG_V_LEVEL1         ((UINT16) 38)    /**<        */
#define WGL_SYS_IMG_PG_V_LEVEL1_DIS     ((UINT16) 39)    /**<        */
#define WGL_SYS_IMG_PG_V_LEVEL1_HT      ((UINT16) 40)    /**<        */
#define WGL_SYS_IMG_PG_V_LEVEL2         ((UINT16) 41)    /**<        */
#define WGL_SYS_IMG_PG_V_LEVEL2_DIS     ((UINT16) 42)    /**<        */
#define WGL_SYS_IMG_PG_V_LEVEL2_HT      ((UINT16) 43)    /**<        */
#define WGL_SYS_IMG_PG_V_LEVEL3         ((UINT16) 44)    /**<        */
#define WGL_SYS_IMG_PG_V_LEVEL3_DIS     ((UINT16) 45)    /**<        */
#define WGL_SYS_IMG_PG_V_LEVEL3_HT      ((UINT16) 46)    /**<        */
#define WGL_SYS_IMG_PG_V_LEVEL4         ((UINT16) 47)    /**<        */
#define WGL_SYS_IMG_PG_V_LEVEL4_DIS     ((UINT16) 48)    /**<        */
#define WGL_SYS_IMG_PG_V_LEVEL4_HT      ((UINT16) 49)    /**<        */
#define WGL_SYS_IMG_PG_V_LEVEL5         ((UINT16) 50)    /**<        */
#define WGL_SYS_IMG_PG_V_LEVEL5_DIS     ((UINT16) 51)    /**<        */
#define WGL_SYS_IMG_PG_V_LEVEL5_HT      ((UINT16) 52)    /**<        */
#define WGL_SYS_IMG_EB                  ((UINT16) 53)    /**<Index of EditBox's background image in enabled state*/
#define WGL_SYS_IMG_EB_DIS              ((UINT16) 54)    /**<Index of EditBox's background image in disabled state.*/
#define WGL_SYS_IMG_EB_HT               ((UINT16) 55)    /**<Index of EditBox's background image in highlighted state.*/

#define WGL_SYS_IMG_LB                      ((UINT16) 56)    /**<        */
#define WGL_SYS_IMG_LB_DIS                  ((UINT16) 57)    /**<        */
#define WGL_SYS_IMG_LB_HT                   ((UINT16) 58)    /**<        */
#define WGL_SYS_IMG_LB_ELEM                 ((UINT16) 59)    /**<        */
#define WGL_SYS_IMG_LB_ELEM_DIS             ((UINT16) 60)    /**<        */
#define WGL_SYS_IMG_LB_ELEM_HT              ((UINT16) 61)    /**<        */
#define WGL_SYS_IMG_LB_ELEM_HT_UNFOCUS      ((UINT16) 62)    /**<        */
#define WGL_SYS_IMG_LB_ELEM_PUSHED          ((UINT16) 63)    /**<        */
#define WGL_SYS_IMG_LB_ELEM_SEL             ((UINT16) 64)    /**<        */
#define WGL_SYS_IMG_LB_ELEM_SEL_DIS         ((UINT16) 65)    /**<        */
#define WGL_SYS_IMG_PG_CURSOR               ((UINT16) 66)    /**<        */
#define WGL_SYS_IMG_PG_CURSOR_DIS           ((UINT16) 67)    /**<        */
#define WGL_SYS_IMG_PG_CURSOR_HT            ((UINT16) 68)    /**<        */
#define WGL_SYS_IMG_PG_V_CURSOR             ((UINT16) 69)    /**<        */
#define WGL_SYS_IMG_PG_V_CURSOR_DIS         ((UINT16) 70)    /**<        */
#define WGL_SYS_IMG_PG_V_CURSOR_HT          ((UINT16) 71)    /**<        */



#define WGL_SYS_IMG_MIN                 ((UINT16) WGL_SYS_IMG_FM_BK)    /**<        */
#define WGL_SYS_IMG_MAX                 ((UINT16) WGL_SYS_IMG_PG_V_CURSOR_HT)    /**<        */

#define WGL_SYS_IMG_COUNT               ((UINT16) WGL_SYS_IMG_MAX + 1)    /**<        */

/*System Color-Set ID*/
#define WGL_SYS_CLR_TEXT                ((UINT16) 0)    /**<Index of string color for Text Static, ProgressBar, Button, and EditBox in enabled state (enabled and not focused). */
#define WGL_SYS_CLR_TEXT_DIS            ((UINT16) 1)    /**<Index of string color for Text Static, ProgressBar, Button, and EditBox in disabled state.*/
#define WGL_SYS_CLR_TEXT_HT             ((UINT16) 2)    /**<Index of string color for Text Static, ProgressBar, Button, and EditBox in highlighted state.*/
#define WGL_SYS_CLR_TEXT_PUSHED         ((UINT16) 3)    /**<Index of string color for Button in pushed state.*/
#define WGL_SYS_CLR_TEXT_PUSHED_UNHT    ((UINT16) 4)    /**< Index of string color for Button in pushed state(unhightlight).*/
#define WGL_SYS_CLR_FM_BK               ((UINT16) 5)    /**<Index of Frame's background color in enabled state (enabled and not focused).*/
#define WGL_SYS_CLR_FM_BK_DIS           ((UINT16) 6)    /**<Index of Frame's background color in disabled state.*/
#define WGL_SYS_CLR_FM_BK_HT            ((UINT16) 7)    /**<Index of Frame's background color in highlighted state.*/
#define WGL_SYS_CLR_STATIC              ((UINT16) 8)    /**<Index of background color of Icon Static and Text Static in enabled state (enabled and not focused).*/
#define WGL_SYS_CLR_STATIC_DIS          ((UINT16) 9)    /**<Index of background color of Icon Static and Text Static in disabled state.*/
#define WGL_SYS_CLR_STATIC_HT           ((UINT16) 10)    /**<Index of background color of Icon Static and Text Static in highlighted state.*/
#define WGL_SYS_CLR_BTN                 ((UINT16) 11)    /**<Index of Button's background color in enabled state (enabled, not focused, and not pushed).*/
#define WGL_SYS_CLR_BTN_DIS             ((UINT16) 12)    /**<Index of Button's background color in disabled state..*/
#define WGL_SYS_CLR_BTN_HT              ((UINT16) 13)    /**<Index of Button's background color in highlighted state (highlighted and not pushed).*/
#define WGL_SYS_CLR_BTN_PUSHED          ((UINT16) 14)    /**<Index of Button's background color in pushed state.*/
#define WGL_SYS_CLR_BTN_PUSHED_UNHT     ((UINT16) 15)    /**<Index of Button's background color in pushed state(unhightlight).*/
#define WGL_SYS_CLR_PG_BK               ((UINT16) 16)    /**<Index of ProgressBar's background color in enabled state (enabled and not focused).*/
#define WGL_SYS_CLR_PG_BK_DIS           ((UINT16) 17)    /**<Index of ProgressBar's background color in disabled state*/
#define WGL_SYS_CLR_PG_BK_HT            ((UINT16) 18)    /**<Index of ProgressBar's background color in highlighted state */
#define WGL_SYS_CLR_PG_BODY             ((UINT16) 19)    /**<Index of ProgressBar's body color in enabled state*/
#define WGL_SYS_CLR_PG_BODY_DIS         ((UINT16) 20)    /**<Index of ProgressBar's body color in disabled state*/
#define WGL_SYS_CLR_PG_BODY_HT          ((UINT16) 21)    /**<Index of ProgressBar's body color in highlighted state*/
#define WGL_SYS_CLR_PG_LEVEL1           ((UINT16) 22)    /**<Index of ProgressBar's level 1 color in enabled state.*/
#define WGL_SYS_CLR_PG_LEVEL1_DIS       ((UINT16) 23)    /**<Index of ProgressBar's level 1 color in disabled state*/
#define WGL_SYS_CLR_PG_LEVEL1_HT        ((UINT16) 24)    /**<Index of ProgressBar's level 1 color in highlighted state*/
#define WGL_SYS_CLR_PG_LEVEL2           ((UINT16) 25)    /**<Index of ProgressBar's level 2 color in enabled state.*/
#define WGL_SYS_CLR_PG_LEVEL2_DIS       ((UINT16) 26)    /**<Index of ProgressBar's level 2 color in disabled state*/
#define WGL_SYS_CLR_PG_LEVEL2_HT        ((UINT16) 27)    /**<Index of ProgressBar's level 2 color in highlighted state*/
#define WGL_SYS_CLR_PG_LEVEL3           ((UINT16) 28)    /**<Index of ProgressBar's level 3 color in enabled state.*/
#define WGL_SYS_CLR_PG_LEVEL3_DIS       ((UINT16) 29)    /**<Index of ProgressBar's level 3 color in disabled state*/
#define WGL_SYS_CLR_PG_LEVEL3_HT        ((UINT16) 30)    /**<Index of ProgressBar's level 3 color in highlighted state*/
#define WGL_SYS_CLR_PG_LEVEL4           ((UINT16) 31)    /**<Index of ProgressBar's level 4 color in enabled state.*/
#define WGL_SYS_CLR_PG_LEVEL4_DIS       ((UINT16) 32)    /**<Index of ProgressBar's level 4 color in disabled state*/
#define WGL_SYS_CLR_PG_LEVEL4_HT        ((UINT16) 33)    /**<Index of ProgressBar's level 4 color in highlighted state.*/
#define WGL_SYS_CLR_PG_LEVEL5           ((UINT16) 34)    /**<Index of ProgressBar's level 5 color in enabled state.*/
#define WGL_SYS_CLR_PG_LEVEL5_DIS       ((UINT16) 35)    /**<Index of ProgressBar's level 5 color in disabled state.*/
#define WGL_SYS_CLR_PG_LEVEL5_HT        ((UINT16) 36)    /**<Index of ProgressBar's level 5 color in highlighted state */
#define WGL_SYS_CLR_EB                  ((UINT16) 37)    /**<Index of EditBox's background color in enabled state.*/
#define WGL_SYS_CLR_EB_DIS              ((UINT16) 38)    /**<Index of EditBox's background color in disabled state*/
#define WGL_SYS_CLR_EB_HT               ((UINT16) 39)    /**<Index of EditBox's background color in highlighted state. */

#define WGL_SYS_CLR_LB_FG                   ((UINT16) 40)    /**<Index of the system default color set for listbox foreground in enable state*/
#define WGL_SYS_CLR_LB_FG_DIS               ((UINT16) 41)    /**<Index of the system default color set for listbox foreground in disable state */
#define WGL_SYS_CLR_LB_FG_HT                ((UINT16) 42)    /**<Index of the system default color set for listbox foreground in highlight state*/
#define WGL_SYS_CLR_LB_BK                   ((UINT16) 43)    /**<Index of the system default color set for listbox background in enable state*/
#define WGL_SYS_CLR_LB_BK_DIS               ((UINT16) 44)    /**<Index of the system default color set for listbox background in disable state*/
#define WGL_SYS_CLR_LB_BK_HT                ((UINT16) 45)    /**<Index of the system default color set for listbox background in highlight state*/
#define WGL_SYS_CLR_LB_VP                   ((UINT16) 46)    /**<Index of the system default color set for viewport in enable state*/
#define WGL_SYS_CLR_LB_VP_DIS               ((UINT16) 47)    /**<Index of the system default color set for viewport in disable state */
#define WGL_SYS_CLR_LB_VP_HT                ((UINT16) 48)    /**<Index of the system default color set for viewport in highlight state*/
#define WGL_SYS_CLR_LB_ELEM                 ((UINT16) 49)    /**<Index of the system default color set for listbox element in enable state*/
#define WGL_SYS_CLR_LB_ELEM_DIS             ((UINT16) 50)    /**<Index of the system default color set for listbox element in disable state*/
#define WGL_SYS_CLR_LB_ELEM_HT              ((UINT16) 51)    /**<Index of the system default color set for listbox element in highlight state*/
#define WGL_SYS_CLR_LB_ELEM_HT_UNFOCUS      ((UINT16) 52)    /**<Index of the system default color set for listbox element in highlight but unfocus state*/
#define WGL_SYS_CLR_LB_ELEM_PUSHED          ((UINT16) 53)    /**<Index of the system default color set for listbox element in pushed state */
#define WGL_SYS_CLR_LB_ELEM_SEL             ((UINT16) 54)    /**<Index of the system default color set for listbox element in selected state*/
#define WGL_SYS_CLR_LB_ELEM_SEL_DIS         ((UINT16) 55)    /**<Index of the system default color set for listbox element in selected but disable state*/

#define WGL_SYS_CLR_LB_ELEM_TEXT            ((UINT16) 56)    /**<        */
#define WGL_SYS_CLR_LB_ELEM_TEXT_DIS        ((UINT16) 57)    /**<        */
#define WGL_SYS_CLR_LB_ELEM_TEXT_HT         ((UINT16) 58)    /**<        */
#define WGL_SYS_CLR_LB_ELEM_TEXT_HT_UNFOCUS ((UINT16) 59)    /**<        */
#define WGL_SYS_CLR_LB_ELEM_TEXT_PUSHED     ((UINT16) 60)    /**<        */
#define WGL_SYS_CLR_LB_ELEM_TEXT_SEL        ((UINT16) 61)    /**<        */
#define WGL_SYS_CLR_LB_ELEM_TEXT_SEL_DIS    ((UINT16) 62)    /**<        */

#define WGL_SYS_CLR_HTS_BK                  ((UINT16) 63)    /**<        */
#define WGL_SYS_CLR_HTS_BK_DIS              ((UINT16) 64)    /**<        */
#define WGL_SYS_CLR_HTS_BK_HT               ((UINT16) 65)    /**<        */
#define WGL_SYS_CLR_HTS_VP                  ((UINT16) 66)    /**<        */
#define WGL_SYS_CLR_HTS_VP_DIS              ((UINT16) 67)    /**<        */
#define WGL_SYS_CLR_HTS_VP_HT               ((UINT16) 68)    /**<        */
#define WGL_SYS_CLR_HTS_TEXT                ((UINT16) 69)    /**<        */
#define WGL_SYS_CLR_HTS_TEXT_BK             ((UINT16) 70)    /**<        */

#define WGL_SYS_CLR_TKTP_BK                 ((UINT16) 71)    /**<        */
#define WGL_SYS_CLR_TKTP_MSG_FG             ((UINT16) 72)    /**<        */
#define WGL_SYS_CLR_TKTP_MSG_BK             ((UINT16) 73)    /**<        */

#define WGL_SYS_CLR_BDR_FG                  ((UINT16) 74)    /**<        */
#define WGL_SYS_CLR_BDR_FG_DIS              ((UINT16) 75)    /**<        */
#define WGL_SYS_CLR_BDR_FG_HT               ((UINT16) 76)    /**<        */
#define WGL_SYS_CLR_BDR_FG_HT_INACT         ((UINT16) 77)    /**<        */
#define WGL_SYS_CLR_BDR_FG_PUSHED           ((UINT16) 78)    /**<        */
#define WGL_SYS_CLR_BDR_FG_PUSHED_UNHT      ((UINT16) 79)    /**<        */
#define WGL_SYS_CLR_BDR_BK                  ((UINT16) 80)    /**<        */
#define WGL_SYS_CLR_BDR_BK_DIS              ((UINT16) 81)    /**<        */
#define WGL_SYS_CLR_BDR_BK_HT               ((UINT16) 82)    /**<        */
#define WGL_SYS_CLR_BDR_BK_HT_INACT         ((UINT16) 83)    /**<        */
#define WGL_SYS_CLR_BDR_BK_PUSHED           ((UINT16) 84)    /**<        */
#define WGL_SYS_CLR_BDR_BK_PUSHED_UNHT      ((UINT16) 85)    /**<        */

#define WGL_SYS_CLR_MIN                 ((UINT16) WGL_SYS_CLR_TEXT)    /**<        */
#define WGL_SYS_CLR_MAX                 ((UINT16) WGL_SYS_CLR_BDR_BK_PUSHED_UNHT)    /**<        */
#define WGL_SYS_CLR_COUNT               ((UINT16) WGL_SYS_CLR_MAX + 1)    /**<        */

/*widget appearance limitation*/
#define WGL_MAX_WIDGET_WIDTH            5000    /**<        */
#define WGL_MAX_WIDGET_HEIGHT           5000    /**<        */

/*font name limitation*/
#define WGL_MAX_FONT_NAME               32    /**<        */

/*Segment Type*/
/*------------------------------------------------------------------*/
/*! @enum WGL_IMG_ST_T
 *  @brief Define the image style.
 *  @code
 *  typedef enum _WGL_IMG_ST_T
 *  {
 *          WGL_IMG_ST_H_ONLY       = 1,
 *          WGL_IMG_ST_V_ONLY,
 *          WGL_IMG_ST_HV,
 *          WGL_IMG_ST_NO_SEGMENT
 *  } WGL_IMG_ST_T;
 *  @endcode
 *  @li@c  WGL_IMG_ST_H_ONLY                                 -Segment the template image in x-axis
 *  @li@c  WGL_IMG_ST_V_ONLY                                 -Segment the template image in y-axis
 *  @li@c  WGL_IMG_ST_HV                                     -Segment the template image both in x-axis and y axis
 *  @li@c  WGL_IMG_ST_NO_SEGMENT                             -Don't segment the template image
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_IMG_ST_T
{
        WGL_IMG_ST_H_ONLY       = 1,
        WGL_IMG_ST_V_ONLY,
        WGL_IMG_ST_HV,
        WGL_IMG_ST_NO_SEGMENT
} WGL_IMG_ST_T;

/*Color-Set Type*/
/*------------------------------------------------------------------*/
/*! @enum WGL_COLOR_SET_TYPE_T
 *  @brief Define color set type.
 *  @code
 *  typedef enum
 *  {
 *          WGL_COLOR_SET_BASIC = 0,
 *          WGL_COLOR_SET_STANDARD,
 *          WGL_COLOR_SET_EXTEND
 *  } WGL_COLOR_SET_TYPE_T;
 *  @endcode
 *  @li@c  WGL_COLOR_SET_BASIC                     -Indicate that the color set is WGL_COLOR_BASIC_SET_T structure
 *  @li@c  WGL_COLOR_SET_STANDARD             -Indicate that the color set is WGL_COLOR_STD_SET_T structure
 *  @li@c  WGL_COLOR_SET_EXTEND                  -Indicate that the color set is WGL_COLOR_EXT_SET_T structure
 */
/*------------------------------------------------------------------*/
typedef enum
{
        WGL_COLOR_SET_BASIC = 0,
        WGL_COLOR_SET_STANDARD,
        WGL_COLOR_SET_EXTEND
} WGL_COLOR_SET_TYPE_T;

/*Image-Set Type*/
/*------------------------------------------------------------------*/
/*! @enum WGL_IMG_SET_TYPE_T
 *  @brief Define the image set type.
 *  @code
 *  typedef enum _WGL_IMG_SET_TYPE_T
 *  {
 *          WGL_IMG_SET_BASIC = 0,
 *          WGL_IMG_SET_STANDARD,
 *          WGL_IMG_SET_EXTEND
 *  } WGL_IMG_SET_TYPE_T;
 *  @endcode
 *  @li@c  WGL_IMG_SET_BASIC                                 -Indicate that the image set is WGL_IMG_BASIC_SET_T structure
 *  @li@c  WGL_IMG_SET_STANDARD                              -Indicate that the image set is WGL_IMG_STD_SET_T structure
 *  @li@c  WGL_IMG_SET_EXTEND                                -Indicate that the image set is WGL_IMG_EXT_SET_T structure
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_IMG_SET_TYPE_T
{
        WGL_IMG_SET_BASIC = 0,
        WGL_IMG_SET_STANDARD,
        WGL_IMG_SET_EXTEND
} WGL_IMG_SET_TYPE_T;

/*Font-Set Type*/
/*------------------------------------------------------------------*/
/*! @enum WGL_FONT_SET_TYPE_T
 *  @brief Define the font set type.
 *  @code
 *  typedef enum _WGL_FONT_SET_TYPE_T
 *  {
 *          WGL_FONT_SET_BASIC = 0
 *  } WGL_FONT_SET_TYPE_T;
 *  @endcode
 *  @li@c  WGL_FONT_SET_BASIC                                -Indicate that the font set is WGL_FONT_BASIC_SET_T structure
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_FONT_SET_TYPE_T
{
        WGL_FONT_SET_BASIC = 0
} WGL_FONT_SET_TYPE_T;


#define WGL_TEXT_ATTR_TYPE_T           UINT32    /**<Font attribute type.*/
#define WGL_TEXT_ATTR_REORDER    ((WGL_TEXT_ATTR_TYPE_T) 0)  /**<ARTTR value:UINT8 embedding level*/


#define IS_LEGAL_SYS_IMG_ID(ui2_img_id)                                   \
        ((ui2_img_id) <= WGL_SYS_IMG_MAX)    /**<Judge is system image id or not. */

#define IS_LEGAL_SYS_CLR_ID(ui2_color_id)                                 \
        ((ui2_color_id) <= WGL_SYS_CLR_MAX)    /**<        */

#define IS_LEGAL_CLR_SET_TYPE(e_clr_set_type)                             \
        (((INT32)(e_clr_set_type))>=0 && ((INT32)(e_clr_set_type))<=2)    /**<        */

#define IS_LEGAL_IMG_SET_TYPE(e_img_set_type)                             \
        (((INT32)(e_img_set_type))>=0 && ((INT32)(e_img_set_type))<=2)    /**<        */


#define WGL_COPY_INSET(pt_dest_inset, pt_src_inset)                       \
        {                                                                 \
                (pt_dest_inset)->i4_left = (pt_src_inset)->i4_left;       \
                (pt_dest_inset)->i4_right = (pt_src_inset)->i4_right;     \
                (pt_dest_inset)->i4_top = (pt_src_inset)->i4_top;         \
                (pt_dest_inset)->i4_bottom = (pt_src_inset)->i4_bottom;   \
        }    /**<        */

#define WGL_IS_H_LEFT(ui1_align)                                          \
        ((ui1_align) & ((UINT8)0x10))    /**<        */

#define WGL_IS_H_CENTER(ui1_align)                                        \
        ((ui1_align) & ((UINT8)0x20))    /**<        */

#define WGL_IS_H_RIGHT(ui1_align)                                         \
        ((ui1_align) & ((UINT8)0x40))    /**<        */

#define WGL_IS_V_TOP(ui1_align)                                           \
        ((ui1_align) & ((UINT8)0x01))    /**<        */

#define WGL_IS_V_CENTER(ui1_align)                                        \
        ((ui1_align) & ((UINT8)0x02))    /**<        */

#define WGL_IS_V_BOTTOM(ui1_align)                                        \
        ((ui1_align) & ((UINT8)0x04))    /**<        */


#define WGL_MAX_UPDATE_RECT_NUM     (12)

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
/*Color-Sets*/
/*------------------------------------------------------------------*/
/*! @struct WGL_COLOR_BASIC_SET_T
 *  @brief It is the basic structure that contains two colors for enabled and disabled state
 *  @code
 *  typedef struct _WGL_COLOR_BASIC_SET_T
 *  {
 *          GL_COLOR_T              t_enable;
 *          GL_COLOR_T              t_disable;
 *  } WGL_COLOR_BASIC_SET_T;
 *  @endcode
 *  @li@c  t_enable                                          -Specify the color used in enabled state
 *  @li@c  t_disable                                         -Specify the color used in disabled state
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_COLOR_BASIC_SET_T
{
        GL_COLOR_T              t_enable;
        GL_COLOR_T              t_disable;
} WGL_COLOR_BASIC_SET_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_COLOR_STD_SET_T
 *  @brief It is the standard structure that contains three colors for enabled, disabled, and highlight state.
 *  @code
 *  typedef struct _WGL_COLOR_STD_SET_T
 *  {
 *          GL_COLOR_T              t_enable;
 *          GL_COLOR_T              t_disable;
 *          GL_COLOR_T              t_highlight;
 *  } WGL_COLOR_STD_SET_T;
 *  @endcode
 *  @li@c  t_enable                                          -Specify the color for enabled state
 *  @li@c  t_disable                                         -Specify the color for disable state
 *  @li@c  t_highlight                                       -Specify the color for highlight state
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_COLOR_STD_SET_T
{
        GL_COLOR_T              t_enable;
        GL_COLOR_T              t_disable;
        GL_COLOR_T              t_highlight;
} WGL_COLOR_STD_SET_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_COLOR_EXT_SET_T
 *  @brief It is the extended structure that contains five colors for enabled, disabled, highlight, highlight
-inactive, and pushed state.
 *  @code
 *  typedef struct _WGL_COLOR_EXT_SET_T
 *  {
 *          GL_COLOR_T              t_enable;
 *          GL_COLOR_T              t_disable;
 *          GL_COLOR_T              t_highlight;
 *          GL_COLOR_T              t_highlight_inactive;
 *          GL_COLOR_T              t_push;
 *          GL_COLOR_T              t_push_unhighlight;
 *  } WGL_COLOR_EXT_SET_T;
 *  @endcode
 *  @li@c  t_enable                                          -Specify the color for enabled state
 *  @li@c  t_disable                                         -Specify the color for disable state
 *  @li@c  t_highlight                                       -Specify the color for enabled state
 *  @li@c  t_highlight_inactive                              -Specify the color for highlight but inactive  state
 *  @li@c  t_push                                            -Specify the color for push state
 *  @li@c  t_push_unhighlight                                -Specify the color for push but unhighlight state
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_COLOR_EXT_SET_T
{
        GL_COLOR_T              t_enable;
        GL_COLOR_T              t_disable;
        GL_COLOR_T              t_highlight;
        GL_COLOR_T              t_highlight_inactive;
        GL_COLOR_T              t_push;
        GL_COLOR_T              t_push_unhighlight;
} WGL_COLOR_EXT_SET_T;

/*Color Infomation*/
/*------------------------------------------------------------------*/
/*! @struct WGL_COLOR_INFO_T
 *  @brief This struct contains widget color information.
 *  @code
 *  typedef struct _WGL_COLOR_INFO_T
 *  {
 *          WGL_COLOR_SET_TYPE_T            e_type;
 *          union
 *          {
 *                  WGL_COLOR_BASIC_SET_T   t_basic;
 *                  WGL_COLOR_STD_SET_T     t_standard;
 *                  WGL_COLOR_EXT_SET_T     t_extend;
 *          } u_color_data;
 *  } WGL_COLOR_INFO_T;
 *  @endcode
 *  @li@c  e_type                                            -Specify the color-set format this structure contains
 *  @li@c  t_basic                                           -Contains basic color information.
 *  @li@c  t_standard                                        -Contains standard color information.
 *  @li@c  t_extend                                          -Contains extend color information.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_COLOR_INFO_T
{
        WGL_COLOR_SET_TYPE_T            e_type;
        union
        {
                WGL_COLOR_BASIC_SET_T   t_basic;
                WGL_COLOR_STD_SET_T     t_standard;
                WGL_COLOR_EXT_SET_T     t_extend;
        } u_color_data;
} WGL_COLOR_INFO_T;

/*Image Sets*/
/*------------------------------------------------------------------*/
/*! @struct WGL_IMG_BASIC_SET_T
 *  @brief It is the basic structure that contains two template images for enabled and disabled state
 *  @code
 *  typedef struct _WGL_IMG_BASIC_SET_T
 *  {
 *          WGL_HIMG_TPL_T          t_enable;
 *          WGL_HIMG_TPL_T          t_disable;
 *  } WGL_IMG_BASIC_SET_T;
 *  @endcode
 *  @li@c  t_enable                                          -Specify the template image for enabled state
 *  @li@c  t_disable                                         -Specify the template image for disabled state
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_IMG_BASIC_SET_T
{
        WGL_HIMG_TPL_T          t_enable;
        WGL_HIMG_TPL_T          t_disable;
} WGL_IMG_BASIC_SET_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_IMG_STD_SET_T
 *  @brief It is the standard structure that contains three template images for enabled, disabled, and 
highlight state.
 *  @code
 *  typedef struct _WGL_IMG_STD_SET_T
 *  {
 *          WGL_HIMG_TPL_T          t_enable;
 *          WGL_HIMG_TPL_T          t_disable;
 *          WGL_HIMG_TPL_T          t_highlight;
 *  } WGL_IMG_STD_SET_T;
 *  @endcode
 *  @li@c  t_enable                                          -Specify the template image for enabled state
 *  @li@c  t_disable                                         -Specify the template image for disabled state
 *  @li@c  t_highlight                                       -Specify the template image for highlight state
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_IMG_STD_SET_T
{
        WGL_HIMG_TPL_T          t_enable;
        WGL_HIMG_TPL_T          t_disable;
        WGL_HIMG_TPL_T          t_highlight;
} WGL_IMG_STD_SET_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_IMG_EXT_SET_T
 *  @brief It is the extended structure that contains five template images for enabled, disabled, highlight, 
highlight-inactive, and pushed state.
 *  @code
 *  typedef struct _WGL_IMG_EXT_SET_T
 *  {
 *          WGL_HIMG_TPL_T          t_enable;
 *          WGL_HIMG_TPL_T          t_disable;
 *          WGL_HIMG_TPL_T          t_highlight;
 *          WGL_HIMG_TPL_T          t_highlight_inactive;
 *          WGL_HIMG_TPL_T          t_push;
 *          WGL_HIMG_TPL_T          t_push_unhighlight;
 *  } WGL_IMG_EXT_SET_T;
 *  @endcode
 *  @li@c  t_enable                                          -Specify the template image for enabled state
 *  @li@c  t_disable                                         -Specify the template image for disable state
 *  @li@c  t_highlight                                       -Specify the template image for highlight state
 *  @li@c  t_highlight_inactive                              -Specify the template image for highlight but inactive state
 *  @li@c  t_push                                            -Specify the template image for push state
 *  @li@c  t_push_unhighlight                                -Specify the template image for push but unhighlight state
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_IMG_EXT_SET_T
{
        WGL_HIMG_TPL_T          t_enable;
        WGL_HIMG_TPL_T          t_disable;
        WGL_HIMG_TPL_T          t_highlight;
        WGL_HIMG_TPL_T          t_highlight_inactive;
        WGL_HIMG_TPL_T          t_push;
        WGL_HIMG_TPL_T          t_push_unhighlight;
} WGL_IMG_EXT_SET_T;

/*Image Information*/
/*------------------------------------------------------------------*/
/*! @struct WGL_IMG_INFO_T
 *  @brief This struct contains image information.
 *  @code
 *  typedef struct _WGL_IMG_INFO_T
 *  {
 *          WGL_IMG_SET_TYPE_T              e_type;
 *          union
 *          {
 *                  WGL_IMG_BASIC_SET_T     t_basic;
 *                  WGL_IMG_STD_SET_T       t_standard;
 *                  WGL_IMG_EXT_SET_T       t_extend;
 *          } u_img_data;
 *  } WGL_IMG_INFO_T;
 *  @endcode
 *  @li@c  e_type                                            -Specify the image-set format this structure contains
 *  @li@c  t_basic                                           -Contains basic image set information.
 *  @li@c  t_standard                                        -Contains standard image set information.
 *  @li@c  t_extend                                          -Contains extend image set information.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_IMG_INFO_T
{
        WGL_IMG_SET_TYPE_T              e_type;
        union
        {
                WGL_IMG_BASIC_SET_T     t_basic;
                WGL_IMG_STD_SET_T       t_standard;
                WGL_IMG_EXT_SET_T       t_extend;
        } u_img_data;
} WGL_IMG_INFO_T;

/*font info*/
/*------------------------------------------------------------------*/
/*! @struct WGL_FONT_INFO_T
 *  @brief This struct contain widget font information.
 *  @code
 *  typedef struct _WGL_FONT_INFO_T
 *  {
 *          FE_FNT_SIZE                     e_font_size;
 *          FE_FNT_STYLE                    e_font_style;
 *          FE_CMAP_ENCODING                e_font_cmap;
 *          CHAR                            a_c_font_name[WGL_MAX_FONT_NAME];
 *          INT16                           i2_width;                           
 *          UINT8                           ui1_custom_size;
 *  } WGL_FONT_INFO_T;
 *  @endcode
 *  @li@c  e_font_size                                       -Font size 
 *  @li@c  e_font_style                                      -Font style
 *  @li@c  e_font_cmap                                       -Font encoding cmap.
 *  @li@c  a_c_font_name[WGL_MAX_FONT_NAME]                  -Font name string.
 *  @li@c  i2_width                                          -font width. non-zero: fixed width. zero: width from font file
 *  @li@c  ui1_custom_size                                   -this value will be meaningful when e_font_size == FE_FNT_SIZE_CUSTOM
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_FONT_INFO_T
{
        FE_FNT_SIZE                     e_font_size;
        FE_FNT_STYLE                    e_font_style;
        FE_CMAP_ENCODING                e_font_cmap;
        CHAR                            a_c_font_name[WGL_MAX_FONT_NAME];
        INT16                           i2_width;                          
        UINT8                           ui1_custom_size;
} WGL_FONT_INFO_T;

/*font sets*/
/*------------------------------------------------------------------*/
/*! @struct WGL_FONT_BASIC_SET_T
 *  @brief This struct contains font basic information.
 *  @code
 *  typedef struct _WGL_FONT_BASIC_SET_T
 *  {
 *          WGL_FONT_INFO_T                 t_focused;
 *          WGL_FONT_INFO_T                 t_unfocused;
 *  } WGL_FONT_BASIC_SET_T;
 *  @endcode
 *  @li@c  t_focused                                         -
 *  @li@c  t_unfocused                                       -
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_FONT_BASIC_SET_T
{
        WGL_FONT_INFO_T                 t_focused;
        WGL_FONT_INFO_T                 t_unfocused;
} WGL_FONT_BASIC_SET_T;

/*extend font info*/
/*------------------------------------------------------------------*/
/*! @struct WGL_FONT_INFO_EX_T
 *  @brief The struct contains the extend font information.
 *  @code
 *  typedef struct _WGL_FONT_INFO_EX_T
 *  {
 *          WGL_FONT_SET_TYPE_T     e_type;
 *          union
 *          {
 *              WGL_FONT_BASIC_SET_T     t_basic;
 *          } u_font_data;
 *  } WGL_FONT_INFO_EX_T;
 *  @endcode
 *  @li@c  e_type                                            -Contains font set type
 *  @li@c  t_basic                                           -Contains the font information for basic type.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_FONT_INFO_EX_T
{
        WGL_FONT_SET_TYPE_T     e_type;

        union
        {
            WGL_FONT_BASIC_SET_T     t_basic;
        } u_font_data;

} WGL_FONT_INFO_EX_T;

/*inset*/
/*------------------------------------------------------------------*/
/*! @struct WGL_INSET_T
 *  @brief The struct contains the inset information.
 *  @code
 *  typedef struct _WGL_INSET_T
 *  {
 *          INT32                           i4_left;
 *          INT32                           i4_right;
 *          INT32                           i4_top;
 *          INT32                           i4_bottom;    
 *  } WGL_INSET_T;    
 *  @endcode
 *  @li@c  i4_left                          -The relative value of left.
 *  @li@c  i4_right                        -The relative value of right.
 *  @li@c  i4_top                          -The relative value of top.
 *  @li@c  i4_bottom                     -The relative value of bottom.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_INSET_T
{
        INT32                           i4_left;
        INT32                           i4_right;
        INT32                           i4_top;
        INT32                           i4_bottom;    
} WGL_INSET_T;    

/*------------------------------------------------------------------*/
/*! @struct WGL_TEXT_ATTR_T
 *  @brief This struct contains text attribute information
 *  @code
 *  typedef struct _WGL_TEXT_ATTR_T
 *  {    
 *      WGL_TEXT_ATTR_TYPE_T t_font_attr_type;
 *      VOID*                pv_attr_data;    
 *  } WGL_TEXT_ATTR_T;
 *  @endcode
 *  @li@c  t_font_attr_type                                  -
 *  @li@c  pv_attr_data                                      -
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TEXT_ATTR_T
{    
    WGL_TEXT_ATTR_TYPE_T t_font_attr_type;
    VOID*                pv_attr_data;    
} WGL_TEXT_ATTR_T;

typedef struct _WGL_UPD_REGION_T
{    
    UINT32                      ui4_update_rect_num;   
    GL_RECT_T                   at_update_rect[WGL_MAX_UPDATE_RECT_NUM];
} WGL_UPD_REGION_T;

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



