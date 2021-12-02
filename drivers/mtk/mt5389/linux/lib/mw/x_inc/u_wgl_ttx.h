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
/*! @file u_wgl_ttx.h 
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
/*! @defgroup groupMW_WGL_TELETEXT   Teletext Widget 
 *
 *  @ingroup groupMW_WGL
 *  @brief Provides definitions and exported function of teletext widget.
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW_WGL
 *  @{ 
 */
/*----------------------------------------------------------------------------*/


#ifndef _U_WGL_TTX_H_
#define _U_WGL_TTX_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "u_gl.h"
#include "u_wgl_common.h"
#include "u_wgl_tools.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* commands */
/** This command set the teletext widget visible or not  and update or not.The API command is 
SM_TTXH_SET_TYPE_WGL_ATTR for app to use and the command parameter is in SM_TTXH_WGL_ATTR_T struct. 

ui1_wgl_id:             -The teletext widget index.

ui4_wgl_cmd:          -Must set to WGL_CMD_TTX_SET_VISB

pv_wgl_do_cmd_param1:          -Point to BOOL data that indicates set widget visible or not.

pv_wgl_do_cmd_param2:          -Point to BOOL data that indicates update widget or not.
*/
#define WGL_CMD_TTX_SET_VISB                    WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 0)    

/** This command set the header panel of teletext to be visible or not .The API command is 
SM_TTXH_SET_TYPE_WGL_ATTR for app to use and the command parameter is in SM_TTXH_WGL_ATTR_T struct. 

ui1_wgl_id:             -The teletext widget index.

ui4_wgl_cmd:          -Must set to WGL_CMD_TTX_SET_VISB_HDR_PNL

pv_wgl_do_cmd_param1:          -Point to BOOL data that indicates set header panel visible or not.

pv_wgl_do_cmd_param2:          -Null.
*/
#define WGL_CMD_TTX_SET_VISB_HDR_PNL            WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 1)    

/** This command set the main and side panel of teletext to be visible or not .The API command is 
SM_TTXH_SET_TYPE_WGL_ATTR for app to use and the command parameter is in SM_TTXH_WGL_ATTR_T struct. 

ui1_wgl_id:             -The teletext widget index.

ui4_wgl_cmd:          -Must set to WGL_CMD_TTX_SET_VISB_MAIN_SIDE_PNL

pv_wgl_do_cmd_param1:          -Point to BOOL data that indicates set main and side panel visible or not.

pv_wgl_do_cmd_param2:          -Null.
*/
#define WGL_CMD_TTX_SET_VISB_MAIN_SIDE_PNL      WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 2)    

/** This command set the app panel of teletext to be visible or not .The API command is 
SM_TTXH_SET_TYPE_WGL_ATTR for app to use and the command parameter is in SM_TTXH_WGL_ATTR_T struct. 

ui1_wgl_id:             -The teletext widget index.

ui4_wgl_cmd:          -Must set to WGL_CMD_TTX_SET_VISB_APP_PNL

pv_wgl_do_cmd_param1:          -Point to the UINT8 data that is app row number.

pv_wgl_do_cmd_param2:          -Point to BOOL data that indicates set app panel of special row visible or not.
*/
#define WGL_CMD_TTX_SET_VISB_APP_PNL            WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 3)    

/** This command set the app layer of teletext to be visible or not .The API command is 
SM_TTXH_SET_TYPE_WGL_ATTR for app to use and the command parameter is in SM_TTXH_WGL_ATTR_T struct. 

ui1_wgl_id:             -The teletext widget index.

ui4_wgl_cmd:          -Must set to WGL_CMD_TTX_SET_VISB_APP_LAYER

pv_wgl_do_cmd_param1:          -Point to BOOL data that indicates set app layer visible or not.

pv_wgl_do_cmd_param2:          -Null.
*/
#define WGL_CMD_TTX_SET_VISB_APP_LAYER          WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 4)    

/** This command set teletext border inforamtion .The API command is SM_TTXH_SET_TYPE_WGL_ATTR 
for app to use and the command parameter is in SM_TTXH_WGL_ATTR_T struct. 

ui1_wgl_id:             -The teletext widget index.

ui4_wgl_cmd:          -Must set to WGL_CMD_TTX_SET_BDR

pv_wgl_do_cmd_param1:          -Point to UINT32 data that contains the border type information.

pv_wgl_do_cmd_param2:          -Null.
*/
#define WGL_CMD_TTX_SET_BDR                     WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 5)    

/** This command is to do border inforamtion .The API command is SM_TTXH_SET_TYPE_WGL_ATTR 
for app to use and the command parameter is in SM_TTXH_WGL_ATTR_T struct. 

ui1_wgl_id:             -The teletext widget index.

ui4_wgl_cmd:          -Must set to WGL_CMD_TTX_DO_BDR_CMD

pv_wgl_do_cmd_param1:          -Point to WGL_TTX_BDR_CMD_INFO_T struct that contains the border 
information.

pv_wgl_do_cmd_param2:          -Null.
*/
#define WGL_CMD_TTX_DO_BDR_CMD                  WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 6)    

/** This command is to set the special app row information .The API command is SM_TTXH_SET_TYPE_WGL_ATTR 
for app to use and the command parameter is in SM_TTXH_WGL_ATTR_T struct. 

ui1_wgl_id:             -The teletext widget index.

ui4_wgl_cmd:          -Must set to WGL_CMD_TTX_SET_APP_PNL

pv_wgl_do_cmd_param1:          -Contains the UINT8 data that is app row number information.

pv_wgl_do_cmd_param2:          -Point to WGL_TTX_APP_PNL_T struct that contains app panel information.
*/
#define WGL_CMD_TTX_SET_APP_PNL                 WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 7)    

/** This command is to set the special app mask row information .The API command is SM_TTXH_SET_TYPE_WGL_ATTR 
for app to use and the command parameter is in SM_TTXH_WGL_ATTR_T struct. 

ui1_wgl_id:             -The teletext widget index.

ui4_wgl_cmd:          -Must set to WGL_CMD_TTX_SET_APP_MASK

pv_wgl_do_cmd_param1:          -Contains the UINT8 data that is app mask row number(0 ~ (WGL_TTX_MAIN_PNL_MAX_ROW_NUM - 1)).

pv_wgl_do_cmd_param2:          -Point to WGL_TTX_APP_MASK_T struct that contains app mask panel information.
*/
#define WGL_CMD_TTX_SET_APP_MASK                WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 8)    

/** This command is to clear the special app mask row information .The API command is SM_TTXH_SET_TYPE_WGL_ATTR 
for app to use and the command parameter is in SM_TTXH_WGL_ATTR_T struct. 

ui1_wgl_id:             -The teletext widget index.

ui4_wgl_cmd:          -Must set to WGL_CMD_TTX_SET_APP_MASK

pv_wgl_do_cmd_param1:          -Contains the UINT8 data that is app mask row number(0 ~ (WGL_TTX_MAIN_PNL_MAX_ROW_NUM - 1)).

pv_wgl_do_cmd_param2:          -Null
*/
#define WGL_CMD_TTX_CLEAR_APP_MASK              WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 9)    

/** This command is to reveal teletext widget or not .The API command is SM_TTXH_SET_TYPE_WGL_ATTR 
for app to use and the command parameter is in SM_TTXH_WGL_ATTR_T struct. 

ui1_wgl_id:             -The teletext widget index.

ui4_wgl_cmd:          -Must set to WGL_CMD_TTX_REVEAL

pv_wgl_do_cmd_param1:          -Point to BOOL data that indicates reveal or not.

pv_wgl_do_cmd_param2:          -Null
*/
#define WGL_CMD_TTX_REVEAL                      WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 10)   

/** This command is to update teletext display information .The API command is SM_TTXH_SET_TYPE_WGL_ATTR 
for app to use and the command parameter is in SM_TTXH_WGL_ATTR_T struct. 

ui1_wgl_id:             -The teletext widget index.

ui4_wgl_cmd:          -Must set to WGL_CMD_TTX_UPDATE_DISP_INFO

pv_wgl_do_cmd_param1:          -Point to WGL_TTX_DISP_INFO_T struct that contains teletext display 
information.

pv_wgl_do_cmd_param2:          -Null
*/
#define WGL_CMD_TTX_UPDATE_DISP_INFO            WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 11)  

/** This command is to update teletext mix mode information .The API command is SM_TTXH_SET_TYPE_WGL_ATTR 
for app to use and the command parameter is in SM_TTXH_WGL_ATTR_T struct. 

ui1_wgl_id:             -The teletext widget index.

ui4_wgl_cmd:          -Must set to WGL_CMD_TTX_UPDATE_MIX_MODE

pv_wgl_do_cmd_param1:          -Point to WGL_TTX_MIX_MODE_T struct that contains teletext mix mode 
information.

pv_wgl_do_cmd_param2:          -Null
*/
#define WGL_CMD_TTX_UPDATE_MIX_MODE             WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 12)    

/** This command is to update the special teletext row information .The API command is SM_TTXH_SET_TYPE_WGL_ATTR 
for app to use and the command parameter is in SM_TTXH_WGL_ATTR_T struct. 

ui1_wgl_id:             -The teletext widget index.

ui4_wgl_cmd:          -Must set to WGL_CMD_TTX_UPDATE_TTX_ROW

pv_wgl_do_cmd_param1:          -Point to UINT8 data that is the teletext row number.

pv_wgl_do_cmd_param2:          -Null
*/
#define WGL_CMD_TTX_UPDATE_TTX_ROW              WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 13)    

/** This command is to update teletext  layer information .The API command is SM_TTXH_SET_TYPE_WGL_ATTR 
for app to use and the command parameter is in SM_TTXH_WGL_ATTR_T struct. 

ui1_wgl_id:             -The teletext widget index.

ui4_wgl_cmd:          -Must set to WGL_CMD_TTX_UPDATE_TTX_LAYER

pv_wgl_do_cmd_param1:          -Null.

pv_wgl_do_cmd_param2:          -Null
*/
#define WGL_CMD_TTX_UPDATE_TTX_LAYER            WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 14)    

/** This command is to update teletext  app mask information .The API command is SM_TTXH_SET_TYPE_WGL_ATTR 
for app to use and the command parameter is in SM_TTXH_WGL_ATTR_T struct. 

ui1_wgl_id:             -The teletext widget index.

ui4_wgl_cmd:          -Must set to WGL_CMD_TTX_UPDATE_TTX_LAYER

pv_wgl_do_cmd_param1:          -Contains UINT8 data that is teletext row number.

pv_wgl_do_cmd_param2:          -Null
*/
#define WGL_CMD_TTX_UPDATE_APP_MASK             WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 15)   

/** This command is to update  app panel information .The API command is SM_TTXH_SET_TYPE_WGL_ATTR 
for app to use and the command parameter is in SM_TTXH_WGL_ATTR_T struct. 

ui1_wgl_id:             -The teletext widget index.

ui4_wgl_cmd:          -Must set to WGL_CMD_TTX_UPDATE_TTX_LAYER

pv_wgl_do_cmd_param1:          -Contains UINT8 data that is teletext app row number.

pv_wgl_do_cmd_param2:          -Null
*/
#define WGL_CMD_TTX_UPDATE_APP_PNL              WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 16)    

/** This command is to update  app layere information .The API command is SM_TTXH_SET_TYPE_WGL_ATTR 
for app to use and the command parameter is in SM_TTXH_WGL_ATTR_T struct. 

ui1_wgl_id:             -The teletext widget index.

ui4_wgl_cmd:          -Must set to WGL_CMD_TTX_UPDATE_TTX_LAYER

pv_wgl_do_cmd_param1:          -Null.

pv_wgl_do_cmd_param2:          -Null
*/
#define WGL_CMD_TTX_UPDATE_APP_LAYER            WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 17)   

/** This command is to update  the whole teletext information .The API command is SM_TTXH_SET_TYPE_WGL_ATTR 
for app to use and the command parameter is in SM_TTXH_WGL_ATTR_T struct. 

ui1_wgl_id:             -The teletext widget index.

ui4_wgl_cmd:          -Must set to WGL_CMD_TTX_UPDATE

pv_wgl_do_cmd_param1:          -Null.

pv_wgl_do_cmd_param2:          -Null
*/
#define WGL_CMD_TTX_UPDATE                      WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 18)  

/** This command is to set teletext transparent information .The API command is SM_TTXH_SET_TYPE_WGL_ATTR 
for app to use and the command parameter is in SM_TTXH_WGL_ATTR_T struct. 

ui1_wgl_id:             -The teletext widget index.

ui4_wgl_cmd:          -Must set to WGL_CMD_TTX_SET_TRANSPARENT_INF

pv_wgl_do_cmd_param1:          -Point to WGL_TTX_TRANSP_INF_T struct that contain the transparent 
information.The detail can see WGL_TTX_TRANSP_INF_T struct.

pv_wgl_do_cmd_param2:          -Null
*/
#define WGL_CMD_TTX_SET_TRANSPARENT_INF         WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 19)   

/** This command is to set teletext view port margin clear method.The method has 
WGL_TTX_VP_MARGIN_CLR_METHOD_AUTO , WGL_TTX_VP_MARGIN_CLR_METHOD_TRANSP .The API command is
SM_TTXH_SET_TYPE_WGL_ATTR for app to use and the command parameter is in SM_TTXH_WGL_ATTR_T struct. 

ui1_wgl_id:             -The teletext widget index.

ui4_wgl_cmd:          -Must set to WGL_CMD_TTX_SET_VP_MARGIN_CLR_METHOD

pv_wgl_do_cmd_param1:          -Point to WGL_TTX_VP_MARGIN_CLR_METHOD_T struct that contain 
viewport margin clear method information.

pv_wgl_do_cmd_param2:          -Null
*/
#define WGL_CMD_TTX_SET_VP_MARGIN_CLR_METHOD    WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 20)   

/** This command is to set teletext box show type. The API command is SM_TTXH_SET_TYPE_WGL_ATTR 
for app to use and the command parameter is in SM_TTXH_WGL_ATTR_T struct. 

ui1_wgl_id:             -The teletext widget index.

ui4_wgl_cmd:          -Must set to WGL_CMD_TTX_SET_BOX_SHOW_TYPE

pv_wgl_do_cmd_param1:          -Contain UINT32 data that is box show type information.It maybe 
WGL_TTX_BOX_SHOW_TYPE_NORMAL,WGL_TTX_BOX_SHOW_TYPE_HP,WGL_TTX_BOX_SHOW_TYPE_MP,
WGL_TTX_BOX_SHOW_TYPE_MP_TR,WGL_TTX_BOX_SHOW_TYPE_APP etc type.

pv_wgl_do_cmd_param2:          -Null
*/
#define WGL_CMD_TTX_SET_BOX_SHOW_TYPE           WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 21)   

#define WGL_CMD_TTX_SET_MPTR_ATTR               WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 22)   

/** This command set the teletext widget visible or not  and update or not.The API command is 
SM_TTXH_SET_TYPE_WGL_ATTR for app to use and the command parameter is in SM_TTXH_WGL_ATTR_T struct. 

ui1_wgl_id:             -The teletext widget index.

ui4_wgl_cmd:          -Must set to WGL_CMD_TTX_SET_VISB

pv_wgl_do_cmd_param1:          -Point to BOOL data that indicates set widget visible or not.

pv_wgl_do_cmd_param2:          -Point to BOOL data that indicates update widget or not.
*/
#define WGL_CMD_TTX_SET_VISB_SYNC                 WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_TTX, 23)    



/* specific color targets - teletext widget */
/**Clear full screen flag*/
#define WGL_CLR_TTX_FULL_SCRN               ((UINT8) WGL_CLR_WGT_SPF_START + 1)   

/**Clear full row flag*/
#define WGL_CLR_TTX_FULL_ROW                ((UINT8) WGL_CLR_WGT_SPF_START + 2)   


/* constants */
#define WGL_TTX_APP_MAX_CELL_NUM            ((UINT8) 56)    /**<Define app max cells num */

#define WGL_TTX_MAIN_PNL_CELL_NUM           ((UINT8) 40)    /**<Define main panel cells num*/

#define WGL_TTX_SIDE_PNL_CELL_NUM           ((UINT8) 16)    /**<Define side panel cells num*/

/**<Define app panel max cells num */
#define WGL_TTX_APP_PNL_MAX_CELL_NUM        (WGL_TTX_MAIN_PNL_CELL_NUM + WGL_TTX_SIDE_PNL_CELL_NUM)    

#define WGL_TTX_MAX_HGRID_NUM               ((UINT8) 56)    /**<Define the max horizontal grids num*/

#define WGL_TTX_MAX_VGRID_NUM               ((UINT8) 30)    /**<Define the max vertical grids num*/

#define WGL_TTX_CGID_INFO_NUM               ((UINT8) 4)/**< Define the max grids num for one char information*/    

/* notify reason */
/*------------------------------------------------------------------*/
/*! @enum  WGL_TTX_NFY_REASON_T
 *  @brief Defines the ttx widget notify reason types
 *  @code
 *  typedef enum _WGL_TTX_NFY_REASON_T
 *  {
 *      WGL_TTX_NFY_REASON_UPT_FINISHED = 0,
 *      WGL_TTX_NFY_REASON_REVEAL_FINISHED,
 *      WGL_TTX_NFY_REASON_UPT_DISP_INFO_FINISHED,
 *      WGL_TTX_NFY_REASON_UPT_MIX_MODE_FINISHED
 *  } WGL_TTX_NFY_REASON_T;
 *  @endcode
 *  @li@c  WGL_TTX_NFY_REASON_UPT_FINISHED                   -Update finished notify reason
 *  @li@c  WGL_TTX_NFY_REASON_REVEAL_FINISHED              -Reveal finished notify reason
 *  @li@c  WGL_TTX_NFY_REASON_UPT_DISP_INFO_FINISHED   -Update display information finished notify reason.
 *  @li@c  WGL_TTX_NFY_REASON_UPT_MIX_MODE_FINISHED   -Update mix mode finished notify reason
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_TTX_NFY_REASON_T
{
    WGL_TTX_NFY_REASON_UPT_FINISHED = 0,
    WGL_TTX_NFY_REASON_REVEAL_FINISHED,
    WGL_TTX_NFY_REASON_UPT_DISP_INFO_FINISHED,
    WGL_TTX_NFY_REASON_UPT_MIX_MODE_FINISHED,
    WGL_TTX_NFY_REASON_PAINT_FINISHED
} WGL_TTX_NFY_REASON_T;

/* WGL_TTX_HGRID_NUM_T */
/*------------------------------------------------------------------*/
/*! @enum WGL_TTX_HGRID_NUM_T
 *  @brief Define the horizontal grids num information
 *  @code
 *  typedef enum _WGL_TTX_HGRID_NUM_T
 *  {
 *      WGL_TTX_HGRID_NUM_NO_SIDE_PNL   = WGL_TTX_MAIN_PNL_CELL_NUM,
 *      WGL_TTX_HGRID_NUM_WITH_SIDE_PNL = (WGL_TTX_MAIN_PNL_CELL_NUM + WGL_TTX_SIDE_PNL_CELL_NUM)
 *  } WGL_TTX_HGRID_NUM_T;
 *  @endcode
 *  @li@c  WGL_TTX_HGRID_NUM_NO_SIDE_PNL       -Define the horizontal grids num while no side panel
 *  @li@c  WGL_TTX_HGRID_NUM_WITH_SIDE_PNL    -Define the horizontal grids num while with side panel
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_TTX_HGRID_NUM_T
{
    WGL_TTX_HGRID_NUM_NO_SIDE_PNL   = WGL_TTX_MAIN_PNL_CELL_NUM,
    WGL_TTX_HGRID_NUM_WITH_SIDE_PNL = (WGL_TTX_MAIN_PNL_CELL_NUM + WGL_TTX_SIDE_PNL_CELL_NUM)
    
} WGL_TTX_HGRID_NUM_T;

/* WGL_TTX_APP_CELL_TYPE_T */
/*------------------------------------------------------------------*/
/*! @enum WGL_TTX_APP_CELL_TYPE_T
 *  @brief Defines the app cell type 
 *  @code
 *  typedef enum _WGL_TTX_APP_CELL_TYPE_T
 *  {
 *      WGL_TTX_APP_CELL_TYPE_IGNORE,
 *      WGL_TTX_APP_CELL_TYPE_CHAR,
 *      WGL_TTX_APP_CELL_TYPE_IMG,
 *      WGL_TTX_APP_CELL_TYPE_ATTR_OVRD
 *  } WGL_TTX_APP_CELL_TYPE_T;
 *  @endcode
 *  @li@c  WGL_TTX_APP_CELL_TYPE_IGNORE             -Define the cell is ignore
 *  @li@c  WGL_TTX_APP_CELL_TYPE_CHAR                -Define the cell is char type cell
 *  @li@c  WGL_TTX_APP_CELL_TYPE_IMG                  -Define the cell is image type cell
 *  @li@c  WGL_TTX_APP_CELL_TYPE_ATTR_OVRD       -Define the cell attribute is overlay type that ttx 
 cell information can be overlayed by app cell information
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_TTX_APP_CELL_TYPE_T
{
    WGL_TTX_APP_CELL_TYPE_IGNORE,
    WGL_TTX_APP_CELL_TYPE_CHAR,
    WGL_TTX_APP_CELL_TYPE_IMG,
    WGL_TTX_APP_CELL_TYPE_ATTR_OVRD
    
} WGL_TTX_APP_CELL_TYPE_T;

/* WGL_TTX_APP_PNL_DISP_MODE_T */
/*------------------------------------------------------------------*/
/*! @enum WGL_TTX_APP_PNL_DISP_MODE_T
 *  @brief Defines the app panel display mode
 *  @code
 *  typedef enum _WGL_TTX_APP_PNL_DISP_MODE_T
 *  {
 *      WGL_TTX_APP_PNL_DM_SGL_HEIGHT = 0,
 *      WGL_TTX_APP_PNL_DM_DBL_HEIGHT
 *  } WGL_TTX_APP_PNL_DISP_MODE_T;
 *  @endcode
 *  @li@c  WGL_TTX_APP_PNL_DM_SGL_HEIGHT            -Define app cell is signal height size
 *  @li@c  WGL_TTX_APP_PNL_DM_DBL_HEIGHT            -Define app cell is double height size
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_TTX_APP_PNL_DISP_MODE_T
{
    WGL_TTX_APP_PNL_DM_SGL_HEIGHT = 0,
    WGL_TTX_APP_PNL_DM_DBL_HEIGHT
    
} WGL_TTX_APP_PNL_DISP_MODE_T;

/* WGL_TTX_APP_PNL_HZ_POS_MODE_T */
/*------------------------------------------------------------------*/
/*! @enum WGL_TTX_APP_PNL_HZ_POS_MODE_T
 *  @brief Defines the app cell position types
 *  @code
 *  typedef enum _WGL_TTX_APP_PNL_HZ_POS_MODE_T
 *  {
 *      WGL_TTX_APP_PNL_HZ_POS_ANCHOR_MAIN_PNL = 0,
 *      WGL_TTX_APP_PNL_HZ_POS_ANCHOR_VP_LEFT,
 *      WGL_TTX_APP_PNL_HZ_POS_ANCHOR_VP_RIGHT,
 *      WGL_TTX_APP_PNL_HZ_POS_FULL_VP_WIDTH
 *  } WGL_TTX_APP_PNL_HZ_POS_MODE_T;
 *  @endcode
 *  @li@c  WGL_TTX_APP_PNL_HZ_POS_ANCHOR_MAIN_PNL       -Define app position is reference by main panel.
 *  @li@c  WGL_TTX_APP_PNL_HZ_POS_ANCHOR_VP_LEFT          -Define app left position is reference by viewpoint left position.
 *  @li@c  WGL_TTX_APP_PNL_HZ_POS_ANCHOR_VP_RIGHT       -Define app right position is reference by viewpoint right.
 *  @li@c  WGL_TTX_APP_PNL_HZ_POS_FULL_VP_WIDTH           -Define app posintion is reference by the full viewpoint
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_TTX_APP_PNL_HZ_POS_MODE_T
{
    WGL_TTX_APP_PNL_HZ_POS_ANCHOR_MAIN_PNL = 0,
    WGL_TTX_APP_PNL_HZ_POS_ANCHOR_VP_LEFT,
    WGL_TTX_APP_PNL_HZ_POS_ANCHOR_VP_RIGHT,
    WGL_TTX_APP_PNL_HZ_POS_FULL_VP_WIDTH
    
} WGL_TTX_APP_PNL_HZ_POS_MODE_T;

/* WGL_TTX_HDR_PNL_HZ_POS_MODE_T */
/*------------------------------------------------------------------*/
/*! @enum WGL_TTX_HDR_PNL_HZ_POS_MODE_T
 *  @brief Define the ttx header panel postiont mode  types
 *  @code
 *  typedef enum _WGL_TTX_HDR_PNL_HZ_POS_MODE_T
 *  {
 *      WGL_TTX_HDR_PNL_HZ_POS_ANCHOR_MAIN_PNL = 0,
 *      WGL_TTX_HDR_PNL_HZ_POS_ANCHOR_VP_LEFT,
 *      WGL_TTX_HDR_PNL_HZ_POS_ANCHOR_VP_RIGHT,
 *      WGL_TTX_HDR_PNL_HZ_POS_FULL_VP_WIDTH
 *  } WGL_TTX_HDR_PNL_HZ_POS_MODE_T;
 *  @endcode
 *  @li@c  WGL_TTX_HDR_PNL_HZ_POS_ANCHOR_MAIN_PNL     -Define ttx header panel position is reference by main panel
 *  @li@c  WGL_TTX_HDR_PNL_HZ_POS_ANCHOR_VP_LEFT       -Define ttx header panel position is reference by viewpoint left
 *  @li@c  WGL_TTX_HDR_PNL_HZ_POS_ANCHOR_VP_RIGHT     -Define ttx header panel position is reference by viewpoint right.
 *  @li@c  WGL_TTX_HDR_PNL_HZ_POS_FULL_VP_WIDTH         -Define ttx header panel postion is reference by full viewpoint 
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_TTX_HDR_PNL_HZ_POS_MODE_T
{
    WGL_TTX_HDR_PNL_HZ_POS_ANCHOR_MAIN_PNL = 0,
    WGL_TTX_HDR_PNL_HZ_POS_ANCHOR_VP_LEFT,
    WGL_TTX_HDR_PNL_HZ_POS_ANCHOR_VP_RIGHT,
    WGL_TTX_HDR_PNL_HZ_POS_FULL_VP_WIDTH
    
} WGL_TTX_HDR_PNL_HZ_POS_MODE_T;

/* WGL_TTX_HZ_DISP_MODE_T */
/*------------------------------------------------------------------*/
/*! @enum WGL_TTX_HZ_DISP_MODE_T
 *  @brief  Define the ttx horizontal display mode types
 *  @code
 *  typedef enum _WGL_TTX_HZ_DISP_MODE_T
 *  {
 *      WGL_TTX_HZ_DM_SCRN_WIDTH = 0,
 *      WGL_TTX_HZ_DM_LEFT_HALF,
 *      WGL_TTX_HZ_DM_RIGHT_HALF,
 *      WGL_TTX_HZ_DM_ANY_WIDTH
 *  } WGL_TTX_HZ_DISP_MODE_T;
 *  @endcode
 *  @li@c  WGL_TTX_HZ_DM_SCRN_WIDTH       -Define ttx displays on the whole screen in horizontal
 *  @li@c  WGL_TTX_HZ_DM_LEFT_HALF           -Define ttx displays on the left half region in horizontal 
 *  @li@c  WGL_TTX_HZ_DM_RIGHT_HALF        -Define ttx displays on the right half region in horizontal
 *  @li@c  WGL_TTX_HZ_DM_ANY_WIDTH         -Define ttx display on any special region in the screen in horizontal
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_TTX_HZ_DISP_MODE_T
{
    WGL_TTX_HZ_DM_SCRN_WIDTH = 0,
    WGL_TTX_HZ_DM_LEFT_HALF,
    WGL_TTX_HZ_DM_RIGHT_HALF,
    WGL_TTX_HZ_DM_ANY_WIDTH
    
} WGL_TTX_HZ_DISP_MODE_T;

/* WGL_TTX_VT_DISP_MODE_T */
/*------------------------------------------------------------------*/
/*! @enum WGL_TTX_VT_DISP_MODE_T
 *  @brief Define ttx vertical display mode 
 *  @code
 *  typedef enum _WGL_TTX_VT_DISP_MODE_T
 *  {
 *      WGL_TTX_VT_DM_SCRN_HEIGHT = 0,
 *      WGL_TTX_VT_DM_DBL_UPPER,
 *      WGL_TTX_VT_DM_DBL_LOWER,
 *      WGL_TTX_VT_DM_DBL_SCRLED
 *  } WGL_TTX_VT_DISP_MODE_T;
 *  @endcode
 *  @li@c  WGL_TTX_VT_DM_SCRN_HEIGHT   -Define vertical display is on screen 
 *  @li@c  WGL_TTX_VT_DM_DBL_UPPER       -Define vertical display is on upper region while double mode
 *  @li@c  WGL_TTX_VT_DM_DBL_LOWER      -Define vertical display is on lower region while double mode
 *  @li@c  WGL_TTX_VT_DM_DBL_SCRLED     -Define vertical display is by scroll define region
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_TTX_VT_DISP_MODE_T
{
    WGL_TTX_VT_DM_SCRN_HEIGHT = 0,
    WGL_TTX_VT_DM_DBL_UPPER,
    WGL_TTX_VT_DM_DBL_LOWER,
    WGL_TTX_VT_DM_DBL_SCRLED
    
} WGL_TTX_VT_DISP_MODE_T;

/* WGL_TTX_APP_PNL_CELL_NUM_MODE_T */
/*------------------------------------------------------------------*/
/*! @enum WGL_TTX_APP_PNL_CELL_NUM_MODE_T
 *  @brief Define ttx app panel cells num mode
 *  @code
 *  typedef enum _WGL_TTX_APP_PNL_CELL_NUM_MODE_T
 *  {
 *      WGL_TTX_APP_PNL_CELL_NUM_NO_SIDE_PNL    = WGL_TTX_MAIN_PNL_CELL_NUM,
 *      WGL_TTX_APP_PNL_CELL_NUM_WITH_SIDE_PNL  = (WGL_TTX_MAIN_PNL_CELL_NUM + WGL_TTX_SIDE_PNL_CELL_NUM)
 *  } WGL_TTX_APP_PNL_CELL_NUM_MODE_T;
 *  @endcode
 *  @li@c  WGL_TTX_APP_PNL_CELL_NUM_NO_SIDE_PNL        -Define app panel cells num while no side panel
 *  @li@c  WGL_TTX_APP_PNL_CELL_NUM_WITH_SIDE_PNL     -Define app panel cells num while have side panel
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_TTX_APP_PNL_CELL_NUM_MODE_T
{
    WGL_TTX_APP_PNL_CELL_NUM_NO_SIDE_PNL    = WGL_TTX_MAIN_PNL_CELL_NUM,
    WGL_TTX_APP_PNL_CELL_NUM_WITH_SIDE_PNL  = (WGL_TTX_MAIN_PNL_CELL_NUM + WGL_TTX_SIDE_PNL_CELL_NUM)
    
} WGL_TTX_APP_PNL_CELL_NUM_MODE_T;

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
/* WGL_TTX_BDR_CMD_INFO_T */
/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_BDR_CMD_INFO_T
 *  @brief This strcut define the border command information
 *  @code
 *  typedef struct _WGL_TTX_BDR_CMD_INFO_T
 *  {
 *      UINT32          ui4_bdr_cmd;
 *      VOID*           pv_param1;
 *      BOOL            b_pv_param1_not_ptr;
 *      SIZE_T          z_len_param1; 
 *      VOID*           pv_param2;
 *      BOOL            b_pv_param2_not_ptr;
 *      SIZE_T          z_len_param2; 
 *  } WGL_TTX_BDR_CMD_INFO_T;
 *  @endcode
 *  @li@c  ui4_bdr_cmd                      -Define border command type
 *  @li@c  pv_param1                        -Define the border command parameter 1 point
 *  @li@c  b_pv_param1_not_ptr         -True: parameter 1 has not point
 *  @li@c  z_len_param1                    -Definet the lenght of parameter 1 
 *  @li@c  pv_param2                        -Define the border command parameter 2 point
 *  @li@c  b_pv_param2_not_ptr         -True: parameter 2 has not point
 *  @li@c  z_len_param2                    -Definet the lenght of parameter 2
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_BDR_CMD_INFO_T
{
    UINT32          ui4_bdr_cmd;
    
    VOID*           pv_param1;
    BOOL            b_pv_param1_not_ptr;
    SIZE_T          z_len_param1; /* if (b_pv_param1_not_ptr == TRUE) z_len_param1 = sizeof(VOID *) */
    
    VOID*           pv_param2;
    BOOL            b_pv_param2_not_ptr;
    SIZE_T          z_len_param2; /* if (b_pv_param2_not_ptr == TRUE) z_len_param2 = sizeof(VOID *) */
    
} WGL_TTX_BDR_CMD_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_APP_FLASH_T 
 *  @brief  This struct contains the teletext app flash information.The min flash tick time is 1/6 
 second.
 *  @code
 *  typedef struct _WGL_TTX_APP_FLASH_T
 *  {
 *      BOOL                               b_invert_first;
 *      UINT8                              ui1_ftk_normal;
 *      UINT8                              ui1_ftk_invert;
 *  } WGL_TTX_APP_FLASH_T;    
 *  @endcode
 *  @li@c  b_invert_first                  -indicate if invert phase go before normal phase
 *  @li@c  ui1_ftk_normal               -number of flash tick for normal phase
 *  @li@c  ui1_ftk_invert                 -number of flash tick for invert phase
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_APP_FLASH_T
{
    BOOL                               b_invert_first;
    
    UINT8                              ui1_ftk_normal;
    
    UINT8                              ui1_ftk_invert;
    
} WGL_TTX_APP_FLASH_T;    


/* ui1_flag : WGL_TTX_APP_CELL_CHAR_T */
/**The flag is that app cell char background is transparent color */
#define WGL_TTX_APP_CELL_CHAR_FLAG_TRANSP_BK                MAKE_BIT_MASK_8(0)    


/* ui2_style : WGL_TTX_APP_CELL_CHAR_T */
/**The flag is that app cell char font is italic style */
#define WGL_TTX_APP_CELL_CHAR_STL_ITALIC                    MAKE_BIT_MASK_16(0)    

/**The flag is that app cell char font is bold style */
#define WGL_TTX_APP_CELL_CHAR_STL_BOLD                      MAKE_BIT_MASK_16(1)   

/**The flag is that app cell char font has underline style */
#define WGL_TTX_APP_CELL_CHAR_STL_UNDERLINE                 MAKE_BIT_MASK_16(2)   

/**The flag is that app cell char is flash mode style */
#define WGL_TTX_APP_CELL_CHAR_STL_FLASH                     MAKE_BIT_MASK_16(3)   

/* WGL_TTX_APP_CELL_CHAR_T */
/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_APP_CELL_CHAR_T
 *  @brief This struct defines app cell char information
 *  @code
 *  typedef struct _WGL_TTX_APP_CELL_CHAR_T
 *  {
 *      UINT8                               ui1_flag;
 *      UTF16_T                             w2_char;
 *      UINT8                               ui1_hgrid_char;
 *      UINT8                               ui1_vgrid_char;
 *      UINT16                              ui2_style;
 *      WGL_TTX_APP_FLASH_T                 t_flash;
 *      GL_COLOR_T                          t_clr_fg;
 *      GL_COLOR_T                          t_clr_bk;
 *  } WGL_TTX_APP_CELL_CHAR_T;
 *  @endcode
 *  @li@c  ui1_flag                             -Flags that contains the cell information(Double size,flash etc.)
 *  @li@c  w2_char                             -Char code 
 *  @li@c  ui1_hgrid_char                    -Character hgrid : indicates which hgrid of w2_char shall be drawn on this cell 
 *  @li@c  ui1_vgrid_char                    -Character vgrid : indicates which vgrid of w2_char shall be drawn on this cell 
 *  @li@c  ui2_style                            -Style(normal,italic,bold, underline,flash styple etc.)
 *  @li@c  t_flash                                -Flash parameters,valid ONLY when WGL_TTX_APP_CELL_CHAR_STYLE_FLASH is on
 *  @li@c  t_clr_fg                               -Foreground color 
 *  @li@c  t_clr_bk                              -Background color,ignored when WGL_TTX_APP_CELL_CHAR_FLAG_TRANSP_BK is on 
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_APP_CELL_CHAR_T
{
    /* flags */
    UINT8                               ui1_flag;
    
    /* char code */
    UTF16_T                             w2_char;
    
    /* character hgrid : indicates which hgrid of w2_char shall be drawn on this cell */
    UINT8                               ui1_hgrid_char;
    
    /* character vgrid : indicates which vgrid of w2_char shall be drawn on this cell */
    UINT8                               ui1_vgrid_char;
    
    /* style */
    UINT16                              ui2_style;
    
    /* flash parameters */
    /* valid ONLY when WGL_TTX_APP_CELL_CHAR_STYLE_FLASH is on */
    WGL_TTX_APP_FLASH_T                 t_flash;
    
    /* foreground color */
    GL_COLOR_T                          t_clr_fg;
    
    /* background color */
    /* ignored when WGL_TTX_APP_CELL_CHAR_FLAG_TRANSP_BK is on */
    GL_COLOR_T                          t_clr_bk;
    
} WGL_TTX_APP_CELL_CHAR_T;


/* ui1_flag : WGL_TTX_APP_CELL_IMG_T */
/**Flag is that app image cell background is transparent*/
#define WGL_TTX_APP_CELL_IMG_FLAG_TRANSP_BK                 MAKE_BIT_MASK_8(0)    

/**Flag is that app image cell is flash mode*/
#define WGL_TTX_APP_CELL_IMG_FLAG_FLASH                     MAKE_BIT_MASK_8(1)    

/* WGL_TTX_APP_CELL_IMG_T */
/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_APP_CELL_IMG_T
 *  @brief This struction contains the app image cell information
 *  @code
 *  typedef struct _WGL_TTX_APP_CELL_IMG_T
 *  {
 *      UINT8                               ui1_flag;
 *      UINT8                               ui1_align; 
 *      WGL_HIMG_TPL_T                      h_img;
 *      UINT8                               ui1_num_hgrid_img;
 *      UINT8                               ui1_num_vgrid_img;
 *      UINT8                               ui1_hgrid_img;
 *      UINT8                               ui1_vgrid_img;
 *      GL_COLOR_T                          t_clr_bk;
 *      WGL_TTX_APP_FLASH_T                 t_flash;
 *  } WGL_TTX_APP_CELL_IMG_T;
 *  @endcode
 *  @li@c  ui1_flag                             -Flags that contain app cell flags information
 *  @li@c  ui1_align                           -Define the cell alignment type
 *  @li@c  h_img                               -The image handle of the cell
 *  @li@c  ui1_num_hgrid_img            -Number of h_img's hgrid
 *  @li@c  ui1_num_vgrid_img            -Number of h_img's vgrid
 *  @li@c  ui1_hgrid_img                    -Image hgrid : indicates which hgrid of h_img shall be drawn on this cell
 *  @li@c  ui1_vgrid_img                    -Image vgrid : indicates which vgrid of h_img shall be drawn on this cell 
 *  @li@c  t_clr_bk                             -Background color,ignored when WGL_TTX_APP_CELL_CHAR_FLAG_TRANSP_BK is on
 *  @li@c  t_flash                               -Flash parameters,valid ONLY when WGL_TTX_APP_CELL_IMG_FLAG_FLASH is on
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_APP_CELL_IMG_T
{
    /* flags */
    UINT8                               ui1_flag;
    
    /* alignment */
    UINT8                               ui1_align; /* e.g. WGL_AS_LEFT_TOP */
    
    /* image handle */
    WGL_HIMG_TPL_T                      h_img;

    /* number of h_img's hgrid */
    UINT8                               ui1_num_hgrid_img;
    
    /* number of h_img's vgrid */
    UINT8                               ui1_num_vgrid_img;

    /* image hgrid : indicates which hgrid of h_img shall be drawn on this cell */
    UINT8                               ui1_hgrid_img;
    
    /* image vgrid : indicates which vgrid of h_img shall be drawn on this cell */
    UINT8                               ui1_vgrid_img;
    
    /* background color */
    /* ignored when WGL_TTX_APP_CELL_CHAR_FLAG_TRANSP_BK is on */
    GL_COLOR_T                          t_clr_bk;
    
    /* flash parameters */
    /* valid ONLY when WGL_TTX_APP_CELL_IMG_FLAG_FLASH is on */
    WGL_TTX_APP_FLASH_T                 t_flash;

} WGL_TTX_APP_CELL_IMG_T;


/* ui4_flag : WGL_TTX_APP_CELL_ATTR_OVRD_T */
/**Flag is that app cell is overlayed char */
#define WGL_TTX_APP_CELL_AO_FLAG_OVRD_CHAR                  MAKE_BIT_MASK_32(0)    

/**Flag is that app overlayed cell is italic style*/
#define WGL_TTX_APP_CELL_AO_FLAG_OVRD_STL_ITALIC            MAKE_BIT_MASK_32(1)    

/**Flag is that app overlayed cell is bold style*/
#define WGL_TTX_APP_CELL_AO_FLAG_OVRD_STL_BOLD              MAKE_BIT_MASK_32(2)    

/**Flag is that app overlayed cell is underline style*/
#define WGL_TTX_APP_CELL_AO_FLAG_OVRD_STL_UNDERLINE         MAKE_BIT_MASK_32(3)    

/**Flag is that app overlayed cell is flash style*/
#define WGL_TTX_APP_CELL_AO_FLAG_OVRD_STL_FLASH             MAKE_BIT_MASK_32(4)    

/**Flag is that app overlayed cell has foreground color*/
#define WGL_TTX_APP_CELL_AO_FLAG_OVRD_FG_CLR                MAKE_BIT_MASK_32(5)    

/**Flag is that app overlayed cell has background color*/
#define WGL_TTX_APP_CELL_AO_FLAG_OVRD_BK_CLR                MAKE_BIT_MASK_32(6)    

/* WGL_TTX_APP_CELL_ATTR_OVRD_T */
/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_APP_CELL_ATTR_OVRD_T
 *  @brief This struct contains app overlay cell information 
 *  @code
 *  typedef struct _WGL_TTX_APP_CELL_ATTR_OVRD_T
 *  {
 *      UINT32                              ui4_flag;
 *      UTF16_T                             w2_char;
 *      UINT8                               ui1_hgrid_char;
 *      UINT8                               ui1_vgrid_char;
 *      UINT16                              ui2_style;
 *      WGL_TTX_APP_FLASH_T                 t_flash;
 *      GL_COLOR_T                          t_clr_fg;
 *      GL_COLOR_T                          t_clr_bk;
 *  } WGL_TTX_APP_CELL_ATTR_OVRD_T;
 *  @endcode
 *  @li@c  ui4_flag                        -Flags contain overlay cell information flags
 *  @li@c  w2_char                        -Character,valid only when ui4_flag : WGL_TTX_APP_CELL_AO_FLAG_OVRD_CHAR is on
 *  @li@c  ui1_hgrid_char               -Character hgrid:indicates which hgrid of w2_char shall be drawn on this cell,
 valid only when ui4_flag : WGL_TTX_APP_CELL_AO_FLAG_OVRD_CHAR is on
 *  @li@c  ui1_vgrid_char               -Character vgrid : indicates which vgrid of w2_char shall be drawn on this 
 cell,valid only when ui4_flag : WGL_TTX_APP_CELL_AO_FLAG_OVRD_CHAR is on
 *  @li@c  ui2_style                       -Cell style,valid only when ui4_flag : 
 WGL_TTX_APP_CELL_AO_FLAG_OVRD_STL_[ITALIC|BOLD|UNDERLINE] is on
 *  @li@c  t_flash                          -Flash parameters,valid only when ui2_style : 
 WGL_TTX_APP_CELL_CHAR_STL_FLASH is on
 *  @li@c  t_clr_fg                         -Foreground color,alid only when ui4_flag : 
 WGL_TTX_APP_CELL_AO_FLAG_OVRD_FG_CLR is on
 *  @li@c  t_clr_bk                        -Backgound color,valid only when ui4_flag : 
 WGL_TTX_APP_CELL_AO_FLAG_OVRD_BK_CLR is on 
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_APP_CELL_ATTR_OVRD_T
{
    /* flags */
    UINT32                              ui4_flag;

    /* character */
    /* valid only when ui4_flag : WGL_TTX_APP_CELL_AO_FLAG_OVRD_CHAR is on */
    UTF16_T                             w2_char;

    /* character hgrid : indicates which hgrid of w2_char shall be drawn on this cell */
    /* valid only when ui4_flag : WGL_TTX_APP_CELL_AO_FLAG_OVRD_CHAR is on */
    UINT8                               ui1_hgrid_char;
    
    /* character vgrid : indicates which vgrid of w2_char shall be drawn on this cell */
    /* valid only when ui4_flag : WGL_TTX_APP_CELL_AO_FLAG_OVRD_CHAR is on */
    UINT8                               ui1_vgrid_char;
    
    /* style */
    /* valid only when ui4_flag : WGL_TTX_APP_CELL_AO_FLAG_OVRD_STL_[ITALIC|BOLD|UNDERLINE] is on */
    UINT16                              ui2_style;
    
    /* flash parameters */
    /* valid only when ui2_style : WGL_TTX_APP_CELL_CHAR_STL_FLASH is on */
    WGL_TTX_APP_FLASH_T                 t_flash;
    
    /* foreground color */
    /* valid only when ui4_flag : WGL_TTX_APP_CELL_AO_FLAG_OVRD_FG_CLR is on */
    GL_COLOR_T                          t_clr_fg;
    
    /* background color */
    /* valid only when ui4_flag : WGL_TTX_APP_CELL_AO_FLAG_OVRD_BK_CLR is on */
    GL_COLOR_T                          t_clr_bk;
    
} WGL_TTX_APP_CELL_ATTR_OVRD_T;


/* WGL_TTX_APP_CELL_DATA_T */ 
/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_APP_CELL_DATA_T
 *  @brief This struct contains the app cell data information
 *  @code
 typedef union _WGL_TTX_APP_CELL_DATA_T
{
    WGL_TTX_APP_CELL_CHAR_T             t_char;   
    WGL_TTX_APP_CELL_IMG_T              t_img;
    WGL_TTX_APP_CELL_ATTR_OVRD_T        t_attr_ovrd;
} WGL_TTX_APP_CELL_DATA_T;  
 *  @endcode
 *  @li@c  t_char                -Contains the app char cell information
 *  @li@c  t_img                 -Contains the app image cell  information
 *  @li@c  t_attr_ovrd         -Contains the app overlay cell  information
 */
/*------------------------------------------------------------------*/
 typedef union _WGL_TTX_APP_CELL_DATA_T
{
    /* WGL_TTX_APP_CELL_TYPE_CHAR */
    WGL_TTX_APP_CELL_CHAR_T             t_char;   
    
    /* WGL_TTX_APP_CELL_TYPE_IMG */
    WGL_TTX_APP_CELL_IMG_T              t_img;
    
    /* WGL_TTX_APP_CELL_TYPE_ATTR_OVRD */
    WGL_TTX_APP_CELL_ATTR_OVRD_T        t_attr_ovrd;
    
} WGL_TTX_APP_CELL_DATA_T;    


/* WGL_TTX_APP_CELL_T */
/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_APP_CELL_T
 *  @brief This struct contain app cell information
 *  @code
 *  typedef struct _WGL_TTX_APP_CELL_T
 *  {
 *      UINT8                               ui1_flag;
 *      WGL_TTX_APP_CELL_TYPE_T             e_type;
 *      WGL_TTX_APP_CELL_DATA_T             u_data;
 *  } WGL_TTX_APP_CELL_T;    
 *  @endcode
 *  @li@c  ui1_flag                         -Flag that contains the cell information flags
 *  @li@c  e_type                           -Define the app cell type(char,image,overlay etc)
 *  @li@c  u_data                           -Define the app cell data information
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_APP_CELL_T
{
    /* flags */
    /* currently no flag defined */
    UINT8                               ui1_flag;
    
    /* app cell type */
    WGL_TTX_APP_CELL_TYPE_T             e_type;
    
    /* data */
    WGL_TTX_APP_CELL_DATA_T             u_data;
    
} WGL_TTX_APP_CELL_T;    


/* ui1_flag : WGL_TTX_APP_PNL_T */
/**Flag is that app panel full backgroud color flag */
#define WGL_TTX_APP_PNL_FLAG_FULL_BK_COLOR              MAKE_BIT_MASK_8(0)    

/**Flag is that app cell can be ignore data */
#define WGL_TTX_APP_PNL_FLAG_IGNORE_CELL_DATA           MAKE_BIT_MASK_8(1)    

/* WGL_TTX_APP_PNL_T */
/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_APP_PNL_T
 *  @brief This struct defines app panel information
 *  @code
 *  typedef struct _WGL_TTX_APP_PNL_T
 *  {
 *      UINT8                                                   ui1_flag;
 *      WGL_TTX_APP_PNL_HZ_POS_MODE_T       e_hz_pos;
 *      WGL_TTX_APP_PNL_DISP_MODE_T            e_disp_mode;
 *      GL_COLOR_T                                          t_clr_bk;
 *      WGL_TTX_APP_PNL_CELL_NUM_MODE_T    e_num_cell;
 *      WGL_TTX_APP_CELL_T                             at_cells[WGL_TTX_APP_MAX_CELL_NUM];
 *  } WGL_TTX_APP_PNL_T;    
 *  @endcode
 *  @li@c  ui1_flag                         -Flag that contains the app panel flags information
 *  @li@c  e_hz_pos                       -Define the horizontal position mode
 *  @li@c  e_disp_mode                  -Define the display mode
 *  @li@c  t_clr_bk                         -Define the app panel background color
 *  @li@c  e_num_cell                     -Define the app panel cells number
 *  @li@c  at_cells[WGL_TTX_APP_MAX_CELL_NUM]    -Contains app panel cells information
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_APP_PNL_T
{
    /* flags */
    UINT8                               ui1_flag;
    
    /* horizontal position */
    WGL_TTX_APP_PNL_HZ_POS_MODE_T       e_hz_pos;
    
    /* display mode */
    WGL_TTX_APP_PNL_DISP_MODE_T         e_disp_mode;
    
    /* panel background color */ 
    /* valid ONLY if "WGL_TTX_APP_PNL_FLAG_FULL_BK_COLOR" is specified */
    GL_COLOR_T                          t_clr_bk;
    
    /* cell number */
    WGL_TTX_APP_PNL_CELL_NUM_MODE_T     e_num_cell;
    
    /* cell data */
    WGL_TTX_APP_CELL_T                  at_cells[WGL_TTX_APP_MAX_CELL_NUM];
    
} WGL_TTX_APP_PNL_T;    


/* ui1_flag : WGL_TTX_APP_MASK_T */
/**Flag is that app mask side panel cells flag*/
#define WGL_TTX_APP_MASK_FLAG_SIDE_PNL_CELLS                     MAKE_BIT_MASK_8(0)    

/* WGL_TTX_APP_MASK_T */
/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_APP_MASK_T
 *  @brief  This struct contains app mask panel information
 *  @code
 *  typedef struct _WGL_TTX_APP_MASK_T
 *  {
 *      UINT8                                         ui1_flag;
 *      WGL_TTX_APP_CELL_T                  at_cells_mp[WGL_TTX_MAIN_PNL_CELL_NUM];
 *      WGL_TTX_APP_CELL_T                  at_cells_sp[WGL_TTX_SIDE_PNL_CELL_NUM];
 *  } WGL_TTX_APP_MASK_T;
 *  @endcode
 *  @li@c  ui1_flag                                          -Flag that contains app mask information flags
 *  @li@c  at_cells_mp[WGL_TTX_MAIN_PNL_CELL_NUM]        -The main panel of app mask data information
 *  @li@c  at_cells_sp[WGL_TTX_SIDE_PNL_CELL_NUM]          -The side panel of app mask data information
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_APP_MASK_T
{
    UINT8                               ui1_flag;
    
    /* cell data for main panel */
    WGL_TTX_APP_CELL_T                  at_cells_mp[WGL_TTX_MAIN_PNL_CELL_NUM];

    /* cell data for side panel */
    WGL_TTX_APP_CELL_T                  at_cells_sp[WGL_TTX_SIDE_PNL_CELL_NUM];
    
} WGL_TTX_APP_MASK_T;


/* WGL_TTX_MARGIN_T */
/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_MARGIN_T
 *  @brief This struct defines the ttx margin information
 *  @code
 *  typedef struct _WGL_TTX_MARGIN_T
 *  {
 *      UINT16                              ui2_left;
 *      UINT16                              ui2_top;
 *      UINT16                              ui2_right;
 *      UINT16                              ui2_bottom;
 *  } WGL_TTX_MARGIN_T;
 *  @endcode
 *  @li@c  ui2_left                        -Define left of margin
 *  @li@c  ui2_top                        -Define top of margin
 *  @li@c  ui2_right                      -Define right of margin
 *  @li@c  ui2_bottom                   -Define bottom of margin
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_MARGIN_T
{
    UINT16                              ui2_left;
    UINT16                              ui2_top;
    UINT16                              ui2_right;
    UINT16                              ui2_bottom;
    
} WGL_TTX_MARGIN_T;


/* WGL_TTX_VP_DIM_INFO_MAP_KEY_T */
/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_VP_DIM_INFO_MAP_KEY_T
 *  @brief This struct defines the viewport information
 *  @code
 *  typedef struct _WGL_TTX_VP_DIM_INFO_MAP_KEY_T
 *  {
 *      GL_SIZE_T                           t_sz_vp_logical;
 *      WGL_TTX_HGRID_NUM_T                 e_num_hgrid;
 *      UINT8                               ui1_num_vgrid;
 *  } WGL_TTX_VP_DIM_INFO_MAP_KEY_T;
 *  @endcode
 *  @li@c  t_sz_vp_logical                   -Define the size of viewport
 *  @li@c  e_num_hgrid                      -Define the hgrid number in the viewport 
 *  @li@c  ui1_num_vgrid                   -Define the vgrid number in the viewport
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_VP_DIM_INFO_MAP_KEY_T
{
    /* size of viewport */
    GL_SIZE_T                           t_sz_vp_logical;
    
    /* hgrid number in the viewport */
    WGL_TTX_HGRID_NUM_T                 e_num_hgrid;
    
    /* vgrid number in the viewport */
    UINT8                               ui1_num_vgrid;
    
} WGL_TTX_VP_DIM_INFO_MAP_KEY_T;


/* WGL_TTX_VP_CFG_T */
/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_VP_CFG_T
 *  @brief This struct contains the viewport config information
 *  @code
 *  typedef struct _WGL_TTX_VP_CFG_T
 *  {
 *      WGL_TTX_MARGIN_T                             t_margin_vp_no_side_pnl;
 *      WGL_TTX_VP_DIM_INFO_MAP_KEY_T       t_key_no_side_pnl;
 *      BOOL                                                   b_scale_no_side_pnl;
 *      WGL_TTX_MARGIN_T                             t_margin_vp_side_pnl;
 *      WGL_TTX_VP_DIM_INFO_MAP_KEY_T       t_key_with_side_pnl;
 *      BOOL                                                    b_scale_with_side_pnl;
 *  } WGL_TTX_VP_CFG_T;
 *  @endcode
 *  @li@c  t_margin_vp_no_side_pnl      -Define margine of viewpoint for no side panel case 
 *  @li@c  t_key_no_side_pnl                -Define the viewpoint config information for no side panel case
 *  @li@c  b_scale_no_side_pnl             -True:permit to scale for no side panel case
 *  @li@c  t_margin_vp_side_pnl           -Define margine of viewpoint for side panel case 
 *  @li@c  t_key_with_side_pnl              -Define the viewpoint config information for side panel case
 *  @li@c  b_scale_with_side_pnl           -True:permit to scale for side panel case
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_VP_CFG_T
{
    /* viewport config for no side panel case */
    WGL_TTX_MARGIN_T                    t_margin_vp_no_side_pnl;
    WGL_TTX_VP_DIM_INFO_MAP_KEY_T       t_key_no_side_pnl;
    BOOL                                b_scale_no_side_pnl;
    
    /* viewport config for side panel case */
    WGL_TTX_MARGIN_T                    t_margin_vp_side_pnl;
    WGL_TTX_VP_DIM_INFO_MAP_KEY_T       t_key_with_side_pnl;
    BOOL                                b_scale_with_side_pnl;
    
} WGL_TTX_VP_CFG_T;

/* ui1_flag : WGL_TTX_HZ_DM_PARAM_SCRN_W_T */
/**Flag is that horizontal position mode can be fro header panel when side panel is enabled*/
#define WGL_TTX_HZ_DM_PARAM_SCRN_W_FLAG_EXCLUDE_SP_ROW_0        MAKE_BIT_MASK_8(0)    

/* WGL_TTX_HZ_DM_PARAM_SCRN_W_T */
/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_HZ_DM_PARAM_SCRN_W_T
 *  @brief This struct define the horizontal position screen mode.
 *  @code
 *  typedef struct _WGL_TTX_HZ_DM_PARAM_SCRN_W_T
 *  {
 *      UINT8                                                     ui1_flag;  
 *      WGL_TTX_HDR_PNL_HZ_POS_MODE_T       e_hz_pos_hdr_pnl_no_sp_row_0;
 *  } WGL_TTX_HZ_DM_PARAM_SCRN_W_T;    
 *  @endcode
 *  @li@c  ui1_flag                                          -Flags for screen horizontal posittion mode
 *  @li@c  e_hz_pos_hdr_pnl_no_sp_row_0       -Horizontal position mode for header panel (TTX row 0)
 when side panel is enabled,valid if (  side_panel_not_suppressed ui1_flag & 
 WGL_TTX_HZ_DM_PARAM_SCRN_W_FLAG_EXCLUDE_SP_ROW_0 )
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_HZ_DM_PARAM_SCRN_W_T
{
    /* flags */
    UINT8                               ui1_flag;  
    
    /* horizontal position mode for header panel (TTX row 0) when side panel is enabled */
    /* valid if (  side_panel_not_suppressed                                 &&  */
    /*             ui1_flag & WGL_TTX_HZ_DM_PARAM_SCRN_W_FLAG_EXCLUDE_SP_ROW_0 ) */
    WGL_TTX_HDR_PNL_HZ_POS_MODE_T       e_hz_pos_hdr_pnl_no_sp_row_0;
    
} WGL_TTX_HZ_DM_PARAM_SCRN_W_T;    


/* ui1_flag : WGL_TTX_HZ_DM_PARAM_HALF_SCRN_W_T */
/**Flag is that suppress side panel for half screen mode*/
#define WGL_TTX_HZ_DM_PARAM_HALF_SCRN_W_FLAG_SUPPRESS_SP        MAKE_BIT_MASK_8(0)    

/**Flag is that execlude the side header panel */
#define WGL_TTX_HZ_DM_PARAM_HALF_SCRN_W_FLAG_EXCLUDE_SP_ROW_0   MAKE_BIT_MASK_8(1)    


/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_HZ_DM_PARAM_HALF_SCRN_W_T
 *  @brief This struct contains the horizontal position for screen width information. 
 *  @code
 *  typedef struct _WGL_TTX_HZ_DM_PARAM_HALF_SCRN_W_T
 *  {
 *      UINT8                                                     ui1_flag;
 *      WGL_TTX_HDR_PNL_HZ_POS_MODE_T       e_hz_pos_hdr_pnl_no_sp_row_0;
 *  } WGL_TTX_HZ_DM_PARAM_HALF_SCRN_W_T;
 *  @endcode
 *  @li@c  ui1_flag                                          -Flags
 *  @li@c  e_hz_pos_hdr_pnl_no_sp_row_0        -horizontal position mode for header panel 
 (TTX row 0) when side panel is enabled
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_HZ_DM_PARAM_HALF_SCRN_W_T
{
    /* flags */
    UINT8                               ui1_flag;
    
    /* horizontal position mode for header panel (TTX row 0) when side panel is enabled */
    /* valid if ( side_panel_not_suppressed                                     &&   */
    /*            ui1_flag & WGL_TTX_HZ_DM_PARAM_HALF_SCRN_W_FLAG_EXCLUDE_SP_ROW_0 ) */
    WGL_TTX_HDR_PNL_HZ_POS_MODE_T       e_hz_pos_hdr_pnl_no_sp_row_0;
    
} WGL_TTX_HZ_DM_PARAM_HALF_SCRN_W_T;


/* ui1_flag : WGL_TTX_HZ_DM_PARAM_ANY_W_T */
/**Flag is that suppress side panel for any width mode*/
#define WGL_TTX_HZ_DM_PARAM_ANY_W_FLAG_SUPPRESS_SP              MAKE_BIT_MASK_8(0)    

/**Flag is that execlude the side header panel for any width mode*/
#define WGL_TTX_HZ_DM_PARAM_ANY_W_FLAG_EXCLUDE_SP_ROW_0         MAKE_BIT_MASK_8(1)    

/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_HZ_DM_PARAM_ANY_W_T
 *  @brief This struct contain horizontal position information for anay width screen width mode.The 
 flag maybe WGL_TTX_HZ_DM_PARAM_ANY_W_FLAG_SUPPRESS_SP or 
 WGL_TTX_HZ_DM_PARAM_ANY_W_FLAG_EXCLUDE_SP_ROW_0 mode.
 *  @code
 *  typedef struct _WGL_TTX_HZ_DM_PARAM_ANY_W_T
 *  {
 *      UINT8                                                   ui1_flag;
 *      INT32                                                   i4_left;
 *      INT32                                                   i4_right;
 *      WGL_TTX_HDR_PNL_HZ_POS_MODE_T     e_hz_pos_hdr_pnl_no_sp_row_0;
 *  } WGL_TTX_HZ_DM_PARAM_ANY_W_T;    
 *  @endcode
 *  @li@c  ui1_flag                                       -Flags
 *  @li@c  i4_left                                         -Coordinate of widget's left boundary (in screen coordinate) 
 *  @li@c  i4_right                                       -Coordinate of widget's right boundary (in screen coordinate) 
 *  @li@c  e_hz_pos_hdr_pnl_no_sp_row_0    -Horizontal position mode for header panel (TTX row 0) when side panel is enabled
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_HZ_DM_PARAM_ANY_W_T
{
    /* flags */
    UINT8                               ui1_flag;
    
    /* coordinate of widget's left boundary (in screen coordinate) */
    INT32                               i4_left;
    
    /* coordinate of widget's right boundary (in screen coordinate) */
    INT32                               i4_right;
    
    /* horizontal position mode for header panel (TTX row 0) when side panel is enabled */
    /* valid if ( side_panel_not_suppressed                                 && */
    /*            ui1_flag & WGL_TTX_HZ_DM_PARAM_ANY_W_FLAG_EXCLUDE_SP_ROW_0 ) */
    WGL_TTX_HDR_PNL_HZ_POS_MODE_T       e_hz_pos_hdr_pnl_no_sp_row_0;
    
} WGL_TTX_HZ_DM_PARAM_ANY_W_T;    

/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_HZ_DISP_MODE_PARAM_T
 *  @brief This struct contains the horizontal display mode parameter information.
 *  @code
*   typedef union _WGL_TTX_HZ_DISP_MODE_PARAM_T
*   {
*      WGL_TTX_HZ_DM_PARAM_SCRN_W_T        t_scrn_w;    
*      WGL_TTX_HZ_DM_PARAM_HALF_SCRN_W_T   t_left_half;
*      WGL_TTX_HZ_DM_PARAM_HALF_SCRN_W_T   t_right_half;
*      WGL_TTX_HZ_DM_PARAM_ANY_W_T         t_any_w;
*   } WGL_TTX_HZ_DISP_MODE_PARAM_T;    
 *  @endcode
 *  @li@c t_scrn_w      -Contains screen width information if it is WGL_TTX_HZ_DM_SCRN_WIDTH mode.
 *  @li@c t_left_half    -Contains left screen width information if it is WGL_TTX_HZ_DM_LEFT_HALF mode.
 *  @li@c t_right_half  -Contains right screen width information if it is WGL_TTX_HZ_DM_RIGHT_HALF mode.
 *  @li@c t_any_w      -Contains any screen width information if it is WGL_TTX_HZ_DM_ANY_WIDTH mode.
 */
/*------------------------------------------------------------------*/
typedef union _WGL_TTX_HZ_DISP_MODE_PARAM_T
{
    /* WGL_TTX_HZ_DM_SCRN_WIDTH */
    WGL_TTX_HZ_DM_PARAM_SCRN_W_T        t_scrn_w;    
    
    /* WGL_TTX_HZ_DM_LEFT_HALF */
    WGL_TTX_HZ_DM_PARAM_HALF_SCRN_W_T   t_left_half;
    
    /* WGL_TTX_HZ_DM_RIGHT_HALF */
    WGL_TTX_HZ_DM_PARAM_HALF_SCRN_W_T   t_right_half;
    
    /* WGL_TTX_HZ_DM_ANY_WIDTH */
    WGL_TTX_HZ_DM_PARAM_ANY_W_T         t_any_w;
    
} WGL_TTX_HZ_DISP_MODE_PARAM_T;    


/* WGL_TTX_HZ_DISP_INFO_T */
/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_HZ_DISP_INFO_T
 *  @brief This struct defines the horizontal display information
 *  @code
 *  typedef struct _WGL_TTX_HZ_DISP_INFO_T
 *  {
 *      WGL_TTX_HZ_DISP_MODE_T              e_disp_mode;
 *      WGL_TTX_HZ_DISP_MODE_PARAM_T        u_param;
 *  } WGL_TTX_HZ_DISP_INFO_T;
 *  @endcode
 *  @li@c  e_disp_mode              -Horizontal display mode
 *  @li@c  u_param                    -Display mode parameters data information
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_HZ_DISP_INFO_T
{
    /* horizontal display mode */
    WGL_TTX_HZ_DISP_MODE_T              e_disp_mode;
    
    /* display mode parameters */
    WGL_TTX_HZ_DISP_MODE_PARAM_T        u_param;
    
} WGL_TTX_HZ_DISP_INFO_T;



/* tail row (24,25) area slot content type */
/*------------------------------------------------------------------*/
/*! @enum WGL_TTX_MP_TR_SLOT_CNT_TYPE_T
 *  @brief This struct define tail row (24,25) area slot content type 
 *  @code
 *  typedef enum _WGL_TTX_MP_TR_SLOT_CNT_TYPE_T
 *  {
 *      TR_SLOT_CNT_TYPE_IGNORE,
 *      TR_SLOT_CNT_TYPE_ROW_COLOR,
 *      TR_SLOT_CNT_TYPE_ROW_24,
 *      TR_SLOT_CNT_TYPE_ROW_25
 *  } WGL_TTX_MP_TR_SLOT_CNT_TYPE_T;
 *  @endcode
 *  @li@c  TR_SLOT_CNT_TYPE_IGNORE                 -Ingore
 *  @li@c  TR_SLOT_CNT_TYPE_ROW_COLOR          -Define the tail row color type
 *  @li@c  TR_SLOT_CNT_TYPE_ROW_24                -Define the row 24 content type 
 *  @li@c  TR_SLOT_CNT_TYPE_ROW_25                -Define the row 25 content type 
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_TTX_MP_TR_SLOT_CNT_TYPE_T
{
    TR_SLOT_CNT_TYPE_IGNORE,
    TR_SLOT_CNT_TYPE_ROW_COLOR,
    TR_SLOT_CNT_TYPE_ROW_24,
    TR_SLOT_CNT_TYPE_ROW_25
   
} WGL_TTX_MP_TR_SLOT_CNT_TYPE_T;

/* tail row (24,25) information */
/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_MP_TR_INFO_T
 *  @brief This struct define tail row (24,25) area information
 *  @code
 *  typedef struct _WGL_TTX_MP_TR_INFO_T
 *  {
 *      UINT8                               ui1_num_row_min; 
 *      UINT8                               ui1_num_row_max;
 *      WGL_TTX_MP_TR_SLOT_CNT_TYPE_T       e_slot_1_N24_N25;
 *      WGL_TTX_MP_TR_SLOT_CNT_TYPE_T       e_slot_2_N24_N25;
 *      WGL_TTX_MP_TR_SLOT_CNT_TYPE_T       e_slot_1_N24_Y25;
 *      WGL_TTX_MP_TR_SLOT_CNT_TYPE_T       e_slot_2_N24_Y25;
 *      WGL_TTX_MP_TR_SLOT_CNT_TYPE_T       e_slot_1_Y24_N25;
 *      WGL_TTX_MP_TR_SLOT_CNT_TYPE_T       e_slot_2_Y24_N25;
 *      WGL_TTX_MP_TR_SLOT_CNT_TYPE_T       e_slot_1_Y24_Y25;
 *      WGL_TTX_MP_TR_SLOT_CNT_TYPE_T       e_slot_2_Y24_Y25;
 *  } WGL_TTX_MP_TR_INFO_T;
 *  @endcode
 *  @li@c  ui1_num_row_min             -Minimal row number for reserving the tail-row space (0 ~ 2)
 *  @li@c  ui1_num_row_max            -Maximal row number for reserving the tail-row space (ui1_num_row_min ~ 2)
 *  @li@c  e_slot_1_N24_N25            -Slot content designation
 *  @li@c  e_slot_2_N24_N25            -Slot content designation
 *  @li@c  e_slot_1_N24_Y25            -Slot content designation
 *  @li@c  e_slot_2_N24_Y25            -Slot content designation
 *  @li@c  e_slot_1_Y24_N25            -Slot content designation
 *  @li@c  e_slot_2_Y24_N25            -Slot content designation
 *  @li@c  e_slot_1_Y24_Y25            -Slot content designation
 *  @li@c  e_slot_2_Y24_Y25            -Slot content designation
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_MP_TR_INFO_T
{
    /* minimal row number for reserving the tail-row space (0 ~ 2) */
    UINT8                               ui1_num_row_min; 
    
    /* maximal row number for reserving the tail-row space (ui1_num_row_min ~ 2) */
    UINT8                               ui1_num_row_max;

    /* slot content designation */
    WGL_TTX_MP_TR_SLOT_CNT_TYPE_T       e_slot_1_N24_N25;
    WGL_TTX_MP_TR_SLOT_CNT_TYPE_T       e_slot_2_N24_N25;
  
    WGL_TTX_MP_TR_SLOT_CNT_TYPE_T       e_slot_1_N24_Y25;
    WGL_TTX_MP_TR_SLOT_CNT_TYPE_T       e_slot_2_N24_Y25;

    WGL_TTX_MP_TR_SLOT_CNT_TYPE_T       e_slot_1_Y24_N25;
    WGL_TTX_MP_TR_SLOT_CNT_TYPE_T       e_slot_2_Y24_N25;

    WGL_TTX_MP_TR_SLOT_CNT_TYPE_T       e_slot_1_Y24_Y25;
    WGL_TTX_MP_TR_SLOT_CNT_TYPE_T       e_slot_2_Y24_Y25;
    
} WGL_TTX_MP_TR_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_VT_DM_PARAM_SCRN_H_T
 *  @brief  This struction contains teletext widget vertical screen information.
 *  @code
 *  typedef struct _WGL_TTX_VT_DM_PARAM_SCRN_H_T
 *  {
 *      UINT8                               ui1_flag;  
 *      UINT8                               ui1_num_vgrid;
 *      UINT8                               ui1_vgrid_hp;
 *      UINT8                               ui1_vgrid_mp_start;
 *      UINT8                               ui1_vgrid_mp_tr_start;
 *      WGL_TTX_MP_TR_INFO_T                t_mp_tr_info;
 *  } WGL_TTX_VT_DM_PARAM_SCRN_H_T;
 *  @endcode
 *  @li@c  ui1_flag                         -Flags
 *  @li@c  ui1_num_vgrid               -Vgrid number for the viewport
 *  @li@c  ui1_vgrid_hp                  -Header panel vgrid
 *  @li@c  ui1_vgrid_mp_start         -Main panel start vgrid (ttx-row: 1 ~ 23)
 *  @li@c  ui1_vgrid_mp_tr_start     -Main panel tail row (ttx-row : 24 and/or 25) position & info
 *  @li@c  t_mp_tr_info                  -Contain main panel of tail row information
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_VT_DM_PARAM_SCRN_H_T
{
    /* flags */
    /* currently no flag available */
    UINT8                               ui1_flag;  
    
    /* vgrid number for the viewport */
    UINT8                               ui1_num_vgrid;
    
    /* header panel vgrid */
    UINT8                               ui1_vgrid_hp;
    
    /* main panel start vgrid */
    /* main panel ExTR area start: (ttx-row: 1 ~ 23) */
    UINT8                               ui1_vgrid_mp_start;
    
    /* main panel tail row (ttx-row : 24 and/or 25) position & info */
    UINT8                               ui1_vgrid_mp_tr_start;
    WGL_TTX_MP_TR_INFO_T                t_mp_tr_info;

} WGL_TTX_VT_DM_PARAM_SCRN_H_T;


/* ui4_flag : WGL_TTX_VT_DM_PARAM_DBL_SCRN_H_T */
/**Flag is that header panel is hide for vertical display mode*/
#define WGL_TTX_VT_DM_PARAM_DBL_SCRN_H_FLAG_HP_HIDE         MAKE_BIT_MASK_32(0)    

/**Flag is that header panel is double height at double screen for vertical display mode*/
#define WGL_TTX_VT_DM_PARAM_DBL_SCRN_H_FLAG_HP_DBL_H        MAKE_BIT_MASK_32(1)    

/**Flag is that header panel shows tail row of main panel at double screen for vertical display mode*/
#define WGL_TTX_VT_DM_PARAM_DBL_SCRN_H_FLAG_MP_TR_SHOW      MAKE_BIT_MASK_32(2)    

/**Flag is that header panel shows double tail height of main panel for vertical display mode*/
#define WGL_TTX_VT_DM_PARAM_DBL_SCRN_H_FLAG_MP_TR_DBL_H     MAKE_BIT_MASK_32(3)    

/* WGL_TTX_VT_DM_PARAM_DBL_SCRN_H_T */
/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_VT_DM_PARAM_DBL_SCRN_H_T
 *  @brief Thise struct defines double screen information for vertical display mode.
 *  @code
 *  typedef struct _WGL_TTX_VT_DM_PARAM_DBL_SCRN_H_T
 *  {
 *      UINT32                              ui4_flag;
 *      UINT8                               ui1_num_vgrid;
 *      UINT8                               ui1_vgrid_hp_start;
 *      UINT8                               ui1_vgrid_mp_extr_start;
 *      UINT8                               ui1_vgrid_mp_extr_end;
 *      UINT8                               ui1_ttx_row_mp_extr_start;
 *      UINT8                               ui1_ttx_row_mp_extr_end;
 *      UINT8                               ui1_vgrid_mp_tr_start;
 *      WGL_TTX_MP_TR_INFO_T                t_mp_tr_info;
 *  } WGL_TTX_VT_DM_PARAM_DBL_SCRN_H_T;
 *  @endcode
 *  @li@c  ui4_flag                                 -Flags
 *  @li@c  ui1_num_vgrid                       -The vgrid number for vertical display mode
 *  @li@c  ui1_vgrid_hp_start                  -Header panel (ttx row 0) parameters
              flag: WGL_TTX_VT_DM_PARAM_DBL_SCRN_H_FLAG_HP_HIDE,
              flag: WGL_TTX_VT_DM_PARAM_DBL_SCRN_H_FLAG_HP_DBL_H                                                             
 *  @li@c  ui1_vgrid_mp_extr_start          -Main panel (ttx rows 1 ~ 23) start position
 *  @li@c  ui1_vgrid_mp_extr_end           -Main panel (ttx rows 1 ~ 23) end position
 *  @li@c  ui1_ttx_row_mp_extr_start      -Tail row(ttx rows: 24 and/or 25) start position
 *  @li@c  ui1_ttx_row_mp_extr_end       -Tail row(ttx rows: 24 and/or 25) end position
 *  @li@c  ui1_vgrid_mp_tr_start            -Main side panel tail row (ttx rows: 24 and/or 25) area 
 *  @li@c  t_mp_tr_info                         -The tail row of main panel information
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_VT_DM_PARAM_DBL_SCRN_H_T
{
    /* flags */
    UINT32                              ui4_flag;
    
    /* vgrid number */
    UINT8                               ui1_num_vgrid;
    
    /* header panel (ttx row 0) parameters */
    /* flag: WGL_TTX_VT_DM_PARAM_DBL_SCRN_H_FLAG_HP_HIDE */
    /* flag: WGL_TTX_VT_DM_PARAM_DBL_SCRN_H_FLAG_HP_DBL_H */
    /* position: ui1_vgrid_hp_start */
    UINT8                               ui1_vgrid_hp_start;
    
    /* main panel excluding tail row (ttx rows 1 ~ 23) area */
    UINT8                               ui1_vgrid_mp_extr_start;
    UINT8                               ui1_vgrid_mp_extr_end;
    UINT8                               ui1_ttx_row_mp_extr_start;
    UINT8                               ui1_ttx_row_mp_extr_end;
    
    /* main side panel tail row (ttx rows: 24 and/or 25) area    */
    /* flag:     WGL_TTX_VT_DM_PARAM_DBL_SCRN_H_FLAG_MP_TR_SHOW  */
    /* flag:     WGL_TTX_VT_DM_PARAM_DBL_SCRN_H_FLAG_MP_TR_DBL_H */
    /* position: ui1_vgrid_mp_tr_start                           */
    /* info:     t_mp_tr_info                                    */
    UINT8                               ui1_vgrid_mp_tr_start;
    WGL_TTX_MP_TR_INFO_T                t_mp_tr_info;

} WGL_TTX_VT_DM_PARAM_DBL_SCRN_H_T;


/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_VT_DISP_MODE_PARAM_T
 *  @brief This struct contains the vertical display mode information.
 *  @code
*   typedef union _WGL_TTX_VT_DISP_MODE_PARAM_T
*   {
*      WGL_TTX_VT_DM_PARAM_SCRN_H_T        t_scrn_h;    
*      WGL_TTX_VT_DM_PARAM_DBL_SCRN_H_T   t_dbl_upr;
*      WGL_TTX_VT_DM_PARAM_DBL_SCRN_H_T   t_dbl_lwr;
*      WGL_TTX_VT_DM_PARAM_DBL_SCRN_H_T   t_dbl_scrled;
*   } WGL_TTX_VT_DISP_MODE_PARAM_T;    
 *  @endcode
 *  @li@c t_scrn_h      -Contains screen height mode information if it is WGL_TTX_VT_DM_SCRN_HEIGHT mode.
 *  @li@c t_dbl_upr    -Contains upper of double screen width information if it is WGL_TTX_VT_DM_DBL_UPPER mode.
 *  @li@c t_dbl_lwr  -Contains lower of double screen width information if it is WGL_TTX_VT_DM_DBL_LOWER mode.
 *  @li@c t_dbl_scrled      -Contains scrolled of double screen information if it is WGL_TTX_VT_DM_DBL_SCRLED mode.
 */
/*------------------------------------------------------------------*/
typedef union _WGL_TTX_VT_DISP_MODE_PARAM_T
{
    /* WGL_TTX_VT_DM_SCRN_HEIGHT */
    WGL_TTX_VT_DM_PARAM_SCRN_H_T        t_scrn_h;   
    
    /* WGL_TTX_VT_DM_DBL_UPPER */
    WGL_TTX_VT_DM_PARAM_DBL_SCRN_H_T    t_dbl_upr;
    
    /* WGL_TTX_VT_DM_DBL_LOWER */
    WGL_TTX_VT_DM_PARAM_DBL_SCRN_H_T    t_dbl_lwr;
    
    /* WGL_TTX_VT_DM_DBL_SCRLED */
    WGL_TTX_VT_DM_PARAM_DBL_SCRN_H_T    t_dbl_scrled;
    
} WGL_TTX_VT_DISP_MODE_PARAM_T; 


/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_VT_DISP_INFO_T
 *  @brief This struct defines the vertical display information
 *  @code
 *  typedef struct _WGL_TTX_VT_DISP_INFO_T
 *  {
 *      WGL_TTX_VT_DISP_MODE_T              e_disp_mode;
 *      WGL_TTX_VT_DISP_MODE_PARAM_T        u_param;
 *  } WGL_TTX_VT_DISP_INFO_T;
 *  @endcode
 *  @li@c  e_disp_mode                  -The vertical display mode
 *  @li@c  u_param                        -The display mode parameters
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_VT_DISP_INFO_T
{
    /* vertical display mode */
    WGL_TTX_VT_DISP_MODE_T              e_disp_mode;
    
    /* display mode parameters */
    WGL_TTX_VT_DISP_MODE_PARAM_T        u_param;
    
} WGL_TTX_VT_DISP_INFO_T;


/* ui1_flag : WGL_TTX_DISP_INFO_T */
/**Flag is that display information ingore the side panel*/
#define WGL_TTX_DISP_INFO_FLAG_IGNORE_SIDE_PNL        MAKE_BIT_MASK_32(0)    

/**Flag is that horizontal information of display information is availible*/
#define WGL_TTX_DISP_INFO_FLAG_HZ_INFO_AVAIL          MAKE_BIT_MASK_32(1)    

/**Flag is that vertical information of display information is availible*/
#define WGL_TTX_DISP_INFO_FLAG_VT_INFO_AVAIL          MAKE_BIT_MASK_32(2)    

/* WGL_TTX_DISP_INFO_T */
/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_DISP_INFO_T
 *  @brief This struct contains the teletext widget display information,including horizontal 
 display information,vertical display information and screen margin information.
 *  @code
 *  typedef struct _WGL_TTX_DISP_INFO_T
 *  {
 *      UINT32                              ui4_flag;
 *      WGL_TTX_HZ_DISP_INFO_T              t_hz_disp;
 *      WGL_TTX_VT_DISP_INFO_T              t_vt_disp;
 *      WGL_TTX_MARGIN_T                    t_margin_scrn;
 *      WGL_TTX_VP_CFG_T                    t_vp_cfg;
 *  } WGL_TTX_DISP_INFO_T;
 *  @endcode
 *  @li@c  ui4_flag                 -Flags
 *  @li@c  t_hz_disp               -The horizontal display info
 *  @li@c  t_vt_disp                -The vertical display info
 *  @li@c  t_margin_scrn         -Define screen margin
 *  @li@c  t_vp_cfg                -The viewport config information
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_DISP_INFO_T
{
    /* flags */
    UINT32                              ui4_flag;

    /* horizontal display info */
    WGL_TTX_HZ_DISP_INFO_T              t_hz_disp;
    
    /* vertical display info */
    WGL_TTX_VT_DISP_INFO_T              t_vt_disp;

    /* screen margin */
    WGL_TTX_MARGIN_T                    t_margin_scrn;
    
    /* viewport config */
    WGL_TTX_VP_CFG_T                    t_vp_cfg;
    
} WGL_TTX_DISP_INFO_T;
 

/* ui4_flag : WGL_TTX_MIX_MODE_T */ 
 /**Flag is that mix ttx foreground for mix mode */
#define WGL_TTX_MIX_MODE_FLAG_MIX_TTX_FG            MAKE_BIT_MASK_32(0)   

 /**Flag is that mix ttx background for mix mode */
#define WGL_TTX_MIX_MODE_FLAG_MIX_TTX_BK            MAKE_BIT_MASK_32(1)   

 /**Flag is that mix ttx foreground and background for mix mode */
#define WGL_TTX_MIX_MODE_FLAG_MIX_ALL             ( WGL_TTX_MIX_MODE_FLAG_MIX_TTX_FG | \
                                                    WGL_TTX_MIX_MODE_FLAG_MIX_TTX_BK )    
                                                    
 /**Flag is that set default screen color for mix mode */
#define WGL_TTX_MIX_MODE_FLAG_DFT_SCRN_CLR_AVAIL    MAKE_BIT_MASK_32(2)    

 /**Flag is that set default row color for mix mode */
#define WGL_TTX_MIX_MODE_FLAG_DFT_ROW_CLR_AVAIL     MAKE_BIT_MASK_32(3)    

 /**Flag is that last flag for mix mode */
#define WGL_TTX_MIX_MODE_LAST_FLAG                  MAKE_BIT_MASK_32(3)    
                                                    
/* WGL_TTX_MIX_MODE_T */ 
/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_MIX_MODE_T
 *  @brief This struct defines the mix mode information
 *  @code
 *  typedef struct _WGL_TTX_MIX_MODE_T
 *  {
 *      UINT32                                  ui4_flag;
 *      UINT8                                    ui1_alpha;
 *      GL_COLOR_T                          t_clr_dft_ttx_scrn;
 *      GL_COLOR_T                          t_clr_dft_ttx_row;
 *  } WGL_TTX_MIX_MODE_T;
 *  @endcode
 *  @li@c  ui4_flag                          - Flags
 *  @li@c  ui1_alpha                        -The alpha transparency,valid ONLY when 
                        ((pt_mix_mode->ui4_flag & WGL_TTX_MIX_MODE_FLAG_MIX_ALL) != 0)
 *  @li@c  t_clr_dft_ttx_scrn             - Default full screen color ,valid ONLY when 
                        ((pt_mix_mode->ui4_flag & WGL_TTX_MIX_MODE_FLAG_DFT_SCRN_CLR_AVAIL) != 0)
 *  @li@c  t_clr_dft_ttx_row              -Default full row color,valid ONLY when 
                        ((pt_mix_mode->ui4_flag & WGL_TTX_MIX_MODE_FLAG_DFT_ROW_CLR_AVAIL) != 0)
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_MIX_MODE_T
{
    /* flags */
    UINT32                              ui4_flag;
    
    /* alpha transparency */
    /* valid ONLY when ((pt_mix_mode->ui4_flag & WGL_TTX_MIX_MODE_FLAG_MIX_ALL) != 0) */
    UINT8                               ui1_alpha;
    
    /* default full screen color */
    /* valid ONLY when ((pt_mix_mode->ui4_flag & WGL_TTX_MIX_MODE_FLAG_DFT_SCRN_CLR_AVAIL) != 0) */
    GL_COLOR_T                          t_clr_dft_ttx_scrn;
    
    /* default full row color */
    /* valid ONLY when ((pt_mix_mode->ui4_flag & WGL_TTX_MIX_MODE_FLAG_DFT_ROW_CLR_AVAIL) != 0) */
    GL_COLOR_T                          t_clr_dft_ttx_row;

} WGL_TTX_MIX_MODE_T;

#define WGL_TTX_REGION_ROW_MAX_NUM      2    /**<Define the max number of ttx regions */

/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_REGION_ROW_INF_T
 *  @brief This struct defines the ttx region row information
 *  @code
 *  typedef struct _WGL_TTX_REGION_ROW_INF_T
 *  {
 *      UINT8                               ui1_start_row;
 *      UINT8                               ui1_end_row;
 *  } WGL_TTX_REGION_ROW_INF_T;
 *  @endcode
 *  @li@c  ui1_start_row                  -Start row number
 *  @li@c  ui1_end_row                   -End row number 
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_REGION_ROW_INF_T
{
    UINT8                               ui1_start_row;

    UINT8                               ui1_end_row;
} WGL_TTX_REGION_ROW_INF_T;

/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_TRANSP_INF_T
 *  @brief This struct define the transparent region information
 *  @code
 *  typedef struct _WGL_TTX_TRANSP_INF_T
 *  {
 *      UINT8                               ui1_wgl_id; 
 *      BOOL                                b_enable; 
 *      BOOL                                b_by_region;
 *      UINT8                               ui1_row_rgn_cnt;
 *      WGL_TTX_REGION_ROW_INF_T            t_row_rng_inf[WGL_TTX_REGION_ROW_MAX_NUM];
 *      GL_RECT_T                           t_region;
 *  } WGL_TTX_TRANSP_INF_T;
 *  @endcode
 *  @li@c  ui1_wgl_id                                        -The widget id
 *  @li@c  b_enable                                          -1:set transpatent enable; 0:disable transparent
 *  @li@c  b_by_region                                      -Transparent region is region ,or by ttx row
                            1:set transpatent region by t_region
                            0: transparent by row information t_row_rgn_inf .
 *  @li@c  ui1_row_rgn_cnt                                -set region count by row,if b_by_region=0
 *  @li@c  t_row_rng_inf[WGL_TTX_REGION_ROW_MAX_NUM]         -Region information if b_by_region=0
 *  @li@c  t_region                             -the transparent region if b_by_region=1
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_TRANSP_INF_T
{
    UINT8                               ui1_wgl_id; /* transparent ttx wgl id */

    BOOL                                b_enable; /* 1:set transpatent enable; 0:disable transparent */

    /*transparent region is region ,or by ttx row*/
    BOOL                                b_by_region;/* 1:set transpatent region by t_region; 
                                                                                0: transparent by row information t_row_rgn_inf */

    UINT8                               ui1_row_rgn_cnt;/*set region count by row, t_row_rgn_inf */

    WGL_TTX_REGION_ROW_INF_T            t_row_rng_inf[WGL_TTX_REGION_ROW_MAX_NUM];

    /*the transparent region*/
    GL_RECT_T                           t_region;

} WGL_TTX_TRANSP_INF_T;


/* WGL_TTX_VP_MARGIN_CLR_METHOD_T */
/*------------------------------------------------------------------*/
/*! @enum WGL_TTX_VP_MARGIN_CLR_METHOD_T
 *  @brief This struct define ttx viewpoint margin color method
 *  @code
 *  typedef enum _WGL_TTX_VP_MARGIN_CLR_METHOD_T
 *  {
 *      WGL_TTX_VP_MARGIN_CLR_METHOD_AUTO,
 *      WGL_TTX_VP_MARGIN_CLR_METHOD_TRANSP,
 *      WGL_TTX_VP_MARGIN_CLR_METHOD_END
 *  } WGL_TTX_VP_MARGIN_CLR_METHOD_T;
 *  @endcode
 *  @li@c  WGL_TTX_VP_MARGIN_CLR_METHOD_AUTO              -Using auto color method
 *  @li@c  WGL_TTX_VP_MARGIN_CLR_METHOD_TRANSP           -Using transparent method
 *  @li@c  WGL_TTX_VP_MARGIN_CLR_METHOD_END                -Method end
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_TTX_VP_MARGIN_CLR_METHOD_T
{
    WGL_TTX_VP_MARGIN_CLR_METHOD_AUTO,
    WGL_TTX_VP_MARGIN_CLR_METHOD_TRANSP,
    WGL_TTX_VP_MARGIN_CLR_METHOD_END
    
} WGL_TTX_VP_MARGIN_CLR_METHOD_T;


/* ui4_flag : WGL_TTX_CUST_INIT_T */
/**Flag is that auto hide app panels and app masks for subtitle or news pages*/
#define WGL_TTX_CUST_INIT_FLAG_AUTO_HIDE_APP_LAYER_IN_BOX_MODE      MAKE_BIT_MASK_32(0)    

/**Flag is that using auto screen and auto row color mthod*/
#define WGL_TTX_CUST_INIT_FLAG_AUTO_SCRN_ROW_COLOR                  MAKE_BIT_MASK_32(1)    

/* WGL_TTX_CUST_INIT_T */
/*------------------------------------------------------------------*/
/*! @struct WGL_TTX_CUST_INIT_T
 *  @brief This struct defines the ttx initial information
 *  @code
 *  typedef struct _WGL_TTX_CUST_INIT_T
 *  {
 *      UINT32                                  ui4_flag;
 *      WGL_TTX_DISP_INFO_T           t_disp_info;
 *      GL_COLOR_T                          t_clr_dft_ttx_scrn;
 *      GL_COLOR_T                          t_clr_dft_ttx_row;
 *      WGL_TTX_VP_MARGIN_CLR_METHOD_T      e_clr_method_vp_margin;
 *  } WGL_TTX_CUST_INIT_T;
 *  @endcode
 *  @li@c  ui4_flag                               -Flags
 *  @li@c  t_disp_info                           -Contains the display infromation
 *  @li@c  t_clr_dft_ttx_scrn                  -Default sreen color
 *  @li@c  t_clr_dft_ttx_row                   -Default row color
 *  @li@c  e_clr_method_vp_margin        -Viewpoint margin color
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_TTX_CUST_INIT_T
{
    /* flags */
    UINT32                              ui4_flag;
    
    /* display info */
    WGL_TTX_DISP_INFO_T                 t_disp_info;
    
    /* default color for teletext Full Screen Color when no page */
    GL_COLOR_T                          t_clr_dft_ttx_scrn;
    
    /* default color for teletext Full Row color when no page */
    GL_COLOR_T                          t_clr_dft_ttx_row;
    
    /* vp margin color */
    WGL_TTX_VP_MARGIN_CLR_METHOD_T      e_clr_method_vp_margin;
    
} WGL_TTX_CUST_INIT_T;


/**The area except of box does not show while subtitle or newflash page */
#define WGL_TTX_BOX_SHOW_TYPE_NORMAL         ((UINT32) 0x0001)

/**The header panel that is except of box will show while subtitle or newflash page */
#define WGL_TTX_BOX_SHOW_TYPE_HP             ((UINT32) 0x0002)

/**The main panel that is except of box will show while subtitle or newflash page */
#define WGL_TTX_BOX_SHOW_TYPE_MP             ((UINT32) 0x0004)

/**The main trail panel that is except of box will show while subtitle or newflash page */
#define WGL_TTX_BOX_SHOW_TYPE_MP_TR          ((UINT32) 0x0008)

/**The app panel that is except of box will show while subtitle or newflash page */
#define WGL_TTX_BOX_SHOW_TYPE_APP            ((UINT32) 0x0010)

/**The trail of main panle to hide flof data flag or not */
#define WGL_TTX_MPTR_ATTR_HIDE_FLOF          ((UINT32) 0x0020)

#ifdef __cplusplus
}
#endif


#endif


/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_WGL_TELETEXT*/
/*----------------------------------------------------------------------------*/


