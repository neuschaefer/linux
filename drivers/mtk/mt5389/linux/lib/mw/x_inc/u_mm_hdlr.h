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
/*! @file u_mm_hdlr.h
 *  $RCSfile: $
 *  $Revision: #8 $
 *  $Date: 2012/07/24 $
 *  $Author: jiangbo.wang $
 *
 *  @par Description:
 *
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_SVCTX_MM_HDLR Multi Media Handler
 *
 *  @ingroup groupMW_SVCTX
 *  @brief Provides external function TV Application
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW_SVCTX
 *  @{ 
 */
/*----------------------------------------------------------------------------*/
#ifndef _U_MM_HDLR_H_
#define _U_MM_HDLR_H_
/*-----------------------------------------------------------------------------
 *  include files
 *---------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_mm_common.h"
#include "u_handle.h"
#include "u_drm.h"
#include "u_divx_drm.h"
#include "u_drm_drv.h"
#include "u_minfo.h"
#if 1//MM_LYRIC_SUPPORT
#include "u_mm_sbtl_engine.h"
#endif

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#define HT_MEDIA_STORGE_ELM_TYPE                (HT_GROUP_MSVCTX+3)    /**<        */
#define HT_MEDIA_MEM_ELM_TYPE                   (HT_GROUP_MSVCTX+4)    /**<        */
#define HT_MEDIA_NETWORK_ELM_TYPE               (HT_GROUP_MSVCTX+5)    /**<        */

/* source type begin */
/*SRC_TYPE_BRDCST_SVL , SRC_TYPE_BRDCST_FREQ,   SRC_TYPE_AVC  SRC_TYPE_RECORD*/
#define SRC_TYPE_MEDIA_STORGE                   ((SRC_TYPE_T) 5) /*source is from file storge*/    /**<        */
#define SRC_TYPE_MEDIA_MEM_CLIP                 ((SRC_TYPE_T) 6) /*source is from memory clip*/    /**<        */
#define SRC_TYPE_MEDIA_NETWORK                  ((SRC_TYPE_T) 7) /*source is from network*/    /**<        */
#define SRC_TYPE_MEDIA_TIME_SHIFT               ((SRC_TYPE_T) 8) /*source is from time-shift*/    /**<        */
#define SRC_TYPE_MEDIA_PVR                      ((SRC_TYPE_T) 9) /*source is from PVR*/    /**<        */
/* source type end*/

#define MM_AB_REPEAT_SUPPORT                    1    /**<        */

typedef enum _MM_SVC_PROTOCOL_T
{
    PROTOCL_MODE_UNKNOWN,
    PROTOCL_MODE_PULL,
    PROTOCL_MODE_PUSH,
    PROTOCL_MODE_URI,
    PROTOCL_MODE_DLNA
} MM_SVC_PROTOCOL_T;

/******************** Fill SCDB Mode *******************************/
typedef UINT32 MM_SVC_FILL_SCDB_MODE;

#define MM_SVC_FILL_SCDB_UPDATE              ((MM_SVC_FILL_SCDB_MODE) 0)
#define MM_SVC_FILL_SCDB_APPEND              ((MM_SVC_FILL_SCDB_MODE) 1)

/******************** Command set definitions *******************************/
typedef UINT32 MM_SVC_CMD_CODE_T;    /**<        */

#define MM_SVC_CMD_CODE_END               ((MM_SVC_CMD_CODE_T) 0)    /**<        */
#define MM_SVC_CMD_CODE_SVL_NAME          ((MM_SVC_CMD_CODE_T) 1)    /**<        */
#define MM_SVC_CMD_CODE_SVL_ID            ((MM_SVC_CMD_CODE_T) 2)    /**<        */
#define MM_SVC_CMD_CODE_SRC_TYPE          ((MM_SVC_CMD_CODE_T) 3)    /**<        */
#define MM_SVC_CMD_CODE_MEDIA_PATH        ((MM_SVC_CMD_CODE_T) 4)    /**<        */
#define MM_SVC_CMD_CODE_SET_STRM_INFO     ((MM_SVC_CMD_CODE_T) 5)    /**<        */
#define MM_SVC_CMD_CODE_EXT_SBTL_PATH     ((MM_SVC_CMD_CODE_T) 6)       /*attached data is pv_data and it points to a MM_EXT_SBTL_INFO variable.*/    /**<        */
#ifdef MM_TIME_SHIFT_UNIT_TEST
#define MM_SVC_CMD_CODE_TIME_SHIFT_UNIT_TEST ((MM_SVC_CMD_CODE_T) 7)    /**<        */
#endif /* MM_TIME_SHIFT_UNIT_TEST */
#define MM_SVC_CMD_CODE_SET_BOOT_ANIM     ((MM_SVC_CMD_CODE_T) 8)    /**<        */
#define MM_SVC_CMD_CODE_EXT_LYRIC_PATH     ((MM_SVC_CMD_CODE_T) 9)
#define  MM_SVC_CMD_CODE_SET_PROTOCL             ((MM_SVC_CMD_CODE_T) 10)          /**<        */
#define  MM_SVC_CMD_CODE_SET_META_INFO           ((MM_SVC_CMD_CODE_T) 11)           /**<        */
#define  MM_SVC_CMD_CODE_SET_PULL_MODE_INFO      ((MM_SVC_CMD_CODE_T) 12)           /**<        */
#define  MM_SVC_CMD_CODE_SET_PUSH_MODE_INFO      ((MM_SVC_CMD_CODE_T) 13)           /**<        */
#define  MM_SVC_CMD_CODE_SET_SRC_SUB_TYPE        ((MM_SVC_CMD_CODE_T) 14)           /**<        */
#define  MM_SVC_CMD_CODE_SET_PLAY_MODE           ((MM_SVC_CMD_CODE_T) 15)           /**<        */
#define  MM_SVC_CMD_CODE_SET_DRM_INFO            ((MM_SVC_CMD_CODE_T) 16)           /**<        */

/*------------ DO NOT INSERT OTHER TYPE TO THE FOLLOWING URL--------------*/
#define  MM_SVC_CMD_CODE_SET_PROXY_URL           ((MM_SVC_CMD_CODE_T) 17)           /**<        */
#define  MM_SVC_CMD_CODE_SET_AGENT_URL           ((MM_SVC_CMD_CODE_T) 18)           /**<        */
#define  MM_SVC_CMD_CODE_SET_COOKIE_URL          ((MM_SVC_CMD_CODE_T) 19)           /**<        */
#define  MM_SVC_CMD_CODE_SET_FILL_SCDB_MODE      ((MM_SVC_CMD_CODE_T) 20)           /**<        */
#define  MM_SVC_CMD_CODE_SET_PVR_TOTAl_DUR      ((MM_SVC_CMD_CODE_T) 21)   /**<        */


#if 1/*MKV DivXPlus*/
#define MM_DIVX_PLUS_ID_TO_POS(_pl_id, _chap_id)    ((UINT64)((UINT64)(((UINT64)_pl_id) << 32) | (UINT64)_chap_id))
#define MM_DIVX_PLUS_POS_TO_PL_ID(_pos)             ((UINT32)(((UINT64)_pos) >> 32))
#define MM_DIVX_PLUS_POS_TO_CHAP_ID(_pos)           ((UINT32)(((UINT64)_pos) & 0xFFFFFFFF))
#endif



/*! @enum MM_PLAY_MODE_T
 *  @brief play mode
 *  @code
 *  typedef enum
 *  {
 *      MM_PLAY_MODE_PREV = 0,
 *      MM_PLAY_MODE_PLAY
 *  } MM_PLAY_MODE_T;
 *  @endcode
 *  @li@c  MM_PLAY_MODE_PREV                             - preview mode.
 *  @li@c  MM_PLAY_MODE_PLAY                              - play mode.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_PLAY_MODE_PLAY = 0,
    MM_PLAY_MODE_PREV
} MM_PLAY_MODE_T;

/*------------------------------------------------------------------*/
/*! @enum MM_EXT_SBTL_TYPE_T
 *  @brief External subtitle type
 *  @code
 *  typedef enum
 *  {
 *      MM_EXT_SBTL_TYPE_FULL = 0,
 *      MM_EXT_SBTL_TYPE_LST
 *  } MM_EXT_SBTL_TYPE_T;
 *  @endcode
 *  @li@c  MM_EXT_SBTL_TYPE_FULL                             - Full.
 *  @li@c  MM_EXT_SBTL_TYPE_LST                              - List.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_EXT_SBTL_TYPE_FULL = 0,              /*full path is valid.*/
    MM_EXT_SBTL_TYPE_LST                    /*extension list is valid.*/
} MM_EXT_SBTL_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum MM_LYRIC_TYPE_T
 *  @brief Lyric type
 *  @code
 *  typedef enum
 *  {
 *      MM_LYRIC_TYPE_LRC = 0,
 *  } MM_LYRIC_TYPE_T;
 *  @endcode
 *  @li@c  MM_LYRIC_TYPE_LRC                             - LRC format.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_LYRIC_TYPE_LRC = 0,
    MM_LYRIC_TYPE_LRC_FULL_PATH
} MM_LYRIC_TYPE_T;
/*------------------------------------------------------------------*/
/*! @struct MM_EXT_SBTL_INFO
 *  @brief Subtitle information.
 *  @code
 *  typedef struct
 *  {
 *      MM_EXT_SBTL_TYPE_T  e_type;
 *      union
 *      {
 *          CHAR*           ps_full;
 *          CHAR*           ps_ext_lst;
 *      } u;
 *      CHAR*   ps_prf_lang_lst;
 *  } MM_EXT_SBTL_INFO;
 *  @endcode
 *  @li@c  e_type                                            - Type.
 *  @li@c  ps_full                                           - Contains the pull path of the external subtitle
 *  @li@c  ps_ext_lst                                        - Contains the extension list and extensions are separated by ';'
 *  @li@c  ps_prf_lang_lst                                   - ISO 639 3 character code. EX: "eng;chi"
 */
/*------------------------------------------------------------------*/
typedef struct
{
    MM_EXT_SBTL_TYPE_T  e_type;
    union
    {
        CHAR*           ps_full;            /*A null character terminated string that contains the pull path of the external subtitle*/
        CHAR*           ps_ext_lst;         /*A null character terminated extension list and extensions are separated by ';'.
                                                 The order represents the extension priority. Ex: "srt;sub;ssa"*/
    } u;
    CHAR*   ps_prf_lang_lst;                  /*ISO 639 3 character code. EX: "eng;chi"*/
} MM_EXT_SBTL_INFO;

/*------------------------------------------------------------------*/
/*! @struct MM_LYRIC_INFO
 *  @brief Lyric information.
 *  @code
 *  typedef struct
 *  {
 *      MM_LYRIC_TYPE_T  e_type;
 *      union
 *      {
 *          CHAR*           ps_lyric_list;
 *      } u;
 *      CHAR*   ps_prf_lang_lst;
 *  } MM_LYRIC_INFO;
 *  @endcode
 *  @li@c  e_type                                               - Type.
 *  @li@c  ps_lyric_list                                        - Contains the extension list and extensions are separated by ';'
 *  @li@c  ps_prf_lang_lst                                   - ISO 639 3 character code. EX: "eng;chi"
 */
/*------------------------------------------------------------------*/
typedef struct
{
    MM_LYRIC_TYPE_T  e_type;
    union
    {
        CHAR*           ps_full;
        CHAR*           ps_lyric_list;         /*A null character terminated extension list and extensions are separated by ';'.
                                                 The order represents the extension priority. Ex: "txt;lrc;ssa"*/
    } u;
    CHAR*   ps_prf_lang_lst;                  /*ISO 639 3 character code. EX: "eng;chi"*/
} MM_LYRIC_INFO;
/*------------------------------------------------------------------*/
/*! @struct MM_SVC_COMMAND_T
 *  @brief Sevice command.
 *  @code
 *  typedef struct
 *  {
 *      MM_SVC_CMD_CODE_T  e_code;
 *      union
 *      {
 *          BOOL       b_boolean;
 *          UINT8      ui1_data;
 *          UINT16     ui2_data;
 *          UINT32     ui4_data;
 *          HANDLE_T   h_obj;
 *          CHAR*      ps_text;
 *          VOID*      pv_data;
 *      } u;
 *  } MM_SVC_COMMAND_T;
 *  @endcode
 *  @li@c  e_code                                            - Command type.
 *  @li@c  b_boolean                                         - True or false.
 *  @li@c  ui1_data                                          - Data1.
 *  @li@c  ui2_data                                          - Data2.
 *  @li@c  ui4_data                                          - Data3.
 *  @li@c  h_obj                                             - Object handle.
 *  @li@c  ps_text                                           - Text.
 *  @li@c  pv_data                                           - Data.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    MM_SVC_CMD_CODE_T  e_code;
    union
    {
        BOOL       b_boolean;
        UINT8      ui1_data;
        UINT16     ui2_data;
        UINT32     ui4_data;
        HANDLE_T   h_obj;
        CHAR*      ps_text;
        VOID*      pv_data;
    } u;
} MM_SVC_COMMAND_T;

#define SVCTX_GRP_MM                                  (4<<SVCTX_TYPE_CODE_BITS)    /**<        */
/****************** Multimedia handler set types *****************************/
#define SVCTX_MM_SET_TYPE_TRICK           ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 0))    /**<        */
#define SVCTX_MM_SET_TYPE_BTN             ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 1))    /**<        */
#define SVCTX_MM_SET_TYPE_REPEAT          ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 2))    /**<        */
#define SVCTX_MM_SET_TYPE_SEEK            ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 3))    /**<        */
#define SVCTX_MM_SET_TYPE_SEND_PUSH_BUF     ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 4))    /**<        */
#define SVCTX_MM_SET_TYPE_REL_PUSH_BUF      ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 5))    /**<        */
#define SVCTX_MM_SET_TYPE_SEND_IBC          ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 6))    /**<        */

//MM_LYRIC_SUPPORT
#define SVCTX_MM_SBTL_SET_TYPE_DISP_MODE  ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 7))    /**<        */
#define SVCTX_MM_SBTL_SET_TYPE_HILT_STL   ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 8))    /**<        */
#define SVCTX_MM_SBTL_SET_TYPE_TIME_OFST  ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 9))    /**<        */
#define SVCTX_MM_SBTL_SET_TYPE_FONT_ENC   ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 10))    /**<        */
#define SVCTX_MM_SBTL_SET_TYPE_HILT_ATTR  ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 11))    /**<        */
#define SVCTX_MM_SBTL_SET_TYPE_NRML_ATTR  ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 12))    /**<        */
#define SVCTX_MM_SBTL_SET_TYPE_SHOW_HIDE  ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 13))    /**<        */
#define SVCTX_MM_SBTL_SET_TYPE_DEFAULT    ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 14))    /**<        */

#define SVCTX_MM_SET_TYPE_PULL_INFO       ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 15))    /**<        */
#define SVCTX_MM_SET_TYPE_STRM_INFO       ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 16))    /**<        */
#define SVCTX_MM_SET_TYPE_STRM_CODEC_INFO ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 17))    /**<        */
#define SVCTX_MM_SET_TYPE_CHANGE_STRM     ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 18))    /**<        */
#define SVCTX_MM_SET_TYPE_AUTOTEST_INFO   ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 19))    /**<        */
#define SVCTX_MM_SET_TYPE_TICK_CTRL_BLK   ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 20))    /**<        */
#define SVCTX_MM_SET_TYPE_3D_INFO         ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 21))    /**<        */
#ifdef SUPPORT_SET_SBTL_ENCODING
#define SVCTX_MM_SET_SUBTITLE_ENCODING    ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 22))    /**<        */
#endif
#define SVCTX_MM_SET_TYPE_LANGUAGE        ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 23))    /**<        */
#define SVCTX_MM_SBTL_SET_FONT_BG_COLOR   ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 24))
#define SVCTX_MM_SET_CANCEL_URI_READ      ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 25))
#define SVCTX_MM_SET_TYPE_CRYPT_KEY       ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 26))    /**<        */
#define SVCTX_MM_SET_TYPE_PCR_PID         ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 27))    /**<        */


/* DRM set types *************************************************************/
#define SVCTX_DRM_SET_TYPE_PLAYBACK       ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 10000))    /**<        */
#define SVCTX_DRM_SET_TYPE_EXT_SUBTITLE_PREFER  ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 20000))    /**<        */

/* GENERIC set types *********************************************************/
#define SVCTX_GENGRIC_SET_TYPE_DIVX_DRM_DEACTIVATION      ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 30000))    /**<        */
#define SVCTX_GENGRIC_SET_TYPE_DIVX_DRM_MEMORY_RW_FUNC    ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 50000))    /**<        */

/* MEIDA Property set types *************************************************************/
#define SVCTX_MEDIA_SET_TYPE_EXT_SUBTITLE_PREFER  ((SVCTX_SET_TYPE_T)(SVCTX_GRP_MM | 40000))    /**<        */

/******************* Multimedia handler get types ****************************/
#define SVCTX_MM_GET_TYPE_TIME_CODE       ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 0))    /**<        */
#define SVCTX_MM_GET_TYPE_DUR             ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 1))    /**<        */
#define SVCTX_MM_GET_TYPE_TT_INFO         ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 2))    /**<        */
#define SVCTX_MM_GET_TYPE_CH_INFO         ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 3))    /**<        */
#define SVCTX_MM_GET_TYPE_POSIT_INFO      ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 4))    /**<        */
#define SVCTX_MM_GET_TYPE_MEDIA_FORMAT    ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 5))    /**<        */
#define SVCTX_MM_GET_TYPE_MENU_INFO       ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 6))    /**<        */
#define SVCTX_MM_GET_TYPE_UOP_CAP_INFO    ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 7))    /**<        */
#define SVCTX_MM_GET_TYPE_SPEED           ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 8))    /**<        */
#define SVCTX_MM_GET_TYPE_VIDEO_INFO      ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 9))    /**<        */
#define SVCTX_MM_GET_TYPE_META_DATA       ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 10))    /**<        */
#define SVCTX_MM_GET_TYPE_CURR_PROCESS_PROGRESS   ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 11))    /**<        */
#ifdef TIME_SHIFT_SUPPORT
#define SVCTX_MM_GET_TYPE_TIMESHIFT_RANGE_INFO ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 12))    /**<        */
#define SVCTX_MM_GET_BRDCST_TYPE          ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 13))    /**<        */
#endif
#if (MM_AB_REPEAT_SUPPORT || defined(MM_LAST_MEMORY_SUPPORT))
#define SVCTX_MM_GET_TYPE_POS_PTS       ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 14))    /**<        */
#endif
#define SVCTX_MM_GET_TYPE_VID_CODEC_INFO    ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 15))    /**<        */
#define SVCTX_MM_GET_TYPE_BUFFCTRL_INFO     ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 17))  /**<        */
#define SVCTX_MM_GET_TYPE_STRM_INFO         ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 18))  /**<        */
#define SVCTX_MM_GET_TYPE_GENERIC_INFO      ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 19))  /**<        */
#define SVCTX_MM_GET_TYPE_PUSH_BUFF           ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 20))  /**<        */
#if 1/*MKV DivXPlus*/
#define SVCTX_MM_GET_TYPE_TITLE_NUM         ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 21))
#define SVCTX_MM_GET_TYPE_TITLE_INFO        ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 22))
#define SVCTX_MM_GET_TYPE_PLAYLIST_INFO     ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 23))
#define SVCTX_MM_GET_TYPE_CHAP_INFO         ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 24))
#define SVCTX_MM_GET_TYPE_DISP_INFO         ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 25))
#define SVCTX_MM_GET_TYPE_CUR_CHAP_INFO     ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 26))
#define SVCTX_MM_GET_TYPE_CUR_POS           ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 27))
#endif
#if 1//MM_LYRIC_SUPPORT
#define SVCTX_MM_SBTL_GET_TYPE_LYRIC_INFO   ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 28))  /**<        */
#endif
#define SVCTX_MM_GET_TYPE_TIME_TO_SIZE      ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 29))
#define SVCTX_MM_GET_TYPE_PTS               ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 30))
#define SVCTX_MM_GET_VS_BIT_RATE            ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 31))
#define SVCTX_MM_GET_MEM_BAND_STATUS        ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 32))
#define SVCTX_MM_GET_PROGRAM_NUM            ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 33))
#if 1//def MM_LAST_MEMORY_SUPPORT 
#define SVCTX_MM_GET_MM_IDENTIFIER          ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 34))
#define SVCTX_MM_GET_STOP_INFO              ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 35))
#endif
#ifdef SUPPORT_SET_SBTL_ENCODING
#define SVCTX_MM_GET_SUBTITLE_TYPE          ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 36))
#endif

#define SVCTX_MM_GET_TYPE_RAW_DATA          ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 37))
#define SVCTX_MM_GET_TYPE_ID3_RAW_DATA      ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 38))

#define SVCTX_MM_GET_TYPE_TIME_CODE_MS      ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 39))    /**<        */
#define SVCTX_MM_GET_BUF_STATUS             ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 40))
#define SVCTX_MM_GET_THUMBNAIL              ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 41))
#define SVCTX_MM_GET_HTTP_DOWNLOAD_SPEED    ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 42))
#define SVCTX_MM_GET_TYPE_3D_VID_INFO       ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 43))
#define SVCTX_MM_GET_TYPE_AFIFO_AMOUNT      ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 44))
#define SVCTX_MM_GET_CURRENT_TICK           ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 45))

/* DRM get types *************************************************************/
#define SVCTX_DRM_GET_TYPE_INFO           ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 10000))    /**<        */
/* GENERIC get types *********************************************************/
#define SVCTX_GENGRIC_GET_TYPE_DIVX_DRM_REGISTRATION_CODE ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 20000))    /**<        */
#define SVCTX_GENGRIC_GET_TYPE_DIVX_DRM_UI_HELP_INFO      ((SVCTX_GET_TYPE_T)(SVCTX_GRP_MM | 20001))    /**<        */


/* This structure defines divx drm info for svctx. */
/*------------------------------------------------------------------*/
/*! @struct DIVX_DRM_INFO_SVCTX_T
 *  @brief Divx drm information
 *  @code
 *  typedef struct _DIVX_DRM_INFO_SVCTX_T
 *  {
 *      DIVX_DRM_BASIC_INFO_T* pt_divx_drm_basic_info;
 *      DRM_OUTPUT_SIGNAL_PROTECTION_INFO_T* pt_drm_output_signal_protection_info;
 *  } DIVX_DRM_INFO_SVCTX_T;
 *  @endcode
 *  @li@c  pt_divx_drm_basic_info                            - Basic information.
 *  @li@c  pt_drm_output_signal_protection_info              - Output signal protection infor.
 */
/*------------------------------------------------------------------*/
typedef struct _DIVX_DRM_INFO_SVCTX_T
{
    DIVX_DRM_BASIC_INFO_T* pt_divx_drm_basic_info;
    DRM_OUTPUT_SIGNAL_PROTECTION_INFO_T* pt_drm_output_signal_protection_info;
} DIVX_DRM_INFO_SVCTX_T;

/* This structure defines drm info for svctx. */
/*------------------------------------------------------------------*/
/*! @struct DRM_INFO_SVCTX_T
 *  @brief DRM information.
 *  @code
 *  typedef struct _DRM_INFO_SVCTX_T
 *  {
 *      DRM_TYPE_T t_drm_type;
 *      union _DRM_INFO_UNION_SVCTX_T
 *      {
 *          DIVX_DRM_INFO_SVCTX_T t_divx_drm_info_svctx;
 *      } u;
 *  } DRM_INFO_SVCTX_T;
 *  @endcode
 *  @li@c  t_drm_type                                        - Type.
 *  @li@c  t_divx_drm_info_svctx                             - DRM infor.
 */
/*------------------------------------------------------------------*/
typedef struct _DRM_INFO_SVCTX_T
{
    DRM_TYPE_T t_drm_type;
    UINT32     ui4_idx;
    union _DRM_INFO_UNION_SVCTX_T
    {
        DIVX_DRM_INFO_SVCTX_T t_divx_drm_info_svctx;
    } u;
} DRM_INFO_SVCTX_T;


/******************* Multimedia Search Type **********************************/
#if 1//MM_LYRIC_SUPPORT
typedef enum
{
    MM_SBTL_DISP_MODE_OFF = 0,
    MM_SBTL_DISP_MODE_SINGLE_LINE,
    MM_SBTL_DISP_MODE_MULTI_LINE
} MM_SBTL_DISP_MODE_T;

typedef struct _SVC_MM_SBTL_DISP_MODE_T
{
    MM_SBTL_DISP_MODE_T          e_disp_mode;
    UINT16                       ui2_param;
} MM_SVC_SBTL_DISP_MODE_T;

typedef enum
{
    MM_SBTL_HILT_STL_BY_LINE = 0,
    MM_SBTL_HILT_STL_KARAOKE
} MM_SBTL_HILT_STL_T;

typedef struct _SVC_MM_SBTL_HLT_STL_T
{
    MM_SBTL_HILT_STL_T       e_hlt_style;
    UINT16                   ui2_param;
} MM_SVC_SBTL_HILT_STL_T;

typedef enum
{
    MM_SBTL_TIME_OFST_OFF = 0,
    MM_SBTL_TIME_OFST_AUTO,
    MM_SBTL_TIME_OFST_USER_DEF
} MM_SBTL_TIME_OFST_T;

typedef struct _SVC_MM_SBTL_TIME_OFST_T
{
    MM_SBTL_TIME_OFST_T       e_time_ofst;
    UINT32                    ui4_ofst_value;
} MM_SVC_SBTL_TIME_OFST_T;

typedef enum
{
    MM_SBTL_FONT_ENC_AUTO = 0,
    MM_SBTL_FONT_ENC_UTF8,
    MM_SBTL_FONT_ENC_UTF16,
    MM_SBTL_FONT_ENC_BIG5,
    MM_SBTL_FONT_ENC_GB
} MM_SBTL_FONT_ENC_T;

typedef struct _SVC_MM_SBTL_FONT_ENC_T
{
    MM_SBTL_FONT_ENC_T           e_enc_type;
    UINT16                       ui2_param;
} MM_SVC_SBTL_FONT_ENC_T;

/* define a enum type to set border type of each sentence of lyric */
typedef enum
{
    MM_SBTL_BDR_TYPE_NULL = 0,
    MM_SBTL_BDR_TYPE_SOLID_LINE
} MM_SBTL_BDR_TYPE_T;

/* define a enum type to set roll type for multiline display mode */
typedef enum
{
    MM_SBTL_ROLL_TYPE_DEF = 0
} MM_SBTL_ROLL_TYPE_T;

/* set more than one attributes at a time with MM_SBTL_ATTR_T type */
typedef UINT16 MM_SBTL_ATTR_T;

#define MM_SBTL_ATTR_FNT_INFO             ((MM_SBTL_ATTR_T) (1 << 0))
#define MM_SBTL_ATTR_BKG_CLR              ((MM_SBTL_ATTR_T) (1 << 1))
#define MM_SBTL_ATTR_TXT_CLR              ((MM_SBTL_ATTR_T) (1 << 2))
#define MM_SBTL_ATTR_BDR_TYPE             ((MM_SBTL_ATTR_T) (1 << 3))
#define MM_SBTL_ATTR_BDR_WIDTH            ((MM_SBTL_ATTR_T) (1 << 4))
#define MM_SBTL_ATTR_ROLL_TYPE            ((MM_SBTL_ATTR_T) (1 << 5))
#define MM_SBTL_ATTR_DISP_RECT            ((MM_SBTL_ATTR_T) (1 << 6))
#define MM_SBTL_ATTR_OSD_OFFSET            ((MM_SBTL_ATTR_T) (1 << 7))
#define MM_SBTL_ATTR_EDG_CLR              ((MM_SBTL_ATTR_T) (1 << 8))

typedef struct _SVC_MM_SBTL_ATTR_T
{
    MM_SBTL_ATTR_T            t_attr_field;
    WGL_FONT_INFO_T           t_font_info;
    GL_COLOR_T                t_bkg_clr;
    GL_COLOR_T                t_txt_clr;
    GL_COLOR_T                t_edge_clr;
    MM_SBTL_BDR_TYPE_T        e_bdr_type;
    UINT16                    ui4_bdr_width;
    MM_SBTL_ROLL_TYPE_T       e_roll_type;
    GL_POINT_T                t_osd_offset;
    GL_RECT_T                 t_disp_rect;
} MM_SVC_SBTL_DISP_ATTR_T;

/* set subtitle to be show(TRUE) or hide(FALSE) */
typedef struct _SVC_MM_SBTL_SHOW_HIDE_T
{
    BOOL                      b_sbtl_show;
} MM_SVC_SBTL_SHOW_HIDE_T;

typedef struct _SVC_MM_SBTL_LYRIC_INFO_T
{
    WGL_FONT_INFO_T           t_font;
    MM_SBTL_DISP_PAGE_T       t_page;
    MM_SBTL_TM_OFST_INF       t_tm_ofst;
    WGL_COLOR_INFO_T          t_bg_clr;
    WGL_COLOR_INFO_T          t_fg_clr;
    WGL_COLOR_INFO_T          t_edg_clr;
    GL_RECT_T                 t_rect;
    GL_POINT_T                t_osd_offset;
    MM_SBTL_ENCODING_T        e_enc;
    BOOL                      b_hide;
    BOOL                      b_has_lyric;
    BOOL                      b_is_font_bg_clr_set;
    WGL_COLOR_INFO_T          t_font_bg_color;
}SVC_MM_SBTL_LYRIC_INFO_T;
#endif

/*------------------------------------------------------------------*/
/*! @enum MM_SVC_POS_TYPE_T
 *  @brief Service position type.
 *  @code
 *  typedef enum
 *  {
 *      MM_SVC_POS_TYPE_TT_TIME = 1,
 *      MM_SVC_POS_TYPE_CH_TIME,
 *      MM_SVC_POS_TYPE_ALL_TIME,
 *      MM_SVC_POS_TYPE_TT_OFFSET,
 *      MM_SVC_POS_TYPE_CH_OFFSET,
 *      MM_SVC_POS_TYPE_ALL_OFFSET,
 *      MM_SVC_POS_TYPE_PICTURE_IDX
 *  } MM_SVC_POS_TYPE_T;
 *  @endcode
 *  @li@c  MM_SVC_POS_TYPE_TT_TIME                           - Total time.
 *  @li@c  MM_SVC_POS_TYPE_CH_TIME                           - Channel time.
 *  @li@c  MM_SVC_POS_TYPE_ALL_TIME                          - All offset.
 *  @li@c  MM_SVC_POS_TYPE_TT_OFFSET                         - Total offset.
 *  @li@c  MM_SVC_POS_TYPE_CH_OFFSET                         - Channel offset.
 *  @li@c  MM_SVC_POS_TYPE_ALL_OFFSET                        - All offset.
 *  @li@c  MM_SVC_POS_TYPE_PICTURE_IDX                       - Picture index.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_SVC_POS_TYPE_TT_TIME = 1,
    MM_SVC_POS_TYPE_CH_TIME,
    MM_SVC_POS_TYPE_ALL_TIME, /*file or channel*/
    MM_SVC_POS_TYPE_TT_OFFSET,
    MM_SVC_POS_TYPE_CH_OFFSET,
    MM_SVC_POS_TYPE_ALL_OFFSET,/*file or channel*/
    MM_SVC_POS_TYPE_PICTURE_IDX
#if 1/*MKV DivXPlus*/
    ,MM_SVC_POS_TYPE_CHAPTER
#endif
    ,MM_SVC_POS_TYPE_RANGE_AB_START
} MM_SVC_POS_TYPE_T;

/******************** Multimedia Repeat Type *********************************/
/*------------------------------------------------------------------*/
/*! @enum MM_SVC_REPEAT_TYPE_T
 *  @brief Repeat type.
 *  @code
 *  typedef enum
 *  {
 *      MM_SVC_REPEAT_ALL = 1,
 *      MM_SVC_REPEAT_CHAPTER,
 *      MM_SVC_REPEAT_TITLE,
 *      MM_SVC_REPEAT_AB,
 *      MM_SVC_REPEAT_OFF
 *  } MM_SVC_REPEAT_TYPE_T;
 *  @endcode
 *  @li@c  MM_SVC_REPEAT_ALL                                 - All.
 *  @li@c  MM_SVC_REPEAT_CHAPTER                             - Chapter.
 *  @li@c  MM_SVC_REPEAT_TITLE                               - Title.
 *  @li@c  MM_SVC_REPEAT_AB                                  - A to B.
 *  @li@c  MM_SVC_REPEAT_OFF                                 - Off.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_SVC_REPEAT_ALL = 1,/*file or channel*/
    MM_SVC_REPEAT_CHAPTER,
    MM_SVC_REPEAT_TITLE,
    MM_SVC_REPEAT_AB,
    MM_SVC_REPEAT_OFF
} MM_SVC_REPEAT_TYPE_T;

/********************* Multimedia Duration Type ******************************/
/*------------------------------------------------------------------*/
/*! @enum MM_SVC_DUR_TYPE_T
 *  @brief Duration type.
 *  @code
 *  typedef enum
 *  {
 *      MM_SVC_DUR_TYPE_TITLE = 1,
 *      MM_SVC_DUR_TYPE_CHAPTER,
 *      MM_SVC_DUR_TYPE_ALL,
 *      MM_SVC_DUR_TYPE_TITLE_OFFSET,
 *      MM_SVC_DUR_TYPE_CHAPTER_OFFSET,
 *      MM_SVC_DUR_TYPE_ALL_OFFSET,
 *      MM_SVC_DUR_TYPE_BUFFERED
 *  } MM_SVC_DUR_TYPE_T;
 *  @endcode
 *  @li@c  MM_SVC_DUR_TYPE_TITLE                             - Title.
 *  @li@c  MM_SVC_DUR_TYPE_CHAPTER                           - Chapter.
 *  @li@c  MM_SVC_DUR_TYPE_ALL                               - All.
 *  @li@c  MM_SVC_DUR_TYPE_TITLE_OFFSET                      - Title offset.
 *  @li@c  MM_SVC_DUR_TYPE_CHAPTER_OFFSET                    - Chapter offset.
 *  @li@c  MM_SVC_DUR_TYPE_ALL_OFFSET                        - All offset.
 *  @li@c  MM_SVC_DUR_TYPE_BUFFERED                          - Buffered.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_SVC_DUR_TYPE_TITLE = 1,
    MM_SVC_DUR_TYPE_CHAPTER,
    MM_SVC_DUR_TYPE_ALL, /*file or channel*/
    MM_SVC_DUR_TYPE_TITLE_OFFSET,
    MM_SVC_DUR_TYPE_CHAPTER_OFFSET,
    MM_SVC_DUR_TYPE_ALL_OFFSET,
    MM_SVC_DUR_TYPE_BUFFERED
} MM_SVC_DUR_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum MM_SVC_TM_CODE_TYPE_T
 *  @brief Time code type.
 *  @code
 *  typedef enum
 *  {
 *      MM_SVC_TM_CODE_TYPE_TITLE = 1,
 *      MM_SVC_TM_CODE_TYPE_CHAPTER,
 *      MM_SVC_TM_CODE_TYPE_ALL
 *  } MM_SVC_TM_CODE_TYPE_T;
 *  @endcode
 *  @li@c  MM_SVC_TM_CODE_TYPE_TITLE                         - Title.
 *  @li@c  MM_SVC_TM_CODE_TYPE_CHAPTER                       - Chapter.
 *  @li@c  MM_SVC_TM_CODE_TYPE_ALL                           - All.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_SVC_TM_CODE_TYPE_TITLE = 1,
    MM_SVC_TM_CODE_TYPE_CHAPTER,
    MM_SVC_TM_CODE_TYPE_ALL /*file or channel*/
} MM_SVC_TM_CODE_TYPE_T;

/******************* Multimedia  Button Type **********************************/
/*------------------------------------------------------------------*/
/*! @enum MM_SVC_BTN_TYPE_T
 *  @brief Button type.
 *  @code
 *  typedef enum
 *  {
 *      MM_SVC_BTN_TYPE_UP = 1,
 *      MM_SVC_BTN_TYPE_DOWN,
 *      MM_SVC_BTN_TYPE_LEFT,
 *      MM_SVC_BTN_TYPE_RIGHT,
 *      MM_SVC_BTN_TYPE_ENTER,
 *      MM_SVC_BTN_TYPE_STARTING_MENU,
 *      MM_SVC_BTN_TYPE_ROOT_MENU,
 *      MM_SVC_BTN_TYPE_TITLE_MENU,
 *      MM_SVC_BTN_TYPE_CHAPTER_MENU,
 *      MM_SVC_BTN_TYPE_MENU_ENDING_PLAYBACK,
 *      MM_SVC_BTN_TYPE_SELECT_STREAM
 *  } MM_SVC_BTN_TYPE_T;
 *  @endcode
 *  @li@c  MM_SVC_BTN_TYPE_UP                                - Up.
 *  @li@c  MM_SVC_BTN_TYPE_DOWN                              - Down.
 *  @li@c  MM_SVC_BTN_TYPE_LEFT                              - Left.
 *  @li@c  MM_SVC_BTN_TYPE_RIGHT                             - Right.
 *  @li@c  MM_SVC_BTN_TYPE_ENTER                             - Enter.
 *  @li@c  MM_SVC_BTN_TYPE_STARTING_MENU                     - Start Menu.
 *  @li@c  MM_SVC_BTN_TYPE_ROOT_MENU                         - Root Menu.
 *  @li@c  MM_SVC_BTN_TYPE_TITLE_MENU                        - Title Menu.
 *  @li@c  MM_SVC_BTN_TYPE_CHAPTER_MENU                      - Chapter Menu.
 *  @li@c  MM_SVC_BTN_TYPE_MENU_ENDING_PLAYBACK              - Menu ending play.
 *  @li@c  MM_SVC_BTN_TYPE_SELECT_STREAM                     - Select stream.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_SVC_BTN_TYPE_UP = 1,
    MM_SVC_BTN_TYPE_DOWN,
    MM_SVC_BTN_TYPE_LEFT,
    MM_SVC_BTN_TYPE_RIGHT,
    MM_SVC_BTN_TYPE_ENTER,
    MM_SVC_BTN_TYPE_STARTING_MENU, /*return back the first present menu "WMEN" */
    MM_SVC_BTN_TYPE_ROOT_MENU,     /*return back the root menu "WMEN" */
    MM_SVC_BTN_TYPE_TITLE_MENU, /*not define yet for divx*/
    MM_SVC_BTN_TYPE_CHAPTER_MENU,
    MM_SVC_BTN_TYPE_MENU_ENDING_PLAYBACK,
    MM_SVC_BTN_TYPE_SELECT_STREAM, /*return tex*/
    MM_SVC_BTN_TYPE_SELECT_DIGTS,
    MM_SVC_BTN_TYPE_SELECT_NEXT,
    MM_SVC_BTN_TYPE_SELECT_PREV
} MM_SVC_BTN_TYPE_T;


typedef enum
{
    SVCTX_NTFY_CODE_MEDIA_IS_BUFFERING2PLAY =1 
    /*to be extend*/
}
SVCTX_BUFCTL_NTFY_CODE_T;
/******************* Multimedia ZOOM Type **********************************/

/*typedef enum
{

} MM_SVC_ZOOM_TYPE_T;
*/


/********************* Video Info *********************************/
/*------------------------------------------------------------------*/
/*! @struct MM_VIDEO_INFO_T
 *  @brief Video information.
 *  @code
 *  typedef struct _MM_VIDEO_INFO_T
 *  {
 *     UINT32               ui4_width;
 *     UINT32               ui4_height;
 *  } MM_VIDEO_INFO_T;
 *  @endcode
 *  @li@c  ui4_width                                         - Width.
 *  @li@c  ui4_height                                        - Height.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_VIDEO_INFO_T
{
   UINT32               ui4_width;
   UINT32               ui4_height;    

   UINT8                ui1_par_w;
   UINT8                ui1_par_h;
   BOOL                 b_src_asp;
} MM_VIDEO_INFO_T;

typedef struct _MM_PUSH_BUF_INFO_T
{
   UINT8                ui1_id;
   UINT32               ui4_buf_size;
   UINT8*               pu1_push_buf;
} MM_PUSH_BUF_INFO_T;

#if (defined(MM_LAST_MEMORY_SUPPORT) || MM_AB_REPEAT_SUPPORT)
/*------------------------------------------------------------------*/
/*! @struct MM_SVC_RENDER_INFO_T
 *  @brief Render information.
 *  @code
 *  typedef struct _MM_SVC_RENDER_INFO_T
 *  {
 *      UINT64  ui8_pts_info;
 *      UINT64  ui8_i_frame_position;
 *      UINT64  ui8_frame_position;
 *      INT32   i4_temporal_reference;
 *      UINT16  ui2_decoding_order;
 *  }MM_SVC_RENDER_INFO_T;
 *  @endcode
 *  @li@c  ui8_pts_info                                      - Pts infor.
 *  @li@c  ui8_i_frame_position                              - I frame position.
 *  @li@c  ui8_frame_position                                - Frame position.
 *  @li@c  i4_temporal_reference                             - Temp reference.
 *  @li@c  ui2_decoding_order                                - Decoding order.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_SVC_RENDER_INFO_T
{
    UINT64  ui8_stc;
    UINT64  ui8_aud_pts_info;
    UINT64  ui8_aud_frame_position;
    UINT64  ui8_i_pts_info;
    UINT64  ui8_pts_info;
    UINT64  ui8_i_frame_position;
    UINT64  ui8_frame_position;
    UINT64  ui8_frame_position_disp;
    INT32   i4_temporal_reference;
    UINT16  ui2_decoding_order;
    UINT32  ui4_timestap;
}MM_SVC_RENDER_INFO_T;
#endif
/********************* Multimedia Search Info *******************************
 * t_seek_type :
 *      represent the search type.
 * ui2_idx :
 *      represent the CH,TT,CHANNEL number,if the
 *      t_seek_type is ALL ,the value should be zero.
 * ui2_pos_val :
 *      represent the time value in seconds or file offest in bytes.
 *      if this value is zero , it means CH,TT,CHANNEL,or file start point.
 *      in Timeshift playback, it represents the time value in tick number.
 ****************************************************************************/
/*------------------------------------------------------------------*/
/*! @struct MM_SVC_POS_INFO_T
 *  @brief Service position information.
 *  @code
 *  typedef struct _MM_SVC_POS_INFO_T
 *  {
 *      MM_SVC_POS_TYPE_T     t_pos_type;
 *      UINT16                ui2_idx;
 *      UINT64                ui8_pos_val;
 *      union
 *      {
 *          MM_SVC_RENDER_INFO_T render_info;
 *      } u;
 *  } MM_SVC_POS_INFO_T;
 *  @endcode
 *  @li@c  t_pos_type                                        - Position type.
 *  @li@c  ui2_idx                                           - Index.
 *  @li@c  ui8_pos_val                                       - Position value.
 *  @li@c  render_info                                       - Render infor.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_SVC_POS_INFO_T
{
    MM_SVC_POS_TYPE_T     t_pos_type;      /*(I)*/
    UINT16                ui2_idx;         /*(I)*/
    UINT64                ui8_pos_val;     /*(I)*/
#if (defined(MM_LAST_MEMORY_SUPPORT) || MM_AB_REPEAT_SUPPORT)
    union
    {
        MM_SVC_RENDER_INFO_T render_info;
    } u;
#endif
} MM_SVC_POS_INFO_T;

#if 1/*MKV DivXPlus*/
typedef struct _MM_SVC_CUR_CHAP_ID_T
{
    UINT32                  ui4_playlist_id;
    UINT32                  ui4_chapter_id;
}MM_SVC_CUR_CHAP_ID_T;
#endif
/********************** Multimedia Duration Info *****************************
 * t_dur_type :
 *      represent the duration type.
 * ui4_tt_ch_num :
 *      represent the CH,TT,number,if the
 *      t_srh_type is ALL(Channle or File),the value should be zero.
 * ui2_pos_val_2 :
 *      retrieve the total time duration value in seconds.
 ****************************************************************************/
/*------------------------------------------------------------------*/
/*! @struct MM_SVC_DUR_INFO_T
 *  @brief Service Duration information.
 *  @code
 *  typedef struct _MM_SVC_DUR_INFO_T
 *  {
 *     MM_SVC_DUR_TYPE_T    t_dur_type;
 *     UINT16               ui2_idx;
 *     UINT32               ui4_duration;
 *  } MM_SVC_DUR_INFO_T;
 *  @endcode
 *  @li@c  t_dur_type                                        - Duration type.
 *  @li@c  ui2_idx                                           - Index.
 *  @li@c  ui4_duration                                      - Duration.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_SVC_DUR_INFO_T
{
   MM_SVC_DUR_TYPE_T    t_dur_type;    /*(I)*/
   UINT16               ui2_idx;       /*(I)*/
   UINT64               ui8_duration;  /*(O)*/
} MM_SVC_DUR_INFO_T;


/********************** Multimedia Timecode Info ******************************
 *t_tm_code_type:
 *      represent the time code type
 *ui2_idx:
 *      represent the CH,TT number , if the t_tm_code_type is
 *      ALL(Channel or File),the value should be zero
 *pui4_time
 *      retrieve the time offest.
 ******************************************************************************/
/*------------------------------------------------------------------*/
/*! @struct MM_SVC_TIME_CODE_INFO_T
 *  @brief Service time code information.
 *  @code
 *  typedef struct _MM_SVC_TIME_CODE_INFO_T
 *  {
 *     MM_SVC_TM_CODE_TYPE_T   t_tm_code_type;
 *     UINT16                  ui2_idx;
 *     UINT64                  ui8_time;
 *  } MM_SVC_TIME_CODE_INFO_T;
 *  @endcode
 *  @li@c  t_tm_code_type                                    - Time code type.
 *  @li@c  ui2_idx                                           - Index.
 *  @li@c  ui8_time                                          - Time.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_SVC_TIME_CODE_INFO_T
{
   MM_SVC_TM_CODE_TYPE_T   t_tm_code_type;   /*(I)*/
   UINT16                  ui2_idx;          /*(I)*/
   UINT64                  ui8_time;         /*(O)*/
} MM_SVC_TIME_CODE_INFO_T;

/********************** Multimedia PTS Info ******************************
 *ui8_aud_pts:
 *      aud pts value
 *ui8_vid_pts:
 *      video pts value
 ******************************************************************************/
/*------------------------------------------------------------------*/
/*! @struct MM_SVC_PTS_INFO_T
 *  @brief Service pts information.
 *  @code
 *  typedef struct _MM_SVC_PTS_INFO_T
 *  {
 *     UINT64                  ui8_aud_pts;        
 *     UINT64                  ui8_vid_pts;       
 *  } MM_SVC_PTS_INFO_T;
 *  @endcode
 *  @li@c  ui8_aud_pts                                    - audio pts.
 *  @li@c  ui8_vid_pts                                     - video pts.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_SVC_PTS_INFO_T
{
   UINT64                  ui8_aud_pts;         /*(O)*/
   UINT64                  ui8_vid_pts;         /*(O)*/
} MM_SVC_PTS_INFO_T;

/* ****************** Multimedia Repeat Info *********************************/
#if MM_AB_REPEAT_SUPPORT
/*------------------------------------------------------------------*/
/*! @enum MM_SVC_REPEAT_AB_INFO
 *  @brief AB repeat setting type infomation.
 *  @code
 *  typedef enum
 *  {
 *      MM_SVC_REPEAT_AB_INFO_NONE = 0,
 *      MM_SVC_REPEAT_AB_INFO_SET_A,
 *      MM_SVC_REPEAT_AB_INFO_SET_B,
 *      MM_SVC_REPEAT_AB_INFO_CANCEL_ALL,
 *  } MM_SVC_REPEAT_AB_INFO;
 *  @endcode
 *  @li@c  MM_SVC_REPEAT_AB_INFO_NONE                                - None.
 *  @li@c  MM_SVC_REPEAT_AB_INFO_SET_A                              - Set a point.
 *  @li@c  MM_SVC_REPEAT_AB_INFO_SET_B                              - Set b point.
 *  @li@c  MM_SVC_REPEAT_AB_INFO_CANCEL_ALL                     - Cancel all point.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_SVC_REPEAT_AB_TYPE_NONE = 0,
    MM_SVC_REPEAT_AB_TYPE_SET_A,
    MM_SVC_REPEAT_AB_TYPE_SET_B,
    MM_SVC_REPEAT_AB_TYPE_CANCEL_ALL
} MM_SVC_REPEAT_AB_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct MM_REPEAT_AB_INFO_T;
 *  @brief Repeat a/b position infomation.
 *  @code
 *  typedef struct _MM_REPEAT_AB_INFO_T
 *  {
 *      BOOL                            b_set_repeat_a;
 *      BOOL                            b_set_repeat_b;
 *      MM_SVC_POS_INFO_T    t_repeat_info_a;
 *      MM_SVC_POS_INFO_T    t_repeat_info_b;
 *  } MM_REPEAT_AB_INFO_T;
 *  @endcode
 *  @li@c  b_set_repeat_a                                     - If it is A set.
 *  @li@c  b_set_repeat_a                                     - If it is B set.
 *  @li@c  t_repeat_info_a                                     - Repeat a position.
 *  @li@c  t_repeat_info_b                                     - Repeat b position.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_REPEAT_AB_INFO_T
{
    BOOL                 b_set_repeat_a;
    BOOL                 b_set_repeat_b;
    BOOL                 b_seeking;
    MM_SVC_POS_INFO_T    t_repeat_info_a;
    MM_SVC_POS_INFO_T    t_repeat_info_b;
} MM_REPEAT_AB_INFO_T;

#endif

/*------------------------------------------------------------------*/
/*! @struct MM_SVC_REPEAT_INFO_T;
 *  @brief Repeat type.
 *  @code
 *  typedef struct _MM_SVC_REPEAT_INFO_T
 *  {
 *      MM_SVC_REPEAT_TYPE_T    t_repeat_type;
 *      union
 *      {
 *          MM_SVC_REPEAT_AB_TYPE_T   e_repeat_ab_type;
 *      } u;
 *  } MM_SVC_REPEAT_INFO_T;
 *  @endcode
 *  @li@c  t_repeat_type                                     - Type.
 *  @li@c  e_repeat_ab_type                               - Repeat ab sub-type.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_SVC_REPEAT_INFO_T
{
    MM_SVC_REPEAT_TYPE_T    t_repeat_type;  /*(I)*/
#if MM_AB_REPEAT_SUPPORT
    union
    {
        MM_SVC_REPEAT_AB_TYPE_T   e_repeat_ab_type;
    } u;
#endif
} MM_SVC_REPEAT_INFO_T;

#ifdef SUPPORT_SET_SBTL_ENCODING
typedef struct _MM_SVC_SUBTITLE_INFO_T
{
    union
    {
        BOOL                    b_is_extra_subtitle;
        MM_SBTL_ENCODING_T      t_mm_sbtl_encoding;
    } u;
} MM_SVC_SUBTITLE_INFO_T;
#endif

/********************* Multimedia Trick Info **********************************/
/*------------------------------------------------------------------*/
/*! @struct MM_SVC_TRICK_INFO_T
 *  @brief Speed type.
 *  @code
 *  typedef struct _MM_SVC_TRICK_INFO_T
 *  {
 *      MM_SPEED_TYPE_T     e_speed_type;
 *  } MM_SVC_TRICK_INFO_T;
 *  @endcode
 *  @li@c  e_speed_type                                      - Speed type.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_SVC_TRICK_INFO_T
{
    MM_SPEED_TYPE_T     e_speed_type;      /*(I)*/
    union
    {
        BOOL         b_boolean;
        UINT32      ui4_data;
    } u;
} MM_SVC_TRICK_INFO_T;

/********************* Multimedia CHPATER Info **********************************/
/*------------------------------------------------------------------*/
/*! @struct MM_SVC_CHAPTER_INFO_T
 *  @brief Service chapter information.
 *  @code
 *  typedef struct _MM_SVC_CHAPTER_INFO_T
 *  {
 *      UINT16   ui2_ch_total_ns;
 *      UINT16   ui2_curr_ch_num;
 *  } MM_SVC_CHAPTER_INFO_T;
 *  @endcode
 *  @li@c  ui2_ch_total_ns                                   - Chapter total time in ns.
 *  @li@c  ui2_curr_ch_num                                   - Current chapter number.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_SVC_CHAPTER_INFO_T
{
    UINT16   ui2_ch_total_ns;
    UINT16   ui2_curr_ch_num;
} MM_SVC_CHAPTER_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct MM_SVC_TITLE_INFO_T
 *  @brief Service title information.
 *  @code
 *  typedef struct _MM_SVC_TITLE_INFO_T
 *  {
 *      UINT16   ui2_tt_ns;
 *      UINT16   ui2_curr_tt_num;
 *  } MM_SVC_TITLE_INFO_T;
 *  @endcode
 *  @li@c  ui2_tt_ns                                         - Title number.
 *  @li@c  ui2_curr_tt_num                                   - Current title number.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_SVC_TITLE_INFO_T
{
    UINT16   ui2_tt_ns;
    UINT16   ui2_curr_tt_num;
} MM_SVC_TITLE_INFO_T;


/********************* Multimedia Button Info **********************************/
/*------------------------------------------------------------------*/
/*! @struct MM_SVC_BTN_INFO_T
 *  @brief Service button type.
 *  @code
 *  typedef struct _MM_SVC_BTN_INFO_T
 *  {
 *      MM_SVC_BTN_TYPE_T   t_btn_type;
  *      union
 *      {
 *          UINT32                    ui4_value;
 *          CHAR*                    ps_next;
 *      }u;
 *  } MM_SVC_BTN_INFO_T;
 *  @endcode
 *  @li@c  t_btn_type                                        - Button type.
  *  @li@c  ui4_value                                      - auto chapter value 0~9.
 *  @li@c  ps_next                                      - Reserved.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_SVC_BTN_INFO_T
{
    MM_SVC_BTN_TYPE_T   t_btn_type;      /*(I)*/
    union
    {
        UINT32                    ui4_value;  /*(I)*/
        CHAR*                     ps_next;    /*(I)*/
    }u;
} MM_SVC_BTN_INFO_T;

typedef UINT32 (*x_svctx_buffctrl_nfy_fct)(
                    HANDLE_T                                 h_svctx,          /* in  */
                    SVCTX_BUFCTL_NTFY_CODE_T   e_code,            /* in  */
                    VOID*                                       pv_nfy_tag,       /* in  */
                    UINT32                                      ui4_data           /*in , ptr with carry inform*/
                    );

typedef enum
{
    MM_SVC_BUFFCTRL_TYPE_BYTE = 1,
    MM_SVC_BUFFCTRL_TYPE_SEC
 } MM_SVC_BUFFCTRL_TYPE_T;


/********************* Btn Action Type ******************************/
/*------------------------------------------------------------------*/
/*! @enum MM_SVC_BTN_ACTION_TYPE_T
 *  @brief Btn Type.
 *  @code
 *  typedef enum
 *  {
 *      MM_SVC_BTN_ACTION_TYPE_SEEK = 1,
 *      MM_SVC_BTN_ACTION_TYPE_TEXT_DISPLAY,
 *  } MM_SVC_BTN_ACTION_TYPE_T;
 *  @endcode
 *  @li@c  MM_SVC_BTN_ACTION_TYPE_SEEK                             - auto chapter select.
 *  @li@c  MM_SVC_BTN_ACTION_TYPE_TEXT_DISPLAY               - Reserved.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_SVC_BTN_ACTION_TYPE_SEEK = 1,
    MM_SVC_BTN_ACTION_TYPE_TEXT_DISPLAY
    /*to be extend*/
}MM_SVC_BTN_ACTION_TYPE_T;

/******************* Auto chapter Button Action Info**********************************/
/*------------------------------------------------------------------*/
/*! @enum MM_SVC_BTN_ACTION_INFO_T
 *  @brief Button Action Info.
 *  @code
 *  typedef struct _MM_SVC_BTN_INFO_T
 *  {
 *      MM_SVC_BTN_TYPE_T                t_btn_type;
 *      MM_SVC_BTN_ACTION_TYPE_T        t_btn_act_type;
 *      union
 *      {
 *          UINT32                    ui4_value;
 *          CHAR*                    ps_next;
 *      }u;
 *  }MM_SVC_BTN_INFO_T;
 *  @endcode
 *  @li@c  MM_SVC_BTN_TYPE_T                                    - BTN type.
 *  @li@c  MM_SVC_BTN_ACTION_TYPE_T                        -return BTN action type.
 *  @li@c  ui4_value                                              - auto chapter value 0~9.
 *  @li@c  ps_next                                              - Reserved.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_SVC_BTN_ACTION_INFO_T
{
    MM_SVC_BTN_INFO_T            t_btn_type;
    MM_SVC_BTN_ACTION_TYPE_T    t_btn_act_type;
    union
    {
        UINT32                    ui4_value;
        CHAR*                    ps_next;
    }u;
}MM_SVC_BTN_ACTION_INFO_T;

typedef struct _MM_SVC_BUFFCTRL_INFO_T
{
  /*------ DO NOT ADD NEW MEMBER HERE---- start --*/ 
   union { 
   UINT32   ui4_bytes;  
   UINT32   ui4_duration; 
   }  ui4_buf_size ;
   UINT32  ui4_keepbuf_threshold;                    /*I*/ 
   UINT32  ui4_rebuf_threshold;                     /*I*/
  /*------ DO NOT ADD NEW MEMBER HERE---- end --*/ 
   MM_SVC_BUFFCTRL_TYPE_T e_ctrl_type;
   BOOL    b_is_prebuff;                            /*I*/
   UINT32  ui4_buff_fullness;                       /*O*/
   UINT32  ui4_buff_emptyness;                      /*O*/   
   x_svctx_buffctrl_nfy_fct       pf_nfy;           /*I*/  
} MM_SVC_BUFFCTRL_INFO_T;

typedef struct _MM_SVC_TIME_TO_SIZE_T
{
    UINT32  ui4_ms; /*IN: time in miliseconds*/
    UINT32  ui4_size; /*OUT: size in bytes*/
} MM_SVC_TIME_TO_SIZE_T;

typedef struct _MM_SVC_BUF_STATUS_T
{
    UINT64  ui8_begin_pos;
    UINT64  ui8_end_pos;
    UINT64  ui8_data_ofst; /* Need minus header size*/
} MM_SVC_BUF_STATUS_T;

/********************* For Marlin Mp4 Use  **************************/
typedef enum
{
    MM_DRM_TYPE_UNKNOWN  = 0,
    MM_DRM_TYPE_DIVX_DRM,
    MM_DRM_TYPE_MLN
} MM_DRM_TYPE_T;

typedef struct 
{
    HANDLE_T        h_timer;
    UINT32          ui4_max_view_time;
} MM_DRM_CTRL_INFO_T;

typedef struct 
{
    UINT8*              pui1_act_tkn_url;                  /* action token url from CAD file */
    UINT16              ui2_act_tkn_url_length;            /* length of action token url, include null charater '\0' */
    UINT8*              pui1_afl_tkn;                      /* affiliation token from CAD file */
    UINT16              ui2_afl_tkn_length;                /* length of affiliation token, include null charater '\0' */
} MM_MLN_DRM_INFO_T;

typedef struct 
{
    MM_DRM_TYPE_T       e_drm_type;
    union
    {
        MM_MLN_DRM_INFO_T   t_mln_drm_inf;
    }u;
} MM_DRM_INFO_T;

/********************* Multimedia Info *********************************/
/*------------------------------------------------------------------*/
/*! @struct SRC_MM_INFO_T
 *  @brief MM information
 *  @code
 *  typedef struct _SRC_MM_INFO_T
 *  {
 *      HANDLE_T                        h_media;
 *      MM_SPEED_TYPE_T                 t_speed;
 *      MM_SVC_POS_INFO_T               t_pos_info;
 *      MM_SVC_BUFFCTRL_INFO_T    t_bufctrl_info;
 *  #if 0
 *      UINT16                          ui2_svc_lst_id;
 *      UINT16                          ui2_svl_rec_id;
 *  #endif
 *  } SRC_MM_INFO_T;
 *  @endcode
 *  @li@c  h_media                                           - Media handle.
 *  @li@c  t_speed                                           - Speed.
 *  @li@c  t_pos_info                                        - Position information.
 *  @li@c  ui2_svc_lst_id                                    - Service list ID.
 *  @li@c  ui2_svl_rec_id                                    - Service record ID.
 */
/*------------------------------------------------------------------*/
typedef struct _SRC_MM_INFO_T
{
    HANDLE_T                        h_media;
    MM_SPEED_TYPE_T                 t_speed;
    MM_SVC_POS_INFO_T               t_pos_info;
    MM_SVC_BUFFCTRL_INFO_T          t_bufctrl_info;
    BOOL                            b_still_at_last;
    BOOL                            b_sub_free_run;
    /* TS file Program ID */
    UINT16                          ui2_pmt_idx;
    UINT16                          ui2_svc_pid;
#if 1/*MKV DivXPlus*/
    UINT32                          ui4_title_idx;
    UINT32                          ui4_playlist_idx;
    UINT32                          ui4_chap_idx;
#endif
    MM_DRM_INFO_T                   t_ml_drm;
#if 0 /* disuse */
    /* TS file */
    UINT16                          ui2_svc_lst_id;
    UINT16                          ui2_svl_rec_id;
#endif
} SRC_MM_INFO_T;

#ifdef TIME_SHIFT_SUPPORT
/* SVCTX_MM_GET_TYPE_TIMESHIFT_RANGE_INFO data info *************************/
/*------------------------------------------------------------------*/
/*! @enum MM_TIMESHIFT_RANGE_INFO_TYPE_T
 *  @brief Time shift range information type.
 *  @code
 *  typedef enum
 *  {
 *      MM_TIMESHIFT_RANGE_INFO_TYPE_TICK = 1,
 *  } MM_TIMESHIFT_RANGE_INFO_TYPE_T;
 *  @endcode
 *  @li@c  MM_TIMESHIFT_RANGE_INFO_TYPE_TICK                 - Type tick.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MM_TIMESHIFT_RANGE_INFO_TYPE_TICK = 1,
    /*MM_TIMESHIFT_RANGE_INFO_TYPE_LBA*/
} MM_TIMESHIFT_RANGE_INFO_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct MM_TIMESHIFT_RANGE_INFO_T
 *  @brief Time shift range information.
 *  @code
 *  typedef struct _MM_TIMESHIFT_RANGE_INFO_T
 *  {
 *     MM_TIMESHIFT_RANGE_INFO_TYPE_T   e_type;
 *     UINT32                           ui4_start;
 *     UINT32                           ui4_end;
 *     UINT32                           ui4_current;
 *     BOOL                             b_bad_tick;
 *     TIME_T                           t_curr_sys_time;
 *  } MM_TIMESHIFT_RANGE_INFO_T;
 *  @endcode
 *  @li@c  e_type                                            - Type.
 *  @li@c  ui4_start                                         - Start tick.
 *  @li@c  ui4_end                                           - End tick.
 *  @li@c  ui4_current                                       - Current tick.
 *  @li@c  b_bad_tick                                        - Is bad tick.
 *  @li@c  t_curr_sys_time                                   - Current system time.
 */
/*------------------------------------------------------------------*/
typedef struct _MM_TIMESHIFT_RANGE_INFO_T
{
   MM_TIMESHIFT_RANGE_INFO_TYPE_T   e_type;            /*(I)*/
   UINT32                           ui4_start;         /*(O) in tick index*/
   UINT32                           ui4_end;           /*(O) in tick index*/
   UINT32                           ui4_current;       /*(O) in tick index*/
   BOOL                             b_bad_tick;        /*(O)*/
   TIME_T                           t_curr_sys_time;   /*(O)*/
} MM_TIMESHIFT_RANGE_INFO_T;
#endif

/********************* Multimedia Specific Stream Configuraton ****************/

/*------------------------------------------------------------------*/
/*! @struct MM_STRM_ATRBT_T
 *  @brief Stream attribute.
 *
 *  @code
 *  typedef struct _MM_STRM_ATRBT_T
 *  {
 *      STREAM_TYPE_T                       e_type;
 *      MINFO_TYPE_STM_ATRBT_T              t_stm_atrbt;
 *  } MM_STRM_ATRBT_T;
 *  @endcode
 *
 *  @li@c  e_type                                          - Type.
 *  @li@c  t_stm_atrbt                                     - Attribute information.
 */
/*------------------------------------------------------------------*/

typedef struct _MM_STRM_ATRBT_T
{
    STREAM_TYPE_T                       e_type;
    MINFO_TYPE_STM_ATRBT_T              t_stm_atrbt;
} MM_STRM_ATRBT_T;

/*------------------------------------------------------------------*/
/*! @struct MM_STRM_INFO_T
 *  @brief MM stream information.
 *
 *  @code
 *  typedef struct _MM_STRM_INFO_T
 *  {
 *      MEDIA_FORMAT_T                      t_format;
 *      UINT16                              ui2_num_stm;
 *      MM_STRM_ATRBT_T*                    pt_stm_atrbt;
 *  } MM_STRM_INFO_T;
 *  @endcode
 *
 *  @li@c  t_format                                          - Media format.
 *  @li@c  ui2_num_stm                                       - Number of stream.
 *  @li@c  pt_stm_atrbt                                      - Stream attribute.
 */
/*------------------------------------------------------------------*/

#define SUPPORTED_STRM_NUM_MAX          (1+8+8)/*1 Video + 8 Audio + 8 Subtitle*/
typedef struct _MM_STRM_INFO_T
{
    MEDIA_FORMAT_T                      t_format;
    /* we can not support more than one video or audio or subtilte stream */
    UINT16                              ui2_num_stm;
    MM_STRM_ATRBT_T                     at_stm_atrbt[SUPPORTED_STRM_NUM_MAX];
} MM_STRM_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct MM_3D_VID_INFO_T
 *  @brief 3D vid stream information.
 *
 *  @code
 *  typedef struct _MM_3D_VID_INFO_T
 *  {
 *      MM_SRC_TAG3D_TYPE_T                e_tag3d_type;
 *      
 *  } MM_STRM_INFO_T;
 *  @endcode
 *
 *  @li@c  MM_SRC_TAG3D_TYPE_T                e_tag3d_type.
 *
 */
/*------------------------------------------------------------------*/
typedef struct _MM_3D_VID_INFO_T
{
    MM_SRC_TAG3D_TYPE_T                e_tag3d_type;
} MM_3D_VID_INFO_T;

typedef struct _MM_STRM_CODEC_INFO_T
{
    UINT8       ui1_strm_idx;
    UINT32      ui4_codec_info_size;
    UINT8       aui1_codec_info[2048];
}MM_STRM_CODEC_INFO_T;

typedef struct _BUF_AGT_FLOW_CTRL_T
{    
    UINT32  ui4_buffer_len;             /*Buffer size in Bytes. */    
    UINT32  ui4_keep_buffer_threshold;  /*Keep threshold in Bytes, at least keep this size of data which can used by seek backward. */    
    UINT32  ui4_re_buferf_threshold;    /*Re-Buffer threshold in Bytes, at least this size of space before pulling data from Application */
} MM_BUF_AGT_FLOW_CTRL_T;

typedef struct _MM_SVC_STOP_INFO
{
    UINT64              ui8_clip_id;
    UINT32              ui4_title_idx;
    UINT32              ui4_playlist_idx;
    UINT32              ui4_chap_idx;
    MM_SVC_POS_INFO_T   t_mm_svc_pos_info;
} MM_SVC_STOP_INFO;    

typedef enum 
{
    ID_CNDTN_UNKNOWN,
    ID_CNDTN_SRC,
    ID_CNDTN_END
}ID_CNDTN_TYPE;

typedef struct _ID_CVT_CNDTN_T
{
    ID_CNDTN_TYPE e_cndtn_type;
    union
    {
        UINT8 t_src_val;
    }u;
}ID_CVT_CNDTN_T;
#endif /* _U_MM_HDLR_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                              /* end of groupMW_SVCTX_MM_HDLR*/
/*----------------------------------------------------------------------------*/


