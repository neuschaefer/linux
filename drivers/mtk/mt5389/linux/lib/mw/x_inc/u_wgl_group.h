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
/*! @file u_wgl_group.h 
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
/*! @defgroup groupMW_WGL_GROUP  Group Widget
 *
 *  @ingroup groupMW_WGL
 *  @brief Provides definitions,commands of the group widget.
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW_WGL
 *  @{ 
 */
/*----------------------------------------------------------------------------*/ 
#ifndef _U_WGL_GROUP_H_
#define _U_WGL_GROUP_H_

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
#define GROUP_CNT_PRIVATE_START                 0    /**< group private command start id       */
/*Specific Widget Style*/
#define WGL_STL_GRP_VIRTICAL           (((UINT32)(WGL_STL_WGT_SPF_START))<<0)    /**< Indicate widgets in group is virtical layout        */
#define WGL_STL_GRP_HORIZONTAL         (((UINT32)(WGL_STL_WGT_SPF_START))<<1)    /**<  Indicate widgets in group is horicontal layout          */
#define WGL_STL_GRP_HIDE_NO_SPACE      (((UINT32)(WGL_STL_WGT_SPF_START))<<2)    /**< Indicate widget who is invisible in group will occupy no space, this style is Default style  */
#define WGL_STL_GRP_HIDE_WITH_SPACE    (((UINT32)(WGL_STL_WGT_SPF_START))<<3)    /**<  Indicate widget who is invisible in group will occupy space indicated by its rectangle      */
/*Specific Command*/
#define WGL_CMD_GRP_SET_ALIGN          WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_GROUP, 0)    /**< set all widgets in group's alignment ,

                                                                                                                                                              related parameters in c_wgl_do_cmd(),

                                                                                                                                                              pv_param1: UINT8* alignment value;  

                                                                                                                                                              pv_param2:NULL*/
#define WGL_CMD_GRP_GET_ALIGN          WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_GROUP, 1)    /**< get all widgets in group's alignment ,

                                                                                                                                                              related parameters in c_wgl_do_cmd(),

                                                                                                                                                              pv_param1: UINT8* alignment value;  

                                                                                                                                                              pv_param2:NULL*/

#define WGL_CMD_GRP_SET_CHILD_INFO     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_GROUP, 2)    /**< Set child infomation to group,just gap infomation is set at moment.

                                                                                                                                                               related parameters in c_wgl_do_cmd(),

                                                                                                                                                               pv_param1: HANDLE_T h_child_widget;

                                                                                                                                                               pv_param2:UINT32 ui4_gap;gap between h_child_widget and its previous brother widget */

#define WGL_CMD_GRP_GET_CHILD_INFO     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_GROUP, 3)    /**< Get child infomation from group,just gap infomation can be get at moment.

                                                                                                                                                               related parameters in c_wgl_do_cmd(),

                                                                                                                                                               pv_param1: HANDLE_T h_child_widget;

                                                                                                                                                               pv_param2:UINT32 *pui4_gap;gap between h_child_widget and its previous brother widget */
#define WGL_CMD_GRP_UPDATE_ALL         WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_GROUP, 4)    /**<relayout all children,

                                                                                                                                                              related parameters in c_wgl_do_cmd(),

                                                                                                                                                              pv_param1: NULL;  

                                                                                                                                                              pv_param2:NULL*/
#define WGL_CMD_GRP_UPDATE             WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_GROUP, 5)    /**< relayout specific child widget,

                                                                                                                                                              related parameters in c_wgl_do_cmd(),

                                                                                                                                                              pv_param1: HANDLE_T h_child_widget;  

                                                                                                                                                              pv_param2:NULL*/


/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/



#ifdef __cplusplus
}
#endif

#endif

/*----------------------------------------------------------------------------*/
/*! @} */   /* end of groupMW_WGL_GROUP*/
/*----------------------------------------------------------------------------*/
