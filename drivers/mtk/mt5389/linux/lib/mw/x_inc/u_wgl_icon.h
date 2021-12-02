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
/*! @file u_wgl_icon.h 
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
/*! @defgroup groupMW_WGL_ICON   Icon Widget  
 *
 *  @ingroup groupMW_WGL
 *  @brief Provides definitions and exported function of the widget icon library.
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW_WGL
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

 
#ifndef _U_WGL_ICON_H_
#define _U_WGL_ICON_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_wgl.h"
#include "u_wgl_common.h"
#include "u_wgl_tools.h"


#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* Specific ICON style */
#define WGL_STL_ICON_MULTI_IMG                  ((UINT32) WGL_STL_WGT_SPF_START)    /**< If this style is assigned, it means the Icon Static will have multiple foreground images.*/

/* Specific Command */
/**
This command is used to get minimum widget size, which includes minimum sizes of content and 
border. Note: this command isn't supported in this version.

Input arguments

pv_param1	Pointer of a GL_SIZE_T to receive the size. It cannot be NULL.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
*/
#define WGL_CMD_ICON_GET_MIN_SIZE               WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_ICON, 0)    


/* Specific Command Supported by ICON-Content */
/**
This command is used to set Icon Static's foreground alignment.

Input arguments

pv_param1	Specifies the alignment style. It can be from WGL_AS_LEFT_TOP to WGL_AS_RIGHT_BOTTOM. 
About the explanation of these alignment styles.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The style specified by pv_param1 is invalid.
*/
#define WGL_CMD_ICON_SET_ALIGN                  WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_ICON, 0)   

/**
This command returns the foreground's alignment of an Icon Static.

Input arguments

pv_param1	Pointer to a UINT8 to receive its alignment style. It cannot be NULL.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
*/
#define WGL_CMD_ICON_GET_ALIGN                  WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_ICON, 1)   

/**
This command is used to set the display margin of an Icon Static's foreground. The i4_left is for 
the left margin, the i4_right is for the right margin, the i4_top is for the top margin, and the 
i4_bottom is for the bottom margin. All of them cannot be less than 0.

Input arguments

pv_param1	Pointer to a WGL_INSET_T value to specify the display margin. It cannot be NULL.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	pv_param1 is NULL, or the sum of i4_left and i4_right isn't less than Icon Static's 
width, or the sum of i4_top and i4_bottom isn't less than Icon Static's height.
*/
#define WGL_CMD_ICON_SET_CNT_INSET              WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_ICON, 2)   

/**
This command returns the display margin of an Icon Static's foreground. The i4_left is the left 
margin, the i4_right is the right margin, the i4_top is the top margin, and the i4_bottom is the 
bottom margin. 

Input arguments

pv_param1	Pointer to a WGL_INSET_T value to receive the display margin. It cannot be NULL.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
*/
#define WGL_CMD_ICON_GET_CNT_INSET              WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_ICON, 3)   

/**
This command is used to set the max foreground image number, which means such number of images 
can be contained in this icon instance at most.

Input arguments

pv_param1	The maximum images number. Its type must be UINT32.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_ICON_NOT_SUPPORT_IN_DEFAULT_STL
	The command is done under NONE WGL_STL_ICON_MULTI_IMG style.

WGLR_OUT_OF_MEMORY  Fail to reallocate memory for multiple image nodes.
*/
#define WGL_CMD_ICON_SET_MAX_FG_IMG_NUM         WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_ICON, 4)   

/**
This command is used to get the max foreground image number, which means such number of images 
can be contained in this icon instance at most.

Input arguments

pv_param1	Pointer of a UINT32 to receive the number. It cannot be NULL.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.

WGLR_ICON_NOT_SUPPORT_IN_DEFAULT_STL
	The command is done under NONE WGL_STL_ICON_MULTI_IMG style.
*/
#define WGL_CMD_ICON_GET_MAX_FG_IMG_NUM         WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_ICON, 5)   


/**(WGL_CMD_ICON_GET_MAX_FG_IMG_NUM - WGL_CMD_ICON_SET_ALIGN + 1)*/
#define ICON_CNT_PRIVATE_START                  6       

/* Specific ICON image target*/
#define WGL_IMG_ICON_MULTIPLE                   ((UINT8) WGL_IMG_WGT_SPF_START + 1)    /**<This target is used for Icon widget with WGL_STL_ICON_MULTI_IMG to set multi images.*/


/* Specific ICON widget return value*/
/**This value shall be returned when the image target WGL_IMG_ICON_MULTIPLE is used to set or get multi images or the command WGL_CMD_ICON_SET_MAX_FG_IMG_NUM, 
WGL_CMD_ICON_GET_MAX_FG_IMG_NUM is done to set or get max images number under NONE WGL_STL_ICON_MULTI_IMG style.*/
#define WGLR_ICON_NOT_SUPPORT_IN_DEFAULT_STL    ((INT32) WGLR_WIDGET_SPF_START - 1)    

/**This value shall be returned when the image target WGL_IMG_FG is used to set or get the single image under WGL_STL_ICON_MULTI_IMG style.*/
#define WGLR_ICON_NOT_SUPPORT_IN_MULTI_IMG_STL  ((INT32) WGLR_WIDGET_SPF_START - 2)    

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @struct WGL_ICON_FG_IMG_NODE_T
 *  @brief This structure holds the foreground image node information.
 *  @code
 *  typedef struct _WGL_ICON_FG_IMG_NODE_T
 *  {
 *      WGL_IMG_INFO_T            t_img_info;       
 *      INT32                     i4_ahead_gap;     
 *  } WGL_ICON_FG_IMG_NODE_T;
 *  @endcode
 *  @li@c  t_img_info                                        -image information of this image
 *  @li@c  i4_ahead_gap                                      - gap between this image and previous image
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_ICON_FG_IMG_NODE_T
{
    WGL_IMG_INFO_T            t_img_info;       /* image information of this icon */
    INT32                     i4_ahead_gap;     /* gap between this icon and previous icon */
} WGL_ICON_FG_IMG_NODE_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_ICON_MULTIPLE_INFO_T
 *  @brief This structure holds the multiple nodes information.
 *  @code
 *  typedef struct _WGL_ICON_MULTIPLE_INFO_T
 *  {
 *      UINT32                    ui4_node_num;     
 *      WGL_ICON_FG_IMG_NODE_T*   at_nodes;         
 *  } WGL_ICON_MULTIPLE_INFO_T;
 *  @endcode
 *  @li@c  ui4_node_num                                      - icon nodes number
 *  @li@c  at_nodes                                          - contains icon nodes information
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_ICON_MULTIPLE_INFO_T
{
    UINT32                    ui4_node_num;     /* icon node number*/
    WGL_ICON_FG_IMG_NODE_T*   at_nodes;         /* contains icon nodes information */
} WGL_ICON_MULTIPLE_INFO_T;


/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/



#ifdef __cplusplus
}
#endif

#endif

/*----------------------------------------------------------------------------*/
/*! @} */   /* end of groupMW_WGL_ICON*/
/*----------------------------------------------------------------------------*/


