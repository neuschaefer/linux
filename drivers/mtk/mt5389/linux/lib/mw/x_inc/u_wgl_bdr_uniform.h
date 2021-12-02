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
/*! @file u_wgl_bdr_uniform.h 
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
/*! @defgroup groupMW_WGL_BDR_UNIFORM   Bord Uniform Widget 
 *
 *  @ingroup groupMW_WGL
 *  @brief Provides definitions and exported function of the widget bord uniform library.
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW_WGL
 *  @{ 
 */
/*----------------------------------------------------------------------------*/



#ifndef _U_WGL_BDR_UNIFORM_H_
#define _U_WGL_BDR_UNIFORM_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_wgl_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* specific border commands for uniform border */
/**
This command sets the thickness of the border body.

Input arguments

pv_param1	Specifies the thickness of the border body. This argument must be of type UINT16. Note 
that the thickness can be zero and if the thickness is zero, no border body will be rendered. 

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.
*/
#define WGL_CMD_BDR_UNFM_SET_THICKNESS          WGL_MAKE_BORDER_CMD(WGL_BORDER_UNIFORM, 0) 

/**
This command retrieves the thickness of the border body.

Input arguments

pv_param2	Reserved. It must be NULL.

Output arguments

pv_param1	Contains the pointer of a single UINT16 buffer to receive the border body's thickness. 
Thus this argument must be of type (UINT16 *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
*/
#define WGL_CMD_BDR_UNFM_GET_THICKNESS          WGL_MAKE_BORDER_CMD(WGL_BORDER_UNIFORM, 1) 

/**
This command sets the margin of the inner border area. Refer to 7 for more information about 
inner border area.

Input arguments

pv_param1	Specifies the margin of the inner border area. This argument must be of type (
WGL_BDR_UNFM_MARGIN_T *) and cannot be NULL. 

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
*/
#define WGL_CMD_BDR_UNFM_SET_MARGIN             WGL_MAKE_BORDER_CMD(WGL_BORDER_UNIFORM, 2) 

/**
This command retrieves the margin of the inner border area. Refer to 7 for more information about 
inner border area.

Input arguments

pv_param2	Reserved. It must be NULL.
Output arguments

pv_param1	Contains the pointer of a single WGL_BDR_UNFM_MARGIN_T buffer to receive the margin of 
the inner border area. Thus this argument must be of type (WGL_BDR_UNFM_MARGIN_T *) and cannot be 
NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
*/
#define WGL_CMD_BDR_UNFM_GET_MARGIN             WGL_MAKE_BORDER_CMD(WGL_BORDER_UNIFORM, 3) 

/**
This command sets the attributes of border title. 

Input arguments

pv_param1	Specifies the attributes of the border title. This argument must be of type (
WGL_BDR_UNFM_TITLE_INFO_T *) and cannot be NULL.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	At least in one of the following cases:
 1. The argument pv_param1 is NULL.
 2. The Title Styles is invalid
 3. The Font cannot be created
 4. The Reference Rectangle is not a valid rectangle
 5. The specified alignment mode is invalid
 */
#define WGL_CMD_BDR_UNFM_SET_TITLE_INFO         WGL_MAKE_BORDER_CMD(WGL_BORDER_UNIFORM, 4) 

/**
This command retrieves the attributes of the border title.

Input arguments

pv_param2	Reserved. It must be NULL.

Output arguments

pv_param1	Contains the pointer of a single WGL_BDR_UNFM_TITLE_INFO_T buffer to receive the border 
title's attributes. Thus this argument must be of type (WGL_BDR_UNFM_TITLE_INFO_T *) and cannot 
be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
*/
#define WGL_CMD_BDR_UNFM_GET_TITLE_INFO         WGL_MAKE_BORDER_CMD(WGL_BORDER_UNIFORM, 5) 

/**
This command sets the text of the title.

Input arguments

pv_param1	Specifies the UTF16_T string of the title text. This argument must be of type (UTF16_T *
). If this argument is NULL, the title text will be cleared and in this case the text length 
specified in pv_param2 must be zero.

pv_param2	Specifies the length of the title text counted in UTF16_T character unit. This argument 
must be of type UINT16. If this argument is zero, the title text will be cleared.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL but the text length specified in pv_param2 is not zero
. 
*/
#define WGL_CMD_BDR_UNFM_SET_TITLE_TEXT         WGL_MAKE_BORDER_CMD(WGL_BORDER_UNIFORM, 6) 

/** 
This command retrieves the text of the border title.

Input arguments

pv_param2	Contains the size of the UTF16_T string buffer specified in the argument pv_param1. 
Note that the size of the buffer is counted in UTF16_T character unit. The argument must be of 
type UINT16.

Output arguments

pv_param1	Contains the pointer of a UTF16_T buffer to receive the title text. Thus this argument 
must be of type (UTF16_T *) and cannot be NULL. The size of the buffer must be specified in the 
argument pv_param2. 

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.

WGLR_BUFFER_NOT_ENOUGH   -The size of the buffer is not big enough for receiving the title text.
*/
#define WGL_CMD_BDR_UNFM_GET_TITLE_TEXT         WGL_MAKE_BORDER_CMD(WGL_BORDER_UNIFORM, 7) 


/* specific color targets for uniform border */
/**Indicates that the color target is the inner border area's color. */
#define WGL_CLR_BDR_UNFM_INNER_BK               (WGL_CLR_WGT_SPF_START+10)  

/**Indicates that the color target is the text of title. */
#define WGL_CLR_BDR_UNFM_TITLE_TEXT             (WGL_CLR_WGT_SPF_START+11)  

/**Indicates that the color target is the text background of title. */
#define WGL_CLR_BDR_UNFM_TITLE_TEXT_BK          (WGL_CLR_WGT_SPF_START+12)  

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
/* uniform border margin */
/*------------------------------------------------------------------*/
/*! @struct WGL_BDR_UNFM_MARGIN_T
 *  @brief This structure holds the margin reserved for Uniform Border's inner border area.
 *  @code
 *  typedef struct _WGL_BDR_UNFM_MARGIN_T 
 *  {
 *      INT32           i4_left;
 *      INT32           i4_top;
 *      INT32           i4_right;
 *      INT32           i4_bottom;
 *  } WGL_BDR_UNFM_MARGIN_T;    
 *  @endcode
 *  @li@c  i4_left                                           -Contains the left margin for inner border area.
 *  @li@c  i4_top                                            -Contains the top margin for inner border area.
 *  @li@c  i4_right                                          -Contains the right margin for inner border area.
 *  @li@c  i4_bottom                                         -Contains the bottom margin for inner border area.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_BDR_UNFM_MARGIN_T 
{
    INT32           i4_left;
    INT32           i4_top;
    INT32           i4_right;
    INT32           i4_bottom;
} WGL_BDR_UNFM_MARGIN_T;    

/* title information */
/**Indicates that the title has no text background. If this style is specified, only the text will 
be rendered and the text background is completely transparent. */
#define WGL_BDR_UNFM_TITLE_STL_TRANSP_BK        (((UINT16)1)<<0)   
/**It is last style.*/
#define WGL_BDR_UNFM_TITLE_STL_LAST_STYLE       WGL_BDR_UNFM_TITLE_STL_TRANSP_BK  
/*------------------------------------------------------------------*/
/*! @struct WGL_BDR_UNFM_TITLE_INFO_T
 *  @brief This structure holds the attributes of title, such as styles, font, reference rectangle and 
alignment of the title.
 *  @code
 *  typedef struct _WGL_BDR_UNFM_TITLE_INFO_T
 *  {
 *      UINT16                  ui2_style;
 *      WGL_FONT_INFO_T         t_font;
 *      GL_RECT_T               t_rc_ref;
 *      UINT8                   ui1_align;
 *  } WGL_BDR_UNFM_TITLE_INFO_T;
 *  @endcode
 *  @li@c  ui2_style                                         -Contains the styles of the title. 
 *  @li@c  t_font                                            -Contains the font information of the title. 
 *  @li@c  t_rc_ref                                          -Contains the reference rectangle for automatically positioning the title.
 *  @li@c  ui1_align                                         -Contains the alignment mode of the title.  
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_BDR_UNFM_TITLE_INFO_T
{
    UINT16                  ui2_style;
    WGL_FONT_INFO_T         t_font;
    GL_RECT_T               t_rc_ref;
    UINT8                   ui1_align;
    
} WGL_BDR_UNFM_TITLE_INFO_T;




#ifdef __cplusplus
}
#endif
                         
#endif

/*----------------------------------------------------------------------------*/
/*! @} */   /* end of groupMW_WGL_BDR_UNIFORM*/
/*----------------------------------------------------------------------------*/


