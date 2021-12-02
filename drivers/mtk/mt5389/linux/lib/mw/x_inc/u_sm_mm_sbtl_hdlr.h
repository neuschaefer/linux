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
/*! @file u_sm_mm_sbtl_hdlr.h 
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
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_STRM_MNGR_MM_SBTL_HDLR Mutilmedia Stream Subtitle Handle 
 *
 *  @ingroup groupMW_STRM_MNGR
 *  @brief Provides mutilmedia subtitle stream handler specific definitions that are for exported 
 to applications.
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW_STRM_MNGR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_SM_MM_SBTL_HDLR_H_
#define _U_SM_MM_SBTL_HDLR_H_


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

/**This is to set multimedia subtitle video handle.The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_SET_VS_HANDLE

h_obj       -Contains the handle of the video stream

Returns:

SMR_OK      -The command is successful.
*/
#define SM_MM_SBTL_CMD_CODE_SET_VS_HANDLE                           \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 0))    

/**This is to set the osd canvas handle for multimedia subtitle.The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_SET_OSD_HANDLE

h_obj       -Contains the handle of the OSD canvas 

Returns:

SMR_OK      -The command is successful.
*/
#define SM_MM_SBTL_CMD_CODE_SET_OSD_HANDLE                          \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 1))   
      
/**This is to set the video resolution information for multimedia subtitle.The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_VIDEO_RESOLUTION

pv_data       -Point to GL_SIZE_T data that contains the the video resolution.

Returns:

SMR_OK      -The command is successful.

SMR_INV_ARG     -The input parameter pv_data is null.
*/
#define SM_MM_SBTL_CMD_CODE_VIDEO_RESOLUTION                        \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 2))    

/**This is to set the display rectangle information for multimedia subtitle.The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_SET_DISPLAY_RECT

pv_data       -Point to GL_SIZE_T data that contains the display rectangle.

Returns:

SMR_OK      -The command is successful.

SMR_INV_ARG     -The input parameter pv_data is null.
*/
#define SM_MM_SBTL_CMD_CODE_SET_DISPLAY_RECT                        \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 3))    

/**This is to set the video rectangle information for multimedia subtitle.The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_SET_VIDEO_RECT

pv_data       -Point to GL_SIZE_T data that  contains the video rectangle.

Returns:

SMR_OK      -The command is successful.

SMR_INV_ARG     -The input parameter pv_data is null.
*/
#define SM_MM_SBTL_CMD_CODE_SET_VIDEO_RECT                          \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 4))    

/**This is to set the overscan rectangle information for multimedia subtitle.The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_SET_VIDEO_OVERSCAN

ui4_data       -Contains video overscan info left|top|right|bottom information by using UINT32 type.

Returns:

SMR_OK      -The command is successful.
*/
#define SM_MM_SBTL_CMD_CODE_SET_VIDEO_OVERSCAN                      \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 5))    

/**This is to set the multimedia subtitle type.The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_SET_SUBTITLE_TYPE

ui4_data       -contains the multimedia subtitle type,by using MM_SBTL_TYPE_T.

Returns:

SMR_OK      -The command is successful.
*/
#define SM_MM_SBTL_CMD_CODE_SET_SUBTITLE_TYPE                       \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 6))    

/**This is to set the multimedia subtitle display osd rectangle.The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_SET_OSD_RECT

pv_data       -Point to GL_SIZE_T data that contains the osd rectangle information.

Returns:

SMR_OK      -The command is successful.

SMR_INV_ARG     -The input parameter pv_data is null.
*/
#define SM_MM_SBTL_CMD_CODE_SET_OSD_RECT                            \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 7))    

/**This is to set the osd insect rectangle information for multimedia subtitle.The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_SET_OSD_INSECT

pv_data       -Point to WGL_INSET_T data that contains the subtitle osd insect rectangle .

Returns:

SMR_OK      -The command is successful.

SMR_INV_ARG     -The input parameter pv_data is null.
*/
#define SM_MM_SBTL_CMD_CODE_SET_OSD_INSECT                          \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 8))    

/**This is to set the multimedia subtitle osd offset position.If not do this command,it will using 
the default offset(0,0).The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_SET_OSD_OFFSET

pv_data       -Point to GL_POINT_T data that contains osd offset position.

Returns:

SMR_OK      -The command is successful.

SMR_INV_ARG     -The input parameter pv_data is null.
*/
#define SM_MM_SBTL_CMD_CODE_SET_OSD_OFFSET                          \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 9))    

/**This is to set the multimedia subtitle frontground color.If not do this command,subtitle will 
using default frontground color. The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_SET_OSD_FG_COLOR

pv_data       -Point to WGL_COLOR_INFO_T data that contains frontground color information.

Returns:

SMR_OK      -The command is successful.

SMR_INV_ARG     -The input parameter pv_data is null.
*/
#define SM_MM_SBTL_CMD_CODE_SET_OSD_FG_COLOR                        \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 10))    

/**This is to set the multimedia subtitle background color.If not do this command,subtitle will 
using default background color.The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_SET_OSD_BG_COLOR

pv_data       -Point to WGL_COLOR_INFO_T data that contains contains the background color information.

Returns:

SMR_OK      -The command is successful.

SMR_INV_ARG     -The input parameter pv_data is null.
*/
#define SM_MM_SBTL_CMD_CODE_SET_OSD_BG_COLOR                        \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 11))    

/**This is to set the multimedia subtitle font information.If not do this command,subtitle will 
using default font informaton.The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_SET_OSD_FONT_INFO

pv_data       -Point to WGL_FONT_INFO_T data that contains the font information.

Returns:

SMR_OK      -The command is successful.

SMR_INV_ARG     -The input parameter pv_data is null.
*/
#define SM_MM_SBTL_CMD_CODE_SET_OSD_FONT_INFO                       \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 12))    
    
    
/**This is to set text alignment for multimedia subtitle.If not do this command ,subtitle will 
using default WGL_AS_CENTER_BOTTOM.The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_SET_OSD_TEXT_ALIGNMENT

ui1_data       -Contains the text alignment information by using UINT8 type.

Returns:

SMR_OK      -The command is successful.
*/
#define SM_MM_SBTL_CMD_CODE_SET_OSD_TEXT_ALIGNMENT                  \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 13))    
    
    
/**This is to set the divx menu item information for multimedia subtitle.The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_SET_DIVX_MENU_ITEM

ui2_data       -Contain the divx menu item information by using UINT16.

Returns:

SMR_OK      -The command is successful.
*/
#define SM_MM_SBTL_CMD_CODE_SET_DIVX_MENU_ITEM                      \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 14))    
    
    
/**This is to set the multimedia subtitle speed.The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_SET_SPEED

ui4_data       -Contains the subtitle speed by using UINT32 type.

Returns:

SMR_OK      -The command is successful.
*/
#define SM_MM_SBTL_CMD_CODE_SET_SPEED                               \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 15))    

/**This is to set the multimedia subtitle file handle.The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_SET_SUBTITLE_FILE_HANDLE

h_obj       -Contains  the subtitle file handle.

Returns:

SMR_OK      -The command is successful.
*/
#define SM_MM_SBTL_CMD_CODE_SET_SUBTITLE_FILE_HANDLE                \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 16))   

/**This is to set multimedia subtitle index file handle.The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_SET_INDEX_FILE_HANDLE

h_obj       -Contains the handle of the index file handle.

Returns:

SMR_OK      -The command is successful.
*/
#define SM_MM_SBTL_CMD_CODE_SET_INDEX_FILE_HANDLE                   \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 17))    

/**This is to set the multimedia subtitle video rate.The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_SET_VIDEO_RATE

ui4_data       -Contains the video rate.

Returns:

SMR_OK      -The command is successful.

SMR_INV_ARG     -The input parameter pv_data is null.
*/
#define SM_MM_SBTL_CMD_CODE_SET_VIDEO_RATE                          \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 18))    

/**This is to set the video scale information for multimedia subtitle. If not do this command,it 
will not scale.The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_SET_VIDEO_SCALE

ui4_data       -Contains the video scale information.

Returns:

SMR_OK      -The command is successful.
*/
#define SM_MM_SBTL_CMD_CODE_SET_VIDEO_SCALE                         \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 19))   

/**This is to set the divx HD mode for multimedia subtitle.If not do this command,it is decided by 
the stream information. The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_SET_DIVX_HD

Returns:

SMR_OK      -The command is successful.
*/
#define SM_MM_SBTL_CMD_CODE_SET_DIVX_HD                             \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 20))   

/**This is to set the multimedia subtitle stream id information. If not do this command,it will 
using the default. The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_SET_STREAM_ID

ui4_data       -Contain the stream id information.

Returns:

SMR_OK      -The command is successful.
*/
#define SM_MM_SBTL_CMD_CODE_SET_STREAM_ID                           \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 21))    

/**This is to set the subtitle prefered lanuage list information for multimedia subtitle. If not do 
this command, it will use default language.The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_PREFERED_LANG_LIST

ps_text       -Point to the CHAR* data that contains the prefered language list.

Returns:

SMR_OK      -The command is successful.

SMR_INV_ARG     -The input parameter ps_text is null.
*/
#define SM_MM_SBTL_CMD_CODE_PREFERED_LANG_LIST                      \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 22))    


/**This is to set the mkv private data information for multimedia subtitle.The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_CODE_PRIVATE

pv_data       -Point to SBTL_MKV_CODEC_PRIVATE_T data that contains mkv private data information.

Returns:

SMR_OK      -The command is successful.

SMR_INV_ARG     -The input parameter pv_data is null.
*/
#define SM_MM_SBTL_CMD_CODE_CODE_PRIVATE                      \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 23))    


/**This is to set the mkv content encoding list information for multimedia subtitle.If not do this 
command,there is no encoding for the mkv subtitle.The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_MKV_ENCODING_LIST

pv_data       -Point to SBTL_MKV_CONTENT_ENCODINGS_T data that contains mkv content encoding list 
information.

Returns:

SMR_OK      -The command is successful.

SMR_INV_ARG     -The input parameter pv_data is null.
*/
#define SM_MM_SBTL_CMD_CODE_MKV_ENCODING_LIST                      \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 24))    

/**This is to set the subtitle display information for multimedia subtitle.If not do this 
command,the subtitle display osd is using default that is by canvas region.The command is using SM_COMMAND_T struct.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_SET_SUBTITLE_DISPLAY_INFO

pv_data       -Point to SBTL_DISPLAY_INFO_CMD_T data that contains video region,subtitle display position 
    and size info,font size info.

Returns:

SMR_OK      -The command is successful.

SMR_INV_ARG     -The input parameter pv_data is null.
*/
#define SM_MM_SBTL_CMD_CODE_SET_SUBTITLE_DISPLAY_INFO                      \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 25))    


/**This is to set the display page information for multimedia subtitle..

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_SET_PAGE_INFO

pv_data       -Point to MM_SBTL_DISP_PAGE_T data that contains the display page information.

Returns:

SMR_OK      -The command is successful.

SMR_INV_ARG     -The input parameter pv_data is null.
*/
#define SM_MM_SBTL_CMD_CODE_SET_PAGE_INFO                      \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 26))


/**This is to set the time offset mode information for multimedia subtitle.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_ADJ_TM_OFST

pv_data     -Point to MM_SBTL_TM_OFST_INF data that contains time offset mode information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -Set command fail.
*/
#define SM_MM_SBTL_CMD_CODE_ADJ_TM_OFST                      \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 27))


/**This is to set the encoding type for multimedia subtitle.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_ADJ_TM_OFST

ui4_data     -contains encoding type.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -Set command fail.
*/
#define SM_MM_SBTL_CMD_CODE_ENCODING_TYPE                      \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 28))


/**This is to set the subtitle show/hide for multimedia subtitle.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_SET_SHOW_HIDE

b_boolean     -True: show;False:hide.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -Set command fail.
*/
#define SM_MM_SBTL_CMD_CODE_SET_SHOW_HIDE                      \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 29))


/**This is to set the language index for multimedia subtitle.

Input arguments:

e_code      -SM_MM_SBTL_CMD_CODE_SET_LANG_IDX

ui1_data     -language index

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -Set command fail.
*/
#define SM_MM_SBTL_CMD_CODE_SET_LANG_IDX                      \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 30))



      
/**This is to set the  multimedia subtitle pts callback function.

Input arguments:

e_code      -Set to SM_MM_SBTL_CMD_CODE_SET_PTS_CB

pv_data     -Point to the MM_SBTL_PTS_CB data.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -Set command fail.
*/
#define SM_MM_SBTL_CMD_CODE_SET_PTS_CB                      \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 31))

#define SM_MM_SBTL_CMD_CODE_SET_OSD_FONT_EDGE_COLOR         \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 32))



      
/* set/get type groups *******************************************************/
/**MM subtitle groups type */
#define SM_GRP_MMSBTLH                      (( SM_GRP_LAST_ENTRY + 23 ) << SM_CODE_BITS)    

/** set types *****************************************************************/
/**Ignore this set command.
*/
#define SM_MM_SBTL_SET_TYPE_IGNORE                  ((SM_SET_TYPE_T)(SM_GRP_MMSBTLH | 0))    

/**This is to set the video retangle information for multimedia subtitle.

Input arguments:

e_set_type      -Set to SM_MM_SBTL_SET_TYPE_VIDEO_RECT

pv_set_info     -Point to GL_RECT_T data that contain video rectagle.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -Set command fail.
*/
#define SM_MM_SBTL_SET_TYPE_VIDEO_RECT              ((SM_SET_TYPE_T)(SM_GRP_MMSBTLH | 1))    

/**This is to set the video overscan retangle information for multimedia subtitle.

Input arguments:

e_set_type      -Set to SM_MM_SBTL_SET_TYPE_VIDEO_OVERSCAN

pv_set_info     -Point to GL_RECT_T data that contain video overscan rectagle.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -Set command fail.
*/
#define SM_MM_SBTL_SET_TYPE_VIDEO_OVERSCAN          ((SM_SET_TYPE_T)(SM_GRP_MMSBTLH | 2))    

/**This is to set the osd retangle information for multimedia subtitle.

Input arguments:

e_set_type      -Set to SM_MM_SBTL_SET_TYPE_OSD_RECT

pv_set_info     -Point to GL_RECT_T data that contain osd rectagle.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -Set command fail.
*/
#define SM_MM_SBTL_SET_TYPE_OSD_RECT                ((SM_SET_TYPE_T)(SM_GRP_MMSBTLH | 3))    

/**This is to set the osd inset retangle information for multimedia subtitle.

Input arguments:

e_set_type      -Set to SM_MM_SBTL_SET_TYPE_OSD_INSECT

pv_set_info     -Point to WGL_INSET_T data that contains the subtitle osd insect rectangle .

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -Set command fail.
*/
#define SM_MM_SBTL_SET_TYPE_OSD_INSECT              ((SM_SET_TYPE_T)(SM_GRP_MMSBTLH | 4))    

/**This is to set the osd offset information for multimedia subtitle.

Input arguments:

e_set_type      -Set to SM_MM_SBTL_SET_TYPE_OSD_OFFSET

pv_set_info     -Point to GL_POINT_T data that contains osd offset position.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -Set command fail.
*/
#define SM_MM_SBTL_SET_TYPE_OSD_OFFSET              ((SM_SET_TYPE_T)(SM_GRP_MMSBTLH | 5))    

/**This is to set the osd fontground color information for multimedia subtitle.

Input arguments:

e_set_type      -Set to SM_MM_SBTL_SET_TYPE_OSD_FG_COLOR

pv_set_info     -Point to WGL_COLOR_INFO_T data that contains frontground color information.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -Set command fail.
*/
#define SM_MM_SBTL_SET_TYPE_OSD_FG_COLOR            ((SM_SET_TYPE_T)(SM_GRP_MMSBTLH | 6))    

/**This is to set the osd background information for multimedia subtitle.

Input arguments:

e_set_type      -Set to SM_MM_SBTL_SET_TYPE_OSD_BG_COLOR

pv_set_info     -Point to WGL_COLOR_INFO_T data that contains contains the background color information.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -Set command fail.
*/
#define SM_MM_SBTL_SET_TYPE_OSD_BG_COLOR            ((SM_SET_TYPE_T)(SM_GRP_MMSBTLH | 7))    

/**This is to set the multimedia subtitle osd font information.

Input arguments:

e_set_type      -Set to SM_MM_SBTL_SET_TYPE_OSD_FONT_INFO

pv_set_info     -Point to WGL_FONT_INFO_T data that contains the font information.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -Set command fail.
*/
#define SM_MM_SBTL_SET_TYPE_OSD_FONT_INFO           ((SM_SET_TYPE_T)(SM_GRP_MMSBTLH | 8))    

/**This is to set the text alignment attribute information for multimedia subtitle.

Input arguments:

e_set_type      -Set to SM_MM_SBTL_SET_TYPE_OSD_TEXT_ALIGNMENT

pv_set_info     -Point to the UINT8 data that contains the text alignment information.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -Set command fail.
*/
#define SM_MM_SBTL_SET_TYPE_OSD_TEXT_ALIGNMENT      ((SM_SET_TYPE_T)(SM_GRP_MMSBTLH | 9))    

/**This is to set the divx menu item information for multimedia divx subtitle.

Input arguments:

e_set_type      -Set to  SM_MM_SBTL_SET_TYPE_DIVX_MENU_ITEM

pv_set_info     -Point to UINT16 data that contain the divx menu item information.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -Set command fail.
*/
#define SM_MM_SBTL_SET_TYPE_DIVX_MENU_ITEM          ((SM_SET_TYPE_T)(SM_GRP_MMSBTLH | 10))   

/**This is to set the video speed information for multimedia subtitle.

Input arguments:

e_set_type      -Set to SM_MM_SBTL_SET_TYPE_SPEED

pv_set_info     -Point to UINT32 data that contains the subtitle speed.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -Set command fail.
*/
#define SM_MM_SBTL_SET_TYPE_SPEED                   ((SM_SET_TYPE_T)(SM_GRP_MMSBTLH | 11))   

/**This is to set the encoding type for multimedia subtitle.

Input arguments:

e_set_type      -Set to SM_MM_SBTL_SET_TYPE_ENCODING

pv_set_info     -Point to MM_SBTL_ENCODING_T data that contains encoding type.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -Set command fail.
*/
#define SM_MM_SBTL_SET_TYPE_ENCODING                ((SM_SET_TYPE_T)(SM_GRP_MMSBTLH | 12))   

/**This is to set the time offset mode information for multimedia subtitle.

Input arguments:

e_set_type      -Set to SM_MM_SBTL_SET_TYPE_ADJUST_TIME

pv_set_info     -Point to MM_SBTL_TM_OFST_INF data that contains time offset mode information.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -Set command fail.
*/
#define SM_MM_SBTL_SET_TYPE_ADJUST_TIME             ((SM_SET_TYPE_T)(SM_GRP_MMSBTLH | 13))   

/**This is to set the hightlight fontground color for multimedia subtitle.

Input arguments:

e_set_type      -Set to SM_MM_SBTL_SET_TYPE_HLT_FG_COLOR

pv_set_info     -Point to GL_COLOR_T data that contains color information.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -Set command fail.
*/
#define SM_MM_SBTL_SET_TYPE_HLT_FG_COLOR            ((SM_SET_TYPE_T)(SM_GRP_MMSBTLH | 14))   

/**This is to set the hightlight background color for multimedia subtitle.

Input arguments:

e_set_type      -Set to SM_MM_SBTL_SET_TYPE_HLT_BG_COLOR

pv_set_info     -Point to GL_COLOR_T data that contains color information.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -Set command fail.
*/
#define SM_MM_SBTL_SET_TYPE_HLT_BG_COLOR            ((SM_SET_TYPE_T)(SM_GRP_MMSBTLH | 15))   

/**This is to set the display page information for multimedia subtitle.

Input arguments:

e_set_type      -Set to SM_MM_SBTL_SET_TYPE_PAGE_INFO

pv_set_info     -Point to MM_SBTL_DISP_PAGE_T data that contains display page information.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -Set command fail.
*/
#define SM_MM_SBTL_SET_TYPE_PAGE_INFO               ((SM_SET_TYPE_T)(SM_GRP_MMSBTLH | 16))   

/**This is to set the display page line gap information for multimedia subtitle.

Input arguments:

e_set_type      -Set to SM_MM_SBTL_SET_TYPE_LINE_GAP

pv_set_info     -Point to INT32 data that contains line gap value.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -Set command fail.
*/
#define SM_MM_SBTL_SET_TYPE_LINE_GAP               ((SM_SET_TYPE_T)(SM_GRP_MMSBTLH | 17))   

/**This is to set the subtitle show/hide for multimedia subtitle.

Input arguments:

e_set_type      -Set to SM_MM_SBTL_SET_TYPE_SHOW_HIDE

pv_set_info     -Point to BOOL data,True: show;False:hide.

z_set_info_len  -The size of the BOOL information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -Set command fail.
*/

#define SM_MM_SBTL_SET_TYPE_SHOW_HIDE              ((SM_SET_TYPE_T)(SM_GRP_MMSBTLH | 18))   

#define SM_MM_SBTL_SET_TYPE_OSD_FONT_EDGE_COLOR    ((SM_SET_TYPE_T)(SM_GRP_MMSBTLH | 19))

#define SM_MM_SBTL_SET_TYPE_FONT_BG_COLOR          ((SM_SET_TYPE_T)(SM_GRP_MMSBTLH | 20))  

#define SM_MM_SBTL_SET_TYPE_SRC_HTTP_FLAG          ((SM_SET_TYPE_T)(SM_GRP_MMSBTLH | 21))  




/** get types *****************************************************************/
/**Ignore this get command.
*/
#define SM_MM_SBTL_GET_TYPE_IGNORE                  ((SM_GET_TYPE_T)(SM_GRP_MMSBTLH | 0))    

/**This is to get the video retangle information for multimedia subtitle.

Input arguments:

e_set_type      -get to SM_MM_SBTL_SET_TYPE_VIDEO_RECT

pv_set_info     -Point to GL_RECT_T data that contain video rectagle.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -get command fail.
*/
#define SM_MM_SBTL_GET_TYPE_VIDEO_RECT              ((SM_GET_TYPE_T)(SM_GRP_MMSBTLH | 1))    

/**This is to get the video overscan retangle information for multimedia subtitle.

Input arguments:

e_set_type      -get to SM_MM_SBTL_SET_TYPE_VIDEO_OVERSCAN

pv_set_info     -Point to GL_RECT_T data that contain video overscan rectagle.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -get command fail.
*/
#define SM_MM_SBTL_GET_TYPE_VIDEO_OVERSCAN          ((SM_GET_TYPE_T)(SM_GRP_MMSBTLH | 2))    

/**This is to get the osd retangle information for multimedia subtitle.

Input arguments:

e_set_type      -get to SM_MM_SBTL_SET_TYPE_OSD_RECT

pv_set_info     -Point to GL_RECT_T data that contain osd rectagle.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -get command fail.
*/
#define SM_MM_SBTL_GET_TYPE_OSD_RECT                ((SM_GET_TYPE_T)(SM_GRP_MMSBTLH | 3))    

/**This is to get the osd inset retangle information for multimedia subtitle.

Input arguments:

e_set_type      -get to SM_MM_SBTL_SET_TYPE_OSD_INSECT

pv_set_info     -Point to WGL_INSET_T data that contains the subtitle osd insect rectangle .

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -get command fail.
*/
#define SM_MM_SBTL_GET_TYPE_OSD_INSECT              ((SM_GET_TYPE_T)(SM_GRP_MMSBTLH | 4))    

/**This is to get the osd offset information for multimedia subtitle.

Input arguments:

e_set_type      -get to SM_MM_SBTL_SET_TYPE_OSD_OFFSET

pv_set_info     -Point to GL_POINT_T data that contains osd offset position.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -get command fail.
*/
#define SM_MM_SBTL_GET_TYPE_OSD_OFFSET              ((SM_GET_TYPE_T)(SM_GRP_MMSBTLH | 5))    

/**This is to get the osd fontground color information for multimedia subtitle.

Input arguments:

e_set_type      -get to SM_MM_SBTL_SET_TYPE_OSD_FG_COLOR

pv_set_info     -Point to WGL_COLOR_INFO_T data that contains frontground color information.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -get command fail.
*/
#define SM_MM_SBTL_GET_TYPE_OSD_FG_COLOR            ((SM_GET_TYPE_T)(SM_GRP_MMSBTLH | 6))    

/**This is to get the osd background information for multimedia subtitle.

Input arguments:

e_set_type      -get to SM_MM_SBTL_SET_TYPE_OSD_BG_COLOR

pv_set_info     -Point to WGL_COLOR_INFO_T data that contains contains the background color information.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -get command fail.
*/
#define SM_MM_SBTL_GET_TYPE_OSD_BG_COLOR            ((SM_GET_TYPE_T)(SM_GRP_MMSBTLH | 7))    

/**This is to get the multimedia subtitle osd font information.

Input arguments:

e_set_type      -get to SM_MM_SBTL_SET_TYPE_OSD_FONT_INFO

pv_set_info     -Point to WGL_FONT_INFO_T data that contains the font information.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -get command fail.
*/
#define SM_MM_SBTL_GET_TYPE_OSD_FONT_INFO           ((SM_GET_TYPE_T)(SM_GRP_MMSBTLH | 8))    

/**This is to get the text alignment attribute information for multimedia subtitle.

Input arguments:

e_set_type      -get to SM_MM_SBTL_SET_TYPE_OSD_TEXT_ALIGNMENT

pv_set_info     -Point to the UINT8 data that contains the text alignment information.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -get command fail.
*/
#define SM_MM_SBTL_GET_TYPE_OSD_TEXT_ALIGNMENT      ((SM_GET_TYPE_T)(SM_GRP_MMSBTLH | 9))    

/**This is to get the divx menu item information for multimedia divx subtitle.

Input arguments:

e_set_type      -get to  SM_MM_SBTL_SET_TYPE_DIVX_MENU_ITEM

pv_set_info     -Point to UINT16 data that contain the divx menu item information.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -get command fail.
*/
#define SM_MM_SBTL_GET_TYPE_DIVX_MENU_ITEM          ((SM_GET_TYPE_T)(SM_GRP_MMSBTLH | 10))   

/**This is to get the video speed information for multimedia subtitle.

Input arguments:

e_set_type      -Set to SM_MM_SBTL_SET_TYPE_SPEED

pv_set_info     -Point to UINT32 data that contains the subtitle speed.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -get command fail.
*/
#define SM_MM_SBTL_GET_TYPE_SPEED                   ((SM_GET_TYPE_T)(SM_GRP_MMSBTLH | 11))   

/**This is to get the encoding type for multimedia subtitle.

Input arguments:

e_set_type      -get to SM_MM_SBTL_SET_TYPE_ENCODING

pv_set_info     -Point to MM_SBTL_ENCODING_T data that contains encoding type.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -get command fail.
*/
#define SM_MM_SBTL_GET_TYPE_ENCODING                ((SM_GET_TYPE_T)(SM_GRP_MMSBTLH | 12))   

/**This is to get the time offset mode information for multimedia subtitle.

Input arguments:

e_set_type      -get to SM_MM_SBTL_SET_TYPE_ADJUST_TIME

pv_set_info     -Point to MM_SBTL_TM_OFST_INF data that contains time offset mode information.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -get command fail.
*/
#define SM_MM_SBTL_GET_TYPE_ADJUST_TIME             ((SM_GET_TYPE_T)(SM_GRP_MMSBTLH | 13))   

/**This is to get the hightlight fontground color for multimedia subtitle.

Input arguments:

e_set_type      -get to SM_MM_SBTL_SET_TYPE_HLT_FG_COLOR

pv_set_info     -Point to GL_COLOR_T data that contains color information.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -get command fail.
*/
#define SM_MM_SBTL_GET_TYPE_HLT_FG_COLOR            ((SM_GET_TYPE_T)(SM_GRP_MMSBTLH | 14))   

/**This is to get the hightlight background color for multimedia subtitle.

Input arguments:

e_set_type      -Get to SM_MM_SBTL_SET_TYPE_HLT_BG_COLOR

pv_set_info     -Point to GL_COLOR_T data that contains color information.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -get command fail.
*/
#define SM_MM_SBTL_GET_TYPE_HLT_BG_COLOR            ((SM_GET_TYPE_T)(SM_GRP_MMSBTLH | 15))   

/**This is to get the display page information for multimedia subtitle.

Input arguments:

e_set_type      -get to SM_MM_SBTL_SET_TYPE_PAGE_INFO

pv_set_info     -Point to MM_SBTL_DISP_PAGE_T data that contains display page information.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -get command fail.
*/
#define SM_MM_SBTL_GET_TYPE_PAGE_INFO               ((SM_GET_TYPE_T)(SM_GRP_MMSBTLH | 16))   

/**This is to get the display page line gap information for multimedia subtitle.

Input arguments:

e_set_type      -get to SM_MM_SBTL_SET_TYPE_LINE_GAP

pv_set_info     -Point to INT32 data that contains line gap value.

z_set_info_len  -The size of the pv_set_info information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -get command fail.
*/
#define SM_MM_SBTL_GET_TYPE_LINE_GAP               ((SM_GET_TYPE_T)(SM_GRP_MMSBTLH | 17))  

/**This is to get the subtitle show/hide for multimedia subtitle.

Input arguments:

e_set_type      -get to SM_MM_SBTL_SET_TYPE_SHOW_HIDE

pv_set_info     -Point to BOOL data,True: show;False:hide.

z_set_info_len  -The size of the BOOL information.

Returns:

MMSBTLR_OK      -The set command is successful.

MMSBTLR_INV_ARG     -get command fail.
*/

#define SM_MM_SBTL_GET_TYPE_SHOW_HIDE              ((SM_GET_TYPE_T)(SM_GRP_MMSBTLH | 18)) 


/**This is to get the subtitle type for multimedia subtitle.

Input arguments:

e_set_type      -get to SM_MM_SBTL_SET_TYPE_SHOW_HIDE

pv_set_info     -Point to BOOL data,True: show;False:hide.

z_set_info_len  -The size of the BOOL information.

Returns:

MMSBTLR_OK      -The get command is successful.

MMSBTLR_INV_ARG     -get command fail.
*/
#define SM_MM_SBTL_GET_TYPE_SUBTITLE_TYPE              ((SM_GET_TYPE_T)(SM_GRP_MMSBTLH | 19)) 
#define SM_MM_SBTL_GET_TYPE_OSD_FONT_EDGE_COLOR        ((SM_GET_TYPE_T)(SM_GRP_MMSBTLH | 20)) 
/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/

#endif /* _U_SM_MM_SBTL_HDLR_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_STRM_MNGR_MM_SBTL_HDLR*/
/*----------------------------------------------------------------------------*/



