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
/*! @file u_sm_sbtl_hdlr.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Subtitle Stream handler specific 
 *         definitions, which are exported to applications.
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_SM_SBTL_HDLR_H_
#define _U_SM_SBTL_HDLR_H_

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_STRM_MNGR_SBTL_HDLR DVB Subtitle Handler
  *  @ingroup groupMW_STRM_MNGR
  *  @brief Provides external function TV Application
  *  %MW Application API would be used by TV Application, lot of these APIs would
  *   be wrapped from Exported API
  *  @see groupMW_STRM_MNGR_SBTL_HDLR
  *  @{ 
  */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_sys_name.h"
#include "u_handle.h"
#include "u_sm.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/


/* command sets *************************************************************/
/* primary commands */
/* argument is h_obj contains the handle of the video stream */
#define SM_SBTL_CMD_CODE_SET_VS_HANDLE                        \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 0))    /**< set video handle information */

/* argument is h_obj contains the handle of the OSD driver component */
#define SM_SBTL_CMD_CODE_SET_OSD_HANDLE                       \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 1))    /**< set OSD canvas handle */
      
/* argument is h_obj contains the handle of the subtitle SCART canvas */
#define SM_SBTL_CMD_CODE_SET_SCART_CANVAS_HANDLE                       \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 2))    /**< set SCART canvas handle */

/* argument is b_boolean contains the flag that set the state invisble of main canvas when open subtitle stream */
#define SM_SBTL_CMD_CODE_SET_OPEN_MAIN_INVISIBLE_FLAG                       \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 3))    /**< set the invisible flag of main canvas when open subtitle stream */
      
/* set/get type groups *******************************************************/
#define SM_GRP_SBTLH                      (( SM_GRP_LAST_ENTRY + 12 ) << SM_CODE_BITS)    /**< get/set command group id */

/* set types *****************************************************************/
#define SM_SBTLH_SET_TYPE_IGNORE          ((SM_SET_TYPE_T)(SM_GRP_SBTLH | 0))    /**< setting type is IGNORE */

#define SM_SBTLH_SET_TYPE_VISIBLE         ((SM_SET_TYPE_T)(SM_GRP_SBTLH | 1))    /**< setting type is VISIBLE */
#define SM_SBTLH_SET_TYPE_INVISIBLE       ((SM_SET_TYPE_T)(SM_GRP_SBTLH | 2))    /**< setting type is INVISIBLE */
#define SM_SBTLH_SET_TYPE_SPEED           ((SM_SET_TYPE_T)(SM_GRP_SBTLH | 3))    /**< setting type is SPEED */

/* get types *****************************************************************/
#define SM_SBTLH_GET_TYPE_IGNORE          ((SM_GET_TYPE_T)(SM_GRP_SBTLH | 0))    /**< getting type is IGNORE */


/*------------------------------------------------------------------*/
/*! @struct SM_SBTLH_STREAM_INFO
 *  @brief  This structure contains the configuration data for APP playing subtitle stream
 *  @code
 *  typedef struct _SM_SBTLH_STREAM_INFO
 *  {
 *      HANDLE_T            h_scart_plane;
 *      BOOL                   b_open_main_invisible;
 *  }   SM_SBTLH_STREAM_INFO;
 *  @endcode
 *  @li@c  h_scart_plane                                     - Destination plane of bypass subtitle
 *  @li@c  b_open_main_invisible                         - Main plane's invisible flag when open subtitle
 */
/*------------------------------------------------------------------*/
typedef struct _SM_SBTLH_STREAM_INFO
{
    HANDLE_T            h_scart_plane;
    BOOL                b_open_main_invisible;
}   SM_SBTLH_STREAM_INFO;


/* Init configuration Structure  */
/*------------------------------------------------------------------*/
/*! @struct SM_SBTLH_CONFIG_T
 *  @brief  This structure contains the configuration data for subtitle stream handler
 *  @code
 *  typedef struct _SM_SBTLH_CONFIG_T
 *  {
 *      HANDLE_T            h_partition;            
 *      THREAD_DESCR_T      t_thrd_descr;           
 *      UINT8               ui1_max_service_cnt;    
 *      UINT8               ui1_color_depth;        
 *      UINT32              ui4_page_width;         
 *      UINT32              ui4_page_height;        
 *      UINT8               ui1_wgl_order;          
 *  } SM_SBTLH_CONFIG_T;
 *  @endcode
 *  @li@c  h_partition                                       - Partition of subtitle stream handler
 *  @li@c  t_thrd_descr                                      - Subtitle handler thread information
 *  @li@c  ui1_max_service_cnt                               - The maximum number of subtitle stream service
 *  @li@c  ui1_color_depth                                   - The maximum color depth of subtitle page
 *  @li@c  ui4_page_width                                    - The page width of subtitle. The spec value is 720, but it may vary by different country
 *  @li@c  ui4_page_height                                   - The page height of subtitle. The spec value is 567, but it may vary by different country
 *  @li@c  ui1_wgl_order                                     - Subtitle widget app order
 */
/*------------------------------------------------------------------*/
typedef struct _SM_SBTLH_CONFIG_T
{
    HANDLE_T            h_partition;            /* Partition of subtitle stream handler */
    THREAD_DESCR_T      t_thrd_descr;           /* Subtitle handler thread */
    UINT8               ui1_max_service_cnt;    /* Number of maximum concurrent subtitle service */
    UINT8               ui1_color_depth;        /* Subtitle decoder depth */   
    UINT32              ui4_page_width;         /* Width of default canvas */
    UINT32              ui4_page_height;        /* Height of default canvas */
    UINT8               ui1_wgl_order;          /* Subtitle widget app order */
} SM_SBTLH_CONFIG_T;

/*------------------------------------------------------------------*/
/*! @struct SM_SBTLH_RENDER_INFO_T
 *  @brief  This structure contains the render information for ROM render engine and text render engine.
 *  @code
 *  typedef struct _SM_SBTLH_RENDER_INFO_T
 *  {
 *      UINT16 ui2_obj_x_pos;
 *      UINT16 ui2_obj_y_pos;
 *      UINT8  ui1_obj_fg_color;        
 *      UINT8  ui1_obj_bg_color;        
 *      UINT8  ui1_rgn_depth;
 *      UINT16 ui2_rgn_width;
 *      UINT16 ui2_rgn_height;
 *      UINT8* pui1_rgn;
 *      UINT16 ui2_obj_width;       
 *      UINT16 ui2_obj_height;      
 *  } SM_SBTLH_RENDER_INFO_T;
 *  @endcode
 *  @li@c  ui2_obj_x_pos                                     - The horizontal position of the top left pixel of the object
 *  @li@c  ui2_obj_y_pos                                     - The vertical position of the top left pixel of the object
 *  @li@c  ui1_obj_fg_color                                  - The entry in the applied CLUT that has been selected as the foreground colour of the characters
 *  @li@c  ui1_obj_bg_color                                  - The entry in the applied CLUT that has been selected as the background colour of the characters
 *  @li@c  ui1_rgn_depth                                     - The pixel depth of the region. 0x01 means 2 bit; 0x02 means 4 bit; 0x03 means 8bit
 *  @li@c  ui2_rgn_width                                     - The horizontal length of the region
 *  @li@c  ui2_rgn_height                                    - The vertical length of the region
 *  @li@c  pui1_rgn                                          - Pointer points to the byte buffer of the region. It's a byte-aligned buffer for each horizontal line pixels
 *  @li@c  ui2_obj_width                                     - The horizontal length of the object. It's an output field
 *  @li@c  ui2_obj_height                                    - The vertical length of the object. It's an output field
 */
/*------------------------------------------------------------------*/
typedef struct _SM_SBTLH_RENDER_INFO_T
{
    UINT16 ui2_obj_x_pos;
    UINT16 ui2_obj_y_pos;
    UINT8  ui1_obj_fg_color;        /* Only used as a text object */
    UINT8  ui1_obj_bg_color;        /* Only used as a text object */
    UINT8  ui1_rgn_depth;
    UINT16 ui2_rgn_width;
    UINT16 ui2_rgn_height;
    UINT8* pui1_rgn;
    
    UINT16 ui2_obj_width;       /* Output */
    UINT16 ui2_obj_height;      /* Output */
} SM_SBTLH_RENDER_INFO_T;

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/

#endif /* _U_SM_SBTL_HDLR_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                /* end of groupMW_STRM_MNGR_SBTL_HDLR */
/*----------------------------------------------------------------------------*/


