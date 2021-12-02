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
/*! @file u_wgl_lb.h 
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
/*! @defgroup groupMW_WGL_LIST_BOX   List Box Widget  
 *
 *  @ingroup groupMW_WGL
 *  @brief Provides definitions and exported function of the widget list box library.
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW_WGL
 *  @{ 
 */
/*----------------------------------------------------------------------------*/
 
#ifndef _U_WGL_LB_H_
#define _U_WGL_LB_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_handle.h"
#include "u_gl.h"
#include "u_wgl_common.h"
#include "u_wgl_lb_cnt_common.h"


#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#ifdef WGL_ANIM_SUPPORT
#define  WGL_LB_ANIM_ACT_START    WGL_ANIM_ACTION_INGORE
#define  WGL_LB_ANIM_ACT_END      WGL_ANIM_ACTION_END

#define  WGL_LB_ANIM_ACT_NUM      ((UINT8)(WGL_LB_ANIM_ACT_END - WGL_LB_ANIM_ACT_START + 1))
#endif
/* list box specific commands */
/**

This command gets the style of the List Box. The format of the returned style flags combinations 
is the same as the ui4_style field of the LB_INIT_T structure. For more information about the 
specific styles of List Box, refer to the section "Widget Style - List Box Specific". As for the 
general styles supported by List Box, refer to the section "General Widget Styles" in the 
document "SW Design Middleware - WGL Common Widget". 

Input arguments

pv_param2	Reserved. This argument must be NULL.

Output arguments

pv_param1	Returns the style of the List Box. This argument must be of type (UINT32 *). This 
argument cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
*/
#define WGL_CMD_LB_GET_STYLE                WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 0)    

/**
This command gets the maximum element number the List Box can contain. 

Input arguments

pv_param2	Reserved. This argument must be NULL.

Output arguments

pv_param1	Returns the maximum element number. This argument must be of type (UINT16 *). This 
argument cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
*/
#define WGL_CMD_LB_GET_MAX_ELEM_NUM         WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 1)    

/**
This command copies the "snapshot" states of all elements in the List Box into the state buffer 
prepared by the caller of this command. Note that the state-buffer can only be managed by this 
command and the owner of this state buffer shall not do any private change upon this buffer. In 
other words, the state buffer is read-only with regard to the owner of the buffer. The state of 
individual element in the state buffer can be retrieved through the following macros:
 	WGL_LB_IS_ELEM_DIS ()
 	WGL_LB_IS_ELEM_HLT ()
 	WGL_LB_IS_ELEM_SEL ()
For more information about the above macros.

Input arguments

pv_param2	Contains the size of the state buffer in byte-count. No matter in any circumstance, the 
size of the state buffer (in byte-count) must be equal to (WGL_LB_ELEM_STATE_BUF_SIZE * element-
number). This argument must be of type UINT16.

Output arguments

pv_param1	Contains a pointer to a state buffer to receive the snapshot states of all elements. 
Note that the byte-size of the state buffer must be at least (WGL_LB_ELEM_STATE_BUF_SIZE * element
-number) as specified in the argument pv_param2. This argument must be of type (VOID *) and 
cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The argument pv_param1 is NULL.
 	The size of the state buffer specified in pv_param2 is invalid.
*/
#define WGL_CMD_LB_GET_ALL_ELEM_STATES      WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 2)    

/**
This command restores the "snapshot" states of all elements in the List Box by coping states from 
the specified state buffer. The state buffer shall be prepared by the caller of this command and 
this buffer can only be managed by the command WGL_CMD_LB_GET_ALL_ELEM_STATES. For more 
information about the state buffer, refer to the command WGL_CMD_LB_GET_ALL_ELEM_STATES.

Input arguments

pv_param1	Contains the state buffer whose byte-size must be  at least (WGL_LB_ELEM_STATE_BUF_SIZE 
* element-number) as specified in the argument pv_param2. This argument must be of type (VOID *) 
and cannot be NULL.

pv_param2	Contains the size of the state buffer in byte-count. No matter in any circumstance, the 
size of the state buffer (in byte-count) must be equal to (WGL_LB_ELEM_STATE_BUF_SIZE * element-
number). This argument must be of type UINT16.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The argument pv_param1 is NULL.
 	The size of the state buffer specified in pv_param2 is invalid.
*/
#define WGL_CMD_LB_RESTORE_ALL_ELEM_STATES  WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 3)    


/**
This command repaints a specified range of elements.

Input arguments

pv_param1	Specifies the index of the first element in the range to be repainted. This argument 
must be of type UINT16. 

pv_param2	Specifies the index of the last element in the range to be repainted. This argument 
must be of type UINT16. Note that the index must be equal to or larger than the index specified 
in pv_param1. In addition, this index must be less than (element_num-1).

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs: 
 	The element range is invalid
 	The specified range of elements is out of the total number of elements in the List Box. 
*/
#define WGL_CMD_LB_REPAINT_ELEMS            WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 4)    

/**
This command repaints a specified item. If the specified item is invisible from the viewport, 
this command has no effects.

Input arguments

pv_param1	Contains both of the index of the element and the index of the item to be repainted. 
The element index and item index must be packed by WGL_PACK_2(element-index, item-index). This 
argument must be packed as WGL_PACK_2(UINT16, UINT8). 

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The index of the element is larger than (element_number-1)
 	The index of the item is larger than (column_num-1)
*/
#define WGL_CMD_LB_REPAINT_ITEM             WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 5)    

/**
This command is used to append an element at the tail of the List Box. The appended element can 
be empty by specifying pv_param1 as NULL, or can be initialized by an item data array with type 
WGL_LB_ITEM_T. If the style WGL_STL_LB_FIXED_ELEM_NUM is assigned upon this List Box, this 
command fails. Note that the element number cannot be larger than ui2_max_elem_num assigned in 
WGL_LB_INIT_T; otherwise, this command fails. 

If the List Box operates in forced-highlighting mode and no element is highlighted, the appended 
element will be automatically highlighted. In this case, an element-highlighting notification 
WGL_NC_LB_ELEM_HLT will be arisen.

If the List Box operates in forced-selecting mode and no element is selected, the appended 
element will be automatically selected. In this case, an element selection notification 
WGL_NC_LB_ELEM_SEL will be arisen.

Input arguments

pv_param1	Specifies the contents of all the items of the new appended element. This parameter 
shall contain an item array with type (WGL_LB_ITEM_T *) to specify all items' contents. If this 
argument is NULL, an empty element is appended.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_CMD	At least one of the following cases occurs: 
 	The style WGL_STL_LB_FIXED_ELEM_NUM is assigned upon this List Box.
 	The element number exceeds ui2_max_elem_num assigned in WGL_LB_INIT_T

WGLR_INV_ARG	The item-types of some of the specified items are not fitted with the item-types of 
the corresponding columns. For example, if a text item is passed into a column that should 
contain an image item, the command is failed and WGLR_INV_ARG is returned. 

WGLR_OUT_OF_MEMORY	There is not enough memory to complete the operation.
*/
#define WGL_CMD_LB_APPEND_ELEM              WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 6) 

/**
This command is used to insert an element before the specified element-index of the List Box. The 
element index before which the element is to be inserted is specified in pv_param1. The inserted 
element can be empty by specifying pv_param2 as NULL, or can be initialized by an item data array 
with type WGL_LB_ITEM_T. If the style WGL_STL_LB_FIXED_ELEM_NUM is assigned upon this List Box, 
this command fails. Note that the element number cannot be larger than ui2_max_elem_num assigned 
in WGL_LB_INIT_T; otherwise, this command fails.

If the List Box operates in forced-highlighting mode and no element is highlighted, the appended 
element will be automatically highlighted. In this case, an element-highlighting notification 
WGL_NC_LB_ELEM_HLT will be arisen.

If the List Box operates in forced-selecting mode and no element is selected, the appended 
element will be automatically selected. In this case, an element selection notification 
WGL_NC_LB_ELEM_SEL will be arisen.

Input arguments

pv_param1	Contains the element-index before which the element is to be inserted. If the index is 
larger than (element_number-1), this command is failed. This argument must be of type UINT16.

pv_param2	Specifies the contents of all the items of the new inserted element. This parameter 
shall contain an item array with type (WGL_LB_ITEM_T *) to specify all items' contents. If this 
argument is NULL, an empty element is inserted.

Returns

WGLR_OK	The command is successful.

WGLR_INV_CMD	At least one of the following cases occurs: 
 	The style WGL_STL_LB_FIXED_ELEM_NUM is assigned upon this List Box.
 	The element number exceeds ui2_max_elem_num assigned in WGL_LB_INIT_T

WGLR_INV_ARG	At least one of the following cases occurs: 
 	The element index is larger than (element_number-1)
 	The item-types of some of the specified items are not fitted with the item-types of the 
corresponding columns. For example, if a text item is passed into a column that should contain an 
image item, the command is failed and WGLR_INV_ARG is returned. 

WGLR_OUT_OF_MEMORY	There is not enough memory to complete the operation.
*/
#define WGL_CMD_LB_INSERT_ELEM              WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 7)   

/**
This command deletes a specified element. If the style WGL_STL_LB_FIXED_ELEM_NUM is assigned upon 
this List Box, this command fails. 

If the element to be deleted is currently highlighted and the List Box operates in forced-
highlighting mode, after the element is deleted, the List Box will automatically pick one 
highlight-able element and highlight it. In this case, an element-highlighting notification 
WGL_NC_LB_ELEM_HLT will be arisen. 

If the element to be deleted is currently selected and the List Box operates in forced-selecting 
mode, after the element is deleted, the List Box will automatically pick one element and select it
. In this case, an element selection notification WGL_NC_LB_ELEM_SEL will be arisen.

Input arguments

pv_param1	Contains the element-index of the element to be deleted. The index must be less than or 
equal to (element_number-1). This argument must be of type UINT16.

pv_param2	Reserved. This argument must be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_CMD	The style WGL_STL_LB_FIXED_ELEM_NUM is assigned upon this List Box.

WGLR_INV_ARG	The element index is larger than (element_number-1)
*/
#define WGL_CMD_LB_DEL_ELEM                 WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 8)   

/**
This command deletes all elements currently in the List Box. If the style 
WGL_STL_LB_FIXED_ELEM_NUM is assigned upon this List Box, this command fails.

Input arguments

pv_param1	Reserved. This argument must be NULL.

pv_param2	Reserved. This argument must be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_CMD	The style WGL_STL_LB_FIXED_ELEM_NUM is assigned upon this List Box.
*/
#define WGL_CMD_LB_DEL_ALL                  WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 9)   

/**
This command changes the element number in the List Box by appending empty elements or removing 
tail elements. Necessary element-highlighting notifications WGL_NC_LB_ELEM_HLT and element-
selected notifications WGL_NC_LB_ELEM_SEL are arisen if this command causes highlighted-state or 
selected-state change in any elements. If the style WGL_STL_LB_FIXED_ELEM_NUM is assigned upon 
this List Box, this command fails. Note that the specified element number cannot be larger than 
ui2_max_elem_num assigned in WGL_LB_INIT_T; otherwise, this command fails.

Input arguments

pv_param1	Contains the new element number. This argument must be of type UINT16.

pv_param2	Reserved. This argument must be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_CMD	The style WGL_STL_LB_FIXED_ELEM_NUM is assigned upon this List Box.

WGLR_INV_ARG	The new element number is larger than ui2_max_elem_num assigned in WGL_LB_INIT_T.
*/
#define WGL_CMD_LB_SET_ELEM_NUM             WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 10)  

/**
This command gets the element number currently in the List Box. 

Input arguments

pv_param2	Reserved. This argument must be NULL.

Output arguments

pv_param1	Returns the element number. This argument must be of type (UINT16 *). This argument 
cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
*/
#define WGL_CMD_LB_GET_ELEM_NUM             WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 11)  

/**
This command gets the index of currently highlighted element. For the List Box that is not in the 
forced-highlighting mode, it's possible that no element is highlighted. In this case, the 
returned index is WGL_LB_NULL_INDEX. When the List Box contains no element, the returned index is 
also WGL_LB_NULL_INDEX.

Input arguments

pv_param2	Reserved. This argument must be NULL.
Output arguments

pv_param1	Returns the index of currently highlighted element. If no element is highlighted 
currently, WGL_LB_NULL_INDEX is returned. This argument must be of type (UINT16 *). This argument 
cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
*/
#define WGL_CMD_LB_GET_HLT_INDEX            WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 12)  

/**
This command queries if the element with the specified index is highlighted or not.

Input arguments

pv_param1	Contains the index of the element to be queried. If the index is larger than (
element_number-1), this command is failed. This argument must be of type UINT16.

Output arguments

pv_param2	Returns if the specified element is highlighted or not. This argument must be of type (
BOOL *). This argument cannot be NULL.

Returns

WGLR_OK     The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element index is larger than (element_number-1)
 	The argument pv_param2 is NULL
*/
#define WGL_CMD_LB_IS_INDEX_HLT             WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 13)  

/**
This command tries to highlight the next element. This command causes the same effects that are 
caused by the highlighting operations performed by a user when the user presses the NEXT key (e.g
. DOWN key) if the automatic repaint flag in pv_param1 is specified as TRUE. An element-
highlighting notification WGL_NC_LB_ELEM_HLT is arisen if this command causes highlighted-state 
change in any element. In auto-selection mode, the element-selected notification 
WGL_NC_LB_ELEM_SEL is arisen if this command causes selected-state changes in any elements. 

Input arguments

pv_param1	Contains a BOOL flag indicating if an automatic repaint needs to be performed or not. 
TRUE means auto repaint is necessary. This argument must be of type BOOL.

pv_param2	Reserved. This argument must be NULL.

Returns

WGLR_OK	The command is successful.
*/
#define WGL_CMD_LB_HLT_NEXT_ELEM            WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 14)  

/**
This command tries to highlight the previous element. This command causes the same effects that 
are caused by the highlighting operations performed by a user when the user presses the PREV key (
e.g. UP key) if the automatic repaint flag in pv_param1 is specified as TRUE. An element-
highlighting notification WGL_NC_LB_ELEM_HLT is arisen if this command causes highlighted-state 
change in any element. In auto-selection mode, the element-selected notification 
WGL_NC_LB_ELEM_SEL is arisen if this command causes selected-state changes in any elements. 

Input arguments

pv_param1	Contains a BOOL flag indicating if an automatic repaint needs to be performed or not. 
TRUE means auto repaint is necessary. This argument must be of type BOOL.

pv_param2	Reserved. This argument must be NULL.

Returns

WGLR_OK	The command is successful.
*/
#define WGL_CMD_LB_HLT_PREV_ELEM            WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 15)  

/**
This command highlights a specified element. This command causes the same effects that are caused 
by the highlighting operations performed by a user. If the specified element is already in 
highlighted state, this command causes no effects. In case the specified element is in disabled 
state, this command causes no effects. If the auto-selecting mode flag is set, the highlighted 
element will also be automatically selected. An element-highlighting notification 
WGL_NC_LB_ELEM_HLT is arisen if this command causes highlighted-state change in any element. In 
auto-selection mode, the element-selected notification WGL_NC_LB_ELEM_SEL is arisen if this 
command causes selected-state changes in any elements. 

Input arguments

pv_param1	Contains the element-index of the element to be highlighted. The index must be less 
than or equal to (element_number-1). This argument must be of type UINT16.

pv_param2	Reserved. This argument must be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The element index is larger than (element_number-1)
 */
#define WGL_CMD_LB_HLT_ELEM                 WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 16)  

/**
This command is identical to WGL_CMD_LB_HLT_ELEM command, except that this command ensures the 
highlighted element visible at some position in the viewport. It's possible that the highlighted 
element cannot be moved to the specified position. For example, it's impossible to move the 
element with index 0 to the second position in the viewport. In this case, the element will be 
moved to the first position of the viewport, which is the position closest to the specified 
position among all possible positions for this element. In other words, this command moves the 
highlighted element to some point that is as close as possible to the specified position. For 
more information about this command, refer to the command WGL_CMD_LB_HLT_ELEM. 

Input arguments

pv_param1	Contains the element-index of the element to be highlighted. The index must be less 
than or equal to (element_number-1). This argument must be of type UINT16.

pv_param2	Contains the index of the new position for this highlighted element. This argument must 
be of type UINT16.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The element index is larger than (element_number-1)
 */
#define WGL_CMD_LB_HLT_ELEM_VISIBLE         WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 17)  

/**
This command un-highlights a specified element. If the specified element is already in un-
highlighted state, this command causes no effects. This command is valid only when the List Box 
is not in "forced-highlighting mode". An element-highlighting notification WGL_NC_LB_ELEM_HLT is 
arisen if this command causes highlighted-state change in any element.

Input arguments

pv_param1	Contains the element-index of the element to be un-highlighted. The index must be less 
than or equal to (element_number-1). This argument must be of type UINT16.

pv_param2	Reserved. This argument must be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_CMD	The element to be un-highlighted is in highlighted state currently, but the List Box 
is in forced-highlighting mode.

WGLR_INV_ARG	The element index is larger than (element_number-1)
*/
#define WGL_CMD_LB_UNHLT_ELEM               WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 18)  

/**
This command gets the number of currently selected elements in the List Box. 

Input arguments

pv_param2	Reserved. This argument must be NULL.

Output arguments

pv_param1	Returns the selected element number. This argument must be of type (UINT16 *). This 
argument cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
*/
#define WGL_CMD_LB_GET_SEL_NUM              WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 19)  

/** 
This command returns the indexes of all the currently selected elements in the List Box. The 
sender of this command must provide an UINT16 array to receive these indexes. The size (in 
element number) of this array must be at least selected_element_number; otherwise this command 
fails.

Input arguments

pv_param1	Contains the size (in element number) of the input UINT16 array. This argument must be 
of type UINT16.

Output arguments

pv_param2	Returns the indexes of all the selected elements. This argument must be of type (UINT16 
*) which points to an UINT16 array with size at least (selected_element_number). This argument 
cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The argument pv_param1 is less than (selected_element_number)
 	The argument pv_param2 is NULL.
*/
#define WGL_CMD_LB_GET_SEL_INDEXES          WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 20)  

/**
This command queries if no element is selected. If the List Box is in forced-selecting mode, this 
commands always returns FALSE unless it contains no element.

Input arguments

pv_param2	Reserved. This argument must be NULL.
Output arguments

pv_param1	Returns if no element is selected. This argument must be of type (BOOL *). This 
argument cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
*/
#define WGL_CMD_LB_IS_EMPTY_SEL             WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 21)  

/**
This command queries if the element with the specified index is selected or not.

Input arguments

pv_param1	Contains the index of the element to be queried. If the index is larger than (
element_number-1), this command is failed. This argument must be of type UINT16.
Output arguments

pv_param2	Returns if the specified element is selected or not. This argument must be of type (
BOOL *). This argument cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element index is larger than (element_number-1)
 	The argument pv_param2 is NULL.
 */
#define WGL_CMD_LB_IS_INDEX_SEL             WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 22)  

/**
This command is to set the select element by the special index.

Input arguments

pv_param1   Contains the index of the element to be setted. If the index is larger than (
element_number-1), this command is failed. This argument must be of type UINT16.

pv_param2   NULL.

Returns

WGLR_OK         The command is successful.

WGLR_INV_ARG    At least one of the following cases occurs:
                          The element index is larger than (element_number-1).
WGLR_INV_HANDLE      The widget has been destroyed.
*/
#define WGL_CMD_LB_SET_INDEX_SEL            WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 23)  

/**
This command is to not select element by the special index.

Input arguments

pv_param1   Contains the index of the element to be setted. If the index is larger than (
element_number-1), this command is failed. This argument must be of type UINT16.

pv_param2   NULL.

Returns

WGLR_OK         The command is successful.

WGLR_INV_ARG    At least one of the following cases occurs:
                          The element index is larger than (element_number-1).

WGLR_INV_HANDLE      The widget has been destroyed.
*/
#define WGL_CMD_LB_SET_INDEX_DESEL          WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 24) 

/**
This command selects a specified element. If the specified element is already in selected state, 
this command causes no effects. If the specified element is in disabled state, this command 
causes the specified element to get into selected-disabled state. In case the List Box is not in 
multiple-selection mode and there is already an element selected, this command deselects the 
element already selected and selects the newly specified element. In other words, if a List Box 
is not in multiple-selection mode, it operates in toggle mode. 

If the auto-selecting mode flag is set and there's already an element highlighted, this command 
takes no effects.

This command causes the same effects that are caused by the deselecting action performed by a user
. An element selection notification WGL_NC_LB_ELEM_SEL is arisen if this command causes any 
element to change its selected-state.

Input arguments

pv_param1	Contains the element-index of the element to be selected. The index must be less than 
or equal to (element_number-1). This argument must be of type UINT16.

pv_param2	Reserved. This argument must be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The element index is larger than (element_number-1).
*/
#define WGL_CMD_LB_SEL_ELEM                 WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 25) 

/**
This command deselects a specified element. If the specified element is already in deselected 
state, this command causes no effects. In case the List Box is in forced-selecting mode and the 
specified element is the only selected element, this command fails. 

If the auto-selecting mode flag is set and there's already an element highlighted, this command 
takes no effects.

This command causes the same effects that are caused by the deselecting action performed by a user
. An element selection notification WGL_NC_LB_ELEM_SEL is arisen if this command causes any 
element to change its selected-state.

Input arguments

pv_param1	Contains the element-index of the element to be deselected. The index must be less than 
or equal to (element_number-1). This argument must be of type UINT16.

pv_param2	Reserved. This argument must be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_CMD	In case the List Box is in forced-selecting mode and the specified element is the 
only selected element

WGLR_INV_ARG	The element index is larger than (element_number-1)
*/
#define WGL_CMD_LB_DESEL_ELEM               WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 26) 

/**
This command deselects all selected elements. If no element is selected, this command causes no 
effects. If the auto-selecting mode flag is set and there's already an element highlighted, this 
command takes no effects. This command is valid only if the List Box is not in forced-selecting 
mode. Element selection notifications WGL_NC_LB_ELEM_SEL are arisen for each element that changes 
its selected-state during this command. 

Input arguments

pv_param1	Reserved. This argument must be NULL.

pv_param2	Reserved. This argument must be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_CMD	In case the List Box is in forced-selecting mode
*/
#define WGL_CMD_LB_DESEL_ALL                WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 27) 

/**
This command disables a specified element. A disabled element can be selected by commands, but 
cannot be highlighted. Disabling a selected element causes the element to get into selected-
disabled state. The problem is that when the element to be disabled is in highlighted state, one 
of the following effects occurs:
 	In case the List Box is in forced-highlighting mode, the specified element is un-highlighted 
and get into disabled state. However, because the List Box must have one highlighted element, it 
automatically picks one highlight-able element and makes it get into highlighted state. Only if 
there is no highlight-able element, the List Box could have no highlighted element. 
 	In case the List Box is not in forced-highlighting mode, the specified element is un-
highlighted and get into disabled state. After this command, no element is highlighted in the 
List Box. 

If the specified element is already in disabled state, this command causes no effects. All 
necessary element-highlighting notifications WGL_NC_LB_ELEM_HLT are arisen if this command causes 
highlighted-state change in any element. 

Input arguments

pv_param1	Contains the element-index of the element to be disabled. The index must be less than 
or equal to (element_number-1). This argument must be of type UINT16.

pv_param2	Reserved. This argument must be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The element index is larger than (element_number-1)
 */
#define WGL_CMD_LB_DISABLE_ELEM             WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 28) 

/**
This command enables a specified element. An enabled element can be selected and highlighted. 
Note that if the List Box is in forced-highlighting mode and no highlight-able element is 
highlighted now, the enabled element will automatically get into highlighted state. 

If the specified element is already in enabled state, this command causes no effects. All 
necessary element-highlighting notifications WGL_NC_LB_ELEM_HLT are arisen if this command causes 
highlighted-state change in any element. 

Input arguments

pv_param1	Contains the element-index of the element to be enabled. The index must be less than or 
equal to (element_number-1). This argument must be of type UINT16.

pv_param2	Reserved. This argument must be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The element index is larger than (element_number-1)
 */
#define WGL_CMD_LB_ENABLE_ELEM              WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 29) 

/** 
This command queries if the element with the specified index is enabled.

Input arguments

pv_param1	Contains the index of the element to be queried. If the index is larger than (
element_number-1), this command is failed. This argument must be of type UINT16.

Output arguments

pv_param2	Returns if the specified element is enabled. This argument must be of type (BOOL *). 
This argument cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element index is larger than (element_number-1)
 	The argument pv_param2 is NULL.
*/
#define WGL_CMD_LB_IS_INDEX_ENABLED         WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 30) 

/**
This command performs a page-up operation. This command performs the same effect that is 
performed after a user pushes the page-up key. 

Input arguments

pv_param1	Reserved. This argument must be NULL.

pv_param2	Reserved. This argument must be NULL.

Returns

WGLR_OK	The command is successful.
*/
#define WGL_CMD_LB_PAGE_UP                  WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 31) 

/**
This command performs a page-down operation. This command performs the same effect that is 
performed after a user pushes the page-down key. 

Input arguments

pv_param1	Reserved. This argument must be NULL.

pv_param2	Reserved. This argument must be NULL.

Returns

WGLR_OK	The command is successful.
 */
#define WGL_CMD_LB_PAGE_DOWN                WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 32) 

/**
This command makes the currently highlighted element visible. If there is no highlighted element, 
this command has no effect. An element is visible if and only if it is completely visible by the 
user. 

Input arguments

pv_param1	Reserved. This argument must be NULL.

pv_param2	Reserved. This argument must be NULL.

Returns

WGLR_OK	The command is successful.
 */
#define WGL_CMD_LB_ENSURE_HLT_ELEM_VISIBLE  WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 33) 

/**
This command makes the specified element visible. If the specified element is originally not 
visible before this command, the command brings this element into the visible area with minimal 
movement. An element is visible if and only if it is completely visible by the user. 

Input arguments

pv_param1	The index of the element that is to be ensured visible. This argument must be of type 
UINT16.

pv_param2	Reserved. This argument must be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The element index is larger than (element_num-1)
 */
#define WGL_CMD_LB_ENSURE_ELEM_VISIBLE      WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 34) 

/**
This command makes the specified element visible at the specified position in the viewport. It's 
possible that the specified element cannot be moved to the specified position. For example, it's 
impossible to move the element with index 0 to the second position in the viewport. In this case, 
the element will be moved to the first position of the viewport, which is the position closest to 
the specified position among all possible positions for this element. In other words, this 
command moves the specified element to some point that is as close as possible to the specified 
position.

Input arguments

pv_param1	The index of the element that is to be moved to the specified position in the viewport. 
This argument must be of type UINT16.

pv_param2	The index of the new position (in the viewport) for the specified element. This 
argument must be of type UINT16.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The element index is larger than (element_num-1)
*/
#define WGL_CMD_LB_MOVE_ELEM_VISIBLE        WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 35) 

/**
This command is to set the highlight element margin number.It is used for scroll mode.

Input arguments

pv_param1	The highlight number value.

pv_param2	Reserved. This argument must be NULL.

Returns

WGLR_OK	The command is successful.
 */
#define WGL_CMD_LB_SET_HLT_MARGIN           WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 36) 

/**
This command is to get the highlight element margin number.It is used for scroll mode.

Input arguments

pv_param1	Point to the highlight number value.

pv_param2	Reserved. This argument must be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG  The listbox style is not scroll type.
 */
#define WGL_CMD_LB_GET_HLT_MARGIN           WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 37) 

/**
This command sets the control key set of the List Box. The control key set of a List Box is 
defined in WGL_LB_KEY_MAP_T. The control keys of a List Box are used to
 	Change the highlighted element (ui4_key_next, ui4_key_prev)
 	Select / Deselect an element (ui4_key_select)
 	Page-up / page-down

The default control keys of a List Box are:
 	ui4_key_next: KEY_DOWN 
 	ui4_key_prev: KEY_UP 
 	ui4_key_select: KEY_ENTER
 	ui4_key_page_up: (no key assigned)
 	ui4_key_page_down: (no key assigned)

Input arguments

pv_param1	Contains the control key set of the List Box. This argument must be of type (
WGL_LB_KEY_MAP_T *).

pv_param2	Reserved. This argument must be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	pv_param1 is NULL or some key codes are invalid key codes
  */
#define WGL_CMD_LB_SET_KEY_MAP              WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 38) 

/**
This command gets the number of columns in the List Box. 

Input arguments

pv_param2	Reserved. This argument must be NULL.

Output arguments

pv_param1	Returns the column number. This argument must be of type (UINT8 *). This argument 
cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL
 */
#define WGL_CMD_LB_GET_COL_NUM              WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 39) 

/**
This command queries the data type of the specified column. 

Input arguments

pv_param1	Contains the index of the column to be queried. This argument must be of type UINT8.

Output arguments

pv_param2	Returns the data type of the specified column. This argument must be of type (
WGL_LB_COL_TYPE_T *). 

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The index of the column is larger than (column_num-1)
 	pv_param2 is NULL
 */
#define WGL_CMD_LB_GET_COL_TYPE             WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 40) 

/**
This command is to set the tag information for the special element. 

Input arguments

pv_param1   Contains the index of the element to be set. This argument must be of type UINT16.

pv_param2   Point to the tag data information.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The index of the element is larger than (element_num-1)
 */
#define WGL_CMD_LB_SET_ELEM_TAG             WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 41) 

/**
This command is to get the tag data of the special element. 

Input arguments

pv_param1   Contains the index of the element to be set. This argument must be of type UINT16.

Output arguments

pv_param2   Point to the tag data information.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The index of the element is larger than (element_num-1)
 */
#define WGL_CMD_LB_GET_ELEM_TAG             WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 42) 

/**
This command sets the data of each column in the specified element in a batch manner. Note that 
in order to achieve quick data modification, this command does not check the validity of the data 
(e.g. the handle of an image). It's the caller's responsibility to ensure the validity of the 
inputted data.

Input arguments

pv_param1	Contains the index of the element to be set data. This argument must be of type UINT16.

pv_param2	Contains a pointer to the buffer of the structure WGL_LB_ELEM_DATA_T. Therefore, this 
argument must be of type (WGL_LB_ELEM_DATA_T *). If this argument is NULL, the specified element 
will be cleared.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element index specified in pv_param1 is invalid
*/
#define WGL_CMD_LB_SET_ELEM_DATA            WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 43) 

/**
This command retrieves the data of each column in the specified element in a batch manner

Input arguments

pv_param1	Contains the index of the element to be queried data. This argument must be of type 
UINT16.

Output arguments

pv_param2	Contains a pointer to the buffer of the structure WGL_LB_ELEM_DATA_T. Therefore, this 
argument must be of type (WGL_LB_ELEM_DATA_T *) and cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The element index specified in pv_param1 is invalid
 	The argument pv_param2 is NULL.
*/
#define WGL_CMD_LB_GET_ELEM_DATA            WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 44) 

/** 
This command sets the data of a batch of elements in the specified range. Note that in order to 
achieve quick data modification, this command does not check the validity of the data (e.g. the 
handle of an image). It's the caller's responsibility to ensure the validity of the inputted data.

Input arguments

pv_param1	Contains the range of elements to be set data. The range is specified as WGL_PACK_2(
element-index-start, element-index-end). This argument must be packed as WGL_PACK_2(UINT16, UINT16
).

pv_param2	Contains the array of elements data. This argument must be of type (WGL_LB_ELEM_DATA_T *
), which contains a pointer to an element data array prepared by the caller of this command. The 
size of the data array shall be at least (element-index-end - element-index-start + 1). This 
argument cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The argument pv_param2 is NULL
 	The element range specified in pv_param1 is invalid.
*/
#define WGL_CMD_LB_SET_ELEMS_BY_RANGE       WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 45) 

/**
This command clears the data of the specified element.

Input arguments

pv_param1	Contains the index of the element to be cleared. This argument must be of type UINT16. 

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The element index specified in pv_param1 is invalid.
*/
#define WGL_CMD_LB_CLEAR_ELEM_DATA          WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 46) 

/**
This command clears the data of a batch of elements in the specified range. 

Input arguments

pv_param1	Contains the start index of the range of elements to be set data. This argument must be 
of type UINT16.

pv_param2	Contains the end index of the range of elements to be set data. This argument must be 
of type UINT16.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The element range specified in pv_param1 and pv_param2 is invalid. 
  */
#define WGL_CMD_LB_CLEAR_ELEMS_BY_RANGE     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 47) 

/** 
This command sets the text for the specified text item. This command is valid only for text items
. If the length of the text string is larger than WGL_MAX_STRING, the text string will be 
truncated.

Input arguments

pv_param1	Contains both of the index of the element and the index of the item to be set text. The 
element index and item index must be packed by WGL_PACK_2(element-index, item-index). This 
argument must be packed as WGL_PACK_2(UINT16, UINT8). 

pv_param2	Contains the text string to be set. This argument must be of type (UTF16_T *). If this 
argument is NULL, an empty text is set. 

Returns

WGLR_OK	The command is successful.

WGLR_INV_CMD	The specified item is not a text item

WGLR_INV_ARG	At least one of the following cases occurs:
 	The index of the element is larger than (element_number-1)
 	The index of the item is larger than (column_num-1)
*/
#define WGL_CMD_LB_SET_ITEM_TEXT            WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 48) 

/**
This command gets the text for the specified text item. This command is valid only for text items. 

Input arguments

pv_param1	Contains the index of the element and the index of the item to be queried text. The 
element index and item index must be packed by WGL_PACK_2(element-index, item-index). This 
argument must be packed as WGL_PACK_2(UINT16, UINT8). 

Output arguments

pv_param2	Returns the text string of the text item. The text of the specified text item will be 
copied into the text buffer referred by pv_param2. Therefore, pv_param2 must be of type (UTF16_T *
) and refer to a text buffer that must be long enough for containing the returned text string. 
More specifically, the length of this text buffer must be more than the maximal text length 
defined for this column. This argument cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_CMD	The specified item is not a text item

WGLR_INV_ARG	At least one of the following cases occurs:
 	pv_param2 is NULL
 	The index of the element is larger than (element_number-1)
 	The index of the item is larger than (column_num-1)
  */
#define WGL_CMD_LB_GET_ITEM_TEXT            WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 49) 

/**
This command sets the image for the specified image item. This command is valid only for image 
items. 

Input arguments

pv_param1	Contains the index of the element and the index of the item to be set image. The 
element index and item index must be packed by WGL_PACK_2(element-index, item-index). This 
argument must be packed as WGL_PACK_2(UINT16, UINT8). 

pv_param2	Contains the image handle to be set. This argument must be of type WGL_HIMG_TPL_T. If 
this argument is NULL, an empty image is set. 

Returns

WGLR_OK	The command is successful.

WGLR_INV_CMD	The specified item is not an image item

WGLR_INV_ARG	At least one of the following cases occurs:
 	The input image handle is an invalid handle 
 	The index of the element is larger than (element_number-1)
 	The index of the item is larger than (column_num-1)
 */
#define WGL_CMD_LB_SET_ITEM_IMAGE           WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 50) 

/**
This command queries the handle of the image for the specified image item. This command is valid 
only for image items. 

Input arguments

pv_param1	Contains the index of the element and the index of the item to be queried image. The 
element index and item index must be packed by WGL_PACK_2(element-index, item-index). This 
argument must be packed as WGL_PACK_2(UINT16, UINT8). 

Output arguments

pv_param2	Returns the image handle of the item's image. This argument must be of type (
WGL_HIMG_TPL_T *). This argument cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_CMD	The specified item is not a image item

WGLR_INV_ARG	At least one of the following cases occurs:
 	pv_param2 is NULL
 	The index of the element is larger than (element_number-1)
 	The index of the item is larger than (column_num-1)
  */
#define WGL_CMD_LB_GET_ITEM_IMAGE           WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 51) 

/**
This command sets the image-suit for the specified image-suit item. This command is valid only 
for image-suit items. 

Input arguments

pv_param1	Contains the index of the element and the index of the item to be set image-suit. The 
element index and item index must be packed by WGL_PACK_2(element-index, item-index). This 
argument must be packed as WGL_PACK_2(UINT16, UINT8). 

pv_param2	Contains the image-suit to be set. This argument must be of type (
WGL_LB_ITEM_IMG_SUIT_T *). If this argument is NULL, an empty image-suit is set. 

Returns

WGLR_OK	The command is successful.

WGLR_INV_CMD	The specified item is not an image-suit item

WGLR_INV_ARG	At least one of the following cases occurs:
 	The image-suit contains some invalid image handles 
 	The index of the element is larger than (element_number-1)
 	The index of the item is larger than (column_num-1)
  */
#define WGL_CMD_LB_SET_ITEM_IMAGE_SUIT      WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 52) 

/**
This command queries the image-suit for the specified image-suit item. This command is valid only 
for image-suit items. 

Input arguments

pv_param1	Contains the index of the element and the index of the item to be queried image-suit. 
The element index and item index must be packed by WGL_PACK_2(element-index, item-index). This 
argument must be packed as WGL_PACK_2(UINT16, UINT8). 

Output arguments

pv_param2	Returns the item's image-suit. The content of the image-suit will be copied into the 
WGL_LB_ITEM_IMG_SUIT_T structure pointed by this argument. This argument must be of type (
WGL_LB_ITEM_IMG_SUIT_T *). This argument cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_CMD	The specified item is not an image-suit item

WGLR_INV_ARG	At least one of the following cases occurs:
 	pv_param2 is NULL
 	The index of the element is larger than (element_number-1)
 	The index of the item is larger than (column_num-1)
  */
#define WGL_CMD_LB_GET_ITEM_IMAGE_SUIT      WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 53) 

/**
This command registers listbox with the special event handler information. As only WGL_LB_EVN_HDLR_TYPE_NVGT_IN_VP
is supported presently, the command should not be done in WGL_STL_LB_FORCE_HLT style.

Input arguments

pv_param1   Point to the event handle information(WGL_LB_EVN_HDLR_INFO_T). 

pv_param2	Reserved. 

Returns

WGLR_OK	The command is successful.

WGLR_INV_CMD    The command is done in WGL_STL_LB_FORCE_HLT style.

WGLR_INV_ARG	At least one of the following cases occurs:
             Event handle function point is null
             The event handle style is not WGL_LB_EVN_HDLR_MASK_NVGT_IN_VP
 */
#define WGL_CMD_LB_REG_EVN_HDLR_INFO        WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 54) 

/**
This command gets the current event handler information. 

Output arguments

pv_param1   Point to the event handle information(WGL_LB_EVN_HDLR_INFO_T). 

pv_param2   Reserved. 

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG    The pv_param1 is null.
 */
 
#define WGL_CMD_LB_GET_EVN_HDLR_INFO        WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 55) 

/**
This command set listbox notify callback function. 

Output arguments

pv_param1   Point to notify function wgl_lb_cb_fct

pv_param2   Reserved. 

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG    The pv_param1 is null.
 */
#define WGL_CMD_LB_SET_NTF_CB               WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 56) 

#ifdef WGL_ANIM_SUPPORT

/**
This command create listbox animation function. 

Output arguments

pv_param1   Point to UINT32,that is style information

pv_param2   Reserved. 

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG    The pv_param1 is null.
 */
#define WGL_CMD_LB_CREAT_ANIM                WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 57) 

/**
This command destroy listbox animation function. 

Output arguments

pv_param1   Reserved.

pv_param2   Reserved. 

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG    The pv_param1 is null.
 */
#define WGL_CMD_LB_DESTROY_ANIM              WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 58) 
/*
#define WGL_CMD_LB_SET_ANIM_MARGIN           WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 59) 

#define WGL_CMD_LB_GET_ANIM_MARGIN           WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 60) 
*/
/**
This command set animation action mapping information.If it is exist,change it.

Output arguments

pv_param1   Point to the new action map information using the struction WGL_ANIM_ACTION_MAP_T.

pv_param2   Reserved. 

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG    The pv_param1 is null.
 */
#define WGL_CMD_LB_SET_ANIM_ACTMAP           WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 61) 

/**
This command get animation action mapping information.

Output arguments

pv_param1   Point to the getting action map information using the struction WGL_ANIM_ACTION_MAP_T.

pv_param2   Reserved. 

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG    The pv_param1 is null.
 */
#define WGL_CMD_LB_GET_ANIM_ACTMAP           WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 62) 

/** This command is used to set listbox animation image information.

input arguments:

pv_param1: The image sequence index to set.

pv_param2: Point ot WGL_ANIM_IMG_SEQ_T,it contain the image table information.

Returns:

WGLR_OK: The function call is successful.

WGLR_INV_ARG:  The information to get is not valid information.

WGLR_INV_HANDLE: The widget handle or the animation handle is invalid.   
*/   
#define WGL_CMD_LB_ANIM_SET_IMG_SEQ            WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 63) 

/** This command is used to set animation max image sequence number. It must set before setting image 
seq information and the number will not change after it setted.

input arguments:

pv_param1:   Point to UINT16.

pv_param2:   Reserve.

Returns:

WGLR_OK: The function call is successful.

WGLR_INV_ARG:  The information to get is not valid information.

WGLR_INV_HANDLE: The widget handle or the animation handle is invalid.    
*/ 
#define WGL_CMD_LB_ANIM_SET_MAX_IMG_SEQ            WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 64) 

/** This command is used to set animation data information that is cannot getted from the listbox internal.
For example,the image sequence information.It may have more than one animation type data that listbox needed.
In this case the need animation will get data sequently.

input arguments:

pv_param1:   The animation data information count.

pv_param2:   Point to the struction array,WGL_ANIM_DATA_INFO.

Returns:

WGLR_OK: The function call is successful.

WGLR_INV_ARG:  The information to get is not valid information.
*/
#define WGL_CMD_LB_ANIM_SET_DATA_INFO              WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 65) 

/** This command is used to set\clear animation flag information that will control some animation function or flow.

input arguments:

pv_param1:   Point to BOOL data,TRUE:set the pv_param2 flag; FLASE:clear this flag.

pv_param2:   Point to UINT32 data that indicating the flag.

Returns:

WGLR_OK: The function call is successful.

WGLR_INV_ARG:  The information to get is not valid information.
*/
#define WGL_CMD_LB_ANIM_SET_FLAG                   WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 66) 


/** This command is used to 

input arguments:

pv_param1:   

pv_param2:   

Returns:

WGLR_OK: The function call is successful.

WGLR_INV_ARG:  The information to get is not valid information.
*/
#define WGL_CMD_LB_LBW_RLD                          WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 67) 

/** This command is used to 

input arguments:

pv_param1:   

pv_param2:   

Returns:

WGLR_OK: The function call is successful.

WGLR_INV_ARG:  The information to get is not valid information.
*/
#define WGL_CMD_LB_LBW_RLD_PARTIAL                     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 68) 

#define WGL_CMD_LB_GET_ELEM_TOTAL_NUM                  WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 69) 


#endif

/** This command is used to get the current viewport visible range information.
 *  
 *  input arguments:
 *
 *  pv_param1:  It use point to UINT32 data that include first and last index. 
 *  The up 16 bits are first index of current viewport;the below 16 bits are last index.
 *
 *  pv_param2:  Reserved. 
 *
 *  Returns:
 *
 *  WGLR_OK: The function call is successful.
 *
 *  WGLR_INV_ARG:  The information to get is not valid information.
 *  */
#define WGL_CMD_LB_GET_VISIBLE_RANGE                    WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_LIST_BOX, 70) 

/*-----------------------------------------------------------------------------
                    List Box Specific Notification Codes
 ----------------------------------------------------------------------------*/
/**This notification is arisen when one of the following cases occurs:1. The user presses a key to change the highlighted 
element. 2. The WGL_CMD_LB_HLT_ELEM \WGL_CMD_LB_UNHLT_ELEM command is executed and the highlighted 
element is changed 3. Any other commands that may change the highlighted state of any element.*/ 
#define WGL_NC_LB_ELEM_HLT              ((UINT32)WGL_NC_WIDGET_SPF_START + 0)    

/**This notification is arisen when one of the following cases occurs: 1. The user presses a key to select or deselect of an element.
2. The WGL_CMD_LB_SEL_ELEM command is executed and the selected-state of the specified element is 
changed. 3.The WGL_CMD_LB_DESEL_ELEM command is executed and the selected-state of the specified element 
is changed. 4.Any other commands that may change the highlighted state of any element. */
#define WGL_NC_LB_ELEM_SEL              ((UINT32)WGL_NC_WIDGET_SPF_START + 1) 

/**This notification is arisen when a user presses the select key on a highlighted element. It will not be arisen when a user presses the 
select key but no any element is highlighted. For each push action on a highlighted element, two separated WGL_NC_LB_ELEM_PUSH 
notifications will be arisen: one indicates the push-down event while the other is arisen right after the user releases the select key. */
#define WGL_NC_LB_ELEM_PUSH             ((UINT32)WGL_NC_WIDGET_SPF_START + 2)  

/**This notification is arisen when no highlight-able element prior to the current highlighted 
element but someone still tried to highlight an element prior to the current highlighted element. */
#define WGL_NC_LB_REACH_TOP             ((UINT32)WGL_NC_WIDGET_SPF_START + 3)    

/**This notification is arisen when no highlight-able element posterior to the current highlighted 
element but someone still tried to highlight an element posterior to the current highlighted element. */
#define WGL_NC_LB_REACH_BOTTOM          ((UINT32)WGL_NC_WIDGET_SPF_START + 4)    


/*-----------------------------------------------------------------------------
                    List Box Specific Styles
 ----------------------------------------------------------------------------*/
 /**If this style is assigned, the List Box operates in forced- highlighting mode. */
#define WGL_STL_LB_FORCE_HLT            (((UINT32)(WGL_STL_WGT_SPF_START))<<0)    

/**If this style is assigned, the List Box operates in forced-selecting mode. */
#define WGL_STL_LB_FORCE_SEL            (((UINT32)(WGL_STL_WGT_SPF_START))<<1)    

/**If this style is assigned, the List Box operates in multiple-selection mode. Otherwise, the List 
Box allows at most only one element to be selected at a time. Note the style WGL_STL_LB_AUTO_SEL must NOT be assigned. */
#define WGL_STL_LB_MULTI_SEL            (((UINT32)(WGL_STL_WGT_SPF_START))<<2)    

/**If this style is NOT assigned, elements in the List Box will be highlighted in wrap over mode. */
#define WGL_STL_LB_NO_WRAP_OVER         (((UINT32)(WGL_STL_WGT_SPF_START))<<3)    

/**If this style is assigned, the element number of the List Box is determined during initialization 
and fixed after initialization.  */
#define WGL_STL_LB_FIXED_ELEM_NUM       (((UINT32)(WGL_STL_WGT_SPF_START))<<4)    

/**If this style is assigned, the List Box doesn't do the smart cut for all texts in the List Box. 
If not assigned, the over part will be replaced by '…'*/
#define WGL_STL_LB_NO_SMART_CUT         (((UINT32)(WGL_STL_WGT_SPF_START))<<5)    

/**If this style is assigned, the List Box automatically selects an element whenever the element is 
highlighted. In other words, this style flag always ensures the currently highlighted element to 
be selected. Note that if this style is assigned, the WGL_STL_LB_MULTI_SEL must NOT be assigned.   */
#define WGL_STL_LB_AUTO_SEL             (((UINT32)(WGL_STL_WGT_SPF_START))<<6)    

/**If this style is assigned, when users attempt to highlight an invisible element by remote control 
keys, List Box shows up invisible elements in page-up/page-down manner, instead of scrolling 
elements into the viewport. */
#define WGL_STL_LB_NO_SCROLL            (((UINT32)(WGL_STL_WGT_SPF_START))<<7)    

/**If this style is assigned, the highlighted element never gets into pushed state when the user 
presses the select key (e.g. ENTER) on it.  */
#define WGL_STL_LB_ELEM_NO_PUSH         (((UINT32)(WGL_STL_WGT_SPF_START))<<8)    

/**If this style is assigned ,it use suppress notify. */
#define WGL_STL_LB_SUPPRESS_NFY         (((UINT32)(WGL_STL_WGT_SPF_START))<<9)    

/**If this style is assigned,it ingore the newline symbor.  */
#define WGL_STL_LB_IGNORE_NEWLINE       (((UINT32)(WGL_STL_WGT_SPF_START))<<10)   

/**If this style is assigned, it operates in customization mode.It doesn't automatically repaint itself after 
certain internal messages are processed. Not be automatically repainted are WGL_MSG_KEY_DOWN\UP,WGL_MSG_COMMAND.*/
#define WGL_STL_LB_CUSTOM_MODE          (((UINT32)(WGL_STL_WGT_SPF_START))<<11)   

#ifdef WGL_LB_NO_STORE_TEXT
/**If this style is assigned, the listbox widget will not store the text information,it will get text information by
wgl_lb_cb_fct when it need draw text.*/
#define WGL_STL_LB_NO_STORE_TEXT        (((UINT32)(WGL_STL_WGT_SPF_START))<<12)   
#endif

/**If this style is assigned, the listbox animation mode is by the action map mode.It should will use default or not.*/
#define WGL_STL_LB_ANIM_BY_ACT_MAP      (((UINT32)(WGL_STL_WGT_SPF_START))<<13)   

/*It not used now*/
#define WGL_STL_LB_ANIM_BY_CUST_PATH    (((UINT32)(WGL_STL_WGT_SPF_START))<<14)   

/*If this style is assigned, it will force diable the listbox animation wheneve the default listbox animation is opened or not.*/
#define WGL_STL_LB_ANIM_FORCE_DISABLE    (((UINT32)(WGL_STL_WGT_SPF_START))<<15)   

#define WGL_STL_LB_LBW                  (((UINT32)(WGL_STL_WGT_SPF_START))<<16)   

#define WGL_STL_LB_SUPPORT_MULTILINE    (((UINT32)(WGL_STL_WGT_SPF_START))<<17)   

/**This is the last style. */
#define WGL_STL_LB_LAST                 (((UINT32)(WGL_STL_WGT_SPF_START))<<18)   




/**Define the notify type */
typedef UINT8 WGL_LB_NFY_TYPE_T;

#define WGL_LB_NFY_REQ_ELEM_TEXT_DATA           ((WGL_LB_NFY_TYPE_T)1)

#define WGL_LB_NFY_REQ_ELEM_DATA                ((WGL_LB_NFY_TYPE_T)2)


#if 1/*def WGL_ANIM_SUPPORT*/
/*Listbox animation action flag,the ui4_flag in WGL_ANIM_ACTION_MAP_T*/

/**If this flag is assigned, the listbox has WGL_STL_LB_NO_SCROLL style and it will do animation while switch page.
Deafault it will not do animation while switch page when it has assigned WGL_STL_LB_NO_SCROLL*/
#define WGL_LB_ACTMAP_SWITCH_PAGE_ANIM          MAKE_BIT_MASK_32(0)


/*Listbox animation flag*/
/**If this flag is assigned, it will force disable this widget animation whenever it open animation style or using default 
animation. If this flag is cleared, it will enable animation when it open animation function.*/
#define WGL_LB_ANIM_FLAG_DISABLE                MAKE_BIT_MASK_32(0)
#endif

/*-----------------------------------------------------------------------------
                    Macros
 ----------------------------------------------------------------------------*/
/**This macro retrieves the disabled-state of the specified element in the state buffer. */ 
#define WGL_LB_IS_ELEM_DIS(_av_elem_states ,_ui2_idx_elem)      \
        WGL_IS_FLAG_TURN_ON(((UINT8 *)(_av_elem_states))[_ui2_idx_elem], (((UINT8)1)<<0))    

/**This macro retrieves the highlighted-state of the specified element in the state buffer. */ 
#define WGL_LB_IS_ELEM_HLT(_av_elem_states ,_ui2_idx_elem)      \
        WGL_IS_FLAG_TURN_ON(((UINT8 *)(_av_elem_states))[_ui2_idx_elem], (((UINT8)1)<<1))

/**This macro retrieves the selected-state of the specified element in the state buffer. */ 
#define WGL_LB_IS_ELEM_SEL(_av_elem_states ,_ui2_idx_elem)      \
        WGL_IS_FLAG_TURN_ON(((UINT8 *)(_av_elem_states))[_ui2_idx_elem], (((UINT8)1)<<2))


/*-----------------------------------------------------------------------------
                    Contants
 ----------------------------------------------------------------------------*/
#define WGL_LB_MAX_ELEM_NUM          ((UINT16)0xFF00)  /**<Indicates the maximal number of elements a List Box may contain. */
#define WGL_LB_NULL_INDEX            (WGL_LB_MAX_ELEM_NUM+1) /**<Indicates a dummy index that is not an index of any element. */
#define WGL_LB_MAX_ELEM_SIZE         ((UINT16)0xFF00)  /**<Indicates the maximal size of each element in the List Box. */
#define WGL_LB_DEF_ELEM_SIZE         (WGL_LB_MAX_ELEM_SIZE+1) /**<Indicates the default element size of the List Box. */
#define WGL_LB_MAX_ELEM_DIST         ((UINT16)0xFF00)  /**<Indicates the maximal distance (in pixel) between elements in the List Box. */
#define WGL_LB_DEF_ELEM_DIST         (WGL_LB_MAX_ELEM_DIST+1)  /**<Indicates the default distance (in pixel) between elements in the List Box. */
#define WGL_LB_ELEM_STATE_BUF_SIZE   (1)    /**< Indicates the element state buffer size for a single element. This constant may be used 
when an application needs to prepare the buffer before doing the command WGL_CMD_LB_STORE_ALL_ELEM_STATE.*/


/**<  indicates font attribute type is uniform width.    */
#define WGL_LB_FONT_ATTR_UNIFORM_WIDTH    ((UINT8) 0x0)   

/**<   indicates font attribute type is unrecognized character.  */
#define WGL_LB_FONT_ATTR_UNREC_CHAR       ((UINT8) 0x1)   

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
/* column types */
/*------------------------------------------------------------------*/
/*! @enum WGL_LB_COL_TYPE_T
 *  @brief This structure enumerates the types of columns in a List Box.
 *  @code
 *  typedef enum _WGL_LB_COL_TYPE_T
 *  {
 *      LB_COL_TYPE_TEXT = 0,
 *      LB_COL_TYPE_IMG = 1,
 *      LB_COL_TYPE_IMG_SUIT = 2
 *  } WGL_LB_COL_TYPE_T;
 *  @endcode
 *  @li@c  LB_COL_TYPE_TEXT                                  -Indicates this column shall contain a text. 
 *  @li@c  LB_COL_TYPE_IMG                                   -Indicates this column shall contain an image. 
 *  @li@c  LB_COL_TYPE_IMG_SUIT                              -Indicates this column shall contain an image suit. 
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_LB_COL_TYPE_T
{
    LB_COL_TYPE_TEXT = 0,
    LB_COL_TYPE_IMG = 1,
    LB_COL_TYPE_IMG_SUIT = 2
} WGL_LB_COL_TYPE_T;


/*------------------------------------------------------------------*/
/*! @struct WGL_LB_ITEM_IMG_SUIT_T
 *  @brief This structure holds a suit of item images, namely image-suit, used for an image-suit item. 
 *  @code
 *  typedef struct _WGL_LB_ITEM_IMG_SUIT_T
 *  {
 *      WGL_HIMG_TPL_T              h_normal;
 *      WGL_HIMG_TPL_T              h_disable;
 *      WGL_HIMG_TPL_T              h_highlight;
 *      WGL_HIMG_TPL_T              h_highlight_unfocus;
 *      WGL_HIMG_TPL_T              h_pushed;
 *      WGL_HIMG_TPL_T              h_selected;            
 *      WGL_HIMG_TPL_T              h_selected_disable;
 *      WGL_HIMG_TPL_T              h_selected_highlight;
 *      WGL_HIMG_TPL_T              h_selected_highlight_unfocus;
 *      WGL_HIMG_TPL_T              h_selected_pushed;
 *  } WGL_LB_ITEM_IMG_SUIT_T;    
 *  @endcode
 *  @li@c  h_normal                                -Contains the item image related to the normal state of an element. An element is in this state 
when it's in un-highlighted, deselected state and when the List Box looks in the enabled state.
 *  @li@c  h_disable                                -Contains the item image related to the disabled state of an element. A deselected element is in 
this state when it's in disabled or when the List Box looks in disabled state.
 *  @li@c  h_highlight                              -Contains the item image related to the highlighted state of an element when the element is 
highlighted and the List Box looks in focused state.
 *  @li@c  h_highlight_unfocus                  -Contains the item image related to the highlighted & unfocus state of an element when the element 
is highlighted and the List Box looks in unfocused or inactive state.
 *  @li@c  h_pushed                                -Contains the item image related to the pushed state of an element when the element is pushed.
 *  @li@c  h_selected                               -Contains the item image related to the selected state of an element when the element is selected 
but not disabled when the List Box doesn't look in disabled state.
 *  @li@c  h_selected_disable                    -Contains the item image related to the selected state of an element when the element is selected 
but not disabled when the List Box doesn't look in disabled state.
 *  @li@c  h_selected_highlight                 -Contains the item image related to the selected & highlighted state of an element when the 
element is highlighted and selected when the List Box looks in focused state.
 *  @li@c  h_selected_highlight_unfocus    -Contains the item image related to selected & highlighted &unfocus state of an element.
 *  @li@c  h_selected_pushed                   -Contains the item image related to the pushed & selected state of an element 
 when the selected element is pushed.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_LB_ITEM_IMG_SUIT_T
{
    WGL_HIMG_TPL_T              h_normal;
    WGL_HIMG_TPL_T              h_disable;
    WGL_HIMG_TPL_T              h_highlight;
    WGL_HIMG_TPL_T              h_highlight_unfocus;
    WGL_HIMG_TPL_T              h_pushed;
    
    WGL_HIMG_TPL_T              h_selected;            
    WGL_HIMG_TPL_T              h_selected_disable;
    WGL_HIMG_TPL_T              h_selected_highlight;
    WGL_HIMG_TPL_T              h_selected_highlight_unfocus;
    WGL_HIMG_TPL_T              h_selected_pushed;
} WGL_LB_ITEM_IMG_SUIT_T;    


/*------------------------------------------------------------------*/
/*! @struct WGL_LB_ITEM_DATA_T
 *  @brief This struct hold the data of an item.
 *  @code
 * typedef union _WGL_LB_ITEM_DATA_T
 * {
 *     UTF16_T                     *pw2s_text;    
 *     WGL_HIMG_TPL_T               h_img;
 *     WGL_LB_ITEM_IMG_SUIT_T      *pt_img_suit;
 *  } WGL_LB_ITEM_DATA_T;
 *  @endcode
 *  @li@c  pw2s_text                      -Contains the text of a text item.
 *  @li@c  h_img                            -Contains the handle of an image item's image
 *  @li@c  pt_img_suit                    -Contains the image-suit for the image-suit item.
 */
/*------------------------------------------------------------------*/
typedef union _WGL_LB_ITEM_DATA_T
{
    UTF16_T                     *pw2s_text;    
    WGL_HIMG_TPL_T               h_img;
    WGL_LB_ITEM_IMG_SUIT_T      *pt_img_suit;
} WGL_LB_ITEM_DATA_T;


/*------------------------------------------------------------------*/
/*! @struct WGL_LB_ITEM_T
 *  @brief This structure holds the data of an item information.
 *  @code
 *  typedef struct _WGL_LB_ITEM_T
 *  {
 *      WGL_LB_COL_TYPE_T           e_col_type;
 *      WGL_LB_ITEM_DATA_T          data;
 *  } WGL_LB_ITEM_T;
 *  @endcode
 *  @li@c  e_col_type          -Indicates which type of the data is contained in the "data" union. 
                                          LB_COL_TYPE_TEXT indicates the field pw2s_text is used. 
                                          LB_COL_TYPE_IMG indicates the field h_img is used. 
                                          LB_COL_TYPE_IMG_SUIT indicates the field pt_img_suit is used. 
 *  @li@c  data                   -The data of item.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_LB_ITEM_T
{
    WGL_LB_COL_TYPE_T           e_col_type;
    WGL_LB_ITEM_DATA_T          data;
} WGL_LB_ITEM_T;


/*------------------------------------------------------------------*/
/*! @struct WGL_LB_ELEM_DATA_T
 *  @brief This structure holds the data of element.
 *  @code
 *  typedef struct _WGL_LB_ELEM_DATA_T
 *  {
 *      WGL_LB_ITEM_DATA_T          *at_items;
 *  } WGL_LB_ELEM_DATA_T;
 *  @endcode
 *  @li@c  *at_items                                         -Contains the datas of item.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_LB_ELEM_DATA_T
{
    WGL_LB_ITEM_DATA_T          *at_items;
} WGL_LB_ELEM_DATA_T;


/** flags for WGL_LB_COL_INIT_T.ui2_width :
case 1: !WGL_LB_COL_WIDTH_STL_AUTO && !WGL_LB_COL_WIDTH_STL_COMPACT 
         -> WGL_LB_GET_COL_WIDTH(): column width                     
case 2:  WGL_LB_COL_WIDTH_STL_AUTO && !WGL_LB_COL_WIDTH_STL_COMPACT 
         -> WGL_LB_GET_COL_WIDTH(): 0 if item is empty               
                                   the item's width if the item is not empty 
case 3:  WGL_LB_COL_WIDTH_STL_AUTO &&  WGL_LB_COL_WIDTH_STL_COMPACT 
         -> WGL_LB_GET_COL_WIDTH(): the maximum item width  */
         
#define WGL_LB_COL_WIDTH_STL_AUTO        MAKE_BIT_MASK_16(15)    
#define WGL_LB_COL_WIDTH_STL_COMPACT     MAKE_BIT_MASK_16(14)    

#define WGL_LB_COL_WIDTH_STL_LAST_FLAG   WGL_LB_COL_WIDTH_STL_COMPACT    


/**It indicates max width of column */
#define WGL_LB_MAX_COL_WIDTH             ((WGL_LB_COL_WIDTH_STL_LAST_FLAG >> 1) - 1)   

/**It get the width of column */
#define WGL_LB_GET_COL_WIDTH(_ui2_col_w) ((_ui2_col_w) & ((WGL_LB_COL_WIDTH_STL_LAST_FLAG >> 1) - 1))

/*------------------------------------------------------------------*/
/*! @struct WGL_LB_COL_INIT_T
 *  @brief This structure holds the initialization formats for a column.
 *  @code
 *  typedef struct _WGL_LB_COL_INIT_T
 *  {
 *      WGL_LB_COL_TYPE_T           e_col_type;
 *      UINT8                       ui1_align;
 *      UINT16                      ui2_width;
 *      UINT16                      ui2_max_text_len;
 *  } WGL_LB_COL_INIT_T;
 *  @endcode
 *  @li@c  e_col_type            -Indicates column type. Refer to the section WGL_LB_COL_TYPE_T for more information.
 *  @li@c  ui1_align              -Indicates the default alignment of this column. 
 *  @li@c  ui2_width             -Indicates width of this column. This field can be zero.
 *  @li@c  ui2_max_text_len  -Indicates the max character number of the text in the text item,only for text.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_LB_COL_INIT_T
{
    WGL_LB_COL_TYPE_T           e_col_type;
    UINT8                       ui1_align;
    UINT16                      ui2_width;
    UINT16                      ui2_max_text_len;
} WGL_LB_COL_INIT_T;

/*------------------------------------------------------------------*/
/*! @struct  WGL_LB_INIT_T
 *  @brief This structure holds the data needed to initialize a List Box. The pointer of this structure will be passed as the pv_param argument of the API 
x_wgl_create_widget when a List Box is created.
 *  @code
 *  typedef struct _WGL_LB_INIT_T
 *  {
 *      UINT32                      ui4_style;
 *      UINT16                      ui2_max_elem_num;
 *      UINT8                       ui1_col_num;
 *      WGL_LB_COL_INIT_T           *at_cols;
 *      UINT16                      ui2_elem_size;
 *      UINT16                      ui2_elem_distance;
 *  }  WGL_LB_INIT_T;
 *  @endcode
 *  @li@c  ui4_style                      -Contains the style of the List Box. 
 *  @li@c  ui2_max_elem_num       -Indicates the maximal number of elements in the List Box. It provides important information for List Box to
 optimize the memory utilization and to assign this number as smaller as possible.If this argument is zero, it means the maximal element number cannot be determined. 
 *  @li@c  ui1_col_num                 -Contains the number of columns in the List Box. This field cannot be zero.
 *  @li@c  *at_cols                       -Contains an WGL_LB_COL_INIT_T array consisting of the initialization information of all columns 
in the List Box. This field cannot be NULL. 
 *  @li@c  ui2_elem_size               -Contains the pixel size (either width or height) of each element. This field cannot be zero. 
 If WGL_LB_DEF_ELEM_SIZE is assigned for this field, the default element size is used.
 *  @li@c  ui2_elem_distance         -Contains the distance (in pixel) between two elements. If WGL_LB_DEF_ELEM_DIST is assigned for 
this field, the default element size is used.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_LB_INIT_T
{
    UINT32                      ui4_style;
    UINT16                      ui2_max_elem_num;
    UINT8                       ui1_col_num;
    WGL_LB_COL_INIT_T           *at_cols;
    
    UINT16                      ui2_elem_size;
    UINT16                      ui2_elem_distance;
}  WGL_LB_INIT_T;


/*------------------------------------------------------------------*/
/*! @struct WGL_LB_KEY_MAP_T
 *  @brief This structure holds the control keys of the List Box.
 *  @code
 *  typedef struct _WGL_LB_KEY_MAP_T
 *  {
 *      UINT32                      ui4_key_next;
 *      UINT32                      ui4_key_prev;
 *      UINT32                      ui4_key_select;
 *      UINT32                      ui4_key_page_up;
 *      UINT32                      ui4_key_page_down;
 *  } WGL_LB_KEY_MAP_T;    
 *  @endcode
 *  @li@c  ui4_key_next                -Contains the key code of the key that changes the highlighted element to the next one.
 *  @li@c  ui4_key_prev                -Contains the key code of the key that changes the highlighted element to the previous one.
 *  @li@c  ui4_key_select              -Contains the key code of the key that selects or deselects the highlighted element.
 *  @li@c  ui4_key_page_up          -Contains the key code of the key that performs the page-up operation.
 *  @li@c  ui4_key_page_down      -Contains the key code of the key that performs the page-down operation.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_LB_KEY_MAP_T
{
    UINT32                      ui4_key_next;
    UINT32                      ui4_key_prev;
    UINT32                      ui4_key_select;
    UINT32                      ui4_key_page_up;
    UINT32                      ui4_key_page_down;
} WGL_LB_KEY_MAP_T;    

typedef UINT8   WGL_LB_EVN_HDLR_TYPE_T;    /**< Indicates the event handler type is navigation in same viewport*/
#define WGL_LB_EVN_HDLR_TYPE_NVGT_IN_VP    ((WGL_LB_EVN_HDLR_TYPE_T) 1)    /**<Indicates the event handler type is navigation in same viewport*/


 /**Indicates the event handle mask. */
#define WGL_LB_MAKE_EVN_HDLR_MASK(_e_evn_hdlr_type) MAKE_BIT_MASK_32((UINT8) (_e_evn_hdlr_type))   

 /**Indicates the NVGT_IN_VP event shall be handled in the event handler function */
#define WGL_LB_EVN_HDLR_MASK_NVGT_IN_VP             WGL_LB_MAKE_EVN_HDLR_MASK(WGL_LB_EVN_HDLR_TYPE_NVGT_IN_VP)    /**<        */

/* event handler function */
/*------------------------------------------------------------------*/
/*! @brief This is event handler function for list box.
 *  @param [in]h_lb        -The list box handle.
 *  @param [in]pv_tag     -The list box event tag.
 *  @param [in]e_evn_hdlr_type   -The list box event handle type.
 *  @param [in]ui4_data    -The enent data address.
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*wgl_lb_event_hdlr_fct) (
    HANDLE_T                    h_lb,
    VOID*                       pv_tag,
    WGL_LB_EVN_HDLR_TYPE_T      e_evn_hdlr_type,
    UINT32                      ui4_data);

/* event handler info */
/*------------------------------------------------------------------*/
/*! @struct WGL_LB_EVN_HDLR_INFO_T
 *  @brief This struct contains the event handle information.
 *  @code
 *  typedef struct _WGL_LB_EVN_HDLR_INFO_T
 *  {
 *      UINT32                      ui4_evn_hdlr_mask;   
 *      VOID*                       pv_tag_evn_hdlr;
 *      wgl_lb_event_hdlr_fct       pf_event_hdlr;       
 *  } WGL_LB_EVN_HDLR_INFO_T;
 *  @endcode
 *  @li@c  ui4_evn_hdlr_mask         -Indicates the special event shall be handled in the event handler function.
 *  @li@c  pv_tag_evn_hdlr             -Reference to the user defined tag data
 *  @li@c  pf_event_hdlr                 - Reference to the event handler function, can be NULL if ui4_evn_hdlr_mask == 0
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_LB_EVN_HDLR_INFO_T
{
    UINT32                      ui4_evn_hdlr_mask;  
    VOID*                       pv_tag_evn_hdlr;
    wgl_lb_event_hdlr_fct       pf_event_hdlr;      
} WGL_LB_EVN_HDLR_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct WGL_LB_EVN_HDLR_INFO_NVGT_IN_VP_T
 *  @brief This struct contains the enent handle data information.
 *  @code
 *  typedef struct _WGL_LB_EVN_HDLR_INFO_NVGT_IN_VP_T
 *  {
 *      UINT16                      ui2_new_hlt;       
 *      UINT16                      ui2_old_hlt;       
 *      BOOL                        b_sequential;      
 *      GL_RECT_T                   t_old_rect;        
 *      GL_RECT_T                   t_new_rect;        
 *      BOOL                        b_do_nfy_after_cb; 
 *  } WGL_LB_EVN_HDLR_INFO_NVGT_IN_VP_T;
 *  @endcode
 *  @li@c  ui2_new_hlt               - Input parameter, indicates the old hlt elem index.
 *  @li@c  ui2_old_hlt                 - Input parameter, indicates the new hlt elem index 
 *  @li@c  b_sequential               -Input parameter, indicates if the new hlt elem is just next to the old elem.
 *  @li@c  t_old_rect                   -Input parameter, contains the old hlt elem rect
 *  @li@c  t_new_rect                  -Input parameter, contains the new hlt elem rect
 *  @li@c  b_do_nfy_after_cb       - Input parameter, AP should fill b_do_nfy_after_cb for informing Listbox to do the necessary 
notifications after the callback is returned.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_LB_EVN_HDLR_INFO_NVGT_IN_VP_T
{
    UINT16                      ui2_new_hlt;       /* IN, the old hlt elem index */
    UINT16                      ui2_old_hlt;       /* IN, the new hlt elem index */
    BOOL                        b_sequential;      /* IN, indicates if the new hlt elem is just next to the old elem */
    GL_RECT_T                   t_old_rect;        /* IN, contains the old hlt elem rect */
    GL_RECT_T                   t_new_rect;        /* IN, contains the new hlt elem rect */
    
    /* AP should fill b_do_nfy_after_cb for informing Listbox to 
       do the necessary notifications after the callback is returned */
    BOOL                        b_do_nfy_after_cb; /* out */ 
    
} WGL_LB_EVN_HDLR_INFO_NVGT_IN_VP_T;



typedef VOID (*wgl_lb_cb_fct) (
    HANDLE_T                    h_lb,
    BOOL                        b_is_frm_mw_thread,
    WGL_LB_NFY_TYPE_T           e_nfy_type,
    VOID*                       pv_param1,
    VOID*                      pv_param2);

typedef struct _WGL_LB_CB_DATA_T
{
    VOID*                       pv_cb_tag;
    BOOL*                       ab_sels_buf;
    UINT16                      ui2_num_avail;
} WGL_LB_CB_DATA_T;

#define WGL_LB_ELEM_DATA_FLAG_BY_DATA             ((UINT32)1<<0)
#define WGL_LB_ELEM_DATA_FLAG_BY_CMD              ((UINT32)1<<1)
#define WGL_LB_ELEM_DATA_FLAG_ERR                 ((UINT32)1<<31)

typedef struct _WGL_LB_ELEM_DATA_INFO_T
{
    /*at_cols_fmt, ui2_elem_num, ui1_col_num are for listbox information to check if neccessory*/
    WGL_LB_COL_INIT_T*            at_cols_fmt;
    UINT16                        ui2_elem_num;
    UINT8                         ui1_col_num;

    /*This flag is for elem data information.*/
    UINT32                        ui4_flag;

    /*The get data element start and end index*/
    UINT16                        ui2_elem_start;
    UINT16                        ui2_elem_end;

    /*This is point to data information memory,it allocate by lb widget. If the ui4_flag is 
     * setted WGL_LB_ELEM_DATA_FLAG_BY_DATA,the pt_elem_data is valid(had setted by called). 
     * If ui4_flag is setted WGL_LB_ELEM_DATA_FLAG_BY_CMD,the data will set by wgl cmd*/
    WGL_LB_ELEM_DATA_T*           pt_elem_data;
} WGL_LB_ELEM_DATA_INFO_T;

typedef enum _WGL_LB_RLD_HLT_OPT_T
{
    WGL_LB_RLD_HLT_OPT_KEEP_IDX = 0, /* keep highlighting the current highlighted elem */
    
    WGL_LB_RLD_HLT_OPT_RESET_IDX,    /* highlight a new element, auto jump to the page containing the new highlight element */
    
    WGL_LB_RLD_HLT_OPT_RESET_POS     /* 1. Only valid in SCROLL mode, (i.e. valid when NOT set WGL_STL_LB_NO_SCROLL) */
                                  /* 2. same as LBW_RLD_HLT_OPT_RESET_IDX, but the position                       */
                                  /*    of the new highlight element can be re-assigned                           */
} WGL_LB_RLD_HLT_OPT_T;


/* repaint mode */
typedef enum _WGL_LB_RPNT_MODE_T 
{
    WGL_LB_RPNT_MODE_NO_RPNT = 0,
    WGL_LB_RPNT_MODE_PARTIAL_RPNT,
    WGL_LB_RPNT_MODE_FULL_RPNT
} WGL_LB_RPNT_MODE_T;

typedef struct _WGL_LB_RLD_INF_T
{
    UINT16                        ui2_total_elems;
    WGL_LB_RPNT_MODE_T            e_paint_mode;
    
    /*reload hlt information*/
    WGL_LB_RLD_HLT_OPT_T          e_rld_hlt;
    UINT16                        ui2_hlt_elem;
    UINT16                        ui2_hlt_pos;
} WGL_LB_RLD_INF_T;

typedef struct _WGL_LB_RLD_PARTIAL_T
{
    UINT16                        ui2_elem_start;
    UINT16                        ui2_elem_end;
    WGL_LB_RPNT_MODE_T            e_paint_mode;
} WGL_LB_RLD_PARTIAL_T;

#ifdef __cplusplus
}
#endif

#endif

/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_WGL_LIST_BOX*/
/*----------------------------------------------------------------------------*/


