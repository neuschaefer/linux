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
/*! @file u_wgl_edit.h 
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
/*! @defgroup groupMW_WGL_EDIT  Edit Widget
 *
 *  @ingroup groupMW_WGL
 *  @brief Provides definitions,commands of the edit widget.
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW_WGL
 *  @{ 
 */
/*----------------------------------------------------------------------------*/
 
#ifndef _U_WGL_EDIT_H_
#define _U_WGL_EDIT_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_wgl.h"
#include "u_wgl_tools.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*Specific Widget Style*/
#define WGL_STL_EB_PASSWORD                     ((UINT32) WGL_STL_WGT_SPF_START)    /**< 	If this style is assigned, it means the characters displayed in an edit box are replaced by password character.        */
#define WGL_STL_EB_MAX_CHAR_16                  ((UINT32) WGL_STL_WGT_SPF_START << 1)    /**<  This style indicates that EditBox has 16 characters capacity.      */
#define WGL_STL_EB_MAX_CHAR_64                  ((UINT32) WGL_STL_WGT_SPF_START << 2)    /**<   This style indicates that EditBox has 64 characters capacity.     */
#define WGL_STL_EB_MAX_CHAR_256                 ((UINT32) WGL_STL_WGT_SPF_START << 3)    /**<   This style indicates that EditBox has 256 characters capacity.     */
#define WGL_STL_EB_RECYCLE                      ((UINT32) WGL_STL_WGT_SPF_START << 4)    /**<  This style indicates that EditBox will move the cursor to the beginning when user presses 'right' key at the end.      */
#define WGL_STL_EB_CHAR_WITH_IMG                ((UINT32) WGL_STL_WGT_SPF_START << 5)    /**<  This style indicates that each char in EditBox will have a image background for different states.
                                                                                                                                                                     In this style, WGL_CMD_GL_SET_FONT, WGL_CMD_GL_SET_FONT_EX command can only set font with fixed width.       */
#define WGL_STL_EB_PASSWORD_ESC_LAST            ((UINT32) WGL_STL_WGT_SPF_START << 6)    /**< 	If this style is assigned, it means the characters displayed in an edit box are replaced by password character.        */

/*Color Command Target for EditBox*/
#define WGL_CLR_EB_CURSOR                       ((UINT8) WGL_CLR_WGT_SPF_START + 1)    /**< This definition indicates that the color is for drawing EditBox's cursor char.       */

/*extension attribute*/
#define WGL_EB_EA_AUTO_EDITING                  ((UINT16)0x0001)    /**< This attribute indicates EditBox will enter editing mode automatically while it gets focus.       */
#define WGL_EB_CHAR_SUPPORT                     ((UINT16)0x0002)    /**<  This attribute indicates EditBox support input char besides number(0--9).      */
#define WGL_EB_ALWAYS_HT                        ((UINT16)0x0004)    /**< This attribute indicates EditBox will always show highlight cursor  even if it has not focus       */
#define WGL_EB_KEEP_CURSOR_POS                  ((UINT16)0x0008)    /**< This attribute indicates EditBox's cursor postion will not change when edit mode is changed, if this extra attribute dose not set when edit mode changed(for example, edit box lost focus, edit mode is set to false),then current cursor position will be reset to 0       */

/*Specific Command*/
#define WGL_CMD_EB_SET_INPUT_LIMIT              WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_EDIT_BOX, 0)    /**< This command sets an EditBox's input capacity. Note: this command may affect the string kept in EditBox - the characters over new input limit will be deleted.

                                                                                                                                                                              related parameters in c_wgl_do_cmd(),
                                                                                                                                                                              
                                                                                                                                                                              pv_param1:   An UINT16 value specifies the capacity. It cannot be over EditBox's character capacity.

                                                                                                                                                                              pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                              */
#define WGL_CMD_EB_GET_INPUT_LIMIT              WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_EDIT_BOX, 1)    /**< This command returns the input capacity of an Edit Box.   

                                                                                                                                                                               related parameters in c_wgl_do_cmd(),
                                                                                                                                                                               
                                                                                                                                                                               pv_param1:   Pointer to a UINT16 variable to receive the input capacity. It cannot be NULL.

                                                                                                                                                                               pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                               */
#define WGL_CMD_EB_SET_TEXT                     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_EDIT_BOX, 2)    /**< This command is used to set the text displayed in the widget.

                                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                                            pv_param1:   Pointer to a UTF16_T string. It must be a null terminated string and cannot be over the input limit. If it is, those characters over limit won't be copied. If it is NULL, it means to remove the old string.

                                                                                                                                                                            pv_param2:   A UINT16 specifies the length in character of the string. The null character isn't included. 
                                                                                                                                                                            */
#define WGL_CMD_EB_GET_TEXT                     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_EDIT_BOX, 3)    /**<This command is used to get the text.

                                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                                            pv_param1:   Pointer to a UTF16_T buffer to receive EditBox's string.

                                                                                                                                                                            pv_param2:   A UINT16 specifies the length in character of the buffer. The length cannot be less than the length of EditBox's string. The null character isn't included.
                                                                                                                                                                            */
#define WGL_CMD_EB_GET_TEXT_LENGTH              WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_EDIT_BOX, 4)    /**<This command returns the length of EditBox's string.

                                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                                            pv_param1:   Pointer to a UINT16 variable to receive the length. It cannot be NULL.

                                                                                                                                                                            pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                            */
#define WGL_CMD_EB_SET_CURSOR_POS               WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_EDIT_BOX, 5)    /**< This command moves the cursor to the specific position. If the new position is larger than the current length, the cursor will be set at the end.

                                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                                            pv_param1   A UINT16 value specifies the new position. 

                                                                                                                                                                           pv_param2   Reserved. It must be NULL.
                                                                                                                                                                            */
#define WGL_CMD_EB_GET_CURSOR_POS               WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_EDIT_BOX, 6)    /**<This command returns EditBox's cursor position.

                                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                                            pv_param1:   Pointer to a UINT16 variable to receive the position. It cannot be NULL.

                                                                                                                                                                            pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                            */
#define WGL_CMD_EB_SET_CHANGE_FLAG              WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_EDIT_BOX, 7)    /**<This command changes the flag that indicates whether an EditBox's string is changed or not. EditBox turns on this flag when user changes the string via keyboard/RC.

                                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                                            pv_param1:   A BOOL value specifies the new state. If it is TRUE, it means the string is changed; otherwise, it means the string isn't changed.

                                                                                                                                                                            pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                            */
#define WGL_CMD_EB_GET_CHANGE_FLAG              WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_EDIT_BOX, 8)    /**<This command returns the change-flag of an EditBox.

                                                                                                                                                                            related parameters in c_wgl_do_cmd(),
                                                                                                                                                                            
                                                                                                                                                                            pv_param1:   Pointer to a BOOL variable to receive the flag. If the return value is TRUE, it means the string is changed; otherwise, it means the string isn't changed. In addition, it cannot be NULL.

                                                                                                                                                                            pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                            */
#define WGL_CMD_EB_SET_KEY_MAP                  WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_EDIT_BOX, 9)    /**<  This command changes the keys for operating an EditBox.

                                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                                            pv_param1:   Pointer to a WGL_EB_KEY_MAP_T that contains the new key map. If it is NULL, it means to use the EditBox's default setting.  

                                                                                                                                                                            pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                            */
#define WGL_CMD_EB_GET_KEY_MAP                  WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_EDIT_BOX, 10)    /**<This command returns the key map of an EditBox.

                                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                                              pv_param1:   Pointer to a WGL_EB_KEY_MAP_T variable to receive the key map. It cannot be NULL.

                                                                                                                                                                              pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                              */
#define WGL_CMD_EB_SET_INSERT_MODE              WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_EDIT_BOX, 11)    /**< This command switches the mode between 'insert' and 'replace'. 

                                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                                            pv_param1:   A WGL_EB_MODE_T value specifies the mode. If it is WGL_EB_MODE_INSERT, it means to change to 'insert' mode; if it is WGL_EB_MODE_REPLACE, it means to change to 'replace' mode.

                                                                                                                                                                            pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                            */
#define WGL_CMD_EB_GET_INSERT_MODE              WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_EDIT_BOX, 12)    /**<This command returns EditBox's input mode.

                                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                                            pv_param1:   Pointer to a WGL_EB_MODE_T variable to receive the input mode. It cannot be NULL.

                                                                                                                                                                            pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                            */
#define WGL_CMD_EB_SET_EXT_ATTR                 WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_EDIT_BOX, 13)    /**<This command specifies EditBox's extension attributes.

                                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                                            pv_param1:   A UNIT16 value specifies which bits are used.

                                                                                                                                                                            pv_param2:   A UNIT16 value specifies its extension attributes.
                                                                                                                                                                            */
#define WGL_CMD_EB_GET_EXT_ATTR                 WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_EDIT_BOX, 14)    /**<This command returns EditBox's extension attributes.

                                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                                            pv_param1:   Pointer to a UNIT16 variable to receive the extension attributes. It cannot be NULL.

                                                                                                                                                                            pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                            */
                

/*Specific Command Supported by EDITBOX-Content*/
#define WGL_CMD_EB_SET_ALIGN                    WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_EDIT_BOX, 0)    /**< This command is used to set EditBox's foreground alignment.

                                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                pv_param1:   Specifies the alignment style. It can be from WGL_AS_LEFT_TOP to WGL_AS_RIGHT_BOTTOM. About the explanation of these alignment styles.

                                                                                                                                                                                pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                */
#define WGL_CMD_EB_GET_ALIGN                    WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_EDIT_BOX, 1)    /**<  This command returns the EditBox's foreground alignment.

                                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                                             pv_param1:   Pointer to a UINT8 to receive its alignment style. It cannot be NULL.

                                                                                                                                                                             pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                             */
#define WGL_CMD_EB_SET_CNT_INSET                WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_EDIT_BOX, 2)    /**< This command is used to set the display margin of EditBox's foreground. The i4_left is for the left margin, the i4_right is for the right margin, the i4_top is for the top margin, and the i4_bottom is for the bottom margin. All of them cannot be less than 0.

                                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                                            pv_param1:   Pointer to a WGL_INSET_T value to specify the display margin. It cannot be NULL.

                                                                                                                                                                            pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                            */
#define WGL_CMD_EB_GET_CNT_INSET                WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_EDIT_BOX, 3)    /**< This command returns the display margin of EditBox's foreground. The i4_left is the left margin, the i4_right is the right margin, the i4_top is the top margin, and the i4_bottom is the bottom margin. 

                                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                                            pv_param1:   Pointer to a WGL_INSET_T value to receive the display margin. It cannot be NULL.

                                                                                                                                                                            pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                            */
#define WGL_CMD_EB_SET_PWD_CHAR                 WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_EDIT_BOX, 4)    /**<This command changes the password character for an EditBox.

                                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                                            pv_param1:   A UTF16_T value specifies the new password character. 

                                                                                                                                                                            pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                            */
#define WGL_CMD_EB_GET_PWD_CHAR                 WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_EDIT_BOX, 5)    /**< This command returns EditBox's password character.

                                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                                            pv_param1:   Pointer to a UTF16_T variable to receive the password character. It cannot be NULL.

                                                                                                                                                                            pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                            */
#define WGL_CMD_EB_ENABLE_CURSOR_HT             WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_EDIT_BOX, 6)   /**< This command enable cursor character highlight.

                                                                                                                                                                                   related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                  pv_param1: BOOL. TRUE to enable "Cursor Highlight", FALSE to disable "Cursor Highlight".   

                                                                                                                                                                                  pv_param2:Reserved. It must be NULL.
                                                                                                                                                                                  */
#define WGL_CMD_EB_SET_CURSOR_CHAR              WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_EDIT_BOX, 7)   /**<This command set cursor character .

                                                                                                                                                                                  related parameters in c_wgl_do_cmd(), 

                                                                                                                                                                                    pv_param1: pointer to UTF16_T variable.      

                                                                                                                                                                                    pv_param2:   Reserved. It must be NULL.*/
#define WGL_CMD_EB_GET_CURSOR_CHAR              WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_EDIT_BOX, 8)   /**<  This command get cursor character .

                                                                                                                                                                                     related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                     pv_param1: pointer ot a UTF16_T varable.      

                                                                                                                                                                                     pv_param2:   Reserved. It must be NULL.*/

#define WGL_CMD_EB_SET_CHAR_IMG                 WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_EDIT_BOX, 9)    /**<This command sets char background image, in order to get best ui effect, image's width plus ui4_gap need to be equal to chars' width.

                                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                                            pv_param1:   Pointer to a WGL_EB_CHAR_IMG_T variable  that contains the char background image info. 

                                                                                                                                                                            pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                            */
#define WGL_CMD_EB_GET_CHAR_IMG                 WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_EDIT_BOX, 10)    /**<This command gets char background image info.

                                                                                                                                                                                related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                pv_param1:   Pointer to a WGL_EB_CHAR_IMG_T variable. 

                                                                                                                                                                                pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                */
#define WGL_CMD_EB_SET_PWD_BK_CHAR              WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_EDIT_BOX, 11)    /**< This command set default background display character for password style      

                                                                                                                                                                                     related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                     pv_param1:  A UTF16_T value.       

                                                                                                                                                                                     pv_param2:   Reserved. It must be NULL.*/
#define WGL_CMD_EB_GET_PWD_BK_CHAR              WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_EDIT_BOX, 12)    /**< This command get default background display character for password style      

                                                                                                                                                                                     related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                     pv_param1: pointer ot a UTF16_T varable.     

                                                                                                                                                                                     pv_param2:   Reserved. It must be NULL.*/

#define EB_CNT_PRIVATE_START                    13      /**< (WGL_CMD_EB_GET_PWD_BK_CHAR - WGL_CMD_EB_SET_ALIGN + 1)       */


/*Specific Notify Code*/
#define WGL_NC_EB_CHANGED                       ((UINT32)WGL_NC_WIDGET_SPF_START + 0)    /**< This notification is sent when its string is changed, and the change is caused by user's keyboard/RC input.
                                                                                                                                                            The associated data for this notify is WGL_NOTIFY_DATA_T without any data in its u member, and its ui4_nc_code is WGL_NC_EB_CHANGED.*/
#define WGL_NC_EB_OVER_MAX                      ((UINT32)WGL_NC_WIDGET_SPF_START + 1)    /**<  This notification is sent when user inputs in two situations; one is the cursor is at the end and the string is full; the other is the string is full and the edit mode is 'Insert'. 
                                                                                                                                                             The associated data for this notify is WGL_NOTIFY_DATA_T without any data in its u member, and its ui4_nc_code is WGL_NC_EB_OVER_MAX.*/

/*max length for different widget style*/
#define WGL_EB_MAX_16_LEN                       ((UINT16)16)    /**<        */
#define WGL_EB_MAX_64_LEN                       ((UINT16)64)    /**<        */
#define WGL_EB_MAX_256_LEN                      ((UINT16)256)    /**<        */
        
#define WGL_EB_GET_MAX_LENGTH(ui4_style)                                                                                                        \
        ( ((ui4_style) & WGL_STL_EB_MAX_CHAR_256) ? WGL_EB_MAX_256_LEN                                \
                : (((ui4_style) & WGL_STL_EB_MAX_CHAR_64) ? WGL_EB_MAX_64_LEN : WGL_EB_MAX_16_LEN) )    /**<        */

/**/
/*------------------------------------------------------------------*/
/*! @enum WGL_EB_MODE_T
 *  @brief Define replace/insert mode in edit mode
 *  @code
 *  typedef enum _WGL_EB_MODE_T
 *  {
 *          WGL_EB_MODE_REPLACE = 0,
 *          WGL_EB_MODE_INSERT
 *  } WGL_EB_MODE_T;
 *  @endcode
 *  @li@c  WGL_EB_MODE_REPLACE                               -input character will replace with current cursor position's character
 *  @li@c  WGL_EB_MODE_INSERT                                -input character will be inserted to current cursor positon
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_EB_MODE_T
{
        WGL_EB_MODE_REPLACE = 0,
        WGL_EB_MODE_INSERT
} WGL_EB_MODE_T;

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @struct WGL_EB_KEY_MAP_T
 *  @brief Define key code and user action map table 
 *  @code
 *  typedef struct _WGL_EB_KEY_MAP_T
 *  {
 *          UINT32                  ui4_cursor_left;
 *          UINT32                  ui4_cursor_right;
 *          UINT32                  ui4_backsapce;
 *          UINT32                  ui4_switch_insert;
 *          UINT32                  ui4_enter_editing;
 *          UINT32                  ui4_leave_editing;
 *  } WGL_EB_KEY_MAP_T;
 *  @endcode
 *  @li@c  ui4_cursor_left                                   - key code to move cursor toward left direction.
 *  @li@c  ui4_cursor_right                                  -key code to move cursor toward right direction.
 *  @li@c  ui4_backsapce                                     -key code to remove the charater on the left of current cursor position's character
 *  @li@c  ui4_switch_insert                                 - key code to switch insert/replace mode
 *  @li@c  ui4_enter_editing                                 - key code to enter edit mode
 *  @li@c  ui4_leave_editing                                 - key code to leave edit mode
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_EB_KEY_MAP_T
{
        UINT32                  ui4_cursor_left;
        UINT32                  ui4_cursor_right;
        UINT32                  ui4_backsapce;
        UINT32                  ui4_switch_insert;
        UINT32                  ui4_enter_editing;
        UINT32                  ui4_leave_editing;
} WGL_EB_KEY_MAP_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_EB_CHAR_IMG_T
 *  @brief Define char image info
 *  @code
 *  typedef struct _WGL_EB_CHAR_IMG_T
 *  {
 *          WGL_HIMG_TPL_T          h_normal;           
 *          WGL_HIMG_TPL_T          h_highlight;        
 *          UINT32                  ui4_gap;            
 *          UINT32                  ui4_img_num;        
 *  } WGL_EB_CHAR_IMG_T;
 *  @endcode
 *  @li@c  h_normal                                          -normal char's background image
 *  @li@c  h_highlight                                       -highlight char's background image
 *  @li@c  ui4_gap                                           -gap between images
 *  @li@c  ui4_img_num                                       -the number of images
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_EB_CHAR_IMG_T
{
        WGL_HIMG_TPL_T          h_normal;          
        WGL_HIMG_TPL_T          h_highlight;       
        UINT32                  ui4_gap;           
        UINT32                  ui4_img_num;       
} WGL_EB_CHAR_IMG_T;


/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/



#ifdef __cplusplus
}
#endif

#endif

/*----------------------------------------------------------------------------*/
/*! @} */   /* end of groupMW_WGL_EDIT*/
/*----------------------------------------------------------------------------*/

