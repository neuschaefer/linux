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
/*! @file u_wgl_bdr_common.h 
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

 
#ifndef _U_WGL_BDR_COMMON_H_
#define _U_WGL_BDR_COMMON_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_wgl_common.h"


/*-----------------------------------------------------------------------------
                    general command definition
 ----------------------------------------------------------------------------*/
/**
This command sets the inset of the specified border. If the inputted inset is too large to fit 
into the widget, or the inputted inset is too small for the border, the command fails.

Input arguments

pv_param1	References a WGL_INSET_T structure. This argument must be of type (WGL_INSET_T *). A 
NULL value of this argument indicates the default inset of this border will be used.

pv_param2	Not used. This argument must be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The inputted inset is not valid.
*/
#define WGL_CMD_GL_SET_INSET                    WGL_MAKE_BORDER_CMD(WGL_BORDER_BASE, 0)    

/**
This command gets the inset of the specified border.

Input arguments

pv_param2	Not used. This argument must be NULL.

Output arguments

pv_param1	Returns the inset of the border. This argument must be of type (WGL_INSET_T *). This 
argument cannot be NULL.

Returns

WGLR_OK	The command is successful.
*/
#define WGL_CMD_GL_GET_INSET                    WGL_MAKE_BORDER_CMD(WGL_BORDER_BASE, 1)    

/**
This command queries the minimum inset of the specified border.

Input arguments

pv_param2	Not used. This argument must be NULL.

Output arguments

pv_param1	Returns the minimum inset of the border. This argument must be of type (WGL_INSET_T *). 
This argument cannot be NULL.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	pv_param1 is NULL.
*/
#define WGL_CMD_GL_GET_MIN_INSET                WGL_MAKE_BORDER_CMD(WGL_BORDER_BASE, 2)    

/**
This command sets a BOOL flag indicating whether the border draws the interior part or not. The 
interior part is the portion of a widget that content occupy. 

Input arguments

pv_param1	Contains a BOOL variable indicating whether the border draws interior or not. This 
argument must be of type BOOL.

pv_param2	Not used. This argument must be NULL.

Returns

WGLR_OK	The command is successful.
*/
#define WGL_CMD_GL_SET_IS_BDR_DRAW_INTERIOR     WGL_MAKE_BORDER_CMD(WGL_BORDER_BASE, 3)    

/**
This command gets a BOOL flag indicating whether the border draws the interior part or not. The 
interior part is the portion of a widget that content occupy. 

Input arguments

pv_param2	Not used. This argument must be NULL.

Output arguments

pv_param1	Returns a BOOL variable indicating whether the border draws interior or not. This 
argument must be of type (BOOL *). This argument cannot be NULL.

Returns

WGLR_OK	The command is successful.
*/
#define WGL_CMD_GL_GET_IS_BDR_DRAW_INTERIOR     WGL_MAKE_BORDER_CMD(WGL_BORDER_BASE, 4)    

/**
This command sets the foreground color, background color or other type of color defined in 
specific border. The type of color to be set is indicated in pv_param1. The type of color-set and 
the value of the color-set are specified in pv_param2, which is the pointer of the structure 
WGL_COLOR_INFO_T. Note that the choice of the color-set (WGL_COLOR_SET_BASIC, 
WGL_COLOR_SET_STANDARD or WGL_COLOR_SET_EXTEND) depends on which type of the border's host widget
. For example, if the border's host widget is an Icon widget, which has three states: disabled, 
enabled, and focused, it's strongly recommended to choose the color-set type 
WGL_COLOR_SET_STANDARD. If the border's host widget is a Button widget, which has an additional 
state: pushed state, it's recommended to choose the color-set type WGL_COLOR_SET_EXTEND. 

Input arguments

pv_param1	Contains the target type of color to be set. Some available color targets are such as 
WGL_CLR_BDR_BK and WGL_CLR_BDR_FG. More color targets are independently available for each type 
of border. For example, "uniform border" has additional specific color targets such as 
WGL_CLR_BDR_UNFM_INNER_BK, WGL_CLR_BDR_UNFM_TITLE_TEXT, or WGL_CLR_BDR_UNFM_TITLE_TEXT_BK. Note 
that the specific color targets can only be applied on the corresponding border component. For 
more information about the available target color types that are general for all types of borders
, refer to the section "General Border Color Command Target". For the specific color targets for 
each border, refer to the "Border Color Command Target" section in each border's documentation.

pv_param2	Contains the color information. This argument must be of type (WGL_COLOR_INFO_T *). If 
this argument is NULL, the default color will be used. For more information about the structure 
WGL_COLOR_INFO_T, refer to the document "SW Design Middleware - WGL Common Widget". Note that the 
choice of the color-set (WGL_COLOR_SET_BASIC, WGL_COLOR_SET_STANDARD or WGL_COLOR_SET_EXTEND) 
depends on the type of the border's host widget.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The target type of color specified in argument pv_param1 is invalid.
*/
#define WGL_CMD_GL_SET_BDR_COLOR                WGL_MAKE_BORDER_CMD(WGL_BORDER_BASE, 5)  

/**
This command queries the foreground color, background color or other type of color defined in 
specific border. The type of color to be queried is indicated in pv_param1. 

Input arguments

pv_param1	Contains the target type of color to be queried. Some available color targets are such 
as WGL_CLR_BDR_BK and WGL_CLR_BDR_FG. More color targets are independently available for each 
type of border. For example, "uniform border" has additional specific color targets such as 
WGL_CLR_BDR_UNFM_INNER_BK, WGL_CLR_BDR_UNFM_TITLE_TEXT, or WGL_CLR_BDR_UNFM_TITLE_TEXT_BK. Note 
that the specific color targets can only be applied on the corresponding border component. For 
more information about the available target color types that are general for all types of borders
, refer to the section "General Border Color Command Target". For the specific color targets for 
each border, refer to the "Border Color Command Target" section in each border's documentation.

Output arguments

pv_param2	Returns the color information. This argument must be of type (WGL_COLOR_INFO_T *). This 
argument cannot be NULL. For more information about the structure WGL_COLOR_INFO_T, refer to the 
document "SW Design Middleware - WGL Common Widget".

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	At least one of the following cases occurs:
 	The target type of color specified in argument pv_param1 is invalid
 	The argument pv_param2 is NULL.
*/
#define WGL_CMD_GL_GET_BDR_COLOR                WGL_MAKE_BORDER_CMD(WGL_BORDER_BASE, 6)  

/**
This command sets the text direction attribute that defined in specific border. It indicates the 
border text direction (WGL_TEXT_DIR_BDR_LEFT_TO_RIGHT or WGL_TEXT_DIR_BDR_RIGHT_TO_LEFT)

Input arguments

pv_param1	Contains the text direction value.It maybe WGL_TEXT_DIR_BDR_LEFT_TO_RIGHT or 
WGL_TEXT_DIR_BDR_RIGHT_TO_LEFT.

pv_param2	Reserve.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The text direction is not WGL_TEXT_DIR_BDR_LEFT_TO_RIGHT or 

WGL_TEXT_DIR_BDR_RIGHT_TO_LEFT.
*/
#define WGL_CMD_GL_SET_BDR_TEXT_DIRECTION       WGL_MAKE_BORDER_CMD(WGL_BORDER_BASE, 7)  

/**
This command gets the text direction attribute that defined in specific border. 

Output arguments

pv_param1	Contains the text direction value.It maybe WGL_TEXT_DIR_BDR_LEFT_TO_RIGHT or 
WGL_TEXT_DIR_BDR_RIGHT_TO_LEFT.

pv_param2	Reserve.

Returns

WGLR_OK	The command is successful.

WGLR_INV_ARG	The point is null.
*/
#define WGL_CMD_GL_GET_BDR_TEXT_DIRECTION       WGL_MAKE_BORDER_CMD(WGL_BORDER_BASE, 8)  

/*-----------------------------------------------------------------------------
                    Color Command Target definition
 ----------------------------------------------------------------------------*/
#define WGL_CLR_BDR_BK          (WGL_CLR_WGT_SPF_START+1)    /**< Indicates the color target is the outer border area of Uniform Border. */
#define WGL_CLR_BDR_FG          (WGL_CLR_WGT_SPF_START+2)    /**< Indicates the color target is the border body of Uniform Border. */

/*-----------------------------------------------------------------------------
                   Text Direction Command Target definition
 ----------------------------------------------------------------------------*/
#define WGL_TEXT_DIR_BDR_LEFT_TO_RIGHT  0x01    /**<Define the text direction si from left to right of special border. */
#define WGL_TEXT_DIR_BDR_RIGHT_TO_LEFT  0x02    /**<Define the text direction si from right to left of special border. */

#endif




/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_WGL*/
/*----------------------------------------------------------------------------*/



