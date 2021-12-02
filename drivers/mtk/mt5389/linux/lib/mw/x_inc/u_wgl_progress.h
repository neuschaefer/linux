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
/*! @file u_wgl_progress.h 
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
/*! @defgroup groupMW_WGL_PROGRESS   Progress Widget
 *
 *  @ingroup groupMW_WGL
 *  @brief Provides definitions and exported function of the progress widget.
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW_WGL
 *  @{ 
 */
/*----------------------------------------------------------------------------*/ 
#ifndef _U_WGL_PROGRESS_H_
#define _U_WGL_PROGRESS_H_

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
/*Specific Color Command Target*/
#define WGL_CLR_PG_BODY                         ((UINT8) WGL_CLR_WGT_SPF_START + 1)    /**<  This definition indicates that the color is for drawing ProgressBar's body.       */
#define WGL_CLR_PG_LEVEL_1                      ((UINT8) WGL_CLR_WGT_SPF_START + 2)    /**<  This definition indicates that the color is for drawing ProgressBar's level-1 foreground.      */
#define WGL_CLR_PG_LEVEL_2                      ((UINT8) WGL_CLR_WGT_SPF_START + 3)    /**<  This definition indicates that the color is for drawing ProgressBar's level-2 foreground.       */
#define WGL_CLR_PG_LEVEL_3                      ((UINT8) WGL_CLR_WGT_SPF_START + 4)    /**<   This definition indicates that the color is for drawing ProgressBar's level-3 foreground.    */
#define WGL_CLR_PG_LEVEL_4                      ((UINT8) WGL_CLR_WGT_SPF_START + 5)    /**< 	 This definition indicates that the color is for drawing ProgressBar's level-4 foreground.       */
#define WGL_CLR_PG_LEVEL_5                      ((UINT8) WGL_CLR_WGT_SPF_START + 6)    /**<  This definition indicates that the color is for drawing ProgressBar's level-5 foreground.     */
#define WGL_CLR_PG_CURSOR                       ((UINT8) WGL_CLR_WGT_SPF_START + 7)    /**<  This definition indicates that the color is for drawing ProgressBar's cursor.      */
#define WGL_CLR_PG_CURSOR_INSET                 ((UINT8) WGL_CLR_WGT_SPF_START + 8)    /**<   This definition indicates that the color is for drawing ProgressBar's cursor inset.     */
/*Specific Image Command Target*/
#define WGL_IMG_PG_BODY                         ((UINT8) WGL_IMG_WGT_SPF_START + 1)    /**<  This definition indicates that the template image is for drawing ProgressBar's body.       */
#define WGL_IMG_PG_LEVEL_1                      ((UINT8) WGL_IMG_WGT_SPF_START + 2)    /**< This definition indicates that the template image is for drawing ProgressBar's level-1 foreground.       */
#define WGL_IMG_PG_LEVEL_2                      ((UINT8) WGL_IMG_WGT_SPF_START + 3)    /**<  This definition indicates that the template image is for drawing ProgressBar's level-2 foreground.     */
#define WGL_IMG_PG_LEVEL_3                      ((UINT8) WGL_IMG_WGT_SPF_START + 4)    /**<  This definition indicates that the template image is for drawing ProgressBar's level-3 foreground.      */
#define WGL_IMG_PG_LEVEL_4                      ((UINT8) WGL_IMG_WGT_SPF_START + 5)    /**< 	This definition indicates that the template image is for drawing ProgressBar's level-4 foreground.       */
#define WGL_IMG_PG_LEVEL_5                      ((UINT8) WGL_IMG_WGT_SPF_START + 6)    /**<  This definition indicates that the template image is for drawing ProgressBar's level-5 foreground.      */
#define WGL_IMG_PG_CURSOR                       ((UINT8) WGL_CLR_WGT_SPF_START + 7)    /**< This definition indicates that the template image is for drawing ProgressBar's cursor.       */

/*Specific Widget Style*/
#define WGL_STL_PG_VERTICAL_UP                  ((UINT32) WGL_STL_WGT_SPF_START)    /**< If this style is assigned, it means the widget is vertical and the level-1 is below the level-2 in display.       */
#define WGL_STL_PG_VERTICAL_DOWN                ((UINT32) WGL_STL_WGT_SPF_START << 1)    /**< If this style is assigned, it means the widget is vertical and the level-1 is above the level-2 in display.       */
#define WGL_STL_PG_BLOCK                        ((UINT32) WGL_STL_WGT_SPF_START << 2)    /**<  If this style is assigned, it means the ProgressBar will display the progress status in a segmented bar.      */
#define WGL_STL_PG_POINT                        ((UINT32) WGL_STL_WGT_SPF_START << 3)    /**<   If this style is assigned, it means the value indicator is a block, not a bar.     */
#define WGL_STL_PG_CURSOR                       ((UINT32) WGL_STL_WGT_SPF_START << 4)    /**<  If this style is assigned, it means the value indicator is a cursor, not a bar or block       */


/*Display Mode*/
#define WGL_PG_DM_HIDE_IDR                      ((UINT8) 0x01)    /**<  If this style is assigned, a ProgressBar won't show any string for its current pos.     */
#define WGL_PG_DM_SHOW_MIN                      ((UINT8) 0x02)    /**<  	If this style is assigned, a progress bar will show its minimum.      */
#define WGL_PG_DM_SHOW_MAX                      ((UINT8) 0x04)    /**<  If this style is assigned, a progress bar will show its maximum.      */
#define WGL_PG_DM_PERCENT                       ((UINT8) 0x08)    /**< 	If this style is assigned, a progress bar will show the percentage for its current pos, minimum, and maximum; whereas it shows the concrete value.    */
#define WGL_PG_DM_HIDE_BAR                      ((UINT8) 0x10)    /**< If this style is assigned, a progress bar will hide its bar.       */
#define WGL_PG_DM_SHOW_ALL                      (WGL_PG_DM_SHOW_MIN | WGL_PG_DM_SHOW_MAX)    /**< If this style is assigned, a progress bar will show its minimum and maximum.   */
#define WGL_PG_DM_SHOW_ALL_PNT                  (WGL_PG_DM_SHOW_ALL | WGL_PG_DM_PERCENT)    /**< If this style is assigned, a progress bar will show its minimum, maximum and percentage for its current pos.        */

/*Specific Command*/
#define WGL_CMD_PG_SET_RANGE                    WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 0)    /**< This command is used to set the range for a ProgressBar. Note: the command may affect its level setting. All level boundaries that aren't less than the new maximum will be removed.

                                                                                                                                                                                    related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                    pv_param1:   An INT32 value specifies the minimum.

                                                                                                                                                                                    pv_param2:   An INT32 value specifies the maximum.
                                                                                                                                                                                    */
#define WGL_CMD_PG_GET_RANGE                    WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 1)    /**<This command returns the range of a ProgressBar.

                                                                                                                                                                                    related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                    pv_param1:   Pointer to an INT32 variable to receive the minimum. It cannot be NULL.

                                                                                                                                                                                    pv_param2:   Pointer to an INT32 variable to receive the maximum. It cannot be NULL.
                                                                                                                                                                                    */
#define WGL_CMD_PG_SET_STEP                     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 2)    /**<  This command sets the step increment for a ProgressBar.

                                                                                                                                                                                    related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                    pv_param1:   An INT32 value specifies the step increment. If the value is less than zero, the position of a ProgressBar will become smaller after invoking its WGL_CMD_PG_STEP_POS command with a positive value.

                                                                                                                                                                                    pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                    */
#define WGL_CMD_PG_GET_STEP                     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 3)    /**<  This command returns the step increment from a ProgressBar.

                                                                                                                                                                                    related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                    pv_param1:   Pointer to an INT32 variable to receive the step increment. It cannot be NULL.

                                                                                                                                                                                    pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                    */
#define WGL_CMD_PG_SET_POS                      WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 4)    /**<   This command moves the current position to the new one.

                                                                                                                                                                                    related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                    pv_param1:   An INT32 value specifies the new position. 

                                                                                                                                                                                    pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                    */
#define WGL_CMD_PG_GET_POS                      WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 5)    /**<  This command returns the current position of a ProgressBar.

                                                                                                                                                                                    related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                    pv_param1:   Pointer to an INT32 value to receive the current position. It cannot be NULL.

                                                                                                                                                                                    pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                    */
#define WGL_CMD_PG_INCREASE_POS                 WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 6)    /**<    This command adds the specified offset to the current position of a ProgressBar.

                                                                                                                                                                                     related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                     pv_param1:   An INT32 value specifies the offset. If the offset is less than zero, it'll decrease the current position.

                                                                                                                                                                                     pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                     */
#define WGL_CMD_PG_STEP_POS                     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 7)    /**<  This command advances ProgressBar's current position by the total step increments.     

                                                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                        pv_param1:   An INT32 value specifies the amount of steps. The offset is equal to pv_param1 multiplied by the step increment.

                                                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                        */
#define WGL_CMD_PG_SET_KEY_MAP                  WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 8)    /**<   This command changes the keys for ProgressBar's position operations.

                                                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                        pv_param1:   Pointer to a WGL_PG_KEY_MAP_T that contains the new key map. It cannot be NULL.  

                                                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                        */
#define WGL_CMD_PG_GET_KEY_MAP                  WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 9)    /**<  This command returns the key map of a ProgressBar.

                                                                                                                                                                                    related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                    pv_param1:   Pointer to a WGL_PG_KEY_MAP_T to receive the key map. It cannot be NULL.

                                                                                                                                                                                    pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                    */
#define WGL_CMD_PG_SET_FAST_MODE_ATTR           WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 10)    /**<  This command changes the settings of ProgressBar's fast mode.

                                                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                        pv_param1:   Pointer to a WGL_PG_FAST_MODE_ATTR_T variable that contains the settings for fast mode. If it is NULL, it will use the default settings.

                                                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                        */
#define WGL_CMD_PG_GET_FAST_MODE_ATTR           WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 11)    /**< This command returns the settings of ProgressBar's fast mode.

                                                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                        pv_param1:   Pointer to a WGL_PG_FAST_MODE_ATTR_T variable to receive the settings for fast mode. It cannot be NULL.

                                                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                        */
#define WGL_CMD_PG_SET_WORKING_RANGE            WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 12)    /**<    This command is used to set the working range for a ProgressBar.

                                                                                                                                                                                    related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                    pv_param1:   An INT32 value specifies the minimum can not be less than range's minimum value.

                                                                                                                                                                                    pv_param2:   An INT32 value specifies the maximum can not be large then range's maximum value.
                                                                                                                                                                                    */
#define WGL_CMD_PG_GET_WORKING_RANGE            WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 13)    /**<   This command returns the working range of a ProgressBar.

                                                                                                                                                                                    related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                    pv_param1:   Pointer to an INT32 variable to receive the minimum. It cannot be NULL.

                                                                                                                                                                                    pv_param2:   Pointer to an INT32 variable to receive the maximum. It cannot be NULL.
                                                                                                                                                                                    */
    

/*Specific Command Supported by PROGRESS-BAR-Content*/
#define WGL_CMD_PG_SET_DISPLAY_MODE             WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 0)    /**< This command is used to set the display mode for a ProgressBar.

                                                                                                                                                                                       related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                       pv_param1:   A UINT8 value specifies the display mode. 

                                                                                                                                                                                       pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                       */
#define WGL_CMD_PG_GET_DISPLAY_MODE             WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 1)    /**<   This command returns the display mode of a ProgressBar.

                                                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                        pv_param1   Pointer to a UINT8 variable to receive the display mode. This argument cannot be NULL.

                                                                                                                                                                                        pv_param2   Reserved. It must be NULL.
                                                                                                                                                                                        */
#define WGL_CMD_PG_SET_BLOCK_GAP                WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 2)    /**<  This command is used to set the interval between segmented blocks.

                                                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                        pv_param1:   A UINT8 integer specifies the interval.

                                                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                        */
#define WGL_CMD_PG_GET_BLOCK_GAP                WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 3)    /**<  This command returns the interval between segmented blocks.

                                                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                        pv_param1:   Pointer to a UINT8 variable to receive the interval. It cannot be NULL.

                                                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                        */
#define WGL_CMD_PG_SET_BLK_PT_SIZE              WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 4)    /**< This command is used to set the size of segmented block if the ProgressBar's style is WGL_STL_PG_BLOCK or the point indicator if its style is WGL_STL_PG_POINT.

                                                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                        pv_param1:   A UINT16 value specifies the block's width.

                                                                                                                                                                                        pv_param2:   A UINT16 value specifies the block's height.
                                                                                                                                                                                        */
#define WGL_CMD_PG_GET_BLK_PT_SIZE              WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 5)    /**< This command returns the size of segmented block if the ProgressBar's style is WGL_STL_PG_BLOCK or the point indicator if its style is WGL_STL_PG_POINT.

                                                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                        pv_param1:   Pointer to a UINT16 variable to receive the block's width. It cannot be NULL.

                                                                                                                                                                                        pv_param2:   Pointer to a UINT16 variable to receive the block's height. It cannot be NULL.
                                                                                                                                                                                        */
#define WGL_CMD_PG_SET_CNT_INSET                WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 6)    /**<  This command is used to set the display margin of ProgressBar's body. 

                                                                                                                                                                                         related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                         pv_param1:   Pointer to a WGL_INSET_T variable that contains body margins. It cannot be NULL.

                                                                                                                                                                                         pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                         */
#define WGL_CMD_PG_GET_CNT_INSET                WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 7)    /**<   This command returns the display margin of ProgressBar's body.

                                                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                        pv_param1:   Pointer to a WGL_INSET_T variable to receive body margins. It cannot be NULL.

                                                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                        */
#define WGL_CMD_PG_SET_LEVELS                   WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 8)    /**<     This command is used to set levels info. 

                                                                                                                                                                                         related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                         pv_param1:   Pointer to a WGL_PG_LEVELS_INFO_T variable that contains body margins. It cannot be NULL.

                                                                                                                                                                                         pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                         */
#define WGL_CMD_PG_GET_LEVELS                   WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 9)    /**<    This command returns the levels info.

                                                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                        pv_param1:   Pointer to a WGL_PG_LEVELS_INFO_T variable to receive level infos. It cannot be NULL.

                                                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                        */
#define WGL_CMD_PG_SET_BAR_INSET                WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 10)    /**<  This command is used to set the display margin of ProgressBar's bar. The i4_left is for the left margin, the i4_right is for the right margin, the i4_top is for the top margin, and the i4_bottom is for the bottom margin. All of them cannot be less than 0, and they're relative to ProgressBar's body.

                                                                                                                                                                                         related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                         pv_param1:   Pointer to a WGL_INSET_T variable that contains bar margins. It cannot be NULL.

                                                                                                                                                                                         pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                         */
#define WGL_CMD_PG_GET_BAR_INSET                WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 11)    /**< This command returns the display margin of ProgressBar's bar.

                                                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                        pv_param1:   Pointer to a WGL_INSET_T variable to receive bar margins. It cannot be NULL.

                                                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                        */
#define WGL_CMD_PG_SET_IDR_LOC                  WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 12)    /**<This command sets indicators' locations.

                                                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                        pv_param1:   A WGL_PG_CNT_IDR_LOC_TYPE_T value to indicate the target  indicator.

                                                                                                                                                                                        pv_param2:   Pointer to a WGL_PG_IDR_LOC_T variable that contains the settings for the target indicator. If it is NULL, it will use the default settings.
                                                                                                                                                                                        */
#define WGL_CMD_PG_GET_IDR_LOC                  WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 13)    /**< This command returns indicators' locations

                                                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                        pv_param1:   A WGL_PG_CNT_IDR_LOC_TYPE_T value to indicate the interested indicator.

                                                                                                                                                                                        pv_param2:   Pointer to a WGL_PG_IDR_LOC_T variable to receive the settings. It cannot be NULL.
                                                                                                                                                                                        */
#define WGL_CMD_PG_SET_BAR_MIN_SIZE             WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 14)    /**<  This command is used to set bar minimum size. 

                                                                                                                                                                                         related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                         pv_param1:   A UINT16 value specifies the bar minimum size.

                                                                                                                                                                                         pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                         */
#define WGL_CMD_PG_GET_BAR_MIN_SIZE             WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 15)    /**<   This command returns bar minimum size. 

                                                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                        pv_param1:   Pointer to a UINT16 value

                                                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                        */
#define WGL_CMD_PG_SET_CURSOR_W_H               WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 16)    /**<This command is used to set cursor size. 

                                                                                                                                                                                         related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                         pv_param1:   Pointer to a GL_SIZE_T variable that contains cursor size.

                                                                                                                                                                                         pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                         */
#define WGL_CMD_PG_GET_CURSOR_W_H               WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 17)    /**< This command returns cursor size. 

                                                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                        pv_param1:   Pointer to a GL_SIZE_T value to receive cursor size

                                                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                        */
#define WGL_CMD_PG_SET_CURSOR_INSET              WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 18)    /**< This command is used to set cursor inset. 

                                                                                                                                                                                         related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                         pv_param1:   Pointer to a WGL_INSET_T value to contain cursor inset

                                                                                                                                                                                         pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                         */
#define WGL_CMD_PG_GET_CURSOR_INSET              WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 19)    /**< This command returns cursor inset. 

                                                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                        pv_param1:   Pointer to a WGL_INSET_T value to receive cursor inset

                                                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                        */
#define WGL_CMD_PG_SET_STEP_SIZE                 WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 20) /**< This command returns cursor inset. 

                                                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                        pv_param1:   Pointer to a UINT8 value

                                                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                        */                                                                                                                                                                                       
#define WGL_CMD_PG_GET_STEP_SIZE                 WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_PROGRESS_BAR, 21) /**< This command returns cursor inset. 

                                                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                                                        pv_param1:   Pointer to a UINT8 value

                                                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                                                        */

#define PG_CNT_PRIVATE_START                    22         /**< (WGL_CMD_PG_GET_BAR_MIN_SIZE - WGL_CMD_PG_SET_DISPLAY_MODE + 1)       */


/*------------------------------------------------------------------*/
/*! @enum WGL_PG_CNT_IDR_LOC_TYPE_T
 *  @brief  target for WGL_CMD_PG_SET_IDR_LOC and WGL_CMD_PG_GET_IDR_LOC
 *  @code
 *  typedef enum _WGL_PG_CNT_IDR_LOC_TYPE_T
 *  {
 *      WGL_PG_CNT_ILT_MAX = 0,
 *      WGL_PG_CNT_ILT_MIN,
 *      WGL_PG_CNT_ILT_POS
 *  } WGL_PG_CNT_IDR_LOC_TYPE_T;
 *  @endcode
 *  @li@c  WGL_PG_CNT_ILT_MAX                                -indicator locator type is show maximum value
 *  @li@c  WGL_PG_CNT_ILT_MIN                                -indicator locator type is show minimum value
 *  @li@c  WGL_PG_CNT_ILT_POS                                -indicator locator type is show current postion value
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_PG_CNT_IDR_LOC_TYPE_T
{
    WGL_PG_CNT_ILT_MAX = 0,
    WGL_PG_CNT_ILT_MIN,
    WGL_PG_CNT_ILT_POS
} WGL_PG_CNT_IDR_LOC_TYPE_T;

/*Specific Notify Code*/
#define WGL_NC_PG_POS_CHANGE                    ((UINT32)WGL_NC_WIDGET_SPF_START + 0)    /**<This notification is sent when the current position is changed. The change is caused by user's keyboard/RC input.         */


/*Other*/
#define WGL_PG_LEVEL_MAX                        ((UINT8) 5)    /**<   maximum level number     */

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*! @struct WGL_RANGE_T
 *  @brief value range
 *  @code
 *  typedef struct _WGL_RANGE_T
 *  {
 *          INT32           i4_min;
 *          INT32           i4_max;
 *  } WGL_RANGE_T;
 *  @endcode
 *  @li@c  i4_min                                            -minimum value in range
 *  @li@c  i4_max                                            -maximum value in range
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_RANGE_T
{
        INT32           i4_min;
        INT32           i4_max;
} WGL_RANGE_T;


/*------------------------------------------------------------------*/
/*! @struct WGL_PG_LEVELS_INFO_T
 *  @brief  Level Infomation
 *  @code
 *  typedef struct _WGL_PG_LEVELS_INFO_T
 *  {
 *          UINT8           ui1_level_number;
 *          INT32           a_boundary[WGL_PG_LEVEL_MAX - 1];
 *  } WGL_PG_LEVELS_INFO_T;
 *  @endcode
 *  @li@c  ui1_level_number                                  - level number whose valid value is in range(0,WGL_PG_LEVEL_MAX)
 *  @li@c  a_boundary[WGL_PG_LEVEL_MAX - 1]                  -level data array
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_PG_LEVELS_INFO_T
{
        UINT8           ui1_level_number;
        INT32           a_boundary[WGL_PG_LEVEL_MAX - 1];
} WGL_PG_LEVELS_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct  WGL_PG_KEY_MAP_T
 *  @brief Control Key Map
 *  @code
 *  typedef struct _WGL_PG_KEY_MAP_T
 *  {
 *          UINT32          ui4_pos_up;
 *          UINT32          ui4_pos_down;
 *          UINT32          ui4_step_up;
 *          UINT32          ui4_step_down;
 *  } WGL_PG_KEY_MAP_T;
 *  @endcode
 *  @li@c  ui4_pos_up                                        - key code to incress one position 
 *  @li@c  ui4_pos_down                                      -key code to decress one position 
 *  @li@c  ui4_step_up                                       -key code to incress one step
 *  @li@c  ui4_step_down                                     -key code to decress one step
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_PG_KEY_MAP_T
{
        UINT32          ui4_pos_up;
        UINT32          ui4_pos_down;
        UINT32          ui4_step_up;
        UINT32          ui4_step_down;
} WGL_PG_KEY_MAP_T;


/*------------------------------------------------------------------*/
/*! @struct WGL_PG_FAST_MODE_ATTR_T
 *  @brief  Fast Mode Attribute
 *  @code
 *  typedef struct _WGL_PG_FAST_MODE_ATTR_T
 *  {
 *      UINT16              ui2_key_threshold;              
 *      UINT16              ui2_first_lvl_trigger_cnt;      
 *      UINT16              ui2_first_lvl_increment;        
 *      UINT16              ui2_second_lvl_trigger_cnt;     
 *      UINT16              ui2_second_lvl_increment;       
 *  } WGL_PG_FAST_MODE_ATTR_T;
 *  @endcode
 *  @li@c  ui2_key_threshold                                 -key code to threshold in ticks, default is 40
 *  @li@c  ui2_first_lvl_trigger_cnt                         - first level to trigger count default is 3
 *  @li@c  ui2_first_lvl_increment                           - first level to increment value default is 3
 *  @li@c  ui2_second_lvl_trigger_cnt                        -second level to trigger count  default is 6
 *  @li@c  ui2_second_lvl_increment                          -second level to increment value default is 6
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_PG_FAST_MODE_ATTR_T
{
    UINT16              ui2_key_threshold;              
    UINT16              ui2_first_lvl_trigger_cnt;     
    UINT16              ui2_first_lvl_increment;        /*default is 3*/
    UINT16              ui2_second_lvl_trigger_cnt;     /*default is 6*/
    UINT16              ui2_second_lvl_increment;       /*default is 6*/
} WGL_PG_FAST_MODE_ATTR_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_PG_IDR_LOC_T
 *  @brief progress indicator locator info
 *  @code
 *  typedef struct _WGL_PG_IDR_LOC_T
 *  {
 *      GL_RECT_T           t_loc_rect;
 *      UINT8               ui1_align;
 *  } WGL_PG_IDR_LOC_T;
 *  @endcode
 *  @li@c  t_loc_rect                                        -indicator locator region
 *  @li@c  ui1_align                                         -indicator loactor alignment according to progress bar
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_PG_IDR_LOC_T
{
    GL_RECT_T           t_loc_rect;
    UINT8               ui1_align;
} WGL_PG_IDR_LOC_T;

/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/



#ifdef __cplusplus
}
#endif

#endif
/*----------------------------------------------------------------------------*/
/*! @} */   /* end of groupMW_WGL_PROGRESS*/
/*----------------------------------------------------------------------------*/

