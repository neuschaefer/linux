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
/*! @file u_wgl_lb_cnt_common.h 
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
/*! @addtogroup groupMW_WGL_LIST_BOX
 *  @{ 
 */
/*----------------------------------------------------------------------------*/


 /*Exported Commands for List Box Content*/
#ifndef _U_WGL_LB_CNT_COMMON_H
#define _U_WGL_LB_CNT_COMMON_H

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* common commands for all listbox content */

/**
This command sets the display margin for List Box Content. The i4_left is for the left margin, 
the i4_right is for the right margin, the i4_top is for the top margin, and the i4_bottom is for 
the bottom margin. All of them cannot be less than 0.

Input arguments

pv_param1	Pointer to a WGL_INSET_T value to specify the display margin. If it is NULL, it means 
to use the system default setting.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	One of argument pv_param1's members is invalid.
 */
#define WGL_CMD_LB_SET_CNT_INSET                WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 100)    

/**
This command returns the display margin for List Box Content. The i4_left is the left margin, the 
i4_right is the right margin, the i4_top is the top margin, and the i4_bottom is the bottom margin. 

Input arguments

pv_param2	Reserved. It must be NULL.

Output arguments

pv_param1	Pointer to a buffer of type WGL_INSET_T to receive the display margin. It cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
 */
#define WGL_CMD_LB_GET_CNT_INSET                WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 101) 

/**
This command sets the text of the title. 

Input arguments

pv_param1	Contains the text of the title to be set. This argument must be of type (UTF16_T *). If 
this argument is NULL, the title text will be cleared 

pv_param2	Contains the length of the title text to be set. If the length is zero, the title text 
will be cleared. Note that the text length is counted in UTF16_T characters count. This argument 
must be of type UINT16. 

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	When pv_param1 is NULL but the title text length indicated in pv_param2 is not zero. 
 */
#define WGL_CMD_LB_SET_TITLE_TEXT               WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 102) 

/**
This command returns the text of the title. Note that the caller must prepare a buffer of type (
UTF16_T *) to receive the UTF16_T string of the title text. The size of the buffer (in UTF16_T 
character count) shall also be indicated in pv_param2. 

Input & Output arguments

pv_param1	Pointer to a buffer of type (UTF16_T *) to receive the title text. It cannot be NULL.
Input arguments

pv_param2	Contains the size of the buffer (in UTF16_T character count).

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.

WGLR_BUFFER_NOT_ENOUGH
	The size of the text buffer (indicated in the argument pv_param2) prepared by the caller is too 
small to hold the text of title.
 */
#define WGL_CMD_LB_GET_TITLE_TEXT               WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 103) 

/**
This command sets the title information for List Box. 

Input arguments

pv_param1	Contains the title information to be set. This argument must be of type (
WGL_LB_TITLE_INFO_T *). If this argument is NULL, the default title information will be used. 

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The Alignment mode indicated in the field ui1_align of the structure WGL_LB_TITLE_INFO_T is 
invalid. 
 	The rectangle indicated in the field t_rect of the structure WGL_LB_TITLE_INFO_T is invalid.  
 */
#define WGL_CMD_LB_SET_TITLE_INFO               WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 104) 

/**
This command returns the title information for the List Box Content. Note that the caller must 
prepare a buffer of type WGL_LB_TITLE_INFO_T to receive the title information, which is indicated 
in the argument pv_param1. 

Input & Output arguments

pv_param1	Pointer to a buffer of type WGL_LB_TITLE_INFO_T to receive the title information. It 
cannot be NULL.This argument must be of type (WGL_LB_TITLE_INFO_T *).
Input arguments

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
 */
#define WGL_CMD_LB_GET_TITLE_INFO               WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 105) 

/** 
This command gets the size attributes shared with all elements. For vertical List Box, because 
the width for all elements is fixed, the element "size" only means the height of each element. 
For the same reason, in horizontal List Box, the term "size" means only the width of each element. 

Input arguments

pv_param2	Reserved. It must be NULL.

Output arguments

pv_param1	Returns the size of elements in the List Box. This argument must be of type (UINT16 *). 
It cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	pv_param1 is NULL
 */
#define WGL_CMD_LB_GET_ELEM_SIZE                WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 106) 

/** 
This command gets the distance between elements. 

Input arguments

pv_param2	Reserved. It must be NULL.

Output arguments

pv_param1	Returns the distance between elements in the List Box. This argument must be of type (
UINT16 *). It cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	pv_param1 is NULL
 */
#define WGL_CMD_LB_GET_ELEM_DISTANCE            WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 107) 

/**
This command is used to set the display margin for List Box elements. The i4_left is for the left 
margin, the i4_right is for the right margin, the i4_top is for the top margin, and the i4_bottom 
is for the bottom margin. All of them cannot be less than 0. 

Input arguments

pv_param1	Pointer to a WGL_INSET_T value to specify the display margin of List Box elements. If 
it is NULL, it means default margins are set on List Box elements.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	One of argument pv_param1's members is invalid.
 */
#define WGL_CMD_LB_SET_ELEM_INSET               WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 108) 

/** 
This command retrieves the display margin for List Box elements. The i4_left is the left margin, 
the i4_right is the right margin, the i4_top is the top margin, and the i4_bottom is the bottom 
margin. The element insets are zero by default.

Input arguments

pv_param1	Pointer to a WGL_INSET_T value to receive the display margin for List Box elements. It 
cannot be NULL.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
 */
#define WGL_CMD_LB_GET_ELEM_INSET               WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 109) 

/**
This command sets the color attributes shared with all elements and the target can be elements' 
background color, or other colors defined by List Box's content component.  

Input arguments

pv_param1	Specify the target. It can be WGL_CLR_BK or others defined by List Box's content 
component. This argument must be of type UINT8.

pv_param2	Pointer to a WGL_LB_COLOR_ELEM_SET_T. If it is NULL, it means to use the system default 
color set for the List Box. 

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The target index contained in pv_param1 is invalid.
*/
#define WGL_CMD_LB_SET_ELEM_COLOR               WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 110)  

/**
This command is used to get the color attributes shared with all elements in a List Box, and the 
source may be elements' background color or other colors defined by List Box's content component.  

Input arguments

pv_param1	Specify the source. It can be WGL_CLR_BK or others defined by List Box's content 
component. This argument must be of type UINT8.

Output arguments

pv_param2	Returns the color attributes. This argument must be of type (WGL_LB_COLOR_ELEM_SET_T *)
. It cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is invalid or pv_param2 is NULL.
  */
#define WGL_CMD_LB_GET_ELEM_COLOR               WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 111) 

/** 
This command gets the visible element number currently in the List Box. An element is visible if 
and only if it is completely visible by the user. 

Input arguments

pv_param2	Reserved. This argument must be NULL.

Output arguments

pv_param1	Returns the currently visible element number. This argument must be of type (UINT16 *). 
This argument cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
 */
#define WGL_CMD_LB_GET_VISIBLE_ELEM_NUM         WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 112)  

/**
This command gets the index of the first (top or left) completely visible element. If the List 
Box doesn't have any element, the returned index is WGL_LB_NULL_INDEX. An element is visible if 
and only if it is completely visible by the user. 

Input arguments

pv_param2	Reserved. This argument must be NULL.

Output arguments

pv_param1	Returns the index of the first visible element. If the List Box has no element, the 
returned index is WGL_LB_NULL_INDEX. This argument must be of type (UINT16 *). This argument 
cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
  */
#define WGL_CMD_LB_GET_FIRST_VISIBLE_INDEX      WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 113)  

/**
This command gets the index of the last (bottom or right) completely visible element. If the List 
Box doesn't have any element, the returned index is WGL_LB_NULL_INDEX. An element is visible if 
and only if it is completely visible by the user. 

Input arguments

pv_param2	Reserved. This argument must be NULL.
Output arguments

pv_param1	Returns the index of the last visible element. If the List Box has no element, the 
returned index is WGL_LB_NULL_INDEX. This argument must be of type (UINT16 *). This argument 
cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
 */
#define WGL_CMD_LB_GET_LAST_VISIBLE_INDEX       WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 114)  

/** 
This command sets the width of the specified column. Note that the width of a column can be zero.
Input arguments

pv_param1	Contains the index of the column to be set width. This argument must be of type UINT8.

pv_param2	Contains the width of the specified column in pixel. This argument must be of type 
UINT16.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 The index of the column is larger than (column_num-1).
 */
#define WGL_CMD_LB_SET_COL_WIDTH                WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 115) 

/**
This command queries the width of the specified column. 

Input arguments

pv_param1	Contains the index of the column to be queried. This argument must be of type UINT8.

Output arguments

pv_param2	Returns the width of the specified column in pixel. This argument must be of type (
UINT16 *). This argument cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The index of the column is larger than (column_num-1)
 	pv_param2 is NULL
 */
#define WGL_CMD_LB_GET_COL_WIDTH                WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 116) 

/**
This command sets the inset of the specified column. 

Input arguments

pv_param1	Contains the index of the column to be set inset. This argument must be of type UINT8.

pv_param2	Contains the inset. This argument must be of type (WGL_INSET_T *). If it is NULL, it 
means to use the default column inset.  

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The column-index is larger than (column_num-1)
 	The inset is invalid
  */
#define WGL_CMD_LB_SET_COL_INSET                WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 117)  

/** 
This command gets the inset of the specified column. 

Input arguments

pv_param1	Contains the index of the column to be got inset. This argument must be of type UINT8.
Output arguments

pv_param2	Returns the column inset. This argument must be of type (WGL_INSET_T  *). It cannot be 
NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The column index is larger than (column_num-1)
 	The argument pv_param2 is NULL.
 */
#define WGL_CMD_LB_GET_COL_INSET                WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 118) 

/**
This command sets the alignment mode for the text or image in a specified column. 

Input arguments

pv_param1	Contains the index of the target column. This argument must be of type UINT8.

pv_param2	Contains the alignment mode of the specified column. The possible values are 
WGL_AS_LEFT_TOP, WGL_AS_LEFT_CENTER, WGL_AS_LEFT_BOTTOM, WGL_AS_CENTER_TOP, WGL_AS_CENTER_CENTER, 
WGL_AS_CENTER_BOTTOM, WGL_AS_RIGHT_TOP, WGL_AS_RIGHT_CENTER, WGL_AS_RIGHT_BOTTOM. For more 
information about the alignment modes, refer to the document SW Design Middleware - WGL Common 
Widget.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The column index is larger than (column_num-1)
 	The argument pv_param2 is not a valid alignment mode number.
 */
#define WGL_CMD_LB_SET_COL_ALIGN                WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 119) 

/**
This command gets the alignment mode for the text or image in a specified column. 

Input arguments

pv_param1	Contains the index of the target column. The type of this argument must be UINT8.
Output arguments

pv_param2	Returns the alignment mode of the specified column. The possible values are 
WGL_AS_LEFT_TOP, WGL_AS_LEFT_CENTER, WGL_AS_LEFT_BOTTOM, WGL_AS_CENTER_TOP, WGL_AS_CENTER_CENTER, 
WGL_AS_CENTER_BOTTOM, WGL_AS_RIGHT_TOP, WGL_AS_RIGHT_CENTER, WGL_AS_RIGHT_BOTTOM. For more 
information about the alignment styles, refer to the document SW Design Middleware - WGL Common 
Widget.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The column index is larger than (column_num-1)
 	The argument pv_param2 is NULL.
 */
#define WGL_CMD_LB_GET_COL_ALIGN                WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 120) 

/**
This command sets the color attributes of the specified column and the target can be the column's 
background color, text color, or other colors defined by List Box's content component.  Note that 
the "text color" target is valid only for text columns. 

Input arguments

pv_param1	Contains both of the index of the column and the index of the target to be set color. 
The target index can be WGL_CLR_BK, WGL_CLR_TEXT, or others defined by List Box's content 
component. Note that the target index WGL_CLR_TEXT is valid only for text items. The target index 
must be of type UINT8. The column index and the target index must be packed by WGL_PACK_2(column-
index, target-index). 

pv_param2	Pointer to a WGL_LB_COLOR_ELEM_SET_T. If it is NULL, it means to reference the default 
color set attributes. The default color set attributes are derived by following rules:
 	If the target is WGL_CLR_BK, the default color set is derived from the background color set 
currently shared by all elements
 	If the target is WGL_CLR_TEXT, the default color set is the system default color set for List 
Box element text. 

Returns

WGLR_OK	The command is successful.

WGLR_INV_CMD	The text color is tried to set on a non-text column.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The column-index is larger than (column_num-1)
 	The target-index is invalid
 */
#define WGL_CMD_LB_SET_COL_COLOR                WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 121) 

/**
This command gets the color attributes of the specified column and the source can be the column's 
background color, text color, or other colors defined by List Box's content component.  Note that 
the "text color" source is valid only for text items. 

Input arguments

pv_param1	Contains both of the index of the column and the index of the source to be queried color
. The source index can be WGL_CLR_BK,  WGL_CLR_TEXT, or others defined by List Box's content 
component. Note that the source index WGL_CLR_TEXT is valid only for text items. The column index 
and the source index must be packed by WGL_PACK_2(column-index, source-index).

Output arguments

pv_param2	Returns the color attributes. This argument must be of type (WGL_LB_COLOR_ELEM_SET_T *)
. It cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_CMD	The text color is tried to retrieve on a non-text column.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The column index is larger than (column_num-1)
 	The source-index is invalid
 	The argument pv_param2 is NULL.
*/
#define WGL_CMD_LB_GET_COL_COLOR                WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 122) 

/**
This command sets the image background attributes of the specified column.

Input arguments

pv_param1	Point ot the index of the column . The index must be of type UINT8. 

pv_param2	Pointer to a WGL_LB_IMG_ELEM_SET_T. If it is NULL, it means no image background for the 
                   specialcolumn.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The column-index is larger than (column_num-1)
 	The image background data sets is invalid
 */
#define WGL_CMD_LB_SET_COL_BK_IMAGE             WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 123) 

/**
This command gets the image background attributes of the specified column.

Input arguments

pv_param1	Point ot the index of the column . The index must be of type UINT8. 

Output arguments

pv_param2	Pointer to a WGL_LB_IMG_ELEM_SET_T. If it is NULL, it means no image background for the 
                   specialcolumn.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The column-index is larger than (column_num-1)
 */
#define WGL_CMD_LB_GET_COL_BK_IMAGE             WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 124) 

/**
This command sets the font attributes of the specified text column. This command is valid only 
for text columns.

Input arguments

pv_param1	Contains the index of the column to be set font. This argument must be of type UINT8.

pv_param2	Contains the font handle. This argument must be of type (WGL_FONT_INFO_T *). If it is 
NULL, it means to use the font attribute of the List Box.  

Returns

WGLR_OK	The command is successful.

WGLR_INV_CMD	The specified column is not a text column

WGLR_INV_ARG	At least one of the following cases occurs:
 	The column-index is larger than (column_num-1)
 	The specified font information is invalid
  */
#define WGL_CMD_LB_SET_COL_FONT                 WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 125)  

/** 
This command gets the font attribute of the specified text column. This command is valid only for 
text-columns.

Input arguments

pv_param1	Contains the index of the text column to be queried font. This argument must be of type 
UINT8.

Output arguments

pv_param2	Returns the font information. This argument must be of type (WGL_FONT_INFO_T  *). It 
cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_CMD	The specified column is not a text column

WGLR_INV_ARG	At least one of the following cases occurs:
 	The column index is larger than (column_num-1)
 	The argument pv_param2 is NULL
*/
#define WGL_CMD_LB_GET_COL_FONT                 WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 126)  

/**
This command forces an item to use the default values, which are the format attributes of the 
corresponding column. This command has some advantages about the memory usage. Practically 
speaking, when this command operates upon an item that has local attributes, the memory space 
used to hold the local attributes of the item can be freed. 

Input arguments

pv_param1	Contains the element index of the item to be forced to use default. This argument must 
be of type UINT16.

pv_param2	Contains the column index of the specified item. This argument must be of type UINT8.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element-index is larger than (element_number-1)
 	The column-index is larger than (column_num-1)
*/
#define WGL_CMD_LB_SET_IS_ITEM_USE_DEF          WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 127)  

/** 
This command queries if an item uses the default values, which are the format attributes of the 
corresponding column, or not. For more information, refer to the description of 
WGL_CMD_LB_SET_IS_ITEM_USE_DEF command.

Input arguments

pv_param1	Contains both of the element index and the column index of the specified item. The 
element index and the column index must be packed by WGL_PACK_2(element-index, column-index). 

Output arguments

pv_param2	Returns if the specified item uses default or not. This argument must be of type (BOOL *
).

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element-index is larger than (element_number-1)
 	The column-index is larger than (column_num-1)
 	The argument pv_param2 is NULL
 */
#define WGL_CMD_LB_GET_IS_ITEM_USE_DEF          WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 128) 

/**
This command sets the alignment mode for the specified item. Note that whenever any local 
attributes of an item is set, an additional memory section will be dynamically allocated to hold 
local attributes of an item. This memory section is a structure that holds all local attributes 
of an item, such as text color, background color, font, and alignment mode.

Input arguments

pv_param1	Contains both of the element index and the column index of the specified item. The 
element index and the column index must be packed by WGL_PACK_2(element-index, column-index). 

pv_param2	Contains the alignment mode of the specified column. The possible values are 
WGL_AS_LEFT_TOP, WGL_AS_LEFT_CENTER, WGL_AS_LEFT_BOTTOM, WGL_AS_CENTER_TOP, WGL_AS_CENTER_CENTER, 
WGL_AS_CENTER_BOTTOM, WGL_AS_RIGHT_TOP, WGL_AS_RIGHT_CENTER, WGL_AS_RIGHT_BOTTOM. For more 
information about the alignment styles, refer to the document SW Design Middleware - WGL Common 
Widget.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element index is larger than (element_num-1)
 	The column index is larger than (column_num-1)
 	The argument pv_param2 is not a valid alignment mode number.
  */
#define WGL_CMD_LB_SET_ITEM_ALIGN               WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 129) 

/**
This command gets the alignment mode for the specified item. 

Input arguments

pv_param1	Contains both of the element index and the column index of the specified item. The 
element index and the column index must be packed by WGL_PACK_2(element-index, column-index). 

Output arguments

pv_param2	Returns the alignment mode of the specified column. The possible values are 
WGL_AS_LEFT_TOP, WGL_AS_LEFT_CENTER, WGL_AS_LEFT_BOTTOM, WGL_AS_CENTER_TOP, WGL_AS_CENTER_CENTER, 
WGL_AS_CENTER_BOTTOM, WGL_AS_RIGHT_TOP, WGL_AS_RIGHT_CENTER, WGL_AS_RIGHT_BOTTOM. For more 
information about the alignment styles, refer to the document SW Design Middleware - WGL Common 
Widget. This argument must be of type (UINT8 *). It cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element index is larger than (element_num-1)
 	The column index is larger than (column_num-1)
 	The argument pv_param2 is NULL.
 */
#define WGL_CMD_LB_GET_ITEM_ALIGN               WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 130) 

/**
This command sets the local value of the specified item's color attributes and the target can be 
the item's background color, text color, or other colors defined by List Box's content component
.  Note that the "text color" target is valid only for text items. Note that whenever any local 
attributes of an item is set, an additional memory section will be dynamically allocated to hold 
local attributes of an item. This memory section is a structure that holds all local attributes 
of an item, such as text color, background color, font, and alignment mode.

Input arguments

pv_param1	Contains three indexes: (1) the index of the specified item's element (2) the index of 
the specified item's column and (3) the index of the target to be set color. The target index can 
be WGL_CLR_BK, WGL_CLR_TEXT, or others defined by List Box's content component. Note that the 
target index WGL_CLR_TEXT is valid only for text items. The three indexes must be packed by 
WGL_PACK_3(element-index, column-index, target-index). 

pv_param2	Pointer to a WGL_LB_COLOR_ELEM_SET_T. If it is NULL, it means to reference the default 
color set attributes defined in the corresponding column.

Returns

WGLR_OK	The command is successful.

WGLR_INV_CMD	The text color is tried to set on a non-text item.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element-index is larger than (element_number-1)
 	The column-index is larger than (column_num-1)
 	The target-index is invalid
 */
#define WGL_CMD_LB_SET_ITEM_COLOR               WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 131) 

/**
This command gets the local values of the specified item's color attributes and the source can be 
the item's background color, text color, or other colors defined by List Box's content component
.  Note that the "text color" source is valid only for text items. 

Input arguments

pv_param1	Contains three indexes: (1) the index of the specified item's element (2) the index of 
the specified item's column and (3) the index of the source to be set color. The source index can 
be WGL_CLR_BK,  WGL_CLR_TEXT, or others defined by List Box's content component. Note that the 
source index WGL_CLR_TEXT is valid only for text items. The three indexes must be packed by 
WGL_PACK_3(element-index, column-index, source-index).

Output arguments

pv_param2	Returns the color attributes. This argument must be of type (WGL_LB_COLOR_ELEM_SET_T *)
. It cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_CMD	The text color is tried to retrieve on a non-text column.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element index is larger than (element_number-1)
 	The column index is larger than (column_num-1)
 	The source-index is invalid
 	The argument pv_param2 is NULL.
 */
#define WGL_CMD_LB_GET_ITEM_COLOR               WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 132)  

/**
This command sets the local value of the specified item's image background attributes and the target can be 
the item's background image.

Input arguments

pv_param1	Contains both indexes: (1) the index of the specified item's element (2) the index of 
the specified item's column.

pv_param2	Pointer to a WGL_LB_IMG_ELEM_SET_T. If it is NULL, it means no image background.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element-index is larger than (element_number-1)
 	The column-index is larger than (column_num-1)
 	The image background information is invalid
 */
#define WGL_CMD_LB_SET_ITEM_BK_IMAGE            WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 133) 

/**
This command gets the local value of the specified item's image background attributes and the target can be 
the item's background image.

Input arguments

pv_param1	Contains both indexes: (1) the index of the specified item's element (2) the index of 
the specified item's column.

Output arguments

pv_param2	Pointer to a WGL_LB_IMG_ELEM_SET_T. If it is NULL, it means no image background.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element-index is larger than (element_number-1)
 	The column-index is larger than (column_num-1)
 	The image background information is invalid
 */
#define WGL_CMD_LB_GET_ITEM_BK_IMAGE            WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 134)  

/**
This command sets the font attributes of the specified text item. This command is valid only for 
text items. Note that whenever any local attributes of an item is set, an additional memory 
section will be dynamically allocated to hold local attributes of an item. This memory section is 
a structure that holds all local attributes of an item, such as text color, background color, font
, and alignment mode.

Input arguments

pv_param1	Contains both of the element-index and the column-index of the specified item. The 
element-index and the column-index must be packed by WGL_PACK_2(element-index, column-index).

pv_param2	Contains the font handle. This argument must be of type (WGL_FONT_INFO_T *). If it is 
NULL, it means to use the font attribute defined in the format attributes of the corresponding 
column.  

Returns

WGLR_OK	The command is successful.

WGLR_INV_CMD	The specified item is not a text item

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element-index is larger than (element_number-1)
 	The column-index is larger than (column_num-1)
 	The specified font information is invalid
 */
#define WGL_CMD_LB_SET_ITEM_FONT                WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 135)  

/**
This command retrieves the font attribute of the specified text item. This command is valid only 
for text-items.

Input arguments

pv_param1	Contains both of the element-index and the column-index of the specified item. The 
element-index and the column-index must be packed by WGL_PACK_2(element-index, column-index).

Output arguments

pv_param2	Returns the font information of the specified item. This argument must be of type (
WGL_FONT_INFO_T  *). It cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_CMD	The specified item is not a text item

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element-index is larger than (element_number-1)
 	The column-index is larger than (column_num-1)
 	The argument pv_param2 is NULL.
  */
#define WGL_CMD_LB_GET_ITEM_FONT                WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 136)  

/**
This command sets the extend font attributes of the specified text column. This command is valid 
only for text columns. 

Input arguments

pv_param1	Contains the index of the column to be set font. This argument must be of type UINT8.

pv_param2	Contains the font handle. This argument must be of type (WGL_LB_FONT_INFO_EX_T *). If 
it is NULL, it means to use the extend font attribute of the List Box.  

Returns

WGLR_OK	The command is successful.

WGLR_INV_CMD	The specified column is not a text column

WGLR_INV_ARG	At least one of the following cases occurs:
 	The column-index is larger than (column_num-1)
 	The specified font information is invalid
 */
#define WGL_CMD_LB_SET_COL_FONT_EX              WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 137)  

/**
This command gets the extend font attribute of the specified text column. This command is valid 
only for text-columns.

Input arguments

pv_param1	Contains the index of the text column to be queried font. This argument must be of type 
UINT8.

Output arguments

pv_param2	Returns the font information. This argument must be of type (WGL_LB_FONT_INFO_EX_T  *). 
It cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_CMD	The specified column is not a text column

WGLR_INV_ARG	At least one of the following cases occurs:
 	The column index is larger than (column_num-1)
 	The argument pv_param2 is NULL.
  */
#define WGL_CMD_LB_GET_COL_FONT_EX              WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 138) 

/**
This command sets the extend font attributes of the specified text item. This command is valid 
only for text items. Note that whenever any local attributes of an item is set, an additional 
memory section will be dynamically allocated to hold local attributes of an item. This memory 
section is a structure that holds all local attributes of an item, such as text color, background 
color, font, and alignment mode. 

Input arguments

pv_param1	Contains both of the element-index and the column-index of the specified item. The 
element-index and the column-index must be packed by WGL_PACK_2(element-index, column-index).

pv_param2	Contains the font handle. This argument must be of type (WGL_LB_FONT_INFO_EX_T *). If 
it is NULL, it means to use the extend font attribute defined in the format attributes of the 
corresponding column.  

Returns

WGLR_OK	The command is successful.

WGLR_INV_CMD	The specified item is not a text item

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element-index is larger than (element_number-1)
 	The column-index is larger than (column_num-1)
 	The specified font information is invalid
 */
#define WGL_CMD_LB_SET_ITEM_FONT_EX             WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 139)  

/**
This command retrieves the extend font attribute of the specified text item. This command is 
valid only for text-items.

Input arguments

pv_param1	Contains both of the element-index and the column-index of the specified item. The 
element-index and the column-index must be packed by WGL_PACK_2(element-index, column-index).
Output arguments

pv_param2	Returns the font information of the specified item. This argument must be of type (
WGL_LB_FONT_INFO_EX_T  *). It cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_CMD	The specified item is not a text item

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element-index is larger than (element_number-1)
 	The column-index is larger than (column_num-1)
 	The argument pv_param2 is NULL.
  */
#define WGL_CMD_LB_GET_ITEM_FONT_EX             WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 140)  

/**
This command sets the local value of the specified item's align attributes to use default format.

Input arguments

pv_param1	Contains both indexes: (1) the index of the specified item's element (2) the index of 
the specified item's column.

pv_param2	Reserve.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element-index is larger than (element_number-1)
 	The column-index is larger than (column_num-1)
 */
#define WGL_CMD_LB_SET_ITEM_ALIGN_USE_DEF       WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 141) 

/**
This command gets the local value of the specified item's align attributes to use default format.

Input arguments

pv_param1	Contains both indexes: (1) the index of the specified item's element (2) the index of 
the specified item's column.

pv_param2	Point to the align value,using BOOL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element-index is larger than (element_number-1)
 	The column-index is larger than (column_num-1)
 */
#define WGL_CMD_LB_GET_ITEM_ALIGN_USE_DEF       WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 142)  

/**
This command sets the local value of the specified item's color attributes by using user default format and the 
target can be the item's background color, text color, or other colors defined by List Box's content component. 
Note that the "text color" target is valid only for text items. Note that whenever any local attributes of an item 
is set, an additional memory section will be dynamically allocated to hold local attributes of an item. This 
memory section is a structure that holds all local attributes of an item, such as text color, background color, 
font, and alignment mode.

Input arguments

pv_param1	Contains three indexes: (1) the index of the specified item's element (2) the index of 
the specified item's column and (3) the index of the target to be set color. The target index can 
be WGL_CLR_BK, WGL_CLR_TEXT, or others defined by List Box's content component. Note that the 
target index WGL_CLR_TEXT is valid only for text items. The three indexes must be packed by 
WGL_PACK_3(element-index, column-index, target-index). 

pv_param2	Reserve.

Returns

WGLR_OK	The command is successful.

WGLR_INV_CMD	The text color is tried to set on a non-text item.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element-index is larger than (element_number-1)
 	The column-index is larger than (column_num-1)
 	The target-index is invalid
 */
#define WGL_CMD_LB_SET_ITEM_COLOR_USE_DEF       WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 143) 

/**
This command gets the local value of the specified item's color attributes by using user default format and the 
target can be the item's background color, text color, or other colors defined by List Box's content component. 
Note that the "text color" target is valid only for text items. Note that whenever any local attributes of an item 
is set, an additional memory section will be dynamically allocated to hold local attributes of an item. This 
memory section is a structure that holds all local attributes of an item, such as text color, background color, 
font, and alignment mode.

Input arguments

pv_param1	Contains three indexes: (1) the index of the specified item's element (2) the index of 
the specified item's column and (3) the index of the target to be set color. The target index can 
be WGL_CLR_BK, WGL_CLR_TEXT, or others defined by List Box's content component. Note that the 
target index WGL_CLR_TEXT is valid only for text items. The three indexes must be packed by 
WGL_PACK_3(element-index, column-index, target-index). 

pv_param2	The value is user default formation or not.It uses BOOL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_CMD	The text color is tried to set on a non-text item.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element-index is larger than (element_number-1)
 	The column-index is larger than (column_num-1)
 	The target-index is invalid
 */
#define WGL_CMD_LB_GET_ITEM_COLOR_USE_DEF       WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 144)  

/**
This command sets the local value of the specified item's image background attributes.

Input arguments

pv_param1	Contains both indexes: (1) the index of the specified item's element (2) the index of 
the specified item's column.

pv_param2   Reserve.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element-index is larger than (element_number-1)
 	The column-index is larger than (column_num-1)
 */
 #define WGL_CMD_LB_SET_ITEM_BK_IMAGE_USE_DEF    WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 145)  

/**
This command gets the local value of the specified item's image background attributes.

Input arguments

pv_param1	Contains both indexes: (1) the index of the specified item's element (2) the index of 
the specified item's column.

Output arguments

pv_param2	Pointer to a BOOL that indicates using use default format or not.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element-index is larger than (element_number-1)
 	The column-index is larger than (column_num-1)
 */
#define WGL_CMD_LB_GET_ITEM_BK_IMAGE_USE_DEF    WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 146)  

/**
This command sets the local value of the specified item's font by using default attributes.

Input arguments

pv_param1	Contains both indexes: (1) the index of the specified item's element (2) the index of 
the specified item's column.

pv_param2   Reserve.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element-index is larger than (element_number-1)
 	The column-index is larger than (column_num-1)
 */
#define WGL_CMD_LB_SET_ITEM_FONT_USE_DEF        WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 147)  

/**
This command gets the local value of the specified item's font by using default format or not .

Input arguments

pv_param1	Contains both indexes: (1) the index of the specified item's element (2) the index of 
the specified item's column.

Output arguments

pv_param2	Pointer to a BOOL that indicates using use default format or not.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element-index is larger than (element_number-1)
 	The column-index is larger than (column_num-1)
 */
 #define WGL_CMD_LB_GET_ITEM_FONT_USE_DEF        WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 148)  

/**
This command sets the local value of the specified item's external font by using default .

Input arguments

pv_param1	Contains both indexes: (1) the index of the specified item's element (2) the index of 
the specified item's column.

pv_param2   Reserve.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element-index is larger than (element_number-1)
 	The column-index is larger than (column_num-1)
 */
#define WGL_CMD_LB_SET_ITEM_FONT_EX_USE_DEF     WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 149)  

/**
This command gets the local value of the specified item's external font by using default or not.

Input arguments

pv_param1	Contains both indexes: (1) the index of the specified item's element (2) the index of 
the specified item's column.

Output arguments

pv_param2	Pointer to a BOOL that indicates using use default format or not.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element-index is larger than (element_number-1)
 	The column-index is larger than (column_num-1)
 */
#define WGL_CMD_LB_GET_ITEM_FONT_EX_USE_DEF     WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 150)  

/**
This command sets the text's reading direction attributes of the List Box's column fixed to 
WGL_TEXT_DIR_LEFT_TO_RIGHT. The alignment of the column which is set to fixed text direction 
using this command will not works oppositely. The default text direction is WGL_TEXT_DIR_LEFT_TO_RIGHT.

Input arguments

pv_param1	Specifies the List Box's column index who will be set to fixed text read direction. 
This argument must be of type UINT8.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is invalid.
*/
#define WGL_CMD_LB_SET_COL_FIX_TEXT_DIR         WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 151)    

/**
This command sets the text's reading direction align attributes to fixed mode.Indicates that whenthe list box and one 
specific column can change text direction, user can restrict the column use fixed text alignmentwhen rendering 
using this command

Input arguments

pv_param1	Specifies the List Box's column index who will be set to fixed text read direction. 
This argument must be of type UINT8.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is invalid.
*/
#define WGL_CMD_LB_SET_COL_FIX_TEXT_DIR_ALIGN   WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 152)   

/** 
This command sets the element size attributes shared with all elements for the specail list box.

Input arguments

pv_param1   Contain the element size command information by using WGL_LB_ELEM_SIZE_T struct.It 
should not be NULL. 

Output arguments

pv_param2  Reserve.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	pv_param1 is NULL
 */
#define WGL_CMD_LB_SET_ELEM_SIZE                WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 153)

/** 
This command sets the element distance attributes shared with all elements for the specail list box.

Input arguments

pv_param1   Contain the element distance value by using UINT16.  

Output arguments

pv_param2  Reserve.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	pv_param1 is NULL
 */
#define WGL_CMD_LB_SET_ELEM_DISTANCE            WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 154)

/** 
This command sets the listbox content font attributes.

Input arguments

pv_param1   Contains the target,now it only support WGL_LB_FONT_ATTR_UNREC_CHAR attribute.  

Output arguments

pv_param2  When it is WGL_LB_FONT_ATTR_UNREC_CHAR target ,it indicates the char code that to fill 
while unrecognize.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	pv_param1 is NULL
 */
#define WGL_CNT_LB_SET_FONT_ATTR                WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 155)

/** 
This command sets the font attributes of the special column.

Input arguments

pv_param1   It using UINT32. bit 0~7: Contains the column index. bit 8~15:Contains the target,
now it only support WGL_LB_FONT_ATTR_UNREC_CHAR attribute.  

Output arguments

pv_param2  When it is WGL_LB_FONT_ATTR_UNREC_CHAR target ,it indicates the char code that to fill 
while unrecognize.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	pv_param1 is NULL
 */
#define WGL_CNT_LB_SET_COL_FONT_ATTR            WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 156)


/**This command sets conetent style for all elements. */

/** 
This command sets content style for all elements.

Input arguments

pv_param1   Contain the content style value by using UINT32. Content style can be 
WGL_LB_CNT_STL_UNFOCUS_AUTO_HIDE_SCRL_IMG, WGL_LB_CNT_STL_UNFOCUS_AUTO_HIDE_PAGI_IMG.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	pv_param1 is not valid.
 */
#define WGL_CMD_LB_EXT_SET_CNT_STYLE            WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 200)    

/** 
This command gets content style of all elements.

Input arguments

pv_param1   Contain the content style value by using UINT32. Content style can be 
WGL_LB_CNT_STL_UNFOCUS_AUTO_HIDE_SCRL_IMG, WGL_LB_CNT_STL_UNFOCUS_AUTO_HIDE_PAGI_IMG.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	pv_param1 is not valid.
 */

#define WGL_CMD_LB_EXT_GET_CNT_STYLE            WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 201)   

/**
This command sets the template image set for rendering the border and background of all elements. 
Each template image in the template image set corresponds to a state of an element. The template 
image set is contained in the data structure WGL_LB_IMG_ELEM_SET_T.

Input arguments

pv_param1	Contains the template image set for rendering the border and background of all elements
. This argument must be of type (WGL_LB_IMG_ELEM_SET_T *). A NULL value for this argument means 
the 	default List Box template image set for element will be used.

pv_param2	Reserved. It must be NULL.  

Returns

WGLR_OK	The command is successful.
  */
#define WGL_CMD_LB_EXT_SET_ELEM_BK_IMAGE        WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 202) 

/**
This command queries the template image set for rendering the border and background of all 
elements. The template image set is contained in the data structure WGL_LB_IMG_ELEM_SET_T.

Input arguments

pv_param2	Reserved. It must be NULL.  

Output arguments

pv_param1	Returns the template image set for rendering the border and background of all elements. 
This argument must be of type (WGL_LB_IMG_ELEM_SET_T *). This argument cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
  */
#define WGL_CMD_LB_EXT_GET_ELEM_BK_IMAGE        WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 203) 

/**
This command sets the scrolling-up and scrolling-down indication images indicating if there is at 
least one element prior to or posterior to the highlighted element or not. The scrolling 
indication image information is contained in the structure WGL_LB_SCROLL_IMG_INFO_T. For more 
information about the scrolling indication images, refer to the structure WGL_LB_SCROLL_IMG_INFO_T.

Input arguments

pv_param1	Contains the scrolling indication image information. This argument must be of type (
WGL_LB_SCROLL_IMG_INFO_T *). A NULL value for this argument means no scrolling indication images 
will be used.

pv_param2	Indicates whether to automatically repaint the newly set scrolling indication images or 
not. The argument must be of type BOOL. TRUE indicates automatically repainting, while FALSE 
indicates no automatic repaint.

Returns

WGLR_OK	The command is successful.
 */
#define WGL_CMD_LB_EXT_SET_SCROLL_IMAGE         WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 204)  

/**
This command retrieves the scrolling indication image information. The scrolling indication image 
information is contained in the structure WGL_LB_SCROLL_IMG_INFO_T. 

Input arguments

pv_param2	Reserved. It must be NULL. 

Output arguments

pv_param1	Returns the scrolling indication image information. This argument must be of type (
WGL_LB_SCROLL_IMG_INFO_T *). This argument cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
  */
#define WGL_CMD_LB_EXT_GET_SCROLL_IMAGE         WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 205) 

/**
This command sets the page-up and page-down indication images indicating if there is at least one 
page prior to or posterior to the current page in the viewport. The paging indication image 
information is contained in the structure WGL_LB_SCROLL_IMG_INFO_T. For more information about 
the paging indication images, refer to the structure WGL_LB_SCROLL_IMG_INFO_T.

Input arguments

pv_param1	Contains the paging indication image information. This argument must be of type (
WGL_LB_SCROLL_IMG_INFO_T *). A NULL value for this argument means no paging indication images 
will be used.

pv_param2	Indicates whether to automatically repaint the newly set paging indication images or not
. The argument must be of type BOOL. TRUE indicates automatically repainting, while FALSE 
indicates no automatic repaint.

Returns

WGLR_OK	The command is successful.
  */
#define WGL_CMD_LB_EXT_SET_PAGING_IMAGE         WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 206) 

/**
This command retrieves the paging indication image information. The paging indication image 
information is contained in the structure WGL_LB_SCROLL_IMG_INFO_T. 

Input arguments

pv_param2	Reserved. It must be NULL. 

Output arguments

pv_param1	Returns the paging indication image information. This argument must be of type (
WGL_LB_SCROLL_IMG_INFO_T *). This argument cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
 */
#define WGL_CMD_LB_EXT_GET_PAGING_IMAGE         WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 207)  

/**
This command sets highlight element effect information that is contained in the structure 
WGL_LB_HLT_ELEM_EFFECT_T.

Input arguments

pv_param1   Contains effect element information in the structure WGL_LB_HLT_ELEM_EFFECT_T.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
 */
#define WGL_CMD_LB_EXT_SET_HLT_ELEM_EFFECT      WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 208)  

/**
This command gets highlight element effect information that is contained in the structure 
WGL_LB_HLT_ELEM_EFFECT_T.

Output arguments

pv_param1   Contains effect element information in the structure WGL_LB_HLT_ELEM_EFFECT_T.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
 */#define WGL_CMD_LB_EXT_GET_HLT_ELEM_EFFECT      WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 209)  

/**
This command sets scroll bar information.

Input arguments

pv_param1   Contains scroll bar information in the structure WGL_LB_SCRL_BAR_T.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
 */
#define WGL_CMD_LB_EXT_SET_SCRL_BAR             WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 210) 

/**
This command gets scroll bar information.

Input arguments

pv_param1   Contains scroll bar information in the structure WGL_LB_SCRL_BAR_T.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
 */
#define WGL_CMD_LB_EXT_GET_SCRL_BAR             WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 211) 

/**
This command change scroll bar status.

Input arguments

pv_param1   Contains scroll bar status information in the structure WGL_LB_SCRL_BAR_STATUS_T.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
 */
#define WGL_CMD_LB_EXT_CHG_SCRL_BAR_STATUS      WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 212)  

#ifdef WGL_ANIM_SUPPORT
/**
This command change the listbox animation effect information for animation to use.

Input arguments

pv_param1   Contains animation effect information in the structure WGL_LB_ANIM_ELEM_EFFECT_T.

Returns

WGLR_OK     The command is successful.

WGLR_INV_ARG    The argument pv_param1 is NULL.
 */
#define WGL_CMD_LB_SET_ANIM_ELEM                WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 213)  

/**
This command get the listbox animation effect information.

Input arguments

pv_param1   Contains animation effect information in the structure WGL_LB_ANIM_ELEM_EFFECT_T.

Returns

WGLR_OK     The command is successful.

WGLR_INV_ARG    The argument pv_param1 is NULL.
 */
#define WGL_CMD_LB_GET_ANIM_ELEM                WGL_MAKE_CONTENT_CMD(WGL_CONTENT_LIST_BOX_DEF, 214)  
#endif



/* specific color command target */
#define WGL_CLR_LB_VP_BK               (WGL_CLR_WGT_SPF_START+1)    /**<Indicate the view point background target. */
#define WGL_CLR_LB_TITLE_FG            (WGL_CLR_WGT_SPF_START+2)    /**<Indicate the title foreground target*/
#define WGL_CLR_LB_TITLE_BK            (WGL_CLR_WGT_SPF_START+3)    /**<Indicate the title background target.*/

#define WGL_IMG_LB_VP_BK               (WGL_IMG_WGT_SPF_START+1)    /**<specific image command target*/

/* default content styles  */
/* used by WGL_CMD_LB_EXT_SET_CNT_STYLE command */
#define WGL_LB_CNT_STL_UNFOCUS_AUTO_HIDE_SCRL_IMG       MAKE_BIT_MASK_32(0)    /**<Auto hidle scrolling image while unfocus*/
#define WGL_LB_CNT_STL_UNFOCUS_AUTO_HIDE_PAGI_IMG       MAKE_BIT_MASK_32(1)    /**<Auto hidle page image while unfocus*/

#define WGL_LB_CNT_STL_NOT_SHOW_LAST_VP_ELEM_BK         MAKE_BIT_MASK_32(2)    /**<Not show the background of the element that is in the last one of current viewport*/

#define WGL_LB_CNT_STL_NOT_SHOW_LAST_ELEM_BK            MAKE_BIT_MASK_32(3)    /**<Not show the background of the last element*/

#define WGL_LB_CNT_STL_SHOW_HLT_ELEM_BK                 MAKE_BIT_MASK_32(4)    /**<Show the element background when it is highlt element*/




/* text scrolling styles */
/**This is a text scrolling styles that indicates text scrolling from the first char.  It is 
specified in ui4_style of the struct WGL_LB_TEXT_SCRL_T*/
#define WGL_LB_TEXT_SCRL_STL_FROM_FIRST_CHAR    MAKE_BIT_MASK_32(0)    

/**This is a text scrolling styles that indicates text scrolling till to the last char.  It is 
specified in ui4_style of the struct WGL_LB_TEXT_SCRL_T*/
#define WGL_LB_TEXT_SCRL_STL_TILL_LAST_CHAR     MAKE_BIT_MASK_32(1)

/**This is a text scrolling styles that indicates text scrolling in the repeat mode.  It is 
specified in ui4_style of the struct WGL_LB_TEXT_SCRL_T*/
#define WGL_LB_TEXT_SCRL_STL_REPEAT             MAKE_BIT_MASK_32(2)

/**This is a text scrolling styles that indicates text scrolling mode is forced to normal unfocus 
mode.  It is specified in ui4_style of the struct WGL_LB_TEXT_SCRL_T*/
#define WGL_LB_TEXT_SCRL_STL_FORCE_SCRL_UNFOCUS MAKE_BIT_MASK_32(3)

/**This is a text scrolling styles that indicates text scrolling is to delay start,the delay value 
is special in the ui4_ms_start_delay of WGL_LB_TEXT_SCRL_T.  The style is specified in 
ui4_style of the struct WGL_LB_TEXT_SCRL_T*/
#define WGL_LB_TEXT_SCRL_STL_DELAY_START        MAKE_BIT_MASK_32(4)

/**This is a text scrolling styles that indicates text scrolling retain current information when it changes
to unfocus state.  It is specified in ui4_style of the struct WGL_LB_TEXT_SCRL_T*/
#define WGL_LB_TEXT_SCRL_STL_RETAIN_INF_UNFOCUS  MAKE_BIT_MASK_32(5)

/** This is indicating the scrolling text min delay value.The delay value( ui4_ms_delay of 
WGL_LB_TEXT_SCRL_T) must larger than it when*/
#define WGL_LB_TEXT_SCRL_MIN_DELAY              10    



/**This is a styles for highlighted element effect.It indicates that enlarging the highlighted element size when the 
ui4_style in the WGL_LB_HLT_ELEM_EFFECT_T is set to WGL_LB_HEF_STL_ENLARGE_SIZE. */
#define WGL_LB_HEF_STL_ENLARGE_SIZE             MAKE_BIT_MASK_32(0) 

/**This is a styles for highlighted element effect.It indicates that keep enlarg the element size 
in the unfocus state when the ui4_style in the WGL_LB_HLT_ELEM_EFFECT_T is set to WGL_LB_HEF_STL_UNFOCUS_KEEP_ENLARGE. */
#define WGL_LB_HEF_STL_UNFOCUS_KEEP_ENLARGE     MAKE_BIT_MASK_32(1) 

/**This is a styles for highlighted element effect.It indicates that set the highlighted element effect to visible 
including the extra-margin area when ui4_style in the WGL_LB_HLT_ELEM_EFFECT_T is set 
to WGL_LB_HEF_STL_PUT_ABOVE_VP.*/
#define WGL_LB_HEF_STL_PUT_ABOVE_VP             MAKE_BIT_MASK_32(2) 

/**This is a styles for highlighted element effect.It indicates to enhance the highlight element 
background. It will using the WGL_LB_IMG_HLT_ELEM_SET_T image information for the background.
when ui4_style in the WGL_LB_HLT_ELEM_EFFECT_T is set to WGL_LB_HEF_STL_ENHANCE_ELEM_BK.*/
#define WGL_LB_HEF_STL_ENHANCE_ELEM_BK          MAKE_BIT_MASK_32(3) 

/**This is a styles for highlighted element effect.It indicates that the highlighted element enable 
scroll for the long text when ui4_style in the WGL_LB_HLT_ELEM_EFFECT_T is set 
to WGL_LB_HEF_STL_SCRL_LONG_TEXT.*/
#define WGL_LB_HEF_STL_SCRL_LONG_TEXT           MAKE_BIT_MASK_32(4) 



/**This is a styles for scroll bar. It indicates that the scroll bar size is fixed when ui4_style in the 
WGL_LB_SCRL_BAR_T is set to WGL_LB_SB_STL_FIXED_THUMB_SIZE.*/
#define WGL_LB_SB_STL_FIXED_THUMB_SIZE          MAKE_BIT_MASK_32(0)    

/**This is a styles for scroll bar. It indicates that the scroll bar is no scroll and keep drawing 
the body information when ui4_style in the WGL_LB_SCRL_BAR_T is set to 
WGL_LB_SB_STL_NO_SCRL_KEEP_BODY.*/
#define WGL_LB_SB_STL_NO_SCRL_KEEP_BODY         MAKE_BIT_MASK_32(1)    

/**This is a styles for scroll bar. It indicates that the scroll bar size is fixed when ui4_style in the 
WGL_LB_SCRL_BAR_T is set to WGL_LB_SB_STL_MANUAL_MODE.*/
#define WGL_LB_SB_STL_MANUAL_MODE               MAKE_BIT_MASK_32(2)    



#ifdef WGL_ANIM_SUPPORT
/*For listbox element animation style,default is only display margin information*/
#define WGL_LB_EA_STL_CNT_BY_ELEM               MAKE_BIT_MASK_32(0)    

#define WGL_LB_EA_STL_ONLY_ANIM_DATA            MAKE_BIT_MASK_32(1)    

#define WGL_LB_EA_STL_WITH_ANIM_ELEM            MAKE_BIT_MASK_32(2)    

#endif

/**It specifiy the a non-transparent background.see the example 1 below.

  Background Transparency Indicator :

       (1) WGL_LB_LAYER_BK_SOLID

       (2) WGL_LB_LAYER_BK_TRANSPARENT

  Must be specified in "alpha" component of GL_COLOR_T for 

       (1) viewport background, 

       (2) element background, 

       (3) item background

       (4) title background
 
  [Note] No matter the color mode of GL_COLOR_T is direct color mode (e.g. GL_COLORMODE_ARGB_D4444,
  GL_COLORMODE_ARGB_D1555, GL_COLORMODE_RGB_D565)  Or palette color mode,   
  (e.g. GL_COLORMODE_ARGB_CLUT8, GL_COLORMODE_AYUV_CLUT8). Whevever the value of ".a" in GL_COLOR_T 
  is NOT 255 (in the range 0~254), it is ALWAYS interpreted as transparent and the value in "u1", "u2", and "u3" 
  are ignored!
 
  Example 1: to specify a non-transparent background in GL_COLORMODE_ARGB_CLUT8 mode
  
     {

        GL_COLOR_T               t_palette_color;

        t_palette_color.a        = WGL_LB_LAYER_BK_SOLID;

        t_palette_color.u3.index = <some_color_index>;

     }
 
  Example 2: following code causes background completely transparent, and 
             the color index in "t_palette_color.u3.index" will be ignored!!

     {

        GL_COLOR_T               t_palette_color;

        t_palette_color.a        = 137;  <-- A value NOT equal to 255

        t_palette_color.u3.index = <some_color_index>;

     }
 
  Example 3: to specify a transparent background in GL_COLORMODE_RGB_565 mode

     {

        GL_COLOR_T       t_rgb_color;

        t_rgb_color.a    = WGL_LB_LAYER_BK_TRANSPARENT;

        t_rgb_color.u1.r = 255;

        t_rgb_color.u2.g = 255;

        t_rgb_color.u3.b = 255;

     } 
  [Note] The color would NOT be interpreted as WHITE color! It is completely transparent instead!
  
  Example 4: Following code causes the background completely transparent 
             even in GL_COLORMODE_ARGB_D4444 mode
     {

        GL_COLOR_T       t_argb_color;

        t_argb_color.a    = 170;  <-- A value NOT equal to 255

        t_argb_color.u1.r = 255;

        t_argb_color.u2.g = 255;

        t_argb_color.u3.b = 255;

     }
     [Note] Since "t_argb_color.a" is not 255, it would NOT be interpreted as translucent WHITE color!
            The R,G,B value are ignored and the BK will be completely transparent instead!
*/
#define WGL_LB_LAYER_BK_SOLID                      ((UINT8)255) 

/**
It specify a transparent background,only if 1, 2, ...254 also make a transparent background .It can see the
example 3 in the WGL_LB_LAYER_BK_SOLID specification.*/   
#define WGL_LB_LAYER_BK_TRANSPARENT                ((UINT8)0)    

/** It jude the listbox lay is transparent background color or not.*/
#define WGL_LB_IS_LAYER_BK_TRANSPARENT(_pt_color)       \
            ((_pt_color)->a != 255)    

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
/* scroll / page up-down indicator image info */
/*------------------------------------------------------------------*/
/*! @struct WGL_LB_SCROLL_IMG_INFO_T
 *  @brief This structure holds the information for scrolling or paging indication images of List Box.
 *  @code
 *  typedef struct _WGL_LB_SCROLL_IMG_INFO_T
 *  {
 *      GL_POINT_T          t_location_up;
 *      GL_POINT_T          t_location_down;
 *      WGL_HIMG_TPL_T      h_img_scr_up_on;
 *      WGL_HIMG_TPL_T      h_img_scr_up_off;
 *      WGL_HIMG_TPL_T      h_img_scr_down_on;
 *      WGL_HIMG_TPL_T      h_img_scr_down_off;
 *  } WGL_LB_SCROLL_IMG_INFO_T;
 *  @endcode
 *  @li@c  t_location_up                       -Contains the location of the scrolling-up indication image. The location is relative to the left-
upper corner of the List Box.
 *  @li@c  t_location_down                   -Contains the location of the scrolling-down indication image. The location is relative to the left
-upper corner of the List Box.
 *  @li@c  h_img_scr_up_on                 -Contains the handle of the template image which indicates there's at least one element prior to 
the currently highlighted element. An NULL_HANDLE value of this argument indicates no scrolling-up
-on indication image will be used.
 *  @li@c  h_img_scr_up_off                 -Contains the handle of the template image which indicates there's no element prior to the 
currently highlighted element when the List Box style flag WGL_STL_LB_NO_WRAP_OVER is turned on. An NULL_HANDLE value of this 
argument indicates no scrolling-up-off indication image will be used.
 *  @li@c  h_img_scr_down_on             -Contains the handle of the template image which indicates there's at least one element posterior 
to the currently highlighted element. An NULL_HANDLE value of this argument indicates no scrolling-down-on indication 
image will be used.
 *  @li@c  h_img_scr_down_off             -Contains the handle of the template image which indicates there's no element posterior to the 
currently highlighted element when the List Box style flag WGL_STL_LB_NO_WRAP_OVER is turned on. An NULL_HANDLE value of this 
argument indicates no scrolling-down-off indication image will be used.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_LB_SCROLL_IMG_INFO_T
{
    GL_POINT_T          t_location_up;
    GL_POINT_T          t_location_down;

    WGL_HIMG_TPL_T      h_img_scr_up_on;
    WGL_HIMG_TPL_T      h_img_scr_up_off;
    WGL_HIMG_TPL_T      h_img_scr_down_on;
    WGL_HIMG_TPL_T      h_img_scr_down_off;

} WGL_LB_SCROLL_IMG_INFO_T;

/* element color set */
/*------------------------------------------------------------------*/
/*! @struct WGL_LB_COLOR_ELEM_SET_T
 *  @brief This structure holds the corresponding color set when an element is in some state.
 *  @code
 *  typedef struct _WGL_LB_COLOR_ELEM_SET_T
 *  {
 *      GL_COLOR_T          t_normal;
 *      GL_COLOR_T          t_disable;
 *      GL_COLOR_T          t_highlight;
 *      GL_COLOR_T          t_highlight_unfocus;
 *      GL_COLOR_T          t_pushed;            
 *      GL_COLOR_T          t_selected;            
 *      GL_COLOR_T          t_selected_disable;            
 *  } WGL_LB_COLOR_ELEM_SET_T;    
 *  @endcode
 *  @li@c  t_normal                      -Contains the color value related to the normal state of an 
 element that is in enabled, un-highlighted, un-pushed and deselected state.
 *  @li@c  t_disable                      -Contains the color value related to the disable state of an element.
 *  @li@c  t_highlight                    -Contains the color value related to the highlighted state of an element.
 *  @li@c  t_highlight_unfocus       -Contains the color value related to the highlighted state of an element when the List Box is in 
unfocused state.
 *  @li@c  t_pushed                      -Contains the color value related to the pushed state of an element.
 *  @li@c  t_selected                     -Contains the color value related to the selected state of an element.
 *  @li@c  t_selected_disable         -Contains the color value related to the selected state of an element when at least one of the 
following cases occurs:1.The element is disabled;2. The List box is in unfocused state;3.This List Box is in disabled state.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_LB_COLOR_ELEM_SET_T
{
    GL_COLOR_T          t_normal;
    GL_COLOR_T          t_disable;
    GL_COLOR_T          t_highlight;
    GL_COLOR_T          t_highlight_unfocus;
    GL_COLOR_T          t_pushed;            
    GL_COLOR_T          t_selected;            
    GL_COLOR_T          t_selected_disable;            
} WGL_LB_COLOR_ELEM_SET_T;    

/* element image set */
/*------------------------------------------------------------------*/
/*! @struct WGL_LB_IMG_ELEM_SET_T
 *  @brief This structure holds a set of template images, each of which corresponds to certain state of an 
element. Each field of this structure is a template image handle, which is of type WGL_HIMG_TPL_T. 
 *  @code
 *  typedef struct _WGL_LB_IMG_ELEM_SET_T
 *  {
 *      WGL_HIMG_TPL_T      h_normal;
 *      WGL_HIMG_TPL_T      h_disable;
 *      WGL_HIMG_TPL_T      h_highlight;
 *      WGL_HIMG_TPL_T      h_highlight_unfocus;
 *      WGL_HIMG_TPL_T      h_pushed;           
 *      WGL_HIMG_TPL_T      h_selected;            
 *      WGL_HIMG_TPL_T      h_selected_disable;            
 *  } WGL_LB_IMG_ELEM_SET_T;    
 *  @endcode
 *  @li@c  h_normal                        -Contains the template image related to the normal state of an element. 
 An element is in normal state when it's in enabled, un-highlighted, un-pushed and deselected state.
 *  @li@c  h_disable                        -Contains the template image related to the disable state of an element.
 *  @li@c  h_highlight                     -Contains the template image related to the highlighted state of an element.
 *  @li@c  h_highlight_unfocus         -Contains the template image related to the highlighted state of an element when the List Box is 
in unfocused state.
 *  @li@c  h_pushed                       -Contains the template image related to the pushed state of an element.
 *  @li@c  h_selected                      -Contains the template image related to the selected state of an element.
 *  @li@c  h_selected_disable           -Contains the template image related to the selected state of an element when at least one of the 
following cases occurs:1.The element is disabled;2. The List box is in unfocused state;3.This List Box is in disabled state.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_LB_IMG_ELEM_SET_T
{
    WGL_HIMG_TPL_T      h_normal;
    WGL_HIMG_TPL_T      h_disable;
    WGL_HIMG_TPL_T      h_highlight;
    WGL_HIMG_TPL_T      h_highlight_unfocus;
    WGL_HIMG_TPL_T      h_pushed;           
    WGL_HIMG_TPL_T      h_selected;            
    WGL_HIMG_TPL_T      h_selected_disable;            
} WGL_LB_IMG_ELEM_SET_T;    

/*extend column/item font info*/
/*------------------------------------------------------------------*/
/*! @struct WGL_LB_FONT_INFO_EX_T
 *  @brief This structure holds the extend font information for column and item of List Box. 
 *  @code
 *  typedef struct _WGL_LB_COL_FONT_INFO_EX_T
 *  {
 *      WGL_FONT_INFO_T     t_normal;
 *      WGL_FONT_INFO_T     t_highlight;
 *      WGL_FONT_INFO_T     t_highlight_unfocused;
 *      WGL_FONT_INFO_T     t_selected;
 *  } WGL_LB_FONT_INFO_EX_T;
 *  @endcode
 *  @li@c  t_normal                          -Contains the font info in normal state. 
 *  @li@c  t_highlight                       -Contains the font info in highlight state.
 *  @li@c  t_highlight_unfocused       -Contains the font info in highlight unfocused state.
 *  @li@c  t_selected                        -Contains the font info in selected state.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_LB_COL_FONT_INFO_EX_T
{
    WGL_FONT_INFO_T	    t_normal;
    WGL_FONT_INFO_T	    t_highlight;
    WGL_FONT_INFO_T	    t_highlight_unfocused;
    WGL_FONT_INFO_T	    t_selected;
} WGL_LB_FONT_INFO_EX_T;

/* title info */
/*------------------------------------------------------------------*/
/*! @struct WGL_LB_TITLE_INFO_T
 *  @brief This structure holds the information for title of List Box. 
 *  @code
 *  typedef struct _WGL_LB_TITLE_INFO_T
 *  {
 *      GL_RECT_T           t_rect;
 *      UINT8               ui1_align;
 *  } WGL_LB_TITLE_INFO_T;
 *  @endcode
 *  @li@c  t_rect                -Contains the reference bounding rectangle of the title. The coordinates of this rectangle is 
relative to the left-upper corner of the List Box.
 *  @li@c  ui1_align           -Contains the alignment mode of the title text relative to the reference bounding rectangle 
indicated in the field t_rect. 
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_LB_TITLE_INFO_T
{
    GL_RECT_T           t_rect;
    UINT8               ui1_align;
} WGL_LB_TITLE_INFO_T;

/* extra area */
/*------------------------------------------------------------------*/
/*! @struct WGL_LB_EXTRA_MARGIN_T
 *  @brief This structure holds the extrant margin information of list box.
 *  @code
 *  typedef struct _WGL_LB_EXTRA_MARGIN_T
 *  {
 *      UINT16              ui2_left;
 *      UINT16              ui2_top;
 *      UINT16              ui2_right;
 *      UINT16              ui2_bottom;
 *  } WGL_LB_EXTRA_MARGIN_T; 
 *  @endcode
 *  @li@c  ui2_left                                          -
 *  @li@c  ui2_top                                           -
 *  @li@c  ui2_right                                         -
 *  @li@c  ui2_bottom                                        -
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_LB_EXTRA_MARGIN_T
{
    UINT16              ui2_left;
    UINT16              ui2_top;
    UINT16              ui2_right;
    UINT16              ui2_bottom;
    
} WGL_LB_EXTRA_MARGIN_T; 

/* background image set for highlight element */
/*------------------------------------------------------------------*/
/*! @struct WGL_LB_IMG_HLT_ELEM_SET_T
 *  @brief This struct holds highlight element image set information.
 *  @code
 *  typedef struct _WGL_LB_IMG_HLT_ELEM_SET_T
 *  {
 *      WGL_HIMG_TPL_T      h_highlight;
 *      WGL_HIMG_TPL_T      h_highlight_unfocus;
 *      WGL_HIMG_TPL_T      h_pushed;
 *      WGL_HIMG_TPL_T      h_selected_highlight;
 *      WGL_HIMG_TPL_T      h_selected_highlight_unfocus;
 *      WGL_HIMG_TPL_T      h_selected_pushed;
 *  } WGL_LB_IMG_HLT_ELEM_SET_T;
 *  @endcode
 *  @li@c  h_highlight                                      -The image is for element while in highlight state.
 *  @li@c  h_highlight_unfocus                          -The image is for element whiel in highlight but unfocus.
 *  @li@c  h_pushed                                         -The image is for element while in pushed state.
 *  @li@c  h_selected_highlight                          -The image is for element while in selected and highlight state.
 *  @li@c  h_selected_highlight_unfocus             -The image is for element while in selected and highlight unfocus state.
 *  @li@c  h_selected_pushed                            -The image is for element while in selected and pushed state.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_LB_IMG_HLT_ELEM_SET_T
{
    WGL_HIMG_TPL_T      h_highlight;
    WGL_HIMG_TPL_T      h_highlight_unfocus;
    WGL_HIMG_TPL_T      h_pushed;

    WGL_HIMG_TPL_T      h_selected_highlight;
    WGL_HIMG_TPL_T      h_selected_highlight_unfocus;
    WGL_HIMG_TPL_T      h_selected_pushed;
    
} WGL_LB_IMG_HLT_ELEM_SET_T;


/*------------------------------------------------------------------*/
/*! @struct WGL_LB_TEXT_SCRL_T
 *  @brief This struct holds text scroll information for list box.
 *  @code
 *  typedef struct _WGL_LB_TEXT_SCRL_T
 *  {
 *      UINT32              ui4_style;
 *      UINT32              ui4_scrl_count; 
 *      UINT32              ui4_ms_delay;
 *      UINT32              ui4_ms_start_delay;
 *      BOOL                b_sync;
 *  } WGL_LB_TEXT_SCRL_T;
 *  @endcode
 *  @li@c  ui4_style                        -The style of text scroll for list box.
 *  @li@c  ui4_scrl_count                -The count of text scroll.Specify 0 if "WGL_LB_TEXT_SCRL_STL_REPEAT" is on.
 *  @li@c  ui4_ms_delay                 -The delay time.
 *  @li@c  ui4_ms_start_delay         -The start delay time.
 *  @li@c  b_sync                          -True:sync scroll. False:Async scroll.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_LB_TEXT_SCRL_T
{
    UINT32              ui4_style;
    UINT32              ui4_scrl_count; 
    UINT32              ui4_ms_delay;
    UINT32              ui4_ms_start_delay;
    BOOL                b_sync;
    
} WGL_LB_TEXT_SCRL_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_LB_HLT_ELEM_EFFECT_T
 *  @brief This struct holds highlighted element effect information.
 *  @code
 *  typedef struct _WGL_LB_HLT_ELEM_EFFECT_T
 *  {
 *      UINT32                      ui4_style;
 *      WGL_LB_EXTRA_MARGIN_T       t_ex_margin;
 *      WGL_LB_IMG_HLT_ELEM_SET_T   t_imgs_bk;
 *      WGL_LB_TEXT_SCRL_T          t_text_scrl;
 *  } WGL_LB_HLT_ELEM_EFFECT_T;
 *  @endcode
 *  @li@c  ui4_style                   -The style of highlighted element.
 *  @li@c  t_ex_margin              -The extrant margin for the list box.
 *  @li@c  t_imgs_bk                 -Contains image background information.
 *  @li@c  t_text_scrl                 -Conatains text scroll information.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_LB_HLT_ELEM_EFFECT_T
{
    UINT32                      ui4_style;
    WGL_LB_EXTRA_MARGIN_T       t_ex_margin;
    WGL_LB_IMG_HLT_ELEM_SET_T   t_imgs_bk;
    WGL_LB_TEXT_SCRL_T          t_text_scrl;
    
} WGL_LB_HLT_ELEM_EFFECT_T;

/* scroll bar info */
/*------------------------------------------------------------------*/
/*! @struct WGL_LB_SCRL_BAR_T
 *  @brief This struct holds the scroll bar information of the special list box
 *  @code
 *  typedef struct _WGL_LB_SCRL_BAR_T
 *  {
 *      UINT32                  ui4_style;
 *      GL_RECT_T               t_rc_body;          
 *      WGL_INSET_T             t_inset_track;      
 *      UINT32                  ui4_sz_thumb_min;   
 *      WGL_COLOR_STD_SET_T     t_clrs_body;
 *      WGL_IMG_STD_SET_T       t_imgs_body;
 *      WGL_COLOR_STD_SET_T     t_clrs_thumb;
 *      WGL_IMG_STD_SET_T       t_imgs_thumb;
 *  } WGL_LB_SCRL_BAR_T;
 *  @endcode
 *  @li@c  ui4_style                                         -
 *  @li@c  t_rc_body                                         -
 *  @li@c  t_inset_track                                     -
 *  @li@c  ui4_sz_thumb_min                                  -
 *  @li@c  t_clrs_body                                       -
 *  @li@c  t_imgs_body                                       -
 *  @li@c  t_clrs_thumb                                      -
 *  @li@c  t_imgs_thumb                                      -
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_LB_SCRL_BAR_T
{
    UINT32                  ui4_style;
    
    GL_RECT_T               t_rc_body;          
    WGL_INSET_T             t_inset_track;      
    UINT32                  ui4_sz_thumb_min;   
    
    WGL_COLOR_STD_SET_T     t_clrs_body;
    WGL_IMG_STD_SET_T       t_imgs_body;
    WGL_COLOR_STD_SET_T     t_clrs_thumb;
    WGL_IMG_STD_SET_T       t_imgs_thumb;
    
} WGL_LB_SCRL_BAR_T;

/* scroll bar status */
/*------------------------------------------------------------------*/
/*! @struct WGL_LB_SCRL_BAR_STATUS_T
 *  @brief This struct holds the scroll bar status information.
 *  @code
 *  typedef struct _WGL_LB_SCRL_BAR_STATUS_T
 *  {
 *      UINT32      ui4_size_vp;
 *      UINT32      ui4_size_total;
 *      UINT32      ui4_pos_vp_top;
 *  } WGL_LB_SCRL_BAR_STATUS_T;
 *  @endcode
 *  @li@c  ui4_size_vp                     -The size of viewport.
 *  @li@c  ui4_size_total                  -The total size.
 *  @li@c  ui4_pos_vp_top               -The top position of viewport.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_LB_SCRL_BAR_STATUS_T
{
    UINT32      ui4_size_vp;
    UINT32      ui4_size_total;
    UINT32      ui4_pos_vp_top;
    
} WGL_LB_SCRL_BAR_STATUS_T;

/* Element size information */
/*------------------------------------------------------------------*/
/*! @struct WGL_LB_ELEM_SIZE_T
 *  @brief This struct contain the element size information for  WGL_CMD_LB_SET_ELEM_SIZE command.
 *  @code
 *  typedef struct _WGL_LB_ELEM_SIZE_T
 *  {
 *      BOOL        b_size_by_num;
 *      UINT16      ui2_elem_height;
 *      UINT16      ui2_elem_num;
 *  } WGL_LB_ELEM_SIZE_T;
 *  @endcode
 *  @li@c  b_size_by_num                 -TRUE:the element size value is by ui1_elem_num calculate.
 *                                                      FALSE:element size is set by ui1_elem_height.
 *  @li@c  ui2_elem_height               -The element height data.
 *  @li@c  ui2_elem_num                  -The element number display in the viewport.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_LB_ELEM_SIZE_T
{
    BOOL        b_size_by_num;
    UINT16      ui2_elem_height;
    UINT16      ui2_elem_num;
} WGL_LB_ELEM_SIZE_T;

#ifdef WGL_ANIM_SUPPORT
typedef struct _WGL_LB_IMG_ANIM_ELEM_SET_T
{
    WGL_HIMG_TPL_T      h_anim_hlt;
    
} WGL_LB_IMG_ANIM_ELEM_SET_T;


typedef struct _WGL_LB_ANIM_ELEM_EFFECT_T
{
    UINT32                      ui4_style;
    WGL_LB_EXTRA_MARGIN_T       t_ex_margin;
    WGL_LB_IMG_ANIM_ELEM_SET_T  t_imgs;
    UINT8                       ui1_alpha_s;
    UINT8                       ui1_alpha_e;
} WGL_LB_ANIM_ELEM_EFFECT_T;

#endif

#ifdef __cplusplus
}
#endif

#endif

/*----------------------------------------------------------------------------*/
/*! @} */   /* end of groupMW_WGL_LIST_BOX*/
/*----------------------------------------------------------------------------*/


