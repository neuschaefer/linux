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
/*! @file u_wgl_obj_type.h 
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
 
#ifndef _U_WGL_OBJ_TYPE_H_
#define _U_WGL_OBJ_TYPE_H_

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
/*Content Type Definition*/
#define WGL_CONTENT_FRAME_DEF               WGL_MAKE_CONTENT_ID(HT_WGL_WIDGET_FRAME, 1)    /**<Frame widget's default content type used in c_wgl_create_widget()         */
#define WGL_CONTENT_ICON_DEF                WGL_MAKE_CONTENT_ID(HT_WGL_WIDGET_ICON, 1)    /**<Icon widget's default content type used in c_wgl_create_widget()            */
#define WGL_CONTENT_TEXT_DEF                WGL_MAKE_CONTENT_ID(HT_WGL_WIDGET_TEXT, 1)    /**< Text widget's default content type used in c_wgl_create_widget()           */
#define WGL_CONTENT_HYPER_TEXT_DEF          WGL_MAKE_CONTENT_ID(HT_WGL_WIDGET_HYPER_TEXT, 1)    /**<  Hyper Text widget's default content type used in c_wgl_create_widget()          */
#define WGL_CONTENT_BUTTON_DEF              WGL_MAKE_CONTENT_ID(HT_WGL_WIDGET_BUTTON, 1)    /**< Button widget's default content type used in c_wgl_create_widget()           */
#define WGL_CONTENT_EDIT_BOX_DEF            WGL_MAKE_CONTENT_ID(HT_WGL_WIDGET_EDIT_BOX, 1)    /**<Edit widget's default content type used in c_wgl_create_widget()            */
#define WGL_CONTENT_PROGRESS_BAR_DEF        WGL_MAKE_CONTENT_ID(HT_WGL_WIDGET_PROGRESS_BAR, 1)    /**<   Progress widget's default content type used in c_wgl_create_widget()         */
#define WGL_CONTENT_CC_DEF                  WGL_MAKE_CONTENT_ID(HT_WGL_WIDGET_CC, 1)    /**<CC  widget's default content type used in c_wgl_create_widget()            */
#define WGL_CONTENT_LIST_BOX_DEF            WGL_MAKE_CONTENT_ID(HT_WGL_WIDGET_LIST_BOX, 1)    /**< List Box widget's default content type used in c_wgl_create_widget()           */
#define WGL_CONTENT_LIST_BOX_HORIZ          WGL_MAKE_CONTENT_ID(HT_WGL_WIDGET_EDIT_BOX, 2)    /**< Edit widget's content type used in c_wgl_create_widget()           */
#define WGL_CONTENT_TKTP_DEF                WGL_MAKE_CONTENT_ID(HT_WGL_WIDGET_TKTP, 1)    /**<Ticker Tape widget's default content type used in c_wgl_create_widget()            */
#define WGL_CONTENT_HTS_DEF                 WGL_MAKE_CONTENT_ID(HT_WGL_WIDGET_HTS, 1)    /**<Hyper Text widget's default content type used in c_wgl_create_widget()            */
#define WGL_CONTENT_SBTL_DEF                WGL_MAKE_CONTENT_ID(HT_WGL_WIDGET_SBTL, 1)    /**<Subtitle widget's default content type used in c_wgl_create_widget()            */
#define WGL_CONTENT_TTX_DEF                 WGL_MAKE_CONTENT_ID(HT_WGL_WIDGET_TTX, 1)    /**<Teletext widget's default content type used in c_wgl_create_widget()            */
#define WGL_CONTENT_ANIM_DEF                WGL_MAKE_CONTENT_ID(HT_WGL_WIDGET_ANIM, 1)    /**< Animation widget's default content type used in c_wgl_create_widget()           */
#define WGL_CONTENT_GROUP_DEF               WGL_MAKE_CONTENT_ID(HT_WGL_WIDGET_GROUP, 1)    /**< Group widget's default content type used in c_wgl_create_widget()           */
#define WGL_CONTENT_CC_ISDB_DEF             WGL_MAKE_CONTENT_ID(HT_WGL_WIDGET_CC_ISDB, 1)    /**<CC ISDB widget's default content type used in c_wgl_create_widget()            */

/*Border Type Definition*/
#define WGL_BORDER_BASE                     WGL_MAKE_BORDER_ID(0)    /**<        */
#define WGL_BORDER_ID_FIRST                 WGL_MAKE_BORDER_ID(1)    /**<        */

/*-----------------------*/
#define WGL_BORDER_NULL                     WGL_MAKE_BORDER_ID(1)    /**< Widget's border type is NULL Border  used in c_wgl_create_widget()     */
#define WGL_BORDER_TRANSP                   WGL_MAKE_BORDER_ID(2)    /**< Widget's border type is Transparent Border used in c_wgl_create_widget()      */
#define WGL_BORDER_UNIFORM                  WGL_MAKE_BORDER_ID(3)    /**<Widget's border type is Uniform Border used in c_wgl_create_widget()       */
#define WGL_BORDER_RAISED                   WGL_MAKE_BORDER_ID(4)    /**< Widget's border type is Raised Border  used in c_wgl_create_widget()     */
#define WGL_BORDER_DEPRESSED                WGL_MAKE_BORDER_ID(5)    /**< Widget's border type is Depressed Border  used in c_wgl_create_widget()    */
#define WGL_BORDER_DROP_SHADOW_LEFT         WGL_MAKE_BORDER_ID(6)    /**<Widget's border type is Drop Shadow Left Border used in c_wgl_create_widget()       */
#define WGL_BORDER_DROP_SHADOW_RIGHT        WGL_MAKE_BORDER_ID(7)    /**< Widget's border type is Drop Shadow Right Border used in c_wgl_create_widget()      */
#define WGL_BORDER_TIMG                     WGL_MAKE_BORDER_ID(8)    /**<Widget's border type is Timg Border used in c_wgl_create_widget()       */
/*-----------------------*/

#define WGL_BORDER_ID_LAST                  WGL_MAKE_BORDER_ID(8)    /**<        */
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
/*! @} */   /* end of  groupMW_WGL*/
/*----------------------------------------------------------------------------*/


