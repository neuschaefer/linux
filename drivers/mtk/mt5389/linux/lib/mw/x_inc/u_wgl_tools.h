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
/*! @file u_wgl_tools.h 
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
 
#ifndef _U_WGL_TOOLS_H_
#define _U_WGL_TOOLS_H_

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

#define WIDGET_SERIAL_ID(_widget_type_id)                               \
            ((UINT32)((_widget_type_id)-HT_WGL_WIDGET_START))    /**<To get widget type serial id.  */

#define WGL_MAKE_CONTENT_ID(_widget_type_id, _content_type_id)          \
            ( (WIDGET_SERIAL_ID(_widget_type_id)<<9) | ((UINT32)(_content_type_id) & 0x000001FF) )    /**< Content Type ID:
                                                                                                                              --> Reserved(14:0x0) + WidgetTypeID(9) + ContentTypeID(9)       */

#define WGL_IS_CONTENT_BELONG_WIDGET(_content_type_id, _widget_type_id) \
            ( ((UINT32)(_content_type_id)>>9) == WIDGET_SERIAL_ID(_widget_type_id) )    /**<Judge the content si belong to widget or not.   */

#define WGL_MAKE_BORDER_ID(_border_type_id)                             \
            ((UINT32)(_border_type_id)&0x000001FF)    /**<  Border Type ID:
                                                                                                         --> Reserved(23:0x0) + BorderTypeID(9) */

#define TAG(b, l, v)                                                \
    (                                                               \
        (((((UINT32) 1 ) << ((UINT8)(l))) - 1) & ((UINT32) (v)))    \
          << ((UINT8)(b) - (UINT8)(l) + 1)                          \
    )    /**<Tag */

/*Command Category Prefix*/
#define WGL_CMDCTG_WIDGET       0x01    /**< Widget Command Category Prefix       */
#define WGL_CMDCTG_BORDER       0x02    /**<  Border Command Category Prefix      */
#define WGL_CMDCTG_CONTENT      0x04    /**< Content Command Category Prefix       */

#define WIDGET_SERIAL_ID_BASE   0x01FF    /**<The widget serial base id. */


#define WGL_MAKE_GL_WIDGET_CMD(_cmd_id)    \
                    (TAG(31, 4, WGL_CMDCTG_WIDGET) | TAG(27, 9, 0) | TAG(18, 9, WIDGET_SERIAL_ID_BASE) | TAG(9, 10, (_cmd_id)) )    
                    /**<Widget make general widget command by command id.  */

#define WGL_MAKE_ANIM_WIDGET_CMD(_cmd_id)  \
                    WGL_MAKE_GL_WIDGET_CMD(_cmd_id)    /**<To make animation widget command.  */

#define WGL_MAKE_WIDGET_CMD(_widget_type_id, _cmd_id)       \
                    (TAG(31, 4, WGL_CMDCTG_WIDGET) | TAG(27, 9, 0) | TAG(18, 9, WIDGET_SERIAL_ID(_widget_type_id)) | TAG(9, 10, (_cmd_id)) )    /**<Widget Commands:
                                                                                                                                                                                                                                                            --> Category prefix(4) + Reserved (9:0x0) + WidgetTypeID(9) + Command Id (10)         */

#define WGL_MAKE_CONTENT_CMD(_content_type_id, _cmd_id)     \
                    (TAG(31, 4, WGL_CMDCTG_CONTENT) | TAG(27, 18, (_content_type_id)) | TAG(9, 10, (_cmd_id)) )    /**<     Content Commands:
                                                                                                                                                                                                                    --> Category prefix(4) + WidgetTypeID(9) + ContentTypeID(9) + Command Id (10)     */

#define WGL_MAKE_BORDER_CMD(_border_type_id, _cmd_id)       \
                    (TAG(31, 4, WGL_CMDCTG_BORDER) | TAG(27, 9, 0) | TAG(18, 9, (_border_type_id)) | TAG(9, 10, (_cmd_id)) )    /**<   Border Commands:
                                                                                                                                                                                                                                    --> Category prefix(4) + Reserved (9:0x0) + BorderTypeID (9) + Command Id (10)       */


#define WGL_PACK(_x)                                                        \
        ((VOID *)((UINT32)(_x)))    /**< To pack for widget. */
        
#define WGL_PACK_2(_x, _y)                                                  \
        ((VOID *) (  (((UINT32)((UINT16)(_x)))<<16) | ((UINT32)((UINT16)(_y)))  ) )    /**<To pack by two UINT16 for widget. */
        
#define WGL_PACK_3(_x16, _y8, _z8)                                          \
        (VOID *)(  (((UINT32)((UINT16)(_x16)))<<16) | ((UINT32)((UINT8)(_y8))<<8) | ((UINT32)((UINT8)(_z8))) )    /**< To pack by UINT16+UINT8+UINT8 for widget. */
        
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


