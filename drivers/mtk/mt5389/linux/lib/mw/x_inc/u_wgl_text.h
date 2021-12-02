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
/*! @file u_wgl_text.h 
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
/*! @defgroup groupMW_WGL_TEXT   Text Widget
 *
 *  @ingroup groupMW_WGL
 *  @brief Provides definitions and exported function of the text widget.
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW_WGL
 *  @{ 
 */
/*----------------------------------------------------------------------------*/ 

#ifndef _U_WGL_TEXT_H_
#define _U_WGL_TEXT_H_

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
/*Font attribute type*/
#define WGL_TEXT_FONT_ATTR_TYPE_T           UINT32    /**<   indicates font attribute type     */
#define WGL_TEXT_FONT_ATTR_UNIFORM_WIDTH    ((WGL_TEXT_FONT_ATTR_TYPE_T) 0)   /**<  indicates font attribute type is uniform width.    */
#define WGL_TEXT_FONT_ATTR_UNREC_CHAR       ((WGL_TEXT_FONT_ATTR_TYPE_T) 1)   /**<   indicates font attribute type is unrecognized character.    */
/*String Position Display type*/
#define WGL_TEXT_POSITION_TYPE_T           UINT32    /**<   indicates string position display type    */
#define WGL_TEXT_POSITION_BY_STRING_INDEX    ((WGL_TEXT_POSITION_TYPE_T) 0)   /**<  indicates string display position by string index.    */
#define WGL_TEXT_POSITION_BY_INDICATE        ((WGL_TEXT_POSITION_TYPE_T) 1)   /**<  indicates string display position by user indicate.  */
/*Font attribute target*/
#define WGL_TEXT_FONT_ATTR_UNFOCUS_FONT ((UINT8)1)    /**<  indicates font attribute target is unfocused state font.      */
#define WGL_TEXT_FONT_ATTR_FOCUS_FONT      ((UINT8)2)    /**< indicates font attribute target is focused state font.       */

/*Page Scroll Method*/
/*------------------------------------------------------------------*/
/*! @enum WGL_TEXT_PS_TYPE_T
 *  @brief Define page scroll type
 *  @code 
 *  typedef enum _WGL_TEXT_PS_TYPE_T
 *  {
 *      WGL_TEXT_PS_TYPE_KEEP_LAST   = 0,
 *      WGL_TEXT_PS_TYPE_REPLACE_ALL,
 *      WGL_TEXT_PS_TYPE_USER_DEF
 *  } WGL_TEXT_PS_TYPE_T;
 *  @endcode
 *  @li@c  WGL_TEXT_PS_TYPE_KEEP_LAST                        -scroll will keep last line of previous page
 *  @li@c  WGL_TEXT_PS_TYPE_REPLACE_ALL                      -scroll will replace with all lines of previous page
 *  @li@c  WGL_TEXT_PS_TYPE_USER_DEF                         -scroll lines defined by user
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_TEXT_PS_TYPE_T
{
    WGL_TEXT_PS_TYPE_KEEP_LAST   = 0,
    WGL_TEXT_PS_TYPE_REPLACE_ALL,
    WGL_TEXT_PS_TYPE_USER_DEF
} WGL_TEXT_PS_TYPE_T;

/*Widget Style*/
#define WGL_STL_TEXT_MULTILINE          ((UINT32) WGL_STL_WGT_SPF_START)    /**< If this style is assigned, it means the Text widget will have multiple lines. It happens when the length is over the max that one line can contain or a carriage-return character is met.       */
#define WGL_STL_TEXT_SMART_CUT          ((UINT32) WGL_STL_WGT_SPF_START << 1)    /**<When the area for displaying whole string is over the widget's display area, there's '? token is at the display end if this style is assigned; otherwise, the part out of display rectangle will be truncated.        */
#define WGL_STL_TEXT_MAX_32             ((UINT32) WGL_STL_WGT_SPF_START << 2)    /**<This style indicates that Text widget has 32 characters capacity.        */
#define WGL_STL_TEXT_MAX_128            ((UINT32) WGL_STL_WGT_SPF_START << 3)    /**<  This style indicates that Text widget has 128 characters capacity.         */
#define WGL_STL_TEXT_MAX_512            ((UINT32) WGL_STL_WGT_SPF_START << 4)    /**<   This style indicates that Text widget has 512 characters capacity.         */
#define WGL_STL_TEXT_MAX_2048           ((UINT32) WGL_STL_WGT_SPF_START << 5)    /**<  This style indicates that Text widget has 2048 characters capacity.          */
#define WGL_STL_TEXT_MAX_4096           ((UINT32) WGL_STL_WGT_SPF_START << 6)    /**<  This style indicates that Text widget has 4096 characters capacity.   */
#define WGL_STL_TEXT_MAX_DIS_5_LINE     ((UINT32) WGL_STL_WGT_SPF_START << 7)    /**< This style indicates that Text widget has 5-line capacity for displaying string.       */
#define WGL_STL_TEXT_MAX_DIS_10_LINE    ((UINT32) WGL_STL_WGT_SPF_START << 8)    /**< This style indicates that Text widget has 10-line capacity for displaying string.           */
#define WGL_STL_TEXT_MAX_DIS_20_LINE    ((UINT32) WGL_STL_WGT_SPF_START << 9)    /**<   This style indicates that Text widget has 20-line capacity for displaying string.         */
#define WGL_STL_TEXT_MAX_DIS_40_LINE    ((UINT32) WGL_STL_WGT_SPF_START << 10)    /**< This style indicates that Text widget has 40-line capacity for displaying string.       
                                                                                                                                                Note: WGL_STL_TEXT_MAX_DIS_40_LINE, WGL_STL_TEXT_MAX_DIS_20_LINE, WGL_STL_TEXT_MAX_DIS_10_LINE, 
                                                                                                                                                WGL_STL_TEXT_MAX_DIS_5_LINE should not be used at the same time. 
                                                                                                                                                If it happens, the priority is: WGL_STL_TEXT_MAX_DIS_40_LINE > WGL_STL_TEXT_MAX_DIS_20_LINE > WGL_STL_TEXT_MAX_DIS_10_LINE > WGL_STL_TEXT_MAX_DIS_5_LINE. 
                                                                                                                                                If none is assigned, the default setting is WGL_STL_TEXT_MAX_DIS_5_LINE.*/
#define WGL_STL_TEXT_MAX_DIS_USER_DEF   ((UINT32) WGL_STL_WGT_SPF_START << 11)    /**<  This style indicates that Text widget's line capacity for displaying string is determined by user.        */
#define WGL_STL_TEXT_SMART_CUT_OVER_MAX ((UINT32) WGL_STL_WGT_SPF_START << 12)    /**< This style indicates Text widget has smart-cut characters at the end of last page if the input string is over its characters capacity.       */
#define WGL_STL_TEXT_END_AS_NEWLINE     ((UINT32) WGL_STL_WGT_SPF_START << 13)    /**<  This style indicates Text  widget will only break line when there exists break character for example '\n' or '\r'    */
#define WGL_STL_TEXT_IGNORE_NEWLINE     ((UINT32) WGL_STL_WGT_SPF_START << 14)    /**< This sytle indicates Text widget will ignore break character when break line       */
#define WGL_STL_TEXT_MAX_32768	        ((UINT32) WGL_STL_WGT_SPF_START << 15)    /**< This style indicates that Text widget has 32768 characters capacity.       
                                                                                                                                                Note: WGL_STL_TEXT_MAX_32768,WGL_STL_TEXT_MAX_4096, WGL_STL_TEXT_MAX_2048, WGL_STL_TEXT_MAX_512, 
                                                                                                                                                WGL_STL_TEXT_MAX_128, and WGL_STL_TEXT_MAX_32 should not be used at the same time. 
                                                                                                                                                If it happens, the priority is: WGL_STL_TEXT_MAX_32768>WGL_STL_TEXT_MAX_4096 > WGL_STL_TEXT_MAX_2048 > WGL_STL_TEXT_MAX_512 > WGL_STL_TEXT_MAX_128 > WGL_STL_TEXT_MAX_32. 
                                                                                                                                                Besides, if none is used, the default setting is WGL_STL_TEXT_MAX_32.*/
#define WGL_STL_TEXT_MULTISTRING       ((UINT32) WGL_STL_WGT_SPF_START << 16)    /**< This sytle indicates Text widget will Support Multi string       */
#define WGL_STL_TEXT_MS_NO_SMARTCUT    ((UINT32) WGL_STL_WGT_SPF_START << 17)    /**<When the area for displaying whole string is over the widget's display area, there's '? token is at the display end if this style is assigned; otherwise, the part out of display rectangle will be truncated.        */

/* Specific Command */
#define WGL_CMD_TEXT_GET_MIN_SIZE       WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TEXT, 0)    /**<  This command is used to get minimum widget size, which includes minimum sizes of content and border.

                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                        pv_param1:   Pointer of a GL_SIZE_T to receive the size. It cannot be NULL.

                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                        */

/*Specific Command*/
#define WGL_CMD_TEXT_SET_KEY_MAP        WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TEXT, 1)    /**< This command changes the keys for operating a Text.

                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                        pv_param1:   Pointer to a WGL_TEXT_KEY_MAP_T that contains the new key map. If it is NULL, it means to use the Text's default setting. 

                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                        */
#define WGL_CMD_TEXT_GET_KEY_MAP        WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TEXT, 2)    /**<This command returns the key map of a Text.

                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                        pv_param1:   Pointer to a WGL_TEXT_KEY_MAP_T variable to receive the key map. It cannot be NULL.

                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                        */
#define WGL_CMD_TEXT_SET_PAGE_SCROL     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TEXT, 3)    /**< This command sets the scroll type for a Text.

                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                        pv_param1:   A WGL_TEXT_PS_TYPE_T value to indicate the scroll type. 

                                                                                                                                                        pv_param2:   A UINT16 value that specify the line number for a scroll operation. This parameter is valid for WGL_TEXT_PS_TYPE_USER_DEF only.
                                                                                                                                                        */
#define WGL_CMD_TEXT_GET_PAGE_SCROL     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TEXT, 4)    /**< This command returns the scroll type of a Text.

                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                        pv_param1:   Pointer to a WGL_TEXT_PS_TYPE_T variable to receive the scroll type.

                                                                                                                                                        pv_param2:   Pointer to a UINT16 variable to receive the line number of a scroll operation. For these types except WGL_TEXT_PS_TYPE_USER_DEF, the return value is always 0.
                                                                                                                                                        */
#define WGL_CMD_TEXT_PAGE_SCROL         WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TEXT, 5)    /**<  This command raises a page scroll request.

                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                        pv_param1   TRUE to Page Up and FALSE to page down.

                                                                                                                                                        pv_param2   TRUE to have a repaint if the content in display rectangle is dirty, and FALSE to leave this duty to the caller.
                                                                                                                                                        */
#define WGL_CMD_TEXT_SET_DISPLAY_START  WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TEXT, 6)    /**< This command is used to set the first display line.

                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                        pv_param1:   A UINT16 specifies the line number. It is zero based.

                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                        */
#define WGL_CMD_TEXT_SET_DISPLAY_START_EX  WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TEXT, 7)    /**< This command is just for sbtl.
	
																																							related parameters in c_wgl_do_cmd(),
	
																																							pv_param1:	 A UINT16 specifies the line number. It is zero based.
	
																																							pv_param2:	 Reserved. It must be NULL.
																																							*/
#define WGL_CMD_TEXT_GET_DISPLAY_START  WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TEXT, 8)    /**<  This command returns the line number of the first display line.
	
																																							related parameters in c_wgl_do_cmd(),
	
	
																																							pv_param1	Pointer to a UINT16 variable to receive the line number. It cannot be NULL.
	
																																							pv_param2	Reserved. It must be NULL.
																																							*/
#define WGL_CMD_TEXT_GET_TOTAL_LINES    WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TEXT, 9)    /**< This command returns the line count of Text Static. Changing width of a Text Static will affect the line count.
	
																																							related parameters in c_wgl_do_cmd(),
	
																																							pv_param1:	 Pointer to a UINT16 variable to receive the line count. It cannot be NULL.
	
																																							pv_param2:	 Reserved. It must be NULL.
																																							*/
#define WGL_CMD_TEXT_GET_PAGE_LINES     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TEXT, 10)    /**< This command returns the line count of a page. Changing width or height of a Text will affect this value.
	
																																							related parameters in c_wgl_do_cmd(),
	
																																							pv_param1:	 Pointer to a UINT16 variable to receive the line count. It cannot be NULL.
	
																																							pv_param2:	 Reserved. It must be NULL.
																																							*/
#define WGL_CMD_TEXT_BEGIN_SCRL         WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TEXT, 11)    /**< This command begins text scrolling for single line mode.

																						related parameters in c_wgl_do_cmd(),

																						pv_param1:	 Pointer to a WGL_TEXT_SCRL_T structure which indicates text scrolling start parameter.

																						pv_param2:	 Reserved, must be NULL.
																					      */
#define WGL_CMD_TEXT_END_SCRL           WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TEXT, 12)    /**<  This command end text scrolling for single line mode.
	
																																							related parameters in c_wgl_do_cmd(),
	
																																							pv_param1:	 Reserved, must be NULL.
	
																																							pv_param2:	 Reserved, must be NULL.
																																							*/
#define WGL_CMD_TEXT_GET_MAX_LINE_WIDTH      WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TEXT, 13) /**<  This command return text string content's  maximum line width after layout.      
		
																																									related parameters in c_wgl_do_cmd(),
		
																																									pv_param1:	Pointer to a UINT16 value which is output argument. It will contain maximum line width 
		
																																									pv_param2:	Must be NULL.
																																									*/

/*Specific Command Supported by Text-Content*/
#define WGL_CMD_TEXT_SET_ALIGN          WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 0)    /**< This command is used to set Text Static's foreground alignment.

                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                        pv_param1:   Specifies the alignment style. It can be from WGL_AS_LEFT_TOP to WGL_AS_RIGHT_BOTTOM. 

                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                        */
#define WGL_CMD_TEXT_GET_ALIGN          WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 1)    /**< This command returns the foreground's alignment of Text Static.

                                                                                                                                                          related parameters in c_wgl_do_cmd(),

                                                                                                                                                          pv_param1:   Pointer to a UINT8 to receive its alignment style. It cannot be NULL.

                                                                                                                                                          pv_param2:   Reserved. It must be NULL.
                                                                                                                                                          */
#define WGL_CMD_TEXT_SET_TEXT           WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 2)    /**<  This command is used to set the string displayed in Text Static.

                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                            pv_param1:   Pointer to a UTF16_T string. It must be a null terminated string. If the length is over the Text Static's character capacity (the null character isn't included), the string will be truncated. If it is NULL, it means to remove the old string.

                                                                                                                                                            pv_param2:   A UINT16 integer to specify the character length of pv_param1 (the null character isn't included). For example, the pv_param2 for "Example" is 7.
                                                                                                                                                            */
#define WGL_CMD_TEXT_GET_TEXT           WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 3)    /**< This command is used to get the string of Text Static.

                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                            pv_param1:   Pointer to a buffer, which the length should not be less than the string length stored in the Text Static (the null character isn't included).

                                                                                                                                                            pv_param2:   A UINT16 integer to specify the length in character of pv_param1. For example, the pv_param2 for getting "Example" should be 8. Besides, it will append a '\0' at the end of the return string.
                                                                                                                                                            */
#define WGL_CMD_TEXT_SET_CNT_INSET      WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 4)    /**< This command is used to set the display margin of Text Static's foreground. The i4_left is for the left margin, the i4_right is for the right margin, the i4_top is for the top margin, and the i4_bottom is for the bottom margin. All of them cannot be less than 0.

                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                        pv_param1:   Pointer to a WGL_INSET_T value to specify the display margin. It cannot be NULL.

                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                        */
#define WGL_CMD_TEXT_GET_CNT_INSET      WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 5)    /**<This command returns the display margin of Text Static's foreground. The i4_left is the left margin, the i4_right is the right margin, the i4_top is the top margin, and the i4_bottom is the bottom margin. 

                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                        pv_param1:   Pointer to a WGL_INSET_T value to receive the display margin. It cannot be NULL.

                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                        */
#define WGL_CMD_TEXT_SET_LINE_GAP       WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 6)    /**<   This command is used to set the vertical interval between lines.

                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                        pv_param1:   An INT32 value that specifies the gap between lines.

                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                        */
#define WGL_CMD_TEXT_GET_LINE_GAP       WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 7)    /**< This command returns the vertical interval between lines.

                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                        pv_param1:   Pointer to an INT32 variable to receive the interval. It cannot be NULL.

                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                        */
#define WGL_CMD_TEXT_APPEND_TEXT        WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 8)    /**< This command appends string at the tail.

                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                        pv_param1:   Pointer to a UTF16_T string. It must be a null terminated string. If the total length is over the Text widget's character capacity (the null character isn't included), the string will be truncated. If it is NULL, it means to remove the old string.

                                                                                                                                                        pv_param2:   A UINT16 integer to specify the character length of pv_param1 (the null character isn't included). For example, the pv_param2 for "Example" is 7.
                                                                                                                                                        */
#define WGL_CMD_TEXT_SET_FONT_ATTR      WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 9)     /**<This command set font attribute for text widget unfocused state font.

                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                            pv_param1:   Pointer to a WGL_TEXT_FONT_ATTR_TYPE_T value. 

                                                                                                                                                            pv_param2:   accordint to WGL_TEXT_FONT_ATTR_TYPE_T

                                                                                                                                                            if type is WGL_TEXT_FONT_ATTR_UNIFORM_WIDTH, pv_param2 is a UINT16 integer to specify WGL_FONT_ATTR_T. t_uniform. ui2_width value.

                                                                                                                                                             if type is WGL_TEXT_FONT_ATTR_UNREC_CHAR, pv_param2 is a UTF16_T*  to specify unrecognized character.
                                                                                                                                                            */
#define WGL_CMD_TEXT_SET_MAX_DIS_LINE   WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 10)   /**< This command set max display line number for text widget with style(WGL_STL_TEXT_MULTILINE| WGL_STL_TEXT_MAX_DIS_USER_DEF) .   

                                                                                                                                                                related parameters in c_wgl_do_cmd(),

                                                                                                                                                                pv_param1:   Pointer to a UINT16 value. It indicates max display line number and can not larger than 4096.

                                                                                                                                                                pv_param2:   Must be NULL.
                                                                                                                                                        */
#define WGL_CMD_TEXT_SET_FONT_ATTR_EX      WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 11) /**<  This command set font attribute for text widget focus/unfocused state font.      

                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                            pv_param1:   A UINT8 integer to specify target font type. Valid value is: 

                                                                                                                                                            WGL_TEXT_FONT_ATTR_FOCUS_FONT,

                                                                                                                                                            WGL_TEXT_FONT_ATTR_UNFOCUS_FONT.

                                                                                                                                                            pv_param2:   A WGL_TEXT_FONT_ATTR_T type value to specify font attribute value.
                                                                                                                                                            */


#define WGL_CMD_TEXT_SET_STRING          WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 12)   /**<  This command set a string to the end of the string list.      

                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                            pv_param1:   Pointer to a UTF16_T string. It must be a null terminated string. If the length is over the Text Static's character capacity (the null character isn't included), the string will be truncated. If it is NULL, it means to remove the old string. 

                                                                                                                                                            pv_param2:   A UINT16 integer to specify the character length of pv_param1 (the null character isn't included). For example, the pv_param2 for "Example" is 7.
                                                                                                                                                            */

#define WGL_CMD_TEXT_SET_STRING_PREV     WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 13) /**<  This command set set a string to the header of the string list.       

                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                            pv_param1:   Pointer to a UTF16_T string. It must be a null terminated string. If the length is over the Text Static's character capacity (the null character isn't included), the string will be truncated. If it is NULL, it means to remove the old string. 

                                                                                                                                                            pv_param2:   A UINT16 integer to specify the character length of pv_param1 (the null character isn't included). For example, the pv_param2 for "Example" is 7.
                                                                                                                                                            */

#define WGL_CMD_TEXT_STRING_CLEAR        WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 14) /**<  This command clear the string list.      

                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                            pv_param1:   NULL

                                                                                                                                                            pv_param2:   NULL
                                                                                                                                                            */

#define WGL_CMD_TEXT_SET_HIGHLIGHT_POSTION   WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 15) /**<  This command set highlight postion in text wgl.      

                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                            pv_param1:   A UINT16 integer to specify the line pistion in wgl rectangle.

                                                                                                                                                            pv_param2:  NULL
                                                                                                                                                            */

#define WGL_CMD_TEXT_SET_HIGHLIGHT_STRING   WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 16) /**<  This command set highlight string index in string list.      

                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                            pv_param1:  A UINT16 integer to specify highlight string index in string list.

                                                                                                                                                            pv_param2:  NULL
                                                                                                                                                            */


#define WGL_CMD_TEXT_PLAY_NEXT_STRING    WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 17) /**<  This command remove the header of the string list.      

                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                            pv_param1:   NULL 

                                                                                                                                                            pv_param2:   NULL
                                                                                                                                                            */

#define WGL_CMD_TEXT_PLAY_PREV_STRING    WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 18) /**<  This command remove the end of the string list.      

                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                             pv_param1:   NULL 

                                                                                                                                                            pv_param2:   NULL
                                                                                                                                                            */

#define WGL_CMD_TEXT_SET_INTERLACED      WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 19) /**<  This command set the interlaced of the text.      

                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                            pv_param1:   A UINT8 Interlaced

                                                                                                                                                            pv_param2:   NULL
                                                                                                                                                            */

#define WGL_CMD_TEXT_GET_INTERLACED      WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 20) /**<  This command get  the interlaced of the text.       

                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                            pv_param1:   A UINT8  point to the interlaced

                                                                                                                                                            pv_param2:   NULL
                                                                                                                                                            */

#define WGL_CMD_TEXT_GET_FONT_HANDLE     WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 21) /**<  This command get  the interlaced of the text.       

                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                            pv_param1:   A UINT8  point to the interlaced

                                                                                                                                                            pv_param2:   NULL
                                                                                                                                                            */


#define WGL_CMD_TEXT_GET_CNT_WIDTH     WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 22) /**<  This command get  the interlaced of the text.       

                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                            pv_param1:   A UIN16  point to the width of wgl

                                                                                                                                                            pv_param2:   NULL
                                                                                                                                                            */
#define WGL_CMD_TEXT_SET_BREAK_CHARS     WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 23) /**<  This command set break chars which is used to break lines for multiple line style.
                                                                                                                                                            
                                                                                                                                                            If user dose not set break chars using this command then system will use {0x002D, 0x0020, 0x00AD} as default break chars.

                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                            pv_param1:   UIN16[]  array  which contains break chars and can not be NULL

                                                                                                                                                            pv_param2:   UINT8 value to indicate break chars's length in pv_param1 and maximum length is 3.
                                                                                                                                                            */                                                                                                                                                            
#define WGL_CMD_TEXT_GET_FONT_POINT_SIZE_FROM_HEIGHT	WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 24)      /* THis command get the font point size from pixel height value.*/                                                                                                                                                      

#define WGL_CMD_TEXT_SET_CUSTOM_ALIGN          WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 25)    /**< This command is used to set Text Static's foreground alignment.
    
                                                                                                                                                            related parameters in c_wgl_do_cmd(),
    
                                                                                                                                                            pv_param1:   Specifies the alignment style. It can be WGL_TEXT_H_ALIGN_CUSTOM. 
    
                                                                                                                                                            pv_param2:   Reserved. It must be NULL.
                                                                                                                                                            */


#define WGL_CMD_TEXT_SET_IGNORE_NEWLINE          WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_TEXT, 26)    /**< This command is used to set Text Static's foreground alignment.
    
                                                                                                                                                            related parameters in c_wgl_do_cmd(),
    
                                                                                                                                                            pv_param1:   Specifies the alignment style. It can be WGL_TEXT_H_ALIGN_CUSTOM. 
    
                                                                                                                                                            pv_param2:   Reserved. It must be NULL.
                                                                                                                                                            */
                                                                                                                                                            

#define TEXT_CNT_PRIVATE_START          27      /**<(WGL_CMD_TEXT_SET_MAX_DIS_LINE - WGL_CMD_TEXT_SET_CUSTOM_ALIGN + 1)        */


/*Specific Notify Code*/
#define WGL_NC_TEXT_START_LINE_CHANGE   ((UINT32)WGL_NC_WIDGET_SPF_START + 0)    /**<        */


/* text scrolling styles */
#define WGL_TEXT_SCRL_FROM_FIRST_CHAR     ((UINT16) 0x0001)    /**< indicates scrolling from first char of text string.       */
#define WGL_TEXT_SCRL_TILL_LAST_CHAR      ((UINT16) 0x0002)    /**< indicates scrolling till last char of text string.       */
#define WGL_TEXT_SCRL_REPEAT              ((UINT16) 0x0004)    /**< indicates scrolling will repeat.     */
#define WGL_TEXT_SCRL_SMART               ((UINT16) 0x0008)    /**<        */

#define WGL_TEXT_SCRL_MIN_DELAY          10    /**<    indicates scrolling delay time can't be small than this min value.    */

#define WGL_TEXT_NONE_ALIGN_CUSTOM              ((UINT8) 0x00)    /**<        */

#define WGL_TEXT_H_ALIGN_CUSTOM              ((UINT8) 0x01)    /**<  In horizental direction, the longest line will be center alignment and other lines's start position will be same with the longest line      */



/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
/*Control Key Map*/
/*------------------------------------------------------------------*/
/*! @struct WGL_TEXT_KEY_MAP_T
 *  @brief This structure holds text navigation key code.
 *  @code
 *  typedef struct _WGL_TEXT_KEY_MAP_T
 *  {
 *      UINT32          ui4_line_up;
 *      UINT32          ui4_line_down;
 *      UINT32          ui4_page_up;
 *      UINT32          ui4_page_down;
 *  } WGL_TEXT_KEY_MAP_T;
 *  @endcode
 *  @li@c  ui4_line_up                                       -indicates key code to previous line
 *  @li@c  ui4_line_down                                     -indicates key code to next line
 *  @li@c  ui4_page_up                                       -indicates key code to previous page
 *  @li@c  ui4_page_down                                     -indicates key code to next page
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TEXT_KEY_MAP_T
{
    UINT32          ui4_line_up;
    UINT32          ui4_line_down;
    UINT32          ui4_page_up;
    UINT32          ui4_page_down;
} WGL_TEXT_KEY_MAP_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_TEXT_SCRL_T
 *  @brief This structure holds text scrolling begins parameter for single line mode
 *  @code
 *  typedef struct _WGL_TEXT_SCRL_T
 *  {    
 *      UINT32              ui4_ms_delay;
 *      UINT16              ui2_style;
 *      UINT16              ui2_scrl_count; 
 *      BOOL                b_sync;    
 *  } WGL_TEXT_SCRL_T;
 *  @endcode
 *  @li@c  ui4_ms_delay                                      -indicates delay timespace in ms
 *  @li@c  ui2_style                                         -indicates scrolling style 
 *  @li@c  ui2_scrl_count                                    -indicates scrolling repeat count when scrolling style contains WGL_TEXT_SCRL_REPEAT.
 *  @li@c  b_sync                                            -indicates timer sync mode.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TEXT_SCRL_T
{    
    UINT32              ui4_ms_delay;
    UINT16              ui2_style;
    UINT16              ui2_scrl_count; 
    BOOL                b_sync;    
} WGL_TEXT_SCRL_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_TEXT_FONT_ATTR_T
 *  @brief This structure holds text font attribute type and value.
 *  @code
 *  typedef struct _WGL_TEXT_FONT_ATTR_T
 *  {    
 *      WGL_TEXT_FONT_ATTR_TYPE_T t_font_attr_type;
 *      VOID*                pv_attr_data;    
 *  } WGL_TEXT_FONT_ATTR_T;
 *  @endcode
 *  @li@c  t_font_attr_type                                  -indicates attribute type 
 *  @li@c  pv_attr_data                                      - indicates attribute value, for WGL_TEXT_FONT_ATTR_UNIFORM_WIDTH attribute type, pv_attr_data specify a UINT16 value.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TEXT_FONT_ATTR_T
{    
    WGL_TEXT_FONT_ATTR_TYPE_T t_font_attr_type;
    VOID*                pv_attr_data;    
} WGL_TEXT_FONT_ATTR_T;

#ifdef __cplusplus
}
#endif

#endif
/*----------------------------------------------------------------------------*/
/*! @} */   /* end of groupMW_WGL_TEXT*/
/*----------------------------------------------------------------------------*/
