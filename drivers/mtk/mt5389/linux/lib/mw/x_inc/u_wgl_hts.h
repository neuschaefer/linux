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
/*! @file u_wgl_hts.h 
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
/*! @defgroup groupMW_WGL_HTS Hyper Text  Widget
 *
 *  @ingroup groupMW_WGL
 *  @brief Provides definitions,commands of the Hyper Text  widget.The Hyper Text widget provides capability of showing different sections of texts which contain different attributes.
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW_WGL
 *  @{ 
 */
/*----------------------------------------------------------------------------*/ 

#ifndef _U_WGL_HTS_H_
#define _U_WGL_HTS_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_wgl_hts_cnt_common.h"
#include "u_wgl_bdr_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define WGL_CMD_HTS_START_NEW_PARAG             WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 0)   
/**< 
This command creates a new paragraph and appends the new paragraph at the back of the current paragraph. After this command, the new paragraph becomes the current paragraph.

Input arguments

pv_param1	Specifies a pointer to the structure WGL_HTS_PARAG_ATTR_T containing the attributes of the new paragraph to be appended. If this argument is NULL, the attributes of the new paragraph will be copied from the attributes of the previous paragraph. Otherwise, this argument must be of type (WGL_HTS_PARAG_ATTR_T *).

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument specified in the argument pv_param1 is invalid. For example, if the justification is invalid, the error code is returned.

< 0	Returns other internal error codes.
*/

#define WGL_CMD_HTS_CUR_PARAG_SET               WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 1)  
/**< 
This commands changes the attributes of the current paragraph. 

Input arguments

pv_param1	Specifies a pointer to the structure WGL_HTS_PARAG_ATTR_T containing the new attributes to be set to the current paragraph. This argument must be of type (WGL_HTS_PARAG_ATTR_T *) and cannot be NULL. 

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	When one of the following cases occurs:

 	1,The argument pv_param1 is NULL.

 	2,The argument specified in the argument pv_param1 is invalid. For example, if the justification is invalid, the error code is returned.

< 0	Returns other internal error codes.
*/

#define WGL_CMD_HTS_CUR_PARAG_GET               WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 2)  
/**<
This command retrieves the attributes of the current paragraph. 

Input arguments

pv_param2	Reserved. It must be NULL.

Output arguments

pv_param1	A pointer to a single WGL_HTS_PARAG_ATTR_T buffer to receive the paragraph attributes. Note that this argument must be of type (WGL_HTS_PARAG_ATTR_T *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGL_INV_ARG	The argument pv_param1 is NULL.
*/

#define WGL_CMD_HTS_CUR_PARAG_SET_JUSTIF        WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 3)   
/**< 
This command sets the justification of the current paragraph. 

Input arguments

pv_param1	Specifies the new justification type of the current paragraph. This argument must be of type WGL_HTS_JUSTIF_TYPE_T.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The specified justification type is invalid. 
*/

#define WGL_CMD_HTS_CUR_PARAG_GET_JUSTIF        WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 4)  
/**< 
This command retrieves the justification of the current paragraph. 

Input arguments

pv_param2	Reserved. It must be NULL.

Output arguments

pv_param1	A pointer of a single WGL_HTS_JUSTIF_TYPE_T buffer to receive the justification of the current paragraph. Note that this argument must be of type (WGL_HTS_JUSTIF_TYPE_T *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGL_INV_ARG	The argument pv_param1 is NULL.
*/

#define WGL_CMD_HTS_CUR_PARAG_SET_INDENT        WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 5) 
/**<  
This command sets the indent size of the current paragraph. The indent size is counted in "space" character units. The pixel size of the indent is counted by multiplying the indent size with the pixel size of the "space" character. 

Input arguments

pv_param1	Specifies the indent size of the current paragraph in "space" character count. If this argument is 0, the paragraph would not have indent. This argument must be of type UINT16.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.
*/

#define WGL_CMD_HTS_CUR_PARAG_GET_INDENT        WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 6) 
/**<  
This command retrieves the indent size (in "space" character count) of the current paragraph. 

Input arguments

pv_param2	Reserved. It must be NULL.

Output arguments

pv_param1	Pointer to a single UINT16 buffer to receive the indent size. Hence this argument must be of type (UINT16 *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGL_INV_ARG	The argument pv_param1 is NULL.
*/

#define WGL_CMD_HTS_RESET                       WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 7) 
/**< 

This command resets the Hyper Text widget. Like WGL_CMD_HTS_RESET command, all the text, images, tabs, sections and paragraphs of the article are cleared. In addition, the following attributes would be reset to the default value as the Hyper Text widget is just created.

 	The attributes of the current paragraph, which are managed by WGL_CMD_HTS_CUR_PARAG_* commands
 	The default attributes for newly created section, which are managed by WGL_CMD_HTS_DFT_SEC_* commands

The following attributes would not be reset:

 	The scroll line step size
 	
 	The key map
 	
 	The page up/down indicator images
 	
 	The insets of border 
 	
 	The insets of content
 	
 	The background color of the content
 	
 	The background color of the viewport
 	
 	The background image of the content
 	
 	The background image of the viewport
 	
Input arguments

pv_param1	Reserved. It must be NULL.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.
*/

#define WGL_CMD_HTS_CLEAR_ALL                   WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 8) 
/**<
This command clears all the text, images, tabs, sections and paragraphs of the article. After this command, the article becomes empty except that no attributes return to default values. 

Input arguments

pv_param1	A BOOL value indicates whether the Hyper Text should auto repaint after this command or not.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.
*/

#define WGL_CMD_HTS_APPEND_TEXT_SEC             WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 9)   
/**< 
This command appends a text section at the end of the article. The text and the attributes of the newly created text section can be specified in the argument pv_param1. If pv_param1 is NULL, the text section is empty without any text and the attributes of the new text section would be derived from the default section attributes set, which are managed by the WGL_CMD_HTS_DFT_SEC_* command set. 

Input arguments

pv_param1	Contains the information for the new text section. If this argument is NULL, an empty text section will be created and the attributes will be derived from the default section attributes set. Otherwise, this argument must be of type (WGL_HTS_TEXT_INFO_T *) to specify the text and attributes of the new section.

Output arguments

pv_param2	A pointer of a single UINT16 buffer to received the section id of the newly created text section. This argument must be of type (UINT16 *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	In one of the following cases:
 	The argument pv_param2 is NULL
 	The text attributes specified in pv_param2 is invalid
 	The input string is invalid.
*/

#define WGL_CMD_HTS_APPEND_IMG_SEC              WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 10)  
/**< 
This command appends an image section at the end of the article. The image handle and the attributes can be specified in the argument pv_param1. If pv_param1 is NULL, an empty image section will be created and some attributes of the image section will be derived from the default section attributes set. 

Input arguments

pv_param1	Contains the information for the new image section. If this argument is NULL, an empty image section will be created and some attributes will be derived form the default section attributes set. Otherwise, this argument must be of type (WGL_HTS_IMG_INFO_T *) to specify the image and attributes of the section. 

Output arguments

pv_param2	A pointer of a single UINT16 buffer to received the section id of the newly created image section. This argument must be of type (UINT16 *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	In one of the following cases:

 	The argument pv_param2 is NULL

 	The specified image is invalid

 	The specified section vertical anchor is invalid.
*/

#define WGL_CMD_HTS_APPEND_TAB                  WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 11) 
/**< 
This command appends a tab with a specified size.

Input arguments

pv_param1	Specifies the size of the tab in "space" character count. This argument must be of type UINT16.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

*/

#define WGL_CMD_HTS_DFT_SEC_SET_V_ANCHOR        WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 12) 
/**<
This command sets the vertical anchor of the default section attribute set for newly appended sections.

Input arguments

pv_param1	Contains the vertical anchor of the default section attributes set. This argument must be of type WGL_HTS_V_ANCHOR_T. 

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The vertical anchor specified in pv_param1 is invalid. 
*/

#define WGL_CMD_HTS_DFT_SEC_GET_V_ANCHOR        WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 13)  
/**<
This command gets the vertical anchor of the default section attributes set.  

Input arguments

pv_param2	Reserved. It must be NULL.

Output arguments

pv_param1	A pointer of a single WGL_HTS_V_ANCHOR_T buffer to receive the vertical anchor. Note that this argument must be of type (WGL_HTS_V_ANCHOR_T *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGL_INV_ARG	The argument pv_param1 is NULL.
*/

#define WGL_CMD_HTS_DFT_SEC_SET_FONT            WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 14) 
/**<
This command sets the font information of the default section attributes set for newly appended text sections. 

Input arguments

pv_param1	Contains the font information for the default attributes of newly appended text section. This argument must be of type (WGL_FONT_INFO_T *). If this argument is NULL, the default font information of the application will be used.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

< 0 	Returns some internal error code indicating the font information is invalid or other internal error. 
*/

#define WGL_CMD_HTS_DFT_SEC_GET_FONT            WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 15) 
/**< 
This command retrieves the current font information for the default section attributes set. 

Input arguments

pv_param2	Reserved. It must be NULL.

Output arguments

pv_param1	Contains an pointer of a single WGL_FONT_INFO_T buffer to receive the font information. Hence this argument must be of type (WGL_FONT_INFO_T *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGL_INV_ARG	The argument pv_param1 is NULL.	
*/

#define WGL_CMD_HTS_DFT_SEC_SET_COLOR           WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 16) 
/**< 
This command sets the default color for newly appended sections, which is stored in the default section attributes set. The target of the color can be specified in the argument pv_param1, while the argument pv_param2 contains the color. 

Input arguments

pv_param1	Indicates the target of the color to be set. This argument must be of type UINT8. This command currently supports the following color targets:

 	WGL_CLR_HTS_SEC_TEXT: indicates the default text color for newly appended text sections

 	WGL_CLR_HTS_SEC_TEXT_BK: indicates the default text background color for newly appended text sections

pv_param2	Contains the color. This argument must be of type (GL_COLOR_T *) and cannot be NULL. 

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param2 is NULL. 

WGLR_NOT_SUPPORT_TARGET  The color target specified in the argument pv_param1 is invalid
*/

#define WGL_CMD_HTS_DFT_SEC_GET_COLOR           WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 17)  
/**< 
This command retrieves the default color for newly appended sections, which is stored in the default section attributes set. 

Input arguments

pv_param1	Indicates the target of the color to be retrieved. This argument must be of type UINT8. This command currently supports the following color targets:

 	WGL_CLR_HTS_SEC_TEXT: indicates the default text color for newly appended text sections

 	WGL_CLR_HTS_SEC_TEXT_BK: indicates the default text background color for newly appended text sections


Output arguments

pv_param2	Contains a pointer of a single GL_COLOR_T buffer to receive the specified color. Hence this argument must be of type (GL_COLOR_T *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param2 is NULL.

WGLR_NOT_SUPPORT_TARGET	
The color target specified in the argument pv_param1 is invalid
*/

#define WGL_CMD_HTS_SEC_REPAINT                 WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 18) 
/**< 
This command partially repaints the specified section.

Input arguments

pv_param1	Contains the id of the section to be repainted. This argument must be of type UINT16.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

< 0	The section is invalid or other internal error.
*/

#define WGL_CMD_HTS_SEC_SET_V_ANCHOR            WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 19) 
/**< 
This command sets the vertical anchor or the specified section.

Input arguments

pv_param1	Contains the id of the specified section. This argument must be of type UINT16.

pv_param2	Contains the vertical anchor. This argument must be of type WGL_HTS_V_ANCHOR_T.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The section is invalid or the specified vertical anchor is invalid.
*/

#define WGL_CMD_HTS_SEC_GET_V_ANCHOR            WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 20) 
/**< 
This command retrieves the vertical anchor or the specified section. 

Input arguments

pv_param1	Contains the id of the specified section. This argument must be of type UINT16.

Output arguments

pv_param2	Contains a pointer of a single WGL_HTS_V_ANCHOR_T buffer to receive the vertical anchor of the specified section. Hence this argument must be of type (WGL_HTS_V_ANCHOR_T *) and cannot be NULL. 

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The section id is invalid or the argument pv_param2 is NULL. 
*/

#define WGL_CMD_HTS_SEC_SET_IMG                 WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 21) 
/**< 
This command sets the image handle and related attributes of a specified image section. If the image handle is NULL_HANDLE, the image section would become empty.

Input arguments

pv_param1	Contains the id of the specified image section. This argument must be of type UINT16 and the specified section must be an image section.

pv_param2	Specifies the image information contained in the structure WGL_HTS_IMG_INFO_T. Hence this argument must be of type (WGL_HTS_IMG_INFO_T *) and cannot be NULL. If the field h_img of the WGL_HTS_IMG_INFO_T structure is NULL_HANDLE, the image section becomes empty.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	In one of the following cases: 

 	The specified section id is invalid.

 	The argument pv_param2 is NULL.

 	The specified vertical anchor is invalid.

WGLR_INV_HANDLE	The specified image handle is invalid. 

WGLR_INTERNAL_ERROR	Some internal errors occur.

< 0 	the specified section is not an image section or other internal errors.
*/

#define WGL_CMD_HTS_SEC_GET_IMG                 WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 22) 
/**< 
This command retrieves the image information of the specified image section.

Input arguments

pv_param1	Contains the id of the specified section. This argument must be of type UINT16 and the specified section must be an image section.

Output arguments

pv_param2	Contains the pointer of a single WGL_HTS_IMG_INFO_T buffer to receive the image information. Hence this argument must be of type (WGL_HTS_IMG_INFO_T *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	In one of the following cases: 

 	The specified section id is invalid.

 	The argument pv_param2 is NULL.

< 0 	the specified section is not an image section or other internal errors.
*/

#define WGL_CMD_HTS_SEC_SET_TEXT                WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 23)  
/**<  
This command sets the text of a text section. If an empty string is set, the text section becomes empty. The text should be specified in the WGL_HTS_STRING_T structure and the fields of WGL_HTS_STRING_T should be set as following.

If a non-empty string is to be set:

 	ui2_size_buf: indicates the size of the UTF16_T buffer specified in the field w2s_str. Note that the size is counted in UTF16_T character unit. Note that ui2_size_buf must be larger than ui2_len_str. 

 	w2s_str: the pointer of the UTF16_T string. The UTF16_T string is not necessary NULL-terminated since its length is indicated in the ui2_len_str field. 

 	ui2_len_str: indicates the length of the UTF16_T string. Note that the field is counted in UTF16_T character unit. Note that this field cannot exceed (ui2_size_buf-1).


If an empty string is to be set:

 	ui2_size_buf: must be zero 

 	w2s_str: must be NULL 

 	ui2_len_str: must be zero.


Input arguments

pv_param1	Contains the id of the specified section. This argument must be of type UINT16 and the specified section must be a text section.

pv_param2	Specifies the text contained in the WGL_HTS_STRING_T structure. Note that the string is copied into the internal buffer of the Hyper Text widget, therefore the application doesn't need to permanently keep the string after this command. For example, the buffer of the string can be allocated in stack, instead of x_mem_alloc() API. Note that the argument must be of type (WGL_HTS_STRING_T *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	In one of the following cases:
 	The section id is invalid
 	The argument pv_param2 is NULL
 	The string information contained in the argument pv_param2 is invalid.

< 0	the specified section is not a text section or other internal errors.
*/

#define WGL_CMD_HTS_SEC_GET_TEXT                WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 24)  
/**< 
This command retrieves the text of a specified text section. The application needs to prepare a UTF16_T string buffer to receive the text. The length and pointer of the UTF16_T string buffer have to be packed in the structure WGL_HTS_STRING_T.  Before retrieving the text, the fields of WGL_HTS_STRING_T structure should be set as following:

 	ui2_size_buf: indicates the size of the UTF16_T buffer specified in the field w2s_str. Note that the size is counted in UTF16_T character unit.
 	w2s_str: the pointer of the UTF16_T string buffer to receive the text. The received UTF16_T string would be always appended by a UTF16_T NULL-character. Note that by the definition of WGL_HTS_STRING_T structure, it's possible that the received string contains any UTF16_T character, including the UTF16_T NULL character before the appended UTF16_T NULL character at the end.
 	ui2_len_str: must be zero. After the command, the length of the UTF16_T string will be contained in this field.

Input arguments

pv_param1	Contains the id of the specified section. This argument must be of type UINT16 and the specified section must be a text section.

Output arguments

pv_param2	Contains a pointer of a single WGL_HTS_STRING_T buffer and a UTF16_T string buffer to receive the text. In the WGL_HTS_STRING_T structure, the pointer of the UTF16_T string buffer is indicated in the w2s_str field and the size of the UTF16_T string buffer is indicated in the ui2_size_buf field. The ui2_len_str field must be zero. Note that the argument pv_param2 must be of type (WGL_HTS_STRING_T *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	In one of the following cases:

 	The specified section id is invalid
 	
 	The specified section is not a text section
 	
 	The argument pv_param2 is NULL.
 	
 	The ui2_size_buf field of WGL_HTS_STRING_T structure is zero
 	
 	The w2s_str field of WGL_HTS_STRING_T structure is NULL
 	
 	The ui2_len_str field of WGL_HTS_STRING_T structure is not zero
 	
 	The size of the UTF16_T string buffer is too small to contain the whole text of the specified text section.
*/

#define WGL_CMD_HTS_SEC_SET_FONT                WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 25) 
/**<
This command sets the font information of the specified text section. 

Input arguments

pv_param1	Contains the id of the specified text section. This argument must be of type UINT16. Note that the specified section must be a text section.

pv_param2	Contains the font information. If this argument is NULL, the application default font will be used. Otherwise, this argument must be of type WGL_FONT_INFO_T. 

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	In one of the following cases:

 	The specified section id is invalid
 	
 	The specified section is not a text section

< 0	the specified font information is invalid or indicates other internal errors.
*/

#define WGL_CMD_HTS_SEC_GET_FONT                WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 26)
/**<
This command retrieves the font information of the specified text section.

Input arguments

pv_param1	Contains the id of the specified section. This argument must be of type UINT16. Note that the specified section must be a text section.

Output arguments

pv_param2	Contains the pointer of a single WGL_FONT_INFO_T buffer to receive the font information. Hence this argument must be of type (WGL_FONT_INFO_T *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	In one of the following cases:

 	The specified section id is invalid
 	
 	The specified section is not a text section
 	
 	The argument pv_param2 is NULL.

< 0	indicates other internal errors.
*/

#define WGL_CMD_HTS_SEC_SET_COLOR               WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 27)  
/**<
This command sets the color of a specified section. The section id and the target of the color can be specified in the argument pv_param1, while the argument pv_param2 contains the color.

Input arguments

pv_param1	Contains the section id and the target of the color. Note that this argument must be specified as WGL_PACK_2 (ui2_sec_id, ui1_color_target). The first element of WGL_PACK_2 () macro must be of type UINT16 while the second element must be of type UINT8. This command currently supports the following color targets:

 	WGL_CLR_HTS_SEC_TEXT: indicates the text color of the specified text section.

 	WGL_CLR_HTS_SEC_TEXT_BK: indicates the text background color of the specified text section.

pv_param2	Contains the color. This argument must be of type (GL_COLOR_T *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	In one of the following cases:

 	The argument pv_param2 is NULL. 
 	
 	The section id is invalid
 	
 	The specified section is not a text section

WGLR_NOT_SUPPORT_TARGET	The color target is invalid
*/

#define WGL_CMD_HTS_SEC_GET_COLOR               WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 28) 
/**<  
This command retrieves the color of a specified text section.

Input arguments

pv_param1	Contains the section id and the target of the color. Note that this argument must be specified as WGL_PACK_2 (ui2_sec_id, ui1_color_target). The first element of WGL_PACK_2 () macro must be of type UINT16 while the second element must be of type UINT8. This command currently supports the following color targets:

 	WGL_CLR_HTS_SEC_TEXT: indicates the text color of the specified text section.

 	WGL_CLR_HTS_SEC_TEXT_BK: indicates the text background color of the specified text section.

Output arguments

pv_param2	Contains the pointer of a single GL_COLOR_T buffer to receive the color. Hence this argument must be of type (GL_COLOR_T *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	In one of the following cases:

 	The argument pv_param2 is NULL. 
 	
 	The section id is invalid
 	
 	The specified section is not a text section

WGLR_NOT_SUPPORT_TARGET	
The color target is invalid
*/

#define WGL_CMD_HTS_SEC_GET_LOCATION            WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 29) 
/**<
This command queries the range of lines that the specified range of sections occupies. 

Input arguments

pv_param1	Contains the section id's of the start section and end section of the section range. This argument must be specified as WGL_PACK_2 (ui2_sec_id_start, ui2_sec_id_end). The two elements of WGL_PACK_2 () macro must be of type UINT16. 

Output arguments 

pv_param2	Contains the pointer of a single WGL_HTS_LINE_RANGE_T buffer to receive the line range. Hence this argument must be of type (WGL_HTS_LINE_RANGE_T *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	 The argument pv_param2 is NULL.

< 0	in one of the following cases:

 	The section range specified in pv_param1 is invalid.
 	
 	Other internal errors
*/

#define WGL_CMD_HTS_GET_PAGE_NUM                WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 30) 
/**< 
This command queries the current number of pages according to the content of the article and the current size of the viewport. Note that any changes to the article content or viewport size may also change the page number.

Input arguments

pv_param2	Reserved. It must be NULL.

Output arguments

pv_param1	Contains the pointer of a single UINT16 buffer to receive the page number. Note that this argument must be of type (UINT16 *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGL_INV_ARG	The argument pv_param1 is NULL.
*/

#define WGL_CMD_HTS_GET_LINE_NUM                WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 31)  
/**< 
This command queries the total number of lines.

Input arguments

pv_param2	Reserved. It must be NULL.

Output arguments

pv_param1	Contains the pointer of a single UINT16 buffer to receive the line number. Note that this argument must be of type (UINT16 *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGL_INV_ARG	The argument pv_param1 is NULL.
*/

#define WGL_CMD_HTS_GET_PARAG_NUM               WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 32) 
/**<
This command queries the total number of paragraphs.

Input arguments

pv_param2	Reserved. It must be NULL.

Output arguments

pv_param1	Contains the pointer of a single UINT16 buffer to receive the paragraph number. Note that this argument must be of type (UINT16 *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGL_INV_ARG	The argument pv_param1 is NULL.
*/

#define WGL_CMD_HTS_SCRL_UP                     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 33) 
/**<
This command scrolls up the article for several lines. This command has the same effect as that when the user presses the scroll-up key by the remote controller. The default line number for scrolling up is 1 and could be changed by the WGL_CMD_HTS_SET_SCRL_LINE_STEP command. 

One of the following notifications could be arisen after this command:

 	WGL_NC_HTS_REACH_TOP: arisen if the command is sent when top of the article is reached and cannot be scrolled up anymore.

 	WGL_NC_HTS_VP_MOVED: arisen if the command has moved the article

Input arguments

pv_param1	Contains a BOOL flag indicating whether the Hyper Text should be repainted or not.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The value specified in pv_param1 is neither TRUE nor FALSE.

< 0	other internal errors
*/

#define WGL_CMD_HTS_SCRL_DOWN                   WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 34) 
/**<
This command scrolls down the article for several lines. This command has the same effect as that when the user presses the scroll-down key by the remote controller. The default line number for scrolling down is 1 and could be changed by the WGL_CMD_HTS_SET_SCRL_LINE_STEP command. 

One of the following notifications could be arisen after this command:

 	WGL_NC_HTS_REACH_BOTTOM: arisen if the command is sent when bottom of the article is reached and cannot be scrolled down anymore.

 	WGL_NC_HTS_VP_MOVED: arisen if the command has moved the article

Input arguments

pv_param1	Contains a BOOL flag indicating whether the Hyper Text should be repainted or not.

pv_param2	Reserved. It must be NULL.

Returns

WGL_OK	The function call is successful.

WGLR_INV_ARG	The value specified in pv_param1 is neither TRUE nor FALSE.

< 0	other internal errors
*/

#define WGL_CMD_HTS_PAGE_UP                     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 35) 
/**<
This command pages up the article. This command has the same effect as that when the user presses the page-up key by the remote controller. If the widget style WGL_STL_HTS_NEW_PAGE_KEEP_LAST_LINE  is turned on, the top line of the original page would be remained as the bottom line of the new page. 

One of the following notifications could be arisen after this command:

 	WGL_NC_HTS_REACH_TOP: arisen if the command is sent when top of the article is reached and cannot be paged up anymore.

 	WGL_NC_HTS_VP_MOVED: arisen if the command has moved the article

Input arguments

pv_param1	Contains a BOOL flag indicating whether the Hyper Text should be repainted or not.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The value specified in pv_param1 is neither TRUE nor FALSE.

< 0	other internal errors
*/

#define WGL_CMD_HTS_PAGE_DOWN                   WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 36) 
/**< 
This command pages down the article. This command has the same effect as that when the user presses the page-down key by the remote controller. If the widget style WGL_STL_HTS_NEW_PAGE_KEEP_LAST_LINE  is turned on, the bottom line of the original page would be remained as the top line of the new page. 

One of the following notifications could be arisen after this command:

 	WGL_NC_HTS_REACH_BOTTOM: arisen if the command is sent when bottom of the article is reached and cannot be paged down anymore.

 	WGL_NC_HTS_VP_MOVED: arisen if the command has moved the article

Input arguments

pv_param1	Contains a BOOL flag indicating whether the Hyper Text should be repainted or not.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The value specified in pv_param1 is neither TRUE nor FALSE.

< 0	other internal errors
*/

#define WGL_CMD_HTS_SET_FIRST_VISIBLE_LINE      WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 37)
/**< 
This command sets the first visible line of the viewport. If the command has moved the article, the notification WGL_NC_HTS_VP_MOVED would be arisen.

Input arguments

pv_param1	Specifies the index of the line to be moved to the first visible line of the viewport. Note that this argument must be of type UINT16. 

pv_param2	Contains a BOOL flag indicating whether the Hyper Text should be repainted or not.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	In one of the following cases:

 	The value specified in pv_param2 is neither TRUE nor FALSE.

 	The specified line index is invalid

< 0	other internal errors
*/

#define WGL_CMD_HTS_GET_FIRST_VISIBLE_LINE      WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 38) 
/**<
This command gets the index of the first visible line. 

Input arguments

pv_param2	Reserved. It must be NULL.

Output arguments

pv_param1	Contains the pointer of a single UINT16 buffer to receive the line index. Note that this argument must be of type (UINT16 *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGL_INV_ARG	The argument pv_param1 is NULL.
*/

#define WGL_CMD_HTS_GET_LAST_VISIBLE_LINE       WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 39) 
/**< 
This command gets the index of the last visible line.

Input arguments

pv_param2	Reserved. It must be NULL.

Output arguments

pv_param1	Contains the pointer of a single UINT16 buffer to receive the line index. Note that this argument must be of type (UINT16 *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGL_INV_ARG	The argument pv_param1 is NULL.
*/

#define WGL_CMD_HTS_SET_SCRL_LINE_STEP          WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 40)
/**< 
This command sets the number of lines each time the scroll-down or scroll-up could move the article. By default, each scroll-down or scroll-up scrolls one line.

Input arguments

pv_param1	Contains the new scroll line step. This argument must be of type UINT16.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.
*/

#define WGL_CMD_HTS_GET_SCRL_LINE_STEP          WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 41) 
/**< 
This command gets the number of lines each time the scroll-down or scroll-up could move the article.

Input arguments

pv_param2	Reserved. It must be NULL.

Output arguments

pv_param1	Contains the pointer of a single UINT16 buffer to receive the scroll line step. Note that this argument must be of type (UINT16 *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGL_INV_ARG	The argument pv_param1 is NULL.
*/

#define WGL_CMD_HTS_SET_KEY_MAP                 WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 42)
/**< 
This command sets the key map.

Input arguments

pv_param1	Contains the key map. This argument must be of type (WGL_HTS_KEY_MAP_T *) and cannot be NULL. The new key map will be copied into the internal buffer of Hyper Text widget.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.
*/

#define WGL_CMD_HTS_GET_KEY_MAP                 WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_HTS, 43) 
/**< 
This command gets the key map.

Input arguments

pv_param2	Reserved. It must be NULL.

Output arguments

pv_param1	Contains the pointer of a single WGL_HTS_KEY_MAP_T buffer to receive the key map. Note that this argument must be of type (WGL_HTS_KEY_MAP_T *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGL_INV_ARG	The argument pv_param1 is NULL.
*/



/* styles */
/**<If this style is specified, the Hyper Text widget always shows the last line in the viewport even if this line is partially visible on the bottom boundary of the viewport. Otherwise, the partial visible row would not be shown.         */
#define WGL_STL_HTS_SHOW_HALF_VISIBLE_LINE      (((UINT32)(WGL_STL_WGT_SPF_START))<<0)  
/**< If this style is specified, the Hyper Text widget always keeps a line originally staying in the previous page whenever a new page is generated during the page-up or page-down operation is performed.  */
#define WGL_STL_HTS_NEW_PAGE_KEEP_LAST_LINE     (((UINT32)(WGL_STL_WGT_SPF_START))<<1)   
/**<If this style is specified, the Hyper Text widget keeps the last page filled with lines when performing page-down operations to the last page.  */
#define WGL_STL_HTS_FORCE_FILL_LAST_PAGE        (((UINT32)(WGL_STL_WGT_SPF_START))<<2)  

#define WGL_STL_HTS_VP_NO_BK                    (((UINT32)(WGL_STL_WGT_SPF_START))<<3)/**<If this sytle is speicified, the Hyper Text widget shall not render view port's background.   */

#define WGL_STL_HTS_LAST_STYLE                  WGL_STL_HTS_VP_NO_BK /**< last style equals to WGL_STL_HTS_VP_NO_BK       */ 

/* notifications */
#define WGL_NC_HTS_REACH_TOP                    ((UINT32)WGL_NC_WIDGET_SPF_START + 0)  
/**<
This notification is arisen when the top of the article has been reached and thus no more lines on the top could be scrolled into the viewport. This notification may be arisen when the application sends the WGL_CMD_HTS_SCRL_UP or WGL_CMD_HTS_PAGE_UP commands, or when a user tries to scroll up or page up the article by remote control keys. 

Notify Associated Data

The associated data for this notify is WGL_NOTIFY_DATA_T without any data in its u member, and its ui4_nc_code is WGL_NC_HTS_REACH_TOP.
*/

#define WGL_NC_HTS_REACH_BOTTOM                 ((UINT32)WGL_NC_WIDGET_SPF_START + 1) 
/**< 
This notification is arisen when the end of the article has been reached and thus no more lines below the article could be scrolled into the viewport. This notification may be arisen when the application sends the WGL_CMD_HTS_SCRL_DOWN or WGL_CMD_HTS_PAGE_DOWN commands, or when a user tries to scroll down or page down the article by remote control keys.

Notify Associated Data

The associated data for this notify is WGL_NOTIFY_DATA_T without any data in its u member, and its ui4_nc_code is WGL_NC_HTS_REACH_BOTTOM
*/

#define WGL_NC_HTS_VP_MOVED                     ((UINT32)WGL_NC_WIDGET_SPF_START + 2)  
/**<
This notification is arisen when the position or range (between first visible line and last visible line) of the article in the viewport is changed. This notification may be arisen in the following scenarios: 

 	When the Hyper Text widget is created

 	When the size of the Hyper Text widget is changed

 	When the border insets of the Hyper Text widget is changed

 	When the content insets of the Hyper Text widget is changed

Notify Associated Data

The associated data for this notify is WGL_NOTIFY_DATA_T without any data in its u member, and its ui4_nc_code is WGL_NC_HTS_VP_MOVED.
*/


/* invalid section id */
#define WGL_HTS_INV_SEC_ID                      ((UINT16)0xF000)    
/**< Indicates the invalid section id. If the command WGL_CMD_HTS_APPEND_TEXT_SEC or WGL_CMD_HTS_APPEND_IMG_SEC failed, the section id returned is WGL_HTS_INV_SEC_ID. */


/*------------------------------------------------------------------*/
/*! @enum WGL_HTS_JUSTIF_TYPE_T
 *  @brief  This enumeration indicates the justification type of a paragraph.
 *  @code
 *  typedef enum _WGL_HTS_JUSTIF_TYPE_T
 *  {
 *      WGL_HTS_JUSTIF_LEFT                 =  0,
 *      WGL_HTS_JUSTIF_RIGHT,
 *      WGL_HTS_JUSTIF_CENTER   
 *  } WGL_HTS_JUSTIF_TYPE_T;
 *  @endcode
 *  @li@c  WGL_HTS_JUSTIF_LEFT                               -indicates the left justification type of a paragraph.
 *  @li@c  WGL_HTS_JUSTIF_RIGHT                              -Indicates the right justification type of a paragraph.
 *  @li@c  WGL_HTS_JUSTIF_CENTER                             -Indicates the center justification type of a paragraph.
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_HTS_JUSTIF_TYPE_T
{
    WGL_HTS_JUSTIF_LEFT                 =  0,
    WGL_HTS_JUSTIF_RIGHT,
    WGL_HTS_JUSTIF_CENTER   

} WGL_HTS_JUSTIF_TYPE_T;


/*------------------------------------------------------------------*/
/*! @enum WGL_HTS_V_ANCHOR_T
 *  @brief This enumeration indicates the vertical anchor type of a section. 
 *  @code
 *  typedef enum _WGL_HTS_V_ANCHOR_T
 *  {
 *      WGL_HTS_V_ANCHOR_BOTTOM             = 0,
 *      WGL_HTS_V_ANCHOR_CENTER,
 *      WGL_HTS_V_ANCHOR_TOP    
 *  } WGL_HTS_V_ANCHOR_T;
 *  @endcode
 *  @li@c  WGL_HTS_V_ANCHOR_BOTTOM                           -Indicates a section to anchor on its bottom boundary for its vertical positioning in a line.
 *  @li@c  WGL_HTS_V_ANCHOR_CENTER                           -Indicates a section to anchor on its vertical center for its vertical positioning in a line.
 *  @li@c  WGL_HTS_V_ANCHOR_TOP                              -Indicates a section to anchor on its top boundary for its vertical positioning in a line.
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_HTS_V_ANCHOR_T
{
    WGL_HTS_V_ANCHOR_BOTTOM             = 0,
    WGL_HTS_V_ANCHOR_CENTER,
    WGL_HTS_V_ANCHOR_TOP    
} WGL_HTS_V_ANCHOR_T;


/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @struct WGL_HTS_FONT_REG_INFO_T
 *  @brief   This structure holds an entry of the font auxiliary information registration table. Each entry of this table contains the "key" fields and "value" fields of a specified font. When a font is specified by the application, the Hyper Text widget tries to search for a matching entry in the font info table by comparing the key fields, including s_font_name and e_font_size. If a matching entry is found, the attributes in the value fields, ui2_line_inset_top and ui2_line_inset_bottom, would be applied with the specified font.
 *  @code
 *  typedef struct _WGL_HTS_FONT_REG_INFO_T
 *  {
 *      CHAR                        			s_font_name[WGL_MAX_FONT_NAME];
 *      FE_FNT_SIZE                 		    e_font_size;
 *      UINT16                      			ui2_line_inset_top;
 *      UINT16                      			ui2_line_inset_bottom;
 *  } WGL_HTS_FONT_REG_INFO_T;    
 *  @endcode
 *  @li@c  s_font_name[WGL_MAX_FONT_NAME]                    -The key field contains the name of the font. 
 *  @li@c  e_font_size                                       -The key field contains the size of the font.
 *  @li@c  ui2_line_inset_top                                -The value field contains the expected top line inset for this font entry.
 *  @li@c  ui2_line_inset_bottom                             -The value field contains the expected bottom line inset for this font entry.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_HTS_FONT_REG_INFO_T
{
    /* key fields: font name & font size */
    CHAR                        			s_font_name[WGL_MAX_FONT_NAME];
    FE_FNT_SIZE                 		    e_font_size;
    
    /* font info */
    UINT16                      			ui2_line_inset_top;
    UINT16                      			ui2_line_inset_bottom;
    
} WGL_HTS_FONT_REG_INFO_T;    

/*------------------------------------------------------------------*/
/*! @struct  WGL_HTS_INIT_T
 *  @brief  This structure holds the initialization information that has to be passed as the pv_param argument of c_wgl_create_widget when the Hyper Text widget is created.
 *  @code
 *  typedef struct _WGL_HTS_INIT_T
 *  {
 *      UINT32                                  ui4_style;
 *      WGL_HTS_FONT_REG_INFO_T                 *at_font_reg_table;
 *      UINT16                                  ui2_num_reg_font;
 *      WGL_FONT_INFO_T                         t_font_info_dft;
 *  } WGL_HTS_INIT_T;  
 *  @endcode
 *  @li@c  ui4_style                                         -Contains the styles of the Hyper Text widget. 
 *  @li@c  *at_font_reg_table                                -Contains the font auxiliary information table to be registered. 
 *  @li@c  ui2_num_reg_font                                  -Contains the number of entries contained in the font auxiliary information table.
 *  @li@c  t_font_info_dft                                   -Contains the default font information. The Hyper Text widget takes the font information for creating the first default paragraph and section during its initialization.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_HTS_INIT_T
{
    UINT32                                  ui4_style;
    WGL_HTS_FONT_REG_INFO_T                 *at_font_reg_table;
    UINT16                                  ui2_num_reg_font;
    WGL_FONT_INFO_T                         t_font_info_dft;
} WGL_HTS_INIT_T;  

/*------------------------------------------------------------------*/
/*! @struct  WGL_HTS_PARAG_ATTR_T
 *  @brief  This structure contains all the attributes of a paragraph, including the justification type, the indent, the gaps between paragraphs, and the gaps between lines in the paragraph.
 *  @code
 *  typedef struct _WGL_HTS_PARAG_ATTR_T
 *  {
 *      WGL_HTS_JUSTIF_TYPE_T                   e_justif;
 *      UINT16                                  ui2_indent;
 *      UINT16                                  ui2_parag_gap_top;
 *      UINT16                                  ui2_parag_gap_bottom;
 *      INT32                                   i4_line_gap_top;
 *      INT32                                   i4_line_gap_bottom;
 *  } WGL_HTS_PARAG_ATTR_T;
 *  @endcode
 *  @li@c  e_justif                                          -Indicates the justification of the paragraph.
 *  @li@c  ui2_indent                                        -Indicates the indent of the paragraph.    
 *  @li@c  ui2_parag_gap_top                                 -Indicates the gap between the top of the paragraph and the bottom of its previous paragraph.
 *  @li@c  ui2_parag_gap_bottom                              -Indicates the gap between the bottom of the paragraph and the top of its next paragraph.
 *  @li@c  i4_line_gap_top                                   -Indicates the top gap of each line in the paragraph.
 *  @li@c  i4_line_gap_bottom                                -Indicates the bottom gap of each line in the paragraph.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_HTS_PARAG_ATTR_T
{
    WGL_HTS_JUSTIF_TYPE_T                   e_justif;
    UINT16                                  ui2_indent;

    UINT16                                  ui2_parag_gap_top;
    UINT16                                  ui2_parag_gap_bottom;
    INT32                                   i4_line_gap_top;
    INT32                                   i4_line_gap_bottom;

} WGL_HTS_PARAG_ATTR_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_HTS_STRING_T
 *  @brief  This structure holds a string. This structure is used for setting or getting a string from the Hyper Text widget. Note that the string contained in this structure (specified by the field w2s_str) is not necessary a NULL-terminated string. Any characters (including the NULL character) are acceptable and counted in the length of the string (specified in the field ui2_len_str).
 *  @code
 *  typedef struct _WGL_HTS_STRING_T
 *  {
 *      UINT16                                  ui2_size_buf;
 *      UTF16_T                                 *w2s_str;
 *      UINT16                                  ui2_len_str;
 *  } WGL_HTS_STRING_T;    
 *  @endcode
 *  @li@c  ui2_size_buf                                      -This structure holds a string. This structure is used for setting or getting a string from the Hyper Text widget. Note that the string contained in this structure (specified by the field w2s_str) is not necessary a NULL-terminated string. Any characters (including the NULL character) are acceptable and counted in the length of the string (specified in the field ui2_len_str).
 *  @li@c  *w2s_str                                          -Pointer to an UTF16_T buffer. The size of the buffer is indicated in the previous field ui2_size_buf and counted in UTF16_T character unit. The length of the string contained in the buffer is specified in the next field ui2_len_str. Note that the UTF16_T string contained in the buffer is not necessary NULL-terminated. Any UTF16_T characters (including the NULL character) are acceptable and counted in the length of the string
 *  @li@c  ui2_len_str                                       -Indicates the length of the string. Any UTF16_T characters, including the NULL characters, are counted in the length of the string. Note that this field is important since the string specified in w2s_str field may not be NULL-terminated.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_HTS_STRING_T
{
    UINT16                                  ui2_size_buf;
    UTF16_T                                 *w2s_str;
    UINT16                                  ui2_len_str;
} WGL_HTS_STRING_T;    

/*------------------------------------------------------------------*/
/*! @struct  WGL_HTS_TEXT_INFO_T
 *  @brief  This structure holds all the information for a text section, including the string, the vertical anchor, the font and the text foreground and background color.
 *  @code
 *  typedef struct _WGL_HTS_TEXT_INFO_T
 *  {
 *      WGL_HTS_STRING_T                        t_string;
 *      WGL_HTS_V_ANCHOR_T                      e_v_anchor;
 *      WGL_FONT_INFO_T                         t_font;
 *      GL_COLOR_T                              t_clr_text;
 *      GL_COLOR_T                              t_clr_text_bk;
 *  } WGL_HTS_TEXT_INFO_T;    
 *  @endcode
 *  @li@c  t_string                                          -Contains the text of the text section.    
 *  @li@c  e_v_anchor                                        -Contains the anchor of the text section for vertical positioning. 
 *  @li@c  t_font                                            -Contains the font information of the text section. 
 *  @li@c  t_clr_text                                        -Contains the text color of the text section.  
 *  @li@c  t_clr_text_bk                                     -Contains the text background color of the text section.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_HTS_TEXT_INFO_T
{
    WGL_HTS_STRING_T                        t_string;
    WGL_HTS_V_ANCHOR_T                      e_v_anchor;
    WGL_FONT_INFO_T                         t_font;
    GL_COLOR_T                              t_clr_text;
    GL_COLOR_T                              t_clr_text_bk;
} WGL_HTS_TEXT_INFO_T;    

/*------------------------------------------------------------------*/
/*! @struct  WGL_HTS_IMG_INFO_T
 *  @brief  This structure holds the information for a image section, including the handle of the image, the vertical anchor and the gaps around the image.
 *  @code
 *  typedef struct _WGL_HTS_IMG_INFO_T
 *  {
 *      WGL_HIMG_TPL_T                          h_img;
 *      WGL_HTS_V_ANCHOR_T                      e_v_anchor;
 *      UINT16                                  ui2_gap_left;
 *      UINT16                                  ui2_gap_top;
 *      UINT16                                  ui2_gap_right;    
 *      UINT16                                  ui2_gap_bottom;
 *  } WGL_HTS_IMG_INFO_T;    
 *  @endcode
 *  @li@c  h_img                                             -The template image handle of the image section. 
 *  @li@c  e_v_anchor                                        -The anchor of the image section for vertical positioning. 
 *  @li@c  ui2_gap_left                                      -The left gap of the image section. This gap specifies the distance between the previous section and the left boundary of the image
 *  @li@c  ui2_gap_top                                       -The top gap of the image section. This gap specifies the distance between the bottom boundary of previous line and the top boundary of the image.
 *  @li@c  ui2_gap_right                                     -The right gap of the image section. This gap specifies the distance between the right boundary of the image and the left boundary of the next section.
 *  @li@c  ui2_gap_bottom                                    -The bottom gap of the image section. This gap specifies the distance between the bottom boundary of the image and the top boundary of the next line.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_HTS_IMG_INFO_T
{
    WGL_HIMG_TPL_T                          h_img;
    WGL_HTS_V_ANCHOR_T                      e_v_anchor;
    UINT16                                  ui2_gap_left;
    UINT16                                  ui2_gap_top;
    UINT16                                  ui2_gap_right;    
    UINT16                                  ui2_gap_bottom;
} WGL_HTS_IMG_INFO_T;    

/*------------------------------------------------------------------*/
/*! @struct WGL_HTS_KEY_MAP_T
 *  @brief This structure holds the key map of Hyper Text widget. 
 *  @code
 *  typedef struct _WGL_HTS_KEY_MAP_T
 *  {
 *      UINT32                                  ui4_scrl_up;
 *      UINT32                                  ui4_scrl_down;
 *      UINT32                                  ui4_page_up;
 *      UINT32                                  ui4_page_down;
 *  } WGL_HTS_KEY_MAP_T;
 *  @endcode
 *  @li@c  ui4_scrl_up                                       -Indicates the key for scrolling up the viewport.
 *  @li@c  ui4_scrl_down                                     -Indicates the key for scrolling down the viewport.
 *  @li@c  ui4_page_up                                       -Indicates the key for page up. 
 *  @li@c  ui4_page_down                                     -Indicates the key for page down.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_HTS_KEY_MAP_T
{
    UINT32                                  ui4_scrl_up;
    UINT32                                  ui4_scrl_down;
    UINT32                                  ui4_page_up;
    UINT32                                  ui4_page_down;
} WGL_HTS_KEY_MAP_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_HTS_LINE_RANGE_T
 *  @brief This structure holds the line range that is used as the output parameter of WGL_CMD_HTS_SEC_GET_LOCATION command. 
 *  @code
 *  typedef struct _WGL_HTS_LINE_RANGE_T
 *  {
 *      UINT16                                  ui2_line_start;
 *      UINT16                                  ui2_line_end;
 *  } WGL_HTS_LINE_RANGE_T;    
 *  @endcode
 *  @li@c  ui2_line_start                                    -Indicates the line index of the start of the line range. 
 *  @li@c  ui2_line_end                                      -Indicates the line index of the end of the line range. 
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_HTS_LINE_RANGE_T
{
    UINT16                                  ui2_line_start;
    UINT16                                  ui2_line_end;
} WGL_HTS_LINE_RANGE_T;    


#ifdef __cplusplus
}
#endif


#endif

/*----------------------------------------------------------------------------*/
/*! @} */   /* end of groupMW_WGL_HTS*/
/*----------------------------------------------------------------------------*/

