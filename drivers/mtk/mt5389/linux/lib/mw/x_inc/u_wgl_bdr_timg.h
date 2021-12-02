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
/*! @file u_wgl_bdr_timg.h 
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
/*! @defgroup groupMW_WGL_BDR_TIMG   Bord Template Image Widget  
 *
 *  @ingroup groupMW_WGL
 *  @brief Provides definitions and exported function of the widget bord template image library.
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW_WGL
 *  @{ 
 */
/*----------------------------------------------------------------------------*/
 
#ifndef _U_WGL_BDR_TIMG_H_
#define _U_WGL_BDR_TIMG_H_
 
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
/* specific border commands for template image border */

/**This command is to set template image border style. 

Input arguments

pv_param1      -Contains the styles of the border(UINT32).

Returns: 

WGLR_OK         -Routine successful.

WGLR_INV_ARG   -The styles combination is invalid
*/

#define WGL_CMD_BDR_TIMG_SET_STYLE              WGL_MAKE_BORDER_CMD(WGL_BORDER_TIMG, 0)   

/** This command is to get template image border style.

Output arguments:

pv_param1      -Contains the styles of the border(UINT32)

 Returns: 

WGLR_OK         -Routine successful.

WGLR_INV_ARG   -The styles combination is invalid
*/
#define WGL_CMD_BDR_TIMG_GET_STYLE              WGL_MAKE_BORDER_CMD(WGL_BORDER_TIMG, 1)   

/** This command is to set template image background image of special border. 

Input arguments

pv_param1       -Contains the background image set(WGL_IMG_INFO_T *).

Returns: 

WGLR_OK            -Routine successful.

WGLR_INV_ARG   -The image set information is invalid
*/
#define WGL_CMD_BDR_TIMG_SET_BK_IMG             WGL_MAKE_BORDER_CMD(WGL_BORDER_TIMG, 2)   

/** This command is to get template image background image of special border.

Output arguments

pv_param1       -Contains the background image set(WGL_IMG_INFO_T *).

Returns:

WGLR_OK            -Routine successful.

WGLR_INV_ARG   -The image set information is invalid
*/
#define WGL_CMD_BDR_TIMG_GET_BK_IMG             WGL_MAKE_BORDER_CMD(WGL_BORDER_TIMG, 3)   

/**  This command is to set template image title information of special border. 

Input arguments

pv_param1   - Contains the title infomation(WGL_BDR_TIMG_TITLE_INFO_T *).

Returns:

WGLR_OK            -Routine successful.

WGLR_INV_ARG   -The image titile set information is invalid
*/
#define WGL_CMD_BDR_TIMG_SET_TITLE_INFO         WGL_MAKE_BORDER_CMD(WGL_BORDER_TIMG, 4)   

/**This command is to get template image title information of special border.

Output arguments

pv_param1   - Contains the title infomation(WGL_BDR_TIMG_TITLE_INFO_T *).

Returns: 

WGLR_OK            -Routine successful.

WGLR_INV_ARG   -The image titile set information is invalid
*/
#define WGL_CMD_BDR_TIMG_GET_TITLE_INFO         WGL_MAKE_BORDER_CMD(WGL_BORDER_TIMG, 5)   

/**This command is to set template image title text information of special border. 

Input arguments

pv_param1   -(UTF16_T *) Contains the UTF16_T string of the title text,.If NULL, the title text will be cleared.

pv_param2   (UINT16) Contains the string length. If 0, the title text will be cleared.

Returns: 

WGLR_OK       -Routine successful.

WGLR_INV_ARG   -pv_param1 is NULL but pv_param2 is not 0 or <0; Other errors
*/
#define WGL_CMD_BDR_TIMG_SET_TITLE_TEXT         WGL_MAKE_BORDER_CMD(WGL_BORDER_TIMG, 6)   

/**This command is to get template image title text information of special border.

Output arguments

pv_param1   -(UTF16_T *) Contains the UTF16_T string of the title text,.If NULL, the title text will be cleared.

pv_param2   (UINT16) Contains the string length. If 0, the title text will be cleared.

Returns: 

WGLR_OK                 -Routine successful.

WGLR_INV_ARG         -The pv_param1 is NULL

WGLR_BUFFER_NOT_ENOUGH     -The buffer is not big enough
*/
#define WGL_CMD_BDR_TIMG_GET_TITLE_TEXT         WGL_MAKE_BORDER_CMD(WGL_BORDER_TIMG, 7)   

/**This command is to set template image icon information of special border. 

Input arguments

pv_param1   -(WGL_BDR_TIMG_ICON_INFO_T *) Contains the icon info

Returns: 

WGLR_OK       -Routine successful.

WGLR_INV_ARG    -(1)pv_param1 is NULL, (2) the icon info is invalid
*/
#define WGL_CMD_BDR_TIMG_SET_ICON_INFO          WGL_MAKE_BORDER_CMD(WGL_BORDER_TIMG, 8)   

/**This command is to get template image icon information of special border. 

Output arguments

pv_param1   -(WGL_BDR_TIMG_ICON_INFO_T *) Contains the icon info

Returns:

WGLR_OK         Routine successful.

WGLR_INV_ARG    The pv_param1 is NULL
*/
#define WGL_CMD_BDR_TIMG_GET_ICON_INFO          WGL_MAKE_BORDER_CMD(WGL_BORDER_TIMG, 9)   

/**
This command is to set template image icon.

Input arguments

pv_param1   -(WGL_IMG_INFO_T *) Contains the image info of the icon
                       if NULL, the image info of the icon will be cleared
 
Output arguments

 Returns:

 WGLR_OK         Routine successful.

 WGLR_INV_ARG    The image info is invalid
 */
#define WGL_CMD_BDR_TIMG_SET_ICON               WGL_MAKE_BORDER_CMD(WGL_BORDER_TIMG, 10)  

/**This command is to get template image icon.

Input arguments

pv_param1      -(WGL_IMG_INFO_T *) Contains the image info of the icon.
                              if NULL, the image info of the icon will be cleared
Returns: 

WGLR_OK         Routine successful.

WGLR_INV_ARG    The image info is invalid
*/
#define WGL_CMD_BDR_TIMG_GET_ICON               WGL_MAKE_BORDER_CMD(WGL_BORDER_TIMG, 11)  


/**  This command is to set external template image title information of special border,it will used
when this bord titile is focused.

Input arguments

pv_param1   - Contains the title infomation(WGL_BDR_TIMG_TITLE_INFO_T *).

Returns:

WGLR_OK            -Routine successful.

WGLR_INV_ARG   -The image titile set information is invalid
*/
#define WGL_CMD_BDR_TIMG_SET_TITLE_INFO_EX         WGL_MAKE_BORDER_CMD(WGL_BORDER_TIMG, 12)   


/**It specific text color targets for template image border .

Input arguments:
        None.
        
Output arguments:
        None.
        
Returns:WGLR_OK             -
*/
#define WGL_CLR_BDR_TIMG_TITLE_TEXT             (WGL_CLR_WGT_SPF_START+21)   

/**It specific text background color targets for template image border .

Input arguments
        None.

Output arguments
        None.

Returns:WGLR_OK             -
*/
#define WGL_CLR_BDR_TIMG_TITLE_TEXT_BK          (WGL_CLR_WGT_SPF_START+22)   

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
/* styles */
#define WGL_STL_BDR_TIMG_NO_IMG_UI              MAKE_BIT_MASK_32(0)    /**< The style indicate that border has no image.*/
#define WGL_STL_BDR_TIMG_NO_BK                  MAKE_BIT_MASK_32(1)    /**<The style indicate that border has no background.*/
#define WGL_STL_BDR_TIMG_NO_SMART_CUT           MAKE_BIT_MASK_32(3)    /**<The style indicate that title need no smart cut when too long.*/

#define WGL_STL_BDR_TIMG_LAST_STYLE             WGL_STL_BDR_TIMG_NO_BK    /**<The last style.*/

/* title information */
#define WGL_BDR_TIMG_TITLE_STL_TRANSP_BK        MAKE_BIT_MASK_16(0)    /**<It iddicate thate title of template image border uses transparent background style.*/
#define WGL_BDR_TIMG_TITLE_STL_LAST_STYLE       WGL_BDR_TIMG_TITLE_STL_TRANSP_BK    /**<It indcate the last style.*/
/*------------------------------------------------------------------*/
/*! @struct  WGL_BDR_TIMG_TITLE_INFO_T
 *  @brief This struct contain title information of template image border.
 *  @code
 *  typedef struct _WGL_BDR_TIMG_TITLE_INFO_T
 *  {
 *      UINT16                  ui2_style;
 *      WGL_FONT_INFO_T         t_font;
 *      GL_RECT_T               t_rc_ref;
 *      UINT8                   ui1_align;
 *  } WGL_BDR_TIMG_TITLE_INFO_T;
 *  @endcode
 *  @li@c  ui2_style                       -Contain the style of template image border
 *  @li@c  t_font                           -Contain the font information.
 *  @li@c  t_rc_ref                         -The resource reference region.
 *  @li@c  ui1_align                       -Contain align information.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_BDR_TIMG_TITLE_INFO_T
{
    UINT16                  ui2_style;
    WGL_FONT_INFO_T         t_font;
    GL_RECT_T               t_rc_ref;
    UINT8                   ui1_align;
    
} WGL_BDR_TIMG_TITLE_INFO_T;

/* icon information */
/*------------------------------------------------------------------*/
/*! @struct WGL_BDR_TIMG_ICON_INFO_T
 *  @brief This strcut contains icon information of template image border.
 *  @code
 *  typedef struct _WGL_BDR_TIMG_ICON_INFO_T
 *  {
 *      GL_RECT_T               t_rc_ref;
 *      UINT8                   ui1_align;
 *  } WGL_BDR_TIMG_ICON_INFO_T;    
 *  @endcode
 *  @li@c  t_rc_ref                   -Contain the resource reference reggion information.
 *  @li@c  ui1_align                 -Contains align information.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_BDR_TIMG_ICON_INFO_T
{
    GL_RECT_T               t_rc_ref;
    UINT8                   ui1_align;
} WGL_BDR_TIMG_ICON_INFO_T;    

#ifdef __cplusplus
}
#endif
                         
#endif
/*----------------------------------------------------------------------------*/
/*! @} */   /* end of groupMW_WGL_BDR_TIMG*/
/*----------------------------------------------------------------------------*/


