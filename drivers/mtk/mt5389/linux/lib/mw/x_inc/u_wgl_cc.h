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
/*! @file u_wgl_cc.h
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: TC Yin $
 * $MD5HEX: d2bf2b06da1519fb224e76db92870054 $
 *
 * @par Description: 
 *         This header file contains public interfaces, constant and type 
 *         definitions of the Widget Library.
 *---------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_WGL_CC Closed Caption Widget
  *  @ingroup groupMW_WGL
  *  @brief Provides external function TV Application
  *  %MW Application API would be used by TV Application, lot of these APIs would
  *   be wrapped from Exported API
  *  @see groupMW_WGL
  *  @{ 
  */
/*----------------------------------------------------------------------------*/

     
#ifdef MW_CC_SUPPORT_CUST_1
     /*here we include the .h files in cc_c1 directory*/
#include "u_wgl_cc.h"
#else


#ifndef _U_WGL_CC_H_
#define _U_WGL_CC_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_fe.h"
#include "u_common.h"
#include "u_gl.h"
#include "u_wgl_common.h"
#include "u_wgl_cc_cnt_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define WGL_CMD_CC_UPDATE_ATTR                      WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 0)    
/**<This command updates most attributes of this closed caption widget. Those attributes are encapsulated in the structure WGL_CC_INFO_T. The default values of some attributes are also designated in this structure. */
#define WGL_CMD_CC_RESCALE                          WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 1)    
/**<This command selects a new font table and rescales this closed caption widget by attributes encapsulated in the structure WGL_CC_RESCALE_INFO_T. If the new attributes is identical to  the original attributes of the closed caption widget, this command has no effects.*/
#define WGL_CMD_CC_RESET                            WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 2)    
/**<This command resets the closed caption widget. After this command, all text and data in the widget are cleared. This command also resets all attributes to their default or initial values. */
#define WGL_CMD_CC_DESTROY                          WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 3)    
/**<This command destroys the closed caption widget. */
#define WGL_CMD_CC_SHOW                             WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 4)    
/**<This command shows or hides the closed caption widget without any display effect. For showing or hiding the widget with display effects, refer to the command WGL_CMD_CC_SHOW_WITH_EFFECT.*/
#define WGL_CMD_CC_SHOW_WITH_EFFECT                 WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 5)    
/**<This command shows or hides the closed caption widget with the current display effect. The current display effect of a closed caption widget can be specified by the WGL_CMD_CC_SET_WIN_ATTR command. Note that the display effect would only take effect when the pv_param2 argument is TRUE. For showing or hiding the widget without any display effects, refer to the command WGL_CMD_CC_SHOW. */
#define WGL_CMD_CC_RESIZE                           WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 6)    
/**<This command resizes the closed caption widget by specifying the new row count and column count. If the specified size is identical to the original size of the closed caption widget, this command has no effects.*/
#define WGL_CMD_CC_MOVE                             WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 7)    
/**<This command changes the location of the closed caption widget by specifying the new ui2_anchor_v and ui2_anchor_h. */
#define WGL_CMD_CC_UPDATE                           WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 8)    
/**<This command updates (repaints) the CC widget according to its current state. */
#define WGL_CMD_CC_SET_WIN_ATTR                     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 9)    
/**<This command sets the window attributes of this closed caption widget. The window attributes are encapsulated in the structure WGL_CC_WIN_ATTR_T. */
#define WGL_CMD_CC_GET_WIN_ATTR                     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 10)   
/**<This command gets the window attributes of this closed caption widget. The window attributes are encapsulated in the structure WGL_CC_WIN_ATTR_T. */
#define WGL_CMD_CC_SET_PEN_FONT                     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 11)   
/**<This command sets the current pen attributes of this closed caption widget. The pen attributes are encapsulated in the structure WGL_CC_PEN_FONT_T. */
#define WGL_CMD_CC_GET_PEN_FONT                     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 12)   
/**<This command gets the pen attributes of this closed caption widget. The pen attributes are encapsulated in the structure WGL_CC_PEN_FONT_T. */
#define WGL_CMD_CC_SET_PEN_COLOR                    WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 13)   
/**<This command sets the current pen color attributes of this closed caption widget. The pen color attributes are encapsulated in the structure WGL_CC_PEN_COLOR_T. */
#define WGL_CMD_CC_GET_PEN_COLOR                    WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 14)   
/**<This command gets the pen color attributes of this closed caption widget. The pen color attributes are encapsulated in the structure WGL_CC_PEN_COLOR_T. */
#define WGL_CMD_CC_SET_PEN_PARAG_LOCATION           WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 15)   
/**<
This command sets the pen location of this closed caption widget. The pen location is determined by the index of the current paragraph and the character index in this paragraph. 

If the closed caption widget has WGL_CC_WIN_ATTR_LIMIT_ROW attribute and the index of the paragraph of the pen location is not less than the row count of this widget, this command has no effects.

If the closed caption widget has WGL_CC_WIN_ATTR_LIMIT_COL attribute and the pen target position is greater than the column count of this widget, the pen will be located at the end of the paragraph first row.

The pen moving modes can be specified in the argument pv_param2. For example, if the pen-moving mode is set as WGL_CC_MOVE_MODE_FILL_TRANSP_SPACE and the new pen location is out of the range of the specified paragraph, necessary stuffing transparent spaces will be automatically padded.
*/
#define WGL_CMD_CC_SET_PEN_VISIBLE_PARAG_LOCATION   WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 16)   
/**<
This command sets the pen location relative to the visible paragraphs of this closed caption widget. The location of pen is determined by the index of the paragraph and the character index in this paragraph. Note that the index of the paragraph is relative to the first visible paragraph in the closed caption widget's viewport. 

If the closed caption widget has WGL_CC_WIN_ATTR_LIMIT_ROW attribute and the index of the paragraph of the pen location is not less than the row count of this widget, this command has no effects.

If the closed caption widget has WGL_CC_WIN_ATTR_LIMIT_COL attribute and the pen target position is greater than the column count of this widget, the pen will be located at the end of the paragraph first row.

The pen moving modes can be specified in the argument pv_param2. For example, if the pen-moving mode is set as WGL_CC_MOVE_MODE_FILL_TRANSP_SPACE and the new pen location is out of the range of the specified paragraph, necessary stuffing transparent spaces will be automatically padded.
*/
#define WGL_CMD_CC_GET_PEN_PARAG_LOCATION           WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 17)   
/**<This command gets the current pen location of this closed caption widget. */
#define WGL_CMD_CC_MOVE_PEN_RIGHT                   WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 18)   
/**<
This command moves the current pen location right with the specified columns.

If the closed caption widget has WGL_CC_WIN_ATTR_LIMIT_COL attribute and the pen target position is greater than the column count of this widget, the pen will be moved to the end of current row.

Specific pen moving mode can be specified in the argument pv_param2. 
*/
#define WGL_CMD_CC_MOVE_PEN_LEFT                    WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 19)   
/**<This command moves the current pen location left with the specified columns. Specific pen moving mode can be specified in the argument pv_param2.*/
#define WGL_CMD_CC_MOVE_PEN_TO_COL                  WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 20)   
/**<
This command moves the current pen location to the specified column in current paragraph. 
If the closed caption widget has WGL_CC_WIN_ATTR_LIMIT_COL attribute and the pen target position is greater than the column count of this widget, the pen will be moved to the end of current row.

Specific pen moving mode can be specified in the argument pv_param2. 
*/
#define WGL_CMD_CC_MOVE_PEN_TO_NEXT_PARAG           WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 21)   
/**<This command moves the current pen location to the first position of the next paragraph. If the current pen location is at the last paragraph of the whole article, a new empty paragraph is automatically created and appended to the end of the article, and the pen location is moved to the first position of the new created paragraph. Depending on the BOOL value specified in the parameter pv_param1, this command rolls up one row of the article. */
#define WGL_CMD_CC_OVERRIDE_ATTR                    WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 22)   
/**<This command sets the global overriding attributes. By default, the global overriding attributes have no effect upon the attributes used by all sections. But whenever any of the global overriding attributes is set, the related attribute of each existing section will be overridden as the global overriding attribute. The related attribute of the pen will also be overridden. Thus, the attributes of all the text written later will also be replaced by the global overriding attributes no matter what their original attributes are. Multiple WGL_CMD_CC_OVERRIDE_ATTR commands sent to CC widget early or late would be accumulated and or-ed together. If pv_param1 is NULL, the global overriding attributes returns back to the original one and all existing texts are re-layout-ed and redrawn according to their own original attributes. The pen attributes also return back to its original setting. For undoing overriding individual attribute independently, refer to the command WGL_CMD_CC_UNDO_OVERRIDE_ATTR.*/
#define WGL_CMD_CC_UNDO_OVERRIDE_ATTR               WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 23)   
/**<This command undoes overriding the global overriding attributes. For more information about attributes overriding, refer to the command WGL_CMD_CC_OVERRIDE_ATTR.*/
#define WGL_CMD_CC_SET_INDENT_SIZE                  WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 24)   
/**<This command sets the indent size in character count. To disable the indent after a CR, set the indent size as zero.*/
#define WGL_CMD_CC_GET_INDENT_SIZE                  WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 25)   
/**<This command gets the indent size in character count. */
#define WGL_CMD_CC_SET_TAB_SIZE                     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 26)   
/**<This command sets the tab size in character count. */
#define WGL_CMD_CC_GET_TAB_SIZE                     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 27)   
/**<This command gets the tab size in character count. */
#define WGL_CMD_CC_SET_SMOOTH_SCROLL_TIME_BOUND     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 28)   
/**<This command sets the time bound for the widget to scroll a row when performing the smooth scrolling effect. The widget must finish scrolling a row before the specified time bound expires. Note that this command is valid only when the smooth scrolling flag is set as TRUE.*/
#define WGL_CMD_CC_GET_SMOOTH_SCROLL_TIME_BOUND     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 29)   
/**<This command gets the time bound for the widget to scroll a row when performing the smooth scrolling effect. The widget will finish scrolling a row before the time bound expires. Note that this command is valid only when the smooth scrolling flag is set as TRUE.*/
#define WGL_CMD_CC_SET_TYPEWRITER_EFFECT_TIME_BOUND WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 30)   
/**<This command sets the time bound before that the widget has to finish drawing a character when performing the typewriter effect. Note that this command is valid only when the typewriter effect flag is set as TRUE.*/
#define WGL_CMD_CC_GET_TYPEWRITER_EFFECT_TIME_BOUND WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 31)   
/**<This command gets the time bound before that the widget has to finish drawing a character when performing the typewriter effect. Note that this command is valid only when the typewriter effect flag is set as TRUE.*/
#define WGL_CMD_CC_START_FLASH                      WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 32)   
/**<This command starts the flashing effect for the flashing objects. By default, Closed Caption automatically starts flashing whenever there is flashing object.*/
#define WGL_CMD_CC_STOP_FLASH                       WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 33)   
/**<This command stops the flashing effect for the flashing objects. */
#define WGL_CMD_CC_SET_FLASH_INTERVAL               WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 34)   
/**<This command sets the interval for the widget to perform the flashing effect for flash objects. Note that the flashing interval must be larger than or equal to WGL_CC_MIN_FLASH_INTERVAL.*/
#define WGL_CMD_CC_GET_FLASH_INTERVAL               WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 35)   
/**<This command gets the interval for the widget to perform the flashing effect for flash objects. */
#define WGL_CMD_CC_WRITE_TEXT                       WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 36)   
/**<This command writes a serial of UTF16_T characters at the current pen location. The attributes of the written text are derived from the current pen attributes. After the text is written, the pen location will be moved to the end of the written text. Note that if the Closed Caption widget needs to perform the typewriter effect, the command will not return until each character of the specified text is written. In other words, the typewriter effect is performed in the caller thread's context in a synchronized way. Besides, if the Closed Caption widget needs to perform the smooth scrolling effect and if the written text causes an automatic scrolling action, the smooth scrolling effect is also performed in the caller thread's context. */
#define WGL_CMD_CC_WRITE_TEXT_UPDATE                WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 37)   
/**<This command writes a serial of UTF16_T characters at the current pen location and automatically redraws the CC widget. Except for the automatically redrawing function, this command results in the same effect as the command WGL_CMD_CC_WRITE_TEXT.*/
#define WGL_CMD_CC_WRITE_TAB                        WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 38)   
/**<This command writes a TAB at the current pen location. Several tab modes are available and can be specified in the first argument pv_param1.*/
#define WGL_CMD_CC_INSERT_CR                        WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 39)   
/**<This command writes a Carriage Return at the current pen location. */
#define WGL_CMD_CC_INSERT_EMPTY_PARAGS              WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 40)   
/**<This command inserts a number of empty paragraphs before a specified paragraph. */
#define WGL_CMD_CC_CLEAR_TEXT                       WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 41)   
/**<This command clears all text currently in the Closed Caption window.*/
#define WGL_CMD_CC_BACKSPACE_KEEP_IN_PARAG          WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 42)   
/**<This command deletes a character immediately before the current pen location. If the pen location is at beginning of a row, this command has no effect. Several backspace modes are available and can be specified in the first argument pv_param1.*/
#define WGL_CMD_CC_BACKSPACE                        WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 43)   
/**<This command deletes a character immediately before the current pen location. If the character to be deleted is a control code such as CR, TWB and TAB, the control code will also be deleted and the text content will be re-layout-ed. */
#define WGL_CMD_CC_BACKSPACE_SKIP_EMPTY_PARAG       WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 44)   
/**<This command deletes a character immediately before the current pen location. If the pen location is at beginning of a paragraph, the pen will be moved upwardly to the end of the first found no-empty paragraph. If the character to be deleted is a control code such as CR, TWB and TAB, the control code will also be deleted and the text content will be re-layout-ed. */
#define WGL_CMD_CC_DEL_PARAGRAPH                    WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 45)   
/**<This command deletes the paragraph with the specified paragraph-index.*/
#define WGL_CMD_CC_CLEAR_CURRENT_PARAG              WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 46)   
/**<This command clears all the text in the current paragraph. The paragraph becomes an empty paragraph containing only a CR or a TWB.*/
#define WGL_CMD_CC_DEL_ROW                          WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 47)   
/**<This command deletes the row with the specified row-index.*/
#define WGL_CMD_CC_CLEAR_ROW                        WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 48)   
/**<This command clears all the text in a row with the specified row-index. If there is a CR or a TWB at the end of the specified row, this command doesn't clear either of them. In this case, the row becomes an empty row containing only a CR or a TWB. If the specified row doesn't contain a CR or a TWB at the end, the text originally located at that row is cleared and the text after that row is automatically rolled up to replace original text in the specified row.*/
#define WGL_CMD_CC_DEL_ALL_INVISIBLE_PARAG          WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 49)   
/**<This command deletes all invisible paragraphs above the viewport. */
#define WGL_CMD_CC_CLEAR_TO_EOP                     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 50)   
/**<This command clears all characters ranging from current pen location till the end of the current paragraph. Note that the character that locates at the current pen location will also be deleted.*/
#define WGL_CMD_CC_ROLL_UP                          WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 51)   
/**<This command rolls up the next row immediately below the last visible row. If there is no row below the last visible row, this command has no effect.*/
#define WGL_CMD_CC_ENABLE_SMOOTH_SCROLL             WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 52)   
/**<This command enables or disables the smooth scrolling effect. Note that this command has higher priority than other commands. In other words, if this command is sent when there are other commands in command queue, this command will be executed earlier than other commands.*/
#define WGL_CMD_CC_SET_NTSC_MODE                    WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 53)   
/**<This command sets one of the four NTSC modes.*/
#define WGL_CMD_CC_GET_PARAGRAPH_NUM                WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 54)   
/**<This command gets the number of paragraphs currently contained by the Closed Caption window.*/
#define WGL_CMD_CC_GET_ROW_NUM                      WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 55)   
/**<This command gets the number of rows currently contained by the Closed Caption window.*/
#define WGL_CMD_CC_GET_ROW_LENGTH                   WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 56)   
/**<This command gets the number of character contained by the row with the specified row-index.*/
#define WGL_CMD_CC_GET_FIRST_VISIBLE_ROW_INDEX      WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 57)   
/**<This command gets the index of the first visible row in the Closed Caption window.*/
#define WGL_CMD_CC_GET_LAST_VISIBLE_ROW_INDEX       WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 58)   
/**<This command gets the index of the last visible row in the Closed Caption window.*/
#define WGL_CMD_CC_CORE_CLEAR_QUEUE                 WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_CC, 59)   
/**<This command clears the exist messages in the cc core message queue. */

/*------ Specific Notify Code -------*/
#define WGL_NC_CC_RESIZED                   ((UINT32)WGL_NC_WIDGET_SPF_START + 0)   
/**<This notification is arisen when the Closed Caption window is resized. */
#define WGL_NC_CC_MOVED                     ((UINT32)WGL_NC_WIDGET_SPF_START + 1)   
/**<This notification is arisen when the Closed Caption window is moved. */
#define WGL_NC_CC_SCROLLED                  ((UINT32)WGL_NC_WIDGET_SPF_START + 2)   
/**<This notification is arisen when the rows in Closed Caption window is scrolled up or scrolled down. */
#define WGL_NC_CC_CMD_FINISHED              ((UINT32)WGL_NC_WIDGET_SPF_START + 3)   
/**<This notification is arisen when a command is finished */
#define WGL_NC_CC_CORE_QUEUE_FULL           ((UINT32)WGL_NC_WIDGET_SPF_START + 4)   
/**<This notification is arisen when there are more messages then allowed number in the message queue. */

/*------ Attibutes Control Code -------*/
#define WGL_CC_ATTR_CODE_START              ((UTF16_T) ((FE_UTF16_SPACE_USER_START + 11) & ((UTF16_T) 0xFFFE)))
/**<The definition of closed caption attribute control code start value*/


/* color index */
typedef UINT16 WGL_CC_COLOR_IDX_T;  
/**<This type is used to define the color index of some predefine color.*/
 
#define WGL_CC_COLOR_IDX_WHITE          ((WGL_CC_COLOR_IDX_T) 0)    
/**<Indicates the white color index.*/
#define WGL_CC_COLOR_IDX_GREEN          ((WGL_CC_COLOR_IDX_T) 1)    
/**<Indicates the green color index.*/
#define WGL_CC_COLOR_IDX_BLUE           ((WGL_CC_COLOR_IDX_T) 2)    
/**<Indicates the blue color index.*/
#define WGL_CC_COLOR_IDX_CYAN           ((WGL_CC_COLOR_IDX_T) 3)    
/**<Indicates the cyan color index.*/
#define WGL_CC_COLOR_IDX_RED            ((WGL_CC_COLOR_IDX_T) 4)    
/**<Indicates the red color index.*/
#define WGL_CC_COLOR_IDX_YELLOW         ((WGL_CC_COLOR_IDX_T) 5)    
/**<Indicates the yellow color index.*/
#define WGL_CC_COLOR_IDX_MAGENTA        ((WGL_CC_COLOR_IDX_T) 6)    
/**<Indicates the magenta color index.*/
#define WGL_CC_COLOR_IDX_BLACK          ((WGL_CC_COLOR_IDX_T) 7)    
/**<Indicates the black color index.*/
#define WGL_CC_COLOR_IDX_TRANSP         ((WGL_CC_COLOR_IDX_T) 8)    
/**<Indicates the transparent color index.*/
#define WGL_CC_COLOR_IDX_KEEP           ((WGL_CC_COLOR_IDX_T) 9)    
/**<Indicates the keeping color index.*/


/* Foreground Control Code - Code Definition */
#define WGL_CC_FCC_CODE_START           ((UTF16_T) (WGL_CC_ATTR_CODE_START))        
/**<The definition of closed caption foreground control code start value*/
#define WGL_CC_FCC_CODE_END             ((UTF16_T) (WGL_CC_FCC_CODE_START + 21))    
/**<The definition of closed caption foreground control code end value*/


/* Supported color index: WGL_CC_COLOR_IDX_WHITE ~ WGL_CC_COLOR_IDX_BLACK */


#define WGL_CC_MAKE_FCC_CHAR(_e_color_idx, _b_underline)  \
            ((UTF16_T) (WGL_CC_FCC_CODE_START + (((UTF16_T) (_e_color_idx)) * 2) + ((UTF16_T) ((_b_underline) & 0x01))))
/**<The definition of make fcc char*/


#define WGL_CC_FCC_RESERVED_1           WGL_CC_MAKE_FCC_CHAR (WGL_CC_COLOR_IDX_TRANSP, FALSE)
/**<The definition of fcc reserved_1 char */


#define WGL_CC_FCC_RESERVED_2           WGL_CC_MAKE_FCC_CHAR (WGL_CC_COLOR_IDX_TRANSP, TRUE)
/**<The definition of fcc reserved_2 char */


#define WGL_CC_FCC_KEEP                 WGL_CC_MAKE_FCC_CHAR (WGL_CC_COLOR_IDX_KEEP, FALSE)
/**<The definition of fcc keep char */


#define WGL_CC_FCC_KEEP_UNDERLINE       WGL_CC_MAKE_FCC_CHAR (WGL_CC_COLOR_IDX_KEEP, TRUE)
/**<The definition of fcc underline char */


#define WGL_CC_FCC_ITALICS              ((UTF16_T) (WGL_CC_FCC_CODE_START + ((UTF16_T) 20)))
/**<The definition of fcc italics char */


#define WGL_CC_FCC_ITALICS_UNDERLINE    ((UTF16_T) (WGL_CC_FCC_CODE_START + ((UTF16_T) 20) + ((UTF16_T) TRUE)))
/**<The definition of fcc italics & underline  char */



/* Foreground Control Code - Macros */
#define WGL_CC_IS_FCC_CHAR(_w2_ch)              ((_w2_ch) >= WGL_CC_FCC_CODE_START && (_w2_ch) <= WGL_CC_FCC_CODE_END)
/**<The judegment of the char is fcc one or not*/


#define WGL_CC_FCC_GET_COLOR_IDX(_w2_ch)        ((WGL_CC_COLOR_IDX_T) (((_w2_ch) - WGL_CC_FCC_CODE_START) / 2))
/**<The prototype to get color index */

#define WGL_CC_IS_FCC_WITH_COLOR(_w2_ch)        (((INT32) WGL_CC_FCC_GET_COLOR_IDX(_w2_ch)) >= ((INT32) WGL_CC_COLOR_IDX_WHITE) && WGL_CC_FCC_GET_COLOR_IDX(_w2_ch) <= WGL_CC_COLOR_IDX_BLACK)
/**<The judegment of foreground control code is with color or not*/


#define WGL_CC_IS_FCC_ITALIC_ON(_w2_ch)         (((_w2_ch) == WGL_CC_FCC_ITALICS) || ((_w2_ch) == WGL_CC_FCC_ITALICS_UNDERLINE))
/**<The judegment of foreground control code is italic or not*/


#define WGL_CC_IS_FCC_UNDERLINE_ON(_w2_ch)      ((_w2_ch) & ((UTF16_T) 0x0001))
/**<The judegment of foreground control code is underline or not*/



/* Background Control Code - Code Definition */
#define WGL_CC_BCC_CODE_START                   ((UTF16_T) (WGL_CC_ATTR_CODE_START + ((UTF16_T) 24)))
/**<The definition of closed caption background control code start value*/

#define WGL_CC_BCC_CODE_END                     ((UTF16_T) (WGL_CC_BCC_CODE_START + ((UTF16_T) 16)))
/**<The definition of closed caption background control code end value*/


/* Supported color index: WGL_CC_COLOR_IDX_WHITE ~ WGL_CC_COLOR_IDX_BLACK */
#define WGL_CC_MAKE_BCC_CHAR(_e_color_idx, _b_semi_transp)  \
            ((UTF16_T) (WGL_CC_BCC_CODE_START + (((UTF16_T) (_e_color_idx)) * 2) + ((UTF16_T) ((_b_semi_transp) & 0x01))))
/**<The definition of make bcc char*/


#define WGL_CC_BCC_TRANSP_OPAQUE                WGL_CC_MAKE_BCC_CHAR (WGL_CC_COLOR_IDX_TRANSP, FALSE)
/**<The definition of bcc transparent opaque char */



/* Background Control Code - Macros */
#define WGL_CC_IS_BCC_CHAR(_w2_ch)              ((_w2_ch) >= WGL_CC_BCC_CODE_START && (_w2_ch) <= WGL_CC_BCC_CODE_END)
/**<The judegment of the char is bcc one or not*/


#define WGL_CC_BCC_GET_COLOR_IDX(_w2_ch)        ((WGL_CC_COLOR_IDX_T) ((_w2_ch) - WGL_CC_BCC_CODE_START))
/**<The prototype to get bcc char color index*/


/* Miscellaneous Control Code - Code Definition */
#define WGL_CC_MCC_CODE_START                   ((UTF16_T) (WGL_CC_ATTR_CODE_START + ((UTF16_T) 42)))
/**<The definition of closed caption miscellaneous control code start value*/

#define WGL_CC_MCC_CODE_END                     ((UTF16_T) (WGL_CC_MCC_CODE_START + ((UTF16_T) 1)))
/**<The definition of closed caption miscellaneous control code end value*/

#define WGL_CC_MCC_FLASH_ON                     ((UTF16_T) ((WGL_CC_MCC_CODE_START + ((UTF16_T) 0))))
/**<The mcc char of falsh*/

#define WLG_CC_IS_MCC_CHAR(_w2_ch)              ((_w2_ch) == WGL_CC_MCC_FLASH_ON)
/**<The judegment of the char is mcc one or not*/


/* Macros for all codes */
#define WGL_CC_IS_ATTR_CODE(_w2_ch)             (WGL_CC_IS_FCC_CHAR (_w2_ch) ||     \
                                                 WGL_CC_IS_BCC_CHAR (_w2_ch) ||     \
                                                 WLG_CC_IS_MCC_CHAR (_w2_ch) )
/**<The judegment of the char is attribute code*/




/*------ Anchor Point -------*/
#define WGL_CC_ANCHOR_POINT_0   ((UINT8)0)      
/**<Indicates the anchor point 0.*/
#define WGL_CC_ANCHOR_POINT_1   ((UINT8)1)      
/**<Indicates the anchor point 1.*/
#define WGL_CC_ANCHOR_POINT_2   ((UINT8)2)      
/**<Indicates the anchor point 2.*/
#define WGL_CC_ANCHOR_POINT_3   ((UINT8)3)      
/**<Indicates the anchor point 3.*/
#define WGL_CC_ANCHOR_POINT_4   ((UINT8)4)      
/**<Indicates the anchor point 4.*/
#define WGL_CC_ANCHOR_POINT_5   ((UINT8)5)      
/**<Indicates the anchor point 5.*/
#define WGL_CC_ANCHOR_POINT_6   ((UINT8)6)      
/**<Indicates the anchor point 6.*/
#define WGL_CC_ANCHOR_POINT_7   ((UINT8)7)      
/**<Indicates the anchor point 7.*/
#define WGL_CC_ANCHOR_POINT_8   ((UINT8)8)      
/**<Indicates the anchor point 8.*/

/*------ Tab Writing Mode -------*/
#define WGL_CC_TAB_MODE_OVERRIDE                        ((UINT8)1)  
/**<Used with WGL_CMD_CC_WRITE_TAB command to write a TAB character at the current pen location. The original character would be overridden by this TAB character.*/
#define WGL_CC_TAB_MODE_MOVE_PEN_FILL_SPACE             ((UINT8)2)  
/**<Used with WGL_CMD_CC_WRITE_TAB command, a TAB is interpreted as pen moving. If the end of article is reached but the target location is not reached yet, necessary "space" characters will be automatically inserted.*/
#define WGL_CC_TAB_MODE_MOVE_PEN_FILL_TRANSP_SPACE      ((UINT8)3)  
/**<Used with WGL_CMD_CC_WRITE_TAB command, a TAB is interpreted as pen moving. If the end of article is reached but the target location is not reached yet, necessary "transparent space" characters will be automatically inserted.*/
#define WGL_CC_TAB_MODE_MOVE_PEN_FILL_NULL_SPACE        ((UINT8)4)  
/**<Used with WGL_CMD_CC_WRITE_TAB command, a TAB is interpreted as pen moving. If the end of article is reached but the target location is not reached yet, necessary "null space" characters will be automatically inserted.*/

/*------ Macros -------*/
#define WGL_CC_MIN_TRANSC_ID            ((UINT16)WGLR_USER)
/**<The definition of min transc id*/

#define WGL_CC_MAX_TRANSC_ID            ((UINT16)0xFFFF)
/**<The definition of max transc id*/


#define WGL_CC_IS_TRANSC_ID(_i4_ret)                                        \
            ((((INT32)(_i4_ret)) >= ((INT32)(WGL_CC_MIN_TRANSC_ID)))        \
                && (((INT32)(_i4_ret)) <= ((INT32)(WGL_CC_MAX_TRANSC_ID)) ) )
/**<The judgement of the return value if transc id or not*/


#define WGL_CC_MAKE_FONT_TBL_ID(_ui2_key_w, _ui2_key_h)                     \
            ((((UINT32)((UINT16)(_ui2_key_w)))<<16) | ((UINT32)((UINT16)(_ui2_key_h))))
/**<The prototype to generate the font table id*/

/* constants */

#define WGL_CC_MIN_FLASH_INTERVAL           ((UINT32)100)
/**<The definition of closed caption minimize flash interval*/


#define END_OF_ARRAY_FONT_REG_INFO_RES_PARAM_T                      \
    {                                                               \
        0,                                                          \
        {                                                           \
            0, 0, 0                                                 \
        },                                                          \
        {                                                           \
            0, 0, 0                                                 \
        },                                                          \
        {                                                           \
            0, 0, 0                                                 \
        }                                                           \
    }
/**<The definition of the end variable of the resolution of font register information array*/

#define END_OF_ARRAY_FONT_REG_INFO_T                                \
    {                                                               \
        "",                                                         \
        NULL                                                        \
    }
/**<The definition of the end fo font register information array*/


/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
#define WGL_CC_EDGE_TYPE_NONE               ((UINT8)0)      
/**<Indicates the no text edge.*/
#define WGL_CC_EDGE_TYPE_RAISED             ((UINT8)1)      
/**<Indicates the raised text edge.*/
#define WGL_CC_EDGE_TYPE_DEPRESSED          ((UINT8)2)      
/**<Indicates the depressed text edge.*/
#define WGL_CC_EDGE_TYPE_UNIFORM            ((UINT8)3)      
/**<Indicates the uniform text edge.*/
#define WGL_CC_EDGE_TYPE_LEFT_DROP_SHADOW   ((UINT8)4)      
/**<Indicates the left-drop-shadow text edge.*/
#define WGL_CC_EDGE_TYPE_RIGHT_DROP_SHADOW  ((UINT8)5)      
/**<Indicates the right-drop-shadow text edge.*/
#define WGL_CC_EDGE_TYPE_OUTLINE            ((UINT8)6)      
/**<Indicates the outlined text edge.*/

#define WGL_CC_EDGE_TYPE_LAST               ((UINT8)6)      
/**<Reserved. Indicates the last text edge type.*/



/*------------------------------------------------------------------*/
/*! @brief     Indicates the font style of closed caption.
  *  @code 
  *  typedef enum _WGL_CC_FNT_STYLE_T
  *  { 
  *      WGL_CC_FNT_STYLE_REGULAR     =   FE_FNT_STYLE_REGULAR,
  *      WGL_CC_FNT_STYLE_ITALIC        =   FE_FNT_STYLE_ITALIC,
  *      WGL_CC_FNT_STYLE_BOLD          =   FE_FNT_STYLE_BOLD, 
  *      WGL_CC_FNT_STYLE_UNDERLINE  =   FE_FNT_STYLE_UNDERLINE,
  *      WGL_CC_FNT_STYLE_STRIKEOUT  =   FE_FNT_STYLE_STRIKEOUT
  *  } WGL_CC_FNT_STYLE_T; 
  *  @endcode 
  *  @li@c  WGL_CC_FNT_STYLE_REGULAR       -Indicated regular font style.
  *  @li@c  WGL_CC_FNT_STYLE_ITALIC          -Indicated italic font style.
  *  @li@c  WGL_CC_FNT_STYLE_BOLD             -Indicated bold font style.
  *  @li@c  WGL_CC_FNT_STYLE_UNDERLINE    -Indicated underline font style.
  *  @li@c  WGL_CC_FNT_STYLE_STRIKEOUT    -Indicated strikeout font style.
  */
/*------------------------------------------------------------------*/
typedef enum _WGL_CC_FNT_STYLE_T
{
    WGL_CC_FNT_STYLE_REGULAR        =   FE_FNT_STYLE_REGULAR,
    WGL_CC_FNT_STYLE_ITALIC         =   FE_FNT_STYLE_ITALIC,
    WGL_CC_FNT_STYLE_BOLD           =   FE_FNT_STYLE_BOLD,
    WGL_CC_FNT_STYLE_UNDERLINE      =   FE_FNT_STYLE_UNDERLINE,
    WGL_CC_FNT_STYLE_STRIKEOUT      =   FE_FNT_STYLE_STRIKEOUT
} WGL_CC_FNT_STYLE_T;


/*------------------------------------------------------------------*/
/*! @brief the structure of closed caption font information.
  *  @code 
  *  typedef struct _WGL_CC_FONT_INFO_T 
  *  { 
  *      CHAR                               s_font_name[WGL_MAX_FONT_NAME];
  *      FE_FNT_SIZE                    e_size;
  *      WGL_CC_FNT_STYLE_T     e_style;
  *      FE_CMAP_ENCODING        e_cmap;
  *      UINT8                              ui1_edge_type;
  *  } WGL_CC_FONT_INFO_T; 
  *  @endcode 
  *  @li@c  s_font_name         -Reference to the font name string. 
  *  @li@c  e_size                  -Reference to the font size infomation. 
  *  @li@c  e_style                 -Reference to the font style information.
  *  @li@c  e_cmap                -Reference to cmap infomation. 
  *  @li@c  ui1_edge_type      -Reference to the font edge type information.
  */
/*------------------------------------------------------------------*/
typedef struct _WGL_CC_FONT_INFO_T
{
    CHAR                        s_font_name[WGL_MAX_FONT_NAME];
    FE_FNT_SIZE                 e_size;
    WGL_CC_FNT_STYLE_T          e_style;
    FE_CMAP_ENCODING            e_cmap;
    UINT8                       ui1_edge_type;
    
} WGL_CC_FONT_INFO_T;


#define WGL_CC_JUSTIFY_LEFT                 ((UINT8)0)      
/**<Indicates the left justification.*/
#define WGL_CC_JUSTIFY_RIGHT                ((UINT8)1)      
/**<Indicates the right justification.*/
#define WGL_CC_JUSTIFY_CENTER               ((UINT8)2)      
/**<Indicates the center justification.*/
#define WGL_CC_JUSTIFY_FULL                 ((UINT8)3)      
/**<Indicates the full justification.*/

#define WGL_CC_PRINT_DIR_LEFT_TO_RIGHT      ((UINT8)0)      
/**<Indicates the left to right print direction.*/
#define WGL_CC_PRINT_DIR_RIGHT_TO_LEFT      ((UINT8)1)      
/**<Indicates the right to left print direction.*/
#define WGL_CC_PRINT_DIR_TOP_TO_BOTTOM      ((UINT8)2)      
/**<Indicates the top to bottom print direction.*/
#define WGL_CC_PRINT_DIR_BOTTOM_TO_TOP      ((UINT8)3)      
/**<Indicates the bottom to top print direction.*/

#define WGL_CC_SCROLL_DIR_LEFT_TO_RIGHT     ((UINT8)0)      
/**<Indicates the left to right scrolling direction.*/
#define WGL_CC_SCROLL_DIR_RIGHT_TO_LEFT     ((UINT8)1)      
/**<Indicates the right to left scrolling direction.*/
#define WGL_CC_SCROLL_DIR_TOP_TO_BOTTOM     ((UINT8)2)      
/**<Indicates the top to bottom scrolling direction.*/
#define WGL_CC_SCROLL_DIR_BOTTOM_TO_TOP     ((UINT8)3)      
/**<Indicates the bottom to top scrolling direction.*/

#define WGL_CC_DISPLAY_EFFECT_SNAP          ((UINT8)0)      
/**<Indicates the snaps display effect.*/
#define WGL_CC_DISPLAY_EFFECT_FADE          ((UINT8)1)      
/**<Indicates the fades display effect.*/
#define WGL_CC_DISPLAY_EFFECT_WIPE          ((UINT8)2)      
/**<Indicates the wipes display effect.*/

#define WGL_CC_DISPLAY_EFFECT_DIR_LEFT_TO_RIGHT     ((UINT8)0)  
/**<Indicates the left to right display effect direction.*/
#define WGL_CC_DISPLAY_EFFECT_DIR_RIGHT_TO_LEFT     ((UINT8)1)  
/**<Indicates the left to right display effect direction.*/
#define WGL_CC_DISPLAY_EFFECT_DIR_TOP_TO_BOTTOM     ((UINT8)2)  
/**<Indicates the top to bottom display effect direction.*/
#define WGL_CC_DISPLAY_EFFECT_DIR_BOTTOM_TO_TOP     ((UINT8)3)  
/**<Indicates the bottom to top display effect direction.*/

#define WGL_CC_BORDER_TYPE_NONE                     ((UINT8)0)  
/**<Indicates the no window border.*/
#define WGL_CC_BORDER_TYPE_RAISED                   ((UINT8)1)  
/**<Indicates the raised window border.*/
#define WGL_CC_BORDER_TYPE_DEPRESSED                ((UINT8)2)  
/**<Indicates the depressed window border.*/
#define WGL_CC_BORDER_TYPE_UNIFORM                  ((UINT8)3)  
/**<Indicates the uniform window border.*/
#define WGL_CC_BORDER_TYPE_LEFT_DROP_SHADOW         ((UINT8)4)  
/**<Indicates the left-drop-shadow window border.*/
#define WGL_CC_BORDER_TYPE_RIGHT_DROP_SHADOW        ((UINT8)5)  
/**<Indicates the right-drop-shadow window border.*/

#define WGL_CC_WIN_ATTR_WORD_WRAP                   ((UINT16) 0) /* ==> deprecates "wordwrap" attribute according to CEA-708-C, 8.4.8 */
/**<
If this flag is set, the word wrap mode is turned on; otherwise the word wrap mode is turned off.
It is deprecated according to the DTVCC spec.
*/

#define WGL_CC_WIN_ATTR_LIMIT_ROW                   (((UINT16)1)<<1)
/**<If this flag is set, the Closed Caption window limits the pen location not beyond the maximum row of the window.*/

#define WGL_CC_WIN_ATTR_LIMIT_COL                   (((UINT16)1)<<2)
/**<If this flag is set, the Closed Caption window limits the pen location not beyond the maximum column of a row.*/

#define WGL_CC_WIN_ATTR_AUTO_SCROLL                 (((UINT16)1)<<3)
/**<If this flag is set, the Closed Caption window automatically scroll up a new row to the visible area of the window.*/

#define WGL_CC_WIN_ATTR_SMOOTH_SCROLL               (((UINT16)1)<<4)
/**<If this flag is set, the Closed Caption window performs smoothly scrolling effect when scrolling rows in the visible area.*/

#define WGL_CC_WIN_ATTR_TYPEWRITER_EFFECT           (((UINT16)1)<<5)
/**<If this flag is set, the Closed Caption window performs typewriter effect when displaying newly coming text.*/

#define WGL_CC_WIN_ATTR_LAST_FLAG                   WGL_CC_WIN_ATTR_TYPEWRITER_EFFECT
/**<Reserved. Indicates the last window attribute flag.*/


/*------------------------------------------------------------------*/
/*! @brief the structure of closed caption window attribute.
  *  @code 
  *  typedef struct _WGL_CC_WIN_ATTR_T 
  *  { 
  *     UINT16              ui2_win_attr;
  *     UINT8               ui1_justify;
  *     UINT8               ui1_print_dir;
  *     UINT8               ui1_scroll_dir;
  *     UINT8               ui1_display_effect;
  *     UINT8               ui1_display_effect_dir;
  *     UINT32              ui4_display_effect_duration;
    
  *     GL_COLOR_T      t_color_bk;
  *     BOOL                b_is_flash_bk;
  *     UINT8               ui1_border_type;
  *     GL_COLOR_T      t_color_border;
  *  } WGL_CC_WIN_ATTR_T; 
  *  @endcode 
  *  @li@c  ui2_win_attr                        -Reference to the window attribute. 
  *  @li@c  ui1_justify                          -Reference to the justify of caption. 
  *  @li@c  ui1_print_dir                       -Reference to the print direction of caption.
  *  @li@c  ui1_scroll_dir                      -Reference to the scroll direction of caption. 
  *  @li@c  ui1_display_effect               -Reference to the dispaly effect of caption.
  *  @li@c  ui1_display_effect_dir          -Reference to the display effect direction of caption.
  *  @li@c  ui4_display_effect_duration  -Reference to the display effect duration of caption. 
  *  @li@c  t_color_bk                          -Reference to the background color. 
  *  @li@c  b_is_flash_bk                     -Reference to the background flash attribute. 
  *  @li@c  ui1_border_type                 -Reference to the window border type. 
  *  @li@c  t_color_border                    -Reference to the window border color.   
  */
/*------------------------------------------------------------------*/
typedef struct _WGL_CC_WIN_ATTR_T
{
    UINT16      ui2_win_attr;
    UINT8       ui1_justify;
    UINT8       ui1_print_dir;
    UINT8       ui1_scroll_dir;
    UINT8       ui1_display_effect;
    UINT8       ui1_display_effect_dir;
    UINT32      ui4_display_effect_duration;
    
    GL_COLOR_T  t_color_bk;
    BOOL        b_is_flash_bk;
    UINT8       ui1_border_type;
    GL_COLOR_T  t_color_border;
    
} WGL_CC_WIN_ATTR_T;


    
#define WGL_CC_TEXT_OFFSET_NORMAL           ((UINT8)0)  
/**<Indicates the no text offset.*/
#define WGL_CC_TEXT_OFFSET_SUBSCRIPT        ((UINT8)1)  
/**<Indicates the subscript text offset.*/
#define WGL_CC_TEXT_OFFSET_SUPERSCRIPT      ((UINT8)2)  
/**<Indicates the superscript text offset.*/

/*------------------------------------------------------------------*/
/*! @brief the structure of closed caption pen font property.
  *  @code 
  *  typedef struct _WGL_CC_PEN_FONT_T 
  *  { 
  *     WGL_CC_FONT_INFO_T      t_font;
  *     UINT8                               ui1_offset;
  *  } WGL_CC_PEN_FONT_T; 
  *  @endcode 
  *  @li@c  t_font                        -Reference to the font information. 
  *  @li@c  ui1_offset                  -Reference to the font offset information. 
  */
/*------------------------------------------------------------------*/
typedef struct _WGL_CC_PEN_FONT_T
{
    WGL_CC_FONT_INFO_T      t_font;
    UINT8                   ui1_offset;
    
} WGL_CC_PEN_FONT_T;


#define WGL_CC_OPACITY_SOLID            ((UINT8)0)  
/**<Indicates the solid opacity.*/
#define WGL_CC_OPACITY_FLASH            ((UINT8)1)  
/**<Indicates the flashing opacity.*/
#define WGL_CC_OPACITY_TRANSLUCENT      ((UINT8)2)  
/**<Indicates the translucent opacity.*/
#define WGL_CC_OPACITY_TRANSPARENT      ((UINT8)3)  
/**<Indicates the transparent opacity.*/


/*------------------------------------------------------------------*/
/*! @brief the structure of closed caption pen color property.
  *  @code 
  *  typedef struct _WGL_CC_PEN_COLOR_T 
  *  { 
  *     GL_COLOR_T         t_color_text;
  *     GL_COLOR_T         t_color_bk;
  *     GL_COLOR_T         t_color_edge;
  *     BOOL                    b_is_flash_fg;
  *     BOOL                    b_is_flash_bk;
  *  } WGL_CC_PEN_COLOR_T; 
  *  @endcode 
  *  @li@c  t_color_text                 -Reference to the text color attribute. 
  *  @li@c  t_color_bk                   -Reference to the background color attribute. 
  *  @li@c  t_color_edge                -Reference to the text edge color attribute.
  *  @li@c  b_is_flash_fg               -Reference to the foreground flash attribute.
  *  @li@c  b_is_flash_bk               -Reference to the background flash attribute.  
  */
/*------------------------------------------------------------------*/
typedef struct _WGL_CC_PEN_COLOR_T
{
    GL_COLOR_T              t_color_text;
    GL_COLOR_T              t_color_bk;
    GL_COLOR_T              t_color_edge;
    
    BOOL                    b_is_flash_fg;
    BOOL                    b_is_flash_bk;
} WGL_CC_PEN_COLOR_T;


/*------------------------------------------------------------------*/
/*! @brief the structure of closed caption text attribute.
  *  @code 
  *  typedef struct _WGL_CC_TEXT_ATTR_T 
  *  { 
  *     WGL_CC_FONT_INFO_T      t_font;
  *     UINT8                               ui1_offset;
  *     GL_COLOR_T                     t_color_text;
  *     GL_COLOR_T                     t_color_bk;
  *     GL_COLOR_T                     t_color_edge;
  *     BOOL                                b_is_flash_fg;
  *     BOOL                                b_is_flash_bk;
  *  } WGL_CC_TEXT_ATTR_T; 
  *  @endcode 
  *  @li@c  t_font                        -Reference to the font information. 
  *  @li@c  ui1_offset                  -Reference to the font offset information. 
  *  @li@c  t_color_text                 -Reference to the text color attribute. 
  *  @li@c  t_color_bk                   -Reference to the background color attribute. 
  *  @li@c  t_color_edge                -Reference to the text edge color attribute.
  *  @li@c  b_is_flash_fg               -Reference to the foreground flash attribute.
  *  @li@c  b_is_flash_bk               -Reference to the background flash attribute.  
  */
/*------------------------------------------------------------------*/
typedef struct _WGL_CC_TEXT_ATTR_T
{
    WGL_CC_FONT_INFO_T      t_font;
    UINT8                   ui1_offset;
    GL_COLOR_T              t_color_text;
    GL_COLOR_T              t_color_bk;
    GL_COLOR_T              t_color_edge;
    
    BOOL                    b_is_flash_fg;
    BOOL                    b_is_flash_bk;
    
} WGL_CC_TEXT_ATTR_T;    


/* flags for WGL_CC_OVERRIDE_ATTR_T.ui2_flag */
#define WGL_CC_OVERRIDE_FLAG_COLOR_WIN                      MAKE_BIT_MASK_16(0)
/**<If this flag is set, the t_color_win field of the structure WGL_CC_OVERRIDE_ATTR_T is valid.*/

#define WGL_CC_OVERRIDE_FLAG_IS_FLASH_WIN                   MAKE_BIT_MASK_16(1)
/**<If this flag is set, the b_is_flash_win field of the structure WGL_CC_OVERRIDE_ATTR_T is valid.*/

#define WGL_CC_OVERRIDE_FLAG_BORDER_TYPE_WIN                MAKE_BIT_MASK_16(2)
/**<If this flag is set, the ui1_border_type_win field of the structure WGL_CC_OVERRIDE_ATTR_T is valid.*/

#define WGL_CC_OVERRIDE_FLAG_COLOR_WIN_BORDER               MAKE_BIT_MASK_16(3)
/**<If this flag is set, the t_color_win_border field of the structure WGL_CC_OVERRIDE_ATTR_T is valid.*/

#define WGL_CC_OVERRIDE_FLAG_JUSTIFY_WIN                    MAKE_BIT_MASK_16(4)
/**<If this flag is set, the ui1_justify  field of the structure WGL_CC_OVERRIDE_ATTR_T is valid.*/

#define WGL_CC_OVERRIDE_FLAG_FONT_INFO                      MAKE_BIT_MASK_16(5)
/**<If this flag is set, the t_font_info field of the structure WGL_CC_OVERRIDE_ATTR_T is valid.*/

#define WGL_CC_OVERRIDE_FLAG_COLOR_TEXT                     MAKE_BIT_MASK_16(6)
/**<If this flag is set, the t_color_text field of the structure WGL_CC_OVERRIDE_ATTR_T is valid.*/

#define WGL_CC_OVERRIDE_FLAG_COLOR_TEXT_BK                  MAKE_BIT_MASK_16(7)
/**<If this flag is set, the t_color_text_bk field of the structure WGL_CC_OVERRIDE_ATTR_T is valid.*/

#define WGL_CC_OVERRIDE_FLAG_COLOR_TEXT_EDGE                MAKE_BIT_MASK_16(8)
/**<If this flag is set, the t_color_text_edge field of the structure WGL_CC_OVERRIDE_ATTR_T is valid.*/

#define WGL_CC_OVERRIDE_FLAG_IS_FLASH_TEXT_FG               MAKE_BIT_MASK_16(9)
/**<If this flag is set, the b_is_flash_text_fg field of the structure WGL_CC_OVERRIDE_ATTR_T is valid.*/

#define WGL_CC_OVERRIDE_FLAG_IS_FLASH_TEXT_BK               MAKE_BIT_MASK_16(10)
/**<If this flag is set, the b_is_flash_text_bk field of the structure WGL_CC_OVERRIDE_ATTR_T is valid.*/

#define WGL_CC_OVERRIDE_FLAG_LAST_FLAG                      MAKE_BIT_MASK_16(10)
/**<Reserved. Indicates the last override flag.*/

#define WGL_CC_OVERRIDE_ALL_FLAGS                           ((WGL_CC_OVERRIDE_FLAG_LAST_FLAG<<1)-1)
/**<If this flag is set,  it means all the above flags are set and all the accordingly fields of the structure WGL_CC_OVERRIDE_ATTR_T are valid.*/

/* flags for WGL_CC_OVERRIDE_ATTR_T.ui4_flag_ex */
#define WGL_CC_OVERRIDE_FLAG_EX_FONT_NAME                   MAKE_BIT_MASK_32(0)
/**<If this flag is set, the sub-field s_font_name in the structure WGL_CC_OVERRIDE_ATTR_T 's field t_font_info is effective.*/

#define WGL_CC_OVERRIDE_FLAG_EX_FONT_SIZE                   MAKE_BIT_MASK_32(1)
/**<If this flag is set, the sub-field e_size in the structure WGL_CC_OVERRIDE_ATTR_T 's field t_font_info is effective.*/

#define WGL_CC_OVERRIDE_FLAG_EX_FONT_STYLE                  MAKE_BIT_MASK_32(2)
/**<If this flag is set, the sub-field e_style in the structure WGL_CC_OVERRIDE_ATTR_T 's field t_font_info is effective.*/

#define WGL_CC_OVERRIDE_FLAG_EX_FONT_EDGE_TYPE              MAKE_BIT_MASK_32(3)
/**<If this flag is set, the sub-field ui1_edge_type in the structure WGL_CC_OVERRIDE_ATTR_T 's field t_font_info is effective.*/

#define WGL_CC_OVERRIDE_FLAG_EX_COLOR_WIN_RGB               MAKE_BIT_MASK_32(4)
/**<If this flag is set, the red, green, and blue color components in the structure WGL_CC_OVERRIDE_ATTR_T 's field t_color_win is effective.*/

#define WGL_CC_OVERRIDE_FLAG_EX_COLOR_WIN_ALPHA             MAKE_BIT_MASK_32(5)
/**<If this flag is set, the alpha color component in the structure WGL_CC_OVERRIDE_ATTR_T 's field t_color_win is effective.*/

#define WGL_CC_OVERRIDE_FLAG_EX_COLOR_WIN_BORDER_RGB        MAKE_BIT_MASK_32(6)
/**<If this flag is set, the red, green, and blue color components in the structure WGL_CC_OVERRIDE_ATTR_T 's field t_color_win_border is effective.*/

#define WGL_CC_OVERRIDE_FLAG_EX_COLOR_WIN_BORDER_ALPHA      MAKE_BIT_MASK_32(7)
/**<If this flag is set, the alpha color component in the structure WGL_CC_OVERRIDE_ATTR_T 's field t_color_win_border is effective.*/

#define WGL_CC_OVERRIDE_FLAG_EX_COLOR_TEXT_RGB              MAKE_BIT_MASK_32(8)
/**<If this flag is set, the red, green, and blue color components in the structure WGL_CC_OVERRIDE_ATTR_T 's field t_color_text is effective.*/

#define WGL_CC_OVERRIDE_FLAG_EX_COLOR_TEXT_ALPHA            MAKE_BIT_MASK_32(9)
/**<If this flag is set, the alpha color component in the structure WGL_CC_OVERRIDE_ATTR_T 's field t_color_text is effective.*/

#define WGL_CC_OVERRIDE_FLAG_EX_COLOR_TEXT_BK_RGB           MAKE_BIT_MASK_32(10)
/**<If this flag is set, the red, green, and blue color components in the structure WGL_CC_OVERRIDE_ATTR_T 's field t_color_text_bk is effective.*/

#define WGL_CC_OVERRIDE_FLAG_EX_COLOR_TEXT_BK_ALPHA         MAKE_BIT_MASK_32(11)
/**<If this flag is set, the alpha color component in the structure WGL_CC_OVERRIDE_ATTR_T 's field t_color_text_bk is effective.*/

#define WGL_CC_OVERRIDE_FLAG_EX_COLOR_TEXT_EDGE_RGB         MAKE_BIT_MASK_32(12)
/**<If this flag is set, the red, green, and blue color components in the structure WGL_CC_OVERRIDE_ATTR_T 's field t_color_text_edge is effective.*/

#define WGL_CC_OVERRIDE_FLAG_EX_COLOR_TEXT_EDGE_ALPHA       MAKE_BIT_MASK_32(13)
/**<If this flag is set, the alpha color component in the structure WGL_CC_OVERRIDE_ATTR_T 's field t_color_text_edge is effective.*/

#define WGL_CC_OVERRIDE_FLAG_EX_LAST_FLAG                   MAKE_BIT_MASK_32(13)
/**<Reserved. Indicated the last override extension flag.*/

#define WGL_CC_OVERRIDE_ALL_EX_FLAGS                        ((WGL_CC_OVERRIDE_FLAG_EX_LAST_FLAG<<1)-1)
/**<If this flag is set, it means all the above flags are set and the accordingly fields are effective.*/


/*------------------------------------------------------------------*/
/*! @brief the structure of closed caption override attribute.
  *  @code 
  *  typedef struct _WGL_CC_OVERRIDE_ATTR_T 
  *  { 
  *  UINT16                             ui2_flag;
  *  UINT32                             ui4_flag_ex;
  *  GL_COLOR_T                     t_color_win;
  *  BOOL                               b_is_flash_win;
  *  UINT8                              ui1_border_type_win;
  *  GL_COLOR_T                     t_color_win_border;
  *  UINT8                              ui1_justify;
  *  WGL_CC_FONT_INFO_T      t_font_info;
  *  GL_COLOR_T                     t_color_text;
  *  GL_COLOR_T                     t_color_text_bk;
  *  GL_COLOR_T                     t_color_text_edge;
  *  BOOL                               b_is_flash_text_fg;
  *  BOOL                               b_is_flash_text_bk;      
  *  } WGL_CC_OVERRIDE_ATTR_T; 
  *  @endcode 
  *  @li@c  ui2_flag                        -Reference to the flag of override attribute. 
  *  @li@c  ui4_flag_ex                  -Reference to the extended flag of override attribute. 
  *  @li@c  t_color_win                        -Reference to the window color attribute.   
  *  @li@c  b_is_flash_win                        -Reference to window flash attribute. 
  *  @li@c  ui1_border_type_win                  -Reference to border type of window. 
  *  @li@c  t_color_win_border                        -Reference to border color of window. 
  *  @li@c  ui1_justify                  -Reference to the justify attribute of caption.
  *  @li@c  t_font_info                  -Reference to the font offset information. 
  *  @li@c  t_color_text                 -Reference to the text color attribute. 
  *  @li@c  t_color_text_bk                   -Reference to the background color attribute. 
  *  @li@c  t_color_text_edge                -Reference to the text edge color attribute.
  *  @li@c  b_is_flash_text_fg               -Reference to the foreground flash attribute.
  *  @li@c  b_is_flash_text_bk               -Reference to the background flash attribute.  
  */
/*------------------------------------------------------------------*/
typedef struct _WGL_CC_OVERRIDE_ATTR_T
{
        UINT16                  ui2_flag;
        UINT32                  ui4_flag_ex;
        GL_COLOR_T              t_color_win;
        BOOL                    b_is_flash_win;
        UINT8                   ui1_border_type_win;
        GL_COLOR_T              t_color_win_border;
        UINT8                   ui1_justify;
        WGL_CC_FONT_INFO_T      t_font_info;
        GL_COLOR_T              t_color_text;
        GL_COLOR_T              t_color_text_bk;
        GL_COLOR_T              t_color_text_edge;
        BOOL                    b_is_flash_text_fg;
        BOOL                    b_is_flash_text_bk;    
        
} WGL_CC_OVERRIDE_ATTR_T;    


#define WGL_CC_INFO_DTVCC_MODE                  (((UINT16)1)<<0)    /* mutual exclusive with WGL_CC_INFO_NTSC_MODE */
/**<
If this flag is set, the Closed Caption widget is for DTV CC. 
It must be mutual exclusive with WGL_CC_INFO_NTSC_MODE.
*/

#define WGL_CC_INFO_NTSC_MODE                   (((UINT16)1)<<1)    /* mutual exclusive with WGL_CC_INFO_DTVCC_MODE */
/**<
If this flag is set, the Closed Caption widget is for NTSC CC. 
It must be mutual exclusive with WGL_CC_INFO_DTVCC_MODE.
*/

#define WGL_CC_INFO_ROW_LOCK                    (((UINT16)1)<<2)
/**<If this flag is set, the Closed Caption widget performs row locking when resizing.*/

#define WGL_CC_INFO_COL_LOCK                    (((UINT16)1)<<3)
/**<If this flag is set, the Closed Caption widget performs column locking when resizing.*/

#define WGL_CC_INFO_VISIBLE                     (((UINT16)1)<<4)
/**<If this flag is set, the Closed Caption widget is visible.*/

#define WGL_CC_INFO_AUTO_DEL_INVISIBLE_PARAG    (((UINT16)1)<<5)
/**<If this flag is set, the Closed Caption widget automatically deletes invisible paragraphs that have been scrolled above the viewport.*/

#define WGL_CC_INFO_SKIP_UNDERLINE              (((UINT16)1)<<6)
/**<If this flag is set, the Closed Caption widget automatically skipps the underline attributes.*/

#define WGL_CC_INFO_IS_VALID_WIN_ATTR           (((UINT16)1)<<7)
/**<This flag indicates the t_win_attr field of WGL_CC_INFO_T is valid.*/

#define WGL_CC_INFO_IS_VALID_PEN_FONT           (((UINT16)1)<<8)
/**<This flag indicates the t_pen_font field of WGL_CC_INFO_T is valid.*/

#define WGL_CC_INFO_IS_VALID_PEN_COLOR          (((UINT16)1)<<9)
/**<This flag indicates the t_pen_color field of WGL_CC_INFO_T is valid.*/

#define WGL_CC_INFO_LAST_FLAG                   (WGL_CC_INFO_IS_VALID_PEN_COLOR)
/**<Reserved. This flag indicates the last info flag.*/


/*------------------------------------------------------------------*/
/*! @brief the structure of closed caption information.
  *  @code 
  *  typedef struct _WGL_CC_INFO_T 
  *  { 
  *         UINT16                                ui2_cc_info_flags;
  *         UINT8                                 ui1_anchor;
  *         UINT16                               ui2_anchor_v;
  *         UINT16                               ui2_anchor_h;
  *         UINT16                               ui2_row_count;
  *         UINT16                               ui2_col_count;    
  *         WGL_CC_WIN_ATTR_T         t_win_attr;
  *         WGL_CC_PEN_FONT_T         t_pen_font;
  *         WGL_CC_PEN_COLOR_T      t_pen_color;
  *  } WGL_CC_INFO_T; 
  *  @endcode 
  *  @li@c  ui2_cc_info_flags          -Reference to the valid flag of cc information. 
  *  @li@c  ui1_anchor                   -Reference to the anchor information. 
  *  @li@c  ui2_anchor_v               -Reference to the anchor vertical position infomation.   
  *  @li@c  ui2_anchor_h               -Reference to the anchor horizontal position information. 
  *  @li@c  ui2_row_count             -Reference to row count of caption. 
  *  @li@c  ui2_col_count              -Reference to column count of caption. 
  *  @li@c  t_win_attr                   -Reference to window attribute information.
  *  @li@c  t_pen_font                  -Reference to font attribute information. 
  *  @li@c  t_pen_color                 -Reference to color attribute information. 
  */
/*------------------------------------------------------------------*/
typedef struct _WGL_CC_INFO_T
{
    UINT16                  ui2_cc_info_flags;
    UINT8                   ui1_anchor;
    UINT16                  ui2_anchor_v;
    UINT16                  ui2_anchor_h;
    UINT16                  ui2_row_count;
    UINT16                  ui2_col_count;
    
    WGL_CC_WIN_ATTR_T       t_win_attr;
    WGL_CC_PEN_FONT_T       t_pen_font;
    WGL_CC_PEN_COLOR_T      t_pen_color;
    
} WGL_CC_INFO_T;    


/*------------------------------------------------------------------*/
/*! @brief the structure of closed caption initialization information.
  *  @code 
  *  typedef struct _WGL_CC_INIT_INFO_T 
  *  { 
  *         WGL_CC_OVERRIDE_ATTR_T      t_override_attr;
  *         UINT32                                      ui4_font_tbl_id;
  *         UINT16                                      ui2_max_width;
  *         UINT16                                      ui2_max_height;
  *         UINT32                                      ui4_flashing_interval;
  *         UINT32                                      ui4_smooth_scroll_time_bound;
  *         UINT32                                      ui4_typewriter_effect_time_bound;
  *         UINT16                                      ui2_tab_size;    
  *         WGL_CC_INFO_T                       t_cc_info;
  *  } WGL_CC_INIT_INFO_T; 
  *  @endcode 
  *  @li@c  t_override_attr                               -Reference to the override attribute of caption.
  *  @li@c  ui4_font_tbl_id                               -Reference to the font table id of caption. 
  *  @li@c  ui2_max_width                               -Reference to the max width attribute of caption.   
  *  @li@c  ui2_max_height                              -Reference to the max height attribute of caption. 
  *  @li@c  ui4_flashing_interval                       -Reference to flash interval attribute of caption. 
  *  @li@c  ui4_smooth_scroll_time_bound        -Reference to smooth scroll time bound of caption. 
  *  @li@c  ui4_typewriter_effect_time_bound   -Reference to typewriter effect time bound of caption.
  *  @li@c  ui2_tab_size                                  -Reference to tab size attribute. 
  *  @li@c  t_cc_info                                       -Reference to cc information attribute. 
  */
/*------------------------------------------------------------------*/
typedef struct _WGL_CC_INIT_INFO_T
{
    WGL_CC_OVERRIDE_ATTR_T      t_override_attr;
    UINT32                      ui4_font_tbl_id;
    UINT16                      ui2_max_width;
    UINT16                      ui2_max_height;
    UINT32                      ui4_flashing_interval;
    UINT32                      ui4_smooth_scroll_time_bound;
    UINT32                      ui4_typewriter_effect_time_bound;
    UINT16                      ui2_tab_size;
    
    WGL_CC_INFO_T               t_cc_info;
    
} WGL_CC_INIT_INFO_T;    

/* ntsc mode */
#define WGL_CC_NTSC_MODE_UNKNOWN                ((UINT8) 0) 
/**<Indicates the NTSC mode is unknown.*/
#define WGL_CC_NTSC_MODE_ROLL                   ((UINT8) 1) 
/**<Indicates the NTSC mode is ROLL.*/
#define WGL_CC_NTSC_MODE_POP                    ((UINT8) 2) 
/**<Indicates the NTSC mode is POP.*/
#define WGL_CC_NTSC_MODE_PAINT_ON               ((UINT8) 3) 
/**<Indicates the NTSC mode is PAINT-ON.*/
#define WGL_CC_NTSC_MODE_TEXT                   ((UINT8) 4) 
/**<Indicates the NTSC mode is TEXT.*/

/*Move mode definitions*/
#define WGL_CC_MOVE_MODE_ACROSS_PARAG           ((UINT8)0x01)   
/**<Indicates that when the pen location movement may go out of the bound of a paragraph, the pen doesn't keep in the paragraph and will be moved to other paragraph that locates above or below the paragraph.*/
#define WGL_CC_MOVE_MODE_STOP_AT_SOP            ((UINT8)0x02)   
/**<Indicates that when the pen location movement may go out of the left-bound of a paragraph, the pen doesn't move further and will stop at the start of the paragraph.*/
#define WGL_CC_MOVE_MODE_STOP_AT_EOP            ((UINT8)0x03)   
/**<Indicates that when the pen location movement may go out of the end-bound of a paragraph, the pen doesn't move further and will stop at the end of the paragraph.*/
#define WGL_CC_MOVE_MODE_FILL_SPACE             ((UINT8)0x04)   
/**<Indicates that when the pen location movement may go out of the end-bound of a paragraph, the pen doesn't jump to other paragraphs. Instead, the pen keeps in current paragraph and the CC widget automatically fills necessary amount of "non-transparent space" characters to achieve the pen movement.*/
#define WGL_CC_MOVE_MODE_FILL_TRANSP_SPACE      ((UINT8)0x05)   
/**<Indicates that when the pen location movement may go out of the end-bound of a paragraph, the pen doesn't jump to other paragraphs. Instead, the pen keeps in current paragraph and the CC widget automatically fills necessary amount of "transparent space" characters to achieve the pen movement.*/
#define WGL_CC_MOVE_MODE_FILL_NULL_SPACE        ((UINT8)0x06)   
/**<Indicates that when the pen location movement may go out of the end-bound of a paragraph, the pen doesn't jump to other paragraphs. Instead, the pen keeps in current paragraph and the CC widget automatically fills necessary amount of "null space" characters to achieve the pen movement.*/



/*------------------------------------------------------------------*/
/*! @brief the structure of closed caption text paragraph location property.
  *  @code 
  *  typedef struct _WGL_CC_TEXT_PARAG_LOCATION_T 
  *  { 
  *         UINT16      ui2_idx_parag;
  *         UINT16      ui2_col;
  *  } WGL_CC_TEXT_PARAG_LOCATION_T; 
  *  @endcode 
  *  @li@c  ui2_idx_parag                     -Reference to index of paragraph.
  *  @li@c  ui2_col                               -Reference to column in the specified paragraph.
  */
/*------------------------------------------------------------------*/
typedef struct _WGL_CC_TEXT_PARAG_LOCATION_T
{
    UINT16      ui2_idx_parag;
    UINT16      ui2_col;
    
} WGL_CC_TEXT_PARAG_LOCATION_T;    

/* update mode */
/*------------------------------------------------------------------*/
/*! @brief     Indicates the update mode of closed caption.
  *  @code 
  *  typedef enum _WGL_CC_UPDATE_MODE_T
  *  { 
  *      WGL_CC_UPDATE_MODE_DEFAULT = 0,
  *      WGL_CC_UPDATE_MODE_AUTO_SCRL,
  *      WGL_CC_UPDATE_MODE_AUTO_SMTH_SCRL, 
  *  } WGL_CC_UPDATE_MODE_T; 
  *  @endcode 
  *  @li@c  WGL_CC_UPDATE_MODE_DEFAULT                 -Indicated default update mode.
  *  @li@c  WGL_CC_UPDATE_MODE_AUTO_SCRL            -Indicated auto scroll update mode.
  *  @li@c  WGL_CC_UPDATE_MODE_AUTO_SMTH_SCRL   -Indicated auto smooth scroll update.
  */
/*------------------------------------------------------------------*/
typedef enum _WGL_CC_UPDATE_MODE_T
{
    WGL_CC_UPDATE_MODE_DEFAULT = 0,
    WGL_CC_UPDATE_MODE_AUTO_SCRL,
    WGL_CC_UPDATE_MODE_AUTO_SMTH_SCRL,
    
} WGL_CC_UPDATE_MODE_T;

/* backspace mode */
#define WGL_CC_BS_FLAG_DEFAULT                          ((UINT32) NULL)
/**<When processing backspace, it is in the default way.*/

#define WGL_CC_BS_FLAG_SKIP_AT_COL_LIMIT                MAKE_BIT_MASK_32 (0)
/**<When processing backspace, if the pen location is beyond the column count and it is not at the end of current paragraph, nothing needs to do.*/

#define WGL_CC_BS_FLAG_ONLY_DEL_STD_SP                  MAKE_BIT_MASK_32 (1)
/**<When processing backspace, */

#define WGL_CC_BS_FLAG_LAST_FLAG                        WGL_CC_BS_FLAG_ONLY_DEL_STD_SP
/**<Reserved. Indicates the last backspace mode.*/


/* rescale info */

/*------------------------------------------------------------------*/
/*! @brief the structure of closed caption resclale information.
  *  @code 
  *  typedef struct _WGL_CC_RESCALE_INFO_T 
  *  { 
  *         UINT32      ui4_font_tbl_id;
  *         UINT16      ui2_max_width;
  *         UINT16      ui2_max_height;
  *         UINT16      ui2_anchor_h;
  *         UINT16      ui2_anchor_v;
  *  } WGL_CC_RESCALE_INFO_T; 
  *  @endcode 
  *  @li@c  ui4_font_tbl_id                    -Reference to font tabld in of caption.
  *  @li@c  ui2_max_width                   -Reference to max width information of caption.
  *  @li@c  ui2_max_height                  -Reference to max height information of caption.
  *  @li@c  ui2_anchor_v                      -Reference to the anchor vertical position infomation.   
  *  @li@c  ui2_anchor_h                      -Reference to the anchor horizontal position information. 
  */
/*------------------------------------------------------------------*/
typedef struct _WGL_CC_RESCALE_INFO_T
{
    UINT32      ui4_font_tbl_id;
    UINT16      ui2_max_width;
    UINT16      ui2_max_height;
    UINT16      ui2_anchor_h;
    UINT16      ui2_anchor_v;
    
} WGL_CC_RESCALE_INFO_T;


/*------------------------------------------------------------------*/
/*! @brief the structure of closed caption register information parameters.
  *  @code 
  *  typedef struct _WGL_CC_FONT_REG_INFO_PARAM_T 
  *  { 
  *         INT16                               i2_row_inset_top;
  *         INT16                               i2_row_inset_bottom;
  *         UINT16                              ui2_mono_space_width;
  *  } WGL_CC_FONT_REG_INFO_PARAM_T; 
  *  @endcode 
  *  @li@c  i2_row_inset_top                    -Reference to font top inset attribute.
  *  @li@c  i2_row_inset_bottom              -Reference to font bottom inset attribute.
  *  @li@c  ui2_mono_space_width          -Reference to font monospace width attribute.
  */
/*------------------------------------------------------------------*/
typedef struct _WGL_CC_FONT_REG_INFO_PARAM_T
{
    INT16                               i2_row_inset_top;
    INT16                               i2_row_inset_bottom;
    UINT16                              ui2_mono_space_width;
} WGL_CC_FONT_REG_INFO_PARAM_T;


/*------------------------------------------------------------------*/
/*! @brief the structure of closed caption resource paramter of font register information.
  *  @code 
  *  typedef struct _WGL_CC_FONT_REG_INFO_RES_PARAM_T 
  *  { 
  *         UINT32                                                   ui4_font_tbl_id;
  *         WGL_CC_FONT_REG_INFO_PARAM_T        t_param_small;
  *         WGL_CC_FONT_REG_INFO_PARAM_T        t_param_medium;
  *         WGL_CC_FONT_REG_INFO_PARAM_T        t_param_large; 
  *  } WGL_CC_FONT_REG_INFO_RES_PARAM_T; 
  *  @endcode 
  *  @li@c  ui4_font_tbl_id          -Reference to font table id information.
  *  @li@c  t_param_small          -Reference to small font registration table information.
  *  @li@c  t_param_medium      -Reference to medium font registration table information.
  *  @li@c  t_param_large          -Reference to large font registration table information.
  */
/*------------------------------------------------------------------*/
typedef struct _WGL_CC_FONT_REG_INFO_RES_PARAM_T
{
    /* key field: font_tbl_id (made by macro WGL_CC_MAKE_FONT_TBL_ID())*/
    UINT32                              ui4_font_tbl_id;
    
    /* value fields: font param */
    WGL_CC_FONT_REG_INFO_PARAM_T        t_param_small;
    WGL_CC_FONT_REG_INFO_PARAM_T        t_param_medium;
    WGL_CC_FONT_REG_INFO_PARAM_T        t_param_large;
    
} WGL_CC_FONT_REG_INFO_RES_PARAM_T;




/*------------------------------------------------------------------*/
/*! @brief the structure of closed caption font register information.
  *  @code 
  *  typedef struct _WGL_CC_FONT_REG_INFO_T 
  *  { 
  *     CHAR                                                        s_font_name[WGL_MAX_FONT_NAME];
  *     WGL_CC_FONT_REG_INFO_RES_PARAM_T   *at_tbl_res_param;
  *  } WGL_CC_FONT_REG_INFO_T; 
  *  @endcode 
  *  @li@c  s_font_name                        -Reference to the font name string.
  *  @li@c  at_tbl_res_param                 -Reference to the registration information. 
  */
/*------------------------------------------------------------------*/
typedef struct _WGL_CC_FONT_REG_INFO_T
{
    /* key field: font name */
    CHAR                                s_font_name[WGL_MAX_FONT_NAME];
    
    /* value field: table mapping from res_key to font param */
    WGL_CC_FONT_REG_INFO_RES_PARAM_T    *at_tbl_res_param;
    
} WGL_CC_FONT_REG_INFO_T;    



#ifdef __cplusplus
}
#endif


#endif

#endif

/*----------------------------------------------------------------------------*/
/*! @} */                                /* end of MW_WGL_CC */
/*----------------------------------------------------------------------------*/


